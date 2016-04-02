#include "Player.h"

void *S_NPC_LOCATION_f(player_t *player, ushort type)
{
	P_Buffer_t * buffer = New_WBuffer(S_NPC_LOCATION);

	if (player) {
		Player_Writer4(buffer, player->Character_Id);
		Player_Writer4(buffer, player->SUBID);
	}

	//начальная точка
	for (int i=0; i<3; i++)
		Player_WriterF(buffer, player->DjW->XYZ[i]);

	//поворот
	Player_Writer2(buffer, player->DjW->heading);

	//скорость
	if (player->DjW->XYZ[0] == player->DjW->toXYZ[0] && player->DjW->XYZ[1] == player->DjW->toXYZ[1])
		Player_Writer2(buffer, 0);
	else {
		if ( type == MOVE_WALK)
			Player_Writer2(buffer, walk_speed);
		else {
			if (player) {
				/*switch (player->moveType[1] != MOVE_JUMP ? player->moveType[1] : player->moveType[0])
				{
					case MOVE_WALK:
					{
						Player_Writer2(buffer, walk_speed);
					}
					default:
					{*/
						Player_Writer2(buffer, player->mount ? player->mount->Speed : player->Stats.runSpeed[1]);
					/*}
				}*/
			}
		}
	}

	//конечная точка
	for (int i=0; i<3; i++)
		Player_WriterF(buffer, player->DjW->toXYZ[i]);

	//тип движения
	Player_Writer2(buffer, type);

	Player_Writer2(buffer, 0);

	return buffer;
}
