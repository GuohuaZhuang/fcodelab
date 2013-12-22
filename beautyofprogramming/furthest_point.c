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
			// printf("cur_distance = %f\n", cur_distance);
			if (cur_distance > max_distance) {
				max_distance = cur_distance;
				*pa = points[i]; *pb = points[j];
			}
		}
	}
	return max_distance;
}

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

POINT* convert_point_to_vector_by_base(POINT* points, int size, POINT base) {
	int i = 0;
	for (i = 0; i < size; i ++) {
		points[i].x -= base.x; points[i].y -= base.y;
	}
	return points;
}

double cosines_angel(POINT v) {
	// dot point with i(1,0) vector
	double denominator = v.x*v.x + v.y*v.y;
	if (denominator <= 0) { return INFINITY; }
	return (v.x) / sqrt(v.x*v.x + v.y*v.y);
}

#define SWAP_VECTORANGLE(points, cosines, i, j) {							\
	_tmpswap = cosines[i]; cosines[i] = cosines[j]; cosines[j] = _tmpswap;	\
	points[i].x ^= points[j].x; points[i].y ^= points[j].y;					\
	points[j].x ^= points[i].x; points[j].y ^= points[i].y;					\
	points[i].x ^= points[j].x; points[i].y ^= points[j].y;					\
}

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

void printout_cosines(double* cosines, int size) {
	int i = 0;
	for (i = 0; i < size; i ++) {
		if (0 != i) { printf(", "); }
		printf("%f", cosines[i]);
	}
	printf("\n");
}

// quick sort descending as cosines bigger, then angel smaller.
void quicksort_vector_by_angle(POINT* points, int size) {
	int i = 0;
	double* cosines = (double*) malloc(sizeof(double) * size);
	for (i = 0; i < size; i ++) {
		cosines[i] = cosines_angel(points[i]);
	}
	quicksort_vectorangle(points, cosines, 0, size-1);
	printout_cosines(cosines, size);
	free(cosines);
}

POINT* link_edge(POINT* vector, int size) {
	int i = 0;
	for (i = size-1; i > 0; i --) {
		vector[i].x -= vector[i-1].x;
		vector[i].y -= vector[i-1].y;
	}
	return vector;
}

int cross_multiply(POINT v1, POINT v2) {
	return (v1.x*v2.y - v2.x*v1.y);
}

// 会确保flags[1]=1, flags[size-1]=1
int* backtrack_vertices(POINT* edges, int size) {
	int* flags = (int*) malloc(sizeof(int) * size);
	int i = 0, j = 0, m = 0, n = 0;
	for (i = 0; i < size; i ++) { flags[i] = 0; }
	// 先倒着找一遍
	for (i = size-1, j = i-1; i >= 2 && j >= 1;) {
		if (cross_multiply(edges[j], edges[i]) > 0) {
			flags[i] = 1;
			// 向前再回溯删除不必要的顶点
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

void restore_vertices(POINT* edges, int size, int* flags, POINT base) {
	int i = 0, j = 0;
	flags[0] = 1; edges[0].x = base.x; edges[0].y = base.y;
	for (i = 1, j = 0; i < size; i ++) {
		if (0 == flags[i]) { continue; }
		edges[i].x += edges[j].x; edges[i].y += edges[j].y;
		j = i;
	}
}

void printout_vertices(POINT* edges, int size, int* flags) {
	int i = 0;
	for (i = 0; i < size; i ++) {
		if (0 == flags[i]) { continue; }
		// printf("%d => DEBUG(%d,%d)\n", i, edges[i].x, edges[i].y);
		printf("(%d,%d)\n", edges[i].x, edges[i].y);
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
	// 1.1 取min(y,x)点为基点（直接找基点）该点也必然为凸边形的顶点。
	POINT base = search_minimum_point(points, size);
	// 1.2 把其余点与基点作向量，然后按这个向量的x轴夹角排序。(升序)
	POINT* points_copy = (POINT*) malloc(sizeof(POINT) * size);
	memcpy(points_copy, points, sizeof(POINT) * size);
	printout_points(points_copy, size);
	POINT* pvector = convert_point_to_vector_by_base(points_copy, size, base);
	printout_points(pvector, size);
	quicksort_vector_by_angle(pvector, size);
	/// DEBUG
	printout_points(pvector, size);
	// 1.3 将所有这些向量首尾相连。
	POINT* edges = link_edge(pvector, size);
	printout_points(pvector, size);
	// 1.4 回溯删除利用外积判断方向不在凸边形的边上的向量。
	//     (PS:我刚刚自己证明了这个右手法则如果大姆指向上为正，向下为负的：
	//          向量a和向量b的外积也叫叉积，为x1y2-x2y1。试想，如果结果为：
	//          x1y2-x2y1 > 0，则x1/y1 > x2/y2，也就是cot(a)的值比cot(b)大，
	//          画出cot曲线看看，在0~pi的区间里，也就是说a与x轴夹角比b与x轴
	//          夹角小，所以从a到b的右手法则四个手指从a扫向b，大姆指向上。
	//          反之亦然：大姆指向下，四手指从b扫向a，a的夹角比b大。
	//          在pi到2pi的cot曲线里也相同。
	//          如果x1y2-x2y1 == 0，说明a与b两个向量为么同向，要么反向。
	//     )
	// 1.5 最后首尾相连的就是凸边形的边。
	int* flags = backtrack_vertices(edges, size);
	// printout_vertices(edges, size, flags);
	restore_vertices(edges, size, flags, base);
	printout_vertices(edges, size, flags);
	free(points_copy); free(flags);
	return 0;
}


int main(int argc, const char *argv[])
{
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
		{3,6}, {17,15}, {13,15}, {6,12}, {9,1}, {2,7}, {10,19}, {3,6}, {0,6}, {12,16}, {11,8}, {7,9}, {2,10}, {2,3}, {7,15}, {9,2}, {2,18}, {9,7}, {13,16}, {11,2}, {9,13}, {1,19}, {4,17}, {18,4}, {15,10}, {13,6}, {11,0}, {16,13}, {2,10}, {16,1}, {5,5}, {4,7}, {16,5}, {6,9}, {13,17}, {4,15}, {2,5}, {14,7}, {14,4}, {3,10}, {7,8}, {16,18}, {8,4}, {3,11}, {14,19}, {12,0}, {16,8}, {19,12}, {6,6}, {14,19}, {15,10}, {14,18}, {7,1}, {17,2}, {17,12}, {12,16}, {1,0}, {6,1}, {5,9}, {4,19}, {0,9}, {11,17}, {17,11}, {1,15}, {9,7}, {7,16}, {17,13}, {6,5}, {6,3}, {19,4}, {8,11}, {12,9}, {3,19}, {10,8}, {8,15}, {0,9}, {16,3}, {18,5}, {6,11}, {1,15}, {19,8}, {4,8}, {1,10}, {13,0}, {14,4}, {4,14}, {7,16}, {3,11}, {7,5}, {19,16}, {12,11}, {17,8}, {15,7}, {14,1}, {18,15}, {9,17}, {15,13}, {18,8}, {3,11}, {8,9} };
//	const POINT points[] = {
//		{6,0}, {8,11}, {12,16}, {16,22}, {27,29} };
	const int size = sizeof(points) / sizeof(points[0]);

	POINT* vertices = NULL;
	int count = 0, i = 0;
	find_convex_polygon_vertices(points, size, &vertices, &count);
	printf("there are %d count of vertices:\n", count);
	for (i = 0; i < count; i ++) {
		printf("(%d,%d)\n", vertices[i].x, vertices[i].y);
	}

//	POINT a, b;
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
