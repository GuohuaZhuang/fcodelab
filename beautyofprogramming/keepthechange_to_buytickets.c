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
* @file keepthechange_to_buytickets.c
* @brief solve the keep the change to buy tickets order count problem.
* @author firstboy0513
* @version 0.0.1
* @date 2013-12-27
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
* @brief catalan number.
*
* @param N given input n.
* @param cache store catalan value array.
*
* @return catalan value.
*/
int catalan(int N, int* cache) {
	if (0 == N) { return 1; }
	if (cache[N-1] > 0) { return cache[N-1]; }
	int i = 0, sum = 0, n = N/2;
	for (i = 0; i < n; i ++) {
		sum += catalan(2*i, cache) * catalan(2*(n-i-1), cache);
	}
	cache[N-1] = sum;
	return sum;
}

/**
* @brief catalen number use recurrence.
*
* @param n given input Cn n, not the f(2n).
* @param cache store catalan value array.
*
* @return catalan value.
*/
int catalan_recurrence(int n, int* cache) {
	if (0 == n) { cache[0] = 1; return 1; }
	int i = 0, sum = 0;
	for (i = 0; i < n; i ++) {
		sum += cache[i] * cache[(n-i-1)];
	}
	cache[n] = sum;
	return sum;
}

/**
* @brief test case for catalan.
*/
void testcase_catalan() {
	int N = 19;
	int* cache = (int*) malloc(sizeof(int) * N * 2);
	memset(cache, 0, sizeof(int) * N * 2);
	printf("There are %d order to buy tickets when N = %d.\n", 
		catalan(2*N, cache), N);
	free(cache);

	N = 19;
	int* cache2 = (int*) malloc(sizeof(int) * N * 2);
	memset(cache2, 0, sizeof(int) * N * 2);
	int i = 0;
	for (i = 0; i <= N; i ++) {
		printf("There are %d order to buy tickets when N = %d.\n", 
			catalan_recurrence(i, cache2), i);
	}
	free(cache2);
}

int main(int argc, const char *argv[])
{
	testcase_catalan();

	return 0;
}
