#ifndef _FIXED_THREAD_HPP_
#define _FIXED_THREAD_HPP_

#include "internal/thread.hpp"

typedef struct Fixed_thread_t {
	thread_list*	thread;
	Fixed_thread_t*	next;
	Fixed_thread_t*	prev;
} Fixed_thread_t;

typedef struct Fixed_thread_W_t {
	bool init = false;
	pthread_mutex_t	mtx;
	Fixed_thread_t*	threads[2] = {NULL, NULL};
} Fixed_thread_W_t;

void Fixed_thread_Close();
void Fixed_thread_Close_Add(void (*)());

void Fixed_thread_Cleanup(Fixed_thread_W_t*);
void Fixed_thread_Signal(Fixed_thread_W_t*);
Fixed_thread_t *Fixed_thread_Add(void (*)(), Fixed_thread_W_t*, thread_list*);
void Fixed_thread_Del(Fixed_thread_W_t*, Fixed_thread_t*);

#endif // _FIXED_THREAD_HPP_
