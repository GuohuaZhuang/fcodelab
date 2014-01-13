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
* @file minesweeper.c
* @brief minesweeper calculate mine probability.
* @author firstboy0513
* @version 0.0.1
* @date 2014-01-13
*/
#include <stdio.h>
#include <stdlib.h>

/**
* @brief output mine probability in the minesweeper game.
* The problem is calculate probability of the A, B and C cell:
*   o A o
*   o 1 o
*   o B o
*   o 2 o
*   o C o
* We can use find the total count to find the probability. We can know the mine
* count in it will be 2 or 3, not any other possible count.
* Suppose the total cell count is M, total mine count is N. The area cell count
* is 3*5 == 15.
* If mine count is 2, the number of times has (M-15, N-2)*(3, 1)*(5, 1).
*   For A: 0 times.
*   For B: (M-15, N-2)*1*(5, 1) times.
*   For C: (M-15, N-2)*(3, 1)*1 times.
* If mine count is 3, the number of times has (M-15, N-3)*(5, 1)*(5, 2).
*   For A: (M-15, N-3)*1*(5, 2) times.
*   For B: 0 times.
*   For C: (M-15, N-3)*(5, 1)*1*(4, 1) times.
* So the P(a), P(b) and P(c) probability is:
* 	Set denominator D = (M-15, N-2)*(3, 1)*(5, 1) + (M-15, N-3)*(5, 1)*(5, 2).
* 	P(a) = [ 0 + (M-15, N-3)*1*(5, 2) ] / [ D ];
* 	P(b) = [ (M-15, N-2)*1*(5, 1) + 0 ] / [ D ];
* 	P(a) = [ (M-15, N-2)*(3, 1)*1 + (M-15, N-3)*(5, 1)*1*(4, 1) ] / [ D ].
* And we simplify them as:
* 	P(a) = (2*N-4) / (3*M+7*N-56);
* 	P(b) = (M-N-12) / (3*M+7*N-56);
* 	P(c) = 1/5 + P(a).
*
* @param cell_count total cell count.
* @param mine_count mine count.
*/
void output_mine_probability(int cell_count, int mine_count) {
	double pa = 0.0, pb = 0.0, pc = 0.0;
	pa = (2.0*mine_count-4.0) / (3*cell_count+7*mine_count-56);
	pb = (cell_count-mine_count-12.0) / (3*cell_count+7*mine_count-56);
	pc = (1.0/5) + pa;
	printf("P(a) = %g\n", pa);
	printf("P(b) = %g\n", pb);
	printf("P(c) = %g\n", pc);
}

int main(int argc, const char *argv[])
{
	output_mine_probability(16*16, 40);

	return 0;
}
