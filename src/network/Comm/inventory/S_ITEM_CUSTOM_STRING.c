#include "Player.h"

void S_ITEM_CUSTOM_STRING_f(player_t *player)
{
	P_Buffer_t buffer;
	set_WBuffer(&buffer, S_ITEM_CUSTOM_STRING);

	Player_Writer4(&buffer, 0);
	Player_Writer4(&buffer, player->Character_Id);
	Player_Writer4(&buffer, player->SUBID);

	Player_Send(player, &buffer);
}
