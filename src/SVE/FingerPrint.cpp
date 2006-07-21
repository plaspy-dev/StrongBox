#include "StdAfx.h"
#include "fingerprint.h"
# include "../iofile/xunzip.h"

CFingerPrint *g_FingerPrint=NULL;

void CALLBACK ImageEventHandler(char* idSensor, unsigned int width, unsigned int height, unsigned char* rawImage, int res) {
	
	if(g_FingerPrint)
		g_FingerPrint->ImageEventHandler(idSensor, width, height, rawImage, res);
}

void CALLBACK FingerEventHandler(char* idSensor, GRCAP_FINGER_EVENTS event) {
	if(g_FingerPrint)
		g_FingerPrint->FingerEventHandler(idSensor, event);
}

void CALLBACK StatusEventHandler(char* idSensor, GRCAP_STATUS_EVENTS event) {

	if(g_FingerPrint)
		g_FingerPrint->StatusEventHandler(idSensor, event);
}

CFingerPrint::CFingerPrint(CWnd *parent)
{
	m_pParent= parent;
    // template
	m_pBitmap= NULL;
	g_FingerPrint= this;
}

CFingerPrint::~CFingerPrint(void)
{
}

CFingerBitmap *CFingerPrint::SetBitmap(CFingerBitmap  *bitmap) {
	CFingerBitmap *old=m_pBitmap;
	m_pBitmap= bitmap;
	return old;
}

void CFingerPrint::Init(CFingerBitmap  *bitmap) {

	m_pBitmap = bitmap;
	const CString commandLine(AfxGetApp()->m_lpCmdLine);
	if (commandLine == _T("--without-reader"))
		return;
	m_bInitialized = GrInitialize() == GR_OK;
	if (m_bInitialized)
		m_bCaptureInitialized = GrCapInitialize(::StatusEventHandler) == GR_OK;
}

void CFingerPrint::OnDestroy() {

	g_FingerPrint=NULL;
	if (m_bCaptureInitialized) {
		GrCapFinalize();
		m_bCaptureInitialized = false;
	}
	if (m_bInitialized) {
		GrFinalize();
		m_bInitialized = false;
	}
}

CWnd *CFingerPrint::SetPadre(CWnd *padre) {

	CWnd *old= m_pParent;
	m_pParent= padre;
	return old;
}

void CFingerPrint::PrintBiometricDisplay(bool biometricDisplay , int context) {
  
	// handle to finger image
	CDC *dc= m_pBitmap->GetDC();
	if (biometricDisplay) {
		GrBiometricDisplay((char *)m_pBitmap->m_cTemplate, m_pBitmap->m_cRaw, m_pBitmap->m_nAncho, m_pBitmap->m_nAlto, m_pBitmap->m_nResolucion, *dc, m_pBitmap->m_hBitmap, context);
	} else {
		GrCapRawImageToHandle(m_pBitmap->m_cRaw, m_pBitmap->m_nAncho, m_pBitmap->m_nAlto, *dc, m_pBitmap->m_hBitmap);
	}
	m_pBitmap->SetWindowText("");
	m_pBitmap->SetBitmap(m_pBitmap->m_hBitmap);
	m_pBitmap->Invalidate();
}

int CFingerPrint::ExtractTemplate() {
  
	m_pBitmap->m_nTemplate=GR_MAX_SIZE_TEMPLATE;

	int result=GrExtract(m_pBitmap->m_cRaw, m_pBitmap->m_nAncho, m_pBitmap->m_nAlto, m_pBitmap->m_nResolucion, (char*)m_pBitmap->m_cTemplate,
		&m_pBitmap->m_nTemplate, GR_DEFAULT_CONTEXT);
	// if error, set template size to 0
	if (result < 0){
		m_pBitmap->Clean();
	} else if( result==GR_BAD_QUALITY) {
		m_pBitmap->Clean();
	} else {
		// PrintBiometricDisplay(true, GR_NO_CONTEXT);
	}
	return result;
}

void CFingerPrint::ImageEventHandler(char* idSensor, unsigned int width, unsigned int height, unsigned char* rawImage, int res) {
	
	// Copying aquired image
	m_szSensor= idSensor;
	memcpy(m_pBitmap->m_cRaw, rawImage, width*height);
	m_pBitmap->m_nAlto= height;
	m_pBitmap->m_nAncho = width;
	m_pBitmap->m_nResolucion = res;

	// WriteEvent(idSensor, GR_IMAGE);
	PrintBiometricDisplay(false, GR_DEFAULT_CONTEXT);
	ExtractTemplate();

	if(m_pParent && ::IsWindow(*m_pParent)) 
		m_pParent->SendMessage(WM_FINGERPRINT, 0, (LPARAM)m_pBitmap);
}

bool CFingerPrint::Prepare() {
	if (!m_bInitialized)
		return false;
	
	if( !m_pBitmap->HasHuella())
		return false;
	return GrIdentifyPrepare((char*)m_pBitmap->m_cTemplate, GR_DEFAULT_CONTEXT)>=0;
}

bool CFingerPrint::Compare(const char *dedo, int *score) {
	if (!m_bInitialized || !dedo)
		return false;
	
	ASSERT(dedo);
	if( !(dedo && m_pBitmap->HasHuella()))
		return false;
	int realscore=0;
	bool igual= GrIdentify((char*)dedo, &realscore, GR_DEFAULT_CONTEXT)==GR_MATCH;
	// bool igual= GrVerify((char *)m_pBitmap->m_cTemplate, (char *)dedo->GetDedo(0), &realscore, GR_DEFAULT_CONTEXT)==GR_MATCH;
	if(score)
		*score= realscore;
	return igual;
}

unsigned char *CFingerPrint::GetRawFoto() {

	return m_pBitmap->m_cRaw;
}

void CFingerPrint::FingerEventHandler(char* idSensor, GRCAP_FINGER_EVENTS event) {
	// Just signals that a finger event ocurred.
	// WriteEvent(idSensor, event);
}

void CFingerPrint::StatusEventHandler(char* idSensor, GRCAP_STATUS_EVENTS event) {

	if (event == GR_PLUG) {
		m_szSensor= idSensor;
		GrCapStartCapture(idSensor, ::FingerEventHandler, ::ImageEventHandler);
	} else if (event == GR_UNPLUG) {
		// Stop capturing from unplugged sensor
		m_szSensor= idSensor;
		GrCapStopCapture(idSensor);
	}
}

bool CFingerPrint::Stop() {
	if (!m_bCaptureInitialized)
		return false;

	if(m_szSensor.IsEmpty())
		return false;
	m_pBitmap->m_nTemplate=0;
	char disp[MAX_PATH];
	strcpy(disp, m_szSensor);
	GrCapStopCapture(disp);
	return true;
}

bool CFingerPrint::Start() {
	if (!m_bCaptureInitialized)
		return false;
	
	if(m_szSensor.IsEmpty())
		return false;
	m_pBitmap->m_nTemplate=0;
	char disp[MAX_PATH];
	strcpy(disp, m_szSensor);
	GrCapStartCapture(disp, ::FingerEventHandler, ::ImageEventHandler);
	return true;
}

int CFingerPrint::GetAncho() {
	return m_pBitmap->m_nAncho;
}

int CFingerPrint::GetAlto() {
	return m_pBitmap->m_nAlto;
}

int CFingerPrint::GetResolucion() {
	return m_pBitmap->m_nResolucion;
}

HBITMAP CFingerPrint::GetBitmap() {
	return m_pBitmap->m_hBitmap;
}

bool CFingerPrint::SetHuella(CByteArray *bytes) {
	
	if( bytes==NULL)
		return false;
	memcpy(m_pBitmap->m_cRaw, bytes->GetData(), bytes->GetCount());
	// aqui hago algo
	PrintBiometricDisplay(false, GR_DEFAULT_CONTEXT);
	ExtractTemplate();
	return true;
}

bool CFingerPrint::SetTemplate(CByteArray *bytes) {
	if( bytes==NULL)
		return false;
	m_pBitmap->m_nTemplate=bytes->GetCount();
	memcpy(m_pBitmap->m_cTemplate, bytes->GetData(),m_pBitmap->m_nTemplate);
	return true;
}

int	CFingerPrint::GetTemplateLength() {

	return m_pBitmap->m_nTemplate;
}

unsigned char *CFingerPrint::GetTemplate() {
	
	return m_pBitmap->m_cTemplate;
}
