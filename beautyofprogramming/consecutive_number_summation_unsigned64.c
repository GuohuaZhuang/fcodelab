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
* @version 0.0.3
* @date 2013-12-01
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
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

#define MAX_UINT64_T UINT64_MAX

/**
* @brief print number in 64bit compatible output.
*
* @param a input number.
*/
void print_number(uint64_t a) {
#ifdef PRINT_OUTPUT
	printf("a = %"PRIuS", (0x%"PRIxS")\n", a, a);
#endif // PRINT_OUTPUT
}

/**
* @brief print consecutive number summation list.
*
* @param a input number.
* @param n_start_number start number.
* @param f_direction direction right is 0, left is 1.
*/
void print_consecutive_number_summation(uint64_t a, uint64_t n_start_number, 
	uint64_t f_direction) {
#ifdef PRINT_OUTPUT
	printf("%"PRIuS" = ", a);
	while (a > 0) {
		printf("%"PRIuS" + ", n_start_number);
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
int print_all_consecutive_number_summation(uint64_t a) {
	if (a <= 2) return 0;
	uint64_t n_summation_count = 0;
	uint64_t n_right_number = a - 1;
	while (n_right_number > 0) {
		uint64_t n_pos_number = n_right_number;
		uint64_t n_summation = 0;
		while (n_summation < a && n_pos_number > 0) {
			if (MAX_UINT64_T - n_summation < n_pos_number) break;
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
int print_all_consecutive_number_summation_use_equation(uint64_t a) {
	if (a <= 2) return 0;
	uint64_t n_summation_count = 0;
	uint64_t n_right_number = a - 1;
	while (n_right_number > 1) {
		uint64_t n_summation = 0;
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
int print_all_consecutive_number_summation_use_divide(uint64_t a) {
	if (a <= 2) return 0;
	uint64_t n_summation_count = 0;
	uint64_t n_max_count = (uint64_t)sqrt((double)a*2);
	uint64_t n = 0;
	for (n = 2; n <= n_max_count; n ++) {
		uint64_t mod = (n*(n-1))/2;
		if (a <= mod) break;
		uint64_t minus = a-mod;
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
int get_consecutive_number_summation_count_use_divide(uint64_t a) {
	if (a <= 2) return 0;
	uint64_t n_summation_count = 0;
	uint64_t n_max_count = (uint64_t)sqrt((double)a*2);
	uint64_t n = 0;
	for (n = 2; n <= n_max_count; n ++) {
		uint64_t mod = (n*(n-1))/2;
		if (a <= mod) break;
		uint64_t minus = a-mod;
		if (minus%n == 0) {
			n_summation_count ++;
		}
	}
	return n_summation_count;
}

/**
* @brief print max consecutive between 1 to MAX_UINT64_T.
*/
void print_max_consecutive_number_summation_count_use_divide() {
	uint64_t n_max_count = 0;
	uint64_t x = 1;
	for (x = 1; x < MAX_UINT64_T; x ++) {
		uint64_t count = get_consecutive_number_summation_count_use_divide(x);
		if (n_max_count < count) {
			#ifdef PRINT_OUTPUT
			printf("x = %"PRIuS", (0x%016"PRIxS"), count = %"PRIuS"\n", 
				x, x, count);
			fflush(stdout);
			#endif // PRINT_OUTPUT
			n_max_count = count;
		}
	}
}

#ifndef PRIMES_COUNT
#define PRIMES_COUNT 15U
#endif // PRIMES_COUNT
/**
* @brief primes summation result struct.
*/
typedef struct _PRIME_SUM_RET {
	uint64_t m_primes_max_count[PRIMES_COUNT]; // Note: just for no recurse.
	uint64_t m_primes[PRIMES_COUNT]; // less than 51 primes.
	uint64_t m_power[PRIMES_COUNT]; // current powers.
	uint64_t m_bestpower[PRIMES_COUNT]; // best count corresponding powers.
	uint64_t m_number; // best count corresponding summation.
	uint64_t m_count; // best count.
} PRIME_SUM_RET;

/**
* @brief PRIME_SUM_RET struct initialization.
*
* @return malloc newer PRIME_SUM_RET struct point.
*/
PRIME_SUM_RET* ret_init() {
	PRIME_SUM_RET* r = (PRIME_SUM_RET*) malloc(sizeof(PRIME_SUM_RET));
	const uint64_t m_primes[] = {3,5,7,11,13,17,19,23,29,31,37,41,43,47,51};
	uint64_t i = 0U;
	for (; i < PRIMES_COUNT; i ++) {
		r->m_primes_max_count[i] = 
			ceil(log((double)UINT64_MAX)/log((double)m_primes[i]));
		r->m_primes[i] = m_primes[i];
		r->m_power[i] = 0U;
		r->m_bestpower[i] = 0U;
	}
	r->m_number = 1U;
	r->m_count = 1U;
	return r;
}

/**
* @brief release PRIME_SUM_RET struct.
*
* @param r PRIME_SUM_RET struct point.
*/
void ret_release(PRIME_SUM_RET* r) {
	free(r);
}

/**
* @brief print current powers and its summation combination.
*
* @param r PRIME_SUM_RET struct point.
*/
void ret_print_current(PRIME_SUM_RET* r) {
	uint64_t i = 0U;
	printf("current number = ");
	for (i = 0U; i < PRIMES_COUNT; i ++) {
		if (0U != i) { printf(" * "); }
		printf("%"PRIuS"^%"PRIuS"", r->m_primes[i], r->m_power[i]);
	} 
	printf("\n");
	printf("current power = ");
	for (i = 0U; i < PRIMES_COUNT; i ++) {
		if (0U != i) { printf(" * "); }
		printf("(%"PRIuS"+1)", r->m_power[i]);
	}
	printf("\n");
}

/**
* @brief print best powers and its summation combination and its number.
*
* @param r PRIME_SUM_RET struct point.
*/
void ret_print(PRIME_SUM_RET* r) {
	printf("m_number = %"PRIuS", m_count-1 = %"PRIuS" \n", r->m_number, 
		r->m_count-1);
	uint64_t i = 0U;
	printf("m_number = ");
	for (i = 0U; i < PRIMES_COUNT; i ++) {
		if (0U != i) { printf(" * "); }
		printf("%"PRIuS"^%"PRIuS"", r->m_primes[i], r->m_bestpower[i]);
	} 
	printf("\n");
	printf("m_count = ");
	for (i = 0U; i < PRIMES_COUNT; i ++) {
		if (0U != i) { printf(" * "); }
		printf("(%"PRIuS"+1)", r->m_bestpower[i]);
	}
	printf("\n");
}

/**
* @brief get consecutive number use recurse method, and recommend method.
*
* @param current_index current index, will set 1 when begin.
* @param sum current summation.
* @param current_count current count.
* @param prepower pre max index as power to limit the next cycle times.
* @param r PRIME_SUM_RET struct point.
*/
void get_consecutive_number(uint64_t current_index, uint64_t sum, 
	uint64_t current_count, uint64_t prepower, PRIME_SUM_RET* r) {
	if (current_index == PRIMES_COUNT) {
		if (r->m_count < current_count) {
			r->m_count = current_count;
			r->m_number = sum;
			memcpy(r->m_bestpower, r->m_power, sizeof(r->m_power));
		}
		return;
	}
	uint64_t i = 0;
	for (i = 0; i <= prepower; i ++) {
		if (0 != i) {
			if (sum > MAX_UINT64_T/r->m_primes[current_index]) break;
			sum *= r->m_primes[current_index];
		}
		r->m_power[current_index] = i;
		get_consecutive_number(current_index + 1, sum, 
			current_count*(i+1), i, r);
	}
}

/**
* @brief calculate number to find best count and record its powers and number.
*
* @param r PRIME_SUM_RET struct point.
* @param sum current summation.
* @param current_count current count.
*
* @return just return 1 now, not any means.
*/
int calculate_number(PRIME_SUM_RET* r, uint64_t sum, uint64_t current_count) {
//	printf("[calculate_number] sum = %"PRIuS", current_count = %"PRIuS"\n", 
//		sum, current_count);
//	uint64_t sum = 1U;
//	uint64_t i = 0U;
//	uint64_t current_count = 1U;
//	for (i = 0U; i < PRIMES_COUNT; i ++) {
//		if (r->m_power[i] > 0) {
//			uint64_t t = pow(r->m_primes[i], r->m_power[i]);
//			if (MAX_UINT64_T/t < sum) {
//				return 0;
//			}
//			sum *= t;
//		}
//		current_count *= (r->m_power[i]+1);
//	}
	if (r->m_count < current_count) {
		r->m_count = current_count;
		r->m_number = sum;
		memcpy(r->m_bestpower, r->m_power, sizeof(r->m_power));
	}
//	printf("\n");
//	ret_print_current(r);
//	ret_print(r);
//	printf("\n");
	return 1;
}

/**
* @brief get consecutive number without recurse, not recommend use it as it is 
* complex and do more operator than recurse.
*
* @param current_index current index.
* @param sum current summation.
* @param current_count current count.
* @param prepower prepower just use the first power to limit cycle times.
* @param r PRIME_SUM_RET struct point.
*/
void get_consecutive_number_without_recursive(uint64_t current_index, 
	uint64_t sum, uint64_t current_count, uint64_t prepower, 
	PRIME_SUM_RET* r) {
	int k = PRIMES_COUNT - 1;
	while (k >= 0) {
		if (!calculate_number(r, sum, current_count)) ;
		k = PRIMES_COUNT - 1;
		while (k >= 0) {
			if ((k == 0 && r->m_power[k] < prepower) 
				|| (k > 0 && r->m_power[k] < r->m_power[k-1] 
					&& r->m_power[k] <= r->m_primes_max_count[k])) {
				r->m_power[k] ++;
				if (sum <= MAX_UINT64_T/r->m_primes[k]) {
					sum *= r->m_primes[k];
					current_count /= (r->m_power[k]);
					current_count *= (r->m_power[k]+1);
				} else {
					if (r->m_power[k] > 1) {
						sum /= (uint64_t)pow(r->m_primes[k], r->m_power[k]-1);
						current_count /= (r->m_power[k]);
					}
					r->m_power[k] = 0;
					k --;
					continue;
				}
				break;
			} else {
				if (r->m_power[k] > 0) {
					sum /= (uint64_t)pow(r->m_primes[k], r->m_power[k]);
					current_count /= (r->m_power[k]+1);
				}
				r->m_power[k] = 0;
				k --;
			}
		}
		if (k < 0) break;
	}
}

int main(int argc, const char *argv[])
{
///		// This is find the consecutive number summation.
/// 	uint64_t x = 1;
/// 	// for (x = 15679689456006530955U; 
///			x < 15679689456006530956U/*MAX_UINT64_T*/; x ++) {
/// 	for (x = 0x000000000d021fd1; 
///			x < 0x000000000d021fd2/*MAX_UINT64_T*/; x ++) {
/// 		// uint64_t count = 
/// 		// 	print_all_consecutive_number_summation(x);
/// 		// uint64_t count = 
/// 		// 	print_all_consecutive_number_summation_use_equation(x);
/// 		uint64_t count = 
/// 			print_all_consecutive_number_summation_use_divide(x);
/// 		if (0 == count) print_number(x);
/// 	}
	
	// Find the minimum number has most kind summation of consecutive in 64bit.
	PRIME_SUM_RET* r = ret_init();
	get_consecutive_number(0U, 1U, 1U, 41U, r);
	// get_consecutive_number_without_recursive(0U, 1U, 1U, 41U, r);
	ret_print(r);
	ret_release(r);
	return 0;
}
