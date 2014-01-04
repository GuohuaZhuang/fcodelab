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

/**
* @brief judge and output first take win strategy is has definitely win the 
* last take lose nim game.
* [TODO]
* Note if the situation has lose game, then return it, not calculate any more.
* NOte get x/2 to reduce symmetry calculate.
* Note store result in dynamic programming map to reduce calculate.
*
* references:
* 	http://arieshout.me/2012/04/nim-problem.html
*
* @param situation situation of now stone location.
*
* @return return 1 means has definitely win the game, otherwise return 0 means
* not definitely win the game, and also not any output.
*/
int recursion_get_lasttakelose_win_strategy(string situation,
	map<string, int>& dp) {

	map<string, int>::const_iterator it = dp.find(situation);
	if (dp.end() != it) { return it->second; }

	int definitely_win = 0;

	// recurrence to take 2 consecutive stone

	// recurrence to take 1 stone

	// map["xx_xx"] = definitely_win;
	
	return definitely_win;
}

int main(int argc, const char *argv[])
{
	printf("nim last take win game will win strategy? %s\n", 
		(output_nim_lasttakewin_win_strategy(10, 2) == 1 ? "yes" : "no"));

	printf("\n");

	map<string, int> dp;
	printf("when stone count = %d, nim last take lose game %s strategy.\n", 7,
		(recursion_get_lasttakelose_win_strategy("7", dp) 
		 ? "will win" : "may lose") );

	return 0;
}
