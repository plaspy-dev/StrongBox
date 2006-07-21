#pragma once

class __declspec(dllexport) CStrongBoxMiscelaneosEx
{
	CString			m_szNombre;
	CString			m_szExtra;
	DWORD			m_dwData;
	CStrongBoxMiscelaneosEx	*m_pSig;
	void			DeleteLast();
public:
	CStrongBoxMiscelaneosEx &operator = (CStrongBoxMiscelaneosEx &item);
	CStrongBoxMiscelaneosEx *Add(CString &nombre, DWORD data, CString szExtra="");
	int GetCount();
	CStrongBoxMiscelaneosEx *GetAt(int index);
	CString GetNombre();
	DWORD GetData();
	CString GetExtra();
	void SetData(DWORD data);
	void SetExtra(CString extra);
	void SetNombre(CString nombre);
	CStrongBoxMiscelaneosEx *Find(CString nombre);
	CStrongBoxMiscelaneosEx *Find(DWORD data);
	CStrongBoxMiscelaneosEx *FindNoCase(CString nombre);
	CStrongBoxMiscelaneosEx(void);
	void	DeleteAll();
	void	RemoveAt(int index);
	~CStrongBoxMiscelaneosEx(void);
};
