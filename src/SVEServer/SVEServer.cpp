// SVEServer.cpp : define los comportamientos de las clases para la aplicación.
//

#include "stdafx.h"
#include "SVEServer.h"
#include "SVEServerDlg.h"
# include "../conector/defender.h"
# include "../iofile/myregkey.h"
#include "../sve/LicenceDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSVEServerApp

BEGIN_MESSAGE_MAP(CSVEServerApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// Construcción de CSVEServerApp

CSVEServerApp::CSVEServerApp()
{
}


// El único objeto CSVEServerApp

CSVEServerApp theApp;


// Inicialización de CSVEServerApp

BOOL CSVEServerApp::InitInstance()
{
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

#ifdef ENABLE_LICENSE_CHECK
	if (!CApplicacion::HasInstallerMetadata()) {
		AfxMessageBox(_T("El servidor requiere el modulo de instalacion de StrongBox y una licencia valida."), MB_ICONERROR);
		return FALSE;
	}

	SetRegistryKey(_T("SDG"));

	CMyRegKey key;
	key.MyOpen(HKEY_LOCAL_MACHINE, "SoftWare\\SDG\\SDGFingerCA\\control");
	CDefender info(key.GetString("licencia","", ""));
	if( info.IsValidSerial()!=&info) {
		CLicenceDlg dlg;
		if(dlg.DoModal()!=IDOK)
			return false;
		CDefender info1(key.GetString("licencia","", ""));
		if( info1.IsValidSerial()!=&info1)
			return false;
	}

#else
	SetRegistryKey(CApplicacion::HasInstallerMetadata() ? _T("SDG") : _T("StrongBox"));
#endif
	CSVEServerDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{	
	}
	else if (nResponse == IDCANCEL)
	{
	}
	return FALSE;
}
