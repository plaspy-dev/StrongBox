// Hhelp.h : main header file for the Hhelp DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

# ifndef __HHELP_EXPORT__
#define __HHELP_EXPORT__ __declspec(dllimport)
# endif

# include <IOFile/IOFile.h>

# define WM_HHELP	WM_USER+1055

# include "HelpBar.h"
# include "HtmlHelpView.h"

class CHhelpApp : public CWinApp
{
public:
	CHhelpApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

#ifndef _HHELP_DLL_
#ifdef _DEBUG
# pragma comment(lib,"hhelpd")
# else
# pragma comment(lib,"hhelp")
# endif
#endif
