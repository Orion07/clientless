#include "stdafx.h"
#include "Main.h"
typedef std::vector<CUser *> Users;
Users userPool;

CMain::CMain(void)
{
}


CMain::~CMain(void)
{
}

void CMain::Initialize()
{
	LoadAccountSettings();
	LoadGameServersDetail();
	for(int i = 0;i<AccountCount;i++)
		userPool.push_back(new CUser(acc[i]));
	
}
void CMain::LoadAccountSettings()
{
	CIniReader ini(".\\settings.ini");
	AccountCount = ini.ReadInteger("GENERAL","totalAccount");
	printf("Account count : %d\n",AccountCount);
	for(int i = 0;i<AccountCount;i++)
	{
		char section[30];
		sprintf(section,"ACCOUNT%d",i+1);
		acc[i].accountID = std::string(ini.ReadString(section,"accountID"));
		printf("AccountID%d : %s\n",i+1,acc[i].accountID.c_str());
		acc[i].accountPW = std::string(ini.ReadString(section,"accountPW"));
		printf("AccountPW%d : %s\n",i+1,acc[i].accountPW.c_str());
		acc[i].serverIndex = ini.ReadInteger(section,"serverIndex");
		acc[i].characterIndex = ini.ReadInteger(section,"characterIndex");
		acc[i].useProxy = ini.ReadInteger(section,"useProxy");
		acc[i].proxyType = ini.ReadInteger(section,"proxyType");
		acc[i].proxyIP = std::string(ini.ReadString(section,"proxyIP"));
		acc[i].proxyPort = ini.ReadInteger(section,"proxyPort");
		acc[i].proxyID = std::string(ini.ReadString(section,"proxyID"));
		acc[i].proxyPW = std::string(ini.ReadString(section,"proxyPW"));
		acc[i].useOTP = ini.ReadInteger(section,"useOTP");
		acc[i].otpPW = std::string(ini.ReadString(section,"otpPW"));

	}
}
void CMain::LoadGameServersDetail()
{
	
	servermap[0].serverip = "188.132.161.194";//ARES|ARES1
	servermap[0].serverIndex = 1;
	servermap[0].channelIndex = 0;

	servermap[1].serverip = "188.132.161.195";//DIEZ|DIEZ1
	servermap[1].serverIndex = 2;
	servermap[2].channelIndex = 0;

	servermap[2].serverip = "188.132.161.196";//GORDION|GORDION1
	servermap[2].serverIndex = 3;
	servermap[2].channelIndex = 0;

	servermap[3].serverip = "188.132.161.197";//EDANA|EDANA1
	servermap[3].serverIndex = 4;
	servermap[3].channelIndex = 0;

	servermap[4].serverip = "188.132.161.201";//MIDGARD
	servermap[4].serverIndex = 5;
	servermap[4].channelIndex = 0;

	servermap[5].serverip = "188.132.161.205";//OLYMPIA(NEW)|OLYMP1
	servermap[5].serverIndex = 6;
	servermap[5].channelIndex = 0;

	servermap[6].serverip = "188.132.161.206";//OLYMPIA(NEW)|OLYMP2
	servermap[6].serverIndex = 6;
	servermap[6].channelIndex = 1;

	servermap[7].serverip = "188.132.161.207";//OLYMPIA(NEW)|OLYMP3
	servermap[7].serverIndex = 6;
	servermap[7].channelIndex = 2;

	servermap[8].serverip = "188.132.161.208";//OLYMPIA(NEW)|OLYMP4
	servermap[8].serverIndex = 6;
	servermap[8].channelIndex = 3;

	servermap[9].serverip = "188.132.161.200";//EUROPA|EUROPA1
	servermap[9].serverIndex = 7;
	servermap[9].channelIndex = 0;

	servermap[10].serverip = "188.132.161.226";//DESTAN|DESTAN1
	servermap[10].serverIndex = 0x14;
	servermap[10].channelIndex = 0;

	servermap[11].serverip = "188.132.161.227";//DESTAN|DESTAN2
	servermap[11].serverIndex = 0x14;
	servermap[11].channelIndex = 1;
}