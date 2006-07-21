// HtmlHelp.cpp: archivo de implementación
//

#include "stdafx.h"
#include "Hhelp.h"
#include "HtmlHelpview.h"
# include <resourcedll.h>
// CHtmlHelpView

IMPLEMENT_DYNCREATE(CHtmlHelpView, CHtmlView)

CHtmlHelpView::CHtmlHelpView()
{
	m_bVisibleToolbar=false;
}

CHtmlHelpView::~CHtmlHelpView()
{
}

void CHtmlHelpView::DoDataExchange(CDataExchange* pDX)
{
	CHtmlView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CHtmlHelpView, CHtmlView)
	ON_MESSAGE(WM_HHELP, OnQueryNavigate)
	ON_COMMAND(ID_ATRAS, OnAtras)
	ON_COMMAND(ID_ADELANTE, OnAdelante)
	ON_COMMAND(ID_CANCELAR, OnCancelar)
	ON_COMMAND(ID_RELOAD, OnReload)
	ON_COMMAND(ID_HOME, OnHome)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipText)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipText)
END_MESSAGE_MAP()


// Diagnósticos de CHtmlHelpView

#ifdef _DEBUG
void CHtmlHelpView::AssertValid() const
{
	CHtmlView::AssertValid();
}

void CHtmlHelpView::Dump(CDumpContext& dc) const
{
	CHtmlView::Dump(dc);
}
#endif //_DEBUG


// Controladores de mensajes de CHtmlHelpView

void CHtmlHelpView::OnInitialUpdate()
{
	CWaitCursor wait;
	CHtmlView::OnInitialUpdate();

	GetDocument()->SetTitle("Ayuda");

	CString ruta= CMyRegKey::GetProfileString("ayuda", "ruta", "ayuda\\");
	m_szAppPath= CApplicacion::GetAppPath(ruta);
	
	Navigate2( AfxGetApp()->GetProfileString("search", "last",m_szAppPath + ruta + "inicio.html"));

	CMDIFrameWndXP *main= (CMDIFrameWndXP *)AfxGetMainWnd();
	ASSERT(main);
	
	CResourceDLL *res= new CResourceDLL();
	if (!m_wndToolBarNavegador.CreateEx(main, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_HIDE_INPLACE | CBRS_SIZE_DYNAMIC, 
		CRect(0,0,0,0), IDR_TOOLBARNAVEGADOR) ||
		!m_wndToolBarNavegador.LoadToolBar(IDR_TOOLBARNAVEGADOR))
	{
		TRACE0("No se pudo crear la barra de herramientas DEL navegador\n");
		return ;      // No se pudo crear
	}
	delete res;

	m_wndToolBarNavegador.EnableDocking(CBRS_ALIGN_ANY);
	main->DockControlBar(&m_wndToolBarNavegador, AFX_IDW_DOCKBAR_TOP);
	
	POSITION pos= main->m_listControlBars.GetHeadPosition();
	for (int i=0;i < main->m_listControlBars.GetCount();i++)
	{
		CWnd *handle= (CWnd *)main->m_listControlBars.GetNext(pos);
		if(handle && handle->IsKindOf(RUNTIME_CLASS(CToolBarXP))) {
			main->DockControlBarLeftOf(&m_wndToolBarNavegador,(CToolBar *)handle);
			break;
		}
	}
}

LRESULT CHtmlHelpView::OnQueryNavigate(WPARAM wParam, LPARAM lParam) {

	if( wParam==1) {
        CString pagina= (const char *) lParam;
		Navigate2(m_szAppPath + pagina);
	}
	return 0;
}

void CHtmlHelpView::OnBeforeNavigate2(LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName, CByteArray& baPostedData, LPCTSTR lpszHeaders, BOOL* pbCancel)
{
	CWnd *main= AfxGetMainWnd();
	if(main && ::IsWindow(*main))
		main->SendMessage(WM_HHELP, 0, (LPARAM) (const char *)lpszURL);
	CHtmlView::OnBeforeNavigate2(lpszURL, nFlags, lpszTargetFrameName, baPostedData, lpszHeaders, pbCancel);
}


void CHtmlHelpView::OnAtras() {

	GoBack();
}

void CHtmlHelpView::OnAdelante() {

	GoForward();
}

void CHtmlHelpView::OnCancelar() {

	Stop();
}

void CHtmlHelpView::OnReload() {

	Refresh();
}

void CHtmlHelpView::OnHome() {

	CString ruta= CMyRegKey::GetProfileString("ayuda", "ruta", "ayuda\\");
	Navigate2( m_szAppPath + ruta + "inicio.html");
}

void CHtmlHelpView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	bool visible= false;
	CWnd *mdi;
	for(mdi= AfxGetMainWnd()->GetTopWindow() ; mdi; mdi= mdi->GetNextWindow()) {
		char wndClass[32];
		::GetClassName(*mdi, wndClass, 32);
		if (strncmp(wndClass, "MDIClient", 32) == 0) 
			break;
	}
	if(mdi) {
		mdi= mdi->GetTopWindow();
		if( mdi&& mdi->IsKindOf(RUNTIME_CLASS(CMDIChildWnd))) {

			CView *view= ((CMDIChildWnd *) mdi)->GetActiveView();
			visible= view==this;
		}
	}
	if(m_bVisibleToolbar!=visible) {

		m_bVisibleToolbar=visible;
		m_wndToolBarNavegador.ShowWindow(visible ? SW_NORMAL : SW_HIDE );
		CMDIFrameWndXP *main= (CMDIFrameWndXP *)AfxGetMainWnd();
		ASSERT(main);
		main->RecalcLayout();
	}
	CHtmlView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}


BOOL CHtmlHelpView::OnToolTipText(UINT, NMHDR* pNMHDR, LRESULT* pResult)
{
	ASSERT(pNMHDR->code == TTN_NEEDTEXTA || pNMHDR->code == TTN_NEEDTEXTW);
	// if there is a top level routing frame then let it handle the message
	if (GetRoutingFrame() != NULL) return FALSE;
	// to be thorough we will need to handle UNICODE versions of the message also !!
	TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
	TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
	CString strTipText,strStatus;
	UINT nID = pNMHDR->idFrom;

	if (pNMHDR->code == TTN_NEEDTEXTA && (pTTTA->uFlags & TTF_IDISHWND) ||
		pNMHDR->code == TTN_NEEDTEXTW && (pTTTW->uFlags & TTF_IDISHWND))
	{
		// idFrom is actually the HWND of the tool 
		nID = ::GetDlgCtrlID((HWND)nID);
	}
	if (nID != 0) // will be zero on a separator
	{
		switch(nID) {
			case ID_ATRAS:
				strStatus="Vuelve a la página anterior";
				strTipText="Atras";
				break;
			case ID_ADELANTE:
				strStatus="Vuelve a la página anterior";
				strTipText="Adelante";
				break;
			case ID_CANCELAR:
				strStatus="Cancela la navegación";
				strTipText="Cancelar";
				break;
			case ID_RELOAD:
				strStatus="Vuelve a cargar la página actual";
				strTipText="Actualizar";
				break;
			case ID_HOME:
				strStatus="Va a la página de inicio";
				strTipText="Inicio";
				break;
		}
#ifndef _UNICODE
		if (pNMHDR->code == TTN_NEEDTEXTA)
		{
			lstrcpyn(pTTTA->szText, strTipText, sizeof(pTTTA->szText));
		}
		else
		{
			_mbstowcsz(pTTTW->szText, strTipText, sizeof(pTTTW->szText));
		}
#else
		if (pNMHDR->code == TTN_NEEDTEXTA)
		{
			_wcstombsz(pTTTA->szText, strTipText,sizeof(pTTTA->szText));
		}
		else
		{
			lstrcpyn(pTTTW->szText, strTipText, sizeof(pTTTW->szText));
		}
#endif
		*pResult = 0;
		// bring the tooltip window above other popup windows
		::SetWindowPos(pNMHDR->hwndFrom, HWND_TOP, 0, 0, 0, 0,SWP_NOACTIVATE|SWP_NOSIZE|SWP_NOMOVE|SWP_NOOWNERZORDER);
		SetError(NULL, strStatus);
		return TRUE;
	}
	return FALSE;
}
void CHtmlHelpView::OnTitleChange(LPCTSTR lpszText)
{
	CWnd *main= AfxGetMainWnd();
	if(main) {
		GetDocument()->SetTitle(CApplicacion::GetAppName());
		main->SetWindowText(lpszText);
	} else {
		CString titulo= lpszText;
		titulo+=" - ";
		titulo+=CApplicacion::GetAppName(); 
		GetDocument()->SetTitle(titulo);
	}
	CHtmlView::OnTitleChange(lpszText);
}
