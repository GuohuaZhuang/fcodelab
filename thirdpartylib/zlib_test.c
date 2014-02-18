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
* @file zlib_test.c
* @brief zlib simple deflate and inflate interface test case.
* complie:
* 	gcc -g3 -O0 -Wall zlib_test.c -lz -o zlib_test
*
* Note: the following valgrind test contains uninitialized values.
* reference: http://stackoverflow.com/questions/12483612/libzip-example-
* contains-uninitialised-values-when-checked-with-valgrind
*
* Yes, it's an error inside zlib (used by libzip), since both the allocation 
* and the use of the memory is inside inflateInit2_ on the same call. Your code 
* doesn't even have a chance to get to that memory.
*
* I can repeat the problem using zlib 1.2.3, but it's not showing up anymore 
* in 1.2.7. I didn't have the code for 1.2.3 available, but if you're looking 
* at it, I'd check the initialization of state and how it's used inside 
* inflateReset2.
*
* Edit: Tracked down the problem, I downloaded Ubuntu's source package for 
* zlib (1.2.3.4) and the offending line is;
*
*     if (state->wbits != windowBits && state->window != Z_NULL) {
*
* wbits is not initialized previous to this, and will cause the warning. The 
* odd thing is that neither the original zlib 1.2.3 or 1.2.4 have this problem, 
* it seems to be unique for Ubuntu. 1.2.3 doesn't even have the function 
* inflateReset2, and 1.2.4 has it right;
*
*     if (state->window != Z_NULL && state->wbits != (unsigned)windowBits) {
*
* since window is previously initialized to Z_NULL, the uninitialized wbits 
* read won't happen.
*
* @author firstboy0513
* @version 0.0.1
* @date 2014-02-18
*/
#include <zlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ZLIBBUFSIZ     8192

void my_zlib_deflate(char* input, int size, char* output, int* output_n) {
	z_stream strm = { .zalloc = Z_NULL, .zfree = Z_NULL, .opaque = Z_NULL };
	deflateInit2(&strm, 6, Z_DEFLATED, 15 + 16, 9, Z_DEFAULT_STRATEGY); // gzip
	// deflateInit(&strm, 9);
	strm.next_in = (unsigned char*)input; strm.avail_in = size;
	strm.next_out = (unsigned char*)output; strm.avail_out = ZLIBBUFSIZ;
	deflate(&strm, Z_FINISH);
	deflateEnd(&strm);
	*output_n = strm.avail_out;
}

void my_zlib_inflate(char* input, int size, char* output, int* output_n) {
	z_stream strm = { .zalloc = Z_NULL, .zfree = Z_NULL, .opaque = Z_NULL };
	strm.next_in = (unsigned char*)input; strm.avail_in = size;
	inflateInit2(&strm, 15 + 16);	// gzip
	// inflateInit(&strm);
	strm.next_out = (unsigned char*)output; strm.avail_out = ZLIBBUFSIZ;
	inflate(&strm, Z_NO_FLUSH);
	inflateEnd(&strm);
	*output_n = strm.avail_out;
}

int main(int argc, const char *argv[])
{
	char* s = "This is a test text which will compress use zlib.";
	int size = strlen(s), pivot_size = 0, result_size = 0;
	char* pivot = (char*) malloc(ZLIBBUFSIZ);
	memset(pivot, 0, ZLIBBUFSIZ);
	char* result = (char*) malloc(ZLIBBUFSIZ);
	memset(result, 0, ZLIBBUFSIZ);
	my_zlib_deflate(s, size, pivot, &pivot_size);
	my_zlib_inflate(pivot, pivot_size, result, &result_size);
	printf("[riginal]: [%s]\n", s);
	printf("[result]: [%s]\n", result);
	free(pivot);
	free(result);

	return 0;
}
