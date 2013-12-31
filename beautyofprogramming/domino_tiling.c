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
* @file domino_tiling.c
* @brief domino tiling ways.
* @author firstboy0513
* @version 0.0.1
* @date 2013-12-31
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#ifndef PI
#define PI 3.1415926
#endif // PI

/**
* @brief get domino tiling number ways use formula.
*
* @param m
* @param n
*
* @return 
*/
int domino_tiling_ways(int m, int n) {
	int j = 0, k = 0;
	double result = 1;
	for (j = 1; j <= m/2; j ++) {
		for (k = 1; k <= n/2; k ++) {
			result *= 4*pow(cos(PI*j/(m+1)), 2) + 4*pow(cos(PI*k/(n+1)), 2);
		}
	}
	return result;
}

int main(int argc, const char *argv[])
{
	int m = 0, n = 0;
	printf("input m = "); scanf("%d", &m);
	printf("input n = "); scanf("%d", &n);
	double result = domino_tiling_ways(m, n);
	printf("%d * %d tiling has %g number or ways to cover.\n", m, n, result);
	return 0;
}
