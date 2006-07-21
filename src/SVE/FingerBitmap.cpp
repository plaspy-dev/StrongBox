#include "StdAfx.h"
#include "FingerBitmap.h"
# include "../componentes/draw.h"
# include "GrFinger.h"

CFingerBitmap::CFingerBitmap(void)
{
	m_dwBordes= ::GetSysColor(COLOR_HIGHLIGHT);
	m_dwUnSelBorde= HLS_TRANSFORM( m_dwBordes,  +80, -66);
	m_bSelected=false;
	m_nTemplate=0;
	m_hBitmap= NULL;
	m_cTemplate=new unsigned char[GR_MAX_SIZE_TEMPLATE];
	m_nAncho=0;
	m_nAlto=0;
	m_nResolucion=0;
	m_cRaw=new unsigned char[GR_MAX_IMAGE_HEIGHT * GR_MAX_IMAGE_WIDTH];
	m_szMensaje="";
}

CFingerBitmap::~CFingerBitmap(void)
{
	if( m_cTemplate)
		delete m_cTemplate;
	if( m_cRaw)
		delete m_cRaw;
}

BEGIN_MESSAGE_MAP(CFingerBitmap, CBitmapPicture)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

void CFingerBitmap::Clean() {
	if(m_nTemplate) {
		m_nTemplate=0;
		SetBitmap((HBITMAP) NULL);
		SetWindowText(m_szMensaje);
		Invalidate();
	}
}
void CFingerBitmap::SetMessage(CString mensaje) {

	m_szMensaje= mensaje;
}
void CFingerBitmap::Select(bool select/*=true*/) {
	
	if( m_bSelected==select)
		return;
	m_bSelected= select;
	Invalidate();
}
bool CFingerBitmap::IsSelected() {
	return m_bSelected;
}

void CFingerBitmap::SetBorderColor(COLORREF color) {

	if(m_dwBordes==color)
		return;
	m_dwBordes= color;
	Invalidate();
}

BOOL CFingerBitmap::OnEraseBkgnd(CDC* pDC)
{
	CBitmapPicture::OnEraseBkgnd(pDC);
	CRect rect;
	GetClientRect(rect);
	
	CPen pen(PS_SOLID,2,m_bSelected ? m_dwBordes : m_dwUnSelBorde);
	CBrush *brush=(CBrush *) pDC->SelectStockObject(NULL_BRUSH);
	CPen *oldpen= pDC->SelectObject(&pen);
	pDC->Rectangle(rect);
	pDC->SelectObject(oldpen);
	pDC->SelectObject(brush);
	return TRUE;
}

bool CFingerBitmap::HasHuella() {

	return m_nTemplate!=0;
}

DWORD CFingerBitmap::GetSize() {

	return m_nAlto*m_nAncho;
}

void CFingerBitmap::SetParametros(int alto, int ancho, int res) {
	
	m_nAlto= alto;
	m_nAncho= ancho;
	m_nResolucion= res;
}