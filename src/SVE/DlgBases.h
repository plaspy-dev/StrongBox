#pragma once
#include "afxcmn.h"
#include "afxwin.h"

#include "../componentes/separador\sectionseparator.h"
#include "../componentes/separador\separator.h"
#include "../componentes/separador\CheckBoxSeparator.h"
#include "../componentes/separador\staticseparator.h"
#include "../componentes/EditXP.h"
#include "../componentes/buttonxp.h"
#include "../componentes/mylistctrl.h"
# include "resource.h"
// CDlgBases dialog

class CDlgBases : public CDialog
{
	DECLARE_DYNAMIC(CDlgBases)

public:
	CDlgBases(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgBases();

// Dialog Data
	enum { IDD = IDD_DIALOGCONEXIONES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	bool		m_bModificar;
	int			m_nSeleccion;
	CMyListCtrl m_LCDB;
	CEditXP		m_ENombreServidor;
	CEditXP		m_ENombreDB;
	CEditXP		m_EPuerto;
	CEditXP		m_EReintentos;
	CButtonXP	m_BOK;
	CButtonXP	m_BCancelar;
	CSectionSeparator m_SPrimero;
	CSeparator m_SSegundo;

	afx_msg void OnBnClickedOk();
	afx_msg void OnNMRclickListdb(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkListdb(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnEnChangeEditservidor();
	afx_msg void OnListasEditar();
	afx_msg void OnListasEliminar();
	afx_msg void OnBnClickedCancel();
};
