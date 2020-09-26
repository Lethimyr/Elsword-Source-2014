#include ".\xslitem.h"
#include ".\XSLUnit.h"
//{{ 2011. 06. 01	최육사	강화된 아이템 보상 큐브
#ifdef SERV_ENCHANTED_ITEM_CUBE_REWARD
	#include ".\XSLAttribEnchantItem.h"
#endif SERV_ENCHANTED_ITEM_CUBE_REWARD
//}}

//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
	#include "Enum/Enum.h"
#endif SERV_RELATIONSHIP_SYSTEM
//}

//{{ 2012. 05. 23	김민성	[이벤트] 에이핑크가 응원합니다!
//#ifdef SERV_EVENT_APINK
	#include ".\xslitemmanager.h"
//#endif SERV_EVENT_APINK
//}}

#include "XSLSquareUnit.h"

CXSLItem::CXSLItem( ItemData* pItemData )
{
	m_pItemData		= pItemData;
	m_bEqip			= false;
}

CXSLItem::~CXSLItem(void)
{
	SAFE_DELETE( m_pItemData );
}

//{{ 2009. 7. 28  최육사	GM무기
bool CXSLItem::IsGMweapon( const int iItemID )
{
	switch( iItemID )
	{
	case 128015: // 엘소드GM무기
	case 128016: // 엘소드GM상의
	case 128017: // 레나GM무기
	case 128018: // 레나GM상의
	case 128019: // 아이샤GM무기
	case 128020: // 아이샤GM상의
	case 128078: // 레이븐GM무기
	case 128079: // 레이븐GM상의
	case 28005: // 이브 GM무기
	case 28006: // 이브 GM상의
	case 183248: // 엘리시스GM무기
	case 183249: // 엘리시스GM상의
		return true;
	}

	return false;
}
//}}

//{{ 2009. 9. 22  최육사	전직캐쉬
char CXSLItem::GetCashItemChangeUnitClass( const int iItemID )
{
	switch( iItemID )
	{
	case CI_CHANGE_JOB_ELSWORD_KNIGHT:	return CXSLUnit::UC_ELSWORD_KNIGHT;
	case CI_CHANGE_JOB_MAGIC_KNIGHT:	return CXSLUnit::UC_ELSWORD_MAGIC_KNIGHT;
#ifdef SERV_ELSWORD_SHEATH_KNIGHT
	case CI_CHANGE_JOB_SHEATH_KNIGHT:	return CXSLUnit::UC_ELSWORD_SHEATH_KNIGHT;
#endif SERV_ELSWORD_SHEATH_KNIGHT
		//{{ 2009. 10. 24  최육사	엘소드2차전직
	case CI_CHANGE_JOB_LORD_KNIGHT:		return CXSLUnit::UC_ELSWORD_LORD_KNIGHT;
	case CI_CHANGE_JOB_RUNE_SLAYER:		return CXSLUnit::UC_ELSWORD_RUNE_SLAYER;
		//}}
	case CI_CHANGE_JOB_COMBAT_RANGER:	return CXSLUnit::UC_LIRE_COMBAT_RANGER;
	case CI_CHANGE_JOB_SNIPING_RANGER:	return CXSLUnit::UC_LIRE_SNIPING_RANGER;
#ifdef SERV_TRAPPING_RANGER_TEST
	case CI_CHANGE_JOB_TRAPPING_RANGER:	return CXSLUnit::UC_LIRE_TRAPPING_RANGER;
#endif SERV_TRAPPING_RANGER_TEST
		//{{ 2009. 11. 23  최육사	레나2차전직
	case CI_CHANGE_JOB_WIND_SNEAKER:	return CXSLUnit::UC_LIRE_WIND_SNEAKER;
	case CI_CHANGE_JOB_GRAND_ARCHER:	return CXSLUnit::UC_LIRE_GRAND_ARCHER;
		//}}
	case CI_CHANGE_JOB_HIGH_MAGICIAN:	return CXSLUnit::UC_ARME_HIGH_MAGICIAN;
	case CI_CHANGE_JOB_DARK_MAGICIAN:	return CXSLUnit::UC_ARME_DARK_MAGICIAN;
#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
	case CI_CHANGE_JOB_BATTLE_MAGICIAN:	return CXSLUnit::UC_ARME_BATTLE_MAGICIAN;
#endif SERV_ADD_ARME_BATTLE_MAGICIAN
		//{{ 2009. 12. 11  최육사	아이샤 2차 전직
	case CI_CHANGE_JOB_ELMENTAL_MASTER: return CXSLUnit::UC_ARME_ELEMENTAL_MASTER;
	case CI_CHANGE_JOB_VOID_PRINCESS:	return CXSLUnit::UC_ARME_VOID_PRINCESS;
		//}}
	case CI_CHANGE_JOB_SOUL_TAKER:		return CXSLUnit::UC_RAVEN_SOUL_TAKER;
	case CI_CHANGE_JOB_OVER_TAKER:		return CXSLUnit::UC_RAVEN_OVER_TAKER;
#ifdef SERV_RAVEN_WEAPON_TAKER
	case CI_CHANGE_JOB_WEAPON_TAKER:	return CXSLUnit::UC_RAVEN_WEAPON_TAKER;
#endif SERV_RAVEN_WEAPON_TAKER
		//{{ 2010. 1. 20  손영준	레이븐 2차 전직
	case CI_CHANGE_JOB_BLADE_MASTER:	return CXSLUnit::UC_RAVEN_BLADE_MASTER;
	case CI_CHANGE_JOB_RECKLESS_FIST:	return CXSLUnit::UC_RAVEN_RECKLESS_FIST;
		//}}	
	case CI_CHANGE_JOB_EXOTIC_GEAR:		return CXSLUnit::UC_EVE_EXOTIC_GEAR;
	case CI_CHANGE_JOB_ARCHITECTURE:	return CXSLUnit::UC_EVE_ARCHITECTURE;
#ifdef SERV_EVE_ELECTRA
	case CI_CHANGE_JOB_EVE_ELECTRA:		return CXSLUnit::UC_EVE_ELECTRA;
#endif SERV_EVE_ELECTRA
		//{{ 2010. 1. 20  손영준	이브 2차 전직
	case CI_CHANGE_JOB_NEMESIS:			return CXSLUnit::UC_EVE_CODE_NEMESIS;
	case CI_CHANGE_JOB_EMPRESS:			return CXSLUnit::UC_EVE_CODE_EMPRESS;
		//}}
		//{{ 2011. 01. 26	최육사	청 1차 전직
	case CI_CHANGE_JOB_FURY_GUARDIAN:	return CXSLUnit::UC_CHUNG_FURY_GUARDIAN;
	case CI_CHANGE_JOB_SHOOTER_GUARDIAN:return CXSLUnit::UC_CHUNG_SHOOTING_GUARDIAN;
#ifdef SERV_ADD_CHUNG_SHELLING_GUARDIAN
	case CI_CHANGE_JOB_SHELLING_GUARDIAN:	return CXSLUnit::UC_CHUNG_SHELLING_GUARDIAN;
#endif
		//}}
		//{{ JHKang / 강정훈 / 2011.6.13 / 청 2차 전직
	case CI_CHANGE_JOB_IRON_PALADIN:	return CXSLUnit::UC_CHUNG_IRON_PALADIN;
	case CI_CHANGE_JOB_DEADLY_CHASER:	return CXSLUnit::UC_CHUNG_DEADLY_CHASER;
		//}}

#ifdef SERV_ELSWORD_INFINITY_SWORD
	case CI_CHANGE_JOB_INFINITY_SWORD:	return CXSLUnit::UC_ELSWORD_INFINITY_SWORD;
#endif
#ifdef SERV_ARME_DIMENSION_WITCH
	case CI_CHANGE_JOB_DIMENSION_WITCH:	return CXSLUnit::UC_ARME_DIMENSION_WITCH;
#endif
#ifdef SERV_RENA_NIGHT_WATCHER
	case CI_CHANGE_JOB_NIGHT_WATCHER:	return CXSLUnit::UC_LIRE_NIGHT_WATCHER;
#endif
#ifdef SERV_RAVEN_VETERAN_COMMANDER
	case CI_CHANGE_JOB_VETERAN_COMMANDER:	return CXSLUnit::UC_RAVEN_VETERAN_COMMANDER;
#endif
#ifdef SERV_EVE_BATTLE_SERAPH
	case CI_CHANGE_JOB_BATTLE_SERAPH:	return CXSLUnit::UC_EVE_BATTLE_SERAPH;
#endif
#ifdef SERV_CHUNG_TACTICAL_TROOPER
	case CI_CHANGE_JOB_TACTICAL_TROOPER:	return CXSLUnit::UC_CHUNG_TACTICAL_TROOPER;
#endif
#ifdef SERV_ARA_CHANGE_CLASS_FIRST
	case CI_CHANGE_JOB_LITTLE_HSIEN:	return CXSLUnit::UC_ARA_LITTLE_HSIEN;		/// 소선
	case CI_CHANGE_JOB_SAKRA_DEVANAM:	return CXSLUnit::UC_ARA_SAKRA_DEVANAM;		/// 제천
#endif //SERV_ARA_CHANGE_CLASS_FIRST
#ifdef SERV_NEW_CHARACTER_EL
	case CI_CHANGE_JOB_SABER_KNIGHT:	return CXSLUnit::UC_ELESIS_SABER_KNIGHT;	/// 세이버 나이트
	case CI_CHANGE_JOB_PYRO_KNIGHT:		return CXSLUnit::UC_ELESIS_PYRO_KNIGHT;		/// 파이로 나이트
#endif //SERV_NEW_CHARACTER_EL
#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
	case CI_CHANGE_JOB_LITTLE_DEVIL:	return CXSLUnit::UC_ARA_LITTLE_DEVIL;		/// 소마
	case CI_CHANGE_JOB_YAMA_RAJA:			return CXSLUnit::UC_ARA_YAMA_RAJA;		/// 명왕
#endif // SERV_ARA_CHANGE_CLASS_SECOND
#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE //김창한
	case CI_CHANGE_JOB_GRAND_MASTER:	return CXSLUnit::UC_ELESIS_GRAND_MASTER;	/// 그렌드 마스터
	case CI_CHANGE_JOB_BLAZING_HEART:	return CXSLUnit::UC_ELESIS_BLAZING_HEART;	/// 블레이징 하트
#endif //SERV_ELESIS_SECOND_CLASS_CHANGE
#ifdef SERV_9TH_NEW_CHARACTER // 김태환 ( 캐릭터 추가용 )
	case CI_CHANGE_JOB_PSYCHIC_TRACER:	return CXSLUnit::UC_ADD_PSYCHIC_TRACER;		/// 사이킥 트레이서
#endif //SERV_9TH_NEW_CHARACTER
#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
	case CI_CHANGE_JOB_LUNATIC_PSYKER:	return CXSLUnit::UC_ADD_LUNATIC_PSYKER;		/// 루나틱 사이커
#endif //SERV_ADD_LUNATIC_PSYKER
	}

	return CXSLUnit::UC_NONE;
}

int CXSLItem::GetCashItemByUnitClass( const char cUnitClass )
{
	switch( cUnitClass )
	{
	case CXSLUnit::UC_ELSWORD_KNIGHT:		return CI_CHANGE_JOB_ELSWORD_KNIGHT;
	case CXSLUnit::UC_ELSWORD_MAGIC_KNIGHT:	return CI_CHANGE_JOB_MAGIC_KNIGHT;
#ifdef SERV_ELSWORD_SHEATH_KNIGHT
	case CXSLUnit::UC_ELSWORD_SHEATH_KNIGHT:return CI_CHANGE_JOB_SHEATH_KNIGHT;
#endif SERV_ELSWORD_SHEATH_KNIGHT
		//{{ 2009. 10. 24  최육사	엘소드2차전직
	case CXSLUnit::UC_ELSWORD_LORD_KNIGHT:	return CI_CHANGE_JOB_LORD_KNIGHT;
	case CXSLUnit::UC_ELSWORD_RUNE_SLAYER:	return CI_CHANGE_JOB_RUNE_SLAYER;
		//}}
	case CXSLUnit::UC_LIRE_COMBAT_RANGER:	return CI_CHANGE_JOB_COMBAT_RANGER;
	case CXSLUnit::UC_LIRE_SNIPING_RANGER:	return CI_CHANGE_JOB_SNIPING_RANGER;
#ifdef SERV_TRAPPING_RANGER_TEST
	case CXSLUnit::UC_LIRE_TRAPPING_RANGER:	return CI_CHANGE_JOB_TRAPPING_RANGER;
#endif SERV_TRAPPING_RANGER_TEST
		//{{ 2009. 11. 23  최육사	레나2차전직
	case CXSLUnit::UC_LIRE_WIND_SNEAKER:	return CI_CHANGE_JOB_WIND_SNEAKER;
	case CXSLUnit::UC_LIRE_GRAND_ARCHER:	return CI_CHANGE_JOB_GRAND_ARCHER;
		//}}
	case CXSLUnit::UC_ARME_HIGH_MAGICIAN:	return CI_CHANGE_JOB_HIGH_MAGICIAN;
	case CXSLUnit::UC_ARME_DARK_MAGICIAN:	return CI_CHANGE_JOB_DARK_MAGICIAN;
#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
	case CXSLUnit::UC_ARME_BATTLE_MAGICIAN:	return CI_CHANGE_JOB_BATTLE_MAGICIAN;
#endif SERV_ADD_ARME_BATTLE_MAGICIAN
		//{{ 2009. 12. 11  최육사	아이샤 2차 전직
	case CXSLUnit::UC_ARME_ELEMENTAL_MASTER:	return CI_CHANGE_JOB_ELMENTAL_MASTER;
	case CXSLUnit::UC_ARME_VOID_PRINCESS:	return CI_CHANGE_JOB_VOID_PRINCESS;
		//}}
	case CXSLUnit::UC_RAVEN_SOUL_TAKER:		return CI_CHANGE_JOB_SOUL_TAKER;
	case CXSLUnit::UC_RAVEN_OVER_TAKER:		return CI_CHANGE_JOB_OVER_TAKER;
#ifdef SERV_RAVEN_WEAPON_TAKER
	case CXSLUnit::UC_RAVEN_WEAPON_TAKER:	return CI_CHANGE_JOB_WEAPON_TAKER;
#endif SERV_RAVEN_WEAPON_TAKER
		//{{ 2010. 1. 20  손영준	레이븐 2차 전직
	case CXSLUnit::UC_RAVEN_BLADE_MASTER:	return CI_CHANGE_JOB_BLADE_MASTER;
	case CXSLUnit::UC_RAVEN_RECKLESS_FIST:	return CI_CHANGE_JOB_RECKLESS_FIST;
		//}}	
	case CXSLUnit::UC_EVE_EXOTIC_GEAR:		return CI_CHANGE_JOB_EXOTIC_GEAR;
	case CXSLUnit::UC_EVE_ARCHITECTURE:		return CI_CHANGE_JOB_ARCHITECTURE;
#ifdef SERV_EVE_ELECTRA
	case CXSLUnit::UC_EVE_ELECTRA:			return CI_CHANGE_JOB_EVE_ELECTRA;
#endif SERV_EVE_ELECTRA
		//{{ 2010. 1. 20  손영준	이브 2차 전직
	case CXSLUnit::UC_EVE_CODE_NEMESIS:		return CI_CHANGE_JOB_NEMESIS;
	case CXSLUnit::UC_EVE_CODE_EMPRESS:		return CI_CHANGE_JOB_EMPRESS;
		//}}	
		//{{ 2011. 01. 26	최육사	청 1차 전직
	case CXSLUnit::UC_CHUNG_FURY_GUARDIAN:	return CI_CHANGE_JOB_FURY_GUARDIAN;
	case CXSLUnit::UC_CHUNG_SHOOTING_GUARDIAN: return CI_CHANGE_JOB_SHOOTER_GUARDIAN;
		//}}
#ifdef SERV_ADD_CHUNG_SHELLING_GUARDIAN
	case CXSLUnit::UC_CHUNG_SHELLING_GUARDIAN:	return CI_CHANGE_JOB_SHELLING_GUARDIAN;
#endif
		//{{ JHKang / 강정훈 / 2011.6.13 / 청 2차 전직
	case CXSLUnit::UC_CHUNG_IRON_PALADIN:	return CI_CHANGE_JOB_IRON_PALADIN;
	case CXSLUnit::UC_CHUNG_DEADLY_CHASER:	return CI_CHANGE_JOB_DEADLY_CHASER;
		//}}

#ifdef SERV_ELSWORD_INFINITY_SWORD
	case CXSLUnit::UC_ELSWORD_INFINITY_SWORD:	return CI_CHANGE_JOB_INFINITY_SWORD;
#endif
#ifdef SERV_ARME_DIMENSION_WITCH
	case CXSLUnit::UC_ARME_DIMENSION_WITCH:		return CI_CHANGE_JOB_DIMENSION_WITCH;
#endif
#ifdef SERV_RENA_NIGHT_WATCHER
	case CXSLUnit::UC_LIRE_NIGHT_WATCHER:		return CI_CHANGE_JOB_NIGHT_WATCHER;
#endif
#ifdef SERV_RAVEN_VETERAN_COMMANDER
	case CXSLUnit::UC_RAVEN_VETERAN_COMMANDER:	return CI_CHANGE_JOB_VETERAN_COMMANDER;
#endif
#ifdef SERV_EVE_BATTLE_SERAPH
	case CXSLUnit::UC_EVE_BATTLE_SERAPH:	return CI_CHANGE_JOB_BATTLE_SERAPH;
#endif
#ifdef SERV_CHUNG_TACTICAL_TROOPER
	case CXSLUnit::UC_CHUNG_TACTICAL_TROOPER:	return CI_CHANGE_JOB_TACTICAL_TROOPER;
#endif
#ifdef SERV_ARA_CHANGE_CLASS_FIRST
	case CXSLUnit::UC_ARA_LITTLE_HSIEN: return CI_CHANGE_JOB_LITTLE_HSIEN;				/// 소선
	case CXSLUnit::UC_ARA_SAKRA_DEVANAM: return CI_CHANGE_JOB_SAKRA_DEVANAM;			/// 제천
#endif //SERV_ARA_CHANGE_CLASS_FIRST
#ifdef SERV_NEW_CHARACTER_EL
	case CXSLUnit::UC_ELESIS_SABER_KNIGHT:		return CI_CHANGE_JOB_SABER_KNIGHT;		/// 세이버 나이트
	case CXSLUnit::UC_ELESIS_PYRO_KNIGHT:		return CI_CHANGE_JOB_PYRO_KNIGHT;		/// 파이로 나이트
#endif //SERV_NEW_CHARACTER_EL
#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
	case CXSLUnit::UC_ARA_LITTLE_DEVIL:			return CI_CHANGE_JOB_LITTLE_DEVIL;		/// 소마
	case CXSLUnit::UC_ARA_YAMA_RAJA:			return CI_CHANGE_JOB_YAMA_RAJA;			/// 명왕
#endif // SERV_ARA_CHANGE_CLASS_SECOND
#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE //김창한
	case CXSLUnit::UC_ELESIS_GRAND_MASTER:		return CI_CHANGE_JOB_GRAND_MASTER;		/// 그랜드 마스터
	case CXSLUnit::UC_ELESIS_BLAZING_HEART:		return CI_CHANGE_JOB_BLAZING_HEART;		/// 블레이징 하트
#endif //SERV_ELESIS_SECOND_CLASS_CHANGE
#ifdef SERV_9TH_NEW_CHARACTER // 김태환 ( 캐릭터 추가용 )
	case CXSLUnit::UC_ADD_PSYCHIC_TRACER:		return CI_CHANGE_JOB_PSYCHIC_TRACER;	/// 애드
#endif //SERV_9TH_NEW_CHARACTER
#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
	case CXSLUnit::UC_ADD_LUNATIC_PSYKER:		return CI_CHANGE_JOB_LUNATIC_PSYKER;	/// 애드
#endif //SERV_ADD_LUNATIC_PSYKER
	}

	return 0;
}
//}}


//{{ 2010. 04. 26  최육사	PC방 전용 캐쉬템
bool CXSLItem::IsPcBangOnlyCashItem( const int iItemID )
{
	switch( iItemID )
	{
#ifdef SERV_NET_CAFE_CASHSHOP
	case 70007272: // 로열 윙(레어)
	case 70007277: // 가열기 2개 큐브 100엔
#else //SERV_NET_CAFE_CASHSHOP
	case 221560: // PC방 할인 판매[얼음 조각상 가열기]
	case 221570: // PC방 할인 판매[뽀루의 발도장(보라색))
	case 221580: // PC방 할인 판매[홍조]
	case 221590: // PC방 할인 판매[여우 꼬리]		
#endif //SERV_NET_CAFE_CASHSHOP
		return true;
	}

	return false;
}
//}}

#ifdef SERV_KEEP_ITEM_SHOW_CASHSHOP
bool CXSLItem::IsKeepItemShowItem( const int iItemID )
{
	switch( iItemID )
	{
		case EI_SERVICE_COUPON_TYPE_A_PACKAGE:	 // 맞춤 서비스 쿠폰 Type A 패키지 
		case EI_SERVICE_COUPON_TYPE_B_PACKAGE: 	 // 맞춤 서비스 쿠폰 Type B 패키지
		case EI_SERVICE_COUPON_TYPE_C_PACKAGE: 	 // 맞춤 서비스 쿠폰 Type C 패키지
			return true;
	}

	return false;
}

bool CXSLItem::IsKeepItemShowItem2( const int iItemID )
{
	switch( iItemID )
	{
	case EI_ARA_SPECIAL_ATTRACTION_PACKAGE:		// 아라 스페셜 가열기 패키지
	case EI_ARA_SPECIAL_AVATAR1_PACKAGE:		// 아라 스페셜 아바타 패키지1
	case EI_ARA_SPECIAL_AVATAR2_PACKAGE:		// 아라 스페셜 아바타 패키지2
	case EI_ARA_SPECIAL_PET_PACKAGE:			// 아라 스페셜 펫 패키지
		return true;
	}

	return false;
}

bool CXSLItem::IsKeepItemShowItem3( const int iItemID )
{
	switch( iItemID )
	{
	case EI_SPECIAL_PRESENT_COPPER_KEY:
		return true;
	}

	return false;
}

bool CXSLItem::IsKeepItemShowItem4( const int iItemID )
{
	switch( iItemID )
	{
	case EI_SPECIAL_PRESENT_SILVER_KEY:
		return true;
	}

	return false;
}

bool CXSLItem::IsKeepItemShowItem5( const int iItemID )
{
	switch( iItemID )
	{
	case EI_SPECIAL_PRESENT_GOLD_KEY:
		return true;
	}

	return false;
}

bool CXSLItem::IsKeepItemShowItem6( const int iItemID )
{
	switch( iItemID )
	{
	case EI_SPECIAL_HEAVENLY_BOSS_RARE:
		return true;
	}

	return false;
}
#endif //SERV_KEEP_ITEM_SHOW_CASHSHOP

#ifndef DELETE_EXCEPTION_WISHLIST_CHANGEJOBITEM //캐시샾 찜하기에서 전직 아이템에 대해 예외처리 해제.
//{{ 캐시샵 찜하기 문제 임시 처리
//{{ 2010. 03. 30  김정협 전직아이템인지 확인하는 함수
bool CXSLItem::IsChangeJobItem( const int iItemID )
{
	switch( iItemID )
	{
		// 1차 전직
	case CI_CHANGE_JOB_ELSWORD_KNIGHT:
	case CI_CHANGE_JOB_MAGIC_KNIGHT:
	case CI_CHANGE_JOB_HIGH_MAGICIAN:
	case CI_CHANGE_JOB_DARK_MAGICIAN:
	case CI_CHANGE_JOB_COMBAT_RANGER:
	case CI_CHANGE_JOB_SNIPING_RANGER:
	case CI_CHANGE_JOB_SOUL_TAKER:
	case CI_CHANGE_JOB_OVER_TAKER:
	case CI_CHANGE_JOB_EXOTIC_GEAR:
	case CI_CHANGE_JOB_ARCHITECTURE:
		//{{ kimhc // 2011.1.3 // 청 1차 전직
	case CI_CHANGE_JOB_FURY_GUARDIAN:
	case CI_CHANGE_JOB_SHOOTER_GUARDIAN:
		//}} kimhc // 2011.1.3 // 청 1차 전직

		//{{ 트랜스폼 1차
#ifdef SERV_ELSWORD_SHEATH_KNIGHT
	case CI_CHANGE_JOB_SHEATH_KNIGHT:
#endif SERV_ELSWORD_SHEATH_KNIGHT
#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
	case CI_CHANGE_JOB_BATTLE_MAGICIAN:
#endif SERV_ADD_ARME_BATTLE_MAGICIAN
#ifdef SERV_TRAPPING_RANGER_TEST
	case CI_CHANGE_JOB_TRAPPING_RANGER:
#endif SERV_TRAPPING_RANGER_TEST
#ifdef SERV_RAVEN_WEAPON_TAKER
	case CI_CHANGE_JOB_WEAPON_TAKER:
#endif SERV_RAVEN_WEAPON_TAKER
#ifdef SERV_EVE_ELECTRA
	case CI_CHANGE_JOB_EVE_ELECTRA:
#endif SERV_EVE_ELECTRA
#ifdef SERV_ADD_CHUNG_SHELLING_GUARDIAN
	case CI_CHANGE_JOB_SHELLING_GUARDIAN:
#endif
		//}} 트랜스폼 1차

		//{{ 소선
#ifdef SERV_ARA_CHANGE_CLASS_FIRST
	case CI_CHANGE_JOB_LITTLE_HSIEN:
#endif //SERV_ARA_CHANGE_CLASS_FIRST
		//}} 소선

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
	case CI_CHANGE_JOB_LITTLE_DEVIL:
#endif // SERV_ARA_CHANGE_CLASS_SECOND

		//{{ 엘리1차
#ifdef SERV_NEW_CHARACTER_EL
	case CI_CHANGE_JOB_SABER_KNIGHT:
	case CI_CHANGE_JOB_PYRO_KNIGHT:
#endif //SERV_NEW_CHARACTER_EL
		//}} 엘리1차
		
#ifdef SERV_9TH_NEW_CHARACTER	// 해외팀 추가
	case CI_CHANGE_JOB_PSYCHIC_TRACER:	// 애드 1차 전직 사이킥 트레이
#endif SERV_9TH_NEW_CHARACTER	// 해외팀 추가

		// 2차 전직
	case CI_CHANGE_JOB_LORD_KNIGHT:
	case CI_CHANGE_JOB_RUNE_SLAYER:
	case CI_CHANGE_JOB_WIND_SNEAKER:
	case CI_CHANGE_JOB_GRAND_ARCHER:
	case CI_CHANGE_JOB_ELMENTAL_MASTER:
	case CI_CHANGE_JOB_VOID_PRINCESS:
	case CI_CHANGE_JOB_BLADE_MASTER:
	case CI_CHANGE_JOB_RECKLESS_FIST:
	case CI_CHANGE_JOB_NEMESIS:
	case CI_CHANGE_JOB_EMPRESS:
//{{ JHKang / 강정훈 / 2011.6.13 / 청 2차 전직
	case CI_CHANGE_JOB_IRON_PALADIN:
	case CI_CHANGE_JOB_DEADLY_CHASER:
//}}
#ifdef SERV_ELSWORD_INFINITY_SWORD
	case CI_CHANGE_JOB_INFINITY_SWORD:
#endif
#ifdef SERV_ARME_DIMENSION_WITCH
	case CI_CHANGE_JOB_DIMENSION_WITCH:
#endif
#ifdef SERV_RENA_NIGHT_WATCHER
	case CI_CHANGE_JOB_NIGHT_WATCHER:
#endif
#ifdef SERV_RAVEN_VETERAN_COMMANDER
	case CI_CHANGE_JOB_VETERAN_COMMANDER:
#endif
#ifdef SERV_EVE_BATTLE_SERAPH
	case CI_CHANGE_JOB_BATTLE_SERAPH:
#endif
#ifdef SERV_CHUNG_TACTICAL_TROOPER
	case CI_CHANGE_JOB_TACTICAL_TROOPER:
#endif
		//{{ 제천
#ifdef SERV_ARA_CHANGE_CLASS_FIRST
	case CI_CHANGE_JOB_SAKRA_DEVANAM:
#endif //SERV_ARA_CHANGE_CLASS_FIRST
		//}} 제천
#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
	case CI_CHANGE_JOB_YAMA_RAJA:
#endif // SERV_ARA_CHANGE_CLASS_SECOND
#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE //김창한
	case CI_CHANGE_JOB_GRAND_MASTER:
	case CI_CHANGE_JOB_BLAZING_HEART:
#endif //SERV_ELESIS_SECOND_CLASS_CHANGE
#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
	case CI_CHANGE_JOB_LUNATIC_PSYKER:
#endif //SERV_ADD_LUNATIC_PSYKER
		return true;
	}

	return false;
}
#endif //DELETE_EXCEPTION_WISHLIST_CHANGEJOBITEM

bool CXSLItem::IsBankExpandItem( const int iItemID )
{
	switch( iItemID )
	{
	case CI_BANK_MEMBERSHIP_SILVER:
	case CI_BANK_MEMBERSHIP_GOLD:
	case CI_BANK_MEMBERSHIP_EMERALD:
	case CI_BANK_MEMBERSHIP_DIAMOND:
	case CI_BANK_MEMBERSHIP_PLATINUM:
	case CI_BANK_MEMBERSHIP_ONE_PACKAGE:
		//{{ 2011. 12. 13  김민성	퀵 슬롯 확장 아이템
#ifdef SERV_EXPAND_QUICK_SLOT
	case CI_EXPAND_QUICK_SLOT:
	case CI_EXPAND_QUICK_SLOT_ARA:
#endif SERV_EXPAND_QUICK_SLOT
		//}}
#ifdef SERV_SHARING_BANK_QUEST_CASH
	case CI_CASH_SHARING_BACK_OPEN:
#endif SERV_SHARING_BANK_QUEST_CASH
#ifdef SERV_CASH_ITEM_FOR_ELESIS	// 적용날짜: 2013-07-11
	case CI_EXPAND_QUICK_SLOT_ELESIS:
#endif	// SERV_CASH_ITEM_FOR_ELESIS
		return true;
	}
    
	return false;
}
//}}

//{{ 2011. 05. 11	최육사	대리상인
#ifdef SERV_PSHOP_AGENCY
int CXSLItem::GetPShopAgencyDays( IN const int iItemID )
{
	switch( iItemID )
	{
	case CXSLItem::SI_PSHOP_AGENCY_1_DAY:	return 1;
	case CXSLItem::SI_PSHOP_AGENCY_2_DAYS:	return 2;
	case CXSLItem::SI_PSHOP_AGENCY_3_DAYS:	return 3;
	case CXSLItem::SI_PSHOP_AGENCY_5_DAYS:	return 5;
	case CXSLItem::SI_PSHOP_AGENCY_7_DAYS:	return 7;
	case CXSLItem::SI_PSHOP_AGENCY_10_DAYS:	return 10;
	case CXSLItem::SI_PSHOP_AGENCY_15_DAYS: return 15;
	case CXSLItem::SI_PSHOP_AGENCY_30_DAYS:	return 30;
    case CXSLItem::SI_PSHOP_AGENCY_7_DAYS_ED: return 7;
    case CXSLItem::SI_PSHOP_AGENCY_15_DAYS_ED: return 15;
    case CXSLItem::SI_PSHOP_AGENCY_30_DAYS_ED: return 30;
	default: return 0;
	}
	
    return 0;
}

int CXSLItem::GetPShopAgencyType( IN const int iItemID_ )
{
    int ret = 0;
    switch( iItemID_ )
    {
    case CXSLItem::SI_PSHOP_AGENCY_1_DAY:	
    case CXSLItem::SI_PSHOP_AGENCY_2_DAYS:	
    case CXSLItem::SI_PSHOP_AGENCY_3_DAYS:	
    case CXSLItem::SI_PSHOP_AGENCY_5_DAYS:	
    case CXSLItem::SI_PSHOP_AGENCY_7_DAYS:	
    case CXSLItem::SI_PSHOP_AGENCY_10_DAYS:	
    case CXSLItem::SI_PSHOP_AGENCY_15_DAYS: 
    case CXSLItem::SI_PSHOP_AGENCY_30_DAYS:	
#ifdef SERV_UPGRADE_TRADE_SYSTEM
        ret = SEnum::AST_PREMIUM;
#else //SERV_UPGRADE_TRADE_SYSTEM
		ret = CXSLSquareUnit::PST_PREMIUM;
#endif //SERV_UPGRADE_TRADE_SYSTEM
        break;
    case CXSLItem::SI_PSHOP_AGENCY_7_DAYS_ED:
    case CXSLItem::SI_PSHOP_AGENCY_15_DAYS_ED:
    case CXSLItem::SI_PSHOP_AGENCY_30_DAYS_ED:
#ifdef SERV_UPGRADE_TRADE_SYSTEM
        ret = SEnum::AST_NORMAL;
#else //SERV_UPGRADE_TRADE_SYSTEM
		ret = CXSLSquareUnit::PST_NORMAL;
#endif //SERV_UPGRADE_TRADE_SYSTEM
        break;

    default: 
        ret = 0;
    }

    return ret;
}

#endif SERV_PSHOP_AGENCY
//}}

//{{ 2011. 06. 01	최육사	강화된 아이템 보상 큐브
#ifdef SERV_ENCHANTED_ITEM_CUBE_REWARD
//{{ 2013. 05. 15	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
bool CXSLItem::UpdateEnchantedItemCubeReward( IN const int iCubeItemID, IN OUT KItemInfo& kInsertItemInfo )
#else
void CXSLItem::UpdateEnchantedItemCubeReward( IN const int iCubeItemID, IN OUT KItemInfo& kInsertItemInfo )
#endif SERV_NEW_ITEM_SYSTEM_2013_05
//}}
{
	//{{ 2012. 05. 23	김민성	[이벤트] 에이핑크가 응원합니다!
//#ifdef SERV_EVENT_APINK
	const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( kInsertItemInfo.m_iItemID );
	if( pItemTemplet == NULL )
	{
		START_LOG( cerr, L"아이템 정보 가 없습니다." )
			<< BUILD_LOG( iCubeItemID )
			<< BUILD_LOG( kInsertItemInfo.m_iItemID )
			<< END_LOG;

		//{{ 2013. 05. 15	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
		return false;
#endif SERV_NEW_ITEM_SYSTEM_2013_05
		//}}
	}
//#endif SERV_EVENT_APINK
	//}}

	switch( iCubeItemID )
	{
#ifdef SERV_GLOBAL_EVENT_ITEM
	case 60007610: // 10강 무기 큐브 Lv.20
	case 60007611: // 10강 무기 큐브 Lv.32
	case 60007612: // 10강 무기 큐브 Lv.44
	case 60007613: // 10강 무기 큐브 Lv.52
	case 60007614: // 10강 무기 큐브 Lv.60
		{
			kInsertItemInfo.m_cEnchantLevel = 10;
		}
		break;
	case 67003610: // 용맹한 마도 대전 용사 큐브 1개
		{
			kInsertItemInfo.m_cEnchantLevel = 6;
		}
		break;
#endif //SERV_GLOBAL_EVENT_ITEM
	case 160134: // 숲의 수호자 무기 큐브 1개
		{
			kInsertItemInfo.m_cEnchantLevel = 6;
			kInsertItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 = CXSLAttribEnchantItem::ET_WATER;
		}
		break;
	case 160135:	// 시민의 수호자 무기 큐브 1개
		{
			kInsertItemInfo.m_cEnchantLevel = 6;
			kInsertItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 = CXSLAttribEnchantItem::ET_LIGHT;
		}
		break;
	case 160136: // 파멸의 용병 무기 큐브 1개
		{
			kInsertItemInfo.m_cEnchantLevel = 6;
			kInsertItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 = CXSLAttribEnchantItem::ET_DARK;
		}
		break;
	case 160137: // 분노의 화신 무기 큐브 1개
		{
			kInsertItemInfo.m_cEnchantLevel = 6;
			kInsertItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 = CXSLAttribEnchantItem::ET_BLAZE;
		}
		break;
	case 160138: // 문명의 제압자 무기 큐브 1개
		{
			kInsertItemInfo.m_cEnchantLevel = 6;
			kInsertItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 = CXSLAttribEnchantItem::ET_WIND;
		}
		break;
	case 160139: // 마도대전 용사의 무기 큐브 1개
		{
			kInsertItemInfo.m_cEnchantLevel = 6;
			kInsertItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 = CXSLAttribEnchantItem::ET_DARK;
		}
		break;
	case 160140: // 블랙스미스 연합 무기 큐브 1개
		{
			kInsertItemInfo.m_cEnchantLevel = 6;
			kInsertItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 = CXSLAttribEnchantItem::ET_LIGHT;
		}
		break;
	case 160141: // 혈투의 노래 무기 큐브 1개
		{
			kInsertItemInfo.m_cEnchantLevel = 6;
			kInsertItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 = CXSLAttribEnchantItem::ET_BLAZE;
		}
		break;
	case 160142: // 빛의 수호자 무기 큐브 1개
		{
			kInsertItemInfo.m_cEnchantLevel = 6;
			kInsertItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 = CXSLAttribEnchantItem::ET_WIND;
		}
		break;
		////////////////////////////////////////////////////////////////////////// 추가 : 2011년 9월 14일
	case 160276: // 용감한 숲의 수호자 무기 큐브(엘리트)
		{
			kInsertItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 = CXSLAttribEnchantItem::ET_WATER;
		}
		break;
	case 160277: // 정의로운 시민의 수호자 무기 큐브(엘리트)
		{
			kInsertItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 = CXSLAttribEnchantItem::ET_LIGHT;
		}
		break;
	case 160278: // 분노한 파멸의 용병 무기 큐브(엘리트)
		{
			kInsertItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 = CXSLAttribEnchantItem::ET_DARK;
		}
		break;
	case 160279: // 매서운 분노의 화신 무기 큐브(엘리트)
		{
			kInsertItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 = CXSLAttribEnchantItem::ET_BLAZE;
		}
		break;
	case 160280: // 찬란한 문명의 제압자 무기 큐브(엘리트)
		{
			kInsertItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 = CXSLAttribEnchantItem::ET_WIND;
		}
		break;
	case 160281: // 용맹한 마도대전 용사의 무기 큐브(엘리트)
		{
			kInsertItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 = CXSLAttribEnchantItem::ET_DARK;
		}
		break;
	case 160282: // 비밀스런 블랙스미스 연합 무기 큐브(엘리트)
		{
			kInsertItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 = CXSLAttribEnchantItem::ET_LIGHT;
		}
		break;
	case 160283: // 피끓는 혈투의 노래 무기 큐브(엘리트)
		{
			kInsertItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 = CXSLAttribEnchantItem::ET_BLAZE;
		}
		break;
	case 160284: // 2차 전직 엘리트 무기 큐브
		{
			kInsertItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 = CXSLAttribEnchantItem::ET_WATER;
		}
		break;
		////////////////////////////////////////////////////////////////////////// 추가 : 2011년 9월 14일

		////////////////////////////////////////////////////////////////////////// 추가 : 2012년 3월 28일
	case 160424: // +6 용감한 숲의 수호자 풀세트 큐브
		{
			kInsertItemInfo.m_cEnchantLevel = 6;
		}
		break;
	case 160425: // +6 정의로운 시민의 수호자 풀세트 큐브
		{
			kInsertItemInfo.m_cEnchantLevel = 6;
		}
		break;
	case 160426: // +6 분노한 파멸의 용병 풀세트 큐브
		{
			kInsertItemInfo.m_cEnchantLevel = 6;
		}
		break;
	case 160427: // +6 매서운 분노의 화신 풀세트 큐브
		{
			kInsertItemInfo.m_cEnchantLevel = 6;
		}
		break;
	case 160428: // +6 찬란한 문명의 제압자 풀세트 큐브
		{
			kInsertItemInfo.m_cEnchantLevel = 6;
		}
		break;
	case 160429: // +6 용맹한 마도대전 용사 풀세트 큐브
		{
			kInsertItemInfo.m_cEnchantLevel = 6;
		}
		break;
	case 160430: // +6 블랙스미스 연합 풀세트 큐브
		{
			kInsertItemInfo.m_cEnchantLevel = 6;
		}
		break;
	case 160431: // +6 피끓는 혈투의 노래 풀세트 큐브
		{
			kInsertItemInfo.m_cEnchantLevel = 6;
		}
		break;
	case 160432: // +6 엘리오스의 영웅 풀세트 큐브
		{
			kInsertItemInfo.m_cEnchantLevel = 6;
		}
		break;
	case 160433: // +6 광폭한 지배자 풀세트 큐브
		{
			kInsertItemInfo.m_cEnchantLevel = 6;
		}
		break;
	case 160434: // +6 군단 지휘관 풀세트 큐브
		{
			kInsertItemInfo.m_cEnchantLevel = 6;
		}
		break;
		////////////////////////////////////////////////////////////////////////// 추가 : 2012년 3월 28일

	case 135098: // +10강화 방어구 풀세트 큐브
		{
			kInsertItemInfo.m_cEnchantLevel = 10;
		}
		break;
	case 161048: // +6강화 [코보]도전의 무기 상자
		{
			kInsertItemInfo.m_cEnchantLevel = 6;
		} break;
		//{{ 2012. 05. 23	김민성	[이벤트] 에이핑크가 응원합니다!
//#ifdef SERV_EVENT_APINK
	case 181456:   // +6 에이핑크의 수호자 풀세트 큐브
		{
			if( pItemTemplet->m_ItemType == CXSLItem::IT_WEAPON )
			{
				kInsertItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 = CXSLAttribEnchantItem::ET_BLAZE;
			}
			kInsertItemInfo.m_cEnchantLevel = 6;
		}
		break;
	case 181457:   // +6 정의로운 에이핑크 풀세트 큐브
		{
			if( pItemTemplet->m_ItemType == CXSLItem::IT_WEAPON )
			{
				kInsertItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 = CXSLAttribEnchantItem::ET_WATER;
			}
			kInsertItemInfo.m_cEnchantLevel = 6;
		}
		break;
	case 181458:   // +6 분노한 에이핑크의 풀세트 큐브
		{
			if( pItemTemplet->m_ItemType == CXSLItem::IT_WEAPON )
			{
				kInsertItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 = CXSLAttribEnchantItem::ET_DARK;
			}
			kInsertItemInfo.m_cEnchantLevel = 6;
		}
		break;
	case 181459:   // +6 사랑의 아이돌 에이핑크 풀세트 큐브
		{
			if( pItemTemplet->m_ItemType == CXSLItem::IT_WEAPON )
			{
				kInsertItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 = CXSLAttribEnchantItem::ET_BLAZE;
			}
			kInsertItemInfo.m_cEnchantLevel = 6;
		}
		break;
	case 181460:   // +6 나를 제압한 에이핑크 풀세트 큐브
		{
			if( pItemTemplet->m_ItemType == CXSLItem::IT_WEAPON )
			{
				kInsertItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 = CXSLAttribEnchantItem::ET_WATER;
			}
			kInsertItemInfo.m_cEnchantLevel = 6;
		}
		break;
	case 181461:   // +6 용감한 에이핑크 용사 풀세트 큐브
		{
			if( pItemTemplet->m_ItemType == CXSLItem::IT_WEAPON )
			{
				kInsertItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 = CXSLAttribEnchantItem::ET_NATURE;
			}
			kInsertItemInfo.m_cEnchantLevel = 6;
		}
		break;
	case 181462:   // +6 에이핑크 연합 풀세트 큐브
		{
			if( pItemTemplet->m_ItemType == CXSLItem::IT_WEAPON )
			{
				kInsertItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 = CXSLAttribEnchantItem::ET_DARK;
			}
			kInsertItemInfo.m_cEnchantLevel = 6;
		}
		break;
	case 181463:   // +6 감성 아이돌 에이핑크 풀세트 큐브
		{
			if( pItemTemplet->m_ItemType == CXSLItem::IT_WEAPON )
			{
				kInsertItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 = CXSLAttribEnchantItem::ET_WATER;
			}
			kInsertItemInfo.m_cEnchantLevel = 6;
		}
		break;
	case 181464:   // +6 에이핑크의 영웅 풀세트 큐브
		{
			if( pItemTemplet->m_ItemType == CXSLItem::IT_WEAPON )
			{
				kInsertItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 = CXSLAttribEnchantItem::ET_DARK;
			}
			kInsertItemInfo.m_cEnchantLevel = 6;
		}
		break;
	case 181465:   // +6 에이핑크가 좋아한 풀세트 큐브
		{
			if( pItemTemplet->m_ItemType == CXSLItem::IT_WEAPON )
			{
				kInsertItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 = CXSLAttribEnchantItem::ET_WATER;
			}
			kInsertItemInfo.m_cEnchantLevel = 6;
		}
		break;
	case 181466:   // +6 에이핑크를 사랑한 용사 풀세트 큐브
		{
			if( pItemTemplet->m_ItemType == CXSLItem::IT_WEAPON )
			{
				kInsertItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 = CXSLAttribEnchantItem::ET_DARK;
			}
			kInsertItemInfo.m_cEnchantLevel = 6;
		}
		break;
//#endif SERV_EVENT_APINK
		//}}

	case 160619:	// +6 천년 여우 수호자 풀세트 큐브
	case 160732:	// 엘리오스 견습 모험가의 풀세트 큐브
		{
			if( pItemTemplet->m_ItemType == CXSLItem::IT_WEAPON )
			{
				kInsertItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 = CXSLAttribEnchantItem::ET_BLAZE;
			}
			kInsertItemInfo.m_cEnchantLevel = 6;
		}
		break;
	case 160620:	// +6 봉인된 천년 여우 풀세트 큐브
	case 160733:	// 엘리오스 초보 모험가의 풀세트 큐브
		{
			if( pItemTemplet->m_ItemType == CXSLItem::IT_WEAPON )
			{
				kInsertItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 = CXSLAttribEnchantItem::ET_WATER;
			}
			kInsertItemInfo.m_cEnchantLevel = 6;
		}
		break;
	case 160621:	// +6 분노한 천년 여우 풀세트 큐브
	case 160734:	// 엘리오스 초급 모험가의 풀세트 큐브
		{
			if( pItemTemplet->m_ItemType == CXSLItem::IT_WEAPON )
			{
				kInsertItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 = CXSLAttribEnchantItem::ET_DARK;
			}
			kInsertItemInfo.m_cEnchantLevel = 6;
		}
		break;
	case 160622:	// +6 불가사의한 천년 여우 풀세트 큐브
	case 160735:	// 엘리오스 중급 모험가의 풀세트 큐브
		{
			if( pItemTemplet->m_ItemType == CXSLItem::IT_WEAPON )
			{
				kInsertItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 = CXSLAttribEnchantItem::ET_BLAZE;
			}
			kInsertItemInfo.m_cEnchantLevel = 6;
		}
		break;
	case 160623:	// +6 나를 제압한 천년 여우 풀세트 큐브
	case 160736:	// 엘리오스 상급 모험가의 풀세트 큐브
		{
			if( pItemTemplet->m_ItemType == CXSLItem::IT_WEAPON )
			{
				kInsertItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 = CXSLAttribEnchantItem::ET_WATER;
			}
			kInsertItemInfo.m_cEnchantLevel = 6;
		}
		break;
	case 160624:	// +6 부활한 천년 여우 풀세트 큐브
	case 160737:	// 엘리오스 최상급 모험가의 풀세트 큐브
		{
			if( pItemTemplet->m_ItemType == CXSLItem::IT_WEAPON )
			{
				kInsertItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 = CXSLAttribEnchantItem::ET_NATURE;
			}
			kInsertItemInfo.m_cEnchantLevel = 6;
		}
		break;
	case 160625:	// +6 각성한 천년 여우 풀세트 큐브
	case 160738:	// 엘리오스 노련한 모험가의 풀세트 큐브
		{
			if( pItemTemplet->m_ItemType == CXSLItem::IT_WEAPON )
			{
				kInsertItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 = CXSLAttribEnchantItem::ET_DARK;
			}
			kInsertItemInfo.m_cEnchantLevel = 6;
		}
		break;
	case 160626:	// +6 얄미운 천년 여우 풀세트 큐브
	case 160739:	// 엘리오스 숙련된 모험가의 풀세트 큐브
		{
			if( pItemTemplet->m_ItemType == CXSLItem::IT_WEAPON )
			{
				kInsertItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 = CXSLAttribEnchantItem::ET_WATER;
			}
			kInsertItemInfo.m_cEnchantLevel = 6;
		}
		break;
	case 160627:	// +6 꼬리 치는 천년 여우 풀세트 큐브
	case 160740:	// 엘리오스 세련된 모험가의 풀세트 큐브
		{
			if( pItemTemplet->m_ItemType == CXSLItem::IT_WEAPON )
			{
				kInsertItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 = CXSLAttribEnchantItem::ET_DARK;
			}
			kInsertItemInfo.m_cEnchantLevel = 6;
		}
		break;
	case 160628:	// +6 질주하는 천년 여우 풀세트 큐브
	case 160741:	// 엘리오스 절정의 모험가 풀세트 큐브
		{
			if( pItemTemplet->m_ItemType == CXSLItem::IT_WEAPON )
			{
				kInsertItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 = CXSLAttribEnchantItem::ET_WATER;
			}
			kInsertItemInfo.m_cEnchantLevel = 6;
		}
		break;
	case 160629:    // +6 신이 된 천년 여우 풀세트 큐브
	case 160742:	// 엘리오스 완벽한 모험가 풀세트 큐브
		{
			if( pItemTemplet->m_ItemType == CXSLItem::IT_WEAPON )
			{
				kInsertItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 = CXSLAttribEnchantItem::ET_DARK;
			}
			kInsertItemInfo.m_cEnchantLevel = 6;
		}
		break;
	case 160641:   // 천년 여우의 힘 30레벨 10강화 무기 큐브
		{
			kInsertItemInfo.m_cEnchantLevel = 10;
		}
		break;

	case 8655943:   // +7 [코보] 해킹 지원 장비 큐브 (Lv.30)
	case 8655944:   // +7 [코보] 해킹 지원 장비 큐브 (Lv.40)
	case 8655945:   // +7 [코보] 해킹 지원 장비 큐브 (Lv.50)
	case 8655946:   // +7 [코보] 해킹 지원 장비 큐브 (Lv.60)
	case 8655947:   // +7 [코보] 해킹 지원 장비 큐브 (Lv.65)
		{
			if( pItemTemplet->m_ItemType == CXSLItem::IT_WEAPON )
			{
				kInsertItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 = CXSLAttribEnchantItem::ET_DARK;
				kInsertItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant1 = CXSLAttribEnchantItem::ET_DARK;
			}
			kInsertItemInfo.m_cEnchantLevel = 7;
		}
		break;
#ifdef SERV_EN_OPEN_ENCHANT_ITEM_EVENT
	case 85003011:   // 영국 상용화 각성한 모험가의 큐브
		{
			kInsertItemInfo.m_cEnchantLevel = 5;
		}
		break;
#endif //SERV_EN_OPEN_ENCHANT_ITEM_EVENT

#ifdef SERV_PLAY_TOGETHER_EVENT
	case 90003000:	// 신규 유저 지원 장비 큐브 (Lv.16)
	case 90003001:	// 신규 유저 지원 장비 큐브 (Lv.22)
	case 90003002:	// 신규 유저 지원 장비 큐브 (Lv.26)
	case 90003003:	// 신규 유저 지원 장비 큐브 (Lv.34)
		{
			kInsertItemInfo.m_cEnchantLevel = 6;
		}
		break;
#endif SERV_PLAY_TOGETHER_EVENT

#ifdef SERV_BR_ENCHANT_CUBE_EVENT
	case 152000450:
	case 152000451:
	case 152000452:
	case 152000453:
	case 152000454:
	case 152000455:
		{
			kInsertItemInfo.m_cEnchantLevel = 5;
		}
		break;
#endif //SERV_CHUNG_EVENT_WEAPON_ENCHANT

#ifdef SERV_NEW_YEAR_EVENT_2014
	case 153000102:	// 6강 장비 풀세트 큐브(22레벨)
		{
			kInsertItemInfo.m_cEnchantLevel = 6;
		}
		break;
#endif //SERV_NEW_YEAR_EVENT_2014

#ifdef SERV_EVENT_ENCHANT_LEVEL
	case 67003609:	// 6강 매서운 분노의 화신 풀세트 큐브
		{
			kInsertItemInfo.m_cEnchantLevel = 6;
		}
		break;
#endif SERV_EVENT_ENCHANT_LEVEL

#ifdef SERV_EVENT_CHUNG_GIVE_ITEM
	case 141000632:   // 각성한 여왕의 기어 큐브
		{
			kInsertItemInfo.m_cEnchantLevel = 6; //강화 레벨을 설정해 준다.
		}
		break;
#endif SERV_EVENT_CHUNG_GIVE_ITEM

		//{{ 2013. 05. 15	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
	default:
		{
			return false;
		}
#endif SERV_NEW_ITEM_SYSTEM_2013_05
		//}}
	}

	//{{ 2013. 05. 15	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
	return true;
#endif SERV_NEW_ITEM_SYSTEM_2013_05
	//}}
}
#endif SERV_ENCHANTED_ITEM_CUBE_REWARD
//}}

#ifdef	SERV_UPGRADE_SKILL_SYSTEM_2013 // 적용날짜: 2013-06-27
#else	// SERV_UPGRADE_SKILL_SYSTEM_2013
/*
//{{ 2011. 05. 27    김민성    휴면 복귀 유저 보상
#ifdef SERV_COME_BACK_USER_REWARD
int	CXSLItem::GetComeBackRewardSkillPointDays( IN const int iItemID )
{
	switch( iItemID )
	{
	case CXSLItem::SI_COME_BACK_REWARD_SKILL_5_POINT_15_DAY:	return 15;
	case CXSLItem::SI_COME_BACK_REWARD_SKILL_5_POINT_30_DAY:	return 30;
	case CXSLItem::SI_COME_BACK_REWARD_SKILL_5_POINT_60_DAY:	return 60;

	default: return 0;
	}

	return 0;
}
#endif SERV_COME_BACK_USER_REWARD
//}}
*/
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013

//{{ 2011. 11. 21  김민성	전직 변경 아이템
#ifdef SERV_UNIT_CLASS_CHANGE_ITEM
int CXSLItem::GetClassChangeCashItem( const int iItemID )
{
	switch( iItemID )
	{
////////// 엘소드 ////////////////////////////////////////////////////////////////////////
	case CI_CLASS_CHANGE_ELSWORD_KNIGHT:			return CXSLUnit::UC_ELSWORD_KNIGHT;
	case CI_CLASS_CHANGE_ELSWORD_LORD_KNIGHT:		return CXSLUnit::UC_ELSWORD_LORD_KNIGHT;

	case CI_CLASS_CHANGE_ELSWORD_MAGIC_KNIGHT:		return CXSLUnit::UC_ELSWORD_MAGIC_KNIGHT;
	case CI_CLASS_CHANGE_ELSWORD_RUNE_SLAYER:		return CXSLUnit::UC_ELSWORD_RUNE_SLAYER;

	case CI_CLASS_CHANGE_ELSWORD_SHEATH_KNIGHT:		return CXSLUnit::UC_ELSWORD_SHEATH_KNIGHT;
#ifdef SERV_ELSWORD_INFINITY_SWORD
	case CI_CLASS_CHANGE_ELSWORD_INFINITY_SWORD:	return CXSLUnit::UC_ELSWORD_INFINITY_SWORD;
#else
	case CI_CLASS_CHANGE_ELSWORD_SHEATH_KNIGHT_2:	return CXSLUnit::UC_ELSWORD_SHEATH_KNIGHT_2;
#endif
////////// 아이샤 ////////////////////////////////////////////////////////////////////////
	case CI_CLASS_CHANGE_AISHA_HIGH_MAGICIAN:		return CXSLUnit::UC_ARME_HIGH_MAGICIAN;
	case CI_CLASS_CHANGE_AISHA_ELMENTAL_MASTER:		return CXSLUnit::UC_ARME_ELEMENTAL_MASTER;

	case CI_CLASS_CHANGE_AISHA_DARK_MAGICIAN:		return CXSLUnit::UC_ARME_DARK_MAGICIAN;
	case CI_CLASS_CHANGE_AISHA_VOID_PRINCESS:		return CXSLUnit::UC_ARME_VOID_PRINCESS;

	case CI_CLASS_CHANGE_AISHA_BATTLE_MAGICIAN:		return CXSLUnit::UC_ARME_BATTLE_MAGICIAN;
#ifdef SERV_ARME_DIMENSION_WITCH
	case CI_CLASS_CHANGE_AISHA_DIMENSION_WITCH:		return CXSLUnit::UC_ARME_DIMENSION_WITCH;
#else
	case CI_CLASS_CHANGE_AISHA_BATTLE_MAGICIAN_2:	return CXSLUnit::UC_ARME_BATTLE_MAGICIAN_2;
#endif
////////// 레나 ////////////////////////////////////////////////////////////////////////
	case CI_CLASS_CHANGE_RENA_COMBAT_RANGER:		return CXSLUnit::UC_LIRE_COMBAT_RANGER;
	case CI_CLASS_CHANGE_RENA_WIND_SNEAKER:			return CXSLUnit::UC_LIRE_WIND_SNEAKER;

	case CI_CLASS_CHANGE_RENA_SNIPING_RANGER:		return CXSLUnit::UC_LIRE_SNIPING_RANGER;
	case CI_CLASS_CHANGE_RENA_GRAND_ARCHER:			return CXSLUnit::UC_LIRE_GRAND_ARCHER;

	case CI_CLASS_CHANGE_RENA_TRAPPING_RANGER:		return CXSLUnit::UC_LIRE_TRAPPING_RANGER;
#ifdef SERV_RENA_NIGHT_WATCHER
	case CI_CLASS_CHANGE_RENA_NIGHT_WATCHER:		return CXSLUnit::UC_LIRE_NIGHT_WATCHER;
#else
	case CI_CLASS_CHANGE_RENA_TRAPPING_RANGER_2:	return CXSLUnit::UC_LIRE_TRAPPING_RANGER_2;
#endif
////////// 레이븐 ////////////////////////////////////////////////////////////////////////
	case CI_CLASS_CHANGE_REVEN_SOUL_TAKER:			return CXSLUnit::UC_RAVEN_SOUL_TAKER;
	case CI_CLASS_CHANGE_REVEN_BLADE_MASTER:		return CXSLUnit::UC_RAVEN_BLADE_MASTER;

	case CI_CLASS_CHANGE_REVEN_OVER_TAKER:			return CXSLUnit::UC_RAVEN_OVER_TAKER;
	case CI_CLASS_CHANGE_REVEN_RECKLESS_FIST:		return CXSLUnit::UC_RAVEN_RECKLESS_FIST;

	case CI_CLASS_CHANGE_RAVEN_WEAPON_TAKER:		return CXSLUnit::UC_RAVEN_WEAPON_TAKER;
#ifdef SERV_RAVEN_VETERAN_COMMANDER
	case CI_CLASS_CHANGE_RAVEN_VETERAN_COMMANDER:	return CXSLUnit::UC_RAVEN_VETERAN_COMMANDER;
#else
	case CI_CLASS_CHANGE_RAVEN_WEAPON_TAKER_2:		return CXSLUnit::UC_RAVEN_WEAPON_TAKER_2;
#endif
////////// 이브 ////////////////////////////////////////////////////////////////////////
	case CI_CLASS_CHANGE_EVE_EXOTIC_GEAR:			return CXSLUnit::UC_EVE_EXOTIC_GEAR;
	case CI_CLASS_CHANGE_EVE_NEMESIS:				return CXSLUnit::UC_EVE_CODE_NEMESIS;

	case CI_CLASS_CHANGE_EVE_ARCHITECTURE:			return CXSLUnit::UC_EVE_ARCHITECTURE;
	case CI_CLASS_CHANGE_EVE_EMPRESS:				return CXSLUnit::UC_EVE_CODE_EMPRESS;

	case CI_CLASS_CHANGE_EVE_ELECTRA:				return CXSLUnit::UC_EVE_ELECTRA;
#ifdef SERV_EVE_BATTLE_SERAPH
	case CI_CLASS_CHANGE_EVE_BATTLE_SERAPH:			return CXSLUnit::UC_EVE_BATTLE_SERAPH;
#else
	case CI_CLASS_CHANGE_EVE_ELECTRA_2:				return CXSLUnit::UC_EVE_ELECTRA_2;
#endif
////////// 청 ////////////////////////////////////////////////////////////////////////
	case CI_CLASS_CHANGE_CHUNG_FURY_GUARDIAN:		return CXSLUnit::UC_CHUNG_FURY_GUARDIAN;
	case CI_CLASS_CHANGE_CHUNG_IRON_PALADIN:		return CXSLUnit::UC_CHUNG_IRON_PALADIN;

	case CI_CLASS_CHANGE_CHUNG_SHOOTER_GUARDIAN:	return CXSLUnit::UC_CHUNG_SHOOTING_GUARDIAN;
	case CI_CLASS_CHANGE_CHUNG_DEADLY_CHASER:		return CXSLUnit::UC_CHUNG_DEADLY_CHASER;

	case CI_CLASS_CHANGE_CHUNG_SHELLING_GUARDIAN:		return CXSLUnit::UC_CHUNG_SHELLING_GUARDIAN;
#ifdef SERV_CHUNG_TACTICAL_TROOPER
	case CI_CLASS_CHANGE_CHUNG_TACTICAL_TROOPER:		return CXSLUnit::UC_CHUNG_TACTICAL_TROOPER;
#else
	case CI_CLASS_CHANGE_CHUNG_SHELLING_GUARDIAN_2:		return CXSLUnit::UC_CHUNG_SHELLING_GUARDIAN_2;
#endif
////////// 엘리시스 ////////////////////////////////////////////////////////////////////////
#ifdef SERV_NEW_CHARACTER_EL	// 적용날짜: 2013-07-18
	case CI_CLASS_CHANGE_ELESIS_SABER_KNIGHT:		return CXSLUnit::UC_ELESIS_SABER_KNIGHT;
	case CI_CLASS_CHANGE_ELESIS_PYRO_KNIGHT:		return CXSLUnit::UC_ELESIS_PYRO_KNIGHT;
#endif	// SERV_NEW_CHARACTER_EL
#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE //김창한
	case CI_CLASS_CHANGE_ELESIS_GRAND_MASTER:		return CXSLUnit::UC_ELESIS_GRAND_MASTER;
	case CI_CLASS_CHANGE_ELESIS_BLAZING_HEART:		return CXSLUnit::UC_ELESIS_BLAZING_HEART;
#endif //SERV_ELESIS_SECOND_CLASS_CHANGE
////////// 아라 ////////////////////////////////////////////////////////////////////////
#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
	case CI_CLASS_CHANGE_UC_ARA_LITTLE_HSIEN:		return CXSLUnit::UC_ARA_LITTLE_HSIEN;
	case CI_CLASS_CHANGE_ARA_LITTLE_DEVIL:			return CXSLUnit::UC_ARA_LITTLE_DEVIL;

	case CI_CLASS_CHANGE_ARA_SAKRA_DEVANAM:			return CXSLUnit::UC_ARA_SAKRA_DEVANAM;
	case CI_CLASS_CHANGE_ARA_YAMA_RAJA:				return CXSLUnit::UC_ARA_YAMA_RAJA;
#endif // SERV_ARA_CHANGE_CLASS_SECOND
////////// 애드 ////////////////////////////////////////////////////////////////////////
#ifdef SERV_9TH_NEW_CHARACTER // 김태환 ( 캐릭터 추가용 )
	case CI_CLASS_CHANGE_PSYCHIC_TRACER:			return CXSLUnit::UC_ADD_PSYCHIC_TRACER;
#endif //SERV_9TH_NEW_CHARACTER
#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
	case CI_CLASS_CHANGE_LUNATIC_PSYKER:			return CXSLUnit::UC_ADD_LUNATIC_PSYKER;
#endif //SERV_ADD_LUNATIC_PSYKER

	}

	return CXSLUnit::UC_NONE;
}

#ifdef SERV_UNIT_CLASS_CHANGE_ITEM_POST_MESSEGE_FIX
int CXSLItem::GetClassChangeCashItemIDByUnitClass( const int iUnitClass_ )
{
	switch( iUnitClass_ )
	{
		////////// 엘소드 ////////////////////////////////////////////////////////////////////////
	case CXSLUnit::UC_ELSWORD_KNIGHT:			return CI_CLASS_CHANGE_ELSWORD_KNIGHT;
	case CXSLUnit::UC_ELSWORD_LORD_KNIGHT:		return CI_CLASS_CHANGE_ELSWORD_LORD_KNIGHT;

	case CXSLUnit::UC_ELSWORD_MAGIC_KNIGHT:		return CI_CLASS_CHANGE_ELSWORD_MAGIC_KNIGHT;
	case CXSLUnit::UC_ELSWORD_RUNE_SLAYER:		return CI_CLASS_CHANGE_ELSWORD_RUNE_SLAYER;

	case CXSLUnit::UC_ELSWORD_SHEATH_KNIGHT:		return CI_CLASS_CHANGE_ELSWORD_SHEATH_KNIGHT;
#ifdef SERV_ELSWORD_INFINITY_SWORD
	case CXSLUnit::UC_ELSWORD_INFINITY_SWORD:	return CI_CLASS_CHANGE_ELSWORD_INFINITY_SWORD;
#else
	case CXSLUnit::UC_ELSWORD_SHEATH_KNIGHT_2:	return CI_CLASS_CHANGE_ELSWORD_SHEATH_KNIGHT_2;
#endif
		////////// 아이샤 ////////////////////////////////////////////////////////////////////////
	case CXSLUnit::UC_ARME_HIGH_MAGICIAN:		return CI_CLASS_CHANGE_AISHA_HIGH_MAGICIAN;
	case CXSLUnit::UC_ARME_ELEMENTAL_MASTER:		return CI_CLASS_CHANGE_AISHA_ELMENTAL_MASTER;

	case CXSLUnit::UC_ARME_DARK_MAGICIAN:		return CI_CLASS_CHANGE_AISHA_DARK_MAGICIAN;
	case CXSLUnit::UC_ARME_VOID_PRINCESS:		return CI_CLASS_CHANGE_AISHA_VOID_PRINCESS;

	case CXSLUnit::UC_ARME_BATTLE_MAGICIAN:		return CI_CLASS_CHANGE_AISHA_BATTLE_MAGICIAN;
#ifdef SERV_ARME_DIMENSION_WITCH
	case CXSLUnit::UC_ARME_DIMENSION_WITCH:		return CI_CLASS_CHANGE_AISHA_DIMENSION_WITCH;
#else
	case CXSLUnit::UC_ARME_BATTLE_MAGICIAN_2:	return CI_CLASS_CHANGE_AISHA_BATTLE_MAGICIAN_2;
#endif
		////////// 레나 ////////////////////////////////////////////////////////////////////////
	case CXSLUnit::UC_LIRE_COMBAT_RANGER:		return CI_CLASS_CHANGE_RENA_COMBAT_RANGER;
	case CXSLUnit::UC_LIRE_WIND_SNEAKER:			return CI_CLASS_CHANGE_RENA_WIND_SNEAKER;

	case CXSLUnit::UC_LIRE_SNIPING_RANGER:		return CI_CLASS_CHANGE_RENA_SNIPING_RANGER;
	case CXSLUnit::UC_LIRE_GRAND_ARCHER:			return CI_CLASS_CHANGE_RENA_GRAND_ARCHER;

	case CXSLUnit::UC_LIRE_TRAPPING_RANGER:		return CI_CLASS_CHANGE_RENA_TRAPPING_RANGER;
#ifdef SERV_RENA_NIGHT_WATCHER
	case CXSLUnit::UC_LIRE_NIGHT_WATCHER:		return CI_CLASS_CHANGE_RENA_NIGHT_WATCHER;
#else
	case CXSLUnit::UC_LIRE_TRAPPING_RANGER_2:	return CI_CLASS_CHANGE_RENA_TRAPPING_RANGER_2;
#endif
		////////// 레이븐 ////////////////////////////////////////////////////////////////////////
	case CXSLUnit::UC_RAVEN_SOUL_TAKER:			return CI_CLASS_CHANGE_REVEN_SOUL_TAKER;
	case CXSLUnit::UC_RAVEN_BLADE_MASTER:		return CI_CLASS_CHANGE_REVEN_BLADE_MASTER;

	case CXSLUnit::UC_RAVEN_OVER_TAKER:			return CI_CLASS_CHANGE_REVEN_OVER_TAKER;
	case CXSLUnit::UC_RAVEN_RECKLESS_FIST:		return CI_CLASS_CHANGE_REVEN_RECKLESS_FIST;

	case CXSLUnit::UC_RAVEN_WEAPON_TAKER:		return CI_CLASS_CHANGE_RAVEN_WEAPON_TAKER;
#ifdef SERV_RAVEN_VETERAN_COMMANDER
	case CXSLUnit::UC_RAVEN_VETERAN_COMMANDER:	return CI_CLASS_CHANGE_RAVEN_VETERAN_COMMANDER;
#else
	case CXSLUnit::UC_RAVEN_WEAPON_TAKER_2:		return CI_CLASS_CHANGE_RAVEN_WEAPON_TAKER_2;
#endif
		////////// 이브 ////////////////////////////////////////////////////////////////////////
	case CXSLUnit::UC_EVE_EXOTIC_GEAR:			return CI_CLASS_CHANGE_EVE_EXOTIC_GEAR;
	case CXSLUnit::UC_EVE_CODE_NEMESIS:				return CI_CLASS_CHANGE_EVE_NEMESIS;

	case CXSLUnit::UC_EVE_ARCHITECTURE:			return CI_CLASS_CHANGE_EVE_ARCHITECTURE;
	case CXSLUnit::UC_EVE_CODE_EMPRESS:				return CI_CLASS_CHANGE_EVE_EMPRESS;

	case CXSLUnit::UC_EVE_ELECTRA:				return CI_CLASS_CHANGE_EVE_ELECTRA;
#ifdef SERV_EVE_BATTLE_SERAPH
	case CXSLUnit::UC_EVE_BATTLE_SERAPH:			return CI_CLASS_CHANGE_EVE_BATTLE_SERAPH;
#else
	case CXSLUnit::UC_EVE_ELECTRA_2:				return CI_CLASS_CHANGE_EVE_ELECTRA_2;
#endif
		////////// 청 ////////////////////////////////////////////////////////////////////////
	case CXSLUnit::UC_CHUNG_FURY_GUARDIAN:		return CI_CLASS_CHANGE_CHUNG_FURY_GUARDIAN;
	case CXSLUnit::UC_CHUNG_IRON_PALADIN:		return CI_CLASS_CHANGE_CHUNG_IRON_PALADIN;

	case CXSLUnit::UC_CHUNG_SHOOTING_GUARDIAN:	return CI_CLASS_CHANGE_CHUNG_SHOOTER_GUARDIAN;
	case CXSLUnit::UC_CHUNG_DEADLY_CHASER:		return CI_CLASS_CHANGE_CHUNG_DEADLY_CHASER;

	case CXSLUnit::UC_CHUNG_SHELLING_GUARDIAN:		return CI_CLASS_CHANGE_CHUNG_SHELLING_GUARDIAN;
#ifdef SERV_CHUNG_TACTICAL_TROOPER
	case CXSLUnit::UC_CHUNG_TACTICAL_TROOPER:		return CI_CLASS_CHANGE_CHUNG_TACTICAL_TROOPER;
#else
	case CXSLUnit::UC_CHUNG_SHELLING_GUARDIAN_2:		return CI_CLASS_CHANGE_CHUNG_SHELLING_GUARDIAN_2;
#endif
		////////// 아라 ////////////////////////////////////////////////////////////////////////
#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
	case CXSLUnit::UC_ARA_LITTLE_HSIEN:				return CI_CLASS_CHANGE_UC_ARA_LITTLE_HSIEN;
	case CXSLUnit::UC_ARA_LITTLE_DEVIL:				return CI_CLASS_CHANGE_ARA_LITTLE_DEVIL;

	case CXSLUnit::UC_ARA_SAKRA_DEVANAM:			return CI_CLASS_CHANGE_ARA_SAKRA_DEVANAM;
	case CXSLUnit::UC_ARA_YAMA_RAJA:				return CI_CLASS_CHANGE_ARA_YAMA_RAJA;
#endif // SERV_ARA_CHANGE_CLASS_SECOND
		////////// 엘리시스 ////////////////////////////////////////////////////////////////////////
#ifdef SERV_NEW_CHARACTER_EL // 추후 디파인 제거 할 예정
	case CXSLUnit::UC_ELESIS_SABER_KNIGHT:			return CI_CLASS_CHANGE_ELESIS_SABER_KNIGHT;
	case CXSLUnit::UC_ELESIS_PYRO_KNIGHT:			return CI_CLASS_CHANGE_ELESIS_PYRO_KNIGHT;
#endif // SERV_NEW_CHARACTER_EL
#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE //김창한
	case CXSLUnit::UC_ELESIS_GRAND_MASTER:			return CI_CLASS_CHANGE_ELESIS_GRAND_MASTER;
	case CXSLUnit::UC_ELESIS_BLAZING_HEART:			return CI_CLASS_CHANGE_ELESIS_BLAZING_HEART;
#endif //SERV_ELESIS_SECOND_CLASS_CHANGE
		////////// 애드 ////////////////////////////////////////////////////////////////////////
#ifdef SERV_9TH_NEW_CHARACTER // 김태환 ( 캐릭터 추가용 )
	case CXSLUnit::UC_ADD_PSYCHIC_TRACER:			return CI_CLASS_CHANGE_PSYCHIC_TRACER;
#endif //SERV_9TH_NEW_CHARACTER
#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
	case CXSLUnit::UC_ADD_LUNATIC_PSYKER:			return CI_CLASS_CHANGE_LUNATIC_PSYKER;
#endif //SERV_ADD_LUNATIC_PSYKER
	}

	return 0;
}
#endif SERV_UNIT_CLASS_CHANGE_ITEM_POST_MESSEGE_FIX
#endif SERV_UNIT_CLASS_CHANGE_ITEM
//}}

#ifdef SERV_WISH_ITEM_DISABLE
const bool	CXSLItem::IsPossibleCart( const int iItemID_ )
{
	if( CXSLItem::GetClassChangeCashItem( iItemID_ ) != CXSLUnit::UC_NONE )
		return false;

	return true;
}
#endif // SERV_WISH_ITEM_DISABLE

#ifdef SERV_GOLD_TICKET_SEAL_COUNT
bool CXSLItem::IsGoldTicket( const int iItemID )
{
	switch( iItemID )
	{
	case CXSLItem::SI_GOLD_TICKET_10_ID:
	case CXSLItem::SI_GOLD_TICKET_50_ID:
	case CXSLItem::SI_GOLD_TICKET_100_ID:
	case CXSLItem::SI_GOLD_TICKET_500_ID:
	case CXSLItem::SI_GOLD_TICKET_1000_ID:
	case CXSLItem::SI_GOLD_TICKET_2000_ID:
		return true;
	}

	return false;
}
#endif SERV_GOLD_TICKET_SEAL_COUNT

#ifdef SERV_SKILL_PAGE_SYSTEM
bool CXSLItem::IsItemToMakeSkillPageExpanded( const int iItemID_ )
{
	switch ( iItemID_ )
	{
	case CI_EXPAND_SKILL_PAGE:
		return true;
		break;

	default:
		break;
	}

	return false;	
}
#endif // SERV_SKILL_PAGE_SYSTEM