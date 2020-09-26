#pragma once

//#ifdef SERV_SKILL_PAGE_SYSTEM
#include "CacheData.h"
//#endif // SERV_SKILL_PAGE_SYSTEM

#include "x2data/XSLSkillTree.h"

//DB access time
//:: 스킬은 배울때 db에 바로 등록
//:: 슬롯은 게임중 슬롯만 체인지 하고 db update는 unit update를 이용


#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013 // 적용날짜: 2013-06-27
class KUserSkillTree
{
public:
	
	enum SKILL_SLOT_INDEX
	{
		SKILL_SLOT_A1 = 0,
		SKILL_SLOT_A2,
		SKILL_SLOT_A3,
		SKILL_SLOT_A4,

		SKILL_SLOT_B1,
		SKILL_SLOT_B2,
		SKILL_SLOT_B3,
		SKILL_SLOT_B4,

		MAX_SKILL_SLOT,
	};


	enum SKILL_SLOT_B_EXPIRATION_STATE
	{
		SSBES_EXPIRED,					// 기간제 만료
		SSBES_NOT_EXPIRED,				// 기간제 남음
		SSBES_PERMANENT,				// 무제한
	};


	struct UserSkillData
	{
		int m_iSkillLevel;
		int m_iSkillCSPoint;

		UserSkillData() : 
		m_iSkillLevel( 0 ),
			m_iSkillCSPoint( 0 )
		{
		}

		UserSkillData( int iLevel, int iSkillCSPoint )
		{
			m_iSkillLevel	= iLevel;
			m_iSkillCSPoint = iSkillCSPoint;
		}
	};

	typedef std::map<int,UserSkillData> SkillDataMap;	// (skill id, UserSkillData)					

#ifdef SERV_SKILL_PAGE_SYSTEM
	struct SkillPageData 
	{
		KCacheData<int>			m_iSPoint;
		//int						m_iSPoint;
		int						m_iCSPoint;
		
		// 기존에는 배열 하나에 일렬로 되어 있었으나, 
		// CommonPacket.h의 KUserSkillPageData의 m_aEquippedSkill 과의 변환이 용이하도록
		// A, B 슬롯 두개로 나누었음
		// KUserSkillPageData를 일렬로 바꾸는 것도 생각해 보았으나, 
		// 그럴 경우 클라이언트 코드까지 바꾸어야 하는 비용이 있어 이렇게 변경함
		//int						m_aiEquippedSkillSlot[KUserSkillPageData::EQUIPPED_SKILL_SLOT_COUNT];	// 장착 스킬 슬롯 정보		
		//int						m_aiEquippedSkillSlotB[KUserSkillPageData::EQUIPPED_SKILL_SLOT_COUNT];	// 장착 스킬 슬롯 정보		
		int						m_aiSkillSlot[MAX_SKILL_SLOT];	// 장착 스킬 슬롯 정보
		SkillDataMap			m_mapSkillTree;					// 배운 스킬 리스트

		SkillPageData()
			: /*m_iSPoint(0), */m_iCSPoint(0)
		{
			ClearLearnedSkillTree();
			FillEquippedSkillSlotWithZero();
		}

// 		int GetSPoint() const { return m_iSPoint; }
// 		void SetSPoint(int val) { m_iSPoint = val; }

		KCacheData<int>& GetSPoint() { return m_iSPoint; }

		int GetCSPoint() const { return m_iCSPoint; }
		void SetCSPoint(int val) { m_iCSPoint = val; }

		void ClearLearnedSkillTree()
		{
			m_mapSkillTree.clear();
		}

		void FillEquippedSkillSlotWithZero()
		{
				ZeroMemory( m_aiSkillSlot, sizeof(int) * MAX_SKILL_SLOT );
	// 			ZeroMemory( m_aiEquippedSkillSlot, KUserSkillPageData::EQUIPPED_SKILL_SLOT_COUNT );
	// 			ZeroMemory( m_aiEquippedSkillSlotB, KUserSkillPageData::EQUIPPED_SKILL_SLOT_COUNT );
		}
	};
	
#endif // SERV_SKILL_PAGE_SYSTEM


public:
	KUserSkillTree(void);
	~KUserSkillTree(void);

	
#ifdef SERV_SKILL_PAGE_SYSTEM
	void	ResetThisSkillPage( IN const int iSkillPagesNumber_ );
	void	ResetEveryPage();

	bool	SetSkillIDToSlotUsedPage( IN const int iSlotID_, IN const int iSkillID_ );
	int		GetSkillIDFromSlotUsedPage( IN const int iSlotID_ ) const;

	void	GetSkillSlotFromEveryPage( OUT std::map< int, std::vector<int> >& mapSkillSlotVector_ ) const;
#else //SERV_SKILL_PAGE_SYSTEM
	void	Reset( bool bResetSkillTree, bool bResetEquippedSkill, bool bResetUnsealedSkill, bool bResetCashSkillPoint, bool bResetSkillNote );
	void	InitSkill( IN std::vector<KUserSkillData>& vecSkillList, IN int aSkillSlot[], IN std::wstring& wstrSkillSlotBEndDate, IN std::vector<short int>& vecUnsealedSkillList, IN int iUnitClass ); //초기 접속시
#endif // SERV_SKILL_PAGE_SYSTEM

	//{{ 2010. 03. 22  최육사	기술의 노트
	void	InitSkillNote( IN char cSkillNoteMaxPageNum, IN const std::map< char, int >& mapSkillNote );
		
	bool	ChangeSkillSlot( int iSlotID, int iSkillID );

	//{{ 2012. 12. 3	박세훈	스킬 슬롯 체인지 패킷 통합
	bool	ChangeSkillSlot( IN const KEGS_CHANGE_SKILL_SLOT_REQ& kPacket_, OUT KEGS_CHANGE_SKILL_SLOT_ACK& kPacket );
	void	GetSkillSlot( OUT std::vector<int>& vecSkillID );
	void	GetSkillSlot( OUT std::vector<KSkillData>& vecSkillSlot );
	void	GetSkillSlot( OUT KSkillData aSkillSlot[] );	

	//{{ 2012. 12. 3	박세훈	스킬 슬롯 체인지 패킷 통합
	int		GetSkillID( int iSlotID );
	int		GetSlotID( int iSkillID );

	bool	IsSkillSlotB( int iSlotID ) { return ( iSlotID >= SKILL_SLOT_B1 && iSlotID < MAX_SKILL_SLOT ); }
	void	ExpandSkillSlotB( std::wstring& wstrSkillSlotBEndDate );
	void	ExpireSkillSlotB();
	KUserSkillTree::SKILL_SLOT_B_EXPIRATION_STATE GetSkillSlotBExpirationState();
	const std::wstring& GetSkillSlotBEndDateString() const { return m_wstrSkillSlotBEndDate; }

	bool	IsSkillUnsealed( int iSkillID );

	//{{ 2009. 8. 4  최육사		스킬봉인해제
	bool	SkillUnseal( int iSkillID );

	bool	IsCashSkillPointExpired() const;

	int		GetMaxCSPoint() const { return m_iMaxCSPoint; }
	void	SetMaxCSPoint(int val) { m_iMaxCSPoint = val; }
	const std::wstring& GetCSPointEndDateString() { return m_wstrCSPointEndDate; }
	CTime	GetCSPointEndDate() const { return m_tCSPointEndDate; }
	bool	SetCSPointEndDate( std::wstring wstrEndDate );

	bool	IsMyUnitClassSkill( int iSkillID );										// 내 유닛의 스킬트리에 포함되는 스킬인지
	bool	IsAllPrecedingSkillLearned( int iSkillID, std::map< int, KGetSkillInfo >& mapSkillList );							// 선행스킬을 모두 최고레벨까지 배웠는지
	bool	IsAllFollowingSkillLevelZero( int iSkillID );							// leaf node인지

	void	SetUnitClass(int val) { m_iUnitClass = val; }

	//{{ 2010. 03. 22  최육사	기술의 노트
	char	GetSkillNoteMaxPageNum()						{ return m_cSkillNoteMaxPageNum; }
	void	GetSkillNote( OUT std::vector< int >& vecSkillNote );
	void	UpdateSkillNoteMaxPageNum( IN char cPageNum )	{ m_cSkillNoteMaxPageNum = cPageNum; }
	bool	GetExpandSkillNotePage( IN u_char ucLevel, OUT char& cPageNum );
	bool	IsHaveSkillNote()								{ return ( m_cSkillNoteMaxPageNum > 0 ); }
	bool	IsExistSkillNotePage( IN char cPageNum );
	bool	IsEmptySkillNotePage( IN char cPageNum )		{ return ( m_mapSkillNote.find( cPageNum ) == m_mapSkillNote.end() ); }
	bool	IsExistSkillNoteMemoID( IN int iSkillNoteMemoID );
	void	UpdateSkillNoteMemo( IN char cPageNum, IN int iMemoID );

	//{{ 2011. 01. 06  김민성  스킬슬롯체인지 체크(인벤토리-기간제) 기능 구현
	void SetSkillSolotBEndDate( IN const std::wstring& wstrSkillSlotBEndDate );

#ifdef	SERV_SKILL_SLOT_ITEM_BUG_FIX	// 적용날짜: 2013-07-04
	void GetSkillSolotBEndDate( CTime& tSkillSlotBEndDate ) { tSkillSlotBEndDate = m_tSkillSlotBEndDate; }
#endif	// SERV_SKILL_SLOT_ITEM_BUG_FIX

	//{{ 2011. 11. 21  김민성	전직 변경 아이템
	void	GetUnSealedSkillList( OUT std::set< int >& setUnsealedSkillID )			{ setUnsealedSkillID = m_setUnsealedSkillID; }
	void	GetUnSealedSkillList( OUT std::vector< short >& vecUnsealedSkillID );
	void	SetClassChangeSkill( IN std::map< int, int >& mapSkill );
	void	GetMemoList( OUT std::map< char, int >& mapSkillNote )					{ mapSkillNote = m_mapSkillNote; }
	void	SetClassChangeMemo( IN std::map< int, int >& mapMemo );

	void	CheckAddSkillStat_BaseHP( IN const KStat& kStat, IN OUT KStat& kModifiedBaseStatBySkill );

#ifdef SERV_SKILL_PAGE_SYSTEM
	
	std::vector<SkillPageData>& AccessSkillPageDataVector() { return m_vecSkillPageData; }
	const std::vector<SkillPageData>& GetSkillPageDataVector() const { return m_vecSkillPageData; }

	int		GetSkillLevel( IN int iSkillID_ ) const;
	
	int		GetSPoint() const;
	
	void	SubtractSPoint( IN const int iSPoint_ );

	void	AddSPointEveryPage( IN const int iSPoint_ );
	void	AddSPointEveryPage( IN const std::vector<int>& vecSPoint_ );
	void	AddSPoint( IN const int iSkillPagesIndex_, IN const int iSPoint_ );

	void	SetSPoint( IN const int iSPoint_ );
	void	SetSPoint( IN const int iSkillPagesIndex_, IN const int iCSPoint_ );
	void	SetSPointEveryPage( IN const int iCSPoint_ );

	int		GetCSPointAnyPage() const;
	// kimhc // 김현철 // 2013-11-17 // GetCSPointFromUsedPage 라고 변경하고 싶었으나 그냥 놔둠
	int		GetCSPoint() const;
	void	SetCSPoint( IN const int iSkillPagesIndex_, IN const int iCSPoint_ );
	void	SetCSPointEveryPage( IN const int iCSPoint_ );

	bool	GetSkillLevelAndCSP( OUT int& iSkillLevel_, OUT int& iSkillCSPoint_, 
		IN const int iSkillID_, IN const SkillDataMap& mapSkillTree_ ) const;	// 획득한 스킬이 존재하는지 체크, 있다면 스킬레벨을 return, 없으면 -1 return

	void	CheckAndUpdateSkillLevelAndCSPOnEveryPage( IN const int iSkillID_, 
		IN const int iSkillLevel_, IN const int iSkillCSPoint_ );
	void	CheckAndUpdateSkillLevelAndCSPOnUsedPage( IN const int iSkillID_, 
		IN const int iSkillLevel_, IN const int iSkillCSPoint_ );
	void	UpdateSkillLevelAndCspOnThisPage( IN const int iSkillID_, IN const int iSkillLevel_, 
		IN const int iSkillCSPoint_, IN const int iSkillPagesIndex_ );

	bool	IsExistOnThisPage( IN const int iSkillID_, IN const int iSkillPagesIndex_ ) const;
	bool	IsExistOnUsedPage( IN const int iSkillID_ ) const;
	bool	IsExistOnEveryPages( IN const int iSkillID_ ) const;
	void	GetPassiveSkillData( OUT std::vector<KSkillData>& vecSkillSlot_, IN const SkillDataMap& mapSkillTree_ ) const;
	void	CalcUsedSPointAndCSPoint( OUT std::vector<int>& vecSPoint_, 
		OUT std::vector<int>& vecCSPoint_ ) const;
	void	CalcExpireCashSkillPointEveryPage( OUT std::vector<KRetrievedSkillPageData>& vecRetrivedSkillPageData_ ) const;
	void	CalcExpireCashSkillPoint( OUT int& iRetrievedSPoint_, 
		OUT std::vector<KUserSkillData>& vecModifiedUserSkillData_, 
		IN const SkillPageData& skillPageData_ ) const;

	int		GetRollBackLevelStep( IN const UserSkillData& userSkillData_, 
			IN const CXSLSkillTree::SkillTemplet* pSkillTemplet_, IN const bool bDefaultSkill_ ) const;
	void	ExpireCashSkillPoint();
	void	GetSkillStat( OUT KStat& kStat_, IN const SkillDataMap& mapSkillTree_ ) const;
	bool	IsMasterSkillLevel( IN const int iSkillID_, IN const SkillDataMap& mapSkillTree_ ) const; // 스킬을 최고레벨까지 찍었는지
	bool	IsSkillLearned( IN const int iSkillID_, IN const SkillDataMap& mapSkillTree_ ) const;	// 스킬을 습득했는가?
	// 배우는데 필요한 스킬 포인트가 있는가?
	bool	GetNecessarySkillPoint( IN OUT std::map< int, KGetSkillInfo >& mapSkillList_, 
		OUT int& iTotalSP_, OUT int& iTotalCSP_, IN const SkillDataMap& mapSkillTree_ ) const;
	// 배운 스킬 중 특정 tire 의 스킬목록을 얻는다
	void	GetTierSkillList( OUT std::vector< int >& vecTierSkillList_, OUT bool& bDefaultSkillTire_, IN const int iTier_, IN const SkillDataMap& mapSkillTree_ ) const;
	// 배운 스킬 리스트
	void	GetHaveSkillList( OUT std::vector< std::map<int, int> >& vecMapHaveSkill_ ) const;
	void	ResetSkill( IN OUT SkillDataMap& mapSkillTree_, IN int iSkillID, IN bool bDefaultSkill = false );

#else // SERV_SKILL_PAGE_SYSTEM

	int		GetSkillLevel( IN int iSkillID );		
	int		GetCSPoint() const { return m_iCSPoint; }
	void	SetCSPoint(int val) { m_iCSPoint = val; }

	bool	GetSkillLevelAndCSP( IN int iSkillID, OUT int& iSkillLevel, OUT int& iSkillCSPoint );	// 획득한 스킬이 존재하는지 체크, 있다면 스킬레벨을 return, 없으면 -1 return

	bool 	SetSkillLevelAndCSP( int iSkillID, int iSkillLevel, int iSkillCSPoint );				// 스킬이 존재하면 스킬레벨을 지정된 레벨로, 없으면 추가하고 스킬레벨을 지정된 레벨로
	bool	IsExist( IN int iSkillID );
	void	GetPassiveSkillData( OUT std::vector<KSkillData>& vecSkillSlot );
	void	CalcUsedSPointAndCSPoint( OUT int& iSPoint, OUT int& iCSPoint );
	void	CalcExpireCashSkillPoint( OUT int& iRetrievedSPoint, OUT std::vector<KUserSkillData>& vecModifiedUserSkillData );
	void	ExpireCashSkillPoint();
	void	GetSkillStat( KStat& kStat );
	bool	IsMasterSkillLevel( IN int iSkillID );										// 스킬을 최고레벨까지 찍었는지
	bool	IsSkillLearned( IN int iSkillID );											// 스킬을 습득했는가?
	// 배우는데 필요한 스킬 포인트가 있는가?
	bool	GetNecessarySkillPoint( IN OUT std::map< int, KGetSkillInfo >& mapSkillList, IN int& iTotalSP, IN int& iTotalCSP );
	// 배운 스킬 중 특정 tire 의 스킬목록을 얻는다
	void	GetTierSkillList( IN int iTier, OUT std::vector< int >& vecTierSkillList, OUT bool& bDefaultSkillTire );
	// 배운 스킬 리스트
	void	GetHaveSkillList( OUT std::map< int, int >& mapHaveSkill );
	void	ResetSkill( IN int iSkillID, IN bool bDefaultSkill = false );
#endif // SERV_SKILL_PAGE_SYSTEM


	bool	CheckGetNewSkill( IN std::map< int, KGetSkillInfo >& mapGetSkillList, IN int iUnitClass, IN int iLevel, OUT KEGS_GET_SKILL_ACK& kPacket );
	bool	CheckResetSkill( IN KEGS_RESET_SKILL_REQ& kPacket_, IN int iUnitClass, IN int iLevel, OUT int& iOK, OUT bool& bSKillInitLevel );

#ifdef SERV_SKILL_PAGE_SYSTEM
	
	unsigned char		GetTheNumberOfSkillPagesAvailable() const { return m_vecSkillPageData.size(); }
	//void				SetTheNumberOfSkillPagesAvailable(unsigned char val) { m_nTheNumberOfSkillPagesAvailable = val; }

	unsigned char		GetActiveSkillPagesIndex() const { return m_nActiveSkillPageNumber - 1; }
	unsigned char		GetActiveSkillPageNumber() const { return m_nActiveSkillPageNumber; }
	void				SetActiveSkillPageNumber(unsigned char val) { m_nActiveSkillPageNumber = val; }

	
	bool				IsActiveSkillPageNumberValid() const
	{ 
		// GetActiveSkillPageNumber() 는 1부터 시작 함
		if ( GetActiveSkillPagesIndex() < 0
			|| GetActiveSkillPagesIndex() >= m_vecSkillPageData.size() )
			return false;
		else
			return true;
	}

	bool				CanExpandSkillPage() const { return ( GetTheNumberOfSkillPagesAvailable() < MAX_COUNT_OF_PAGES_AVAILABLE ); }
	bool				CanExpandSkillPage( const int iTheNumberOfPagesToBeAdded_ ) const 
	{ 
		const int iTheNumberOfPagesAfterAdding 
			= GetTheNumberOfSkillPagesAvailable() + iTheNumberOfPagesToBeAdded_;

		return ( iTheNumberOfPagesAfterAdding <= MAX_COUNT_OF_PAGES_AVAILABLE );
	}

	SkillDataMap&		AccessLearnedSkillTree() { return m_vecSkillPageData[GetActiveSkillPagesIndex()].m_mapSkillTree; }
	
	void				InitEverySkillPage( IN OUT std::vector<KUserSkillPageData>& vecSkillPage_, 
		IN const std::wstring& wstrSkillSlotBEndDate_, 
		IN const std::vector<short int>& vecUnsealedSkillList_, 
		IN const int iUnitClass_, IN const int iSPointAtThisLevel_ );

	void				ExpandSkillPage( IN const int iDefaultSkillId_[], IN const int iSPoint_, IN const int iCSPoint_ );
	bool				GetKUserSkillPageData( OUT KUserSkillPageData& kUserSkillPageData_, IN const int iSkillPagesNumberYouWantToGet_ ) const;

protected:
	void				InitializeEquippedSkillSlotEveryPage();
	void				InitializeEquippedSkillSlot( const int iSkillPagesIndex_ );

	void				InitializeEveryLearnedSkillTree();
	void				InitializeLearnedSkillTree( const int iSkillPagesIndex_ );

	void				SetSkillPageDataWithLearnedSkills( OUT SkillPageData& skillPageData_, IN const KUserSkillPageData& kUserSkillPage_ );
	void				SetSkillPageDataWithEquippedSkills( OUT SkillPageData& skillPageData_, IN const KUserSkillPageData& kUserSkillPage_ );

	bool				CanUpdateSkillLevelAndCSPOnThisPage( IN const int iSkillID_, IN const int iSkillLevel_, IN const int iSkillPageIndex_ ) const;

#endif // SERV_SKILL_PAGE_SYSTEM


#ifdef SERV_SKILL_PAGE_SYSTEM
public:
	// kimhc / 2013-12-09 // 기획의 요청으로 ED 금액 통일 시킴
	const static int			ED_NEEDED_TO_EXPAND_SECOND_SKILL_PAGE		= 50000000;
	const static int			MAX_COUNT_OF_PAGES_AVAILABLE				= 3;
#endif // SERV_SKILL_PAGE_SYSTEM

protected:
	std::set< int >			m_setUnsealedSkillID;			// 봉인해제된 스킬ID					


	std::wstring			m_wstrSkillSlotBEndDate;
	CTime					m_tSkillSlotBEndDate;

	int						m_iMaxCSPoint;					// 캐시 스킬 포인트 구매시 캐시스킬포인트
	std::wstring			m_wstrCSPointEndDate;					
	CTime					m_tCSPointEndDate;		

	int						m_iUnitClass;					// skilltreetemplet 정보를 참조하기 위해 unit class 정보가 필요함

#ifdef SERV_SKILL_PAGE_SYSTEM
	std::vector<SkillPageData>	m_vecSkillPageData;
	//unsigned char				m_nTheNumberOfSkillPagesAvailable;
	unsigned char				m_nActiveSkillPageNumber;					
	
#else // SERV_SKILL_PAGE_SYSTEM
	SkillDataMap			m_mapSkillTree;					// 배운 스킬 리스트
	int						m_aiSkillSlot[MAX_SKILL_SLOT];	// 장착 스킬 슬롯 정보
	int						m_iCSPoint;
#endif // SERV_SKILL_PAGE_SYSTEM
	
	std::map< char, int >	m_mapSkillNote;					// [key:SlotID, value:MemoID]
	//{{ 2010. 03. 22  최육사	기술의 노트
	char					m_cSkillNoteMaxPageNum;	
};

#else	// SERV_UPGRADE_SKILL_SYSTEM_2013
/*
class KUserSkillTree
{
public:
	enum SKILL_SLOT_INDEX
	{
		SKILL_SLOT_A1 = 0,
		SKILL_SLOT_A2,
		SKILL_SLOT_A3,
		SKILL_SLOT_A4,

		SKILL_SLOT_B1,
		SKILL_SLOT_B2,
		SKILL_SLOT_B3,
		SKILL_SLOT_B4,

		MAX_SKILL_SLOT,
	};


	enum SKILL_SLOT_B_EXPIRATION_STATE
	{
		SSBES_EXPIRED,					// 기간제 만료
		SSBES_NOT_EXPIRED,				// 기간제 남음
		SSBES_PERMANENT,				// 무제한
	};


	struct UserSkillData
	{
		int m_iSkillLevel;
		int m_iSkillCSPoint;

		UserSkillData() : 
		m_iSkillLevel( 0 ),
		m_iSkillCSPoint( 0 )
		{
		}

		UserSkillData( int iLevel, int iSkillCSPoint )
		{
			m_iSkillLevel	= iLevel;
			m_iSkillCSPoint = iSkillCSPoint;
		}
	};


public:
	KUserSkillTree(void);
	~KUserSkillTree(void);

	void	Reset( bool bResetSkillTree, bool bResetEquippedSkill, bool bResetUnsealedSkill, bool bResetCashSkillPoint, bool bResetSkillNote );
	void	InitSkill( IN std::vector<KUserSkillData>& vecSkillList, IN int aSkillSlot[], IN std::wstring& wstrSkillSlotBEndDate, IN std::vector<short int>& vecUnsealedSkillList, IN int iUnitClass ); //초기 접속시
	//{{ 2010. 03. 22  최육사	기술의 노트
#ifdef SERV_SKILL_NOTE
	void	InitSkillNote( IN char cSkillNoteMaxPageNum, IN const std::map< char, int >& mapSkillNote );
#endif SERV_SKILL_NOTE
	//}}

	int		GetSkillLevel( IN int iSkillID );	
	bool	GetSkillLevelAndCSP( IN int iSkillID, OUT int& iSkillLevel, OUT int& iSkillCSPoint );	// 획득한 스킬이 존재하는지 체크, 있다면 스킬레벨을 return, 없으면 -1 return
	bool 	SetSkillLevelAndCSP( int iSkillID, int iSkillLevel, int iSkillCSPoint );				// 스킬이 존재하면 스킬레벨을 지정된 레벨로, 없으면 추가하고 스킬레벨을 지정된 레벨로
	bool	IsExist( IN int iSkillID );

	bool	ChangeSkillSlot( int iSlotID, int iSkillID );
	//{{ 2012. 12. 3	박세훈	스킬 슬롯 체인지 패킷 통합
#ifdef SERV_SKILL_SLOT_CHANGE_PACKET_INTEGRATE
	bool	ChangeSkillSlot( IN const KEGS_CHANGE_SKILL_SLOT_REQ& kPacket_, OUT KEGS_CHANGE_SKILL_SLOT_ACK& kPacket );
#endif SERV_SKILL_SLOT_CHANGE_PACKET_INTEGRATE
	//}}
	void	GetSkillSlot( OUT std::vector<int>& vecSkillID );
	void	GetSkillSlot( OUT std::vector<KSkillData>& vecSkillSlot );
	void	GetSkillSlot( OUT KSkillData aSkillSlot[] );	

	//{{ 2012. 12. 3	박세훈	스킬 슬롯 체인지 패킷 통합
#ifdef SERV_SKILL_SLOT_CHANGE_PACKET_INTEGRATE
	int		GetSkillID( int iSlotID );
	int		GetSlotID( int iSkillID );
#endif SERV_SKILL_SLOT_CHANGE_PACKET_INTEGRATE
	//}}

	void	GetPassiveSkillData( OUT std::vector<KSkillData>& vecSkillSlot );

	void	GetSkillStat( KStat& kStat );

	
	bool	IsSkillSlotB( int iSlotID ) { return ( iSlotID >= SKILL_SLOT_B1 && iSlotID < MAX_SKILL_SLOT ); }
	void	ExpandSkillSlotB( std::wstring& wstrSkillSlotBEndDate );
	void	ExpireSkillSlotB();
	KUserSkillTree::SKILL_SLOT_B_EXPIRATION_STATE GetSkillSlotBExpirationState();
	const std::wstring& GetSkillSlotBEndDateString() const { return m_wstrSkillSlotBEndDate; }


	
	void	CalcUsedSPointAndCSPoint( OUT int& iSPoint, OUT int& iCSPoint );
	void	CalcCumulativeUsedSPointOnEachTier( OUT std::vector< int >& vecTierSPoint );

	bool	IsSkillUnsealed( int iSkillID );
	//{{ 2009. 8. 4  최육사		스킬봉인해제
	bool	SkillUnseal( int iSkillID );
	//}}

	bool	IsCashSkillPointExpired();
	void	CalcExpireCashSkillPoint( OUT int& iRetrievedSPoint, OUT std::vector<KUserSkillData>& vecModifiedUserSkillData );
	void	ExpireCashSkillPoint();

	int		GetCSPoint() const { return m_iCSPoint; }
	void	SetCSPoint(int val) { m_iCSPoint = val; }
	int		GetMaxCSPoint() const { return m_iMaxCSPoint; }
	void	SetMaxCSPoint(int val) { m_iMaxCSPoint = val; }
	const std::wstring& GetCSPointEndDateString() { return m_wstrCSPointEndDate; }
	CTime	GetCSPointEndDate() const { return m_tCSPointEndDate; }
	bool	SetCSPointEndDate( std::wstring wstrEndDate );

	bool	IsMyUnitClassSkill( int iSkillID );										// 내 유닛의 스킬트리에 포함되는 스킬인지
	bool	IsAllPrecedingSkillMaxLevel( int iSkillID );							// 선행스킬을 모두 최고레벨까지 배웠는지
	bool	IsAllFollowingSkillLevelZero( int iSkillID );							// leaf node인지
	bool	IsTierOpened( int iSkillID );

	void	SetUnitClass(int val) { m_iUnitClass = val; }

	//{{ 2010. 03. 22  최육사	기술의 노트
#ifdef SERV_SKILL_NOTE
	char	GetSkillNoteMaxPageNum()						{ return m_cSkillNoteMaxPageNum; }
	void	GetSkillNote( OUT std::vector< int >& vecSkillNote );
	void	UpdateSkillNoteMaxPageNum( IN char cPageNum )	{ m_cSkillNoteMaxPageNum = cPageNum; }
	bool	GetExpandSkillNotePage( IN u_char ucLevel, OUT char& cPageNum );
	bool	IsHaveSkillNote()								{ return ( m_cSkillNoteMaxPageNum > 0 ); }
	bool	IsExistSkillNotePage( IN char cPageNum );
	bool	IsEmptySkillNotePage( IN char cPageNum )		{ return ( m_mapSkillNote.find( cPageNum ) == m_mapSkillNote.end() ); }
	bool	IsExistSkillNoteMemoID( IN int iSkillNoteMemoID );
	void	UpdateSkillNoteMemo( IN char cPageNum, IN int iMemoID );
#endif SERV_SKILL_NOTE
	//}}

	//{{ 2011. 01. 06  김민성  스킬슬롯체인지 체크(인벤토리-기간제) 기능 구현
#ifdef SERV_SKILL_SLOT_CHANGE_INVENTORY
	void SetSkillSolotBEndDate( std::wstring& wstrSkillSlotBEndDate );
#endif SERV_SKILL_SLOT_CHANGE_INVENTORY

	//{{ 2011. 11. 21  김민성	전직 변경 아이템
#ifdef SERV_UNIT_CLASS_CHANGE_ITEM
	void	GetUnSealedSkillList( OUT std::set< int >& setUnsealedSkillID )			{ setUnsealedSkillID = m_setUnsealedSkillID; }
	void	GetUnSealedSkillList( OUT std::vector< short >& vecUnsealedSkillID );
	void	SetClassChangeSkill( IN std::map< int, int >& mapSkill );
	void	GetMemoList( OUT std::map< char, int >& mapSkillNote )					{ mapSkillNote = m_mapSkillNote; }
	void	SetClassChangeMemo( IN std::map< int, int >& mapMemo );
#endif SERV_UNIT_CLASS_CHANGE_ITEM
	//}}

	bool	IsMaxSkillLevel( IN int iSkillID );														// 스킬을 최고레벨까지 찍었는지
	
	

protected:
	typedef std::map< int, UserSkillData > SkillDataMap;	// (skill id, UserSkillData)					
	SkillDataMap			m_mapSkillTree;					// 배운 스킬 리스트
	int						m_aiSkillSlot[MAX_SKILL_SLOT];	// 장착 스킬 슬롯 정보
	std::set< int >			m_setUnsealedSkillID;			// 봉인해제된 스킬ID					


	std::wstring			m_wstrSkillSlotBEndDate;
	CTime					m_tSkillSlotBEndDate;

	int						m_iCSPoint;
	int						m_iMaxCSPoint;					// 캐시 스킬 포인트 구매시 캐시스킬포인트
	std::wstring			m_wstrCSPointEndDate;					
	CTime					m_tCSPointEndDate;		

	int						m_iUnitClass;					// skilltreetemplet 정보를 참조하기 위해 unit class 정보가 필요함

	//{{ 2010. 03. 22  최육사	기술의 노트
#ifdef SERV_SKILL_NOTE	
	char					m_cSkillNoteMaxPageNum;	
	std::map< char, int >	m_mapSkillNote;					// [key:SlotID, value:MemoID]
#endif SERV_SKILL_NOTE
	//}}
};
*/
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013




