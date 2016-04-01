#include "Player.h"

void *S_UNMOUNT_VEHICLE_f(player_t *player)
{
	P_Buffer_t * buffer = New_WBuffer(S_UNMOUNT_VEHICLE);

	Player_Writer4(buffer, player->Charcter_Id);
	Player_Writer4(buffer, player->SUBID);
	Player_Writer4(buffer, player->mount->Skill_Id);

	return buffer;
}

void *S_UNMOUNT_VEHICLE_f2(player_t *player)
{
	P_Buffer_t * buffer = New_WBuffer(S_UNMOUNT_VEHICLE);

	Player_Writer4(buffer, player->Charcter_Id);
	Player_Writer4(buffer, player->SUBID);
	Player_Writer4(buffer, 111111);

	return buffer;
}
