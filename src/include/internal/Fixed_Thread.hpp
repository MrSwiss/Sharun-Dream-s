#ifndef _FIXED_THREAD_HPP_
#define _FIXED_THREAD_HPP_

#include "internal/thread.hpp"

typedef struct Fixed_thread_t {
	thread_list*	thread;
	Fixed_thread_t*	next;
	Fixed_thread_t*	prev;
} Fixed_thread_t;

void Fixed_thread_Close();
void Fixed_thread_Close_Add(void (*cleanup)());

Fixed_thread_t *create_Fixed_thread_t(thread_list*);
void Fixed_thread_Cleanup(pthread_mutex_t*, Fixed_thread_t**, bool*);
void Fixed_thread_Signal(Fixed_thread_t**);
Fixed_thread_t *Fixed_thread_Add(void (*)(), pthread_mutex_t*, Fixed_thread_t**, thread_list*, bool*);
void Fixed_thread_Del(pthread_mutex_t*, Fixed_thread_t**, Fixed_thread_t*);

#endif // _FIXED_THREAD_HPP_
