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
* @file black_and_white_ball.c
* @brief judge the final ball color.
* The problem is want to determine the final situation of the bucket, which has
* some count of black ball and white ball, and every time take out two ball.
* If two ball have the save color, then give back a black ball, otherwise the 
* two ball have different color, then give back a white ball.
* The solution is think of it as a logical XOR operation. See black as 0, and 
* white as 1, then we can found (0 XOR 0 == 0), (1 XOR 1 == 0) and 
* (1 XOR 0 == 0). So we just found if the count of white ball is an even, then 
* the final ball is 0, also means a black ball. otherwise the count of white 
* ball is an odd, then the final ball is 1, also knowe as a white ball.
* @author firstboy0513
* @version 0.0.1
* @date 2013-12-25
*/
#include <stdio.h>
#include <stdlib.h>

/**
* @brief 0 means black ball, and 1 means white ball.
*/
#define BALL(b) (0 == b ? "black" : "white")

typedef enum { BLACK, WHITE } BALL_COLOR;

/**
* @brief get final ball color just determine the white ball count is even or 
* odd.
*
* @param black black ball count.
* @param white white ball count.
*
* @return the final ball color.
*/
BALL_COLOR final_ball(int black, int white) {
	if (0 == white%2) {
		return BLACK;
	}
	return WHITE;
}

int main(int argc, const char *argv[])
{
	printf("%d black ball and %d white ball, the final ball is %s ball.\n", 
		100, 100, BALL(final_ball(100, 100)));
	printf("%d black ball and %d white ball, the final ball is %s ball.\n", 
		99, 99, BALL(final_ball(99, 99)));
	printf("%d black ball and %d white ball, the final ball is %s ball.\n", 
		98, 20, BALL(final_ball(98, 20)));
	return 0;
}
