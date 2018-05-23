#ifndef SOCKET_H
#define SOCKET_H
#pragma once
#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <Windows.h>
#include "Apr.h"
#include "Thread.h"
#include "Packet.h"
#include "JvCryption.h"

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
class CSocket : public CApr
{

public:
	CThread *t;
	CJvCryption *jv;
	int charStatus;
	CSocket();
	CSocket(Account acc);
	CSocket(char* serverIp,int serverPort);
	virtual ~CSocket(void);
	bool Initialize();
	void Uninitialize();
	bool CreateSocket();
	void Send(Packet * pkt);
	static void WINAPI Recv(void* lpParam);
	virtual void HandlePacket(Packet &pkt) = 0;
	bool CSocket::connected();
	Packet CSocket::RecvTest();
	bool checkSocket();
	void addHeader(uint8 *pkt,size_t size,uint8 *out);
	int removeHeader(uint8 *pkt);
	CRITICAL_SECTION CriticalSection;
	SOCKET socketHandle;
	char* serverIp;
	char* proxyIp;
	char* proxyId;
	char* proxyPw;
	int serverPort;
	int proxyPort;
	int proxyType;
	Account acc;

};
#endif
