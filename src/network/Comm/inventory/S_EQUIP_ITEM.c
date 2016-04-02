#include "Player.h"

void S_EQUIP_ITEM_f(player_t *player, ulong item_id)
{
	P_Buffer_t buffer;
	set_WBuffer(&buffer, S_EQUIP_ITEM);

	Player_Writer4(&buffer, player->Character_Id);
	Player_Writer4(&buffer, player->SUBID);
	Player_Writer8(&buffer, item_id);

	Player_Send(player, &buffer);
}
