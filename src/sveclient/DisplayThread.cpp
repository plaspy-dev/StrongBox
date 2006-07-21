#include "StdAfx.h"
#include "DisplayThread.h"
# include "display.h"


CDisplayThread::CDisplayThread(void)
{
	m_bRunning= false;
}

CDisplayThread::~CDisplayThread(void)
{
	Kill();
	m_Event.ResetEvent();
	m_Thread.Kill();
}

UINT CDisplayThread::EventProc(LPVOID pProcParam) {

	CDisplayThread* pClass= reinterpret_cast<CDisplayThread*>(pProcParam);
	pClass->Init();
	return 0;
}

void CDisplayThread::Run() {

	if(m_bRunning)
		return;
	m_bRunning= true;
	m_Thread.AddEvent(m_Event, EventProc, this);
	m_Thread.Start(THREAD_PRIORITY_LOWEST);
	m_Event.SetEvent();
}

void CDisplayThread::Kill() {

	if(!m_bRunning) 
		return;
	m_bRunning= false;
}

bool CDisplayThread::IsRunning() {

	return true;
}

void CDisplayThread::SetMessage(CString linea1, CString linea2) {

	if(linea1.GetLength()>REAL_LCD) {
		linea1= linea1.Left(REAL_LCD);
	}
	if(linea2.GetLength()>REAL_LCD ) {
		linea2= linea2.Left(REAL_LCD);
	}
	if( linea1==m_szLinea1 && linea2==m_szLinea2) {
		return;
	}
	m_szLinea1= linea1;
	m_szLinea2= linea2;

	if(linea1.GetLength()<ANCHO_LCD) {
		for( int i= linea1.GetLength(); i<ANCHO_LCD; i++)
			linea1+=" ";
	}
	if(linea2.GetLength()<ANCHO_LCD) {
		for( int i= linea2.GetLength(); i<ANCHO_LCD; i++)
			linea2+=" ";
	}
	m_szNuevo= linea1+linea2;
}

bool CDisplayThread::Init() {

	char mensaje[255];

	LCD_Init();
	LCD_Clear();

	while(m_bRunning) {
		if(!m_szNuevo.IsEmpty()) {
			strcpy(mensaje, m_szNuevo);
			m_szNuevo.Empty();
			LCD_DisplayScreen(mensaje);
		}
	}
	return true;
}
