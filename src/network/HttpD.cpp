#include "Sharun.hpp"
#include "internal/Fixed_Thread.hpp"

#include "httpd_link.h"

#include <queue>

std::queue<httpd_link_t*> httpd_queue;
bool httpd_mtx_init = false;
pthread_mutex_t httpd_mtx;
Fixed_thread_t *http_thread[2] = {NULL, NULL};

void HTTP_Add(httpd_link_t *httpd_link)
{
	pthread_mutex_lock(&httpd_mtx);
	httpd_queue.push(httpd_link);
	pthread_mutex_unlock(&httpd_mtx);
	Fixed_thread_Signal(http_thread);
}

void HTTP_Cleanup()
{
	Fixed_thread_Cleanup(&httpd_mtx, &http_thread[0], &httpd_mtx_init);
	while (!httpd_queue.empty())
		httpd_queue.pop();
}

void HTTP_Thread(thread_list *my_thread)
{
	httpd_link_t *httpd_link;
	Fixed_thread_t *http_thread_l;
	http_thread_l = Fixed_thread_Add(HTTP_Cleanup, &httpd_mtx, http_thread, my_thread, &httpd_mtx_init);
	while (my_thread->run) {
		pthread_mutex_lock(&httpd_mtx);
		if (httpd_queue.empty()) {
			pthread_mutex_unlock(&httpd_mtx);
			my_thread->wait();
		} else {
			httpd_link = httpd_queue.front();
			httpd_queue.pop();
			pthread_mutex_unlock(&httpd_mtx);
			HTTP_Work(httpd_link);
		}
	}
	Fixed_thread_Del(&httpd_mtx, http_thread, http_thread_l);
	DEBUG("%s (%i) :: Httpd thread Stopped.\n", __FILE__, __LINE__);
	pthread_exit(NULL);
}
