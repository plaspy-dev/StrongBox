#pragma once
#include "FormHelp.h"


class __HHELP_EXPORT__ CHelpBar : public CGuiControlBar
{
protected:
	CMultiDocTemplateXP* m_pDocTemplate;
public:
	CHelpBar(void);
	~CHelpBar(void);

	CFormHelp	m_pDlgHelp;
	CMultiDocTemplateXP *GetHelpDoc();
	CString		FindTitulo(const CString &titulo);

	virtual void ScrollOn();
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
