#pragma once

#include "DungeonRoom.h"

SmartPointer( KTrainingCenterRoom );

//훈련소 방은 던전의 기능을 그대로 가지고
// 게임시작 / 결과처리 부분만을 수정하여
// 개인플레이어 게임처리를 위해 제작한다.

class KTrainingCenterRoom : public KDungeonRoom
{
	NiDeclareRTTI;
	DeclToStringW;

public:
	KTrainingCenterRoom();
	virtual ~KTrainingCenterRoom();

	// override.
	//virtual void Tick();

protected:
	// Get Set
	virtual void SetRoomInfo( const KTCInfo& kInfo );

	// override.
	virtual void ProcessNativeEvent( const KEventPtr& spEvent );
	virtual void StartGame();
	//virtual void StartPlay();
	//virtual void EndPlay();
	//virtual void EndGame();
	//virtual bool CheckIfPlayEnd();
	//virtual int  DecideWinTeam();
	//virtual int  GetPlayMode();
	//virtual void HandleTimeOutUser( std::map< UidType, UidType >& mapUserCIDNGS );
	virtual	void SendRoomListInfo( IN const int iCode ){}	//훈련소는 방리스트가 없기때문에 동작을 안되게 한다.
	virtual float GetRemainingPlayTime(){ return KRoom::GetRemainingPlayTime(); }

	DECL_ON_FUNC( ERM_OPEN_TC_ROOM_REQ );
   _DECL_ON_FUNC( ERM_END_TC_GAME_REQ, KEGS_END_TC_GAME_REQ );
   _DECL_ON_FUNC( ERM_SET_TC_REMAINING_TIME_REQ, KEGS_SET_TC_REMAINING_TIME_REQ );
   //{{ 2007. 10. 23  최육사  
   _DECL_ON_FUNC( ERM_LEAVE_TC_ROOM_REQ, KERM_LEAVE_ROOM_REQ );
   //}}

protected:
	int			m_iTCID;
};




