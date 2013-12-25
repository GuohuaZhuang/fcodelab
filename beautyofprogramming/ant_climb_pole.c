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
* @file ant_climb_pole.c
* @brief caculate all ant leave pole time.
* @author firstboy0513
* @version 0.0.1
* @date 2013-12-25
*/
#include <stdio.h>
#include <stdlib.h>

/**
* @brief get extremum (minimum leave time and maximum heave time) of all ants 
* leave from pole.
*
* @param positions all ants positions in pole.
* @param size ants count.
* @param speed ant move speed.
* @param pole pole length.
* @param pminleave output minimum leave time.
* @param pmaxleave output maximum leave time.
*/
void get_extremum_leavetime(const int* positions, const int size, 
	const int speed, const int pole, int* pminleave, int* pmaxleave) {
	if (!positions || size <= 0 || speed <= 0 || pole <= 0 
			|| !pminleave || !pmaxleave) {
		printf("[ERR] -- get_extremum_leavetime input invalid!\n");
		return;
	}
	int i = 0, end = 0;
	for (i = 0; i < size; i ++) {
		end = pole - positions[i];
		if (positions[i] < end) {
			if (*pminleave < positions[i]) { *pminleave = positions[i]; }
			if (*pmaxleave < end) { *pmaxleave = end; }
		} else {
			if (*pminleave < end) { *pminleave = end; }
			if (*pmaxleave < positions[i]) { *pmaxleave = positions[i]; }
		}
	}
	*pminleave /= speed, *pmaxleave /= speed;
}

int main(int argc, const char *argv[])
{
	const int ants_position[] = {3, 7, 11, 17, 23};
	const int ants_count = sizeof(ants_position) / sizeof(ants_position[0]);
	const int pole_length = 27;
	int min_leavetime = 0, max_leavetime = 0;
	get_extremum_leavetime(ants_position, ants_count, 1, pole_length,
		&min_leavetime, &max_leavetime);
	printf("min_leavetime = %d, max_leavetime = %d\n", 
		min_leavetime, max_leavetime);
	return 0;
}
