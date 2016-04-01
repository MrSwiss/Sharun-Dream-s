#include "Player.h"

void C_GET_USER_LIST_f(player_t *player)
{
		OpCodes_Write(player, S_GET_USER_LIST);
		OpCodes_Write(player, S_ACCOUNT_PACKAGE_LIST);
		OpCodes_Write(player, S_CONFIRM_INVITE_CODE_BUTTON);
		OpCodes_Write(player, S_UPDATE_CONTENTS_ON_OFF);
	Player_NextHeaderR(player);
}
