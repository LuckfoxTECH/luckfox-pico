/*
 *
 * Copyright (c) 1994
 * Hewlett-Packard Company
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Hewlett-Packard Company makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 *
 * Copyright (c) 1996
 * Silicon Graphics Computer Systems, Inc.
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Silicon Graphics makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 */

/*
Copyright (c) 2007 Lao wen bo

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
    claim that you wrote the original software. If you use this software
    in a product, an acknowledgment in the product documentation would be
    appreciated but is not required.

    2. Altered source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.

    3. This notice may not be removed or altered from any source
    distribution.

	Lao wen bo
	viewpl(at)gmail.com
*/

/*
NOTE: This is an internal header file, You should not attempt to use it directly.
*/

#ifndef _C_FUNCTION_H
#define _C_FUNCTION_H

#include "c_iterator.h"

#define c_unary_function		_c_unary_function
#define c_binary_function		_c_binary_function
#define c_unary_predicate		_c_unary_predicate
#define c_binary_predicate		_c_binary_predicate

#define c_unary_negate			_c_unary_negate
#define c_binary_negate			_c_binary_negate

#define c_identity			_c_identity
#define c_select1st			_c_select1st
#define c_select1stptr			_c_select1stptr

#define c_unary_adapt			_c_unary_adapt
#define c_binary_adapt			_c_binary_adapt

typedef value_type (*UNARY_FUNCTION)(value_type);
typedef value_type (*BINARY_FUNCTION)(value_type, value_type);
typedef c_bool (*UNARY_PREDICATE)(value_type);
typedef c_bool (*BINARY_PREDICATE)(value_type, value_type);
typedef c_bool (*PREDICATE)(value_type);


typedef struct c_unary_function c_unary_function;
typedef struct c_binary_function c_binary_function;

struct c_unary_function
{
	value_type (*O)(c_unary_function * thiz, value_type val);
	void * _l;
};

struct c_binary_fuction
{
	value_type (*O)(c_binary_function * thiz, value_type val1, value_type val2);
	void * _l;
};

typedef struct c_unary_predicate c_unary_predicate;
typedef struct c_binary_predicate c_binary_predicate;

struct c_unary_predicate
{
	c_bool (*O)(c_unary_predicate * thiz, value_type val);
	void * _l;
};

struct c_binary_predicate
{
	c_bool (*O)(c_binary_predicate * thiz, value_type val1, value_type val2);
	void * _l;
};


c_unary_predicate c_unary_negate(UNARY_PREDICATE unary_pred);
c_binary_predicate c_binary_negate(BINARY_PREDICATE binary_pred);

c_unary_function c_identity();
c_unary_function c_select1st();
c_unary_function c_select1stptr();

c_unary_predicate c_unary_adapt(UNARY_PREDICATE unary_pred);
c_binary_predicate c_binary_adapt(BINARY_PREDICATE binary_pred);

#endif /* _C_FUNCTION_H */

