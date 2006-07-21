// NoHabilesView.cpp : implementation file
//

#include "stdafx.h"
#include "SVE.h"
#include "NoHabilesView.h"


// CNoHabilesView

IMPLEMENT_DYNCREATE(CNoHabilesView, CFormView)

CNoHabilesView::CNoHabilesView()
	: CFormView(CNoHabilesView::IDD)
{

}

CNoHabilesView::~CNoHabilesView()
{
}

void CNoHabilesView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CNoHabilesView, CFormView)
END_MESSAGE_MAP()


// CNoHabilesView diagnostics

#ifdef _DEBUG
void CNoHabilesView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CNoHabilesView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CNoHabilesView message handlers
