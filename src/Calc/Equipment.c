#include "Player.h"


int DB_getEquipment(void *DB_row, enum SlotType Slot, uint owner_id)
{
	char ** row;
	SQL_Seek(DB_row, 0);
	while ((row = SQL_Next_Row(DB_row))) {
		if (atoi(row[0]) == owner_id && atoi(row[2]) == Slot) return atol(row[1]);
	}
	return 0;
}

char DB_getEquipment_Ench(void *DB_row, enum SlotType Slot, uint owner_id)
{
	char ** row;
	SQL_Seek(DB_row, 0);
	while ((row = SQL_Next_Row(DB_row))) {
		if (atoi(row[0]) == owner_id && atoi(row[2]) == Slot) return atoi(row[3]);
	}
	return 0;
}

char DB_getEquipment_Max(void *DB_row, enum SlotType Slot, uint owner_id)
{
	char ** row;
	SQL_Seek(DB_row, 0);
	while ((row = SQL_Next_Row(DB_row))) {
		if (atoi(row[0]) == owner_id && atoi(row[2]) == Slot) {
			char lvl = atoi(row[3]);
			char max = atoi(row[4]);
			return lvl > max ? lvl : max;
		}
	}
	return 0;
}
