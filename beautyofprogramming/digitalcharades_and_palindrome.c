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
int has_repeat_digial(const int* digitals, const int size) {
	int* bits = (int*) malloc(sizeof(int) * size);
	memset(bits, 0, sizeof(int) * size);
	int i = 0;
	for (i = 0; i < size; i ++) {
		if (bits[g_possibledigitals[i][digitals[i]] - 1] > 0) {
			free(bits);
			return 1;
		}
		bits[g_possibledigitals[i][digitals[i]] - 1] = 1;
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
void output_digitalcharade(const int* digitals, const int size) {
	int i = 0;
	for (i = 0; i < size; i ++) {
		printf("%d", g_possibledigitals[i][digitals[i]]);
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
	if (has_repeat_digial(digitals, size)) { return 0; }
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
			output_digitalcharade(digitals, size);
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

int main(int argc, const char *argv[])
{
	testcase_digitalcharades();
	return 0;
}
