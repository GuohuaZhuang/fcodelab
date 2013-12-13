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
* @file sub2darray_maximum_summation.c
* @brief get the maximum summation from 2-dimensional array.
* @author firstboy0513
* @version 0.0.1
* @date 2013-12-13
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int max(int a, int b) { return (a > b ? a : b); }

/**
* @brief accumulate the array summation from (i_min,j_min) to (i_max,j_max).
*
* @param A[][7] array.
* @param i_min i_min.
* @param i_max i_max.
* @param j_min j_min.
* @param j_max j_max.
*
* @return summation.
*/
int array_sum(const int A[][7], int i_min, int i_max, int j_min, int j_max) {
	int sum = 0, i = 0, j = 0;
	for (i = i_min; i <= i_max; i ++) {
		for (j = j_min; j <= j_max; j ++) {
			sum += A[i][j];
		}
	}
	return sum;
}

/**
* @brief bet maximum summation in the sub2array.
* To get the ith line and mth column is A[i][j].
* It take O(n^2*m^2*sum) time complexity.
*
* @param A 2-dimensional array.
* @param n 2-dimensional array line count.
* @param m 2-dimensional array column count.
*
* @return return maximum summation, or return INT32_MIN means error occur.
*/
int sub2array_maximum_summation(const int A[][7], const int n, const int m) {
	int max_sum = INT32_MIN;
	int i_min = 0, i_max = 0, j_min = 0, j_max = 0;
	for (i_min = 0; i_min < n; i_min ++) {
		for (i_max = i_min; i_max < n; i_max ++) {
			for (j_min = 0; j_min < m; j_min ++) {
				for (j_max = j_min; j_max < m; j_max ++) {
					max_sum = max(max_sum, 
						array_sum(A, i_min, i_max, j_min, j_max));
				}
			}
		}
	}
	return max_sum;
}

/**
* @brief bet maximum summation in the sub2array.
* To get the ith line and mth column is A[i][j].
* It take O(n^2*m^2) time complexity, like dynamic programming.
*
* @param A 2-dimensional array.
* @param n 2-dimensional array line count.
* @param m 2-dimensional array column count.
*
* @return return maximum summation, or return INT32_MIN means error occur.
*/
int sub2array_maximum_summation_dp(const int A[][7], const int n, const int m) {
	// init boundary ans part-summation array.
	int i = 0, j = 0;
	int** PS = (int**) malloc(sizeof(int*) * (n+1));
	for (i = 0; i <= n; i ++) { PS[i] = (int*) malloc(sizeof(int) * (m+1));
		PS[i][0] = 0; }
	for (j = 0; j <= m; j ++) { PS[0][j] = 0; }
	for (i = 1; i <= n; i ++) {
		for (j = 1; j <= m; j ++) {
			PS[i][j] = PS[i-1][j] + PS[i][j-1] - PS[i-1][j-1] + A[i-1][j-1];
		}
	}
	// use exhaustion method to find maximum summation.
	int max_sum = INT32_MIN;
	int i_min = 0, i_max = 0, j_min = 0, j_max = 0;
	for (i_min = 0; i_min < n; i_min ++) {
		for (i_max = i_min; i_max < n; i_max ++) {
			for (j_min = 0; j_min < m; j_min ++) {
				for (j_max = j_min; j_max < m; j_max ++) {
					max_sum = max(max_sum, (PS[i_max+1][j_max+1]
						- PS[i_min][j_max+1] - PS[i_max+1][j_min]
						+ PS[i_min][j_min]) );
				}
			}
		}
	}
	for (i = 0; i <= n; i ++) { free(PS[i]); } free(PS);
	return max_sum;
}

/**
* @brief get part summation from line: i_min to i_max, and the column is m.
* This function use for sub2array_maximum_summation_dp_optimize.
*
* @param PS part-summation array.
* @param i_min i_min.
* @param i_max i_max.
* @param m column position.
*
* @return return the give column line summation.
*/
int BC(int** PS, int i_min, int i_max, int m) {
	return (PS[i_max+1][m+1] - PS[i_max+1][m] - PS[i_min][m+1] + PS[i_min][m]);
}

/**
* @brief bet maximum summation in the sub2array.
* To get the ith line and mth column is A[i][j].
* It take O(n^2*m) time complexity, like dynamic programming.
*
* @param A 2-dimensional array.
* @param n 2-dimensional array line count.
* @param m 2-dimensional array column count.
*
* @return return maximum summation, or return INT32_MIN means error occur.
*/
int sub2array_maximum_summation_dp_optimize(
	const int A[][7], const int n, const int m) {
	// init boundary and part-summation array.
	int i = 0, j = 0;
	int** PS = (int**) malloc(sizeof(int*) * (n+1));
	for (i = 0; i <= n; i ++) { PS[i] = (int*) malloc(sizeof(int) * (m+1));
		PS[i][0] = 0; }
	for (j = 0; j <= m; j ++) { PS[0][j] = 0; }
	for (i = 1; i <= n; i ++) {
		for (j = 1; j <= m; j ++) {
			PS[i][j] = PS[i-1][j] + PS[i][j-1] - PS[i-1][j-1] + A[i-1][j-1];
		}
	}
	// use exhaustion method to find maximum summation.
	int max_sum = INT32_MIN, start = 0, best = 0;
	int i_min = 0, i_max = 0;
	for (i_min = 0; i_min < n; i_min ++) {
		for (i_max = i_min; i_max < n; i_max ++) {
			start = BC(PS, i_min, i_max, m-1);
			best = start;
			for (i = m-2; i >= 0; i --) {
				if (start < 0) { start = 0; }
				start += BC(PS, i_min, i_max, i);
				if (start > best) { best = start; }
				if (best > max_sum) { max_sum = best; }
			}
		}
	}
	for (i = 0; i <= n; i ++) { free(PS[i]); } free(PS);
	return max_sum;
}

int main(int argc, const char *argv[])
{
	const int A[][7] = {
		{-2, 5, 3, -6, 4, -8, 6},
		{1, -2, 3, 5, -3, 2, 3},
		{0, -2, 3, 5, -1, 2, 1},
		{-9, -2, -3, -5, -3, -2, -3}
	};
	const int n = sizeof(A) / sizeof(A[0]);
	const int m = sizeof(A[0]) / sizeof(A[0][0]);
	// int max_sum = sub2array_maximum_summation(A, n, m);
	// int max_sum = sub2array_maximum_summation_dp(A, n, m);
	int max_sum = sub2array_maximum_summation_dp_optimize(A, n, m);
	printf("max_sum = %d\n", max_sum);
	return 0;
}
