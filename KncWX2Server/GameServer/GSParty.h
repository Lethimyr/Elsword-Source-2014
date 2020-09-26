#pragma once

#include <KncSmartPtr.h>
#include "Event.h"
#include "CommonPacket.h"
//{{ 2010. 01. 15  최육사	boost allocator
#ifdef SERV_BOOST_ALLOCATOR
	#include <ObjectPool/BoostAllocator.h>
#endif SERV_BOOST_ALLOCATOR
//}}


//////////////////////////////////////////////////////////////////////////
#ifdef SERV_CHANNEL_PARTY
//////////////////////////////////////////////////////////////////////////


#define KGSPartyPtr KGSParty*
#undef KGSPartyPtr
SmartPointer( KGSParty );

//////////////////////////////////////////////////////////////////////////
#ifdef SERV_BOOST_ALLOCATOR
BOOST_ALOC_CLASS( KGSParty )
#else
class KGSParty
#endif SERV_BOOST_ALLOCATOR
//////////////////////////////////////////////////////////////////////////
{
public:
	KGSParty( const KPartyInfo& kInfo );
    virtual ~KGSParty();

	// 정보 저장 함수
	void	SetInfo( IN const KPartyInfo& kInfo );
	void	AddMember( IN const KPartyUserInfo& kUserInfo );
	void	DeleteMember( IN UidType nCID );

	// 정보 획득 함수
	void	GetInfo( OUT KPartyInfo& kInfo );

	//{{ 2011. 06. 21	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
	char	GetPartyType()						{ return m_kPartyInfo.m_cPartyType; }
#endif SERV_PVP_NEW_SYSTEM
	//}}
	UidType GetPartyUID()						{ return m_kPartyInfo.m_iPartyUID; }
	const std::wstring& GetPartyName()			{ return m_kPartyInfo.m_wstrPartyName; }
	int		GetDungeonID()						{ return m_kPartyInfo.m_iDungeonID; }
	char	GetDungeonDif()						{ return m_kPartyInfo.m_DifficultyLevel; }
	char	GetDungeonMode()					{ return m_kPartyInfo.m_cDungeonMode; }
	//{{ 2012. 02. 28	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	short	GetPartyFever() const 				{ return m_kPartyInfo.m_sPartyFever; }
#else
	u_char	GetPartyFever()						{ return m_kPartyInfo.m_ucPartyFever; }
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}	

	bool	IsMember( IN UidType iUnitUID )		{ return ( m_mapMember.find( iUnitUID ) != m_mapMember.end() ); }	
	int		GetMaxMember()						{ return static_cast<int>(m_kPartyInfo.m_MaxSlot); }
	int		GetNumMember()						{ return static_cast<int>(m_mapMember.size()); }

	UidType GetHostUID();

	void	GetListUserInfoList( OUT std::vector< KPartyListUserInfo >& vecUserInfo );

	float	GetPartyFeverExpRate();	

	//{{ 2012. 02. 28	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	void	SetPartyFever( IN const short sPartyFever )	{ m_kPartyInfo.m_sPartyFever = sPartyFever; }
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	// 파티 멤버 정보 갱신
	void	UpdatePartyUserInfo( IN const KPartyUserInfo& kInfo );

	//{{ 2011. 03. 21  김민성	던전 난이도 보정
#ifdef SERV_DUNGEON_DIFFICULTY_REVISION
	bool	GetCheckLowLevel()							{ return m_kPartyInfo.m_bCheckLowLevel; }
	bool	CheckPartyMinLevel( IN const int& iDungeonMinLevel );
#endif SERV_DUNGEON_DIFFICULTY_REVISION
	//}}

protected:
	KPartyInfo								m_kPartyInfo;
    std::map< UidType, KPartyUserInfo >		m_mapMember;
};


//////////////////////////////////////////////////////////////////////////
#endif SERV_CHANNEL_PARTY
//////////////////////////////////////////////////////////////////////////

