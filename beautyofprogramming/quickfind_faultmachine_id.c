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
* @brief find one fault machine id.
* Be sure the idlist array must has just one fault machine!
*
* @param idlist all id list.
* @param size id list size.
*
* @return return fault machine id.
*/
int find_one_faultmachine_id(const int* idlist, const int size) {
	int i = 0, id = 0;
	for (i = 0; i < size; i ++) {
		id ^= idlist[i];
	}
	return id;
}

int main(int argc, const char *argv[])
{
	const int idlist[] = {
		5, 23, 2, 5, 1, 34, 92, 23, 92, 1, 2
	};
	const int size = sizeof(idlist) / sizeof(idlist[0]);
	int id = find_one_faultmachine_id(idlist, size);
	printf("one fault machine id = %d\n", id);
	return 0;
}
