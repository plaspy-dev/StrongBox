// MainFrm.cpp: implementación de la clase CMainFrame
//

#include "stdafx.h"

#include "SVE.h"
# include "DlgIngreso.h"
# include "DlgBases.h"
# include "atlbase.h"
# include "afxole.h"
# include "../iofile/iofile.h"
# include "../iofile/import.h"
#include "../iofile/FileInfo.h"
# include "../componentes/newmenu/mdichildwndxp.h"
# include "mytime.h"
# include "../componentes/componentes.h"
# include "administrador/FormUser.h"
# include "../sveclient/TrialInfo.h"

#include "MainFrm.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

# include "svedoc.h"
# include "SVEView.h"
# include "informesview.h"


# define	JUPDATER_TIMER			50041
# define	JUPDATER_INTERVALO		60*1000*60
# define	JUPDATER_INTERVALO_INICIAL	1000*60
# define	ANIMATION_TIMER			300
# define	TIMER_EVENT				32000
# define	ACELERADORES_PREFIJO	32000
# define	MESSAGE_DELAY			3000

// LoadFunciones
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWndXP)
IMPLEMENT_JDOCK(CMainFrame)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWndXP)
	ON_WM_CREATE()
	ON_JDOCK_MESSAGES()
	ON_WM_CLOSE()
	ON_COMMAND(ID_VER_NAVEGADOR, OnVerNavegador)
	ON_UPDATE_COMMAND_UI(ID_VER_NAVEGADOR, OnUpdateVerNavegador)
	ON_COMMAND(ID_CONFIGURACION_SERVIDORES, OnConfiguracionServidores)
	ON_WM_TIMER()
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_COMMAND(ID_EDIT_SELECT_ALL, OnEditSelectAll)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEAR, OnUpdateEditClear)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SELECT_ALL, OnUpdateEditSelectAll)
	ON_COMMAND(ID_NEXT_PANEEX, OnNextPane)
	ON_UPDATE_COMMAND_UI(ID_NEXT_PANEEX, OnUpdateNextPane)
	ON_COMMAND(ID_PREV_PANEEX, OnPrevPane)
	ON_UPDATE_COMMAND_UI(ID_PREV_PANEEX, OnUpdatePrevPane)
	ON_WM_ACTIVATEAPP()
	ON_UPDATE_COMMAND_UI(ID_FILE_NEW, OnUpdateFileNew)
	ON_MESSAGE(WM_QUERYTIME, OnQueryTime)
	ON_MESSAGE(WM_STATUSBAR, OnStatusBar)
	ON_MESSAGE(WM_TREENAVEGADOR,OnTreeNavegador)
	ON_MESSAGE(WM_QUERYCONECTOR,OnQueryConector)
	ON_MESSAGE(WM_SETSTATUS,OnSetStatus)
	ON_MESSAGE(WM_TREADRESULT,OnThreadResult)
	ON_MESSAGE(WM_QUERYUSUARIO,OnQueryUsuario) 
	ON_MESSAGE(WM_MYSQL,OnMySQL)
	ON_UPDATE_COMMAND_UI_RANGE(ACELERADORES_PREFIJO, ACELERADORES_PREFIJO+50, OnUpdateMenus)
	ON_COMMAND_RANGE(ACELERADORES_PREFIJO, ACELERADORES_PREFIJO+50, OnCommandMenu)
	ON_UPDATE_COMMAND_UI_RANGE(VENTANAS_PREFIJO, VENTANAS_PREFIJO+50, OnUpdateVentanas)
	ON_COMMAND_RANGE(VENTANAS_PREFIJO, VENTANAS_PREFIJO+50, OnCommandVentanas)
	ON_COMMAND(ID_WINDOW_CLOSE_ALL, OnCloseAll)
	ON_MESSAGE(WM_INSERTODS,OnInsertODS)
	ON_COMMAND(ID_VER_PROPIEDADES, OnVerPropiedades)
	ON_UPDATE_COMMAND_UI(ID_VER_PROPIEDADES, OnUpdateVerPropiedades)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,
	ID_INDICATOR_EXT,
	ID_INDICATOR_EXT,
	ID_INDICATOR_EXT,
	ID_INDICATOR_EXT,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
};

CMainFrame::CMainFrame()
{
	m_bFocus= true;
	m_pUsuario= NULL;
	m_ulTotal=0;
	m_ulRealizados=0;
	m_nInactivo= 0;
	m_bConectando= false;
	m_bInactivo= false;
	m_pFadeWnd= NULL;
	m_nCurrentHilos=0;
	m_nAnimation=0;
	m_pTLocal= m_pTServidor= CTime::GetCurrentTime();
}

CMainFrame::~CMainFrame()
{
	if(m_pUsuario)
		delete m_pUsuario;
	CStrongBoxMiscelaneosEx *info;
	for( int i=0; i<m_pMenus.GetCount(); i++) {
		if( info= m_pMenus.GetAt(i)) {
			CNewMenu *menu = (CNewMenu *)info->GetData();
            delete menu;
		}
	}
	SAFE_DELETE(m_pFadeWnd);
}

bool IsWindowsNT() {

	OSVERSIONINFO osv;
	osv.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	if (GetVersionEx(&osv))
	{
	 switch(osv.dwPlatformId)
	 {
		case VER_PLATFORM_WIN32s:
		case VER_PLATFORM_WIN32_WINDOWS:
			return false;
		case VER_PLATFORM_WIN32_NT:
		default:
			return true;
		}
	}
	return false;
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWndXP::OnCreate(lpCreateStruct) == -1)
		return -1;
	// barra de herramientas principal
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_HIDE_INPLACE | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("No se pudo crear la barra de herramientas\n");
		return -1;      // No se pudo crear
	}
	// barra de estado
	if (!m_wndStatusBar.CreateStatusBar(this, indicators, sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("No se pudo crear la barra de estado\n");
		return -1;      // Fehler beim Erzeugen
	}
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	CREATE_JDOCK(this);

	// barras de herramientas
	if (!m_MyBar2.Create(_T("Navegador"),WS_CHILD, this, 200))
	{
		TRACE0("Failed to create my bar2\n");
		return -1;
	}

	if (!m_PropBar.Create(_T("Propiedades"), WS_CHILD,this, 202))
	{
		TRACE0("Failed to create my ODS Bar\n");
		return -1;
	}
	
	m_MyBar2.SetBarStyle(m_MyBar2.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	m_PropBar.SetBarStyle(m_PropBar.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	m_MyBar2.EnableDocking(CBRS_ALIGN_ANY);
	m_PropBar.EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_MyBar2, AFX_IDW_DOCKBAR_RIGHT);
    DockControlBar(&m_PropBar, AFX_IDW_DOCKBAR_LEFT);

	// los tabs
	DWORD style = MT_TOP | MT_IMAGES;
	style|= AfxGetApp()->GetProfileInt("control", "showtab", 1)==2 ? MT_HIDEWLT2VIEWS : 0;
	m_mdiTabs.Create(this, style);
	
	// el conector
	m_pConector.SetMainFrame( this);
	m_pConector.Cargar();
	
	// barra de estado
	m_wndStatusBar.SetMode(1, XSB_PROGRESS | XSB_SMOOTH);
	m_wndStatusBar.SetRange(1, 0, 100);
	m_wndStatusBar.SetWidth(1, 100);
	m_wndStatusBar.GetProgressCtrl(1)->SetShowText(false);
	m_wndStatusBar.SetMode(2, XSB_TEXT | DT_CENTER);
	m_wndStatusBar.SetText(2, " ");
	m_wndStatusBar.SetWidth( 2, 100);
	
	if( AfxGetApp()->GetProfileInt("control", "animation", 1)) {
		m_wndStatusBar.SetMode(4, XSB_BITMAP | DT_CENTER);
		CString temp;
		temp.Format("%d", IDB_BLOADING1);
		m_nAnimation=IDB_BLOADING1;
		m_wndStatusBar.SetBitmap(4, temp, temp);
		SetTimer(ANIMATION_TIMER, AfxGetApp()->GetProfileInt("control", "animationtimer", 200), NULL);
	} else {
		m_wndStatusBar.SetMode(4, XSB_TEXT | DT_CENTER);
		m_wndStatusBar.SetNumber(4, 0, 0);
	}

	m_wndStatusBar.SetMode(3, XSB_TEXT | DT_CENTER);
	m_wndStatusBar.SetText(3, " ", " ");
	m_wndStatusBar.SetWidth(3, 100);
	
	m_DefaultNewMenu.LoadToolBar(IDR_MAINFRAME, NULL);
	Aldia();
	if(!Registrar())
		return -1;
#ifdef ENABLE_LICENSE_CHECK
	SetTimer(4, 30*60*1000, NULL);
#endif
	return 0;
}

void CMainFrame::ResetToolbars() {
    
	DockControlBar(&m_wndToolBar,AFX_IDW_DOCKBAR_TOP);

	m_MyBar2.OnCerrar();
	m_PropBar.OnCerrar();

	DockControlBar(&m_MyBar2, AFX_IDW_DOCKBAR_RIGHT);
	DockControlBar(&m_PropBar, AFX_IDW_DOCKBAR_LEFT);

	m_MyBar2.LoadStateBar("Barras\\Barras");
	m_PropBar.LoadStateBar("Barras\\Barras");

	m_MyBar2.ShowWindow(SW_SHOW);
	m_PropBar.m_nLastAlingDocking= CBRS_ALIGN_LEFT;
	m_PropBar.SetAutoHide(true);
	m_PropBar.InitAutoHide();

	RecalcLayout();
	AfxGetApp()->WriteProfileInt("control", "notstore", 1);
}

void CMainFrame::DockControlBarLeftOf(CToolBar* Bar, CToolBar* LeftOf)
{
	CRect rect;
	DWORD dw;
	UINT n;

	RecalcLayout(TRUE);
	
	LeftOf->GetWindowRect(&rect);
	rect.OffsetRect(1,0);
	dw=LeftOf->GetBarStyle();
	n = 0;
	n = (dw&CBRS_ALIGN_TOP) ? AFX_IDW_DOCKBAR_TOP : n;
	n = (dw&CBRS_ALIGN_BOTTOM && n==0) ? AFX_IDW_DOCKBAR_BOTTOM : n;
	n = (dw&CBRS_ALIGN_LEFT && n==0) ? AFX_IDW_DOCKBAR_LEFT : n;
	n = (dw&CBRS_ALIGN_RIGHT && n==0) ? AFX_IDW_DOCKBAR_RIGHT : n;
	DockControlBar(Bar,n,&rect);
}

void CMainFrame::LoadState() {

	CWaitCursor wait;

	LoadBarState("barras\\Barras");
	m_MyBar2.LoadStateBar("Barras\\Barras");
	m_PropBar.LoadStateBar("Barras\\Barras");

	if( !AfxGetApp()->GetProfileInt("barras\\Barras", "store",0)) {
		m_MyBar2.ShowWindow(SW_SHOW);
		m_PropBar.m_nLastAlingDocking= CBRS_ALIGN_LEFT;
		m_PropBar.SetAutoHide(true);
		m_PropBar.InitAutoHide();
	}
	
	int nfocus;
	CString szDocumentos=AfxGetApp()->GetProfileString("control", "classname");
	CStringArray documentos;

	while( (nfocus=szDocumentos.Find(","))>-1) {

		documentos.Add(szDocumentos.Mid(0, nfocus));
		szDocumentos.Delete(0, nfocus+1);
	}
	if( !szDocumentos.IsEmpty())
		documentos.Add(szDocumentos);

	nfocus= AfxGetApp()->GetProfileInt("control","focus", -1);

	CDocManager *doc= AfxGetApp()->m_pDocManager;
	CMultiDocTemplateXP *temp, *focus=NULL;
	CString temporal;
	if( !doc)
		return;

	for( int i=0; i<documentos.GetCount(); i++) {

		temporal= documentos.GetAt(i);
		POSITION pos= doc->GetFirstDocTemplatePosition();
		while( pos) {

			temp= (CMultiDocTemplateXP *) doc->GetNextDocTemplate(pos);
			if( temp && temp->GetViewClassName()==temporal) {

				PostMessage(WM_TREENAVEGADOR, NM_DBLCLK, (LPARAM) temp);
				if( nfocus==i)
					focus= temp;
				break;
			}
		}
	}
	if( focus)
		PostMessage(WM_TREENAVEGADOR, NM_DBLCLK, (LPARAM) focus);
}

bool CMainFrame::Registrar() {
	
	return true;
}

void CMainFrame::SaveState() {

	if( !m_pUsuario || !m_pUsuario->GetID())

		return;

	if(AfxGetApp()->GetProfileInt("control", "notstore", 0)==0) {
		CString temp;
		TCITEM item;
		item.mask = TCIF_PARAM;
		HWND hWnd;

		for( int i=0; i<m_mdiTabs.GetItemCount(); i++) {

			m_mdiTabs.GetItem(i, &item);
			hWnd = HWND(item.lParam);
			for( CWnd *child=m_LogoMdiClientWnd.GetTopWindow(); child; child =child->GetNextWindow()) {

				if( hWnd ==(*child)) {
					if( !temp.IsEmpty())
						temp+=",";
					temp+=((CMDIChildWnd *) child)->GetActiveView()->GetRuntimeClass()->m_lpszClassName;
					break;
				}
			}
		}
		AfxGetApp()->WriteProfileString("control","classname",temp);
		AfxGetApp()->WriteProfileInt("control","focus", m_mdiTabs.GetCurFocus());
	} else {
		AfxGetApp()->WriteProfileString("control","classname","");
		AfxGetApp()->WriteProfileInt("control","focus", -1);
	}
	AfxGetApp()->WriteProfileInt("barras\\Barras", "store",1);
	SaveBarState("barras\\Barras");
	m_MyBar2.SaveBar("Barras\\Barras");
	m_PropBar.SaveBar("Barras\\Barras");
}

bool CMainFrame::Conectar() {

	FUNCTION_STACK_TRACE("AAAAAAAA");
	bool existe=false;
	if( m_pUsuario) 
		if( m_pUsuario->GetID())
			existe=true;	
	if( !m_pConector.GetDatabasesCount()) {

		OnConfiguracionServidores();
		if( !m_pConector.GetDatabasesCount())
			return false;
	}
	
	CDlgIngreso dlg;
	KillTimer(50000);
	dlg.m_bExiste= existe;
	m_bConectando= true;
	if( dlg.DoModal()==IDOK) {

		if( !existe)
			m_MyBar2.m_Tree.DeleteAllItems();
		m_pConector.SetPasswd( dlg.m_szPasswd);
		m_pConector.SetUser(dlg.m_szNombreUsuario);

		if( !m_pUsuario)
			m_pUsuario= new CUsuario( dlg.m_szNombreReal, dlg.m_dwID, dlg.m_dwGrupo);
		else {
			m_pUsuario->SetUser(dlg.m_szNombreReal);
			m_pUsuario->SetId(dlg.m_dwID);
			m_pUsuario->SetGrupo(dlg.m_dwGrupo);
		}
		// m_pConector.SetRealUser(dlg.m_szNombreReal);

		m_wndStatusBar.SetText(2, dlg.m_szNombreReal);
		m_PropBar.SetConector(&m_pConector);

		// registrarse en el chat
		SendMessage(WM_REGISTRARUSUARIO, 0, -2);
		if( !existe) {
			AfxGetApp()->WriteProfileInt("control", "loadstate", 1);
			LoadDocumentos();
			LoadState();
			m_pConector.Query("SELECT SYSDATE()", TIMER_EVENT, this);
		}
		SendMessageBroadCast(WM_QUERYUSUARIO, 0, (LPARAM)m_pUsuario);
		SendMessageBroadCast(WM_QUERYCONECTOR, 0, (LPARAM)&m_pConector);
		if( AfxGetApp()->GetProfileInt("control", "timeout", 300))
			SetTimer(50000, 1000, NULL);
		SAFE_DELETE(m_pFadeWnd);
		SendMessageBroadCast(WM_INACTIVIDAD, 0,1);
		AfxGetApp()->WriteProfileInt("control", "loadstate", 0);
		m_bConectando= false;
		return true;
	}
	return false;
}

bool CMainFrame::AddAcelerator(ACCEL nueva) {

	int cAcelerator= CopyAcceleratorTable(m_hAccelTable, NULL, 0)+1;
	LPACCEL lpaccelnew= (LPACCEL) LocalAlloc( LPTR, cAcelerator* sizeof(ACCEL));
	cAcelerator--;
	if(!lpaccelnew)
		return false;
	CopyAcceleratorTable(m_hAccelTable, lpaccelnew, cAcelerator);
	lpaccelnew[cAcelerator].cmd=nueva.cmd;
	lpaccelnew[cAcelerator].fVirt= nueva.fVirt;
	lpaccelnew[cAcelerator].key= nueva.key;
	DestroyAcceleratorTable(m_hAccelTable);
	cAcelerator++;
	m_hAccelTable= CreateAcceleratorTable(lpaccelnew, cAcelerator);
	ASSERT(CopyAcceleratorTable(m_hAccelTable, NULL, 0)==cAcelerator);
	return true;
}

CString CMainFrame::ParseAcelerator(CString label, int id) {

	ACCEL nueva;
	nueva.fVirt= FCONTROL | FNOINVERT | FVIRTKEY;
	nueva.cmd= ACELERADORES_PREFIJO+id;
	nueva.key= '1'+id;
	CString temp;

	if( nueva.key<='9') {
		AddAcelerator(nueva);
		temp.Format("%s\tCtrl+%c", label, nueva.key);
	} else if( nueva.key=='9'+1) {
		nueva.key='0';
		AddAcelerator(nueva);
		temp.Format("%s\tCtrl+%c", label, nueva.key);
	} else if( nueva.key<='9'+10) {
		nueva.key-=10;
		nueva.fVirt= FALT | FNOINVERT | FVIRTKEY;
		AddAcelerator(nueva);
		temp.Format("%s\tAlt+%c", label, nueva.key);
	} else if( nueva.key=='9'+11) {
		nueva.key='0';
		nueva.fVirt= FALT | FNOINVERT | FVIRTKEY;
		AddAcelerator(nueva);
		temp.Format("%s\tAlt+%c", label, nueva.key);
	} else if( nueva.key<='9'+20) {
		nueva.key-=20;
		nueva.fVirt= FALT | FNOINVERT | FVIRTKEY | FCONTROL;
		AddAcelerator(nueva);
		temp.Format("%s\tCtrl+Alt+%c", label, nueva.key);
	} else if( nueva.key=='9'+21) {
		nueva.key='0';
		nueva.fVirt= FALT | FNOINVERT | FVIRTKEY | FCONTROL;
        AddAcelerator(nueva);
		temp.Format("%s\tCtrl+Alt+%c", label, nueva.key);
	} else temp= label;
	return temp;
}

bool CMainFrame::LoadDocument( CNewMenu *menu, HTREEITEM hRoot, CString nombre, UINT nIDResource,CRuntimeClass* pViewClass) {

	HTREEITEM item= m_MyBar2.m_Tree.InsertItem( nombre, hRoot);

	CString temp;

	CMultiDocTemplateXP* pDocTemplate;
	pDocTemplate = new CMultiDocTemplateXP(nIDResource,RUNTIME_CLASS(CSVEDoc),RUNTIME_CLASS(CMDIChildWndXP), pViewClass);
	AfxGetApp()->AddDocTemplate( pDocTemplate);

	temp= pViewClass->m_lpszClassName;
	m_MyBar2.m_Tree.SetItemData( item, (DWORD) pDocTemplate);

	m_pAceleradores.Add(temp, ACELERADORES_PREFIJO+m_pAceleradores.GetCount());
	temp= ParseAcelerator(nombre, m_pAceleradores.GetCount()-1);
	menu->AppendMenu( MF_STRING, ACELERADORES_PREFIJO+m_pAceleradores.GetCount()-1, temp);

	return true;
}

bool CMainFrame::LoadDocumentos() {

	FUNCTION_STACK_TRACE("AAAAAAAB");

	HTREEITEM hRoot;

	hRoot= m_MyBar2.m_Tree.InsertItem( "StrongBox");
	m_MyBar2.m_Tree.SetItemState(hRoot, TVIS_BOLD,TVIS_BOLD);

	CNewMenu *mimenu= new CNewMenu();

	mimenu->CreateMenu();

	LoadDocument(mimenu, hRoot, "Ingreso", IDR_SVETYPE,RUNTIME_CLASS(CSVEView));
	LoadDocument(mimenu, hRoot, "Informes", IDR_SVETYPE,RUNTIME_CLASS(CInformesView));
	if( m_pUsuario->Pertenece(GRUPO_ADMINISTRADOR)) {
		LoadDocument(mimenu, hRoot, "Usuarios", IDR_SVETYPE,RUNTIME_CLASS(CFormUser));
	}

	m_MyBar2.m_Tree.Expand(hRoot, TVE_EXPAND);

	// modificamos el menu del mainframe
	OnCreateChild(m_DefaultNewMenu.m_hMenu);

	CDocManager *doc= AfxGetApp()->m_pDocManager;
	if( doc) {
		POSITION pos= doc->GetFirstDocTemplatePosition();
		CMultiDocTemplate *temp;
		while( pos) {
			temp= (CMultiDocTemplate *)doc->GetNextDocTemplate(pos);
			if( temp)
				OnCreateChild(temp->m_hMenuShared);
		}
	}
	return true;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{

	if( AfxGetApp()->GetProfileInt("control", "loadstate", 0)) {
		CRegKey key;
		if(	key.Open(HKEY_CURRENT_USER, "Software\\SDG\\SDGFingerSB\\control")==ERROR_SUCCESS) {

			key.DeleteValue("posicion");
			key.DeleteValue("status");
			key.DeleteValue("focus");
			key.DeleteValue("classname");
			key.Close();
		}
		if( key.Open(HKEY_CURRENT_USER, "Software\\SDG\\SDGFingerSB")==ERROR_SUCCESS) {
			if(key.RecurseDeleteKey("barras")!=ERROR_SUCCESS)
				key.DeleteSubKey("barras");
			if(key.RecurseDeleteKey("ingreso")!=ERROR_SUCCESS)
				key.DeleteSubKey("ingreso");
			if(key.RecurseDeleteKey("listas")!=ERROR_SUCCESS)
				key.DeleteSubKey("listas");
			if(key.RecurseDeleteKey("chat")!=ERROR_SUCCESS)
				key.DeleteSubKey("chat");
			if(key.RecurseDeleteKey("consulta")!=ERROR_SUCCESS)
				key.DeleteSubKey("consulta");
			key.Close();
		}
	}
	int show= AfxGetApp()->GetProfileInt("control", "status", -1);
	if( show!=-1) { // ya se habia escrito

		if( show!= SW_SHOWMINIMIZED)
			AfxGetApp()->m_nCmdShow= show;
		RECT *myrect;
		CRect rect(0,0,0,0);
		show = 0;
		if( AfxGetApp()->GetProfileBinary("control", "posicion", (LPBYTE*)&myrect, (UINT *)&show)) {
			rect= *myrect;
			delete [] myrect;
			if( rect.Width()>0 && rect.Height()>0) { // restauramos la posicion de la ventana

				if( rect.Height()<400)
					rect.bottom= rect.top+400;
				if( rect.Width()<400)
					rect.right= rect.left+400;	
				if( rect.left<0)
					rect.MoveToX(0);
				if( rect.top<0)
					rect.MoveToY(0);

				int x= GetSystemMetrics(SM_CXSCREEN);
				int y= GetSystemMetrics(SM_CYSCREEN);
                
				if( rect.left<x && rect.top<y) {
					
					if( rect.Height()<y && rect.Width()<x) {
					
						if( rect.right>x)
							rect.MoveToX(x-rect.Width());
						if( rect.bottom>y)
							rect.MoveToY(y-rect.Height());

						cs.x = rect.left;
						cs.y = rect.top;
						cs.cx= rect.Width();
						cs.cy= rect.Height();
					}
				}
			}
		}
	}
	if( !CMDIFrameWndXP::PreCreateWindow(cs) )
		return FALSE;
	return TRUE;
}

// Diagnósticos de CMainFrame

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWndXP::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWndXP::Dump(dc);
}

#endif //_DEBUG

void CMainFrame::OnUpdateVentanas(CCmdUI *pCmdUI) {

	pCmdUI->Enable((pCmdUI->m_nID-VENTANAS_PREFIJO)<m_mdiTabs.GetItemCount());
}

void CMainFrame::OnCommandVentanas(UINT id) {
	if((id-VENTANAS_PREFIJO)<m_mdiTabs.GetItemCount())
		m_mdiTabs.SetSelEx(id-VENTANAS_PREFIJO);
}

void CMainFrame::OnUpdateFrameTitle(BOOL bAddToTitle)
{
	FUNCTION_STACK_TRACE("AAAAAAAC");
	CMDIFrameWnd::OnUpdateFrameTitle(bAddToTitle);
	if (!m_mdiTabs.GetSafeHwnd())
		return;
	m_mdiTabs.Update();
	int sel= m_mdiTabs.GetCurSel();
	if( sel!=-1) {
	
		TCITEM item;
		item.mask = TCIF_PARAM;
		m_mdiTabs.GetItem(sel, &item);
		HWND hWnd = HWND(item.lParam);

		for (CWnd *child=m_LogoMdiClientWnd.GetTopWindow(); child; child =child->GetNextWindow()) 
			
			if( hWnd== (*child)) {
				
				CView *view= ((CMDIChildWnd *) child)->GetActiveView();
				if( view) m_MyBar2.SetSel(view->GetRuntimeClass()->m_lpszClassName);
				break;
			}
	}
}

LRESULT CMainFrame::OnTreeNavegador(WPARAM wParam, LPARAM lParam) {

	if( wParam== NM_DBLCLK) {

		if( lParam) {
			CMultiDocTemplateXP* temp= (CMultiDocTemplateXP *)lParam;
			ASSERT(temp);
			CWnd *child=m_LogoMdiClientWnd.GetTopWindow();
			CView *view;

			for (; child; child =child->GetNextWindow()) {
			
				view=((CMDIChildWnd *) child)->GetActiveView(); 
				if( view && view->GetRuntimeClass()->m_lpszClassName==temp->GetViewClassName()) {
					
					child->BringWindowToTop();
					child->SetFocus();
					return 0;
				}
			}
			temp->OpenDocumentFile(NULL);
		}
	} else if( wParam==100) {
		HTREEITEM item=m_MyBar2.FindItemEx((const char *)lParam);
		if( item)
			SendMessage(WM_TREENAVEGADOR, NM_DBLCLK, m_MyBar2.m_Tree.GetItemData(item));
	}
	return 0;
}

LRESULT CMainFrame::OnStatusBar(WPARAM wParam, LPARAM lParam) {

	if( wParam==1 || wParam==4) { // agregamos un nuevo elemento
		m_ulRealizados= wParam==1 ? m_ulRealizados+1 : lParam;
		if( m_ulTotal) {
			int pos= (int) (m_ulRealizados*100)/m_ulTotal;
			m_wndStatusBar.SetPos(1,pos);
			char real[MAX_PATH],real1[MAX_PATH];
			CString atributo;
			_i64toa(m_ulRealizados, real, 10);
			_i64toa(m_ulTotal, real1, 10);
			atributo.Format( "%s de %s", real, real1);
			m_wndStatusBar.GetProgressCtrl(1)->SetWindowText(atributo);
		}
	} else if( wParam==0) {// iniciamos la busqueda donde lparam es el total

		m_ulTotal= lParam;
		m_wndStatusBar.GetProgressCtrl(1)->SetShowText(AfxGetApp()->GetProfileInt("toolbar", "showtext",false));
		m_wndStatusBar.GetProgressCtrl(1)->SetPos(0);
	} else if( wParam==2) { // terminamos la busqueda
		m_wndStatusBar.GetProgressCtrl(1)->SetShowText(true);
		m_wndStatusBar.SetPos(1, 0);
		if( m_ulTotal) {
			char real[MAX_PATH];
			CString atributo;
			_i64toa(m_ulTotal, real, 10);
			atributo.Format( "%s Resultado%s", real, m_ulTotal>1 ? "s" : "");
			m_wndStatusBar.GetProgressCtrl(1)->SetWindowText(atributo);
		}
		m_ulTotal=0;
		m_ulRealizados=0;
	}
	return 0;
}

LRESULT CMainFrame::OnQueryConector(WPARAM wParam, LPARAM lParam) {

	CWnd *temp= (CWnd *) lParam;
	temp->SendMessage( WM_QUERYCONECTOR, 0, (LPARAM) &m_pConector);
	return (LRESULT) &m_pConector;
}

LRESULT CMainFrame::OnSetStatus(WPARAM wParam, LPARAM lParam) {

	CString temp= (const char *) lParam;
	m_wndStatusBar.RestorePane(0);
	m_wndStatusBar.SetWindowText(temp);
	if( wParam>1) {

		m_wndStatusBar.SavePane(0);
		m_wndStatusBar.SetBkColor(0, wParam==2 ? GetSysColor(COLOR_ACTIVECAPTION) : RGB(255,0,0));
		m_wndStatusBar.SetFgColor(0, GetSysColor(COLOR_CAPTIONTEXT));
		MessageBeep(MB_OK);
		SetTimer( 123, MESSAGE_DELAY, NULL);
	}
	return 0;
} 

LRESULT CMainFrame::OnThreadResult(WPARAM wParam, LPARAM lParam) {

	switch( wParam) {
		case 0:
			if( m_nAnimation) {
				m_nCurrentHilos--;
			} else m_wndStatusBar.Decrement(4);
			break;
		case 1:
			if( m_nAnimation) {
				m_nCurrentHilos++;
			} else m_wndStatusBar.Increment(4);
			break;
		default:
			{
				CString temp;
				temp.Format("conector\\servidor%d", (UINT)lParam);
				m_wndStatusBar.SetText(3, AfxGetApp()->GetProfileString(temp, "servidor"));
			}
			break;
	}
	return 0;
} 

LRESULT CMainFrame::OnQueryUsuario(WPARAM wParam, LPARAM lParam) {

	CWnd *temp= (CWnd *) lParam;
	temp->SendMessage( WM_QUERYUSUARIO, 1, (LPARAM) m_pUsuario);
	return (LRESULT) m_pUsuario;
} 

int CMainFrame::OnMySQLThreadEnd(CThreadMySQL *hilo) {

	PostMessage(WM_STATUSBAR, 2);
	if( hilo->m_nID>49000 || TIMER_EVENT==hilo->m_nID) {
		return 0;
	} else if( hilo->m_nID>99) {
	} else DeleteHilos(hilo);
	return 0;
}

int CMainFrame::OnMySQLQueryResult(MySQLMessage *message) {

	switch( message->GetID()) {
		case TIMER_EVENT:
			{
				CStrongBoxMyTime tiempo(message->GetAt(0));
				m_pTServidor=tiempo.m_pTiempo;
				m_pTLocal= CTime::GetCurrentTime();
				m_psDiferencia= m_pTLocal-m_pTServidor;
				SetTimer(TIMER_EVENT, 1000, NULL);
				SendMessageBroadCast(WM_QUERYTIME, 0, (LPARAM)&m_pTServidor);
			}
		case 50001: // no se hace nada
			return 0;
	}
	if( message->GetID()>99) {
        PostMessage(WM_STATUSBAR, 1);
	} else Messages(message); // SOLO en el caso de NEW_XML
	return 0;
}

int CMainFrame::OnQueryEmpty(int owner) {
	
	switch( owner) {
		case TIMER_EVENT:
		case 50001: // no se hace nada
			return 0;
	}
	return 0;
}

int CMainFrame::OnQueryTitle(MySQLMessage *message) {

	switch( message->GetID()) {
		case TIMER_EVENT:
		case 50001: // no se hace nada
		case 50000: // chat
			return 0;
	}
	return 0;
}

LRESULT CMainFrame::OnMySQL(WPARAM wParam, LPARAM lParam) {

	switch( wParam) {
		case THREAD_END:
			OnMySQLThreadEnd((CThreadMySQL *)lParam);
			break;
		case QUERY_COUNT2:
			PostMessage(WM_STATUSBAR, 0, ((MySQLMessage *)lParam)->GetCount());
			break;
		case QUERY_RESULT:
			OnMySQLQueryResult((MySQLMessage *)lParam);
			break;
		case QUERY_EMPTY:
			OnQueryEmpty(((MySQLMessage *)lParam)->GetID());
			break;
		case QUERY_TITLE:
			OnQueryTitle((MySQLMessage *)lParam);
			break;
		case QUERY_ERROR:
			{
				MySQLMessage *message= (MySQLMessage *)lParam;
# ifdef _DEBUG
				SetErrorEx(NULL, message->GetAt(0)+ " \r\nMientras se realizaba la consulta\r\n" + message->GetAt(1), 3, "mysql");
# endif
			}
			break;
	}
	return 0;
}

void CMainFrame::OnUpdateMenus(CCmdUI *pCmdUI) {

	pCmdUI->Enable(m_pAceleradores.Find(pCmdUI->m_nID)!=NULL);
}

void CMainFrame::OnCommandMenu(UINT id) {

	CStrongBoxMiscelaneosEx *info=m_pAceleradores.Find(id);
	if( info)
		PostMessage(WM_TREENAVEGADOR, NM_DBLCLK, m_MyBar2.m_Tree.GetItemData(m_MyBar2.FindItemEx(info->GetNombre())));
}

LRESULT CMainFrame::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	return CMDIFrameWndXP::WindowProc(message, wParam, lParam);
}

bool CMainFrame::DeleteHilos( CThreadMySQL *hilo) {
	return true;
}

void CMainFrame::SendMessageBroadCast(UINT message,WPARAM wParam, LPARAM lParam) {

	for ( CWnd *child=m_LogoMdiClientWnd.GetTopWindow(); child; child =child->GetNextWindow()) {
		CView *vista=((CMDIChildWnd *) child)->GetActiveView();
		if( vista && ::IsWindow(*vista))
			vista->SendMessage(message, wParam, lParam);
	}
}

bool CMainFrame::Messages( MySQLMessage *contenido) {

	FUNCTION_STACK_TRACE("AAAAAAAE");
	PostMessage( WM_STATUSBAR, 1);
	return true;
}
void CMainFrame::OnClose()
{	

	if( AfxGetApp()->GetProfileInt("control", "notstore", 0)) {
		AfxGetApp()->WriteProfileInt("control","status",SW_SHOWMINIMIZED);
		AfxGetApp()->WriteProfileBinary("control", "posicion", 0, 1);
	} else {
		WINDOWPLACEMENT wp;
		GetWindowPlacement(&wp);
		AfxGetApp()->WriteProfileInt("control","status", wp.showCmd);
		AfxGetApp()->WriteProfileBinary("control", "posicion", (LPBYTE)&wp.rcNormalPosition, (UINT) sizeof( wp.rcNormalPosition));
	}
	SaveState();
	AfxGetApp()->WriteProfileInt("control","notstore",0);
	CMDIFrameWndXP::OnClose();
}

void CMainFrame::OnVerNavegador()
{
	if( !m_MyBar2.IsWindowVisible()) 

		if(m_MyBar2.IsHide()) {

			m_MyBar2.InitShow();
			return;
		}
	m_MyBar2.ShowWindow((m_MyBar2.IsWindowVisible()) == 0);
	RecalcLayout();
}

void CMainFrame::OnUpdateVerNavegador(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_pUsuario!=NULL);
	pCmdUI->SetCheck( m_MyBar2.IsWindowVisible());
}

void CMainFrame::OnConfiguracionServidores()
{
	CDlgBases mydb;
	if( mydb.DoModal()==IDOK) {

		if( m_pUsuario) {
			SetErrorEx(NULL, "Debe Reiniciar SDGFinger Control acceso para realizar los cambios", 0, "conector");
		} else {
			m_pConector.SetMainFrame( this);
			m_pConector.Cargar();
		}
	}
}

LRESULT CMainFrame::OnQueryTime(WPARAM wParam, LPARAM lParam) {

	if( lParam) 
		((CWnd *) lParam)->SendMessage(WM_QUERYTIME, 0, (LPARAM)&m_pTServidor);
	return (LRESULT) &m_pTServidor;
}

void CMainFrame::OnTimer(UINT nIDEvent)
{
	if( nIDEvent==TIMER_EVENT) {
		CTime tiempo= CTime::GetCurrentTime();
		CTimeSpan span(0, 0, 1, 0);
		if( tiempo<m_pTLocal || span < tiempo-m_pTLocal) {
			KillTimer(TIMER_EVENT);
			m_pConector.Query("SELECT SYSDATE()", TIMER_EVENT, this);
		} else {
			m_pTLocal= tiempo;
			m_pTServidor= m_pTLocal-m_psDiferencia;
		}
	} else if( nIDEvent==ANIMATION_TIMER) {
		
		if( !m_nCurrentHilos) {
			if( m_nAnimation!= IDB_BLOADING1) {
				m_nAnimation=IDB_BLOADING1;
				CString temp;
				temp.Format("%d", m_nAnimation);
				m_wndStatusBar.SetBitmap(4, temp, temp);
			}
			return;
		}
		if( m_nAnimation==IDB_BLOADING10) 
			m_nAnimation=IDB_BLOADING1;
		m_nAnimation++;
		CString temp;
		temp.Format("%d", m_nAnimation);
		m_wndStatusBar.SetBitmap(4, temp, temp);
	} else if( nIDEvent==123) {

		KillTimer(123);
		m_wndStatusBar.RestorePane(0);
		return;
	} else if( nIDEvent==	JUPDATER_TIMER) {

		KillTimer( JUPDATER_TIMER);
		if( !m_szPathUpdate.IsEmpty() && m_nTipoActualizacion) {

			CString temp;
			temp.Format( "\"%s\" %d sdgfingersb",m_szPathUpdate, m_nTipoActualizacion);
			WinExec( temp, SW_MINIMIZE);
		}
		return;
	} else if( nIDEvent==50000) {
		// inactividad
		KillTimer(nIDEvent);
		return;
		m_nInactivo++;
		if( m_bInactivo || m_nInactivo>=AfxGetApp()->GetProfileInt("control", "timeout", 300)) {
			KillTimer(50000);
			SendMessage(WM_REGISTRARUSUARIO, 0, -3);
			SendMessageBroadCast(WM_INACTIVIDAD, 0,0);
			m_bInactivo=true;
			SAFE_DELETE(m_pFadeWnd);
			if( m_bFocus) {
				if( AfxGetApp()->GetProfileInt("control", "efecto", 1) && IsWindowsNT())
					m_pFadeWnd= new CFadeWnd(this);
				while( Conectar()==false) {
					if( AfxMessageBox( "Esta acción Cerrara SDGFinger.\n¿Desea Cerrarlo?", MB_ICONQUESTION | MB_YESNO)==IDYES) {
						PostMessage(WM_CLOSE);
						return;
					}
				}
				m_bInactivo=false;
				m_nInactivo=0;
			}
		}
#ifdef ENABLE_LICENSE_CHECK
	} else if(nIDEvent==4) {
		CStrongBoxTrialInfo info;
		if( info.GetLeftDays()<=0) {
			MessageBox("Su periodo de prueba.\rHa terminado");
			PostMessage(WM_CLOSE);
		}
#endif
	}
	CMDIFrameWndXP::OnTimer(nIDEvent);
}

void CMainFrame::Aldia() {
	if (!CApplicacion::HasInstallerMetadata())
		return;

	HKEY hRootKey = HKEY_LOCAL_MACHINE;
	CRegKey hReg;
	CString temp;
	TCHAR szEXEPath[MAX_PATH];

	if( ERROR_SUCCESS==hReg.Create( hRootKey, _T("software\\SDG\\SDGFingerSB"))) {

		temp= CApplicacion::GetAppPath();
		hReg.SetStringValue( "InstallPath", temp);
		hReg.Close();
	}
	m_szPathUpdate= _T("");
	if( ERROR_SUCCESS== hReg.Open( hRootKey, _T("software\\SDG\\SDGFingerSB"), KEY_READ)) {

		ULONG	tamano= MAX_PATH;
		if( hReg.QueryStringValue( "JUpdaterPath", szEXEPath, &tamano)== ERROR_SUCCESS) 

			m_szPathUpdate= szEXEPath;
		hReg.Close();
	}
	if( m_szPathUpdate.IsEmpty())

		return;
	m_nTipoActualizacion= AfxGetApp()->GetProfileInt("jupdater", "Actualizacion", 1);
	if( m_nTipoActualizacion)

		SetTimer( JUPDATER_TIMER, JUPDATER_INTERVALO_INICIAL, NULL);
}


void CMainFrame::OnEditPaste()
{
	GetFocus()->SendMessage(WM_PASTE);
}

void CMainFrame::OnEditCopy()
{
	GetFocus()->SendMessage(WM_COPY);
}

void CMainFrame::OnEditCut()
{
	GetFocus()->SendMessage(WM_CUT);
}

void CMainFrame::OnEditUndo()
{
	GetFocus()->SendMessage(WM_UNDO);
}

void CMainFrame::OnEditClear()
{
	GetFocus()->SendMessage(WM_CLEAR);
}

void CMainFrame::OnEditSelectAll()
{
	CWnd *focus= GetFocus();
	if( focus) {
		if( focus->IsKindOf(RUNTIME_CLASS(CEdit))) 
			((CEdit *)focus)->SetSel(0, focus->GetWindowTextLength());
		else  if( focus->IsKindOf(RUNTIME_CLASS(CRichEditCtrl))) 
			((CRichEditCtrl*) focus)->SetSel(0, focus->GetWindowTextLength());
		else focus->SendMessage(WM_COMMAND, ID_EDIT_SELECT_ALL,0);
	}
}

void CMainFrame::OnUpdateEditPaste(CCmdUI *pCmdUI)
{
	CWnd *focus=GetFocus();
	bool enable=false;
	if( focus && focus->IsKindOf(RUNTIME_CLASS(CEdit))) {
		if(!(focus->GetStyle()&ES_READONLY)) {
			COleDataObject	obj;
			if (obj.AttachClipboard() && obj.IsDataAvailable(CF_TEXT))
				enable=true;
		}
	} else enable=true;
	pCmdUI->Enable( enable);
}

void CMainFrame::OnUpdateEditUndo(CCmdUI *pCmdUI)
{
	bool enable= false;
	CWnd *focus= GetFocus();
	if( focus) 
		if(focus->IsKindOf(RUNTIME_CLASS(CEdit))) 
			enable= ((CEdit *)focus)->CanUndo();
		else if( focus->IsKindOf(RUNTIME_CLASS(CListCtrl)))
			enable= ((CListCtrl*)focus)->GetItemCount()!=0;
		else enable=true;
	pCmdUI->Enable(enable);
}

void CMainFrame::OnUpdateEditCopy(CCmdUI *pCmdUI)
{
	CWnd *focus= GetFocus();
	int x=0, y=0;
	if( focus) 
		if(focus->IsKindOf(RUNTIME_CLASS(CEdit)) || focus->IsKindOf(RUNTIME_CLASS(CRichEditCtrl)))
			((CEdit *)focus)->GetSel(x,y);
		else if( focus->IsKindOf(RUNTIME_CLASS(CListCtrl)))
			y= ((CListCtrl *)focus)->GetItemCount();
		else y= 1;
	pCmdUI->Enable(x!=y);
}

void CMainFrame::OnUpdateEditCut(CCmdUI *pCmdUI)
{
	CWnd *focus= GetFocus();
	int x=0, y=0;
	if( focus) {
		if(focus->IsKindOf(RUNTIME_CLASS(CEdit)) || focus->IsKindOf(RUNTIME_CLASS(CRichEditCtrl))) {
			if(!(focus->GetStyle()&ES_READONLY)) {
				((CEdit *)focus)->GetSel(x,y);
			}
		} else if( focus->IsKindOf(RUNTIME_CLASS(CListCtrl))) {
			y=((CListCtrl *)focus)->GetItemCount();
		} else y=1;
	}
	pCmdUI->Enable(x!=y);
}

void CMainFrame::OnUpdateEditClear(CCmdUI *pCmdUI)
{
	bool enable=false;
	CWnd *focus= GetFocus();
	if( focus) {
		if( focus->IsKindOf(RUNTIME_CLASS(CEdit))) {
			if(!(focus->GetStyle()&ES_READONLY)) {
				enable= focus->GetWindowTextLength()>0;
			}
		} else if( focus->IsKindOf(RUNTIME_CLASS(CListCtrl))) {
			enable= ((CListCtrl *)focus)->GetItemCount()!=0;
		} else enable=true;
	}
	pCmdUI->Enable(enable);
}

void CMainFrame::OnUpdateEditSelectAll(CCmdUI *pCmdUI)
{
	bool enable=false;
	CWnd *focus= GetFocus();
	if( focus) {
		if(focus->IsKindOf(RUNTIME_CLASS(CEdit)))
			enable= focus->GetWindowTextLength()>0;
		else if (focus->IsKindOf(RUNTIME_CLASS(CListCtrl)))
			enable= ((CListCtrl *)focus)->GetItemCount()!=0;
		else if( focus->IsKindOf(RUNTIME_CLASS(CRichEditCtrl)))
			enable= focus->GetWindowTextLength()>0;
		else enable=true;
	}
	pCmdUI->Enable(enable);
}

void CMainFrame::OnCreateChild(HMENU menu) {

	CNewMenu *nose= (CNewMenu *)CNewMenu::FromHandle(menu);
	if( nose) {
		if( nose->GetSafeHmenu() && nose->GetMenuItemCount()) {
			CNewMenu *submenu, *add;
			CString nombre;
			CStrongBoxMiscelaneosEx *info;

			for( int i=0; i<m_pMenus.GetCount(); i++) {
				submenu= (CNewMenu *)nose->GetSubMenu(0);
				if(submenu) {
					submenu->GetMenuString(0, nombre, MF_BYPOSITION);
					info= m_pMenus.GetAt(i);
					if( nombre!=info->GetNombre()) {
						add= (CNewMenu *) info->GetData();
						if( i==0)
							submenu->ModifyMenu(i, MF_BYPOSITION | MF_POPUP, (UINT)add->m_hMenu, info->GetNombre());
						else 
							submenu->InsertMenu(i, MF_BYPOSITION | MF_POPUP, (UINT) add->m_hMenu, info->GetNombre());
					}
				}
			}
		}
	}
}

void CMainFrame::OnNextPane()
{
	m_mdiTabs.SetSelEx(m_mdiTabs.GetCurSel()+1);
}

void CMainFrame::OnUpdateNextPane(CCmdUI *pCmdUI)
{
	pCmdUI->Enable((m_mdiTabs.GetCurSel()<(m_mdiTabs.GetItemCount()-1)));
}

void CMainFrame::OnPrevPane()
{
	m_mdiTabs.SetSelEx(m_mdiTabs.GetCurSel()-1);
}

void CMainFrame::OnUpdatePrevPane(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_mdiTabs.GetCurSel()>0);
}

void CMainFrame::OnActivateApp(BOOL bActive, DWORD dwThreadID)
{
	CMDIFrameWndXP::OnActivateApp(bActive, dwThreadID);
	m_bFocus= bActive;
	if( bActive) {
		if( m_bInactivo && !m_bConectando) {
			KillTimer(50000);
			SetTimer(50000, 100, NULL);
		}
	}
}

void CMainFrame::OnUpdateFileNew(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!m_pUsuario || AfxGetApp()->GetProfileInt("control", "timeout", 1));
}

void CMainFrame::OnCloseAll() {

	int pos= m_mdiTabs.GetCurSel();
	if( pos>0) {
		CWaitCursor wait;
		DWORD data= m_mdiTabs.GetItemData(pos);
		DWORD del;
		ASSERT(data);
		pos=0;
		while( m_mdiTabs.GetItemCount()>1) {
			del= m_mdiTabs.GetItemData(pos);
			if( del==data)
				pos++;
			else ::SendMessage(HWND(del), WM_CLOSE, 0,0);
		}
	}
}

LRESULT CMainFrame::OnInsertODS(WPARAM wParam, LPARAM lParam) {

	if(wParam==10) {
		CString m_szODS= (const char *) lParam;
		m_PropBar.SetODS(m_szODS, wParam);
	} else {
		SendMessage(WM_TREENAVEGADOR, NM_DBLCLK, m_MyBar2.m_Tree.GetItemData(m_MyBar2.FindItemEx("CSVEView")));
		SendMessageBroadCast(WM_INSERTODS, wParam, lParam);
	}
	return 0;
}

void CMainFrame::OnVerPropiedades()
{
	if( !m_PropBar.IsWindowVisible()) 

		if(m_PropBar.IsHide()) {

			m_PropBar.InitShow();
			m_PropBar.Update(true);
			return;
		}
	if(!m_PropBar.IsWindowVisible())
		m_PropBar.Update(true);
	m_PropBar.ShowWindow((m_PropBar.IsWindowVisible()) == 0);
	RecalcLayout();
}

void CMainFrame::OnUpdateVerPropiedades(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_pUsuario!=NULL);
	pCmdUI->SetCheck( m_PropBar.IsWindowVisible());
}
