#pragma once

#include "fingerbitmap.h"
# include "GrFinger.h"

# define WM_FINGERPRINT	WM_USER+1054
# define WM_FINGERPUT	WM_USER+1055

class CFingerPrint
{

protected:
	CWnd	*m_pParent;
	CFingerBitmap *m_pBitmap;
	bool m_bInitialized = false;
	bool m_bCaptureInitialized = false;

	CString		m_szSensor;

	void PrintBiometricDisplay(bool biometricDisplay , int context);
	int ExtractTemplate();

public:
	void ImageEventHandler(char* idSensor, unsigned int width, unsigned int height, unsigned char* rawImage, int res);
	void FingerEventHandler(char* idSensor, GRCAP_FINGER_EVENTS event);
	void StatusEventHandler(char* idSensor, GRCAP_STATUS_EVENTS event);
public:
	CFingerPrint(CWnd *parent);
	~CFingerPrint(void);
	void Init(CFingerBitmap *bitmap);
	CFingerBitmap *SetBitmap(CFingerBitmap *bitmap);
	void OnDestroy();
	bool Stop();
	bool Start();
	int GetAncho();
	int GetAlto();
	int GetResolucion();
	unsigned char *GetRawFoto();
	unsigned char *GetTemplate();
	int	GetTemplateLength();
	CWnd *SetPadre(CWnd *padre);

	bool SetHuella(CByteArray *bytes);
	bool SetTemplate(CByteArray *bytes);
	HBITMAP GetBitmap();
	bool Compare(const char *dedo, int *score);
	bool Prepare();
};

// globales
void CALLBACK ImageEventHandler(char* idSensor, unsigned int width, unsigned int height, unsigned char* rawImage, int res);
void CALLBACK FingerEventHandler(char* idSensor, GRCAP_FINGER_EVENTS event);
void CALLBACK StatusEventHandler(char* idSensor, GRCAP_STATUS_EVENTS event);
