// SVEView.cpp: implementación de la clase CSVEView
//

#include "stdafx.h"
#include <Conector/Query.h>
#include "SVE.h"

#include "SVEDoc.h"
#include "SVEView.h"
# include "mytime.h"
# include "../iofile/xzip.h"
# include "../componentes/componentes.h"
# include "../componentes/draw.h"
# include "DlgConfirmar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CSVEView, CFormView)

BEGIN_MESSAGE_MAP(CSVEView, CFormView)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
	ON_EN_KILLFOCUS(IDC_ECEDULA, OnEnKillfocusEcedula)
	ON_WM_DESTROY()
	ON_MESSAGE(WM_MYSQL, OnMysqlMessage)
	ON_MESSAGE(WM_INSERTODS,OnInsertODS)
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_PREVIEW, OnUpdatePrint)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_DIRECT, OnUpdatePrint)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT, OnUpdatePrint)
	ON_MESSAGE(WM_FINGERPRINT, OnFingerMessage)
	ON_STN_CLICKED(IDC_SPULGARD, &CSVEView::OnStnClickedSpulgard)
	ON_STN_CLICKED(IDC_SPULGARI, &CSVEView::OnStnClickedSpulgari)
	ON_STN_CLICKED(IDC_SINDICED, &CSVEView::OnStnClickedSindiced)
	ON_STN_CLICKED(IDC_SINDICEI, &CSVEView::OnStnClickedSindicei)
	ON_STN_CLICKED(IDC_SCORAZOND, &CSVEView::OnStnClickedScorazond)
	ON_STN_CLICKED(IDC_SCORAZONI, &CSVEView::OnStnClickedScorazoni)
	ON_STN_CLICKED(IDC_SANULARD, &CSVEView::OnStnClickedSanulard)
	ON_STN_CLICKED(IDC_SANULARI, &CSVEView::OnStnClickedSanulari)
	ON_STN_CLICKED(IDC_SMENIQUED, &CSVEView::OnStnClickedSmeniqued)
	ON_STN_CLICKED(IDC_SMENIQUEI, &CSVEView::OnStnClickedSmeniquei)
	ON_STN_CLICKED(IDC_SHUELLA, &CSVEView::OnStnClickedShuella)
END_MESSAGE_MAP()

// Construcción o destrucción de CSVEView

CSVEView::CSVEView()
	: CFormView(CSVEView::IDD)
{
	m_pPrint=NULL;
	m_pSplash=NULL;
	m_bExiste=false;
	m_pFinger=NULL;
}

CSVEView::~CSVEView()
{
	if( m_pFinger)
		delete m_pFinger;
	if( m_pPrint) 
		delete m_pPrint;
}

void CSVEView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ECEDULA, m_EDocumento);
	DDX_Control(pDX, IDC_ENOMBRE, m_ENombre);
	DDX_Control(pDX, IDC_EAPELLIDO, m_EApellido);
	DDX_Control(pDX, IDOK, m_BAceptar);
	DDX_Control(pDX, IDC_BUTTON3, m_BCancelar);
	DDX_Control(pDX, IDC_SHUELLA, m_SHuella);
	DDX_Control(pDX, IDC_SLOGO, m_SLogo);
	DDX_Control(pDX, IDC_SMENSAJEI, m_SMensaje);
	DDX_Control(pDX, IDC_SPULGARD, m_SPulgarD);
	DDX_Control(pDX, IDC_SPULGARI, m_SPulgarI);
	DDX_Control(pDX, IDC_SINDICED, m_SIndiceD);
	DDX_Control(pDX, IDC_SINDICEI, m_SIndiceI);
	DDX_Control(pDX, IDC_SCORAZOND, m_SCorazonD);
	DDX_Control(pDX, IDC_SCORAZONI, m_SCorazonI);
	DDX_Control(pDX, IDC_SANULARD, m_SAnularD);
	DDX_Control(pDX, IDC_SANULARI, m_SAnularI);
	DDX_Control(pDX, IDC_SMENIQUED, m_SMeniqueD);
	DDX_Control(pDX, IDC_SMENIQUEI, m_SMeniqueI);
	DDX_Control(pDX, IDC_EOBSERVACIONES, m_EObservaciones);
	DDX_Control(pDX, IDC_CHORARIO, m_CBHorario);
}

BOOL CSVEView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CFormView::PreCreateWindow(cs);
}

void CSVEView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	
	m_pFinger= new CFingerPrint(this);

	GetDocument()->SetTitle("Ingreso");

	CWaitCursor wait;

	m_pConector= (CConector *) AfxGetMainWnd()->SendMessage(WM_QUERYCONECTOR, 0, (LPARAM)this);
	m_pUsuario= (CUsuario *) AfxGetMainWnd()->SendMessage(WM_QUERYUSUARIO, 0, (LPARAM)this);

	m_SLogo.SetBitmap(IDB_BLOGO);

	m_pSplash=new CSplashScreenEx();

	m_pSplash->Create(this,"SDGFingerSB splash",0,CSS_FADE | CSS_CENTERSCREEN | CSS_SHADOW);
	m_pSplash->SetBitmap(IDB_ACERCADE,255,0,255);
	m_pSplash->SetTextFont("Arial",100,CSS_TEXT_NORMAL);
	m_pSplash->SetTextRect(CRect(100,260,500,310));
	m_pSplash->SetTextColor(RGB(0,0,0));
	m_pSplash->SetTextFormat(DT_RIGHT | DT_BOTTOM);
	m_pSplash->Show();
	CString szTemp= "Versión 1.0 Trial\r\n";
	m_pSplash->SetText(szTemp);

	Clean(true);

	SetError(NULL, "Inicializando Dispositivo de Huellas");
	if(m_pSplash) {
		CString szTemp= "Versión 1.0 Trial\r\nIniciando Dispositivo de Huellas";
		m_pSplash->SetText(szTemp);
	}
	m_SHuella.HideInfo();
	m_SPulgarD.HideInfo();
	m_SPulgarI.HideInfo();
	m_SIndiceD.HideInfo();
	m_SIndiceI.HideInfo();
	m_SCorazonD.HideInfo();
	m_SCorazonI.HideInfo();
	m_SAnularI.HideInfo();
	m_SAnularD.HideInfo();
	m_SMeniqueI.HideInfo();
	m_SMeniqueD.HideInfo();

	m_SPulgarD.SetMessage("Pulgar");
	m_SPulgarI.SetMessage("Pulgar");
	m_SIndiceD.SetMessage("Indice");
	m_SIndiceI.SetMessage("Indice");
	m_SCorazonD.SetMessage("Corazón");
	m_SCorazonI.SetMessage("Corazón");
	m_SAnularD.SetMessage("Anular");
	m_SAnularI.SetMessage("Anular");
	m_SMeniqueD.SetMessage("Meñique");
	m_SMeniqueI.SetMessage("Meñique");
	m_SHuella.SetMessage("Buscar");

	m_SHuella.Select();
	m_pFinger->Init(&m_SHuella);

	m_pSplash->Hide();
	m_pSplash=NULL;

	m_pConector->Query("SELECT id,nombre FROM shared WHERE nReservado=5 AND (szReservado IS NULL OR szReservado!=-1)",115, this);
}


// Diagnósticos de CSVEView

#ifdef _DEBUG
void CSVEView::AssertValid() const
{
	CFormView::AssertValid();
}

void CSVEView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CSVEDoc* CSVEView::GetDocument() const // La versión de no depuración es en línea
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSVEDoc)));
	return (CSVEDoc*)m_pDocument;
}
#endif //_DEBUG


// Controladores de mensaje de CSVEView

void CSVEView::OnBnClickedOk()
{
	CWaitCursor wait;

	CStringArray valores, campos;
	CString temp,archivo;
	CString codigo;
	CString documento;
	
	campos.Add("documento");
	campos.Add("nombre");
	campos.Add("apellido");
	campos.Add("grupo");
	campos.Add("observaciones");

	m_EDocumento.GetWindowText(documento);
	if( documento.IsEmpty()) {
		SetErrorEx(&m_EDocumento, "Por favor ingrese un documento", 2, "ingreso");
		return;
	}
	// documento
	valores.Add(documento);
	m_ENombre.GetWindowText(temp);
	if( temp.IsEmpty()) {
		SetErrorEx(&m_ENombre, "Por favor ingrese un nombre", 2, "ingreso");
		return;
	}
	valores.Add(temp);
	m_EApellido.GetWindowText(temp);
	if( temp.IsEmpty()) {
		SetErrorEx(&m_EApellido, "Por favor ingrese un apellido", 2, "ingreso");
		return;
	}
	valores.Add(temp);
	// grupo

	temp.Empty();
	int pos= m_CBHorario.GetCurSelEx();
	if(pos==-1) {
		SetErrorEx(&m_CBHorario, "Por favor seleccione un grupo.", 2, "ingreso");
		return;
	}
	temp.Format("%d", m_CBHorario.GetItemData(pos));
	valores.Add(temp);

	// observaciones
	m_EObservaciones.GetWindowText(temp);
	valores.Add(temp);

	int cantidad=0;
	if(m_SPulgarD.HasHuella())
		cantidad++;
	if(m_SPulgarI.HasHuella())
		cantidad++;
	if(m_SIndiceD.HasHuella())
		cantidad++;
	if(m_SIndiceI.HasHuella())
		cantidad++;
	if(m_SCorazonD.HasHuella())
		cantidad++;
	if(m_SCorazonI.HasHuella())
		cantidad++;
	if(m_SAnularD.HasHuella())
		cantidad++;
	if(m_SAnularI.HasHuella())
		cantidad++;
	if(m_SMeniqueD.HasHuella())
		cantidad++;
	if(m_SMeniqueI.HasHuella())
		cantidad++;
	if( cantidad<2) {
		SetErrorEx(NULL, "Por favor coloque dos huellas", 2, "IngresoH");
		SelectBitmap(&m_SIndiceD);
		return;
	}
	// usuario
	m_pConector->Query(m_bExiste ? CQuery::CreateUpdate("usuario", campos, valores, "documento=" + documento) 
		: CQuery::CreateInsert("usuario", campos, valores), 0, this);
	// propiedades
	codigo= documento;
	valores.RemoveAll();
	valores.Add(codigo);
	CString usuario;
	usuario.Format("%d", m_pUsuario->GetID());
	valores.Add(usuario);
	valores.Add("SYSDATE()");

	m_pConector->Query(CQuery::CreateInsert("propiedades", valores), 0, this);

	valores.RemoveAll();
	
	m_pConector->Query("DELETE FROM huella WHERE documento='" + documento+ "'", 0 , this);
	m_pConector->Query("DELETE FROM template WHERE documento='" + documento+ "'", 0 , this);


	InsertarHuella(documento, &m_SPulgarD, "0");
	InsertarHuella(documento, &m_SPulgarI, "1");
	InsertarHuella(documento, &m_SIndiceD, "2");
	InsertarHuella(documento, &m_SIndiceI, "3");
	InsertarHuella(documento, &m_SCorazonD, "4");
	InsertarHuella(documento, &m_SCorazonI, "5");
	InsertarHuella(documento, &m_SAnularD, "6");
	InsertarHuella(documento, &m_SAnularI, "7");
	InsertarHuella(documento, &m_SMeniqueD, "8");
	InsertarHuella(documento, &m_SMeniqueI, "9");

	m_szOld="";
	/*if(m_pActual && !m_pActual->GetID().IsEmpty()) 
		m_pActual=NULL;*/
	OnBnClickedButton3();
}

int CSVEView::InsertarHuella(CString documento, CFingerBitmap *bitmap, CString dedo) {

	if(!bitmap->HasHuella())
		return 0;

	CStringArray valores;
	CString temp;

	valores.RemoveAll();
	valores.Add(documento);
	// dedo
	valores.Add(dedo);
	// ancho
	temp.Format("%d", bitmap->m_nAncho);
	valores.Add(temp);
	// alto
	temp.Format("%d", bitmap->m_nAlto);
	valores.Add(temp);
	// resolucion
	temp.Format("%d", bitmap->m_nResolucion);
	valores.Add(temp);
	// huella
	valores.Add("%0%");
	CString app;
	BYTE *buffer= bitmap->m_cRaw;
	for( int i=0; i< bitmap->GetSize(); i++) {
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
	CString query= CQuery::CreateInsert("huella", valores);
	if(AfxGetApp()->GetProfileInt("control", "compress", 1))
		query.Replace("\"%0%\"", "COMPRESS(\"%0%\")");
	query.Replace("%0%", app);
	m_pConector->Query(query, 0 , this);

	// vectores de la huella
	valores.RemoveAll();
	valores.Add(documento);
	// dedo
	valores.Add(dedo);
	// huella
	valores.Add("%0%");
	app.Empty();
	buffer= bitmap->m_cTemplate;
	for( int i=0; i<bitmap->m_nTemplate; i++) {
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
	query= CQuery::CreateInsert("template", valores);
	query.Replace("%0%", app);
	m_pConector->Query(query, 0 , this);
	return 1;
}

void CSVEView::OnBnClickedButton3()
{
	m_EDocumento.SetWindowText("");
	Clean(true);
	m_szOld="";
	SetError(&m_EDocumento, "Inicializando Huellas");
}

void CSVEView::OnEnKillfocusEcedula()
{
	CString temp;
	m_EDocumento.GetWindowText(temp);
	if( temp.IsEmpty() || temp==m_szOld)
		return;
	m_szOld=temp;
	temp.Format("SELECT nombre,apellido,grupo,observaciones FROM usuario WHERE documento='%s'", m_szOld);
	Clean();
	m_EDocumento.SetWindowText(m_szOld);
	m_pConector->Query(temp, 1, this);
}

void CSVEView::Clean(bool search) {
	
	m_ENombre.SetWindowText("");
	m_EApellido.SetWindowText("");
	m_EObservaciones.SetWindowText("");
	/*if(m_pActual) 
		delete m_pActual;
	m_pActual=NULL;*/

	m_bExiste=false;

	m_SPulgarD.Clean();
	m_SPulgarI.Clean();
	m_SIndiceD.Clean();
	m_SIndiceI.Clean();
	m_SCorazonD.Clean();
	m_SCorazonI.Clean();
	m_SAnularD.Clean();
	m_SAnularI.Clean();
	m_SMeniqueD.Clean();
	m_SMeniqueI.Clean();
	m_CBHorario.SetCurSel(-1);

	m_BAceptar.EnableWindow();
	m_EApellido.SetReadOnly(false);
	m_ENombre.SetReadOnly(false);
	m_EObservaciones.SetReadOnly(false);
	m_CBHorario.EnableWindow();
}

void CSVEView::OnDestroy()
{
	OnBnClickedButton3();
	m_pFinger->OnDestroy();
	CFormView::OnDestroy();
}

LRESULT CSVEView::OnFingerMessage(WPARAM wParam, LPARAM lParam) {

	if(m_SHuella.IsSelected()) {
		Clean();
		m_EDocumento.SetReadOnly(true);
		m_EDocumento.SetWindowText("");
		m_SMensaje.SetWindowText("Buscando");
		VERIFY(m_pFinger->Prepare());
		m_pTemporal= GetTickCount();
		m_pConector->QueryBinary("SELECT huella,documento,dedo FROM template", 100, this);
	} else if(wParam==0) {
		PostMessage(WM_FINGERPRINT, 1, lParam);
	} else if(wParam) {
		CDlgConfirmar dlg;
		dlg.m_pFinger= m_pFinger;
		if(dlg.DoModal()!=IDOK) {
			SetError(NULL, "Confirmación cancelada.");
			CFingerBitmap *bitmap= (CFingerBitmap *)lParam;
			if(bitmap && ::IsWindow(*bitmap)) {
				bitmap->Clean();
			}
		} else SetError(NULL, "Huella confirmada.");
	}
	return 0;
}

CFingerBitmap *CSVEView::GetDedo(int dedo) {

	switch(dedo) {
		case 0:
			return &m_SPulgarD;
		case 1:
			return &m_SPulgarI;
		case 2:
			return &m_SIndiceD;
		case 3:
			return &m_SIndiceI;
		case 4:
			return &m_SCorazonD;
		case 5:
			return &m_SCorazonI;
		case 6:
			return &m_SAnularD;
		case 7:
			return &m_SAnularI;
		case 8:
			return &m_SMeniqueD;
		case 9:
			return &m_SMeniqueI;
	}
	return NULL;
}

LRESULT CSVEView::OnMysqlMessage(WPARAM wParam, LPARAM lParam) {

	MySQLMessage *mess=(MySQLMessage *)lParam;
	MySQLMessageB *messb=(MySQLMessageB *)lParam;
	CThreadMySQL *hilo= (CThreadMySQL *) lParam;

	switch( wParam) {
		case THREAD_END:
			if( hilo->m_nID==8) {
				SelectBitmap(&m_SHuella);
			} else if(hilo->m_nID==100) {
				m_EDocumento.SetReadOnly(false);
				if(!m_bExiste) {
					CString resultado;
					resultado.Format("Huella no encontrada en %g segundos",(double)(GetTickCount()-m_pTemporal)/1000);
					m_SMensaje.SetWindowText(resultado);
				}
			} 
			break;
		case QUERY_EMPTY:
			if( mess->GetID()==1) {
				m_bExiste=false;
				Clean(true);
			}
			break;
		case QUERY_TITLE:
			if(mess->GetID()==115) {
				m_CBHorario.ResetContent();
			}
			break;
		case QUERY_RESULTB:
			if( messb->GetID()==8) {
				SelectBitmap(GetDedo(atoi(messb->GetAt(0))));
				m_pFinger->SetHuella(messb->GetBinaryData());
			} else if( messb->GetID()==10) {
			} else if(messb->GetID()==100) {
				int score=0;
				if(m_pFinger->Compare((const char *)messb->GetBinaryData()->GetData(), &score)) {
					CString resultado;
					resultado.Format("Huella encontrada en %g segundos",(double)(GetTickCount()-m_pTemporal)/1000);
					m_pConector->KillQuery(100, this);
					m_SMensaje.SetWindowText(resultado);
					m_EDocumento.SetWindowText(messb->GetAt(0));
					m_szOld.Empty();
					OnEnKillfocusEcedula();
					m_bExiste=true;
				}
			}
			break;
		case QUERY_RESULT:
			if( mess->GetID()==1) {
				
				CWaitCursor wait;
				m_ENombre.SetWindowText(mess->GetAt(0));
				m_EApellido.SetWindowText(mess->GetAt(1));
				m_CBHorario.SetSelData(atoi(mess->GetAt(2)));
				m_EObservaciones.SetWindowText(mess->GetAt(3));
				m_bExiste=true;
				
				AfxGetMainWnd()->SendMessage(WM_INSERTODS, 10, (LPARAM)(const char *)m_szOld);
				
				m_EApellido.SetReadOnly();
				m_ENombre.SetReadOnly();
				m_EObservaciones.SetReadOnly();
				m_CBHorario.EnableWindow(false);

				m_BAceptar.EnableWindow(false);

				m_pConector->Query("SELECT ancho,alto,resolucion,dedo FROM huella WHERE documento=" + m_szOld, 7, this);
				if(AfxGetApp()->GetProfileInt("control", "compress", 1))
					m_pConector->QueryBinary("SELECT UNCOMPRESS(huella),dedo FROM huella WHERE documento=" + m_szOld, 8, this);
				else m_pConector->QueryBinary("SELECT huella,dedo FROM huella WHERE documento=" + m_szOld, 8, this);
			} else if( mess->GetID()==7) {
				CFingerBitmap *bitmap=GetDedo(atoi(mess->GetAt(3)));
				if(bitmap) {
					bitmap->SetParametros(atoi(mess->GetAt(1)),
						atoi(mess->GetAt(0)),
						atoi(mess->GetAt(2)));
				}
			} else if(mess->GetID()==115) {
				m_CBHorario.SetItemData(m_CBHorario.AddString(mess->GetAt(1)), atoi(mess->GetAt(0)));
			}
			break;
		case QUERY_ERROR:
			MessageBox(mess->GetAt(1), mess->GetAt(0));
			break;
	}
	return 0;
}

LRESULT CSVEView::OnInsertODS(WPARAM wParam, LPARAM lParam) {
	
	CString texto;
	texto.Format("%d", lParam);
	m_szOld="";
	m_EDocumento.SetWindowText(texto);
	OnEnKillfocusEcedula();
	return 0;
}

void CSVEView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	m_pPrint->OnBeginPrinting(pDC, pInfo);
}

void CSVEView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	m_pPrint->OnPrint(pDC, pInfo);
}

BOOL CSVEView::OnPreparePrinting(CPrintInfo* pInfo)
{	
	return DoPreparePrinting(pInfo);
}

void CSVEView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
}

void CSVEView::OnUpdatePrint(CCmdUI *pCmdUI) {

	pCmdUI->Enable(false);
}

void CSVEView::SelectBitmap(CFingerBitmap *bitmap) {

	m_pFinger->SetBitmap(bitmap);
	m_SPulgarD.Select(bitmap==&m_SPulgarD);
	m_SPulgarI.Select( bitmap==&m_SPulgarI);
	m_SIndiceD.Select( bitmap==&m_SIndiceD);
	m_SIndiceI.Select( bitmap==&m_SIndiceI);
	m_SCorazonD.Select( bitmap==&m_SCorazonD);
	m_SCorazonI.Select( bitmap==&m_SCorazonI);
	m_SAnularD.Select( bitmap==&m_SAnularD);
	m_SAnularI.Select( bitmap==&m_SAnularI);
	m_SMeniqueD.Select( bitmap==&m_SMeniqueD);
	m_SMeniqueI.Select( bitmap==&m_SMeniqueI);
	m_SHuella.Select(bitmap==&m_SHuella);
}

void CSVEView::OnStnClickedSpulgard()
{
	if(m_BAceptar.IsWindowEnabled())
		SelectBitmap(&m_SPulgarD);
}

void CSVEView::OnStnClickedSpulgari()
{
	if(m_BAceptar.IsWindowEnabled())
		SelectBitmap(&m_SPulgarI);
}

void CSVEView::OnStnClickedSindiced()
{
	if(m_BAceptar.IsWindowEnabled())
		SelectBitmap(&m_SIndiceD);
}

void CSVEView::OnStnClickedSindicei()
{
	if(m_BAceptar.IsWindowEnabled())
		SelectBitmap(&m_SIndiceI);
}

void CSVEView::OnStnClickedScorazond()
{
	if(m_BAceptar.IsWindowEnabled())
		SelectBitmap(&m_SCorazonD);
}

void CSVEView::OnStnClickedScorazoni()
{
	if(m_BAceptar.IsWindowEnabled())
		SelectBitmap(&m_SCorazonI);
}

void CSVEView::OnStnClickedSanulard()
{
	if(m_BAceptar.IsWindowEnabled())
		SelectBitmap(&m_SAnularD);
}

void CSVEView::OnStnClickedSanulari()
{
	if(m_BAceptar.IsWindowEnabled())
		SelectBitmap(&m_SAnularI);
}

void CSVEView::OnStnClickedSmeniqued()
{
	if(m_BAceptar.IsWindowEnabled())
		SelectBitmap(&m_SMeniqueD);
}

void CSVEView::OnStnClickedSmeniquei()
{
	if(m_BAceptar.IsWindowEnabled())
		SelectBitmap(&m_SMeniqueI);
}

void CSVEView::OnStnClickedShuella()
{
	if(m_BAceptar.IsWindowEnabled())
		SelectBitmap(&m_SHuella);
}
