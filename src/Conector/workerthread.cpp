// WorkerThread.cpp: implementation of the CWorkerThread class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WorkerThread.h"
#include <assert.h>
# include "../shared/definiciones.h"

#define WORKERTHREADEVENT_KILL		0
#define WORKERTHREADEVENT_TIMEOUT	500

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////



/***************************** CWorkerThread::CWorkerThread ******************************
description:
   Constructor

access: public
------------------------------------------------------------------------------------------
created: 30.08.01 - 11:47:20
creator: Frank
*****************************************************************************************/
CWorkerThread::CWorkerThread()
{
	FUNCTION_STACK_TRACE("FFFFC03B");
	m_dwID = 0;
	m_hThread = NULL;

	// Create kill event...
	m_hEventKill = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	assert(m_hEventKill);

	// Create is killed event...
	m_hEventIsKilled = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	assert(m_hEventIsKilled);
	
	// Add kill event...
	THREADEVENT event = {m_hEventKill, NULL, NULL};
	m_arrEvents.push_back(event);
}



/**************************** CWorkerThread::~CWorkerThread ******************************
description:
   Destructor

access: public, state: virtual
------------------------------------------------------------------------------------------
created: 30.08.01 - 11:47:36
creator: Frank
*****************************************************************************************/
CWorkerThread::~CWorkerThread()
{
	FUNCTION_STACK_TRACE("FFFFC03C");
	::CloseHandle(m_hEventKill);
	::CloseHandle(m_hEventIsKilled);
}



/********************************* CWorkerThread::Start **********************************
description:
   Starts the thread

@nPriority[in]:
   Priority for the thread

access: public
------------------------------------------------------------------------------------------
created: 30.08.01 - 11:48:31
creator: Frank
*****************************************************************************************/
void CWorkerThread::Start(const int& nPriority /* = THREAD_PRIORITY_NORMAL */)
{
	FUNCTION_STACK_TRACE("FFFFC03D");
	assert(!m_hThread);
	
	DWORD dwThreadID = 0;
	m_hThread = ::CreateThread(NULL,
							   NULL,
							   (LPTHREAD_START_ROUTINE) ThreadProc,
							   this,
							   CREATE_SUSPENDED,
							   &m_dwID);
	if( !m_hThread) {
		dwThreadID= GetLastError();
	}
	assert(m_hThread);

	::SetThreadPriority(m_hThread, nPriority);
	::ResumeThread(m_hThread);
}



/********************************* CWorkerThread::Reset **********************************
description:
   Resets the thread

access: public
------------------------------------------------------------------------------------------
created: 30.08.01 - 11:49:19
creator: Frank
*****************************************************************************************/
void CWorkerThread::Reset()
{
	FUNCTION_STACK_TRACE("FFFFC03E");
	Kill();

	m_dwID = 0;
	m_hThread = NULL;
	m_arrEvents.clear();

	// Add kill event...
	THREADEVENT event = {m_hEventKill, NULL, NULL};
	m_arrEvents.push_back(event);
}



/****************************** CWorkerThread::ThreadProc ********************************
description:
   The "main" thread procedure

@pProcParam[in]:
   Pointer to the thread object

value:
   Thread exit code

access: private
------------------------------------------------------------------------------------------
created: 30.08.01 - 11:50:59
creator: Frank
*****************************************************************************************/
UINT CWorkerThread::ThreadProc(LPVOID pProcParam)
{
	FUNCTION_STACK_TRACE("FFFFC03F");
	assert(pProcParam);

	CWorkerThread* pThread = reinterpret_cast<CWorkerThread*>(pProcParam);
	DWORD dwResult = 0;

	int nSize = pThread->m_arrEvents.size();
	HANDLE* pArrEvents = new HANDLE[nSize];
	for (int i = 0; i < nSize; i++)
		pArrEvents[i] = pThread->m_arrEvents[i].hEvent;

	// Install the event queue...
	while (true)
	{
		dwResult = ::WaitForMultipleObjects(nSize,
											pArrEvents,
											FALSE,
											INFINITE) - WAIT_OBJECT_0;

		if (dwResult == WORKERTHREADEVENT_KILL)
			break;
		// Execute appropriate function...
		(pThread->m_arrEvents[dwResult].pProc)(pThread->m_arrEvents[dwResult].pProcParam);
	}

	delete[] pArrEvents;
	
	::SetEvent(pThread->m_hEventIsKilled);

	return 0;
}



/******************************* CWorkerThread::AddEvent *********************************
description:
   Adds an event, the funtion to execute in case of the event and the optional parameter
   to pass to the function to the thread object

@hEvent[in]:
   Handle of the event
@pProc[in]:
   Pointer to an EVENTPROC
@pProcParam[in]:
   Pointer to an optional EVENTPROC parameter

access: public
------------------------------------------------------------------------------------------
created: 30.08.01 - 11:56:40
creator: Frank
*****************************************************************************************/
void CWorkerThread::AddEvent(HANDLE hEvent, EVENTPROC pProc, LPVOID pProcParam /* = NULL */)
{
	FUNCTION_STACK_TRACE("FFFFC040");
	assert(!m_hThread || hEvent || pProc);

	THREADEVENT event;
	event.hEvent = hEvent;
	event.pProc = pProc;
	event.pProcParam = pProcParam;
	m_arrEvents.push_back(event);
}



/********************************* CWorkerThread::Kill ***********************************
description:
   Kills the thread

value:
   true || false

access: public
------------------------------------------------------------------------------------------
created: 30.08.01 - 11:57:12
creator: Frank
*****************************************************************************************/
bool CWorkerThread::Kill()
{
	FUNCTION_STACK_TRACE("FFFFC041");
	if (!m_hThread)
		return true;
	
	// Give the thread enough power to complete...
	::SetThreadPriority(m_hThread, THREAD_PRIORITY_TIME_CRITICAL);
	
	// ...and signal kill event...
	::SetEvent(m_hEventKill);
	
	if (::WaitForSingleObject(m_hEventIsKilled, WORKERTHREADEVENT_TIMEOUT) == WAIT_TIMEOUT)
	{
		DWORD dwExitCode = 0;		
		if (!::GetExitCodeThread(m_hThread, &dwExitCode))
			return false;
		if (!::TerminateThread(m_hThread, dwExitCode))
			return false;
	}

	::CloseHandle(m_hThread);
	
	return true;
}



/******************************** CWorkerThread::Suspend *********************************
description:
   Suspends the thread

value:
   true || false

access: public
------------------------------------------------------------------------------------------
created: 30.08.01 - 11:57:35
creator: Frank
*****************************************************************************************/
bool CWorkerThread::Suspend()
{
	FUNCTION_STACK_TRACE("FFFFC042");
	return ::SuspendThread(m_hThread) != -1;
}



/******************************** CWorkerThread::Resume **********************************
description:
   Resumes a suspended thread

value:
   true || false

access: public
------------------------------------------------------------------------------------------
created: 30.08.01 - 11:58:01
creator: Frank
*****************************************************************************************/
bool CWorkerThread::Resume()
{
	FUNCTION_STACK_TRACE("FFFFC043");
	return ::ResumeThread(m_hThread) != -1;
}



/****************************** CWorkerThread::SetPriority *******************************
description:
   Sets the priority of the thread

@nPriority[in]:
   The new priority

value:
   true || false

access: public
------------------------------------------------------------------------------------------
created: 30.08.01 - 11:58:33
creator: Frank
*****************************************************************************************/
bool CWorkerThread::SetPriority(const int& nPriority)
{
	FUNCTION_STACK_TRACE("FFFFC044");
	return ::SetThreadPriority(m_hThread, nPriority) != 0;
}



/********************************* CWorkerThread::GetID **********************************
description:
   Access to the thread ID

value:
   The thread ID

access: public
------------------------------------------------------------------------------------------
created: 30.08.01 - 12:23:23
creator: Frank
*****************************************************************************************/
DWORD CWorkerThread::GetID() const
{
	FUNCTION_STACK_TRACE("FFFFC045");
	return m_dwID;
}

