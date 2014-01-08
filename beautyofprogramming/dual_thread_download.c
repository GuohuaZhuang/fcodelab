/* Copyright (C) 
* 2014 - firstboy0513
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
* 
*/
/**
* @file dual_thread_download.c
* @brief dual thread download lmitate program.
* @author firstboy0513
* @version 0.0.1
* @date 2014-01-08
*/
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <string.h>

#define BUFFER_COUNT 10

/**
* @brief global struct.
*/
typedef struct _shared_t {
	int buffer[BUFFER_COUNT];    // share buffer
	int in;                      // put in index
	int out;                     // put out index
	int download_complete;       // flag of download completed or not
	sem_t full;                  // full semaphore
	sem_t empty;                 // empty semaphore
	pthread_mutex_t mutex;       // mutex to write buffer
	int fake_number;             // lmitate fake number to control end state
} shared_t;

shared_t g_shared;// = {{0}, 0, 0, {0}, {0}, PTHREAD_MUTEX_INITIALIZER};

/**
* @brief lmitate get block from net.
*
* @param p buff element point to store block value from net.
*
* @return return 1 means completed, otherwise return 0 means not completed.
*/
int get_block_from_net(int* p) {
	pthread_mutex_lock(&g_shared.mutex);
	*p = ++ g_shared.fake_number;
	pthread_mutex_unlock(&g_shared.mutex);

	printf("get %d block from net\n", *p);
	if (g_shared.fake_number >= 20) { return 1; }
	return 0;
}

/**
* @brief lmitate write block do disk.
*
* @param p buff element point to write.
*/
void write_block_to_disk(int* p) {
	printf("write %d block to disk ...\n", *p);

	pthread_mutex_lock(&g_shared.mutex);
	*p = 0;
	pthread_mutex_unlock(&g_shared.mutex);
}

/**
* @brief thread for get block from net.
*
* @param arg thread argument.
*
* @return thread return.
*/
void* procA(void* arg) {
	while (1) {
		sem_wait(&g_shared.empty);
		g_shared.download_complete 
			= get_block_from_net(g_shared.buffer + g_shared.in);
		g_shared.in = (g_shared.in + 1) % BUFFER_COUNT;
		sem_post(&g_shared.full);
		if (g_shared.download_complete) { break; }
	}
	return NULL;
}

/**
* @brief thread B for write block to disk.
*
* @param arg thread argument.
*
* @return thread return.
*/
void* procB(void* arg) {
	while (1) {
		sem_wait(&g_shared.full);
		write_block_to_disk(g_shared.buffer + g_shared.out);
		g_shared.out = (g_shared.out + 1) % BUFFER_COUNT;
		sem_post(&g_shared.empty);
		if (g_shared.download_complete 
				&& g_shared.out == g_shared.in) { break; }
	}
	return NULL;
}

int main(int argc, const char *argv[])
{
	memset((void*)&g_shared, 0, sizeof(shared_t));
	sem_init(&g_shared.empty, 0, BUFFER_COUNT);
	sem_init(&g_shared.full, 0, 0);

	pthread_t pa, pb;
	if (pthread_create(&pa, NULL, procA, NULL) != 0) {
		printf("pthread error!\n");
	}
	if (pthread_create(&pb, NULL, procB, NULL) != 0) {
		printf("pthread error!\n");
	}

	pthread_join(pa, NULL);
	pthread_join(pb, NULL);
	sem_destroy(&g_shared.empty);
	sem_destroy(&g_shared.full);
	return 0;
}
