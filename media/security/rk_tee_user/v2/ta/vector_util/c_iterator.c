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

#include "c_iterator.h"

typedef void ** pp_t;

static c_iterator _c_array_iterator_assign(c_piterator thiz, const c_piterator val)
{
	if(thiz != val)
		thiz->_i = val->_i;
	return *thiz;
}

static value_type _c_array_iterator_ref(c_piterator thiz)
{
	return *(pp_t)thiz->_i;
}

static value_type _c_array_iterator_ref_assign(c_piterator thiz, const value_type val)
{
	return *(pp_t)thiz->_i = val;
}

static c_iterator _c_array_iterator_inc(c_piterator thiz)
{
	pp_t p = thiz->_i;
	++ p;
	thiz->_i = p;
	return *thiz;
}

static c_iterator _c_array_iterator_inc_n(c_piterator thiz, difference_type n)
{
	pp_t p = thiz->_i;
	p += n;
	thiz->_i = p;
	return *thiz;
}

static c_iterator _c_array_iterator_dec(c_piterator thiz)
{
	pp_t p = thiz->_i;
	-- p;
	thiz->_i = p;
	return *thiz;
}

static c_iterator _c_array_iterator_dec_n(c_piterator thiz, difference_type n)
{
	pp_t p = thiz->_i;
	p -= n;
	thiz->_i = p;
	return *thiz;
}

static difference_type _c_array_iterator_diff(c_piterator thiz, const c_piterator val)
{
	return ((pp_t)thiz->_i - (pp_t)val->_i);
}

static value_type _c_array_iterator_at(c_piterator thiz, difference_type n)
{
	return *((pp_t)thiz->_i + n);
}

static c_iterator _c_array_iterator_positive_n(c_piterator thiz, difference_type n)
{
	c_iterator iter;
	pp_t p = thiz->_i;
	p += n;
	iter = c_get_array_iterator(p);
	return iter;
}

static c_iterator _c_array_iterator_negative_n(c_piterator thiz, difference_type n)
{
	c_iterator iter;
	pp_t p = thiz->_i;
	p -= n;
	iter = c_get_array_iterator(p);
	return iter;
}

static c_bool _c_array_iterator_equal(c_piterator thiz, const c_piterator val)
{
	return (thiz->_i == val->_i &&
				thiz->_pft == val->_pft);
}

static c_bool _c_array_iterator_less(c_piterator thiz, const c_piterator val)
{
	return ((pp_t)thiz->_i < (pp_t)val->_i);
}

static c_iterator_ft _c_array_iterator_ft =
{
	_c_array_iterator_assign,
	_c_array_iterator_ref,
	_c_array_iterator_ref_assign,
	_c_array_iterator_inc,
	_c_array_iterator_inc_n,
	_c_array_iterator_dec,
	_c_array_iterator_dec_n,
	_c_array_iterator_diff,
	_c_array_iterator_at,
	_c_array_iterator_positive_n,
	_c_array_iterator_negative_n,
	_c_array_iterator_equal,
	_c_array_iterator_less
};

c_iterator c_get_array_iterator(void ** ppt)
{
	c_iterator iter;
	iter._pft = &_c_array_iterator_ft;
	iter._i = ppt;
	return iter;
}

static c_reverse_iterator _c_array_reverse_iterator_assign(c_preverse_iterator thiz, const c_preverse_iterator val)
{
	if(thiz != val)
		thiz->_i = val->_i;
	return *thiz;
}

static value_type _c_array_reverse_iterator_ref(c_preverse_iterator thiz)
{
	return *(pp_t)thiz->_i;
}

static value_type _c_array_reverse_iterator_ref_assign(c_preverse_iterator thiz, const value_type val)
{
	return *(pp_t)thiz->_i = val;
}

static c_reverse_iterator _c_array_reverse_iterator_inc(c_preverse_iterator thiz)
{
	pp_t p = thiz->_i;
	-- p;
	thiz->_i = p;
	return *thiz;
}

static c_reverse_iterator _c_array_reverse_iterator_inc_n(c_preverse_iterator thiz, difference_type n)
{
	pp_t p = thiz->_i;
	p -= n;
	thiz->_i = p;
	return *thiz;
}

static c_reverse_iterator _c_array_reverse_iterator_dec(c_preverse_iterator thiz)
{
	pp_t p = thiz->_i;
	++ p;
	thiz->_i = p;
	return *thiz;
}

static c_reverse_iterator _c_array_reverse_iterator_dec_n(c_preverse_iterator thiz, difference_type n)
{
	pp_t p = thiz->_i;
	p += n;
	thiz->_i = p;
	return *thiz;
}

static difference_type _c_array_reverse_iterator_diff(c_preverse_iterator thiz, const c_preverse_iterator val)
{
	return ((pp_t)val->_i - (pp_t)thiz->_i);
}

static value_type _c_array_reverse_iterator_at(c_preverse_iterator thiz, difference_type n)
{
	return *((pp_t)thiz->_i - n);
}

static c_reverse_iterator _c_array_reverse_iterator_positive_n(c_preverse_iterator thiz, difference_type n)
{
	c_reverse_iterator iter;
	pp_t p = thiz->_i;
	p -= n;
	iter = c_get_array_reverse_iterator(p);
	return iter;
}

static c_reverse_iterator _c_array_reverse_iterator_negative_n(c_preverse_iterator thiz, difference_type n)
{
	c_reverse_iterator iter;
	pp_t p = thiz->_i;
	p += n;
	iter = c_get_array_reverse_iterator(p);
	return iter;
}

static c_bool _c_array_reverse_iterator_equal(c_preverse_iterator thiz, const c_preverse_iterator val)
{
	return (thiz->_i == val->_i &&
				thiz->_pft == val->_pft);
}

static c_bool _c_array_reverse_iterator_less(c_preverse_iterator thiz, const c_preverse_iterator val)
{
	return ((pp_t)thiz->_i > (pp_t)val->_i);
}

static c_reverse_iterator_ft _c_array_reverse_iterator_ft =
{
	_c_array_reverse_iterator_assign,
	_c_array_reverse_iterator_ref,
	_c_array_reverse_iterator_ref_assign,
	_c_array_reverse_iterator_inc,
	_c_array_reverse_iterator_inc_n,
	_c_array_reverse_iterator_dec,
	_c_array_reverse_iterator_dec_n,
	_c_array_reverse_iterator_diff,
	_c_array_reverse_iterator_at,
	_c_array_reverse_iterator_positive_n,
	_c_array_reverse_iterator_negative_n,
	_c_array_reverse_iterator_equal,
	_c_array_reverse_iterator_less
};

c_reverse_iterator c_get_array_reverse_iterator(void ** ppt)
{
	c_reverse_iterator iter;
	iter._pft = &_c_array_reverse_iterator_ft;
	iter._i = ppt;
	return iter;
}

difference_type c_distance(c_iterator first, c_iterator last)
{
	if(CHECK_RANDOM_ACCESS_ITERATOR(first) &&
		CHECK_RANDOM_ACCESS_ITERATOR(last))
		return ITER_DIFF(last, first);
	else
	{
		difference_type n = 0;
		while(!ITER_EQUAL(first, last))
		{
			ITER_INC(first);
			++ n;
		}
		return n;
	}
}

void c_distance1(c_iterator first, c_iterator last, difference_type * pn)
{
	if(CHECK_RANDOM_ACCESS_ITERATOR(first) &&
		CHECK_RANDOM_ACCESS_ITERATOR(last))
		*pn += ITER_DIFF(last, first);
	else
	{
		while(!ITER_EQUAL(first, last))
		{
			ITER_INC(first);
			++ *pn;
		}
	}
}

void c_advance(c_piterator pval, difference_type n)
{
	c_iterator val = *pval;
	if(CHECK_RANDOM_ACCESS_ITERATOR(val))
		ITER_INC_N(val, n);
	else if(CHECK_BIDIRECTIONAL_ITERATOR(val))
	{
		if(n >= 0)
			while(n --)
				ITER_INC(val);
		else
			while(n ++)
				ITER_DEC(val);
	}
	else
		while(n --)
			ITER_INC(val);
	*pval = val;
}

value_type c_iter_ref(c_iterator x)
{
	return ITER_REF(x);
}

value_type c_iter_ref_assign(c_iterator x, const value_type val)
{
	return ITER_REF_ASSIGN(x, val);
}

difference_type c_iter_diff(c_iterator x, c_iterator y)
{
	return ITER_DIFF(x, y);
}

value_type c_iter_at(c_iterator x, difference_type n)
{
	return ITER_AT(x, n);
}

c_iterator c_iter_positive_n(c_iterator x, difference_type n)
{
	return ITER_POSITIVE_N(x, n);
}

c_iterator c_iter_negative_n(c_iterator x, difference_type n)
{
	return ITER_NEGATIVE_N(x, n);
}

c_bool c_iter_equal(c_iterator x, c_iterator y)
{
	return ITER_EQUAL(x, y);
}

c_bool c_iter_less(c_iterator x, c_iterator y)
{
	return ITER_LESS(x, y);
}


