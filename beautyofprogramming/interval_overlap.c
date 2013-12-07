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
* @file interval_overlap.c
* @brief interval overlap judgment.
* @author firstboy0513
* @version 0.0.1
* @date 2013-12-04
*/
#include <stdio.h>
#include <stdlib.h>

typedef struct _INTERVAL {
	int a; // left endpoint
	int b; // right endpoint
} INTERVAL;

/**
* @brief Intervals varify.
*
* @param intervals a set intervals.
* @param len intervals length.
*
* @return return 1 if valid, otherwise return 0 means invalid.
*/
int intervals_varify(INTERVAL* intervals, int len) {
	while (--len >= 0) {
		if (intervals[len].a > intervals[len].b) {
			printf("[ERR] -- intervals is invalid!\n");
			return 0;
		}
	}
	return 1;
}

// bubble sort
/**
* @brief use bubble sort method to order intervals in left endpoint.
*
* @param intervals a set intervals.
* @param len intervals length.
*
* @return just return 1.
*/
int intervals_order(INTERVAL* intervals, int len) {
	int i = 0, j = 0;
	for (i = 0; i < len; i ++) {
		INTERVAL t;
		t.a = intervals[i].a; t.b = intervals[i].b;
		for (j = i+1; j < len; j ++) {
			if (t.a > intervals[j].a) {
				t.a ^= intervals[j].a; intervals[j].a ^= t.a; 
				t.a ^= intervals[j].a;
				t.b ^= intervals[j].b; intervals[j].b ^= t.b; 
				t.b ^= intervals[j].b;
			}
		}
		intervals[i].a = t.a; intervals[i].b = t.b;
	}
	return 1;
}

/**
* @brief merger a ordered intervals.
*
* @param intervals a set intervals.
* @param len intervals length.
*
* @return just return mergered intervals length.
*/
int intervals_merger(INTERVAL* intervals, int len) {
	int i = 0, j = 0;
	for (i = 0; i < len-1; i ++) {
		if (intervals[i].b >= intervals[i+1].a) {
			if (intervals[i].b < intervals[i+1].b) {
				intervals[i].b = intervals[i+1].b;
			}
			// move forward other intervals.
			for (j = i+1; j < len-1; j ++) {
				intervals[j].a = intervals[j+1].a;
				intervals[j].b = intervals[j+1].b;
			}
			len --, i --;
		}
	}
	return len;
}

/**
* @brief use bianrysearch to find given oneinterval in intervals.
*
* @param intervals a set intervals.
* @param len intervals length.
* @param oneinterval one interval to find.
*
* @return return 1 if found, otherwise return 0.
*/
int intervals_find(INTERVAL* intervals, int len, INTERVAL oneinterval) {
	int low = 0, high = len-1, mid = 0;
	while (low <= high) {
		mid = (int)(low+high)/2;
		if (intervals[mid].a == oneinterval.a) {
			break;
		} else if (intervals[mid].a < oneinterval.a) {
			low = mid + 1;
		} else {
			high = mid - 1;
		}
	}
	if (intervals[mid].a <= oneinterval.a) {
		if (intervals[mid].b >= oneinterval.b) {
			return 1;
		}
	} else if (mid-1 >= 0) {
		if (intervals[mid-1].b >= oneinterval.b) {
			return 1;
		}
	}
	return 0;
}

/**
* @brief Interval overlap judgment.
*
* @param intervals a set intervals.
* @param len intervals length.
* @param oneinterval one interval to judge.
*
* @return return 1 if overlap, otherwise return 0 means not overlap, and return
* -1 means some error occur.
*/
int interval_overlap_judgment(INTERVAL* intervals, int len, 
	INTERVAL oneinterval) {
	if (!intervals_varify(intervals, len)) {
		return -1;
	}
	// order Intervals
	intervals_order(intervals, len);
	
	// merger Intervals
	len = intervals_merger(intervals, len);
	
	// use bianrysearch to find one interval
	return intervals_find(intervals, len, oneinterval);
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
int* index_mapping_build(INTERVAL* intervals, int len, int* pindex_count) {
	// malloc index_map array space.
	int size = 2*len;
	int* index_map = (int*) malloc(sizeof(int)*size);
	int i = 0, j = 0;
	for (i = 0; i < len; i ++) {
		index_map[2*i] = intervals[i].a;
		index_map[2*i+1] = intervals[i].b;
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
	if (i < 0 && index_map[0] > value) return -1;
	return ++ i;
}

int index_mapping_get_upper_index(int* index_map, int size, int value) {
	int i = 0;
	while (i < size && index_map[i] <= value) {
		i ++;
	}
	if (i >= size && index_map[size-1] < value) return -1;
	return i;
}

/**
* @brief Interval overlap judgment use interval tree algorithm.
*
* @param intervals a set intervals.
* @param len intervals length.
* @param oneinterval one interval to judge.
*
* @return return 1 if overlap, otherwise return 0 means not overlap, and return
* -1 means some error occur.
*/
int interval_overlap_judgment_use_intervaltree(INTERVAL* intervals, int len, 
	INTERVAL oneinterval) {
	if (!intervals_varify(intervals, len)) {
		return -1;
	}
	if (!intervals_varify(&oneinterval, 1)) {
		return -1;
	}
	
	// build index mapping
	int index_count = 0;
	int* index_map = index_mapping_build(intervals, len, &index_count);
	
	// build interval tree
	INTERVAL_TREE_NODE* tree = interval_tree_build(0, index_count);
	int i = 0;
	for (i = 0; i < len; i ++) {
		interval_tree_add(tree, 
			index_mapping_get_index(index_map, index_count, intervals[i].a), 
			index_mapping_get_index(index_map, index_count, intervals[i].b) );
	}
	// find interval
	int a = index_mapping_get_lower_index(index_map, index_count, 
		oneinterval.a);
	int b = index_mapping_get_upper_index(index_map, index_count, 
		oneinterval.b);
	int r = 0;
	if (-1 != a && -1 != b) {
		r = interval_tree_find(tree, a, b);
	}

	// release memory
	interval_tree_destory(tree);
	free(index_map);
	return r;
}

int main(int argc, const char *argv[])
{
	INTERVAL intervals[] = {{2, 3}, /*{2, 2}, */{3, 9}, {2, 6}};
	INTERVAL oneinterval = {7, 6};
	// printf("interval overlap judgment return %d\n", 
	// 	interval_overlap_judgment(intervals, 
	// 		sizeof(intervals)/sizeof(intervals[0]), oneinterval) );
	printf("interval overlap judgment return %d\n", 
		interval_overlap_judgment_use_intervaltree(intervals, 
			sizeof(intervals)/sizeof(intervals[0]), oneinterval) );
	return 0;
}
