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
* @file avl_tree.c
* @brief AVL tree, also named as balance binary search/sort tree, implement in 
* C program. The insert and delete will rebalance, it is a bit complex in it.
* reference:
* 	http://en.wikipedia.org/wiki/AVL_tree
* 	http://zh.wikipedia.org/wiki/AVL%E6%A0%91
* Some code in the net and not correct, there are lose the delete method or 
* wrong implement, and some are more complex or not good efficiency in the 
* delete method, and I download some code in the wikipedia last links, but there 
* implement is also complex, so I code it myself, I suggest you also to write 
* your own AVL tree yourself, it will let you more clear to know its insert and
* delete method to do. Those code implement the AVL tree in C is just reference
* to use, I test it in 10000000 random integer numbers just use 37 more seconds 
* include insert, search and delete operatorations.
*
* @author firstboy0513
* @version 0.0.1
* @date 2014-01-20
*/
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief AVL tree public method extern to use or private be invoke.
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
* @brief AVL node struct, with data, left child point and right child point, and
* also a balance factor named as bf may equal LH, EH or RH.
*/
typedef struct _NODE {
	ELEMENT data;
	int		bf;
	struct _NODE* lchild;
	struct _NODE* rchild;
} NODE, *TREE;

/**
* @brief balance factor enum.
*/
typedef enum {LH = 1, EH = 0, RH = -1} BF;

/**
* @brief AVL tree initialize method.
*
* @return return AVL tree point.
*/
PUBLIC TREE avl_init(void) {
	return NULL;
}

/**
* @brief AVL tree destory method.
*
* @param T tree point.
*/
PUBLIC void avl_destory(TREE T) {
	if (!T) { return; }
	if (T->lchild) { avl_destory(T->lchild); }
	if (T->rchild) { avl_destory(T->rchild); }
	free(T);
}

/**
* @brief AVL tree search method. With given one element and find it.
*
* @param T tree point.
* @param d element to find.
*
* @return return 1 means find it in the AVL tree, otherwise return 0 means not
* find it in the AVL tree.
*/
PUBLIC int avl_search(TREE T, ELEMENT d) {
	if (!T) {
		return 0;
	} else if (EQ(d, T->data)) {
		return 1;
	} else if (LT(d, T->data)) {
		return avl_search(T->lchild, d);
	} else {
		return avl_search(T->rchild, d);
	}
}

/**
* @brief AVL tree new node.
*
* @param d element in node.
*
* @return return malloc node point.
*/
PRIVATE NODE* _avl_newnode(ELEMENT d) {
	NODE* P = (NODE*) malloc(sizeof(NODE));
	ELEMENT_COPY(P->data, d);
	P->bf = EH;
	P->lchild = NULL;
	P->rchild = NULL;
	return P;
}

/**
* @brief AVL tree also know as right-right rotate when insert new node in the
* right right subtree, and update the balance factor where node subtree is 
* change.
*
* @param pT tree quote point.
*/
PRIVATE void _avl_rr_rotate(TREE* pT) {
	NODE* lchild = (*pT)->lchild;
	// rotate
	(*pT)->lchild = lchild->rchild;
	lchild->rchild = (*pT);
	(*pT) = lchild;
	// update balance factor
	(*pT)->bf = EH; (*pT)->rchild->bf = EH;
}

/**
* @brief AVL tree also know as left-right rotate when insert new node in the
* left right subtree, and update the balance factor where node subtree is 
* change.
*
* @param pT tree quote point.
*/
PRIVATE void _avl_lr_rotate(TREE* pT) {
	NODE* lchild = (*pT)->lchild;
	NODE* lrchild = lchild->rchild;
	// rotate
	(*pT)->lchild = lrchild->rchild;
	lchild->rchild = lrchild->lchild;
	lrchild->lchild = lchild; lrchild->rchild = (*pT);
	(*pT) = lrchild;
	// update balance factor
	if (lrchild->bf == LH) {
		(*pT)->bf = EH; (*pT)->lchild->bf = EH; (*pT)->rchild->bf = RH;
	} else if (lrchild->bf == EH) { // this case may happen as h-3==0 situation
		(*pT)->bf = EH; (*pT)->lchild->bf = EH; (*pT)->rchild->bf = EH;
	} else { // lrchild->bf == RH
		(*pT)->bf = EH; (*pT)->lchild->bf = LH; (*pT)->rchild->bf = EH;
	}
}

/**
* @brief AVL tree also know as left-left rotate when insert new node in the left
* left subtree, and update the balance factor where node subtree is change.
*
* @param pT tree quote point.
*/
PRIVATE void _avl_ll_rotate(TREE* pT) {
	NODE* rchild = (*pT)->rchild;
	// rotate
	(*pT)->rchild = rchild->lchild;
	rchild->lchild = (*pT);
	(*pT) = rchild;
	// update balance factor
	(*pT)->bf = EH; (*pT)->lchild->bf = EH;
}

/**
* @brief AVL tree also know as right-left rotate when insert new node in the 
* right left subtree, and update the balance factor where node subtree is 
* change.
*
* @param pT tree quote point.
*/
PRIVATE void _avl_rl_rotate(TREE* pT) {
	NODE* rchild = (*pT)->rchild;
	NODE* rlchild = rchild->lchild;
	// rotate
	(*pT)->rchild = rlchild->lchild;
	rchild->lchild = rlchild->rchild;
	rlchild->lchild = (*pT); rlchild->rchild = rchild;
	(*pT) = rlchild;
	// update balance factor
	if (rlchild->bf == LH) {
		(*pT)->bf = EH; (*pT)->lchild->bf = EH; (*pT)->rchild->bf = RH;
	} else if (rlchild->bf == EH) { // this case may happen as h-3==0 situation
		(*pT)->bf = EH; (*pT)->lchild->bf = EH; (*pT)->rchild->bf = EH;
	} else { // rlchild->bf == RH
		(*pT)->bf = EH; (*pT)->lchild->bf = LH; (*pT)->rchild->bf = EH;
	}
}

/**
* @brief AVL tree rebalance when left subtree is change to be taller.
*
* @param pT tree quote point.
* @param ptaller output becomes taller tree or not.
*/
PRIVATE void _avl_left_taller_rebalance(TREE* pT, int* ptaller) {
	// assert ptaller not null and *ptaller == 1
	if ((*pT)->bf == LH) {
		NODE* lchild = (*pT)->lchild;
		if (lchild->bf == LH) { _avl_rr_rotate(pT); }
		else { _avl_lr_rotate(pT); }
		(*ptaller) = 0;
	} else if ((*pT)->bf == EH) {
		(*pT)->bf = LH; // (*ptaller) = 1; as input taller is 1, so not set it
	} else {
		(*pT)->bf = EH; (*ptaller) = 0;
	}
}

/**
* @brief AVL tree rebalance when right subtree is change to be taller.
*
* @param pT tree quote point.
* @param ptaller output becomes taller tree or not.
*/
PRIVATE void _avl_right_taller_rebalance(TREE* pT, int* ptaller) {
	// assert ptaller not null and *ptaller == 1
	if ((*pT)->bf == LH) {
		(*pT)->bf = EH; (*ptaller) = 0;
	} else if ((*pT)->bf == EH) {
		(*pT)->bf = RH; // (*ptaller) = 1; as input taller is 1, so not set it
	} else {
		NODE* rchild = (*pT)->rchild;
		if (rchild->bf == RH) { _avl_ll_rotate(pT); }
		else { _avl_rl_rotate(pT); }
		(*ptaller) = 0;
	}
}

/**
* @brief AVL tree insert method. With given one element, and find the place to 
* insert it.
*
* @param pT tree quote point.
* @param d element to insert.
* @param ptaller output taller or not.
*
* @return return 1 means insert success, otherwise return 0 means insert failed,
* or return -1 means input invalid error.
*/
PUBLIC int avl_insert(TREE* pT, ELEMENT d, int* ptaller) {
	if (!pT || !ptaller) {
		printf("[ERR] -- avl_insert input invalid!\n");
		return -1;
	}
	int ret = 0, lr = 0;
	if (!(*pT)) {
		(*pT) = _avl_newnode(d); (*ptaller) = 1;
		return 1;
	} else if (EQ(d, (*pT)->data)) {
		(*ptaller) = 0;
		return 0;
	} else if (LT(d, (*pT)->data)) {
		ret = avl_insert(&((*pT)->lchild), d, ptaller);
	} else {
		ret = avl_insert(&((*pT)->rchild), d, ptaller); lr = 1;
	}
	if (!ret || 0 == (*ptaller)) { return ret; }
	if (!lr) { _avl_left_taller_rebalance(pT, ptaller); }
	else { _avl_right_taller_rebalance(pT, ptaller); }
	return ret;
}

/**
* @brief AVL tree stack use for store father point in delete node method.
*/
typedef struct _AVLTREE_STACK {
	TREE* pT;
	struct _AVLTREE_STACK* next;
} AVLTREE_STACK;

/**
* @brief AVL tree stack push.
*
* @param pTS stack quote point.
* @param pT push father AVL tree quote point.
*/
void tree_stack_push(AVLTREE_STACK** pTS, TREE* pT) {
	if (!pTS) { return; }
	AVLTREE_STACK* _ts = (AVLTREE_STACK*) malloc(sizeof(AVLTREE_STACK));
	_ts->pT = pT; _ts->next = (*pTS);
	(*pTS) = _ts;
}
/**
* @brief AVL tree stack front father quote point.
*
* @param pTS stack quote point.
*
* @return first father quote point.
*/
TREE* tree_stack_front(AVLTREE_STACK** pTS) {
	if (!pTS) { return NULL; }
	return (*pTS)->pT;
}
/**
* @brief AVL tree stack pop.
*
* @param pTS stack quote point.
*
* @return pop from stack, and return the pop father quote point.
*/
TREE* tree_stack_pop(AVLTREE_STACK** pTS) {
	if (!pTS) { return NULL; }
	if (!(*pTS)) { return NULL; }
	AVLTREE_STACK* _ts = (*pTS);
	(*pTS) = (*pTS)->next;
	TREE* pT = _ts->pT;
	free(_ts);
	return pT;
}

/**
* @brief AVL tree rebalance when left subtree becomes lower height.
* In (*pT)->bf == RH situation, think about 3 situation to process:
* 	if ((*pT)-rchild->bf == EH) {
* 		// (*pT)->rchild's left/right child height all is h-2.
* 	} else if ((*pT)->rchild->bf == LH) {
* 		// (*pT)->rchild's left height is h-2, right height is h-3.
* 	} else { // (*pT)->rchild->bf == RH
* 		// (*pT)->rchild's left height is h-3, right height is h-2.
* 	}
* Draw the tree, then we will find its logic how to rebalance tree.
*
* @param pT tree quote point.
* @param plower output tree becomes lower for father or not.
*/
PRIVATE void _avl_left_lower_rebalance(TREE* pT, int* plower) {
	if ((*pT)->bf == LH) {
		(*pT)->bf = EH; (*plower) = 1;
	} else if ((*pT)->bf == EH) {
		(*pT)->bf = RH; (*plower) = 0;
	} else { // (*pT)->bf == RH
		NODE* rchild = (*pT)->rchild;
		if (rchild->bf == LH) {
			// rotate
			NODE* rlchild = rchild->lchild;
			(*pT)->rchild = rlchild->lchild;
			rchild->lchild = rlchild->rchild;
			rlchild->lchild = (*pT);
			rlchild->rchild = rchild;
			(*pT) = rlchild;
			// update balance factor
			if (rlchild->bf == LH) {
				(*pT)->bf = EH; (*pT)->lchild->bf = EH; (*pT)->rchild->bf = RH;
				(*plower) = 1;
			} else if (rlchild->bf == EH) {
				(*pT)->bf = EH; (*pT)->lchild->bf = EH; (*pT)->rchild->bf = EH;
				(*plower) = 1;
			} else { //  rlchild->bf == RH
				(*pT)->bf = EH; (*pT)->lchild->bf = LH; (*pT)->rchild->bf = EH;
				(*plower) = 1;
			}
		} else if (rchild->bf == EH) {
			// rotate
			(*pT)->rchild = rchild->lchild;
			rchild->lchild = (*pT);
			(*pT) = rchild;
			// update balance factor
			(*pT)->bf = LH; (*pT)->lchild->bf = RH;
			(*plower) = 0;
		} else { // rchild->bf == RH
			// rotate
			(*pT)->rchild = rchild->lchild;
			rchild->lchild = (*pT);
			(*pT) = rchild;
			// update balance factor
			(*pT)->bf = EH; (*pT)->lchild->bf = EH;
			(*plower) = 1;
		}
	}
}

/**
* @brief AVL tree rebalance when right subtree becomes lower height.
* Draw the tree, then we will find its logic how to rebalance tree, it is the 
* same logic corresponding with left subtree becomes lower height.
*
* @param pT tree quote point.
* @param plower output tree becomes lower for father or not.
*/
PRIVATE void _avl_right_lower_rebalance(TREE* pT, int* plower) {
	if ((*pT)->bf == RH) {
		(*pT)->bf = EH; (*plower) = 1;
	} else if ((*pT)->bf == EH) {
		(*pT)->bf = LH; (*plower) = 0;
	} else { // (*pT)->bf == LH
		NODE* lchild = (*pT)->lchild;
		if (lchild->bf == RH) {
			// rotate
			NODE* lrchild = lchild->rchild;
			(*pT)->lchild = lrchild->rchild;
			lchild->rchild = lrchild->lchild;
			lrchild->rchild = (*pT);
			lrchild->lchild = lchild;
			(*pT) = lrchild;
			// update balance factor
			if (lrchild->bf == RH) {
				(*pT)->bf = EH; (*pT)->lchild->bf = LH; (*pT)->rchild->bf = EH;
				(*plower) = 1;
			} else if (lrchild->bf == EH) {
				(*pT)->bf = EH; (*pT)->lchild->bf = EH; (*pT)->rchild->bf = EH;
				(*plower) = 1;
			} else { //  lrchild->bf == LH
				(*pT)->bf = EH; (*pT)->lchild->bf = EH; (*pT)->rchild->bf = RH;
				(*plower) = 1;
			}
		} else if (lchild->bf == EH) {
			// rotate
			(*pT)->lchild = lchild->rchild;
			lchild->rchild = (*pT);
			(*pT) = lchild;
			// update balance factor
			(*pT)->bf = RH; (*pT)->rchild->bf = LH;
			(*plower) = 0;
		} else { // lchild->bf == LH
			// rotate
			(*pT)->lchild = lchild->rchild;
			lchild->rchild = (*pT);
			(*pT) = lchild;
			// update balance factor
			(*pT)->bf = EH; (*pT)->rchild->bf = EH;
			(*plower) = 1;
		}
	}
}

/**
* @brief AVL tree delete one node. This method is to find its front data child,
* means its first left-right child to cover its data value, and replace to 
* remove the child node. The verbose analysis is the following.
* if pT has single child or no, then remove pT and attach its child to its 
* father, now his father bf --, and require rebalance it.
* elseif pT has two child, find the front data child, means its first left-right
* child, and copy the data in pT, and begin to remove this node named Y. As we 
* know Y has no child or single left child, so we can attach the child to Y's 
* father. And then require rebalance from Y's father to pT.
* May be the Y should split 2 situation:
* 	1. the Y is the pT's lchild.
* 	2. the Y is the pT's lchild's grand right son.
*
* @param pT tree quote point.
* @param plower output tree becomes lower or not.
*/
PRIVATE void _avl_deletenode(TREE* pT, int* plower) {
	if (NULL == (*pT)->lchild) {
		NODE* Y = (*pT); (*pT) = (*pT)->rchild; (*plower) = 1; free(Y);
	} else if (NULL == (*pT)->rchild) {
		NODE* Y = (*pT); (*pT) = (*pT)->lchild; (*plower) = 1; free(Y);
	} else { // (*pT)->lchild and (*pT)->rchild are all not null.
		NODE* Y = (*pT)->lchild;
		if (NULL == Y->rchild) {
			ELEMENT_COPY((*pT)->data, Y->data);
			(*pT)->lchild = Y->lchild; (*plower) = 1; free(Y);
			_avl_left_lower_rebalance(pT, plower);
		} else { // NULL != Y->rchild
			AVLTREE_STACK* TS = NULL;
			tree_stack_push(&TS, &((*pT)->lchild));
			while (Y->rchild) {
				if (Y->rchild->rchild) { tree_stack_push(&TS, &(Y->rchild)); }
				Y = Y->rchild;
			}
			/// copy data to pT node and delete Y node
			ELEMENT_COPY((*pT)->data, Y->data);
			TREE* YF = tree_stack_front(&TS);
			(*YF)->rchild = Y->lchild;
			(*plower) = 1; free(Y);
			// pop all father line and right lower rabalance
			while (NULL != (YF = tree_stack_pop(&TS))) {
				if (0 == (*plower)) { continue; }
				_avl_right_lower_rebalance(YF, plower);
			}
			// finally do left lower rebalance if lower from left subtree
			if (1 == (*plower)) { _avl_left_lower_rebalance(pT, plower); }
		}
	}
}

/**
* @brief AVL tree delete method. With given one element, then find it to delete
* the node of this element.
*
* @param pT tree quote point.
* @param d element to delete.
* @param plower output lower or not.
*
* @return return 1 means delete success, otherwise return 0 means delete failed,
* or return -1 means input invalid error.
*/
PUBLIC int avl_delete(TREE* pT, ELEMENT d, int* plower) {
	if (!pT || !plower) {
		printf("[ERR] -- avl_delete input invalid!\n");
		return -1;
	}
	int ret = 0, lr = 0;
	if (!(*pT)) {
		(*plower) = 0;
		return 0;
	} else if (EQ(d, (*pT)->data)) {
		_avl_deletenode(pT, plower); // (*plower) = 1; do not know the plower
		return 1;
	} else if (LT(d, (*pT)->data)) {
		ret = avl_delete(&((*pT)->lchild), d, plower);
	} else {
		ret = avl_delete(&((*pT)->rchild), d, plower); lr = 1;
	}
	if (!ret || 0 == (*plower)) { return ret; }
	if (!lr) { _avl_left_lower_rebalance(pT, plower); }
	else { _avl_right_lower_rebalance(pT, plower); }
	return ret;
}

/**
* @brief AVL tree traveral use left-middle-right order.
*
* @param T tree.
* @param function traveral every node function.
*/
PUBLIC void avl_traveral(TREE T, void function(NODE*)) {
	if (!T) { return; }
	if (T->lchild) { avl_traveral(T->lchild, function); }
	function(T);
	if (T->rchild) { avl_traveral(T->rchild, function); }
}

/////////////////////////////// TEST CASE //////////////////////////////////////

void printout_node(NODE* p) { printf("%d ", p->data); }

#include <time.h>

/**
* @brief test case for single number insert, search and delete.
*/
void testcase_for_single() {
	TREE T = avl_init();
	int taller = 0, lower = 0;

	printf("avl_insert(T, 32) = %d\n", avl_insert(&T, 32, &taller));

	printf("avl_search(T, 32) = %d\n", avl_search(T, 32));

	printf("avl_delete(T, 32) = %d\n", avl_delete(&T, 32, &lower));

	printf("avl_traveral: ");
	avl_traveral(T, printout_node);
	printf("\n");

	avl_destory(T);
}

/**
* @brief test case for random number insert, search and delete.
*/
void testcase_for_random() {
	TREE T = avl_init();
	int taller = 0, lower = 0;
	int i = 0, n = 100;
	int _t = 0;
	srand(time(NULL));
	for (i = 0; i < n; i ++) {
		_t = rand() % n;
		printf("avl_insert(&T, %d) = %d\n", _t, avl_insert(&T, _t, &taller));
	}
	for (i = 0; i < n; i ++) {
		_t = rand() % n;
		printf("avl_search(T, %d) = %d\n", _t, avl_search(T, _t));
	}
	printf("avl_traveral: ");
	avl_traveral(T, printout_node);
	printf("\n");
	for (i = 0; i < n; i ++) {
		_t = rand() % n;
		printf("avl_delete(&T, %d) = %d\n", _t, avl_delete(&T, _t, &lower));
	}
	printf("avl_traveral: ");
	avl_traveral(T, printout_node);
	printf("\n");
	avl_destory(T);
}

int main(int argc, const char *argv[])
{
	testcase_for_single();
	testcase_for_random();

	return 0;
}
