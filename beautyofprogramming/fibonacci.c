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
* @file fibonacci.c
* @brief compute fibonacci value.
* @author firstboy0513
* @version 0.0.1
* @date 2013-12-17
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define SWAP(a, b) { (a) ^= (b); (b) ^= (a); (a) ^= (b); }

/**
* @brief compute fibonacci use recursion.
*
* @param n a number to compute its fibonacci value.
*
* @return fibonacci value.
*/
int fibonacci_recursion(int n) {
	if (n <= 1) { return n; };
	return fibonacci_recursion(n-2) + fibonacci_recursion(n-1);
}

/**
* @brief compute fibonacci use previous two value.
*
* @param n a number to compute its fibonacci value.
*
* @return fibonacci value.
*/
int fibonacci_previous(int n) {
	int i = 0, a = 0, b = 1;
	while (i < n) { i ++; a += b; SWAP(a, b); }
	return a;
}

int main(int argc, const char *argv[])
{
	const int n = 20;
	printf("fibonacci_recursion = %d\n", fibonacci_recursion(n));
	printf("fibonacci_previous = %d\n", fibonacci_previous(n));
	return 0;
}
