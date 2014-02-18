#include <stdio.h>
#include <time.h>

int main(int argc, const char *argv[])
{
	time_t t = time(NULL);
	printf("time(NULL) = %ld\n", t);

	return 0;
}
