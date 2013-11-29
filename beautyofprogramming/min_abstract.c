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
* @file min_abstract.c
* @brief Get minimum abstract.
* @author firstboy0513
* @version 0.0.1
* @date 2013-11-28
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
* @brief Element, can be a character in test code, or an English word or 
* a Chinese word in the real world. We assume it use not more than 4 length, 
* so use data[5] array to represent it.
*/
typedef struct _ELE {
	char data[5];
} ELE;

/**
* @brief result to find the abstract and current scan window position.
*/
typedef struct _RET {
	int lpos; // left scan position
	int lpos_q; // left scan position query word index
	int rpos; // right scan position
	int rpos_q; // right scan position query word index
	int abs_len; // current minimum abstract length
	int abs_start; // minimum abstract start position
	int abs_end; // minimum abstract end position (Note include this position)
} RET;

/**
* @brief init the RET.
*
* @param pret input RET.
*/
void init_ret(RET* pret) {
	pret->lpos = -1;
	pret->lpos_q = -1;
	pret->rpos = -1;
	pret->rpos_q = -1;
	pret->abs_len = -1;
	pret->abs_start = -1;
	pret->abs_end = -1;
}

/**
* @brief judge Element A and Element B whether are equal or not.
*
* @param a input Element A.
* @param b input Element B.
*
* @return return 1 if equal, otherwise return 0 means not equal.
*/
int is_equal(ELE a, ELE b) {
	return (!strcmp(a.data, b.data));
}

/**
* @brief Set current scan window position.
*
* @param qindex input query keyword index.
* @param index input scan hit word position.
* @param pret output RET result.
*/
void set_ret_pos(int qindex, int index, RET* pret) {
	if (pret->lpos < 0 || index < pret->lpos) {
		pret->lpos = index;
		pret->lpos_q = qindex;
	}
	if (pret->rpos < 0 || index > pret->rpos) {
		pret->rpos = index;
		pret->rpos_q = qindex;
	}
}

/**
* @brief Calculate current abstract window length and update minimum abstract
* length if necessary.
*
* @param pret
*/
void set_ret_len(RET* pret) {
	int curr_len = pret->rpos - pret->lpos + 1;
	if (pret->abs_len < 0 || curr_len < pret->abs_len) {
		pret->abs_len = curr_len;
		pret->abs_start = pret->lpos;
		pret->abs_end = pret->rpos;
	}
}

/**
* @brief Set current minimum position in P array as the window left position.
*
* @param pret output RET result.
* @param P input to scan P array.
* @param plen P array length.
*/
void set_ret_pos_by_p(RET* pret, int* P, int plen) {
	int i = 0;
	for (i = 0; i < plen; i ++) {
		if (pret->lpos < 0 || P[i] < pret->lpos) {
			pret->lpos = P[i];
			pret->lpos_q = i;
		}
	}
}

/**
* @brief Give a query word index i, to find equal element in S string array 
* and set current position in RET result and update the query index array P.
*
* @param i a query word index.
* @param pj current travesal element index in S string array.
* @param len_s S length.
* @param S[] S string array.
* @param Q[] query word array.
* @param pret RET result point.
* @param P record query index.
*/
void find_equal_to_set_pos(int i, int* pj, int len_s, ELE S[], ELE Q[], 
	RET* pret, int* P)
{
    for ( ; (*pj) < len_s; (*pj) ++) {
		if (is_equal(Q[i], S[(*pj)])) {
			P[i] = (*pj);
			set_ret_pos(i, (*pj), pret);
			break;
		}
	}
}

/**
* @brief Find minimum abstract.
* I call it walking legs minimum abstract algorithm, contrast as the earthworm minimum
* abstract algorithm (it implement in the correspond cpp source code, may be 
* named min_abstract.cpp).
* It use O(N*M) time complexity.
*
* @param S[] scan string word array.
* @param len_s scan string length.
* @param Q[] query keyword array.
* @param len_q query keyword length.
*
* @return return include getted minimum abstract information RET struct.
*/
RET find_min_abstract(ELE S[], int len_s, ELE Q[], int len_q) {
	RET ret;
	init_ret(&ret);
    if (!S || len_s <= 0 || !Q || len_q <= 0) {
        printf("[ERR] -- find_min_abstract input invalid args.\n");
        return ret;
    }
	
	int* P = (int*)malloc(sizeof(int)*len_q);
	int i = 0, j = 0;
	while (i < len_q) { P[i] = -1; i ++; }
	for (i = 0; i < len_q; i ++) {
	    // Find first p
		j = 0;
		find_equal_to_set_pos(i, &j, len_s, S, Q, &ret, P);
		if (P[i] < 0) {
			printf("[ERR] -- find_min_abstract invalid not find Q in S.\n");
			goto OK_RET;
		}
	}
	while (1) {
		set_ret_len(&ret);
		j = ret.lpos + 1;
		ret.lpos = -1;
		set_ret_pos_by_p(&ret, P, len_q);
		// Find min abstract
		P[ret.lpos_q] = -1;
		find_equal_to_set_pos(ret.lpos_q, &j, len_s, S, Q, &ret, P);
		if (j >= len_s || P[ret.lpos_q] < 0) {
			goto OK_RET;
		}
	}
OK_RET:
	free(P);
	return ret;
}

/**
* @brief Print abstract information.
*
* @param S[] scan string word array.
* @param ret include minimum abstract RET struct.
*/
void print_abstract(ELE S[], RET ret) {
	int i = 0;
	printf("[ABSTRACT]: ");
	for (i = ret.abs_start; i <= ret.abs_end; i ++) {
		if (i != ret.abs_start) {
			printf(", ");
		}
		printf("\"%s\"", S[i].data);
	}
	printf("\n");
}

int main(int argc, const char *argv[])
{
	// init test string array and query word array
	ELE S[] = {{"w0"}, {"w1"}, {"w2"}, {"w3"}, {"q0"}, {"w4"}, {"w5"}, {"q1"}, 
		{"w6"}, {"w7"}, {"w8"}, {"q0"}, {"w9"}, {"q1"}};
	int len_s = sizeof(S)/sizeof(S[0]);
	ELE Q[] = {{"q0"}, {"q1"}};
	int len_q = sizeof(Q)/sizeof(Q[0]);

	// find minimum abstract
	RET ret = find_min_abstract(S, len_s, Q, len_q);
	
	// show RET result
	printf("RET lpos = %d\n", ret.lpos);
	printf("RET rpos = %d\n", ret.rpos);
	printf("RET abs_len = %d\n", ret.abs_len);
	printf("RET abs_start = %d\n", ret.abs_start);
	printf("RET abs_end = %d\n", ret.abs_end);
	print_abstract(S, ret);

	return 0;
}
