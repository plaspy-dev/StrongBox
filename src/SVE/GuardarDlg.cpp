// GuardarDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SVE.h"
#include "GuardarDlg.h"


// CGuardarDlg dialog

IMPLEMENT_DYNAMIC(CGuardarDlg, CDialog)

CGuardarDlg::CGuardarDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGuardarDlg::IDD, pParent)
{
	m_nCampos=0;
}

CGuardarDlg::~CGuardarDlg()
{
}

void CGuardarDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CUSUARIO, m_CUsuario);
	DDX_Control(pDX, IDC_CDIAS, m_CDias);
	DDX_Control(pDX, IDC_CHORAS, m_CHoras);
}


BEGIN_MESSAGE_MAP(CGuardarDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CGuardarDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CGuardarDlg message handlers

BOOL CGuardarDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_CUsuario.SetCheck(AfxGetApp()->GetProfileInt("guardar", "usuario", 1));
	m_CDias.SetCheck(AfxGetApp()->GetProfileInt("guardar", "dias", 1));
	m_CHoras.SetCheck(AfxGetApp()->GetProfileInt("guardar", "horas", 1));

	return TRUE;
}

void CGuardarDlg::OnBnClickedOk()
{
	if( m_CUsuario.GetCheck()==0 && m_CDias.GetCheck()==0 && m_CHoras.GetCheck()==0) {
		MessageBox("Por favor seleccione un campo.");
		return;
	}
	AfxGetApp()->WriteProfileInt("guardar", "usuario", m_CUsuario.GetCheck());
	AfxGetApp()->WriteProfileInt("guardar", "dias", m_CDias.GetCheck());
	AfxGetApp()->WriteProfileInt("guardar", "horas", m_CHoras.GetCheck());

	m_nCampos=0;
	m_nCampos|=m_CUsuario.GetCheck();
	m_nCampos|=2*m_CDias.GetCheck();
	m_nCampos|=4*m_CHoras.GetCheck();

	OnOK();
}
