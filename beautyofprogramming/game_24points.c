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
* @file game_24points.c
* @brief 24 points game.
* @author firstboy0513
* @version 0.0.1
* @date 2013-12-28
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>

#define POINT_COUNT 4
#define MAX_NUMBER 13
#define END_RESULT 24
#define ACCURACY_THRESHOLD 1E-6
#define MAX_RESULT_LENGTH ((2+4)*3+1)

/**
* @brief points game use exhaustion method.
*
* @param numbers numbers.
* @param size numbers count.
* @param results[][MAX_RESULT_LENGTH] string express results.
*
* @return return 1 if found result success, otherwise return 0 means not found.
*/
int points_game(double* numbers, int size, char results[][MAX_RESULT_LENGTH]) {
	if (0 >= size) { return 0; }
	if (1 == size) {
		if (fabs(numbers[0] - END_RESULT) < ACCURACY_THRESHOLD) {
			printf("Result: %d = %s\n", END_RESULT, results[0]); return 1;
		} else { return 0; }
	}
	int i = 0, j = 0;
	double a = 0, b = 0;
	char expa[MAX_RESULT_LENGTH] = {0}, expb[MAX_RESULT_LENGTH] = {0};
	for (i = 0; i < size; i ++) {
		for (j = i+1; j < size; j ++) {
			a = numbers[i]; b = numbers[j];
			strcpy(expa, results[i]); strcpy(expb, results[j]);
			if (j != size-1) {
				numbers[j] = numbers[size-1];
				strcpy(results[j], results[size-1]);
			}
			// + add
			sprintf(results[i], "(%s+%s)", expa, expb); numbers[i] = a + b;
			if (points_game(numbers, size-1, results)) { return 1; }
			// - subtract
			sprintf(results[i], "(%s-%s)", expa, expb); numbers[i] = a - b;
			if (points_game(numbers, size-1, results)) { return 1; }
			sprintf(results[i], "(%s-%s)", expb, expa); numbers[i] = b - a;
			if (points_game(numbers, size-1, results)) { return 1; }
			// * multiply
			sprintf(results[i], "(%s*%s)", expa, expb); numbers[i] = a * b;
			if (points_game(numbers, size-1, results)) { return 1; }
			// / division
			if (0 != b) {
				sprintf(results[i], "(%s/%s)", expa, expb); numbers[i] = a / b;
				if (points_game(numbers, size-1, results)) { return 1; }
			}
			if (0 != a) {
				sprintf(results[i], "(%s/%s)", expb, expa); numbers[i] = b / a;
				if (points_game(numbers, size-1, results)) { return 1; }
			}
			numbers[i] = a; numbers[j] = b;
			strcpy(results[i], expa); strcpy(results[j], expb);
		}
	}
	return 0;
}

/**
* @brief output all point numbers.
*
* @param numbers numbers array.
* @param size numbers count.
*/
void output_points(const double* numbers, const int size) {
	int i = 0;
	printf("points: ");
	for (i = 0; i < size; i ++) {
		if (0 != i) { printf(", "); }
		printf("%g", numbers[i]);
	}
	printf("\n");
}

/**
* @brief (4,1) also know as C(4, 1) (1-3) combinations and C(4, 2) (2-2) 
* combinations maximum size. one (3) combination is use C(3, 2) * (1) * 6 * (6).
* another (2) combination is use C(2, 2) is use C(2, 2) * (6) * 6 * (6).
* So the max fork number is C(4, 1) * C(3, 2)*1*6*6 + C(4, 2) * C(2,2)*6*6*6.
* It is equal 4*3*1*6*6 + 6*1*6*6*6 = 12*6*6 + 6*6*6*6 = 8*6*6*6 = 1728.
*/
#define MAX_FORK_NUMBER 1728

/**
* @brief every combine result is a number and correspond a string result.
*/
typedef struct _ELE {
	double number;
	char result[MAX_RESULT_LENGTH];
} ELE;

/**
* @brief one FORK is means has a list of combine numbers.
*/
typedef struct _FORK {
	ELE values[MAX_FORK_NUMBER];
	int count;
} FORK;

/**
* @brief unique the fork elements by its same numbers.
*
* @param pfork fork point.
*/
void fork_unique(FORK* pfork) {
	int i = 0, j = 0, count = 0, unique = 0;
	ELE values[MAX_FORK_NUMBER];
	for (i = 0, count = 0; i < pfork->count; i ++) {
		unique = 1;
		for (j = 0; j < count; j ++) {
			if (fabs(pfork->values[i].number - values[j].number) 
				< ACCURACY_THRESHOLD) {
				unique = 0; break;
			}
		}
		if (unique) {
			values[count].number = pfork->values[i].number;
			memcpy(values[count ++].result, pfork->values[i].result, 
				MAX_RESULT_LENGTH);
		}
	}
	pfork->count = count;
	for (i = 0; i < count; i ++) {
		pfork->values[i].number = values[i].number;
		memcpy(pfork->values[i].result, values[i].result, MAX_RESULT_LENGTH);
	}
}

/**
* @brief combine 6*m*n use 6 probability combinations.
* The combinations are: (a+b), (a-b), (b-a), (a*b), (a/b), (b/a).
*
* @param pa fork a point.
* @param pb fork b point.
* @param values values to store ELE array result.
*
* @return return combinations ELE count.
*/
int fork_comb(FORK* pa, FORK* pb, ELE* values) {
	int i = 0, j = 0, count = 0;
	for (i = 0; i < pa->count; i ++) {
		for (j= 0; j < pb->count; j ++) {
			double ea = pa->values[i].number, eb = pb->values[j].number;
			char* sa = pa->values[i].result; char* sb = pb->values[j].result;

			sprintf(values[count].result, "(%s+%s)", sa, sb);
			values[count ++].number = (ea + eb);

			sprintf(values[count].result, "(%s-%s)", sa, sb);
			values[count ++].number = (ea - eb);

			sprintf(values[count].result, "(%s-%s)", sb, sa);
			values[count ++].number = (eb - ea);

			sprintf(values[count].result, "(%s*%s)", sa, sb);
			values[count ++].number = (ea * eb);

			if (eb != 0) {
				sprintf(values[count].result, "(%s/%s)", sa, sb);
				values[count ++].number = (ea / eb);
			}
			if (ea != 0) {
				sprintf(values[count].result, "(%s/%s)", sb, sa);
				values[count ++].number = (eb / ea);
			}
		}
	}
	return count;
}

/**
* @brief fork union, combine tow fork, and then unique it.
*
* @param pfork the result fork point.
* @param pa one fork to combine.
* @param pb another fork to combine.
*/
void fork_union(FORK* pfork, FORK* pa, FORK* pb) {
	ELE values[MAX_FORK_NUMBER];
	int count = fork_comb(pa, pb, values);
	int i = 0;
	for (i = 0; i < count; i ++) {
		pfork->values[pfork->count] = values[i];
		pfork->count ++;
	}
	fork_unique(pfork);
}

/**
* @brief fork recursion to get the fork result. If the x index correspond FORK
* is calculated, then return it, otherwise use fork union to calculate it.
*
* @param S the total FORK array point.
* @param x x index.
*
* @return return the result FORK element point.
*/
FORK* fork_recursion(FORK* S, int x) {
	if (S[x].count > 0) { return (FORK*)(S+x); }
	int i = 1;
	for (i = 1; i < x; i ++) {
		if ((i & x) == i) {
			fork_union((FORK*)(S+x), fork_recursion(S, i), 
				fork_recursion(S, x-i));
		}
	}
	return (FORK*)(S+x);
}

/**
* @brief fork check whether has result END_RESULT element, if has return 1 and 
* print out the result.
*
* @param x input FORK.
*
* @return return 1 if found result success, otherwise return 0 means not found.
*/
int fork_check(FORK fork) {
	int i = 0;
	for (i = 0; i < fork.count; i ++) {
		if (fabs(fork.values[i].number - END_RESULT) < ACCURACY_THRESHOLD) {
			printf("fork Result: %d = %s\n", END_RESULT, fork.values[i].result);
			return 1;
		}
	}
	return 0;
}

/**
* @brief output fork just use for debug fork informations.
*
* @param fork fork point.
*/
void output_fork(FORK* pfork) {
	int i = 0;
	for (i = 0; i < pfork->count; i ++) {
		printf("%s=%g\n", pfork->values[i].result, pfork->values[i].number);
	}
}

/**
* @brief points game use fork method.
*
* @param numbers numbers.
* @param size numbers count.
* @param results[][MAX_RESULT_LENGTH] string express results.
*
* @return return 1 if found result success, otherwise return 0 means not found.
*/
int points_game_use_fork(double* numbers, int size) {
	if (!numbers || size <= 0) { return 0; }
	// init S to empty set
	int max_fork = pow(2, size) - 1;
	FORK* S = (FORK*) malloc(sizeof(FORK) * (max_fork+1));
	if (!S) { printf("[ERR] -- malloc error!\n"); return 0; }
	int i = 0, index = 0;
	for (i = 1; i <= max_fork; i ++) {
		S[i].count = 0;
		memset(S[i].values, 0, sizeof(S[i].values));
	}

	// init S[2^n] to single value
	for (i = 0; i < size; i ++) {
		index = pow(2, i);
		S[index].values[0].number = numbers[i];
		S[index].count = 1;
		sprintf(S[index].values[0].result, "%g", numbers[i]);
	}

	// fork recursion
	for (i = 1; i <= max_fork; i ++) {
		fork_recursion(S, i);
	}

	// check whether has END_RESULT in last FORK element
	if (fork_check(S[max_fork])) {
		free(S); return 1;
	}
	free(S); return 0;
}

int main(int argc, const char *argv[])
{
	double numbers[POINT_COUNT] = {0};
	double numbers_copy[POINT_COUNT] = {0};
	char results[POINT_COUNT][MAX_RESULT_LENGTH] = {{0}};
	char results_copy[POINT_COUNT][MAX_RESULT_LENGTH] = {{0}};
	int i = 0;

// #define DEBUG
#ifndef DEBUG
	srand(time(NULL));
	for (i = 0; i < POINT_COUNT; i ++) {
		numbers[i] = (rand() % MAX_NUMBER) + 1;
		sprintf(results[i], "%g", numbers[i]);
		numbers_copy[i] = numbers[i];
		memcpy(results_copy[i], results[i], MAX_RESULT_LENGTH);
	}
#else // DEBUg
	int _debug[] = {
//		5, 5, 5, 1
//		3, 3, 7, 7
//		3, 3, 8, 8
//		1, 4, 5, 6
//		3, 8, 8, 10
//		4, 4, 10, 10
//		9, 9, 6, 2
//		3, 2, 2, 2
		11, 2, 12, 4
	};
	for (i = 0; i < POINT_COUNT; i ++) {
		numbers[i] = _debug[i];
		sprintf(results[i], "%g", numbers[i]);
		numbers_copy[i] = numbers[i];
		memcpy(results_copy[i], results[i], MAX_RESULT_LENGTH);
	}
#endif // DEBUG

	// use recursion method to get game result.
	output_points(numbers, POINT_COUNT);
	if (points_game(numbers, POINT_COUNT, results)) {
		printf("24 points game is success to find result.\n");
	} else {
		printf("24 points game numbers is failed combine.\n");
	}

	// use fork method to get game result.
	for (i = 0; i < POINT_COUNT; i ++) {
		numbers[i] = numbers_copy[i];
		sprintf(results[i], "%s", results_copy[i]);
	}

	output_points(numbers, POINT_COUNT);
	if (points_game_use_fork(numbers, POINT_COUNT)) {
		printf("24 points game is success to find result.\n");
	} else {
		printf("24 points game numbers is failed combine.\n");
	}

	return 0;
}
