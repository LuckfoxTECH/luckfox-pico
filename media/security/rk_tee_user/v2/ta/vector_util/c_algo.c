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

#include "c_algo.h"

void c_iter_swap(c_iterator x, c_iterator y)
{
	value_type tmp = ITER_REF(x);
	ITER_REF_ASSIGN(x, ITER_REF(y));
	ITER_REF_ASSIGN(y, tmp);
}

c_iterator c_copy(c_iterator first, c_iterator last, c_iterator result)
{
	for(; !ITER_EQUAL(first, last); ITER_INC(first), ITER_INC(result))
		ITER_REF_ASSIGN(result, ITER_REF(first));
	return result;
}

c_iterator c_copy_backward(c_iterator first, c_iterator last, c_iterator result)
{
	while(!ITER_EQUAL(first, last))
	{
		ITER_DEC(result);
		ITER_DEC(last);
		ITER_REF_ASSIGN(result, ITER_REF(last));
	}
	return result;
}

c_bool c_lexicographical_compare(c_iterator first1,
                                    c_iterator last1,
                                    c_iterator first2,
                                    c_iterator last2,
                                    COMPARER cmp)
{
	for(; !ITER_EQUAL(first1, last1) &&
			!ITER_EQUAL(first2, last2);
			ITER_INC(first1), ITER_INC(first2))
	{
		if((*cmp)(ITER_REF(first1), ITER_REF(first2)) < 0)
			return true;
		if((*cmp)(ITER_REF(first1), ITER_REF(first2)) > 0)
			return false;
	}
	return (ITER_EQUAL(first1, last1) && !ITER_EQUAL(first2, last2));
}

c_iterator c_uninitialized_copy(c_iterator first, c_iterator last, c_iterator result)
{
	return c_copy(first, last, result);
}

void c_fill(c_iterator first, c_iterator last, const value_type val)
{
	for(; !ITER_EQUAL(first, last); ITER_INC(first))
		ITER_REF_ASSIGN(first, val);
}

c_iterator c_fill_n(c_iterator first, size_type n, const value_type val)
{
	for(; n > 0; -- n, ITER_INC(first))
		ITER_REF_ASSIGN(first, val);
	return first;
}

c_iterator c_uninitialized_fill_n(c_iterator first, size_type n, const value_type val)
{
	return c_fill_n(first, n, val);
}

c_bool c_equal(c_iterator first1, c_iterator last1, c_iterator first2, BINARY_PREDICATE pf)
{
	for(; !ITER_EQUAL(first1, last1); ITER_INC(first1), ITER_INC(first2))
		if(!(pf)(ITER_REF(first1), ITER_REF(first2)))
			return false;
	return true;
}

c_bool c_equal2(c_iterator first1, c_iterator last1, c_iterator first2, c_binary_predicate binary_pred)
{
	for(; !ITER_EQUAL(first1, last1); ITER_INC(first1), ITER_INC(first2))
		if(!(binary_pred.O)(&binary_pred, ITER_REF(first1), ITER_REF(first2)))
			return false;
	return true;
}

UNARY_FUNCTION c_for_each(c_iterator first, c_iterator last, UNARY_FUNCTION pf)
{
	for(; !ITER_EQUAL(first, last); ITER_INC(first))
		pf(ITER_REF(first));
	return pf;
}

c_iterator c_find(c_iterator first, c_iterator last, const value_type val)
{
	if(CHECK_RANDOM_ACCESS_ITERATOR(first))
	{
		difference_type tpc = ITER_DIFF(last, first) >> 2;
		for(; tpc > 0; -- tpc)
		{
			if(ITER_REF(first) == val)
				return first;
			ITER_INC(first);

			if(ITER_REF(first) == val)
				return first;
			ITER_INC(first);

			if(ITER_REF(first) == val)
				return first;
			ITER_INC(first);

			if(ITER_REF(first) == val)
				return first;
			ITER_INC(first);
		}

		switch(ITER_DIFF(last, first))
		{
			case 3:
				if(ITER_REF(first) == val)
					return first;
				ITER_INC(first);
			case 2:
				if(ITER_REF(first) == val)
					return first;
				ITER_INC(first);
			case 1:
				if(ITER_REF(first) == val)
					return first;
				ITER_INC(first);
			case 0:
			default:
				return last;
		}
	}
	else
	{
		while(!ITER_EQUAL(first, last) && !(ITER_REF(first) == val))
			ITER_INC(first);
		return first;
	}
}

c_iterator c_find_if(c_iterator first, c_iterator last, UNARY_PREDICATE pf)
{
	if(CHECK_RANDOM_ACCESS_ITERATOR(first))
	{
		difference_type tpc = ITER_DIFF(last, first) >> 2;
		for(; tpc > 0; -- tpc)
		{
			if(pf(ITER_REF(first)))
				return first;
			ITER_INC(first);

			if(pf(ITER_REF(first)))
				return first;
			ITER_INC(first);

			if(pf(ITER_REF(first)))
				return first;
			ITER_INC(first);

			if(pf(ITER_REF(first)))
				return first;
			ITER_INC(first);
		}

		switch(ITER_DIFF(last, first))
		{
			case 3:
				if(pf(ITER_REF(first)))
					return first;
				ITER_INC(first);
			case 2:
				if(pf(ITER_REF(first)))
					return first;
				ITER_INC(first);
			case 1:
				if(pf(ITER_REF(first)))
					return first;
				ITER_INC(first);
			case 0:
			default:
				return last;
		}
	}
	else
	{
		while(!ITER_EQUAL(first, last) && !pf(ITER_REF(first)))
			ITER_INC(first);
		return first;
	}
}

c_iterator c_adjacent_find(c_iterator first, c_iterator last, BINARY_PREDICATE pf)
{
	c_iterator next = first;

	if(ITER_EQUAL(first, last))
		return last;

	while(!c_iter_equal(ITER_INC(next), last))
	{
		if(pf(ITER_REF(first), ITER_REF(next)))
			return first;
		first = next;
	}
	return last;
}

size_type c_count(c_iterator first, c_iterator last, const value_type val)
{
	size_type n = 0;
	for(; !ITER_EQUAL(first, last); ITER_INC(first))
		if(ITER_REF(first) == val)
			++ n;
	return n;
}

size_type c_count_if(c_iterator first, c_iterator last, UNARY_PREDICATE pf)
{
	size_type n = 0;
	for(; !ITER_EQUAL(first, last); ITER_INC(first))
		if(pf(ITER_REF(first)))
			++ n;
	return n;
}

void c_reverse(c_iterator first, c_iterator last)
{
	if(CHECK_RANDOM_ACCESS_ITERATOR(first))
	{
		while(ITER_LESS(first, last))
		{
			c_iter_swap(first, ITER_DEC(last));
			ITER_INC(first);
		}
	}
	else
	{
		while(true)
			if(ITER_EQUAL(first, last) ||
					c_iter_equal(first, ITER_DEC(last)))
				return;
			else
			{
				c_iter_swap(first, last);
				ITER_INC(first);
			}
	}
}

c_iterator c_search(c_iterator first1,
			c_iterator last1,
			c_iterator first2,
			c_iterator last2,
			BINARY_PREDICATE pf)
{
	if(ITER_EQUAL(first1, last1) ||
			ITER_EQUAL(first2, last2))
		return first1;

	if(true)
	{
		c_iterator tmp = first2;
		ITER_INC(tmp);
		if(ITER_EQUAL(tmp, last2))
		{
			while(!ITER_EQUAL(first1, last1) &&
					!pf(ITER_REF(first1), ITER_REF(first2)))
				ITER_INC(first1);
			return first1;
		}
	}

	if(true)
	{
		c_iterator p1, p;
		c_iterator current = first1;

		p1 = first2;
		ITER_INC(p1);

		while(!ITER_EQUAL(first1, last1))
		{
			while(!ITER_EQUAL(first1, last1))
			{
				if(pf(ITER_REF(first1), ITER_REF(first2)))
					break;
				ITER_INC(first1);
			}

			while(!ITER_EQUAL(first1, last1) &&
					!pf(ITER_REF(first1), ITER_REF(first2)))
				ITER_INC(first1);
			if(ITER_EQUAL(first1, last1))
				return last1;

			p = p1;
			current = first1;
			ITER_INC(current);
			if(ITER_EQUAL(current, last1))
				return last1;

			while(pf(ITER_REF(current), ITER_REF(p)))
			{
				ITER_INC(p);
				if(ITER_EQUAL(p, last2))
					return first1;
				ITER_INC(current);
				if(ITER_EQUAL(current, last1))
					return last1;
			}

			ITER_INC(first1);
		}

		return first1;
	}
}


