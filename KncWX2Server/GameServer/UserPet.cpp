#include ".\userpet.h"
#include "BaseServer.h"
#include "Lottery.h"
#include "X2Data/XSLItemManager.h"
#include "X2Data/XSLSocketItem.h"
#include "NetError.h"


//{{ 2010. 07. 21  최육사	펫 시스템
#ifdef SERV_PET_SYSTEM


KUserPet::KUserPet( const KPetInfo& kInfo, const std::map< UidType, std::map< UidType, KInventoryItemInfo > >& mapPetItem, bool bCreatePet )
{
	//{{edit : 장훈
	m_sExtroversion.SetVerifyMin(static_cast<short>(CXSLPetManager::PE_MIN_EXTROVERSION));
	m_sEmotion.SetVerifyMin(static_cast<short>(CXSLPetManager::PE_MIN_EMOTION));
	//}}edit : 장훈

	// 초기화
	Clear();

	// 펫 정보 업데이트
	UpdatePetInfo( kInfo );

	// 펫 인벤토리 업데이트
	std::map< UidType, std::map< UidType, KInventoryItemInfo > >::const_iterator mit;
	mit = mapPetItem.find( kInfo.m_iPetUID );
	if( mit != mapPetItem.end() )
	{
		UpdatePetInventory( mit->second );
	}

	// 펫 최대 친밀도 설정
	if( SiCXSLPetManager()->GetEvolutionStepMaxIntimacy( GetPetID(), GetEvolutionStep(), m_iMaxIntimacy ) == false )
	{
		START_LOG( cerr, L"펫의 최대 친밀도값 얻기가 실패하였습니다!" )
			<< BUILD_LOG( GetPetUID() )
			<< BUILD_LOGc( GetPetID() )
			<< BUILD_LOGc( GetEvolutionStep() )
			<< END_LOG;
	}

	// 펫 생성하는거라면?
	if( bCreatePet )
	{
		// DB업데이트 예약!
		m_bReservedDBUpdate = true;
	}
}

KUserPet::~KUserPet(void)
{
}

void KUserPet::Clear()
{
	m_iPetUID				= 0;
#ifdef SERV_PETID_DATA_TYPE_CHANGE //2013.07.02
	m_iPetID				= 0;
#else //SERV_PETID_DATA_TYPE_CHANGE
	m_cPetID				= 0;
#endif //SERV_PETID_DATA_TYPE_CHANGE
	m_cEvolutionStep		= 0;
	m_bAutoFeed				= false;
	m_iCumulativeSatiety	= 0;
	m_iFeedCount			= 0;
	m_iCumulativeIntimacy	= 0;
	m_iMaxIntimacy			= 0;
	m_bReservedDBUpdate		= false;
	m_tPetSommonTimer.restart();
	m_mapPetInventory.clear();
#ifdef SERV_PERIOD_PET
	m_bDestroy				= false;
#endif SERV_PERIOD_PET
#ifdef SERV_PET_SYSTEM_EX1
	m_bAlwaysMaxSatiety = false;
#endif //SERV_PET_SYSTEM_EX1
}

void KUserPet::GetPetInfo( OUT KPetInfo& kInfo ) const
{
	kInfo.m_iPetUID				= m_iPetUID;
#ifdef SERV_PETID_DATA_TYPE_CHANGE //2013.07.02
	kInfo.m_iPetID				= m_iPetID;
#else //SERV_PETID_DATA_TYPE_CHANGE
	kInfo.m_cPetID				= m_cPetID;
#endif //SERV_PETID_DATA_TYPE_CHANGE
	kInfo.m_wstrPetName			= m_wstrPetName;
	kInfo.m_cEvolutionStep		= m_cEvolutionStep;
	kInfo.m_sSatiety			= m_sSatiety;
	kInfo.m_iIntimacy			= m_iIntimacy;
	kInfo.m_sExtroversion		= m_sExtroversion;
	kInfo.m_sEmotion			= m_sEmotion;
	kInfo.m_bAutoFeed			= m_bAutoFeed;
	//kInfo.m_wstrRegDate			= ( std::wstring )( m_tRegDate.Format( _T( "%Y-%m-%d %H:%M:%S" ) ) );
	//kInfo.m_wstrLastSummonDate	= ( std::wstring )( m_tLastSummonDate.Format( _T( "%Y-%m-%d %H:%M:%S" ) ) );
	//{{ 2012. 12. 24	박세훈	펫 오토 루팅 기능 추가
#ifdef SERV_PET_AUTO_LOOTING
	kInfo.m_bAutoLooting		= m_bAutoLooting;
#endif SERV_PET_AUTO_LOOTING
	//}}
#ifdef SERV_EVENT_PET_INVENTORY
	kInfo.m_bEventFoodEat		= m_bEventFoodEat;
	kInfo.m_bIsEventPetID       = m_bIsEventPetID;
#endif SERV_EVENT_PET_INVENTORY
#ifdef SERV_PERIOD_PET
	kInfo.m_wstrDestroyDate		= L"";
	if( m_bDestroy == true )
	{
		kInfo.m_wstrDestroyDate = ( std::wstring )( m_tDestroyDate.Format( _T( "%Y-%m-%d %H:%M:%S" ) ) );
	}
#endif SERV_PERIOD_PET
#ifdef SERV_PET_SYSTEM_EX1
	kInfo.m_bAlwaysMaxSatiety	= m_bAlwaysMaxSatiety;
#endif //SERV_PET_SYSTEM_EX1
}

int KUserPet::GetPetInventorySlotSize() const
{
	// 펫 종류와 진화단계에 따른 인벤토리 크기 얻기
	return SiCXSLPetManager()->GetPetInventorySlotSize( GetPetID(), GetEvolutionStep() );
}

//{{ 2010. 10. 18	최육사	펫 오라 스킬
#ifdef SERV_PET_AURA_SKILL
//{{ 2011. 07. 08    김민성    옵션 수치화
#ifdef SERV_USE_PERCENT_IN_OPTION_DATA
void KUserPet::GetStatByPetSkill( IN bool bIsDungeon, IN const KStat& kBaseStat, IN OUT float& fTotalIncHPRate, OUT KStat& kAddStat, IN unsigned int& uiHP_OnePoint, IN unsigned int& uiUnitLevel )
#else
void KUserPet::GetStatByPetSkill( IN bool bIsDungeon, IN const KStat& kBaseStat, IN OUT float& fTotalIncHPRate, OUT KStat& kAddStat )
#endif SERV_USE_PERCENT_IN_OPTION_DATA
//}} 
{
	kAddStat.Init();

	// 펫이 배고픔 상태인 경우 
	if( IsHungry() == true )
	{
		// 오라 스킬 적용 안됨!
		return;
	}

	//{{ 2011. 07. 25    김민성    아이템 옵션ID 데이터 사이즈 증가
#ifdef SERV_ITEM_OPTION_DATA_SIZE
	std::vector< int > vecAuraSkillOption;
#else
	std::vector< short > vecAuraSkillOption;
#endif SERV_ITEM_OPTION_DATA_SIZE
	//}} 

	// 펫 소켓 옵션 얻기
	if( SiCXSLPetManager()->GetAuraSkillOption( static_cast<CXSLPetManager::PET_UNIT_ID>(GetPetID()), GetEvolutionStep(), vecAuraSkillOption ) == false )
	{
		START_LOG( cerr, L"존재 하지 않는 PetID입니다!" )
			<< BUILD_LOGc( GetPetID() )
			<< BUILD_LOGc( GetEvolutionStep() )
			<< END_LOG;
		return;
	}

	// 소켓 옵션 정보 얻기
	KStat kSocketOptionStat;
	//{{ 2011. 07. 08    김민성    옵션 수치화
#ifdef SERV_USE_PERCENT_IN_OPTION_DATA
	SiCXSLSocketItem()->GetSocketOptionStat( vecAuraSkillOption, bIsDungeon, kBaseStat, fTotalIncHPRate, kSocketOptionStat, uiHP_OnePoint, uiUnitLevel );
#else
	SiCXSLSocketItem()->GetSocketOptionStat( vecAuraSkillOption, bIsDungeon, kBaseStat, fTotalIncHPRate, kSocketOptionStat );
#endif SERV_USE_PERCENT_IN_OPTION_DATA
	//}} 

	// 스탯 더하기
	kAddStat.AddStat( kSocketOptionStat );
}

void KUserPet::GetStatIncreaseRateByPetSkill( IN bool bIsDungeon, IN OUT KStatIncreaseRate& kStatIncreaseRateInfo )
{
	// 펫이 배고픔 상태인 경우
	if( IsHungry() == true )
	{
		// 오라 스킬 적용 안됨!
		return;
	}

	//{{ 2011. 07. 25    김민성    아이템 옵션ID 데이터 사이즈 증가
#ifdef SERV_ITEM_OPTION_DATA_SIZE
	std::vector< int > vecAuraSkillOption;
#else

	std::vector< short > vecAuraSkillOption;
#endif SERV_ITEM_OPTION_DATA_SIZE
	//}} 

	// 펫 소켓 옵션 얻기
	if( SiCXSLPetManager()->GetAuraSkillOption( static_cast<CXSLPetManager::PET_UNIT_ID>(GetPetID()), GetEvolutionStep(), vecAuraSkillOption ) == false )
	{
		START_LOG( cerr, L"존재 하지 않는 PetID입니다!" )
			<< BUILD_LOGc( GetPetID() )
			<< BUILD_LOGc( GetEvolutionStep() )
			<< END_LOG;
		return;
	}

	// 소켓 옵션 정보 얻기
	SiCXSLSocketItem()->GetSocketOptionIncreaseRate( vecAuraSkillOption, bIsDungeon, kStatIncreaseRateInfo );
}

#endif SERV_PET_AURA_SKILL
//}}

void KUserPet::GetDBUpdatePetInfo( OUT KPetInfo& kInfo )
{
	// 현재값
	kInfo.m_iPetUID				= m_iPetUID;
#ifdef SERV_PETID_DATA_TYPE_CHANGE //2013.07.02
	kInfo.m_iPetID				= m_iPetID;
#else //SERV_PETID_DATA_TYPE_CHANGE
	kInfo.m_cPetID				= m_cPetID;
#endif //SERV_PETID_DATA_TYPE_CHANGE
	kInfo.m_wstrPetName			= m_wstrPetName;
	kInfo.m_cEvolutionStep		= m_cEvolutionStep;
	kInfo.m_bAutoFeed			= m_bAutoFeed;
	kInfo.m_wstrLastFeedDate	= ( std::wstring )( m_tFirstFeedDate.Format( _T( "%Y-%m-%d %H:%M:%S" ) ) );
	kInfo.m_wstrLastSummonDate	= ( std::wstring )( m_tLastSummonDate.Format( _T( "%Y-%m-%d %H:%M:%S" ) ) );
    
	// 증분값
	kInfo.m_sSatiety		= m_sSatiety.GetChangeValue();
	m_sSatiety.SetValue( m_sSatiety );

	kInfo.m_iIntimacy		= m_iIntimacy.GetChangeValue();
	m_iIntimacy.SetValue( m_iIntimacy );

	kInfo.m_sExtroversion	= m_sExtroversion.GetChangeValue();
	m_sExtroversion.SetValue( m_sExtroversion );

	kInfo.m_sEmotion		= m_sEmotion.GetChangeValue();
	m_sEmotion.SetValue( m_sEmotion );
}

void KUserPet::RollBackDBUpdatePetInfo( IN const KPetInfo& kInfo )
{
	// 증분값 롤백
	m_sSatiety.AdjustInitValue( m_sSatiety.GetInitValue() - kInfo.m_sSatiety );
	m_iIntimacy.AdjustInitValue( m_iIntimacy.GetInitValue() - kInfo.m_iIntimacy );
	m_sExtroversion.AdjustInitValue( m_sExtroversion.GetInitValue() - kInfo.m_sExtroversion );
	m_sEmotion.AdjustInitValue( m_sEmotion.GetInitValue() - kInfo.m_sEmotion );
}

void KUserPet::Summon( OUT KPetInfo& kPetInfo, OUT int& iPetCategorySlotSize, OUT std::map< UidType, KInventoryItemInfo >& mapPetItem )
{
	//////////////////////////////////////////////////////////////////////////	
	// 소환시 체크하는 함수들 호출!
    
	// 펫 정보 얻기
	GetPetInfo( kPetInfo );

	// 펫 인벤토리 슬롯 갯수 얻기
	iPetCategorySlotSize = GetPetInventorySlotSize();

	// 슬롯 갯수만큼 아이템이 있는지 체크!
	LIF( iPetCategorySlotSize >= static_cast<int>(mapPetItem.size()) );

	// 펫 인벤토리 얻기
	GetPetInventory( mapPetItem );

	// 소환이 되었다면 DB업데이트 예약 걸자!
	m_bReservedDBUpdate = true;

	// 펫 소환 시각 갱신!
	m_tLastSummonDate = CTime::GetCurrentTime();
}

void KUserPet::Unsummon()
{
	//////////////////////////////////////////////////////////////////////////	
	// 소환 해제시 동작하는 함수들 호출!
    
	// 외향지수 변동!
	CheckExtroversion();

	// 감성지수 변동!
	CheckEmotion();
}

void KUserPet::UpdatePetInfo( IN const KPetInfo& kPetInfo )
{
	m_iPetUID			= kPetInfo.m_iPetUID;
#ifdef SERV_PETID_DATA_TYPE_CHANGE //2013.07.02
	m_iPetID			= kPetInfo.m_iPetID;
#else //SERV_PETID_DATA_TYPE_CHANGE
	m_cPetID			= kPetInfo.m_cPetID;
#endif //SERV_PETID_DATA_TYPE_CHANGE
	m_wstrPetName		= kPetInfo.m_wstrPetName;
	m_cEvolutionStep	= kPetInfo.m_cEvolutionStep;		// 진화 단계
	m_sSatiety.SetValue( kPetInfo.m_sSatiety );				// 포만도
	m_iIntimacy.SetValue( kPetInfo.m_iIntimacy );			// 친밀도
	m_sExtroversion.SetValue( kPetInfo.m_sExtroversion );	// 외향성
	m_sEmotion.SetValue( kPetInfo.m_sEmotion );				// 감수성
	m_bAutoFeed			= kPetInfo.m_bAutoFeed;
	//{{ 2012. 12. 24	박세훈	펫 오토 루팅 기능 추가
#ifdef SERV_PET_AUTO_LOOTING
	m_bAutoLooting		= kPetInfo.m_bAutoLooting;			// 오토 루팅 기능 여부
#endif SERV_PET_AUTO_LOOTING
	//}}
#ifdef SERV_PET_SYSTEM_EX1
	m_bAlwaysMaxSatiety = kPetInfo.m_bAlwaysMaxSatiety;	// 항상 최대 포만도
#endif//SERV_PET_SYSTEM_EX1
#ifdef SERV_EVENT_PET_INVENTORY
		m_bEventFoodEat		= kPetInfo.m_bEventFoodEat;
		m_bIsEventPetID		= kPetInfo.m_bIsEventPetID;
#endif SERV_EVENT_PET_INVENTORY
	if( KncUtil::ConvertStringToCTime( kPetInfo.m_wstrLastFeedDate, m_tFirstFeedDate ) == false )
	{
		START_LOG( cerr, L"첫 먹이 시각 정보 변환 실패." )
			<< BUILD_LOG( kPetInfo.m_iPetUID )
			<< BUILD_LOG( kPetInfo.m_wstrLastFeedDate )
			<< END_LOG;

		// 변환 실패하면 새벽 6시를 기준으로 첫 먹이 시각을 설정
		CTime tCurTime = CTime::GetCurrentTime();
		CTime tFirstFeedDate = CTime( tCurTime.GetYear(), tCurTime.GetMonth(), tCurTime.GetDay(), 6, 0, 0, 0 );
		if( tFirstFeedDate < tCurTime )
		{
			tFirstFeedDate += CTimeSpan( 1, 0, 0, 0	);
		}

		m_tFirstFeedDate = tFirstFeedDate;
	}

	if( KncUtil::ConvertStringToCTime( kPetInfo.m_wstrLastSummonDate, m_tLastSummonDate ) == false )
	{
		START_LOG( cerr, L"마지막 소환 시각 정보 변환 실패." )
			<< BUILD_LOG( kPetInfo.m_iPetUID )
			<< BUILD_LOG( kPetInfo.m_wstrLastSummonDate )
			<< END_LOG;

		// 변환 실패하면 현재 시각으로 설정
		m_tLastSummonDate = CTime::GetCurrentTime();
	}

	if( KncUtil::ConvertStringToCTime( kPetInfo.m_wstrRegDate, m_tRegDate ) == false )
	{
		START_LOG( cerr, L"펫 생성 시각 정보 변환 실패." )
			<< BUILD_LOG( kPetInfo.m_iPetUID )
			<< BUILD_LOG( kPetInfo.m_wstrRegDate )
			<< END_LOG;

		// 변환 실패하면 현재 시각으로 설정
		m_tRegDate = CTime::GetCurrentTime();
	}

#ifdef SERV_PERIOD_PET
	m_bDestroy = false;
	if( kPetInfo.m_wstrDestroyDate != L"" )
	{
		if( KncUtil::ConvertStringToCTime( kPetInfo.m_wstrDestroyDate, m_tDestroyDate ) == true )
		{
			m_bDestroy = true;
		}
		else
		{
			START_LOG( cerr, L"만료 시각 정보 변환 실패." )
				<< BUILD_LOG( kPetInfo.m_iPetUID )
				<< BUILD_LOG( kPetInfo.m_wstrDestroyDate )
				<< END_LOG;
		}
	}
#endif SERV_PERIOD_PET
}

void KUserPet::UpdatePetInventory( IN const std::map< UidType, KInventoryItemInfo >& mapPetItem )
{
    m_mapPetInventory.clear();
	m_mapPetInventory = mapPetItem;
}

bool KUserPet::CommandsForPet( IN CXSLPetManager::PET_ACTION_COMMAND eActionCommand, OUT CXSLPetManager::PET_ACTION_COMMAND& ePetActionResult )
{
	ePetActionResult = CXSLPetManager::PAC_NONE;

	// 정상적인 명령인지 체크!
	if( CXSLPetManager::IsValidCommand( eActionCommand ) == false )
	{
		START_LOG( cerr, L"정상적인 명령값이 아닙니다!" )
			<< BUILD_LOG( GetPetUID() )
			<< BUILD_LOG( eActionCommand )
			<< END_LOG;

		return false;
	}

	// 이곳으로 오면 안되는 명령
	switch( eActionCommand )
	{
	case CXSLPetManager::PAC_EAT:
	case CXSLPetManager::PAC_STUFFED:
		{
			START_LOG( cerr, L"이곳으로 오면 안되는 명령입니다!" )
				<< BUILD_LOG( GetPetUID() )
				<< BUILD_LOG( eActionCommand )
				<< END_LOG;
		}
		return false;
	}

	// 무조건 액션을 취하는 명령인지 검사
	if( CXSLPetManager::IsUnconditionalCommand( eActionCommand ) == true )
	{
		ePetActionResult = eActionCommand;
		return true;
	}

	// 최대 친밀도값 검사
	if( GetMaxIntimacy() <= 0 )
	{
		START_LOG( cerr, L"최대 친밀도값이 이상합니다. 펫에게 명령내리기 실패!" )
			<< BUILD_LOG( GetPetUID() )			
			<< BUILD_LOG( GetMaxIntimacy() )
			<< BUILD_LOG( eActionCommand )
			<< END_LOG;

		return false;
	}

	// 포만도 퍼센티지가 40% 이하일때 배고픔 액션을 결정하자!
	if( ( static_cast<float>(GetSatiety()) / static_cast<float>(CXSLPetManager::PE_MAX_SATIETY) * 100.f ) <= 40.f )
	{
		//{{ 2010. 10. 06	최육사	펫 명령 랜덤 함수
#ifdef SERV_PET_COMMAND_RANDOM
		const int iHungryActionRate = static_cast<int>( 100.f - static_cast<float>(GetSatiety()) / static_cast<float>(CXSLPetManager::PE_MAX_SATIETY) * 100.f );

		// 액션 결정!
		const int iRandResult = ( rand() % 100 ) + 1;
		if( iRandResult < iHungryActionRate )
		{
			ePetActionResult = CXSLPetManager::PAC_HUNGRY;
			return true;
		}
#else
		const float fHungryActionRate = 100.f - static_cast<float>(GetSatiety()) / static_cast<float>(CXSLPetManager::PE_MAX_SATIETY) * 100.f;

		// 배고픈 액션이 나올 확률을 넣자!
		KLottery kLot;
		kLot.AddCase( 0, fHungryActionRate );

		// 액션 결정!
		if( kLot.Decision() != KLottery::CASE_BLANK )
		{
			ePetActionResult = CXSLPetManager::PAC_HUNGRY;
			return true;
		}
#endif SERV_PET_COMMAND_RANDOM
		//}}
	}
	
	// 친밀도 퍼센티지로 액션을 결정하자!
	{
		//{{ 2010. 10. 06	최육사	펫 명령 랜덤 함수
#ifdef SERV_PET_COMMAND_RANDOM
		int iActionRate = static_cast<int>( static_cast<float>(GetIntimacy()) / static_cast<float>(GetMaxIntimacy()) * 100.f );
		if( iActionRate < 20 )
		{
			// 최소 20%의 성공확률은 보장
			iActionRate = 20;
		}

		// 액션 결정!
		const int iRandResult = ( rand() % 100 ) + 1;
		if( iRandResult > iActionRate )
		{
			ePetActionResult = CXSLPetManager::PAC_DONOTKNOW;
			return true;
		}
#else
		float fActionRate = static_cast<float>(GetIntimacy()) / static_cast<float>(GetMaxIntimacy()) * 100.f;
		if( fActionRate < 20.f )
		{
			// 최소 20%의 성공확률은 보장
			fActionRate = 20.f;
		}

		// 명령을 알아들을 확률을 넣자!
		KLottery kLot;
		kLot.AddCase( 0, fActionRate );

		// 액션 결정!
		if( kLot.Decision() == KLottery::CASE_BLANK )
		{
			ePetActionResult = CXSLPetManager::PAC_DONOTKNOW;
			return true;
		}
#endif SERV_PET_COMMAND_RANDOM
		//}}		
	}	
    
	ePetActionResult = eActionCommand;
	return true;
}

//{{ 2012. 04. 26	박세훈	펫 장난감
#ifdef SERV_PET_TOY
void KUserPet::DecreaseSatietyOnDungeonRoom( IN char cUserCount, IN short sSubStageExpNpcNum, IN const bool& bHaveToy )
#else
void KUserPet::DecreaseSatietyOnDungeonRoom( IN char cUserCount, IN short sSubStageExpNpcNum )
#endif SERV_PET_TOY
//}}
{
	if( sSubStageExpNpcNum <= 0 )
	{
		// 경험치 주는 npc가 없으므로 포만도를 깎지 않음.
		return;
	}

	// 공식으로 깎을 양을 계산!
	//const short sDecreaseSatiety = static_cast<short>( ( 21.0 - static_cast<double>(cUserCount) ) / static_cast<double>(sSubStageExpNpcNum) );
	//{{ 2011. 04. 25	최육사	펫 포만도 소모량 변경
#ifdef SERV_PET_SATIETY_NEW
	//{{ 2012. 06. 01	김민성       특정 스테이지에서 펫 포만도가 감소하지 않는 오류 수정
#ifdef SERV_PET_SATIETY_DECREASE_BUG_FIX
	short sDecreaseSatiety = SiCXSLPetManager()->CalcDecreaseSatietyValueOnDungeonRoom( (CXSLPetManager::PET_UNIT_ID)GetPetID(), GetEvolutionStep(), sSubStageExpNpcNum );
#else
	const short sDecreaseSatiety = SiCXSLPetManager()->CalcDecreaseSatietyValueOnDungeonRoom( (CXSLPetManager::PET_UNIT_ID)GetPetID(), GetEvolutionStep(), sSubStageExpNpcNum );
#endif SERV_PET_SATIETY_DECREASE_BUG_FIX
	//}}
#else
	const short sDecreaseSatiety = static_cast<short>( 30.0 / static_cast<double>(sSubStageExpNpcNum) ); // 수정된 공식
#endif SERV_PET_SATIETY_NEW
	//}}

	//{{ 2012. 06. 01	김민성       특정 스테이지에서 펫 포만도가 감소하지 않는 오류 수정
#ifdef SERV_PET_SATIETY_DECREASE_BUG_FIX
	if( sDecreaseSatiety < 1 )
	{
		START_LOG( clog, L"펫 포만도 감소값이 이상합니다!" )
			<< BUILD_LOGc( GetPetID() )
			<< BUILD_LOGc( GetEvolutionStep() )
			<< BUILD_LOG( sSubStageExpNpcNum )
			<< END_LOG;

		sDecreaseSatiety = 1;
	}
#else
	if( sDecreaseSatiety == 0 )
	{
		START_LOG( cerr, L"펫 포만도 감소값이 이상합니다!" )
			<< BUILD_LOGc( GetPetID() )
			<< BUILD_LOGc( GetEvolutionStep() )
			<< BUILD_LOG( sSubStageExpNpcNum )
			<< END_LOG;
	}
#endif SERV_PET_SATIETY_DECREASE_BUG_FIX
	//}}
	

	// 포만도를 깎자!
	DecreaseSatiety( sDecreaseSatiety );

	// 친밀도 변동을 위해 미리 계산해서 누적시키자!
	
	//{{ 2012. 04. 26	박세훈	펫 장난감
#ifdef SERV_PET_TOY
	const int iCalcIntimacy = CXSLPetManager::CalcChangeIntimacyValue( CXSLRoom::RT_DUNGEON, cUserCount, GetSatiety(), bHaveToy );
#else
	const int iCalcIntimacy = CXSLPetManager::CalcChangeIntimacyValue( CXSLRoom::RT_DUNGEON, cUserCount, GetSatiety() );
#endif SERV_PET_TOY
	//}}

	if( iCalcIntimacy > 0 )
	{
        IncreaseIntimacy( iCalcIntimacy );
	}
	else if( iCalcIntimacy < 0 )
	{
        DecreaseIntimacy( abs( iCalcIntimacy ) );
	}
}

//{{ 2012. 04. 26	박세훈	펫 장난감
#ifdef SERV_PET_TOY
void KUserPet::DecreaseSatietyOnPvpRoom( IN char cUserCount, IN const bool& bHaveToy )
#else
void KUserPet::DecreaseSatietyOnPvpRoom( IN char cUserCount )
#endif SERV_PET_TOY
//}}
{
	// 공식으로 깎을 양을 계산!
	//const short sDecreaseSatiety = static_cast<short>( ( 21.0 - static_cast<double>(cUserCount) ) / 2.5 );
	//{{ 2011. 04. 25	최육사	펫 포만도 소모량 변경
#ifdef SERV_PET_SATIETY_NEW
	//{{ 2012. 06. 01	김민성       특정 스테이지에서 펫 포만도가 감소하지 않는 오류 수정
#ifdef SERV_PET_SATIETY_DECREASE_BUG_FIX
	short sDecreaseSatiety = SiCXSLPetManager()->CalcDecreaseSatietyValueOnPvpRoom( (CXSLPetManager::PET_UNIT_ID)GetPetID(), GetEvolutionStep() );
#else
	const short sDecreaseSatiety = SiCXSLPetManager()->CalcDecreaseSatietyValueOnPvpRoom( (CXSLPetManager::PET_UNIT_ID)GetPetID(), GetEvolutionStep() );
#endif SERV_PET_SATIETY_DECREASE_BUG_FIX
	//}}
#else
	const short sDecreaseSatiety = static_cast<short>( 30.0 / 2.5 ); // 수정된 공식
#endif SERV_PET_SATIETY_NEW
	//}}

	//{{ 2012. 06. 01	김민성       특정 스테이지에서 펫 포만도가 감소하지 않는 오류 수정
#ifdef SERV_PET_SATIETY_DECREASE_BUG_FIX
	if( sDecreaseSatiety < 1 )
	{
		START_LOG( clog, L"펫 포만도 감소값이 이상합니다!" )
			<< BUILD_LOGc( GetPetID() )
			<< BUILD_LOGc( GetEvolutionStep() )
			<< END_LOG;

		sDecreaseSatiety = 1;
	}
#else
	if( sDecreaseSatiety == 0 )
	{
		START_LOG( cerr, L"펫 포만도 감소값이 이상합니다!" )
			<< BUILD_LOGc( GetPetID() )
			<< BUILD_LOGc( GetEvolutionStep() )
			<< END_LOG;
	}
#endif SERV_PET_SATIETY_DECREASE_BUG_FIX
	//}}

	// 포만도를 깎자!
	DecreaseSatiety( sDecreaseSatiety );

	// 변경되는 친밀도 계산해서 저장하기!

	//{{ 2012. 04. 26	박세훈	펫 장난감
#ifdef SERV_PET_TOY
	const int iCalcIntimacy = CXSLPetManager::CalcChangeIntimacyValue( CXSLRoom::RT_PVP, cUserCount, GetSatiety(), bHaveToy );
#else
	const int iCalcIntimacy = CXSLPetManager::CalcChangeIntimacyValue( CXSLRoom::RT_PVP, cUserCount, GetSatiety() );
#endif SERV_PET_TOY
	//}}

	if( iCalcIntimacy > 0 )
	{
		IncreaseIntimacy( iCalcIntimacy );
	}
	else if( iCalcIntimacy < 0 )
	{
		DecreaseIntimacy( abs( iCalcIntimacy ) );
	}
}

short KUserPet::IncreaseSatiety( IN short sIncreaseSatiety )
{
	short sIncreased = 0;

	if( sIncreaseSatiety <=  0 )
	{
		START_LOG( cwarn, L"증가시키려는 포만도 값이 이상함." )
			<< BUILD_LOG( sIncreaseSatiety )
			<< END_LOG;

		return GetSatiety();
	}

	if( GetSatiety() < 0 )
	{
		START_LOG( cerr, L"포만도 값이 이상함." )
			<< BUILD_LOG( GetSatiety() )
			<< END_LOG;

		return GetSatiety();
	}

	// 너무 많이 높여서 최대치를 넘지 않도록 한다!
	sIncreased = std::min< int >( ( CXSLPetManager::PE_MAX_SATIETY - GetSatiety() ), sIncreaseSatiety );

	// 높이자!
	m_sSatiety += sIncreased;

	// 변경되는 값 누적
	m_iCumulativeSatiety += sIncreased;

	// 포만도 증가 횟수 저장
	++m_iFeedCount;

	// 변경된 값을 리턴해주자!
	return GetSatiety();
}

short KUserPet::DecreaseSatiety( IN short sDecreaseSatiety )
{
	short sDecreased = 0;

	if( sDecreaseSatiety <=  0 )
	{
		START_LOG( cerr, L"감소시키려는 포만도 값이 이상함." )
			<< BUILD_LOG( sDecreaseSatiety )
			<< END_LOG;

		return GetSatiety();
	}

	if( GetSatiety() < 0 )
	{
		START_LOG( cerr, L"포만도 값이 이상함." )
			<< BUILD_LOG( GetSatiety() )
			<< END_LOG;

		return GetSatiety();
	}

	// 너무 많이 깎아서 마이너스값이 되지 않도록 한다!
	sDecreased = std::min< short >( GetSatiety(), sDecreaseSatiety );

#ifdef SERV_PET_SYSTEM_EX1
	if( m_bAlwaysMaxSatiety == true )
		sDecreased = 0;
#endif //SERV_PET_SYSTEM_EX1
	// 깎자!
	m_sSatiety -= sDecreased;

	// 변경되는 값 누적 - 깎는 경우는 누적에 포함 안함.
	//m_iCumulativeSatiety -= sDecreased;

	// 변경된 값을 리턴해주자!
    return GetSatiety();
}

int KUserPet::IncreaseIntimacy( IN int iIncreaseIntimacy )
{
	int iIncreased = 0;

	if( iIncreaseIntimacy <=  0 )
	{
		START_LOG( cerr, L"증가시키려는 친밀도 값이 이상함." )
			<< BUILD_LOG( iIncreaseIntimacy )
			<< END_LOG;

		return GetIntimacy();
	}

	if( GetIntimacy() < 0 )
	{
		START_LOG( cerr, L"포만도 값이 이상함." )
			<< BUILD_LOG( GetIntimacy() )
			<< END_LOG;

		return GetIntimacy();
	}

	// 너무 많이 높여서 최대치를 넘지 않도록 한다!
	iIncreased = std::min< int >( ( GetMaxIntimacy() - GetIntimacy() ), iIncreaseIntimacy );

	// 높이자!
	m_iIntimacy += iIncreased;

	// 변경되는 값 누적
	m_iCumulativeIntimacy += iIncreased;

	// 변경된 값을 리턴해주자!
	return GetIntimacy();
}

int KUserPet::DecreaseIntimacy( IN int iDecreaseIntimacy )
{
	int iDecreased = 0;

	if( iDecreaseIntimacy <=  0 )
	{
		START_LOG( cerr, L"감소시키려는 친밀도 값이 이상함." )
			<< BUILD_LOG( iDecreaseIntimacy )
			<< END_LOG;

		return GetIntimacy();
	}

	if( GetIntimacy() < 0 )
	{
		START_LOG( cerr, L"친밀도 값이 이상함." )
			<< BUILD_LOG( GetIntimacy() )
			<< END_LOG;

		return GetIntimacy();
	}

	// 너무 많이 깎아서 마이너스값이 되지 않도록 한다!
	iDecreased = std::min< int >( GetIntimacy(), iDecreaseIntimacy );

	// 깎자!
	m_iIntimacy -= iDecreased;

	// 변경되는 값 누적
	m_iCumulativeIntimacy -= iDecreased;

	// 변경된 값을 리턴해주자!
	return GetIntimacy();
}

short KUserPet::IncreaseExtroversion( IN short sIncreaseExtroversion )
{
	//short sIncreased = 0;

	if( sIncreaseExtroversion <=  0 )
	{
		START_LOG( cerr, L"증가시키려는 외향성값이 이상함." )
			<< BUILD_LOG( sIncreaseExtroversion )
			<< END_LOG;

		return GetExtroversion();
	}

	//{{edit : 장훈
	if( GetExtroversion() < CXSLPetManager::PE_MIN_EXTROVERSION ||
		GetExtroversion() > CXSLPetManager::PE_MAX_EXTROVERSION )
	{
		START_LOG( cerr, L"외향성 값이 이상함." )
			<< BUILD_LOG( GetExtroversion() )
			<< END_LOG;
		return GetExtroversion();
	}

	if( (GetExtroversion() + sIncreaseExtroversion) > CXSLPetManager::PE_MAX_EXTROVERSION )
	{
		//START_LOG( cerr, L"외향성 값이 이상함." )
		//	<< BUILD_LOG( GetExtroversion() )
		//	<< END_LOG;

		short sTemp = (GetExtroversion() + sIncreaseExtroversion) - static_cast<short>(CXSLPetManager::PE_MAX_EXTROVERSION);
		m_sExtroversion += (sIncreaseExtroversion - sTemp);

		return GetExtroversion();
	}
	
	m_sExtroversion += sIncreaseExtroversion;

	// 너무 많이 높여서 최대치를 넘지 않도록 한다!
	//sIncreased = std::min< int >( ( CXSLPetManager::PE_MAX_EXTROVERSION - GetExtroversion() ), sIncreaseExtroversion );

	// 높이자!
	//m_sExtroversion += sIncreased;
	//}}edit : 장훈

	// 변경된 값을 리턴해주자!
	return GetExtroversion();
}

short KUserPet::DecreaseExtroversion( IN short sDecreaseExtroversion )
{
	//short sDecreased = 0;

	if( sDecreaseExtroversion <=  0 )
	{
		START_LOG( cerr, L"감소시키려는 외향성 값이 이상함." )
			<< BUILD_LOG( sDecreaseExtroversion )
			<< END_LOG;

		return GetExtroversion();
	}

	//{{edit : 장훈
	if( GetExtroversion() < CXSLPetManager::PE_MIN_EXTROVERSION ||
		GetExtroversion() > CXSLPetManager::PE_MAX_EXTROVERSION )
	{
		START_LOG( cerr, L"외향성 값이 이상함." )
			<< BUILD_LOG( GetExtroversion() )
			<< END_LOG;
	}

	if( (GetExtroversion() - sDecreaseExtroversion) < CXSLPetManager::PE_MIN_EXTROVERSION )
	{
		//START_LOG( cerr, L"외향성 값이 이상함." )
		//	<< BUILD_LOG( GetExtroversion() )
		//	<< END_LOG;

		short sTemp = (GetExtroversion() - sDecreaseExtroversion) - static_cast<short>(CXSLPetManager::PE_MIN_EXTROVERSION);
		m_sExtroversion -= (sDecreaseExtroversion + sTemp); // sDes 값에서 빼줌

		return GetExtroversion();
	}

	m_sExtroversion -= sDecreaseExtroversion;

	// 너무 많이 깎아서 마이너스값이 되지 않도록 한다!
	//sDecreased = std::min< short >( GetExtroversion(), sDecreaseExtroversion );

	// 깎자!
	//m_sExtroversion -= sDecreased;

	//}}edit : 장훈

	// 변경된 값을 리턴해주자!
	return GetExtroversion();
}

short KUserPet::IncreaseEmotion( IN short sIncreaseEmotion )
{
	//short sIncreased = 0;

	if( sIncreaseEmotion <=  0 )
	{
		START_LOG( cerr, L"증가시키려는 감수성 값이 이상함." )
			<< BUILD_LOG( sIncreaseEmotion )
			<< END_LOG;

		return GetEmotion();
	}
	//{{edit : 장훈
	if( GetEmotion() < CXSLPetManager::PE_MIN_EMOTION ||
		GetEmotion() > CXSLPetManager::PE_MAX_EMOTION )
	{
		START_LOG( cerr, L"감수성 값이 이상함." )
			<< BUILD_LOG( GetEmotion() )
			<< END_LOG;
	}

	if( (GetEmotion() + sIncreaseEmotion) > CXSLPetManager::PE_MAX_EMOTION )
	{
		//START_LOG( cerr, L"감수성 값이 이상함." )
		//	<< BUILD_LOG( GetEmotion() )
		//	<< END_LOG;

		short sTemp = (GetEmotion() + sIncreaseEmotion) - static_cast<short>(CXSLPetManager::PE_MAX_EMOTION);
		m_sEmotion += (sIncreaseEmotion - sTemp);

		return GetEmotion();
	}

	m_sEmotion += sIncreaseEmotion;

	// 너무 많이 높여서 최대치를 넘지 않도록 한다!
	//sIncreased = std::min< int >( ( CXSLPetManager::PE_MAX_EMOTION - GetEmotion() ), sIncreaseEmotion );

	// 높이자!
	//m_sEmotion += sIncreased;

	//}}edit : 장훈

	// 변경된 값을 리턴해주자!
	return GetEmotion();
}

short KUserPet::DecreaseEmotion( IN short sDecreaseEmotion )
{
	//short sDecreased = 0;

	if( sDecreaseEmotion <=  0 )
	{
		START_LOG( cerr, L"감소시키려는 감수성 값이 이상함." )
			<< BUILD_LOG( sDecreaseEmotion )
			<< END_LOG;

		return GetEmotion();
	}

	//{{edit : 장훈
	if( GetEmotion() < CXSLPetManager::PE_MIN_EMOTION ||
		GetEmotion() > CXSLPetManager::PE_MAX_EMOTION )
	{
		START_LOG( cerr, L"감수성 값이 이상함." )
			<< BUILD_LOG( GetEmotion() )
			<< END_LOG;
	}

	if( (GetEmotion() - sDecreaseEmotion) < CXSLPetManager::PE_MIN_EMOTION )
	{
		//START_LOG( cerr, L"감수성 값이 이상함." )
		//	<< BUILD_LOG( GetEmotion() )
		//	<< END_LOG;
		short sTemp = (GetEmotion() - sDecreaseEmotion) - static_cast<short>(CXSLPetManager::PE_MIN_EMOTION);
		m_sEmotion -= (sDecreaseEmotion + sTemp);

		return GetEmotion();
	}

	m_sEmotion -= sDecreaseEmotion;

	// 너무 많이 깎아서 마이너스값이 되지 않도록 한다!
	//sDecreased = std::min< short >( GetEmotion(), sDecreaseEmotion );

	// 깎자!
	//m_sEmotion -= sDecreased;

	//}}edit : 장훈

	// 변경된 값을 리턴해주자!
	return GetEmotion();
}

bool KUserPet::IsPetStuffed( IN float fStuffedPercent /*= 80.f*/ ) const
{
	// 포만도가 80%를 넘으면 배부른 상태이다!
	return ( ( static_cast<float>(GetSatiety()) / static_cast<float>(CXSLPetManager::PE_MAX_SATIETY) * 100.f ) >= fStuffedPercent );
}

bool KUserPet::IsHungry() const
{
    // 포만도가 40%보다 낮으면 배고픈 상태이다!
	return ( ( static_cast<float>(GetSatiety()) / static_cast<float>(CXSLPetManager::PE_MAX_SATIETY) * 100.f ) < 40.f );
}

bool KUserPet::IsPetSpecialFeed() const
{
	// 최대 친밀도 값 검사
	if( GetMaxIntimacy() <= 0 )
	{
		START_LOG( cerr, L"최대 친밀도 값이 이상합니다!" )
			<< BUILD_LOG( GetPetUID() )
			<< BUILD_LOGc( GetPetID() )
			<< BUILD_LOG( GetMaxIntimacy() )
			<< END_LOG;
		return false;
	}

	// 친밀도가 70%를 넘으면 특별한 아이템만 먹어요!
	return ( ( static_cast<float>(GetIntimacy()) / static_cast<float>(GetMaxIntimacy()) * 100.f ) >= 70.f );
}

//{{ 2011. 05. 23  김민성	큐피엘 젤리 친밀도 증감
#ifdef SERV_ADD_KEWPIEL_JELLY
bool KUserPet::Feed( IN char cLevel, IN int iFeedItemID, OUT bool& bIsFirstFeed, OUT short& sCurrentSatiety, OUT int& iCurrentIntimacy, OUT int& iAddIntimacy )
#else
bool KUserPet::Feed( IN char cLevel, IN int iFeedItemID, OUT bool& bIsFirstFeed, OUT short& sCurrentSatiety, OUT int& iCurrentIntimacy )
#endif SERV_ADD_KEWPIEL_JELLY
//}}
{
	SET_ERROR( NET_OK );	
#ifdef SERV_EVENT_PET_INVENTORY
///현재 펫 먹이가 이벤트 먹이고 펫 ID가 이벤트 펫 ID라면 먹이를 먹이고 true를 반환한다.
///대신 한번 먹었던 먹이를 또 먹게 하면 안되니까 먹었는지 안먹었는지 체크 한다.
///이벤트 펫이 아닌데 펫 먹이가 이벤트 먹이라면 먹이지 말고 리턴 시킨다.
	if( iFeedItemID == 141000440 )
	{
		if( m_bIsEventPetID )
		{
			if( m_bEventFoodEat == false ) //먹이를 먹지 않았다.
			{
				m_bEventFoodEat = true; //먹이를 먹인다.
				return true;
			}
			else
			{
				//이벤트 펫이지만 이미 먹이를 먹었기 떄문에 패스한다
				SET_ERROR( ERR_PET_11 );
				return false;
			}
		}
		else
		{
			SET_ERROR( ERR_PET_11 );
			return false;
		}
	}
#endif SERV_EVENT_PET_INVENTORY
	// 포만도가 80% 이상이면 먹이를 안먹어요!
	if( IsPetStuffed() == true )
	{
		SET_ERROR( ERR_PET_10 );
		return false;
	}

	// 친밀도가 70%를 너머으면 특별한 아이템만 먹어요!
	if( IsPetSpecialFeed() == true )
	{
		if( SiCXSLPetManager()->IsSpecialFeedItemID( iFeedItemID ) == false )
		{
			SET_ERROR( ERR_PET_12 );
			return false;
		}
	}

	// 존재하는 먹이 인지 체크!
	const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iFeedItemID );
	if( pItemTemplet == NULL )
	{
		START_LOG( cerr, L"존재하지 않는 아이템을 먹이로 주려고 했습니다!" )
			<< BUILD_LOG( iFeedItemID )
			<< END_LOG;

		SET_ERROR( ERR_ITEM_04 );
		return false;
	}

	// 아바타인지 검사
	if( pItemTemplet->m_bFashion == true )
	{
		SET_ERROR( ERR_PET_16 );
		return false;
	}	

	short sIncreaseSatiety = 0;

	//{{ 2011. 05. 23  김민성	큐피엘 젤리 친밀도 증감
#ifdef SERV_ADD_KEWPIEL_JELLY
	int iAddIncreaseIntimacy = 0;	// 초기화
	iAddIntimacy = 0;				// 초기화
#endif SERV_ADD_KEWPIEL_JELLY
	//}}

	// 먹이로 줄수 있는 아이템인지 검사
	switch( pItemTemplet->m_ItemType )
	{
	case CXSLItem::IT_WEAPON:
	case CXSLItem::IT_DEFENCE:
		{
			// 아이템 착용 레벨이 캐릭터 레벨보다 11렙 낮으면 먹이로 줄수 없다.
			if( static_cast<int>(cLevel) > pItemTemplet->m_UseLevel )
			{
				if( ( static_cast<int>(cLevel) - pItemTemplet->m_UseLevel ) > CXSLPetManager::PE_PET_FEED_ITEM_LEVEL )
				{
					SET_ERROR( ERR_PET_17 );
					return false;
				}
			}

			// 식사한 만큼 포만도 증가량을 얻자!
			if( SiCXSLPetManager()->CalcIncreaseSatietyForFeed( GetPetID(), iFeedItemID, cLevel, sIncreaseSatiety ) == false )
			{
				START_LOG( cerr, L"장비 먹이 포만도 증가량 얻기 실패!" )
					<< BUILD_LOG( GetPetUID() )
					<< BUILD_LOGc( GetPetID() )
					<< BUILD_LOG( pItemTemplet->m_Price )
					<< BUILD_LOGc( cLevel )
					<< END_LOG;

				SET_ERROR( ERR_PET_11 );
				return false;
			}
			//{{ 2011. 05. 23  김민성	큐피엘 젤리 친밀도 증감
#ifdef SERV_ADD_KEWPIEL_JELLY
			if( SiCXSLItemManager()->IsFantasticKewpielJelly( pItemTemplet->m_ItemID ) == true )
			{
				float fResult = 0.0f;
				float fIncreasRate = SiCXSLPetManager()->GetSpecialFeedIncreaseIntimacyRate();
				iAddIncreaseIntimacy = static_cast<int>( static_cast<double>(GetMaxIntimacy()) * fIncreasRate );
				IncreaseIntimacy( iAddIncreaseIntimacy );
				fResult = fIncreasRate * 100.f;
				iAddIntimacy = static_cast<int>( fResult );

			}
			else if( SiCXSLItemManager()->IsOddKewpielJelly( pItemTemplet->m_ItemID ) == true )
			{
				float fResult = 0.0f;
				float fDecreasRate = SiCXSLPetManager()->GetSpecialFeedDecreaseIntimacyRate();
				iAddIncreaseIntimacy = static_cast<int>( static_cast<double>(GetMaxIntimacy()) * fDecreasRate );
				DecreaseIntimacy( iAddIncreaseIntimacy );
				fResult = fDecreasRate * (-100.f);
				iAddIntimacy = static_cast<int>( fResult );
			}
#endif SERV_ADD_KEWPIEL_JELLY
			//}}
		}
		break;

	case CXSLItem::IT_SPECIAL:
		{
			// 펫 먹이 캐쉬템인지 확인하고 포만도 증가량을 얻자!
			if( SiCXSLPetManager()->GetCashPetFeedItemInfo( iFeedItemID, sIncreaseSatiety ) == false )
			{
				START_LOG( clog, L"캐쉬 먹이 포만도 증가량 얻기 실패! 먹이 아이템이 아닌가?" )
					<< BUILD_LOG( GetPetUID() )
					<< BUILD_LOG( iFeedItemID )
					<< END_LOG;

				SET_ERROR( ERR_PET_18 );
				return false;
			}
		}
		break;

	default:
		START_LOG( clog, L"펫의 먹이로 줄 수 없는 아이템 타입입니다." )
			<< BUILD_LOG( iFeedItemID )
			<< BUILD_LOG( pItemTemplet->m_ItemType )
			<< END_LOG;

		SET_ERROR( ERR_PET_16 );
		return false;
	}	

	// 포만도 올리자!
	IncreaseSatiety( sIncreaseSatiety );

	//////////////////////////////////////////////////////////////////////////	
	// 첫 먹이인지 체크하여 첫먹이라면 친밀도를 2% 올리도록 하자!
	// 펫은 매일 AM 06:00 이후 첫 식사때 포만도 이외에 친밀도가 2% 증가합니다.
	//////////////////////////////////////////////////////////////////////////	

    // 현재 시각 얻기
	CTime tCurTime = CTime::GetCurrentTime();

	// 현재 시각이 첫 먹이 체크 시각보다 크면 첫먹이다!
	if( m_tFirstFeedDate <= tCurTime )
	{
		// 2%만큼 친밀도 증가 시키자!
        const int iIncreaseIntimacy = static_cast<int>( static_cast<double>(GetMaxIntimacy()) * 0.0201 );

        IncreaseIntimacy( iIncreaseIntimacy );

		bIsFirstFeed = true;

		// 먹이 주기가 성공 했으므로 다음 첫 먹이 시각을 저장하자!
		CTime tNextFirstFeedDate = CTime( tCurTime.GetYear(), tCurTime.GetMonth(), tCurTime.GetDay(), 6, 0, 0, 0 );
		if( tNextFirstFeedDate < tCurTime )
		{
			tNextFirstFeedDate += CTimeSpan( 1, 0, 0, 0	);
		}

		m_tFirstFeedDate = tNextFirstFeedDate;
	}
	else
	{
		bIsFirstFeed = false;
	}

	// 현재 포만도와 친밀도 얻기
	sCurrentSatiety = GetSatiety();
	iCurrentIntimacy = GetIntimacy();
    return true;
}

bool KUserPet::CanEvolution()
{
	// 여기서 더 진화할 수 있는지 체크!
	char cNextEvolutionStep = 0;
	if( SiCXSLPetManager()->CheckNextEvolution( GetPetID(), GetEvolutionStep(), cNextEvolutionStep ) == false )
	{
		return false;
	}

	// 친밀도가 100%가 되었을때 진화가 가능하다!
	if( GetIntimacy() != GetMaxIntimacy() )
	{
		return false;
	}

	return true;
}

bool KUserPet::Evolution( OUT KPetInfo& kResultPetInfo, OUT int& iPetCategorySlotSize )
{
	SET_ERROR( NET_OK );

	// 여기서 더 진화할 수 있는지 체크!
	char cNextEvolutionStep = 0;
	if( SiCXSLPetManager()->CheckNextEvolution( GetPetID(), GetEvolutionStep(), cNextEvolutionStep ) == false )
	{
		START_LOG( cerr, L"진화 할 수 없는 진화단계입니다!" )
			<< BUILD_LOG( GetPetUID() )
			<< BUILD_LOGc( GetPetID() )
			<< BUILD_LOGc( GetEvolutionStep() )
			<< END_LOG;

		SET_ERROR( ERR_PET_14 );
		return false;
	}

	// 친밀도가 100%가 되었을때 진화가 가능하다!
	if( GetIntimacy() != GetMaxIntimacy() )
	{
		START_LOG( cerr, L"친밀도가 100%가 아닌데 진화 요청이 들어왔다!" )
			<< BUILD_LOG( GetPetUID() )
			<< BUILD_LOGc( GetPetID() )
			<< BUILD_LOG( GetIntimacy() )
			<< BUILD_LOG( GetMaxIntimacy() )
			<< END_LOG;

		SET_ERROR( ERR_PET_15 );
		return false;
	}

	// 다음 단계로 진화 시키자!
	m_cEvolutionStep = cNextEvolutionStep;

	// 펫 최대 친밀도 재설정
	if( SiCXSLPetManager()->GetEvolutionStepMaxIntimacy( GetPetID(), GetEvolutionStep(), m_iMaxIntimacy ) == false )
	{
		START_LOG( cerr, L"펫의 최대 친밀도값 얻기가 실패하였습니다!" )
			<< BUILD_LOG( GetPetUID() )
			<< BUILD_LOGc( GetPetID() )
			<< BUILD_LOGc( GetEvolutionStep() )
			<< END_LOG;
	}

	// 친밀도를 0으로 초기화 하자!
	if( GetIntimacy() > 0 )
	{
		const int iCurIntimacy = DecreaseIntimacy( GetIntimacy() );
		LIF( iCurIntimacy == 0 );
	}

	// 진화 결과 PetInfo를 얻자!
	GetPetInfo( kResultPetInfo );

	// 펫 인벤토리 크기 얻자!
	iPetCategorySlotSize = GetPetInventorySlotSize();

	// 진화가 되었다면 DB업데이트 예약 걸자!
	m_bReservedDBUpdate = true;
    return true;
}

bool KUserPet::Evolution_Cheat( IN char cEvolutionStep, OUT KPetInfo& kResultPetInfo, OUT int& iPetCategorySlotSize )
{
	// 여기서 더 진화할 수 있는지 체크!
	char cNextEvolutionStep = 0;

	if( cEvolutionStep != 0 )
	{
		if( SiCXSLPetManager()->CheckNextEvolution( GetPetID(), ( cEvolutionStep - 1 ), cNextEvolutionStep ) == false )
		{
			START_LOG( cout, L"진화 할 수 없는 진화 단계 입니다! 개발자님 치트 제대로 써주세요!" )
				<< BUILD_LOG( GetPetUID() )
				<< BUILD_LOGc( GetPetID() )
				<< BUILD_LOGc( cEvolutionStep )
				<< END_LOG;
			return false;
		}
	}

	// 다음 단계로 진화 시키자!
	m_cEvolutionStep = cNextEvolutionStep;

	// 펫 최대 친밀도 재설정
	if( SiCXSLPetManager()->GetEvolutionStepMaxIntimacy( GetPetID(), GetEvolutionStep(), m_iMaxIntimacy ) == false )
	{
		START_LOG( cerr, L"펫의 최대 친밀도값 얻기가 실패하였습니다!" )
			<< BUILD_LOG( GetPetUID() )
			<< BUILD_LOGc( GetPetID() )
			<< BUILD_LOGc( GetEvolutionStep() )
			<< END_LOG;
	}

	// 친밀도를 0으로 초기화 하자!
	if( GetIntimacy() > 0 )
	{
		const int iCurIntimacy = DecreaseIntimacy( GetIntimacy() );
		LIF( iCurIntimacy == 0 );
	}	

	// 진화 결과 PetInfo를 얻자!
	GetPetInfo( kResultPetInfo );

	// 펫 인벤토리 크기 얻자!
	iPetCategorySlotSize = GetPetInventorySlotSize();
	return true;    
}

void KUserPet::CheckExtroversion()
{
	// 로그아웃을 하거나 펫을 소환 해제할때 체크함수를 호출합니다!

	short sChangeExtroversion = 0;
	if( SiCXSLPetManager()->CheckExtroversion( GetPetID(), GetExtroversion(), m_iCumulativeIntimacy, sChangeExtroversion ) == false )
	{
		START_LOG( cerr, L"존재하지 않는 PetID값입니다." )
			<< BUILD_LOG( GetPetUID() )
			<< BUILD_LOGc( GetPetID() )
			<< END_LOG;
		return;
	}

	if( sChangeExtroversion > 0 )
	{
		IncreaseExtroversion( sChangeExtroversion );
	}
	else if( sChangeExtroversion < 0 )
	{
		//IncreaseExtroversion( -sChangeExtroversion );
		DecreaseExtroversion( -sChangeExtroversion );
	}

	// 누적 친밀도는 초기화 시킨다!
	m_iCumulativeIntimacy = 0;
}

void KUserPet::CheckEmotion()
{
	// 펫에게 먹이를 줄때마다 변경되는 포만도를 평균내어 로그아웃 하거나 펫을 소환할때 변경!

	short sChangeEmotion = 0;
	if( SiCXSLPetManager()->CheckEmotion( GetPetID(), GetEmotion(), m_iCumulativeSatiety, m_iFeedCount, sChangeEmotion ) == false )
	{
		START_LOG( cerr, L"존재하지 않는 PetID값입니다." )
			<< BUILD_LOG( GetPetUID() )
			<< BUILD_LOGc( GetPetID() )
			<< END_LOG;
		return;
	}

	if( sChangeEmotion > 0 )
	{
		IncreaseEmotion( sChangeEmotion );
	}
	else if( sChangeEmotion < 0 )
	{
		DecreaseEmotion( -sChangeEmotion );
	}

	// 누적 포만도와 먹이 카운트는 초기화 시킨다!
	m_iCumulativeSatiety = 0;
	m_iFeedCount = 0;
}

#endif SERV_PET_SYSTEM
//}}

//{{ 2011. 12.13    김민성	던전 클리어 시 아이템 지급 이벤트 - 현자의 주문서(중복 지급 금지)
#if defined(SERV_DUNGEON_CLEAR_PAYMENT_ITEM_EVENT) || defined(SERV_DUNGEON_CLEAR_PAYMENT_ITEM_EVENT_EX)
bool KUserPet::ExistItemInInventory( IN int& iItemID )
{
	std::map< UidType, KInventoryItemInfo >::iterator mit =	m_mapPetInventory.begin();
	if( mit == m_mapPetInventory.end() )
	{
		return false;
	}

	for( ; mit != m_mapPetInventory.end() ; ++mit )
	{
		if( iItemID == mit->second.m_kItemInfo.m_iItemID )
		{
			return true;
		}
	}

	return false;
}
#endif // SERV_DUNGEON_CLEAR_PAYMENT_ITEM_EVENT || SERV_DUNGEON_CLEAR_PAYMENT_ITEM_EVENT_EX
//}}

#ifdef SERV_EVENT_VC
bool KUserPet::IncreaseIntimacyByItem( OUT int& iCurrentIntimacy, OUT int& iUpPercent )
{
	// 2%만큼 친밀도 증가 시키자!
	// 나중에 증가값을 아이템별로 다르게 하고 싶으면, 칼럼 추가하고 여기서 수정하자
	int iIncreaseIntimacy = static_cast<int>( static_cast<double>(GetMaxIntimacy()) * 0.0201 );

	if( iIncreaseIntimacy == 0 )
	{
		// 최대 친밀도가 200 이하일수도 있으니 이경우는 100% 상승시켜 준다
		iIncreaseIntimacy = GetMaxIntimacy();
		iUpPercent = 100;
	}
	else
	{
		iUpPercent = 2;
	}

	if( iIncreaseIntimacy != 0 )
	{

		IncreaseIntimacy( iIncreaseIntimacy );

		// 현재 포만도와 친밀도 얻기
		iCurrentIntimacy = GetIntimacy();
		return true;
	}
	else
		return false;
}
#endif //SERV_EVENT_VC