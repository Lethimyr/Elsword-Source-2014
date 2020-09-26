#pragma once

class CX2DTGUNPC
{
public:

    struct GUNPCData
	{
        int	m_iHardLevelExp;
		int m_iTargetNearRange;
		int m_iTargetRange;
		int m_iTargetLostRange;

		GUNPCData()
		{
			m_iHardLevelExp		= 0;
			m_iTargetNearRange	= 0;
			m_iTargetRange		= 0;
			m_iTargetLostRange	= 0;
		}
    };

public:
	CX2DTGUNPC(void);
	~CX2DTGUNPC(void);

	bool OpenScriptFile( std::wstring& wsLuaScriptName, GUNPCData& sData );

	bool GetHardLevelExp( std::wstring& wsLuaScriptName, int& iHardLevelExp );
	bool GetTargetNearRange( std::wstring& wsLuaScriptName, int& iTargetNearRange );
	bool GetTargetRange( std::wstring& wsLuaScriptName, int& iTargetRange );
	bool GetTargetLostRange( std::wstring& wsLuaScriptName, int& iTargetLostRange );

private:	
	std::map< std::wstring, GUNPCData > m_mapGUNPCList;
};
