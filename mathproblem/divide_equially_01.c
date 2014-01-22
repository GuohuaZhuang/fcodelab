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
* @file divide_equially_01.c
* @brief use recursion stupid method to solve the divide equially 0 and 1 
* character problem, let it very slow when the string length is extension.
* @author firstboy0513
* @version 0.0.1
* @date 2014-01-22
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
* @brief suture struct.
*/
typedef struct _SUTURE {
    int issuture;         // 0 means not, 1 means yes.
    int count0;			  // 0 character count.
    int count1;			  // 1 character count.
	int varify;			  // use to tip the varified combine flag.
} SUTURE;

/**
* @brief varify split is valid or not to given two heap the same count of 0 
* character and 1 character.
*
* @param suture_place suture struct place array.
* @param length length of characters.
* @param sum0 0 character sum.
* @param sum1 1 character sum.
*
* @return return 1 means valid, otherwise return 0 means invalid.
*/
int varify_split(SUTURE* suture_place, const int length, 
	int sum0, int sum1) {
	if (0 == sum0 && 0 == sum1) { return 1; }
    int i = 0;
    for (i = 0; i < length; i ++) {
		if (suture_place[i].varify == 1) { continue; }
        if (suture_place[i].issuture == 1) { continue; }
		if (suture_place[i].count0 > sum0 || suture_place[i].count1 > sum1) {
			continue;
		}
		suture_place[i].varify = 1;
		if (1 == varify_split(suture_place, length, 
			sum0 - suture_place[i].count0,
			sum1 - suture_place[i].count1)) {
			return 1;
		}
		suture_place[i].varify = 0;
    }
	return 0;
}

/**
* @brief record minimum cutting times in the recursion calculations
*
* @param k current cutting times.
* @param pmincut output minimum cutting times point.
*/
void record_mincuttimes(const int k, int* pmincut) {
    if ((*pmincut) > k) { (*pmincut) = k; }
}

/**
* @brief use recursion method to calculate the minimum cutting times.
*
* @param suture_place suture place array.
* @param length characters total count.
* @param k current cutting times.
* @param pmincut output minimum cutting times point.
*/
void recurse(SUTURE* suture_place, int length, int k, int* pmincut) {
    int i = 0, prei = 0, sum0 = 0, sum1 = 0;
    for (i = 1; i < length; i ++) {
        if (suture_place[i].issuture == 1) { continue; }
		// copy suture place
        SUTURE* suture_place_copy = (SUTURE*) malloc((length)*sizeof(SUTURE));
        memcpy(suture_place_copy, suture_place, (length)*sizeof(SUTURE));
		// suture preindex and i
		suture_place_copy[i].issuture = 1;
		suture_place_copy[prei].varify = 1;
		sum0 = (suture_place_copy[prei].count0 += suture_place_copy[i].count0);
		sum1 = (suture_place_copy[prei].count1 += suture_place_copy[i].count1);
        if (1 == varify_split(suture_place_copy, length, sum0, sum1)) {
			suture_place_copy[prei].varify = 0;
            recurse(suture_place_copy, length, k-1, pmincut);
        } else {
            record_mincuttimes(k, pmincut);
        }
		prei = i;
        free(suture_place_copy);
    }
}

/**
* @brief calculate minimum cutting times to divide two heap with equially 0 and 
* 1 character count.
*
* @param s string which just include 0 and 1.
* @param size string size.
*
* @return return the minimum cutting times count.
*/
int calculate_min_cuttimes(const char* s, const int size) {
	if (!s || size <= 0) {
		printf("[ERR] -- input s is invalid!\n");
		return -1;
	}
    SUTURE* suture_place = (SUTURE*) malloc((size)*sizeof(SUTURE));
    memset(suture_place, 0, (size)*sizeof(SUTURE));
	int i = 0;
	for (i = 0; i < size; i ++) {
		if ('0' == s[i]) {
			suture_place[i].count0 = 1;
		} else if ('1' == s[i]) {
			suture_place[i].count1 = 1;
		} else {
			printf("[ERR] -- input s is invalid!\n");
			free(suture_place);
			return -1;
		}
	}
    int mincut = size-1;
    recurse(suture_place, size, size-1, &mincut);
    free(suture_place);
    return mincut;
}

int main(int argc, const char *argv[])
{
	const char* s = "010111";
	const int size = strlen(s);
	int min_cuttimes = calculate_min_cuttimes(s, size);
	printf("when s = [%s], min_cuttimes = %d\n", s, min_cuttimes);

	return 0;
}
