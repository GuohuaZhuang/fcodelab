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
* @author firstboy0513
* @version 0.0.1
* @date 2014-01-26
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
* @brief Element type.
*/
typedef int ELEMENT;

typedef struct _NODE {
	ELEMENT data;
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
	T->root = NULL;
	T->nil = (NODE*) malloc(sizeof(NODE));
	memset(T->nil, 0, sizeof(NODE));
	return T;
}

void rbt_destory(TREE* T) {
	// node_destory(T->root);
	free(T->nil);
	free(T);
}

int main(int argc, const char *argv[])
{
	return 0;
}
