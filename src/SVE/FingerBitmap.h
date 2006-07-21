#pragma once
#include <Componentes/BitmapPicture.h>

class CFingerBitmap : public CBitmapPicture
{
protected:
	COLORREF	m_dwBordes;
	COLORREF	m_dwUnSelBorde;
	bool		m_bSelected;
	CString		m_szMensaje;
public:
	int m_nTemplate;
	unsigned char *m_cTemplate;
	int m_nAncho;
	int m_nAlto;
	int m_nResolucion;
	unsigned char *m_cRaw;
	HBITMAP m_hBitmap;
	CFingerBitmap(void);
public:
	~CFingerBitmap(void);
public:
	DECLARE_MESSAGE_MAP()
public:
	void Select(bool select=true);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	void SetBorderColor(COLORREF color);
	void Clean();
	void SetMessage(CString mensaje);
	bool HasHuella();
	DWORD GetSize();
	void SetParametros(int alto, int ancho, int res);
	bool IsSelected();
};
