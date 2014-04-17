#include <stdio.h>
#include <stdlib.h>

#define MAX_N 10

int BIT[MAX_N+1] = {};
int A[MAX_N+1] = {};

int lowbit(int x)
{
    return x&(-x);
}

void build()
{
    int i=1;
    int j = 0;
    for (;i<=MAX_N;i++)
    {
        BIT[i]=A[i];
        for (j=i-lowbit(i);j>0;j-=lowbit(j))
            BIT[i]+=BIT[j];
    }
}

int sum (int k)
{
    int ret = 0;
    int i = k;
    for (; i>0; i-=lowbit(i))
        ret+=BIT[i];
    return ret;
}

int main(int argc, const char *argv[])
{
    int _A[MAX_N] = {23, 35, 28, 85, 99, 21, 65, 24, 56, 5};
    int i = 0;
    for (i = 0; i < MAX_N; i ++) {
        A[i+1] = _A[i];
    }
    build();
    printf("sum = %d\n", sum(3));
    return 0;
}
