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
* @file nim_game_twopilesstone.c
* @brief NIM game of two piles stone. Also the same as Wythoff game.
* The Wythoff game can see more in http://baike.baidu.com/view/1952620.htm
* @author firstboy0513
* @version 0.0.1
* @date 2014-01-01
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SWAP_INT(x,y) { (x) ^= (y); (y) ^= (x); (x) ^= (y); }

/**
* @brief Give nim game of two piles of stone count, to judge whether the first 
* person take the stone will definitely win the game. The result also see it as
* a safety situation when it is 1, also means as true. And if return 0 means 
* unsafety situation. This method is use formula of x = [a*n], y = [b*n].
* and the a, b in accordance with 1/a + 1/b = 1 and b - a = n;
* so the a and b is accord to Betti's theorem, as 1/a + 1/b = 1, and 
* {[a*n]|n belong to N}, {[b*n]|n belong to N}. As we add b - a = n, so the 
* a and b can solve them to a = (1+sqrt(5))/2, b = (3+sqrt(5))/2.
*
* About Betti's theorem prove, see more in refereces.
* refereces:
* 	http://www.cis.umac.mo/~fstitl/10mmo/betty.html
*
* @param x one pile stone count.
* @param y another pile stone count.
*
* @return return 1 means the first person take the stone will definitely win 
* the game, otherwise he will lose the game if another person is sane in 
* theoretically.
*/
int nim_game_twopilesstone_formula(int x, int y) {
	double a = (1.0 + sqrt(5))/2.0;
	if (x > y) { SWAP_INT(x,y) }
	return ((x != (int)floor((y - x) * a)) ? 1 : 0);
}

int main(int argc, const char *argv[])
{
	int x = 12, y = 20;
	printf("nim_game_twopilesstone_formula(%d, %d) = %d\n", 
		x, y, nim_game_twopilesstone_formula(x, y));

	return 0;
}
