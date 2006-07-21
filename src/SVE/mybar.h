# pragma once
#include "../componentes/GuiControlBar.h"
#include "../componentes/treectrlxp.h"

# define WM_TREENAVEGADOR WM_USER+9000

class CMyBar2 : public CGuiControlBar
{
public:
	bool	m_bHiden;
	CMyBar2();
	CTreeCtrlXP m_Tree;
	virtual ~CMyBar2();
	HTREEITEM FindItem(UINT id, HTREEITEM hItem=NULL);
	HTREEITEM FindItem(CString nombre, HTREEITEM hItem=NULL);
	HTREEITEM GetNextItemEx(HTREEITEM hItem);
	HTREEITEM FindItemEx(CString nombre, HTREEITEM hItem=NULL);
	void SetSel( UINT id);
	void SetSel( CString vista);
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnChangeSelArbol(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OpenDocument();
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

