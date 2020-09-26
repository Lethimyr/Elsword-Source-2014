#pragma once

#define MAX_SPECIAL_ABILITY_NUM	 3
#define MAX_ENCHANT_CARD_NUM	10



class CX2Stat
{
public:
	enum ATTRIBUTE
	{
		ATT_NONE,
		ATT_PHYSIC,
		ATT_FIRE,
		ATT_ICE,
		ATT_LIGHTNING,
		ATT_EARTH,
		ATT_LIGHT,
		ATT_DARK,
		ATT_UNIVERSAL,
	};

public:
	struct Stat
	{
		float	m_fBaseHP;

		//공격
		float	m_fAtkPhysic;
		float	m_fAtkMagic;

		float	m_fAtkFire;
		float	m_fAtkIce;
		float	m_fAtkLightning;
		float	m_fAtkEarth;
		float	m_fAtkLight;
		float	m_fAtkDark;
		float	m_fAtkUniversal;

		//방어
		float	m_fDefPhysic;
		float	m_fDefMagic;

		float	m_fDefFire;
		float	m_fDefIce;
		float	m_fDefLightning;
		float	m_fDefEarth;
		float	m_fDefLight;
		float	m_fDefDark;
		float	m_fDefUniversal;

		Stat()
		{
			Init();
		}
		void Init()
		{
			m_fBaseHP			= 0.0f;

			m_fAtkPhysic		= 0.0f;
			m_fAtkMagic			= 0.0f;

			m_fAtkFire			= 0.0f;
			m_fAtkIce			= 0.0f;
			m_fAtkLightning		= 0.0f;
			m_fAtkEarth			= 0.0f;
			m_fAtkLight			= 0.0f;
			m_fAtkDark			= 0.0f;
			m_fAtkUniversal		= 0.0f;

			m_fDefPhysic		= 0.0f;
			m_fDefMagic			= 0.0f;

			m_fDefFire			= 0.0f;
			m_fDefIce			= 0.0f;
			m_fDefLightning		= 0.0f;
			m_fDefEarth			= 0.0f;
			m_fDefLight			= 0.0f;
			m_fDefDark			= 0.0f;
			m_fDefUniversal		= 0.0f;
		}
		void AddStat( const Stat& baseStat )
		{
			m_fBaseHP			+= baseStat.m_fBaseHP;

			m_fAtkPhysic		+= baseStat.m_fAtkPhysic;
			m_fAtkMagic			+= baseStat.m_fAtkMagic;

			m_fAtkFire			+= baseStat.m_fAtkFire;
			m_fAtkIce			+= baseStat.m_fAtkIce;
			m_fAtkLightning		+= baseStat.m_fAtkLightning;
			m_fAtkEarth			+= baseStat.m_fAtkEarth;
			m_fAtkLight			+= baseStat.m_fAtkLight;
			m_fAtkDark			+= baseStat.m_fAtkDark;
			m_fAtkUniversal		+= baseStat.m_fAtkUniversal;

			m_fDefPhysic		+= baseStat.m_fDefPhysic;
			m_fDefMagic			+= baseStat.m_fDefMagic;

			m_fDefFire			+= baseStat.m_fDefFire;
			m_fDefIce			+= baseStat.m_fDefIce;
			m_fDefLightning		+= baseStat.m_fDefLightning;
			m_fDefEarth			+= baseStat.m_fDefEarth;
			m_fDefLight			+= baseStat.m_fDefLight;
			m_fDefDark			+= baseStat.m_fDefDark;
			m_fDefUniversal		+= baseStat.m_fDefUniversal;
		}

		void Verify();
	};

public:
	CX2Stat(void) {};
	~CX2Stat(void) {};

	void	InitStat(){ m_Stat.Init(); }
	Stat*	GetStat(){ return &m_Stat; }
	void	AddStat( const Stat& baseStat ){ m_Stat.AddStat( baseStat ); }

private:
	Stat		m_Stat;
};


class CX2Item;
class CX2Inventory;
class CX2Eqip;
class CX2Unit
{
public:
	enum UNIT_TYPE
	{
		UT_NONE	= 0,
		UT_ELSWORD,
		UT_ARME,
		UT_LIRE,
		UT_RAVEN,
		UT_END,			//  엘리시스가 추가되면 END를 엘리시스 아래로..
		UT_ELISIS,
	};

	enum UNIT_CLASS
	{
		//초기직업
		UC_NONE = 0,
		UC_ELSWORD_SWORDMAN,
		UC_ARME_VIOLET_MAGE,
		UC_LIRE_ELVEN_RANGER,
		UC_RAVEN_FIGHTER,
		UC_ELISIS_KNIGHT,

		//일차전직
		UC_ELSWORD_KNIGHT = 10,
		UC_ELSWORD_MAGIC_KNIGHT,
		UC_LIRE_COMBAT_RANGER,
		UC_LIRE_SNIPING_RANGER,
		UC_ARME_HIGH_MAGICIAN,
		UC_ARME_DARK_MAGICIAN,


		//임시
		UC_ELSWORD_SWORD_EXPERT,
		UC_ELSWORD_SQUERD_LEADER,

		UC_ELISIS_ARCANA_KNIGHT,
		UC_ELISIS_CENTURION_KNIGHT,

		UC_ELISIS_KNIGHT_MASTER,
		UC_ELISIS_LEGION_COMMENDER,

		UC_ARME_ARCH_MAGE,
		UC_ARME_ARCHIMIST,
		UC_ARME_ARCANE_MASTER,
	};

	enum WEAPON_TYPE
	{
		WT_NONE = 0,
		WT_GREATE_SWORD,
		WT_LONG_BOW,
		WT_WAND,
	};

	enum EQIP_POSITION
	{
		EP_NONE		= 0,
		EP_QUICK_SLOT,		//퀵슬롯 아이템

		//무기
		EP_WEAPON_HAND,			//무기
		EP_WEAPON_TEMP1,		//임시1
		EP_WEAPON_TEMP2,		//임시2
		EP_WEAPON_TEMP3,		//임시3

		//방어구
		EP_DEFENCE_HAIR,		//헤어스타일
		EP_DEFENCE_FACE,		//얼굴
		EP_DEFENCE_BODY,		//상의
		EP_DEFENCE_LEG,			//하의
		EP_DEFENCE_HAND,		//장갑
		EP_DEFENCE_FOOT,		//신발
		EP_DEFENCE_TEMP1,		//임시1
		EP_DEFENCE_TEMP2,		//임시2
		EP_DEFENCE_TEMP3,		//임시3

		//액세서리
		EP_AC_TITLE,			//칭호
		EP_AC_HAIR,				//헤어
		EP_AC_FACE1,				//눈
		EP_AC_FACE2,			//입
		EP_AC_FACE3,				//귀
		EP_AC_BODY,				//상의
		EP_AC_ARM,				//팔
		EP_AC_LEG,				//다리
		EP_AC_RING,				//반지
		EP_AC_NECKLESS,			//목걸이
		EP_AC_TEMP1,			//임시1
		EP_AC_TEMP2,			//임시2
		EP_AC_TEMP3,			//임시3
		EP_AC_TEMP4,			//임시4
		EP_AC_TEMP5,			//임시5

		//필살기
		EP_SKILL_1,				//1단계 필살기
		EP_SKILL_2,				//2단계 필살기
		EP_SKILL_3,				//3단계 필살기
		EP_SKILL_TEMP1,			//임시1
		EP_SKILL_TEMP2,			//임시2
		EP_SKILL_TEMP3,			//임시3

		EP_END,
	};


	enum EQIP_SLOT_ID
	{
		ESI_NONE		= 0,
		ESI_QUICK_SLOT,		//퀵슬롯 아이템

		ESI_WEAPON_HAND,			//무기

		ESI_DEFENCE_BODY,		//상의
		ESI_DEFENCE_LEG,			//하의
		ESI_DEFENCE_HAND,		//장갑
		ESI_DEFENCE_FOOT,		//신발

		ESI_DEFENCE_HAIR_FASHION,		//헤어스타일

		ESI_DEFENCE_BODY_FASHION,		//상의
		ESI_DEFENCE_LEG_FASHION,			//하의
		ESI_DEFENCE_HAND_FASHION,		//장갑
		ESI_DEFENCE_FOOT_FASHION,		//신발

		ESI_SKILL_1,				//1단계 필살기
		ESI_SKILL_2,				//2단계 필살기
		ESI_SKILL_3,				//3단계 필살기

		ESI_END,
	};

	struct UnitTemplet
	{
		UNIT_TYPE			m_UnitType;
		UNIT_CLASS			m_UnitClass;

		wstring				m_Name;
		wstring				m_Description;

		wstring				m_GameMotionFile;
		wstring				m_UIMotionFile;

		wstring				m_GameAniXETName;
		wstring				m_UIAniXETName;

		wstring				m_BasicAniXETName;
		wstring				m_BasicChangeTexXETName;

		CX2Stat::Stat		m_Stat;

		int					m_BasicWeaponItemID;
		int					m_BasicHairItemID;
		int					m_BasicFaceItemID;
		int					m_BasicBodyItemID;
		int					m_BasicLegItemID;
		int					m_BasicHandItemID;
		int					m_BasicFootItemID;
	};

	struct UnitData
	{
		UidType					m_UnitUID;
		UidType					m_UserUID;
		UNIT_CLASS				m_UnitClass;

		wstring				    m_IP;
		int						m_Port;

		wstring					m_NickName;

		int		m_GP;
		int		m_VSPoint;
		int		m_Level;
		int		m_EXP;

		int						m_Win;
		int						m_Lose;
		int						m_Seceder;

		int		m_NowBaseLevelEXP;
		int		m_NextBaseLevelEXP;

		int		m_EquipInventorySize;		
		int		m_SkillInventorySize;		
		int		m_MaterialInventorySize;	
		int		m_SpecialInventorySize;	
		int		m_CardInventorySize;		
		int		m_QuickSlotInventorySize;	
		int		m_AvartaInventorySize;	

		int		m_QuickSlotSize;

		CX2Stat::Stat 			m_Stat;
		int		m_RemainStatPoint;

		vector<UidType>			m_NowEqipItemUIDList;
		CX2Inventory*			m_pInventory;

		int		m_nStraightVictories;
		int		m_nMapID;

		//UnitData& operator=( const KUnitInfo& data );

		//UnitData( CX2Unit* pOwnerUnit, const KUnitInfo& data );
		~UnitData();

		void Verify();
	};

public:
	CX2Unit() {} ;
	virtual ~CX2Unit(void) {};


};


class CX2Item
{
	public:
		enum ITEM_TYPE
		{
			IT_NONE = 0,		//에러
			IT_WEAPON,			//무기
			IT_DEFENCE,			//방어구
			IT_ACCESSORY,		//액세서리
			IT_SKILL,			//스킬
			IT_QICK_SLOT,		//퀵슬롯 아이템(소비성)
			IT_MATERIAL,		//재료
			IT_SPECIAL,			//특수
			IT_CARD,			//카드
			IT_OUTLAY,			//순간소비성
			IT_ETC,				//기타]
			IT_END,
		};

		enum EQIP_TYPE
		{
			ET_NONE = 0,		//장비가 아님
			ET_SKIN,			//스킨 애니메이션
			ET_ATTACH_ANIM,		//어태치 애니메이션
			ET_ATTACH_NORMAL,	//어태치 메시
		};

		enum PERIOD_TYPE
		{
			PT_INFINITY = 0,	//무제한
			PT_ENDURANCE,		//내구도(기간제)
			PT_QUANTITY,		//수량성
		};

		enum SHOP_PRICE_TYPE
		{
			SPT_NONE = 0,		//팔기 불능
			SPT_CASH,			//캐쉬
			SPT_GP,				//GP
		};

		enum EQIP_CONDITION
		{
			EC_NONE = 0,		//장비가 아님
			EC_EVERYONE,		//모두다
			EC_ONE_UNIT,		//이 유닛만
			EC_ONE_CLASS,		//이 클래스만
		};

		enum SPECIAL_ABILITY_TYPE
		{
			SAT_NONE = 0,		//없음
			SAT_HP_UP,			//HP증가
			SAT_MP_UP,			//MP증가
			SAT_REMOVE_CURSE,	//저주제거
			SAT_REMOVE_SLOW,	//슬로우제거
			SAT_REMOVE_FIRE,	//화염제거
			SAT_REMOVE_POISON,	//독제거
			SAT_HYPER_MODE,			//각성
			SAT_SOUL_GAGE_UP,		//각성게이지 상승
			SAT_END,
		};

		struct SpecialAbility
		{
			SPECIAL_ABILITY_TYPE		m_Type;
			int							m_CoolTime;
			int							m_Value1;
			int							m_Value2;
			int							m_Value3;

			SpecialAbility()
			{
				m_Type			= SAT_NONE;
				m_CoolTime		= 0;
				m_Value1		= 0;
				m_Value2		= 0;
				m_Value3		= 0;
			}
		};

		struct ItemTemplet
		{
			int							m_ItemID;					//아이템 ID
			std::wstring				m_Name;						//아이템 이름
			std::wstring				m_Description;				//아이템 설명

			std::wstring				m_ModelName;				//하이퍼 모드는 _Hyper 를 붙인다
			std::wstring				m_TextureChangeXETName;		//하이퍼 모드는 _Hyper 를 붙인다
			std::wstring				m_AniXETName;				//하이퍼 모드는 _Hyper 를 붙인다
			std::wstring				m_AniName;					//하이퍼 모드는 _Hyper 를 붙인다
			std::wstring				m_ShopImage;
			std::wstring				m_DropViewer;				//드롭시 사용할 파티클 시스템 이름

			ITEM_TYPE					m_ItemType;
			bool						m_bFashion;					//패션 아이템
			bool						m_bVested;					//귀속 아이템(양도불능/판매불능)
			bool						m_bRare;					//레어아이템인가?
			bool						m_bCanEnchant;				//강화가능 아이템인가

			EQIP_TYPE					m_EqipType;
			std::wstring				m_AttachFrameName;			//어느 본에 붙는가
			bool						m_bCanHyperMode;			//하이퍼 모드 가능 아이템인가
			
			PERIOD_TYPE					m_PeriodType;				//아이템 존재기간
			int							m_Endurance;				//초기내구도
			int							m_Quantity;					//1묶음수량
			
			bool						m_bSale;					//판매하는가?
			SHOP_PRICE_TYPE				m_PriceType;				//가격타입
			int							m_Price;					//가격
			int							m_PricePvPPoint;			//구매대전포인트

			bool						m_bUseInUI;					//ui 창에서 사용 버튼이 붙는가?

			EQIP_CONDITION				m_EqipConditon;				//장착 조건
			CX2Unit::UNIT_TYPE			m_UnitType;					//장착 유닛
			CX2Unit::UNIT_CLASS			m_UnitClass;				//장착 클래스
			int							m_EqipLevel;				//장착 레벨

			CX2Unit::EQIP_POSITION		m_EqipPosition;				//장착 위치
			CX2Stat::Stat				m_Stat;						//아이템 스탯

			std::vector<SpecialAbility>	m_SpecialAbilityList;

			ItemTemplet()
			{
				m_ItemID			= -1;				//아이템 ID

				m_ItemType			= IT_NONE;
				m_bFashion			= false;			//패션 아이템
				m_bVested			= false;			//귀속 아이템(양도불능/판매불능)
				m_bRare				= false;			//레어아이템인가?
				m_bCanEnchant		= false;			//강화가능 아이템인가

				m_EqipType			= ET_NONE;
				m_bCanHyperMode		= false;			//하이퍼 모드 가능 아이템인가

				m_PeriodType		= PT_INFINITY;		//아이템 존재기간
				m_Endurance			= 0;				//초기내구도
				m_Quantity			= 0;				//1묶음수량

				m_bSale				= false;			//판매하는가?
				m_PriceType			= SPT_NONE;			//가격타입
				m_Price				= 0;				//가격
				m_PricePvPPoint		= 0;				//구매대전포인트

				m_bUseInUI			= false;			//ui 창에서 사용 버튼이 붙는가?

				m_EqipConditon		= EC_NONE;			//장착 조건
				m_UnitType			= CX2Unit::UT_NONE;	//장착 유닛
				m_UnitClass			= CX2Unit::UC_NONE;	//장착 클래스
				m_EqipLevel			= 0;				//장착 레벨

				m_EqipPosition		= CX2Unit::EP_NONE;	//장착 위치
			}
		};

		struct ItemData
		{
			UidType				m_ItemUID;				//UID
			int					m_ItemID;

            PERIOD_TYPE         m_PeriodType;
			int					m_Endurance;			//내구도
			int					m_Quantity;				//수량

			int					m_EnchantAttribute;
			int					m_EnchantLevel;

			ItemData()
			{
				m_ItemUID				= 0;				//UID
				m_ItemID				= 0;

                m_PeriodType            = PT_INFINITY;
				m_Endurance				= 0;				//내구도
				m_Quantity				= 0;				//수량

				m_EnchantAttribute		= 0;
				m_EnchantLevel			= 0;
			}
			//ItemData& operator=( const KItemInfo& data )
			//{
			//	m_ItemUID				= data.m_iItemUID;					//UID
			//	m_ItemID				= data.m_iItemID;

   //             m_PeriodType            = (CX2Item::PERIOD_TYPE)data.m_iUsageType;
			//	m_Endurance				= data.m_iEndurance;				//내구도
			//	m_Quantity				= data.m_iQuantity;					//수량

			//	m_EnchantAttribute		= data.m_iEType;
			//	m_EnchantLevel			= data.m_iELevel;

			//	return *this;
			//}

			//void MakePacket( KItemInfo& itemInfo )
			//{
			//	itemInfo.m_iItemUID				= m_ItemUID;				//UID
			//	itemInfo.m_iItemID				= m_ItemID;

   //             itemInfo.m_iUsageType           = m_PeriodType;
			//	itemInfo.m_iEndurance			= m_Endurance;			//내구도
			//	itemInfo.m_iQuantity			= m_Quantity;			//수량

			//	itemInfo.m_iEType				= m_EnchantAttribute;
			//	itemInfo.m_iELevel				= m_EnchantLevel;
			//}
		};

	public:
		CX2Item( ItemData* pItemData );
		~CX2Item(void);

		ItemData*		GetItemData()			{ return m_pItemData; }
		ItemTemplet*	GetItemTemplet()		{ return m_pItemTemplet; }

		UidType			GetUID()				{ return m_pItemData->m_ItemUID; }
		void			SetEqip( bool bEqip )	{ m_bEqip = bEqip; }
		bool			GetEqip()				{ return m_bEqip; }

	private:
		ItemData*		m_pItemData;
		ItemTemplet*	m_pItemTemplet;

		bool			m_bEqip;
};
