#include "Player.h"

void S_SKILL_LEARN_LIST_f(player_t *player)
{
	P_Buffer_t buffer;
	set_WBuffer(&buffer, S_SKILL_LEARN_LIST);
	Player_Writer4(&buffer, 524422);

	char *sql = "SELECT * FROM `skill_learn` WHERE `class` IN (-1, %i) AND `race` IN (-1, %i) AND `id`*10000+`lv`*100+`type` NOT IN (SELECT `skill_id`*10000+`lv`*100+`type` FROM `character_skill` WHERE `owner_id`='%i' AND `skill_id`=`id`) ORDER BY `id`, `lv`, `type`;";
	char Query[strlen(sql)+MAX_UINT*3];
	sprintf(Query, sql, player->Class_Id, player->Race_Id, player->Charcter_Id);
	void *rslt = SQL_Query_Fast(Query, 1);
	char **row;

/*
00 id
01 type
02 lv
03 class
04 race
05 active
06 plv
07 price
*/
	ushort replPos, nextPos = 0;
	while ((row = SQL_Next_Row(rslt))) {
		if (nextPos) Player_WriterSet2(&buffer, buffer.Pos, nextPos);

		Player_Writer2(&buffer, buffer.Pos);
		nextPos = buffer.Pos;
		Player_Writer2(&buffer, 0);

		if(atoi(row[2]) > 1)
			Player_Writer4(&buffer, 0);
		else {
			Player_Writer2(&buffer, 1);
			replPos = buffer.Pos;
			buffer.Pos += 2;
		}

		Player_Writer4(&buffer, 0);

		uint id = atoi(row[0])*10000 + atoi(row[2])*100 + atoi(row[1]);
		Player_Writer4(&buffer,id);// ид скила

		Player_Writer1(&buffer, atoi(row[5]));// ативный 01, пассивный 00

		Player_Writer4(&buffer, atoi(row[7]));// цена за изучение скила
		Player_Writer4(&buffer, atoi(row[6]));// минимальный лвл на изучение
		Player_Writer1(&buffer, 1);//SkillShopDialog.isLearneableSkill(player, currentSkills, skill, false)? 1 : 0); //может изучать 1 или неможет 0

		if(atoi(row[2]) > 1)
		{
			Player_WriterSet2(&buffer, buffer.Pos, replPos);
			Player_Writer2(&buffer, buffer.Pos);
			Player_Writer2(&buffer, 0);
			uint replid = atoi(row[0])*10000 + atoi(row[2])*100 + atoi(row[1])-1;
			Player_Writer4(&buffer, replid);// ид заменяемого скила
			Player_Writer1(&buffer, 1);// ативный 01, пассивный 00 заменяемый скилл
		}
	}

	SQL_Result_Clear(rslt);
	Player_Send(player, &buffer);
}
