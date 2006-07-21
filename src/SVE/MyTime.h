#pragma once
#include "atltime.h"

class __declspec(dllexport) CStrongBoxMyTime
{
	CString		m_szFecha;
	CString		m_szFechaLarga;
	CString		m_szHora;
	bool		m_bIsSet;
public:
	CTime		m_pTiempo;
	static CString	GetFechaLargaFormat();
	static CString	GetFechaCortaFormat();
	static CString	GetHoraFormat();
	CString		GetFechaHora(bool larga= false);
	CString		GetFecha(bool larga=false);
	CString		GetHora();
	void		SetDateTime(CString tiempo);
	void		SetTiempo(CTime &tiempo);
	CString		GetFormatoMysql(bool hora=false);
	CStrongBoxMyTime(bool currenttime=false);
	CStrongBoxMyTime(CString tiempo);
	~CStrongBoxMyTime(void);
};
