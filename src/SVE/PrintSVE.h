#pragma once

#define MARGIN_LEFT     20
#define MARGIN_TOP      15
#define MARGIN_RIGHT    20
#define MARGIN_BOTTOM   15
#define HEADER_LINES   2
#define FOOTER_LINES   2
#define HEADER_LINE_SIZE  5
#define FOOTER_LINE_SIZE  5

class CPrintSVE
{
// Construction
public:
   CPrintSVE();
   virtual ~CPrintSVE();

// Operations
public:
	BOOL OnPreparePrinting(CPrintInfo* pInfo, bool first=true);
	void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	void SetText(CString texto, CString texto2);

// Implementation
public:
	HBITMAP		m_hBitmap;
	BYTE		*m_pBitmap[3];
	BITMAPINFO	*m_pInfo[3];
	CSize		m_pSize[3];
	void SetDocTitle(const CString& strName);
	void SetAppName(const CString& strName);
	void SetFooter(const CString& strFooter);
	void SetFooterIzq(const CString& strFooter);
	void SetFinal(const CString& strFooter);
   void SetMarginLeftPix(int leftMargin){ m_marginLeft = leftMargin; }
   int GetMarginLeftPix(){ return m_marginLeft; }
   void SetMarginTopPix(int topMargin){ m_marginTop = topMargin; }
   int GetMarginTopPix(){ return m_marginTop; }
   void SetMarginRightPix(int rightMargin){ m_marginRight = rightMargin; }
   int GetMarginRightPix(){ return m_marginRight; }
   void SetMarginBottomPix(int bottomMargin){ m_marginBottom = bottomMargin; }
   int GetMarginBottomPix(){ return m_marginBottom; }
   void SetHeadLines(int lines){ m_headerLines = lines; }
   int GetHeadLines(){ return m_headerLines; }
   void SetFootLines(int lines){ m_footerLines = lines; }
   int GetFootLines(){ return m_footerLines; }

protected:
#ifdef _DEBUG
   void DrawRect(CDC* pDC, CRect rect, COLORREF color);
#endif
	void PrintFooter(CDC* pDC, CPrintInfo* pInfo);
   void PrintBody(CDC* pDC, CPrintInfo* pInfo);
	void PrintHeader(CDC* pDC, CPrintInfo* pInfo);
   CFont* CreateFont(CDC* pDC, CString strName = "", int nPoints = 0, int lfWeight = FW_NORMAL, bool lfItalic = false);
   CRect GetBodyRect();
   CRect GetPaperRect(CDC* pDC);
   CRect GetPageRect();
   CRect GetHeaderRect();
   CRect GetFooterRect();
   CSize GetCharSize(CDC* pDC, CFont* pFont);
   int m_nCellPadding;

private:
	CString m_szTexto;
	CString m_szTexto2;
   CString m_strAppName;
   CString m_strDocName;
   CString m_strFooter;
   CString m_strFooterIzquierdo;
   CString m_strFinal;
   CRect m_rectHeader;
   CRect m_rectFooter;
   CRect m_rectBody;
   CRect m_rectPage;
	CRect m_rectPaper;
   CFont* m_pFontHeader;
   CFont* m_pFontFooter;
   CFont* m_pFontColumn;
   CFont* m_pFontBody;
   CSize m_CharSizeHeader;
   CSize m_CharSizeFooter;
   CSize m_CharSizeBody;
   CSize m_pixelpantalla;
   int m_marginLeft;
   int m_marginTop;
   int m_marginRight;
   int m_marginBottom;
   int m_headerHeight;
   int m_headerLines;
   int m_footerHeight;
   int m_footerLines;
   int m_nPageCount;
   int m_nRowCount;
   int m_nEspaciado;
};