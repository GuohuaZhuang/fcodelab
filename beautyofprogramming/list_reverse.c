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
* @file list_reverse.c
* @brief list node reverse and random node delete.
* @author firstboy0513
* @version 0.0.1
* @date 2013-11-29
*/
#include <stdio.h>
#include <stdlib.h>

#ifndef NULL
#define NULL 0
#endif

/**
* @brief list node struct.
*/
typedef struct _Node {
	int data;
	struct _Node* next;
} Node;

/**
* @brief Malloc a new node.
*
* @param data new node element data.
*
* @return new node point.
*/
Node* NewNode(int data) {
	Node* p = (Node*)malloc(sizeof(Node));
	p->data = data;
	p->next = NULL;
	return p;
}

/**
* @brief Release list memory.
*
* @param head list head point.
*
* @return return 0 if release ok, otherwise return -1 if error occur.
*/
int ReleaseList(Node* head) {
	Node* p = head;
	while (p) {
		head = p->next;
		free(p);
		p = head;
	}
	return 0;
}

/**
* @brief Head insert node, begin in the head of list.
*
* @param head list head point.
* @param data insert new node data.
*
* @return inserted list head point.
*/
Node* InsertNode(Node* head, int data) {
	if (head == NULL) {
		head = NewNode(data);
	} else {
		Node* p = NewNode(data);
		p->next = head;
		head = p;
	}
	return head;
}

/**
* @brief Show list data in a line.
*
* @param head list head point.
*/
void ShowList(Node* head) {
	Node* p = head;
	while (p) {
		printf("%d --> ", p->data);
		p = p->next;
	}
	printf("NULL\n");
}

/**
* @brief Find node by data.
*
* @param head list head point.
* @param data input data to finded.
*
* @return return node point if finded, otherwise return NULL.
*/
Node* FindNode(Node* head, int data) {
	Node* p = head;
	while (p) {
		if (p->data == data) {
			return p;
		}
		p = p->next;
	}
	return NULL;
}

/**
* @brief Delete node by random point.
*
* @param pCurrent the node point to be delete.
*
* @return return 0 if delete ok, otherwise return -1 if error occur.
*/
int DeleteRandomNode(Node* pCurrent) {
	if (NULL == pCurrent) {
		printf("[ERR] -- Can not delete in DeleteRandomNode.\n");
		return -1;
	}
	Node* pNext = pCurrent->next;
	if (NULL == pNext) {
		printf("[ERR] -- Can not delete in DeleteRandomNode.\n");
		return -1;
	}
	pCurrent->next = pNext->next;
	pCurrent->data = pNext->data;
	free(pNext);
	return 0;
}

/**
* @brief Reverse list with one traversal.
*
* @param head list head point.
*
* @return return 0 if reverse ok, otherwise return -1 if error occur.
*/
Node* ReverseList(Node* head) {
	Node* pfront = NULL, *pnext = NULL;
	Node* p = head;
	while (p) {
		pnext = p->next;
		p->next = pfront;
		pfront = p;
		p = pnext;
	}
	return pfront;
}

/**
* @brief Generate test list.
*
* @return test list head point.
*/
Node* GenerateTestList() {
	Node* head = NULL;
	head = InsertNode(head, 8);
	head = InsertNode(head, 7);
	head = InsertNode(head, 6);
	head = InsertNode(head, 5);
	head = InsertNode(head, 4);
	head = InsertNode(head, 3);
	head = InsertNode(head, 2);
	head = InsertNode(head, 1);
	return head;
}

int main(int argc, const char *argv[])
{
	Node* head = GenerateTestList();
	printf("Generate test list: \n\t");
	ShowList(head);

	DeleteRandomNode(FindNode(head, 3));
	printf("After delete random 3 node: \n\t");
	ShowList(head);

	head = ReverseList(head);
	printf("After reverse list: \n\t");
	ShowList(head);

	ReleaseList(head);
	return 0;
}
