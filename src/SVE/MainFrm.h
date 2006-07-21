// MainFrm.h: interfaz de la clase CMainFrame
//
#pragma once

#include "mybar.h"
#include "../conector/conector.h"
#include "../conector/usuario.h"
# include "../componentes/newmenu/mdiframewndxp.h"
# include "../componentes/newmenu/multidoctemplatexp.h"
# include "../componentes/toolbarxp.h"
#include "../componentes/FadeWnd.h"
#include "../componentes/guidocktoolbarex.h"
# include "miscelaneosex.h"
# include "PropiedadesBar.h"

class CMainFrame : public CMDIFrameWndXP
{
	DECLARE_DYNAMIC(CMainFrame)
	// implementado para tener mas dinamismo en los archivos
	CStrongBoxMiscelaneosEx	m_pMenus;
	CStrongBoxMiscelaneosEx  m_pAceleradores;

private:

	// varios
	bool	Messages( MySQLMessage *contenido);
	bool	DeleteHilos( CThreadMySQL *hilo);
	bool	LoadDocumentos();
	int		m_nCurrentHilos;
	int		m_nAnimation;
	ULONGLONG		m_ulTotal;
	ULONGLONG		m_ulRealizados;

	bool			AddAcelerator(ACCEL nueva);
	CTime			m_pTLocal, m_pTServidor;
	CTimeSpan		m_psDiferencia;
	// id del usuario de la tabla chat
	bool			m_bConectando;
	bool			LoadDocument( CNewMenu *menu, HTREEITEM hRoot, CString nombre, UINT nIDResource, CRuntimeClass* pViewClass);
	CFadeWnd		*m_pFadeWnd;
protected:
	CToolBarXP		m_wndToolBar;
// Reemplazos
public:
	bool			m_bInactivo;
	bool			m_bFocus;
	int				m_nInactivo;
	void			DockControlBarLeftOf(CToolBar* Bar, CToolBar* LeftOf);
	bool			Conectar();
	bool			Registrar();
	void			Aldia();
	CString			m_szPathUpdate;
	DWORD			m_nTipoActualizacion;
	CString			m_szFullPath;
	CUsuario		*m_pUsuario;
	void			ResetToolbars();
	CPropiedadesBar	m_PropBar;

	CConector		m_pConector;

	CMyBar2			m_MyBar2;
	virtual	void OnCreateChild(HMENU menu);

	void	SaveState();
	void	LoadState();

	CMainFrame();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
	DECLARE_JDOCK()
	virtual void OnUpdateFrameTitle(BOOL bAddToTitle);
	void SendMessageBroadCast(UINT message,WPARAM wParam=0, LPARAM lParam=0);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnClose();
	afx_msg void OnVerNavegador();
	afx_msg void OnUpdateVerNavegador(CCmdUI *pCmdUI);
	afx_msg void OnConfiguracionServidores();
	afx_msg void OnVerBarradenavegacion();
	afx_msg void OnUpdateVerBarradenavegacion(CCmdUI *pCmdUI);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnEditPaste();
	afx_msg void OnEditCopy();
	afx_msg void OnEditCut();
	afx_msg void OnEditUndo();
	afx_msg void OnEditClear();
	afx_msg void OnEditSelectAll();
	afx_msg void OnUpdateEditPaste(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditUndo(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditCopy(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditCut(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditClear(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditSelectAll(CCmdUI *pCmdUI);
	afx_msg void OnNextPane();
	afx_msg void OnUpdateNextPane(CCmdUI *pCmdUI);
	afx_msg void OnPrevPane();
	afx_msg void OnUpdatePrevPane(CCmdUI *pCmdUI);
	afx_msg LRESULT OnQueryTime(WPARAM wParam, LPARAM lParam);
	afx_msg void OnActivateApp(BOOL bActive, DWORD dwThreadID);
	afx_msg void OnUpdateFileNew(CCmdUI *pCmdUI);
	afx_msg LRESULT OnStatusBar(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTreeNavegador(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnQueryConector(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetStatus(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnThreadResult(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnQueryUsuario(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMySQL(WPARAM wParam, LPARAM lParam);
	int OnMySQLThreadEnd(CThreadMySQL *hilo);
	int OnMySQLQueryResult(MySQLMessage *message);
	int OnQueryEmpty(int owner);
	int OnQueryTitle(MySQLMessage *message);
	void OnUpdateMenus(CCmdUI *pCmdUI);
	void OnCommandMenu(UINT id);
	CString ParseAcelerator(CString label, int id);
	void OnUpdateVentanas(CCmdUI *pCmdUI);
    void OnCommandVentanas(UINT id);
	afx_msg void OnCloseAll();
	afx_msg LRESULT OnInsertODS(WPARAM wParam, LPARAM lParam);
	void OnVerPropiedades();
	void OnUpdateVerPropiedades(CCmdUI *pCmdUI);
};
