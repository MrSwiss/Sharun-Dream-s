#include "Player.h"

void C_CHECK_USERNAME_f(player_t *player)
{
	Player_Reader2(player);
	char Name[100];
	Player_ReaderSI(player, Name);


	if ( strchr(Name, '[') || strchr(Name, ']') ||
	     strchr(Name, '(') || strchr(Name, ')') )
		S_CHECK_USERNAME_f(player, 0);
	else {

		SQL_Format_Char(Name);

		char *SQL = "SELECT COUNT(`object_id`) FROM `characters` WHERE `char_name` = '%s'";
		char Query[strlen(SQL)+strlen(Name)];
		sprintf(Query, SQL, Name);

		if(SQL_int(Query))
			S_CHECK_USERNAME_f(player, 0);
		else
			S_CHECK_USERNAME_f(player, 1);
	}

	Player_NextHeaderR(player);
}
