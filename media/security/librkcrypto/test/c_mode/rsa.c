/*
 *  The RSA public-key cryptosystem
 *
 *  Copyright (C) 2006-2015, ARM Limited, All Rights Reserved
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  This file is part of mbed TLS (https://tls.mbed.org)
 */
/*
 *  The following sources were referenced in the design of this implementation
 *  of the RSA algorithm:
 *
 *  [1] A method for obtaining digital signatures and public-key cryptosystems
 *      R Rivest, A Shamir, and L Adleman
 *      http://people.csail.mit.edu/rivest/pubs.html#RSA78
 *
 *  [2] Handbook of Applied Cryptography - 1997, Chapter 8
 *      Menezes, van Oorschot and Vanstone
 *
 */


//#include "oid.h"

#include <string.h>
#include "rsa.h"
#include "config.h"
#include "mbed_rsa_key.h"
#if defined(MBEDTLS_GENPRIME)

/*
 * Generate an RSA keypair
 */
int mbedtls_rsa_gen_key( mbedtls_rsa_context *ctx,
                 int (*f_rng)(void *, unsigned char *, size_t),
                 void *p_rng,
                 unsigned int nbits, int exponent )
{
    int ret;
    mbedtls_mpi P1, Q1, H, G;

    if( f_rng == NULL || nbits < 128 || exponent < 3 )
        return( MBEDTLS_ERR_RSA_BAD_INPUT_DATA );

    if( nbits % 2 )
        return( MBEDTLS_ERR_RSA_BAD_INPUT_DATA );

    mbedtls_mpi_init( &P1 ); mbedtls_mpi_init( &Q1 );
    mbedtls_mpi_init( &H ); mbedtls_mpi_init( &G );

    /*
     * find primes P and Q with Q < P so that:
     * GCD( E, (P-1)*(Q-1) ) == 1
     */
    MBEDTLS_MPI_CHK( mbedtls_mpi_lset( &ctx->E, exponent ) );

    do
    {
        MBEDTLS_MPI_CHK( mbedtls_mpi_gen_prime( &ctx->P, nbits >> 1, 0,
                                f_rng, p_rng ) );

        MBEDTLS_MPI_CHK( mbedtls_mpi_gen_prime( &ctx->Q, nbits >> 1, 0,
                                f_rng, p_rng ) );

        if( mbedtls_mpi_cmp_mpi( &ctx->P, &ctx->Q ) == 0 )
            continue;

        MBEDTLS_MPI_CHK( mbedtls_mpi_mul_mpi( &ctx->N, &ctx->P, &ctx->Q ) );
        if( mbedtls_mpi_bitlen( &ctx->N ) != nbits )
            continue;

        if( mbedtls_mpi_cmp_mpi( &ctx->P, &ctx->Q ) < 0 )
                                mbedtls_mpi_swap( &ctx->P, &ctx->Q );

        MBEDTLS_MPI_CHK( mbedtls_mpi_sub_int( &P1, &ctx->P, 1 ) );
        MBEDTLS_MPI_CHK( mbedtls_mpi_sub_int( &Q1, &ctx->Q, 1 ) );
        MBEDTLS_MPI_CHK( mbedtls_mpi_mul_mpi( &H, &P1, &Q1 ) );
        MBEDTLS_MPI_CHK( mbedtls_mpi_gcd( &G, &ctx->E, &H  ) );
    }
    while( mbedtls_mpi_cmp_int( &G, 1 ) != 0 );

    /*
     * D  = E^-1 mod ((P-1)*(Q-1))
     * DP = D mod (P - 1)
     * DQ = D mod (Q - 1)
     * QP = Q^-1 mod P
     */
    MBEDTLS_MPI_CHK( mbedtls_mpi_inv_mod( &ctx->D , &ctx->E, &H  ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_mod_mpi( &ctx->DP, &ctx->D, &P1 ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_mod_mpi( &ctx->DQ, &ctx->D, &Q1 ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_inv_mod( &ctx->QP, &ctx->Q, &ctx->P ) );

    ctx->len = ( mbedtls_mpi_bitlen( &ctx->N ) + 7 ) >> 3;

cleanup:

    mbedtls_mpi_free( &P1 ); mbedtls_mpi_free( &Q1 ); mbedtls_mpi_free( &H ); mbedtls_mpi_free( &G );

    if( ret != 0 )
    {
        mbedtls_rsa_free( ctx );
        return( MBEDTLS_ERR_RSA_KEY_GEN_FAILED + ret );
    }

    return( 0 );
}

#endif /* MBEDTLS_GENPRIME */

/*
 * Initialize an RSA context
 */
void mbedtls_rsa_init( mbedtls_rsa_context *ctx)
{
    memset( ctx, 0, sizeof( mbedtls_rsa_context ) );
}


/*
 * Check a public RSA key
 */
int mbedtls_rsa_check_pubkey( const mbedtls_rsa_context *ctx )
{
    if( !ctx->N.p || !ctx->E.p )
        return( MBEDTLS_ERR_RSA_KEY_CHECK_FAILED );

    if( ( ctx->N.p[0] & 1 ) == 0 ||
        ( ctx->E.p[0] & 1 ) == 0 )
        return( MBEDTLS_ERR_RSA_KEY_CHECK_FAILED );
    if( mbedtls_mpi_bitlen( &ctx->N ) < 128 ||
        mbedtls_mpi_bitlen( &ctx->N ) > MBEDTLS_MPI_MAX_BITS )
        return( MBEDTLS_ERR_RSA_KEY_CHECK_FAILED );

    if( mbedtls_mpi_bitlen( &ctx->E ) < 2 ||
        mbedtls_mpi_cmp_mpi( &ctx->E, &ctx->N ) >= 0 )
        return( MBEDTLS_ERR_RSA_KEY_CHECK_FAILED );

    return( 0 );
}

/*
 * Check a private RSA key
 */
int mbedtls_rsa_check_privkey( const mbedtls_rsa_context *ctx )
{
    int ret;
    mbedtls_mpi PQ, DE, P1, Q1, H, I, G, G2, L1, L2, DP, DQ, QP;

    if( ( ret = mbedtls_rsa_check_pubkey( ctx ) ) != 0 )
        return( ret );

    if( !ctx->P.p || !ctx->Q.p || !ctx->D.p )
        return( MBEDTLS_ERR_RSA_KEY_CHECK_FAILED );

    mbedtls_mpi_init( &PQ ); mbedtls_mpi_init( &DE ); mbedtls_mpi_init( &P1 ); mbedtls_mpi_init( &Q1 );
    mbedtls_mpi_init( &H  ); mbedtls_mpi_init( &I  ); mbedtls_mpi_init( &G  ); mbedtls_mpi_init( &G2 );
    mbedtls_mpi_init( &L1 ); mbedtls_mpi_init( &L2 ); mbedtls_mpi_init( &DP ); mbedtls_mpi_init( &DQ );
    mbedtls_mpi_init( &QP );

    MBEDTLS_MPI_CHK( mbedtls_mpi_mul_mpi( &PQ, &ctx->P, &ctx->Q ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_mul_mpi( &DE, &ctx->D, &ctx->E ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_sub_int( &P1, &ctx->P, 1 ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_sub_int( &Q1, &ctx->Q, 1 ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_mul_mpi( &H, &P1, &Q1 ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_gcd( &G, &ctx->E, &H  ) );

    MBEDTLS_MPI_CHK( mbedtls_mpi_gcd( &G2, &P1, &Q1 ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_div_mpi( &L1, &L2, &H, &G2 ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_mod_mpi( &I, &DE, &L1  ) );

    MBEDTLS_MPI_CHK( mbedtls_mpi_mod_mpi( &DP, &ctx->D, &P1 ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_mod_mpi( &DQ, &ctx->D, &Q1 ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_inv_mod( &QP, &ctx->Q, &ctx->P ) );
    /*
     * Check for a valid PKCS1v2 private key
     */
    if( mbedtls_mpi_cmp_mpi( &PQ, &ctx->N ) != 0 ||
        mbedtls_mpi_cmp_mpi( &DP, &ctx->DP ) != 0 ||
        mbedtls_mpi_cmp_mpi( &DQ, &ctx->DQ ) != 0 ||
        mbedtls_mpi_cmp_mpi( &QP, &ctx->QP ) != 0 ||
        mbedtls_mpi_cmp_int( &L2, 0 ) != 0 ||
        mbedtls_mpi_cmp_int( &I, 1 ) != 0 ||
        mbedtls_mpi_cmp_int( &G, 1 ) != 0 
	 )
    {
		ret = MBEDTLS_ERR_RSA_KEY_CHECK_FAILED;
    }

cleanup:
    mbedtls_mpi_free( &PQ ); mbedtls_mpi_free( &DE ); mbedtls_mpi_free( &P1 ); mbedtls_mpi_free( &Q1 );
    mbedtls_mpi_free( &H  ); mbedtls_mpi_free( &I  ); mbedtls_mpi_free( &G  ); mbedtls_mpi_free( &G2 );
    mbedtls_mpi_free( &L1 ); mbedtls_mpi_free( &L2 ); mbedtls_mpi_free( &DP ); mbedtls_mpi_free( &DQ );
    mbedtls_mpi_free( &QP );

    if( ret == MBEDTLS_ERR_RSA_KEY_CHECK_FAILED )
        return( ret );

    if( ret != 0 )
        return( MBEDTLS_ERR_RSA_KEY_CHECK_FAILED + ret );

    return( 0 );
}

/*
 * Check if contexts holding a public and private key match
 */
int mbedtls_rsa_check_pub_priv( const mbedtls_rsa_context *pub, const mbedtls_rsa_context *prv )
{
    if( mbedtls_rsa_check_pubkey( pub ) != 0 ||
        mbedtls_rsa_check_privkey( prv ) != 0 )
    {
        return( MBEDTLS_ERR_RSA_KEY_CHECK_FAILED );
    }

    if( mbedtls_mpi_cmp_mpi( &pub->N, &prv->N ) != 0 ||
        mbedtls_mpi_cmp_mpi( &pub->E, &prv->E ) != 0 )
    {
        return( MBEDTLS_ERR_RSA_KEY_CHECK_FAILED );
    }

    return( 0 );
}

/*
 * Do an RSA public key operation
 */
int mbedtls_rsa_public( mbedtls_rsa_context *ctx,
                const unsigned char *input,
                unsigned char *output )
{
    int ret;
    size_t olen;
    mbedtls_mpi T;

    mbedtls_mpi_init( &T );

#if defined(MBEDTLS_THREADING_C)
    if( ( ret = mbedtls_mutex_lock( &ctx->mutex ) ) != 0 )
        return( ret );
#endif

    MBEDTLS_MPI_CHK( mbedtls_mpi_read_binary( &T, input, ctx->len ) );

    if( mbedtls_mpi_cmp_mpi( &T, &ctx->N ) >= 0 )
    {
        ret = MBEDTLS_ERR_MPI_BAD_INPUT_DATA;
        goto cleanup;
    }

    olen = ctx->len;
    MBEDTLS_MPI_CHK( mbedtls_mpi_exp_mod( &T, &T, &ctx->E, &ctx->N, &ctx->RN ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_write_binary( &T, output, olen ) );

cleanup:
#if defined(MBEDTLS_THREADING_C)
    if( mbedtls_mutex_unlock( &ctx->mutex ) != 0 )
        return( MBEDTLS_ERR_THREADING_MUTEX_ERROR );
#endif

    mbedtls_mpi_free( &T );

    if( ret != 0 )
        return( MBEDTLS_ERR_RSA_PUBLIC_FAILED + ret );

    return( 0 );
}

/*
 * Generate or update blinding values, see section 10 of:
 *  KOCHER, Paul C. Timing attacks on implementations of Diffie-Hellman, RSA,
 *  DSS, and other systems. In : Advances in Cryptology-CRYPTO'96. Springer
 *  Berlin Heidelberg, 1996. p. 104-113.
 */
static int rsa_prepare_blinding( mbedtls_rsa_context *ctx,
                 int (*f_rng)(void *, unsigned char *, size_t), void *p_rng )
{
    int ret, count = 0;

    if( ctx->Vf.p != NULL )
    {
        /* We already have blinding values, just update them by squaring */
        MBEDTLS_MPI_CHK( mbedtls_mpi_mul_mpi( &ctx->Vi, &ctx->Vi, &ctx->Vi ) );
        MBEDTLS_MPI_CHK( mbedtls_mpi_mod_mpi( &ctx->Vi, &ctx->Vi, &ctx->N ) );
        MBEDTLS_MPI_CHK( mbedtls_mpi_mul_mpi( &ctx->Vf, &ctx->Vf, &ctx->Vf ) );
        MBEDTLS_MPI_CHK( mbedtls_mpi_mod_mpi( &ctx->Vf, &ctx->Vf, &ctx->N ) );

        goto cleanup;
    }

    /* Unblinding value: Vf = random number, invertible mod N */
    do {
        if( count++ > 10 )
            return( MBEDTLS_ERR_RSA_RNG_FAILED );

        MBEDTLS_MPI_CHK( mbedtls_mpi_fill_random( &ctx->Vf, ctx->len - 1, f_rng, p_rng ) );
        MBEDTLS_MPI_CHK( mbedtls_mpi_gcd( &ctx->Vi, &ctx->Vf, &ctx->N ) );
    } while( mbedtls_mpi_cmp_int( &ctx->Vi, 1 ) != 0 );

    /* Blinding value: Vi =  Vf^(-e) mod N */
    MBEDTLS_MPI_CHK( mbedtls_mpi_inv_mod( &ctx->Vi, &ctx->Vf, &ctx->N ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_exp_mod( &ctx->Vi, &ctx->Vi, &ctx->E, &ctx->N, &ctx->RN ) );


cleanup:
    return( ret );
}

/*
 * Do an RSA private key operation
 */
int mbedtls_rsa_private( mbedtls_rsa_context *ctx,
                 int (*f_rng)(void *, unsigned char *, size_t),
                 void *p_rng,
                 const unsigned char *input,
                 unsigned char *output )
{
    int ret;
    size_t olen;
    mbedtls_mpi T, T1, T2;

    /* Make sure we have private key info, prevent possible misuse */
    if( ctx->P.p == NULL || ctx->Q.p == NULL || ctx->D.p == NULL )
        return( MBEDTLS_ERR_RSA_BAD_INPUT_DATA );

    mbedtls_mpi_init( &T ); mbedtls_mpi_init( &T1 ); mbedtls_mpi_init( &T2 );

#if defined(MBEDTLS_THREADING_C)
    if( ( ret = mbedtls_mutex_lock( &ctx->mutex ) ) != 0 )
        return( ret );
#endif

    MBEDTLS_MPI_CHK( mbedtls_mpi_read_binary( &T, input, ctx->len ) );
    if( mbedtls_mpi_cmp_mpi( &T, &ctx->N ) >= 0 )
    {
        ret = MBEDTLS_ERR_MPI_BAD_INPUT_DATA;
        goto cleanup;
    }

    if( f_rng != NULL )
    {
        /*
         * Blinding
         * T = T * Vi mod N
         */
        MBEDTLS_MPI_CHK( rsa_prepare_blinding( ctx, f_rng, p_rng ) );
        MBEDTLS_MPI_CHK( mbedtls_mpi_mul_mpi( &T, &T, &ctx->Vi ) );
        MBEDTLS_MPI_CHK( mbedtls_mpi_mod_mpi( &T, &T, &ctx->N ) );
    }

#if defined(MBEDTLS_RSA_NO_CRT)
    MBEDTLS_MPI_CHK( mbedtls_mpi_exp_mod( &T, &T, &ctx->D, &ctx->N, &ctx->RN ) );
#else
    /*
     * faster decryption using the CRT
     *
     * T1 = input ^ dP mod P
     * T2 = input ^ dQ mod Q
     */
    MBEDTLS_MPI_CHK( mbedtls_mpi_exp_mod( &T1, &T, &ctx->DP, &ctx->P, &ctx->RP ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_exp_mod( &T2, &T, &ctx->DQ, &ctx->Q, &ctx->RQ ) );

    /*
     * T = (T1 - T2) * (Q^-1 mod P) mod P
     */
    MBEDTLS_MPI_CHK( mbedtls_mpi_sub_mpi( &T, &T1, &T2 ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_mul_mpi( &T1, &T, &ctx->QP ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_mod_mpi( &T, &T1, &ctx->P ) );

    /*
     * T = T2 + T * Q
     */
    MBEDTLS_MPI_CHK( mbedtls_mpi_mul_mpi( &T1, &T, &ctx->Q ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_add_mpi( &T, &T2, &T1 ) );
#endif /* MBEDTLS_RSA_NO_CRT */

    if( f_rng != NULL )
    {
        /*
         * Unblind
         * T = T * Vf mod N
         */
        MBEDTLS_MPI_CHK( mbedtls_mpi_mul_mpi( &T, &T, &ctx->Vf ) );
        MBEDTLS_MPI_CHK( mbedtls_mpi_mod_mpi( &T, &T, &ctx->N ) );
    }

    olen = ctx->len;
    MBEDTLS_MPI_CHK( mbedtls_mpi_write_binary( &T, output, olen ) );

cleanup:
#if defined(MBEDTLS_THREADING_C)
    if( mbedtls_mutex_unlock( &ctx->mutex ) != 0 )
        return( MBEDTLS_ERR_THREADING_MUTEX_ERROR );
#endif

    mbedtls_mpi_free( &T ); mbedtls_mpi_free( &T1 ); mbedtls_mpi_free( &T2 );

    if( ret != 0 )
        return( MBEDTLS_ERR_RSA_PRIVATE_FAILED + ret );

    return( 0 );
}

/*
 * Free the components of an RSA key
 */
void mbedtls_rsa_free( mbedtls_rsa_context *ctx )
{
    mbedtls_mpi_free( &ctx->Vi ); mbedtls_mpi_free( &ctx->Vf );
    mbedtls_mpi_free( &ctx->RQ ); mbedtls_mpi_free( &ctx->RP ); mbedtls_mpi_free( &ctx->RN );
    mbedtls_mpi_free( &ctx->QP ); mbedtls_mpi_free( &ctx->DQ ); mbedtls_mpi_free( &ctx->DP );
    mbedtls_mpi_free( &ctx->Q  ); mbedtls_mpi_free( &ctx->P  ); mbedtls_mpi_free( &ctx->D );
    mbedtls_mpi_free( &ctx->E  ); mbedtls_mpi_free( &ctx->N  );

#if defined(MBEDTLS_THREADING_C)
    mbedtls_mutex_free( &ctx->mutex );
#endif
}

int rk_rsa_private(rsa_key_t *key, const unsigned char *input, unsigned char *output)
{
    int ret = -1;
    mbedtls_rsa_context rsa;

    mbedtls_rsa_init( &rsa);

    rsa.len = key->key_len;
    MBEDTLS_MPI_CHK( mbedtls_mpi_read_binary( &rsa.N , key->n, key->key_len) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_read_binary( &rsa.E , key->e, key->e_len) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_read_binary( &rsa.D , key->d, key->d_len) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_read_binary( &rsa.P , key->p, key->p_len  ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_read_binary( &rsa.Q , key->q, key->q_len  ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_read_binary( &rsa.DP, key->dp, key->dp_len  ));
    MBEDTLS_MPI_CHK( mbedtls_mpi_read_binary( &rsa.DQ, key->dq, key->dq_len  ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_read_binary( &rsa.QP, key->iq, key->iq_len) );

    if(mbedtls_rsa_check_privkey( &rsa ) != 0)
    {
        return -1;
    }

	ret = mbedtls_rsa_private(&rsa, NULL, NULL, input, output);

	ret = (ret==0)?0:-1;
cleanup:
	mbedtls_rsa_free( &rsa );
	return ret;
}

int rk_rsa_public(rsa_key_t *key, const unsigned char *input, unsigned char *output)
{
    int ret = -1;
    mbedtls_rsa_context rsa;

    mbedtls_rsa_init( &rsa);

    rsa.len = key->key_len;
	MBEDTLS_MPI_CHK( mbedtls_mpi_read_binary( &rsa.N , key->n, key->key_len) );
	MBEDTLS_MPI_CHK( mbedtls_mpi_read_binary( &rsa.E , key->e, key->e_len) );

    if(mbedtls_rsa_check_pubkey( &rsa ) != 0)
    {
        goto cleanup;
    }

	ret = mbedtls_rsa_public(&rsa, input, output);
	ret = (ret==0)?0:-1;
cleanup:
	mbedtls_rsa_free( &rsa );
    return ret;
}

int rk_rsa_public2(const mbed_rsa_key_t *key, const unsigned char *input, unsigned char *output)
{
    int ret = -1;
    mbedtls_rsa_context rsa;

    mbedtls_rsa_init( &rsa);

    rsa.len = key->n_len;
	MBEDTLS_MPI_CHK( mbedtls_mpi_read_binary( &rsa.N , key->n, key->n_len) );
	MBEDTLS_MPI_CHK( mbedtls_mpi_read_binary( &rsa.E , key->e, key->e_len) );

    if(mbedtls_rsa_check_pubkey( &rsa ) != 0)
    {
        goto cleanup;
    }

	ret = mbedtls_rsa_public(&rsa, input, output);
	ret = (ret==0)?0:-1;
cleanup:
	mbedtls_rsa_free( &rsa );
    return ret;
}


