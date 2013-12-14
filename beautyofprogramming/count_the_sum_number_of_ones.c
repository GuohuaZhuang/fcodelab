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
* @file count_the_sum_number_of_ones.c
* @brief count the number of ones by given a number n.
* @author firstboy0513
* @version 0.0.1
* @date 2013-12-14
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define NUMBER 	long int
#define NF		"%ld" // number printf format

/**
* @brief count the number of one by given the number n.
*
* @param n number n.
*
* @return the number of one.
*/
NUMBER count_the_number_of_one(NUMBER n) {
	NUMBER count = 0;
	while (n) {
		count += (1 == n % 10 ? 1 : 0);
		n /= 10;
	}
	return count;
}

/**
* @brief count the number of one from 1 to n.
*
* @param n number n.
*
* @return the total number of one.
*/
NUMBER count_the_number_of_one_from_one_to_n(NUMBER n) {
	NUMBER i = 0, count = 0;
	for (i = 1; i <= n; i ++) {
		count += count_the_number_of_one(i);
	}
	return count;
}

/**
* @brief count the number of one from 1 to n use piecewise to three situation.
* when current number bit is:
* if 0: count += (higher * factor);
* if 1: count += (higher * factor) + (lower + 1);
* if >1: count += (higher+1) * factor;
* This method use a little time more then the above function exhaustive method.
*
* @param n number n.
*
* @return the total number of one.
*/
NUMBER count_the_number_of_one_from_one_to_n_use_piecewise(NUMBER n) {
	NUMBER lower = 0, higher = 0, factor = 1, count = 0, number = 0;
	while (0 != n / factor) {
		lower = n - (n / factor) * factor;
		number = (n / factor) % 10;
		higher = n / factor / 10;
		switch(number) {
			case 0:
				count += (higher * factor);
				break;
			case 1:
				count += (higher * factor) + (lower + 1);
				break;
			default:
				count += (higher + 1) * factor;
		}
		factor *= 10;
	}
	return count;
}

int main(int argc, const char *argv[])
{
	NUMBER n = 12;
	printf("count the number of one from 1 to "NF" = "NF"\n", n, 
		count_the_number_of_one_from_one_to_n(n));
	printf("count the number of one from 1 to "NF" = "NF"\n", n, 
		count_the_number_of_one_from_one_to_n_use_piecewise(n));

	NUMBER i = 0;
	for (i = 1023; i < 1025; i ++) {
		printf("f("NF") = "NF"", i, count_the_number_of_one_from_one_to_n(i));
		printf(", f("NF") = "NF"\n", i, 
			count_the_number_of_one_from_one_to_n_use_piecewise(i));
	}

	// find the biggest number which suffice f(n) = n.
	// we know 1~n is a increment series, and f(n) is also a increment series.
	// and we found this:
	// f(9) = 1
	// f(99) = 20
	// f(999) = 300
	// f(9,999) = 4,000
	// ...
	// f(999,999,999) = 900,000,000
	// f(9,999,999,999) = 10,000,000,000
	// we can get f(10^n-1) = n*10^(n-1), and when n=10, 10^10-1 = 10*10^9=10^10
	// that it to say f(10^10-1) < 10*10^(10-1), and f(1) = 1, f(2) = 1.
	// According to zero-value theorem or imtermediate-value theorem, we can be
	// sure that between 2 and 10^10-1 there is a biggest number the suffice
	// f(x) = x. And it is more possible closer to 10^10-1, so we begin find
	// from 10^10-1 to 2.
	// the end result is 1111111110. is may run some seconds.
	// we can reduce the scope by manual. 10^9-1=999999999 < 9*10^8=900000000
	// how about use binarysearch method?
	NUMBER count = 0, larger = 10000000000-1, small = 1000000000-1, middle = 0;
	while (larger > small) {
		middle = (larger + small) / 2;
		count = count_the_number_of_one_from_one_to_n_use_piecewise(middle);
		if (middle == count) {
			printf("we found the biggest f("NF") = "NF"\n", middle, middle);
			break;
		} else if (middle < count) {
			larger = middle + 1;
		} else {
			small = middle - 1;
		}
	}

	return 0;
}
