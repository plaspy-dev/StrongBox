// SVEServerDlg.cpp: archivo de implementación
//

#include "stdafx.h"
#include "SVEServer.h"
#include "SVEServerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cuadro de diálogo CAboutDlg utilizado para el comando Acerca de

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Datos del cuadro de diálogo
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Compatibilidad con DDX/DDV

// Implementación
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// Cuadro de diálogo de CSVEServerDlg




CSVEServerDlg::CSVEServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSVEServerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pFinger=NULL;
	m_pConector= NULL;
}

void CSVEServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EUSUARIO, m_EUsuario);
	DDX_Control(pDX, IDC_ECONTRASENA, m_EContrasena);
	DDX_Control(pDX, IDC_SESTADO, m_SEstado);
	DDX_Control(pDX, IDC_ESERVIDOR, m_EServidor);
	DDX_Control(pDX, IDC_EPUERTO, m_EPuerto);
	DDX_Control(pDX, IDC_EBDATOS, m_EBDatos);
	DDX_Control(pDX, IDC_SHUELLA, m_SHuella);
}

BEGIN_MESSAGE_MAP(CSVEServerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CSVEServerDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CSVEServerDlg::OnBnClickedCancel)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_MESSAGE(WM_MYSQL, OnMysqlMessage)
END_MESSAGE_MAP()


// Controladores de mensaje de CSVEServerDlg

BOOL CSVEServerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);
	m_pConector= new CConector();

	if(!m_pConector->Cargar()) {
		m_pConector->AddDataBase("sdgfingerca", "", "", "localhost");
	}
	m_EBDatos.SetWindowText(AfxGetApp()->GetProfileString("conector\\servidor0", "db", "sdgfingerca"));
	m_EServidor.SetWindowText(AfxGetApp()->GetProfileString("conector\\servidor0", "servidor", "localhost"));
	CString temp;
	temp.Format("%d", AfxGetApp()->GetProfileInt("conector\\servidor0", "puerto", 3306));
	m_EPuerto.SetWindowText(temp);
	m_EUsuario.SetWindowText(AfxGetApp()->GetProfileString("conector\\servidor0", "usuario", "root"));
	m_EContrasena.SetWindowText(AfxGetApp()->GetProfileString("conector\\servidor0", "passwd", ""));
	m_pConector->SetUser(AfxGetApp()->GetProfileString("conector\\servidor0", "usuario", "root"));
	m_pConector->SetPasswd(AfxGetApp()->GetProfileString("conector\\servidor0", "passwd", ""));

	m_pFinger= new CFingerPrint(this);
	m_pFinger->Init(&m_SHuella);

	SetTimer(1, 100, NULL);
	SetTimer(2, 60*1000, NULL);
	return TRUE;
}

void CSVEServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

void CSVEServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR CSVEServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSVEServerDlg::OnBnClickedOk()
{
	// OnOK();
	KillTimer(1);
	KillTimer(2);
	CWaitCursor wait;
	CString temp;
	m_EBDatos.GetWindowText(temp);
	AfxGetApp()->WriteProfileString("conector\\servidor0", "db", temp);
	m_EServidor.GetWindowText(temp);
	AfxGetApp()->WriteProfileString("conector\\servidor0", "servidor", temp);
	m_EPuerto.GetWindowText(temp);
	AfxGetApp()->WriteProfileInt("conector\\servidor0", "puerto",atoi(temp));
	m_EUsuario.GetWindowText(temp);
	AfxGetApp()->WriteProfileString("conector\\servidor0", "usuario", temp);
	m_EContrasena.GetWindowText(temp);
	AfxGetApp()->WriteProfileString("conector\\servidor0", "passwd", temp);

	if(m_pConector) {
		m_pConector->KillQuery(-1, this);
		delete m_pConector;
		m_pConector=NULL;
	}
	m_pConector= new CConector();
	m_pConector->Cargar();
	m_pConector->SetUser(AfxGetApp()->GetProfileString("conector\\servidor0", "usuario", "root"));
	m_pConector->SetPasswd(AfxGetApp()->GetProfileString("conector\\servidor0", "passwd", ""));
	m_SEstado.SetWindowText("Configuración guardada.");
	SetTimer(1, 100, NULL);
	SetTimer(2, 60*1000, NULL);
}

void CSVEServerDlg::OnBnClickedCancel()
{
#ifndef _DEBUG
	if( MessageBox("Esta Seguro de Salir de ", NULL, MB_ICONQUESTION | MB_YESNOCANCEL)==IDYES)
#endif
		OnCancel();
}

void CSVEServerDlg::OnTimer(UINT_PTR nIDEvent)
{
	if( nIDEvent==1) {
		KillTimer(1);
		m_SEstado.SetWindowText("Esperando Consultas");
		m_szCurrentID.Empty();
		if( m_pConector) {
			m_pConector->QueryBinary("SELECT huella,id FROM busqueda WHERE usuario IS NULL LIMIT 1", 1, this);
		}
	} else if(nIDEvent==2) {
		if( m_pConector)
			m_pConector->Query("DELETE FROM busqueda WHERE fecha<SUBDATE(SYSDATE(),1)", 0, this);
	}
	CDialog::OnTimer(nIDEvent);
}

void CSVEServerDlg::OnDestroy()
{
	KillTimer(1);
	KillTimer(2);
	m_pFinger->OnDestroy();
	if( m_pFinger)
		delete m_pFinger;
	if(m_pConector) {
		m_pConector->KillQuery(-1, this);
		delete m_pConector;
		m_pConector=NULL;
	}
	CDialog::OnDestroy();
}

LRESULT CSVEServerDlg::OnMysqlMessage(WPARAM wParam, LPARAM lParam) {

	MySQLMessage *mess=(MySQLMessage *)lParam;
	MySQLMessageB *messb=(MySQLMessageB *)lParam;
	CThreadMySQL *hilo= (CThreadMySQL *) lParam;

	switch( wParam) {
		case THREAD_END:
			if( hilo->m_nID==1) {
				if( m_szCurrentID.IsEmpty()) {
					SetTimer(1, 100, NULL);
				}
			} else if( hilo->m_nID==100) {
				if( !m_szCurrentID.IsEmpty()) {
					CString resultado;
					resultado.Format("Huella no encontrada en %g segundos, id=" + m_szCurrentID,(double)(GetTickCount()-m_pTemporal)/1000);
					m_SEstado.SetWindowText(resultado);
					if(m_pConector)
						m_pConector->Query("UPDATE busqueda SET usuario=-1 WHERE id=" + m_szCurrentID, 0, this);
				}
				m_szCurrentID.Empty();
				SetTimer(1, 100, NULL);
			}
			break;
		case QUERY_EMPTY:
			break;
		case QUERY_RESULTB:
			if( messb->GetID()==1) {
				m_szCurrentID=messb->GetAt(0);
				m_pFinger->SetTemplate(messb->GetBinaryData());
				m_pFinger->Prepare();
				m_pTemporal= GetTickCount();
				m_SEstado.SetWindowText("Realizando consulta");
				if(m_pConector)
					m_pConector->QueryBinary("SELECT huella,documento,dedo FROM template", 100, this);
			} else if(messb->GetID()==100) {
				int score=0;
				if(m_pFinger->Compare((const char *)messb->GetBinaryData()->GetData(), &score)) {
					CString resultado;
					resultado.Format("Huella encontrada en %g segundos, usuario=" + messb->GetAt(0),(double)(GetTickCount()-m_pTemporal)/1000);
					if( m_pConector)
						m_pConector->KillQuery(100, this);
					m_SEstado.SetWindowText(resultado);
					resultado= "UPDATE busqueda SET usuario=" + messb->GetAt(0);
					// registro de busqueda
					if(m_pConector) {
						CString key;
						CStringArray valores;
						if(m_pConector->Find("SELECT id FROM sucesos WHERE usuario='" + messb->GetAt(0) +"' AND salida IS NULL", this, &valores)>0 && !valores.IsEmpty()) {
							key= valores.GetAt(0);
							if( !key.IsEmpty()) {
								resultado+=",suceso="+ key;
							} 
						}
					}
					resultado+= " WHERE id=" + m_szCurrentID;
					if(m_pConector)
						m_pConector->Query(resultado, 0, this);
					m_szCurrentID.Empty();
				}
			}
			break;
		case QUERY_RESULT:
			break;
		case QUERY_ERROR:
			MessageBox(mess->GetAt(1), mess->GetAt(0));
			break;
	}
	return 0;
}