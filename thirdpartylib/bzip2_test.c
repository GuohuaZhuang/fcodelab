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
* @file bzip2_test.c
* @brief bzip2 library use low-level interface test case.
* complie:
* 	gcc -g3 -O0 -Wall -std=c99 bzip2_test.c -lbz2 -o bzip2_test
* @author firstboy0513
* @version 0.0.1
* @date 2014-02-15
*/
#include <bzlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BZIPBUFSIZ     8192

void my_bzcompress(char* input, int size, char* ouput, int* output_n) {
	bz_stream strm = { .bzalloc = NULL, .bzfree = NULL, .opaque = NULL };
	BZ2_bzCompressInit(&strm, 9, 0, 30);
	strm.next_in = input; strm.avail_in = size;
	strm.next_out = malloc(BZIPBUFSIZ); strm.avail_out = BZIPBUFSIZ;
	BZ2_bzCompress(&strm, BZ_FINISH);
	BZ2_bzCompressEnd(&strm);
	*output_n = strm.avail_out;
}

void my_bzdecompress(char* input, int size, char* output, int* output_n) {
	bz_stream strm = { .bzalloc = NULL, .bzfree = NULL, .opaque = NULL };
	BZ2_bzDecompressInit(&strm, 0, 0);
	strm.next_in = input; strm.avail_in = size;
	strm.next_out = malloc(BZIPBUFSIZ); strm.avail_out = BZIPBUFSIZ;
	BZ2_bzDecompressEnd(&strm);
	BZ2_bzCompress(&strm, BZ_FINISH);
	*output_n = strm.avail_out;
}

int main(int argc, const char *argv[])
{
	char* s = "This is a test text which will compress use bzip2.";
	int size = strlen(s), pivot_size = 0, result_size = 0;
	char* pivot = (char*) malloc(BZIPBUFSIZ);
	char* result = (char*) malloc(BZIPBUFSIZ);
	my_bzcompress(s, size, pivot, &pivot_size);
	my_bzdecompress(pivot, pivot_size, result, &result_size);
	printf("[riginal]: [%s]\n", s);
	printf("[result]: [%s]\n", result);
	free(pivot);
	free(result);

	return 0;
}
