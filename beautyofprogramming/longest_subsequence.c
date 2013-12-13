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
* @file longest_subsequence.c
* @brief find the longest sequence length.
* There is another method was to say more optimize than the dynamic programming
* method, but I found it's not update the MaxV[j] value in some time, it is a 
* bug, so I do not implement it in here.
* @author firstboy0513
* @version 0.0.1
* @date 2013-12-13
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/**
* @brief find the longest sequence length.
* It use dynamic programming method.
*
* @param array array.
* @param length array length.
*
* @return  longest sequence length or return -1 means error occur.
*/
int array_longest_sequence_use_dp(const int* array, const int length) {
	if (!array || length <= 0) {
		printf("array_longest_sequence input invalid!\n");
		return -1;
	}
	int i = 0, j = 0, global_longest = 1;
	int* longests = (int*) malloc(sizeof(int) * length);
	longests[0] = 1;
	for (i = 1; i < length; i ++) {
		longests[i] = 1;
		for (j = i - 1; j >= 0; j --) {
			if (array[i] > array[j] && longests[j] + 1 > longests[i]) {
				longests[i] = longests[j] + 1;
			}
		}
		if (longests[i] > global_longest) {
			global_longest = longests[i];
		}
	}
	free(longests);
	return global_longest;
}

int main(int argc, const char *argv[])
{
	const int array[] = {1, -1, 2, -3, 4, -5, 6, -7};
	const int length = sizeof(array) / sizeof(array[0]);
	int longest_length = array_longest_sequence_use_dp(array, length);
	printf("longest_length = %d\n", longest_length);
	return 0;
}
