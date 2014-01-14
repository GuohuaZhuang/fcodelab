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
* @file sudoku_storage.c
* @brief sudoku mininum storage length problem, and estimate all sudoku case 
* count. This problem is not even more code, just analysis theory in the book 
* <Beauty of programming>.
* @author firstboy0513
* @version 0.0.1
* @date 2014-01-14
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/**
* @brief get factorial value.
*
* @param n input number.
*
* @return return n! factorial value.
*/
double factorial(int n) {
	if (n < 0) {
		printf("[ERR] -- factorial can not input negative!\n");
		return -1;
	}
	if (n == 0) { return 1; }
	int i = n;
	while (i -- > 1) { n *= i; }
	return (double)n;
}

#define F(n) factorial(n)

/**
* @brief get all sudoku case count.
*
* @return all sudoku case count.
*/
double get_all_sudoku_case_count() {
	double horizon_solution = F(9) * ( 2*pow(F(3),6)+3*2*9*pow(F(3),6) );
	double total_solution = pow(horizon_solution,6)/pow(F(9),9); 
	return total_solution;
}

/**
* @brief sudoku storage lower bound length in theory.
*
* @param total_solution all sudoku case count.
*
* @return return lower bound in theory.
*/
int sudoku_storage_lower_bound_length(double total_solution) {
	double lower_bound = log(total_solution)/log(2);
	return ceil(lower_bound);
}

int main(int argc, const char *argv[])
{
	double total_solution = get_all_sudoku_case_count();
	printf("total_solution = %g\n", total_solution);

	printf("can use 8*8 less than 9*9 to storage length.\n");
	int minimum_storage_length = 
		sudoku_storage_lower_bound_length(total_solution);
	printf("the lower bound in theory is: %d\n", minimum_storage_length);

	return 0;
}
