#include "StdAfx.h"

#ifdef NEW_VILLAGE_UI

#include ".\x2uimanager.h"

//class CX2Slot;
//class CX2SlotManager::CX2Slot;
CX2UIManager::CX2UIManager(void) :
m_bFlag(true),
m_pCharInfo(NULL),
m_pInventory(NULL),
m_pWatch(NULL),
m_pPersonalShop(NULL),
m_pPersonalTrade(NULL),
m_pShop(NULL),
m_pManufacture(NULL),
m_pQuickSlot(NULL),

#ifdef SERV_EPIC_QUEST
m_pQuestNew(NULL),
#else
m_pQuestReceive(NULL),
m_pQuest(NULL),
#endif SERV_EPIC_QUEST
#ifdef SERV_PET_SYSTEM
m_pPetList(NULL),
#endif
#ifdef NEW_SKILL_TREE_UI
m_pSkillTree(NULL),
#endif
#ifdef COMBO_TREE_UI_TEST
m_pComboTreeUI( NULL ),
#endif COMBO_TREE_UI_TEST

//{{ kimhc // 실시간 엘소드 중 실시간 아이템 획득 관련 임시 인벤토리
#ifdef	REAL_TIME_ELSWORD
m_pTempInventory( NULL ),
#endif	REAL_TIME_ELSWORD
//}} kimhc // 실시간 엘소드 중 실시간 아이템 획득 관련 임시 인벤토리

#ifdef POSTBOX_NOTICE
m_pPostBoxWarning( NULL ),
#endif POSTBOX_NOTICE

#ifdef DEF_TRADE_BOARD // oasis907 : 김상윤 [2010.3.12] //  개인 상점 검색 시스템
m_pPersonalShopBoard( NULL ),
#endif DEF_TRADE_BOARD
#ifdef GUILD_BOARD // oasis907 : 김상윤 [2009.11.24] // 길드 게시판
m_pGuildBoard( NULL ),
#endif GUILD_BOARD

//{{ kimhc // 2009-07-09 // 헤니르의 시공에 추가되는 Ranking UI 작업
#ifdef	RANKING_INFO_UI
m_pRankingInfo( NULL ),
#endif	RANKING_INFO_UI
//}} kimhc // 2009-07-09 // 헤니르의 시공에 추가되는 Ranking UI 작업

//{{ kimhc // 2009-07-25 // 헤니르의 시공에 추가되는 ITEM_EXCHANGE_SHOP 작업
#ifdef	ITEM_EXCHANGE_SHOP
m_pItemExchangeShop( NULL ),
#endif	ITEM_EXCHANGE_SHOP
//}} kimhc // 2009-07-25 // 헤니르의 시공에 추가되는 ITEM_EXCHANGE_SHOP 작업

//{{ kimhc // 2009-08-04 // 캐릭터별 은행
#ifdef	PRIVATE_BANK
m_pPrivateBank( NULL ),
#endif	PRIVATE_BANK
//}} kimhc // 2009-08-04 // 캐릭터별 은행

#ifdef SERV_SKILL_NOTE
m_pSkillNote( NULL), 
#endif

//{{ 2011.03.16   임규수 아바타 분해 시스템
#ifdef SERV_MULTI_RESOLVE
m_pResolveItem( NULL ),
#endif SERV_MULTI_RESOLVE
//}}
//{{ 2011.05.04   임규수 아바타 합성 시스템
#ifdef SERV_SYNTHESIS_AVATAR
m_pSynthesisItem( NULL ),
#endif SERV_SYNTHESIS_AVATAR
//}}

#ifdef SERV_HERO_PVP_MANAGE_LIST
m_pHeroMatch( NULL ),
#endif //SERV_HERO_PVP_MANAGE_LIST

m_LUMenu1st(UI_MENU_END),
m_LUMenu2nd(UI_MENU_END)
//m_vLayer.clear();
#ifdef MODIFY_DUNGEON_STAGING
,m_bShowUI(true)
#endif //MODIFY_DUNGEON_STAGING
#ifdef REFORM_UI_SKILLSLOT
, m_bShowSkillSlot(true)
, m_bShowQuickSlot(true)
#endif //REFORM_UI_SKILLSLOT
#ifdef RIDING_SYSTEM
, m_bShowRidingPetSkillSlot( false )
#endif //RIDING_SYSTEM
#ifdef SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT
, m_fWeddingClose( -1.f )
, m_pDlgWeddingCaution( NULL )
, m_pDlgWeddingEventLetter( NULL )
, m_hMeshHandleWedding( INVALID_MESH_INSTANCE_HANDLE )
#endif //SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT
#ifdef ALWAYS_EVENT_ADAMS_UI_SHOP
,m_bShowAdamsShop(false)
#endif ALWAYS_EVENT_ADAMS_UI_SHOP
{
	// 변수 초기화
	m_vLayer.clear();
	
}

// ResetUIManager : 인벤토리 등의 각종 UI가 생성시에 UIManager의 일부 Method를 사용하기 때문에 만듬.
// UIManager의 생성자에서 하위 UI들을 생성할 경우 Seg Fault.
void CX2UIManager::ResetUIManager()
{
	// 인벤토리랑 캐릭터창, 스킬트리는 시작할 때 생성
	// 그 외의 UI는 생성 타이밍을 최초 열릴 때로 미룸
	// 서버 이벤트를 받아야 하는 UI는 생성해 둠
	SAFE_DELETE(m_pCharInfo);
	CreateUICharInfo();

	SAFE_DELETE(m_pInventory);
	m_pInventory = new CX2UIInventory(g_pMain->GetNowState(), NULL );

#ifdef SERV_PET_SYSTEM
	SAFE_DELETE( m_pPetList );
#endif

#ifdef NEW_SKILL_TREE_UI
	SAFE_DELETE(m_pSkillTree);
#endif



#ifdef COMBO_TREE_UI_TEST
	SAFE_DELETE( m_pComboTreeUI );
#endif COMBO_TREE_UI_TEST


	//{{ kimhc // 실시간 엘소드 중 실시간 아이템 획득 관련 임시 인벤토리
#ifdef	REAL_TIME_ELSWORD
	SAFE_DELETE( m_pTempInventory );
#endif	REAL_TIME_ELSWORD
	//}} kimhc // 실시간 엘소드 중 실시간 아이템 획득 관련 임시 인벤토리
#ifdef POSTBOX_NOTICE
	SAFE_DELETE_DIALOG( m_pPostBoxWarning );
#endif POSTBOX_NOTICE
#ifdef DEF_TRADE_BOARD
	SAFE_DELETE( m_pPersonalShopBoard );
#endif DEF_TRADE_BOARD
#ifdef GUILD_BOARD // oasis907 : 김상윤 [2009.11.24] // 길드 게시판
	SAFE_DELETE( m_pGuildBoard );
#endif GUILD_BOARD

	//{{ kimhc // 2009-07-09 // 헤니르의 시공에 추가되는 Ranking UI 작업
#ifdef	RANKING_INFO_UI
	SAFE_DELETE( m_pRankingInfo );
#endif	RANKING_INFO_UI
		//}} kimhc // 2009-07-09 // 헤니르의 시공에 추가되는 Ranking UI 작업

	//{{ kimhc // 2009-07-25 // 헤니르의 시공에 추가되는 ITEM_EXCHANGE_SHOP 작업
#ifdef	ITEM_EXCHANGE_SHOP
	SAFE_DELETE( m_pItemExchangeShop );
#endif	ITEM_EXCHANGE_SHOP
	//}} kimhc // 2009-07-25 // 헤니르의 시공에 추가되는 ITEM_EXCHANGE_SHOP 작업

	//{{ kimhc // 2009-08-04 // 캐릭터별 은행
#ifdef	PRIVATE_BANK
	SAFE_DELETE( m_pPrivateBank );
#endif	PRIVATE_BANK
	//}} kimhc // 2009-08-04 // 캐릭터별 은행

	SAFE_DELETE(m_pPersonalTrade);
	m_pPersonalTrade = new CX2UIPersonalTrade(g_pMain->GetNowState(), NULL);
	
	SAFE_DELETE(m_pPersonalShop);
	m_pPersonalShop = NULL;

	SAFE_DELETE(m_pShop);
	m_pShop = NULL;

	SAFE_DELETE(m_pManufacture);
	m_pManufacture = NULL;

	SAFE_DELETE(m_pWatch);
	m_pWatch = NULL;    
	m_vLayer.clear();

	SAFE_DELETE(m_pQuickSlot);
	m_pQuickSlot = new CX2UIQuickSlot(g_pMain->GetNowState(), NULL);
	m_pQuickSlot->SetShow(false);

#ifdef SERV_EPIC_QUEST
	SAFE_DELETE(m_pQuestNew);
	m_pQuestNew = NULL;
#ifndef WORLD_TOOL
	m_pQuestNew = new CX2UIQuestNew(g_pMain->GetNowState(), NULL );
#endif
#else
	SAFE_DELETE(m_pQuestReceive);
	m_pQuestReceive = new CX2UIQuestReceive(g_pMain->GetNowState(), NULL );
	m_pQuestReceive->SetShow(false);	

	SAFE_DELETE(m_pQuest);
	m_pQuest = NULL;
#endif SERV_EPIC_QUEST
	//{{ 2011.03.16   임규수 아바타 분해 시스템
#ifdef SERV_MULTI_RESOLVE
	SAFE_DELETE(m_pResolveItem);
	m_pResolveItem = NULL;
#endif SERV_MULTI_RESOLVE
	//}}
	//{{ 2011.05.04   임규수 아바타 합성 시스템
#ifdef SERV_SYNTHESIS_AVATAR
	SAFE_DELETE(m_pSynthesisItem);
	m_pSynthesisItem = NULL;
#endif SERV_SYNTHESIS_AVATAR
	//}}

	m_LUMenu1st = UI_MENU_END;
	m_LUMenu2nd = UI_MENU_END;


#ifdef SERV_HERO_PVP_MANAGE_LIST
	m_pHeroMatch = new CX2UIHeroMatch();
#endif //SERV_HERO_PVP_MANAGE_LIST

#ifdef SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT
	ClearWeaddingXmesh();
	SAFE_DELETE_DIALOG( m_pDlgWeddingCaution );
	SAFE_DELETE_DIALOG( m_pDlgWeddingEventLetter );
#endif //SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT

	//m_pPersonalShop = new CX2UIPersonalShop(g_pMain->GetNowState(), NULL );
	//m_pPvPLobby = new CX2UIPvPList(g_pMain->GetNowState(), L"DLG_UI_Emulate.lua" );
	//m_pWatch	= new CX2UIWatch(g_pMain->GetNowState(), L"DLG_UI_Watch.lua" );
}


CX2UIManager::~CX2UIManager(void)
{
    SAFE_DELETE(m_pCharInfo);
	SAFE_DELETE(m_pInventory);
	SAFE_DELETE(m_pWatch);
	SAFE_DELETE(m_pPersonalShop);
	SAFE_DELETE(m_pPersonalTrade);
	SAFE_DELETE(m_pManufacture);
	SAFE_DELETE(m_pQuickSlot);

#ifdef SERV_EPIC_QUEST
	SAFE_DELETE(m_pQuestNew);
#else
	SAFE_DELETE(m_pQuestReceive);
	SAFE_DELETE(m_pQuest);
#endif SERV_EPIC_QUEST

#ifdef SERV_PET_SYSTEM
	SAFE_DELETE( m_pPetList );
#endif

#ifdef NEW_SKILL_TREE_UI
	SAFE_DELETE(m_pSkillTree);
#endif

#ifdef COMBO_TREE_UI_TEST
	SAFE_DELETE( m_pComboTreeUI );
#endif COMBO_TREE_UI_TEST


	//{{ kimhc // 실시간 엘소드 중 실시간 아이템 획득 관련 임시 인벤토리
#ifdef	REAL_TIME_ELSWORD
	SAFE_DELETE( m_pTempInventory );
#endif	REAL_TIME_ELSWORD
	//}} kimhc // 실시간 엘소드 중 실시간 아이템 획득 관련 임시 인벤토리
#ifdef POSTBOX_NOTICE
	SAFE_DELETE_DIALOG( m_pPostBoxWarning );
#endif POSTBOX_NOTICE
#ifdef DEF_TRADE_BOARD
	SAFE_DELETE( m_pPersonalShop );
#endif DEF_TRADE_BOARD
#ifdef GUILD_BOARD // oasis907 : 김상윤 [2009.11.24] // 길드 게시판
	SAFE_DELETE( m_pGuildBoard );	
#endif GUILD_BOARD

	//{{ kimhc // 2009-07-09 // 헤니르의 시공에 추가되는 Ranking UI 작업
#ifdef	RANKING_INFO_UI
	SAFE_DELETE( m_pRankingInfo );
#endif	RANKING_INFO_UI
	//}} kimhc // 2009-07-09 // 헤니르의 시공에 추가되는 Ranking UI 작업

	//{{ kimhc // 2009-07-25 // 헤니르의 시공에 추가되는 ITEM_EXCHANGE_SHOP 작업
#ifdef	ITEM_EXCHANGE_SHOP
	SAFE_DELETE( m_pItemExchangeShop );
#endif	ITEM_EXCHANGE_SHOP
	//}} kimhc // 2009-07-25 // 헤니르의 시공에 추가되는 ITEM_EXCHANGE_SHOP 작업

	//{{ kimhc // 2009-08-04 // 캐릭터별 은행
#ifdef	PRIVATE_BANK
	SAFE_DELETE( m_pPrivateBank );
#endif	PRIVATE_BANK
	//}} kimhc // 2009-08-04 // 캐릭터별 은행

	//{{ JHKang / 메모리 릭 수정하기 / 2010/10/29
#ifdef FIX_MEMORY_LEAK
	SAFE_DELETE( m_pShop );
#endif FIX_MEMORY_LEAK
	//}} JHKang / 메모리 릭 수정하기 / 2010/10/29

	//{{ 2011.03.16   임규수 아바타 분해 시스템
#ifdef SERV_MULTI_RESOLVE
	SAFE_DELETE( m_pResolveItem );
#endif SERV_MULTI_RESOLVE
	//}}
	//{{ 2011.05.04   임규수 아바타 합성 시스템
#ifdef SERV_SYNTHESIS_AVATAR
	SAFE_DELETE(m_pSynthesisItem);
#endif SERV_SYNTHESIS_AVATAR
	//}}

#ifdef SERV_HERO_PVP_MANAGE_LIST
	SAFE_DELETE( m_pHeroMatch );
#endif //SERV_HERO_PVP_MANAGE_LIST

#ifdef SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT
	ClearWeaddingXmesh();
	SAFE_DELETE_DIALOG( m_pDlgWeddingCaution );
	SAFE_DELETE_DIALOG( m_pDlgWeddingEventLetter );
#endif //SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT
}


HRESULT CX2UIManager::OnFrameMove( double fTime, float fElapsedTime )
{
	KTDXPROFILE();

	if(m_pQuickSlot != NULL && m_pQuickSlot->GetShow()) m_pQuickSlot->OnFrameMove( fTime, fElapsedTime );
	if(m_pCharInfo != NULL && m_pCharInfo->GetShow()) m_pCharInfo->OnFrameMove( fTime, fElapsedTime );
	if(m_pInventory != NULL && m_pInventory->GetShow()
#ifdef EXCHANGE_OPEN_IMAGE
		|| m_pInventory != NULL && m_pInventory->GetItemObtainResultType() != CX2UIInventory::IORT_NONE
#endif EXCHANGE_OPEN_IMAGE
		) m_pInventory->OnFrameMove( fTime, fElapsedTime );
	if(m_pWatch != NULL && m_pWatch->GetShow()) m_pWatch->OnFrameMove(fTime, fElapsedTime );
	if(m_pPersonalShop != NULL && m_pPersonalShop->GetShow()) m_pPersonalShop->OnFrameMove(fTime, fElapsedTime);
	if(m_pPersonalTrade != NULL) m_pPersonalTrade->OnFrameMove(fTime, fElapsedTime);		// GetShow에 관계없이 보내 준다
	if(m_pShop != NULL && m_pShop->GetShow() ) m_pShop->OnFrameMove(fTime, fElapsedTime);
	if(m_pManufacture != NULL && m_pManufacture->GetShow() ) m_pManufacture->OnFrameMove(fTime, fElapsedTime);
#ifdef SERV_EPIC_QUEST
	//if(GetShow(UI_MENU_QUEST_NEW)) m_pQuestNew->OnFrameMove(fTime, fElapsedTime);
#endif SERV_EPIC_QUEST

#ifndef SERV_EPIC_QUEST
	if(GetShow(UI_MENU_QUEST_RECEIVE) ) m_pQuestReceive->OnFrameMove(fTime, fElapsedTime);
	if(GetShow(UI_MENU_QUEST)) m_pQuest->OnFrameMove(fTime, fElapsedTime);
#endif SERV_EPIC_QUEST

#ifdef SERV_PET_SYSTEM
	if(GetShow(UI_MENU_PET_LIST)) m_pPetList->OnFrameMove(fTime, fElapsedTime);
#endif

#ifdef SERV_SKILL_NOTE
	if(GetShow(UI_SKILL_NOTE)) m_pSkillNote->OnFrameMove(fTime, fElapsedTime);
#endif

	//{{ kimhc // 실시간 엘소드 중 실시간 아이템 획득 관련 임시 인벤토리
#ifdef	REAL_TIME_ELSWORD
	if ( m_pTempInventory != NULL && m_pTempInventory->GetShow() )
		m_pTempInventory->OnFrameMove( fTime, fElapsedTime );
#endif	REAL_TIME_ELSWORD
	//}} kimhc // 실시간 엘소드 중 실시간 아이템 획득 관련 임시 인벤토리

#ifdef DEF_TRADE_BOARD
	if ( m_pPersonalShopBoard != NULL && m_pPersonalShopBoard->GetShow() )
		m_pPersonalShopBoard->OnFrameMove( fTime, fElapsedTime );
#endif DEF_TRADE_BOARD
#ifdef GUILD_BOARD // oasis907 : 김상윤 [2009.11.24] // 길드 게시판
	if ( m_pGuildBoard != NULL && m_pGuildBoard->GetShow() )
		m_pGuildBoard->OnFrameMove( fTime, fElapsedTime );
#endif GUILD_BOARD

	//{{ kimhc // 2009-07-09 // 헤니르의 시공에 추가되는 Ranking UI 작업
#ifdef	RANKING_INFO_UI
	if ( m_pRankingInfo != NULL && m_pRankingInfo->GetShow() )
		m_pRankingInfo->OnFrameMove( fTime, fElapsedTime );
#endif	RANKING_INFO_UI
	//}} kimhc // 2009-07-09 // 헤니르의 시공에 추가되는 Ranking UI 작업

	//{{ kimhc // 2009-07-25 // 헤니르의 시공에 추가되는 ITEM_EXCHANGE_SHOP 작업
#ifdef	ITEM_EXCHANGE_SHOP
	if ( m_pItemExchangeShop != NULL && m_pItemExchangeShop->GetShow() )
		m_pItemExchangeShop->OnFrameMove( fTime, fElapsedTime );
#endif	ITEM_EXCHANGE_SHOP
	//}} kimhc // 2009-07-25 // 헤니르의 시공에 추가되는 ITEM_EXCHANGE_SHOP 작업

	//{{ kimhc // 2009-08-04 // 캐릭터별 은행
#ifdef	PRIVATE_BANK
	if ( m_pPrivateBank	!=	NULL && m_pPrivateBank->GetShow() )
		m_pPrivateBank->OnFrameMove( fTime, fElapsedTime );
#endif	PRIVATE_BANK
	//}} kimhc // 2009-08-04 // 캐릭터별 은행

	/*if( NULL != m_pQuickSlot )
	{
		m_pQuickSlot->OnFrameMove( fTime, fElapsedTime );
	}*/

	//{{ 2011.03.16   임규수 아바타 분해 시스템
#ifdef SERV_MULTI_RESOLVE
	if ( m_pResolveItem != NULL && m_pResolveItem->GetShow() )
		m_pResolveItem->OnFrameMove( fTime, fElapsedTime );
#endif SERV_MULTI_RESOLVE
	//}}
	//{{ 2011.05.04   임규수 아바타 합성 시스템
#ifdef SERV_SYNTHESIS_AVATAR
	if ( m_pSynthesisItem != NULL && m_pSynthesisItem->GetShow() )
		m_pSynthesisItem->OnFrameMove( fTime, fElapsedTime );
#endif SERV_SYNTHESIS_AVATAR
	//}}

#ifdef SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT
	if( 0 < m_fWeddingClose )
	{
		m_fWeddingClose -= fElapsedTime;

		if( 0 > m_fWeddingClose )
		{
			m_fWeddingClose = -1;
			ClearWeaddingXmesh();
			if( NULL != m_pDlgWeddingEventLetter )
			{
				m_pDlgWeddingEventLetter->SetShow(false);
				
				CKTDGUIIMEEditBox* pImeEdit = static_cast<CKTDGUIIMEEditBox*>( m_pDlgWeddingEventLetter->GetControl(L"IME_Edit_Message") );
				if( NULL != pImeEdit )
				{
					pImeEdit->ClearText();
				}
			}
		}	
	}
#endif //SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT

    return S_OK;
}

HRESULT CX2UIManager::OnFrameRender()
{
    return S_OK;
}


bool CX2UIManager::MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	bool bFlag = false;

	switch(uMsg)
	{
	case WM_RBUTTONUP:
		{
			CX2State* pState = (CX2State*)g_pMain->GetNowState();	// DownCast니까 안심
			if(pState != NULL)
			{				
				CX2Cursor* pCursor = pState->GetCursor();
				if ( pCursor != NULL && pCursor->GetCurorState() != CX2Cursor::XCS_NORMAL )
				{
#ifdef SERV_ITEM_EXCHANGE_NEW // 디파인 잘 못 두른 것 해외팀 수정
					if(true == g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_ITEM_EXCHANGE_SHOP))
					{
						g_pData->GetUIManager()->GetUIItemExchangeShop()->InvalidLastSelectExchange();
					}
#endif SERV_ITEM_EXCHANGE_NEW
					pCursor->ChangeCursorState(CX2Cursor::XCS_NORMAL);
					return true;
				}
			}
		}
		break;
	case WM_LBUTTONDOWN:
		{
			SetDLGFlag(true);
		}
		break;
	default:
		break;
	}
#ifdef GUILD_SKILL	
	//{{ oasis907 : 김상윤 //// 2009-11-11 //// 
	if(g_pData != NULL && 
		g_pData->GetGuildManager() != NULL && 
		g_pData->GetGuildManager()->GetUIGuild() != NULL && 
		g_pData->GetGuildManager()->GetUIGuild()->GetUIGuildSkillTree() != NULL)
	{
		if ( g_pData->GetGuildManager()->GetUIGuild()->GetUIGuildSkillTree()->MsgProc(hWnd, uMsg, wParam, lParam) == true)
			return true;
	}
	//}} oasis907 : 김상윤 //// 2009-11-11 //// 
#endif GUILD_SKILL

#ifdef SERV_HERO_PVP_MANAGE_LIST //영웅대전 UI
	if( NULL != m_pHeroMatch && true == m_pHeroMatch->GetShow() )
	{
		if( true == m_pHeroMatch->MsgProc(hWnd, uMsg, wParam, lParam))
				return true;
	}
#endif //SERV_HERO_PVP_MANAGE_LIST

	if( m_pQuickSlot != NULL && 
		m_pQuickSlot->GetShow() == true &&
		m_pQuickSlot->MsgProc(hWnd, uMsg, wParam, lParam) ) 
	{
		bFlag = true;
		return true;
	}
	
	// enum이 int인 걸 이용한 땜빵 코드.
	// 정렬된 순서로 MSGPROC을 위에 있는 UI부터 처리한다.
	// 각 MSGPROC은 이벤트를 처리한 경우에 true -> 위부터 처리되므로 아래쪽의 UI에는 이미 처리된 메시지가 들어가지 않게 한다.
	// -> UI들이 겹친 경우 가장 위의 것만 메시지를 처리했다는 것을 확실하게 하자.
	// 구현시 주의사항 : 하위 UI들은 MsgProc에서 return true path를 탈 때가 아니면 *pSlotBeforeDragging을 리셋하지 말 것.
	// m_vLayer에 속한다는 것은 그 UI가 열려있다는 뜻이다 : GetShow 확인 삭제.

	bool bFlagChatBox = false;

	if( NULL != g_pData->GetMessenger() &&
		g_pData->GetMessenger()->GetOpenChatBox() == true)
	{
		if(g_pData->GetMessenger()->MsgProc(hWnd, uMsg, wParam, lParam) == true)
		{
			bFlagChatBox = true;
			bFlag = true;
		}
	}

#ifdef POSTBOX
	if( bFlagChatBox == false && g_pMain->GetPostBox() != NULL)
	{
		if(g_pMain->GetPostBox()->MsgProc( hWnd, uMsg, wParam, lParam ) == true)
		{
			bFlag = true;
		}
	}
#endif

#ifdef CLIENT_SECOND_SECURITY
	if( g_pMain->GetSecurityPad() != NULL)
	{
		if(g_pMain->GetSecurityPad()->MsgProc( hWnd, uMsg, wParam, lParam ) == true)
		{
			bFlag = true;
		}
	}
#endif CLIENT_SECOND_SECURITY

	//{{ 2011.03.16   임규수 아바타 분해 시스템
#ifdef SERV_MULTI_RESOLVE
	if(m_pResolveItem != NULL)
	{
		if(m_pResolveItem->MsgProc(hWnd, uMsg, wParam, lParam))
				bFlag = true;
	}
#endif SERV_MULTI_RESOLVE
	//}}
	//{{ 2011.05.04   임규수 아바타 합성 시스템
#ifdef SERV_SYNTHESIS_AVATAR
	if(m_pSynthesisItem != NULL)
	{
		if(m_pSynthesisItem->MsgProc(hWnd, uMsg, wParam, lParam))
			bFlag = true;
	}
#endif SERV_SYNTHESIS_AVATAR
	//}}

	for(int i=(int)m_vLayer.size()-1; i>= 0; i--)
	{
//		if(bFlag) break;
		UI_MENU menu = m_vLayer[i];
		switch(menu)
		{
#ifdef SERV_EPIC_QUEST
		case UI_MENU_QUEST_NEW:
			if(!bFlag)
			{
				if(m_pQuestNew->MsgProc(hWnd, uMsg, wParam, lParam) ) 
					bFlag = true;
			}
			else
			{
				m_pQuestNew->InvalidSelectedItem();
				m_pQuestNew->InvalidSlotDesc();
			}
			break;
#else
		case UI_MENU_QUEST:
			if(!bFlag)
			{
				if(m_pQuest->MsgProc(hWnd, uMsg, wParam, lParam) ) 
					bFlag = true;
			}
			else
			{
				m_pQuest->InvalidSelectedItem();
				m_pQuest->InvalidSlotDesc();
			}
			break;
		case UI_MENU_QUEST_RECEIVE:
			if(!bFlag)
			{
                if(m_pQuestReceive->MsgProc(hWnd, uMsg, wParam, lParam) ) 
					bFlag = true;
			}
			else
			{
				m_pQuestReceive->InvalidSelectedItem();
				m_pQuestReceive->InvalidSlotDesc();
			}
			break;
#endif SERV_EPIC_QUEST
		case UI_MENU_MANUFACTURE:
			{
				if(!bFlag)
				{
					if(m_pManufacture->MsgProc(hWnd, uMsg, wParam, lParam) ) 
						bFlag = true;
				}
				else
				{
					m_pManufacture->InvalidSelectedItem();
					m_pManufacture->InvalidSlotDesc();
				}
			}
			break;
		case UI_MENU_SHOP:
			{
				if(!bFlag)
				{
					if(m_pShop->MsgProc(hWnd, uMsg, wParam, lParam) ) 
						bFlag = true;
				}
				else
				{
					m_pShop->InvalidSelectedItem();
					m_pShop->InvalidSlotDesc();
				}
			}
			break;

#ifdef DEF_TRADE_BOARD
		case UI_MENU_PERSONAL_SHOP_BOARD:
			{
				if ( !bFlag )
				{
					if ( m_pPersonalShopBoard->MsgProc( hWnd, uMsg, wParam, lParam ) )
						bFlag = true;
				}
			}
			break;
#endif DEF_TRADE_BOARD

#ifdef GUILD_BOARD // oasis907 : 김상윤 [2009.11.24] // 길드 게시판
		case UI_MENU_GUILD_BOARD:
			{
				if ( !bFlag )
				{
					if ( m_pGuildBoard->MsgProc( hWnd, uMsg, wParam, lParam ) )
						bFlag = true;
				}
			}
			break;
#endif GUILD_BOARD


			//{{ kimhc // 2009-07-09 // 헤니르의 시공에 추가되는 Ranking UI 작업
#ifdef	RANKING_INFO_UI
		case UI_MENU_RANKING_INFO:
			{
				if ( !bFlag )
				{
					if ( m_pRankingInfo->MsgProc( hWnd, uMsg, wParam, lParam ) )
						bFlag = true;
				}
			}
			break;

#endif	RANKING_INFO_UI
			//}} kimhc // 2009-07-09 // 헤니르의 시공에 추가되는 Ranking UI 작업

			//{{ kimhc // 2009-07-25 // 헤니르의 시공에 추가되는 ITEM_EXCHANGE_SHOP 작업
#ifdef	ITEM_EXCHANGE_SHOP
		case UI_MENU_ITEM_EXCHANGE_SHOP:
			{
				if ( !bFlag )
				{
					if ( m_pItemExchangeShop->MsgProc( hWnd, uMsg, wParam, lParam ) )
						bFlag = true;					
				}
				
			}
			break;
#endif	ITEM_EXCHANGE_SHOP
			//}} kimhc // 2009-07-25 // 헤니르의 시공에 추가되는 ITEM_EXCHANGE_SHOP 작업

			//{{ kimhc // 2009-08-04 // 캐릭터별 은행
#ifdef	PRIVATE_BANK
		case UI_MENU_PRIVATE_BANK:
			{
				if ( !bFlag )
				{
					if ( m_pPrivateBank->MsgProc( hWnd, uMsg, wParam, lParam ) )
						bFlag = true;					
				}
				else
				{
					m_pPrivateBank->InvalidSelectedItem();
					m_pPrivateBank->InvalidSlotDesc();
				}
			}
			break;
#endif	PRIVATE_BANK
			//}} kimhc // 2009-08-04 // 캐릭터별 은행

		case UI_MENU_PERSONAL_SHOP:
			if(!bFlag)
			{
				if(m_pPersonalShop->MsgProc(hWnd, uMsg, wParam, lParam) ) 
					bFlag = true;
			}
			else
			{
				m_pPersonalShop->InvalidSelectedItem();
				m_pPersonalShop->InvalidSlotDesc();
			}
			break;
		case UI_MENU_CHARINFO:
			if(!bFlag)
			{
				if(m_pCharInfo->MsgProc(hWnd, uMsg, wParam, lParam) ) 
					bFlag = true;
			}
			else
			{
				m_pCharInfo->InvalidSelectedItem();
				m_pCharInfo->InvalidSlotDesc();
			}
			break;
		case UI_MENU_WATCH:
			if(!bFlag)
			{
				if(m_pWatch->MsgProc(hWnd, uMsg, wParam, lParam) ) 
					bFlag = true;
			}
			else
			{
				m_pWatch->InvalidSelectedItem();
				m_pWatch->InvalidSlotDesc();
			}
			break;
#ifdef SERV_SKILL_NOTE
		case UI_SKILL_NOTE:
			if(!bFlag)
			{
				if(m_pSkillNote->MsgProc(hWnd, uMsg, wParam, lParam) ) 
					bFlag = true;
			}			
			break;
#endif
		case UI_MENU_INVEN:
			if(!bFlag)
			{
				if(m_pInventory->MsgProc(hWnd, uMsg, wParam, lParam) ) 
					bFlag = true;
			}
			else
			{
				m_pInventory->InvalidSelectedItem();
				m_pInventory->InvalidSlotDesc();
			}
			break;
		case UI_MENU_PERSONAL_TRADE:
			if(!bFlag)
			{
				if(m_pPersonalTrade->MsgProc(hWnd, uMsg, wParam, lParam) )
					bFlag = true;
			}
			else
			{
				m_pPersonalTrade->InvalidSelectedItem();
				m_pPersonalTrade->InvalidSlotDesc();
			}
			break;

		case UI_MENU_COMMUNITY:
			if(!bFlag)
			{
				if( NULL != g_pData->GetMessenger() )
				{
					if(g_pData->GetMessenger()->MsgProc(hWnd, uMsg, wParam, lParam) ) 
						bFlag = true;
				}
			}
			break;
#ifdef SERV_PET_SYSTEM
		case UI_MENU_PET_LIST:
			{
				if(!bFlag)
				{
					if(m_pPetList->MsgProc(hWnd, uMsg, wParam, lParam))
						bFlag = true;
				}
			} break;
#endif
#ifdef NEW_SKILL_TREE_UI
		case UI_MENU_SKILL:
			{
				if(!bFlag)
				{
					if(m_pSkillTree->MsgProc(hWnd, uMsg, wParam, lParam))
						bFlag = true;
				}
				else
				{
					if ( false == m_pSkillTree->GetShow() )
					{
						m_pSkillTree->ShowSkillDesc(false);
						m_pSkillTree->ShowUIDesc(false);
					}
				}
			} break;
#endif


			//{{ kimhc // 실시간 엘소드 중 실시간 아이템 획득 관련 임시 인벤토리
#ifdef	REAL_TIME_ELSWORD
		case UI_TEMP_INVENTORY:
			{
				if ( !bFlag )
				{
					if ( m_pTempInventory != NULL && m_pTempInventory->GetShow() == true )
					{
						if ( m_pTempInventory->MsgProc( hWnd, uMsg, wParam, lParam ) == true )
							bFlag = true;
					}
				}
				else
				{
					if ( m_pTempInventory != NULL )
					{
						m_pTempInventory->InvalidSelectedItem();
						m_pTempInventory->InvalidSlotDesc();
					}

				}
			}
			break;
#endif	REAL_TIME_ELSWORD
			//}} kimhc // 실시간 엘소드 중 실시간 아이템 획득 관련 임시 인벤토리
		default:
			break;
		}
	}

	switch( uMsg )
	{
	case WM_RBUTTONUP:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
		{
			SetNowDragItemReturn();
		}
	default:
		break;
	}
		
    return bFlag;
}


bool CX2UIManager::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{

#ifdef SERV_LOCAL_RANKING_SYSTEM
	if( NULL != g_pData->GetProfileManager() && true == g_pData->GetProfileManager()->GetShow() )
	{
		if( true == g_pData->GetProfileManager()->UICustomEventProc( hWnd, uMsg, wParam, lParam ) )
			return true;
	}
#endif //SERV_LOCAL_RANKING_SYSTEM

	if(m_pCharInfo != NULL && m_pCharInfo->GetShow() == true)
	{
		// 추후 다른 UI와 동시 처리될 수 있는지 확인하여 return값 결정한다.
		if(m_pCharInfo->UICustomEventProc(hWnd, uMsg, wParam, lParam) == true)
			return true;
	}
	// 2008.11.10 김태완 : 인벤토리 쪽 msg proc 추가
	if(m_pInventory != NULL && m_pInventory->GetShow() == true)
	{
		// 추후 다른 UI와 동시 처리될 수 있는지 확인하여 return값 결정한다.
		if(m_pInventory->UICustomEventProc(hWnd, uMsg, wParam, lParam) == true)
			return true;
	}
#ifdef RANDOM_CUBE_UI_NEW
	else if(m_pInventory != NULL && m_pInventory->GetDLGResolveItemResult() != NULL
#ifdef EXCHANGE_OPEN_IMAGE
		|| m_pInventory != NULL && m_pInventory->GetItemObtainResultType() != CX2UIInventory::IORT_NONE
#endif EXCHANGE_OPEN_IMAGE
		)
	{
		// oasis907 : 김상윤 [2010.7.27] // 인벤토리창은 닫혔는데 큐브 결과창은 열린상태
		if(m_pInventory->UICustomEventProc(hWnd, uMsg, wParam, lParam) == true)
			return true;
	}
#endif RANDOM_CUBE_UI_NEW

	if(m_pWatch != NULL && m_pWatch->GetShow() == true)
	{
		if(m_pWatch->UICustomEventProc(hWnd, uMsg, wParam, lParam) == true)
			return true;
	}
#ifdef SERV_UPGRADE_TRADE_SYSTEM // 김태환
	if ( m_pPersonalShop != NULL )
#else //SERV_UPGRADE_TRADE_SYSTEM
	if(m_pPersonalShop != NULL && m_pPersonalShop->GetShow() == true)
#endif //SERV_UPGRADE_TRADE_SYSTEM
	{
		if(m_pPersonalShop->UICustomEventProc(hWnd, uMsg, wParam, lParam) == true)
			return true;
	}
	if(m_pPersonalTrade != NULL)		// Show랑 상관없이
	{
		if(m_pPersonalTrade->UICustomEventProc(hWnd, uMsg, wParam, lParam) == true)
			return true;
	}
	
	if(m_pShop != NULL && m_pShop->GetShow() == true)
	{
		if(m_pShop->UICustomEventProc(hWnd, uMsg, wParam, lParam) == true)
			return true;
	}

#ifdef DEF_TRADE_BOARD
	if ( m_pPersonalShopBoard != NULL && m_pPersonalShopBoard->GetShow() == true )
	{
		if ( m_pPersonalShopBoard->UICustomEventProc( hWnd, uMsg, wParam, lParam ) == true )
			return true;
	}
#endif DEF_TRADE_BOARD
#ifdef	GUILD_BOARD // oasis907 : 김상윤 [2009.11.24] // 길드 게시판
	if ( m_pGuildBoard != NULL && m_pGuildBoard->GetShow() == true )
	{
		if ( m_pGuildBoard->UICustomEventProc( hWnd, uMsg, wParam, lParam ) == true )
			return true;
	}
#endif	GUILD_BOARD


	//{{ kimhc // 2009-07-09 // 헤니르의 시공에 추가되는 Ranking UI 작업
#ifdef	RANKING_INFO_UI
	if ( m_pRankingInfo != NULL && m_pRankingInfo->GetShow() == true )
	{
		if ( m_pRankingInfo->UICustomEventProc( hWnd, uMsg, wParam, lParam ) == true )
			return true;
	}
#endif	RANKING_INFO_UI
	//}} kimhc // 2009-07-09 // 헤니르의 시공에 추가되는 Ranking UI 작업

	//{{ kimhc // 2009-07-25 // 헤니르의 시공에 추가되는 ITEM_EXCHANGE_SHOP 작업
#ifdef	ITEM_EXCHANGE_SHOP
	if ( m_pItemExchangeShop != NULL && m_pItemExchangeShop->GetShow() == true )
	{
		if ( m_pItemExchangeShop->UICustomEventProc( hWnd, uMsg, wParam, lParam )  == true )
			return true;		
	}	
#endif	ITEM_EXCHANGE_SHOP
	//}} kimhc // 2009-07-25 // 헤니르의 시공에 추가되는 ITEM_EXCHANGE_SHOP 작업

	//{{ kimhc // 2009-08-04 // 캐릭터별 은행
#ifdef	PRIVATE_BANK
	if ( m_pPrivateBank != NULL && m_pPrivateBank->GetShow() == true )
	{
		if ( m_pPrivateBank->UICustomEventProc( hWnd, uMsg, wParam, lParam ) == true )
			return true;		
	}
#endif	PRIVATE_BANK
	//}} kimhc // 2009-08-04 // 캐릭터별 은행

	if(m_pManufacture != NULL && m_pManufacture->GetShow() == true)
	{
		if(m_pManufacture->UICustomEventProc(hWnd, uMsg, wParam, lParam) == true)
			return true;
	}

#ifdef SERV_HERO_PVP_MANAGE_LIST //영웅대전 UI
	if( NULL != m_pHeroMatch && true == m_pHeroMatch->GetShow() )
	{
		if( true == m_pHeroMatch->UICustomEventProc( hWnd, uMsg, wParam, lParam ) ) 
			return true;
	}
#endif //SERV_HERO_PVP_MANAGE_LIST

#ifdef SERV_EPIC_QUEST
	if(m_pQuestNew != NULL)
	{
		if(m_pQuestNew->UICustomEventProc(hWnd, uMsg, wParam, lParam) == true)
			return true;
	}
#else
	if(GetShow(UI_MENU_QUEST_RECEIVE))
	{
		if(m_pQuestReceive->UICustomEventProc(hWnd, uMsg, wParam, lParam) == true)
			return true;
	}
	if(m_pQuest != NULL)		// Show랑 상관없이 : 나중에 퀵퀘스트 열린 거 여부랑 같이 확인하게 바꾸자
	{
		if(m_pQuest->UICustomEventProc(hWnd, uMsg, wParam, lParam) == true)
			return true;
	}
#endif SERV_EPIC_QUEST

#ifdef SERV_PET_SYSTEM
	if( m_pPetList != NULL )
	{
		if( m_pPetList->UICustomEventProc(hWnd, uMsg, wParam, lParam) == true )
			return true;
	}
#endif

#ifdef NEW_SKILL_TREE_UI
	if(m_pSkillTree != NULL)	// Show랑 상관없이 : 게임 중에 슬롯 툴팁을 표시하기 위해서
	{
		if(m_pSkillTree->UICustomEventProc(hWnd, uMsg, wParam, lParam) == true)
			return true;
	}
#endif


#ifdef COMBO_TREE_UI_TEST
	if( NULL != m_pComboTreeUI )
	{
		if( true == m_pComboTreeUI->UICustomEventProc( hWnd, uMsg, wParam, lParam ) )
			return true;
	}

#endif COMBO_TREE_UI_TEST


	//{{ kimhc	// 실시간 엘소드 중 실시간 아이템 획득 관련 임시 인벤토리
#ifdef	REAL_TIME_ELSWORD
	if ( m_pTempInventory != NULL )
	{
		if ( m_pTempInventory->UICustomEventProc( hWnd, uMsg, wParam, lParam ) == true )
			return true;
	}
#endif	REAL_TIME_ELSWORD
	//}} kimhc	// 실시간 엘소드 중 실시간 아이템 획득 관련 임시 인벤토리

#ifdef SERV_SKILL_NOTE
	if(m_pSkillNote != NULL && m_pSkillNote->GetShow() == true)
	{
		// 추후 다른 UI와 동시 처리될 수 있는지 확인하여 return값 결정한다.
		if(m_pSkillNote->UICustomEventProc(hWnd, uMsg, wParam, lParam) == true)
			return true;
	}
#endif

	//{{ 2011.03.16   임규수 아바타 분해 시스템
#ifdef SERV_MULTI_RESOLVE
	if(m_pResolveItem != NULL && m_pResolveItem->GetShow() == true)
	{
		if(m_pResolveItem->UICustomEventProc(hWnd, uMsg, wParam, lParam) == true)
			return true;
	}
#endif SERV_MULTI_RESOLVE
	//}}
	//{{ 2011.05.04   임규수 아바타 합성 시스템
#ifdef SERV_SYNTHESIS_AVATAR
	if(m_pSynthesisItem != NULL && m_pSynthesisItem->GetShow() == true)
	{
		if(m_pSynthesisItem->UICustomEventProc(hWnd, uMsg, wParam, lParam) == true)
			return true;
	}
#endif SERV_SYNTHESIS_AVATAR
	//}}
    return false;
}

bool CX2UIManager::UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{

	// Server Event는 SetShow에 상관없이 처리하도록 한다 : 인벤토리/장비 업데이트 등등에 영향을 주는 경우가 있다.
	// 사실 Server Event는 UI에서 처리할 성질의 것이 아니긴 한데.. 
	// 스킬트리는 서버이벤트를 받지 않는다 (Data와 UI가 분리되어 있다 : X2SkillTree, X2UserSkillTree, X2UISkillTree 로 분리)
	
	if(m_pCharInfo != NULL)
	{
		// 추후 다른 UI와 동시 처리될 수 있는지 확인하여 return값 결정한다.
		if(m_pCharInfo->UIServerEventProc(hWnd, uMsg, wParam, lParam) == true)
			return true;
	}
	// 2008.11.10 김태완 : 인벤토리 쪽 msg proc 추가
	if(m_pInventory != NULL == true)
	{
		// 추후 다른 UI와 동시 처리될 수 있는지 확인하여 return값 결정한다.
		if(m_pInventory->UIServerEventProc(hWnd, uMsg, wParam, lParam) == true)
			return true;
	}

	if(m_pPersonalShop != NULL)
	{
		if(m_pPersonalShop->UIServerEventProc(hWnd, uMsg, wParam, lParam) == true)
			return true;
	}
	if(m_pPersonalTrade != NULL)
	{
		if(m_pPersonalTrade->UIServerEventProc(hWnd, uMsg, wParam, lParam) == true)
			return true;
	}
	if(m_pShop != NULL)
	{
		if(m_pShop->UIServerEventProc(hWnd, uMsg, wParam, lParam) == true)
			return true;
	}

#ifdef DEF_TRADE_BOARD
	if ( m_pPersonalShopBoard != NULL )
	{
		if ( m_pPersonalShopBoard->UIServerEventProc( hWnd, uMsg, wParam, lParam ) == true )
			return true;
	}
#endif DEF_TRADE_BOARD
#ifdef GUILD_BOARD // oasis907 : 김상윤 [2009.11.24] // 길드 게시판
	if ( m_pGuildBoard != NULL )
	{
		if ( m_pGuildBoard->UIServerEventProc( hWnd, uMsg, wParam, lParam ) == true )
			return true;
	}
#endif GUILD_BOARD



	//{{ kimhc // 2009-07-09 // 헤니르의 시공에 추가되는 Ranking UI 작업
#ifdef	RANKING_INFO_UI
	if ( m_pRankingInfo != NULL )
	{
		if ( m_pRankingInfo->UIServerEventProc( hWnd, uMsg, wParam, lParam ) == true )
			return true;
	}
#endif	RANKING_INFO_UI
	//}} kimhc // 2009-07-09 // 헤니르의 시공에 추가되는 Ranking UI 작업

	//{{ kimhc // 2009-07-25 // 헤니르의 시공에 추가되는 ITEM_EXCHANGE_SHOP 작업
#ifdef	ITEM_EXCHANGE_SHOP
	if ( m_pItemExchangeShop != NULL )
	{
		if ( m_pItemExchangeShop->UIServerEventProc( hWnd, uMsg, wParam, lParam ) == true )
			return true;		
	}
#endif	ITEM_EXCHANGE_SHOP
	//}} kimhc // 2009-07-25 // 헤니르의 시공에 추가되는 ITEM_EXCHANGE_SHOP 작업

	//{{ kimhc // 2009-08-04 // 캐릭터별 은행
#ifdef	PRIVATE_BANK
	if ( m_pPrivateBank	!= NULL )
	{
		if ( m_pPrivateBank->UIServerEventProc( hWnd, uMsg, wParam, lParam ) == true )
			return true;		
	}
#endif	PRIVATE_BANK
		//}} kimhc // 2009-08-04 // 캐릭터별 은행

	if(m_pManufacture != NULL)
	{
		if(m_pManufacture->UIServerEventProc(hWnd, uMsg, wParam, lParam) == true)
			return true;
	}
#ifdef SERV_EPIC_QUEST
	if(m_pQuestNew != NULL)
	{
		if(m_pQuestNew->UIServerEventProc(hWnd, uMsg, wParam, lParam) == true)
			return true;
	}
#else
	if(m_pQuestReceive != NULL)
	{
		if(m_pQuestReceive->UIServerEventProc(hWnd, uMsg, wParam, lParam) == true)
			return true;
	}
	if(m_pQuest != NULL)
	{
		if(m_pQuest->UIServerEventProc(hWnd, uMsg, wParam, lParam) == true)
			return true;
	}
#endif SERV_EPIC_QUEST
	if( NULL != m_pQuickSlot )
	{
		if( true == m_pQuickSlot->UIServerEventProc(hWnd, uMsg, wParam, lParam) )
			return true;
	}

	//{{ kimhc	// 실시간 엘소드 중 실시간 아이템 획득 관련 임시 인벤토리
#ifdef	REAL_TIME_ELSWORD
	if ( m_pTempInventory != NULL )
	{
		if ( m_pTempInventory->UIServerEventProc( hWnd, uMsg, wParam, lParam ) == true )
			return true;
	}
#endif	REAL_TIME_ELSWORD
	//}} kimhc	// 실시간 엘소드 중 실시간 아이템 획득 관련 임시 인벤토리

#ifdef SERV_SKILL_NOTE
	if( m_pSkillNote != NULL )
	{
		if( m_pSkillNote->UIServerEventProc( hWnd, uMsg, wParam, lParam) == true )
			return true;
	}
#endif

#ifdef SERV_PET_SYSTEM
	if( m_pPetList != NULL )
	{
		if( m_pPetList->UIServerEventProc( hWnd, uMsg, wParam, lParam) == true )
			return true;
	}
#endif

	//{{ 2011.03.16   임규수 아바타 분해 시스템
#ifdef SERV_MULTI_RESOLVE
	if( m_pResolveItem != NULL )
	{
		if(m_pResolveItem->UIServerEventProc(hWnd, uMsg, wParam, lParam) == true)
			return true;
	}
#endif SERV_MULTI_RESOLVE
	//}}
	//{{ 2011.05.04   임규수 아바타 합성 시스템
#ifdef SERV_SYNTHESIS_AVATAR
		if( m_pSynthesisItem != NULL )
		{
			if(m_pSynthesisItem->UIServerEventProc(hWnd, uMsg, wParam, lParam) == true)
				return true;
		}
#endif SERV_SYNTHESIS_AVATAR
	//}}

#ifdef SERV_HERO_PVP_MANAGE_LIST //영웅대전 UI
	if( NULL != m_pHeroMatch && true == m_pHeroMatch->GetShow() )
	{
		if( true == m_pHeroMatch->UIServerEventProc( hWnd, uMsg, wParam, lParam ) ) 
			return true;
	}
#endif //SERV_HERO_PVP_MANAGE_LIST

	switch( wParam)
	{
	case EGS_WATCH_UNIT_ACK:
		return Handler_EGS_WATCH_UNIT_ACK( hWnd, uMsg, wParam, lParam );

#ifdef SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT
	case EGS_SEND_LOVE_LETTER_EVENT_ACK:
		return Handler_EGS_SEND_LOVE_LETTER_EVENT_ACK( hWnd, uMsg, wParam, lParam );
#endif //SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT

#ifdef SERV_ITEM_ACTION_BY_DBTIME_SETTING // 2012.12.12 lygan_조성욱 // 석근이 작업 리뉴얼 ( DB에서 실시간 값 반영, 교환, 제조 쪽도 적용 )
	case EGS_BUY_UI_SETTING_ACK:
		return Handler_EGS_BUY_UI_SETTING_ACK( hWnd, uMsg, wParam, lParam );

	case EGS_GET_TIME_CONTROL_ITME_LIST_NOT:
		return Handler_EGS_GET_TIME_CONTROL_ITME_LIST_NOT( hWnd, uMsg, wParam, lParam );

	case EGS_GET_TIME_CONTROL_ITME_TALK_LIST_ACK:
		return Handler_EGS_GET_TIME_CONTROL_ITME_TALK_LIST_ACK( hWnd, uMsg, wParam, lParam );
#endif //SERV_ITEM_ACTION_BY_DBTIME_SETTING
#ifdef SERV_MANUFACTURE_PERIOD_FIX	
	case EGS_MANUFACTURE_PERIOD_SETTING_ACK:
		return Handler_EGS_MANUFACTURE_PERIOD_SETTING_ACK( hWnd, uMsg, wParam, lParam );
#endif //SERV_MANUFACTURE_PERIOD_FIX
	}

    return false;
}

bool CX2UIManager::IsOpenedWindow()
{
	bool bShow = false;

	bShow = GetShowNpcShop();

#ifdef SERV_EPIC_QUEST
	if(bShow == false && m_pQuestNew != NULL) 
		bShow = m_pQuestNew->GetShow();
#else
	if(bShow == false && m_pQuest != NULL) 
		bShow = m_pQuest->GetShow();
	if(bShow == false && m_pQuestReceive != NULL ) 
		bShow = m_pQuestReceive->GetShow();
#endif SERV_EPIC_QUEST
	if(bShow == false && m_pPersonalTrade != NULL )
		bShow = m_pPersonalTrade->GetShow();
	if(bShow == false && m_pPersonalShop!=NULL)
		bShow = m_pPersonalShop->GetShow();
	if(bShow == false && m_pCharInfo!=NULL)
		bShow = m_pCharInfo->GetShow();
	if(bShow == false && m_pWatch != NULL)
		bShow = m_pWatch->GetShow();
	if(bShow == false && m_pInventory!=NULL)
		bShow = m_pInventory->GetShow();
#ifdef SERV_PET_SYSTEM
	if(bShow == false && m_pPetList != NULL )
		bShow = m_pPetList->GetShow();
#endif
#ifdef NEW_SKILL_TREE_UI
	if(bShow == false && m_pSkillTree != NULL )
		bShow = m_pSkillTree->GetShow();
#endif

#ifdef COMBO_TREE_UI_TEST
	if( bShow == false && m_pComboTreeUI != NULL )
		bShow = m_pComboTreeUI->GetShow();
#endif COMBO_TREE_UI_TEST

	//{{ kimhc	// 실시간 엘소드 중 실시간 아이템 획득 관련 임시 인벤토리
#ifdef	REAL_TIME_ELSWORD
	if ( bShow == false && m_pTempInventory != NULL )
		bShow = m_pTempInventory->GetShow();
#endif	REAL_TIME_ELSWORD
	//}} kimhc	// 실시간 엘소드 중 실시간 아이템 획득 관련 임시 인벤토리

	return bShow;
}

// ~~가 열렸습니까?
// NULL Check의 역할도 동시에 겸한다.
bool CX2UIManager::GetShow(UI_MENU menuInx)
{
	switch(menuInx)
	{
#ifdef SERV_EPIC_QUEST
	case UI_MENU_QUEST_NEW:
		if(m_pQuestNew != NULL) return m_pQuestNew->GetShow();
		break;
#else
	case UI_MENU_QUEST:
		if(m_pQuest != NULL) return m_pQuest->GetShow();
		break;
	case UI_MENU_QUEST_RECEIVE:
		if(m_pQuestReceive != NULL ) return m_pQuestReceive->GetShow();
		break;
#endif SERV_EPIC_QUEST
	case UI_MENU_QUICK_SLOT:
		if(m_pQuickSlot != NULL) return m_pQuickSlot->GetShow();
		break;
	case UI_MENU_MANUFACTURE:
		if(m_pManufacture != NULL) return m_pManufacture->GetShow();
		break;
	case UI_MENU_SHOP:
		if(m_pShop != NULL ) return m_pShop->GetShow();
		break;
#ifdef POSTBOX_NOTICE
	case UI_MENU_POSTBOX_NOTICE:
		if ( m_pPostBoxWarning != NULL )
			return m_pPostBoxWarning->GetShow();
		break;
#endif POSTBOX_NOTICE
#ifdef DEF_TRADE_BOARD
	case UI_MENU_PERSONAL_SHOP_BOARD:
		if ( m_pPersonalShopBoard != NULL )
			return m_pPersonalShopBoard->GetShow();
		break;
#endif DEF_TRADE_BOARD
#ifdef GUILD_BOARD // oasis907 : 김상윤 [2009.11.24] //
	case UI_MENU_GUILD_BOARD:
		if ( m_pGuildBoard != NULL )
			return m_pGuildBoard->GetShow();
		break;
#endif GUILD_BOARD


		//{{ kimhc // 2009-07-09 // 헤니르의 시공에 추가되는 Ranking UI 작업
#ifdef	RANKING_INFO_UI
	case UI_MENU_RANKING_INFO:
		if ( m_pRankingInfo != NULL )
			return m_pRankingInfo->GetShow();
		break;
#endif	RANKING_INFO_UI
		//}} kimhc // 2009-07-09 // 헤니르의 시공에 추가되는 Ranking UI 작업

		//{{ kimhc // 2009-07-25 // 헤니르의 시공에 추가되는 ITEM_EXCHANGE_SHOP 작업
#ifdef	ITEM_EXCHANGE_SHOP
	case UI_MENU_ITEM_EXCHANGE_SHOP:
		if ( m_pItemExchangeShop != NULL )
			return m_pItemExchangeShop->GetShow();
		break;		
#endif	ITEM_EXCHANGE_SHOP
		//}} kimhc // 2009-07-25 // 헤니르의 시공에 추가되는 ITEM_EXCHANGE_SHOP 작업

		//{{ kimhc // 2009-08-04 // 캐릭터별 은행
#ifdef	PRIVATE_BANK
	case UI_MENU_PRIVATE_BANK:
		if ( m_pPrivateBank != NULL )
			return m_pPrivateBank->GetShow();
		break;
#endif	PRIVATE_BANK
		//}} kimhc // 2009-08-04 // 캐릭터별 은행

	case UI_MENU_PARTY:
		if(g_pMain->GetPartyUI() != NULL ) return g_pMain->GetPartyUI()->GetShow();
		break;				
	case UI_MENU_PERSONAL_TRADE:
		if(m_pPersonalTrade != NULL ) return m_pPersonalTrade->GetShow();
		break;
	case UI_MENU_PERSONAL_SHOP:
		if(m_pPersonalShop!=NULL) return m_pPersonalShop->GetShow();
		break;
	case UI_MENU_CHARINFO:
		if(m_pCharInfo!=NULL) return m_pCharInfo->GetShow();
		break;
#ifdef SERV_PET_SYSTEM
	case UI_MENU_PET_LIST:
		if( m_pPetList != NULL ) return m_pPetList->GetShow();
		break;
#endif
#ifdef NEW_SKILL_TREE_UI
	case UI_MENU_SKILL:
		if(m_pSkillTree != NULL) return m_pSkillTree->GetShow();
		break;	
#endif


#ifdef COMBO_TREE_UI_TEST
	case UI_MENU_COMBO_TREE:
		{
			if( NULL != m_pComboTreeUI )
			{
				return m_pComboTreeUI->GetShow();
			}
		} break;
#endif COMBO_TREE_UI_TEST

		

		//{{ kimhc	// 실시간 엘소드 중 실시간 아이템 획득 관련 임시 인벤토리
#ifdef	REAL_TIME_ELSWORD
	case UI_TEMP_INVENTORY:
		if ( m_pTempInventory != NULL )
			return m_pTempInventory->GetShow();

		break;
#endif	REAL_TIME_ELSWORD
		//}} kimhc	// 실시간 엘소드 중 실시간 아이템 획득 관련 임시 인벤토리

	case UI_MENU_WATCH:
		if(m_pWatch != NULL) return m_pWatch->GetShow();
		break;
#ifdef SERV_SKILL_NOTE
	case UI_SKILL_NOTE:
		if( m_pSkillNote != NULL ) return m_pSkillNote->GetShow();
		break;
#endif
	case UI_MENU_INVEN:
		if(m_pInventory!=NULL) return m_pInventory->GetShow();
		break;
	case UI_MENU_COMMUNITY:		
		if(g_pData->GetMessenger() != NULL)
		{
			if( true == g_pData->GetMessenger()->GetOpen() && CX2Community::XMUT_GUILD == g_pData->GetMessenger()->GetOpenTab() )
				return false;
			else
				return g_pData->GetMessenger()->GetOpen();
		}
		break;
	case UI_MENU_OPTION:
		return false;
		//{{ 2011.03.16   임규수 아바타 분해 시스템
#ifdef SERV_MULTI_RESOLVE
	case UI_MENU_RESOLVE:
		if(m_pResolveItem != NULL ) return m_pResolveItem->GetShow();
		break;
#endif SERV_MULTI_RESOLVE
		//}}
		//{{ 2011.05.04   임규수 아바타 합성 시스템
#ifdef SERV_SYNTHESIS_AVATAR
	case UI_MENU_SYNTHESIS:
		if(m_pSynthesisItem != NULL) return m_pSynthesisItem->GetShow();
		break;
#endif SERV_SYNTHESIS_AVATAR
		//}}
#ifdef REFORM_UI_SKILLSLOT
	case UI_SKILL_SLOT:
		if( NULL != m_pSkillTree && NULL != m_pSkillTree->GetDLGGameSkillSLot() )
			return m_pSkillTree->GetDLGGameSkillSLot()->GetShow();
		break;
#endif //REFORM_UI_SKILLSLOT
	case UI_MENU_GUILD :
		{
			if(g_pData->GetMessenger() != NULL) 
			{
				if( true == g_pData->GetMessenger()->GetOpen() && CX2Community::XMUT_GUILD != g_pData->GetMessenger()->GetOpenTab() ) 
					return false;
				else
					return g_pData->GetMessenger()->GetOpen();
			}
		}
		break;

#ifdef ADDED_RELATIONSHIP_SYSTEM
	case UI_MENU_RELATION:
		{
			if(g_pData->GetMessenger() != NULL) 
			{
				if( true == g_pData->GetMessenger()->GetOpen() && CX2Community::XMUT_RELATION != g_pData->GetMessenger()->GetOpenTab() ) 
					return false;
				else
					return g_pData->GetMessenger()->GetOpen();
			}
		}
		break;
#endif //ADDED_RELATIONSHIP_SYSTEM

#ifdef SERV_HERO_PVP_MANAGE_LIST
	case UI_MENU_HERO:
		{
			if(m_pHeroMatch != NULL) return m_pHeroMatch->GetShow();
		}
		break;
#endif //SERV_HERO_PVP_MANAGE_LIST
#ifdef SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT
	case UI_MENU_WEDDING_EVENT_LETTER:
		{
			if( NULL != m_pDlgWeddingEventLetter )
				return m_pDlgWeddingEventLetter->GetShow();
		}
		break;
#endif //SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT

#ifdef RIDING_SYSTEM
	case UI_RIDING_SKILL_SLOT:
		{
			if( NULL != CX2RidingPetManager::GetInstance()->GetSkillSlot() )
				return CX2RidingPetManager::GetInstance()->GetSkillSlot()->GetShowRidingSkillSlot();
		}
		break;
#endif //RIDING_SYSTEM

	default:
		return false;
	}
    return false;
}


// 예전의 ToggleUI와 같다 : 켜져있으면 끄고 꺼져있으면 켜고
bool CX2UIManager::ToggleUI(UI_MENU menuInx, UidType arg)
{
	return ToggleUI(menuInx, !GetShow(menuInx), arg);
}

// 특정 다이얼로그의 경우 전달 인자가 필요하다 : int로 받게 했음.
// 각 UI의 여닫기가 완료되었을 때 UIOpened / UIClosed를 호출해 줄 것.
// forOpen = true면 켜고, false면 끈다. false path의 경우 반드시 닫히게 만들 것!
// NULL CHECK 조심할 것.
bool CX2UIManager::ToggleUI(UI_MENU menuInx, bool forOpen, UidType arg)
{
	if(false == ToggleEnableCondition(menuInx, forOpen)) return true;

	if(GetShow(menuInx) == forOpen)
	{
		// 열려 있는 창에 열어달라는 요청이 왔으면 최상위로 올리기만 해주자
		// 닫힌 창에 닫아달라고 했으면 그냥 종료
		if(forOpen) 
			SetUILayerIndex(menuInx, true);
		
		return true;
	}

	SetNowDragItemReturn();

	//ToggleEnableCondition에서 다 막혀 있다..
#ifdef ALWAYS_EVENT_ADAMS_UI_SHOP
	int UnitType = (int) CX2LocationManager::HI_EVENT_ADAMS_UI_SHOP;
	if( (int)arg == UnitType )
	{
		m_bShowAdamsShop = true;
	}
	else
	{
		m_bShowAdamsShop = false;
	}
#endif ALWAYS_EVENT_ADAMS_UI_SHOP

    switch(menuInx)
    {
#ifdef SERV_PET_SYSTEM
	case UI_MENU_PET_LIST:
		{
			if(m_pPetList == NULL)
			{
				CreatePetList();
			}
			m_pPetList->SetShow(forOpen, arg);

			//{{ kimhc // 2009-08-06 // 캐릭터별 은행
#ifdef	PRIVATE_BANK
			if ( forOpen == true &&
				GetShow( UI_MENU_PRIVATE_BANK ) == true )	// 은행이 열려있는 경우 은행 닫기
			{
				ToggleUI( UI_MENU_PRIVATE_BANK, false );
			}
#endif	PRIVATE_BANK
			//}} kimhc // 2009-08-06 // 캐릭터별 은행

#ifdef SERV_SKILL_NOTE
			if( forOpen == true && GetShow( UI_SKILL_NOTE ) == true )
			{
				ToggleUI( UI_SKILL_NOTE, false );
			}
#endif
		}
		break;
#endif

#ifdef NEW_SKILL_TREE_UI
	case UI_MENU_SKILL:
		{
			if(m_pSkillTree == NULL)
			{
				CreateUISkillTree();
			}

#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
			if ( forOpen )
				m_pSkillTree->SetShow(forOpen);
			else	/// 닫을 때, 스킬을 찍었는지에 대한 팝업창 생성 여부 처리
			{
				if (	NULL != g_pData &&
						NULL != g_pData->GetSkillTree() &&
						false == g_pData->GetSkillTree()->GetMapSkillInfo().empty() )
				{
					g_pData->GetSkillTree()->ClearMapSkillInfo();
				}
				else
					m_pSkillTree->SetShow(forOpen);
			}
#else // UPGRADE_SKILL_SYSTEM_2013
			m_pSkillTree->SetShow(forOpen);
#endif // UPGRADE_SKILL_SYSTEM_2013

			//{{ kimhc // 2009-08-06 // 캐릭터별 은행
#ifdef	PRIVATE_BANK
			if ( forOpen == true &&
				 GetShow( UI_MENU_PRIVATE_BANK ) == true )	// 은행이 열려있는 경우 은행 닫기
			{
				ToggleUI( UI_MENU_PRIVATE_BANK, false );
			}
#endif	PRIVATE_BANK
			//}} kimhc // 2009-08-06 // 캐릭터별 은행
			
#ifdef SERV_SKILL_NOTE
			if( forOpen == true && GetShow( UI_SKILL_NOTE ) == true )
			{
				ToggleUI( UI_SKILL_NOTE, false );
			}
#endif

		} break;
#endif NEW_SKILL_TREE_UI





#ifdef COMBO_TREE_UI_TEST
	case UI_MENU_COMBO_TREE:
		{
			if( NULL == m_pComboTreeUI )
			{
				CreateUIComboTree();
			}
			m_pComboTreeUI->SetShow( forOpen );

#ifdef	PRIVATE_BANK
			if ( forOpen == true &&
				GetShow( UI_MENU_PRIVATE_BANK ) == true )	// 은행이 열려있는 경우 은행 닫기
			{
				ToggleUI( UI_MENU_PRIVATE_BANK, false );
			}
#endif	PRIVATE_BANK

#ifdef SERV_SKILL_NOTE
			if( forOpen == true && GetShow( UI_SKILL_NOTE ) == true )
			{
				ToggleUI( UI_SKILL_NOTE, false );
			}
#endif

			
		} break;
#endif COMBO_TREE_UI_TEST





		//{{ kimhc	// 실시간 엘소드 중 실시간 아이템 획득 관련 임시 인벤토리
#ifdef	REAL_TIME_ELSWORD
	case UI_TEMP_INVENTORY:
		{
			if ( m_pTempInventory == NULL )
			{
				CreateUITempInventory();
			}
			m_pTempInventory->SetShow( forOpen );

		}
		break;
#endif	REAL_TIME_ELSWORD
		//}} kimhc	// 실시간 엘소드 중 실시간 아이템 획득 관련 임시 인벤토리

#ifdef SERV_SKILL_NOTE
	case UI_SKILL_NOTE:
		{
			if( m_pSkillNote == NULL )
			{
				CreateUISkillNote();
			}
			m_pSkillNote->SetShow( forOpen );
		}
		break;
#endif


#ifdef SERV_EPIC_QUEST
	case UI_MENU_QUEST_NEW:
		{
			if(m_pQuestNew == NULL)
			{
				CreateUIQuestNew();
			}

			if(forOpen)
			{
				if(arg != NULL)
				{
					CloseAllNPCDlg();
					m_pQuestNew->SetNPCID((CX2UnitManager::NPC_UNIT_ID)arg); 
					m_pQuestNew->SetTalkingQuestNPC(true, (CX2UnitManager::NPC_UNIT_ID)arg);
				}
			}
			m_pQuestNew->SetShow(forOpen);

			//{{ kimhc // 2009-08-06 // 캐릭터별 은행
#ifdef	PRIVATE_BANK
			if ( forOpen == true &&
				GetShow( UI_MENU_PRIVATE_BANK ) == true )	// 은행이 열려있는 경우 은행 닫기
			{
				ToggleUI( UI_MENU_PRIVATE_BANK, false );
			}
#endif	PRIVATE_BANK
			//}} kimhc // 2009-08-06 // 캐릭터별 은행

#ifdef SERV_SKILL_NOTE
			if( forOpen == true && GetShow( UI_SKILL_NOTE ) == true )
			{
				ToggleUI( UI_SKILL_NOTE, false );
			}
#endif



		}
		break;	
#else

	case UI_MENU_QUEST:
		{
			if(m_pQuest == NULL)
			{
				CreateUIQuest();
				//m_pQuest = new CX2UIQuest(g_pMain->GetNowState(), NULL);
			}
			m_pQuest->SetShow(forOpen);

			//{{ kimhc // 2009-08-06 // 캐릭터별 은행
#ifdef	PRIVATE_BANK
			if ( forOpen == true &&
				GetShow( UI_MENU_PRIVATE_BANK ) == true )	// 은행이 열려있는 경우 은행 닫기
			{
				ToggleUI( UI_MENU_PRIVATE_BANK, false );
			}
#endif	PRIVATE_BANK
			//}} kimhc // 2009-08-06 // 캐릭터별 은행

#ifdef SERV_SKILL_NOTE
			if( forOpen == true && GetShow( UI_SKILL_NOTE ) == true )
			{
				ToggleUI( UI_SKILL_NOTE, false );
			}
#endif

			
		}
		break;	
	case UI_MENU_QUEST_RECEIVE:
		{
			if(m_pQuestReceive == NULL)
			{
				CreateUIQuestReceive();
				//m_pQuestReceive = new CX2UIQuestReceive(g_pMain->GetNowState(), NULL );
			}

			if(forOpen)
			{
				CloseAllNPCDlg();
				m_pQuestReceive->SetNPCID((CX2UnitManager::NPC_UNIT_ID)arg); 
			}
			m_pQuestReceive->SetShow(forOpen);

#ifndef REFORM_NOVICE_GUIDE
			// 초보자가이드
			if(forOpen == true && g_pTFieldGame->GetNoviceGuide() != NULL && g_pTFieldGame->GetNoviceGuide()->GetIsPlayGuide() == true)
			{
				if( (g_pTFieldGame->GetNoviceGuide()->GetNowGuideStep() == CX2NoviceGuide::NGS_STEP2 && 
						g_pTFieldGame->GetJoinNpcId() == CX2UnitManager::NUI_ARIEL) ||
					(g_pTFieldGame->GetNoviceGuide()->GetNowGuideStep() == CX2NoviceGuide::NGS_STEP3 &&
						g_pTFieldGame->GetJoinNpcId() == CX2UnitManager::NUI_HAGERS) ||
						(g_pTFieldGame->GetNoviceGuide()->GetNowGuideStep() == CX2NoviceGuide::NGS_STEP8 &&
						g_pTFieldGame->GetJoinNpcId() == CX2UnitManager::NUI_HAGERS) )
				{
					g_pTFieldGame->GetNoviceGuide()->SetMouseClickIndex(3);					
				}
			}
#endif //REFORM_NOVICE_GUIDE
			
#ifdef SERV_SKILL_NOTE
			if( forOpen == true && GetShow( UI_SKILL_NOTE ) == true )
			{
				ToggleUI( UI_SKILL_NOTE, false );
			}
#endif

		}
		break;

#endif SERV_EPIC_QUEST
	case UI_MENU_QUICK_SLOT:
		{
			if(m_pQuickSlot == NULL)
			{
				CreateUIQuickSlot();
			}
			m_pQuickSlot->SetShow(forOpen);
			return true;
		}
		break;
	case UI_MENU_PARTY:
		{
			if(g_pMain->GetPartyUI() != NULL)
			{
				g_pMain->GetPartyUI()->OpenPartyDLG(forOpen);
				g_pMain->GetPartyUI()->UpdatePartyDLG();
				// g_pMain->GetPartyUI()->OpenPartyListDLG(forOpen);
				return true;
			}
		}
		break;


	case UI_MENU_MANUFACTURE:
		{
			if(m_pManufacture == NULL)
			{
				CreateUIManufacture();
				//m_pManufacture = new CX2UIManufacture(g_pMain->GetNowState(), NULL);
			}
			if(forOpen)
			{
				CloseAllNPCDlg();

#ifdef SERV_ITEM_ACTION_BY_DBTIME_SETTING // 2012.12.12 lygan_조성욱 // 석근이 작업 리뉴얼 ( DB에서 실시간 값 반영, 교환, 제조 쪽도 적용 )
				Handler_EGS_BUY_UI_SETTING_REQ((int)arg, UTCIT_MANUFACTURE );
#else //SERV_ITEM_ACTION_BY_DBTIME_SETTING
			//{{ kimhc // 2009-11-23 // 제조 아이템 NPC 별로 가능하도록

#ifdef SERV_MANUFACTURE_PERIOD_FIX
				Handler_EGS_MANUFACTURE_PERIOD_SETTING_REQ( (int)arg );
#else SERV_MANUFACTURE_PERIOD_FIX
#ifdef	ADD_HOUSE_ID_TO_MANUFACTURE
				m_pManufacture->SetHouseID( (int)arg );
#endif	ADD_HOUSE_ID_TO_MANUFACTURE
#endif //SERV_MANUFACTURE_PERIOD_FIX


			//}} kimhc // 2009-11-23 // 제조 아이템 NPC 별로 가능하도록
				m_pManufacture->SetShow(true);
#endif //SERV_ITEM_ACTION_BY_DBTIME_SETTING
				return true;
			}
			else
			{
				m_pManufacture->SetShow(false);
				return true;
			}
		}
		break;
	case UI_MENU_SHOP:
		{
			if(forOpen)
			{
				CloseAllNPCDlg();
				if(m_pShop == NULL)
				{
					CreateUIShop();
					//m_pShop = new CX2UIShop(g_pMain->GetNowState(), NULL);
				}
#ifdef SERV_ITEM_ACTION_BY_DBTIME_SETTING // 2012.12.12 lygan_조성욱 // 석근이 작업 리뉴얼 ( DB에서 실시간 값 반영, 교환, 제조 쪽도 적용 )
#ifdef ALWAYS_EVENT_ADAMS_UI_SHOP
				int UnitType = (int) CX2LocationManager::HI_EVENT_ADAMS_UI_SHOP;
				if( (int)arg == UnitType )
				{
					m_pShop->SetHouseID((int)arg);
					m_pShop->SetShow(true);
				}
				else
				{
					Handler_EGS_BUY_UI_SETTING_REQ((int)arg, UTCIT_SHOP );
				}
#else ALWAYS_EVENT_ADAMS_UI_SHOP
				Handler_EGS_BUY_UI_SETTING_REQ((int)arg, UTCIT_SHOP );
#endif ALWAYS_EVENT_ADAMS_UI_SHOP
#else //SERV_ITEM_ACTION_BY_DBTIME_SETTING
				m_pShop->SetHouseID((int)arg);
				m_pShop->SetShow(true);
#endif //SERV_ITEM_ACTION_BY_DBTIME_SETTING
			}
			else
			{
				m_pShop->SetShow(false);
			}			
			return true;
		}
		break;

#ifdef POSTBOX_NOTICE
	case UI_MENU_POSTBOX_NOTICE:
		{
			if ( forOpen )
			{
				if ( m_pPostBoxWarning == NULL )
					CreateUIPostBoxWarning();
				m_pPostBoxWarning->SetShow( true );
				g_pData->GetUIManager()->SetUILayerIndex(CX2UIManager::UI_MENU_POSTBOX_NOTICE, true);
			}
			else
			{
				m_pPostBoxWarning->SetShow( false );
			}
			return true;
		}
		break;
#endif POSTBOX_NOTICE


#ifdef DEF_TRADE_BOARD
	case UI_MENU_PERSONAL_SHOP_BOARD:
		{
			if ( forOpen )
			{
				CloseAllNPCDlg();

				if ( m_pPersonalShopBoard == NULL )
					CreateUIPersonalShopBoard();
				m_pPersonalShopBoard->SetShow( true );
			}
			else
			{
				m_pPersonalShopBoard->SetShow( false );
			}
			return true;
		}
		break;
#endif DEF_TRADE_BOARD

#ifdef GUILD_BOARD
		//{{ oasis907 : 김상윤 [2009.11.24] // 길드 게시판
	case UI_MENU_GUILD_BOARD:
		{
			if ( forOpen )
			{
				CloseAllNPCDlg();

				if ( m_pGuildBoard == NULL )
					CreateUIGuildBoard();
				m_pGuildBoard->SetShow( true );
			}
			else
			{
				m_pGuildBoard->SetShow( false );
			}
			return true;
		}
		break;
		//}} oasis907 : 김상윤 [2009.11.24] // 길드 게시판
#endif GUILD_BOARD

#ifdef SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT
	case UI_MENU_WEDDING_EVENT_LETTER:
		{
			if( forOpen )
			{
				CloseAllNPCDlg();

				bool Is_Ok = true;

				if( m_pDlgWeddingEventLetter == NULL )
					Is_Ok = CreateWeddingEventLetter();

				if( true == Is_Ok )
				{				
					CreateWeddingXmesh(0);
					m_pDlgWeddingEventLetter->SetShow( true );
					g_pData->GetUIManager()->SetUILayerIndex(CX2UIManager::UI_MENU_WEDDING_EVENT_LETTER, true);

					SetShowLetterGuide( true );
				}		
			}
			else
			{
				if( m_pDlgWeddingEventLetter != NULL && m_fWeddingClose == -1.f )
				{
					m_fWeddingClose = 1.f;
					ClearWeaddingXmesh();
					CreateWeddingXmesh(1);
					SAFE_DELETE_DIALOG( m_pDlgWeddingCaution );
					//m_pDlgWeddingEventLetter->SetShow( false );
				}
			}
			return true;
		}
		break;
#endif //SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT


		//{{ kimhc // 2009-07-09 // 헤니르의 시공에 추가되는 Ranking UI 작업
#ifdef	RANKING_INFO_UI
	case UI_MENU_RANKING_INFO:
		{
			if ( forOpen )
			{
				CloseAllNPCDlg();

				if ( m_pRankingInfo == NULL )
					CreateUIRankingInfo();

				//{{ 허상형 : [2009/8/20] //	이벤트 컨트롤 활성화
#ifdef NEW_TERM_EVENT
                m_pRankingInfo->InitUIRankingEventControl( true );
#endif	//	NEW_TERM_EVENT
				//}} 허상형 : [2009/8/20] //	이벤트 컨트롤 활성화

				m_pRankingInfo->SetShow( true );
			}
			else
			{
				m_pRankingInfo->SetShow( false );
			}
			return true;
		}
		break;
#endif	RANKING_INFO_UI
		//}} kimhc // 2009-07-09 // 헤니르의 시공에 추가되는 Ranking UI 작업

		//{{ kimhc // 2009-07-25 // 헤니르의 시공에 추가되는 ITEM_EXCHANGE_SHOP 작업
#ifdef	ITEM_EXCHANGE_SHOP
	case UI_MENU_ITEM_EXCHANGE_SHOP:
		{
			if ( forOpen )
			{
				CloseAllNPCDlg();

				if ( m_pItemExchangeShop == NULL )
					CreateUIItemExchangeShop();

#ifdef SERV_ITEM_ACTION_BY_DBTIME_SETTING // 2012.12.12 lygan_조성욱 // 석근이 작업 리뉴얼 ( DB에서 실시간 값 반영, 교환, 제조 쪽도 적용 )
				Handler_EGS_BUY_UI_SETTING_REQ((int)arg, UTCIT_EXCHANGE_SHOP );
#else //SERV_ITEM_ACTION_BY_DBTIME_SETTING
				m_pItemExchangeShop->SetHouseID( (int)arg );
				m_pItemExchangeShop->SetShow( true );	
#endif //SERV_ITEM_ACTION_BY_DBTIME_SETTING
			}
			else
			{
				m_pItemExchangeShop->SetShow( false );
			}
			return true;			
		}
		break;
#endif	ITEM_EXCHANGE_SHOP
		//}} kimhc // 2009-07-25 // 헤니르의 시공에 추가되는 ITEM_EXCHANGE_SHOP 작업

		//{{ kimhc // 2009-08-04 // 캐릭터별 은행
#ifdef	PRIVATE_BANK
	case UI_MENU_PRIVATE_BANK:
		{
			if ( forOpen )
			{
				CloseAllNPCDlg();

				if ( m_pPrivateBank	== NULL )
					CreateUIPrivateBank();
				
				if ( m_pPrivateBank != NULL )
				{
					// 캐릭터 선택 후 한번만
                    if ( m_pPrivateBank->GetMyInventory() != NULL )
                    {
					    if ( m_pPrivateBank->GetMyInventory()->GetCoutForGetMyBankInfoReq() == 0 )
					    {
						    g_pData->GetServerProtocol()->SendID( EGS_GET_MY_BANK_INFO_REQ );
						    m_pPrivateBank->AccessMyInventory()->SetCountForGetMyBankInfoReq( 1 );
					    }
                    }
					
					m_pPrivateBank->SetShow( true );
					if ( GetShow( UI_MENU_INVEN ) == false )
						ToggleUI( UI_MENU_INVEN, true );

					if ( GetShow( UI_MENU_CHARINFO ) == true )
						ToggleUI( UI_MENU_CHARINFO, false );

#ifdef SERV_EPIC_QUEST
					if ( GetShow( UI_MENU_QUEST_NEW ) == true )
						ToggleUI( UI_MENU_QUEST_NEW, false );
#endif SERV_EPIC_QUEST
					if ( GetShow( UI_MENU_QUEST ) == true )
						ToggleUI( UI_MENU_QUEST, false );

					if ( GetShow( UI_MENU_SKILL ) == true )
						ToggleUI( UI_MENU_SKILL, false );

#ifdef SERV_SKILL_NOTE
					if( GetShow( UI_SKILL_NOTE ) == true )
						ToggleUI( UI_SKILL_NOTE, false );
#endif
				}
			}
			else
			{
				if ( m_pPrivateBank != NULL )
				{
					m_pPrivateBank->SetShow( false );
				}
			}
			return true;
		}
		break;
#endif	PRIVATE_BANK
			//}} kimhc // 2009-08-04 // 캐릭터별 은행

	case UI_MENU_PERSONAL_TRADE:
		{			
			if(forOpen)
			{
				if(arg  == NULL)
					return false;
				if(m_pPersonalTrade == NULL)
				{
					CreateUIPersonalTrade();
				}
				// Uid로 개설 신청을 하자
				CloseAllLUDlg();
				m_pPersonalTrade->Handler_EGS_REQUEST_TRADE_REQ( arg );
			}
			else
			{
				// 닫아버려
                m_pPersonalTrade->BreakTrade();
			}
			return true;
		}
		break;

	case UI_MENU_PERSONAL_SHOP:	// REQ/ACK 스타일. 안에서 Opened/Closed 호출해 줘야 함
		{
			if(m_pPersonalShop == NULL)
			{
				CreateUIPersonalShop();
				//m_pPersonalShop = new CX2UIPersonalShop(g_pMain->GetNowState(), NULL);
			}
			// 열 때
			if(forOpen)
			{
				CloseAllLUDlg();
				if(arg == CX2UIPersonalShop::OPENFORSELL)
					m_pPersonalShop->Handler_EGS_CREATE_PERSONAL_SHOP_REQ();		// 개설
#ifdef DEF_TRADE_BOARD
				//{{ oasis907 : 김상윤 [2010.3.25] // 	
				else if(arg == CX2UIPersonalShop::OPENFROMBOARD)
				{
					m_pPersonalShop->Handler_EGS_JOIN_PERSONAL_SHOP_FROM_BOARD_REQ();
				}
				//}} oasis907 : 김상윤 [2010.3.25] // 
#endif DEF_TRADE_BOARD
#ifdef SERV_PSHOP_AGENCY
				else if(arg == CX2UIPersonalShop::OPENFORAGENCYTRADER)
				{
					m_pPersonalShop->Handler_EGS_CREATE_PSHOP_AGENCY_REQ();
				}
				else if(arg == CX2UIPersonalShop::OPENMYSHOPFROMBOARD)
				{
					m_pPersonalShop->Handler_EGS_JOIN_MY_PSHOP_AGENCY_REQ();
				}
				else if(arg == CX2UIPersonalShop::OPENTAKESHOPAGNECY)
				{
					m_pPersonalShop->Handler_EGS_JOIN_MY_PSHOP_AGENCY_REQ( false );
				}
#endif
				else
					m_pPersonalShop->Handler_EGS_JOIN_PERSONAL_SHOP_REQ(arg);		// 남의 동네 구경
				return true;
			}
			// 닫을 때
			else
			{
                m_pPersonalShop->BreakShop();
				return true;
			}
			break;
		}
	case UI_MENU_CHARINFO:		// 그냥 열고닫기
		{
			if(m_pCharInfo == NULL)
			{
				CreateUICharInfo();
			}

			m_pCharInfo->SetShow(forOpen);
			if(forOpen == true)
			{
				m_pCharInfo->SetTitle();

				//{{ kimhc // 2009-08-06 // 캐릭터별 은행
#ifdef	PRIVATE_BANK
				if ( GetShow( UI_MENU_PRIVATE_BANK ) == true )	// 은행이 열려있는 경우 은행 닫기
				{
					ToggleUI( UI_MENU_PRIVATE_BANK, false );
				}
#endif	PRIVATE_BANK
				//}} kimhc // 2009-08-06 // 캐릭터별 은행
				
#ifdef SERV_SKILL_NOTE
				if( forOpen == true && GetShow( UI_SKILL_NOTE ) == true )
				{
					ToggleUI( UI_SKILL_NOTE, false );
				}
#endif

			}
			else
			{
				g_pData->GetTitleManager()->CloseTitle();
			}

			return true;
		}		
		break;
	case UI_MENU_WATCH:		// 그냥 열고닫기, OpenDialog에서 UIOpened 호출함
		{
			if(forOpen)
			{
#ifdef OTHER_SERVER_GROUP_WATCH
				if( true == IsSameServerGroupByUnitUid(arg) )
				{
					Handler_EGS_WATCH_UNIT_REQ(arg);
				}
				else
				{
					OpenWatchUIByOtherServerGroupUnitUid(arg);
				}		
#else
				Handler_EGS_WATCH_UNIT_REQ(arg);				
#endif //OTHER_SERVER_GROUP_WATCH		
			}
			else
			{
				if(m_pWatch != NULL)
				{
					m_pWatch->SetShow(false);
				}
			}
			return true;
		}
		break;
	case UI_MENU_INVEN:	// 열고닫기
		{
			if(m_pInventory == NULL) 
			{
				CreateUIInventory();
			}
			
			m_pInventory->SetShow(forOpen);
#ifdef SERV_PET_SYSTEM
#ifndef ADD_PET_INVENTORY_BUTTON
			// oasis907 : 김상윤 [2010.10.13] // 인벤토리에 펫 인벤 버튼 생기면서, 기능 삭제
			if(forOpen == true)
			{
				if(PetInventoryOpenCondition() == true)
				{
					m_pInventory->OpenPetInventory();
				}
			}
#endif ADD_PET_INVENTORY_BUTTON
#endif SERV_PET_SYSTEM
			return true;
		}
		break;
#ifdef SERV_HERO_PVP_MANAGE_LIST //영웅대전 UI
	case UI_MENU_HERO:
		{
			if(m_pHeroMatch == NULL)
			{
				CreateUIHeroMatch();
			}
			m_pHeroMatch->SetOpen(forOpen);
		}
		break;
#endif //SERV_HERO_PVP_MANAGE_LIST
	case UI_MENU_COMMUNITY:
		{
			if(g_pData != NULL && g_pData->GetMessenger() != NULL)
			{
				if(g_pData->GetMessenger()->GetHideWindow() == false)
				{
					g_pData->GetMessenger()->SetOpen(forOpen);
				}
				else 
				{
					g_pData->GetMessenger()->SetHideWindow(false);
					g_pData->GetMessenger()->SetOpen(forOpen);
				}
			}
			return true;
		}
		break;
	case UI_MENU_OPTION:
		if(forOpen)
		{
			CloseAll();
		}		
		break;
		//{{ 2011.05.04   임규수 아바타 합성 시스템
#ifdef SERV_SYNTHESIS_AVATAR
	case UI_MENU_SYNTHESIS:
		{
			if(m_pSynthesisItem == NULL)
			{
				CreateUISynthesisItem();
			}
			if(forOpen)
			{
				CloseAllNPCDlg();
				m_pSynthesisItem->SetShow(true);
				return true;
			}
			else
			{
				m_pSynthesisItem->SetShow(false);
				return true;
			}
		}
		break;
#endif SERV_SYNTHESIS_AVATAR
		//}}		
#ifdef REFORM_UI_SKILLSLOT
	case UI_SKILL_SLOT:
		{
			if( NULL != m_pSkillTree && NULL != m_pSkillTree->GetDLGGameSkillSLot() )
			{
#ifdef FIX_OBSERVER_MODE
				/// 옵저버는 스킬 퀵 슬롯 숨겨버리자.
				if( g_pX2Room != NULL && g_pX2Room->GetMySlot() != NULL && g_pX2Room->GetMySlot()->m_bObserver == true )
					forOpen = false;
#endif FIX_OBSERVER_MODE
					
				m_pSkillTree->GetDLGGameSkillSLot()->SetShow( forOpen );
				m_pSkillTree->UpdateSkillEquipedSlotUI();
			}
			return true;
		}
		break;
#endif //REFORM_UI_SKILLSLOT

#ifdef RIDING_SYSTEM
	case UI_RIDING_SKILL_SLOT:
		{
			if( NULL != CX2RidingPetManager::GetInstance()->GetSkillSlot() )
			{
				if( NULL != g_pX2Room && NULL != g_pX2Room->GetMySlot() && true == g_pX2Room->GetMySlot()->m_bObserver )
					forOpen = false;

				CX2RidingPetManager::GetInstance()->GetSkillSlot()->SetShowRidingSkillSlot( forOpen );
			}
			return true;
		}
		break;
#endif //RIDING_SYSTEM
	case UI_MENU_GUILD:
		//길드창과 커뮤니티창을 분리할 예정이므로 버튼 먼저 분리
		//이후에 길드창과 커뮤니티창 분리 되면 손봐줘야됨..
		{
			if( NULL != g_pData && NULL != g_pData->GetMessenger() )
			{
				if( false == g_pData->GetMessenger()->GetHideWindow() )
				{
					if( CX2Community::XMUT_GUILD == g_pData->GetMessenger()->GetOpenTab() )
					{
						g_pData->GetMessenger()->SetOpen(forOpen);
					}
					else
					{
						g_pData->GetMessenger()->SetGuildTab(forOpen);
						g_pData->GetMessenger()->SetOpen(forOpen);
					}
				}
			}
			return true;
		}
		break;

#ifdef ADDED_RELATIONSHIP_SYSTEM
	case UI_MENU_RELATION:
		//길드창과 커뮤니티창을 분리할 예정이므로 버튼 먼저 분리
		//이후에 길드창과 커뮤니티창 분리 되면 손봐줘야됨..
		{
			if( NULL != g_pData && NULL != g_pData->GetMessenger() )
			{
				if( false == g_pData->GetMessenger()->GetHideWindow() )
				{
					if( CX2Community::XMUT_RELATION == g_pData->GetMessenger()->GetOpenTab() )
					{
						g_pData->GetMessenger()->SetOpen(forOpen);
					}
					else
					{
						g_pData->GetMessenger()->SetRelationTab( forOpen );
						g_pData->GetMessenger()->SetOpen(forOpen);
					}
				}
			}
			return true;
		}
		break;
#endif //ADDED_RELATIONSHIP_SYSTEM

	default:
		break;
    }

#ifdef SERV_PET_SYSTEM
	if(forOpen == true &&
		PetInventoryCloseCondition(menuInx) == true)
	{	
		if(m_pInventory->GetUIPetInventory() != NULL)
		{
			m_pInventory->GetUIPetInventory()->SetShow(false);
		}
	}
#ifdef ADD_PET_INVENTORY_BUTTON
	if(m_pInventory != NULL && GetShow(UI_MENU_INVEN) == true)
	{
		m_pInventory->UpdateButtonPetInventory();
	}
#endif ADD_PET_INVENTORY_BUTTON
#endif SERV_PET_SYSTEM


    return false;
}


void CX2UIManager::CloseAll()
{
	// CloseAll()이 필요한 곳은 다른 state(던전 등)으로 넘어갈 때로 가정한다.
	// 엿보기 창 등 광장에서만 필요한 UI는 삭제처리
	for(int i = (int)UI_MENU_UPPER; i<(int)UI_MENU_END; i++ )
		ToggleUI((UI_MENU)i, false);

	// State 이동 시 메모리에 남겨둘 필요가 없는 UI의 경우 그냥 삭제해버린다.
	SAFE_DELETE(m_pWatch);
	SAFE_DELETE(m_pManufacture);
	SAFE_DELETE(m_pShop);
#ifdef DEF_TRADE_BOARD
	SAFE_DELETE( m_pPersonalShopBoard );	
#endif DEF_TRADE_BOARD
#ifdef GUILD_BOARD
	// oasis907 : 김상윤 [2009.11.24] // 길드 게시판
	SAFE_DELETE( m_pGuildBoard );
#endif GUILD_BOARD

	//{{ kimhc // 2009-07-09 // 헤니르의 시공에 추가되는 Ranking UI 작업
#ifdef	RANKING_INFO_UI
	SAFE_DELETE( m_pRankingInfo );
#endif	RANKING_INFO_UI
	//}} kimhc // 2009-07-09 // 헤니르의 시공에 추가되는 Ranking UI 작업

	//{{ kimhc // 2009-07-25 // 헤니르의 시공에 추가되는 ITEM_EXCHANGE_SHOP 작업
#ifdef	ITEM_EXCHANGE_SHOP
	SAFE_DELETE( m_pItemExchangeShop );
#endif	ITEM_EXCHANGE_SHOP
	//}} kimhc // 2009-07-25 // 헤니르의 시공에 추가되는 ITEM_EXCHANGE_SHOP 작업

	//{{ kimhc // 2009-08-04 // 캐릭터별 은행
#ifdef	PRIVATE_BANK
	if ( g_pMain->GetNowStateID() == CX2Main::XS_VILLAGE_MAP )
	{
		ToggleUI( UI_MENU_PRIVATE_BANK, false );
	}
	else
	{
		SAFE_DELETE( m_pPrivateBank );
	}
#endif	PRIVATE_BANK
	//}} kimhc // 2009-08-04 // 캐릭터별 은행
	//{{ 2011.05.04   임규수 아바타 분해 시스템
#ifdef SERV_MULTI_RESOLVE
	SAFE_DELETE(m_pResolveItem);
#endif SERV_MULTI_RESOLVE
	//}}
	//{{ 2011.05.04   임규수 아바타 합성 시스템
#ifdef SERV_SYNTHESIS_AVATAR
	SAFE_DELETE(m_pSynthesisItem);
#endif SERV_SYNTHESIS_AVATAR
	//}}
	SAFE_DELETE(m_pPersonalShop);

	if(m_pPersonalTrade != NULL)
	{
		m_pPersonalTrade->DestoryAllInviteMessage();
	}

	m_LUMenu1st = UI_MENU_END;
	m_LUMenu2nd = UI_MENU_END;
	m_vLayer.clear();

#ifdef SERV_LOCAL_RANKING_SYSTEM
	if( NULL != g_pData->GetProfileManager() )
		g_pData->GetProfileManager()->CloseAll();
#endif //SERV_LOCAL_RANKING_SYSTEM

}

void CX2UIManager::CloseAllLUDlg()
{
	for(int i=(int)UI_MENU_UPPER; i<(int)UI_MENU_LU; i++ )
		ToggleUI((UI_MENU)i, false);

	SAFE_DELETE(m_pWatch);
	m_LUMenu1st = UI_MENU_END;
	m_LUMenu2nd = UI_MENU_END;
	
}

// 해당 UI는 이미 열렸다/닫혔다고 보고, 위치관계만 처리
// 모든 하위 UI는 실제로 열고 닫힐 때 이걸 호출해 줄 것
void CX2UIManager::UIClosed(UI_MENU menuInx)
{
    if(UI_MENU_UPPER < menuInx && menuInx < UI_MENU_LU )
		SetPosLUDialogOnClose(menuInx);
	if(UI_MENU_UPPER < menuInx && menuInx < UI_MENU_LOWEST )
		SetUILayerIndex(menuInx, false);


#ifdef SERV_PET_SYSTEM
#ifdef ADD_PET_INVENTORY_BUTTON
	if(m_pInventory != NULL && GetShow(UI_MENU_INVEN) == true)
	{
		m_pInventory->UpdateButtonPetInventory();
	}
#endif ADD_PET_INVENTORY_BUTTON
#endif SERV_PET_SYSTEM



}

void CX2UIManager::UIOpened(UI_MENU menuInx)
{
	if(UI_MENU_UPPER < menuInx && menuInx < UI_MENU_LU )
		SetPosLUDialogOnOpen(menuInx);
	if(UI_MENU_UPPER < menuInx && menuInx < UI_MENU_LOWEST )
		SetUILayerIndex(menuInx, true);



#ifdef SERV_PET_SYSTEM
#ifdef ADD_PET_INVENTORY_BUTTON
	if(PetInventoryCloseCondition(menuInx) == true)
	{	
		if(m_pInventory->GetUIPetInventory() != NULL)
		{
			m_pInventory->GetUIPetInventory()->SetShow(false);
		}
		m_pInventory->UpdateButtonPetInventory();
	}
#endif ADD_PET_INVENTORY_BUTTON
#endif SERV_PET_SYSTEM

}


// 특정 다이얼로그를 열 때 위치조정, private
void CX2UIManager::SetPosLUDialogOnOpen(UI_MENU openDLG)
{
	if(m_LUMenu1st == UI_MENU_END || false == GetShow(m_LUMenu1st) )
	{
		m_LUMenu1st = openDLG;
	}
	else
	{
		if(m_LUMenu2nd != UI_MENU_END)
		{
			// 좌상 창이 두개가 열려 있는데 하나가 더 열리는 경우. 열려있는 창 중 우선순위가 낮은 쪽을 닫아준다.
			ToggleUI(m_LUMenu2nd, false);
			//m_LUMenu2nd = UI_MENU_END;
		}

		if( m_LUMenu1st <= openDLG )	// 작은 쪽이 우선순위가 높다
		{
			m_LUMenu2nd = openDLG;
		}
		else
		{
			m_LUMenu2nd = m_LUMenu1st;
			m_LUMenu1st = openDLG;
		}
		// 우선순위기 낮은 UI(2번)의 위치를 조정해 준다 : 1번의 사이즈를 받아서 그만큼 밀어낸다
		// 2번이 확실히 열려있고 생성되어 있다고 가정하고 있다.
		D3DXVECTOR2 vec = GetDLGSize(m_LUMenu1st);
		vec.y = 0;
		SetDLGPosition(m_LUMenu2nd, vec);
	}
	//우선순위가 높은 UI의 위치를 조정해 준다
	SetDLGPosition(m_LUMenu1st, D3DXVECTOR2(0.0f, 0.0f) );
}

// 특정 다이얼로그를 닫을 때 위치조정, private
void CX2UIManager::SetPosLUDialogOnClose(UI_MENU closeDLG)
{
	// CloseDLG 자체를 실제로 닫는 것은 ToggleUI에서 처리되었다고 생각하고
	
	// 만일 닫는 게 1번이면 2번과 바꾸고
	if(closeDLG == m_LUMenu1st)
		m_LUMenu1st = m_LUMenu2nd;
	else if(closeDLG != m_LUMenu2nd)		// 1번이 아닌데 2번도 아니면 할 일 없다 제껴
		return;

	// 2번은 없애버리고
	m_LUMenu2nd = UI_MENU_END;
	
	//1번의 위치를 되돌려 보낸다
	if(m_LUMenu1st != UI_MENU_END)
	{
		SetDLGPosition(m_LUMenu1st, D3DXVECTOR2(0.0f, 0.0f));
	}

}

D3DXVECTOR2 CX2UIManager::GetDLGSize(UI_MENU DLG)
{
	// 왼쪽 위에 떠야 하는 다이얼로그에만 필요한 기능이므로 해당 다이얼로그에 관해서만 구현
	
	if(false == GetShow(DLG)) return D3DXVECTOR2(0.0f, 0.0f);
	switch(DLG)
	{
#ifdef SERV_EPIC_QUEST
	case UI_MENU_QUEST_NEW:
		return m_pQuestNew->GetDLGSize();
#else
	case UI_MENU_QUEST:
		return m_pQuest->GetDLGSize();
	case UI_MENU_QUEST_RECEIVE:
		return m_pQuestReceive->GetDLGSize();
#endif SERV_EPIC_QUEST
	case UI_MENU_MANUFACTURE:
		return m_pManufacture->GetDLGSize();
	case UI_MENU_SHOP:
		return m_pShop->GetDLGSize();
#ifdef DEF_TRADE_BOARD
	case UI_MENU_PERSONAL_SHOP_BOARD:
		return m_pPersonalShopBoard->GetDLGSize();
#endif DEF_TRADE_BOARD
#ifdef GUILD_BOARD // oasis907 : 김상윤 [2009.11.24] // 길드 게시판
	case UI_MENU_GUILD_BOARD:
		return m_pGuildBoard->GetDLGSize();
#endif GUILD_BOARD

		//{{ kimhc // 2009-07-09 // 헤니르의 시공에 추가되는 Ranking UI 작업
#ifdef	RANKING_INFO_UI
	case UI_MENU_RANKING_INFO:
		return m_pRankingInfo->GetDLGSize();
#endif	RANKING_INFO_UI
		//}} kimhc // 2009-07-09 // 헤니르의 시공에 추가되는 Ranking UI 작업

		//{{ kimhc // 2009-07-25 // 헤니르의 시공에 추가되는 ITEM_EXCHANGE_SHOP 작업
#ifdef	ITEM_EXCHANGE_SHOP
	case UI_MENU_ITEM_EXCHANGE_SHOP:
		return m_pItemExchangeShop->GetDLGSize();
#endif	ITEM_EXCHANGE_SHOP
		//}} kimhc // 2009-07-25 // 헤니르의 시공에 추가되는 ITEM_EXCHANGE_SHOP 작업

		//{{ kimhc // 2009-08-04 // 캐릭터별 은행
#ifdef	PRIVATE_BANK
	case UI_MENU_PRIVATE_BANK:
		return m_pPrivateBank->GetDLGSize();
#endif	PRIVATE_BANK
			//}} kimhc // 2009-08-04 // 캐릭터별 은행

	case UI_MENU_CHARINFO:
		return m_pCharInfo->GetDLGSize();
	case UI_MENU_WATCH:
		return m_pWatch->GetDLGSize();
	case UI_MENU_PERSONAL_SHOP:
		return m_pPersonalShop->GetDLGSize();
	case UI_MENU_PERSONAL_TRADE:
		return m_pPersonalTrade->GetDLGSize();
#ifdef SERV_PET_SYSTEM
	case UI_MENU_PET_LIST:
		return m_pPetList->GetDLGSize();
#endif
#ifdef NEW_SKILL_TREE_UI
	case UI_MENU_SKILL:
		return m_pSkillTree->GetDLGSize();
#endif


#ifdef COMBO_TREE_UI_TEST
	case UI_MENU_COMBO_TREE:
		{
			if( NULL != m_pComboTreeUI )
			{
				return m_pComboTreeUI->GetDLGSize();
			}
		} break;
#endif COMBO_TREE_UI_TEST

		//{{ 2011.05.04   임규수 아바타 합성 시스템
#ifdef SERV_SYNTHESIS_AVATAR
	case UI_MENU_SYNTHESIS:
		return m_pSynthesisItem->GetDLGSize();
#endif SERV_SYNTHESIS_AVATAR
		//}}
	default:
		break;
	}
	return D3DXVECTOR2(0.0f, 0.0f);
}

void CX2UIManager::SetDLGPosition(UI_MENU DLG, D3DXVECTOR2 vec)
{
	// 왼쪽 위에 떠야 하는 다이얼로그에만 필요한 기능이므로 해당 다이얼로그에 관해서만 구현
	// 의도하지 않은 호출 ( 닫혀 있거나 생성되지 않은 다이얼로그를 부르는 경우 ) 이 혹시 없을지 Assert 해 두고 예외처리
	ASSERT( GetShow(DLG) == true );
	if(GetShow(DLG) == false) return;
	switch(DLG)
	{
#ifdef SERV_PET_SYSTEM
	case UI_MENU_PET_LIST:
		m_pPetList->SetPosition( vec );
		break;
#endif

#ifdef NEW_SKILL_TREE_UI
	case UI_MENU_SKILL:
		m_pSkillTree->SetPosition( vec );
		break;
#endif


#ifdef COMBO_TREE_UI_TEST
	case UI_MENU_COMBO_TREE:
		if( NULL != m_pComboTreeUI )
		{
			m_pComboTreeUI->SetPosition( vec );
		} break;
#endif COMBO_TREE_UI_TEST

#ifdef SERV_EPIC_QUEST
	case UI_MENU_QUEST_NEW:
		m_pQuestNew->SetPosition( vec );
		break;
#else
	case UI_MENU_QUEST:
		m_pQuest->SetPosition( vec );
		break;
	case UI_MENU_QUEST_RECEIVE:
		m_pQuestReceive->SetPosition( vec );
		break;
#endif SERV_EPIC_QUEST
	case UI_MENU_MANUFACTURE:
		m_pManufacture->SetPosition( vec );
		break;
	case UI_MENU_SHOP:
		m_pShop->SetPosition( vec );
		break;
#ifdef DEF_TRADE_BOARD
	case UI_MENU_PERSONAL_SHOP_BOARD:
		m_pPersonalShopBoard->SetPosition( vec );
		break;
#endif DEF_TRADE_BOARD
#ifdef GUILD_BOARD // oasis907 : 김상윤 [2009.11.24] // 길드 게시판
	case UI_MENU_GUILD_BOARD:
		m_pGuildBoard->SetPosition( vec );
		break;
#endif GUILD_BOARD

		//{{ kimhc // 2009-07-09 // 헤니르의 시공에 추가되는 Ranking UI 작업
#ifdef	RANKING_INFO_UI
	case UI_MENU_RANKING_INFO:
		m_pRankingInfo->SetPosition( vec );
		break;
#endif	RANKING_INFO_UI
		//}} kimhc // 2009-07-09 // 헤니르의 시공에 추가되는 Ranking UI 작업

		//{{ kimhc // 2009-07-25 // 헤니르의 시공에 추가되는 ITEM_EXCHANGE_SHOP 작업
#ifdef	ITEM_EXCHANGE_SHOP
		m_pItemExchangeShop->SetPosition( vec );
		break;
#endif	ITEM_EXCHANGE_SHOP
		//}} kimhc // 2009-07-25 // 헤니르의 시공에 추가되는 ITEM_EXCHANGE_SHOP 작업

		//{{ kimhc // 2009-08-04 // 캐릭터별 은행
#ifdef	PRIVATE_BANK
		m_pPrivateBank->SetPosition( vec );
		break;
#endif	PRIVATE_BANK
		//}} kimhc // 2009-08-04 // 캐릭터별 은행

	case UI_MENU_PARTY:
		break;
	case UI_MENU_CHARINFO:
		m_pCharInfo->SetPosition( vec );
		break;
	case UI_MENU_WATCH:
		m_pWatch->SetPosition( vec );
		break;
	case UI_MENU_PERSONAL_SHOP:
		m_pPersonalShop->SetPosition( vec );
		break;
		//{{ 2011.05.04   임규수 아바타 합성 시스템
#ifdef SERV_SYNTHESIS_AVATAR
	case UI_MENU_SYNTHESIS:
		m_pSynthesisItem->SetPosition( vec );
		break;
#endif SERV_SYNTHESIS_AVATAR
		//}}
	default:
		break;
	}
}


// 사실 두개로 분리해도 될 함수를 T/F 받게 해서 하나로 합쳐둔 건 순전히 편의성 때문입니다...
void CX2UIManager::SetUILayerIndex(UI_MENU menuInx, bool bAdd /* true면 추가 or 최상단 설정, false면 삭제 */ )
{
	if( UI_MENU_UPPER > menuInx || menuInx > UI_MENU_LOWEST )
		return;
	// 원래 벡터에서 지워준다.
	for (int i = (int)m_vLayer.size() - 1; i>=0; i--)
	{
		UI_MENU menu = m_vLayer[i];
		if(menu == menuInx)
		{
			// 추가(또는 최상단 설정)을 하려고 하는데 i가 제일 뒤면 현재 최상위인 다이얼로그다 : 재정렬 필요 없음.
			if(i == m_vLayer.size() - 1 && true == bAdd) 
				return;
			// 목표물을 원래 벡터에서 지워준다.
			m_vLayer.erase( m_vLayer.begin() + i );
			break;
		}
	}

	// 여는 경우에만
	if(bAdd)
	{
		// 제일 뒤에 추가
		m_vLayer.push_back(menuInx);
	}

	// 그리고 Sort 해 준다.
	SortUIbyLayerIndex();
	DisableDialogOnCondition();

}



void CX2UIManager::SortUIbyLayerIndex()
{
	// 나중에 좀 더 잘 수정하자..
	X2_DIALOG_LAYER Layer = XDL_NORMAL_3;
	
	if( g_pMain == NULL )
		return;

	// 던전에서 스킬 슬롯창의 Layer가 NORMAL_2라 위로 튀어나오는 현상 때문에 예외처리
	switch(g_pMain->GetNowStateID())
	{
	case CX2Main::XS_DUNGEON_GAME:
	case CX2Main::XS_TRAINING_GAME:
	case CX2Main::XS_PVP_GAME:
	case CX2Main::XS_BATTLE_FIELD:
		{
			//{{ kimhc // 실시간 엘소드 중 실시간 칭호 획득 및 교체
#ifdef	REAL_TIME_ELSWORD
			Layer = XDL_OPTION;
#else	REAL_TIME_ELSWORD
			Layer = XDL_POP_UP;
#endif	REAL_TIME_ELSWORD
			//}} kimhc // 실시간 엘소드 중 실시간 칭호 획득 및 교체
			
		} break;
	default:
		{
			Layer = XDL_NORMAL_3;
		} break;
	}
	// enum이 int인 걸 이용한 땜빵 코드
	for(int i=(int)m_vLayer.size()-1; i>= 0; i--)
	{
		UI_MENU menu = m_vLayer[i];
		switch(menu)
		{
#ifdef SERV_PET_SYSTEM
		case UI_MENU_PET_LIST:
			m_pPetList->SetLayer( Layer );
			break;
#endif
#ifdef NEW_SKILL_TREE_UI
		case UI_MENU_SKILL:
			m_pSkillTree->SetLayer( Layer );
			break;
#endif


#ifdef COMBO_TREE_UI_TEST
		case UI_MENU_COMBO_TREE:
			{
				if( NULL != m_pComboTreeUI )
				{
					m_pComboTreeUI->SetLayer( Layer );
				}
			} break;
#endif COMBO_TREE_UI_TEST



			//{{ kimhc	// 실시간 엘소드 중 실시간 아이템 획득 관련 임시 인벤토리
#ifdef	REAL_TIME_ELSWORD
		case UI_TEMP_INVENTORY:
			m_pTempInventory->SetLayer( Layer );
			break;
#endif	REAL_TIME_ELSWORD
			//}} kimhc	// 실시간 엘소드 중 실시간 아이템 획득 관련 임시 인벤토리


#ifdef SERV_EPIC_QUEST
		case UI_MENU_QUEST_NEW:
			m_pQuestNew->SetLayer( Layer );				
			break;
#else
		case UI_MENU_QUEST:
			m_pQuest->SetLayer( Layer );				
			break;
		case UI_MENU_QUEST_RECEIVE:
			m_pQuestReceive->SetLayer( Layer );
			break;
#endif SERV_EPIC_QUEST
		case UI_MENU_MANUFACTURE:
			m_pManufacture->SetLayer( Layer );
			break;
		case UI_MENU_SHOP:
			m_pShop->SetLayer( Layer );
			break;
#ifdef POSTBOX_NOTICE
		case UI_MENU_POSTBOX_NOTICE:
			g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer( m_pPostBoxWarning, Layer);
			g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pPostBoxWarning, false );
			break;
#endif POSTBOX_NOTICE
#ifdef DEF_TRADE_BOARD
		case UI_MENU_PERSONAL_SHOP_BOARD:
			m_pPersonalShopBoard->SetLayer( Layer );
			break;
#endif DEF_TRADE_BOARD
#ifdef GUILD_BOARD	// oasis907 : 김상윤 [2009.11.24] // 길드 게시판
		case UI_MENU_GUILD_BOARD:
			m_pGuildBoard->SetLayer( Layer );
			break;
#endif GUILD_BOARD

			//{{ kimhc // 2009-07-09 // 헤니르의 시공에 추가되는 Ranking UI 작업
#ifdef	RANKING_INFO_UI
		case UI_MENU_RANKING_INFO:
			m_pRankingInfo->SetLayer( Layer );
			break;
#endif	RANKING_INFO_UI
			//}} kimhc // 2009-07-09 // 헤니르의 시공에 추가되는 Ranking UI 작업

			//{{ kimhc // 2009-07-25 // 헤니르의 시공에 추가되는 ITEM_EXCHANGE_SHOP 작업
#ifdef	ITEM_EXCHANGE_SHOP
		case UI_MENU_ITEM_EXCHANGE_SHOP:
			m_pItemExchangeShop->SetLayer( Layer );
			break;
#endif	ITEM_EXCHANGE_SHOP
			//}} kimhc // 2009-07-25 // 헤니르의 시공에 추가되는 ITEM_EXCHANGE_SHOP 작업

			//{{ kimhc // 2009-08-04 // 캐릭터별 은행
#ifdef	PRIVATE_BANK
		case UI_MENU_PRIVATE_BANK:
			m_pPrivateBank->SetLayer( Layer );
			break;
#endif	PRIVATE_BANK
			//}} kimhc // 2009-08-04 // 캐릭터별 은행

		case UI_MENU_PERSONAL_SHOP:
			m_pPersonalShop->SetLayer( Layer );
			break;
		case UI_MENU_CHARINFO:
			m_pCharInfo->SetLayer( Layer );
			break;
		case UI_MENU_WATCH:
			m_pWatch->SetLayer( Layer );
			break;
#ifdef SERV_SKILL_NOTE
		case UI_SKILL_NOTE:
			m_pSkillNote->SetLayer( Layer );
			break;
#endif
		case UI_MENU_INVEN:
			m_pInventory->SetLayer( Layer );
			break;
		case UI_MENU_PERSONAL_TRADE:
			m_pPersonalTrade->SetLayer( Layer );
			break;		
		case UI_MENU_COMMUNITY:
			{
				if( NULL != g_pData->GetMessenger() )
				{
					g_pData->GetMessenger()->SetLayer( Layer );
				}
			} break;
			//{{ 2011.05.04   임규수 아바타 합성 시스템
#ifdef SERV_SYNTHESIS_AVATAR
		case UI_MENU_SYNTHESIS:
			m_pSynthesisItem->SetLayer( Layer );
			break;
#endif SERV_SYNTHESIS_AVATAR
			//}}			
#ifdef SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT
		case UI_MENU_WEDDING_EVENT_LETTER:
			if( NULL != m_pDlgWeddingEventLetter )
				m_pDlgWeddingEventLetter->SetLayer( Layer );
			break;
#endif //SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT
		default:
			break;
		}
		//if(Layer > XDL_NORMAL_1) Layer = (X2_DIALOG_LAYER)(Layer - 1);
		if(Layer == XDL_NORMAL_3 )
		{
			Layer = XDL_NORMAL_2;
		}
		else if( Layer == XDL_OPTION )
		{
			Layer = XDL_NORMAL_4;
		}

	}
}

// 특정 다이얼로그를 특정 조건일 때 Disable시킨다.
void CX2UIManager::DisableDialogOnCondition()
{

	if(g_pMain != NULL && NULL != g_pMain->GetPartyUI())
	{
		bool bPartyEnable = true;
		for(int i=(int)UI_MENU_UPPER; i<(int)UI_MENU_LOWEST; i++)
		{
			//{{ kimhc // 실시간 엘소드 중 임시 인벤토리
#ifdef	REAL_TIME_ELSWORD
			if(	(UI_MENU)i != UI_TEMP_INVENTORY &&
				(UI_MENU)i != UI_MENU_COMMUNITY &&
				GetShow((UI_MENU)i))
#else	REAL_TIME_ELSWORD
			if(	(UI_MENU)i != UI_MENU_INVEN &&
				(UI_MENU)i != UI_MENU_COMMUNITY &&
				GetShow((UI_MENU)i))
#endif	REAL_TIME_ELSWORD
			//}} kimhc // 실시간 엘소드 중 임시 인벤토리
			{
				bPartyEnable = false;
			}
		}
		g_pMain->GetPartyUI()->EnableDialog( bPartyEnable );
		

	}
	
	
}

// NPC와 대화중일 때 사용하는 다이얼로그를 모두 닫는다.
void CX2UIManager::CloseAllNPCDlg()
{
	ToggleUI(UI_MENU_SHOP, false);

#ifdef DEF_TRADE_BOARD
	ToggleUI( UI_MENU_PERSONAL_SHOP_BOARD, false );
#endif DEF_TRADE_BOARD
#ifdef GUILD_BOARD // oasis907 : 김상윤 [2009.11.24] // 길드 게시판
	ToggleUI( UI_MENU_GUILD_BOARD, false );
#endif GUILD_BOARD

	//{{ kimhc // 2009-07-09 // 헤니르의 시공에 추가되는 Ranking UI 작업
#ifdef	RANKING_INFO_UI
	ToggleUI( UI_MENU_RANKING_INFO, false );
#endif	RANKING_INFO_UI
	//}} kimhc // 2009-07-09 // 헤니르의 시공에 추가되는 Ranking UI 작업

	//{{ kimhc // 2009-07-25 // 헤니르의 시공에 추가되는 ITEM_EXCHANGE_SHOP 작업
#ifdef	ITEM_EXCHANGE_SHOP
	ToggleUI( UI_MENU_ITEM_EXCHANGE_SHOP, false );
#endif	ITEM_EXCHANGE_SHOP
	//}} kimhc // 2009-07-25 // 헤니르의 시공에 추가되는 ITEM_EXCHANGE_SHOP 작업

	//{{ kimhc // 2009-08-04 // 캐릭터별 은행
#ifdef	PRIVATE_BANK
	ToggleUI( UI_MENU_PRIVATE_BANK, false );
	SAFE_DELETE( m_pPrivateBank );
#endif	PRIVATE_BANK
		//}} kimhc // 2009-08-04 // 캐릭터별 은행


	ToggleUI(UI_MENU_MANUFACTURE, false);
#ifdef SERV_EPIC_QUEST
	ToggleUI(UI_MENU_QUEST_NEW, false);
#else
	ToggleUI(UI_MENU_QUEST_RECEIVE, false);
#endif SERV_EPIC_QUEST
	//{{ 2011.03.16   임규수 아바타 분해 시스템
#ifdef SERV_MULTI_RESOLVE
	ToggleUI(UI_MENU_RESOLVE, false);
#endif SERV_MULTI_RESOLVE
	//}}
	//{{ 2011.05.04   임규수 아바타 합성 시스템
#ifdef SERV_SYNTHESIS_AVATAR
	ToggleUI(UI_MENU_SYNTHESIS, false);
#endif SERV_SYNTHESIS_AVATAR
	//}}
#ifdef SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT
	ToggleUI( UI_MENU_WEDDING_EVENT_LETTER, false );
#endif //SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT
}

// NPC와 대화중일때 사용되는 다이얼로그가 혹시 열려있는지 본다.
bool CX2UIManager::GetShowNpcShop()
{
	if( GetShow(UI_MENU_MANUFACTURE) == true ||
		GetShow(UI_MENU_SHOP) == true ||
#ifdef SERV_EPIC_QUEST
		GetShow(UI_MENU_QUEST_NEW) == true 
#else
		GetShow(UI_MENU_QUEST_RECEIVE) == true 
#endif SERV_EPIC_QUEST
#ifdef DEF_TRADE_BOARD
		|| GetShow( UI_MENU_PERSONAL_SHOP_BOARD ) == true
#endif DEF_TRADE_BOARD
#ifdef GUILD_BOARD // oasis907 : 김상윤 [2009.11.24] // 길드 게시판
		|| GetShow( UI_MENU_GUILD_BOARD ) == true
#endif GUILD_BOARD
		//{{ kimhc // 2009-07-09 // 헤니르의 시공에 추가되는 Ranking UI 작업
#ifdef	RANKING_INFO_UI
		|| GetShow( UI_MENU_RANKING_INFO ) == true
#endif	RANKING_INFO_UI
		//}} kimhc // 2009-07-09 // 헤니르의 시공에 추가되는 Ranking UI 작업

		//{{ kimhc // 2009-07-25 // 헤니르의 시공에 추가되는 ITEM_EXCHANGE_SHOP 작업
#ifdef	ITEM_EXCHANGE_SHOP
		|| GetShow( UI_MENU_ITEM_EXCHANGE_SHOP ) == true
#endif	ITEM_EXCHANGE_SHOP
		//}} kimhc // 2009-07-25 // 헤니르의 시공에 추가되는 ITEM_EXCHANGE_SHOP 작업

		//{{ kimhc // 2009-08-04 // 캐릭터별 은행
#ifdef	PRIVATE_BANK
		|| GetShow( UI_MENU_PRIVATE_BANK ) == true
#endif	PRIVATE_BANK
		//}} kimhc // 2009-08-04 // 캐릭터별 은행
		//{{ 2011.03.16   임규수 아바타 분해 시스템
#ifdef SERV_MULTI_RESOLVE
		|| GetShow(UI_MENU_RESOLVE) == true
#endif SERV_MULTI_RESOLVE
		//}}
		//{{ 2011.05.04   임규수 아바타 합성 시스템
#ifdef SERV_SYNTHESIS_AVATAR
		|| GetShow(UI_MENU_SYNTHESIS) == true
#endif SERV_SYNTHESIS_AVATAR
		//}}

		)
		return true;
	
	return false;	
}

// State가 바뀔 때 하는 일
void CX2UIManager::OnStateChange()
{
	// 드래깅 끝날 때의 처리
	SetNowDragItemReturn();
	
	CloseAll();
// 	if (IsStateMenu())
// 		//|| g_pMain->GetNowStateID() == CX2Main::XS_DUNGEON_GAME)
// 	{
//         GetUIQuickSlot()->SetShow(true);
// 	}
// 	else
// 	{
// 		GetUIQuickSlot()->SetShow(false);
// 	}


	if( NULL != GetUIComboTree() )
	{
		GetUIComboTree()->OnStateChange();
	}



	switch(g_pMain->GetNowStateID())
	{
#ifdef ELSWORD_NEW_BEGINNING
	case CX2Main::XS_BEGINNING:
#else
	case CX2Main::XS_SERVER_SELECT:
#endif ELSWORD_NEW_BEGINNING
		{
#ifndef SERV_EPIC_QUEST
			SAFE_DELETE(m_pQuest);
#endif SERV_EPIC_QUEST

#ifdef SERV_PET_SYSTEM
			SAFE_DELETE(m_pPetList);
#endif

#ifdef NEW_SKILL_TREE_UI
			SAFE_DELETE(m_pSkillTree);
#endif NEW_SKILL_TREE_UI


#ifdef NEW_MESSENGER
			if( NULL != g_pData->GetMessenger())
				g_pData->GetMessenger()->ClearChatSessionList();
#endif NEW_MESSENGER
			if( NULL != GetUICharInfo() )
			{
				GetUICharInfo()->DeleteUnitViewerUI();
			}
#ifdef SERV_EPIC_QUEST
			if(m_pQuestNew != NULL)
			{
				m_pQuestNew->SetShowQuickQuestDLG(false);
			}
#endif SERV_EPIC_QUEST

		} break;
	case CX2Main::XS_DUNGEON_GAME:
		{
			if(false == g_pMain->GetIsPlayingTutorial())
			{
#ifdef SERV_EPIC_QUEST
				if(m_pQuestNew != NULL)
				{
					m_pQuestNew->ResetQuestUI(true, true);
				}
#else
				if(m_pQuest != NULL)
				{
					m_pQuest->SetShowQuickQuestDLG(true);
				}
#endif SERV_EPIC_QUEST
			}
		} break;
	case CX2Main::XS_VILLAGE_MAP:
	case CX2Main::XS_SQUARE_GAME:
	case CX2Main::XS_BATTLE_FIELD:
		{
#ifdef SERV_EPIC_QUEST
			if(m_pQuestNew == NULL)
			{
				CreateUIQuestNew();
			}
			m_pQuestNew->ResetQuestUI(true, true);
			m_pQuestNew->SetShowQuickQuestDLG(true);
			m_pQuestNew->UpdateNewQuestNotice();
#else
			if(m_pQuest == NULL)
			{
				CreateUIQuest();
				m_pQuest->UpdateQuestList();
			}
			m_pQuest->SetShowQuickQuestDLG(true);

			if( m_pQuestReceive == NULL )
			{
				ASSERT( !"QuestReceive is NULL" );		// 최초 생성된 후 지워질 일이 없는 class.
				CreateUIQuestReceive();
			}
			m_pQuestReceive->UpdateNewQuestNotice();
#endif SERV_EPIC_QUEST
			g_pMain->GetPostBox()->UpdateUnReadMailNotice();
		} break;
	case CX2Main::XS_TRAINING_SCHOOL:
	case CX2Main::XS_PVP_LOBBY:
		{
			if(NULL != g_pMain->GetPartyUI())
			{
#ifdef SERV_PVP_NEW_SYSTEM
				g_pMain->GetPartyUI()->OpenPartyMenu( false );
#endif
				g_pMain->GetPartyUI()->OpenPartyDLG(false);
			}
			
		} break;
	default:
		{
#ifdef SERV_EPIC_QUEST
			if(m_pQuestNew != NULL)
			{
				m_pQuestNew->SetShowQuickQuestDLG(false);
			}
#else
			if(m_pQuest != NULL)
			{
				m_pQuest->SetShowQuickQuestDLG(false);
			}
#endif SERV_EPIC_QUEST
		} break;

	}

}

// State는 그대로고 필드만 바뀌었을 때 ( 마을이동시 ) 호출되는 것
void CX2UIManager::OnFieldChange()
{
	SortUIbyLayerIndex();		// 열려있던 UI들의 순서가 뒤죽박죽이 되고 슬롯이 위로 튀어나오는 현상 때문에 재정렬을 해준다
}

// false를 return하면 toggle이 작동하지 않는다.
bool CX2UIManager::ToggleEnableCondition(UI_MENU menuInx, bool forOpen)
{


	// 판매중의 / 판매 준비중의 개인상점이 열려 있으면 토글 다 작동 안하게..
	// 인벤토리는 되게
	if (GetShow(UI_MENU_PERSONAL_SHOP) == true && 
#ifdef SERV_PSHOP_AGENCY
		( GetUIPersonalShop()->GetPersonalShopState() == CX2UIPersonalShop::XPSS_SELL || 
		  GetUIPersonalShop()->GetPersonalShopState() == CX2UIPersonalShop::XPSS_WAIT ||
		  GetUIPersonalShop()->GetPersonalShopState() == CX2UIPersonalShop::XPSS_AGENCY_WAIT ) &&
#else
		( GetUIPersonalShop()->GetPersonalShopState() == CX2UIPersonalShop::XPSS_SELL || GetUIPersonalShop()->GetPersonalShopState() == CX2UIPersonalShop::XPSS_WAIT ) &&
#endif
		( menuInx != UI_MENU_PERSONAL_SHOP && menuInx != UI_MENU_INVEN && menuInx != UI_MENU_COMMUNITY ))
	{
		return false;
	}

	if(menuInx == UI_MENU_PARTY)
	{
		// 안전장치
		switch(g_pMain->GetNowStateID())
		{
		case CX2Main::XS_DUNGEON_GAME:
		case CX2Main::XS_PVP_GAME:
		case CX2Main::XS_SQUARE_GAME:
		case CX2Main::XS_PVP_ROOM:
		case CX2Main::XS_PVP_LOBBY:
			if(forOpen)
			{
				return false;
			} break;
		default:
			break;
		}
		// 꺼진 상태에서는 켤 수 있게. 켜진 상태에서는 Toggle 작동하지 않게
		if(forOpen == true)
		{
			switch(g_pMain->GetNowStateID())
			{
			case CX2Main::XS_TRAINING_SCHOOL:
			case CX2Main::XS_PVP_LOBBY:
				return false;
			default:
				break;
			}
		}
		
		if(GetShow(UI_MENU_PARTY))
		{
			if(g_pData->GetPartyManager()->DoIHaveParty())
			{
				return false;
			}
			else if(	true == g_pData->GetLocationManager()->IsDungeonGate( g_pData->GetLocationManager()->GetCurrentVillageID() ) )
			{
				return false;
			}
#ifdef SERV_PVP_NEW_SYSTEM
			else if( g_pData->GetPartyManager()->GetProcessPvpMatch() == true )
			{
				return false;
			}
#endif

		}

		if( NULL != g_pTFieldGame &&
			true == g_pTFieldGame->GetTalkNpc() )
		{
			return false;
		}
	}
	

// 	// 퀵슬롯은 닫히지 않아!
//	UI_MENU_UPPER 위의 UI들(퀵슬롯)은 아예 CloseAll 등의 광역마법-_- 대상에서 제외해둔다
//  현재 퀵슬롯은 statemenu와 생사고락을 함께하고 있음
// 	if( menuInx == UI_MENU_QUICK_SLOT && forOpen == false 
// 		&& IsStateMenu() )
// 	{
// 		return false;
// 	}

	return true;	
}

void CX2UIManager::CreateUICharInfo()
{
	SAFE_DELETE(m_pCharInfo);

	//{{ kimhc // 2011-07-05 // 옵션데이타 수치화 작업
#if defined(SERV_PVP_NEW_SYSTEM)
	#ifdef PVP_SEASON2
	m_pCharInfo = new CX2UICharInfo( g_pMain->GetNowState(),  L"DLG_UI_CHARACTER_NEW_2.lua");
	#else
	m_pCharInfo = new CX2UICharInfo(g_pMain->GetNowState(), L"DLG_UI_CHARACTER_NEW.lua");
	#endif
#else
	m_pCharInfo = new CX2UICharInfo(g_pMain->GetNowState(), L"DLG_UI_CHARACTER.lua");
#endif
	//}} kimhc // 2011-07-05 // 옵션데이타 수치화 작업
}

CX2UICharInfo* CX2UIManager::GetUICharInfo()
{ 	
	return m_pCharInfo;
}

void CX2UIManager::CreateUIInventory()
{
	SAFE_DELETE(m_pInventory);

#ifdef SERV_PET_SYSTEM

	#ifdef	SERV_REFORM_INVENTORY_TEST
		m_pInventory = new CX2UIInventory(g_pMain->GetNowState(), L"DLG_UI_Inventory_reform_Ver2.lua" );
	#else	SERV_REFORM_INVENTORY_TEST
		m_pInventory = new CX2UIInventory(g_pMain->GetNowState(), L"DLG_UI_Inventory_Ver2.lua" );
	#endif	SERV_REFORM_INVENTORY_TEST

#else
	m_pInventory = new CX2UIInventory(g_pMain->GetNowState(), L"DLG_UI_INVENTORY.lua" );
#endif
}

CX2UIInventory* CX2UIManager::GetUIInventory()
{ 
	return m_pInventory; 
}

void CX2UIManager::CreateUIPersonalShop()
{
	SAFE_DELETE(m_pPersonalShop);
	m_pPersonalShop = new CX2UIPersonalShop(g_pMain->GetNowState(), NULL);
}

CX2UIPersonalShop* CX2UIManager::GetUIPersonalShop()
{ 
	return m_pPersonalShop; 
}

#ifdef SERV_UPGRADE_TRADE_SYSTEM // 김태환

void CX2UIManager::DestroyPeronalShopUI()
{
	SAFE_DELETE( m_pPersonalShop );
}

#endif //SERV_UPGRADE_TRADE_SYSTEM

void CX2UIManager::CreateUIPersonalTrade()
{
	SAFE_DELETE(m_pPersonalTrade);

	m_pPersonalTrade = new CX2UIPersonalTrade(g_pMain->GetNowState(), NULL);
}

CX2UIPersonalTrade* CX2UIManager::GetUIPersonalTrade()
{
	return m_pPersonalTrade; 
}


void CX2UIManager::CreateUIShop()
{
	SAFE_DELETE(m_pShop);
	m_pShop = new CX2UIShop(g_pMain->GetNowState(), NULL);
}

CX2UIShop* CX2UIManager::GetUIShop()
{
	return m_pShop; 
}

void CX2UIManager::CreateUIManufacture()
{
	SAFE_DELETE(m_pManufacture);
	m_pManufacture = new CX2UIManufacture(g_pMain->GetNowState(), NULL);
}

CX2UIManufacture* CX2UIManager::GetUIManufacture()
{ 
	return m_pManufacture; 
}

void CX2UIManager::CreateUIQuickSlot()
{
	SAFE_DELETE(m_pQuickSlot);
	m_pQuickSlot = new CX2UIQuickSlot(g_pMain->GetNowState(), NULL);
}

CX2UIQuickSlot* CX2UIManager::GetUIQuickSlot()
{ 
	return m_pQuickSlot; 
}

#ifdef SERV_EPIC_QUEST
CX2UIQuestNew* CX2UIManager::GetUIQuestNew()
{ 
	return m_pQuestNew; 
}
void CX2UIManager::CreateUIQuestNew()
{
	SAFE_DELETE(m_pQuestNew);
	m_pQuestNew = new CX2UIQuestNew(g_pMain->GetNowState(), NULL );
}
#else
CX2UIQuest* CX2UIManager::GetUIQuest()
{ 
	return m_pQuest; 
}
void CX2UIManager::CreateUIQuest()
{
	SAFE_DELETE(m_pQuest);
	m_pQuest = new CX2UIQuest(g_pMain->GetNowState(), NULL );
}


CX2UIQuestReceive* CX2UIManager::GetUIQuestReceive()
{	
	return m_pQuestReceive; 
}

void CX2UIManager::CreateUIQuestReceive()
{	
	SAFE_DELETE(m_pQuestReceive);
	m_pQuestReceive = new CX2UIQuestReceive(g_pMain->GetNowState(), NULL );
}
#endif SERV_EPIC_QUEST

void CX2UIManager::CreateUIWatch()
{
	SAFE_DELETE(m_pWatch);
	
#ifdef SERV_PVP_NEW_SYSTEM
	m_pWatch = new CX2UIWatch(g_pMain->GetNowState(), L"DLG_UI_Watch_Ver1.lua" );
#else
	m_pWatch = new CX2UIWatch(g_pMain->GetNowState(), L"DLG_UI_Watch.lua" );
#endif
	ToggleUI(UI_MENU_WATCH, false);
	
}

CX2UIWatch*	CX2UIManager::GetUIWatch()
{ 
	return m_pWatch; 
}

#ifdef SERV_PET_SYSTEM
void CX2UIManager::CreatePetList()
{
	SAFE_DELETE( m_pPetList );
	m_pPetList = new CX2UIPetInfo( g_pMain->GetNowState() );
	ToggleUI(UI_MENU_PET_LIST, false);
	//펫리스트 UI를 열지 않고 펫 소지 여부를 파악 하기 위해 함수 추가
	m_pPetList->GetPetListReq();
}
CX2UIPetInfo* CX2UIManager::GetUIPetList()
{
	return m_pPetList;
}
#endif

#ifdef NEW_SKILL_TREE_UI
void CX2UIManager::CreateUISkillTree()
{
	SAFE_DELETE(m_pSkillTree);

#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
	m_pSkillTree = new CX2UISkillTreeNew(g_pMain->GetNowState() );
#else // UPGRADE_SKILL_SYSTEM_2013
	m_pSkillTree = new CX2UISkillTree(g_pMain->GetNowState() );
#endif // UPGRADE_SKILL_SYSTEM_2013
	
	ToggleUI(UI_MENU_SKILL, false);
}

#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
CX2UISkillTreeNew* CX2UIManager::GetUISkillTree()
#else // UPGRADE_SKILL_SYSTEM_2013
CX2UISkillTree* CX2UIManager::GetUISkillTree()
#endif // UPGRADE_SKILL_SYSTEM_2013
{
	return m_pSkillTree;
}
#endif



#ifdef COMBO_TREE_UI_TEST
	void CX2UIManager::CreateUIComboTree()
	{
		SAFE_DELETE(m_pComboTreeUI);
		m_pComboTreeUI = new CX2UIComboTree;
		ToggleUI(UI_MENU_COMBO_TREE, false);
	}
	CX2UIComboTree* CX2UIManager::GetUIComboTree()
	{
		return m_pComboTreeUI;
	}
#endif COMBO_TREE_UI_TEST


#ifdef SERV_HERO_PVP_MANAGE_LIST //영웅대전 UI
	void CX2UIManager::CreateUIHeroMatch()
	{
		SAFE_DELETE(m_pHeroMatch);
		m_pHeroMatch = new CX2UIHeroMatch();
		ToggleUI(UI_MENU_HERO, false);
	}
	CX2UIHeroMatch* CX2UIManager::GetUIHeroMatch()
	{
		return m_pHeroMatch;
	}
#endif //SERV_HERO_PVP_MANAGE_LIST



//{{ kimhc	// 실시간 엘소드 중 실시간 아이템 획득 관련 임시 인벤토리
#ifdef	REAL_TIME_ELSWORD
void CX2UIManager::CreateUITempInventory()
{
	SAFE_DELETE( m_pTempInventory );
	m_pTempInventory = new CX2UITempInventory( g_pMain->GetNowState(), L"DLG_Item_Box.lua" );
}

CX2UITempInventory* CX2UIManager::GetUITempInventory() const
{
	return m_pTempInventory;
}

void CX2UIManager::DestoryUITempInventory()
{
	SAFE_DELETE( m_pTempInventory ); // TempInventory의 소멸자에서 SetShow(false) 실행
}
#endif	REAL_TIME_ELSWORD
//}} kimhc	// 실시간 엘소드 중 실시간 아이템 획득 관련 임시 인벤토리

#ifdef POSTBOX_NOTICE
void CX2UIManager::CreateUIPostBoxWarning()
{
	SAFE_DELETE_DIALOG( m_pPostBoxWarning );
	m_pPostBoxWarning = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Postbox_Notice.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pPostBoxWarning );   
	m_pPostBoxWarning->SetDisableUnderWindow(true);
	//m_pPostBoxWarning->SetPos(D3DXVECTOR2(600, 140));

}

CKTDGUIDialog* CX2UIManager::GetUIPostBoxWarning() const
{
	return m_pPostBoxWarning;
}
#endif POSTBOX_NOTICE

#ifdef DEF_TRADE_BOARD
void CX2UIManager::CreateUIPersonalShopBoard()
{
	SAFE_DELETE( m_pPersonalShopBoard );
	m_pPersonalShopBoard = new CX2UIPersonalShopBoard( g_pMain->GetNowState(), L"DLG_Personal_Shop_Board.lua" );
}

CX2UIPersonalShopBoard* CX2UIManager::GetUIPersonalShopBoard() const
{
	return m_pPersonalShopBoard;
}
#endif DEF_TRADE_BOARD

#ifdef GUILD_BOARD //{{ oasis907 : 김상윤 [2009.11.24] // 길드 게시판
void CX2UIManager::CreateUIGuildBoard()
{
	SAFE_DELETE( m_pGuildBoard );
	m_pGuildBoard = new CX2UIGuildBoard( g_pMain->GetNowState(), L"DLG_Guild_Board.lua" );
}

CX2UIGuildBoard* CX2UIManager::GetUIGuildBoard() const
{
	return m_pGuildBoard;
}
#endif GUILD_BOARD //}} oasis907 : 김상윤 [2009.11.24] // 길드 게시판


//{{ kimhc // 2009-07-09 // 헤니르의 시공에 추가되는 Ranking UI 작업
#ifdef	RANKING_INFO_UI
void CX2UIManager::CreateUIRankingInfo()
{
	SAFE_DELETE( m_pRankingInfo );
#ifdef SERV_LOCAL_RANKING_SYSTEM	
	m_pRankingInfo = new CX2UIRankingInfo( g_pMain->GetNowState(), L"DLG_Ranking_Henir_Season.lua" );
#else
	m_pRankingInfo = new CX2UIRankingInfo( g_pMain->GetNowState(), L"DLG_Ranking.lua" );
#endif //SERV_LOCAL_RANKING_SYSTEM
}

CX2UIRankingInfo* CX2UIManager::GetUIRankingInfo() const
{
	return m_pRankingInfo;
}
#endif	RANKING_INFO_UI
//}} kimhc // 2009-07-09 // 헤니르의 시공에 추가되는 Ranking UI 작업


//{{ kimhc // 2009-07-25 // 헤니르의 시공에 추가되는 ITEM_EXCHANGE_SHOP 작업
#ifdef	ITEM_EXCHANGE_SHOP
void CX2UIManager::CreateUIItemExchangeShop()
{
	SAFE_DELETE( m_pItemExchangeShop );
	m_pItemExchangeShop		= new CX2UIItemExchangeShop( g_pMain->GetNowState(), L"DLG_Change_Item.lua" );
}

CX2UIItemExchangeShop* CX2UIManager::GetUIItemExchangeShop() const
{
	return m_pItemExchangeShop;
}
#endif	ITEM_EXCHANGE_SHOP
//}} kimhc // 2009-07-25 // 헤니르의 시공에 추가되는 ITEM_EXCHANGE_SHOP 작업

//{{ kimhc // 2009-08-04 // 캐릭터별 은행
#ifdef	PRIVATE_BANK
void	CX2UIManager::CreateUIPrivateBank()
{
	SAFE_DELETE( m_pPrivateBank );

#ifdef	SERV_SHARING_BANK_TEST
	m_pPrivateBank		= new CX2UIPrivateBank( g_pMain->GetNowState(), L"DLG_Private_Bank_Reform.lua" );
#else	SERV_SHARING_BANK_TEST
	m_pPrivateBank		= new CX2UIPrivateBank( g_pMain->GetNowState(), L"DLG_Private_Bank.lua" );
#endif	SERV_SHARING_BANK_TEST
}

CX2UIPrivateBank*	CX2UIManager::GetUIPrivateBank() const
{
	return m_pPrivateBank;
}
#endif	PRIVATE_BANK
//}} kimhc // 2009-08-04 // 캐릭터별 은행

#ifdef SERV_SKILL_NOTE
void CX2UIManager::CreateUISkillNote()
{
	SAFE_DELETE( m_pSkillNote );
	#ifdef REFORM_SKILL_NOTE_UI
	m_pSkillNote		= new CX2UISkillNote( g_pMain->GetNowState(), L"DLG_Note_Of_Skill_New.lua" );
	#else
	m_pSkillNote		= new CX2UISkillNote( g_pMain->GetNowState(), L"DLG_Note_Of_Skill.lua" );
	#endif // REFORM_SKILL_NOTE_UI
}

CX2UISkillNote*	CX2UIManager::GetUISkillNote() const
{
	return m_pSkillNote;
}
#endif

//{{ 2011.03.16   임규수 아바타 분해 시스템
#ifdef SERV_MULTI_RESOLVE
void CX2UIManager::CreateUIResolveItem()
{
	SAFE_DELETE( m_pResolveItem );
	m_pResolveItem		= new CX2UIResolveItem( g_pMain->GetNowState(), L"DLG_UI_Resolve.lua" );
}

CX2UIResolveItem*	CX2UIManager::GetUIResolveItem() const
{
	return m_pResolveItem;
}
#endif SERV_MULTI_RESOLVE
//}}

//{{ 2011.05.04   임규수 아바타 합성 시스템
#ifdef SERV_SYNTHESIS_AVATAR
void CX2UIManager::CreateUISynthesisItem()
{
	SAFE_DELETE( m_pSynthesisItem );
	m_pSynthesisItem		= new CX2UISynthesisItem( g_pMain->GetNowState(), L"DLG_UI_SynthesisItem.lua" );
}

CX2UISynthesisItem*	CX2UIManager::GetUISynthesisItem() const
{
	return m_pSynthesisItem;
}
#endif SERV_SYNTHESIS_AVATAR
//}}

#ifdef SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT
void CX2UIManager::SetShowLetterGuide( const bool _show )
{
	if( NULL != m_pDlgWeddingEventLetter )
	{
		CKTDGUIStatic* pStatic = static_cast<CKTDGUIStatic*>( m_pDlgWeddingEventLetter->GetControl( L"LetterMessage" ) );
		if( NULL != pStatic )
		{
			pStatic->SetShow( _show );
		}
	}
}

void CX2UIManager::ClearWeaddingXmesh()
{
	if( NULL != g_pData && NULL != g_pData->GetUIMajorXMeshPlayer() )
	{
		if( INVALID_MESH_INSTANCE_HANDLE != m_hMeshHandleWedding )
		{
			g_pData->GetUIMajorXMeshPlayer()->DestroyInstanceHandle(m_hMeshHandleWedding);
			m_hMeshHandleWedding = INVALID_MESH_INSTANCE_HANDLE;

			if( NULL != m_pDlgWeddingEventLetter )
			{
				m_pDlgWeddingEventLetter->SetHasUnit(NULL);
			}
		}
	}
}

void CX2UIManager::CreateWeddingXmesh( int Select_ )
{
	if( INVALID_MESH_INSTANCE_HANDLE == m_hMeshHandleWedding )
	{
		const wstring Name_[] = { L"MarriedSystemEventLatterStart", L"MarriedSystemEventLatterEnd" };
		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst_Wedding = NULL;

		pMeshInst_Wedding = g_pData->GetUIMajorXMeshPlayer()->CreateInstance( NULL,  Name_[Select_].c_str(),  300.f,-100.f,200.f, 0,0,0, 0,0,0 );
		if( NULL != pMeshInst_Wedding )
		{
			m_pDlgWeddingEventLetter->SetHasUnit(pMeshInst_Wedding);
			g_pKTDXApp->GetDGManager()->RemoveObjectChain(pMeshInst_Wedding);	
			m_hMeshHandleWedding = pMeshInst_Wedding->GetHandle();
		}
	}
}

bool CX2UIManager::CreateWeddingEventLetter()
{
	bool Is_Create = false;

	SAFE_DELETE_DIALOG( m_pDlgWeddingEventLetter );
	m_pDlgWeddingEventLetter = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_PPoru_Letter.lua" );
	
	if( NULL != m_pDlgWeddingEventLetter )
	{
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDlgWeddingEventLetter );   
		//m_pDlgWeddingEventLetter->SetDisableUnderWindow(true);
		Is_Create = true;
	}

	return Is_Create;
}

void CX2UIManager::CreateWeddingCaution()
{
	if( NULL != m_pDlgWeddingCaution )
	{
		SAFE_DELETE_DIALOG( m_pDlgWeddingCaution );
	}

	m_pDlgWeddingCaution = g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(300.f, 200.f), GET_STRING( STR_ID_24608 ), g_pMain->GetNowState() );
}

void CX2UIManager::Handler_EGS_SEND_LOVE_LETTER_EVENT_REQ()
{
	if( NULL != m_pDlgWeddingEventLetter )
	{
		CKTDGUIIMEEditBox* pImeEdit = static_cast<CKTDGUIIMEEditBox*>( m_pDlgWeddingEventLetter->GetControl(L"IME_Edit_Message") );

		if( NULL != pImeEdit )
		{
			KEGS_SEND_LOVE_LETTER_EVENT_REQ kPacket;
			kPacket.m_wstrMessage = pImeEdit->GetText();

			g_pData->GetServerProtocol()->SendPacket( EGS_SEND_LOVE_LETTER_EVENT_REQ, kPacket );
			g_pMain->AddServerPacket( EGS_SEND_LOVE_LETTER_EVENT_ACK, 60.f ); 
		}
	}
}

bool CX2UIManager::Handler_EGS_SEND_LOVE_LETTER_EVENT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	bool Is_Ok = false;

	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_SEND_LOVE_LETTER_EVENT_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( true == g_pMain->DeleteServerPacket( EGS_SEND_LOVE_LETTER_EVENT_ACK ) )
	{
		if( true == g_pMain->IsValidPacket( kEvent.m_iOK ) )
		{
			if( NULL != g_pData && NULL != g_pData->GetMyUser() &&
				NULL != g_pData->GetMyUser()->GetSelectUnit() )
			{
				g_pData->GetMyUser()->GetSelectUnit()->AccessInventory().UpdateInventorySlotList( kEvent.m_vecUpdatedInventorySlot );
				g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_ED		= kEvent.m_iED;		

				if( GetUIInventory() != NULL)
				{
					GetUIInventory()->UpdateInventorySlotList( kEvent.m_vecUpdatedInventorySlot );
				}

				if( true == GetShow(CX2UIManager::UI_MENU_CHARINFO) && NULL != GetUICharInfo() )
				{
					g_pData->GetUIManager()->GetUICharInfo()->ResetEDnCashnVP();
				}

				ToggleUI( UI_MENU_WEDDING_EVENT_LETTER, false );
			}

			Is_Ok = true;
		}
	}

	return Is_Ok;
}
#endif //SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT

void CX2UIManager::SetNowDragItemReturn()
{
	CX2SlotManager::CX2Slot** pSlotBeforeDragging = (g_pData->GetSlotBeforeDragging());
	if ( *pSlotBeforeDragging != NULL )
	{
		// 혹시 장비면
		if ( (*pSlotBeforeDragging)->GetSlotType() == CX2SlotManager::CX2Slot::ST_EQUIPPED )
		{
			//{{ 2008.11.13 김태완 : UI 예외처리
			UidType DraggingItemUID = ((CX2SlotItem*)(*pSlotBeforeDragging))->GetItemUID();
			//}}
			if ( g_pData->GetMyUser()->GetSelectUnit()->AddEqip( DraggingItemUID ) == true 
				&& g_pData->GetUIManager()->GetUICharInfo() != NULL )
			{
				g_pData->GetUIManager()->GetUICharInfo()->EquipChanged();						
			}
		}
		(*pSlotBeforeDragging)->ResetPos();
		if ( (*pSlotBeforeDragging)->GetDialog() != NULL )
		{
			(*pSlotBeforeDragging)->GetDialog()->SetColor( D3DXCOLOR(1,1,1,1) );
			g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer( (*pSlotBeforeDragging)->GetDialog(), (*pSlotBeforeDragging)->GetNormalLayer() );
		}

	}
	*pSlotBeforeDragging = NULL;
}

void CX2UIManager::Handler_EGS_WATCH_UNIT_REQ(UidType uid)
{
	KEGS_WATCH_UNIT_REQ kPacket;
	
	kPacket.m_iUnitUID = uid;

	g_pData->GetServerProtocol()->SendPacket( EGS_WATCH_UNIT_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_WATCH_UNIT_ACK );
}

bool CX2UIManager::Handler_EGS_WATCH_UNIT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_WATCH_UNIT_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_WATCH_UNIT_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			if(m_pWatch == NULL)
			{
				CreateUIWatch();
				//m_pWatch = new CX2UIWatch(g_pMain->GetNowState(), L"DLG_UI_Watch.lua" );
			}

			CX2Unit *pTargetUnit = m_pWatch->GetUnitByUid( kEvent.m_kCUnitInfo.m_iUnitUID );
			if(pTargetUnit != NULL)
			{
				CX2Unit::UnitData* pUnitData = &pTargetUnit->AccessUnitData();
				if(pUnitData != NULL)
				{
					pUnitData->m_Win = kEvent.m_kCUnitInfo.m_iWin;
					pUnitData->m_Lose = kEvent.m_kCUnitInfo.m_iLose;
#ifdef FIX_WATCH_UI
					pUnitData->m_iRPoint = kEvent.m_kCUnitInfo.m_iRPoint;
#endif

					m_pWatch->OpenDialog( (UidType)kEvent.m_kCUnitInfo.m_iUnitUID );
				}
				else
				{
					// 현재 위치에서는 유저 정보를 볼수 없으므로 에러 출력
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(270,350), GET_STRING( STR_ID_4851 ), g_pMain->GetNowState() );		
				}
			}
			else
			{
				// 현재 위치에서는 유저 정보를 볼수 없으므로 에러 출력
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(270,350), GET_STRING( STR_ID_4851 ), g_pMain->GetNowState() );		
			}
			
			return true;
		}
		//validpacket에서 실패햇으므로 넷에러로 인한 팝업창이 출력되므로 이 출력창은 주석처리
		//아니면 팝업창이 2개가 출력되게 됨.
		//else 
		//{
		//	// 현재 위치에서는 유저 정보를 볼수 없으므로 에러 출력
		//	g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(270,350), GET_STRING( STR_ID_4851 ), g_pMain->GetNowState() );			
		//}
	}
	return false;
}

#ifdef SERV_PET_SYSTEM
bool CX2UIManager::PetInventoryOpenCondition()
{
	// oasis907 : 김상윤 [2010.9.15] // 인벤 열릴 수 있는 스테이트에서 펫 인벤은 안열려야하는 케이스 예외처리
	switch(g_pMain->GetNowStateID())
	{
	case CX2Main::XS_SQUARE_GAME:
	case CX2Main::XS_PVP_LOBBY:
	case CX2Main::XS_PVP_ROOM:
		{
			return false;
		} break;
	}
	//}}

	for(int i=(int)UI_MENU_UPPER; i<(int)UI_MENU_END; i++)
	{

		if(	GetShow((UI_MENU)i) == true &&
			(UI_MENU)i != UI_MENU_PARTY &&
			(UI_MENU)i != UI_TEMP_INVENTORY &&
			(UI_MENU)i != UI_MENU_INVEN )
		{
			return false;
		}
	}
	return true;
}
bool CX2UIManager::PetInventoryCloseCondition(UI_MENU menuInx)
{
	if( GetShow( UI_MENU_INVEN ) == true &&
		menuInx != UI_MENU_PARTY &&
		menuInx != UI_TEMP_INVENTORY &&
		menuInx != UI_MENU_INVEN )
	{
		return true;
	}
	return false;
}
#ifdef ADD_PET_INVENTORY_BUTTON
bool CX2UIManager::PetInventoryForceOpenCondition()
{
	switch(g_pMain->GetNowStateID())
	{
	case CX2Main::XS_SQUARE_GAME:
	case CX2Main::XS_PVP_LOBBY:
	case CX2Main::XS_PVP_ROOM:
		{
			return false;
		} break;
	}

	for(int i=(int)UI_MENU_UPPER; i<(int)UI_MENU_END; i++)
	{
		if(	GetShow((UI_MENU)i) == true &&
			(UI_MENU)i != UI_MENU_PARTY &&
			(UI_MENU)i != UI_TEMP_INVENTORY &&
			(UI_MENU)i != UI_MENU_INVEN &&
			////// 펫 인벤이 열리면서 끌 수 있는 다이얼로그
			(UI_MENU)i != UI_MENU_CHARINFO &&
			(UI_MENU)i != UI_MENU_PET_LIST &&
			(UI_MENU)i != UI_MENU_SKILL &&
			(UI_MENU)i != UI_MENU_COMBO_TREE &&
			(UI_MENU)i != UI_MENU_QUEST_NEW
#ifdef RIDING_SYSTEM
			&& (UI_MENU)i != UI_RIDING_SKILL_SLOT 
#endif // RIDING_SYSTEM
			)
			////////////////////////////////////
		{
			return false;
		}
	}
	return true;
}

void CX2UIManager::PetInventoryForceCloseOtherDlg()
{	
	/////// 펫 인벤이 열리면서 끌 수 있는 다이얼로그
	ToggleUI( UI_MENU_CHARINFO, false );
	ToggleUI( UI_MENU_PET_LIST, false );
	ToggleUI( UI_MENU_SKILL, false );
	ToggleUI( UI_MENU_COMBO_TREE, false );
	ToggleUI( UI_MENU_QUEST_NEW, false );
	///////////////////////////////////////////

	return;
}

#endif ADD_PET_INVENTORY_BUTTON
#endif SERV_PET_SYSTEM
#endif


void CX2UIManager::SetShowUI(bool bVal, bool bForce/* = false*/)
{
	if( m_bShowUI == bVal && false == bForce )
		return;

	m_bShowUI = bVal;

#ifdef RIDING_SYSTEM
	if( NULL != CX2RidingPetManager::GetInstance() && NULL != CX2RidingPetManager::GetInstance()->GetMyRidingPet() )
	{
		SetShowRidingPetSkillSlot( m_bShowUI );
		SetShowSkillSlot( false );
	}
	else
#endif //RIDING_SYSTEM
	{
		SetShowSkillSlot(m_bShowUI);
		SetShowRidingPetSkillSlot( false );
	}

	SetShowQuickSlot(m_bShowUI);
	SetShowMinimapUI(m_bShowUI);
	SetShowGageUI(m_bShowUI);
	SetShowQucikQuest(m_bShowUI);
	SetShowPlayGuide(m_bShowUI);

#ifdef FIELD_BOSS_RAID
	if( false == g_pData->GetBattleFieldManager().GetIsBossRaidCurrentField() )
#endif // FIELD_BOSS_RAID
	{
		SetShowPartyMenu(m_bShowUI);
	}

#ifdef ADDED_RELATIONSHIP_SYSTEM
	SetShowSkillDesc(m_bShowUI);
#endif // ADDED_RELATIONSHIP_SYSTEM

	switch ( g_pMain->GetNowStateID() )
	{
	case CX2Main::XS_DUNGEON_GAME:
		{
			SetShowMenu(bVal);
			SetShowResurrectionStone(bVal);
			SetShowCinematicUI(bVal);
			SetShowFeverUI(bVal);
			//던전에서는 던전,대전 도구창 무조건 꺼지도록 설정
			SetShowPartyMenu(false); 

			if( NULL != g_pX2Game && g_pX2Game->GetGameType() == CX2Game::GT_DUNGEON )
			{
				CX2DungeonGame* pDungeonGame = static_cast<CX2DungeonGame*>(g_pX2Game);
				if( NULL != pDungeonGame )
					pDungeonGame->SetShowMonsterIndicator(m_bShowUI);

#ifdef SERV_HALLOWEEN_EVENT_2013 // 2013.10.14 / JHKang
				if( NULL != g_pData->GetDungeonRoom() )
				{
					switch( g_pData->GetDungeonRoom()->GetDungeonID() )
					{
						case SEnum::DI_EVENT_HALLOWEEN_DAY:
							{
								g_pData->GetUIManager()->SetShowQuickSlot(false);
							} break;
						default:
							break;
					}
				}
#endif //SERV_HALLOWEEN_EVENT_2013
				if( CX2Dungeon::DM_HENIR_CHALLENGE == g_pData->GetDungeonRoom()->GetDungeonMode() )
					SetShowQuickSlot(false);
			}
		}
		break;
	case CX2Main::XS_BATTLE_FIELD:
		{
			SetShowFeverUI(bVal);
			SetShowResurrectionStone(bVal);
			SetShowCinematicUI(bVal);
			SetShowMenu(bVal);

#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
			CX2BattleFieldGame* pBattleFieldGame = static_cast<CX2BattleFieldGame*>(g_pX2Game);
			if( NULL != pBattleFieldGame )		
				pBattleFieldGame->SetShowMonsterIndicator(m_bShowUI); // 필드도 Indicator 추가
#endif // SERV_BATTLEFIELD_MIDDLE_BOSS

#ifdef FIELD_BOSS_RAID // 보스게이지
			if( false == m_bShowUI )
			{
				if( true == g_pData->GetBattleFieldManager().GetIsBossRaidCurrentField() )
					CX2GageManager::GetInstance()->ShowBossGageUI(false);
			}
#endif // FIELD_BOSS_RAID

			if( NULL != g_pData &&
				NULL != g_pData->GetWorldMissionManager() &&
				NULL != g_pData->GetWorldMissionManager()->GetUIWorldMission() )
			{
				g_pData->GetWorldMissionManager()->GetUIWorldMission()->SetShowTimeDlg( m_bShowUI );
			}
		} break;
#ifdef ADDED_RELATIONSHIP_SYSTEM
	case CX2Main::XS_WEDDING_GAME:
		{
			SetShowMenu(bVal);
			SetShowFeverUI(bVal);
			SetShowPartyMenu(false);
		} break;
#endif // ADDED_RELATIONSHIP_SYSTEM
	default:
		break;
	}
}
#ifdef OTHER_SERVER_GROUP_WATCH
bool CX2UIManager::IsSameServerGroupByUnitUid( UidType UnitUid_ )
{
#ifdef EXTEND_SERVER_GROUP_MASK
	return ( static_cast<int>( g_pMain->ExtractServerGroupID(UnitUid_) ) == g_pInstanceData->GetServerGroupID() );
#else EXTEND_SERVER_GROUP_MASK
	return (static_cast<SERVER_GROUP_ID>(g_pMain->ExtractServerGroupID(UnitUid_)) == g_pInstanceData->GetServerGroupID());
#endif EXTEND_SERVER_GROUP_MASK
}
void CX2UIManager::OpenWatchUIByOtherServerGroupUnitUid(UidType UnitUid_)
{
	bool bIsSuccess = false;
	if(m_pWatch == NULL)
	{
		CreateUIWatch();
	}

	CX2Unit *pTargetUnit = m_pWatch->GetUnitByUid( UnitUid_ );
	if(pTargetUnit != NULL)
	{
		CX2Unit::UnitData* pUnitData = &pTargetUnit->AccessUnitData();
		if(pUnitData != NULL)
		{
			pUnitData->m_Win = -1;
			pUnitData->m_iRPoint = -1;
			m_pWatch->OpenDialog( UnitUid_ );
			bIsSuccess = true;
		}
	}

	if( false == bIsSuccess )
	{
		// 현재 위치에서는 유저 정보를 볼수 없으므로 에러 출력
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(270,350), GET_STRING( STR_ID_4851 ), g_pMain->GetNowState() );		
	}
}
#endif //OTHER_SERVER_GROUP_WATCH

void CX2UIManager::SetShowMinimapUI(bool bVal)
{
	if( NULL != g_pInstanceData->GetMiniMapUI() )
	{
		g_pInstanceData->GetMiniMapUI()->SetShowMiniMap(CX2MiniMapUI::MMT_DUNGEON, bVal);
	}
}
void CX2UIManager::SetShowGageUI(bool bVal)
{
	if( NULL != CX2GageManager::GetInstance() )
	{
		CX2GageManager::GetInstance()->SetShow( bVal );
		
#ifdef BUFF_ICON_UI
		CX2GageManager::GetInstance()->SetShowBuffIcon( bVal );	// 버프 아이콘
#endif // BUFF_ICON_UI

	}
}
void CX2UIManager::SetShowQucikQuest(bool bVal)
{
	if( NULL != GetUIQuestNew() )
	{
		GetUIQuestNew()->SetShowQuickQuestDLG(bVal);
	}
}
void CX2UIManager::SetShowPlayGuide(bool bVal)
{
	if( NULL != g_pData->GetPlayGuide() )
	{
		g_pData->GetPlayGuide()->SetHide(!bVal);
	}
}
void CX2UIManager::SetShowFeverUI(bool bVal)
{	
	if( g_pData->GetPartyManager()->GetMyPartyData()->GetPartyMemberCount() >= 2 )
	{
		if( NULL != g_pMain->GetPartyUI() )
		{
			g_pMain->GetPartyUI()->OpenPartyFeverDLG( bVal );
			g_pMain->GetPartyUI()->UpdatePartyFeverDLG();
		}
	}
}
#ifdef ADDED_RELATIONSHIP_SYSTEM

void CX2UIManager::SetShowSkillDesc(bool bVal)
{
	if( false == bVal )
	{
		if( NULL != GetUISkillTree() && NULL != GetUISkillTree()->GetDLGGameSkillSLot() )
		{
			GetUISkillTree()->ShowSkillDesc(bVal);
			GetUISkillTree()->ShowUIDesc(bVal);
		}
	}
}
#endif // ADDED_RELATIONSHIP_SYSTEM

void CX2UIManager::SetShowCinematicUI(bool bVal, float fTime_ )
{
	if( NULL != g_pX2Game )
	{
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
		if ( fTime_ > 0 )															// fTime_ 을 설정했다면
		{
			CX2Game::CinematicUI * pCinematicUI = g_pX2Game->GetCinematicUI (); // CinematicUI 객체를 받아와서
			if ( NULL != pCinematicUI  &&				
				 true == pCinematicUI->GetIsCinematicUIActive ()					// CinematicUI 가 활성화 상태가 아니라면
				 )		
				 g_pX2Game->SetCinematicUI(bVal, fTime_ );							// CinematicUI 설정
		}
		else
#endif // #ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
			g_pX2Game->SetCinematicUI(!bVal);
	}
}

void CX2UIManager::SetShowResurrectionStone(bool bVal)
{
	if( NULL != g_pX2Game && NULL != g_pX2Game->GetDLGMyScore() )
	{
		g_pX2Game->GetDLGMyScore()->SetShow(bVal);		
		if( NULL != g_pData->GetPicCharGameScore() )		
			g_pData->GetPicCharGameScore()->Clear();
	}
}
void CX2UIManager::SetShowMenu(bool bVal)
{
#ifdef CHECK_INHERIT_STATE_MENU
	if( g_pMain->IsInheritStateMenu() )
#endif //CHECK_INHERIT_STATE_MENU
	{
		if( NULL != static_cast<CX2StateMenu*>( g_pMain->GetNowState() ) )
			static_cast<CX2StateMenu*>( g_pMain->GetNowState() )->SetShowMenu(bVal);
	}
}
void CX2UIManager::SetShowPartyMenu(bool bVal)
{
	if( NULL != g_pMain->GetPartyUI() )
	{
		g_pMain->GetPartyUI()->OpenPartyMenu( bVal );
		if( false == bVal)
			g_pMain->GetPartyUI()->OpenPartyDLG( false );
	}
}

#ifdef RIDING_SYSTEM
void CX2UIManager::SetShowRidingPetSkillSlot(bool val)
{ 
	// UI 모두 끈 상태인데, 스킬 슬롯 UI 켜려고 시도하면 무시하기
	if( false == m_bShowUI &&
		true == val )
		return;

	m_bShowRidingPetSkillSlot = val; 
	
	if( true == m_bShowRidingPetSkillSlot &&
		NULL != m_pSkillTree && NULL != m_pSkillTree->GetDLGGameSkillSLot() )
	{
		m_pSkillTree->ShowSkillDesc(false);
		m_pSkillTree->ShowUIDesc(false);
	}

}
#endif //RIDING_SYSTEM

void CX2UIManager::SetShowSkillSlot(bool val)
{
	// UI가 꺼져있을 때는, 스킬트리가 열려있을 때만 보여주기
	if( true == val )
	{
		if( false == m_bShowUI &&
			NULL != GetUISkillTree() &&
			false == GetUISkillTree()->GetShow() )
		{
			return;
		}
	}

	m_bShowSkillSlot = val;
}

#ifdef SERV_ITEM_ACTION_BY_DBTIME_SETTING // 2012.12.12 lygan_조성욱 // 석근이 작업 리뉴얼 ( DB에서 실시간 값 반영, 교환, 제조 쪽도 적용 )
bool CX2UIManager::Handler_EGS_BUY_UI_SETTING_REQ( int iHouseID, int iTimeControlItemType)
{
	KEGS_BUY_UI_SETTING_REQ kEvent;

	switch( iTimeControlItemType )
	{
	case UTCIT_SHOP:
		m_setBanBuyItem.clear();
		break;
	case UTCIT_MANUFACTURE:
		m_setBanManufactureItem.clear();
		break;
	case UTCIT_EXCHANGE_SHOP:
		m_setBanExchangeItem.clear();
		break;
	}

	kEvent.m_iTimeControlItemType = iTimeControlItemType;
	kEvent.m_iHouseID = iHouseID;

	g_pData->GetServerProtocol()->SendPacket( EGS_BUY_UI_SETTING_REQ, kEvent );
	g_pMain->AddServerPacket( EGS_BUY_UI_SETTING_ACK );

	return true;
}

bool CX2UIManager::Handler_EGS_BUY_UI_SETTING_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_BUY_UI_SETTING_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if ( g_pMain->DeleteServerPacket( EGS_BUY_UI_SETTING_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			switch(kEvent.m_iTimeControlItemType)
			{
			case UTCIT_SHOP:
				{
					if ( m_pShop != NULL )
					{
						m_setBanBuyItem.clear();
						m_setBanBuyItem = kEvent.m_setGetItemOnOff;
						m_pShop->SetHouseID( kEvent.m_iHouseID );
						m_pShop->SetShow(true);
					}
				}
				break;
			case UTCIT_MANUFACTURE:
				{
					if ( m_pManufacture != NULL )
					{
						m_setBanManufactureItem.clear();
						m_setBanManufactureItem = kEvent.m_setGetItemOnOff;
						//{{ kimhc // 2009-11-23 // 제조 아이템 NPC 별로 가능하도록
#ifdef	ADD_HOUSE_ID_TO_MANUFACTURE
						m_pManufacture->SetHouseID( kEvent.m_iHouseID );
#endif	ADD_HOUSE_ID_TO_MANUFACTURE
						//}} kimhc // 2009-11-23 // 제조 아이템 NPC 별로 가능하도록
						m_pManufacture->SetShow(true);
					}
				}
				break;
			case UTCIT_EXCHANGE_SHOP:
				{
					if ( m_pItemExchangeShop != NULL )
					{
						m_setBanExchangeItem.clear();
						m_setBanExchangeItem = kEvent.m_setGetItemOnOff;
						m_pItemExchangeShop->SetHouseID( kEvent.m_iHouseID );
						m_pItemExchangeShop->SetShow( true );	
					}
				}
				break;
			}
		}

		return true;
	}

	return false;
}

bool CX2UIManager::Handler_EGS_GET_TIME_CONTROL_ITME_LIST_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{

	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_GET_TIME_CONTROL_ITME_LIST_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if ( kEvent.m_iOK == 0 )
	{
		if ( m_pShop != NULL )
			m_pShop->SetShow(false);
	
		if ( m_pManufacture != NULL )
			m_pManufacture->SetShow(false);
	
		if ( m_pItemExchangeShop != NULL )
			m_pItemExchangeShop->SetShow( false );	

		if ( g_pTFieldGame != NULL )
		{
			if(g_pTFieldGame->GetJoinNpc() == true)
				g_pTFieldGame->SetShowNpcMessage(false);
		}
	}

	return true;
}

bool CX2UIManager::Handler_EGS_GET_TIME_CONTROL_ITME_TALK_LIST_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{

	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_GET_TIME_CONTROL_ITME_TALK_LIST_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if ( g_pMain->DeleteServerPacket( EGS_GET_TIME_CONTROL_ITME_TALK_LIST_ACK ) == true )
	{
		m_setBanBuyItem.clear();
		m_setBanManufactureItem.clear();
		m_setBanExchangeItem.clear();

		if ( kEvent.m_mapGetItemOnOff.empty() == false )
		{
			std::map<int , std::vector<KPacketGetItemOnOff> >::iterator mit = kEvent.m_mapGetItemOnOff.begin();

			for ( ; mit != kEvent.m_mapGetItemOnOff.end(); ++mit )
			{
				switch( mit->first )
				{
				case UTCIT_SHOP:
					{
						BOOST_TEST_FOREACH(KPacketGetItemOnOff , iShopItemID , mit->second )
						{
							m_setBanBuyItem.insert(iShopItemID.m_iItemID  );
						}
					}

					break;
				case UTCIT_MANUFACTURE:
					{
						BOOST_TEST_FOREACH(KPacketGetItemOnOff , iManufactureItemID , mit->second )
						{
							m_setBanManufactureItem.insert(iManufactureItemID.m_iItemID  );
						}
					}
					break;
				case UTCIT_EXCHANGE_SHOP:
					{
						BOOST_TEST_FOREACH(KPacketGetItemOnOff , iExchangeItemID , mit->second )
						{
							m_setBanExchangeItem.insert(iExchangeItemID.m_iItemID  );
						}
					}

					break;
				}
			}
		}
	}

	return true;
}
#endif //SERV_ITEM_ACTION_BY_DBTIME_SETTING


#ifdef SERV_MANUFACTURE_PERIOD_FIX
bool CX2UIManager::Handler_EGS_MANUFACTURE_PERIOD_SETTING_REQ( int iHouseID )
{
	KEGS_MANUFACTURE_PERIOD_SETTING_REQ kEvent;

	kEvent.m_iHouseID = iHouseID;

	g_pData->GetServerProtocol()->SendPacket( EGS_MANUFACTURE_PERIOD_SETTING_REQ, kEvent );
	g_pMain->AddServerPacket( EGS_MANUFACTURE_PERIOD_SETTING_ACK );

	return true;
}
bool CX2UIManager::Handler_EGS_MANUFACTURE_PERIOD_SETTING_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_MANUFACTURE_PERIOD_SETTING_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if ( g_pMain->DeleteServerPacket( EGS_MANUFACTURE_PERIOD_SETTING_ACK ) == true )
	{
		if ( m_pManufacture != NULL )
		{
			m_pManufacture->SetPeriodGroup( kEvent.m_mapPeriodGroup );
			//{{ kimhc // 2009-11-23 // 제조 아이템 NPC 별로 가능하도록
#ifdef	ADD_HOUSE_ID_TO_MANUFACTURE
			m_pManufacture->SetHouseID( kEvent.m_iHouseID );
#endif	ADD_HOUSE_ID_TO_MANUFACTURE
			//}} kimhc // 2009-11-23 // 제조 아이템 NPC 별로 가능하도록
			m_pManufacture->SetShow(true);
		}
		return true;
	}
	return false;
}
#endif //SERV_MANUFACTURE_PERIOD_FIX