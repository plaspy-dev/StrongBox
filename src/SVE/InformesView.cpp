// InformesView.cpp: archivo de implementación
//

#include "stdafx.h"
#include "SVE.h"
#include "InformesView.h"
# include "mytime.h"
# include "../iofile/iofile.h"
# include "resource.h"
# include "../Componentes/EditFloat.h"

// CInformesView

IMPLEMENT_DYNCREATE(CInformesView, CFormView)

CInformesView::CInformesView()
	: CFormView(CInformesView::IDD)
{
	m_pListPrint= NULL;
}

CInformesView::~CInformesView()
{
	if( m_pListPrint) 
		delete m_pListPrint;
}

void CInformesView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_BAceptar);
	DDX_Control(pDX, IDC_LRESULTADOS, m_LCResultados);
	DDX_Control(pDX, IDC_DDESDE, m_DTDesde);
	DDX_Control(pDX, IDC_DHASTA, m_DTHasta);
	DDX_Control(pDX, IDC_EDOCUMENTO, m_EDocumento);
}

BEGIN_MESSAGE_MAP(CInformesView, CFormView)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_MESSAGE(WM_MYSQL, OnMysqlMessage)
	ON_WM_SIZE()
	ON_NOTIFY(NM_DBLCLK, IDC_LRESULTADOS, OnNMDblclkLresultados)
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_PREVIEW, OnUpdatePrint)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_DIRECT, OnUpdatePrint)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT, OnUpdatePrint)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdatePrint)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, OnUpdatePrint)
	ON_COMMAND(ID_FILE_SAVE, OnArchivoExportar)
	ON_COMMAND(ID_FILE_SAVE_AS, OnArchivoExportar)
END_MESSAGE_MAP()


// Diagnósticos de CInformesView

#ifdef _DEBUG
void CInformesView::AssertValid() const
{
	CFormView::AssertValid();
}

void CInformesView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

void CInformesView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	
	GetDocument()->SetTitle("Informes");

	m_pConector= (CConector *)AfxGetMainWnd()->SendMessage(WM_QUERYCONECTOR, 0, (LPARAM)this);
	m_pUsuario= (CUsuario *)AfxGetMainWnd()->SendMessage(WM_QUERYUSUARIO, 0, (LPARAM)this);

	m_LCResultados.SetOrdenable();

	m_LCResultados.InsertColumn(0, "Documento", LVCFMT_RIGHT, 100);
	m_LCResultados.InsertColumn(1, "Nombre", LVCFMT_RIGHT, 100);
	m_LCResultados.InsertColumn(2, "Dinero", LVCFMT_LEFT, 100);
	m_LCResultados.InsertColumn(3, "Fecha", LVCFMT_LEFT, 100);
	m_LCResultados.InsertColumn(4, "Recojido por", LVCFMT_RIGHT, 100);
	m_LCResultados.InsertColumn(5, "Fecha", LVCFMT_RIGHT, 100);

	m_LCResultados.SetType(0, CMyListCtrl::tipocol_numero);
	m_LCResultados.SetType(2, CMyListCtrl::tipocol_double);
	m_LCResultados.SetType(3, CMyListCtrl::tipocol_fechahora);
	m_LCResultados.SetType(5, CMyListCtrl::tipocol_fechahora);

	m_LCResultados.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	m_LCResultados.SetCanShowMenu();

	m_LCResultados.AutoSizeColumns();
	OnBnClickedOk();
}

void CInformesView::OnBnClickedOk()
{
	if( !m_pConector)
		return;
	m_LCResultados.SetMessageLista("Realizando consulta");
	m_LCResultados.DeleteAllItems();

	CString desde, hasta;
	CStrongBoxMyTime tiempo;
	m_DTDesde.GetTime(tiempo.m_pTiempo);
	tiempo.SetTiempo(tiempo.m_pTiempo);
	desde= tiempo.GetFormatoMysql();
	m_DTHasta.GetTime(tiempo.m_pTiempo);
	tiempo.SetTiempo(tiempo.m_pTiempo);
	hasta= tiempo.GetFormatoMysql();
	CString usuario;
	m_EDocumento.GetWindowText(usuario);

	CString query;
	query.Format("SELECT usuario.documento,usuario.nombre,usuario.apellido,sucesos.dinero,sucesos.fecha\
				 ,recibido.nombre,recibido.apellido,sucesos.salida  \
				 FROM sucesos \
				 LEFT JOIN usuario ON sucesos.usuario=usuario.documento \
				 LEFT JOIN usuario AS recibido ON recibido.documento=sucesos.responsable \
				 WHERE sucesos.fecha>=\"%s 00:00:01\" AND sucesos.fecha<=\"%s 23:59:59\"", desde, hasta);
	if( !usuario.IsEmpty())
		query+=" AND (sucesos.usuario='" + usuario + "' OR sucesos.responsable='" + usuario+"' )";

	m_pConector->Query(query,1, this);
}

LRESULT CInformesView::OnMysqlMessage(WPARAM wParam, LPARAM lParam) {

	MySQLMessage *mess=(MySQLMessage *)lParam;
	CThreadMySQL *hilo= (CThreadMySQL *) lParam;

	switch( wParam) {
		case QUERY_EMPTY:
			break;
		case QUERY_RESULT:
			{
				int pos= m_LCResultados.GetItemCount();
				////usuario.documento, 0
				m_LCResultados.InsertItem(pos, mess->GetAt(0));
				//usuario.nombre, 1
				//	usuario.apellido, 2
				m_LCResultados.SetItemText(pos, 1, mess->GetAt(1)+ " "+mess->GetAt(2));
				//	sucesos.cantidad, 3 
				m_LCResultados.SetItemText(pos, 2, CEditFloat::GetFormatNumber(mess->GetAt(3)));
				//	sucesos.fecha\ 4 
				CStrongBoxMyTime tiempo(mess->GetAt(4));
				m_LCResultados.SetItemText(pos, 3, tiempo.GetFechaHora());
				////	,recibido.nombre, 5
				//recibido.apellido, 6 
				m_LCResultados.SetItemText(pos, 4, mess->GetAt(5)+ " "+mess->GetAt(6));
				//sucesos.salida 7
				tiempo.SetDateTime(mess->GetAt(7));
				m_LCResultados.SetItemText(pos, 5, tiempo.GetFechaHora());
			}
			break;
		case QUERY_ERROR:
			MessageBox(mess->GetAt(1), mess->GetAt(0));
			break;
		case THREAD_END:
			m_LCResultados.SetMessageLista("No se encontraron resultados");
			m_LCResultados.AutoSizeColumns();
			break;
	}
	return 0;
}
void CInformesView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);
	if( ::IsWindow(m_LCResultados)) {
		m_LCResultados.MoveWindow(5, 70, cx-10, cy-80);
	}
}

void CInformesView::OnNMDblclkLresultados(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if( pNMListView->iItem == -1 || pNMListView->iSubItem == -1) 
		return;
	CString texto= m_LCResultados.GetItemText(pNMListView->iItem, 0);
	AfxGetMainWnd()->SendMessage(WM_INSERTODS, 0, (LPARAM) atol(texto));
	*pResult = 0;
}

BOOL CInformesView::OnPreparePrinting(CPrintInfo* pInfo)
{

	if( m_pListPrint)
		delete m_pListPrint;
	m_pListPrint= new CListDemoViewPrint();

	m_pListPrint->SetListCtrl(&m_LCResultados);
	m_pListPrint->SetAppName("SVE");
	m_pListPrint->SetDocTitle(GetDocument()->GetTitle());
	m_pListPrint->SetFooterIzq(m_pUsuario->GetNombre());

	CStrongBoxMyTime tiempo;
	tiempo.m_pTiempo= CTime::GetCurrentTime();
	tiempo.SetTiempo( tiempo.m_pTiempo);

	m_pListPrint->SetFooter(tiempo.GetFechaHora(AfxGetApp()->GetProfileInt("fechas", "larga", 0)));

	m_pListPrint->OnPreparePrinting(pInfo);

	m_pListPrint->SetMarginBottomPix(AfxGetApp()->GetProfileInt("printer", "cy", 20));
	m_pListPrint->SetMarginLeftPix(AfxGetApp()->GetProfileInt("printer", "x", 20));
	m_pListPrint->SetMarginRightPix(AfxGetApp()->GetProfileInt("printer", "cx", 20));
	m_pListPrint->SetMarginTopPix(AfxGetApp()->GetProfileInt("printer", "y", 20));

	return DoPreparePrinting(pInfo);
}

void CInformesView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	m_pListPrint->OnBeginPrinting(pDC, pInfo);
}

void CInformesView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	m_pListPrint->OnPrint(pDC, pInfo);
}

void CInformesView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	m_pListPrint->OnEndPrinting(pDC, pInfo);
	delete m_pListPrint;
	m_pListPrint= NULL;
}

void CInformesView::OnUpdatePrint(CCmdUI *pCmdUI) {

	if(m_pUsuario->Pertenece(1) || m_pUsuario->Pertenece(4))
		pCmdUI->Enable(m_LCResultados.GetItemCount()>0);
	else pCmdUI->Enable(false);
}

void CInformesView::OnArchivoExportar()
{
	Exportar( &m_LCResultados, GetDocument()->GetTitle(), true);
}

BOOL CInformesView::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN) {
		if(AfxGetApp()->GetProfileInt("componentes","enter", 0) && pMsg->wParam== VK_RETURN) {

			CWnd *foco= GetFocus();
			if( foco)
				if( !foco->IsKindOf( RUNTIME_CLASS(CButtonXP))) 

					pMsg->wParam= VK_TAB;
		}
	}
	return CFormView::PreTranslateMessage(pMsg);
}
