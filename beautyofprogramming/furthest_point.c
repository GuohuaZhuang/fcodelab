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
* @file furthest_point.c
* @brief find one furthest pair of points from points use exhaustion method.
* @author firstboy0513
* @version 0.0.1
* @date 2013-12-20
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
* @brief print out points.
*
* @param points points.
* @param size points length.
*/
void printout_points(const POINT* points, const int size) {
	int i = 0;
	for (i = 0; i < size; i ++) {
		if (0 != i) { printf(", "); }
		printf("(%d,%d)", points[i].x, points[i].y);
	}
	printf("\n");
}

/**
* @brief get distance use Euclidean distance.
*
* @param a point a.
* @param b point b.
*
* @return Euclidean distance.
*/
double get_distance(const POINT a, const POINT b) {
	return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

/**
* @brief find one furthest pair of points from points use exhaustion method.
*
* @param points points.
* @param size points size.
* @param pa the furthest first point.
* @param pb the furthest second point.
*
* @return the furthest distance.
*/
double find_furthest_point_exhaust(const POINT* points, const int size, 
	POINT* pa, POINT* pb) {
	int i = 0, j = 0; double max_distance = INT32_MIN, cur_distance = 0;
	if (!points || size <= 0 || !pa || !pb) {
		printf("[ERR] -- find_furthest_point_exhaust input invalid!\n");
		return max_distance;
	}
	for (i = 0; i < size; i ++) {
		for (j = i+1; j < size; j ++) {
			cur_distance = get_distance(points[i], points[j]);
			printf("cur_distance = %f\n", cur_distance);
			if (cur_distance > max_distance) {
				max_distance = cur_distance;
				*pa = points[i]; *pb = points[j];
			}
		}
	}
	return max_distance;
}

int main(int argc, const char *argv[])
{
//	const POINT points[] = { 
//		{8,12}, {17,11}, {6,5}, {12,3}, {13,8}, {15,16}, {2,10} };
//	const POINT points[] = {
//		{16,13}, {25,27}, {25,23}, {12,16}, {1,9}, {7,2}, {19,20}, {16,23}, 
//		{6,0}, {16,22}, {8,11}, {9,27}, {20,2}, {13,2}, {25,7}, {12,29}, 
//		{18,12}, {27,29}, {16,13}, {22,1} };
	const POINT points[] = {
		{6,0}, {8,11}, {12,16}, {16,22}, {27,29} };
	const int size = sizeof(points) / sizeof(points[0]);
	POINT a, b;

	double max_distance = find_furthest_point_exhaust(points, size, &a, &b);

	if (INT32_MIN != max_distance) {
		printf("maximum distance is %g\n", max_distance);
		printf("\ta(%d,%d) to b(%d,%d)\n", a.x, a.y, b.x, b.y);
	} else {
		printf("[ERR] -- some error occur to find nearest point.\n");
	}

	return 0;
}
