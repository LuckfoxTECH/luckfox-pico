/*****************************************************************************
* randm.c - Random number generator program file.
*
* Copyright (c) 2003 by Marc Boucher, Services Informatiques (MBSI) inc.
* Copyright (c) 1998 by Global Election Systems Inc.
*
* The authors hereby grant permission to use, copy, modify, distribute,
* and license this software and its documentation for any purpose, provided
* that existing copyright notices are retained in all copies and that this
* notice and the following disclaimer are included verbatim in any
* distributions. No written agreement, license, or royalty fee is required
* for any of the authorized uses.
*
* THIS SOFTWARE IS PROVIDED BY THE CONTRIBUTORS *AS IS* AND ANY EXPRESS OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL THE CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
* NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
* THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
* THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
******************************************************************************
* REVISION HISTORY
*
* 03-01-01 Marc Boucher <marc@mbsi.ca>
*   Ported to lwIP.
* 98-06-03 Guy Lancaster <lancasterg@acm.org>, Global Election Systems Inc.
*   Extracted from avos.
*****************************************************************************/

#if   0
#include "lwip/opt.h"
#include <stdlib.h>
#include <stdio.h>

#include "echo_random.h"
#include "md5.h"
#include <string.h>

#define LWIP_MAX(x , y)  (((x) > (y)) ? (x) : (y))
#define LWIP_MIN(x , y)  (((x) < (y)) ? (x) : (y))

#define RANDPOOLSZ 16   /* Bytes stored in the pool of randomness. */

/*****************************/
/*** LOCAL DATA STRUCTURES ***/
/*****************************/
static char randPool[RANDPOOLSZ];   /* Pool of randomness. */
static long randCount = 0;      /* Pseudo-random incrementer */


/***********************************/
/*** PUBLIC FUNCTION DEFINITIONS ***/
/***********************************/
/*
 * Initialize the random number generator.
 *
 * Since this is to be called on power up, we don't have much
 *  system randomess to work with.  Here all we use is the
 *  real-time clock.  We'll accumulate more randomness as soon
 *  as things start happening.
 */
void avRandomInit()
{
    avChurnRand(NULL, 0);
}

/*
 * Churn the randomness pool on a random event.  Call this early and often
 *  on random and semi-random system events to build randomness in time for
 *  usage.  For randomly timed events, pass a null pointer and a zero length
 *  and this will use the system timer and other sources to add randomness.
 *  If new random data is available, pass a pointer to that and it will be
 *  included.
 *
 * Ref: Applied Cryptography 2nd Ed. by Bruce Schneier p. 427
 */
void avChurnRand(char *randData, uint32_t randLen)
{
    MD5_CTX md5;

    /* LWIP_DEBUGF(LOG_INFO, ("churnRand: %u@%P\n", randLen, randData)); */
    MD5Init(&md5);
    MD5Update(&md5, (u_char *)randPool, sizeof(randPool));
    if (randData)
    {
        MD5Update(&md5, (u_char *)randData, randLen);
    }
    else
    {
        struct
        {
            /* INCLUDE fields for any system sources of randomness */
            char foobar;
        } sysData;

        /* Load sysData fields here. */
        MD5Update(&md5, (u_char *)&sysData, sizeof(sysData));
    }
    MD5Final((u_char *)randPool, &md5);
    /*  LWIP_DEBUGF(LOG_INFO, ("churnRand: -> 0\n")); */
}

/*
 * Use the random pool to generate random data.  This degrades to pseudo
 *  random when used faster than randomness is supplied using churnRand().
 * Note: It's important that there be sufficient randomness in randPool
 *  before this is called for otherwise the range of the result may be
 *  narrow enough to make a search feasible.
 *
 * Ref: Applied Cryptography 2nd Ed. by Bruce Schneier p. 427
 *
 * XXX Why does he not just call churnRand() for each block?  Probably
 *  so that you don't ever publish the seed which could possibly help
 *  predict future values.
 * XXX Why don't we preserve md5 between blocks and just update it with
 *  randCount each time?  Probably there is a weakness but I wish that
 *  it was documented.
 */
void avGenRand(char *buf, uint32_t bufLen)
{
    MD5_CTX md5;
    u_char tmp[16];
    uint32_t n;

    while (bufLen > 0)
    {
        n = LWIP_MIN(bufLen, RANDPOOLSZ);
        MD5Init(&md5);
        MD5Update(&md5, (u_char *)randPool, sizeof(randPool));
        MD5Update(&md5, (u_char *)&randCount, sizeof(randCount));
        MD5Final(tmp, &md5);
        randCount++;
        MEMCPY(buf, tmp, n);
        buf += n;
        bufLen -= n;
    }
}

/*
 * Return a new random number.
 */
uint32_t avRandom()
{
    uint32_t newRand;

    avGenRand((char *)&newRand, sizeof(newRand));

    return newRand;
}

#endif