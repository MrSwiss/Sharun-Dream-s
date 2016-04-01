#include "Player.h"

void *S_MOUNT_VEHICLE_f(player_t *player)
{
	if (!player->mount)
		return NULL;
	P_Buffer_t * buffer = New_WBuffer(S_MOUNT_VEHICLE);
	Player_Writer4(buffer, player->Charcter_Id);
	Player_Writer4(buffer, player->SUBID);
	Player_Writer4(buffer, player->mount->Mount_Id); // mountId
	Player_Writer4(buffer, player->mount->Skill_Id); // skillId
	Player_Writer1(buffer, 0);
	return buffer;
}

void *S_MOUNT_VEHICLE_f2(player_t *player, uint id)
{
	P_Buffer_t * buffer = New_WBuffer(S_MOUNT_VEHICLE);
	Player_Writer4(buffer, player->Charcter_Id);
	Player_Writer4(buffer, player->SUBID);
	Player_Writer4(buffer, id); // mountId
	Player_Writer4(buffer, 111111); // skillId
	Player_Writer1(buffer, 0);
	return buffer;
}
