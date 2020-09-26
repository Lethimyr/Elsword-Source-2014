#pragma once

#include <windows.h>
#include <RTTI.h>
#include <map>
#include <ToString.h>
#include <KNCSingleton.h>

class KSpiritTable
{
	DeclareSingleton( KSpiritTable );

public:
	KSpiritTable(void);
	virtual ~KSpiritTable(void);	

	bool	OpenScriptFile( const char* pFileName );
	bool	SetMaxSpirit_LUA();
	bool	AddSpiritTable_LUA();
	bool	GetSpiritByLevel( IN int nLevel, OUT int& nSpirit );
	int		GetMaxSpirit() { return m_iMaxSpirit; }

private:
	std::map< int, int >	m_mapSpiritTable;
	int						m_iMaxSpirit;
};

DefSingletonInline( KSpiritTable );