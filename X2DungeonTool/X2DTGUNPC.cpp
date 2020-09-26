#include "dxstdafx.h"
#include ".\x2dtgunpc.h"

CX2DTGUNPC::CX2DTGUNPC(void)
{
}

CX2DTGUNPC::~CX2DTGUNPC(void)
{
}

bool CX2DTGUNPC::OpenScriptFile( std::wstring& wsLuaScriptName, GUNPCData& sData )
{
	KLuaManager luaManager;

	//스크립트 파일을 로드한다
	g_pKTDXApp->GetDeviceManager()->LoadLuaTinker( wsLuaScriptName.c_str(), false );
	g_pKTDXApp->GetDeviceManager()->LoadLuaManager( &luaManager, L"Enum.lua" );
	if ( g_pKTDXApp->GetDeviceManager()->LoadLuaManager( &luaManager, wsLuaScriptName.c_str(), false ) == false )
	{
		MessageBox( NULL, wsLuaScriptName.c_str(), L"게임유닛 스크립트 로드 실패!", MB_OK );
		return false;
	}

	GUNPCData sGUNPCData;

	// HARD_LEVEL
	if( luaManager.BeginTable( "INIT_COMPONENT" ) == true )
	{
		if( luaManager.BeginTable( "HARD_LEVEL" ) == true )
		{
			luaManager.GetValue( "EXP", sGUNPCData.m_iHardLevelExp );
			luaManager.EndTable();
		}
		luaManager.EndTable();
	}

	// NPC AI
	if( luaManager.BeginTable( "INIT_AI" ) == true )
	{
		if( luaManager.BeginTable( "TARGET" ) == true )
		{
			luaManager.GetValue( "TARGET_NEAR_RANGE", sGUNPCData.m_iTargetNearRange );
			luaManager.GetValue( "TARGET_RANGE",	  sGUNPCData.m_iTargetRange );
			luaManager.GetValue( "TARGET_LOST_RANGE", sGUNPCData.m_iTargetLostRange );
			luaManager.EndTable();
		}
		luaManager.EndTable();
	}

	m_mapGUNPCList.insert( std::make_pair( wsLuaScriptName, sGUNPCData ) );
	sData = sGUNPCData;
	return true;
}

bool CX2DTGUNPC::GetHardLevelExp( std::wstring& wsLuaScriptName, int& iHardLevelExp )
{
	std::map< std::wstring, GUNPCData >::iterator mit = m_mapGUNPCList.find( wsLuaScriptName );
	if( mit != m_mapGUNPCList.end() )
	{
		iHardLevelExp = mit->second.m_iHardLevelExp;
        return true;
	}
	else
	{	
		GUNPCData sGUNPCData;
		if( OpenScriptFile( wsLuaScriptName, sGUNPCData ) == false )
			return false;

		iHardLevelExp = sGUNPCData.m_iHardLevelExp;		
	}

	return true;
}

bool CX2DTGUNPC::GetTargetNearRange( std::wstring& wsLuaScriptName, int& iTargetNearRange )
{
	std::map< std::wstring, GUNPCData >::iterator mit = m_mapGUNPCList.find( wsLuaScriptName );
	if( mit != m_mapGUNPCList.end() )
	{
		iTargetNearRange = mit->second.m_iTargetNearRange;
		return true;
	}
	else
	{	
		GUNPCData sGUNPCData;
		if( OpenScriptFile( wsLuaScriptName, sGUNPCData ) == false )
			return false;

		iTargetNearRange = sGUNPCData.m_iTargetNearRange;
	}

	return true;
}

bool CX2DTGUNPC::GetTargetRange( std::wstring& wsLuaScriptName, int& iTargetRange )
{
	std::map< std::wstring, GUNPCData >::iterator mit = m_mapGUNPCList.find( wsLuaScriptName );
	if( mit != m_mapGUNPCList.end() )
	{
		iTargetRange = mit->second.m_iTargetRange;
		return true;
	}
	else
	{	
		GUNPCData sGUNPCData;
		if( OpenScriptFile( wsLuaScriptName, sGUNPCData ) == false )
			return false;

		iTargetRange = sGUNPCData.m_iTargetRange;
	}

	return true;
}

bool CX2DTGUNPC::GetTargetLostRange( std::wstring& wsLuaScriptName, int& iTargetLostRange )
{
	std::map< std::wstring, GUNPCData >::iterator mit = m_mapGUNPCList.find( wsLuaScriptName );
	if( mit != m_mapGUNPCList.end() )
	{
		iTargetLostRange = mit->second.m_iTargetLostRange;
		return true;
	}
	else
	{	
		GUNPCData sGUNPCData;
		if( OpenScriptFile( wsLuaScriptName, sGUNPCData ) == false )
			return false;

		iTargetLostRange = sGUNPCData.m_iTargetLostRange;
	}

	return true;
}