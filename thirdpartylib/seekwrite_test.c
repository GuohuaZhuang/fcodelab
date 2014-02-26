#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, const char *argv[])
{
	int fd = open("seekwrite.file", O_RDWR | O_CREAT, 0644);
	char* buf = "the end";
	int off = 10000;
	printf("lseek return %d.\n", (int)lseek(fd, off, SEEK_SET));
	printf("write %d bytes in file.\n", (int)write(fd, buf, strlen(buf)));
	close(fd);
	return 0;
}
