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
* @file m2n_ary.c
* @brief Convert M ary number into N ary number.
* Note: The M ary can not more than total char2int number! If the M ary is 
* bigger than array of char2int number, will occur some errors as it is 
* overflow now. this program is support not more than 61 base numbers, 
* because total char has just "0-9", "A-Z", "a-z" 62 characters.
* @author firstboy0513
* @version 0.0.1
* @date 2014-04-18
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
* @brief Convert M ary number into N ary number use simple modulo method.
*
* @param m M ary.
* @param m_num M ary number express as a string.
* @param n N ary.
* @param n_num N ary number express as a string.
*/
void m2n(int m, const char *m_num, int n, char *n_num) {
    int x = 0;
    const char *mp = m_num; char *np = n_num;
    while (*mp) {
        x = m*x + (*mp - '0');
        mp ++;
    }
    while (x > 0) {
        *np++ = (x % n) + '0';
        x /= n;
    }
    *np = '\0'; np --;
    while (np > n_num) {
        (*np) ^= (*n_num);
        (*n_num) ^= (*np);
        (*np) ^= (*n_num);
        np --, n_num ++;
    }
}

/**
* @brief convert char to int.
*
* @param c input char.
*
* @return return integer.
*/
int char2int(char c) {
    if ('0' <= c && c <= '9') {
        return c - '0';
    } else if ('A' <= c && c <= 'Z') {
        return c - 'A' + 10;
    } else if ('a' <= c && c <= 'z') {
        return c - 'a' + 36;
    } else {
        printf("invalid char args c = \'%c\'\n", c);
        return -1;
    }
}

/**
* @brief convert int to char.
*
* @param d input integer.
*
* @return return char.
*/
char int2char(int d) {
    if (0 <= d && d <= 9) {
        return d + '0';
    } else if (10 <= d && d <= 35) {
        return d - 10 + 'A';
    } else if (36 <= d && d <= 61) {
        return d - 36 + 'a';
    } else {
        printf("invalid int args d = %d\n", d);
        return '\0';
    }
}

/**
* @brief convert M ary number into N ary number use iterator method.
*
* @param m M ary.
* @param m_num M ary number express as a string.
* @param n N ary.
* @param n_num N ary number express as a string.
*/
void m2n_iterator_method(int m, const char *m_num, int n, char *n_num) {
    if (m <= 0 || n <= 0) { printf("[ERR] -- input invalid!\n"); return; }
    if (!m_num || !n_num) { return; }
    int m_num_len = strlen(m_num);
    char *m_num_copy = (char*) malloc(m_num_len + 1);
    if (!m_num_copy) { printf("[ERR] -- malloc failed!\n"); return; }
    char *m_pivot = (char*) malloc(m_num_len + 1);
    if (!m_pivot) { printf("[ERR] -- malloc failed!\n"); return; }
    memset(m_pivot, 0, m_num_len + 1);
    strcpy(m_num_copy, m_num);

    char *pm = NULL, *pp = NULL, *pn = n_num;
    int non_zero = 1, x = 0;
    while (non_zero != 0) {
        pm = m_num_copy; pp = m_pivot;
        non_zero = 0; x = 0;
        while (*pm != '\0') {
            x = x * m + char2int(*pm);
            *pp = int2char(x / n);
            if (*pp != '0') { non_zero = 1; }
            x %= n;
            pm ++; pp ++;
        }
        *pn++ = int2char(x); *pp = '\0';
        pm = m_num_copy; pp = m_pivot;
        while (*pp == '0') { pp ++; }
        while (*pp != '\0') { *pm++ = *pp++; } *pm = '\0';
    }

    if (m_num_copy) { free(m_num_copy); }
    if (m_pivot) { free(m_pivot); }

    *pn = '\0'; pn --;
    while (pn > n_num) {
        (*pn) ^= (*n_num); (*n_num) ^= (*pn); (*pn) ^= (*n_num);
        pn --; n_num ++;
    }
}

#include <math.h>

int main(int argc, const char *argv[])
{
//    int m = 62, n = 2;
//    const char* m_num = "abcdefghiz";
//    int m = 10, n = 16;
//    const char* m_num = "1234567890123456789012345678901234567890";
//    int m = 16, n = 35;
//    const char* m_num = "3A0C92075C0DBF3B8ACBC5F96CE3F0AD2";
//    int m = 35, n = 23;
//    const char* m_num = "333YMHOUE8JPLT7OX6K9FYCQ8A";
//    int m = 23, n = 49;
//    const char* m_num = "946B9AA02MI37E3D3MMJ4G7BL2F05";
//    int m = 49, n = 61;
//    const char* m_num = "1VbDkSIMJL3JjRgAdlUfcaWj";
//    int m = 61, n = 5;
//    const char* m_num = "dl9MDSWqwHjDnToKcsWE1S";
//    int m = 5, n = 10;
//    const char* m_num = "42104444441001414401221302402201233340311104212022133030";
    int m = 8, n = 10;
    const char* m_num = "731";
    // the length of n, imaging hex and bin ary convert length.
    char* n_num = (char*) malloc((int)ceil(strlen(m_num) * log(m)/log(n)) + 1);

    // m2n(m, m_num, n, n_num);
    m2n_iterator_method(m, m_num, n, n_num);
    printf("m_num = [%s](%d)\n", m_num, m);
    printf("n_num = [%s](%d)\n", n_num, n);

    if (n_num) { free(n_num); }
    return 0;
}
