#pragma once
#ifndef INIREADER_H
#define INIREADER_H
class CIniReader
{
public:
	CIniReader(char* szFileName); 
	virtual ~CIniReader(void);
	int ReadInteger(char* szSection, char* szKey);
	float ReadFloat(char* szSection, char* szKey);
	bool ReadBoolean(char* szSection, char* szKey);
	char* ReadString(char* szSection, char* szKey);
private:
	char m_szFileName[256];
};
#endif//INIREADER_H