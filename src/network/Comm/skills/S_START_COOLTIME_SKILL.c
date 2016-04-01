#include "Player.h"

void S_START_COOLTIME_SKILL_f(player_t *player, uint Skill_Id, uint Delay)
{
	P_Buffer_t buffer;
	set_WBuffer(&buffer, S_START_COOLTIME_SKILL);

	Player_Writer4(&buffer, Skill_Id);
	Player_Writer4(&buffer, Delay);

	Player_Send(player, &buffer);
}
