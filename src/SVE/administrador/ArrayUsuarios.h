#pragma once
#include "..\..\conector\usuario.h"

class CStrongBoxArrayUsuarios : public CUsuario
{
	CStrongBoxArrayUsuarios *m_pSig;
public:
	CStrongBoxArrayUsuarios(void);
	~CStrongBoxArrayUsuarios(void);
	void AddUsuario( CString &nombre, DWORD id, DWORD grupo);
	CUsuario *FindUsuario( CString nombre);
	CUsuario *FindUsuario( DWORD id);
	void	DeleteAll();
	int		GetCount();
	CUsuario *GetAt(int index);
};
