#include "Player.h"

void Buffer_Reader(byte *buffer, byte *data, ushort *pos, ushort size)
{
  memcpy(data, buffer + *pos, size);
  *pos += size;
}

void Player_Reader(player_t *player, byte *data, ushort size)
{
  Buffer_Reader(player->Packet_R, data, &player->Packet_R_Pos, size);
}

uint64_t Player_ReaderNB(player_t *player, ushort size)
{
  uint64_t data = 0;
  Buffer_Reader(player->Packet_R, (byte*)&data, &player->Packet_R_Pos, size);
  return data;
}

float Player_ReaderF(player_t *player)
{
  float data = 0;
  Buffer_Reader(player->Packet_R, (byte*)&data, &player->Packet_R_Pos, 4);
  return data;
}

char* Player_ReaderS(player_t *player)
{
  WCHAR *Wdata = (WCHAR *)&player->Packet_R[player->Packet_R_Pos];
  char *data = to_CHAR(Wdata);
  player->Packet_R_Pos += (WCHARlen(Wdata)+1)*sizeof(WCHAR);
  return data;
}

void Player_ReaderSI(player_t *player, char *data)
{
  WCHAR *Wdata = (WCHAR *)&player->Packet_R[player->Packet_R_Pos];
  to_CHARi(Wdata, data);
  player->Packet_R_Pos += (WCHARlen(Wdata)+1)*sizeof(WCHAR);
}


void Player_NextHeaderR(player_t *player)
{
  ushort *size = (ushort *)&player->Packet_R[player->Packet_R_HeadPos];
  player->Packet_R_HeadPos += *size;
  player->Packet_R_Pos = player->Packet_R_HeadPos + 4;
}
