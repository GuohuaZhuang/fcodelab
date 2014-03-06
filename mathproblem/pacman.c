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
* @author firstboy0513
* @version 0.0.1
* @date 2014-03-05
*/
#include <stdio.h>
#include <stdlib.h>

typedef struct _road_point {
	int x;
	int y;
	struct _road_point* next;		// use for paths
} ROAD_POINT;

typedef struct _road_element {
	struct _road_point point;		// road point
	int sumweight;					// path weight sum
	ROAD_POINT* paths;				// path from starting
	struct _road_element* next; 	// link as a list
} ROAD_ELE;

typedef struct _table_cell {
	int weight;						// path weight
	int score; 					 	// score of fruit
	int passed;						// passed or not
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
		fprintf(stderr, "initialize table failed\n"); exit(-1);
	}
	if (start_x < 1 || start_x > rows || start_y < 1 || start_y > columns
		|| end_x < 1 || end_x > rows || end_y < 1 || end_y > columns) {
		fprintf(stderr, "initialize table failed\n"); exit(-1);
	}
	TABLE* table = (TABLE*) malloc(sizeof(TABLE));
	if (!table) { fprintf(stderr, "malloc error\n"); exit(-1); }
	int i = 0, j = 0;
	table->arr = (TABLE_CELL**) malloc(sizeof(TABLE_CELL*) * (rows+1));
	if (!table->arr) { fprintf(stderr, "malloc error\n"); exit(-1); }
	for (i = 0; i <= rows; i ++) {
		table->arr[i] = (TABLE_CELL*) malloc(sizeof(TABLE_CELL) * (columns+1));
		if (!table->arr[i]) { fprintf(stderr, "malloc error\n"); exit(-1); }
		for (j = 0; j <= columns; j ++) {
			table->arr[i][j].score = 1;
			table->arr[i][j].passed = 0;
			table->arr[i][j].weight = 0;
		}
	}
	table->rows = rows; table->columns = columns;
//	table->start.x = end_x; table->start.y = end_y;
//	table->end.x = start_x; table->end.y = start_y;
	table->start.x = start_x; table->start.y = start_y;
	table->end.x = end_x; table->end.y = end_y;
	table->arr[table->start.x][table->start.y].score = -100;
	table->arr[table->start.x][table->start.y].passed = 1;
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
	if (x > 0 && x <= table->rows && y > 0 && y <= table->columns) {
		table->arr[x][y].score = weight;
	} else {
		fprintf(stderr, "set weight failed!\n"); exit(-1);
	}
}

void LIST_DEL(ROAD_ELE** plist, int x, int y) {
	ROAD_ELE* tmp = *plist;
	ROAD_ELE* pretmp = NULL;
	while (tmp) {
		if (tmp->point.x == x && tmp->point.y == y) {
			ROAD_ELE* dmp = tmp;
			if (tmp == *plist) { *plist = tmp->next; }
			else { pretmp->next = tmp->next; }
			pretmp = tmp; tmp = tmp->next;
			free(dmp);
		} else {
			pretmp = tmp; tmp = tmp->next;
		}
	}
}

// OK
int canbeupdate(TABLE* table, int x, int y, int sumweight, ROAD_ELE** plist) {
	if (x < 1 || y < 1) { return 0; }
	if (x > table->rows || y > table->columns) { return 0; }
	if (-100 == table->arr[x][y].score) { return 0; }
	if (0 == table->arr[x][y].passed) {
		table->arr[x][y].weight = table->arr[x][y].score + sumweight;
		return 1;
	}
	if (table->arr[x][y].weight < table->arr[x][y].score + sumweight) {
		LIST_DEL(plist, x, y);
		table->arr[x][y].weight = table->arr[x][y].score + sumweight;
		return 1;
	}
	return 0;
}

int FIND_ALREADY_IN_PATHS(ROAD_POINT* paths, int x, int y) {
	ROAD_POINT* tmp = paths;
	while (tmp) {
		if (tmp->x == x && tmp->y == y) { return 1; }
		tmp = tmp->next;
	}
	return 0;
}

void LIST_ADD(TABLE* table, ROAD_ELE** plist, int x, int y, ROAD_POINT* paths, 
	int preweight) {
	if (FIND_ALREADY_IN_PATHS(paths, x, y)) { return; }
	ROAD_ELE* re = (ROAD_ELE*) malloc(sizeof(ROAD_ELE));
	if (!re) { fprintf(stderr, "malloc error\n"); exit(-1); }
	re->point.x = x; re->point.y = y;
	re->sumweight = table->arr[x][y].score + preweight;
	ROAD_POINT* step = (ROAD_POINT*) malloc(sizeof(ROAD_POINT));
	if (!step) { fprintf(stderr, "malloc error\n"); exit(-1); }
	step->x = x; step->y = y; step->next = paths;
	re->paths = step;
	re->next = *plist;
	*plist = re;
	table->arr[x][y].passed = 1;
}

// OK
void add_four_direction(TABLE* table, ROAD_ELE* s, ROAD_ELE** plist) {
	if (canbeupdate(table, s->point.x, s->point.y+1, s->sumweight, plist)) 
		LIST_ADD(table, plist, s->point.x, s->point.y+1, s->paths,s->sumweight);
	if (canbeupdate(table, s->point.x+1, s->point.y, s->sumweight, plist)) 
		LIST_ADD(table, plist, s->point.x+1, s->point.y, s->paths,s->sumweight);
	if (canbeupdate(table, s->point.x, s->point.y-1, s->sumweight, plist)) 
		LIST_ADD(table, plist, s->point.x, s->point.y-1, s->paths,s->sumweight);
	if (canbeupdate(table, s->point.x-1, s->point.y, s->sumweight, plist)) 
		LIST_ADD(table, plist, s->point.x-1, s->point.y, s->paths,s->sumweight);
}

ROAD_ELE* LIST_POP(ROAD_ELE** plist) {
	ROAD_ELE* tl = (*plist);
	*plist = (*plist)->next;
	return tl;
}

// OK
ROAD_ELE* operator_forward_list(TABLE* table, ROAD_ELE* forward_list) {
	ROAD_ELE* new_list = NULL;
	while (forward_list) {
		ROAD_ELE* t = LIST_POP(&forward_list);
		add_four_direction(table, t, &new_list);
		free(t);
	}
	return new_list;
}

// OK
void pacman_shortestpath(TABLE* table) {
	ROAD_ELE* forward_list = NULL;
	ROAD_ELE s = (ROAD_ELE) { table->start, 0, 0, NULL };
	add_four_direction(table, &s, &forward_list);
	int max_circle = table->rows * table->columns;
	while (forward_list && -- max_circle) {
		forward_list = operator_forward_list(table, forward_list);
		if (table->arr[table->end.x][table->end.y].passed) { break; }
	}
	if (table->arr[table->end.x][table->end.y].passed) {
		while (forward_list) {
			ROAD_ELE* t = LIST_POP(&forward_list);
			if ((t->point.x == table->end.x) && (t->point.y == table->end.y)) {
				ROAD_POINT* paths = t->paths;
				int i = 0;
				while (paths) {
					if (0 != i) { printf(","); }
					printf("<%d,%d>", paths->x, paths->y); i ++;
					paths = paths->next;
				}
				printf("<%d,%d>", table->start.x, table->start.y);
				printf("\n");
				break;
			}
		}
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
	} while (1);

	printf("Output:\n");
	pacman_shortestpath(table);

	release_table(table);

	return 0;
}
