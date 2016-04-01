#include "Player.h"

void S_PLAYER_STAT_UPDATE_f(player_t *player)
{
	P_Buffer_t buffer;
	set_WBuffer(&buffer, S_PLAYER_STAT_UPDATE);

	Player_Writer4(&buffer, player->Stats.HP); // Actuel HP
	Player_Writer4(&buffer, player->Stats.MP); // Actuel MP

	Player_Writer4(&buffer, 0);

	Player_Writer4(&buffer, player->Stats.maxHP[1]); // Max HP
	Player_Writer4(&buffer, player->Stats.maxMP[1]); // Max MP

	Player_Writer4(&buffer, player->Stats.stat_detail[Unbuffed][powerFactor]);
	Player_Writer4(&buffer, player->Stats.stat_detail[Unbuffed][defenceFactor]);
	Player_Writer2(&buffer, player->Stats.stat_detail[Unbuffed][impactFactor]);
	Player_Writer2(&buffer, 0);//0000 // new
	Player_Writer2(&buffer, player->Stats.stat_detail[Unbuffed][balanceFactor]);
	Player_Writer2(&buffer, 0);//0000 // new
	Player_Writer2(&buffer, player->Stats.runSpeed[0]); // базовая скорость бега
	Player_Writer2(&buffer, walk_speed);//2800 Walk speed
	Player_Writer2(&buffer, player->Stats.atkSpeed[0]);

	Player_WriterF(&buffer, player->Stats.stat_detail[Unbuffed][critRate]); // шанс крита
	Player_WriterF(&buffer, player->Stats.stat_detail[Unbuffed][critRcpt]); // защита от крита (пока не ясно кд или шанс режет)
	Player_WriterF(&buffer, player->Stats.stat_detail[Unbuffed][critDmg]); // крит дамаг
	Player_Writer4(&buffer, player->Stats.attack[0]); // базовая атака мин
	Player_Writer4(&buffer, player->Stats.attack[0]); // базовая атака макс
	Player_Writer4(&buffer, player->Stats.defense[0]);
	Player_Writer2(&buffer, player->Stats.impact[0]);
	Player_Writer2(&buffer, 0);//0000 // new
	Player_Writer2(&buffer, player->Stats.balance[0]);
	Player_Writer2(&buffer, 0);//0000 // new
	Player_WriterF(&buffer, player->Stats.stat_detail[Unbuffed][weakRcpt]);// (Hex)Сопротивление к Ядам 38
	Player_WriterF(&buffer, player->Stats.stat_detail[Unbuffed][dmgRcpt]);// (Hex)Сопротивление к повреждениям 38
	Player_WriterF(&buffer, player->Stats.stat_detail[Unbuffed][stunRcpt]);// (Hex)Сопротивление к обиздвиживанию 38

	Player_Writer4(&buffer, player->Stats.stat_detail[Buffed][powerFactor] - player->Stats.stat_detail[Unbuffed][powerFactor]); // бонус к повер фактору
	Player_Writer4(&buffer, player->Stats.stat_detail[Buffed][defenceFactor] - player->Stats.stat_detail[Unbuffed][defenceFactor]); // бонус к дефенс фактору
	Player_Writer2(&buffer, player->Stats.stat_detail[Buffed][impactFactor] - player->Stats.stat_detail[Unbuffed][impactFactor]); // бонус к импакт фактору
	Player_Writer2(&buffer, 0);//0000 // new
	Player_Writer2(&buffer, player->Stats.stat_detail[Buffed][balanceFactor] - player->Stats.stat_detail[Unbuffed][balanceFactor]); // бонус к баланс фактору
	Player_Writer2(&buffer, 0);//0000 // new
	Player_Writer2(&buffer, (player->mount ? player->mount->Speed : player->Stats.runSpeed[1]) - player->Stats.runSpeed[0]); // Бонус к скорости бега...
	Player_Writer2(&buffer, 0);//0000 // new
	Player_Writer2(&buffer, player->Stats.atkSpeed[1] - player->Stats.atkSpeed[0]); // бонус к атак спиду

	Player_WriterF(&buffer, player->Stats.stat_detail[Buffed][critRate] - player->Stats.stat_detail[Unbuffed][critRate]); // крит рейт бонус
	Player_WriterF(&buffer, player->Stats.stat_detail[Buffed][critRcpt] - player->Stats.stat_detail[Unbuffed][critRcpt]); // крит ресист бонус
	Player_WriterF(&buffer, player->Stats.stat_detail[Buffed][critDmg] - player->Stats.stat_detail[Unbuffed][critDmg]); // крит мощность бонус
	Player_Writer4(&buffer, player->Stats.attack[1] - player->Stats.attack[0]); // бонус к атаке мин
	Player_Writer4(&buffer, player->Stats.attack[1] - player->Stats.attack[0]); // бонус к атаке макс
	Player_Writer4(&buffer, player->Stats.defense[1] - player->Stats.defense[0]); // бонус к защите
	Player_Writer2(&buffer, player->Stats.impact[1] - player->Stats.impact[0]); // бонус к импакту
	Player_Writer2(&buffer, player->Stats.balance[1] - player->Stats.balance[0]); // бонус к балансу

	Player_WriterF(&buffer, player->Stats.stat_detail[Buffed][weakRcpt] - player->Stats.stat_detail[Unbuffed][weakRcpt]);
	Player_WriterF(&buffer, player->Stats.stat_detail[Buffed][dmgRcpt] - player->Stats.stat_detail[Unbuffed][dmgRcpt]);
	Player_WriterF(&buffer, player->Stats.stat_detail[Buffed][stunRcpt] - player->Stats.stat_detail[Unbuffed][stunRcpt]);
	Player_Writer2(&buffer, 0);//0000 // new
	Player_Writer2(&buffer, 0);//0000 // new
	Player_Writer2(&buffer, player->Level);
	Player_Writer2(&buffer, /*player.isBattleStanced() ? 1 : */0);
	Player_Writer2(&buffer, 4);
	Player_Writer1(&buffer, 1);
	Player_Writer4(&buffer, player->Stats.maxHP[1] - player->Stats.maxHP[0]);
	Player_Writer4(&buffer, player->Stats.maxMP[1] - player->Stats.maxMP[0]);
	Player_Writer4(&buffer, player->Stats.Stamina[1]);
	Player_Writer4(&buffer, player->Stats.maxStamina);
	Player_Writer4(&buffer, 0);
	Player_Writer4(&buffer, 0);
	Player_Writer4(&buffer, 0);
	Player_Writer4(&buffer, 0);// карма

	Player_Writer4(&buffer, 0);// Уровень брони
	Player_Writer4(&buffer, 0);// Уровень брони
	Player_Writer8(&buffer, 0);
	Player_Writer4(&buffer, 8000);
	Player_Writer4(&buffer, 0x00000003);//03000000
	Player_Writer4(&buffer, 1);

	Player_Send(player, &buffer);
}
