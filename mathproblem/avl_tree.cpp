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

int main(int argc, const char *argv[])
{
	BSTTree T = NULL;
	bool taller = 0;
	printf("insert ret = %d\n", InsertAVL(T, 32, taller));
	printf("insert ret = %d\n", InsertAVL(T, 31, taller));
	printf("insert ret = %d\n", InsertAVL(T, 32, taller));
	printf("search ret = %d\n", SearchAVL(T, 32));
	printf("search ret = %d\n", SearchAVL(T, 33));

	return 0;
}
