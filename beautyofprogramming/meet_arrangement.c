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
* @file meet_arrangement.c
* @brief meet arrangement.
* @author firstboy0513
* @version 0.0.1
* @date 2014-01-07
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define MAX_SELECT 3

/**
* @brief get max meet value.
*
* @param student_meets[][MAX_SELECT]
* @param student_size
*
* @return return max meet value.
*/
int get_max_meet(const int student_meets[][MAX_SELECT], 
	const int student_size) {
	int i = 0, j = 0, max_meet = INT32_MIN;
	for (i = 0; i < student_size; i ++) {
		for (j = 0; j < MAX_SELECT; j ++) {
			if (max_meet < student_meets[i][j]) {
				max_meet = student_meets[i][j];
			}
		}
	}
	return max_meet;
}

/**
* @brief get all meets unique.
*
* @param student_meets[][MAX_SELECT] student select meets.
* @param student_size student count.
* @param p_meets_size output meets size point.
*
* @return return uniqued meets array, be sure to free outside.
*/
int* get_all_meets(const int student_meets[][MAX_SELECT], 
	const int student_size, int* p_meets_size) {
	if (student_size <= 0) { return NULL; }
	(*p_meets_size) = get_max_meet(student_meets, student_size) + 1;
	int* meets = (int*) malloc(sizeof(int) * (*p_meets_size));
	memset(meets, 0, sizeof(int) * (*p_meets_size));
	return meets;
}

/**
* @brief build student meet select array.
*
* @param student_meets[][MAX_SELECT]
* @param student_size
* @param meets
* @param meets_size
*
* @return return select array.
*/
int** build_student_meet_select(const int student_meets[][MAX_SELECT], 
	const int student_size, const int* meets, const int meets_size) {
	int** select = (int**) malloc(sizeof(int*) * meets_size);
	int i = 0, j = 0;
	for (i = 0; i < meets_size; i ++) {
		select[i] = (int*) malloc(sizeof(int) * student_size);
		for (j = 0; j < student_size; j ++) {
			select[i][j] = 0;
		}
	}
	for (j = 0; j < student_size; j ++) {
		for (i = 0; i < MAX_SELECT; i ++) {
			select[student_meets[j][i]][j] = 1;
		}
	}
	return select;
}

/**
* @brief judge two meet whether has confilct.
*
* @param select select.
* @param meets_size meets_size as row size in select array.
* @param student_size student_size as column size in select array.
* @param a one meet a.
* @param b another meet b.
*
* @return return 1 means a and b has confilct, otherwise return 0 means no 
* confilct.
*/
int has_confilct(int** select, const int meets_size, 
	const int student_size, int a, int b) {
	int j = 0;
	for (j = 0; j < student_size; j ++) {
		if (select[a][j] == 1 && select[b][j] == 1) {
			return 1;
		}
	}
	return 0;
}

/**
* @brief release select array.
*
* @param select select.
* @param size select one dimension size.
*/
void release_select_array(int** select, const int size) {
	int i = 0;
	for (i = 0; i < size; i ++) {
		free(select[i]);
	}
	free(select);
}

/**
* @brief output meets array.
*
* @param meets meets array.
* @param size meets count.
*/
void output_meets(const int* meets, const int size) {
	int i = 0;
	for (i = 1; i < size; i ++) {
		if (1 != i) { printf(", "); }
		printf("%d", meets[i]);
	}
	printf("\n");
}

/**
* @brief output meet arrangement.
* Just scan the setted meets before from i-1 to 1. If has confilct, then scan
* next k, which is the time from 1 to current minimum_time.
*
* @param student_meets[][MAX_SELECT]
* @param student_size
*/
void output_arrangement(const int student_meets[][MAX_SELECT], 
	int student_size) {
	int meets_size = 0;
	int* meets = get_all_meets(student_meets, student_size, &meets_size);
	int** select = build_student_meet_select(student_meets, student_size, 
		meets, meets_size);
	meets[1] = 1;
	int i = 0, j = 0, k = 0, minimum_time = 1, confilct = 0;
	for (i = 2; i < meets_size; i ++) {
		for (k = 1; k <= minimum_time; k ++) {
			for (j = i-1; j > 0; j --) {
				if (meets[j] != k) { continue; }
				confilct = has_confilct(select, meets_size, student_size, i, j);
				if (confilct) { break; }
			}
			if (j <= 0) { meets[i] = k; break; }
		}
		if (k > minimum_time) { meets[i] = ++minimum_time; }
	}
	output_meets(meets, meets_size);
	release_select_array(select, meets_size);
	free(meets);
	printf("minimum_time = %d\n", minimum_time);
}

/**
* @brief test case for output arrangement.
*/
void testcase_output_arrangement() {
	const int student_meets[][MAX_SELECT] = {
		{1, 2, 3}, {1, 3, 4}
	};
	const int student_size = sizeof(student_meets) / sizeof(student_meets[0]);
	output_arrangement(student_meets, student_size);
}

/**
* @brief meet time interval from begin to end.
*/
typedef struct _MEET_TIME {
	int begin;
	int end;
} MEET_TIME;

/**
* @brief time type, begin and end.
*/
typedef enum { _BEGIN, _END } TIME_TYPE;

/**
* @brief meet time struct with time value and type begin and end.
*/
typedef struct _MTIME {
	int time;
	TIME_TYPE type;
} MTIME;

/**
* @brief get meet time struct.
*
* @param meets meets.
* @param meets_size meets size.
*
* @return return mtime struct array.
*/
MTIME* get_mtimes(const MEET_TIME* meets, const int meets_size) {
	if (!meets || meets_size <= 0) { return NULL; }
	MTIME* mtimes = (MTIME*) malloc(sizeof(MTIME) * meets_size * 2);
	int i = 0;
	for (i = 0; i < meets_size; i ++) {
		mtimes[2*i] = (MTIME){ meets[i].begin, _BEGIN };
		mtimes[2*i+1] = (MTIME){ meets[i].end, _END };
	}
	return mtimes;
}

/**
* @brief add a.type < b.type let if a.time == b.time, then order the _END first
*/
#define MTIME_GT(a, b) (a.time > b.time ? 1 :  \
	(a.time == b.time ? (a.type < b.type) : 0))
#define SWAP_MTIME(a, b) { MTIME _t = a; a = b; b = _t; }

/**
* @brief mtime quick sort.
*
* @param mtimes mtimes.
* @param left left index for quick sort.
* @param right right index for quick sort.
*/
void mtime_quicksort(MTIME* mtimes, const int left, const int right) {
	int i = left, j = right;
	MTIME pivot = mtimes[left+(right-left)/2];
	while (i <= j) {
		while (MTIME_GT(mtimes[j], pivot)) { j --; }
		while (MTIME_GT(pivot, mtimes[i])) { i ++; }
		if (i <= j) {
			if (i != j) { SWAP_MTIME(mtimes[i], mtimes[j]) }
			i ++, j --;
		}
	}
	if (left < j) { mtime_quicksort(mtimes, left, j); }
	if (i < right) { mtime_quicksort(mtimes, i, right); }
}

/**
* @brief test case for output arrangement with head and tail meets.
*/
void testcase_output_arrangement_headandtail() {
	const MEET_TIME meets[] = {
		{1, 5}, {2, 3}, {3, 4}, {3, 6}
	};
	const int meets_size = sizeof(meets) / sizeof(meets[0]);
	MTIME* mtimes = get_mtimes(meets, meets_size);
	const int mtimes_size = meets_size*2;
	mtime_quicksort(mtimes, 0, mtimes_size-1);
	int i = 0, curcolor = 0, maxcolor = 0;
	for (i = 0; i < mtimes_size; i ++) {
		if (mtimes[i].type == _BEGIN) {
			curcolor ++;
			if (maxcolor < curcolor) { maxcolor = curcolor; }
		} else {
			curcolor --;
		}
	}
	printf("max color use in head and tail meets arrangement = %d\n", maxcolor);
	free(mtimes);
}

int main(int argc, const char *argv[])
{
	testcase_output_arrangement();
	testcase_output_arrangement_headandtail();

	return 0;
}
