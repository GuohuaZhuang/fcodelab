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
* @file multiply_only_one_and_zero.c
* @brief given a number N, find a number M to make N*M result to a number which
* is only make up by one and zero in decimal.
* @author firstboy0513
* @version 0.0.1
* @date 2013-12-17
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#define NUMBER  int
#define NF      "%d"
#define MAX_BIT 31
#define ERR_NUM INT32_MIN

/**
* @brief given a number N, find a number M to make N*M result to a number which
* is only make up by one and zero in decimal.
*
* @param N a given number.
*
* @return find that number M.
*/
NUMBER multiply_only_one_and_zero(const NUMBER N) {
	NUMBER k = 0, X = 1;
	while (k < MAX_BIT) {
		if (0 == X % N) {
			return (X/N);
		}
		k = 0;
		while (k < MAX_BIT) {
			if (0 == ((NUMBER)(X/pow(10,k)))%10) {
				X += pow(10,k); break;
			}
			X -= pow(10,k); k ++;
		}
		if (k > MAX_BIT || X < 0) break;
	}
	return ERR_NUM;
}

int main(int argc, const char *argv[])
{
	const NUMBER N = 3;
	NUMBER M = multiply_only_one_and_zero(N);
	if (ERR_NUM != M) {
		printf(""NF"*"NF" = "NF"\n", N, M, N*M);
	} else {
		printf("Can not find the M to make N*M = only one and zero number.\n");
	}
	return 0;
}
