#include "pthread_sub.hpp"

#include <unistd.h>

void pthread_t_init(pthread_t *pt, void *useless)
{
#ifdef PTW32_VERSION
	pt->p = NULL;
	pt->x = 0;
#else
	*pt = 0;
#endif
}

bool pthread_t_used(pthread_t *pt)
{
#ifdef PTW32_VERSION
	if (pt->p) return true;
#else
	if (*pt) return true;
#endif
	return false;
}

void pthread_pause_init(pthread_pause_t *ppt, void *useless)
{
#ifdef WIN32
	*ppt = CreateEvent(NULL, TRUE, FALSE, NULL);
#else
	pthread_mutex_init(&ppt->mtx, NULL);
	pthread_cond_init(&ppt->cnd, NULL);
#endif
}

void pthread_pause_destroy(pthread_pause_t *ppt)
{
#ifdef WIN32
	CloseHandle(*ppt);
#else
	pthread_mutex_destroy(&ppt->mtx);
	pthread_cond_destroy(&ppt->cnd);
#endif
}

void pthread_pause_wait(pthread_pause_t *ppt)
{
#ifdef WIN32
	WaitForSingleObject(*ppt, INFINITE);
	ResetEvent(*ppt);
#else
	pthread_cond_wait(&ppt->cnd, &ppt->mtx);
#endif
}

void pthread_pause_signal(pthread_pause_t *ppt)
{
#ifdef WIN32
	SetEvent(*ppt);
#else
	pthread_cond_signal(&ppt->cnd);
#endif
}
