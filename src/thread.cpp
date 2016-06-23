#include "internal/thread.hpp"

thread_list* thread_L = NULL;
thread_list* thread_L_last = NULL;
bool thread_L_mtx_init = false;
pthread_mutex_t thread_L_mtx;

void thread_L_cleanup()
{
	if (thread_L_mtx_init) {
		pthread_mutex_lock(&thread_L_mtx);
		while (thread_L) {
			pthread_mutex_unlock(&thread_L_mtx);
			delete thread_L;
			pthread_mutex_lock(&thread_L_mtx);
		}
		pthread_mutex_unlock(&thread_L_mtx);
		pthread_mutex_destroy(&thread_L_mtx);
	}
	thread_L_mtx_init = false;
}

thread_list::thread_list()
{
	if (!thread_L_mtx_init) {
		thread_L_mtx_init = true;
		pthread_mutex_init(&thread_L_mtx, NULL);
	}
	run = true;
	next = NULL;
	prev = NULL;
	mother = NULL;
	detached = false;
	pthread_t_init(&id, NULL);
	pthread_pause_init(&pause, NULL);
	pthread_mutex_lock(&thread_L_mtx);
	if (!thread_L)
		thread_L = thread_L_last = this;
	else {
		thread_L_last->next = this;
		prev = thread_L_last;
		thread_L_last = this;
	}
	pthread_mutex_unlock(&thread_L_mtx);
	add_thread_used();
}

thread_list::~thread_list()
{
/*	if (mother)
		delete mother;*/
	pthread_mutex_lock(&thread_L_mtx);
	if (prev)
		prev->next = next;
	else
		thread_L = next;
	if (next)
		next->prev = prev;
	detached = false;
	pthread_mutex_unlock(&thread_L_mtx);
	pthread_pause_destroy(&pause);
	del_thread_used();
}

void thread_list::internal_delete()
{
	mother = NULL;
	delete this;
}

bool thread_list::start(void*start_routine, void* mother_t)
{
	mother = mother_t;
	if (pthread_create(&id, NULL, (void*(*)(void*))start_routine, mother))
		detached = false;
	else
		detached = true;
	return detached;
}

void thread_list::wait()
{
	pthread_pause_wait(&pause);
}

void thread_list::signal()
{
	pthread_pause_signal(&pause);
}

void thread_list::join()
{
	run = false;
	if (detached)
		signal();
	pthread_join(id, NULL);
}
