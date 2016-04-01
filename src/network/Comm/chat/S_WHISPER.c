#include "Player.h"

void S_WHISPER_f(player_t *player, player_t *TOplayer, char *text)
{
	P_Buffer_t buffer;
	set_WBuffer(&buffer, S_WHISPER);

	ushort Name_Pos = buffer.Pos;
	buffer.Pos += 2;
	ushort Name_Pos2 = buffer.Pos;
	buffer.Pos += 2;
	ushort Text_Pos = buffer.Pos;
	buffer.Pos += 2;

	if (player->Right > 4)
		Player_Writer1(&buffer, 1);

	Player_WriterSet2(&buffer, buffer.Pos, Name_Pos);
	Player_WriterS(&buffer, player->name, 36);//имя

	Player_WriterSet2(&buffer, buffer.Pos, Name_Pos2);
	Player_WriterS(&buffer, TOplayer->name, 36);//имя

	Player_WriterSet2(&buffer, buffer.Pos, Text_Pos);
	if (text)
		Player_WriterS2(&buffer, text);
	else
		Player_WriterS2(&buffer, "[system]Now, you can whispe with this player.");

	Player_Send(TOplayer, &buffer);

	if (text) {
		set_WBuffer(&buffer, S_WHISPER);

		ushort Name_Pos = buffer.Pos;
		buffer.Pos += 2;
		ushort Name_Pos2 = buffer.Pos;
		buffer.Pos += 2;
		ushort Text_Pos = buffer.Pos;
		buffer.Pos += 2;

		if (TOplayer->Right > 4)
			Player_Writer1(&buffer, 1);

		Player_WriterSet2(&buffer, buffer.Pos, Name_Pos);
		Player_WriterS(&buffer, player->name, 36);//имя

		Player_WriterSet2(&buffer, buffer.Pos, Name_Pos2);
		Player_WriterS(&buffer, TOplayer->name, 36);//имя

		Player_WriterSet2(&buffer, buffer.Pos, Text_Pos);
		Player_WriterS2(&buffer, text);

		Player_Send(player, &buffer);
	}
}
