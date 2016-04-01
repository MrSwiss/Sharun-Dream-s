#include <stdio.h>
#include <stdlib.h>

#ifndef _TERACRYPT_H_
#define _TERACRYPT_H_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
  void *TeraCrypt_Init(unsigned char *CKey1, unsigned char *CKey2, unsigned char *SKey1, unsigned char *SKey2);
  void TeraCrypt_Enc(void *TCrypt, unsigned char *data, int size);
  void TeraCrypt_Dec(void *TCrypt, unsigned char *data, int size);
  void TeraCrypt_Free(void *TCrypt);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _TERACRYPT_H_
