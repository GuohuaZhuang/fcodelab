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
* @file find_array_extremum.c
* @brief array extremum to get maximum and minimum element.
* @author firstboy0513
* @version 0.0.1
* @date 2013-12-13
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/**
* @brief array extremum before verify.
*
* @param array array.
* @param length array length.
* @param pmax max element point.
* @param pmin min element point.
*
* @return return 0 if invalid, otherwise return 1.
*/
int array_extremum_verfy(const int* array, const int length, 
	int* pmax, int* pmin) {
	if (!array || length <= 0) {
		printf("array_extremum_verfy input invalid!\n");
		return 0;
	}
	if (!pmax || !pmin) {
		printf("array_extremum_verfy input invalid!\n");
		return 0;
	}
	return 1;
}

/**
* @brief array extremum use O(2*n) time complexity.
*
* @param array array.
* @param length array length.
* @param pmax max element point.
* @param pmin min element point.
*/
void array_extremum_2n(const int* array, const int length, 
	int* pmax, int* pmin) {
	if (!array_extremum_verfy(array, length, pmax, pmin)) return;
	int i = 0;
	for (i = 0; i < length; i ++) {
		if (array[i] > *pmax) { *pmax = array[i]; }
		if (array[i] < *pmin) { *pmin = array[i]; }
	}
}

/**
* @brief array extremum use O(1.5*n) time complexity.
*
* @param array array.
* @param length array length.
* @param pmax max element point.
* @param pmin min element point.
*/
void array_extremum_1p5n(const int* array, const int length, 
	int* pmax, int* pmin) {
	if (!array_extremum_verfy(array, length, pmax, pmin)) return;
	int i = 0, current_max = 0, current_min = 0;
	for (i = 0; i < length; i += 2) {
		if (i+1 > length) {
			current_max = current_min = array[i];
		} else {
			if (array[i] > array[i+1]) {
				current_max = array[i]; current_min = array[i+1];
			} else {
				current_min = array[i]; current_max = array[i+1];
			}
		}
		if (current_max > *pmax) { *pmax = current_max; }
		if (current_min < *pmin) { *pmin = current_min; }
	}
}

/**
* @brief array extremum use O(1.5*n) time complexity by use divide and comquer
* method, also be called for short DC algorithm.
*
* @param array array.
* @param length array length.
* @param pmax max element point.
* @param pmin min element point.
*/
void array_extremum_dc(const int* array, const int length, 
	int* pmax, int* pmin, int begin, int end) {
	if (!array_extremum_verfy(array, length, pmax, pmin)) return;
	if (end - begin <= 1) {
		if (array[begin] < array[end]) {
			*pmax = array[end]; *pmin = array[begin]; return;
		} else {
			*pmin = array[end]; *pmax = array[begin]; return;
		}
	}
	int lmax = 0, lmin = 0, rmax = 0, rmin = 0, middle = begin+(end-begin)/2;
	array_extremum_dc(array, length, &lmax, &lmin, begin, middle);
	array_extremum_dc(array, length, &rmax, &rmin, middle+1, end);
	*pmax = (lmax > rmax ? lmax : rmax);
	*pmin = (lmin < rmin ? lmin : rmin);
}

int main(int argc, const char *argv[])
{
	const int array[] = {5, 6, 8, 3, 7, 9};
	const int length = sizeof(array) / sizeof(array[0]);
	int max = INT32_MIN, min = INT32_MAX;
	// array_extremum_2n(array, length, &max, &min);
	// array_extremum_1p5n(array, length, &max, &min);
	array_extremum_dc(array, length, &max, &min, 0, length-1);
	printf("max = %d, min = %d\n", max, min);
	return 0;
}
