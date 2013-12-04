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
* @file consecutive_number_summation.c
* @brief consecutive number summation.
* @author firstboy0513
* @version 0.0.2
* @date 2013-12-01
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#define PRINT_OUTPUT

// printf macros for size_t, in the style of inttypes.h
#ifdef _LP64
#define __PRIS_PREFIX "z"
#else
#define __PRIS_PREFIX
#endif

// Use these macros after a % in a printf format string
// to get correct 32/64 bit behavior, like this:
// size_t size = records.size();
// printf("%"PRIuS"\n", size);
#define PRIdS __PRIS_PREFIX "d"
#define PRIxS __PRIS_PREFIX "x"
#define PRIuS __PRIS_PREFIX "u"
#define PRIXS __PRIS_PREFIX "X"
#define PRIoS __PRIS_PREFIX "o"

// #define MAX_INT64_T (9223372036854775807)
#define MAX_INT64_T (0x7fffffffffffffff)

/**
* @brief print number in 64bit compatible output.
*
* @param a input number.
*/
void print_number(int64_t a) {
#ifdef PRINT_OUTPUT
	printf("a = %"PRIdS", (0x%"PRIxS")\n", a, a);
#endif // PRINT_OUTPUT
}

/**
* @brief print consecutive number summation list.
*
* @param a input number.
* @param n_start_number start number.
* @param f_direction direction right is 0, left is 1.
*/
void print_consecutive_number_summation(int64_t a, int64_t n_start_number, 
	int64_t f_direction) {
#ifdef PRINT_OUTPUT
	int64_t a_backup = a;
	printf("%"PRIdS" = ", a);
	while (a > 0) {
		if (a != a_backup) printf(" + ");
		printf("%"PRIdS"", n_start_number);
		a -= n_start_number;
		(f_direction == 0) ? (n_start_number --) : (n_start_number ++);
	}
	printf("\n");
#endif // PRINT_OUTPUT
}

/**
* @brief print all consecutive number summation use plus traversal.
* It use 0m0.016s in my computer with 2~1000 numbers.
*
* @param a input number.
*
* @return return summation combina count.
*/
int print_all_consecutive_number_summation(int64_t a) {
	if (a <= 2) return 0;
	int64_t n_summation_count = 0;
	int64_t n_right_number = a - 1;
	while (n_right_number > 0) {
		int64_t n_pos_number = n_right_number;
		int64_t n_summation = 0;
		while (n_summation < a && n_pos_number > 0) {
			n_summation += n_pos_number;
			n_pos_number --;
		}
		if (n_summation == a) {
			print_consecutive_number_summation(a, n_right_number, 0);
			n_summation_count ++;
		}
		n_right_number --;
	}
	return n_summation_count;
}

/**
* @brief print all consecutive number summation use equation.
* 	sum = nm - n(n-1)/2
* n is begin 2 and then increase by degree 1.
* Derivation:
* 	sum_2 = m + (m-1) = 2m-1
* 	sum_3 = m + (m-1) + (m-2) = 3m-(1+2)
* 	sum_4 = m + (m-1) + (m-2) + (m-3) = 4m-(1+2+3)
* 	...
* 	sum_n = m + (m-1) + ... + (m-(n-1)) = nm - (1+2+...+(n-1)) = nm - n(n-1)/2
* 
* It use real 0m4.758s in my computer with 2~1000 numbers.
*
* @param a input number.
*
* @return return summation combina count.
*/
int print_all_consecutive_number_summation_use_equation(int64_t a) {
	if (a <= 2) return 0;
	int64_t n_summation_count = 0;
	int64_t n_right_number = a - 1;
	while (n_right_number > 1) {
		int64_t n_summation = 0;
		int n = 2;
		while (1) {
			n_summation = n * n_right_number - ((n * (n-1)) / 2);
			if (n_summation == a) {
				print_consecutive_number_summation(a, n_right_number, 0);
				n_summation_count ++;
			} else if (n_summation > a) {
				break;
			}
			n ++;
		}
		n_right_number --;
	}
	return n_summation_count;
}

/**
* @brief print all consecutive number summation use divide.
* As we know, sum = n*m - (n*(n-1))/2, so it means:
* 	[sum + (n*(n-1))/2] % n == 0
* Also as not to transboundary, we can use 
* 	sum = m + (m+1) + (m+2) + ... + (m+n-1)
* 	    = n*m + (n*(n-1))/2
* so, 
* 	[sum - (n*(n-1))/2] % n == 0
* And we know, max n <= sqrt(a); but I think it's be a/2 better.
* It use real 0m0.004s in my computer with 2~1000 numbers.
* Consider little number to calculate, we know [sum - (n*(n-1))/2] >= 0
* [sum - (n*(n-1))/2] > [sum - (n^2)/2], so we get max n = sqrt(sum*2)
* and let minus value [sum-(n*(n-1))/2] not to be less than 0.
*
* @param a input number.
*
* @return return summation combina count.
*/
int print_all_consecutive_number_summation_use_divide(int64_t a) {
	if (a <= 2) return 0;
	int64_t n_summation_count = 0;
	int64_t n_max_count = (int64_t)sqrt((double)a*2);
	int64_t n = 0;
	for (n = 2; n <= n_max_count; n ++) {
		int64_t mod = (n*(n-1))/2;
		int64_t minus = a-mod;
		if (minus <= 0) break;
		if (minus%n == 0) {
			print_consecutive_number_summation(a, (minus/n), 1);
			n_summation_count ++;
		}
	}
	return n_summation_count;
}

/**
* @brief get count of consecutive number summation.
*
* @param a input number.
*
* @return consecutive number.
*/
int get_consecutive_number_summation_count_use_divide(int64_t a) {
	if (a <= 2) return 0;
	int64_t n_summation_count = 0;
	int64_t n_max_count = (int64_t)sqrt((double)a*2);
	int64_t n = 0;
	for (n = 2; n <= n_max_count; n ++) {
		int64_t mod = (n*(n-1))/2;
		int64_t minus = a-mod;
		if (minus <= 0) break;
		if (minus%n == 0) {
			// print_consecutive_number_summation(a, (minus/n), 1);
			n_summation_count ++;
		}
	}
	return n_summation_count;
}

/**
* @brief print max consecutive between 1 to MAX_INT64_T.
*/
void print_max_consecutive_number_summation_count_use_divide() {
	int64_t n_max_count = 0;
	int64_t x = 1;
	for (x = 1; x < MAX_INT64_T; x ++) {
		int64_t count = get_consecutive_number_summation_count_use_divide(x);
		if (n_max_count < count) {
			#ifdef PRINT_OUTPUT
			printf("x = %"PRIdS", (0x%016"PRIxS"), count = %"PRIdS"\n", 
				x, x, count);
			fflush(stdout);
			#endif // PRINT_OUTPUT
			n_max_count = count;
		}
	}
}

int main(int argc, const char *argv[])
{
	int64_t x = 1;
	for (x = 1; x < 100/*MAX_INT64_T*/; x ++) {
		// int64_t count = 
		// 	print_all_consecutive_number_summation(x);
		// int64_t count = 
		// 	print_all_consecutive_number_summation_use_equation(x);
		int64_t count = 
			print_all_consecutive_number_summation_use_divide(x);
		if (0 == count) print_number(x);
	}
	// print_max_consecutive_number_summation_count_use_divide();
	return 0;
}
