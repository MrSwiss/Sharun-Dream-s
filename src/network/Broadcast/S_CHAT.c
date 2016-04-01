#include "Player.h"

void *S_CHAT_f(player_t *player, ushort type, char *text, char *name)
{
	P_Buffer_t * buffer = New_WBuffer(S_CHAT);

	switch (type) {
		case PARTY_CHAT:
			buffer->BC_Type = BC_TYPE_PARTY;
			break;
		case GUILD_CHAT:
			buffer->BC_Type = BC_TYPE_GUILD;
			break;
		case SHAUT_CHAT:
			buffer->BC_Type = BC_TYPE_ZONE;
			break;
		case RAID_CHAT:
		case RAID_LEADER_CHAT:
			buffer->BC_Type = BC_TYPE_RAID;
			break;
		case PRIVATE_CHAT:
		case WHISHPER_CHAT:
			buffer->BC_Type = BC_TYPE_TARGET;
			break;
		case MAIN_CHAT:
			buffer->BC_Type = BC_TYPE_ARROUND;
			break;
		default:
			buffer->BC_Type = BC_TYPE_MAIN;
	}

	Player_Writer2(buffer, 22);
	ushort Text_Pos = buffer->Pos;
	buffer->Pos += 2;
	Player_Writer4(buffer, type);

	if (player) {
		Player_Writer4(buffer, player->Charcter_Id);
		Player_Writer4(buffer, player->SUBID);
	} else {
		Player_Writer4(buffer, 0);
		Player_Writer4(buffer, 0);
	}

	Player_Writer1(buffer, 0);
	if (player && player->Right > 4)
		Player_Writer1(buffer, 1);// chat_icon
	else
		Player_Writer1(buffer, 0);// chat_icon

	if (name)
		Player_WriterS(buffer, name, 36);//имя

	Player_Writer1(buffer, 0);

	if(!player)
		Player_Writer2(buffer, 0x2000);

	Player_Writer1(buffer, 0);
	Player_WriterSet2(buffer, buffer->Pos, Text_Pos);
	if (!strstr(text, "<FONT>")) {
		char text2[6+strlen(text)+7+1];
		sprintf(text2, "<FONT>%s</FONT>", text);
		Player_WriterS2(buffer, text2);//текст
	} else
		Player_WriterS2(buffer, text);//текст

	Player_Writer1(buffer, 0);

	return buffer;
}

void S_CHAT_Back_to_Me_f(player_t *player, ushort type, char *text)
{
	P_Buffer_t * buffer = S_CHAT_f(player, type, text, player->name);
	Player_Send(player, buffer);
	free(buffer);
}

void S_CHAT_to_Me_f(player_t *player, ushort type, char *text)
{
	P_Buffer_t * buffer = S_CHAT_f(NULL, SYSTEM_CHAT, text, NULL);
	Player_Send(player, buffer);
	free(buffer);
}
