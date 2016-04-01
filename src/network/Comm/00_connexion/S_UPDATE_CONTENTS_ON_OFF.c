#include "Player.h"

void S_UPDATE_CONTENTS_ON_OFF_f(player_t *player)
{
	P_Buffer_t buffer;
	set_WBuffer(&buffer, S_UPDATE_CONTENTS_ON_OFF);

	buffer.Pos++;
	Player_Writer4(&buffer, 0);
	
	for (uchar i=2; i<5; i++) {
		Player_WriterSet1(&buffer, i, 4);
		P_Buffer_t bufferTmp;
		memcpy(&bufferTmp, &buffer, sizeof(P_Buffer_t));
		Player_Send(player, &bufferTmp);
	}
}
