#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int* COMPUTE_PREFIX_FUNCTION(const char* P, const int m) {
    int* pi = (int*) malloc(sizeof(int) * (m + 1));
    if (!pi) { fprintf(stderr, "malloc err!\n"); return NULL; }
    pi[0] = -1;
    int k = -1;
    int q = 0;
    for (q = 1; q < m; q ++) {
        while (k >= 0 && P[k+1] != P[q]) { k = pi[k]; }
        if (P[k+1] == P[q]) { k ++; }
        pi[q] = k;
    }
    return pi;
}

void KMP_MATCHER(const char* T, const char* P) {
    int n = strlen(T);
    int m = strlen(P)-1;
    int* pi = COMPUTE_PREFIX_FUNCTION(P, m);
    if (!pi) { return; }
    int q = -1;
    int i = 0;
    for (i = 0; i < n; i ++) {
        while (q >= 0 && P[q+1] != T[i]) { q = pi[q]; }
        if (P[q+1] == T[i]) { q ++; }
        if (q == m) {
            printf("Pattern occurs with shift %d\n", i-m);
            q = pi[q];
        }
    }
    free(pi);
}

int main(int argc, const char *argv[])
{
    const char* t = "bacbababaabcbab";
    const char* p = "ababaa";
    KMP_MATCHER(t, p);
    return 0;
}
