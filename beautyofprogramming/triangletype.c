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
* @file triangletype.c
* @brief some test case for triangle type method.
* @author firstboy0513
* @version 0.0.1
* @date 2013-12-23
*/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/**
* @brief get triangle type.
*
* @param a one edge length of triangle.
* @param b one edge length of triangle.
* @param c one edge length of triangle.
*
* @return the type of triangle which the correspond bit and type:
*     7: isvalid triangle.
*     6: not use.
*     5: not use.
*     4: is right triangle.
*     3: is acute triangle.
*     2: is obtuse triangle.
*     1: is equilateral triangle.
*     0: is isosceles triangle.
*/
char get_triangle_type(int a, int b, int c) {
	char flag = '\0';
	do {
		if (a <= 0 || b <= 0 || c <= 0) { break; }
		if (a + b <= c || a + c <= b || c + b <= a) { break; }
		flag |= 0x80;
		if (a == b || b == c || c == a) { flag |= 0x01; }
		if (a == b && b == c) { flag |= 0x02; }
		if (a > c) { a ^= c; c ^= a; a ^= c; } // swap let c to be the max edge
		if (b > c) { b ^= c; c ^= b; b ^= c; } // swap let c to be the max edge
		int twosquare = a*a + b*b, onesquare = c*c;
		if (twosquare < onesquare) { flag |= 0x04; }
		else if (twosquare > onesquare) { flag |= 0x08; }
		else { flag |= 0x10; } // twosquare == onesquare
	} while(0);
	return flag;
}

int main(int argc, const char *argv[])
{
	assert(0B00000000 == (0xff&get_triangle_type(4, 1, 2)));
	assert(0B10001011 == (0xff&get_triangle_type(5, 5, 5)));
	assert(0B10000101 == (0xff&get_triangle_type(2, 2, 3)));
	assert(0B10010000 == (0xff&get_triangle_type(3, 4, 5)));
	assert(0B10000100 == (0xff&get_triangle_type(2, 3, 4)));
	assert(0B10000100 == (0xff&get_triangle_type(100, 99, 2)));

	assert(0B00000000 == (0xff&get_triangle_type(0, 1, 2)));
	assert(0B00000000 == (0xff&get_triangle_type(-1, 1, 2)));
	// assert(0B00000000 == (0xff&get_triangle_type(a, 1, 2)));

	assert(0B10001001 == (0xff&get_triangle_type(50, 50, 1)));
	assert(0B10001001 == (0xff&get_triangle_type(50, 50, 2)));
	assert(0B10001001 == (0xff&get_triangle_type(100, 100, 99)));
	assert(0B10001011 == (0xff&get_triangle_type(100, 100, 100)));
	assert(0B00000000 == (0xff&get_triangle_type(50, 50, 100)));
	assert(0B10001011 == (0xff&get_triangle_type(1, 1, 1)));
	assert(0B00000000 == (0xff&get_triangle_type(1, 1, 2)));
	assert(0B00000000 == (0xff&get_triangle_type(1, 1, 99)));
	assert(0B00000000 == (0xff&get_triangle_type(1, 1, 100)));

	return 0;
}
