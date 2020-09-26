#include "dxstdafx.h"
#include ".\aitest.h"




#if 0 //////////////////////////////////////////////////////////////////////////


CAITest::~CAITest(void)
{
	m_pGame->GameEnd();
	SAFE_DELETE(m_pGame);
}

CAITest::CAITest(void)
{
	m_pGame			= NULL;

	//UI 파티클 시스템 생성
	g_pData->ResetUIMajorParticle( L"UIMajorParticle.txt" );
	g_pData->ResetUIMinorParticle( L"UIMinorParticle.txt" );

	//UI 메시플레이어
	g_pData->ResetUIMajorXMeshPlayer( L"UIMajorXMeshPlayer.txt" );
	g_pData->ResetUIMinorXMeshPlayer( L"UIMinorXMeshPlayer.txt" );

	//유닛 데이타 로드
	g_pData->ResetUnitManager( L"UnitTemplet.lua", L"NPCTemplet.lua" );

	//아이템 데이타 로드
	g_pData->ResetItemManager( L"Item.lua" );

	//월드 데이타 로드
	g_pData->ResetWorldManager( L"WorldData.lua" );	

	CreateNewGame();	
}

HRESULT CAITest::OnLostDevice()
{
	HRESULT hr = S_OK;
	if( NULL != m_pGame )
		hr = m_pGame->OnLostDevice();

	hr = g_pKTDXApp->GetDGManager()->GetDialogManager()->OnLostDevice();
	return hr;
}

HRESULT CAITest::OnResetDevice()
{
	HRESULT hr = S_OK;
	if( NULL != m_pGame )
		hr = m_pGame->OnResetDevice();

	hr = g_pKTDXApp->GetDGManager()->GetDialogManager()->OnResetDevice();
	return hr;
}

HRESULT CAITest::OnFrameMove( double fTime, float fElapsedTime )
{
	HRESULT hr = S_OK;
	if( NULL != m_pGame )
		hr = m_pGame->OnFrameMove( fTime, fElapsedTime );
	return hr;
}

HRESULT CAITest::OnFrameRender()
{
	HRESULT hr = S_OK;
	if( NULL != m_pGame )
	{
		hr = m_pGame->OnFrameRender();

		if( NULL != m_pGame->GetWorld() &&
			NULL != m_pGame->GetWorld()->GetLineMap() )
		{
			m_pGame->GetWorld()->GetLineMap()->OnFrameRender();
		}
	}
	return hr;
}

bool CAITest::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	bool bNoFurtherProcessing = false;
	
	if( NULL != m_pGame )
	{	
		bNoFurtherProcessing = m_pGame->MsgProc(hWnd, uMsg, wParam, lParam);
	}

	return bNoFurtherProcessing;
}


void CAITest::CreateNewGame()
{
	const int MAGIC_USER_UID = 777;
	const int MAGIC_UNIT_UID = 777;

	//const CX2Unit::UNIT_CLASS MAGIC_UNIT_CLASS = CX2Unit::UC_ELSWORD_SWORDMAN;
	//const CX2Unit::UNIT_CLASS MAGIC_UNIT_CLASS = CX2Unit::UC_ARME_VIOLET_MAGE;	
	const CX2Unit::UNIT_CLASS MAGIC_UNIT_CLASS = CX2Unit::UC_LIRE_ELVEN_RANGER;

	const int MAGIC_WORLD_ID = CX2World::WI_TREE_OF_LIGHT_TOP;


	// user data setup
    {
	    CX2User::UserData kUserData;
	    {
		    kUserData.userUID		= MAGIC_USER_UID;
		    kUserData.userID		= L"WorldToolUserID";
		    kUserData.userNickName	= L"WorldToolUserNickName";
		    kUserData.maxUnitCount	= 1;	
	    }
	    g_pData->ResetMyUser( kUserData );
    }
/*
	CX2Unit::UnitData* pUnitData = new CX2Unit::UnitData();
	{		
		pUnitData->userUID		= g_pData->GetMyUser()->GetUID();
		pUnitData->unitUID		= MAGIC_UNIT_UID;
		pUnitData->unitClass	= MAGIC_UNIT_CLASS;
		pUnitData->IP			= L"0,0,0,0"; // L"127.0.0.1";
		pUnitData->nickName		= L"*^^*♡";
		pUnitData->GP			= 0;
		pUnitData->level		= 1;
		pUnitData->EXP			= 0;
	}
	*/
	KUnitInfo kUnitInfo;
	kUnitInfo.m_nUnitUID			= MAGIC_UNIT_UID;
	kUnitInfo.m_iUnitClass		= MAGIC_UNIT_CLASS;
	//kUnitInfo.m_vecIP			= L"0,0,0,0"; // L"127.0.0.1";
	kUnitInfo.m_wstrNickName	= L"*^^*♡";
	kUnitInfo.m_iGP				= 0;
	kUnitInfo.m_ucLevel			= 1;
	kUnitInfo.m_iEXP			= 0;

	CX2Unit* pUnit	= new CX2Unit( kUnitInfo );
	g_pData->GetMyUser()->AddUnit( pUnit );

	// room data setup
	CX2PVPRoom::PVPRoomData* pRoomData = new CX2PVPRoom::PVPRoomData();
	{
		pRoomData->roomName				= L"WorldToolRoomName";
		pRoomData->worldID				= (CX2World::WORLD_ID) MAGIC_WORLD_ID;
		CX2Room::SlotData* pSlotData	= new CX2Room::SlotData();
		pSlotData->slotState			= CX2Room::SS_JOIN_HOST;
		pSlotData->pUnit				= g_pData->GetMyUser()->GetUnitByUID( MAGIC_UNIT_UID );
		pRoomData->slotDataList.push_back( pSlotData );
	}
	g_pData->ResetPVPRoom( pRoomData );

	g_pX2Game = NULL;
	SAFE_DELETE( m_pGame );

	m_pGame = new CX2SoloGame();
	g_pX2Game = m_pGame;
	m_pGame->HostReady( g_pData->GetMyUser()->GetUID() );
	m_pGame->GameLoading( g_pData->GetPVPRoom() );
	m_pGame->GameStart();
	m_pGame->GetWorld()->StopBGM();
	g_pKTDXApp->SkipFrame();


#if 0
	CMapGraph mapGraph;
	mapGraph.ReBuildGraphFromLineMap( m_pGame->GetWorld()->GetLineMap() );
#endif
		

}



#endif //////////////////////////////////////////////////////////////////////////
