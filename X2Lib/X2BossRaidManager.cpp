#include "StdAfx.h"
#include "X2BossRaidManager.h"

#ifdef FIELD_BOSS_RAID


namespace _CONST_RAID_MANAGER_
{
	const float UPDATE_CYCLE_TIME_CONTRIBUTION_RANKING = 10.f;  // 기여도 순위 갱신 주기
}

CX2BossRaidManager* CX2BossRaidManager::m_pManager = NULL;
/*static */CX2BossRaidManager* CX2BossRaidManager::GetInstance()
{
	if( NULL == m_pManager)
		m_pManager = new CX2BossRaidManager;

	return m_pManager;
}

/*static*/ void CX2BossRaidManager::ResetBossRaidManager()
{
	DestroyInstance();
}

/*static*/ void CX2BossRaidManager::DestroyInstance()
{
	SAFE_DELETE( m_pManager );
}

CX2BossRaidManager::CX2BossRaidManager():
m_bIsOngoingRaid( false ),
m_eCurrentRaidMapID( SEnum::VMI_INVALID ),
m_eBossRaidCreatorMapID( SEnum::VMI_INVALID ),
m_fRaidFieldClearLimitTime ( 0.f ),
m_bShowFieldClearTime(false),
//m_fBanLeftTimeFromRaidField(-1.f),
m_bSendIntrudeRestrictionNot(false)
#ifdef SHOW_CONTRIBUTION_RANKING_AT_RAID_FIELD
,m_fElaspedTimeCalRank(0.f)
#endif // SHOW_CONTRIBUTION_RANKING_AT_RAID_FIELD
//m_bSendNpcLoadReq(false)
{
	m_pFontForTime = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( XUF_HEADLINE_30_NORMAL );
}

CX2BossRaidManager::~CX2BossRaidManager()
{
}
void CX2BossRaidManager::OnFrameMove( double fTime, float fElapsedTime )
{
	if( NULL == g_pMain )
		return;

	switch( g_pMain->GetNowStateID() )
	{
	case CX2Main::XS_BATTLE_FIELD:
		{
			OnFrameMove_BattleField( fTime , fElapsedTime );
		} break;
	default:
		break;
	}
}
void CX2BossRaidManager::OnFrameMove_BattleField( double fTime, float fElapsedTime )
{
	CX2BattleFieldManager BattleFieldManager = g_pData->GetBattleFieldManager();
	// 활성화 중인 배틀필드에서만 동작
	if( SEnum::VMI_INVALID != m_eCurrentRaidMapID &&
		m_eCurrentRaidMapID == BattleFieldManager.GetBattleFieldIdWhereIam() )
	{
		m_bShowFieldClearTime = true;

		// 보스 레이드 체력 10% 이하 일 때 난입 불가 처리
		if( NULL != g_pX2Game &&
			true == g_pX2Game->IsHost() )
		{
			if( true == IsBossHPLessThen10() )
				Handler_EGS_BOSS_FIELD_INTRUDE_RESTRICTION_REQ();
		}
#ifdef SHOW_CONTRIBUTION_RANKING_AT_RAID_FIELD
		// 기여도 순위 갱신
		m_fElaspedTimeCalRank += fElapsedTime;
		if( m_fElaspedTimeCalRank >= _CONST_RAID_MANAGER_::UPDATE_CYCLE_TIME_CONTRIBUTION_RANKING )
		{
			UpdateContributionRanking();
			m_fElaspedTimeCalRank = 0.f;
		}
#endif // SHOW_CONTRIBUTION_RANKING_AT_RAID_FIELD
	}
	else
	{
		m_bShowFieldClearTime = false;
	}

	
	if( GetStandardLeftTime() > 0.f )
	{
		// #1 입장 후 경과 시간 얻기 : 
		// 현재 시간에서, 레이드 필드 입장 시 받은 시간 빼기
		CTimeSpan tElaspedTime = CTime::GetCurrentTime() - GetReceivePacketTime();

		// #2 실제 남은 시간 얻기
		// 입장 했을 때 원래 남은시간에서 입장 후 경과시간 빼기
		float fLeftTime = GetStandardLeftTime() - tElaspedTime.GetTimeSpan() ;

		// #3 실제 남은시간 설정
		SetRaidFieldClearLimitTime( fLeftTime ) ;

		if( fLeftTime < 0.f )
		{
			SetStandardLeftTime(0.f);
		}

#ifdef ADD_RAID_FIELD_LOG
		m_fTestLimitTime -= fElapsedTime;
#endif // ADD_RAID_FIELD_LOG
	}


}
void CX2BossRaidManager::OnFrameRender()
{	
	switch( g_pMain->GetNowStateID() )
	{
	case CX2Main::XS_BATTLE_FIELD:
		{
			OnFrameRender_BattleField();
		} break;
	default:
		break;
	}
}
/** @function : OnFrameRender_BattleField
	@Note : OnFrameRender에서 호출 될 때 스테이트 검사 한다고 판단하고 만듦.
*/
void CX2BossRaidManager::OnFrameRender_BattleField()
{
	if( true == m_bShowFieldClearTime && 
		false == static_cast<CX2BattleFieldGame*>(g_pX2Game)->GetShowLoadUi() )
	{
		const float fRaidFieldClearLimitTime = GetRaidFieldClearLimitTime();
		if( fRaidFieldClearLimitTime > 0.f )
		{			
			// 남은 시간 표기
			WCHAR wszText[64] = L"";
			int iMinute = static_cast<int>(fRaidFieldClearLimitTime / 60.f);
			int iSecond = static_cast<int>(fRaidFieldClearLimitTime-static_cast<float>(iMinute)*60.f);

			if( iSecond < 10 )
				StringCchPrintf(wszText, 64, L"%2d:0%d", iMinute, iSecond ); 
			else
				StringCchPrintf( wszText, 64, L"%2d:%2d", iMinute, iSecond );

			m_pFontForTime->OutTextXY( 512, 60, wszText, D3DXCOLOR(1,1,0,1), 
				CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_CENTER );

#ifdef SHOW_CONTRIBUTION_RANKING_AT_RAID_FIELD
			// 기여도 순위 표기
			RenderRanking();
#endif // SHOW_CONTRIBUTION_RANKING_AT_RAID_FIELD
		}

#ifdef ADD_RAID_FIELD_LOG
		// 테스트를 위해, 이전 남은시간 계산 방법 표기
		if( m_fTestLimitTime > 0.f)
		{
			WCHAR wszText[64] = L"";
			int iMinute = static_cast<int>(m_fTestLimitTime / 60.f);
			int iSecond = static_cast<int>(m_fTestLimitTime-static_cast<float>(iMinute)*60.f);

			if( iSecond < 10 )
				StringCchPrintf(wszText, 64, L"%2d:0%d", iMinute, iSecond ); 
			else
				StringCchPrintf( wszText, 64, L"%2d:%2d", iMinute, iSecond );

			m_pFontForTime->OutTextXY( 512, 100, wszText, D3DXCOLOR(1,1,0,1), 
				CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_CENTER );
		}
#endif // ADD_RAID_FIELD_LOG

#ifdef ADD_RAID_FIELD_LOG
		// 테스트를 위해 보스 체력 표기
		CX2GUNPC* pBossUnit = g_pX2Game->GetNPCUnitByType( GetBossNPCIDByMapID(m_eCurrentRaidMapID) );
		if( NULL != pBossUnit )
		{
			D3DCOLOR colorFont = 0xffffaa77;
			D3DCOLOR colorOutLine = 0xffff0000;
			WCHAR adBuf[200] = {0,};
			StringCchPrintfW( adBuf, ARRAY_SIZE( adBuf ), L"HP: %d/%d", (int)pBossUnit->GetNowHp(), (int)pBossUnit->GetMaxHp() );
			g_pX2Game->GetFontForUnitName()->OutProjectionText( D3DXVECTOR3(0,0,0), adBuf, 	colorFont, CKTDGFontManager::FS_SHELL, colorOutLine, NULL, DT_CENTER  );
		}
#endif // ADD_RAID_FIELD_LOG	
	}
}
bool CX2BossRaidManager::UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( wParam )
	{
	case EGS_BATTLE_FIELD_BOSS_GATE_OPEN_NOT:	// 거대 보스 레이드로 가는 게이트 열림
		{
			return Handler_EGS_BATTLE_FIELD_BOSS_GATE_OPEN_NOT( hWnd, uMsg, wParam, lParam );
		} break;
	case EGS_BATTLE_FIELD_BOSS_GATE_CLOSE_NOT:  // 거대 보스 레이드로 가는 게이트 닫힘
		{
			return Handler_EGS_BATTLE_FIELD_BOSS_GATE_CLOSE_NOT( hWnd, uMsg, wParam, lParam );
		} break;
	case EGS_BOSS_FIELD_CLOSE_PROCESS_NOT:		// 거대 보스 레이드 시간이 종료 되거나 / 거대 보스가 죽음
		{
			return Handler_EGS_BOSS_FIELD_CLOSE_PROCESS_NOT( hWnd, uMsg, wParam, lParam );
		} break;
	case EGS_ADMIN_BOSS_FIELD_GATE_OPEN_ACK:	// 개발자 치트로 게이트가 열림
		{
			return Handler_EGS_ADMIN_BOSS_FIELD_GATE_OPEN_ACK( hWnd, uMsg, wParam, lParam );
		} break;
	case EGS_ADMIN_GET_TOTAL_DANGEROUS_VALUE_ACK: // 개발자 치트 위험도 확인
		{
			return Handler_EGS_ADMIN_GET_TOTAL_DANGEROUS_VALUE_ACK( hWnd, uMsg, wParam, lParam );
		} break;
	case EGS_ADMIN_SET_TOTAL_DANGEROUS_VALUE_ACK: // 개발자 치트 위험도 설정
		{
			return Handler_EGS_ADMIN_SET_TOTAL_DANGEROUS_VALUE_ACK( hWnd, uMsg, wParam, lParam );
		} break;
	case EGS_BOSS_FIELD_INTRUDE_RESTRICTION_ACK: // 체력 10% 이하일 때 난입 불가 설정
		{
			return Handler_EGS_BOSS_FIELD_INTRUDE_RESTRICTION_ACK( hWnd, uMsg, wParam, lParam );
		} break; 
	case EGS_BOSS_FIELD_RETURN_PROCESS_NOT:	// 레이드필드 -> 일반 필드로 강제로 돌아가도록 함
		{
			return Handler_EGS_BOSS_FIELD_RETURN_PROCESS_NOT( hWnd, uMsg, wParam, lParam );
		} break;
	default:
		return false;
		break;
	}
	return false;
}
bool CX2BossRaidManager::Handler_EGS_BATTLE_FIELD_BOSS_GATE_OPEN_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ){
	KSerBuffer* pBuff = reinterpret_cast<KSerBuffer*>(lParam);
	KEGS_BATTLE_FIELD_BOSS_GATE_OPEN_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	m_eCurrentRaidMapID = static_cast<SEnum::VILLAGE_MAP_ID>( kEvent.m_iBossFieldID );
	m_eBossRaidCreatorMapID = static_cast<SEnum::VILLAGE_MAP_ID>( kEvent.m_iBattleFieldID );

	SetIsOngoingRaid(true);

	// 레이드 필드로 가는 게이트 이펙트 생성
	UpdateLinemapPortalEffect();

	// 입구 오픈 엠블럼 설정
	CX2EmblemManager::GetInstance()->PlayEmblem(CX2EmblemManager::EI_OPEN_FIELD_BOSS_RAID, true);
	wstring wstrFieldOpenTextureName = L"";
	GetFieldOpenTextureName(m_eBossRaidCreatorMapID, wstrFieldOpenTextureName);
	CX2EmblemManager::GetInstance()->SetTexture2ndName(CX2EmblemManager::EI_OPEN_FIELD_BOSS_RAID, wstrFieldOpenTextureName );

	// 입구 클로즈 엠블럼 설정
	CX2EmblemManager::GetInstance()->EndEmblem(CX2EmblemManager::EI_CLOSE_FIELD_BOSS_RAID);

	return true;
}bool CX2BossRaidManager::Handler_EGS_BATTLE_FIELD_BOSS_GATE_CLOSE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = reinterpret_cast<KSerBuffer*>(lParam);
	KEGS_BATTLE_FIELD_BOSS_GATE_CLOSE_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	m_eBossRaidCreatorMapID = static_cast<SEnum::VILLAGE_MAP_ID>( kEvent.m_iBattleFieldID );

	SetIsOngoingRaid(false);
	UpdateLinemapPortalEffect();

	// #NOTE Map ID 값 초기화는 EI_OPEN_FIELD_BOSS_RAID 생성 후 이뤄지도록 처리 함. 
	CX2EmblemManager::GetInstance()->PlayEmblem(CX2EmblemManager::EI_CLOSE_FIELD_BOSS_RAID, true);
	CX2EmblemManager::GetInstance()->EndEmblem(CX2EmblemManager::EI_OPEN_FIELD_BOSS_RAID);

	return true;
}
/** @function : Handler_EGS_BOSS_FIELD_CLOSE_PROCESS_NOT
	@brief : 보스몬스터가 죽거나 / 시간이 종료 되었을 때 받는 패킷
*/
bool CX2BossRaidManager::Handler_EGS_BOSS_FIELD_CLOSE_PROCESS_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
 	if( NULL != g_pX2Game )
 	{
 		CX2GUNPC* pBossUnit = g_pX2Game->GetNPCUnitByType( GetBossNPCIDByMapID(m_eCurrentRaidMapID) );
 		if( NULL != pBossUnit )
 		{
			// 체력이 충분한데 패킷을 받았다면 도망가는 스테이트로 변경
			if( 0.f < pBossUnit->GetNowHp() )
 				pBossUnit->StateChange( "FIELD_BOSS_IRON_ELTRION_ESCAPE", true);

			CX2GageManager::GetInstance()->ClearBossGageUIList();
			g_pChatBox->AddChatLog( GET_STRING(STR_ID_29065), KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
			SetRaidFieldClearLimitTime(0.f);
			SetStandardLeftTime(0.f);


#ifdef ADD_RAID_FIELD_LOG
			// 확인을 위한 테스트용 로그 팝업
			if( 0.f < pBossUnit->GetNowHp() )
			{
				if( GetRaidFieldClearLimitTime() > 30.f )
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), L"시간이 30초이상 남았는데 종료 패킷을 받았습니다.", g_pMain->GetNowState() );
			}
			else
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), L"엘트리온을 처치하였습니다.", g_pMain->GetNowState() );
			}
#endif // ADD_RAID_FIELD_LOG
 		}
 	}
	return true;
}
bool CX2BossRaidManager::Handler_EGS_BOSS_FIELD_RETURN_PROCESS_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if( CX2Main::XS_BATTLE_FIELD == g_pMain->GetNowStateID() )
		static_cast<CX2StateBattleField*>(g_pMain->GetNowState())->MoveToBeforePlaceAtRaidField();

	return true;
}
void CX2BossRaidManager::Handler_EGS_BOSS_FIELD_INTRUDE_RESTRICTION_REQ()
{
	if( false == g_pMain->IsWaitingServerPacket(EGS_BOSS_FIELD_INTRUDE_RESTRICTION_ACK) )
	{
		if( false == m_bSendIntrudeRestrictionNot )
		{
			g_pData->GetServerProtocol()->SendID( EGS_BOSS_FIELD_INTRUDE_RESTRICTION_REQ );
			g_pMain->AddServerPacket( EGS_BOSS_FIELD_INTRUDE_RESTRICTION_ACK ); 
		}
	}
}
bool CX2BossRaidManager::Handler_EGS_BOSS_FIELD_INTRUDE_RESTRICTION_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = reinterpret_cast<KSerBuffer*>(lParam);
	KEGS_BOSS_FIELD_INTRUDE_RESTRICTION_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if ( g_pMain->DeleteServerPacket( EGS_BOSS_FIELD_INTRUDE_RESTRICTION_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
#ifdef ADD_RAID_FIELD_LOG
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), L"몬스터 체력 10% 이하로 난입 불가 설정 함", g_pMain->GetNowState() );
#endif // ADD_RAID_FIELD_LOG
			SetSendIntrudeRestrictionNot(true);
			return true;
		}
	}

	return true;
}

/** @function : InitRaidFieldMapID
	@brief : 레이드 필드 포탈을 생성한 MapID 초기화.
			 종료 엠블럼 정상적으로 보여주기 위해, 엠블럼 종료 구문에서 초기화 처리 함
*/
void CX2BossRaidManager::InitRaidFieldMapID()
{
	m_eCurrentRaidMapID = SEnum::VMI_INVALID;
	m_eBossRaidCreatorMapID = SEnum::VMI_INVALID;
}
/** @function : IsActiveRaidPortal
	@brief : 레이드 포탈 활성화 여부 검사.
*/
bool CX2BossRaidManager::IsActiveRaidPortal() const
{

	// 1. 현재 내가 필드에 위치해 있는가
	if( CX2Main::XS_BATTLE_FIELD != g_pMain->GetNowStateID() )
		return false;

	// 2. 필드 레이드 진행중인가.
	if( false == GetIsOngoingRaid() )
		return false;

	// 3. 현재 필드가 보스레이드 생성 필드가 맞는가
	if( m_eBossRaidCreatorMapID != g_pData->GetBattleFieldManager().GetBattleFieldIdWhereIam() )
		return false;

	return true;
}

void CX2BossRaidManager::OpenCheat()
{
	Handler_EGS_BATTLE_FIELD_BOSS_GATE_OPEN_NOT(0,0,0,0);
}
void CX2BossRaidManager::CloseCheat()
{
	Handler_EGS_BATTLE_FIELD_BOSS_GATE_CLOSE_NOT(0,0,0,0);
}
/** @function : UpdateLinemapPortalEffect
	@brief : 포탈 이펙트 갱신.
			 해당 필드에 레이드 필드 입구가 열리면, 이펙트 생성 할 때 사용
*/
void CX2BossRaidManager::UpdateLinemapPortalEffect()
{
	if( CX2Main::XS_BATTLE_FIELD == g_pMain->GetNowStateID() )
	{
		// #1 현재 필드가 보스레이드 생성하는 필드인가?
		if( true ) 
		{
			// #2 보스레이드 입구 이펙트 갱신
			if( NULL != g_pX2Game )
			{
				CX2BattleFieldGame* pBattleFieldGame = static_cast<CX2BattleFieldGame*>(g_pX2Game);
				if( NULL != pBattleFieldGame )
				{
					if( true == GetIsOngoingRaid() )
						pBattleFieldGame->CreateRaidPotal();
					else
						pBattleFieldGame->DestoryRaidPotalParticle();
				}
			}
		}
	}
}
void CX2BossRaidManager::GetFieldOpenTextureName( IN SEnum::VILLAGE_MAP_ID eBossRaidCreatorMapID_, OUT wstring& wstrOpenTextureName_ ) const
{
	switch( eBossRaidCreatorMapID_ )
	{
	case SEnum::VMI_BATTLE_FIELD_HAMEL_FIELD_01:	wstrOpenTextureName_ = L"RaidField_Open_Hamel_0.dds"; break;
	case SEnum::VMI_BATTLE_FIELD_HAMEL_FIELD_02:	wstrOpenTextureName_ = L"RaidField_Open_Hamel_1.dds"; break;
	case SEnum::VMI_BATTLE_FIELD_HAMEL_FIELD_03:	wstrOpenTextureName_ = L"RaidField_Open_Hamel_2.dds"; break;
	case SEnum::VMI_BATTLE_FIELD_HAMEL_FIELD_04:	wstrOpenTextureName_ = L"RaidField_Open_Hamel_3.dds"; break;
	case SEnum::VMI_BATTLE_FIELD_SANDER_FIELD_01:	wstrOpenTextureName_ = L"RaidField_Open_Sander_0.dds"; break;
	case SEnum::VMI_BATTLE_FIELD_SANDER_FIELD_02:	wstrOpenTextureName_ = L"RaidField_Open_Sander_1.dds"; break;
	case SEnum::VMI_BATTLE_FIELD_SANDER_FIELD_03:	wstrOpenTextureName_ = L"RaidField_Open_Sander_2.dds"; break;
	case SEnum::VMI_BATTLE_FIELD_SANDER_FIELD_04:	wstrOpenTextureName_ = L"RaidField_Open_Sander_3.dds"; break;
	default:
		break;
	}
}
/** @function : JoinRaidFieldProcess
	@brief : 레이드 필드 입장에 대한 예외처리
*/
void CX2BossRaidManager::JoinFieldProcess( float fFieldClearLeftTime_ )
{
// 일반 필드->필드 입장 시 처리
	// 던전/대전 버튼 제거
	if( NULL != g_pMain && 
		NULL != g_pMain->GetPartyUI() )
	{
		g_pMain->GetPartyUI()->UpdatePartyMenu();
	}

// 레이드 필드로 입장 했을 때만 처리
	if( true == g_pData->GetBattleFieldManager().GetIsBossRaidCurrentField() )
	{
		// 매칭 취소
		if( NULL != g_pData &&
			NULL != g_pData->GetPartyManager() )
		{
			g_pData->GetPartyManager()->Handler_EGS_CANCEL_MATCH_MAKING_REQ();
			g_pData->GetPartyManager()->Handler_EGS_CANCEL_AUTO_PARTY_MAKING_REQ();
		}
	}
#ifdef ADD_RAID_FIELD_LOG
	// 테스트를 위해 예전 남은 시간 계산하던 루틴 유지.
	m_fTestLimitTime = fFieldClearLeftTime_;
#endif // ADD_RAID_FIELD_LOG

	// 입장했을 때 레이드 필드 남은 시간 저장
	SetStandardLeftTime(fFieldClearLeftTime_);

	// Npc Load REQ 패킷 전송 여부
	//SetSendNpcLoadReq(false);

	// 현재 레이드 필드 남은 시간 초기화
	SetRaidFieldClearLimitTime(0.f);
	
	// 체력 10% 이하 일 때 난입 불가 패킷 전송 여부 초기화
	SetSendIntrudeRestrictionNot(false);

	SetReceivePacketTime(CTime::GetCurrentTime());

#ifdef SHOW_CONTRIBUTION_RANKING_AT_RAID_FIELD
	// 기여도 순위 초기화
	m_vecContributionRanking.clear();
#endif // SHOW_CONTRIBUTION_RANKING_AT_RAID_FIELD
}
/** @function : IsBossHPLessThen10
	@brief : 보스HP 10퍼센트 이하에 대한 검사.
			 필드에 따라 보스가 여러 마리가 될 수 있기 때문에 함수로 제작 함
*/

bool CX2BossRaidManager::IsBossHPLessThen10() const
{
	if( NULL != g_pX2Game )
	{
		CX2UnitManager::NPC_UNIT_ID const eBossNPCID = GetBossNPCIDByMapID(static_cast<SEnum::VILLAGE_MAP_ID>(g_pData->GetBattleFieldManager().GetBattleFieldIdWhereIam()));
		CX2GUNPC* pBossUnit = g_pX2Game->GetNPCUnitByType( eBossNPCID );
		if( NULL != pBossUnit )
		{
			if( 0.1f >= (pBossUnit->GetNowHp()/pBossUnit->GetMaxHp()) )
			{
				return true;
			}
		}
		else
		{
			return false;
		}
	}

	return false;
}
void CX2BossRaidManager::Handler_EGS_ADMIN_BOSS_FIELD_GATE_OPEN_REQ() // 보스 필드 포탈을 생성
{
	if( CX2Main::XS_BATTLE_FIELD == g_pMain->GetNowStateID() )
	{
		KEGS_ADMIN_BOSS_FIELD_GATE_OPEN_REQ kPacket;
		kPacket.m_iBattleFieldID = g_pData->GetBattleFieldManager().GetBattleFieldIdWhereIam();

		g_pData->GetServerProtocol()->SendPacket( EGS_ADMIN_BOSS_FIELD_GATE_OPEN_REQ, kPacket );
		g_pMain->AddServerPacket( EGS_ADMIN_BOSS_FIELD_GATE_OPEN_ACK ); 
	}
	else
	{
		g_pChatBox->AddChatLog( L"필드에서만 사용 할 수 있습니다.", KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
	}
}
bool CX2BossRaidManager::Handler_EGS_ADMIN_BOSS_FIELD_GATE_OPEN_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = reinterpret_cast<KSerBuffer*>(lParam);
	KEGS_ADMIN_BOSS_FIELD_GATE_OPEN_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if ( g_pMain->DeleteServerPacket( EGS_ADMIN_BOSS_FIELD_GATE_OPEN_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			return true;
		}
	}
	return true;
}
void CX2BossRaidManager::Send_EGS_ADMIN_BOSS_FIELD_GATE_CLOSE_NOT() // 보스 필드 포탈을 닫기
{
	g_pData->GetServerProtocol()->SendID( EGS_ADMIN_BOSS_FIELD_GATE_CLOSE_NOT );
}
// 배틀 필드의 TotalDangerousValue를 알아내기
void CX2BossRaidManager::Handler_EGS_ADMIN_GET_TOTAL_DANGEROUS_VALUE_REQ() 
{
	if( CX2Main::XS_BATTLE_FIELD == g_pMain->GetNowStateID() )
	{
		KEGS_ADMIN_GET_TOTAL_DANGEROUS_VALUE_REQ kPacket;
		kPacket.m_iBattleFieldID = g_pData->GetBattleFieldManager().GetBattleFieldIdWhereIam();

		g_pData->GetServerProtocol()->SendPacket( EGS_ADMIN_GET_TOTAL_DANGEROUS_VALUE_REQ, kPacket );
		g_pMain->AddServerPacket( EGS_ADMIN_GET_TOTAL_DANGEROUS_VALUE_ACK ); 
	}
	else
	{
		g_pChatBox->AddChatLog( L"필드에서만 사용 할 수 있습니다.", KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
	}
}
// 배틀 필드의 TotalDangerousValue를 알아내기
bool CX2BossRaidManager::Handler_EGS_ADMIN_GET_TOTAL_DANGEROUS_VALUE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = reinterpret_cast<KSerBuffer*>(lParam);
	KEGS_ADMIN_GET_TOTAL_DANGEROUS_VALUE_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if ( g_pMain->DeleteServerPacket( EGS_ADMIN_GET_TOTAL_DANGEROUS_VALUE_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			if( kEvent.m_byteTotalDangerousValue >= 0 )
			{
				g_pChatBox->AddChatLog( GET_REPLACED_STRING( ( STR_ID_19208, "i", kEvent.m_byteTotalDangerousValue ) ),
					KEGS_CHAT_REQ::CPT_TOTAL, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
			}	
			
			return true;
		}
	}
	return true;
}
// 배틀 필드의 TotalDangerousValue를 설정하기 위한 치트 패킷
void CX2BossRaidManager::Handler_EGS_ADMIN_SET_TOTAL_DANGEROUS_VALUE_REQ( int byDangerousValue_ ) 
{
	if( CX2Main::XS_BATTLE_FIELD == g_pMain->GetNowStateID() )
	{
		KEGS_ADMIN_SET_TOTAL_DANGEROUS_VALUE_REQ kPacket;
		kPacket.m_iTotalDangerousValue = byDangerousValue_;
		kPacket.m_iBattleFieldID = g_pData->GetBattleFieldManager().GetBattleFieldIdWhereIam();

		// true : 대입
		kPacket.m_bSet = true;

		g_pData->GetServerProtocol()->SendPacket( EGS_ADMIN_SET_TOTAL_DANGEROUS_VALUE_REQ, kPacket );
		g_pMain->AddServerPacket( EGS_ADMIN_SET_TOTAL_DANGEROUS_VALUE_ACK ); 
	}
	else
	{
		g_pChatBox->AddChatLog( L"필드에서만 사용 할 수 있습니다.", KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
	}
}
bool CX2BossRaidManager::Handler_EGS_ADMIN_SET_TOTAL_DANGEROUS_VALUE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = reinterpret_cast<KSerBuffer*>(lParam);
	KEGS_ADMIN_SET_TOTAL_DANGEROUS_VALUE_ACK kEvent;
	DeSerialize( pBuff, &kEvent );
	if ( g_pMain->DeleteServerPacket( EGS_ADMIN_SET_TOTAL_DANGEROUS_VALUE_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			return true;
		}
	}
	return true;
}

/** @function : GetCameraIndex
	@brief : 게이트 위치를 비추는 카메라 인덱스
*/
int CX2BossRaidManager::GetCameraIndex( IN SEnum::VILLAGE_MAP_ID eFieldMapID_  ) const
{
	switch( eFieldMapID_ )
	{
	case SEnum::VMI_BATTLE_FIELD_HAMEL_FIELD_01:	return 4; break;
	case SEnum::VMI_BATTLE_FIELD_HAMEL_FIELD_02:	return 2; break;
	case SEnum::VMI_BATTLE_FIELD_HAMEL_FIELD_03:	return 4; break;
	case SEnum::VMI_BATTLE_FIELD_HAMEL_FIELD_04:	return 5; break;
	case SEnum::VMI_BATTLE_FIELD_SANDER_FIELD_01:	return 4; break;
	case SEnum::VMI_BATTLE_FIELD_SANDER_FIELD_02:	return 5; break;
	case SEnum::VMI_BATTLE_FIELD_SANDER_FIELD_03:	return 3; break;
	case SEnum::VMI_BATTLE_FIELD_SANDER_FIELD_04:	return 6; break;
	default:
		return -1;
		break;
	}

	return -1;
}

/** @function : GetLoadingUIFileName
	@brief : 현재 활성화된 레이드 필드의 로딩 UI 파일 이름 얻기
*/
void CX2BossRaidManager::GetLoadingUIFileName( OUT wstring& wstrImgLFileName_, OUT wstring& wstrImgRFileName_) const
{
	switch( m_eCurrentRaidMapID )
	{
	case SEnum::VMI_FIELD_BOSSRAID_KINGDOM_OF_NASOD:	
		wstrImgLFileName_ = L"HQ_LoadingField_Boss_Gate_01.tga";
		wstrImgRFileName_ = L"HQ_LoadingField_Boss_Gate_02.tga";
		break;
	case SEnum::VMI_FIELD_BOSSRAID_VALLEY_OF_ABYSS:	
		wstrImgLFileName_ = L"HQ_LoadingField_Boss_Gate_01.tga";
		wstrImgRFileName_ = L"HQ_LoadingField_Boss_Gate_02.tga";
		break;
	default:
		wstrImgLFileName_ = L"HQ_LoadingField_Boss_Gate_01.tga";
		wstrImgRFileName_ = L"HQ_LoadingField_Boss_Gate_02.tga";
		break;
	}
}
// 월드가 변경되지 않은 채로, 보스가 생성되려고 할 때의 예외처리.
// TODO : 엘트리온 제외 보스 몬스터에 대해서도 처리가 되어야 함.
// TODO : 지금은 필드->레이드 필드 에서만 동작 함.레이드 필드->필드도 처리 해야 함.
void CX2BossRaidManager::CreateNpcExceptProcess( const CX2UnitManager::NPC_UNIT_ID eNpcID_ ) const
{
	switch(eNpcID_)
	{
	case CX2UnitManager::NUI_FIELD_BOSS_IRON_ELTRION:
		{
			// 엘트리온은 무조건 WI_FIELD_BOSSRAID_KINGDOM_OF_NASOD에서만 생성 됨
			if( g_pX2Game->GetWorld()->GetWorldData()->worldID != CX2World::WI_FIELD_BOSSRAID_KINGDOM_OF_NASOD )
			{
#ifdef ADD_RAID_FIELD_LOG
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), L"레이드 필드가 아닌 곳에서, 엘트리온이 소환되었습니다.", g_pMain->GetNowState() );
#endif // ADD_RAID_FIELD_LOG

				if( CX2Main::XS_BATTLE_FIELD == g_pMain->GetNowStateID() )
				{
					static_cast<CX2StateBattleField*>(g_pMain->GetNowState())->SwapWorld();

					if( NULL != g_pX2Game->GetMyUnit() )
						g_pX2Game->GetMyUnit()->InitPosition( false, 1 );
				}
			}
		} break;
	default:
		break;
	}
}
CX2UnitManager::NPC_UNIT_ID CX2BossRaidManager::GetBossNPCIDByMapID( SEnum::VILLAGE_MAP_ID eFieldMapID_ ) const
{
	switch ( eFieldMapID_ )
	{
	case SEnum::VMI_FIELD_BOSSRAID_KINGDOM_OF_NASOD: return CX2UnitManager::NUI_FIELD_BOSS_IRON_ELTRION; break;
	default:
		break;
	}

	return CX2UnitManager::NUI_NONE;
}
#ifdef SHOW_CONTRIBUTION_RANKING_AT_RAID_FIELD
void CX2BossRaidManager::RenderRanking() const
{
	if( false == g_pKTDXApp->GetDGManager()->GetDialogManager()->GetHideDialog() &&
		true == g_pData->GetUIManager()->GetShowUI() )
	{
		if( false == m_vecContributionRanking.empty() )
		{
			WCHAR wstrRanking[200] = {0,};
			int iIndex = 0;
			int iPosY = 48;

#pragma NOTE("본섭 패치 된다면 문자열 스트링툴로 뽑기")
			g_pMain->GetFontForDebuf()->OutTextXY( 829, iPosY-20, L"기여도 순위", D3DXCOLOR(1,1,0,1), 
				CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_LEFT);
			BOOST_FOREACH(const wstring& wstrUnitName, m_vecContributionRanking )
			{
				swprintf( wstrRanking, L"%d위 : %s", ++iIndex, wstrUnitName.c_str() );
				if( g_pMain->GetFontForDebuf() != NULL )
					g_pMain->GetFontForDebuf()->OutTextXY( 829, iPosY, wstrRanking, D3DXCOLOR(1,1,0,1), 
					CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_LEFT);

				iPosY += 16;
			}
		}
	}
}
void CX2BossRaidManager::UpdateContributionRanking()
{
	bool bIsActive = true;

	// 1. 현재 내가 필드에 위치해 있는가
	if( CX2Main::XS_BATTLE_FIELD != g_pMain->GetNowStateID() )
		bIsActive = false;

	if( true == bIsActive )
	{
		CX2GUNPC* pBossUnit = g_pX2Game->GetNPCUnitByType( GetBossNPCIDByMapID(m_eCurrentRaidMapID) );
		if( NULL != pBossUnit )
		{
			// 데미지 를 key 로 둔 이유는 상위 5명을 선정하기 위해 자동 정렬 시킴
			std::map< float, UidType, std::greater<float> > mapDamage;


			// #1 데미지 순으로 1~5위 구하기
			std::map<UidType,float> mapDamagedMap = pBossUnit->GetDamagedMap();
			std::map<UidType,float>::const_iterator iter = mapDamagedMap.begin();			
			for ( ; iter != mapDamagedMap.end(); ++iter )
			{
				mapDamage.insert( std::make_pair( iter->second, iter->first ) );
			} 

			m_vecContributionRanking.clear();			
			// #2 순위별로 닉네임 저장하기
			if( false == mapDamage.empty() )
			{
				m_vecContributionRanking.reserve( mapDamage.size() );
				std::map< float, UidType, std::greater<float> >::iterator mit = mapDamage.begin();
				for( ; mit != mapDamage.end(); ++mit )
				{
					CX2GUUser* pGUUser = g_pX2Game->GetUserUnitByUID( mit->second );
					if( NULL != pGUUser )
					{
						m_vecContributionRanking.push_back( pGUUser->GetUnitName() );
						if( m_vecContributionRanking.size() >= 5 )
							break;
					}
				}
			}
		}
	}
	else
	{
		m_vecContributionRanking.clear();
	}
}
#endif // SHOW_CONTRIBUTION_RANKING_AT_RAID_FIELD
// bool CX2BossRaidManager::IsValideNpcLoadReq() const
// {
// 	// #1 현재 보스레이드 필드인가?
// 	if( true == g_pData->GetBattleFieldManager().GetIsBossRaidCurrentField() )
// 	{
// 		// #2 이미 패킷을 보냈는가?
// 		if( true == GetSendNpcLoadReq() ) 	
// 			return false;
// 
// 		// #3 조건 걸리지 않았으면 true 리턴
// 		return true;
// 	}
// 
// 	// #4 보스레이드 필드가 아니면 무조건 true
// 	return true;
// }
#endif // FIELD_BOSS_RAID
