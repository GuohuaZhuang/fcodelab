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
* @version 0.0.1
* @date 2013-12-06
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct _WINDOW {
	int x0; // left bottom point x value (minimum x value).
	int y0; // left bottom point y value (minimum y value).
	int x1; // right top point x value (maximum x value).
	int y1; // right top point y value (maximum y value).
} WINDOW;

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
* @brief window completely cover judgment.
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
	return -1;
}

int main(int argc, const char *argv[])
{
	WINDOW windows[] = {{-15,0,5,10}, {-5,8,20,25}, {15,-4,24,14}, {0,-6,16,4}};
	WINDOW onewindow = {2,15,10,22};
	printf("interval window overlap judgment return %d\n",
			window_overlap_judgment(windows,
				sizeof(windows)/sizeof(windows[0]), onewindow) );
	return 0;
}
