/* Copyright (C) 
* 2014 - firstboy0513
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
* @file buybook_with_discount.c
* @brief buy book with discount problem.
* @author firstboy0513
* @version 0.0.1
* @date 2014-01-04
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
* @brief dp struct with dp array, multiply like base of array, and also 
* discount for book types.
*/
typedef struct _dp {
	double* _dp;
	int* _mul;
	double* _discount;
	int _dp_size;
	int _mul_size;
} DP;

/**
* @brief dynamic programming initialize struct.
*
* @param books books.
* @param size books count.
* @param discount book discount correspond buy multiply types of books.
*
* @return return DP struct.
*/
DP* dp_array_init(const int* books, const int size, const double* discount) {
	int i = 0, dp_size = 1;
	int* mul = (int*) malloc(sizeof(int) * size);
	double* _discount = (double*) malloc(sizeof(double) * size);
	for (i = 0; i < size; i ++) {
		mul[i] = dp_size;
		if (books[i] > 0) { dp_size *= (books[i] + 1); }
		_discount[i] = discount[i];
	}
	double* array = (double*) malloc(sizeof(double) * dp_size);
	memset(array, 0, sizeof(double) * dp_size);
	DP* dp = (DP*) malloc(sizeof(DP));
	dp->_dp = array; dp->_mul = mul; dp->_discount = _discount;
	dp->_dp_size = dp_size; dp->_mul_size = size;
	return dp;
}

/**
* @brief release dynamic programming struct memory.
*
* @param dp
*/
void dp_array_release(DP* dp) {
	free(dp->_mul); free(dp->_discount); free(dp->_dp); free(dp);
}

/**
* @brief give books count with different types, return the index in the dp 
* struct.
*
* @param dp DP struct.
* @param books books.
* @param size books types count.
*
* @return return index in the DP struct.
*/
int dp_array_index(DP* dp, const int* books, const int size) {
	int i = 0, index = 0;
	for (i = 0; i < size; i ++) {
		index += (books[i] * dp->_mul[i]);
	}
	return index;
}

/**
* @brief set dp value in the dp struct array.
*
* @param dp dp struct.
* @param books books.
* @param size books array size.
* @param pay the minimum pay value.
*/
void dp_array_set(DP* dp, const int* books, const int size, const double pay) {
	int index = dp_array_index(dp, books, size);
	if (index >= 0) { dp->_dp[index] = pay; }
}

/**
* @brief get dp value in the dp struct array.
*
* @param dp dp struct.
* @param books books.
* @param size books array size.
*
* @return return the minimum pay value in dp_array_get.
*/
double dp_array_get(DP* dp, const int* books, const int size) {
	int index = dp_array_index(dp, books, size);
	if (index >= 0) {
		return dp->_dp[dp_array_index(dp, books, size)];
	} else {
		return 0;
	}
}

/**
* @brief output books, use for debug show information.
*
* @param books books array.
* @param size books array size.
*/
void output_books(const int* books, const int size) {
	printf("[books]: ");
	int i = 0;
	for (i = 0; i < size; i ++) {
		if (0 != i) { printf(", "); }
		printf("%d", books[i]);
	}
	printf("\n");
}

#define SWAP_INT(a, b) { (a) ^= (b); (b) ^= (a); (a) ^= (b); }

/**
* @brief just 5 types to sort, so use bubble sort is ok.
*
* @param books books array be sort.
* @param size books array size.
*/
void array_des_sort(int* books, const int size) {
	int i = 0, j = 0;
	for (i = 0; i < size-1; i ++) {
		for (j = i+1; j < size; j ++) {
			if (books[i] < books[j]) {
				SWAP_INT(books[i], books[j]);
			}
		}
	}
}

/**
* @brief recursive implement dynamic programming to find minimum pay to buy the
* discount book combines.
*
* @param dp dp struct.
* @param books books array.
* @param size books array size.
*
* @return return minimum pay.
*/
double dp_lowest_recursion(DP* dp, int* books, int size) {
	array_des_sort(books, size);
	if (books[0] <= 0) { return 0; }
	double min_discount = -1, cur_discount = 0;
	if ((min_discount = dp_array_get(dp, books, size)) > 0) {
		return min_discount;
	} else {
		min_discount = -1;
	}
	int i = 0, j = 0;
	for (i = size-1; i >= 0; i --) {
		if (books[i] >= 1) {
			int* books_copy = (int*) malloc(sizeof(int) * size);
			memcpy(books_copy, books, sizeof(int) * size);
			for (j = 0; j <= i; j ++) { books_copy[j] --; }
			cur_discount = (i+1) * 8.0 * (1.0 - dp->_discount[i]) 
				+ dp_lowest_recursion(dp, books_copy, size);
			free(books_copy);
			if (min_discount < 0 || min_discount > cur_discount) {
				min_discount = cur_discount;
			}
		}
	}
	dp_array_set(dp, books, size, min_discount);
	return min_discount;
}

/**
* @brief output lowest price, also as the minimum pay in the global books buy.
*
* @param books books array.
* @param size books array size.
* @param discount discount array.
*/
void output_dp_lowest_price(const int* books, const int size, 
	const double* discount) {
	int* books_copy = (int*) malloc(sizeof(int) * size);
	memcpy(books_copy, books, sizeof(int) * size);
	array_des_sort(books_copy, size);

	DP* dp = dp_array_init(books_copy, size, discount);
	dp_lowest_recursion(dp, books_copy, size);

	memcpy(books_copy, books, sizeof(int) * size);
	array_des_sort(books_copy, size);
	printf("minimum pay = %g\n", dp->_dp[dp->_dp_size-1]);

	free(books_copy);
	dp_array_release(dp);
}

int main(int argc, const char *argv[])
{
	int books[] = {2, 2, 1, 2, 1};
	double discount[] = {0.0, 0.05, 0.10, 0.20, 0.25};
	int books_size = sizeof(books) / sizeof(books[0]);
	output_dp_lowest_price(books, books_size, discount);

	return 0;
}
