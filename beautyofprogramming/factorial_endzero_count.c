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
* @file factorial_endzero_count.c
* @brief factorial end zero count and factorial first end 1 position and 
* calculate the powers in sums of 2.
* @author firstboy0513
* @version 0.0.1
* @date 2013-12-14
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/**
* @brief calculate the count of end zero in the given N! value.
* We found end zero is equal to find the pair of 2*5, also 2 times is more than
* 5, so we just calculate 5 times.
* This is a intuition method to find wether ever number from 1 to N is module 5
* to zero.
*
* @param N factorial number N.
*
* @return the count of end zero in N!.
*/
int factorial_endzero_count(int N) {
	int i = 0, j = 0, count = 0;
	for (i = 1; i < N; i ++) {
		j = i;
		while (0 == j % 5) {
			count ++;
			j /= 5;
		}
	}
	return count;
}

/**
* @brief calculate the count of end zero in the given N! value.
* We found end zero is equal to find the pair of 2*5, also 2 times is more than
* 5, so we just calculate 5 times.
* It use divide to calculate.
*
* @param N factorial number N.
*
* @return the count of end zero in N!.
*/
int factorial_endzero_count_use_divide(int N) {
	int count = 0;
	while (N) {
		N /= 5;
		count += N;
	}
	return count;
}

/**
* @brief calculate the first end 1 position in the given N! value.
*
* @param N factorial number N.
*
* @return the first end 1 position in the given N! value, it is the index from
* 0 in the end number.
*/
int factorial_first_end1_position(int N) {
	int count = 0;
	while (N) {
		N >>= 1;
		count += N;
	}
	return count;
}

/**
* @brief count 1 times.
* It use for factorial_first_end1_position_use_count1.
*
* @param N a given number N.
*
* @return 1 times or say 1 count.
*/
int count_1_times(int N) {
	int count = 0;
	while (N) {
		N &= (N-1);
		count ++;
	}
	return count;
}

/**
* @brief calculate the first end 1 position in the given N! value.
*
* @param N factorial number N.
*
* @return the first end 1 position in the given N! value, it is the index from
* 0 in the end number.
*/
int factorial_first_end1_position_use_count1(int N) {
	return (N - count_1_times(N));
}

/**
* @brief judge whether a given N is the powers in sums of 2.
*
* @param N a given N to judge.
*
* @return return 1 means true, otherwise return 0 means not the powers in sums.
*/
int judge_powers_in_sums_of_2(int N) {
	return (N > 0 && (0 == (N & (N-1))) );
}

int main(int argc, const char *argv[])
{
	// factorial endzero count test case
	int N = 1024;
	printf("factorial_endzero_count = %d\n", factorial_endzero_count(N));
	printf("factorial_endzero_count_use_divide = %d\n", 
		factorial_endzero_count_use_divide(N));

	// factorial first end1 position test case
	printf("factorial_first_end1_position = %d\n", 
		factorial_first_end1_position(N));
	printf("factorial_first_end1_position_use_count1 = %d\n", 
		factorial_first_end1_position_use_count1(N));

	// judge powers in sums of 2 test case
	printf("judge_powers_in_sums_of_2 = %d\n", judge_powers_in_sums_of_2(N));

	return 0;
}
