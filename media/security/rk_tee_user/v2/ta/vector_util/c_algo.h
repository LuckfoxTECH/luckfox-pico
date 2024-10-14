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

#ifndef _C_ALGO_H
#define _C_ALGO_H

#include "c_iterator.h"
#include "c_function.h"

#define c_iter_swap			_c_iter_swap
#define c_copy                          _c_copy
#define c_copy_backward                 _c_copy_backward
#define c_lexicographical_compare       _c_lexicographical_compare
#define c_uninitialized_copy            _c_uninitialized_copy
#define c_fill                          _c_fill
#define c_fill_n                        _c_fill_n
#define c_uninitialized_fill_n          _c_unintialized_fill_n
#define c_equal                         _c_equal
#define c_equal2                        _c_equal2
#define c_for_each			_c_for_each
#define c_find				_c_find
#define c_find_if			_c_find_if
#define c_adjacent_find			_c_adjacent_find
#define c_count				_c_count
#define c_count_if			_c_count_if
#define c_reverse			_c_reverse
#define c_search			_c_search

void c_iter_swap(c_iterator x, c_iterator y);
c_iterator c_copy(c_iterator first, c_iterator last, c_iterator result);
c_iterator c_copy_backward(c_iterator first, c_iterator last, c_iterator result);
c_bool c_lexicographical_compare(c_iterator first1,
                                    c_iterator last1,
                                    c_iterator first2,
                                    c_iterator last2,
                                    COMPARER cmp);
c_iterator c_uninitialized_copy(c_iterator first, c_iterator last, c_iterator result);
void c_fill(c_iterator first, c_iterator last, const value_type val);
c_iterator c_fill_n(c_iterator first, size_type n, const value_type val);
c_iterator c_uninitialized_fill_n(c_iterator first, size_type n, const value_type val);
c_bool c_equal(c_iterator first1, c_iterator last1, c_iterator first2, BINARY_PREDICATE pf);
c_bool c_equal2(c_iterator first1, c_iterator last1, c_iterator first2, c_binary_predicate binary_pred);
UNARY_FUNCTION c_for_each(c_iterator first, c_iterator last, UNARY_FUNCTION pf);
c_iterator c_find(c_iterator first, c_iterator last, const value_type val);
c_iterator c_find_if(c_iterator first, c_iterator last, UNARY_PREDICATE pf);
c_iterator c_adjacent_find(c_iterator first, c_iterator last, BINARY_PREDICATE pf);
size_type c_count(c_iterator first, c_iterator last, const value_type val);
size_type c_count_if(c_iterator first, c_iterator last, UNARY_PREDICATE pf);
void c_reverse(c_iterator first, c_iterator last);
c_iterator c_search(c_iterator first1,
			c_iterator last1,
			c_iterator first2,
			c_iterator last2,
			BINARY_PREDICATE pf);


#define C_SWAP(X, Y, TMP)   do{(TMP)=(X);(X)=(Y);(Y)=(TMP);}while(0)
#define C_MAX(X, Y) ((X) >= (Y) ? (X) : (Y))
#define C_MIN(X, Y) ((X) <= (Y) ? (X) : (Y))

#endif /* _C_ALGO_H */
