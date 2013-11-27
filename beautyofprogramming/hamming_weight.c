/* Copyright (C) 
* 2013 - firstboy0513
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
* @file hamming_weight.c
* @brief hamming weght.
* Or call it population count, popcount or sideways sum. It reference in 
* http://en.wikipedia.org/wiki/Hamming_weight
* It can use to calculate hamming distance.
* @author firstboy0513
* @version 0.0.1
* @date 2013-11-26
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
* @brief hamming weight using modulo method.
*
* @param x input unsigned.
*
* @return hamming weight, number of 1's.
*/
int hammingweight_modulo_method(unsigned int x) {
	int n = 0;
	do { ((1 == x%2) ? (n++) : n); } while (x >>= 1);
	return n;
}

/**
* @brief hamming weight using logic and method.
*
* @param x input unsigned.
*
* @return hamming weight, number of 1's.
*/
int hammingweight_logicand_method(unsigned int x) {
	int n = 0;
	do { ((x & 0x01) ? (n++) : n); } while (x >>= 1);
	return n;
}

/**
* @brief hamming weight using logic and minus one method.
*
* @param x input unsigned.
*
* @return hamming weight, number of 1's.
*/
int hammingweight_minusand_method(unsigned int x) {
	int n = 0;
	while (x) { n ++; x &= (x - 1); }
	return n;
}

/**
* @brief hamming weight using search table method.
* This method is just for a fixed length input.
*
* @param x input unsigned.
*
* @return hamming weight, number of 1's.
*/
int hammingweight_table_method(unsigned int x) {
	static int table[256] = {
		0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4, 
		1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 
		1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 
		2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
		1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 
		2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
		2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
		3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 
		1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 
		2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
		2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
		3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 
		2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
		3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 
		3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 
		4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8
	};
	return table[x];
}

#include <ctype.h>
/**
* @brief Reference from http://en.wikipedia.org/wiki/Hamming_weight
* Types and constants used in the functions below.
*/
typedef unsigned long long int uint64;  //assume this gives 64-bits
const uint64 m1  = 0x5555555555555555; //binary: 0101...
const uint64 m2  = 0x3333333333333333; //binary: 00110011..
const uint64 m4  = 0x0f0f0f0f0f0f0f0f; //binary:  4 zeros,  4 ones ...
const uint64 m8  = 0x00ff00ff00ff00ff; //binary:  8 zeros,  8 ones ...
const uint64 m16 = 0x0000ffff0000ffff; //binary: 16 zeros, 16 ones ...
const uint64 m32 = 0x00000000ffffffff; //binary: 32 zeros, 32 ones
const uint64 hff = 0xffffffffffffffff; //binary: all ones
const uint64 h01 = 0x0101010101010101; //the sum of 256 to the power of 0,1,...

/**
* @brief hamming weight using population count naive method.
* This is a naive implementation, shown for comparison,
* and to help in understanding the better functions.
* It uses 24 arithmetic operations (shift, add, and).
*
* @param x input unsigned.
*
* @return hamming weight, number of 1's.
*/
int hammingweight_popcount_naive_method(uint64 x) {
	x = (x & m1) + ((x >> 1) & m1);
	x = (x & m2) + ((x >> 2) & m2);
	x = (x & m4) + ((x >> 4) & m4);
	x = (x & m8) + ((x >> 8) & m8);
	x = (x & m16) + ((x >> 16) & m16);
	x = (x & m32) + ((x >> 32) & m32);
	return x;
}

/**
* @brief hamming weight using fewer arithmetic population count method.
* This uses fewer arithmetic operations than any other known  
* implementation on machines with slow multiplication.
* It uses 17 arithmetic operations.
* As the first 2 bit minus like this:
*     00 result: 0(00)
*     01 result: 1(01=01-00)
*     10 result: 1(01=10-01)
*     11 result: 2(10=11-01)
* So the first minus is ok, just for reduice one arithmetic.
* And the end move 4 bit can add first beause its max count is remain in 4 bit.
* So as 8/16/32 bit is the same situation, and just remain last 0x7f 7 bit 
* number as result is ok.
*
* @param x input unsigned.
*
* @return hamming weight, number of 1's.
*/
int hammingweight_popcount_fewer_method(uint64 x) {
	x -= ((x >> 1) & m1);
	x = (x & m2) + ((x >> 2) & m2);
	x = ((x + (x >> 4)) & m4);
	x += (x >> 8);
	x += (x >> 16);
	x += (x >> 32);
	return (x & 0x7f);
}

/**
* @brief hamming weight using more fewer arithmetic population count method.
* This uses fewer arithmetic operations than any other known 
* implementation on machines with fast multiplication.
* It uses 12 arithmetic operations, one of which is a multiply.
* The end return is reason like this:
*
* x' = x+(x>>8)
*    = (x*0x0100)>>8 + (x>>8) // cast aside head 8 bit
*    = ((x*0x0100) + x)>>8
*    = (x*0x0101)>>8 // substituting
* 
* x'' = x'+(x'>>16)
*     = (x'*0x010000)>>16 + (x'>>16) // cast aside head 16 bit
*     = ((x'*0x010000) + x')>>16
*     = ((x*0x0101*0x010000) + x*0x0101)>>24 // substituting
*     = (x*0x01010000 + x*0x0101)>>24
*     = (x*0x01000000 + x*0x010000 + x*0x0100 + x*01)>>24
*     = (x*0x01010101)>>24
* 
* x''' = x''+(x''>>32)
*      = (x''*0x0100000000)>>32 + (x''>>32) // cast aside head 32 bit
*      = ((x''*0x0100000000) + x'')>>32
*      = ((x*0x01010101*0x0100000000) + x*0x01010101)>>56 // substituting
*      = (x*0x0101010101010101)>>56
*
* The cast aside head bit like this:
* I calculate x 'results after calculating x'' in front of more than bit-depth 
* understanding not feel that way, how could this child on OK's.
* 
* X is the current
*     0x 0808 0808 0808 0808
* 
* Using direct addition method:
*     x+(x>>8)
*     0x 0808 0808 0808 0808 + 
*     0x 0008 0808 0808 0808 = 
*     0x 0810 1010 1010 1010
* 
* Using the displacement method: (to eliminate the contents of the former 8bit)
*     (x*0x0100)>>8 + (x>>8)
*     0x 0008 0808 0808 0808 + 
*     0x 0008 0808 0808 0808 = 
*     0x 0010 1010 1010 1010
* 
* Then, look 24 bit.
*     = (x'*0x010000)>>16 + (x'>>16)
*     = ((x*0x0100)>>8)*0x010000 + (x>>8)*0x010000 
*       + ((x*0x0100)>>8)>>16 + (x>>8)>>16
*     = (((x*0x0100)>>8)*0x010000 + (x>>8)*0x010000)>>16 
*     	+ (x*0x0100)>>24 + (x>>24)
*     = ((x*0x01000000)>>24) + ((x*0x010000)>>24) 
*       + ((x*0x0100)>>24) + (x>>24) // ★
* 
*     0x 0000 0008 0808 0808 +
*     0x 0000 0008 0808 0808 +
*     0x 0000 0008 0808 0808 +
*     0x 0000 0008 0808 0808 =
*     0x 0000 0020 2020 2020
* 
* So finally when turned 56 bit, as it is directly in front of the 56bit get 
* washed!
*     ...
*     ...
*     = ((x*0x0100000000000000)>>56) + ((x*0x01000000000000)>>56) 
*       + ((x*0x010000000000)>>56) + ((x*0x0100000000)>>56)
*       + ((x*0x01000000)>>56) + ((x*0x010000)>>56) 
*       + ((x*0x0100)>>56) + (x>>56) // ★
* 
*     0x 0000 0000 0000 0008 +
*     0x 0000 0000 0000 0008 +
*     0x 0000 0000 0000 0008 +
*     0x 0000 0000 0000 0008 +
*     0x 0000 0000 0000 0008 +
*     0x 0000 0000 0000 0008 +
*     0x 0000 0000 0000 0008 +
*     0x 0000 0000 0000 0008 =
*     0x 0000 0000 0000 0040
* 
* So the end (& 0x7f) is not must to wash.
* 
* @param x input unsigned.
*
* @return hamming weight, number of 1's.
*/
int hammingweight_popcount_morefewer_method(uint64 x) {
	x -= ((x >> 1) & m1);
	x = (x & m2) + ((x >> 2) & m2);
	x = ((x + (x >> 4)) & m4);
	return (x * h01) >> 56; // returns left 8 bits of 
							// x + (x<<8) + (x<<16) + (x<<24) + ... 
}

/**
* @brief use MIT HAKMEM naive method.
* As we want to calculate 64bit input, so we split every 8bit element.
*
* 1. We know, a number in computer is express like this:
*	 	S = A0*(2^0) + A1*(2^1) + A2*(2^2) + ...
*	 if replace 2 base as n, then it express like this:
*	 	S = A0*(n^0) + A1*(n^1) + A2(n^2) + ...
*	 So, the popcount is just want the (A0+A1+A2+A3).
*
* 2. We know this enquation:
*	 	n%(n-1) = 1
*	 And we can also get:
*	 	(n^N)^(n-1)
*	 You can use Mathematical induction to prove it.
*	 	we know (2^N)%1 = 1
*	 	and if (n^(k-1))%(n-1) = 1
*	 	then (n^k)%(n-1) = (n*(n^(k-1)))%(n-1) = ((n-1+1)*(n^(k-1)))%(n-1)
*	 		= ((n-1)*(n^(k-1))+(n^(k-1)))%(n-1)
*	 		= (n^(k-1))%(n-1)
*	 		= 1
*	 	so (n^k)%(n-1) = 1.
*
* 3. If we want to get (A0+A1+A2+...), how about get a modulo base which must 
* 	greater than max summation of (A0+A1+A2+...). How about 64bit, the max 
* 	summation is 0x40, we can use 7bit or more, as (2^7 = 128 > 64), as we want
* 	to optimize it use minus method, so we use 8bit to do it.(2^8 = 256 > 64) 
* 	can do it either. so base n = 256, and modulo base n-1 = 255.
* 		P{A} = A0*(256^0) + A1*(256^1) + ... + A7*(256^7)
* 		P{A} % 255 == (A0 + A1 + ... + A7) % 255
*
* @param x input unsigned.
*
* @return hamming weight, number of 1's.
*/
int hammingweight_popcount_mithakmem_8bitnaive_method(uint64 x) {
	uint64 t = (x & 0x0101010101010101)
		+ ((x >> 1) & 0x0101010101010101)
		+ ((x >> 2) & 0x0101010101010101)
		+ ((x >> 3) & 0x0101010101010101)
		+ ((x >> 4) & 0x0101010101010101)
		+ ((x >> 5) & 0x0101010101010101)
		+ ((x >> 6) & 0x0101010101010101)
		+ ((x >> 7) & 0x0101010101010101);
	return (t % 255);
}

/**
* @brief use MIT HAKMEM merger method.
* As 8 bit max count is 00001000, it use just 4bit to store result, so we can
* merger it binary.
*
* @param x input unsigned.
*
* @return hamming weight, number of 1's.
*/
int hammingweight_popcount_mithakmem_8bitmerger_method(uint64 x) {
	uint64 t = (x & 0x1111111111111111)
		+ ((x >> 1) & 0x1111111111111111)
		+ ((x >> 2) & 0x1111111111111111)
		+ ((x >> 3) & 0x1111111111111111);
	return ((t + (t >> 4)) & 0x0f0f0f0f0f0f0f0f) % 255;
}

/**
* @brief use MIT HAKMEM minus method.
* Inspire by 
* 		(4a+2b+c) - (2a+b) - (a) = (a+b+c), 
* we can use
* 		(8a+4b+2c+d) - (4a+2b+c) - (2a+b) - (a) = (a+b+c+d), 
* so, use the 8bit minus method like this.
* It uses 14 arithmetic operations.
*
* @param x input unsigned.
*
* @return hamming weight, number of 1's.
*/
int hammingweight_popcount_mithakmem_8bitminus_method(uint64 x) {
	uint64 t = x
		- ((x >> 1) & 0x7777777777777777)
		- ((x >> 2) & 0x3333333333333333)
		- ((x >> 3) & 0x1111111111111111);
	return ((t + (t >> 4)) & 0x0f0f0f0f0f0f0f0f) % 255;
}

/**
* @brief use MIT HAKMEM naive method.
* Note this x input is a 32 unsigned int, and 0... is a 8ary.
*
* @param x input unsigned.
*
* @return hamming weight, number of 1's.
*/
int hammingweight_popcount_mithakmem_6bitnaive_method(unsigned int x) {
	unsigned int t = (x & 010101010101)
		+ ((x >> 1) & 010101010101)
		+ ((x >> 2) & 010101010101)
		+ ((x >> 3) & 010101010101)
		+ ((x >> 4) & 010101010101)
		+ ((x >> 5) & 010101010101);
	return (t % 63);
}

/**
* @brief use MIT HAKMEM merger method.
* Note this x input is a 32 unsigned int, and 0... is a 8ary.
*
* @param x input unsigned.
*
* @return hamming weight, number of 1's.
*/
int hammingweight_popcount_mithakmem_6bitmerger_method(unsigned int x) {
	unsigned int t = (x &011111111111)
		+ ((x >> 1) & 011111111111)
		+ ((x >> 2) & 011111111111);
	return ((t + (t >> 3)) & 030707070707) % 63;
}

/**
* @brief use MIT HAKMEM minus method.
* Note this x input is a 32 unsigned int, and 0... is a 8ary.
* It uses 10 arithmetic operations.
*
* @param x input unsigned.
*
* @return hamming weight, number of 1's.
*/
int hammingweight_popcount_mithakmem_6bitminus_method(unsigned int x) {
	unsigned int t = x
		- ((x >> 1) & 033333333333)
		- ((x >> 2) & 011111111111);
	return ((t + (t >> 3)) & 030707070707) % 63;
}

//////////////////////Another method///////////////////////////////

int pop(unsigned x) {
	unsigned n;
	n = (x >> 1) & 0x77777777;
	x = x - n;
	n = (n >> 1) & 0x77777777;
	x = x - n;
	n = (n >> 1) & 0x77777777;
	x = x - n;
	x = (x + (x >> 4)) & 0x0F0F0F0F;
	x = x * 0x01010101;
	return x >> 24;
}

int NumberOfSetBits(int i) {
    i = i - ((i >> 1) & 0x55555555);
    i = (i & 0x33333333) + ((i >> 2) & 0x33333333);
    return (((i + (i >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
}

extern int __builtin_popcount (unsigned int x);

int countSetBits(int n) {
    return !n ? 0 : 1 + countSetBits(n & (n-1));
}

#define MASK_01010101 (((unsigned int)(-1))/3)
#define MASK_00110011 (((unsigned int)(-1))/5)
#define MASK_00001111 (((unsigned int)(-1))/17)
int bitcount_mask(unsigned int n) {
	n = (n & MASK_01010101) + ((n >> 1) & MASK_01010101) ; 
	n = (n & MASK_00110011) + ((n >> 2) & MASK_00110011) ; 
	n = (n & MASK_00001111) + ((n >> 4) & MASK_00001111) ; 
	return n % 255 ;
}

unsigned numbits(unsigned int v) {
	unsigned int const MASK1 = 0x55555555;
	unsigned int const MASK2 = 0x33333333;
	unsigned int const MASK4 = 0x0f0f0f0f;
	unsigned int const w = v - ((v >> 1) & MASK1);
	unsigned int const x = (w & MASK2) + ((w >> 2) & MASK2);
	unsigned int const y = (x + ((x >> 4) & MASK4));
	unsigned int const c = (y * 0x01010101) >> 24;
	return c;
}

/*
写到这里都很惊讶有木有，方法原来这么多，简直就是一千个哈姆雷特了。
期待自己写一篇综述类型的文档。
并把这里的算法整理一下，另外看看有没有更好的突破点。
reference: 
	http://en.wikipedia.org/wiki/Hamming_weight
	http://blog.csdn.net/msquare/article/details/4536388
Note: 另外，有闲暇之余写个综述类型的Blog放到cbigu.com上面。
*/
int main(int argc, const char *argv[])
{
	uint64 x = 0xffffffffffffffff;
	// unsigned int x = 0xe2;
	// int n = hammingweight_modulo_method(x);
	// int n = hammingweight_logicand_method(x);
	// int n = hammingweight_minusand_method(x);
	// int n = hammingweight_table_method(x);
	// int n = hammingweight_popcount_naive_method(x);
	// int n = hammingweight_popcount_fewer_method(x);
	// int n = hammingweight_popcount_morefewer_method(x);
	// int n = hammingweight_popcount_mithakmem_method(x);
	// int n = hammingweight_popcount_mithakmem_method(x);
	// int n = hammingweight_popcount_mithakmem_8bitnaive_method(x);
	// int n = hammingweight_popcount_mithakmem_8bitmerger_method(x);
	int n = hammingweight_popcount_mithakmem_8bitminus_method(x);
	printf("Hamming weight n = 0x%016x\n", n);

	return 0;
}
