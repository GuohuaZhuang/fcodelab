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
* @file sudoku.c
* @brief sudoku game generate valid table cells.
* @author firstboy0513
* @version 0.0.1
* @date 2014-01-10
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SUDOKU_SIZE 9

/**
* @brief coord position struct.
*/
typedef struct _COORD {
	int x;
	int y;
} COORD;

/**
* @brief cell struct of sudoku table element.
*/
typedef struct _CELL {
	int validvalues[SUDOKU_SIZE];
	int validvalues_size;
	int validindex;
} CELL;

/**
* @brief sudoku table cells initialize.
*
* @param size cells row size or column size.
*
* @return return sudo ku table cells.
*/
CELL** cells_init(const int size) {
	CELL** cells = (CELL**) malloc(sizeof(CELL*) * size);
	int i = 0, j = 0;
	for (i = 0; i < size; i ++) {
		cells[i] = (CELL*) malloc(sizeof(CELL) * size);
		for (j = 0; j < size; j ++) {
			cells[i][j].validvalues_size = -1;
			cells[i][j].validindex = 0;
			memset(cells[i][j].validvalues, 0, sizeof(int) * size);
		}
	}
	return cells;
}

/**
* @brief sudoku table cells release.
*
* @param cells sudoku table cells.
* @param size cells size.
*/
void cells_release(CELL** cells, const int size) {
	int i = 0;
	for (i = 0; i < size; i ++) {
		free(cells[i]);
	}
	free(cells);
}

/**
* @brief get valid values for sudoku cell values.
*
* @param cells sudoku table cells.
* @param size sudoku table row size or column size.
* @param cell current cell point.
* @param coCurrent current coord position.
*/
void get_valid_values(CELL** cells, const int size, CELL* cell, 
	COORD coCurrent) {
	int _candidate[SUDOKU_SIZE] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
	int i = 0, j = 0, x = coCurrent.x, y = coCurrent.y;
	// vertical valid
	if (x > 0) {
		for (i = x-1; i >= 0; i --) {
			_candidate[
				cells[i][y].validvalues[cells[i][y].validindex] - 1
				] = -1;
		}
	}
	// horizontal valid
	if (y > 0) {
		for (i = y-1; i >= 0; i --) {
			_candidate[
				cells[x][i].validvalues[cells[x][i].validindex] - 1
				] = -1;
		}
	}
	// sub 3*3 array valid
	int subx = x/3, suby = y/3;
	for (i = subx*3; i <= x; i ++) {
		for (j = suby*3; j < (suby+1)*3; j ++) {
			if (i == x && j == y) { break; }
			_candidate[
				cells[i][j].validvalues[cells[i][j].validindex] - 1
				] = -1;
		}
	}
	cell->validvalues_size = 0;
	for (i = 0; i < SUDOKU_SIZE; i ++) {
		if (_candidate[i] > 0) {
			cell->validvalues[cell->validvalues_size ++] = _candidate[i];
		}
	}
	cell->validindex = cell->validvalues_size;
}

/**
* @brief get next coord position.
*
* @param coCurrent current coord position.
*
* @return return next coord position.
*/
COORD next_coord(COORD coCurrent, const int size) {
	if (coCurrent.y < size-1) {
		coCurrent.y ++;
	} else {
		if (coCurrent.x < size-1) {
			coCurrent.x ++;
			coCurrent.y = 0;
		}
	}
	return coCurrent;
}

/**
* @brief get previous coord position.
*
* @param coCurrent current coord position.
*
* @return return previous coord position.
*/
COORD prev_coord(COORD coCurrent, const int size) {
	if (coCurrent.y > 0) {
		coCurrent.y --;
	} else {
		if (coCurrent.x > 0) {
			coCurrent.x --;
			coCurrent.y = size-1;
		}
	}
	return coCurrent;
}

/**
* @brief generate valid sudoku use recursion.
*
* @return return 0 means success generated, otherwise return -1 means can not
* recursion to generate a valid sudoku.
*/
int generate_valid_sudoku_use_recursion(CELL** cells, const int size) {
	COORD coCurrent = {0, 0};
	while (1) {
		CELL* cell = &(cells[coCurrent.x][coCurrent.y]);
		if (-1 == cell->validvalues_size) {
			get_valid_values(cells, size, cell, coCurrent);
		}

		if (cell->validvalues_size > 0) {
			cell->validindex --;
			cell->validvalues_size --;
			if (coCurrent.x == size-1 && coCurrent.y == size-1) {
				break;
			}
			coCurrent = next_coord(coCurrent, size);
		} else {
			if (coCurrent.x == 0 && coCurrent.y == 0) {
				printf("[ERR] -- can not recursion a valid sudoku.\n");
				break;
			}
			cells[coCurrent.x][coCurrent.y].validvalues_size = -1;
			coCurrent = prev_coord(coCurrent, size);
		}
	}
	if (coCurrent.x == 0 && coCurrent.y == 0) { return -1; }
	return 0;
}

/**
* @brief output valid sudoku table.
*
* @param cells sudoku table.
* @param size cells row size or column size.
*/
void output_valid_sudoku(CELL** cells, const int size) {
	int i = 0, j = 0;
	for (i = 0; i < size; i ++) {
		for (j = 0; j < size; j ++) {
			if (cells[i][j].validvalues_size >= 0) {
				printf("%d\t", 
					cells[i][j].validvalues[cells[i][j].validindex]);
			} else {
				printf("0\t");
			}
		}
		printf("\n");
	}
}

#define ALL_9_BIT 0x1ff

/**
* @brief verify sudoku array.
*
* @param cells sudoku table array.
* @param size sudoku size.
*
* @return return 1 if sudoku is valid, otherwise return 0 if invalid.
*/
int verify_sudoku(CELL** cells, const int size) {

	int i = 0, j = 0, m = 0, n = 0, all_9_bit = 0, sub_size = size/3;

	// horizontal verify
	for (i = 0; i < size; i ++) {
		all_9_bit = ALL_9_BIT;
		for (j = 0; j < size; j ++) {
			all_9_bit &= 
				~(0x01 << (cells[i][j].validvalues[cells[i][j].validindex]-1));
		}
		if (all_9_bit != 0) { return 0; }
	}

	// vertical verify
	for (j = 0; j < size; j ++) {
		all_9_bit = ALL_9_BIT;
		for (i = 0; i < size; i ++) {
			all_9_bit &= 
				~(0x01 << (cells[i][j].validvalues[cells[i][j].validindex]-1));
		}
		if (all_9_bit != 0) { return 0; }
	}

	// sub 3*3 verify
	for (m = 0; m < sub_size; m ++) {
		for (n = 0; n < sub_size; n ++) {
			all_9_bit = ALL_9_BIT;
			for (i = m*sub_size; i < (m+1)*sub_size; i ++) {
				for (j = n*sub_size; j < (n+1)*sub_size; j ++) {
					all_9_bit &= 
						~(0x01 << (cells[i][j].validvalues[
							cells[i][j].validindex]-1));
				}
			}
			if (all_9_bit != 0) { return 0; }
		}
	}
	return 1;
}

/**
* @brief test case for generate valid sudoku use recursion.
*/
void testcase_for_generate_sudoku_use_recursion() {

	CELL** cells = cells_init(SUDOKU_SIZE);

	generate_valid_sudoku_use_recursion(cells, SUDOKU_SIZE);
	output_valid_sudoku(cells, SUDOKU_SIZE);

	if (verify_sudoku(cells, SUDOKU_SIZE)) {
		printf("the sudoku is valid!\n");
	} else {
		printf("the sudoku is invalid!\n");
	}

	cells_release(cells, SUDOKU_SIZE);
}

////////////////////////////// sign replacement ////////////////////////////////

/**
* @brief generate valid sudoku signs, but is save index from 0~8 as the sign.
* the same as horizontal memcpy as core like top-left and top-right, 
* bottom-left and bottom-right.
*
* @param cells[SUDOKU_SIZE][SUDOKU_SIZE] cells.
*/
void generate_valid_sudoku_signs(int cells[SUDOKU_SIZE][SUDOKU_SIZE]) {

	int i = 0, j = 0;

	// fill core 3*3 sub array
	for (i = 0; i < SUDOKU_SIZE; i ++) {
		cells[ 3+(i/3) ] [ 3+(i%3) ] = i;
	}
	
	// fill core's left, right, top, bottom 3*3 sub array
	for (i = 0; i < 3; i ++) {
		for (j = 0; j < 3; j ++) {
			// left and right sub array
			cells[3+(i)][0+j] = cells[3+((i+1)%3)][3+j];
			cells[3+(i)][6+j] = cells[3+((i+2)%3)][3+j];
			// top and bottom sub array
			cells[0+j][3+(i)] = cells[3+j][3+((i+1)%3)];
			cells[6+j][3+(i)] = cells[3+j][3+((i+2)%3)];
		}
	}
	
	// fill 4 angle 3*3 sub array
	for (i = 0; i < 3; i ++) {
		for (j = 0; j < 3; j ++) {
			cells[0+(i)][0+j] = cells[0+((i+1)%3)][3+j];
			cells[0+(i)][6+j] = cells[0+((i+2)%3)][3+j];
			cells[6+(i)][0+j] = cells[6+((i+1)%3)][3+j];
			cells[6+(i)][6+j] = cells[6+((i+2)%3)][3+j];
		}
	}
}

/**
* @brief fill the valid sudoku signs valu use it self index sign.
*
* @param cells[SUDOKU_SIZE][SUDOKU_SIZE] sudoku cells.
* @param sign[SUDOKU_SIZE] sign array.
*/
void fill_valid_sudoku_signs_value(int cells[SUDOKU_SIZE][SUDOKU_SIZE], 
	int sign[SUDOKU_SIZE]) {
	int i = 0, j = 0;
	for (i = 0; i < SUDOKU_SIZE; i ++) {
		for (j = 0; j < SUDOKU_SIZE; j ++) {
			cells[i][j] = sign[ cells[i][j] ];
		}
	}
}

/**
* @brief output valid sign sudoku table.
*
* @param cells sudoku sign table.
* @param size cells row size or column size.
*/
void output_valid_sign_sudoku(int cells[SUDOKU_SIZE][SUDOKU_SIZE]) {
	int i = 0, j = 0;
	for (i = 0; i < SUDOKU_SIZE; i ++) {
		for (j = 0; j < SUDOKU_SIZE; j ++) {
			printf("%d\t", cells[i][j]);
		}
		printf("\n");
	}
}

/**
* @brief verify sign sudoku array.
*
* @param cells sign sudoku table array.
* @param size sudoku size.
*
* @return return 1 if sudoku is valid, otherwise return 0 if invalid.
*/
int verify_sign_sudoku(int cells[SUDOKU_SIZE][SUDOKU_SIZE]) {

	int i = 0, j = 0, m = 0, n = 0, all_9_bit = 0, sub_size = SUDOKU_SIZE/3;

	// horizontal verify
	for (i = 0; i < SUDOKU_SIZE; i ++) {
		all_9_bit = ALL_9_BIT;
		for (j = 0; j < SUDOKU_SIZE; j ++) {
			all_9_bit &= ~(0x01 << (cells[i][j]-1));
		}
		if (all_9_bit != 0) { return 0; }
	}

	// vertical verify
	for (j = 0; j < SUDOKU_SIZE; j ++) {
		all_9_bit = ALL_9_BIT;
		for (i = 0; i < SUDOKU_SIZE; i ++) {
			all_9_bit &= ~(0x01 << (cells[i][j]-1));
		}
		if (all_9_bit != 0) { return 0; }
	}

	// sub 3*3 verify
	for (m = 0; m < sub_size; m ++) {
		for (n = 0; n < sub_size; n ++) {
			all_9_bit = ALL_9_BIT;
			for (i = m*sub_size; i < (m+1)*sub_size; i ++) {
				for (j = n*sub_size; j < (n+1)*sub_size; j ++) {
					all_9_bit &= ~(0x01 << (cells[i][j]-1));
				}
			}
			if (all_9_bit != 0) { return 0; }
		}
	}
	return 1;
}

/**
* @brief random generate sign.
*
* @param sign[SUDOKU_SIZE] sign array.
*/
void random_generate_sign(int sign[SUDOKU_SIZE]) {
	srand(time(NULL));
	int i = 0, j = 0;
	for (i = 0; i < SUDOKU_SIZE; i ++) {
		j = rand() % SUDOKU_SIZE;
		if (i != j) {
			sign[i] ^= sign[j];
			sign[j] ^= sign[i];
			sign[i] ^= sign[j];
		}
	}
}

/**
* @brief test case for generate valid sudoku use replacement.
*/
void testcase_for_generate_sudoku_use_replacement() {

	int sign[SUDOKU_SIZE] = {1, 2, 3, 4, 5, 6, 7, 8, 9};

	// can random the sign here
	random_generate_sign(sign);
	
	int cells[SUDOKU_SIZE][SUDOKU_SIZE] = {{0}};

	generate_valid_sudoku_signs(cells);
	fill_valid_sudoku_signs_value(cells, sign);
	output_valid_sign_sudoku(cells);

	if (verify_sign_sudoku(cells)) {
		printf("the sudoku is valid!\n");
	} else {
		printf("the sudoku is invalid!\n");
	}
}

int main(int argc, const char *argv[])
{
	testcase_for_generate_sudoku_use_recursion();
	testcase_for_generate_sudoku_use_replacement();

	return 0;
}
