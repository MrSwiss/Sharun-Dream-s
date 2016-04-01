#include "Player.h"

void *S_SOCIAL_f(player_t *player, uint id)
{
	P_Buffer_t * buffer = New_WBuffer(S_SOCIAL);

	Player_Writer4(buffer, player->Charcter_Id);
	Player_Writer4(buffer, player->SUBID);
	Player_Writer4(buffer, id);
	Player_Writer4(buffer, 0);
	Player_Writer1(buffer, 0);

	return buffer;
}
