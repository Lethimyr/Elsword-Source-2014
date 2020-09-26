#pragma once

class CX2EXPTable
{
	public:
		struct EXPData
		{
			int m_Level;
			int	m_nNeedExp;
			int	m_nTotalExp;

			EXPData()
			{
				m_Level		= 0;
				m_nNeedExp	= 0;
				m_nTotalExp	= 0;
			}
		};

	public:
		CX2EXPTable(void);
		~CX2EXPTable(void);

		bool	OpenScriptFile( const WCHAR* wstrFileName );
		bool	AddExpTable_LUA();

		EXPData	GetEXPData( int level );
		int		GetLevel( int iNowEXP );

	private:
		std::map<int,EXPData>	m_mapExpTable;
};
