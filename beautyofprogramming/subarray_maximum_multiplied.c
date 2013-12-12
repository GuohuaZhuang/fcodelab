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
* @file subarray_maximum_multiplied.c
* @brief get array maximum multiplied excluded index.
* @author firstboy0513
* @version 0.0.1
* @date 2013-12-12
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/**
* @brief get array's maximum multiplied sub array index.
* Given a length N array, just using multiply, without division, find out the 
* maximum multiplied in any N-1 element in the array.
*
* @param array array.
* @param length array length.
*
* @return the one element is excluded in multiplied in the N-1 element index.
* if return -1 means some error occur.
*/
int subarray_maximum_multiplied(const int* array, const int length) {
	if (!array || length <= 0) {
		printf("[ERR] -- subarray_maximum_multiplied input invalid!\n");
		return -1;
	}
	int i = 0;
	int* left_multiplied = (int*) malloc(sizeof(int) * length);
	int* right_multiplied = (int*) malloc(sizeof(int) * length);
	for (i = 0; i < length; i ++) {
		left_multiplied[i] = right_multiplied[length-i-1] = 1;
		if (i > 0) {
			left_multiplied[i] *= array[i-1];
			right_multiplied[length-i-1] *= array[length-i];
		}
	}
	int current_multiplied = 1;
	int maximum_multiplied_index = -1, maximum_multiplied = INT32_MIN;
	for (i = 0; i < length; i ++) {
		current_multiplied = left_multiplied[i] * right_multiplied[i];
		if (current_multiplied > maximum_multiplied) {
			maximum_multiplied = current_multiplied;
			maximum_multiplied_index = i;
		}
	}
	free(left_multiplied); free(right_multiplied);
	return maximum_multiplied_index;
}

/**
* @brief print out the multiplied result.
*
* @param array array.
* @param length array length.
* @param excluded_index excluded index.
*/
void print_multiplied(const int* array, const int length, 
	const int excluded_index) {
	int i = 0, j = 0, multiplied = 1;
	printf("the array is: \n\tarray = {");
	for (i = 0; i < length; i ++) {
		if (0 != i) printf(", ");
		printf("%d", array[i]);
	}
	printf("}\n\texcluded_index = %d\n", excluded_index);
	printf("the maximum multiplied is: \n\t");
	for (i = 0; i < length; i ++) {
		if (i == excluded_index) continue;
		if (0 != j++) printf(" * ");
		printf("%d", array[i]);
		multiplied *= array[i];
	}
	printf(" = %d\n", multiplied);
}

int main(int argc, const char *argv[])
{
	const int array[] = {5, 2, 9, -2, 8, -4, 1, -4};
	const int length = sizeof(array) / sizeof(array[0]);
	int excluded_index = subarray_maximum_multiplied(array, length);
	if (excluded_index > 0) print_multiplied(array, length, excluded_index);
	return 0;
}
