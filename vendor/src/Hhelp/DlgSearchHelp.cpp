// DlgSearchHelp.cpp: archivo de implementación
//

#include "stdafx.h"
#include "Hhelp.h"
#include "DlgSearchHelp.h"


// Cuadro de diálogo de CDlgSearchHelp

IMPLEMENT_DYNAMIC(CDlgSearchHelp, CTabPageSSL)
CDlgSearchHelp::CDlgSearchHelp(CWnd* pParent /*=NULL*/)
	: CTabPageSSL(CDlgSearchHelp::IDD, pParent)
{
	m_nComboBox=0;
	m_pSearch= NULL;
	m_pArchivos= NULL;
}

CDlgSearchHelp::~CDlgSearchHelp()
{
	if(m_pSearch) {
		delete m_pSearch;
		m_pSearch= NULL;
	}
}

void CDlgSearchHelp::DoDataExchange(CDataExchange* pDX)
{
	CTabPageSSL::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CBUSCAR, m_CBBuscar);
	DDX_Control(pDX, IDC_BBUSCAR, m_BBuscar);
	DDX_Control(pDX, IDC_LRESULTADOS, m_LCResultados);
}


BEGIN_MESSAGE_MAP(CDlgSearchHelp, CTabPageSSL)
	ON_BN_CLICKED(IDC_BBUSCAR, OnBnClickedBbuscar)
	ON_WM_SIZE()
	ON_NOTIFY(NM_DBLCLK, IDC_LRESULTADOS, OnNMDblclkLresultados)
END_MESSAGE_MAP()

BOOL CDlgSearchHelp::OnInitDialog()
{
	CTabPageSSL::OnInitDialog();

	CString temp;

	for( int i=0; i<100; i++) {
		temp.Format("palabra%d", i);
		temp= AfxGetApp()->GetProfileString("search", temp);
		if( temp.IsEmpty())
			break;
		m_CBBuscar.AddString(temp);
	}
	m_nComboBox= AfxGetApp()->GetProfileInt("search", "index", 0);
	if(m_nComboBox==100)
		m_nComboBox=0;
	m_LCResultados.InsertColumn(0, "Titulo");
	m_LCResultados.InsertColumn(1, "Puntaje");
	m_LCResultados.SetOrdenable();
	m_LCResultados.SetCanClean();
	m_LCResultados.SetCanShowMenu();
	m_LCResultados.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_LCResultados.AutoSizeColumns();
	m_LCResultados.SetMessageLista("");
	m_LCResultados.SetType(1, 1);
	m_BBuscar.EnableWindow(m_pArchivos!=NULL);
	m_BBuscar.m_bValidate=false;
	return TRUE;
}

void CDlgSearchHelp::OnBnClickedBbuscar()
{
	CString temp;
	m_CBBuscar.GetWindowText(temp);
	if( temp.IsEmpty())
		return;
	if( m_CBBuscar.FindStringExact(-1, temp)==-1) {
		
		m_CBBuscar.AddString(temp);
		CString formato;
		formato.Format("palabra%d", m_nComboBox);
		AfxGetApp()->WriteProfileString("search", formato, temp);
		m_nComboBox++;
		if(m_nComboBox==100) 
			m_nComboBox=0;
		AfxGetApp()->WriteProfileInt("search", "index", m_nComboBox);
	}
	m_LCResultados.SetMessageLista("Buscando");
	m_CBBuscar.SetFocus();
	if(m_pSearch) {
		delete m_pSearch;
		m_pSearch= NULL;
	}
	m_pSearch= new CThreadSearch();
	m_pSearch->m_pArchivos= m_pArchivos;
	m_pSearch->m_pList= &m_LCResultados;
	m_pSearch->m_pParent= this;
	m_pSearch->m_szCadena= temp;
	m_pSearch->Run();
}

void CDlgSearchHelp::SetArchivos(CMiscelaneosEx *archivos) {

	m_pArchivos= archivos;
	if(::IsWindow(m_BBuscar))
		m_BBuscar.EnableWindow(m_pArchivos!=NULL);
}

void CDlgSearchHelp::OnSize(UINT nType, int cx, int cy)
{
	CTabPageSSL::OnSize(nType, cx, cy);
	if( ::IsWindow(m_LCResultados)) {
		
		CRect rect, rect1;

		m_BBuscar.GetWindowRect(rect);
		m_CBBuscar.GetWindowRect(rect1);
		m_CBBuscar.MoveWindow( 2, 2, cx-rect.Width()-6, rect1.Height());
		m_BBuscar.MoveWindow(cx-rect.Width()-2, 2, rect.Width(), rect.Height());
		m_LCResultados.MoveWindow(2, rect.Height()+4, cx-4, cy-rect.Height()-8);
	}
}

void CDlgSearchHelp::OnNMDblclkLresultados(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* item = (NM_LISTVIEW*)pNMHDR;
	if( item->iItem == -1 || item->iSubItem == -1) 
		return;
	CMiscelaneosEx *info=(CMiscelaneosEx *)m_LCResultados.GetItemData(item->iItem);
	if( info && !info->GetExtra().IsEmpty()) {
		AfxGetMainWnd()->SendMessage(WM_HHELP, 1, (LPARAM) (const char *) info->GetExtra());
	}
	*pResult = 0;
}
