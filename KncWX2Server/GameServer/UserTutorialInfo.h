#pragma once

#include <Windows.h>
#include <atltime.h>
#include "CommonPacket.h"

SmartPointer(KGSUser);

//## des : 개인 사제정보 관리를 해주는 클래스.

class KUserTutorialInfo
{
public:
	enum TUTORIAL_UNIT_TYPE
	{
		TUT_NONE = -1,
		TUT_TEACHER,
		TUT_STUDENT,
		TUT_TOTAL_NUM,
	};

	//struct USER_TUTORIAL_INFO
	//{
	//	KTutorialUnitInfo	m_kUnitInfo;
	//	CTime				m_LastDate;
	//};

public:
	KUserTutorialInfo(void);
	~KUserTutorialInfo(void);

	TUTORIAL_UNIT_TYPE	GetTutorUnitType(){ return m_TutorUnitType; }

	bool	IsExistUnit( UidType iUnitUID );
	bool	IsExistbyTutorData();
	int		GetTutorialUnitNum();
	void	GetTutorUnitUIDList( std::vector<UidType>& vecUIDList );

	bool	OnSelectUnitAck( KGSUserPtr spUser, std::vector<KTutorialDBUnitInfo>& vecTutorialUnitInfo );
	bool	OnDelTutorialReq( KGSUserPtr spUser, KEGS_DEL_TUTORIAL_REQ& kInfo );
	bool	OnDelTutorialAck( KGSUserPtr spUser, KDBE_DEL_TUTORIAL_ACK& kInfo );
	bool	OnDelTutorialNot( KGSUserPtr spUser, KEGS_DEL_TUTORIAL_NOT& kInfo );
	bool	OnTutorialUpdateUnitInfoNot( KGSUserPtr spUser, KEGS_TUTORIAL_UPDATE_UNIT_INFO_NOT& kInfo );
	void	OnDeleteUnitReq( UidType iDeleteUnitUID ){ m_iDeleteUnitUID = iDeleteUnitUID; }
	void	OnDeleteUnitAck( KGSUserPtr spUser );	//유닛삭젯할대 모든 사제데이터를 정리함.

	void	DisconnectUpdateUnitInfo( KGSUserPtr spUser );
	void	OnInsertTutorialAck( KGSUserPtr spUser, KTutorialUnitInfo& kInfo );

	//캐릭터 레벨업시 호출된다.
	void	ProcCharLevelUP( u_char ucBeforeCharLv, KGSUserPtr spUser );

#ifdef SERV_JUMPING_CHARACTER// 작업날짜: 2013-07-12	// 박세훈
	void	ProcJumpingCharLevelUP( IN const u_char ucBeforeCharLv, IN const KGSUserPtr spUser );
#endif // SERV_JUMPING_CHARACTER

private:
	void	SetTutorUnitType( TUTORIAL_UNIT_TYPE eType ){ m_TutorUnitType = eType; }
	void	SendUpdateUnitInfo( KGSUserPtr spUser, UidType iReceiverUID, bool bIsOnline = true, bool bIsReply = false );
	void	ResetData();

protected:
	std::vector<KTutorialUnitInfo>	m_vecTutorInfo;
	TUTORIAL_UNIT_TYPE	m_TutorUnitType;

	UidType		m_iDeleteUnitUID;	//유닛삭제시 사용될 임시 uid
};


