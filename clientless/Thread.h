#pragma once
class CThread
{

public:
	CThread();
	CThread(void* lpfunc,void* lpParam = nullptr);
	virtual ~CThread(void);
	void Start();
	void Suspend();
	void Resume();
	void Terminate();
	HANDLE hThread;
	DWORD dwThreadId;
	void* lpfunc;
	void* lpParam;
};

