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
* @file elevator_best_park.c
* @brief elevator best park floor.
* @author firstboy0513
* @version 0.0.1
* @date 2013-12-07
*/
#include <stdio.h>
#include <stdlib.h>

/**
* @brief elevator best park floor use exhaustion to calculate it.
*
* @param floor_person_number the person number in which floor they want to go.
* @param floor_length floor_person_number array length.
* @param pbest_floor_ladder output best floor ladder cost.
* @param calorie the ladder get up stair to use calorie as more weight to go 
* down stair.
*
* @return best floor park, or return -1 means error occur.
*/
int elevator_best_park_use_exhaustion(const int* floor_person_number, 
	const int floor_length, int* pbest_floor_ladder, int calorie) {
	if (!floor_person_number || floor_length <= 0) {
		printf("[ERR] -- elevator best park invalid input.\n");
		return -1;
	}
	int min_floor_ladder = -1, best_floor_park = 0;
	int i = 0, j = 0;
	for (i = 0; i < floor_length; i ++) { // assume park in the ith floor
		int current_floor_ladder = 0;
		for (j = 0; j < i; j ++) {
			current_floor_ladder += floor_person_number[j] * (i - j) * calorie;
		}
		for (j = i + 1; j < floor_length; j ++) {
			current_floor_ladder += floor_person_number[j] * (j - i);
		}
		if (-1 == min_floor_ladder 
			|| current_floor_ladder < min_floor_ladder) {
			min_floor_ladder = current_floor_ladder;
			best_floor_park = i;
		}
		printf("Now is park %dth floor, it cost %d floor ladder.\n", 
			i+1, current_floor_ladder);
	}
	if (min_floor_ladder >= 0 && pbest_floor_ladder) {
		*pbest_floor_ladder = min_floor_ladder;
	}
	return best_floor_park;
}

/**
* @brief elevator best park floor not use exhaustion.
* Assume now is calculate the xth floor, and the N1 means the bottom blow x 
* floor people number, and N2 means current xth floor people number, and N3 
* means the top in the head of x floor people number. And Y is the current xth
* floor will cost the floor ladders.
* So, if move to (x+1)th floor, then the floor ladder cost will be change to:
* 	Y + N1 + N2 - N3   ==>   Y + (N1 + N2 - N3)
* and if move to (x-1)th floor, the the floor ladder cost will be change to:
* 	Y - N1 + N2 + N3   ==>   Y - (N1 - N2 - N3)
* So, if (N1 + N2 - N3) < 0, it means (x+1)th floor is better than xth floor.
* And if (N1 - N2 - N3) > 0, it means (x-1)th fllor is better than xth floor.
* We know the beginning floor begin from 1, there maybe (N1 + N2 - N3) < 0, 
* so we just traversal from it, util it come to be (N1 + N2 - N3) >= 0, then 
* do not traversal, just stop it, we can get the best ladder floor.
* This method just cost O(N) times.
*
* @param floor_person_number the person number in which floor they want to go.
* @param floor_length floor_person_number array length.
* @param pbest_floor_ladder output best floor ladder cost.
* @param calorie the ladder get up stair to use calorie as more weight to go 
* down stair.
*
* @return best floor park, or return -1 means error occur.
*/
int elevator_best_park_no_exhaustion(const int* floor_person_number, 
	const int floor_length, int* pbest_floor_ladder, int calorie) {
	if (!floor_person_number || floor_length <= 0) {
		printf("[ERR] -- elevator best park invalid input.\n");
		return -1;
	}
	int best_floor_park = 0;
	int N1 = 0, N2 = 0, N3 = 0, Y = 0;
	// set the N3 and beginning Y first
	int i = 0;
	for (N2 = floor_person_number[0], i = 1; i < floor_length; i ++) {
		N3 += floor_person_number[i];
		Y += floor_person_number[i] * (i - 0);
	}
	for (i = 1; i < floor_length; i ++) {
		if (calorie * (N1 + N2) >= N3) {
			break;
		}
		best_floor_park = i;
		Y += (calorie * (N1 + N2) - N3);
		N1 += N2;
		N2 = floor_person_number[i];
		N3 -= floor_person_number[i];
	}
	if (Y >= 0 && pbest_floor_ladder) {
		*pbest_floor_ladder = Y;
	}
	return best_floor_park;
}

int main(int argc, const char *argv[])
{
	int floor_person_number[] = {0, 2, 3, 2, 9, 2, 1, 7, 2, 4, 2};
	int best_floor_ladder = -1;
//	int best_floor_park = elevator_best_park_use_exhaustion(floor_person_number, 
//		sizeof(floor_person_number)/sizeof(floor_person_number[0]), 
//		&best_floor_ladder, 2);
	int best_floor_park = elevator_best_park_no_exhaustion(floor_person_number, 
		sizeof(floor_person_number)/sizeof(floor_person_number[0]), 
		&best_floor_ladder, 2);
	printf("best floor to park is %d, and its ladder cost %d\n", 
		best_floor_park+1, best_floor_ladder);
	return 0;
}
