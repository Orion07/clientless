#include "stdafx.h"


void CUser::versionCheck(Packet & pkt)
{
	uint8 idk,keySize;
	uint16 version;
	uint32 key[4] = {0};
	pkt >> idk >> version >> keySize;
	for(int i =0;i<4;i++)
		pkt >> key[i];

	AprInitialize((DWORD*)key);
	printf("\nGame Version : %d\n",version);
}
void CUser::LoginProcess(Packet & pkt)
{
	uint8 nation;
	pkt >> nation;
	//if(nation == 0)
		//selectNation();
	if(nation > 1 && nation < 3)
		getUserList();
	else
		printf("nation : %d\n",nation);
}
void CUser::LsCryptionHandler(Packet & pkt)
{
	uint8 key[8];
	for(int i = 0;i<8;i++)
		pkt >> key[i];
	jv->JvInitialize(key);
}
void CUser::BeforeLogin()
{
	Packet pkt(LS_LOGIN_REQ);
	char encrytPass[50] = "";
	passEncrypt(acc.accountPW.c_str(),encrytPass);
	std::string pw(encrytPass);
	pkt.DByte();
	pkt << acc.accountID << pw << uint8(0) << uint8(0) << uint8(0);
	Send(&pkt);
}
void CUser::LoginReqHandler(Packet & pkt)
{
	uint8 resultcode;
	uint16 idk,pre,preHour;
	string accountid;
	pkt.DByte();
	pkt >> idk >> resultcode >> pre;
	if(pre != 0xffff)
		pkt >> preHour;
	pkt >> accountid;
	printf("LS Account ID : %s\n",accountid.c_str());
	printf("LS resultcode : %d\n",resultcode);
	Packet f7(LS_CLOSESOCKET);
	Send(&f7);
	if(resultcode == 1 || resultcode == 5 ){
		CThread *tmp = t;
		Uninitialize();
		charStatus = CHAR_SVRLOGIN;
		Initialize();
		Packet pkt2b(WIZ_VERSION_CHECK);
		pkt2b << uint16(-1);
		Send(&pkt2b);
		tmp->Terminate();
	}else if(resultcode == 3)
	{
		CThread *tmp = t;
		Uninitialize();
		charStatus = CHAR_ACCLOGIN;
		Initialize();
		tmp->Terminate();
	}
}
void CUser::login()
{
	Packet pkt(WIZ_LOGIN);
	char encrytPass[50] = "";
	passEncrypt(acc.accountPW.c_str(),encrytPass);
	std::string pw(encrytPass);
	pkt.DByte();
	pkt << acc.accountID << pw << uint8(0) << uint8(0) << uint8(0) << uint16(acc.serverIndex) << uint32(1);
	Send(&pkt);
}
void CUser::getCharAllInfo()
{
	Packet pkt(WIZ_ALLCHAR_INFO_REQ);
	pkt << uint8(1);
	Send(&pkt);
}
void CUser::setAllCharInfo(Packet & pkt)
{
	uint8 idk;
	string strCharID;
	pkt >> idk >> idk;
	for(int i = 0;i<4;i++)
	{
		pkt >> Char[i].strCharID >> Char[i].bRace >> Char[i].sClass >> Char[i].bLevel >> Char[i].bFace >> Char[i].nHair >> Char[i].bZone;
		/*printf("CharID : %s\n",Char[i].strCharID.c_str());
		printf("Race : %d,Class : %d,Level : %d,Face : %d,Hair : %X,Zone : %d\n",Char[i].bRace,Char[i].sClass,Char[i].bLevel,Char[i].bFace,Char[i].nHair,Char[i].bZone);
		printf("Items\n");*/
		for(int j = 0;j<8;j++){
			pkt >> Char[i].Items[j].itemID >> Char[i].Items[j].sDurability;
			//printf("ItemID : %d,Durability : %d\n",Char[i].Items[j].itemID,Char[i].Items[j].sDurability);
		}
	}
}
void CUser::getUserList()
{
	Packet pkt(WIZ_USERLIST);//WIZ_USERLIST
	pkt << uint8(1);
	Send(&pkt);
}
void CUser::SelectCharacter()
{
//04+0A00+30377862656B6F783037+0C00+4C6F72645072696573743037+015C
	Packet pkt(WIZ_SEL_CHAR);
	pkt.DByte();
	pkt << acc.accountID << Char[acc.characterIndex].strCharID << uint8(1) << uint8(Char[acc.characterIndex].bZone);
	Send(&pkt);
}
