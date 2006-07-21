// Hhelp.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "Hhelp.h"
# include <ResourceDLL.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static AFX_EXTENSION_MODULE HHelpDLL = { NULL, NULL };

// CHhelpApp

BEGIN_MESSAGE_MAP(CHhelpApp, CWinApp)
END_MESSAGE_MAP()


// CHhelpApp construction

CHhelpApp::CHhelpApp()
{
}

CHhelpApp theApp;

BOOL CHhelpApp::InitInstance()
{
	CWinApp::InitInstance();
	if (!AfxInitExtensionModule(HHelpDLL, m_hInstance))
		return false;
	new CDynLinkLibrary(HHelpDLL);
#ifndef _DEBUG
	if(CApplicacion::HasInstallerMetadata() && !RegisterFile(CApplicacion::GetAppPath("hhelp.dll"))) {

			CString texto;
			texto.Format("%s Esta corrupto.\nPor favor ReInstale %s o Informe al Administrador del sistema.", CApplicacion::GetAppName(), CApplicacion::GetAppName());
			AfxMessageBox(texto, MB_ICONERROR);
			return false;
	}
#endif
	return TRUE;
}

CResourceDLL::CResourceDLL()
{
	m_hOldInst = AfxGetResourceHandle ();
	ASSERT (m_hOldInst != NULL);

	AfxSetResourceHandle( HHelpDLL.hResource );
}

CResourceDLL::~CResourceDLL()
{
	AfxSetResourceHandle( m_hOldInst );
}
