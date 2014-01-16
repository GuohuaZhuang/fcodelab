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
* @file binary_search_tree.c
* @brief binary search/sort tree implement in c.
* @author firstboy0513
* @version 0.0.1
* @date 2014-01-16
*/
#include <stdio.h>
#include <stdlib.h>

/**
* @brief tree node.
*/
typedef struct _NODE {
	int key;
	struct _NODE* lchild;
	struct _NODE* rchild;
} NODE, *TREE;

/**
* @brief binary search tree initiation.
*
* @return 
*/
TREE bst_init() {
	return NULL;
}

/**
* @brief binary search tree destory.
*
* @param T
*/
void bst_destory(TREE T) {
	if (T->lchild) { bst_destory(T->lchild); }
	if (T->rchild) { bst_destory(T->rchild); }
	free(T);
}

/**
* @brief binary search/sort tree search method, if find the key in the tree, 
* then return true(means 1) and output the node point in pret, otherwise not 
* find the key in the tree return false(means 0) and output the father node 
* point in pret.
*
* @param T tree point.
* @param key key to search.
* @param father father node point.
* @param pret output node point if found, otherwise return its father node 
* point.
*
* @return return 1 means find it, otherwise return 0 means not find.
*/
int bst_search(TREE T, const int key, NODE* father, NODE** pret) {
	if (!T) { *pret = father; return 0; }
	if (T->key == key) { *pret = T; return 1; }
	else if (T->key > key) { return bst_search(T->lchild, key, T, pret); }
	else /*if (T->key < key)*/ { return bst_search(T->rchild, key, T, pret); }
}

/**
* @brief binary search/sort tree insert method, if the key is already in the 
* tree, then return 0 means insert failed, otherwise return 1 means insert 
* success.
*
* @param pT tree point point.
* @param key key to insert.
*
* @return return 1 means insert ok, otherwise return 0 means failed.
*/
int bst_insert(TREE* pT, const int key) {
	NODE* p = NULL;
	if (bst_search((*pT), key, NULL, &p)) {
		return 0;
	} else {
		NODE* s = (NODE*) malloc(sizeof(NODE));
		s->key = key; s->lchild = NULL; s->rchild = NULL;
		if (!p) { (*pT) = s; }
		else if (p->key > key) { p->lchild = s; }
		else { p->rchild = s; }
		return 1;
	}
}

/**
* @brief binary search/sort tree delete node, used for delete method.
*
* @param pp input and output node point point.
*/
void bst_delete_node(NODE** pp) {
	NODE* _tp = NULL; NODE* s = NULL;
	if (!((*pp)->lchild)) {
		_tp = *pp; *pp = (*pp)->rchild; free(_tp);
	} else if (!((*pp)->rchild)) {
		_tp = *pp; *pp = (*pp)->lchild; free(_tp);
	} else {
		_tp = (*pp); s = (*pp)->lchild;
		while (s->rchild) { _tp = s; s = s->rchild; }
		(*pp)->key = s->key;
		if (_tp != (*pp)) { _tp->rchild = s->lchild; }
		else { _tp->lchild = s->lchild; }
		free(s);
	}
}

/**
* @brief binary search/sort tree delete method.
*
* @param T tree point.
* @param key key to delete.
*
* @return return 1 means delete ok, otherwise return 0 means failed.
*/
int bst_delete(TREE T, const int key) {
	if (!T) { return 0; }
	if (T->key == key) { bst_delete_node(&T); return 1; }
	else if (T->key > key) { return bst_delete(T->lchild, key); }
	else { return bst_delete(T->rchild, key); }
}

typedef void BST_TRAVERSE_FUN(NODE*);

/**
* @brief binary search/sort tree traverse method.
*
* @param T tree point.
* @param visit traverse function.
*/
void bst_traverse(TREE T, BST_TRAVERSE_FUN visit) {
	if (!T) { return; }
	if (T->lchild) { bst_traverse(T->lchild, visit); }
	visit(T);
	if (T->rchild) { bst_traverse(T->rchild, visit); }
}

int main(int argc, const char *argv[])
{
	const int array[] = {53, 23, 54, 29, 95, 34, 22, 21, 53};
	const int size = sizeof(array) / sizeof(array[0]);

	TREE T = bst_init();

	int i = 0;
	for (i = 0; i < size; i ++) {
		bst_insert(&T, array[i]);
	}

	void printout_bst_node(NODE* p) { printf("%d ", p->key); }

	printf("traverse printout:\n\t");
	bst_traverse(T, printout_bst_node);
	printf("\n");

	NODE* p = NULL;
	printf("bst_search(T, 23) = %d\n", bst_search(T, 23, NULL, &p));
	printf("bst_search(T, 25) = %d\n", bst_search(T, 25, NULL, &p));
	printf("bst_delete(T, 23) = %d\n", bst_delete(T, 23));
	printf("bst_search(T, 23) = %d\n", bst_search(T, 23, NULL, &p));

	printf("traverse printout:\n\t");
	bst_traverse(T, printout_bst_node);
	printf("\n");

	bst_destory(T);

	return 0;
}
