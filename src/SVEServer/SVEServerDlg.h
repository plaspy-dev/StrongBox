// SVEServerDlg.h: archivo de encabezado
//

#pragma once
#include "afxwin.h"
# include "../conector/conector.h"
# include "../sve/FingerBitmap.h"
# include "../sve/fingerprint.h"

// Cuadro de diálogo de CSVEServerDlg
class CSVEServerDlg : public CDialog
{
	CConector	*m_pConector;
	CString		m_szCurrentID;
	CFingerPrint	*m_pFinger;
	DWORD			m_pTemporal;
// Construcción
public:
	CSVEServerDlg(CWnd* pParent = NULL);	// Constructor estándar

// Datos del cuadro de diálogo
	enum { IDD = IDD_SVESERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// Compatibilidad con DDX/DDV


// Implementación
protected:
	HICON m_hIcon;

	// Funciones de asignación de mensajes generadas
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CEdit m_EUsuario;
	CEdit m_EContrasena;
	CStatic m_SEstado;
	CEdit m_EServidor;
	CEdit m_EPuerto;
	CEdit m_EBDatos;
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	LRESULT OnMysqlMessage(WPARAM wParam, LPARAM lParam);
	CFingerBitmap m_SHuella;
};
