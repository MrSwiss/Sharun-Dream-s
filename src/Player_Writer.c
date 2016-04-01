#include "Player.h"

void Buffer_Writer(byte *buffer, byte *data, ushort *pos, ushort size)
{
  memcpy(buffer + *pos, data, size);
  *pos += size;
}

void Player_Writer(P_Buffer_t *buffer, byte *data, ushort size)
{
  Buffer_Writer(buffer->Data, data, &buffer->Pos, size);
}

void Player_WriterNB(P_Buffer_t *buffer, uint64_t data, ushort size)
{
  Buffer_Writer(buffer->Data, (byte*)&data, &buffer->Pos, size);
}

void Player_WriterF(P_Buffer_t *buffer, float data)
{
  Buffer_Writer(buffer->Data, (byte*)&data, &buffer->Pos, 4);
}

void Player_WriterS(P_Buffer_t *buffer, char *data, int len)
{
  WCHAR * wData = (WCHAR*)&buffer->Data[buffer->Pos];
  int wData_Len = cpy_WCHAR(wData, data, len);
  buffer->Pos += (wData_Len+1)*sizeof(WCHAR);
}

void Player_WriterS2(P_Buffer_t *buffer, char *data)
{
  WCHAR * wData = (WCHAR*)&buffer->Data[buffer->Pos];
  int wData_Len = cpy_WCHAR2(wData, data);
  buffer->Pos += (wData_Len+1)*sizeof(WCHAR);
}

void Player_WriterSet(P_Buffer_t *buffer, uint64_t data, ushort size, ushort pos)
{
  memcpy(buffer->Data + pos, &data, size);
}

void Player_UpdateHeaderS(P_Buffer_t *buffer)
{
	ushort *size = (ushort *)&buffer->Data[0];
	*size = buffer->Pos;
}

void set_WBuffer(P_Buffer_t *buffer, ushort OpCode)
{
	buffer->BC_Type = 0;
	buffer->Pos = 4;
	buffer->Sender = NULL;
	buffer->Raid = NULL;
	buffer->Party = NULL;
	buffer->Field = NULL;
	buffer->Size = Packet_Max_Size;
	buffer->OpCode = OpCode;
}

P_Buffer_t *New_WBuffer(ushort OpCode)
{
	P_Buffer_t *buffer = malloc(sizeof(P_Buffer_t));
	set_WBuffer(buffer, OpCode);
	return buffer;
}

void Player_Send_Fast(player_t *player, ushort OpCode)
{
	P_Buffer_t buffer;
	set_WBuffer(&buffer, OpCode);
	Player_Send(player, &buffer);
}
