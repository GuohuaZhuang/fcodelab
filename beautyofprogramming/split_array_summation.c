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
* @file split_array_summation.c
* @brief split the array summation aim to balance two half child array.
* @author firstboy0513
* @version 0.0.1
* @date 2013-12-09
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/**
* @brief calculate summation with given the bag 0 and 1.
*
* @param array 2*n array.
* @param length array length.
* @param bag bag 0 and 1.
*
* @return the summation.
*/
int get_summation(const int* array, const int length, const int* bag) {
	int i = 0, sum = 0;
	for (i = 0; i < length; i ++) {
		if (1 == bag[i]) {
			sum += array[i];
		}
	}
	return sum;
}

/**
* @brief use recursive to solve split array summation problem.
*
* @param array 2*n array.
* @param length array length.
*
* @return the best balance summation bag 0 and 1 as a split.
*/
int* split_array_summation(const int* array, const int length) {
	if (!array || length <= 0 || length % 2 != 0) {
		printf("split_array_summation input invalid!\n");
		return NULL;
	}
	int* split = (int*) malloc(sizeof(int) * length);
	int* bag = (int*) malloc(sizeof(int) * length);
	int i = 0, half_sum = 0, half_length = length / 2;
	for (i = 0; i < length; i ++) {
		bag[i] = split[i] = 0;
		half_sum += array[i];
	}
	half_sum /= 2;
	int max_sum = INT32_MIN, half_count = 0;
	while (1) {
		if (half_count == half_length) {
			int sum = get_summation(array, length, bag);
			if (sum <= half_sum && sum > max_sum) {
				max_sum = sum;
				memcpy(split, bag, sizeof(int) * length);
			}
		}
		int k = length - 1;
		while (k >= 0) {
			if (0 == bag[k]) { bag[k] = 1; half_count ++; break; }
			bag[k] = 0; half_count --; k --;
		}
		if (k < 0) break;
	}

	free(bag);
	return split;
}

/**
* @brief print out the split to half array result.
*
* @param array 2*n array.
* @param length array length.
* @param split split result with bag 0 and 1.
*/
void print_result(const int* array, const int length, const int* split) {
	int i = 0, j = 0;
	printf("original array is:\n\t");
	for (i = 0; i < length; i ++) {
		if (0 != i) printf(", ");
		printf("%d", array[i]);
	}
	printf("\nsplited array are:\n\t");
	for (j = 0, i = 0; i < length; i ++) {
		if (0 == split[i]) continue;
		if (0 != j ++) printf(", ");
		printf("%d", array[i]);
	}
	printf("\n\t");
	for (j = 0, i = 0; i < length; i ++) {
		if (1 == split[i]) continue;
		if (0 != j ++) printf(", ");
		printf("%d", array[i]);
	}
	printf("\n");
}

int main(int argc, const char *argv[])
{
	int array[] = {1, 5, 7, 8, 9, 6, 3, 11, 20, 17};
	int length = sizeof(array) / sizeof(array[0]);
	int* split = split_array_summation(array, length);
	if (split) {
		print_result(array, length, split);
		free(split);
	}
	return 0;
}
