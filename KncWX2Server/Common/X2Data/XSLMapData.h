#pragma once

#include "XSLStat.h"
#include "RefreshSingleton.h"

class CXSLMapData
{
	DeclareRefreshSingleton( CXSLMapData );
	DeclareLuaScriptParser;
	DeclDumpToLogFile;
	DeclDump;
	DeclToStringW;

public:
	struct sMAP_DATA
	{
		int					m_iID;

		int					m_iBeforeID;
		int					m_iAfterID;

		int					m_iRequireMinLevel;
		int					m_iRequireMaxLevel;

		std::vector< int >	m_vecRequireDungeonID;

		sMAP_DATA()
		{
			m_iID				= -1;
			m_iBeforeID			= -1;
			m_iAfterID			= -1;
			m_iRequireMinLevel	= -1;
			m_iRequireMaxLevel	= -1;
		}
	};

	//{{ 2012. 08. 29	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	struct SLocalMapInfo
	{
		std::vector< KDungeonGameSimpleInfo >	m_vecDungeonInfo;
	};
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

public:
	CXSLMapData(void);
	virtual ~CXSLMapData(void);
	
	void	AddMapData_LUA();
	//{{ 2012. 08. 29	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	void	AddLocalMapInfo_LUA( int iVillageMapID, int iDungeonMode, int iDungeonIDWithDif );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	int		GetDefaultMapID() { return m_kDefaultMapData.m_iID; }
	//{{ 2009. 3. 30  최육사	이전 mapid
	int		GetBeforeMapID( int iMapID );
	//}}
	//{{ 2012. 11. 13	최육사		입장 가능한 마을 검사 및 보정
#ifdef SERV_CHECK_POSSIBLE_ENTER_VILLAGE
	int		GetPossibleEnterVillageMapID( IN const u_char ucLevel, IN const std::map< int, KDungeonClearInfo >& mapClearInfo );
#endif SERV_CHECK_POSSIBLE_ENTER_VILLAGE
	//}}

	// 존재하는 마을인지.?
	bool	CheckMapData( const int iMapID );

	// 입장조건 체크
	//{{ 2012. 09. 21   김민성   마을 입장 오류 메시지 구분
#ifdef SERV_ENTER_VILLAGE_ERROR
	bool	CheckRequireData( IN const int iMapID, IN const u_char ucLevel, IN const std::map< int, KDungeonClearInfo >& mapClearInfo, OUT int& iLimitLevel, OUT int& iRequireDungeonID );
#else
	bool	CheckRequireData( const int iMapID, u_char ucLevel, std::map< int, KDungeonClearInfo >& mapClearInfo );
#endif SERV_ENTER_VILLAGE_ERROR
	//}}
	
	//{{ 2008. 11. 5  최육사	필드 정보
	void	UpdateFieldInfo( int iMapID, UidType iFieldUID );
	bool	GetFieldUID( int iMapID, UidType& iFieldUID );
	bool	VerifyMapID( int iMapID );
	const std::map< int, UidType >& GetFieldInfo() const { return m_mapFieldInfo; }
	//}}

	//{{ 2012. 05. 16	김민성       케릭터 선택시 레벨에 따라 맵ID 를 변환
#ifdef SERV_SELECT_UNIT_CHANGE_MAP_ID
	void	CheckMapID( IN unsigned char& ucLevel, IN OUT int& iMapID );
#endif SERV_SELECT_UNIT_CHANGE_MAP_ID
	//}}

	//{{ 2012. 08. 29	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	const SLocalMapInfo* GetLocalMapList( IN const int iVillageMapID ) const;
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	// lastposition packing and unpacking
	static void PackingLastPosition( IN const int iMapID, IN const u_char ucLastTouchLineIndex, IN const u_short usLastPosValue, OUT int& iLastPosition );
	static void UnpackingLastPosition( IN const int iLastPosition, OUT UINT& uiMapID, OUT u_char& ucLastTouchLineIndex, OUT u_short& usLastPosValue );

	static	int GetMapID( int iDungeonID );
	static bool IsDungeonGate( int iMapID );

	//{{ 2012. 10. 31	박세훈	코보 익스프레스 티켓 추가
#ifdef SERV_ADD_COBO_EXPRESS_TICKET
	int		CheckCOBOExpressTicketMapID( IN int iMapID );
	int		ComputeCOBOExpressTicketCost( IN int iSrcMapID, IN int iDesMapID, IN u_char ucLevel );
#endif SERV_ADD_COBO_EXPRESS_TICKET
	//}}

protected:
	const sMAP_DATA*	GetMapData( const int iMapID );

protected:
	std::map< int, sMAP_DATA >			m_mapMapData;
	sMAP_DATA							m_kDefaultMapData;
	
	//{{ 2008. 11. 5  최육사	필드 정보
	std::map< int, UidType >			m_mapFieldInfo;
	//}}
	//{{ 2012. 08. 29	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM	
	std::map< int, SLocalMapInfo >		m_mapLocalMap;
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
};

DefRefreshSingletonInline( CXSLMapData );


