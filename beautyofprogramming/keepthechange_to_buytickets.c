#include <stdio.h>
#include <stdlib.h>

/**
* @brief catalan number.
*
* @param N given input n.
*
* @return catalan value.
*/
int catalan(int N) {
	if (0 == N) { return 1; }
	int i = 0, sum = 0, n = N/2;
	for (i = 0; i < n; i ++) {
		sum += catalan(2*i) * catalan(2*(n - i -1));
	}
	return sum;
}

/**
* @brief test case for catalan.
*/
void testcase_catalan() {
	int N = 12;
	printf("There are %d case of order to buy tickets also as catalan.\n", 
		catalan(2*N));
}

/**
* @brief output range array.
*
* @param array array.
* @param size array size.
*/
void output_range_array(const int* array, const int size) {
	int i = 0;
	for (i = 0; i < size; i ++) {
		printf("%d", array[i]);
	}
}

#define SWAP(a,b) { (a) ^= (b); (b) ^= (a); (a) ^= (b); }

/**
* @brief judge the array[end] has the same element from array[start] to 
* array[end-1].
*
* @param array array.
* @param start start index.
* @param end end index.
*
* @return return 1 means has repeat element, otherwise means no repeat.
*/
int has_repeat(int* array, int start, int end) {
	int i = 0;
	for (i = start; i < end; i ++) {
		if (array[i] == array[end]) {
			return 1;
		}
	}
	return 0;
}

/**
* @brief print out all range numbers.
*
* @param array array of numbers.
* @param size array size.
* @param start start change index.
* @param isrepeat default is 1 can repeat, if input 0 means deduplication.
*/
void allrange(int* array, int size, int start, int repeat) {
	if (!array || size <= 0 || size <= start) {
		printf("[ERR] -- allrange input invalid!\n");
		return;
	}
	if (start == size-1) {
		output_range_array(array, size); printf("\n");
		return;
	}
	int i = 0;
	allrange(array, size, start+1, repeat);
	for (i = start+1; i < size; i ++) {
		if (repeat || (!repeat && !has_repeat(array, start, i))) {
			SWAP(array[start], array[i]);
			allrange(array, size, start+1, repeat);
			SWAP(array[start], array[i]);
		}
	}
}

/**
* @brief test case or all range.
*/
void testcase_allrange() {
	int array[] = {1, 2, 3, 2};
	int size = sizeof(array) / sizeof(array[0]);
	allrange(array, size, 0, 0);
}

int main(int argc, const char *argv[])
{
	testcase_catalan();
	testcase_allrange();

	return 0;
}
