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
* @file drinks_supply_problem.c
* @brief drinks supply best satisfaction problem.
* @author firstboy0513
* @version 0.0.1
* @date 2014-01-07
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define N 5

/**
* @brief output array.
*
* @param X[] array X.
* @param size array size.
*/
void output_array(int X[], int size) {
	int i = 0;
	for (i = 0; i < size; i ++) {
		if (0 != i) { printf(", "); }
		printf("%d", X[i]);
	}
	printf("\n");
}

/**
* @brief initialtion optimal matrix.
*
* @param totalV total volume.
* @param T all types count.
*
* @return return optimal matrix.
*/
int** init_opt(int totalV, int T) {
	int** opt = (int**) malloc(sizeof(int*) * (totalV+1));
	int i = 0, j = 0;
	for (i = 0; i <= totalV; i ++) {
		opt[i] = (int*) malloc(sizeof(int) * (T+1));
		for (j = 0; j <= T; j ++) {
			opt[i][j] = INT32_MIN;
		}
	}
	for (i = 0; i <= T; i ++) {
		opt[0][i] = 0;
	}
	return opt;
}

/**
* @brief release optimal matrix.
*
* @param opt optimal matrix.
* @param totalV total volume.
*/
void release_opt(int** opt, int totalV) {
	int i = 0;
	for (i = 0; i <= totalV; i ++) {
		free(opt[i]);
	}
	free(opt);
}

/**
* @brief get best satisfaction solution of optimal matrix.
*
* @param V volume array of different type of drinks.
* @param C constraints of drinks.
* @param H satisfactions of drinks.
* @param B the actual purchase amount of drinks.
* @param size size of drinks.
* @param totalV total volume.
*
* @return return best satisfaction, but not any actual purchase amount result.
*/
int best_satisfaction_solution(int* V, int* C, int* H, int* B, int size, 
	int totalV) {
	int** opt = init_opt(totalV, size);
	int v = 0, i = 0, k = 0;
	for (v = 0; v <= totalV; v ++) {
		for (i = size-1; i >= 0; i --) {
			for (k = 0; k <= C[i]; k ++) {
				if (v < V[i]*k) { break; }
				int x = opt[v-V[i]*k][i+1];
				if (INT32_MIN != x) {
					x += H[i]*k;
					if (x > opt[v][i]) {
						opt[v][i] = x;
					}
				}
			}
		}
	}

	int best_satisfaction = opt[totalV][0];
	release_opt(opt, totalV);
	return best_satisfaction;
}

/**
* @brief get best satisfaction solution of optimal matrix recursion sub method.
*
* @param V volume array of different type of drinks.
* @param C constraints of drinks.
* @param H satisfactions of drinks.
* @param B the actual purchase amount of drinks.
* @param size size of drinks.
* @param totalV total volume.
*
* @return return best satisfaction, but not any actual purchase amount result.
*/
int best_satisfaction_solution_recursion(int* V, int* C, int* H, int* B, 
	int size, int totalV, int** opt, int v, int t) {
	if (t == size) {
		if (v == 0) { return 0; }
		else { return INT32_MIN; }
	}
	if (v < 0) {
		return INT32_MIN;
	} else if (v == 0) {
		return 0;
	} else if (opt[v][t] != INT32_MIN) {
		return opt[v][t];
	}
	int ret = INT32_MIN;
	int i = 0;
	for (i = 0; i <= C[t]; i ++) {
		int temp = best_satisfaction_solution_recursion(V, C, H, B, size, 
			totalV, opt, v-V[t]*i, t+1);
		if (INT32_MIN != temp) {
			temp += H[t]*i;
			if (ret < temp) { ret = temp; }
		}
	}
	opt[v][t] = ret;
	return ret;
}

/**
* @brief get best satisfaction solution of optimal matrix use recursion.
*
* @param V volume array of different type of drinks.
* @param C constraints of drinks.
* @param H satisfactions of drinks.
* @param B the actual purchase amount of drinks.
* @param size size of drinks.
* @param totalV total volume.
*
* @return return best satisfaction, but not any actual purchase amount result.
*/
int best_satisfaction_solution_use_recursion(int* V, int* C, int* H, int* B, 
	int size, int totalV) {
	int** opt = init_opt(totalV, size);
	best_satisfaction_solution_recursion(V, C, H, B, size, totalV, opt, totalV,
		0);
	int best_satisfaction = opt[totalV][0];
	release_opt(opt, totalV);
	return best_satisfaction;
}

int main(int argc, const char *argv[])
{
	int V[N] = {2, 8, 32, 8, 64};
	int C[N] = {64, 64, 128, 128, 128};
	int H[N] = {1, 2, 4, 3, 5};
	int B[N] = {0, 0, 0, 0, 0};
	int totalV = 256;

	// int satisfaction = best_satisfaction_solution(V, C, H, B, N, totalV);
	int satisfaction = best_satisfaction_solution_use_recursion(
		V, C, H, B, N, totalV);
	printf("The best satisfaction solution is: ");
	output_array(B, N);
	printf("And its satisfaction = %d\n", satisfaction);

	return 0;
}
