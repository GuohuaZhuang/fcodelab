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
* @file pacman.c
* @brief Pacman Game calculate shortest paths question.
* This question is comes from freelancer.com. Following is the details.
* Pacman Game.
* ======================
* -- The input of the game will be a table.
* -- Every cell in the table is a score (ie: score of fruit: if score is > 0 
*  (positive) = fresh fruit; if < 0 = rotten fruit).
* -- The user will get the stating and ending point.
* -- The user will need to find (with starting and ending pts) the shortest 
*  path.
* The input
* ======================
* A. First of all you'll need to select the representation type of the table.
* There are 2 types of representations:
* 	0. Matrix Representation.
* 	1. Non-Matrix representation.
* B. Secondly, right after, you'll need to select the size of the table (in 
* "int" and it has to be positive!)
* 	Ex: 4 5
* 	4 = rows
* 	5 = columns
* <1, 1> will be the top left cell and <4, 5> will be the bottom right.
* C. Input the starting point. Input will be as follows <row, column>
* 	Ex: 1 2
* 	1 = rows
* 	2 = columns
* D. Input the ending point. Input will be as follows <row, column>
* 	Ex: 3, 3
* 	3 = rows
* 	3 = columns
* E. On this step you will also need to input the weight of each cell. The input
* of the weight will be as follows: <row, column, weight>
* 	-- If weight is not defined, it automatically sets itself to 1 (default).
* 	-- If a cell has the weight of -100: consider it as a blocked cell which 
* 	means you cannot work through the cell (Mark it as 'X').
* 	Ex: 1 2 4
* 		3 3 -30
* 		2 5 -100
* F. To exit the input user has to select -1 -1 -1.
*
* Rules of the Game
* =========================
* You can move from one cell to another by following the conditions:
* 	1. Can't get out of the limits.
* 	2. Move Up, Down, Left, Right (no diagonally).
* 	3. Can't move to a blocked cell.
* 	4. Can't move to the same cell you last visited.
* 
* The Output
* =========================
* You need to report the path that is shortest from the starting point to the 
* ending point.
* If there are several (similar) paths you need to report the one that has the 
* most weight amongst them.
* And if there are several of such paths (with weights) you need to report the 
* maximum amongst them.
* When cell <x1, y1> is smaller than cell <x2, y2> if x1<x2, or if y1<y2 while
* x1==x2.
* Ex: 3.3 is smaller than 4,1 and smaller than 3,4.
* The order between cells will be from starting point to ending point.
* The output will include the details of the path from starting point to ending
* point (see last example). If there aren't any paths like it (ie. Path is 
* blocked by blocked cells) print: no path.
* Example1:
* 	Input:
* 		0
* 		4 3
* 		2 1
* 		4 2
* 		1 2 -100
* 		2 2 -21
* 		4 1 100
* 		-1 -1 -1
* 	Output:
* 		<2,1>,<3,1>,<4,1>,<4,2>
*
* Example2:
* 	Input:
* 		1
* 		400 300
* 		1 1
* 		400 300
* 		1 2 -100
* 		2 1 -100
* 		-1 -1 -1
* 	Output:
* 		No path
* 
* Simple algorithm:
* 	1. generate a table has rows*columns capacity, and with passed, road, weight.
* 	2. 
*
* @author firstboy0513
* @version 0.0.1
* @date 2014-03-05
*/
#include <stdio.h>
#include <stdlib.h>

typedef struct _road_element {
//	int x;			// coordinate x-axis
//	int y;			// coordinate y-axis
	struct _road_point point;		// road point
	int road;		// path length
	int weight;		// path weight sum
	struct _road_element* paths;	// path from starting
} ROAD_ELE;

typedef struct _road_point {
	int x;
	int y;
} ROAD_POINT;

typedef struct _table_cell {
	int score;		// score of fruit
	int passed;		// passed or not
} TABLE_CELL;

typedef struct _table {
	TABLE_CELL** arr;
	ROAD_POINT start;
	ROAD_POINT end;
	int rows;
	int columns;
} TABLE;

TABLE* init_table(int rows, int columns, 
	int start_x, int start_y, int end_x, int end_y) {
	if (rows < 1 || columns < 1) {
		fprintf("initialize table failed\n"); exit(-1);
	}
	if (start_x < 1 || start_x > rows || start_y < 1 || start_y > columns
		|| end_x < 1 || end_x > rows || end_y < 1 || end_y > columns) {
		fprintf("initialize table failed\n"); exit(-1);
	}
	TABLE* table = (TABLE*) malloc(sizeof(TABLE));
	if (!table) { fprintf(stderr, "malloc error\n"); exit(-1); }
	int i = 0, j = 0;
	table->arr = (TABLE_CELL**) malloc(sizeof(TABLE_CELL*) * rows);
	if (!table->arr) { fprintf(stderr, "malloc error\n"); exit(-1); }
	for (i = 0; i < rows; i ++) {
		table->arr[i] = (TABLE_CELL*) malloc(sizeof(TABLE_CELL) * columns);
		if (!table->arr[i]) { fprintf(stderr, "malloc error\n"); exit(-1); }
		for (j = 0; j < columns; j ++) {
			table->arr[i][j] = 1;
		}
	}
	table->rows = rows; table->columns = columns;
	table->start.x = start_x; table->start.y = start_y;
	table->end.x = end_x; table->end.y;
	return table;
}

void release_table(TABLE* table) {
	int i = 0;
	for (i = 0; i < table->rows; i ++) {
		free(table->arr[i]);
	}
	free(table->arr);
	free(table);
}

void setweight_table(TABLE* table, int x, int y, int weight) {
	if (x > 0 && x <= rows && y > 0 && y <= columns) {
		table->arr[x][y] = weight;
	} else {
		fprintf(stderr, "set weight failed!\n"); exit(-1);
	}
}

void add_four_direction(ROAD_ELE s, forward_list) {
	if (... is updated) LIST_ADD(forward_list, s->left);
	if (... is updated) LIST_ADD(forward_list, s->top);
	if (... is updated) LIST_ADD(forward_list, s->right);
	if (... is updated) LIST_ADD(forward_list, s->bottom);
}

ROAD_ELE* operator_forward_list(forward_list) {
	ROAD_ELE* new_list;
	while (forward_list) {
		do {
			ROAD_ELE t = LIST_POP(forward_list);
			add_four_direction(, new_list);
		} while (forward_list);
	}
	return new_list;
}

ROAD_ELE pacman_shortestpath(TABLE table) {
	add_four_direction(start, forward_list);
	while (forward_list) {
		forward_list = operator_forward_list(forward_list);
	}
	if (ending->passed) {
		printf("printf the paths\n");
	} else {
		printf("No path\n");
	}
}

int main(int argc, const char *argv[])
{
	printf("Input:\n");
	int rows = 0, columns = 0, start_x = 0, start_y = 0, end_x = 0, end_y = 0;
	scanf("%d", &rows);
	scanf("%d", &rows); scanf("%d", &columns);
	scanf("%d", &start_x); scanf("%d", &start_y);
	scanf("%d", &end_x); scanf("%d", &end_y);
	TABLE* table = init_table(rows, columns, start_x, start_y, end_x, end_y);
	int x = 0, y = 0, w = 0;
	do {
		scanf("%d", &x); scanf("%d", &y); scanf("%d", &w);
		if (-1 == x || -1 == y) { break; }
		setweight_table(table, x, y, w);
	}

	printf("Output:\n");
	pacman_shortestpath(table);

	release_table(table);

	return 0;
}
