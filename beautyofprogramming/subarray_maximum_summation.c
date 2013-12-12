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
* Add partition method to solve it, it names divide and conquer.
* @author firstboy0513
* @version 0.0.2
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

/**
* @brief get array maximum summation in its sub arrays.
* This function is use partition (as name divide and conquer) to solve it.
*
* @param array array.
* @param length array length.
*
* @return return maximum summation.
*/
int subarray_maximum_summation_partition(const int* array, const int length) {
	if (length == 1) { return array[0]; }
	if (length <= 0) { exit(-1); } // assert length can never less than 0.
	int i = 0, middle = length / 2;
	int max_sum = INT32_MIN, current_sum = 0, left_sum = 0, right_sum = 0;
	if (middle > 0) {
		left_sum = subarray_maximum_summation_partition(array, middle);
	}
	if (middle < length) {
		right_sum = subarray_maximum_summation_partition(array+middle, 
			length-middle);
	}
	int left_max = INT32_MIN, right_max = INT32_MIN;
	current_sum = 0;
	for (i = middle-1; i >= 0; i --) {
		current_sum += array[i];
		if (current_sum > left_max) { left_max = current_sum; }
	}
	current_sum = 0;
	for (i = middle; i < length; i ++) {
		current_sum += array[i];
		if (current_sum > right_max) { right_max = current_sum; }
	}
	max_sum = left_max + right_max;
	if (max_sum < left_sum) { max_sum = left_sum; }
	if (max_sum < right_sum) { max_sum = right_sum; }
	return max_sum;
}

int max(int a, int b) { return (a > b ? a : b); }

/**
* @brief get array maximum summation in its sub arrays.
* This function is use dynamic programming to solve it.
* Look array[0], we know there are 3 situations here. If the best maximum 
* summation is array[i] to array[j], then:
* 	0 = i = j, it means the maximum element summation is just from array[0];
* 	0 = i < j, it means the maximum summation is start from array[0].
* 	0 < i, it means the maximum summation is exclude array[0].
* so, if we start from the end of array, assume the start array is begin from 
* array element, like start[k] means the maximum summation from array[k].
* And assume the best array is the maximum summation from the 
* array[k]~array[n-1], which n is the length of array. So we can start like:
*   start[n-1] = array[n-1]
*   best[n-1] = array[n-1]
* then, start[n-2] = max(array[n-2], array[n-2]+start[n-1]);
* and best[n-2] = max(start[n-2], best[n-1]);
* and so on..., util to the 0th element. then we found the best[0] is the total
* maximum summation in the array we want.
*
* @param array array.
* @param length array length.
*
* @return return maximum summation, or return INT32_MIN means error occur.
*/
int subarray_maximum_summation_dynamicprogramming(
	const int* array, const int length) {
	if (!array || length <= 0) {
		printf("[ERR] -- subarray_maximum_summation input invalid!\n");
		return INT32_MIN;
	}
	int i = 0, max_sum = INT32_MIN;
	int* start = (int*) malloc(sizeof(int) * length);
	int* best = (int*) malloc(sizeof(int) * length);
	best[length - 1] = start[length - 1] = array[length - 1];
	for (i = length - 2; i >= 0; i --) {
		start[i] = max(array[i], array[i]+start[i+1]);
		best[i] = max(start[i], best[i+1]);
	}
	max_sum = best[0];
	free(start); free(best);
	return max_sum;
}

/**
* @brief get array maximum summation in its sub arrays.
* Use dynamic programming as the same of the above method, it optimize the 
* memory to hot use any heap memory.
*
* @param array array.
* @param length array length.
*
* @return return maximum summation, or return INT32_MIN means error occur.
*/
int subarray_maximum_summation_dynamicprogramming_optimizememory(
	const int* array, const int length) {
	if (!array || length <= 0) {
		printf("[ERR] -- subarray_maximum_summation input invalid!\n");
		return INT32_MIN;
	}
	int i = 0, max_sum = array[length - 1], max_start = array[length - 1];
	for (i = length - 2; i >= 0; i --) {
		max_start = max(array[i], array[i]+max_start);
		max_sum = max(max_start, max_sum);
	}
	return max_sum;
}

int main(int argc, const char *argv[])
{
	const int array[] = 
		{-2, 5, 3, -6, 4, -8, 6}; // result is 8
		// {1, -2, 3, 5, -3, 2}; // result is 8
		// {0, -2, 3, 5, -1, 2}; // result is 9
		// {-9, -2, -3, -5, -3}; // result is -2
	const int length = sizeof(array) / sizeof(array[0]);
	// int max_summation = subarray_maximum_summation(array, length);
	// int max_summation = subarray_maximum_summation_partition(array, length);
	// int max_summation = subarray_maximum_summation_dynamicprogramming(
	// 	array, length);
	int max_summation = 
		subarray_maximum_summation_dynamicprogramming_optimizememory(
			array, length);
	printf("max_summation = %d\n", max_summation);
	return 0;
}
