#include "Sharun.hpp"
#include "internal/Fixed_Thread.hpp"

void GameD_Cleanup();
void HTTP_Cleanup();

typedef struct Fixed_thread_Close_t {
	void (*Cleanup)();
	Fixed_thread_Close_t* next;
} Fixed_thread_Close_t;

Fixed_thread_Close_t *Fixed_thread_Close_list = NULL;

void Fixed_thread_Close()
{
	Fixed_thread_Close_t *Cleanup_t;
	while (Fixed_thread_Close_list) {
		Cleanup_t = Fixed_thread_Close_list;
		Fixed_thread_Close_list = Fixed_thread_Close_list->next;
		Cleanup_t->Cleanup();
		delete Cleanup_t;
	}
}

void Fixed_thread_Close_Add(void (*cleanup)())
{
	Fixed_thread_Close_t *Cleanup_t = new Fixed_thread_Close_t;
	Cleanup_t->next = NULL;
	Cleanup_t->Cleanup = cleanup;
	if (!Fixed_thread_Close_list)
		Fixed_thread_Close_list = Cleanup_t;
	else {
		Fixed_thread_Close_t *Fixed_thread_Close_lt = Fixed_thread_Close_list;
		while (Fixed_thread_Close_lt->next && Fixed_thread_Close_lt->Cleanup != cleanup)
			Fixed_thread_Close_lt = Fixed_thread_Close_lt->next;
		if (Fixed_thread_Close_lt->Cleanup == cleanup)
			delete Cleanup_t;
		else
			Fixed_thread_Close_lt->next = Cleanup_t;
		
	}
}

Fixed_thread_t *create_Fixed_thread_t(thread_list *my_thread)
{
	Fixed_thread_t *Fixed_thread_l = new Fixed_thread_t;
	Fixed_thread_l->next = NULL;
	Fixed_thread_l->prev = NULL;
	Fixed_thread_l->thread = my_thread;
	return Fixed_thread_l;
}

void Fixed_thread_Cleanup(pthread_mutex_t *mtx, Fixed_thread_t **thread, bool *mtx_init)
{
	if (*mtx_init) {
		pthread_mutex_lock(mtx);
		while (*thread) {
			pthread_mutex_unlock(mtx);
			(*thread)->thread->join();
			pthread_mutex_lock(mtx);
		}
		pthread_mutex_unlock(mtx);
		pthread_mutex_destroy(mtx);
	}
	*mtx_init = false;
}

void Fixed_thread_Signal(Fixed_thread_t **thread)
{
	for (Fixed_thread_t *thread_l = thread[0] ; thread_l ; ) {
		thread_l->thread->signal();
		thread_l = thread_l->next;
	}
}

Fixed_thread_t *Fixed_thread_Add(void (*cleanup)(), pthread_mutex_t *mtx, Fixed_thread_t** voit_thread, thread_list* my_thread, bool *mtx_init)
{
	if (!*mtx_init) {
		*mtx_init = true;
		pthread_mutex_init(mtx, NULL);
		Fixed_thread_Close_Add(cleanup);
	}
	Fixed_thread_t *Fixed_thread_l = create_Fixed_thread_t(my_thread);
	pthread_mutex_lock(mtx);
	if (!voit_thread[0])
		voit_thread[0] = voit_thread[1] = Fixed_thread_l;
	else {
		voit_thread[1]->next = Fixed_thread_l;
		Fixed_thread_l->prev = voit_thread[1];
		voit_thread[1] = voit_thread[1]->next;
	}
	pthread_mutex_unlock(mtx);
	return Fixed_thread_l;
}

void Fixed_thread_Del(pthread_mutex_t* mtx, Fixed_thread_t** voit_thread, Fixed_thread_t* Fixed_thread_l)
{
	thread_list* my_thread = Fixed_thread_l->thread;
	pthread_mutex_lock(mtx);
	if (Fixed_thread_l->prev)
		Fixed_thread_l->prev->next = Fixed_thread_l->next;
	else
		voit_thread[0] = Fixed_thread_l->next;
	if (Fixed_thread_l->next)
		Fixed_thread_l->next->prev = Fixed_thread_l->prev;
	else
		voit_thread[1] = Fixed_thread_l->prev;
	pthread_mutex_unlock(mtx);
	delete Fixed_thread_l;
	my_thread->internal_delete();
}
