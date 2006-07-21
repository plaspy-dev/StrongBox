/****************************************************************************\
Datei  : MDITabs.h
Projekt: MDITabs, a tabcontrol for switching between MDI-views
Inhalt : CMDITabs implementation
Datum  : 03.10.2001
Autor  : Christian Rodemeyer
Hinweis: © 2001 by Christian Rodemeyer
\****************************************************************************/

#include "stdafx.h"
#include "MDITabs.h"
#include <AFXPRIV.H>
#include <algorithm>
#include <vector>
# include "..\..\componentes\draw.h"
# include "..\..\componentes\newmenu/newmenu.h"
# include "../resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMDITabs

CMDITabs::CMDITabs()
{
  m_mdiClient = NULL;
  m_minViews = 0;
  m_bImages = false;
  m_bTop    = false;
  m_stateBtn=0;
  m_bDrag= false;
  m_nItemDrag= -1;
  m_rectButton= CRect(0,0,0,0);
}

CMDITabs::~CMDITabs() {

	m_bFont.DeleteObject();
	m_bFontSel.DeleteObject();
}

BEGIN_MESSAGE_MAP(CMDITabs, CTabCtrl)
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, OnSelChange)
	ON_WM_PAINT()
	ON_WM_CONTEXTMENU()
	ON_WM_LBUTTONDBLCLK()
	ON_MESSAGE(WM_SIZEPARENT, OnSizeParent)
	ON_WM_ERASEBKGND()
	ON_WM_NCPAINT()
	ON_WM_NCMOUSEMOVE()
	ON_WM_TIMER()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_NCLBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_NCHITTEST()
	ON_WM_LBUTTONUP()
	ON_WM_CAPTURECHANGED()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMDITabs message handlers

afx_msg LRESULT CMDITabs::OnSizeParent(WPARAM, LPARAM lParam)
{
  if (GetItemCount() < m_minViews) 
  {
    ShowWindow(SW_HIDE);
  }
  else 
  {  
    AFX_SIZEPARENTPARAMS* pParams = reinterpret_cast<AFX_SIZEPARENTPARAMS*>(lParam);

    const int height = 22 + (m_bImages ? 1 : 0);
    const int offset = 2;

    m_height = height + offset;
    m_width  = pParams->rect.right - pParams->rect.left;	

    if (m_bTop)
    {
      pParams->rect.top += height;
      MoveWindow(pParams->rect.left, pParams->rect.top - height, m_width, m_height, true);
    }
    else
    {
      pParams->rect.bottom -= height;
      MoveWindow(pParams->rect.left, pParams->rect.bottom - offset, m_width, m_height, true);
    }
    ShowWindow(SW_NORMAL);
  }
  return 0;
}

void CMDITabs::OnSelChange(NMHDR* pNMHDR, LRESULT* pResult)
{
  ::BringWindowToTop(HWND(GetItemData(GetCurSel())));
  *pResult = 0;
}

void CMDITabs::Update()
{
  SetRedraw(false);

  HWND active = ::GetTopWindow(m_mdiClient); // get active view window (actually the frame of the view)

  typedef std::vector<HWND> TWndVec;
  typedef TWndVec::iterator TWndIter;

  TWndVec vChild; // put all child windows in a list (actually a vector)
  for (HWND child = active; child; child = ::GetNextWindow(child, GW_HWNDNEXT))
  {
    vChild.push_back(child);
  }

  TCITEM item;
  char text[256];
  item.pszText = text;

  for (int i = GetItemCount(); i--;)  // for each tab
  {
    item.mask = TCIF_PARAM;
    GetItem(i, &item);

    TWndIter it = std::find(vChild.begin(), vChild.end(), HWND(item.lParam));
    if (it == vChild.end()) // associatete view does no longer exist, so delete the tab
    {
      DeleteItem(i);
      if (m_bImages) RemoveImage(i);
    }
    else // update the tab's text, image and selection state
    {
      item.mask = TCIF_TEXT;
      ::GetWindowText(*it, text, 256);
      if (m_bImages) m_images.Replace(i, (HICON)::GetClassLong(*it, GCL_HICONSM));
      SetItem(i, &item);
      if (*it == active) SetCurSel(i); // associated view is active => make it the current selection
      vChild.erase(it);                // remove view from list
    }
  }

  // all remaining views in vChild have to be added as new tabs
  int i = GetItemCount();
  for (TWndIter it = vChild.begin(), end = vChild.end(); it != end; ++it)
  {
    item.mask = TCIF_TEXT|TCIF_PARAM|TCIF_IMAGE;
    ::GetWindowText(*it, text, 256);
    if (m_bImages) m_images.Add((HICON)::GetClassLong(*it, GCL_HICONSM));
    item.iImage = i;
    item.lParam = LPARAM(*it);
    InsertItem(i, &item);
    if (*it == active) SetCurSel(i);
    ++i;
  }

  // this removes the control when there are no tabs and shows it when there is at least one tab
  bool bShow = GetItemCount() >= m_minViews;
  if ((!bShow && IsWindowVisible()) || (bShow && !IsWindowVisible())) 
  {
    static_cast<CMDIFrameWnd*>(FromHandlePermanent(::GetParent(m_mdiClient)))->RecalcLayout();
  }
  RedrawWindow(NULL, NULL, RDW_FRAME|RDW_INVALIDATE|RDW_ERASE);
  SetRedraw(true);
}

void  CMDITabs::Drawtabs(CDC* dc)
{
	CPen light(PS_SOLID,1,GetSysColor(COLOR_BTNHIGHLIGHT));
	CPen Dark(PS_SOLID,1,GetSysColor(COLOR_3DSHADOW));

	CRect rectText;
	CFont* m_fontOld=dc->SelectObject(&m_bFont);
	CPen* oldPen= dc->SelectObject(&light);
	CString szStrPoints=_T("...");
	CRect itemrect;

	int witdh=m_width-24;

	if( ::IsWindow(m_wndSpin) && m_wndSpin.IsWindowVisible()) {
		m_wndSpin.GetWindowRect(itemrect);
        witdh-= itemrect.Width();
	}

	for (int iCont=0; iCont< GetItemCount();iCont++)
	{
		GetItemRect(iCont, itemrect);
		if( itemrect.right>witdh)
			itemrect.right=witdh;

		if (iCont ==GetCurSel())
		{
			if( itemrect.left<witdh) {
				CBrush cbr;	

				cbr.CreateSysColorBrush(COLOR_BTNFACE);

				dc->SelectObject(&m_bFontSel);

				itemrect.bottom+=5;
				itemrect.top++;
				dc->FillRect(itemrect,&cbr);

				dc->MoveTo(itemrect.left,itemrect.top-3);
				dc->LineTo(itemrect.left,itemrect.bottom-1);

				CPen* OldPen=dc->SelectObject(&Dark);
				dc->MoveTo(itemrect.left,itemrect.top-1);
				dc->LineTo(itemrect.right,itemrect.top-1);
				////linea derecha observe que se pinta dos veces para
				////dar el efecto de redondeada en la puntas
				dc->MoveTo(itemrect.right-1,itemrect.top-1);
				dc->LineTo(itemrect.right-1,itemrect.bottom-1);
				dc->MoveTo(itemrect.right,itemrect.top-1);
				dc->LineTo(itemrect.right,itemrect.bottom-1);
				dc->MoveTo(itemrect.left-1,itemrect.top-1);
				dc->LineTo(itemrect.left-1,itemrect.bottom-1);

				dc->SelectObject(OldPen);
				cbr.DeleteObject();
				itemrect.top-=5;
			}
		} else if ((iCont+1) != GetCurSel()) {

			CPen* pOldPen=dc->SelectObject(&Dark);
			dc->MoveTo(itemrect.right-1,itemrect.top+2);
			dc->LineTo(itemrect.right-1,itemrect.bottom-2);
			dc->SelectObject(pOldPen);
			dc->SelectObject(&m_bFont);
		} else dc->SelectObject(&m_bFont);
		//se dibuja el icono
		char texto[255];
		TCITEM item;
		item.mask = TCIF_TEXT | TCIF_IMAGE;
		item.pszText= texto;
		item.cchTextMax= 255;

		GetItem(iCont, &item);

		int nMode = dc->SetBkMode(TRANSPARENT);
		CSize m_sChar=dc->GetTextExtent(item.pszText,strlen(item.pszText));
		COLORREF clrtext;
		if (iCont !=GetCurSel())
			clrtext=dc->SetTextColor( GetSysColor(COLOR_GRAYTEXT));
		
		if (m_sChar.cx < itemrect.Width()/*-8*/)
		{
			/*Image.Draw(dc,item.iImage,CPoint(itemrect.left+2,itemrect.top+2),ILD_TRANSPARENT);*/
			// TODO: mirar si puedo colocar iconos aqui.
			rectText=itemrect;
			/*rectText.left+=8;
			rectText.right-=2;*/
			dc->DrawText(item.pszText,rectText,DT_SINGLELINE|DT_CENTER|DT_VCENTER);
		} else {
			//verificamos si se puede poner algun texto o
			//por lo memos el icono
			if (0< itemrect.Width())
			{
				/*Image.Draw(dc, item.iImage,CPoint(itemrect.left+2,itemrect.top+2),ILD_TRANSPARENT);*/
				rectText=itemrect;
				rectText.left+=8;
				rectText.right-=5;
				CString m_cadBreak=item.pszText;
				int c=0;
				int cont=m_cadBreak.GetLength();
				while(cont > 1 )
				{
					CString m_scadtemp=m_cadBreak+szStrPoints;
					CSize coor=dc->GetTextExtent(m_scadtemp,m_scadtemp.GetLength());
					if(coor.cx > rectText.Width())
						m_cadBreak=m_cadBreak.Left(m_cadBreak.GetLength()-1);
					else
						break;
					cont--;

				}
				m_cadBreak+=szStrPoints;
				rectText.right+=3;
				dc->DrawText(m_cadBreak,rectText,DT_SINGLELINE|DT_LEFT|DT_VCENTER);
			}
		}
		if (iCont !=GetCurSel())
			dc->SetTextColor(clrtext);
		dc->SetBkMode(nMode);
	}
	dc->SelectObject(oldPen);
	dc->SelectObject(m_fontOld);
}

void CMDITabs::OnPaint()
{
	if (GetItemCount() <=0) return;
	CPaintDC dc(this);
	Drawtabs(&dc);
}

void CMDITabs::Create(CFrameWnd* pMainFrame, DWORD dwStyle)
{
  m_bTop = (dwStyle & MT_TOP);
  m_minViews = (dwStyle & MT_HIDEWLT2VIEWS) ? 2 : 1;

  CTabCtrl::Create(WS_CHILD|WS_VISIBLE|(m_bTop?0:TCS_BOTTOM)|TCS_SINGLELINE|TCS_FOCUSNEVER|TCS_FORCEICONLEFT|WS_CLIPSIBLINGS|WS_CLIPCHILDREN, CRect(0, 0, 0, 0), pMainFrame, 42);
  ModifyStyleEx(0, WS_EX_CLIENTEDGE);
  SendMessage(WM_SETFONT, WPARAM(GetStockObject(DEFAULT_GUI_FONT)), 0);

  HWND wnd ;
  for (wnd = ::GetTopWindow(*pMainFrame); wnd; wnd = ::GetNextWindow(wnd, GW_HWNDNEXT))
  {
    char wndClass[32];
    ::GetClassName(wnd, wndClass, 32);
    if (strncmp(wndClass, "MDIClient", 32) == 0) break;
  }
  m_mdiClient = wnd;

  ASSERT(m_mdiClient); // Ooops, no MDIClient window?

  // manipulate Z-order so, that our tabctrl is above the mdi client, but below any status bar
  ::SetWindowPos(m_hWnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
  ::SetWindowPos(m_mdiClient, m_hWnd, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
  m_bImages = (dwStyle & MT_IMAGES) != 0;
  if (m_bImages)
  {
    if (m_images.GetSafeHandle()) 
    {
      m_images.SetImageCount(0);
    }
    else    
    {
      m_images.Create(16, 16, ILC_COLORDDB|ILC_MASK, 1, 1);
    }
    SetImageList(&m_images);
  }
	m_bFont.CreateStockObject(DEFAULT_GUI_FONT);
	LOGFONT lf;
	m_bFont.GetLogFont(&lf);
	lf.lfWeight= FW_SEMIBOLD;
	m_bFontSel.CreateFontIndirect(&lf);

	m_CloseBtn.SetData(6,_T("Close"));
	m_CloseBtn.SetImageList(IDB_BITMAP1,9,10,RGB(255,0,255));
}

static CString GetShort1(UINT id)
{
	CString str;
	str.LoadString(id);
	int nIndex = str.ReverseFind(_T('\n'));
	if(nIndex!=-1)
	{
		str=str.Mid(nIndex+1);
	}
	return str;
}

void CMDITabs::SetSelEx( int sel) {

	if( sel>=0 && sel<GetItemCount()) {

		TCITEM item;
		item.mask = TCIF_PARAM;
		GetItem(sel, &item);

		HWND hWnd = HWND(item.lParam);
		SetCurSel(sel);
		::BringWindowToTop(hWnd);
	}
}

CString	CMDITabs::GetItemText(int sel) {

	CString temp;
	if( sel>=0 && sel<GetItemCount()) {

		TCITEM item;
		char texto[255];
		item.mask = TCIF_TEXT;
		item.pszText= texto;
		item.cchTextMax= 255;
		GetItem(sel, &item);
		temp= texto;
	}
	return temp;
}

void CMDITabs::OnContextMenu(CWnd* pWnd, CPoint point) 
{
  TCHITTESTINFO hit;
  hit.pt = point;
  ScreenToClient(&hit.pt);
  int i = HitTest(&hit);
  if (i >= 0) 
  {
    TCITEM item;
    item.mask = TCIF_PARAM;
    GetItem(i, &item);

    HWND hWnd = HWND(item.lParam);
    SetCurSel(i);
    ::BringWindowToTop(hWnd);


	CNewMenu menu;
	CNewMenu submenu,submenu1;
	menu.CreatePopupMenu();
	submenu.CreatePopupMenu();

	int pos=0;

	menu.InsertMenu(pos++, MF_BYPOSITION , ID_FILE_CLOSE, GetShort1(ID_FILE_CLOSE) + "\tCtrl+F4");
	if( GetItemCount()>1) {
		menu.InsertMenu(pos++, MF_BYPOSITION | MF_STRING, ID_WINDOW_CLOSE_ALL, GetItemCount()==2 ? "C&errar Otra" : "C&errar Otras");
	}
	menu.InsertMenu(pos++, MF_BYPOSITION|MF_SEPARATOR);
	menu.InsertMenu(pos++, MF_BYPOSITION, ID_WINDOW_CASCADE, GetShort1(ID_WINDOW_CASCADE));
	menu.InsertMenu(pos++, MF_BYPOSITION, ID_WINDOW_TILE_HORZ, GetShort1(ID_WINDOW_TILE_HORZ));
	if( GetItemCount()>1) {

		menu.InsertMenu(pos++, MF_BYPOSITION|MF_SEPARATOR);
		if( i!=GetItemCount()-1) {
			menu.InsertMenu(pos, MF_BYPOSITION, ID_NEXT_PANEEX, GetShort1(ID_NEXT_PANE)+ "\tAlt+Flecha Derecha");
			pos++;
		}
		if( i!=0)
			menu.InsertMenu(pos, MF_BYPOSITION, ID_PREV_PANEEX, GetShort1(ID_PREV_PANE)+ "\tAlt+Flecha Izquierda");
		
		for( int j=0; j<GetItemCount(); j++) {
			if( i==j)
				continue;
			CString temp = GetItemText(j);
			submenu.AppendMenu(MF_STRING,j+VENTANAS_PREFIJO,GetItemText(j));
		}
		menu.AppendMenu(MF_SEPARATOR);
		menu.AppendMenu(MF_POPUP, (UINT) submenu.GetSafeHmenu(), "&Ventanas");
	}
	menu.TrackPopupMenu( TPM_RIGHTBUTTON, point.x, point.y, AfxGetMainWnd());
  }
}

void CMDITabs::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
  int i = GetCurSel();
  if (i >= 0) 
  {
    HWND hWnd = HWND(GetItemData(i));
	WINDOWPLACEMENT wp;
	if( ::GetWindowPlacement(hWnd, &wp)) {
		if(wp.showCmd==SW_SHOWMAXIMIZED) {
			::ShowWindow(hWnd, SW_RESTORE);
		} else {
			::ShowWindow(hWnd, SW_MAXIMIZE);
		}
	} else ::ShowWindow(hWnd, SW_MAXIMIZE);
  }
}

void CMDITabs::DrawButton() {

	int alto= 14;
	CWnd *child= GetWindow(GW_CHILD);
	if( child) {
		if( !child->IsKindOf(RUNTIME_CLASS(CLBSpinButtonCtrl))) 
			m_wndSpin.SubclassWindow(child->GetSafeHwnd());
		CRect rect;
		child->GetWindowRect(rect);
		int width= rect.Width();
		if( rect.left!=m_width-23-width) {
			child->ScreenToClient(rect);
			rect.left= m_width-23-width;
			rect.right= rect.left+width;
			rect.top= (m_height-alto)/2-2;
			rect.bottom = rect.top + alto;
			child->MoveWindow(rect);
		}
	}
	m_rectButton.left= m_width-20;
	m_rectButton.right= m_rectButton.left+alto;
	m_rectButton.top= (m_height-alto)/2;
	m_rectButton.bottom = m_rectButton.top+alto;
	m_CloseBtn.Paint(GetWindowDC(),m_stateBtn,m_rectButton,HLS_TRANSFORM(::GetSysColor (COLOR_3DFACE), +60, 0));
}

BOOL CMDITabs::OnEraseBkgnd(CDC* pDC)
{
	CBrush cbr;

	CRect m_rect(0,0, m_width, m_height);
	cbr.CreateSolidBrush(HLS_TRANSFORM(::GetSysColor (COLOR_3DFACE), +60, 0));
	pDC->FillRect(m_rect,&cbr);
	cbr.DeleteObject();
	DrawButton();
	return TRUE;
}

void CMDITabs::OnNcPaint()
{
	HDC hdc = ::GetWindowDC(m_hWnd);

	CRect rect;
	CRect itemrect;

	rect.left = 0;
	rect.top = 0;
	rect.right = m_width;
	rect.bottom = m_height;

	int witdh=m_width-24;

	if( ::IsWindow(m_wndSpin) && m_wndSpin.IsWindowVisible()) {
		m_wndSpin.GetWindowRect(itemrect);
        witdh-= itemrect.Width();
	}

	GetItemRect(GetCurSel(), itemrect);

	if( itemrect.right>witdh)
		itemrect.right = witdh;

	HPEN pen = ::CreatePen(PS_SOLID, 0, ::GetSysColor(COLOR_3DFACE));
	HGDIOBJ old = ::SelectObject(hdc, pen);
	DrawEdge(hdc, rect, EDGE_SUNKEN, BF_LEFT|BF_RIGHT|BF_TOP);
	::MoveToEx(hdc, 2, m_height - 1, NULL);
	::LineTo(hdc, m_width - 2, m_height - 1);
	::MoveToEx(hdc, 2, m_height - 2, NULL);
	::LineTo(hdc, m_width - 2, m_height - 2);

	HPEN pen1 = ::CreatePen(PS_SOLID, 0, ::GetSysColor(COLOR_3DHIGHLIGHT));
	::SelectObject(hdc, pen1);
	::MoveToEx(hdc, 2, m_height - 1, NULL);

	if( itemrect.left< witdh) {	
		::LineTo(hdc, itemrect.left, m_height - 1);
		::MoveToEx(hdc, itemrect.right+1, m_height - 1, NULL);
	}
	::LineTo(hdc, m_width - 2, m_height - 1);

	::SelectObject(hdc, old);
	::DeleteObject(pen);
	::DeleteObject(pen1);
	::ReleaseDC(m_hWnd, hdc);	
}

void CMDITabs::OnNcMouseMove(UINT nHitTest, CPoint point)
{
	if (nHitTest == HTCLOSE)
		if (m_stateBtn != NORMAL) return;
	if (nHitTest == HTCLOSE)
	{
		m_stateBtn=OVER;
		SetTimer(1,100,0);
	}
	DrawButton();
	CTabCtrl::OnNcMouseMove(nHitTest, point);
}

void CMDITabs::OnTimer(UINT nIDEvent)
{
	if (nIDEvent == 1)
	{
		if (m_stateBtn==NORMAL) return;
		CRect rc;
		CPoint pt;
		GetCursorPos(&pt);

		CRect rcT=m_rectButton;
		ClientToScreen(rcT);
		if (!rcT.PtInRect(pt))
		{
			m_stateBtn=NORMAL;
			KillTimer(1);
		}
		DrawButton();
	}
	CTabCtrl::OnTimer(nIDEvent);
}

void CMDITabs::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	if( nHitTest == HTCLOSE)
	{
		m_stateBtn=PRESS;
		DrawButton();
		return;
	}
	CTabCtrl::OnNcLButtonDown(nHitTest, point);
}

DWORD CMDITabs::GetItemData(int index) {

	if( index<0 || index>=GetItemCount()) 
		return 0;
	TCITEM item;
	item.mask = TCIF_PARAM;
	GetItem(index, &item);
	return item.lParam;
}

void CMDITabs::OnNcLButtonUp(UINT nHitTest, CPoint point)
{
	CRect rcT=m_rectButton;
	ClientToScreen(rcT);
	if (rcT.PtInRect(point))
	{
		if (m_stateBtn ==PRESS)
		{
			m_stateBtn=NORMAL;
			KillTimer(1);
			int i= GetCurSel();
			if( i >=0) {
				CWaitCursor wait;
				HWND hWnd = HWND(GetItemData(i));
				::SendMessage( hWnd, WM_CLOSE, 0,0);
			}
		}
		DrawButton();
		return;
	}
	CTabCtrl::OnNcLButtonUp(nHitTest, point);
}
void CMDITabs::OnMouseMove(UINT nFlags, CPoint point)
{
	if( m_bDrag) {

		CRect rect;
		GetWindowRect(rect);
		ScreenToClient(rect);
		if( rect.PtInRect(point)) {

			SetCursor(::LoadCursor(NULL,IDC_ARROW));
			GetItemRect(m_nItemDrag,rect);
			if( rect.PtInRect(point))
				return;
			int nuevo=m_nItemDrag;
			CRect newrect;
			if( m_nItemDrag>0) {
				GetItemRect(m_nItemDrag-1, newrect);
				if( (rect.left-newrect.Width()/2)>point.x)
					m_nItemDrag--;
			} 
			if( m_nItemDrag<GetItemCount()-1 && nuevo==m_nItemDrag) {
				GetItemRect(m_nItemDrag+1, newrect);
				if((rect.right+newrect.Width()/2)<point.x)
					m_nItemDrag++;
			} 
			if(nuevo==m_nItemDrag)
				return;
			char texto[255], textonuevo[255];
			TCITEM item, itemnuevo;
			itemnuevo.mask= item.mask = TCIF_TEXT | TCIF_IMAGE | TCIF_PARAM;
			item.pszText= texto;
			itemnuevo.pszText= textonuevo;
			itemnuevo.cchTextMax=item.cchTextMax= 255;
			GetItem(nuevo, &itemnuevo);
			GetItem(m_nItemDrag, &item);
			SetItem(nuevo, &item);
			SetItem(m_nItemDrag, &itemnuevo);
			SetCurSel(m_nItemDrag);
			OnNcPaint();
		} else 
			SetCursor(::LoadCursor(NULL,IDC_NO));
	}
	CTabCtrl::OnMouseMove(nFlags, point);
}

void CMDITabs::OnLButtonDown(UINT nFlags, CPoint point)
{
	CTabCtrl::OnLButtonDown(nFlags, point);
	m_nItemDrag= GetCurSel();
	if(m_nItemDrag >= 0 && GetItemCount()>1) 
	{
		m_bDrag= true;
		SetCapture();
	} else m_nItemDrag=-1;
}

LRESULT CMDITabs::OnNcHitTest(CPoint point)
{
	CRect rcT=m_rectButton;
	ClientToScreen(rcT);
	if( rcT.PtInRect(point))
		return HTCLOSE;
	return CTabCtrl::OnNcHitTest(point);
}

void CMDITabs::OnLButtonUp(UINT nFlags, CPoint point)
{
	if( m_bDrag) {
		m_bDrag= false;
		m_nItemDrag= -1;
		ReleaseCapture();
		SetCursor(::LoadCursor(NULL, IDC_ARROW));
	}
	CTabCtrl::OnLButtonUp(nFlags, point);
}

void CMDITabs::OnCaptureChanged(CWnd *pWnd)
{
	if(m_bDrag) {
		ReleaseCapture();
		m_bDrag=false;
		m_nItemDrag= -1;
	}
	CTabCtrl::OnCaptureChanged(pWnd);
}
