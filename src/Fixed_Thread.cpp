#include "Sharun.hpp"
#include "internal/Fixed_Thread.hpp"

#include <vector>

bool Fixed_thread_Close_init = false;
pthread_mutex_t Fixed_thread_Close_mtx;
std::vector<void (*)()> Fixed_thread_Close_list;

void Fixed_thread_Close()
{
	pthread_mutex_lock(&Fixed_thread_Close_mtx);
	while (!Fixed_thread_Close_list.empty()) {
		Fixed_thread_Close_list.back()();
		Fixed_thread_Close_list.pop_back();
	}
	pthread_mutex_unlock(&Fixed_thread_Close_mtx);
	pthread_mutex_destroy(&Fixed_thread_Close_mtx);
	Fixed_thread_Close_init = false;
}

void Fixed_thread_Close_Add(void (*cleanup)())
{
	if (!Fixed_thread_Close_init) {
		pthread_mutex_init(&Fixed_thread_Close_mtx, NULL);
		Fixed_thread_Close_init = true;
	}
	pthread_mutex_lock(&Fixed_thread_Close_mtx);
	int i = Fixed_thread_Close_list.size(); 
	while (i > 0) {
		i--;
		if (cleanup == Fixed_thread_Close_list[i])
			return;
	}
	Fixed_thread_Close_list.push_back(cleanup);
	pthread_mutex_unlock(&Fixed_thread_Close_mtx);
}

Fixed_thread_t *create_Fixed_thread_t(thread_list *my_thread)
{
	Fixed_thread_t *Fixed_thread_l = new Fixed_thread_t;
	Fixed_thread_l->next = NULL;
	Fixed_thread_l->prev = NULL;
	Fixed_thread_l->thread = my_thread;
	return Fixed_thread_l;
}

void Fixed_thread_Cleanup(Fixed_thread_W_t* void_thread)
{
	if (void_thread->init) {
		pthread_mutex_lock(&void_thread->mtx);
		while (void_thread->threads[0]) {
			pthread_mutex_unlock(&void_thread->mtx);
			void_thread->threads[0]->thread->join();
			pthread_mutex_lock(&void_thread->mtx);
		}
		pthread_mutex_unlock(&void_thread->mtx);
		pthread_mutex_destroy(&void_thread->mtx);
	}
	void_thread->init = false;
}

void Fixed_thread_Signal(Fixed_thread_W_t* void_thread)
{
	for (Fixed_thread_t *thread_l = void_thread->threads[0] ; thread_l ; ) {
		thread_l->thread->signal();
		thread_l = thread_l->next;
	}
}

Fixed_thread_t *Fixed_thread_Add(void (*cleanup)(), Fixed_thread_W_t* void_thread, thread_list* my_thread)
{
	if (!void_thread->init) {
		void_thread->init = true;
		pthread_mutex_init(&void_thread->mtx, NULL);
		Fixed_thread_Close_Add(cleanup);
	}
	Fixed_thread_t *Fixed_thread_l = create_Fixed_thread_t(my_thread);
	pthread_mutex_lock(&void_thread->mtx);
	if (!void_thread->threads[0])
		void_thread->threads[0] = void_thread->threads[1] = Fixed_thread_l;
	else {
		void_thread->threads[1]->next = Fixed_thread_l;
		Fixed_thread_l->prev = void_thread->threads[1];
		void_thread->threads[1] = void_thread->threads[1]->next;
	}
	pthread_mutex_unlock(&void_thread->mtx);
	return Fixed_thread_l;
}

void Fixed_thread_Del(Fixed_thread_W_t* void_thread, Fixed_thread_t* Fixed_thread_l)
{
	thread_list* my_thread = Fixed_thread_l->thread;
	pthread_mutex_lock(&void_thread->mtx);
	if (Fixed_thread_l->prev)
		Fixed_thread_l->prev->next = Fixed_thread_l->next;
	else
		void_thread->threads[0] = Fixed_thread_l->next;
	if (Fixed_thread_l->next)
		Fixed_thread_l->next->prev = Fixed_thread_l->prev;
	else
		void_thread->threads[1] = Fixed_thread_l->prev;
	pthread_mutex_unlock(&void_thread->mtx);
	delete Fixed_thread_l;
	my_thread->internal_delete();
}
