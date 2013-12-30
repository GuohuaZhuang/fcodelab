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
* @file chinese_chess_generals_problem.c
* @brief solution for Chinese chess generals problem.
* @author firstboy0513
* @version 0.0.1
* @date 2013-12-30
*/
#include <stdio.h>
#include <stdlib.h>

#define HALF_BITS_LENGTH 4
#define FULL_MASK 255
#define RMASK (FULL_MASK >> HALF_BITS_LENGTH)
#define LMASK (FULL_MASK << HALF_BITS_LENGTH)
#define RSET(b, n) ((b) = ((LMASK & (b)) | (n)))
#define LSET(b, n) ((b) = ((RMASK & (b)) | ((n) << HALF_BITS_LENGTH)))
#define RGET(b) (RMASK & (b))
#define LGET(b) ((LMASK & (b)) >> HALF_BITS_LENGTH)
#define GRIDWIDTH 3

/**
* @brief output all generals valid postions use bit mask.
*/
void output_all_generals_valid_positions_use_mask() {
	unsigned char b = 0;
	for (LSET(b, 1); LGET(b) <= GRIDWIDTH*GRIDWIDTH; LSET(b, LGET(b)+1)) {
		for (RSET(b, 1); RGET(b) <= GRIDWIDTH*GRIDWIDTH; RSET(b, RGET(b)+1)) {
			if (LGET(b)%GRIDWIDTH != RGET(b)%GRIDWIDTH) {
				printf("A = %d, B = %d\n", LGET(b), RGET(b));
			}
		}
	}
}

/**
* @brief output all generals valid postions use all range from 1 to 9*9.
*/
void output_all_generals_valid_positions_use_allrange() {
	unsigned char x = 81;
	while (x --) {
		if ( (x/9)%3 != (x%9)%3 ) {
			printf("A = %d, B = %d\n", (x/9)+1, (x%9)+1);
		}
	}
}

/**
* @brief position of Chinese chess two generals position from 1 to 9.
*/
typedef struct _POSITION {
	unsigned char a:4;
	unsigned char b:4;
} POSITION;

/**
* @brief output all generals valid postions.
*/
void output_all_generals_valid_positions() {
	POSITION p;
	for (p.a = 1; p.a <= 9; p.a ++) {
		for (p.b = 1; p.b <= 9; p.b ++) {
			if (p.a % 3 != p.b % 3) {
				printf("A = %d, B = %d\n", p.a, p.b);
			}
		}
	}
}

int main(int argc, const char *argv[])
{
	output_all_generals_valid_positions_use_mask();
	// output_all_generals_valid_positions_use_allrange();
	// output_all_generals_valid_positions();
	return 0;
}
