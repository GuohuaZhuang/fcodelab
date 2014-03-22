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
* @file string_matching.c
* @brief string matching with brute force string matching algorithm, 
* Knuth-Morris-Pratt algorithm and Boyer-Moore algorithm.
* references:
*   http://www-igm.univ-mlv.fr/~lecroq/string/node8.html
*   http://igm.univ-mlv.fr/~lecroq/string/node14.html
*   book <Introduction to Algorithms>
*   http://www.cs.ucla.edu/classes/cs219/NS/tcl7.6/compat/strstr.c
*   library project QDBM
*
* @author firstboy0513
* @version 0.0.1
* @date 2014-03-22
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define XSIZE 255
#define ASIZE 256
#define OUTPUT(x) {printf("%d index is matched!\n", (int)(x));}
#define MAX(x,y) ((x) > (y) ? (x) : (y))
#define DEBUG

#ifdef DEBUG
void _debug_output_array(int* arr, int m) {
    int i = 0;
    for (i = 0; i < m; i ++) {
        printf("%d\t", i);
    }
    printf("\n");
    for (i = 0; i < m; i ++) {
        printf("%d\t", arr[i]);
    }
    printf("\n");
}
void _debug_output_chararray(signed char* arr, int m) {
    int i = 0;
    for (i = 0; i < m; i ++) {
        printf("%d\t", i);
    }
    printf("\n");
    for (i = 0; i < m; i ++) {
        printf("%d\t", (int)(arr[i]));
    }
    printf("\n");
}
#endif // DEBUG

///////////////////// Brute Force algorithm ///////////////////////////////////

void BF1(const char *x, int m, const char *y, int n) {
    int i, j;

    /* Searching */
    for (j = 0; j <= n - m; ++j) {
        for (i = 0; i < m && x[i] == y[i + j]; ++i);
        if (i >= m) { OUTPUT(j); }
    }
}

#define EOS '\0'
void BF2(const char *x, int m, const char *y, int n) { 
    const char *yb; 
    /* Searching */ 
    for (yb = y; *y != EOS; ++y) {
        if (memcmp(x, y, m) == 0) {
            OUTPUT(y - yb);
        }
    }
}

char* mystrstr(string, substring)
    register char *string;  /* String to search. */
    char *substring;        /* Substring to try to find in string. */
{
    register char *a, *b;

    /* First scan quickly through the two strings looking for a
     * single-character match.  When it's found, then compare the
     * rest of the substring.
     */
    b = substring;
    if (*b == 0) { return string; }
    for ( ; *string != 0; string += 1) {
        if (*string != *b) { continue; }
        a = string;
        while (1) {
            if (*b == 0) { return string; }
            if (*a++ != *b++) { break; }
        }
        b = substring;
    }
    return (char *) 0;
}

///////////////////// Knuth-Morris-Pratt algorithm ////////////////////////////

void preKmp(const char *x, int m, int kmpNext[]) {
    int i = 0, j = -1;
    kmpNext[0] = -1;
    while (i < m) {
        while (j > -1 && x[i] != x[j]) { j = kmpNext[j]; }
        i++; j++;
        if (x[i] == x[j]) {
            kmpNext[i] = kmpNext[j];
        } else {
            kmpNext[i] = j;
        }
    }
}

void KMP(const char *x, int m, const char *y, int n) {
    int i, j, kmpNext[XSIZE];

    /* Preprocessing */
    preKmp(x, m, kmpNext);
#ifdef DEBUG
    _debug_output_array((int*)kmpNext, m+1);
#endif // DEBUG

    /* Searching */
    i = j = 0;
    while (j < n) {
        while (i > -1 && x[i] != y[j]) { i = kmpNext[i]; }
        i++; j++;
        if (i >= m) {
            OUTPUT(j - i);
            i = kmpNext[i];
        }
    }
}

#include <assert.h>

char* strstr_kmp(const char* haystack, const char* needle) {
    int i = 0, j = 0, hlen = 0, nlen = 0;
    signed char tbl[0x100];
    assert(haystack && needle);
    nlen = strlen(needle);
    if (nlen >= 0x100) return strstr(haystack, needle);
    tbl[0] = -1; 
    i = 0, j = -1; 
    while (i < nlen) {
        while ((j >= 0) && (needle[i] != needle[j])) {
            j = tbl[j];
        }   
        i ++, j ++; 
        tbl[i] = j;
    }
#ifdef DEBUG
    _debug_output_chararray(tbl, nlen+1);
#endif // DEBUG
    hlen = strlen(haystack);
    i = 0, j = 0;
    while (i < hlen && j < nlen) {
        while ((j >= 0) && (haystack[i] != needle[j])) {
            j = tbl[j];
        }   
        i ++, j ++; 
    }   
    if (j == nlen) return (char*)(haystack + i - nlen);
    return NULL;
}

///////////////////// Boyer-Moore algorithm ///////////////////////////////////

void preBmBc(const char *x, int m, int bmBc[]) {
    int i;
    for (i = 0; i < ASIZE; ++i) { bmBc[i] = m; }
    for (i = 0; i < m - 1; ++i) { bmBc[(int)(x[i])] = m - i - 1; }
}

void suffixes(const char *x, int m, int *suff) {
    int f, g, i;

    suff[m - 1] = m;
    g = m - 1;
    for (i = m - 2; i >= 0; --i) {
        if (i > g && suff[i + m - 1 - f] < i - g) {
            suff[i] = suff[i + m - 1 - f];
        } else {
            if (i < g) { g = i; }
            f = i;
            while (g >= 0 && x[g] == x[g + m - 1 - f]) { --g; }
            suff[i] = f - g;
        }
    }
}

void preBmGs(const char *x, int m, int bmGs[]) {
    int i, j, suff[XSIZE];

    suffixes(x, m, suff);

    for (i = 0; i < m; ++i) { bmGs[i] = m; }
    j = 0;
    for (i = m - 1; i >= 0; --i) {
        if (suff[i] == i + 1) {
            for (; j < m - 1 - i; ++j) {
                if (bmGs[j] == m) { bmGs[j] = m - 1 - i; }
            }
        }
    }
    for (i = 0; i <= m - 2; ++i) {
        bmGs[m - 1 - suff[i]] = m - 1 - i;
    }
}

void BM(const char *x, int m, const char *y, int n) {
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
        } else {
            j += MAX(bmGs[i], bmBc[(int)(y[i + j])] - m + 1 + i);
        }
    }
}

int main(int argc, const char *argv[])
{
    const char* t = "bacbabGCAGaAGGCAGAGAGbaabcbab";
    const char* p = "GCAGAGAG";
    { const char* _r = strstr_kmp(t, p); OUTPUT(_r ? (_r-t) : -1); }
    KMP(p, strlen(p), t, strlen(t));
    BM(p, strlen(p), t, strlen(t));
    { const char* _r = mystrstr(t, p); OUTPUT(_r ? (_r-t) : -1); }
    BF1(p, strlen(p), t, strlen(t));
    BF2(p, strlen(p), t, strlen(t));
    
    return 0;
}

