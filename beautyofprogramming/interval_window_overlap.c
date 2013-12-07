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
* @file interval_window_overlap.c
* @brief window overlap and completely cover judgment.
* @author firstboy0513
* @version 0.0.2
* @date 2013-12-06
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/**
* @brief window struct from left bottom point to right top point.
*/
typedef struct _WINDOW {
	int x0; // left bottom point x value (minimum x value).
	int y0; // left bottom point y value (minimum y value).
	int x1; // right top point x value (maximum x value).
	int y1; // right top point y value (maximum y value).
} WINDOW;

/**
* @brief Intervals varify.
*
* @param intervals a set intervals.
* @param len intervals length.
*
* @return return 1 if valid, otherwise return 0 means invalid.
*/
int windows_varify(WINDOW* windows, int len) {
	while (--len >= 0) {
		if (windows[len].y0 > windows[len].y1 
			|| windows[len].x0 > windows[len].x1) {
			printf("[ERR] -- windows is invalid from left bottom point to "
				"right top point!\n");
			return 0;
		}
	}
	return 1;
}

/**
* @brief window overlap judgment.
*
* @param windows a set windows.
* @param len windows length.
* @param onewindow one window to judgment.
*
* @return return 1 if overlap, otherwise return 0 means not overlap, and return
* -1 means some error occur.
*/
int window_overlap_judgment(WINDOW* windows, int len, 
	WINDOW onewindow) {
	if(!windows_varify(windows, len)) {
		return -1;
	}
	if(!windows_varify(&onewindow, 1)) {
		return -1;
	}
	int i = 0;
	for (i = 0; i < len; i ++) {
		if (onewindow.x1 < windows[i].x0 || onewindow.x0 > windows[i].x1
			|| onewindow.y1 < windows[i].y0 || onewindow.y0 > windows[i].y1) {
		} else {
			return 1;
		}
	}
	return 0;
}

/**
* @brief use interval tree to judgment overlap interval.
*/
typedef struct _INTERVAL_TREE_NODE {
	int cover; 	// cover times
	int low; 	// low number
	int high;   // high number
	struct _INTERVAL_TREE_NODE* left;  // left child point
	struct _INTERVAL_TREE_NODE* right; // right child point
} INTERVAL_TREE_NODE;

/**
* @brief generate new interval tree node.
* It initialize struct elements.
*
* @return newer interval tree node.
*/
INTERVAL_TREE_NODE* generate_new_interval_tree_node() {
	INTERVAL_TREE_NODE* pnode = (INTERVAL_TREE_NODE*) malloc(
		sizeof(INTERVAL_TREE_NODE));
	pnode->cover = 0;
	pnode->low = 0, pnode->high = 0;
	pnode->left = NULL, pnode->right = NULL;
	return pnode;
}

/**
* @brief destory interval tree.
*
* @param tree interval tree point.
*/
void interval_tree_destory(INTERVAL_TREE_NODE* tree) {
	if (NULL == tree) return;
	interval_tree_destory(tree->left);
	interval_tree_destory(tree->right);
	free(tree);
}

void interval_tree_printout(INTERVAL_TREE_NODE* tree) {
	if (NULL == tree) {
		return;
	} else {
		printf("tree node cover = %d, low = %d, high = %d\n", tree->cover, 
			tree->low, tree->high);
	}
	interval_tree_printout(tree->left);
	interval_tree_printout(tree->right);
}

/**
* @brief build interval tree.
*
* @param low low number in interval.
* @param high high number in interval.
*
* @return interval tree point.
*/
INTERVAL_TREE_NODE* interval_tree_build(int low, int high) {
	if (low >= high) {
		printf("[ERR] -- interval tree build invalid low > high input.\n");
		return NULL;
	}
	INTERVAL_TREE_NODE* tree = generate_new_interval_tree_node();
	tree->low = low, tree->high = high;
	if (high - low > 1) {
		int mid = (low + high) / 2;
		tree->left = interval_tree_build(low, mid);
		tree->right = interval_tree_build(mid, high);
	}
	return tree;
}

/**
* @brief add new interval to interval tree.
*
* @param tree interval tree point.
* @param low low number in interval.
* @param high high number in interval.
*/
void interval_tree_add(INTERVAL_TREE_NODE* tree, int low, int high) {
	if (low <= tree->low && high >= tree->high) {
		tree->cover ++;
	} else if (tree->high - tree->low > 1) {
		int mid = (tree->low + tree->high) / 2;
		if (low < mid) interval_tree_add(tree->left, low, high);
		if (high > mid) interval_tree_add(tree->right, low, high);
	}
}

/**
* @brief delete interval in interval tree.
*
* @param tree interval tree point.
* @param low low number in interval.
* @param high high number in interval.
*/
void interval_tree_delete(INTERVAL_TREE_NODE* tree, int low, int high) {
	if (low <= tree->low && high >= tree->high) {
		tree->cover --;
	} else if (tree->high - tree->low > 1) {
		int mid = (tree->low + tree->high) / 2;
		if (low < mid) interval_tree_delete(tree->left, low, high);
		if (high > mid) interval_tree_delete(tree->right, low, high);
	}
}

/**
* @brief find inverval in interval tree, means whether it be cover in the 
* interval tree intervals.
*
* @param tree interval tree point.
* @param low low number in interval.
* @param high high number in interval.
*
* @return return 1 if found, otherwise return 0 means not found, and return -1
* means some error occur.
*/
int interval_tree_find(INTERVAL_TREE_NODE* tree, int low, int high) {
	if (low <= tree->low && high >= tree->high && tree->cover > 0) {
		return 1;
	} else if (tree->high - tree->low > 1) {
		int ret = 1;
		int mid = (tree->low + tree->high) / 2;
		if (low < mid) ret = interval_tree_find(tree->left, low, high);
		if (0 == ret) return 0;
		if (high > mid) ret = interval_tree_find(tree->right, low, high);
		if (0 == ret) return 0;
		return 1;
	}
	return 0;
}

/**
* @brief build index mapping array.
*
* @param intervals a set intervals.
* @param len intervals length.
* @param pindex_count output deduplicated index count.
*
* @return index mapping array point.
*/
int* index_mapping_build(WINDOW* windows, int len, int* pindex_count) {
	// malloc index_map array space.
	int size = 2*len;
	int* index_map = (int*) malloc(sizeof(int)*size);
	int i = 0, j = 0;
	for (i = 0; i < len; i ++) {
		index_map[2*i] = windows[i].y0;
		index_map[2*i+1] = windows[i].y1;
	}
	// ascending sort (use bubble sort algorithm) endpoint values.
	for (i = 0; i < size-1; i ++) {
		for (j = i+1; j < size; j ++) {
			if (index_map[i] > index_map[j]) {
				index_map[i] ^= index_map[j];
				index_map[j] ^= index_map[i];
				index_map[i] ^= index_map[j];
			}
		}
	}
	// deduplication endpoint values.
	for (j = 0, i = 1; i < size; i ++) {
		if (index_map[i-1] != index_map[i]) {
			index_map[j++] = index_map[i-1];
		}
	}
	index_map[j++] = index_map[i-1];
	// result output
	if (pindex_count) {
		*pindex_count = j;
	}
	return index_map;
}

int index_mapping_get_index(int* index_map, int size, int value) {
	int i = size-1;
	while (i >= 0 && index_map[i] >= value) {
		i --;
	}
	if (i < 0 && index_map[0] > value) return -1;
	return ++ i;
}

int index_mapping_get_lower_index(int* index_map, int size, int value) {
	int i = size-1;
	while (i >= 0 && index_map[i] >= value) {
		i --;
	}
	if (i < 0 && index_map[0] > value) {
		return -1;
	} else if (i+1 < size && index_map[i+1] == value) {
		return ++ i;
	}
	return i;
}

int index_mapping_get_upper_index(int* index_map, int size, int value) {
	int i = 0;
	while (i < size && index_map[i] <= value) {
		i ++;
	}
	if (i >= size && index_map[size-1] < value) {
		return -1;
	} else if (i-1 >= 0 && index_map[i-1] == value) {
		return -- i;
	}
	return i;
}

/**
* @brief given a x-axis position and a y-axis line from low to high.
*/
typedef struct _X_LINE {
	int low;	// y-axis low location
	int high;   // y-axis high location
	int x;      // x-axis location
	enum X_DIRECTION {X_LEFT, X_RIGHT} flag; // 0 means left, 1 means right.
} X_LINE;

void line_array_swap(X_LINE* p, X_LINE* q) {
	p->x ^= q->x; q->x ^= p->x; p->x^= q->x;
	p->low ^= q->low; q->low ^= p->low; p->low^= q->low;
	p->high ^= q->high; q->high ^= p->high; p->high^= q->high;
	p->flag ^= q->flag; q->flag ^= p->flag; p->flag^= q->flag;
}

X_LINE* line_array_build(WINDOW* windows, int len, int* pline_count) {
	// malloc line_array space.
	int size = 2*len;
	X_LINE* line_array = (X_LINE*) malloc(sizeof(X_LINE)*size);
	int i = 0, j = 0;
	for (i = 0; i < len; i ++) {
		line_array[2*i].low = line_array[2*i+1].low = windows[i].y0;
		line_array[2*i].high = line_array[2*i+1].high = windows[i].y1;
		line_array[2*i].x = windows[i].x0; line_array[2*i+1].x = windows[i].x1;
		line_array[2*i].flag = X_LEFT; line_array[2*i+1].flag = X_RIGHT;
	}
	// ascending sort (use bubble sort algorithm) x-axis values.
	for (i = 0; i < size-1; i ++) {
		for (j = i+1; j < size; j ++) {
			if (line_array[i].x > line_array[j].x) {
				line_array_swap(line_array + i, line_array + j);
			}
		}
	}
	// result output
	if (pline_count) {
		*pline_count = size;
	}
	return line_array;
}

int window_completeltycover_find(int* index_map, int index_count, 
	X_LINE* line_array, int line_count, WINDOW onewindow) {
	// build interval tree
	INTERVAL_TREE_NODE* tree = interval_tree_build(0, index_count);
	int r = 0;
	
	// find interval
	int i = 0;
	interval_tree_add(tree, 
		index_mapping_get_index(index_map, index_count, line_array[i].low), 
		index_mapping_get_index(index_map, index_count, line_array[i].high) );
	for (i = 1; i < line_count-1; i ++) {
		if (X_LEFT == line_array[i].flag) { // X_LEFT
			interval_tree_add(tree, 
				index_mapping_get_index(index_map, index_count, line_array[i].low), 
				index_mapping_get_index(index_map, index_count, line_array[i].high) );
		} else { // X_RIGHT
			interval_tree_delete(tree, 
				index_mapping_get_index(index_map, index_count, line_array[i].low), 
				index_mapping_get_index(index_map, index_count, line_array[i].high) );
		}

		// find the y-axis interval in the interval tree
		if (line_array[i].x != line_array[i-1].x 
			&& onewindow.x0 <= line_array[i+1].x 
			&& onewindow.x1 >= line_array[i].x) {
			// find interval
			int a = index_mapping_get_lower_index(index_map, index_count, onewindow.y0);
			int b = index_mapping_get_upper_index(index_map, index_count, onewindow.y1);
			if (-1 != a && -1 != b) {
				r = interval_tree_find(tree, a, b);
			} else {
				r = 0;
			}
			if (0 == r) {
				break;
			}
		} else if (onewindow.x1 < line_array[i].x) {
			break;
		}
	}

	// release memory
	interval_tree_destory(tree);
	return r;
}

/**
* @brief window completely cover judgment.
* It use interval tree algorithm to judgment y-axis value, and use LINE to 
* store x-axis, and use index mapping to store value-index array.
*
* @param windows a set windows.
* @param len windows length.
* @param onewindow one window to judgment.
*
* @return return 1 if onewindow is all cover by a set windows, otherwise 
* return 0 means not all cover, and return -1 means some error eccur.
*/
int window_completeltycover_judgment(WINDOW* windows, int len, 
	WINDOW onewindow) {
	if(!windows_varify(windows, len)) {
		return -1;
	}
	if(!windows_varify(&onewindow, 1)) {
		return -1;
	}
	
	// build index mapping and line array.
	int index_count = 0, line_count = 0;
	int* index_map = index_mapping_build(windows, len, &index_count);
	X_LINE* line_array = line_array_build(windows, len, &line_count);
	// find window in a line array (it use interval tree algorithm).
	int r = window_completeltycover_find(index_map, index_count, line_array, 
		line_count, onewindow);
	// release memory.
	free(index_map);
	free(line_array);
	return r;
}

int main(int argc, const char *argv[])
{
	WINDOW windows[] = {{-15,0,5,10}, {-5,8,20,25}, {15,-4,24,14}, {0,-6,16,4}};
	WINDOW onewindow = {2,15,10,22};
//	printf("interval window overlap judgment return %d\n",
//			window_overlap_judgment(windows,
//				sizeof(windows)/sizeof(windows[0]), onewindow) );
	printf("interval window completely cover judgment return %d\n",
			window_completeltycover_judgment(windows,
				sizeof(windows)/sizeof(windows[0]), onewindow) );
	return 0;
}
