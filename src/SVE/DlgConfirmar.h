#pragma once
#include "afxwin.h"
# include "fingerprint.h"
# include "fingerbitmap.h"
#include "..\componentes\buttonxp.h"

// CDlgConfirmar dialog

class CDlgConfirmar : public CDialog
{
	DECLARE_DYNAMIC(CDlgConfirmar)

public:
	CDlgConfirmar(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgConfirmar();

// Dialog Data
	enum { IDD = IDD_DCONFIRMAR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CFingerBitmap *m_SHuellaOld;
	CWnd *m_pOldParent;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedCancel();
public:
	CFingerBitmap m_SHuella;
public:
	virtual BOOL OnInitDialog();
	CFingerPrint *m_pFinger;
	LRESULT OnFingerPrint(WPARAM wParam, LPARAM lParam);
public:
	CStatic m_SMensaje;
public:
	CButtonXP m_BCancelar;
};
