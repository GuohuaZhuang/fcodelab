/* Copyright (C) 
* 2013 - firstboy0513
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
* @file hierarchical_tree_traversal.c
* @brief hierarchical_tree_traversal
* @author firstboy0513
* @version 0.0.1
* @date 2013-11-24
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct Node {
	int data;
	struct Node* lChild;
	struct Node* rChild;
} Node;

/**
* @brief Generate a new node of malloc memory and initialize node value.
*
* @param data node data value.
*
* @return node point.
*/
Node* GenerateNewNode(int data) {
	Node* pNode = (Node*) malloc(sizeof(Node));
	pNode->data = data;
	pNode->lChild = NULL;
	pNode->rChild = NULL;
	return pNode;
}

/**
* @brief Insert node and its left and right node.
*
* @param root tree root.
* @param data current node data.
* @param ldata current node left node data.
* @param rdata current node right node data.
*/
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

/**
* @brief Destory tree.
*
* @param root tree root.
*/
void DestoryTree(Node* root) {
	if (NULL == root) return;
	DestoryTree(root->lChild);
	DestoryTree(root->rChild);
	free(root);
}

/**
* @brief Generate test tree.
* the test tree is a binary tree like the following:
*           1
*        2     3
*      4   5     6
*         7 8      
*
* @return tree root.
*/
Node* GenerateTestTree(void) {
	Node* root = GenerateNewNode(1);
	InsertNode(root, 1, 2, 3);
	InsertNode(root, 2, 4, 5);
	InsertNode(root, 3, -1, 6);
	InsertNode(root, 5, 7, 8);
	return root;
}

/**
* @brief Print tree node at given level.
*
* @param root tree root.
* @param level given level.
*
* @return return 0 if root is null or level is negative, return 1 if level is 
* reach.
*/
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

/**
* @brief Print tree node data at all level from root 0 level.
*
* @param root tree level.
*
* @return return 1.
*/
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
