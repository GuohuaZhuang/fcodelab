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
*
* @return best floor park.
*/
int elevator_best_park_use_exhaustion(const int* floor_person_number, 
	const int floor_length, int* pbest_floor_ladder) {
	int min_floor_ladder = -1, best_floor_park = 0;
	int i = 0, j = 0;
	for (i = 0; i < floor_length; i ++) { // assume park in the ith floor
		int current_floor_ladder = 0;
		for (j = 0; j < i; j ++) {
			current_floor_ladder += floor_person_number[j] * (i - j);
		}
		for (j = i + 1; j < floor_length; j ++) {
			current_floor_ladder += floor_person_number[j] * (j - i);
		}
		if (-1 == min_floor_ladder 
			|| current_floor_ladder < min_floor_ladder) {
			min_floor_ladder = current_floor_ladder;
			best_floor_park = i;
		}
		// printf("Now is park %dth floor, it cost %d floor ladder.\n", 
		// 	i+1, current_floor_ladder);
	}
	if (min_floor_ladder >= 0 && *pbest_floor_ladder) {
		*pbest_floor_ladder = min_floor_ladder;
	}
	return best_floor_park;
}

int main(int argc, const char *argv[])
{
	int floor_person_number[] = {0, 2, 3, 2, 9, 2, 1, 7, 2, 4, 2};
	int best_floor_ladder = -1;
	int best_floor_park = elevator_best_park_use_exhaustion(floor_person_number, 
		sizeof(floor_person_number)/sizeof(floor_person_number[0]), 
		&best_floor_ladder);
	printf("best floor to park is %d, and its ladder cost %d\n", 
		best_floor_park+1, best_floor_ladder);
	return 0;
}
