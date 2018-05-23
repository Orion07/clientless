#pragma once
#ifndef MAIN_H
#define MAIN_H

#include <vector>
#include "Thread.h"
#include "IniReader.h"
#include "globals.h"

using namespace std;


class CMain
{
public:
	typedef std::vector<CThread *> Threads;
	int AccountCount;
	Account acc[MAX_USER];
	Threads threadPool;
	std::map<int,server> servermap;
	CMain(void);
	~CMain(void);
	void Initialize();
	void LoadAccountSettings();
	void LoadGameServersDetail();
};
extern CMain *dlgMain;
#endif
