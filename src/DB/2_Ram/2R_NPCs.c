#include "Player.h"

ZONE_t* Zone;

bool in_ZoneList(uint Zone_Id)
{
	ZONE_t *t_Zone = Zone;
	while (t_Zone) {
		if (t_Zone->Id == Zone_Id) 
			return true;
		t_Zone = t_Zone->next;
	}
	return false;
}

/*
00 id
01 name
02 huntingZone
03 class
04 level
05 attack
06 impact
07 defense
08 balance
09 hpBase
10 size
11 scale
12 skills
13 npc_id
14 npc_type
15 x
16 y
17 z
18 map_id
19 heading
*/

void init_NPC()
{
	Zone = NULL;
	char **row, *sql2 = "SELECT `Zones` FROM `Zones_HuntingZone` WHERE `Zones` NOT IN ( SELECT `from` FROM `Zones_Channeling`);";
//	char **row, *sql2 = "SELECT `map_id` FROM `npc_locs` WHERE `map_id`>4 GROUP BY `map_id` ORDER BY `map_id`;";
	void *rslt2 = NULL;
	char filename[strlen(Cfg_Server_DirSave)+20+1];
	
	ZONE_t * t_Zone = NULL;
	/*Zone = t_Zone = malloc(sizeof(ZONE_t));
	t_Zone->Id = 5;
	t_Zone->Friendly = NULL;
	t_Zone->Monsters = NULL;
	t_Zone->next = NULL;*/


	NPC_t* t_npc = NULL;
	sprintf(filename, "%s/DB/zones.db", Cfg_Server_DirSave);
	FILE *fp = fopen(filename, "rb");
	if (!fp) rslt2 = SQL_Query_Fast(sql2, 2);
	uint i, i2, i_zone=0;
	while ((fp && !feof(fp)) || (!fp && (row = SQL_Next_Row(rslt2)))) {
		uint	Id = 0;
		if (fp)
			fread(&Id, 1, 4, fp);
		else
			Id = atoi(row[0]);
		if (Id && !in_ZoneList(Id)) {
			if (!Zone)
				Zone = t_Zone = malloc(sizeof(ZONE_t));
			else {
				t_Zone->next = malloc(sizeof(ZONE_t));
				t_Zone = t_Zone->next;
			}
			t_Zone->Id = Id;
			t_Zone->Friendly = NULL;
			t_Zone->Monsters = NULL;
			t_Zone->next = NULL;
			i_zone++;
		}
	}

	DEBUG("%s (%i) :: %i Zones loaded%s.\n", __func__, __LINE__, i_zone, fp ? " (from file)": "");
	if (rslt2) SQL_Result_Clear(rslt2);
	if (fp) fclose(fp);

	i = 0, i2=0;
	//uint map_id = 0;
	char DBFile = 0;
	t_Zone = Zone;
	while (t_Zone) {
		sql2 = "SELECT * FROM `npcs`, `npc_locs` WHERE `class`='FRIENDLY' AND `id`=`npc_id` AND `map_id`='%i' AND `huntingZone`=`npc_type` AND `huntingZone` IN (SELECT `HuntingZone` FROM `Zones_HuntingZone` , `Zones_Channeling` WHERE (`Zones` = `from` AND `to` = '%i') OR `Zones` = '%i' GROUP BY `HuntingZone`)";
	//	sql2 = "SELECT * FROM `npcs`, `npc_locs` WHERE `class`='FRIENDLY' AND `id`=`npc_id` AND `map_id`=13 AND `huntingZone`=`npc_type` AND `huntingZone` IN (SELECT `HuntingZone` FROM `Zones_HuntingZone` , `Zones_Channeling` WHERE (`Zones` = `from` AND `to` = '13') OR `Zones` = '13' GROUP BY `HuntingZone`)";
	//	sql2 = "SELECT * FROM `npcs`, `npc_locs` WHERE `class`='FRIENDLY' AND `npc_id`=`id` AND `map_id`=13 AND `huntingZone`=`npc_type` AND `huntingZone`=64";
		char Query[strlen(sql2)+MAX_USHORT*3];
		sprintf(Query, sql2, t_Zone->Id, t_Zone->Id, t_Zone->Id);

		i2++;
		rslt2 = NULL;
		sprintf(filename, "%s/DB/npc_%i.db", Cfg_Server_DirSave, t_Zone->Id);
		fp = fopen(filename, "rb");		
		if (!fp) rslt2 = SQL_Query_Fast(Query, 1);
		else DBFile = 1;
		while ((fp && !feof(fp)) || (!fp && (row = SQL_Next_Row(rslt2)))) {
			uint	Id = 0;
			if (fp)
				fread(&Id, 1, 4, fp);
			else
				Id = atoi(row[0]);
			if (Id) {
//				printf("Zone %i/%i ; NPCs %i\r", i2, i_zone, i+1);
				/*map_id = atoi(row[18]);
				if (t_Zone && t_Zone->Id != map_id) {
					t_Zone = Zone;
					while (t_Zone->Id != map_id) t_Zone = t_Zone->next;
					t_npc = t_Zone->Friendly;
				}*/
				if (!t_Zone->Friendly)
					t_Zone->Friendly = t_npc = malloc(sizeof(NPC_t));
				else {
					while(t_npc->next) t_npc = t_npc->next;
					t_npc->next = malloc(sizeof(NPC_t));
					t_npc = t_npc->next;
				}

				t_npc->struct_type = 2;
				t_npc->Id = i;
				t_npc->SUBID = Cfg_PLAYER.NPC_FRENDLY_SUBID;
				t_npc->spawned = 1;
				t_npc->next = NULL;
				t_npc->skills = NULL;
				t_npc->World.Zone = t_Zone;
				t_npc->npc_id = Id;
				if (fp) {
					fread(&t_npc->npc_type, 1, 4, fp);
					fread(t_npc->name, 1, 141, fp);
					fread(&t_npc->huntingZone, 1, 4, fp);
					fread(&t_npc->level, 1, 2, fp);
					fread(&t_npc->hpBase, 1, 4, fp);
					fread(&t_npc->attack, 1, 4, fp);
					fread(&t_npc->impact, 1, 4, fp);
					fread(&t_npc->defense, 1, 4, fp);
					fread(&t_npc->balance, 1, 4, fp);
					for (short k=0; k<3; k++)
						fread(&t_npc->World.XYZ[k], 1, 4, fp);
					fread(&t_npc->World.heading, 1, 2, fp);
				} else {
					strcpy(t_npc->name, row[1]);
					for (short k=0; k<3; k++)
						t_npc->World.XYZ[k]	= atof2(row[16+k]);
					t_npc->World.heading	= atoi(row[19]);
					t_npc->npc_type		= atoi(row[2]);
					t_npc->huntingZone	= atoi(row[2]);
					t_npc->level		= atoi(row[4]);
					t_npc->hpBase		= atoi(row[9]);
					t_npc->attack		= atoi(row[5]);
					t_npc->impact		= atoi(row[6]);
					t_npc->defense		= atoi(row[7]);
					t_npc->balance		= atoi(row[8]);
				}
				for (short k=0; k<3; k++)
					t_npc->World.toXYZ[k]	= t_npc->World.XYZ[k];
				t_npc->World.toheading	= t_npc->World.heading;
				i++;
			}
		}

		t_Zone = t_Zone->next;
		if (rslt2) SQL_Result_Clear(rslt2);
		if (fp) fclose(fp);
	}
	DEBUG("%s (%i) :: %i NPCs loaded%s.\n", __func__, __LINE__, i, DBFile ? " (from file)": "");

	i = 0, i2=0;
	t_Zone = Zone;
	DBFile = 0;
	while (t_Zone) {
	sql2 = "SELECT * FROM `npcs`, `npc_locs` WHERE `class`<>'FRIENDLY' AND `id`=`npc_id` AND `map_id`='%i' AND `huntingZone`=`npc_type` AND `huntingZone` IN (SELECT `HuntingZone` FROM `Zones_HuntingZone` , `Zones_Channeling` WHERE (`Zones` = `from` AND `to` = '%i') OR `Zones` = '%i' GROUP BY `HuntingZone`)";
//	sql2 = "SELECT * FROM `npcs`, `npc_locs` WHERE `class`<>'FRIENDLY' AND `id`=`npc_id` AND `map_id`=13 AND `huntingZone`=`npc_type` AND `huntingZone` IN (SELECT `HuntingZone` FROM `Zones_HuntingZone` , `Zones_Channeling` WHERE (`Zones` = `from` AND `to` = '13') OR `Zones` = '13' GROUP BY `HuntingZone`)";
//	sql2 = "SELECT * FROM `npcs`, `npc_locs` WHERE `class`<>'FRIENDLY' AND `npc_id`=`id` AND `map_id`=13 AND `huntingZone`=`npc_type` AND `huntingZone`=64";
		char Query[strlen(sql2)+MAX_USHORT*3];
		sprintf(Query, sql2, t_Zone->Id, t_Zone->Id, t_Zone->Id);

		i2++;
		rslt2 = NULL;
		sprintf(filename, "%s/DB/mob_%i.db", Cfg_Server_DirSave, t_Zone->Id);
		fp = fopen(filename, "rb");
		if (!fp) rslt2 = SQL_Query_Fast(Query, 1);
		else DBFile = 1;
		while ((fp && !feof(fp)) || (!fp && (row = SQL_Next_Row(rslt2)))) {
			uint	Id = 0;
			if (fp)
				fread(&Id, 1, 4, fp);
			else
				Id = atoi(row[0]);
			if (Id) {
//				printf("Zone %i/%i ; Monsters %i\r", i2, i_zone, i+1);
				/*map_id = atoi(row[18]);
				if (t_Zone && t_Zone->Id != map_id) {
					t_Zone = Zone;
					while (t_Zone->Id != map_id) t_Zone = t_Zone->next;
					t_npc = t_Zone->Monsters;
				}*/
				if (!t_Zone->Monsters)
					t_Zone->Monsters = t_npc = malloc(sizeof(NPC_t));
				else {
					while(t_npc->next) t_npc = t_npc->next;
					t_npc->next = malloc(sizeof(NPC_t));
					t_npc = t_npc->next;
				}

				t_npc->struct_type = 2;
				t_npc->Id = i;
				t_npc->SUBID = Cfg_PLAYER.NPC_FRENDLY_SUBID;
				t_npc->spawned = 1;
				t_npc->next = NULL;
				t_npc->skills = NULL;
				t_npc->World.Zone = t_Zone;
				t_npc->npc_id = Id;
				if (fp) {
					fread(&t_npc->npc_id, 1, 4, fp);
					fread(&t_npc->npc_type, 1, 4, fp);
					fread(t_npc->name, 1, 141, fp);
					fread(&t_npc->huntingZone, 1, 4, fp);
					fread(&t_npc->level, 1, 2, fp);
					fread(&t_npc->hpBase, 1, 4, fp);
					fread(&t_npc->attack, 1, 4, fp);
					fread(&t_npc->impact, 1, 4, fp);
					fread(&t_npc->defense, 1, 4, fp);
					fread(&t_npc->balance, 1, 4, fp);
					for (short k=0; k<3; k++)
						fread(&t_npc->World.XYZ[k], 1, 4, fp);
					fread(&t_npc->World.heading, 1, 2, fp);
				} else {
					strcpy(t_npc->name, row[1]);
					for (short k=0; k<3; k++)
						t_npc->World.XYZ[k]	= atof2(row[16+k]);
					t_npc->World.heading	= atoi(row[19]);
					t_npc->npc_id		= atoi(row[0]);
					t_npc->npc_type		= atoi(row[2]);
					t_npc->huntingZone	= atoi(row[2]);
					t_npc->level		= atoi(row[4]);
					t_npc->hpBase		= atoi(row[9]);
					t_npc->attack		= atoi(row[5]);
					t_npc->impact		= atoi(row[6]);
					t_npc->defense		= atoi(row[7]);
					t_npc->balance		= atoi(row[8]);
				}
				for (short k=0; k<3; k++)
					t_npc->World.toXYZ[k]	= t_npc->World.XYZ[k];
				t_npc->World.toheading	= t_npc->World.heading;
				i++;
			}
		}

		t_Zone = t_Zone->next;
		if (rslt2) SQL_Result_Clear(rslt2);
		if (fp) fclose(fp);
	}
	DEBUG("%s (%i) :: %i Monsters loaded%s.\n", __func__, __LINE__, i, DBFile ? " (from file)": "");
}

void get_Zone(player_t *player, uint Zone_Id)
{
	player->World.Zone = NULL;
	ZONE_t *t_Zone = Zone;
	while (t_Zone) {
		if (t_Zone->Id == Zone_Id) {
			player->World.Zone = t_Zone;
			return;
		}
		t_Zone = t_Zone->next;
	}
}

void get_ZoneDJ(player_t *player, uint Zone_Id)
{
	player->DJ.Zone = NULL;
	ZONE_t *t_Zone = Zone;
	while (t_Zone) {
		if (t_Zone->Id == Zone_Id) {
			player->DJ.Zone = t_Zone;
			return;
		}
		t_Zone = t_Zone->next;
	}
}

void close_NPC()
{
	ZONE_t *t_Zone = NULL;
	NPC_t *t_npc = NULL;
	while (Zone) {
		t_Zone = Zone;
		while (t_Zone->Friendly) {
			t_npc = t_Zone->Friendly;
			t_Zone->Friendly = t_npc->next;
			free(t_npc);
		}
		while (t_Zone->Monsters) {
			t_npc = t_Zone->Monsters;
			t_Zone->Monsters = t_npc->next;
			free(t_npc);
		}
		Zone = Zone->next;
		free(t_Zone);
	}
}
