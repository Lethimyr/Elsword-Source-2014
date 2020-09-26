#include "StdAfx.h"
#include ".\x2trainingcentertable.h"

CX2TrainingCenterTable::CX2TrainingCenterTable(void)
{
}

CX2TrainingCenterTable::~CX2TrainingCenterTable(void)
{
}

bool CX2TrainingCenterTable::OpenScriptFile( const WCHAR* pFileName )
{
	lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(),  "KTrainingCenterTable", this );

    if ( g_pKTDXApp->LoadLuaTinker( pFileName ) == false )
    {
		return false;
    }

	return true;
}


bool CX2TrainingCenterTable::AddTCTemplet_LUA()
{
	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
#ifndef X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
    TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING

	TC_TABLE_INFO	kInfo;

	LUA_GET_VALUE(	luaManager, "m_iID",					kInfo.m_iID,				0		);
	LUA_GET_VALUE(	luaManager, "m_iDungeonID",				kInfo.m_iDungeonID,			0		);
	LUA_GET_VALUE(	luaManager, "m_iMinLevel",				kInfo.m_iMinLevel,			0		);
	LUA_GET_VALUE(	luaManager, "m_iBeforeID",				kInfo.m_iBeforeID,			0		);
	LUA_GET_VALUE(	luaManager, "m_fPlayTime",				kInfo.m_fPlayTime,			0.f		);
	LUA_GET_VALUE(	luaManager, "m_iDifficulty",			kInfo.m_iDifficulty,		1		);
	
	LUA_GET_VALUE(	luaManager, "m_iTrainingNumber",		kInfo.m_iTrainingNumber,	0		);
	
	int str_id;
	LUA_GET_VALUE(	luaManager, "m_wstrTitle",				str_id,			STR_ID_EMPTY		);
	kInfo.m_wstrTitle = GET_STRING( str_id );
	LUA_GET_VALUE(	luaManager, "m_wstrDesc",				str_id,			STR_ID_EMPTY		);
	kInfo.m_wstrDesc = GET_STRING( str_id );
	LUA_GET_VALUE(	luaManager, "m_wstrTextureName",		kInfo.m_wstrTextureName,			L"" );
	LUA_GET_VALUE(	luaManager, "m_wstrTexturePieceName",	kInfo.m_wstrTexturePieceName,		L"" );

	LUA_GET_VALUE(	luaManager, "m_wstrDescTextureName",		kInfo.m_wstrDescTextureName,			L"" );
	LUA_GET_VALUE(	luaManager, "m_wstrDescTexturePieceName",	kInfo.m_wstrDescTexturePieceName,		L"" );
	
	if( luaManager.BeginTable( "UNIT_CLASS" ) == true )
	{
		int index	= 1; 
		int buf		= -1;
		while( luaManager.GetValue( index, buf ) == true )
		{
			if( buf >= 0 )
			{
				kInfo.m_vecUnitClass.push_back( buf );
			}
			index++;
		}

		if( index == 1 && buf == -1 )
		{
			luaManager.EndTable();
			return false;
		}

		luaManager.EndTable();
	}

	if( luaManager.BeginTable( "REWARD" ) == true )
	{
		LUA_GET_VALUE(	luaManager, "m_iRewardED",		kInfo.m_iRewardED,	0		);
		LUA_GET_VALUE(	luaManager, "m_iRewardEXP",	kInfo.m_iRewardEXP,		0		);

		luaManager.EndTable();
	}

	m_mapTCInfo.insert( std::make_pair( kInfo.m_iID, kInfo ) );
	
	return true;
}

vector<int>& CX2TrainingCenterTable::GetTrainingListByUnitClass( CX2Unit::UNIT_CLASS eUnitClass )
{
	std::map< CX2Unit::UNIT_CLASS, vector<int> >::iterator it;
	it = m_mapTrainingListByUnitClass.find( eUnitClass );
	if( 0 != m_mapTrainingListByUnitClass.size() && it != m_mapTrainingListByUnitClass.end() )
	{
		return it->second;
	}
	else
	{
		vector<int> vecNewClassTrainingList;
		m_mapTrainingListByUnitClass[eUnitClass] = vecNewClassTrainingList;
		return m_mapTrainingListByUnitClass[eUnitClass];
	}
}


bool CX2TrainingCenterTable::SetTrainingListByUnitClass_LUA()
{
	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
#ifndef X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
    TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING


	CX2Unit::UNIT_CLASS eUnitClass;
	LUA_GET_VALUE_ENUM( luaManager, "UNIT_CLASS", eUnitClass, CX2Unit::UNIT_CLASS, CX2Unit::UC_NONE );

	if( CX2Unit::UC_NONE == eUnitClass )
		return false;


	vector<int>& vecTrainingList = GetTrainingListByUnitClass( eUnitClass );
	vecTrainingList.clear();

	if( true == luaManager.BeginTable( "TRAINING_ID_LIST" ) )
	{
		int index	= 1; 
		int iTrainingID = -1;
		while( true == luaManager.GetValue( index, iTrainingID ) )
		{
			if( iTrainingID >= 0 )
			{
				vecTrainingList.push_back( iTrainingID );
			}
			index++;
		}

		luaManager.EndTable();
	}

	return true;
}


int CX2TrainingCenterTable::GetDungeonID( int iTrainingID )
{
	std::map<int, TC_TABLE_INFO>::iterator mit;

	mit = m_mapTCInfo.find( iTrainingID );

	if( mit != m_mapTCInfo.end() )
	{
		return (int) mit->second.m_iDungeonID;
	}

	return -1; 
}


bool CX2TrainingCenterTable::CheckIfEnter( IN std::map< int, KTCClearInfo >& mapTCClear, IN int iTCID )
{
	//진행하면서 제약을 체크한다.
	std::map< int , TC_TABLE_INFO >::iterator mit;
	std::map< int , KTCClearInfo >::iterator mitBefore;
	mit = m_mapTCInfo.find( iTCID );

	if( mit != m_mapTCInfo.end() )
	{
		if( mit->second.m_iBeforeID == 0 )
		{
			return true;
		}

		if( mapTCClear.size() > 0 )
		{
			mitBefore = mapTCClear.find( mit->second.m_iBeforeID );

			if( mitBefore != mapTCClear.end() )
			{
				return true;
			}
		}
	}

	return false;
}

int CX2TrainingCenterTable::GetNextTrainingID( CX2Unit::UNIT_CLASS eUnitClass, int iNowTrainingID )
{
	vector<int>& vecTrainingList = GetTrainingListByUnitClass( eUnitClass );

	for( UINT i=0; i<vecTrainingList.size(); i++ )
	{
		int iTrainingID = vecTrainingList[i];
		if( iTrainingID == iNowTrainingID )
		{
			if( i == vecTrainingList.size()-1 )
				return -1;
			else
				return vecTrainingList[i+1];
		}
	}

	return -1;
}

bool CX2TrainingCenterTable::GetTrainingInfo( TC_TABLE_INFO& trainingInfo, int iTrainingID )
{
	std::map<int, TC_TABLE_INFO>::iterator it;
	it = m_mapTCInfo.find( iTrainingID );
	if( it != m_mapTCInfo.end() )
	{
		trainingInfo = it->second;
		return true;
	}

	return false;
}