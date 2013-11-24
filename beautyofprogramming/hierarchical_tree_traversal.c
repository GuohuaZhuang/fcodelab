#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct Node {
	int data;
	struct Node* lChild;
	struct Node* rChild;
} Node;

/// int InsertNode(Node* root, int data) {
/// 	if (data < 0) {
/// 	}
/// }
/// 
/// int DestoryTree(Node* root) {
/// 	Node* p = root;
/// 	while (p
/// }

Node* GenerateNewNode(int data) {
	Node* pNode = (Node*) malloc(sizeof(Node));
	pNode->data = data;
	pNode->lChild = NULL;
	pNode->rChild = NULL;
	return pNode;
}

Node* GenerateTestTree(void) {
	Node* root = GenerateNewNode(1);
	root->lChild = GenerateNewNode(2);
	root->lChild->lChild = GenerateNewNode(4);
	root->lChild->rChild = GenerateNewNode(5);
	root->lChild->rChild->lChild = GenerateNewNode(7);
	root->lChild->rChild->rChild = GenerateNewNode(8);
	root->rChild = GenerateNewNode(3);
	root->rChild->rChild = GenerateNewNode(6);
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

int main(int argc, const char *argv[])
{
	Node* root = GenerateTestTree();
	// PrintNodeAtLevel(root, 3);
	PrintNodeAllLevel(root);
	printf("\n");
	return 0;
}
