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
* @author firstboy0513
* @version 0.0.1
* @date 2014-04-18
*/
#include <stdio.h>
#include <stdlib.h>

/**
* @brief Convert M ary number into N ary number use simple modulo method.
*
* @param m M ary.
* @param m_num M ary number express as a string.
* @param n N ary.
* @param n_num N ary number express as a string.
*/
void m2n(int m, const char* m_num, int n, char* n_num) {
    int x = 0;
    const char *mp = m_num; char *np = n_num;
    while (*mp) {
        x = m*x + (*mp-'0') % m;
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

int main(int argc, const char *argv[])
{
    int m = 10, n = 10;
    const char* m_num = "235";
    char* n_num = (char*) malloc(20);
    m2n(m, m_num, n, n_num);
    printf("m_num = [%s]\n", m_num);
    printf("n_num = [%s]\n", n_num);

    if (n_num) { free(n_num); }
    return 0;
}
