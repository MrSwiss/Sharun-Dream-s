#include "Player.h"

void *S_SOCIAL_CANCEL_f(player_t *player)
{
	P_Buffer_t * buffer = New_WBuffer(S_SOCIAL_CANCEL);

	Player_Writer4(buffer, player->Charcter_Id);
	Player_Writer4(buffer, player->SUBID);

	return buffer;
}
