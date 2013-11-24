#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct Node {
	int data;
	struct Node* lChild;
	struct Node* rChild;
} Node;

Node* GenerateNewNode(int data) {
	Node* pNode = (Node*) malloc(sizeof(Node));
	pNode->data = data;
	pNode->lChild = NULL;
	pNode->rChild = NULL;
	return pNode;
}

void InsertNode(Node* root, int data, int ldata, int rdata) {
	if (NULL == root) {
		return;
	}
	if (root->data == data) {
		if (ldata > 0) root->lChild = GenerateNewNode(ldata);
		if (rdata > 0) root->rChild = GenerateNewNode(rdata);
		return;
	}
	InsertNode(root->lChild, data, ldata, rdata);
	InsertNode(root->rChild, data, ldata, rdata);
}

void DestoryTree(Node* root) {
	if (NULL == root) return;
	DestoryTree(root->lChild);
	DestoryTree(root->rChild);
	free(root);
}

Node* GenerateTestTree(void) {
	Node* root = GenerateNewNode(1);
	InsertNode(root, 1, 2, 3);
	InsertNode(root, 2, 4, 5);
	InsertNode(root, 3, -1, 6);
	InsertNode(root, 5, 7, 8);
	return root;
}

int PrintNodeAtLevel(Node* root, int level) {
	if (!root || level < 0) {
		return 0;
	}
	if (0 == level) {
		printf("%d  ", root->data);
		return 1;
	}
	return PrintNodeAtLevel(root->lChild, level - 1) 
		+ PrintNodeAtLevel(root->rChild, level - 1);
}

int PrintNodeAllLevel(Node* root) {
	int level = 0;
	while (PrintNodeAtLevel(root, level++) > 0) {
		printf("\n");
	}
	return 1;
}

int main(int argc, const char *argv[]) {

	Node* root = GenerateTestTree();
	
	PrintNodeAtLevel(root, 3);
	// PrintNodeAllLevel(root);
	printf("\n");

	DestoryTree(root);
	return 0;
}
