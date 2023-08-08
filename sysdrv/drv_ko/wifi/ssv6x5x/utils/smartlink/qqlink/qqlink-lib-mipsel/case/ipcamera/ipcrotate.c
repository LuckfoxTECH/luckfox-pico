/*
 * Copyright (c) 2015 iComm-semi Ltd.
 *
 * This program is free software: you can redistribute it and/or modify 
 * it under the terms of the GNU General Public License as published by 
 * the Free Software Foundation, either version 3 of the License, or 
 * (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
 * See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <string.h>
int cb_on_set_definition(int definition, char *cur_definition, int cur_definition_length)
{
    printf("==============cb_on_set_definition, definition:%d\n", definition);
    return 0;
}
int cb_on_control_rotate(int rotate_direction, int rotate_degree)
{
    printf("===============cb_on_control_rotate, rotate_direction:%d, rotate_degree:%d\n", rotate_direction, rotate_degree);
    return 0;
}
