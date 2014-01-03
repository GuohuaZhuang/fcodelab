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
* @file nim_game_onepilestone.cpp
* @brief nim game of one pile stone to judge the win strategy or not.
* @author firstboy0513
* @version 0.0.1
* @date 2014-01-03
*/
#include <cstdio>
#include <cstdlib>
#include <cmath>

#include <map>
#include <string>

using std::map;
using std::string;

/**
* @brief output win strategy if has definitely win the game.
* This nim game has one pile stone, and make rules that the last take stone 
* will win this nim game.
* Output format:
*     1_1_2 means 1 stone, then blank place not consecutive stone, then another
* 1 stone, and also blank, and then 2 consecutive stone. the '_' underline 
* means a split blank let its left stone and right stone are not consecutive.
*
* @param count the count of one pile stone.
* @param k one can take the maximum count stone.
*
* @return return 1 means has definitely win the game, otherwise return 0 means
* not definitely win the game.
*/
int output_nim_lasttakewin_win_strategy(const int count, const int k) {
	printf("nim last take win game with count = %d, k = %d \nwill definitely "
		"win the game, and first strategy: ", count, k);
	int twoside = (int) floor((count + 1) / 2) - 1;
	printf("<%d_%d>\n", twoside, twoside);
	return 1;
}

int main(int argc, const char *argv[])
{
	printf("nim last take win game will win strategy? %s\n", 
		(output_nim_lasttakewin_win_strategy(10, 2) == 1 ? "yes" : "no"));
	return 0;
}
