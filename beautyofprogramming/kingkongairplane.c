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
* @file kingkongairplane.c
* @brief King Kong airplane problem.
* Note: the extend problem require the order from 1 to n of people board the 
* plane is remove, without order it, we can easy know the same probability as 
* the ordered people board the plane.
* @author firstboy0513
* @version 0.0.1
* @date 2013-12-31
*/
#include <stdio.h>
#include <stdlib.h>

/**
* @brief probability of random sit own position.
* The random sit own position just see it as and full permutation, so the 
* probability is (N-1)!/N! == 1/N, the N is the count of people want to board 
* the plane, include the king kong.
*
* @param n total people count or sits on the airplane..
*
* @return return probability of random sit own position.
*/
double probability_random_sit_own_position(int n) {
	return (1.0/n);
}

/**
* @brief probability of priority sit own position.
* This problem is a bit more complex, analysis it in the following.
* Suppose king kong select the nth seat, and now we want to get the ith people 
* probability.
* If n > i, that is to say it is 100% to let ith people sit his own seat, 
* because the ith seat is blank, and he want to sit, so why not, uh.
* If n == i, he can not sit his own seat, because he can not beat king kong.
* If n < i, his seat may be sit by the front people, although not by king kong.
* Accroding to Bayesian probability formula, or the full probability formula, 
* when the n+1 people select the king kong's seat, then ith people is have his 
* own seat, if the n+1 people select the other seat, not the king kong's seat, 
* then we can see it as another king kong, because his is overbearing like 
* king kong for the following people who want board the plane. It is the 
* 1/(N-n+1) * f(n+1), So on the n+2 is the same probability.
*   f(n) = 1/(N-n+1) + 1/(N-n+1)*f(n+1) + 1/(N-n+1)*f(n+2) + ... 1/(N-n+1)*f(N)
*   f(n) = 1/(N-n+1) * ( 1 + f(n+1) + f(n+2) + ... + f(N) )
* then we can get:
*   f(n+1) = 1/(N-n) * ( 1 + f(n+2) + ... + f(N) )
* so, transposition denominator and subtract them, we can get:
*   f(n) * (N-n+1) - f(n+1) * (N-n) = f(n+1)
* At the end, we got f(n) = f(n+1) if n+1 < i. So, f(i):
*   f(i) = f(i-1), it means if king kong sit the (i-1)th seat, then the (i-1)th
* people is not sit to the ith seat is ok for ith people to sit his own seat.
*   f(i-1) = 1-1/(N-(i-1)+1) = 1-1/(N-i+2) = (N-i+1)/(N-i+2)
*
* @param n total people count or sits on the airplane.
* @param i the ith people.
*
* @return return probability of priority sit own position.
*/
double probability_priority_sit_own_position(int n, int i) {
	return ((double)n-i+1)/(n-i+2);
}

int main(int argc, const char *argv[])
{
	int n = 100;
	printf("n = 100, probability_random_sit_own_position = %g\n", 
		probability_random_sit_own_position(n));
	int i = 0;
	for (i = 2; i <= n; i ++) {
		printf("n = 100, i = %d, probability_priority_sit_own_position = %g\n", 
			i, probability_priority_sit_own_position(n, i));
	}
	return 0;
}
