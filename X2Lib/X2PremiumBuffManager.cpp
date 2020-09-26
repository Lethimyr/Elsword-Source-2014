/**@file	X2PremiumBuff.cpp
   @breif	휴면 복귀 유저 버프 및 기타 특수 버프 관리 
*/
#pragma once

#include "StdAfx.h"
#include ".\X2PremiumBuffManager.h"

#ifdef COME_BACK_REWARD

#pragma region CX2PremiumBuffManager
/** @function 	: CX2PremuimBuff
	@brief 		: 생성자
	@param		: 없음
	@return		: 없음
*/
CX2PremiumBuffManager::CX2PremiumBuffManager()
	//{{ kimhc // 2011-08-08 // 투니 랜드 채널링 이벤트
#ifdef	SERV_TOONILAND_CHANNELING_EVENT
//: m_bTooniLandChannelingEvent( false )	/// 투니랜드 채널링 이벤트 동안 신규 계정을 생성한 유저
#endif	SERV_TOONILAND_CHANNELING_EVENT
//}} kimhc // 2011-08-08 // 투니 랜드 채널링 이벤트
#ifdef BUFF_TEMPLET_SYSTEM
: m_bBuffTempletUI( false )
#endif
{
	Clear();
}
#pragma endregion CX2PremiumBuffManager

#pragma region ~CX2PremiumBuffManager
/** @function 	: ~CX2PremuimBuff
	@brief 		: 소멸자
	@param		: 없음
	@return		: 없음
*/
CX2PremiumBuffManager::~CX2PremiumBuffManager()
{
	SAFE_DELETE_DIALOG( m_pDLGWelcomeComeBackUser );
	SAFE_DELETE_DIALOG( m_pDLGComeBackUserBuffDesc );
}
#pragma endregion ~CX2PremiumBuffManager

#pragma region Clear
/** @function 	: Clear
	@brief 		: 멤버 초기화
	@param		: 없음
	@return		: 없음
*/
void CX2PremiumBuffManager::Clear()
{
	//{{ bool
	m_bComeBackUser				= false;
	m_bDescComplete				= false;
	m_bChatViewFirst			= true;
	m_bFirstComeBack			= false;
	m_bFirstComeBackBuffDesc	= false;
	m_bPremiumPC				= false;
	m_bPremiumBuffUI			= false;
	m_iComeBackBuffEnd			= false;
	m_bComeBackuserInParty		= false;

	//{{ int
	m_iComeBackBuffEnd			= 0;
	//}}

	//{{ Pointer
	m_pDLGWelcomeComeBackUser	= NULL;
	m_pDLGComeBackUserBuffDesc	= NULL;
	//}}

#ifdef SERV_PC_BANG_TYPE
	m_kPcBangReward.m_bAvatar					= false;
	m_kPcBangReward.m_iPcBangType				= -1;
	m_kPcBangReward.m_fEXPRate					= 0.0f;
	m_kPcBangReward.m_fEDRate					= 0.0f;
	m_kPcBangReward.m_fVSPointRate				= 0.0f;
	m_kPcBangReward.m_bSpiritFree				= false;
	m_kPcBangReward.m_fPetEXPRate				= 0.0f;
	m_kPcBangReward.m_iAdditionalHenirReward	= 0;
	m_kPcBangReward.m_bSecretDungeonFree		= false;
	m_kPcBangReward.m_bAvatar					= false;
	m_kPcBangReward.m_bTitle					= false;
#endif SERV_PC_BANG_TYPE
}
#pragma endregion Clear

#pragma region IsComeBackUser
/** @function 	: IsComeBackUser
	@brief 		: 휴면 복귀 유저 판단
	@param		: 없음
	@return		: bool
*/
bool CX2PremiumBuffManager::IsComeBackUser() const
{
	return m_bComeBackUser;
}
#pragma endregion IsComeBackUser

#pragma region SetComeBackUser
/** @function 	: SetComeBackUser
	@brief 		: 휴면 복귀 유저 설정
	@param		: bool
	@return		: 없음
*/
void CX2PremiumBuffManager::SetComeBackUser( IN bool bComeBackUser_ )
{
	m_bComeBackUser = bComeBackUser_;
}
#pragma endregion SetComeBackUser

#pragma region GetComeBackBuffEnd
/** @function 	: GetComeBackBuffEnd
	@brief 		: 버프 남은 시간 값을 넘김
	@return		: ref INT64
*/
INT64 CX2PremiumBuffManager::GetComeBackBuffEnd() const
{
	return m_iComeBackBuffEnd;
}
#pragma endregion GetComeBackBuffEnd

#pragma region IsFirstComeBack
/** @function 	: IsFirstComeBack
	@brief 		: 휴면 복귀 유저 첫 접속 여부 확인
	@param		: 없음
	@return		: bool
*/
bool CX2PremiumBuffManager::IsFirstComeBack() const
{
	return m_bFirstComeBack;
}
#pragma endregion IsFirstComeBack

#pragma region IsFirstComeBackBuffDesc
/** @function 	: IsFirstComeBackBuffDesc
	@brief 		: 휴면 복귀 유저 첫 접속 설명 표시 여부 확인
	@param		: 없음
	@return		: bool
*/
bool CX2PremiumBuffManager::IsFirstComeBackBuffDesc() const
{
	return m_bFirstComeBackBuffDesc;
}
#pragma endregion IsFirstComeBackBuffDesc

#pragma region IsDescComplete
/** @function 	: IsDescComplete
	@brief 		: Premium Buff 설명 확인 완료 후 타이틀 활성화
	@return		: bool
*/
bool CX2PremiumBuffManager::IsDescComplete() const
{
	return m_bDescComplete;
}
#pragma endregion IsDescComplete

#pragma region SetFirstComeBack
/** @function 	: SetFirstComeBack
	@brief 		: 휴면 복귀 유저 첫 접속 설정
	@param		: bool
	@return		: 없음
*/
void CX2PremiumBuffManager::SetFirstComeBack( IN bool bFirstComeBack_ )
{
	m_bFirstComeBack = m_bFirstComeBackBuffDesc = bFirstComeBack_;
}
#pragma endregion SetFirstComeBack

#pragma region SetShowFirstComBackUI
/** @function 	: SetShowFirstComBackUI
	@brief 		: 휴면 복귀 유저 첫 접속 설정
	@param		: bool
	@return		: 없음
*/
void CX2PremiumBuffManager::SetShowFirstComBackUI( IN bool bShow_ )
{
	switch ( g_pMain->GetNowStateID() )
	{
	case CX2Main::XS_SERVER_SELECT:
		if ( NULL != m_pDLGWelcomeComeBackUser )
		{
#ifdef NO_UI_COMEBACK_USER
			m_pDLGWelcomeComeBackUser->SetShowEnable( false, false );
#else
			m_pDLGWelcomeComeBackUser->SetShowEnable( true, true );
#endif NO_UI_COMEBACK_USER
		}
		break;
	case CX2Main::XS_VILLAGE_MAP:
	case CX2Main::XS_BATTLE_FIELD:
		if ( NULL != m_pDLGComeBackUserBuffDesc )
		{
#ifdef NO_UI_COMEBACK_USER
			m_pDLGComeBackUserBuffDesc->SetShowEnable( false, false );
#else
			m_pDLGComeBackUserBuffDesc->SetShowEnable( true, true );
#endif NO_UI_COMEBACK_USER
		}
		break;
	}
}
#pragma endregion SetShowFirstComBackUI

#pragma region IsPremiumPC
/** @function 	: IsPremiumPC
	@brief 		: PC방인지 확인
	@param		: 없음
	@return		: bool
*/
bool CX2PremiumBuffManager::IsPremiumPC() const
{
	return m_bPremiumPC;
}
#pragma endregion IsPremiumPC

#pragma region SetPremiumPC
/** @function 	: SetPremiumPC
	@brief 		: PC방 설정
	@param		: bool
	@return		: 없음
*/
void CX2PremiumBuffManager::SetPremiumPC( IN bool bPremiumPC_ )
{
	m_bPremiumPC = bPremiumPC_;
}
#pragma endregion SetPremiumPC

#pragma region GetShowPremiumBuffUI
/** @function 	: GetShowPremiumBuffUI
	@brief 		: 프리미엄 버프 표시 확인
	@param		: 없음
	@return		: bool
*/
bool CX2PremiumBuffManager::GetShowPremiumBuffUI() const
{
	return m_bPremiumBuffUI;
}
#pragma endregion GetShowPremiumBuffUI

#pragma region SetShowPremiumBuffUI
/** @function 	: SetShowPremiumBuffUI
	@brief 		: 프리미엄 버프 표시 설정
	@param		: bool
	@return		: 없음
*/
bool CX2PremiumBuffManager::SetShowPremiumBuffUI( IN bool bShow_ )
{
/*
	switch( g_pMain->GetNowStateID() )
	{
	case CX2Main::XS_VILLAGE_MAP:
	case CX2Main::XS_DUNGEON_GAME:
	//case CX2Main::XS_SERVER_SELECT:
		// 마을과 던전인 경우에만 다음 부분을 수행
		break;

	default:
		// 그외의 State에서는 그냥 return
		return false;
		break;
	}

	CX2StateMenu* pStateMenu = static_cast<CX2StateMenu*>( g_pMain->GetNowState() );

	if ( NULL == pStateMenu )
		return false;
	
	if ( pStateMenu->SetShowPremiumBuffUI( bShow_ ) )
	{		
		m_bPremiumBuffUI = bShow_;
		return true;
	}
	else
	{
		m_bPremiumBuffUI = bShow_;
		return false;
	}
*/

	return false;
}
#pragma endregion SetShowPremiumBuffUI

#pragma region GetViewBuffChat
/** @function 	: GetViewBuffChat
	@brief 		: 채팅창 버프 설명 관련
	@return		: bool
*/
bool CX2PremiumBuffManager::GetViewBuffChat() const
{
	return m_bChatViewFirst;
}
#pragma endregion GetViewBuffChat

#pragma region SetViewBuffChat
/** @function 	: SetViewBuffChat
	@brief 		: 채팅창 버프 설명 관련
	@param		: bool
*/
void CX2PremiumBuffManager::SetViewBuffChat( IN bool bShow_ )
{
	m_bChatViewFirst = bShow_;
}
#pragma endregion SetViewBuffChat

#pragma region IsComeBackUserInParty
/** @function 	: IsComeBackUserInParty
	@brief 		: 파티 중에 휴면 복귀 유저가 있는지 확인
	@return		: bool
*/
bool CX2PremiumBuffManager::IsComeBackUserInParty() const
{
	return m_bComeBackuserInParty;
}
#pragma endregion IsComeBackUserInParty

#pragma region SetComeBackUserInParty
/** @function 	: SetComeBackUserInParty
	@brief 		: 파티 중에 휴면 복귀 유저가 있는지 확인
	@param		: bool
*/
void CX2PremiumBuffManager::SetComeBackUserInParty( IN bool bShow_ )
{
	m_bComeBackuserInParty = bShow_;
}
#pragma endregion SetComeBackUserInParty

#pragma region UpdateGuideDescPremiumBuffUI
/** @function 	: UpdateGuideDescPremiumBuffUI
	@brief 		: 버프 아이콘 출력
*/
void CX2PremiumBuffManager::UpdateGuideDescPremiumBuffUI()
{
	switch( g_pMain->GetNowStateID() )
	{
	case CX2Main::XS_VILLAGE_MAP:
	case CX2Main::XS_DUNGEON_GAME:
		// 마을과 던전인 경우에만 다음 부분을 수행
		break;

	default:
		// 그외의 State에서는 그냥 return
		return;
		break;
	}

#ifdef CHECK_INHERIT_STATE_MENU
	if( g_pMain->IsInheritStateMenu() )
#endif //CHECK_INHERIT_STATE_MENU
	{
		CX2StateMenu* pStateMenu = static_cast<CX2StateMenu*>( g_pMain->GetNowState() );

		if ( NULL == pStateMenu )
			return;
		else
			pStateMenu->UpdateGuideDescPremiumBuffUI();
	}
}
#pragma endregion UpdateGuideDescPremiumBuffUI

#pragma region UICustomEventProc
/** @function 	: UICustomEventProc
	@brief 		: UI 이벤트
	@param		: procedure
	@return		: bool
*/
bool CX2PremiumBuffManager::UICustomEventProc( IN HWND hWnd, IN UINT uMsg, IN WPARAM wParam, IN LPARAM lParam )
{
	switch ( wParam )
	{
	case SCBUCM_WELCOME_OK:
		{
			if ( NULL != m_pDLGWelcomeComeBackUser )
			{
				m_bFirstComeBack = false;
				m_pDLGWelcomeComeBackUser->SetShowEnable( false, false );
			}
		} break;
	case SCBUCM_COME_BACK_BUFF_DESC_OK:
		{
			if ( NULL != m_pDLGComeBackUserBuffDesc )
			{
				m_bDescComplete = true;
				m_bFirstComeBackBuffDesc = false;
				m_pDLGComeBackUserBuffDesc->SetShowEnable( false, false );
#ifdef SERV_EVENT_RETURN_USER_MARK
				//엘리오스 가이드북 이벤트 페이지 오픈

	#ifdef CHECK_INHERIT_STATE_MENU
				if( g_pMain->IsInheritStateMenu() )
	#endif //CHECK_INHERIT_STATE_MENU
				{
					CX2StateMenu* pStateMenu = (CX2StateMenu*) g_pMain->GetNowState();
					pStateMenu->OpenElliosGuide(110);
				}
#endif //SERV_EVENT_RETURN_USER_MARK
			}
		} break;
	}

	return false;
}
#pragma endregion UICustomEventProc

#pragma region UIServerEventProc
/** @function 	: UIServerEventProc
	@brief 		: 서버 이벤트
	@param		: procedure
	@return		: bool
*/
bool CX2PremiumBuffManager::UIServerEventProc( IN HWND hWnd, IN UINT uMsg, IN WPARAM wParam, IN LPARAM lParam )
{
	KTDXPROFILE();
	switch( wParam )
	{
	case EGS_COME_BACK_USER_NOT:
		{
			return Handler_EGS_COME_BACK_USER_NOT( hWnd, uMsg, wParam, lParam );
		} break;

	case EGS_COME_BACK_BUFF_NOT:
		{
			return Handler_EGS_COME_BACK_BUFF_NOT( hWnd, uMsg, wParam, lParam );
		} break;

#ifdef BUFF_TEMPLET_SYSTEM	
	case EGS_UPDATE_BUFF_INFO_NOT:
		{
			return Handler_EGS_UPDATE_BUFF_INFO_NOT( hWnd, uMsg, wParam, lParam );
		} break;

	case EGS_UPDATE_BUFF_INFO_IN_ROOM_NOT:
		{
			return Handler_EGS_UPDATE_BUFF_INFO_IN_ROOM_NOT( hWnd, uMsg, wParam, lParam );
		} break;
#endif BUFF_TEMPLET_SYSTEM

	default:
		break;
	}
	return false;
}
#pragma endregion UIServerEventProc

#pragma region Handler_EGS_COME_BACK_USER_NOT
/** @function 	: Handler_EGS_COME_BACK_USER_NOT
	@brief 		: 휴면 복귀 유저가 처음 접속했을 때 Dialog 표시를 위해서 사용
	@param		: HWND, UINT, WPARAM, LPARAM
	@return		: bool
*/
bool CX2PremiumBuffManager::Handler_EGS_COME_BACK_USER_NOT( IN HWND hWnd, IN UINT uMsg, IN WPARAM wParam, IN LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_COME_BACK_USER_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if ( true == kEvent )
	{
		if ( g_pMain->GetNowStateID() == CX2Main::XS_SERVER_SELECT )
		{
			SetFirstComeBack( true );

			m_pDLGWelcomeComeBackUser = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Return_User_Popup_Small.lua" );
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGWelcomeComeBackUser );
			m_pDLGWelcomeComeBackUser->SetShowEnable( false, false );

			m_pDLGComeBackUserBuffDesc = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Return_User_Popup_Big.lua" );
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGComeBackUserBuffDesc );
			m_pDLGComeBackUserBuffDesc->SetShowEnable( false, false );
		}
	}
	else
	{
		SetFirstComeBack( false );
	}
	
	return true;
}
#pragma endregion Handler_EGS_COME_BACK_USER_NOT

#ifdef BUFF_TEMPLET_SYSTEM


/** @function : Handler_EGS_UPDATE_BUFF_INFO_IN_ROOM_NOT
	@brief : 룸 안에 있는 유저들의 케릭터에 대한 서버 버프 처리 ( 대전, 던전, 필드 )
	@param : 서버에서 보내준 버프 정보들( 적용시켜야 할 버프 목록 ) ( kBuffInfo )
	@return : true
*/
bool CX2PremiumBuffManager::Handler_EGS_UPDATE_BUFF_INFO_IN_ROOM_NOT( IN HWND hWnd, IN UINT uMsg, IN WPARAM wParam, IN LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_UPDATE_BUFF_INFO_IN_ROOM_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if ( NULL != g_pX2Game )
	{
		CX2GUUser* pCX2GUUser = g_pX2Game->GetUserUnitByUID( kEvent.m_iUnitUID );

		if ( NULL != pCX2GUUser )
		{
			CX2Unit* pCX2Unit = pCX2GUUser->GetUnit();

			if( NULL != pCX2Unit )
			{
	#ifdef SET_WORLD_BUFF_AT_RESURRECTION // 김태환			버프 펙터 저장에서 버프 펙터 아이디 저장으로 변경
				vector<BUFF_FACTOR_ID>& vecWorldBuffFactorID = pCX2Unit->GetWorldBuffFactorID();

				BOOST_FOREACH( BUFF_FACTOR_ID eWorldBuffFactorID, vecWorldBuffFactorID )			/// 모든 서버 버프 해제
				{
					CX2BuffFactorPtr ptrBuffFactor = CX2BuffTempletManager::GetInstance()->GetBuffFactorPtr( eWorldBuffFactorID );

					if ( NULL != ptrBuffFactor )
						pCX2GUUser->EraseBuffTempletFromGameUnit( ptrBuffFactor->GetBuffTempletID() );
				}

				vecWorldBuffFactorID.clear();	/// 서버 버프 저장 컨테이너 초기화
	#else // SET_WORLD_BUFF_AT_RESURRECTION 
				vector<CX2BuffFactorPtr>& vecWorldBuffFactorPtr = pCX2Unit->GetWorldBuffFactorPtr();

				BOOST_FOREACH( CX2BuffFactorPtr WorldBuffFactorPtr, vecWorldBuffFactorPtr )			/// 모든 서버 버프 해제
				{
					pCX2GUUser->EraseBuffTempletFromGameUnit( WorldBuffFactorPtr->GetBuffTempletID() );
				}

				vecWorldBuffFactorPtr.clear();	/// 서버 버프 저장 컨테이너 초기화
	#endif // SET_WORLD_BUFF_AT_RESURRECTION

				BOOST_FOREACH( KBuffInfo& kBuffInfo, kEvent.m_vecActivateBuffList )			/// 서버에서 준 버프 적용
				{

#ifdef SERV_PLAY_WITH_CHAR_PARTY_BONUS_EXP
#pragma NOTE("빠른 수정을 위해 임시 예외처리 함. 관련 이벤트 계속 사용하려면 변경해야 함")
					switch( kBuffInfo.m_kFactorInfo.m_iFactorID )
					{
					case BFI_EMPTY_EXP_BUFF:
						if( CX2Game::GT_BATTLE_FIELD == g_pX2Game->GetGameType() )
							continue;
						break;
					default:
						break;
					}
#endif // SERV_PLAY_WITH_CHAR_PARTY_BONUS_EXP

					UINT			 iFactorID	   = kBuffInfo.m_kFactorInfo.m_iFactorID;	/// Factor ID
					CX2BuffFactorPtr ptrBuffFactor = CX2BuffTempletManager::GetInstance()->GetBuffFactorPtr( kBuffInfo.m_kFactorInfo.m_iFactorID );

					if ( NULL != ptrBuffFactor )
					{
						CX2BuffFactorPtr ptrBuffFactorClone = ptrBuffFactor->GetClonePtr();	/// 클론 버프 팩터로 대입

						if ( NULL != ptrBuffFactorClone )
						{
	#ifdef SET_WORLD_BUFF_AT_RESURRECTION // 김태환
							/// 버프 적용을 성공 하였거나, 죽은 상태라면 서버 버프 정보 저장
							if ( true == pCX2GUUser->SetBuffFactorToGameUnitByBuffFactorID( static_cast<UINT>( kBuffInfo.m_kFactorInfo.m_iFactorID ) ) ||
								( pCX2GUUser->GetNowHp() <= 0.0f || pCX2GUUser->GetNowStateID() == CX2GUUser::GUSI_DIE ) )
								vecWorldBuffFactorID.push_back( static_cast<BUFF_FACTOR_ID>( kBuffInfo.m_kFactorInfo.m_iFactorID ) );	/// 서버 버프 저장
	#else // SET_WORLD_BUFF_AT_RESURRECTION 
							if ( true == pCX2GUUser->SetBuffFactorToGameUnitByBuffFactorID( static_cast<UINT>( kBuffInfo.m_kFactorInfo.m_iFactorID ) ) )	/// 버프 적용
								vecWorldBuffFactorPtr.push_back( ptrBuffFactorClone );	/// 서버 버프 저장
	#endif // SET_WORLD_BUFF_AT_RESURRECTION
						}
					}
				}
			}
		}
	}

	return true;
}

/** @function : Handler_EGS_UPDATE_BUFF_INFO_NOT
	@brief : 룸이 아닌 곳에 있는 자신의 케릭터에 대한 서버 버프 아이콘 처리 ( 마을 )
	@param : 서버에서 보내준 버프 정보들( 적용시켜야 할 버프 목록 ) ( kBuffInfo )
	@return : true
*/
bool CX2PremiumBuffManager::Handler_EGS_UPDATE_BUFF_INFO_NOT( IN HWND hWnd, IN UINT uMsg, IN WPARAM wParam, IN LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_UPDATE_BUFF_INFO_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if( NULL != g_pX2Game )		/// 룸 안에 있는데 해당 패킷이 날아오면, 동작 않시킴
		return true;

	if(  NULL != g_pData && NULL != g_pData->GetMyUser() )
	{
		CX2Unit* pCX2Unit = g_pData->GetMyUser()->GetSelectUnit();

		if( NULL != pCX2Unit )
		{
#ifdef SET_WORLD_BUFF_AT_RESURRECTION // 김태환
			vector<BUFF_FACTOR_ID>& vecWorldBuffFactorID = pCX2Unit->GetWorldBuffFactorID();

			BOOST_FOREACH( BUFF_FACTOR_ID eWorldBuffFactorID, vecWorldBuffFactorID )				/// 모든 서버 버프 아이콘 해제
			{
				CX2BuffFactorPtr ptrBuffFactor = CX2BuffTempletManager::GetInstance()->GetBuffFactorPtr( eWorldBuffFactorID );

				if ( NULL != ptrBuffFactor )
				{
					CX2BuffTempletPtr ptrBuffTemplet = CX2BuffTempletManager::GetInstance()->GetBuffTempletPtr( ptrBuffFactor->GetBuffTempletID() );

#ifdef BUFF_ICON_UI
					if( NULL != ptrBuffTemplet && true == ptrBuffTemplet->GetUseBuffIcon() )
					{
						CX2GageManager::GetInstance()->EraseBuff( pCX2Unit->GetUID(), ptrBuffTemplet->GetBuffTempletID(), ptrBuffTemplet->IsDeBuff() );
					}
#endif // BUFF_ICON_UI
				}
			}

			vecWorldBuffFactorID.clear();	/// 서버 버프 저장 컨테이너 초기화
#else // SET_WORLD_BUFF_AT_RESURRECTION
			vector<CX2BuffFactorPtr>& vecWorldBuffFactorPtr = pCX2Unit->GetWorldBuffFactorPtr();

			BOOST_FOREACH( CX2BuffFactorPtr WorldBuffFactorPtr, vecWorldBuffFactorPtr )				/// 모든 서버 버프 아이콘 해제
			{
				CX2BuffTempletPtr ptrBuffTemplet = CX2BuffTempletManager::GetInstance()->GetBuffTempletPtr( WorldBuffFactorPtr->GetBuffTempletID() );

	#ifdef BUFF_ICON_UI
				if( NULL != ptrBuffTemplet && true == ptrBuffTemplet->GetUseBuffIcon() )
				{
					CX2GageManager::GetInstance()->EraseBuff( pCX2Unit->GetUID(), ptrBuffTemplet->GetBuffTempletID(), ptrBuffTemplet->IsDeBuff() );
				}
	#endif // BUFF_ICON_UI
			}

			vecWorldBuffFactorPtr.clear();	/// 서버 버프 저장 컨테이너 초기화
#endif // SET_WORLD_BUFF_AT_RESURRECTION

			BOOST_FOREACH( KBuffInfo& kBuffInfo, kEvent.m_vecActivateBuffList )				/// 서버에서 준 버프 아이콘 적용
			{
#ifdef SERV_PLAY_WITH_CHAR_PARTY_BONUS_EXP
#pragma NOTE("빠른 수정을 위해 임시 예외처리 함. 관련 이벤트 계속 사용하려면 변경해야 함")
				switch( kBuffInfo.m_kFactorInfo.m_iFactorID )
				{
				case BFI_EMPTY_EXP_BUFF:
					continue;
					break;
				default:
					break;
				}
#endif // SERV_PLAY_WITH_CHAR_PARTY_BONUS_EXP

#ifdef FIX_REFORM_ENTRY_POINT_7TH					// 김종훈, 귀환자 엠블렘 출력 부분 Emblem Manager 로 변경
				switch( kBuffInfo.m_kFactorInfo.m_iFactorID )
				{
					// 서버로 부터 받은 버프의 Factor ID 가 귀환자 버프 일 때 엠블렘을 출력한다.
					case BFI_BUFF_RETURN_OF_HERO:
						CX2EmblemManager::GetInstance()->PlayEmblem(CX2EmblemManager::EI_RETURN_OF_HERO, true);					
						break;
					default:
						break;
				}
#endif // FIX_REFORM_ENTRY_POINT_7TH				// 김종훈, 귀환자 엠블렘 출력 부분 Emblem Manager 로 변경
				UINT			 iFactorID	   = kBuffInfo.m_kFactorInfo.m_iFactorID;		/// Factor ID
				CX2BuffFactorPtr ptrBuffFactor = CX2BuffTempletManager::GetInstance()->GetBuffFactorPtr( kBuffInfo.m_kFactorInfo.m_iFactorID );

				if ( NULL != ptrBuffFactor )
				{
					CX2BuffFactorPtr ptrBuffFactorClone = ptrBuffFactor->GetClonePtr();		/// 클론 버프 팩터로 대입

					if ( NULL != ptrBuffFactorClone )
					{
						CX2BuffTempletPtr ptrBuffTemplet	= CX2BuffTempletManager::GetInstance()->GetBuffTempletPtr( ptrBuffFactor->GetBuffTempletID() );

#ifdef BUFF_ICON_UI
						if ( NULL != ptrBuffTemplet )
						{
							if( true == ptrBuffTemplet->GetUseBuffIcon() )
							{
								CX2GageManager::GetInstance()->PushBuff( pCX2Unit->GetUID(), CX2GageUI::BuffIcon( ptrBuffTemplet->GetBuffTempletID(),ptrBuffTemplet->GetIconFileName(), 
									ptrBuffTemplet->GetIconKeyName() , ptrBuffTemplet->GetBuffName(), ptrBuffTemplet->GetBuffDesc() ), ptrBuffTemplet->IsDeBuff() );	/// 버프 아이콘 적용
							}
						}
#endif // BUFF_ICON_UI

#ifdef SET_WORLD_BUFF_AT_RESURRECTION // 김태환
						vecWorldBuffFactorID.push_back( static_cast<BUFF_FACTOR_ID>( kBuffInfo.m_kFactorInfo.m_iFactorID ) );	/// 서버 버프 저장
#else // SET_WORLD_BUFF_AT_RESURRECTION
						vecWorldBuffFactorPtr.push_back( ptrBuffFactorClone );	/// 서버 버프 저장
#endif // SET_WORLD_BUFF_AT_RESURRECTION

					}
				}
			}
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
			/// 버프로 인하여 추가된 스킬 레벨 재설정
				pCX2Unit->ResetIncreaseSkillLevelByBuff();
	#endif // UPGRADE_SKILL_SYSTEM_2013

		}
	}

	return true;
}
#endif BUFF_TEMPLET_SYSTEM

#pragma region Handler_EGS_COME_BACK_BUFF_NOT
/** @function 	: Handler_EGS_COME_BACK_BUFF_NOT
	@brief 		: 휴먼 복귀 유저일 경우
	@param		: HWND, UINT, WPARAM, LPARAM
	@return		: bool
*/
bool CX2PremiumBuffManager::Handler_EGS_COME_BACK_BUFF_NOT( IN HWND hWnd, IN UINT uMsg, IN WPARAM wParam, IN LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_COME_BACK_BUFF_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if ( false == kEvent.m_bIsFinish )
	{
		if ( g_pMain->GetNowStateID() == CX2Main::XS_SERVER_SELECT )
		{
			SetComeBackUser( true );
			SetShowPremiumBuffUI( true );
			SetViewBuffChat( true );

			m_iComeBackBuffEnd = kEvent.m_iComeBackBuffEnd;
		}
	}
	else
	{
		SetComeBackUser( false );
	} 

	return true;
}
#pragma endregion Handler_EGS_COME_BACK_BUFF_NOT

#pragma region OnFrameMove
HRESULT CX2PremiumBuffManager::OnFrameMove( IN double fTime, IN float fElapsedTime )
{
	//if ( true == IsComeBackUser() || IsComeBackUserInParty() )
	{
		{
			if ( false == GetShowPremiumBuffUI() )
			{
				// NPC와 대화 중도 아니고, 거래 광장도 아니면
				if ( ( NULL != g_pTFieldGame && false == g_pTFieldGame->GetJoinNpc() ) &&
					( NULL == g_pSquareGame ) )
					SetShowPremiumBuffUI( true );
			}
			else
			{
				// NPC와 대화중이거나, 거래 광장이면
				if ( ( NULL != g_pTFieldGame && true == g_pTFieldGame->GetJoinNpc() ) ||
					( NULL != g_pSquareGame ) )
					SetShowPremiumBuffUI( false );
				else
					SetShowPremiumBuffUI( true );

				UpdateGuideDescPremiumBuffUI();		
			}
		}		
	}
	//else
	//{
	//	SetShowPremiumBuffUI( false );
	//}

#ifdef BUFF_TEMPLET_SYSTEM

#ifdef CHECK_INHERIT_STATE_MENU
	if( g_pMain->IsInheritStateMenu() )
#endif //CHECK_INHERIT_STATE_MENU
	{
		CX2StateMenu* pStateMenu = static_cast<CX2StateMenu*>( g_pMain->GetNowState() );

		switch( g_pMain->GetNowStateID() )
		{
		case CX2Main::XS_VILLAGE_MAP:
		case CX2Main::XS_DUNGEON_GAME:
		case CX2Main::XS_SQUARE_GAME:
			{
				pStateMenu->SetShowBuffTempletIcon( true );

				if( NULL != g_pData && NULL != g_pData->GetCashShop() )		//캐시샵이 열려 있을 때, 아이콘 표시를 막는다.
				{
					if( true == g_pData->GetCashShop()->GetOpen() )
						pStateMenu->SetShowBuffTempletIcon( false );
				}

				// NPC와 대화중일 때,
				if ( NULL != g_pTFieldGame && true == g_pTFieldGame->GetJoinNpc() )
				{
					pStateMenu->SetShowBuffTempletIcon( false );
				}
			}break;
		default:
			{
				if( GetShowBuffTempletUI() != false )
					pStateMenu->SetShowBuffTempletIcon( false );
			}break;
		}
	}
#endif BUFF_TEMPLET_SYSTEM

	return S_OK;
}
#pragma endregion OnFrameMove


#endif

#ifdef BUFF_TEMPLET_SYSTEM	
void CX2PremiumBuffManager::InsertPremiumBuff( const CX2PremiumBuffTemplet::BUFF_ID ePremiumBuffID_, wstring wstrEndTime )
{
	const WCHAR* pFileName	= NULL;
	const WCHAR* pKeyName	= NULL;
	bool		 bUsingIcon = NULL; //버프 아이콘 사용 유무

	//if ( false == GetFileAndKeyName( ePremiumBuffID_, &pFileName, &pKeyName ) )
	if ( false == GetFileAndKeyName( ePremiumBuffID_, &pFileName, &pKeyName, &bUsingIcon ) )
		return;

	UINT uiIndex = m_vecPremiumBuffList.size();

	CX2PremiumBuffIconPtr ptrPremiumBuff = CX2PremiumBuffIcon::CreatePremiumBuff( ePremiumBuffID_, uiIndex, pFileName, pKeyName, wstrEndTime, bUsingIcon );

	m_vecPremiumBuffList.push_back( ptrPremiumBuff );
}


void CX2PremiumBuffManager::ErasePremiumBuff( const CX2PremiumBuffTemplet::BUFF_ID ePremiumBuffID_ )
{
	if ( false == m_vecPremiumBuffList.empty() )
	{
		auto funcFindBuff = [ePremiumBuffID_]( CX2PremiumBuffIconPtr val_ ) {
			return ( val_->GetPremiumBuffID() == ePremiumBuffID_ );
		};

		vector<CX2PremiumBuffIconPtr>::iterator vItr = find_if( m_vecPremiumBuffList.begin(), m_vecPremiumBuffList.end(), funcFindBuff );

		if ( m_vecPremiumBuffList.end() != vItr )
		{
			m_vecPremiumBuffList.erase( vItr );
			UpdatePremiumBuffIconPosition();
		}
	}
}


bool CX2PremiumBuffManager::GetFileAndKeyName( IN const CX2PremiumBuffTemplet::BUFF_ID ePremiumBuffID_, OUT const WCHAR** ppFileName_, OUT const WCHAR** ppKeyName_, OUT bool* bUsingIcon_ )
{
	switch ( ePremiumBuffID_ )
	{
	case CX2PremiumBuffTemplet::BI_GATHERING_OF_HERO:
		{
			*ppFileName_	= L"DLG_ui_button18.tga";
			*ppKeyName_	= L"PC_BANG";
		} break;

	case CX2PremiumBuffTemplet::BI_REST_FOR_REGROUP:
		{
			*ppFileName_	= L"DLG_ui_button18.tga";
			*ppKeyName_	= L"blood";
		} break;

	case CX2PremiumBuffTemplet::BI_WARM_EL_AURA:
		{
			*ppFileName_	= L"DLG_ui_button18.tga";
			*ppKeyName_	= L"BUBBLE_EVENT";
		} break;

	case CX2PremiumBuffTemplet::BI_PROTECTION_OF_NUT:			// 누트의 가호   ( 비약 )
		{
			*ppFileName_	= L"DLG_ui_button20.tga";
			*ppKeyName_	= L"NUTE";
		} break;

	case CX2PremiumBuffTemplet::BI_PROTECTION_OF_PUTA:			// 프타의 가호   ( 비약 )
		{
			*ppFileName_	= L"DLG_ui_button20.tga";
			*ppKeyName_	= L"PTA";
		} break;

	case CX2PremiumBuffTemplet::BI_PROTECTION_OF_LAHELL:		// 라헬의 가호   ( 비약 )
		{
			*ppFileName_	= L"DLG_ui_button20.tga";
			*ppKeyName_	= L"RAHEL";
		} break;

	case CX2PremiumBuffTemplet::BI_PROTECTION_OF_JAKIELL:		// 자키엘의 가호 ( 비약 )
		{
			*ppFileName_	= L"DLG_ui_button20.tga";
			*ppKeyName_	= L"JAKIEL";
		} break;

	case CX2PremiumBuffTemplet::BI_BELSSING_OF_SERAPHIM:		// 세라핌의 축복	 ( 영약 )
		{
			*ppFileName_	= L"DLG_ui_button20.tga";
			*ppKeyName_	= L"SERAPHIM";
		} break;

	case CX2PremiumBuffTemplet::BI_BELSSING_OF_GEV:				// 게브의 축복	 ( 영약 )
		{
			*ppFileName_	= L"DLG_ui_button20.tga";
			*ppKeyName_	= L"GEV";
		} break;

	case CX2PremiumBuffTemplet::BI_BELSSING_OF_AMON:			// 아몬의 축복	 ( 영약 )
		{
			*ppFileName_	= L"DLG_ui_button20.tga";
			*ppKeyName_	= L"AMON";
		} break;
		
	case CX2PremiumBuffTemplet::BI_BELSSING_OF_CRONOS:			// 크로노스의 축복 ( 영약 )
		{
			*ppFileName_	= L"DLG_ui_button20.tga";
			*ppKeyName_	= L"CRONOS";
		} break;

	case CX2PremiumBuffTemplet::BI_SAVE_CRYSTAL_BUFF_LEVEL_1:	// 어둠의 문 개인 보상 버프 Level 1
		{
			*ppFileName_	= L"DLG_ui_button20.tga";
			*ppKeyName_	= L"HERO";
		} break;

	case CX2PremiumBuffTemplet::BI_SAVE_CRYSTAL_BUFF_LEVEL_2:	// 어둠의 문 개인 보상 버프 Level 2
		{
			*ppFileName_	= L"DLG_ui_button20.tga";
			*ppKeyName_	= L"HERO";
		} break;

	case CX2PremiumBuffTemplet::BI_SAVE_CRYSTAL_BUFF_LEVEL_3:	// 어둠의 문 개인 보상 버프 Level 3
		{
			*ppFileName_	= L"DLG_ui_button20.tga";
			*ppKeyName_	= L"HERO";
		} break;

	case CX2PremiumBuffTemplet::BI_SAVE_CRYSTAL_BUFF_LEVEL_4:	// 어둠의 문 개인 보상 버프 Level 4
		{
			*ppFileName_	= L"DLG_ui_button20.tga";
			*ppKeyName_	= L"HERO";
		} break;
	case CX2PremiumBuffTemplet::BI_STRENGTHENING_BODY:			// 강화된 신체 ( 비약 사용 제한 디버프 )
		{
			*ppFileName_	= L"DLG_ui_button20.tga";
			*ppKeyName_	= L"DEBUFF";
		} break;

	default:
		{
			//버프 아이콘이 없는 버프인 경우, 아이콘을 표시하지 않는다. ( 예 : 엘의 수정 버프 )
			*ppFileName_	= NULL;
			*ppKeyName_		= NULL;
			*bUsingIcon_	= false;	//버프 아이콘 미사용

			return false;
		} break;
	}
	*bUsingIcon_ = true;				//버프 아이콘 사용중
	return true;
}

void CX2PremiumBuffManager::UpdatePremiumBuffIconPosition()
{
	if ( false == m_vecPremiumBuffList.empty() )
	{
		UINT uiNewIndex = 0;
		vector<CX2PremiumBuffIconPtr>::iterator vItr = m_vecPremiumBuffList.begin();
		while ( m_vecPremiumBuffList.end() != vItr )
		{
			if ( (*vItr)->GetIndex() != uiNewIndex )
				(*vItr)->ChangeBuffIconPosition( uiNewIndex );
				

			++uiNewIndex;
			++vItr;
		}
	}
}

void CX2PremiumBuffManager::AddBuffInfo_LUA()
{
	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
	//TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );

	CX2PremiumBuffTempletPtr ptrPremiumBuffTemplet = CX2PremiumBuffTemplet::CreatePremiumBuffTemplet();

	if ( ptrPremiumBuffTemplet->ParsingScript( luaManager ) )
		m_mapPremiumBuffTempletMap.insert( make_pair( ptrPremiumBuffTemplet->GetPremiumBuffID(), ptrPremiumBuffTemplet ) );
	else
	{
		ASSERT( !L"BuffTemplet Parsing Error" );
		StateLog( L"BuffTemplet Parsing Error" );
	}
}

void CX2PremiumBuffManager::OpenScriptFile( const wchar_t* wszScriptFileName_ )
{
	lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(),  "BuffManager", this );

    if ( g_pKTDXApp->LoadLuaTinker( wszScriptFileName_ ) == false )
    {
		ASSERT( !"DoMemory doesn't work!" );
		ErrorLogMsg( XEM_ERROR2, wszScriptFileName_ );
		//return false;
    }
}

CX2PremiumBuffTempletPtr CX2PremiumBuffManager::GetPremiumBuffTempletPtr( const CX2PremiumBuffTemplet::BUFF_ID ePremiumBuffID_ )
{
	PremiumBuffTempletMap::iterator mItr = m_mapPremiumBuffTempletMap.find( ePremiumBuffID_ );

	if ( m_mapPremiumBuffTempletMap.end() != mItr)
		return mItr->second;
	else
		return CX2PremiumBuffTempletPtr();
}

void CX2PremiumBuffManager::GetSocketOptions( OUT vector<int>& vecBuffSocetOptions_ )
{
	BOOST_FOREACH( CX2PremiumBuffIconPtr ptrValue, m_vecPremiumBuffList )
	{
		CX2PremiumBuffTempletPtr ptrTemplet = GetPremiumBuffTempletPtr( ptrValue->GetPremiumBuffID() );

		const vector<int>& vecSocketOptionsPerBuff = ptrTemplet->GetSocketOptionsList();
		vecBuffSocetOptions_.insert( vecBuffSocetOptions_.end(), vecSocketOptionsPerBuff.begin(), vecSocketOptionsPerBuff.end() );
	}
}

void CX2PremiumBuffManager::GetSocketOptions( IN const vector<int>& vecBuffIdList_, OUT vector<int>& vecBuffSocetOptions_ )
{
	BOOST_FOREACH( const int& iBuffId, vecBuffIdList_ )
	{
		CX2PremiumBuffTempletPtr ptrTemplet = GetPremiumBuffTempletPtr( static_cast<CX2PremiumBuffTemplet::BUFF_ID>( iBuffId ) );

		const vector<int>& vecSocketOptionsPerBuff = ptrTemplet->GetSocketOptionsList();
		vecBuffSocetOptions_.insert( vecBuffSocetOptions_.end(), vecSocketOptionsPerBuff.begin(), vecSocketOptionsPerBuff.end() );
	}
}
/* 필드에서 사용중인 버프 아이콘 좌표
const float				CX2PremiumBuffIcon::POSITION_X		= 264.0f;
const float				CX2PremiumBuffIcon::POSITION_Y		= 686.0f;
*/
const float				CX2PremiumBuffIcon::POSITION_X		= 254.0f;
const float				CX2PremiumBuffIcon::POSITION_Y		= 686.0f;

CX2PremiumBuffIcon::~CX2PremiumBuffIcon()
{
	SAFE_DELETE_DIALOG( m_pDlgBuffIcon );
}

/*static*/ CX2PremiumBuffIconPtr CX2PremiumBuffIcon::CreatePremiumBuff( const CX2PremiumBuffTemplet::BUFF_ID ePremiumBuffID_, const UINT uiIndex_, 
	const WCHAR* pFileName_, const WCHAR* pKeyName_, wstring wstrEndTime, bool m_bUsingIcon )
{
	CX2PremiumBuffIconPtr ptrPremiumBuff( new CX2PremiumBuffIcon( ePremiumBuffID_, uiIndex_, wstrEndTime, m_bUsingIcon ) );
	ptrPremiumBuff->CreateBuffIcon( pFileName_, pKeyName_ );

	return ptrPremiumBuff;
}

void CX2PremiumBuffIcon::CreateBuffIcon( const WCHAR* pFileName_, const WCHAR* pKeyName_ )
{
/*
	m_pDlgBuffIcon = new CKTDGUIDialog( NULL, L"DLG_UI_Buff_Icon.lua" );	//임시 설정

	CKTDGUIButton* pButton = static_cast<CKTDGUIButton*>( m_pDlgBuffIcon->GetControl( L"Button_Buff_Icon" ) );	//임시 설정
	if ( NULL != pButton )
	{
		pButton->SetNormalTex( pFileName_, pKeyName_ );  
		pButton->SetOverTex( pFileName_, pKeyName_ );
		pButton->SetDownTex( pFileName_, pKeyName_ );
		pButton->SetEnable( true );

		if( g_pData->GetPremiumBuffManager()->GetShowBuffTempletUI() == true )
			pButton->SetShow( true );
		else
			pButton->SetShow( false );

		UpdateBuffIconPosition( GetIndex() );

		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDlgBuffIcon );
	}*/

}

void CX2PremiumBuffIcon::UpdateBuffIconPosition( const UINT uiIndex_ )
{
	UINT uiColumnIndex	= uiIndex_ / NUMBER_OF_ROW;
	UINT uiRowIndex		= uiIndex_ % NUMBER_OF_ROW;

	float fx = static_cast<float>( uiRowIndex * SIZE_OF_ICON );
	float fy = static_cast<float>( uiColumnIndex * SIZE_OF_ICON );

	m_pDlgBuffIcon->SetPos( D3DXVECTOR2( POSITION_X + fx, POSITION_Y - fy ) );
	//pButton_->MoveControl( fx, fy );
}

void CX2PremiumBuffIcon::ChangeBuffIconPosition( const UINT uiNewIndex_ )
{
	if ( NULL == m_pDlgBuffIcon )
		return;

	if ( uiNewIndex_ != GetIndex() )
	{
		SetIndex( uiNewIndex_ );
		UpdateBuffIconPosition( GetIndex() );
	}
}

void CX2PremiumBuffIcon::SetBuffDesc()
{
	CKTDGUIButton* pButton = static_cast<CKTDGUIButton*>( m_pDlgBuffIcon->GetControl( L"Button_Buff_Icon" ) );
	if ( NULL != pButton )
	{
// 		wstring wstrGuideDesc;
// 
// 		g_pData->GetPremiumBuffManager()->
// 		pButton->SetGuideDesc( )
	}
}

bool CX2PremiumBuffTemplet::ParsingScript( IN KLuaManager& luaManager_ )
{
	LUA_GET_VALUE_ENUM( luaManager_,	 "m_eBuffID",		    m_ePremiumBuffID,		     BUFF_ID,		 BI_NONE );

	LUA_GET_VALUE(		luaManager_,	 "m_strBuffName",	    m_uiStringID,			     STR_ID_EMPTY );

	LUA_GET_VALUE(		luaManager_,	 "m_fBuffPeriodSecond", m_fPremiumBuffPeriodSecond,  0			  );

	if ( luaManager_.BeginTable( "USING_BUFF_ROOM_TYPE" ) )
	{
		int iIndex = 1;
		CX2PremiumBuffTemplet::USING_ROOM_TYPE eUsingRoomType = CX2PremiumBuffTemplet::URT_NONE;

		int iTemp = -1;
		while ( luaManager_.GetValue( iIndex, iTemp ) ) { 
			eUsingRoomType = static_cast<CX2PremiumBuffTemplet::USING_ROOM_TYPE>(iTemp);
			m_vUsingRoomType.push_back( eUsingRoomType );
			++iIndex;
		}
		luaManager_.EndTable();
	}

	if ( luaManager_.BeginTable( "SOCKET_OPTION" ) )
	{
		int iIndex = 1;
		int iSocketOption = 0;

		while ( luaManager_.GetValue( iIndex, iSocketOption ) ) {
			m_vecSocketOptions.push_back( iSocketOption );
			++iIndex;
			iSocketOption = 0; // 초기화
		}
		luaManager_.EndTable();
	}

	return true;
}
#endif BUFF_TEMPLET_SYSTEM

#ifdef SERV_PC_BANG_TYPE
bool CX2PremiumBuffManager::IsPcBangAvatar()
{
	return m_kPcBangReward.m_bAvatar;
}

void CX2PremiumBuffManager::SetPcBangReward( KPcBangReward kPcBangReward )
{
	m_kPcBangReward = kPcBangReward;
}

wstring CX2PremiumBuffManager::GetPcBangBuffString()
{
	wstring wstrPcBangBuff;

	switch( m_kPcBangReward.m_iPcBangType )
	{
	case 0:
		// 프리미엄 PC방 서비스
		wstrPcBangBuff = GET_STRING( STR_ID_20839 );
		break;
	case 1:
		// 프리미엄 PC방 서비스
		wstrPcBangBuff = GET_STRING( STR_ID_23944 );
		break;
	case 2:
		// 프리미엄 PC방 서비스++
		wstrPcBangBuff = GET_STRING( STR_ID_23945 );
		break;
	case 3:
		// 프리미엄 PC방 서비스++
		wstrPcBangBuff = GET_STRING( STR_ID_23946 );
		break;
	default:
		return L"";
	}
	wstrPcBangBuff += L"\n";

	// STR_ID_20840
	// 근성도 무제한, 경험치 +20%\nPC방 인벤토리(전용 아바타) 사용 가능\nPC방 전용 칭호 사용 가능\n비밀던전 일반 무제한 입장\n헤니르의 시공 보상 횟수 +2회 추가

	// 근성도 무제한 
	if( m_kPcBangReward.m_bSpiritFree == true )
	{
		wstrPcBangBuff += GET_STRING( STR_ID_23935 );
		wstrPcBangBuff += _GetLastString( wstrPcBangBuff );
	}

	// 경험치 +@1%
	if( m_kPcBangReward.m_fEXPRate > 0.0f )
	{
		wstrPcBangBuff += GET_REPLACED_STRING( ( STR_ID_23936, "f", m_kPcBangReward.m_fEXPRate * 100.0f ) );
		wstrPcBangBuff += _GetLastString( wstrPcBangBuff );
	}

	// ED획득량 +@1%
	if( m_kPcBangReward.m_fEDRate > 0.0f )
	{
		wstrPcBangBuff += GET_REPLACED_STRING( ( STR_ID_23942, "f", m_kPcBangReward.m_fEDRate * 100.0f ) );
		wstrPcBangBuff += _GetLastString( wstrPcBangBuff );
	}

	// AP획득량 +@1%
	if( m_kPcBangReward.m_fVSPointRate > 0.0f )
	{
		wstrPcBangBuff += GET_REPLACED_STRING( ( STR_ID_23943, "f", m_kPcBangReward.m_fVSPointRate * 100.0f ) );
		wstrPcBangBuff += _GetLastString( wstrPcBangBuff );
	}

	// 펫 경험치 +@1%
	if( m_kPcBangReward.m_fPetEXPRate > 0.0f )
	{
		wstrPcBangBuff += GET_REPLACED_STRING( ( STR_ID_23941, "f", m_kPcBangReward.m_fPetEXPRate * 100.0f ) );
		wstrPcBangBuff += _GetLastString( wstrPcBangBuff );
	}

	// PC방 인벤토리(전용 아바타) 사용 가능
	if( m_kPcBangReward.m_bAvatar == true )
	{
		wstrPcBangBuff += GET_STRING( STR_ID_23937 );
		wstrPcBangBuff += _GetLastString( wstrPcBangBuff );
	}

	// PC방 전용 칭호 사용 가능
	if( m_kPcBangReward.m_bTitle == true )
	{
		wstrPcBangBuff += GET_STRING( STR_ID_23938 );
		wstrPcBangBuff += _GetLastString( wstrPcBangBuff );
	}

	// 헤니르의 시공 보상 횟수 +@1회 추가
	if( m_kPcBangReward.m_iAdditionalHenirReward > 0 )
	{
		wstrPcBangBuff += GET_REPLACED_STRING( ( STR_ID_23940, "i", m_kPcBangReward.m_iAdditionalHenirReward ) );
		wstrPcBangBuff += _GetLastString( wstrPcBangBuff );
	}

	return wstrPcBangBuff;
}
#endif SERV_PC_BANG_TYPE
