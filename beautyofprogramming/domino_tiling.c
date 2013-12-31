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
* @file domino_tiling.c
* @brief domino tiling ways.
* references: 
* 	http://en.wikipedia.org/wiki/domino_tiling
* 	http://bbs.csdn.net/topics/340145482
* @author firstboy0513
* @version 0.0.1
* @date 2013-12-31
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#ifndef PI
#define PI 3.1415926
#endif // PI

/**
* @brief get domino tiling number ways use formula.
* It good in time complex, but not accurately.
*
* @param m the m*n floor board.
* @param n the m*n floor board.
*
* @return the number of ways to cover 1*2 tiling on m*n floor board.
*/
int domino_tiling_ways_formula(int m, int n) {
	int j = 0, k = 0;
	double result = 1;
	for (j = 1; j <= m/2; j ++) {
		for (k = 1; k <= n/2; k ++) {
			result *= 4*pow(cos(PI*j/(m+1)), 2) + 4*pow(cos(PI*k/(n+1)), 2);
		}
	}
	return result;
}

/**
* @brief fill the element type of m*n board.
*/
typedef enum { BLANK_FILL, NEIBOR_FILL, TOP_FILL } FILLTYPE;

/**
* @brief init fill type for dynamic programming method.
* The fill type array element is 0, 1, 2, means blank, neibor, top fill type.
*
* @param n n.
*
* @return return filltypes array point.
*/
FILLTYPE* init_filltypes(int n) {
	FILLTYPE* filltypes = (FILLTYPE*) malloc(sizeof(FILLTYPE) * n);
	memset(filltypes, 0, sizeof(FILLTYPE) * n);
	return filltypes;
}

/**
* @brief release filltypes.
*
* @param filltypes filltypes array point.
*/
void release_filltypes(FILLTYPE* filltypes) {
	if (filltypes) { free(filltypes); }
}

/**
* @brief init board 3-dimension array.
*
* @param m m*n board.
* @param n m*n board.
*
* @return board array point.
*/
int*** init_board(int m, int n) {
	int i = 0, j = 0, linerange = pow(2, n);
	int*** board = (int***) malloc(sizeof(int**) * n);
	for (i = 0; i < n; i ++) {
		board[i] = (int**) malloc(sizeof(int*) * m);
		for (j = 0; j < m; j ++) {
			board[i][j] = (int*) malloc(sizeof(int) * linerange);
			memset(board[i][j], 0, sizeof(int) * linerange);
		}
	}
	return board;
}

/**
* @brief release board memory.
*
* @param m m*n board.
* @param n m*n board.
* @param board board array point.
*/
void release_board(int m, int n, int*** board) {
	int i = 0, j = 0;
	for (i = 0; i < n; i ++) {
		for (j = 0; j < m; j ++) {
			free(board[i][j]);
		}
		free(board[i]);
	}
	free(board);
}

/**
* @brief dynamic programming to calculate tiling ways.
* Every fill type has 3 ways, there are blank fill for the next line, or fill
* by neibor, or fill by the top line element. So we name them BLANK_FILL, 
* NEIBOR_FILL and TOP_FILL. them use dp method to solve it. The current number 
* ways of tiling cover is the summarize recursion. As we create the dp array 
* a 3-dimension array, which is dp[n][m][xxx], xxx is means from 0~2^n-1, image
* it as a column, and m is the line, horizon x-axis.
*
* @param filltypes filltypes array.
* @param board board array.
* @param m m*n floor board.
* @param n m*n floor board.
*/
void dp_tilingways(FILLTYPE* filltypes, int*** board, int m, int n, int step) {
	if (step >= n) {
		int abovelinevalue = 0, currlinevalue = 0, i = 0;
		while (i < n) {
			abovelinevalue <<= 1; currlinevalue <<= 1;
			switch (filltypes[i]) {
				case BLANK_FILL:
					abovelinevalue |= 0x01; break;
				case NEIBOR_FILL:
					abovelinevalue |= 0x01; currlinevalue |= 0x01; break;
				case TOP_FILL:
					currlinevalue |= 0x01; break;
			}
			i ++;
		}
		if (m > 1) {
			board[n+(-1)][m+(-1)][currlinevalue] 
				+= board[n+(-1)][m-1+(-1)][abovelinevalue];
		} else {
			board[n+(-1)][m+(-1)][currlinevalue] = 1;
		}
		return;
	}
	filltypes[step] = BLANK_FILL;
	dp_tilingways(filltypes, board, m, n, step+1);
	if (step+1 < n) {
		filltypes[step] = filltypes[step+1] = NEIBOR_FILL;
		dp_tilingways(filltypes, board, m, n, step+2);
	}
	if (m > 1) {
		filltypes[step] = TOP_FILL;
		dp_tilingways(filltypes, board, m, n, step+1);
	}
}

/**
* @brief get domino tiling number ways use dynamic programming method.
* It use dynamic programming, and accurately.
*
* @param m the m*n floor board.
* @param n the m*n floor board.
*
* @return the number of ways to cover 1*2 tiling on m*n floor board.
*/
int domino_tiling_ways_dynamicprogramming(int m, int n) {
	if (m <= 0 || n <= 0) { return 0; }
	int i = 0, j = 0, linerange = pow(2, n);
	FILLTYPE* filltypes = init_filltypes(n);
	int*** board = init_board(m, n);
	for (i = 1; i <= m; i ++) {
		for (j = 1; j <= n; j ++) {
			dp_tilingways(filltypes, board, i, j, 0);
		}
	}
	int ways = board[n+(-1)][m+(-1)][linerange-1];
	release_board(m, n, board);
	release_filltypes(filltypes);
	return ways;
}

int main(int argc, const char *argv[])
{
	int m = 0, n = 0;
	printf("input m = "); scanf("%d", &m);
	printf("input n = "); scanf("%d", &n);
	// double result = domino_tiling_ways_formula(m, n);
	double result = domino_tiling_ways_dynamicprogramming(m, n);
	printf("%d * %d tiling has %g number or ways to cover.\n", m, n, result);
	return 0;
}
