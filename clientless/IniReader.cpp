#include "stdafx.h"
#include "IniReader.h"


CIniReader::CIniReader(char* szFileName)
{
 memset(m_szFileName, 0x00, 255);
 memcpy(m_szFileName, szFileName, strlen(szFileName));
}
int CIniReader::ReadInteger(char* szSection, char* szKey)
{
 int iResult = GetPrivateProfileIntA(szSection,  szKey, NULL, m_szFileName); 
 return iResult;
}
float CIniReader::ReadFloat(char* szSection, char* szKey)
{
 char szResult[255];
 float fltResult;
 GetPrivateProfileStringA(szSection,  szKey, NULL, szResult, 255, m_szFileName); 
 fltResult =  atof(szResult);
 return fltResult;
}
bool CIniReader::ReadBoolean(char* szSection, char* szKey)
{
 char szResult[255];
 bool bolResult;
 GetPrivateProfileStringA(szSection, szKey, NULL, szResult, 255, m_szFileName); 
 bolResult =  (strcmp(szResult, "True") == 0 || 
		strcmp(szResult, "true") == 0) ? true : false;
 return bolResult;
}
char* CIniReader::ReadString(char* szSection, char* szKey)
{
 char* szResult = new char[255];
 memset(szResult, 0x00, 255);
 GetPrivateProfileStringA(szSection,  szKey, NULL, szResult, 255, m_szFileName); 
 return szResult;
}


CIniReader::~CIniReader(void)
{
}
