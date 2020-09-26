#include "GSUser.h"
#include ".\userpetmanager.h"
#include "BaseServer.h"
#include "X2Data/XSLPetManager.h"
#include "NetError.h"


//{{ 2010. 07. 21  최육사	펫 시스템
#ifdef SERV_PET_SYSTEM


KUserPetManager::KUserPetManager(void)
{
	Clear();
}

KUserPetManager::~KUserPetManager(void)
{
}

void KUserPetManager::Clear()
{
    m_mapPetList.clear();
	m_vecPetList.clear();
	m_mapCreatePetReserve.clear();
	m_iSummonedPetUID = 0;
	m_iLastSummonedPetUID = 0;
	m_iSummonCount = 0;
}

bool KUserPetManager::AddUserPet( IN const KPetInfo& kPetInfo, IN bool bCreatePet /*= false*/ )
{
	std::map< UidType, std::map< UidType, KInventoryItemInfo > > mapDummy;
    return AddUserPet( kPetInfo, mapDummy, bCreatePet );
}

bool KUserPetManager::AddUserPet( IN const KPetInfo& kPetInfo, IN const std::map< UidType, std::map< UidType, KInventoryItemInfo > >& mapPetItem, IN bool bCreatePet /*= false*/ )
{
	// 펫 객체 생성
	KUserPetPtr spPet( new KUserPet( kPetInfo, mapPetItem, bCreatePet ) );
	if( spPet == NULL )
	{
		START_LOG( cerr, L"펫 포인터 생성 실패! 있을 수 없는 에러!" )
			<< BUILD_LOG( kPetInfo.m_iPetUID )
			<< END_LOG;

		return false;
	}

	// map컨테이너에서 이미 등록된 펫인지 검사
	bool bDelUserPet = false;

	if( m_mapPetList.find( kPetInfo.m_iPetUID ) != m_mapPetList.end() )
	{
		START_LOG( cerr, L"이미 등록된 펫이네? 있을 수 없는 에러!" )
			<< BUILD_LOG( kPetInfo.m_iPetUID )
			<< END_LOG;

		// 좀비 펫일수 있으니 지우자!
		bDelUserPet = true;
	}

	// vector컨테이너에서 이미 등록된 펫인지 검사
	BOOST_TEST_FOREACH( KUserPetPtr, spPet, m_vecPetList )
	{
		if( spPet == NULL )
		{
			START_LOG( cerr, L"펫 포인터 값이 이상합니다." )
				<< END_LOG;
			continue;
		}

		if( spPet->GetPetUID() != kPetInfo.m_iPetUID )
			continue;
		
		bDelUserPet = true;
		break;
	}

	// 좀비 펫일수 있으니 지우자!
	if( bDelUserPet )
	{
		DelUserPet( kPetInfo.m_iPetUID );
	}

#ifdef SERV_PET_SYSTEM_EX1
	// 포만도 max 는 이벤트성 옵션이므로 여기서 templet 통해서 셋팅해준다
	spPet->SetAlwaysMaxSatiety(SiCXSLPetManager()->IsAlwaysMaxSatietyPet( kPetInfo.m_iPetID ));
#endif //SERV_PET_SYSTEM_EX1

	// 컨테이너에 펫 정보 넣자!
	m_mapPetList.insert( std::make_pair( kPetInfo.m_iPetUID, spPet ) );
	m_vecPetList.push_back( spPet );
	return true;
}

bool KUserPetManager::DelUserPet( IN UidType iPetUID )
{
	// map컨테이너에서 등록 안된 펫인지 검사
	if( m_mapPetList.find( iPetUID ) == m_mapPetList.end() )
	{
		START_LOG( cerr, L"등록 안된 펫이네? 있을 수 없는 에러!" )
			<< BUILD_LOG( iPetUID )
			<< END_LOG;
	}

	// vector컨테이너에서 이미 등록된 펫인지 검사
	std::vector< KUserPetPtr >::iterator vit;
	for( vit = m_vecPetList.begin(); vit != m_vecPetList.end(); ++vit )
	{
		if( *vit == NULL )
		{
			START_LOG( cerr, L"펫 포인터 값이 이상합니다." )
				<< END_LOG;
			continue;
		}

		if( (*vit)->GetPetUID() == iPetUID )
		{
			break;
		}
	}

	// 컨테이너에서 펫 정보 삭제하자!
	m_mapPetList.erase( iPetUID );
	m_vecPetList.erase( vit );
	return true;
}

struct PetListSortFunctor
{
	UidType m_iSummonedPetUID;
	PetListSortFunctor( UidType iSummonedPetUID ) { m_iSummonedPetUID = iSummonedPetUID; }
	UidType GetSummonedPetUID() const { return m_iSummonedPetUID; }

	bool operator()( const KUserPetPtr spFirst, const KUserPetPtr spSecond ) const
	{
		// NULL포인터 검사
		if( spFirst == NULL  ||  spSecond == NULL )
			return false;

		// 1. 한번도 소환하지 않은 펫
		if( spFirst->IsNeverSummon() == true  &&  spSecond->IsNeverSummon() == false )
		{
			return true;
		}

		// 2. 소환중인 펫
		if( GetSummonedPetUID() == spFirst->GetPetUID()  &&  GetSummonedPetUID() != spSecond->GetPetUID()  &&
			spFirst->IsNeverSummon() == spSecond->IsNeverSummon() )
		{
			return true;
		}

		// 3. 가장 최근에 소환한 펫
		if( spFirst->GetLastSummonDate() > spSecond->GetLastSummonDate()  &&
			GetSummonedPetUID() != spFirst->GetPetUID()  &&  GetSummonedPetUID() != spSecond->GetPetUID()  &&
			spFirst->IsNeverSummon() == spSecond->IsNeverSummon() )
		{
			return true;
		}

		// 4. 친밀도가 높은 펫
		if( spFirst->GetIntimacy() > spSecond->GetIntimacy()  &&  
			spFirst->GetLastSummonDate() == spSecond->GetLastSummonDate()  &&
			GetSummonedPetUID() != spFirst->GetPetUID()  &&  GetSummonedPetUID() != spSecond->GetPetUID()  &&
			spFirst->IsNeverSummon() == spSecond->IsNeverSummon() )
		{
			return true;
		}

		// 5. 이름 순서 (가나다순)
		if( spFirst->GetPetName() < spSecond->GetPetName()  &&
			spFirst->GetIntimacy() == spSecond->GetIntimacy()  &&
			spFirst->GetLastSummonDate() == spSecond->GetLastSummonDate()  &&
			GetSummonedPetUID() != spFirst->GetPetUID()  &&  GetSummonedPetUID() != spSecond->GetPetUID()  &&
			spFirst->IsNeverSummon() == spSecond->IsNeverSummon() )
		{
			return true;
		}

		return false;
	}
};

void KUserPetManager::PetListSort()
{
	// [참고] 객체를 정렬하기 위해서는 std::stable_sort()함수를 써야함.

	std::stable_sort( m_vecPetList.begin(), m_vecPetList.end(), PetListSortFunctor( GetSummonedPetUID() ) );
}

void KUserPetManager::Init( IN KGSUserPtr spUser, 
						    IN UidType iSummonedPetUID, 
							IN const std::map< UidType, std::map< UidType, KInventoryItemInfo > >& mapPetItem, 
							IN OUT std::vector< KPetInfo >& vecPet,
							OUT int& iPetCategorySlotSize,
							OUT std::map< UidType, KInventoryItemInfo >& mapPetInventory,
							OUT std::vector< UidType >& vecCanEvolutionPetList,
							OUT std::vector< UidType >& vecNeverSummonPetList )
{
	iPetCategorySlotSize = 0;
	mapPetInventory.clear();
	vecCanEvolutionPetList.clear();
	vecNeverSummonPetList.clear();

	BOOST_TEST_FOREACH( const KPetInfo&, kPet, vecPet )
	{
		// 실제 존재하는 PetID인지 검사!
#ifdef SERV_PETID_DATA_TYPE_CHANGE //2013.07.02
		if( SiCXSLPetManager()->IsExistPetID( kPet.m_iPetID ) == false )
		{
			START_LOG( cerr, L"존재하지 않는 PetID가 DB에 있었습니다!" )
				<< BUILD_LOG( kPet.m_iPetUID )
				<< BUILD_LOGc( kPet.m_iPetID )
				<< END_LOG;
			continue;
		}
#else //SERV_PETID_DATA_TYPE_CHANGE
		if( SiCXSLPetManager()->IsExistPetID( kPet.m_cPetID ) == false )
		{
			START_LOG( cerr, L"존재하지 않는 PetID가 DB에 있었습니다!" )
				<< BUILD_LOG( kPet.m_iPetUID )
				<< BUILD_LOGc( kPet.m_cPetID )
				<< END_LOG;
			continue;
		}
#endif //SERV_PETID_DATA_TYPE_CHANGE
		
		// 펫 추가
		LIF( AddUserPet( kPet, mapPetItem ) );
	}

	BOOST_TEST_FOREACH( KUserPetPtr, spPet, m_vecPetList )
	{
		if( spPet == NULL )
			continue;

		// 진화 가능한 펫인지 체크하고 리스트를 만들자!
		if( spPet->CanEvolution() == true )
		{
			vecCanEvolutionPetList.push_back( spPet->GetPetUID() );
		}

		// 소환된 적이 없는지 체크하고 리스트를 만들자!
		if( spPet->IsNeverSummon() == true )
		{
			vecNeverSummonPetList.push_back( spPet->GetPetUID() );
		}
	}
	
	// 클라이언트에게 소환된 펫만 주기 위해 vector를 비우자!
	vecPet.clear();

	// 소환된 펫이 있으면 소환 처리 하자!
	if( iSummonedPetUID > 0 )
	{
		SetSummonedPetUID( iSummonedPetUID );

		KUserPetPtr spSummonedPet = GetSummonedPet();
		if( spSummonedPet == NULL )
		{
			START_LOG( cerr, L"펫 정보 얻을려고 했더니 펫이 없다? 있을 수 없는 에러!" )
				<< BUILD_LOG( GetSummonedPetUID() )
				<< END_LOG;
			return;
		}

		// 펫 소환!
		KPetInfo kSummonedPetInfo;
		spSummonedPet->Summon( kSummonedPetInfo, iPetCategorySlotSize, mapPetInventory );
		vecPet.push_back( kSummonedPetInfo );
	}

	// 펫 리스트 정렬!
	PetListSort();
}

KUserPetPtr KUserPetManager::GetPet( IN UidType iPetUID )
{
	std::map< UidType, KUserPetPtr >::iterator mit;
	mit = m_mapPetList.find( iPetUID );
	if( mit == m_mapPetList.end() )
	{
		START_LOG( cwarn, L"찾는 펫이 없습니다.!" )
			<< BUILD_LOG( iPetUID )
			<< END_LOG;

		return KUserPetPtr();
	}

	if( mit->second == NULL )
	{
		START_LOG( cerr, L"펫 포인터 이상.!" )
			<< BUILD_LOG( iPetUID )
			<< END_LOG;

		return KUserPetPtr();
	}

	return mit->second;
}

bool KUserPetManager::GetPetInfo( IN UidType iPetUID, OUT KPetInfo& kPetInfo )
{
	KUserPetPtr spPet = GetPet( iPetUID );
	if( spPet == NULL )
	{
		START_LOG( cerr, L"보유 하고 있지 않는 PetUID입니다." )
			<< BUILD_LOG( iPetUID )
			<< BUILD_LOG( m_mapPetList.size() )
			<< END_LOG;
		return false;
	}

	spPet->GetPetInfo( kPetInfo );
	return true;
}

void KUserPetManager::SetSummonedPetUID( IN UidType iPetUID )
{
    m_iSummonedPetUID = iPetUID;

	// 소환인 경우!
	if( iPetUID > 0 )
	{
		m_iLastSummonedPetUID = iPetUID;
		++m_iSummonCount;
	}
}

void KUserPetManager::GetSummonedPetInfo( OUT std::vector< KPetInfo >& vecPet )
{
	vecPet.clear();

	if( GetSummonedPetUID() == 0 )
		return;

	KPetInfo kInfo;
	if( GetPetInfo( GetSummonedPetUID(), kInfo ) == false )
	{
		START_LOG( cerr, L"소환된 펫으로 설정된 PetUID가 실제로는 없는 펫이다? 있을 수 없는 에러!" )
			<< BUILD_LOG( GetSummonedPetUID() )
			<< END_LOG;
		return;
	}

	// 현재 소환된 펫을 넣자!
	vecPet.push_back( kInfo );
}

void KUserPetManager::GetSummonedPetInfo( OUT std::vector< KFieldPetInfo >& vecPet )
{
	vecPet.clear();

	if( GetSummonedPetUID() == 0 )
		return;

	KPetInfo kInfo;
	if( GetPetInfo( GetSummonedPetUID(), kInfo ) == false )
	{
		START_LOG( cerr, L"소환된 펫으로 설정된 PetUID가 실제로는 없는 펫이다? 있을 수 없는 에러!" )
			<< BUILD_LOG( GetSummonedPetUID() )
			<< END_LOG;
		return;
	}

	// 현재 소환된 펫을 넣자!
	KFieldPetInfo kFieldPet;
	kFieldPet.m_iPetUID			= kInfo.m_iPetUID;
#ifdef SERV_PETID_DATA_TYPE_CHANGE //2013.07.02
	kFieldPet.m_iPetID			= kInfo.m_iPetID;
#else //SERV_PETID_DATA_TYPE_CHANGE
	kFieldPet.m_cPetID			= kInfo.m_cPetID;
#endif //SERV_PETID_DATA_TYPE_CHANGE
	kFieldPet.m_wstrPetName		= kInfo.m_wstrPetName;
	kFieldPet.m_cEvolutionStep	= kInfo.m_cEvolutionStep;
	kFieldPet.m_iIntimacy		= kInfo.m_iIntimacy;
	vecPet.push_back( kFieldPet );
}

bool KUserPetManager::GetPetInventory( IN UidType iPetUID, OUT std::map< UidType, KInventoryItemInfo >& mapPetItem )
{
	KUserPetPtr spPet = GetPet( iPetUID );
	if( spPet == NULL )
	{
		START_LOG( cerr, L"존재 하지 않는 펫입니다!" )
			<< BUILD_LOG( iPetUID )
			<< END_LOG;

		return false;
	}

	spPet->GetPetInventory( mapPetItem );
	return true;
}

void KUserPetManager::GetDBUpdatePetInfo( OUT std::vector< KPetInfo >& vecPetInfo )
{
	vecPetInfo.clear();

	std::map< UidType, KUserPetPtr >::const_iterator mit;
	for( mit = m_mapPetList.begin(); mit != m_mapPetList.end(); ++mit )
	{
		if( mit->second == NULL )
		{
			START_LOG( cerr, L"펫 포인터값이 이상합니다." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		// 소환된적이 한번도 없으면 가져오지 말자!
		if( mit->second->IsReservedDBUpdate() == false )		
			continue;

		KPetInfo kInfo;
		mit->second->GetDBUpdatePetInfo( kInfo );
		vecPetInfo.push_back( kInfo );
	}
}

void KUserPetManager::RollBackDBUpdatePetInfo( IN const std::vector< KPetInfo >& vecPetInfo )
{
	BOOST_TEST_FOREACH( const KPetInfo&, kPet, vecPetInfo )
	{
        KUserPetPtr spPet = GetPet( kPet.m_iPetUID );
		if( spPet == NULL )
			continue;

		// DB업데이트 실패로 인한 증분값 롤백!
		spPet->RollBackDBUpdatePetInfo( kPet );
	}
}

bool KUserPetManager::IsExistPet( IN const std::wstring& wstrPetName ) const
{
	BOOST_MAP_CONST_FOREACH( UidType, KUserPetPtr, kPet, m_mapPetList )
	{
		if( kPet.second == NULL )
			continue;

        if( kPet.second->GetPetName() == wstrPetName )
			return true;
	}

	return false;
}

//{{ 2010. 10. 18	최육사	펫 오라 스킬
#ifdef SERV_PET_AURA_SKILL
bool KUserPetManager::IsSummonedPetHungry()
{
	if( GetSummonedPetUID() == 0 )
		return false;

    KUserPetPtr spPet = GetSummonedPet();
	if( spPet == NULL )
	{
		START_LOG( cerr, L"소환된 펫이 없는데 배고픈 상태인지 체크하네?" )
			<< BUILD_LOG( GetSummonedPetUID() )
			<< END_LOG;
		return false;
	}
	
	return spPet->IsHungry();
}
#endif SERV_PET_AURA_SKILL
//}}

bool KUserPetManager::UpdatePetInventory( IN UidType iPetUID, IN const std::map< UidType, KInventoryItemInfo >& mapPetItem )
{
	KUserPetPtr spPet = GetPet( iPetUID );
	if( spPet == NULL )
	{
		START_LOG( cerr, L"펫 정보를 업데이트 하려고 하는데 펫이 존재 하지 않습니다!" )
			<< BUILD_LOG( iPetUID )
			<< END_LOG;

		return false;
	}	

	// 펫 정보 업데이트
	spPet->UpdatePetInventory( mapPetItem );
	return true;
}

void KUserPetManager::OnDeselectUnit( IN KGSUserPtr spUser )
{
	// 소환된 펫이 있나?
	if( GetSummonedPetUID() == 0 )
		return;

    // 소환된 펫 찾기!
	KUserPetPtr spSummonedPet = GetSummonedPet();
	if( spSummonedPet == NULL )
	{
		START_LOG( cerr, L"소환된 PetUID값은 있는데 펫 정보는 없네? 일어나면 안되는 에러!" )
			<< BUILD_LOG( GetSummonedPetUID() )
			<< END_LOG;
		return;
	}

	// 캐릭터 선택 해제시 처리되는 함수 호출!
	spSummonedPet->Unsummon();

	// 펫 소환 통계
	{
		CTime tCurTime = CTime::GetCurrentTime();

		KE_LOCAL_LOG_PET_SUMMON_NOT kNot;
		kNot.m_wstrNickName		= spUser->GetCharName();
		kNot.m_ucLevel			= spUser->GetLevel();
		kNot.m_iPetUID			= spSummonedPet->GetPetUID();
#ifdef SERV_PETID_DATA_TYPE_CHANGE //2013.07.02
		kNot.m_iPetID			= spSummonedPet->GetPetID();
#else //SERV_PETID_DATA_TYPE_CHANGE
		kNot.m_cPetID			= spSummonedPet->GetPetID();
#endif //SERV_PETID_DATA_TYPE_CHANGE
		kNot.m_cEvolutionStep	= spSummonedPet->GetEvolutionStep();
		kNot.m_iIntimacy		= spSummonedPet->GetIntimacy();
		kNot.m_sExtroversion	= spSummonedPet->GetExtroversion();
		kNot.m_sEmotion			= spSummonedPet->GetEmotion();
		kNot.m_wstrRegDate		= (std::wstring)tCurTime.Format( _T("%Y-%m-%d %H:%M:%S") );
		KSIManager.QueueingEvent( E_LOCAL_LOG_PET_SUMMON_NOT, kNot );
	}
}

//{{ 2012. 04. 26	박세훈	펫 장난감
#ifdef SERV_PET_TOY
bool KUserPetManager::DecreaseSatietyOnDungeonRoom( IN char cUserCount, IN short sSubStageExpNpcNum, IN const bool& bHaveToy, OUT KEGS_CHANGED_PET_SATIETY_INTIMACY_NOT& kNot )
#else
bool KUserPetManager::DecreaseSatietyOnDungeonRoom( IN char cUserCount, IN short sSubStageExpNpcNum, OUT KEGS_CHANGED_PET_SATIETY_INTIMACY_NOT& kNot )
#endif SERV_PET_TOY
//}}
{
	// 소환된 펫이 없다면 패스!
	if( GetSummonedPetUID() == 0 )
		return false;

	// 경험치를 가진 몬스터 수가 0이면 패스!
	if( sSubStageExpNpcNum == 0 )
		return false;

	// 소환된 펫 정보 얻기!
	KUserPetPtr spSummonedPet = GetSummonedPet();
	if( spSummonedPet == NULL )
	{
		START_LOG( cerr, L"펫 정보를 찾을 수 없습니다!" )
			<< BUILD_LOG( GetSummonedPetUID() )
			<< END_LOG;
		return false;
	}

	// 포만도 깎자!

	//{{ 2012. 04. 26	박세훈	펫 장난감
#ifdef SERV_PET_TOY
	spSummonedPet->DecreaseSatietyOnDungeonRoom( cUserCount, sSubStageExpNpcNum, bHaveToy );
#else
	spSummonedPet->DecreaseSatietyOnDungeonRoom( cUserCount, sSubStageExpNpcNum );
#endif SERV_PET_TOY
	//}}

	// 결과 받자!
	kNot.m_iPetUID = GetSummonedPetUID();
	kNot.m_sCurrentSatiety = spSummonedPet->GetSatiety();
	kNot.m_iCurrentIntimacy = spSummonedPet->GetIntimacy();
	return true;
}

//{{ 2012. 04. 26	박세훈	펫 장난감
#ifdef SERV_PET_TOY
bool KUserPetManager::DecreaseSatietyOnPvpRoom( IN char cUserCount, IN const bool& bHaveToy, OUT KEGS_CHANGED_PET_SATIETY_INTIMACY_NOT& kNot )
#else
bool KUserPetManager::DecreaseSatietyOnPvpRoom( IN char cUserCount, OUT KEGS_CHANGED_PET_SATIETY_INTIMACY_NOT& kNot )
#endif SERV_PET_TOY
//}}
{
	// 소환된 펫이 없다면 패스!
	if( GetSummonedPetUID() == 0 )
		return false;

	// 소환된 펫 정보 얻기!
	KUserPetPtr spSummonedPet = GetSummonedPet();
	if( spSummonedPet == NULL )
	{
		START_LOG( cerr, L"펫 정보를 찾을 수 없습니다!" )
			<< BUILD_LOG( GetSummonedPetUID() )
			<< END_LOG;
		return false;
	}

	// 포만도 깎자!

	//{{ 2012. 04. 26	박세훈	펫 장난감
#ifdef SERV_PET_TOY
	spSummonedPet->DecreaseSatietyOnPvpRoom( cUserCount, bHaveToy );
#else
	spSummonedPet->DecreaseSatietyOnPvpRoom( cUserCount );
#endif SERV_PET_TOY
	//}}

	// 결과 받자!
	kNot.m_iPetUID = GetSummonedPetUID();
	kNot.m_sCurrentSatiety = spSummonedPet->GetSatiety();
	kNot.m_iCurrentIntimacy = spSummonedPet->GetIntimacy();
	return true;
}

//{{ 2011. 05. 23  김민성	큐피엘 젤리 친밀도 증감
#ifdef SERV_ADD_KEWPIEL_JELLY
bool KUserPetManager::CheckAutoFeed( IN KGSUserPtr spUser, OUT bool& bIsFirstFeed, OUT KEGS_CHANGED_PET_SATIETY_NOT& kNotSatiety, OUT KEGS_CHANGED_PET_INTIMACY_NOT& kNotIntimacy, OUT KEGS_AUTO_FEED_NOT& kNotAutoFeed, OUT int& iAddIntimacy )
#else
bool KUserPetManager::CheckAutoFeed( IN KGSUserPtr spUser, OUT bool& bIsFirstFeed, OUT KEGS_CHANGED_PET_SATIETY_NOT& kNotSatiety, OUT KEGS_CHANGED_PET_INTIMACY_NOT& kNotIntimacy, OUT KEGS_AUTO_FEED_NOT& kNotAutoFeed )
#endif SERV_ADD_KEWPIEL_JELLY
//}}
{
	// 1. 소환된 펫이 없다면 그냥 리턴!
	if( GetSummonedPetUID() == 0 )
		return false;

	// 2. 소환된 펫 정보 얻기!
	KUserPetPtr spSummonedPet = GetSummonedPet();
	if( spSummonedPet == NULL )
	{
		START_LOG( cerr, L"펫 정보를 찾을 수 없습니다!" )
			<< BUILD_LOG( GetSummonedPetUID() )
			<< END_LOG;
		return false;
	}

	// 3. 자동 먹이 기능이 있는지 체크!
	if( spSummonedPet->IsAutoFeed() == false )
	{
		return false;
	}

	// 4. 펫이 지금 배부른 상태인지 체크! 자동먹이는 80퍼센트!
	if( spSummonedPet->IsPetStuffed( 80.f ) == true )
	{
		return false;
	}

	// 5. 펫 인벤토리를 뒤져서 먹을만한게 있는지 찾아보자!
	if( spUser->AutoFeedInPetInventory( spSummonedPet->IsPetSpecialFeed(), kNotAutoFeed.m_iFeedItemID, kNotAutoFeed.m_vecPetInventorySlotInfo ) == false )
	{
		return false;
	}
#ifdef SERV_EVENT_PET_INVENTORY
	///이벤트 먹이는 자동먹이 기능으로 사용 할 수 없다.
	if( kNotAutoFeed.m_iFeedItemID == 141000440 )
	{
		return false;
	}
#endif SERV_EVENT_PET_INVENTORY
	// 6. 먹이를 먹자!
	//{{ 2011. 05. 23  김민성	큐피엘 젤리 친밀도 증감
#ifdef SERV_ADD_KEWPIEL_JELLY
	iAddIntimacy = 0;
	if( spSummonedPet->Feed( spUser->GetLevel(), kNotAutoFeed.m_iFeedItemID, bIsFirstFeed, kNotSatiety.m_sCurrentSatiety, kNotIntimacy.m_iCurrentIntimacy, iAddIntimacy ) == false )
#else
	if( spSummonedPet->Feed( spUser->GetLevel(), kNotAutoFeed.m_iFeedItemID, bIsFirstFeed, kNotSatiety.m_sCurrentSatiety, kNotIntimacy.m_iCurrentIntimacy ) == false )
#endif SERV_ADD_KEWPIEL_JELLY
	//}}
	{
		START_LOG( cerr, L"먹이 먹기 실패! 미리 체크 했는데 에러가 뜨네! 절대 일어나면 안되는 에러!" )
			<< BUILD_LOG( NetError::GetLastErrMsg() ) 
			<< BUILD_LOGc( spUser->GetLevel() )
			<< BUILD_LOG( kNotAutoFeed.m_iFeedItemID )
			<< END_LOG;
		return false;
	}

	// 7. 결과 정보를 담자!
	kNotSatiety.m_iUnitUID = spUser->GetCharUID();
	kNotSatiety.m_iPetUID = GetSummonedPetUID();
	return true;
}

//{{ 2010. 10. 18	최육사	펫 오라 스킬
#ifdef SERV_PET_AURA_SKILL
//{{ 2011. 07. 08    김민성    옵션 수치화
#ifdef SERV_USE_PERCENT_IN_OPTION_DATA
void KUserPetManager::GetStatByPetSkill( IN bool bIsDungeon, IN const KStat& kBaseStat, IN OUT float& fTotalIncHPRate, OUT KStat& kAddStat, IN unsigned int& uiHP_OnePoint, IN unsigned int& uiUnitLevel )
#else
void KUserPetManager::GetStatByPetSkill( IN bool bIsDungeon, IN const KStat& kBaseStat, IN OUT float& fTotalIncHPRate, OUT KStat& kAddStat )
#endif SERV_USE_PERCENT_IN_OPTION_DATA
//}} 
{
	kAddStat.Init();

	if( GetSummonedPetUID() == 0 )
		return;

	KUserPetPtr spPet = GetSummonedPet();
	if( spPet == NULL )
	{
		START_LOG( cerr, L"소환된 펫UI는 있는데 실제 펫 정보는 없다?" )
			<< BUILD_LOG( GetSummonedPetUID() )
			<< END_LOG;
		return;
	}

	//{{ 2011. 07. 08    김민성    옵션 수치화
#ifdef SERV_USE_PERCENT_IN_OPTION_DATA
	spPet->GetStatByPetSkill( bIsDungeon, kBaseStat, fTotalIncHPRate, kAddStat, uiHP_OnePoint, uiUnitLevel );
#else
	spPet->GetStatByPetSkill( bIsDungeon, kBaseStat, fTotalIncHPRate, kAddStat );
#endif SERV_USE_PERCENT_IN_OPTION_DATA
	//}} 
}

void KUserPetManager::GetStatIncreaseRateByPetSkill( IN bool bIsDungeon, IN OUT KStatIncreaseRate& kStatIncreaseRateInfo )
{
	if( GetSummonedPetUID() == 0 )
		return;

	KUserPetPtr spPet = GetSummonedPet();
	if( spPet == NULL )
	{
		START_LOG( cerr, L"소환된 펫UI는 있는데 실제 펫 정보는 없다?" )
			<< BUILD_LOG( GetSummonedPetUID() )
			<< END_LOG;
		return;
	}

	spPet->GetStatIncreaseRateByPetSkill( bIsDungeon, kStatIncreaseRateInfo );
}

#endif SERV_PET_AURA_SKILL
//}}

bool KUserPetManager::Handler_EGS_CREATE_PET_REQ( IN const KEGS_CREATE_PET_REQ& kReq, IN int iItemID, OUT KDBE_CREATE_PET_REQ& kPacketToDB )
{
	SET_ERROR( NET_OK );

	// 1. 펫 이름 중복 체크
	if( IsExistPet( kReq.m_wstrPetName ) == true )
	{
		SET_ERROR( ERR_PET_01 );
		return false;
	}

	// 2. 펫 이름 글자 길이 체크
	std::string strPetName = KncUtil::toNarrowString( kReq.m_wstrPetName );
	if( strPetName.size() > 12 )
	{
		SET_ERROR( ERR_PET_02 );
		return false;
	}

	// 3. 사용한 아이템이 펫 생성 아이템이 맞는가?
#ifdef SERV_PERIOD_PET
	CXSLPetManager::PET_UNIT_ID ePetID;
	SiCXSLPetManager()->GetPetIDByItemID( iItemID, ePetID, kPacketToDB.m_sPeriod );
#else SERV_PERIOD_PET
	const CXSLPetManager::PET_UNIT_ID ePetID = SiCXSLPetManager()->GetPetIDByItemID( iItemID );
#endif SERV_PERIOD_PET

	if( ePetID == CXSLPetManager::PUI_NONE )
	{
		START_LOG( cerr, L"펫 생성 아이템도 아니면서 사용하려고 했다!" )
			<< BUILD_LOG( kReq.m_iItemUID )
			<< BUILD_LOG( iItemID )
			<< END_LOG;

		SET_ERROR( ERR_PET_00 );
		return false;
	}

	// 4. 이미 생성 예약된 펫이 있는가?
	if( m_mapCreatePetReserve.empty() == false )
	{
		SET_ERROR( ERR_PET_03 );
		return false;
	}

	// 5. 새로운 펫 정보 생성!
	if( SiCXSLPetManager()->MakeNewPetInfo( ePetID, kReq.m_wstrPetName, kPacketToDB.m_kPetInfo ) == false )
	{
		START_LOG( cerr, L"새로운 펫 정보 생성 실패!" )
			<< BUILD_LOG( ePetID )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( kReq.m_wstrPetName )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;

		SET_ERROR( ERR_PET_00 );
        return false;
	}

	// DB에 들어가기 전에 예약 리스트에 미리 넣어두자!
	m_mapCreatePetReserve.insert( std::make_pair( kPacketToDB.m_kPetInfo.m_wstrPetName, kPacketToDB.m_kPetInfo ) );
	return true;
}

bool KUserPetManager::Handler_DBE_CREATE_PET_ACK( IN const KDBE_CREATE_PET_ACK& kAck )
{
	SET_ERROR( NET_OK );

	//////////////////////////////////////////////////////////////////////////	
	// 예약된 펫 정보가 있는지 확인
	if( m_mapCreatePetReserve.empty() == true )
	{
		START_LOG( cerr, L"예약된 펫 정보가 없는데 펫 생성 결과처리를 하네? 있을 수 없는 에러!" )
			<< BUILD_LOG( kAck.m_kPetInfo.m_iPetUID )
#ifdef SERV_PETID_DATA_TYPE_CHANGE //2013.07.02
			<< BUILD_LOGc( kAck.m_kPetInfo.m_iPetID )
#else //SERV_PETID_DATA_TYPE_CHANGE
			<< BUILD_LOGc( kAck.m_kPetInfo.m_cPetID )
#endif //SERV_PETID_DATA_TYPE_CHANGE
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( kAck.m_kPetInfo.m_wstrPetName )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOGc( kAck.m_kPetInfo.m_cEvolutionStep )
			<< END_LOG;

		// 그냥 넘어 가자!
	}

	// 예약된 펫이 맞는지 확인하자!
	if( m_mapCreatePetReserve.find( kAck.m_kPetInfo.m_wstrPetName ) == m_mapCreatePetReserve.end() )
	{
		START_LOG( cerr, L"예약된 펫은 있는데 생성요청했던 펫의 닉네임은 없네? 있을 수 없는 에러!" )
			<< BUILD_LOG( kAck.m_kPetInfo.m_iPetUID )
#ifdef SERV_PETID_DATA_TYPE_CHANGE //2013.07.02
			<< BUILD_LOGc( kAck.m_kPetInfo.m_iPetID )
#else //SERV_PETID_DATA_TYPE_CHANGE
			<< BUILD_LOGc( kAck.m_kPetInfo.m_cPetID )
#endif //SERV_PETID_DATA_TYPE_CHANGE
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( kAck.m_kPetInfo.m_wstrPetName )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOGc( kAck.m_kPetInfo.m_cEvolutionStep )
			<< END_LOG;

		// 그냥 넘어 가자!
	}

	// 예약된 정보에 대한 검증을 마쳤으니 예약리스트를 비웁시다
	m_mapCreatePetReserve.clear();
	
	//////////////////////////////////////////////////////////////////////////	
    // 혹시나 해서 검사해봄
	if( IsExistPet( kAck.m_kPetInfo.m_iPetUID ) == true )
	{
		START_LOG( cerr, L"똑같은 PetUID가 발급 되었네? 아니면 두번 업데이트 된건가? 있을 수 없는 에러!" )
			<< BUILD_LOG( kAck.m_kPetInfo.m_iPetUID )
#ifdef SERV_PETID_DATA_TYPE_CHANGE //2013.07.02
			<< BUILD_LOGc( kAck.m_kPetInfo.m_iPetID )
#else //SERV_PETID_DATA_TYPE_CHANGE
			<< BUILD_LOGc( kAck.m_kPetInfo.m_cPetID )
#endif //SERV_PETID_DATA_TYPE_CHANGE
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( kAck.m_kPetInfo.m_wstrPetName )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOGc( kAck.m_kPetInfo.m_cEvolutionStep )
			<< END_LOG;

		SET_ERROR( ERR_PET_00 );
		return false;
	}

	// 혹시나 해서 검사해봄
	if( IsExistPet( kAck.m_kPetInfo.m_wstrPetName ) == true )
	{
		START_LOG( cerr, L"똑같은 펫 이름이 있네? 아니면 두번 업데이트 된건가? 있을 수 없는 에러!" )
			<< BUILD_LOG( kAck.m_kPetInfo.m_iPetUID )
#ifdef SERV_PETID_DATA_TYPE_CHANGE //2013.07.02
			<< BUILD_LOGc( kAck.m_kPetInfo.m_iPetID )
#else //SERV_PETID_DATA_TYPE_CHANGE
			<< BUILD_LOGc( kAck.m_kPetInfo.m_cPetID )
#endif //SERV_PETID_DATA_TYPE_CHANGE
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( kAck.m_kPetInfo.m_wstrPetName )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOGc( kAck.m_kPetInfo.m_cEvolutionStep )
			<< END_LOG;

		SET_ERROR( ERR_PET_00 );
		return false;
	}

	// 펫 생성!
	LIF( AddUserPet( kAck.m_kPetInfo, true ) );

	// 펫 리스트 정렬!
	PetListSort();
	return true;
}

void KUserPetManager::Handler_EGS_GET_PET_LIST_REQ( IN const KEGS_GET_PET_LIST_REQ& kReq, OUT KEGS_GET_PET_LIST_ACK& kAck ) const
{
	kAck.m_vecPetList.clear();
	kAck.m_nViewPage = kReq.m_nViewPage;

	_JIF( kReq.m_nViewPage > 0, return; );

	if( m_vecPetList.empty() )
	{
		kAck.m_nViewPage = 1;
		kAck.m_nTotalPage = 1;
		return;
	}

	kAck.m_nTotalPage = m_vecPetList.size() / PE_PAGE_PER_COUNT;
	if( ( m_vecPetList.size() % PE_PAGE_PER_COUNT ) > 0 )
	{
		++kAck.m_nTotalPage;
	}

	u_int uiBeginIndex = ( kAck.m_nViewPage * PE_PAGE_PER_COUNT ) - PE_PAGE_PER_COUNT;
	if( m_vecPetList.size() <= uiBeginIndex )
	{
		kAck.m_nViewPage = kAck.m_nTotalPage; // 마지막 페이지
		uiBeginIndex = ( kAck.m_nViewPage * PE_PAGE_PER_COUNT ) - PE_PAGE_PER_COUNT;

		if( m_vecPetList.size() <= uiBeginIndex )
		{
			START_LOG( cerr, L"일어날 수 없는 에러!" )
				<< BUILD_LOG( kAck.m_nViewPage )
				<< BUILD_LOG( kAck.m_nTotalPage )
				<< BUILD_LOG( uiBeginIndex )
				<< BUILD_LOG( m_vecPetList.size() )
				<< END_LOG;
			return;
		}
	}

	std::vector< KUserPetPtr >::const_iterator vit = m_vecPetList.begin();
	vit += uiBeginIndex;

	while( vit != m_vecPetList.end() )
	{
		if( (*vit) == NULL )
			continue;

		KPetInfo kInfo;
		(*vit)->GetPetInfo( kInfo );

		kAck.m_vecPetList.push_back( kInfo );

		if( kAck.m_vecPetList.size() >= PE_PAGE_PER_COUNT )
			break;

		++vit;
	}
}

bool KUserPetManager::Handler_EGS_SUMMON_PET_REQ( IN KGSUserPtr spUser, IN const KEGS_SUMMON_PET_REQ& kReq, OUT KDBE_SUMMON_PET_REQ& kPacketToDB )
{
	SET_ERROR( NET_OK );

	// SummonPetUID값이 0이면 소환 해제를 하려는것이다!
	if( kReq.m_iSummonPetUID == 0 )
	{
		// 이미 소환된 펫이 있는지 확인한다.
		if( GetSummonedPetUID() == 0 )
		{
			START_LOG( cerr, L"소환된 펫이 없는데 뭘 소환해제할려는거냐!" )
				<< BUILD_LOG( GetSummonedPetUID() )
				<< END_LOG;

			SET_ERROR( ERR_PET_06 );
			return false;
		}
	}
	// SummonPetUID값이 0이 아니면 소환을 하려고 하는 것이다!
	else
	{
		// 이미 소환된 펫을 다시 소환하려고 하는게 아닌지 확인한다.
		if( GetSummonedPetUID() == kReq.m_iSummonPetUID )
		{
			START_LOG( cerr, L"이미 소환되어 있는 펫인데 또 소환하려고 한다!" )
				<< BUILD_LOG( GetSummonedPetUID() )
				<< END_LOG;

			SET_ERROR( ERR_PET_07 );
			return false;
		}
	}

	// 이전에 소환된 펫이 있었다면 소환 해제 시키자!
	kPacketToDB.m_iBeforeSummonPetUID = GetSummonedPetUID();
	if( kPacketToDB.m_iBeforeSummonPetUID != 0 )
	{
		// 소환 해제!
		KUserPetPtr spSummonedPet = GetSummonedPet();
		if( spSummonedPet == NULL )
		{
			START_LOG( cerr, L"소환 PetUID는 존재하는데 펫 정보는 없다? 있을 수 없는 에러!" )
				<< BUILD_LOG( GetSummonedPetUID() )
				<< END_LOG;

			SET_ERROR( ERR_PET_04 );
			return false;
		}

		// 펫 소환 해제 처리!
        spSummonedPet->Unsummon();

		// 펫 소환 통계
		{
			CTime tCurTime = CTime::GetCurrentTime();

			KE_LOCAL_LOG_PET_SUMMON_NOT kNot;
			kNot.m_wstrNickName		= spUser->GetCharName();
			kNot.m_ucLevel			= spUser->GetLevel();
			kNot.m_iPetUID			= spSummonedPet->GetPetUID();
#ifdef SERV_PETID_DATA_TYPE_CHANGE //2013.07.02
			kNot.m_iPetID			= spSummonedPet->GetPetID();
#else //SERV_PETID_DATA_TYPE_CHANGE
			kNot.m_cPetID			= spSummonedPet->GetPetID();
#endif //SERV_PETID_DATA_TYPE_CHANGE
			kNot.m_cEvolutionStep	= spSummonedPet->GetEvolutionStep();
			kNot.m_iIntimacy		= spSummonedPet->GetIntimacy();
			kNot.m_sExtroversion	= spSummonedPet->GetExtroversion();
			kNot.m_sEmotion			= spSummonedPet->GetEmotion();
			kNot.m_wstrRegDate		= (std::wstring)tCurTime.Format( _T("%Y-%m-%d %H:%M:%S") );
			KSIManager.QueueingEvent( E_LOCAL_LOG_PET_SUMMON_NOT, kNot );
		}
	}

	// 소환하러 가기전에 소환할 PetUID 또는 소환해제값을 업데이트
	kPacketToDB.m_iSummonPetUID = kReq.m_iSummonPetUID;
	SetSummonedPetUID( kReq.m_iSummonPetUID );
	return true;
}

bool KUserPetManager::Handler_DBE_SUMMON_PET_ACK( IN const KDBE_SUMMON_PET_ACK& kAck, OUT KPetInfo& kSummonedPetInfo, OUT int& iPetCategorySlotSize, OUT std::map< UidType, KInventoryItemInfo >& mapPetItem )
{
	SET_ERROR( NET_OK );

	iPetCategorySlotSize = 0;
	mapPetItem.clear();

	if( kAck.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"DB에서 펫 소환 처리 실패!" )
			<< BUILD_LOG( NetError::GetErrStr( kAck.m_iOK ) )
			<< BUILD_LOG( kAck.m_iSummonPetUID )
			<< END_LOG;

		SET_ERROR( ERR_PET_04 );
		goto end_proc;
	}

    // 소환하려던 펫이 맞는지 검사!
	if( GetSummonedPetUID() != kAck.m_iSummonPetUID )
	{
		START_LOG( cerr, L"DB업데이트 하러 갈때 저장했던 PutUID와 다르다!? 일어나면 안되는 없는에러!" )
			<< BUILD_LOG( GetSummonedPetUID() )
			<< BUILD_LOG( kAck.m_iSummonPetUID )
			<< END_LOG;

		// 덮어 씌우자!
		SetSummonedPetUID( kAck.m_iSummonPetUID );
	}

	// 펫 소환하기!
	if( kAck.m_iSummonPetUID > 0 )
	{
		KUserPetPtr spSummonedPet = GetSummonedPet();
		if( spSummonedPet == NULL )
		{
			START_LOG( cerr, L"펫 정보 얻을려고 했더니 펫이 없다? 있을 수 없는 에러!" )
				<< BUILD_LOG( GetSummonedPetUID() )
				<< END_LOG;

			SET_ERROR( ERR_PET_04 );
			goto end_proc;
		}

		// 펫 소환!
		spSummonedPet->Summon( kSummonedPetInfo, iPetCategorySlotSize, mapPetItem );

		// 펫 리스트 정렬!
		PetListSort();
	}
	
    return true;

end_proc:
	// 소환 실패했다면 현재 소환 상태를 초기화 하자!
	SetSummonedPetUID( 0 );
	return false;
}

bool KUserPetManager::Handler_EGS_COMMANDS_FOR_PETS_REQ( IN KGSUserPtr spUser, IN const KEGS_COMMANDS_FOR_PETS_REQ& kReq, OUT CXSLPetManager::PET_ACTION_COMMAND& ePetActionResult )
{
	SET_ERROR( NET_OK );

	// 소환된 펫이 있는지 체크!
	if( GetSummonedPetUID() == 0 )
	{
		START_LOG( cerr, L"소환된 펫이 없는데 명령을 내렸다! 일어나서는 안되는 에러!" )
			<< BUILD_LOG( spUser->GetCharUID() )
			<< BUILD_LOGc( kReq.m_cActionType )
			<< BUILD_LOG( m_mapPetList.size() )
			<< END_LOG;

		SET_ERROR( ERR_PET_06 );
		return false;
	}

	// 소환된 펫을 찾자!
	KUserPetPtr spSummonedPet = GetSummonedPet();
	if( spSummonedPet == NULL )
	{
		START_LOG( cerr, L"소환된 펫 정보가 존재하지 않습니다! 일어나서는 안되는 에러!" )
			<< BUILD_LOG( GetSummonedPetUID() )
			<< BUILD_LOGc( kReq.m_cActionType )
			<< END_LOG;

		SET_ERROR( ERR_PET_06 );
		return false;
	}

	// 펫에게 명령을 내리자!
	if( spSummonedPet->CommandsForPet( static_cast<CXSLPetManager::PET_ACTION_COMMAND>(kReq.m_cActionType), ePetActionResult ) == false )
	{
		START_LOG( cerr, L"펫에게 명령 주기가 실패하였습니다!" )
			<< BUILD_LOG( GetSummonedPetUID() )
			<< BUILD_LOGc( kReq.m_cActionType )
			<< END_LOG;

		SET_ERROR( ERR_PET_09 );
		return false;
	}

    return true;
}

//{{ 2011. 05. 23  김민성	큐피엘 젤리 친밀도 증감
#ifdef SERV_ADD_KEWPIEL_JELLY
bool KUserPetManager::Handler_EGS_FEED_PETS_REQ( IN char cLevel, IN int iFeedItemID, OUT bool& bIsFirstFeed, OUT KEGS_CHANGED_PET_SATIETY_NOT& kNotSatiey, OUT KEGS_CHANGED_PET_INTIMACY_NOT& kNotIntimacy, OUT int& iAddIntimacy )
#else
bool KUserPetManager::Handler_EGS_FEED_PETS_REQ( IN char cLevel, IN int iFeedItemID, OUT bool& bIsFirstFeed, OUT KEGS_CHANGED_PET_SATIETY_NOT& kNotSatiey, OUT KEGS_CHANGED_PET_INTIMACY_NOT& kNotIntimacy )
#endif SERV_ADD_KEWPIEL_JELLY
//}}
{
	SET_ERROR( NET_OK );

    // 현재 소환된 펫이 있는지 체크!
	if( GetSummonedPetUID() == 0 )
	{
		START_LOG( cerr, L"소환된 펫이 없는데 먹이를 주려고 하네? 있을 수 없는에러!" )
			<< BUILD_LOG( iFeedItemID )
			<< END_LOG;

		SET_ERROR( ERR_PET_06 );
		return false;
	}

	// 소환된 펫 정보얻기!
	KUserPetPtr spSummonedPet = GetSummonedPet();
	if( spSummonedPet == NULL )
	{
		START_LOG( cerr, L"소환 펫의 PetUID값은 있는데 펫 정보는 없네?" )
			<< BUILD_LOG( GetSummonedPetUID() )
			<< END_LOG;

		SET_ERROR( ERR_PET_06 );
		return false;
	}

	// 먹이 주기!
	//{{ 2011. 05. 23  김민성	큐피엘 젤리 친밀도 증감
#ifdef SERV_ADD_KEWPIEL_JELLY
	if( spSummonedPet->Feed( cLevel, iFeedItemID, bIsFirstFeed, kNotSatiey.m_sCurrentSatiety, kNotIntimacy.m_iCurrentIntimacy, iAddIntimacy ) == false )
#else
	if( spSummonedPet->Feed( cLevel, iFeedItemID, bIsFirstFeed, kNotSatiey.m_sCurrentSatiety, kNotIntimacy.m_iCurrentIntimacy ) == false )
#endif SERV_ADD_KEWPIEL_JELLY
	//}}
	{
		// NetError::GetLastError()
		return false;
	}

	kNotSatiey.m_iPetUID = GetSummonedPetUID();
	kNotIntimacy.m_iPetUID = GetSummonedPetUID();
	return true;
}

bool KUserPetManager::Handler_EGS_SET_AUTO_FEED_PETS_REQ( IN const KEGS_SET_AUTO_FEED_PETS_REQ& kReq )
{
	SET_ERROR( NET_OK );
    
	// 현재 소환된 펫이 있는지 체크!
	if( GetSummonedPetUID() == 0 )
	{
		START_LOG( cerr, L"소환된 펫이 없는데 자동 먹이 기능 세팅을 하려고 하네? 있을 수 없는에러!" )
			<< BUILD_LOG( kReq.m_bAutoFeed )
			<< END_LOG;

		SET_ERROR( ERR_PET_06 );
		return false;
	}

	// 소환된 펫 정보얻기!
	KUserPetPtr spSummonedPet = GetSummonedPet();
	if( spSummonedPet == NULL )
	{
		START_LOG( cerr, L"소환 펫의 PetUID값은 있는데 펫 정보는 없네?" )
			<< BUILD_LOG( GetSummonedPetUID() )
			<< END_LOG;

		SET_ERROR( ERR_PET_06 );
		return false;
	}

	// 이미 되어있는 세팅인지 체크!
	if( spSummonedPet->IsAutoFeed() == kReq.m_bAutoFeed )
	{
		START_LOG( cerr, L"이미 세팅된 내용을 또 세팅하려고 합니다." )
			<< BUILD_LOG( GetSummonedPetUID() )
			<< BUILD_LOG( kReq.m_bAutoFeed )
			<< END_LOG;

		SET_ERROR( ERR_PET_13 );
		return false;
	}
	
	// 설정 저장
    spSummonedPet->UpdateAutoFeed( kReq.m_bAutoFeed );
	return true;
}

bool KUserPetManager::Handler_EGS_PET_EVOLUTION_REQ( IN UidType iPetUID, OUT KEGS_PET_EVOLUTION_ACK& kAck, OUT int& iPetCategorySlotSize, OUT bool& bIsSummonedPet )
{
	SET_ERROR( NET_OK );

	// 펫 정보얻기!
	KUserPetPtr spPet = GetPet( iPetUID );
	if( spPet == NULL )
	{
		START_LOG( cerr, L"소환 펫의 PetUID값은 있는데 펫 정보는 없네?" )
			<< BUILD_LOG( iPetUID )
			<< END_LOG;

		SET_ERROR( ERR_PET_06 );
		return false;
	}

	// 현재 소환된 펫 인지 체크!
	bIsSummonedPet = ( GetSummonedPetUID() == iPetUID );

	// 진화를 할 수 있는 조건이 만족되는지 체크한다!
	if( spPet->Evolution( kAck.m_kEvolutionResultPetInfo, iPetCategorySlotSize ) == false )
	{
		// NetError::GetLastError()
		return false;
	}

	return true;
}

void KUserPetManager::Handler_ERM_UPDATE_PVP_UNIT_INFO_NOT()
{
	// 펫 리스트 정렬
    PetListSort();
}

void KUserPetManager::Handler_ERM_UPDATE_DUNGEON_UNIT_INFO_NOT()
{
	// 펫 리스트 정렬
	PetListSort();
}

bool KUserPetManager::Handler_EGS_ADMIN_CHANGE_PET_INFO_NOT( IN const KEGS_ADMIN_CHANGE_PET_INFO_NOT& kNot, IN KGSUserPtr spUser, OUT KPetInfo& kPetInfo, OUT int& iPetCategorySlotSize )
{
	// 현재 소환된 펫이 있는지 체크!
	if( GetSummonedPetUID() == 0 )
	{
		START_LOG( cout, L"소환된 펫이 없는데 펫 치트를 썼네? 영자님 치트좀 제대로 쓰세요!" )
			<< BUILD_LOG( spUser->GetCharName() )
			<< BUILD_LOG( kNot.m_iCheatType )
			<< BUILD_LOG( kNot.m_iValue );
		return false;
	}

	// 소환된 펫 정보얻기!
	KUserPetPtr spSummonedPet = GetSummonedPet();
	if( spSummonedPet == NULL )
	{
		START_LOG( cerr, L"소환 펫의 PetUID값은 있는데 펫 정보는 없네?" )
			<< BUILD_LOG( GetSummonedPetUID() )
			<< END_LOG;
		return false;
	}

    switch( kNot.m_iCheatType )
	{
	case KEGS_ADMIN_CHANGE_PET_INFO_NOT::PCT_EVOLUTION_STEP:
		{
			// 진화 치트
            spSummonedPet->Evolution_Cheat( static_cast<char>(kNot.m_iValue), kPetInfo, iPetCategorySlotSize );
		}
		break;

	case KEGS_ADMIN_CHANGE_PET_INFO_NOT::PCT_SATIETY:
		{
			if( kNot.m_iValue < 0 )
			{
				spSummonedPet->DecreaseSatiety( static_cast<short>(-kNot.m_iValue) );
				spSummonedPet->GetPetInfo( kPetInfo );
			}
			else
			{
				spSummonedPet->IncreaseSatiety( static_cast<short>(kNot.m_iValue) );
				spSummonedPet->GetPetInfo( kPetInfo );
			}
		}
		break;

	case KEGS_ADMIN_CHANGE_PET_INFO_NOT::PCT_INTIMACY:
		{
			if( kNot.m_iValue < 0 )
			{
				spSummonedPet->DecreaseIntimacy( -kNot.m_iValue );
				spSummonedPet->GetPetInfo( kPetInfo );
			}
			else
			{
				spSummonedPet->IncreaseIntimacy( kNot.m_iValue );
				spSummonedPet->GetPetInfo( kPetInfo );
			}
		}
		break;

	case KEGS_ADMIN_CHANGE_PET_INFO_NOT::PCT_EXTROVERSION:
		{
			if( kNot.m_iValue < 0 )
			{
				spSummonedPet->DecreaseExtroversion( static_cast<short>(-kNot.m_iValue) );
				spSummonedPet->GetPetInfo( kPetInfo );
			}
			else
			{
				spSummonedPet->IncreaseExtroversion( static_cast<short>(kNot.m_iValue) );
				spSummonedPet->GetPetInfo( kPetInfo );
			}
		}
		break;

	case KEGS_ADMIN_CHANGE_PET_INFO_NOT::PCT_EMOTION:
		{
			if( kNot.m_iValue < 0 )
			{
				spSummonedPet->DecreaseEmotion( static_cast<short>(-kNot.m_iValue) );
				spSummonedPet->GetPetInfo( kPetInfo );
			}
			else
			{
				spSummonedPet->IncreaseEmotion( static_cast<short>(kNot.m_iValue) );
				spSummonedPet->GetPetInfo( kPetInfo );
			}
		}
		break;

	default:
		{
			START_LOG( cerr, L"이상한 펫 치트타입이 왔습니다! 해킹 아니야?" )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( kNot.m_iCheatType )
				<< BUILD_LOG( kNot.m_iValue )
				<< END_LOG;
		}
		return false;
	}

	return true;
}


#endif SERV_PET_SYSTEM
//}}

//{{ 2011. 12.13    김민성	던전 클리어 시 아이템 지급 이벤트 - 현자의 주문서(중복 지급 금지)
#if defined(SERV_DUNGEON_CLEAR_PAYMENT_ITEM_EVENT) || defined(SERV_DUNGEON_CLEAR_PAYMENT_ITEM_EVENT_EX)
bool KUserPetManager::ExistItemInPetInventory( IN int iItemID )
{
	std::map< UidType, KUserPetPtr >::iterator mit = m_mapPetList.begin();
	if( mit == m_mapPetList.end() )
	{
		return false;
	}

	KUserPetPtr pUserPet;

	for( ; mit != m_mapPetList.end() ; ++mit )
	{
		pUserPet = mit->second;

		if( pUserPet != NULL )
		{
			if( pUserPet->ExistItemInInventory( iItemID ) == true )
			{
				return true;
			}
		}
	}

	return false;
}
#endif //SERV_DUNGEON_CLEAR_PAYMENT_ITEM_EVENT || SERV_DUNGEON_CLEAR_PAYMENT_ITEM_EVENT_EX
//}}

//{{ 2012. 02. 22	박세훈	펫 이름 변경권
#ifdef SERV_PET_CHANGE_NAME
int KUserPetManager::GetExistPetNum( void )
{
	if( m_mapPetList.size() != m_vecPetList.size() )
	{
		START_LOG( cerr, L"컨테이너에 저장된 펫 정보가 일치하지 않습니다." )
			<< BUILD_LOG( m_mapPetList.size() )
			<< BUILD_LOG( m_vecPetList.size() )
			<< END_LOG;
		
		return -1;
	}
	return m_mapPetList.size();
}
#endif SERV_PET_CHANGE_NAME
//}}

#ifdef SERV_PERIOD_PET
bool KUserPetManager::GetExpirePetUID( OUT std::vector<UidType>& vecPetUID )
{
	vecPetUID.clear();
	BOOST_TEST_FOREACH( const KUserPetPtr, spInfo, m_vecPetList )
	{
		if( spInfo->IsExpirePet() == true )
		{
			vecPetUID.push_back( spInfo->GetPetUID() );
		}
	}

	return true;
}
bool KUserPetManager::IsExpirePet( IN UidType iPetUID, OUT int& iPetID )
{
	KUserPetPtr spPet = GetPet( iPetUID );
	if( spPet == NULL )
	{
		START_LOG( cerr, L"보유 하고 있지 않는 PetUID입니다." )
			<< BUILD_LOG( iPetUID )
			<< BUILD_LOG( m_mapPetList.size() )
			<< END_LOG;
		return false;
	}

	if( spPet->IsExpirePet() == false )
	{
		return false;
	}

	iPetID = spPet->GetPetID();
	return true;
}
bool KUserPetManager::ReleasePet( IN UidType iPetUID )
{
	// 소환되어 있는 라이딩 펫인가?
	if( ( GetSummonedPet() != NULL ) &&
		( GetSummonedPetUID() == iPetUID )
		)
	{
		return false;
	}

	KUserPetPtr spPet = GetPet( iPetUID );

	// 내가 가지고 있는 라이딩 펫인가?
	if( spPet == NULL )
	{
		return false;
	}

	// 삭제
	return DelUserPet( iPetUID );
}
#endif SERV_PERIOD_PET

#ifdef SERV_EVENT_VC
bool KUserPetManager::Handler_EGS_USE_INTIMACY_UP_ITEM_REQ( OUT KEGS_CHANGED_PET_INTIMACY_NOT& kNotIntimacy, OUT int& iUpPercent )
{
	SET_ERROR( NET_OK );

	// 현재 소환된 펫이 있는지 체크!
	if( GetSummonedPetUID() == 0 )
	{
		START_LOG( cerr, L"소환된 펫이 없는데 친밀 상승아이템을 주려고 하네? 있을 수 없는에러!" )
			<< END_LOG;

		SET_ERROR( ERR_PET_06 );
		return false;
	}

	// 소환된 펫 정보얻기!
	KUserPetPtr spSummonedPet = GetSummonedPet();
	if( spSummonedPet == NULL )
	{
		START_LOG( cerr, L"소환 펫의 PetUID값은 있는데 펫 정보는 없네?" )
			<< BUILD_LOG( GetSummonedPetUID() )
			<< END_LOG;

		SET_ERROR( ERR_PET_06 );
		return false;
	}

	// 먹이 주기!
	if( false == spSummonedPet->IncreaseIntimacyByItem( kNotIntimacy.m_iCurrentIntimacy, iUpPercent ) )
	{
		START_LOG( cerr, L"최대 친밀도가 잘못되어 있습니다." )
			<< BUILD_LOG( GetSummonedPetUID() )
			<< END_LOG;

		SET_ERROR( ERR_PET_11 );
		return false;
	}

	kNotIntimacy.m_iPetUID = GetSummonedPetUID();
	return true;
}
#endif //SERV_EVENT_VC