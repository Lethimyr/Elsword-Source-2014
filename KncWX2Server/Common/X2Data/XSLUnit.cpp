#include ".\xslunit.h"

CXSLUnit::CXSLUnit(void)
{
}

CXSLUnit::~CXSLUnit(void)
{
}

CXSLUnit::UnitData::UnitData()
{
	m_UnitUID				= 0;
	m_UserUID				= 0;
	m_UnitClass				= UC_NONE;

	m_Port					= 0;

	m_ED					= 0;
	//{{ 2011. 07. 11	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
	m_Rating				= 0;
	m_RPoint				= 0;
	m_APoint				= 0;
#else
	m_VSPoint				= 0;
#endif SERV_PVP_NEW_SYSTEM
	//}}	
	m_Level					= 0;
	m_EXP					= 0;

	m_Win					= 0;
	m_Lose					= 0;
	m_Seceder				= 0;

	m_NowBaseLevelEXP		= 0;
	m_NextBaseLevelEXP		= 0;

	m_EquipInventorySize		= 0;		
	m_SkillInventorySize		= 0;			
	m_MaterialInventorySize		= 0;		
	m_SpecialInventorySize		= 0;		
	m_CardInventorySize			= 0;			
	m_QuickSlotInventorySize	= 0;		
	m_AvartaInventorySize		= 0;		


	m_QuickSlotSize			= 0;

	m_RemainStatPoint		= 0;
	m_pInventory			= NULL;

	m_nStraightVictories	= 0;
	m_nMapID				= 0;

	m_bIsGameBang			= false;

	m_pInventory = NULL;//new CX2Inventory( pOwnerUnit );
}

CXSLUnit::UnitData::~UnitData()
{
}



CXSLUnit::UnitData& CXSLUnit::UnitData::operator=( const KUnitInfo& data )
{
	m_UnitUID				= data.m_nUnitUID;
	m_UserUID				= data.m_iOwnerUserUID;
	m_UnitClass				= static_cast<UNIT_CLASS>(data.m_cUnitClass);

	m_IP					= data.m_wstrIP;
	m_Port					= data.m_usPort;

	m_NickName				= data.m_wstrNickName;

	m_ED					= data.m_iED;
	//{{ 2011. 07. 11	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
	m_Rating				= data.m_iRating;
	m_RPoint				= data.m_iRPoint;
	m_APoint				= data.m_iAPoint;
#else
	m_VSPoint				= data.m_iVSPoint;
#endif SERV_PVP_NEW_SYSTEM
	//}}
	m_Level					= data.m_ucLevel;
	m_EXP					= data.m_iEXP;	

	m_Win					= data.m_iWin;
	m_Lose					= data.m_iLose;

	m_NowBaseLevelEXP		= data.m_nNowBaseLevelEXP;
	m_NextBaseLevelEXP		= data.m_nNextBaseLevelEXP;

	//m_RemainStatPoint		= data.m_ucRemainStatPoint;

	m_nStraightVictories	= data.m_nStraightVictories;
	//{{ 2012. 02. 02	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	m_nMapID				= data.m_kLastPos.m_iMapID;
#else
	m_nMapID				= data.m_nMapID;
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}	

	m_bIsGameBang			= data.m_bIsGameBang;

	m_Stat.m_fBaseHP		= static_cast<float>(data.m_kStat.m_iBaseHP);
	m_Stat.m_fAtkPhysic		= static_cast<float>(data.m_kStat.m_iAtkPhysic);
	m_Stat.m_fAtkMagic		= static_cast<float>(data.m_kStat.m_iAtkMagic);
	m_Stat.m_fDefPhysic		= static_cast<float>(data.m_kStat.m_iDefPhysic);
	m_Stat.m_fDefMagic		= static_cast<float>(data.m_kStat.m_iDefMagic);

	return *this;
}

//{{ 2012. 09. 11	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
// 랭크 업/다운 Rating Point boundary
CXSLUnit::PVP_RANK CXSLUnit::ComputeNewPVPRank( IN CXSLUnit::PVP_RANK eCurrentPvpRank, IN const int iNowRating )
{
	char cNowPvpRank = eCurrentPvpRank;
	int iLowerRating;
	int iUpperRating;

	while( 1 )
	{
		switch( cNowPvpRank )
		{
		case PVPRANK_RANK_E:	iLowerRating = 0;		iUpperRating = 250;			break;
		case PVPRANK_RANK_D:	iLowerRating = 150;		iUpperRating = 550;			break;
		case PVPRANK_RANK_C:	iLowerRating = 450;		iUpperRating = 950;			break;
		case PVPRANK_RANK_B:	iLowerRating = 850;		iUpperRating = 1350;		break;
		case PVPRANK_RANK_A:	iLowerRating = 1250;	iUpperRating = 1850;		break;
		case PVPRANK_RANK_S:	iLowerRating = 1750;	iUpperRating = 2350;		break;
		case PVPRANK_RANK_SS:	iLowerRating = 2250;	iUpperRating = 2950;		break;
		case PVPRANK_RANK_SSS:	iLowerRating = 2850;
		//case PVPRANK_RANK_SSS:	iLowerRating = 2950;
			if( iNowRating <= PE_END )
			{
				iUpperRating = PE_END+1;
				break;
			}
		default:
			START_LOG( cerr, L"현재 랭크에 대한 정보가 이상함!" )
				<< BUILD_LOG( static_cast<PVP_RANK>( cNowPvpRank ) )
				<< BUILD_LOG( iNowRating )
				<< END_LOG;
			return CXSLUnit::PVPRANK_NONE;
		}

		if( iNowRating < iLowerRating )
		{
			// 랭크 다운
			--cNowPvpRank;
		}
		else if( iUpperRating <= iNowRating )
		{
			// 랭크 업
			++cNowPvpRank;
		}
		else
		{
			return static_cast<PVP_RANK>( cNowPvpRank );
		}
	}
}
#endif SERV_2012_PVP_SEASON2
//}}


//////////////////////////////////////////////////////////////////////////
// 해당 UnitClass의 하위 Class를 얻는 함수
CXSLUnit::UNIT_CLASS CXSLUnit::GetUnitClassDownGrade( UNIT_CLASS eUnitClass )
{
	switch( eUnitClass )
	{
	case CXSLUnit::UC_ELSWORD_SWORDMAN:		return CXSLUnit::UC_ELSWORD_SWORDMAN;
	case CXSLUnit::UC_ELSWORD_KNIGHT:		return CXSLUnit::UC_ELSWORD_SWORDMAN;
	case CXSLUnit::UC_ELSWORD_MAGIC_KNIGHT:	return CXSLUnit::UC_ELSWORD_SWORDMAN;
#ifdef SERV_ELSWORD_SHEATH_KNIGHT
	case CXSLUnit::UC_ELSWORD_SHEATH_KNIGHT:return CXSLUnit::UC_ELSWORD_SWORDMAN;
#endif SERV_ELSWORD_SHEATH_KNIGHT
		//{{ 2009. 10. 24  최육사	엘소드2차전직
	case CXSLUnit::UC_ELSWORD_LORD_KNIGHT:	return CXSLUnit::UC_ELSWORD_KNIGHT;
	case CXSLUnit::UC_ELSWORD_RUNE_SLAYER:	return CXSLUnit::UC_ELSWORD_MAGIC_KNIGHT;
		//}}
#ifdef SERV_ELSWORD_INFINITY_SWORD
	case CXSLUnit::UC_ELSWORD_INFINITY_SWORD:	return CXSLUnit::UC_ELSWORD_SHEATH_KNIGHT;
#endif

	case CXSLUnit::UC_ARME_VIOLET_MAGE:		return CXSLUnit::UC_ARME_VIOLET_MAGE;
	case CXSLUnit::UC_ARME_HIGH_MAGICIAN:	return CXSLUnit::UC_ARME_VIOLET_MAGE;
	case CXSLUnit::UC_ARME_DARK_MAGICIAN:	return CXSLUnit::UC_ARME_VIOLET_MAGE;
#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
	case CXSLUnit::UC_ARME_BATTLE_MAGICIAN:	return CXSLUnit::UC_ARME_VIOLET_MAGE;
#endif SERV_ADD_ARME_BATTLE_MAGICIAN
		//{{ 2009. 12. 11  최육사	아이샤 2차 전직
	case CXSLUnit::UC_ARME_ELEMENTAL_MASTER:return CXSLUnit::UC_ARME_HIGH_MAGICIAN;
	case CXSLUnit::UC_ARME_VOID_PRINCESS:	return CXSLUnit::UC_ARME_DARK_MAGICIAN;
		//}}
#ifdef SERV_ARME_DIMENSION_WITCH
	case CXSLUnit::UC_ARME_DIMENSION_WITCH:	return CXSLUnit::UC_ARME_BATTLE_MAGICIAN;
#endif

	case CXSLUnit::UC_LIRE_ELVEN_RANGER:	return CXSLUnit::UC_LIRE_ELVEN_RANGER;
	case CXSLUnit::UC_LIRE_COMBAT_RANGER:	return CXSLUnit::UC_LIRE_ELVEN_RANGER;
	case CXSLUnit::UC_LIRE_SNIPING_RANGER:	return CXSLUnit::UC_LIRE_ELVEN_RANGER;
#ifdef	SERV_TRAPPING_RANGER_TEST
	case CXSLUnit::UC_LIRE_TRAPPING_RANGER:	return CXSLUnit::UC_LIRE_ELVEN_RANGER;
#endif	SERV_TRAPPING_RANGER_TEST
		//{{ 2009. 11. 23  최육사	레나2차전직
	case CXSLUnit::UC_LIRE_WIND_SNEAKER:	return CXSLUnit::UC_LIRE_COMBAT_RANGER;
	case CXSLUnit::UC_LIRE_GRAND_ARCHER:	return CXSLUnit::UC_LIRE_SNIPING_RANGER;
		//}}
#ifdef SERV_RENA_NIGHT_WATCHER
	case CXSLUnit::UC_LIRE_NIGHT_WATCHER:	return CXSLUnit::UC_LIRE_TRAPPING_RANGER;
#endif

	case CXSLUnit::UC_RAVEN_FIGHTER:		return CXSLUnit::UC_RAVEN_FIGHTER;
	case CXSLUnit::UC_RAVEN_SOUL_TAKER:		return CXSLUnit::UC_RAVEN_FIGHTER;
	case CXSLUnit::UC_RAVEN_OVER_TAKER:		return CXSLUnit::UC_RAVEN_FIGHTER;
#ifdef SERV_RAVEN_WEAPON_TAKER
	case CXSLUnit::UC_RAVEN_WEAPON_TAKER:	return CXSLUnit::UC_RAVEN_FIGHTER;
#endif SERV_RAVEN_WEAPON_TAKER
		//{{ 2010.01.20 손영준 레이븐 2차 전직
	case CXSLUnit::UC_RAVEN_BLADE_MASTER:	return CXSLUnit::UC_RAVEN_SOUL_TAKER;
	case CXSLUnit::UC_RAVEN_RECKLESS_FIST:	return CXSLUnit::UC_RAVEN_OVER_TAKER;
		//}} 
#ifdef SERV_RAVEN_VETERAN_COMMANDER
	case CXSLUnit::UC_RAVEN_VETERAN_COMMANDER:	return CXSLUnit::UC_RAVEN_WEAPON_TAKER;
#endif

	case CXSLUnit::UC_EVE_NASOD:			return CXSLUnit::UC_EVE_NASOD;
	case CXSLUnit::UC_EVE_EXOTIC_GEAR:		return CXSLUnit::UC_EVE_NASOD;
	case CXSLUnit::UC_EVE_ARCHITECTURE:		return CXSLUnit::UC_EVE_NASOD;
#ifdef SERV_EVE_ELECTRA
	case CXSLUnit::UC_EVE_ELECTRA:			return CXSLUnit::UC_EVE_NASOD;
#endif SERV_EVE_ELECTRA
		//{{ 2010.01.19 // kimhc // 이브 2차 전직
	case CXSLUnit::UC_EVE_CODE_NEMESIS:		return CXSLUnit::UC_EVE_EXOTIC_GEAR;
	case CXSLUnit::UC_EVE_CODE_EMPRESS:		return CXSLUnit::UC_EVE_ARCHITECTURE;
		//}} 
#ifdef SERV_EVE_BATTLE_SERAPH
	case CXSLUnit::UC_EVE_BATTLE_SERAPH:	return CXSLUnit::UC_EVE_ELECTRA;
#endif

		// kimhc // 2010-12-23 에 추가될 신캐릭터 청
	case CXSLUnit::UC_CHUNG_IRON_CANNON:	return CXSLUnit::UC_CHUNG_IRON_CANNON;
		//{{ kimhc // 2011.1.3 // 청 1차 전직
	case CXSLUnit::UC_CHUNG_FURY_GUARDIAN:		return CXSLUnit::UC_CHUNG_IRON_CANNON;
	case CXSLUnit::UC_CHUNG_SHOOTING_GUARDIAN:	return CXSLUnit::UC_CHUNG_IRON_CANNON;
		//}} kimhc // 2011.1.3 // 청 1차 전직
#ifdef SERV_ADD_CHUNG_SHELLING_GUARDIAN
	case CXSLUnit::UC_CHUNG_SHELLING_GUARDIAN:	return CXSLUnit::UC_CHUNG_IRON_CANNON;
#endif		
		//{{ JHKang / 강정훈 / 2011.6.13 / 청 2차 전직
	case CXSLUnit::UC_CHUNG_IRON_PALADIN:	return CXSLUnit::UC_CHUNG_FURY_GUARDIAN;
	case CXSLUnit::UC_CHUNG_DEADLY_CHASER:	return CXSLUnit::UC_CHUNG_SHOOTING_GUARDIAN;
		//}}
#ifdef SERV_CHUNG_TACTICAL_TROOPER
	case CXSLUnit::UC_CHUNG_TACTICAL_TROOPER:	return CXSLUnit::UC_CHUNG_SHELLING_GUARDIAN;
#endif

#ifdef SERV_ARA_MARTIAL_ARTIST
	case CXSLUnit::UC_ARA_MARTIAL_ARTIST:	return CXSLUnit::UC_ARA_MARTIAL_ARTIST;
#endif
#ifdef SERV_ARA_CHANGE_CLASS_FIRST
	case CXSLUnit::UC_ARA_LITTLE_HSIEN:		return CXSLUnit::UC_ARA_MARTIAL_ARTIST;
	case CXSLUnit::UC_ARA_SAKRA_DEVANAM:	return CXSLUnit::UC_ARA_LITTLE_HSIEN;
#endif //SERV_ARA_CHANGE_CLASS_FIRST
#ifdef SERV_NEW_CHARACTER_EL
	case CXSLUnit::UC_ELESIS_KNIGHT:			return CXSLUnit::UC_ELESIS_KNIGHT;
	case CXSLUnit::UC_ELESIS_SABER_KNIGHT:		return CXSLUnit::UC_ELESIS_KNIGHT;
	case CXSLUnit::UC_ELESIS_PYRO_KNIGHT:		return CXSLUnit::UC_ELESIS_KNIGHT;
#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE	  // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트
	case CXSLUnit::UC_ELESIS_GRAND_MASTER:		return CXSLUnit::UC_ELESIS_SABER_KNIGHT;
	case CXSLUnit::UC_ELESIS_BLAZING_HEART:		return CXSLUnit::UC_ELESIS_PYRO_KNIGHT;
#endif // SERV_ELESIS_SECOND_CLASS_CHANGE // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트

#endif // SERV_NEW_CHARACTER_EL
#ifdef SERV_ARA_CHANGE_CLASS_SECOND
	case CXSLUnit::UC_ARA_LITTLE_DEVIL:			return CXSLUnit::UC_ARA_MARTIAL_ARTIST;
	case CXSLUnit::UC_ARA_YAMA_RAJA:			return CXSLUnit::UC_ARA_LITTLE_DEVIL;
#endif // SERV_ARA_CHANGE_CLASS_SECOND
#ifdef SERV_9TH_NEW_CHARACTER
	case CXSLUnit::UC_ADD_PSYCHIC_TRACER:		return CXSLUnit::UC_ADD_NASOD_RULER;
#endif // SERV_9TH_NEW_CHARACTER
#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
	case CXSLUnit::UC_ADD_LUNATIC_PSYKER:		return CXSLUnit::UC_ADD_PSYCHIC_TRACER;
#endif //SERV_ADD_LUNATIC_PSYKER
	}

	return CXSLUnit::UC_NONE;
}

//////////////////////////////////////////////////////////////////////////
// 해당 UnitClass의 기본 UnitClass를 얻는 함수
CXSLUnit::UNIT_CLASS CXSLUnit::GetUnitClassBaseGrade( UNIT_CLASS eUnitClass )
{
	switch( eUnitClass )
	{
	case CXSLUnit::UC_ELSWORD_SWORDMAN:
	case CXSLUnit::UC_ELSWORD_KNIGHT:
	case CXSLUnit::UC_ELSWORD_MAGIC_KNIGHT:
#ifdef SERV_ELSWORD_SHEATH_KNIGHT
	case CXSLUnit::UC_ELSWORD_SHEATH_KNIGHT:
#endif SERV_ELSWORD_SHEATH_KNIGHT
		//{{ 2009. 10. 24  최육사	엘소드2차전직
	case CXSLUnit::UC_ELSWORD_LORD_KNIGHT:
	case CXSLUnit::UC_ELSWORD_RUNE_SLAYER:
#ifdef SERV_ELSWORD_INFINITY_SWORD
	case CXSLUnit::UC_ELSWORD_INFINITY_SWORD:
#endif
		//}}
		return CXSLUnit::UC_ELSWORD_SWORDMAN;

	case CXSLUnit::UC_ARME_VIOLET_MAGE:
	case CXSLUnit::UC_ARME_HIGH_MAGICIAN:
	case CXSLUnit::UC_ARME_DARK_MAGICIAN:
#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
	case CXSLUnit::UC_ARME_BATTLE_MAGICIAN:
#endif SERV_ADD_ARME_BATTLE_MAGICIAN
		//{{ 2009. 12. 11  최육사	아이샤 2차 전직
	case CXSLUnit::UC_ARME_ELEMENTAL_MASTER:
	case CXSLUnit::UC_ARME_VOID_PRINCESS:
		//}}
#ifdef SERV_ARME_DIMENSION_WITCH
	case CXSLUnit::UC_ARME_DIMENSION_WITCH:
#endif
		return CXSLUnit::UC_ARME_VIOLET_MAGE;

	case CXSLUnit::UC_LIRE_ELVEN_RANGER:
	case CXSLUnit::UC_LIRE_COMBAT_RANGER:
	case CXSLUnit::UC_LIRE_SNIPING_RANGER:
#ifdef	SERV_TRAPPING_RANGER_TEST
	case CXSLUnit::UC_LIRE_TRAPPING_RANGER:
#endif	SERV_TRAPPING_RANGER_TEST
		//{{ 2009. 11. 23  최육사	레나2차전직
	case CXSLUnit::UC_LIRE_WIND_SNEAKER:
	case CXSLUnit::UC_LIRE_GRAND_ARCHER:
		//}}
#ifdef SERV_RENA_NIGHT_WATCHER
	case CXSLUnit::UC_LIRE_NIGHT_WATCHER:
#endif
		return CXSLUnit::UC_LIRE_ELVEN_RANGER;

	case CXSLUnit::UC_RAVEN_FIGHTER:
	case CXSLUnit::UC_RAVEN_SOUL_TAKER:
	case CXSLUnit::UC_RAVEN_OVER_TAKER:
#ifdef SERV_RAVEN_WEAPON_TAKER
	case CXSLUnit::UC_RAVEN_WEAPON_TAKER:
#endif SERV_RAVEN_WEAPON_TAKER
		//{{ 2010.01.20 // 손영준 레이븐 2차 전직
	case CXSLUnit::UC_RAVEN_BLADE_MASTER:
	case CXSLUnit::UC_RAVEN_RECKLESS_FIST:
		//}}
#ifdef SERV_RAVEN_VETERAN_COMMANDER
	case CXSLUnit::UC_RAVEN_VETERAN_COMMANDER:
#endif
		return CXSLUnit::UC_RAVEN_FIGHTER;

	case CXSLUnit::UC_EVE_NASOD:
	case CXSLUnit::UC_EVE_EXOTIC_GEAR:
	case CXSLUnit::UC_EVE_ARCHITECTURE:
#ifdef SERV_EVE_ELECTRA
	case CXSLUnit::UC_EVE_ELECTRA:
#endif SERV_EVE_ELECTRA
		//{{ 2010.01.19 // kimhc // 이브 2차 전직
	case CXSLUnit::UC_EVE_CODE_NEMESIS:
	case CXSLUnit::UC_EVE_CODE_EMPRESS:
#ifdef SERV_EVE_BATTLE_SERAPH
	case CXSLUnit::UC_EVE_BATTLE_SERAPH:
#endif
		//}}
		return CXSLUnit::UC_EVE_NASOD;

		//kimhc // 2010-12-23 에 추가될 신캐릭터 청
	case CXSLUnit::UC_CHUNG_IRON_CANNON:
		//{{ kimhc // 2011.01.03 // 청 1차 전직
	case CXSLUnit::UC_CHUNG_FURY_GUARDIAN:
	case CXSLUnit::UC_CHUNG_SHOOTING_GUARDIAN:
		//}} kimhc // 2011.01.03 // 청 1차 전직
#ifdef SERV_ADD_CHUNG_SHELLING_GUARDIAN
	case CXSLUnit::UC_CHUNG_SHELLING_GUARDIAN:
#endif
		//{{ JHKang / 강정훈 / 2011.6.13 / 청 2차 전직
	case CXSLUnit::UC_CHUNG_IRON_PALADIN:
	case CXSLUnit::UC_CHUNG_DEADLY_CHASER:
		//}}
#ifdef SERV_CHUNG_TACTICAL_TROOPER
	case CXSLUnit::UC_CHUNG_TACTICAL_TROOPER:
#endif
		return CXSLUnit::UC_CHUNG_IRON_CANNON;

#ifdef SERV_ARA_MARTIAL_ARTIST
	case CXSLUnit::UC_ARA_MARTIAL_ARTIST:
#ifdef SERV_ARA_CHANGE_CLASS_FIRST
	case CXSLUnit::UC_ARA_LITTLE_HSIEN:
	case CXSLUnit::UC_ARA_SAKRA_DEVANAM:
#endif //SERV_ARA_CHANGE_CLASS_FIRST
#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
	case CXSLUnit::UC_ARA_LITTLE_DEVIL:
	case CXSLUnit::UC_ARA_YAMA_RAJA:
#endif // SERV_ARA_CHANGE_CLASS_SECOND
		return CXSLUnit::UC_ARA_MARTIAL_ARTIST;
#endif
#ifdef SERV_NEW_CHARACTER_EL
	case CXSLUnit::UC_ELESIS_KNIGHT:
	case CXSLUnit::UC_ELESIS_SABER_KNIGHT:
	case CXSLUnit::UC_ELESIS_PYRO_KNIGHT:
#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE	  // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트
	case CXSLUnit::UC_ELESIS_GRAND_MASTER:
	case CXSLUnit::UC_ELESIS_BLAZING_HEART:
#endif // SERV_ELESIS_SECOND_CLASS_CHANGE // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트
		return CXSLUnit::UC_ELESIS_KNIGHT;
#endif // SERV_NEW_CHARACTER_EL

#ifdef SERV_9TH_NEW_CHARACTER // 김태환 ( 캐릭터 추가용 )
	case UC_ADD_NASOD_RULER:
	case UC_ADD_PSYCHIC_TRACER:
#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
	case UC_ADD_LUNATIC_PSYKER:
#endif //SERV_ADD_LUNATIC_PSYKER
		return UC_ADD_NASOD_RULER;
#endif //SERV_9TH_NEW_CHARACTER
	}

	return CXSLUnit::UC_NONE;
}

//////////////////////////////////////////////////////////////////////////
//{{ 2009. 10. 24  최육사	엘소드2차전직 - 1차전직 UnitClass의 상위 Class를 얻는 함수
CXSLUnit::UNIT_CLASS CXSLUnit::GetUnitClassUpGrade( UNIT_CLASS eUnitClass )
{
	switch( eUnitClass )
	{
	case CXSLUnit::UC_ELSWORD_KNIGHT:		return CXSLUnit::UC_ELSWORD_LORD_KNIGHT;
	case CXSLUnit::UC_ELSWORD_MAGIC_KNIGHT:	return CXSLUnit::UC_ELSWORD_RUNE_SLAYER;
#ifdef SERV_ELSWORD_INFINITY_SWORD
	case CXSLUnit::UC_ELSWORD_SHEATH_KNIGHT:	return CXSLUnit::UC_ELSWORD_INFINITY_SWORD;
#endif

		//{{ 2009. 11. 23  최육사	레나2차전직
	case CXSLUnit::UC_LIRE_COMBAT_RANGER:	return CXSLUnit::UC_LIRE_WIND_SNEAKER;
	case CXSLUnit::UC_LIRE_SNIPING_RANGER:	return CXSLUnit::UC_LIRE_GRAND_ARCHER;
		//}}
#ifdef SERV_RENA_NIGHT_WATCHER
	case CXSLUnit::UC_LIRE_TRAPPING_RANGER:	return CXSLUnit::UC_LIRE_NIGHT_WATCHER;
#endif

		//{{ 2009. 12. 11  최육사	아이샤 2차 전직
	case CXSLUnit::UC_ARME_HIGH_MAGICIAN:	return CXSLUnit::UC_ARME_ELEMENTAL_MASTER;
	case CXSLUnit::UC_ARME_DARK_MAGICIAN:	return CXSLUnit::UC_ARME_VOID_PRINCESS;
		//}}
#ifdef SERV_ARME_DIMENSION_WITCH
	case CXSLUnit::UC_ARME_BATTLE_MAGICIAN:	return CXSLUnit::UC_ARME_DIMENSION_WITCH;
#endif

		//{{ 2010. 1. 20  손영준	레이븐 2차 전직
	case CXSLUnit::UC_RAVEN_SOUL_TAKER:	return CXSLUnit::UC_RAVEN_BLADE_MASTER;
	case CXSLUnit::UC_RAVEN_OVER_TAKER:	return CXSLUnit::UC_RAVEN_RECKLESS_FIST;
		//}}
#ifdef SERV_RAVEN_VETERAN_COMMANDER
	case CXSLUnit::UC_RAVEN_WEAPON_TAKER:	return CXSLUnit::UC_RAVEN_VETERAN_COMMANDER;
#endif

		//{{ 2010.01.19 // kimhc // 이브 2차 전직
	case CXSLUnit::UC_EVE_EXOTIC_GEAR:		return CXSLUnit::UC_EVE_CODE_NEMESIS;
	case CXSLUnit::UC_EVE_ARCHITECTURE:		return CXSLUnit::UC_EVE_CODE_EMPRESS;
		//}}
#ifdef SERV_EVE_BATTLE_SERAPH
	case CXSLUnit::UC_EVE_ELECTRA:			return CXSLUnit::UC_EVE_BATTLE_SERAPH;
#endif

		//{{ JHKang / 강정훈 / 2011.6.13 / 청 2차 전직
	case CXSLUnit::UC_CHUNG_FURY_GUARDIAN:		return CXSLUnit::UC_CHUNG_IRON_PALADIN;
	case CXSLUnit::UC_CHUNG_SHOOTING_GUARDIAN:	return CXSLUnit::UC_CHUNG_DEADLY_CHASER;
		//}}
#ifdef SERV_CHUNG_TACTICAL_TROOPER
	case CXSLUnit::UC_CHUNG_SHELLING_GUARDIAN:	return CXSLUnit::UC_CHUNG_TACTICAL_TROOPER;
#endif

#ifdef SERV_ARA_CHANGE_CLASS_FIRST
	case CXSLUnit::UC_ARA_LITTLE_HSIEN:	return CXSLUnit::UC_ARA_SAKRA_DEVANAM;
#endif //SERV_ARA_CHANGE_CLASS_FIRST

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
	case CXSLUnit::UC_ARA_LITTLE_DEVIL:	return CXSLUnit::UC_ARA_YAMA_RAJA;
#endif // SERV_ARA_CHANGE_CLASS_SECOND

#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE	  // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트
	case CXSLUnit::UC_ELESIS_SABER_KNIGHT:	return CXSLUnit::UC_ELESIS_GRAND_MASTER;
	case CXSLUnit::UC_ELESIS_PYRO_KNIGHT:	return CXSLUnit::UC_ELESIS_BLAZING_HEART;
#endif // SERV_ELESIS_SECOND_CLASS_CHANGE // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트

#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
	case CXSLUnit::UC_ADD_PSYCHIC_TRACER:	return CXSLUnit::UC_ADD_LUNATIC_PSYKER;
#endif //SERV_ADD_LUNATIC_PSYKER
	}

	return CXSLUnit::UC_NONE;
}
//}}

//////////////////////////////////////////////////////////////////////////
// UnitClass를 UnitType으로 변환하는 함수
CXSLUnit::UNIT_TYPE CXSLUnit::GetUnitClassToUnitType( UNIT_CLASS eUnitClass )
{
	switch( eUnitClass )
	{
	case CXSLUnit::UC_ELSWORD_SWORDMAN:
	case CXSLUnit::UC_ELSWORD_KNIGHT:
	case CXSLUnit::UC_ELSWORD_MAGIC_KNIGHT:
#ifdef SERV_ELSWORD_SHEATH_KNIGHT
	case CXSLUnit::UC_ELSWORD_SHEATH_KNIGHT:
#endif SERV_ELSWORD_SHEATH_KNIGHT
		//{{ 2009. 10. 24  최육사	엘소드2차전직
	case CXSLUnit::UC_ELSWORD_LORD_KNIGHT:
	case CXSLUnit::UC_ELSWORD_RUNE_SLAYER:
		//}}
#ifdef SERV_ELSWORD_INFINITY_SWORD
	case CXSLUnit::UC_ELSWORD_INFINITY_SWORD:
#endif
		return CXSLUnit::UT_ELSWORD;

	case CXSLUnit::UC_LIRE_ELVEN_RANGER:
	case CXSLUnit::UC_LIRE_COMBAT_RANGER:
	case CXSLUnit::UC_LIRE_SNIPING_RANGER:
#ifdef	SERV_TRAPPING_RANGER_TEST
	case CXSLUnit::UC_LIRE_TRAPPING_RANGER:
#endif	SERV_TRAPPING_RANGER_TEST
		//{{ 2009. 11. 23  최육사	레나2차전직
	case CXSLUnit::UC_LIRE_WIND_SNEAKER:
	case CXSLUnit::UC_LIRE_GRAND_ARCHER:
		//}}
#ifdef SERV_RENA_NIGHT_WATCHER
	case CXSLUnit::UC_LIRE_NIGHT_WATCHER:
#endif
		return CXSLUnit::UT_LIRE;

	case CXSLUnit::UC_ARME_VIOLET_MAGE:
	case CXSLUnit::UC_ARME_HIGH_MAGICIAN:
	case CXSLUnit::UC_ARME_DARK_MAGICIAN:
#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
	case CXSLUnit::UC_ARME_BATTLE_MAGICIAN:
#endif SERV_ADD_ARME_BATTLE_MAGICIAN
		//{{ 2009. 12. 11  최육사	아이샤 2차 전직
	case CXSLUnit::UC_ARME_ELEMENTAL_MASTER:
	case CXSLUnit::UC_ARME_VOID_PRINCESS:
		//}}
#ifdef SERV_ARME_DIMENSION_WITCH
	case CXSLUnit::UC_ARME_DIMENSION_WITCH:
#endif
		return CXSLUnit::UT_ARME;

	case CXSLUnit::UC_RAVEN_FIGHTER:
	case CXSLUnit::UC_RAVEN_SOUL_TAKER:
	case CXSLUnit::UC_RAVEN_OVER_TAKER:
#ifdef SERV_RAVEN_WEAPON_TAKER
	case CXSLUnit::UC_RAVEN_WEAPON_TAKER:
#endif SERV_RAVEN_WEAPON_TAKER
		//{{ 2010.01.20 // 손영준	레이븐 2차 전직
	case CXSLUnit::UC_RAVEN_BLADE_MASTER:
	case CXSLUnit::UC_RAVEN_RECKLESS_FIST:
		//}}
#ifdef SERV_RAVEN_VETERAN_COMMANDER
	case CXSLUnit::UC_RAVEN_VETERAN_COMMANDER:
#endif
		return CXSLUnit::UT_RAVEN;

	case CXSLUnit::UC_EVE_NASOD:
	case CXSLUnit::UC_EVE_EXOTIC_GEAR:
	case CXSLUnit::UC_EVE_ARCHITECTURE:
#ifdef SERV_EVE_ELECTRA
	case CXSLUnit::UC_EVE_ELECTRA:
#endif SERV_EVE_ELECTRA
		//{{ 2010.01.19 // kimhc // 이브 2차 전직
	case CXSLUnit::UC_EVE_CODE_NEMESIS:
	case CXSLUnit::UC_EVE_CODE_EMPRESS:
		//}}
#ifdef SERV_EVE_BATTLE_SERAPH
	case CXSLUnit::UC_EVE_BATTLE_SERAPH:
#endif
		return CXSLUnit::UT_EVE;

	// kimhc // 2010-12-23 에 추가될 신캐릭터 청
	case CXSLUnit::UC_CHUNG_IRON_CANNON:
		//{{ kimhc // 2011.1.3 // 청 1차 전직
	case CXSLUnit::UC_CHUNG_FURY_GUARDIAN:
	case CXSLUnit::UC_CHUNG_SHOOTING_GUARDIAN:
		//}} kimhc // 2011.1.3 // 청 1차 전직
#ifdef SERV_ADD_CHUNG_SHELLING_GUARDIAN
	case CXSLUnit::UC_CHUNG_SHELLING_GUARDIAN:
#endif
		//{{ JHKang / 강정훈 / 2011.6.13 / 청 2차 전직
	case CXSLUnit::UC_CHUNG_IRON_PALADIN:
	case CXSLUnit::UC_CHUNG_DEADLY_CHASER:
		//}}
#ifdef SERV_CHUNG_TACTICAL_TROOPER
	case CXSLUnit::UC_CHUNG_TACTICAL_TROOPER:
#endif
		return CXSLUnit::UT_CHUNG;

#ifdef SERV_ARA_MARTIAL_ARTIST
	case CXSLUnit::UC_ARA_MARTIAL_ARTIST:
#ifdef SERV_ARA_CHANGE_CLASS_FIRST
	case CXSLUnit::UC_ARA_LITTLE_HSIEN:
	case CXSLUnit::UC_ARA_SAKRA_DEVANAM:
#endif //SERV_ARA_CHANGE_CLASS_FIRST
#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
	case CXSLUnit::UC_ARA_LITTLE_DEVIL:
	case CXSLUnit::UC_ARA_YAMA_RAJA:
#endif // SERV_ARA_CHANGE_CLASS_SECOND
		return CXSLUnit::UT_ARA;
#endif
#ifdef SERV_NEW_CHARACTER_EL
	case CXSLUnit::UC_ELESIS_KNIGHT:
	case CXSLUnit::UC_ELESIS_SABER_KNIGHT:
	case CXSLUnit::UC_ELESIS_PYRO_KNIGHT:
#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE	  // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트
	case CXSLUnit::UC_ELESIS_GRAND_MASTER:
	case CXSLUnit::UC_ELESIS_BLAZING_HEART:
#endif // SERV_ELESIS_SECOND_CLASS_CHANGE // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트		
		return CXSLUnit::UT_ELESIS;
#endif // SERV_NEW_CHARACTER_EL

#ifdef SERV_9TH_NEW_CHARACTER // 김태환 ( 캐릭터 추가용 )
	case UC_ADD_NASOD_RULER:
	case UC_ADD_PSYCHIC_TRACER:
#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
	case UC_ADD_LUNATIC_PSYKER:
#endif //SERV_ADD_LUNATIC_PSYKER
		return UT_ADD;
#endif //SERV_9TH_NEW_CHARACTER
	}

	return CXSLUnit::UT_NONE;
}

bool CXSLUnit::IsFirstChangeJob( CXSLUnit::UNIT_CLASS unitClass )
{
	switch( unitClass )
	{
	case CXSLUnit::UC_ELSWORD_KNIGHT:
	case CXSLUnit::UC_ELSWORD_MAGIC_KNIGHT:
#ifdef SERV_ELSWORD_SHEATH_KNIGHT
	case CXSLUnit::UC_ELSWORD_SHEATH_KNIGHT:
#endif SERV_ELSWORD_SHEATH_KNIGHT
		//{{ 2009. 11. 23  최육사	레나2차전직
	case CXSLUnit::UC_LIRE_COMBAT_RANGER:
	case CXSLUnit::UC_LIRE_SNIPING_RANGER:
#ifdef	SERV_TRAPPING_RANGER_TEST
	case CXSLUnit::UC_LIRE_TRAPPING_RANGER:
#endif	SERV_TRAPPING_RANGER_TEST
		//}}
		//{{ 2009. 12. 11  최육사	아이샤 2차 전직
	case CXSLUnit::UC_ARME_HIGH_MAGICIAN:
	case CXSLUnit::UC_ARME_DARK_MAGICIAN:
#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
	case CXSLUnit::UC_ARME_BATTLE_MAGICIAN:
#endif SERV_ADD_ARME_BATTLE_MAGICIAN
		//}}

		//{{ 2010.1.20 손영준 레이븐 2차 전직
	case CXSLUnit::UC_RAVEN_SOUL_TAKER:
	case CXSLUnit::UC_RAVEN_OVER_TAKER:
		//}}
#ifdef SERV_RAVEN_WEAPON_TAKER
	case CXSLUnit::UC_RAVEN_WEAPON_TAKER:
#endif SERV_RAVEN_WEAPON_TAKER

		//{{ 2010.01.19 // kimhc // 이브 2차 전직
	case CXSLUnit::UC_EVE_EXOTIC_GEAR:
	case CXSLUnit::UC_EVE_ARCHITECTURE:
#ifdef SERV_EVE_ELECTRA
	case CXSLUnit::UC_EVE_ELECTRA:
#endif SERV_EVE_ELECTRA
		//}}
		//{{ kimhc // 2011.1.3 // 청 1차 전직
	case CXSLUnit::UC_CHUNG_FURY_GUARDIAN:
	case CXSLUnit::UC_CHUNG_SHOOTING_GUARDIAN:
		//}} kimhc // 2011.1.3 // 청 1차 전직
#ifdef SERV_ADD_CHUNG_SHELLING_GUARDIAN
	case CXSLUnit::UC_CHUNG_SHELLING_GUARDIAN:
#endif
#ifdef SERV_ARA_CHANGE_CLASS_FIRST
	case CXSLUnit::UC_ARA_LITTLE_HSIEN: //소선
#endif //SERV_ARA_CHANGE_CLASS_FIRST
#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 20130812 // 김현철
	case CXSLUnit::UC_ARA_LITTLE_DEVIL:			//소마
#endif // SERV_ARA_CHANGE_CLASS_SECOND
#ifdef SERV_NEW_CHARACTER_EL
	case CXSLUnit::UC_ELESIS_SABER_KNIGHT:
	case CXSLUnit::UC_ELESIS_PYRO_KNIGHT:
#endif // SERV_NEW_CHARACTER_EL
#ifdef SERV_9TH_NEW_CHARACTER // 김태환 ( 캐릭터 추가용 )
	case CXSLUnit::UC_ADD_PSYCHIC_TRACER:
#endif //SERV_9TH_NEW_CHARACTER
		return true;
	}

	return false;
}

bool CXSLUnit::IsSecondChangeJob( CXSLUnit::UNIT_CLASS unitClass )
{
	switch( unitClass )
	{
	case CXSLUnit::UC_ELSWORD_LORD_KNIGHT:
	case CXSLUnit::UC_ELSWORD_RUNE_SLAYER:
		//{{ 2009. 11. 23  최육사	레나2차전직
	case CXSLUnit::UC_LIRE_WIND_SNEAKER:
	case CXSLUnit::UC_LIRE_GRAND_ARCHER:
		//}}
		//{{ 2009. 12. 11  최육사	아이샤 2차 전직
	case CXSLUnit::UC_ARME_ELEMENTAL_MASTER:
	case CXSLUnit::UC_ARME_VOID_PRINCESS:
		//}}

		//{{ 2010.1.20 손영준 레이븐 2차 전직
	case CXSLUnit::UC_RAVEN_BLADE_MASTER:
	case CXSLUnit::UC_RAVEN_RECKLESS_FIST:
		//}}

		//{{ 2010.01.19 // kimhc // 이브 2차 전직
	case CXSLUnit::UC_EVE_CODE_NEMESIS:
	case CXSLUnit::UC_EVE_CODE_EMPRESS:
		//}}

		//{{ JHKang / 강정훈 / 2011.6.13 / 청 2차 전직
	case CXSLUnit::UC_CHUNG_IRON_PALADIN:
	case CXSLUnit::UC_CHUNG_DEADLY_CHASER:
		//}}

#ifdef SERV_ELSWORD_INFINITY_SWORD
	case CXSLUnit::UC_ELSWORD_INFINITY_SWORD:
#endif
#ifdef SERV_ARME_DIMENSION_WITCH
	case CXSLUnit::UC_ARME_DIMENSION_WITCH:
#endif
#ifdef SERV_RENA_NIGHT_WATCHER
	case CXSLUnit::UC_LIRE_NIGHT_WATCHER:
#endif
#ifdef SERV_RAVEN_VETERAN_COMMANDER
	case CXSLUnit::UC_RAVEN_VETERAN_COMMANDER:
#endif
#ifdef SERV_EVE_BATTLE_SERAPH
	case CXSLUnit::UC_EVE_BATTLE_SERAPH:
#endif
#ifdef SERV_CHUNG_TACTICAL_TROOPER
	case CXSLUnit::UC_CHUNG_TACTICAL_TROOPER:
#endif
#ifdef SERV_ARA_CHANGE_CLASS_FIRST
	case CXSLUnit::UC_ARA_SAKRA_DEVANAM:		//제천
#endif //SERV_ARA_CHANGE_CLASS_FIRST
#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
	case CXSLUnit::UC_ARA_YAMA_RAJA:			//명왕
#endif // SERV_ARA_CHANGE_CLASS_SECOND

#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE	  // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트
	case CXSLUnit::UC_ELESIS_GRAND_MASTER:		//그랜드 마스터
	case CXSLUnit::UC_ELESIS_BLAZING_HEART:		//블레이징 하트
#endif // SERV_ELESIS_SECOND_CLASS_CHANGE // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트

#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
	case CXSLUnit::UC_ADD_LUNATIC_PSYKER:		//루나틱 사이커
#endif //SERV_ADD_LUNATIC_PSYKER
		return true;
	}

	return false;
}

//{{ 2011. 11. 21  김민성	전직 변경 아이템
#ifdef SERV_UNIT_CLASS_CHANGE_ITEM
bool CXSLUnit::IsInitNormalJob( CXSLUnit::UNIT_CLASS unitClass )
{
	switch( unitClass )
	{
	case CXSLUnit::UC_ELSWORD_SWORDMAN:
	case CXSLUnit::UC_ARME_VIOLET_MAGE:
	case CXSLUnit::UC_LIRE_ELVEN_RANGER:
	case CXSLUnit::UC_RAVEN_FIGHTER:
	case CXSLUnit::UC_EVE_NASOD:
	case CXSLUnit::UC_CHUNG_IRON_CANNON:
#ifdef SERV_ARA_MARTIAL_ARTIST
	case CXSLUnit::UC_ARA_MARTIAL_ARTIST:
#endif
#ifdef SERV_NEW_CHARACTER_EL
	case CXSLUnit::UC_ELESIS_KNIGHT:
#endif //SERV_NEW_CHARACTER_EL
#ifdef SERV_9TH_NEW_CHARACTER // 김태환 ( 캐릭터 추가용 )
	case CXSLUnit::UC_ADD_NASOD_RULER:
#endif //SERV_9TH_NEW_CHARACTER
		return true;
	}

	return false;
}
#endif SERV_UNIT_CLASS_CHANGE_ITEM
//}}

//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
bool CXSLUnit::IsMan_UnitType( UNIT_TYPE eUnitType )
{
	switch( eUnitType )
	{
	case UT_ELSWORD:
	case UT_RAVEN:
	case UT_CHUNG:
#ifdef SERV_9TH_NEW_CHARACTER // 김태환
	case UT_ADD:
#endif //SERV_9TH_NEW_CHARACTER
		{
			return true;
		}break;
	}
	
/*
	UT_ARME
	UT_LIRE
	UT_EVE
	UT_ARA	
*/
	return false;
}
#endif SERV_RELATIONSHIP_SYSTEM
//}