#pragma once

#include "RefreshSingleton.h"
#include "Lottery.h"
#include "XSLItem.h"

class CXSLAttribEnchantItem
{
	DeclareRefreshSingleton( CXSLAttribEnchantItem );
	DeclareLuaScriptParser;
	DeclDumpToLogFile;
	DeclDump;
	DeclToStringW;

public:
	enum ENCHANT_TYPE
	{
		ET_NONE = 0,
		ET_BLAZE,
		ET_WATER,
		ET_NATURE,
		ET_WIND,
		ET_LIGHT,
		ET_DARK,

		ET_RANDOM,
	};

	enum ENCHANT_SLOT_ID
	{
        ESI_SLOT_1 = 0,
		ESI_SLOT_2,
		ESI_SLOT_3,

		ESI_SLOT_MAX,
	};

	// 하드코딩임돠..
	enum ATTRIB_ITEM_ID
	{
		ATI_NONE			= 0,

		ATI_UNKNOWN			= 130047, // 무속성
		ATI_RED				= 130048, // 불
		ATI_BLUE			= 130049, // 물
		ATI_GREEN			= 130051, // 자연
		ATI_WIND			= 130050, // 바람
		ATI_LIGHT			= 130053, // 빛
		ATI_DARK			= 130052, // 암흑

		ATI_IDENTIFY_STONE	= 130054, // 감정석
		ATI_UNKNOWN_STONE	= 130055, // 복구석

//#ifdef SERV_FINALITY_SKILL_SYSTEM	// 적용날짜: 2013-08-01
		ATI_ESSENCE			= 130208, // 엘의 정수
//#endif // SERV_FINALITY_SKILL_SYSTEM
	};

	enum ATTRIB_ENCHANT_TYPE
	{
		AET_NONE = 0,

		AET_SINGLE_WEAPON,
		AET_DUAL_WEAPON,
		AET_ARMOR,

		//{{ 2009. 11. 9  최육사	트리플속성
		AET_TRIPLE_WEAPON,
		AET_DUAL_ARMOR,
		//}}
	};

	//{{ 2009. 11. 9  최육사	트리플속성
	enum ATTRIB_COUNT_TYPE
	{
		ACT_NONE = 0,
		ACT_SINGLE,
		ACT_DUAL,
		ACT_TRIPLE,
	};
	//}}



	struct KIdentifyInfo
	{
		int			m_iRequireItemID;
		KLottery	m_kResult;
	};

public:
	CXSLAttribEnchantItem(void);
	virtual ~CXSLAttribEnchantItem(void);

	// For Lua	
	bool	AddEnchantRequire_LUA( int iAttribEnchantType, int iEquipLv );
	bool	AddRandomAttribSingle_LUA( char cEncahntType, float fRate );
	bool	AddRandomAttribDual_LUA( char cSingleEncahntType, char cDualEncahntType, float fRate );
	bool	AddRandomAttribTriple_LUA( char cSingleEncahntType, char cDualEncahntType, char cTripleEnchantType, float fRate );
	bool	AddIdentifyInfo_LUA( int iSourceItemID, int iRequireItemID, int iResultItemID, float fRate );
	
	//{{ 2011. 01. 04  김민성	속성 부적
#ifdef SERV_ATTRIBUTE_CHARM
	bool	AddAttribAttachInfo_LUA( int iItemID, char cSlot0, char cSlot1, char cSlot2 );
#endif SERV_ATTRIBUTE_CHARM
	//}}

	// 속성석 감정 정보
	const KIdentifyInfo* GetIdentifyInfo( int iItemID );

	// 속성강화 요구사항 함수
	static ATTRIB_COUNT_TYPE GetAttribCountType( IN const KItemAttributeEnchantInfo& kInfo );
	static char GetAttribEnchantType( IN ATTRIB_COUNT_TYPE eCountType, IN bool bWeapon );
	bool	GetRequireItemCount_Insert( IN ATTRIB_COUNT_TYPE eCountType, IN bool bWeapon, IN int iEqipLevel, IN int iItemGrade, OUT int& iRequireCount ) const;
	bool	GetRequireED_Insert( IN ATTRIB_COUNT_TYPE eCountType, IN bool bWeapon, IN int iEqipLevel, IN int iItemGrade, OUT int& iRequireED ) const;
	bool	GetRequireED_Remove( IN ATTRIB_COUNT_TYPE eCountType, IN bool bWeapon, IN int iEqipLevel, IN int iItemGrade, OUT int& iRequireED ) const;
	static int GetRequireEDFactor( int iItemGrade );

	// 속성강화 검증 함수
	bool	IsAttribItemID( int iItemID );
	bool	VerifyAttribEnchantType( IN char cAttribEnchantType );
	char	GetRandomAttribResult( IN ATTRIB_COUNT_TYPE eCurrentCountType, IN char cFirstAttribEnchant = ET_NONE, IN char cSecondAttribEnchant = ET_NONE ) const;
	bool	IsPossibleAttribEnchant( IN OUT char& cAttribEnchantType, IN char cAttribEnchantSlotNo, IN const KItemAttributeEnchantInfo& kInfo ) const;


	static int GetRequireItemID( IN char cAttribEnchantType );
	bool	IsPossibleToPush( IN char cExistingType, IN char cTypeToAdd ) const;

	//void	SetWeaponEnchantData( CX2GUUser* pGUUser );
	//void	SetDefenseEnchantData( CX2GUUser* pGUUser );

	//void	ProcessEnchant( CX2GameUnit* pDefender, CX2DamageManager::DamageData* pDamageData, float fRandValue );

	//{{ 2011. 01. 04	최육사	속성 부적
#ifdef SERV_ATTRIBUTE_CHARM
	bool	GetAttribAttachInfo( IN int iAttachItemID, OUT KItemAttributeEnchantInfo& kInfo );
#endif SERV_ATTRIBUTE_CHARM
	//}}

#ifdef SERV_FINALITY_SKILL_SYSTEM	// 적용날짜: 2013-08-01
	static	bool	IsExtractItem( const int iItemID );
#endif // SERV_FINALITY_SKILL_SYSTEM

private:
	typedef std::map< std::pair< int, int >, std::map< int, int > > MapEnchantRequire;
	typedef std::pair< char, char >	KTripleEnchantKey;
		
	KLottery								m_kLotRandomAttribSingle;	// 싱글 속성 인챈트
	std::map< char, KLottery >				m_mapRandomAttribDual;		// 듀얼 속성 인챈트
	std::map< KTripleEnchantKey, KLottery > m_mapRandomAttribTriple;	// 트리플 속성 인챈트

	MapEnchantRequire						m_mapEnchantRequireInfo;	// 속성 강화 필요 아이템 정보
	std::map< int, KIdentifyInfo >			m_mapIdentifyInfo;			// 아이템 감정

	//{{ 2011. 01. 04  김민성	속성 부적
#ifdef SERV_ATTRIBUTE_CHARM
	std::map< int, KItemAttributeEnchantInfo >	m_mapAttribAttachInfo;	// 속성 부적의 적용 정보
#endif SERV_ATTRIBUTE_CHARM
	//}}


};

DefRefreshSingletonInline( CXSLAttribEnchantItem );



