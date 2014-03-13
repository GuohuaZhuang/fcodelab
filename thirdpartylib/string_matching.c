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

void preBmBc(char *x, int m, int bmBc[]) {
   int i;
 
   for (i = 0; i < ASIZE; ++i)
      bmBc[i] = m;
   for (i = 0; i < m - 1; ++i)
      bmBc[x[i]] = m - i - 1;
}
 
 
void suffixes(char *x, int m, int *suff) {
   int f, g, i;
 
   suff[m - 1] = m;
   g = m - 1;
   for (i = m - 2; i >= 0; --i) {
      if (i > g && suff[i + m - 1 - f] < i - g)
         suff[i] = suff[i + m - 1 - f];
      else {
         if (i < g)
            g = i;
         f = i;
         while (g >= 0 && x[g] == x[g + m - 1 - f])
            --g;
         suff[i] = f - g;
      }
   }
}
 
void preBmGs(char *x, int m, int bmGs[]) {
   int i, j, suff[XSIZE];
 
   suffixes(x, m, suff);
 
   for (i = 0; i < m; ++i)
      bmGs[i] = m;
   j = 0;
   for (i = m - 1; i >= 0; --i)
      if (suff[i] == i + 1)
         for (; j < m - 1 - i; ++j)
            if (bmGs[j] == m)
               bmGs[j] = m - 1 - i;
   for (i = 0; i <= m - 2; ++i)
      bmGs[m - 1 - suff[i]] = m - 1 - i;
}
 
 
void BM(char *x, int m, char *y, int n) {
   int i, j, bmGs[XSIZE], bmBc[ASIZE];
 
   /* Preprocessing */
   preBmGs(x, m, bmGs);
   preBmBc(x, m, bmBc);
 
   /* Searching */
   j = 0;
   while (j <= n - m) {
      for (i = m - 1; i >= 0 && x[i] == y[i + j]; --i);
      if (i < 0) {
         OUTPUT(j);
         j += bmGs[0];
      }
      else
         j += MAX(bmGs[i], bmBc[y[i + j]] - m + 1 + i);
   }
}
