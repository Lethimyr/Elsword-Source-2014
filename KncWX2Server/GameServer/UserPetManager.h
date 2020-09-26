#pragma once
#include "UserPet.h"
#include "Event.h"
#include "CommonPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"
#include "X2Data/XSLRoom.h"


//{{ 2010. 07. 21  최육사	펫 시스템
#ifdef SERV_PET_SYSTEM

SmartPointer(KGSUser);

class KUserPetManager
{
public:
	enum PET_ENUM
	{
		PE_MAX_PET_COUNT	= 3,
		PE_PAGE_PER_COUNT	= 6,
	};

public:
	KUserPetManager(void);
	~KUserPetManager(void);

	//////////////////////////////////////////////////////////////////////////	
	// init
	void	Clear();
	void	ClearReservedPet()							{ m_mapCreatePetReserve.clear(); }
	void	Init( IN KGSUserPtr spUser, 
				  IN UidType iSummonedPetUID, 
				  IN const std::map< UidType, std::map< UidType, KInventoryItemInfo > >& mapPetItem, 
				  IN OUT std::vector< KPetInfo >& vecPet,
				  OUT int& iPetCategorySlotSize,
				  OUT std::map< UidType, KInventoryItemInfo >& mapPetInventory,
				  OUT std::vector< UidType >& vecCanEvolutionPetList,
				  OUT std::vector< UidType >& vecNeverSummonPetList );

	//////////////////////////////////////////////////////////////////////////	
	// get
	KUserPetPtr GetPet( IN UidType iPetUID );
	KUserPetPtr GetSummonedPet()					{ return GetPet( GetSummonedPetUID() ); }
	bool	GetPetInfo( IN UidType iPetUID, OUT KPetInfo& kPetInfo );
	UidType GetSummonedPetUID() const				{ return m_iSummonedPetUID; }
	UidType GetLastSummonedPetUID() const			{ return m_iLastSummonedPetUID; }
	void	SetSummonedPetUID( IN UidType iPetUID );
	void	GetSummonedPetInfo( OUT std::vector< KPetInfo >& vecPet );
	void	GetSummonedPetInfo( OUT std::vector< KFieldPetInfo >& vecPet );
	bool	GetPetInventory( IN UidType iPetUID, OUT std::map< UidType, KInventoryItemInfo >& mapPetItem );	
	void	GetDBUpdatePetInfo( OUT std::vector< KPetInfo >& vecPetInfo );
	int		GetSummonCount()						{ return m_iSummonCount; }

	void	RollBackDBUpdatePetInfo( IN const std::vector< KPetInfo >& vecPetInfo );

	//////////////////////////////////////////////////////////////////////////	
	// function
	bool	IsExistPet( IN UidType iPetUID ) const		{ return ( m_mapPetList.find( iPetUID ) != m_mapPetList.end() ); }
	bool	IsExistPet( IN const std::wstring& wstrPetName ) const;
	//{{ 2010. 10. 18	최육사	펫 오라 스킬
#ifdef SERV_PET_AURA_SKILL
	bool	IsSummonedPetHungry();
#endif SERV_PET_AURA_SKILL
	//}}
	bool	UpdatePetInventory( IN UidType iPetUID, IN const std::map< UidType, KInventoryItemInfo >& mapPetItem );
	void	OnDeselectUnit( IN KGSUserPtr spUser );

	// 펫 포만도 감소

	//{{ 2012. 04. 26	박세훈	펫 장난감
#ifdef SERV_PET_TOY
	bool	DecreaseSatietyOnDungeonRoom( IN char cUserCount, IN short sSubStageExpNpcNum, IN const bool& bHaveToy, OUT KEGS_CHANGED_PET_SATIETY_INTIMACY_NOT& kNot );
	bool	DecreaseSatietyOnPvpRoom( IN char cUserCount, IN const bool& bHaveToy, OUT KEGS_CHANGED_PET_SATIETY_INTIMACY_NOT& kNot );
#else
	bool	DecreaseSatietyOnDungeonRoom( IN char cUserCount, IN short sSubStageExpNpcNum, OUT KEGS_CHANGED_PET_SATIETY_INTIMACY_NOT& kNot );
	bool	DecreaseSatietyOnPvpRoom( IN char cUserCount, OUT KEGS_CHANGED_PET_SATIETY_INTIMACY_NOT& kNot );
#endif SERV_PET_TOY
	//}}

	// 펫 자동먹이 기능
	//{{ 2011. 05. 23  김민성	큐피엘 젤리 친밀도 증감
#ifdef SERV_ADD_KEWPIEL_JELLY
	bool	CheckAutoFeed( IN KGSUserPtr spUser, OUT bool& bIsFirstFeed, OUT KEGS_CHANGED_PET_SATIETY_NOT& kNotSatiety, OUT KEGS_CHANGED_PET_INTIMACY_NOT& kNotIntimacy, OUT KEGS_AUTO_FEED_NOT& kNotAutoFeed, OUT int& iAddIntimacy );
#else
	bool 	CheckAutoFeed( IN KGSUserPtr spUser, OUT bool& bIsFirstFeed, OUT KEGS_CHANGED_PET_SATIETY_NOT& kNotSatiety, OUT KEGS_CHANGED_PET_INTIMACY_NOT& kNotIntimacy, OUT KEGS_AUTO_FEED_NOT& kNotAutoFeed );
#endif SERV_ADD_KEWPIEL_JELLY
		//}}
	

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

	//////////////////////////////////////////////////////////////////////////	
	// event handler

	// 펫 생성
	bool	Handler_EGS_CREATE_PET_REQ(			IN const KEGS_CREATE_PET_REQ& kReq, IN int iItemID, OUT KDBE_CREATE_PET_REQ& kPacketToDB );
	bool	Handler_DBE_CREATE_PET_ACK(			IN const KDBE_CREATE_PET_ACK& kAck );

	// 펫 리스트 얻기
	void	Handler_EGS_GET_PET_LIST_REQ(		IN const KEGS_GET_PET_LIST_REQ& kReq, OUT KEGS_GET_PET_LIST_ACK& kAck ) const;

	// 펫 소환
	bool	Handler_EGS_SUMMON_PET_REQ(			IN KGSUserPtr spUser, IN const KEGS_SUMMON_PET_REQ& kReq, OUT KDBE_SUMMON_PET_REQ& kPacketToDB );
	bool	Handler_DBE_SUMMON_PET_ACK(			IN const KDBE_SUMMON_PET_ACK& kAck, OUT KPetInfo& kSummonedPetInfo, OUT int& iPetCategorySlotSize, OUT std::map< UidType, KInventoryItemInfo >& mapPetItem );

	// 펫에게 명령 내리기
	bool	Handler_EGS_COMMANDS_FOR_PETS_REQ(	IN KGSUserPtr spUser, IN const KEGS_COMMANDS_FOR_PETS_REQ& kReq, OUT CXSLPetManager::PET_ACTION_COMMAND& ePetActionResult );

	// 펫 먹이주기
	//{{ 2011. 05. 23  김민성	큐피엘 젤리 친밀도 증감
#ifdef SERV_ADD_KEWPIEL_JELLY
	bool Handler_EGS_FEED_PETS_REQ( IN char cLevel, IN int iFeedItemID, OUT bool& bIsFirstFeed, OUT KEGS_CHANGED_PET_SATIETY_NOT& kNotSatiey, OUT KEGS_CHANGED_PET_INTIMACY_NOT& kNotIntimacy, OUT int& iAddIntimacy );
#else
	bool Handler_EGS_FEED_PETS_REQ( IN char cLevel, IN int iFeedItemID, OUT bool& bIsFirstFeed, OUT KEGS_CHANGED_PET_SATIETY_NOT& kNotSatiey, OUT KEGS_CHANGED_PET_INTIMACY_NOT& kNotIntimacy );
#endif SERV_ADD_KEWPIEL_JELLY
		//}}

	// 펫 자동 먹이 세팅
	bool	Handler_EGS_SET_AUTO_FEED_PETS_REQ(	IN const KEGS_SET_AUTO_FEED_PETS_REQ& kReq );

	// 펫 진화
	bool	Handler_EGS_PET_EVOLUTION_REQ(		IN UidType iPetUID, OUT KEGS_PET_EVOLUTION_ACK& kAck, OUT int& iPetCategorySlotSize, OUT bool& bIsSummonedPet );

	// 게임 클리어 후 호출
	void	Handler_ERM_UPDATE_PVP_UNIT_INFO_NOT();
	void	Handler_ERM_UPDATE_DUNGEON_UNIT_INFO_NOT();

	// 펫 치트
	bool	Handler_EGS_ADMIN_CHANGE_PET_INFO_NOT( IN const KEGS_ADMIN_CHANGE_PET_INFO_NOT& kNot, IN KGSUserPtr spUser, OUT KPetInfo& kPetInfo, OUT int& iPetCategorySlotSize );

#ifdef SERV_EVENT_VC
	bool	Handler_EGS_USE_INTIMACY_UP_ITEM_REQ( OUT KEGS_CHANGED_PET_INTIMACY_NOT& kNotIntimacy, OUT int& iUpPercent );
#endif //SERV_EVENT_VC

	//{{ 2011. 12.13    김민성	던전 클리어 시 아이템 지급 이벤트 - 현자의 주문서(중복 지급 금지)
#if defined(SERV_DUNGEON_CLEAR_PAYMENT_ITEM_EVENT) || defined(SERV_DUNGEON_CLEAR_PAYMENT_ITEM_EVENT_EX)
	bool	ExistItemInPetInventory( IN int iItemID );
#endif // SERV_DUNGEON_CLEAR_PAYMENT_ITEM_EVENT || SERV_DUNGEON_CLEAR_PAYMENT_ITEM_EVENT_EX
	//}}

	//{{ 2012. 02. 22	박세훈	펫 이름 변경권
#ifdef SERV_PET_CHANGE_NAME
	int GetExistPetNum( void );
#endif SERV_PET_CHANGE_NAME
	//}}

#ifdef SERV_PERIOD_PET
	bool	GetExpirePetUID( OUT std::vector<UidType>& vecPetUID );
	bool	IsExpirePet( IN const UidType iPetUID, OUT int& iPetID );
	bool	ReleasePet( IN const UidType iPetUID );
#endif SERV_PERIOD_PET

private:
	bool	AddUserPet( IN const KPetInfo& kPetInfo, IN bool bCreatePet = false );
	bool	AddUserPet( IN const KPetInfo& kPetInfo, IN const std::map< UidType, std::map< UidType, KInventoryItemInfo > >& mapPetItem, IN bool bCreatePet = false );
	bool	DelUserPet( IN UidType iPetUID );

	void	PetListSort();	

private:
	std::map< UidType, KUserPetPtr >	m_mapPetList;			// 펫 컨테이너
	std::vector< KUserPetPtr >			m_vecPetList;			// 펫 컨테이너
	std::map< std::wstring, KPetInfo >	m_mapCreatePetReserve;	// 펫 생성 예약 리스트
    UidType								m_iSummonedPetUID;		// 현재 소환된 PetUID
	UidType								m_iLastSummonedPetUID;	// 마지막으로 소환된 PetUID
	int									m_iSummonCount;			// 소환 횟수 체크
};

#endif SERV_PET_SYSTEM
//}}


