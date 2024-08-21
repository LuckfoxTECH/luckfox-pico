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

#include "c_tree.h"
#include "c_vector.h"
#include "c_algorithm.h"

static const _c_rb_tree_color_type _S_c_rb_tree_red = false;
static const _c_rb_tree_color_type _S_c_rb_tree_black = true;

static _base_ptr _S_minimum(_base_ptr val)
{
    while(val->_A_left)
        val = val->_A_left;
    return val;
}

static _base_ptr _S_maximum(_base_ptr val)
{
    while(val->_A_right)
        val = val->_A_right;
    return val;
}

static void _A_iterator_increment(c_piterator thiz)
{
    if(((_base_ptr)(thiz->_i))->_A_right)
    {
        thiz->_i = ((_base_ptr)(thiz->_i))->_A_right;
        while(((_base_ptr)(thiz->_i))->_A_left)
            thiz->_i = ((_base_ptr)(thiz->_i))->_A_left;
    }
    else
    {
        _base_ptr val = ((_base_ptr)(thiz->_i))->_A_parent;
        while(((_base_ptr)(thiz->_i)) == val->_A_right)
        {
            thiz->_i = val;
            val = val->_A_parent;
        }
        if(((_base_ptr)(thiz->_i))->_A_right != val)
            thiz->_i = val;
    } 
}

static void _A_iterator_decrement(c_piterator thiz)
{
    if((((_base_ptr)(thiz->_i))->_A_color == _S_c_rb_tree_red) &&
       ((_base_ptr)(thiz->_i))->_A_parent->_A_parent == ((_base_ptr)(thiz->_i)))
        thiz->_i = ((_base_ptr)(thiz->_i))->_A_right;
    else if(((_base_ptr)(thiz->_i))->_A_left)
    {
        _base_ptr val = ((_base_ptr)(thiz->_i))->_A_left;
        while(val->_A_right)
            val = val->_A_right;
        thiz->_i = val;
    }
    else
    {
        _base_ptr val = ((_base_ptr)(thiz->_i))->_A_parent;
        while(((_base_ptr)(thiz->_i)) == val->_A_left)
        {
            thiz->_i = val;
            val = val->_A_parent;
        }
        thiz->_i = val;
    }
}

static void _A_reverse_iterator_increment(c_preverse_iterator thiz)
{
	c_iterator iter;
	iter._i = thiz->_i;
	_A_iterator_decrement(&iter);
	thiz->_i = iter._i;
}

static void _A_reverse_iterator_decrement(c_preverse_iterator thiz)
{
	c_iterator iter;
	iter._i = thiz->_i;
	_A_iterator_increment(&iter);
	thiz->_i = iter._i;
}

static c_iterator _c_rb_tree_iterator_assign(c_piterator thiz, const c_piterator val)
{
    if(thiz != val)
        thiz->_i = val->_i;
    return *thiz;
}

static value_type _c_rb_tree_iterator_ref(c_piterator thiz)
{
    return ((_base_ptr)(thiz->_i))->_A_value_field;
}

static value_type _c_rb_tree_iterator_ref_assign(c_piterator thiz, const value_type val)
{
    return ((_base_ptr)(thiz->_i))->_A_value_field = val;
}

static c_iterator _c_rb_tree_iterator_inc(c_piterator thiz)
{
    _A_iterator_increment(thiz);
    return *thiz;
}

static c_iterator _c_rb_tree_iterator_dec(c_piterator thiz)
{
    _A_iterator_decrement(thiz);
    return *thiz;
}

static c_bool _c_rb_tree_iterator_equal(c_piterator thiz, const c_piterator val)
{
	return (thiz->_i == val->_i &&
				thiz->_pft == val->_pft);
}

static c_iterator_ft _c_rb_tree_iter_ft =
{
    _c_rb_tree_iterator_assign,
    _c_rb_tree_iterator_ref,
    _c_rb_tree_iterator_ref_assign,
    _c_rb_tree_iterator_inc,
    NULL, /* _c_rb_tree_iterator_inc_n */
    _c_rb_tree_iterator_dec,
    NULL, /* _c_rb_tree_iterator_dec_n */
    NULL, /* _c_rb_tree_iterator_diff */
    NULL, /* _c_rb_tree_iterator_at */
    NULL, /* _c_rb_tree_iterator_positive_n */
    NULL, /* _c_rb_tree_iterator_negative_n */
    _c_rb_tree_iterator_equal,
    NULL /* _c_rb_tree_iterator_less */
};

static c_iterator _A_get_iterator(_base_ptr val)
{
	c_iterator iter;
	iter._pft = &_c_rb_tree_iter_ft;
	iter._i = val;
	return iter;
}

static c_reverse_iterator _c_rb_tree_reverse_iterator_assign(c_preverse_iterator thiz,
								const c_preverse_iterator val)
{
	if(thiz != val)
		thiz->_i = val->_i;
	return *thiz;
}

static value_type _c_rb_tree_reverse_iterator_ref(c_preverse_iterator thiz)
{
	c_iterator iter = _A_get_iterator(thiz->_i);
	_A_iterator_decrement(&iter);
	return ((_base_ptr)(iter._i))->_A_value_field;
}

static value_type _c_rb_tree_reverse_iterator_ref_assign(c_preverse_iterator thiz, const value_type val)
{
	c_iterator iter = _A_get_iterator(thiz->_i);
	_A_iterator_decrement(&iter);
	return ((_base_ptr)(iter._i))->_A_value_field = val;
}

static c_reverse_iterator _c_rb_tree_reverse_iterator_inc(c_preverse_iterator thiz)
{
	_A_reverse_iterator_increment(thiz);
	return *thiz;
}

static c_reverse_iterator _c_rb_tree_reverse_iterator_dec(c_preverse_iterator thiz)
{
	_A_reverse_iterator_decrement(thiz);
	return *thiz;
}

static c_bool _c_rb_tree_reverse_iterator_equal(c_preverse_iterator thiz, const c_preverse_iterator val)
{
	return (thiz->_i == val->_i &&
				thiz->_pft == val->_pft);
}

static c_reverse_iterator_ft _c_rb_tree_reverse_iter_ft =
{
    _c_rb_tree_reverse_iterator_assign,
    _c_rb_tree_reverse_iterator_ref,
    _c_rb_tree_reverse_iterator_ref_assign,
    _c_rb_tree_reverse_iterator_inc,
    NULL, /* _c_rb_tree_reverse_iterator_inc_n */
    _c_rb_tree_reverse_iterator_dec,
    NULL, /* _c_rb_tree_reverse_iterator_dec_n */
    NULL, /* _c_rb_tree_reverse_iterator_diff */
    NULL, /* _c_rb_tree_reverse_iterator_at */
    NULL, /* _c_rb_tree_reverse_iterator_positive_n */
    NULL, /* _c_rb_tree_reverse_iterator_negative_n */
    _c_rb_tree_reverse_iterator_equal,
    NULL, /* _c_rb_tree_reverse_iterator_less */
};

static c_reverse_iterator _A_get_reverse_iterator(_base_ptr val)
{
	c_reverse_iterator iter;
	iter._pft = &_c_rb_tree_reverse_iter_ft;
	iter._i = val;
	return iter;
}

static void _c_rb_tree_rotate_left(_base_ptr val, _base_ptr * proot)
{
	_base_ptr wal = val->_A_right;
	val->_A_right = wal->_A_left;
	if(wal->_A_left != NULL)
		wal->_A_left->_A_parent = val;
	wal->_A_parent = val->_A_parent;

	if(val == *proot)
		*proot = wal;
	else if(val == val->_A_parent->_A_left)
		val->_A_parent->_A_left = wal;
	else
		val->_A_parent->_A_right = wal;
	wal->_A_left = val;
	val->_A_parent = wal;
}

static void _c_rb_tree_rotate_right(_base_ptr val, _base_ptr * proot)
{
	_base_ptr wal = val->_A_left;
	val->_A_left = wal->_A_right;
	if(wal->_A_right != NULL)
		wal->_A_right->_A_parent = val;
	wal->_A_parent = val->_A_parent;

	if(val == *proot)
		*proot = wal;
	else if(val == val->_A_parent->_A_right)
		val->_A_parent->_A_right = wal;
	else
		val->_A_parent->_A_left = wal;
	wal->_A_right = val;
	val->_A_parent = wal;
}

void _c_rb_tree_rebalance(_base_ptr val, _base_ptr * proot)
{
	val->_A_color = _S_c_rb_tree_red;
	while(val != *proot && val->_A_parent->_A_color == _S_c_rb_tree_red)
	{
		if(val->_A_parent == val->_A_parent->_A_parent->_A_left)
		{
			_base_ptr wal = val->_A_parent->_A_parent->_A_right;
			if(wal && wal->_A_color == _S_c_rb_tree_red)
			{
				val->_A_parent->_A_color = _S_c_rb_tree_black;
				wal->_A_color = _S_c_rb_tree_black;
				val->_A_parent->_A_parent->_A_color = _S_c_rb_tree_red;
				val = val->_A_parent->_A_parent;
			}
			else
			{
				if(val == val->_A_parent->_A_right)
				{
					val = val->_A_parent;
					_c_rb_tree_rotate_left(val, proot);
				}
				val->_A_parent->_A_color = _S_c_rb_tree_black;
				val->_A_parent->_A_parent->_A_color = _S_c_rb_tree_red;
				_c_rb_tree_rotate_right(val->_A_parent->_A_parent, proot);
			}
		}
		else
		{
			_base_ptr wal = val->_A_parent->_A_parent->_A_left;
			if(wal && wal->_A_color == _S_c_rb_tree_red)
			{
				val->_A_parent->_A_color = _S_c_rb_tree_black;
				wal->_A_color = _S_c_rb_tree_black;
				val->_A_parent->_A_parent->_A_color = _S_c_rb_tree_red;
				val = val->_A_parent->_A_parent;
			}
			else
			{
				if(val == val->_A_parent->_A_left)
				{
					val = val->_A_parent;
					_c_rb_tree_rotate_right(val, proot);
				}
				val->_A_parent->_A_color = _S_c_rb_tree_black;
				val->_A_parent->_A_parent->_A_color = _S_c_rb_tree_red;
				_c_rb_tree_rotate_left(val->_A_parent->_A_parent, proot);
			}
		}
	}
	(*proot)->_A_color = _S_c_rb_tree_black;
}

static _base_ptr _c_rb_tree_rebalance_for_erase(_base_ptr z,
						     _base_ptr * root,
						     _base_ptr * leftmost,
						     _base_ptr * rightmost)
{
	_base_ptr y = z;
	_base_ptr x = NULL;
	_base_ptr x_parent = NULL;

	if(y->_A_left == NULL)
		x = y->_A_right;
	else
		if(y->_A_right == NULL)
			x = y->_A_left;
		else
		{
			y = y->_A_right;
			while(y->_A_left != NULL)
				y = y->_A_left;
			x = y->_A_right;
		}

	if(y != z)
	{
		_c_rb_tree_color_type tmp;
		z->_A_left->_A_parent = y;
		y->_A_left = z->_A_left;
		if(y != z->_A_right)
		{
			x_parent = y->_A_parent;
			if(x)
				x->_A_parent = y->_A_parent;
			y->_A_parent->_A_left = x;
			y->_A_right = z->_A_right;
			z->_A_right->_A_parent = y;
		}
		else
			x_parent = y;
		if(*root == z)
			*root = y;
		else if(z->_A_parent->_A_left == z)
			z->_A_parent->_A_left = y;
		else
			z->_A_parent->_A_right = y;
		y->_A_parent = z->_A_parent;
		C_SWAP(y->_A_color, z->_A_color, tmp);
		y = z;
	}
	else
	{
		x_parent = y->_A_parent;
		if(x)
			x->_A_parent = y->_A_parent;
		if(*root == z)
			*root = x;
		else
		{
			if(z->_A_parent->_A_left == z)
				z->_A_parent->_A_left = x;
			else
				z->_A_parent->_A_right = x;
		}
		if(*leftmost == z)
		{
			if(z->_A_right == NULL)
				*leftmost = z->_A_parent;
			else
				*leftmost = _S_minimum(x);
		}
		if(*rightmost == z)
		{
			if(z->_A_left == NULL)
				*rightmost = z->_A_parent;
			else
				*rightmost = _S_maximum(x);
		}
	}
	if(y->_A_color != _S_c_rb_tree_red)
	{
		while(x != *root &&
			(x == NULL ||
			 x->_A_color == _S_c_rb_tree_black))
			if(x == x_parent->_A_left)
			{
				_base_ptr w = x_parent->_A_right;
				if(w->_A_color == _S_c_rb_tree_red)
				{
					w->_A_color = _S_c_rb_tree_black;
					x_parent->_A_color = _S_c_rb_tree_red;
					_c_rb_tree_rotate_left(x_parent, root);
					w = x_parent->_A_right;
				}
				if((w->_A_left == NULL ||
				    w->_A_left->_A_color == _S_c_rb_tree_black) &&
				   (w->_A_right == NULL ||
				    w->_A_right->_A_color == _S_c_rb_tree_black))
				{
					w->_A_color = _S_c_rb_tree_red;
					x = x_parent;
					x_parent = x_parent->_A_parent;
				}
				else
				{
					if(w->_A_right == NULL ||
					   w->_A_right->_A_color == _S_c_rb_tree_black)
					{
						if(w->_A_left)
							w->_A_left->_A_color = _S_c_rb_tree_black;
						w->_A_color = _S_c_rb_tree_red;
						_c_rb_tree_rotate_right(w, root);
						w = x_parent->_A_right;
					}
					w->_A_color = x_parent->_A_color;
					x_parent->_A_color = _S_c_rb_tree_black;
					if(w->_A_right)
						w->_A_right->_A_color = _S_c_rb_tree_black;
					_c_rb_tree_rotate_left(x_parent, root);
					break;
				}
			}
			else
			{
				_base_ptr w = x_parent->_A_left;
				if(w->_A_color == _S_c_rb_tree_red)
				{
					w->_A_color = _S_c_rb_tree_black;
					x_parent->_A_color = _S_c_rb_tree_red;
					_c_rb_tree_rotate_right(x_parent, root);
					w = x_parent->_A_left;
				}
				if((w->_A_right == NULL ||
				    w->_A_right->_A_color == _S_c_rb_tree_black) &&
				   (w->_A_left == NULL ||
				    w->_A_left->_A_color == _S_c_rb_tree_black))
				{
					w->_A_color = _S_c_rb_tree_red;
					x = x_parent;
					x_parent = x_parent->_A_parent;
				}
				else
				{
					if(w->_A_left == NULL ||
						w->_A_left->_A_color == _S_c_rb_tree_black)
					{
						if(w->_A_right)
							w->_A_right->_A_color = _S_c_rb_tree_black;
						w->_A_color = _S_c_rb_tree_red;
						_c_rb_tree_rotate_left(w, root);
						w = x_parent->_A_left;
					}
					w->_A_color = x_parent->_A_color;
					x_parent->_A_color = _S_c_rb_tree_black;
					if(w->_A_left)
						w->_A_left->_A_color = _S_c_rb_tree_black;
					_c_rb_tree_rotate_right(x_parent, root);
					break;
				}
			}
		if(x)
			x->_A_color = _S_c_rb_tree_black;
	}
	return y;
}

static _base_ptr _A_get_node()
{
	_base_ptr tmp = __c_malloc(sizeof(_c_rb_tree_node));
	memset(tmp, 0, sizeof(_c_rb_tree_node));
	return tmp;
}

static void _A_put_node(_base_ptr val)
{
	__c_free(val);
}

static _base_ptr _A_create_node(const value_type val)
{
	_base_ptr tmp = _A_get_node();
	tmp->_A_value_field = val;
	return tmp;
}

static _base_ptr _A_clone_node(_base_ptr val)
{
	_base_ptr tmp = _A_create_node(val->_A_value_field);
	tmp->_A_color = val->_A_color;
	tmp->_A_left = NULL;
	tmp->_A_right = NULL;
	return tmp;
}

static void _A_destroy_node(_base_ptr val)
{
	_A_put_node(val);
}

static _base_ptr * _A_root(c_prb_tree thiz)
{
	return &thiz->_A_header->_A_parent;
}

static _base_ptr * _A_leftmost(c_prb_tree thiz)
{
	return &thiz->_A_header->_A_left;
}

static _base_ptr * _A_rightmost(c_prb_tree thiz)
{
	return &thiz->_A_header->_A_right;
}

static _base_ptr * _S_left(_base_ptr val)
{
	return &val->_A_left;
}

static _base_ptr * _S_right(_base_ptr val)
{
	return &val->_A_right;
}

static _base_ptr * _S_parent(_base_ptr val)
{
	return &val->_A_parent;
}

static value_type * _S_value(_base_ptr val)
{
	return &val->_A_value_field;
}

static key_type _S_key(c_prb_tree thiz, _base_ptr val)
{
	return thiz->_A_keyofvalue.O(&thiz->_A_keyofvalue, *_S_value(val));
}

static _color_type * _S_color(_base_ptr val)
{
	return &val->_A_color;
}

static c_iterator _A_insert(c_prb_tree thiz, _base_ptr x, _base_ptr y, const value_type val)
{
	_base_ptr _x = x;
	_base_ptr _y = y;
	_base_ptr _z = NULL;
	if(_y == thiz->_A_header ||
		_x != NULL ||
		thiz->_A_key_compare(thiz->_A_keyofvalue.O(&thiz->_A_keyofvalue, val),
			_S_key(thiz, _y)) < 0)
	{
		_z = _A_create_node(val);
		*_S_left(_y) = _z;

		if(_y == thiz->_A_header)
		{
			*_A_root(thiz) = _z;
			*_A_rightmost(thiz) = _z;
		}
		else if(_y == *_A_leftmost(thiz))
			*_A_leftmost(thiz) = _z;
	}
	else
	{
		_z = _A_create_node(val);
		*_S_right(_y) = _z;
		if(_y == *_A_rightmost(thiz))
			*_A_rightmost(thiz) = _z;
	}

	*_S_parent(_z) = _y;
	*_S_left(_z) = NULL;
	*_S_right(_z) = NULL;
	_c_rb_tree_rebalance(_z, &thiz->_A_header->_A_parent);
	++ thiz->_A_node_count;
	return _A_get_iterator(_z);
}

static _base_ptr _A_copy(_base_ptr x, _base_ptr p)
{
	_base_ptr top = _A_clone_node(x);
	top->_A_parent = p;

	if(x->_A_right)
		top->_A_right = _A_copy(*_S_right(x), top);
	p = top;
	x = *_S_left(x);

	while(x != NULL)
	{
		_base_ptr y = _A_clone_node(x);
		p->_A_left = y;
		y->_A_parent = p;
		if(x->_A_right)
			y->_A_right = _A_copy(*_S_right(x), y);
		p = y;
		x = *_S_left(x);
	}

	return top;
}

static void _A_erase(_base_ptr x)
{
	while(x != NULL)
	{
		_base_ptr y;
		_A_erase(*_S_right(x));
		y = *_S_left(x);
		_A_destroy_node(x);
		x = y;
	}
}

static void _A_empty_initialize(c_prb_tree thiz)
{
	*_S_color(thiz->_A_header) = _S_c_rb_tree_red;
	*_A_root(thiz) = NULL;
	*_A_leftmost(thiz) = thiz->_A_header;
	*_A_rightmost(thiz) = thiz->_A_header;
}

void __c_rb_tree(c_prb_tree thiz, COMPARER pcmp)
{
	thiz->_A_header = _A_get_node();
	thiz->_A_node_count = 0;
	thiz->_A_key_compare = pcmp;
	_A_empty_initialize(thiz);
}

void __c_eert_br(c_prb_tree thiz)
{
	c_rb_tree_clear(thiz);
	_A_put_node(thiz->_A_header);
}

c_prb_tree c_rb_tree_assign(c_prb_tree thiz, const c_prb_tree T)
{
	if(thiz != T)
	{
		c_rb_tree_clear(thiz);
		thiz->_A_node_count = 0;
		thiz->_A_key_compare = T->_A_key_compare;
		if(*_A_root(T) == NULL)
		{
			*_A_root(thiz) = NULL;
			*_A_leftmost(thiz) = thiz->_A_header;
			*_A_rightmost(thiz) = thiz->_A_header;
		}
		else
		{
			*_A_root(thiz) = _A_copy(*_A_root(T), thiz->_A_header);
			*_A_leftmost(thiz) = _S_minimum(*_A_root(thiz));
			*_A_rightmost(thiz) = _S_maximum(*_A_root(thiz));
			thiz->_A_node_count = T->_A_node_count;
		}
	}
	return thiz;
}

c_iterator c_rb_tree_begin(c_prb_tree thiz)
{
	return _A_get_iterator(*_A_leftmost(thiz));
}

c_iterator c_rb_tree_end(c_prb_tree thiz)
{
	return _A_get_iterator(thiz->_A_header);
}

c_reverse_iterator c_rb_tree_rbegin(c_prb_tree thiz)
{
	return _A_get_reverse_iterator(thiz->_A_header);
}

c_reverse_iterator c_rb_tree_rend(c_prb_tree thiz)
{
	return _A_get_reverse_iterator(*_A_leftmost(thiz));
}

c_bool c_rb_tree_empty(c_prb_tree thiz)
{
	return thiz->_A_node_count == 0;
}

size_type c_rb_tree_size(c_prb_tree thiz)
{
	return thiz->_A_node_count;
}

size_type c_rb_tree_max_size(c_prb_tree thiz)
{
	return (size_type)(-1);
}

void c_rb_tree_swap(c_prb_tree thiz, c_prb_tree T)
{
	c_rb_tree tmp;
	C_SWAP(*thiz, *T, tmp);
}

c_iter_bool_pair c_rb_tree_insert_unique(c_prb_tree thiz, const value_type val)
{
	_base_ptr y = thiz->_A_header;
	_base_ptr x = *_A_root(thiz);
	c_bool comp = 1;
	c_iterator j;

	while(x != NULL)
	{
		y = x;
		comp = thiz->_A_key_compare(thiz->_A_keyofvalue.O(&thiz->_A_keyofvalue, val),
									_S_key(thiz, x)) < 0;
		x = comp ? *_S_left(x) : *_S_right(x);
	}
	j = _A_get_iterator(y);
	if(comp)
	{
		c_iterator beg = c_rb_tree_begin(thiz);
		if(ITER_EQUAL(j, beg))
			return c_make_iter_bool_pair(_A_insert(thiz, x, y, val), true);
		else
			ITER_DEC(j);
	}
	if(thiz->_A_key_compare(_S_key(thiz, j._i),
				thiz->_A_keyofvalue.O(&thiz->_A_keyofvalue, val)) < 0)
		return c_make_iter_bool_pair(_A_insert(thiz, x, y, val), true);
	return c_make_iter_bool_pair(j, false);
}

c_iterator c_rb_tree_insert_equal(c_prb_tree thiz, const value_type val)
{
	_base_ptr y = thiz->_A_header;
	_base_ptr x = *_A_root(thiz);
	while(x != NULL)
	{
		y = x;
		x = thiz->_A_key_compare(thiz->_A_keyofvalue.O(&thiz->_A_keyofvalue, val),
					_S_key(thiz, x)) < 0 ?
			*_S_left(x) : *_S_right(x);
	}
	return _A_insert(thiz, x, y, val);
}

c_iterator c_rb_tree_insert_unique1(c_prb_tree thiz, c_iterator position, const value_type val)
{
	if(position._i == thiz->_A_header->_A_left)
	{
		if(c_rb_tree_size(thiz) > 0 &&
			thiz->_A_key_compare(thiz->_A_keyofvalue.O(&thiz->_A_keyofvalue, val),
						_S_key(thiz, position._i)) < 0)
			return _A_insert(thiz, position._i, position._i, val);
		else
			return c_rb_tree_insert_unique(thiz, val).first;
	}
	else if(position._i == thiz->_A_header)
	{
		if(thiz->_A_key_compare(_S_key(thiz, *_A_rightmost(thiz)),
					thiz->_A_keyofvalue.O(&thiz->_A_keyofvalue,
								val)) < 0)
			return _A_insert(thiz, 0, *_A_rightmost(thiz), val);
		else
			return c_rb_tree_insert_unique(thiz, val).first;
	}
	else
	{
		c_iterator before = position;
		ITER_DEC(before);
		if(thiz->_A_key_compare(_S_key(thiz, before._i),
					thiz->_A_keyofvalue.O(&thiz->_A_keyofvalue,
								val)) < 0 &&
			thiz->_A_key_compare(thiz->_A_keyofvalue.O(&thiz->_A_keyofvalue,
									val),
						_S_key(thiz, position._i)) < 0)
			if(*_S_right(before._i) == NULL)
				return _A_insert(thiz, 0, before._i, val);
			else
				return _A_insert(thiz, position._i, position._i, val);
		else
			return c_rb_tree_insert_unique(thiz, val).first;
	}
}

c_iterator c_rb_tree_insert_equal1(c_prb_tree thiz, c_iterator position, const value_type val)
{
	if(position._i == thiz->_A_header->_A_left)
	{
		if(c_rb_tree_size(thiz) > 0 &&
			thiz->_A_key_compare(_S_key(thiz, position._i),
						thiz->_A_keyofvalue.O(&thiz->_A_keyofvalue,
									val)) >= 0)
			return _A_insert(thiz, position._i, position._i, val);
		else
			return c_rb_tree_insert_equal(thiz, val);
	}
	else if(position._i == thiz->_A_header)
	{
		if(thiz->_A_key_compare(thiz->_A_keyofvalue.O(&thiz->_A_keyofvalue, val),
					_S_key(thiz, *_A_rightmost(thiz)))  >= 0)
			return _A_insert(thiz, 0, *_A_rightmost(thiz), val);
		else
			return c_rb_tree_insert_equal(thiz, val);
	}
	else
	{
		c_iterator before = position;
		ITER_DEC(before);
		if(thiz->_A_key_compare(thiz->_A_keyofvalue.O(&thiz->_A_keyofvalue,
						val),
					_S_key(thiz, before._i)) >= 0 &&
			thiz->_A_key_compare(_S_key(thiz, position._i),
						thiz->_A_keyofvalue.O(&thiz->_A_keyofvalue,
									val)) >= 0)
		{
			if(*_S_right(before._i) == NULL)
				return _A_insert(thiz, 0, before._i, val);
			else
				return _A_insert(thiz, position._i, position._i, val);
		}
		else
			return c_rb_tree_insert_equal(thiz, val);

	}
}

void c_rb_tree_insert_unique2(c_prb_tree thiz, c_iterator first, c_iterator last)
{
	for(; !ITER_EQUAL(first, last); ITER_INC(first))
		c_rb_tree_insert_unique(thiz, ITER_REF(first));
}

void c_rb_tree_insert_equal2(c_prb_tree thiz, c_iterator first, c_iterator last)
{
	for(; !ITER_EQUAL(first, last); ITER_INC(first))
		c_rb_tree_insert_equal(thiz, ITER_REF(first));
}

void c_rb_tree_erase(c_prb_tree thiz, c_iterator position)
{
	_base_ptr y = _c_rb_tree_rebalance_for_erase(position._i,
							&thiz->_A_header->_A_parent,
							&thiz->_A_header->_A_left,
							&thiz->_A_header->_A_right);
	_A_destroy_node(y);
	-- thiz->_A_node_count;
}

size_type c_rb_tree_erase1(c_prb_tree thiz, key_type key)
{
	c_iter_iter_pair p = c_rb_tree_equal_range(thiz, key);
	difference_type n = 0;
	c_distance1(p.first, p.second, &n);
	c_rb_tree_erase2(thiz, p.first, p.second);
	return n;
}

void c_rb_tree_erase2(c_prb_tree thiz, c_iterator first, c_iterator last)
{
	c_iterator begin, end;
	begin = c_rb_tree_begin(thiz);
	end = c_rb_tree_end(thiz);
	if(ITER_EQUAL(first, begin) &&
		ITER_EQUAL(last, end))
		c_rb_tree_clear(thiz);
	else
		while(!ITER_EQUAL(first, last))
		{
			c_rb_tree_erase(thiz, first);
			ITER_INC(first);
		}
}

void c_rb_tree_clear(c_prb_tree thiz)
{
	if(thiz->_A_node_count != 0)
	{
		_A_erase(*_A_root(thiz));
		*_A_leftmost(thiz) = thiz->_A_header;
		*_A_root(thiz) = NULL;
		*_A_rightmost(thiz) = thiz->_A_header;
		thiz->_A_node_count = 0;
	}
}


c_iterator c_rb_tree_find(c_prb_tree thiz, key_type key)
{
	_base_ptr y = thiz->_A_header;
	_base_ptr x = *_A_root(thiz);
	c_iterator j;
	c_iterator end = c_rb_tree_end(thiz);

	while(x != NULL)
		if(thiz->_A_key_compare(_S_key(thiz, x), key) >= 0)
		{
			y = x;
			x = *_S_left(x);
		}
		else
			x = *_S_right(x);
	j = _A_get_iterator(y);
	return (ITER_EQUAL(j, end) ||
			thiz->_A_key_compare(key, _S_key(thiz, j._i)) < 0) ?
			end : j;
}

size_type c_rb_tree_count(c_prb_tree thiz, key_type key)
{
	c_iter_iter_pair p = c_rb_tree_equal_range(thiz, key);
	difference_type n = 0;
	c_distance1(p.first, p.second, &n);
	return abs(n);
}

c_iterator c_rb_tree_lower_bound(c_prb_tree thiz, key_type key)
{
	_base_ptr y = thiz->_A_header;
	_base_ptr x = *_A_root(thiz);

	while(x != NULL)
		if(thiz->_A_key_compare(_S_key(thiz, x), key) >= 0)
		{
			y = x;
			x = *_S_left(x);
		}
		else
			x = *_S_right(x);

	return _A_get_iterator(y);
}

c_iterator c_rb_tree_upper_bound(c_prb_tree thiz, key_type key)
{
	_base_ptr y = thiz->_A_header;
	_base_ptr x = *_A_root(thiz);

	while(x != NULL)
		if(thiz->_A_key_compare(key, _S_key(thiz, x)) < 0)
		{
			y = x;
			x = *_S_left(x);
		}
		else
			x = *_S_right(x);

	return _A_get_iterator(y);
}

c_iter_iter_pair c_rb_tree_equal_range(c_prb_tree thiz, key_type key)
{
	return c_make_iter_iter_pair(c_rb_tree_lower_bound(thiz, key),
					c_rb_tree_upper_bound(thiz, key));
}

c_bool c_rb_tree_less(c_prb_tree thiz, const c_prb_tree T, COMPARER cmp)
{
	return c_lexicographical_compare(c_rb_tree_begin(thiz),
					 c_rb_tree_end(thiz),
					 c_rb_tree_begin(T),
					 c_rb_tree_end(T),
					 cmp);
}

c_bool c_rb_tree_equal(c_prb_tree thiz, const c_prb_tree T, COMPARER cmp)
{
	c_binary_predicate bpred = c_binary_negate(cmp);
	return c_rb_tree_size(thiz) == c_rb_tree_size(T) &&
		c_equal2(c_rb_tree_begin(thiz), c_rb_tree_end(thiz), c_rb_tree_begin(T), bpred);
}

static int __black_count(_base_ptr node, _base_ptr root)
{
	if(node == NULL)
		return 0;
	else
	{
		int bc = node->_A_color == _S_c_rb_tree_black ? 1 : 0;
		if(node == root)
			return bc;
		else
			return bc + __black_count(node->_A_parent, root);
	}
}

c_bool __c_rb_tree_verify(c_prb_tree thiz)
{
	c_iterator begin = c_rb_tree_begin(thiz);
	c_iterator end = c_rb_tree_end(thiz);
	int len;
	c_iterator iter;

	if(thiz->_A_node_count == 0 || ITER_EQUAL(begin, end))
		return thiz->_A_node_count == 0 && ITER_EQUAL(begin, end) &&
			thiz->_A_header->_A_left == thiz->_A_header &&
			thiz->_A_header->_A_right == thiz->_A_header;

	len = __black_count(*_A_leftmost(thiz), *_A_root(thiz));
	for(iter = c_rb_tree_begin(thiz); !ITER_EQUAL(iter, end); ITER_INC(iter))
	{
		_base_ptr x = iter._i;
		_base_ptr L = *_S_left(x);
		_base_ptr R = *_S_right(x);

		if(x->_A_color == _S_c_rb_tree_red)
			if((L && L->_A_color == _S_c_rb_tree_red) ||
				(R && R->_A_color == _S_c_rb_tree_red))
			return false;

		if(L && thiz->_A_key_compare(_S_key(thiz, x), _S_key(thiz, L)) < 0)
			return false;
		if(R && thiz->_A_key_compare(_S_key(thiz, R), _S_key(thiz, x)) < 0)
			return false;

		if(!L && !R && __black_count(x, *_A_root(thiz)) != len)
			return false;
	}

	if(*_A_leftmost(thiz) != _S_minimum(*_A_root(thiz)))
		return false;
	if(*_A_rightmost(thiz) != _S_maximum(*_A_root(thiz)))
		return false;

	return true;
}
