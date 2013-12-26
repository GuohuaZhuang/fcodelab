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
* @file ant_climb_pole.c
* @brief caculate all ant leave pole time.
* Some extended problems:
* 1. 从左边数起的第i只蚂蚁什么时候走出木杆？
* 2. 所有蚂蚁从一开始到全部离开木杆共碰撞了多少次？
* 3. 哪只蚂蚁的碰撞次数最多？
* 4. 第k次碰撞发生在哪个时刻？哪个位置？哪两个蚂蚁之间？
*
* 	 result: 视每个蚂蚁背米袋，把每个米袋碰撞也可以说是交换的时候都可能通过米袋
* 	 第蚂蚁主人到最后离开的蚂蚁之间求碰撞点及时间，然后具体哪两只蚂蚁碰撞，则可
* 	 以通过从首次米袋蚂蚁主人到最后离开带走这个米袋的蚂蚁之间做遍历可能知道，比
* 	 如是第一次碰撞得到的，那就是前两只蚂蚁，第二次碰撞的就是第二只和第三只蚂蚁
* 	 相碰撞。所以要保持三个数的element组成一个空间复杂度，每个element表示起始米
* 	 袋，另外再建立一个总碰撞次数的空间复杂度，存储每次碰撞的信息，里面有每次碰
* 	 撞的时间、位置，谁和谁碰撞。
*
* 5. 如果不是一根木杆而是一个铁圈，经过一段时间后所有蚂蚁都会回到的状态吗？
*    这个时间的上界是多少？
*    
*    result: (LCM(N*s)/s) * T, N为蚂蚁数量，T = L/v，L为木杆长，v为speed，T为走
*    一圈的时间，s为走一圈一只蚂蚁移动的位置数。
*
* 6. 如果每只蚂蚁的速度不一样(这就有可能由于追赶而产生碰撞，此时根据动量守恒定
*    律(速度互换)，上述扩展问题的答案是什么呢？
*
*    result: 碰撞次数不变。最后从左leave和从右leave的蚂蚁数量还是一样。
*
* 7. 如果蚂蚁在一个平面上运动，同样也是碰头后原路返回(注意这不等同于两只蚂蚁交
*    换继续前进)，问是否所有蚂蚁都能最终离开平面？
*
*    result: 有可能始终无法离开平面。
*
* reference: http://lam8da.sinaapp.com/?p=11
*
* @author firstboy0513
* @version 0.0.1
* @date 2013-12-25
*/
#include <stdio.h>
#include <stdlib.h>

/**
* @brief get extremum (minimum leave time and maximum heave time) of all ants 
* leave from pole.
*
* @param positions all ants positions in pole.
* @param size ants count.
* @param speed ant move speed.
* @param pole pole length.
* @param pminleave output minimum leave time.
* @param pmaxleave output maximum leave time.
*/
void get_extremum_leavetime(const int* positions, const int size, 
	const int speed, const int pole, int* pminleave, int* pmaxleave) {
	if (!positions || size <= 0 || speed <= 0 || pole <= 0 
			|| !pminleave || !pmaxleave) {
		printf("[ERR] -- get_extremum_leavetime input invalid!\n");
		return;
	}
	int i = 0, end = 0;
	for (i = 0; i < size; i ++) {
		end = pole - positions[i];
		if (positions[i] < end) {
			if (*pminleave < positions[i]) { *pminleave = positions[i]; }
			if (*pmaxleave < end) { *pmaxleave = end; }
		} else {
			if (*pminleave < end) { *pminleave = end; }
			if (*pmaxleave < positions[i]) { *pmaxleave = positions[i]; }
		}
	}
	*pminleave /= speed, *pmaxleave /= speed;
}

typedef enum { LEFT, RIGHT } DIRECTOIN;

/**
* @brief get leave time of ant in the pole.
*
* @param positions ant initial position in the pole.
* @param size ants count.
* @param speed ant move speed.
* @param pole pole length.
* @param initdirections ant initial move directions.
*
* @return return every ant leave time, note you should release its memory 
* outside.
*/
int* get_ant_leavetime(const int* positions, const int size, const int speed, 
	const int pole, const int* initdirections) {
	if (!positions || size <= 0 || speed <= 0 || pole <= 0 || !initdirections) {
		printf("[ERR] -- get_ant_leavetime input invalid!\n");
		return NULL;
	}
	int* leavetimes = (int*) malloc(sizeof(int) * size);
	int i = 0, j = 0;
	for (i = 0, j = i; j < size; j ++) {
		if (initdirections[j] == LEFT) {
			leavetimes[i++] = positions[j] / speed;
		}
	}
	for (i = size-1, j = i; j >= 0; j --) {
		if (initdirections[j] == RIGHT) {
			leavetimes[i--] = pole-positions[j] / speed;
		}
	}
	return leavetimes;
}

/**
* @brief get collision times of ant in the pole.
*
* @param positions ant initial position in the pole.
* @param size ants count.
* @param speed ant move speed.
* @param pole pole length.
* @param initdirections ant initial move directions.
*
* @return return every ant collision times, note you should release its memory 
* outside.
*/
int* get_ant_collisions(const int* positions, const int size, const int speed, 
	const int pole, const int* initdirections) {
	if (!positions || size <= 0 || speed <= 0 || pole <= 0 || !initdirections) {
		printf("[ERR] -- get_ant_leavetime input invalid!\n");
		return NULL;
	}
	int* collisiontimes = (int*) malloc(sizeof(int) * size);
	int i = 0, j = 0, rightcount = 0, leftcount = 0;
	// left leave all ants count
	for (i = 0, j = i; j < size; j ++) {
		if (initdirections[j] == LEFT) { i ++; }
	}
	// left collision times is twice of right count and add 1 right self
	for (j = 0; j < i; j ++) {
		collisiontimes[j] = rightcount * 2;
		if (initdirections[j] == RIGHT) {
			collisiontimes[j] ++; rightcount ++;
		}
	}
	// right leave all ants count
	for (i = size-1, j = i; j >= 0; j --) {
		if (initdirections[j] == RIGHT) { i --; }
	}
	// right collision times is twice of left count and add 1 left self
	for (j = size-1; j > i; j --) {
		collisiontimes[j] = leftcount * 2;
		if (initdirections[j] == LEFT) {
			collisiontimes[j] ++; leftcount ++;
		}
	}
	return collisiontimes;
}

/**
* @brief test case for all ant extremum leave times.
*/
void testcase_all_ant_extremum_leavetime() {
	const int ants_position[] = {3, 7, 11, 17, 23};
	const int ants_count = sizeof(ants_position) / sizeof(ants_position[0]);
	const int pole_length = 27;

	int min_leavetime = 0, max_leavetime = 0;
	get_extremum_leavetime(ants_position, ants_count, 1, pole_length,
		&min_leavetime, &max_leavetime);
	printf("min_leavetime = %d, max_leavetime = %d\n", 
		min_leavetime, max_leavetime);
}

/**
* @brief test case for every ant leave time.
*/
void testcase_every_ant_leavetime() {
	const int ants_position[] = {3, 7, 11, 17, 23};
	const int ants_count = sizeof(ants_position) / sizeof(ants_position[0]);
	const int pole_length = 27;

	int i = 0;
	const int ants_initdirections[] = {RIGHT, RIGHT, RIGHT, LEFT, LEFT};
	int* leavetimes = get_ant_leavetime(ants_position, ants_count, 1, 
		pole_length, ants_initdirections);
	if (leavetimes) {
		printf("every ant leave time is \"index(leave time)\":\n");
		for (i = 0; i < ants_count; i ++) {
			if (0 != i) { printf(", "); }
			printf("%d(%d)", i, leavetimes[i]);
		}
		printf("\n");
	}
	free(leavetimes);
}

/**
* @brief test case for ant collision times.
*/
void testcase_ant_collisiontimes() {
	const int ants_position[] = {3, 7, 11, 17, 23};
	const int ants_count = sizeof(ants_position) / sizeof(ants_position[0]);
	const int pole_length = 27;

	int i = 0;
	int total_collision_count = 0, max_collision_count = 0;
	const int ants_initdirections[] = {RIGHT, RIGHT, RIGHT, LEFT, LEFT};
	int* collisiontimes = get_ant_collisions(ants_position, ants_count, 1, 
		pole_length, ants_initdirections);
	if (collisiontimes) {
		printf("every ant collision times is \"index(collision times)\":\n");
		for (i = 0; i < ants_count; i ++) {
			if (0 != i) { printf(", "); }
			printf("%d(%d)", i, collisiontimes[i]);
			total_collision_count += collisiontimes[i];
			if (max_collision_count < collisiontimes[i]) {
				max_collision_count = collisiontimes[i];
			}
		}
		printf("\n");
	}
	total_collision_count /= 2;
	printf("max_collision_count = %d, total_collision_count = %d\n", 
		max_collision_count, total_collision_count);
	free(collisiontimes);
}

int main(int argc, const char *argv[])
{
	testcase_all_ant_extremum_leavetime(); printf("\n");
	testcase_every_ant_leavetime(); printf("\n");
	testcase_ant_collisiontimes(); printf("\n");

	return 0;
}
