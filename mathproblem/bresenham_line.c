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
* @file bresenham_line.c
* @brief Bresenham line algorithm implement in C.
* reference:
*   http://en.wikipedia.org/wiki/Bresenham's_line_algorithm
* @author firstboy0513
* @version 0.0.1
* @date 2014-03-22
*/
#include <stdio.h>
#include <stdlib.h>

void plot(int x, int y) { printf("(%d,%d) ", x, y); }

// start require x0 < x1, and y0 < y1 and deltax > deltay
void line1(int x0, int x1, int y0, int y1) {
    int deltax = x1 - x0;
    int deltay = y1 - y0;
    double error = 0;
    double deltaerr = (double)deltay / deltax;
    int y = y0, x;
    for (x = x0; x <= x1; x ++) {
        plot(x,y);
        error = error + deltaerr;
        if (abs(error) >= 0.5) {
            y = y + 1;
            error = error - 1.0;
        }
    }
}

#define SWAP(a, b) { a ^= b; b ^= a; a ^= b; }

// optimize line1() not require two conditions.
void line2(int x0, int x1, int y0, int y1) {
    int steep = abs(y1 - y0) > abs(x1 - x0);
    if (steep) { SWAP(x0, y0); SWAP(x1, y1); }
    if (x0 > x1) { SWAP(x0, x1); SWAP(y0, y1); }
    int deltax = x1 - x0;
    int deltay = y1 - y0;
    double error = 0;
    double deltaerr = (double)deltay / deltax;
    int y = y0, x, ystep = 1;
    if (y0 < y1) { ystep = 1; } else { ystep = -1; }
    for (x = x0; x <= x1; x ++) {
        if (steep) { plot(y, x); } else { plot(x, y); }
        error = error + deltaerr;
        if (error >= 0.5) {
            y = y + ystep;
            error = error - 1.0;
        }
    }
}

// optimize line2() not use double to operator.
void line3(int x0, int x1, int y0, int y1) {
    int steep = abs(y1 - y0) > abs(x1 - x0);
    if (steep) { SWAP(x0, y0); SWAP(x1, y1); }
    if (x0 > x1) { SWAP(x0, x1); SWAP(y0, y1); }
    int deltax = x1 - x0;
    int deltay = y1 - y0;
    int error = deltax / 2;
    int y = y0, x, ystep = 1;
    if (y0 < y1) { ystep = 1; } else { ystep = -1; }
    for (x = x0; x <= x1; x ++) {
        if (steep) { plot(y, x); } else { plot(x, y); }
        error = error - deltay;
        if (error < 0) {
            y = y + ystep;
            error = error + deltax;
        }
    }
}

// optimize line3() not to swap x axis.
void line4(int x0, int x1, int y0, int y1) {
    int steep = abs(y1 - y0) > abs(x1 - x0);
    if (steep) { SWAP(x0, y0); SWAP(x1, y1); }
    int deltax = x1 - x0;
    int deltay = y1 - y0;
    int error = deltax / 2;
    int y = y0, x, ystep = 1, xstep = 1;
    if (x0 < x1) { xstep = 1; } else { xstep = -1; }
    if (y0 < y1) { ystep = 1; } else { ystep = -1; }
    for (x = x0; x <= x1; x += xstep) {
        if (steep) { plot(y, x); } else { plot(x, y); }
        error = error - deltay;
        if (error < 0) {
            y = y + ystep;
            error = error + deltax;
        }
    }
}

// optimize line4() not to swap if deltax < deltay 
void line5(int x0, int x1, int y0, int y1) {
    int dx = abs(x1-x0);
    int dy = abs(y1-y0);
    int sx = 1, sy = 1;
    if (x0 < x1) { sx = 1; } else { sx = -1; }
    if (y0 < y1) { sy = 1; } else { sy = -1; }
    int err = dx-dy, e2;

    while (1) {
        plot(x0,y0);
        if (x0 == x1 && y0 == y1) { break; }
        e2 = 2*err;
        if (e2 > -dy) {
            err = err - dy;
            x0 = x0 + sx;
        }
        if (e2 < dx) {
            err = err + dx;
            y0 = y0 + sy;
        }
    }
}

int main(int argc, const char *argv[])
{
    printf("line1: "); line1(0, 12, 0, 6); printf("\n");
    printf("line2: "); line2(0, 12, 0, 6); printf("\n");
    printf("line3: "); line3(0, 12, 0, 6); printf("\n");
    printf("line4: "); line4(0, 12, 0, 6); printf("\n");
    printf("line5: "); line5(0, 12, 0, 6); printf("\n");
    return 0;
}
