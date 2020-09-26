#pragma once

/** @class : CBattleFieldPortalMovingInfo
@brief : 필드에서 생성 되는 포탈의 이동 정보 클래스
@date  : 2011/10/25
*/

class CBattleFieldPortalMovingInfo
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
    : private boost::noncopyable
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
{
public:
	// 생성자: 기본Type은 Initialize에 넣어주시고, 사용자 정의 타입은 멤버 초기화 리스트에서 초기화 해주세요
	CBattleFieldPortalMovingInfo() 
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
        : m_uRefCount(0)
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
    { _OnCreate(); 
    }
	
	void Initialize() {
		_OnCreate();	// 기본Type 초기화
		// 여기 부터는 기본Type이 아닌 것 초기화	
	}

	bool ParsingScriptFile( IN KLuaManager& luaManager_ );	// CBattleFieldPortalMoveInfo에 필요한 정보를 읽어들인다

	UINT GetLineNumber() const { return m_uiLineNumber; }
	void SetLineNumber( const UINT uiLineNumber_ ) { m_uiLineNumber = uiLineNumber_; }

	UINT GetPlaceIdToMove() const { return m_uiPlaceIdToMove; }
	void SetPlaceIdToMove( const UINT uiPlaceIdToMove_ ) { m_uiPlaceIdToMove = uiPlaceIdToMove_; }

	USHORT GetPositionIndexToMove() const { return m_usPositionIndexToMove; }
	void SetPositionIndexToMove( const USHORT usPositionIndexToMove_ ) { m_usPositionIndexToMove = usPositionIndexToMove_; }

	USHORT GetPortalMoveType() const { return m_usPortalMoveType; }
	void SetPortalMoveType( const USHORT usPortalMoveType_ ) { m_usPortalMoveType = usPortalMoveType_; }


#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
    void    AddRef()    {   ++m_uRefCount; }
    void    Release()   { if ( (--m_uRefCount) == 0 )   delete this; }
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

private:	// private 함수명 앞에는 _를 붙여 주었음

	/// 생성자에서 기본Type의 변수들을 초기화 시키기 위한 용도로 사용
	void _OnCreate() {
		m_uiLineNumber				= 0;
		m_uiPlaceIdToMove			= 0;
		m_usPositionIndexToMove		= 0;
		m_usPortalMoveType			= 0;
#ifdef QUEST_GUIDE
		m_uiFieldID					= 0;
#endif //QUEST_GUIDE
	}

private:
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
    unsigned                                        m_uRefCount;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
	UINT	m_uiLineNumber;				/// 필드의 라인맵에서 이 포탈이 위치한 필드 내의 라인 번호 및 인덱스
	UINT	m_uiPlaceIdToMove;			/// 이동할 장소의 ID (VillageMapID or BattleFieldID)

	USHORT	m_usPositionIndexToMove;	/// 이동할 장소의 Postion Index로 보통 마을의 경우는 DLG_Map_Village.lua에 있는 StartPosId, 필드의 경우에는 StartPos(?) 가 될듯

	USHORT	m_usPortalMoveType;			/// 포탈의 이동 타입 (CX2BattleFieldManager::PORTAL_MOVE_TYPE, 포탈->마을, 포탈->포탈)
#ifdef QUEST_GUIDE
	UINT	m_uiFieldID;				/// BATTLE_FIELD_PORTAL_MOVING_INFO를 가지고 있는 필드의 ID
#endif //QUEST_GUIDE
};

IMPLEMENT_INTRUSIVE_PTR( CBattleFieldPortalMovingInfo );

/** @class : CBattleFieldRiskInfo
@brief : 필드의 위험도 정보 클래스로 필드의 위험도 수치에 따라 어떤 몬스터가 나오는지에 대한 정보를 가지고 있음
@date  : 2011/10/25
*/
class CBattleFieldRiskInfo
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
    : private boost::noncopyable
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
{
public:
	// true, false 속성을 가지는 타입의 플래그, std::bitset으로 관리
	enum FLAG_RISK_INFO {
		FRI_SHOW_BOSS_NAME = 0,				/// 보스 이름을 보여 줄 것인지 여부
		FRI_DEVIDE_BOSS_HP_GAUGE_TO_LINES,	/// 보스의 HP 게이지를 여러 줄로 나눌 것인지 여부
		FRI_FLAG_MAX,						/// 이 ENUM의 MAX 값(bitset의 크기로 사용)
	};

	// 생성자: 기본Type은 Initialize에 넣어주시고, 사용자 정의 타입은 멤버 초기화 리스트에서 초기화 해주세요
	CBattleFieldRiskInfo() 
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
        : m_uRefCount(0)
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
    { _OnCreate(); }

	bool ParsingScriptFile( IN KLuaManager& luaManager_ );	// CBattleFieldRiskInfo에 필요한 정보를 읽어들인다

	void Initialize() {
		_OnCreate();	// 기본Type 초기화
		// 여기 부터는 기본Type이 아닌 것 초기화	
		m_bitsetFlagRiskInfo.reset();
	}

	UINT GetMonsterIdToBeSpawned() const { return m_uiMonsterIdToBeSpawned; }
	void SetMonsterIdToBeSpawned( const UINT uiMonsterIdToBeSpawned_ ) { m_uiMonsterIdToBeSpawned = uiMonsterIdToBeSpawned_; }

	USHORT GetRiskValue() const { return m_usRiskValue; }
	void SetRiskValue( const USHORT usRiskValue_ ) { m_usRiskValue = usRiskValue_; }

	bool GetBitSetFlag( const FLAG_RISK_INFO eFlagRiskInfo_ ) { m_bitsetFlagRiskInfo.test( eFlagRiskInfo_ ); }
	void SetBitSetFlag( const FLAG_RISK_INFO eFlagRiskInfo_, const bool bFlag_ ) {
		m_bitsetFlagRiskInfo.set( eFlagRiskInfo_, bFlag_ );
	}

#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
    void    AddRef()    {   ++m_uRefCount; }
    void    Release()   { if ( (--m_uRefCount) == 0 )   delete this; }
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

private:

	/// 생성자에서 기본Type의 변수들을 초기화 시키기 위한 용도로 사용
	void _OnCreate() {
		m_uiMonsterIdToBeSpawned	= 0;
		m_usRiskValue				= 0;
	}

private:
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
    unsigned                                        m_uRefCount;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

	UINT			m_uiMonsterIdToBeSpawned;			/// 위험도 수치에 따라 출현할 몬스터
	USHORT			m_usRiskValue;						/// 위험도 수치
	
	std::bitset<FRI_FLAG_MAX>	m_bitsetFlagRiskInfo;	/// 위험도 정보에서 쓰이는 Flag의 BitSet
};

IMPLEMENT_INTRUSIVE_PTR( CBattleFieldRiskInfo );

#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS

class CBattleFieldMiddleBossInfo		// 필드 중간 보스에 대한 정보를 담고 있는 클래스, BattleFieldData.lua 에서 파싱
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
    : private boost::noncopyable
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
{
	public :
		CBattleFieldMiddleBossInfo ()  
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
        : m_uRefCount(0)
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
        { _OnCreate(); }

		UINT GetMonsterSpawnID() const { return m_uiMonsterSpawnID; }
		void SetMonsterSpawnID(UINT val) { m_uiMonsterSpawnID = val; }


		USHORT GetRiskValue() const { return m_usRiskValue; }
		void SetRiskValue(USHORT val) { m_usRiskValue = val; }

		bool GetIsSplitBossGage() const { return m_bIsSplitBossGage; }
		void SetIsSplitBossGage(bool val) { m_bIsSplitBossGage = val; }

		bool GetIsShowBossGage() const { return m_bIsShowBossGage; }
		void SetIsShowBossGage(bool val) { m_bIsShowBossGage = val; }

		USHORT GetSpawnRate() const { return m_usSpawnRate; }
		void SetSpawnRate(USHORT val) { m_usSpawnRate = val; }

		USHORT GetMonsterSpawnMany() const { return m_usMonsterSpawnMany; }
		void SetMonsterSpawnMany(USHORT val) { m_usMonsterSpawnMany = val; }

		wstring GetMainMonsterName() const { return m_wstrMainMonsterName; }
		void SetMainMonsterName(wstring val) { m_wstrMainMonsterName = val; }

		bool ParsingScriptFile ( IN KLuaManager& luaManager_ );	

#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
        void    AddRef()    {   ++m_uRefCount; }
        void    Release()   { if ( (--m_uRefCount) == 0 )   delete this; }
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

	private : 

		void _OnCreate()
		{
			m_uiMonsterSpawnID = 0;
			m_usSpawnRate = 0;
			m_usRiskValue = 0;
			m_bIsShowBossGage = 0;
			m_bIsSplitBossGage = 0;
			m_usMonsterSpawnMany = 0;
		}

		void Initialize ()
		{
			_OnCreate();
			m_wstrMainMonsterName.clear();	
			m_iBossGroupStringIndex = 0;
			m_vecMonsterSpawnGroupID.clear();
		}

#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
        unsigned            m_uRefCount;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
		USHORT				m_usMonsterSpawnMany;
		UINT				m_uiMonsterSpawnID;		
		std::vector<UINT>	m_vecMonsterSpawnGroupID;		
		USHORT				m_usSpawnRate;		
		USHORT				m_usRiskValue;
		bool				m_bIsShowBossGage;
		bool				m_bIsSplitBossGage;
		int					m_iBossGroupStringIndex;	// 몬스터 그룹을 나타내는 String Index, Parsing 에 쓰임
		wstring				m_wstrMainMonsterName;		// 몬스터 그룹 이름
};

IMPLEMENT_INTRUSIVE_PTR( CBattleFieldMiddleBossInfo );

#endif // SERV_BATTLEFIELD_MIDDLE_BOSS



/** @class : CBattleFieldData
@brief : 필드에 대한 정보를 모아 놓은 클래스
@date  : 2011/10/25
*/

class CBattleFieldData
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
    : private boost::noncopyable
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
{

public:

#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
	typedef boost::intrusive_ptr<CBattleFieldPortalMovingInfo> CBattleFieldPortalMovingInfoPtr;
	typedef boost::intrusive_ptr<CBattleFieldRiskInfo> CBattleFieldRiskInfoPtr;
#else   X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
	typedef boost::shared_ptr<CBattleFieldPortalMovingInfo> CBattleFieldPortalMovingInfoPtr;
	typedef boost::shared_ptr<CBattleFieldRiskInfo> CBattleFieldRiskInfoPtr;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
	typedef boost::shared_ptr<CBattleFieldMiddleBossInfo> CBattleFieldMiddleBossInfoPtr;
#endif // SERV_BATTLEFIELD_MIDDLE_BOSS

	CBattleFieldData() 
		: m_wstrBattleFieldName(), m_wstrBattleFieldTextureName(), m_uiReturnVillageId( 0 ) 
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
        , m_uRefCount(0)
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR    
    {
		_OnCreate();
	}

	void Initialize() {
		_OnCreate();// 기본Type 초기화
		// 여기 부터는 기본Type이 아닌 것 초기화	
		m_wstrBattleFieldName.clear();
		m_wstrBattleFieldTextureName.clear();
		m_vecBattleFieldPortalMovingInfoPtr.resize(0);
		m_vecBattleFieldRiskInfoPtr.resize(0);
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
		m_vecBattleFieldMiddleBossInfoPtr.resize(0);	
#endif // SERV_BATTLEFIELD_MIDDLE_BOSS
	}

	bool ParsingScriptFile( IN KLuaManager& luaManager_ );	// CBattleFieldData에 필요한 정보를 읽어들인다

	UINT GetBattleFieldId() const { return m_uiBattleFieldId; }
	void SetBattleFieldId( const UINT uiBattleFieldId_ ) { m_uiBattleFieldId = uiBattleFieldId_; }

	UINT GetWorldId() const { return m_uiWorldId; }
	void SetWorldId( const UINT uiWorldId_ ) { m_uiWorldId = uiWorldId_; }

	UINT GetStandardMonsterLevel() const { return m_uiStandardMonsterLevel; }
	void SetStandardMonsterLevel( const UINT uiStandardMonsterLevel_ ) { m_uiStandardMonsterLevel = uiStandardMonsterLevel_; }

	USHORT GetMaxNumberOfMonsterInThisBattleField() const { 
		return m_usMaxNumberOfMonsterInThisBattleField; }
	void SetMaxNumberOfMonsterInThisBattleField( USHORT usMaxNumberOfMonsterInThisBattleField_ ) {
		m_usMaxNumberOfMonsterInThisBattleField = usMaxNumberOfMonsterInThisBattleField_; }

	const WCHAR* GetBattleFieldName() const { return m_wstrBattleFieldName.c_str(); }
	void SetBattleFieldName( const WCHAR* wszBattleFieldName_ ) { m_wstrBattleFieldName = wszBattleFieldName_; }

	const WCHAR* GetBattleFieldTextureName() const { 
		return ( m_wstrBattleFieldName.empty() ? NULL : m_wstrBattleFieldTextureName.c_str() ); }
	void SetBattleFieldTextureName( const WCHAR* wszBattleFieldTextureName_ ) {
		m_wstrBattleFieldTextureName = wszBattleFieldTextureName_; }

	const WCHAR* GetBattleFieldTextureKey() const { 
		return ( m_wstrBattleFieldTextureKey.empty() ? NULL : m_wstrBattleFieldTextureKey.c_str() ); }
	void SetBattleFieldTextureKey( const WCHAR* wszBattleFieldTextureKey_ ) {
		m_wstrBattleFieldTextureKey = wszBattleFieldTextureKey_; }

	UINT GetReturnVillageId() const { return m_uiReturnVillageId; }
	void SetReturnVillageId( const UINT uiReturnVillageId_ ) { m_uiReturnVillageId = uiReturnVillageId_; }

	CBattleFieldPortalMovingInfoPtr GetBattleFieldPortalMovingInfoPtrByLineMapIndex( const UINT uiLineMapIndex_ );
#ifdef QUEST_GUIDE
	bool GetBattleFieldPortalMovingInfo( OUT map<UINT, UINT>& mapBattleFieldMovingInfo_);
#endif //QUEST_GUIDE

#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
	wstring GetBattleFieldMiddleBossInfoGroupName ( int iSpawnGroup_ )
	{
		std::vector<CBattleFieldMiddleBossInfoPtr>::iterator it = m_vecBattleFieldMiddleBossInfoPtr.begin();
		for ( ; it != m_vecBattleFieldMiddleBossInfoPtr.end(); ++it )
		{
			if ( (*it) != NULL )  
			{
				if ( (*it)->GetMonsterSpawnID() == iSpawnGroup_ )		// 소환된 그룹 명과 검색 중인 해당 그룹의 이름이 같다면
				{
					return (*it)->GetMainMonsterName();					// 해당 그룹 명을 반환한다.
				}
			}
		}		
		return L"NoData";
	}
#endif // SERV_BATTLEFIELD_MIDDLE_BOSS

#ifdef FIELD_BOSS_RAID
	bool GetIsBossRaidField() const { return m_bIsBossRaidField; }
	const USHORT GetRaidFieldPortalLineIndex() const { return m_usRaidFieldPortalPositionIndex; }
#endif // FIELD_BOSS_RAID

#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
    void    AddRef()    {   ++m_uRefCount; }
    void    Release()   { if ( (--m_uRefCount) == 0 )   delete this; }
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

private:

	void _OnCreate() {
		m_uiBattleFieldId = 0;
		m_uiWorldId = 0;
		m_uiStandardMonsterLevel = 0;
		m_usMaxNumberOfMonsterInThisBattleField = 0;
#ifdef FIELD_BOSS_RAID
		m_bIsBossRaidField = false;
		m_usRaidFieldPortalPositionIndex = 0;
#endif // FIELD_BOSS_RAID
	}

	// CBattleFieldPortalMovingInfo의 shared_ptr 생성
	CBattleFieldPortalMovingInfoPtr CreateBattleFieldPortalMovingInfoPtr() { 
		return CBattleFieldPortalMovingInfoPtr( new CBattleFieldPortalMovingInfo() ); }

	// CBattleFieldRiskInfo의 shared_ptr 생성
	CBattleFieldRiskInfoPtr CreateBattleFieldRiskInfoPtr() { 
		return CBattleFieldRiskInfoPtr( new CBattleFieldRiskInfo() ); }

	bool ParsingBattleFieldMovingInfo( IN KLuaManager& luaManager_ );	// BattleFieldPortalMovingInfo 파싱
	bool ParsingBattleFieldRiskInfo( IN KLuaManager& luaManager_ );	// BattleFieldRiskInfo 파싱

#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
	CBattleFieldMiddleBossInfoPtr CreateBattleFieldMiddleBossInfoPtr() 
	{ 
		return CBattleFieldMiddleBossInfoPtr( new CBattleFieldMiddleBossInfo() ); 
	}

	void ParsingBattleFieldMiddleBossInfo ( IN KLuaManager& luaManager_ );	// BATTLE_FIELD_MIDDLE_BOSS_INFO 파싱

#endif // SERV_BATTLEFIELD_MIDDLE_BOSS

private:
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
    unsigned                                        m_uRefCount;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
	UINT		m_uiBattleFieldId;				/// 현재 필드의 ID, 선언은 UINT로 했지만 BattleField_ID 값을 대입
	UINT		m_uiWorldId;				/// 현재 필드가 사용하는 CX2World::WORLD_ID enum값
	UINT		m_uiStandardMonsterLevel;	/// 현재 필드에서 나오는 몬스터들의 기준 레벨 (일단 가지고 있기는 하지만.. 클라이언트에서 사용하는지는 두고 보자)
	UINT		m_uiReturnVillageId;		/// 유저가 이 필드에서 죽고서 부활하지 않을 경우 돌아가게될 마을ID
	
	USHORT		m_usMaxNumberOfMonsterInThisBattleField;	/// 현재 필드에서 최대로 나올 수 있는 몬스터 수
	
	// 이름이 필요한지 생각해 보자
	wstring		m_wstrBattleFieldName;			/// 필드 명 (국가마다 언어가 다르기 때문에 wstring 객체 사용)
	
	wstring		m_wstrBattleFieldTextureName;		/// 필드 명을 나타내는 텍스쳐 명
	wstring		m_wstrBattleFieldTextureKey;		/// 필드 명을 나타내는 텍스쳐 키
	
	std::vector<CBattleFieldPortalMovingInfoPtr> m_vecBattleFieldPortalMovingInfoPtr;	/// BattleFieldPortalMovingInfo의 shared_ptr을 담은 vector
	std::vector<CBattleFieldRiskInfoPtr> m_vecBattleFieldRiskInfoPtr;	/// BattleFieldRiskInfo의 shared_ptr을 담은 vector

#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
	std::vector<CBattleFieldMiddleBossInfoPtr> m_vecBattleFieldMiddleBossInfoPtr;	/// BattleFieldMiddleBossInfo의 shared_ptr을 담은 vector
#endif // SERV_BATTLEFIELD_MIDDLE_BOSS

#ifdef FIELD_BOSS_RAID
	bool		m_bIsBossRaidField;
	USHORT		m_usRaidFieldPortalPositionIndex;	// 보스 레이드 필드로 이동하는 포탈 포지션 인덱스
#endif // FIELD_BOSS_RAID

};

IMPLEMENT_INTRUSIVE_PTR( CBattleFieldData );


/** @class : CX2BattleFieldManager
@brief : 엘소드 필드의 Data를 루아로 부터 파싱하고, 해당 BattleFieldData를 관리하는 클래스
@date  : 2011/10/25
*/
class CX2BattleFieldManager {

public:

#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
    typedef boost::intrusive_ptr<CBattleFieldData> CBattleFieldDataPtr;
#else   X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
	typedef boost::shared_ptr<CBattleFieldData> CBattleFieldDataPtr;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

	enum PORTAL_MOVE_TYPE {
		PMT_INVALID = 0,
		PMT_MOVE_TO_VILLAGE,				/// 필드 -> 마을로 이동하는 포탈
		PMT_MOVE_TO_BATTLE_FIELD,			/// 필드 -> 필드로 이동하는 포탈
		PMT_MOVE_TO_RAID_FIELD,				/// 필드 -> 레이드 필드로 이동하는 포탈
		PMT_END,
	};

public:

	struct BATTLE_FIELD_POSITION_INFO {
		UINT		m_uiBattleFieldIdWhereIam;		/// 현재 자신이 있는 곳의 필드 ID (필드 이동 시 갱신 한다.)
		USHORT		m_usBattleFieldPositionIndexWhereIShouldBe;		/// 필드간 또는 다른 곳에서의 필드로 이동 시에 자신이 있어야 할 필드내의 Position 인덱스
		USHORT		m_usBattleFieldPostionValue;	/// PositionIndex로 지정된 라인맵데이터에서 몇 % 위치에 해당하는지에 대한 정보
		bool		m_bStartPosition;	/// 라인맵의 Start정보 인가? 
		// (m_bStartPosition 가 true이면 m_usBattleFieldPostionValue가 무의미하고, m_usBattleFieldPositionIndexWhereIShould가 LineMapData의 Index가 아니라 StartPos Index임)

		BATTLE_FIELD_POSITION_INFO() : m_uiBattleFieldIdWhereIam( 0 ), m_usBattleFieldPositionIndexWhereIShouldBe( 0 ),
			m_usBattleFieldPostionValue( 0 ), m_bStartPosition( false )
		{}
	};

	CX2BattleFieldManager() : m_BattleFieldPositionInfo()
	{}

	void OpenScriptFile( const wchar_t* wszScriptFileName_ );
	void AddBattleFieldData_LUA();
	void SetMonsterRespawnFactorByUserCount_LUA();		// dummy
	void SetMonsterRespawnTimeMinMax_LUA();				// dummy
	void SetBattleFieldFactor_LUA();					// dummy

	void ReOpenScriptFile( const wchar_t* wszScriptFileName_ );

	// CBattleFieldData의 shared_ptr 생성
	CBattleFieldDataPtr CreateBattleFieldDataPtr() { return CBattleFieldDataPtr( new CBattleFieldData() ); }	

	// 현재 자신이 있는 곳의 필드 ID (필드 이동 시 갱신 한다.)
	UINT GetBattleFieldIdWhereIam() const { return m_BattleFieldPositionInfo.m_uiBattleFieldIdWhereIam; }
	void SetBattleFieldIdWhereIam( const UINT uiBattleFieldIdWhereIam_  ) { m_BattleFieldPositionInfo.m_uiBattleFieldIdWhereIam = uiBattleFieldIdWhereIam_; }

	// 필드간 또는 다른 곳에서의 필드로 이동 시에 자신이 있어야 할 필드내의 Position 인덱스
	USHORT GetBattleFieldPositionIndexWhereIShouldBe() const { return m_BattleFieldPositionInfo.m_usBattleFieldPositionIndexWhereIShouldBe; }
	void SetBattleFieldPositionIndexWhereIShouldBe( const USHORT usBattleFieldPositionIndexWhereIShouldBe_ ) 
	{ 
		m_BattleFieldPositionInfo.m_usBattleFieldPositionIndexWhereIShouldBe = usBattleFieldPositionIndexWhereIShouldBe_; 
	}

	USHORT GetBattleFieldPositionValue() const { return m_BattleFieldPositionInfo.m_usBattleFieldPostionValue; }
	void SetBattleFieldPositionValue( const USHORT usBattleFieldPositionValue_ ) { m_BattleFieldPositionInfo.m_usBattleFieldPostionValue = usBattleFieldPositionValue_; }

	bool GetNowBattleFieldPositionInfoStartPosition() const { return m_BattleFieldPositionInfo.m_bStartPosition; }
	void SetNowBattleFieldPositionInfoStartPosition( const bool bNowFieldPositionInfoStartPosition_ ) { m_BattleFieldPositionInfo.m_bStartPosition = bNowFieldPositionInfoStartPosition_; }

	UINT GetWorldIdByBattleFieldId( const UINT uiBattleFieldId_ ) const;
	CBattleFieldData::CBattleFieldPortalMovingInfoPtr GetPortalMovingInfoByLineMapIndexInNowBattleField( const int iLineMapIndex_ ) const;

	UINT GetReturnVillageId( UINT uiBattleFieldId_ = -1 ) const;

	bool GetBattleFieldTextureNameAndKey( IN const UINT uiBattleFieldId_, OUT const WCHAR** pTextureName_, OUT const WCHAR** pKeyName_ );

#ifdef REFORM_QUEST
	const WCHAR* GetBattleFieldNameByBattleFieldId( const UINT uiBattleFieldId_ ) const;
	bool		 GetBattleFieldPortalMovingInfo( OUT map<UINT, UINT>& mapBattleFieldMovingInfo_);
#endif //REFORM_QUEST

#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
	wstring GetBattleFieldBossDataGroupName ( UINT uiBattleFieldId_, int iBattleFieldBossGroupId_ );	// Field ID 와 Boss Group ID 를 받아서 보스 그룹 명을 반환함

#endif // SERV_BATTLEFIELD_MIDDLE_BOSS

#ifdef FIELD_BOSS_RAID
	bool		GetIsBossRaidFieldByFieldID( const UINT uiBattleFieldID_ ) const;
	bool		GetIsBossRaidCurrentField() const;
	const USHORT GetRaidFieldPortalLineByFieldID( const UINT uiBattleFieldID_ ) const;
#endif // FIELD_BOSS_RAID

private:
	typedef std::map<UINT, CBattleFieldDataPtr> BattleFieldDataPtrMap;
	typedef std::pair<UINT, CBattleFieldDataPtr> BattleFieldDataPtrPair;

	BattleFieldDataPtrMap	m_mapBattleFieldDataPtr;	/// key: BattleField_ID, value: CBattleFieldDataPtr

	BATTLE_FIELD_POSITION_INFO	m_BattleFieldPositionInfo;
};