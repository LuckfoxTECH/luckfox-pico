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

#ifndef _C_PAIR_H
#define _C_PAIR_H

#include "c_iterator.h"
#include "c_memory.h"

#define c_pair      		_c_pair
#define c_ppair     		_c_ppair
#define c_iter_bool_pair	_c_iter_bool_pair
#define c_piter_bool_pair	_c_piter_bool_pair
#define c_iter_iter_pair	_c_iter_iter_pair
#define c_piter_iter_pair	_c_piter_iter_pair
#define c_make_pair 		_c_make_pair
#define c_make_iter_bool_pair	_c_make_iter_bool_pair
#define c_make_iter_iter_pair	_c_make_iter_iter_pair

typedef value_type first_type;
typedef value_type second_type;

typedef struct c_pair c_pair, * c_ppair;
typedef int (*PAIR_COMPARER)(c_ppair, c_ppair);

struct c_pair
{
	first_type first;
	second_type second;
};


typedef struct c_iter_bool_pair c_iter_bool_pair, c_piter_bool_pair;

struct c_iter_bool_pair
{
	c_iterator first;
	c_bool second;
};


typedef struct c_iter_iter_pair c_iter_iter_pair, c_piter_iter_pair;

struct c_iter_iter_pair
{
	c_iterator first;
	c_iterator second;
};


c_pair c_make_pair(const value_type x, const value_type y);
c_iter_bool_pair c_make_iter_bool_pair(c_iterator x, c_bool y);
c_iter_iter_pair c_make_iter_iter_pair(c_iterator x, c_iterator y);




#endif /* _C_PAIR_H */





