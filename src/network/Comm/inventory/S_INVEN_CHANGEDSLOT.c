#include "Player.h"

void S_INVEN_CHANGEDSLOT_f(player_t *player, short slot)
{
	P_Buffer_t buffer;
	set_WBuffer(&buffer, S_INVEN_CHANGEDSLOT);

	Player_Writer2(&buffer, player->Item_slot_count*8);//);1); // Count
	Player_Writer2(&buffer, 8);

	ushort next = 0;
	for (ushort inv_pos=0; inv_pos < player->Item_slot_count*8; inv_pos++) {
		if (inv_pos) Player_WriterSet2(&buffer, buffer.Pos, next);
		Player_Writer2(&buffer, buffer.Pos); // Actual
		next = buffer.Pos;
		Player_Writer2(&buffer, 0); // Next
		Player_Writer1(&buffer, 0); // Next
		Player_Writer4(&buffer, inv_pos+1); // slot id
	}

	Player_Send(player, &buffer);
}
