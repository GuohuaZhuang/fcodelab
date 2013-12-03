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
* @file readprogram_findthenumber.c
* @brief find the number just for read a program.
* @author firstboy0513
* @version 0.0.1
* @date 2013-12-03
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#ifndef MAX_INT32_T
#define MAX_INT32_T (0x7fffffff)
#endif // MAX_INT32_T

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
* @brief find the number can be evenly divisible by rg[k] and rg[k+1] but 
* not divisible for every other element in rg.
* the k is 0 <= k <= n-1.
* Analysis it:
* Every number can be split mulitplied by prime number.
* as rg element is lower 31, we can give all primes:
* 	2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31
* and then mulitplied all element:
* 2 = 2^1
* 3 = 3^1
* 4 = 2^2
* 5 = 5^1
* 6 = 2*3
* 7 = 7^1
* 8 = 2^3
* 9 = 3^2
* 10 = 2*5
* 11 = 11^1
* 12 = 2^2*3
* 13 = 13^1
* 14 = 2*7
* 15 = 3*5
* 16 = 2^4
* 17 = 17^1
* 18 = 2*3^2
* 19 = 19^1
* 20 = 2^2*5
* 21 = 3*7
* 22 = 2*11
* 23 = 23^1
* 24 = 2^3*3
* 25 = 5^2
* 26 = 2*13
* 27 = 3^3
* 28 = 2^2*7
* 29 = 29^1
* 30 = 2*3*5
* 31 = 31^1
* we cat the only max power as 16 has 2^4, 27 has 3^3, 25 has 5^2, 17 has 17^1,
* 19 has 19^1, 29 has 29^1, 31 has 31, the we find only 16 an 17 neighbors.
* so we find 2^3 and 17^0 not to be modulo, the we have this:
* min number = 2^3*3^3*5^2*7*11*13*19*23*29*31 = 2123581660200
* so the 2123581660200 is the min number.
*
*/
void find_the_number() {
	int64_t rg[] = {2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,
		18,19,20,21,22,23,24,25,26,27,28,29,30,31};
	int n_rg_len = sizeof(rg)/sizeof(rg[0]);

	int64_t i = 0;
	for (i = 1; i < MAX_INT64_T; i ++) {
		int hit = 0;
		int hit1 = -1;
		int hit2 = -1;
		int j = 0;
		for (j = 0; (j < n_rg_len) && (hit <= 2); j ++) {
			if ((i % rg[j]) != 0) {
				hit ++;
				if (hit == 1) {
					hit1 = j;
				} else if (hit == 2) {
					hit2 = j;
				} else {
					break;
				}
			}
		}
		if ((hit == 2) && (hit1 + 1 == hit2)) {
			printf("found %"PRIdS"\n", i);
		}
	}
}

int main(int argc, const char *argv[])
{
	find_the_number();
	return 0;
}
