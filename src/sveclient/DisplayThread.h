#pragma once
# include "../Conector/workerthread.h"
# define ANCHO_LCD				20
# define REAL_LCD				16

class CDisplayThread
{
private:
	CEvent			m_Event;
	CWorkerThread	m_Thread;

	static UINT		EventProc(LPVOID pProcParam);
	bool			Init();
	void			Kill();
	bool			m_bRunning;
	CString			m_szLinea1;
	CString			m_szLinea2;
	CString			m_szNuevo;
public:
	CDisplayThread(void);
public:
	~CDisplayThread(void);
	bool			IsRunning();
	void			Run();
	void			SetMessage(CString linea1, CString linea2);
};
