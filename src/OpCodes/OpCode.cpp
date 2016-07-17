#include "Sharun.hpp"

#include <dirent.h>

extern const char *OpCode_Gen[];

opcodes::~opcodes()
{
	for (int i=0; i < Max_OpCode_List; i++) {
		while (list[i].Unknow) {
			opcode_unk_t *tmp_OPC_Unk = list[i].Unknow;
			list[i].Unknow = list[i].Unknow->next;
			delete tmp_OPC_Unk->name;
			delete tmp_OPC_Unk;
		}
	}
}

opcodes::opcodes()
{
	size = 0;
	for (int i=0; i < Max_OpCode_List; i++) {
		memset(list[i].version, 0, 10);
		list[i].Unknow = NULL;
		list[i].detect = -1;
		for (int j=0; j < OPC_MAX; j++)
			list[i].Send[j] = -1;
		for (int j=0; j < PACKET_MAX_SIZE; j++) {
			list[i].OpName[j] = NULL;
			list[i].Recv[j] = NULL;
		}
	}
}

void opcodes::Set_Recv(const char *opcode_n, void* (*opcode_f)(void**))
{
	for (int i=0; i < Max_OpCode_List; i++) {
		if (strlen(list[i].version)) {
			int j=0;
			bool loop_me = true;
			while (loop_me && j < PACKET_MAX_SIZE) {
				if (list[i].OpName[j] && !strcmp(list[i].OpName[j], opcode_n)) {
					loop_me = false;
				} else
					j++;
			}
			if (j < PACKET_MAX_SIZE) {
				if (!strcmp(opcode_n, "C_LOGIN_ARBITER"))
					list[i].detect = j;
					list[i].Recv[j] = (void*(*)(player*, packet*)) opcode_f;
			}
		}
	}
}

void opcodes::Set_Send(int opcode_id, void* (*opcode_f)(void**))
{
	
	Send[opcode_id] = opcode_f;
}

void* opcodes::Recv(player* player_l, packet* packet_l)
{
	void *ret = NULL;
	int GID = -1;
	if (player_l)
		GID = player_l->OpCode_GID;
	else {
		for (int i=0; GID < 0 && i < Max_OpCode_List; i++) {
			if (strlen(list[i].version) && list[i].detect >= 0 && list[i].detect == packet_l->id) 
				GID = i;
		}
		if (GID < 0)
			return NULL;
	}
	ret = list[GID].Recv[packet_l->id](player_l, packet_l);
	if (!player_l && ret) {
		player_l = (player*) ret;
		player_l->OpCode_GID = GID;
	}
	return ret;
}

bool opcodes::load()
{
	DIR *pdir;
	FILE *fp;
	struct dirent *dirp;
	if ((pdir = opendir(Sharun->Settings.Dirs.OpCodes.c_str()))) {
		while ((dirp = readdir(pdir))) {
			if ((strcmp(dirp->d_name, ".")) && (strcmp(dirp->d_name, "..")) && (strcmp(dirp->d_name, ".")) && (!dir_exist(dirp->d_name)) && ((!strcmp(strrchr(dirp->d_name, '.'), ".txt")) || (!strcmp(strrchr(dirp->d_name, '.'), ".TXT")))) {
				std::string filename = Sharun->Settings.Dirs.OpCodes + "/" + dirp->d_name;
				if ((fp = fopen(filename.c_str(), "r"))) {
					memset(list[size].version, 0, 10);
					strncpy(list[size].version, dirp->d_name, strrchr(dirp->d_name, '.') - dirp->d_name);
					DEBUG("%s%s", size ? ", " : "Loading OpCode Files : ", list[size].version);
					char tmpS[100];
					int tmpI;
					while (fp && !feof(fp)) {
						tmpS[0] = 0;
						tmpI = 0;
						fscanf(fp, " %s = %i", tmpS, &tmpI);
						if (strlen(tmpS)) {
							int i=0;
							while (i < OPC_MAX && OpCode_Gen[i] && strcmp(OpCode_Gen[i], tmpS))
								i++;
							if (i < OPC_MAX) {
								list[size].OpName[tmpI] = OpCode_Gen[i];
								list[size].Send[i] = tmpI;
							} else {
								opcode_unk_t * tmp_OPC_Unk;
								tmp_OPC_Unk = list[size].Unknow;
								if (tmp_OPC_Unk) {
									while (tmp_OPC_Unk->next)
										tmp_OPC_Unk = tmp_OPC_Unk->next;
									tmp_OPC_Unk->next = new opcode_unk_t;
									tmp_OPC_Unk = tmp_OPC_Unk->next;
								} else
									tmp_OPC_Unk = list[size].Unknow = new opcode_unk_t;
								tmp_OPC_Unk->next = NULL;
								tmp_OPC_Unk->func = NULL;
								tmp_OPC_Unk->name = strdump(tmpS);
								tmp_OPC_Unk->code = tmpI;
							}
						}
					}
					size++;
				}
			}
		}
		closedir(pdir);
	}
	if (size)
		DEBUG(".\n");
	DEBUG("%s (%i) :: OpCodes loaded (%i files). ~\n", __FILE__, __LINE__, size);
	if (size)
		return true;
	return false;
}
