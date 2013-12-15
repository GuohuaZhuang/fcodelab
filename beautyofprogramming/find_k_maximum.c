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
* @file find_k_maximum.c
* @brief find k maximum number from a given array.
* @author firstboy0513
* @version 0.0.1
* @date 2013-12-15
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#define NUMBER				int			// array element type
#define NF					"%010d"     // array element print format
#define TEST_ARRAY_LENGTH	20          // test array length
#define TEST_K				3			// k maximum

/**
* @brief prepare test array: initialize test array.
*
* @param length array length.
*
* @return array point.
*/
NUMBER* init_test_array(NUMBER length) {
	NUMBER* array = (NUMBER*) malloc(sizeof(NUMBER) * length);
	srand(time(NULL));
	NUMBER i = 0;
	for (i = 0; i < length; i ++) {
		array[i] = (NUMBER)rand();
	}
	return array;
}

/**
* @brief release the heap memory of test array.
*
* @param array array point.
*/
void release_test_array(NUMBER* array) {
	free(array);
}

/**
* @brief print array all element.
*
* @param array array point.
* @param length array length.
*/
void print_array(NUMBER* array, NUMBER length, const char* split) {
	NUMBER i = 0;
	for (i = 0; i < length; i ++) {
		if (0 != i) printf("%s", split);
		printf(NF, array[i]);
	}
	printf("\n");
}

#define SWAP(a, b) { (a) ^= (b); (b) ^= (a); (a) ^= (b); }

/**
* @brief use for quicksort to find the index of pivot.
*
* @param array array point.
* @param left left index, or call it lower index.
* @param right right index, or call it high index.
*
* @return pivot index.
*/
NUMBER quicksort_partition(NUMBER* array, NUMBER left, NUMBER right) {
	NUMBER pivot = array[left];
	while (left < right) {
		while (left < right && array[right] >= pivot) { right --; }
		array[left] = array[right];
		while (left < right && array[left] <= pivot) { left ++; }
		array[right] = array[left];
	}
	array[left] = pivot;
	return left;
}

/**
* @brief quick sort use regular implement.
*
* @param array array point.
* @param left left index.
* @param right right index.
*/
void quicksort(NUMBER* array, NUMBER left, NUMBER right) {
	if (right <= left) { return; }
	NUMBER pivot_index = quicksort_partition(array, left, right);
	if (left < pivot_index - 1) { quicksort(array, left, pivot_index - 1); }
	if (right > pivot_index + 1) { quicksort(array, pivot_index + 1, right); }
}

/**
* @brief output maximum k number.
*
* @param array array point.
* @param length array length.
* @param k maximum count.
*/
void output_k_maximum(NUMBER* array, NUMBER length, NUMBER k) {
	if (k > length) { k = length; }
	while (k --) {
		printf(NF"\n", array[length - k - 1]);
	}
}

int main(int argc, const char *argv[])
{
	NUMBER* array = init_test_array(TEST_ARRAY_LENGTH);
	printf("array before sort:\n");
	print_array(array, TEST_ARRAY_LENGTH, ", \n");
	printf("==============\n\n");
	quicksort(array, 0, TEST_ARRAY_LENGTH-1);
	printf("array after sort:\n");
	print_array(array, TEST_ARRAY_LENGTH, ", \n");
	printf("==============\n\n");
	printf("output_k_maximum %d:\n", TEST_K);
	output_k_maximum(array, TEST_ARRAY_LENGTH, TEST_K);
	printf("==============\n\n");
	release_test_array(array);

	return 0;
}
