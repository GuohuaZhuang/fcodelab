#include <stdio.h>

int main(int argc, const char *argv[])
{
	int a = 1;
	printf("a = %d\n", a);

	char buf[sizeof(int)];
	*(int*)buf = 0x6f1;
	printf("%d\n", buf[0]);

	return 0;
}
