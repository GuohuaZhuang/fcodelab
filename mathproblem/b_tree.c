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
* @file b_tree.c
* @brief B tree implement in C.
* @author firstboy0513
* @version 0.0.1
* @date 2014-02-01
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief B tree public method extern to use or private be invoke.
*/
#define PUBLIC
#define PRIVATE static

#define NIL		NULL

#define BT					4
#define FULL_KEY_COUNT 		(2*BT-1)
#define FULL_CHILD_COUNT 	(2*BT)

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
* @brief Boolean type define.
*/
typedef enum { FALSE, TRUE } BOOL;

/**
* @brief B tree node struct, with x.n and leaf boolean, and n keys and n+1 
* children pointers. The keys seperate the ranges of keys stored in each 
* subtree. All leaves have the same depth, which is the tree's height h.
* Nodes have lower and upper bounds on the number of keys they can contain.
* We express these bounds in terms of a fixed integer t>=2 called the minimum 
* degree of the B tree.
* 	a. Every node other than the root must have at least t-1 keys. Every 
* 	internal node other than the root must thus has at least t children. If the
* 	tree is nonempty, the root must have at least one key.
* 	b. Every node may contain at most 2t-1 keys. Therefore, an internal node may
* 	have at most 2t children. We say that a node is full if it contains exactly 
* 	2t-1 keys.
*/
typedef struct _NODE {
	int n;				// the numbers of keys currently stored in the node.
	BOOL leaf;			// a boolean value that is TRUE if x is a leaf and FALSE
						// if x is an Internal node.
	ELEMENT* key;		// x.n keys themselves, from key1 to keyn stored in 
						// nondecreasing order.
	struct _NODE** c;	// if node is internal, it contains x.n+1 pointers from
						// c1 to cn+1 to its children. Leaf nodes have no 
						// children, and the ci attributes are undefined.
} NODE;

/**
* @brief search result struct.
*/
typedef struct _SEARCH_RET {
	NODE* x;			// node pointer.
	int i;				// the index of key.
} SEARCH_RET;

#define SEARCH_NIL (SEARCH_RET){NIL, 0}

/**
* @brief B tree struct with root point.
*/
typedef struct _TREE {
	struct _NODE* root;
} TREE;

/**
* @brief Out use methods of B tree structure.
*/
PUBLIC TREE* btree_create();
PUBLIC void btree_destory(TREE* T);
PUBLIC int btree_search(TREE* T, ELEMENT d);
PUBLIC int btree_insert(TREE* T, ELEMENT d);
PUBLIC int btree_delete(TREE* T, ELEMENT d);
PUBLIC void btree_traversal(TREE* T, void function(NODE*));

/**
* @brief Internal use method of B tree structure.
*/
PRIVATE void _btree_disk_read(NODE* x);
PRIVATE void _btree_disk_write(NODE* x);
PRIVATE NODE* _btree_allocate_node();
PRIVATE void _btree_create(TREE* T);
PRIVATE void _btree_free_node(NODE* x);
PRIVATE void _btree_destory_node(NODE* x);
PRIVATE SEARCH_RET _btree_search(NODE* x, ELEMENT k);
PRIVATE void _btree_split_child(NODE* x, int i);
PRIVATE void _btree_merge_child(NODE* x, int i);
PRIVATE void _btree_insert_nonfull(NODE* x, ELEMENT k);
PRIVATE void _btree_delete_balance(NODE* x, int i);
PRIVATE void _btree_delete(NODE* x, ELEMENT k);
PRIVATE void _btree_traveral(NODE* x, void function(ELEMENT));

// TODO
PRIVATE void _btree_disk_read(NODE* x) {
}

// TODO
PRIVATE void _btree_disk_write(NODE* x) {
}

/**
* @brief Internal methods of B tree structure.
*/
PRIVATE NODE* _btree_allocate_node() {
	NODE* x = (NODE*) malloc(sizeof(NODE));
	memset(x, 0, sizeof(NODE));
	x->c = (struct _NODE**) malloc(sizeof(struct _NODE*) * FULL_CHILD_COUNT);
	x->key = (ELEMENT*) malloc(sizeof(ELEMENT) * FULL_KEY_COUNT);
	memset(x->c, 0, sizeof(struct _NODE*) * FULL_CHILD_COUNT);
	memset(x->key, 0, sizeof(ELEMENT) * FULL_KEY_COUNT);
	return x;
}

/**
* @brief B tree internal create method.
*
* @param T B tree had malloc memory.
*/
PRIVATE void _btree_create(TREE* T) {
	NODE* x = _btree_allocate_node();
	x->leaf = TRUE;
	x->n = 0;
	_btree_disk_write(x);
	T->root = x;
}

/**
* @brief B tree create and initizlization method.
*
* @return return the initizlization B tree.
*/
PUBLIC TREE* btree_create() {
	TREE* T = (TREE*) malloc(sizeof(TREE));
	memset(T, 0, sizeof(TREE));
	_btree_create(T);
	return T;
}

/**
* @brief B tree internal free node.
* This method is just free the node memory, not include its children memorys.
*
* @param x the node pointer.
*/
PRIVATE void _btree_free_node(NODE* x) {
	if (!x) { return; }
	free(x->c);
	free(x->key);
	free(x);
}

/**
* @brief B tree internal destory method.
*
* @param x subtree root use to recursive destory.
*/
PRIVATE void _btree_destory_node(NODE* x) {
	if (!x) { return; }
	int i = 0;
	if (!(x->leaf)) {
		for (i = 0; i <= x->n; i ++) {
			_btree_destory_node(x->c[i]);
		}
	}
	_btree_free_node(x);
}

/**
* @brief B tree destory method.
*
* @param T B tree.
*/
PUBLIC void btree_destory(TREE* T) {
	if (!T) { return; }
	_btree_destory_node(T->root);
	free(T);
}

/**
* @brief B tree internal search.
*
* @param x subtree root.
* @param k the element to search.
*
* @return return the search result, if not find the element, it will return 
* SEARCH_NIL, which the node pointer is NIL.
*/
PRIVATE SEARCH_RET _btree_search(NODE* x, ELEMENT k) {
	int i = 0;
	while (i < x->n && GT(k, x->key[i])) { i ++; }
	if (i < x->n && EQ(k, x->key[i])) {
		return (SEARCH_RET){x, i};
	} else if (x->leaf) {
		return SEARCH_NIL;
	} else {
		_btree_disk_read(x->c[i]);
		return _btree_search(x->c[i], k);
	}
}

/**
* @brief B tree search method.
*
* @param T B tree.
* @param k the element to search.
*
* @return return 1 means find the element is in the tree, otherwise return 0 
* means the element can not find in the tree.
*/
PUBLIC int btree_search(TREE* T, ELEMENT k) {
	if (!T || !(T->root)) { return 0; }
	SEARCH_RET sr;
	sr = _btree_search(T->root, k);
	if (NIL == sr.x) {
		return 0;
	} else {
		return 1;
	}
}

/**
* @brief B tree split child.
* Require the x->c[i] child node has full keys.
*
* @param x x node's i child.
* @param i index of the child pointer.
*/
PRIVATE void _btree_split_child(NODE* x, int i) {
	NODE* z = _btree_allocate_node();
	NODE* y = x->c[i];
	z->leaf = y->leaf;
	z->n = BT-1;
	int j = 0;
	// Split x->c[i] into y and z, the y indicate x->c[i], and z indicate the 
	// newer allocate node.
	// 1. move end part of y key[] and c[] into z, also update both n and leaf.
	// 2. move x key[] and c[] next step let x->c[i+1] and x->key[i] blank.
	// 3. move the middle element in y into the x->c[i+1] and x->key[i].
	for (j = 0; j < BT-1; j ++) { ELEMENT_COPY(z->key[j], y->key[BT+j]); }
	if (!(y->leaf)) { for (j = 0; j < BT; j ++) { z->c[j] = y->c[BT+j]; } }
	y->n = BT-1;

	for (j = x->n; j >= i+1; j --) { x->c[j+1] = x->c[j]; }
	x->c[i+1] = z;
	for (j = x->n-1; j >= i; j --) { ELEMENT_COPY(x->key[j+1], x->key[j]); }
	ELEMENT_COPY(x->key[i], y->key[BT-1]);
	x->n ++;
	_btree_disk_write(y);
	_btree_disk_write(z);
	_btree_disk_write(x);
}

/**
* @brief B tree merge child.
* Some preconditions:
*   1. Require x is not a leaf node.
*   2. Not require the x->c[i] and x->c[i+1] child has BT-1 keys.
*
* @param x x node's i child. it must lest than x->n-1, let i+1 index valid.
* @param i index of the left child pointer.
*/
PRIVATE void _btree_merge_child(NODE* x, int i) {
	NODE* y = x->c[i];
	NODE* z = x->c[i+1];
	int j = 0, yn = y->n, zn = z->n;
	// Merge x->c[i] and x->c[i+1] node into x->c[i].
	// Let y indicate x->c[i] node, and z indicate x->c[i+1] node.
	// 1. move x->key[i] to y->key[yn];
	// 2. move z's key[] and c[] to the end of y node, also update y->n.
	// 3. move x's key[] and c[] front step to cover x->key[i] and x->c[i+1].
	y->key[yn] = x->key[i];
	for (j = 0; j < zn; j ++) { ELEMENT_COPY(y->key[yn+1+j], z->key[j]); }
	if (!(z->leaf)) { for (j = 0; j <= zn; j ++) { y->c[yn+1+j] = z->c[j]; } }
	y->n = yn+1+zn;

	for (j = i+1; j < x->n; j ++) { ELEMENT_COPY(x->key[j-1], x->key[j]); }
	for (j = i+1; j < x->n; j ++) { x->c[j] = x->c[j+1]; }
	x->n --;

	_btree_disk_write(y);
	/// _btree_disk_write(z); delete z in the disk file.
	_btree_disk_write(x);

	_btree_free_node(z);
}

/**
* @brief B tree internal insert element into non-full node method.
*
* @param x the subtree root.
* @param k the element to insert.
*/
PRIVATE void _btree_insert_nonfull(NODE* x, ELEMENT k) {
	int i = x->n-1;
	if (x->leaf) {
		while (i >= 0 && LT(k, x->key[i])) {
			ELEMENT_COPY(x->key[i+1], x->key[i]);
			i --;
		}
		ELEMENT_COPY(x->key[i+1], k);
		x->n ++;
		_btree_disk_write(x);
	} else {
		while (i >= 0 && LT(k, x->key[i])) { i --; }
		i ++;
		_btree_disk_read(x->c[i]);
		if (FULL_KEY_COUNT == x->c[i]->n) {
			_btree_split_child(x, i);
			if (GT(k, x->key[i])) { i ++; }
		}
		_btree_insert_nonfull(x->c[i], k);
	}
}

/**
* @brief B tree insert method.
*
* @param T B tree.
* @param k the element to insert.
*
* @return return 1 means insert success, otherwise return 0 means not insert it
* as the element is already include in the B tree.
*/
PUBLIC int btree_insert(TREE* T, ELEMENT k) {
	if (btree_search(T, k)) { return 0; }
	NODE* r = T->root;
	if (FULL_KEY_COUNT == r->n) {
		NODE* s = _btree_allocate_node();
		T->root = s;
		s->leaf = FALSE;
		s->n = 0;
		s->c[0] = r;
		_btree_split_child(s, 0);
		_btree_insert_nonfull(s, k);
	} else {
		_btree_insert_nonfull(r, k);
	}
	return 1;
}

/**
* @brief B tree internal balance after delete method.
*
* @param x the subtree root.
* @param i the x->c[i] child delete one element.
*/
PRIVATE void _btree_delete_balance(NODE* x, int i) {
	if (x->c[i]->n >= BT-1) { return; }
	if (i > 0 && x->c[i-1]->n >= BT) {
		// left immediate sibling
		// before to move origin key[] and c[] in x->c[i] move next
		// Let y indicate x->c[i] node, and z indicate x->c[i-1] node.
		// 1. move y next step, let y->key[0] and y->c[0] blank.
		// 2. move x->key[i-1] to y->key[0], z->key[zn-1] to x->key[i-1].
		// 3. move z->c[zn] to y->c[0].
		// 4. update y and z n keys count.
		NODE *y = x->c[i], *z = x->c[i-1];
		int j = 0, yn = y->n, zn = z->n;
		for (j = yn-1; j >= 0; j --) { y->key[j+1] = y->key[j]; }	// step 1
		for (j = yn; j >= 0; j --) { y->c[j+1] = y->c[j]; }      	// step 1
		ELEMENT_COPY(y->key[0], x->key[i-1]);   					// step 2
		ELEMENT_COPY(x->key[i-1], z->key[zn-1]);					// step 2
		y->c[0] = z->c[zn];											// step 3
		y->n ++; z->n --;											// step 4
	} else if (i < x->n-1 && x->c[i+1]->n >= BT) {
		// right immediate sibling
		// after to move origin key[] and c[] in x->c[i+1] move front
		// let y indicate x->c[i] node, and z indicate x->c[i+1] node.
		// 1. move x->key[i] to y->key[yn], z->key[0] to x->key[i].
		// 2. move z->c[0] to y->c[yn+1].
		// 3. move z front step, cover the z->key[0] and z->c[0].
		// 4. update y and z n keys count.
		NODE *y = x->c[i], *z = x->c[i+1];
		int j = 0, yn = y->n, zn = z->n;
		ELEMENT_COPY(y->key[yn], x->key[i]);						// step 1
		ELEMENT_COPY(x->key[i], z->key[0]); 						// step 1
		y->c[yn+1] = z->c[0];               						// step 2
		for (j = 0; j < zn-1; j ++) { z->key[j] = z->key[j+1]; }	// step 3
		for (j = 0; j < zn; j ++) { z->c[j] = z->c[j+1]; }			// step 3
		y->n ++; z->n --;											// step 4
	} else {
		// merge with one sibling
		if (i > 0) { // merge left sibling
			// first move c->key[i] into x->c[i], let x->c[i]->n == BT-1.
			// or modify the merge method let it can merge not BT-1 length.
			_btree_merge_child(x, i-1);
		} else { // merge right sibling
			// first move c->key[i] into x->c[i], let x->c[i]->n == BT-1.
			// or modify the merge method let it can merge not BT-1 length.
			_btree_merge_child(x, i);
		}
	}
}

/**
* @brief B tree internal delete from subtree root.
*
* @param x subtree root.
* @param k element to delete.
*/
PRIVATE void _btree_delete(NODE* x, ELEMENT k) {
	int j = 0;
	if (x->leaf) {
		int i = x->n-1;
		while (i >= 0 && LT(k, x->key[i])) { i --; }
		if (i >= 0 && EQ(k, x->key[i])) {
			for (j = i+1; j <= x->n-1; j ++) {
				ELEMENT_COPY(x->key[j-1], x->key[j]);
			}
			x->n --;
		}
	} else {
		int i = x->n-1;
		while (i >= 0 && LT(k, x->key[i])) { i --; }
		if (i >= 0 && EQ(k, x->key[i])) { // k is in a internal node
			if (x->c[i]->n > BT-1) {
				ELEMENT k_r = x->c[i]->key[x->c[i]->n-1];
				ELEMENT_COPY(x->key[i], k_r);
				_btree_delete(x->c[i], k_r); _btree_delete_balance(x, i);
			} else if (x->c[i+1]->n > BT-1) {
				ELEMENT k_r = x->c[i+1]->key[x->c[i+1]->n-1];
				ELEMENT_COPY(x->key[i], k_r);
				_btree_delete(x->c[i+1], k_r); _btree_delete_balance(x, i);
			} else {
				_btree_merge_child(x, i);
				_btree_delete(x->c[i], k); _btree_delete_balance(x, i);
			}
		} else {
			i ++;
			_btree_disk_read(x->c[i]);
			_btree_delete(x->c[i], k); _btree_delete_balance(x, i);
		}
	}
}

/**
* @brief B tree delete method.
*
* @param T B tree.
* @param k the element to delete.
*
* @return return 1 means delete success, otherwise return 0 means not delete it
* as the element k is not include in the B tree.
*/
PUBLIC int btree_delete(TREE* T, ELEMENT k) {
	if (!btree_search(T, k)) { return 0; }
	_btree_delete(T->root, k);
	return 1;
}

/**
* @brief B tree internal traveral recursion method.
*
* @param p subtree node point.
* @param function traveral a node callback function.
*/
PRIVATE void _btree_traveral(NODE* x, void function(ELEMENT)) {
	if (x) {
		int i = 0;
		for (i = 0; i < x->n; i ++) {
			_btree_traveral(x->c[i], function);
			function(x->key[i]);
		}
		_btree_traveral(x->c[i], function);
	}
}

/**
* @brief B tree traveral method.
*
* @param T B tree.
* @param function traveral a element callback function.
*/
PUBLIC void btree_traveral(TREE* T, void function(ELEMENT)) {
	_btree_traveral(T->root, function);
}

/////////////////////////////// TEST CASE //////////////////////////////////////

void printout_node(ELEMENT k) { printf("%d ", k); }

/**
* @brief test case for single insert, search and delete method.
*/
void testcase_for_single() {
	TREE* T = btree_create();

	printf("btree_insert(T, 32) = %d\n", btree_insert(T, 32));

	printf("btree_search(T, 32) = %d\n", btree_search(T, 32));

	printf("btree_delete(T, 32) = %d\n", btree_delete(T, 32));

	printf("btree_search(T, 32) = %d\n", btree_search(T, 32));

	printf("btree_traveral: ");
	btree_traveral(T, printout_node);
	printf("\n");

	btree_destory(T);
}

#include <time.h>

/**
* @brief test case for random number insert, search and delete.
*/
void testcase_for_random() {
	TREE* T = btree_create();
	int i = 0, n = 100;
	int _t = 0;
	srand(time(NULL));
	for (i = 0; i < n; i ++) {
		_t = rand() % n;
		printf("btree_insert(T, %d) = %d\n", _t, btree_insert(T, _t));
	}
	for (i = 0; i < n; i ++) {
		_t = rand() % n;
		printf("btree_search(T, %d) = %d\n", _t, btree_search(T, _t));
	}

	printf("btree_traveral: ");
	btree_traveral(T, printout_node);
	printf("\n");

	for (i = 0; i < n; i ++) {
		_t = rand() % n;
		printf("btree_delete(T, %d) = %d\n", _t, btree_delete(T, _t));
	}

	printf("btree_traveral: ");
	btree_traveral(T, printout_node);
	printf("\n");

	btree_destory(T);
}

int main(int argc, const char *argv[])
{
	testcase_for_single();
	testcase_for_random();

	return 0;
}
