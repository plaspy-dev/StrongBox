#include "StdAfx.h"
#include "TrialInfo.h"
# include "../iofile/myregkey.h"
#include "../conector/Crc32Static.h"

# define MAX_DIAS	20

CStrongBoxTrialInfo::CStrongBoxTrialInfo(void)
{
	m_nLeftDays=0;
}

CStrongBoxTrialInfo::~CStrongBoxTrialInfo(void)
{

}

bool CStrongBoxTrialInfo::Register() {

	CMyRegKey key;
	CString archivo;

	double dtiempo= COleDateTime::GetCurrentTime();

	COleDateTime fredy;
	fredy.SetDateTime(1980,1,1,5,0,0);

	COleDateTime tiempo;
	tiempo= dtiempo;

	COleDateTimeSpan span;
	span= tiempo-fredy;
	double total= span.GetTotalMinutes();
	DWORD crc;
	
	key.MyOpen(HKEY_CLASSES_ROOT, "CLSID");
	// guardamos la fecha de la instalacion
#ifndef _DEBUG
	if(key.GetString("{76F32C44-ADA6-4537-BD90-115F0A510643}\\InProcServer32", "ThreadingModel", "-1")!="-1")
		return false;
	if(key.GetString("{0F8DB967-A473-4ef4-9BA8-D4EE0FD6D1D2}\\InProcServer32", "ThreadingModel", "-1")!="-1")
		return false;
#endif

	archivo.Format("%lf", dtiempo);
	key.WriteString("{76F32C44-ADA6-4537-BD90-115F0A510643}\\InProcServer32", "ThreadingModel", archivo);
	archivo.Format("%lf", span.GetTotalHours());
	CCrc32Static::StringCrc32(archivo, crc);
	// guardamos el crc de las horas a mi nacimiento
	key.WriteInt("{76F32C44-ADA6-4537-BD90-115F0A510643}\\InProcServer32", "ProgID", crc);


	archivo.Format("%lf", total);
	CCrc32Static::StringCrc32(archivo, crc);
	// guardamos los minutos a mi nacimiento
	key.WriteString("{0F8DB967-A473-4ef4-9BA8-D4EE0FD6D1D2}\\InProcServer32", "ThreadingModel", archivo);
	// guardamos el crc de eso.
	key.WriteInt("{0F8DB967-A473-4ef4-9BA8-D4EE0FD6D1D2}\\InProcServer32", "ProgID", crc);


	char path[MAX_PATH];
	path[0]=0;
	SHGetSpecialFolderPath(NULL, path, CSIDL_COMMON_APPDATA, true);
	CString ruta= path;
	ruta+="\\SDG";
	CreateDirectory(ruta, NULL);
	ruta+="\\SDGFinger";
	CreateDirectory(ruta, NULL);
	
	// ultima apertura del programa
	archivo=ruta+ "\\key.dat";
	FILE *f;
#ifndef _DEBUG
	if( f=fopen(archivo, "r")) {
		fclose(f);
		return false;
	}
#endif
	if(!(f=fopen(archivo, "w"))) {
		return false;
	}
	fprintf(f, "%lf", dtiempo);
	fclose(f);
	key.MyOpen(HKEY_CURRENT_USER, "Software\\microsoft\\Internet Explorer\\Main");
	archivo.Format("%lf", dtiempo);
	key.WriteString("", "Last Page", archivo);

	// crc de los dias faltantes
	archivo= ruta+"\\sec.dat";
#ifndef _DEBUG
	if( f=fopen(archivo, "r")) {
		fclose(f);
		return false;
	}
#endif
	if(!(f=fopen(archivo, "w"))) {
		return false;
	}
	fprintf(f, "%lu", crc);
	fclose(f);
	return true;
}

int  CStrongBoxTrialInfo::GetLeftDays() {

#ifdef _DEBUG
	return 100;
#endif
	CMyRegKey key;
	CString archivo;
	m_nLeftDays=0;

	COleDateTime fredy;
	fredy.SetDateTime(1980,1,1,5,0,0);

	key.MyOpen(HKEY_CLASSES_ROOT, "CLSID");

	double dtiempo=atof(key.GetString("{76F32C44-ADA6-4537-BD90-115F0A510643}\\InProcServer32", "ThreadingModel"));
	if( dtiempo==0)
		return 0;

	COleDateTime tiempo;
	tiempo= dtiempo;

	COleDateTimeSpan span;
	span= tiempo-fredy;
	double total= span.GetTotalMinutes();
	DWORD crc;

	archivo.Format("%lf", span.GetTotalHours());
	CCrc32Static::StringCrc32(archivo, crc);
	
	if(crc!=key.GetInt("{76F32C44-ADA6-4537-BD90-115F0A510643}\\InProcServer32", "ProgID"))
		return 0;

	archivo.Format("%lf", total);
	CCrc32Static::StringCrc32(archivo, crc);
	
	if(archivo!=key.GetString("{0F8DB967-A473-4ef4-9BA8-D4EE0FD6D1D2}\\InProcServer32", "ThreadingModel"))
		return 0;
	// guardamos el crc de eso.
	if(crc!=key.GetInt("{0F8DB967-A473-4ef4-9BA8-D4EE0FD6D1D2}\\InProcServer32", "ProgID"))
		return 0;	

	COleDateTime actual;
	double dactual=COleDateTime::GetCurrentTime();
	actual= dactual;
	
	COleDateTimeSpan left;
	left= actual-tiempo;
	
	m_nLeftDays= MAX_DIAS-left.GetDays();

	char path[MAX_PATH];
	path[0]=0;
	SHGetSpecialFolderPath(NULL, path, CSIDL_COMMON_APPDATA, false);
	CString ruta= path;

	ruta+="\\SDG";
	ruta+="\\SDGFinger";
	
	double last;
	DWORD lastcrc,lastreal;
	COleDateTime lasttiempo;

	archivo=ruta+ "\\key.dat";
	FILE *f;
	if(!(f=fopen(archivo, "r"))) {
		return 0;
	}
	fscanf(f, "%lf", &last);
	fclose(f);

	key.MyOpen(HKEY_CURRENT_USER, "Software\\MicroSoft\\Internet Explorer\\Main");

	if( last!= atof(key.GetString("", "Last Page")))
		return 0;

	lasttiempo= last;
	if(actual<lasttiempo)
		return 0;

	left= lasttiempo-fredy;
	archivo.Format("%lf", left.GetTotalMinutes());
	CCrc32Static::StringCrc32(archivo, lastreal);

	archivo= ruta+"\\sec.dat";
	if(!(f=fopen(archivo, "r"))) {
		return 0;
	}
	fscanf(f, "%lu", &lastcrc);
	fclose(f);
	if( lastcrc!=lastreal)
		return 0;

	archivo=ruta+ "\\key.dat";
	if(!(f=fopen(archivo, "w"))) {
		return 0;
	}
	fprintf(f, "%lf", dactual);
	fclose(f);

	left= actual-fredy;
	archivo.Format("%lf", left.GetTotalMinutes());
	CCrc32Static::StringCrc32(archivo, crc);

	// crc de los dias faltantes
	archivo= ruta+"\\sec.dat";
	if(!(f=fopen(archivo, "w"))) {
		return 0;
	}
	fprintf(f, "%lu", crc);
	fclose(f);

	archivo.Format("%lf", dactual);
	key.WriteString("", "Last Page", archivo);

	return m_nLeftDays;
}
