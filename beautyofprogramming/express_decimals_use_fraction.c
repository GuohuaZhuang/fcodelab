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
* @file express_decimals_use_fraction.c
* @brief express decimals use fraction.
* @author firstboy0513
* @version 0.0.1
* @date 2013-12-17
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

/**
* @brief calculate greatest common divisor of two number.
*
* @param x one number.
* @param y another number.
*
* @return the greatest common divisor of two number.
*/
int greatest_common_divisor(int x, int y) {
	return (y ? greatest_common_divisor(y, x%y) : x);
}

/**
* @brief express decimals use fraction.
* If input is just a finite decimals X without recurring, then:
*     X = 0.a1a2...an = a1a2...an/(10^n)
* Otherwise a finite part and recurring part, then:
*     X = 0.a1a2...an(b1b2...bm) = (a1a2...an+0.(b1b2...bm))/(10^n)
* and suppose Y = 0.(b1b2...bm), we can know Y*(10^m) = b1b2...bm.(b1b2...bm)
* subtract it, we can get Y(10^m-1) = b1b2...bm, so Y:
*     Y = b1b2...bm/(10^m-1)
* so, the decimals with recurring X is:
*     X = (a1a2...an+b1b2...bm/(10^m-1))/(10^n)
*       = (a1a2...an*(10^m-1) + b1b2...bm)/(10^n * (10^m-1))
*
* @param finite finite part in decimal.
* @param recur recurring part in decimal.
* @param pnumerator output numerator.
* @param pdenominator output denominator.
*/
void express_decimals_use_fraction(const double finite, const int recur,
	int* pnumerator, int* pdenominator) {
	if (!pnumerator || !pdenominator) {
		printf("[ERR] express_decimals_use_fraction input invalid!\n");
		return;
	}
	int a = 0, b = recur, n = 0, m = 1;
	while (a = finite*pow(10,n), (finite-a < 10e-10)) { n ++; } // <10e-10 equal
	*pnumerator = a, *pdenominator = pow(10,n);

	if (recur) {
		while (b /= 10) { m ++; }
		*pnumerator = *pnumerator * (pow(10,m)-1) + recur;
		*pdenominator *= (pow(10,m)-1);
	}
	int gcd = greatest_common_divisor(*pdenominator, *pnumerator);
	*pnumerator /= gcd; *pdenominator /= gcd;
}

int main(int argc, const char *argv[])
{
	// const double finite = 0.3; const int recur = 33;
	const double finite = 0.285714; const int recur = 285714;
	int numerator = 0, denominator = 0;
	express_decimals_use_fraction(finite, recur, &numerator, &denominator);
	printf("%10.10g(%d) = %d/%d\n", finite, recur, numerator, denominator);
	return 0;
}
