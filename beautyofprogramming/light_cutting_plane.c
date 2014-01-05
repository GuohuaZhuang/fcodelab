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
*
* @param points points.
* @param size points count.
* @param left left axis interval bound.
* @param right right axis interval bound.
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
* @param xA x-axis left bound.
* @param xB x-axis right bound.
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

int main(int argc, const char *argv[])
{
	// x-axis interval from A to B, be sure A < B.
	const double xA = -1.0, xB = 10.0;
	LIGHT_LINE lines[] = {{1.5, 1.0}, {0.0, 1.0}, {2.0, 2.0}};
	const int lines_size = sizeof(lines) / sizeof(lines[0]);

	int split_count = light_cutting_plane_count(lines, lines_size, xA, xB);
	printf("split_count = %d\n", split_count);

	return 0;
}
