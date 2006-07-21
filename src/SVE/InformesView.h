#pragma once
#include "..\componentes\buttonxp.h"
#include "..\componentes\mylistctrl.h"
# include "../conector/conector.h"
# include "../conector/usuario.h"
#include "afxdtctl.h"
#include "..\componentes\editxp.h"
#include "../iofile/ListDemoViewPrint.h"

// Vista de formulario de CInformesView

class CInformesView : public CFormView
{
	DECLARE_DYNCREATE(CInformesView)

protected:
	CInformesView();           // Constructor protegido utilizado por la creación dinámica
	virtual ~CInformesView();
	CConector	*m_pConector;
	CListDemoViewPrint *m_pListPrint;
	CUsuario	*m_pUsuario;

public:
	enum { IDD = IDD_FINFORMES };
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Compatibilidad con DDX o DDV

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	CButtonXP m_BAceptar;
	CMyListCtrl m_LCResultados;
	afx_msg void OnBnClickedOk();
	CDateTimeCtrl m_DTDesde;
	CDateTimeCtrl m_DTHasta;
	LRESULT OnMysqlMessage(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNMDblclkLresultados(NMHDR *pNMHDR, LRESULT *pResult);
	CEditXP m_EDocumento;
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	void OnUpdatePrint(CCmdUI *pCmdUI);
	void OnArchivoExportar();
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};


