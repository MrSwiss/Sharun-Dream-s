#include "internal/pthread_sub.hpp"

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


/****************************************************************************
 * 
 *	Max Thread Counter Part
 * 
*****************************************************************************/

typedef struct thread_max_count_t {
	pthread_t	pt;
	thread_max_count_t*	next;
} thread_max_count_t;

unsigned int thread_used = 0;
unsigned int thread_max = 0;

unsigned int get_thread_used()
{
	return thread_used;
}
void add_thread_used()
{
	thread_used++;
}
void del_thread_used()
{
	thread_used--;
}

void set_thread_max(unsigned int max)
{
	thread_max = max;
}

unsigned int get_thread_max()
{
	return thread_max;
}

void thread_max_count(void *mtx)
{
	if (mtx) {
		pthread_mutex_lock((pthread_mutex_t*)mtx);
		pthread_mutex_unlock((pthread_mutex_t*)mtx);
		pthread_exit(NULL);
	} else {
		thread_max = 0;
		thread_max_count_t *l_test_thread = NULL;
		thread_max_count_t *test_thread_t = NULL;
		pthread_mutex_t thread_mtx;
		pthread_mutex_init(&thread_mtx, NULL);
		pthread_mutex_lock(&thread_mtx);
		do {
			if (!test_thread_t) {
				test_thread_t = new thread_max_count_t;
				l_test_thread = test_thread_t;
			} else {
				test_thread_t->next = new thread_max_count_t;
				test_thread_t = test_thread_t->next;
			}
			test_thread_t->next = NULL;
			thread_max++;
		} while (!pthread_create(&test_thread_t->pt, NULL, (void*(*)(void*))thread_max_count, &thread_mtx));
		thread_max--;
		pthread_mutex_unlock(&thread_mtx);
		for (unsigned int i=0; i<thread_max; i++) {
			test_thread_t = l_test_thread;
			l_test_thread = l_test_thread->next;
			pthread_join(test_thread_t->pt, NULL);
			delete test_thread_t;
		}
		while (l_test_thread) {
			test_thread_t = l_test_thread;
			l_test_thread = l_test_thread->next;
			delete l_test_thread;
		}
		pthread_mutex_destroy(&thread_mtx);
	}
}
