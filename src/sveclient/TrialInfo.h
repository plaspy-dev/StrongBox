#pragma once

class CStrongBoxTrialInfo
{
	int		m_nLeftDays;
public:
	CStrongBoxTrialInfo(void);
public:
	~CStrongBoxTrialInfo(void);
public:
	bool Register();
	int  GetLeftDays();
};
