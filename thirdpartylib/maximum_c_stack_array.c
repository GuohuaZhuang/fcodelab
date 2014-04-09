/* Copyright (C) 
* 2014 - firstboy0513
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
* 
*/
/**
* @file maximum_c_stack_array.c
* @brief maximum c stack array in linux gcc.
* Use ulimit -a to view and ulimit -s STACK_SIZE to modify the stack size.
* @author firstboy0513
* @version 0.0.1
* @date 2014-04-09
*/
#include <stdio.h>
#include <stdlib.h>

#define MAX_ARRAY_SIZE 2048000

int main(int argc, const char *argv[])
{
    int array[MAX_ARRAY_SIZE] = {0};
    array[MAX_ARRAY_SIZE-1] = 23*24;
    printf("array[%d] = %d\n", MAX_ARRAY_SIZE-1, array[MAX_ARRAY_SIZE-1]);
    return 0;
}
