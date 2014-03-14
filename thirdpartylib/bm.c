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
* @file bm.c
* @brief Boyer-Moore string match algorithm.
* main references:
*   http://igm.univ-mlv.fr/~lecroq/string/node14.html
* other references:
*   http://www.cnblogs.com/lanxuezaipiao/p/3452579.html
*   http://www.searchtb.com/2011/07/字符串匹配那些事（一）.html
*   http://blog.csdn.net/sealyao/article/details/4568167
*
* @author firstboy0513
* @version 0.0.1
* @date 2014-03-14
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ASIZE 256

#define OUTPUT(j) do {                              \
    printf("string match in the offset %d\n", j);   \
} while (0); 

#define MAX(a, b) ((a) > (b) ? (a) : (b))

void preBmBc(const char* x, int m, int bmBc[]) {
    int i = 0;
    for (i = 0; i < ASIZE; i ++) { bmBc[i] = m; }
    for (i = 0; i < m - 1; i ++) { bmBc[(int)(x[i])] = (m-1) - i; }
}

void suffixes(const char* x, int m, int* suffix, int XSIZE) {
    int i = 0;
//  int j = 0;
//  for (i = m-2; i >= 0; i --) {       // the same use more time complexity
//        for (j = i; j >= 0 && x[j] == x[(m-1)-(i-j)]; j --) ;
//        suffix[i] = (i-j);
//  }
    suffix[m-1] = m;
    int f = 0, g = m-1;
    for (i = m-2; i >= 0; i --) {
        if (i > g && suffix[(m-1)-(f-i)] < i - g) {
            suffix[i] = suffix[(m-1)-(f-i)];
        } else {
            if (i < g) { g = i; } /*!!!!这里我觉得应该每次g=i;才对!!!!*/
            f = i;
            while (g >= 0 && x[g] == x[(m-1)-(f-g)]) g --;
            suffix[i] = (f-g);
        }
    }
}

void preBmGs(const char* x, int m, int* bmGs, int XSIZE) {
    int i = 0, j = 0, *suffix = NULL;
    if (!(suffix = (int*) malloc(sizeof(int)*XSIZE))) {
        fprintf(stderr, "malloc err!\n"); return;
    }
    suffixes(x, m, suffix, XSIZE);
    for (i = 0; i < XSIZE; i ++) { bmGs[i] = m; }   // init consider no suffix
    for (i = m-2; i >= 0; i --) {                   // a head prefix as suffix
        if (bmGs[i] == i+1) {
            for (j = m-1-i; j >= 0; j --) { bmGs[j] = (m-1) - i; }
        }
    }
    for (i = 0; i <= m-2; i ++) {                   // find a good suffix
        bmGs[(m-1) - suffix[i]] = (m-1) - i;
    }
    free(suffix);
}

/**
* @brief string matching with Boyer-Moore algorithm.
* Output the offset begin with pattern, otherwise nothing output means not any 
* pattern find in the search string.
* Note: if do not want to use malloc, can use stack memory like ASIZE make a 
* int[] array but not a int* array, and when m >= XSIZE then use strstr(). This
* application in a project name "QDBM" database code.
*
* @param x pattern.
* @param m pattern length.
* @param y search string.
* @param n search string length.
*/
void string_matching_with_bm_algorithm(const char* x, int m, 
    const char* y, int n) {
    int i = 0, j = 0, bmBc[ASIZE] = {0}, *bmGs = NULL, XSIZE = m+1;
    if (!(bmGs = (int*) malloc(sizeof(int)*XSIZE))) {
        fprintf(stderr, "malloc err!\n"); return;
    }
    preBmGs(x, m, bmGs, XSIZE); preBmBc(x, m, bmBc);        // preprocessing
    while (j <= n-m) {                                      // searching
        for (i = m-1; i >= 0 && x[i] == y[j+i]; i --) ;
        if (i < 0) { OUTPUT(j); j += bmGs[0]; }
        else { j += MAX(bmGs[i], bmBc[(int)(y[j+i])]-((m-1)-i)); }
    }
    free(bmGs);
}

int main(int argc, const char *argv[])
{
    const char* x = "cdas";
    const char* y = "ababacdasfdsafds";
    string_matching_with_bm_algorithm(x, strlen(x), y, strlen(y));
    return 0;
}

