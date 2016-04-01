#include "internal.hpp"
#include "TeraCrypt.h"

extern "C" {

void *TeraCrypt_Init(byte *CKey1, byte *CKey2, byte *SKey1, byte *SKey2)
{
	Crypt::Session *Sess = new Crypt::Session(CKey1, CKey2, SKey1, SKey2);
	return Sess;
}

void TeraCrypt_Enc(void *TCrypt, byte *data, int size)
{
	Crypt::Session *Sess = (Crypt::Session *)TCrypt;
	Sess->Encrypt(data, size);
}

void TeraCrypt_Dec(void *TCrypt, byte *data, int size)
{
	Crypt::Session *Sess = (Crypt::Session *)TCrypt;
	Sess->Decrypt(data, size);
}

void TeraCrypt_Free(void *TCrypt)
{
	Crypt::Session *Sess = (Crypt::Session *)TCrypt;
	delete Sess;
}

}
