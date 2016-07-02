#ifndef _THREAD_LIST_HPP_
#define _THREAD_LIST_HPP_

#include "internal/pthread_sub.hpp"

class thread_list {
	protected:
		pthread_t id;
		bool	detached;
		pthread_pause_t pause;

	public:
		thread_list();
		virtual ~thread_list();
		bool	run;
		short	sub_param;
		void*	mother;
		thread_list* next;
		thread_list* prev;
		void	internal_delete();
		bool	start(void*, void*);
		void	wait();
		void	signal();
		void	join();
};

void thread_L_cleanup();

#endif // _THREAD_LIST_HPP_
