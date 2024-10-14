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

#ifndef _C_VECTOR_H
#define _C_VECTOR_H

#include "c_def.h"
#include "c_iterator.h"

#define c_vector			_c_vector
#define c_pvector			_c_pvector
#define c_vector_create			__c_vector
#define c_vector_destroy		__c_rotcev
#define c_vector_begin			_c_vector_begin
#define c_vector_end			_c_vector_end
#define c_vector_rbegin			_c_vector_rbegin
#define c_vector_rend			_c_vector_rend
#define c_vector_size			_c_vector_size
#define c_vector_max_size		_c_vector_max_size
#define c_vector_empty			_c_vector_empty
#define c_vector_at			_c_vector_at
#define c_vector_assign			_c_vector_assign
#define c_vector_reserve		_c_vector_reserve
#define c_vector_front			_c_vector_front
#define c_vector_back			_c_vector_back
#define c_vector_push_back		_c_vector_push_back
#define c_vector_pop_back		_c_vector_pop_back
#define c_vector_swap			_c_vector_swap
#define c_vector_insert			_c_vector_insert
#define c_vector_insert2		_c_vector_insert2
#define c_vector_fill_insert		_c_vector_fill_insert
#define c_vector_erase			_c_vector_erase
#define c_vector_erase2			_c_vector_erase2
#define c_vector_clear			_c_vector_clear
#define c_vector_resize			_c_vector_resize
#define c_vector_equal			_c_vector_equal
#define c_vector_less			_c_vector_less
#define abs  				_abs

typedef struct c_vector
{
    COMPARER _cmp;
    void * _l;
} c_vector, * c_pvector;

int  _abs(signed int i);
void __c_vector(c_pvector thiz, COMPARER pcmp);
void __c_rotcev(c_pvector thiz);
c_iterator c_vector_begin(c_pvector thiz);
c_iterator c_vector_end(c_pvector thiz);
c_reverse_iterator c_vector_rbegin(c_pvector thiz);
c_reverse_iterator c_vector_rend(c_pvector thiz);
size_type c_vector_size(c_pvector thiz);
size_type c_vector_max_size(c_pvector thiz);
size_type c_vector_capacity(c_pvector thiz);
c_bool c_vector_empty(c_pvector thiz);
value_type c_vector_at(c_pvector thiz, size_type n);
c_pvector c_vector_assign(c_pvector thiz, const c_pvector V);
void c_vector_reserve(c_pvector thiz, size_t n);
value_type c_vector_front(c_pvector thiz);
value_type c_vector_back(c_pvector thiz);
void c_vector_push_back(c_pvector thiz, const value_type val);
void c_vector_pop_back(c_pvector thiz);
void c_vector_swap(c_pvector thiz, c_pvector V);
c_iterator c_vector_insert(c_pvector thiz, c_iterator pos, const value_type val);
void c_vector_insert2(c_pvector thiz, c_iterator pos, c_iterator first, c_iterator last);
void c_vector_fill_insert(c_pvector thiz, c_iterator pos, size_type n, const value_type val);
c_iterator c_vector_erase(c_pvector thiz, c_iterator pos);
c_iterator c_vector_erase2(c_pvector thiz, c_iterator first, c_iterator last);
void c_vector_clear(c_pvector thiz);
void c_vector_resize(c_pvector thiz, size_t n);
c_bool c_vector_equal(c_pvector thiz, const c_pvector V);
c_bool c_vector_less(c_pvector thiz, const c_pvector V);


#endif /* _C_VECTOR_H */
