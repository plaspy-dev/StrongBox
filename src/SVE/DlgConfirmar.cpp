// DlgConfirmar.cpp : implementation file
//

#include "stdafx.h"
#include "SVE.h"
#include "DlgConfirmar.h"


// CDlgConfirmar dialog

IMPLEMENT_DYNAMIC(CDlgConfirmar, CDialog)

CDlgConfirmar::CDlgConfirmar(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgConfirmar::IDD, pParent)
{
	m_SHuellaOld=NULL;
	m_pFinger=NULL;
	m_pOldParent=NULL;
}

CDlgConfirmar::~CDlgConfirmar()
{
}

void CDlgConfirmar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SHUELLA, m_SHuella);
	DDX_Control(pDX, IDC_SMENSAJE, m_SMensaje);
	DDX_Control(pDX, IDCANCEL, m_BCancelar);
}


BEGIN_MESSAGE_MAP(CDlgConfirmar, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgConfirmar::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgConfirmar::OnBnClickedCancel)
	ON_MESSAGE(WM_FINGERPRINT,OnFingerPrint)
END_MESSAGE_MAP()


// CDlgConfirmar message handlers

void CDlgConfirmar::OnBnClickedOk()
{
	// OnOK();
}

void CDlgConfirmar::OnBnClickedCancel()
{
	m_pFinger->SetPadre(m_pOldParent);
	m_pFinger->SetBitmap(m_SHuellaOld);
	OnCancel();
}

BOOL CDlgConfirmar::OnInitDialog()
{
	CDialog::OnInitDialog();
	ASSERT(m_pFinger);
	if(m_pFinger==NULL)
		return FALSE;
	VERIFY(m_pFinger->Prepare());
	m_SHuella.HideInfo();
	m_SHuellaOld=m_pFinger->SetBitmap(&m_SHuella);
	m_pOldParent= m_pFinger->SetPadre(this);
	return TRUE;
}

LRESULT CDlgConfirmar::OnFingerPrint(WPARAM wParam, LPARAM lParam) {

	if( lParam==(LPARAM)&m_SHuella) {
		int score=0;
		if(m_pFinger->Compare((const char *)m_SHuella.m_cTemplate, &score)) {
			m_SMensaje.SetWindowText("Huella confirmada.");
			m_pFinger->SetPadre(m_pOldParent);
			m_pFinger->SetBitmap(m_SHuellaOld);
			OnOK();
			return 0;
		}
		m_SMensaje.SetWindowText("Por favor intente\r de nuevo.");
	}
	return 0;
}