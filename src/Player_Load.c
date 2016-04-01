#include "Player.h"

void Player_Load(player_t *player)
{
	P_Buffer_t buffer;
	set_WBuffer(&buffer, S_LOGIN);

	char *sql = "SELECT * FROM `characters`, `character_appearances`, `character_detail_2` WHERE `character_appearances`.`object_id`=`characters`.`object_id` AND `character_detail_2`.`object_id`=`characters`.`object_id` AND `characters`.`object_id`='%i';";
	char Query[1024];
	sprintf(Query, sql, player->Charcter_Id);
	void *rslt = SQL_Query_Fast(Query, 1);
	char **row = SQL_Next_Row(rslt);

	player->Class_Id = atoi(row[2]);
	player->Race_Id = atoi(row[3]);
	player->Sex = atoi(row[4]);
	player->Level = atoi(row[14]);
	player->exp = atol(row[15]);

	get_Zone(player, atoi(row[26]));
	if (!player->World.Zone)
		get_Zone(player, 5);
	player->World.heading = atoi(row[6]);
	for (int i=0; i<3; i++) {
		player->World.XYZ[i] = atof2(row[18+i]);
		player->World.toXYZ[i] = atof2(row[18+i]);
	}
	player->DjW = &player->World;

	player->DJ.Zone = NULL;
	get_ZoneDJ(player, 0);
	if (player->DJ.Zone) {
		player->DJ.heading = 0;//atoi(row[6]);
		for (int i=0; i<3; i++) {
			player->DJ.XYZ[i] = 0;//atof2(row[18+i]);
			player->DJ.toXYZ[i] = 0;//atof2(row[18+i]);
		}
		player->DjW = &player->DJ;
	}

/**
 * LOAD ITEMS
 **/
	char ** row2, *sql2 = "SELECT `character_items`.*, `characters`.`char_name` FROM `character_items`, `characters` WHERE `characters`.`object_id`=`character_items`.`binded` AND `owner_id` = '%i'";
	sprintf(Query, sql2, player->Charcter_Id);
	void *rslt2 = SQL_Query_Fast(Query, 1);
	player->money = 0;
	player->Item_slot_count = 5;
	for (short index=0; index<39+20*8; index++)
		player->Item_slot[index].item = NULL;
	while ((row2 = SQL_Next_Row(rslt2))) {
		if (atoi(row2[6]) >= 0) {
			short index = atoi(row2[6]);
			player->Item_slot[index].id = 0;
			player->Item_slot[index].item = get_Item(	  atoi(row2[2]));
			if (player->Item_slot[index].item) {
				player->Item_slot[index].id		= atol(row2[0]);
				player->Item_slot[index].count		= atoi(row2[3]);
				player->Item_slot[index].enchant[0]	= atoi(row2[4]);
				player->Item_slot[index].enchant[1]	= atoi(row2[5]);
				for (int i=0; i<4; i++)
					player->Item_slot[index].crystal[i] = atoi(row2[7+i]);
				player->Item_slot[index].binded		= atoi(row2[11]);
				strcpy(player->Item_slot[index].binder,   row2[12]);
			} else
				printf("Item not found : \"%s\" on %i\n", row2[2], index);
		} else {
			uint ID = atoi(row2[2]);
			switch (ID) {
				case 20000000:
					player->money = atol(row2[3]);
					break;
				case 90000000:
					player->Item_slot_count = atoi(row2[3]);
					break;
			}
		}
	}
	SQL_Result_Clear(rslt2);

/**
 * LOAD SKILL 1
 ****
    owner_id
    skill_id
    type
    lv
    **/
	for (int j=0; j<50; j++) {
		player->skill[j].id = j;
		player->skill[j].type = 0;
		player->skill[j].level = 0;
	}
	sql2 = "SELECT * FROM `character_skill` WHERE `owner_id` = '%i'";
	sprintf(Query, sql2, player->Charcter_Id);
	rslt2 = SQL_Query_Fast(Query, 1);
	while ((row2 = SQL_Next_Row(rslt2)))
	{
		short ji = atoi(row2[1]);
		player->skill[ji].type = atoi(row2[2]);
		player->skill[ji].level = atoi(row2[3]);
	}
	SQL_Result_Clear(rslt2);

/**
 * LOAD SKILL 2
 ***
    owner_id
    skill_id
    **/
	player->skill2 = NULL;
	sql2 = "SELECT * FROM `character_skill2` WHERE `owner_id` = '%i' ORDER BY `skill_id`";
	sprintf(Query, sql2, player->Charcter_Id);
	rslt2 = SQL_Query_Fast(Query, 1);
	skill_2_t *skill2 = NULL;
	while ((row2 = SQL_Next_Row(rslt2)))
	{
		if (!skill2) {
			skill2 = malloc(sizeof(skill_2_t));
			player->skill2 = skill2;
		} else {
			skill2->next = malloc(sizeof(skill_2_t));
			skill2= skill2->next;
		}
		skill2->next = NULL;
		skill2->id = atoi(row2[1]);
	}
	SQL_Result_Clear(rslt2);

	strcpy(player->name, row[5]);
	strcpy(player->title, row[11]);
	for (int j = 0; j < 6; j++)
		player->appearance[j] = atoi(row[DB_detail_1+j]);
	for (int j = 0; j<32; j++)
		player->appearance[6+j] = atoi(row[DB_detail_1+6+j]);
	for (int j = 0; j < 64; j++)
		player->appearance[6+32+j] = atoi(row[DB_detail_1+6+32+1+j]);

	char **row3, *sql3 = "SELECT * FROM `template` WHERE `Server_ID` IN (0, %i) AND (`Class_ID` = %i OR (`Class_ID` = -1 AND `Race_ID` IN (-1, %i) AND `Sex` IN ( -1, %i))) ORDER BY `Server_ID` DESC";
	char Query3[strlen(sql3)+MAX_UINT*5];
	sprintf(Query3, sql3, Cfg_Server_ID, player->Class_Id, player->Race_Id, player->Sex);
	void *rslt3 = SQL_Query_Fast(Query3, 1);

	player->Stats.Stamina[1] = atoi(row[21]);
	player->Stats.Stamina[0] = player->Stats.Stamina[1];
	player->Stats.maxStamina = 120;
	player->Stats.HP = atoi(row[16]);
	player->Stats.MP = atoi(row[17]);
	for (int i=0; i<3; i++)
		player->collect[i] = atoi(row[29+i]);

	player->SUBID = Cfg_PLAYER.PLayer_SUBID;

	for (int i=0; i<STAT_NAME_MAX; i++)
		for (int j=0; j<2; j++)
			player->Stats.stat_detail[j][i] = j ? 1 : 0;

	for (int i=0; (row3 = SQL_Next_Row(rslt3)); i++)
		player->Stats.stat_detail[atoi(row3[3])][atoi(row3[4])] = atof2(row3[5]);

	if (Cfg_PLAYER.newHP)
		player->Stats.stat_detail[0][maxHP] *= 1.588;


	for (int i=0; i<STAT_NAME_MAX; i++)
		player->Stats.stat_detail[2][i] = player->Stats.stat_detail[0][i] * player->Stats.stat_detail[1][i];

	SQL_Result_Clear(rslt3);
	SQL_Result_Clear(rslt);
}
