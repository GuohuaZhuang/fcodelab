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
* @file b_tree.c
* @brief B tree implement in C.
* @author firstboy0513
* @version 0.0.1
* @date 2014-02-01
*/
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Red-Black tree public method extern to use or private be invoke.
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
* @brief Red-Black tree node struct, with data also named as key, color, and 3
* point which is parent node point, left child and right child node point.
*/
typedef struct _NODE {
	ELEMENT data;
	struct _NODE* p;
	struct _NODE* left;
	struct _NODE* right;
} NODE;

/**
* @brief Red-Black tree struct with nil node and root point.
*/
typedef struct _TREE {
	struct _NODE* root;
	struct _NODE* nil;
} TREE;

int main(int argc, const char *argv[])
{
	return 0;
}
