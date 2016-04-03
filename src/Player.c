#include "Player.h"

/** DEBUG COMMAND:
printf("%s (%i)\n", __func__, __LINE__);
 **/

#include "TeraCrypt.h"

int class_mat[] = {
	LEATHER,
	METAL,
	LEATHER,
	METAL,
	CLOTH,
	LEATHER,
	CLOTH,
	CLOTH,
	LEATHER,
	METAL
};

float HP_CLASS_MOD[] = {
	1.057F,
	1.057F,
	1.057F,
	1.0575F,
	1.058F,
	1.057F,
	1.0581F,
	1.0581F,
	1.057F,
	1.0575F,
	1.0575F
};

ulong Experience_LEVEL[LV_Max+2] = {
	0,
	840,// lv 1
	2686,
	4800,
	8300,
	12000,
	18600,
	25000,
	46000,
	64000,
	83795, //lv 10
	107904,
	137844,
	173664,
	217412,
	270045,
	332904,
	411478,
	509555,
	624045,
	767158, //lv 20
	946049,
	1169663,
	1444708,
	1780263,
	2199707,
	2714012,
	3356893,
	4104477,
	4943952,
	5898783, //lv 30
	7021426,
	8275585,
	9730408,
	11450277,
	13544471,
	16137427,
	19167071,
	22590568,
	26459120,
	30907955, //lv 40
	36335533,
	43011454,
	51356356,
	61787484,
	74722083,
	90114255,
	107815253,
	127819509,
	149819509,
	174770163, //lv 50
	202477270,
	233242615,
	267400774,
	305322656,
	347419424,
	394146836,
	446010054,
	541343794,
	692902622,
	1855567467, //lv 60
	3117029930,
	4495199471,
	6016452207,
	7701537916,
	9616157695, //lv 65
	0
};

void Player_Recv(player_t *player)
{
	player->Packet_R_Pos = 4;
	player->Packet_R_HeadPos = 0;
	player->Packet_R_Size = recv(player->NetLink->sock, (char*)player->Packet_R, Packet_Max_Size, 0);
	if (player->Packet_R_Size >= 0) {
		TeraCrypt_Dec(player->TERACrypt, player->Packet_R, player->Packet_R_Size);
		const char *name = OpCodes_FindName(player);
		if ( !name || ( name
		  && strcmp(name, "C_SIMPLE_TIP_REPEAT_CHECK")
		  && strcmp(name, "C_SET_VISIBLE_RANGE") ) )
			hexdump(__func__, name, player->Packet_R, player->Packet_R_Size);
	}
}

void Player_Send(player_t *player, P_Buffer_t *buffer)
{
	Player_UpdateHeaderS(buffer);
	OpCodes_GetOpCode(player, buffer);
	const char *name = OpCodes_GetOpCode_Name(player, buffer->OpCode);
	if ( buffer->OpCode != S_SIMPLE_TIP_REPEAT_CHECK
	  && buffer->OpCode != S_RESPONSE_GAMESTAT_PONG
	  && buffer->OpCode != 0x1111 )
		hexdump(__func__, name, buffer->Data, buffer->Pos);
	Mutex_Lock(player->send_locker);
	TeraCrypt_Enc(player->TERACrypt, buffer->Data, buffer->Pos);
	send(player->NetLink->sock, (char*)buffer->Data, buffer->Pos, 0);
	Mutex_Unlock(player->send_locker);
}

void Player_Login(netlink_t *NetLink)
{
	player_t *player = malloc(sizeof(player_t));
	player->NetLink = NetLink;
	player->struct_type = 1;
	player->TERACrypt = NULL;
	player->OpCode_GID = -1;
	player->Account_Id = 0;
	player->Packet_R_Pos = 4;
	player->Packet_R_HeadPos = 0;
	player->exit_type = 0;
	player->loged = 0;
	player->spawned = 0;
	player->Saved_Time = 0;
	player->send_locker = Mutex_Init();
	player->spawS = NULL;
	player->spawS_locker = Mutex_Init();

	char rrun = 1;
	unsigned char *Key = malloc(4*128);
	for (int i=0; i<2; i++)
		for (int j=0; j<128; j++)
			Key[(2+i)*128+j] = rand() % 0xff;
	for (int i=0; i<2 && rrun; i++) {
		int ret = recv(player->NetLink->sock, (char*)&Key[(0+i)*128], 128, 0);
		if (!i && (!memcmp(&Key[(0+i)*128], "GET ", 4) || !memcmp(&Key[(0+i)*128], "POST ", 5) || !memcmp(&Key[(0+i)*128], "HEAD ", 5) || !memcmp(&Key[(0+i)*128], "TRACE ", 6))) {
			// Last chance for an Web access.
			NetLink->Size = ret;
			NetLink->noHead = true;			
			memcpy(NetLink->Head, &Key[(0+i)*128], NetLink->Size);
			NetLink->Size += recv(NetLink->sock, NetLink->Head+NetLink->Size, 512-NetLink->Size, 0);
			NetLink->C_Thread = CreateThread(NULL, 0, (void*)HTTP_Work, NetLink, 0, NULL);
			player->NetLink = NULL;
			Player_Exit(player);
			free(Key);
			return;
		}
//hexdump(__func__, "Incomming", &Key[(0+i)*128], ret);
		if (ret != 128) {
			DEBUG("%s (%i) :: Client Key %i ! (%i<128)\n", __func__, __LINE__, i+1, ret);
			rrun = 0;
		}
		if (rrun)
			ret = send(player->NetLink->sock, (char*)&Key[(2+i)*128], 128, 0);
		if (rrun  && ret != 128) {
			DEBUG("%s (%i) :: Server Key %i ! (%i<128)\n", __func__, __LINE__, i+1, ret);
			rrun = 0;
		}
	}

	if (rrun) {
		player->TERACrypt = TeraCrypt_Init(&Key[0*128], &Key[1*128], &Key[2*128], &Key[3*128]);
		free(NetLink->Head);
		NetLink->Head = NULL;
		free(Key);

		Player_Recv(player);
		if (player->Packet_R_Size >= 0) {
			PACKET_h *PACKET_H = (PACKET_h*)&player->Packet_R[player->Packet_R_HeadPos];
			player->Packet_R_HeadPos += PACKET_H->Size;
			PACKET_H = (PACKET_h*)&player->Packet_R[player->Packet_R_HeadPos];
			player->OpCode_GID = OpCodes_DetectOpCode(PACKET_H->Code);
			if (player->OpCode_GID < 0) {
				DEBUG("%s (%i) :: Can't find OpCodes for this version ! (C_LOGIN_ARBITER %i 0x%04X)\n", __func__, __LINE__, PACKET_H->Code, PACKET_H->Code);
				rrun = 0;
			}
			else {
				OpCodes_Read(player);
				if (!player->Account_Id) {
					rrun = 0;
					S_LOGIN_FAILED_f(player);
				} else
					player->Regen_Thread = CreateThread(NULL, 0, (void*)Regen_Player, player, 0, NULL);
			}
			long Still_Online;
			player->Still_Online = &Still_Online;
			while (rrun && player->Packet_R_Size >= 0) {
				Still_Online = time(NULL);
				Player_Recv(player);
				if (!Still_Online) {
					Player_Exit(player);
					return;
				}
				if (player->Packet_R_Size >= Packet_Max_Size) {
					// CLient have begin to do a global crash !
					Player_Exit(player);
					return;
				}
				do {
					PACKET_h *PACKET_H = (PACKET_h*)&player->Packet_R[player->Packet_R_HeadPos];
					if (PACKET_H->Size + player->Packet_R_HeadPos >= Packet_Max_Size) {
						// CLient have begin to do a packet crash !
						Player_Exit(player);
						return;
					}
					OpCodes_Read(player);
				} while (player->Packet_R_Size >= 0 && player->Packet_R_Pos < player->Packet_R_Size);
			}
		}
	} else
		DEBUG("%s (%i) :: Can't Recv Login Data !\n", __func__, __LINE__);

	Player_Exit(player);
}

void Player_Delog(player_t *player)
{
	if (player->Account_Id) {
		if (player->loged) World_Exit(player);
		Mutex_Lock(Players_mtx);
		Players[player->Player_Id] = NULL;
		Mutex_Unlock(Players_mtx);
		player->Player_Id = 0;
		player->Account_Id = 0;
		CloseHandle(player->Regen_Thread);
	}

	if (player->TERACrypt) TeraCrypt_Free(player->TERACrypt);
	player->TERACrypt = NULL;

	if (player->send_locker) {
		Mutex_Close(player->send_locker);
		player->send_locker = NULL;
	}

	spawS_free(player);
	if (player->spawS_locker) {
		Mutex_Close(player->spawS_locker);
		player->spawS_locker = NULL;
	}

	if (player->NetLink) {
		DEBUG("%s (%i) :: Client (%i - %s) disconnected.\n", __func__, __LINE__, player->Account_Id, player->NetLink->IP);
		NetLink_close(player->NetLink);
		free(player->NetLink);
		player->NetLink = NULL;
	}
}

short Player_have_crashed(player_t *player)
{
	if (player->loged && *player->Still_Online > 0 && *player->Still_Online + 60 < time(NULL)) {
		*player->Still_Online = 0;
//		Player_Delog(player);
		Player_Exit(player);
		return 1;
	}
	return 0;
}

void Player_Exit(player_t *player)
{
	bool msg = true;
	if (!player->NetLink) msg = false;
	Player_Delog(player);

	if (msg) DEBUG("%s (%i) :: Client (%i) cleaned.\n", __func__, __LINE__, player->Account_Id);
	free(player);

	c_THREAD_free(NULL);
}
