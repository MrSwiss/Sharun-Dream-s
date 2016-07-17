#include "Sharun.hpp"
#include "internal/DB.hpp"

#include <cstdarg>

void	(*MySQL_Data_Seek_PLG)(void*, ulong);
char**	(*MySQL_Fetch_Row_PLG)(void*);
void	(*MySQL_Free_Result_PLG)(void*);
void	(*MySQL_Add_PLG)(SQL_QUEUE*);

bool	is_MySQL_threaded();

void SQL_QUEUE::signal()
{
	pthread_pause_signal(resume);
}

void database::start()
{
	MySQL_Data_Seek_PLG = (void (*)(void*, ulong)) mysql_data_seek;
	MySQL_Fetch_Row_PLG = (char** (*)(void*)) mysql_fetch_row;
	MySQL_Free_Result_PLG = (void (*)(void*)) mysql_free_result;
	MySQL_Add_PLG = (void (*)(SQL_QUEUE*)) MySQL_Add;

	thread_list *thread = NULL;
	if (is_MySQL_threaded()) {
		for (int i=0; DB_Set.Login && i < Sharun->Settings.Thread.DataBase.Fast; i++) {
			thread = new thread_list();
			thread->sub_param = 1;
			if (!thread->start((void*)MySQL_Thread, thread))
				DEBUG("%s (%i) :: Could not start MySQL thread %i !\n", __FILE__, __LINE__, i);
			else
				DEBUG("%s (%i) :: MySQL fast thread %i started.\n", __FILE__, __LINE__, i);
		}
		for (int i=0; DB_Set.Login && i < Sharun->Settings.Thread.DataBase.Slow; i++) {
			thread = new thread_list();
			if (!thread->start((void*)MySQL_Thread, thread))
				DEBUG("%s (%i) :: Could not start MySQL thread %i !\n", __FILE__, __LINE__, i);
			else
				DEBUG("%s (%i) :: MySQL slow thread %i started.\n", __FILE__, __LINE__, i);
		}
	}
}
