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
* @file getdayofweek.c
* @brief get day of week use Zeller's congruence.
* references:
* 	http://en.wikipedia.org/wiki/Determination_of_the_day_of_the_week
* 	http://en.wikipedia.org/wiki/Zeller%27s_congruence
* @author firstboy0513
* @version 0.0.1
* @date 2014-02-18
*/
#include <stdio.h>

/**
* @brief This is a variant from Zeller's congruence algorithm, it calculate week
* without use c, and not get negative result to use (w%7+7)%7.
* It is now a good implement I found it in the QDBM project.
*
* @param year year.
* @param month month.
* @param day day.
*
* @return 0 means sunday, 1-6 are from monday to saturday.
*/
int getdayofweek(int year, int month, int day) {
	if (month < 3) { year --; month += 12; }
	return (day + ((8 + (13 * month)) / 5) + (year + (year / 4) - (year / 100) 
		+ (year / 400))) % 7;
}

/**
* @brief This is the orginal Zeller's congruence algorithm.
*
* @param year year.
* @param month month.
* @param day day.
*
* @return 0 means sunday, 1-6 are from monday to saturday.
*/
int getdayofweek2(int year, int month, int day) {
	if (month < 3) { year --; month += 12; }
	int c = year / 100, y = year % 100, m = month, d = day;
	int w = (y + y/4 + c/4 - 2*c + 26*(m+1)/10 + d - 1);
	w = (w % 7 + 7) % 7;
	return w;
}

int main(int argc, const char *argv[])
{
	int y = 0, m = 0, d = 0;
	printf("input year: "); scanf("%d", &y);
	printf("input month: "); scanf("%d", &m);
	printf("input day: "); scanf("%d", &d);
	printf("getdayofweek(%d, %d, %d) = %d\n", y, m, d, getdayofweek(y, m, d));
	printf("getdayofweek2(%d, %d, %d) = %d\n", y, m, d, getdayofweek2(y, m, d));

	return 0;
}
