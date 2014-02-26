#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void* thread_function(void* args) {
    pid_t pid;
    pthread_t tid;
    pid = getpid();
    tid = pthread_self();
    printf("subthread pid %u tid %u (0x%x)\n", (unsigned int)pid,
            (unsigned int)tid, (unsigned int)tid);
	return NULL;
}

int main(int argc, char* argv[])
{
    pid_t pid;
    pthread_t tid;
    pid = getpid();
    tid = pthread_self();
    printf("mainthread: pid %u tid %u (0x%x)\n", (unsigned int)pid,
            (unsigned int)tid, (unsigned int)tid);
	
	int i = 0;
	pthread_t threads[5];
	for (i = 0; i < 5; i ++) {
		pthread_create(&(threads[i]), NULL, thread_function, NULL);
	}
	for (i = 0; i < 5; i ++) {
		pthread_join(threads[i], NULL);
	}

    return 0;
}
