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
* @file suffix_array.c
* @brief suffix array implement use doubling calculate mathod.
* @author firstboy0513
* @version 0.0.1
* @date 2014-04-25
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BASE 256

void output(int* arr, int size, const char* name) {
    int i = 0;
    printf("%s[]: ", name);
    for (i = 0; i < size; i ++) {
        if (0 != i) { printf(", "); }
        printf("%d", arr[i]);
    }
    printf("\n");
}

void output_suffix(int* sa, char* s, int size) {
    int i = 0;
    printf("suffixs:\n");
    for (i = 0; i < size; i ++) {
        printf("[%s]\n", s + sa[i]);
    }
    printf("\n");
}

/**
* @brief use radix to sort can just use O(n) times.
*/
void radixsort(int* sa, int* rank, int* tsa, int* sum, int size, int j) {
    int i = 0;
    // sort suffix half part in tsa
    memset(sum, 0, sizeof(int) * BASE);
    for (i = 0; i < size; i ++) {
        if (i+j < size) { sum[ rank[i+j] ] ++; }
        else { sum[ 0 ] ++; }
    }
    for (i = 1; i < BASE; i ++) { sum[i] += sum[i-1]; }
    for (i = size-1; i >= 0; i --) {
        if (i+j < size) { tsa[ --sum[ rank[i+j] ] ] = i; }
        else { tsa[ --sum[ 0 ] ] = i; }
    }
    // sort prefix half part in sa
    memset(sum, 0, sizeof(int) * BASE);
    for (i = 0; i < size; i ++) { sum[ rank[i] ] ++; }
    for (i = 1; i < BASE; i ++) { sum[i] += sum[i-1]; }
    for (i = size-1; i >= 0; i --) {
        sa[ --sum[ rank[tsa[i]] ] ] = tsa[i];
    }
}

/**
* @brief get suffix array and rank array.
*/
void get_sa(char* s, int size, int** psa, int** prank) {
    int p = 0, i = 0, j = 0;
    int *trank = (int*) malloc(sizeof(int) * (size));
    int *rank = (int*) malloc(sizeof(int) * (size));
    int *sum = (int*) malloc(sizeof(int) * BASE);
    int *sa = (int*) malloc(sizeof(int) * (size));
    int *tsa = (int*) malloc(sizeof(int) * (size));
    memset(sum, 0, sizeof(int) * BASE);
    for (i = 0; i < size; i ++) { trank[i] = (int)s[i]; }
    for (i = 0; i < size; i ++) { sum[ trank[i] ] ++; }
    for (i = 1; i < BASE; i ++) { sum[i] += sum[i-1]; }
    for (i = size-1; i >= 0; i --) { sa[ --sum[ trank[i] ] ] = i; }
    rank[ sa[0] ] = 1;
    for (i = 1, p = 1; i < size; i ++) {
        if (trank[ sa[i] ] != trank[ sa[i-1] ]) { p ++; }
        rank[ sa[i] ] = p;
    }
    for (j = 1; j <= size; j *= 2) {
        radixsort(sa, rank, tsa, sum, size, j);
        trank[ sa[0] ] = 1;
        for (i = 1, p = 1; i < size; i ++) {
            if ((rank[ sa[i] ] != rank[ sa[i-1] ])
                || (sa[i] + j < size && sa[i-1] + j >= size)
                || (sa[i] + j >= size && sa[i-1] + j < size)
                || (sa[i] + j < size && sa[i-1] + j < size 
                    && rank[ sa[i] + j ] != rank[ sa[i-1] + j ])) { p ++; }
            trank[ sa[i] ] = p;
        }
        for (i = 0; i < size; i ++) { rank[i] = trank[i]; }
    }
    free(trank);
    free(sum);
    free(tsa);
    if (psa) { *psa = sa; } else { free(sa); }
    if (prank) { *prank = rank; } else { free(rank); }
}

/**
* @brief get height array.
*/
void get_height(char* s, int *sa, int *rank, int size, int **pheight) {
    int i = 0, j = 0;
    int *height = (int*) malloc(sizeof(int) * size);
    memset(height, 0, sizeof(int) * size);
    for (i = 0, j = 0; i < size; i ++) {
        if (rank[i] == 1) { continue; }
        for (; i+j < size && sa[rank[i]-1-1]+j < size 
            && s[i+j] == s[ sa[rank[i]-1-1]+j ]; ) { j ++; }
        height[ rank[i]-1 ] = j;
        if (j > 0) { j --; }
    }
    if (pheight) { *pheight = height; } else { free(height); }
}

int main(int argc, const char *argv[])
{
    // char s[] = "mississippi";
    char s[] = "aabaaaab";
    int size = strlen(s);
    printf("size = %d\n", size);
    int *sa = NULL, *rank = NULL, *height = NULL;
    get_sa(&s[0], size, &sa, &rank);
    printf("---------End results----------------\n");
    output(sa, size, "SA");
    output(rank, size, "Rank");
    output_suffix(sa, &s[0], size);
    get_height(&s[0], sa, rank, size, &height);
    output(height, size, "height");
    if (sa) { free(sa); sa = NULL; }
    if (rank) { free(rank); rank = NULL; }
    if (height) { free(height); height = NULL; }

    // calculate longest palindrome length
    char palindrome_s[1024];
    sprintf(palindrome_s, "%s#", s);
    // reverse s
    int begin = 0, end = size-1; char c;
    while (begin < end) { c = s[begin]; s[begin++] = s[end]; s[end--] = c; }
    strcat(palindrome_s, s);
    size = strlen(palindrome_s);
    get_sa(palindrome_s, size, &sa, &rank);
    get_height(palindrome_s, sa, rank, size, &height);
    output(height, size, "height");
    output_suffix(sa, palindrome_s, size);
    if (sa) { free(sa); sa = NULL; }
    if (rank) { free(rank); rank = NULL; }
    if (height) { free(height); height = NULL; }
    return 0;
}
