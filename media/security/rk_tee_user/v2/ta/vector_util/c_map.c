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

#include "c_map.h"

void __c_map(c_pmap thiz, COMPARER keycmp)
{
	c_prb_tree ptree = (c_prb_tree)__c_malloc(sizeof(c_rb_tree));
	__c_rb_tree(ptree, keycmp);
	ptree->_A_keyofvalue = c_select1st();
	thiz->_l = ptree;
}

void __c_pam(c_pmap thiz)
{
	__c_eert_br((c_prb_tree)thiz->_l);
	__c_free(thiz->_l);
}

c_pmap c_map_assign(c_pmap thiz, const c_pmap M)
{
	c_rb_tree_assign((c_prb_tree)thiz->_l, (c_prb_tree)M->_l);
	return thiz;
}

c_iterator c_map_begin(c_pmap thiz)
{
	return c_rb_tree_begin((c_prb_tree)thiz->_l);
}

c_iterator c_map_end(c_pmap thiz)
{
	return c_rb_tree_end((c_prb_tree)thiz->_l);
}

c_reverse_iterator c_map_rbegin(c_pmap thiz)
{
	return c_rb_tree_rbegin((c_prb_tree)thiz->_l);
}

c_reverse_iterator c_map_rend(c_pmap thiz)
{
	return c_rb_tree_rend((c_prb_tree)thiz->_l);
}

c_bool c_map_empty(c_pmap thiz)
{
	return c_rb_tree_empty((c_prb_tree)thiz->_l);
}

size_type c_map_size(c_pmap thiz)
{
	return c_rb_tree_size((c_prb_tree)thiz->_l);
}

size_type c_map_max_size(c_pmap thiz)
{
	return c_rb_tree_max_size((c_prb_tree)thiz->_l);
}

value_type c_map_at(c_pmap thiz, key_type key)
{
	c_iterator iter = c_map_lower_bound(thiz, key);
	c_iterator end = c_map_end(thiz);
	if(ITER_EQUAL(iter, end) ||
		((c_prb_tree)thiz->_l)->_A_key_compare(key,
						       ((c_ppair)ITER_REF(iter))->first) < 0)
		return NULL;	/* difference between STL */
	return ((c_ppair)ITER_REF(iter))->second;
}

void c_map_swap(c_pmap thiz, c_pmap M)
{
	c_rb_tree_swap((c_prb_tree)thiz->_l, (c_prb_tree)M->_l);
}

c_iter_bool_pair c_map_insert(c_pmap thiz, const value_type val)
{
	return c_rb_tree_insert_unique((c_prb_tree)thiz->_l, val);
}

c_iterator c_map_insert1(c_pmap thiz, c_iterator position, const value_type val)
{
	return c_rb_tree_insert_unique1((c_prb_tree)thiz->_l, position, val);
}

void c_map_insert2(c_pmap thiz, c_iterator first, c_iterator last)
{
	c_rb_tree_insert_unique2((c_prb_tree)thiz->_l, first, last);
}

void c_map_erase(c_pmap thiz, c_iterator position)
{
	c_rb_tree_erase((c_prb_tree)thiz->_l, position);
}

size_type c_map_erase1(c_pmap thiz, key_type key)
{
	return c_rb_tree_erase1((c_prb_tree)thiz->_l, key);
}

void c_map_erase2(c_pmap thiz, c_iterator first, c_iterator last)
{
	c_rb_tree_erase2((c_prb_tree)thiz->_l, first, last);
}

void c_map_clear(c_pmap thiz)
{
	c_rb_tree_clear((c_prb_tree)thiz->_l);
}

c_iterator c_map_find(c_pmap thiz, key_type key)
{
	return c_rb_tree_find((c_prb_tree)thiz->_l, key);
}

size_type c_map_count(c_pmap thiz, key_type key)
{
	c_iterator key_iter = c_rb_tree_find((c_prb_tree)thiz->_l, key);
	c_iterator end = c_rb_tree_end((c_prb_tree)thiz->_l);
	return ITER_EQUAL(key_iter, end) ? 0 : 1;
}

c_iterator c_map_lower_bound(c_pmap thiz, key_type key)
{
	return c_rb_tree_lower_bound((c_prb_tree)thiz->_l, key);
}

c_iterator c_map_upper_bound(c_pmap thiz, key_type key)
{
	return c_rb_tree_upper_bound((c_prb_tree)thiz->_l, key);
}

c_iter_iter_pair c_map_equal_range(c_pmap thiz, key_type key)
{
	return c_rb_tree_equal_range((c_prb_tree)thiz->_l, key);
}

c_bool c_map_less(c_pmap thiz, const c_pmap M, COMPARER paircmp)
{
	return c_rb_tree_less((c_prb_tree)thiz->_l, (c_prb_tree)M->_l, paircmp);
}

c_bool c_map_equal(c_pmap thiz, const c_pmap M, COMPARER paircmp)
{
	return c_rb_tree_equal((c_prb_tree)thiz->_l, (c_prb_tree)M->_l, paircmp);
}

