#pragma once
#include "SimLayer.h"

#include "Event.h"
#include "CommonPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"

//{{ 2012. 10. 31	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	#include "GlobalUserList.h"
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

class KGlobalSimLayer : public KSimLayer
{
    NiDeclareRTTI;
    DeclToStringW;
    DeclDump;

public:
    KGlobalSimLayer(void);
    virtual ~KGlobalSimLayer(void);

    // derived from KSimLayer
    virtual void Init();
    virtual void Tick();
    virtual void ShutDown();
    virtual void RegToLua();
    
	//{{ 2012. 10. 31	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
    bool	GetGSUserInfoByUnitUID( IN const UidType iUnitUID, OUT KGlobalUserList::KGSUserInfo& kInfo );
#ifdef	SERV_GLOBAL_USER_LIST_NICKNAME_STORAGE // 적용날짜: 2013-04-04
	bool	GetGSUserInfoByUnitNickName( IN const UidType iServerGroupID, IN const std::wstring& wstrUnitNickName, OUT KGlobalUserList::KGSUserInfo& kInfo );
#else	// SERV_GLOBAL_USER_LIST_NICKNAME_STORAGE
    bool	GetGSUserInfoByUnitNickName( IN const std::wstring& wstrUnitNickName, OUT KGlobalUserList::KGSUserInfo& kInfo );
#endif	// SERV_GLOBAL_USER_LIST_NICKNAME_STORAGE
	//{{ 2013. 05. 29	최육사	제재 리스트 통합
#ifdef SERV_BLOCK_LIST
	bool	GetGSUserInfoByUserUID( IN const UidType iUserUID, OUT KGlobalUserList::KGSUserInfo& kInfo );
#endif SERV_BLOCK_LIST
	//}}

    void    UnRegAllUserOfThisGS( IN const UidType iGSUID );
    int		RegUnit( IN const KRegUnitInfo& kUnitInfo );
    void	UnRegUnit( IN const UidType iUnitUID );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

#ifdef	SERV_HERO_PVP_MANAGE_LIST // 적용날짜: 2013-04-04
	inline	void	GetHeroPvpUserList( OUT KGlobalUserList::HERO_PVP_USER_TYPE& vecUserList ) const{	m_kUserList.GetHeroPvpUserList( vecUserList );	}
	inline	int		FindHeroPvpUnit( IN const UidType iServerGroupID, IN const std::wstring wstrNickName ) const{	return m_kUserList.FindHeroPvpUnit( iServerGroupID, wstrNickName );	}
	
	inline	void	AddHeroPvpUserList( IN const std::vector< std::pair< UidType, std::wstring > >& vecAddUserList
		, OUT std::vector< std::pair< UidType, std::wstring > >& vecAddResultUserList
		)
	{
		m_kUserList.AddHeroPvpUserList( vecAddUserList, vecAddResultUserList );
	}

	inline	void	RemoveHeroPvpUserList( IN const std::vector< std::pair< UidType, std::wstring > >& vecRemoveUserList
		, OUT std::vector< std::pair< UidType, std::wstring > >& vecRemoveResultUserList
		)
	{
		m_kUserList.RemoveHeroPvpUserList( vecRemoveUserList, vecRemoveResultUserList );
	}

	inline	void	GetHeroPvpUserChangeInfo( OUT std::vector< std::pair< bool, KHeroPvpUserInfo > >& vecHeroPvpUserChangeInfo ){	m_kUserList.GetHeroPvpUserChangeInfo( vecHeroPvpUserChangeInfo );	}
	inline	void	GetHeroPvpObserverList( OUT std::vector< std::pair< UidType, UidType > >& vecHeroPvpObserverList ){	m_kUserList.GetHeroPvpObserverList( vecHeroPvpObserverList );	}

	inline	void	SetHeroPvpUserTournamentConnection( IN const UidType iServerGroupID, IN const std::wstring wstrNickName, IN const bool bConnect ){	m_kUserList.SetHeroPvpUserTournamentConnection( iServerGroupID, wstrNickName, bConnect );	}
#endif	// SERV_HERO_PVP_MANAGE_LIST

	//{{ 임홍락 [2012.05.16] 서버 단일화 // SERV_USE_NATION_FLAG 필수 포함임
#ifdef SERV_UNITED_SERVER_EU
	std::string GetStrPvpNpcDataLua();
#endif SERV_UNITED_SERVER_EU
	//}}
protected:
	//{{ 2012. 10. 31	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	KGlobalUserList			m_kUserList;
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
};

DefKObjectInline( KGlobalSimLayer, KSimLayer );


