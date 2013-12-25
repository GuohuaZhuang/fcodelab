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
* @file digitalcharades_and_palindrome.c
* @brief digital charades and palindrome problem.
* @author firstboy0513
* @version 0.0.1
* @date 2013-12-24
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/**
* @brief global possible digitals.
*/
int g_possibledigitals[][9] = {
	{1, 3, 7, 9}, 
	{4, 8}, 
	{1, 3, 7, 9}, 
	{2, 6}, 
	{5}, 
	{4, 8}, 
	{1, 3, 7, 9}, 
	{2, 6}, 
	{1, 3, 7, 9}
};

/**
* @brief global possible numbers of digitals.
*/
int g_possiblenumbers[9] = {4, 2, 4, 2, 1, 2, 4, 2, 4};

/**
* @brief judge whether the digital has the repeat number.
*
* @param digitals digitals.
* @param size digitals size.
*
* @return return 1 means has the repeat numbers, otherwise return 0 means none
* repeat number, all numbers are unique.
*/
int has_repeat_digial(int possibles[][9], const int* digitals, const int size) {
	int* bits = (int*) malloc(sizeof(int) * 10);
	memset(bits, 0, sizeof(int) * 10);
	int i = 0;
	for (i = 0; i < size; i ++) {
		if (bits[possibles[i][digitals[i]]] > 0) {
			free(bits);
			return 1;
		}
		bits[possibles[i][digitals[i]]] = 1;
	}
	free(bits);
	return 0;
}

/**
* @brief output digital charades.
*
* @param digitals digitals.
* @param size digitals size.
*/
void output_result(int possibles[][9], 
	const int* digitals, const int size) {
	int i = 0;
	for (i = 0; i < size; i ++) {
		printf("%d", possibles[i][digitals[i]]);
	}
	printf("\n");
}

/**
* @brief digital charade varification whether is ok.
*
* @param digitals digitals.
* @param size digitals size.
*
* @return return 1 if success, otherwise return 0 means invalid charades.
*/
int digitalcharade_varification(const int* digitals, const int size) {
	if (has_repeat_digial(g_possibledigitals, digitals, size)) { return 0; }
	int num = 0, i = 0;
	for (i = 0; i < size; i ++) {
		num = (num * 10 + g_possibledigitals[i][digitals[i]]);
		if (0 != num % (i+1)) {
			return 0;
		}
	}
	return 1;
}

/**
* @brief test case for digital charades.
*/
void testcase_digitalcharades() {
	int digitals[9] = {0};
	int size = sizeof(digitals) / sizeof(digitals[0]);
	int k = 0;
	while (k < size) {
		if (digitalcharade_varification(digitals, size)) {
			output_result(g_possibledigitals, digitals, size);
		}
		k = 0;
		while (k < size) {
			if (digitals[k] < g_possiblenumbers[k]-1) {
				digitals[k] ++; break;
			}
			digitals[k] = 0; k ++;
		}
	}
}

/**
* @brief global possible palindromes.
*/
int g_possiblepalindromes[6][9] = {
	{1, 2, 3, 4, 5, 6, 7, 8, 9}, 
	{1, 2, 3, 4, 5, 6, 7, 8, 9}, 
	{1, 2, 3, 4, 5, 6, 7, 8, 9}, 
	{1, 2, 3, 4, 5, 6, 7, 8, 9}, 
	{1, 2, 3, 4, 5, 6, 7, 8, 9}, 
	{2, 3, 4, 5, 6, 7, 8, 9}
};

/**
* @brief global possible numbers of palindromes.
*/
int g_palindromenumbers[6] = {9, 9, 9, 9, 9, 8};

/**
* @brief digital palindrome varification whether is ok.
*
* @param digitals digitals.
* @param size digitals size.
*
* @return return 1 if success, otherwise return 0 means invalid palindromes.
*/
int palindrome_varification(const int* digitals, const int size) {
	if (has_repeat_digial(g_possiblepalindromes, digitals, size)) { return 0; }
	int num = 0, i = 0, value = 0;
	for (i = 0; i < size-1; i ++) {
		num = (num * 10 + g_possiblepalindromes[i][digitals[i]]);
		value = (value * 10 
			+ g_possiblepalindromes[size-2-i][digitals[size-2-i]]);
	}
	if (num * g_possiblepalindromes[size-1][digitals[size-1]] 
		== value) {
		return 1;
	}
	return 0;
}

/**
* @brief output digital charades.
*
* @param digitals digitals.
* @param size digitals size.
*/
void output_result_palindrom(int possibles[][9], 
	const int* digitals, const int size) {
	int i = 0;
	for (i = 0; i < size-1; i ++) {
		printf("%d", possibles[i][digitals[i]]);
	}
	printf(" * %d = ", possibles[size-1][digitals[size-1]]);
	for (i = size-2; i >= 0; i --) {
		printf("%d", possibles[i][digitals[i]]);
	}
	printf("\n");
}

/**
* @brief test case for palindromes.
* The problem is to find the digital as "人过大佛寺*我 == 寺佛大过人" palindrom.
* Every Chinese character represent a digital from 1 to 9.
*/
void testcase_palindromes() {
	int digitals[6] = {0};
	int size = sizeof(digitals) / sizeof(digitals[0]);
	int k = 0;
	while (k < size) {
		if (palindrome_varification(digitals, size)) {
			output_result_palindrom(g_possiblepalindromes, digitals, size);
		}
		k = 0;
		while (k < size) {
			if (digitals[k] < g_palindromenumbers[k]-1) {
				digitals[k] ++; break;
			}
			digitals[k] = 0; k ++;
		}
	}
}

/**
* @brief get palindrome count with given digital bit count.
*
* @param bit_count digital bit count.
*
* @return palindrome count.
*/
int palindrome_count(int bit_count) {
	return (int)(9*pow(10, bit_count/2+(bit_count%2-1)));
}

/**
* @brief test case for palindrome count.
*/
void testcase_palindrome_count() {
	int i = 0;
	for (i = 2; i < 10; i ++) {
		printf("%d bit count have %d palindrome count.\n", i, 
			palindrome_count(i));
	}
}

#define NUM_CHAR(x) ( (x < 10) ? (x + '0') : (x - 10 + 'a') )

/**
* @brief output sh3she expression.
*
* @param he he value.
* @param ary ary.
*/
void output_he2she(int he, int ary) {
	int she = he*he;
	printf("he2she ary(%d) is (%c%c)^2 = (%c%c%c)\n", ary, 
		NUM_CHAR(he/ary), NUM_CHAR(he%ary), 
		NUM_CHAR(she/ary/ary), NUM_CHAR(she/ary%ary), NUM_CHAR(she%ary));
}

/**
* @brief output all possible he2she expression in given ary.
*
* @param ary ary.
*/
void he2she(int ary) {
	int max = pow(ary, 3), h = 0, e = 0, he = 0, she = 0, mod = pow(ary, 2);
	for (h = 1; h < ary; h ++) {
		for (e = 0; e < ary; e ++) {
			he = h*ary + e;
			she = he*he;
			if (she >= max) { return; }
			if (she % mod == he) {
				output_he2she(he, ary);
			}
		}
	}
}

/**
* @brief test case he2she expression.
*/
void testcase_he2she() {
	int i = 0;
	for (i = 3; i <= 16; i ++) {
		he2she(i);
	}
}

int main(int argc, const char *argv[])
{
	testcase_digitalcharades();
	testcase_palindromes();
	testcase_palindrome_count();
	testcase_he2she();
	return 0;
}
