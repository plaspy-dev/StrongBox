// DlgIndex.cpp: archivo de implementación
//

#include "stdafx.h"
#include "Hhelp.h"
#include "DlgIndex.h"
# include "FormHelp.h"

// Cuadro de diálogo de CDlgIndex

IMPLEMENT_DYNAMIC(CDlgIndex, CTabPageSSL)
CDlgIndex::CDlgIndex(CWnd* pParent /*=NULL*/)
	: CTabPageSSL(CDlgIndex::IDD, pParent)
{
	m_nRoot=-1;
	m_nRootOver=-1;
	m_nItem=-1;
	m_nItemOver=-1;
	m_nRecomendado=-1;
	m_htRecomendado= NULL;
}

CDlgIndex::~CDlgIndex()
{
}

void CDlgIndex::DoDataExchange(CDataExchange* pDX)
{
	CTabPageSSL::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TINDEX, m_TCIndex);
}


BEGIN_MESSAGE_MAP(CDlgIndex, CTabPageSSL)
	ON_WM_SIZE()
	ON_MESSAGE(WM_HHELP, OnQueryNavigate)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TINDEX, OnTvnSelchangedTindex)
END_MESSAGE_MAP()


// Controladores de mensajes de CDlgIndex

BOOL CDlgIndex::OnInitDialog()
{
	CTabPageSSL::OnInitDialog();

	HIMAGELIST himlNormal;
	SHFILEINFO sfi;

	himlNormal = (HIMAGELIST) SHGetFileInfo ((LPCSTR) "C:\\", 
		0, &sfi, sizeof (SHFILEINFO), SHGFI_SYSICONINDEX | SHGFI_SMALLICON);

	if (himlNormal)
	{
		::SendMessage(m_TCIndex, TVM_SETIMAGELIST, (WPARAM)TVSIL_NORMAL, (LPARAM)himlNormal);
	}

	SHFILEINFO sfi1;
	memset(&sfi1, 0, sizeof(sfi));

	SHGetFileInfo("algo\\", 
			FILE_ATTRIBUTE_DIRECTORY, 
			&sfi1, 
			sizeof(sfi1), 
			SHGFI_SMALLICON | SHGFI_SYSICONINDEX | SHGFI_USEFILEATTRIBUTES
		);
	m_nRoot = sfi1.iIcon;

	SHGetFileInfo("algo\\", 
			FILE_ATTRIBUTE_DIRECTORY, 
			&sfi1, 
			sizeof(sfi1), 
			SHGFI_SMALLICON | SHGFI_SYSICONINDEX | SHGFI_USEFILEATTRIBUTES | SHGFI_OPENICON
		);
	m_nRootOver = sfi1.iIcon;

	SHGetFileInfo("algo.html", 
			FILE_ATTRIBUTE_NORMAL, 
			&sfi1, 
			sizeof(sfi1), 
			SHGFI_SMALLICON | SHGFI_SYSICONINDEX | SHGFI_USEFILEATTRIBUTES
		);
	m_nItem= sfi1.iIcon;

	SHGetFileInfo("algo.html", 
			FILE_ATTRIBUTE_NORMAL, 
			&sfi1, 
			sizeof(sfi1), 
			SHGFI_SMALLICON | SHGFI_SYSICONINDEX | SHGFI_USEFILEATTRIBUTES | SHGFI_OPENICON
		);

	SHGetFileInfo("algo.hlp", 
			FILE_ATTRIBUTE_NORMAL, 
			&sfi1, 
			sizeof(sfi1), 
			SHGFI_SMALLICON | SHGFI_SYSICONINDEX | SHGFI_USEFILEATTRIBUTES
		);
	m_nRecomendado= sfi1.iIcon;

	XmlReader xml;
	xml.Open(CApplicacion::GetAppPath(CMyRegKey::GetProfileString("ayuda", "ruta", "ayuda\\") + "index.xml"));

	HTREEITEM parentroot= m_TCIndex.InsertItem("Contenido",m_nRecomendado, m_nRecomendado);
	m_TCIndex.SetItemState(parentroot, TVIS_BOLD,TVIS_BOLD);
	HTREEITEM item=NULL;
	CString nombre, titulo;

	CString szName, szValue;

	while( xml.ReadString()) {
		switch( xml.GetNodeType()) {
			case NODETYPE_CONTENT:
				xml.GetNodeName(szName);
				xml.GetNodeContent(szValue);
				szValue.TrimLeft("\t ");
				szValue= GetUnescapedXmlString(szValue);
				if( szValue.IsEmpty()) 
					break;
				if( !szName.CompareNoCase("titulo")) {
					titulo= szValue;
				} else if( !szName.CompareNoCase("nombre")) {
					nombre=szValue;
					if( !titulo.IsEmpty()) {
						item= m_TCIndex.InsertItem(titulo, m_nRoot, m_nRootOver, parentroot);
						m_TCIndex.SetItemData(item, (DWORD) m_pArchivos.Add(titulo, (DWORD) item, ""));
					}
				} else if( !szName.CompareNoCase("path")) {
					if( nombre.IsEmpty()) {
						if( !titulo.IsEmpty()) {
							item= m_TCIndex.InsertItem(titulo, m_nRoot, m_nRootOver,parentroot);
							m_TCIndex.SetItemData(item, (DWORD) m_pArchivos.Add(titulo, (DWORD) item, szValue));
						}
					} else {
						HTREEITEM indice=m_TCIndex.InsertItem(nombre, m_nItem , m_nItemOver , item);
						m_TCIndex.SetItemData(indice, (DWORD) m_pArchivos.Add(nombre, (DWORD) indice, szValue));
					}
					titulo.Empty();
					nombre.Empty();
				}
				break;
		}
	}
	xml.Close();
	m_TCIndex.Expand(parentroot, TVE_EXPAND);

	CWnd *parent= GetParent();
	ASSERT(parent);
	ASSERT(parent->IsKindOf(RUNTIME_CLASS(CFormHelp)));
	if(parent->IsKindOf(RUNTIME_CLASS(CFormHelp))) {
		((CFormHelp *)parent)->SetArchivos(&m_pArchivos);
	}
	return TRUE; 
}

LRESULT CDlgIndex::OnQueryNavigate(WPARAM wParam, LPARAM lParam) {

	if( !wParam) {
		CString temp;
		temp= (const char *) lParam;
		if( !temp.IsEmpty()) {
			temp= GetRightString(temp,CMyRegKey::GetProfileString("ayuda", "ruta", "ayuda\\"));
			CMiscelaneosEx *info=m_pArchivos.FindExtraNoCase(temp);
			if( info && info->GetData()) {
				m_TCIndex.SelectItem((HTREEITEM) info->GetData());
			}
		}
	} else if( wParam==3 || wParam==4) {
		
		CString nombre= (const char *) lParam;
		if( !nombre.IsEmpty()) {
			nombre= GetRightString(nombre, CMyRegKey::GetProfileString("ayuda", "ruta", "ayuda\\"));
			CMiscelaneosEx *info=m_pArchivos.FindExtraNoCase(nombre);
			if( info && info->GetData()) {
				
				if( wParam==3 && m_pRecomendado.GetCount()) {

					for( int i=0; i<m_pRecomendado.GetCount(); i++)
						m_TCIndex.DeleteItem((HTREEITEM)m_pRecomendado.GetAt(i)->GetData());
					m_pRecomendado.DeleteAll();
				}
				bool expand=false;
				if( !m_htRecomendado) {
					
					m_htRecomendado = m_TCIndex.InsertItem("Ayuda Dinámica", m_nRecomendado, m_nRecomendado, TVI_ROOT, TVI_FIRST);
					m_TCIndex.SetItemState(m_htRecomendado, TVIS_BOLD,TVIS_BOLD);
					expand=true;
				}

				HTREEITEM indice=m_TCIndex.InsertItem(info->GetNombre(),-1,-1, m_htRecomendado);
				m_TCIndex.SetItemData(indice, (DWORD) m_pRecomendado.Add(info->GetNombre(), (DWORD) indice, info->GetExtra()));

				if(expand)
					m_TCIndex.Expand(m_htRecomendado, TVE_EXPAND);
			}
		}
	}
	return 0;
}

void CDlgIndex::OnSize(UINT nType, int cx, int cy)
{
	CTabPageSSL::OnSize(nType, cx, cy);
	if( ::IsWindow( m_TCIndex)) 
		m_TCIndex.MoveWindow(0,0,cx,cy);
}

void CDlgIndex::OnTvnSelchangedTindex(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	if( pNMTreeView->itemNew.hItem) {
		CMiscelaneosEx *info= (CMiscelaneosEx *)m_TCIndex.GetItemData(pNMTreeView->itemNew.hItem);
		if( info && !info->GetExtra().IsEmpty()) {
			AfxGetMainWnd()->SendMessage(WM_HHELP, 1, (LPARAM) (const char *) info->GetExtra());
		}
	}
	*pResult = 0;
}

CString	CDlgIndex::FindTitulo(const CString &titulo) {

	for( int i=0; i<m_pArchivos.GetCount(); i++) {
		if(m_pArchivos[i]->GetNombre()==titulo)
			return m_pArchivos[i]->GetExtra();
	}
	return "";
}

