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
* @file split_array_summation.cpp
* @brief split array summation use accumulate method.
* @author firstboy0513
* @version 0.0.3
* @date 2013-12-10
*/
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <vector>

using std::vector;

#ifndef INT32_MIN
#define INT32_MIN      (-2147483647-1)
#endif // INT32_MIN

/**
* @brief get summation by given array and its bag flag 0,1 array.
*
* @param array array.
* @param length array length.
* @param bag bag flag 0,1 array.
*
* @return summation.
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
* @brief get a summation split by given summation.
*
* @param array array.
* @param length array length.
* @param summation given summation.
*
* @return split bag array.
*/
int* get_summation_split(const int* array, const int length, int summation) {
	int* split = (int*) malloc(sizeof(int) * length);
	memset(split, 0, sizeof(int) * length);
	int k = 0, split_count = 0, half_length = length / 2;
	while (1) {
		if (split_count == half_length 
			&& summation == get_summation(array, length, split)) {
			break;
		}
		k = length - 1;
		while (k >= 0) {
			if (split[k] == 0) {
				split[k] = 1; split_count ++; break;
			}
			split[k] = 0; split_count --; k --;
		}
		if (k < 0) break;
	}
	return split;
}

/**
* @brief split array summation use accumulate method.
* heap[0] to heap[half_length] means have the count number to summation.
* Like this:
* heap[0] = {empty}
* heap[1] = {(1), (5), (7)}
* heap[2] = {(1,5), (1,7), (5,7)}
* heap[3] = {(1,5,7),...}
* first initiate heap[0] = {empty}, the add (1) + empty to heap[1], 
* then add (5) + {empty} to heap[1] and (5) + (1) to heap[2], and so on.
*
* @param array array.
* @param length array length.
*
* @return split flag array.
*/
int* split_array_summation_use_accumulate(const int* array, const int length) {
    if (!array || length <= 0 || length % 2 != 0) {
        printf("split_array_summation input invalid!\n");
        return NULL;
    }
	int i = 0, j = 0, current_max_index = 0, current_sum = 0;
	int half_length = length / 2, half_sum = 0, max_half_sum = INT32_MIN;
	for (i = 0; i < length; i ++) { half_sum += array[i]; } half_sum /= 2;
	vector< vector<int> > heap; heap.resize(half_length + 1);
	vector<int>::iterator isubheap;
	heap[0].push_back(0);
	for (i = 0; i < length; i ++) { // add array[i] now
		current_max_index = (i < half_length) ? i : (half_length-1);
		for (j = current_max_index; j >= 0; j --) { // push heap 1 ~ n
			for (isubheap = heap[j].begin(); isubheap != heap[j].end();
				isubheap ++) {
				if (half_sum >= (current_sum = (*isubheap) + array[i])) {
					heap[j+1].push_back(current_sum);
				}
			}
		}
	}
	for (isubheap = heap[half_length].begin(); 
		isubheap != heap[half_length].end(); isubheap ++) {
		if (max_half_sum < *isubheap) {
			max_half_sum = *isubheap;
		}
	}
	return get_summation_split(array, length, max_half_sum);
}

/**
* @brief split array summation use back accumulate.
* This method is just back accumulate.Like this:
* heap[0] = {1, 0, 0, 0, .... } // another 0 is v-1 times.
* then want to find if(v-array[i]) is ok which be set as 1.
* we found heap[1-0][v-1] when v=1 is ok, because heap[0][0] = 1, 
* so set heap[1][1] = 1.
* also, we found heap[1-0][v-5] when v=5 is ok, heap[1-0][v-7] when v=7 is ok,
* so set heap[1][5] = 1, and heap[1][7] = 1.
* also, we found heap[2-1][v-1] when v=6 is ok, because heap[1][5] is ok now,
* and set heap[2][6] = 1. it means the 2 floor v=1 means (6-5)=1.
*
* @param array array.
* @param length array length.
*
* @return split flag array point.
*/
int* split_array_summation_use_back_accumulate(const int* array, 
	const int length) {
    if (!array || length <= 0 || length % 2 != 0) {
        printf("split_array_summation input invalid!\n");
        return NULL;
    }
	int i = 0, j = 0, v = 0, current_max_index = 0;
	int half_length = length / 2, half_sum = 0, max_half_sum = INT32_MIN;
	for (i = 0; i < length; i ++) { half_sum += array[i]; } half_sum /= 2;
	vector< vector<int> > heap; heap.resize(half_length + 1);
	for (i = 0; i <= half_length; i ++) { heap[i].resize(half_sum + 1); }
	vector<int>::iterator isubheap;
	heap[0][0] = 1;
	for (i = 0; i < length; i ++) {
		current_max_index = (i < half_length ? i+1 : half_length);
		for (j = current_max_index; j > 0; j --) {
			for (v = 1; v <= half_sum; v ++) {
				if (v >= array[i] && heap[j - 1][v - array[i]] == 1) {
					heap[j][v] = 1;
				}
			}
		}
	}
	for (v = half_sum; v >= 0; v --) {
		if (heap[half_length][v] == 1) {
			max_half_sum = v;
			break;
		}
	}
	return get_summation_split(array, length, max_half_sum);
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
	// int* split = split_array_summation_use_accumulate(array, length);
	int* split = split_array_summation_use_back_accumulate(array, length);
	if (split) {
		print_result(array, length, split);
		free(split);
	}
	return 0;
}
