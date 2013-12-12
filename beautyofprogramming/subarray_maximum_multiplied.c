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
* @brief analysis each situations to find the maximum multiplied excluded index.
*
* @param array array.
* @param length array length.
* @param zero_count zero count in the array.
* @param positive_count positive count in the array.
* @param negative_count negative count in the array.
* @param positive_min_index positive minimum index in the array.
* @param negative_min_index negative minimum index in the array.
* @param positive_max_index positive maximum index in the array.
* @param negative_max_index negative maximum index in the array.
* @param zero_index any zero (actually the last) index in the array.
* @param multiplied multiplied the whole element in the array.
* @param multiplied_without_zero multiplied element without zero in the array.
*
* @return the one element is excluded in multiplied in the N-1 element index.
* if return -1 means some error occur.
*/
int maximum_multiplied_analysis(const int* array, const int length, 
	int zero_count, int positive_count, int negative_count, 
	int positive_min_index, int negative_min_index, 
	int positive_max_index, int negative_max_index, 
	int zero_index, int multiplied, int multiplied_without_zero) {
	if (multiplied == 0) {
		if (zero_count > 1) { return zero_index; } // mul(N-1) = 0, any index
		else {
			if (multiplied_without_zero > 0) { // mul(N-1) > 0
				return zero_index;
			} else {
				return negative_min_index; // mul(N-1) < 0, any negative index
			}
		}
	} else if (multiplied > 0) {
		if (positive_min_index >= 0) { // have positive, return pos min index.
			return positive_min_index;
		}
		return negative_min_index; // no positive, return neg max abs(max)index.
	} else {
		return negative_max_index; // must have negative, return neg abs(min).
	}
}

/**
* @brief get array's maximum multiplied sub array index.
* Given a length N array, just using multiply, without division, find out the 
* maximum multiplied in any N-1 element in the array.
* This method is analysis complex, but time is ok.
* This method is just use in this excluded one index find problem.
* This method is no heap memory alloc.
*
* @param array array.
* @param length array length.
*
* @return the one element is excluded in multiplied in the N-1 element index.
* if return -1 means some error occur.
*/
int subarray_maximum_multiplied_analysis(const int* array, const int length) {
	if (!array || length <= 0) {
		printf("[ERR] -- subarray_maximum_multiplied input invalid!\n");
		return -1;
	}
	int i = 0, zero_count = 0, positive_count = 0, negative_count = 0;
	int positive_min_index = -1, negative_min_index = -1;
	int positive_max_index = -1, negative_max_index = -1;
	int zero_index = -1, multiplied = 1, multiplied_without_zero = 1;
	for (i = 0; i < length; i ++) {
		multiplied *= array[i];
		// count add up and extremum find out
		if (array[i] == 0) {
			zero_count ++, zero_index = i;
		} else if (array[i] > 0) {
			multiplied_without_zero *= array[i];
			positive_count ++;
			if (positive_min_index==-1 || array[i]<array[positive_min_index]) {
				positive_min_index = i;
			}
			if (positive_max_index==-1 || array[i]>array[positive_max_index]) {
				positive_max_index = i;
			}
		} else {
			multiplied_without_zero *= array[i];
			negative_count ++;
			if (negative_min_index==-1 || array[i]<array[negative_min_index]) {
				negative_min_index = i;
			}
			if (negative_max_index==-1 || array[i]>array[negative_max_index]) {
				negative_max_index = i;
			}
		}
	}
	return maximum_multiplied_analysis(array, length, zero_count, 
		positive_count, negative_count, positive_min_index, negative_min_index, 
		positive_max_index, negative_max_index, zero_index, multiplied, 
		multiplied_without_zero);
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
	// int excluded_index = subarray_maximum_multiplied(array, length);
	int excluded_index = subarray_maximum_multiplied_analysis(array, length);
	if (excluded_index > 0) print_multiplied(array, length, excluded_index);
	return 0;
}
