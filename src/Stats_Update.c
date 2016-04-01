#include "Player.h" 

void Stats_Update(player_t *player)
{
	float stamina_factor = Stamina(player);

	player->Stats.attack[0] = 4;
	player->Stats.defense[0] = 2;
	player->Stats.impact[0] = 0;
	player->Stats.balance[0] = 0;


	player->Stats.maxHP[0] = player->Stats.stat_detail[2][maxHP] * HP_MOD[player->Class_Id][player->Level];

	player->Stats.maxMP[0] = player->Stats.stat_detail[2][maxMP];


	player->Stats.maxHP[1] = player->Stats.stat_detail[2][maxHP] * HP_MOD[player->Class_Id][player->Level] * stamina_factor;
	player->Stats.maxMP[1] = player->Stats.stat_detail[2][maxMP] * stamina_factor;

	if (Cfg_PLAYER.Stamina4spd) {
		player->Stats.atkSpeed[0] = player->Stats.stat_detail[2][atkSpd] * stamina_factor;
		player->Stats.runSpeed[0] = player->Stats.stat_detail[2][runSpd] * stamina_factor;
	} else {
		player->Stats.atkSpeed[0] = player->Stats.stat_detail[2][atkSpd];
		player->Stats.runSpeed[0] = player->Stats.stat_detail[2][runSpd];
	}
	player->Stats.atkSpeed[1] = player->Stats.atkSpeed[0];
	player->Stats.runSpeed[1] = player->Stats.runSpeed[0];

	// ADD WEAPON + CRISTAL MODDER


	player->Stats.attack[1] = player->Stats.attack[0] = 4;
	player->Stats.defense[1] = player->Stats.defense[0] = 2;
	player->Stats.impact[1] = player->Stats.impact[0] = 0;
	player->Stats.balance[1] = player->Stats.balance[0] = 0;

	for (int i=0; i<STAT_NAME_MAX; i++)
		player->Stats.stat_detail[3][i] = player->Stats.stat_detail[2][i];

	// ADD BUFF MODDER
}