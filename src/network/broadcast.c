#include "Player.h"

typedef struct bdc_t {
	P_Buffer_t*	buffer;
	void*		next;
} bdc_t;

void*	bdc_mtx = NULL;
void**	bdc_thd = NULL;
void**	bdc_brk = NULL;
bdc_t*	bdc_stk = NULL;
bdc_t*	bdc_stk_last = NULL;
bool	bdc_run = 0;


void broadcast_th(void *argv)
{
	ushort *bdc_id = argv;
	short thread_id = *bdc_id;
	bdc_t*	l_bdc_stk = NULL;
	P_Buffer_t bufferTmp, *buffer = NULL;
	DEBUG("%s (%i) :: Broadcast Thread Started.\n", __func__, __LINE__);
	usleep(1);

	while (bdc_run) {
		Mutex_Lock(bdc_mtx);
		if (bdc_stk) {
			l_bdc_stk = bdc_stk;
			bdc_stk = bdc_stk->next;
		}
		Mutex_Unlock(bdc_mtx);
		if (l_bdc_stk) {
			buffer = l_bdc_stk->buffer;
			free(l_bdc_stk);
			l_bdc_stk = NULL;
		}
	
		for (int i=0; buffer && i<Player_Max; i++) {
			bool is_spawS_b = false;
			if (Players[i] && buffer->Sender) is_spawS_b = is_spawS(Players[i], buffer->Sender);
			if (   buffer
			    && (!(buffer->BC_Type & BC_TYPE_NOT_ME) || ((buffer->BC_Type & BC_TYPE_NOT_ME) && buffer->Sender->Player_Id != i))
			    && Players[i]
			    && Players[i]->loged
			    && Players[i]->DjW
			    && Players[i]->DjW->Zone
			    && (  (buffer->BC_Type & BC_TYPE_MAIN)
			      || ((buffer->BC_Type & BC_TYPE_ZONE) && buffer->Sender && buffer->Sender->DjW->Zone == Players[i]->DjW->Zone)
			      || ((buffer->BC_Type & BC_TYPE_ARROUND) && buffer->Sender && buffer->Sender->DjW->Zone == Players[i]->DjW->Zone
					&& ( (buffer->OpCode == S_DESPAWN_NPC && ((buffer->BC_Type & BC_TYPE_FORCE) || getDistance_2D(buffer->Sender->DjW->XYZ, Players[i]->DjW->XYZ) > 100) && is_spawS_b)
					  || (buffer->OpCode == S_SPAWN_USER  && getDistance_2D(buffer->Sender->DjW->XYZ, Players[i]->DjW->XYZ) <= 100 && !is_spawS_b)
/*					  || ((buffer->OpCode == S_USER_EXTERNAL_CHANGE || buffer->OpCode == S_MOUNT_VEHICLE || buffer->OpCode == S_UNMOUNT_VEHICLE) && getDistance_2D(buffer->Sender->DjW->XYZ, Players[i]->DjW->XYZ) <= 100
					    && (Players[i] == buffer->Sender || (Players[i] != buffer->Sender && is_spawS_b)))
*/					  || (buffer->OpCode == S_CHAT && getDistance_2D(buffer->Sender->DjW->XYZ, Players[i]->DjW->XYZ) < 200)
					  || ((buffer->OpCode != S_DESPAWN_NPC && buffer->OpCode != S_SPAWN_USER && buffer->OpCode != S_CHAT) && getDistance_2D(buffer->Sender->DjW->XYZ, Players[i]->DjW->XYZ) <= 100
					    && (Players[i] == buffer->Sender || (Players[i] != buffer->Sender && is_spawS_b)))
					   )
				 )
			    )
			) {
				if (buffer->OpCode == S_DESPAWN_NPC)
					spawS_del(Players[i], buffer->Sender);
				else if (buffer->OpCode == S_SPAWN_USER)
					spawS_add(Players[i], buffer->Sender);
				memcpy(&bufferTmp, buffer, sizeof(P_Buffer_t));
				Player_Send(Players[i], &bufferTmp);
			}
		}

		free(buffer);
		buffer = NULL;
		if (!bdc_stk) //usleep(Cfg_Server_Usleep_Time);
			Mutex_Pause(bdc_brk[thread_id]);
	}
	DEBUG("%s (%i) :: Broadcast Thread Stopped.\n", __func__, __LINE__);

	c_THREAD_free(NULL);
}

void Broadcast(P_Buffer_t *buffer)
{
	bdc_t*	l_bdc_stk = malloc(sizeof(bdc_t));
	l_bdc_stk->next = NULL;
	l_bdc_stk->buffer = buffer;
	Mutex_Lock(bdc_mtx);
	if (!bdc_stk)
		bdc_stk = l_bdc_stk;
	else
		bdc_stk_last->next = l_bdc_stk;
	bdc_stk_last = l_bdc_stk;
	Mutex_Unlock(bdc_mtx);
	for (short i=0; i<Cfg_Broadcast_Max_Thread; i++)
		Mutex_Resume(bdc_brk[i]);
}

void init_Broadcast()
{
	bdc_run = 1;
	bdc_mtx = Mutex_Init();
	bdc_brk = malloc(sizeof(void *)*Cfg_Broadcast_Max_Thread);
	bdc_thd = malloc(sizeof(void*)*Cfg_Broadcast_Max_Thread);
	for (short i=0; i<Cfg_Broadcast_Max_Thread; i++) {
		ushort *bdc_id = malloc(sizeof(ushort));
		*bdc_id = i;
		bdc_brk[i] = Mutex_Init();
		bdc_thd[i] = CreateThread(NULL, 0, (void*)broadcast_th, bdc_id, 0, NULL);
		usleep(1);
	}
	DEBUG("%s (%i) :: Broadcast Started.\n", __func__, __LINE__);
}

void close_Broadcast()
{
	while (bdc_stk) usleep(1);
	bdc_run = 0;
	for (short i=0; i<Cfg_Broadcast_Max_Thread; i++)
		Mutex_Resume(bdc_brk[i]);
	if (bdc_thd) WaitForMultipleObjects(Cfg_Broadcast_Max_Thread, bdc_thd, TRUE, INFINITE);
	if (bdc_mtx) Mutex_Close(bdc_mtx);
	for (short i=0; i<Cfg_Broadcast_Max_Thread; i++)
		Mutex_Close(bdc_brk[i]);
	if (bdc_brk) free(bdc_brk);
	if (bdc_thd) free(bdc_thd);
	bdc_brk = NULL;
	bdc_thd = NULL;
	bdc_mtx = NULL;
	DEBUG("%s (%i) :: Broadcast Stopped.\n", __func__, __LINE__);
}

void Greet(player_t *player, ushort Opcode_Id, uchar BC_Type)
{
//	Mutex_Lock(Players_mtx);
	for (int i=0; i<Player_Max; i++) {
		bool is_spawS_b = false;
		if (Players[i] && player) is_spawS_b = is_spawS(player, Players[i]);
		if (   player
		    && (!(BC_Type & BC_TYPE_NOT_ME) || ((BC_Type & BC_TYPE_NOT_ME) && player->Player_Id != i))
		    && Players[i]
		    && Players[i]->loged
		    && Players[i]->DjW
		    && Players[i]->DjW->Zone
		    && (  (BC_Type & BC_TYPE_MAIN)
		      || ((BC_Type & BC_TYPE_ZONE) && player->DjW->Zone == Players[i]->DjW->Zone)
		      || ((BC_Type & BC_TYPE_ARROUND) && player->DjW->Zone == Players[i]->DjW->Zone
				&& ((getDistance_2D(player->DjW->toXYZ, Players[i]->DjW->toXYZ) <= 100  && !is_spawS_b)
				 || (getDistance_2D(player->DjW->toXYZ, Players[i]->DjW->toXYZ) > 100  &&  is_spawS_b)
				   )
			 )
		    )
		) {
			P_Buffer_t * buffer = OpCodes_Write(Players[i], Opcode_Id);
			if (Opcode_Id == S_SPAWN_USER)
				spawS_add(player, Players[i]);
			else if (Opcode_Id == S_DESPAWN_NPC)
				spawS_del(player, Players[i]);
			Player_Send(player, buffer);
			free(buffer);
		}
	}
//	Mutex_Unlock(Players_mtx);
}
