#pragma once
#include "afxwin.h"
#include "../iofile/MD5Checksum.h"
# include "../componentes/newmenu/dialogxp.h"
# include "resource.h"
# include "../conector/conector.h"
# include "../componentes/buttonxp.h"
# include "../componentes/editxp.h"
// CDlgIngreso dialog

class CDlgIngreso : public CDialogXP
{
	DECLARE_DYNAMIC(CDlgIngreso)

public:
	CDlgIngreso(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgIngreso();

// Dialog Data
	enum { IDD = IDD_DIALOGCONECTAR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	bool	validateUser(CString usuario, CString passwd, bool codificado= true);
	DECLARE_MESSAGE_MAP()
public:
	CMD5Checksum m_pCoding;
	
	CString		m_szNombreUsuario;
	CString		m_szPasswd;
	CString		m_szNombreReal;
	DWORD		m_dwID;
	DWORD		m_dwGrupo;
	DWORD		m_dwCac;
	CEditXP		m_EUsuario;
	CEditXP		m_EContrasena;
	CButtonXP	m_BOK;
	CButtonXP	m_BCancelar;
	CConector	*m_pConector;
	CStatic		m_SAyuda;
	bool		m_bExiste;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancelar();
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	virtual BOOL OnInitDialog();
};
