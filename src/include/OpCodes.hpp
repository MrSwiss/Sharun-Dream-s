#ifndef _OPCODES_HPP_
#define _OPCODES_HPP_

#include "player.hpp"
#include "packet.hpp"

#define Max_OpName_Size 60
#define Max_OpCode_List 10

typedef struct opcode_t {
	char	version[10];
	void (*Recv[PACKET_MAX_SIZE])(player*, packet*);
	struct send_t {
		char *Name;
		packet* (call)(player*, void*);
	} Send[Max_OpName_Size];
} opcode_t;

class opcodes {
	public:
		opcodes();
		~opcodes();
		ushort	size;
		char	opNames[0xffff+1][Max_OpName_Size];
		opcode_t	list[Max_OpCode_List];
};

#endif // _OPCODES_HPP_
