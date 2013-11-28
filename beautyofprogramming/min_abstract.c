#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct _ELE {
	char data[5];
} ELE;

typedef struct _RET {
	int min_pos;
	int min_pos_q;
	int max_pos;
	int max_pos_q;
	int min_len;
	int min_len_start;
	int min_len_end;
} RET;

void init_ret(RET* pret) {
	pret->min_pos = -1;
	pret->min_pos_q = -1;
	pret->max_pos = -1;
	pret->max_pos_q = -1;
	pret->min_len = -1;
	pret->min_len_start = -1;
	pret->min_len_end = -1;
}

int is_equal(ELE a, ELE b) {
	return (!strcmp(a.data, b.data));
}

void set_ret_pos(int qindex, int index, RET* pret) {
	if (pret->min_pos < 0 || index < pret->min_pos) {
		pret->min_pos = index;
		pret->min_pos_q = qindex;
	}
	if (pret->max_pos < 0 || index > pret->max_pos) {
		pret->max_pos = index;
		pret->max_pos_q = qindex;
	}
}

void set_ret_len(RET* pret) {
	int curr_len = pret->max_pos - pret->min_pos + 1;
	if (pret->min_len < 0 || curr_len < pret->min_len) {
		pret->min_len = curr_len;
		pret->min_len_start = pret->min_pos;
		pret->min_len_end = pret->max_pos;
	}
}

void set_ret_pos_by_p(RET* pret, int* P, int plen) {
	int i = 0;
	for (i = 0; i < plen; i ++) {
		if (pret->min_pos < 0 || P[i] < pret->min_pos) {
			pret->min_pos = P[i];
			pret->min_pos_q = i;
		}
	}
}

RET find_min_abstract(ELE S[], int len_s, ELE Q[], int len_q) {
	RET ret;
	init_ret(&ret);

	// Find first p
	int* P = (int*)malloc(sizeof(int)*len_q);
	int i = 0, j = 0;
	while (i < len_q) { P[i] = -1; i ++; }
	for (i = 0; i < len_q; i ++) {
		ELE q = Q[i];
		for (j = 0; j < len_s; j ++) {
			if (is_equal(q, S[j])) {
				P[i] = j;
				set_ret_pos(i, j, &ret);
				break;
			}
		}
		if (P[i] < 0) {
			printf("[ERR] -- find_min_abstract invalid input not find Q in"
				" S.\n");
			return ret;
		}
	}
	while (1) {
		set_ret_len(&ret);
		j = ret.min_pos + 1;
		ret.min_pos = -1;
		set_ret_pos_by_p(&ret, P, len_q);
		// Find min abstract
		P[ret.min_pos_q] = -1;
		for(; j < len_s; j ++) {
			if (is_equal(Q[ret.min_pos_q], S[j])) {
				P[ret.min_pos_q] = j;
				set_ret_pos(ret.min_pos_q, j, &ret);
				break;
			}
		}
		if (j >= len_s || P[ret.min_pos_q] < 0) {
			goto OK_RET;
		}
	}
OK_RET:
	free(P);
	return ret;
}

void print_abstract(ELE S[], RET ret) {
	int i = 0;
	printf("[ABSTRACT]: ");
	for (i = ret.min_len_start; i <= ret.min_len_end; i ++) {
		if (i != ret.min_len_start) {
			printf(", ");
		}
		printf("\"%s\"", S[i].data);
	}
	printf("\n");
}

int main(int argc, const char *argv[])
{
	ELE S[] = {{"w0"}, {"w1"}, {"w2"}, {"w3"}, {"q0"}, {"w4"}, {"w5"}, {"q1"}, 
		{"w6"}, {"w7"}, {"w8"}, {"q0"}, {"w9"}, {"q1"}};
	int len_s = sizeof(S)/sizeof(S[0]);
	ELE Q[] = {{"q0"}, {"q1"}};
	int len_q = sizeof(Q)/sizeof(Q[0]);
	RET ret = find_min_abstract(S, len_s, Q, len_q);
	printf("RET min_pos = %d\n", ret.min_pos);
	printf("RET max_pos = %d\n", ret.max_pos);
	printf("RET min_len = %d\n", ret.min_len);
	printf("RET min_len_start = %d\n", ret.min_len_start);
	printf("RET min_len_end = %d\n", ret.min_len_end);
	print_abstract(S, ret);

	return 0;
}
