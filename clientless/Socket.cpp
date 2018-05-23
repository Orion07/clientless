#include "Socket.h"
#include "stdafx.h"

CSocket::CSocket()
{
}
CSocket::CSocket(char* serverIp,int serverPort)
{
	this->serverIp = serverIp;
	this->serverPort = serverPort;
}
CSocket::CSocket(Account acc)
{
	this->acc = acc;
}


CSocket::~CSocket(void)
{
	DeleteCriticalSection(&CriticalSection);
	closesocket(socketHandle);
	WSACleanup();
}
void CSocket::Uninitialize()
{
	DeleteCriticalSection(&CriticalSection);
	closesocket(socketHandle);
	WSACleanup();
}
bool CSocket::Initialize()
{
	if (!InitializeCriticalSectionAndSpinCount(&CriticalSection, 0x00000400) )
		return false;
	if(!CreateSocket())
		return false;

	return true;
}
void CSocket::addHeader(uint8 *pkt,size_t len,uint8 *out)
{
	memset(out,0,256);
	memcpy(out,"\xAA\x55",2);
	*(uint16*)(out+2) = len;
	memcpy(out+4,pkt,len);
	memcpy(out+4+len,"\x55\xAA",2);
}
int CSocket::removeHeader(uint8 *pkt)
{
	uint16 firstHeader = *(uint16*)(pkt);
	uint16 size = 0;
	if(firstHeader == 0x55AA)
	{
		size = *(uint16*)(pkt+2);
		if(size > 0)
		{
			memcpy(pkt,pkt+4,size);
			return size;
		}
	}
	else
	{
		printf("First Header : %d\n",firstHeader);
	}
	return -1;
}
void CSocket::Send(Packet * pkt)
{
	/*if(pkt->size() <= 0){
		printf("Send Len Failure : %d\n",pkt->size());
		return;
	}*/
	EnterCriticalSection(&CriticalSection); 
	uint8 opcode = pkt->GetOpcode();
	int len = pkt->size() + 1;
	BYTE out[4096];
	uint8 buff[4096];
	uint8 packet[4096];
	printf("\nSend[%X]\n",opcode);
	if(opcode == WIZ_VERSION_CHECK || opcode == LS_CRYPTION)
	{
		out[0] = opcode;
		if(pkt->size()>0)
			memcpy(&out[1],pkt->contents(),pkt->size());
	}else{
		if(charStatus == CHAR_ACCLOGIN){
			packet[0] = opcode;
			if(pkt->size() > 0)
				memcpy(&packet[1],pkt->contents(),pkt->size());
			jv->Cryption(packet,len,true);
			len+=8;
			memcpy(out,packet,len);
		}else{
			len++;
			send_counter&=0xFF;
			packet[0] = ++send_counter;
			packet[1] = opcode;
			if(pkt->size()>0)
				memcpy(&packet[2],pkt->contents(),pkt->size());
			Cryption(packet,len,true,&out[0]);
			memcpy(out+1,out,len);
			out[0] = 1;
			len++;
		}
	}
	
	memset(buff,0,len);
	addHeader(out,len,buff);
	printf("Send[%d] : ",len);
	for (int i = 0; i < len+6; i++)
	{
		printf("%02X", buff[i]);
	}
	printf("\n");
	int iResult = send(socketHandle, (char*)buff, len+6, 0 );
		if (iResult == SOCKET_ERROR) {
			printf("send failed with error: %d\n", WSAGetLastError());
			closesocket(socketHandle);
			WSACleanup();
			return ;
		}
	LeaveCriticalSection(&CriticalSection);
}

void WINAPI CSocket::Recv(void* lpParam)
{
	CSocket *socket = static_cast<CSocket *>(lpParam);
	uint8 buffer[BUFFER_SIZE];
	BYTE out[BUFFER_SIZE];
	int tmpLen = 0;
	int iResult = 1;
	int i = 0;
	while(socket->checkSocket())// && iResult > 0
	{
		printf("i : %d,Status : %d,iResult : %d\n",++i,socket->charStatus,iResult);
		memset(buffer,0,BUFFER_SIZE);
		fd_set set;
		struct timeval timeout;
		FD_ZERO(&set);
		FD_SET(socket->socketHandle, &set); 
		timeout.tv_sec =0;
		timeout.tv_usec = 250;
		int rv = select(socket->socketHandle+1, &set, NULL, NULL, &timeout);
		if (rv == SOCKET_ERROR)
		{
			//printf("Select Error : %d\n",WSAGetLastError());
			break;
		}
		else if (rv == 0)
		{
			//printf("Select Time out : %d\n",WSAGetLastError());
			//break;
		}
		else
		{
			iResult = recv(socket->socketHandle, (char*)&buffer[0], sizeof(buffer), 0);
			if(iResult > 0 && iResult < BUFFER_SIZE){
				tmpLen = iResult;
				int parseLen = socket->removeHeader(buffer);
				if(parseLen > 0 && parseLen < BUFFER_SIZE){
					if(socket->charStatus == CHAR_ACCLOGIN)
					{
						if(buffer[0] == LS_CRYPTION && parseLen == 9)
						{
							parseLen--;
							Packet pkt(buffer[0],(size_t)parseLen);
							pkt.resize(parseLen);
							memcpy((void*)pkt.contents(),&buffer[1],parseLen);
							socket->HandlePacket(pkt);
						}
						else
						{
							socket->jv->Cryption(buffer,parseLen,false);
							parseLen-=5;
							memcpy(&buffer[0],&buffer[5],parseLen);
							Packet pkt(buffer[0],(size_t)--parseLen);
							pkt.resize(parseLen);
							memcpy((void*)pkt.contents(),&buffer[1],parseLen);
							socket->HandlePacket(pkt);
						}			
					}
					else
					{
						if(buffer[0] == WIZ_VERSION_CHECK && parseLen == 0x16){
							parseLen--;
							Packet pkt(buffer[0],(size_t)parseLen);
							pkt.resize(parseLen);
							memcpy((void*)pkt.contents(),&buffer[1],parseLen);
							socket->HandlePacket(pkt);
						}else{
							if(parseLen > 0x0F){
								memset(out,0,BUFFER_SIZE);
								socket->Cryption(buffer,parseLen,false,&out[0]);

								if(out[0]!= WIZ_NPC_MOVE)
									printf("\nRecv[%X]\n",out[0]);

								Packet pkt(out[0],(size_t)--parseLen);
								pkt.resize(parseLen);
								memcpy((void*)pkt.contents(),&out[1],parseLen);
								if(out[0] == WIZ_COMPRESS_PACKET)
								{
									printf("Compress Packet : %d\n",parseLen);
								}
								socket->HandlePacket(pkt);

								/*printf("\nRecv[%d] : ",parseLen+1);
								for (unsigned int i = 0; i < parseLen+1; i++)
								{
									printf("%02X", out[i]);
								}
								printf("\n");*/
							}else{
								printf("\nCryption Socket Len : %d\n",parseLen);
							}
						}
					}
				}
			}
			if(iResult == 0)
			{
				break;
			}
	}
		Sleep(200);
	}
	printf("Recv Kapatildi : %d\n",socket->charStatus);
}
/*
else{
				printf("Recv Result2 : %d\n",tmpLen);
				printf("\n");
						for (unsigned int i = 0; i < tmpLen+6; i++)
						{
							printf("%02X", out[i]);
						}
						printf("\n");
			}


			*/
Packet CSocket::RecvTest()
{
	uint8 buffer[BUFFER_SIZE];
	BYTE out[BUFFER_SIZE];
	int tmpLen = 0;
	int iResult = 1;
	while(checkSocket())// && iResult > 0
	{
		memset(buffer,0,BUFFER_SIZE);

		iResult = recv(socketHandle, (char*)&buffer[0], sizeof(buffer), 0);
		if(iResult > 0 && iResult < 20000){
			tmpLen = iResult;
			int parseLen = removeHeader(buffer);		
			if(parseLen > 0 && parseLen < 20000)
			{
				if(charStatus == CHAR_ACCLOGIN)
				{
						
				}
				else
				{
					if(buffer[0] == WIZ_VERSION_CHECK && parseLen == 0x16)
					{
						parseLen--;
						Packet pkt(buffer[0],(size_t)parseLen);
						pkt.resize(parseLen);
						memcpy((void*)pkt.contents(),&buffer[1],parseLen);
						HandlePacket(pkt);
					}
					else
					{
							memset(out,0,BUFFER_SIZE);
							Cryption(buffer,parseLen,false,&out[0]);
							Packet pkt(out[0],(size_t)--parseLen);
							pkt.resize(parseLen);
							memcpy((void*)pkt.contents(),&out[1],parseLen);
							if(out[0] == WIZ_COMPRESS_PACKET)
							{
								printf("Compress Packet : %d\n",parseLen);
							}
					}
				}
			}else{
				printf("Recv Result2 : %d\n",tmpLen);
			}
		}else{
			printf("Recv Result : %d\n",iResult);
		}
		Sleep(1);
	}
	printf("Recv Kapatildi\n");
	Packet za(WIZ_LOGIN);
	return za;
}
bool CSocket::connected()
{
     char buf;
     int err = recv(socketHandle, &buf, 1, MSG_PEEK);
     if(err == SOCKET_ERROR)
     {
          if(WSAGetLastError() != WSAEWOULDBLOCK)
          {return false;}
     }
     return true;
}
bool CSocket::CreateSocket()
{
	WSADATA wsaData;
    socketHandle = INVALID_SOCKET;
    struct addrinfo *result = NULL,
                    *ptr = NULL,
                    hints;
    int iResult;


    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return false;
    }

    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;


    // Resolve the server address and port
	char port[10];
	memset(port,0,10);
	if(charStatus == CHAR_ACCLOGIN){
		itoa(LOGINSERVER_PORT,port,10);
		serverIp = (char*)LOGINSERVER_IP;
	}
	else{
		itoa(GAMESERVER_PORT,port,10);
		serverIp = (char*)dlgMain->servermap[acc.serverIndex].serverip.c_str();
	}
	iResult = getaddrinfo(serverIp, port, &hints, &result);
    if ( iResult != 0 ) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
		return false;
    }

        // Create a SOCKET for connecting to server
        socketHandle = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
        if (socketHandle == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            return false;
        }

        // Connect to server.
        iResult = connect( socketHandle, result->ai_addr, (int)result->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(socketHandle);
            socketHandle = INVALID_SOCKET;
			printf("connect() error %d\n",iResult);
        }
	u_long iMode=1;
	ioctlsocket(socketHandle,FIONBIO,&iMode);

    freeaddrinfo(result);

    if (socketHandle == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return false;
    }

	printf("serverip = %s,serverport = %s,Connection successfully!\n",serverIp,port);
	

	t = new CThread(&(CSocket::Recv),(void*)this);
	t->Start();
	dlgMain->threadPool.push_back(t);
	if(charStatus == CHAR_ACCLOGIN){
		Packet pkt(LS_CRYPTION);
		Send(&pkt);
	}
	return true;
}

bool CSocket::checkSocket()
{
	int optVal;
	int optLen = sizeof(int);
	if(getsockopt(socketHandle, SOL_SOCKET, SO_ACCEPTCONN, (char*)&optVal, &optLen)!=SOCKET_ERROR)
		return true;

	return false;
}
