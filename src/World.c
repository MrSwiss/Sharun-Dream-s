#include "Player.h"

void *S_UNMOUNT_VEHICLE_f2(player_t *);
void *S_MOUNT_VEHICLE_f2(player_t *, uint);
ulong PLAYER_COUNT = 0;

P_Buffer_t *command(player_t *, char *);

void* NPC_th(player_t *player)
{
//		Mutex_Lock(Players_mtx);
	NPC_t *t_npc = NULL;
	for (short i=0; i<2; i++) {
		switch (i) {
			case 1:
				t_npc = player->DjW->Zone->Monsters;
				break;
			default:
				t_npc = player->DjW->Zone->Friendly;
		}
		while (t_npc) {
			float dist = getDistance_2D(player->DjW->toXYZ, t_npc->World.XYZ);
			if (dist > 100 && is_spawS(player, t_npc)) {
				P_Buffer_t * buffer = S_DESPAWN_NPC_f(player, t_npc);
				Player_Send(player, buffer);
				free(buffer);
				spawS_del(player, t_npc);
			} else
			if (dist <= 100 && !is_spawS(player, t_npc)) {
				S_SPAWN_NPC_f(player, t_npc, i ? 0 : 1);
				spawS_add(player, t_npc);
			}
			t_npc = t_npc->next;
		}
	}
//		Mutex_Unlock(Players_mtx);
	return NULL;
}

void World_Info(player_t *player)
{
	char Query[100];
	sprintf(Query, "Server Version :<br>%s: %s", SYSTEM_NAME, L_VERSION);
	S_CHAT_to_Me_f(player, SYSTEM_CHAT, Query);
	sprintf(Query, "Welcome to %s !<br>Type \"--help\" for commands.", Cfg_Server_Name);
	S_CHAT_to_Me_f(player, SYSTEM_CHAT, Query);
}

void World_Spam(player_t *player)
{
	if (!player->loged) {
		Stats_Update(player);
		player->loged = time(NULL);
		player->Saved_Time = time(NULL);
		player->mount = NULL;
		player->spawS = NULL;
		player->moveType[0] = 0;
		player->moveType[1] = 0;
		PLAYER_COUNT++;
		char Query[100];
		sprintf(Query, "UPDATE `Server_Stats` SET `Player_Count`=`Player_Count`+1 WHERE `Server_ID`='%i';", Cfg_Server_ID);
		SQL_Query_Fast(Query, 0);
		sprintf(Query, "INSERT INTO `Server_Used_Character` VALUES ('%i', '%i');", Cfg_Server_ID, player->Charcter_Id);
		SQL_Query_Fast(Query, 0);
		World_Info(player);
	}

	OpCodes_Write(player, S_SPAWN_ME);

	NPC_th(player);

	Greet(player, S_SPAWN_USER, BC_TYPE_ARROUND | BC_TYPE_NOT_ME);

	P_Buffer_t * buffer = OpCodes_Write(player, S_SPAWN_USER);
	buffer->BC_Type = BC_TYPE_ARROUND | BC_TYPE_NOT_ME;
	buffer->Sender = player;
	Broadcast(buffer);
}

void World_PlayerMove(player_t *player, ushort type)
{
	NPC_th(player);

	P_Buffer_t * buffer = S_NPC_LOCATION_f(player, type);
	buffer->BC_Type = BC_TYPE_ARROUND | BC_TYPE_NOT_ME;
	buffer->Sender = player;
	Broadcast(buffer);

	Greet(player, S_SPAWN_USER, BC_TYPE_ARROUND | BC_TYPE_NOT_ME);

	buffer = OpCodes_Write(player, S_SPAWN_USER);
	buffer->BC_Type = BC_TYPE_ARROUND | BC_TYPE_NOT_ME;
	buffer->Sender = player;
	Broadcast(buffer);
}

void World_PlayerSocial(player_t *player, int id)
{
	if (player->mount)
		return;
	P_Buffer_t * buffer = NULL;
	if (id < 0)
		buffer = S_SOCIAL_CANCEL_f(player);
	else
		buffer = S_SOCIAL_f(player, id);
	buffer->BC_Type = BC_TYPE_ARROUND;
	buffer->Sender = player;
	Broadcast(buffer);
}

void World_PlayerMount(player_t *player, bool id, uint Skill_Id)
{
	P_Buffer_t * buffer = NULL;
	S_START_COOLTIME_SKILL_f(player, player->mount->Skill_Id, 1000);
	if (player->mount->Skill_Id != Skill_Id)
		S_START_COOLTIME_SKILL_f(player, Skill_Id, 1000);
	if (!id) {
		buffer = S_UNMOUNT_VEHICLE_f(player);
		if (buffer) player->mount = NULL;
	}
	else
		buffer = S_MOUNT_VEHICLE_f(player);
	if (buffer) {
		buffer->BC_Type = BC_TYPE_ARROUND;
		buffer->Sender = player;
		Broadcast(buffer);
		S_PLAYER_STAT_UPDATE_f(player);
	}
}

void World_PlayerSkill(player_t *player, player_t *target, uint Skill_Id)
{
	mount_t * mount = get_Mount(player, Skill_Id);
	if (mount) {
		if (player->mount)// && player->mount->Skill_Id == Skill_Id)
			World_PlayerMount(player, 0, Skill_Id);
		else {
			player->mount = mount;
			World_PlayerMount(player, 1, Skill_Id);
		}
	}
}

void World_PlayerMountGM(player_t *player, int id)
{
	P_Buffer_t * buffer = NULL;
	if (id < 0)
		buffer = S_UNMOUNT_VEHICLE_f2(player);
	else
		buffer = S_MOUNT_VEHICLE_f2(player, id);
	if (buffer) {
		buffer->BC_Type = BC_TYPE_ARROUND;
		buffer->Sender = player;
		Broadcast(buffer);
	}
}

void World_Chat(player_t *player, ushort type, char *text)
{
	char * b = strstr(text, ">");
	P_Buffer_t * buffer = NULL;
	if (b[1] == '-' && b[2] == '-')
		buffer = command(player, b);
	else
		buffer = S_CHAT_f(player, type, text, player->name);
	if (buffer) {
		buffer->Sender = player;
		Broadcast(buffer);
	}
}

void World_UnSpam(player_t *player)
{
	P_Buffer_t * buffer = S_DESPAWN_NPC_f(player, NULL);
	buffer->BC_Type = BC_TYPE_ARROUND | BC_TYPE_NOT_ME | BC_TYPE_FORCE;
	buffer->Sender = player;
	Broadcast(buffer);
	spawS_free(player);
	player->spawS = NULL;
}

void World_Exit(player_t *player)
{
	Character_Save(player);
	player->loged = 0;
	player->spawned = 0;

	World_UnSpam(player);

	PLAYER_COUNT--;
	char Query[100];
	sprintf(Query, "UPDATE `Server_Stats` SET `Player_Count`=`Player_Count`-1 WHERE `Server_ID`='%i';", Cfg_Server_ID);
	SQL_Query_Fast(Query, 0);

	sprintf(Query, "DELETE FROM `Server_Used_Character` WHERE `Character_ID`='%i' AND `Server_ID`='%i';", player->Charcter_Id, Cfg_Server_ID);
	SQL_Query_Fast(Query, 0);

}
