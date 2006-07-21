// SVEServer.h: archivo de encabezado principal para la aplicación PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "incluir 'stdafx.h' antes de incluir este archivo para PCH"
#endif

#include "resource.h"		// Símbolos principales


// CSVEServerApp:
// Consulte la sección SVEServer.cpp para obtener información sobre la implementación de esta clase
//

class CSVEServerApp : public CWinApp
{
public:
	CSVEServerApp();

// Reemplazos
	public:
	virtual BOOL InitInstance();

// Implementación

	DECLARE_MESSAGE_MAP()
};

extern CSVEServerApp theApp;