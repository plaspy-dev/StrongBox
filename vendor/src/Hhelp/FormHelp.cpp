// FormHelp.cpp: archivo de implementación
//

#include "stdafx.h"
#include "Hhelp.h"
#include "FormHelp.h"


// Cuadro de diálogo de CFormHelp

IMPLEMENT_DYNAMIC(CFormHelp, CDialog)
CFormHelp::CFormHelp(CWnd* pParent /*=NULL*/)
	: CDialog(CFormHelp::IDD, pParent)
{
}

CFormHelp::~CFormHelp()
{
}

void CFormHelp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TELEMENTOS, m_TabCtrl);
}


BEGIN_MESSAGE_MAP(CFormHelp, CDialog)
	ON_WM_SIZE()
	ON_MESSAGE(WM_HHELP, OnQueryNavigate)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TELEMENTOS, OnTcnSelchangeTelementos)
	ON_NOTIFY(NM_CLICK, IDC_TELEMENTOS, OnNMClickTelementos)
END_MESSAGE_MAP()


// Controladores de mensajes de CFormHelp

BOOL CFormHelp::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_TabCtrl.ModifyStyle(0,TCS_BOTTOM);
	m_pDlgIndex.Create(CDlgIndex::IDD, this);
	m_TabCtrl.AddSSLPage("Indice", 0, &m_pDlgIndex);

	m_pDlgSearch.Create(CDlgSearchHelp::IDD, this);
	m_TabCtrl.AddSSLPage("Buscar", 1, &m_pDlgSearch);
	m_TabCtrl.SetCurFocus(AfxGetApp()->GetProfileInt("search", "tab", 0));

	return TRUE;
}

void CFormHelp::SetArchivos(CMiscelaneosEx *archivos) {

	m_pDlgSearch.SetArchivos(archivos);
}

LRESULT CFormHelp::OnQueryNavigate(WPARAM wParam, LPARAM lParam) {

	CWnd *handle= &m_TabCtrl;
	handle = handle ? m_TabCtrl.GetSSLPage(0) : NULL;
	return handle ? handle->SendMessage(WM_HHELP, wParam, lParam) : 0;
}

void CFormHelp::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	if( ::IsWindow(m_TabCtrl))
		m_TabCtrl.MoveWindow(0,0, cx,cy);
}

void CFormHelp::OnTcnSelchangeTelementos(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
}

void CFormHelp::OnNMClickTelementos(NMHDR *pNMHDR, LRESULT *pResult)
{
	AfxGetApp()->WriteProfileInt("search","tab", m_TabCtrl.GetCurFocus());
	*pResult = 0;
}

CString	CFormHelp::FindTitulo(const CString &titulo) {

	return m_pDlgIndex.FindTitulo(titulo);
}
