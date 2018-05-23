#ifndef USER_H
#define USER_H
#pragma once
#include "globals.h"
#include "Socket.h"
#include "Main.h"

class CMain;
class CUser : public CSocket
{
	uint32 dwSpeedHackCheck;
	std::string strAccountID;
	std::string strAccountPW;
	uint16 serverIndex;
	typedef struct {
		uint32 itemID;
		uint16 sDurability,sCount,sRentalTime;
		uint8 bFlag;
	}Item;
	typedef struct {
		string strCharID;
		uint8 bRace, bLevel, bFace, bZone;
		uint16 sClass;
		uint32 nHair;
		Item Items[8];
	}Character;

	uint8 nation;
	Character Char[4];
	Item inv[INVENTORY_TOTAL];
	Account acc;
public:
	CUser();
	CUser::CUser(Account acc);
	CUser(char* serverIp,int serverPort,int);
	virtual ~CUser();
	virtual void HandlePacket(Packet & pkt);
private:
	void LsCryptionHandler(Packet & pkt);
	void BeforeLogin();
	void LoginReqHandler(Packet & pkt);
	void versionCheck(Packet & pkt);
	void login();
	void LoginProcess(Packet & pkt);
	void getCharAllInfo();
	void getUserList();
	void setAllCharInfo(Packet & pkt);
	void SelectCharacter();
	void StoreCloseRequest();
	void ShoppingMall(Packet & pkt);
	void HandleStoreClose(Packet & pkt);
	void GameStart(uint8 index);
	void SendServerIndex();
	void XigncodeHandler(Packet & pkt);

};
#endif

