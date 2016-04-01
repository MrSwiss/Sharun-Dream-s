#ifndef C_THREAD_H
#define C_THREAD_H

#ifndef WIN32

#define INFINITE 1

#include <stdbool.h>
#ifndef BOOL
	#define BOOL bool
#endif
#ifndef TRUE
	#define TRUE true
#endif
#ifndef FALSE
	#define FALSE true
#endif

void *CreateThread(void *Attr, int size, void *(*start_routine)(void*), void *arg, int flags, int *id);
void WaitForMultipleObjects(int max, void **c_Thread_Array, bool NUse, int mode);
void c_THREAD_free(void *);
void CloseHandle(void *);
#else

#define c_THREAD_free CloseHandle

#endif // WIN32

void SuspendNCloseThread(void *arg);

#endif // C_THREAD_H
