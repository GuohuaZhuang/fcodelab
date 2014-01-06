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
* @file light_cutting_plane.c
* @brief light cutting plane problem.
* @author firstboy0513
* @version 0.0.1
* @date 2014-01-05
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
* @brief light line express as y=ax+b, we just store the argument a and b.
*/
typedef struct _LIGHT_LINE {
	double a;
	double b;
} LIGHT_LINE;

/**
* @brief intersect point struct with x-axis and y-axis value.
*/
typedef struct _INTERSECT_POINT {
	double x;
	double y;
} INTERSECT_POINT;

/**
* @brief get two line intersection point.
*
* @param l1 line 1.
* @param l2 line 2.
* @param ppoint output intersection point.
*
* @return return 1 means has intersection point, otherwise return 0 means no 
* intersection point.
*/
int line_intersection(const LIGHT_LINE l1, const LIGHT_LINE l2, 
	INTERSECT_POINT* ppoint) {
	if (!ppoint) {
		printf("[ERR] -- line_intersection input ppoint is none!\n");
		return 0;
	}
	if (l1.a != l2.a) {
		(*ppoint).x = (l2.b-l1.b)/(l1.a-l2.a);
		(*ppoint).y = l1.a * (*ppoint).x + l1.b;
		return 1;
	}
	return 0;
}

/**
* @brief get all lines intersection points.
*
* @param lines input lines.
* @param size lines size.
* @param ppoints output all intersection points, you should be sure to free the
* memory outside.
*
* @return return intersection points count.
*/
int lines_all_intersections(const LIGHT_LINE* lines, const int size, 
	INTERSECT_POINT** ppoints) {
	(*ppoints) = NULL;
	int max_intersection = size*(size-1)/2;
	(*ppoints) = (INTERSECT_POINT*) 
		malloc(sizeof(INTERSECT_POINT) * max_intersection);
	INTERSECT_POINT tmppoint = {0.0, 0.0};
	int i = 0, j = 0, k = 0;
	for (i = 0; i < size-1; i ++) {
		for (j = i+1; j < size; j ++) {
			if (line_intersection(lines[i], lines[j], &tmppoint) > 0) {
				(*ppoints)[k++] = tmppoint;
			}
		}
	}
	return k;
}

/**
* @brief count points in x-axis interval.
* Note that not include the point in the boundarys, as we just want to know how 
* many plane area are split.
*
* @param points points.
* @param size points count.
* @param left left axis interval boundary.
* @param right right axis interval boundary.
*
* @return return the count of points in x-axis interval.
*/
int pointscount_in_xaxis_interval(const INTERSECT_POINT* points, const int size, 
	const double left, const double right) {
	int i = 0, count = 0;
	for (i = 0; i < size; i ++) {
		if (points[i].x > left && points[i].x < right) {
			count ++;
		}
	}
	return count;
}

/**
* @brief caculate the count of split plane by lines in given x-axis interval.
* Be sure that no lines parallel with y-axis, and also not any intersection 
* point is shared more than 2 lines.
*
* @param lines lines.
* @param size lines size.
* @param xA x-axis left boundary.
* @param xB x-axis right boundary.
*
* @return return the split plane count in interval.
*/
int light_cutting_plane_count(const LIGHT_LINE* lines, const int size, 
	const double xA, const double xB) {
	INTERSECT_POINT* points = NULL;
	int points_size = lines_all_intersections(lines, size, &points);
	int points_count_interval = pointscount_in_xaxis_interval(
		points, points_size, xA, xB);
	if (points) { free(points); }
	return (size + points_count_interval + 1);
}

/**
* @brief projector in x-axis boundary of left and right.
*/
typedef struct _BOUNDARY {
	double left;
	double right;
} BOUNDARY;

/**
* @brief get lines interval boundaryst.
* Note the boundarys memory will be free outside.
*
* @param lines lines.
* @param size lines size.
* @param xA interval left boundary.
* @param xB interval right boundary.
*
* @return return boundarys, if input invalid, return null.
*/
BOUNDARY* get_lines_interval_boundary(const LIGHT_LINE* lines, 
	const int size, const double xA, const double xB) {
	if (!lines || size <= 0) { return NULL; }
	BOUNDARY* boundarys = (BOUNDARY*) malloc(sizeof(BOUNDARY) * size);
	int i = 0;
	for (i = 0; i < size; i ++) {
		boundarys[i].left = lines[i].a * xA + lines[i].b;
		boundarys[i].right = lines[i].a * xB + lines[i].b;
	}
	return boundarys;
}

#define BOUNDARY_GT(a, b) ( (a.left > b.left ? 1 :  \
	(a.left == b.left ? (a.right > b.right) : 0)))
#define SWAP_BOUNDARY(a, b) { BOUNDARY _t = a; a = b; b = _t; }

/**
* @brief quick sort the boundary use its left boundary value.
* Note that if boundary left is equal, then compare right value, let if left 
* start the same point, then do not to calculate one more intersection point.
*
* @param boundarys boundarys.
* @param left quick sort left index.
* @param right quick sort right index.
*/
void quicksort_boundary(BOUNDARY* boundarys, const int left, const int right) {
	int i = left, j = right;
	BOUNDARY pivot = boundarys[(left+right)/2];
	while (i <= j) {
		while (BOUNDARY_GT(boundarys[j], pivot)) { j --; }
		while (BOUNDARY_GT(pivot, boundarys[i])) { i ++; }
		if (i <= j) {
			if (i != j) { SWAP_BOUNDARY(boundarys[i], boundarys[j]); }
			j --, i ++;
		}
	}
	if (left < j) { quicksort_boundary(boundarys, left, j); }
	if (i < right) { quicksort_boundary(boundarys, i, right); }
}

/**
* @brief merge the left ordered and right ordered to caculate reverse number.
*
* @param boundarys boundarys.
* @param left boundarys left index.
* @param mid boundarys middle index.
* @param right boundarys right index.
* @param tmp tmp array.
*
* @return reverse number.
*/
int merge_reversenumber(BOUNDARY* boundarys, const int left, const int mid, 
	const int right, BOUNDARY* tmp) {
	memcpy(tmp+left, boundarys+left, sizeof(BOUNDARY) * (right-left+1));
	int i = left, j = mid+1, k = left, reversenumber = 0;
	while (i <= mid && j <= right) {
		if (tmp[i].right <= tmp[j].right) {
			boundarys[k++] = tmp[i++];
		} else {
			boundarys[k++] = tmp[j++];
			reversenumber += (mid-i+1);
		}
	}
	if (i <= mid) {
		memcpy(boundarys+k, tmp+i, sizeof(BOUNDARY) * (mid-i+1));
	}
	if (j <= right) {
		memcpy(boundarys+k, tmp+j, sizeof(BOUNDARY) * (right-j+1));
	}
	return reversenumber;
}

/**
* @brief merge sort to calculate reverse number.
*
* @param boundarys boundarys.
* @param left boundary left index.
* @param right boundary right index.
* @param tmp tmp array.
*
* @return reverse number.
*/
int mergesort_reversenumber(BOUNDARY* boundarys, const int left,
	const int right, BOUNDARY* tmp) {
	if (right - left < 1) { return 0; }
	int reversenumber = 0, mid = (left+right)/2;
		reversenumber += mergesort_reversenumber(boundarys, left, mid, tmp);
		reversenumber += mergesort_reversenumber(boundarys, mid+1, right, tmp);
		reversenumber += merge_reversenumber(boundarys, left, mid, right, tmp);
	return reversenumber;
}

/**
* @brief caculate the count of split plane by lines in given x-axis interval 
* use reverse number method.
* Be sure that no lines parallel with y-axis, and also not any intersection 
* point is shared more than 2 lines.
* You can also see the test lines plot in google:
*   http://www.google.com/#newwindow=1&q=y%3D1.5x%2B1+and+y%3D2x%2B2+and+y+%3D+1
*
* @param lines lines.
* @param size lines size.
* @param xA x-axis left boundary.
* @param xB x-axis right boundary.
*
* @return return the split plane count in interval.
*/
int light_cutting_plane_count_use_reversenumber(const LIGHT_LINE* lines, 
	const int size, const double xA, const double xB) {
	BOUNDARY* boundarys = get_lines_interval_boundary(lines, size, xA, xB);
	quicksort_boundary(boundarys, 0, size-1);
	BOUNDARY* boundarys_copy = (BOUNDARY*) malloc(sizeof(BOUNDARY) * size);
	int reversenumber = mergesort_reversenumber(boundarys, 0, size-1, 
		boundarys_copy);
	free(boundarys_copy); free(boundarys);
	return (size + reversenumber + 1);
}

int main(int argc, const char *argv[])
{
	// x-axis interval from A to B, be sure A < B.
	const double xA = -2.0, xB = 1.0;
	LIGHT_LINE lines[] = {{1.5, 1.0}, {0.0, 1.0}, {2.0, 2.0}};
	const int lines_size = sizeof(lines) / sizeof(lines[0]);

	int split_count = light_cutting_plane_count(lines, lines_size, xA, xB);
	printf("split_count = %d\n", split_count);
	split_count = light_cutting_plane_count_use_reversenumber(
		lines, lines_size, xA, xB);
	printf("split_count (use reverse number method) = %d\n", split_count);

	return 0;
}
