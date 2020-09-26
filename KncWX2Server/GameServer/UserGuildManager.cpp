#include "GSUser.h"
#include ".\userguildmanager.h"
#include "Enum\Enum.h"
#include "X2Data\XSLGuildManager.h"
#include "NetError.h"

#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013 // 적용날짜: 2013-06-27

KUserGuildManager::KUserGuildManager(void)
{
	Clear();
}

KUserGuildManager::~KUserGuildManager(void)
{
}

void KUserGuildManager::Clear()
{
	m_iGuildUID			 = 0;
	m_wstrGuildName		 = L"";
	m_ucMemberShipGrade	 = SEnum::GUG_INVALID;

	//{{ 2009. 10. 27  최육사	길드레벨
	m_iHonorPoint		 = 0;

	//{{ 2009. 11. 24  최육사	길드스킬
	m_mapSkillTree.clear();
	m_iMaxGuildCSPoint	 = 0;

	//{{ 2012. 02. 22	박세훈	길드 이름 변경권
	bIsGuildNameChanging = false;

}

void KUserGuildManager::SetGuildInfo( IN const KUserGuildInfo& kInfo )
{
	if( kInfo.m_iGuildUID <= 0 )
		return;

	m_iGuildUID			 = kInfo.m_iGuildUID;
	m_wstrGuildName		 = kInfo.m_wstrGuildName;
	m_ucMemberShipGrade	 = kInfo.m_ucMemberShipGrade;
	m_iHonorPoint		 = kInfo.m_iHonorPoint;
}

void KUserGuildManager::SetGuildInfo( IN int iGuildUID, IN const std::wstring& wstrGuildName, IN u_char ucMemberShipGrade, IN int iHonorPoint /*= 0*/ )
{
	KUserGuildInfo kInfo;
	kInfo.m_iGuildUID		  = iGuildUID;
	kInfo.m_wstrGuildName	  = wstrGuildName;
	kInfo.m_ucMemberShipGrade = ucMemberShipGrade;
	kInfo.m_iHonorPoint		  = iHonorPoint;
	SetGuildInfo( kInfo );
}

void KUserGuildManager::SetMemberShipGrade( IN u_char ucMemberShipGrade )
{
	if( SEnum::GUG_MASTER > ucMemberShipGrade  ||
		SEnum::GUG_NEW_USER < ucMemberShipGrade )
	{
		START_LOG( cerr, L"길드 멤버 등급값이 이상합니다. 절대 발생하면 안되는 에러!" )
			<< BUILD_LOGc( ucMemberShipGrade )
			<< END_LOG;
	}

	m_ucMemberShipGrade = ucMemberShipGrade;
}

void KUserGuildManager::OnDecreaseSpirit( IN KGSUserPtr spUser, IN int iDecreaseSpirit )
{
	if( spUser == NULL )
		return;

	if( spUser->GetGuildUID() <= 0 )
	{
		START_LOG( cerr, L"길드원이 아닌데 이 함수를 호출했다?" )
			<< BUILD_LOG( spUser->GetCharUID() )
			<< END_LOG;
		return;
	}

	if( iDecreaseSpirit < 0 )
	{
		START_LOG( cerr, L"소모 근성도값이 마이너스값이 들어오면 안된다. 잘못 계산한듯." )
			<< BUILD_LOG( iDecreaseSpirit )
			<< END_LOG;
		return;
	}

	if( iDecreaseSpirit == 0 )
		return;

	// 명예 포인트 얻기
	const int iIncreaseHonorPoint = SiCXSLGuildManager()->GetHonorPoint_Dungeon( spUser->GetLevel(), GetMemberShipGrade(), iDecreaseSpirit, spUser->IsPcBang() );
#ifdef SERV_GUILD_BONUSRATE_POINT
	IF_EVENT_ENABLED(CEI_GUILD_BONUSRATE_POINT)
	{
		const int iTemp = iIncreaseHonorPoint * 2;
		m_iHonorPoint += iTemp;
	}
	ELSE
	{
		// 명예 포인트를 채웁니다.
		m_iHonorPoint += iIncreaseHonorPoint;
	}
#else
	// 명예 포인트를 채웁니다.
	m_iHonorPoint += iIncreaseHonorPoint;
#endif SERV_GUILD_BONUSRATE_POINT

	// 명예 포인트가 가득찼는지 확인
	CheckHonorPoint( spUser );
}

//{{ 2009. 11. 16  최육사	대전길드포인트
void KUserGuildManager::OnUpdatePvpUnitInfo( IN KGSUserPtr spUser, IN int iPvpMemberCount )
{
	if( spUser == NULL )
		return;

	if( spUser->GetGuildUID() <= 0 )
	{
		START_LOG( cerr, L"길드원이 아닌데 이 함수를 호출했다?" )
			<< BUILD_LOG( spUser->GetCharUID() )
			<< END_LOG;
		return;
	}

	if( iPvpMemberCount == 0 )
		return;

	// 명예 포인트 얻기
	const int iIncreaseHonorPoint = SiCXSLGuildManager()->GetHonorPoint_Pvp( spUser->GetLevel(), GetMemberShipGrade(), iPvpMemberCount, spUser->IsPcBang() );

#ifdef SERV_GUILD_BONUSRATE_POINT
	IF_EVENT_ENABLED(CEI_GUILD_BONUSRATE_POINT)
	{
		const int iTemp = iIncreaseHonorPoint * 2;
		m_iHonorPoint += iTemp;
	}
	ELSE
	{
		// 명예 포인트를 채웁니다.
		m_iHonorPoint += iIncreaseHonorPoint;
	}
#else
	// 명예 포인트를 채웁니다.
	m_iHonorPoint += iIncreaseHonorPoint;
#endif SERV_GUILD_BONUSRATE_POINT

	// 명예 포인트가 가득찼는지 확인
	CheckHonorPoint( spUser );
}

void KUserGuildManager::CheckHonorPoint( IN KGSUserPtr spUser )
{
	if( spUser == NULL )
		return;

	// 명예 포인트 최대치인지 확인
	if( GetHonorPoint() < HONOR_POINT_MAX )
		return;

	m_iHonorPoint -= HONOR_POINT_MAX; // 최대치 만큼 감소 시킵니다. 초과치 만큼 남은량으로 계산됨

	// 길드 포인트 DB에 업데이트하러 가자!
	KDBE_UPDATE_GUILD_EXP_REQ kPacketToDB;
	kPacketToDB.m_iUnitUID = spUser->GetCharUID();
	kPacketToDB.m_iGuildUID = GetGuildUID();
	kPacketToDB.m_iGuildEXP = 1; // 1포인트 상승
	kPacketToDB.m_iGuildLimitEXP = SiCXSLGuildManager()->GetGuildLimitEXP();
	kPacketToDB.m_iHonorPoint = GetHonorPoint();
	spUser->SendToGameDB( DBE_UPDATE_GUILD_EXP_REQ, kPacketToDB );
}
//}}

bool KUserGuildManager::CheckBuyGuildItem( IN KGSUserPtr spUser, IN const std::map< int, int >& mapBuyItemList )
{
	SET_ERROR( NET_OK );

	if( spUser == NULL )
	{
		START_LOG( cerr, L"유저 객체 포인터가 이상합니다." )
			<< END_LOG;
		return false;
	}

	// 운영자면 바로 통과
	if( spUser->GetAuthLevel() >= SEnum::UAL_GM )
		return true;

	// 길드 창단서 검사
	if( mapBuyItemList.find( CXSLItem::SI_GUILD_FOUNDATION ) != mapBuyItemList.end() )
	{
		// 길드 멤버인지 검사
		if( spUser->GetGuildUID() > 0 )
		{
			START_LOG( cerr, L"길드원이면 길드 창단서를 구매할 수 없다!" )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetGuildUID() )
				<< END_LOG;

			SET_ERROR( ERR_GUILD_37 );
			return false;
		}

		// 길드 퀘스트를 클리어 했는지 검사
		if( spUser->IsCompleteQuest( GUILD_QUEST_ID ) == false )
		{
			SET_ERROR( ERR_GUILD_44 );
			return false;
		}

		// 이미 길드 창단서를 구매했는지 검사
		if( spUser->IsExistInventoryItemWithBank( CXSLItem::SI_GUILD_FOUNDATION ) == true )
		{
			SET_ERROR( ERR_GUILD_38 );
			return false;
		}
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
//{{ 2009. 11. 24  최육사	길드스킬

void KUserGuildManager::SetGuildSkill( IN const KGuildSkillInfo& kInfo )
{
	std::vector< KGuildSkillData >::const_iterator vit;
	for( vit = kInfo.m_vecGuildSkill.begin(); vit != kInfo.m_vecGuildSkill.end(); ++vit )
	{
		m_mapSkillTree.insert( std::make_pair( vit->m_iSkillID, *vit ) );
	}

	// 사용중인 기간제 스킬 포인트
	SetMaxGuildCSPoint( kInfo.m_iMaxGuildCSPoint );
}

void KUserGuildManager::UpdateGuildSkill( IN const KEGS_GET_GUILD_SKILL_NOT& kInfo )
{
	GuildSkillDataMap::iterator mit = m_mapSkillTree.find( kInfo.m_iGuildSkillID );
	if( mit == m_mapSkillTree.end() )
	{
		// 새로운 스킬을 배운다면..
		KGuildSkillData kGuildSkill;
		kGuildSkill.m_iSkillID		= kInfo.m_iGuildSkillID;
		kGuildSkill.m_cSkillLevel	= kInfo.m_iGuildSkillLv;
		kGuildSkill.m_cSkillCSPoint = kInfo.m_iGuildSkillCSPoint;
		m_mapSkillTree.insert( std::make_pair( kInfo.m_iGuildSkillID, kGuildSkill ) );
	}
	else
	{
		// 길드 스킬 레벨이 변경된다면
		mit->second.m_cSkillLevel = static_cast<u_char>(kInfo.m_iGuildSkillLv);
		mit->second.m_cSkillCSPoint = static_cast<u_char>(kInfo.m_iGuildSkillCSPoint);
	}
}

bool KUserGuildManager::ResetGuildSkill( IN const KEGS_RESET_GUILD_SKILL_NOT& kInfo )
{
	GuildSkillDataMap::iterator mit = m_mapSkillTree.find( kInfo.m_iDelGuildSkillID );
	if( mit == m_mapSkillTree.end() )
		return false;

	// 길드 스킬 레벨 감소
	--mit->second.m_cSkillLevel;

	// 길드 스킬 레벨이 0이 된다면..
	if( mit->second.m_cSkillLevel <= 0 )
	{
		m_mapSkillTree.erase( mit );
	}

	return true;
}

void KUserGuildManager::ExpireGuildSkill( IN const KEGS_EXPIRE_GUILD_CASH_SKILL_POINT_NOT& kInfo )
{
	std::vector< KGuildSkillData >::const_iterator vit;
	for( vit = kInfo.m_vecGuildSkillData.begin(); vit != kInfo.m_vecGuildSkillData.begin(); ++vit )
	{
		GuildSkillDataMap::iterator mit = m_mapSkillTree.find( vit->m_iSkillID );
		if( mit != m_mapSkillTree.end() )
		{
			// 길드 스킬 정보 갱신
			mit->second.m_cSkillLevel = vit->m_cSkillLevel;
			mit->second.m_cSkillCSPoint = vit->m_cSkillCSPoint;
		}
	}

	// 기간제 스킬 포인트 초기화
	SetMaxGuildCSPoint( 0 );
}

void KUserGuildManager::InitGuildSkill()
{
	// 길드 스킬트리 초기화
	m_mapSkillTree.clear();    
}

void KUserGuildManager::GetGuildPassiveSkillData( OUT std::vector< KSkillData >& vecSkillData )
{
	vecSkillData.clear();

	// 신입 길드원이면 길드 스킬을 적용시키지 않는다.
	if( GetMemberShipGrade() == SEnum::GUG_NEW_USER )
		return;

	GuildSkillDataMap::iterator mit;
	for( mit = m_mapSkillTree.begin(); mit != m_mapSkillTree.end(); ++mit )
	{
		KSkillData kSkillData;
		kSkillData.m_iSkillID		= mit->first;
		kSkillData.m_cSkillLevel	= mit->second.m_cSkillLevel;
		vecSkillData.push_back( kSkillData );
	}
}

void KUserGuildManager::GetGuildSkillStat( KStat& kStat )
{
	kStat.Init();

	// 신입 길드원이면 스탯 적용 시키지 않는다.
	if( GetMemberShipGrade() == SEnum::GUG_NEW_USER )
		return;

	GuildSkillDataMap::const_iterator mit;
	for( mit = m_mapSkillTree.begin(); mit != m_mapSkillTree.end(); ++mit )
	{
		const KGuildSkillData& kGuildSkillData = mit->second;
		if( kGuildSkillData.m_cSkillLevel <= 0 )
			continue;

		const CXSLSkillTree::SkillTemplet* pSkillTemplet = SiCXSLSkillTree()->GetSkillTemplet( mit->first );
		if( pSkillTemplet == NULL )
		{
			START_LOG( cerr, L"스킬 템플릿 포인트 이상.!" )
				<< BUILD_LOG( mit->first )
				<< BUILD_LOGc( kGuildSkillData.m_cSkillLevel )
				<< END_LOG;
			continue;
		}

		if( kGuildSkillData.m_cSkillLevel > CXSLSkillTree::SkillTemplet::SLV_MAX_LEVEL )
			continue;

		switch( pSkillTemplet->m_eType )
		{
		case CXSLSkillTree::ST_GUILD_PASSIVE_PHYSIC_ATTACK_MAGIC_DEFENCE:
		case CXSLSkillTree::ST_GUILD_PASSIVE_MAGIC_ATTCK_PHYSIC_DEFENCE:
			{
				kStat.m_iBaseHP			+= (int)pSkillTemplet->m_vecStat[kGuildSkillData.m_cSkillLevel-1].m_fBaseHP;
				kStat.m_iAtkPhysic		+= (int)pSkillTemplet->m_vecStat[kGuildSkillData.m_cSkillLevel-1].m_fAtkPhysic;
				kStat.m_iAtkMagic		+= (int)pSkillTemplet->m_vecStat[kGuildSkillData.m_cSkillLevel-1].m_fAtkMagic;
				kStat.m_iDefPhysic		+= (int)pSkillTemplet->m_vecStat[kGuildSkillData.m_cSkillLevel-1].m_fDefPhysic;
				kStat.m_iDefMagic		+= (int)pSkillTemplet->m_vecStat[kGuildSkillData.m_cSkillLevel-1].m_fDefMagic;
			} break;
		}
	}
}

void KUserGuildManager::GetGuildSkillBonusRate( OUT float& fAddGuildBonusExpRate, IN OUT float& fAddSocketOptEDRate )
{
	fAddGuildBonusExpRate = 0.0f;

	// 신입 길드원이면 길드 스킬을 적용시키지 않는다.
	if( GetMemberShipGrade() == SEnum::GUG_NEW_USER )
		return;

	// 동료의 응원
	GuildSkillDataMap::const_iterator mit = m_mapSkillTree.find( CXSLSkillTree::SI_GP_COMMON_CHEERING_OF_TEAM );
	if( mit != m_mapSkillTree.end() )
	{
		if( mit->second.m_cSkillLevel > 0 )
		{
			const CXSLSkillTree::SkillTemplet* pSkillTemplet = SiCXSLSkillTree()->GetSkillTemplet( mit->first );
			if( pSkillTemplet != NULL )
			{
				// 길드원끼리 플레이시 경험치 증가되는 길드 스킬.
				fAddGuildBonusExpRate += pSkillTemplet->GetSkillAbilityValue( CXSLSkillTree::SA_EXP_GAIN_REL, (int)mit->second.m_cSkillLevel );
			}
			else
			{
				START_LOG( cerr, L"스킬 템플릿 포인트 이상.!" )
					<< BUILD_LOG( mit->first )
					<< BUILD_LOGc( mit->second.m_cSkillLevel )
					<< END_LOG;
			}
		}
	}

	// 마이더스의 손
	mit = m_mapSkillTree.find( CXSLSkillTree::SI_GP_COMMON_MIDAS );
	if( mit != m_mapSkillTree.end() )
	{
		if( mit->second.m_cSkillLevel > 0 )
		{
			const CXSLSkillTree::SkillTemplet* pSkillTemplet = SiCXSLSkillTree()->GetSkillTemplet( mit->first );
			if( pSkillTemplet != NULL )
			{
				// 길드원끼리 플레이시 경험치 증가되는 길드 스킬.
				fAddSocketOptEDRate += pSkillTemplet->GetSkillAbilityValue( CXSLSkillTree::SA_ED_GAIN_REL, (int)mit->second.m_cSkillLevel );
			}
			else
			{
				START_LOG( cerr, L"스킬 템플릿 포인트 이상.!" )
					<< BUILD_LOG( mit->first )
					<< BUILD_LOGc( mit->second.m_cSkillLevel )
					<< END_LOG;
			}
		}
	}
}


//{{ 2012. 02. 22	박세훈	길드 이름 변경권
bool KUserGuildManager::IsGuildMaster( void )
{
	return GetMemberShipGrade() == SEnum::GUG_MASTER;
}

void KUserGuildManager::SetGuildNameChanging( void )
{
	bIsGuildNameChanging = true;
}
void KUserGuildManager::ResetGuildNameChanging( void )
{
	bIsGuildNameChanging = false;
}
bool KUserGuildManager::CheckGuildNameChanging( void )
{
	return bIsGuildNameChanging;
}

//////////////////////////////////////////////////////////////////////////

#else	// SERV_UPGRADE_SKILL_SYSTEM_2013
/*

#ifdef GUILD_TEST

KUserGuildManager::KUserGuildManager(void)
{
Clear();
}

KUserGuildManager::~KUserGuildManager(void)
{
}

void KUserGuildManager::Clear()
{
m_iGuildUID			 = 0;
m_wstrGuildName		 = L"";
m_ucMemberShipGrade	 = SEnum::GUG_INVALID;
//{{ 2009. 10. 27  최육사	길드레벨
m_iHonorPoint		 = 0;
//}}
//{{ 2009. 11. 24  최육사	길드스킬
#ifdef GUILD_SKILL_TEST	
m_mapSkillTree.clear();
m_iMaxGuildCSPoint	 = 0;
#endif GUILD_SKILL_TEST
//}}

//{{ 2012. 02. 22	박세훈	길드 이름 변경권
#ifdef SERV_GUILD_CHANGE_NAME
bIsGuildNameChanging = false;
#endif SERV_GUILD_CHANGE_NAME
//}}
}

void KUserGuildManager::SetGuildInfo( IN const KUserGuildInfo& kInfo )
{
if( kInfo.m_iGuildUID <= 0 )
return;

m_iGuildUID			 = kInfo.m_iGuildUID;
m_wstrGuildName		 = kInfo.m_wstrGuildName;
m_ucMemberShipGrade	 = kInfo.m_ucMemberShipGrade;
m_iHonorPoint		 = kInfo.m_iHonorPoint;
}

void KUserGuildManager::SetGuildInfo( IN int iGuildUID, IN const std::wstring& wstrGuildName, IN u_char ucMemberShipGrade, IN int iHonorPoint )
{
	KUserGuildInfo kInfo;
	kInfo.m_iGuildUID		  = iGuildUID;
	kInfo.m_wstrGuildName	  = wstrGuildName;
	kInfo.m_ucMemberShipGrade = ucMemberShipGrade;
	kInfo.m_iHonorPoint		  = iHonorPoint;
	SetGuildInfo( kInfo );
}

void KUserGuildManager::SetMemberShipGrade( IN u_char ucMemberShipGrade )
{
	if( SEnum::GUG_MASTER > ucMemberShipGrade  ||
		SEnum::GUG_NEW_USER < ucMemberShipGrade )
	{
		START_LOG( cerr, L"길드 멤버 등급값이 이상합니다. 절대 발생하면 안되는 에러!" )
			<< BUILD_LOGc( ucMemberShipGrade )
			<< END_LOG;
	}

	m_ucMemberShipGrade = ucMemberShipGrade;
}

void KUserGuildManager::OnDecreaseSpirit( IN KGSUserPtr spUser, IN int iDecreaseSpirit )
{
	if( spUser == NULL )
		return;

	if( spUser->GetGuildUID() <= 0 )
	{
		START_LOG( cerr, L"길드원이 아닌데 이 함수를 호출했다?" )
			<< BUILD_LOG( spUser->GetCharUID() )
			<< END_LOG;
		return;
	}

	if( iDecreaseSpirit < 0 )
	{
		START_LOG( cerr, L"소모 근성도값이 마이너스값이 들어오면 안된다. 잘못 계산한듯." )
			<< BUILD_LOG( iDecreaseSpirit )
			<< END_LOG;
		return;
	}

	if( iDecreaseSpirit == 0 )
		return;

	// 명예 포인트 얻기
	const int iIncreaseHonorPoint = SiCXSLGuildManager()->GetHonorPoint_Dungeon( spUser->GetLevel(), GetMemberShipGrade(), iDecreaseSpirit, spUser->IsPcBang() );

	// 명예 포인트를 채웁니다.
	m_iHonorPoint += iIncreaseHonorPoint;

	// 명예 포인트가 가득찼는지 확인
	CheckHonorPoint( spUser );
}

//{{ 2009. 11. 16  최육사	대전길드포인트
void KUserGuildManager::OnUpdatePvpUnitInfo( IN KGSUserPtr spUser, IN int iPvpMemberCount )
{
	if( spUser == NULL )
		return;

	if( spUser->GetGuildUID() <= 0 )
	{
		START_LOG( cerr, L"길드원이 아닌데 이 함수를 호출했다?" )
			<< BUILD_LOG( spUser->GetCharUID() )
			<< END_LOG;
		return;
	}

	if( iPvpMemberCount == 0 )
		return;

	// 명예 포인트 얻기
	const int iIncreaseHonorPoint = SiCXSLGuildManager()->GetHonorPoint_Pvp( spUser->GetLevel(), GetMemberShipGrade(), iPvpMemberCount, spUser->IsPcBang() );

	// 명예 포인트를 채웁니다.
	m_iHonorPoint += iIncreaseHonorPoint;

	// 명예 포인트가 가득찼는지 확인
	CheckHonorPoint( spUser );
}

void KUserGuildManager::CheckHonorPoint( IN KGSUserPtr spUser )
{
	if( spUser == NULL )
		return;

	// 명예 포인트 최대치인지 확인
	if( GetHonorPoint() < HONOR_POINT_MAX )
		return;

	m_iHonorPoint -= HONOR_POINT_MAX; // 최대치 만큼 감소 시킵니다. 초과치 만큼 남은량으로 계산됨

	// 길드 포인트 DB에 업데이트하러 가자!
	KDBE_UPDATE_GUILD_EXP_REQ kPacketToDB;
	kPacketToDB.m_iUnitUID = spUser->GetCharUID();
	kPacketToDB.m_iGuildUID = GetGuildUID();
	kPacketToDB.m_iGuildEXP = 1; // 1포인트 상승
	kPacketToDB.m_iGuildLimitEXP = SiCXSLGuildManager()->GetGuildLimitEXP();
	kPacketToDB.m_iHonorPoint = GetHonorPoint();
	spUser->SendToGameDB( DBE_UPDATE_GUILD_EXP_REQ, kPacketToDB );
}
//}}

bool KUserGuildManager::CheckBuyGuildItem( IN KGSUserPtr spUser, IN const std::map< int, int >& mapBuyItemList )
{
	SET_ERROR( NET_OK );

	if( spUser == NULL )
	{
		START_LOG( cerr, L"유저 객체 포인터가 이상합니다." )
			<< END_LOG;
		return false;
	}

	// 운영자면 바로 통과
	if( spUser->GetAuthLevel() >= SEnum::UAL_GM )
		return true;

	// 길드 창단서 검사
	if( mapBuyItemList.find( CXSLItem::SI_GUILD_FOUNDATION ) != mapBuyItemList.end() )
	{
		// 길드 멤버인지 검사
		if( spUser->GetGuildUID() > 0 )
		{
			START_LOG( cerr, L"길드원이면 길드 창단서를 구매할 수 없다!" )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetGuildUID() )
				<< END_LOG;

			SET_ERROR( ERR_GUILD_37 );
			return false;
		}

		// 길드 퀘스트를 클리어 했는지 검사
		if( spUser->IsCompleteQuest( GUILD_QUEST_ID ) == false )
		{
			SET_ERROR( ERR_GUILD_44 );
			return false;
		}

		// 이미 길드 창단서를 구매했는지 검사
		if( spUser->IsExistInventoryItemWithBank( CXSLItem::SI_GUILD_FOUNDATION ) == true )
		{
			SET_ERROR( ERR_GUILD_38 );
			return false;
		}
	}

	return true;
}

#endif GUILD_TEST

//////////////////////////////////////////////////////////////////////////
//{{ 2009. 11. 24  최육사	길드스킬
#ifdef GUILD_SKILL_TEST

void KUserGuildManager::SetGuildSkill( IN const KGuildSkillInfo& kInfo )
{
	std::vector< KGuildSkillData >::const_iterator vit;
	for( vit = kInfo.m_vecGuildSkill.begin(); vit != kInfo.m_vecGuildSkill.end(); ++vit )
	{
		m_mapSkillTree.insert( std::make_pair( vit->m_iSkillID, *vit ) );
	}

	// 사용중인 기간제 스킬 포인트
	SetMaxGuildCSPoint( kInfo.m_iMaxGuildCSPoint );
}

void KUserGuildManager::UpdateGuildSkill( IN const KEGS_GET_GUILD_SKILL_NOT& kInfo )
{
	GuildSkillDataMap::iterator mit = m_mapSkillTree.find( kInfo.m_iGuildSkillID );
	if( mit == m_mapSkillTree.end() )
	{
		// 새로운 스킬을 배운다면..
		KGuildSkillData kGuildSkill;
		kGuildSkill.m_iSkillID		= kInfo.m_iGuildSkillID;
		kGuildSkill.m_cSkillLevel	= kInfo.m_iGuildSkillLv;
		kGuildSkill.m_cSkillCSPoint = kInfo.m_iGuildSkillCSPoint;
		m_mapSkillTree.insert( std::make_pair( kInfo.m_iGuildSkillID, kGuildSkill ) );
	}
	else
	{
		// 길드 스킬 레벨이 변경된다면
		mit->second.m_cSkillLevel = static_cast<u_char>(kInfo.m_iGuildSkillLv);
		mit->second.m_cSkillCSPoint = static_cast<u_char>(kInfo.m_iGuildSkillCSPoint);
	}
}

bool KUserGuildManager::ResetGuildSkill( IN const KEGS_RESET_GUILD_SKILL_NOT& kInfo )
{
	GuildSkillDataMap::iterator mit = m_mapSkillTree.find( kInfo.m_iDelGuildSkillID );
	if( mit == m_mapSkillTree.end() )
		return false;

	// 길드 스킬 레벨 감소
	--mit->second.m_cSkillLevel;

	// 길드 스킬 레벨이 0이 된다면..
	if( mit->second.m_cSkillLevel <= 0 )
	{
		m_mapSkillTree.erase( mit );
	}

	return true;
}

void KUserGuildManager::ExpireGuildSkill( IN const KEGS_EXPIRE_GUILD_CASH_SKILL_POINT_NOT& kInfo )
{
	std::vector< KGuildSkillData >::const_iterator vit;
	for( vit = kInfo.m_vecGuildSkillData.begin(); vit != kInfo.m_vecGuildSkillData.begin(); ++vit )
	{
		GuildSkillDataMap::iterator mit = m_mapSkillTree.find( vit->m_iSkillID );
		if( mit != m_mapSkillTree.end() )
		{
			// 길드 스킬 정보 갱신
			mit->second.m_cSkillLevel = vit->m_cSkillLevel;
			mit->second.m_cSkillCSPoint = vit->m_cSkillCSPoint;
		}
	}

	// 기간제 스킬 포인트 초기화
	SetMaxGuildCSPoint( 0 );
}

void KUserGuildManager::InitGuildSkill()
{
	// 길드 스킬트리 초기화
	m_mapSkillTree.clear();    
}

void KUserGuildManager::GetGuildPassiveSkillData( OUT std::vector< KSkillData >& vecSkillData )
{
	vecSkillData.clear();

	// 신입 길드원이면 길드 스킬을 적용시키지 않는다.
	if( GetMemberShipGrade() == SEnum::GUG_NEW_USER )
		return;

	GuildSkillDataMap::iterator mit;
	for( mit = m_mapSkillTree.begin(); mit != m_mapSkillTree.end(); ++mit )
	{
		KSkillData kSkillData;
		kSkillData.m_iSkillID		= mit->first;
		kSkillData.m_cSkillLevel	= mit->second.m_cSkillLevel;
		vecSkillData.push_back( kSkillData );
	}
}

void KUserGuildManager::GetGuildSkillStat( KStat& kStat )
{
	kStat.Init();

	// 신입 길드원이면 스탯 적용 시키지 않는다.
	if( GetMemberShipGrade() == SEnum::GUG_NEW_USER )
		return;

	GuildSkillDataMap::const_iterator mit;
	for( mit = m_mapSkillTree.begin(); mit != m_mapSkillTree.end(); ++mit )
	{
		const KGuildSkillData& kGuildSkillData = mit->second;
		if( kGuildSkillData.m_cSkillLevel <= 0 )
			continue;

		const CXSLSkillTree::SkillTemplet* pSkillTemplet = SiCXSLSkillTree()->GetSkillTemplet( mit->first, kGuildSkillData.m_cSkillLevel );
		if( pSkillTemplet == NULL )
		{
			START_LOG( cerr, L"스킬 템플릿 포인트 이상.!" )
				<< BUILD_LOG( mit->first )
				<< BUILD_LOGc( kGuildSkillData.m_cSkillLevel )
				<< END_LOG;
			continue;
		}

		switch( pSkillTemplet->m_eType )
		{
		case CXSLSkillTree::ST_GUILD_PASSIVE_PHYSIC_ATTACK_MAGIC_DEFENCE:
		case CXSLSkillTree::ST_GUILD_PASSIVE_MAGIC_ATTCK_PHYSIC_DEFENCE:
			{
				kStat.m_iBaseHP			+= (int)pSkillTemplet->m_Stat.m_fBaseHP;
				kStat.m_iAtkPhysic		+= (int)pSkillTemplet->m_Stat.m_fAtkPhysic;
				kStat.m_iAtkMagic		+= (int)pSkillTemplet->m_Stat.m_fAtkMagic;
				kStat.m_iDefPhysic		+= (int)pSkillTemplet->m_Stat.m_fDefPhysic;
				kStat.m_iDefMagic		+= (int)pSkillTemplet->m_Stat.m_fDefMagic;
			} break;
		}
	}
}

void KUserGuildManager::GetGuildSkillBonusRate( OUT float& fAddGuildBonusExpRate, IN OUT float& fAddSocketOptEDRate )
{
	fAddGuildBonusExpRate = 0.0f;

	// 신입 길드원이면 길드 스킬을 적용시키지 않는다.
	if( GetMemberShipGrade() == SEnum::GUG_NEW_USER )
		return;

	// 동료의 응원
	GuildSkillDataMap::const_iterator mit = m_mapSkillTree.find( CXSLSkillTree::SI_GP_COMMON_CHEERING_OF_TEAM );
	if( mit != m_mapSkillTree.end() )
	{
		if( mit->second.m_cSkillLevel > 0 )
		{
			const CXSLSkillTree::SkillTemplet* pSkillTemplet = SiCXSLSkillTree()->GetSkillTemplet( mit->first, mit->second.m_cSkillLevel );
			if( pSkillTemplet != NULL )
			{
				// 길드원끼리 플레이시 경험치 증가되는 길드 스킬.
				fAddGuildBonusExpRate += pSkillTemplet->GetSkillAbilityValue( CXSLSkillTree::SA_EXP_GAIN_REL );
			}
			else
			{
				START_LOG( cerr, L"스킬 템플릿 포인트 이상.!" )
					<< BUILD_LOG( mit->first )
					<< BUILD_LOGc( mit->second.m_cSkillLevel )
					<< END_LOG;
			}
		}
	}

	// 마이더스의 손
	mit = m_mapSkillTree.find( CXSLSkillTree::SI_GP_COMMON_MIDAS );
	if( mit != m_mapSkillTree.end() )
	{
		if( mit->second.m_cSkillLevel > 0 )
		{
			const CXSLSkillTree::SkillTemplet* pSkillTemplet = SiCXSLSkillTree()->GetSkillTemplet( mit->first, mit->second.m_cSkillLevel );
			if( pSkillTemplet != NULL )
			{
				// 길드원끼리 플레이시 경험치 증가되는 길드 스킬.
				fAddSocketOptEDRate += pSkillTemplet->GetSkillAbilityValue( CXSLSkillTree::SA_ED_GAIN_REL );
			}
			else
			{
				START_LOG( cerr, L"스킬 템플릿 포인트 이상.!" )
					<< BUILD_LOG( mit->first )
					<< BUILD_LOGc( mit->second.m_cSkillLevel )
					<< END_LOG;
			}
		}
	}
}

#endif GUILD_SKILL_TEST
//}}

//{{ 2012. 02. 22	박세훈	길드 이름 변경권
#ifdef SERV_GUILD_CHANGE_NAME
bool KUserGuildManager::IsGuildMaster( void )
{
	return GetMemberShipGrade() == SEnum::GUG_MASTER;
}

void KUserGuildManager::SetGuildNameChanging( void )
{
	bIsGuildNameChanging = true;
}
void KUserGuildManager::ResetGuildNameChanging( void )
{
	bIsGuildNameChanging = false;
}
bool KUserGuildManager::CheckGuildNameChanging( void )
{
	return bIsGuildNameChanging;
}
#endif SERV_GUILD_CHANGE_NAME
//}}

//////////////////////////////////////////////////////////////////////////

*/
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013




