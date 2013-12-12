/* Copyright (C) 
* 2013 - firstboy0513
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
* @file subarray_maximum_summation.c
* @brief get array maximum summation in its sub arrays.
* @author firstboy0513
* @version 0.0.1
* @date 2013-12-12
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/**
* @brief get array maximum summation in its sub arrays.
*
* @param array array.
* @param length array length.
*
* @return return maximum summation.
*/
int subarray_maximum_summation(const int* array, const int length) {
	int i = 0, j = 0;
	int max_sum = INT32_MIN, current_sum = 0;
	for (i = 0; i < length; i ++) {
		current_sum = 0;
		for (j = i; j < length; j ++) {
			current_sum += array[j];
			if (current_sum > max_sum) { max_sum = current_sum; }
		}
	}
	return max_sum;
}

int main(int argc, const char *argv[])
{
	const int array[] = 
		{-2, 5, 3, -6, 4, -8, 6};
		// {1, -2, 3, 5, -3, 2};
		// {0, -2, 3, 5, -1, 2};
		// {-9, -2, -3, -5, -3};
	const int length = sizeof(array) / sizeof(array[0]);
	int max_summation = subarray_maximum_summation(array, length);
	printf("max_summation = %d\n", max_summation);
	return 0;
}
