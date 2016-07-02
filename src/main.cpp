#include "Sharun.hpp"
#include "OpCodes.hpp"
#include "internal/network.hpp"
#include "internal/Fixed_Thread.hpp"
//*******************************************************************************************
#include <unistd.h>
#include <csignal>
#include <string>
//*******************************************************************************************
network *Network = NULL;
sharun_t *Sharun = NULL;

pthread_pause_t main_pause;
//*******************************************************************************************
void intHandler(int useless)
{
	Sharun->Settings.main_run = false;
	pthread_pause_signal(&main_pause);
}

int main(int argc, const char *argv[])
{
#ifdef I_M_CRAZY
	thread_max_count(NULL);
#else
	set_thread_max(850);
#endif

	Network = new network();
	Sharun = new sharun_t;

	pthread_pause_init(&main_pause, NULL);
	DEBUG("Main sizeof(sharun_t)=%ld.\n", sizeof(sharun_t));

	signal(SIGINT, intHandler);

	Sharun->DB.start();
	Network->start();

//	pthread_pause_wait(&main_pause);

	while (Sharun->Settings.main_run) {
		char Input_Key[1024];
		memset(Input_Key, 0, 1024);
		scanf(" %[^\n]s\n", Input_Key);
		for (size_t i=0; i<strlen(Input_Key); i++)
			Input_Key[i] = toupper(Input_Key[i]);
		if ( !strcmp(Input_Key, "BYE")
		  || !strcmp(Input_Key, "QUIT")
		  || !strcmp(Input_Key, "STOP")
		  || !strcmp(Input_Key, "EXIT") )
			break;
	}
	Sharun->Settings.main_run = false;

	delete Network;
	Fixed_thread_Close();
	thread_L_cleanup();
	delete Sharun;
	pthread_pause_destroy(&main_pause);
	DEBUG("%s (%i) :: Bye. ~\n", __func__, __LINE__);
	return 0;
}
