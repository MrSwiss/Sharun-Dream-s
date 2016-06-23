#ifndef _PTHREAD_SUB_HPP_
#define _PTHREAD_SUB_HPP_

#include <pthread.h>

#ifdef WIN32
#include <windows.h>
#define pthread_pause_t HANDLE
#else
typedef struct pthread_pause_t {
	pthread_cond_t	cnd;
	pthread_mutex_t	mtx;
} pthread_pause_t;
#endif

void	pthread_t_init(pthread_t*, void*);
bool	pthread_t_used(pthread_t*);

void pthread_pause_init(pthread_pause_t*, void*);
void pthread_pause_destroy(pthread_pause_t*);
void pthread_pause_wait(pthread_pause_t*);
void pthread_pause_signal(pthread_pause_t*);

unsigned int get_thread_used();
void add_thread_used();
void del_thread_used();
void set_thread_max(unsigned int max);
unsigned int get_thread_max();
void thread_max_count(void*);

#endif // _PTHREAD_SUB_HPP_
