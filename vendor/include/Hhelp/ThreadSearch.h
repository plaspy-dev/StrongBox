#pragma once
# include <Conector/Conector.h>

class __HHELP_EXPORT__ CThreadSearch : public CThreadAuto
{
protected:
	virtual bool	Init();
	CStringArray	m_saTodas;
	CString			m_szAppPath;
public:
	CMiscelaneosEx *m_pArchivos;
	CWnd			*m_pParent;
	CMyListCtrl		*m_pList;
	CString			m_szCadena;
	CThreadSearch(void);
	~CThreadSearch(void);
};
