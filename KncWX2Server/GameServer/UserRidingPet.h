#pragma once
#include "Event.h"
#include "CommonPacket.h"

//{{ 2010. 01. 15  최육사	boost allocator
#ifdef SERV_BOOST_ALLOCATOR
	#include <ObjectPool/BoostAllocator.h>
#endif SERV_BOOST_ALLOCATOR
//}}

#ifdef	SERV_RIDING_PET_SYSTM// 적용날짜: 2013-04-20
#define KUserRidingPetPtr KUserRidingPet*
#undef KUserRidingPetPtr
SmartPointer( KUserRidingPet );

//////////////////////////////////////////////////////////////////////////
#ifdef SERV_BOOST_ALLOCATOR
BOOST_ALOC_CLASS( KUserRidingPet )
#else
class KUserRidingPet
#endif SERV_BOOST_ALLOCATOR
	//////////////////////////////////////////////////////////////////////////
{
public:
	KUserRidingPet( IN KRidingPetInfo kInfo, IN const bool bCreated );
	~KUserRidingPet(void);

	void	SetData( IN const KRidingPetInfo& kInfo, IN const bool bChanged = true )
	{
		m_kInfo		= kInfo;
		SetChanged( bChanged );
	}

	void	GetData( OUT KRidingPetInfo& kInfo ) const{	kInfo = m_kInfo;	}

	UidType	GetUID( void ) const{	return m_kInfo.m_iRidingPetUID;	}
	USHORT	GetID( void ) const{	return m_kInfo.m_usRindingPetID;	}
	bool	EnableCheck( void ) const;
	__int64	GetLastUnSummonDate( void ) const{	return m_kInfo.m_iLastUnSummonDate;	}

	void	SetChanged( IN const bool bChanged ){	m_bChanged = bChanged;	}
	bool	GetChanged( void ) const{	return m_bChanged;	}
	bool	IsExpirationRidingPet( void ) const;
	bool	CheckRidingPetStamina( IN const float fStamina );

private:
	bool			m_bChanged;
	KRidingPetInfo	m_kInfo;
};
#endif	// SERV_RIDING_PET_SYSTM
