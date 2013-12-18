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
* @file nearest_point.c
* @brief find nearest point.
* @author firstboy0513
* @version 0.0.1
* @date 2013-12-18
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

/**
* @brief point has two dimension x and y.
*/
typedef struct _POINT {
	int x;
	int y;
} POINT;

/**
* @brief get distance use Euclidean distance.
*
* @param a point a.
* @param b point b.
*
* @return Euclidean distance.
*/
int get_distance(const POINT a, const POINT b) {
	return pow(a.x - b.x, 2) + pow(a.y - b.y, 2);
}

/**
* @brief find nearest point from points.
*
* @param points points.
* @param size points size.
* @param pa the nearest first point.
* @param pb the nearest second point.
*
* @return the nearest distance.
*/
int find_nearest_point_exhaust(const POINT* points, const int size, 
	POINT* pa, POINT* pb) {
	int i = 0, j = 0, min_distance = INT32_MAX, cur_distance = 0;
	if (!points || size <= 0 || !pa || !pb) {
		printf("[ERR] -- find_nearest_point_exhaust input invalid!\n");
		return min_distance;
	}
	for (i = 0; i < size; i ++) {
		for (j = i+1; j < size; j ++) {
			cur_distance = get_distance(points[i], points[j]);
			if (cur_distance < min_distance) {
				min_distance = cur_distance;
				*pa = points[i]; *pb = points[j];
			}
		}
	}
	return min_distance;
}

int main(int argc, const char *argv[])
{
	const POINT points[] = { {2,10}, {6,5}, {8,12}, {12,3}, {13,8}, {15,16}, 
		{17,11} };
	const int size = sizeof(points) / sizeof(points[0]);
	POINT a, b;
	int min_distance = find_nearest_point_exhaust(points, size, &a, &b);
	if (INT32_MAX != min_distance) {
		printf("minimum distance is %d\n", min_distance);
		printf("\ta(%d,%d) to b(%d,%d)\n", a.x, a.y, b.x, b.y);
	} else {
		printf("[ERR] -- some error occur to find nearest point.\n");
	}
	return 0;
}
