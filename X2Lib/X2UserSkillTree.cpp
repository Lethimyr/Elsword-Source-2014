#include "StdAfx.h"
#include ".\x2userskilltree.h"


/** @function	CX2userSkillTree
	@brief		생성자, 장착 스킬 초기화, 길드클래스 초기화
	*/
CX2UserSkillTree::CX2UserSkillTree(void)
	: m_eSkillSlotBExpirationState( SSBES_EXPIRED )
	, m_wstrSkillSlotBEndDate( L"" )
	, m_iUnitClass( CX2Unit::UC_NONE )
#ifdef SKILL_PAGE_SYSTEM //JHKang
	, m_usUsingPage( 0 )
	, m_usOpenedPage( 0 )
	, m_usSelectPage( 0 )
#endif //SKILL_PAGE_SYSTEM
{
	BOOST_STATIC_ASSERT( ARRAY_SIZE( m_aEquippedSkill ) == ARRAY_SIZE( m_aEquippedSkillSlotB ) );
	for( int i=0; i<ARRAY_SIZE( m_aEquippedSkill ); ++i )
	{
		m_aEquippedSkill[i].Init();
		m_aEquippedSkillSlotB[i].Init();
	}
	
#ifdef GUILD_SKILL
	m_iGuildClass = 0;
#endif GUILD_SKILL

#ifdef SKILL_PAGE_SYSTEM //JHKang
	ZeroMemory( m_arrSkillPoint, sizeof(m_arrSkillPoint) );
	ZeroMemory( m_arrCashSkillPoint, sizeof(m_arrCashSkillPoint) );
#endif //SKILL_PAGE_SYSTEM

}

/** @function	~CX2UserSkillTree
	@brief		소멸자
*/
CX2UserSkillTree::~CX2UserSkillTree(void)
{
}

/** @function	OnFrameMove
	@brief		프레임 갱신 시, 스킬 쿨타임 처리
	@param[in]	fTime : 현재 시간
	@param[in]	fElapsedTime : 경과 시간
*/
void CX2UserSkillTree::OnFrameMove( double fTime, float fElapsedTime )
{
	CX2GageManager* pGageManager = CX2GageManager::GetInstance();


#ifdef NEXON_QA_CHEAT_REQ
	if( NULL != g_pData->GetMyUser() && g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_SPECIAL_USER && true == g_pMain->GetNoCoolTime() )
	{
		AllSkillNoCoolTime();
		g_pMain->SetNoCoolTime( false );
	}
	else
	{

#endif //NEXON_QA_CHEAT_REQ


	for( int i=0; i<ARRAY_SIZE( m_aEquippedSkill ); ++i )
	{
		SkillSlotData& skillData = m_aEquippedSkill[i];
		if( skillData.m_fCoolTimeLeft > 0.f )
		{
			skillData.m_fCoolTimeLeft -= fElapsedTime;
			if( skillData.m_fCoolTimeLeft < 0.f )
			{
				skillData.m_fCoolTimeLeft = 0.f;
			}

			pGageManager->SetMySkillCoolTimeList( i, skillData.m_fCoolTimeLeft );
		}
	}

	for( int i=0; i<ARRAY_SIZE( m_aEquippedSkillSlotB ); i++ )
	{
		SkillSlotData& skillData = m_aEquippedSkillSlotB[i];
		if( skillData.m_fCoolTimeLeft > 0.f )
		{
			skillData.m_fCoolTimeLeft -= fElapsedTime;
			if( skillData.m_fCoolTimeLeft < 0.f )
			{
				skillData.m_fCoolTimeLeft = 0.f;
			}

			pGageManager->SetMySkillCoolTimeList( i + EQUIPPED_SKILL_SLOT_COUNT, skillData.m_fCoolTimeLeft );
		}
	}	


#ifdef NEXON_QA_CHEAT_REQ
	}
#endif //NEXON_QA_CHEAT_REQ


	if( NULL != g_pData && NULL != g_pData->GetMyUser() && NULL != g_pData->GetMyUser()->GetSelectUnit() )
	{
		int iCurrentSP = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_iSPoint + g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_iCSPoint;	
		static int iOldSP = 0;

		if( iCurrentSP != iOldSP )
		{
			if( NULL != g_pData->GetPlayGuide() )
			{
				if( iCurrentSP > iOldSP )
				{	//SP가 이전보다 늘었을 때 SP 획득 가이드 출력
					g_pData->GetPlayGuide()->SetPlayGuide(CX2PlayGuide::PGT_OBTAIN_SKILL_POINT, true, iCurrentSP);
				}
				iOldSP = iCurrentSP;
			}

			if( iCurrentSP > 0 )
			{	//SP가 0초과 일 때 SP 보유 가이드 출력

				if( NULL != g_pMain && NULL != g_pMain->GetKeyPad() )
				{
					g_pData->GetPlayGuide()->SetPlayGuide(CX2PlayGuide::PGT_HAS_SKILL_POINT, true, g_pMain->GetKeyPad()->GetKeyString( GAMEACTION_SKILL_TREE ) );
				}
			}
			else
			{
				g_pData->GetPlayGuide()->SetPlayGuide(CX2PlayGuide::PGT_HAS_SKILL_POINT, false);
			}
		}
	}

#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
	/// 스킬 레벨 관련 인자 유효성 검사
	if ( g_pKTDXApp->GetIsNowVeryfy() == true
		&& g_pInstanceData != NULL 
		&& g_pInstanceData->GetVerifyGageManagerTimer() <= 0.f
#ifdef SKILL_PAGE_SYSTEM //JHKang
		&& true == IsEnableSkillAcquiredPage()
#endif //SKILL_PAGE_SYSTEM
		)
	{
#ifdef SKILL_PAGE_SYSTEM //JHKang
		SkillDataMap::const_iterator mit = m_vecSkillDataMap[m_usUsingPage].begin();

		for ( mit; mit != m_vecSkillDataMap[m_usUsingPage].end(); ++mit )
		{
			if( mit->second.m_iSkillLevel.Verify() == false ||
				mit->second.m_iSkillCSPoint.Verify() == false ||
				mit->second.m_iIncreaseSkillLevelByBuff.Verify() == false ||
				mit->second.m_iIncreaseSkillLevelBySocket.Verify() == false )
			{
				if( NULL != g_pData &&
					NULL != g_pData->GetMyUser() &&
					NULL != g_pData->GetServerProtocol() )
				{
					if( g_pData->GetMyUser()->GetUserData().hackingUserType != CX2User::HUT_AGREE_HACK_USER &&
						g_pKTDXApp->GetFindHacking() == false )
					{
						g_pData->GetServerProtocol()->SendID( EGS_REPORT_HACK_USER_NOT );
					}

				#ifdef ADD_COLLECT_CLIENT_INFO
					g_pMain->SendHackInfo1( ANTIHACKING_ID::ANTIHACKING_GAME_03, ANTI_HACK_STRING_AntiHacking_Skill_CoolTime_Verify, true, true );
					g_pInstanceData->SetRemainedTimeByForceQuitGame( REMAINED_TIME_BY_FORCE_QUIT_GAME );
				#endif // ADD_COLLECT_CLIENT_INFO
					return;
				}
			}
		}
#else //SKILL_PAGE_SYSTEM
		SkillDataMap::const_iterator mit = m_mapSkillAcquired.begin();

		for ( mit; mit != m_mapSkillAcquired.end(); ++mit )
		{
			if( mit->second.m_iSkillLevel.Verify() == false ||
				mit->second.m_iSkillCSPoint.Verify() == false ||
				mit->second.m_iIncreaseSkillLevelByBuff.Verify() == false ||
				mit->second.m_iIncreaseSkillLevelBySocket.Verify() == false )
			{
				if( NULL != g_pData &&
					NULL != g_pData->GetMyUser() &&
					NULL != g_pData->GetServerProtocol() )
				{
					if( g_pData->GetMyUser()->GetUserData().hackingUserType != CX2User::HUT_AGREE_HACK_USER &&
						g_pKTDXApp->GetFindHacking() == false )
					{
						g_pData->GetServerProtocol()->SendID( EGS_REPORT_HACK_USER_NOT );
					}

				#ifdef ADD_COLLECT_CLIENT_INFO
					g_pMain->SendHackInfo1( ANTIHACKING_ID::ANTIHACKING_GAME_03, ANTI_HACK_STRING_AntiHacking_Skill_CoolTime_Verify, true, true );
					g_pInstanceData->SetRemainedTimeByForceQuitGame( REMAINED_TIME_BY_FORCE_QUIT_GAME );
				#endif // ADD_COLLECT_CLIENT_INFO
					return;
				}
			}
		}
#endif //SKILL_PAGE_SYSTEM

		SkillDataMap::const_iterator mitGuild = m_mapGuildSkillAcquired.begin();

		for ( mitGuild; mitGuild != m_mapGuildSkillAcquired.end(); ++mitGuild )
		{
			if( mitGuild->second.m_iSkillLevel.Verify() == false ||
				mitGuild->second.m_iSkillCSPoint.Verify() == false ||
				mitGuild->second.m_iIncreaseSkillLevelByBuff.Verify() == false ||
				mitGuild->second.m_iIncreaseSkillLevelBySocket.Verify() == false )
			{
				if( NULL != g_pData &&
					NULL != g_pData->GetMyUser() &&
					NULL != g_pData->GetServerProtocol() )
				{
					if( g_pData->GetMyUser()->GetUserData().hackingUserType != CX2User::HUT_AGREE_HACK_USER &&
						g_pKTDXApp->GetFindHacking() == false )
					{
						g_pData->GetServerProtocol()->SendID( EGS_REPORT_HACK_USER_NOT );
					}

#ifdef ADD_COLLECT_CLIENT_INFO
					g_pMain->SendHackInfo1( ANTIHACKING_ID::ANTIHACKING_GAME_03, ANTI_HACK_STRING_AntiHacking_Skill_CoolTime_Verify, true, true );
					g_pInstanceData->SetRemainedTimeByForceQuitGame( REMAINED_TIME_BY_FORCE_QUIT_GAME );
#endif // ADD_COLLECT_CLIENT_INFO
					return;
				}
			}
		}
	}
#endif // UPGRADE_SKILL_SYSTEM_2013
}

/** @function	Reset
	@brief		재설정, 장착 스킬(A, B), 스킬 트리, 봉인 스킬 초기화
	@param[in]	bResetSkillTree : 스킬 트리
	@param[in]	bResetEquippedSkill : 장착 스킬
	@param[in]	bResetUnsealedSkill : 봉인 해제된 스킬
	@param[in]	bResetSkillSlotB : B 슬롯
*/
void CX2UserSkillTree::Reset( bool bResetSkillTree, bool bResetEquippedSkill, bool bResetUnsealedSkill, bool bResetSkillSlotB )
{
	if( true == bResetEquippedSkill )
	{
		for( int i=0; i<ARRAY_SIZE( m_aEquippedSkill ); ++i )
		{
			m_aEquippedSkill[i].Init();
			m_aEquippedSkillSlotB[i].Init();
		}
	}

	if( true == bResetSkillTree )
	{
	#ifdef SKILL_PAGE_SYSTEM //JHKang
		if ( m_vecSkillDataMap.size() > 0 )
			m_vecSkillDataMap[m_usUsingPage].clear();
	#else //SKILL_PAGE_SYSTEM
		m_mapSkillAcquired.clear();
	#endif //SKILL_PAGE_SYSTEM
	}

	if( true == bResetUnsealedSkill )
	{
		m_setUnsealedSkillID.clear();
	}

	if( bResetSkillSlotB )
	{
		SetSkillSlotBExpirationState( SSBES_EXPIRED );
	}
}

/** @function	Reset
	@brief		이전 함수 길드 스킬 초기화 추가
	@param[in]	bResetSkillTree : 스킬 트리
	@param[in]	bResetGuildSkillTree : 길드 스킬 트리
	@param[in]	bResetEquippedSkill : 장착 스킬
	@param[in]	bResetUnsealedSkill : 봉인 해제된 스킬
	@param[in]	bResetSkillSlotB : B 슬롯
*/
#ifdef GUILD_SKILL
//{{ oasis907 : 김상윤 //// 2009-11-12 //// 
void CX2UserSkillTree::Reset( bool bResetSkillTree, bool bResetGuildSkillTree, bool bResetEquippedSkill, bool bResetUnsealedSkill, bool bResetSkillSlotB )
{
	if( true == bResetEquippedSkill )
	{
		for( int i=0; i<ARRAY_SIZE( m_aEquippedSkill ); ++i )
		{
			m_aEquippedSkill[i].Init();
			m_aEquippedSkillSlotB[i].Init();
		}
	}

	if( true == bResetSkillTree )
	{
	#ifdef SKILL_PAGE_SYSTEM //JHKang
		BOOST_FOREACH( SkillDataMap& userSkillPageData, m_vecSkillDataMap )
		{
			userSkillPageData.clear();
		}

		m_vecSkillDataMap.clear();
	#else //SKILL_PAGE_SYSTEM
		m_mapSkillAcquired.clear();
	#endif //SKILL_PAGE_SYSTEM
	}

	if( true == bResetGuildSkillTree )
	{
		m_mapGuildSkillAcquired.clear();
	}

	if( true == bResetUnsealedSkill )
	{
		m_setUnsealedSkillID.clear();
	}

	if( bResetSkillSlotB )
	{
		SetSkillSlotBExpirationState( SSBES_EXPIRED );
	}
}
//}} oasis907 : 김상윤 //// 2009-11-12 //// 
#endif GUILD_SKILL

/** @function	ConstructSkillTreeNotMyUnit
	@brief		대전, 던전 게임에서 내 캐릭터가 아닌 다른 캐릭터가 어떤 passive skill을 배웠는지 설정할 때 사용되는 함수
	@param[in]	vecSkillData : 스킬 정보
	@param[out]	aEquippedSkill[] : A Slot
	@param[out]	aEquippedSkillSlotB[] : B Slot
*/
void CX2UserSkillTree::ConstructSkillTreeNotMyUnit( const std::vector<KSkillData>& vecSkillData, const KSkillData aEquippedSkill[], const KSkillData aEquippedSkillSlotB[] )
{
	// 오현빈 // 
	// 추가 된 m_iIncreaseSkillLevelBySocket값을 초기화 시키지 않기 위해 false로 변경
#ifdef SKILL_PAGE_SYSTEM //JHKang
	BOOST_FOREACH( SkillDataMap& userSkillPageData, m_vecSkillDataMap )
	{
		userSkillPageData.clear();
	}
	m_vecSkillDataMap.clear();

	SkillDataMap EmptySkillData;
	m_vecSkillDataMap.push_back( EmptySkillData );
#endif //SKILL_PAGE_SYSTEM

	Reset( false, false, false, false );

	for( int i=0; i<EQUIPPED_SKILL_SLOT_COUNT; i++ )
	{
		SetSkillLevelAndCSP( (CX2SkillTree::SKILL_ID) aEquippedSkill[i].m_iSkillID, (int) aEquippedSkill[i].m_cSkillLevel, 0 );
	}

	for( int i=0; i<EQUIPPED_SKILL_SLOT_COUNT; i++ )
	{
		SetSkillLevelAndCSP( (CX2SkillTree::SKILL_ID) aEquippedSkillSlotB[i].m_iSkillID, (int) aEquippedSkillSlotB[i].m_cSkillLevel, 0 );
	}

	BOOST_TEST_FOREACH( const KSkillData&, kSkillData, vecSkillData )
	{
		SetSkillLevelAndCSP( (CX2SkillTree::SKILL_ID) kSkillData.m_iSkillID, (int) kSkillData.m_cSkillLevel, 0 );
	}
}

#ifdef GUILD_SKILL
//{{ oasis907 : 김상윤 //// 2009-11-18 //// 
/** @function	ConstructGuildSkillTreeNotMyUnit
	@brief		서버로부터 받은 길드 스킬도 지정하는 함수, 나중에 위의 함수를 대체 X
				길드 스킬만 업데이트하는 함수 (서버 통신 때문에)
				대전, 던전게임에서 내 캐릭터가 아닌 다른 캐릭터가 어떤 passive skill을 배웠는지 설정할 때 사용되는 함수
	@param[in]	vecGuildSkillData : 길드 스킬 정보
*/
void CX2UserSkillTree::ConstructGuildSkillTreeNotMyUnit( const std::vector<KSkillData>& vecGuildSkillData )
{
	Reset( false, true, false, false, false );

	// 길드 스킬
	BOOST_TEST_FOREACH( const KSkillData&, kSkillData, vecGuildSkillData )
	{
		SetSkillLevelAndCSP( (CX2SkillTree::SKILL_ID) kSkillData.m_iSkillID, (int) kSkillData.m_cSkillLevel, 0 );
	}
}
//}} oasis907 : 김상윤 //// 2009-11-18 //// 
#endif GUILD_SKILL

void CX2UserSkillTree::SetEquippedSkill( const int iSlotIndex_, const bool bSlotB_, const KSkillData& equippedSkillData_ )
{
	if ( bSlotB_ )
	{
		if ( iSlotIndex_ >= 0 && iSlotIndex_ < ARRAY_SIZE( m_aEquippedSkillSlotB ) )
		{
			m_aEquippedSkillSlotB[iSlotIndex_].Init();
			m_aEquippedSkillSlotB[iSlotIndex_].m_eID			= static_cast<CX2SkillTree::SKILL_ID>( equippedSkillData_.m_iSkillID );
			m_aEquippedSkillSlotB[iSlotIndex_].m_iSkillLevel	= static_cast<int>( equippedSkillData_.m_cSkillLevel );
		}
	}
	else
	{
		if ( iSlotIndex_ >= 0 && iSlotIndex_ < ARRAY_SIZE( m_aEquippedSkill ) )
		{
			m_aEquippedSkill[iSlotIndex_].Init();
			m_aEquippedSkill[iSlotIndex_].m_eID			= static_cast<CX2SkillTree::SKILL_ID>( equippedSkillData_.m_iSkillID );
			m_aEquippedSkill[iSlotIndex_].m_iSkillLevel	= static_cast<int>( equippedSkillData_.m_cSkillLevel );
		}
	}
}

/** @function	SetEquippedSkill
	@brief		장착 스킬 설정
	@param[out]	aEquippedSkill[] : A Slot
	@param[out]	aEquippedSkillSlotB[] : B Slot
*/
void CX2UserSkillTree::SetEquippedSkill( const KSkillData aEquippedSkill[], const KSkillData aEquippedSkillSlotB[] )
{
	/// 초기화
	for( int i=0; i<ARRAY_SIZE( m_aEquippedSkill ); ++i )
	{
		m_aEquippedSkill[i].Init();
		m_aEquippedSkillSlotB[i].Init();
	}

	
	/// A 슬롯(ID, 레벨)
	for( int i=0; i<ARRAY_SIZE( m_aEquippedSkill ); ++i )
	{
		m_aEquippedSkill[i].m_eID			= (CX2SkillTree::SKILL_ID) aEquippedSkill[i].m_iSkillID;
		m_aEquippedSkill[i].m_iSkillLevel	= (int) aEquippedSkill[i].m_cSkillLevel;
	}

	/// B 슬롯(ID, 레벨)
	for( int i=0; i<ARRAY_SIZE( m_aEquippedSkillSlotB ); i++ )
	{
		m_aEquippedSkillSlotB[i].m_eID			= (CX2SkillTree::SKILL_ID) aEquippedSkillSlotB[i].m_iSkillID;
		m_aEquippedSkillSlotB[i].m_iSkillLevel	= (int) aEquippedSkillSlotB[i].m_cSkillLevel;
	}
}

void CX2UserSkillTree::SetEquippedSkillLevelPlusBySlotIndex( const int iEquippedSkillLevelPlus_, const int iSlotIndex_, const bool bIsSlotB_ )
{
	if ( bIsSlotB_ )
	{
		if ( iSlotIndex_ >= 0 && iSlotIndex_ < ARRAY_SIZE( m_aEquippedSkillSlotB ) )
			m_aEquippedSkillSlotB[iSlotIndex_].m_iSkillLevelPlus = iEquippedSkillLevelPlus_;
	}
	else
	{
		if ( iSlotIndex_ >= 0 && iSlotIndex_ < ARRAY_SIZE( m_aEquippedSkill ) )
			m_aEquippedSkill[iSlotIndex_].m_iSkillLevelPlus = iEquippedSkillLevelPlus_;
	}
}

/** @function	SetEquippedSkillLevelPlus
	@brief		장착 스킬 레벨 증가(아이템에 의한)
	@param[in]	aEquippedSkillLevelPlus[] : A Slot 스킬 레벨 증가 값
	@param[in]	aEquippedSkillLevelPlusSlotB[] : B Slot 스킬 레벨 증가 값
*/
void CX2UserSkillTree::SetEquippedSkillLevelPlus( const int aEquippedSkillLevelPlus[], const int aEquippedSkillLevelPlusSlotB[] )
{
	for( int i=0; i<ARRAY_SIZE( m_aEquippedSkill ); ++i )
	{
		m_aEquippedSkill[i].m_iSkillLevelPlus	= aEquippedSkillLevelPlus[i];
	}

	for( int i=0; i<ARRAY_SIZE( m_aEquippedSkillSlotB ); ++i )
	{
		m_aEquippedSkillSlotB[i].m_iSkillLevelPlus	= aEquippedSkillLevelPlusSlotB[i];
	}
}


/** @function	SetAcquiredSkill
	@brief		취득한 스킬 설정
	@param[in]	스킬 리스트
*/
#ifdef SKILL_PAGE_SYSTEM //JHKang
void CX2UserSkillTree::SetAcquiredSkill( const std::vector<KUserSkillPageData>& vecSkillList )
{
	BOOST_FOREACH( SkillDataMap& userSkillPageData, m_vecSkillDataMap )
	{
		userSkillPageData.clear();
	}
	m_vecSkillDataMap.clear();

	for ( UINT i = 0; i < m_usOpenedPage; ++i )
	{
		SkillDataMap EmptySkillData;
		m_vecSkillDataMap.push_back( EmptySkillData );
	}

	int iPage = 0;

	BOOST_FOREACH( const KUserSkillPageData& userSkillPageData, vecSkillList )
	{
		BOOST_FOREACH( const KUserSkillData& userSkillData, userSkillPageData.m_vecUserSkillData )
		{
			if ( true == m_vecSkillDataMap.empty() || iPage >= m_vecSkillDataMap.size() )
				continue;

			SkillDataMap::iterator mit;
			mit = m_vecSkillDataMap[iPage].find( (CX2SkillTree::SKILL_ID) userSkillData.m_iSkillID );
			ASSERT( mit == m_vecSkillDataMap[iPage].end() );
			if( mit == m_vecSkillDataMap[iPage].end() )
			{
				// 이미 존재하면 삽입되지 않고 존재하지 않으면 삽입된다.
				m_vecSkillDataMap[iPage][ (CX2SkillTree::SKILL_ID)userSkillData.m_iSkillID ] = UserSkillData( (int)userSkillData.m_cSkillLevel, (int)userSkillData.m_cSkillCSPoint );
			}
		}
		m_arrSkillPoint[iPage] = userSkillPageData.m_usSkillPoint;
		m_arrCashSkillPoint[iPage] = userSkillPageData.m_usCashSkillPoint;
		++iPage;
	}
}

void CX2UserSkillTree::AddSkillPage()
{
	SkillDataMap EmptySkillData;
	m_vecSkillDataMap.push_back( EmptySkillData );
}
#else //SKILL_PAGE_SYSTEM
void CX2UserSkillTree::SetAcquiredSkill( const std::vector<KUserSkillData>& vecSkillList )
{
	m_mapSkillAcquired.clear();
	BOOST_TEST_FOREACH( const KUserSkillData&, userSkillData, vecSkillList )
	{
		SkillDataMap::iterator mit;
		mit = m_mapSkillAcquired.find( (CX2SkillTree::SKILL_ID) userSkillData.m_iSkillID );
		ASSERT( mit == m_mapSkillAcquired.end() );
		if( mit == m_mapSkillAcquired.end() )
		{
			// 이미 존재하면 삽입되지 않고 존재하지 않으면 삽입된다.
			m_mapSkillAcquired[ (CX2SkillTree::SKILL_ID)userSkillData.m_iSkillID ] = UserSkillData( (int)userSkillData.m_cSkillLevel, (int)userSkillData.m_cSkillCSPoint );
		}
	}
}
#endif //SKILL_PAGE_SYSTEM

#ifdef GUILD_SKILL
//{{ oasis907 : 김상윤 //// 2009-11-12 //// 
/** @function	SetAcquiredGuildSkill
	@brief		취득한 길드 스킬 설정
	@param[in]	길드 스킬 리스트
*/
void CX2UserSkillTree::SetAcquiredGuildSkill( const std::vector<KGuildSkillData>& vecSkillList )
{
	m_mapGuildSkillAcquired.clear();
	BOOST_TEST_FOREACH( const KGuildSkillData&, userSkillData, vecSkillList )
	{
		SkillDataMap::iterator mit;
		mit = m_mapGuildSkillAcquired.find( (CX2SkillTree::SKILL_ID) userSkillData.m_iSkillID );
		ASSERT( mit == m_mapGuildSkillAcquired.end() );
		if( mit == m_mapGuildSkillAcquired.end() )
		{
			// 이미 존재하면 삽입되지 않고 존재하지 않으면 삽입된다.
			m_mapGuildSkillAcquired[ (CX2SkillTree::SKILL_ID)userSkillData.m_iSkillID ] = UserSkillData( (int)userSkillData.m_cSkillLevel, (int)userSkillData.m_cSkillCSPoint );
		}
	}
}
//}} oasis907 : 김상윤 //// 2009-11-12 //// 
#endif GUILD_SKILL
/** @function	SetUnsealedSkill
	@brief		봉인 해제된 스킬 설정
	@param[in]	봉인 해제된 스킬 리스트
*/
void CX2UserSkillTree::SetUnsealedSkill( const std::vector<short>& vecUnsealedSkillList )
{
	//  봉인해제된 스킬 목록
	m_setUnsealedSkillID.clear();
	for( UINT i=0; i<vecUnsealedSkillList.size(); i++ )
	{
		m_setUnsealedSkillID.insert( (CX2SkillTree::SKILL_ID) vecUnsealedSkillList[i] );
	}
}

/** @function	GetSkillLevel
	@brief		스킬 레벨 얻기
	@param[in]	스킬 ID, 추가된 스킬 효과( 캐시 포인트, 소켓, 버프 ) 적용 여부
	@return		스킬 레벨
*/
#ifdef SKILL_PAGE_SYSTEM //JHKang
int CX2UserSkillTree::GetSkillLevel( CX2SkillTree::SKILL_ID eSkillID, bool bBaslcLevel /*= false*/, USHORT usPageNumber_ /*= 0*/ ) const
#else //SKILL_PAGE_SYSTEM
int CX2UserSkillTree::GetSkillLevel( CX2SkillTree::SKILL_ID eSkillID, bool bBaslcLevel /*= false*/ ) const
#endif //SKILL_PAGE_SYSTEM
{
	KTDXPROFILE();

#ifdef GUILD_SKILL
	//{{ oasis907 : 김상윤 //// 2009-11-16 //// 
	if(IsGuildSkill(eSkillID))
	{
		SkillDataMap::const_iterator mit = m_mapGuildSkillAcquired.find( eSkillID );
		if( mit != m_mapGuildSkillAcquired.end() )
		{
			const UserSkillData& userSkillData = mit->second;

		#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
			const int iSkillLevel = true == bBaslcLevel ? userSkillData.m_iSkillLevel : userSkillData.GetTotalSkillLevel();

			return min( iSkillLevel, MAX_LIMITED_SKILL_LEVEL );
		#else // UPGRADE_SKILL_SYSTEM_2013
			return userSkillData.m_iSkillLevel;
		#endif // UPGRADE_SKILL_SYSTEM_2013
		}
		return 0;
	}
	//}} oasis907 : 김상윤 //// 2009-11-16 //// 
#endif GUILD_SKILL

	#ifdef SKILL_PAGE_SYSTEM //JHKang
	if( m_vecSkillDataMap.empty() == true )
		return 0;

	USHORT usSkillPage = m_usUsingPage;

	if ( usPageNumber_ > 0 )
		usSkillPage = usPageNumber_ - 1;

	if( m_vecSkillDataMap.size() <= usSkillPage )
		usSkillPage = max( 0, m_vecSkillDataMap.size() - 1 );
	
	SkillDataMap::const_iterator mit = m_vecSkillDataMap[usSkillPage].find( eSkillID );

	if( mit != m_vecSkillDataMap[usSkillPage].end() )
	{
		const UserSkillData& userSkillData = mit->second;
		const int iSkillLevel = true == bBaslcLevel ? userSkillData.m_iSkillLevel : userSkillData.GetTotalSkillLevel();

		return min( iSkillLevel, MAX_LIMITED_SKILL_LEVEL );
	#else //SKILL_PAGE_SYSTEM
	SkillDataMap::const_iterator mit = m_mapSkillAcquired.find( eSkillID );
	if( mit != m_mapSkillAcquired.end() )
	{
		const UserSkillData& userSkillData = mit->second;

	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
		const int iSkillLevel = true == bBaslcLevel ? userSkillData.m_iSkillLevel : userSkillData.GetTotalSkillLevel();

		return min( iSkillLevel, MAX_LIMITED_SKILL_LEVEL );
	#else // UPGRADE_SKILL_SYSTEM_2013
		return userSkillData.m_iSkillLevel;
	#endif // UPGRADE_SKILL_SYSTEM_2013
	#endif //SKILL_PAGE_SYSTEM
	}
	return 0;
}

//{{ oasis907 : 김상윤 //// 2009-11-18 ////
/** @function	GetUserSkillTemplet
	@brief		유저 스킬 템플릿 얻기
	@param[in]	스킬 ID
	@return		스킬 템플릿
*/
const CX2SkillTree::SkillTemplet* CX2UserSkillTree::GetUserSkillTemplet( CX2SkillTree::SKILL_ID eSkillID) const
{
	KTDXPROFILE();

	//// passive skill 에만 사용해야 한다. active skill은 getequippedactiveskill()함수를 써야함
	//ASSERT( NULL == g_pData->GetSkillTree()->GetSkillTemplet( eSkillID, 1 ) ||
	//	CX2SkillTree::ST_PASSIVE_PHYSIC_ATTACK == g_pData->GetSkillTree()->GetSkillTemplet( eSkillID, 1 )->m_eType ||
	//	CX2SkillTree::ST_PASSIVE_MAGIC_ATTACK == g_pData->GetSkillTree()->GetSkillTemplet( eSkillID, 1 )->m_eType ||
	//	CX2SkillTree::ST_PASSIVE_MAGIC_DEFENCE == g_pData->GetSkillTree()->GetSkillTemplet( eSkillID, 1 )->m_eType ||
	//	CX2SkillTree::ST_PASSIVE_PHYSIC_DEFENCE == g_pData->GetSkillTree()->GetSkillTemplet( eSkillID, 1 )->m_eType ||
	//	CX2SkillTree::ST_PASSIVE == g_pData->GetSkillTree()->GetSkillTemplet( eSkillID, 1 )->m_eType );

#ifdef SKILL_PAGE_SYSTEM //JHKang

	if ( true == m_vecSkillDataMap.empty() || m_usUsingPage >= m_vecSkillDataMap.size() )
		return NULL;

	SkillDataMap::const_iterator mit = m_vecSkillDataMap[m_usUsingPage].find( eSkillID );

	if( mit == m_vecSkillDataMap[m_usUsingPage].end() )
	{
		return NULL;
	}
#else //SKILL_PAGE_SYSTEM
	SkillDataMap::const_iterator mit = m_mapSkillAcquired.find( eSkillID );

	if( mit == m_mapSkillAcquired.end() )
	{
		return NULL;
	}
#endif //SKILL_PAGE_SYSTEM

	const UserSkillData& userSkillData = mit->second;
	/////////////////////////////////////////////
#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
	return g_pData->GetSkillTree()->GetSkillTemplet(eSkillID);
#else // UPGRADE_SKILL_SYSTEM_2013
	return g_pData->GetSkillTree()->GetSkillTemplet(eSkillID, userSkillData.m_iSkillLevel);
#endif // UPGRADE_SKILL_SYSTEM_2013
}
//}} oasis907 : 김상윤 //// 2009-11-18 ////

#ifdef GUILD_SKILL
//{{ oasis907 : 김상윤 //// 2009-11-18 ////
/** @function	GetSkillLevel
	@brief		길드 스킬 레벨 얻기
	@param[out]	int 길드 스킬 레벨
*/
int CX2UserSkillTree::GetSkillLevel( CX2SkillTree::SKILL_ID eSkillID, int iGuildMemberGrade) const
{
	KTDXPROFILE();
	if(iGuildMemberGrade == CX2GuildManager::GUG_INVALID ||
		iGuildMemberGrade >= g_pData->GetGuildManager()->GetLimitGuildGrade()) // 지정될 커트라인 길드 멤버 등급
	{
		return 0;      // 등급이 낮아 길드 스킬 적용 안될때
	}

	SkillDataMap::const_iterator mit = m_mapGuildSkillAcquired.find( eSkillID );
	if( mit != m_mapGuildSkillAcquired.end() )
	{
		const UserSkillData& userSkillData = mit->second;
		return userSkillData.m_iSkillLevel;
	}

	return 0;
}
//}} oasis907 : 김상윤 //// 2009-11-18 ////

//{{ oasis907 : 김상윤 //// 2009-11-18 ////
/** @function	GetUserSkillTemplet
	@brief		길드 유저 스킬 템플릿 얻기
	@param[in]	스킬 ID
	@param[in]	길드 멤버 등급
	@return		스킬 템플릿
*/
const CX2SkillTree::SkillTemplet* CX2UserSkillTree::GetUserSkillTemplet( CX2SkillTree::SKILL_ID eSkillID, int iGuildMemberGrade) const
{
	KTDXPROFILE();

	if(iGuildMemberGrade == CX2GuildManager::GUG_INVALID ||
		iGuildMemberGrade >= g_pData->GetGuildManager()->GetLimitGuildGrade())  // 지정될 커트라인 길드 멤버 등급
	{
		return NULL;	// 등급이 낮아 길드 스킬 적용 안될때
	}

	SkillDataMap::const_iterator mit = m_mapGuildSkillAcquired.find( eSkillID );

	if( mit == m_mapGuildSkillAcquired.end() )
	{
		return NULL;   
	}

	const UserSkillData& userSkillData = mit->second;
	/////////////////////////////////////////////

#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
	return g_pData->GetSkillTree()->GetSkillTemplet(eSkillID );
#else // UPGRADE_SKILL_SYSTEM_2013
	return g_pData->GetSkillTree()->GetSkillTemplet(eSkillID, userSkillData.m_iSkillLevel);
#endif // UPGRADE_SKILL_SYSTEM_2013
}
//}} oasis907 : 김상윤 //// 2009-11-18 //// 
#endif GUILD_SKILL

/** @function	GetSkillLevelAndCSP
	@brief		스킬 레벨, 스펠 포인트 얻기
	@param[in]	스킬 ID
	@param[out]	스킬 레벨
	@param[out]	스킬 Spell Point
*/
bool CX2UserSkillTree::GetSkillLevelAndCSP( IN CX2SkillTree::SKILL_ID eSkillID, OUT int& iSkillLevel, OUT int& iSkillCSPoint ) const
{
#ifdef GUILD_SKILL
	//{{ oasis907 : 김상윤 //// 2009-11-16 //// 
	if(IsGuildSkill(eSkillID))
	{
		SkillDataMap::const_iterator mit = m_mapGuildSkillAcquired.find( eSkillID );
		if( mit != m_mapGuildSkillAcquired.end() )
		{
			const UserSkillData& userSkillData = mit->second;
			iSkillLevel		= userSkillData.m_iSkillLevel;
			iSkillCSPoint	= userSkillData.m_iSkillCSPoint;
			return true;
		}
		return false;
	}
	//}} oasis907 : 김상윤 //// 2009-11-16 //// 
#endif GUILD_SKILL

#ifdef SKILL_PAGE_SYSTEM //JHKang
	if ( true == m_vecSkillDataMap.empty() || m_usUsingPage >= m_vecSkillDataMap.size() )
		return false;

	SkillDataMap::const_iterator mit = m_vecSkillDataMap[m_usUsingPage].find( eSkillID );

	if( mit == m_vecSkillDataMap[m_usUsingPage].end() )
#else //SKILL_PAGE_SYSTEM
	SkillDataMap::const_iterator mit = m_mapSkillAcquired.find( eSkillID );
	if( mit != m_mapSkillAcquired.end() )
#endif //SKILL_PAGE_SYSTEM
	{
		const UserSkillData& userSkillData = mit->second;
		iSkillLevel		= userSkillData.m_iSkillLevel;
		iSkillCSPoint	= userSkillData.m_iSkillCSPoint;
		return true;
	}

	return false;
}


/** @function	SetSkillLevelAndCSP
	@brief		데이터가 있으면 값을 변경하고, 없으면 추가한다, skill level이 0보다 작으면 삭제
	@param[in]	스킬 ID
	@param[in]	스킬 레벨
	@param[in]	스킬 Spell Point
	@return		false : 변경 실패
	@return		true : 변경 완료
*/
#ifdef SKILL_PAGE_SYSTEM //JHKang
bool CX2UserSkillTree::SetSkillLevelAndCSP( CX2SkillTree::SKILL_ID eSkillID, int iSkillLevel, int iSkillCSPoint, USHORT usSkillPage_ )
#else //SKILL_PAGE_SYSTEM
bool CX2UserSkillTree::SetSkillLevelAndCSP( CX2SkillTree::SKILL_ID eSkillID, int iSkillLevel, int iSkillCSPoint )
#endif //SKILL_PAGE_SYSTEM
{
#ifdef GUILD_SKILL
	//{{ oasis907 : 김상윤 //// 2009-11-16 //// 
	if(IsGuildSkill(eSkillID))
	{
#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( eSkillID );
#else // UPGRADE_SKILL_SYSTEM_2013
		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( eSkillID, 1 );
#endif // UPGRADE_SKILL_SYSTEM_2013
		if( NULL == pSkillTemplet )
			return false;

		if( iSkillLevel > g_pData->GetSkillTree()->GetMaxGuildSkillLevel( (int)m_iGuildClass, eSkillID ) )
			return false;

#ifndef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
		if( iSkillLevel > 0 &&
			iSkillLevel < iSkillCSPoint )
			return false;
#endif // UPGRADE_SKILL_SYSTEM_2013


		SkillDataMap::iterator mit;
		mit = m_mapGuildSkillAcquired.find( eSkillID );
		if( mit != m_mapGuildSkillAcquired.end() )
		{
			if( iSkillLevel > 0 )
			{
				UserSkillData& userSkillData = mit->second;
				userSkillData.m_iSkillLevel		= iSkillLevel;
				userSkillData.m_iSkillCSPoint	= iSkillCSPoint;
			}
			else
			{
				m_mapGuildSkillAcquired.erase( mit );
			}
		}
		else 
		{
			if( iSkillLevel > 0 )
			{
				m_mapGuildSkillAcquired[ eSkillID ] = UserSkillData( iSkillLevel, iSkillCSPoint );
			}
		}
		return true;

	}
	//}} oasis907 : 김상윤 //// 2009-11-16 //// 
#endif GUILD_SKILL

#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
	const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( eSkillID );
#else // UPGRADE_SKILL_SYSTEM_2013
	const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( eSkillID, 1 );
#endif // UPGRADE_SKILL_SYSTEM_2013
	if( NULL == pSkillTemplet )
		return false;

	if( iSkillLevel > g_pData->GetSkillTree()->GetMaxSkillLevel( (int)m_iUnitClass, eSkillID ) )
		return false;

#ifndef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
	if( iSkillLevel > 0 &&
		iSkillLevel < iSkillCSPoint )
		return false;
#endif // UPGRADE_SKILL_SYSTEM_2013


	SkillDataMap::iterator mit;

#ifdef SKILL_PAGE_SYSTEM //JHKang
	if ( m_vecSkillDataMap.size() <= usSkillPage_ )
		return false;

	USHORT usPageNumber = m_usUsingPage;

	if ( usSkillPage_ > 0 )
		usPageNumber = usSkillPage_;

	mit = m_vecSkillDataMap[usPageNumber].find( eSkillID );
	if( mit != m_vecSkillDataMap[usPageNumber].end() )
	{
		if( iSkillLevel > 0 )
		{
			UserSkillData& userSkillData = mit->second;
			userSkillData.m_iSkillLevel		= iSkillLevel;
			userSkillData.m_iSkillCSPoint	= iSkillCSPoint;
		}
		else
		{
			m_vecSkillDataMap[usPageNumber].erase( mit );
		}
	}
	else 
	{
		if( iSkillLevel > 0 )
		{
			m_vecSkillDataMap[usPageNumber][ eSkillID ] = UserSkillData( iSkillLevel, iSkillCSPoint );
		}
	}
#else //SKILL_PAGE_SYSTEM
	mit = m_mapSkillAcquired.find( eSkillID );
	if( mit != m_mapSkillAcquired.end() )
	{
		if( iSkillLevel > 0 )
		{
			UserSkillData& userSkillData = mit->second;
			userSkillData.m_iSkillLevel		= iSkillLevel;
			userSkillData.m_iSkillCSPoint	= iSkillCSPoint;
		}
		else
		{
			m_mapSkillAcquired.erase( mit );
		}
	}
	else 
	{
		if( iSkillLevel > 0 )
		{
			m_mapSkillAcquired[ eSkillID ] = UserSkillData( iSkillLevel, iSkillCSPoint );
		}
	}
#endif //SKILL_PAGE_SYSTEM
	return true;
}

/** @function	IsMaxSkillLevel
	@brief		스킬 레벨 확인(최고치)
	@param[in]	스킬 ID
	@return		최고치 인지 아닌지 여부
*/
bool CX2UserSkillTree::IsMaxSkillLevel( IN CX2SkillTree::SKILL_ID eSkillID ) const
{
#ifdef GUILD_SKILL
	//{{ oasis907 : 김상윤 //// 2009-11-16 //// 
	if(IsGuildSkill(eSkillID))
	{
#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( eSkillID );
#else // UPGRADE_SKILL_SYSTEM_2013
		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( eSkillID, 1 );
#endif // UPGRADE_SKILL_SYSTEM_2013
		if( NULL == pSkillTemplet )
			return false;

		SkillDataMap::const_iterator mit;
		mit = m_mapGuildSkillAcquired.find( eSkillID );
		if( mit != m_mapGuildSkillAcquired.end() )
		{
			const UserSkillData& userSkillData = mit->second;
			if( userSkillData.m_iSkillLevel >= g_pData->GetSkillTree()->GetMaxGuildSkillLevel( (int)m_iGuildClass, eSkillID ) )
				return true;
			else 
				return false;
		}

		return false;
	}
	//}} oasis907 : 김상윤 //// 2009-11-16 //// 
#endif GUILD_SKILL

#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
	const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( eSkillID );
#else // UPGRADE_SKILL_SYSTEM_2013
	const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( eSkillID, 1 );
#endif // UPGRADE_SKILL_SYSTEM_2013
	if( NULL == pSkillTemplet )
		return false;

	SkillDataMap::const_iterator mit;
#ifdef SKILL_PAGE_SYSTEM //JHKang
	if ( true == m_vecSkillDataMap.empty() || m_usUsingPage >= m_vecSkillDataMap.size() )
		return false;

	mit = m_vecSkillDataMap[m_usUsingPage].find( eSkillID );
	if( mit != m_vecSkillDataMap[m_usUsingPage].end() )
#else //SKILL_PAGE_SYSTEM
	mit = m_mapSkillAcquired.find( eSkillID );
	if( mit != m_mapSkillAcquired.end() )
#endif //SKILL_PAGE_SYSTEM
	{
		const UserSkillData& userSkillData = mit->second;
		if( userSkillData.m_iSkillLevel >= g_pData->GetSkillTree()->GetMaxSkillLevel( (int)m_iUnitClass, eSkillID ) )
			return true;
		else 
			return false;
	}

	return false;
}

//{{ kimhc // 2010.12.14 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
/** @function	SetSkillSlotInfo
	@brief		스킬 슬롯 정보 설정(캐논볼 추가에 따른 구현)
	@param[in]	스킬 슬롯 ID
	@param[in]	스킬 ID
*/
void CX2UserSkillTree::SetSkillSlotInfo( int iSkillSlotID, CX2SkillTree::SKILL_ID eSkillID ) 
{
	// 캐논볼 추가
	switch( iSkillSlotID )
	{
	case CX2UserSkillTree::SKILL_SLOT_A1:
	case CX2UserSkillTree::SKILL_SLOT_A2:
	case CX2UserSkillTree::SKILL_SLOT_A3:
	case CX2UserSkillTree::SKILL_SLOT_A4:
		{
			const CX2SkillTree::SkillTemplet* pSkillTemplet = GetUserSkillTemplet( eSkillID);
			if(pSkillTemplet != NULL)
			{
#ifdef ARA_CHARACTER_BASE

	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
				if ( NULL == g_pData ||
					 NULL == g_pData->GetMyUser() || 
					 NULL == g_pData->GetMyUser()->GetSelectUnit() )
					return;
	
				const CX2UserSkillTree& userSkillTree = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_UserSkillTree;
	
				const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// 스킬 레벨
	
				SetSkillSlot( iSkillSlotID, eSkillID, pSkillTemplet->GetSkillMPConsumptionValue( iSkillTempletLevel ), pSkillTemplet->m_usCBConsumption,
					pSkillTemplet->m_usFPConsumtion, false );
	#else // UPGRADE_SKILL_SYSTEM_2013
				SetSkillSlot( iSkillSlotID, eSkillID, pSkillTemplet->m_fMPConsumption, pSkillTemplet->m_usCBConsumption,
					pSkillTemplet->m_usFPConsumtion, false );
	#endif // UPGRADE_SKILL_SYSTEM_2013
				
#else
				SetSkillSlot( iSkillSlotID, eSkillID, pSkillTemplet->m_fMPConsumption, pSkillTemplet->m_usCBConsumption, false );
#endif
			}
			else
			{
#ifdef ARA_CHARACTER_BASE
				SetSkillSlot( iSkillSlotID, eSkillID, 0, 0, 0, false );
#else
				SetSkillSlot( iSkillSlotID, eSkillID, 0, 0, false );
#endif
			}

		} break;

	case CX2UserSkillTree::SKILL_SLOT_B1:
	case CX2UserSkillTree::SKILL_SLOT_B2:
	case CX2UserSkillTree::SKILL_SLOT_B3:
	case CX2UserSkillTree::SKILL_SLOT_B4:
		{
			const CX2SkillTree::SkillTemplet* pSkillTemplet = GetUserSkillTemplet( eSkillID);
			if(pSkillTemplet != NULL)
			{
#ifdef ARA_CHARACTER_BASE

	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
				if ( NULL == g_pData ||
					NULL == g_pData->GetMyUser() || 
					NULL == g_pData->GetMyUser()->GetSelectUnit() )
					return;

				const CX2UserSkillTree& userSkillTree = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_UserSkillTree;

				const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( pSkillTemplet->m_eID ) );	/// 스킬 레벨

				SetSkillSlot( iSkillSlotID-(int)CX2UserSkillTree::SKILL_SLOT_B1, eSkillID, pSkillTemplet->GetSkillMPConsumptionValue( iSkillTempletLevel ), 
					pSkillTemplet->m_usCBConsumption, pSkillTemplet->m_usFPConsumtion, true );
	#else // UPGRADE_SKILL_SYSTEM_2013
				SetSkillSlot( iSkillSlotID-(int)CX2UserSkillTree::SKILL_SLOT_B1, eSkillID, pSkillTemplet->m_fMPConsumption, 
					pSkillTemplet->m_usCBConsumption, pSkillTemplet->m_usFPConsumtion, true );
	#endif // UPGRADE_SKILL_SYSTEM_2013
				
#else
				SetSkillSlot( iSkillSlotID-(int)CX2UserSkillTree::SKILL_SLOT_B1, eSkillID, pSkillTemplet->m_fMPConsumption, 
					pSkillTemplet->m_usCBConsumption, true );
#endif
			}
			else
			{
#ifdef ARA_CHARACTER_BASE
				SetSkillSlot( iSkillSlotID-(int)CX2UserSkillTree::SKILL_SLOT_B1, eSkillID, 0, 0, 0, true );
#else
				SetSkillSlot( iSkillSlotID-(int)CX2UserSkillTree::SKILL_SLOT_B1, eSkillID, 0, 0, true );
#endif
			}
		} break;
	}
}

#else	NEW_CHARACTER_CHUNG
/** @function	SetSkillSlotInfo
	@brief		스킬 슬롯 정보 설정
	@param[in]	스킬 슬롯 ID
	@param[in]	스킬 ID
*/
void CX2UserSkillTree::SetSkillSlotInfo( int iSkillSlotID, CX2SkillTree::SKILL_ID eSkillID ) 
{
	switch( iSkillSlotID )
	{
	case CX2UserSkillTree::SKILL_SLOT_A1:
	case CX2UserSkillTree::SKILL_SLOT_A2:
	case CX2UserSkillTree::SKILL_SLOT_A3:
	case CX2UserSkillTree::SKILL_SLOT_A4:
		{
			const CX2SkillTree::SkillTemplet* pSkillTemplet = GetUserSkillTemplet( eSkillID);
			if(pSkillTemplet != NULL)
			{
				SetSkillSlot( iSkillSlotID, eSkillID, pSkillTemplet->m_fMPConsumption, false );
			}
			else
			{
				SetSkillSlot( iSkillSlotID, eSkillID, 0, false );
			}

		} break;

	case CX2UserSkillTree::SKILL_SLOT_B1:
	case CX2UserSkillTree::SKILL_SLOT_B2:
	case CX2UserSkillTree::SKILL_SLOT_B3:
	case CX2UserSkillTree::SKILL_SLOT_B4:
		{
			const CX2SkillTree::SkillTemplet* pSkillTemplet = GetUserSkillTemplet( eSkillID);
			if(pSkillTemplet != NULL)
			{
				SetSkillSlot( iSkillSlotID-(int)CX2UserSkillTree::SKILL_SLOT_B1, eSkillID, pSkillTemplet->m_fMPConsumption, true );
			}
			else
			{
				SetSkillSlot( iSkillSlotID-(int)CX2UserSkillTree::SKILL_SLOT_B1, eSkillID, 0, true );
			}
		} break;
	}
}

#endif	NEW_CHARACTER_CHUNG
//}} kimhc // 2010.12.14 //  2010-12-23 New Character CHUNG

//{{ kimhc // 2010.12.14 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG

/** @function : GetCBConsumption
	@brief : 슬롯 인덱스와 슬롯A,B 에 따른 CB소모량을 얻어옴
	@param : const int iSlotIndex_, const bool bSlotB_
	@return : USHORT (CB 소모량)
*/
USHORT CX2UserSkillTree::GetCBConsumption( const int iSlotIndex_, const bool bSlotB_ ) const
{
	if( iSlotIndex_ < 0 || iSlotIndex_ > ARRAY_SIZE( m_aEquippedSkill ) -1 )
		return 0;

	if( true == bSlotB_ )
	{
		return m_aEquippedSkillSlotB[iSlotIndex_].m_usCBConsumption;
	}
	else
	{
		return m_aEquippedSkill[iSlotIndex_].m_usCBConsumption;
	}
}

/** @function : SetCBConsumption
	@brief : 현재 슬롯에 장착되었있는 CB 소모량을 Set함
	@param : int iSlotIndex_, bool bSlotB_, int iCBConsumption_
*/
void CX2UserSkillTree::SetCBConsumption( int iSlotIndex_, bool bSlotB_, USHORT usCBConsumption_ )
{
	if( iSlotIndex_ < 0 || iSlotIndex_ > ARRAY_SIZE( m_aEquippedSkill ) -1 )
		return;

	if( true == bSlotB_ )
	{
		m_aEquippedSkillSlotB[iSlotIndex_].m_usCBConsumption = usCBConsumption_;
	}
	else
	{
		m_aEquippedSkill[iSlotIndex_].m_usCBConsumption = usCBConsumption_;
	}
}

#pragma region ForcePower
#ifdef ARA_CHARACTER_BASE
/** @function 	: GetFPConsumption
	@brief 		: 해당 슬롯의 기력 사용량 확인
*/
USHORT CX2UserSkillTree::GetFPConsumption( const int iSlotIndex_, const bool bSlotB_ ) const
{
	if( iSlotIndex_ < 0 || iSlotIndex_ > ARRAY_SIZE( m_aEquippedSkill ) -1 )
		return 0;

	if( true == bSlotB_ )
	{
		return m_aEquippedSkillSlotB[iSlotIndex_].m_usFPConsumption;
	}
	else
	{
		return m_aEquippedSkill[iSlotIndex_].m_usFPConsumption;
	}
}

/** @function 	: SetFPConsumption
	@brief 		: 해당 슬롯의 기력 사용량 설정
*/
void CX2UserSkillTree::SetFPConsumption( int iSlotIndex_, bool bSlotB_, USHORT usFPConsumption_ )
{
	if( iSlotIndex_ < 0 || iSlotIndex_ > ARRAY_SIZE( m_aEquippedSkill ) -1 )
		return;

	if( true == bSlotB_ )
	{
		m_aEquippedSkillSlotB[iSlotIndex_].m_usFPConsumption = usFPConsumption_;
	}
	else
	{
		m_aEquippedSkill[iSlotIndex_].m_usFPConsumption = usFPConsumption_;
	}
}

/** @function 	: GetFPConsumption
	@brief 		: 해당 슬롯의 기력 사용량 확인
*/
USHORT CX2UserSkillTree::GetFPGainCount( const int iSlotIndex_, const bool bSlotB_ ) const
{
	if( iSlotIndex_ < 0 || iSlotIndex_ > ARRAY_SIZE( m_aEquippedSkill ) -1 )
		return 0;

	if( true == bSlotB_ )
	{
		return m_aEquippedSkillSlotB[iSlotIndex_].m_usFPGainCount;
	}
	else
	{
		return m_aEquippedSkill[iSlotIndex_].m_usFPGainCount;
	}
}

/** @function 	: SetFPConsumption
	@brief 		: 해당 슬롯의 기력 사용량 설정
*/
void CX2UserSkillTree::SetFPGainCount( int iSlotIndex_, bool bSlotB_, USHORT usFPGainCount_ )
{
	if( iSlotIndex_ < 0 || iSlotIndex_ > ARRAY_SIZE( m_aEquippedSkill ) -1 )
		return;

	if( true == bSlotB_ )
	{
		m_aEquippedSkillSlotB[iSlotIndex_].m_usFPGainCount = usFPGainCount_;
	}
	else
	{
		m_aEquippedSkill[iSlotIndex_].m_usFPGainCount = usFPGainCount_;
	}
}
#endif
#pragma endregion 기력


/** @function	SetSkillSlot
	@brief		스킬 슬롯 설정
	@param[in]	슬롯 인덱스
	@param[in]	스킬 ID
	@param[in]	소모 마나
	@param[in]	소모 캐논볼
	@param[in]	소모 기력
	@param[in]	B 슬롯 인지 아닌지
*/
#ifdef ARA_CHARACTER_BASE
bool CX2UserSkillTree::SetSkillSlot( int iSlotIndex_, CX2SkillTree::SKILL_ID eSkillID_, float fMPConsume_, USHORT usCBConsume_,
									 USHORT usFPConsume, bool bSlotB_ )
#else
bool CX2UserSkillTree::SetSkillSlot( int iSlotIndex_, CX2SkillTree::SKILL_ID eSkillID_, float fMPConsume_, USHORT usCBConsume_, bool bSlotB_ )
#endif
{
	if( iSlotIndex_ < 0 ||
		iSlotIndex_ > ARRAY_SIZE( m_aEquippedSkill ) - 1 )
	{
		return false;
	}

	//if( true == bSlotB &&
	//	false == GetEnabledSkillSlotB() )
	//{
	//	return false;
	//}

	if( true == bSlotB_ )
	{
		m_aEquippedSkillSlotB[iSlotIndex_].m_eID = eSkillID_;
		m_aEquippedSkillSlotB[iSlotIndex_].m_fCoolTimeLeft = 0.f;
		m_aEquippedSkillSlotB[iSlotIndex_].m_fMPConsumption = fMPConsume_;
		m_aEquippedSkillSlotB[iSlotIndex_].m_usCBConsumption = usCBConsume_;
#ifdef ARA_CHARACTER_BASE
		m_aEquippedSkillSlotB[iSlotIndex_].m_usFPConsumption = usFPConsume;
#endif
	}
	else
	{
		m_aEquippedSkill[iSlotIndex_].m_eID = eSkillID_;
		m_aEquippedSkill[iSlotIndex_].m_fCoolTimeLeft = 0.f;
		m_aEquippedSkill[iSlotIndex_].m_fMPConsumption = fMPConsume_;
		m_aEquippedSkill[iSlotIndex_].m_usCBConsumption = usCBConsume_;
#ifdef ARA_CHARACTER_BASE
		m_aEquippedSkillSlotB[iSlotIndex_].m_usFPConsumption = usFPConsume;
#endif
	}

	return true;
}


#else	NEW_CHARACTER_CHUNG
/** @function	SetSkillSlot
	@brief		스킬 슬롯 설정
	@param[in]	슬롯 인덱스
	@param[in]	스킬 ID
	@param[in]	소모 마나
	@param[in]	B 슬롯 인지 아닌지
*/
bool CX2UserSkillTree::SetSkillSlot( int iSlotIndex, CX2SkillTree::SKILL_ID eSkillID, float fMPConsumption, bool bSlotB )
{
	if( iSlotIndex < 0 ||
		iSlotIndex > ARRAY_SIZE( m_aEquippedSkill ) - 1 )
	{
		return false;
	}

	//if( true == bSlotB &&
	//	false == GetEnabledSkillSlotB() )
	//{
	//	return false;
	//}

	if( true == bSlotB )
	{
		m_aEquippedSkillSlotB[iSlotIndex].m_eID = eSkillID;
		m_aEquippedSkillSlotB[iSlotIndex].m_fCoolTimeLeft = 0.f;
		m_aEquippedSkillSlotB[iSlotIndex].m_fMPConsumption = fMPConsumption;
	}
	else
	{
		m_aEquippedSkill[iSlotIndex].m_eID = eSkillID;
		m_aEquippedSkill[iSlotIndex].m_fCoolTimeLeft = 0.f;
		m_aEquippedSkill[iSlotIndex].m_fMPConsumption = fMPConsumption;
	}

	return true;
}

#endif	NEW_CHARACTER_CHUNG
//}} kimhc // 2010.12.14 //  2010-12-23 New Character CHUNG

/** @function	GetSkillSlot
	@brief		스킬 슬롯 얻기
	@param[in]	슬롯 인덱스
	@param[in]	B 슬롯인지 아닌지
*/
const CX2UserSkillTree::SkillSlotData* CX2UserSkillTree::GetSkillSlot( int iSlotIndex, bool bSlotB ) const
{
	if( iSlotIndex < 0 ||
		iSlotIndex > ARRAY_SIZE( m_aEquippedSkill ) -1 )
	{
		return NULL;
	}
	
	if( iSlotIndex >= 4 )
	{
		bSlotB = true;
		iSlotIndex -= 4;
	}
	
	if( true == bSlotB )
	{
		return &m_aEquippedSkillSlotB[iSlotIndex];
	}
	else
	{
		return &m_aEquippedSkill[iSlotIndex];
	}
}

/** @function : GetSkillSlot
	@brief : 스킬슬롯데이터의 포인터를 얻는 함수
	@param : 스킬ID(eSkillID_)
	@return : 스킬슬롯데이터 포인터
*/
const CX2UserSkillTree::SkillSlotData* CX2UserSkillTree::GetSkillSlot( const CX2SkillTree::SKILL_ID eSkillID_ ) const
{
	const UINT uiArraySize = ARRAY_SIZE( m_aEquippedSkill );

	for ( UINT uiIndex = 0; uiIndex < uiArraySize; ++uiIndex )
	{
		if ( eSkillID_ == m_aEquippedSkill[uiIndex].m_eID )
			return &m_aEquippedSkill[uiIndex];
		else if ( eSkillID_ == m_aEquippedSkillSlotB[uiIndex].m_eID )
			return &m_aEquippedSkillSlotB[uiIndex];			
	}

	return NULL;
}

/** @function : ChangeMpConsumptionPercent
	@brief : Mp 소모율 변경
	@param : Mp 소모율을 변경할 스킬ID, 변경 인자(fMultiplier)
*/
void CX2UserSkillTree::ChangeMpConsumptionPercent( const CX2SkillTree::SKILL_ID eSkillID_, const float fMultiplier_ )
{
	const UINT uiArraySize = ARRAY_SIZE( m_aEquippedSkill );

	for ( UINT uiIndex = 0; uiIndex < uiArraySize; ++uiIndex )
	{
		if ( eSkillID_ == m_aEquippedSkill[uiIndex].m_eID )
		{
			m_aEquippedSkill[uiIndex].m_fMPConsumption *= fMultiplier_;
			break;
		}
		else if ( eSkillID_ == m_aEquippedSkillSlotB[uiIndex].m_eID )
		{
			m_aEquippedSkillSlotB[uiIndex].m_fMPConsumption *= fMultiplier_;
			break;
		}
	}
}

/** @function	SetMpConsumption
	@brief		마나 소모량 설정
	@param[in]	슬롯 인덱스
	@param[in]	B 슬롯?
	@param[in]	마나 소모량
*/
void CX2UserSkillTree::SetMpConsumption( int iSlotIndex_, bool bSlotB_, float fMpConsumption_ )
{
	if( iSlotIndex_ < 0 || iSlotIndex_ > ARRAY_SIZE( m_aEquippedSkill ) -1 )
		return;

	if( true == bSlotB_ )
	{
		m_aEquippedSkillSlotB[iSlotIndex_].m_fMPConsumption = fMpConsumption_;
	}
	else
	{
		m_aEquippedSkill[iSlotIndex_].m_fMPConsumption = fMpConsumption_;
	}
}

/** @function	GetSkillStat
	@brief		스킬 Stat 얻기
	@return		CX2Stat::Stat
*/
CX2Stat::Stat CX2UserSkillTree::GetSkillStat() const
{
	CX2Stat::Stat retStat;

#ifdef SKILL_PAGE_SYSTEM //JHKang
	if ( true == m_vecSkillDataMap.empty() || m_usUsingPage >= m_vecSkillDataMap.size() )
		return retStat;

	BOOST_TEST_FOREACH( const SkillDataMap::value_type&, value, m_vecSkillDataMap[m_usUsingPage] )
#else //SKILL_PAGE_SYSTEM
	BOOST_TEST_FOREACH( const SkillDataMap::value_type&, value, m_mapSkillAcquired )
#endif //SKILL_PAGE_SYSTEM
	{
		const UserSkillData& userSkillData = value.second;
		
#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( value.first );
#else // UPGRADE_SKILL_SYSTEM_2013
		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( value.first, userSkillData.m_iSkillLevel );
#endif // UPGRADE_SKILL_SYSTEM_2013
		
		if( pSkillTemplet == NULL )
			continue;

		switch( pSkillTemplet->m_eType )
		{
		case CX2SkillTree::ST_PASSIVE_PHYSIC_ATTACK: 
		case CX2SkillTree::ST_PASSIVE_MAGIC_ATTACK:  
		case CX2SkillTree::ST_PASSIVE_MAGIC_DEFENCE: 
		case CX2SkillTree::ST_PASSIVE_PHYSIC_DEFENCE:
			{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
				retStat.AddStat( pSkillTemplet->GetSkillStatValue( userSkillData.m_iSkillLevel ), false );
	#else // UPGRADE_SKILL_SYSTEM_2013
				retStat.AddStat( pSkillTemplet->m_Stat, false );
	#endif // UPGRADE_SKILL_SYSTEM_2013
			} break;
		}
	}

	return retStat;
}

#ifdef GUILD_SKILL
//{{ oasis907 : 김상윤 //// 2009-11-12 //// 
/** @function	GetGuildSkillStat
	@brief		길드 스킬 스탯 얻기
	@return		CX2Stat:Stat
*/
CX2Stat::Stat CX2UserSkillTree::GetGuildSkillStat(int iGuildMemberGrade) const
{
	CX2Stat::Stat retStat;

	// oasis907 : 김상윤 [2009.12.8] // 길드 등급
	if(iGuildMemberGrade == CX2GuildManager::GUG_INVALID ||
		iGuildMemberGrade >= g_pData->GetGuildManager()->GetLimitGuildGrade()) // 지정될 커트라인 길드 멤버 등급
	{
		return retStat;
	}

	BOOST_TEST_FOREACH( const SkillDataMap::value_type&, value, m_mapGuildSkillAcquired )
	{
		const UserSkillData& userSkillData = value.second;

	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( value.first );

		if( pSkillTemplet == NULL )
			continue;

		switch( pSkillTemplet->m_eType )
		{
		case CX2SkillTree::ST_GUILD_PASSIVE_PHYSIC_ATTACK_MAGIC_DEFENCE: 
		case CX2SkillTree::ST_GUILD_PASSIVE_MAGIC_ATTCK_PHYSIC_DEFENCE:  
			{
				retStat.AddStat( pSkillTemplet->GetSkillStatValue( userSkillData.m_iSkillLevel ), false );
			} break;
		}
	#else // UPGRADE_SKILL_SYSTEM_2013
		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( value.first, userSkillData.m_iSkillLevel );

		if( pSkillTemplet == NULL )
			continue;

		switch( pSkillTemplet->m_eType )
		{
		case CX2SkillTree::ST_GUILD_PASSIVE_PHYSIC_ATTACK_MAGIC_DEFENCE: 
		case CX2SkillTree::ST_GUILD_PASSIVE_MAGIC_ATTCK_PHYSIC_DEFENCE:  
			{
				retStat.AddStat( pSkillTemplet->m_Stat, false );
			} break;
		}
	#endif // UPGRADE_SKILL_SYSTEM_2013
	}

	return retStat;
}
//}} oasis907 : 김상윤 //// 2009-11-12 //// 
#endif GUILD_SKILL

/** @function	CalcUsedSPointAndCSPoint
	@brief		사용된 Skill Point와 캐쉬 스킬 포인트 값 계산
	@param[out]	iSPont : 스킬 포인트
	@param[out]	iCSPoint : 캐쉬 스킬 포인트
*/
void CX2UserSkillTree::CalcUsedSPointAndCSPoint( OUT int& iSPoint, OUT int& iCSPoint ) const
{
#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
	if ( NULL == g_pData || NULL == g_pData->GetSkillTree() )
		return;

	iSPoint		= 0;
	iCSPoint	= 0;

#ifdef SKILL_PAGE_SYSTEM //JHKang
	if ( m_vecSkillDataMap.size() < m_usUsingPage )
		return;

	for( SkillDataMap::const_iterator mit = m_vecSkillDataMap[m_usUsingPage].begin(); mit != m_vecSkillDataMap[m_usUsingPage].end(); ++mit )
#else //SKILL_PAGE_SYSTEM
	for( SkillDataMap::const_iterator mit = m_mapSkillAcquired.begin(); mit != m_mapSkillAcquired.end(); ++mit )
#endif //SKILL_PAGE_SYSTEM
	{
#ifdef ADDED_RELATIONSHIP_SYSTEM
		/// 결혼 스킬이라면, 스킬 포인트 연산 제외
		CX2SkillTree::SKILL_ID eSkillID = mit->first;

		if ( CX2SkillTree::SI_ETC_WS_COMMON_LOVE == eSkillID )
			continue;
#endif ADDED_RELATIONSHIP_SYSTEM
#ifndef ADDED_RELATIONSHIP_SYSTEM
		CX2SkillTree::SKILL_ID eSkillID = mit->first;
#endif ADDED_RELATIONSHIP_SYSTEM 
		
		/// 스킬 템플릿 반환
		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( eSkillID );

		if ( NULL != pSkillTemplet )
		{
			const UserSkillData& userSkillData = mit->second;

			/// 적용된 스킬 포인트 연산 ( 캐시 스킬 포인트도 포함 )
			for( int i = 0 ; i < userSkillData.m_iSkillLevel ; ++i )
			{
				if( i == 0 )	/// 레벨이 1일 떼
				{
					if( true == g_pData->GetSkillTree()->isDefaultSkill( eSkillID ) )	/// 기본 스킬이면 제외
						continue;

					iSPoint += pSkillTemplet->m_iRequireLearnSkillPoint;	/// 습득 포인트를 연산
				}
				else
				{
					iSPoint += pSkillTemplet->m_iRequireUpgradeSkillPoint;	/// 강화 포인트를 연산
				}
			}

			iCSPoint	+= userSkillData.m_iSkillCSPoint;	/// 캐시 스킬 포인트를 연산
			iSPoint		-= userSkillData.m_iSkillCSPoint;	/// 적용된 캐시 스킬 포인트를 제외시켜, 일반 스킬 포인트만 연산
			
		}
	}

#else // UPGRADE_SKILL_SYSTEM_2013

	iSPoint = 0;
	iCSPoint = 0;

	for( SkillDataMap::iterator mit = m_mapSkillAcquired.begin(); mit != m_mapSkillAcquired.end(); ++mit )
	{
#ifdef ADDED_RELATIONSHIP_SYSTEM
		/// 결혼 스킬이라면, 스킬 포인트 연산 제외
		CX2SkillTree::SKILL_ID eSkillID = mit->first;

		if ( CX2SkillTree::SI_ETC_WS_COMMON_LOVE == eSkillID )
			continue;
#endif ADDED_RELATIONSHIP_SYSTEM

		const UserSkillData& userSkillData = mit->second;

		if( userSkillData.m_iSkillLevel > 0 )
		{
			iSPoint		+= userSkillData.m_iSkillLevel - userSkillData.m_iSkillCSPoint;
			iCSPoint	+= userSkillData.m_iSkillCSPoint;
		}
	}

#endif // UPGRADE_SKILL_SYSTEM_2013
}

/** @function	CalcCumulativeUsedSPointOnEachTier
	@brief		tier별로 소모된 누적 SP+CSP를 계산해준다
	@param[out]	vecTierSPoint : 
*/
void CX2UserSkillTree::CalcCumulativeUsedSPointOnEachTier( OUT std::vector<int>& vecTierSPoint ) const
{

	int iMaxTierIndex = 0;
#ifdef SKILL_PAGE_SYSTEM //JHKang
	if ( true == m_vecSkillDataMap.empty() || m_usUsingPage >= m_vecSkillDataMap.size() )
		return;

	for( SkillDataMap::const_iterator mit = m_vecSkillDataMap[m_usUsingPage].begin(); mit != m_vecSkillDataMap[m_usUsingPage].end(); ++mit )
#else //SKILL_PAGE_SYSTEM
	for( SkillDataMap::const_iterator mit = m_mapSkillAcquired.begin(); mit != m_mapSkillAcquired.end(); ++mit )
#endif //SKILL_PAGE_SYSTEM
	{
		const UserSkillData& userSkillData = mit->second;
		if( userSkillData.m_iSkillLevel > 0 )
		{
			const CX2SkillTree::SkillTreeTemplet* pSkillTreeTemplet = g_pData->GetSkillTree()->GetSkillTreeTemplet( (int)m_iUnitClass, mit->first );
			if( NULL != pSkillTreeTemplet )
			{
				if( iMaxTierIndex < pSkillTreeTemplet->m_iTier )
					iMaxTierIndex = pSkillTreeTemplet->m_iTier;
			}
		}
	}

	vecTierSPoint.resize(0);
	for( int i=0; i<= iMaxTierIndex; i++ )
	{
		vecTierSPoint.push_back( 0 );
	}


	
#ifdef SKILL_PAGE_SYSTEM //JHKang
	for( SkillDataMap::const_iterator mit = m_vecSkillDataMap[m_usUsingPage].begin(); mit != m_vecSkillDataMap[m_usUsingPage].end(); ++mit )
#else //SKILL_PAGE_SYSTEM
	for( SkillDataMap::const_iterator mit = m_mapSkillAcquired.begin(); mit != m_mapSkillAcquired.end(); ++mit )
#endif //SKILL_PAGE_SYSTEM
	{
		const UserSkillData& userSkillData = mit->second;
		if( userSkillData.m_iSkillLevel > 0 )
		{
			const CX2SkillTree::SkillTreeTemplet* pSkillTreeTemplet = g_pData->GetSkillTree()->GetSkillTreeTemplet( (int)m_iUnitClass, mit->first );
			if( NULL != pSkillTreeTemplet )
			{
				vecTierSPoint[ pSkillTreeTemplet->m_iTier ] += userSkillData.m_iSkillLevel; 

//{{ kimhc // 2010.8.17 // 스킬 되돌리기가 되면 안되는 상황에서 되돌리기가 되어 비정상적인 스킬트리가 되는 문제를 해결한 것입니다.
#ifdef	SERV_RESET_SKILL_BUG_FIX
				vecTierSPoint[ pSkillTreeTemplet->m_iTier ] -= userSkillData.m_iSkillCSPoint; 
#endif	SERV_RESET_SKILL_BUG_FIX
//}} kimhc // 2010.8.17 // 스킬 되돌리기가 되면 안되는 상황에서 되돌리기가 되어 비정상적인 스킬트리가 되는 문제를 해결한 것입니다.
			}
		}
	}


	int iCumulativeTierSPoint = 0;
	BOOST_TEST_FOREACH( int&, iTierSPoint, vecTierSPoint )
	{
		iCumulativeTierSPoint += iTierSPoint;
		iTierSPoint = iCumulativeTierSPoint;
	}
}


#ifdef GUILD_SKILL
//{{ oasis907 : 김상윤 //// 2009-11-12 //// 
void CX2UserSkillTree::CalcUsedGuildSPointAndCSPoint( OUT int& iSPoint, OUT int& iCSPoint ) const
{
	iSPoint = 0;
	iCSPoint = 0;

	for( SkillDataMap::const_iterator mit = m_mapGuildSkillAcquired.begin(); mit != m_mapGuildSkillAcquired.end(); ++mit )
	{
		const UserSkillData& userSkillData = mit->second;

		if( userSkillData.m_iSkillLevel > 0 )
		{
			iSPoint		+= userSkillData.m_iSkillLevel - userSkillData.m_iSkillCSPoint;
			iCSPoint	+= userSkillData.m_iSkillCSPoint;
		}

	}
}



// tier별로 소모된 누적 SP+CSP를 계산해준다
void CX2UserSkillTree::CalcCumulativeUsedGuildSPointOnEachTier( OUT std::vector<int>& vecTierSPoint ) const
{

	int iMaxTierIndex = 0;
	for( SkillDataMap::const_iterator mit = m_mapGuildSkillAcquired.begin(); mit != m_mapGuildSkillAcquired.end(); ++mit )
	{
		const UserSkillData& userSkillData = mit->second;
		if( userSkillData.m_iSkillLevel > 0 )
		{
			const CX2SkillTree::SkillTreeTemplet* pSkillTreeTemplet = g_pData->GetSkillTree()->GetGuildSkillTreeTemplet( (int)m_iGuildClass, mit->first );
			if( NULL != pSkillTreeTemplet )
			{
				if( iMaxTierIndex < pSkillTreeTemplet->m_iTier )
					iMaxTierIndex = pSkillTreeTemplet->m_iTier;
			}
		}
	}

	vecTierSPoint.resize(0);
	for( int i=0; i<= iMaxTierIndex; i++ )
	{
		vecTierSPoint.push_back( 0 );
	}



	for( SkillDataMap::const_iterator mit = m_mapGuildSkillAcquired.begin(); mit != m_mapGuildSkillAcquired.end(); ++mit )
	{
		const UserSkillData& userSkillData = mit->second;
		if( userSkillData.m_iSkillLevel > 0 )
		{
			const CX2SkillTree::SkillTreeTemplet* pSkillTreeTemplet = g_pData->GetSkillTree()->GetGuildSkillTreeTemplet( (int)m_iGuildClass, mit->first );
			if( NULL != pSkillTreeTemplet )
			{
				vecTierSPoint[ pSkillTreeTemplet->m_iTier ] += userSkillData.m_iSkillLevel; 
				//{{ kimhc // 2010.8.17 // 스킬 되돌리기가 되면 안되는 상황에서 되돌리기가 되어 비정상적인 스킬트리가 되는 문제를 해결한 것입니다.
#ifdef	SERV_RESET_SKILL_BUG_FIX
				vecTierSPoint[ pSkillTreeTemplet->m_iTier ] -= userSkillData.m_iSkillCSPoint; 
#endif	SERV_RESET_SKILL_BUG_FIX
				//}} kimhc // 2010.8.17 // 스킬 되돌리기가 되면 안되는 상황에서 되돌리기가 되어 비정상적인 스킬트리가 되는 문제를 해결한 것입니다.
			}
		}
	}


	int iCumulativeTierSPoint = 0;
	BOOST_TEST_FOREACH( int&, iTierSPoint, vecTierSPoint )
	{
		iCumulativeTierSPoint += iTierSPoint;
		iTierSPoint = iCumulativeTierSPoint;
	}
}
//}} oasis907 : 김상윤 //// 2009-11-12 //// 
#endif GUILD_SKILL


// cash skill point로 찍은 스킬의 레벨을 조정한다
#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환
void CX2UserSkillTree::ExpireCashSkillPoint( OUT const std::map<int, int> mapHaveSKill_ )
#else // UPGRADE_SKILL_SYSTEM_2013
void CX2UserSkillTree::ExpireCashSkillPoint()
#endif // UPGRADE_SKILL_SYSTEM_2013
{
#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환

	/// 배운 스킬 목록 초기화
	Reset( true, false, false, false );

	/// 그노시스 습득 이전에 배웠던 스킬들로 설정
	std::map<int, int>::const_iterator it = mapHaveSKill_.begin();

	for ( it; it != mapHaveSKill_.end(); ++it  )
	{
		const CX2SkillTree:: SKILL_ID	eSkillID	= static_cast<CX2SkillTree::SKILL_ID>( it->first );
		const int						eSkillLevel	= it->second;

#ifdef SKILL_PAGE_SYSTEM //JHKang
		SetSkillLevelAndCSP( eSkillID, eSkillLevel, 0 );
#else //SKILL_PAGE_SYSTEM
		SetSkillLevelAndCSP( eSkillID, eSkillLevel, 0 );
#endif //SKILL_PAGE_SYSTEM
	}

#else // UPGRADE_SKILL_SYSTEM_2013 

	for( SkillDataMap::iterator mit = m_mapSkillAcquired.begin(); mit != m_mapSkillAcquired.end(); ++mit )
	{
		UserSkillData& userSkillData = mit->second;

		if( userSkillData.m_iSkillCSPoint > 0 )
		{
			userSkillData.m_iSkillLevel		-= userSkillData.m_iSkillCSPoint;
			userSkillData.m_iSkillCSPoint	= 0;
		}
	}

#endif UPGRADE_SKILL_SYSTEM_2013


	// 장착된 스킬 중에서 skill level이 0이하인 스킬이 있으면 탈착시킨다.
	for( int i=0; i<ARRAY_SIZE( m_aEquippedSkill ); ++i )
	{
		SkillSlotData& skillSlotData = m_aEquippedSkill[i];
		if( false == DoIHaveThisSkill( skillSlotData.m_eID ) )
		{
//{{ kimhc // 2010.12.14 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
#ifdef ARA_CHARACTER_BASE
			SetSkillSlot( i, CX2SkillTree::SI_NONE, 0, 0, 0, false );
#else
			SetSkillSlot( i, CX2SkillTree::SI_NONE, 0, 0, false );
#endif //ARA_CHARACTER_BASE
#else	NEW_CHARACTER_CHUNG
			SetSkillSlot( i, CX2SkillTree::SI_NONE, 0, false );
#endif	NEW_CHARACTER_CHUNG
//}} kimhc // 2010.12.14 //  2010-12-23 New Character CHUNG
		}
	}


	for( int i=0; i<ARRAY_SIZE( m_aEquippedSkillSlotB ); i++ )
	{
		SkillSlotData& skillSlotData = m_aEquippedSkillSlotB[i];
		if( false == DoIHaveThisSkill( skillSlotData.m_eID ) )
		{
			//{{ kimhc // 2010.12.14 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
#ifdef ARA_CHARACTER_BASE
			SetSkillSlot( i, CX2SkillTree::SI_NONE, 0, 0, 0, true );
#else
			SetSkillSlot( i, CX2SkillTree::SI_NONE, 0, 0, true );
#endif //ARA_CHARACTER_BASE
#else	NEW_CHARACTER_CHUNG
			SetSkillSlot( i, CX2SkillTree::SI_NONE, 0, true );
#endif	NEW_CHARACTER_CHUNG
			//}} kimhc // 2010.12.14 //  2010-12-23 New Character CHUNG
		}
	}
}
#ifdef GUILD_SKILL
//{{ oasis907 : 김상윤 [2009.11.27] //
// cash skill point로 찍은 스킬의 레벨을 조정한다
void CX2UserSkillTree::ExpireGuildCashSkillPoint()
{
	for( SkillDataMap::iterator mit = m_mapGuildSkillAcquired.begin(); mit != m_mapGuildSkillAcquired.end(); ++mit )
	{
		UserSkillData& userSkillData = mit->second;

		if( userSkillData.m_iSkillCSPoint > 0 )
		{
			userSkillData.m_iSkillLevel		-= userSkillData.m_iSkillCSPoint;
			userSkillData.m_iSkillCSPoint	= 0;
		}
	}
    // oasis907 : 김상윤 [2009.11.27] // 길드 스킬에 액티브가 없으므로 필요없음
/*
	// 장착된 스킬 중에서 skill level이 0이하인 스킬이 있으면 탈착시킨다.
	for( int i=0; i<ARRAY_SIZE( m_aEquippedSkill ); i++ )
	{
		SkillSlotData& skillSlotData = m_aEquippedSkill[i];
		if( false == DoIHaveThisSkill( skillSlotData.m_eID ) )
		{
			SetSkillSlot( i, CX2SkillTree::SI_NONE, 0, false );
		}
	}


	for( int i=0; i<ARRAY_SIZE( m_aEquippedSkillSlotB ); i++ )
	{
		SkillSlotData& skillSlotData = m_aEquippedSkillSlotB[i];
		if( false == DoIHaveThisSkill( skillSlotData.m_eID ) )
		{
			SetSkillSlot( i, CX2SkillTree::SI_NONE, 0, false );
		}
	}
*/
}
#endif GUILD_SKILL




//}} oasis907 : 김상윤 [2009.11.27] //

bool CX2UserSkillTree::IsSkillUnsealed( CX2SkillTree::SKILL_ID eSkillID ) const
{
	std::set< CX2SkillTree::SKILL_ID >::const_iterator it = m_setUnsealedSkillID.find( eSkillID );
	if( it != m_setUnsealedSkillID.end() )
	{
		return true;
	}

	return false;
}



void CX2UserSkillTree::AddSkillUnsealed( CX2SkillTree::SKILL_ID eSkillID )
{
	if( true == IsSkillUnsealed( eSkillID ) )
		return;

	m_setUnsealedSkillID.insert( eSkillID );
}


bool CX2UserSkillTree::DoIHaveThisSkill( CX2SkillTree::SKILL_ID eSkillID ) const
{
#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경

	/// 현재 스킬 포인트가 투자된 스킬인지 검사
	if( GetSkillLevel(eSkillID) > 0 )
		return true;
	/// 아니라면, 스킬창에서 스킬 레벨을 올리고 있는 스킬인지 검사
	else if (	NULL != g_pData &&
				NULL != g_pData->GetUIManager() &&
				NULL != g_pData->GetSkillTree() &&
				NULL != g_pData->GetUIManager()->GetUISkillTree() &&
				true == g_pData->GetUIManager()->GetUISkillTree()->GetNowLearnSkill() )
	{
		/// 올린 목록에 해당 스킬이 있으면, 소유중으로 처리
		const std::map<int, KGetSkillInfo> mapSkillInfo = g_pData->GetSkillTree()->GetMapSkillInfo();

		const std::map<int, KGetSkillInfo>::const_iterator it = mapSkillInfo.find( static_cast<int>( eSkillID ) );

		if ( it != mapSkillInfo.end() )
			return true;
	}

	return false;
#else // UPGRADE_SKILL_SYSTEM_2013

	if( GetSkillLevel(eSkillID) <= 0 )
		return false;

	return true;

#endif // UPGRADE_SKILL_SYSTEM_2013
}


bool CX2UserSkillTree::CanIResetThisSkill( CX2SkillTree::SKILL_ID eSkillID ) const
{
#ifdef GUILD_SKILL
//{{ oasis907 : 김상윤 //// 2009-11-17 //// 
	if(IsGuildSkill(eSkillID))
	{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
		if( true == g_pData->GetSkillTree()->IsUnitTypeDefaultSkill( eSkillID ) && GetSkillLevel( eSkillID, true ) <= 1)	// 태완 수정 : 1레벨까지는 떨굴 수 있게..
	#else // UPGRADE_SKILL_SYSTEM_2013
		if( true == g_pData->GetSkillTree()->IsUnitTypeDefaultSkill( eSkillID ) && GetSkillLevel( eSkillID ) <= 1)	// 태완 수정 : 1레벨까지는 떨굴 수 있게..
	#endif // UPGRADE_SKILL_SYSTEM_2013
			return false;

		int iMySkillLevel = GetSkillLevel( eSkillID );
		if( iMySkillLevel <= 0 )
			return false;

	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( eSkillID );
	#else // UPGRADE_SKILL_SYSTEM_2013
		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( eSkillID, iMySkillLevel );
	#endif // UPGRADE_SKILL_SYSTEM_2013
		
		if( NULL == pSkillTemplet )
			return false;


		const CX2SkillTree::SkillTreeTemplet* pSkillTreeTemplet = g_pData->GetSkillTree()->GetGuildSkillTreeTemplet( (int)m_iGuildClass, eSkillID );
		if( NULL == pSkillTreeTemplet )
			return false;


#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
		CX2SkillTree::SKILL_ID eFollowingSkillID = static_cast<CX2SkillTree::SKILL_ID>(pSkillTreeTemplet->m_iFollowingSkill);
		if( true == DoIHaveThisSkill( eFollowingSkillID ) )
			return false;
#else // UPGRADE_SKILL_SYSTEM_2013
		for( UINT i=0; i<pSkillTreeTemplet->m_vecFollowingSkill.size(); i++ )
		{
			CX2SkillTree::SKILL_ID eFollowingSkillID = (CX2SkillTree::SKILL_ID) pSkillTreeTemplet->m_vecFollowingSkill[i];
			if( true == DoIHaveThisSkill( eFollowingSkillID ) )
				return false;
		}
#endif // UPGRADE_SKILL_SYSTEM_2013


		// 스킬 되돌리기를 하고 난 후에 후행 tier에 스킬을 못 찍게 되는 경우 스킬 리셋을 할 수 없다
		std::vector<int> vecCumulativeTierSPoint;
		CalcCumulativeUsedGuildSPointOnEachTier( vecCumulativeTierSPoint );


		const int MAGIC_SKILL_POINT_PER_TIER = 5;
		for( UINT i=pSkillTreeTemplet->m_iTier+1; i<vecCumulativeTierSPoint.size(); i++ )
		{
			if( vecCumulativeTierSPoint[i-1]-1 < (int)i * MAGIC_SKILL_POINT_PER_TIER )
			{
				return false;
			}
		}

		return true;
	}
//}} oasis907 : 김상윤 //// 2009-11-17 //// 
#endif GUILD_SKILL

#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
	if( true == g_pData->GetSkillTree()->IsUnitTypeDefaultSkill( eSkillID ) && GetSkillLevel( eSkillID, true ) <= 1)	// 태완 수정 : 1레벨까지는 떨굴 수 있게..
#else // UPGRADE_SKILL_SYSTEM_2013
	if( true == g_pData->GetSkillTree()->IsUnitTypeDefaultSkill( eSkillID ) && GetSkillLevel( eSkillID ) <= 1)	// 태완 수정 : 1레벨까지는 떨굴 수 있게..
#endif // UPGRADE_SKILL_SYSTEM_2013
		return false;


	int iMySkillLevel = GetSkillLevel( eSkillID );
	if( iMySkillLevel <= 0 )
		return false;

#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
	const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( eSkillID );
#else // UPGRADE_SKILL_SYSTEM_2013
	const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( eSkillID, iMySkillLevel );
#endif // UPGRADE_SKILL_SYSTEM_2013
	
	if( NULL == pSkillTemplet )
		return false;


	const CX2SkillTree::SkillTreeTemplet* pSkillTreeTemplet = g_pData->GetSkillTree()->GetSkillTreeTemplet( (int)m_iUnitClass, eSkillID );
	if( NULL == pSkillTreeTemplet )
		return false;


#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
		CX2SkillTree::SKILL_ID eFollowingSkillID = static_cast<CX2SkillTree::SKILL_ID>( pSkillTreeTemplet->m_iFollowingSkill );
		if( true == DoIHaveThisSkill( eFollowingSkillID ) )
			return false;
#else // UPGRADE_SKILL_SYSTEM_2013
	for( UINT i=0; i<pSkillTreeTemplet->m_vecFollowingSkill.size(); i++ )
	{
		CX2SkillTree::SKILL_ID eFollowingSkillID = (CX2SkillTree::SKILL_ID) pSkillTreeTemplet->m_vecFollowingSkill[i];
		if( true == DoIHaveThisSkill( eFollowingSkillID ) )
			return false;
	}
#endif // UPGRADE_SKILL_SYSTEM_2013



	// 스킬 되돌리기를 하고 난 후에 후행 tier에 스킬을 못 찍게 되는 경우 스킬 리셋을 할 수 없다
	std::vector<int> vecCumulativeTierSPoint;
	CalcCumulativeUsedSPointOnEachTier( vecCumulativeTierSPoint );


	const int MAGIC_SKILL_POINT_PER_TIER = 5;
	for( UINT i=pSkillTreeTemplet->m_iTier+1; i<vecCumulativeTierSPoint.size(); i++ )
	{
		if( vecCumulativeTierSPoint[i-1]-1 < (int)i * MAGIC_SKILL_POINT_PER_TIER )
		{
			return false;
		}
	}

	return true;
}


bool CX2UserSkillTree::CanILearnThisSkill( CX2SkillTree::SKILL_ID eSkillID, bool bCheckSP /*= true*/ ) const
{
#ifdef GUILD_SKILL
//{{ oasis907 : 김상윤 //// 2009-11-17 //// 
	if(IsGuildSkill(eSkillID))
	{
		if( NULL == g_pData ||
			NULL == g_pData->GetMyUser() ||
			NULL == g_pData->GetMyUser()->GetSelectUnit() )
		{
			return false;
		}

		const CX2Unit::UnitData* pUnitData = &g_pData->GetMyUser()->GetSelectUnit()->GetUnitData();

		if(bCheckSP == true)
		{
			if( pUnitData->m_iGuildSPoint + pUnitData->m_iGuildCSPoint < 1 )
				return false;
		}

		if( true == IsMaxSkillLevel( eSkillID ) )
			return false;

		int iSkillLevelToLearn = GetSkillLevel( eSkillID ) + 1;

	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( eSkillID );
	#else // UPGRADE_SKILL_SYSTEM_2013
		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( eSkillID, iSkillLevelToLearn );
	#endif // UPGRADE_SKILL_SYSTEM_2013
		
		if( NULL == pSkillTemplet )
			return false;



		// 봉인해제되지 않은 스킬은 획득할 수 없다
		if( true == pSkillTemplet->m_bBornSealed )
		{
			if( false == IsSkillUnsealed( eSkillID ) )
			{
				return false;
			}
		}

		const CX2SkillTree::SkillTreeTemplet* pSkillTreeTemplet = g_pData->GetSkillTree()->GetGuildSkillTreeTemplet( (int)m_iGuildClass, eSkillID );
		if( NULL == pSkillTreeTemplet )
			return false;


		const int MAGIC_SKILL_POINT_PER_TIER = 5;
		int iUsedSPoint = 0;
		int iUsedCSPoint = 0;
		CalcUsedGuildSPointAndCSPoint( iUsedSPoint, iUsedCSPoint );
		if( iUsedSPoint + iUsedCSPoint < pSkillTreeTemplet->m_iTier * MAGIC_SKILL_POINT_PER_TIER )
		{
			return false;
		}



#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
		CX2SkillTree::SKILL_ID ePrecedingSkillID = static_cast<CX2SkillTree::SKILL_ID>( pSkillTreeTemplet->m_iPrecedingSkill );

		if( CX2SkillTree::SI_NONE != ePrecedingSkillID &&false == IsMaxSkillLevel( ePrecedingSkillID ) )
			return false;
#else // UPGRADE_SKILL_SYSTEM_2013
		for( UINT i=0; i<pSkillTreeTemplet->m_vecPrecedingSkill.size(); i++ )
		{
			CX2SkillTree::SKILL_ID ePrecedingSkillID = (CX2SkillTree::SKILL_ID) pSkillTreeTemplet->m_vecPrecedingSkill[i];
			if( false == IsMaxSkillLevel( ePrecedingSkillID ) )
				return false;
		}
#endif // UPGRADE_SKILL_SYSTEM_2013

		return true;

	}
//}} oasis907 : 김상윤 //// 2009-11-17 //// 
#endif GUILD_SKILL

	if( NULL == g_pData ||
		NULL == g_pData->GetMyUser() ||
		NULL == g_pData->GetMyUser()->GetSelectUnit() )
	{
		return false;
	}

	const CX2Unit::UnitData* pUnitData = &g_pData->GetMyUser()->GetSelectUnit()->GetUnitData();

	if(bCheckSP == true)
	{
		if( pUnitData->m_iSPoint + pUnitData->m_iCSPoint < 1 )
			return false;
	}
	
	if( true == IsMaxSkillLevel( eSkillID ) )
		return false;

	int iSkillLevelToLearn = GetSkillLevel( eSkillID ) + 1;

#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
	const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( eSkillID );
#else // UPGRADE_SKILL_SYSTEM_2013
	const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( eSkillID, iSkillLevelToLearn );
#endif // UPGRADE_SKILL_SYSTEM_2013
	
	if( NULL == pSkillTemplet )
		return false;


    
	// 봉인해제되지 않은 스킬은 획득할 수 없다
	if( true == pSkillTemplet->m_bBornSealed )
	{
		if( false == IsSkillUnsealed( eSkillID ) )
		{
			return false;
		}
	}


	const CX2SkillTree::SkillTreeTemplet* pSkillTreeTemplet = g_pData->GetSkillTree()->GetSkillTreeTemplet( (int)m_iUnitClass, eSkillID );
	if( NULL == pSkillTreeTemplet )
		return false;


	const int MAGIC_SKILL_POINT_PER_TIER = 5;
	int iUsedSPoint = 0;
	int iUsedCSPoint = 0;
	CalcUsedSPointAndCSPoint( iUsedSPoint, iUsedCSPoint );
	if( iUsedSPoint + iUsedCSPoint < pSkillTreeTemplet->m_iTier * MAGIC_SKILL_POINT_PER_TIER )
	{
		return false;
	}



#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
	CX2SkillTree::SKILL_ID ePrecedingSkillID = static_cast<CX2SkillTree::SKILL_ID>( pSkillTreeTemplet->m_iFollowingSkill );
	if( false == IsMaxSkillLevel( ePrecedingSkillID ) )
		return false;
#else // UPGRADE_SKILL_SYSTEM_2013
	for( UINT i=0; i<pSkillTreeTemplet->m_vecPrecedingSkill.size(); i++ )
	{
		CX2SkillTree::SKILL_ID ePrecedingSkillID = (CX2SkillTree::SKILL_ID) pSkillTreeTemplet->m_vecPrecedingSkill[i];
		if( false == IsMaxSkillLevel( ePrecedingSkillID ) )
			return false;
	}
#endif // UPGRADE_SKILL_SYSTEM_2013

	return true;
}


bool CX2UserSkillTree::CanIInitSkillTree() const
{
#ifdef SKILL_PAGE_SYSTEM //JHKang
	if ( true == m_vecSkillDataMap.empty() || m_usUsingPage >= m_vecSkillDataMap.size() )
		return false;

	for( SkillDataMap::const_iterator mit = m_vecSkillDataMap[m_usUsingPage].begin(); mit != m_vecSkillDataMap[m_usUsingPage].end(); ++mit )
#else //SKILL_PAGE_SYSTEM
	for( SkillDataMap::const_iterator mit = m_mapSkillAcquired.begin(); mit != m_mapSkillAcquired.end(); ++mit )
#endif //SKILL_PAGE_SYSTEM
	{
		CX2SkillTree::SKILL_ID eSkillID = mit->first;
		const UserSkillData& userSkillData = mit->second;
		if( false == g_pData->GetSkillTree()->IsUnitTypeDefaultSkill( eSkillID ) )
		{
			return true;
		}
		else if( GetSkillLevel( eSkillID ) > 1 )
		{
			return true;
		}
	}

	return false;

}
#ifdef GUILD_SKILL
//{{ oasis907 : 김상윤 //// 2009-11-12 //// 
bool CX2UserSkillTree::CanIInitGuildSkillTree() const
{
	for(SkillDataMap::const_iterator mit = m_mapGuildSkillAcquired.begin(); mit != m_mapGuildSkillAcquired.end(); ++mit)
	{
		CX2SkillTree::SKILL_ID eSkillID = mit->first;
		const UserSkillData& userSkillData = mit->second;
		if( false == g_pData->GetSkillTree()->IsUnitTypeDefaultSkill( eSkillID ) )
		{
			return true;
		}
		else if( GetSkillLevel( eSkillID ) > 1 )
		{
			return true;
		}
	}

	return false;

}
//}} oasis907 : 김상윤 //// 2009-11-12 //// 
#endif GUILD_SKILL


int CX2UserSkillTree::GetSkillEquippedSlotIndex( CX2SkillTree::SKILL_ID eSkillID, bool bSlotB ) const
{
	if( false == bSlotB )
	{
		for( int i=0; i<ARRAY_SIZE( m_aEquippedSkill ); ++i )
		{
			if( m_aEquippedSkill[i].m_eID == eSkillID )
				return i;
		}

	}
	else
	{
		for( int i=0; i<ARRAY_SIZE( m_aEquippedSkillSlotB ); ++i )
		{
			if( m_aEquippedSkillSlotB[i].m_eID == eSkillID )
				return i;
		}
	}

	return -1;

}

bool CX2UserSkillTree::GetEnabledSkillSlotB() const
{
	switch( m_eSkillSlotBExpirationState )
	{
	case SSBES_EXPIRED:
		{
			return false;
		} break;

	case SSBES_NOT_EXPIRED:
	case SSBES_PERMANENT:
		{
			return true;
		} break;
	}

	return false;
}



void CX2UserSkillTree::SetSkillCoolTimeLeft( CX2SkillTree::SKILL_ID eSkillID, float fCoolTimeLeft )
{
	CX2GageManager* pGageManager = CX2GageManager::GetInstance();

	for( int i=0; i<ARRAY_SIZE( m_aEquippedSkill ); ++i )
	{
		SkillSlotData& skillSlot = m_aEquippedSkill[ i ];
				
		if( eSkillID == skillSlot.m_eID )
		{
#ifdef FINALITY_SKILL_SYSTEM //JHKang
			const CX2SkillTree::SkillTemplet* pSkillTemplet = GetUserSkillTemplet( skillSlot.m_eID );
			
			if( pSkillTemplet != NULL && ( pSkillTemplet->m_eType == CX2SkillTree::ST_RELATIONSHIP_SKILL
				|| ( pSkillTemplet->m_eType == CX2SkillTree::ST_HYPER_ACTIVE_SKILL && skillSlot.m_eID != CX2SkillTree::SI_HA_EEP_LINK_OVERCHARGE_ILLUSION ) ) )
			{
				const int iSkillTempletLevel = max( 1, GetSkillLevel( pSkillTemplet->m_eID ) );
				const float fTime = pSkillTemplet->GetSkillCoolTimeValue( iSkillTempletLevel );

				skillSlot.m_fCoolTimeLeft = fTime;
				pGageManager->SetMySkillCoolTimeList( i, fTime );
			}
			else
			{
				skillSlot.m_fCoolTimeLeft = fCoolTimeLeft;
				pGageManager->SetMySkillCoolTimeList( i, fCoolTimeLeft );
			}
#else //FINALITY_SKILL_SYSTEM
			skillSlot.m_fCoolTimeLeft = fCoolTimeLeft;
			pGageManager->SetMySkillCoolTimeList( i, fCoolTimeLeft );
#endif //FINALITY_SKILL_SYSTEM

#ifdef SERV_BALANCE_FINALITY_SKILL_EVENT
			float fDecreaseRate = 1.0f;			
			if( NULL != pSkillTemplet && NULL != g_pData && NULL != g_pData->GetMyUser() && NULL != g_pData->GetMyUser()->GetSelectUnit() )
				fDecreaseRate =	g_pData->GetMyUser()->GetSelectUnit()->GetSkillCoolTimeDecreaseRate(eSkillID, pSkillTemplet->m_eType);
			
			if( 1.0f != fDecreaseRate )
			{
				float fCoolTimeLeftOld = skillSlot.m_fCoolTimeLeft;
				skillSlot.m_fCoolTimeLeft = fCoolTimeLeftOld * fDecreaseRate;
				pGageManager->SetMySkillCoolTimeList( i, fCoolTimeLeftOld * fDecreaseRate );
			}
#endif //SERV_BALANCE_FINALITY_SKILL_EVENT
		}
	}

	if( true == GetEnabledSkillSlotB() )
	{
		for( int i=0; i<ARRAY_SIZE( m_aEquippedSkillSlotB ); ++i )
		{
			SkillSlotData& skillSlot = m_aEquippedSkillSlotB[ i ];
			
			if( eSkillID == skillSlot.m_eID )
			{
#ifdef FINALITY_SKILL_SYSTEM //JHKang
				const CX2SkillTree::SkillTemplet* pSkillTemplet = GetUserSkillTemplet( skillSlot.m_eID );

				if( pSkillTemplet != NULL && ( pSkillTemplet->m_eType == CX2SkillTree::ST_RELATIONSHIP_SKILL
					|| ( pSkillTemplet->m_eType == CX2SkillTree::ST_HYPER_ACTIVE_SKILL && skillSlot.m_eID != CX2SkillTree::SI_HA_EEP_LINK_OVERCHARGE_ILLUSION ) ) )
				{
					const int iSkillTempletLevel = max( 1, GetSkillLevel( pSkillTemplet->m_eID ) );
					const float fTime = pSkillTemplet->GetSkillCoolTimeValue( iSkillTempletLevel );
					
					skillSlot.m_fCoolTimeLeft = fTime;
					pGageManager->SetMySkillCoolTimeList( i + 4, fTime );
				}
				else
				{
					skillSlot.m_fCoolTimeLeft = fCoolTimeLeft;
					pGageManager->SetMySkillCoolTimeList( i + 4, fCoolTimeLeft );
				}
#else //FINALITY_SKILL_SYSTEM
				skillSlot.m_fCoolTimeLeft = fCoolTimeLeft;
				pGageManager->SetMySkillCoolTimeList( i + 4, fCoolTimeLeft );
#endif //FINALITY_SKILL_SYSTEM

#ifdef SERV_BALANCE_FINALITY_SKILL_EVENT
				float fDecreaseRate = 1.0f;
				if( NULL != pSkillTemplet && NULL != g_pData->GetMyUser() && NULL != g_pData->GetMyUser()->GetSelectUnit() )
					fDecreaseRate =	g_pData->GetMyUser()->GetSelectUnit()->GetSkillCoolTimeDecreaseRate(eSkillID, pSkillTemplet->m_eType);

				if( 1.0f != fDecreaseRate )
				{
					float fCoolTimeLeftOld = skillSlot.m_fCoolTimeLeft;
					skillSlot.m_fCoolTimeLeft = fCoolTimeLeftOld * fDecreaseRate;
					pGageManager->SetMySkillCoolTimeList( i, fCoolTimeLeftOld * fDecreaseRate );
				}
#endif //SERV_BALANCE_FINALITY_SKILL_EVENT
			}
		}
	}
}

//{{ kimhc // 2010.11.2 // 엠프레스 - 충전 추진기
#ifdef	NEW_SKILL_2010_11

void CX2UserSkillTree::ResetLeftSkillCoolTimeAll()
{
	// 슬롯 a와 슬롯 b의 사이즈가 같다고 가정하였음
	const int iSizeOfIndex = ARRAY_SIZE( m_aEquippedSkill );
	CX2GageManager* pGageManager = CX2GageManager::GetInstance();

	for ( int iCurrentIndex = 0; iCurrentIndex < iSizeOfIndex; ++iCurrentIndex )
	{
#ifdef FINALITY_SKILL_SYSTEM //JHKang
	#ifdef FINALITY_SKILL_COOLTIMERESET_IN_TRAINING
		if( NULL != g_pMain && g_pMain->GetNowStateID() == CX2Main::XS_TRAINING_GAME )
		{
			m_aEquippedSkill[iCurrentIndex].m_fCoolTimeLeft = 0.0f;
			m_aEquippedSkillSlotB[iCurrentIndex].m_fCoolTimeLeft = 0.0f;

			pGageManager->SetMySkillCoolTimeList( iCurrentIndex, 0.0f );
			pGageManager->SetMySkillCoolTimeList( iCurrentIndex + 4, 0.0f );
		}
		else
	#endif //FINALITY_SKILL_COOLTIMERESET_IN_TRAINING
		{
			const CX2SkillTree::SkillTemplet* pSkillTempletA = GetUserSkillTemplet( m_aEquippedSkill[iCurrentIndex].m_eID );

			if ( pSkillTempletA != NULL && ( pSkillTempletA->m_eType != CX2SkillTree::ST_HYPER_ACTIVE_SKILL &&
				 pSkillTempletA->m_eType != CX2SkillTree::ST_RELATIONSHIP_SKILL ) )
			{
				m_aEquippedSkill[iCurrentIndex].m_fCoolTimeLeft = 0.0f;
				pGageManager->SetMySkillCoolTimeList( iCurrentIndex, 0.0f );
			}

			const CX2SkillTree::SkillTemplet* pSkillTempletB = GetUserSkillTemplet( m_aEquippedSkillSlotB[iCurrentIndex].m_eID );

			if ( pSkillTempletB != NULL && ( pSkillTempletB->m_eType != CX2SkillTree::ST_HYPER_ACTIVE_SKILL &&
				pSkillTempletB->m_eType != CX2SkillTree::ST_RELATIONSHIP_SKILL ) )
			{
				m_aEquippedSkillSlotB[iCurrentIndex].m_fCoolTimeLeft = 0.0f;
				pGageManager->SetMySkillCoolTimeList( iCurrentIndex + 4, 0.0f );
			}
		}
#else //FINALITY_SKILL_SYSTEM
		m_aEquippedSkill[iCurrentIndex].m_fCoolTimeLeft = 0.0f;
		m_aEquippedSkillSlotB[iCurrentIndex].m_fCoolTimeLeft = 0.0f;

		pGageManager->SetMySkillCoolTimeList( iCurrentIndex, 0.0f );
		pGageManager->SetMySkillCoolTimeList( iCurrentIndex + 4, 0.0f );
#endif //FINALITY_SKILL_SYSTEM
	}
}

#ifdef UPGRADE_SKILL_SYSTEM_2013 /// 김태환

#ifdef ADDED_BBT_ALL_EQUIP_SKILL_SET_OR_RESET_COOLTIME					// 김종훈 / 13-07-04 / 모든 착용 중인 스킬의 쿨타임 최대치로 설정되게 하는 BBT 추가
void CX2UserSkillTree::ResetLeftSkillCoolTimeBySkillType( const CX2SkillTree::SKILL_TYPE eSkillType, const float fCoolTime /*= 0.f*/, bool bForceSet /*= true*/, BUFF_CHANGE_TYPE eChangeType /*= BCT_RELATION_VALUE*/, const float fCoolTimeRate /* = 1.f */, const bool bIsRelativeAllSkill /* = false */ )

#else // ADDED_BBT_ALL_EQUIP_SKILL_SET_OR_RESET_COOLTIME
void CX2UserSkillTree::ResetLeftSkillCoolTimeBySkillType( const CX2SkillTree::SKILL_TYPE eSkillType, const float fCoolTime /*= 0.f*/, bool bForceSet /*= true*/, BUFF_CHANGE_TYPE eChangeType /*= BCT_RELATION_VALUE*/ )

#endif // ADDED_BBT_ALL_EQUIP_SKILL_SET_OR_RESET_COOLTIME					// 김종훈 / 13-07-04 / 모든 착용 중인 스킬의 쿨타임 최대치로 설정되게 하는 BBT 추가

#else // UPGRADE_SKILL_SYSTEM_2013
void CX2UserSkillTree::ResetLeftSkillCoolTimeBySkillType( const CX2SkillTree::SKILL_TYPE eSkillType )
#endif // UPGRADE_SKILL_SYSTEM_2013
{
	// 슬롯 a와 슬롯 b의 사이즈가 같다고 가정하였음
	const int iSizeOfIndex = ARRAY_SIZE( m_aEquippedSkill );
	CX2GageManager* pGageManager = CX2GageManager::GetInstance();

	for ( int iCurrentIndex = 0; iCurrentIndex < iSizeOfIndex; ++iCurrentIndex )
	{
		const CX2SkillTree::SkillTemplet* pSkillTempletSlotA = GetUserSkillTemplet( m_aEquippedSkill[iCurrentIndex].m_eID );

		if ( NULL != pSkillTempletSlotA && ( eSkillType == pSkillTempletSlotA->m_eType 
#ifdef ADDED_BBT_ALL_EQUIP_SKILL_SET_OR_RESET_COOLTIME					// 김종훈, 모든 착용 중인 스킬의 쿨타임 최대치로 설정되게 하는 BBT 추가
			|| bIsRelativeAllSkill == true								// 장착 가능한 스킬은 모두 관여하는 스킬 타입  
#endif // ADDED_BBT_ALL_EQUIP_SKILL_SET_OR_RESET_COOLTIME				// 김종훈, 모든 착용 중인 스킬의 쿨타임 최대치로 설정되게 하는 BBT 추가			
			) )
		{

#ifdef UPGRADE_SKILL_SYSTEM_2013 /// 김태환
			float fNowCoolTime = m_aEquippedSkill[iCurrentIndex].m_fCoolTimeLeft;		/// 해당 스킬의 현재 쿨타임

			switch ( eChangeType )
			{
			case BCT_FIX_VALUE:
				{
					/// 만약 쿨타임 강제 대입 설정을 했거나 현재 쿨타임이 변경할 쿨타임보다 훨씬 클때, 현재 쿨타임을 변경할 쿨타임으로 대입한다.
					if ( true == bForceSet || fNowCoolTime > fCoolTime )
					{
						m_aEquippedSkill[iCurrentIndex].m_fCoolTimeLeft = fCoolTime;
						pGageManager->SetMySkillCoolTimeList( iCurrentIndex, fCoolTime );
					}
				} break;

			case BCT_RELATION_VALUE:
				{
					fNowCoolTime += fCoolTime;
					fNowCoolTime = max( 0, fNowCoolTime );

					m_aEquippedSkill[iCurrentIndex].m_fCoolTimeLeft = fNowCoolTime;
					pGageManager->SetMySkillCoolTimeList( iCurrentIndex, fNowCoolTime );
				} break;
#ifdef ADDED_BBT_ALL_EQUIP_SKILL_SET_OR_RESET_COOLTIME					// 김종훈, 모든 착용 중인 스킬의 쿨타임 최대치로 설정되게 하는 BBT 추가
			case BCT_PERCENT:
				{										
					int iSkillLevel = GetSkillLevel ( pSkillTempletSlotA->m_eID );
					float fMaxCoolTime = pSkillTempletSlotA->GetSkillCoolTimeValue( iSkillLevel );
					fNowCoolTime = fMaxCoolTime * fCoolTimeRate;

					m_aEquippedSkill[iCurrentIndex].m_fCoolTimeLeft = fNowCoolTime;
					pGageManager->SetMySkillCoolTimeList( iCurrentIndex, fNowCoolTime);
				} break;
#endif // ADDED_BBT_ALL_EQUIP_SKILL_SET_OR_RESET_COOLTIME					// 김종훈, 모든 착용 중인 스킬의 쿨타임 최대치로 설정되게 하는 BBT 추가

			default:
				break;
			}
			
			
	#else // UPGRADE_SKILL_SYSTEM_2013
			m_aEquippedSkill[iCurrentIndex].m_fCoolTimeLeft = 0.0f;
			pGageManager->SetMySkillCoolTimeList( iCurrentIndex, 0.0f );
	#endif // UPGRADE_SKILL_SYSTEM_2013
			 
		}

#ifdef BALANCE_PATCH_20110303
		const CX2SkillTree::SkillTemplet* pSkillTempletSlotB = GetUserSkillTemplet( m_aEquippedSkillSlotB[iCurrentIndex].m_eID );
#else
		const CX2SkillTree::SkillTemplet* pSkillTempletSlotB = GetUserSkillTemplet( m_aEquippedSkill[iCurrentIndex].m_eID );
#endif

		if ( NULL != pSkillTempletSlotB && ( eSkillType == pSkillTempletSlotB->m_eType 
#ifdef ADDED_BBT_ALL_EQUIP_SKILL_SET_OR_RESET_COOLTIME					// 김종훈, 모든 착용 중인 스킬의 쿨타임 최대치로 설정되게 하는 BBT 추가
			|| bIsRelativeAllSkill == true								// 장착 가능한 스킬은 모두 관여하는 스킬 타입  
#endif // ADDED_BBT_ALL_EQUIP_SKILL_SET_OR_RESET_COOLTIME					// 김종훈, 모든 착용 중인 스킬의 쿨타임 최대치로 설정되게 하는 BBT 추가			

			) )
		{

	#ifdef UPGRADE_SKILL_SYSTEM_2013 /// 김태환
			float fNowCoolTime = m_aEquippedSkillSlotB[iCurrentIndex].m_fCoolTimeLeft;	/// 해당 스킬의 현재 쿨타임

			switch( eChangeType )
			{
			case BCT_FIX_VALUE:
				{
					/// 만약 쿨타임 강제 대입 설정을 했거나 현재 쿨타임이 변경할 쿨타임보다 훨씬 클때, 현재 쿨타임을 변경할 쿨타임으로 대입한다.
					if ( true == bForceSet || fNowCoolTime > fCoolTime )
					{
						m_aEquippedSkillSlotB[iCurrentIndex].m_fCoolTimeLeft = fCoolTime;
						pGageManager->SetMySkillCoolTimeList( iCurrentIndex + 4, fCoolTime );
					}
				} break;

			case BCT_RELATION_VALUE:
				{
					fNowCoolTime += fCoolTime;
					fNowCoolTime = max( 0, fNowCoolTime );

					m_aEquippedSkillSlotB[iCurrentIndex].m_fCoolTimeLeft = fNowCoolTime;
					pGageManager->SetMySkillCoolTimeList( iCurrentIndex + 4, fNowCoolTime );
				} break;


#ifdef ADDED_BBT_ALL_EQUIP_SKILL_SET_OR_RESET_COOLTIME					// 김종훈, 모든 착용 중인 스킬의 쿨타임 최대치로 설정되게 하는 BBT 추가
			case BCT_PERCENT:
				{										
					int iSkillLevel = GetSkillLevel ( pSkillTempletSlotB->m_eID );
					float fMaxCoolTime = pSkillTempletSlotB->GetSkillCoolTimeValue( iSkillLevel );
					fNowCoolTime = fMaxCoolTime * fCoolTimeRate;

					m_aEquippedSkillSlotB[iCurrentIndex].m_fCoolTimeLeft = fNowCoolTime;
					pGageManager->SetMySkillCoolTimeList( iCurrentIndex, fNowCoolTime);
				} break;
#endif // ADDED_BBT_ALL_EQUIP_SKILL_SET_OR_RESET_COOLTIME					// 김종훈, 모든 착용 중인 스킬의 쿨타임 최대치로 설정되게 하는 BBT 추가


			default:
				break;
			}
			
	#else // UPGRADE_SKILL_SYSTEM_2013
			m_aEquippedSkillSlotB[iCurrentIndex].m_fCoolTimeLeft = 0.0f;
			pGageManager->SetMySkillCoolTimeList( iCurrentIndex + 4, 0.0f );
	#endif // UPGRADE_SKILL_SYSTEM_2013

		}
	}
}

#endif	NEW_SKILL_2010_11
//}} kimhc // 2010.11.2 // 엠프레스 - 충전 추진기

#ifdef GUILD_SKILL
//{{ oasis907 : 김상윤 //// 2009-11-16 //// 
bool CX2UserSkillTree::IsGuildSkill( CX2SkillTree::SKILL_ID eSkillID ) const
{
#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
	const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet(eSkillID );
#else // UPGRADE_SKILL_SYSTEM_2013
	const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet(eSkillID, 1);
#endif // UPGRADE_SKILL_SYSTEM_2013
	
	if(pSkillTemplet == NULL)
	{
		return false;
	}
	if(pSkillTemplet->m_bGuildSkill)
	{
		return true;
	}
	return false;

}
//}} oasis907 : 김상윤 //// 2009-11-16 //// 
#endif GUILD_SKILL


#ifdef SERV_SKILL_NOTE
bool CX2UserSkillTree::GetEqipSkillMemo(int iSkillMemo) const
{
#if defined( _IN_HOUSE_ ) || defined( _OPEN_TEST_ ) || defined( _OPEN_TEST_2_ )
	if( g_pInstanceData != NULL )
	{
		if( g_pInstanceData->GetFrameScale() == true )
		{
			return true;
		}
	}		
#endif

	for(UINT i=0; i<m_vecSkillNote.size(); ++i)
	{
		if(m_vecSkillNote[i] == iSkillMemo)
		{
			return true;
		}
	}

	return false;
}
#endif

//슬롯 B에 장착 할 때 레벨값을 갱신하지 않는 오류 수정
int CX2UserSkillTree::UpdateEquippedSkillLevelFromAcqureidMap( const CX2SkillTree::SKILL_ID eSkillID, const int iSlotIndex_, bool bSlotB )
{
#ifdef SKILL_PAGE_SYSTEM //JHKang
	if ( m_usUsingPage < m_vecSkillDataMap.size() && !m_vecSkillDataMap[m_usUsingPage].empty() )
#else //SKILL_PAGE_SYSTEM
	if ( !m_mapSkillAcquired.empty() )
#endif //SKILL_PAGE_SYSTEM
	{
	#ifdef SKILL_PAGE_SYSTEM //JHKang
		SkillDataMap::const_iterator cmItr = m_vecSkillDataMap[m_usUsingPage].find( eSkillID );
		if ( m_vecSkillDataMap[m_usUsingPage].end() != cmItr )
	#else //SKILL_PAGE_SYSTEM
		SkillDataMap::const_iterator cmItr = m_mapSkillAcquired.find( eSkillID );
		if ( m_mapSkillAcquired.end() != cmItr )
	#endif //SKILL_PAGE_SYSTEM
		{
			if ( iSlotIndex_ >= 0 && iSlotIndex_ < ARRAY_SIZE( m_aEquippedSkill ) )
			{
				if( true == bSlotB )
				{
					m_aEquippedSkillSlotB[iSlotIndex_].m_iSkillLevel = cmItr->second.m_iSkillLevel;
					return m_aEquippedSkillSlotB[iSlotIndex_].m_iSkillLevel;
				}
				else
				{
					m_aEquippedSkill[iSlotIndex_].m_iSkillLevel = cmItr->second.m_iSkillLevel;
					return m_aEquippedSkill[iSlotIndex_].m_iSkillLevel;
				}
			}
		}
	}

	return 0;
}

void CX2UserSkillTree::InitSkillCoolTimeFromGageManager( const vector<float>& vecSkillCoolTime_ )
{
	// 슬롯 a와 슬롯 b의 사이즈가 같다고 가정하였음
	const int iSizeOfIndex = ARRAY_SIZE( m_aEquippedSkill );
	
	for ( UINT uiSlotIndex = 0; uiSlotIndex < vecSkillCoolTime_.size(); ++uiSlotIndex )
	{
		if ( iSizeOfIndex > uiSlotIndex )
			m_aEquippedSkill[uiSlotIndex].m_fCoolTimeLeft = vecSkillCoolTime_[uiSlotIndex];
		else
			m_aEquippedSkillSlotB[uiSlotIndex - iSizeOfIndex].m_fCoolTimeLeft = vecSkillCoolTime_[uiSlotIndex];
	}	
}

/*static*/ void CX2UserSkillTree::GetSlotIndexAndSlotB( IN const int iSkillSlotId_, OUT int& iSlotIndex_, OUT bool& bSlotB_ )
{
	// SlotB의 첫번째 슬롯 ID int형으로 변경
	const int iSlotBStartId = static_cast<int>( CX2UserSkillTree::SKILL_SLOT_B1 );

	// iSkillSlotId_가 B슬롯인지 아닌지..
	bSlotB_ = ( iSkillSlotId_ >= iSlotBStartId ? true : false );

	// B슬롯이면 B슬롯의 시작 슬롯을 현재 인자로 넘겨진 아이디에서 빼서 (0~3) 사이의 슬롯 Index를 구함
	iSlotIndex_ = ( bSlotB_ == true ? iSkillSlotId_ - iSlotBStartId : iSkillSlotId_ );
}




#ifdef NEXON_QA_CHEAT_REQ
void CX2UserSkillTree::AllSkillNoCoolTime()
{
	CX2GageManager* pGageManager = CX2GageManager::GetInstance();

	if( NULL == pGageManager )
		return;

	for( int i=0; i<ARRAY_SIZE( m_aEquippedSkill ); ++i )
	{
		SkillSlotData& skillData = m_aEquippedSkill[i];
		skillData.m_fCoolTimeLeft = 0.f;

		pGageManager->SetMySkillCoolTimeList( i, skillData.m_fCoolTimeLeft );
	}

	for( int i=0; i<ARRAY_SIZE( m_aEquippedSkillSlotB ); i++ )
	{
		SkillSlotData& skillData = m_aEquippedSkillSlotB[i];
		skillData.m_fCoolTimeLeft = 0.f;

		pGageManager->SetMySkillCoolTimeList( i + EQUIPPED_SKILL_SLOT_COUNT, skillData.m_fCoolTimeLeft );
	}	
}
#endif //NEXON_QA_CHEAT_REQ

#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경

/** @function	: SetDefaultSkill
	@brief		: 각 전직별 기본 스킬 설정 함수 ( SkillData.lua )
*/
#ifdef SKILL_PAGE_SYSTEM //JHKang
void CX2UserSkillTree::SetDefaultSkill( USHORT usPageIndex_, bool bCanPassIfChecked_ /* = false */ )
#else //SKILL_PAGE_SYSTEM
void CX2UserSkillTree::SetDefaultSkill( bool bCanPassIfChecked_ /* = false */ )
#endif //SKILL_PAGE_SYSTEM
{
	/// 각 전직별 기본 스킬 설정
	if ( NULL == g_pData  ||
		 NULL == g_pData->GetSkillTree() ||
		 NULL == g_pData->GetMyUser()->GetSelectUnit() )
	{
		return;
	}

	const int iUnitClass = static_cast<int>( g_pData->GetMyUser()->GetSelectUnit()->GetClass() );	/// 해당 유닛의 클래스
	const std::map<int, std::vector<int>>					mapDefaultSkill		= g_pData->GetSkillTree()->GetMapDefaultSkill();	/// 기본 스킬 정보

	const std::map<int, std::vector<int>>::const_iterator	it					= mapDefaultSkill.find( iUnitClass );


	if ( it != mapDefaultSkill.end() )	/// 해당 클래스의 기본 스킬 정보가 있다면, 설정
	{
		std::vector<int> vecDefaultSkillID = it->second;
	
		BOOST_FOREACH( int iValue, vecDefaultSkillID )
		{
			if ( true == bCanPassIfChecked_ &&	
				 0 < g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( static_cast<CX2SkillTree::SKILL_ID> ( iValue ) ) )
			{
				continue;
			}
#ifdef SKILL_PAGE_SYSTEM //JHKang
			SetSkillLevelAndCSP( static_cast<CX2SkillTree::SKILL_ID>( iValue ), 1, 0, usPageIndex_ );
#else //SKILL_PAGE_SYSTEM
			SetSkillLevelAndCSP( static_cast<CX2SkillTree::SKILL_ID>( iValue ), 1, 0 );
#endif //SKILL_PAGE_SYSTEM
		}
	}
}

/** @function	GetIncreaseSkillLevel
	@brief		스킬 레벨 증가 효과치 얻기
	@param[in]	스킬 ID
	@return		int : 스킬 레벨
*/
int CX2UserSkillTree::GetIncreaseSkillLevel( CX2SkillTree::SKILL_ID eSkillID ) const
{
	KTDXPROFILE();

#ifdef GUILD_SKILL
	if(IsGuildSkill(eSkillID))
	{
		SkillDataMap::const_iterator mit = m_mapGuildSkillAcquired.find( eSkillID );
		if( mit != m_mapGuildSkillAcquired.end() )
		{
			const UserSkillData& userSkillData = mit->second;
			return userSkillData.GetIncreaseSkillLevel();
		}
		return 0;
	}
#endif GUILD_SKILL

#ifdef SKILL_PAGE_SYSTEM //JHKang
	if ( true == m_vecSkillDataMap.empty() || m_usUsingPage >= m_vecSkillDataMap.size() )
		return 0;

	SkillDataMap::const_iterator mit = m_vecSkillDataMap[m_usUsingPage].find( eSkillID );
	if( mit != m_vecSkillDataMap[m_usUsingPage].end() )
#else //SKILL_PAGE_SYSTEM
	SkillDataMap::const_iterator mit = m_mapSkillAcquired.find( eSkillID );
	if( mit != m_mapSkillAcquired.end() )
#endif //SKILL_PAGE_SYSTEM
	{
		const UserSkillData& userSkillData = mit->second;
		return userSkillData.GetIncreaseSkillLevel();
	}
	return 0;
}

#endif // UPGRADE_SKILL_SYSTEM_2013

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
/// 스킬 슬롯 ID에 설정 되어있는 스킬 아이디 반환
const CX2SkillTree::SKILL_ID CX2UserSkillTree::GetSkillIDByEquipSlotID( IN const int iStateID_ ) const
{
	int  iSkillSlotIndex	= -1;
	bool bSlotB				= false;

	switch ( CX2GUUser::USER_STATE_ID( iStateID_ ) )
	{
	case CX2GUUser::USI_SPECIAL_ATTACK_1:
	case CX2GUUser::USI_SPECIAL_ATTACK_HYPER_1:
		{
			iSkillSlotIndex = 0;
		} break;
	case CX2GUUser::USI_SPECIAL_ATTACK_2:
	case CX2GUUser::USI_SPECIAL_ATTACK_HYPER_2:
		{
			iSkillSlotIndex = 1;
		} break;
	case CX2GUUser::USI_SPECIAL_ATTACK_3:
	case CX2GUUser::USI_SPECIAL_ATTACK_HYPER_3:
		{
			iSkillSlotIndex = 2;
		} break;
	case CX2GUUser::USI_SPECIAL_ATTACK_4:
	case CX2GUUser::USI_SPECIAL_ATTACK_HYPER_4:
		{
			iSkillSlotIndex = 3;
		} break;

	case CX2GUUser::USI_SPECIAL_ATTACK_SLOTB_1:
	case CX2GUUser::USI_SPECIAL_ATTACK_SLOTB_HYPER_1:
		{
			iSkillSlotIndex = 0;
			bSlotB = true;
		} break;
	case CX2GUUser::USI_SPECIAL_ATTACK_SLOTB_2:
	case CX2GUUser::USI_SPECIAL_ATTACK_SLOTB_HYPER_2:
		{
			iSkillSlotIndex = 1;
			bSlotB = true;
		} break;
	case CX2GUUser::USI_SPECIAL_ATTACK_SLOTB_3:
	case CX2GUUser::USI_SPECIAL_ATTACK_SLOTB_HYPER_3:
		{
			iSkillSlotIndex = 2;
			bSlotB = true;
		} break;
	case CX2GUUser::USI_SPECIAL_ATTACK_SLOTB_4:
	case CX2GUUser::USI_SPECIAL_ATTACK_SLOTB_HYPER_4:
		{
			iSkillSlotIndex = 3;
			bSlotB = true;
		} break;
	}

	const SkillSlotData* pSkillSlotData = GetSkillSlot( iSkillSlotIndex, bSlotB );

	if ( NULL != pSkillSlotData )
		return pSkillSlotData->m_eID;

	return CX2SkillTree::SI_NONE;
}
#endif // SERV_ARA_CHANGE_CLASS_SECOND

#ifdef SERV_9TH_NEW_CHARACTER // 김태환
/** @function : GetIncreaseGainSoulRateByHit() 
	@brief : '타격 시' 증가 하는 '각성충전속도'의 증가 배율
			 -> 현재는 길드 스킬에 의해서만 증가 됨
*/
const float CX2UserSkillTree::GetIncreaseGainSoulRateByHit() const
{
	float fIncreaseSoulRate = 1.f;

#ifdef	GUILD_SKILL
	/// 분노 해방
	const CX2SkillTree::SkillTemplet* pSkillTemplet = 
		GetUserSkillTemplet( CX2SkillTree::SI_GP_COMMON_LIBERATION_OF_ANGER, g_pData->GetSelectUnitMemberShipGrade() );

	if( NULL != pSkillTemplet )
	{
		// 오현빈
		//TODO : 길드 스킬 레벨을 Basic Level만 받아 오고 있는데, 소켓 효과도 적용 받을 수 있도록 해야 함.
		//     : 현재 모든 길드 스킬이 true로 지정되어 있어서 true 유지 함
		const int iSkillTempletLevel = max( 1, GetSkillLevel( pSkillTemplet->m_eID, true ) );	/// 스킬 레벨
		float fRate = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_SOUL_GAIN_ON_HIT_REL, iSkillTempletLevel );
		fIncreaseSoulRate += CalculateIncreasingRate( fRate );
	}
#endif // GUILD_SKILL

	return fIncreaseSoulRate;
}
/** @function : GetIncreaseGainSoulRateByDamage() 
	@brief : '피격 시' 증가 하는 '각성충전속도'의 증가 배율
			 -> 현재는 길드 스킬에 의해서만 증가 됨
*/
const float CX2UserSkillTree::GetIncreaseGainSoulRateByDamage() const
{
	float fIncreaseSoulRate = 1.f;

#ifdef	GUILD_SKILL
	/// 격노
	const CX2SkillTree::SkillTemplet* pSkillTemplet = 
		GetUserSkillTemplet( CX2SkillTree::SI_GP_COMMON_VIOLENT, g_pData->GetSelectUnitMemberShipGrade() );
	if( NULL != pSkillTemplet )
	{
		// 오현빈
		//TODO : 길드 스킬 레벨을 Basic Level만 받아 오고 있는데, 소켓 효과도 적용 받을 수 있도록 해야 함.
		//     : 현재 모든 길드 스킬이 true로 지정되어 있어서 true 유지 함
		const int iSkillTempletLevel = max( 1, GetSkillLevel( pSkillTemplet->m_eID, true) );	/// 스킬 레벨
		float fRate = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_SOUL_GAIN_GET_HIT_REL, iSkillTempletLevel );
		fIncreaseSoulRate += CalculateIncreasingRate( fRate );					
	}
#endif // GUILD_SKILL

	return fIncreaseSoulRate;
}
#endif //SERV_9TH_NEW_CHARACTER