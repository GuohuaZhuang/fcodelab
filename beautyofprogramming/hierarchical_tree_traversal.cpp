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
#include <vector>
#include <queue>
#include <deque>

using std::vector;
using std::queue;
using std::deque;

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
*/
void PrintNodeAllLevel(Node* root) {
	int level = 0;
	while (PrintNodeAtLevel(root, level++) > 0) {
		printf("\n");
	}
}

/**
* @brief Print tree node data at all level from root 0 level.
* And print align is left to right using vector.
*
* @param root tree level.
*/
void PrintNodeAllLevel_UseVector(Node* root) {
	if (NULL == root) {
		return;
	}

	vector<Node*> vec;
	vec.push_back(root);
	int cur = 0;
	int last = 1;
	while (cur < (int)vec.size()) {
		last = vec.size();
		while (cur < last) {
			printf("%d  ", vec[cur]->data);
			if (NULL != vec[cur]->lChild) vec.push_back(vec[cur]->lChild);
			if (NULL != vec[cur]->rChild) vec.push_back(vec[cur]->rChild);
			cur ++;
		}
		printf("\n");
	}
}

/**
* @brief Print tree node data at all level from root 0 level.
* And print align is left to right using queue but no wrap is output.
*
* @param root tree level.
*/
void PrintNodeAllLevel_NoWrap_UseQueue(Node* root) {
	if (NULL == root) {
		return ;
	}
	queue<Node*> que;
	que.push(root);
	while (!que.empty()) {
		Node* cur = que.front(); que.pop();
		printf("%d  ", cur->data);
		if (NULL != cur->lChild) que.push(cur->lChild);
		if (NULL != cur->rChild) que.push(cur->rChild);
	}
}

/**
* @brief Print tree node data at all level from root 0 level.
* And print align is left to right using two queue to swap and record the wrap.
*
* @param root tree level.
*/
void PrintNodeAllLevel_HasWrap_UseQueue(Node* root) {
	if (NULL == root) {
		return ;
	}
	deque<Node*> Q1, Q2;
	Q1.push_back(root);
	while (!Q1.empty()) {
		while (!Q1.empty()) {
			Node* cur = Q1.front(); Q1.pop_front();
			printf("%d  ", cur->data);
			if (NULL != cur->lChild) Q2.push_back(cur->lChild);
			if (NULL != cur->rChild) Q2.push_back(cur->rChild);
		}
		printf("\n");
		Q1.swap(Q2);
	}
}

/**
* @brief Print tree node data at all level from root 0 level.
* And print align is left to right using one que and insert one NULL to record
* wrap.
*
* @param root tree level.
*/
void PrintNodeAllLevel_HasWrap_OneCycle_UseQueue(Node* root) {
	if (NULL == root) {
		return ;
	}
	queue<Node*> que;
	que.push(root);
	que.push(NULL);
	while (!que.empty()) {
		Node* cur = que.front(); que.pop();
		if (NULL != cur) {
			printf("%d  ", cur->data);
			if (NULL != cur->lChild) que.push(cur->lChild);
			if (NULL != cur->rChild) que.push(cur->rChild);
		} else if (!que.empty()) {
			printf("\n");
			que.push(NULL);
		}
	}
}

/**
* @brief Print tree node data at all level from max level to root.
* And print align is left to right using one vector and insert one NULL to 
* record wrap.
*
* @param root
*/
void PrintNodeAllLevel_Desc_LeftRight_UseVector(Node* root) {
	if (NULL == root) {
		return ;
	}
	vector<Node*> vec;
	vec.push_back(root);
	int cur = 0, last = 1;
	while (cur < (int)vec.size()) {
		vec.push_back(NULL);
		last = vec.size();
		while (cur < last) {
			if (vec[cur]->rChild) vec.push_back(vec[cur]->rChild);
			if (vec[cur]->lChild) vec.push_back(vec[cur]->lChild);
			cur ++;
			if (vec[cur] == NULL) {
				cur ++;
			}
		}
	}
	cur -= 2; // back a NULL in tail
	while (cur >= 0) {
		if (vec[cur] != NULL ) {
			printf("%d  ", vec[cur]->data);
		} else {
			printf("\n");
		}
		cur --;
	}
	printf("\n");
}

/**
* @brief Print tree node data at all level from max level to root.
* And print align is right to left using one vector and insert one NULL to 
* record wrap.
*
* @param root
*/
void PrintNodeAllLevel_Desc_RightLeft_UseVector(Node* root) {
	if (NULL == root) {
		return ;
	}
	vector<Node*> vec;
	vec.push_back(root);
	int cur = 0, last = 1;
	while (cur < (int)vec.size()) {
		vec.push_back(NULL);
		last = vec.size();
		while (cur < last) {
			if (vec[cur]->lChild) vec.push_back(vec[cur]->lChild);
			if (vec[cur]->rChild) vec.push_back(vec[cur]->rChild);
			cur ++;
			if (vec[cur] == NULL) {
				cur ++;
			}
		}
	}
	cur -= 2; // back a NULL in tail
	while (cur >= 0) {
		if (vec[cur] != NULL ) {
			printf("%d  ", vec[cur]->data);
		} else {
			printf("\n");
		}
		cur --;
	}
	printf("\n");
}

int main(int argc, const char *argv[]) {

	Node* root = GenerateTestTree();
	
	// PrintNodeAtLevel(root, 3);
	// PrintNodeAllLevel(root);
	// PrintNodeAllLevel_UseVector(root);
	// PrintNodeAllLevel_NoWrap_UseQueue(root);
	// PrintNodeAllLevel_HasWrap_UseQueue(root);
	// PrintNodeAllLevel_HasWrap_OneCycle_UseQueue(root);
	// PrintNodeAllLevel_Desc_Lef to lefttRight_UseVector(root);
	PrintNodeAllLevel_Desc_RightLeft_UseVector(root);
	printf("\n");

	DestoryTree(root);
	return 0;
}
