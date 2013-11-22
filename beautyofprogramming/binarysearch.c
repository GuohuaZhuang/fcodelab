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
* @file binarysearch.c
* @brief Beauty of Programming about binarysearch
* @author firstboy0513
* @version 0.0.1
* @date 2013-11-23
* compiler: gcc binarysearch.c -o binarysearch -g3 -O0 -Wall
*/
#include <stdio.h>
#include <string.h>

/**
* @brief binarysearch
* Give a ascending array, find index of x in it.
*
* @param arr A ascending array.
* @param arr_count arr element count.
* @param x the element value to find.
*
* @return Return finded element index in arr, otherwise return -1.
*/
int binarysearch(int* arr, int arr_count, int x) {
	int leftindex = 0, rightindex = arr_count - 1, middleindex = 0;
	while (leftindex < rightindex) {
		middleindex = leftindex + ((rightindex - leftindex) / 2);
		if (arr[middleindex] == x) {
			return middleindex;
		} else if (arr[middleindex] < x) {
			leftindex = middleindex + 1;
		} else {
			rightindex = middleindex - 1;
		}
	}
	if (arr[leftindex] == x) {
		return leftindex;
	}
	return -1;
}

/**
* @brief binarysearch_maxindex
* Give a ascending array, find maxindex of x in it.
*
* @param arr A ascending array.
* @param arr_count arr element count.
* @param x the element value to find.
*
* @return Return finded element index in arr, otherwise return -1.
*/
int binarysearch_maxindex(int* arr, int arr_count, int x) {
	int leftindex = 0, rightindex = arr_count - 1, middleindex = 0;
	while (leftindex < rightindex) {
		middleindex = leftindex + ((rightindex - leftindex) / 2);
		if (arr[middleindex] == x) {
			int findedindex = middleindex;
			while (++middleindex <= rightindex && arr[middleindex] == x) {
				findedindex = middleindex;
			}
			return findedindex;
		} else if (arr[middleindex] < x) {
			leftindex = middleindex + 1;
		} else {
			rightindex = middleindex - 1;
		}
	}
	if (arr[leftindex] == x) {
		return leftindex;
	}
	return -1;
}

/**
* @brief binarysearch_minindex
* Give a ascending array, find minindex of x in it.
*
* @param arr A ascending array.
* @param arr_count arr element count.
* @param x the element value to find.
*
* @return Return finded element index in arr, otherwise return -1.
*/
int binarysearch_minindex(int* arr, int arr_count, int x) {
	int leftindex = 0, rightindex = arr_count - 1, middleindex = 0;
	while (leftindex < rightindex) {
		middleindex = leftindex + ((rightindex - leftindex) / 2);
		if (arr[middleindex] == x) {
			int findedindex = middleindex;
			while (--middleindex >= leftindex && arr[middleindex] == x) {
				findedindex = middleindex;
			}
			return findedindex;
		} else if (arr[middleindex] < x) {
			leftindex = middleindex + 1;
		} else {
			rightindex = middleindex - 1;
		}
	}
	if (arr[leftindex] == x) {
		return leftindex;
	}
	return -1;
}

/**
* @brief find_maxindex_lessthan
* Give a ascending array and current middleindex, use left traversal to find 
* first element which less than x in array.
*
* @param arr A ascending array.
* @param middleindex current middleindex.
* @param x the element value to find.
*
* @return Return finded index, otherwise return -1.
*/
int find_maxindex_lessthan(int* arr, int middleindex, int x) {
	while (middleindex >= 0 && arr[middleindex] >= x) {
		middleindex --;
	}
	if (middleindex < 0) {
		return -1;
	} else {
		return middleindex;
	}
}

/**
* @brief binarysearch_maxindex_lessthan
* Give a ascending array, find maxindex of first element which less than x in 
* it.
*
* @param arr A ascending array.
* @param arr_count arr element count.
* @param x the element value to find.
*
* @return Return finded element index in arr, otherwise return -1.
*/
int binarysearch_maxindex_lessthan(int* arr, int arr_count, int x) {
	int leftindex = 0, rightindex = arr_count - 1, middleindex = 0;
	while (leftindex < rightindex) {
		middleindex = leftindex + ((rightindex - leftindex) / 2);
		if (arr[middleindex] == x) {
			return find_maxindex_lessthan(arr, middleindex, x);
		} else if (arr[middleindex] < x) {
			leftindex = middleindex + 1;
		} else {
			rightindex = middleindex - 1;
		}
	}
	return find_maxindex_lessthan(arr, leftindex, x);
}

/**
* @brief find_minindex_greaterthan
* Give a ascending array and current middleindex, use right traversal to find 
* first element which greater than x in array.
*
* @param arr A ascending array.
* @param middleindex current middleindex.
* @param x the element value to find.
*
* @return Return finded index, otherwise return -1.
*/
int find_minindex_greaterthan(int* arr, int arr_count, int middleindex, 
	int x) {
	int maxindex = arr_count - 1;
	while (middleindex <= maxindex && arr[middleindex] <= x) {
		middleindex ++;
	}
	if (middleindex > maxindex) {
		return -1;
	} else {
		return middleindex;
	}
}

/**
* @brief binarysearch_minindex_greaterthan
* Give a ascending array, find minindex of first element which greater than x 
* in it.
*
* @param arr A ascending array.
* @param arr_count arr element count.
* @param x the element value to find.
*
* @return Return finded element index in arr, otherwise return -1.
*/
int binarysearch_minindex_greaterthan(int* arr, int arr_count, int x) {
	int leftindex = 0, rightindex = arr_count - 1, middleindex = 0;
	while (leftindex < rightindex) {
		middleindex = leftindex + ((rightindex - leftindex) / 2);
		if (arr[middleindex] == x) {
			return find_minindex_greaterthan(arr, arr_count, middleindex, x);
		} else if (arr[middleindex] < x) {
			leftindex = middleindex + 1;
		} else {
			rightindex = middleindex - 1;
		}
	}
	return find_minindex_greaterthan(arr, arr_count, leftindex, x);
}


int main(int argc, const char *argv[])
{
	int arr[] = {1, 4, 5, 6, 24, 24, 24, 24, 58, 59, 96, 99, 104};
	//           0, 1, 2, 3, 4 , 5 , 6 , 7 , 8 , 9 , 10, 11, 12
	int x = 0;
	printf("Please input x: ");
	scanf("%d", &x);
	// int ret = binarysearch_minindex(
	// 	(int*)arr, (sizeof(arr)/sizeof(arr[0])), x);
	// int ret = binarysearch_maxindex_lessthan(
	// 	(int*)arr, (sizeof(arr)/sizeof(arr[0])), x);
	int ret = binarysearch_minindex_greaterthan(
		(int*)arr, (sizeof(arr)/sizeof(arr[0])), x);
	if (ret >= 0) {
		printf("Getted it ret = %d\n", ret);
	} else {
		printf("Dammit, can\'t find %d in array.\n", x);
	}

	return 0;
}

