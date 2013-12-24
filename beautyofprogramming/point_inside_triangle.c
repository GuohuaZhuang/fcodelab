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
* @file point_inside_triangle.c
* @brief determine whether one point is inside the triangle.
* @author firstboy0513
* @version 0.0.1
* @date 2013-12-24
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

/**
* @brief point in 2-dimensional with x-axis and y-axis.
*/
typedef struct _POINT {
	int x;
	int y;
} POINT;

/**
* @brief triangle verification.
*
* @param A one point of triangle.
* @param B one point of triangle.
* @param C one point of triangle.
*
* @return return 1 if is legal verification, otherwise return 0 means invalid.
*/
int triangle_verification(POINT A, POINT B, POINT C) {
	return (0 != (B.x - A.x) * (C.y - B.y) - (C.x - B.x) * (B.y - A.y));
}

/**
* @brief calculate triangle area use Helen formula.
*
* @param A one point of triangle.
* @param B one point of triangle.
* @param C one point of triangle.
*
* @return the triangle area.
*/
double triangle_area(POINT A, POINT B, POINT C) {
	// three edge length a(AB), b(BC), c(CA)
	double a = .0, b = .0, c = .0;
	a = sqrt( pow(B.x-A.x, 2) + pow(B.y-A.y, 2) );
	b = sqrt( pow(C.x-B.x, 2) + pow(C.y-B.y, 2) );
	c = sqrt( pow(A.x-C.x, 2) + pow(A.y-C.y, 2) );

	double p = (a + b + c) / 2;
	return sqrt( p * (p - a) * (p - b) * (p - c) );
}

/**
* @brief judge the point D whether is inside the triangle.
*
* @param A one point of triangle.
* @param B one point of triangle.
* @param C one point of triangle.
* @param D one point to judge whether it is inside the triangle.
*
* @return return 0 means not inside the triangle, else return 1 means inside in 
* the triangle, otherwise return -1 means some error occur, such as invalid 
* triangle.
*/
int point_inside_triangle_use_areasum(POINT A, POINT B, POINT C, POINT D) {
	if (!triangle_verification(A, B, C)) { return -1; }
	return (triangle_area(A, B, C) >= triangle_area(A, B, D) 
		+ triangle_area(B, C, D) + triangle_area(C, A, D));
}

/**
* @brief cross product from AB to AC.
*
* @param A one point of vector AB.
* @param B one point of vector AB and vector BC.
* @param C one point of vector BC.
*
* @return cross product value.
*/
double crossproduct(POINT A, POINT B, POINT C) {
	return (B.x - A.x) * (C.y - A.y) - (C.x - A.x) * (B.y - A.y);
}

/**
* @brief use quick sort to sort point by (y-axis, x-axis) descend.
*
* @param P[] point array.
* @param left left sort index.
* @param right right sort index.
*/
void quicksort_descend_yx(POINT* P[], int left, int right) {
	if (!P || left >= right) { return; }
	int i = left, j = right;
	POINT* pivot = P[ (i+j)/2 ];
	while (i <= j) {
		while (P[j]->y > pivot->y 
			|| (P[j]->y == pivot->y && P[j]->x > pivot->x) ) {
			j --;
		}
		while (P[i]->y < pivot->y
			|| (P[i]->y == pivot->y && P[i]->x < pivot->x) ) {
			i ++;
		}
		if (i <= j) {
			if (i != j) {
				P[i]->x ^= P[j]->x; P[i]->y ^= P[j]->y;
				P[j]->x ^= P[i]->x; P[j]->y ^= P[i]->y;
				P[i]->x ^= P[j]->x; P[i]->y ^= P[j]->y;
			}
			i ++, j --;
		}
	}
	if (left < j) { quicksort_descend_yx(P, left, j); }
	if (right > i) { quicksort_descend_yx(P, i, right); }
}

/**
* @brief judge the point D whether is inside the triangle.
*
* @param A one point of triangle.
* @param B one point of triangle.
* @param C one point of triangle.
* @param D one point to judge whether it is inside the triangle.
*
* @return return 0 means not inside the triangle, else return 1 means inside in 
* the triangle, otherwise return -1 means some error occur, such as invalid 
* triangle.
*/
int point_inside_triangle_use_crossproduct(POINT A, POINT B, POINT C, POINT D) {
	if (!triangle_verification(A, B, C)) { return -1; }
	// order as A, B, C by min(y-axis, x-axis)
	POINT* P[] = {&A, &B, &C};
	quicksort_descend_yx(P, 0, 2);
	
	// judge the cross product
	return (crossproduct(A, B, D) >= 0 && crossproduct(B, C, D) >= 0 
		&& crossproduct(C, A, D) >= 0);
}

/////////////////////////EXtended Problems//////////////////////////////////////
// 1. If do not want include the point in the edge of triangle as inside it,
// then just remove the equal of crossproduct.
// 2. If want to judge one point whether inside a convex polygon, the same 
// method use cross product sign to determine it. You should just find the right
// order of convex polygon vertices.
// 3. If want to judge one point whether inside a not-self-intersect polygon, 
// the simple i found is to find the closest edge in this polygon, and found 
// the direction of it, the closest is just calculate the minimum of vertical
// distance, and two endpoint distance. Then use cross product sign again.
// 4. How to judge one point whether in a tetrahedron? We know a tetrahedron has
// 6 flats, we can put the point's 6 projection one every flat, and then judge 
// the point whether inside a 2-dimensional quadrilateral is easy use problem 3
// method. You should just calculate the projection with a point and a flat.
// We know a flat formula is Ax + By + Cz + D = 0, and suppose the point is 
// M1(x1, y1, z2), then we want the projection M2(x2, y2, z2), we found that:
//     (x1-x2, y1-y2, z1-z2) = k * (A, B, C)
// the (A, B, C) is the normals vector of the flat. And we have:
//     Ax2 + By2 + Cz2 + D = 0
// So, just calculate it, then we can get the M2(x2, y2, z2) and k value.
////////////////////////////////////////////////////////////////////////////////

int main(int argc, const char *argv[])
{
	POINT A = {-5, -1}, B = {8, 2}, C = {4, 10}, D = {3, 6};
	printf("point_inside_triangle_use_areasum result is %d\n", 
		point_inside_triangle_use_areasum(A, B, C, D) );
	printf("point_inside_triangle_use_crossproduct result is %d\n", 
		point_inside_triangle_use_crossproduct(B, C, A, D) );
	return 0;
}
