#include "Player.h"

void S_CREATE_USER_f_Fail(player_t*);

void C_CREATE_USER_f(player_t *player)
{
	char *SQL[] = {
		"INSERT INTO `characters` (`create_time`, `AccoundId`, `sex`, `race_id`, `class_id`, `char_name`, `Order`, `Server_ID`) VALUE (UNIX_TIMESTAMP(), '%i', '%i', '%i', '%i', '%s', '%i', '%i');",
		"SELECT `object_id` FROM `characters` WHERE `AccoundId`='%i' AND `char_name`='%s' AND `Server_ID` IN (0, %i);",
		"INSERT INTO `character_appearances` VALUE ('%ld', '%i', '%i', '%i', '%i', '%i', '%i'",
		"INSERT INTO `character_detail_2` VALUE ('%ld'",
		"INSERT INTO `character_items` (`owner_id`, `item_id`, `item_count`, `autor`, `location`, `index`) SELECT %ld, `Item_ID`, `Count`, 'CreatePlayer', `location`, `index` FROM `Noobs` WHERE `location`=0 OR (`Class_ID`=%i AND `Material_ID`=-1) OR (`Class_ID`=-1 AND `Material_ID`=%i)",
		"UPDATE `characters` SET `x`='-48077', `y`='-52002', `z`='642', `heading`='45330', `zone_id`='7087', `level`='50' WHERE `object_id`='%i';"
	};

	ushort Name_Pos = Player_Reader2(player);
	ushort Detail1_Pos = Player_Reader2(player);
	/*ushort Detail1_Len = */Player_Reader2(player);
	ushort Detail2_Pos = Player_Reader2(player);
	/*ushort Detail2_Len = */Player_Reader2(player);

	uint sex = Player_Reader4(player);
	uint race = Player_Reader4(player);
	uint class = Player_Reader4(player);

	Player_Reader1(player);

	ushort FaceColor = Player_Reader1(player);
	ushort FaceSkin = Player_Reader1(player);
	ushort AdormentsSkin = Player_Reader1(player);
	ushort FeaturesSkin = Player_Reader1(player);
	ushort FeaturesColor = Player_Reader1(player);
	ushort Voice = Player_Reader1(player);

	/*Player_Reader1(player);

	Player_Reader2(player);
	Player_Reader2(player);
	Player_Reader2(player);*/

	player->Packet_R_Pos = player->Packet_R_HeadPos + Name_Pos;
	char Name[100];
	Player_ReaderSI(player, Name);

	if ( strchr(Name, '[') || strchr(Name, ']') ||
	     strchr(Name, '(') || strchr(Name, ')') )
		S_CREATE_USER_f_Fail(player);
	else {
		char Query[2000];
		SQL_Format_Char(Name);

		sprintf(Query, SQL[0], player->Account_Id, sex, race, class, Name, player->Charcters_Count, Cfg_Server_ID);
		SQL_Query_Fast(Query, 0);

		sprintf(Query, SQL[1], player->Account_Id, Name, Cfg_Server_ID);
		uint object_id = SQL_int(Query);

		player->Packet_R_Pos = player->Packet_R_HeadPos + Detail1_Pos;
		sprintf(Query, SQL[2], object_id, FaceColor, FaceSkin, AdormentsSkin, FeaturesSkin, FeaturesColor, Voice);
		for (int i=0; i<32; i++)
			sprintf(&Query[strlen(Query)], ", '%i'", (ushort)Player_Reader1(player));
		sprintf(&Query[strlen(Query)], ");");
		SQL_Query_Fast(Query, 0);

		player->Packet_R_Pos = player->Packet_R_HeadPos + Detail2_Pos;
		sprintf(Query, SQL[3], object_id);
		for (int i=0; i<64; i++)
			sprintf(&Query[strlen(Query)], ", '%i'", (ushort)Player_Reader1(player));
		sprintf(&Query[strlen(Query)], ");");
		SQL_Query_Fast(Query, 0);

		// Add item to character
		sprintf(Query, SQL[4], object_id, class, class == REAPER ? -1 : class_mat[class]);
		SQL_Query_Fast(Query, 0);

		if (class == REAPER) {
			sprintf(Query, SQL[5], object_id);
			SQL_Query_Fast(Query, 0);
		}

		OpCodes_Write(player, S_CREATE_USER);
	}
	Player_NextHeaderR(player);
}
