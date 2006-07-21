// sveclientDlg.cpp: archivo de implementación
//

#include "stdafx.h"
#include <Conector/Query.h>
#include "sveclient.h"
#include "sveclientDlg.h"
# include "../componentes/newmenu/newmenu.h"
#include <Componentes/SplashScreenEx.h>
# include "../sve/MyTime.h"
# include "trialinfo.h"
# include "resource.h"
# include "../Componentes/EditFloat.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


# define RETRASO_MENSAJE		30000
# define CLEAN_OLD				10*60*1000
# define SERVIDOR_REINTENTOS	600

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


// Cuadro de diálogo de CsveclientDlg




CsveclientDlg::CsveclientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CsveclientDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pFinger=NULL;
	m_pConector= NULL;
	m_bAutonomo=true;
	m_nReintentos=0;
	m_bSalida=false;
	m_nEstado=0;
}

void CsveclientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EUSUARIO, m_EUsuario);
	DDX_Control(pDX, IDC_ECONTRASENA, m_EContrasena);
	DDX_Control(pDX, IDC_SESTADO, m_SEstado);
	DDX_Control(pDX, IDC_ESERVIDOR, m_EServidor);
	DDX_Control(pDX, IDC_EPUERTO, m_EPuerto);
	DDX_Control(pDX, IDC_EBDATOS, m_EBDatos);
	DDX_Control(pDX, IDC_SHUELLA, m_SHuella);
	DDX_Control(pDX, IDC_CAUTONOMO, m_BAutonomo);
	DDX_Control(pDX, IDC_SMATRIZ, m_SMatriz);
}

BEGIN_MESSAGE_MAP(CsveclientDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDCANCEL, &CsveclientDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CsveclientDlg::OnBnClickedOk)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_MESSAGE(WM_FINGERPRINT, OnFingerMessage)
	ON_MESSAGE(WM_MYSQL, OnMysqlMessage)
	ON_BN_CLICKED(IDC_CAUTONOMO, &CsveclientDlg::OnBnClickedCautonomo)
	ON_REGISTERED_MESSAGE(CakTrayIcon::WM_TRAYICONNOTIFY, OnTrayIconNotify)
	ON_COMMAND(2000,OnConfigurar)
	ON_COMMAND(2001,OnAcercaDe)
	ON_COMMAND(2002,OnSalir)
	ON_COMMAND(2003,OnCancel)
	ON_WM_CHAR()
END_MESSAGE_MAP()


// Controladores de mensaje de CsveclientDlg

BOOL CsveclientDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Agregar el elemento de menú "Acerca de..." al menú del sistema.

	// IDM_ABOUTBOX debe estar en el intervalo de comandos del sistema.
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

	// Establecer el icono para este cuadro de diálogo. El marco de trabajo realiza esta operación
	//  automáticamente cuando la ventana principal de la aplicación no es un cuadro de diálogo
	SetIcon(m_hIcon, TRUE);			// Establecer icono grande
	SetIcon(m_hIcon, FALSE);		// Establecer icono pequeño

	CSplashScreenEx *m_pSplash=new CSplashScreenEx();

	m_pSplash->Create(this,"SDGFingerSB splash",0,CSS_FADE | CSS_CENTERSCREEN | CSS_SHADOW);
	m_pSplash->SetBitmap(IDB_ACERCADE,255,0,255);
	m_pSplash->SetTextFont("Arial",100,CSS_TEXT_NORMAL);
	m_pSplash->SetTextRect(CRect(100,260,500,310));
	m_pSplash->SetTextColor(RGB(0,0,0));
	m_pSplash->SetTextFormat(DT_RIGHT | DT_BOTTOM);
	m_pSplash->Show();
	CString szTemp= "Versión 1.0 Trial\r\n";
	m_pSplash->SetText(szTemp);

	m_TrayIcon.Create(this, 1001);
	m_TrayIcon.SetIconAndTip(LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME)), _T("SDGFinger Cliente"));

	m_SMatriz.SetNumberOfLines(2);
	m_SMatriz.SetXCharsPerLine(MATRIZ_ANCHO);
	m_SMatriz.SetSize(CMatrixStatic::SMALL);
	m_SMatriz.AdjustClientXToSize(MATRIZ_ANCHO);
	m_SMatriz.AdjustClientYToSize(2);

	m_pDlgPopUp.Create(CDlgPopUp::IDD, GetDesktopWindow());

	m_pSplash->SetText("Iniciando El Panel LCD.");
	
	if (AfxGetApp()->GetProfileInt("control", "lcd", 0))
		m_pDisplay.Run();
	
	SetMatrizTexto("Listo.", 0);

	m_pConector= new CConector();

	if(!m_pConector->Cargar()) {
		m_pConector->AddDataBase("sdgfingersb", "", "", "localhost");
	}
	m_EBDatos.SetWindowText(AfxGetApp()->GetProfileString("conector\\servidor0", "db", "sdgfingersb"));
	m_EServidor.SetWindowText(AfxGetApp()->GetProfileString("conector\\servidor0", "servidor", "localhost"));
	CString temp;
	temp.Format("%d", AfxGetApp()->GetProfileInt("conector\\servidor0", "puerto", 3306));
	m_EPuerto.SetWindowText(temp);
	m_EUsuario.SetWindowText(AfxGetApp()->GetProfileString("conector\\servidor0", "usuario", "root"));
	m_EContrasena.SetWindowText(AfxGetApp()->GetProfileString("conector\\servidor0", "passwd", ""));
	m_pConector->SetUser(AfxGetApp()->GetProfileString("conector\\servidor0", "usuario", "root"));
	m_pConector->SetPasswd(AfxGetApp()->GetProfileString("conector\\servidor0", "passwd", ""));

	m_BAutonomo.SetCheck(AfxGetApp()->GetProfileInt("control", "autonomo", 1));
	m_bAutonomo= m_BAutonomo.GetCheck();

	m_pSplash->SetText("Iniciando dispositivo de huellas");
	
	m_pFinger= new CFingerPrint(this);
	m_SHuella.HideInfo();
	m_pFinger->Init(&m_SHuella);

	m_pSplash->Hide();

	temp=AfxGetApp()->m_lpCmdLine;
	if( temp.Find("-start")!=-1)
		PostMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
	SetTimer(4, 30*60*1000, NULL);
	SetTimer(5, 1000, NULL);

	return TRUE;
}

void CsveclientDlg::SetMatrizTexto(CString texto, int estilo, CString linea2) {
	
	KillTimer(2);
	m_nEstado=estilo;

	switch(estilo) {
		case 2: // rojo
			m_SMatriz.SetDisplayColors(RGB(0, 0, 0), RGB(255, 60, 0), RGB(103, 30, 0));
			KillTimer(5);
			SetTimer(2, RETRASO_MENSAJE, NULL);
			break;
		case 5:
		case 4:
		case 3:
		case 1: // verde
			m_SMatriz.SetDisplayColors(RGB(0, 0, 0), RGB(0, 255, 50), RGB(0, 103, 30));
			KillTimer(5);
			SetTimer(2, RETRASO_MENSAJE, NULL);
			break;
		default: // azul
			m_SMatriz.SetDisplayColors(RGB(0, 0, 0), RGB(63, 181, 255), RGB(23, 64,  103));
			break;
	}
	CString lcd1=texto;
	CString lcd2=linea2;
	if(texto.GetLength()<MATRIZ_ANCHO) {
		for( int i= texto.GetLength(); i<MATRIZ_ANCHO; i++)
			texto+=" ";
	} else if(texto.GetLength()>MATRIZ_ANCHO) {
		texto= texto.Left(MATRIZ_ANCHO);
	}
	if(linea2.GetLength()<MATRIZ_ANCHO) {
		for( int i= linea2.GetLength(); i<MATRIZ_ANCHO; i++)
			linea2+=" ";
	} else if(linea2.GetLength()>MATRIZ_ANCHO) {
		linea2= linea2.Left(MATRIZ_ANCHO);
	}
	m_SMatriz.SetText(texto+linea2);
	m_pDisplay.SetMessage(lcd1,lcd2);
	if( estilo && !::IsWindowVisible(*this))
		m_pDlgPopUp.Show(texto+linea2, estilo);
}

void CsveclientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		OnAcercaDe();
	}
	else
	{
		if( nID==SC_MINIMIZE) {
			ShowWindow(SW_HIDE);
			m_TrayIcon.Show();
		} else if( nID==SC_CLOSE) {
			OnBnClickedCancel();
		} else CDialog::OnSysCommand(nID, lParam);
	}
}

void CsveclientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // Contexto de dispositivo para dibujo

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Centrar icono en el rectángulo de cliente
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Dibujar el icono
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// El sistema llama a esta función para obtener el cursor que se muestra mientras el usuario arrastra
//  la ventana minimizada.
HCURSOR CsveclientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CsveclientDlg::OnBnClickedCancel()
{
#ifndef _DEBUG
	if( MessageBox("Esta Seguro de Salir de SDGFinger", NULL, MB_ICONQUESTION | MB_YESNOCANCEL)==IDYES)
#endif
		OnCancel();
}

void CsveclientDlg::Reset() {

	KillTimer(3);
	m_szLastSearch.Empty();
	KillTimer(2);
	SetTimer(5, 1000, NULL);
	CStrongBoxMyTime tiempo(true);
	CString temp= tiempo.GetFechaHora();
	temp.Replace(":", ";");
	SetMatrizTexto("Listo.", 0, temp);
}

void CsveclientDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	// OnOK();
	if(m_nEstado==1) {	
		if(!m_szTexto.IsEmpty()) {
			SetMatrizTexto("Esta seguro", 3, "$" +CEditFloat::GetFormatNumber(m_szTexto));
		}
		return;
	} else if(m_nEstado==3) {
		// agregar la cantidad 
		// m_szTexto
		CStringArray valores,campos;
		campos.Add("usuario");
		valores.Add(m_szLastSearch);
		campos.Add("fecha");
		valores.Add("SYSDATE()");
		campos.Add("dinero");
		valores.Add(m_szTexto);
		m_pConector->Query(CQuery::CreateInsert("sucesos", campos, valores), 0, this);
		Reset();
		return;
	} else if(m_nEstado==4) {
		SetMatrizTexto("Esta seguro", 5,"$" +CEditFloat::GetFormatNumber(m_szTexto));
	} else if(m_nEstado==5) {
		m_pConector->Find("UPDATE sucesos SET salida=SYSDATE(),responsable='"+m_szLastSearch + "' WHERE salida IS NULL;", this,NULL);
		Reset();
	}
	KillTimer(1);

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
	AfxGetApp()->WriteProfileInt( "control", "autonomo", m_BAutonomo.GetCheck());

	if(m_pConector) {

		m_pConector->KillQuery(-1, this);
		delete m_pConector;
		m_pConector=NULL;
	}
	m_bAutonomo= m_BAutonomo.GetCheck();
	m_pConector= new CConector();
	m_pConector->Cargar();

	m_pConector->SetUser(AfxGetApp()->GetProfileString("conector\\servidor0", "usuario", "root"));
	m_pConector->SetPasswd(AfxGetApp()->GetProfileString("conector\\servidor0", "passwd", ""));
	m_SEstado.SetWindowText("Configuración guardada.");
}

void CsveclientDlg::OnDestroy()
{
	KillTimer(1);
	KillTimer(4);
	KillTimer(5);

	m_TrayIcon.Hide();

	m_pFinger->OnDestroy();
	if( m_pFinger)
		delete m_pFinger;
	if(m_pConector) {
		m_pConector->KillQuery(-1, this);
		delete m_pConector;
	}
	CDialog::OnDestroy();
}

void CsveclientDlg::OnTimer(UINT_PTR nIDEvent)
{
	if( nIDEvent==1) {
		KillTimer(1);
		m_nReintentos++;
		m_SEstado.SetWindowText("Buscando Usuario");
		if(m_pConector)
			m_pConector->Query("SELECT usuario,suceso FROM busqueda WHERE id=" + m_szCurrentID + " AND usuario IS NOT NULL", 1, this);
	} else if(nIDEvent==3) {
		Reset();
	} else if(nIDEvent==4) {
#ifndef _DEBUG
		/*CStrongBoxTrialInfo info;
		if( info.GetLeftDays()<=0) {
			CWaitCursor wait;
			SetMatrizTexto("Su periodo de prueba.", 2, "Ha terminado");
			Sleep(5000);
			OnCancel();
		}*/
#endif
	} else if(nIDEvent==5 || nIDEvent==2) {
		if( nIDEvent==2) {
			KillTimer(2);
			SetTimer(5, 1000, NULL);
		}
		CStrongBoxMyTime tiempo(true);
		CString temp= tiempo.GetFechaHora();
		temp.Replace(":", ";");
		SetMatrizTexto("Listo.", 0, temp);
	}
	CDialog::OnTimer(nIDEvent);
}

LRESULT CsveclientDlg::OnMysqlMessage(WPARAM wParam, LPARAM lParam) {

	MySQLMessage *mess=(MySQLMessage *)lParam;
	MySQLMessageB *messb=(MySQLMessageB *)lParam;
	CThreadMySQL *hilo= (CThreadMySQL *) lParam;

	switch( wParam) {
		case THREAD_END:
			if( hilo->m_nID==1 && !m_szCurrentID.IsEmpty()) {
				if(m_nReintentos==SERVIDOR_REINTENTOS) {
					m_szCurrentID.Empty();
					SetMatrizTexto("No se ha podido conectar", 2, "con el servidor");
				} else {
					SetTimer(1, 100, NULL);
				}
			} else if( hilo->m_nID==100) {
				if( m_szCurrentID==" " || m_szCurrentID.IsEmpty()) {
					CString resultado;
					resultado.Format("Huella no encontrada en %g segundos",(double)(GetTickCount()-m_pTemporal)/1000);
					m_SEstado.SetWindowText(resultado);
					SetMatrizTexto("Usuario No Encontrado", 2);
				}
				m_szCurrentID.Empty();
			}
			break;
		case QUERY_EMPTY:
			break;
		case QUERY_RESULTB:
			if(messb->GetID()==100) {
				int score=0;
				if(m_pFinger->Compare((const char *)messb->GetBinaryData()->GetData(), &score)) { // autonomo

					CWaitCursor wait;
					CString resultado;
					m_szCurrentID=messb->GetAt(0);
					resultado.Format("Huella encontrada en %g segundos, documento=%s",(double)(GetTickCount()-m_pTemporal)/1000, m_szCurrentID);
					if(m_pConector)
						m_pConector->KillQuery(100, this);
					m_SEstado.SetWindowText(resultado);
					if(m_pConector) {
						
						CString grupo;
						CStringArray valores;
						if(m_pConector->Find("SELECT CONCAT(usuario.nombre,' ',usuario.apellido),shared.szReservado FROM usuario LEFT JOIN shared ON usuario.grupo=shared.id WHERE usuario.documento='" + messb->GetAt(0) +"'", this, &valores)>0 && !valores.IsEmpty()) {
							m_szLastUsuario= valores.GetAt(0);
							grupo= valores.GetAt(1);
						}
						if(grupo=="1") {
							m_szTexto.Empty();
							SetMatrizTexto(m_szLastUsuario,1,"Ingrese la suma.");
						} else if(grupo=="2") {
							m_szTexto.Empty();
							CStringArray valores;
							if(m_pConector->Find("SELECT SUM(dinero) FROM sucesos WHERE salida IS NULL;", this, &valores)!=-1 && !valores.IsEmpty()) {
								m_szTexto= valores.GetAt(0);
							}
							SetMatrizTexto("Suma actual", 4, "$" + CEditFloat::GetFormatNumber(m_szTexto));
						}
					}
					m_szLastSearch= m_szCurrentID;
					KillTimer(3);
					SetTimer(3, CLEAN_OLD, NULL);
				}
			}
			break;
		case QUERY_RESULT:
			if( mess->GetID()==1) {
				if( mess->GetAt(0)=="-1") { 
					m_SEstado.SetWindowText("Usuario No encontrado");
					SetMatrizTexto("Usuario No Encontrado", 2);
				} else {
					CWaitCursor wait;
					CString resultado;
					m_SEstado.SetWindowText("Documento= " + mess->GetAt(0));
					if( m_pConector && m_szLastSearch!=mess->GetAt(0)) {
						
						CStringArray valores;
						CString key= mess->GetAt(1);
						if( key.IsEmpty()) {

							valores.Add("");
							// usuario
							valores.Add(mess->GetAt(0));
							// entrada 
							valores.Add("SYSDATE()");
							// salida 
							valores.Add("");
							m_pConector->Query(CQuery::CreateInsert("sucesos", valores), 0, this);
							if(m_pConector->Find("SELECT CONCAT(nombre,' ',apellido),SYSDATE() FROM usuario WHERE documento=" + mess->GetAt(0), this, &valores) && !valores.IsEmpty()) {
								CStrongBoxMyTime tiempo(valores.GetAt(1));
								m_szLastUsuario= valores.GetAt(0);
								m_szLastHora= tiempo.GetHora();
								m_szLastHora.Replace(":", ";");
								SetMatrizTexto("Bienvenido (" + m_szLastHora + ")", 1, valores.GetAt(0));
								m_bSalida=false;
							}
						} else {
							// salida
							CStringArray campos;
							campos.Add("salida");
							valores.Add("SYSDATE()");
							m_pConector->Query(CQuery::CreateUpdate("sucesos", campos,valores, "id="+ key), 0, this);
							if(m_pConector->Find("SELECT CONCAT(nombre,' ',apellido),SYSDATE() FROM usuario WHERE documento=" + mess->GetAt(0), this, &valores) && !valores.IsEmpty()) {
								CStrongBoxMyTime tiempo(valores.GetAt(1));
								m_szLastUsuario= valores.GetAt(0);
								m_szLastHora= tiempo.GetHora();
								m_szLastHora.Replace(":", ";");
								SetMatrizTexto("Hasta luego (" + m_szLastHora + ")", 1, valores.GetAt(0));
								m_bSalida=true;
							}
						}
					} else {
						CString linea1, linea2;
						linea2= m_szLastUsuario;
						if( m_bSalida) {
							linea1= "Hasta Luego (" + m_szLastHora + ")";
						} else {
							linea1= "Bienvenido (" + m_szLastHora + ")";
						}
						SetMatrizTexto(linea1, 1, linea2);
					}
					m_szLastSearch= mess->GetAt(0);
					m_szCurrentID.Empty();
					KillTimer(3);
					SetTimer(3, CLEAN_OLD, NULL);
				}
				m_szCurrentID.Empty();
			}
			break;
		case QUERY_ERROR:
			MessageBox(mess->GetAt(1), mess->GetAt(0));
			break;
	}
	return 0;
}

LRESULT CsveclientDlg::OnFingerMessage(WPARAM wParam, LPARAM lParam) {

	if(!m_szCurrentID.IsEmpty() || !m_SHuella.HasHuella())
		return 0;
	m_szCurrentID=" ";
	SetMatrizTexto("Buscando ...", 0);
	m_SEstado.SetWindowText("Buscando");

	if(m_bAutonomo) {

		m_pFinger->Prepare();
		if(m_pConector)
			m_pConector->QueryBinary("SELECT huella,documento,dedo FROM template", 100, this);
		m_pTemporal= GetTickCount();
	} else {
		CStringArray valores;
		// id
		valores.Add("");
		// huella
		valores.Add("%0%");
		// usuario=NULL
		valores.Add("");
		// suceso=NULL
		valores.Add("");
		// fecha
		valores.Add("SYSDATE()");

		CString app;
		BYTE *buffer= m_SHuella.m_cTemplate;
		for( int i=0; i<m_SHuella.m_nTemplate; i++) {
			switch( (buffer)[i]) {
				case 0:
					app+="\\0";
					break;
				case '\\':
					app+="\\\\";
					break;
				case '\"':
					app+="\\\"";
					break;
				default:
					app+= (buffer)[i];
					break;
			}
		}
		CString query= CQuery::CreateInsert("busqueda", valores);
		query.Replace("%0%", app);
		if(m_pConector && m_pConector->Find(query, this, &valores)!=-1) {
			m_szCurrentID=valores.GetAt(0);
			m_nReintentos=0;
			SetTimer(1, 100, NULL);
		} else m_szCurrentID.Empty();
	}
	return 0;
}
void CsveclientDlg::OnBnClickedCautonomo()
{
	
}
LRESULT CsveclientDlg::OnTrayIconNotify(WPARAM wParam, LPARAM lParam)
{
	if (1001 == wParam)
    {
        if (WM_LBUTTONDBLCLK == lParam)
        {
			OnConfigurar();
        } else if (WM_RBUTTONUP == lParam)
        {
            CPoint pt;
            GetCursorPos(&pt);
			CNewMenu menu;
			menu.CreatePopupMenu();
			menu.AppendMenu(MF_STRING, 2000, "&Configurar");
			menu.AppendMenu(MF_STRING, 2001, "&Acerca de SDGFinger");
			menu.AppendMenu(MF_STRING, 2002, "&Salir");
			menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, pt.x, pt.y, this);
            PostMessage(WM_NULL);
		}
    }
    return 0;
}

void CsveclientDlg::OnConfigurar() {
	ShowWindow(SW_SHOW);
	SetForegroundWindow();
}

void CsveclientDlg::OnSalir() {

	OnBnClickedCancel();
}

void CsveclientDlg::OnAcercaDe() {
	
	/*CAboutDlg dlgAbout;
	dlgAbout.DoModal();*/
	CSplashScreenEx *m_pSplash=new CSplashScreenEx();

	m_pSplash->Create(NULL,"SDGFingerSB Aboutbox",0,CSS_FADE | CSS_CENTERSCREEN | CSS_SHADOW | CSS_HIDEONCLICK);
	m_pSplash->SetBitmap(IDB_ACERCADE,255,0,255);
	m_pSplash->SetTextFont("Arial",100,CSS_TEXT_NORMAL);
	m_pSplash->SetTextRect(CRect(100,260,500,310));
	m_pSplash->SetTextColor(RGB(0,0,0));
	m_pSplash->SetTextFormat(DT_RIGHT | DT_BOTTOM);
	m_pSplash->Show();
	CString szTemp= "Versión 1.0 Trial\r\n";
	m_pSplash->SetText(szTemp);
}
void CsveclientDlg::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CDialog::OnChar(nChar, nRepCnt, nFlags);
}

BOOL CsveclientDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message==WM_CHAR) {
		char letra=(char)pMsg->wParam; 
		if(m_nEstado==1 || m_nEstado==3) {
			if(letra>='0' && letra<='9') {
				if(m_szTexto.GetLength()<(REAL_LCD-4)) {
					m_szTexto+=letra;
					SetMatrizTexto(m_szLastUsuario, 1,"$" +CEditFloat::GetFormatNumber(m_szTexto));
				}
			} else if(letra==VK_DELETE || letra=='.' || letra==',') {
				if(!m_szTexto.IsEmpty()) {
					m_szTexto.Delete(m_szTexto.GetLength()-1);
					SetMatrizTexto(m_szLastUsuario, 1,m_szTexto.IsEmpty() ? "Ingrese la suma." : "$" +CEditFloat::GetFormatNumber(m_szTexto));
				} else {
					Reset();
				}
			}
		} else if(m_nEstado==4 || m_nEstado==5) {
			if(letra==VK_DELETE || letra=='.' || letra==',') {
				Reset();
			}
		}
		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}
