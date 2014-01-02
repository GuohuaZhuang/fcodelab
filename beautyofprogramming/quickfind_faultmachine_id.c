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
* @file quickfind_faultmachine_id.c
* @brief Quick find the fault machine id.
* @author firstboy0513
* @version 0.0.1
* @date 2014-01-02
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/**
* @brief find one fault machine id with 2 backup.
* Be sure the idlist array must has just one fault machine!
*
* @param idlist all id list.
* @param size id list size.
*
* @return return fault machine id.
*/
int find_2backup_one_faultmachine_id(const int* idlist, const int size) {
	int i = 0, id = 0;
	for (i = 0; i < size; i ++) {
		id ^= idlist[i];
	}
	return id;
}

/**
* @brief get sum of idlist ids value.
*
* @param idlist all id list.
* @param size id list size.
*
* @return sum of idlist.
*/
int idlist_sum(const int* idlist, const int size) {
	int i = 0, sum = 0;
	for (i = 0; i < size; i ++) {
		sum += idlist[i];
	}
	return sum;
}

/**
* @brief test case for find one id use xor method with 2 backup.
*
* @param idlist all id list.
* @param size id list size.
*/
void testcase_find_2backup_one_id_use_xor(const int* idlist, const int size) {
	int id = find_2backup_one_faultmachine_id(idlist, size);
	printf("one fault machine (method xor) id = %d\n", id);
}

/**
* @brief test case for find one id use sum method with 2 backup.
*
* @param idlist all id list.
* @param size id list size.
* @param sum all id list sum.
*/
void testcase_find_2backup_one_id_use_sum(const int* idlist, const int size, 
	int sum) {
	sum *= 2;
	int i = 0;
	for (i = 0; i < size; i ++) {
		sum -= idlist[i];
	}
	printf("one fault machine (method sum) id = %d\n", sum);
}

/**
* @brief find 2 backup one fault machine id and filter two value by bit.
*
* @param idlist id list.
* @param size id list size.
* @param bit bit value, just has only one 1bit.
* @param pa output a point.
* @param pb output b point.
*/
void find_2backup_one_faultmachine_id_filterbybit(
	const int* idlist, const int size, int bit, int* pa, int* pb) {
	if (!idlist || size <= 0 || !pa || !pb) {
		return;
	}
	int i = 0;
	for (i = 0; i < size; i ++) {
		if (bit & idlist[i]) {
			(*pa) ^= idlist[i];
		} else {
			(*pb) ^= idlist[i];
		}
	}
}

/**
* @brief test case for find 2 backup tow id use xor.
*
* @param idlist id list.
* @param size id list size.
* @param sum origin id list sum.
*/
void testcase_find_2backup_two_id_use_xor(const int* idlist, const int size, 
	int sum) {
	int ids = find_2backup_one_faultmachine_id(idlist, size);
	if (0 == ids) {                          // 2 id is the same id
		int listsum = idlist_sum(idlist, size);
		printf("two lost id is the same id = %d\n", (int)(((sum*2)-listsum)/2));
	} else {                                 // 2 id is not the same id
		int bit = (ids ^ (ids & (ids-1)));   // get first low 1 bit
		int a = 0, b = 0;
		find_2backup_one_faultmachine_id_filterbybit(idlist, size, bit, &a, &b);
		printf("two lost id are id1 = %d, id2 = %d\n", a, b);
	}
}

int main(int argc, const char *argv[])
{
	const int origin_idlist[] = {
		1, 2, 5, 23, 34, 92
	};
	const int origin_size = sizeof(origin_idlist) / sizeof(origin_idlist[0]);
	const int idlist[] = {
		5, 23, 2, 5, 1, 34, 92, 23, 92, 1, 2
	};
	const int size = sizeof(idlist) / sizeof(idlist[0]);
	const int sum = idlist_sum(origin_idlist, origin_size);

	testcase_find_2backup_one_id_use_xor(idlist, size);
	testcase_find_2backup_one_id_use_sum(idlist, size, sum);

	const int idlist_lost2id[] = {
		5, 23, 2, 5, 1, 92, 92, 1, 23, 2
	};
	const int lost2id_size = sizeof(idlist_lost2id) / sizeof(idlist_lost2id[0]);
	testcase_find_2backup_two_id_use_xor(idlist_lost2id, lost2id_size, sum);

	return 0;
}
