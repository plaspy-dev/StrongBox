// DlgIngreso.cpp : implementation file
//

#include "stdafx.h"
#include "DlgIngreso.h"
# include <atlbase.h>

// CDlgIngreso dialog

IMPLEMENT_DYNAMIC(CDlgIngreso, CDialogXP)
CDlgIngreso::CDlgIngreso(CWnd* pParent /*=NULL*/)
	: CDialogXP(CDlgIngreso::IDD, pParent)
{
	m_dwGrupo= 0;
	m_dwID=0;
	m_dwCac= 0;
	m_bExiste= false;
	m_pConector= NULL;
}

CDlgIngreso::~CDlgIngreso()
{
	m_pConector->KillQuery(-1, this);
	m_pConector->SetMainFrame(AfxGetMainWnd());
	if( m_bExiste && m_pConector)
		delete m_pConector;
}

void CDlgIngreso::DoDataExchange(CDataExchange* pDX)
{
	CDialogXP::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDITNOMBRE, m_EUsuario);
	DDX_Control(pDX, IDC_EDITCONTRASENA, m_EContrasena);
	DDX_Control(pDX, IDOK, m_BOK);
	DDX_Control(pDX, IDCANCEL, m_BCancelar);
	DDX_Control(pDX, IDC_STATICAYUDA, m_SAyuda);
}


BEGIN_MESSAGE_MAP(CDlgIngreso, CDialogXP)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancelar)
END_MESSAGE_MAP()


// CDlgIngreso message handlers
bool CDlgIngreso::validateUser(CString usuario, CString passwd, bool codificado) {

	m_EContrasena.EnableWindow(false);
	m_EUsuario.EnableWindow(false);
	m_BOK.EnableWindow(false);

	// m_pConector->SetRealUser(usuario);

	if( codificado) { // convertir a md5

		char user[MAX_PATH];
		CString usuariomin= usuario;
		usuariomin.MakeLower();
		strcpy(user, usuariomin);
		m_szNombreUsuario= m_pCoding.GetMD5((BYTE *)user, strlen(user)).Mid(0, 15);
	} else m_szNombreUsuario= usuario;

	m_pConector->SetUser(m_szNombreUsuario);
	m_pConector->SetPasswd(passwd);

	m_dwID= 0;
	m_dwGrupo= 0;
	m_szPasswd= passwd;
	m_dwCac= 0;
    m_szNombreReal= usuario;

	m_pConector->ResetState();

	m_pConector->Query("SELECT id,szReservado,descripcion FROM shared WHERE nReservado=2 AND szReservado!=\"-1\" AND nombre=\"" + usuario + "\"", codificado ? 1 : 2, this);

	return true;
}

void CDlgIngreso::OnBnClickedCancelar() {

	m_pConector->KillQuery(-1, this);
	CDialog::OnCancel();
}

void CDlgIngreso::OnBnClickedOk()
{
	CString user;
	CString contrasena;
	m_EUsuario.GetWindowText(user);
	if( user.IsEmpty()) {

		m_EUsuario.SetFocus();
		return;
	}
	m_EContrasena.GetWindowText(contrasena);
	if( contrasena.IsEmpty()) {

		m_EContrasena.SetFocus();
		return;
	}
	m_SAyuda.SetWindowText("Validando usuario");
	AfxGetMainWnd()->SendMessage(WM_SETSTATUS, 0, (LPARAM) (const char *)"Validando Usuario");
	validateUser(user, contrasena);
}

LRESULT CDlgIngreso::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if( message==WM_QUERYCONECTOR) {
		
		if( !m_bExiste) 
			m_pConector = ((CConector *)lParam); 
		else {
			if( m_pConector)
				delete m_pConector;
			m_pConector= new CConector();
			m_pConector->Cargar();
		}
		if( !m_pConector->GetDatabasesCount())
			OnCancel();
		m_pConector->SetMainFrame(NULL);
		m_EContrasena.EnableWindow();
		m_EUsuario.EnableWindow(!m_bExiste);
		m_BOK.EnableWindow();
		return 0;
	} else if( message==WM_MYSQL) {

		MySQLMessage	*message= (MySQLMessage *) lParam;
		if( wParam==QUERY_RESULT) {
			m_dwID= atol(message->GetAt(0));
			m_dwGrupo= atol(message->GetAt(1));
			m_dwCac= atol(message->GetAt(2));
			AfxGetApp()->WriteProfileString("", "LastUser", m_szNombreReal);
			AfxGetMainWnd()->SendMessage(WM_SETSTATUS, 0, (LPARAM) (const char *)"Listo");
			m_pConector->KillQuery(-1, this);
			m_pConector->CopyState();
			m_pConector->SetMainFrame(AfxGetMainWnd());
			CDialog::OnOK();
			return 0;
		} else if(wParam==QUERY_EMPTY) {

		} else if( wParam==QUERY_ERROR) {

			if( message->GetID()==2) {

				m_SAyuda.SetWindowText(message->GetAt(0));
				AfxGetMainWnd()->SendMessage(WM_SETSTATUS, 2, (LPARAM) (const char *)message->GetAt(0));
			} else {
//#ifdef _DEBUG
//				AfxGetMainWnd()->SendMessage(WM_SETSTATUS, 2, (LPARAM) (const char *)message->GetAt(0));
//#endif
			}
		} else if( wParam==THREAD_END) {
			CThreadMySQL *hilo=(CThreadMySQL *)lParam;
			if( hilo->m_nID==1) {
				validateUser(m_szNombreReal, m_szPasswd, false);
			} else {
				m_EContrasena.EnableWindow(true);
				m_EUsuario.EnableWindow(!m_bExiste);
				m_BOK.EnableWindow(true);
				m_EUsuario.SetSel(0, m_EUsuario.GetWindowTextLength());
				m_EContrasena.SetSel(0, m_EContrasena.GetWindowTextLength());
				m_bExiste ? m_EContrasena.SetFocus() : m_EUsuario.SetFocus();
			}
		}
	}
	return CDialogXP::WindowProc(message, wParam, lParam);
}

BOOL CDlgIngreso::OnInitDialog()
{
	CDialogXP::OnInitDialog();
	m_SAyuda.SetWindowText("");
	m_EContrasena.EnableWindow(false);
	m_EUsuario.EnableWindow(false);
	m_BOK.EnableWindow(false);
	AfxGetMainWnd()->SendMessage(WM_QUERYCONECTOR, 0, (LPARAM) this);
	m_EUsuario.SetWindowText(AfxGetApp()->GetProfileString("", "LastUser", ""));
	return TRUE;
}
