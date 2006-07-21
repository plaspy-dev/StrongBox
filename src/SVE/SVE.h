// SVE.h: archivo de encabezado principal para la aplicación SVE
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // Símbolos principales


// CSVEApp:
// Consulte la sección SVE.cpp para obtener información sobre la implementación de esta clase
//

class CSVEApp : public CWinApp
{
public:
	CSVEApp();

	LPARAM	m_mlparamold;
// Reemplazos
public:
	virtual BOOL InitInstance();
	afx_msg void OnFileNew();
	afx_msg void OnFileOpen();
	HWND		RegisterApp(HWND mainframe);

// Implementación
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual CDocument* OpenDocumentFile(LPCTSTR lpszFileName);
	afx_msg void OnFileSave();
	afx_msg void OnFilePrintSetup();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

extern CSVEApp theApp;
