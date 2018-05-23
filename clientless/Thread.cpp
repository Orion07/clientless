#include "stdafx.h"
#include "Thread.h"



CThread::CThread()
{

}
CThread::CThread(void* lpfunc,void* lpParam /*= nullptr*/)
{
	this->lpfunc = lpfunc;
	this->lpParam = lpParam;
}
CThread::~CThread(void)
{
}

void CThread::Start()
{
	hThread = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)lpfunc,lpParam,0,&dwThreadId);
}
void CThread::Suspend()
{
	SuspendThread(hThread);
}
void CThread::Resume()
{
	ResumeThread(hThread);
}
void CThread::Terminate()
{
	TerminateThread(hThread,0);
	CloseHandle(hThread);
}
