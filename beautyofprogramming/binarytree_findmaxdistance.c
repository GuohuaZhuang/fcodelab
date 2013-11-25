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
* @file binarytree_findmaxdistance.c
* @brief binary tree find max edge distance.
* @author firstboy0513
* @version 0.0.1
* @date 2013-11-26
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
*
* another tree:
*              1
*       2             3
*   4       5        
* 9   10  7   8
* 12      11
* 
* @return tree root.
*/
Node* GenerateTestTree(void) {
	Node* root = GenerateNewNode(1);
	// InsertNode(root, 1, 2, 3);
	// InsertNode(root, 2, 4, 5);
	// InsertNode(root, 3, -1, 6);
	// InsertNode(root, 5, 7, 8);
	InsertNode(root, 1, 2, 3);
	InsertNode(root, 2, 4, 5);
	InsertNode(root, 5, 7, 8);
	InsertNode(root, 4, 9, 10);
	InsertNode(root, 7, 11, -1);
	InsertNode(root, 9, 12, -1);
	return root;
}

/**
* @brief return note in binary tree max distance and max depth result.
*/
typedef struct _RESULT {
	int nMaxDistance;
	int nMaxDepth;
} RESULT;

#define MAX(a, b) ((a) > (b) ? (a) : (b))

/**
* @brief Find max edges in the tree.
*
* @param root binary tree root.
*
* @return max edges distance.
*/
RESULT FindMaxDistance(Node* root) {
	if (NULL == root) {
		RESULT ret = {0, -1};
		return ret;
	}
	RESULT retl = FindMaxDistance(root->lChild);
	RESULT retr = FindMaxDistance(root->rChild);
	int nMaxDepth = MAX(retl.nMaxDepth, retr.nMaxDepth) + 1;
	int nMaxDistance = MAX(
		MAX(retl.nMaxDistance, retr.nMaxDistance), 
		retl.nMaxDepth + retr.nMaxDepth + 2);
	RESULT ret = {nMaxDistance, nMaxDepth};
	return ret;
}

int main(int argc, const char *argv[])
{
	Node* root = GenerateTestTree();
	RESULT ret = FindMaxDistance(root);
	int nMaxDistance = (ret.nMaxDistance > 0) ? ret.nMaxDistance : 0;
	printf("Max distance is %d\n", nMaxDistance);
	DestoryTree(root);
	return 0;
}
