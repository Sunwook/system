
#define _GNU_SOURCE
#include <stdio.h>       /* standard I/O routines                 */ 
#include <pthread.h>     /* pthread functions and data structures */ 
#include <stdlib.h>
#include <errno.h>


//pthread_mutex_t a_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t a_mutex;

int isThreadStarted = 0;
int isTryLocked = 0;

void* 
do_loop(void* data) 
{

	printf("Thread started!\n");

	int rc; /* contain mutex lock/unlock results */ 

	rc = pthread_mutex_lock(&a_mutex);

	printf("Thread locked!\n");

	isThreadStarted = 1;

	while (!isTryLocked) {}

	rc = pthread_mutex_unlock(&a_mutex); 
	
	printf("Thread unlocked!\n");
}

int 
main(void)
{   
	int        thr_id1;        /* thread ID for the first new thread    */ 
	int rc; /* contain mutex lock/unlock results */ 
	pthread_t  p_thread1;      /* first thread's structure              */ 
	pthread_mutexattr_t attr;

	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(&a_mutex, &attr);
	
	printf("Main: created Thread!\n");

	/* create a new thread that will execute 'do_loop()' with '1'       */ 
	thr_id1 = pthread_create(&p_thread1, NULL, do_loop, NULL); 

	while (!isThreadStarted) {}

	rc = pthread_mutex_trylock(&a_mutex);

	printf("Main: trylocked!\n");

	isTryLocked = 1;

	printf("result = %d\n", rc);

	printf("EBUSY = %d\n", EBUSY);

	pthread_join(p_thread1, NULL);

	printf("Main: Thread joined!\n");

	return 0;
}

