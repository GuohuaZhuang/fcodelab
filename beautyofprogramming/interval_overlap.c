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
	while (len-- > 0) {
		if (intervals[len].a > intervals[len].b) {
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
			intervals[i].b = intervals[i+1].b;
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
	if(!intervals_varify(intervals, len)) {
		return -1;
	}
	// order Intervals
	intervals_order(intervals, len);
	
	// merger Intervals
	len = intervals_merger(intervals, len);
	
	// use bianrysearch to find one interval
	return intervals_find(intervals, len, oneinterval);
}

int main(int argc, const char *argv[])
{
	INTERVAL intervals[] = {{2, 3}, {1, 2}, {3, 9}, {2, 6}};
	INTERVAL oneinterval = {1, 6};
	printf("interval overlap judgment return %d\n", 
		interval_overlap_judgment(intervals, 
			sizeof(intervals)/sizeof(intervals[0]), oneinterval) );
	return 0;
}
