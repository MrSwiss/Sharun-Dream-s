#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "globale.h"
#include "OpCodes/OpCodes.h"

extern void*	Players_mtx;
extern player_t*	Players[Player_Max];

void	S_LOGIN_FAILED_f(player_t *player);

#define Player_Writer1(buffer, data) Player_WriterNB(buffer, data, 1)
#define Player_Writer2(buffer, data) Player_WriterNB(buffer, data, 2)
#define Player_Writer4(buffer, data) Player_WriterNB(buffer, data, 4)
#define Player_Writer8(buffer, data) Player_WriterNB(buffer, data, 8)

#define Player_WriterSet1(buffer, data, pos) Player_WriterSet(buffer, data, 1, pos)
#define Player_WriterSet2(buffer, data, pos) Player_WriterSet(buffer, data, 2, pos)
#define Player_WriterSet4(buffer, data, pos) Player_WriterSet(buffer, data, 4, pos)
#define Player_WriterSet8(buffer, data, pos) Player_WriterSet(buffer, data, 8, pos)

#define Player_Reader1(player) Player_ReaderNB(player, 1)
#define Player_Reader2(player) Player_ReaderNB(player, 2)
#define Player_Reader4(player) Player_ReaderNB(player, 4)
#define Player_Reader8(player) Player_ReaderNB(player, 8)

extern int	class_mat[];
extern float	HP_CLASS_MOD[CLASS_MAX];
extern ulong	Experience_LEVEL[];

extern float	HP_MOD[CLASS_MAX][LV_Max+1];

short	Player_have_crashed(player_t *);
void	Player_Delog(player_t *);
void	Player_Exit(player_t *);

void	spawS_add(player_t *, void *);
bool	is_spawS(player_t *, void *);
void	spawS_del(player_t *, void *);
void	spawS_free(player_t *);

void	Buffer_Writer(byte *buffer, byte *data, ushort *pos, ushort size);
void	Player_Writer(P_Buffer_t *, byte *data, ushort size);
void	Player_WriterNB(P_Buffer_t *, uint64_t data, ushort size);
void	Player_WriterF(P_Buffer_t *, float data);
void	Player_WriterS(P_Buffer_t *, char *data, int len);
void	Player_WriterS2(P_Buffer_t *, char *data);
void	Player_WriterSet(P_Buffer_t *, uint64_t data, ushort size, ushort pos);
void	Player_UpdateHeaderS(P_Buffer_t *);
P_Buffer_t*	New_WBuffer(ushort);
void	set_WBuffer(P_Buffer_t *buffer, ushort);

void	Buffer_Reader(byte *buffer, byte *data, ushort *pos, ushort size);
void	Player_Reader(player_t *, byte *data, ushort size);
ulong	Player_ReaderNB(player_t *, ushort size);
float	Player_ReaderF(player_t *);
char*	Player_ReaderS(player_t *);
void	Player_ReaderSI(player_t *, char *data);
void	Player_NextHeaderR(player_t *);

void	HTTP_Work(netlink_t *);

void	Player_Login(netlink_t *);
void	Player_Recv(player_t *);
void	Player_Send(player_t *, P_Buffer_t *);
void	Player_Send_Fast(player_t *, ushort);

mount_t*	get_Mount(player_t *, uint);

void	Player_Load(player_t *);

void	get_Zone(player_t *, uint);
void	get_ZoneDJ(player_t *, uint);


void	Add_Item(player_t *, uint ID, short count);

Item_t*	get_Item(uint);
Item_t*	get_Equipment(uint);

int	DB_getEquipment(void *, enum SlotType, uint);
char	DB_getEquipment_Ench(void *, enum SlotType, uint);
char	DB_getEquipment_Max(void *, enum SlotType, uint);


void	Broadcast(P_Buffer_t *);
void	Greet(player_t *, ushort, uchar);

float	Stamina(player_t *);
float	Stamina_power(player_t *);
float	Stamina_defense(player_t *);
float	Stamina_owerturnPower(player_t *);
float	Stamina_owerturnRcpt(player_t *);

void	World_Spam(player_t *);
void	World_PlayerMove(player_t *, ushort);
void	World_PlayerSocial(player_t *, int);
void	World_PlayerMount(player_t *, bool, uint);
void	World_PlayerSkill(player_t *, player_t *target, uint);
void	World_Chat(player_t *, ushort type, char *text);
void	World_UnSpam(player_t *);
void	World_Exit(player_t *);

void	S_CHAT_Back_to_Me_f(player_t *, ushort type, char *text);
void	S_CHAT_to_Me_f(player_t *, ushort, char *);

float	getDistance(float sX, float sY, float sZ, float tX, float tY, float tZ);
float	getDistance_2D(float *s, float *t);
float	getDistance_3D(float *s, float *t);

void	Stats_Update(player_t *);
void*	Regen_Player(void *);
void	Character_Save(player_t *);
void	USER_SETTING_Saved(player_t *, ushort);

short	OpCodes_DetectOpCode(ushort);
void	OpCodes_GetOpCode(player_t *, P_Buffer_t *);
const char*	OpCodes_GetOpCode_Name(player_t *, ushort);
const char*	OpCodes_FindName(player_t *);
void	OpCodes_Read(player_t *);
void*	OpCodes_Write(player_t *, ushort);

#endif // __PLAYER_H__
