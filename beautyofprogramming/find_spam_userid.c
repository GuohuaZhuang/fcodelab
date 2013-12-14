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
* @file find_spam_userid.c
* @brief find spam use id, like more than half count posts or quarter count.
* @author firstboy0513
* @version 0.0.1
* @date 2013-12-14
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/**
* @brief find more than half count id from given id list.
*
* @param ids id list, actually it is an array.
* @param len ids length.
*
* @return more than half count id, otherwise return INT32_MIN means error occur.
*/
int find_morethan_half_id(const int* ids, const int len) {
	if (!ids || len <= 0) {
		printf("[ERR] -- find_morethan_halfcount_id input invalid!\n");
		return INT32_MIN;
	}
	int i = 0, candidate = INT32_MIN, candidate_count = 0;
	for (i = 0; i < len; i ++) {
		if (0 == candidate_count) {
			candidate = ids[i]; candidate_count = 1;
		} else if (candidate != ids[i]) {
			candidate_count --;
		} else { // 0 != candidate_count && candidate == ids[i]
			candidate_count ++;
		}
	}
	return candidate;
}

/**
* @brief find more than half count id from given id list.
*
* @param ids id list, actually it is an array.
* @param len ids length.
*
* @return just return 0 means ok, otherwise return INT32_MIN means error occur.
*/
int find_three_morethan_quarter_id(const int* ids, const int len, 
	int* pid1, int* pid2, int* pid3) {
	if (!ids || len <= 0 || !pid1 || !pid2 || !pid3) {
		printf("[ERR] -- find_three_morethan_quarter_id input invalid!\n");
		return INT32_MIN;
	}
	int i = 0, j = 0, candidate[3] = {INT32_MIN}, candidate_count[3] = {0};
	for (i = 0; i < len; i ++) {
		for (j = 0; j < 3; j ++) {
			if (candidate_count[j] > 0 && ids[i] == candidate[j]) {
				candidate_count[j] ++; break;
			}
		}
		if (j < 3) continue;
		for (j = 0; j < 3; j ++) {
			if (0 == candidate_count[j]) {
				candidate[j] = ids[i]; candidate_count[j] = 1; break;
			}
		}
		if (j >= 3) { for (j = 0; j < 3; j ++) { candidate_count[j] --; } }
	}
	*pid1 = candidate[0]; *pid2 = candidate[1]; *pid3 = candidate[2];
	return 0;
}

int main(int argc, const char *argv[])
{
	// find more than half count user id test case.
	const int ids[] = {2, 2, 2, 1, 1};
	const int len = sizeof(ids) / sizeof(ids[0]);
	int mf_id = find_morethan_half_id(ids, len);
	printf("more than half count id = %d\n", mf_id);

	// find three more than quarter count user id test case.
	const int ids_quarter[] = {4, 2, 5, 2, 6, 2, 6, 8, 4, 4, 6};
	const int len_quarter = sizeof(ids_quarter) / sizeof(ids_quarter[0]);
	int mq_id1 = 0, mq_id2 = 0, mq_id3 = 0;
	find_three_morethan_quarter_id(ids_quarter, len_quarter, 
		&mq_id1, &mq_id2, &mq_id3);
	printf("more than quarter count id = (%d, %d, %d)\n", 
		mq_id1, mq_id2, mq_id3);

	return 0;
}
