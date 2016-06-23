#include "Sharun.hpp"
#include "internal/Fixed_Thread.hpp"

#include <queue>

std::queue<connexion_list*> GameD_queue;
Fixed_thread_W_t	GameD_thread;

void GameD_Add(connexion_list *connex)
{
	pthread_mutex_lock(&GameD_thread.mtx);
	GameD_queue.push(connex);
	pthread_mutex_unlock(&GameD_thread.mtx);
	Fixed_thread_Signal(&GameD_thread);
}

void GameD_Cleanup()
{
	Fixed_thread_Cleanup(&GameD_thread);
	while (!GameD_queue.empty())
		GameD_queue.pop();
}

void GameD_Thread(thread_list *my_thread)
{
	connexion_list *connex;
	Fixed_thread_t *GameD_thread_l;
	GameD_thread_l = Fixed_thread_Add(GameD_Cleanup, &GameD_thread, my_thread);
	while (my_thread->run) {
		pthread_mutex_lock(&GameD_thread.mtx);
		if (GameD_queue.empty()) {
			pthread_mutex_unlock(&GameD_thread.mtx);
			my_thread->wait();
		} else {
			connex = GameD_queue.front();
			GameD_queue.pop();
			pthread_mutex_unlock(&GameD_thread.mtx);
//			HTTP_Work(connex);
			delete connex; // prout, à faire
		}
	}
	Fixed_thread_Del(&GameD_thread, GameD_thread_l);
	DEBUG("%s (%i) :: Httpd thread Stopped.\n", __FILE__, __LINE__);
	pthread_exit(NULL);
}
