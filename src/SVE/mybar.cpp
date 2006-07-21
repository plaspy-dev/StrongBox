#include "stdafx.h"
# include "../componentes/newmenu/multidoctemplatexp.h"
# include "../componentes/newmenu/newmenu.h"
#include "mybar.h"

/////////////////////////////////////////////////////////////////////////////
// CMyBar2

CMyBar2::CMyBar2()
{
}

CMyBar2::~CMyBar2()
{
}

BEGIN_MESSAGE_MAP(CMyBar2, CGuiControlBar)
	ON_WM_CREATE()
	ON_NOTIFY(NM_DBLCLK, 201, OnChangeSelArbol)
	ON_NOTIFY(NM_RCLICK, 201, OnRClick)
	ON_COMMAND(201,OpenDocument)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMyBar message handlers

HTREEITEM CMyBar2::FindItem(CString nombre, HTREEITEM hItem)
{
	if( !m_Tree)
		return NULL;
	HTREEITEM htiCurrent = hItem ? hItem : m_Tree.GetRootItem();
	while(htiCurrent)
	{
		if( m_Tree.GetItemText( htiCurrent)== nombre)
			return htiCurrent;
		htiCurrent = GetNextItemEx(htiCurrent);
	}
	return NULL;
}

HTREEITEM CMyBar2::FindItem(UINT id, HTREEITEM hItem)
{
	if( !m_Tree)
		return NULL;
	HTREEITEM htiCurrent = hItem ? hItem : m_Tree.GetRootItem();
	while(htiCurrent)
	{
		if ((UINT)m_Tree.GetItemData(htiCurrent)== id)
		
			return htiCurrent;
		htiCurrent = GetNextItemEx(htiCurrent);
	}
	return NULL;
}

HTREEITEM CMyBar2::GetNextItemEx(HTREEITEM hItem)
{
	HTREEITEM hti;

	if(m_Tree.ItemHasChildren(hItem))

		return m_Tree.GetChildItem(hItem);
	else
		while((hti = m_Tree.GetNextSiblingItem(hItem)) == NULL)

			if((hItem = m_Tree.GetParentItem(hItem)) == NULL)

				return NULL;
    return hti;
}

HTREEITEM CMyBar2::FindItemEx(CString nombre, HTREEITEM hItem) {

	if( !m_Tree)
		return NULL;
	HTREEITEM htiCurrent = hItem ? hItem : m_Tree.GetRootItem();
	while(htiCurrent)
	{
		DWORD data= m_Tree.GetItemData(htiCurrent);
		if(data && ((CMultiDocTemplateXP *)data)->GetViewClassName()==nombre)
			return htiCurrent;
		htiCurrent = GetNextItemEx(htiCurrent);
	}
	return NULL;
}

void CMyBar2::SetSel( CString vista) {

	m_Tree.SelectItem(FindItemEx(vista));
}

void CMyBar2::SetSel( UINT id) {

	HTREEITEM item=NULL;
	if( id)
		item= FindItem( id);
	m_Tree.SelectItem(item);
}

int CMyBar2::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CGuiControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	CRect rect;
	m_Tree.Create(WS_VISIBLE | WS_BORDER | WS_CHILD | TVS_HASBUTTONS|TVS_LINESATROOT|TVS_HASLINES|TVS_SHOWSELALWAYS , rect, this, 201);
	return 0;
}

void CMyBar2::OpenDocument() {

	CWaitCursor wait;
	HTREEITEM item= m_Tree.GetSelectedItem();
	if( item) 
		AfxGetMainWnd()->SendMessage(WM_TREENAVEGADOR, NM_DBLCLK, (LPARAM) m_Tree.GetItemData( item));
}

void CMyBar2::OnChangeSelArbol( NMHDR* pNMHDR, LRESULT* pResult) {

	OpenDocument();
	pResult= 0;
}

void CMyBar2::OnRClick(NMHDR* pNMHDR, LRESULT* pResult) {

	POINT point;
	POINT gpoint;
	UINT uFlags;

	GetCursorPos( &point);
	gpoint= point;
	m_Tree.ScreenToClient( &point);
	HTREEITEM item= m_Tree.HitTest(point, &uFlags);

	if( item && m_Tree.GetItemData(item) && (TVHT_ONITEM & uFlags)) {
		m_Tree.SelectItem(item);
		CNewMenu menu;
		menu.CreatePopupMenu();
		menu.AppendMenu(MF_STRING, 201, "&Abrir");
		menu.TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, gpoint.x, gpoint.y, this);
	}
	pResult= 0;
}

BOOL CMyBar2::PreTranslateMessage(MSG* pMsg)
{
	if( pMsg->message==WM_KEYUP && pMsg->wParam==VK_RETURN) {
		if( GetFocus()== &m_Tree) 
			OpenDocument();
	}
	return CGuiControlBar::PreTranslateMessage(pMsg);
}
