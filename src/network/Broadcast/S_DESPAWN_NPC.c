#include "Player.h"
/*
enum S_DESPAWN_NPC_EFFECT {
	S_DESPAWN_NPC_DISAPPEARS = 1,
	S_DESPAWN_NPC_DISAPPEARS_DUST = 4,
	S_DESPAWN_NPC_DEAD = 5
}
*/
void *S_DESPAWN_NPC_f(player_t *player, NPC_t *npc)
{
	P_Buffer_t * buffer = New_WBuffer(S_DESPAWN_NPC);

		if (npc) {
			Player_Writer4(buffer, npc->Id);
			Player_Writer4(buffer, npc->SUBID);
		} else {
			Player_Writer4(buffer, player->Character_Id);
			Player_Writer4(buffer, player->SUBID);
		}
		for (int i=0; i<3; i++) {
			if (npc)
				Player_WriterF(buffer, npc->World.XYZ[i]);
			else
				Player_WriterF(buffer, player->DjW->XYZ[i]);
		}
		Player_Writer1(buffer, 1);
		Player_Writer4(buffer, 0);
		Player_Writer4(buffer, 0);

	return buffer;
}
