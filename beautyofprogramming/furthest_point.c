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
* This algorithm use maximum O(n^2) times in find the convex polygon vertices of
* a given set of points.
* @author firstboy0513
* @version 0.0.1
* @date 2013-12-20
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
			if (cur_distance > max_distance) {
				max_distance = cur_distance;
				*pa = points[i]; *pb = points[j];
			}
		}
	}
	return max_distance;
}

/**
* @brief find the minimum point which is the minimum y-axis and then x-axis.
*
* @param points all points.
* @param size points count.
*
* @return the minimum point.
*/
POINT search_minimum_point(const POINT* points, const int size) {
	POINT base; base.x = points[0].x; base.y = points[0].y;
	int i = 0;
	for (i = 1; i < size; i ++) {
		if (points[i].y < base.y 
			|| (points[i].y == base.y && points[i].x < base.x)) {
			base.x = points[i].x; base.y = points[i].y;
		}
	}
	return base;
}

/**
* @brief convert point to vertor from base point to one point in the points set.
* Note this function just modify the element in the points array input, and 
* not malloc any new memory.
*
* @param points all points.
* @param size points count.
* @param base base point.
*
* @return the vector now in the points set.
*/
POINT* convert_point_to_vector_by_base(POINT* points, int size, POINT base) {
	int i = 0;
	for (i = 0; i < size; i ++) {
		points[i].x -= base.x; points[i].y -= base.y;
	}
	return points;
}

/**
* @brief get cosines value as the angel value, which the cosine is bigger, the 
* angel is smaller. It is the dot point with i(1,0) vector.
*
* @param v given vector.
*
* @return return the angel between the vector and x-axis, if the denominator is 
* zero, then return INFINITY. It just let the base point order in the first.
*/
double cosines_angel(POINT v) {
	double denominator = v.x*v.x + v.y*v.y;
	if (denominator <= 0) { return INFINITY; }
	return (v.x) / sqrt(v.x*v.x + v.y*v.y);
}

/**
* @brief swap vertor and angel.
*
* @param points named points, it is the vector the true.
* @param cosines cosines of the angel between x-axis.
* @param i one index.
* @param j anotheer index.
*
* @return just a macro.
*/
#define SWAP_VECTORANGLE(points, cosines, i, j) {							\
	_tmpswap = cosines[i]; cosines[i] = cosines[j]; cosines[j] = _tmpswap;	\
	points[i].x ^= points[j].x; points[i].y ^= points[j].y;					\
	points[j].x ^= points[i].x; points[j].y ^= points[i].y;					\
	points[i].x ^= points[j].x; points[i].y ^= points[j].y;					\
}

/**
* @brief use quicksort to sort the vector with the cosines descending.
*
* @param points vectors.
* @param cosines cosines angel.
* @param left left index use in quicksort.
* @param right right index use in quicksort.
*/
void quicksort_vectorangle(POINT* points, double* cosines, 
	int left, int right) {
	int i = left, j = right;
	double pivot = cosines[(left+right)/2], _tmpswap = 0.0;
	while (i <= j) {
		while (cosines[j] < pivot) { j --; }
		while (cosines[i] > pivot) { i ++; }
		if (i <= j) {
			if (i != j) { SWAP_VECTORANGLE(points, cosines, i, j) }
			i ++, j --;
		}
	}
	if (left < j) { quicksort_vectorangle(points, cosines, left, j); }
	if (right > i) { quicksort_vectorangle(points, cosines, i, right); }
}

/**
* @brief print out the cosines value.
* I thought is just use for a debug, not real use.
*
* @param cosines cosines array.
* @param size cosines count.
*/
void printout_cosines(double* cosines, int size) {
	int i = 0;
	for (i = 0; i < size; i ++) {
		if (0 != i) { printf(", "); }
		printf("%f", cosines[i]);
	}
	printf("\n");
}

/**
* @brief quick sort descending as cosines bigger, then angel smaller.
*
* @param points vectors array.
* @param size vector count.
*/
void quicksort_vector_by_angle(POINT* points, int size) {
	int i = 0;
	double* cosines = (double*) malloc(sizeof(double) * size);
	for (i = 0; i < size; i ++) {
		cosines[i] = cosines_angel(points[i]);
	}
	quicksort_vectorangle(points, cosines, 0, size-1);
	free(cosines);
}

/**
* @brief link all point as an edge.
* Note this function just modify the element in the points array input, and 
* not malloc any new memory.
*
* @param vector vectors array.
* @param size vectors count.
*
* @return linked edges point.
*/
POINT* link_edge(POINT* vector, int size) {
	int i = 0;
	for (i = size-1; i > 0; i --) {
		vector[i].x -= vector[i-1].x;
		vector[i].y -= vector[i-1].y;
	}
	return vector;
}

/**
* @brief calculate two vector cross multiply. (Or called cross product)
* It use right-hand from v1 to v2.
*
* @param v1 one vector.
* @param v2 another vector.
*
* @return cross multiply value.
*/
int cross_multiply(POINT v1, POINT v2) {
	return (v1.x*v2.y - v2.x*v1.y);
}

/**
* @brief backtrack to remove invalid vectices.
* It can be sure the flags[1]=1, flags[size-1]=1, it means the first and the 
* end vectices are valid.
*
* @param edges all edges array.
* @param size edges count.
*
* @return return remain vectices corresponding index flags array.
*/
int* backtrack_vertices(POINT* edges, int size) {
	int* flags = (int*) malloc(sizeof(int) * size);
	int i = 0, j = 0, m = 0, n = 0;
	for (i = 0; i < size; i ++) { flags[i] = 0; }
	// find from end to front.
	for (i = size-1, j = i-1; i >= 2 && j >= 1;) {
		if (cross_multiply(edges[j], edges[i]) > 0) {
			flags[i] = 1;
			// backtrack from current to end.
			for (m = i, n = m+1; m < size-1 && n < size;) {
				while (flags[n] == 0 && n < size) { n ++; }
				if (n >= size) { break; }
				if (1 == flags[m] && cross_multiply(edges[m], edges[n]) > 0) {
					m = n; n ++;
					break;
				}
				flags[m] = 0;
				edges[n].x += edges[m].x; edges[n].y += edges[m].y;
				m = n; n ++;
			}
			i = j; j --;
			continue;
		}
		edges[i].x += edges[j].x; edges[i].y += edges[j].y;
		j --;
	}
	flags[i] = 1;
	return flags;
}

/**
* @brief restore the vectices to the original points.
*
* @param edges all edges.
* @param size edges count.
* @param flags valid index flags array.
* @param base base point.
*/
void restore_vertices(POINT* edges, int size, int* flags, POINT base) {
	int i = 0, j = 0;
	flags[0] = 1; edges[0].x = base.x; edges[0].y = base.y;
	for (i = 1, j = 0; i < size; i ++) {
		if (0 == flags[i]) { continue; }
		edges[i].x += edges[j].x; edges[i].y += edges[j].y;
		j = i;
	}
}

/**
* @brief print out the vertices.
*
* @param edges all edges.
* @param size edges count.
* @param flags valid index flags array.
*/
void printout_vertices(POINT* edges, int size, int* flags) {
	int i = 0;
	for (i = 0; i < size; i ++) {
		if (0 == flags[i]) { continue; }
		printf("(%d,%d)\n", edges[i].x, edges[i].y);
	}
}

/**
* @brief save vertices result to output storage.
*
* @param edges all edges of polygon, include not invalid point.
* @param size edges size.
* @param flags flags to determine whether the point is invalid in edges.
* @param pvertices output vertices.
* @param pcount output vertices count.
*/
void save_vertices_result(POINT* edges, int size, int* flags, 
	POINT** pvertices, int* pcount) {
	*pcount = 0;
	int i = 0, j = 0;
	for (i = 0; i < size; i ++) {
		if (0 != flags[i]) { (*pcount) ++; }
	}
	*pvertices = (POINT*) malloc(sizeof(POINT) * (*pcount));
	for (i = 0; i < size; i ++) {
		if (0 == flags[i]) { continue; }
		(*pvertices)[j].x = edges[i].x; (*pvertices)[j].y = edges[i].y; j ++;
	}
}

/**
* @brief find the convex polygon vertices from given points.
*
* @param points given points.
* @param size points size.
* @param pvertices output vertices point.
* @param pcount output vertices count.
*
* @return vertices count, if return less than 0 means error occur.
*/
int find_convex_polygon_vertices(const POINT* points, const int size, 
	POINT** pvertices, int* pcount) {
	if (!points || size <= 0 || !pvertices || !pcount) {
		printf("[ERR] -- find_convex_polygon_vertices input invalid!\n");
		return -1;
	}
	// 1.1 find min(y-axis, x-axis) base point.
	POINT base = search_minimum_point(points, size);
	// 1.2 convert point to vector by base and then sort by angel with x-axis
	POINT* points_copy = (POINT*) malloc(sizeof(POINT) * size);
	memcpy(points_copy, points, sizeof(POINT) * size);
	POINT* pvector = convert_point_to_vector_by_base(points_copy, size, base);
	quicksort_vector_by_angle(pvector, size);
	// 1.3 link all vector as edges as a polygon
	POINT* edges = link_edge(pvector, size);
	// 1.4 back track to find vertices use cross multiply sign
	int* flags = backtrack_vertices(edges, size);
	// 1.5 restore the end result vertices of convex polygon
	restore_vertices(edges, size, flags, base);
	save_vertices_result(edges, size, flags, pvertices, pcount);

	free(points_copy); free(flags);

	return *pcount;
}

/**
* @brief convert point to vertor from point b to point a.
*
* @param a end point.
* @param b start point.
*
* @return the vertor from point b to point a.
*/
POINT convert_point_to_vector(POINT a, POINT b) {
	POINT point;
	point.x = a.x - b.x;
	point.y = a.y - b.y;
	return point;
}

/**
* @brief use Shamos's algorithm to comput the diameter of given convex polygon.
*
* @param vertices given convex polygon vertices.
* @param count vertices count.
* @param pa output diameter one point.
* @param pb output diameter another point.
*
* @return diameter of the convex polygon.
*/
double get_convex_polygon_diameter(POINT* vertices, int count, 
	POINT* pa, POINT* pb) {
	double max_distance = 0, current_distance = 0;
	int curmax_area = 0, current_area = 0;
	if (!vertices || count <= 0 || !pa || !pb) {
		return 0;
	}
	int i = 0, j = 0, k = 0;
	for (i = 0, k = i+2; i < count; i ++) {
		curmax_area = 0;
		do {
			curmax_area = current_area;
			j = (i+1)%count;
			POINT line = convert_point_to_vector(vertices[j], vertices[i]);
			POINT point = convert_point_to_vector(vertices[k], vertices[i]);
			current_area = cross_multiply(line, point);
			current_distance = get_distance(vertices[i], vertices[k]);
			if (current_distance > max_distance) {
				max_distance = current_distance;
				(*pa) = vertices[i]; (*pb) = vertices[k];
			}
			k = (k+1)%count;
		} while (current_area >= curmax_area);
		k = (k > 0) ? (k-1) : (count-1);
	}
	return get_distance(*pa, *pb);
}

int main(int argc, const char *argv[])
{
	POINT a, b;
	// TEST CASES
//	const POINT points[] = {
//		{6,0}, {8,11}, {12,16}, {16,22}, {27,29} };
//	const POINT points[] = { 
//		{8,12}, {17,11}, {6,5}, {12,3}, {13,8}, {15,16}, {2,10} };
//		{8,12}, {17,11}, {12,3}, {13,8}, {15,16}, {2,10} };
//		{8,12}, {6,5}, {12,3}, {15,16}, {2,10} };
//		{17,11}, {12,3}, {13,8}, {2,10} };
//	const POINT points[] = {
//		{16,13}, {25,27}, {25,23}, {12,16}, {1,9}, {7,2}, {19,20}, {16,23}, 
//		{6,0}, {16,22}, {8,11}, {9,27}, {20,2}, {13,2}, {25,7}, {12,29}, 
//		{18,12}, {27,29}, {16,13}, {22,1} };
	const POINT points[] = {
		{3,6}, {17,15}, {13,15}, {6,12}, {9,1}, {2,7}, {10,19}, {3,6}, {0,6}, 
		{12,16}, {11,8}, {7,9}, {2,10}, {2,3}, {7,15}, {9,2}, {2,18}, {9,7}, 
		{13,16}, {11,2}, {9,13}, {1,19}, {4,17}, {18,4}, {15,10}, {13,6}, 
		{11,0}, {16,13}, {2,10}, {16,1}, {5,5}, {4,7}, {16,5}, {6,9}, {13,17}, 
		{4,15}, {2,5}, {14,7}, {14,4}, {3,10}, {7,8}, {16,18}, {8,4}, {3,11}, 
		{14,19}, {12,0}, {16,8}, {19,12}, {6,6}, {14,19}, {15,10}, {14,18}, 
		{7,1}, {17,2}, {17,12}, {12,16}, {1,0}, {6,1}, {5,9}, {4,19}, {0,9}, 
		{11,17}, {17,11}, {1,15}, {9,7}, {7,16}, {17,13}, {6,5}, {6,3}, {19,4}, 
		{8,11}, {12,9}, {3,19}, {10,8}, {8,15}, {0,9}, {16,3}, {18,5}, {6,11}, 
		{1,15}, {19,8}, {4,8}, {1,10}, {13,0}, {14,4}, {4,14}, {7,16}, {3,11}, 
		{7,5}, {19,16}, {12,11}, {17,8}, {15,7}, {14,1}, {18,15}, {9,17}, 
		{15,13}, {18,8}, {3,11}, {8,9} };
	const int size = sizeof(points) / sizeof(points[0]);

	POINT* vertices = NULL;
	int count = 0, i = 0;
	find_convex_polygon_vertices(points, size, &vertices, &count);
	printf("there are %d count of vertices: ", count);
	for (i = 0; i < count; i ++) {
		if (0 != i) { printf(", "); }
		printf("(%d,%d)", vertices[i].x, vertices[i].y);
	}
	printf("\n");
	double max_distance = get_convex_polygon_diameter(vertices, count, &a, &b);
	printf("and the diameter is %g, from (%d,%d) to (%d,%d)\n", max_distance, 
		a.x, a.y, b.x, b.y);
	if (vertices) { free(vertices); }

//	double max_distance = find_furthest_point_exhaust(points, size, &a, &b);
//
//	if (INT32_MIN != max_distance) {
//		printf("maximum distance is %g\n", max_distance);
//		printf("\ta(%d,%d) to b(%d,%d)\n", a.x, a.y, b.x, b.y);
//	} else {
//		printf("[ERR] -- some error occur to find nearest point.\n");
//	}

	return 0;
}
