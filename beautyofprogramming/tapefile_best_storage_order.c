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
* @file tapefile_best_storage_order.c
* @brief get the best tape file storage order.
* @author firstboy0513
* @version 0.0.1
* @date 2013-12-24
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SWAP_DOUBLE(a, b) { double _tmp = a; a = b; b = _tmp; }
#define SWAP_INT(a, b) { a ^= b; b ^= a; a ^= b; }

/**
* @brief use quick sort to sort the radio of file length and file visit 
* probability.
*
* @param radios the array of radio of file length and file visit probability.
* @param left sort area left index.
* @param right sort area right index.
* @param orders the index of order in the origin fileslength array.
*/
void quicksort_tapefile_radios_descend(double* radios, int left, int right, 
	int* orders) {
	if (left >= right) { return; }
	int i = left, j = right;
	double pivot = radios[ (i+j)/2 ];
	while (i <= j) {
		while (radios[j] < pivot) { j --; }
		while (radios[i] > pivot) { i ++; }
		if (i <= j) {
			if (i != j) {
				SWAP_DOUBLE(radios[i], radios[j])
				SWAP_INT(orders[i], orders[j])
			}
			i ++, j --;
		}
	}
	if (left < j) {
		quicksort_tapefile_radios_descend(radios, left, j, orders);
	}
	if (right > i) {
		quicksort_tapefile_radios_descend(radios, i, right, orders);
	}
}

/**
* @brief get best tapefile storage order.
*
* @param fileslength file length array.
* @param visitprobability file visit probability array.
* @param size file count.
*
* @return the best order index of file.
*/
int* tapefile_best_storage_order(const int* fileslength, 
	const double* visitprobability, const int size) {
	if (!fileslength || !visitprobability || size <= 0) {
		printf("[ERR] -- tapefile_best_storage_order input invalid!\n");
		return NULL;
	}
	int* orders = (int*) malloc(sizeof(int) * size);
	double* radios = (double*) malloc(sizeof(double) * size);
	int i = 0;
	for (i = 0; i < size; i ++) {
		orders[i] = i; radios[i] = visitprobability[i]/fileslength[i];
	}
	quicksort_tapefile_radios_descend(radios, 0, size-1, orders);
	free(radios);
	return orders;
}

int main(int argc, const char *argv[])
{
	const int fileslength[] = {10, 6};
	const double visitprobability[] = {0.4, 0.6};
	const int size = sizeof(fileslength) / sizeof(fileslength[0]);

	int* orders = tapefile_best_storage_order(fileslength, visitprobability, 
		size);

	if (orders) {
		int i = 0;
		printf("The best order is:\n\t");
		for (i = 0; i < size; i ++) {
			if (0 != i) { printf(", "); }
			printf("%d(%g)", fileslength[orders[i]], 
				visitprobability[orders[i]]);
		}
		printf("\n");
		free(orders);
	}

	return 0;
}
