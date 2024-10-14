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

#ifndef _C_TREE_H
#define _C_TREE_H

#include "c_def.h"
#include "c_iterator.h"
#include "c_memory.h"
#include "c_pair.h"
#include "c_function.h"

#define c_rb_tree 			_c_rb_tree
#define c_prb_tree 			_c_prb_tree
#define c_rb_tree_create		__c_rb_tree
#define c_rb_tree_destroy		__c_eert_br
#define c_rb_tree_assign		_c_rb_tree_assign
#define c_rb_tree_begin			_c_rb_tree_begin
#define c_rb_tree_end			_c_rb_tree_end
#define c_rb_tree_rbegin		_c_rb_tree_rbegin
#define c_rb_tree_rend			_c_rb_tree_rend
#define c_rb_tree_empty			_c_rb_tree_empty
#define c_rb_tree_size			_c_rb_tree_size
#define c_rb_tree_max_size		_c_rb_tree_max_size
#define c_rb_tree_swap			_c_rb_tree_swap
#define c_rb_tree_insert_unique		_c_rb_tree_insert_unique
#define c_rb_tree_insert_equal		_c_rb_tree_insert_equal
#define c_rb_tree_insert_unique1	_c_rb_tree_insert_unique1
#define c_rb_tree_insert_equal1		_c_rb_tree_insert_equal1
#define c_rb_tree_insert_unique2	_c_rb_tree_insert_unique2
#define c_rb_tree_insert_equal2		_c_rb_tree_insert_equal2
#define c_rb_tree_erase			_c_rb_tree_erase
#define c_rb_tree_erase1		_c_rb_tree_erase1
#define c_rb_tree_erase2		_c_rb_tree_erase2
#define c_rb_tree_clear			_c_rb_tree_clear
#define c_rb_tree_find			_c_rb_tree_find
#define c_rb_tree_count			_c_rb_tree_count
#define c_rb_tree_lower_bound		_c_rb_tree_lower_bound
#define c_rb_tree_upper_bound		_c_rb_tree_upper_bound
#define c_rb_tree_equal_range		_c_rb_tree_equal_range
#define c_rb_tree_less			_c_rb_tree_less
#define c_rb_tree_equal			_c_rb_tree_equal


typedef c_bool _c_rb_tree_color_type;
typedef value_type key_type;
typedef struct _c_rb_tree_node _c_rb_tree_node, * _c_prb_tree_node;
typedef _c_rb_tree_color_type _color_type;
typedef _c_rb_tree_node * _base_ptr;
typedef _base_ptr _link_type;

struct _c_rb_tree_node
{
    _color_type _A_color;
    _base_ptr _A_parent;
    _base_ptr _A_left;
    _base_ptr _A_right;
    value_type _A_value_field;
};

typedef struct c_rb_tree
{
	_base_ptr _A_header;
	size_type _A_node_count;
	COMPARER _A_key_compare;
	c_unary_function _A_keyofvalue;
}c_rb_tree, * c_prb_tree;



void __c_rb_tree(c_prb_tree thiz, COMPARER pcmp);
void __c_eert_br(c_prb_tree thiz);
c_prb_tree c_rb_tree_assign(c_prb_tree thiz, const c_prb_tree T);
c_iterator c_rb_tree_begin(c_prb_tree thiz);
c_iterator c_rb_tree_end(c_prb_tree thiz);
c_reverse_iterator c_rb_tree_rbegin(c_prb_tree thiz);
c_reverse_iterator c_rb_tree_rend(c_prb_tree thiz);
c_bool c_rb_tree_empty(c_prb_tree thiz);
size_type c_rb_tree_size(c_prb_tree thiz);
size_type c_rb_tree_max_size(c_prb_tree thiz);
void c_rb_tree_swap(c_prb_tree thiz, c_prb_tree T);
c_iter_bool_pair c_rb_tree_insert_unique(c_prb_tree thiz, const value_type val);
c_iterator c_rb_tree_insert_equal(c_prb_tree thiz, const value_type val);
c_iterator c_rb_tree_insert_unique1(c_prb_tree thiz, c_iterator position, const value_type val);
c_iterator c_rb_tree_insert_equal1(c_prb_tree thiz, c_iterator position, const value_type val);
void c_rb_tree_insert_unique2(c_prb_tree thiz, c_iterator first, c_iterator last);
void c_rb_tree_insert_equal2(c_prb_tree thiz, c_iterator first, c_iterator last);
void c_rb_tree_erase(c_prb_tree thiz, c_iterator position);
size_type c_rb_tree_erase1(c_prb_tree thiz, key_type key);
void c_rb_tree_erase2(c_prb_tree thiz, c_iterator first, c_iterator last);
void c_rb_tree_clear(c_prb_tree thiz);
c_iterator c_rb_tree_find(c_prb_tree thiz, key_type key);
size_type c_rb_tree_count(c_prb_tree thiz, key_type key);
c_iterator c_rb_tree_lower_bound(c_prb_tree thiz, key_type key);
c_iterator c_rb_tree_upper_bound(c_prb_tree thiz, key_type key);
c_iter_iter_pair c_rb_tree_equal_range(c_prb_tree thiz, key_type key);
c_bool c_rb_tree_less(c_prb_tree thiz, const c_prb_tree T, COMPARER cmp);
c_bool c_rb_tree_equal(c_prb_tree thiz, const c_prb_tree T, COMPARER cmp);
c_bool __c_rb_tree_verify(c_prb_tree thiz);


#endif /* _C_TREE_H */





