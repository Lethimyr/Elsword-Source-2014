#pragma once
#include "Event.h"
#include "CommonPacket.h"
#include <boost/timer.hpp>
#include "CacheData.h"
#include "X2Data/XSLPetManager.h"
//{{ 2010. 01. 15  최육사	boost allocator
#ifdef SERV_BOOST_ALLOCATOR
	#include <ObjectPool/BoostAllocator.h>
#endif SERV_BOOST_ALLOCATOR
//}}


//{{ 2010. 07. 21  최육사	펫 시스템
#ifdef SERV_PET_SYSTEM

#define KUserPetPtr KUserPet*
#undef KUserPetPtr
SmartPointer( KUserPet );

//////////////////////////////////////////////////////////////////////////
#ifdef SERV_BOOST_ALLOCATOR
BOOST_ALOC_CLASS( KUserPet )
#else
class KUserPet
#endif SERV_BOOST_ALLOCATOR
//////////////////////////////////////////////////////////////////////////
{
public:
	KUserPet( const KPetInfo& kInfo, const std::map< UidType, std::map< UidType, KInventoryItemInfo > >& mapPetItem, bool bCreatePet );
	~KUserPet(void);

	void	Clear();

	//////////////////////////////////////////////////////////////////////////	
	// get
	void	GetPetInfo( OUT KPetInfo& kInfo ) const;
	UidType GetPetUID() const				{ return m_iPetUID; }
#ifdef SERV_PETID_DATA_TYPE_CHANGE //2013.07.02
	int		GetPetID() const				{ return m_iPetID; }
#else //SERV_PETID_DATA_TYPE_CHANGE
	char	GetPetID() const				{ return m_cPetID; }
#endif //SERV_PETID_DATA_TYPE_CHANGE
	const std::wstring& GetPetName() const	{ return m_wstrPetName; }
	char	GetEvolutionStep() const		{ return m_cEvolutionStep; }
	short	GetSatiety() const				{ return m_sSatiety; }
	int		GetIntimacy() const				{ return m_iIntimacy; }
	int		GetMaxIntimacy() const			{ return m_iMaxIntimacy; }
	short	GetExtroversion() const			{ return m_sExtroversion; }
	short	GetEmotion() const				{ return m_sEmotion; }
	CTime	GetLastSummonDate() const		{ return m_tLastSummonDate; }
	bool	IsAutoFeed() const				{ return m_bAutoFeed; }
	bool	IsReservedDBUpdate() const		{ return m_bReservedDBUpdate; }
	bool	IsNeverSummon() const			{ return ( m_tLastSummonDate == m_tRegDate ); } // 펫 생성날짜와 마지막 소환 날짜가 같으면 소환한적 없음!
	
	int		GetPetInventorySlotSize() const;
	void	GetPetInventory( OUT std::map< UidType, KInventoryItemInfo >& mapPetItem ) const { mapPetItem = m_mapPetInventory; }

	//{{ 2010. 10. 18	최육사	펫 오라 스킬
#ifdef SERV_PET_AURA_SKILL
		//{{ 2011. 07. 08    김민성    옵션 수치화
	#ifdef SERV_USE_PERCENT_IN_OPTION_DATA
		void	GetStatByPetSkill( IN bool bIsDungeon, IN const KStat& kBaseStat, IN OUT float& fTotalIncHPRate, OUT KStat& kAddStat, IN unsigned int& uiHP_OnePoint, IN unsigned int& uiUnitLevel );
	#else
		void	GetStatByPetSkill( IN bool bIsDungeon, IN const KStat& kBaseStat, IN OUT float& fTotalIncHPRate, OUT KStat& kAddStat );
	#endif SERV_USE_PERCENT_IN_OPTION_DATA
		//}} 
	void	GetStatIncreaseRateByPetSkill( IN bool bIsDungeon, IN OUT KStatIncreaseRate& kStatIncreaseRateInfo );
#endif SERV_PET_AURA_SKILL
	//}}

	void	GetDBUpdatePetInfo( OUT KPetInfo& kInfo );
	void	RollBackDBUpdatePetInfo( IN const KPetInfo& kInfo );

	//{{ 2012. 02. 22	박세훈	펫 이름 변경권
#ifdef SERV_PET_CHANGE_NAME
	//////////////////////////////////////////////////////////////////////////	
	// set
	void SetPetName( IN const std::wstring& wstrPetName ){ m_wstrPetName = wstrPetName; }
#endif SERV_PET_CHANGE_NAME
	//}}

	//////////////////////////////////////////////////////////////////////////
	// update
	void	UpdatePetInfo( IN const KPetInfo& kPetInfo );
	void	UpdatePetInventory( IN const std::map< UidType, KInventoryItemInfo >& mapPetItem );
	void	UpdateAutoFeed( IN bool bAutoFeed ) { m_bAutoFeed = bAutoFeed; }
	
	short	IncreaseSatiety( IN short sIncreaseSatiety );
	short	DecreaseSatiety( IN short sDecreaseSatiety );
	int		IncreaseIntimacy( IN int iIncreaseIntimacy );
	int		DecreaseIntimacy( IN int iDecreaseIntimacy );
	short	IncreaseExtroversion( IN short sIncreaseExtroversion );
	short	DecreaseExtroversion( IN short sDecreaseExtroversion );
	short	IncreaseEmotion( IN short sIncreaseEmotion );
	short	DecreaseEmotion( IN short sDecreaseEmotion );

	//////////////////////////////////////////////////////////////////////////
	// function

	// 소환
	void	Summon( OUT KPetInfo& kPetInfo, OUT int& iPetCategorySlotSize, OUT std::map< UidType, KInventoryItemInfo >& mapPetItem );
	void	Unsummon();

	// 명령
	bool	CommandsForPet( IN CXSLPetManager::PET_ACTION_COMMAND eActionCommand, OUT CXSLPetManager::PET_ACTION_COMMAND& ePetActionResult );

	// 포만도 처리

	//{{ 2012. 04. 26	박세훈	펫 장난감
#ifdef SERV_PET_TOY
	void	DecreaseSatietyOnDungeonRoom( IN char cUserCount, IN short sSubStageExpNpcNum, IN const bool& bHaveToy );
	void	DecreaseSatietyOnPvpRoom( IN char cUserCount, IN const bool& bHaveToy );
#else
	void	DecreaseSatietyOnDungeonRoom( IN char cUserCount, IN short sSubStageExpNpcNum );
	void	DecreaseSatietyOnPvpRoom( IN char cUserCount );
#endif SERV_PET_TOY
	//}}
	
	// 먹이주기
	bool	IsHungry() const;
	bool	IsPetStuffed( IN float fStuffedPercent = 80.f ) const;
	bool	IsPetSpecialFeed() const;

	//{{ 2011. 05. 23  김민성	큐피엘 젤리 친밀도 증감
#ifdef SERV_ADD_KEWPIEL_JELLY
	bool	Feed( IN char cLevel, IN int iFeedItemID, OUT bool& bIsFirstFeed, OUT short& sCurrentSatiety, OUT int& iCurrentIntimacy, OUT int& iAddIntimacy );
#else
	bool	Feed( IN char cLevel, IN int iFeedItemID, OUT bool& bIsFirstFeed, OUT short& sCurrentSatiety, OUT int& iCurrentIntimacy );
#endif SERV_ADD_KEWPIEL_JELLY
	//}}

	// 진화
	bool	CanEvolution();
	bool	Evolution( OUT KPetInfo& kResultPetInfo, OUT int& iPetCategorySlotSize );
	bool	Evolution_Cheat( IN char cEvolutionStep, OUT KPetInfo& kResultPetInfo, OUT int& iPetCategorySlotSize );

	//{{ 2011. 12.13    김민성	던전 클리어 시 아이템 지급 이벤트 - 현자의 주문서(중복 지급 금지)
#if defined(SERV_DUNGEON_CLEAR_PAYMENT_ITEM_EVENT) || defined(SERV_DUNGEON_CLEAR_PAYMENT_ITEM_EVENT_EX)
	bool	ExistItemInInventory( IN int& iItemID );
#endif SERV_DUNGEON_CLEAR_PAYMENT_ITEM_EVENT || SERV_DUNGEON_CLEAR_PAYMENT_ITEM_EVENT_EX
	//}}

	//{{ 2012. 12. 24	박세훈	펫 오토 루팅 기능 추가
#ifdef SERV_PET_AUTO_LOOTING
	bool	IsAutoLootingPet( void ){	return m_bAutoLooting;	}
	void	SetAutoLootingPet( void ){	m_bAutoLooting = true;	}
#endif SERV_PET_AUTO_LOOTING
	//}}
#ifdef SERV_EVENT_PET_INVENTORY
	bool    IsEventFoodEat(void) { return m_bEventFoodEat; }
#endif SERV_EVENT_PET_INVENTORY

#ifdef SERV_HALLOWEEN_PUMPKIN_FAIRY_PET
	void	ChangePetID( int iChangePetID ){	m_iPetID = iChangePetID; }
#endif //SERV_HALLOWEEN_PUMPKIN_FAIRY_PET

#ifdef SERV_PERIOD_PET
	bool	IsExpirePet() const
	{
		if( m_bDestroy == true && m_tDestroyDate <= CTime::GetCurrentTime() )
			return true;

		return false;
	}
#endif SERV_PERIOD_PET

#ifdef SERV_PET_SYSTEM_EX1
	bool	IsAlwaysMaxSatiety() const { return m_bAlwaysMaxSatiety; }
	void	SetAlwaysMaxSatiety( IN const bool bVal_ ) { m_bAlwaysMaxSatiety = bVal_; }
#endif//SERV_PET_SYSTEM_EX1

#ifdef SERV_EVENT_VC
	bool	IncreaseIntimacyByItem( OUT int& iCurrentIntimacy, OUT int& iUpPercent );	// 친밀도 증가 아이템 사용 처리
#endif //SERV_EVENT_VC

protected:
	void	CheckExtroversion();	// 외향지수 변동
	void	CheckEmotion();			// 감성지수 변동

private:
	// Pet Info
	UidType									m_iPetUID;
#ifdef SERV_PETID_DATA_TYPE_CHANGE //2013.07.02
	int										m_iPetID;
#else //SERV_PETID_DATA_TYPE_CHANGE
	char									m_cPetID;
#endif //SERV_PETID_DATA_TYPE_CHANGE
	std::wstring							m_wstrPetName;
	char									m_cEvolutionStep;		// 진화 단계		DB업데이트 : [20분마다, 소환해제시]
	KCacheData< short >						m_sSatiety;				// 포만도			DB업데이트 : [20분마다, 소환해제시]
	KCacheData< int >						m_iIntimacy;			// 친밀도			DB업데이트 : [20분마다, 소환해제시]
	KCacheData< short >						m_sExtroversion;		// 외향성			DB업데이트 : [20분마다, 소환해제시]
	KCacheData< short >						m_sEmotion;				// 감수성			DB업데이트 : [20분마다, 소환해제시]
	bool									m_bAutoFeed;			// 자동 먹이 기능	DB업데이트 : [20분마다, 소환해제시]	
	CTime									m_tFirstFeedDate;		// 첫 먹이 시각 체크
	CTime									m_tLastSummonDate;		// 마지막 펫 소환 시각
	CTime									m_tRegDate;				// 펫 생성 시각

	int										m_iCumulativeSatiety;	// 포만도 변경값 누적
	int										m_iFeedCount;			// 먹이준 횟수
	int										m_iCumulativeIntimacy;	// 친밀도 변경값 누적
	int										m_iMaxIntimacy;			// 최대 친밀도	
	bool									m_bReservedDBUpdate;	// DB업데이트 예약

	// Pet Manage
	boost::timer							m_tPetSommonTimer;		// 펫 소환 타이머

	// Pet Inventory
	std::map< UidType, KInventoryItemInfo >	m_mapPetInventory;		// 펫 인벤토리

	//{{ 2012. 12. 24	박세훈	펫 오토 루팅 기능 추가
#ifdef SERV_PET_AUTO_LOOTING
	bool									m_bAutoLooting;			// 오토 루팅 여부
#endif SERV_PET_AUTO_LOOTING
	//}}
#ifdef SERV_EVENT_PET_INVENTORY
	bool									m_bEventFoodEat;      // 이벤트 먹이의 사용 유무 
	bool									m_bIsEventPetID;      // 이벤트 성 펫인지 확인 한다.									
#endif SERV_EVENT_PET_INVENTORY
#ifdef SERV_PERIOD_PET
	bool									m_bDestroy;
	CTime									m_tDestroyDate;
#endif SERV_PERIOD_PET

#ifdef SERV_PET_SYSTEM_EX1
	bool									m_bAlwaysMaxSatiety;	// 항상 최대 포만도
#endif//SERV_PET_SYSTEM_EX1
};


#endif SERV_PET_SYSTEM
//}}


