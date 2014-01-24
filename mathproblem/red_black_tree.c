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
* @file red_black_tree.c
* @brief Red-Black tree implement in C.
* @author firstboy0513
* @version 0.0.1
* @date 2014-01-23
*/
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Red-Black tree public method extern to use or private be invoke.
*/
#define PUBLIC
#define PRIVATE static

/**
* @brief Element compare EQ as equal, LT as little than, GT as greate than.
*/
#define EQ(a, b) (a == b)
#define LT(a, b) (a < b)
#define GT(a, b) (a > b)

/**
* @brief Element copy macro.
*
* @param des destination element.
* @param src source element.
*/
#define ELEMENT_COPY(des, src) { des = src; }

/**
* @brief Element type.
*/
typedef int ELEMENT;

/**
* @brief the node of Red-Black tree color enum.
* Note the BLACK value must be 0, because the NIL_NODE initialize with all 0.
*/
typedef enum { BLACK = 0, RED = 1 } COLOR;

/**
* @brief Red-Black tree node.
*/
typedef struct _NODE {
	COLOR color;
	ELEMENT data;
	struct _NODE* parent;
	struct _NODE* lchild;
	struct _NODE* rchild;
} NODE, *TREE;

/**
* @brief NIL or NULL node, not rather than use the null to denote leaf node.
*/
NODE NIL_NODE = {0};

/**
* @brief public methods declaration.
*/
PUBLIC TREE	rbt_init(void);
PUBLIC void	rbt_destory(TREE T);
PUBLIC int	rbt_search(TREE T, ELEMENT d);
PUBLIC int	rbt_insert(TREE* pT, ELEMENT d);
PUBLIC int	rbt_delete(TREE* pT, ELEMENT d);
PUBLIC void	rbt_traveral(TREE T, void function(NODE*));

/**
* @brief Red-Black initialize method.
*
* @return just return NULL when initialize it as a Red-Black tree.
*/
PUBLIC TREE rbt_init(void) {
	return NULL;
}

/**
* @brief Red-Black tree destory method.
*
* @param T Red-Black tree.
*/
PUBLIC void rbt_destory(TREE T) {
	if (!T || (&NIL_NODE) == T) { return; }
	if ((&NIL_NODE) != T->lchild) { rbt_destory(T->lchild); }
	if ((&NIL_NODE) != T->rchild) { rbt_destory(T->rchild); }
	free(T);
}

/**
* @brief Red-Black search method.
*
* @param T Red-Black tree.
* @param d element to search.
*
* @return return 1 means search the element success, otherwise return 0 means 
* not find it in the Red-Black tree.
*/
PUBLIC int rbt_search(TREE T, ELEMENT d) {
	while (T && ((&NIL_NODE) != T)) {
		if (EQ(d, T->data)) {
			return 1;
		} else if (LT(d, T->data)) {
			T = T->lchild;
		} else {
			T = T->rchild;
		}
	}
	return 0;
}

/**
* @brief Red-Black tree internal method to search the place to insert new node.
*
* @param T Red-Black tree.
* @param d new element to search.
* @param P parent of the tree.
* @param pret output the node point if found the element is already in the tree,
* otherwise output the parent node point.
*/
PRIVATE int _rbt_search(TREE T, ELEMENT d, NODE* P, NODE** pret) {
	if (!T || ((&NIL_NODE) == T)) { (*pret) = P; return 0; }
	if (EQ(d, T->data)) { (*pret) = T; return 1; }
	else if (LT(d, T->data)) { return _rbt_search(T->lchild, d, T, pret); }
	else { return _rbt_search(T->rchild, d, T, pret); }
}

/**
* @brief Red-Black tree internal method to insert new node.
*
* @param pT Red-Black tree point, may be to change tree root.
* @param d element to insert.
*
* @return return the new element inserted in the tree, and also setted color to 
* red, otherwise if the element is already in the tree, it will return NULL.
*/
PRIVATE NODE* _rbt_insert(TREE* pT, ELEMENT d) {
	NODE* P = NULL;
	if (_rbt_search((*pT), d, NULL, &P)) {
		return NULL;
	}
	NODE* s = (NODE*) malloc(sizeof(NODE));
	ELEMENT_COPY(s->data, d);
	s->color = RED; s->lchild = s->rchild = &NIL_NODE; s->parent = P;
	if (!P) { (*pT) = s; }
	else if (LT(P->data, d)) {
		P->lchild = s;
	} else {
		P->rchild = s;
	}
	return s;
}

/**
* @brief Red-Black tree get node's grandparent node, it is a internal method.
*
* @param n node.
*
* @return grandparent node.
*/
PRIVATE NODE* _rbt_grandparent(NODE* n) {
	if (!n || !(n->parent)) {
		return NULL;
	}
	return n->parent->parent;
}

/**
* @brief Red-Black tree get node's grandparent node quote, it is a internal 
* method. If the grandparent is the root of Red-Black tree, then it return the 
* tree root quote.
*
* @param n node.
* @param pT Red-Black tree root quote.
*
* @return grandparent node quote, if the G is the root of Red-Black tree, then
* it return the tree root quote.
*/
PRIVATE TREE* _rbt_grandparent_quote(NODE* n, TREE* pT) {
	if (!n || !(n->parent)) {
		return NULL;
	}
	NODE* G = n->parent->parent;
	NODE* GP = G->parent;
	if (NULL == GP) { return pT; }
	if (GP->lchild == G) {
		return &(GP->lchild);
	} else {
		return &(GP->rchild);
	}
}

/**
* @brief Red-Black tree get node's uncle node, it is a internal method.
*
* @param n node.
*
* @return uncle node.
*/
PRIVATE NODE* _rbt_uncle(NODE* n) {
	NODE* G = _rbt_grandparent(n);
	if (n->parent == G->lchild) {
		return G->rchild;
	} else {
		return G->lchild;
	}
}

/**
* @brief Red-Black tree left rotate internal method.
*
* @param pT one tree to rotate.
*/
PRIVATE void _rbt_rotate_left(TREE* pT) {
	NODE* rchild = (*pT)->rchild;
	(*pT)->rchild = rchild->lchild; rchild->lchild->parent = (*pT);
	rchild->lchild = (*pT); (*pT)->parent = rchild;
	(*pT) = rchild;
}

/**
* @brief Red-Black tree right rotate internal method.
*
* @param pT one tree to rotate.
*/
PRIVATE void _rbt_rotate_right(TREE* pT) {
	NODE* lchild = (*pT)->lchild;
	(*pT)->lchild = lchild->rchild; lchild->rchild->parent = (*pT);
	lchild->rchild = (*pT); (*pT)->parent = lchild;
	(*pT) = lchild;
}

/**
* @brief internal insert case from 1 to 5 method declaration.
*/
PRIVATE void _rbt_insert_case_1(NODE* N, TREE* pT);
PRIVATE void _rbt_insert_case_2(NODE* N, TREE* pT);
PRIVATE void _rbt_insert_case_3(NODE* N, TREE* pT);
PRIVATE void _rbt_insert_case_4(NODE* N, TREE* pG);
PRIVATE void _rbt_insert_case_5(NODE* N, TREE* pG);

/**
* @brief Red-Black tree insert case 5 when in following case 5.
* 	G->color == BLACK
* 	P->color == RED
* 	U->color == BLACK
* 	N->color == RED
* where G denote N's grandparent, P denote N's parent, U denote N's uncle, N is 
* the new inserted red node. And the N, P and G is in the same directional line.
*
* @param N the new ineserted red node.
* @param pG N's grandparent node quote.
*/
PRIVATE void _rbt_insert_case_5(NODE* N, TREE* pG) {
	N->parent->color = BLACK;
	(*pG)->color = RED;
	if (N == N->parent->lchild) {
		_rbt_rotate_right(pG);
	} else {
		_rbt_rotate_left(pG);
	}
}

/**
* @brief Red-Black tree insert case 5 when in following case 4.
* 	G->color == BLACK
* 	P->color == RED
* 	U->color == BLACK
* 	N->color == RED
* where G denote N's grandparent, P denote N's parent, U denote N's uncle, N is 
* the new inserted red node. But the N, P and G is not in the same directional
* line, such as the N is the P's right child, while P is the G's left child.
* (Or N is the P's left child, while P is the G's right child.)
*
* @param N the new ineserted red node.
* @param pG N's grandparent node quote.
*/
PRIVATE void _rbt_insert_case_4(NODE* N, TREE* pG) {
	if ((*pG)->lchild == N->parent && N->parent->rchild == N) {
		_rbt_rotate_left(&((*pG)->lchild));
		N = N->lchild;
	} else if ((*pG)->rchild == N->parent && N->parent->lchild == N) {
		_rbt_rotate_right(&((*pG)->rchild));
		N = N->rchild;
	}
	_rbt_insert_case_5(N, pG);
}

/**
* @brief Red-Black tree insert case 5 when in following case 3.
* 	G->color == BLACK
* 	P->color == RED
* 	U->color == RED
* 	N->color == RED
* where G denote N's grandparent, P denote N's parent, U denote N's uncle, N is 
* the new inserted red node.
*
* @param N the new ineserted red node.
* @param pG N's grandparent node quote.
*/
PRIVATE void _rbt_insert_case_3(NODE* N, TREE* pT) {
	NODE* U = _rbt_uncle(N);
	TREE* pG = _rbt_grandparent_quote(N, pT);
	if (RED == U->color) {
		(*pG)->color = RED;
		N->parent->color = U->color = BLACK;
		_rbt_insert_case_1((*pG), pT);
	} else {
		_rbt_insert_case_4(N, pG);
	}
}

/**
* @brief Red-Black tree insert case 5 when in following case 2.
* 	P->color == BLACK
* 	N->color == RED
* where P denote N's parent, N is the new inserted red node.
*
* @param N the new ineserted red node.
* @param pG N's grandparent node quote.
*/
PRIVATE void _rbt_insert_case_2(NODE* N, TREE* pT) {
	if (BLACK == N->parent->color) {
		return;
	} else {
		_rbt_insert_case_3(N, pT);
	}
}

/**
* @brief Red-Black tree insert case 5 when in following case 1.
* 	P == NULL
* 	N->color == RED
* where P denote N's parent, N is the new inserted red node.
*
* @param N the new ineserted red node.
* @param pG N's grandparent node quote.
*/
PRIVATE void _rbt_insert_case_1(NODE* N, TREE* pT) {
	if (NULL == N->parent) {
		N->color = BLACK;
	} else {
		_rbt_insert_case_2(N, pT);
	}
}

/**
* @brief Red-Black tree insert method.
*
* @param pT Red-Black tree quote point.
* @param d element to insert.
*
* @return return 1 means insert success, otherwise return 0 means the element is
* already in the tree and insert failed.
*/
PUBLIC int rbt_insert(TREE* pT, ELEMENT d) {
	if (!pT) {
		printf("[ERR] -- rbt_insert input invalid!\n");
		return 0;
	}
	NODE* N = _rbt_insert(pT, d);
	if (!N) { return 0; }

	_rbt_insert_case_1(N, pT);

	return 1;
}

PRIVATE NODE* _rbt_sibling(NODE* N) {
	if (N == N->parent->lchild) {
		return N->parent->rchild;
	} else {
		return N->parent->lchild;
	}
}

/**
* @brief Red-Black tree get parent quote point.
* Be sure the N is not null and N->parent is not null.
*
* @param N node.
* @param pT Red-Black tree.
*
* @return parent quote point.
*/
PRIVATE TREE* _rbt_parent_quote(NODE* N, TREE* pT) {
	if (N->parent->parent == NULL) {
		return pT;
	}
	NODE* G = N->parent->parent;
	if (N->parent == G->lchild) {
		return &(G->lchild);
	} else {
		return &(G->rchild);
	}
}

/**
* @brief internal delete case from 1 to 6 method declaration.
*/
PRIVATE void _rbt_delete_case_6(NODE* N, TREE* pT);
PRIVATE void _rbt_delete_case_5(NODE* N, TREE* pT);
PRIVATE void _rbt_delete_case_4(NODE* N, TREE* pT);
PRIVATE void _rbt_delete_case_3(NODE* N, TREE* pT);
PRIVATE void _rbt_delete_case_2(NODE* N, TREE* pT);
PRIVATE void _rbt_delete_case_1(NODE* N, TREE* pT);

PRIVATE void _rbt_delete_case_6(NODE* N, TREE* pT) {
	NODE* S = _rbt_sibling(N);
	if (&NIL_NODE == S) { return; }					// ADDDDDDDDDDDDDDD
	TREE* pP = _rbt_parent_quote(N, pT);
	S->color = N->parent->color;
	N->parent->color = BLACK;
	if (N == N->parent->lchild) {
		S->rchild->color = BLACK;
		_rbt_rotate_left(pP);
	} else {
		S->lchild->color = BLACK;
		_rbt_rotate_right(pP);
	}
}

PRIVATE void _rbt_delete_case_5(NODE* N, TREE* pT) {
	NODE* S = _rbt_sibling(N);
	if (BLACK == S->color) {
		if (N == N->parent->lchild
			&& RED == S->lchild->color
			&& BLACK == S->rchild->color) {
			S->color = RED; S->lchild->color = BLACK;
			_rbt_rotate_right(&(N->parent->rchild));
		} else if (N == N->parent->rchild
			&& BLACK == S->lchild->color
			&& RED == S->rchild->color) {
			S->color = RED; S->rchild->color = BLACK;
			_rbt_rotate_left(&(N->parent->lchild));
		}
	}
	_rbt_delete_case_6(N, pT);
}

PRIVATE void _rbt_delete_case_4(NODE* N, TREE* pT) {
	NODE* S = _rbt_sibling(N);
	if (RED == N->parent->color
		&& (&NIL_NODE != S)							// ADDDDDDDDDDDDDDD
		&& BLACK == S->color
		&& BLACK == S->lchild->color
		&& BLACK == S->rchild->color) {
		S->color = RED;
		N->parent->color = BLACK;
	} else {
		_rbt_delete_case_5(N, pT);
	}
}

PRIVATE void _rbt_delete_case_3(NODE* N, TREE* pT) {
	NODE* S = _rbt_sibling(N);
	if (BLACK == N->parent->color 
		&& (&NIL_NODE != S)							// ADDDDDDDDDDDDDDD
		&& BLACK == S->color 
		&& BLACK == S->lchild->color
		&& BLACK == S->rchild->color) {
		S->color = RED;
		_rbt_delete_case_1(N->parent, pT);
	} else {
		_rbt_delete_case_4(N->parent, pT);
	}
}

PRIVATE void _rbt_delete_case_2(NODE* N, TREE* pT) {
	NODE* S = _rbt_sibling(N);
	TREE* pP = _rbt_parent_quote(N, pT);
	if (RED == S->color) {
		N->parent->color = RED;
		S->color = BLACK;
		if (N == N->parent->lchild) {
			_rbt_rotate_left(pP);
		} else {
			_rbt_rotate_right(pP);
		}
	}
	_rbt_delete_case_3(N, pT);
}

PRIVATE void _rbt_delete_case_1(NODE* N, TREE* pT) {
	// N must be a leaf node!
	if (NULL != N->parent) {
		_rbt_delete_case_2(N, pT);
	} else {
		N->parent = NULL;
		(*pT) = NULL;
	}
}

/// TODO
PRIVATE void _rbt_real_delete_node(TREE* pT) {
	NODE* C = (*pT)->lchild;
	if ((&NIL_NODE) == (*pT)->lchild) {
		C = (*pT)->rchild;
	}
	NODE* P = (*pT);
	(*pT) = C;
	if (BLACK == P->color) {
		C->parent = (*pT)->parent;
		if (RED == C->color) {
			C->color = BLACK;
		} else {
			_rbt_delete_case_1(C, pT);
		}
	} else {
		// C must be a leaf node
	}
	free(P);
}

/// TODO
PRIVATE void _rbt_delete_node(TREE* pT) {
	if ((&NIL_NODE) == (*pT)->lchild) {
		_rbt_real_delete_node(pT);
	} else if ((&NIL_NODE) == (*pT)->rchild) {
		_rbt_real_delete_node(pT);
	} else {
		// find the maximum element node in left subtree (in-order predecessor)
		TREE* pS = &((*pT)->lchild);
		while ((&NIL_NODE) != ((*pS)->rchild)) {
			pS = &((*pS)->rchild);
		}
		ELEMENT_COPY((*pT)->data, (*pS)->data);
		_rbt_real_delete_node(pS);
	}
}

/// TODO
PUBLIC int rbt_delete(TREE* pT, ELEMENT d) {
	if (!pT) {
		printf("[ERR] -- rbt_delete input invalid!\n");
		return 0;
	}
	if (!(*pT) || (&(NIL_NODE) == (*pT))) {
		return 0;
	} else if (EQ(d, (*pT)->data)) {
		_rbt_delete_node(pT);
		return 1;
	} else if (LT(d, (*pT)->data)) {
		return rbt_delete(&((*pT)->lchild), d);
	} else {
		return rbt_delete(&((*pT)->rchild), d);
	}
}

/**
* @brief Red-Black tree traveral method.
*
* @param T Red-Black tree.
* @param function traveral a node callback function.
*/
PUBLIC void rbt_traveral(TREE T, void function(NODE*)) {
	if (!T) { return; }
	if ((&NIL_NODE) != T->lchild) { rbt_traveral(T->lchild, function); }
	function(T);
	if ((&NIL_NODE) != T->rchild) { rbt_traveral(T->rchild, function); }
}

/////////////////////////////// TEST CASE //////////////////////////////////////

void printout_node(NODE* p) { printf("%d ", p->data); }

/**
* @brief test case for single insert, search and delete method.
*/
void testcase_for_single() {
	TREE T = rbt_init();

	printf("rbt_insert(T, 32) = %d\n", rbt_insert(&T, 32));

	printf("rbt_search(T, 32) = %d\n", rbt_search(T, 32));

	printf("rbt_delete(T, 32) = %d\n", rbt_delete(&T, 32));

	printf("bst_traveral: ");
	rbt_traveral(T, printout_node);
	printf("\n");

	rbt_destory(T);
}

#include <time.h>

/**
* @brief test case for random number insert, search and delete.
*/
void testcase_for_random() {
	TREE T = rbt_init();
	int i = 0, n = 100;
	int _t = 0;
	srand(time(NULL));
	for (i = 0; i < n; i ++) {
		_t = rand() % n;
		printf("rbt_insert(&T, %d) = %d\n", _t, rbt_insert(&T, _t));
	}
	for (i = 0; i < n; i ++) {
		_t = rand() % n;
		printf("rbt_search(T, %d) = %d\n", _t, rbt_search(T, _t));
	}
	printf("rbt_traveral: ");
	rbt_traveral(T, printout_node);
	printf("\n");
//	for (i = 0; i < n; i ++) {
//		_t = rand() % n;
//		printf("rbt_delete(&T, %d) = %d\n", _t, rbt_delete(&T, _t));
//	}
//	printf("rbt_traveral: ");
//	rbt_traveral(T, printout_node);
//	printf("\n");
	rbt_destory(T);
}

int main(int argc, const char *argv[])
{
	// testcase_for_single();
	testcase_for_random();

	return 0;
}
