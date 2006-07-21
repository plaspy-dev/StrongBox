// SVEView.h: interfaz de la clase CSVEView
//


#pragma once

#include "afxwin.h"
#include "afxctl.h"
# include "showpic.h"
#include "..\componentes\comboboxxp.h"
#include "..\componentes\buttonxp.h"
#include "..\componentes\editxp.h"
#include "..\componentes\mylistctrl.h"
# include "../conector/conector.h"
# include "../conector/usuario.h"
#include <Componentes/BitmapPicture.h>
# include "printsve.h"
#include <Componentes/SplashScreenEx.h>
# include "fingerprint.h"
#include "afxcmn.h"
# include "fingerbitmap.h"


class CVMR_Capture;

class CSVEView : public CFormView
{
protected: // Crear sólo a partir de serialización
	CSVEView();
	DECLARE_DYNCREATE(CSVEView)

public:
	enum{ IDD = IDD_SVE_FORM };

	CString		m_szOld;
	int			m_nTimer;

	bool		m_bExiste;

	CConector	*m_pConector;
	CUsuario	*m_pUsuario;

	CPrintSVE		*m_pPrint;
	CFingerPrint	*m_pFinger;

// Atributos
public:
	CSVEDoc* GetDocument() const;

// Operaciones
public:

// Reemplazos
	public:
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnInitialUpdate();

// Implementación
public:
	virtual ~CSVEView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Funciones de asignación de mensajes generadas
protected:
	DECLARE_MESSAGE_MAP()
public:
	DWORD			m_pTemporal;
	CSplashScreenEx *m_pSplash;
	CEditXP m_EDocumento;
	CEditXP m_ENombre;
	CEditXP m_EApellido;
	CButtonXP m_BAceptar;
	CButtonXP m_BCancelar;

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnEnKillfocusEcedula();
	void Clean(bool search=false);
	afx_msg void OnDestroy();
	LRESULT OnMysqlMessage(WPARAM wParam, LPARAM lParam);
	LRESULT OnFingerMessage(WPARAM wParam, LPARAM lParam);

	CFingerBitmap	m_SHuella;

	afx_msg LRESULT OnInsertODS(WPARAM wParam, LPARAM lParam);

protected:
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);

protected:
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	void OnUpdatePrint(CCmdUI *pCmdUI);
public:
	CBitmapPicture m_SLogo;
public:
	CStatic m_SMensaje;
	CFingerBitmap m_SPulgarD;
	CFingerBitmap m_SPulgarI;
	CFingerBitmap m_SIndiceD;
	CFingerBitmap m_SIndiceI;
	CFingerBitmap m_SCorazonD;
	CFingerBitmap m_SCorazonI;
	CFingerBitmap m_SAnularD;
	CFingerBitmap m_SAnularI;
	CFingerBitmap m_SMeniqueD;
	CFingerBitmap m_SMeniqueI;
	afx_msg void OnStnClickedSpulgard();
	afx_msg void OnStnClickedSpulgari();
	afx_msg void OnStnClickedSindiced();
	afx_msg void OnStnClickedSindicei();
	afx_msg void OnStnClickedScorazond();
	afx_msg void OnStnClickedScorazoni();
	afx_msg void OnStnClickedSanulard();
	afx_msg void OnStnClickedSanulari();
	afx_msg void OnStnClickedSmeniqued();
	afx_msg void OnStnClickedSmeniquei();
	void SelectBitmap(CFingerBitmap *bitmap);
public:
	afx_msg void OnStnClickedShuella();
	int InsertarHuella(CString documento, CFingerBitmap *bitmap, CString dedo);
	CFingerBitmap *GetDedo(int dedo);
public:
	CEditXP m_EObservaciones;
public:
	CComboBoxXP m_CBHorario;
};

#ifndef _DEBUG  // Versión de depuración en SVEView.cpp
inline CSVEDoc* CSVEView::GetDocument() const
   { return reinterpret_cast<CSVEDoc*>(m_pDocument); }
#endif

