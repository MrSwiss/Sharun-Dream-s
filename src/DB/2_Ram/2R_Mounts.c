#include "Player.h"

mount_t * Mounts;

void init_Mounts()
{
	Mounts = NULL;
	char **row, *sql2 = "SELECT * FROM `mounts` ORDER BY `Skill_Id`";
	void *rslt2 = NULL;
	char filename[strlen(Cfg_Server_DirSave)+13+1];

	mount_t * M_tmp;
	int i=0;
	sprintf(filename, "%s/DB/mounts.db", Cfg_Server_DirSave); 
	FILE *fp = fopen(filename, "rb");
	if (!fp) rslt2 = SQL_Query_Fast(sql2, 2);
	while ((fp && !feof(fp)) || (!fp && (row = SQL_Next_Row(rslt2)))) {
		uint	Id = 0;
		if (fp)
			fread(&Id, 1, 4, fp);
		else
			Id = atol(row[0]);
		if (Id) {
			if (!Mounts)
				Mounts = M_tmp = malloc(sizeof(mount_t));
			else {
				M_tmp->next = malloc(sizeof(mount_t));
				M_tmp = M_tmp->next;
			}
			M_tmp->Skill_Id = Id;
			if (fp) {
				fread(&M_tmp->Mount_Id, 1, 2, fp);
				fread(&M_tmp->Speed, 1, 2, fp);
				fread(&M_tmp->R_Stamina, 1, 1, fp);
				fread(&M_tmp->R_HP, 1, 1, fp);
				fread(&M_tmp->R_MP, 1, 1, fp);
			} else {
				M_tmp->Mount_Id =	atoi(row[1]);
				M_tmp->Speed =		atoi(row[2]);
				M_tmp->R_Stamina =	atoi(row[2]);
				M_tmp->R_HP =		atoi(row[3]);
				M_tmp->R_MP =		atoi(row[4]);
			}
			M_tmp->next = NULL;
			i++;
		}
	}

	DEBUG("%s (%i) :: %i Mounts loaded%s.\n", __func__, __LINE__, i, fp ? " (from file)": "");
	if (rslt2) SQL_Result_Clear(rslt2);
	if (fp) fclose(fp);
}

mount_t * get_Mount(player_t *player, uint Skill_Id)
{
	mount_t * M_tmp = Mounts;
	while (M_tmp && M_tmp->Skill_Id != Skill_Id && M_tmp->Skill_Id <= Skill_Id) {
		M_tmp = M_tmp->next;
	}
	if (M_tmp && M_tmp->Skill_Id <= Skill_Id)
		return  M_tmp;
	return NULL;
}

void close_Mounts()
{
	mount_t * M_tmp;
	while (Mounts) {
		M_tmp = Mounts;
		Mounts = Mounts->next;
		free(M_tmp);
	}
}
