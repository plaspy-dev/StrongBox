// LBSpinButtonCtrl.h : header file
//
// Copyright © 1999 Oleg Lobach, All Rights Reserved.
//
// mailto:oleglb@mail.ru
//or
// mailto:oleg@alexen.ru
//
//
// This source file may be redistributed unmodified by any means PROVIDING 
// it is NOT sold for profit without the authors expressed written 
// consent, and providing that this notice and the authors name and all 
// copyright notices remain intact. This software is by no means to be 
// included as part of any third party components library, or as part any
// development solution that offers MFC extensions that are sold for profit. 
// 
// If the source code is used in any commercial applications then a statement 
// along the lines of:
// 
// "Portions Copyright © 1999 Oleg Lobach" must be included in the "Startup 
// Banner", "About Box" or "Printed Documentation". This software is provided 
// "as is" without express or implied warranty. Use it at your own risk! The 
// author(s) accepts no liability for any damage/loss of business that this 
// product may cause.
//
/////////////////////////////////////////////////////////////////////////////
//****************************************************************************

#if !defined(AFX_LBSPINBUTTONCTRL_H__16B8B6EF_C905_11D3_AA40_008048EAE3F5__INCLUDED_)
#define AFX_LBSPINBUTTONCTRL_H__16B8B6EF_C905_11D3_AA40_008048EAE3F5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../componentes/GuiControlBar.h"
#include "../../componentes/tools.h"

/////////////////////////////////////////////////////////////////////////////
// CLBSpinButtonCtrl window

class CLBSpinButtonCtrl : public CSpinButtonCtrl
{
// Construction
public:
	CLBSpinButtonCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLBSpinButtonCtrl)
	protected:
	virtual void PreSubclassWindow();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	bool SetAutoDisable(bool bSetOn);
	virtual ~CLBSpinButtonCtrl();

protected:	

	virtual void Init();
	void CleanUpHook() const;
	void DisableRect(CDC &dc, const CRect &rectDisable) const;

protected:
	CGuiControlBarButton m_BDerecha;
	CGuiControlBarButton m_BIzquierda;
	
	CRect m_RectD;
	CRect m_RectI;
	int m_nStateD;
	int m_nStateI;
	int m_nDisable;
	CMouseMgr	m_MouseMgr;

	void DrawButtons(CRect *recti=NULL,CRect *rectd=NULL, CDC *pDC=NULL);
	int  MiHitTest(CPoint point);
	
	HWND m_hWndBuddy;

	bool m_bAutoDisable;
	bool m_bBuddyIsEdit;
	bool m_bDefaultDirection;
	bool m_bActiveSpinPressed;
	bool m_bVertical;

	CRect m_rctIsPressed;
	CRect m_rctClient;

	int m_nPrevPos;
	int m_nMinPos;
	int m_nMaxPos;

	UINT m_nSpinAlign;
	UINT m_nSpinState;
	enum m_nState {BothEnable=0,DisableRight,DisableLeft,BothDisable};
	enum m_nPosition{Outside=0,OnRightInside,OnLeftInside};

protected:
	//{{AFX_MSG(CLBSpinButtonCtrl)
	afx_msg BOOL OnDeltapos(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()

private:
	HPEN m_penLightShadow;
	HPEN m_penDarkShadow;
	HPEN m_penShadow;
	HPEN m_penLight;
	HPEN m_penButtonFace;
public:
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnMouseOut(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
#endif // !defined(AFX_LBSPINBUTTONCTRL_H__16B8B6EF_C905_11D3_AA40_008048EAE3F5__INCLUDED_)
