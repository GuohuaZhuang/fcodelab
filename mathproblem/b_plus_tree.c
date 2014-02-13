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
* @file b_plus_tree.c
* @brief B+ tree implement in C.
* references:
* 	http://en.wikipedia.org/wiki/B%2B_tree
* @author firstboy0513
* @version 0.0.1
* @date 2014-02-13
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/**
 * @brief B+ tree public method extern to use or private be invoke.
*/
#define PUBLIC
#define PRIVATE static

#define NIL		NULL

/**
 * @brief the order, or branching factor, ORDER_B of a B+ tree measures the 
 * capacity of nodes (i.e., the number of children nodes) for internal nodes in
 * the tree.
*/
#define ORDER_B				4
#define FULL_KEY_COUNT 		(ORDER_B)
#define FULL_CHILD_COUNT 	(ORDER_B+1)

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
* @brief B+ tree node struct, with x.n and leaf boolean, and n keys and n+1 
* children pointers. The keys seperate the ranges of keys stored in each 
* subtree. All leaves have the same depth, which is the tree's height h.
* Nodes have lower and upper bounds on the number of keys they can contain.
* We express these bounds in terms of a fixed integer t>=2 called the minimum 
* degree of the B+ tree.
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
	void** c;			// if node is internal, it contains x.n+1 pointers from
						// c1 to cn+1 to its children. Leaf node contains key
						// value record pointers.
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
* @brief B+ tree struct with root point.
*/
typedef struct _TREE {
	struct _NODE* root;
} TREE;

/**
* @brief Out use methods of B+ tree structure.
*/
PUBLIC TREE* bptree_create();
PUBLIC void bptree_destory(TREE* T);
PUBLIC int bptree_search(TREE* T, ELEMENT d);
PUBLIC int bptree_insert(TREE* T, ELEMENT d);
PUBLIC int bptree_delete(TREE* T, ELEMENT d);
PUBLIC void bptree_traversal(TREE* T, void function(NODE*));

/**
* @brief Internal use method of B+ tree structure.
*/
PRIVATE void _bptree_disk_read(NODE* x);
PRIVATE void _bptree_disk_write(NODE* x);
PRIVATE NODE* _bptree_allocate_node();
PRIVATE void _bptree_create(TREE* T);
PRIVATE void _bptree_free_node(NODE* x);
PRIVATE void _bptree_destory_node(NODE* x);
PRIVATE SEARCH_RET _bptree_search(NODE* x, ELEMENT k);
PRIVATE void _bptree_split_child(NODE* x, int i);
PRIVATE void _bptree_merge_child(NODE* x, int i);
PRIVATE void _bptree_insert_nonfull(NODE* x, ELEMENT k);
PRIVATE void _bptree_delete_balance(NODE* x, int i);
PRIVATE void _bptree_delete(NODE* x, ELEMENT k);
PRIVATE void _bptree_traveral(NODE* x, void function(ELEMENT));
PRIVATE NODE* _bptree_leftmost(TREE* T);

// TODO
PRIVATE void _bptree_disk_read(NODE* x) {
}

// TODO
PRIVATE void _bptree_disk_write(NODE* x) {
}

/**
* @brief Internal methods of B+ tree structure.
*/
PRIVATE NODE* _bptree_allocate_node() {
	NODE* x = (NODE*) malloc(sizeof(NODE));
	memset(x, 0, sizeof(NODE));
	x->c = (void**) malloc(sizeof(void*) * FULL_CHILD_COUNT);
	x->key = (ELEMENT*) malloc(sizeof(ELEMENT) * FULL_KEY_COUNT);
	memset(x->c, 0, sizeof(void*) * FULL_CHILD_COUNT);
	memset(x->key, 0, sizeof(ELEMENT) * FULL_KEY_COUNT);
	return x;
}

/**
* @brief B+ tree internal create method.
*
* @param T B+ tree had malloc memory.
*/
PRIVATE void _bptree_create(TREE* T) {
	NODE* x = _bptree_allocate_node();
	x->leaf = TRUE;
	x->n = 0;
	_bptree_disk_write(x);
	T->root = x;
}

/**
* @brief B+ tree create and initizlization method.
*
* @return return the initizlization B+ tree.
*/
PUBLIC TREE* bptree_create() {
	TREE* T = (TREE*) malloc(sizeof(TREE));
	memset(T, 0, sizeof(TREE));
	_bptree_create(T);
	return T;
}

/**
* @brief B+ tree internal free node.
* This method is just free the node memory, not include its children memorys.
*
* @param x the node pointer.
*/
PRIVATE void _bptree_free_node(NODE* x) {
	if (!x) { return; }
	free(x->c);
	free(x->key);
	free(x);
}

/**
* @brief B+ tree internal destory method.
*
* @param x subtree root use to recursive destory.
*/
PRIVATE void _bptree_destory_node(NODE* x) {
	if (!x) { return; }
	int i = 0;
	if (!(x->leaf)) {
		for (i = 0; i <= x->n; i ++) {
			_bptree_destory_node(x->c[i]);
		}
	}
	_bptree_free_node(x);
}

/**
* @brief B+ tree destory method.
*
* @param T B+ tree.
*/
PUBLIC void bptree_destory(TREE* T) {
	if (!T) { return; }
	_bptree_destory_node(T->root);
	free(T);
}

/**
* @brief B+ tree internal search.
* Keys may be duplicated; Every key to the right of a particular key is >= to 
* that key, and corresponding the left of a particular key is < to that key.
*
* @param x subtree root.
* @param k the element to search.
*
* @return return the search result, if not find the element, it will return 
* SEARCH_NIL, which the node pointer is NIL.
*/
PRIVATE SEARCH_RET _bptree_search(NODE* x, ELEMENT k) {
	int i = 0;
	while (i < x->n && GT(k, x->key[i])) { i ++; }
	if (x->leaf) {
		if (i < x->n && EQ(k, x->key[i])) {
			return (SEARCH_RET){x, i};
		} else {
			return SEARCH_NIL;
		}
	} else {
		if (i < x->n && EQ(k, x->key[i])) { i ++; } // equal case go right
		_bptree_disk_read(x->c[i]);
		return _bptree_search(x->c[i], k);
	}
}

/**
* @brief B+ tree search method.
*
* @param T B+ tree.
* @param k the element to search.
*
* @return return 1 means find the element is in the tree, otherwise return 0 
* means the element can not find in the tree.
*/
PUBLIC int bptree_search(TREE* T, ELEMENT k) {
	if (!T || !(T->root)) { return 0; }
	SEARCH_RET sr;
	sr = _bptree_search(T->root, k);
	if (NIL == sr.x) {
		return 0;
	} else {
		return 1;
	}
}

/**
* @brief B+ tree split child.
* Require the x->c[i] child node has full keys.
*
* @param x x node's i child.
* @param i index of the child pointer.
*/
PRIVATE void _bptree_split_child(NODE* x, int i) {
	int j = 0, L_H = (int)ceil((double)ORDER_B/2), R_H = ORDER_B - L_H;
	NODE* z = _bptree_allocate_node();
	NODE* y = x->c[i];
	z->leaf = y->leaf;
	z->n = R_H;
	// Split x->c[i] into y and z, the y indicate x->c[i], and z indicate the 
	// newer allocate node.
	// if (y is a leaf node) {
	//   1. move end R_H of y key[] and c[] into z, and update sequence pointer.
	//   2. move the z->key[0] and z into x.
	//   3. update z->n = R_H, y->n = L_H and x->n ++.
	// } else { // y is an internal node
	//   1. move end R_H of y key[] and c[] into z.
	//   2. move the y->key[L_H-1] and y->c[L_H] into x.
	//   3. update z->n = R_H, y->n = L_H-1 and x->n ++.
	// }
	for (j = 0; j < R_H; j ++) { ELEMENT_COPY(z->key[j], y->key[L_H+j]); }
	for (j = 0; j <= R_H; j ++) { z->c[j] = y->c[L_H+j]; }
	for (j = x->n; j >= i+1; j --) { x->c[j+1] = x->c[j]; }
	for (j = x->n-1; j >= i; j --) { ELEMENT_COPY(x->key[j+1], x->key[j]); }
	if (y->leaf) {
		z->c[ORDER_B-1] = y->c[ORDER_B-1];
		y->c[ORDER_B-1] = z;
		y->n = L_H;
		ELEMENT_COPY(x->key[i], z->key[0]);
	} else {
		y->n = L_H-1;
		ELEMENT_COPY(x->key[i], y->key[L_H-1]);
	}
	x->c[i+1] = z;
	x->n ++;

	_bptree_disk_write(y);
	_bptree_disk_write(z);
	_bptree_disk_write(x);
}

/**
* @brief B+ tree internal insert element into non-full node method.
*
* @param x the subtree root.
* @param k the element to insert.
*/
PRIVATE void _bptree_insert_nonfull(NODE* x, ELEMENT k) {
	int i = x->n-1;
	if (x->leaf) {
		while (i >= 0 && LT(k, x->key[i])) {
			ELEMENT_COPY(x->key[i+1], x->key[i]);
			i --;
		}
		ELEMENT_COPY(x->key[i+1], k);
		x->n ++;
		_bptree_disk_write(x);
	} else {
		while (i >= 0 && LT(k, x->key[i])) { i --; }
		i ++;
		_bptree_disk_read(x->c[i]);
		_bptree_insert_nonfull(x->c[i], k);
		if (ORDER_B == ((NODE*)(x->c[i]))->n) {
			_bptree_split_child(x, i);
		}
	}
}

/**
* @brief B+ tree insert method.
*
* @param T B+ tree.
* @param k the element to insert.
*
* @return return 1 means insert success, otherwise return 0 means not insert it
* as the element is already include in the B+ tree.
*/
PUBLIC int bptree_insert(TREE* T, ELEMENT k) {
	if (bptree_search(T, k)) { return 0; }	// if duplicated is not support
	NODE* r = T->root;
	_bptree_insert_nonfull(r, k);
	if (ORDER_B == r->n) {
		NODE* s = _bptree_allocate_node();
		T->root = s;
		s->leaf = FALSE;
		s->n = 0;
		s->c[0] = r;
		_bptree_split_child(s, 0);
	}
	return 1;
}

/**
* @brief B+ tree merge child.
*
* @param x x node's i child. it must lest than x->n-1, let i+1 index valid.
* @param i index of the left child pointer.
*/
PRIVATE void _bptree_merge_child(NODE* x, int i) {
	NODE* y = x->c[i];
	NODE* z = x->c[i+1];
	int j = 0, yn = y->n, zn = z->n;
	// Merge x->c[i] and x->c[i+1] node into x->c[i].
	// Let y indicate x->c[i] node, and z indicate x->c[i+1] node.
	// if (y is a leaf node) {
	//     1. move z->key[] and z->c[] into the end of y, and update y->n.
	//     2. remove x->key[i] and x->c[i+1], and reduce x->n.
	// } else { // y is an internal node
	//     1. move x->key[i] to y->key[yn];
	//     2. move z's key[] and c[] to the end of y node, also update y->n.
	//     3. remove x->key[i] and x->c[i+1], and reduce x->n.
	// }
	if (y->leaf) {
		for (j = 0; j < zn; j ++) {
			ELEMENT_COPY(y->key[yn+j], z->key[j]); y->c[yn+j] = z->c[j];
		}
		y->c[ORDER_B-1] = z->c[ORDER_B-1]; // copy the sequence pointer
		y->n = yn+zn;
	} else {
		y->key[yn] = x->key[i];
		for (j = 0; j < zn; j ++) {
			ELEMENT_COPY(y->key[yn+1+j], z->key[j]); y->c[yn+1+j] = z->c[j];
		}
		y->c[yn+1+j] = z->c[j];
		y->n = yn+1+zn;
	}

	for (j = i+1; j < x->n; j ++) {
		ELEMENT_COPY(x->key[j-1], x->key[j]);
		x->c[j] = x->c[j+1];
	}
	x->n --;

	_bptree_disk_write(y);
	/// _bptree_disk_write(z); delete z in the disk file.
	_bptree_disk_write(x);

	_bptree_free_node(z);
}

/**
* @brief B+ tree internal balance after delete method.
*
* @param x the subtree root.
* @param i the x->c[i] child delete one element.
*/
PRIVATE void _bptree_delete_balance(NODE* x, int i) {
	int INTERNAL_LOWER_LIMIT = (int)ceil((double)ORDER_B/2);
	int LEAF_LOWER_LIMIT = (int)floor((double)ORDER_B/2);
	NODE* y = x->c[i];
	if (y->leaf && y->n >= LEAF_LOWER_LIMIT) { return; }
	if (!(y->leaf) && y->n >= INTERNAL_LOWER_LIMIT) { return; }
	if (i > 0 && (
		(y->leaf && ((NODE*)(x->c[i-1]))->n > LEAF_LOWER_LIMIT) || 
		(!(y->leaf) && ((NODE*)(x->c[i-1]))->n > INTERNAL_LOWER_LIMIT) )) {
		// left immediate sibling
		// before to move origin key[] and c[] in x->c[i] move next
		// let y indicate x->c[i] node, and z indicate x->c[i-1] node.
		// if (y is a leaf node) {
		//     1. move y next step, let y->key[0] and y->c[0] blank.
		//     2. move z->key[zn-1] and z->c[zn-1] into y->key[0] and y->c[0].
		//     3. modify x->key[i-1] = y->key[0].
		//     4. update y and z n keys count.
		// } else { // y is an internal node
		//     1. move y next step, let y->key[0] and y->c[0] blank.
		//     2. move x->key[i-1] to y->key[0], z->key[zn-1] to x->key[i-1].
		//     3. move z->c[zn] to y->c[0].
		//     4. update y and z n keys count.
		// }
		NODE* z = x->c[i-1];
		int j = 0, yn = y->n, zn = z->n;
		y->c[yn+1] = y->c[yn];										// step 1
		for (j = yn-1; j >= 0; j --) {								// step 1
			y->key[j+1] = y->key[j]; y->c[j+1] = y->c[j];			// step 1
		}															// step 1
		if (y->leaf) {												// leaf
			ELEMENT_COPY(y->key[0], z->key[zn-1]);					// step 2
			y->c[0] = z->c[zn-1];									// step 2
			ELEMENT_COPY(x->key[i-1], y->key[0]);					// step 3
		} else {													// internal
			ELEMENT_COPY(y->key[0], x->key[i-1]);   				// step 2
			ELEMENT_COPY(x->key[i-1], z->key[zn-1]);				// step 2
			y->c[0] = z->c[zn];										// step 3
		}
		y->n ++; z->n --;											// step 4
	} else if (i < x->n-1 && (
		(y->leaf && ((NODE*)(x->c[i+1]))->n > LEAF_LOWER_LIMIT) || 
		(!(y->leaf) && ((NODE*)(x->c[i+1]))->n > INTERNAL_LOWER_LIMIT) )) {
		// right immediate sibling
		// after to move origin key[] and c[] in x->c[i+1] move front
		// let y indicate x->c[i] node, and z indicate x->c[i+1] node.
		// if (y is a leaf node) {
		//     1. move z->key[0] to y->key[yn] and z->c[0] to y->c[yn].
		//     2. modify x->key[i] = z->key[1].
		//     3. move z front step, cover the z->key[0] and z->c[0].
		//     4. update y->n and z->n.
		// } else { // y is an internal node
		//     1. move x->key[i] to y->key[yn], z->key[0] to x->key[i].
		//     2. move z->c[0] to y->c[yn+1].
		//     2. move z front step, cover the z->key[0] and z->c[0].
		//     4. update y and z n keys count.
		// }
		NODE* z = x->c[i+1];
		int j = 0, yn = y->n, zn = z->n;
		if (y->leaf) {												// leaf
			ELEMENT_COPY(y->key[yn], z->key[0]);					// step 1
			y->c[yn] = z->c[0];                 					// step 1
			ELEMENT_COPY(x->key[i], z->key[1]); 					// step 2
		} else {													// internal
			ELEMENT_COPY(y->key[yn], x->key[i]);					// step 1
			ELEMENT_COPY(x->key[i], z->key[0]); 					// step 1
			y->c[yn+1] = z->c[0];               					// step 2
		}
		for (j = 1; j < zn; j ++) {									// step 3
			z->key[j-1] = z->key[j]; z->c[j-1] = z->c[j];			// step 3
		}                                                			// step 3
		z->c[j-1] = z->c[j];                             			// step 3
		y->n ++; z->n --;											// step 4
	} else {
		// merge with one sibling
		if (i > 0) { // merge left sibling
			// first move c->key[i] into x->c[i], let x->c[i]->n == BT-1.
			// or modify the merge method let it can merge not BT-1 length.
			_bptree_merge_child(x, i-1);
		} else { // merge right sibling
			// first move c->key[i] into x->c[i], let x->c[i]->n == BT-1.
			// or modify the merge method let it can merge not BT-1 length.
			_bptree_merge_child(x, i);
		}
	}
}

// OK
/**
* @brief B+ tree internal delete from subtree root.
*
* @param x subtree root.
* @param k element to delete.
*/
PRIVATE void _bptree_delete(NODE* x, ELEMENT k) {
	int j = 0;
	if (x->leaf) {
		int i = x->n-1;
		while (i >= 0 && LT(k, x->key[i])) { i --; }
		if (i >= 0 && EQ(k, x->key[i])) {
			for (j = i+1; j < x->n; j ++) {
				ELEMENT_COPY(x->key[j-1], x->key[j]);
				x->c[j-1] = x->c[j];
			}
			x->n --;
		}
	} else {
		int i = x->n-1;
		while (i >= 0 && LT(k, x->key[i])) { i --; }
		i ++;
		_bptree_disk_read(x->c[i]);
		_bptree_delete(x->c[i], k); _bptree_delete_balance(x, i);
	}
}

/**
* @brief B+ tree delete method.
*
* @param T B+ tree.
* @param k the element to delete.
*
* @return return 1 means delete success, otherwise return 0 means not delete it
* as the element k is not include in the B+ tree.
*/
PUBLIC int bptree_delete(TREE* T, ELEMENT k) {
	if (!bptree_search(T, k)) { return 0; }
	_bptree_delete(T->root, k);
	NODE* r = T->root;
	if (0 == r->n) {
		T->root = r->c[0];
		_bptree_free_node(r);
	}
	return 1;
}

/**
* @brief B+ tree internal traveral recursion method.
* This traveral is not like the B tree traveral from root to leaf, b+ tree use
* its leftmost node pointer to traveral in sequence order.
*
* @param p subtree node point.
* @param function traveral a node callback function.
*/
PRIVATE void _bptree_traveral(NODE* leftmost, void function(ELEMENT)) {
	if (NIL == leftmost) { return; }
	int i = 0;
	NODE* p = leftmost;
	while (NIL != p) {
		_bptree_disk_read(p);
		for (i = 0; i < p->n; i ++) {
			function(p->key[i]);
		}
		p = p->c[ORDER_B-1];
	}
}

/**
* @brief B+ tree get leftmost node pointer.
*
* @param T B+ tree.
*
* @return return leftmost node pointer, if the tree is not exist or its root is
* NIL, it will return NIL.
*/
PRIVATE NODE* _bptree_leftmost(TREE* T) {
	if (!T) { return NIL; }
	NODE* leftmost = T->root;
	while (leftmost && !(leftmost->leaf)) {
		leftmost = leftmost->c[0];
	}
	return leftmost;
}

/**
* @brief B+ tree traveral method.
*
* @param T B+ tree.
* @param function traveral a element callback function.
*/
PUBLIC void bptree_traveral(TREE* T, void function(ELEMENT)) {
	_bptree_traveral(_bptree_leftmost(T), function);
}

/////////////////////////////// TEST CASE //////////////////////////////////////

void printout_node(ELEMENT k) { printf("%d ", k); }

/**
* @brief test case for single insert, search and delete method.
*/
void testcase_for_single() {
	TREE* T = bptree_create();

	printf("bptree_insert(T, 32) = %d\n", bptree_insert(T, 32));

	printf("bptree_search(T, 32) = %d\n", bptree_search(T, 32));

	printf("bptree_delete(T, 32) = %d\n", bptree_delete(T, 32));

	printf("bptree_search(T, 32) = %d\n", bptree_search(T, 32));

	printf("bptree_traveral: ");
	bptree_traveral(T, printout_node);
	printf("\n");

	bptree_destory(T);
}

#include <time.h>

/**
* @brief test case for random number insert, search and delete.
*/
void testcase_for_random() {
	TREE* T = bptree_create();
	int i = 0, n = 100;
	int _t = 0;
	srand(time(NULL));
	for (i = 0; i < n; i ++) {
		_t = rand() % n;
		printf("bptree_insert(T, %d) = %d\n", _t, bptree_insert(T, _t));
	}
	for (i = 0; i < n; i ++) {
		_t = rand() % n;
		printf("bptree_search(T, %d) = %d\n", _t, bptree_search(T, _t));
	}

	printf("bptree_traveral: ");
	bptree_traveral(T, printout_node);
	printf("\n");

	for (i = 0; i < n; i ++) {
		_t = rand() % n;
		printf("bptree_delete(T, %d) = %d\n", _t, bptree_delete(T, _t));
	}

	printf("bptree_traveral: ");
	bptree_traveral(T, printout_node);
	printf("\n");

	bptree_destory(T);
}

int main(int argc, const char *argv[])
{
	testcase_for_single();
	testcase_for_random();

	return 0;
}
