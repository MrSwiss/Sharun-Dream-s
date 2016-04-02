#include "Player.h"

void S_SPAWN_ME_f(player_t *player)
{
	P_Buffer_t buffer;
	set_WBuffer(&buffer, S_SPAWN_ME);

	Player_Writer4(&buffer, player->Character_Id);
	Player_Writer4(&buffer, player->SUBID);
	for (int i=0; i<3; i++)
		Player_WriterF(&buffer, player->DjW->XYZ[i]);
	Player_Writer2(&buffer, player->DjW->heading);
	Player_Writer2(&buffer, player->Stats.HP ? 1 : 0);

	Player_Send(player, &buffer);

	player->spawned = 1;
}
