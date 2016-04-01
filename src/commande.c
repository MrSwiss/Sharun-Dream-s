#include "Player.h"

void World_Info(player_t *);

P_Buffer_t * command(player_t *player, char *b)
{
	P_Buffer_t * buffer = NULL;
	b += 3;
	char *e = strstr(b, "</FONT>");
	if (e && e-b > 0) {
		e[0] = 0;
		e = b;
		short argc = 0;
		while (e) {
			e = strchr(e+1, ' ');
			argc++;
		}
		char tmp[100] = {0};
		char *argv[argc];
		short i=0;
		argv[i++] = e = b;
		e = strchr(e+1, ' ');
		if (e) {
			argv[i++] = &e[1];
			e[0] = 0;
		}
		if (!strcmp(argv[0], "help")) {
			World_Info(player);
			S_CHAT_to_Me_f(player, SYSTEM_CHAT, "Help :\n --pm full name (for responce of a name with space)\n --add_item xxxID Qte\n --add_money xxxx\n --emote 11-52\n --system\n --notice\n --write 0-30 words_like_that");
		}
		else if (!strcmp(argv[0], "pm") && argc > 1) {
			for (int i=0; i<Player_Max; i++) {
				if ( Players[i]
				  && Players[i]->loged
				  && !strcmp(Players[i]->name, argv[1]) ) {
					S_WHISPER_f(Players[i], player, NULL);
					return NULL;
				}
			}
			if (100-20-strlen(argv[1]) > 0)
				sprintf(tmp, "\"%s\" is not online.", argv[1]);
			else
				sprintf(tmp, "Not online.");
		}
		else if (!strcmp(argv[0], "add_money") && argc > 1 && is_atoi(argv[1])) {
			player->money += atol(argv[1]);
			S_INVEN_f(player, 0);
		}
		else if (!strcmp(argv[0], "emote") && argc > 1 && is_atoi(argv[1]))
			World_PlayerSocial(player, atoi(argv[1]));
		else  if (!strcmp(argv[0], "system") && argc > 1)
			buffer = S_CHAT_f(player, SYSTEM_CHAT, argv[1], player->name);
		else if (!strcmp(argv[0], "notice") && argc > 1)
			buffer = S_CHAT_f(player, NOTICE_CHAT, argv[1], player->name);
		else {
			while (e) {
				e = strchr(e+1, ' ');
				if (e) {
					argv[i++] = &e[1];
					argc++;
					e[0] = 0;
				}
			}
			if (!strcmp(argv[0], "add_item") && argc > 2 && is_atoi(argv[1]) && is_atoi(argv[2])) {
				Add_Item(player, atoi(argv[1]), atoi(argv[2]));
				S_INVEN_f(player, 0);
			}
			else if (!strcmp(argv[0], "write")) {
				if (argc > 2) S_CHAT_Back_to_Me_f(player, atoi(argv[1]), argv[2]);
				else sprintf(tmp, "No arg for \"%s\" command !", argv[0]);
			} else
				sprintf(tmp, "Unknow command: \"%s\" !", argv[0]);
		}
		if (strlen(tmp))
			S_CHAT_to_Me_f(player, SYSTEM_CHAT, tmp);
	}
	return buffer;
}
