#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define N 5

void output_array(int X[], int size) {
	int i = 0;
	for (i = 0; i < size; i ++) {
		if (0 != i) { printf(", "); }
		printf("%d", X[i]);
	}
	printf("\n");
}

int** init_opt(int totalV, int T) {
	int** opt = (int**) malloc(sizeof(int*) * (totalV+1));
	int i = 0, j = 0;
	for (i = 0; i <= totalV; i ++) {
		opt[i] = (int*) malloc(sizeof(int) * (T+1));
		for (j = 0; j <= T; j ++) {
			opt[i][j] = INT32_MIN;
		}
	}
	for (i = 0; i <= T; i ++) {
		opt[0][i] = 0;
	}
	return opt;
}

void release_opt(int** opt, int totalV) {
	int i = 0;
	for (i = 0; i <= totalV; i ++) {
		free(opt[i]);
	}
	free(opt);
}

int best_satisfaction_solution(int* V, int* C, int* H, int* B, int size, 
	int totalV) {
	int** opt = init_opt(totalV, size);
	int v = 0, i = 0, k = 0;
	for (v = 0; v <= totalV; v ++) {
		for (i = size-1; i >= 0; i --) {
			for (k = 0; k <= C[i]; k ++) {
				if (v < V[i]*k) { break; }
				int x = opt[v-V[i]*k][i+1];
				if (INT32_MIN != x) {
					x += H[i]*k;
					if (x > opt[v][i]) { opt[v][i] = x; }
				}
			}
		}
	}

	release_opt(opt, totalV);
	return opt[totalV][0];
}

int main(int argc, const char *argv[])
{
	int V[N] = {2, 8, 32, 8, 64};
	int C[N] = {64, 64, 128, 128, 128};
	int H[N] = {1, 2, 4, 3, 5};
	int B[N] = {0, 0, 0, 0, 0};
	int totalV = 256;

	int satisfaction = best_satisfaction_solution(V, C, H, B, N, totalV);
	printf("The best satisfaction solution is: ");
	output_array(B, N);
	printf("And its satisfaction = %d\n", satisfaction);

	return 0;
}
