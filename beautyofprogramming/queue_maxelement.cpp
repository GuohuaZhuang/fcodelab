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
* @file queue_maxelement.cpp
* @brief Get Queue max element.
* @author firstboy0513
* @version 0.0.1
* @date 2013-11-27
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <sys/types.h>

typedef int Type;
#define MAXN 100

/**
* @brief Stack.
*/
class Stack
{
public:
	Stack() {
		stackTop = -1;
		maxStackItemIndex = -1;
	}
	void Push(Type x) {
		stackTop ++;
		if (stackTop > MAXN) {
			printf("[ERROR] -- stack is full, can not push.\n");
			return;
		}
		stackItem[stackTop] = x;
		if (x > Max()) {
			link2NextMaxItem[stackTop] = maxStackItemIndex;
			maxStackItemIndex = stackTop;
		} else {
			link2NextMaxItem[stackTop] = -1;
		}
	}
	Type Pop() {
		Type ret;
		if (stackTop < 0) {
			printf("[ERROR] -- STACK is empty, can not pop.\n");
			return (Type)-INFINITY;
		}
		ret = stackItem[stackTop];
		if (stackTop == maxStackItemIndex) {
			maxStackItemIndex = link2NextMaxItem[stackTop];
		}
		stackTop --;
		return ret;
	}
	Type Max() {
		return (maxStackItemIndex >= 0) 
			? stackItem[maxStackItemIndex] : -INFINITY;
	}
	bool empty() {
		return (stackTop < 0) ? true : false;
	}
private:
	Type stackItem[MAXN];
	int stackTop;
	int link2NextMaxItem[MAXN];
	int maxStackItemIndex;
};

/**
* @brief Queue.
*/
class Queue
{
public:
	Type MaxValue(Type x, Type y) { return ((x > y) ? x : y); }
	Type Max() { return MaxValue(stackA.Max(), stackB.Max()); }
	void EnQueue(Type v) {
		stackB.Push(v);
	}
	Type DeQueue() {
		if (stackA.empty()) {
			while (!stackB.empty()) {
				stackA.Push(stackB.Pop());
			}
		}
		return stackA.Pop();
	}
private:
	Stack stackA;
	Stack stackB;
};

int main(int argc, const char *argv[])
{
	Stack sa;
	sa.Push(1); printf("sa max = %d\n", sa.Max());
	sa.Push(3); printf("sa max = %d\n", sa.Max());
	sa.Push(2); printf("sa max = %d\n", sa.Max());
	sa.Push(4); printf("sa max = %d\n", sa.Max());
	sa.Pop(); printf("sa max = %d\n", sa.Max());
	sa.Pop(); printf("sa max = %d\n", sa.Max());
	sa.Pop(); printf("sa max = %d\n", sa.Max());
	sa.Pop(); printf("sa max = %d\n", sa.Max());
	Queue qa;
	qa.EnQueue(1); printf("qa max = %d\n", qa.Max());
	qa.EnQueue(3); printf("qa max = %d\n", qa.Max());
	qa.EnQueue(2); printf("qa max = %d\n", qa.Max());
	qa.EnQueue(4); printf("qa max = %d\n", qa.Max());
	qa.DeQueue();  printf("qa max = %d\n", qa.Max());
	qa.DeQueue();  printf("qa max = %d\n", qa.Max());
	qa.DeQueue();  printf("qa max = %d\n", qa.Max());
	qa.DeQueue();  printf("qa max = %d\n", qa.Max());

	return 0;
}
