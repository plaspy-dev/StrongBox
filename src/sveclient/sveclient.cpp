// sveclient.cpp : define los comportamientos de las clases para la aplicación.
//

#include "stdafx.h"
#include "sveclient.h"
#include "sveclientDlg.h"
# include "../conector/defender.h"
# include "../iofile/myregkey.h"
#include "../sve/LicenceDlg.h"
# include "trialinfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CsveclientApp

BEGIN_MESSAGE_MAP(CsveclientApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// Construcción de CsveclientApp

CsveclientApp::CsveclientApp()
{
}


// El único objeto CsveclientApp

CsveclientApp theApp;


// Inicialización de CsveclientApp

BOOL CsveclientApp::InitInstance()
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

	SetRegistryKey(CApplicacion::HasInstallerMetadata() ? _T("SDG") : _T("StrongBox"));
	CString cmd= m_lpCmdLine;

	HWND handle= FindWindow(NULL, "SDGFinger - Cliente");
	if(handle && ::IsWindow(handle)) {
		if( cmd.Find("-close")==-1) {
			ShowWindow(handle, SW_NORMAL);
			SetForegroundWindow(handle);
		} else {
			SendMessage(handle, WM_COMMAND, 2003, 0);
		}
		return false;
	}
	if( cmd.Find("-close")!=-1) {
		return false;
	}

#ifndef  _DEBUG
	/*CStrongBoxTrialInfo trial;
	// trial.Register();
	if( cmd.Find("-Register")!=-1) {
		trial.Register();
		return false;
	}

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

	int dias= trial.GetLeftDays();
	if(dias<=0) {
		AfxMessageBox("Su periodo de Prueba.\rha Terminado.");
		return false;
	} else {
		cmd.Format("Quedan %d días", dias);
		AfxMessageBox(cmd);
	}*/
#endif

	CsveclientDlg dlg;
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
