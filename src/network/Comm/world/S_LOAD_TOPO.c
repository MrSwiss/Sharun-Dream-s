#include "Player.h"

void S_LOAD_TOPO_f(player_t *player)
{
	P_Buffer_t buffer;
	set_WBuffer(&buffer, S_LOAD_TOPO);

	Player_Writer4(&buffer, player->DjW->Zone->Id);
	for (int i=0; i<3; i++)
		Player_WriterF(&buffer, player->DjW->XYZ[i]);
	Player_Writer1(&buffer, 0);

	Player_Send(player, &buffer);

	*player->Still_Online = -1;
}

