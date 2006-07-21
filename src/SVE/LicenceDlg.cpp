// LicenceDlg.cpp: archivo de implementación
//

#include "stdafx.h"
# include <resource.h>
#include "LicenceDlg.h"
# include "../conector/defender.h"
# include "../iofile/myregkey.h"

// Cuadro de diálogo de CLicenceDlg

IMPLEMENT_DYNAMIC(CLicenceDlg, CDialogXP)
CLicenceDlg::CLicenceDlg(CWnd* pParent /*=NULL*/)
	: CDialogXP(CLicenceDlg::IDD, pParent)
{
}

CLicenceDlg::~CLicenceDlg()
{
}

void CLicenceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogXP::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ESEMILLA, m_ESemilla);
	DDX_Control(pDX, IDC_ESERIAL, m_ESerial);
	DDX_Control(pDX, IDOK, m_BAceptar);
	DDX_Control(pDX, IDCANCEL, m_BCancelar);
}


BEGIN_MESSAGE_MAP(CLicenceDlg, CDialogXP)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()


// Controladores de mensajes de CLicenceDlg

void CLicenceDlg::OnBnClickedOk()
{
#ifdef ENABLE_LICENSE_CHECK
	CString temp;
	m_ESerial.GetWindowText(temp);
	CDefender info(temp);
	if(info.IsValidSerial()==&info) {
		CMyRegKey key;
		key.MyOpen(HKEY_LOCAL_MACHINE, "SoftWare\\SDG\\SDGFingerSB\\control");
		key.WriteString("licencia", "", temp);
		MessageBox("Gracias por utilizar SDGFinger StrongBox.");
		OnOK();
	} else {
		CWaitCursor wait;
		Sleep(5000);
		MessageBox("El serial introducido no es valido.\nPor favor consulte a su proveedor.", NULL, MB_ICONERROR);
	}
#else
	OnOK();
#endif
}

void CLicenceDlg::OnBnClickedCancel()
{
	OnCancel();
}

BOOL CLicenceDlg::OnInitDialog()
{
	CDialogXP::OnInitDialog();
	CDefender info;
	m_ESemilla.SetWindowText(info.GetKernel());
	return TRUE;
}
