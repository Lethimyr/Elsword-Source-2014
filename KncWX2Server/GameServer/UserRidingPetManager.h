#pragma once
#include "ServerDefine.h"

#ifdef	SERV_RIDING_PET_SYSTM// 적용날짜: 2013-04-20
#include "UserRidingPet.h"
#include <vector>

class KUserRidingPetManager
{
public:
	enum UserRidingPetManagerResult
	{
		RIDING_PET_SUCCEED				= 0,
		RIDING_PET_FAILED				= 1,
		RIDING_PET_NOT_INITED			= 2,
		RIDING_PET_CREATE_COUNT_LIMITED	= 3,
		RIDING_PET_ALREADY_INITED		= 4,
		RIDING_PET_ALREADY_SUMMONED		= 5,
		RIDING_PET_NOT_EXIST			= 6,
		RIDING_PET_SOMETHING_SUMMONED	= 7,
		RIDING_PET_ALREADY_UNSUMMONED	= 8,
		RIDING_PET_NOT_SUMMONED			= 9,
		RIDING_PET_EMPTY				= 10,
		RIDING_PET_INCORRECTED_STAMINA	= 11,
	};

public:
	KUserRidingPetManager(void);
	~KUserRidingPetManager(void);

	void	Clear( void );
	int		Init( IN const std::vector<KRidingPetInfo>& vecRidingPetList, IN const bool bForce = false );
	int		GetUserRidingPetList( IN const byte byteViewPage, OUT std::vector<KRidingPetInfo>& vecRidingPetList );
	int		SummonRidingPet( IN const UidType iRidingPetUID, OUT KRidingPetInfo& kRidingPetInfo );
	int		UnSummonRidingPet( IN const UidType iRidingPetUID, IN const float fStamina );
	int		RidingPetCreateCheck( void ) const;
	int		AddRidingPet( IN const KRidingPetInfo& kRidingPetInfo, IN const bool bCreated = false );
	int		GetRidingPet( IN const UidType iRidingPetUID, OUT KUserRidingPetPtr& spRidingPet );
	int		GetFirstRidingPet( OUT KUserRidingPetPtr& spRidingPet );
	int		Sort( void );
	void	GetDBUpdateInfo( OUT std::vector<KRidingPetInfo>& vecRidingPetList );
	void	DBUpdateFailedProcess( IN const std::vector<UidType> vecRidingPetUID );
	void	OnDeselectUnit( void );
	int		ReleaseRidingPetCheck( IN const UidType iRidingPetUID );
	int		ReleaseRidingPet( IN const UidType iRidingPetUID );
	int		GetSummonedRidingPetInfo( OUT UidType& iRidingPetUID, OUT USHORT& usRidingPetID );
	int		GetExpirationPetUID( OUT std::vector<UidType>& vecRidingPetUID );
	int		IsExpirationRidingPet( IN const UidType iRidingPetUID, OUT USHORT& usRidingPetID );
	int		UpdateSummonedRidingPetInfo( IN const UidType iRidingPetUID, IN const float fStamina );

#if defined(SERV_EVENT_RIDING_WITH_SUB_QUEST) || defined(SERV_RIDING_PET_WITH_SUB_QUEST)
	KUserRidingPetPtr GetRidingPetSummoned() {return m_spSummoned;}
#endif //SERV_EVENT_RIDING_WITH_SUB_QUEST

protected:
	bool	InitCheck( void ) const{	return m_bInit;	}
	void	SetInit( IN const bool bInit ){	m_bInit = bInit;	}
	int		GetRidingPetNum( void ) const{	return static_cast<int>( m_vecRidingPetList.size() );	}

private:
	bool							m_bInit;
	std::vector<KUserRidingPetPtr>	m_vecRidingPetList;		// 라이딩 펫 컨테이너
	KUserRidingPetPtr				m_spSummoned;
};
#endif	// SERV_RIDING_PET_SYSTM
