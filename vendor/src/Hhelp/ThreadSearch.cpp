#include "StdAfx.h"
#include "threadsearch.h"


CThreadSearch::CThreadSearch(void)
{
	m_pParent= NULL;
	m_pList= NULL;
	m_szAppPath= CMyRegKey::GetProfileString("ayuda", "ruta", "ayuda\\");
}

CThreadSearch::~CThreadSearch(void)
{
}

bool CThreadSearch::Init() {

	ASSERT(m_pParent);
	ASSERT(m_pList);
	ASSERT(m_pArchivos);
	ASSERT(!m_szCadena.IsEmpty());
	ASSERT(!m_szAppPath.IsEmpty());

	int found= 0;
	CString temporal;
	CStdioFile file;
	CMiscelaneosEx *info;
	m_szCadena.MakeLower();

	m_pList->DeleteAllItems();

	while( temporal!=m_szCadena) {
		temporal= GetLeftString(m_szCadena, " ");
		m_szCadena= GetRightString(m_szCadena, " ");
		m_saTodas.Add(temporal);
	}
	int palabras= m_saTodas.GetCount();
	int pos=0;

	for( int i=0; i<m_pArchivos->GetCount(); i++) {

		found= 0;
		info= m_pArchivos->GetAt(i);
		if( info && !info->GetExtra().IsEmpty()) {
			if( file.Open(m_szAppPath + info->GetExtra(), CFile::modeRead|CFile::shareDenyNone)) {
				while( file.ReadString(temporal)) {
					temporal.MakeLower();
					for( int j=0; j<palabras; j++) {
						if( temporal.Find(m_saTodas.GetAt(j))!=-1) {
							found++;
						}
					}
				}
				file.Close();
			}
		}
		if( found) {
			pos=m_pList->GetItemCount();
			for( int j=0; j<pos; j++) {
				if( atol(m_pList->GetItemText(j, 1))<found) {
					pos= j;
					break;
				}
			}
			m_pList->InsertItem(pos, info->GetNombre());
			temporal.Format("%d", found);
			m_pList->SetItemText(pos, 1, temporal);
			m_pList->SetItemData(pos, (DWORD) info);
		}
	} 
	m_pList->SetMessageLista(m_pList->GetItemCount() ? "" : "No se encontrarón resultados");
	m_pList->AutoSizeColumns();
	CThreadAuto::Init();
	return true;
}
