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
* @author firstboy0513
* @version 0.0.1
* @date 2013-12-31
*/
#include <stdio.h>
#include <stdlib.h>

/**
* @brief probablity of random sit own position.
*
* @param n total people count or sits on the airplane..
*
* @return return probablity of random sit own position.
*/
double probablity_random_sit_own_position(int n) {
	return (1.0/n);
}

int main(int argc, const char *argv[])
{
	int n = 100;
	printf("n = 100, probablity_random_sit_own_position = %g\n", 
		probablity_random_sit_own_position(n));
	return 0;
}
