#pragma once

#include <map>
#include "KncUidType.h"
#include "KncSmartPtr.h"
#include "Enum/Enum.h"


//{{ 2012. 10. 31	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM


#pragma pack( push, 1 )


class KGlobalUserList
{
public:
    struct KGSUserInfo 
    {
    public:
        UidType         m_nUserUID;
        UidType			m_nUnitUID;
        std::wstring	m_wstrUnitNickName;
        UidType         m_nGSUID;
		int				m_iChannelID;
		std::wstring	m_wstrIP;
		char			m_cAuthLevel;

        KGSUserInfo()
        {
            m_nUserUID			= 0;
            m_nUnitUID			= 0;
            m_nGSUID			= 0;
			m_iChannelID		= 0;
			m_cAuthLevel		= SEnum::UAL_NORMAL;
        }

        KGSUserInfo( const UidType iUserUID, const std::wstring& wstrUserID, const UidType iGSUID )
        {
            m_nUserUID			= iUserUID;
            m_nUnitUID			= 0;		
            m_nGSUID			= iGSUID;
			m_iChannelID		= 0;
			m_cAuthLevel		= SEnum::UAL_NORMAL;
            return;
        }

        KGSUserInfo( const KGSUserInfo& t )
        {
            *this = t;
        }

        KGSUserInfo& operator=( const KGSUserInfo& t )
        {	
            m_nUserUID			= t.m_nUserUID;
            m_nUnitUID			= t.m_nUnitUID;
            m_wstrUnitNickName	= t.m_wstrUnitNickName;
            m_nGSUID			= t.m_nGSUID;
			m_iChannelID		= t.m_iChannelID;
			m_cAuthLevel		= t.m_cAuthLevel;
            return *this;
        }
    };

#ifdef	SERV_HERO_PVP_MANAGE_LIST // 적용날짜: 2013-04-04
	typedef	std::vector<KHeroPvpUserInfo>	HERO_PVP_USER_TYPE;
#endif	// SERV_HERO_PVP_MANAGE_LIST

public:
	KGlobalUserList();
	virtual ~KGlobalUserList();
    
    bool	GetGSUserInfoByUnitUID( IN const UidType iUnitUID, OUT KGSUserInfo& kInfo );
#ifdef	SERV_GLOBAL_USER_LIST_NICKNAME_STORAGE // 적용날짜: 2013-04-04
	bool	GetGSUserInfoByUnitNickName( IN const UidType iServerGroupID, IN const std::wstring& wstrUnitNickName, OUT KGSUserInfo& kInfo ) const;
#else	// SERV_GLOBAL_USER_LIST_NICKNAME_STORAGE
    bool	GetGSUserInfoByUnitNickName( IN const std::wstring& wstrUnitNickName, OUT KGSUserInfo& kInfo );
#endif	// SERV_GLOBAL_USER_LIST_NICKNAME_STORAGE
	//{{ 2013. 05. 29	최육사	제재 리스트 통합
#ifdef SERV_BLOCK_LIST
	bool	GetGSUserInfoByUserUID( IN const UidType iUserUID, OUT KGSUserInfo& kInfo );
#endif SERV_BLOCK_LIST
	//}}
    
    void    UnRegAllUserOfThisGS( IN const UidType iGSUID );
    int		RegUnit( IN const KRegUnitInfo& kUnitInfo );
    bool	UnRegUnit( IN const UidType iUnitUID );

#ifdef	SERV_HERO_PVP_MANAGE_LIST // 적용날짜: 2013-04-04
	inline	void	GetHeroPvpUserList( OUT HERO_PVP_USER_TYPE& vecUserList ) const{	vecUserList = m_vecHeroPvpUserList;	}
	int		FindHeroPvpUnit( IN const UidType iServerGroupID, IN const std::wstring wstrNickName ) const;
	
	int		AddHeroPvpUser( IN const UidType iServerGroupID, IN const std::wstring wstrNickName );
	int		RemoveHeroPvpUser( IN const UidType iServerGroupID, IN const std::wstring wstrNickName );

	void	AddHeroPvpUserList( IN const std::vector< std::pair< UidType, std::wstring > >& vecAddUserList
		, OUT std::vector< std::pair< UidType, std::wstring > >& vecAddResultUserList
		);

	void	RemoveHeroPvpUserList( IN const std::vector< std::pair< UidType, std::wstring > >& vecRemoveUserList
		, OUT std::vector< std::pair< UidType, std::wstring > >& vecRemoveResultUserList
		);

	void	HeroPvpUserOn( IN const UidType iServerGroupID, IN const std::wstring wstrNickName );
	void	HeroPvpUserOff( IN const UidType iServerGroupID, IN const std::wstring wstrNickName );

	int		ArgChk_HeroPvpUserServerGroupID( IN const UidType iServerGroupID ) const;
	int		ArgChk_HeroPvpUserNickName( IN const std::wstring wstrNickName ) const;

	void	AddHeroPvpObserver( IN const UidType iAddGSUID, IN const UidType iAddUnitUID );
	void	RemoveHeroPvpObserver( IN const UidType iRemoveUnitUID );

	inline	void	GetHeroPvpUserChangeInfo( OUT std::vector< std::pair< bool, KHeroPvpUserInfo > >& vecHeroPvpUserChangeInfo )
	{
		vecHeroPvpUserChangeInfo = m_vecHeroPvpUserChangeInfo;
		m_vecHeroPvpUserChangeInfo.clear();
	}

	inline	void	GetHeroPvpObserverList( OUT std::vector< std::pair< UidType, UidType > >& vecHeroPvpObserverList ){	vecHeroPvpObserverList = m_vecHeroPvpObserverList;	}
	
	void	SetHeroPvpUserTournamentConnection( IN const UidType iServerGroupID, IN const std::wstring wstrNickName, IN const bool bConnect );
#endif	// SERV_HERO_PVP_MANAGE_LIST

protected:
	std::map< UidType,		boost::shared_ptr< KGSUserInfo > >		m_mapUnitUID;
#ifdef	SERV_GLOBAL_USER_LIST_NICKNAME_STORAGE // 적용날짜: 2013-04-04

#ifdef EXTEND_SERVER_GROUP_MASK
	std::map< int, std::map< std::wstring,	boost::shared_ptr< KGSUserInfo > > >		m_mapUnitNickName;
#else EXTEND_SERVER_GROUP_MASK
	std::map< std::wstring,	boost::shared_ptr< KGSUserInfo > >		m_mapUnitNickName[SEnum::SGI_END];
#endif EXTEND_SERVER_GROUP_MASK

#else	// SERV_GLOBAL_USER_LIST_NICKNAME_STORAGE
	std::map< std::wstring,	boost::shared_ptr< KGSUserInfo > >		m_mapUnitNickName;
#endif	// SERV_GLOBAL_USER_LIST_NICKNAME_STORAGE
	//{{ 2013. 05. 29	최육사	제재 리스트 통합
#ifdef SERV_BLOCK_LIST
	std::map< UidType,		boost::shared_ptr< KGSUserInfo > >		m_mapUserUID;
#endif SERV_BLOCK_LIST
	//}}
#ifdef	SERV_HERO_PVP_MANAGE_LIST // 적용날짜: 2013-04-04
	HERO_PVP_USER_TYPE												m_vecHeroPvpUserList;
	std::vector< std::pair< UidType, UidType > >					m_vecHeroPvpObserverList;
	std::vector< std::pair< bool, KHeroPvpUserInfo > >				m_vecHeroPvpUserChangeInfo;
#endif	// SERV_HERO_PVP_MANAGE_LIST
};

#pragma pack( pop )


#endif SERV_BATTLE_FIELD_SYSTEM
//}}