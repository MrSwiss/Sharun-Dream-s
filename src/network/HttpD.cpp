#include "Sharun.hpp"
#include "internal/Fixed_Thread.hpp"

#include "httpd_link.hpp"

#include <queue>

std::queue<httpd_link_t*> httpd_queue;
Fixed_thread_W_t	HttpD_thread;

void HTTP_Add(httpd_link_t *httpd_link)
{
	pthread_mutex_lock(&HttpD_thread.mtx);
	httpd_queue.push(httpd_link);
	pthread_mutex_unlock(&HttpD_thread.mtx);
	Fixed_thread_Signal(&HttpD_thread);
}

void HttpD_Cleanup()
{
	Fixed_thread_Cleanup(&HttpD_thread);
	while (!httpd_queue.empty())
		httpd_queue.pop();
}

void HttpD_Thread(thread_list *my_thread)
{
	httpd_link_t *httpd_link;
	Fixed_thread_t *http_thread_l;
	http_thread_l = Fixed_thread_Add(HttpD_Cleanup, &HttpD_thread, my_thread);
	while (my_thread->run) {
		pthread_mutex_lock(&HttpD_thread.mtx);
		if (httpd_queue.empty()) {
			pthread_mutex_unlock(&HttpD_thread.mtx);
			my_thread->wait();
		} else {
			httpd_link = httpd_queue.front();
			httpd_queue.pop();
			pthread_mutex_unlock(&HttpD_thread.mtx);
			HTTP_Work(httpd_link);
		}
	}
	Fixed_thread_Del(&HttpD_thread, http_thread_l);
	DEBUG("%s (%i) :: Httpd thread Stopped.\n", __FILE__, __LINE__);
	pthread_exit(NULL);
}
