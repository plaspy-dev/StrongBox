#include "StdAfx.h"
#include "Applicacion.h"
# include <atlbase.h>

typedef CString (*FN_STRING)();
typedef int (*FN_INT)();
typedef HBITMAP (*FN_BITMAP)();

static CString ExecutablePath() {
	TCHAR path[MAX_PATH];
	const DWORD length = GetModuleFileName(NULL, path, _countof(path));
	return length > 0 && length < _countof(path) ? CString(path, length) : CString();
}

static CString InstallerMetadataPath() {
	const CString path = ExecutablePath();
	const int separator = path.ReverseFind('\\');
	return separator >= 0 ? path.Left(separator + 1) + _T("appt.dll") : CString();
}

bool CApplicacion::HasInstallerMetadata() {
	const DWORD attributes = GetFileAttributes(InstallerMetadataPath());
	return attributes != INVALID_FILE_ATTRIBUTES && !(attributes & FILE_ATTRIBUTE_DIRECTORY);
}


HINSTANCE CApplicacion::m_pInstancia=NULL;
HBITMAP CApplicacion::m_pBitmap=NULL;
CString CApplicacion::m_szExeName;
CString CApplicacion::m_szAppName;
CString CApplicacion::m_szLongName;
CString CApplicacion::m_szCompany;
CString CApplicacion::m_szAppKey;
int		CApplicacion::m_nID=0;
CString CApplicacion::m_szAppPath;
CString CApplicacion::m_szURL;

CApplicacion::CApplicacion(void)
{
	m_pInstancia = LoadLibraryEx(InstallerMetadataPath(), NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
}

CApplicacion::~CApplicacion(void)
{
	if(m_pInstancia) {
		FreeLibrary(m_pInstancia);
		m_pInstancia=NULL;
	}
}

HBITMAP CApplicacion::GetBitmap() {
	
	// if( m_pBitmap==NULL) {
		
	CApplicacion app;
	FN_BITMAP lpBitmap;
	lpBitmap=(FN_BITMAP)GetProcAddress(m_pInstancia, "GetLogo");
	if(lpBitmap)
		m_pBitmap= lpBitmap();
	// }
	ASSERT(m_pBitmap);
	return m_pBitmap;
}

void CApplicacion::SetExeName(CString nombre) {

	m_szExeName= nombre;
}

CString CApplicacion::GetExeName() {
	if (!HasInstallerMetadata()) {
		const CString path = ExecutablePath();
		return path.Mid(path.ReverseFind('\\') + 1);
	}
	
	if(m_szExeName.IsEmpty()) {
		
		CApplicacion app;
		FN_STRING lpString;
		lpString=(FN_STRING)GetProcAddress(m_pInstancia, "GetExeName");
		if(lpString) 
			m_szExeName=lpString();
	}
	ASSERT(!m_szExeName.IsEmpty());
	return m_szExeName;
}

CString CApplicacion::GetAppName() {
	if (!HasInstallerMetadata()) {
		const CString executable = GetExeName();
		const int extension = executable.ReverseFind('.');
		return extension >= 0 ? executable.Left(extension) : executable;
	}
	if(m_szAppName.IsEmpty()) {
		
		CApplicacion app;
		FN_STRING lpString;
		lpString=(FN_STRING)GetProcAddress(m_pInstancia, "GetAppName");
		if(lpString) 
			m_szAppName=lpString();
	}
	ASSERT(!m_szAppName.IsEmpty());
	return m_szAppName;
}

CString CApplicacion::GetLongName() {
	if (!HasInstallerMetadata())
		return _T("StrongBox");
	if(m_szLongName.IsEmpty()) {
		
		CApplicacion app;
		FN_STRING lpString;
		lpString=(FN_STRING)GetProcAddress(m_pInstancia, "GetLongName");
		if(lpString) 
			m_szLongName=lpString();
	}
	ASSERT(!m_szLongName.IsEmpty());
	return m_szLongName;
}

CString CApplicacion::GetCompany() {
	if (!HasInstallerMetadata())
		return _T("StrongBox");
	if(m_szCompany.IsEmpty()) {
		
		CApplicacion app;
		FN_STRING lpString;
		lpString=(FN_STRING)GetProcAddress(m_pInstancia, "GetCompany");
		if(lpString) 
			m_szCompany=lpString();
	}
	ASSERT(!m_szCompany.IsEmpty());
	return m_szCompany;
}

CString CApplicacion::GetURL() {

	if(m_szURL.IsEmpty()) {

		CApplicacion app;
		FN_STRING lpString;
		lpString=(FN_STRING)GetProcAddress(m_pInstancia, "GetURL");
		if(lpString) 
			m_szURL=lpString();
	}
	ASSERT(!m_szURL.IsEmpty());
	return m_szURL;
}

void CApplicacion::SetAppKey(CString key) {

	m_szAppKey= key;
}

CString CApplicacion::GetAppKey() {
	if (!HasInstallerMetadata())
		return _T("Software\\StrongBox\\") + GetAppName();
	if(m_szAppKey.IsEmpty()) {
		
		CApplicacion app;
		FN_STRING lpString;
		lpString=(FN_STRING)GetProcAddress(m_pInstancia, "GetAppKey");
		if(lpString) 
			m_szAppKey=lpString();
	}
	ASSERT(!m_szAppKey.IsEmpty());
	return m_szAppKey;
}

int CApplicacion::GetID() {

	if(m_nID==0) {

		CApplicacion app;
		FN_INT lpInt;
		lpInt=(FN_INT)GetProcAddress(m_pInstancia, "GetIDApp");
		if(lpInt) 
			m_nID=lpInt();
	}
	ASSERT(m_nID);
	return m_nID;
}

CString CApplicacion::GetAppPath() {
	if (!HasInstallerMetadata()) {
		const CString path = ExecutablePath();
		return path.Left(path.ReverseFind('\\') + 1);
	}

	if(m_szAppPath.IsEmpty()) {
		
#ifdef _DEBUG
		TCHAR szEXEPath[MAX_PATH];
		GetModuleFileName(NULL, szEXEPath, MAX_PATH );
		CString GlobalPath= szEXEPath;
		GlobalPath.MakeLower();
		
		
		int pos= GlobalPath.ReverseFind('\\');
		m_szAppPath= GlobalPath.Left(pos +1);
		CString Exename= GlobalPath.Right(GlobalPath.GetLength()-pos-1);

		if( m_szExeName!=Exename)
			m_szExeName= Exename;
# else 
		CRegKey key;
		if(key.Open(HKEY_LOCAL_MACHINE,GetAppKey(), KEY_READ)==ERROR_SUCCESS) {
			char ruta[MAX_PATH];
			ULONG tamano= MAX_PATH;
			if(key.QueryStringValue("InstallPath",ruta,&tamano)==ERROR_SUCCESS) {
				m_szAppPath= ruta;
			}
		}
# endif
	}
	ASSERT(!m_szAppPath.IsEmpty());
	if(m_szAppPath.IsEmpty()) {

		TCHAR szEXEPath[MAX_PATH];
		GetModuleFileName(NULL, szEXEPath, MAX_PATH );
		CString GlobalPath= szEXEPath;
		GlobalPath.MakeLower();

		int pos= GlobalPath.ReverseFind('\\');
		m_szAppPath= GlobalPath.Left(pos +1);
	}
	return m_szAppPath;
}

CString CApplicacion::GetAppPath(CString archivo) {
	return GetAppPath() + archivo;
}
