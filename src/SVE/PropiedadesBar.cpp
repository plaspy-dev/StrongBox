#include "StdAfx.h"
#include "propiedadesbar.h"
# include "SVE.h"
# include "../iofile/iofile.h"
# include "../componentes/editfloat.h"

CPropiedadesBar::CPropiedadesBar(void)
{
	m_pConector= NULL;
	m_bUpdate= false;
	m_bODS= true;
}

CPropiedadesBar::~CPropiedadesBar(void)
{

}


BEGIN_MESSAGE_MAP(CPropiedadesBar, CGuiControlBar)
ON_WM_CREATE()
ON_WM_SIZE()
ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

void CPropiedadesBar::SetODS(CString NoODS, bool nueva) {

	if( nueva)
		m_szOld.Empty();
	if( m_szOld== NoODS)
		return;
	m_szOld= NoODS;
	m_bUpdate= true;
	Update();
}

void CPropiedadesBar::Update(bool Force) {

	if( !Force)
		if(!IsWindowVisible())
			return;
	m_bUpdate= false;
	CString query;
	query.Format("SELECT propiedades.codigo,shared.nombre,propiedades.fecha FROM propiedades LEFT JOIN shared ON shared.id=propiedades.usuario WHERE codigo=%s",m_szOld);
	if( m_pConector) {
		for( int i=0; i<m_Lista.GetItemCount(); i++)
			m_Lista.SetItemText(i, 1, "");
		m_pConector->Query(query, 0, this);
	}
}

void CPropiedadesBar::SetPropiedades(CString propiedades, bool nueva) {

	if( nueva)
		m_szOld.Empty();
	if( m_szOld== propiedades)
		return;
	m_szOld= propiedades;
	m_bUpdate= true;
	Update();
}

void CPropiedadesBar::SetConector( CConector *conector) {

	m_pConector= conector;
}

void CPropiedadesBar::ScrollOn() {

	CGuiControlBar::ScrollOn();
	if(m_bUpdate) 
		Update(true);
}

void CPropiedadesBar::SaveBar(CString sProfile) {

	CGuiControlBar::SaveBar(sProfile);
	m_Lista.StoreState("Listas\\Lista");
}

int CPropiedadesBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CGuiControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	m_Lista.Create(WS_VISIBLE | WS_BORDER | WS_CHILD | LVS_REPORT, CRect(0,0,0,0), this, 201);
	m_Lista.InsertColumn(0, "Atributo", LVCFMT_LEFT, 50);
	m_Lista.InsertColumn(1, "Valor", LVCFMT_LEFT, 50);
	m_Lista.SetExtendedStyle(LVS_EX_FULLROWSELECT| LVS_EX_GRIDLINES);
	m_Lista.SetOrdenable(true);
	m_Lista.SetCanShowMenu();
    
	m_Lista.InsertItem(0, "Documento");
	m_Lista.SetItemData(0,0);
	m_Lista.InsertItem(1, "Usuario");
	m_Lista.SetItemData(1,1);
	m_Lista.InsertItem(2, "Creación");
	m_Lista.SetItemData(2,2);
	
	m_Lista.RestoreState("Listas\\Lista");
	return 0;
}

LRESULT CPropiedadesBar::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message) {
		case WM_MYSQL:
			if( wParam==QUERY_RESULT) {

				MySQLMessage *mess= (MySQLMessage *)lParam;
				if( mess->GetID()==0) {
					for( int j=0; j<mess->GetMensaje()->GetCount(); j++)
						for( int i=0; i<m_Lista.GetItemCount(); i++) {
							if( m_Lista.GetItemData(i)==j) { // found
								m_Lista.SetItemText(i, 1, mess->GetAt(j));
								break;
							}
						}
				}
			} else if( wParam==QUERY_EMPTY) {
				MySQLMessage *mess= (MySQLMessage *)lParam;
				for( int i=0; i<m_Lista.GetItemCount(); i++) {
					if( mess->GetID()==m_Lista.GetItemData(i)) {
						m_Lista.SetItemText(i, 1, "");
						break;
					}
				}
				break;
			}
			return 0;
	}
	return CGuiControlBar::WindowProc(message, wParam, lParam);
}

void CPropiedadesBar::OnSize(UINT nType, int cx, int cy)
{
	CGuiControlBar::OnSize(nType, cx, cy);
}

void CPropiedadesBar::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CGuiControlBar::OnShowWindow(bShow, nStatus);
	if(bShow)
		Update(true);
}
