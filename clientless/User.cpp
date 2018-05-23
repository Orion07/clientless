#include "stdafx.h"
#include "User.h"

CUser::CUser()
{
}
CUser::CUser(Account acc)
	: CSocket(acc)
{
	this->acc = acc;
	charStatus = CHAR_ACCLOGIN;
	jv = new CJvCryption();
	if(Initialize())
		printf("Socket created\n");
	
}
CUser::CUser(char* serverIp,int serverPort,int serverIndex) 
	: CSocket(serverIp,serverPort)
{

	this->serverIndex = serverIndex;
	charStatus = CHAR_ACCLOGIN;
}


CUser::~CUser()
{

}

void CUser::HandlePacket(Packet & pkt)
{
	uint8 opcode = pkt.GetOpcode();
	if(charStatus == CHAR_ACCLOGIN){
		switch(opcode){
			case LS_CRYPTION:
				LsCryptionHandler(pkt);
				BeforeLogin();
				break;
			case LS_LOGIN_REQ:
				LoginReqHandler(pkt);
				break;
			default:
				printf("bilmedigim paket qnq : %X\n",opcode);
				break;
		}
	}else{
		switch(opcode)
		{
			case WIZ_VERSION_CHECK:
				versionCheck(pkt);
				login();
				break;
			case WIZ_LOGIN:
				LoginProcess(pkt);
				break;
			case WIZ_SEL_CHAR:
				StoreCloseRequest();
				break;
			case WIZ_ALLCHAR_INFO_REQ:
				setAllCharInfo(pkt);
				SelectCharacter();
				break;
			case WIZ_LETTER:
				ShoppingMall(pkt);
				GameStart(1);
				SendServerIndex();
				GameStart(2);
				break;
			case WIZ_USERLIST:
				getCharAllInfo();
				break;
			case WIZ_XIGNCODE:
				XigncodeHandler(pkt);
				break;
		}
	}
}
void CUser::GameStart(uint8 index)
{
	Packet pkt(WIZ_GAMESTART);
	pkt.SByte();
	pkt << uint8(index) << acc.accountID;
	Send(&pkt);
}

void CUser::SendServerIndex()
{
	Packet pkt(WIZ_SERVERINDEX);
	//pkt << uint8(0);
	Send(&pkt);
}
void CUser::XigncodeHandler(Packet & pkt)
{
	Packet x3(WIZ_XIGNCODE,pkt.size());
	x3.resize(pkt.size());
	memcpy((void*)x3.contents(),(void*)pkt.contents(),pkt.size());
	Send(&x3);
}
