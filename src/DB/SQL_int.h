#include "globale.h"

#ifdef WIN32
	#include <windows.h>
	#ifndef sleep
		#define sleep(a) Sleep(a * 1000)
	#endif
#else
	#include "c_Thread.h"
#endif

typedef struct SQL_QUEUE {
	char *Query;
	void *Response;
	void *finished;
	char result;
	void *Next;
} SQL_QUEUE;

SQL_QUEUE *SQL_NewQueue(char *Query);

int		SQL_Mutex_Init();
void	SQL_Mutex_Close();
void	SQL_Mutex_Lock(bool fast);
void	SQL_Mutex_Unlock(bool fast);

void	*Mutex_Init();
void	Mutex_Close(void *);
void	Mutex_Lock(void *);
void	Mutex_Unlock(void *);
void	Mutex_Pause(void *);
void	Mutex_Resume(void *);
