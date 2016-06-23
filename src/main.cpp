#include "Sharun.hpp"
#include "internal/network.hpp"
#include "internal/Fixed_Thread.hpp"
//*******************************************************************************************
#include <unistd.h>
#include <csignal>
#include <string>
//*******************************************************************************************
settings *Settings;
network *Network;

pthread_pause_t main_pause;
//*******************************************************************************************
void intHandler(int useless)
{
	pthread_pause_signal(&main_pause);
}

int main(int argc, const char *argv[])
{
#ifdef I_M_CRAZY
	thread_max_count(NULL);
#else
	set_thread_max(850);
#endif

	Settings = new settings();
	Network = new network();

	pthread_pause_init(&main_pause, NULL);

	signal(SIGINT, intHandler);

	Network->start();

	pthread_pause_wait(&main_pause);

	delete Network;
	Fixed_thread_Close();
	thread_L_cleanup();
	delete Settings;
	pthread_pause_destroy(&main_pause);
	DEBUG("%s (%i) :: Bye. ~\n", __func__, __LINE__);
	return 0;
}
