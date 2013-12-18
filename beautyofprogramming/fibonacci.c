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
#include <math.h>
#include <string.h>

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

/**
* @brief compute fibonacci use general term formula by derivate by 
* characteristic equation.
* With recurrence formula of fibonacci:
*     F(n) = F(n-1) + F(n-2)
* so find r and s to make:
*     F(n) - r*F(n-1) = s*(F(n-1) - r*F(n-2))
*     F(n) = (s+r)*F(n-1) - (s*r)*F(n-2)
* Then, 
*     s+r = 1
*     s*r = 1
* So, r^2 - r - 1 = 0 (or s^2 - r - 1 = 0), so the characteristic formula is:
*     x^2 = x + 1
* and the solution of it is use general derivation (-b+sqrt(b^2-4ac))/2a and 
* (-b-sqrt(b^2-4ac))/2a to get it:
*     x1 = (1+sqrt(5))/2, x2 = (1-sqrt(5))/2
* x1 != x2, so the f(n) = A*x1^n + B*X2^2.
* So we can get f(0) = 0, f(1) = 1, so the we can get A=1/sqrt(5), B=-1/sqrt(5)
*     F(n) = (1/sqrt(5)) * [((1+sqrt(5))/2)^n - ((1-sqrt(5))/2)^n]
*
* @param n a number to compute its fibonacci value.
*
* @return fibonacci value.
*/
int fibonacci_formula(int n) {
	double sqrt_5 = sqrt(5);
	return (1/sqrt_5) * ( pow((1+sqrt_5)/2, n) - pow((1-sqrt_5)/2, n));
}

// 
// USE MATRIX TO COMPUTE FIBONACCI, TEMPORARITY PLACED HERE.
// typedef struct _MATRIX {
// 	int x[4];
// } MATRIX;
// 
// MATRIX* matrix_identity() {
// 	MATRIX* m = (MATRIX*) malloc(sizeof(MATRIX));
// 	memset(m, 0, sizeof(MATRIX));
// 	return m;
// }
// 
// MATRIX* matrix_copy(const MATRIX* m) {
// 	MATRIX* m_copy = (MATRIX*) malloc(sizeof(MATRIX));
// 	memcpy(m_copy, m, sizeof(MATRIX));
// 	return m_copy;
// }
// 
// MATRIX* matrix_pow(const MATRIX* m, int n) {
// 	MATRIX* result = matrix_identity();
// 	MATRIX* tmp = matrix_copy(m);
// 	for (; n; n >>= 1) {
// 		if (n & 1) { result *= tmp; }
// 		tmp *= tmp;
// 	}
// 	free(tmp);
// 	return result;
// }
// 
// int fibonacci_matrix(int n) {
// 	MATRIX* an = matrix_pow(A, n-1);
// 	return F1 * an(0, 0) + F0 * an(1, 0);
// }
// 

int main(int argc, const char *argv[])
{
	const int n = 20;
	printf("fibonacci_recursion = %d\n", fibonacci_recursion(n));
	printf("fibonacci_previous = %d\n", fibonacci_previous(n));
	printf("fibonacci_formula = %d\n", fibonacci_formula(n));
	return 0;
}
