#include "Player.h"

void C_DELETE_USER_f(player_t *player)
{
	uint object_id = Player_Reader4(player);


	char ***rslt, Query[200];
	sprintf(Query, "SELECT COUNT(`object_id`) FROM `characters` WHERE `object_id`='%i' AND `end_ban`<UNIX_TIMESTAMP();", object_id);
	short result = SQL_short(Query);

	char *SQL[] = {
		"UPDATE `characters` AS C1, `characters` AS C2 SET `C1`.`Order`=`C1`.`Order`-1 WHERE `C2`.`object_id`='%i' AND `C1`.`Order` > `C2`.`Order`;",
		"DELETE FROM `characters` WHERE `object_id`='%i';",
		"DELETE FROM `character_appearances` WHERE `object_id`='%i';",
		"DELETE FROM `character_detail_2` WHERE `object_id`='%i';",
		"DELETE FROM `character_items` WHERE `owner_id`='%i';",
		NULL
	};

	if(!result) S_DELETE_USER_f(player, 0);
	else {
		for (int i=0; SQL[i]; i++) {
			sprintf(Query, SQL[i], object_id);
			if (i>1)
				SQL_Query_Slow(Query);
			else
				SQL_Query_Fast(Query, 0);
		}

		sprintf(Query, "SELECT COUNT(`object_id`) FROM `characters` WHERE `object_id`='%i';", object_id);
		rslt = SQL_Query_Fast(Query, 1);
		char **row = SQL_Next_Row(rslt);
		if (atoi(row[0])) S_DELETE_USER_f(player, 0);
		else S_DELETE_USER_f(player, 1);
		SQL_Result_Clear(rslt);
	}

	Player_NextHeaderR(player);
}
