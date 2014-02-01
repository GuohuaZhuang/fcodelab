#include <stdio.h>
#include <stdlib.h>

int previous_index(const int x, const int m) {
	return (0 == x%2 ? x/2 : (x-1)/2+m);
}

/// int front_index(const int i, const int n) {
/// 	return ((2*i)%(n-1));
/// }
/// 
/// void circle_set_test(int* A, const int m, const int index) {
/// 	int buff = A[index], x = index, y = front_index(index, m*2);
/// 	do {
/// 		A[x] = A[previous_index(x, m)];
/// 		x = previous_index(x, m);
/// 	} while (x != y);
/// 	A[y] = buff;
/// }
/// 
/// void perfect_shuffl_test(int* A, const int n) {
/// 	if (0 != n%2) {
/// 		printf("[ERR] perfect_shuffl input invalid!\n");
/// 		return;
/// 	}
/// 	if (n <= 2) { return; }
/// 	const int m = n/2;
/// 	circle_set_test(A, m, 1);
/// }

void perfect_shuffl(int* A, const int n) {
	if (0 != n%2) {
		printf("[ERR] perfect_shuffl input invalid!\n");
		return;
	}
	if (n <= 2) { return; }
	const int m = n/2;
	int buff = A[1], x = 1;
	do {
		A[x] = A[previous_index(x, m)];
		x = previous_index(x, m);
	} while (x != 2);
	A[2] = buff;
}

void output_array(const int* array, const int size) {
	int i = 0;
	for (i = 0; i < size; i ++) {
		if (0 != i) { printf(", "); }
		printf("%d", array[i]);
	}
	printf("\n");
}

int main(int argc, const char *argv[])
{
//	int array[] = {1, 2, 3, 4, 5, 6, -1, -2, -3, -4, -5, -6};
//	const int size = sizeof(array) / sizeof(array[0]);
//	perfect_shuffl(array, size);
//	output_array(array, size);

	/// Your method is falt!!! like m == 4
	int m = 4, i = 0;
	int* array = (int*) calloc(m*2, sizeof(int));
	for (i = 0; i < m; i ++) {
		array[i] = i+1;
		array[i+m] = -(i+1);
	}
	perfect_shuffl_test(array, m*2);
	output_array(array, m*2);

	free(array);

	return 0;
}
