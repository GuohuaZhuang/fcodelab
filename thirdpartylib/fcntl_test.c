#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define FILENAME "book"

int main(int argc, const char *argv[])
{
	int fd = open(FILENAME, O_RDWR, 0644);
	struct flock lock;
	memset(&lock, 0, sizeof(struct flock));
	if (-1 == fcntl(fd, F_SETLK, &lock)) {
		printf("fcntl 1 return error!\n"); return -1;
	}
	printf("now the file '%s' is locked, press enter to unlock it.", FILENAME);
	getchar();
	lock.l_type = F_UNLCK;
	if (-1 == fcntl(fd, F_SETLK, &lock)) {
		printf("fcntl 2 return error!\n"); return -1;
	}
	printf("now the file '%s' is unlocked, press enter to exit.", FILENAME);
	getchar();

	close(fd);

	return 0;
}
