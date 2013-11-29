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
* @file string_distance.c
* @brief calculate string distance.
* This distance is use edit distance, the edit include add, delete, modify.
* @author firstboy0513
* @version 0.0.1
* @date 2013-11-29
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
* @brief get minimum number from 3 number.
*
* @param a number a.
* @param b number b.
* @param c number c.
*
* @return minimum number.
*/
int min3(int a, int b, int c) {
	a = (a < b) ? a : b;
	a = (a < c) ? a : c;
	return a;
}

/**
* @brief calculate string distance.
* This algorithm use recursive.
*
* @param sa string A.
* @param paBegin string A begin index position.
* @param paEnd string A end index position.
* @param sb string B.
* @param pbBegin string B begin index position.
* @param pbEnd string B end index position.
*
* @return return distance between string A and string B.
*/
int CalculateStringDistance(const char* sa, const int paBegin, const int paEnd, 
	const char* sb, const int pbBegin, const int pbEnd) {
	if (paBegin > paEnd) {
		if (pbBegin > pbEnd) {
			return 0;
		}
		return pbEnd - pbBegin + 1;
	}
	if (pbBegin > pbEnd) {
		if (paBegin > paEnd) {
			return 0;
		}
		return paEnd - paBegin + 1;
	}

	if (sa[paBegin] == sb[pbBegin]) {
		return CalculateStringDistance(sa, paBegin + 1, paEnd, 
			sb, pbBegin + 1, pbEnd);
	}
	int d1 = CalculateStringDistance(sa, paBegin + 1, paEnd, 
		sb, pbBegin, pbEnd);
	int d2 = CalculateStringDistance(sa, paBegin, paEnd, 
		sb, pbBegin + 1, pbEnd);
	int d3 = CalculateStringDistance(sa, paBegin + 1, paEnd, 
		sb, pbBegin + 1, pbEnd);
	return (min3(d1, d2, d3) + 1);
}

/**
* @brief Calculate string edit distance using DP(Dynamic Programming)
* algorithm.
*
* @param sa string A.
* @param saLen string A length.
* @param sb string B.
* @param sbLen string B length.
*
* @return return distance between string A and string B.
*/
int CalculateStringDistanceUseDP(const char* sa, const int saLen, 
	const char* sb, const int sbLen) {
	if (saLen < 0 || sbLen < 0) {
		printf("[ERR] -- CalculateStringDistanceUseDP input invalid.\n");
		return -1;
	}
	if (saLen == 0) {
		return sbLen;
	}
	if (sbLen == 0) {
		return saLen;
	}
	int i = 0, j = 0;
	int naLen = saLen + 1, nbLen = sbLen + 1;
	int* _dp = (int*) malloc(sizeof(int) * naLen * nbLen);
	int* dp[10];
	for (i = 0; i < naLen; i ++) { dp[i] = (_dp + nbLen * i); }
	for (i = 0; i < naLen; i ++) { dp[i][0] = i; }
	for (j = 0; j < nbLen; j ++) { dp[0][j] = j; }
	for (i = 1; i < naLen; i ++) {
		for (j = 1; j < nbLen; j ++) {
			if (sa[i-1] == sb[j-1]) {
				dp[i][j] = dp[i-1][j-1];
			} else {
				dp[i][j] = min3(dp[i-1][j-1], dp[i-1][j], dp[i][j-1]) + 1;
			}
		}
	}
	int d = dp[sbLen][saLen];
	free(_dp);
	return d;
}

int main(int argc, const char *argv[])
{
	const char* sa = "abcdef";
	const char* sb = "acdefa";
	// int d = CalculateStringDistance(sa, 0, (strlen(sa)-1),
	// 	sb, 0, (strlen(sb)-1));
	int d = CalculateStringDistanceUseDP(sa, strlen(sa), sb, strlen(sb));
	printf("string distance between A(%s) and B(%s) is %d\n", 
		sa, sb, d);
	return 0;
}
