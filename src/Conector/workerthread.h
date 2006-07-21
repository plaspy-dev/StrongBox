// WorkerThread.h: interface for the CWorkerThread class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WORKERTHREAD_H__33111216_FE43_4B22_8D3C_5599F6A40595__INCLUDED_)
#define AFX_WORKERTHREAD_H__33111216_FE43_4B22_8D3C_5599F6A40595__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxmt.h> 
#include <vector>

typedef UINT (*EVENTPROC) (LPVOID pProcParam);

typedef struct tagTHREADEVENT
{
	HANDLE hEvent;
	EVENTPROC pProc;
	LPVOID pProcParam;
} THREADEVENT, *LPTHREADEVENT;

typedef std::vector<THREADEVENT> eventarray;

class __declspec(dllexport) CWorkerThread
{
public:
	CWorkerThread();
	virtual ~CWorkerThread();

	void Start(const int& nPriority = THREAD_PRIORITY_NORMAL);
	bool Kill();
	void Reset();
	bool Suspend();
	bool Resume();
	bool SetPriority(const int& nPriority);
	void AddEvent(HANDLE hEvent, EVENTPROC pProc, LPVOID pProcParam = NULL);
	DWORD GetID() const;

private:
	static UINT ThreadProc(LPVOID pParam);

protected:
	DWORD m_dwID;
	HANDLE m_hThread;
	HANDLE m_hEventKill;
	HANDLE m_hEventIsKilled;
	eventarray m_arrEvents;

};

#endif // !defined(AFX_WORKERTHREAD_H__33111216_FE43_4B22_8D3C_5599F6A40595__INCLUDED_)
