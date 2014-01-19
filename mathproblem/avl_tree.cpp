#include <stdio.h>
#include <stdlib.h>

#define EQ(a, b) (a == b)
#define LT(a, b) (a < b)
#define GT(a, b) (a > b)
typedef int ElemType;

typedef struct BSTNode {
	ElemType		data;		// node data
	int				bf;			// balance factor
	struct BSTNode* lchild;		// left child point
	struct BSTNode* rchild;		// right child point
} BSTNode, *BSTTree;

#define LH	+1 					// left higher
#define EH	 0					// left and right are equal height
#define RH	-1					// right higher

void R_Rotate(BSTTree &p) {
	BSTNode* lchild = p->lchild;
	p->lchild = lchild->rchild;
	lchild->rchild = p;
	p = lchild;
}

void L_Rotate(BSTTree &p) {
	BSTNode* rchild = p->rchild;
	p->rchild = rchild->lchild;
	rchild->lchild = p;
	p = rchild;
}

void LeftBalance(BSTTree &T) {
	BSTNode* lchild = T->lchild; BSTNode* rlchild = NULL;
	switch (lchild->bf) {
		case LH:
			T->bf = lchild->bf = EH;
			R_Rotate(T);
			break;
		case RH:
			rlchild = lchild->rchild;
			switch (rlchild->bf) {
				case LH: T->bf = RH; lchild->bf = EH; break;
				case EH: T->bf = EH; lchild->bf = EH; break;
				case RH: T->bf = EH; lchild->bf = LH; break;
			} // switch
			rlchild->bf = EH;
			L_Rotate(T->lchild);
			R_Rotate(T);
			break;
	}
}

void RightBalance(BSTTree &T) {
	BSTNode* rchild = T->rchild; BSTNode* lrchild = NULL;
	switch (rchild->bf) {
		case RH:
			T->bf = rchild->bf = EH;
			L_Rotate(T);
			break;
		case LH:
			lrchild = rchild->lchild;
			switch (lrchild->bf) {
				case LH: T->bf = EH; rchild->bf = RH; break;
				case EH: T->bf = EH; rchild->bf = EH; break;
				case RH: T->bf = LH; rchild->bf = EH; break;
			}
			lrchild->bf = EH;
			R_Rotate(T->rchild);
			L_Rotate(T);
			break;
	}
}

int SearchAVL(BSTTree &T, ElemType e) {
	if (!T) { return 0; }
	if (EQ(e, T->data)) { return 1; }
	else if (LT(e, T->data)) { return SearchAVL(T->lchild, e); }
	else { return SearchAVL(T->rchild, e); }
}

void TraversalAVL(BSTTree &T, void function(BSTNode*)) {
	if (!T) { return; }
	if (T->lchild) { TraversalAVL(T->lchild, function); }
	function(T);
	if (T->rchild) { TraversalAVL(T->rchild, function); }
}

typedef struct _POINT {
	BSTNode* p;       // point
	struct _POINT* next; // father
} POINT;

POINT* point_push(POINT* root, BSTNode* p) {
	POINT* new_point = (POINT*) calloc(1, sizeof(POINT));
	new_point->p = p; new_point->next = NULL;
	new_point->next = root;
	root = new_point;
	return root;
}

BSTNode* point_pop(POINT* root) {
	if (!root) { return NULL; }
	BSTNode* p = root->p;
	POINT* cursor = root;
	root = root->next;
	free(cursor);
	return p;
}

void Balance(BSTTree T) {
}

int DeleteAVL(BSTTree &T, ElemType e, bool &taller) {
	if (!T) { return 0; taller = false; }
	BSTNode* Z = NULL, *Y = NULL;
	POINT *ZP = NULL; // Z parent node point
	if (EQ(e, T->data)) {
		ZP = point_push(ZP, T);
		if (T->lchild != NULL && T->rchild != NULL) {
			Y = T->lchild;
			while (Y->rchild) { ZP = point_push(ZP, Y); Y = Y->rchild; Z = Y; }
			T->data = Y->data;
		} else { // has only one child or no child
			Z = (NULL != T->lchild ? T->lchild : T->rchild);
		}
		// remove Z node
		(Z->lchild != NULL) 
			? ZP = point_push(ZP, Z->lchild) : ZP = point_push(ZP, Z->rchild);
		free(Z); taller = true;
		ZP->p->bf --;
		if (ZP->p->bf == LH || ZP->p->bf == RH) { taller = false; }
		else if (ZP->p->bf == 2) {
			LeftBalance(ZP->p); if (ZP->p->bf == EH) { taller = true; }
		} else if (ZP->p->bf == -2) {
			RightBalance(ZP->p); if (ZP->p->bf == EH) { taller = true; }
		} else {
			taller = true;
		}
		if (ZP->p == T) { return 1; }
		BSTNode* _tmp = NULL;
		do {
			_tmp = point_pop(ZP);
			Balance(_tmp);
		} while (_tmp != T);
	} else if (LT(e, T->data)) {
		int ret = DeleteAVL(T->lchild, e, taller);
		if (!ret) { return 0; }
		if (!taller) { return 1; }
		/// balance one ...
	} else {
		int ret = DeleteAVL(T->rchild, e, taller);
		if (!ret) { return 0; }
		if (!taller) { return 1; }
		/// balance one ...
	}
	return 1;
}

int InsertAVL(BSTTree &T, ElemType e, bool &taller) {
	if (!T) {
		T = (BSTTree) malloc(sizeof(BSTNode));
		T->data = e; T->lchild = T->rchild = NULL; T->bf = EH; taller = true;
	} else {
		if (EQ(e, T->data)) {
			taller = false; return 0;
		} else if (LT(e, T->data)) {
			if (!InsertAVL(T->lchild, e, taller)) { return 0; }
			if (taller) {
				switch (T->bf) {
					case LH:
						LeftBalance(T); taller = false; break;
					case EH:
						T->bf = LH; taller = true; break;
					case RH:
						T->bf = EH; taller = false; break;
				} // switch
			} // if (taller)
		} else {
			if (!InsertAVL(T->rchild, e, taller)) { return 0; }
			if (taller) {
				switch (T->bf) {
					case LH:
						T->bf = EH; taller = false; break;
					case EH:
						T->bf = RH; taller = true; break;
					case RH:
						RightBalance(T); taller = false; break;
				} // switch
			} // if (taller)
		}
	}
	return 1;
}

void printout_avl_node(BSTNode* p) { printf("%d ", p->data); }

int main(int argc, const char *argv[])
{
	BSTTree T = NULL;
	bool taller = 0;
	printf("insert ret = %d\n", InsertAVL(T, 32, taller));
	printf("insert ret = %d\n", InsertAVL(T, 31, taller));
	printf("insert ret = %d\n", InsertAVL(T, 32, taller));
	printf("search ret = %d\n", SearchAVL(T, 32));
	printf("search ret = %d\n", SearchAVL(T, 33));

	printf("TraversalAVL:\n");
	TraversalAVL(T, printout_avl_node);
	printf("\n");

	return 0;
}
