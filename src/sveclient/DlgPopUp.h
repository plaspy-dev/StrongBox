#pragma once
#include "afxwin.h"
#include <Componentes/MatrixStatic.h>

# define MATRIZ_ANCHO			30

class CDlgPopUp : public CDialog
{
	DECLARE_DYNAMIC(CDlgPopUp)

	int		m_nSkinWidth;
	int		m_nSkinHeight;
	DWORD	m_dwTimeToShow;
	DWORD	m_dwTimeToLive;
	DWORD	m_dwTimeToHide;
	int		m_nAnimStatus;
	DWORD	m_dwDelayBetweenShowEvents;
	DWORD	m_dwDelayBetweenHideEvents;
	int		m_nIncrement;
	CString	m_strCaption;
	int		m_nStartPosX;
	int		m_nStartPosY;
	int		m_nCurrentPosX;
	int		m_nCurrentPosY;
	int		m_nTaskbarPlacement;
	CRect   m_rectWorkingArea;

public:
	CDlgPopUp(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgPopUp();

// Dialog Data
	enum { IDD = IDD_DPOPUP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CMatrixStatic m_SMatriz;
public:
	virtual BOOL OnInitDialog();
	void Show(CString szCaption,int color=0, DWORD dwTimeToShow=500,DWORD dwTimeToLive=3000,DWORD dwTimeToHide=500,int nIncrement=1);
	void Hide();
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint=1);
};
