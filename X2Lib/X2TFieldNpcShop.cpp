#include "StdAfx.h"



#include ".\x2TFieldNpcShop.h"

CX2TFieldNpcShop::CX2TFieldNpcShop() :
//{{ kimhc // 2009-08-13 // NULL 초기화 추가
m_pButtonQuest( NULL ),
m_pButtonPvp( NULL ),
m_pButtonShop( NULL ),
m_pButtonMake( NULL ),
m_pButtonTraining( NULL ),
m_pButtonFreeTraining( NULL ),
m_pButtonEnchant( NULL ),
m_pButtonAttribute( NULL ),
m_pButtonBack( NULL ),
m_pButtonOut( NULL ),
m_pButtonPass( NULL ),
m_pButtonWindowPass( NULL ),
m_pButtonNext( NULL ),
m_pButtonAgree( NULL ),
m_pButtonComplete( NULL ),
#ifdef GUILD_BOARD // oasis907 : 김상윤 [2009.11.24] // 길드 게시판
m_pButtonGuild( NULL ),
#endif GUILD_BOARD
//}} kimhc // 2009-08-13 // NULL 초기화 추가
m_pButtonRanking( NULL )
//{{ kimhc // 2009-07-25 // 헤니르의 시공에 추가되는 ITEM_EXCHANGE_SHOP 작업
#ifdef	ITEM_EXCHANGE_SHOP
, m_pButtonExchange( NULL )
#endif	ITEM_EXCHANGE_SHOP
//}} kimhc // 2009-07-25 // 헤니르의 시공에 추가되는 ITEM_EXCHANGE_SHOP 작업

//{{ kimhc // 2009-08-03 // 캐릭터별 은행
#ifdef PRIVATE_BANK
,m_pButtonPrivateBank( NULL )
#endif PRIVATE_BANK
//}} kimhc // 2009-08-03 // 캐릭터별 은행
#ifdef WEB_POINT_EVENT // oasis907 : 김상윤 [2010.2.24] // 
,m_pButtonWebEvent( NULL )
#endif WEB_POINT_EVENT
#ifdef DEF_TRADE_BOARD
,m_pButtonPersonalShop( NULL )
#endif DEF_TRADE_BOARD
#ifdef SERV_PSHOP_AGENCY
,m_pButtonAgencyTraderRegister(NULL)
,m_pButtonAgencyTraderReceive(NULL)
#endif
//{{ 2011.05.04   임규수 아바타 합성 시스템
#ifdef SERV_SYNTHESIS_AVATAR
,m_pButtonSynthesis( NULL )
#endif SERV_SYNTHESIS_AVATAR
//}}
#ifdef SERV_PVP_NEW_SYSTEM
, m_pButtonFormalPvp(NULL)
#endif
#ifdef NPC_EVENT_BUTTON
, m_pButtonEvent1(NULL)
, m_pButtonEvent2(NULL)
, m_pButtonEvent3(NULL)
, m_pButtonEvent4(NULL)
#endif NPC_EVENT_BUTTON
#ifdef SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT
, m_pButtonWeddingEvent( NULL )
#endif //SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
, m_pButtoenExchangeNewItem( NULL)
#endif // SERV_NEW_ITEM_SYSTEM_2013_05
#ifdef LOCAL_RANKING_UPDATE
, m_bShowLocalRankingInfo( false )
#endif //LOCAL_RANKING_UPDATE
{
    //RegisterLuaBind();

	m_pDlgMessage	= NULL;
    
#ifdef SERV_PVP_NEW_SYSTEM
	m_pDlgMessage = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Message_Ver1.lua" );
#else
    m_pDlgMessage = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Message.lua" );
#endif
    g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDlgMessage );    
	//m_pDlgMessage->SetLayer(XDL_NORMAL);	
    m_pDlgMessage->SetShowEnable(false, false); 
	//m_pDlgMessage->SetFront(true);

	m_bShowNpcShop = false;

#ifdef DEF_TRADE_BOARD // oasis907 : 김상윤 [2010.3.30] // 
	m_bEnableButtonPersonalShop = true;
#endif DEF_TRADE_BOARD
	// 버튼 
	m_pButtonQuest = (CKTDGUIButton*) m_pDlgMessage->GetControl( L"quest" );

#ifdef SERV_PVP_NEW_SYSTEM
	m_pButtonFormalPvp = (CKTDGUIButton*)m_pDlgMessage->GetControl(L"formal_pvp");
#endif
	m_pButtonPvp = (CKTDGUIButton*) m_pDlgMessage->GetControl( L"pvp" );
	m_pButtonShop = (CKTDGUIButton*) m_pDlgMessage->GetControl( L"shop" );
	m_pButtonMake = (CKTDGUIButton*) m_pDlgMessage->GetControl( L"making"  );	
	m_pButtonTraining = (CKTDGUIButton*) m_pDlgMessage->GetControl( L"training"  );
	m_pButtonFreeTraining = (CKTDGUIButton*) m_pDlgMessage->GetControl( L"free_training"  );

	m_pButtonEnchant = (CKTDGUIButton*) m_pDlgMessage->GetControl( L"Upgrade" );		
	m_pButtonAttribute = (CKTDGUIButton*) m_pDlgMessage->GetControl( L"Attribute" );		

	m_pButtonBack = (CKTDGUIButton*) m_pDlgMessage->GetControl( L"back" );		
	m_pButtonOut = (CKTDGUIButton*) m_pDlgMessage->GetControl( L"out" );	
	m_pButtonPass = (CKTDGUIButton*) m_pDlgMessage->GetControl( L"pass" );
	m_pButtonWindowPass = (CKTDGUIButton*) m_pDlgMessage->GetControl( L"Quest_Next" );

	m_pButtonNext = (CKTDGUIButton*) m_pDlgMessage->GetControl( L"Next" );		
	m_pButtonAgree = (CKTDGUIButton*) m_pDlgMessage->GetControl( L"Agree" );		
	m_pButtonComplete = (CKTDGUIButton*) m_pDlgMessage->GetControl( L"Complete" );	

#ifdef GUILD_BOARD // oasis907 : 김상윤 [2009.11.24] // 길드 게시판
	m_pButtonGuild = (CKTDGUIButton*) m_pDlgMessage->GetControl( L"Guild_ad" );
#endif GUILD_BOARD

	m_pButtonRanking = (CKTDGUIButton*) m_pDlgMessage->GetControl( L"Ranking" );

	//{{ kimhc // 2009-07-25 // 헤니르의 시공에 추가되는 ITEM_EXCHANGE_SHOP 작업
#ifdef	ITEM_EXCHANGE_SHOP
	m_pButtonExchange			= static_cast< CKTDGUIButton* >( m_pDlgMessage->GetControl( L"Exchange" ) );
#endif	ITEM_EXCHANGE_SHOP
	//}} kimhc // 2009-07-25 // 헤니르의 시공에 추가되는 ITEM_EXCHANGE_SHOP 작업

	//{{ kimhc // 2009-08-03 // 캐릭터별 은행
#ifdef PRIVATE_BANK
	m_pButtonPrivateBank		= static_cast< CKTDGUIButton* >( m_pDlgMessage->GetControl( L"PrivateBank" ) );
#endif PRIVATE_BANK
	//}} kimhc // 2009-08-03 // 캐릭터별 은행
#ifdef WEB_POINT_EVENT // oasis907 : 김상윤 [2010.2.24] // 
	m_pButtonWebEvent = static_cast< CKTDGUIButton* >( m_pDlgMessage->GetControl( L"WebEvent" ) );
#endif WEB_POINT_EVENT
#ifdef DEF_TRADE_BOARD
	m_pButtonPersonalShop = static_cast< CKTDGUIButton* >( m_pDlgMessage->GetControl( L"PersonalShop" ) );
#endif DEF_TRADE_BOARD
#ifdef SERV_PSHOP_AGENCY
	m_pButtonAgencyTraderRegister = static_cast< CKTDGUIButton* >( m_pDlgMessage->GetControl( L"agent" ) );
	m_pButtonAgencyTraderReceive = static_cast< CKTDGUIButton* >( m_pDlgMessage->GetControl( L"solditem" ) );
#endif
	//{{ 2011.05.04   임규수 아바타 합성 시스템
#ifdef SERV_SYNTHESIS_AVATAR
	m_pButtonSynthesis	= static_cast< CKTDGUIButton* >( m_pDlgMessage->GetControl( L"Synthesis" ) );
#endif SERV_SYNTHESIS_AVATAR
	//}}
#ifdef NPC_EVENT_BUTTON
	m_pButtonEvent1 = static_cast< CKTDGUIButton* >( m_pDlgMessage->GetControl( L"Event1" ) );
	m_pButtonEvent2 = static_cast< CKTDGUIButton* >( m_pDlgMessage->GetControl( L"Event2" ) );
	m_pButtonEvent3 = static_cast< CKTDGUIButton* >( m_pDlgMessage->GetControl( L"Event3" ) );
	m_pButtonEvent4 = static_cast< CKTDGUIButton* >( m_pDlgMessage->GetControl( L"Event4" ) );
#endif NPC_EVENT_BUTTON

#ifdef SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT
	m_pButtonWeddingEvent = static_cast< CKTDGUIButton* >( m_pDlgMessage->GetControl( L"Wedding" ) );
#endif //SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT

#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
	m_pButtoenExchangeNewItem = static_cast< CKTDGUIButton* >( m_pDlgMessage->GetControl( L"ExchangeNewItem" ) );
#endif // SERV_NEW_ITEM_SYSTEM_2013_05


	pStaticShopName1 = m_pDlgMessage->GetStatic_LUA("ShopName1");
	pStaticShopName2 = m_pDlgMessage->GetStatic_LUA("ShopName2");
	pStaticShopmessage = m_pDlgMessage->GetStatic_LUA("npcMessage");

	//m_pShop = g_pData->GetUIManager()->GetUIShop();

	m_nEnabledButton = 0;

	m_pDLGNpcName = NULL;

	m_eNowState = NSBT_NONE;
	SetShopType(NSBT_NONE);

	m_pButtonBack->SetShowEnable(false, false);

	m_iNowQuestDescPage = 0;
	m_iMaxQuestDescPage = 0;

	m_iIsPlayGuide = 0;

	// 키보드 액션을 위해.. 사용할 버튼 이름을 죽 넣어 둔다 (순서대로)
#ifdef WEB_POINT_EVENT
	m_vButtonNameList.push_back( L"WebEvent" );	// oasis907 : 김상윤 [2010.2.24] // 
#endif WEB_POINT_EVENT
	m_vButtonNameList.push_back(L"quest");
	m_vButtonNameList.push_back(L"training");
#ifdef SERV_PVP_NEW_SYSTEM
	m_vButtonNameList.push_back(L"formal_pvp");
#endif
	m_vButtonNameList.push_back(L"pvp");
	//{{ 2011.05.04   임규수 아바타 합성 시스템
#ifdef SERV_SYNTHESIS_AVATAR
	m_vButtonNameList.push_back(L"Synthesis");
#endif SERV_SYNTHESIS_AVATAR
	//}}
	m_vButtonNameList.push_back(L"Upgrade");
	m_vButtonNameList.push_back(L"Attribute");
	m_vButtonNameList.push_back(L"making");
	m_vButtonNameList.push_back(L"shop");
	m_vButtonNameList.push_back(L"free_training");
	m_vButtonNameList.push_back(L"Guild_ad");	// oasis907 : 김상윤 [2010.1.29] //  길드 게시판 버튼 추가
#ifdef DEF_TRADE_BOARD
	m_vButtonNameList.push_back( L"PersonalShop" );	// oasis907 : 김상윤 [2010.3.12] //  
#endif DEF_TRADE_BOARD
	//m_vButtonNameList.push_back(L"Ranking");	// 2009-07-29 // kimhc 랭킹 버튼 추가
	m_vButtonNameList.push_back(L"Exchange");	// 2009-07-29 // kimhc 교환 버튼 추가
	//{{ kimhc // 2009-08-03 // 캐릭터별 은행
#ifdef	PRIVATE_BANK
	m_vButtonNameList.push_back( L"PrivateBank" );	// 2009-08-03 // kimhc 개인 은행 버튼 추가
#endif	PRIVATE_BANK
	//}} kimhc // 2009-08-03 // 캐릭터별 은행
	m_vButtonNameList.push_back(L"out");
	m_vButtonNameList.push_back(L"Complete");
#ifdef SERV_PSHOP_AGENCY
	m_vButtonNameList.push_back(L"agent");
	m_vButtonNameList.push_back(L"solditem");
#endif
	m_vButtonNameList.push_back(L"Ranking");	// 2009-07-29 // kimhc 랭킹 버튼 추가

	m_iHouseID	= (int)CX2LocationManager::HI_INVALID;
	m_iNpcID	= (int)CX2UnitManager::NUI_NONE;
}

CX2TFieldNpcShop::~CX2TFieldNpcShop(void)
{    

	SetShow(false);
	m_vecShopType.clear();

    SAFE_DELETE_DIALOG( m_pDlgMessage );
	SAFE_DELETE_DIALOG( m_pDLGNpcName );
	
	//SAFE_DELETE( m_pShop );
}

void CX2TFieldNpcShop::SetStage(CKTDXStage *pStage)
{
	if(m_pDlgMessage != NULL)	
		m_pDlgMessage->SetStage(pStage);	
	if(m_pDLGNpcName != NULL)
		m_pDLGNpcName->SetStage(pStage);
}

void CX2TFieldNpcShop::RegisterLuaBind()
{
    lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(),  "g_pTFieldNpcShop", this );
}

void CX2TFieldNpcShop::SetNpcId(int npcId, wstring houseName) 
{ 
	m_iNpcID = npcId; 

	wstring strShopName = g_pData->GetUnitManager()->GetNPCUnitTemplet( (CX2UnitManager::NPC_UNIT_ID)m_iNpcID )->m_Name;
	wstring strHouseName = houseName;

	SetHouseName(strHouseName);
}

void CX2TFieldNpcShop::SetHouseName(wstring houseName) 
{ 	
    if( m_pDlgMessage != NULL )
    {
	    pStaticShopName1->GetString(0)->msg = houseName;
	    pStaticShopName2->GetString(0)->msg = houseName;	
    }//if
}

bool CX2TFieldNpcShop::GetShopType(NPC_SHOP_BUTTON_TYPE eType)
{
    //{{ seojt // 2009-8-21, 11:38
    // 이미 핸들이 invalid한지를 검사한다. jintaeks on 2009-08-21, 11:42
    if( m_pDlgMessage == NULL )
    {
        /** 'm_pButtonQuest'등은 'm_pDlgMessage'에서 얻어낸 포인터인데,
            'm_pDlgMessage'가 이미 invalid해진 경우의 에러를 막는다.
            
            이 문제에 대한 해결책은 두가지이다.

            1) GetControl()로 control에 대한 포인터를 미리 얻어내어서 사용하지 않고,
                필요할 때 마다 GetContrl()을 호출하여 즉시(on the fly) 사용한다.
            2) CKTDGUIControl*를 CKTDGUIDialog 처럼 모두 handle을 사용하는 interface로 바꾼다.

            - jintaeks on 2009-08-21, 11:42
            */
        return false;
    }//if
    //}} seojt // 2009-8-21, 11:38

	// m_pDlgMessage가 바뀔수 있는 문제를 위해 다시 설정한다.
	m_pButtonQuest = (CKTDGUIButton*) m_pDlgMessage->GetControl( L"quest" );
#ifdef SERV_PVP_NEW_SYSTEM
	m_pButtonFormalPvp = (CKTDGUIButton*) m_pDlgMessage->GetControl(L"formal_pvp");
#endif
	m_pButtonPvp = (CKTDGUIButton*) m_pDlgMessage->GetControl( L"pvp" );
	m_pButtonShop = (CKTDGUIButton*) m_pDlgMessage->GetControl( L"shop" );
	m_pButtonMake = (CKTDGUIButton*) m_pDlgMessage->GetControl( L"making"  );	
	m_pButtonTraining = (CKTDGUIButton*) m_pDlgMessage->GetControl( L"training"  );
	m_pButtonFreeTraining = (CKTDGUIButton*) m_pDlgMessage->GetControl( L"free_training"  );

	m_pButtonEnchant = (CKTDGUIButton*) m_pDlgMessage->GetControl( L"Upgrade" );		
	m_pButtonAttribute = (CKTDGUIButton*) m_pDlgMessage->GetControl( L"Attribute" );		

	m_pButtonBack = (CKTDGUIButton*) m_pDlgMessage->GetControl( L"back" );		
	m_pButtonOut = (CKTDGUIButton*) m_pDlgMessage->GetControl( L"out" );	
	m_pButtonPass = (CKTDGUIButton*) m_pDlgMessage->GetControl( L"pass" );
	m_pButtonWindowPass = (CKTDGUIButton*) m_pDlgMessage->GetControl( L"Quest_Next" );

	m_pButtonNext = (CKTDGUIButton*) m_pDlgMessage->GetControl( L"Next" );		
	m_pButtonAgree = (CKTDGUIButton*) m_pDlgMessage->GetControl( L"Agree" );		
	m_pButtonComplete = (CKTDGUIButton*) m_pDlgMessage->GetControl( L"Complete" );		

#ifdef DEF_TRADE_BOARD
	m_pButtonPersonalShop = (CKTDGUIButton*) m_pDlgMessage->GetControl( L"PersonalShop" );
#endif DEF_TRADE_BOARD
#ifdef GUILD_BOARD // oasis907 : 김상윤 [2009.11.24] // 길드 게시판
	m_pButtonGuild = (CKTDGUIButton*) m_pDlgMessage->GetControl( L"Guild_ad" );
#endif GUILD_BOARD

	m_pButtonRanking = (CKTDGUIButton*) m_pDlgMessage->GetControl( L"Ranking" );

	//{{ kimhc // 2009-07-25 // 헤니르의 시공에 추가되는 ITEM_EXCHANGE_SHOP 작업
#ifdef	ITEM_EXCHANGE_SHOP
	m_pButtonExchange			= static_cast< CKTDGUIButton* >( m_pDlgMessage->GetControl( L"Exchange" ) );
#endif	ITEM_EXCHANGE_SHOP
	//}} kimhc // 2009-07-25 // 헤니르의 시공에 추가되는 ITEM_EXCHANGE_SHOP 작업

	//{{ kimhc // 2009-08-03 // 캐릭터별 은행
#ifdef PRIVATE_BANK
	m_pButtonPrivateBank		= static_cast< CKTDGUIButton* >( m_pDlgMessage->GetControl( L"PrivateBank" ) );
#endif PRIVATE_BANK
	//}} kimhc // 2009-08-03 // 캐릭터별 은행

#ifdef SERV_PSHOP_AGENCY
	m_pButtonAgencyTraderRegister = static_cast< CKTDGUIButton* >( m_pDlgMessage->GetControl( L"agent" ) );
	m_pButtonAgencyTraderReceive = static_cast< CKTDGUIButton* >( m_pDlgMessage->GetControl( L"solditem" ) );
#endif

	//{{ 2011.05.04   임규수 아바타 합성 시스템
#ifdef SERV_SYNTHESIS_AVATAR
	m_pButtonSynthesis	= static_cast< CKTDGUIButton* >( m_pDlgMessage->GetControl( L"Synthesis" ) );
#endif SERV_SYNTHESIS_AVATAR
	//}}
#ifdef NPC_EVENT_BUTTON
	m_pButtonEvent1 = static_cast< CKTDGUIButton* >( m_pDlgMessage->GetControl( L"Event1" ) );
	m_pButtonEvent2 = static_cast< CKTDGUIButton* >( m_pDlgMessage->GetControl( L"Event2" ) );
	m_pButtonEvent3 = static_cast< CKTDGUIButton* >( m_pDlgMessage->GetControl( L"Event3" ) );
	m_pButtonEvent4 = static_cast< CKTDGUIButton* >( m_pDlgMessage->GetControl( L"Event4" ) );
#endif NPC_EVENT_BUTTON

#ifdef SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT
	m_pButtonWeddingEvent = static_cast< CKTDGUIButton* >( m_pDlgMessage->GetControl( L"Wedding" ) );
#endif //SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT

#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
	m_pButtoenExchangeNewItem = static_cast< CKTDGUIButton* >( m_pDlgMessage->GetControl( L"ExchangeNewItem" ) );
#endif // SERV_NEW_ITEM_SYSTEM_2013_05

	pStaticShopName1 = m_pDlgMessage->GetStatic_LUA("ShopName1");
	pStaticShopName2 = m_pDlgMessage->GetStatic_LUA("ShopName2");
	pStaticShopmessage = m_pDlgMessage->GetStatic_LUA("npcMessage");


	switch(eType)
	{	
	case NSBT_QUEST:
		{
			if( NULL != m_pButtonQuest )
				return m_pButtonQuest->GetShow();
		} break;
	case NSBT_SHOP:
		{
			if( NULL != m_pButtonShop )
				return m_pButtonShop->GetShow();
		} break;
	case NSBT_PVP:
		{
#ifdef SERV_PVP_NEW_SYSTEM
			if( m_pButtonPvp != NULL && m_pButtonFormalPvp != NULL )
				return ( m_pButtonPvp->GetShow() && m_pButtonFormalPvp->GetShow() );
#else
			if( NULL != m_pButtonPvp )
				return m_pButtonPvp->GetShow();
#endif
		} break;
	case NSBT_ENCHANT:
		{
			if( NULL != m_pButtonEnchant )
				return m_pButtonEnchant->GetShow();
		} break;
	case NSBT_ATTRIBUTE:
		{
			if( NULL != m_pButtonAttribute )
				return m_pButtonAttribute->GetShow();
		} break;
	case NSBT_MAKE:
		{
			if( NULL != m_pButtonMake )
				return m_pButtonMake->GetShow();
		} break;
	case NSBT_FREETRAIN:
		{
			if( NULL != m_pButtonFreeTraining )
				return m_pButtonFreeTraining->GetShow();
		} break;
	case NSBT_TRAINING:
		{
			if( NULL != m_pButtonTraining )
				return m_pButtonTraining->GetShow();
		} break;
	case NSBT_CLOSE:
		{
			if( NULL != m_pButtonOut )
				return m_pButtonOut->GetShow();
		} break;

#ifdef GUILD_BOARD// oasis907 : 김상윤 [2009.11.24] // 길드 게시판
	case NSBT_GUILD:
		{
			if( NULL != m_pButtonGuild )
				return m_pButtonGuild->GetShow();
		} break;
#endif GUILD_BOARD

	case NSBT_RANKING:
		{
			if( NULL != m_pButtonRanking )
				return m_pButtonRanking->GetShow();
		} break;

		//{{ kimhc // 2009-07-25 // 헤니르의 시공에 추가되는 ITEM_EXCHANGE_SHOP 작업
#ifdef	ITEM_EXCHANGE_SHOP
	case NSBT_EXCHANGE:
		{
			if ( NULL != m_pButtonExchange )
				return m_pButtonExchange->GetShow();
		} break;
#endif	ITEM_EXCHANGE_SHOP
		//}} kimhc // 2009-07-25 // 헤니르의 시공에 추가되는 ITEM_EXCHANGE_SHOP 작업

		//{{ kimhc // 2009-08-03 // 캐릭터별 은행
#ifdef	PRIVATE_BANK
	case NSBT_PRIVATE_BANK:
		{
			if ( NULL != m_pButtonPrivateBank )
				return m_pButtonPrivateBank->GetShow();
		} break;
#endif	PRIVATE_BANK
		//}} kimhc // 2009-08-03 // 캐릭터별 은행

#ifdef WEB_POINT_EVENT // oasis907 : 김상윤 [2010.2.24] // 
	case NSBT_WEB_EVENT:
		{
			if ( NULL != m_pButtonWebEvent )
				return m_pButtonWebEvent->GetShow();
		} break;
#endif WEB_POINT_EVENT
#ifdef DEF_TRADE_BOARD
	case NSBT_PERSONAL_SHOP:
		{
			if ( NULL != m_pButtonPersonalShop )
				return m_pButtonPersonalShop->GetShow();
		} break;
#endif DEF_TRADE_BOARD

#ifdef SERV_PSHOP_AGENCY
	case NSBT_AGENCY_TRADER_REGISTER:
		{
			if( m_pButtonAgencyTraderRegister != NULL )
				return m_pButtonAgencyTraderRegister->GetShow();
		}
		break;
	case NSBT_AGENCY_TRADER_RECEIVE:
		{
			if( m_pButtonAgencyTraderReceive != NULL )
				return m_pButtonAgencyTraderReceive->GetShow();
		}
		break;
#endif

		//{{ 2011.05.04   임규수 아바타 합성 시스템
#ifdef SERV_SYNTHESIS_AVATAR
	case NSBT_SYNTHESIS:
		{
			if( NULL != m_pButtonSynthesis )
				return m_pButtonSynthesis->GetShow();
		} break;
		
#endif SERV_SYNTHESIS_AVATAR
		//}}
#ifdef NPC_EVENT_BUTTON
	case NSBT_EVENT1:
		{
			if( NULL != m_pButtonEvent1 )
				return m_pButtonEvent1->GetShow();
		} break;
	case NSBT_EVENT2:
		{
			if( NULL != m_pButtonEvent2 )
				return m_pButtonEvent2->GetShow();
		} break;
	case NSBT_EVENT3:
		{
			if( NULL != m_pButtonEvent3 )
				return m_pButtonEvent3->GetShow();
		} break;
	case NSBT_EVENT4:
		{
			if( NULL != m_pButtonEvent4 )
				return m_pButtonEvent4->GetShow();
		} break;
#endif NPC_EVENT_BUTTON

#ifdef SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT
	case NSBT_WEDDING_EVENT:
		{
			if( NULL != m_pButtonWeddingEvent )
				return m_pButtonWeddingEvent->GetShow();
		}
		break;
#endif //SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
	case NSBT_EXCHANGE_NEW_ITEM:
		{
			if( NULL != m_pButtoenExchangeNewItem )
				return m_pButtoenExchangeNewItem->GetShow();
		}
		break;
#endif // SERV_NEW_ITEM_SYSTEM_2013_05
	}

	return false;
}

void CX2TFieldNpcShop::SetShopType(NPC_SHOP_BUTTON_TYPE eType, bool bShow)
{
    //{{ seojt // 2009-8-21, 14:27
    if( m_pDlgMessage == NULL )
    {
        return;
    }//if
    //}} seojt // 2009-8-21, 14:27

	// m_pDlgMessage가 바뀔수 있는 문제를 위해 다시 설정한다.
	m_pButtonQuest = (CKTDGUIButton*) m_pDlgMessage->GetControl( L"quest" );
#ifdef SERV_PVP_NEW_SYSTEM
	m_pButtonFormalPvp = (CKTDGUIButton*) m_pDlgMessage->GetControl( L"formal_pvp" );
#endif
	m_pButtonPvp = (CKTDGUIButton*) m_pDlgMessage->GetControl( L"pvp" );
	m_pButtonShop = (CKTDGUIButton*) m_pDlgMessage->GetControl( L"shop" );
	m_pButtonMake = (CKTDGUIButton*) m_pDlgMessage->GetControl( L"making"  );	
	m_pButtonTraining = (CKTDGUIButton*) m_pDlgMessage->GetControl( L"training"  );
	m_pButtonFreeTraining = (CKTDGUIButton*) m_pDlgMessage->GetControl( L"free_training"  );

	m_pButtonEnchant = (CKTDGUIButton*) m_pDlgMessage->GetControl( L"Upgrade" );		
	m_pButtonAttribute = (CKTDGUIButton*) m_pDlgMessage->GetControl( L"Attribute" );		

	m_pButtonBack = (CKTDGUIButton*) m_pDlgMessage->GetControl( L"back" );		
	m_pButtonOut = (CKTDGUIButton*) m_pDlgMessage->GetControl( L"out" );	
	m_pButtonPass = (CKTDGUIButton*) m_pDlgMessage->GetControl( L"pass" );
	m_pButtonWindowPass = (CKTDGUIButton*) m_pDlgMessage->GetControl( L"Quest_Next" );

	m_pButtonNext = (CKTDGUIButton*) m_pDlgMessage->GetControl( L"Next" );		
	m_pButtonAgree = (CKTDGUIButton*) m_pDlgMessage->GetControl( L"Agree" );		
	m_pButtonComplete = (CKTDGUIButton*) m_pDlgMessage->GetControl( L"Complete" );		

#ifdef GUILD_BOARD // oasis907 : 김상윤 [2009.11.24] // 길드 게시판
	m_pButtonGuild = (CKTDGUIButton*) m_pDlgMessage->GetControl( L"Guild_ad" ); 
#endif GUILD_BOARD

	m_pButtonRanking = (CKTDGUIButton*) m_pDlgMessage->GetControl( L"Ranking" );

	//{{ kimhc // 2009-07-25 // 헤니르의 시공에 추가되는 ITEM_EXCHANGE_SHOP 작업
#ifdef	ITEM_EXCHANGE_SHOP
	m_pButtonExchange			= static_cast< CKTDGUIButton* >( m_pDlgMessage->GetControl( L"Exchange" ) );
#endif	ITEM_EXCHANGE_SHOP
	//}} kimhc // 2009-07-25 // 헤니르의 시공에 추가되는 ITEM_EXCHANGE_SHOP 작업

	//{{ kimhc // 2009-08-03 // 캐릭터별 은행
#ifdef PRIVATE_BANK
	m_pButtonPrivateBank		= static_cast< CKTDGUIButton* >( m_pDlgMessage->GetControl( L"PrivateBank" ) );
#endif PRIVATE_BANK
	//}} kimhc // 2009-08-03 // 캐릭터별 은행
#ifdef WEB_POINT_EVENT // oasis907 : 김상윤 [2010.2.24] // 
	m_pButtonWebEvent		= static_cast< CKTDGUIButton* >( m_pDlgMessage->GetControl( L"WebEvent" ) );
#endif WEB_POINT_EVENT
#ifdef DEF_TRADE_BOARD
	m_pButtonPersonalShop	= static_cast< CKTDGUIButton* >( m_pDlgMessage->GetControl( L"PersonalShop" ) );
#endif DEF_TRADE_BOARD
#ifdef SERV_PSHOP_AGENCY
	m_pButtonAgencyTraderRegister = static_cast< CKTDGUIButton* >( m_pDlgMessage->GetControl( L"agent" ) );
	m_pButtonAgencyTraderReceive = static_cast< CKTDGUIButton* >( m_pDlgMessage->GetControl( L"solditem" ) );
#endif
	//{{ 2011.05.04   임규수 아바타 합성 시스템
#ifdef SERV_SYNTHESIS_AVATAR
	m_pButtonSynthesis	= static_cast< CKTDGUIButton* >( m_pDlgMessage->GetControl( L"Synthesis" ) );
#endif SERV_SYNTHESIS_AVATAR
	//}}
#ifdef NPC_EVENT_BUTTON
	m_pButtonEvent1 = static_cast< CKTDGUIButton* >( m_pDlgMessage->GetControl( L"Event1" ) );
	m_pButtonEvent2 = static_cast< CKTDGUIButton* >( m_pDlgMessage->GetControl( L"Event2" ) );
	m_pButtonEvent3 = static_cast< CKTDGUIButton* >( m_pDlgMessage->GetControl( L"Event3" ) );
	m_pButtonEvent4 = static_cast< CKTDGUIButton* >( m_pDlgMessage->GetControl( L"Event4" ) );
#endif NPC_EVENT_BUTTON

#ifdef SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT
	m_pButtonWeddingEvent = static_cast< CKTDGUIButton* >( m_pDlgMessage->GetControl( L"Wedding" ) );
#endif //SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
	m_pButtoenExchangeNewItem = static_cast< CKTDGUIButton* >( m_pDlgMessage->GetControl( L"ExchangeNewItem" ) );
#endif // SERV_NEW_ITEM_SYSTEM_2013_05

	pStaticShopName1 = m_pDlgMessage->GetStatic_LUA("ShopName1");
	pStaticShopName2 = m_pDlgMessage->GetStatic_LUA("ShopName2");
	pStaticShopmessage = m_pDlgMessage->GetStatic_LUA("npcMessage");



	bool bExist = false;
	NPCSHOPTYPE shopType;

	shopType.m_bShowNpcShop = false;
	shopType.m_eShopType = eType;

	if(bShow == true && eType != NSBT_NONE)
	{
		for(UINT i=0; i<m_vecShopType.size(); ++i)
		{
			if(m_vecShopType[i].m_eShopType == eType)
			{
				bExist = true;
			}
		}

		if(bExist == false)
			m_vecShopType.push_back(shopType);
	}
	else
	{
		for( std::vector<NPCSHOPTYPE>::iterator iter = m_vecShopType.begin(); iter != m_vecShopType.end();) 
		{
			if( iter->m_eShopType == eType ) 
			{				
				iter = m_vecShopType.erase(iter);
				--m_nEnabledButton;
			}
			else   
			{
				++iter; 
			}
		}
	}

#ifdef DISABLE_USELESS_SHOP
	vector<CX2Item::ItemTemplet*> vecShopItemList;
	vector<int> vecItemList;
	std::vector< CX2ItemManager::ItemExchangeData > vecTempItemExchangeData;
#endif DISABLE_USELESS_SHOP

	switch(eType)
	{	
	case NSBT_QUEST:
		m_pButtonQuest->SetShowEnable(bShow, bShow);
		if(bShow == true && bExist == false)
			++m_nEnabledButton;		
		break;
	case NSBT_SHOP:
		m_pButtonShop->SetShowEnable(bShow, bShow); 
		if(bShow == true && bExist == false)
			++m_nEnabledButton;
		break;
	case NSBT_PVP:
#ifdef SERV_PVP_NEW_SYSTEM

	#ifdef FIX_JOIN_OFFICIAL_PVP_ROOM // 김태환
		/// 던전 매칭 중에는 공식 대전 버튼 숨김
		if ( NULL != g_pData &&
		     NULL != g_pData->GetPartyManager() &&
		 	 false == g_pData->GetPartyManager()->GetProcessDungeonMatch() )
	#endif //FIX_JOIN_OFFICIAL_PVP_ROOM
		{
			m_pButtonFormalPvp->SetShowEnable(bShow, bShow);
			if( bShow == true && bExist == false )
				++m_nEnabledButton;
		}
#endif
		m_pButtonPvp->SetShowEnable(bShow, bShow);		
		if(bShow == true && bExist == false)
			++m_nEnabledButton;
		break;
	//{{ 09.03.24 태완 : 버튼 추가
	case NSBT_ENCHANT:
		m_pButtonEnchant->SetShowEnable(bShow, bShow);
		if(bShow == true && bExist == false)
			++m_nEnabledButton;		
		break;
	case NSBT_ATTRIBUTE:
		m_pButtonAttribute->SetShowEnable(bShow, bShow);
		if(bShow == true && bExist == false)
			++m_nEnabledButton;		
		break;
		//}}
	case NSBT_MAKE:
#ifdef DISABLE_USELESS_SHOP
		g_pData->GetItemManager()->GetManufactureVillageData( m_iHouseID, vecItemList );
		if( true == vecItemList.empty() )
			break;
#endif DISABLE_USELESS_SHOP
		m_pButtonMake->SetShowEnable(bShow, bShow);
		if(bShow == true && bExist == false)
			++m_nEnabledButton;
		break;
	case NSBT_FREETRAIN:
		m_pButtonFreeTraining->SetShowEnable(bShow, bShow);
		if(bShow == true && bExist == false)
			++m_nEnabledButton;
		break;
	case NSBT_TRAINING:
		m_pButtonTraining->SetShowEnable(bShow, bShow);
		if(bShow == true && bExist == false)
			++m_nEnabledButton;
		break;
	case NSBT_CLOSE:
		m_pButtonOut->SetShowEnable(bShow, bShow);
		if(bShow == true && bExist == false)
			++m_nEnabledButton;
		break;
#ifdef GUILD_BOARD
	//{{ oasis907 : 김상윤 [2009.11.24] //
	case NSBT_GUILD:
		{
			m_pButtonGuild->SetShowEnable(bShow, bShow);
#ifdef DEF_TRADE_BOARD
			if(m_bEnableButtonPersonalShop == false)
			{
				m_pButtonGuild->SetEnable( false );
			}
#endif DEF_TRADE_BOARD
			if(bShow == true && bExist == false)
				++m_nEnabledButton;
		}
		break;
	//}} oasis907 : 김상윤 [2009.11.24] //
#endif GUILD_BOARD

	case NSBT_RANKING:
		{
			m_pButtonRanking->SetShowEnable(bShow, bShow);
#ifdef DEF_TRADE_BOARD
			if(m_bEnableButtonPersonalShop == false)
			{
				m_pButtonRanking->SetEnable( false );
			}
#endif DEF_TRADE_BOARD
			if(bShow == true && bExist == false)
				++m_nEnabledButton;
#ifdef LOCAL_RANKING_UPDATE
			if(bShow == true && bExist == true && false == m_bShowLocalRankingInfo )
			{
				if( NULL != g_pData && NULL != g_pData->GetProfileManager() && false == g_pData->GetProfileManager()->GetIsProfileSave() && NULL != g_pData->GetUIMajorParticle() )
				{
					CKTDGParticleSystem::CParticleEventSequence* pTemp = g_pData->GetUIMajorParticle()->CreateSequence( NULL, L"LocalRankingInfo", 570,600,0.75 );
					if( NULL != pTemp )
					{	
						pTemp->SetOverUI( true );
						m_bShowLocalRankingInfo = true;
					}
				}
			}
#endif //LOCAL_RANKING_UPDATE
		}
		break;
		
#ifdef SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT
	case NSBT_WEDDING_EVENT:
		{
			if( NULL != m_pButtonWeddingEvent )
			{
				m_pButtonWeddingEvent->SetShowEnable(bShow, bShow);
	#ifdef DEF_TRADE_BOARD
				if( false == m_bEnableButtonPersonalShop )
				{
					m_pButtonWeddingEvent->SetEnable( false );
				}
	#endif //DEF_TRADE_BOARD
				if( true == bShow && false == bExist )
					++m_nEnabledButton;
			}
		}
		break;
#endif //SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT

		//{{ kimhc // 2009-07-25 // 헤니르의 시공에 추가되는 ITEM_EXCHANGE_SHOP 작업
#ifdef	ITEM_EXCHANGE_SHOP
	case NSBT_EXCHANGE:
#ifdef DISABLE_USELESS_SHOP
		g_pData->GetItemManager()->GetItemExchangeData( m_iHouseID, vecTempItemExchangeData );
		if( true == vecTempItemExchangeData.empty() )
			break;
#endif DISABLE_USELESS_SHOP
		m_pButtonExchange->SetShowEnable(bShow, bShow);
		if(bShow == true && bExist == false)
			++m_nEnabledButton;
		break;
#endif	ITEM_EXCHANGE_SHOP
		//}} kimhc // 2009-07-25 // 헤니르의 시공에 추가되는 ITEM_EXCHANGE_SHOP 작업

		//{{ kimhc // 2009-08-03 // 캐릭터별 은행
#ifdef	PRIVATE_BANK
	case NSBT_PRIVATE_BANK:
		{
			if( NULL != m_pButtonPrivateBank )
				m_pButtonPrivateBank->SetShowEnable( bShow, bShow );
			if ( bShow	== true && bExist == false )
				++m_nEnabledButton;
		} break;
#endif	PRIVATE_BANK
		//}} kimhc // 2009-08-03 // 캐릭터별 은행

#ifdef WEB_POINT_EVENT // oasis907 : 김상윤 [2010.2.24] // 
	case NSBT_WEB_EVENT:
		{
			if( NULL != m_pButtonWebEvent )
				m_pButtonWebEvent->SetShowEnable( bShow, bShow );
			if ( bShow	== true && bExist == false )
				++m_nEnabledButton;
		} break;
#endif WEB_POINT_EVENT
#ifdef DEF_TRADE_BOARD // oasis907 : 김상윤 [2010.3.12] // 
	case NSBT_PERSONAL_SHOP:
		{
			if( NULL != m_pButtonPersonalShop )
			{
				m_pButtonPersonalShop->SetShowEnable( bShow, bShow );
				if(m_bEnableButtonPersonalShop == false)
				{
					m_pButtonPersonalShop->SetEnable( false );
				}
			}
			if ( bShow	== true && bExist == false )
				++m_nEnabledButton;
		} break;
#endif DEF_TRADE_BOARD
#ifdef SERV_PSHOP_AGENCY
	case NSBT_AGENCY_TRADER_REGISTER:
		{
			m_pButtonAgencyTraderRegister->SetShowEnable(bShow, bShow);

	#ifdef SERV_UPGRADE_TRADE_SYSTEM // 김태환
			if(m_bEnableButtonPersonalShop == false)
			{
				m_pButtonAgencyTraderRegister->SetEnable( false );
			}
	#endif //SERV_UPGRADE_TRADE_SYSTEM

			if(bShow == true && bExist == false)
				++m_nEnabledButton;		
			break;
		}
		break;
	case NSBT_AGENCY_TRADER_RECEIVE:
		{
			m_pButtonAgencyTraderReceive->SetShowEnable(bShow, bShow);

	#ifdef SERV_UPGRADE_TRADE_SYSTEM // 김태환
			if(m_bEnableButtonPersonalShop == false)
			{
				m_pButtonAgencyTraderReceive->SetEnable( false );
			}
	#endif //SERV_UPGRADE_TRADE_SYSTEM

			if(bShow == true && bExist == false)
				++m_nEnabledButton;		
			break;
		}
		break;
#endif
		//{{ 2011.05.04   임규수 아바타 합성 시스템
#ifdef SERV_SYNTHESIS_AVATAR
	case NSBT_SYNTHESIS:
		{
			m_pButtonSynthesis->SetShowEnable(bShow, bShow);
			if(bShow == true && bExist == false)
				++m_nEnabledButton;
		} break;
#endif SERV_SYNTHESIS_AVATAR
		//}}
#ifdef NPC_EVENT_BUTTON
	case NSBT_EVENT1:
		{
#ifdef SERV_READY_TO_SOSUN_EVENT
			IF_EVENT_ENABLED( CEI_READY_TO_SOSUN_EVENT )
			{
				m_pButtonEvent1->SetShowEnable(bShow, bShow);
				if(bShow == true && bShow == false )
					++m_nEnabledButton;
			}
			ELSE
			{
				m_pButtonEvent1->SetShowEnable(false, false);
				if(bShow == true && bShow == false )
					++m_nEnabledButton;
			}

#else //SERV_READY_TO_SOSUN_EVENT
			m_pButtonEvent1->SetShowEnable(bShow, bShow);
			if(bShow == true && bShow == false )
				++m_nEnabledButton;
#endif //SERV_READY_TO_SOSUN_EVENT
		} break;
	case NSBT_EVENT2:
		{
			m_pButtonEvent2->SetShowEnable(bShow, bShow);
			if(bShow == true && bShow == false )
				++m_nEnabledButton;
		} break;
	case NSBT_EVENT3:
		{
			m_pButtonEvent3->SetShowEnable(bShow, bShow);
			if(bShow == true && bShow == false )
				++m_nEnabledButton;
		} break;
	case NSBT_EVENT4:
		{
			m_pButtonEvent4->SetShowEnable(bShow, bShow);
			if(bShow == true && bShow == false )
				++m_nEnabledButton;
		} break;
#endif NPC_EVENT_BUTTON
	case NSBT_PASS:
		if(bShow == false)
		{
			m_pButtonPass->SetShowEnable(false, false);	
			m_pButtonWindowPass->SetShowEnable(false, false);
		}
		
		m_pButtonEnchant->SetShowEnable(false, false);
		m_pButtonAttribute->SetShowEnable(false, false);

		m_pButtonQuest->SetShowEnable(false, false);
#ifdef SERV_PVP_NEW_SYSTEM
		m_pButtonFormalPvp->SetShowEnable(false, false);
#endif
		m_pButtonPvp->SetShowEnable(false, false);
		m_pButtonShop->SetShowEnable(false, false); 		
		m_pButtonMake->SetShowEnable(false, false);
		m_pButtonOut->SetShowEnable(false, false);
		m_pButtonTraining->SetShowEnable(false, false);
		m_pButtonFreeTraining->SetShowEnable(false, false);

		m_pButtonNext->SetShowEnable(false, false);
		m_pButtonAgree->SetShowEnable(false, false);
		m_pButtonComplete->SetShowEnable(false, false);
#ifdef GUILD_BOARD
		m_pButtonGuild->SetShowEnable(false, false); // oasis907 : 김상윤 [2009.11.24] //
#endif GUILD_BOARD		
		m_pButtonRanking->SetShowEnable(false, false);

#ifdef SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT
		if( NULL != m_pButtonWeddingEvent )
			m_pButtonWeddingEvent->SetShowEnable(false, false);
#endif //SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT

		//{{ kimhc // 2009-07-25 // 헤니르의 시공에 추가되는 ITEM_EXCHANGE_SHOP 작업
#ifdef	ITEM_EXCHANGE_SHOP
		m_pButtonExchange->SetShowEnable(false, false);
#endif	ITEM_EXCHANGE_SHOP
		//}} kimhc // 2009-07-25 // 헤니르의 시공에 추가되는 ITEM_EXCHANGE_SHOP 작업
		
		//{{ kimhc // 2009-08-03 // 캐릭터별 은행
#ifdef	PRIVATE_BANK
		if( NULL != m_pButtonPrivateBank )
			m_pButtonPrivateBank->SetShowEnable( false, false );
#endif	PRIVATE_BANK
		//}} kimhc // 2009-08-03 // 캐릭터별 은행
#ifdef WEB_POINT_EVENT // oasis907 : 김상윤 [2010.2.24] // 
		m_pButtonWebEvent->SetShowEnable(false, false);
#endif WEB_POINT_EVENT
#ifdef DEF_TRADE_BOARD
		m_pButtonPersonalShop->SetShowEnable(false, false);
#endif DEF_TRADE_BOARD

		//{{ 2011.05.04   임규수 아바타 합성 시스템
#ifdef SERV_SYNTHESIS_AVATAR
		m_pButtonSynthesis->SetShowEnable(false, false);
#endif SERV_SYNTHESIS_AVATAR
		//}}
#ifdef NPC_EVENT_BUTTON
		m_pButtonEvent1->SetShowEnable(false, false);
		m_pButtonEvent2->SetShowEnable(false, false);
		m_pButtonEvent3->SetShowEnable(false, false);
		m_pButtonEvent4->SetShowEnable(false, false);
#endif NPC_EVENT_BUTTON

#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
		m_pButtoenExchangeNewItem->SetShowEnable(false, false);
#endif //SERV_NEW_ITEM_SYSTEM_2013_05

#ifdef SERV_PSHOP_AGENCY
		m_pButtonAgencyTraderRegister->SetShowEnable(false, false);
		m_pButtonAgencyTraderReceive->SetShowEnable(false, false);
#endif
		SetShow(NSBT_ENCHANT, false);
		SetShow(NSBT_ATTRIBUTE, false);

		SetShow(NSBT_QUEST, false);
		SetShow(NSBT_SHOP, false);
		SetShow(NSBT_PVP, false);
		SetShow(NSBT_MAKE, false);
		SetShow(NSBT_FREETRAIN, false);
		SetShow(NSBT_TRAINING, false);
		SetShow(NSBT_CLOSE, false);
		SetShow(NSBT_NEXT, false);
		SetShow(NSBT_AGREE, false);
		SetShow(NSBT_COMPLETE, false);		
#ifdef GUILD_BOARD
		SetShow(NSBT_GUILD, false); // oasis907 : 김상윤 [2009.11.24] //
#endif GUILD_BOARD
		SetShow(NSBT_RANKING, false);

		//{{ kimhc // 2009-07-25 // 헤니르의 시공에 추가되는 ITEM_EXCHANGE_SHOP 작업
#ifdef	ITEM_EXCHANGE_SHOP
		SetShow(NSBT_EXCHANGE, false);
#endif	ITEM_EXCHANGE_SHOP
		//}} kimhc // 2009-07-25 // 헤니르의 시공에 추가되는 ITEM_EXCHANGE_SHOP 작업

		//{{ kimhc // 2009-08-03 // 캐릭터별 은행
#ifdef	PRIVATE_BANK
		SetShow( NSBT_PRIVATE_BANK, false );
#endif	PRIVATE_BANK
		//}} kimhc // 2009-08-03 // 캐릭터별 은행
#ifdef WEB_POINT_EVENT // oasis907 : 김상윤 [2010.2.24] // 
		SetShow( NSBT_WEB_EVENT, false );
#endif WEB_POINT_EVENT
#ifdef DEF_TRADE_BOARD
		SetShow( NSBT_PERSONAL_SHOP, false );
#endif DEF_TRADE_BOARD
#ifdef SERV_PSHOP_AGENCY
		SetShow( NSBT_AGENCY_TRADER_REGISTER, false );
		SetShow( NSBT_AGENCY_TRADER_RECEIVE, false );
#endif
		//{{ 2011.05.04   임규수 아바타 합성 시스템
#ifdef SERV_SYNTHESIS_AVATAR
		SetShow(NSBT_SYNTHESIS, false);
#endif SERV_SYNTHESIS_AVATAR
		//}}
#ifdef NPC_EVENT_BUTTON
		SetShow(NSBT_EVENT1, false);
		SetShow(NSBT_EVENT2, false);
		SetShow(NSBT_EVENT3, false);
		SetShow(NSBT_EVENT4, false);
#endif NPC_EVENT_BUTTON

#ifdef SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT
		SetShow( NSBT_WEDDING_EVENT, false );
#endif //SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT

#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
		SetShow( NSBT_EXCHANGE_NEW_ITEM, false );
#endif //SERV_NEW_ITEM_SYSTEM_2013_05

		m_nEnabledButton = 0; 
		break;
	case NSBT_NEXT:
		m_pButtonNext->SetShowEnable(bShow, bShow);
		if(bShow == true && bExist == false)
			++m_nEnabledButton;
		break;
	case NSBT_AGREE:
		m_pButtonAgree->SetShowEnable(bShow, bShow);
		if(bShow == true && bExist == false)
			++m_nEnabledButton;
		break;
	case NSBT_COMPLETE:
		m_pButtonComplete->SetShowEnable(bShow, bShow);
		if(bShow == true && bExist == false)
			++m_nEnabledButton;
		break;

#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
	case NSBT_EXCHANGE_NEW_ITEM:
		{
			m_pButtoenExchangeNewItem->SetShowEnable(bShow, bShow);
			if( true == bShow && false == bExist )
				++m_nEnabledButton;
		}
		break;
#endif //SERV_NEW_ITEM_SYSTEM_2013_05
	case NSBT_NONE:	
	default:
		m_pButtonEnchant->SetShowEnable(false, false);
		m_pButtonAttribute->SetShowEnable(false, false);

		m_pButtonQuest->SetShowEnable(false, false);
#ifdef SERV_PVP_NEW_SYSTEM
		m_pButtonFormalPvp->SetShowEnable(false, false);
#endif
		m_pButtonPvp->SetShowEnable(false, false);
		m_pButtonShop->SetShowEnable(false, false); 
		m_pButtonPass->SetShowEnable(false, false);
		m_pButtonWindowPass->SetShowEnable(false, false);
		m_pButtonMake->SetShowEnable(false, false);
		m_pButtonOut->SetShowEnable(false, false);
		m_pButtonTraining->SetShowEnable(false, false);
		m_pButtonFreeTraining->SetShowEnable(false, false);

		m_pButtonNext->SetShowEnable(false, false);
		m_pButtonAgree->SetShowEnable(false, false);
		m_pButtonComplete->SetShowEnable(false, false);

#ifdef GUILD_BOARD
		m_pButtonGuild->SetShowEnable(false, false);
#endif GUILD_BOARD
		m_pButtonRanking->SetShowEnable(false, false);

#ifdef SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT
		if( NULL != m_pButtonWeddingEvent )
			m_pButtonWeddingEvent->SetShowEnable(false, false);
#endif //SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT

		//{{ kimhc // 2009-07-25 // 헤니르의 시공에 추가되는 ITEM_EXCHANGE_SHOP 작업
#ifdef	ITEM_EXCHANGE_SHOP
		m_pButtonExchange->SetShowEnable(false, false);
#endif	ITEM_EXCHANGE_SHOP
		//}} kimhc // 2009-07-25 // 헤니르의 시공에 추가되는 ITEM_EXCHANGE_SHOP 작업

		//{{ kimhc // 2009-08-03 // 캐릭터별 은행
#ifdef	PRIVATE_BANK
		if( NULL != m_pButtonPrivateBank )
			m_pButtonPrivateBank->SetShowEnable( false, false );
#endif	PRIVATE_BANK
		//}} kimhc // 2009-08-03 // 캐릭터별 은행
#ifdef WEB_POINT_EVENT // oasis907 : 김상윤 [2010.2.24] // 
		m_pButtonWebEvent->SetShowEnable(false, false);
#endif WEB_POINT_EVENT
#ifdef DEF_TRADE_BOARD
		m_pButtonPersonalShop->SetShowEnable(false, false);
#endif DEF_TRADE_BOARD
#ifdef SERV_PSHOP_AGENCY
		m_pButtonAgencyTraderReceive->SetShowEnable(false, false);
		m_pButtonAgencyTraderRegister->SetShowEnable(false, false);
#endif
		//{{ 2011.05.04   임규수 아바타 합성 시스템
#ifdef SERV_SYNTHESIS_AVATAR
		m_pButtonSynthesis->SetShowEnable(false, false);
#endif SERV_SYNTHESIS_AVATAR
		//}}
#ifdef NPC_EVENT_BUTTON
		m_pButtonEvent1->SetShowEnable(false, false);
		m_pButtonEvent2->SetShowEnable(false, false);
		m_pButtonEvent3->SetShowEnable(false, false);
		m_pButtonEvent4->SetShowEnable(false, false);
#endif NPC_EVENT_BUTTON

#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
		m_pButtoenExchangeNewItem->SetShowEnable(false, false);
#endif //SERV_NEW_ITEM_SYSTEM_2013_05

		SetShow(NSBT_ENCHANT, false);
		SetShow(NSBT_ATTRIBUTE, false);

		SetShow(NSBT_QUEST, false);
		SetShow(NSBT_SHOP, false);
		SetShow(NSBT_PVP, false);
		SetShow(NSBT_MAKE, false);
		SetShow(NSBT_FREETRAIN, false);
		SetShow(NSBT_TRAINING, false);
		SetShow(NSBT_CLOSE, false);

		SetShow(NSBT_NEXT, false);
		SetShow(NSBT_AGREE, false);
		SetShow(NSBT_COMPLETE, false);
#ifdef GUILD_BOARD
		SetShow(NSBT_GUILD, false);
#endif GUILD_BOARD
		SetShow(NSBT_RANKING, false);

		//{{ kimhc // 2009-07-25 // 헤니르의 시공에 추가되는 ITEM_EXCHANGE_SHOP 작업
#ifdef	ITEM_EXCHANGE_SHOP
		SetShow(NSBT_EXCHANGE, false);
#endif	ITEM_EXCHANGE_SHOP
		//}} kimhc // 2009-07-25 // 헤니르의 시공에 추가되는 ITEM_EXCHANGE_SHOP 작업

		//{{ kimhc // 2009-08-03 // 캐릭터별 은행
#ifdef	PRIVATE_BANK
		SetShow( NSBT_PRIVATE_BANK, false );
#endif	PRIVATE_BANK
		//}} kimhc // 2009-08-03 // 캐릭터별 은행
#ifdef WEB_POINT_EVENT // oasis907 : 김상윤 [2010.2.24] // 
		SetShow( NSBT_WEB_EVENT, false );
#endif WEB_POINT_EVENT
#ifdef DEF_TRADE_BOARD
		SetShow( NSBT_PERSONAL_SHOP, false );
#endif DEF_TRADE_BOARD
#ifdef SERV_PSHOP_AGENCY
		SetShow( NSBT_AGENCY_TRADER_REGISTER, false );
		SetShow( NSBT_AGENCY_TRADER_RECEIVE, false );
#endif
		//{{ 2011.05.04   임규수 아바타 합성 시스템
#ifdef SERV_SYNTHESIS_AVATAR
		SetShow(NSBT_SYNTHESIS, false);
#endif SERV_SYNTHESIS_AVATAR
		//}}
#ifdef NPC_EVENT_BUTTON
		SetShow(NSBT_EVENT1, false);
		SetShow(NSBT_EVENT2, false);
		SetShow(NSBT_EVENT3, false);
		SetShow(NSBT_EVENT4, false);
#endif NPC_EVENT_BUTTON

#ifdef SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT
		SetShow( NSBT_WEDDING_EVENT, false );
#endif //SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT
		m_nEnabledButton = 0; 
		m_vecShopType.clear();
		break;
	}

	float MaxRight = 593.f - 30.f;
	float buttonWidth = 88.f;
	float offsetX = MaxRight;

	if(m_pButtonAgree->GetShow())
	{
		m_pButtonAgree->SetOffsetPos_LUA(offsetX, 0.f);
		offsetX -= buttonWidth;
	}
	if(m_pButtonComplete->GetShow())
	{
		m_pButtonComplete->SetEdge( false, 1, D3DXCOLOR( 0.4f,0.6f,0.8f,1.f ));
		m_pButtonComplete->SetOffsetPos_LUA(offsetX, 0.f);
		offsetX -= buttonWidth;
	}
	if(m_pButtonNext->GetShow())
	{
		m_pButtonNext->SetOffsetPos_LUA(offsetX, 0.f);
		offsetX -= buttonWidth;
	}
	if(m_pButtonOut->GetShow())
	{
		m_pButtonOut->SetEdge( false, 1, D3DXCOLOR( 0.4f,0.6f,0.8f,1.f ));
		m_pButtonOut->SetOffsetPos_LUA(offsetX, 0.f);
		offsetX -= buttonWidth;
	}	
	if(m_pButtonRanking->GetShow())
	{
		m_pButtonRanking->SetEdge( false, 1, D3DXCOLOR( 0.4f,0.6f,0.8f,1.f ) );
		m_pButtonRanking->SetOffsetPos_LUA(offsetX, 0.f);
		offsetX -= buttonWidth;
	}

#ifdef SERV_PSHOP_AGENCY
	if( m_pButtonAgencyTraderReceive->GetShow())
	{
		m_pButtonAgencyTraderReceive->SetEdge( false, 1, D3DXCOLOR( 0.4f,0.6f,0.8f,1.f ) );
		m_pButtonAgencyTraderReceive->SetOffsetPos_LUA(offsetX, 0.f);
		offsetX -= buttonWidth;
	}
	if( m_pButtonAgencyTraderRegister->GetShow())
	{
		m_pButtonAgencyTraderRegister->SetEdge( false, 1, D3DXCOLOR( 0.4f,0.6f,0.8f,1.f ) );
		m_pButtonAgencyTraderRegister->SetOffsetPos_LUA(offsetX, 0.f);
		offsetX -= buttonWidth;
	}	
#endif

	//{{ kimhc // 2009-08-03 // 캐릭터별 은행
#ifdef	PRIVATE_BANK
	if( NULL != m_pButtonPrivateBank &&
		m_pButtonPrivateBank->GetShow() )
	{
		m_pButtonPrivateBank->SetEdge( false, 1, D3DXCOLOR( 0.4f, 0.6f, 0.8f, 1.0f) );
		m_pButtonPrivateBank->SetOffsetPos_LUA(offsetX, 0.f);
		offsetX -= buttonWidth;
	}
#endif	PRIVATE_BANK
	//}} kimhc // 2009-08-03 // 캐릭터별 은행

	//{{ kimhc // 2009-07-25 // 헤니르의 시공에 추가되는 ITEM_EXCHANGE_SHOP 작업
#ifdef	ITEM_EXCHANGE_SHOP
	if(m_pButtonExchange->GetShow())
	{
		m_pButtonExchange->SetEdge( false, 1, D3DXCOLOR( 0.4f,0.6f,0.8f,1.f ) );
		m_pButtonExchange->SetOffsetPos_LUA(offsetX, 0.f);
		offsetX -= buttonWidth;
	}
#endif	ITEM_EXCHANGE_SHOP
	//}} kimhc // 2009-07-25 // 헤니르의 시공에 추가되는 ITEM_EXCHANGE_SHOP 작업

	if(m_pButtonFreeTraining->GetShow())
	{
		m_pButtonFreeTraining->SetEdge( false, 1, D3DXCOLOR( 0.4f,0.6f,0.8f,1.f ) );
		m_pButtonFreeTraining->SetOffsetPos_LUA(offsetX, 0.f);
		offsetX -= buttonWidth;
	}
	if(m_pButtonShop->GetShow())
	{
		m_pButtonShop->SetEdge( false, 1, D3DXCOLOR( 0.4f,0.6f,0.8f,1.f ));
		m_pButtonShop->SetOffsetPos_LUA(offsetX, 0.f);
		offsetX -= buttonWidth;
	}
	if(m_pButtonMake->GetShow())
	{
		m_pButtonMake->SetEdge( false, 1, D3DXCOLOR( 0.4f,0.6f,0.8f,1.f ) );
		m_pButtonMake->SetOffsetPos_LUA(offsetX, 0.f);
		offsetX -= buttonWidth;
	}
	if(m_pButtonEnchant->GetShow())
	{
		m_pButtonEnchant->SetEdge( false, 1, D3DXCOLOR( 0.4f,0.6f,0.8f,1.f ) );
		m_pButtonEnchant->SetOffsetPos_LUA(offsetX, 0.f);
		offsetX -= buttonWidth;
	}
	if(m_pButtonAttribute->GetShow())
	{
		m_pButtonAttribute->SetEdge( false, 1, D3DXCOLOR( 0.4f,0.6f,0.8f,1.f ) );
		m_pButtonAttribute->SetOffsetPos_LUA(offsetX, 0.f);
		offsetX -= buttonWidth;
	}
	if(m_pButtonPvp->GetShow())
	{
		m_pButtonPvp->SetEdge( false, 1, D3DXCOLOR( 0.4f,0.6f,0.8f,1.f ) );
		m_pButtonPvp->SetOffsetPos_LUA(offsetX, 0.f);
		offsetX -= buttonWidth;
	}
#ifdef SERV_PVP_NEW_SYSTEM
	if( m_pButtonFormalPvp->GetShow() )
	{
		m_pButtonFormalPvp->SetEdge( false, 1, D3DXCOLOR( 0.4f,0.6f,0.8f,1.f ) );
		m_pButtonFormalPvp->SetOffsetPos_LUA(offsetX, 0.f);
		offsetX -= buttonWidth;
	}
#endif
	if(m_pButtonTraining->GetShow())
	{
		m_pButtonTraining->SetEdge( false, 1, D3DXCOLOR( 0.4f,0.6f,0.8f,1.f ) );
		m_pButtonTraining->SetOffsetPos_LUA(offsetX, 0.f);
		offsetX -= buttonWidth;
	}	
#ifdef DEF_TRADE_BOARD
	if(m_pButtonPersonalShop->GetShow())
	{
		m_pButtonPersonalShop->SetEdge( false, 1, D3DXCOLOR( 0.4f,0.6f,0.8f,1.f ) );
		m_pButtonPersonalShop->SetOffsetPos_LUA(offsetX, 0.f);
		offsetX -= buttonWidth;
	}
#endif DEF_TRADE_BOARD
#ifdef GUILD_BOARD
	//{{ oasis907 : 김상윤 [2009.11.24] //
	if(m_pButtonGuild->GetShow())
	{
		m_pButtonGuild->SetEdge( false, 1, D3DXCOLOR( 0.4f,0.6f,0.8f,1.f ) );
		m_pButtonGuild->SetOffsetPos_LUA(offsetX, 0.f);
		offsetX -= buttonWidth;
	}
#endif GUILD_BOARD
	//{{ 2011.05.04   임규수 아바타 합성 시스템
#ifdef SERV_SYNTHESIS_AVATAR
	if(m_pButtonSynthesis->GetShow())
	{
		m_pButtonSynthesis->SetEdge( false, 1, D3DXCOLOR( 0.4f,0.6f,0.8f,1.f ));
		m_pButtonSynthesis->SetOffsetPos_LUA(offsetX, 0.f);
		offsetX -= buttonWidth;
	}
#endif SERV_SYNTHESIS_AVATAR
	//}}
#ifdef EVENT_BUTTON_EU_ONLY_FIX
	if(m_pButtonQuest->GetShow())
	{
		m_pButtonQuest->SetEdge( false, 1, D3DXCOLOR( 0.4f,0.6f,0.8f,1.f ) );
		m_pButtonQuest->SetOffsetPos_LUA(offsetX, 0.f);
		offsetX -= buttonWidth;
	}	
#endif //EVENT_BUTTON_EU_ONLY_FIX

#ifdef NPC_EVENT_BUTTON
	if(m_pButtonEvent4->GetShow())
	{
		m_pButtonEvent4->SetEdge( false, 1, D3DXCOLOR( 0.4f,0.6f,0.8f,1.f ));
		m_pButtonEvent4->SetOffsetPos_LUA(offsetX, 0.f);
		offsetX -= buttonWidth;
	}

	if(m_pButtonEvent3->GetShow())
	{
		m_pButtonEvent3->SetEdge( false, 1, D3DXCOLOR( 0.4f,0.6f,0.8f,1.f ));
		m_pButtonEvent3->SetOffsetPos_LUA(offsetX, 0.f);
		offsetX -= buttonWidth;
	}

	if(m_pButtonEvent2->GetShow())
	{
		m_pButtonEvent2->SetEdge( false, 1, D3DXCOLOR( 0.4f,0.6f,0.8f,1.f ));
		m_pButtonEvent2->SetOffsetPos_LUA(offsetX, 0.f);
		offsetX -= buttonWidth;
	}

	if(m_pButtonEvent1->GetShow())
	{
#ifdef EVENT_BUTTON_EU_ONLY_FIX	
		offsetX -= 40.f;
#endif //EVENT_BUTTON_EU_ONLY_FIX		
		m_pButtonEvent1->SetEdge( false, 1, D3DXCOLOR( 0.4f,0.6f,0.8f,1.f ));
		m_pButtonEvent1->SetOffsetPos_LUA(offsetX, 0.f);
		offsetX -= buttonWidth;
	}
#endif NPC_EVENT_BUTTON

#ifndef EVENT_BUTTON_EU_ONLY_FIX
	if(m_pButtonQuest->GetShow())
	{
		m_pButtonQuest->SetEdge( false, 1, D3DXCOLOR( 0.4f,0.6f,0.8f,1.f ) );
		m_pButtonQuest->SetOffsetPos_LUA(offsetX, 0.f);
		offsetX -= buttonWidth;
	}	
#endif //EVENT_BUTTON_EU_ONLY_FIX	
#ifdef WEB_POINT_EVENT // oasis907 : 김상윤 [2010.2.24] // 
	if(m_pButtonWebEvent->GetShow())
	{
		m_pButtonWebEvent->SetEdge( false, 1, D3DXCOLOR( 0.4f,0.6f,0.8f,1.f ) );
		m_pButtonWebEvent->SetOffsetPos_LUA(offsetX, 0.f);
		offsetX -= buttonWidth;
	}
#endif WEB_POINT_EVENT
#ifdef SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT
	if( NULL != m_pButtonWeddingEvent && m_pButtonWeddingEvent->GetShow() )
	{
		offsetX -= 220.f;
		m_pButtonWeddingEvent->SetEdge( false, 1, D3DXCOLOR( 0.4f, 0.6f, 0.8f, 1.f) );
		m_pButtonWeddingEvent->SetOffsetPos_LUA(offsetX, 0.f);
		offsetX -= buttonWidth;
	}
#endif //SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
	if( NULL != m_pButtoenExchangeNewItem && m_pButtoenExchangeNewItem->GetShow() )
	{
#ifdef EVENT_BUTTON_EU_ONLY_FIX	
		offsetX -= 30.f;
#else //EVENT_BUTTON_EU_ONLY_FIX
		offsetX -= 70.f;
#endif //EVENT_BUTTON_EU_ONLY_FIX
		m_pButtoenExchangeNewItem->SetEdge( false, 1, D3DXCOLOR( 0.4f, 0.6f, 0.8f, 1.f) );
		m_pButtoenExchangeNewItem->SetOffsetPos_LUA(offsetX, 0.f);
		offsetX -= buttonWidth;
	}
#endif //SERV_NEW_ITEM_SYSTEM_2013_05
}

void CX2TFieldNpcShop::SetShow(bool val)
{
	m_bShowNpcShop = val;
//	m_pDlgMessage->SetFront(val);
	if(m_pDlgMessage != NULL)
		m_pDlgMessage->SetShowEnable(val, val);
	if(m_pDLGNpcName != NULL)
		m_pDLGNpcName->SetShowEnable(val, val);	

	for(UINT i=0; i<m_vecShopType.size(); ++i)
	{
		if(m_vecShopType[i].m_bShowNpcShop == true)
			return;
	}
#ifdef LOCAL_RANKING_UPDATE
	m_bShowLocalRankingInfo = m_bShowNpcShop;
#endif //LOCAL_RANKING_UPDATE
}

void CX2TFieldNpcShop::SetShow(NPC_SHOP_BUTTON_TYPE eType, bool val)
{
	for(UINT i=0; i<m_vecShopType.size(); ++i)
	{
		if(m_vecShopType[i].m_eShopType == eType)
		{
			m_vecShopType[i].m_bShowNpcShop = val;			
			m_eNowState = eType;
			return;
		}
	}
}

bool CX2TFieldNpcShop::GetShow(NPC_SHOP_BUTTON_TYPE eType)
{
	for(UINT i=0; i<m_vecShopType.size(); ++i)
	{
		if(m_vecShopType[i].m_eShopType == eType)
		{
			return m_vecShopType[i].m_bShowNpcShop;
		}
	}
	return false;
}


void CX2TFieldNpcShop::SetNpcMessage(const wstring &npcMsg)
{
    if( m_pDlgMessage != NULL && pStaticShopmessage != NULL )
    {
#ifdef CLIENT_GLOBAL_LINEBREAK
		wstring str = npcMsg;

		CKTDGFontManager::CUKFont* pFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( pStaticShopmessage->GetString(0)->fontIndex );
		if( NULL == pFont )
			return; 
		
		D3DXVECTOR2 vTemp = g_pKTDXApp->ConvertByResolution( 650, 512 );
		const int CHAT_LINE_WIDTH = (int)vTemp.x;
		int addRow = CWordLineHandler::LineBreakInX2Main(str, pFont, CHAT_LINE_WIDTH, L"", true, false);
		
		if( -1 == addRow)
			return;
		
		pStaticShopmessage->GetString(0)->msg = str;
#else //#ifdef CLIENT_GLOBAL_LINEBREAK
	    pStaticShopmessage->GetString(0)->msg = npcMsg;	
#endif //CLIENT_GLOBAL_LINEBREAK
    }//if
}

#ifdef SERV_PSHOP_AGENCY
void CX2TFieldNpcShop::SetNpcMessageWithEtc(const wstring &npcMsg, const wstring &npcMsgEtc)
{
	if( m_pDlgMessage != NULL && pStaticShopmessage != NULL )
	{
		wstring wstrNpcMsg;
		wstrNpcMsg = npcMsg + L"\n\n" + npcMsgEtc;
#ifdef CLIENT_GLOBAL_LINEBREAK
		wstring str = wstrNpcMsg;

		CKTDGFontManager::CUKFont* pFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( pStaticShopmessage->GetString(0)->fontIndex );
		if( NULL == pFont )
			return; 

		D3DXVECTOR2 vTemp = g_pKTDXApp->ConvertByResolution( 650, 512 );
		const int CHAT_LINE_WIDTH = (int)vTemp.x;
		int addRow = CWordLineHandler::LineBreakInX2Main(str, pFont, CHAT_LINE_WIDTH, L"", true, false);

		if( -1 == addRow)
			return;

		pStaticShopmessage->GetString(0)->msg = str;
#else //#ifdef CLIENT_GLOBAL_LINEBREAK
		pStaticShopmessage->GetString(0)->msg = wstrNpcMsg;	
#endif //CLIENT_GLOBAL_LINEBREAK
		
	}//if
}
#endif

void CX2TFieldNpcShop::CreateHouseNPCName(CX2LocationManager::HouseTemplet* pHouse)
{
	CX2LocationManager::HouseTemplet *pHouseTemplet = pHouse;
	m_iHouseID = pHouse->m_ID;

	if ( pHouseTemplet != NULL && pHouseTemplet->m_NPCTextureName.empty() == false && pHouseTemplet->m_NPCTextureKey.empty() == false )
	{
		SAFE_DELETE_DIALOG( m_pDLGNpcName );
		m_pDLGNpcName = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_House_NPC_Name_Templete.lua" );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGNpcName );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDLGNpcName, false );

		D3DXVECTOR2 vPos = pHouseTemplet->m_NPCTexturePos;

 		vPos.y-=10;
		vPos.x += 600.f;
		m_pDLGNpcName->SetPos( vPos );

		CKTDGUIStatic* pStatic = (CKTDGUIStatic*)m_pDLGNpcName->GetControl(L"StaticHouseNPCName");
		if ( pStatic != NULL && pStatic->GetPicture(0) != NULL )
		{
			pStatic->GetPicture(0)->SetTex( pHouseTemplet->m_NPCTextureName.c_str(), pHouseTemplet->m_NPCTextureKey.c_str() );
		}

		m_pDLGNpcName->SetShowEnable(false, false);
	}
}

HRESULT CX2TFieldNpcShop::OnFrameMove( double fTime, float fElapsedTime )
{
    //{{ seojt // 2009-8-21, 11:46
    // check whether handle is invalid or not. jintaeks on 2009-08-21, 11:46
    if( m_pDlgMessage == NULL )
    {
        return S_OK;
    }//if
    //}} seojt // 2009-8-21, 11:46

	if( g_pData->GetCashShop() != NULL )
	{
		if( g_pData->GetCashShop()->GetOpen() == true )
		{
			m_pDlgMessage->SetShowEnable(false, false); 
		}
		else
		{			
			if(m_bShowNpcShop == true && m_pDlgMessage->GetShow() == false)
				m_pDlgMessage->SetShowEnable(true, true); 
		}
	}

	if(g_pData != NULL && g_pData->GetUIManager() != NULL)
	{
#ifdef SERV_EPIC_QUEST
		if(g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_QUEST_NEW) == true)
		{			
			SetShopType(NSBT_PASS);
		}
#else
		if(g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_QUEST_RECEIVE) == true)
		{			
			SetShopType(NSBT_PASS);
		}
#endif SERV_EPIC_QUEST
		else
		{
			SetShopType(NSBT_PASS, false);
			for(UINT i=0; i<m_vecShopType.size(); ++i)
			{
				SetShopType(m_vecShopType[i].m_eShopType);
			}

#ifndef REFORM_NOVICE_GUIDE
			// 초보자 가이드
			if( m_pDlgMessage->GetShow() == true && 
				g_pTFieldGame != NULL && g_pTFieldGame->GetNoviceGuide() != NULL &&
				g_pTFieldGame->GetNoviceGuide()->GetIsPlayGuide() == true )				
			{
				if( g_pTFieldGame->GetNoviceGuide()->GetNowGuideStep() == CX2NoviceGuide::NGS_STEP3 &&
					g_pTFieldGame->GetJoinNpcId() == CX2UnitManager::NUI_HAGERS )
				{
					if(g_pData->GetQuestManager()->GetUnitCompleteQuest(11000) != true)
					{
						g_pTFieldGame->GetNoviceGuide()->SetMouseClickIndex(12);
					}
				}
			}
#endif //REFORM_NOVICE_GUIDE
		}		
	}

	return S_OK;
}

int CX2TFieldNpcShop::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch(wParam)
	{
	case NMUM_CLOSE:
		if( g_pData->GetUIManager()->GetShowNpcShop() == true )
		{
			//g_pData->GetUIManager()->CloseAll();			
			g_pData->GetUIManager()->CloseAllNPCDlg(); //npc UI만 닫는다.
		}
		SetShow(false);		

#ifdef SERV_PSHOP_AGENCY
		if( g_pData != NULL && g_pData->GetUIManager() != NULL && g_pData->GetUIManager()->GetUIPersonalShop() != NULL &&
			( g_pData->GetUIManager()->GetUIPersonalShop()->GetPersonalShopState() == CX2UIPersonalShop::XPSS_AGENCY_SELL ||
			  g_pData->GetUIManager()->GetUIPersonalShop()->GetPersonalShopState() == CX2UIPersonalShop::XPSS_AGENCY_WAIT ||
			  g_pData->GetUIManager()->GetUIPersonalShop()->GetPersonalShopState() == CX2UIPersonalShop::XPSS_AGENCY_TAKE))
		{
			g_pData->GetUIManager()->GetUIPersonalShop()->Handler_EGS_LEAVE_PSHOP_AGENCY_REQ();
		}
#endif
#ifdef DEF_TRADE_BOARD
#ifdef SERV_PSHOP_AGENCY
		if( m_bEnableButtonPersonalShop == false && 
			g_pData != NULL && g_pData->GetUIManager() != NULL && g_pData->GetUIManager()->GetUIPersonalShop() != NULL &&
			g_pData->GetUIManager()->GetUIPersonalShop()->GetPersonalShopState() != CX2UIPersonalShop::XPSS_AGENCY_SELL &&
			g_pData->GetUIManager()->GetUIPersonalShop()->GetPersonalShopState() != CX2UIPersonalShop::XPSS_AGENCY_WAIT )
#else
		if(m_bEnableButtonPersonalShop == false)
#endif SERV_PSHOP_AGENCY
		{
			if(g_pData->GetUIManager()->GetUIPersonalShop() != NULL )
			{
				g_pData->GetUIManager()->GetUIPersonalShop()->Handler_EGS_LEAVE_PERSONAL_SHOP_REQ();
			}
			SetEnablePersonalShopBoardButton(true);
		}
#endif DEF_TRADE_BOARD
		// 초심자 가이드
//		if(g_pTFieldGame != NULL && g_pTFieldGame->GetNoviceGuide()->GetIsPlayGuide() == true)
//		{
//			if( g_pTFieldGame->GetNoviceGuide()->GetNowGuideStep() == CX2NoviceGuide::NGS_STEP7)
//			{
//				g_pTFieldGame->GetNoviceGuide()->SetNotifyGuide(CX2NoviceGuide::GE_COMPLETE);
//			}	
//			else if( g_pTFieldGame->GetNoviceGuide()->GetNowGuideStep() == CX2NoviceGuide::NGS_STEP2 )
//			{				
//				g_pTFieldGame->GetNoviceGuide()->SetNotifyGuide(CX2NoviceGuide::GE_NEXT);
//			}
//			else if( g_pTFieldGame->GetNoviceGuide()->GetNowGuideStep() == CX2NoviceGuide::NGS_STEP1)
//			{
//				g_pTFieldGame->GetNoviceGuide()->SetNotifyGuide(CX2NoviceGuide::GE_NEXT);
//			}	
//#if 0
//			else if( g_pTFieldGame->GetNoviceGuide()->GetNowGuideStep() == CX2NoviceGuide::NGS_STEP5)
//			{
//				g_pTFieldGame->GetNoviceGuide()->SetNotifyGuide(CX2NoviceGuide::GE_ACCEPT);
//				return 2;
//			}	
//#endif
//		}

		return 1;
	case NMUM_SHOP:
		g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU_SHOP, true, m_iHouseID);		
		return 2;
	case NMUM_PVP:		
		{
			if( NULL != g_pData->GetMyUser() &&
				NULL != g_pData->GetMyUser()->GetSelectUnit() &&
				g_pData->GetMyUser()->GetAuthLevel() < CX2User::XUAL_DEV &&
				g_pData->GetSelectUnitLevel() < 3 )
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_657 ), g_pMain->GetNowState() );
				return 2;
			}

			if(g_pTFieldGame != NULL)
				g_pTFieldGame->Handler_EGS_OPEN_PVP_ROOM_LIST_REQ();
		}
		return 2;
	case NMUM_MAKE:
		//{{ kimhc // 2009-11-23 // 제조 아이템 NPC 별로 가능하도록
#ifdef	ADD_HOUSE_ID_TO_MANUFACTURE
		g_pData->GetUIManager()->ToggleUI( CX2UIManager::UI_MENU_MANUFACTURE, true, (UidType)m_iHouseID );
#else	ADD_HOUSE_ID_TO_MANUFACTURE
		g_pData->GetUIManager()->ToggleUI( CX2UIManager::UI_MENU_MANUFACTURE, true );
#endif	ADD_HOUSE_ID_TO_MANUFACTURE
		//}} kimhc // 2009-11-23 // 제조 아이템 NPC 별로 가능하도록
		

		return 2;
	case NMUM_TRAINING:
        if( m_pDlgMessage != NULL )
        {
		    m_pDlgMessage->SetShow(false);
		    m_pDLGNpcName->SetShow(false);
		    g_pData->SetStateArg(1);
		    g_pMain->CreateStateChangeDLG( GET_STRING( STR_ID_739 ) );
		    LeaveField();
		    g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_TRAINING_SCHOOL, CX2LocationManager::HI_LOW_TRAINNING_ZONE, false );
        }//if
		return 2;
	case NMUM_FREETRAIN:
        if( m_pDlgMessage != NULL && m_pDLGNpcName != NULL )
        {
		    m_pDlgMessage->SetShow(false);
		    m_pDLGNpcName->SetShow(false);
		    g_pData->SetStateArg(2);
		    g_pMain->CreateStateChangeDLG( GET_STRING( STR_ID_755 ) );
		    if(g_pTFieldGame != NULL)
			    g_pTFieldGame->Handler_EGS_CREATE_TC_ROOM_REQ( 1 );
		    //g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_TRAINING_SCHOOL, CX2LocationManager::HI_LOW_TRAINNING_ZONE, false );		
        }//if
		return 2;
	case NMUM_QUEST:		
#ifdef SERV_EPIC_QUEST
		g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU_QUEST_NEW, true, (UidType)m_iNpcID);
#else
		g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU_QUEST_RECEIVE, true, (UidType)m_iNpcID);
#endif SERV_EPIC_QUEST
		return 2;
	case NMUM_PASS:
		UpdateQuestDescPage(m_iNowQuestDescPage+1);
		return 2;
	case NMUM_AGREE:
		if(m_iIsPlayGuide > 0)
		{
			// 가이드 진행중 수락버튼 이벤트 발생
			if( g_pData->GetUIManager()->GetShowNpcShop() == true )
			{			
				//g_pData->GetUIManager()->CloseAll();
				g_pData->GetUIManager()->CloseAllNPCDlg(); //npc UI만 닫는다.
			}
			SetShow(false);
#ifndef REFORM_NOVICE_GUIDE
			if(g_pTFieldGame != NULL && g_pTFieldGame->GetNoviceGuide() != NULL )
				g_pTFieldGame->GetNoviceGuide()->SetNotifyGuide(CX2NoviceGuide::GE_NEXT);
#endif //REFORM_NOVICE_GUIDE
		}
		return 2;
	case NMUM_COMPLETE:
		if(m_iIsPlayGuide > 0)
		{
			// 가이드 진행중 완료버튼 이벤트 발생
			if( g_pData->GetUIManager()->GetShowNpcShop() == true )
			{			
				//g_pData->GetUIManager()->CloseAll();
				g_pData->GetUIManager()->CloseAllNPCDlg(); //npc UI만 닫는다.
			}
			SetShow(false);
		}
		return 2;
		//{{ 09.03.24 태완 : 버튼 추가
	case NMUM_ENCHANT:
		{
			g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU_SHOP, true, (UidType)m_iHouseID);
			CX2State* pState = (CX2State*) g_pMain->GetNowState();
			if(pState != NULL  && pState->GetCursor() != NULL)
			{
				pState->GetCursor()->ChangeCursorState( CX2Cursor::XCS_ENCHANT );
				g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU_INVEN, true);
				g_pData->GetUIManager()->GetUIInventory()->ChangeInventoryTab( CX2Inventory::ST_EQUIP );
			}
		} return 2;
	case NMUM_ATTRIBUTE:
		{
			g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU_SHOP, true, (UidType)m_iHouseID);
			CX2State* pState = (CX2State*) g_pMain->GetNowState();
			if(pState != NULL  && pState->GetCursor() != NULL)
			{
				pState->GetCursor()->ChangeCursorState( CX2Cursor::XCS_ATTRIB_ENCHANT );
				g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU_INVEN, true);
				g_pData->GetUIManager()->GetUIInventory()->ChangeInventoryTab( CX2Inventory::ST_EQUIP );
			}

		} return 2;
		//}} 


	case NMUM_GUILD:
		{
#ifdef GUILD_BOARD
			g_pData->GetUIManager()->ToggleUI( CX2UIManager::UI_MENU_GUILD_BOARD, true, m_iHouseID );
#endif GUILD_BOARD

		} return 2;


	case NMUM_RANKING:
		{
#ifdef RANKING_INFO_UI
			g_pData->GetUIManager()->ToggleUI( CX2UIManager::UI_MENU_RANKING_INFO, true, (UidType)m_iHouseID );
#endif RANKING_INFO_UI

		} return 2;

		//{{ kimhc // 2009-07-25 // 헤니르의 시공에 추가되는 ITEM_EXCHANGE_SHOP 작업
#ifdef	ITEM_EXCHANGE_SHOP
	case NMUM_EXCHANGE:
		{
			g_pData->GetUIManager()->ToggleUI( CX2UIManager::UI_MENU_ITEM_EXCHANGE_SHOP, true, (UidType)m_iHouseID );
		} return 2;
#endif	ITEM_EXCHANGE_SHOP
		//}} kimhc // 2009-07-25 // 헤니르의 시공에 추가되는 ITEM_EXCHANGE_SHOP 작업

		//{{ kimhc // 2009-08-03 // 캐릭터별 은행
#ifdef	PRIVATE_BANK
	case NMUN_PRIVATE_BANK:
		{
			g_pData->GetUIManager()->ToggleUI( CX2UIManager::UI_MENU_PRIVATE_BANK, true );
		} return 2;

#endif	PRIVATE_BANK
		//}} kimhc // 2009-08-03 // 캐릭터별 은행

#ifdef WEB_POINT_EVENT // oasis907 : 김상윤 [2010.2.24] // 핸들러 연결
	case NMUM_WEB_EVENT:
		{
			if(g_pTFieldGame != NULL)
				g_pTFieldGame->Handler_EGS_ATTENDANCE_CHECK_REQ();
		} return 2;
#endif WEB_POINT_EVENT
#ifdef DEF_TRADE_BOARD
	case NMUM_PERSONAL_SHOP:
		{
			g_pData->GetUIManager()->ToggleUI( CX2UIManager::UI_MENU_PERSONAL_SHOP_BOARD, true, m_iHouseID );
		} return 2;
#endif DEF_TRADE_BOARD
#ifdef SERV_PSHOP_AGENCY
	case NMUM_AGENCY_TRADER_REGISTER:
		{
			if( g_pData->GetMyUser()->GetUserData().m_bIsGuestUser == true )
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(270,350), GET_STRING( STR_ID_40 ), g_pMain->GetNowState() );
			}
			else
			{
				g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU_PERSONAL_SHOP, true, CX2UIPersonalShop::OPENFORAGENCYTRADER);
			}			
		}
		return 2;
	case NMUM_AGENCY_TRADER_RECEIVE:
		{
			if( g_pData->GetMyUser()->GetUserData().m_bIsGuestUser == true )
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(270,350), GET_STRING( STR_ID_40 ), g_pMain->GetNowState() );
			}
			else
			{
				g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU_PERSONAL_SHOP, true, CX2UIPersonalShop::OPENTAKESHOPAGNECY);
			}
		}
		return 2;
#endif
		//{{ 2011.05.04   임규수 아바타 합성 시스템
#ifdef SERV_SYNTHESIS_AVATAR
	case NMUM_SYNTHESIS:
		{
			g_pData->GetUIManager()->ToggleUI( CX2UIManager::UI_MENU_SYNTHESIS, true);
		} return 2;
#endif SERV_SYNTHESIS_AVATAR
		//}}
#ifdef SERV_PVP_NEW_SYSTEM
	case NMUM_FORMAL_PVP:
		{
#ifdef PVP_UI_OVERLAP_CLOSE
			if( g_pData->GetPartyManager()->GetProcessPvpMatch() == true )
			{
#ifdef CLIENT_COUNTRY_BR
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_25384 ), static_cast<CKTDXStage*>( g_pMain->GetNowState() ) );        
#else //CLIENT_COUNTRY_BR
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_26851 ), static_cast<CKTDXStage*>( g_pMain->GetNowState() ) );        
#endif //CLIENT_COUNTRY_BR
				return 2;
			}
#endif //PVP_UI_OVERLAP_CLOSE

			if( g_pData->GetPartyManager()->DoIHaveParty() == true && g_pData->GetPartyManager()->AmIPartyLeader() == true )
			{
				if( g_pData->GetPartyManager()->GetMyPartyData()->GetPartyMemberCount() > 3 )
				{
					// 3인 초과 파티 이용 불가
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_12715 ), (CKTDXStage*)g_pMain->GetNowState() );	
					return 2;
				}
				else
				{
					for(int iParty=0; iParty<g_pData->GetPartyManager()->GetMyPartyData()->GetPartyMemberCount(); ++iParty)
					{
						if( g_pData->GetPartyManager()->GetMyPartyData()->GetPartyMemberData(iParty)->m_iUnitLevel < 10 )
						{
							// 10렙 미만인 파티원이 있어 대전파티로 변경이 불가합니다.
							g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_13548 ), (CKTDXStage*)g_pMain->GetNowState() );			
							return 2;
						}
					}
				}

				if( g_pData->GetPartyManager()->GetMyPartyData()->m_bPvpParty == false && g_pData->GetPartyManager()->ChangePartyType(true) == false )
				{
					// 대전파티 변경불가
					return 2;
				}
			}
			else
			{
				if( g_pData->GetSelectUnitLevel() < 10 )
				{
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_13549 ), (CKTDXStage*)g_pMain->GetNowState() );
					return 2;
				}
			}

			if( g_pTFieldGame != NULL )
			{
				g_pTFieldGame->ByeNpc();
			}
						
			g_pMain->GetPartyUI()->SetPvpPartyMenu(true);
			g_pMain->GetPartyUI()->OpenPartyDLG(true);
			g_pMain->GetPartyUI()->UpdatePvpPartyDlg();
		}
		return 2;
#endif //SERV_PVP_NEW_SYSTEM

#ifdef NPC_EVENT_BUTTON
	case NMUM_EVENT1:
		{
#ifdef SERV_EVENT_CHARACTER_QUEST_RANKING
			CX2State* pNowState = (CX2State*)g_pMain->GetNowState();
			pNowState->Handler_EGS_EGS_GET_EVENT_INFO_REQ();
#endif //SERV_EVENT_CHARACTER_QUEST_RANKING
#ifdef SERV_READY_TO_SOSUN_EVENT
			if( g_pTFieldGame != NULL )
			{
				g_pData->GetUIManager()->CloseAllNPCDlg();

				if( g_pData->GetMyUser() != NULL &&
					g_pData->GetMyUser()->GetSelectUnit() != NULL )
				{
					if( g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItemByTID( EVENT_READY_TO_SOSUN_ITEM_ID, false ) != NULL )
					{
						SAFE_DELETE_DIALOG( m_pDlgReadyToSosun );
						m_pDlgReadyToSosun = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Ready_To_Sosun_Event.lua" );
						ASSERT( m_pDlgReadyToSosun != NULL );
						if ( m_pDlgReadyToSosun != NULL )
							g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDlgReadyToSosun );

						m_pDlgReadyToSosun->SetShowEnable(true, true);
					}
					else
					{
						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_24607 ), (CKTDXStage*)g_pMain->GetNowState() );
					}
				}
			}
#endif SERV_READY_TO_SOSUN_EVENT
		} return 2;
	case NMUM_EVENT2:
		{

		} return 2;
	case NMUM_EVENT3:
		{

		} return 2;
	case NMUM_EVENT4:
		{

		} return 2;
#endif NPC_EVENT_BUTTON

#ifdef SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT
	case NMUM_WEDDING_EVENT:
		{
			if( NULL != g_pData && NULL != g_pData->GetUIManager() )
				g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU_WEDDING_EVENT_LETTER, true, static_cast<UidType>(m_iNpcID));
		}
		return 2;

	case NMUM_WEDDING_EVENT_SEND_LETTER:
		{
			if( NULL != g_pData && NULL != g_pData->GetUIManager() )
			{
				CX2Item* pItem = NULL;
				if( NULL != g_pData->GetMyUser() && 
					NULL != g_pData->GetMyUser()->GetSelectUnit() )
					pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItemByTID( 135404 );

				if( NULL != pItem )
				{
					g_pData->GetUIManager()->Handler_EGS_SEND_LOVE_LETTER_EVENT_REQ();
				}
				else
				{
					g_pData->GetUIManager()->CreateWeddingCaution();
				}
			}
		}
		return 2;

	case NMUM_WEDDING_EVENT_CLOSE:
		{
			if( NULL != g_pData && NULL != g_pData->GetUIManager() )
				g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU_WEDDING_EVENT_LETTER, false, static_cast<UidType>(m_iNpcID));
		}
		return 2;

	case NMUM_WEDDING_EVENT_FOCUS_LETTER:
		{
			if( NULL != g_pData && NULL != g_pData->GetUIManager() )
				g_pData->GetUIManager()->SetShowLetterGuide( false );
		}
		return 2;
#endif //SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05 // 아이템 교환 버튼
	case NMUM_NEW_ITEM_EXCHANGE:
		{		
			// 마우스 커서 변경
			CX2State* pState = static_cast<CX2State*>(g_pMain->GetNowState());
			if( NULL != pState && NULL != pState->GetCursor() )
			{
				pState->GetCursor()->ChangeCursorState( CX2Cursor::XCS_EXCHANGE_NEW_ITEM );
			}


			// 탭 변경
			if( NULL != g_pData &&
				NULL != g_pData->GetUIManager() &&
				NULL != g_pData->GetUIManager()->GetUIInventory() )
			{
				g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU_INVEN, true);
				g_pData->GetUIManager()->GetUIInventory()->ChangeInventoryTab( CX2Inventory::ST_EQUIP );
			}
		} return 2;
#endif // SERV_NEW_ITEM_SYSTEM_2013_05
	}

	return 0;
}

bool CX2TFieldNpcShop::UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{	
	return false;
}

void CX2TFieldNpcShop::LeaveField()
{	
	//g_pData->GetServerProtocol()->SendID( EGS_STATE_CHANGE_TC_LOBBY_NOT );
	g_pData->GetServerProtocol()->SendID( EGS_LEAVE_FIELD_NOT );
}

void CX2TFieldNpcShop::SetQuestDesc(const wstring &title, const wstring &message)
{
	m_wstrQuestTitle = title;
	UpdateQuestDesc(message);
	UpdateQuestDescPage(1);
}

void CX2TFieldNpcShop::UpdateQuestDesc( const wstring& wstrDesc )
{
	enum CHAR_STATE
	{
		Q_NORMAL,
		Q_SHARP,
		Q_SHARP_C,
	};

	m_vQuestDescSize = D3DXVECTOR2( (float) m_pDlgMessage->GetDummyInt( 0 ), (float) m_pDlgMessage->GetDummyInt( 1 ) );
	m_vQuestDescSize = g_pKTDXApp->ConvertByResolution( m_vQuestDescSize );

	CKTDGUIStatic* pStatic_QuestDesc = (CKTDGUIStatic*) m_pDlgMessage->GetControl( L"npcMessage" );
	if( NULL == pStatic_QuestDesc )
		return;

	CKTDGFontManager::CUKFont* pFont 
		= g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( pStatic_QuestDesc->GetString(0)->fontIndex );
	if( NULL == pFont )
		return; 


	const int MAGIC_MAX_ROW = (int) (m_vQuestDescSize.y / (float ) pFont->GetHeight() * 0.5f);
	const int MAGIC_MAX_WIDTH = (int) m_vQuestDescSize.x;

	m_vecQuestDesc.resize(0);

	if( wstrDesc.length() == 0 )
	{
		m_iNowQuestDescPage = 0;
		m_iMaxQuestDescPage = 0;
		return;
	}


	int iNowCharWidth = 0;		
	int iNowLineWidth = 0;
	int iNowLineCount = 0;
	int iNowCharIndex = 0;
	wstring wstrLineBuf = L"";
	wstring wstrPageBuf = L"";
	wstring wstrColorCode = L"";

	CHAR_STATE eNowCharState = Q_NORMAL;
	bool bIsFirstCharOfLine = true; 

	int iDescLength = (int) wstrDesc.length();
	while( iNowCharIndex < iDescLength )
	{
		WCHAR wszCharBuf = wstrDesc[iNowCharIndex];
		wstring wstrCharBuf = wstrDesc.substr( iNowCharIndex, 1 ); 

		switch( eNowCharState )
		{
		case Q_NORMAL:
			{
				if( 0 == wstrCharBuf.compare( L"#" ) )
				{
					wstrLineBuf += wstrCharBuf;
					eNowCharState = Q_SHARP;
				}
				else if( 0 == wstrCharBuf.compare( L"\n" ) )
				{
					wstrLineBuf += wstrCharBuf;
					wstrPageBuf += wstrLineBuf;
					iNowLineCount++;
					wstrLineBuf = L"";
					iNowLineWidth = 0;
					bIsFirstCharOfLine = true;

					if( 0 != wstrColorCode.length() )
					{
						wstrLineBuf += wstrColorCode;
					}

					if( iNowLineCount >= MAGIC_MAX_ROW )
					{
						m_vecQuestDesc.push_back( wstrPageBuf );
						wstrPageBuf = L"";
						iNowLineCount = 0;
					}
				}
				else
				{
					if( 0 != wstrCharBuf.compare( L" " ) || false == bIsFirstCharOfLine )
					{
						if( true == bIsFirstCharOfLine )
							bIsFirstCharOfLine = false;
						wstrLineBuf += wstrCharBuf;					
						iNowCharWidth = pFont->GetWidth( wszCharBuf );
						iNowLineWidth += iNowCharWidth;
					}
				}

			} break;

		case Q_SHARP:
			{
				if( 0 == wstrCharBuf.compare( L"C" ) )
				{
					wstrLineBuf += wstrCharBuf;
					eNowCharState = Q_SHARP_C;
				}
				else if( 0 == wstrCharBuf.compare( L"m" ) )
				{
					wstrLineBuf.erase( wstrLineBuf.end()-1 ); // '#'을 지우자
					wstrPageBuf += wstrLineBuf;
					m_vecQuestDesc.push_back( wstrPageBuf );
					wstrLineBuf = L"";
					wstrPageBuf = L"";
					iNowLineCount = 0;
					bIsFirstCharOfLine = true;
					if( 0 != wstrColorCode.length() )
					{
						wstrLineBuf += wstrColorCode;
					}

					eNowCharState = Q_NORMAL;
				}
				else
				{
					wstrLineBuf += wstrCharBuf;
					iNowCharWidth = pFont->GetWidth( wszCharBuf );
					iNowLineWidth += iNowCharWidth;

					eNowCharState = Q_NORMAL;
				}

			} break;

		case Q_SHARP_C:
			{
				if( 0 == wstrCharBuf.compare( L"X" ) )
				{
					wstrLineBuf += wstrCharBuf;
					wstrColorCode = L"";
					eNowCharState = Q_NORMAL;
				}
				else // RGB 
				{
					wstrColorCode = L"#C";
					wstrColorCode += wstrDesc.substr( iNowCharIndex, 6 );
					wstrLineBuf += wstrDesc.substr( iNowCharIndex, 6 );
					iNowCharIndex += 5;
					eNowCharState = Q_NORMAL;
				}

			} break;
		}

		if( iNowLineWidth >= MAGIC_MAX_WIDTH && Q_NORMAL == eNowCharState )
		{
			wstrLineBuf += L"\n";
			wstrPageBuf += wstrLineBuf;
			iNowLineCount++;
			wstrLineBuf = L"";
			iNowLineWidth = 0;
			bIsFirstCharOfLine = true;
			if( 0 != wstrColorCode.length() )
			{
				wstrLineBuf += wstrColorCode;
			}

			if( iNowLineCount >= MAGIC_MAX_ROW )
			{
				m_vecQuestDesc.push_back( wstrPageBuf );
				wstrPageBuf = L"";
				iNowLineCount = 0;
			}
		}

		iNowCharIndex++;
	}

	if( wstrLineBuf.length() > 0 )
	{
		wstrPageBuf += wstrLineBuf;
	}

	if( wstrPageBuf.length() > 0 )
	{
		m_vecQuestDesc.push_back( wstrPageBuf );
	}

	m_iMaxQuestDescPage = (int) m_vecQuestDesc.size();
	m_iNowQuestDescPage = 1;

}

void CX2TFieldNpcShop::UpdateQuestDescPage( int iPage )
{

	if( m_vecQuestDesc.size() == 0 )
	{
		m_iNowQuestDescPage = 1;

		SetHouseName(L"");
		SetNpcMessage(L"");
	}
	else
	{
		if( iPage > m_iMaxQuestDescPage )
			return;

		m_iNowQuestDescPage = iPage;

		SetHouseName(m_wstrQuestTitle);
		SetNpcMessage(m_vecQuestDesc[ m_iNowQuestDescPage-1 ]);
		/*pStaticShopName1->GetString(0)->msg	= m_wstrQuestTitle;
		pStaticShopName2->GetString(0)->msg	= m_wstrQuestTitle;
		pStaticShopmessage->GetString(0)->msg	= m_vecQuestDesc[ m_iNowQuestDescPage-1 ];*/
	}

	WCHAR wszText[32] = {0,};
	//wsprintf( wszText, L"%d/%d", m_iNowQuestDescPage, m_iMaxQuestDescPage );
	StringCchPrintf( wszText, 32, L"%d/%d", m_iNowQuestDescPage, m_iMaxQuestDescPage );

	//pStatic_PageNum->GetString(0)->msg = wszText;
	
	m_pButtonPass->SetShowEnable( true, true );
	m_pButtonWindowPass->SetShowEnable(true, true);

	m_pButtonPass->RequestFocus();
#ifndef SERV_EPIC_QUEST
	if( m_iNowQuestDescPage == (int) m_vecQuestDesc.size() )
	{
		m_pButtonPass->SetShowEnable(false, false);
		m_pButtonWindowPass->SetShowEnable(false, false);

		if(true == g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_QUEST_RECEIVE))
		{
			g_pData->GetUIManager()->GetUIQuestReceive()->ShowNextButton(false);	// 09.02.05 김태완 : 퀘스트 대사시 Next 버튼 처리
		}
		
	}
	else
	{
		if(true == g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_QUEST_RECEIVE))
		{
			g_pData->GetUIManager()->GetUIQuestReceive()->ShowNextButton(true);		// 09.02.05 김태완 : 퀘스트 대사시 Next 버튼 처리
		}		
	}
#endif SERV_EPIC_QUEST

#if 0 
	

	CKTDGUIButton* pButton_Accept = (CKTDGUIButton*) m_pDLGQuestDesc->GetControl( L"ButtonQuest_Contents_Agree" );
	CKTDGUIButton* pButton_Complete = (CKTDGUIButton*) m_pDLGQuestDesc->GetControl( L"Quest_Complete" ); 
	CKTDGUIButton* pButton_GiveUp = (CKTDGUIButton*) m_pDLGQuestDesc->GetControl( L"ButtonQuest_Contents_GiveUp" ); 
	CKTDGUIButton* pButton_Next = (CKTDGUIButton*) m_pDLGQuestDesc->GetControl( L"Quest_Contents_Right_Arrow" );
	CKTDGUIButton* pButton_Prev = (CKTDGUIButton*) m_pDLGQuestDesc->GetControl( L"Quest_Contents_Left_Arrow" );


	pButton_Accept->SetShowEnable( false, false );
	pButton_Next->SetShowEnable( false, false );
	pButton_Prev->SetShowEnable( false, false );
	pButton_Complete->SetShowEnable( false, false );
	pButton_GiveUp->SetShowEnable( false, false );


	pButton_Prev->SetShowEnable( true, true );
	pButton_Next->SetShowEnable( true, true );
	if( m_iNowQuestDescPage == (int) m_vecQuestDesc.size() )
	{
		if( NULL != pQuestInst )
		{
			if( true == pQuestInst->IsComplete() && pQuestTemplet->m_eEndNPCID == m_eNPCID )
			{
				pButton_Complete->SetShowEnable( true, true );
			}
			else
			{
				pButton_GiveUp->SetShowEnable( true, true );
			}
		}
		else
		{
			pButton_Accept->SetShowEnable( true, true );
		}
	}
#endif
}


void CX2TFieldNpcShop::SetKeyEvent()
{	
	if( m_pButtonOut->GetShow() ||
		m_pButtonAgree->GetShow() ||
		m_pButtonComplete->GetShow() )
		return;

#ifdef SERV_EPIC_QUEST
	if(true == g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_QUEST_NEW))
		return;

#else
	if(true == g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_QUEST_RECEIVE))
		return;
#endif SERV_EPIC_QUEST
	// 가장 위(왼쪽) 에 있는 버튼이 포커스를 갖게 하기 위함.
	bool bGetFocus = false;

	// Default / NextTab
	for(vector<wstring>::iterator it = m_vButtonNameList.begin(); it<m_vButtonNameList.end(); ++it)
	{
		wstring wstrButtonName = *it;

		CKTDGUIButton* pButton = (CKTDGUIButton*) m_pDlgMessage->GetControl( wstrButtonName.c_str() );
		if( NULL == pButton )
			continue;

		if(pButton->GetShow())
		{
			if(bGetFocus == false)
			{
				pButton->RequestFocus();
				pButton->OnFocusIn();
				m_pDlgMessage->RequestFocus(pButton);
				bGetFocus = true;
			}

			// 오른쪽으로 누르면 뭐가 될지?
			for(vector<wstring>::iterator itfront = it+1; itfront<m_vButtonNameList.end(); ++itfront)
			{
				wstring wstrNextButtonName = *itfront;
				CKTDGUIButton* pButtonNext = (CKTDGUIButton*) m_pDlgMessage->GetControl( wstrNextButtonName.c_str() );
				if( NULL != pButtonNext &&
					pButtonNext->GetShow())
				{
					pButton->SetTabControlName( wstrNextButtonName.c_str() );
					break;
				}
			}

			// 왼쪽으로 누르면 뭐가 될지?
			for(vector<wstring>::iterator itBack = it-1; itBack >= m_vButtonNameList.begin(); --itBack)
			{
				wstring wstrPrevButtonName = *itBack;
				CKTDGUIButton* pButtonBack = (CKTDGUIButton*) m_pDlgMessage->GetControl( wstrPrevButtonName.c_str() );
				if( NULL != pButtonBack &&
					pButtonBack->GetShow())
				{
					pButton->SetPrevTabControlName( wstrPrevButtonName.c_str() );
					break;
				}
			}
		}

	}
// 	
// 	if(m_pButtonQuest->GetShow())
// 	{
// 		if(bGetFocus == false)
// 		{
// 			m_pButtonQuest->RequestFocus();
// 			m_pButtonQuest->OnFocusIn();
// 			m_pDlgMessage->RequestFocus(m_pButtonQuest);
// 			bGetFocus = true;
// 		}
// 
// 		if(m_pButtonTraining->GetShow())
// 		{
// 			m_pButtonQuest->SetTabControlName(L"training");
// 		}
// 		else if(m_pButtonFreeTraining->GetShow())
// 		{
// 			m_pButtonQuest->SetTabControlName(L"free_training");
// 		}
// 		else if(m_pButtonPvp->GetShow())
// 		{
// 			m_pButtonQuest->SetTabControlName(L"pvp");
// 		}
// 		else if(m_pButtonEnchant->GetShow())
// 		{
// 			m_pButtonQuest->SetTabControlName(L"Upgrade");
// 		}
// 		else if(m_pButtonAttribute->GetShow())
// 		{
// 			m_pButtonQuest->SetTabControlName(L"Attribute");
// 		}
// 		else if(m_pButtonMake->GetShow())
// 		{
// 			m_pButtonQuest->SetTabControlName(L"making");
// 		}
// 		else if(m_pButtonShop->GetShow())
// 		{
// 			m_pButtonQuest->SetTabControlName(L"shop");
// 		}
// 	}
// 	if(m_pButtonTraining->GetShow())
// 	{
// 		if(bGetFocus == false)
// 		{
// 			m_pButtonTraining->RequestFocus();
// 			m_pButtonTraining->OnFocusIn();
// 			m_pDlgMessage->RequestFocus(m_pButtonTraining);
// 			bGetFocus = true;
// 		}
// 
// 		if(m_pButtonFreeTraining->GetShow())
// 		{
// 			m_pButtonTraining->SetTabControlName(L"free_training");
// 		}
// 		else if(m_pButtonPvp->GetShow())
// 		{
// 			m_pButtonTraining->SetTabControlName(L"pvp");
// 		}
// 		else if(m_pButtonEnchant->GetShow())
// 		{
// 			m_pButtonTraining->SetTabControlName(L"Upgrade");
// 		}
// 		else if(m_pButtonAttribute->GetShow())
// 		{
// 			m_pButtonTraining->SetTabControlName(L"Attribute");
// 		}
// 		else if(m_pButtonMake->GetShow())
// 		{
// 			m_pButtonTraining->SetTabControlName(L"making");
// 		}
// 		else if(m_pButtonShop->GetShow())
// 		{
// 			m_pButtonTraining->SetTabControlName(L"shop");
// 		}
// 	}
// 	if(m_pButtonFreeTraining->GetShow())
// 	{
// 		if(bGetFocus == false)
// 		{
// 			m_pButtonFreeTraining->RequestFocus();
// 			m_pButtonFreeTraining->OnFocusIn();
// 			m_pDlgMessage->RequestFocus(m_pButtonFreeTraining);
// 			bGetFocus = true;
// 		}
// 
// 		if(m_pButtonPvp->GetShow())
// 		{
// 			m_pButtonFreeTraining->SetTabControlName(L"pvp");
// 		}
// 		else if(m_pButtonEnchant->GetShow())
// 		{
// 			m_pButtonFreeTraining->SetTabControlName(L"Upgrade");
// 		}
// 		else if(m_pButtonAttribute->GetShow())
// 		{
// 			m_pButtonFreeTraining->SetTabControlName(L"Attribute");
// 		}
// 		else if(m_pButtonMake->GetShow())
// 		{
// 			m_pButtonFreeTraining->SetTabControlName(L"making");
// 		}
// 		else if(m_pButtonShop->GetShow())
// 		{
// 			m_pButtonFreeTraining->SetTabControlName(L"shop");
// 		}
// 	}
// 	if(m_pButtonPvp->GetShow())
// 	{
// 		if(bGetFocus == false)
// 		{
// 			m_pButtonPvp->RequestFocus();
// 			m_pButtonPvp->OnFocusIn();
// 			m_pDlgMessage->RequestFocus(m_pButtonPvp);
// 			bGetFocus = true;
// 		}
// 
// 		if(m_pButtonEnchant->GetShow())
// 		{
// 			m_pButtonPvp->SetTabControlName(L"Upgrade");
// 		}
// 		else if(m_pButtonAttribute->GetShow())
// 		{
// 			m_pButtonPvp->SetTabControlName(L"Attribute");
// 		}
// 		else if(m_pButtonMake->GetShow())
// 		{
// 			m_pButtonPvp->SetTabControlName(L"making");
// 		}
// 		else if(m_pButtonShop->GetShow())
// 		{
// 			m_pButtonPvp->SetTabControlName(L"shop");
// 		}
// 
// 	}
// 	if(m_pButtonEnchant->GetShow())
// 	{
// 		if(bGetFocus == false)
// 		{
// 			m_pButtonEnchant->RequestFocus();
// 			m_pButtonEnchant->OnFocusIn();
// 			m_pDlgMessage->RequestFocus(m_pButtonEnchant);
// 			bGetFocus = true;
// 		}
// 
// 		if(m_pButtonAttribute->GetShow())
// 		{
// 			m_pButtonEnchant->SetTabControlName(L"Attribute");
// 		}
// 		else if(m_pButtonMake->GetShow())
// 		{
// 			m_pButtonEnchant->SetTabControlName(L"making");
// 		}
// 		else if(m_pButtonShop->GetShow())
// 		{
// 			m_pButtonEnchant->SetTabControlName(L"shop");
// 		}
// 
// 	}
// 	if(m_pButtonAttribute->GetShow())
// 	{
// 		if(bGetFocus == false)
// 		{
// 			m_pButtonAttribute->RequestFocus();
// 			m_pButtonAttribute->OnFocusIn();
// 			m_pDlgMessage->RequestFocus(m_pButtonAttribute);
// 			bGetFocus = true;
// 		}
// 
// 		if(m_pButtonMake->GetShow())
// 		{
// 			m_pButtonAttribute->SetTabControlName(L"making");
// 		}
// 		else if(m_pButtonShop->GetShow())
// 		{
// 			m_pButtonAttribute->SetTabControlName(L"shop");
// 		}
// 
// 	}
// 	if(m_pButtonMake->GetShow())
// 	{
// 		if(bGetFocus == false)
// 		{
// 			m_pButtonMake->RequestFocus();
// 			m_pButtonMake->OnFocusIn();
// 			m_pDlgMessage->RequestFocus(m_pButtonMake);
// 			bGetFocus = true;
// 		}
// 
// 		if(m_pButtonShop->GetShow())
// 		{
// 			m_pButtonMake->SetTabControlName(L"shop");
// 		}
// 	}
// 	if(m_pButtonShop->GetShow())
// 	{
// 		if(bGetFocus == false)
// 		{
// 			m_pButtonShop->RequestFocus();
// 			m_pButtonShop->OnFocusIn();
// 			m_pDlgMessage->RequestFocus(m_pButtonShop);		
// 			bGetFocus = true;
// 		}
// 		m_pButtonShop->SetTabControlName(L"");
// 	}
// 
// 	
// 	// PrevTab
// 	if(m_pButtonShop->GetShow())
// 	{		
// 		if(m_pButtonMake->GetShow())
// 		{
// 			m_pButtonShop->SetPrevTabControlName(L"making");
// 		}
// 		else if(m_pButtonAttribute->GetShow())
// 		{
// 			m_pButtonShop->SetPrevTabControlName(L"Attribute");
// 		}
// 		else if(m_pButtonEnchant->GetShow())
// 		{
// 			m_pButtonShop->SetPrevTabControlName(L"Upgrade");
// 		}
// 		else if(m_pButtonPvp->GetShow())
// 		{
// 			m_pButtonShop->SetPrevTabControlName(L"pvp");
// 		}
// 		else if(m_pButtonFreeTraining->GetShow())
// 		{
// 			m_pButtonShop->SetPrevTabControlName(L"free_training");
// 		}
// 		else if(m_pButtonTraining->GetShow())
// 		{
// 			m_pButtonShop->SetPrevTabControlName(L"training");
// 		}
// 		else if(m_pButtonQuest->GetShow())
// 		{
// 			m_pButtonShop->SetPrevTabControlName(L"quest");
// 		}
// 		
// 	}
// 	if(m_pButtonMake->GetShow())
// 	{
// 		if(m_pButtonAttribute->GetShow())
// 		{
// 			m_pButtonMake->SetPrevTabControlName(L"Attribute");
// 		}
// 		else if(m_pButtonEnchant->GetShow())
// 		{
// 			m_pButtonMake->SetPrevTabControlName(L"Upgrade");
// 		}
// 		else if(m_pButtonPvp->GetShow())
// 		{
// 			m_pButtonMake->SetPrevTabControlName(L"pvp");
// 		}
// 		else if(m_pButtonFreeTraining->GetShow())
// 		{
// 			m_pButtonMake->SetPrevTabControlName(L"free_training");
// 		}
// 		else if(m_pButtonTraining->GetShow())
// 		{
// 			m_pButtonMake->SetPrevTabControlName(L"training");
// 		}
// 		else if(m_pButtonQuest->GetShow())
// 		{
// 			m_pButtonMake->SetPrevTabControlName(L"quest");
// 		}
// 	}
// 	if(m_pButtonAttribute->GetShow())
// 	{
// 		if(m_pButtonEnchant->GetShow())
// 		{
// 			m_pButtonAttribute->SetPrevTabControlName(L"Upgrade");
// 		}
// 		else if(m_pButtonPvp->GetShow())
// 		{
// 			m_pButtonAttribute->SetPrevTabControlName(L"pvp");
// 		}
// 		else if(m_pButtonFreeTraining->GetShow())
// 		{
// 			m_pButtonAttribute->SetPrevTabControlName(L"free_training");
// 		}
// 		else if(m_pButtonTraining->GetShow())
// 		{
// 			m_pButtonAttribute->SetPrevTabControlName(L"training");
// 		}
// 		else if(m_pButtonQuest->GetShow())
// 		{
// 			m_pButtonAttribute->SetPrevTabControlName(L"quest");
// 		}
// 
// 	}
// 	if(m_pButtonEnchant->GetShow())
// 	{
// 		if(m_pButtonPvp->GetShow())
// 		{
// 			m_pButtonEnchant->SetPrevTabControlName(L"pvp");
// 		}
// 		else if(m_pButtonFreeTraining->GetShow())
// 		{
// 			m_pButtonEnchant->SetPrevTabControlName(L"free_training");
// 		}
// 		else if(m_pButtonTraining->GetShow())
// 		{
// 			m_pButtonEnchant->SetPrevTabControlName(L"training");
// 		}
// 		else if(m_pButtonQuest->GetShow())
// 		{
// 			m_pButtonEnchant->SetPrevTabControlName(L"quest");
// 		}
// 
// 	}
// 	if(m_pButtonPvp->GetShow())
// 	{
// 		if(m_pButtonFreeTraining->GetShow())
// 		{
// 			m_pButtonPvp->SetPrevTabControlName(L"free_training");
// 		}
// 		else if(m_pButtonTraining->GetShow())
// 		{
// 			m_pButtonPvp->SetPrevTabControlName(L"training");
// 		}
// 		else if(m_pButtonQuest->GetShow())
// 		{
// 			m_pButtonPvp->SetPrevTabControlName(L"quest");
// 		}
// 		
// 	}
// 	if(m_pButtonFreeTraining->GetShow())
// 	{
// 		if(m_pButtonTraining->GetShow())
// 		{
// 			m_pButtonFreeTraining->SetPrevTabControlName(L"training");
// 		}
// 		else if(m_pButtonQuest->GetShow())
// 		{
// 			m_pButtonFreeTraining->SetPrevTabControlName(L"quest");
// 		}
// 		
// 	}
// 	if(m_pButtonTraining->GetShow())
// 	{
// 		if(m_pButtonQuest->GetShow())
// 		{
// 			m_pButtonTraining->SetPrevTabControlName(L"quest");
// 		}
// 		
// 	}	
// 	if(m_pButtonQuest->GetShow())
// 	{
// 		m_pButtonQuest->SetPrevTabControlName(L"");
// 	}
}

#ifdef DEF_TRADE_BOARD
void CX2TFieldNpcShop::SetEnablePersonalShopBoardButton(bool bEnable)
{
	if ( NULL != m_pButtonPersonalShop)
	{

		m_bEnableButtonPersonalShop = bEnable;

	}
	return;
}


#endif DEF_TRADE_BOARD



