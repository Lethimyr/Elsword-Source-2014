#include "StdAfx.h"
#include ".\x2GlobalMissionManager.h"

#ifdef SERV_GLOBAL_MISSION_MANAGER

CX2GlobalMissionManager::CX2GlobalMissionManager()
: m_pUIGlobalMission( NULL )
{
	m_pUIGlobalMission = new CX2UIGlobalMission;

	Init();	
}		


CX2GlobalMissionManager::~CX2GlobalMissionManager()
{
	SAFE_DELETE( m_pUIGlobalMission );
}

void CX2GlobalMissionManager::Init()
{
	m_kNowGlobalMissionInfo.m_bActive = false;
}

bool CX2GlobalMissionManager::IsActive()
{
	return m_kNowGlobalMissionInfo.m_bActive;
}

bool CX2GlobalMissionManager::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if( m_pUIGlobalMission != NULL )
	{
		if( true == m_pUIGlobalMission->UICustomEventProc( hWnd, uMsg, wParam, lParam ) )
		{
			return true;
		}
	}

	return false;
}

bool CX2GlobalMissionManager::UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KTDXPROFILE();
	switch( wParam )
	{
	case EGS_GLOBAL_MISSION_UPDATE_NOT:
		{
			return Handler_EGS_GLOBAL_MISSION_UPDATE_NOT( hWnd, uMsg, wParam, lParam );
		} break;
	default:
		break;
	}
	return false;
}

HRESULT CX2GlobalMissionManager::OnFrameMove( double fTime, float fElapsedTime )
{
	if( m_pUIGlobalMission->GetGlobalMissionDlg() != NULL )
	{
		if( CheckShowGlobalMissionUI() == true )
		{
			m_pUIGlobalMission->UpdateNumOfClearCount( m_kNowGlobalMissionInfo.m_iClearCount );
			m_pUIGlobalMission->UpdateNumOfNowClearCount( m_kNowGlobalMissionInfo.m_iNowCount );
			m_pUIGlobalMission->SetShowGlobalMissionDlg( true );
		}
		else
		{
			m_pUIGlobalMission->SetShowGlobalMissionDlg( false );
		}
	}

	return S_OK;
}

bool CX2GlobalMissionManager::Handler_EGS_GLOBAL_MISSION_UPDATE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{	// 게임서버에서 주기적으로 정보를 보내줌.
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_GLOBAL_MISSION_UPDATE_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	CTime tCurrentTime = g_pData->GetServerCurrentTime(); // g_pData->GetServerCurrentTime64()를 쓸까?
	wstring wstrGlobalMissionBeginDate = tCurrentTime.Format( _T("%Y-%m-%d") );
	wstrGlobalMissionBeginDate += L" 00:00:00";

	std::map< std::wstring, KGlobalMissionInfo >::iterator mit = kEvent.m_mapGlobalMissionInfo.find( wstrGlobalMissionBeginDate );
	if( mit != kEvent.m_mapGlobalMissionInfo.end() )
	{
		m_kNowGlobalMissionInfo = mit->second;
	}

	return true;
}

bool CX2GlobalMissionManager::CheckShowGlobalMissionUI()
{
#ifdef SERV_SECOND_CLASS_WORLD_WAR_EVENT
	bool bElesis35Level = false;
	if( _CONST_SECOND_CLASS_WORLD_WAR_EVENT::bWorldWarEventForElesis1stClass == true &&
		g_pData != NULL && g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetSelectUnit() != NULL )
	{
		if( g_pData->GetMyUser()->GetSelectUnit()->GetClass() == CX2Unit::UC_ELESIS_SABER_KNIGHT ||
			g_pData->GetMyUser()->GetSelectUnit()->GetClass() == CX2Unit::UC_ELESIS_PYRO_KNIGHT )
		{
			if( g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_Level >= 35 )
			{
				bElesis35Level = true;
			}
		}
	}
#endif //SERV_SECOND_CLASS_WORLD_WAR_EVENT

	if( IsActive() == true &&																					// 기본적으로 글로벌 미션이 진행 중일때
		(g_pMain->GetNowStateID() == CX2Main::XS_VILLAGE_MAP													// 마을, 던전, 던전 라운지에 있을 때
		|| g_pMain->GetNowStateID() == CX2Main::XS_DUNGEON_ROOM
		|| g_pMain->GetNowStateID() == CX2Main::XS_DUNGEON_GAME
		|| g_pMain->GetNowStateID() == CX2Main::XS_BATTLE_FIELD ) &&											// 배틀필드 추가했음	
		( g_pData == NULL || g_pData->GetCashShop() == NULL || g_pData->GetCashShop()->GetOpen() == false ) &&	// 캐쉬샵이 열지 않았을 때
#ifdef SERV_SECOND_CLASS_WORLD_WAR_EVENT
		( g_pData != NULL && g_pData->GetMyUser() != NULL && ( g_pData->GetMyUser()->GetSelectUnit()->GetClassLevel() == 2 || bElesis35Level == true ) )&&
#endif SERV_SECOND_CLASS_WORLD_WAR_EVENT
		g_pMain->GetIsPlayingTutorial() == false )
	{
		return true;
	}
	else
	{
		return false;
	}
}
#endif SERV_GLOBAL_MISSION_MANAGER