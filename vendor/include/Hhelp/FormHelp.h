#pragma once
#include "afxcmn.h"
# include "resource.h"
#include "DlgIndex.h"
#include "DlgSearchHelp.h"

class CFormHelp : public CDialog
{
	DECLARE_DYNAMIC(CFormHelp)

public:
	CFormHelp(CWnd* pParent = NULL);
	virtual ~CFormHelp();

	enum { IDD = IDD_FHELPTAB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	DECLARE_MESSAGE_MAP()
public:
	CTabCtrlSSL m_TabCtrl;
	virtual BOOL OnInitDialog();
	CDlgIndex		m_pDlgIndex;
	CDlgSearchHelp	m_pDlgSearch;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	LRESULT OnQueryNavigate(WPARAM wParam, LPARAM lParam);
	void SetArchivos(CMiscelaneosEx *archivos);
	afx_msg void OnTcnSelchangeTelementos(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickTelementos(NMHDR *pNMHDR, LRESULT *pResult);
	CString		FindTitulo(const CString &titulo);
};
