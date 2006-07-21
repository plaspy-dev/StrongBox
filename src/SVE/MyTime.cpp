#include "StdAfx.h"
#include "mytime.h"

CStrongBoxMyTime::CStrongBoxMyTime(bool currenttime/*=false*/)
{
	if(currenttime)
		m_pTiempo=CTime::GetCurrentTime();
	m_bIsSet= currenttime;
}

CStrongBoxMyTime::~CStrongBoxMyTime(void)
{

}

CStrongBoxMyTime::CStrongBoxMyTime(CString tiempo) {

	SetDateTime(tiempo);
}

CString	CStrongBoxMyTime::GetFechaLargaFormat() {
	
	char formato[255];
	GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SLONGDATE, formato, 255);
	return formato;
}

CString	CStrongBoxMyTime::GetFechaCortaFormat() {
	
	char formato[255];
	GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SSHORTDATE, formato, 255);
	return formato;
}

CString	CStrongBoxMyTime::GetHoraFormat() {

	char formato[255];
	GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_STIMEFORMAT, formato, 255);
	return formato;
}

CString	CStrongBoxMyTime::GetFecha(bool larga) {

	if(!m_bIsSet)
		return NULL;
	if( larga) {
		if( m_szFechaLarga.IsEmpty())
			m_szFechaLarga= GetFechaLargaFormat();
	} else if( m_szFecha.IsEmpty()) {
			m_szFecha= GetFechaCortaFormat();
	}
	SYSTEMTIME systime;
	m_pTiempo.GetAsSystemTime(systime);
	char salida[255];
	GetDateFormat(LOCALE_USER_DEFAULT, 0, &systime, larga ? m_szFechaLarga : m_szFecha, salida, 255);
	return salida;
}

CString	CStrongBoxMyTime::GetHora() {

	if(!m_bIsSet)
		return NULL;
	if( m_szHora.IsEmpty())
		m_szHora= GetHoraFormat();
	SYSTEMTIME systime;
	m_pTiempo.GetAsSystemTime(systime);
	char salida[255];
	GetTimeFormat(LOCALE_USER_DEFAULT, 0, &systime, m_szHora, salida, 255);
	return salida;
}

CString	CStrongBoxMyTime::GetFechaHora(bool larga) {

	if(!m_bIsSet)
		return NULL;
	CString temp= GetFecha(larga);
	if( larga)
		temp+=",";
	temp+= " ";
	return  temp + GetHora();
}

void CStrongBoxMyTime::SetDateTime(CString tiempo) {

	if( tiempo.IsEmpty() || tiempo.GetLength()<9) {
		m_bIsSet= false;
		return;
	}

	CString ano,mes,dia,hora,min,sec;

	ano+= tiempo.GetAt(0);
	ano+= tiempo.GetAt(1);
	ano+= tiempo.GetAt(2);
	ano+= tiempo.GetAt(3);
	mes+= tiempo.GetAt(5);
	mes+= tiempo.GetAt(6);
	dia+= tiempo.GetAt(8);
	dia+= tiempo.GetAt(9);
	if( tiempo.GetLength()>10) {

		hora= tiempo.GetAt(11);
		hora+= tiempo.GetAt(12);
		// mes son los minutos
		min= tiempo.GetAt(14);
		min+= tiempo.GetAt(15);
		sec= tiempo.GetAt(17);
		sec+= tiempo.GetAt(18);
	}
	m_bIsSet= true;
	m_pTiempo =CTime(atol(ano), atol(mes), atol(dia), atol(hora), atol(min), atol(sec));
}

void CStrongBoxMyTime::SetTiempo(CTime &tiempo) {

	m_bIsSet= true;
	m_pTiempo= tiempo;
}

CString	CStrongBoxMyTime::GetFormatoMysql(bool hora) {

	if( !m_bIsSet)
		return NULL;
	return m_pTiempo.Format(hora ? "%Y-%m-%d %H:%M:00" : "%Y-%m-%d");
}
