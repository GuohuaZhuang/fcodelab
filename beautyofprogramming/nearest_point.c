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
* @version 0.0.2
* @date 2013-12-18
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>

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
int get_distance(const POINT a, const POINT b) {
	return pow(a.x - b.x, 2) + pow(a.y - b.y, 2);
}

/**
* @brief find nearest point from points use exhaustion method.
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

/**
* @brief get minimum distance from left region, right region and across left 
* and right region.
*
* @param points points.
* @param from region start index.
* @param to region end index.
* @param pa output minimum distance of point a.
* @param pb output minimum distance of point b.
*
* @return minimum distance.
*/
int get_min_distance(const POINT* points, const int from, const int to, 
	POINT* pa, POINT* pb) {
	int i = 0, j = 0;
	int min_distance = INT32_MAX, middle = (from+to)/2, cur_distance = 0;
	if (to <= from) { return min_distance; }
	if (to - from == 1) {
		*pa = points[from]; *pb = points[to];
		min_distance = get_distance(points[from], points[to]);
		return min_distance;
	}
	// get the minimum distance in the left region
	POINT a_left, b_left; int min_distance_left = INT32_MAX;
	min_distance_left = get_min_distance(points,from,middle,&a_left,&b_left);
	if (min_distance_left < min_distance) {
		min_distance = min_distance_left; *pa = a_left; *pb = b_left;
	}
	// get the minimum distance in the right region
	POINT a_right, b_right; int min_distance_right = INT32_MAX;
	min_distance_right = get_min_distance(points,middle+1,to,&a_right,&b_right);
	if (min_distance_right < min_distance) {
		min_distance = min_distance_right; *pa = a_right; *pb = b_right;
	}
	// get the minimum distance across the left region and right region.
	int M = (points[middle].x + points[middle+1].x)/2;
	for (i = middle; i >= from; i --) {
		if (M - points[i].x > min_distance) { break; }
		for (j = middle+1; j <= to; j ++) {
			if (points[j].x - M > min_distance) { break; }
			cur_distance = get_distance(points[i], points[j]);
			if (cur_distance < min_distance) {
				min_distance = cur_distance;
				*pa = points[i]; *pb = points[j];
			}
		}
	}
	return min_distance;
}

#define SWAP(a,b) { (a) ^= (b); (b) ^= (a); (a) ^= (b); }
#define SWAP_POINT(p,q) { SWAP(p.x,q.x) SWAP(p.y,q.y) }

/**
* @brief use quick sort method to sort the points.
*
* @param points points.
* @param left left boundary.
* @param right right boundary.
*/
void quicksort_point(POINT* points, int left, int right) {
	int i = left, j = right, pivot = points[(i+j)/2].x;
	while (i <= j) {
		while (points[j].x > pivot) { j --; }
		while (points[i].x < pivot) { i ++; }
		if (i <= j) {
			if (i != j) { SWAP_POINT(points[i], points[j]) }
			i ++; j --;
		}
	}
	if (left < j) { quicksort_point(points, left, j); }
	if (right > i) { quicksort_point(points, i, right); }
}

/**
* @brief find nearest point from points use merger and recursive method.
* 1. sort the point by its x-axis value.
* 2. merger to compute minimum distance of the points.
*
* @param points points.
* @param size points size.
* @param pa the nearest first point.
* @param pb the nearest second point.
*
* @return the nearest distance.
*/
int find_nearest_point_merger(const POINT* points, const int size, 
	POINT* pa, POINT* pb) {
	int min_distance = INT32_MAX;
	if (!points || size <= 0 || !pa || !pb) {
		printf("[ERR] -- find_nearest_point_merger input invalid!\n");
		return min_distance;
	}
	// sort points first. As points is const, so copy it.
	POINT* points_copy = (POINT*) malloc(sizeof(POINT) * size);
	memcpy(points_copy, points, sizeof(POINT) * size);
	// printf("Before sort: "); printout_points(points_copy, size);
	quicksort_point(points_copy, 0, size-1);
	// printf("After sort:  "); printout_points(points_copy, size);
	min_distance = get_min_distance(points_copy, 0, size-1, pa, pb);
	free(points_copy);
	return min_distance;
}

int main(int argc, const char *argv[])
{
	const POINT points[] = { 
		{8,12}, {17,11}, {6,5}, {12,3}, {13,8}, {15,16}, {2,10} };
	const int size = sizeof(points) / sizeof(points[0]);
	POINT a, b;

	// int min_distance = find_nearest_point_exhaust(points, size, &a, &b);
	int min_distance = find_nearest_point_merger(points, size, &a, &b);

	if (INT32_MAX != min_distance) {
		printf("minimum distance is %d\n", min_distance);
		printf("\ta(%d,%d) to b(%d,%d)\n", a.x, a.y, b.x, b.y);
	} else {
		printf("[ERR] -- some error occur to find nearest point.\n");
	}
	return 0;
}
