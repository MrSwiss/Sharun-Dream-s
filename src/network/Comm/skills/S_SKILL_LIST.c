#include "Player.h"

void S_SKILL_LIST_f(player_t *player)
{
	P_Buffer_t buffer;
	set_WBuffer(&buffer, S_SKILL_LIST);

	ushort skillsint = 0, last = 0, here = buffer.Pos;
	buffer.Pos += 2;
	Player_Writer2(&buffer, 8);

	for (int j=0; j<50; j++)
	{
		if (player->skill[j].level) {
			if (last)
				Player_WriterSet2(&buffer, buffer.Pos, last);
			Player_Writer2(&buffer, buffer.Pos);
			last = buffer.Pos;
			Player_Writer2(&buffer, 0);
			Player_Writer4(&buffer, player->skill[j].id*10000 + player->skill[j].level*100 + player->skill[j].type);
			Player_Writer2(&buffer, 1); // active
			skillsint++;
		}
	}

	skill_2_t *skill2 = player->skill2;
	while (skill2)
	{
		if (last)
			Player_WriterSet2(&buffer, buffer.Pos, last);
		Player_Writer2(&buffer, buffer.Pos);
		last = buffer.Pos;
		Player_Writer2(&buffer, 0);
		Player_Writer4(&buffer, skill2->id);
		Player_Writer2(&buffer, 1); // active
		skill2 = skill2->next;
		skillsint++;
	}

	Player_WriterSet2(&buffer, skillsint, here);

	Player_Send(player, &buffer);
}
