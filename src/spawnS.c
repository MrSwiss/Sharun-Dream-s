#include "Player.h"

void spawS_add(player_t *player, void *link)
{
	Mutex_Lock(player->spawS_locker);
	spawS_t *spawS = player->spawS;
	if (!spawS) {
		spawS = player->spawS = malloc(sizeof(spawS_t));
		spawS->link = NULL;
	} else {
		spawS = player->spawS;
		while (spawS->next && spawS->link != link)
			spawS = spawS->next;
		if (spawS->link != link) {
			spawS->next = malloc(sizeof(spawS_t));
			spawS = spawS->next;
			spawS->link = NULL;
		}
	}
	if (spawS->link != link) {
		spawS->link = link;
		spawS->next = NULL;
	}
	Mutex_Unlock(player->spawS_locker);
}

bool is_spawS(player_t *player, void *link)
{
	Mutex_Lock(player->spawS_locker);
	spawS_t *spawS = player->spawS;
	while (spawS && spawS->next && spawS->link != link)
		spawS = spawS->next;
	bool ret = 0;
	if (spawS && spawS->link == link)
		ret = 1;
	Mutex_Unlock(player->spawS_locker);
	return ret;
}

void spawS_del(player_t *player, void *link)
{
	Mutex_Lock(player->spawS_locker);
	spawS_t *spawS_f = NULL, *spawS = player->spawS;
	while (spawS && spawS->next) {
		spawS_f = spawS->next;
		if (spawS_f && spawS_f->link == link)
			break;
		else
			spawS = spawS->next;
	}
	if (spawS && spawS_f && spawS_f->link == link) {
		spawS->next = spawS_f->next;
		free(spawS_f);
	}
	Mutex_Unlock(player->spawS_locker);
}

void spawS_free(player_t *player)
{
	Mutex_Lock(player->spawS_locker);
	spawS_t *spawS = player->spawS;
	while (spawS) {
		player->spawS = spawS->next;
		free(spawS);
		spawS = player->spawS;
	}
	Mutex_Unlock(player->spawS_locker);
}
