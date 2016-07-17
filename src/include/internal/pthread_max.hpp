#ifndef _PTHREAD_MAX_HPP_
#define _PTHREAD_MAX_HPP_

#include "pthread_sub.hpp"

unsigned int get_thread_used();
void add_thread_used();
void del_thread_used();
void set_thread_max(unsigned int max);
unsigned int get_thread_max();
void thread_max_count(void*);

#endif // _PTHREAD_MAX_HPP_
