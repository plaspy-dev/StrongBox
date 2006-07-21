#pragma once
#include "afxcmn.h"
# include <Componentes/Componentes.h>
# include "resource.h"

class CDlgIndex : public CTabPageSSL
{
	DECLARE_DYNAMIC(CDlgIndex)

public:
	CDlgIndex(CWnd* pParent = NULL);
	virtual ~CDlgIndex();
	int				m_nRoot, m_nRootOver;
	int				m_nItem, m_nItemOver;
	int				m_nRecomendado;
	CMiscelaneosEx	m_pArchivos;
	CMiscelaneosEx  m_pRecomendado;
	HTREEITEM		m_htRecomendado;

	enum { IDD = IDD_FINDEX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl m_TCIndex;
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	LRESULT OnQueryNavigate(WPARAM wParam, LPARAM lParam);
	afx_msg void OnTvnSelchangedTindex(NMHDR *pNMHDR, LRESULT *pResult);
	CString		FindTitulo(const CString &titulo);
};
