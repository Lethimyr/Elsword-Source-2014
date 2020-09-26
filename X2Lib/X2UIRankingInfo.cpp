//{{ kimhc // 2009-07-09 // 헤니르의 시공에 추가되는 Ranking UI 작업
#include "StdAfx.h"

#ifdef	RANKING_INFO_UI

#include ".\X2UIRankingInfo.h"
#include < iomanip >

CX2UIRankingInfo::CX2UIRankingInfo( const CKTDXStage* const pNowStage, const WCHAR* const pFileName ) :
m_bShow( false ),
m_pNowStage( pNowStage ),
m_pDlgRankingInfo( NULL ),
m_nowRankingInfoType( RT_DAY_RANKING ),
m_uiNowPage( 1 ),
m_uiMaxPage( 1 ),
m_vPosDlg( 0, 0 ),
m_vSizeDlg( 0, 0 ),
m_pStaticCommonInfo( NULL ),
m_pStaticHenirInfo( NULL ),
m_pStaticPvpInfo( NULL ),
m_pStaticLevelInfo( NULL ),
m_pRadioButtonDaily( NULL ),
m_pRadioButtonWeekly( NULL ),
m_pRadioButtonMonthly( NULL )
#ifdef SERV_LOCAL_RANKING_SYSTEM
,Is_Filter_Check( 0 ),
m_pStaticDnFd( NULL ),
m_pStaticWeekPvp( NULL ),
m_pDlgRankingPopUp( NULL ),
m_nowRankingInfoTypeSub( RT_AREA ),
m_IMEEditPageSearch( NULL ),
m_pStaticNotice( NULL )
#endif //SERV_LOCAL_RANKING_SYSTEM
//{{ 허상형 : [2009/8/21] //	신학기 이벤트 관련 컨트롤 변수 초기화
#ifdef NEW_TERM_EVENT
,
m_pStaticEventInfo( NULL ),
m_pRadioButtonEventHenir( NULL ),
m_pRadioButtonEventDungeon( NULL ),
m_pRadioButtonEventKill( NULL ),
m_pRadioButtonEventDeath( NULL )
#endif	//	NEW_TERM_EVENT
//}} 허상형 : [2009/8/21] //	신학기 이벤트 관련 컨트롤 변수 초기화
//,m_pPicCharRank( NULL )
#ifdef NEW_HENIR_DUNGEON
, m_pRadioButtonHero (NULL)
#endif // NEW_HENIR_DUNGEON
{


	m_pDlgRankingInfo		=		new CKTDGUIDialog( const_cast< CKTDXStage* >( pNowStage ), pFileName );
		
	if ( m_pDlgRankingInfo == NULL )
	{
		ASSERT( false );
		return;
	}

	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDlgRankingInfo );
	m_pDlgRankingInfo->SetDisableUnderWindow( true );

	m_vPosDlg.x			= m_pDlgRankingInfo->GetDummyPos( 0 ).x;		// 0은 posion
	m_vPosDlg.y			= m_pDlgRankingInfo->GetDummyPos( 0 ).y;
	
	m_vSizeDlg.x		= m_pDlgRankingInfo->GetDummyPos( 1 ).x;		// 1은 size
	m_vSizeDlg.y		= m_pDlgRankingInfo->GetDummyPos( 1 ).y;

	// 모든 타입정보에 다 쓰이는 컨트롤
	m_pStaticCommonInfo				= static_cast< CKTDGUIStatic* >( m_pDlgRankingInfo->GetControl( L"g_pCommonStatic" ) );

	if ( m_pStaticCommonInfo != NULL )
	{
		m_pStaticCommonInfo->SetString( 0, GET_STRING( STR_ID_1139 ) );		// 순위
		m_pStaticCommonInfo->SetString( 1, GET_STRING( STR_ID_3727 ) );		// 직업
		m_pStaticCommonInfo->SetString( 2, GET_STRING( STR_ID_1127 ) );		// 닉네임
	}

	

	// 헤니르시공 랭킹 정보에 쓰이는 컨트롤
	m_pStaticHenirInfo		= static_cast< CKTDGUIStatic* >( m_pDlgRankingInfo->GetControl( L"g_pHenirStatic" ) );
	m_pRadioButtonDaily		= static_cast< CKTDGUIRadioButton* >( m_pDlgRankingInfo->GetControl( L"RadioButton_Day" ) );
	m_pRadioButtonWeekly	= static_cast< CKTDGUIRadioButton* >( m_pDlgRankingInfo->GetControl( L"RadioButton_Week" ) );
	m_pRadioButtonMonthly	= static_cast< CKTDGUIRadioButton* >( m_pDlgRankingInfo->GetControl( L"RadioButton_Month" ) );
#ifdef NEW_HENIR_DUNGEON
	m_pRadioButtonHero	= static_cast< CKTDGUIRadioButton* >( m_pDlgRankingInfo->GetControl( L"RadioButton_Hero" ) );
#endif // NEW_HENIR_DUNGEON

	//{{ 허상형 : [2009/8/20] //	신학기 관련 컨트롤 로딩
#ifdef NEW_TERM_EVENT
	m_pStaticEventInfo			= static_cast< CKTDGUIStatic* >( m_pDlgRankingInfo->GetControl( L"g_pStaticScore" ) );
	m_pRadioButtonEventHenir	= static_cast< CKTDGUIRadioButton* >( m_pDlgRankingInfo->GetControl( L"RadioButton_EventHenir" ) );
	m_pRadioButtonEventDungeon	= static_cast< CKTDGUIRadioButton* >( m_pDlgRankingInfo->GetControl( L"RadioButton_EventDungeon" ) );
	m_pRadioButtonEventKill		= static_cast< CKTDGUIRadioButton* >( m_pDlgRankingInfo->GetControl( L"RadioButton_EventKill" ) );
	m_pRadioButtonEventDeath	= static_cast< CKTDGUIRadioButton* >( m_pDlgRankingInfo->GetControl( L"RadioButton_EventDeath" ) );
#endif
	//}} 허상형 : [2009/8/20] //	신학기 관련 컨트롤 로딩

#ifdef HIDE_HENIR_RANKING
	if ( m_pStaticHenirInfo != NULL )
	{
		m_pStaticHenirInfo->SetShowEnable(false, false);
	}
	if (m_pRadioButtonDaily != NULL)
	{
		m_pRadioButtonDaily->SetShowEnable(false, false);
	}
	if (m_pRadioButtonWeekly != NULL)
	{
		m_pRadioButtonWeekly->SetShowEnable(false, false);
	}
	if (m_pRadioButtonMonthly != NULL)
	{
		m_pRadioButtonMonthly->SetShowEnable(false, false);
	}
#endif //HIDE_HENIR_RANKING

	if ( m_pStaticHenirInfo != NULL )
	{
		m_pStaticHenirInfo->SetString( 0, GET_STRING( STR_ID_3728 ) );		// 스테이지/남은시간
	}

	// 대전 랭킹 정보에 쓰이는 컨트롤
	m_pStaticPvpInfo = static_cast< CKTDGUIStatic* >( m_pDlgRankingInfo->GetControl( L"g_pPvpStatic" ) );

	if ( m_pStaticPvpInfo != NULL )
	{
		m_pStaticPvpInfo->SetString( 0, GET_STRING( STR_ID_3729) );		// 대전랭크
		m_pStaticPvpInfo->SetString( 1, GET_STRING( STR_ID_3730) );		// 승/패
	}

	// 레벨(경험치) 랭킹 정보에 쓰이는 컨트롤
	m_pStaticLevelInfo = static_cast< CKTDGUIStatic* >( m_pDlgRankingInfo->GetControl( L"g_pStaticExp" ) );

	if ( m_pStaticLevelInfo != NULL )
	{
		m_pStaticLevelInfo->SetString( 0, GET_STRING( STR_ID_3731) );	// EXP
	}

//{{ 허상형 : [2009/8/21] //	신학기 이벤트 스태틱 컨트롤 설정
#ifdef NEW_TERM_EVENT
	if( m_pStaticEventInfo != NULL )
	{
		m_pStaticEventInfo->SetString( 0, GET_STRING( STR_ID_4413) );	//	점수
	}

#endif	//	NEW_TERM_EVENT
//}} 허상형 : [2009/8/21] //	신학기 이벤트 스태틱 컨트롤 설정


#ifdef SERV_LOCAL_RANKING_SYSTEM
	m_pDlgRankingPopUp = new CKTDGUIDialog( const_cast< CKTDXStage* >( pNowStage ), L"DLG_Ranking_PopUp.lua" );
	if( NULL != m_pDlgRankingPopUp )
	{
		m_pDlgRankingPopUp->SetShowEnable( false, false );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDlgRankingPopUp );
		m_pDlgRankingPopUp->SetDisableUnderWindow( true );
	}

	//페이지 찾기 IME
	m_IMEEditPageSearch = static_cast<CKTDGUIIMEEditBox* >( m_pDlgRankingInfo->GetControl( L"PageSearch" ) );
	if( NULL != m_IMEEditPageSearch )
	{
		m_IMEEditPageSearch->SetText( GET_STRING( STR_ID_23653 ) ); // 검색
	}

	//던전&필드 , 주간대전 에서 공통적으로 쓰이는 세부 항목
	m_sCommonMenu.m_pStaticCommonInfo = static_cast<CKTDGUIStatic* >( m_pDlgRankingInfo->GetControl( L"CommonF" ) );
	if( NULL != m_sCommonMenu.m_pStaticCommonInfo )
	{
		if( NULL != m_sCommonMenu.m_pStaticCommonInfo->GetString( 0 ) )
			m_sCommonMenu.m_pStaticCommonInfo->SetString( 0, GET_STRING( STR_ID_1139 ) ); //순위
		if( NULL != m_sCommonMenu.m_pStaticCommonInfo->GetString( 1 ) )
			m_sCommonMenu.m_pStaticCommonInfo->SetString( 1, GET_STRING( STR_ID_3727 ) ); //직업
		if( NULL != m_sCommonMenu.m_pStaticCommonInfo->GetString( 2 ) )
			m_sCommonMenu.m_pStaticCommonInfo->SetString( 2, GET_STRING( STR_ID_1195 ) ); //성별
		if( NULL != m_sCommonMenu.m_pStaticCommonInfo->GetString( 3 ) )
			m_sCommonMenu.m_pStaticCommonInfo->SetString( 3, GET_STRING( STR_ID_1127 ) ); //닉네임
		if( NULL != m_sCommonMenu.m_pStaticCommonInfo->GetString( 4 ) )
			m_sCommonMenu.m_pStaticCommonInfo->SetString( 4, GET_STRING( STR_ID_23970 ) ); //친구
		if( NULL != m_sCommonMenu.m_pStaticCommonInfo->GetString( 5 ) )
			m_sCommonMenu.m_pStaticCommonInfo->SetString( 5, GET_STRING( STR_ID_3727 ) ); //직업
		if( NULL != m_sCommonMenu.m_pStaticCommonInfo->GetString( 6 ) )
			m_sCommonMenu.m_pStaticCommonInfo->SetString( 6, GET_STRING( STR_ID_535 ) ); //남
		if( NULL != m_sCommonMenu.m_pStaticCommonInfo->GetString( 7 ) )
			m_sCommonMenu.m_pStaticCommonInfo->SetString( 7, GET_STRING( STR_ID_536 ) ); //여
		if( NULL != m_sCommonMenu.m_pStaticCommonInfo->GetPicture( 0 ) )
			m_sCommonMenu.m_pStaticCommonInfo->GetPicture( 0 )->SetShow( true );

		m_sCommonMenu.m_pStaticCommonInfo->SetShowEnable( false, false );
	}

	m_sCommonMenu.m_pRadioArea = static_cast<CKTDGUIRadioButton* >( m_pDlgRankingInfo->GetControl( L"category_area" ) );
	if( NULL != m_sCommonMenu.m_pRadioArea )
		m_sCommonMenu.m_pRadioArea->SetShowEnable( false, false );

	m_sCommonMenu.m_pRadioGuild = static_cast<CKTDGUIRadioButton* >( m_pDlgRankingInfo->GetControl( L"category_guild" ) );
	if( NULL != m_sCommonMenu.m_pRadioGuild )
	{
		m_sCommonMenu.m_pRadioGuild->SetShowEnable( false, false );
		m_sCommonMenu.m_pRadioGuild->SetGuideDesc( L"" );
	}

	m_sCommonMenu.m_pCheckFrield = static_cast<CKTDGUICheckBox* >( m_pDlgRankingInfo->GetControl( L"friendcheck" ) );
	if( NULL != m_sCommonMenu.m_pCheckFrield )
		m_sCommonMenu.m_pCheckFrield->SetShowEnable( false, false );

	m_sCommonMenu.m_pCheckCharacter = static_cast<CKTDGUICheckBox* >( m_pDlgRankingInfo->GetControl( L"charactercheck" ) );
	if( NULL != m_sCommonMenu.m_pCheckCharacter )
		m_sCommonMenu.m_pCheckCharacter->SetShowEnable( false, false );

	m_sCommonMenu.m_pCheckMale = static_cast<CKTDGUICheckBox* >( m_pDlgRankingInfo->GetControl( L"malecheck" ) );
	if( NULL != m_sCommonMenu.m_pCheckMale )
		m_sCommonMenu.m_pCheckMale->SetShowEnable( false, false );

	m_sCommonMenu.m_pCheckFemale = static_cast<CKTDGUICheckBox* >( m_pDlgRankingInfo->GetControl( L"femalecheck" ) );
	if( NULL != m_sCommonMenu.m_pCheckFemale )
		m_sCommonMenu.m_pCheckFemale->SetShowEnable( false, false );

	//던전&필드에서만 쓰이는 항목
	m_pStaticDnFd = static_cast<CKTDGUIStatic* >( m_pDlgRankingInfo->GetControl( L"DnFd" ) );
	if( NULL != m_pStaticDnFd )
	{
		if( NULL != m_pStaticDnFd->GetString( 0 ) )
			m_pStaticDnFd->SetString( 0, GET_STRING( STR_ID_23655 ) ); //근성

		m_pStaticDnFd->SetShowEnable( false, false );
	}
	//주간대전에서만 쓰이는 항목
	m_pStaticWeekPvp = static_cast<CKTDGUIStatic* >( m_pDlgRankingInfo->GetControl( L"WeekPvp" ) );
	if( NULL != m_pStaticWeekPvp )
	{
		if( NULL != m_pStaticWeekPvp->GetString( 0 ) )
			m_pStaticWeekPvp->SetString( 0, GET_STRING( STR_ID_23656 ) ); //주간 누적 AP

		m_pStaticWeekPvp->SetShowEnable( false, false );
	}
	//상단 느낌표, 글씨
	m_pStaticNotice = static_cast<CKTDGUIStatic*>( m_pDlgRankingInfo->GetControl( L"g_pStatic" ) );
	if( NULL != m_pStaticNotice )
	{
		if( NULL != m_pStaticNotice)
			m_pStaticNotice->SetString( 0, GET_STRING(STR_ID_23971) );

		m_pStaticNotice->SetShowEnable( true, true );
	}
#endif //SERV_LOCAL_RANKING_SYSTEM
}

CX2UIRankingInfo::~CX2UIRankingInfo()
{
	m_pStaticCommonInfo		= NULL;
	m_pStaticHenirInfo		= NULL;
	m_pStaticPvpInfo		= NULL;
	m_pStaticLevelInfo		= NULL;

	m_pRadioButtonDaily		= NULL;
	m_pRadioButtonWeekly	= NULL;
	m_pRadioButtonMonthly	= NULL;

	//{{ 허상형 : [2009/8/21] //	신학기 이벤트 컨트롤 변수 해제
#ifdef NEW_TERM_EVENT
	m_pStaticEventInfo			= NULL;

	m_pRadioButtonEventHenir	= NULL;
	m_pRadioButtonEventDungeon	= NULL;
	m_pRadioButtonEventKill		= NULL;
	m_pRadioButtonEventDeath	= NULL;
#endif	//	NEW_TERM_EVENT
	//}} 허상형 : [2009/8/21] //	신학기 이벤크 컨트롤 변수 해제

	DestoryUIRankingInfo();
	SAFE_DELETE_DIALOG( m_pDlgRankingInfo );

#ifdef SERV_LOCAL_RANKING_SYSTEM
	SAFE_DELETE_DIALOG( m_pDlgRankingPopUp );
#endif //SERV_LOCAL_RANKING_SYSTEM
}

void		CX2UIRankingInfo::InitUIRankingInfo()
{
#ifdef SERV_LOCAL_RANKING_SYSTEM
	ChangeBar( BS_NEWBAR );
	m_nowRankingInfoType = RT_DUNGEON_FIELD_TAP;
	m_nowRankingInfoTypeSub = RT_AREA;
	CKTDGUIRadioButton* pRadio = static_cast<CKTDGUIRadioButton*>( m_pDlgRankingInfo->GetControl( L"category_area" ) );
	if( NULL != pRadio )
		pRadio->SetChecked( true );

	const CX2Unit::UnitData* pUnitData = NULL;
	if( NULL != g_pData->GetMyUser() && NULL != g_pData->GetMyUser()->GetSelectUnit())
		pUnitData = &g_pData->GetMyUser()->GetSelectUnit()->GetUnitData();
	if( NULL != pUnitData )
	{
		if( 0 == StrCmp( pUnitData->m_wstrGuildName.c_str(), L"" ) )
		{
			if( NULL != m_sCommonMenu.m_pRadioGuild )
			{
				m_sCommonMenu.m_pRadioGuild->SetGuideDesc( GET_STRING(STR_ID_23797) ); // 길드에 가입하세요
				m_sCommonMenu.m_pRadioGuild->SetOverTex_LUA( "DLG_UI_Button20_NEW.tga", "Bt_Sort_Guild_Inactive" );
				m_sCommonMenu.m_pRadioGuild->SetCustomMsgChecked( URICM_DUMMY_SELECT );
			}
		}
	}
	
	SetShowControlsByType( RT_DAY_RANKING, false );
	SetShowControlsByType( RT_PVP_RANKING, false );
	SetShowControlsByType( RT_LEVEL_RANKING, false );
	SetShowControlsByType( RT_DUNGEON_FIELD_TAP, true );

	
	CKTDGUIButton* pButton = static_cast<CKTDGUIButton* >( m_pDlgRankingInfo->GetControl( L"PageInv" ) );
	if( NULL != pButton )
	{
		pButton->SetGuideDesc( GET_STRING( STR_ID_23798 ) ); // 페이지 검색하기
	}

#else
#ifdef HIDE_HENIR_RANKING
	m_nowRankingInfoType = RT_PVP_RANKING;
	SetShowControlsByType( RT_DAY_RANKING, false );
	SetShowControlsByType( RT_PVP_RANKING, true );
#else HIDE_HENIR_RANKING
	m_nowRankingInfoType = RT_DAY_RANKING;
	SetShowControlsByType( RT_DAY_RANKING, true );
	SetShowControlsByType( RT_PVP_RANKING, false );
#endif HIDE_HENIR_RANKING
	SetShowControlsByType( RT_LEVEL_RANKING, false );
#endif //SERV_LOCAL_RANKING_SYSTEM

	//{{ 허상형 : [2009/8/21] //
#ifdef NEW_TERM_EVENT
	SetShowControlsByType( RT_EVENT_DUNGEON, false );
#endif	//	NEW_TERM_EVENT
	//}} 허상형 : [2009/8/21] //
}

void		CX2UIRankingInfo::InitMyColumn()
{
	// 랭킹
	m_columnForMyRank.m_pStaticRanking		= static_cast< CKTDGUIStatic* >( m_pDlgRankingInfo->GetControl( L"g_pStaticMyScore" ) );
	m_columnForMyRank.m_pStaticRanking->SetShowEnable( false, false );

	// 캐릭터 Icon
	m_columnForMyRank.m_pStaticClass		= static_cast< CKTDGUIStatic* >( m_pDlgRankingInfo->GetControl( L"Static_My_Class" ) );
	m_columnForMyRank.m_pStaticClass->GetPicture( 0 )->SetShow( true );
	m_columnForMyRank.m_pStaticClass->GetPicture( 1 )->SetShow( true );
	m_columnForMyRank.m_pStaticClass->GetPicture( 2 )->SetShow( true );
	m_columnForMyRank.m_pStaticClass->GetPicture( 3 )->SetShow( true );
	m_columnForMyRank.m_pStaticClass->SetShowEnable( false, false );
	

	// 닉네임
	m_columnForMyRank.m_pStaticNickName		= static_cast< CKTDGUIStatic* >( m_pDlgRankingInfo->GetControl( L"g_pStaticMyNickName" ) );
	m_columnForMyRank.m_pStaticNickName->SetShowEnable( false, false );

	// 클리어 스테이지
	m_columnForMyRank.m_pStaticClearStage	= static_cast< CKTDGUIStatic* >( m_pDlgRankingInfo->GetControl( L"Static_Clear_Stage_My" ) );
	m_columnForMyRank.m_pStaticClearStage->SetShowEnable( false, false );

	// 클리어 했을 때의 플레이 시간
	m_columnForMyRank.m_pStaticPlayTime	= static_cast< CKTDGUIStatic* >( m_pDlgRankingInfo->GetControl( L"Static_Clear_Time_My" ) );
	m_columnForMyRank.m_pStaticPlayTime->SetShowEnable( false, false );

	// 헤니르 시공의 기록이 등록된 시간
	m_columnForMyRank.m_pButtonRegistredDate	= static_cast< CKTDGUIButton* >( m_pDlgRankingInfo->GetControl( L"ButtonMyOpacity" ) );
	m_columnForMyRank.m_pButtonRegistredDate->SetShowEnable( false, false );

	// 대전 엠블렘
	m_columnForMyRank.m_pStaticPvpEmblem	= static_cast< CKTDGUIStatic* >( m_pDlgRankingInfo->GetControl( L"Static_My_Pvp_Emblem" ) );
	m_columnForMyRank.m_pStaticPvpEmblem->GetPicture( 0 )->SetShow( true );
	m_columnForMyRank.m_pStaticPvpEmblem->SetShowEnable( false, false );
	
	// 대전 승 횟수
	m_columnForMyRank.m_pStaticWin			= static_cast< CKTDGUIStatic* >( m_pDlgRankingInfo->GetControl( L"Static_Win_My" ) );
	m_columnForMyRank.m_pStaticWin->SetShowEnable( false, false );

	// 대전 패 횟수
	m_columnForMyRank.m_pStaticLose			= static_cast< CKTDGUIStatic* >( m_pDlgRankingInfo->GetControl( L"Static_Lose_My" ) );
	m_columnForMyRank.m_pStaticLose->SetShowEnable( false, false );

	// 경험치
	m_columnForMyRank.m_pStaticExp			= static_cast< CKTDGUIStatic* >( m_pDlgRankingInfo->GetControl( L"Static_EXP_My" ) );
	m_columnForMyRank.m_pStaticExp->SetShowEnable( false, false );

}

void		CX2UIRankingInfo::InitOthersColumn()
{	
	// 다른 유저들의 순위 column의 캐릭터 Icon 초기화
	
	for ( int i = 0; i < _CONST_UIRANKING_INFO_::g_iNumRankingPerPage; i++ )
	{
		// 순위표시 
		m_columnForOthersRank[i].m_pStaticRanking		= 
			static_cast< CKTDGUIStatic* >( m_pDlgRankingInfo->GetControl( GET_REPLACED_STRING( ( STR_ID_3738, "Li", std::wstring( L"StaticRankOthers" ), i + 1 ) ) ) );
		m_columnForOthersRank[i].m_pStaticRanking->SetShowEnable( false, false );

		// 캐릭터 Icon
		m_columnForOthersRank[i].m_pStaticClass			=
			static_cast< CKTDGUIStatic* >( m_pDlgRankingInfo->GetControl( GET_REPLACED_STRING( ( STR_ID_3738, "Li", std::wstring( L"Static_Class" ), i + 1 ) ) ) );
		m_columnForOthersRank[i].m_pStaticClass->GetPicture( 0 )->SetShow( true );	// 캐릭터 이미지
		m_columnForOthersRank[i].m_pStaticClass->GetPicture( 1 )->SetShow( true );	// 클래스 Initial
		m_columnForOthersRank[i].m_pStaticClass->GetPicture( 2 )->SetShow( true );	// 레벨 10의 자리
		m_columnForOthersRank[i].m_pStaticClass->GetPicture( 3 )->SetShow( true );	// 레벨 1의 자리
		m_columnForOthersRank[i].m_pStaticClass->SetShowEnable( false, false );

		// 닉네임
		m_columnForOthersRank[i].m_pStaticNickName		= 
			static_cast< CKTDGUIStatic* >( m_pDlgRankingInfo->GetControl( GET_REPLACED_STRING( ( STR_ID_3738, "Li", std::wstring( L"g_pStaticNickName" ), i + 1 ) ) ) );
		m_columnForOthersRank[i].m_pStaticNickName->SetShowEnable( false, false );

		// 클리어 스테이지
		m_columnForOthersRank[i].m_pStaticClearStage	= 
			static_cast< CKTDGUIStatic* >( m_pDlgRankingInfo->GetControl( GET_REPLACED_STRING( ( STR_ID_3738, "Li", std::wstring( L"Static_Clear_Stage" ), i + 1 ) ) ) );
		m_columnForOthersRank[i].m_pStaticClearStage->SetShowEnable( false, false );

		// 클리어 했을 때의 플레이 시간
		m_columnForOthersRank[i].m_pStaticPlayTime	= 
			static_cast< CKTDGUIStatic* >( m_pDlgRankingInfo->GetControl( GET_REPLACED_STRING( ( STR_ID_3738, "Li", std::wstring( L"Static_Clear_Time" ), i + 1 ) ) ) );
		m_columnForOthersRank[i].m_pStaticPlayTime->SetShowEnable( false, false );

		// 헤니르 시공의 기록이 등록된 시간
		m_columnForOthersRank[i].m_pButtonRegistredDate	= 
			static_cast< CKTDGUIButton* >( m_pDlgRankingInfo->GetControl( GET_REPLACED_STRING( ( STR_ID_3738, "Li", std::wstring( L"opacity" ), i + 1 ) ) ) );
		m_columnForOthersRank[i].m_pButtonRegistredDate->SetShowEnable( false, false );

		// 대전 엠블렘
		m_columnForOthersRank[i].m_pStaticPvpEmblem		= 
			static_cast< CKTDGUIStatic* >( m_pDlgRankingInfo->GetControl( GET_REPLACED_STRING( ( STR_ID_3738, "Li", std::wstring( L"Static_Pvp_Emblem" ), i + 1 ) ) ) );
		m_columnForOthersRank[i].m_pStaticPvpEmblem->GetPicture( 0 )->SetShow( true );
		m_columnForOthersRank[i].m_pStaticPvpEmblem->SetShowEnable( false, false );

		// 대전 승 횟수
		m_columnForOthersRank[i].m_pStaticWin			= 
			static_cast< CKTDGUIStatic* >( m_pDlgRankingInfo->GetControl( GET_REPLACED_STRING( ( STR_ID_3738, "Li", std::wstring( L"Static_Win" ), i + 1 ) ) ) );
		m_columnForOthersRank[i].m_pStaticWin->SetShowEnable( false, false );

		// 대전 패 횟수
		m_columnForOthersRank[i].m_pStaticLose			= 
			static_cast< CKTDGUIStatic* >( m_pDlgRankingInfo->GetControl( GET_REPLACED_STRING( ( STR_ID_3738, "Li", std::wstring( L"Static_Lose" ), i + 1 ) ) ) );
		m_columnForOthersRank[i].m_pStaticLose->SetShowEnable( false, false );

		// 경험치
		m_columnForOthersRank[i].m_pStaticExp			= 
			static_cast< CKTDGUIStatic* >( m_pDlgRankingInfo->GetControl( GET_REPLACED_STRING( ( STR_ID_3738, "Li", std::wstring( L"Static_EXP" ), i + 1 ) ) ) );
		m_columnForOthersRank[i].m_pStaticExp->SetShowEnable( false, false );

	}

}

void		CX2UIRankingInfo::SetShowControlsByType( RANKING_TYPE type, bool bShow )
{
    //{{ seojt // 2009-8-21, 12:15
    /** 'm_pRadioButtonDaily'등을 얻어낸 'm_pDlgRankingInfo'가 이미 invalid하다면,
        리턴한다.
        - jintaeks on 2009-08-21, 12:15 */
    if( m_pDlgRankingInfo == NULL )
    {
        return;
    }//if
    //}} seojt // 2009-8-21, 12:15

	switch ( type )
	{
	case RT_DAY_RANKING:
	case RT_WEEK_RANKING:
	case RT_MONTH_RANKING:
#ifdef NEW_HENIR_DUNGEON
	case RT_HERO_RANKING:
#endif // NEW_HENIR_DUNGEON
		{
#ifndef HIDE_HENIR_RANKING
			CKTDGUIRadioButton*		pRadioButton = 
				static_cast< CKTDGUIRadioButton* >( m_pDlgRankingInfo->GetControl( L"RadioButton_Henir" ) );
			pRadioButton->SetChecked( bShow );

			if ( m_pRadioButtonDaily )
				m_pRadioButtonDaily->SetShowEnable( bShow, bShow );
			
            if ( m_pRadioButtonWeekly )
				m_pRadioButtonWeekly->SetShowEnable( bShow, bShow );

			if ( m_pRadioButtonMonthly )
				m_pRadioButtonMonthly->SetShowEnable( bShow, bShow );
#ifdef NEW_HENIR_DUNGEON
			if ( m_pRadioButtonHero )
				m_pRadioButtonHero->SetShowEnable( bShow, bShow );
#endif // NEW_HENIR_DUNGEON
			if ( m_pStaticHenirInfo )
				m_pStaticHenirInfo->SetShowEnable( bShow, bShow );
			
			if ( type == RT_DAY_RANKING )
				m_pRadioButtonDaily->SetChecked( true );

#ifdef SERV_LOCAL_RANKING_SYSTEM
			if( NULL != m_pStaticCommonInfo )
				m_pStaticCommonInfo->SetShowEnable( bShow, bShow );
			if( NULL != m_pStaticWeekPvp )
				m_pStaticWeekPvp->SetShowEnable( false, false );
			if( NULL != m_pStaticDnFd )
				m_pStaticDnFd->SetShowEnable( false, false );
			if( NULL != m_sCommonMenu.m_pCheckFrield )
				m_sCommonMenu.m_pCheckFrield->SetShowEnable( false, false );
			if( NULL != m_sCommonMenu.m_pCheckCharacter )
				m_sCommonMenu.m_pCheckCharacter->SetShowEnable( false, false );
			if( NULL != m_sCommonMenu.m_pCheckMale )
				m_sCommonMenu.m_pCheckMale->SetShowEnable( false, false );
			if( NULL != m_sCommonMenu.m_pCheckFemale )
				m_sCommonMenu.m_pCheckFemale->SetShowEnable( false, false );
#endif //SERV_LOCAL_RANKING_SYSTEM
#endif HIDE_HENIR_RANKING
		}
		break;

	case RT_LEVEL_RANKING:
		{
#ifndef SERV_LOCAL_RANKING_SYSTEM
			CKTDGUIRadioButton*		pRadioButton = 
				static_cast< CKTDGUIRadioButton* >( m_pDlgRankingInfo->GetControl( L"RadioButton_Level" ) );
			pRadioButton->SetChecked( bShow );
#endif //SERV_LOCAL_RANKING_SYSTEM
			if ( m_pStaticLevelInfo )
				m_pStaticLevelInfo->SetShowEnable( bShow, bShow );
		}
		break;

	case RT_PVP_RANKING:
		{
			CKTDGUIRadioButton*		pRadioButton = 
				static_cast< CKTDGUIRadioButton* >( m_pDlgRankingInfo->GetControl( L"RadioButton_VS" ) );
			pRadioButton->SetChecked( bShow );

			if ( m_pStaticPvpInfo )
				m_pStaticPvpInfo->SetShowEnable( bShow, bShow );

#ifdef SERV_LOCAL_RANKING_SYSTEM
			if( NULL != m_pStaticCommonInfo )
				m_pStaticCommonInfo->SetShowEnable( bShow, bShow );
			if( NULL != m_pStaticWeekPvp )
				m_pStaticWeekPvp->SetShowEnable( false, false );
			if( NULL != m_pStaticDnFd )
				m_pStaticDnFd->SetShowEnable( false, false );
			if( NULL != m_sCommonMenu.m_pCheckFrield )
				m_sCommonMenu.m_pCheckFrield->SetShowEnable( false, false );
			if( NULL != m_sCommonMenu.m_pCheckCharacter )
				m_sCommonMenu.m_pCheckCharacter->SetShowEnable( false, false );
			if( NULL != m_sCommonMenu.m_pCheckMale )
				m_sCommonMenu.m_pCheckMale->SetShowEnable( false, false );
			if( NULL != m_sCommonMenu.m_pCheckFemale )
				m_sCommonMenu.m_pCheckFemale->SetShowEnable( false, false );
#endif //SERV_LOCAL_RANKING_SYSTEM
		}
		break;

#ifdef SERV_LOCAL_RANKING_SYSTEM
	case RT_WEEK_PVP_TAP:
		{
			CKTDGUIRadioButton*		pRadioButton =
				static_cast<CKTDGUIRadioButton* >( m_pDlgRankingInfo->GetControl( L"RadioButton_WeekPvp" ) );
			pRadioButton->SetChecked( bShow );

			CKTDGUIRadioButton* pRadio = static_cast<CKTDGUIRadioButton*>( m_pDlgRankingInfo->GetControl( L"category_area" ) );
			if( NULL != pRadio )
				pRadio->SetChecked( true );

			if( NULL != m_pStaticWeekPvp )
				m_pStaticWeekPvp->SetShowEnable( bShow, bShow );
			if( NULL != m_pStaticDnFd )
				m_pStaticDnFd->SetShowEnable( false, false );

			if( NULL != m_sCommonMenu.m_pStaticCommonInfo )
				m_sCommonMenu.m_pStaticCommonInfo->SetShowEnable( bShow, bShow );
			if( NULL != m_sCommonMenu.m_pRadioArea )
				m_sCommonMenu.m_pRadioArea->SetShowEnable( bShow, bShow );
			if( NULL != m_sCommonMenu.m_pRadioGuild )
					m_sCommonMenu.m_pRadioGuild->SetShowEnable( bShow, bShow );
			if( NULL != m_sCommonMenu.m_pCheckFrield )
				m_sCommonMenu.m_pCheckFrield->SetShowEnable( bShow, bShow );
			if( NULL != m_sCommonMenu.m_pCheckCharacter )
				m_sCommonMenu.m_pCheckCharacter->SetShowEnable( bShow, bShow );
			if( NULL != m_sCommonMenu.m_pCheckMale )
				m_sCommonMenu.m_pCheckMale->SetShowEnable( bShow, bShow );
			if( NULL != m_sCommonMenu.m_pCheckFemale )
				m_sCommonMenu.m_pCheckFemale->SetShowEnable( bShow, bShow );
		}
		break;

	case RT_DUNGEON_FIELD_TAP:
		{
			CKTDGUIRadioButton*		pRadioButton =
				static_cast<CKTDGUIRadioButton* >( m_pDlgRankingInfo->GetControl( L"RadioButton_DungeonField" ) );
			pRadioButton->SetChecked( bShow );

			CKTDGUIRadioButton* pRadio = static_cast<CKTDGUIRadioButton*>( m_pDlgRankingInfo->GetControl( L"category_area" ) );
			if( NULL != pRadio )
				pRadio->SetChecked( true );

			if( NULL != m_pStaticWeekPvp )
				m_pStaticWeekPvp->SetShowEnable( false, false );
			if( NULL != m_pStaticDnFd )
				m_pStaticDnFd->SetShowEnable( bShow, bShow );

			if( NULL != m_sCommonMenu.m_pStaticCommonInfo )
				m_sCommonMenu.m_pStaticCommonInfo->SetShowEnable( bShow, bShow );
			if( NULL != m_sCommonMenu.m_pRadioArea )
				m_sCommonMenu.m_pRadioArea->SetShowEnable( bShow, bShow );
			if( NULL != m_sCommonMenu.m_pRadioGuild )
					m_sCommonMenu.m_pRadioGuild->SetShowEnable( bShow, bShow );
			if( NULL != m_sCommonMenu.m_pCheckFrield )
				m_sCommonMenu.m_pCheckFrield->SetShowEnable( bShow, bShow );
			if( NULL != m_sCommonMenu.m_pCheckCharacter )
				m_sCommonMenu.m_pCheckCharacter->SetShowEnable( bShow, bShow );
			if( NULL != m_sCommonMenu.m_pCheckMale )
				m_sCommonMenu.m_pCheckMale->SetShowEnable( bShow, bShow );
			if( NULL != m_sCommonMenu.m_pCheckFemale )
				m_sCommonMenu.m_pCheckFemale->SetShowEnable( bShow, bShow );
		}
		break;

#endif //SERV_LOCAL_RANKING_SYSTEM

		//{{ 허상형 : [2009/8/20] //	신학기 이벤트
#ifdef NEW_TERM_EVENT
	case RT_EVENT_DUNGEON:
	case RT_EVENT_HENIR:
	case RT_EVENT_PVP_KILL:
	case RT_EVENT_PVP_DIE:
		{
			CKTDGUIRadioButton*		pRadioButton = 
				static_cast< CKTDGUIRadioButton* >( m_pDlgRankingInfo->GetControl( L"RadioButton_Event" ) );
			pRadioButton->SetChecked( bShow );

			if ( m_pStaticEventInfo )
				m_pStaticEventInfo->SetShowEnable( bShow, bShow );

			if ( m_pRadioButtonEventHenir )
				m_pRadioButtonEventHenir->SetShowEnable( bShow, bShow );

			if ( m_pRadioButtonEventDungeon )
				m_pRadioButtonEventDungeon->SetShowEnable( bShow, bShow );

			if ( m_pRadioButtonEventKill )
				m_pRadioButtonEventKill->SetShowEnable( bShow, bShow );

			if ( m_pRadioButtonEventDeath )
				m_pRadioButtonEventDeath->SetShowEnable( bShow, bShow );

			if ( type == RT_EVENT_DUNGEON )
				m_pRadioButtonEventDungeon->SetChecked( true );
		}
		break;

#endif		
		//}} 허상형 : [2009/8/20] //	신학기 이벤트

	default:	// MAX_NUM
		break;
	}
}

void		CX2UIRankingInfo::SetShowMyInfoByType( RANKING_TYPE type, bool bShow )
{
	m_columnForMyRank.m_pStaticRanking->SetShowEnable( bShow, bShow );
	m_columnForMyRank.m_pStaticClass->SetShowEnable( bShow, bShow );
	m_columnForMyRank.m_pStaticNickName->SetShowEnable( bShow, bShow );

	switch ( type )
	{
	case RT_DAY_RANKING:
	case RT_WEEK_RANKING:
	case RT_MONTH_RANKING:
#ifdef NEW_HENIR_DUNGEON
	case RT_HERO_RANKING:
#endif // NEW_HENIR_DUNGEON
		{
			m_columnForMyRank.m_pStaticClearStage->SetShowEnable( bShow, bShow );
			m_columnForMyRank.m_pStaticPlayTime->SetShowEnable( bShow, bShow );
			m_columnForMyRank.m_pButtonRegistredDate->SetShowEnable( bShow, bShow );

		}
		break;

	case RT_LEVEL_RANKING:
		{
			m_columnForMyRank.m_pStaticExp->SetShowEnable( bShow, bShow );
		}
		break;

	case RT_PVP_RANKING:
		{
			m_columnForMyRank.m_pStaticPvpEmblem->SetShowEnable( bShow, bShow );
			m_columnForMyRank.m_pStaticWin->SetShowEnable( bShow, bShow );
			m_columnForMyRank.m_pStaticLose->SetShowEnable( bShow, bShow );
		}
		break;

		//{{ 허상형 : [2009/8/21] //	신학기 이벤트 내 점수 관련 컨트롤 활성화
#ifdef	NEW_TERM_EVENT
	case RT_EVENT_HENIR:
	case RT_EVENT_DUNGEON:
	case RT_EVENT_PVP_KILL:
	case RT_EVENT_PVP_DIE:
		{
			m_columnForMyRank.m_pStaticExp->SetShowEnable( bShow, bShow );	//	점수 표시
		}
		break;
#endif	//	NEW_TERM_EVENT
		//}} 허상형 : [2009/8/21] //	신학기 이벤트 내 점수 관련 컨트롤 활성화

	default:	// MAX_NUM
		{
			m_columnForMyRank.m_pStaticClearStage->SetShowEnable( bShow, bShow );
			m_columnForMyRank.m_pStaticPlayTime->SetShowEnable( bShow, bShow );
			m_columnForMyRank.m_pButtonRegistredDate->SetShowEnable( bShow, bShow );
			m_columnForMyRank.m_pStaticPvpEmblem->SetShowEnable( bShow, bShow );
			m_columnForMyRank.m_pStaticWin->SetShowEnable( bShow, bShow );
			m_columnForMyRank.m_pStaticLose->SetShowEnable( bShow, bShow );
			m_columnForMyRank.m_pStaticExp->SetShowEnable( bShow, bShow );
		}
		break;
	}
}

void		CX2UIRankingInfo::SetShowOthersInfoByType( ColumnForRank* const pColumnForOthersRank, RANKING_TYPE type, bool bShow )
{
	pColumnForOthersRank->m_pStaticRanking->SetShowEnable( bShow, bShow );
	pColumnForOthersRank->m_pStaticClass->SetShowEnable( bShow, bShow );
	pColumnForOthersRank->m_pStaticNickName->SetShowEnable( bShow, bShow );

	switch ( type )
	{
	case RT_DAY_RANKING:
	case RT_WEEK_RANKING:
	case RT_MONTH_RANKING:
#ifdef NEW_HENIR_DUNGEON
	case RT_HERO_RANKING:
#endif // NEW_HENIR_DUNGEON
		{
			pColumnForOthersRank->m_pStaticClearStage->SetShowEnable( bShow, bShow );
			pColumnForOthersRank->m_pStaticPlayTime->SetShowEnable( bShow, bShow );
			pColumnForOthersRank->m_pButtonRegistredDate->SetShowEnable( bShow, bShow );

		}
		break;

	case RT_LEVEL_RANKING:
		{
			pColumnForOthersRank->m_pStaticExp->SetShowEnable( bShow, bShow );
		}
		break;

	case RT_PVP_RANKING:
		{
			pColumnForOthersRank->m_pStaticPvpEmblem->SetShowEnable( bShow, bShow );
			pColumnForOthersRank->m_pStaticWin->SetShowEnable( bShow, bShow );
			pColumnForOthersRank->m_pStaticLose->SetShowEnable( bShow, bShow );
		}
		break;

		//{{ 허상형 : [2009/8/21] //	신학기 이벤트 내 점수 관련 컨트롤 활성화
#ifdef	NEW_TERM_EVENT
	case RT_EVENT_HENIR:
	case RT_EVENT_DUNGEON:
		{
			pColumnForOthersRank->m_pStaticExp->SetShowEnable( bShow, bShow );	//	점수 표시
		}
		break;
#endif	//	NEW_TERM_EVENT
		//}} 허상형 : [2009/8/21] //	신학기 이벤트 내 점수 관련 컨트롤 활성화

	default:	// MAX_NUM	// 이곳에서는 아무 정보도 출력하지 않아야 하는 column 용도로 사용
		{
			pColumnForOthersRank->m_pStaticClearStage->SetShowEnable( bShow, bShow );
			pColumnForOthersRank->m_pStaticPlayTime->SetShowEnable( bShow, bShow );
			pColumnForOthersRank->m_pButtonRegistredDate->SetShowEnable( bShow, bShow );
			pColumnForOthersRank->m_pStaticPvpEmblem->SetShowEnable( bShow, bShow );
			pColumnForOthersRank->m_pStaticWin->SetShowEnable( bShow, bShow );
			pColumnForOthersRank->m_pStaticLose->SetShowEnable( bShow, bShow );
			pColumnForOthersRank->m_pStaticExp->SetShowEnable( bShow, bShow );
		}
		break;
	}
}

///////////핸들러////////////////////////////////////////////////////////////////////////////////

bool		CX2UIRankingInfo::Handler_EGS_GET_RANKING_INFO_REQ()
{
	KEGS_GET_RANKING_INFO_REQ kPacket;

	kPacket.m_iRankingType	= static_cast< int >( m_nowRankingInfoType );
	kPacket.m_uiViewPage	= m_uiNowPage;

	g_pData->GetServerProtocol()->SendPacket( EGS_GET_RANKING_INFO_REQ, kPacket ); 
	g_pMain->AddServerPacket( EGS_GET_RANKING_INFO_ACK, 60.f ); 
	
	return true;
}

bool		CX2UIRankingInfo::Handler_EGS_GET_RANKING_INFO_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_GET_RANKING_INFO_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	g_pMain->DeleteServerPacket( EGS_GET_RANKING_INFO_ACK );
	{
#ifdef LOCAL_RANKING_UPDATE
		CKTDGUIStatic* pStatic = static_cast<CKTDGUIStatic*>(m_pDlgRankingInfo->GetControl( L"ranking_null" ));
		if( NULL != pStatic )
			pStatic->SetShowEnable(false,false);
#endif //LOCAL_RANKING_UPDATE

		m_nowRankingInfoType	= static_cast< RANKING_TYPE >( kEvent.m_iRankingType );

		// 랭킹 정보가 우선 다 보이지 않도록 함
		for( int i = 0; i < _CONST_UIRANKING_INFO_::g_iNumRankingPerPage; i++)
			SetShowOthersInfoByType( (m_columnForOthersRank + i), RT_MAX_NUM, false );

		SetShowMyInfoByType( RT_MAX_NUM, false );

#ifdef SERV_LOCAL_RANKING_SYSTEM
		for( int i = 0; i <_CONST_UIRANKING_FRIEND_INFO_::g_iNumRankingPerPage; i++ )
			SetShowInfoByType_F( (m_columnForOthersFriendRank + i), false );

		SetShowInfoByType_F( &m_columnForMyFriendRank, false );
		ChangeBar( BS_OLDBAR );
#endif //SERV_LOCAL_RANKING_SYSTEM

		// 현재 페이지 수와 전체 페이지 수 업데이트
		if ( kEvent.m_uiTotalPage > _CONST_UIRANKING_INFO_::g_iMaxPageLimit )
		{
			m_uiMaxPage				= _CONST_UIRANKING_INFO_::g_iMaxPageLimit;
		}
		else
		{
			m_uiMaxPage				= kEvent.m_uiTotalPage;
		}
		
		m_uiNowPage				= kEvent.m_uiViewPage;

		UpdatePageUI();

		switch ( m_nowRankingInfoType )
		{
		case RT_DAY_RANKING:
		case RT_WEEK_RANKING:
		case RT_MONTH_RANKING:
#ifdef NEW_HENIR_DUNGEON
		case RT_HERO_RANKING:
#endif // NEW_HENIR_DUNGEON

			{
				// 다른 유저들의 랭킹
				int i = 0;

				//BOOST_TEST_FOREACH( KHenirRankingInfo, kHenirRankingInfo, kEvent.m_vecHenirRankingPage )
				BOOST_TEST_FOREACH( KHenirRankingInfo&, kHenirRankingInfo, kEvent.m_vecHenirRankingPage )
				{
					SetRankNumImage( m_columnForOthersRank[i].m_pStaticRanking, kHenirRankingInfo.m_iRank );

					SetCharIcon( m_columnForOthersRank[i].m_pStaticClass, kHenirRankingInfo.m_cUnitClass, 
						kHenirRankingInfo.m_ucLevel );		// 캐릭터 Icon

#ifdef RANKING_INFO_UNITUID_FOR_GM
					if( NULL != g_pData->GetMyUser() && CX2User::XUAL_OPERATOR <= g_pData->GetMyUser()->GetAuthLevel() )
					{
						std::wstring wstrAddUnitUID = kHenirRankingInfo.m_wstrNickName;
						wstrAddUnitUID += L"\nUID: ";
						wstrAddUnitUID += boost::str( boost::wformat( L"%d" ) % kHenirRankingInfo.m_iUnitUID );
						m_columnForOthersRank[i].m_pStaticNickName->SetString( 0, wstrAddUnitUID.c_str() );	// 닉네임
					}
					else
#endif // RANKING_INFO_UNITUID_FOR_GM
					m_columnForOthersRank[i].m_pStaticNickName->SetString( 0, kHenirRankingInfo.m_wstrNickName.c_str() );	// 닉네임
					m_columnForOthersRank[i].m_pStaticClearStage->SetString( 0, GET_REPLACED_STRING( ( STR_ID_3733, "i", kHenirRankingInfo.m_iStageCount ) ) );
					m_columnForOthersRank[i].m_pStaticPlayTime->SetString( 0, ConvertSecToPlayTimeStr( kHenirRankingInfo.m_ulPlayTime ).c_str() );
					m_columnForOthersRank[i].m_pButtonRegistredDate->SetGuideDesc( ConvertTimeToRegisterDateStr( kHenirRankingInfo.m_tRegDate ).c_str() );

					SetShowOthersInfoByType( (m_columnForOthersRank + i), m_nowRankingInfoType, true );
					i++;
				}

				// 유저 자신의 랭킹 정보
				BOOST_TEST_FOREACH( KHenirRankingInfo&, kMyHenirRankingInfo, kEvent.m_vecMyHenirRanking )
				{
#ifdef CLIENT_GLOBAL_LINEBREAK
					wstring wstrLatestRanking = CWordLineHandler::GetStrByLineBreakColorInX2Main(GET_STRING( STR_ID_3744 ), 55, XUF_DODUM_13_SEMIBOLD);
					m_columnForMyRank.m_pStaticRanking->GetString( 0 )->fontIndex	=	XUF_DODUM_13_SEMIBOLD;
					m_columnForMyRank.m_pStaticRanking->SetString( 0, wstrLatestRanking.c_str());
#else //CLIENT_GLOBAL_LINEBREAK
					m_columnForMyRank.m_pStaticRanking->GetString( 0 )->fontIndex	=	XUF_DODUM_13_SEMIBOLD;
					m_columnForMyRank.m_pStaticRanking->SetString( 0, GET_STRING( STR_ID_3744 ) );
#endif //CLIENT_GLOBAL_LINEBREAK

					SetCharIcon( m_columnForMyRank.m_pStaticClass, kMyHenirRankingInfo.m_cUnitClass, 
						kMyHenirRankingInfo.m_ucLevel );		// 캐릭터 Icon

					m_columnForMyRank.m_pStaticNickName->SetString( 0, kMyHenirRankingInfo.m_wstrNickName.c_str() );	// 닉네임
					m_columnForMyRank.m_pStaticClearStage->SetString( 0, GET_REPLACED_STRING( ( STR_ID_3733, "i", kMyHenirRankingInfo.m_iStageCount ) ) );
					m_columnForMyRank.m_pStaticPlayTime->SetString( 0, ConvertSecToPlayTimeStr( kMyHenirRankingInfo.m_ulPlayTime ).c_str() );
					m_columnForMyRank.m_pButtonRegistredDate->SetGuideDesc( ConvertTimeToRegisterDateStr( kMyHenirRankingInfo.m_tRegDate ).c_str() );

					SetShowMyInfoByType( m_nowRankingInfoType, true );
				}
			}			
			break;

		case RT_LEVEL_RANKING:			// enum.h의 RANKING_TYPE의 RT_DUNGEON_RANKING과 같음
			{
				// 다른 유저들의 랭킹
				int i = 0;

				BOOST_TEST_FOREACH( KDungeonRankingInfo&, kLevelRankingInfo, kEvent.m_vecDungeonRankingPage )
				{
					SetRankNumImage( m_columnForOthersRank[i].m_pStaticRanking, kLevelRankingInfo.m_iRank );

					SetCharIcon( m_columnForOthersRank[i].m_pStaticClass, kLevelRankingInfo.m_cUnitClass, 
						kLevelRankingInfo.m_ucLevel );		// 캐릭터 Icon

#ifdef RANKING_INFO_UNITUID_FOR_GM
					if( NULL != g_pData->GetMyUser() && CX2User::XUAL_OPERATOR <= g_pData->GetMyUser()->GetAuthLevel() )
					{
						std::wstring wstrAddUnitUID = kLevelRankingInfo.m_wstrNickName;
						wstrAddUnitUID += L"\nUID: ";
						wstrAddUnitUID += boost::str( boost::wformat( L"%d" ) % kLevelRankingInfo.m_iUnitUID );
						m_columnForOthersRank[i].m_pStaticNickName->SetString( 0, wstrAddUnitUID.c_str() );	// 닉네임
					}
					else
#endif // RANKING_INFO_UNITUID_FOR_GM
					m_columnForOthersRank[i].m_pStaticNickName->SetString( 0, kLevelRankingInfo.m_wstrNickName.c_str() );	// 닉네임
					m_columnForOthersRank[i].m_pStaticExp->SetString( 0, GET_REPLACED_STRING( ( STR_ID_2632, "i", kLevelRankingInfo.m_iEXP ) ) );

					SetShowOthersInfoByType( (m_columnForOthersRank + i), m_nowRankingInfoType, true );
					i++;
				}

				// 유저 자신의 랭킹 정보

				BOOST_TEST_FOREACH( KDungeonRankingInfo&, kMyLevelRankingInfo, kEvent.m_vecMyDungeonRanking )
				{
					m_columnForMyRank.m_pStaticRanking->GetString( 0 )->fontIndex	=	XUF_DODUM_15_BOLD;
					if ( kMyLevelRankingInfo.m_iRank <= _CONST_UIRANKING_INFO_::g_iLimitRankingToDisplay )
						m_columnForMyRank.m_pStaticRanking->SetString( 0, GET_REPLACED_STRING( ( STR_ID_2632, "i", kMyLevelRankingInfo.m_iRank ) ) );
					else
						m_columnForMyRank.m_pStaticRanking->SetString( 0, GET_STRING( STR_ID_3734 ) );

					SetCharIcon( m_columnForMyRank.m_pStaticClass, kMyLevelRankingInfo.m_cUnitClass, 
						kMyLevelRankingInfo.m_ucLevel );		// 캐릭터 Icon

					m_columnForMyRank.m_pStaticNickName->SetString( 0, kMyLevelRankingInfo.m_wstrNickName.c_str() );	// 닉네임
					m_columnForMyRank.m_pStaticExp->SetString( 0, GET_REPLACED_STRING( ( STR_ID_2632, "i", kMyLevelRankingInfo.m_iEXP ) ) );

					SetShowMyInfoByType( m_nowRankingInfoType, true );
				}			
			}
			break;

		case RT_PVP_RANKING:
			{
				// 다른 유저들의 랭킹
				int i = 0;

				BOOST_TEST_FOREACH( KPvpRankingInfo&, kPvpRankingInfo, kEvent.m_vecPvpRankingPage )
				{
					SetRankNumImage( m_columnForOthersRank[i].m_pStaticRanking, kPvpRankingInfo.m_iRank );

					SetCharIcon( m_columnForOthersRank[i].m_pStaticClass, kPvpRankingInfo.m_cUnitClass, 
						kPvpRankingInfo.m_ucLevel );		// 캐릭터 Icon

#ifdef RANKING_INFO_UNITUID_FOR_GM
					if( NULL != g_pData->GetMyUser() && CX2User::XUAL_OPERATOR <= g_pData->GetMyUser()->GetAuthLevel() )
					{
						std::wstring wstrAddUnitUID = kPvpRankingInfo.m_wstrNickName;
						wstrAddUnitUID += L"\nUID: ";
						wstrAddUnitUID += boost::str( boost::wformat( L"%d" ) % kPvpRankingInfo.m_iUnitUID );
						m_columnForOthersRank[i].m_pStaticNickName->SetString( 0, wstrAddUnitUID.c_str() );	// 닉네임
					}
					else
#endif // RANKING_INFO_UNITUID_FOR_GM
					m_columnForOthersRank[i].m_pStaticNickName->SetString( 0, kPvpRankingInfo.m_wstrNickName.c_str() );	// 닉네임

					CX2PVPEmblem::PVPEmblemData*	pPvpEmblemData	= NULL;

#ifdef SERV_PVP_NEW_SYSTEM

#ifdef PVP_SEASON2
					
#ifdef FIX_VISIBLE_BY_UNKNOWN_RANK // 김태환
					char cRank = kPvpRankingInfo.m_cRank;

					/// 랭크에 대한 범위 체크
					if ( CX2PVPEmblem::PVPRANK_NONE >= cRank || CX2PVPEmblem::PVPRANK_RANK_SSS < cRank )
						cRank = 0;
#else // FIX_VISIBLE_BY_UNKNOWN_RANK
					const char cRank = kPvpRankingInfo.m_cRank;
#endif // FIX_VISIBLE_BY_UNKNOWN_RANK

					pPvpEmblemData = g_pMain->GetPVPEmblem()->GetPVPEmblemData( static_cast<CX2PVPEmblem::PVP_RANK>( cRank ) );
#else
					CX2PVPEmblem::PVP_EMBLEM		emblemID		= CX2PVPEmblem::PE_NONE;
					pPvpEmblemData		= g_pMain->GetPVPEmblem()->GetPVPEmblemData( kPvpRankingInfo.m_iRating );
#endif

#else
					emblemID			= g_pMain->GetPVPEmblem()->PvpEmblemEnumToPvpEmblem( static_cast< CX2PVPEmblem::PVP_EMBLEM_ENUM >( kPvpRankingInfo.m_cPvpEmblem ) );
					pPvpEmblemData		= g_pMain->GetPVPEmblem()->GetPVPEmblemData( emblemID );
#endif

#ifdef FIX_VISIBLE_BY_UNKNOWN_RANK // 김태환
					/// NULL 체크 추가
					if ( static_cast<char>( CX2PVPEmblem::PVPRANK_RANK_ARRANGE ) < cRank && NULL != pPvpEmblemData )
#else // FIX_VISIBLE_BY_UNKNOWN_RANK
					if ( static_cast<char>( CX2PVPEmblem::PVPRANK_RANK_ARRANGE ) < cRank )
#endif // FIX_VISIBLE_BY_UNKNOWN_RANK
					{
						m_columnForOthersRank[i].m_pStaticPvpEmblem->GetPicture( 0 )->SetShow( true );
						m_columnForOthersRank[i].m_pStaticPvpEmblem->GetPicture( 0 )->SetTex( pPvpEmblemData->m_TextureName.c_str(),
							pPvpEmblemData->m_TextureKey.c_str() );
					}
					else
						m_columnForOthersRank[i].m_pStaticPvpEmblem->GetPicture( 0 )->SetShow( false );

					m_columnForOthersRank[i].m_pStaticWin->SetString( 0, GET_REPLACED_STRING( ( STR_ID_3736, "i", kPvpRankingInfo.m_iWin ) ) );
#ifdef SERV_PVP_NEW_SYSTEM
					// 랭킹점수
					m_columnForOthersRank[i].m_pStaticLose->SetString( 0, GET_REPLACED_STRING( ( STR_ID_2632, "i", kPvpRankingInfo.m_iRPoint ) ) );
#else
					m_columnForOthersRank[i].m_pStaticLose->SetString( 0, GET_REPLACED_STRING( ( STR_ID_3737, "i", kPvpRankingInfo.m_iLose ) ) );
#endif

					SetShowOthersInfoByType( (m_columnForOthersRank + i), m_nowRankingInfoType, true );
					i++;
				}

				// 유저 자신의 랭킹 정보
				BOOST_TEST_FOREACH( KPvpRankingInfo&, kMyPvpRankingInfo, kEvent.m_vecMyPvpRanking )
				{
					m_columnForMyRank.m_pStaticRanking->GetString( 0 )->fontIndex	=	XUF_DODUM_15_BOLD;
					if ( kMyPvpRankingInfo.m_iRank <= _CONST_UIRANKING_INFO_::g_iLimitRankingToDisplay )
						m_columnForMyRank.m_pStaticRanking->SetString( 0, GET_REPLACED_STRING( ( STR_ID_2632, "i", kMyPvpRankingInfo.m_iRank ) ) );
					else
						m_columnForMyRank.m_pStaticRanking->SetString( 0, GET_STRING( STR_ID_3734 ) );

					SetCharIcon( m_columnForMyRank.m_pStaticClass, kMyPvpRankingInfo.m_cUnitClass, 
						kMyPvpRankingInfo.m_ucLevel );		// 캐릭터 Icon

					m_columnForMyRank.m_pStaticNickName->SetString( 0, kMyPvpRankingInfo.m_wstrNickName.c_str() );	// 닉네임

					CX2PVPEmblem::PVPEmblemData*	pPvpEmblemData	= NULL;

#ifdef SERV_PVP_NEW_SYSTEM
#ifdef PVP_SEASON2
					const char cRank = kMyPvpRankingInfo.m_cRank;
					pPvpEmblemData = g_pMain->GetPVPEmblem()->GetPVPEmblemData( static_cast<CX2PVPEmblem::PVP_RANK>( cRank ) );
#else
					CX2PVPEmblem::PVP_EMBLEM		emblemID		= CX2PVPEmblem::PE_NONE;
					pPvpEmblemData			= g_pMain->GetPVPEmblem()->GetPVPEmblemData( kMyPvpRankingInfo.m_iRating );
#endif
#else
					emblemID				= g_pMain->GetPVPEmblem()->PvpEmblemEnumToPvpEmblem( static_cast< CX2PVPEmblem::PVP_EMBLEM_ENUM >( kMyPvpRankingInfo.m_cPvpEmblem ) );
					pPvpEmblemData			= g_pMain->GetPVPEmblem()->GetPVPEmblemData( emblemID );
#endif

					if ( static_cast<char>( CX2PVPEmblem::PVPRANK_RANK_ARRANGE ) < cRank )
					{
						m_columnForMyRank.m_pStaticPvpEmblem->GetPicture( 0 )->SetShow( true );
						m_columnForMyRank.m_pStaticPvpEmblem->GetPicture( 0 )->SetTex( pPvpEmblemData->m_TextureName.c_str(),
							pPvpEmblemData->m_TextureKey.c_str() );
					}
					else
						m_columnForMyRank.m_pStaticPvpEmblem->GetPicture( 0 )->SetShow( false );

					m_columnForMyRank.m_pStaticWin->SetString( 0, GET_REPLACED_STRING( ( STR_ID_3736, "i", kMyPvpRankingInfo.m_iWin ) ) );
#ifdef SERV_PVP_NEW_SYSTEM
					m_columnForMyRank.m_pStaticLose->SetString( 0, GET_REPLACED_STRING( ( STR_ID_2632, "i", kMyPvpRankingInfo.m_iRPoint ) ) );
#else
					m_columnForMyRank.m_pStaticLose->SetString( 0, GET_REPLACED_STRING( ( STR_ID_3737, "i", kMyPvpRankingInfo.m_iLose ) ) );
#endif
					SetShowMyInfoByType( m_nowRankingInfoType, true );
				}			
			}
			break;

			//{{ 허상형 : [2009/8/21] //	신학기 이벤트 응답 메시지 처리
#ifdef	NEW_TERM_EVENT
			//	헤니르, 던전 점수
		case RT_EVENT_HENIR:
		case RT_EVENT_DUNGEON:
			{
				// 다른 유저들의 랭킹
				int i = 0;

				BOOST_TEST_FOREACH( KHenirRankingInfo&, kHenirRankingInfo, kEvent.m_vecHenirRankingPage )
				{
					SetRankNumImage( m_columnForOthersRank[i].m_pStaticRanking, kHenirRankingInfo.m_iRank );

					SetCharIcon( m_columnForOthersRank[i].m_pStaticClass, kHenirRankingInfo.m_cUnitClass, 
						kHenirRankingInfo.m_ucLevel );		// 캐릭터 Icon

					m_columnForOthersRank[i].m_pStaticNickName->SetString( 0, kHenirRankingInfo.m_wstrNickName.c_str() );	// 닉네임
					m_columnForOthersRank[i].m_pStaticExp->SetString( 0, GET_REPLACED_STRING( ( STR_ID_2632, "i", kHenirRankingInfo.m_iStageCount ) ) );	//	점수

					SetShowOthersInfoByType( (m_columnForOthersRank + i), m_nowRankingInfoType, true );
					i++;
				}
			}
			//	Note!! break; 없음
			//	유저 자신의 점수 정보는 모드(대전(킬,데스), 던전(헤니르,일반) 에 상관없이 기본으로 보여준다
		case RT_EVENT_PVP_KILL:
		case RT_EVENT_PVP_DIE:
			{
				// 유저 자신의 점수 정보
				BOOST_TEST_FOREACH( KHenirRankingInfo&, kMyHenirRankingInfo, kEvent.m_vecMyHenirRanking )
				{
					m_columnForMyRank.m_pStaticRanking->GetString( 0 )->fontIndex	=	XUF_DODUM_13_SEMIBOLD;
					m_columnForMyRank.m_pStaticRanking->SetString( 0, GET_STRING( STR_ID_4414 ) );

					SetCharIcon( m_columnForMyRank.m_pStaticClass, kMyHenirRankingInfo.m_cUnitClass, 
						kMyHenirRankingInfo.m_ucLevel );		// 캐릭터 Icon

					m_columnForMyRank.m_pStaticNickName->SetString( 0, kMyHenirRankingInfo.m_wstrNickName.c_str() );	// 닉네임
					m_columnForMyRank.m_pStaticExp->SetString( 0, GET_REPLACED_STRING( ( STR_ID_2632, "i", kMyHenirRankingInfo.m_iStageCount ) ) );	//	점수

					SetShowMyInfoByType( m_nowRankingInfoType, true );
				}
			}
			break;
#endif
			//}} 허상형 : [2009/8/21] //	신학기 이벤트 응답 메시지 처리
		}
	}

	return true;
}

void CX2UIRankingInfo::UpdatePageUI() const
{
	if ( m_pDlgRankingInfo != NULL )
	{
		CKTDGUIStatic* pStatic = (CKTDGUIStatic*)( m_pDlgRankingInfo->GetControl( L"Static_PageNumber" ) );

		if ( pStatic != NULL )
		{
			wstringstream wstrstm;
			wstrstm << m_uiNowPage
				<< L"/" << m_uiMaxPage;
			pStatic->SetString( 0, wstrstm.str().c_str() );
		}
	}
}

std::wstring CX2UIRankingInfo::ConvertSecToPlayTimeStr( UINT uiSec ) const
{
	int iSec	= 0;
	int iMin	= 0;
	int iHour	= 0;

	//{{ 2009. 10. 1  최육사	랭킹 표시 - 롤백
	//uiSec		= static_cast< UINT >( MAX_PLAY_TIME - uiSec );		// 남은 시간이 아닌 소요시간으로 수정
	//}}

	iHour		= uiSec / 3600;
	iMin		= ( uiSec % 3600 ) / 60;
	iSec		= uiSec - ( iHour * 3600 + iMin * 60 );

	std::wstringstream strStream;

	if ( iHour >= 0 && iHour < 10 )
		strStream << 0;
	strStream	<< iHour << " : ";

	if ( iMin >= 0 && iMin < 10 )
		strStream << 0;
	strStream	<< iMin << " : " ;

	if ( iSec >= 0 && iSec < 10 )
		strStream << 0;
	strStream	<< iSec << std::endl;
	
	return std::wstring( strStream.str().c_str() );
}

std::wstring CX2UIRankingInfo::ConvertTimeToRegisterDateStr( __int64 tRegisterDate )
{
	CTime cTime( tRegisterDate );

	std::wstring wstrDate;

	wstrDate	=	GET_STRING( STR_ID_3735 );
	wstrDate	+=	L"\n";
	wstrDate	+=	static_cast< const wchar_t * >( cTime.Format( _T("%Y-%m-%d") ) );
	wstrDate	+=	L"\n";
	wstrDate	+=	static_cast< const wchar_t * >( cTime.Format( _T("%p %H:%M") ) );
	
	return wstrDate;
}

void		CX2UIRankingInfo::DestoryUIRankingInfo()
{
	for ( int i = 0; i < _CONST_UIRANKING_INFO_::g_iNumRankingPerPage; i++ )
	{
		m_columnForOthersRank[i].m_pStaticRanking		= NULL;
		m_columnForOthersRank[i].m_pStaticClass			= NULL;
		m_columnForOthersRank[i].m_pStaticNickName		= NULL;
		m_columnForOthersRank[i].m_pStaticClearStage	= NULL;
		m_columnForOthersRank[i].m_pStaticPlayTime		= NULL;
		m_columnForOthersRank[i].m_pButtonRegistredDate	= NULL;
		m_columnForOthersRank[i].m_pStaticPvpEmblem		= NULL;
		m_columnForOthersRank[i].m_pStaticWin			= NULL;
		m_columnForOthersRank[i].m_pStaticLose			= NULL;
		m_columnForOthersRank[i].m_pStaticExp			= NULL;
	}

	m_columnForMyRank.m_pStaticRanking			= NULL;
	m_columnForMyRank.m_pStaticClass			= NULL;
	m_columnForMyRank.m_pStaticNickName			= NULL;
	m_columnForMyRank.m_pStaticClearStage		= NULL;
	m_columnForMyRank.m_pStaticPlayTime			= NULL;
	m_columnForMyRank.m_pButtonRegistredDate	= NULL;
	m_columnForMyRank.m_pStaticPvpEmblem		= NULL;
	m_columnForMyRank.m_pStaticWin				= NULL;
	m_columnForMyRank.m_pStaticLose				= NULL;
	m_columnForMyRank.m_pStaticExp				= NULL;

#ifdef SERV_LOCAL_RANKING_SYSTEM
	for( int i = 0; i < _CONST_UIRANKING_FRIEND_INFO_::g_iNumRankingPerPage; i++ )
	{
		m_columnForOthersFriendRank[i].m_pButtonPopUp		= NULL;
		m_columnForOthersFriendRank[i].m_pStaticClass		= NULL;
		m_columnForOthersFriendRank[i].m_pStaticGender		= NULL;
		m_columnForOthersFriendRank[i].m_pStaticNickName	= NULL;
		m_columnForOthersFriendRank[i].m_pStaticRanking		= NULL;
		m_columnForOthersFriendRank[i].m_pStaticPoint	= NULL;
	}

	m_columnForMyFriendRank.m_pButtonPopUp		= NULL;
	m_columnForMyFriendRank.m_pStaticClass		= NULL;
	m_columnForMyFriendRank.m_pStaticGender		= NULL;
	m_columnForMyFriendRank.m_pStaticNickName	= NULL;
	m_columnForMyFriendRank.m_pStaticRanking	= NULL;
	m_columnForMyFriendRank.m_pStaticPoint	= NULL;
#endif //SERV_LOCAL_RANKING_SYSTEM
}

void		CX2UIRankingInfo::SetCharIcon( CKTDGUIStatic* const pStaticClass, char cUnitClass, int iLevel )
{
	
	wstring fileName;
	wstring pieceName;
	if( true == CX2Data::GetCharacterImageName( fileName, pieceName, (CX2Unit::UNIT_CLASS) cUnitClass, CX2Data::CIT_50by50 ) )
	{
		pStaticClass->GetPicture(0)->SetTex( fileName.c_str(), pieceName.c_str() );
	}
	else
	{
		ASSERT( !"NO" );
	}


	if( true == CX2Data::GetCharacterImageName( fileName, pieceName, (CX2Unit::UNIT_CLASS) cUnitClass, CX2Data::CIT_ClassInitial ) )
	{
		if( true == pieceName.empty() )
		{
			pStaticClass->GetPicture(1)->SetShow( false );
		}
		else
		{
			pStaticClass->GetPicture(1)->SetTex( fileName.c_str(), pieceName.c_str() );
			pStaticClass->GetPicture(1)->SetShow( true );
		}
	}
	else
	{
		ASSERT( !"NO" );
	}


	

	int iLv10	= 0;
	int iLv1	= 0;

	iLv10		= iLevel / 10;
	iLv1		= iLevel % 10;

	SetLevelIcon( pStaticClass->GetPicture( 2 ), iLv10 );
	SetLevelIcon( pStaticClass->GetPicture( 3 ), iLv1 );

	pStaticClass->GetPicture( 2 )->SetShow( true );
	pStaticClass->GetPicture( 3 )->SetShow( true );
}

void CX2UIRankingInfo::SetLevelIcon( CKTDGUIControl::CPictureData* const pPictureData, int num )
{
	switch ( num )
	{
	case 0:
		pPictureData->SetTex( L"DLG_UI_Common_Texture02.tga", L"zero" );
		break;

	case 1:
		pPictureData->SetTex( L"DLG_UI_Common_Texture02.tga", L"one" );
		break;

	case 2:
		pPictureData->SetTex( L"DLG_UI_Common_Texture02.tga", L"two" );
		break;

	case 3:
		pPictureData->SetTex( L"DLG_UI_Common_Texture02.tga", L"three" );
		break;

	case 4:
		pPictureData->SetTex( L"DLG_UI_Common_Texture02.tga", L"four" );
		break;

	case 5:
		pPictureData->SetTex( L"DLG_UI_Common_Texture02.tga", L"five" );
		break;

	case 6:
		pPictureData->SetTex( L"DLG_UI_Common_Texture02.tga", L"six" );
		break;

	case 7:
		pPictureData->SetTex( L"DLG_UI_Common_Texture02.tga", L"seven" );
		break;
		
	case 8:
		pPictureData->SetTex( L"DLG_UI_Common_Texture02.tga", L"eight" );
		break;

	case 9:
		pPictureData->SetTex( L"DLG_UI_Common_Texture02.tga", L"nine" );
		break;

	default:
		ASSERT( false );
		break;
	}
}

void CX2UIRankingInfo::SetRankNumImage( CKTDGUIStatic* const pStaticRankNumImg, int iRankNum )
{
	int iRank100	= 0;
	int iRank10		= 0;
	int iRank1		= 0;

	iRank100		= iRankNum	/	100;
	iRank10			= ( iRankNum	%	100 ) / 10;
	iRank1			= iRankNum	% 10;

	for ( int i = 0; i < _CONST_UIRANKING_INFO_::g_iRankLimit; i ++ )
		pStaticRankNumImg->GetPicture( i )->SetShow( false );

	if ( iRank100 > 0 )			// 세자리 숫자면
	{
		SetNumToPictureForRank( pStaticRankNumImg->GetPicture( 0 ), iRank100 );
		SetNumToPictureForRank( pStaticRankNumImg->GetPicture( 1 ), iRank10 );
		SetNumToPictureForRank( pStaticRankNumImg->GetPicture( 2 ), iRank1 );

	}
	else if ( iRank10 > 0 )		// 두자리 숫자면
	{
		SetNumToPictureForRank( pStaticRankNumImg->GetPicture( 1 ), iRank10 );
		SetNumToPictureForRank( pStaticRankNumImg->GetPicture( 2 ), iRank1 );
	}
	else if ( iRank1 > 0 )		// 한자리 숫자면
	{
		// 한자리 일때는 가운데에 출력
		SetNumToPictureForRank( pStaticRankNumImg->GetPicture( 1 ), iRank1 );
	}
	else
	{
		// 순위가 0인 경우는 올 수 없음
		ASSERT( false );
	}

}

void CX2UIRankingInfo::SetNumToPictureForRank( CKTDGUIControl::CPictureData* const pPictureData, int num )
{
	std::wstring		wstrTexName;
	
	switch ( num )
	{
	case 0:
		wstrTexName		=	L"PC_0.dds";
		break;

	case 1:
		wstrTexName		=	L"PC_1.dds";
		break;

	case 2:
		wstrTexName		=	L"PC_2.dds";
		break;

	case 3:
		wstrTexName		=	L"PC_3.dds";
		break;

	case 4:
		wstrTexName		=	L"PC_4.dds";
		break;

	case 5:
		wstrTexName		=	L"PC_5.dds";
		break;

	case 6:
		wstrTexName		=	L"PC_6.dds";
		break;

	case 7:
		wstrTexName		=	L"PC_7.dds";
		break;

	case 8:
		wstrTexName		=	L"PC_8.dds";
		break;

	case 9:
		wstrTexName		=	L"PC_9.dds";
		break;

	default:
		ASSERT( false );
		break;
	}

	pPictureData->SetTex( wstrTexName.c_str() );

	pPictureData->SetSizeX( pPictureData->pTexture->pTexture->GetWidth() * 0.8f );
	pPictureData->SetSizeY( pPictureData->pTexture->pTexture->GetHeight() * 0.8f );

	pPictureData->SetShow( true );
}



HRESULT		CX2UIRankingInfo::OnFrameMove( double fTime, float fElapsedTime )
{	
	return S_OK;
}

bool		CX2UIRankingInfo::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// 앞에 모달 다이얼로그가 있으면 메시지를 처리하지 않는다
	if( true == g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckFrontModalDlg( m_pDlgRankingInfo ) )
		return false;

	bool bFlag	= false;

	switch( uMsg )
	{
		// 마우스 메시지들에 대해
	case WM_MOUSEMOVE:
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_LBUTTONDBLCLK:
	case WM_MBUTTONDBLCLK:
	case WM_RBUTTONDBLCLK:
		//case WM_MOUSEWHEEL:
		{
			if ( false == m_pDlgRankingInfo->GetIsMouseOver())
			{
				return false;
			}
			bFlag = true;
		}
		break;
	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
		{
			if ( false == m_pDlgRankingInfo->GetIsMouseOver())
			{
				return false;
			}
			g_pData->GetUIManager()->SetUILayerIndex(CX2UIManager::UI_MENU_RANKING_INFO, true);
			bFlag = true;			
		}
		break;
	default:
		break;
	}

	return bFlag;
}

bool		CX2UIRankingInfo::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{

	switch ( wParam )
	{
	case URICM_EXIT:
		{
#ifdef SERV_LOCAL_RANKING_SYSTEM
			if( NULL != m_pDlgRankingPopUp )
				m_pDlgRankingPopUp->SetShowEnable( false, false );
			g_pData->GetProfileManager()->CloseProfile();
#endif //SERV_LOCAL_RANKING_SYSTEM
			SetShow( false );

			if ( g_pTFieldGame != NULL )
			{
				CX2TFieldNpc *pJoinNpc = g_pTFieldGame->GetFieldNPC( g_pTFieldGame->GetJoinNpcIndex() );
				if ( pJoinNpc != NULL )
				{
					if ( pJoinNpc->GetNpcShop() != NULL )
						pJoinNpc->GetNpcShop()->SetKeyEvent();
				}
			}

			return true;
		}
		break;

	case URICM_SELECT_HENIR_TAP:
		{
			SetShowControlsByType( m_nowRankingInfoType, false );
			SetShowControlsByType( RT_DAY_RANKING, true );
			
			m_nowRankingInfoType	=	RT_DAY_RANKING;
			m_uiNowPage				=	1;

			Handler_EGS_GET_RANKING_INFO_REQ();
			return true;
		}
		break;

	case URICM_SELECT_PVP_TAP:
		{
			SetShowControlsByType( m_nowRankingInfoType, false );
			SetShowControlsByType( RT_PVP_RANKING, true );
			
			m_nowRankingInfoType	=	RT_PVP_RANKING;
			m_uiNowPage				=	1;

			Handler_EGS_GET_RANKING_INFO_REQ();

			return true;
		}
		break;

#ifdef SERV_LOCAL_RANKING_SYSTEM
	case URICM_SELECT_WEEK_PVP_TAP:
		{
			SetShowControlsByType( m_nowRankingInfoType, false );
			SetShowControlsByType( RT_WEEK_PVP_TAP, true );

			m_nowRankingInfoType	=	RT_WEEK_PVP_TAP;
			m_nowRankingInfoTypeSub	=	RT_AREA;
			m_uiNowPage				=	1;

			Handler_EGS_LOCAL_RANKING_INQUIRY_REQ();

			return true;
		}
		break;

	case URICM_SELECT_DUNGEON_FIELD_TAP:
		{
			SetShowControlsByType( m_nowRankingInfoType, false );
			SetShowControlsByType( RT_DUNGEON_FIELD_TAP, true );

			m_nowRankingInfoType	=	RT_DUNGEON_FIELD_TAP;
			m_nowRankingInfoTypeSub	=	RT_AREA;
			m_uiNowPage				=	1;

			Handler_EGS_LOCAL_RANKING_INQUIRY_REQ();
			
			return true;
		}
		break;

	case URICM_SELECT_AREA_RADIO_BUTTON:
		{
			m_nowRankingInfoTypeSub = RT_AREA;
			m_uiNowPage				= 1;

			Handler_EGS_LOCAL_RANKING_INQUIRY_REQ();
			return true;
		}
		break;

	case URICM_SELECT_GUILD_RADIO_BUTTON:
		{
			m_nowRankingInfoTypeSub = RT_GUILD;
			m_uiNowPage				= 1;

			Handler_EGS_LOCAL_RANKING_INQUIRY_REQ();
			return true;
		}
		break;

	case URICM_SELECT_CHARACTER_CHECK:
		{
			CKTDGUICheckBox* pCharacterCheck = reinterpret_cast<CKTDGUICheckBox*>(lParam);
			if( NULL != pCharacterCheck )
			{
				if( true == pCharacterCheck->GetChecked() )
				{
					SEnum::AddFlag( Is_Filter_Check, SEnum::LRF_UNIT );
				}
				else
				{
					SEnum::RemoveFlag( Is_Filter_Check, SEnum::LRF_UNIT );
				}
			}

			Handler_EGS_LOCAL_RANKING_INQUIRY_REQ();

			return true;
		}
		break;

	case URICM_SELECT_FRINED_CHECK:
		{
			CKTDGUICheckBox* pFriendCheck = reinterpret_cast<CKTDGUICheckBox*>(lParam);
			if( NULL != pFriendCheck )
			{
				if( true == pFriendCheck->GetChecked() )
				{
					SEnum::AddFlag( Is_Filter_Check, SEnum::LRF_FRIEND );
				}
				else
				{
					SEnum::RemoveFlag( Is_Filter_Check, SEnum::LRF_FRIEND );
				}
			}

			Handler_EGS_LOCAL_RANKING_INQUIRY_REQ();

			return true;
		}
		break;
	case URICM_SELECT_MALE_CHECK:
		{
			CKTDGUICheckBox* pMaleCheck = reinterpret_cast<CKTDGUICheckBox*>(lParam);
			CKTDGUICheckBox* pFemaleCheck = reinterpret_cast<CKTDGUICheckBox*>( m_pDlgRankingInfo->GetControl( L"femalecheck" ) );
			if( NULL != pMaleCheck && NULL != pFemaleCheck )
			{
				if( true == pMaleCheck->GetChecked() )
				{
					pFemaleCheck->SetChecked( false );
					SEnum::AddFlag( Is_Filter_Check, SEnum::LRF_GENDER_MALE );
					SEnum::RemoveFlag( Is_Filter_Check, SEnum::LRF_GENDER_FEMALE );
				}
				else
				{
					SEnum::RemoveFlag( Is_Filter_Check, SEnum::LRF_GENDER_MALE );
				}
			}

			Handler_EGS_LOCAL_RANKING_INQUIRY_REQ();

			return true;
		}
		break;

	case URICM_SELECT_FEMALE_CHECK:
		{
			CKTDGUICheckBox* pMaleCheck = reinterpret_cast<CKTDGUICheckBox*>( m_pDlgRankingInfo->GetControl( L"malecheck" ) );
			CKTDGUICheckBox* pFemalCheck = reinterpret_cast<CKTDGUICheckBox*>(lParam);
			if( NULL != pMaleCheck && NULL != pFemalCheck )
			{
				if( true == pFemalCheck->GetChecked() )
				{
					pMaleCheck->SetChecked( false );
					SEnum::AddFlag( Is_Filter_Check, SEnum::LRF_GENDER_FEMALE );
					SEnum::RemoveFlag( Is_Filter_Check, SEnum::LRF_GENDER_MALE );
				}
				else
				{
					SEnum::RemoveFlag( Is_Filter_Check, SEnum::LRF_GENDER_FEMALE );
				}
			}

			Handler_EGS_LOCAL_RANKING_INQUIRY_REQ();

			return true;
		}
		break;

#ifdef ADDED_RELATIONSHIP_SYSTEM
	case URICM_INVITE_COUPLE:
		{
			if ( NULL != m_columnForOthersFriendRank[m_iPickedUser].m_pStaticNickName &&
				 m_columnForOthersFriendRank[m_iPickedUser].m_pStaticNickName->GetString( 0 ) )
			{
				if ( NULL != g_pData->GetRelationshipManager() )
				{
					g_pData->GetRelationshipManager()->Send_EGS_COUPLE_PROPOSE_REQ 
						( m_columnForOthersFriendRank[m_iPickedUser].m_pStaticNickName->GetString( 0 )->msg );
				}
			}
		}
		break;
		
#endif // ADDED_RELATIONSHIP_SYSTEM
	case URICM_PAGE_SEARCH_FOCUS:
		{
			if( NULL != m_IMEEditPageSearch )
			{
				m_IMEEditPageSearch->ClearText();
				POINT pt; pt.x = 0; pt.y = 0;
				m_IMEEditPageSearch->HandleMouse( WM_LBUTTONDOWN, pt ,wParam, lParam );
			}

			return true;
		}
		break;
	case URICM_PAGE_SERACH_CHANGE:
		{
			if( NULL != m_IMEEditPageSearch )
			{
				int page = g_pMain->GetEDFromString( m_IMEEditPageSearch->GetText() );
				if( 1 > page )
					page = 1;
				else if( page > static_cast<int>(m_uiMaxPage) )
				{
					page = m_uiMaxPage;
				}

				m_IMEEditPageSearch->SetText( GET_REPLACED_STRING( ( STR_ID_2632, "i", page ) ) );
			}
			return true;
		}
		break;

	case URICM_PAGE_SEARCH_ENTER:
		{
			if( NULL != m_IMEEditPageSearch )
			{
				m_uiNowPage = g_pMain->GetEDFromString( m_IMEEditPageSearch->GetText() );
				m_uiNowPage = ( m_uiNowPage < 1 )? 1 : m_uiNowPage;
				m_IMEEditPageSearch->ClearText();

				switch(m_nowRankingInfoType)
				{
				case RT_WEEK_PVP_TAP:
				case RT_DUNGEON_FIELD_TAP:
					Handler_EGS_LOCAL_RANKING_INQUIRY_REQ();
					break;
				
				case RT_DAY_RANKING:
				case RT_WEEK_RANKING:
				case RT_MONTH_RANKING:
				case RT_LEVEL_RANKING:
				case RT_PVP_RANKING:
#ifdef NEW_HENIR_DUNGEON
				case RT_HERO_RANKING:
#endif // NEW_HENIR_DUNGEON
					Handler_EGS_GET_RANKING_INFO_REQ();
					break;
				default:
					return true;
				}
				
			}

			return true;
		}
		break;

	case URICM_OPEN_POPUP_MENU:
		{
			if( NULL != m_pDlgRankingPopUp )
			{
				m_iPickedUser = reinterpret_cast<CKTDGUIButton*>(lParam)->GetDummyInt(0);
				if( 9 == m_iPickedUser )
					return true;

				m_iPickedUser--;

				const CX2Unit::UnitData* pUnitData = NULL;
				if( NULL != g_pData->GetMyUser() && NULL != g_pData->GetMyUser()->GetSelectUnit())
					pUnitData = &g_pData->GetMyUser()->GetSelectUnit()->GetUnitData();
				if( NULL == pUnitData )
					return true;
				if( pUnitData->m_NickName == m_columnForOthersFriendRank[m_iPickedUser].m_pStaticNickName->GetString( 0 )->msg )
					return true;


				D3DXVECTOR2 vMousePos;
				vMousePos.x = static_cast<float>( g_pKTDXApp->GetDIManager()->GetMouse()->GetXPos() );
				vMousePos.y = static_cast<float>( g_pKTDXApp->GetDIManager()->GetMouse()->GetYPos() );
				vMousePos = g_pKTDXApp->MouseConvertByResolution( vMousePos );
				vMousePos.x -= 30.0f;
				vMousePos.y -= 30.0f;

				m_pDlgRankingPopUp->SetPos( vMousePos );
				m_pDlgRankingPopUp->SetShowEnable( true, true );

#ifdef ADDED_RELATIONSHIP_SYSTEM
				CKTDGUIButton* pRelationButton = static_cast <CKTDGUIButton*> ( m_pDlgRankingPopUp->GetControl(L"relation") );

				if ( NULL != pRelationButton &&
					 NULL != g_pData->GetRelationshipManager() && 
					 NULL != g_pData->GetRelationshipManager()->GetMyRelationshipInfo() )
				{
					pRelationButton->SetShowEnable ( false, false );
					if ( g_pData->GetRelationshipManager()->GetMyRelationshipInfo()->m_eRelationStateType ==
						SEnum::RT_SOLO )
					{
						pRelationButton->SetShowEnable ( true, true );
					}
				}
#endif // ADDED_RELATIONSHIP_SYSTEM
				return true;
			}
		}
		break;

	case URICM_DBCLK_CHAINFO:
		{
			m_iPickedUser = reinterpret_cast<CKTDGUIButton*>(lParam)->GetDummyInt(0);
			if( 9 == m_iPickedUser )
				return true;

			m_iPickedUser--;

			const CX2Unit::UnitData* pUnitData = NULL;
			if( NULL != g_pData->GetMyUser() && NULL != g_pData->GetMyUser()->GetSelectUnit())
				pUnitData = &g_pData->GetMyUser()->GetSelectUnit()->GetUnitData();
			if( NULL == pUnitData )
				return true;
			if( pUnitData->m_NickName == m_columnForOthersFriendRank[m_iPickedUser].m_pStaticNickName->GetString( 0 )->msg )
				return true;

			if( NULL != g_pData->GetProfileManager() )
			{
				m_sProfile.area				= m_columnForOthersFriendRank[m_iPickedUser].iArea;
				m_sProfile.birthYear		= m_columnForOthersFriendRank[m_iPickedUser].iBirth;
				m_sProfile.gender			= m_columnForOthersFriendRank[m_iPickedUser].byteGender;
				m_sProfile.nickname			= m_columnForOthersFriendRank[m_iPickedUser].m_pStaticNickName->GetString( 0 )->msg;
				m_sProfile.introduce		= m_columnForOthersFriendRank[m_iPickedUser].m_wstsrIntroduce;
				m_sProfile.level			= m_columnForOthersFriendRank[m_iPickedUser].ucLevel;
				m_sProfile.UnitClass		= m_columnForOthersFriendRank[m_iPickedUser].UnitClass;

				g_pData->GetProfileManager()->SetProfile( &m_sProfile );
				g_pData->GetProfileManager()->OpenProfile( false );
			}

			return true;
		}
		break;

	case URICM_POPUP_SELECT_CHARINFO:
		{
			if( NULL != m_pDlgRankingPopUp )
				m_pDlgRankingPopUp->SetShowEnable( false, false );

			if( NULL != g_pData->GetProfileManager() )
			{
				m_sProfile.area				= m_columnForOthersFriendRank[m_iPickedUser].iArea;
				m_sProfile.birthYear		= m_columnForOthersFriendRank[m_iPickedUser].iBirth;
				m_sProfile.gender			= m_columnForOthersFriendRank[m_iPickedUser].byteGender;
				m_sProfile.nickname			= m_columnForOthersFriendRank[m_iPickedUser].m_pStaticNickName->GetString( 0 )->msg;
				m_sProfile.introduce		= m_columnForOthersFriendRank[m_iPickedUser].m_wstsrIntroduce;
				m_sProfile.level			= m_columnForOthersFriendRank[m_iPickedUser].ucLevel;
				m_sProfile.UnitClass		= m_columnForOthersFriendRank[m_iPickedUser].UnitClass;

				g_pData->GetProfileManager()->SetProfile( &m_sProfile );
				g_pData->GetProfileManager()->OpenProfile( false );
			}

			return true;
		}
		break;

	case URICM_POPUP_SELECT_FRINED:
		{
			if( NULL != m_pDlgRankingPopUp )
				m_pDlgRankingPopUp->SetShowEnable( false, false );

			if( NULL != g_pData->GetMessenger() )
				g_pData->GetMessenger()->Handler_EGS_KNM_REQUEST_NEW_FRIEND_INFO_REQ( m_columnForOthersFriendRank[m_iPickedUser].m_pStaticNickName->GetString( 0 )->msg.c_str() );

			return true;
		}
		break;
			
	case URICM_POPUP_SELECT_WHISPER:
		{
			if( NULL != m_pDlgRankingPopUp )
				m_pDlgRankingPopUp->SetShowEnable( false, false );

			if( NULL != g_pChatBox )
			{
				g_pChatBox->SetPickedUser( m_columnForOthersFriendRank[m_iPickedUser].m_iUserUID, m_columnForOthersFriendRank[m_iPickedUser].m_pStaticNickName->GetString( 0 )->msg );
				return g_pChatBox->OnOpenWhisperChatMode();
			}
		}
		break;

	case URICM_DUMMY_SELECT:
		{
			if( NULL != m_sCommonMenu.m_pRadioGuild )
			{
				m_sCommonMenu.m_pRadioGuild->SetChecked( false );
				m_sCommonMenu.m_pRadioArea->SetChecked( true );

			}
			return true;
		}
		break;
#endif //SERV_LOCAL_RANKING_SYSTEM

	case URICM_SELECT_LEVEL_TAP:
		{
			SetShowControlsByType( m_nowRankingInfoType, false );
			SetShowControlsByType( RT_LEVEL_RANKING, true );
			
			m_nowRankingInfoType	=	RT_LEVEL_RANKING;
			m_uiNowPage				=	1;

			Handler_EGS_GET_RANKING_INFO_REQ();
			return true;
		}
		break;

	case URICM_SELECT_DAY_RADIO_BUTTON:
		{
			m_nowRankingInfoType	=	RT_DAY_RANKING;
			m_uiNowPage				=	1;

			Handler_EGS_GET_RANKING_INFO_REQ();

			return true;
		}
		break;

	case URICM_SELECT_WEEK_RADIO_BUTTON:
		{
			m_nowRankingInfoType	=	RT_WEEK_RANKING;
			m_uiNowPage				=	1;

			Handler_EGS_GET_RANKING_INFO_REQ();

			return true;
		}
		break;

	case URICM_SELECT_MONTH_RADIO_BUTTON:
		{
			m_nowRankingInfoType	=	RT_MONTH_RANKING;
			m_uiNowPage				=	1;

			Handler_EGS_GET_RANKING_INFO_REQ();

			return true;
		}
		break;

	case URICM_PUSH_PREV_PAGE:
		{
			if ( m_uiNowPage > 1 )
			{
				m_uiNowPage--;

#ifdef SERV_LOCAL_RANKING_SYSTEM
				if( m_nowRankingInfoType == RT_WEEK_PVP_TAP || m_nowRankingInfoType == RT_DUNGEON_FIELD_TAP )
					Handler_EGS_LOCAL_RANKING_INQUIRY_REQ();
				else
#endif //SERV_LOCAL_RANKING_SYSTEM
				Handler_EGS_GET_RANKING_INFO_REQ();
			}
			
			return true;
		}
		break;

	case URICM_PUSH_NEXT_PAGE:
		{
			if ( m_uiNowPage < m_uiMaxPage )
			{
				m_uiNowPage++;

#ifdef SERV_LOCAL_RANKING_SYSTEM
				if( m_nowRankingInfoType == RT_WEEK_PVP_TAP || m_nowRankingInfoType == RT_DUNGEON_FIELD_TAP )
					Handler_EGS_LOCAL_RANKING_INQUIRY_REQ();
				else
#endif //SERV_LOCAL_RANKING_SYSTEM
				Handler_EGS_GET_RANKING_INFO_REQ();
			}

			return true;
		}
		break;

		//{{ 허상형 : [2009/8/21] //	신학기 이벤트 메시지
#ifdef NEW_TERM_EVENT
	case URICM_SELECT_EVENT_TAP:
		{
			SetShowControlsByType( m_nowRankingInfoType, false );
			SetShowControlsByType( RT_EVENT_DUNGEON, true );

			m_nowRankingInfoType	=	RT_EVENT_DUNGEON;
			m_uiNowPage				=	1;

			Handler_EGS_GET_RANKING_INFO_REQ();

			return true;
		}
		break;

	case URICM_SELECT_EVENT_HENIR_BUTTON:
		{
			m_nowRankingInfoType	=	RT_EVENT_HENIR;
			m_uiNowPage				=	1;

			Handler_EGS_GET_RANKING_INFO_REQ();

			return true;
		}
		break;
	case URICM_SELECT_EVENT_DUNGEON_BUTTON:
		{
			m_nowRankingInfoType	=	RT_EVENT_DUNGEON;
			m_uiNowPage				=	1;

			Handler_EGS_GET_RANKING_INFO_REQ();

			return true;
		}
		break;
	case URICM_SELECT_EVENT_KILL_BUTTON:
		{
			m_nowRankingInfoType	=	RT_EVENT_PVP_KILL;
			m_uiNowPage				=	1;

			Handler_EGS_GET_RANKING_INFO_REQ();

			return true;
		}
		break;
	case URICM_SELECT_EVENT_DEATH_BUTTON:
		{
			m_nowRankingInfoType	=	RT_EVENT_PVP_DIE;
			m_uiNowPage				=	1;

			Handler_EGS_GET_RANKING_INFO_REQ();

			return true;
		}
		break;
#endif	//	NEW_TERM_EVENT
		//}} 허상형 : [2009/8/21] //	신학기 이벤트 메시지
#ifdef NEW_HENIR_DUNGEON
	case URICM_SELECT_HERO_BUTTON:
		{
			m_nowRankingInfoType	=	RT_HERO_RANKING;
			m_uiNowPage				=	1;

			Handler_EGS_GET_RANKING_INFO_REQ();

			return true;
		} break;
#endif // NEW_HENIR_DUNGEON

	default:
		break;
	}

	return false;
}

bool		CX2UIRankingInfo::UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch ( wParam )
	{
	case EGS_GET_RANKING_INFO_ACK:
		{
			return Handler_EGS_GET_RANKING_INFO_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;

#ifdef SERV_LOCAL_RANKING_SYSTEM
	case EGS_LOCAL_RANKING_INQUIRY_ACK:
		{
			return Handler_EGS_LOCAL_RANKING_INQUIRY_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;
#endif //SERV_LOCAL_RANKING_SYSTEM

	default:
		break;
	}

	return false;
}

bool		CX2UIRankingInfo::GetShow() const
{
	return m_bShow;
}

void		CX2UIRankingInfo::SetShow( bool bShow )
{
	m_bShow = bShow;

	if ( bShow )
	{
		UpdatePageUI();	
		InitMyColumn();
		InitOthersColumn();
		InitUIRankingInfo();
#ifdef SERV_LOCAL_RANKING_SYSTEM
		InitMyColumnFriend();
		InitOthersColumnFriend();
	#ifdef SERV_LOCAL_RANKING_SYSTEM_STATISTICS_LOG
		Handler_EGS_LOCAL_RANKING_INQUIRY_REQ(true);
	#else
		Handler_EGS_LOCAL_RANKING_INQUIRY_REQ();
	#endif //SERV_LOCAL_RANKING_SYSTEM_STATISTICS_LOG
#else
		Handler_EGS_GET_RANKING_INFO_REQ();
#endif //SERV_LOCAL_RANKING_SYSTEM
		g_pData->GetUIManager()->UIOpened( CX2UIManager::UI_MENU_RANKING_INFO );
	}
	else
	{
		g_pData->GetUIManager()->UIClosed( CX2UIManager::UI_MENU_RANKING_INFO );
	}

	m_pDlgRankingInfo->SetShowEnable( m_bShow, m_bShow );
}

D3DXVECTOR2	CX2UIRankingInfo::GetDLGSize() const
{
	return m_vSizeDlg;		// 임시
}

void		CX2UIRankingInfo::SetPosition( D3DXVECTOR2 vec )
{
	m_vPosDlg = vec;
	m_pDlgRankingInfo->SetPos( m_vPosDlg );
}

void		CX2UIRankingInfo::SetLayer( X2_DIALOG_LAYER layer )
{
	if( m_pDlgRankingInfo != NULL) 
	{
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer( m_pDlgRankingInfo, layer);
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDlgRankingInfo, false );
	}
}


#ifdef SERV_LOCAL_RANKING_SYSTEM
#ifdef SERV_LOCAL_RANKING_SYSTEM_STATISTICS_LOG
bool CX2UIRankingInfo::Handler_EGS_LOCAL_RANKING_INQUIRY_REQ( bool IsBoardButton/* = false*/)
#else
bool CX2UIRankingInfo::Handler_EGS_LOCAL_RANKING_INQUIRY_REQ()
#endif //SERV_LOCAL_RANKING_SYSTEM_STATISTICS_LOG
{
	KEGS_LOCAL_RANKING_INQUIRY_REQ kPacket;

	switch( m_nowRankingInfoType )
	{
	case RT_WEEK_PVP_TAP:
		kPacket.m_cMainTabIndex = SEnum::LRMTC_AP;
		break;

	case RT_DUNGEON_FIELD_TAP:
		kPacket.m_cMainTabIndex = SEnum::LRMTC_SPIRIT;
		break;
	default:
		return false;
	}
	
	switch( m_nowRankingInfoTypeSub )
	{
	case RT_AREA:
		kPacket.m_cSubTabIndex = SEnum::LRSTC_LOCATION;
		break;
		
	case RT_GUILD:
		kPacket.m_cSubTabIndex = SEnum::LRSTC_GUILD;
		break;
	default:
		return false;
	}

	kPacket.m_nViewPage		= m_uiNowPage;
	kPacket.m_byteFilter	= Is_Filter_Check;

#ifdef SERV_LOCAL_RANKING_SYSTEM_STATISTICS_LOG
	kPacket.m_bRankingButtonClick = IsBoardButton;
#endif //SERV_LOCAL_RANKING_SYSTEM_STATISTICS_LOG

	g_pData->GetServerProtocol()->SendPacket( EGS_LOCAL_RANKING_INQUIRY_REQ, kPacket ); 
//	g_pMain->AddServerPacket( EGS_LOCAL_RANKING_INQUIRY_ACK, 60.f ); 


	return true;
}

bool CX2UIRankingInfo::Handler_EGS_LOCAL_RANKING_INQUIRY_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_LOCAL_RANKING_INQUIRY_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

//	if( true == g_pMain->DeleteServerPacket( EGS_LOCAL_RANKING_INQUIRY_ACK ) )
//	{

		if( true == g_pMain->IsValidPacket( kEvent.m_iOK ) )
		{
			switch( static_cast<SEnum::LOCAL_RAKING_MAIN_TAB_CATEGORY>( kEvent.m_cMainTabIndex) )
			{
			case SEnum::LRMTC_SPIRIT:
				m_nowRankingInfoType = RT_DUNGEON_FIELD_TAP;
				break;

			case SEnum::LRMTC_AP:
				m_nowRankingInfoType = RT_WEEK_PVP_TAP;
				break;
			default:
				return false;
			}

			switch( static_cast<SEnum::LOCAL_RAKING_SUB_TAB_CATEGORY>( kEvent.m_cSubTabIndex ) )
			{
			case SEnum::LRSTC_LOCATION:
				m_nowRankingInfoTypeSub = RT_AREA;
				break;
					
			case SEnum::LRSTC_GUILD:
				m_nowRankingInfoTypeSub = RT_GUILD;
				break;
			default:
				return false;
			}
			
			// 랭킹 정보가 우선 다 보이지 않도록 함
			for( int i = 0; i < _CONST_UIRANKING_INFO_::g_iNumRankingPerPage; i++)
				SetShowOthersInfoByType( (m_columnForOthersRank + i), RT_MAX_NUM, false );
			SetShowMyInfoByType( RT_MAX_NUM, false );

			for( int i = 0; i <_CONST_UIRANKING_FRIEND_INFO_::g_iNumRankingPerPage; i++ )
				SetShowInfoByType_F( (m_columnForOthersFriendRank + i), false );
			SetShowInfoByType_F( &m_columnForMyFriendRank, false );


			// 현재 페이지 수와 전체 페이지 수 업데이트
			if ( kEvent.m_nTotalPage > _CONST_UIRANKING_FRIEND_INFO_::g_iMaxPageLimit )
			{
				m_uiMaxPage				= _CONST_UIRANKING_FRIEND_INFO_::g_iMaxPageLimit;
			}
			else
			{
				m_uiMaxPage				=  max( 1, kEvent.m_nTotalPage );
			}

			m_uiNowPage				= kEvent.m_nViewPage;
			
			UpdatePageUI();

			int index = 0;
			wstring fileName;
			wstring pieceName;

#ifdef LOCAL_RANKING_UPDATE

			CKTDGUIStatic* pStatic = static_cast<CKTDGUIStatic*>(m_pDlgRankingInfo->GetControl( L"ranking_null" ));

			if( -1 == kEvent.m_kMyRankInfo.m_kUserInfo.m_iCategory )
			{
				ChangeBar( BS_NONE );
				if( NULL != pStatic &&  NULL != pStatic->GetPicture(0) )
				{
					//pStatic->GetPicture(0)->SetShow(true);
					if( NULL != pStatic->GetPicture(1) && NULL != pStatic->GetPicture(2) )
					{
						if( true == SEnum::CheckFlag( kEvent.m_kMyRankInfo.m_kUserInfo.m_byteFilter, SEnum::LRF_USER_INFO_CATEGORY ) )
						{
							//지역 정보 비공개했을때
							pStatic->GetPicture(1)->SetTex(L"DLG_UI_Common_Texture_LocalAdd_Info.TGA", L"Title_LocalInfo_2");
							pStatic->GetPicture(2)->SetTex(L"DLG_UI_Common_Texture_LocalAdd_Info.TGA", L"Text_LocalInfo_2");
						}
						else
						{
							//프로필을 입력 안했을때
							pStatic->GetPicture(1)->SetTex(L"DLG_UI_Common_Texture_LocalAdd_Info.TGA", L"Title_LocalInfo_1");
							pStatic->GetPicture(2)->SetTex(L"DLG_UI_Common_Texture_LocalAdd_Info.TGA", L"Text_LocalInfo_1");
						}
					}
					pStatic->SetShowEnable( true, true );
				}
			}
			else
			{
				if( NULL != pStatic )
					pStatic->SetShowEnable( false, false );
#endif //LOCAL_RANKING_UPDATE
			ChangeBar( BS_NEWBAR );	
			
			//제일 위에서부터 7명 
			BOOST_TEST_FOREACH( KLocalRankingRankerInfo&, kLocalRankingRankerInfo, kEvent.m_vecRankerList )
			{
				if( kLocalRankingRankerInfo.m_kUnitInfo.m_iUnitUID == kEvent.m_kMyRankInfo.m_kUnitInfo.m_iUnitUID )
				{
					kEvent.m_kMyRankInfo.m_kUnitInfo.m_iPoint[0] = kLocalRankingRankerInfo.m_kUnitInfo.m_iPoint[0];
					kEvent.m_kMyRankInfo.m_kUnitInfo.m_iPoint[1] = kLocalRankingRankerInfo.m_kUnitInfo.m_iPoint[1];
				}

				if( 0 != kLocalRankingRankerInfo.m_iRank )
				{
					// 닉네임
					m_columnForOthersFriendRank[index].m_pStaticNickName->SetString( 0, kLocalRankingRankerInfo.m_kUnitInfo.m_wstrNickName.c_str() );	
					m_columnForOthersFriendRank[index].m_pStaticNickName->SetShowEnable( true, true );
					m_columnForOthersFriendRank[index].m_pButtonPopUp->SetShowEnable( true, true );

					//순위
					m_columnForOthersFriendRank[index].m_iRanking = kLocalRankingRankerInfo.m_iRank;
					SetRank( m_columnForOthersFriendRank[index].m_pStaticRanking, m_columnForOthersFriendRank[index].m_iRanking );

					//근성 or AP
					switch( m_nowRankingInfoType )
					{
						case RT_WEEK_PVP_TAP:
							{
								SetPoint( m_columnForOthersFriendRank[index].m_pStaticPoint, kLocalRankingRankerInfo.m_kUnitInfo.m_iPoint[SEnum::LRMTC_AP] );
							}
							break;

						case RT_DUNGEON_FIELD_TAP:
							{
								SetPoint( m_columnForOthersFriendRank[index].m_pStaticPoint, kLocalRankingRankerInfo.m_kUnitInfo.m_iPoint[SEnum::LRMTC_SPIRIT] );
							}
							break;

						default:
							{
								SetPoint( m_columnForOthersFriendRank[index].m_pStaticPoint, 0 );
							}
							break;
					}

					//레벨 저장
					m_columnForOthersFriendRank[index].ucLevel = kLocalRankingRankerInfo.m_kUnitInfo.m_ucLevel;
					//자기 소개 저장
					m_columnForOthersFriendRank[index].m_wstsrIntroduce = kLocalRankingRankerInfo.m_kUserInfo.m_wstrProfile;
					//생일 저장
					m_columnForOthersFriendRank[index].iBirth = kLocalRankingRankerInfo.m_kUserInfo.m_iBirth;
					//지역 저장
					m_columnForOthersFriendRank[index].iArea = kLocalRankingRankerInfo.m_kUserInfo.m_iCategory;
				

				
					//직업
					m_columnForOthersFriendRank[index].UnitClass = kLocalRankingRankerInfo.m_kUnitInfo.m_cUnitClass;
					if( CX2Unit::UC_NONE != static_cast<CX2Unit::UNIT_CLASS>( kLocalRankingRankerInfo.m_kUnitInfo.m_cUnitClass ) )
					if( true == CX2Data::GetCharacterImageName( fileName, pieceName, static_cast<CX2Unit::UNIT_CLASS>( kLocalRankingRankerInfo.m_kUnitInfo.m_cUnitClass ), CX2Data::CIT_50by50 ) )
					{
						m_columnForOthersFriendRank[index].UnitClass = kLocalRankingRankerInfo.m_kUnitInfo.m_cUnitClass;
						m_columnForOthersFriendRank[index].m_pStaticClass->GetPicture( 0 )->SetTex( fileName.c_str(), pieceName.c_str() );
						m_columnForOthersFriendRank[index].m_pStaticClass->GetPicture( 0 )->SetShow( true );
						m_columnForOthersFriendRank[index].m_pStaticClass->SetShowEnable( true, true );
					}

					//성별
					m_columnForOthersFriendRank[index].byteGender = kLocalRankingRankerInfo.m_kUserInfo.m_byteGender;
					SetGender( m_columnForOthersFriendRank[index].m_pStaticGender, static_cast<GENDER_SELECT>( kLocalRankingRankerInfo.m_kUserInfo.m_byteGender ) );
				}
				index++;
			}

			
			//자신 바로 위에 있는 유저
			if( 0 != kEvent.m_kPrevRankerInfo.m_iRank && 1 != kEvent.m_kMyRankInfo.m_iRank )
			{
				// 닉네임
				m_columnForOthersFriendRank[_CONST_UIRANKING_FRIEND_INFO_::g_iNumRankingPerPage-1].m_pStaticNickName->SetString( 0, kEvent.m_kPrevRankerInfo.m_kUnitInfo.m_wstrNickName.c_str() );	
				m_columnForOthersFriendRank[_CONST_UIRANKING_FRIEND_INFO_::g_iNumRankingPerPage-1].m_pStaticNickName->SetShowEnable( true, true );
				m_columnForOthersFriendRank[_CONST_UIRANKING_FRIEND_INFO_::g_iNumRankingPerPage-1].m_pButtonPopUp->SetShowEnable( true, true );

				//순위
				m_columnForOthersFriendRank[_CONST_UIRANKING_FRIEND_INFO_::g_iNumRankingPerPage-1].m_iRanking = kEvent.m_kPrevRankerInfo.m_iRank;
				SetRank( m_columnForOthersFriendRank[_CONST_UIRANKING_FRIEND_INFO_::g_iNumRankingPerPage-1].m_pStaticRanking, m_columnForOthersFriendRank[_CONST_UIRANKING_FRIEND_INFO_::g_iNumRankingPerPage-1].m_iRanking );

				//근성 or AP
				switch( m_nowRankingInfoType )
				{
				case RT_WEEK_PVP_TAP:
					{
						SetPoint( m_columnForOthersFriendRank[_CONST_UIRANKING_FRIEND_INFO_::g_iNumRankingPerPage-1].m_pStaticPoint, kEvent.m_kPrevRankerInfo.m_kUnitInfo.m_iPoint[SEnum::LRMTC_AP] );
					}
					break;

				case RT_DUNGEON_FIELD_TAP:
					{
						SetPoint( m_columnForOthersFriendRank[_CONST_UIRANKING_FRIEND_INFO_::g_iNumRankingPerPage-1].m_pStaticPoint, kEvent.m_kPrevRankerInfo.m_kUnitInfo.m_iPoint[SEnum::LRMTC_SPIRIT] );
					}
					break;

				default:
					{
						SetPoint( m_columnForOthersFriendRank[_CONST_UIRANKING_FRIEND_INFO_::g_iNumRankingPerPage-1].m_pStaticPoint, 0 );
					}
					break;
				}

				//레벨 저장
				m_columnForOthersFriendRank[_CONST_UIRANKING_FRIEND_INFO_::g_iNumRankingPerPage-1].ucLevel = kEvent.m_kPrevRankerInfo.m_kUnitInfo.m_ucLevel;
				//자기 소개 저장
				m_columnForOthersFriendRank[_CONST_UIRANKING_FRIEND_INFO_::g_iNumRankingPerPage-1].m_wstsrIntroduce = kEvent.m_kPrevRankerInfo.m_kUserInfo.m_wstrProfile;
				//생일 저장
				m_columnForOthersFriendRank[_CONST_UIRANKING_FRIEND_INFO_::g_iNumRankingPerPage-1].iBirth =kEvent.m_kPrevRankerInfo.m_kUserInfo.m_iBirth;
				//지역 저장
				m_columnForOthersFriendRank[_CONST_UIRANKING_FRIEND_INFO_::g_iNumRankingPerPage-1].iArea = kEvent.m_kPrevRankerInfo.m_kUserInfo.m_iCategory;


		

				//직업
				//m_columnForOthersFriendRank[index].UnitClass = kEvent.m_kPrevRankerInfo.m_kUnitInfo.m_cUnitClass;
				if( CX2Unit::UC_NONE != static_cast<CX2Unit::UNIT_CLASS>( kEvent.m_kPrevRankerInfo.m_kUnitInfo.m_cUnitClass ) )
				if( true == CX2Data::GetCharacterImageName( fileName, pieceName, static_cast<CX2Unit::UNIT_CLASS>( kEvent.m_kPrevRankerInfo.m_kUnitInfo.m_cUnitClass ), CX2Data::CIT_50by50 ) )
				{
					m_columnForOthersFriendRank[_CONST_UIRANKING_FRIEND_INFO_::g_iNumRankingPerPage-1].UnitClass = kEvent.m_kPrevRankerInfo.m_kUnitInfo.m_cUnitClass;
					m_columnForOthersFriendRank[_CONST_UIRANKING_FRIEND_INFO_::g_iNumRankingPerPage-1].m_pStaticClass->GetPicture( 0 )->SetTex( fileName.c_str(), pieceName.c_str() );
					m_columnForOthersFriendRank[_CONST_UIRANKING_FRIEND_INFO_::g_iNumRankingPerPage-1].m_pStaticClass->GetPicture( 0 )->SetShow( true );
					m_columnForOthersFriendRank[_CONST_UIRANKING_FRIEND_INFO_::g_iNumRankingPerPage-1].m_pStaticClass->SetShowEnable( true, true );
				}

				//성별
				m_columnForOthersFriendRank[_CONST_UIRANKING_FRIEND_INFO_::g_iNumRankingPerPage-1].byteGender = kEvent.m_kPrevRankerInfo.m_kUserInfo.m_byteGender;
				SetGender( m_columnForOthersFriendRank[_CONST_UIRANKING_FRIEND_INFO_::g_iNumRankingPerPage-1].m_pStaticGender, static_cast<GENDER_SELECT>( kEvent.m_kPrevRankerInfo.m_kUserInfo.m_byteGender ) );

			}


#ifdef LOCAL_RANKING_UPDATE
			}
#endif //LOCAL_RANKING_UPDATE

			//자신의 정보
			//닉네임
			m_columnForMyFriendRank.m_pStaticNickName->SetString( 0, kEvent.m_kMyRankInfo.m_kUnitInfo.m_wstrNickName.c_str() );
			m_columnForMyFriendRank.m_pStaticNickName->SetShowEnable( true, true );
			m_columnForMyFriendRank.m_pButtonPopUp->SetShowEnable( true, true );
		
			//순위
			if( 0 != kEvent.m_kMyRankInfo.m_iRank)
			{
				m_columnForMyFriendRank.m_iRanking = kEvent.m_kMyRankInfo.m_iRank;
				SetRank( m_columnForMyFriendRank.m_pStaticRanking, m_columnForMyFriendRank.m_iRanking );
				m_columnForMyFriendRank.m_pStaticRanking->SetShowEnable( true, true );
			}
			else
				m_columnForMyFriendRank.m_pStaticRanking->SetShowEnable( false, false );

			//직업
			if( true == CX2Data::GetCharacterImageName( fileName, pieceName, static_cast<CX2Unit::UNIT_CLASS>( kEvent.m_kMyRankInfo.m_kUnitInfo.m_cUnitClass ), CX2Data::CIT_50by50 ) )
			{
				m_columnForMyFriendRank.m_pStaticClass->GetPicture( 0 )->SetTex( fileName.c_str(), pieceName.c_str() );
				m_columnForMyFriendRank.m_pStaticClass->GetPicture( 0 )->SetShow( true );
				m_columnForMyFriendRank.m_pStaticClass->SetShowEnable( true, true );
			}

			//성별
			SetGender( m_columnForMyFriendRank.m_pStaticGender, static_cast<GENDER_SELECT>( kEvent.m_kMyRankInfo.m_kUserInfo.m_byteGender ) );

			//근성 or AP
			switch( m_nowRankingInfoType )
			{
			case RT_WEEK_PVP_TAP:
				{
					SetPoint( m_columnForMyFriendRank.m_pStaticPoint, kEvent.m_kMyRankInfo.m_kUnitInfo.m_iPoint[SEnum::LRMTC_AP] );
				}
				break;

			case RT_DUNGEON_FIELD_TAP:
				{
					SetPoint( m_columnForMyFriendRank.m_pStaticPoint, kEvent.m_kMyRankInfo.m_kUnitInfo.m_iPoint[SEnum::LRMTC_SPIRIT] );
				}
				break;

			default:
				{
					SetPoint( m_columnForMyFriendRank.m_pStaticPoint, 0 );
				}
				break;
			}
		}
//	}

	return true;
}

void CX2UIRankingInfo::ChangeBar( BAR_SELECT _barSelect )
{
	CKTDGUIStatic* pOldStaticBar = static_cast<CKTDGUIStatic* >( m_pDlgRankingInfo->GetControl( L"staticOldBar" ) );
	if( NULL == pOldStaticBar )
		return;
	CKTDGUIStatic* pNewStaticBar = static_cast<CKTDGUIStatic* >( m_pDlgRankingInfo->GetControl( L"staticNewBar" ) );
	if( NULL == pNewStaticBar )
		return;

	switch (_barSelect)
	{
	case BS_OLDBAR:
		{
			int OldPictureNum = pOldStaticBar->GetPictureNum();
			for( int i = 0; i < OldPictureNum; i++ )
				pOldStaticBar->GetPicture( i )->SetShow( true );

			int NewPictureNum = pNewStaticBar->GetPictureNum();
			for( int i = 0; i < NewPictureNum; i++ )
				pNewStaticBar->GetPicture( i )->SetShow( false );

			if( NULL != m_columnForMyFriendRank.m_pButtonPopUp )
				m_columnForMyFriendRank.m_pButtonPopUp->SetShowEnable( false, false );

			for( int i = 0; i < _CONST_UIRANKING_FRIEND_INFO_::g_iNumRankingPerPage; i++ )
				if( NULL != m_columnForOthersFriendRank[i].m_pButtonPopUp )
					m_columnForOthersFriendRank[i].m_pButtonPopUp->SetShowEnable( false, false );

			if( NULL != m_pStaticNotice)
				m_pStaticNotice->SetString( 0, GET_STRING(STR_ID_3732) );
		}
		break;

	case BS_NEWBAR:
		{
			int OldPictureNum = pOldStaticBar->GetPictureNum();
			for( int i = 0; i < OldPictureNum; i++ )
				pOldStaticBar->GetPicture( i )->SetShow( false );

			int NewPictureNum = pNewStaticBar->GetPictureNum();
			for( int i = 0; i < NewPictureNum; i++ )
				pNewStaticBar->GetPicture( i )->SetShow( true );

			if( NULL != m_pStaticNotice)
				m_pStaticNotice->SetString( 0, GET_STRING(STR_ID_23971) );

		}
		break;

#ifdef LOCAL_RANKING_UPDATE
	case BS_NONE:
		{
			int OldPictureNum = pOldStaticBar->GetPictureNum();
			for( int i = 0; i < OldPictureNum; i++ )
				pOldStaticBar->GetPicture( i )->SetShow( false );

			int NewPictureNum = pNewStaticBar->GetPictureNum();
			for( int i = 0; i < NewPictureNum; i++ )
				pNewStaticBar->GetPicture( i )->SetShow( false );

			if( NULL != m_pStaticNotice)
				m_pStaticNotice->SetString( 0, GET_STRING(STR_ID_23971) );
		}
		break;
#endif //LOCAL_RANKING_UPDATE
	default:
		break;
	}
}

void CX2UIRankingInfo::InitMyColumnFriend()
{
	//클래스 텍스처
	m_columnForMyFriendRank.m_pStaticClass = static_cast<CKTDGUIStatic* >( m_pDlgRankingInfo->GetControl( L"Static_ClassF_9" ) );
	if( NULL != m_columnForMyFriendRank.m_pStaticClass )
	{
		if( NULL != m_columnForMyFriendRank.m_pStaticClass->GetPicture( 0 ) )
			m_columnForMyFriendRank.m_pStaticClass->GetPicture( 0 )->SetShow( true );

		m_columnForMyFriendRank.m_pStaticClass->SetShowEnable( false, false );
	}

	//성별 텍스쳐
	m_columnForMyFriendRank.m_pStaticGender = static_cast<CKTDGUIStatic* >( m_pDlgRankingInfo->GetControl( L"Static_Gender9" ) );
	if( NULL != m_columnForMyFriendRank.m_pStaticGender )
	{
		if( NULL != m_columnForMyFriendRank.m_pStaticGender->GetPicture( 0 ) )
			m_columnForMyFriendRank.m_pStaticGender->GetPicture( 0 )->SetShow( true );

		m_columnForMyFriendRank.m_pStaticGender->SetShowEnable( false, false );
	}

	//닉네임 스트링
	m_columnForMyFriendRank.m_pStaticNickName = static_cast<CKTDGUIStatic* >( m_pDlgRankingInfo->GetControl( L"Id_F_9" ) );
	if( NULL != m_columnForMyFriendRank.m_pStaticNickName )
	{
		if( NULL != m_columnForMyFriendRank.m_pStaticNickName->GetString( 0 ) )
			m_columnForMyFriendRank.m_pStaticNickName->GetString( 0 )->msg = L"";
		m_columnForMyFriendRank.m_pStaticNickName->SetShowEnable( false, false );
	}

	//순위 텍스쳐
	m_columnForMyFriendRank.m_pStaticRanking = static_cast<CKTDGUIStatic* >( m_pDlgRankingInfo->GetControl( L"Static_RankF9" ) );
	if( NULL != m_columnForMyFriendRank.m_pStaticRanking )
	{
		for( int i = 0; i < _CONST_UIRANKING_FRIEND_INFO_::g_iRankLimit; i++ )
			if( NULL != m_columnForMyFriendRank.m_pStaticRanking->GetPicture( i ) )
				m_columnForMyFriendRank.m_pStaticRanking->GetPicture( i )->SetShow( true );

		m_columnForMyFriendRank.m_pStaticRanking->SetShowEnable( false, false );
	}

	//근성도 or 주간 누적 AP 수치 텍스쳐
	m_columnForMyFriendRank.m_pStaticPoint = static_cast<CKTDGUIStatic* >( m_pDlgRankingInfo->GetControl( L"Static_Strong9" ) );
	if( NULL != m_columnForMyFriendRank.m_pStaticPoint )
	{
		for( int i = 0; i < _CONST_UIRANKING_FRIEND_INFO_::g_iLimitStrongGage; i++ )
			if( NULL != m_columnForMyFriendRank.m_pStaticPoint->GetPicture( i ) )
				m_columnForMyFriendRank.m_pStaticPoint->GetPicture( i )->SetShow( true );

		m_columnForMyFriendRank.m_pStaticPoint->SetShowEnable( false, false );
	}
	
	//투명 버튼
	m_columnForMyFriendRank.m_pButtonPopUp = static_cast<CKTDGUIButton* >( m_pDlgRankingInfo->GetControl( L"User9" ) );
	if( NULL != m_columnForMyFriendRank.m_pButtonPopUp )
	{
		m_columnForMyFriendRank.m_pButtonPopUp->SetShowEnable( false, false );
	}

}
void CX2UIRankingInfo::InitOthersColumnFriend()
{
	for( int i = 0; i < _CONST_UIRANKING_FRIEND_INFO_::g_iNumRankingPerPage; i++ )
	{
		//클래스 텍스쳐
		m_columnForOthersFriendRank[i].m_pStaticClass = 
			static_cast<CKTDGUIStatic* >( m_pDlgRankingInfo->GetControl( GET_REPLACED_STRING( ( STR_ID_3738, "Li", std::wstring( L"Static_ClassF_" ), i + 1 ) ) ) );
		if( NULL != m_columnForOthersFriendRank[i].m_pStaticClass )
		{
			if( NULL != m_columnForOthersFriendRank[i].m_pStaticClass->GetPicture( 0 ) )
				 m_columnForOthersFriendRank[i].m_pStaticClass->GetPicture( 0 )->SetShow( true );

			 m_columnForOthersFriendRank[i].m_pStaticClass->SetShowEnable( false, false );
		}

		//성별 텍스쳐
		m_columnForOthersFriendRank[i].m_pStaticGender = 
			static_cast<CKTDGUIStatic* >( m_pDlgRankingInfo->GetControl( GET_REPLACED_STRING( ( STR_ID_3738, "Li", std::wstring( L"Static_Gender" ), i + 1 ) ) ) );
		if( NULL != m_columnForOthersFriendRank[i].m_pStaticGender )
		{
			if( NULL != m_columnForOthersFriendRank[i].m_pStaticGender->GetPicture( 0 ) )
				m_columnForOthersFriendRank[i].m_pStaticGender->GetPicture( 0 )->SetShow( true );

			m_columnForOthersFriendRank[i].m_pStaticGender->SetShowEnable( false, false );
		}

		//닉네임 스트링
		m_columnForOthersFriendRank[i].m_pStaticNickName =
			static_cast<CKTDGUIStatic* >( m_pDlgRankingInfo->GetControl( GET_REPLACED_STRING( ( STR_ID_3738, "Li", std::wstring( L"Id_F_" ), i + 1 ) ) ) );
		if( NULL != m_columnForOthersFriendRank[i].m_pStaticNickName )
		{
			if( NULL != m_columnForOthersFriendRank[i].m_pStaticNickName->GetString( 0 ) )
				m_columnForOthersFriendRank[i].m_pStaticNickName->GetString( 0 )->msg = L"";

			m_columnForOthersFriendRank[i].m_pStaticNickName->SetShowEnable( false, false );
		}

		//순위 텍스쳐
		m_columnForOthersFriendRank[i].m_pStaticRanking = 
			static_cast<CKTDGUIStatic* >( m_pDlgRankingInfo->GetControl( GET_REPLACED_STRING( ( STR_ID_3738, "Li", std::wstring( L"Static_RankF" ), i + 1 ) ) ) );
		if( NULL != m_columnForOthersFriendRank[i].m_pStaticRanking )
		{
			for( int j = 0; j < _CONST_UIRANKING_FRIEND_INFO_::g_iRankLimit; j++ )
				if( NULL != m_columnForOthersFriendRank[i].m_pStaticRanking->GetPicture( j ) )
					m_columnForOthersFriendRank[i].m_pStaticRanking->GetPicture( j )->SetShow( true );

			m_columnForOthersFriendRank[i].m_pStaticRanking->SetShowEnable( false, false );
		}

		//근성도 or 주간 누적 AP 수치 텍스쳐
		m_columnForOthersFriendRank[i].m_pStaticPoint =
			static_cast<CKTDGUIStatic* >( m_pDlgRankingInfo->GetControl( GET_REPLACED_STRING( ( STR_ID_3738, "Li", std::wstring( L"Static_Strong" ), i + 1 ) ) ) );
		if( NULL != m_columnForOthersFriendRank[i].m_pStaticPoint )
		{
			for( int j = 0; j < _CONST_UIRANKING_FRIEND_INFO_::g_iLimitStrongGage; j++ )
				if( NULL != m_columnForOthersFriendRank[i].m_pStaticPoint->GetPicture( j ) )
					m_columnForOthersFriendRank[i].m_pStaticPoint->GetPicture( j )->SetShow( true );

			m_columnForOthersFriendRank[i].m_pStaticPoint->SetShowEnable( false, false );
		}

		//투명 버튼
		m_columnForOthersFriendRank[i].m_pButtonPopUp =
			static_cast<CKTDGUIButton* >( m_pDlgRankingInfo->GetControl( GET_REPLACED_STRING( ( STR_ID_3738, "Li", std::wstring( L"User" ), i + 1 ) ) ) );
		if( NULL != m_columnForOthersFriendRank[i].m_pButtonPopUp )
		{
			m_columnForOthersFriendRank[i].m_pButtonPopUp->SetShowEnable( false, false );
		}
	}

}

void CX2UIRankingInfo::SetGender( CKTDGUIStatic* _static, GENDER_SELECT _select )
{
	if( NULL == _static )
		return;

	CKTDGUIControl::CPictureData* pPicture = _static->GetPicture( 0 );
	if( NULL == pPicture )
		return;

	switch( _select )
	{
	case GS_NONE:
		{
			pPicture->SetTex( L"DLG_UI_Button20_NEW.tga", L"Bg_Gender_Rock" );
			pPicture->SetShow( true );
		}
		break;
	case GS_MALE:
		{
			pPicture->SetTex( L"DLG_UI_Button20_NEW.tga", L"Bt_Male_Over" );
			pPicture->SetShow( true );
		}
		break;
	case GS_FEMALE:
		{
			pPicture->SetTex( L"DLG_UI_Button20_NEW.tga", L"Bt_FeMale_Over" );
			pPicture->SetShow( true );
		}
		break;
	default:
		pPicture->SetShow( false );
		break;
	}

	_static->SetShowEnable( true, true );
}

void CX2UIRankingInfo::SetPoint( CKTDGUIStatic* _static, int _point )
{
	if( NULL == _static )
		return;
	if( _point < 0 )
		_point = 0;
	if( _point > 999999 ) //최대 한계 수치 6자리까지
		_point = 999999;

	const wstring STRONG_KEY[] =
	{
		L"Will_0",L"Will_1",L"Will_2",L"Will_3",L"Will_4",
		L"Will_5",L"Will_6",L"Will_7",L"Will_8",L"Will_9"
	};

	int strong_100000	= _point / 100000;	_point = _point % 100000;
	int strong_10000	= _point / 10000;	_point = _point % 10000;
	int strong_1000		= _point / 1000;	_point = _point % 1000;
	int strong_100		= _point / 100;		_point = _point % 100;
	int strong_10		= _point / 10;	
	int strong_1		= _point % 10;

	int index = 0;
	bool Is_Upper_Zero = false;

	//100000의 자리
	CKTDGUIControl::CPictureData* pPicture = _static->GetPicture( index );
	if( NULL != pPicture )
	{
		if( 0 == strong_100000 )
		{
			Is_Upper_Zero = true;
			pPicture->SetShow( false );
		}
		else
		{
			pPicture->SetTex( L"DLG_UI_Common_Texture_Bg_01.tga", STRONG_KEY[strong_100000].c_str() );
			pPicture->SetShow( true );
		}
	}

	//10000의 자리
	index++;
	pPicture = _static->GetPicture( index );
	if( NULL != pPicture )
	{
		if( 0 == strong_10000 && true == Is_Upper_Zero )
		{
			pPicture->SetShow( false );
		}
		else
		{
			Is_Upper_Zero = false;
			pPicture->SetTex( L"DLG_UI_Common_Texture_Bg_01.tga", STRONG_KEY[strong_10000].c_str() );
			pPicture->SetShow( true );
		}
	}

	//1000의 자리
	index++;
	pPicture = _static->GetPicture( index );
	if( NULL != pPicture )
	{
		if( 0 == strong_1000 && true == Is_Upper_Zero )
		{
			pPicture->SetShow( false );
		}
		else
		{
			Is_Upper_Zero = false;
			pPicture->SetTex( L"DLG_UI_Common_Texture_Bg_01.tga", STRONG_KEY[strong_1000].c_str() );
			pPicture->SetShow( true );
		}
	}

	//100의 자리
	index++;
	pPicture = _static->GetPicture( index );
	if( NULL != pPicture )
	{
		if( 0 == strong_100 && true == Is_Upper_Zero )
		{
			pPicture->SetShow( false );
		}
		else
		{
			Is_Upper_Zero = false;
			pPicture->SetTex( L"DLG_UI_Common_Texture_Bg_01.tga", STRONG_KEY[strong_100].c_str() );
			pPicture->SetShow( true );
		}
	}

	//10의 자리
	index++;
	pPicture = _static->GetPicture( index );
	if( NULL != pPicture )
	{
		if( 0 == strong_10 && true == Is_Upper_Zero )
		{
			pPicture->SetShow( false );
		}
		else
		{
			Is_Upper_Zero = false;
			pPicture->SetTex( L"DLG_UI_Common_Texture_Bg_01.tga", STRONG_KEY[strong_10].c_str() );
			pPicture->SetShow( true );
		}
	}

	//1의 자리
	index++;
	pPicture = _static->GetPicture( index );
	if( NULL != pPicture )
	{
		pPicture->SetTex( L"DLG_UI_Common_Texture_Bg_01.tga", STRONG_KEY[strong_1].c_str() );
		pPicture->SetShow( true );
	}

	_static->SetShowEnable( true, true );
}

void CX2UIRankingInfo::SetRank( CKTDGUIStatic* _static, int _rank )
{
	if( NULL == _static )
		return;
	if( _rank < 0 || _rank > _CONST_UIRANKING_FRIEND_INFO_::g_iLimitRanking )
		return;

	const wstring RANK_KEY[] = 
	{
		L"Rank_0",L"Rank_1",L"Rank_2",L"Rank_3",L"Rank_4",
		L"Rank_5",L"Rank_6",L"Rank_7",L"Rank_8",L"Rank_9"
	};

	int rank_1000	= _rank / 1000;	_rank = _rank % 1000;
	int rank_100	= _rank / 100;	_rank = _rank % 100;
	int rank_10		= _rank / 10;
	int rank_1		= _rank % 10;

	int index = 0;
	bool Is_Upper_Zero = false;

	//1000의 자리
	CKTDGUIControl::CPictureData* pPicture = _static->GetPicture( index );
	if( NULL != pPicture )
	{
		if( 0 == rank_1000 )
		{
			Is_Upper_Zero = true;
			pPicture->SetShow( false );
		}
		else
		{
			pPicture->SetTex( L"DLG_UI_Common_Texture_Bg_01.tga", RANK_KEY[rank_1000].c_str() );
			pPicture->SetShow( true );
		}
	}

	//100의 자리
	index++;
	pPicture = _static->GetPicture( index );
	if( NULL != pPicture )
	{
		if( 0 == rank_100 && true == Is_Upper_Zero )
		{
			pPicture->SetShow( false );
		}
		else
		{
			Is_Upper_Zero = false;
			pPicture->SetTex( L"DLG_UI_Common_Texture_Bg_01.tga", RANK_KEY[rank_100].c_str() );
			pPicture->SetShow( true );
		}
	}

	//10의 자리
	index++;
	pPicture = _static->GetPicture( index );
	if( NULL != pPicture )
	{
		if( 0 == rank_10 && true == Is_Upper_Zero )
		{
			pPicture->SetShow( false );
		}
		else
		{
			Is_Upper_Zero = false;
			pPicture->SetTex( L"DLG_UI_Common_Texture_Bg_01.tga", RANK_KEY[rank_10].c_str() );
			pPicture->SetShow( true );
		}
	}

	//1의 자리
	index++;
	pPicture = _static->GetPicture( index );
	if( NULL != pPicture )
	{
		pPicture->SetTex( L"DLG_UI_Common_Texture_Bg_01.tga", RANK_KEY[rank_1].c_str() );
		pPicture->SetShow( true );
	}

	_static->SetShowEnable( true, true );
}

void CX2UIRankingInfo::SetShowInfoByType_F( ColumnForRankFriend* const pColumnForOthersRank, bool bShow )
{
	pColumnForOthersRank->m_pStaticClass->SetShowEnable( bShow, bShow );
	pColumnForOthersRank->m_pStaticGender->SetShowEnable( bShow, bShow );
	pColumnForOthersRank->m_pStaticNickName->SetShowEnable( bShow, bShow );
	pColumnForOthersRank->m_pStaticRanking->SetShowEnable( bShow, bShow );
	pColumnForOthersRank->m_pStaticPoint->SetShowEnable( bShow, bShow );
	pColumnForOthersRank->m_pButtonPopUp->SetShowEnable( bShow, bShow );
}


#endif //SERV_LOCAL_RANKING_SYSTEM


//{{ 허상형 : [2009/8/20] //	신학기 이벤트 컨트롤 초기화
#ifdef NEW_TERM_EVENT
void CX2UIRankingInfo::InitUIRankingEventControl( bool bShow )
{
	CKTDGUIRadioButton* pRadioEvent = NULL;
	pRadioEvent				= static_cast< CKTDGUIRadioButton* >( m_pDlgRankingInfo->GetControl( L"RadioButton_Event" ) );

	if( pRadioEvent != NULL )
	{
		pRadioEvent->SetShowEnable( bShow, bShow );
	}
}
#endif	//	NEW_TERM_EVENT
//}} 허상형 : [2009/8/20] //	신학기 이벤트 컨트롤 초기화

#endif	RANKING_INFO_UI
//{{ kimhc // 2009-07-09 // 헤니르의 시공에 추가되는 Ranking UI 작업

