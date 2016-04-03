#include "Player.h"
#include <sys/time.h>

void regen_hp(player_t *player)
{
	float stamina_factor = Stamina(player);
	if (!player->Stats.HP && player->spawned<2) {
		player->spawned = 2;
		player->Stats.HP = 0;
		player->Stats.MP = 0;
		player->Stats.Stamina[0] = player->Stats.Stamina[1];
		player->Stats.Stamina[1] = 0;
		player->Stats.maxHP[1] = player->Stats.stat_detail[2][maxHP] * HP_MOD[player->Class_Id][player->Level] * stamina_factor;
		player->Stats.maxMP[1] = player->Stats.stat_detail[2][maxMP] * stamina_factor;
		S_CREATURE_CHANGE_HP_f(player, NULL, 0, 0);
		S_PLAYER_CHANGE_MP_f(player, NULL, 0, 0);
		if (Cfg_PLAYER.Stamina4spd) {
			player->Stats.atkSpeed[0] = player->Stats.stat_detail[2][atkSpd] * stamina_factor;
			player->Stats.runSpeed[0] = player->Stats.stat_detail[2][runSpd] * stamina_factor;
			S_PLAYER_STAT_UPDATE_f(player);
		}
		return;
	}
	if ((player->Stats.HP > 0 && player->Stats.HP < player->Stats.maxHP[1]) || player->Stats.HP > player->Stats.maxHP[1]) {
	int hp = player->Stats.stat_detail[2][regHp] * HP_MOD[player->Class_Id][player->Level];
	if (player->mount && player->mount->R_HP) hp += player->Stats.maxHP[0] * 1.005;
		if (player->Stats.HP + hp > player->Stats.maxHP[1] || player->Stats.HP > player->Stats.maxHP[1])
			player->Stats.HP = player->Stats.maxHP[1];
		else player->Stats.HP += hp;
		player->Stats.maxHP[1] = player->Stats.stat_detail[2][maxHP] * HP_MOD[player->Class_Id][player->Level] * stamina_factor;
		S_CREATURE_CHANGE_HP_f(player, NULL, 0, 0);
	}
}

void regen_mp(player_t *player)
{
	if (!player->Stats.HP) return;
	int mp = player->Stats.stat_detail[2][regMp];
	if (player->mount && player->mount->R_MP) mp += player->Stats.maxMP[0] * 1.005;
	if ((mp>0 && player->Stats.MP < player->Stats.maxMP[1]) || (mp<0 && player->Stats.MP >0 ) || player->Stats.MP > player->Stats.maxMP[1]) {
		if (mp<0 && player->Stats.MP < (mp<0?-(mp):mp))
			player->Stats.MP = 0;
		else if ((mp>0 && player->Stats.MP + mp > player->Stats.maxMP[1]) || player->Stats.MP > player->Stats.maxMP[1])
			player->Stats.MP = player->Stats.maxMP[1];
		else player->Stats.MP += mp;
		float stamina_factor = Stamina(player);
		player->Stats.maxMP[1] = player->Stats.stat_detail[2][maxMP] * stamina_factor;
		S_PLAYER_CHANGE_MP_f(player, NULL, 0, 0);
	}
}

void regen_sp(player_t *player)
{
	if (!player->Stats.HP) return;
	int sp = 0;
	if ((player->Stats.Stamina[1] < 120 || (0 && player->Stats.Stamina[1] < 130)) && player->mount && player->mount->R_Stamina) sp = 1;
	player->Stats.Stamina[1] += sp;
	if (player->Stats.Stamina[0] != player->Stats.Stamina[1]) {
		float stamina_factor = Stamina(player);
		if (Cfg_PLAYER.Stamina4spd) {
			player->Stats.atkSpeed[0] = player->Stats.stat_detail[2][atkSpd] * stamina_factor;
			player->Stats.runSpeed[0] = player->Stats.stat_detail[2][runSpd] * stamina_factor;
			S_PLAYER_STAT_UPDATE_f(player);
		}
		player->Stats.Stamina[0] = player->Stats.Stamina[1];
	}
}

void Regen_Player(player_t *player)
{
	char regen_time = 0;
	while (player->Account_Id) {
		long l_time = microsec();
		if (!Player_have_crashed(player)) {
			if (player->loged) {
				if (player->exit_type && player->exit_time < time(NULL)) {
					if (player->exit_type == 1)
						OpCodes_Write(player, S_EXIT);
					else
						OpCodes_Write(player, S_RETURN_TO_LOBBY);
				}
				if (regen_time > 1) {
					regen_hp(player);
					regen_mp(player);
					regen_sp(player);
					regen_time = 0;
				} else regen_time++;
			}
			if (player->loged && player->Saved_Time + 60 < time(NULL)) {
				Character_Save(player);
				player->Saved_Time = time(NULL);
			}
		}
		usleep((l_time + 1000000 - microsec()));
	}
	c_THREAD_free(NULL);
}
