#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>

void print_truncate_errno(void) {
    switch(errno) {
        case EACCES:       printf("ftruncate error EACCES      \n"); break;
        case EFAULT:       printf("ftruncate error EFAULT      \n"); break;
        case EFBIG:        printf("ftruncate error EFBIG       \n"); break;
        case EINTR:        printf("ftruncate error EINTR       \n"); break;
        case EINVAL:       printf("ftruncate error EINVAL      \n"); break;
        case EIO:          printf("ftruncate error EIO         \n"); break;
        case EISDIR:       printf("ftruncate error EISDIR      \n"); break;
        case ELOOP:        printf("ftruncate error ELOOP       \n"); break;
        case ENAMETOOLONG: printf("ftruncate error ENAMETOOLONG\n"); break;
        case ENOENT:       printf("ftruncate error ENOENT      \n"); break;
        case ENOTDIR:      printf("ftruncate error ENOTDIR     \n"); break;
        case EPERM:        printf("ftruncate error EPERM       \n"); break;
        case EROFS:        printf("ftruncate error EROFS       \n"); break;
        case ETXTBSY:      printf("ftruncate error ETXTBSY     \n"); break;
        case EBADF:        printf("ftruncate error EBADF       \n"); break;
        default:           printf("ftruncate errno unknown     \n"); break;
    }
}

int main(int argc, const char *argv[])
{
    if (-1 == truncate("file", 10000)) {
		print_truncate_errno();
    }
    int fd = open("file", O_RDWR);
	if (-1 == fd) {
		printf("open file failed!\n"); return -1;
	}
    if (-1 == ftruncate(fd, 20000)) {
		print_truncate_errno();
    }
    close(fd);
    return 0;
}
