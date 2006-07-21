// DlgBases.cpp : implementation file
//

#include "stdafx.h"
#include "DlgBases.h"
# include "../componentes/newmenu/newmenu.h"
# include "../conector/conector.h"
// CDlgBases dialog

IMPLEMENT_DYNAMIC(CDlgBases, CDialog)
CDlgBases::CDlgBases(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgBases::IDD, pParent)
{
	m_nSeleccion= -1;
	m_bModificar= false;
}

CDlgBases::~CDlgBases()
{
}

void CDlgBases::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTDB, m_LCDB);
	DDX_Control(pDX, IDC_EDITSERVIDOR, m_ENombreServidor);
	DDX_Control(pDX, IDC_EDITNOMBREDB, m_ENombreDB);
	DDX_Control(pDX, IDC_EDITPUERTO, m_EPuerto);
	DDX_Control(pDX, IDOK, m_BOK);
	DDX_Control(pDX, IDCANCEL, m_BCancelar);
	DDX_Control(pDX, IDC_STATICPRIMERO, m_SPrimero);
	DDX_Control(pDX, IDC_STATICSEGUNDO, m_SSegundo);
	DDX_Control(pDX, IDC_EDITREINTENTOS, m_EReintentos);
}


BEGIN_MESSAGE_MAP(CDlgBases, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_NOTIFY(NM_RCLICK, IDC_LISTDB, OnNMRclickListdb)
	ON_NOTIFY(NM_DBLCLK, IDC_LISTDB, OnNMDblclkListdb)
	ON_EN_CHANGE(IDC_EDITSERVIDOR, OnEnChangeEditservidor)
	ON_COMMAND(32001, OnListasEditar)
	ON_COMMAND(32000, OnListasEliminar)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()

void CDlgBases::OnBnClickedOk()
{
	CString nombre, servidor, puerto;
	m_ENombreDB.GetWindowText(nombre);
	m_ENombreServidor.GetWindowText(servidor);
	m_EPuerto.GetWindowText(puerto);
	
	if( m_bModificar) 

		m_LCDB.DeleteItem(m_nSeleccion);
	else 
		m_nSeleccion= m_LCDB.GetItemCount();

	if( servidor.IsEmpty() || nombre.IsEmpty() || puerto.IsEmpty()) {

		if( !m_bModificar) {
		
			CConector	m_pConector;
			for( int i=0; i<m_LCDB.GetItemCount(); i++) {

				servidor= m_LCDB.GetItemText( i, 0);
				nombre= m_LCDB.GetItemText( i, 1);
				puerto= m_LCDB.GetItemText( i, 2);
				m_pConector.AddDataBase( nombre, "", "", servidor, atol(puerto));
			}
			m_pConector.Guardar();
			m_EReintentos.GetWindowText(puerto);
			AfxGetApp()->WriteProfileInt("control", "retry", atol(puerto));
			OnOK();
		}
		return;
	}
	m_LCDB.InsertItem(m_nSeleccion, servidor);
	m_LCDB.SetItemText(m_nSeleccion, 1, nombre);
	m_LCDB.SetItemText(m_nSeleccion, 2, puerto);
	m_ENombreDB.SetWindowText("");
	m_ENombreServidor.SetWindowText("");
	puerto.Format("%d", MYSQL_PORT);
	m_EPuerto.SetWindowText(puerto);
	m_bModificar= false;
	m_BOK.SetWindowText("&Aceptar");
}

void CDlgBases::OnNMRclickListdb(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if( pNMListView->iItem == -1 || pNMListView->iSubItem == -1) 

		return;

	m_nSeleccion= pNMListView->iItem;

	CNewMenu menu;

	menu.CreatePopupMenu();
	menu.InsertMenu(0, MF_BYPOSITION | MF_STRING, 32001, "&Editar");
	menu.InsertMenu(1, MF_BYPOSITION | MF_STRING, 32000, "&Eliminar");
	
	POINT point;
    GetCursorPos( &point );
	menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	*pResult = 0;
}

void CDlgBases::OnNMDblclkListdb(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if( pNMListView->iItem == -1 || pNMListView->iSubItem == -1) 

		return;
	m_nSeleccion= pNMListView->iItem;
	OnListasEditar();
	*pResult = 0;
}

BOOL CDlgBases::OnInitDialog()
{
	CDialog::OnInitDialog();
	CString temp;
	

	m_SPrimero.SetNextSectionId( IDC_STATICSEGUNDO);
	m_SPrimero.Collapse();

	temp.Format("%d", MYSQL_PORT);
	m_EPuerto.SetWindowText(temp);
	temp.Format("%d", AfxGetApp()->GetProfileInt("control", "retry", 3));
	m_EReintentos.SetWindowText(temp);
	m_LCDB.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_LCDB.InsertColumn(0, "Servidor", LVCFMT_LEFT, 100);
	m_LCDB.InsertColumn(1, "Base de datos", LVCFMT_LEFT, 100);
	m_LCDB.InsertColumn(2, "Puerto", LVCFMT_LEFT, 50);

	CConector	m_pConector;

	m_pConector.Cargar();
	CArrayDB *info=NULL;
	for( int i=0; i<m_pConector.GetDatabasesCount(); i++) {

		info= m_pConector.GetDatabaseAt(i);
		m_LCDB.InsertItem( i, info->GetHost());
		m_LCDB.SetItemText( i, 1, info->GetDB());
		temp.Format("%d", info->GetPort());
		m_LCDB.SetItemText( i, 2, temp);
	}
	return TRUE;
}

void CDlgBases::OnEnChangeEditservidor()
{
	if( m_bModificar) 
		return;
	if( m_ENombreServidor.GetWindowTextLength()) {

		m_BOK.SetWindowText("&Agregar");
		if( !m_ENombreDB.GetWindowTextLength()) {
			m_ENombreDB.SetWindowText("sdgfingersb");
		}
	}
	else m_BOK.SetWindowText("&Aceptar");
}

void CDlgBases::OnListasEditar()
{
	m_bModificar= true;
	m_nSeleccion= m_LCDB.GetNextItem(-1, LVNI_SELECTED);
	m_BOK.SetWindowText("&Modificar");
	m_ENombreServidor.SetWindowText( m_LCDB.GetItemText( m_nSeleccion, 0));
	m_ENombreDB.SetWindowText( m_LCDB.GetItemText(m_nSeleccion, 1));
	m_EPuerto.SetWindowText( m_LCDB.GetItemText( m_nSeleccion, 2));
}

void CDlgBases::OnListasEliminar()
{
	m_nSeleccion= m_LCDB.GetNextItem(-1, LVNI_SELECTED);
	if( MessageBox("Esta Seguro de elminar a " + m_LCDB.GetItemText( m_nSeleccion, 0),NULL, MB_ICONQUESTION | MB_YESNO)==IDYES) {

		m_LCDB.DeleteItem(m_nSeleccion);
		m_bModificar= true;
		OnBnClickedCancel();
	}
}

void CDlgBases::OnBnClickedCancel()
{
	if( m_bModificar) {

		m_ENombreDB.SetWindowText("");
		CString temp;
		temp.Format("%d", MYSQL_PORT);
		m_EPuerto.SetWindowText(temp);
		m_ENombreServidor.SetWindowText("");
		m_nSeleccion= -1;
		m_bModificar= false;
		m_BOK.SetWindowText("&Aceptar");
	} else 	
		OnCancel();
}
