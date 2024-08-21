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

#include "c_vector.h"
#include "c_algo.h"
#include "c_memory.h"

typedef value_type node_t;
typedef node_t  * pnode_t;

typedef struct _c_vector_impl
{
	pnode_t _start;
	pnode_t _finish;
	pnode_t _end_of_storage;
} _c_vector_impl;

static c_iterator _A_get_iterator(pnode_t pn);
static c_reverse_iterator _A_get_reverse_iterator(pnode_t pn);

int  _abs(signed int i){
    return ((i >> 31) ^ i) - (i >> 31);
}

static c_iterator _c_vector_iterator_assign(c_piterator thiz, const c_piterator val)
{
	if(thiz != val)
		thiz->_i = val->_i;
	return *thiz;
}

static value_type _c_vector_iterator_ref(c_piterator thiz)
{
	return *(pnode_t)thiz->_i;
}

static value_type _c_vector_iterator_ref_assign(c_piterator thiz, const value_type val)
{
	return *(pnode_t)thiz->_i = val;
}

static c_iterator _c_vector_iterator_inc(c_piterator thiz)
{
	pnode_t pn = thiz->_i;
	++ pn;
	thiz->_i = pn;
	return *thiz;
}

static c_iterator _c_vector_iterator_inc_n(c_piterator thiz, difference_type n)
{
	pnode_t pn = thiz->_i;
	pn += n;
	thiz->_i = pn;
	return *thiz;
}

static c_iterator _c_vector_iterator_dec(c_piterator thiz)
{
	pnode_t pn = thiz->_i;
	-- pn;
	thiz->_i = pn;
	return *thiz;
}

static c_iterator _c_vector_iterator_dec_n(c_piterator thiz, difference_type n)
{
	pnode_t pn = thiz->_i;
	pn -= n;
	thiz->_i = pn;
	return *thiz;
}

static difference_type _c_vector_iterator_diff(c_piterator thiz, const c_piterator val)
{
	return ((pnode_t)thiz->_i - (pnode_t)val->_i);
}

static value_type _c_vector_iterator_at(c_piterator thiz, difference_type n)
{
	return *((pnode_t)thiz->_i + n);
}

static c_iterator _c_vector_iterator_positive_n(c_piterator thiz, difference_type n)
{
	c_iterator iter;
	pnode_t pn = thiz->_i;
	pn += n;
	iter = _A_get_iterator(pn);
	return iter;
}

static c_iterator _c_vector_iterator_negative_n(c_piterator thiz, difference_type n)
{
	c_iterator iter;
	pnode_t pn = thiz->_i;
	pn -= n;
	iter = _A_get_iterator(pn);
	return iter;
}

static c_bool _c_vector_iterator_equal(c_piterator thiz, const c_piterator val)
{
	return (thiz->_i == val->_i &&
				thiz->_pft == val->_pft);
}

static c_bool _c_vector_iterator_less(c_piterator thiz, const c_piterator val)
{
	return ((pnode_t)thiz->_i < (pnode_t)val->_i);
}

static c_iterator_ft _c_vector_iter_ft =
{
	_c_vector_iterator_assign,
	_c_vector_iterator_ref,
	_c_vector_iterator_ref_assign,
	_c_vector_iterator_inc,
	_c_vector_iterator_inc_n,
	_c_vector_iterator_dec,
	_c_vector_iterator_dec_n,
	_c_vector_iterator_diff,
	_c_vector_iterator_at,
	_c_vector_iterator_positive_n,
	_c_vector_iterator_negative_n,
	_c_vector_iterator_equal,
	_c_vector_iterator_less
};

static c_reverse_iterator _c_vector_reverse_iterator_assign(c_preverse_iterator thiz, const c_preverse_iterator val)
{
	if(thiz != val)
		thiz->_i = val->_i;
	return *thiz;
}

static value_type _c_vector_reverse_iterator_ref(c_preverse_iterator thiz)
{
	return *(pnode_t)thiz->_i;
}

static value_type _c_vector_reverse_iterator_ref_assign(c_preverse_iterator thiz, const value_type val)
{
	return *(pnode_t)thiz->_i = val;
}

static c_reverse_iterator _c_vector_reverse_iterator_inc(c_preverse_iterator thiz)
{
	pnode_t pn = thiz->_i;
	-- pn;
	thiz->_i = pn;
	return *thiz;
}

static c_reverse_iterator _c_vector_reverse_iterator_inc_n(c_preverse_iterator thiz, difference_type n)
{
	pnode_t pn = thiz->_i;
	pn -= n;
	thiz->_i = pn;
	return *thiz;
}

static c_reverse_iterator _c_vector_reverse_iterator_dec(c_preverse_iterator thiz)
{
	pnode_t pn = thiz->_i;
	++ pn;
	thiz->_i = pn;
	return *thiz;
}

static c_reverse_iterator _c_vector_reverse_iterator_dec_n(c_preverse_iterator thiz, difference_type n)
{
	pnode_t pn = thiz->_i;
	pn += n;
	thiz->_i = pn;
	return *thiz;
}

static difference_type _c_vector_reverse_iterator_diff(c_preverse_iterator thiz, const c_preverse_iterator val)
{
	return ((pnode_t)val->_i - (pnode_t)thiz->_i);
}

static value_type _c_vector_reverse_iterator_at(c_preverse_iterator thiz, difference_type n)
{
	return *((pnode_t)thiz->_i - n);
}

static c_reverse_iterator _c_vector_reverse_iterator_positive_n(c_preverse_iterator thiz, difference_type n)
{
	c_reverse_iterator iter;
	pnode_t pn = thiz->_i;
	pn -= n;
	iter = _A_get_reverse_iterator(pn);
	return iter;
}

static c_reverse_iterator _c_vector_reverse_iterator_negative_n(c_preverse_iterator thiz, difference_type n)
{
	c_reverse_iterator iter;
	pnode_t pn = thiz->_i;
	pn += n;
	iter = _A_get_reverse_iterator(pn);
	return iter;
}

static c_bool _c_vector_reverse_iterator_equal(c_preverse_iterator thiz, const c_preverse_iterator val)
{
	return (thiz->_i == val->_i &&
				thiz->_pft == val->_pft);
}

static c_bool _c_vector_reverse_iterator_less(c_preverse_iterator thiz, const c_preverse_iterator val)
{
	return ((pnode_t)thiz->_i > (pnode_t)val->_i);
}

static c_reverse_iterator_ft _c_vector_rev_iter_ft =
{
	_c_vector_reverse_iterator_assign,
	_c_vector_reverse_iterator_ref,
	_c_vector_reverse_iterator_ref_assign,
	_c_vector_reverse_iterator_inc,
	_c_vector_reverse_iterator_inc_n,
	_c_vector_reverse_iterator_dec,
	_c_vector_reverse_iterator_dec_n,
	_c_vector_reverse_iterator_diff,
	_c_vector_reverse_iterator_at,
	_c_vector_reverse_iterator_positive_n,
	_c_vector_reverse_iterator_negative_n,
	_c_vector_reverse_iterator_equal,
	_c_vector_reverse_iterator_less
};

static c_iterator _A_get_iterator(pnode_t pn)
{
	c_iterator iter;
	iter._pft = &_c_vector_iter_ft;
	iter._i = pn;
	return iter;
}

static c_reverse_iterator _A_get_reverse_iterator(pnode_t pn)
{
	c_reverse_iterator iter;
	iter._pft = &_c_vector_rev_iter_ft;
	iter._i = pn;
	return iter;
}

static pnode_t _A_allocate(c_pvector thiz, size_t n)
{
	return __c_malloc(n * sizeof(node_t));
}

static void _A_deallocate(c_pvector thiz, pnode_t p, size_t n)
{
	if(n > 0)
		__c_free(p);
}

static c_iterator _A_allocate_and_copy(c_pvector thiz, size_type n, c_const_iterator first, c_const_iterator last)
{
	c_iterator result = _A_get_iterator(_A_allocate(thiz, n));
	c_uninitialized_copy(first, last, result);
	return result;
}

static void _A_insert_aux1(c_pvector thiz, c_iterator pos, node_t val)
{
	_c_vector_impl * pl = thiz->_l;
	if(pl->_finish != pl->_end_of_storage)
	{
		node_t node;
		*pl->_finish = *(pl->_finish - 1);
		++ pl->_finish;
		node = val;
		c_copy_backward(pos,
				_A_get_iterator(pl->_finish - 2),
				_A_get_iterator(pl->_finish - 1));
		ITER_REF_ASSIGN(pos, node);
	}
	else
	{
		const size_type old_size = c_vector_size(thiz);
		const size_type len = old_size != 0 ? 2 * old_size : 1;
		c_iterator new_start = _A_get_iterator(_A_allocate(thiz, len));
		c_iterator new_finish = new_start;
		new_finish = c_copy(_A_get_iterator(pl->_start), pos, new_start);
		ITER_REF_ASSIGN(new_finish, val);
		ITER_INC(new_finish);
		new_finish = c_copy(pos, _A_get_iterator(pl->_finish), new_finish);
		_A_deallocate(thiz, pl->_start, abs(pl->_end_of_storage - pl->_start));
		pl->_start = new_start._i;
		pl->_finish = new_finish._i;
		pl->_end_of_storage = pl->_start + len;
	}
}

void __c_vector(c_pvector thiz, COMPARER pcmp)
{
	thiz->_cmp = pcmp;
	thiz->_l = __c_malloc(sizeof(_c_vector_impl));
	((_c_vector_impl *)thiz->_l)->_start = NULL;
	((_c_vector_impl *)thiz->_l)->_finish = NULL;
	((_c_vector_impl *)thiz->_l)->_end_of_storage = NULL;
}

void __c_rotcev(c_pvector thiz)
{
	_c_vector_impl * pl = thiz->_l;
	_A_deallocate(thiz, pl->_start, abs(pl->_end_of_storage - pl->_start));
	__c_free(thiz->_l);
}

c_iterator c_vector_begin(c_pvector thiz)
{
	return _A_get_iterator(((_c_vector_impl*)thiz->_l)->_start);
}

c_iterator c_vector_end(c_pvector thiz)
{
	return _A_get_iterator(((_c_vector_impl*)thiz->_l)->_finish);
}

c_reverse_iterator c_vector_rbegin(c_pvector thiz)
{
	return _A_get_reverse_iterator(((_c_vector_impl*)thiz->_l)->_finish - 1);
}

c_reverse_iterator c_vector_rend(c_pvector thiz)
{
	return _A_get_reverse_iterator(((_c_vector_impl*)thiz->_l)->_start - 1);
}

size_type c_vector_size(c_pvector thiz)
{
	c_iterator b = c_vector_begin(thiz), e = c_vector_end(thiz);
	return abs(ITER_DIFF(e, b));
}

size_type c_vector_max_size(c_pvector thiz)
{
	return (size_type)(-1) / sizeof(node_t);
}

size_type c_vector_capacity(c_pvector thiz)
{
	_c_vector_impl * pl = thiz->_l;
	return abs(pl->_end_of_storage - pl->_start);
}

c_bool c_vector_empty(c_pvector thiz)
{
	_c_vector_impl * pl = thiz->_l;
	return (pl->_start == pl->_finish);
}

value_type c_vector_at(c_pvector thiz, size_type n)
{
	_c_vector_impl * pl = thiz->_l;
	return *(pl->_start + n);
}

c_pvector c_vector_assign(c_pvector thiz, const c_pvector V)
{
	if(V != thiz)
	{
		_c_vector_impl * pl = thiz->_l;
		const size_type vlen = c_vector_size(V);
		if(vlen > c_vector_capacity(thiz))
		{
			c_iterator tmp = _A_allocate_and_copy(thiz,
								vlen,
								c_vector_begin(V),
								c_vector_end(V));
			_A_deallocate(thiz,
					pl->_start,
					abs(pl->_end_of_storage - pl->_start));
			pl->_start = tmp._i;
			pl->_end_of_storage = pl->_start + vlen;
		}
		else if(c_vector_size(thiz) >= vlen)
		{
			c_copy(c_vector_begin(V), c_vector_end(V), c_vector_begin(thiz));
		}
		else
		{
			c_iterator bg = c_vector_begin(thiz);
			c_copy(bg, ITER_POSITIVE_N(bg, c_vector_size(thiz)), _A_get_iterator(pl->_start));
			c_uninitialized_copy(ITER_POSITIVE_N(bg, c_vector_size(thiz)),
						c_vector_end(thiz),
						_A_get_iterator(pl->_finish));
		}
		pl->_finish = pl->_start + vlen;
	}
	return thiz;
}

void c_vector_reserve(c_pvector thiz, size_t n)
{
	if(c_vector_capacity(thiz) < n)
	{
		_c_vector_impl * pl = thiz->_l;
		const size_type old_size = c_vector_size(thiz);
		c_iterator tmp = _A_allocate_and_copy(thiz, n, c_vector_begin(thiz), c_vector_end(thiz));
		_A_deallocate(thiz, pl->_start, abs(pl->_end_of_storage - pl->_start));
		pl->_start = tmp._i;
		pl->_finish = pl->_start + old_size;
		pl->_end_of_storage = pl->_start + n;
	}
}

value_type c_vector_front(c_pvector thiz)
{
	_c_vector_impl * pl = thiz->_l;
	return *pl->_start;
}

value_type c_vector_back(c_pvector thiz)
{
	_c_vector_impl * pl = thiz->_l;
	return *(pl->_finish - 1);
}

void c_vector_push_back(c_pvector thiz, const value_type val)
{
	_c_vector_impl * pl = thiz->_l;
	if(pl->_finish != pl->_end_of_storage)
	{
		*pl->_finish = val;
		++ pl->_finish;
	}
	else
		_A_insert_aux1(thiz, c_vector_end(thiz), val);
}

void c_vector_pop_back(c_pvector thiz)
{
	_c_vector_impl * pl = thiz->_l;
	-- pl->_finish;
}

void c_vector_swap(c_pvector thiz, c_pvector V)
{
	c_vector tmp;
	C_SWAP(*thiz, *V, tmp);
}

c_iterator c_vector_insert(c_pvector thiz, c_iterator pos, const value_type val)
{
	c_iterator begin = c_vector_begin(thiz);
	c_iterator end = c_vector_end(thiz);
	size_type n = abs(ITER_DIFF(pos, begin));
	_c_vector_impl * pl = thiz->_l;
	if((pl->_finish != pl->_end_of_storage) &&
		ITER_EQUAL(pos, end))
	{
		*pl->_finish = val;
		++ pl->_finish;
	}
	else
		_A_insert_aux1(thiz, pos, val);
	return ITER_POSITIVE_N(begin, n);
}

void c_vector_insert2(c_pvector thiz, c_iterator pos, c_iterator first, c_iterator last)
{
	if(!ITER_EQUAL(first, last))
	{
		_c_vector_impl * pl = thiz->_l;
		difference_type dn = 0;
		size_type n = 0;
		c_distance1(first, last, &dn);
		n = abs(dn);
		if(abs(pl->_end_of_storage - pl->_finish) >= n)
		{
			const size_type elems_after = pl->_finish - (pnode_t)pos._i;
			c_iterator old_finish = c_vector_end(thiz);
			if(elems_after > n)
			{
				c_uninitialized_copy(_A_get_iterator(pl->_finish - n),
							_A_get_iterator(pl->_finish),
							_A_get_iterator(pl->_finish));
				pl->_finish += n;
				c_copy_backward(pos, ITER_NEGATIVE_N(old_finish, n), old_finish);
				c_copy(first, last, pos);
			}
			else
			{
				c_uninitialized_copy(ITER_POSITIVE_N(first, elems_after),
							last,
							_A_get_iterator(pl->_finish));
				pl->_finish += n - elems_after;
				c_uninitialized_copy(pos, old_finish, _A_get_iterator(pl->_finish));
				pl->_finish += elems_after;
				c_copy(first, ITER_POSITIVE_N(first, elems_after), pos);
			}
		}
		else
		{
			const size_type old_size = c_vector_size(thiz);
			const size_type len = old_size + C_MAX(old_size, n);
			c_iterator new_start = _A_get_iterator(_A_allocate(thiz, len));
			c_iterator new_finish = new_start;
			new_finish = c_uninitialized_copy(_A_get_iterator(pl->_start), pos, new_start);
			new_finish = c_uninitialized_copy(first, last, new_finish);
			new_finish = c_uninitialized_copy(pos, _A_get_iterator(pl->_finish), new_finish);
			_A_deallocate(thiz, pl->_start, abs(pl->_end_of_storage - pl->_start));
			pl->_start = new_start._i;
			pl->_finish = new_finish._i;
			pl->_end_of_storage = ITER_POSITIVE_N(new_start, len)._i;
		}
	}
}

void c_vector_fill_insert(c_pvector thiz, c_iterator pos, size_type n, const value_type val)
{
	_c_vector_impl * pl = thiz->_l;
	if(n != 0)
	{
		if(abs(pl->_end_of_storage - pl->_finish) >= n)
		{
			value_type val_copy = val;
			const size_type elems_after = pl->_finish - (pnode_t)pos._i;
			c_iterator old_finish = c_vector_end(thiz);
			if(elems_after > n)
			{
				c_uninitialized_copy(_A_get_iterator(pl->_finish - n),
							_A_get_iterator(pl->_finish),
							_A_get_iterator(pl->_finish));
				pl->_finish += n;
				c_copy_backward(pos,
						ITER_NEGATIVE_N(old_finish, n),
						old_finish);
				c_fill(pos, ITER_POSITIVE_N(pos, n), val_copy);
			}
			else
			{
				c_uninitialized_fill_n(c_vector_end(thiz), n - elems_after, val_copy);
				pl->_finish += n - elems_after;
				c_uninitialized_copy(pos, old_finish, c_vector_end(thiz));
				pl->_finish += elems_after;
				c_fill(pos, old_finish, val_copy);
			}
		}
		else
		{
			const size_type old_size = c_vector_size(thiz);
			const size_type len = old_size + C_MAX(old_size, n);
			c_iterator new_start = _A_get_iterator(_A_allocate(thiz, len));
			c_iterator new_finish = new_start;
			new_finish = c_uninitialized_copy(_A_get_iterator(pl->_start),
								pos,
								new_start);
			new_finish = c_uninitialized_fill_n(new_finish, n, val);
			new_finish = c_uninitialized_copy(pos, _A_get_iterator(pl->_finish), new_finish);
			_A_deallocate(thiz, pl->_start, abs(pl->_end_of_storage - pl->_start));
			pl->_start = new_start._i;
			pl->_finish = new_finish._i;
			pl->_end_of_storage = pl->_start + len;
		}
	}
}

c_iterator c_vector_erase(c_pvector thiz, c_iterator pos)
{
	c_iterator pos_1 = ITER_POSITIVE_N(pos, 1);
	c_iterator end = c_vector_end(thiz);
	_c_vector_impl * pl = thiz->_l;
	if(!ITER_EQUAL(pos_1, end))
		c_copy(pos_1, _A_get_iterator(pl->_finish), pos);
	-- pl->_finish;
	return pos;
}

c_iterator c_vector_erase2(c_pvector thiz, c_iterator first, c_iterator last)
{
	_c_vector_impl * pl = thiz->_l;
	c_copy(last, _A_get_iterator(pl->_finish), first);
	pl->_finish = pl->_finish - abs(ITER_DIFF(last, first));
	return first;
}

void c_vector_clear(c_pvector thiz)
{
	c_vector_erase2(thiz, c_vector_begin(thiz), c_vector_end(thiz));
}

void c_vector_resize(c_pvector thiz, size_t n)
{
	c_iterator begin = c_vector_begin(thiz);
	if(n < c_vector_size(thiz))
		c_vector_erase2(thiz, ITER_POSITIVE_N(begin, n), c_vector_end(thiz));
	else
		c_vector_fill_insert(thiz, c_vector_end(thiz), n, NULL);
}

c_bool c_vector_equal(c_pvector thiz, const c_pvector V)
{
	c_binary_predicate bp =  c_binary_negate(thiz->_cmp);
	return (c_vector_size(thiz) == c_vector_size(V)) &&
			c_equal2(c_vector_begin(thiz),
					c_vector_end(thiz),
					c_vector_begin(V),
					bp);
}

c_bool c_vector_less(c_pvector thiz, const c_pvector V)
{
	return c_lexicographical_compare(c_vector_begin(thiz),
						c_vector_end(thiz),
						c_vector_begin(V),
						c_vector_end(V),
						thiz->_cmp);
}
