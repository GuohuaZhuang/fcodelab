#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief B+ tree public method extern to use or private be invoke.
*/
#define PUBLIC
#define PRIVATE static

#define NIL		NULL

#define ORDER_B				4
#define FULL_KEY_COUNT 		(ORDER_B-1)
#define FULL_CHILD_COUNT 	(ORDER_B)

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
	struct _NODE* leftmost;
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
/// PRIVATE SEARCH_RET _bptree_search(NODE* x, ELEMENT k);
/// PRIVATE void _bptree_split_child(NODE* x, int i);
/// PRIVATE void _bptree_merge_child(NODE* x, int i);
/// PRIVATE void _bptree_insert_nonfull(NODE* x, ELEMENT k);
/// PRIVATE void _bptree_delete_balance(NODE* x, int i);
/// PRIVATE void _bptree_delete(NODE* x, ELEMENT k);
/// PRIVATE void _bptree_traveral(NODE* x, void function(ELEMENT));

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
* @brief B+ tree traveral method.
*
* @param T B+ tree.
* @param function traveral a element callback function.
*/
PUBLIC void bptree_traveral(TREE* T, void function(ELEMENT)) {
	_bptree_traveral(T->leftmost, function);
}

/////////////////////////////// TEST CASE //////////////////////////////////////

void printout_node(ELEMENT k) { printf("%d ", k); }

/**
* @brief test case for single insert, search and delete method.
*/
void testcase_for_single() {
/// 	TREE* T = bptree_create();
/// 
/// 	printf("bptree_insert(T, 32) = %d\n", bptree_insert(T, 32));
/// 
/// 	printf("bptree_search(T, 32) = %d\n", bptree_search(T, 32));
/// 
/// 	printf("bptree_delete(T, 32) = %d\n", bptree_delete(T, 32));
/// 
/// 	printf("bptree_search(T, 32) = %d\n", bptree_search(T, 32));
/// 
/// 	printf("bptree_traveral: ");
/// 	bptree_traveral(T, printout_node);
/// 	printf("\n");
/// 
/// 	bptree_destory(T);
}

#include <time.h>

/**
* @brief test case for random number insert, search and delete.
*/
void testcase_for_random() {
/// 	TREE* T = bptree_create();
/// 	int i = 0, n = 100;
/// 	int _t = 0;
/// 	srand(time(NULL));
/// 	for (i = 0; i < n; i ++) {
/// 		_t = rand() % n;
/// 		printf("bptree_insert(T, %d) = %d\n", _t, bptree_insert(T, _t));
/// 	}
/// 	for (i = 0; i < n; i ++) {
/// 		_t = rand() % n;
/// 		printf("bptree_search(T, %d) = %d\n", _t, bptree_search(T, _t));
/// 	}
/// 
/// 	printf("bptree_traveral: ");
/// 	bptree_traveral(T, printout_node);
/// 	printf("\n");
/// 
/// 	for (i = 0; i < n; i ++) {
/// 		_t = rand() % n;
/// 		printf("bptree_delete(T, %d) = %d\n", _t, bptree_delete(T, _t));
/// 	}
/// 
/// 	printf("bptree_traveral: ");
/// 	bptree_traveral(T, printout_node);
/// 	printf("\n");
/// 
/// 	bptree_destory(T);
}

int main(int argc, const char *argv[])
{
	testcase_for_single();
	testcase_for_random();

	return 0;
}
