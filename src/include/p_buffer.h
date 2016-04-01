#include "player_struct.h"

typedef struct P_Buffer_t {
	ushort	BC_Type; // Broadcast_Type
	ushort	OpCode;
	ushort	Pos;
	ushort	Size;
	byte	Data[Packet_Max_Size];
	player_t*	Sender; // Player
	void*	Raid;
	void*	Party;
	void*	Field; // Special field like PVP...
	void*	thread;
} P_Buffer_t;
