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

#ifndef  _C_ITERATOR_H
#define _C_ITERATOR_H

#include "c_def.h"

#define c_iterator_ft 			_c_iterator_ft
#define c_iterator_pft 			_c_iterator_pft
#define c_reverse_iterator_ft 		_c_reverse_iterator_ft
#define c_reverse_iterator_pft 		_c_reverse_iterator_pft
#define c_iterator 			_c_iterator
#define c_reverse_iterator 		_c_reverse_iterator
#define c_distance 			_c_distance
#define c_distance1 			_c_distance1
#define c_advance 			_c_advance


#define c_get_array_iterator		_c_get_array_iterator
#define c_get_array_reverse_iterator	_c_get_array_reverse_iterator


#define c_iterator_ref			_c_iterator_ref
#define c_iterator_ref_assign		_c_iterator_ref_assign
#define c_iterator_diff			_c_iterator_diff
#define c_iterator_at			_c_iterator_at
#define c_iterator_positive_n		_c_iterator_positvie_n
#define c_iterator_negative_n		_c_iterator_negative_n
#define c_iterator_equal		_c_iterator_equal
#define c_iterator_less			_c_iterator_less


typedef struct c_iterator c_iterator, * c_piterator;
typedef const c_iterator c_const_iterator;

typedef struct c_reverse_iterator  c_reverse_iterator, * c_preverse_iterator;
typedef const c_reverse_iterator c_const_reverse_iterator;

typedef struct c_iterator_ft c_iterator_ft, * c_iterator_pft;
typedef struct c_reverse_iterator_ft c_reverse_iterator_ft, * c_reverse_iterator_pft;

typedef ptrdiff_t difference_type;
typedef void ** pointer;
typedef const void ** const_pointer;
typedef void * value_type;
typedef size_t size_type;

struct c_iterator
{
	c_iterator_pft _pft;
	void * _i;
};

struct c_reverse_iterator
{
	c_reverse_iterator_pft _pft;
	void * _i;
};

struct c_iterator_ft
{
    c_iterator (* assign)(c_piterator thiz, const c_piterator val);
    value_type (* ref)(c_piterator thiz);
    value_type (* ref_assign)(c_piterator thiz, const value_type val);
    c_iterator (* inc)(c_piterator thiz);
    c_iterator (* inc_n)(c_piterator thiz, difference_type n);
    c_iterator (* dec)(c_piterator thiz);
    c_iterator (* dec_n)(c_piterator thiz, difference_type n);
    difference_type (* diff)(c_piterator thiz, const c_piterator val);
    value_type (* at)(c_piterator thiz, difference_type n);
    c_iterator (* positive_n)(c_piterator thiz, difference_type n);
    c_iterator (* negative_n)(c_piterator thiz, difference_type n);
    c_bool (* equal)(c_piterator thiz, const c_piterator val);
    c_bool (* less)(c_piterator thiz, const c_piterator val);
};

struct c_reverse_iterator_ft
{
    c_reverse_iterator (* assign)(c_preverse_iterator thiz, const c_preverse_iterator val);
    value_type (* ref)(c_preverse_iterator thiz);
    value_type (* ref_assign)(c_preverse_iterator thiz, const value_type val);
    c_reverse_iterator (* inc)(c_preverse_iterator thiz);
    c_reverse_iterator (* inc_n)(c_preverse_iterator thiz, difference_type n);
    c_reverse_iterator (* dec)(c_preverse_iterator thiz);
    c_reverse_iterator (* dec_n)(c_preverse_iterator thiz, difference_type n);
    difference_type (* diff)(c_preverse_iterator thiz, const c_preverse_iterator val);
    value_type (* at)(c_preverse_iterator thiz, difference_type n);
    c_reverse_iterator (* positive_n)(c_preverse_iterator thiz, difference_type n);
    c_reverse_iterator (* negative_n)(c_preverse_iterator thiz, difference_type n);
    c_bool (* equal)(c_preverse_iterator thiz, const c_preverse_iterator val);
    c_bool (* less)(c_preverse_iterator thiz, const c_preverse_iterator val);
};

#define ITER_ASSIGN(X, Y)       (X)._pft->assign(&(X), &(Y))
#define ITER_REF(X)             (X)._pft->ref(&(X))
#define ITER_REF_ASSIGN(X, Y)   (X)._pft->ref_assign(&(X), Y)
#define ITER_INC(X)             (X)._pft->inc(&(X))
#define ITER_INC_N(X, Y)        (X)._pft->inc_n(&(X), Y)
#define ITER_DEC(X)             (X)._pft->dec(&(X))
#define ITER_DEC_N(X, Y)        (X)._pft->dec_n(&(X), Y)
#define ITER_DIFF(X, Y)     	(X)._pft->diff(&(X), &(Y))
#define ITER_AT(X, Y)           (X)._pft->at(&(X), Y)
#define ITER_POSITIVE_N(X, Y)   (X)._pft->positive_n(&(X), Y)
#define ITER_NEGATIVE_N(X, Y)   (X)._pft->negative_n(&(X), Y)
#define ITER_EQUAL(X, Y)        (X)._pft->equal(&(X), &(Y))
#define ITER_LESS(X, Y)         (X)._pft->less(&(X), &(Y))


#define CHECK_OUTPUT_ITERATOR(X)            ((X)._pft->ref_assign)
#define CHECK_INPUT_ITERATOR(X)             ((X)._pft->ref)
#define CHECK_FORWARD_ITERATOR(X)           ((X)._pft->inc)
#define CHECK_BIDIRECTIONAL_ITERATOR(X)     ((X)._pft->inc && (X)._pft->dec)
#define CHECK_RANDOM_ACCESS_ITERATOR(X)     ((X)._pft->at && (X)._pft->diff)

typedef int (* COMPARER)(value_type , value_type);

difference_type c_distance(c_iterator first, c_iterator last);
void c_distance1(c_iterator first, c_iterator last, difference_type * pn);
void c_advance(c_piterator pval, difference_type n);

c_iterator c_get_array_iterator(void ** ppt);
c_reverse_iterator c_get_array_reverse_iterator(void ** ppt);

value_type c_iter_ref(c_iterator x);
value_type c_iter_ref_assign(c_iterator x, const value_type val);
difference_type c_iter_diff(c_iterator x, c_iterator y);
value_type c_iter_at(c_iterator x, difference_type n);
c_iterator c_iter_positive_n(c_iterator x, difference_type n);
c_iterator c_iter_negative_n(c_iterator x, difference_type n);
c_bool c_iter_equal(c_iterator x, c_iterator y);
c_bool c_iter_less(c_iterator x, c_iterator y);


#endif /* _C_ITERATOR_H */
