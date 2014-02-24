#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>

void testcase_mmap_shared() {
	const char* s1 = "the first string";
	const char* s2 = "the second string";

	char* mshared = mmap(NULL, 1024, PROT_READ|PROT_WRITE, 
		MAP_ANONYMOUS|MAP_SHARED, -1, 0);
	if (MAP_FAILED == mshared) {
		printf("mmap failed!\n");
		return;
	}

	strcpy(mshared, s1);

	int parentpid = getpid();
	printf("parentpid(%d) mshared: %s\n", parentpid, mshared);

	pid_t child = fork();
	switch (child) {
		case -1: {
			printf("fork failed!\n"); break;
		}
		case 0: {
			int childpid = getpid();
			printf("childpid(%d) mshared: %s\n", childpid, mshared);
			sleep(2);
			printf("childpid(%d) mshared: %s\n", childpid, mshared);
			munmap(mshared, 1024);
			exit(0);
		}
	}

	sleep(1);
	strcpy(mshared, s2);
	printf("parentpid(%d) mshared: %s\n", parentpid, mshared);

	munmap(mshared, 1024);
	sleep(2);
}

void testcase_mmap_file() {
	int fd = open("txt", O_RDONLY);
	struct stat st;
	fstat(fd, &st);
	char* filemap = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	printf("filemap: %s\n", filemap);
	munmap(filemap, st.st_size);
}

int main(int argc, const char *argv[])
{
	testcase_mmap_shared();
	testcase_mmap_file();

	return 0;
}
