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
* @file list_intersect.c
* @brief list intersect judge and find the first intersect node.
* @author firstboy0513
* @version 0.0.1
* @date 2013-11-27
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
* @brief list node element.
*/
typedef struct _Node {
	int data;
	struct _Node* next;
} Node;

/**
* @brief Generate new node with data.
*
* @param data input data.
*
* @return new node.
*/
Node* GenerateNewNode(int data) {
	Node* node = (Node*) malloc(sizeof(Node));
	node->data = data;
	node->next = NULL;
	return node;
}

/**
* @brief Judge list whether is circle or not circle.
* If the list has no circle, then output last node point.
* If the list has circle, then output entrance node point.
* 
* We know when fast equals to slow, assum the start point to entrance 
* length is l, and the circle circumference is r, assume now length is s, 
* the have:
* 		 s = l + hr
* 		2s = l + nr + hr
* 	the n is a integer, and h is a float number which is (0 <= h <= 1).
* 	so eliminate the s, we get: 
* 		s = nr
* 		l = nr - hr
* 		l = (n-1)r - (1-h)r*
*
* @param list input list to judge.
* @param plast output last node point if has no circle.
* @param pentra output entrance node point if has circle.
*
* @return return 0 if no circle, otherwise return 1 means has circle, or return
* -1 if error occur.
*/
int JudgeListCircle(Node* list, Node** plast, Node** pentra, int *plen) {
	if (!plast || !pentra || !plen) {
		printf("[ERR] -- JudgeListCircle input invalid null args.\n");
		return -1;
	}
	*plast = NULL, *pentra = NULL, *plen = 0;
	if (!list) {
		return 0;
	}

	Node* fast = list;
	Node* slow = list;
	do {
		if (fast && fast->next) {
			fast = fast->next->next;
		} else {
			break;
		}
		(slow = slow->next), *plen += 1;
	} while(fast != slow);
	if (!fast || !fast->next) {
		// no circle list
		while (slow) {
			(*plast = slow), (slow = slow->next), *plen += 1;
		}
		return 0;
	}
	// has circle, and use "l = (n-1)r + (1-h)r" to find entrance node.
	Node* p = list;
	while (p != fast) {
		p = p->next;
		fast = fast->next;
	}
	*pentra = fast;
	return 1;
}

/**
* @brief Get no circle list intersect.
* Be sure your list has no circle.
*
* @param lista input list A.
* @param last_a input list A last node point.
* @param listb input list B.
* @param last_b input list B last node point.
* @param pfirst output first intersect node point.
*
* @return return 0 if not intersect, otherwise return 1 means intersect, and 
* return -1 if error occur.
*/
int GetNoCircleListIntersect(Node* lista, Node* last_a, int len_a, 
	Node* listb, Node* last_b, int len_b, 
	Node** pfirst) {
	if (!lista || !last_a || !listb || !last_b || !pfirst) {
		printf("[ERR] -- GetNoCircleListIntersect input invalid null args.\n");
		return -1;
	}
	*pfirst = NULL;
	if (last_a != last_b) {
		return 0;
	}
	while (len_a > len_b) {
		lista = lista->next;
		len_a --;
	}
	while (len_b > len_a) {
		listb = listb->next;
		len_b --;
	}
	while (lista && listb && lista != listb) {
		lista = lista->next;
		listb = listb->next;
	}
	*pfirst = lista;
	return 1;
}

/**
* @brief Get circle list intersect.
* Be sure your list has circle.
*
* @param lista input list A.
* @param entra_a input circle list A entrance node point.
* @param listb input list B.
* @param entra_b input circle list B entrance node point.
* @param pfirst output first intersect node point.
*
* @return return 0 if not intersect, otherwise return 1 means intersect and has
* the same entrance, return 2 means although intersect but not has the same 
* entrance.
*/
int GetCircleListIntersect(Node* lista, Node* entra_a, 
	Node* listb, Node* entra_b, 
	Node** pfirst) {
	if (!lista || !entra_a || !entra_a->next 
			|| !listb || !entra_b || !entra_b->next || !pfirst) {
		printf("[ERR] -- GetCircleListIntersect input invalid null args.\n");
		return -1;
	}
	*pfirst = NULL;
	if (entra_a == entra_b) {
		*pfirst = entra_a;
		return 1;
	}
	Node* p = entra_b->next;
	while (p != entra_a && p != entra_b) {
		p = p->next;
	}
	if (p == entra_a) {
		*pfirst = p;
		return 2;
	}
	// if (p == entra_b)
	return 0;
}

/**
* @brief Judbe list whether they are intersect or not.
*
* @param lista input list A.
* @param listb input list B.
* @param pfirst output first intersect node point.
*
* @return return 0 if not intersect, otherwise means intersect, and return 1 
* means two list has first intersect node, and return 2 means although they are
* intersect, but they are all circle and not begin the same entrance node, and
* return -1 if error occur.
*/
int JudgeListIntersect(Node* lista, Node* listb, Node** pfirst) {
	if (!lista || !listb) {
		return 0;
	}
	if (!pfirst) {
		printf("[ERR] -- JudgeListCircle input invalid null args.\n");
		return -1;
	}
	Node* last_a = NULL, * entra_a = NULL;
	Node* last_b = NULL, * entra_b = NULL;
	int len_a = 0, len_b = 0;
	int fcircle_a = JudgeListCircle(lista, &last_a, &entra_a, &len_a);
	int fcircle_b = JudgeListCircle(listb, &last_b, &entra_b, &len_b);
	if (fcircle_a != fcircle_b) {
		// One has circle, and anotheer has no circle, obvious not intersect.
		*pfirst = NULL;
		return 0;
	} else if (!fcircle_a) {
		// List A and list B neither has circle.
		return GetNoCircleListIntersect(lista, last_a, len_a, 
			listb, last_b, len_b, pfirst);
	} else {
		// List A and list B all has circle.
		return GetCircleListIntersect(lista, entra_a, listb, entra_b, pfirst);
	}
}

int SetTestDiffCircleList(
	Node* tnodes[], Node** plista, Node** plistb) {
	if (!plista || !plistb) {
		return -1;
	}
	// lista: 0 -> 1 -> 7; listb: 6 -> 8 -> 9 -> 8;
	*plista = tnodes[0];
	tnodes[0]->next = tnodes[1];
	tnodes[1]->next = tnodes[7];
	*plistb = tnodes[6];
	tnodes[6]->next = tnodes[8];
	tnodes[8]->next = tnodes[9];
	tnodes[9]->next = tnodes[8];
	return 0;
}

int SetTestTwoNoCircleNoIntersectList(
	Node* tnodes[], Node** plista, Node** plistb) {
	if (!plista || !plistb) {
		return -1;
	}
	// lista: 0 -> 1 -> 7; listb: 6 -> 8;
	*plista = tnodes[0];
	tnodes[0]->next = tnodes[1];
	tnodes[1]->next = tnodes[7];
	*plistb = tnodes[6];
	tnodes[6]->next = tnodes[8];
	return 0;
}

int SetTestTwoNoCircleIntersectList(
	Node* tnodes[], Node** plista, Node** plistb) {
	if (!plista || !plistb) {
		return -1;
	}
	// lista: 0 -> 1 -> 8; listb: 6 -> 8;
	*plista = tnodes[0];
	tnodes[0]->next = tnodes[1];
	tnodes[1]->next = tnodes[8];
	*plistb = tnodes[6];
	tnodes[6]->next = tnodes[8];
	return 0;
}

int SetTestTwoCircleNoIntersectList(
	Node* tnodes[], Node** plista, Node** plistb) {
	if (!plista || !plistb) {
		return -1;
	}
	// lista: 0 -> 1 -> 2 -> 3 -> 2; listb: 6 -> 7 -> 8 -> 9 -> 7;
	*plista = tnodes[0];
	tnodes[0]->next = tnodes[1];
	tnodes[1]->next = tnodes[2];
	tnodes[2]->next = tnodes[3];
	tnodes[3]->next = tnodes[2];
	*plistb = tnodes[6];
	tnodes[6]->next = tnodes[7];
	tnodes[7]->next = tnodes[8];
	tnodes[8]->next = tnodes[9];
	tnodes[9]->next = tnodes[7];
	return 0;
}

int SetTestTwoCircleIntersectSameEntranceList(
	Node* tnodes[], Node** plista, Node** plistb) {
	if (!plista || !plistb) {
		return -1;
	}
	// lista: 0 -> 1 -> 7 -> 8 -> 9 -> 7; listb: 6 -> 7 -> 8 -> 9 -> 7;
	*plista = tnodes[0];
	tnodes[0]->next = tnodes[1];
	tnodes[1]->next = tnodes[7];
	tnodes[7]->next = tnodes[8];
	tnodes[8]->next = tnodes[9];
	tnodes[9]->next = tnodes[7];
	*plistb = tnodes[6];
	tnodes[6]->next = tnodes[7];
	return 0;
}

int SetTestTwoCircleIntersectDiffEntranceList(
	Node* tnodes[], Node** plista, Node** plistb) {
	if (!plista || !plistb) {
		return -1;
	}
	// lista: 0 -> 1 -> 8 -> 9 -> 7 -> 8; listb: 6 -> 7 -> 8 -> 9 -> 7;
	*plista = tnodes[0];
	tnodes[0]->next = tnodes[1];
	tnodes[1]->next = tnodes[8];
	tnodes[7]->next = tnodes[8];
	tnodes[8]->next = tnodes[9];
	tnodes[9]->next = tnodes[7];
	*plistb = tnodes[6];
	tnodes[6]->next = tnodes[7];
	return 0;
}

int main(int argc, const char *argv[])
{
	// initialize test nodes.
	int i = 0;
	Node* tnodes[10] = {0};
	for (i = 0; i < 10; i ++) {
		tnodes[i] = GenerateNewNode(1);
		printf("tnodes[%d] = %lu\n", i, (size_t)tnodes[i]);
	}
	// combina two node list to test.
	Node* lista = NULL, *listb = NULL, *first = NULL;
	// SetTestDiffCircleList(tnodes, &lista, &listb);
	// SetTestTwoNoCircleNoIntersectList(tnodes, &lista, &listb);
	// SetTestTwoNoCircleIntersectList(tnodes, &lista, &listb);
	// SetTestTwoCircleNoIntersectList(tnodes, &lista, &listb);
	// SetTestTwoCircleIntersectSameEntranceList(tnodes, &lista, &listb);
	SetTestTwoCircleIntersectDiffEntranceList(tnodes, &lista, &listb);
	
	// judbe list intersect, and output intersect first node.
	int ret = JudgeListIntersect(lista, listb, &first);
	if (ret < 0) {
		printf("[ERR] -- Some error occur in JudgeListIntersect.\n");
	} else if (1 == ret) {
		printf("list a and list b are intersect in first node: %lu\n", 
			(size_t)first);
	} else if (2 == ret) {
		printf("list a and list b are intersect circle list, "
			"but not has the same first node.\n");
	} else {
		printf("list a and list b are not intersect.\n");
	}

	// release node memory.
	for (i = 0; i < 10; i ++) {
		free(tnodes[i]);
	}
	return 0;
}
