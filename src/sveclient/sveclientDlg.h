// sveclientDlg.h: archivo de encabezado
//

#pragma once
#include "afxwin.h"
# include "../conector/conector.h"
# include "../sve/FingerBitmap.h"
# include "../sve/fingerprint.h"
# include "dlgpopup.h"
#include <Componentes/MatrixStatic.h>
#include <Componentes/akTrayIcon.h>
# include "DisplayThread.h"

// Cuadro de diálogo de CsveclientDlg
class CsveclientDlg : public CDialog
{
	CString			m_szTexto;
	int				m_nEstado;
	CDisplayThread	m_pDisplay;
	CConector		*m_pConector;
	CString			m_szCurrentID;
	CFingerPrint	*m_pFinger;
	DWORD			m_pTemporal;
	bool			m_bAutonomo;
	CString			m_szLastSearch;
	int				m_nReintentos;
	CDlgPopUp		m_pDlgPopUp;
	bool			m_bSalida;
	CString			m_szLastUsuario;
	CString			m_szLastHora;
public:
	CsveclientDlg(CWnd* pParent = NULL);	// Constructor estándar

// Datos del cuadro de diálogo
	enum { IDD = IDD_SVECLIENT_DIALOG };

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
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	LRESULT OnMysqlMessage(WPARAM wParam, LPARAM lParam);
	LRESULT OnFingerMessage(WPARAM wParam, LPARAM lParam);
	CFingerBitmap m_SHuella;
	CakTrayIcon m_TrayIcon;
public:
	CButton m_BAutonomo;
public:
	afx_msg void OnBnClickedCautonomo();
public:
	CMatrixStatic m_SMatriz;
	void SetMatrizTexto(CString texto, int estilo, CString linea2="");
	LRESULT OnTrayIconNotify(WPARAM wParam, LPARAM lParam);
	void OnConfigurar();
	void OnSalir();
	void OnAcercaDe();
public:
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void Reset();
};
