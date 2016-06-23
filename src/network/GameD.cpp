#include "Sharun.hpp"
#include "internal/Fixed_Thread.hpp"

#include <queue>

std::queue<connexion_list*> GameD_queue;
bool GameD_mtx_init = false;
pthread_mutex_t GameD_mtx;
Fixed_thread_t *GameD_thread[2] = {NULL, NULL};

void GameD_Add(connexion_list *connex)
{
	pthread_mutex_lock(&GameD_mtx);
	GameD_queue.push(connex);
	pthread_mutex_unlock(&GameD_mtx);
	Fixed_thread_Signal(GameD_thread);
}

void GameD_Cleanup()
{
	Fixed_thread_Cleanup(&GameD_mtx, &GameD_thread[0], &GameD_mtx_init);
	while (!GameD_queue.empty())
		GameD_queue.pop();
}

void GameD_Thread(thread_list *my_thread)
{
	connexion_list *connex;
	Fixed_thread_t *GameD_thread_l;
	GameD_thread_l = Fixed_thread_Add(GameD_Cleanup, &GameD_mtx, GameD_thread, my_thread, &GameD_mtx_init);
	while (my_thread->run) {
		pthread_mutex_lock(&GameD_mtx);
		if (GameD_queue.empty()) {
			pthread_mutex_unlock(&GameD_mtx);
			my_thread->wait();
		} else {
			connex = GameD_queue.front();
			GameD_queue.pop();
			pthread_mutex_unlock(&GameD_mtx);
//			HTTP_Work(connex);
			delete connex; // prout, à faire
		}
	}
	Fixed_thread_Del(&GameD_mtx, GameD_thread, GameD_thread_l);
	DEBUG("%s (%i) :: Httpd thread Stopped.\n", __FILE__, __LINE__);
	pthread_exit(NULL);
}
