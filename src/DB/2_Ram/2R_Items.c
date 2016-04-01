#include "Player.h"

Item_t* Items;
Item_t* Equipments;

/*
00 id
01 category
02 name
03 level
04 type
05 cooldown
06 rank
07 rareGrade
08 slotLimit
09 maxStack
10 tradable
11 destroyable
12 bankStorable
13 guildStorable
14 buyPrice
15 sellPrice
16 skill_id
17 bind
*/

void init_Items()
{
	Items = Equipments = NULL;
	char **row, *sql2 = "SELECT * FROM `common_items` ORDER BY `type`, `category`;";
	void *rslt2 = NULL;
	char filename[strlen(Cfg_Server_DirSave)+22+1];
	
	int i_item = 0;
	sprintf(filename, "%s/DB/common_items.db", Cfg_Server_DirSave); 
	FILE *fp = fopen(filename, "rb");
	if (!fp) rslt2 = SQL_Query_Fast(sql2, 2);
	Item_t * t_Item = NULL;
	while ((fp && !feof(fp)) || (!fp && (row = SQL_Next_Row(rslt2)))) {
		uint	Id = 0;
		if (fp)
			fread(&Id, 1, 4, fp);
		else
			Id = atoi(row[ 0]);
		if (Id) {
			if (!Items) {
				Items = t_Item = malloc(sizeof(Item_t));
				t_Item->next = NULL;
				i_item++;
			} else {
				t_Item->next = malloc(sizeof(Item_t));
				t_Item = t_Item->next;
				t_Item->next = NULL;
				i_item++;
			}

			t_Item->id = Id;
			if (fp) {
				fread(t_Item->category, 1, 30, fp);
				fread(&t_Item->level, 1, 1, fp);
				fread(t_Item->type, 1, 30, fp);
				fread(&t_Item->cooldown, 1, 2, fp);
				fread(&t_Item->rank, 1, 2, fp);
				fread(t_Item->rareGrade, 1, 10, fp);
				fread(&t_Item->slotLimit, 1, 1, fp);
				fread(&t_Item->maxStack, 1, 2, fp);

				fread(&t_Item->ability, 1, 1, fp);

				fread(&t_Item->buyPrice, 1, 4, fp);
				fread(&t_Item->sellPrice, 1, 4, fp);
				fread(&t_Item->skill_id, 1, 4, fp);
				fseek(fp, 1+2*4, SEEK_CUR);
				fread(&t_Item->bind, 1, 1, fp);
			} else {
				strcpy(t_Item->category, row[ 1]);
				t_Item->level	  = atoi(row[ 3]);
				strcpy(t_Item->type,	 row[ 4]);
				t_Item->cooldown  = atoi(row[ 5]);
				t_Item->rank	  = atoi(row[ 6]);
				strcpy(t_Item->rareGrade,row[ 7]);
				t_Item->slotLimit = atoi(row[ 8]);
				t_Item->maxStack  = atoi(row[ 9]);

				t_Item->ability   = 0;
				if (atoi(row[10])) t_Item->ability   |= ITEM_CAN_TRADE;
				if (atoi(row[11])) t_Item->ability   |= ITEM_CAN_DESTROY;
				if (atoi(row[12])) t_Item->ability   |= ITEM_CAN_BANK;
				if (atoi(row[13])) t_Item->ability   |= ITEM_CAN_GUILD;

				t_Item->buyPrice  = atoi(row[14]);
				t_Item->sellPrice = atoi(row[15]);
				t_Item->skill_id  = row[16] ? atoi(row[16]) : 0;
				t_Item->bind	  = atoi(row[17]);
			}

			t_Item->c_slots   = 0;
			t_Item->atk	  = 0;
			t_Item->def	  = 0;
			t_Item->bal	  = 0;
			t_Item->ipt	  = 0;
		}
	}
	DEBUG("%s (%i) :: %i Commons Items loaded%s.\n", __func__, __LINE__, i_item, fp ? " (from file)": "");
	if (rslt2) SQL_Result_Clear(rslt2);
	if (fp) fclose(fp);

/*
00 id
01 category
02 level
03 type
04 cooldown
05 rank
06 rareGrade
07 slotLimit
08 maxStack
09 crystal_slots
10 atk
11 def
12 balance
13 impact
14 tradable
15 destroyable
16 bankStorable
17 guildStorable
18 buyPrice
19 sellPrice
20 bind
*/
	Equipments = t_Item->next;
	sql2 = "SELECT * FROM `equipment_items` ORDER BY `type`, `category`;";
	rslt2 = NULL;
	i_item = 0;
	sprintf(filename, "%s/DB/equipment_items.db", Cfg_Server_DirSave); 
	fp = fopen(filename, "rb");
	if (!fp) rslt2 = SQL_Query_Fast(sql2, 2);
	while ((fp && !feof(fp)) || (!fp && (row = SQL_Next_Row(rslt2)))) {
		uint	Id = 0;
		if (fp)
			fread(&Id, 1, 4, fp);
		else
			Id = atoi(row[ 0]);
		if (Id) {
			if (!Items) {
				Equipments = Items = t_Item = malloc(sizeof(Item_t));
				t_Item->next = NULL;
				i_item++;
			} else {
				t_Item->next = malloc(sizeof(Item_t));
				t_Item = t_Item->next;
				if (!Equipments) Equipments = t_Item;
				t_Item->next = NULL;
				i_item++;
			}

			t_Item->id = Id;
			if (fp) {
				fread(t_Item->category, 1, 30, fp);
				fread(&t_Item->level, 1, 1, fp);
				fread(t_Item->type, 1, 30, fp);
				fread(&t_Item->cooldown, 1, 2, fp);
				fread(&t_Item->rank, 1, 2, fp);
				fread(t_Item->rareGrade, 1, 10, fp);
				fread(&t_Item->slotLimit, 1, 1, fp);
				fread(&t_Item->maxStack, 1, 2, fp);

				fread(&t_Item->ability, 1, 1, fp);

				fread(&t_Item->buyPrice, 1, 4, fp);
				fread(&t_Item->sellPrice, 1, 4, fp);
				fread(&t_Item->skill_id, 1, 4, fp);
				fread(&t_Item->c_slots, 1, 1, fp);
				fread(&t_Item->atk, 1, 2, fp);
				fread(&t_Item->def, 1, 2, fp);
				fread(&t_Item->bal, 1, 2, fp);
				fread(&t_Item->ipt, 1, 2, fp);
				fread(&t_Item->bind, 1, 1, fp);
			} else {
				strcpy(t_Item->category, row[ 1]);
				t_Item->level	  = atoi(row[ 2]);
				strcpy(t_Item->type,	 row[ 3]);
				t_Item->cooldown  = atoi(row[ 4]);
				t_Item->rank	  = atoi(row[ 5]);
				strcpy(t_Item->rareGrade,row[ 6]);
				t_Item->slotLimit = atoi(row[ 7]);
				t_Item->maxStack  = atoi(row[ 8]);

				t_Item->c_slots   = atoi(row[ 9]);
				t_Item->atk	  = atoi(row[10]);
				t_Item->def	  = atoi(row[11]);
				t_Item->bal	  = atoi(row[12]);
				t_Item->ipt	  = atoi(row[13]);

				t_Item->ability   = 0;
				if (atoi(row[14])) t_Item->ability   |= ITEM_CAN_TRADE;
				if (atoi(row[15])) t_Item->ability   |= ITEM_CAN_DESTROY;
				if (atoi(row[16])) t_Item->ability   |= ITEM_CAN_BANK;
				if (atoi(row[17])) t_Item->ability   |= ITEM_CAN_GUILD;

				t_Item->buyPrice  = atoi(row[18]);
				t_Item->sellPrice = atoi(row[19]);
				t_Item->skill_id  = 0;//row[16] ? atoi(row[16]) : 0;
				t_Item->bind	  = atoi(row[20]);
			}
		}
	}
	DEBUG("%s (%i) :: %i Equipments Items loaded%s.\n", __func__, __LINE__, i_item, fp ? " (from file)": "");
	if (rslt2) SQL_Result_Clear(rslt2);
	if (fp) fclose(fp);
}

Item_t * get_Item(uint Item_Id)
{
	Item_t * t_Item = Items;
	while (t_Item && t_Item->id != Item_Id)
		t_Item = t_Item->next;
	return t_Item;
}

Item_t * get_Equipment(uint Item_Id)
{
	Item_t * t_Item = Equipments;
	while (t_Item && t_Item->id != Item_Id)
		t_Item = t_Item->next;
	return t_Item;
}

void close_Items()
{
	Item_t * t_Item = NULL;
	while (Items) {
		t_Item = Items;
		Items = Items->next;
		free(t_Item);
	}
}
