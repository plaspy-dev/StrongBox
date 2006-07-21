// DlgPopUp.cpp : implementation file
//

#include "stdafx.h"
#include "sveclient.h"
#include "DlgPopUp.h"

#define IDT_HIDDEN		0
#define IDT_APPEARING		1
#define IDT_WAITING		2
#define IDT_DISAPPEARING	3

#define TASKBAR_ON_TOP		1
#define TASKBAR_ON_LEFT		2
#define TASKBAR_ON_RIGHT	3
#define TASKBAR_ON_BOTTOM	4

// CDlgPopUp dialog

IMPLEMENT_DYNAMIC(CDlgPopUp, CDialog)

CDlgPopUp::CDlgPopUp(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPopUp::IDD, pParent)
{
	m_nSkinWidth=0;
	m_nSkinHeight=0;
	m_dwTimeToShow=0;
	m_dwTimeToLive=0;
	m_dwTimeToHide=0;
	m_nAnimStatus=IDT_HIDDEN;
	m_dwDelayBetweenShowEvents=0;
	m_dwDelayBetweenHideEvents=0;
	m_nIncrement=2;
	m_nStartPosX=0;
	m_nStartPosY=0;
	m_nCurrentPosX=0;
	m_nCurrentPosY=0;
	m_nTaskbarPlacement=0;
}

CDlgPopUp::~CDlgPopUp()
{
}

void CDlgPopUp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SMATRIZ, m_SMatriz);
}


BEGIN_MESSAGE_MAP(CDlgPopUp, CDialog)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDlgPopUp message handlers

BOOL CDlgPopUp::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_SMatriz.SetNumberOfLines(2);
	m_SMatriz.SetXCharsPerLine(MATRIZ_ANCHO);
	m_SMatriz.SetSize(CMatrixStatic::SMALL);
	m_SMatriz.AdjustClientXToSize(MATRIZ_ANCHO);
	m_SMatriz.AdjustClientYToSize(2);

	CRect rect;
	GetClientRect(rect);
	m_nSkinWidth= rect.Width();
	m_nSkinHeight= rect.Height();

	MoveWindow(0,0,0,0);
	ShowWindow(SW_HIDE);

	return TRUE;
}

void CDlgPopUp::Show(CString szCaption,int color, DWORD dwTimeToShow,DWORD dwTimeToLive,DWORD dwTimeToHide,int nIncrement) {
	
	unsigned int nDesktopHeight;
	unsigned int nDesktopWidth;
	unsigned int nScreenWidth;
	unsigned int nScreenHeight;

	m_strCaption=szCaption;

	switch(color) {
		case 2: // rojo
			m_SMatriz.SetDisplayColors(RGB(0, 0, 0), RGB(255, 60, 0), RGB(103, 30, 0));
			break;
		case 1: // verde
			m_SMatriz.SetDisplayColors(RGB(0, 0, 0), RGB(0, 255, 50), RGB(0, 103, 30));
			break;
		default: // azul
			m_SMatriz.SetDisplayColors(RGB(0, 0, 0), RGB(63, 181, 255), RGB(23, 64,  103));
			break;
	}

	m_SMatriz.SetText(m_strCaption);

	m_dwTimeToShow=dwTimeToShow;
	m_dwTimeToLive=dwTimeToLive;
	m_dwTimeToHide=dwTimeToHide;

	::SystemParametersInfo(SPI_GETWORKAREA,0,&m_rectWorkingArea,0);
	nDesktopWidth=m_rectWorkingArea.right-m_rectWorkingArea.left;
	nDesktopHeight=m_rectWorkingArea.bottom-m_rectWorkingArea.top;
	nScreenWidth=::GetSystemMetrics(SM_CXSCREEN);
	nScreenHeight=::GetSystemMetrics(SM_CYSCREEN);

	BOOL bTaskbarOnRight=nDesktopWidth<nScreenWidth && m_rectWorkingArea.left==0;
	BOOL bTaskbarOnLeft=nDesktopWidth<nScreenWidth && m_rectWorkingArea.left!=0;
	BOOL bTaskBarOnTop=nDesktopHeight<nScreenHeight && m_rectWorkingArea.top!=0;
	BOOL bTaskbarOnBottom=nDesktopHeight<nScreenHeight && m_rectWorkingArea.top==0;
	
	switch (m_nAnimStatus)
	{
		case IDT_HIDDEN:
			ShowWindow(SW_SHOW);
			if (bTaskbarOnRight)
			{
				m_dwDelayBetweenShowEvents=m_dwTimeToShow/(m_nSkinWidth/m_nIncrement);
				m_dwDelayBetweenHideEvents=m_dwTimeToHide/(m_nSkinWidth/m_nIncrement);
				m_nStartPosX=m_rectWorkingArea.right;
				m_nStartPosY=m_rectWorkingArea.bottom-m_nSkinHeight;
				m_nTaskbarPlacement=TASKBAR_ON_RIGHT;
			}
			else if (bTaskbarOnLeft)
			{
				m_dwDelayBetweenShowEvents=m_dwTimeToShow/(m_nSkinWidth/m_nIncrement);
				m_dwDelayBetweenHideEvents=m_dwTimeToHide/(m_nSkinWidth/m_nIncrement);
				m_nStartPosX=m_rectWorkingArea.left-m_nSkinWidth;
				m_nStartPosY=m_rectWorkingArea.bottom-m_nSkinHeight;
				m_nTaskbarPlacement=TASKBAR_ON_LEFT;
			}
			else if (bTaskBarOnTop)
			{
				m_dwDelayBetweenShowEvents=m_dwTimeToShow/(m_nSkinHeight/m_nIncrement);
				m_dwDelayBetweenHideEvents=m_dwTimeToHide/(m_nSkinHeight/m_nIncrement);
				m_nStartPosX=m_rectWorkingArea.right-m_nSkinWidth;
				m_nStartPosY=m_rectWorkingArea.top-m_nSkinHeight;
				m_nTaskbarPlacement=TASKBAR_ON_TOP;
			}
			else //if (bTaskbarOnBottom)
			{
				// Taskbar is on the bottom or Invisible
				m_dwDelayBetweenShowEvents=m_dwTimeToShow/(m_nSkinHeight/m_nIncrement);
				m_dwDelayBetweenHideEvents=m_dwTimeToHide/(m_nSkinHeight/m_nIncrement);
				m_nStartPosX=m_rectWorkingArea.right-m_nSkinWidth;
				m_nStartPosY=m_rectWorkingArea.bottom;
				m_nTaskbarPlacement=TASKBAR_ON_BOTTOM;
			}

			m_nCurrentPosX=m_nStartPosX;
			m_nCurrentPosY=m_nStartPosY;
	
			SetTimer(IDT_APPEARING,m_dwDelayBetweenShowEvents,NULL);
			break;

		case IDT_WAITING:
			RedrawWindow();
			KillTimer(IDT_WAITING);
			SetTimer(IDT_WAITING,m_dwTimeToLive,NULL);
			break;

		case IDT_APPEARING:
			RedrawWindow();
			break;

		case IDT_DISAPPEARING:
			KillTimer(IDT_DISAPPEARING);
			SetTimer(IDT_WAITING,m_dwTimeToLive,NULL);
			if (bTaskbarOnRight)
				m_nCurrentPosX=m_rectWorkingArea.right-m_nSkinWidth;
			else if (bTaskbarOnLeft)
				m_nCurrentPosX=m_rectWorkingArea.left;
			else if (bTaskBarOnTop)
				m_nCurrentPosY=m_rectWorkingArea.top;
			else //if (bTaskbarOnBottom)
				m_nCurrentPosY=m_rectWorkingArea.bottom-m_nSkinHeight;
			MoveWindow(m_nCurrentPosX,m_nCurrentPosY,m_nSkinWidth,m_nSkinHeight);
			RedrawWindow();
			break;
	}
}

void CDlgPopUp::Hide() {

	switch (m_nAnimStatus)
	{
		case IDT_APPEARING:
			KillTimer(IDT_APPEARING);
			break;
		case IDT_WAITING:
			KillTimer(IDT_WAITING);
			break;
		case IDT_DISAPPEARING:
			KillTimer(IDT_DISAPPEARING);
			break;
	}
	MoveWindow(0,0,0,0);
	ShowWindow(SW_HIDE);
	m_nAnimStatus=IDT_HIDDEN;
}

void CDlgPopUp::MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint) {

	CRect rect(x, y, x+nWidth, y+nHeight);

	switch(m_nTaskbarPlacement) {
		case TASKBAR_ON_BOTTOM:
			if( rect.bottom>m_rectWorkingArea.bottom)
				rect.bottom= m_rectWorkingArea.bottom;
			break;
		case TASKBAR_ON_LEFT:
			if( rect.left<m_rectWorkingArea.left)
				rect.left= m_rectWorkingArea.left;
			break;
		case TASKBAR_ON_RIGHT:
			if( rect.right>m_rectWorkingArea.right)
				rect.right= m_rectWorkingArea.right;
			break;
		case TASKBAR_ON_TOP:
			if( rect.top<m_rectWorkingArea.top)
				rect.top= m_rectWorkingArea.top;
			break;
	}
	CDialog::MoveWindow(rect, bRepaint);
}

void CDlgPopUp::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
		case IDT_APPEARING:
			m_nAnimStatus=IDT_APPEARING;
			switch(m_nTaskbarPlacement)
			{
				case TASKBAR_ON_BOTTOM:
					if (m_nCurrentPosY>(m_nStartPosY-m_nSkinHeight))
						m_nCurrentPosY-=m_nIncrement;
					else
					{
						KillTimer(IDT_APPEARING);
						SetTimer(IDT_WAITING,m_dwTimeToLive,NULL);
						m_nAnimStatus=IDT_WAITING;
					}
					break;
				case TASKBAR_ON_TOP:
					if ((m_nCurrentPosY-m_nStartPosY)<m_nSkinHeight)
						m_nCurrentPosY+=m_nIncrement;
					else
					{
						KillTimer(IDT_APPEARING);
						SetTimer(IDT_WAITING,m_dwTimeToLive,NULL);
						m_nAnimStatus=IDT_WAITING;
					}
					break;
				case TASKBAR_ON_LEFT:
					if ((m_nCurrentPosX-m_nStartPosX)<m_nSkinWidth)
						m_nCurrentPosX+=m_nIncrement;
					else
					{
						KillTimer(IDT_APPEARING);
						SetTimer(IDT_WAITING,m_dwTimeToLive,NULL);
						m_nAnimStatus=IDT_WAITING;
					}
					break;
				case TASKBAR_ON_RIGHT:
					if (m_nCurrentPosX>(m_nStartPosX-m_nSkinWidth))
						m_nCurrentPosX-=m_nIncrement;
					else
					{
						KillTimer(IDT_APPEARING);
						SetTimer(IDT_WAITING,m_dwTimeToLive,NULL);
						m_nAnimStatus=IDT_WAITING;
					}
					break;
			}
			MoveWindow(m_nCurrentPosX,m_nCurrentPosY,m_nSkinWidth,m_nSkinHeight);
			// SetWindowPos(NULL,m_nCurrentPosX,m_nCurrentPosY,m_nSkinWidth,m_nSkinHeight,SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_NOACTIVATE);
			//RedrawWindow();
			break;

		case IDT_WAITING:
                  KillTimer(IDT_WAITING);
			SetTimer(IDT_DISAPPEARING,m_dwDelayBetweenHideEvents,NULL);
			break;

		case IDT_DISAPPEARING:
			m_nAnimStatus=IDT_DISAPPEARING;
			switch(m_nTaskbarPlacement)
			{
				case TASKBAR_ON_BOTTOM:
					if (m_nCurrentPosY<m_nStartPosY)
						m_nCurrentPosY+=m_nIncrement;
					else
					{
						KillTimer(IDT_DISAPPEARING);
						Hide();
					}
					break;
				case TASKBAR_ON_TOP:
					if (m_nCurrentPosY>m_nStartPosY)
						m_nCurrentPosY-=m_nIncrement;
					else
					{
						KillTimer(IDT_DISAPPEARING);
						Hide();
					}
					break;
				case TASKBAR_ON_LEFT:
					if (m_nCurrentPosX>m_nStartPosX)
						m_nCurrentPosX-=m_nIncrement;
					else
					{
						KillTimer(IDT_DISAPPEARING);
						Hide();
					}
					break;
				case TASKBAR_ON_RIGHT:
					if (m_nCurrentPosX<m_nStartPosX)
						m_nCurrentPosX+=m_nIncrement;
					else
					{
						KillTimer(IDT_DISAPPEARING);
						Hide();
					}
					break;
			}
			MoveWindow(m_nCurrentPosX,m_nCurrentPosY,m_nSkinWidth,m_nSkinHeight);
			// SetWindowPos(NULL,m_nCurrentPosX,m_nCurrentPosY,m_nSkinWidth,m_nSkinHeight,SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_NOACTIVATE);
			//RedrawWindow();
			break;
	}

	CDialog::OnTimer(nIDEvent);
}
