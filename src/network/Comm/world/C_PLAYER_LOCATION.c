#include "Player.h"

void C_PLAYER_LOCATION_f(player_t *player)
{
	for (int i=0; i<3; i++)
		player->DjW->XYZ[i] = Player_ReaderF(player);

	player->DjW->heading = Player_Reader2(player);

	Player_Reader2(player);

	//целевая точка
	for (int i=0; i<3; i++)
		player->DjW->toXYZ[i] = Player_ReaderF(player);

	//тип перемещения
	player->moveType[1] = Player_Reader2(player);
/*
	Player_Reader1(player);
	Player_Reader4(player);
	Player_Reader4(player);
*/
	Player_NextHeaderR(player);

	World_PlayerMove(player, player->moveType[1]);

	player->moveType[0] = player->moveType[1];
}
