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
* @file nim_game_somepilesstone.c
* @brief nim game of some piles stone.
* @author firstboy0513
* @version 0.0.1
* @date 2014-01-03
*/
#include <stdio.h>
#include <stdlib.h>

/**
* @brief nim game of some piles stone to get next win stratery if will 
* definitely win.
*
* @param pilestone some piles stone number array.
* @param size piles count.
*
* @return return 1 means will definitely win and change the pilestone,
* otherwise may be lose, so not change the pilestone number array.
*/
int nim_game_somepilesstone_nextstrategy(int* pilestone, int size) {
	if (!pilestone || size <= 0) { return 0; }
	// get xor end result
	int i = 0, xor = pilestone[0];
	for (i = 1; i < size; i ++) { xor ^= pilestone[i]; }
	if (!xor) { return 0; }

	// find the first bit to change
	int bitcount = sizeof(xor), one = 0x01, firstbit = 0;
	for (i = 0; i < bitcount; i ++) {
		if (xor & one) { firstbit = one; }
		one <<= 1;
	}
	int mask = ~(firstbit - 1);
	
	// find the mininum element to change
	int min = 0x7fff, min_index = size, cur = 0;
	for (i = 0; i < size; i ++) {
		cur = mask & pilestone[i];
		if (min > cur) { min = cur; min_index = i; }
	}
	
	// change the minimum element
	min = pilestone[min_index];
	pilestone[min_index] = ((~xor) & min) | ((~(xor & min) & xor));
	
	return 1;
}

/**
* @brief output pile stone situation.
*
* @param pilestone pile stone number array.
* @param size pile count.
*/
void output_pilestone(const int* pilestone, const int size) {
	int i = 0;
	for (i = 0; i < size; i ++) {
		if (0 != i) { printf(", "); }
		printf("%d", pilestone[i]);
	}
	printf("\n");
}

int main(int argc, const char *argv[])
{
	int pilestone[] = {3, 4, 5};
	int size = sizeof(pilestone) / sizeof(pilestone[0]);
	printf("The origin pile stone situation is:\n\t");
	output_pilestone(pilestone, size);

	int definitely_win = nim_game_somepilesstone_nextstrategy(pilestone, size);
	if (definitely_win) {
		printf("Haha, you will win when next stratery is:\n\t");
		output_pilestone(pilestone, size);
	} else {
		printf("Dude, there not definitely win stratery to do!\n");
	}
	return 0;
}
