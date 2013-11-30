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
* @file phonenumber_word.c
* @brief 
* @author firstboy0513
* @version 0.0.1
* @date 2013-11-30
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
* @brief alpha array that correspond to number.
*/
const char* g_number_correspond_alpha[10] = {
	"", "", "abc", "def", "ghi", "jkl", "mno", "pqrs", "tuv", "wxyz"
};
/**
* @brief alpha count array that correspond to number.
*/
const int g_number_correspond_alpha_count[10] = {
	0, 0, 3, 3, 3, 3, 3, 4, 3, 4
};

/**
* @brief print answer correspond number.
*
* @param number[] number array.
* @param answer answer array point.
* @param n length of number array.
*/
void print_answer_word(const int number[], int* answer, int n) {
	int i = 0;
	for (i = 0; i < n; i ++) {
		if (g_number_correspond_alpha_count[ number[i] ] > 0) {
			printf("%c", g_number_correspond_alpha[ number[i] ][ answer[i] ]);
		} else {
			printf("*");
		}
	}
	printf("\n");
}

/**
* @brief Use recursive to traversal all possible answers correspond number.
*
* @param number[] number array.
* @param answer answer array point.
* @param n length of number array.
* @param index current answer hierarchical recursive index.
*/
void traversal_number_words_use_recursive(const int number[], int* answer, 
	int n, int index) {
	if (index >= n) {
		print_answer_word(number, answer, n);
		return;
	}
	answer[index] = 0;
	do {
		traversal_number_words_use_recursive(number, answer, n, index + 1);
		answer[index] ++;
	} while (answer[index] < g_number_correspond_alpha_count[ number[index] ]);
}

/**
* @brief Use no recursive to traversal all possible answer correspond number.
*
* @param number[] number array.
* @param answer answer array point.
* @param n length of number array.
*/
void traversal_number_words_without_recursive(const int number[], int* answer,
	int n) {
	while (1) {
		print_answer_word(number, answer, n);
		int index = n - 1;
		while (index >= 0) {
			if (answer[index] 
					< g_number_correspond_alpha_count[ number[index] ] - 1) {
				answer[index] ++;
				break;
			}
			answer[index] = 0;
			index --;
		}
		if (index < 0) break;
	}
}

int main(int argc, const char *argv[])
{
	// Prepare test number array.
	const int number[] = {1, 5, 8, 1};
	int n_number_length = sizeof(number) / sizeof(number[0]);
	int* answer = (int*)malloc(sizeof(int) * n_number_length);
	memset(answer, 0, sizeof(int) * n_number_length);

	//traversal_number_words_use_recursive(number, answer, n_number_length, 0);
	traversal_number_words_without_recursive(number, answer, n_number_length);

	free(answer);
	return 0;
}
