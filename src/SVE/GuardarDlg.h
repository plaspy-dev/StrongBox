#pragma once
#include "afxwin.h"


// CGuardarDlg dialog

class CGuardarDlg : public CDialog
{
	DECLARE_DYNAMIC(CGuardarDlg)

public:
	int		m_nCampos;
	CGuardarDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGuardarDlg();

// Dialog Data
	enum { IDD = IDD_DGUARDAR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
public:
	CButton m_CUsuario;
public:
	CButton m_CDias;
public:
	CButton m_CHoras;
public:
	afx_msg void OnBnClickedOk();
};
