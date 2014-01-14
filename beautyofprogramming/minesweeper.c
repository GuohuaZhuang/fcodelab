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
#include <string.h>

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
* Also, if M = 16*16 = 256, further simplify them as:
* 	P(a) = (2*N-4) / (7*N+712);
* 	P(b) = (244-N) / (7*N+712);
* 	P(c) = 1/5 + (2*N-4) / (7*N+712);
* And the curve comes from google:
*
* 	http://www.google.com.hk/#q=y%3D(2*x-4)%2F(7*x%2B712)+and+y%3D(244-x)%2F(7*
* 	x%2B712)+and+y%3D1%2F5%2B(2*x-4)%2F(7*x%2B712)
*
* And When N = 24, P(c) == P(b).
*
* @param cell_count total cell count.
* @param mine_count mine count.
*/
void output_mine_probability(int cell_count, int mine_count) {
	double pa = 0.0, pb = 0.0, pc = 0.0;
	pa = (2.0*mine_count-4.0) / (3*cell_count+7*mine_count-56);
	pb = (cell_count-mine_count-12.0) / (3*cell_count+7*mine_count-56);
	pc = (1.0/5) + pa;
	printf("cell count %d, mine count %d: ", cell_count, mine_count);
	printf("P(a) = %g, ", pa);
	printf("P(b) = %g, ", pb);
	printf("P(c) = %g\n", pc);
}

/**
* @brief test case for abc mine probability.
*/
void testcase_for_abc_mine_probability() {
	output_mine_probability(16*16, 40);

	int i = 0;
	for (i = 10; i <= 240; i ++) {
		output_mine_probability(16*16, i);
	}
}

#define PANEL_WIDTH 16
#define PANEL_HEIGHT 16

/**
* @brief -1 means uncover, otherwise positive number means covered, and 9 means 
* mine.
*/
typedef int CELL;

/**
* @brief output panel.
*
* @param panel[PANEL_HEIGHT][PANEL_WIDTH] panel.
*/
void output_panel(CELL panel[PANEL_HEIGHT][PANEL_WIDTH]) {
	int i = 0, j = 0;
	printf("___________________________________\n");
	for (i = 0; i < PANEL_HEIGHT; i ++) {
		printf("| ");
		for (j = 0; j < PANEL_WIDTH; j ++) {
			if (panel[i][j] < 0) {
				printf("o ");
			} else if (panel[i][j] == 0) {
				printf("  ");
			} else if (panel[i][j] == 9) {
				printf("@ ");
			} else {
				printf("%d ", panel[i][j]);
			}
		}
		printf("|\n");
	}
	printf("-----------------------------------\n");
}

/**
* @brief pretreat panel to find the direct mine cell and no mine cell.
*
* @param panel[PANEL_HEIGHT][PANEL_WIDTH]
*/
void pretreat_panel(CELL panel[PANEL_HEIGHT][PANEL_WIDTH]) {
	// you can do nothing here, or you can prune to subtract complex.
	return;
}

/**
* @brief cell in panel position.
*/
typedef struct _POS {
	int x;
	int y;
} POS;

/**
* @brief judge 8cell has cover cell or not.
*
* @param panel[PANEL_HEIGHT][PANEL_WIDTH] panel.
* @param i position x.
* @param j position y.
*
* @return return 1 means has 8 cell covered cell, so it is a nearby cell, 
* otherwise return 0 means is not a nearby cell.
*/
int has_8cell_cover(CELL panel[PANEL_HEIGHT][PANEL_WIDTH], int i, int j) {

	if (i > 0 && j > 0 && panel[i-1][j-1] >= 0 && panel[i-1][j-1] != 9) {
		return 1;
	}
	if (i > 0 && panel[i-1][j] >= 0 && panel[i-1][j] != 9) {
		return 1;
	}
	if (i > 0 && j+1 < PANEL_WIDTH && panel[i-1][j+1] >= 0 
		&& panel[i-1][j+1] != 9) {
		return 1;
	}
	if (j > 0 && panel[i][j-1] >= 0 && panel[i][j-1] != 9) {
		return 1;
	}
	if (j+1 < PANEL_WIDTH && panel[i][j+1] >= 0 && panel[i][j+1] != 9) {
		return 1;
	}
	if (i+1 < PANEL_HEIGHT && j > 0 && panel[i+1][j-1] >= 0 
		&& panel[i+1][j-1] != 9) {
		return 1;
	}
	if (i+1 < PANEL_HEIGHT && panel[i+1][j] >= 0 && panel[i+1][j] != 9) {
		return 1;
	}
	if (i+1 < PANEL_HEIGHT && j+1 < PANEL_WIDTH && panel[i+1][j+1] >= 0
		&& panel[i+1][j+1] != 9) {
		return 1;
	}

	return 0;
}

/**
* @brief find nearby cells to judge next, otherwise other cell are not simple to
* cover it, and them are get probability use average.
*
* @param panel[PANEL_HEIGHT][PANEL_WIDTH] panel.
* @param pcount output nearby cells count.
*
* @return nearby cells array, remember free it memory outside.
*/
POS* find_nearby_cells(CELL panel[PANEL_HEIGHT][PANEL_WIDTH], int* pcount) {
	if (pcount == NULL) { return NULL; }
	*pcount = 0;
	POS* nearby = (POS*) malloc(sizeof(POS) * PANEL_HEIGHT*PANEL_WIDTH);
	int i = 0, j = 0;
	for (i = 0; i < PANEL_HEIGHT; i ++) {
		for (j = 0; j < PANEL_WIDTH; j ++) {
			if (panel[i][j] < 0 && has_8cell_cover(panel, i, j)) {
				nearby[(*pcount) ++] = (POS){i, j};
			}
		}
	}
	return nearby;
}

/**
* @brief find nearby cells to judge next, otherwise other cell are not simple to
* cover it, and them are get probability use average.
*
* @param panel[PANEL_HEIGHT][PANEL_WIDTH] panel.
* @param nearbys[PANEL_HEIGHT][PANEL_WIDTH] nearbys array.
*/
void find_nearbys_cells(CELL panel[PANEL_HEIGHT][PANEL_WIDTH], 
	CELL nearbys[PANEL_HEIGHT][PANEL_WIDTH]) {
	int i = 0, j = 0;
	for (i = 0; i < PANEL_HEIGHT; i ++) {
		for (j = 0; j < PANEL_WIDTH; j ++) {
			if (panel[i][j] < 0 && has_8cell_cover(panel, i, j)) {
				nearbys[i][j] = 1;
			} else {
				nearbys[i][j] = 0;
			}
		}
	}
}

// OK
void set_panel(CELL tmppanel[PANEL_HEIGHT][PANEL_WIDTH], POS cell, int value) {
	// printf("set panel[%d][%d] = %d\n", cell.x, cell.y, value);
	tmppanel[cell.x][cell.y] = value;
}

// OK
POS* get_number_cells(CELL panel[PANEL_HEIGHT][PANEL_WIDTH], POS cell, 
	CELL numpanel[PANEL_HEIGHT][PANEL_WIDTH], int* pnumcount) {
	int i = cell.x, j = cell.y;
	*pnumcount = 0;
	POS* num_cells = (POS*) malloc(sizeof(POS) * 8);
	if (i > 0 && j > 0 && panel[i-1][j-1] > 0 && panel[i-1][j-1] != 9) {
		if (!numpanel[i-1][j-1]) {
			num_cells[(*pnumcount) ++] = (POS){i-1, j-1};
			numpanel[i-1][j-1] = 1;
		}
	}
	if (i > 0 && panel[i-1][j] > 0 && panel[i-1][j] != 9) {
		if (!numpanel[i-1][j]) {
			num_cells[(*pnumcount) ++] = (POS){i-1, j};
			numpanel[i-1][j] = 1;
		}
	}
	if (i > 0 && j+1 < PANEL_WIDTH && panel[i-1][j+1] > 0 
		&& panel[i-1][j+1] != 9) {
		if (!numpanel[i-1][j+1]) {
			num_cells[(*pnumcount) ++] = (POS){i-1, j+1};
			numpanel[i-1][j+1] = 1;
		}
	}
	if (j > 0 && panel[i][j-1] > 0 && panel[i][j-1] != 9) {
		if (!numpanel[i][j-1]) {
			num_cells[(*pnumcount) ++] = (POS){i, j-1};
			numpanel[i][j-1] = 1;
		}
	}
	if (j+1 < PANEL_WIDTH && panel[i][j+1] > 0 && panel[i][j+1] != 9) {
		if (!numpanel[i][j+1]) {
			num_cells[(*pnumcount) ++] = (POS){i, j+1};
			numpanel[i][j+1] = 1;
		}
	}
	if (i+1 < PANEL_HEIGHT && j > 0 && panel[i+1][j-1] > 0 
		&& panel[i+1][j-1] != 9) {
		if (!numpanel[i+1][j-1]) {
			num_cells[(*pnumcount) ++] = (POS){i+1, j-1};
			numpanel[i+1][j-1] = 1;
		}
	}
	if (i+1 < PANEL_HEIGHT && panel[i+1][j] > 0 && panel[i+1][j] != 9) {
		if (!numpanel[i+1][j]) {
			num_cells[(*pnumcount) ++] = (POS){i+1, j};
			numpanel[i+1][j] = 1;
		}
	}
	if (i+1 < PANEL_HEIGHT && j+1 < PANEL_WIDTH && panel[i+1][j+1] > 0
		&& panel[i+1][j+1] != 9) {
		if (!numpanel[i+1][j+1]) {
			num_cells[(*pnumcount) ++] = (POS){i+1, j+1};
			numpanel[i+1][j+1] = 1;
		}
	}
	return num_cells;
}

// OK
POS* get_uncovered_cells(CELL panel[PANEL_HEIGHT][PANEL_WIDTH], POS cell,
	int* pcount) {
	*pcount = 0;
	POS* cells = (POS*) malloc(sizeof(POS) * 8);
	int i = 0, j = 0, x = cell.x, y = cell.y;
	for (i = x-1; i <= x+1; i ++) {
		for (j = y-1; j <= y+1; j ++) {
			if (i == x && j == y) { continue; }
			if (i < 0 || i >= PANEL_HEIGHT || j < 0 || j >= PANEL_WIDTH) {
				continue;
			}
			if (-1 == panel[i][j]) { cells[(*pcount) ++] = (POS){i, j}; }
		}
	}
	return cells;
}

// OK
POS* get_covered_mine_cells(CELL panel[PANEL_HEIGHT][PANEL_WIDTH], POS cell,
	int* pcount) {
	*pcount = 0;
	POS* cells = (POS*) malloc(sizeof(POS) * 8);
	int i = 0, j = 0, x = cell.x, y = cell.y;
	for (i = x-1; i <= x+1; i ++) {
		for (j = y-1; j <= y+1; j ++) {
			if (i == x && j == y) { continue; }
			if (i < 0 || i >= PANEL_HEIGHT || j < 0 || j >= PANEL_WIDTH) {
				continue;
			}
			if (9 == panel[i][j]) { cells[(*pcount) ++] = (POS){i, j}; }
		}
	}
	return cells;
}

// statement
int judge_hasinvalid(CELL tmppanel[PANEL_HEIGHT][PANEL_WIDTH], POS cell, 
	CELL mineornot, CELL numpanel[PANEL_HEIGHT][PANEL_WIDTH]);

// statement
int is_8cell_valid(CELL panel[PANEL_HEIGHT][PANEL_WIDTH], POS cell, 
	CELL numpanel_[PANEL_HEIGHT][PANEL_WIDTH]);

int judge_hasvalid(CELL tmppanel[PANEL_HEIGHT][PANEL_WIDTH], POS cell, 
	CELL mineornot, CELL numpanel[PANEL_HEIGHT][PANEL_WIDTH]) {
	int i = 0, numcount = 0, rflag = 1;
	set_panel(tmppanel, cell, mineornot ? 9 : 0);
	POS* num_cells = get_number_cells(tmppanel, cell, numpanel, &numcount);
	for (i = 0; i < numcount; i ++) {
		if (!is_8cell_valid(tmppanel, num_cells[i], numpanel)) {
			rflag = 0; break;
		}
	}
	free(num_cells);
	return rflag;
}

// OK
int is_8cell_valid(CELL panel[PANEL_HEIGHT][PANEL_WIDTH], POS cell, 
	CELL numpanel_[PANEL_HEIGHT][PANEL_WIDTH]) {
	int num = panel[cell.x][cell.y], rflag = 1, i = 0;
	int uncovered_count = 0, coveredmine_count = 0;
	POS* uncovereds = get_uncovered_cells(panel, cell, &uncovered_count);
	POS* coveredmines = get_covered_mine_cells(panel, cell, &coveredmine_count);

	do {
		if (num > coveredmine_count+uncovered_count) {
			rflag = 0; break;
		} else if (num == coveredmine_count) {
			for (i = 0; i < uncovered_count; i ++) {
				CELL numpanel[PANEL_HEIGHT][PANEL_WIDTH] = {{0}};
				CELL tmppanel[PANEL_HEIGHT][PANEL_WIDTH] = {{0}};
				memcpy(tmppanel, panel, sizeof(CELL)*PANEL_HEIGHT*PANEL_WIDTH);
				if (!judge_hasvalid(tmppanel, uncovereds[i], 0, numpanel)) {
					rflag = 0; break;
				}
			}
		} else if (num == uncovered_count + coveredmine_count) {
			for (i = 0; i < uncovered_count; i ++) {
				CELL numpanel[PANEL_HEIGHT][PANEL_WIDTH] = {{0}};
				CELL tmppanel[PANEL_HEIGHT][PANEL_WIDTH] = {{0}};
				memcpy(tmppanel, panel, sizeof(CELL)*PANEL_HEIGHT*PANEL_WIDTH);
				if (!judge_hasvalid(tmppanel, uncovereds[i], 1, numpanel)) {
					rflag = 0; break;
				}
			}
		}
	} while(0);

	free(uncovereds); free(coveredmines); return rflag;
}

// OK
int judge_hasinvalid(CELL tmppanel[PANEL_HEIGHT][PANEL_WIDTH], POS cell, 
	CELL mineornot, CELL numpanel[PANEL_HEIGHT][PANEL_WIDTH]) {
	int i = 0, numcount = 0, rflag = 0;
	set_panel(tmppanel, cell, mineornot ? 9 : 0);
	POS* num_cells = get_number_cells(tmppanel, cell, numpanel, &numcount);
	for (i = 0; i < numcount; i ++) {
		if (!is_8cell_valid(tmppanel, num_cells[i], numpanel)) {
			rflag = 1; break;
		}
	}
	free(num_cells);
	return rflag;
}

// OK
int judge_cell_mine_or_not(CELL panel[PANEL_HEIGHT][PANEL_WIDTH], POS cell, 
	CELL nearbys[PANEL_HEIGHT][PANEL_WIDTH]) {
	int i = 0;
	int valid[2] = {0};	// valid[0] for no mine, valid[1] for has mine
	for (i = 0; i < 2; i ++) {
		CELL numpanel[PANEL_HEIGHT][PANEL_WIDTH] = {{0}};
		CELL tmppanel[PANEL_HEIGHT][PANEL_WIDTH] = {{0}};
		memcpy(tmppanel, panel, sizeof(CELL)*PANEL_HEIGHT*PANEL_WIDTH);
		valid[i] = judge_hasinvalid(tmppanel, cell, i, numpanel) ? 0 : 1;
	}

	if (valid[0] && valid[1]) {
		return -1; 			// unknown
	} else if (!valid[0] && valid[1]) {
		printf("set_panel real panel in (%d, %d) is 9\n", cell.x, cell.y);
		set_panel(panel, cell, 9);
		return 9; 			// valid
	} else if (valid[0] && !valid[1]) {
		printf("set_panel real panel in (%d, %d) is 0\n", cell.x, cell.y);
		set_panel(panel, cell, 0);
		return 0; 			// nomine
	} else {
		printf("[ERR] -- find invalid minesweeper cells!\n");
		return -9; 			// error invalid
	}
}

// OK
void recursive_all_nearby(CELL panel[PANEL_HEIGHT][PANEL_WIDTH], 
	CELL nearbys[PANEL_HEIGHT][PANEL_WIDTH], POS* nearby, int nearby_count) {
	int i = 0;
	for (i = 0; i < nearby_count; i ++) {
		if (panel[nearby[i].x][nearby[i].y] >= 0) { continue; }
		printf("After operator nearby(%d, %d):\n", nearby[i].x, nearby[i].y);
		judge_cell_mine_or_not(panel, nearby[i], nearbys);
		output_panel(panel);
	}
}

/**
* @brief test case for fill cell mine probability.
*/
void testcase_for_fill_cell_mine_probability() {
	CELL panel[PANEL_HEIGHT][PANEL_WIDTH] = {
		{-1,-1,-1,-1, 1, 0, 0, 2,-1,-1,-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1, 3, 1, 1, 2,-1,-1,-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1, 2, 9, 1, 1, 1,-1,-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1, 2, 1, 1, 2,-1,-1,-1,-1,-1,-1,-1},
		{-1, 3, 2, 2, 9, 1, 0, 1, 9,-1,-1,-1,-1,-1,-1,-1},
		{ 1, 1, 0, 1, 1, 1, 0, 1, 2, 2,-1,-1,-1,-1,-1,-1},
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,-1,-1,-1,-1,-1,-1},
		{ 0, 1, 1, 2, 1, 1, 1, 1, 1, 1,-1,-1,-1,-1,-1,-1},
		{ 0, 1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
		{ 0, 1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
		{ 2, 2,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}
	};
	CELL nearbys[PANEL_HEIGHT][PANEL_WIDTH] = {{0}};

	printf("Before tag probability:\n");
	output_panel(panel);

	int nearby_count = 0;
	POS* nearby = find_nearby_cells(panel, &nearby_count);
	find_nearbys_cells(panel, nearbys);
//	int i = 0;
//	for (i = 0; i < nearby_count; i ++) {
//		printf("nearby[%d] = (%d, %d)\n", i, nearby[i].x, nearby[i].y);
//	}

	// pretreat_panel(panel, nearbys, nearby);
	recursive_all_nearby(panel, nearbys, nearby, nearby_count);

	printf("After tag probability:\n");
	output_panel(panel);

	free(nearby);
}

int main(int argc, const char *argv[])
{
	// testcase_for_abc_mine_probability();
	testcase_for_fill_cell_mine_probability();

	return 0;
}
