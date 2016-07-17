#ifndef _OPCODES_HPP_
#define _OPCODES_HPP_

#include "OpCode_Gen.hpp"

#define Max_OpName_Size 60
#define Max_OpCode_List 10

typedef struct opcode_unk_t {
	ushort	code;
	void	(*func)(void**);
	const char*	name;
	opcode_unk_t*	next;
} opcode_unk_t;

typedef struct opcode_t {
	char	version[10];
	int	detect;
	int	Send[OPC_MAX];
	void*	(*Recv[PACKET_MAX_SIZE])(player*, packet*);
	const char*	OpName[PACKET_MAX_SIZE];
	opcode_unk_t*	Unknow;
} opcode_t;

class opcodes {
	public:
		opcodes();
		virtual ~opcodes();
		bool	load();
		ushort	size;
		void*	(*Send[OPC_MAX])(void**);
		char	opNames[0xffff+1][Max_OpName_Size];
		opcode_t	list[Max_OpCode_List];
		void	Set_Recv(const char*, void*(*)(void**));
		void	Set_Send(int, void*(*)(void**));
		void*	Recv(player*, packet*);
};

#endif // _OPCODES_HPP_
