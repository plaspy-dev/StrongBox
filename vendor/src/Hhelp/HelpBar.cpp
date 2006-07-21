#include "StdAfx.h"
#include "helpbar.h"
# include <ResourceDLL.h>
# include "HtmlHelpView.h"

CHelpBar::CHelpBar(void)
{
	m_pDocTemplate= NULL;
}

CHelpBar::~CHelpBar(void)
{
}

BEGIN_MESSAGE_MAP(CHelpBar, CGuiControlBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

int CHelpBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CGuiControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	CResourceDLL res;
	m_pDlgHelp.Create( CFormHelp::IDD, this);
	m_pDlgHelp.ShowWindow(SW_NORMAL);
	return 0;
}


void CHelpBar::ScrollOn() { // on show

	CGuiControlBar::ScrollOn();
	// todo aqui
}
void CHelpBar::OnSize(UINT nType, int cx, int cy)
{
	CGuiControlBar::OnSize(nType, cx, cy);
	if( ::IsWindow(m_pDlgHelp))
		m_pDlgHelp.MoveWindow(0,0, cx, cy);
}

CMultiDocTemplateXP *CHelpBar::GetHelpDoc() {
	
	if(m_pDocTemplate==NULL) {
		CWinApp* pApp = AfxGetApp();
		ASSERT(pApp != NULL);
		m_pDocTemplate= new CMultiDocTemplateXP(128,
			RUNTIME_CLASS(CComponentesDoc),
			RUNTIME_CLASS(CMDIChildWndXP),
			RUNTIME_CLASS(CHtmlHelpView));
		pApp->AddDocTemplate(m_pDocTemplate);
	}
	return m_pDocTemplate;
}


CString	CHelpBar::FindTitulo(const CString &titulo) {

	return m_pDlgHelp.FindTitulo(titulo);
}
