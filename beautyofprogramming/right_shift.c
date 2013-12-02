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
* @file right_shift.c
* @brief right shift array by given k bit.
* @author firstboy0513
* @version 0.0.1
* @date 2013-12-02
*/
#include <stdio.h>
#include <stdlib.h>

typedef char ELE;

/**
* @brief Right shift use grouping move.
* It's time complexity use O(N*K), and use 2 additional variables.
*
* @param arr element array.
* @param N array size.
* @param K right shift length.
*/
void RightShift(ELE* arr, int N, int K) {
	K %= N;
	while (K--) {
		ELE t = arr[0];
		int i = 0;
		for (i = 0; i+1 < N; i ++) {
			arr[i] = arr[i+1];
		}
		arr[N-1] = t;
	}
}

/**
* @brief Right shift use grouping move.
* It's time complexity use O(N), but use 3 additional variables.
*
* @param arr element array.
* @param N array size.
* @param K right shift length.
*/
void RightShiftGrouping(ELE* arr, int N, int K) {
	K %= N;
	int i = 0, j = 0;
	for (i = 0; i < K; i ++) {
		ELE t = arr[i];
		for (j = i; j+K < N; j += K) {
			arr[j] = arr[j+K];
		}
		arr[j] = t;
	}
}

/**
* @brief Reverse arr element between left index and right index.
*
* @param arr element array.
* @param left_index left index.
* @param right_index right index.
*/
void Reverse(ELE* arr, int left_index, int right_index) {
	for (; right_index > left_index; right_index --, left_index ++) {
		arr[right_index] ^= arr[left_index];
		arr[left_index] ^= arr[right_index];
		arr[right_index] ^= arr[left_index];
//		ELE t = arr[right_index];
//		arr[right_index] = arr[left_index];
//		arr[left_index] = t;
	}
}

/**
* @brief Right shift use 3 reverse method.
* It's time complexity use O(N), and use even 0 additional variables.
* Alghouth O(N) is (2N).
*
* @param arr element array.
* @param N array size.
* @param K right shift length.
*/
void RightShiftReverse(ELE* arr, int N, int K) {
	K %= N;
	Reverse(arr, 0, K-1);
	Reverse(arr, K, N-1);
	Reverse(arr, 0, N-1);
}

/**
* @brief Print array from high index to lower index.
*
* @param arr element array.
* @param n_test_arr_length array length.
*/
void PrintArray(const ELE* arr, const int n_arr_length) {
	int i = n_arr_length;
	while (i--) {
		printf("%c", arr[i]);
	}
	printf("\n");
}

int main(int argc, const char *argv[])
{
	// prepare a test array, note high index and lower index.
	const ELE test_arr[] = {'a', 'b', 'c', 'd', '1', '2', '3', '4'};
	int n_test_arr_length = sizeof(test_arr) / sizeof(test_arr[0]);
	ELE* arr = (ELE*) malloc(sizeof(ELE) * n_test_arr_length);
	int i = 0;
	for (i = 0; i < n_test_arr_length; i ++) {
		arr[i] = test_arr[n_test_arr_length-i-1];
	}

	// RightShift(arr, n_test_arr_length, 4);
	// RightShiftGrouping(arr, n_test_arr_length, 4);
	RightShiftReverse(arr, n_test_arr_length, 4);
	PrintArray(arr, n_test_arr_length);

	free(arr);
	return 0;
}
