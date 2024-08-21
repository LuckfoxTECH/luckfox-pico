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

#include "c_function.h"
#include "c_pair.h"

static c_bool _c_unary_negate_impl(c_unary_predicate * thiz, value_type val)
{
	return !((UNARY_PREDICATE)(thiz->_l))(val);
}

c_unary_predicate c_unary_negate(UNARY_PREDICATE unary_pred)
{
	c_unary_predicate up;
	up.O = _c_unary_negate_impl;
	up._l = unary_pred;
	return up;
}

static c_bool _c_binary_negate_impl(c_binary_predicate * thiz, value_type val1, value_type val2)
{
	return !((BINARY_PREDICATE)(thiz->_l))(val1, val2);
}

c_binary_predicate c_binary_negate(BINARY_PREDICATE binary_pred)
{
	c_binary_predicate bp;
	bp.O = _c_binary_negate_impl;
	bp._l = binary_pred;
	return bp;
}

static value_type _c_identity_impl(c_unary_function * thiz, value_type val)
{
	return val;
}

c_unary_function c_identity()
{
	c_unary_function uf;
	uf.O = _c_identity_impl;
	uf._l = NULL;
	return uf;
}

static value_type _c_select1st_impl(c_unary_function * thiz, value_type val)
{
	return ((c_ppair)(val))->first;
}

c_unary_function c_select1st()
{
	c_unary_function uf;
	uf.O = _c_select1st_impl;
	uf._l = NULL;
	return uf;
}

static value_type _c_select1stptr_impl(c_unary_function * thiz, value_type val)
{
	return (value_type)(&((c_ppair)(val))->first);
}

c_unary_function c_select1stptr()
{
	c_unary_function uf;
	uf.O = _c_select1stptr_impl;
	uf._l = NULL;
	return uf;
}

static c_bool _c_unary_adapt_impl(c_unary_predicate * thiz, value_type val)
{
	return ((UNARY_PREDICATE)(thiz->_l))(val);
}

c_unary_predicate c_unary_adapt(UNARY_PREDICATE unary_pred)
{
	c_unary_predicate up;
	up.O = _c_unary_adapt_impl;
	up._l = unary_pred;
	return up;
}

static c_bool _c_binary_adapt_impl(c_binary_predicate * thiz, value_type val1, value_type val2)
{
	return ((BINARY_PREDICATE)(thiz->_l))(val1, val2);
}

c_binary_predicate c_binary_adapt(BINARY_PREDICATE binary_pred)
{
	c_binary_predicate bp;
	bp.O = _c_binary_adapt_impl;
	bp._l = binary_pred;
	return bp;
}

