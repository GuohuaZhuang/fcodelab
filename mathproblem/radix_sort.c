#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void outprint(int* arr, int size) {
    int i = 0;
    printf("arr[]: ");
    for (i = 0; i < size; i ++) {
        if (0 != i) { printf(", "); }
        printf("%d", arr[i]);
    }
    printf("\n");
}

void radixsort(int* arr, int size) {
    if (!arr || size <= 0) { return; }
    int i = 0, max = arr[0], exp = 1, BASE = 10;
    int *tarr = (int*) malloc(sizeof(int) * size);
    int *bucket = (int*) malloc(sizeof(int) * BASE);
    memset(tarr, 0, sizeof(int) * size);
    for (i = 1; i < size; i ++) {
        if (max < arr[i]) { max = arr[i]; }
    }
    while (max / exp > 0) {
        memset(bucket, 0, sizeof(int) * BASE);
        for (i = 0; i < size; i ++) {
            bucket[ (arr[i] / exp) % BASE ] ++;
        }
        for (i = 1; i < size; i ++) {
            bucket[i] += bucket[i-1];
        }
        for (i = size-1; i >= 0; i --) {
            tarr[ --bucket[ (arr[i] / exp) % BASE] ] = arr[i];
        }
        for (i = 0; i < size; i ++) {
            arr[i] = tarr[i];
        }
        exp *= BASE;
    }
    free(tarr);
    free(bucket);
}

int main(int argc, const char *argv[])
{
    int arr[] = {32, 53, 23, 8, 92, 29, 5, 99, 24, 9};
    int size = sizeof(arr) / sizeof(arr[0]);
    radixsort(&arr[0], size);
    outprint(&arr[0], size);

    return 0;
}
