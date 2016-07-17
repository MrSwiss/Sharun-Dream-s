#include "Sharun.hpp"

#ifdef DLL_VERSION

sharun_t *Sharun;

void (*DEBUG)(const char*, ...);
void (*DEBUG2)(const char*, ...);

void	(*MySQL_Data_Seek_PLG)(void*, ulong);
char**	(*MySQL_Fetch_Row_PLG)(void*);
void	(*MySQL_Free_Result_PLG)(void*);
void	(*MySQL_Add_PLG)(SQL_QUEUE*);

player*	(*new_player)(uint);
packet*	(*new_packet)();

extern "C" {

void CallBack_Plugin(void** Link)
{
	uint i = 0;
	Sharun = (sharun_t*) Link[i++];

	DEBUG = (void (*)(const char*, ...)) Link[i++];
	DEBUG2 = (void (*)(const char*, ...)) Link[i++];

	MySQL_Data_Seek_PLG = (void (*)(void*, ulong)) Link[i++];
	MySQL_Fetch_Row_PLG = (char** (*)(void*)) Link[i++];
	MySQL_Free_Result_PLG = (void (*)(void*)) Link[i++];
	MySQL_Add_PLG = (void (*)(SQL_QUEUE*)) Link[i++];

	new_player = (player* (*)(uint)) Link[i++];
	new_packet = (packet* (*)()) Link[i++];
}

}

#endif // DLL_VERSION
