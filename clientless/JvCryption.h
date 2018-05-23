#ifndef JV_H
#define JV_H
#pragma once
#include <Windows.h>

class CJvCryption
{
	BYTE xor_key[8];
	uint32 packet_counter;
public:
	CJvCryption(void);
	virtual ~CJvCryption(void);
	void JvInitialize(uint8 key[]);
	void Cryption(BYTE* packet,int len,bool flag);
	void JvDecryption(BYTE* packet,int len,BYTE* out);
	void JvEncryption(BYTE* packet,int len,BYTE* out);
	DWORD getCRC(BYTE* packet,int len);

};
#endif

