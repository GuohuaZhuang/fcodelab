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
* @file allrange.c
* @brief all range program use recursive method and next permutation method.
* @author firstboy0513
* @version 0.0.1
* @date 2013-12-27
*/
#include <stdio.h>
#include <stdlib.h>

/**
* @brief output range array.
*
* @param array array.
* @param size array size.
*/
void output_range_array(const int* array, const int size) {
	int i = 0;
	for (i = 0; i < size; i ++) {
		printf("%d", array[i]);
	}
}

#define SWAP(a,b) { (a) ^= (b); (b) ^= (a); (a) ^= (b); }

/**
* @brief judge the array[end] has the same element from array[start] to 
* array[end-1].
*
* @param array array.
* @param start start index.
* @param end end index.
*
* @return return 1 means has repeat element, otherwise means no repeat.
*/
int has_repeat(int* array, int start, int end) {
	int i = 0;
	for (i = start; i < end; i ++) {
		if (array[i] == array[end]) {
			return 1;
		}
	}
	return 0;
}

/**
* @brief print out all range numbers.
*
* @param array array of numbers.
* @param size array size.
* @param start start change index.
* @param isrepeat default is 1 can repeat, if input 0 means deduplication.
*/
void allrange_recursion(int* array, int size, int start, int repeat) {
	if (!array || size <= 0 || size <= start) {
		printf("[ERR] -- allrange input invalid!\n");
		return;
	}
	if (start == size-1) {
		output_range_array(array, size); printf("\n");
		return;
	}
	int i = 0;
	allrange_recursion(array, size, start+1, repeat);
	for (i = start+1; i < size; i ++) {
		if (repeat || (!repeat && !has_repeat(array, start, i))) {
			SWAP(array[start], array[i]);
			allrange_recursion(array, size, start+1, repeat);
			SWAP(array[start], array[i]);
		}
	}
}

/**
* @brief array reverse, be sure the element is integer number.
*
* @param array array.
* @param size array size.
*/
void array_reverse(int* array, int size) {
	int i = 0, j = size-1;
	while (i < j) {
		SWAP(array[i], array[j])
		i ++, j --;
	}
}

/**
* @brief judge whether has next permutation.
*
* @param array array.
* @param size array size.
*
* @return return 1 means has next permutation, otherwise return 0 means There
* are not any next permutation more.
*/
int has_next_permutation(int* array, int size) {
	if (!array || size <= 1) {
		return 0;
	}
	int changeindex = size-1, reverseleft = 0, reverseright = 0;
	do {
		reverseleft = changeindex; changeindex --;
		if (array[changeindex] < array[reverseleft]) {
			reverseright = size-1;
			while (array[changeindex] >= array[reverseright]) { reverseright--; }
			SWAP(array[changeindex], array[reverseright])
			array_reverse(array + reverseleft, size - reverseleft);
			return 1;
		}
	} while (changeindex > 0);
	array_reverse(array, size);
	return 0;
}

/**
* @brief array heap adjust as big heap as want to get ascending sort.
*
* @param array array.
* @param size array size.
* @param start array start adjust index.
*/
void array_heapadjust(int* array, int size, int start) {
	int j = 0, pivot = array[start+(-1)];
	for (j = (start<<1); j <= size; j <<= 1) {
		if (j < size && array[j+(-1)] < array[j+1+(-1)]) { j ++; }
		if (pivot > array[j+(-1)]) { break; }
		array[start+(-1)] = array[j+(-1)]; start = j;
	}
	array[start+(-1)] = pivot;
}

/**
* @brief array heap sort ascending.
*
* @param array array.
* @param size array size.
*/
void array_heapsort(int* array, int size) {
	int i = 0;
	for (i = (size>>1); i > 0; i --) {
		array_heapadjust(array, size, i);
	}
	for (i = size; i > 1; i --) {
		SWAP(array[i+(-1)], array[0])
		array_heapadjust(array, i-1, 1);
	}
}

/**
* @brief all range without recursion and use next permutation method.
*
* @param array array.
* @param size array size.
*/
void allrange_permutation(int* array, int size) {
	array_heapsort(array, size);
	do {
		output_range_array(array, size); printf("\n");
	} while (has_next_permutation(array, size));
}

/**
* @brief test case or all range.
*/
void testcase_allrange() {
	int array[] = {1, 2, 3, 4};
	int size = sizeof(array) / sizeof(array[0]);
	// allrange_recursion(array, size, 0, 0);
	allrange_permutation(array, size);
}

int main(int argc, const char *argv[])
{
	testcase_allrange();

	return 0;
}
