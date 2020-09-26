#define KX2ITEMMANAGER_FORMAT_MAGIC           (MAKEFOURCC('K','I','M',' '))
#define KX2ITEMMANAGER_FORMAT_VERSION         0x1201

static const int        MAX_MODEL_COUNT_A_ITEM = 6;

enum ITEM_TYPE
{
    IT_NONE = 0,		/// 에러
    IT_WEAPON,			/// 무기
    IT_DEFENCE,			/// 방어구
    IT_ACCESSORY,		/// 액세서리
    IT_SKILL,			/// 스킬
    IT_QICK_SLOT,		/// 퀵슬롯 아이템(소비성)
    IT_MATERIAL,		/// 재료
    IT_SPECIAL,			/// 특수            
    IT_QUEST,			/// 퀘스트
    IT_OUTLAY,			/// 순간소비성
    IT_ETC,				/// 기타
    IT_SKILL_MEMO,		/// 스킬메모
//#ifdef SERV_UPGRADE_TRADE_SYSTEM // 김태환
	IT_PET,				/// 펫
	IT_RIDING,			/// 탈것
//#endif //SERV_UPGRADE_TRADE_SYSTEM
	IT_NUM,
};

enum ITEM_GRADE
{
    IG_NONE = 0,
    IG_UNIQUE,
    IG_ELITE,
    IG_RARE,
    IG_NORMAL,
    IG_LOW,
    IG_NUM,
};

enum PERIOD_TYPE
{
    PT_INFINITY = 0,    /// 무제한
    PT_ENDURANCE,        /// 내구도
    PT_QUANTITY,        /// 수량성
    PT_NUM,
};

enum SHOP_PRICE_TYPE
{
    SPT_NONE = 0,        /// 팔기 불능
    SPT_CASH,            /// 캐쉬
    SPT_GP,                /// GP
    SPT_NUM,
};

enum USE_CONDITION
{
    UC_NONE = 0,        /// 아무도 사용할 수 없음
    UC_ANYONE,            /// 아무나 사용할 수 있음
    UC_ONE_UNIT,        /// 지정된 unit_type 만
    UC_ONE_CLASS,        /// 지정된 unit_class 만
    UC_NUM,
};

enum USE_TYPE
{
    UT_NONE = 0,        /// 장착하는 장비가 아님.
    UT_SKIN,            /// 장착하는 장비, 스킨 애니메이션
    UT_ATTACH_ANIM,        /// 장착하는 장비, 어태치 애니메이션
    UT_ATTACH_NORMAL,    /// 장착하는 장비, 어태치 메시
    UT_NUM,
};

enum SPECIAL_ABILITY_TYPE
{
    SAT_NONE = 0,                    /// 없음
    SAT_HP_UP,                        /// HP증가
    SAT_MP_UP,                        /// MP증가
    SAT_REMOVE_CURSE,                /// 저주제거
    SAT_REMOVE_SLOW,                /// 슬로우제거
    SAT_REMOVE_FIRE,                /// 화염제거
    SAT_REMOVE_POISON,                /// 독제거
    SAT_RECIPE,                        /// 레시피 아이템
    SAT_SUPPLEMENT_ITEM,            /// 보충제 아이템
    SAT_HYPER_MODE,                    /// 각성
    SAT_SOUL_GAGE_UP,                /// 각성게이지 상승
    SAT_SHOW_OPPONENT_MP,            /// 상대팀 MP 보기
    SAT_UP_MP_AT_ATTACK_OR_DAMAGE,    /// 때리거나 맞을때 MP 상승
    SAT_HP_PERCENT_UP,                /// HP 퍼센트 증가
    SAT_TEAM_HP_UP,                    /// 팀원들 전부 HP % 증가
    SAT_TEAM_MP_UP,                    /// 팀원들 전부 MP % 증가
    SAT_PEPPER_RUN,                    /// 고추먹고 맴맴
    SAT_TRANSFORM_UNIT_SCALE,        /// 유닛 스케일 조정
    SAT_SPECIAL_SKILL,                /// 소비성 아이템을 사용함에 의한 특수 기술
    SAT_MP_PERCENT_UP,                /// MP 퍼센트 증가

//#ifdef DUNGEON_ITEM
    SAT_POWER_RATE_UP,                /// 공격력 증가 (%)
    SAT_MOVE_SPEED_UP,                /// 이동 속도 증가 (%)
    SAT_JUMP_SPEED_UP,                /// 점프 속도 증가 (%)
    SAT_MP_REGENERATION,            /// 초당 mp 회복
    SAT_USE_HYPER,                    /// 각성구슬 없이 강제 각성            
    SAT_PHYSIC_ATTACK_UP,            /// 물리 공격력 증가 (%)
    SAT_MAGIC_ATTACK_UP,            /// 마법 공격력 증가 (%)
    SAT_PHYSIC_DEFENCE_UP,            /// 물리 방어력 증가 (%)
    SAT_MAGIC_DEFENCE_UP,            /// 마법 방어력 증가 (%)
    SAT_SUPERARMOR,                    /// 슈퍼아머    
    SAT_SUMMON_SPIRIT,                /// 정령소환
//#endif //DUNGEON_ITEM
    SAT_REMOVE_FROZEN,                // 냉기제거
    SAT_TRANSFORM_MONSTER,            // 몬스터 변신            
    SAT_WAY_OF_SWORD_GAUGE_UP,        // 검의길 게이지 증가                
//#ifdef SUMMON_MONSTER_CARD_SYSTEM
    SAT_SUMMON_MONSTER,                // 몬스터 소환 기능
//#endif //SUMMON_MONSTER_CARD_SYSTEM
    SAT_FORCE_CRITICAL_MAX,            // 다음 타격 크리 100%
    SAT_ENABLE_ATTACK_MONSTER,         // 해외 할로윈 이벤트
//#ifdef ARA_FORCE_POWER_ITEM
	SAT_ARA_FORCE_POWER_PERCENT_UP,    // 해외 아라 기력 증가 (%)
//#endif ARA_FORCE_POWER_ITEM
//#ifdef RETURN_TO_BASE_AT_PVE
	SAT_RETURN_TO_BASE_AT_PVE,			// PVE 귀환석
//#endif // RETURN_TO_BASE_AT_PVE
//#ifdef RIDINGPET_STAMINA_ITEM
	SAT_RIDINGPET_STAMINA_PERCENT_UP,    // 해외 라이딩펫 스테미너 증가 (%)
//#endif RIDINGPET_STAMINA_ITEM
    SAT_END,
};

//#ifdef DUNGEON_ITEM
enum SPECIAL_ITEM_TYPE
{
    SIT_NONE = 0,
    SIT_NOSTRUM,        /// 비약
    SIT_GENIUS,            /// 정령소환
    SIT_CREST,            /// 문장
    SIT_SUMMON_MONSTER, /// 몬스터 소환 : 실패없음
    SIT_END,
};
//#endif  DUNGEON_ITEM



#ifdef    X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING

#pragma pack( push, 1 )


struct	KItemFormatHeader
{
	DWORD				m_dwMagic;
	DWORD				m_dwVersion;
	DWORD				m_dwNumItems;
    DWORD               m_dwNumSetIDs;

    KItemFormatHeader()
    {
        m_dwMagic = 0;
        m_dwVersion = 0;
        m_dwNumItems = 0;
        m_dwNumSetIDs = 0;
    }
};//


enum    ESlashTraceType
{
    STT_SLASH_TRACE = 0,
    STT_SLASH_TRACE_TIP = 1,
    STT_HYPER_SLASH_TRACE = 2,
    STT_HYPER_SLASH_TRACE_TIP = 3,
    STT_END,
};

struct  KItemFormatType_OffsetOrValue
{
    DWORD   m_dwType;
    DWORD   m_dwOffsetOrValue;

    KItemFormatType_OffsetOrValue()
    {
        m_dwType = 0;
        m_dwOffsetOrValue = 0;
    }
};

struct KItemFormatSpecialAbility
{
    DWORD   m_dwType;
    int     m_CoolTime;
    int     m_Value1;
    int     m_Value2;
    int     m_Value3;
//#ifdef SUMMON_MONSTER_CARD_SYSTEM
    DWORD   m_dwOffset_StringValue1;        //Group으로 된 정보를 받아 올 수 있는 인자
//#endif SUMMON_MONSTER_CARD_SYSTEM

    KItemFormatSpecialAbility()
    {
        m_dwType            = (DWORD) SAT_NONE;
        m_CoolTime        = 0;
        m_Value1        = 0;
        m_Value2        = 0;
        m_Value3        = 0;
//#ifdef SUMMON_MONSTER_CARD_SYSTEM
//                m_StringValue1    = L"Group 0";
//#endif SUMMON_MONSTER_CARD_SYSTEM
        m_dwOffset_StringValue1 = 0;
    }

    void    SetType( SPECIAL_ABILITY_TYPE eType )   { m_dwType = (DWORD) eType; }
    SPECIAL_ABILITY_TYPE    GetType() const         { return (SPECIAL_ABILITY_TYPE) m_dwType; }
    const wchar_t*  GetStringValue1() const;
};

//#ifdef USER_WEAPON_PARTICLE_SCRIPT_TEST
struct  KItemFormatAttachedData
{
    DWORD   m_dwOffset_Name;
    DWORD   m_dwOffset_BoneName; 

    KItemFormatAttachedData()
    {
        m_dwOffset_Name = 0;
        m_dwOffset_BoneName = 0;
    }
};
//#endif USER_WEAPON_PARTICLE_SCRIPT_TEST

struct KItemFormatStatData
{
	float m_fBaseHP;
	float m_fAtkPhysic;
	float m_fAtkMagic;

	//방어
	float m_fDefPhysic;
	float m_fDefMagic;

	KItemFormatStatData()
	{
		m_fBaseHP			= 0;
		m_fAtkPhysic		= 0;
		m_fAtkMagic			= 0;

		m_fDefPhysic		= 0;
		m_fDefMagic			= 0;
	}

#ifndef X2OPTIMIZE_SCRIPT_PREPROCESSING_TOOL
    void    AssignToStat( CX2Stat::Stat& statOut_ ) const
    {
        statOut_.m_fBaseHP			= m_fBaseHP;
		statOut_.m_fAtkPhysic		= m_fAtkPhysic;
		statOut_.m_fAtkMagic		= m_fAtkMagic;
		statOut_.m_fDefPhysic		= m_fDefPhysic;
		statOut_.m_fDefMagic		= m_fDefMagic;

		statOut_.m_ExtraStat.m_fIncreaseHPRate = 0.f;
#ifdef PET_AURA_SKILL
		statOut_.m_ExtraStat.m_fIncreaseMPRate = 0.f;
		statOut_.m_ExtraStat.m_fIncreaseAtkPhysicRate = 0.f;
		statOut_.m_ExtraStat.m_fIncreaseAtkMagicRate = 0.f;
		statOut_.m_ExtraStat.m_fIncreaseDefPhysicRate = 0.f;
		statOut_.m_ExtraStat.m_fIncreaseDefMagicRate = 0.f;
#endif
    }

	void AddToStat( CX2Stat::Stat& statInOut_ ) const
	{
		statInOut_.m_fBaseHP		+= m_fBaseHP;

		statInOut_.m_fAtkPhysic		+= m_fAtkPhysic;
		statInOut_.m_fAtkMagic		+= m_fAtkMagic;

		statInOut_.m_fDefPhysic		+= m_fDefPhysic;
		statInOut_.m_fDefMagic		+= m_fDefMagic;

		statInOut_.LimitMaximum();
	}

	void DecFromStat( CX2Stat::Stat& statInOut_ ) const
    {
		statInOut_.m_fBaseHP			-= m_fBaseHP;
		statInOut_.m_fAtkPhysic		-= m_fAtkPhysic;
		statInOut_.m_fAtkMagic			-= m_fAtkMagic;
		statInOut_.m_fDefPhysic		-= m_fDefPhysic;
		statInOut_.m_fDefMagic			-= m_fDefMagic;
		statInOut_.LimitMaximum();
    }

#endif  X2OPTIMIZE_SCRIPT_PREPROCESSING_TOOL
};

//#ifdef ADD_ITEM_TEMPLET_ITEM
struct KItemFormatStatRelLVData
{
	//체력
	BYTE m_byBaseHPRelLV;
	//공격
	BYTE m_byAtkPhysicRelLV;
	BYTE m_byAtkMagicRelLV;
	//방어
	BYTE m_byDefPhysicRelLV;
	BYTE m_byDefMagicRelLV;

	KItemFormatStatRelLVData()
	{
		m_byBaseHPRelLV		= 0;
		m_byAtkPhysicRelLV	= 0;
		m_byAtkMagicRelLV	= 0;

		m_byDefPhysicRelLV	= 0;
		m_byDefMagicRelLV	= 0;
	}

	inline bool IsValideCheck() const
	{
		return 0 != (m_byBaseHPRelLV + m_byAtkPhysicRelLV + m_byAtkMagicRelLV + m_byDefPhysicRelLV + m_byDefMagicRelLV);
	}
	void AddStat( const KItemFormatStatRelLVData& sStatRelLVData_)
	{
		this->m_byBaseHPRelLV += sStatRelLVData_.m_byBaseHPRelLV;
		this->m_byAtkPhysicRelLV += sStatRelLVData_.m_byAtkPhysicRelLV;
		this->m_byAtkMagicRelLV += sStatRelLVData_.m_byAtkMagicRelLV;
		this->m_byDefPhysicRelLV += sStatRelLVData_.m_byDefPhysicRelLV;
		this->m_byDefMagicRelLV += sStatRelLVData_.m_byDefMagicRelLV;
	}
	bool operator < ( const KItemFormatStatRelLVData& rhs ) const 
	{
		if( m_byBaseHPRelLV < rhs.m_byBaseHPRelLV )
			return true;
		else if( m_byBaseHPRelLV > rhs.m_byBaseHPRelLV )
			return false;

		if( m_byAtkPhysicRelLV < rhs.m_byAtkPhysicRelLV )
			return true;
		else if( m_byAtkPhysicRelLV > rhs.m_byAtkPhysicRelLV )
			return false;

		if( m_byAtkMagicRelLV < rhs.m_byAtkMagicRelLV )
			return true;
		else if( m_byAtkMagicRelLV > rhs.m_byAtkMagicRelLV )
			return false;

		if( m_byDefPhysicRelLV < rhs.m_byDefPhysicRelLV )
			return true;
		else if( m_byDefPhysicRelLV > rhs.m_byDefPhysicRelLV )
			return false;

		if( m_byDefMagicRelLV < rhs.m_byDefMagicRelLV )
			return true;

		return false;
	}

	void Init()
	{
		m_byBaseHPRelLV = 0;
		m_byAtkPhysicRelLV = 0;
		m_byAtkMagicRelLV = 0;
		m_byDefPhysicRelLV = 0;
		m_byDefMagicRelLV = 0;
	}
};
//#endif // ADD_ITEM_TEMPLET_ITEM


struct  KItemForamtNeedPartsNumAndOption
{
    DWORD               m_dwNeedPartsNum;
    int                 m_iOption;

    KItemForamtNeedPartsNumAndOption()
    {
        m_dwNeedPartsNum = 0;
        m_iOption = 0;
    }
    bool operator < ( const KItemForamtNeedPartsNumAndOption& rhs_ ) const
    {
        return  m_dwNeedPartsNum < rhs_.m_dwNeedPartsNum;
    }
    bool operator < ( DWORD rhs_ ) const
    {
        return  m_dwNeedPartsNum < rhs_; 
    }
    friend bool operator < ( DWORD lhs_, const KItemForamtNeedPartsNumAndOption& rhs_ )
    {
        return lhs_ < rhs_.m_dwNeedPartsNum;
    }
};


struct    KItemFormatSetItemData
{
    DWORD               m_dwSetID;
//{{ kimhc // 2011-07-05 // 옵션데이타 수치화 작업
//#ifdef	NOT_USE_PERCENT_IN_OPTION_DATA
    int					m_iMaxLevel;	// m_SetID의 효과를 가지고 있는 Item 중 가장 높은 레벨
//#endif	NOT_USE_PERCENT_IN_OPTION_DATA
//}} kimhc // 2011-07-05 // 옵션데이타 수치화 작업
    DWORD               m_dwOffset_SetName;
    DWORD               m_dwOffset_ItemIDs;
    DWORD               m_dwOffset_NeedPartsNumNOptions;

	KItemFormatSetItemData()
    {
        m_dwSetID = 0;
#ifdef	NOT_USE_PERCENT_IN_OPTION_DATA
        m_iMaxLevel = 0;
#endif	NOT_USE_PERCENT_IN_OPTION_DATA
        m_dwOffset_SetName = 0;
        m_dwOffset_NeedPartsNumNOptions = 0;
        m_dwOffset_ItemIDs = 0;
    }

    const wchar_t*      GetSetName() const;
    void                GetSetItemNeedPartsNumNOptions( const KItemForamtNeedPartsNumAndOption*& pBegin, 
        const KItemForamtNeedPartsNumAndOption*& pEnd ) const;
    void                GetSetItemOptions( const int iNumOfEquippedItems_, IN OUT std::vector< int >& vecOptions ) const;
    void                GetSetItemList( OUT std::vector< DWORD >& vecItemID ) const;
    
};



struct    KItemFormatTemplet : public CX2ItemTemplet_Base
{
    friend class    CX2ItemManager;

    enum    EFlags
    {
        FLAG_BIT_FASHION = 0,
        FLAG_BIT_VESTED = 1,
        FLAG_BIT_CAN_ENCHANT = 2,
        FLAG_BIT_CAN_USE_INVENTORY = 3,
        FLAG_BIT_NO_EQUIP = 4,
        FLAG_BIT_IS_PC_BANG = 5,
        FLAG_BIT_CAN_HYPER_MODE = 6,
//#ifdef HIDE_SET_DESCRIPTION
	FLAG_BIT_HIDE_SET_DESC = 7,
//#endif HIDE_SET_DESCRIPTION
        FLAG_POS_USE_TYPE = 8,
        FLAG_NUM_USE_TYPE = 4,
        FLAG_POS_USE_CONDITION = 12,
        FLAG_NUM_USE_CONDITION = 4,
        FLAG_POS_ITEM_TYPE = 16,
        FLAG_NUM_ITEM_TYPE = 4,
        FLAG_POS_ITEM_GRADE = 20,
        FLAG_NUM_ITEM_GRADE = 4,
        FLAG_POS_PERIOD_TYPE = 24,
        FLAG_NUM_PERIOD_TYPE = 4,
        FLAG_POS_SHOP_PRICE_TYPE = 28,
        FLAG_NUM_SHOP_PRICE_TYPE = 4,
    };//

//#ifdef ADD_FLAGS_ITEM_PREPROCESSING
	enum    EFlags2
	{
//#ifdef SERV_PVP_EQUIPMENT
		FLAG_BIT_PVP_ITEM = 0,
//#endif // SERV_PVP_EQUIPMENT
	};//
//#endif ADD_FLAGS_ITEM_PREPROCESSING

    void            SetItemID( DWORD dwItemID )   { m_dwItemID = dwItemID; }
    DWORD           GetItemID() const       { return m_dwItemID; }

    void    SetFashion( bool bFashion )                     { _SetBit( FLAG_BIT_FASHION, bFashion ); }
    bool    GetFashion() const                              { return _GetBit( FLAG_BIT_FASHION ); }
    void    SetVested( bool bVested )                       { _SetBit( FLAG_BIT_VESTED, bVested ); }
    bool    GetVested() const                               { return _GetBit( FLAG_BIT_VESTED ); }
//#ifdef HIDE_SET_DESCRIPTION
    void    SetHideSetDesc( bool bHideSetDesc )     	    { _SetBit( FLAG_BIT_HIDE_SET_DESC, bHideSetDesc ); }
    bool    GetHideSetDesc() const                          { return _GetBit( FLAG_BIT_HIDE_SET_DESC ); }
//#endif HIDE_SET_DESCRIPTION
//#ifdef SERV_PVP_EQUIPMENT
    void    SetPvpItem( bool bPvpItem )              	    { _SetBit2( FLAG_BIT_PVP_ITEM, bPvpItem ); }
    bool    GetPvpItem() const               		    { return _GetBit2( FLAG_BIT_PVP_ITEM ); }
//#endif SERV_PVP_EQUIPMENT
    void    SetCanEnchant( bool bCanEnchant )               { _SetBit( FLAG_BIT_CAN_ENCHANT, bCanEnchant ); }
    bool    GetCanEnchant() const                           { return _GetBit( FLAG_BIT_CAN_ENCHANT ); }
    void    SetCanUseInventory( bool bCanUseInventory )     { _SetBit( FLAG_BIT_CAN_USE_INVENTORY, bCanUseInventory ); }
    bool    GetCanUseInventory() const                      { return _GetBit( FLAG_BIT_CAN_USE_INVENTORY ); }
    void    SetNoEquip( bool bNoEquip )                     { _SetBit( FLAG_BIT_NO_EQUIP, bNoEquip ); }
    bool    GetNoEquip() const                              { return _GetBit( FLAG_BIT_NO_EQUIP ); }
    void    SetIsPcBang( bool bIsPcBang )                   { _SetBit( FLAG_BIT_IS_PC_BANG, bIsPcBang ); }
    bool    GetIsPcBang() const                             { return _GetBit( FLAG_BIT_IS_PC_BANG ); }
    void    SetCanHyperMode( bool bCanHyperMode )           { _SetBit( FLAG_BIT_CAN_HYPER_MODE, bCanHyperMode ); }
    bool    GetCanHyperMode() const                         { return _GetBit( FLAG_BIT_CAN_HYPER_MODE ); }
    void    SetUseType( USE_TYPE eUseType )                 { _SetValue( FLAG_POS_USE_TYPE, FLAG_NUM_USE_TYPE, eUseType ); }
    USE_TYPE GetUseType() const                             { return (USE_TYPE) _GetValue( FLAG_POS_USE_TYPE, FLAG_NUM_USE_TYPE ); }
    void    SetUseCondition( USE_CONDITION eUseCondition )  { _SetValue( FLAG_POS_USE_CONDITION, FLAG_NUM_USE_CONDITION, eUseCondition ); }
    USE_CONDITION GetUseCondition() const                   { return (USE_CONDITION) _GetValue( FLAG_POS_USE_CONDITION, FLAG_NUM_USE_CONDITION ); }
    void    SetItemType( ITEM_TYPE eItemType )              { _SetValue( FLAG_POS_ITEM_TYPE, FLAG_NUM_ITEM_TYPE, eItemType ); }
    ITEM_TYPE GetItemType() const                           { return (ITEM_TYPE) _GetValue( FLAG_POS_ITEM_TYPE, FLAG_NUM_ITEM_TYPE ); }
    void    SetItemGrade( ITEM_GRADE eItemGrade )           { _SetValue( FLAG_POS_ITEM_GRADE, FLAG_NUM_ITEM_GRADE, eItemGrade ); }
    ITEM_GRADE GetItemGrade() const                         { return (ITEM_GRADE) _GetValue( FLAG_POS_ITEM_GRADE, FLAG_NUM_ITEM_GRADE ); }
    void    SetPeriodType( PERIOD_TYPE ePeriodType )        { _SetValue( FLAG_POS_PERIOD_TYPE, FLAG_NUM_PERIOD_TYPE, ePeriodType ); }
    PERIOD_TYPE GetPeriodType() const                       { return (PERIOD_TYPE) _GetValue( FLAG_POS_PERIOD_TYPE, FLAG_NUM_PERIOD_TYPE ); }
    void    SetShopPriceType( SHOP_PRICE_TYPE eShopPriceType ) { _SetValue( FLAG_POS_SHOP_PRICE_TYPE, FLAG_NUM_SHOP_PRICE_TYPE, eShopPriceType ); }
    SHOP_PRICE_TYPE GetShopPriceType() const                { return (SHOP_PRICE_TYPE) _GetValue( FLAG_POS_SHOP_PRICE_TYPE, FLAG_NUM_SHOP_PRICE_TYPE ); }

    void    SetItemLevel( int iItemLevel )                  { m_iItemLevel = iItemLevel; }
    int     GetItemLevel() const                            { return m_iItemLevel; }
    void    SetQuantity( int iQuantity )                    { m_iQuantity = iQuantity; }
    int     GetQuantity() const                             { return m_iQuantity; }
    void    SetRepairED( float fRepairED )                  { m_fRepairED = fRepairED; }
    float   GetRepairED() const                             { return m_fRepairED; }
    void    SetRepairVP( int iRepairVP )                    { m_iRepairVP = iRepairVP; }
    int     GetRepairVP() const                             { return m_iRepairVP; }
    void    SetPrice( int iPrice )                          { m_iPrice = iPrice; }
    int     GetPrice() const                                { return m_iPrice; }
    void    SetPricePvPPoint( int iPricePvPPoint )          { m_iPricePvPPoint = iPricePvPPoint; }
    int     GetPricePvPPoint() const                        { return m_iPricePvPPoint; }

    void    SetSetID( int iSetID )                          { m_iSetID = iSetID; }
    int     GetSetID() const                                { return m_iSetID; }
    void    SetAddMaxMP( float fAddMaxMP )                  { m_fAddMaxMP = fAddMaxMP; }
    float   GetAddMaxMP() const                             { return m_fAddMaxMP; }


    void    SetEndurance( int iEndurance )                  { m_sEndurance = (SHORT) iEndurance; }
    int     GetEndurance() const                            { return m_sEndurance; }
    void    SetEnduranceDamageMinMax( int iMin, int iMax )  { m_sEnduranceDamageMin = (SHORT) iMin; m_sEnduranceDamageMax = (SHORT) iMax; }
    int     GetEnduranceDamageMin() const                   { return m_sEnduranceDamageMin; }
    int     GetEnduranceDamageMax() const                   { return m_sEnduranceDamageMax; }

#ifdef  X2OPTIMIZE_SCRIPT_PREPROCESSING_TOOL
    void    SetSummonNpcID( USHORT usNPCID )                { m_usSummonNpcID = usNPCID; }
    USHORT  GetSummonNpcID() const                          { return m_usSummonNpcID; }
    void    SetBuyPvpRankCondition( BYTE byRank )           { m_byBuyPvpRankCondition = byRank; }
    BYTE    GetBuyPvpRankCondition() const                  { return m_byBuyPvpRankCondition; }
    void    SetUnitType( BYTE byUnitType )                   { m_byUnitType = byUnitType; }
    BYTE    GetUnitType() const                             { return m_byUnitType; }
    void    SetUnitClass( BYTE byUnitClass )                { m_byUnitClass = byUnitClass; }
    BYTE    GetUnitClass() const                            { return m_byUnitClass; }
    void    SetEqipPosition( BYTE byEqipPosition )          { m_byEqipPosition = byEqipPosition; }
    BYTE    GetEqipPosition() const                         { return m_byEqipPosition; }
#else   X2OPTIMIZE_SCRIPT_PREPROCESSING_TOOL
    void    SetSummonNpcID( CX2UnitManager::NPC_UNIT_ID eNPCID )    { m_usSummonNpcID = (USHORT) eNPCID; }
    CX2UnitManager::NPC_UNIT_ID GetSummonNpcID() const      { return (CX2UnitManager::NPC_UNIT_ID) m_usSummonNpcID; }
    void    SetBuyPvpRankCondition( CX2PVPEmblem::PVP_RANK eRank )  { m_byBuyPvpRankCondition = (BYTE) eRank; }
    CX2PVPEmblem::PVP_RANK GetBuyPvpRankCondition() const   { return (CX2PVPEmblem::PVP_RANK) m_byBuyPvpRankCondition; }
    void    SetUnitType( CX2Unit::UNIT_TYPE eUnitType )     { m_byUnitType = (BYTE) eUnitType; }
    CX2Unit::UNIT_TYPE GetUnitType() const                  { return (CX2Unit::UNIT_TYPE) m_byUnitType; }
    void    SetUnitClass( CX2Unit::UNIT_CLASS eUnitClass )  { m_byUnitClass = (BYTE) eUnitClass; }
    CX2Unit::UNIT_CLASS GetUnitClass() const                { return (CX2Unit::UNIT_CLASS) m_byUnitClass; }
    void    SetEqipPosition( CX2Unit::EQIP_POSITION eEqipPosition ) { m_byEqipPosition = (BYTE) eEqipPosition; }
    CX2Unit::EQIP_POSITION GetEqipPosition() const          { return (CX2Unit::EQIP_POSITION) m_byEqipPosition; }
#endif  X2OPTIMIZE_SCRIPT_PREPROCESSING_TOOL

    void    SetUseLevel( int iUseLevel )                    { m_byUseLevel = (BYTE) iUseLevel; }
    int     GetUseLevel() const                             { return (int) m_byUseLevel; }
    void    SetBuffID( int iBuffID )                        { m_byBuffID = (BYTE) iBuffID; }
    int     GetBuffID() const                               { return (int) m_byBuffID; }
    void    SetCoolTime( int iCoolTime )                    { m_byCoolTime = (BYTE) iCoolTime; }
    int     GetCoolTime() const                             { return (int) m_byCoolTime; }
    void    SetMaxSealCount( BYTE byMaxSealCount )          { m_byMaxSealCount = byMaxSealCount; }
    BYTE    GetMaxSealCount() const                         { return m_byMaxSealCount; }
    void    SetMaxAttribEnchantCount( int iCount )          { m_byMaxAttribEnchantCount = (BYTE) iCount; }
    int     GetMaxAttribEnchantCount() const                { return (int) m_byMaxAttribEnchantCount; }
    void    SetAttributeLevel( int iAttrLevel )             { m_byAttributeLevel = (BYTE) iAttrLevel; }
    int     GetAttributeLevel() const                       { return (int) m_byAttributeLevel; }

    int             GetSocketSlotNum() const;
    const wchar_t*  GetNameColor_() const;


    const wchar_t*  GetName() const;
    const wchar_t*  GetFullName_() const { return GetName(); }    //아이템 등급에 따른 칭호 붙은 이름 : 견고한 견습전사 검
    const wchar_t*  GetDescription() const;
    const wchar_t*  GetDescriptionInShop() const;
    const wchar_t*  GetAttachFrameName( unsigned i ) const;
    const wchar_t*  GetModelName( unsigned i ) const;
    const wchar_t*  GetTextureChangeXETName() const;
    const wchar_t*  GetAniXETName() const;
    const wchar_t*  GetAniName() const;
    const wchar_t*  GetShopImage() const;
    const wchar_t*  GetDropViewer() const;
    const wchar_t*  GetDescriptionInSkillNote() const;
    const KItemFormatStatData&  GetStatData() const;
    unsigned        GetNumBuffFactorPtr() const;
#ifndef X2OPTIMIZE_SCRIPT_PREPROCESSING_TOOL
    CX2BuffFactorPtr GetBuffFactorPtr( unsigned i ) const;
#endif  X2OPTIMIZE_SCRIPT_PREPROCESSING_TOOL
    unsigned        GetNumSpecialAbility() const;
    const KItemFormatSpecialAbility&    GetSpecialAbility( unsigned i ) const;
    unsigned        GetNumSocketOption() const;
    int             GetSocketOption( unsigned i ) const;
    void            GetSocketOptions( std::vector<int>& vecOut_ ) const;

//#ifdef ADD_ITEM_TEMPLET_ITEM
	unsigned        GetNumRandomSocketOption() const;
	int             GetRandomSocketOption( unsigned i ) const;
	void            GetRandomSocketOptions( std::vector<int>& vecOut_ ) const;

	const KItemFormatStatRelLVData&  GetStatRelLVData() const;
//#endif // ADD_ITEM_TEMPLET_ITEM


    unsigned        GetNumAttachedParticleData() const;
    void            GetAttachedParticleData( unsigned i, const wchar_t** ppwszOutParticleName = NULL, const wchar_t** ppwszOutBoneName = NULL ) const;
    unsigned        GetNumAttachedMeshData() const;
    void            GetAttachedMeshData( unsigned i, const wchar_t** ppwszOutMeshName = NULL, const wchar_t** ppwszOutBoneName = NULL, bool* pbOutJiggle = NULL ) const;
#ifndef X2OPTIMIZE_SCRIPT_PREPROCESSING_TOOL
    const D3DXVECTOR3&  GetFrameOffset( CX2Unit::UNIT_TYPE eUnitType ) const;
    const D3DXVECTOR3&  GetRenderScale( CX2Unit::UNIT_TYPE eUnitType ) const;
    const D3DXVECTOR3&  GetRenderRotate( CX2Unit::UNIT_TYPE eUnitType ) const;
    const wchar_t*  GetCommonItemModelName( CX2Unit::UNIT_TYPE eUnitType ) const;
	const wchar_t*  GetCommonItemXETName( CX2Unit::UNIT_TYPE eUnitType ) const;
	void            GetSlashTraceColors( D3DXCOLOR& dwOut_, D3DXCOLOR& dwOutTip_, D3DXCOLOR& dwOutHyper_, D3DXCOLOR& dwOutHyperTip_, CX2Unit::UNIT_TYPE eUnitType_ = CX2Unit::UT_NONE ) const;
#else
	void            GetSlashTraceColors( D3DXCOLOR& dwOut_, D3DXCOLOR& dwOutTip_, D3DXCOLOR& dwOutHyper_, D3DXCOLOR& dwOutHyperTip_) const;
#endif  X2OPTIMIZE_SCRIPT_PREPROCESSING_TOOL

    static const BYTE*     GetData() { return ms_pData; }
    static const wchar_t*      GetWideString( DWORD dwOffset_ );

    class LessID 
    {
    public:
        bool operator() ( const KItemFormatTemplet* lhs, const KItemFormatTemplet* rhs ) const
        {
            return lhs->GetItemID() < rhs->GetItemID();
        }
    };

    class LessName
    {
    public:
        bool operator() ( const KItemFormatTemplet* lhs, const KItemFormatTemplet* rhs )
        {
            return  wcscmp( lhs->GetName(), rhs->GetName() ) < 0;
        }
    };

    KItemFormatTemplet()
    {
        KItemFormatTemplet* pThis = this;
        ZeroMemory( pThis, sizeof(KItemFormatTemplet) );

        SetItemID( 0 );

        SetUseType( UT_NONE );
        //SetUseCondition( UC_ANYONE );
        SetUseCondition( UC_NONE );
        SetUseLevel( 0 );                            // 레벨
        //m_ItemID = -1;                            // 아이템 ID
        SetItemType( IT_NONE );
        SetFashion( false );                    // 패션 아이템
        SetVested( false );                     // 귀속 아이템(양도불능/판매불능)
//#ifdef HIDE_SET_DESCRIPTION
        SetHideSetDesc( false );		// 세트 옵션 안보이게
//#endif HIDE_SET_DESCRIPTION
//#ifdef SERV_PVP_EQUIPMENT
        SetPvpItem( false );			// 인도네시아 PVP 전용 아이템
//#endif SERV_PVP_EQUIPMENT
        SetCanEnchant( false );                 // 강화가능 아이템인가
        SetCanUseInventory( false );            // 인벤토리에서 사용가능한 아이템인가
		//{{ 허상형 : [2009/7/29] //	변수 추가
        SetNoEquip( false );                    // 착용할수 없는 장비인가(교환용 장비)	
		//}} 허상형 : [2009/7/29] //	변수 추가
		//{{ 2010. 01. 05  최육사	PC방 프리미엄
        SetIsPcBang( false );                   // PC방 전용 아이템
		//}}
		//{{ 2011. 10. 14	최육사	배틀필드 아이템 레벨 시스템
//#ifdef  SERV_BATTLE_FIELD_ITEM_LEVEL
        SetItemLevel( 0 );                       // 아이템 레벨 정보
//#endif  SERV_BATTLE_FIELD_ITEM_LEVEL
		//}}
        SetItemGrade( IG_NONE );
        SetCanHyperMode( false );               //하이퍼 모드 가능 아이템인가
        SetPeriodType( PT_INFINITY );           //아이템 존재기간 
        SetEndurance( -1 );                      //초기내구도
        SetEnduranceDamageMinMax( 0, 0 );
        //m_EnduranceDamage		= CMinMax<int>(0,0);		//1스테이지플레이당내구도 감소량
		//{{ 2007. 11. 16  최육사  
        SetRepairED( 0.0f );                     // 내구도 1 수리당 ED
        SetRepairVP( 0 );                     // 내구도 1 수리당 VP
        //}} 
        SetQuantity( 0 );                        // 1묶음수량
        SetShopPriceType( SPT_NONE );               // 가격타입
        SetPrice( 0 );                           // 가격
        SetPricePvPPoint( 0 );                   // 구매대전포인트

        SetCoolTime( 0 );
        SetSetID( 0 );

//#ifdef ITEM_SLASH_TRACE_COLOR_TEST
//				m_coSlashTrace				= 0xcc5555ff;
//				m_coSlashTraceHyper 		= 0xccff3333;
//				m_coSlashTraceTip			= 0xff5555ff;
//				m_coSlashTraceTipHyper		= 0xffff3333;
//#endif ITEM_SLASH_TRACE_COLOR_TEST

		//{{ kimhc // 2009-08-19 // 아이템 봉인
//#ifdef	SEAL_ITEM
		SetMaxSealCount( 0 );	// 기본 0, 0이면 봉인 할 수 없음				
//#endif	SEAL_ITEM
		//}} kimhc // 2009-08-19 // 아이템 봉인
        SetMaxAttribEnchantCount( 0 );
//#ifdef PVP_SEASON2_SOCKET
		SetAttributeLevel( 0 );
//#endif
		//{{ kimhc // 2009-10-19 // 최대 MP 증가 값 추가
		SetAddMaxMP( 0.0f );
		//}} kimhc // 2009-10-19 // 최대 MP 증가 값 추가

//#ifdef DUNGEON_ITEM
//				m_SpecialItemType = SIT_NONE;
//				m_wstrDamageEffectName = L"";
//#endif

//				//{{ 2010. 03. 22  최육사	기술의 노트
//#ifdef SERV_SKILL_NOTE                
//				m_DescriptionInSkillNote = L"";
//#endif SERV_SKILL_NOTE
//				//}}

//#ifdef  BUFF_TEMPLET_SYSTEM
        SetBuffID( 0 );
//#endif  BUFF_TEMPLET_SYSTEM

#ifdef  X2OPTIMIZE_SCRIPT_PREPROCESSING_TOOL
        SetUnitType( 0 );        // 유닛
        SetUnitClass( 0 );    // 클래스
        SetEqipPosition( 0 );	//장착 위치
        SetBuyPvpRankCondition( 0 );
//#ifdef  ADD_TRAININGGAME_NPC
        SetSummonNpcID( 0 );
//#endif  ADD_TRAININGGAME_NPC
#else   X2OPTIMIZE_SCRIPT_PREPROCESSING_TOOL
        BOOST_STATIC_ASSERT( CX2Unit::UT_NONE == 0 );
        BOOST_STATIC_ASSERT( CX2Unit::UC_NONE == 0 );
        BOOST_STATIC_ASSERT( CX2Unit::EP_NONE == 0 );
        BOOST_STATIC_ASSERT( CX2PVPEmblem::PVPRANK_NONE == 0 );
        BOOST_STATIC_ASSERT( CX2UnitManager::NUI_NONE == 0 );
        SetUnitType( CX2Unit::UT_NONE );        // 유닛
        SetUnitClass( CX2Unit::UC_NONE );    // 클래스
        SetEqipPosition( CX2Unit::EP_NONE );	//장착 위치
        SetBuyPvpRankCondition( CX2PVPEmblem::PVPRANK_NONE );
//#ifdef  ADD_TRAININGGAME_NPC
        SetSummonNpcID( CX2UnitManager::NUI_NONE );
//#endif  ADD_TRAININGGAME_NPC
#endif  X2OPTIMIZE_SCRIPT_PREPROCESSING_TOOL

    }

private:

    void    _SetBit( DWORD dwBitPos, bool bValue )
    {
        if ( bValue )
            m_dwFlags |= 1<<dwBitPos;
        else
            m_dwFlags &= ~(1<<dwBitPos);
    }
    bool    _GetBit( DWORD dwBitPos ) const { return ( m_dwFlags & (1<<dwBitPos) ) ? true : false; }
    void    _SetValue( DWORD dwBitPos, DWORD dwNumBits, DWORD dwValue )
    {
        m_dwFlags = ( m_dwFlags & ~( ((1<<dwNumBits)-1)<<dwBitPos ) ) | ( ( dwValue & ( (1<<dwNumBits) - 1) ) << dwBitPos );
    }
    DWORD   _GetValue( DWORD dwBitPos, DWORD dwNumBits ) const
    {
        return  ( m_dwFlags >> dwBitPos ) & ( (1<<dwNumBits) - 1 );
    }
//#ifdef ADD_FLAGS_ITEM_PREPROCESSING
	void    _SetBit2( DWORD dwBitPos, bool bValue )
	{
		if ( bValue )
			m_dwFlags2 |= 1<<dwBitPos;
		else
			m_dwFlags2 &= ~(1<<dwBitPos);
	}
	bool    _GetBit2( DWORD dwBitPos ) const { return ( m_dwFlags2 & (1<<dwBitPos) ) ? true : false; }
	void    _SetValue2( DWORD dwBitPos, DWORD dwNumBits, DWORD dwValue )
	{
		m_dwFlags2 = ( m_dwFlags2 & ~( ((1<<dwNumBits)-1)<<dwBitPos ) ) | ( ( dwValue & ( (1<<dwNumBits) - 1) ) << dwBitPos );
	}
	DWORD   _GetValue2( DWORD dwBitPos, DWORD dwNumBits ) const
	{
		return  ( m_dwFlags2 >> dwBitPos ) & ( (1<<dwNumBits) - 1 );
	}
//#endif ADD_FLAGS_ITEM_PREPROCESSING


    static const D3DXVECTOR3&  _GetD3DXVECTOR( DWORD dwOffset_, bool bDefaultOne = false );
    static void                _ResetData();
    static void                _SetData( const BYTE* pData )                        { ms_pData = pData; }

private:

    DWORD       m_dwItemID;
    DWORD       m_dwFlags;
//#ifdef ADD_FLAGS_ITEM_PREPROCESSING
    DWORD       m_dwFlags2;
//#endif ADD_FLAGS_ITEM_PREPROCESSING
    // USE_TYPE m_UseType(4bit)
    // USE_CONDITION m_UseCondition(4bit)
    // ITEM_TYPE m_ItemType(4bit)
    // ITEM_GRADE m_ItemGrade(4bit)
    // PERIOD_TYPE m_PeriodType(4bit)
    // SHOP_PRICE_TYPE    m_PriceType(4bit)
    // bool m_bFashion(1bit)
    // bool m_bVested(1bit)
    // bool m_bCanEnchant(1bit)
    // bool m_bCanUseInventory(1bit)
    // bool m_bNoEquip(1bit)
    // bool m_bIsPcBang(1bit)
    // bool m_bCanHyperMode(1bit)

    int         m_iItemLevel;       // int  m_iItemLevel;
    int         m_iQuantity;        // int   m_Quantity;
    float       m_fRepairED;        // float m_RepairED;
    int         m_iRepairVP;        // int      m_RepairVP;
    int         m_iPrice;           // int m_Price;
    int         m_iPricePvPPoint;   // int m_PricePvPPoint;
    int         m_iSetID;           // int m_SetID;
    float       m_fAddMaxMP;

// 어느 본에 붙는가
    DWORD        m_dwOffset_Name; // wstring m_Name
    DWORD        m_dwOffset_Description; // wstring m_Description;
    DWORD        m_dwOffset_DescriptionInShop; // wstring m_DescriptionInShop;
    DWORD        m_adwOffset_AttachFrameName[MAX_MODEL_COUNT_A_ITEM]; //<- wstring?
    //wstring                        m_AttachFrameName[ MAX_MODEL_COUNT_A_ITEM ];


    DWORD        m_adwOffset_ModelName[MAX_MODEL_COUNT_A_ITEM];
    // std::wstring                m_ModelName[ MAX_MODEL_COUNT_A_ITEM ];
    DWORD        m_dwOffset_TextureChangeXETName;
    // std::wstring                m_TextureChangeXETName;        //하이퍼 모드는 _Hyper 를 붙인다
    DWORD        m_dwOffset_AniXETName;
    // std::wstring                m_AniXETName;
    DWORD       m_dwOffset_AniName;
    //std::wstring                m_AniName;                    //하이퍼 모드는 _Hyper 를 붙인다
    DWORD        m_dwOffset_ShopImage;
    // std::wstring                m_ShopImage;
    DWORD        m_dwOffset_DropViewer;
    // std::wstring                m_DropViewer;
    DWORD       m_dwOffset_DescriptionInSkillNote;
    // std::wstring        m_DescriptionInSkillNote;
    DWORD       m_dwOffset_Stat;               
    // CX2Stat::Stat                m_Stat;

    DWORD       m_dwOffset_BuffFactorIndices;
    DWORD       m_dwOffset_SpecialAbilityList;
    DWORD       m_dwOffset_SocketOptions;
    DWORD       m_dwOffset_AttachedParticleData;
    DWORD       m_dwOffset_AttachedMeshData;
    DWORD       m_dwOffset_Extra;
//#ifdef ADD_ITEM_TEMPLET_ITEM
	DWORD		m_dwOffset_RandomSocketOptions;
	DWORD       m_dwOffset_StatRelationLevel;
//#endif // ADD_ITEM_TEMPLET_ITEM

    SHORT       m_sEndurance; // int m_Endurance;
    SHORT       m_sEnduranceDamageMin;  
    SHORT       m_sEnduranceDamageMax;
    USHORT      m_usSummonNpcID; // CX2UnitManager::NPC_UNIT_ID    m_iSummonNpcID;

    BYTE        m_byBuyPvpRankCondition; // CX2PVPEmblem::PVP_RANK  m_BuyPvpRankCondition(4bit)
    BYTE        m_byUseLevel; // int m_UseLevel;
    BYTE        m_byUnitType; // CX2Unit::UNIT_TYPE    m_UnitType
    BYTE        m_byUnitClass; // CX2Unit::UNIT_CLASS m_UnitClass;
    BYTE        m_byBuffID; // int m_BuffID;
    BYTE        m_byEqipPosition; // CX2Unit::EQIP_POSITION        m_EqipPosition
    BYTE        m_byCoolTime; // int m_CoolTime
    BYTE        m_byMaxSealCount; // BYTE m_ucMaxSealCount;
    BYTE        m_byMaxAttribEnchantCount; // int                    m_iMaxAttribEnchantCount
    BYTE        m_byAttributeLevel; // int                            m_iAttributeLevel

    // Extra
    BYTE        m_byNumFrameOffsets;
    BYTE        m_byNumRenderScales;  // D3DXVECTOR3 m_RanderScal[CX2Unit::UT_END];
    BYTE        m_byNumRenderRotates; // D3DXVECTOR3 m_RanderRotate[CX2Unit::UT_END];
    BYTE        m_byNumCommonItemModelNames; 
    BYTE        m_byNumCommonItemXETNames;
    BYTE        m_byNumSlashTraces;

    // 항상 4byte align 되도록 BYTE padding 한다.

    static const KItemFormatStatData
                                ms_kDummyStatData;
    static const KItemFormatSpecialAbility
                                ms_kDummySpecialAbility;
    static const D3DXVECTOR3    ms_vZero;
    static const D3DXVECTOR3    ms_vOne;
    static KItemFormatHeader    ms_kDummyItemFormatHeader;
    static const BYTE*          ms_pData;

//#ifdef ADD_ITEM_TEMPLET_ITEM
	static const KItemFormatStatRelLVData  ms_kDummyStatRelLVData;
//#endif // ADD_ITEM_TEMPLET_ITEM

};

#pragma pack( pop )

BOOST_STATIC_ASSERT( sizeof(KItemFormatTemplet)/4*4 == sizeof(KItemFormatTemplet) );
BOOST_STATIC_ASSERT( IT_NUM <= (1<<KItemFormatTemplet::FLAG_NUM_ITEM_TYPE) );
BOOST_STATIC_ASSERT( IG_NUM <= (1<<KItemFormatTemplet::FLAG_NUM_ITEM_GRADE) );
BOOST_STATIC_ASSERT( PT_NUM <= (1<<KItemFormatTemplet::FLAG_NUM_PERIOD_TYPE) );
BOOST_STATIC_ASSERT( SPT_NUM <= (1<<KItemFormatTemplet::FLAG_NUM_SHOP_PRICE_TYPE) );
BOOST_STATIC_ASSERT( UC_NUM <= (1<<KItemFormatTemplet::FLAG_NUM_USE_CONDITION) );
BOOST_STATIC_ASSERT( UT_NUM <= (1<<KItemFormatTemplet::FLAG_NUM_USE_TYPE) );
#ifndef  X2OPTIMIZE_SCRIPT_PREPROCESSING_TOOL
BOOST_STATIC_ASSERT( CX2UnitManager::NUI_MAX_LIMIT <= (1<<(8*sizeof(WORD))) );
BOOST_STATIC_ASSERT( CX2PVPEmblem::PVPRANK_MAX <= (1<<(8*sizeof(BYTE))) );
BOOST_STATIC_ASSERT( CX2Unit::UT_END <= (1<<(8*sizeof(BYTE))) );
BOOST_STATIC_ASSERT( CX2Unit::UC_END <= (1<<(8*sizeof(BYTE))) );
BOOST_STATIC_ASSERT( CX2Unit::EP_END <= (1<<(8*sizeof(BYTE))) );
#endif  X2OPTIMIZE_SCRIPT_PREPROCESSING_TOOL

typedef KItemFormatTemplet  ItemTemplet;
typedef KItemFormatSpecialAbility   SpecialAbility;


#endif  //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING