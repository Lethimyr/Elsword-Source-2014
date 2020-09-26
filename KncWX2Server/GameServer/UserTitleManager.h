#pragma once
#include "Event.h"
#include "CommonPacket.h"

#include "CacheData.h"


SmartPointer(KGSUser);

class KUserTitleManager
{
	enum MAX_NUM
	{
		MAX_SUB_MISSION_NUM = 5,
	};

	//{{ 2009. 10. 26  ÃÖÀ°»ç	PC¹æÀÌº¥Æ®	
	enum PC_BANG_EVENT
	{
        PC_BANG_TITLE_ID = 10390,
	};
	//}}

public:
	KUserTitleManager(void);
	~KUserTitleManager(void);

	void	Clear();
	void	Init( IN bool bIsPcBang, IN OUT std::vector< KMissionInstance >& vecMission, IN OUT std::vector< KTitleInfo >& vecTitle );

	// »õ·Î¿î ¹Ì¼Ç µî·ÏÀ» Ã¼Å©ÇØ¾ß ÇÒ¶§ - ·¹º§¾÷, ¹Ì¼ÇÅ¬¸®¾î½Ã
	void	CheckNewMission( IN KGSUserPtr spUser, IN bool bFirst = false );
	KMissionInstance* GetMissionInstance( IN int iMissionID );

	// Å¬¸®¾î Á¶°Çº° Ã¼Å©ÇÔ¼ö
	void	OnNpcUnitDie( IN const int iDungeonID, IN const char cDifficulty, IN const char cDungeonMode, IN const int iMonsterID, IN KGSUserPtr spUser );
	void	OnTalkWithNpc( IN int iNPCID, IN KGSUserPtr spUser );
	//{{ 2010. 8. 23	ÃÖÀ°»ç	Çï¸ðµå ÄªÈ£
#ifdef SERV_HELL_MODE_TITLE
	void	OnDungeonClear( IN KGSUserPtr spUser, IN int iDungeonID, IN char cDifficulty, IN char cDungeonMode, IN char cRank, IN int iPlayTime, IN int iDamage, IN const std::set< UidType >& setPartyMembers );
#else
	void	OnDungeonClear( IN KGSUserPtr spUser, IN int iDungeonID, IN char cDifficulty, IN char cRank, IN int iPlayTime, IN int iDamage, IN const std::set< UidType >& setPartyMembers );
#endif SERV_HELL_MODE_TITLE
	//}}
	void	OnQuestComplete( IN int iQuestID, IN KGSUserPtr spUser );
	void	OnUseItem( IN const int iItemID, IN KGSUserPtr spUser );

	//void	OnPVPPlay( IN int iGameType, IN KGSUserPtr spUser );
	//void	OnPVPWin( IN int iGameType, IN KGSUserPtr spUser );
	//void	OnPVPKill( IN int iGameType, IN KGSUserPtr spUser );

	// ¹Ì¼Ç ¿Ï·á ÇÔ¼ö
	void	CheckCompleteMission( IN int iMissionID, IN KGSUserPtr spUser );
	void	DeleteMissionInstance( IN int iMissionID );

	// Å¸ÀÌÆ² Å»Âø ÇÔ¼ö
	bool	IsExistTitle( IN int iTitleID );
	bool	AddTitle( IN int iTitleID, IN std::wstring&	wstrEndDate, OUT __int64& tCTime, OUT bool& bInfinity );
	//{{ 2010. 11. 17	ÃÖÀ°»ç	ÄªÈ£ µ¥ÀÌÅÍ Å©±â ´ÃÀÌ±â
#ifdef SERV_TITLE_DATA_SIZE
	int		GetEquippedTitleID() const { return m_iEquippedTitle; }
	void	SetEquippedTitleID( int iTitleID ) { m_iEquippedTitle.SetValue( iTitleID ); }
#else
	short	GetEquippedTitleID() const { return static_cast<short>(m_iEquippedTitle); }
	void	SetEquippedTitleID( short sTitleID ) { m_iEquippedTitle.SetValue( sTitleID ); }
#endif SERV_TITLE_DATA_SIZE
	//}}	
	bool	EquipTitle( IN int iTitleID );
	//{{ 2010. 10. 18	ÃÖÀ°»ç	Æê ¿À¶ó ½ºÅ³
#ifdef SERV_PET_AURA_SKILL
		//{{ 2011. 07. 08    ±è¹Î¼º    ¿É¼Ç ¼öÄ¡È­
#ifdef SERV_USE_PERCENT_IN_OPTION_DATA
	void	GetEquippedStat( IN bool bIsDungeon, IN const KStat& kBaseStat, IN OUT float& fTotalIncHPRate, OUT KStat& kAddStat, IN unsigned int& uiHP_OnePoint, IN unsigned int& uiUnitLevel );
#else
	void	GetEquippedStat( IN bool bIsDungeon, IN const KStat& kBaseStat, IN OUT float& fTotalIncHPRate, OUT KStat& kAddStat );
#endif SERV_USE_PERCENT_IN_OPTION_DATA
		//}} 
#else
	void	GetEquippedStat( IN UINT uiBaseHP, OUT KStat& kStat, IN OUT float& fTotalIncHPRate, IN bool bIsDungeon );
#endif SERV_PET_AURA_SKILL
	//}}
	void	GetEquippedTitleBonusRate( IN const bool bIsDungeon, OUT float& fAddTitleExpRate, OUT float& fAddTitleEDRate );
	void	ExpireTitle( IN KGSUserPtr spUser );

	// DB ¾÷µ¥ÀÌÆ® ÇÔ¼ö
	void	AddUpdateMission( IN int iMissionID );
	//{{ 2010. 11. 17	ÃÖÀ°»ç	ÄªÈ£ µ¥ÀÌÅÍ Å©±â ´ÃÀÌ±â
#ifdef SERV_TITLE_DATA_SIZE
	void	GetUpdateInfo( IN bool bIsPcBang, OUT int& iInitTitleID, OUT int& iEquippedTitleID, OUT std::vector< KMissionUpdate >& vecMissionData );
#else
	void	GetUpdateInfo( IN bool bIsPcBang, OUT short& sInitTitleID, OUT short& sEquippedTitleID, OUT std::vector< KMissionUpdate >& vecMissionData );
#endif SERV_TITLE_DATA_SIZE
	//}}	

	//{{ 2011. 05. 16  ±è¹Î¼º	ÄªÈ£ È¹µæ Á¶°Ç Ãß°¡
#ifdef SERV_ADD_TITLE_CONDITION
	bool	GivePetFeed( IN const CXSLItem::ITEM_GRADE eGrade, IN KGSUserPtr spUser );
	void	UseResurrectionStone( IN bool bIsUse )			{ m_bUsedResurrectionStone = bIsUse; }
	bool	IsUseResurrectionStone()						{ return m_bUsedResurrectionStone; }
	void	DeleteUpdateMission( IN OUT std::vector< KMissionInstance >& vecMission , IN int iMissionID );
	void	SetStartedNumMember( IN int& iStartedNumMember )	{ m_iStartedNumMember = iStartedNumMember; }
	int		GetStartedNumMember()								{ return m_iStartedNumMember; }
//{{ 2011. 09. 19  ±è¹Î¼º	ÄªÈ£ È¹µæ Á¶°Ç Ãß°¡
	void	OnUserUnitDie( IN KEGS_USER_UNIT_DIE_REQ::USER_UNIT_DIE_REASON eReason, IN KGSUserPtr spUser );
//}}
#endif SERV_ADD_TITLE_CONDITION
	//}}
	//{{ 2013. 02. 05  ÄªÈ£ È¹µæ Á¶°Ç Ãß°¡(ºÎÈ°¼® »ç¿ë È½¼ö, »÷´õ ¸¶À») - ±è¹Î¼º
#ifdef SERV_ADD_TITLE_CONDITION_SANDER
	void	ClearUseResurrectionStoneCount()							{ m_iUseResurrectionStoneCount = 0; }
	void	AddUseResurrectionStoneCount()								{ ++m_iUseResurrectionStoneCount; }
	int		GetUseResurrectionStoneCount()								{ return m_iUseResurrectionStoneCount; }
#endif SERV_ADD_TITLE_CONDITION_SANDER
	//}

#ifdef SERV_ADD_TITLE_CONDITION_2013_08		// Àû¿ë³¯Â¥: 2013-08-13
	void	OnSocketItem( IN KGSUserPtr spUser, IN int iItemLevel, IN int iSocketUseCount );								// ¸¶¹ý¼® »ç¿ë
	void	OnEnchantItemLevel( IN KGSUserPtr spUser, IN int iItemLevel, IN int iEnchantLevel, IN bool bEnchantResult );	// °­È­ ·¹º§, °­È­ È½¼ö
	void	OnAttribItem( IN KGSUserPtr spUser, IN int iItemLevel );								// ¼Ó¼º ºÎ¿©
	void	OnResolveItem( IN KGSUserPtr spUser );		
#endif // SERV_ADD_TITLE_CONDITION_2013_08

private:
	KCacheData< int >					m_iEquippedTitle;

	std::map< int, KMissionInstance >	m_mapMission;
	std::map< int, KTitleInfo >			m_mapTitle;

	std::set< int >						m_setDBupdateMission;

	//{{ 2011. 05. 16  ±è¹Î¼º	ÄªÈ£ È¹µæ Á¶°Ç Ãß°¡
#ifdef SERV_ADD_TITLE_CONDITION
	bool								m_bUsedResurrectionStone;
	int									m_iStartedNumMember;
#endif SERV_ADD_TITLE_CONDITION
	//}}
	//{{ 2013. 02. 05  ÄªÈ£ È¹µæ Á¶°Ç Ãß°¡(ºÎÈ°¼® »ç¿ë È½¼ö, »÷´õ ¸¶À») - ±è¹Î¼º
#ifdef SERV_ADD_TITLE_CONDITION_SANDER
	int									m_iUseResurrectionStoneCount;
#endif SERV_ADD_TITLE_CONDITION_SANDER
	//}
};


