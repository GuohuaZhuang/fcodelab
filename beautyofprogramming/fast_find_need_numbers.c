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
* @file fast_find_need_numbers.c
* @brief fast find neeed numbers like two summation number.
* @author firstboy0513
* @version 0.0.1
* @date 2013-12-09
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
* @brief print selected.
*
* @param array array.
* @param length array length.
* @param selected found number's index array, 1 is selected, 0 is not selected.
*/
void print_selected(const int* array, const int length, const int* selected) {
	int i = 0;
	printf("find selected element is: \n");
	for (i = 0; i < length; i ++) {
		if (0 == selected[i]) continue;
		printf("\tarray[%d] = %d\n", i, array[i]);
	}
}

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
* @brief sort array ascending use bubble algorithm.
*
* @param array array to sort.
* @param length array length.
*/
void array_sort_ascending(int* array, int length) {
	int i = 0, j = 0;
	for (i = 0; i < length-1; i ++) {
		for (j = i+1; j < length; j ++) {
			if (array[j] < array[i]) {
				array[j] ^= array[i];
				array[i] ^= array[j];
				array[j] ^= array[i];
			}
		}
	}
}

/**
* @brief recover the selected index in the const array from the copy array.
*
* @param array const array.
* @param length array length.
* @param array_copy const array's no-const copy.
* @param selected selected.
*/
void recover_array_index(const int* array, const int length, int* array_copy, 
	int* selected) {
	int* selected_copy = (int*) malloc(sizeof(int) * length);
	memcpy(selected_copy, selected, sizeof(int) * length);
	memset(selected, 0, sizeof(int) * length);
	int i = 0, j = 0;
	for (j = 0; j < length; j ++) {
		if (0 == selected_copy[j]) continue;
		for (i = 0; i < length; i ++) {
			if (array_copy[j] != array[i]) continue;
			selected[i] = 1;
			break;
		}
	}
	free(selected_copy);
}

/**
* @brief fast to find two summation number as the given summation.
*
* @param array array.
* @param length array length.
* @param summation two number's given summation.
*
* @return the number found index as a selected array point. You should release 
* it outside.
*/
int* fast_find_two_summation_number(const int* array, const int length, 
	const int summation, int sorted) {
	if (!array || length <= 0) {
		printf("fast_find_need_numbers input invalid!\n");
		return NULL;
	}
	int* array_copy = (int*)array;
	if (!sorted) {
		array_copy = (int*) malloc(sizeof(int) * length);
		memcpy(array_copy, array, sizeof(int) * length);
		array_sort_ascending(array_copy, length);
	}
	int* selected = (int*) malloc(sizeof(int) * length);
	memset(selected, 0, sizeof(int) * length);

	int left_index = 0, right_index = length - 1;
	while (left_index < right_index) {
		int current_sum = array_copy[left_index] + array_copy[right_index];
		if (current_sum == summation) {
			selected[left_index] = selected[right_index] = 1;
			break;
		} else if (current_sum > summation) {
			right_index --;
		} else { // current_sum < summation
			left_index ++;
		}
	}
	if (left_index >= right_index) {
		if (!sorted) free(array_copy);
		free(selected);
		return NULL;
	}
	if (!sorted) {
		recover_array_index(array, length, array_copy, selected);
		free(array_copy);
	}
	return selected;
}

/**
* @brief fast to find three summation number as the given summation.
*
* @param array array.
* @param length array length.
* @param summation three number's given summation.
*
* @return the number found index as a selected array point. You should release 
* it outside.
*/
int* fast_find_three_summation_number(const int* array, const int length, 
	const int summation) {
	if (!array || length <= 0) {
		printf("fast_find_need_numbers input invalid!\n");
		return NULL;
	}
	int* array_copy = (int*) malloc(sizeof(int) * length);
	memcpy(array_copy, array, sizeof(int) * length);
	array_sort_ascending(array_copy, length);
	int* selected = (int*) malloc(sizeof(int) * length);
	memset(selected, 0, sizeof(int) * length);
	int* sub_selected = NULL;
	int i = 0;
	for (i = 0; i < length - 2; i ++) {
		if (NULL != (sub_selected = fast_find_two_summation_number(
				array_copy+i+1, length-i-1, summation-array_copy[i], 1)) ) {
			memcpy(selected+i+1, sub_selected, sizeof(int) * (length - i));
			selected[i] = 1;
			free(sub_selected);
			break;
		}
	}
	print_selected(array_copy, length, selected);
	recover_array_index(array, length, array_copy, selected);
	free(array_copy);
	return selected;
}

/**
* @brief fast find any closest summation number.
* It is means to find any sub-set in the array which its summation is closest 
* to the given summation. It is a nondeterministic polynomial time.
*
* @param array array.
* @param length array length.
* @param summation given summation.
*
* @return the number found index as a selected array point. You should release 
* it outside.
*/
int* fast_find_any_closest_summation_number(const int* array, const int length, 
	const int summation) {
	if (!array || length <= 0) {
		printf("fast_find_need_numbers input invalid!\n");
		return NULL;
	}
	int* selected = (int*) malloc(sizeof(int) * length);
	memset(selected, 0, sizeof(int) * length);
	int* closest_selected = (int*) malloc(sizeof(int) * length);
	memset(closest_selected, 0, sizeof(int) * length);
	int k = 0, closest_sum = 0, current_sum = 0;
	while (1) {
		current_sum = get_summation(array, length, selected);
		if (current_sum <= summation && current_sum > closest_sum) {
			closest_sum = current_sum;
			memcpy(closest_selected, selected, sizeof(int) * length);
		}
		k = length - 1;
		while (k >= 0) {
			if (selected[k] == 0) { selected[k] = 1; break; }
			selected[k] = 0; k --;
		}
		if (k < 0) break;
	}
	free(selected);
	return closest_selected;
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
int* get_summation_selected(const int* array, const int length, int summation) {
	int* selected = (int*) malloc(sizeof(int) * length);
	memset(selected, 0, sizeof(int) * length);
	int k = 0;
	while (1) {
		if (summation == get_summation(array, length, selected)) {
			break;
		}
		k = length - 1;
		while (k >= 0) {
			if (selected[k] == 0) {
				selected[k] = 1; break;
			}
			selected[k] = 0; k --;
		}
		if (k < 0) break;
	}
	return selected;
}

/**
* @brief fast find any closest summation use storage method which is N^3 times.
* It must assume all element in the array is positive number.
* It use more important that summation is smail, it the summation is a big 
* number, then we advice not use it.
*
* @param array array.
* @param length array length.
* @param summation the given summation.
*
* @return the number found index as a selected array point. You should release 
* it outside.
*/
int* fast_find_any_closest_summation_number_use_N3_times(
	const int* array, const int length, const int summation) {
	if (!array || length <= 0) {
		printf("fast_find_any_closest_summation_number input invalid!\n");
		return NULL;
	}
	int i = 0, j = 0, k = 0, closest_sum = 0;
	int** N = (int**) malloc(sizeof(int*) * (summation+1));
	for (i = 0; i < (summation+1); i ++) {
		N[i] = (int*) malloc(sizeof(int) * (summation+1));
		memset(N[i], 0, sizeof(int) * (summation+1));
	}
	N[0][0] = 1;
	for (i = 0; i < length; i ++) {
		j = (i < summation ? i+1 : summation);
		for (; j > 0; j --) {
			for (k = 1; k <= summation; k ++) {
				if (k >= array[i] && N[j-1][k-array[i]] == 1) {
					N[j][k] = 1;
				}
			}
		}
	}
	for (k = summation; k > 0; k --) {
		for (j = summation; j > 0; j --) {
			if (N[j][k] == 1) { closest_sum = k; break; }
		}
		if (N[j][k] == 1) break;
	}
	for (i = 0; i < (summation+1); i ++) { free(N[i]); } free(N);
	return get_summation_selected(array, length, closest_sum);
}

int main(int argc, const char *argv[])
{
	int array[] = {5, 6, 1, 4, 7, 9, 8};
	int length = sizeof(array) / sizeof(array[0]);
	//int* selected = fast_find_two_summation_number(array, length, 10, 0);
	//int* selected = fast_find_three_summation_number(array, length, 10);
	//int* selected = fast_find_any_closest_summation_number(array, length, 10);
	int* selected = fast_find_any_closest_summation_number_use_N3_times(
		array, length, 10);
	print_selected(array, length, selected);
	free(selected);
	return 0;
}
