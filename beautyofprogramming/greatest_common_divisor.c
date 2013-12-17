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
* @file greatest_common_divisor.c
* @brief greatest common divisor.
* @author firstboy0513
* @version 0.0.1
* @date 2013-12-17
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/**
* @brief calculate greatest common divisor use Euclid's algorithm.
* A much more efficient method is the Euclidean algorithm, which uses a division
* algorithm such as long division in combination with the observation that the 
* gcd of two numbers also divides their difference. To compute gcd(48,18), 
* divide 48 by 18 to get a quotient of 2 and a remainder of 12. Then divide 18 
* by 12 to get a quotient of 1 and a remainder of 6. Then divide 12 by 6 to get 
* a remainder of 0, which means that 6 is the gcd. Note that we ignored the 
* quotient in each step except to notice when the remainder reached 0, 
* signalling that we had arrived at the answer.
*
* @param x one number.
* @param y another number.
*
* @return the greatest common divisor of x and y.
*/
int greatest_common_divisor_euclidean(int x, int y) {
	return (y ? greatest_common_divisor_euclidean(y, x%y) : x);
}

/**
* @brief calculate greatest common divisor use subtraction, it also come from 
* Euclid's algorithm.
*
* @param x one number.
* @param y another number.
*
* @return the greatest common divisor of x and y.
*/
int greatest_common_divisor_subtraction(int x, int y) {
	if (0 == x) { return y; }
	if (0 == y) { return x; }
	return (x > y ? greatest_common_divisor_subtraction(x-y, y) 
				  : greatest_common_divisor_subtraction(y-x, x));
}

/**
* @brief calculate greatest common divisor use divisible by two, it can use 
* little calculate like division or many recursion.
* we know, if a prime number like k, if x = k*x1, y = k*y1, then:
*     gcd(x,y) = k*gcd(x,y)
* and if a prime number p is like x = p*x2, but y%p != 0, so:
*     gcd(x,y) = gcd(x2,y)
* so use the prime number 2 to calculate it is a good idea, because divide 2 is
* just right shift the number, it use a little cpu compute.
*
* @param x one number.
* @param y another number.
*
* @return the greatest common divisor of x and y.
*/
int greatest_common_divisor_divisiblebytwo(int x, int y) {
	if (x < y) { return greatest_common_divisor_divisiblebytwo(y, x); }
	if (0 == y) { return x; }
	if (0x01 & x) {
		if (0x01 & y) { // x and y are all odd.
			return greatest_common_divisor_divisiblebytwo(x-y, y);
		} else { // x is odd, but y is even.
			return greatest_common_divisor_divisiblebytwo(x, y>>1);
		}
	} else {
		if (0x01 & y) { // x is even, but y is odd.
			return greatest_common_divisor_divisiblebytwo(x>>1, y);
		} else { // x and y are all even.
			return (greatest_common_divisor_divisiblebytwo(x>>1, y>>1) << 1);
		}
	}
}

int main(int argc, const char *argv[])
{
	const int x = 42, y = 30;
	printf("greatest_common_divisor = %d\n", 
		greatest_common_divisor_euclidean(x, y));
	printf("greatest_common_divisor = %d\n", 
		greatest_common_divisor_subtraction(x, y));
	printf("greatest_common_divisor = %d\n", 
		greatest_common_divisor_divisiblebytwo(x, y));
	return 0;
}
