#include "..\KncRobotManager.h"
#include ".\x2locationmanager.h"

ImplementSingleton( CX2LocationManager );

CX2LocationManager::CX2LocationManager(void)
{
}

CX2LocationManager::~CX2LocationManager(void)
{
	//메모리 해제 시켜줘야지?
	
	std::map< int, CX2LocationManager::VillageTemplet* >::iterator mitVT;
	for ( mitVT = m_mapVillageTemplet.begin(); mitVT != m_mapVillageTemplet.end(); ++mitVT )
	{
		CX2LocationManager::VillageTemplet* pVillageTemplet = (CX2LocationManager::VillageTemplet*)mitVT->second;
		SAFE_DELETE( pVillageTemplet );
	}
	m_mapVillageTemplet.clear();

	std::map< int, VillageStartPos* >::iterator mitVM;
	for ( mitVM = m_mapVillageMap.begin(); mitVM != m_mapVillageMap.end(); ++mitVM )
	{
		CX2LocationManager::VillageStartPos* pVillageStartPos = (CX2LocationManager::VillageStartPos*)mitVM->second;
		SAFE_DELETE( pVillageStartPos );
	}
	m_mapVillageMap.clear();
}

CX2LocationManager::VillageTemplet*		CX2LocationManager::GetVillageMapTemplet( int iMapID )
{
	CX2LocationManager::VillageTemplet* pVillageTemplet = NULL;

	std::map< int, CX2LocationManager::VillageTemplet* >::iterator mit;
	mit = m_mapVillageTemplet.find( iMapID );

	if( mit != m_mapVillageTemplet.end() )
	{
		pVillageTemplet = mit->second;
	}

	return pVillageTemplet;
}

bool CX2LocationManager::OpenScriptFile( const char* pFileName )
{
//{{ robobeg : 2008-10-28
	//g_pKTDXApp->GetDeviceManager()->LoadLuaManager( &kLuamanager, L"Enum.lua"			);
	//g_pKTDXApp->GetDeviceManager()->LoadLuaManager( &kLuamanager, L"DLG_Map_Enum.lua"	);
//}} robobeg : 2008-10-28

	boost::shared_ptr<lua_State> spLua = SiKncRobotManager()->GetLuaState();

	lua_tinker::class_add<CX2LocationManager>( spLua.get(), "CX2LocationManager" );
	lua_tinker::class_def<CX2LocationManager>( spLua.get(), "Village",		&CX2LocationManager::VillageParcing );
	lua_tinker::decl( spLua.get(), "LocationManager", this );
	
	if( 0 != luaL_dofile( spLua.get(), pFileName ) ) return false;
	
	return true;
}

void CX2LocationManager::VillageParcing()
{
	boost::shared_ptr<lua_State> spLua = SiKncRobotManager()->GetLuaState();
	KLuaManager kLuamanager( spLua.get() );

	//빌리지
	float fCameraDist = 1200.f;
	float fLookatHeight = 200.f;
	float fEyeHeight = 300.f;
	std::vector< int > vecStartPos;
	std::vector< int > vecSummonPos;

	int iMapID = 0;
	LUA_GET_VALUE_RETURN( kLuamanager, L"VillageMapID", iMapID, 0, return; );

	if( kLuamanager.BeginTable( "START_POS" ) == S_OK )
	{
		int index = 1;
		int startPosId = 0;
		while( kLuamanager.BeginTable( index ) == S_OK )
		{
			VillageStartPos *pStartPosInfo = new CX2LocationManager::VillageStartPos();

			// 현재 포지션의 id를 설정한다.
			LUA_GET_VALUE( kLuamanager, L"StartPosId", startPosId , 0 );

			vecStartPos.push_back( startPosId );

			// 현재 포지션을 가지는 마을번호를 설정한다.
			pStartPosInfo->m_MapID = iMapID;

			// 현재 포지션의 3d 위치를 설정한다.
			LUA_GET_VALUE( kLuamanager, L"StartPosX", pStartPosInfo->m_StartPos.x , 0 );
			LUA_GET_VALUE( kLuamanager, L"StartPosY", pStartPosInfo->m_StartPos.y , 0 );
			LUA_GET_VALUE( kLuamanager, L"StartPosZ", pStartPosInfo->m_StartPos.z , 0 );
			
			LUA_GET_VALUE( kLuamanager, L"IsMarket",	pStartPosInfo->m_bIsMarket , false );
			LUA_GET_VALUE( kLuamanager, L"IsNPC",		pStartPosInfo->m_bIsNPC , false );
			LUA_GET_VALUE( kLuamanager, L"IsSummon",	pStartPosInfo->m_bIsSummon , false );
			LUA_GET_VALUE( kLuamanager, L"LinkDungeon", pStartPosInfo->m_iDungeonId , 0 );
			LUA_GET_VALUE( kLuamanager, L"LinkDungeonPos", pStartPosInfo->m_iDungeonPos , 0 );			
			
			if( pStartPosInfo->m_bIsSummon )
				vecSummonPos.push_back( startPosId );

			// 현재 포지션과 연결된 포지션을 설정한다.
			if( kLuamanager.BeginTable( L"LinkPos" ) == S_OK )
			{
				int LinkIndex	= 1; 
				int linkPos		= -1;
				while( kLuamanager.GetValue( LinkIndex, linkPos ) == S_OK )
				{
					if( linkPos >= 0 )
						pStartPosInfo->m_vecLinkStartPos.push_back(linkPos);

					++LinkIndex;
				}

				kLuamanager.EndTable();						
			}

			kLuamanager.EndTable();
			index++;

			m_mapVillageMap.insert( std::make_pair( startPosId, pStartPosInfo ) );							
		}

		kLuamanager.EndTable();
	}

	std::wstring frameMoveFuncName;
	LUA_GET_VALUE( kLuamanager, L"FrameMoveFunc", frameMoveFuncName, L"" );

	CX2LocationManager::VillageTemplet* pVillageTemplet = new CX2LocationManager::VillageTemplet();
	pVillageTemplet->m_MapID			 = (CX2LocationManager::MAP_ID)iMapID;
	pVillageTemplet->m_FrameMoveFuncName = frameMoveFuncName;

	pVillageTemplet->m_fCameraDistance	 = fCameraDist;
	pVillageTemplet->m_fLookAtHeight	 = fLookatHeight;
	pVillageTemplet->m_fEyeHeight		 = fEyeHeight;
	pVillageTemplet->m_vecStartPos		 = vecStartPos;
	pVillageTemplet->m_vecSummonPos		 = vecSummonPos;

	m_mapVillageTemplet.insert( std::make_pair( (CX2LocationManager::MAP_ID)iMapID, pVillageTemplet ) );
}

CX2LocationManager::VillageStartPos* CX2LocationManager::GetVillageStartPos(int startPos)
{
	CX2LocationManager::VillageStartPos* pVillageStartPos = NULL;

	std::map<int, CX2LocationManager::VillageStartPos*>::iterator mit;
	mit = m_mapVillageMap.find( startPos );

	if( mit != m_mapVillageMap.end() )
	{
		pVillageStartPos = mit->second;
	}

	return pVillageStartPos;
}

int CX2LocationManager::GetMapID( int startPos )
{
	CX2LocationManager::VillageStartPos* pVillageStartPos = NULL;
	pVillageStartPos = GetVillageStartPos(startPos);

	if(pVillageStartPos != NULL)
		return pVillageStartPos->m_MapID;

	return MI_NONE;
}
D3DXVECTOR3 CX2LocationManager::GetStartPosLoc(int startPos)
{
	CX2LocationManager::VillageStartPos* pVillageStartPos = NULL;
	pVillageStartPos = GetVillageStartPos(startPos);

	if(pVillageStartPos != NULL)
		return pVillageStartPos->m_StartPos;

	return D3DXVECTOR3(0.f, 0.f, 0.f);
}

void CX2LocationManager::GetVecLinkedPos(std::vector<int>& vStart)
{
	CX2LocationManager::VillageStartPos* pVillageStartPos = NULL;
	
	std::map<int, CX2LocationManager::VillageStartPos*>::iterator mit;

	for(mit = m_mapVillageMap.begin(); mit != m_mapVillageMap.end(); ++mit)
	{
		pVillageStartPos = mit->second;
		if( pVillageStartPos->m_MapID == m_eCurrentVillage && 
			(pVillageStartPos->m_vecLinkStartPos.size() > 0 || pVillageStartPos->m_iDungeonId > 0) && 
			pVillageStartPos->m_bIsNPC == false && pVillageStartPos->m_bIsMarket == false )
			vStart.push_back(mit->first);
	}	
}

void CX2LocationManager::GetVecMarketPos(std::vector<int>& vStart)
{
	CX2LocationManager::VillageStartPos* pVillageStartPos = NULL;

	std::map<int, CX2LocationManager::VillageStartPos*>::iterator mit;

	for(mit = m_mapVillageMap.begin(); mit != m_mapVillageMap.end(); ++mit)
	{
		pVillageStartPos = mit->second;
		if(pVillageStartPos->m_MapID == m_eCurrentVillage && pVillageStartPos->m_bIsMarket == true)
			vStart.push_back(mit->first);
	}	
}

D3DXVECTOR3 CX2LocationManager::GetLastLinkPos(int startPos)
{
	D3DXVECTOR3 vPos = D3DXVECTOR3(0.f, 0.f, 0.f);
	CX2LocationManager::VillageStartPos* pVillageStartPos = NULL;
	pVillageStartPos = GetVillageStartPos(startPos);

	if(pVillageStartPos != NULL)
	{		
		int inx = pVillageStartPos->m_iDungeonPos;
		if(inx > 0)
		{
			vPos = GetStartPosLoc(inx);
		}
	}

	return vPos;
}

int	CX2LocationManager::GetStartPosLink(int startPos)
{
	CX2LocationManager::VillageStartPos* pVillageStartPos = NULL;
	pVillageStartPos = GetVillageStartPos(startPos);

	if(pVillageStartPos != NULL)
	{		
		if(pVillageStartPos->m_vecLinkStartPos.size() == 0)
			return 0;

		if(pVillageStartPos->m_vecLinkStartPos.size() == 1)
		{
			return 	pVillageStartPos->m_vecLinkStartPos[0];
		}

		int randInx = rand() % pVillageStartPos->m_vecLinkStartPos.size();
			return pVillageStartPos->m_vecLinkStartPos[randInx];
	}

	return 0;
}

int CX2LocationManager::GetLinkedDungeon(int startPos)
{
	CX2LocationManager::VillageStartPos* pVillageStartPos = NULL;
	pVillageStartPos = GetVillageStartPos(startPos);

	if(pVillageStartPos != NULL)
	{		
		return pVillageStartPos->m_iDungeonId;
	}

	return 0;
}

int CX2LocationManager::GetLoginPos(UidType mapId)
{
	CX2LocationManager::VillageStartPos* pVillageStartPos = NULL;

	std::vector<int> vecLoginPos;
	std::map<int, CX2LocationManager::VillageStartPos*>::iterator mit;

	for(mit = m_mapVillageMap.begin(); mit != m_mapVillageMap.end(); ++mit)
	{
		pVillageStartPos = mit->second;
		if(pVillageStartPos->m_MapID == mapId && pVillageStartPos->m_vecLinkStartPos.size() <= 0 && pVillageStartPos->m_bIsSummon == true)
			vecLoginPos.push_back(mit->first);
	}

	if(vecLoginPos.size() == 0)
		return 0;
    
	int randInx = rand() % vecLoginPos.size();
	return vecLoginPos[randInx];
}

D3DXVECTOR3 CX2LocationManager::GetNearLoginPos(D3DXVECTOR3 vPos)
{
	CX2LocationManager::VillageStartPos* pVillageStartPos = NULL;

	std::vector<int> vecLoginPos;
	std::map<int, CX2LocationManager::VillageStartPos*>::iterator mit;
	D3DXVECTOR3 vRet;
	float fDist = 99999.f;

	for(mit = m_mapVillageMap.begin(); mit != m_mapVillageMap.end(); ++mit)
	{
		pVillageStartPos = mit->second;
		if(pVillageStartPos->m_MapID == m_eCurrentVillage && pVillageStartPos->m_vecLinkStartPos.size() <= 0 && pVillageStartPos->m_bIsSummon == true)
		{
			D3DXVECTOR3 vStartPos = GetStartPosLoc(mit->first);

			if(fDist >= GetDistance(vPos, vStartPos))
			{
				fDist = GetDistance(vPos, vStartPos);
				vRet = vStartPos;			
			}
		}
	}

	return vRet;
}


void CX2LocationManager::SetVillage(MAP_ID val, D3DXVECTOR3 pos)
{
	m_eOldVillage = m_eCurrentVillage;
	m_vOldPos = m_vCurrentPos;

	m_eCurrentVillage = val;
	m_vCurrentPos = pos;
}
void CX2LocationManager::GetVillage(MAP_ID &VillageId, D3DXVECTOR3 &vPos)
{
	VillageId = m_eCurrentVillage;
	vPos = m_vCurrentPos;	
}
void CX2LocationManager::RestoreVillage()
{
	m_eCurrentVillage = m_eOldVillage;
	m_vCurrentPos = m_vOldPos;
}

//{{ 2008. 12. 8  최육사
bool CX2LocationManager::GetRandomPositionInField( int iMapID, D3DXVECTOR3& kPos )
{
	VillageTemplet* pVillageTemplet = GetVillageMapTemplet( iMapID );
	if( pVillageTemplet == NULL )
	{
		START_LOG( cerr, L"village map 템플릿이 이상합니다" )
			<< BUILD_LOG( iMapID )
			<< END_LOG;
		return false;
	}

	if( pVillageTemplet->m_vecSummonPos.empty() )
		return false;

	int iStartPosRandom = rand() % pVillageTemplet->m_vecSummonPos.size();
	int iStartPos = pVillageTemplet->m_vecSummonPos[iStartPosRandom];

	VillageStartPos* pVillageStartPos = GetVillageStartPos( iStartPos );
	if( pVillageStartPos == NULL )
	{
		START_LOG( cerr, L"Start Pos 템플릿이 이상합니다" )
			<< BUILD_LOG( iStartPos )
			<< END_LOG;
		return false;
	}

	kPos = pVillageStartPos->m_StartPos;
	return true;
}
//}}


