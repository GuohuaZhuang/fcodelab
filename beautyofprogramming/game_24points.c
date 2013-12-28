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

int main(int argc, const char *argv[])
{
	double numbers[POINT_COUNT] = {0};
	char results[POINT_COUNT][MAX_RESULT_LENGTH] = {{0}};
	int i = 0;
	srand(time(NULL));
	for (i = 0; i < POINT_COUNT; i ++) {
		numbers[i] = (rand() % MAX_NUMBER) + 1;
		sprintf(results[i], "%g", numbers[i]);
	}
	output_points(numbers, POINT_COUNT);
	if (points_game(numbers, POINT_COUNT, results)) {
		printf("24 points game is success to find result.\n");
	} else {
		printf("24 points game numbers is failed combine.\n");
	}
	return 0;
}
