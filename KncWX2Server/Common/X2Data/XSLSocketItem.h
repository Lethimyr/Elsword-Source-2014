#pragma once

#include "RefreshSingleton.h"
#include "Lottery.h"
#include "XSLStat.h"

class CXSLSocketItem
{
public:
	enum SOCKET_DATA_TYPE
	{
		SDT_NONE		 = 0,
		SDT_WEAPON_NORMAL,
		SDT_WEAPON_RARE,
		SDT_DEFENCE_NORMAL,
		SDT_DEFENCE_RARE,
		SDT_SET_ITEM,
        SDT_ACCESSORY,
		SDT_TITLE,
		SDT_WEAPON_HARMONY,
		SDT_DEFENCE_HARMONY,
		SDT_WEAPON_MYSTERY,
		SDT_DEFENCE_MYSTERY,
		SDT_WEAPON_SHINE,
		SDT_DEFENCE_SHINE,
		SDT_WEAPON_BATTLE,
		SDT_DEFENCE_BATTLE,
		SDT_WEAPON_INCREASE,
		SDT_DEFENCE_INCREASE,
		SDT_WEAPON_HIT,
		SDT_DEFENCE_HIT,
		SDT_WEAPON_ATK_PHYSIC,
		SDT_DEFENCE_ATK_PHYSIC,
		SDT_WEAPON_ATK_MAGIC,
		SDT_DEFENCE_ATK_MAGIC,
		SDT_WEAPON_DEF_PHYSIC,
		SDT_DEFENCE_DEF_PHYSIC,
		SDT_WEAPON_DEF_MAGIC,
		SDT_DEFENCE_DEF_MAGIC,
		SDT_WEAPON_HYPER_TIME,
		SDT_DEFENCE_HYPER_TIME,
		SDT_WEAPON_INCREASE_HP,
		SDT_DEFENCE_INCREASE_HP,
		SDT_WEAPON_HYPER_CHARGE,
		SDT_DEFENCE_HYPER_CHARGE,
		SDT_WEAPON_SPEED,
		SDT_DEFENCE_SPEED,
		SDT_WEAPON_CRITICAL,
		SDT_DEFENCE_CRITICAL,
		SDT_WEAPON_MOVE_SPEED,
		SDT_DEFENCE_MOVE_SPEED,
		SDT_WEAPON_STRIKE,
		SDT_DEFENCE_STRIKE,
		SDT_WEAPON_AVOID,
		SDT_DEFENCE_AVOID,
		SDT_WEAPON_JUMP_SPEED,
		SDT_DEFENCE_JUMP_SPEED,
//{{ 2011. 07. 27 김민성   소켓 아이템 추가
		SDT_WEAPON_ADD_DAMAGE,
		SDT_DEFENCE_ADD_DAMAGE,
		SDT_WEAPON_ADD_DAMAGE_REDUCE,
		SDT_DEFENCE_ADD_DAMAGE_REDUCE,
//}}
		//{{ 2011. 08. 30	최육사	듀얼 마법석 개편
//#ifdef SERV_NEW_DUAL_MAGIC_STONE
		SDT_WEAPON_DESTROY,
		SDT_DEFENCE_DESTROY,
		SDT_WEAPON_EXPLOSION,
		SDT_DEFENCE_EXPLOSION,
		SDT_WEAPON_BURN,
		SDT_DEFENCE_BURN,
		SDT_WEAPON_QUICK,
		SDT_DEFENCE_QUICK,
		SDT_WEAPON_FURY,
		SDT_DEFENCE_FURY,
//#endif SERV_NEW_DUAL_MAGIC_STONE
		//}}
		//{{ 2012. 07. 09	김민성       현자의 마법석
//#ifdef SERV_DUNGEON_CLEAR_PAYMENT_STONE_EVENT
		SDT_WEAPON_SAGE,
		SDT_DEFENCE_SAGE,
//#endif SERV_DUNGEON_CLEAR_PAYMENT_STONE_EVENT
		//}}

		//{{ 2012. 09. 19	박세훈	2012 대전 시즌2
//#ifdef SERV_2012_PVP_SEASON2
		SDT_WEAPON_HYPER_TIME_PVP,			/// 대련용 마법석 : 무기 각성 지속 시간
		SDT_DEFENCE_HYPER_TIME_PVP,			/// 대련용 마법석 : 방어구 각성 지속 시간
		SDT_WEAPON_INCREASE_HP_PVP,			/// 대련용 마법석 : 무기 체력
		SDT_DEFENCE_INCREASE_HP_PVP,		/// 대련용 마법석 : 방어구 체력
		SDT_WEAPON_HYPER_CHARGE_PVP,		/// 대련용 마법석 : 무기 각성 충전 속도
		SDT_DEFENCE_HYPER_CHARGE_PVP,		/// 대련용 마법석 : 방어구 각성 충전 속도
		SDT_WEAPON_SPEED_PVP,				/// 대련용 마법석 : 무기 동작속도
		SDT_DEFENCE_SPEED_PVP,				/// 대련용 마법석 : 방어구 동작속도
		SDT_WEAPON_CRITICAL_PVP,			/// 대련용 마법석 : 무기 크리티컬
		SDT_DEFENCE_CRITICAL_PVP,			/// 대련용 마법석 : 방어구 크리티컬
		SDT_WEAPON_MOVE_SPEED_PVP,			/// 대련용 마법석 : 무기 이동력
		SDT_DEFENCE_MOVE_SPEED_PVP,			/// 대련용 마법석 : 방어구 이동력
		SDT_WEAPON_STRIKE_PVP,				/// 대련용 마법석 : 무기 명중
		SDT_DEFENCE_STRIKE_PVP,				/// 대련용 마법석 : 방어구 명중
		SDT_WEAPON_AVOID_PVP,				/// 대련용 마법석 : 무기 회피
		SDT_DEFENCE_AVOID_PVP,				/// 대련용 마법석 : 방어구 회피
		SDT_WEAPON_JUMP_SPEED_PVP,			/// 대련용 마법석 : 무기 점프력
		SDT_DEFENCE_JUMP_SPEED_PVP,			/// 대련용 마법석 : 방어구 점프력
		SDT_WEAPON_ADD_DAMAGE_PVP,			/// 대련용 마법석 : 무기 추가 데미지
		SDT_DEFENCE_ADD_DAMAGE_PVP,			/// 대련용 마법석 : 방어구 추가 데미지
		SDT_WEAPON_ADD_DAMAGE_REDUCE_PVP,	/// 대련용 마법석 : 무기 데미지 감소
		SDT_DEFENCE_ADD_DAMAGE_REDUCE_PVP,	/// 대련용 마법석 : 방어구 데미지 감소
		SDT_WEAPON_DESTROY_PVP,				/// 대련용 마법석 : 무기 파괴
		SDT_DEFENCE_DESTROY_PVP,			/// 대련용 마법석 : 방어구 파괴
		SDT_WEAPON_EXPLOSION_PVP,			/// 대련용 마법석 : 무기 폭발
		SDT_DEFENCE_EXPLOSION_PVP,			/// 대련용 마법석 : 방어구 폭발
		SDT_WEAPON_BURN_PVP,				/// 대련용 마법석 : 무기 연소
		SDT_DEFENCE_BURN_PVP,				/// 대련용 마법석 : 방어구 연소
		SDT_WEAPON_QUICK_PVP,				/// 대련용 마법석 : 무기 신속
		SDT_DEFENCE_QUICK_PVP,				/// 대련용 마법석 : 방어구 신속
		SDT_WEAPON_FURY_PVP,				/// 대련용 마법석 : 무기 격노
		SDT_DEFENCE_FURY_PVP,				/// 대련용 마법석 : 방어구 격노
//#endif SERV_2012_PVP_SEASON2
		//}}
		SDT_PVP_TAG_POINT,					// ???
		//{{ 2012. 12. 11  고정 효과 마법석 추가 - 김민성
//#ifdef SERV_FIXED_EFFECTS_SOCKET_ITEM
		SDT_WEAPON_SPEED_2P5_FIXED,				// 고정 효과 마법석 2.5% : 무기 속도 
		SDT_DEFFENCE_SPEED_2P5_FIXED,			// 고정 효과 마법석 2.5% : 방어구 속도
		SDT_WEAPON_SPEED_5P_FIXED,				// 고정 효과 마법석 5% : 무기 속도
		SDT_DEFFENCE_SPEED_5P_FIXED,			// 고정 효과 마법석 5% : 방어구 속도
		SDT_WEAPON_CRITICAL_2P5_FIXED,			// 고정 효과 마법석 2.5% : 무기 크리티컬
		SDT_DEFFENCE_CRITICAL_2P5_FIXED,		// 고정 효과 마법석 2.5% : 방어구 크리티컬
		SDT_WEAPON_CRITICAL_5P_FIXED,			// 고정 효과 마법석 5% : 무기 크리티컬
		SDT_DEFFENCE_CRITICAL_5P_FIXED,			// 고정 효과 마법석 5% : 방어구 크리티컬
		SDT_WEAPON_AVOID_2P5_FIXED,				// 고정 효과 마법석 2.5% : 무기 회피
		SDT_DEFFECNE_AVOID_2P5_FIXED,			// 고정 효과 마법석 2.5% : 방어구 회피
		SDT_WEAPON_AVOID_5P_FIXED,				// 고정 효과 마법석 5% : 무기 회피
		SDT_DEFFENCE_AVOID_5P_FIXED,			// 고정 효과 마법석 5% : 방어구 회피
		SDT_WEAPON_ADD_DAMAGE_2P5_FIXED,		// 고정 효과 마법석 2.5% : 무기 추가 데미지
		SDT_DEFFENCE_ADD_DAMAGE_2P5_FIXED,		// 고정 효과 마법석 2.5% : 방어구 추가 데미지
		SDT_WEAPON_ADD_DAMAGE_5P_FIXED,			// 고정 효과 마법석 5% : 무기 추가 데미지
		SDT_DEFFENCE_ADD_DAMAGE_5P_FIXED,		// 고정 효과 마법석 5% : 방어구 추가 데미지
		SDT_WEAPON_DAMAGE_REDUCE_2P5_FIXED,		// 고정 효과 마법석 2.5% : 무기 데미지 감소
		SDT_DEFFECNE_DAMAGE_REDUCE_2P5_FIXED,	// 고정 효과 마법석 2.5% : 방어구 데미지 감소
		SDT_WEAPON_DAMAGE_REDUCE_5P_FIXED,		// 고정 효과 마법석 5% : 무기 데미지 감소
		SDT_DEFFENCE_DAMAGE_REDUCE_5P_FIXED,	// 고정 효과 마법석 5% : 방어구 데미지 감소
		SDT_WEAPON_HYPER_CHARGE_5P_FIXED,		// 고정 효과 마법석 5% : 무기 각성 충전
		SDT_DEFFENCE_HYPER_CHARGE_5P_FIXED,		// 고정 효과 마법석 5% : 방어구 각성 충전
		SDT_WEAPON_HYPER_CHARGE_10P_FIXED,		// 고정 효과 마법석 10% : 무기 각성 충전
		SDT_DEFFENCE_HYPER_CHARGE_10P_FIXED,	// 고정 효과 마법석 10% : 방어구 각성 충전
//#endif SERV_FIXED_EFFECTS_SOCKET_ITEM

		SDT_SKILL_LV_GROUP,						/// 직업에 따라 다른 스킬을 부여하기 위한 스킬 그룹
		//}}

		SDT_QUEST_CLEAR_COUNT 					= 120, /// 해외팀 머지 작업. 2013-09-03 김창한
		SDT_ITEM_EXCHANGE_COUNT 				= 121, /// 해외팀 머지 작업. 2013-09-03 김창한
	};

	enum MAGIC_STONE_ITEM_ID
	{
		MSI_NONE = 0,
		MSI_MAGIC_STONE				= 109945,
		MSI_RARE_MAGIC_STONE		= 109955,
		//{{ 2010. 04. 22  최육사	소켓 개편
		MSI_HARMONY_MAGIC_STONE		= 132110,
		MSI_MYSTERY_MAGIC_STONE		= 132111,
		MSI_SHINE_MAGIC_STONE		= 132112,
		MSI_BATTLE_MAGIC_STONE		= 132113,
		MSI_INCREASE_MAGIC_STONE	= 132114,
		MSI_HIT_MAGIC_STONE			= 132115,

		MSI_ATK_PHYSIC_MAGIC_STONE	= 132193, // 물리 공격력의 마법석
		MSI_ATK_MAGIC_MAGIC_STONE	= 132194, // 마법 공격력의 마법석
		MSI_DEF_PHYSIC_MAGIC_STONE	= 132195, // 물리 방어력의 마법석
		MSI_DEF_MAGIC_MAGIC_STONE	= 132196, // 마법 방어력의 마법석
		MSI_HYPER_TIME_MAGIC_STONE	= 132197, // 각성 지속의 마법석
		MSI_INCREASE_HP_MAGIC_STONE	= 132198, // 체력의 마법석
		MSI_HYPER_CHARGE_MAGIC_STONE= 132199, // 각성 충전의 마법석
		MSI_SPEED_MAGIC_STONE		= 132200, // 속도의 마법석
		MSI_CRITICAL_MAGIC_STONE	= 132201, // 크리티컬의 마법석
		MSI_MOVE_SPEED_MAGIC_STONE	= 132202, // 이동 속도의 마법석
		MSI_STRIKE_MAGIC_STONE		= 132203, // 명중의 마법석
		MSI_AVOID_MAGIC_STONE		= 132204, // 회피의 마법석0
		MSI_JUMP_SPEED_MAGIC_STONE	= 132205, // 점프 속도의 마법석
		//}}
		//{{ 2011. 07. 27 김민성   소켓 아이템 추가
		MSI_INCREASE_DAMEGE_STONE	= 72510,  // 추가 데미지의 마법석
		MSI_DECREASE_DAMEGE_STONE	= 72520,  // 데미지 감소의 마법석
		//}}
		//{{ 2011. 08. 30	최육사	듀얼 마법석 개편
#ifdef SERV_NEW_DUAL_MAGIC_STONE
		MSI_DESTROY_MAGIC_STONE		= 111701, // 파괴의 마법석
		MSI_EXPLOSION_MAGIC_STONE	= 111702, // 폭발의 마법석
		MSI_BURN_MAGIC_STONE		= 111703, // 불타는 마법석
		MSI_QUICK_MAGIC_STONE		= 111704, // 빠름의 마법석
		MSI_FURY_MAGIC_STONE		= 111705, // 분노의 마법석
#endif SERV_NEW_DUAL_MAGIC_STONE
		//}}
		//{{ 2012. 07. 09	김민성       현자의 마법석
//#ifdef SERV_DUNGEON_CLEAR_PAYMENT_STONE_EVENT
		MSI_WISE_MAN_STONE_ITEM		= 135184, // 현자의 마법석
//#endif SERV_DUNGEON_CLEAR_PAYMENT_STONE_EVENT
		//}}

		//{{ 2012. 09. 19	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
		MSI_MOVE_SPEED_PVP			= 98004,	/// 대련용 마법석 : 이동력
		MSI_JUMP_SPEED_PVP			= 98005,	/// 대련용 마법석 : 점프력
		MSI_INCREASE_HP_PVP			= 98006,	/// 대련용 마법석 : 체력
		MSI_STRIKE_PVP				= 98007,	/// 대련용 마법석 : 명중
		MSI_AVOID_PVP				= 98008,	/// 대련용 마법석 : 회피
		MSI_HYPER_CHARGE_PVP		= 98009,	/// 대련용 마법석 : 각성 충전 속도
		MSI_HYPER_TIME_PVP			= 98010,	/// 대련용 마법석 : 각성 지속 시간
		MSI_CRITICAL_PVP			= 98011,	/// 대련용 마법석 : 크리티컬
		MSI_SPEED_PVP				= 98012,	/// 대련용 마법석 : 동작속도
		MSI_ADD_DAMAGE_PVP			= 98013,	/// 대련용 마법석 : 추가 데미지
		MSI_ADD_DAMAGE_REDUCE_PVP	= 98014,	/// 대련용 마법석 : 데미지 감소
		MSI_EXPLOSION_PVP			= 98015,	/// 대련용 마법석 : 폭발
		MSI_DESTROY_PVP				= 98016,	/// 대련용 마법석 : 파괴
		MSI_QUICK_PVP				= 98017,	/// 대련용 마법석 : 신속
		MSI_BURN_PVP				= 98018,	/// 대련용 마법석 : 연소
		MSI_FURY_PVP				= 98019,	/// 대련용 마법석 : 격노
#endif SERV_2012_PVP_SEASON2
		//}}
		//{{ 2012. 12. 11  고정 효과 마법석 추가 - 김민성
#ifdef SERV_FIXED_EFFECTS_SOCKET_ITEM
		MSI_SPEED_MAGIC_STONE_FIXED_EFFECT			= 98028,	/// 동작 속도 고정 마법석
		MSI_CRITICAL_MAGIC_STONE_FIXED_EFFECT		= 98029,	/// 크리티컬 고정 마법석
		MSI_INCREASE_DAMEGE_STONE_FIXED_EFFECT		= 98030,	/// 추가 데미지 고정 마법석
		MSI_HYPER_CHARGE_MAGIC_STONE_FIXED_EFFECT	= 98031,	/// 각성 충전 속도 고정 마법석
		MSI_AVOID_MAGIC_STONE_FIXED_EFFECT			= 98032,	/// 회피 고정 마법석
		MSI_DECREASE_DAMEGE_STONE_FIXED_EFFECT		= 98033,	/// 데미지 감소 고정 마법석
#endif SERV_FIXED_EFFECTS_SOCKET_ITEM
		//}}

//#ifdef SERV_MISS_ELIOS_STONE_ITEM
		MSI_MISS_ELIOS_STONE_ITEM				= 67003750, // 미스 엘리오스 특별 마법석
//#endif SERV_MISS_ELIOS_STONE_ITEM

//#ifdef SERV_BC_EVENT
		MSI_VC_EVENT_STONE_ITEM					= 60005473, // 불꽃의 마법석
//#endif SERV_BC_EVENT

//#ifdef SERV_FISHING_HOLE_MAGIC_STONE
		MSI_EVENT_MAGIC_STONE_1					= 60004121,
		MSI_EVENT_MAGIC_STONE_2					= 60004122,
		MSI_EVENT_MAGIC_STONE_3					= 60004123,
		MSI_EVENT_MAGIC_STONE_4					= 60004124,
		MSI_EVENT_MAGIC_STONE_5					= 60004125,
//#endif SERV_FISHING_HOLE_MAGIC_STONE

//#ifdef SERV_LURIEL_MAGIC_STONE
		MSI_LURIEL_INCREASE_DAMEGE_STONE		= 85003840, /// 루리엘의 추가 데미지의 마법석
		MSI_LURIEL_DECREASE_DAMEGE_STONE		= 85003841, /// 루리엘의 데미지 감소의 마법석
		MSI_LURIEL_HYPER_TIME_MAGIC_STONE		= 85003842, /// 루리엘의 각성 지속의 마법석
		MSI_LURIEL_INCREASE_HP_MAGIC_STONE		= 85003843, /// 루리엘의 체력의 마법석
		MSI_LURIEL_HYPER_CHARGE_MAGIC_STONE		= 85003844, /// 루리엘의 각성 충전의 마법석
		MSI_LURIEL_SPEED_MAGIC_STONE			= 85003845, /// 루리엘의 속도의 마법석
		MSI_LURIEL_CRITICAL_MAGIC_STONE			= 85003846, /// 루리엘의 크리티컬의 마법석
		MSI_LURIEL_MOVE_SPEED_MAGIC_STONE		= 85003847, /// 루리엘의 이동 속도의 마법석
		MSI_LURIEL_STRIKE_MAGIC_STONE			= 85003848, /// 루리엘의 명중의 마법석
		MSI_LURIEL_AVOID_MAGIC_STONE			= 85003849, /// 루리엘의 회피의 마법석
		MSI_LURIEL_JUMP_SPEED_MAGIC_STONE		= 85003850, /// 루리엘의 점프 속도의 마법석
		MSI_LURIEL_DESTROY_MAGIC_STONE			= 152000699, // 루리엘의 파괴의 마법석(파괴)
		MSI_LURIEL_EXPLOSION_MAGIC_STONE		= 152000700, // 루리엘의 폭발의 마법석(폭발)
		MSI_LURIEL_BURN_MAGIC_STONE				= 152000701, // 루리엘의 불타는 마법석(연소)
		MSI_LURIEL_QUICK_MAGIC_STONE			= 152000702, // 루리엘의 빠름의 마법석(신속)
		MSI_LURIEL_FURY_MAGIC_STONE				= 152000703, // 루리엘의 분노의 마법석(격노)
//#endif SERV_LURIEL_MAGIC_STONE
	};

	enum SOCKET_COUNT
	{
		SC_NONE           = -1,

		SC_WEAPON_LOW     = 0,
		SC_WEAPON_NORMAL  = 1,
		SC_WEAPON_RARE    = 2,
		SC_WEAPON_ELITE   = 3,
		SC_WEAPON_UNIQUE  = 4,

		SC_DEFENCE_LOW    = 0,
		SC_DEFENCE_NORMAL = 1,
		SC_DEFENCE_RARE   = 1,
		SC_DEFENCE_ELITE  = 2,
		SC_DEFENCE_UNIQUE = 3,
	};	

	//{{ 2013. 05. 15	최육사	아이템 개편
	//#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
	enum RANDOM_SOCKET_ENUM
	{
		RSC_MAX			  = 5,
	};
	//#endif SERV_NEW_ITEM_SYSTEM_2013_05
	//}}

	//struct SocketExtraDamage
	//{
	//	float								m_fRate;
	//	CX2DamageManager::ExtraDamageData	m_ExtraDamageData;

	//	SocketExtraDamage()
	//	{
	//		m_fRate = 0.0f;
	//	}
	//};

public:

	struct SocketData
	{
		int					m_ID;
		SOCKET_DATA_TYPE	m_Type;						//옵션타입
		CXSLStat::Stat		m_Stat;						//능력치 증가
		//float				m_fMoveSpeed;				//달리기 속도 증가(%)(최대 30%)
		//float				m_fJumpSpeed;				//점프 속도 증가(%)(최대 20%)
		//float				m_fHyperModeChargeRate;		//소울 게이지 차징 속도 증가(%)(최대 50%)
		//float				m_fHyperModeTime;			//각성 시간 증가(최대 20초)
		float				m_fRepairPriceSale;			//내구도 수리비 감소(%)(최대90%)
		//SocketExtraDamage	m_SocketExtraDamage;

		float				m_IncreaseHPRate;

		bool				m_bDungeonOnly;
		bool				m_bPVPOnly;
		bool				m_bOverlap;

		float				m_fIncreaseEDPer;
		float				m_fIncreaseExpPer;

		//{{ 2008. 11. 15  최육사	속성강화
		float				m_fResistBlaze;
		float				m_fResistWater;
		float				m_fResistNature;
		float				m_fResistWind;
		float				m_fResistLight;
		float				m_fResistDark;
		//}}
		//{{ 2010. 10. 18	최육사	펫 오라 스킬
#ifdef SERV_PET_AURA_SKILL
		float				m_fAttackBlaze;
		float				m_fAttackWater;
		float				m_fAttackNature;
		float				m_fAttackWind;
		float				m_fAttackLight;
		float				m_fAttackDark;

		float				m_fIncreaseMPRate;
		float				m_fIncreaseAtkPhysicRate;
		float				m_fIncreaseAtkMagicRate;
		float				m_fIncreaseDefPhysicRate;
		float				m_fIncreaseDefMagicRate;
#endif SERV_PET_AURA_SKILL
		//}}

		//{{ 2012. 06. 27	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2_1
		bool				m_bIsLevelLinkedStat;
		char				m_cAddLevel;
#endif SERV_2012_PVP_SEASON2_1
		//}}
		//{{ 2013. 06. 04	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
		KStatRelationLevel	m_kStatRelLVData;
#endif SERV_NEW_ITEM_SYSTEM_2013_05
		//}}

		SocketData()
		{
			m_Type						= SDT_NONE;
			//m_fMoveSpeed				= 0.0f;
			//m_fJumpSpeed				= 0.0f;
			//m_fHyperModeChargeRate	= 0.0f;
			//m_fHyperModeTime			= 0.0f;
			m_fRepairPriceSale			= 0.0f;

			m_bDungeonOnly				= false;
			m_bPVPOnly					= false;

			m_IncreaseHPRate			= 0.f;

			m_bOverlap					= false;

			m_fIncreaseEDPer			= 0.0f;
			m_fIncreaseExpPer			= 0.0f;

			//{{ 2008. 11. 15  최육사	속성강화
			m_fResistBlaze				= 0.0f;
			m_fResistWater				= 0.0f;
			m_fResistNature				= 0.0f;
			m_fResistWind				= 0.0f;
			m_fResistLight				= 0.0f;
			m_fResistDark				= 0.0f;
			//}}
			//{{ 2010. 10. 18	최육사	펫 오라 스킬
#ifdef SERV_PET_AURA_SKILL
			m_fAttackBlaze				= 0.0f;
			m_fAttackWater				= 0.0f;
			m_fAttackNature				= 0.0f;
			m_fAttackWind				= 0.0f;
			m_fAttackLight				= 0.0f;
			m_fAttackDark				= 0.0f;

			m_fIncreaseMPRate			= 0.0f;
			m_fIncreaseAtkPhysicRate	= 0.0f;
			m_fIncreaseAtkMagicRate		= 0.0f;
			m_fIncreaseDefPhysicRate	= 0.0f;
			m_fIncreaseDefMagicRate		= 0.0f;
#endif SERV_PET_AURA_SKILL
			//}}

			//{{ 2012. 06. 27	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2_1
			m_bIsLevelLinkedStat		= false;
			m_cAddLevel					= 0;
#endif SERV_2012_PVP_SEASON2_1
			//}}
		}
	};

	typedef std::pair< int, int > KBeginEndEnchantLv;
	typedef std::map< KBeginEndEnchantLv, KLottery > KSocketGroup;

	struct SocketOptionInfo
	{
		int					m_iSocketOptionID;
		int					m_iRandomCount;
		int					m_iEnchantLevel;
		float				m_fRepairPriceSale;
		bool				m_bOverLap;

		SocketOptionInfo()
		{
			m_iSocketOptionID	= 0;
			m_iRandomCount		= 0;
			m_iEnchantLevel		= 0;
			m_fRepairPriceSale	= 0;
			m_bOverLap			= false;
		}
	};

	//{{ 2008. 8. 18  최육사	세트 아이템
	struct SetItemData
	{
		int										m_SetID;
		std::wstring							m_SetName;
		//{{ 2011. 07. 25    김민성    아이템 옵션ID 데이터 사이즈 증가
#ifdef SERV_ITEM_OPTION_DATA_SIZE
		std::map< int, std::vector< int > >	m_mapNeedPartsNumNOptions;
#else
		std::map< int, std::vector< short > >	m_mapNeedPartsNumNOptions;
#endif SERV_ITEM_OPTION_DATA_SIZE
		//}} 

		SetItemData()
		{
			m_SetID = 0;
		}
	};
	//}}

	//{{ 2013. 05. 15	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
	struct SMiracleCubeLevel
	{
        int				m_iMinLevel;
		int				m_iMaxLevel;

		SMiracleCubeLevel()
		{
			m_iMinLevel = 0;
			m_iMaxLevel = 0;
		}
	};
#endif SERV_NEW_ITEM_SYSTEM_2013_05
	//}}

private:
	DeclareRefreshSingleton( CXSLSocketItem );
	DeclareLuaScriptParser;
	DeclDumpToLogFile;
	DeclDump;
	DeclToStringW;

public:
	CXSLSocketItem(void);
	~CXSLSocketItem(void);

	// 소켓 정보
	bool AddSocketData_LUA();
	bool AddSocketRandomData_LUA( int iSocketDataType, int iGroupID, float fRate );
	bool AddSocketGroup_LUA( int iGroupID, int iBeginEnchantLv, int iEndEnchantLv, int iOptionID, float fRate );

	// 세트 아이템 정보
	bool AddSetItemData_LUA();

	//{{ 2012. 11. 28 캐시 아이템 소켓 옵션 기능 수정 - 김민성
#ifdef SERV_CASH_ITEM_SOCKET_OPTION
	void AddSocektGroupDataForCashAvatar_LUA();
	bool CheckCashItemSocketID( IN int iType, IN int iSocketID );
	bool CheckCashItemGroupID( IN int iSocketID, IN int iGroupID  );
#endif SERV_CASH_ITEM_SOCKET_OPTION
	//}

	// 소켓
	const SocketData* GetSocketData( IN int iSocketOptionID ) const;
	int  GetSocketCount( IN int iItemGrade, IN int iItemType ) const;
	bool VerifySocketCount( IN int iItemGrade, IN bool bIsWeapon, IN int iSocketCount ) const;
	SOCKET_DATA_TYPE GetSocketDataType( IN int iItemID, IN int iItemType ) const;
	int  GetMagicStoneItemID( IN bool bIsRare ) const;

	//{{ 2012. 09. 26  김민성  대전 장비 소켓 고정 강화 레벨
#ifdef SERV_2012_PVP_SEASON2_1
	bool GetResultItem( IN int iMagicStoneItemID, IN int iItemType, IN int iPeriodType, IN int iUseLevel, IN bool bFashion, IN int iEnchantLevel, OUT int& iResultSocketOption ) const;
#else
	//{{ 2011. 07. 25    김민성    아이템 옵션ID 데이터 사이즈 증가
//#ifdef SERV_ITEM_OPTION_DATA_SIZE
//	bool GetResultItem( IN int iMagicStoneItemID, IN int iItemType, IN int iPeriodType, IN bool bFashion, IN int iEnchantLevel, OUT int& iResultSocketOption ) const;
//#else
//	bool GetResultItem( IN int iMagicStoneItemID, IN int iItemType, IN int iPeriodType, IN bool bFashion, IN int iEnchantLevel, OUT short& sResultSocketOption ) const;
//#endif SERV_ITEM_OPTION_DATA_SIZE
	//}} 
#endif SERV_2012_PVP_SEASON2_1
	//}}

	//{{ 2010. 04. 15  최육사	소켓 개편
#ifdef SERV_SOCKET_NEW
	bool IsMagicStoneItemID( IN int iItemID );
	bool CalcInsertSocketCost( IN int iItemID, IN int iMagicStoneItemID, IN int iAssignedSocketCount, IN bool bEmptySlot, OUT int& iCost );
#endif SERV_SOCKET_NEW
	//}}

//	//{{ 2011. 07. 25    김민성    아이템 옵션ID 데이터 사이즈 증가
//	#ifdef SERV_ITEM_OPTION_DATA_SIZE
	bool GetSocketOptionStat(	IN const std::vector< int >& vecSocketOption, 
								IN bool bIsDungeon,
								IN const KStat& kBaseStat,
								IN OUT float& fTotalIncHPRate,
								OUT KStat& kAddStat,
								IN unsigned int& uiHP_OnePoint,
								IN unsigned int& iItemLevel
								//{{ 2012. 09. 25	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2_1
								, IN const bool bLevelLinkedStat = false
#endif SERV_2012_PVP_SEASON2_1
								//}}
								);
//	#else
//	void GetSocketOptionStat( IN const std::vector< short >& vecSocketOption, IN bool bIsDungeon, IN const KStat& kBaseStat, IN OUT float& fTotalIncHPRate, OUT KStat& kAddStat, IN unsigned int& uiHP_OnePoint, IN unsigned int& iItemLevel );
//	#endif SERV_ITEM_OPTION_DATA_SIZE
//	//}}

	//{{ 2011. 07. 25    김민성    아이템 옵션ID 데이터 사이즈 증가
#ifdef SERV_ITEM_OPTION_DATA_SIZE
	void GetSocketOptionIncreaseRate( IN const std::vector< int >& vecSocketOption, IN bool bIsDungeon, IN OUT KStatIncreaseRate& kStatIncreaseRateInfo );
#else
	void GetSocketOptionIncreaseRate( IN const std::vector< short >& vecSocketOption, IN bool bIsDungeon, IN OUT KStatIncreaseRate& kStatIncreaseRateInfo );
#endif SERV_ITEM_OPTION_DATA_SIZE
	//}}

	// 세트 아이템	
	const SetItemData* GetSetItem( int setID );
	//{{ 2011. 07. 25    김민성    아이템 옵션ID 데이터 사이즈 증가
#ifdef SERV_ITEM_OPTION_DATA_SIZE
	void GetSetItemOptions( int setID, int partsNum, std::vector< int >& vecOptions );
#else
	void GetSetItemOptions( int setID, int partsNum, std::vector< short >& vecOptions );
#endif SERV_ITEM_OPTION_DATA_SIZE
	//}} 

	//{{ 2013. 05. 15	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
	bool AddRandomSocketGroupInfo_LUA( int iGroupID, int iOptionID, float fRate );	
	bool EvaluateItemRandomSocket( IN const int iItemID, OUT std::vector< int >& vecRandomSocket );

	bool AddMiracleCubeInfo_LUA( int iItemID, int iMinLevel, int iMaxLevel );
	bool IsCanUseMiracleCube( IN const int iMiracleCubeItemID, IN const int iItemLevel ) const;

	void GetAndAddStatRelationLevel( IN const std::vector< int >& vecSocketOption, IN OUT KStatRelationLevel& kAddStatRelLevel ) const;
#endif SERV_NEW_ITEM_SYSTEM_2013_05
	//}}

private:
	//{{ 2008. 11. 17  최육사	아이템 개편
	std::map< int, KLottery >										m_mapSocketRandom;
	std::map< int, KSocketGroup >									m_mapSocketGroup;
	std::map< int, SocketData >										m_mapSocketData;
	//}}

	//{{ 2008. 8. 18  최육사	
	std::map< int, SetItemData >									m_mapSetItemData;	
	//}}

	//{{ 2012. 11. 28 캐시 아이템 소켓 옵션 기능 수정 - 김민성
#ifdef SERV_CASH_ITEM_SOCKET_OPTION
	std::map< int, std::set< int > >								m_mapItemTypeSocketList;				// 아이템 타입, 소켓ID들
	std::map< int, std::set< int > >								m_mapItemGroupSocketList;				// GroupID, 소켓ID들
#endif SERV_CASH_ITEM_SOCKET_OPTION
	//}
	//{{ 2013. 05. 15	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
	std::map< int, KLottery >										m_mapRandomSocketGroup;
	std::map< int, SMiracleCubeLevel >								m_mapMiracleCubeTable;
#endif SERV_NEW_ITEM_SYSTEM_2013_05
	//}}
};

DefRefreshSingletonInline( CXSLSocketItem );


