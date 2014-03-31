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
* @file dijkstra.c
* @brief Dijkstra algorithm to find shorest path implement in C.
* @author firstboy0513
* @version 0.0.1
* @date 2014-03-31
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define INFINITY INT32_MAX

int is_Q_empty(const int* Q, const int n) {
    int i = 0;
    for (i = 0; i < n; i ++) {
        if (Q[i] > 0) { return 0; }
    }
    return 1;
}

int find_smallest_distance_vertex(const int* Q, const int* dist, const int n) {
    int i = 0, min_dist = INFINITY, min_dist_idx = -1;
    for (i = 0; i < n; i ++) {
        if (Q[i] <= 0) { continue; }
        if (min_dist > dist[i]) {
            min_dist = dist[i];
            min_dist_idx = i;
        }
    }
    return min_dist_idx;
}

void set_Q_vertex_empty(int* Q, const int i) {
    Q[i] = 0;
}

int* dijkstra_path(int* const * graph, const int n, 
    const int soure_id, const int target_id) {
    // invalid input here...
    int i = 0, j = 0;
    int* dist = (int*) malloc(sizeof(int) * n);
    int* previous = (int*) malloc(sizeof(int) * n);
    int* Q = (int*) malloc(sizeof(int) * n);
    for (i = 0; i < n; i ++) {
        dist[i] = INFINITY;
        previous[i] = -1; // -1 means undefined
        Q[i] = 1; // 1 means is in Q, otherwise 0 means not in Q
    }
    dist[soure_id] = 0;
    while (!is_Q_empty(Q, n)) {
        int u = find_smallest_distance_vertex(Q, dist, n);
        if (u < 0 || dist[u] == INFINITY) { break; }
        set_Q_vertex_empty(Q, u);
        for (j = 0; j < n; j ++) {
            if (graph[u][j] < 0) { continue; }
            int alt = dist[u] + graph[u][j];
            if (alt < dist[j]) {
                dist[j] = alt;
                previous[j] = u;
            }
        }
    }
    if (dist) { free(dist); dist = NULL; }
    if (Q) { free(Q); Q = NULL; }
    return previous;
}

void print_path(const int* path, const int target_id) {
    printf("path: ");
    int i = target_id;
    while (path[i] >= 0) {
        printf("%d ", i);
        i = path[i];
    }
    printf("\n");
}

int main(int argc, const char *argv[])
{
    #define N   7
    const int graph[N][N] = { 
        {-1, 0, -1, -1, -1, -1, -1},
        {-1, -1, 0, -1, 5, -1, -1},
        {-1, -1, -1, 0, -1, 1, -1},
        {-1, -1, -1, -1, -1, -1, 0}, 
        {-1, -1, 4, -1, -1, 3, -1},
        {-1, -1, -1, 0, -1, -1, -1},
        {-1, -1, -1, -1, -1, -1, -1} };
    int i = 0, j = 0;
    int** _graph = (int**) malloc(sizeof(int*) * N);
    for(i = 0; i < N; i++) {
        _graph[i] = (int*) malloc(sizeof(int) * N);
        for(j = 0; j < N; j++) {
            _graph[i][j] = graph[i][j];
        }
    }
    const int soure_id = 0, target_id = N-1;
    int* path = dijkstra_path(_graph, N, soure_id, target_id);
    print_path(path, N-1);
    if (path) { free(path); path = NULL; }
    if (_graph) {
        for (i = 0; i < N; i ++) { free(_graph[i]); }
        free(_graph);
        _graph = NULL;
    }
    return 0;
}
