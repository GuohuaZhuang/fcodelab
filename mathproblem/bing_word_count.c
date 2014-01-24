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
* @file bing_word_count.c
* @brief use recursion method to calculate the bing word combines count.
* The question is:
* 题目详情
* 	本届大赛由微软必应词典冠名，必应词典(http://cn.bing.com/dict/?form=BDVSP4&m
* kt=zh-CN&setlang=ZH)是微软推出的新一代英语学习引擎，里面收录了很多我们常见的单
* 词。但现实生活中，我们也经常能看到一些毫无规则的字符串，导致词典无法正常收录，
* 不过，我们是否可以从无规则的字符串中提取出正规的单词呢？
* 	例如有一个字符串"iinbinbing"，截取不同位置的字符‘b’、‘i’、‘n’、‘g’组合成单词
* "bing"。若从1开始计数的话，则‘b’ ‘i’ ‘n’ ‘g’这4个字母出现的位置分别为(4,5,6,
* 10) (4,5,9,10),(4,8,9,10)和(7,8,9,10)，故总共可以组合成4个单词”bing“。
* 	咱们的问题是：现给定任意字符串，只包含小写‘b’ ‘i’ ‘n’ ‘g’这4种字母，请问一共
* 能组合成多少个单词bing? 字符串长度不超过10000，由于结果可能比较大，请输出对
* 10^9 + 7取余数之后的结果。
*
* The recursion solution is the basic simple method to solve it, I comes out 
* another more efficency calculate to solve it.
* First set all character index ordered list, also it is ascending indexs.
* Then find every bigger number in the heap, it is O(log(n)) times to use.
* So the maximum use time is (4-1)*(n*n*n)*O(log(n)).
* 呃，这个貌似只是改进了最后一个count++吧，递归性质还是没有改变。
*
* @author firstboy0513
* @version 0.0.1
* @date 2014-01-21
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_CHAR_COUNT 128
#define WORD "bing"
#define WORD_TYPE_COUNT 4

/**
* @brief character index.
*
* @param s string to calculate character index.
* @param char_index input character index array.
* @param char_types_count input character types count.
* @param pchar_array output character indexs array point.
* @param pchar_count output character indexs count point.
*/
void index_character(const char* s, const int* char_index, 
	const int char_types_count, int*** pchar_array, int** pchar_count) {
	int i = 0, index = 0;
	int size = strlen(s);
	(*pchar_count) = (int*) calloc(char_types_count, sizeof(int));
	memset((*pchar_count), 0, char_types_count*sizeof(int));
	(*pchar_array) = (int**) calloc(char_types_count, sizeof(int*));
	for (i = 0; i < char_types_count; i ++) {
		(*pchar_array)[i] = (int*) calloc(size, sizeof(int));
		memset((*pchar_array)[i], 0, size*sizeof(int));
	}
	for (i = 0; i < size; i ++) {
		index = char_index[(int)(s[i])];
		if (index >= 0) {
			(*pchar_array)[ index ][ (*pchar_count)[ index ] ++ ] = i;
		}
	}
}

/**
* @brief set word index.
*
* @param word word to set. like "bing".
* @param char_index char indexs to store.
*/
void set_word_index(const char* word, int* char_index) {
	if (!word || !char_index) {
		printf("[ERR] -- set_word input invalid!\n");
		return;
	}
	int i = 0, size = strlen(word);
	for (i = 0; i < size; i ++) {
		char_index[(int)(word[i])] = i;
	}
}

#define MAx_COUNT 1000000007

/**
* @brief count "bing" word combines count use recursion method.
*
* @param char_array character index array.
* @param char_count character index count.
* @param index the start index to process.
* @param begin the begin minimum index to process, it require not less than 
* begin index, invoke it start with -1 let is start from 0.
*
* @return return the word combines count, be sure if it is greate than 10^9+7, 
* it will moddule 10^9+7 let it be a less number to store in a 4 bytes int type.
*/
int count_word_combines(int** char_array, int* char_count, const int index, 
	const int begin) {
	int count = 0;
	int i = 0;
	if (index == WORD_TYPE_COUNT-1) {
		for (i = 0; i < char_count[index]; i ++) {
			if (char_array[index][i] > begin) {
				count ++;
			}
		}
	} else {
		for (i = 0; i < char_count[index]; i ++) {
			if (char_array[index][i] > begin) {
				count += count_word_combines(char_array, char_count, index+1, 
						char_array[index][i]);
				if (count > MAx_COUNT) { count %= MAx_COUNT; }
			}
		}
	}
	return count;
}

/**
* @brief given a string to calculate the bing word count.
*
* @param s a string has "bing" characters.
*
* @return return the "bing" word combines count.
*/
int get_bing_word_count(const char* s) {
	// initialize
	int char_index[MAX_CHAR_COUNT] = {0};
	memset(char_index, -1, MAX_CHAR_COUNT*sizeof(int));
	set_word_index(WORD, char_index);

	// index character and count
	int** char_array = NULL; int* char_count = NULL;
	index_character(s, char_index, WORD_TYPE_COUNT, 
		&char_array, &char_count);
	int combine_count = count_word_combines(char_array, char_count, 0, -1);

	// free memory
	int i = 0;
	if (char_count) { free(char_count); }
	if (char_array) {
		for (i = 0; i < WORD_TYPE_COUNT; i ++) { free(char_array[i]); }
		free(char_array);
	}
	return combine_count;
}

#define TEST_STRING "iinbinbing"

int main(int argc, const char *argv[])
{
	int combine_count = get_bing_word_count(TEST_STRING);
	printf("combine count = %d\n", combine_count);
	return 0;
}
