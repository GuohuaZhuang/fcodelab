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
* @file tetris.c
* @brief tetris auto pick a good place and state to set block.
* @author firstboy0513
* @version 0.0.1
* @date 2014-01-11
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <string.h>
#include <assert.h>

#define BLOCK_SIZE 4
#define BLOCK_TYPE 7
#define BLOCK_ROTATE_SIZE 4
#define TETRIS_WIDTH 12
#define TETRIS_HEIGHT 20

/**
* @brief 4*4 block means one tetris block.
*/
typedef struct _BLOCK {
	int cells[BLOCK_SIZE][BLOCK_SIZE];
} BLOCK;

/**
* @brief global block sets include 7 block type and every 4 rotate state block.
*/
BLOCK g_block_sets[BLOCK_TYPE][BLOCK_ROTATE_SIZE] = {
	{
		{{{0, 0, 0, 0},
		  {1, 1, 1, 0},
		  {0, 1, 0, 0},
		  {0, 0, 0, 0}}},
		{{{0, 1, 0, 0},
		  {1, 1, 0, 0},
		  {0, 1, 0, 0},
		  {0, 0, 0, 0}}},
		{{{0, 1, 0, 0},
		  {1, 1, 1, 0},
		  {0, 0, 0, 0},
		  {0, 0, 0, 0}}},
		{{{0, 1, 0, 0},
		  {0, 1, 1, 0},
		  {0, 1, 0, 0},
		  {0, 0, 0, 0}}}
	},
	{
		{{{0, 0, 0, 0},
		  {0, 1, 1, 0},
		  {0, 1, 0, 0},
		  {0, 1, 0, 0}}},
		{{{0, 0, 0, 0},
		  {0, 1, 0, 0},
		  {0, 1, 1, 1},
		  {0, 0, 0, 0}}},
		{{{0, 0, 1, 0},
		  {0, 0, 1, 0},
		  {0, 1, 1, 0},
		  {0, 0, 0, 0}}},
		{{{0, 0, 0, 0},
		  {0, 1, 1, 1},
		  {0, 0, 0, 1},
		  {0, 0, 0, 0}}}
	},
	{
		{{{0, 0, 0, 0},
		  {0, 1, 1, 0},
		  {0, 0, 1, 0},
		  {0, 0, 1, 0}}},
		{{{0, 0, 0, 0},
		  {0, 1, 1, 1},
		  {0, 1, 0, 0},
		  {0, 0, 0, 0}}},
		{{{0, 1, 0, 0},
		  {0, 1, 0, 0},
		  {0, 1, 1, 0},
		  {0, 0, 0, 0}}},
		{{{0, 0, 0, 0},
		  {0, 0, 0, 1},
		  {0, 1, 1, 1},
		  {0, 0, 0, 0}}}
	},
	{
		{{{0, 0, 0, 0},
		  {1, 1, 0, 0},
		  {0, 1, 1, 0},
		  {0, 0, 0, 0}}},
		{{{0, 1, 0, 0},
		  {1, 1, 0, 0},
		  {1, 0, 0, 0},
		  {0, 0, 0, 0}}},
		{{{0, 0, 0, 0},
		  {1, 1, 0, 0},
		  {0, 1, 1, 0},
		  {0, 0, 0, 0}}},
		{{{0, 1, 0, 0},
		  {1, 1, 0, 0},
		  {1, 0, 0, 0},
		  {0, 0, 0, 0}}}
	},
	{
		{{{0, 0, 0, 0},
		  {0, 1, 1, 0},
		  {1, 1, 0, 0},
		  {0, 0, 0, 0}}},
		{{{0, 1, 0, 0},
		  {0, 1, 1, 0},
		  {0, 0, 1, 0},
		  {0, 0, 0, 0}}},
		{{{0, 0, 0, 0},
		  {0, 1, 1, 0},
		  {1, 1, 0, 0},
		  {0, 0, 0, 0}}},
		{{{0, 1, 0, 0},
		  {0, 1, 1, 0},
		  {0, 0, 1, 0},
		  {0, 0, 0, 0}}}
	},
	{
		{{{0, 0, 0, 0},
		  {1, 1, 1, 1},
		  {0, 0, 0, 0},
		  {0, 0, 0, 0}}},
		{{{0, 0, 1, 0},
		  {0, 0, 1, 0},
		  {0, 0, 1, 0},
		  {0, 0, 1, 0}}},
		{{{0, 0, 0, 0},
		  {1, 1, 1, 1},
		  {0, 0, 0, 0},
		  {0, 0, 0, 0}}},
		{{{0, 0, 1, 0},
		  {0, 0, 1, 0},
		  {0, 0, 1, 0},
		  {0, 0, 1, 0}}}
	},
	{
		{{{0, 0, 0, 0},
		  {0, 1, 1, 0},
		  {0, 1, 1, 0},
		  {0, 0, 0, 0}}},
		{{{0, 0, 0, 0},
		  {0, 1, 1, 0},
		  {0, 1, 1, 0},
		  {0, 0, 0, 0}}},
		{{{0, 0, 0, 0},
		  {0, 1, 1, 0},
		  {0, 1, 1, 0},
		  {0, 0, 0, 0}}},
		{{{0, 0, 0, 0},
		  {0, 1, 1, 0},
		  {0, 1, 1, 0},
		  {0, 0, 0, 0}}}
	}
};

/**
* @brief panel include TETRIS_HEIGHT*TETRIS_WIDTH table cells, also include 
* column height named as colheight array.
*/
typedef struct _PANEL {
	int panel[TETRIS_HEIGHT][TETRIS_WIDTH];
	int colheight[TETRIS_WIDTH];
} PANEL;

/**
* @brief output a tetrix block.
*
* @param block tetrix block.
*/
void output_tetris_block(BLOCK* block) {
	int i = 0, j = 0;
	printf("-----------\n");
	for (i = 0; i < BLOCK_SIZE; i ++) {
		printf("| ");
		for (j = 0; j < BLOCK_SIZE; j ++) {
			if (block->cells[i][j]) {
				printf("o ");
			} else {
				printf("  ");
			}
		}
		printf("|\n");
	}
	printf("-----------\n");
}

/**
* @brief output all tetrix blocks.
*/
void output_all_tetris_blocks() {
	int i = 0, j = 0;
	for (i = 0; i < BLOCK_TYPE; i ++) {
		for (j = 0; j < BLOCK_ROTATE_SIZE; j ++) {
			output_tetris_block(&g_block_sets[i][j]);
		}
	}
}

/**
* @brief initialize tetrix panel.
*
* @return return initialized tetrix panel.
*/
PANEL* init_tetris_panel() {
	PANEL* panel = (PANEL*) malloc(sizeof(PANEL));
	memset(panel, 0, sizeof(PANEL));
	int i = 0;
	for (i = 0; i < TETRIS_WIDTH; i ++) {
		panel->colheight[i] = TETRIS_HEIGHT;
	}
	return panel;
}

/**
* @brief release tetris panel.
*
* @param panel tetrix panel.
*/
void release_tetris_panel(PANEL* panel) {
	free(panel);
}

/**
* @brief output in the console of tetris panel table cells.
*
* @param panel tetrix panel.
*/
void output_tetris_panel(PANEL* panel) {
	int i = 0, j = 0;
	printf("---------------------------\n");
	for (i = 0; i < TETRIS_HEIGHT; i ++) {
		printf("| ");
		for (j = 0; j < TETRIS_WIDTH; j ++) {
			if (panel->panel[i][j]) {
				printf("o ");
			} else {
				printf("  ");
			}
		}
		printf("|\n");
	}
	printf("---------------------------\n");
}

/**
* @brief best place include the block rotate as best_rotate, best place 
* left-top column position as best_j, best row as best_y, and corresponding 
* best clean line count.
*/
typedef struct _BEST_PLACE {
	int best_rotate;
	int best_j;
	int best_y;
	int best_cleanline;
} BEST_PLACE;

/**
* @brief get block extreme column range.
*
* @param block block point.
* @param pmin_col minimum column index.
* @param pmax_col maximum column index.
*/
void get_col_extreme_range(BLOCK* block, int* pmin_col, int* pmax_col) {

	assert(pmin_col != NULL);
	assert(pmax_col != NULL);

	int i = 0, j = 0, bit_count = 0;
	*pmin_col = -1, *pmax_col = 0;
	for (j = 0; j < BLOCK_SIZE; j ++) {
		bit_count = 0;
		for (i = 0; i < BLOCK_SIZE; i ++) {
			bit_count |= block->cells[i][j];
		}
		if (0 != bit_count) {
			if (-1 == *pmin_col) { *pmin_col = j; }
			*pmax_col = j;
		}
	}
}

/**
* @brief get block extreme row range.
*
* @param block block point.
* @param pmin_row minimum row index.
* @param pmax_row maximum row index.
*/
void get_row_extreme_range(BLOCK* block, int* pmin_row, int* pmax_row) {

	assert(pmin_row != NULL);
	assert(pmax_row != NULL);

	*pmin_row = -1, *pmax_row = 0;
	int i = 0, j = 0, bit_count = 0;
	for (i = 0; i < BLOCK_SIZE; i ++) {
		bit_count = 0;
		for (j = 0; j < BLOCK_SIZE; j ++) {
			bit_count |= block->cells[i][j];
		}
		if (0 != bit_count) {
			if (-1 == *pmin_row) { *pmin_row = j; }
			*pmax_row = j;
		}
	}
}

/**
* @brief get block all column offsets.
*
* @param block block point.
* @param bcol block column array.
*/
void get_offset_block_col(BLOCK* block, int* bcol) {
	int i = 0, j = 0;
	for (j = 0; j < BLOCK_SIZE; j ++) {
		for (i = BLOCK_SIZE-1; i >= 0; i --) {
			if (block->cells[i][j]) { break; }
		}
		bcol[j] = i;
	}
}

/**
* @brief get x offset range.
*
* @param block block point.
* @param pmin_col minimum column index in panel.
* @param pmax_col maximum column index in panel.
* @param pbmin_col block itself minimum column.
* @param pbmax_col block itself maximum column.
*/
void get_offset_x_range(BLOCK* block, 
	int* pmin_col, int* pmax_col, int* pbmin_col, int* pbmax_col) {
	*pbmin_col = 0, *pbmax_col = 0;
	get_col_extreme_range(block, pbmin_col, pbmax_col);
	*pmin_col = 0 - *pbmin_col;
	*pmax_col = TETRIS_WIDTH - 1 - *pbmax_col;
}

/**
* @brief get bottom offset row index as y. The y value is min(colheight - bcol).
* And colheight is the current panel maximum height, bcol is the column offset 
* in the block itself.
*
* @param panel panel.
* @param block block.
* @param j the column of block position left index.
* @param bmin_col block minimum column.
* @param bmax_col block maximum column.
* @param bcol block column offsets array.
*
* @return return the minimum row position as the y offset.
*/
int get_bottom_offset_y(PANEL* panel, BLOCK* block, int j, 
	int bmin_col, int bmax_col, int* bcol) {
	int i = 0, _j = 0, min_offset = TETRIS_HEIGHT, cur_offset = 0;
	for (_j = bmin_col+j, i = bmin_col; _j <= bmax_col+j; _j ++, i ++) {
		cur_offset = panel->colheight[_j] - bcol[i] - 1;
		if (min_offset > cur_offset) {
			min_offset = cur_offset;
		}
	}
	return min_offset;
}
 
/**
* @brief set tetris place in panel use block.
*
* @param panel panel.
* @param block block to set in panel.
* @param place_j block in the panel column position.
* @param place_y block in the panel row position.
*/
void set_tetris_place_use_block(PANEL* panel, BLOCK* block, int place_j, 
	int place_y) {
	int i = 0, j = 0, _i = 0, _j = 0;
	int min_col = 0, max_col = 0, bmin_col = 0, bmax_col = 0;
	get_offset_x_range(block, &min_col, &max_col, &bmin_col, &bmax_col);
	for (j = bmin_col+place_j, _j = bmin_col; 
		j <= bmax_col+place_j; j ++, _j ++) {
		for (i = place_y+BLOCK_SIZE-1, _i = BLOCK_SIZE-1; _i >= 0; 
			i --, _i --) {
			if (block->cells[_i][_j]) {
				panel->panel[i][j] |= 0x01;
				panel->colheight[j] = i;
			}
		}
	}
}
 
/**
* @brief set tetris place in panel use block rotate and block type.
*
* @param panel panel.
* @param block_rotate block rotate.
* @param block_type block type.
* @param place_j block in the panel column position.
* @param place_y block in the panel row position.
*/
void set_tetris_place(PANEL* panel, int block_rotate, int block_type, 
	int place_j, int place_y) {
	BLOCK* block = &(g_block_sets[block_type][block_rotate]);
	set_tetris_place_use_block(panel, block, place_j, place_y);
}

/**
* @brief judge the row in panel is full or not.
*
* @param panel panel.
* @param y the row in panel.
*
* @return return 1 means the row in panel is full, otherwise is not full.
*/
int tetris_panel_row_is_full(PANEL* panel, int y) {
	int _j = 0;
	for (_j = 0; _j < TETRIS_WIDTH; _j ++) {
		if (0 == panel->panel[y][_j]) {
			return 0;
		}
	}
	return 1;
}

/**
* @brief tetris panel cleanline set.
*
* @param panel panel.
* @param y the row of block start in panel.
*/
void tetris_panel_cleanline(PANEL* panel, int y) {
	int _i = 0, _j = 0;
	for (_i = y-1; _i >= 0; _i --) {
		memcpy(panel->panel[_i+1], panel->panel[_i], sizeof(int)*TETRIS_WIDTH);
	}
	for (_j = 0; _j < TETRIS_WIDTH; _j ++) {
		if (panel->colheight[_j] < TETRIS_HEIGHT) {
			panel->colheight[_j] ++;
		}
	}
}

/**
* @brief tetris cleanline and retrn cleanline count.
*
* @param panel panel.
* @param y the row of block start in panel.
*
* @return return cleanline count.
*/
int tetris_cleanline(PANEL* panel, int y) {
	int _i = 0, _max_i = BLOCK_SIZE-1, cleanline = 0;
	for (_i = 0; _i <= _max_i && _i+y < TETRIS_HEIGHT; _i ++) {
		if (tetris_panel_row_is_full(panel, _i+y)) {
			tetris_panel_cleanline(panel, _i+y);
			cleanline ++;
			_i --, _max_i --;
		}
	}
	return cleanline;
}

/**
* @brief weight score of clean line count.
*/
const int g_cleanline_wscore[BLOCK_SIZE] = {1, 3, 7, 13};
/**
* @brief weight score of great than high height.
*/
const int g_high_height = TETRIS_HEIGHT * 3 / 5;

/**
* @brief calculate the hole count in tetris panel in given column.
*
* @param panel tetris panel.
* @param col given column.
*
* @return return hole count in given column.
*/
int tetris_holecount(PANEL* panel, int col) {
	int _i = 0, holecount = 0;
	for (_i = panel->colheight[col]; _i < TETRIS_HEIGHT; _i ++) {
		if (0 == panel->panel[_i][col]) {
			holecount ++;
		}
	}
	return holecount;
}

/**
* @brief calculate tetris place score in one place solution.
* The weight score rule:
* 	1. clean line count is {1, 2, 3, 4} add corresponding {1, 3, 7, 13} score.
* 	2. generate 1 hole subtract 4 score, when holecount > 5, subtract 15 score.
* 	3. when place more than high height subtract 2 score every line.
*
* @param panel tetris panel.
* @param block block point.
* @param bmin_col block itself minimum column.
* @param bmax_col block itself maximum column.
* @param j block column position in panel.
* @param y block row position in panel.
* @param pcleanline output clean line count.
*
* @return return weight score.
*/
double get_tetris_place_score(PANEL* panel, BLOCK* block, 
	int bmin_col, int bmax_col, int j, int y, int* pcleanline) {

	assert(pcleanline != NULL);
	if (y < 0) { *pcleanline = -1; return -1; }

	*pcleanline = 0;
	int _j = 0, holecount = 0;
	double score = 0;

	PANEL* tmppanel = (PANEL*) malloc(sizeof(PANEL));
	memcpy(tmppanel, panel, sizeof(PANEL));
	set_tetris_place_use_block(tmppanel, block, j, y);
	
	// add clean line score
	*pcleanline = tetris_cleanline(tmppanel, y);
	if (*pcleanline > 0) {
		score += g_cleanline_wscore[*pcleanline-1];
	}
	y += *pcleanline;

	// subtract hole count score
	for (_j = j+bmin_col; _j <= j+bmax_col; _j ++) {
		holecount += tetris_holecount(tmppanel, _j)-tetris_holecount(panel, _j);
	}
	score -= holecount*4;
	if (holecount > 5) { score -= 15; }

	// subtract score when too high
	if (y < g_high_height) {
		score -= (g_high_height-y)*2;
	}

	free(tmppanel);

	return score;
}

/**
* @brief recursive to place all possible position, and compare with place 
* weight score to get the best place solution.
* The recursion is use every 4 block rotate, and minimum column to maximum 
* column index in panel.
*
* @param panel tetris panel.
* @param block_type block type.
*
* @return return best place solution.
*/
BEST_PLACE recursion_places_and_get_best(PANEL* panel, int block_type) {
	BEST_PLACE best_place = {0, 0, 0, 0};
	int i = 0, j = 0, y = 0, best_wscore = INT32_MIN, wscore = 0, cleanline = 0;
	int min_col = 0, max_col = 0, bmin_col = 0, bmax_col = 0;
	int bcol[BLOCK_SIZE] = {0};

	for (i = 0; i < BLOCK_ROTATE_SIZE; i ++) {              // all rotate state
		BLOCK* block = &(g_block_sets[block_type][i]);
		get_offset_x_range(block, &min_col, &max_col, &bmin_col, &bmax_col);
		get_offset_block_col(block, bcol);
		for (j = min_col; j <= max_col; j ++) {             // all column range
			y = get_bottom_offset_y(panel, block, j, bmin_col, bmax_col, bcol);
			wscore = get_tetris_place_score(panel, block, bmin_col, bmax_col, 
				j, y, &cleanline);
			if (best_wscore < wscore) {
				best_wscore = wscore;
				best_place.best_rotate = i;
				best_place.best_j = j;
				best_place.best_cleanline = cleanline;
				best_place.best_y = y;
			}
		}
	}

	return best_place;
}

/**
* @brief set best tetris place in the panel.
*
* @param panel tetris panel.
* @param block_type block type.
*
* @return return best place clean line count.
*/
int set_best_tetris_place(PANEL* panel, int block_type) {
	BEST_PLACE best_place = recursion_places_and_get_best(panel, block_type);
	if (best_place.best_y >= 0) {
		set_tetris_place(panel, best_place.best_rotate, block_type, 
			best_place.best_j, best_place.best_y);
		tetris_cleanline(panel, best_place.best_y);
	}
	return best_place.best_cleanline;
}

/**
* @brief run tetris until game ove to break out.
*/
void run_tetris() {

	PANEL* panel = init_tetris_panel();
	int score = 0, cleanline = 0;

	srand(time(NULL));
	while (1) {
		// set best tetris place
		int block_type = (rand()%BLOCK_TYPE);
		cleanline = set_best_tetris_place(panel, block_type);

		// game over break
		if (cleanline < 0) { break; }

		// output block type and the current tetris panel state
		output_tetris_block(
			&(g_block_sets[block_type][rand()%BLOCK_ROTATE_SIZE]));
		output_tetris_panel(panel);
		score += cleanline;
		printf("now the score is: %d\n", score);

		// just for debug
		printf("press enter to go the next circle. ");
		getchar();
		// end for debut
	}

	printf("the end score is: %d\n", score);

	release_tetris_panel(panel);
}

int main(int argc, const char *argv[])
{
	run_tetris();

	return 0;
}
