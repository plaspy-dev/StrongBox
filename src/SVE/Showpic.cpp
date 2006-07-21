//////////////////////////////////////////////////////////////////////
//
//  This class is designed to provide simple interface for 
//  simultaneous Video Capture & Preview using DirectShow
//
//////////////////////////////////////////////////////////////////////
//
//	References: MS DirectShow Samples
//
//		
//////////////////////////////////////////////////////////////////////
//
//	This class was written by Sagar K.R . 
//  Use of this class is not restricted in any
//	way whatsoever.Please report the bugs to krssagar@firsteccom.co.kr
//
//	Special thanks to all the members at The Code Project! 
//	(www.codeproject.com)
//
//////////////////////////////////////////////////////////////////////
// Showpic.cpp : implementation file
//

#include "stdafx.h"
#include "Showpic.h"
# include "../iofile/xunzip.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShowpic

CShowpic::CShowpic()
{
	m_un32ImageWidth=AfxGetApp()->GetProfileInt("bitmap", "ancho", 320);
	m_un32ImageHeight=AfxGetApp()->GetProfileInt("bitmap", "alto", 240);
	m_un32ImageBits=24;
	m_pImageBytes=NULL;
	m_hBitmap=NULL;
	MakeBMPHeader();
	m_pBytes=NULL;
}

CShowpic::~CShowpic()
{
	if(m_pBmp)
	delete []m_pBmp;
	m_pBmp=NULL;
	if (m_hBitmap) ::DeleteObject(m_hBitmap);
	if( m_pBytes) {
		delete m_pBytes;
		m_pBytes=NULL;
	}
}


BEGIN_MESSAGE_MAP(CShowpic, CStatic)
	//{{AFX_MSG_MAP(CShowpic)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShowpic message handlers

void CShowpic::MakeBMPHeader(int alto,int ancho, int bits) {
	if(m_pBmp)
		delete m_pBmp;
	m_pBmp=NULL;
	m_un32ImageHeight= alto;
	m_un32ImageBits= bits;
	m_un32ImageWidth= ancho;
	MakeBMPHeader();
	Invalidate(false);
}

CSize CShowpic::GetTamano() {

	return CSize(m_un32ImageWidth, m_un32ImageHeight);
}

void CShowpic::MakeBMPHeader()
{	

	DWORD  dwBitmapInfoSize;
	dwBitmapInfoSize = sizeof(BITMAPINFO);
	m_pBmp    = (BITMAPINFO *)new BYTE [dwBitmapInfoSize];

	m_pBmp->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    m_pBmp->bmiHeader.biWidth = m_un32ImageWidth;
    m_pBmp->bmiHeader.biHeight = m_un32ImageHeight*-1;	
    m_pBmp->bmiHeader.biPlanes = 1;
    m_pBmp->bmiHeader.biBitCount =(unsigned short) m_un32ImageBits;
    m_pBmp->bmiHeader.biCompression = BI_RGB;
    m_pBmp->bmiHeader.biSizeImage = 0;
    m_pBmp->bmiHeader.biXPelsPerMeter = 0;
    m_pBmp->bmiHeader.biYPelsPerMeter = 0;
    m_pBmp->bmiHeader.biClrUsed = 0;
    m_pBmp->bmiHeader.biClrImportant = 0;

	if( m_un32ImageBits==1) {

#ifndef _DEBUG
		m_pBmp->bmiColors[0].rgbBlue=255;
		m_pBmp->bmiColors[0].rgbGreen=255;
		m_pBmp->bmiColors[0].rgbRed=255;
# endif
	}
}

bool CShowpic::HasImage() {

	return m_hBitmap || m_pImageBytes;
}

void CShowpic::SetWindowText(LPCTSTR lpszString) {
	CStatic::SetWindowText(lpszString);
	Invalidate();
}

void CShowpic::OnPaint() 
{
	CRect rect;
    GetClientRect(rect);
	CPaintDC dc(this); // device context for painting

    // If no bitmap selected, simply erase the background as per normal and return
    if (!m_hBitmap)
    {
		// TODO: Add your message handler code here
		if(m_pImageBytes!=NULL)
		{
			dc.RealizePalette();
			SetStretchBltMode(dc.GetSafeHdc(), COLORONCOLOR);

			int nResult = ::StretchDIBits(
									dc.GetSafeHdc(),
									rect.right-rect.left,
									rect.top,
									-rect.right-rect.left,
									rect.bottom-rect.top,
									0,
									0,
									m_un32ImageWidth, 
									m_un32ImageHeight,
									m_pImageBytes, 
									m_pBmp,
									DIB_RGB_COLORS,
									SRCCOPY);
		} else {
			Default();
		}
		return;
	} else if( m_hBitmap) {

		// We have a bitmap - draw it.

		// Create compatible memory DC using the controls DC
		CDC dcMem;
		VERIFY( dcMem.CreateCompatibleDC(&dc));
	    
		// Select bitmap into memory DC.
		HBITMAP* pBmpOld = (HBITMAP*) ::SelectObject(dcMem.m_hDC, m_hBitmap);

		dc.SetStretchBltMode(COLORONCOLOR);

		// StretchBlt bitmap onto static's client area
		dc.StretchBlt(rect.left, rect.top, rect.Width(), rect.Height(), 
						&dcMem, 0, 0, m_bmInfo.bmWidth-1, m_bmInfo.bmHeight-1,
						SRCCOPY);
		::SelectObject(dcMem.m_hDC, pBmpOld);
	}
}

BOOL CShowpic::SetBitmap(HBITMAP hBitmap)
{
	if( m_pBytes) {
		delete m_pBytes;
		m_pBytes=NULL;
	}
	m_pImageBytes=NULL;
    ::DeleteObject(m_hBitmap);
    m_hBitmap = hBitmap;
	bool retorno=false;
	if( m_hBitmap)
		retorno= ::GetObject(m_hBitmap, sizeof(BITMAP), &m_bmInfo);
	Invalidate();
	return retorno;
}

void CShowpic::ShowImage(BYTE *pImageByte)
{
	if(pImageByte!=NULL)
	{
		if (m_hBitmap) ::DeleteObject(m_hBitmap);
		m_hBitmap=NULL;
		m_pImageBytes=pImageByte;
		this->Invalidate();
	}
}

void CShowpic::GetBitmap(CBitmap *bitmap) {

	CWindowDC dc(this);
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	CRect rect;
	GetWindowRect(rect);
	bitmap->CreateCompatibleBitmap(&memDC, rect.Width(), rect.Height());
    
	CBitmap* pOldBitmap = memDC.SelectObject(bitmap);
	memDC.BitBlt(0, 0, rect.Width(),rect.Height(), &dc, 0, 0, SRCCOPY);
	memDC.SelectObject(pOldBitmap);
}

bool CShowpic::SetBitmap(CByteArray &pImage) {

	if(pImage.IsEmpty())
		return false;
	m_pImageBytes=NULL;
	m_pBytes= new BYTE[pImage.GetCount()];
	memcpy(m_pBytes, pImage.GetData(), pImage.GetCount());
	ShowImage(m_pBytes);
	return true;
}
