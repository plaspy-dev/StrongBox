#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "threadsearch.h"
# include "Resource.h"

// Cuadro de diálogo de CDlgSearchHelp

class CDlgSearchHelp : public CTabPageSSL
{
	DECLARE_DYNAMIC(CDlgSearchHelp)

public:
	CDlgSearchHelp(CWnd* pParent = NULL);   // Constructor estándar
	virtual ~CDlgSearchHelp();

// Datos del cuadro de diálogo
	enum { IDD = IDD_FSEARCHHELP };
	int		m_nComboBox;
	CThreadSearch *m_pSearch;
	CMiscelaneosEx *m_pArchivos;
	void		SetArchivos(CMiscelaneosEx *archivos);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Compatibilidad con DDX o DDV

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CComboBoxXP m_CBBuscar;
	CButtonXP m_BBuscar;
	CMyListCtrl m_LCResultados;
	afx_msg void OnBnClickedBbuscar();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNMDblclkLresultados(NMHDR *pNMHDR, LRESULT *pResult);
};
