#include <malloc.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>

/* The key used to associate a log file pointer with each thread. */
static pthread_key_t thread_log_key;
static pthread_key_t thread_msg_key;

/* Write MESSAGE to the log file for the current thread. */
void write_to_thread_log (const char* message, unsigned int id)
{
	FILE* thread_log = (FILE*) pthread_getspecific (thread_log_key);
	char* thread_msg = (char*) pthread_getspecific (thread_msg_key);
	fprintf (thread_log, "%s with %d\n", message, (int)id);
	fprintf (thread_log, "%s\n", thread_msg);
}

/* Close the log file pointer THREAD_LOG. */
void close_thread_log (void* thread_log)
{
	fclose ((FILE*) thread_log);
}

void free_thread_msg (void* thead_msg)
{
	free ((char*) thead_msg);
}

void* thread_function (void* args)
{
	char thread_log_filename[20];
	FILE* thread_log;
	/* Generate the filename for this thread’s log file. */
	sprintf (thread_log_filename, "thread%d.log", (int) pthread_self ());
	/* Open the log file. */
	thread_log = fopen (thread_log_filename, "w");
	/* Store the file pointer in thread-specific data under thread_log_key. */
	pthread_setspecific (thread_log_key, thread_log);
	char* thread_msg = strdup(thread_log_filename);
	pthread_setspecific (thread_msg_key, thread_msg);

	write_to_thread_log ("Thread starting.", pthread_self());
	/* Do work here... */
	return NULL;
}

int main (int argc, char* argv[])
{
	int i;
	pthread_t threads[5];
	/* Create a key to associate thread log file pointers in
	   thread-specific data. Use close_thread_log to clean up the file
	   pointers. */
	pthread_key_create (&thread_log_key, close_thread_log);
	pthread_key_create (&thread_msg_key, free_thread_msg);
	/* Create threads to do the work. */
	for (i = 0; i < 5; ++i)
		pthread_create (&(threads[i]), NULL, thread_function, NULL);
	/* Wait for all threads to finish. */
	for (i = 0; i < 5; ++i)
		pthread_join (threads[i], NULL);
	return 0;
}
