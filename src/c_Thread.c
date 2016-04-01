#ifndef WIN32

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "c_Thread.h"


void *CreateThread(void *Attr, int size, void *(*start_routine)(void*), void *arg, int flags, int *id)
{
	long thd = 0;
	pthread_create((pthread_t*)&thd, (const pthread_attr_t *)Attr, start_routine, arg);
	return (void*)thd;
}

void WaitForMultipleObjects(int max, void **c_Thread_Array, bool NUse, int mode)
{
	long *thd = (long*)c_Thread_Array;
	for (int i=0; i<max; i++)
		pthread_join(thd[i], NULL);
}
void c_THREAD_free(void *arg)
{
	pthread_exit (arg);
}

void CloseHandle(void *arg)
{
	long thd = (long)arg;
	if (thd) pthread_join(thd, NULL);
}

#endif // WIN32
