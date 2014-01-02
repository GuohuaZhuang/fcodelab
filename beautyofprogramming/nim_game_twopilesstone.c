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

/**
* @brief add new number to cache, and realloc memory if cache capacity is not 
* enough to store cache numbers.
*
* @param cache cache array.
* @param psize output cache size.
* @param pcapacity output cache capacity size.
* @param num given a new number to add in.
*
* @return return cache array.
*/
int* cache_add_new_number(int* cache, int* psize, int* pcapacity, int num) {
	(*psize) ++;
	if (*psize >= *pcapacity) {
		*pcapacity <<= 1;
		cache = (int*)realloc(cache, sizeof(int)*(*pcapacity));
	}
	cache[(*psize)-1] = num;
	return cache;
}

/**
* @brief get minimum blank number from a+1 in cache.
*
* @param cache cache array.
* @param psize output cache size point.
* @param a find the number not less than a.
*
* @return return the minimum blank number in cache.
*/
int cache_get_min_blank_number(int* cache, int* psize, int a) {
	if (0 == a) { return 1; }
	if (++a < cache[0]) { return a; }           // not subtract any
	int i = 0, j = 0;
	do {
		if (a != cache[i]) { break; }
		a ++;
	} while (++i < (*psize));

	int subtract = i;                           // subtract all or middle part
	if (i != (*psize)) {
		for (j = 0; i < (*psize); i ++, j ++) { cache[j] = cache[i]; }
	}
	(*psize) -= subtract;
	return a;
}

/**
* @brief output cache just for debug.
*
* @param cache cache array.
* @param size cache size.
*/
void output_cache(const int* cache, const int size) {
	int i = 0;
	for (i = 0; i < size; i ++) {
		if (0 != i) { printf(", "); }
		printf("%d", cache[i]);
	}
	printf("\n");
}

/**
* @brief Give nim game of two piles of stone count, to judge whether the first 
* person take stone will definitely win the game.
* It use recurrence to find all unsafety stone count combines.
*
* @param x one pile stone count.
* @param y another pile stone count.
*
* @return return 1 means the first person take the stone will definitely win 
* the game, otherwise he will lose the game if another person is sane in 
* theoretically.
*/
int nim_game_twopilesstone_recurrence(int x, int y) {
	if (!x || !y || x == y) { return 1; }
	if (x > y) { SWAP_INT(x,y) }
	int* cache = (int*) malloc(sizeof(int) * 2);
	int a = 0, b = 0, n = 0, size = 0, capacity = 2;
	while ( (a = cache_get_min_blank_number(cache, &size, a)) && a <= x) {
		b = a + (++n);
		if (b == y && a == x) { free(cache); return 0; }
		cache = cache_add_new_number(cache, &size, &capacity, b);
	}
	free(cache); return 1;
}

/**
* @brief nim game ton find next take strategy intelligence.
* Pretreat to suppose x in the minimum number of x and y.
* Then if (0 == x) means to get all y value turn result is x = 0, y = 0.
* If (y - x == n) means meet the unsafety situation, any strategy is ok.
* If (y - x == n1 < n) means find the value x1 = n1 * a, and y1 = x1 + n1.
* If (y - x > n) means find the new n2 let x1 == n2 * a or x1 == n2 * b.
*   If (x1 == n2 * a) then subtract y to y1 = x1 + n2;
*   If (x2 == n2 * b) then subtract y to y1 = x1 - n2;
* This method is belong to the Betti's theorem as a integer belong to 
* {n*a|n belong to N} or {n*b|n belong to N}.
*
* @param px output x point.
* @param py output y point.
*/
void nim_game_intelligent_next_takestrategy(int* px, int* py) {
	if (!px || !py || *px < 0 || *py < 0) {
		printf("[ERR] -- nim game next takestrategy input invalid!\n");
		return ;
	}
	if ((*px) > (*py)) { SWAP_INT(*px, *py) }
	if (0 == (*px)) { (*py) = 0; return; }
	double a = (1.0 + sqrt(5))/2.0;
	double b = (3.0 + sqrt(5))/2.0;
	int subtract = (*py) - (*px);
	int n = (int) ceil( (double) (*px) / a );
	if (subtract == n) {					// the lose situation, any strategy
		(*py) --;
	} else if (subtract > n) {
		if ((*px) == (int) floor(n * a)) {
			(*py) = (*px) + n;
		} else {
			n = (int) ceil( (double) (*px) / b);
			(*py) = (*px) - n;
		}
	} else {								// subtract < n
		(*px) = (int) floor(subtract * a);
		(*py) = (*px) + subtract;
	}
}

int main(int argc, const char *argv[])
{
	int x = 12, y = 20;
	printf("nim_game_twopilesstone_formula(%d, %d) = %d\n", 
		x, y, nim_game_twopilesstone_formula(x, y));
	printf("nim_game_twopilesstone_recurrence(%d, %d) = %d\n", 
		x, y, nim_game_twopilesstone_recurrence(x, y));

	while (printf("input x and y split by blank: "), 
			scanf("%d %d", &x, &y) == 2) {
		nim_game_intelligent_next_takestrategy(&x, &y);
		printf("next take strategy: x = %d, y = %d\n", x, y);
	}

	return 0;
}
