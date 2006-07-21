#include "StdAfx.h"
#include "MiscelaneosEx.h"

CStrongBoxMiscelaneosEx::CStrongBoxMiscelaneosEx(void)
{
	m_pSig= NULL;
}

CStrongBoxMiscelaneosEx::~CStrongBoxMiscelaneosEx(void)
{
	CStrongBoxMiscelaneosEx *info= m_pSig, *aux;
	while( info) {
		aux= info->m_pSig;
		info->m_pSig= NULL;
		delete info;
		info=aux;
	}
}

void CStrongBoxMiscelaneosEx::DeleteAll() {

	m_szNombre.Empty();
	if( m_pSig) {
		delete m_pSig;
		m_pSig= NULL;
	}
}

void CStrongBoxMiscelaneosEx::SetData(DWORD data) {
	m_dwData= data;
}

void CStrongBoxMiscelaneosEx::SetExtra(CString extra) {
	m_szExtra= extra;
}

void CStrongBoxMiscelaneosEx::SetNombre(CString nombre) {
	m_szNombre= nombre;
}

CStrongBoxMiscelaneosEx *CStrongBoxMiscelaneosEx::Add(CString &nombre, DWORD data, CString szExtra) {

	if( m_szNombre.IsEmpty()) {

		m_szNombre= nombre;
		m_dwData= data;
		m_szExtra= szExtra;
		return this;
	}
	CStrongBoxMiscelaneosEx **info= &m_pSig;
	while( *info)
		info= &(*info)->m_pSig;
	ASSERT(*info==NULL);
	*info= new CStrongBoxMiscelaneosEx();
	return (*info)->Add(nombre, data, szExtra);
}

CStrongBoxMiscelaneosEx *CStrongBoxMiscelaneosEx::Find(CString nombre) {

	CStrongBoxMiscelaneosEx *info= this;
	while( info) {

		if( info->m_szNombre==nombre)
			return info;
		info= info->m_pSig;
	}
	return NULL;
}

void CStrongBoxMiscelaneosEx::DeleteLast() {
	if( m_pSig) {
		if( m_pSig->m_pSig) {
			m_pSig->DeleteLast();
		} else {
			delete m_pSig;
			m_pSig=NULL;
		}
	}
}

void CStrongBoxMiscelaneosEx::RemoveAt(int index) {
	
	CStrongBoxMiscelaneosEx *info=GetAt(index);
	if( info) {
		CStrongBoxMiscelaneosEx *update=info;
		info= info->m_pSig;
		while(info) {
			*update=*info;
			info=info->m_pSig;
			update=update->m_pSig;
		}
		if( update==this) {
			m_szNombre.Empty();
			m_dwData=0;
		} else if( update) {
			DeleteLast();
			update= NULL;
		}
	}
}

CStrongBoxMiscelaneosEx &CStrongBoxMiscelaneosEx::operator = (CStrongBoxMiscelaneosEx &item) {

	if( &item!=this) {

		m_szExtra=item.m_szExtra;
		m_szNombre= item.m_szNombre;
		m_dwData= item.m_dwData;
	}
	return *this;
}

CStrongBoxMiscelaneosEx *CStrongBoxMiscelaneosEx::Find(DWORD data) {

	CStrongBoxMiscelaneosEx *info= this;
	while( info) {

		if( info->m_dwData==data)
			return info;
		info= info->m_pSig;
	}
	return NULL;
}

CStrongBoxMiscelaneosEx *CStrongBoxMiscelaneosEx::FindNoCase(CString nombre) {

	CStrongBoxMiscelaneosEx *info= this;
	while( info) {

		if( !info->m_szNombre.CompareNoCase(nombre))
			return info;
		info= info->m_pSig;
	}
	return NULL;
}

int CStrongBoxMiscelaneosEx::GetCount() {

	if( m_szNombre.IsEmpty())
		return 0;
	CStrongBoxMiscelaneosEx *info= this;
	int i=0;
	while( info) {

		i++;
		info= info->m_pSig;
	}
	return i;
}

CStrongBoxMiscelaneosEx *CStrongBoxMiscelaneosEx::GetAt(int index) {

	CStrongBoxMiscelaneosEx *info= this;
	int i=0;
	while( info) {

		if( index==i)
			return info;
		i++;
		info= info->m_pSig;
	}
	return NULL;
}

CString CStrongBoxMiscelaneosEx::GetNombre() {
	return m_szNombre;
}

DWORD CStrongBoxMiscelaneosEx::GetData() {

	return m_dwData;
}

CString CStrongBoxMiscelaneosEx::GetExtra() {

	return m_szExtra;
}

