#pragma once
#include "../componentes/GuiControlBar.h"
# include "..\componentes\componentes.h"
# include "../conector/conector.h"
# include "mytime.h"

class CPropiedadesBar: public CGuiControlBar
{
	CStringArray	m_pArrayQuerys;
	CUIntArray		m_pArrayInt;
	CConector		*m_pConector;
	bool			m_bUpdate;
	CMyListCtrl		m_Lista;
	CString			m_szOld;
	CStrongBoxMyTime			m_pTiempo;
	bool			m_bODS;
public:
	void			SetODS(CString NoODS, bool nueva=false);
	void			SetPropiedades(CString propiedades, bool nueva=false);
	void			SetConector( CConector *conector);
	void			Update(bool Force=false);
	virtual void SaveBar(CString sProfile);
	CPropiedadesBar(void);
	~CPropiedadesBar(void);
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void ScrollOn();
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
