#pragma once
#include "ServerDefine.h"

#ifdef	SERV_RIDING_PET_SYSTM// 적용날짜: 2013-04-21
#include "RefreshSingleton.h"

class CXSLRidingPetManager
{
	DeclareRefreshSingleton( CXSLRidingPetManager );
	DeclareLuaScriptParser;
	DeclDumpToLogFile;
	DeclDump;
	DeclToStringW;

public:
	enum RIDING_PET_UNIT_ID
	{
		RPUI_NONE				= 0,
		RPUI_NASOD_MOBI			= 1,	/// 나소드 모비
		RPUI_KOUCIKA			= 2,	/// 코우시카
		RPUI_ANCIENT_PPORU		= 3,	/// 에인션트 뽀루
		RPUI_SCORPION_TYPE_R	= 4,	/// 퐁고 스콜피온 TYPE-R
		RPUI_ICE_HAMELING		= 5,	/// 아이스 하멜링
		RPUI_DARK_HAMELING		= 6,	/// 다크 하멜링 
		RPUI_RED_HAMELING		= 7,	/// 레드 하멜링 
		
		// 기획상 변경된 능력치를 해외팀에 반영하지 않기 위해 추가 한 ID ~
		RPUI_NASOD_MOBI_INT     = 8,    /// 고대의 돌진 전차 모비RT
		RPUI_KOUCIKA_INT        = 9,    /// 고대의 바람의 코우시카
		RPUI_ANCIENT_PPORU_INT  = 10,   /// 고대의 에이션트 라이딩 뽀루
		// ~ 기획상 변경된 능력치를 해외팀에 반영하지 않기 위해 추가 한 ID

		RPUI_OBERON_CUSTOM_650	= 11,	///	이브 커스텀-오베론650
		RPUI_ANCIENT_PPORU_NAVER = 12,	///	네이버 채널링 이벤트 에이션트 뽀루

		RPUI_OBERON_CUSTOM_650_BLACK_SKULL = 13,	/// 이브 커스텀 - 블랙 스컬
		RPUI_OBERON_CUSTOM_650_ANGEL_HEART = 14,	/// 이브 커스텀 - 엔젤 하트

		RPUI_END,
	};

	enum RIDING_PET_ENUM
	{
		RPE_MAX_PET_COUNT	= 30,
		RPE_PAGE_PER_COUNT	= 6,	// 음수를 넣을 경우 정상 동작을 보장하지 못한다.
	};

	struct RidingPetTemplet
	{
		RIDING_PET_UNIT_ID	m_iRidingPetID;
		USHORT				m_usMaxStamina;
		float				m_fStaminaRecoveryRate;
	};

public:
	CXSLRidingPetManager(void);
	~CXSLRidingPetManager(void);

	// for lua
	bool	AddRidingPetTemplet_LUA();
	bool	AddRidingPetCreateItemInfo_LUA( int iItemID, USHORT usRidingPetID, short sPeriod );
	void	SetEnableStamina_LUA( float fEnableStamina ){	m_fEnableStamina = fEnableStamina;	}
	float	GetEnableStamina( void ) const{	return m_fEnableStamina;	}
	
	// function
	const RidingPetTemplet*	GetRidingPetTemplet( IN const USHORT usRidingPetID ) const;

	bool	IsExistRidingPetID( IN const USHORT usiRidingPetID ) const{	return ( m_mapRidingPetTemplet.find( static_cast<RIDING_PET_UNIT_ID>( usiRidingPetID ) ) != m_mapRidingPetTemplet.end() );	}
	void	GetRidingPetID( IN const int iItemID, OUT USHORT& usCreatePetID, OUT short& sPeriod ) const;
	
	float	GetRecoveryTimePerOne( void );
	
	// static

private:
	std::map< RIDING_PET_UNIT_ID, RidingPetTemplet >		m_mapRidingPetTemplet;			// 라이딩 펫 데이터
	std::map< int, std::pair<RIDING_PET_UNIT_ID, short> >	m_mapRidingPetCreateItem;		// 라이딩 펫 생성 아이템 리스트
	float													m_fEnableStamina;				// 현재 스태미너 수치에 따른 사용 가능 여부 기준
};

DefRefreshSingletonInline( CXSLRidingPetManager );
#endif	// SERV_RIDING_PET_SYSTM