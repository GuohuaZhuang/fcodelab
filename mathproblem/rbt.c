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
* @file rbt.c
* @brief Red-Black Tree implement in C, and reference from the book 
* <Introduction to Algorithm> Third Edition.
* 	1. implement left rotate and right rotate internal method.
* 	2. implement insert new element method.
* 	3. implement delete element method.
* 	4. add PUBLIC and PRIVATE statement before the external and internal method.
*
* @author firstboy0513
* @version 0.0.1
* @date 2014-01-26
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

typedef struct _NODE {
	ELEMENT data;
	COLOR	color;
	struct _NODE* p;
	struct _NODE* left;
	struct _NODE* right;
} NODE;

typedef struct _TREE {
	struct _NODE* root;
	struct _NODE* nil;
} TREE;

TREE* rbt_init() {
	TREE* T = (TREE*) malloc(sizeof(TREE));
	T->nil = (NODE*) malloc(sizeof(NODE));
	memset(T->nil, 0, sizeof(NODE));
	T->root = T->nil;
	return T;
}

void _rbt_node_destory(TREE* T, NODE* p) {
	if (p != T->nil) {
		_rbt_node_destory(T, p->left);
		_rbt_node_destory(T, p->right);
		free(p);
	}
}

void rbt_destory(TREE* T) {
	_rbt_node_destory(T, T->root);
	free(T->nil);
	free(T);
}

int rbt_search(TREE* T, ELEMENT d) {
	NODE* x = T->root;
	while (x != T->nil) {
		if (EQ(d, x->data)) {
			return 1;
		} else if (LT(d, x->data)) {
			x = x->left;
		} else {
			x = x->right;
		}
	}
	return 0;
}

void _rbt_traveral(TREE* T, NODE* p, void function(NODE*)) {
	if (p != T->nil) {
		_rbt_traveral(T, p->left, function);
		function(p);
		_rbt_traveral(T, p->right, function);
	}
}

void rbt_traveral(TREE* T, void function(NODE*)) {
	_rbt_traveral(T, T->root, function);
}

int _rbt_search(TREE* T, ELEMENT d, NODE** p) {
	(*p) = T->nil;
	NODE* x = T->root;
	while (x != T->nil) {
		(*p) = x;
		if (EQ(d, x->data)) {
			return 1;
		} else if (LT(d, x->data)) {
			x = x->left;
		} else {
			x = x->right;
		}
	}
	return 0;
}

void _rbt_left_rotate(TREE* T, NODE* x) {
	NODE* r = x->right;

	x->right = r->left;
	if (r->left != T->nil) {
		r->left->p = x;
	}

	r->p = x->p;
	if (x->p == T->nil) {
		T->root = r;
	} else if (x == x->p->left) {
		x->p->left = r;
	} else {
		x->p->right = r;
	}

	r->left = x;
	x->p = r;
}

void _rbt_right_rotate(TREE* T, NODE* x) {
	NODE* l = x->left;

	x->left = l->right;
	if (l->right != T->nil) {
		l->right->p = x;
	}

	l->p = x->p;
	if (x->p == T->nil) {
		T->root = l;
	} else if (x == x->p->left) {
		x->p->left = l;
	} else {
		x->p->right = l;
	}

	l->right = x;
	x->p = l;
}

void _rbt_insert_fixup(TREE* T, NODE* s) {
	NODE* u = NULL;
	while (s->p->color == RED) {
		if (s->p == s->p->p->left) {
			u = s->p->p->right;
			if (u->color == RED) {
				s->p->p->color = RED;
				s->p->color = u->color = BLACK;
				s = s->p->p;
			} else {
				if (s == s->p->right) {
					s = s->p;
					if (s->right == T->nil) { printf("ERR!!!\n"); }
					_rbt_left_rotate(T, s);
				}
				s->p->color = BLACK;
				s->p->p->color = RED;
				if (s->p->p->left == T->nil) { printf("ERR!!!\n"); }
				_rbt_right_rotate(T, s->p->p);
			}
		} else {
			if (s->p != s->p->p->right) { printf("s->p is error!!\n"); }
			u = s->p->p->left;
			if (u->color == RED) {
				s->p->p->color = RED;
				s->p->color = u->color = BLACK;
				s = s->p->p;
			} else {
				if (s == s->p->left) {
					s = s->p;
					if (s->left == T->nil) { printf("ERR!!!\n"); }
					_rbt_right_rotate(T, s);
				}
				s->p->color = BLACK;
				s->p->p->color = RED;
				if (s->p->p->right == T->nil) { printf("ERR!!!\n"); }
				_rbt_left_rotate(T, s->p->p);
			}
		}
	}
	T->root->color = BLACK;
}

int rbt_insert(TREE* T, ELEMENT d) {
	NODE* p = NULL;
	if (_rbt_search(T, d, &p)) { return 0; }
	NODE* s = (NODE*) malloc(sizeof(NODE));
	ELEMENT_COPY(s->data, d);
	s->color = RED; s->left = s->right = T->nil; s->p = p;
	if (p == T->nil) {
		T->root = s;
	} else if (LT(d, p->data)) {
		p->left = s;
	} else {
		p->right = s;
	}
	_rbt_insert_fixup(T, s);
	return 1;
}

void _rbt_transplant(TREE* T, NODE* u, NODE* v) {
	if (u->p == T->nil) {
		T->root = v;
	} else if (u == u->p->left) {
		u->p->left = v;
	} else {
		u->p->right = v;
	}
	v->p = u->p;
}

void _rbt_delete_fixup(TREE* T, NODE* x) {
	while (x != T->root && x->color == BLACK) {
		if (x == x->p->left) {
			NODE* w = x->p->right;
			if (w->color == RED) {
				w->color = BLACK;
				x->p->color = RED;
				_rbt_left_rotate(T, x->p);
				w = x->p->right;
			}
			if (w->left->color == BLACK && w->right->color == BLACK) {
				w->color = RED;
				x = x->p;
			} else {
				if (w->right->color == BLACK) {
					w->color = RED;
					w->left->color = BLACK;
					_rbt_right_rotate(T, w);
					w = x->p->right;
				}
				w->color = x->p->color;
				x->p->color = BLACK;
				w->right->color = BLACK;
				_rbt_left_rotate(T, x->p);
				x = T->root;
			}
		} else { // x == x->p->right
			NODE* w = x->p->left;
			if (w->color == RED) {
				w->color = BLACK;
				x->p->color = RED;
				_rbt_right_rotate(T, x->p);
				w = x->p->left;
			}
			if (w->left->color == BLACK && w->right->color == BLACK) {
				w->color = RED;
				x = x->p;
			} else {
				if (w->left->color == BLACK) {
					w->color = RED;
					w->right->color = BLACK;
					_rbt_left_rotate(T, w);
					w = x->p->left;
				}
				w->color = x->p->color;
				x->p->color = BLACK;
				w->left->color = BLACK;
				_rbt_right_rotate(T, x->p);
				x = T->root;
			}
		}
	}
	x->color = BLACK;
}

NODE* _rbt_tree_minimum(TREE* T, NODE* p) {
	NODE* r = p;
	while (r->left != T->nil) {
		r = r->left;
	}
	return r;
}

int rbt_delete(TREE* T, ELEMENT d) {
	NODE* p = NULL;
	if (!_rbt_search(T, d, &p)) { return 0; }
	NODE* y = p; NODE* x = NULL;
	COLOR y_original_color = y->color;
	if (p->left == T->nil) {
		x = p->right;
		_rbt_transplant(T, p, p->right);
	} else if (p->right == T->nil) {
		x = p->left;
		_rbt_transplant(T, p, p->left);
	} else {
		y = _rbt_tree_minimum(T, p->right);
		y_original_color = y->color;
		x = y->right;
		if (y->p == p) {
			x->p = y;	// if the x == T->nil, set x->p = y;
		} else {
			_rbt_transplant(T, y, y->right);
			y->right = p->right;
			y->right->p = y;
		}
		_rbt_transplant(T, p, y);
		y->left = p->left;
		y->left->p = y;
		y->color = p->color;
	}

	if (y_original_color == BLACK) {
		_rbt_delete_fixup(T, x);
	}
	free(p);
	return 1;
}

/////////////////////////////// TEST CASE //////////////////////////////////////

void printout_node(NODE* p) { printf("%d ", p->data); }

/**
* @brief test case for single insert, search and delete method.
*/
void testcase_for_single() {
	TREE* T = rbt_init();

	printf("rbt_insert(T, 32) = %d\n", rbt_insert(T, 32));

	printf("rbt_search(T, 32) = %d\n", rbt_search(T, 32));

	printf("rbt_delete(T, 32) = %d\n", rbt_delete(T, 32));

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
	TREE* T = rbt_init();
	int i = 0, n = 100;
	int _t = 0;
	srand(time(NULL));
	for (i = 0; i < n; i ++) {
		_t = rand() % n;
		printf("rbt_insert(T, %d) = %d\n", _t, rbt_insert(T, _t));
	}
	for (i = 0; i < n; i ++) {
		_t = rand() % n;
		printf("rbt_search(T, %d) = %d\n", _t, rbt_search(T, _t));
	}
	printf("rbt_traveral: ");
	rbt_traveral(T, printout_node);
	printf("\n");
	for (i = 0; i < n; i ++) {
		_t = rand() % n;
		printf("rbt_delete(T, %d) = %d\n", _t, rbt_delete(T, _t));
	}
	printf("rbt_traveral: ");
	rbt_traveral(T, printout_node);
	printf("\n");
	rbt_destory(T);
}

int main(int argc, const char *argv[])
{
	testcase_for_single();
	testcase_for_random();

	return 0;
}
