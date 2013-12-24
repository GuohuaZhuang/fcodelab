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
#define NUM_MAX				INT32_MAX	// max number
#define NUM_MIN				INT32_MIN	// min number

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
* @brief quick sort use regular implement optimize with use middle pivot.
*
* @param array array point.
* @param left left index.
* @param right right index.
*/
void quicksort_use_middle_pivot(NUMBER* array, NUMBER left, NUMBER right) {
	NUMBER pivot = array[(left+right)/2], i = left, j = right;
	while (i <= j) {
		while (array[i] < pivot) { i ++; }
		while (array[j] > pivot) { j --; }
		if (i <= j) {
			if (i != j) { SWAP(array[i], array[j]) }
			i ++, j --;
		}
	}
	if (left < j) { quicksort_use_middle_pivot(array, left, j); }
	if (right > i) { quicksort_use_middle_pivot(array, i, right); }
}

/**
* @brief use for heapsort to adjust a heap sort ordered from start to end.
* Note the start minimum is from 1, not 0, but the array minimum index is 0, 
* so there are so many +(-1) when access the array element.
* Note this heap adjust is a maximum heap, if you want use minimum heap, use
* minheapadjust instead.
*
* @param array array point.
* @param start start index.
* @param end end index.
*/
void heapadjust(NUMBER* array, NUMBER start, NUMBER end) {
	NUMBER j = 0, preroot = array[start+(-1)];
	for (j = start * 2; j <= end; j *= 2) {
		if (j < end && array[j+(-1)] < array[j+1+(-1)]) { j ++; }
		if (preroot >= array[j+(-1)]) break;
		array[start+(-1)] = array[j+(-1)]; start = j;
	}
	array[start+(-1)] = preroot;
}

/**
* @brief use for heapsort to adjust a heap sort ordered from start to end.
* Note the start minimum is from 1, not 0, but the array minimum index is 0, 
* so there are so many +(-1) when access the array element.
* Note this heap adjust is a minimum heap, if you want use maximum heap, use
* heapadjust instead.
*
* @param array array point.
* @param start start index.
* @param end end index.
*/
void minheapadjust(NUMBER* array, NUMBER start, NUMBER end) {
	NUMBER j = 0, preroot = array[start+(-1)];
	for (j = start * 2; j <= end; j *= 2) {
		if (j < end && array[j+(-1)] > array[j+1+(-1)]) { j ++; }
		if (preroot <= array[j+(-1)]) break;
		array[start+(-1)] = array[j+(-1)]; start = j;
	}
	array[start+(-1)] = preroot;
}

/**
* @brief heap sort algorithm to sort array.
* It use array to store heap tree.
*
* @param array array point.
* @param length array length.
*/
void heapsort(NUMBER* array, NUMBER length) {
	// bulld heap tree
	NUMBER i = 0;
	for (i = length/2; i > 0; i --) {
		heapadjust(array, i, length);
	}
	// sort array and rebuild remain element
	for (i = length; i > 1; i --) {
		SWAP(array[1+(-1)], array[i+(-1)])
		heapadjust(array, 1, i-1);
	}
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

/**
* @brief a test case for general sort all element and then get k maximum.
*/
void testcase_k_maximum_after_sortall() {
	NUMBER* array = init_test_array(TEST_ARRAY_LENGTH);
	printf("array before sort:\n");
	print_array(array, TEST_ARRAY_LENGTH, ", \n");
	printf("==============\n\n");

	// quicksort(array, 0, TEST_ARRAY_LENGTH-1);
	// quicksort_use_middle_pivot(array, 0, TEST_ARRAY_LENGTH-1);
	heapsort(array, TEST_ARRAY_LENGTH);
	printf("array after sort:\n");
	print_array(array, TEST_ARRAY_LENGTH, ", \n");
	printf("==============\n\n");

	printf("output_k_maximum %d:\n", TEST_K);
	output_k_maximum(array, TEST_ARRAY_LENGTH, TEST_K);
	printf("==============\n\n");
	release_test_array(array);
}

/**
* @brief quick part sort to find k maximum.
*
* @param array array.
* @param left quick sort left index.
* @param right quick sort right index.
* @param k k maximum count.
*/
void quickpartsort(NUMBER* array, NUMBER left, NUMBER right, NUMBER k) {
	if (k <= 0 || left >= right) { return; }
	NUMBER i = left, j = right, pivot = array[ (left+right)/2 ];
	while (i <= j) {
		while (array[j] > pivot) { j --; }
		while (array[i] < pivot) { i ++; }
		if (i <= j) {
			if (i != j) { SWAP(array[i], array[j]) }
			i ++, j --;
		}
	}
	if (k >= right-i+1) {
		if (left < j) { quickpartsort(array, left, j, k-(right-i+1)); }
	} else {
		if (right > i) { quickpartsort(array, i, right, k); }
	}
}

/**
* @brief quick part sort to find k maximum.
*
* @param array array.
* @param length array length.
* @param k k maximum count.
*/
void quickpartsort_k_maximum(NUMBER* array, NUMBER length, NUMBER k) {
	quickpartsort(array, 0, length-1, k);
}

/**
* @brief a test case for use part sort to find k maximum.
*/
void testcase_k_maximum_quickpartsort() {
	NUMBER* array = init_test_array(TEST_ARRAY_LENGTH);
	printf("array before sort:\n");
	print_array(array, TEST_ARRAY_LENGTH, ", \n");
	printf("==============\n\n");

	quickpartsort_k_maximum(array, TEST_ARRAY_LENGTH, TEST_K);
	printf("array after sort:\n");
	print_array(array, TEST_ARRAY_LENGTH, ", \n");
	printf("==============\n\n");

	printf("output_k_maximum %d:\n", TEST_K);
	output_k_maximum(array, TEST_ARRAY_LENGTH, TEST_K);
	printf("==============\n\n");
	release_test_array(array);
}

/**
* @brief count the elements greater than given element.
*
* @param array array.
* @param length array length.
* @param x given number element.
*
* @return the count of elements greater than given element.
*/
NUMBER count_greaterthan_givenelement(NUMBER* array, NUMBER length, NUMBER x) {
	NUMBER count = 0;
	while (length --) {
		if (array[length] >= x) { count ++; }
	}
	return count;
}

/**
* @brief use binary search to find k maximum.
*
* @param array array.
* @param length array size.
* @param k k maximum count.
*
* @return the minimum number in k maximum numbers.
*/
NUMBER binarysearch_k_maximum(NUMBER* array, NUMBER length, NUMBER k) {
	NUMBER vmax = NUM_MIN, vmin = NUM_MAX, vmid = 0;
	NUMBER i = 0, count = 0;
	for (i = 0; i < length; i ++) {
		if (array[i] > vmax) { vmax = array[i]; }
		if (array[i] < vmin) { vmin = array[i]; }
	}
	while (vmax > vmin) {
		vmid = vmin + (vmax - vmin) / 2; // divided by 2
		count = count_greaterthan_givenelement(array, length, vmid);
		if (count > k) {
			vmin = vmid;
		} else if (count < k) {
			vmax = vmid;
		} else {
			vmin = vmid; break;
		}
	}
	return vmin;
}

/**
* @brief output k maximum which less than a given number x.
*
* @param array array.
* @param length array size.
* @param x a given number.
*/
void output_k_maximum_lessthanx(NUMBER* array, NUMBER length, NUMBER x) {
	while (length --) {
		if (array[length] >= x) {
			printf(NF"\n", array[length]);
		}
	}
}

/**
* @brief test case for use binary search to find k maximum.
*/
void testcase_k_maximum_binarysearch() {
	NUMBER* array = init_test_array(TEST_ARRAY_LENGTH);
	printf("array before sort:\n");
	print_array(array, TEST_ARRAY_LENGTH, ", \n");
	printf("==============\n\n");

	NUMBER x = binarysearch_k_maximum(array, TEST_ARRAY_LENGTH, TEST_K);

	printf("output_k_maximum %d:\n", TEST_K);
	output_k_maximum_lessthanx(array, TEST_ARRAY_LENGTH, x);
	printf("==============\n\n");
	release_test_array(array);
}

/**
* @brief output k maximum in a heap array.
*
* @param heap heap array.
* @param size heap array size.
*/
void output_k_maximum_heap(NUMBER* heap, NUMBER size) {
	if (!heap || size <= 0) { return; }
	while (size --) {
		printf(NF"\n", heap[size]);
	}
}

/**
* @brief use heap sort to find k maximum.
*
* @param array array.
* @param length array length.
* @param k k maximum count.
*
* @return heap array. Note you should free it memory outside.
*/
NUMBER* heapsort_k_maximum(NUMBER* array, NUMBER length, NUMBER k) {
	if (!array || length <= 0 || k <= 0) {
		printf("[ERR] -- heapsort_k_maximum input invalid!\n");
		return NULL;
	}
	NUMBER* heap = (NUMBER*) malloc(sizeof(NUMBER) * k);
	NUMBER heap_size = 0, i = 0;
	for (i = 0; i < length; i ++) {
		if (heap_size < k) {
			heap[heap_size] = heap[1+(-1)];
			heap[1+(-1)] = array[i];
			heap_size ++;
			if (k == heap_size) { // heap is full to adjust it
				NUMBER j = 0;
				for (j = k/2; j > 0; j --) { minheapadjust(heap, j, k); }
			}
			continue;
		}
		if (heap[0] < array[i]) {
			heap[0] = array[i];
			minheapadjust(heap, 1, k);
		}
	}
	return heap;
}

/**
* @brief test case for use heap sort to find k maximum.
*/
void testcase_k_maximum_heapsort() {
	NUMBER* array = init_test_array(TEST_ARRAY_LENGTH);
	printf("array before sort:\n");
	print_array(array, TEST_ARRAY_LENGTH, ", \n");
	printf("==============\n\n");

	NUMBER* heap = heapsort_k_maximum(array, TEST_ARRAY_LENGTH, TEST_K);

	printf("output_k_maximum %d:\n", TEST_K);
	output_k_maximum_heap(heap, TEST_K);
	printf("==============\n\n");
	free(heap);
	release_test_array(array);
}

int main(int argc, const char *argv[])
{
//	testcase_k_maximum_after_sortall();
//	testcase_k_maximum_quickpartsort();
//	testcase_k_maximum_binarysearch();
	testcase_k_maximum_heapsort();
	return 0;
}
