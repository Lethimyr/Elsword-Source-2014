#include "GSUser.h"
#include <dbg/dbg.hpp>
#include "GSFSM.h"
#include "NetError.h"

#include "BaseServer.h"

#include "Enum/Enum.h"
#include "X2Data/XSLGame.h"
#include "X2Data/XSLStringFilter.h"

//{{ 2009. 7. 16  최육사	채널매니저
#include "ChannelManager.h"
//}}

#include "odbc/Odbc.h"

//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
	#include "WeddingHallListManager.h"
	#include "GSWeddingHall.h"
#endif SERV_RELATIONSHIP_SYSTEM
//}

#ifdef SERV_STRING_FILTER_USING_DB
	#include "StringFilterManager.h"
#endif //SERV_STRING_FILTER_USING_DB

#define CLASS_TYPE      KGSUser

//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
IMPL_ON_FUNC( EGS_COUPLE_PROPOSE_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	// 마을, 배틀필드, 휴식처 에서만 신청 가능하다
	if( GetStateID() == KGSFSM::S_ROOM )
	{
		switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
		{
		case CXSLRoom::RT_BATTLE_FIELD:
			{
				// 신청 가능한 룸상태 이므로 통과
			}break;
		default:
			{
				KEGS_COUPLE_PROPOSE_ACK kAck;
				kAck.m_iOK = NetError::ERR_RELATIONSHIP_00;
				SendPacket( EGS_COUPLE_PROPOSE_ACK, kAck );
				return;
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// 조건 검사
	//////////////////////////////////////////////////////////////////////////

	// 정상적인 문자열인지 검사
	char charBuf[255] = {0};
	std::string strUnitName;

	WideCharToMultiByte( CP_ACP, 0, kPacket_.m_wstrUnitName.c_str(), -1, charBuf, 255, NULL, NULL );
	strUnitName = charBuf;

	if( strUnitName.empty() == true || strUnitName.size() > 12 )   // 비어있는 값인지 확인
	{
		KEGS_COUPLE_PROPOSE_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_RELATIONSHIP_01;
		SendPacket( EGS_COUPLE_PROPOSE_ACK, kPacket );
		return;
	}

#ifdef SERV_STRING_FILTER_USING_DB
	if( SiKStringFilterManager()->CheckIsValidString( CXSLStringFilter::FT_NICKNAME, kPacket_.m_wstrUnitName ) == false )
#else //SERV_STRING_FILTER_USING_DB
	if( SiCXSLStringFilter()->CheckIsValidString( CXSLStringFilter::FT_NICKNAME, kPacket_.m_wstrUnitName ) == false )
#endif //SERV_STRING_FILTER_USING_DB
	{
		KEGS_COUPLE_PROPOSE_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_RELATIONSHIP_01;
		SendPacket( EGS_COUPLE_PROPOSE_ACK, kPacket );
		return;
	}

	// 쿼리에 직접 들어갈 문자열이므로 injection 대비 검사를 한다.
	if( KODBC::IsInvalidMarkIn( kPacket_.m_wstrUnitName ) == true )
	{
		KEGS_COUPLE_PROPOSE_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_RELATIONSHIP_01;
		SendPacket( EGS_COUPLE_PROPOSE_ACK, kPacket );
		return;
	}

	// 커플링을 가지고 있는가?
	if( m_kInventory.IsExistOnlyInventory( CXSLItem::SI_COUPLE_RING_ITEM ) == false )
	{
		KEGS_COUPLE_PROPOSE_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_RELATIONSHIP_02;
		SendPacket( EGS_COUPLE_PROPOSE_ACK, kPacket );
		return;
	}

	// 자신이 솔로인가
	if( m_kUserRelationshipManager.IsSolo() == false )
	{
		KEGS_COUPLE_PROPOSE_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_RELATIONSHIP_03;
		SendPacket( EGS_COUPLE_PROPOSE_ACK, kPacket );
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	// DB 검사
	SendToGameDB( DBE_COUPLE_PROPOSE_USER_FIND_REQ, kPacket_ );
}

IMPL_ON_FUNC( DBE_COUPLE_PROPOSE_USER_FIND_ACK )
{
	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		KEGS_COUPLE_PROPOSE_ACK kPacket;
		kPacket.m_iOK = kPacket_.m_iOK;
		SendPacket( EGS_COUPLE_PROPOSE_ACK, kPacket );
		return;
	}

	// 캐릭터 성별 비교
#ifdef SERV_NEW_CHARACTER_EL	// 적용날짜: 2013-07-18
	int iOK = m_kUserRelationshipManager.CheckChar( GetUnitClass(), kPacket_.m_cUnitClass );
	if( iOK != NetError::NET_OK )
	{
		KEGS_COUPLE_PROPOSE_ACK kPacket;
		kPacket.m_iOK = iOK;
		SendPacket( EGS_COUPLE_PROPOSE_ACK, kPacket );
		return;
	}
#else	// SERV_NEW_CHARACTER_EL
	if( m_kUserRelationshipManager.CheckChar( GetUnitType(), kPacket_.m_cUnitClass ) == false )
	{
		KEGS_COUPLE_PROPOSE_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_RELATIONSHIP_05;
		SendPacket( EGS_COUPLE_PROPOSE_ACK, kPacket );
		return;
	}
#endif	// SERV_NEW_CHARACTER_EL
	
	// 상대방 인연 상태 확인
	if( m_kUserRelationshipManager.IsSolo( kPacket_.m_cRelationshipType ) == false )
	{
		KEGS_COUPLE_PROPOSE_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_RELATIONSHIP_06;
		SendPacket( EGS_COUPLE_PROPOSE_ACK, kPacket );
		return;
	}

	// 상대방 데이터 무결성 확인
	if( kPacket_.m_wstrOtherNickName.empty() == true || kPacket_.m_iOtherUserUID <= 0 || kPacket_.m_iOtherUnitUID <= 0 )
	{
		KEGS_COUPLE_PROPOSE_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_RELATIONSHIP_07;
		SendPacket( EGS_COUPLE_PROPOSE_ACK, kPacket );
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	// 로그인 서버로 가서 접속 중인지 확인 하자
	KELG_COUPLE_PROPOSE_CHECK_CONNECT_USER_REQ kReq;
	kReq.m_iOtherUserUID = kPacket_.m_iOtherUserUID;
	kReq.m_iOtherUnitUID = kPacket_.m_iOtherUnitUID;
	kReq.m_wstrRequestUnitName = GetCharName();
	kReq.m_iRequestUserUID = GetUID();
	kReq.m_iRequestUnitUID = GetCharUID();
	kReq.m_wstrAcceptUnitName = kPacket_.m_wstrOtherNickName;

	SendToLoginServer( ELG_COUPLE_PROPOSE_CHECK_CONNECT_USER_REQ, kReq );
}

IMPL_ON_FUNC( ELG_COUPLE_PROPOSE_CHECK_CONNECT_USER_ACK )
{
	KEGS_COUPLE_PROPOSE_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	kPacket.m_wstrNickName = kPacket_.m_wstrNickName;
	SendPacket( EGS_COUPLE_PROPOSE_ACK, kPacket );
}

IMPL_ON_FUNC( ELG_COUPLE_PROPOSE_NOT )
{
	KEGS_COUPLE_PROPOSE_NOT kPacket;
	kPacket.m_wstrRequestUnitName = kPacket_.m_wstrRequestUnitName;
	kPacket.m_iRequestUnitUID = kPacket_.m_iRequestUnitUID;
	SendPacket( EGS_COUPLE_PROPOSE_NOT, kPacket );
}

IMPL_ON_FUNC( EGS_COUPLE_PROPOSE_AGREE_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	// 신청자에게 결과를 전달한다.
	kPacket_.m_iAcceptUnitUID = GetCharUID();
	kPacket_.m_wstrAcceptUnitName = GetCharName();
	kPacket_.m_iAcceptUserUID = GetUID();
	kPacket_.m_ucAcceptUnitLevel = GetLevel();
	kPacket_.m_cAcceptUnitClass = GetUnitClass();
	SendToLoginServer( ELG_COUPLE_PROPOSE_AGREE_CHECK_NOT, kPacket_ );
}

_IMPL_ON_FUNC( ELG_COUPLE_PROPOSE_AGREE_CHECK_NOT, KEGS_COUPLE_PROPOSE_AGREE_NOT )
{
	// 거절 한 경우 - 그냥 거절 결과만 전달
	if( kPacket_.m_cAnswer != KEGS_COUPLE_PROPOSE_AGREE_NOT::CRT_YES )
	{
		KEGS_COUPLE_PROPOSE_RESULT_NOT kPacket;
		kPacket.m_iOK = NetError::NET_OK;
		kPacket.m_cResult = kPacket_.m_cAnswer;
		kPacket.m_iUnitUID = kPacket_.m_iAcceptUnitUID;
		kPacket.m_wstrUnitName = kPacket_.m_wstrAcceptUnitName;
		SendPacket( EGS_COUPLE_PROPOSE_RESULT_NOT, kPacket );
		return;
	}

	// 수락 한 경우
	// 인벤토리에서 커플링을 찾아 지워라
	bool bFindItem = false;

	KInventoryItemInfo kResultItemInfo;
	std::vector< UidType > vecItemUIDList;
	m_kInventory.GetItemUIDListThisItem( CXSLItem::SI_COUPLE_RING_ITEM, vecItemUIDList, true );

	std::map< UidType, int > mapDeleteItem;
	std::map< int, KItemInfo> mapDummy;
	std::vector<KItemInfo> vecNewItem;
	std::vector< KInventoryItemInfo > vecUpdatedInventorySlot;

	if( vecItemUIDList.empty() == false )
	{
		// 아무거나 하나면 된다. 첫번째 꺼로 하자
		mapDeleteItem.insert( std::make_pair( vecItemUIDList[0], 1 ) );
		if( m_kInventory.DeleteAndInsert( mapDeleteItem, mapDummy, vecUpdatedInventorySlot, vecNewItem, KDeletedItemInfo::DR_RELATIONSHIP ) == true )
		{
			bFindItem = true;
		}
	}

	if( bFindItem == false )
	{
		// 신청장에게 전달
		KEGS_COUPLE_PROPOSE_RESULT_NOT kPacket;
		kPacket.m_iOK = NetError::ERR_RELATIONSHIP_09;
		kPacket.m_cResult = KEGS_COUPLE_PROPOSE_AGREE_NOT::CRT_NO;
		kPacket.m_iUnitUID = kPacket_.m_iAcceptUnitUID;
		kPacket.m_wstrUnitName = kPacket_.m_wstrAcceptUnitName;
		SendPacket( EGS_COUPLE_PROPOSE_RESULT_NOT, kPacket );

		// 수락자에게 전달
		KERM_COUPLE_PROPOSE_RESULT_FAIL_NOT kFail;
		kFail.m_iOK = NetError::ERR_RELATIONSHIP_10;
		kFail.m_iRequestUnitUID = kPacket_.m_iRequestUnitUID;
		kFail.m_wstrRequestUnitName = kPacket_.m_wstrRequestUnitName;
		kFail.m_iAcceptUnitUID = kPacket_.m_iAcceptUnitUID;
		kFail.m_wstrAcceptUnitName = kPacket_.m_wstrAcceptUnitName;
		SendToLoginServer( ELG_COUPLE_PROPOSE_RESULT_FAIL_NOT, kFail );
		return;
	}

	// 아이템을 찾았으니 DB 에 삭제 요청 하고 커플 성사 시키자
	KDBE_COUPLE_MAKING_SUCCESS_REQ kPacketDB;
	kPacketDB.m_iRequestUnitUID = kPacket_.m_iRequestUnitUID;
	kPacketDB.m_wstrRequestUnitName = kPacket_.m_wstrRequestUnitName;
	kPacketDB.m_iAcceptUserUID = kPacket_.m_iAcceptUserUID;
	kPacketDB.m_ucAcceptUnitLevel = kPacket_.m_ucAcceptUnitLevel;
	kPacketDB.m_cAcceptUnitClass = kPacket_.m_cAcceptUnitClass;
	kPacketDB.m_iAcceptUnitUID = kPacket_.m_iAcceptUnitUID;
	kPacketDB.m_wstrAcceptUnitName = kPacket_.m_wstrAcceptUnitName;
	kPacketDB.m_vecUpdatedInventorySlot = vecUpdatedInventorySlot;
	m_kInventory.FlushDeletedItem( kPacketDB.m_vecDeleted );

	SendToGameDB( DBE_COUPLE_MAKING_SUCCESS_REQ, kPacketDB );
}

IMPL_ON_FUNC( DBE_COUPLE_MAKING_SUCCESS_ACK )
{
	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		// 신청장에게 전달
		KEGS_COUPLE_PROPOSE_RESULT_NOT kPacket;
		kPacket.m_iOK = kPacket_.m_iOK;
		kPacket.m_cResult = KEGS_COUPLE_PROPOSE_AGREE_NOT::CRT_NO;
		kPacket.m_iUnitUID = kPacket_.m_iAcceptUnitUID;
		kPacket.m_wstrUnitName = kPacket_.m_wstrAcceptUnitName;
		SendPacket( EGS_COUPLE_PROPOSE_RESULT_NOT, kPacket );

		if( kPacket_.m_iOK == NetError::ERR_RELATIONSHIP_11 )
		{
			m_kInventory.RollBackDeletedItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted );
		}

		return;
	}

	// 수락자에게는 커플 성공 정보를 보낸다.
	KERM_COUPLE_PROPOSE_RESULT_SUCCESS_NOT kPacket;
	kPacket.m_iOK = NetError::NET_OK;
	kPacket.m_iRequestUnitUID = kPacket_.m_iRequestUnitUID;
	kPacket.m_wstrRequestUnitName = kPacket_.m_wstrRequestUnitName;
	kPacket.m_iAcceptUnitUID = kPacket_.m_iAcceptUnitUID;
	kPacket.m_wstrAcceptUnitName = kPacket_.m_wstrAcceptUnitName;
	kPacket.m_tDate = kPacket_.m_tDate;
	SendToLoginServer( ELG_COUPLE_PROPOSE_RESULT_SUCCESS_NOT, kPacket );

	// 신청자에게는 커플링 삭제 정보와 커플 성공 정보를 보낸다.
	// 신청장에게 전달	
	// 커플 정보 셋팅
	CTime tDate = CTime( kPacket_.m_tDate );
	std::wstring wstrEmpty;
	m_kUserRelationshipManager.SetAnniversaryDay( tDate );
	m_kUserRelationshipManager.SetRelationshipType( SEnum::RT_COUPLE );
	m_kUserRelationshipManager.SetLoverUnitUID( kPacket_.m_iAcceptUnitUID );
	m_kUserRelationshipManager.SetLoverNickName( kPacket_.m_wstrAcceptUnitName );
	m_kUserRelationshipManager.SetLoveWord( wstrEmpty );
	m_kUserRelationshipManager.SetLastRewardDate( tDate );
	m_kUserRelationshipManager.SetRewardTitleType( 0 );
	
	KEGS_COUPLE_PROPOSE_RESULT_NOT kResult;
	kResult.m_cResult = KEGS_COUPLE_PROPOSE_AGREE_NOT::CRT_YES;
	kResult.m_iUnitUID = kPacket_.m_iAcceptUnitUID;
	kResult.m_wstrUnitName = kPacket_.m_wstrAcceptUnitName;
	kResult.m_tDate = kPacket_.m_tDate;
	kResult.m_vecUpdatedInventorySlot = kPacket_.m_vecUpdatedInventorySlot;
	SendPacket( EGS_COUPLE_PROPOSE_RESULT_NOT, kResult );

	CTime tCurr = CTime( kPacket_.m_tDate );
	// 로그 기록(나)
	KDBE_WRITE_RELATIONSHIP_INFO_LOG_REQ kLog;
	kLog.m_iUserUID = GetUID();
	kLog.m_iUnitUID = GetCharUID();
	kLog.m_ucLevel = GetLevel();
	kLog.m_cUnitClass = GetUnitClass();
	kLog.m_cCoupleType = 1;
	kLog.m_wstrRegDate = ( std::wstring )( tCurr.Format( _T( "%Y-%m-%d %H:%M:%S" ) ) );
	SendToLogDB( DBE_WRITE_RELATIONSHIP_INFO_LOG_REQ, kLog );

	// 로그 기록(상대)
	kLog.m_iUserUID = kPacket_.m_iAcceptUserUID;
	kLog.m_iUnitUID = kPacket_.m_iAcceptUnitUID;
	kLog.m_ucLevel = kPacket_.m_ucAcceptUnitLevel;
	kLog.m_cUnitClass = kPacket_.m_cAcceptUnitClass;
	kLog.m_cCoupleType = 1;
	SendToLogDB( DBE_WRITE_RELATIONSHIP_INFO_LOG_REQ, kLog );

	// 신청자가 위치한 곳에 따라 그 주변인 에게 커플 알림 전달(브로드캐스팅)
	KERM_COUPLE_MAKING_SUCCESS_NOT kNot;
	kNot.m_iRequestUnitUID = kPacket_.m_iRequestUnitUID;
	kNot.m_wstrRequestUnitName = kPacket_.m_wstrRequestUnitName;
	kNot.m_iAcceptUnitUID = kPacket_.m_iAcceptUnitUID;
	kNot.m_wstrAcceptUnitName = kPacket_.m_wstrAcceptUnitName;
	if( GetStateID() == KGSFSM::S_ROOM )
	{
		SendToCnRoom( ERM_COUPLE_MAKING_SUCCESS_NOT, kNot );
	}
	else
	{
		if( GetFieldUID() > 0 )
		{
			SendToGSField( ERM_COUPLE_MAKING_SUCCESS_NOT, kNot );
		}
	}
}

IMPL_ON_FUNC( ERM_COUPLE_PROPOSE_RESULT_FAIL_NOT )
{
	KEGS_COUPLE_PROPOSE_RESULT_NOT kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	kPacket.m_cResult = KEGS_COUPLE_PROPOSE_AGREE_NOT::CRT_NO;
	kPacket.m_iUnitUID = kPacket_.m_iRequestUnitUID;	// 상대방 
	kPacket.m_wstrUnitName = kPacket_.m_wstrRequestUnitName;	// 상대방 
	SendPacket( EGS_COUPLE_PROPOSE_RESULT_NOT, kPacket );
}

_IMPL_ON_FUNC( ELG_COUPLE_PROPOSE_RESULT_SUCCESS_NOT, KERM_COUPLE_PROPOSE_RESULT_SUCCESS_NOT )
{
	CTime tDate = CTime( kPacket_.m_tDate );
	std::wstring wstrEmpty;
	m_kUserRelationshipManager.SetAnniversaryDay( tDate );
	m_kUserRelationshipManager.SetRelationshipType( SEnum::RT_COUPLE );
	m_kUserRelationshipManager.SetLoverUnitUID( kPacket_.m_iRequestUnitUID );
	m_kUserRelationshipManager.SetLoverNickName( kPacket_.m_wstrRequestUnitName );
	m_kUserRelationshipManager.SetLoveWord( wstrEmpty );
	m_kUserRelationshipManager.SetLastRewardDate( tDate );
	m_kUserRelationshipManager.SetRewardTitleType( 0 );

	KEGS_COUPLE_PROPOSE_RESULT_NOT kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	kPacket.m_cResult = KEGS_COUPLE_PROPOSE_AGREE_NOT::CRT_YES;
	kPacket.m_iUnitUID = kPacket_.m_iRequestUnitUID;	// 상대방 
	kPacket.m_wstrUnitName = kPacket_.m_wstrRequestUnitName;	// 상대방 
	kPacket.m_tDate = kPacket_.m_tDate;

	SendPacket( EGS_COUPLE_PROPOSE_RESULT_NOT, kPacket );

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		// 수락자가 위치한 곳에 따라 그 주변인 에게 커플 알림 전달(브로드캐스팅)
		KERM_COUPLE_MAKING_SUCCESS_NOT kNot;
		kNot.m_iRequestUnitUID = kPacket_.m_iRequestUnitUID;
		kNot.m_wstrRequestUnitName = kPacket_.m_wstrRequestUnitName;
		kNot.m_iAcceptUnitUID = kPacket_.m_iAcceptUnitUID;
		kNot.m_wstrAcceptUnitName = kPacket_.m_wstrAcceptUnitName;
		if( GetStateID() == KGSFSM::S_ROOM )
		{
			SendToCnRoom( ERM_COUPLE_MAKING_SUCCESS_NOT, kNot );
		}
		else
		{
			if( GetFieldUID() > 0 )
			{
				SendToGSField( ERM_COUPLE_MAKING_SUCCESS_NOT, kNot );
			}
		}
	}
}

_IMPL_ON_FUNC( ERM_COUPLE_MAKING_SUCCESS_NOT, KEGS_COUPLE_MAKING_SUCCESS_NOT )
{
	SendPacket( EGS_COUPLE_MAKING_SUCCESS_NOT, kPacket_ );
}

IMPL_ON_FUNC( EGS_RELATIONSHIP_INFO_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( kPacket_.m_iAcceptUnitUID <= 0 )
	{
		KEGS_RELATIONSHIP_INFO_ACK kPacket;
		kPacket.m_iOK	= NetError::ERR_RELATIONSHIP_12;
		SendPacket( EGS_RELATIONSHIP_INFO_ACK, kPacket );
		return;
	}

	if( m_kUserRelationshipManager.IsSolo() == true )
	{
		KEGS_RELATIONSHIP_INFO_ACK kPacket;
		kPacket.m_iOK	= NetError::ERR_RELATIONSHIP_13;
		SendPacket( EGS_RELATIONSHIP_INFO_ACK, kPacket );
		return;
	}

	// 로그인 서버에서 접속 중인지 확인하자
	KELG_RELATIONSHIP_INFO_REQ kReq;
	kReq.m_iRequestGSUID	= KBaseServer::GetKObj()->GetUID();
	kReq.m_iRequestUnitUID	= GetCharUID();
	kReq.m_iAcceptUnitUID	= kPacket_.m_iAcceptUnitUID;

	SendToLoginServer( ELG_RELATIONSHIP_INFO_REQ, kReq );
}

_IMPL_ON_FUNC( ELG_RELATIONSHIP_INFO_ACK, KEGS_RELATIONSHIP_INFO_ACK )
{
	kPacket_.m_ucMyLevel = GetLevel();
	kPacket_.m_cMyUnitClass = GetUnitClass();
	kPacket_.m_iMyMapID = GetMapID();
	SiKChannelManager()->GetServerUIDByChannelName( kPacket_.m_wstrMyChannelName );

	if( GetRoomListID() > 0 )
	{
		kPacket_.m_cMyPosition = CXSLRoom::RT_PVP;
	}
	else
	{
		if( GetRoomUID() > 0 )
		{
			kPacket_.m_cMyPosition = CXSLRoom::GetRoomType( GetRoomUID() );
		}
		else
		{
			kPacket_.m_cMyPosition = -1; // 방이 아니면 -1
		}
	}

	SendPacket( EGS_RELATIONSHIP_INFO_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ELG_RELATIONSHIP_INFO_GAME_SERVER_REQ, KELG_RELATIONSHIP_INFO_REQ )
{
	KEGS_RELATIONSHIP_INFO_ACK kAck;
	kAck.m_iRequestGSUID = kPacket_.m_iRequestGSUID;
	kAck.m_iRequestUnitUID = kPacket_.m_iRequestUnitUID;
	kAck.m_iAcceptUnitUID = kPacket_.m_iAcceptUnitUID;

	m_kInventory.GetEquippedItem( kAck.m_mapOtherUnitEquippedItem );
	kAck.m_ucLevel = GetLevel();
	kAck.m_cUnitClass = GetUnitClass();
	kAck.m_iMapID = GetMapID();
	kAck.m_tLastLogOutTime = 0;
	kAck.m_bIsConnect = true;
	SiKChannelManager()->GetServerUIDByChannelName( kAck.m_wstrChannelName );

	if( GetRoomListID() > 0 )
	{
		kAck.m_cPosition = CXSLRoom::RT_PVP;
	}
	else
	{
		if( GetRoomUID() > 0 )
		{
			kAck.m_cPosition = CXSLRoom::GetRoomType( GetRoomUID() );
		}
		else
		{
			kAck.m_cPosition = -1; // 방이 아니면 -1
		}
	}

	SendToLoginServer( ELG_RELATIONSHIP_INFO_GAME_SERVER_ACK, kAck );
}

IMPL_ON_FUNC( EGS_RELATIONSHIP_EFFECT_NOT )
{
	bool bAmIMan = CXSLUnit::IsMan_UnitType( static_cast< CXSLUnit::UNIT_TYPE >(GetUnitType()) );

	if( bAmIMan == true )
	{
		kPacket_.m_iManUID = GetCharUID();
		kPacket_.m_iGirlUID = m_kUserRelationshipManager.GetLoverUnitUID();
	}
	else
	{
		kPacket_.m_iManUID = m_kUserRelationshipManager.GetLoverUnitUID();
		kPacket_.m_iGirlUID = GetCharUID();
	}

	m_kUserRelationshipManager.GetLoveWord( kPacket_.m_wstrLoveWord );

	// 방 이라면
	if( GetStateID() == KGSFSM::S_ROOM )
	{
		SendToCnRoom( ERM_RELATIONSHIP_EFFECT_NOT, kPacket_ );
	}
	else
	{
		if( GetFieldUID() > 0 )
		{
			SendToGSField( ERM_RELATIONSHIP_EFFECT_NOT, kPacket_ );
		}
	}
}

_IMPL_ON_FUNC( ERM_RELATIONSHIP_EFFECT_NOT, KEGS_RELATIONSHIP_EFFECT_NOT )
{
	SendPacket( EGS_RELATIONSHIP_EFFECT_NOT, kPacket_ );
}

IMPL_ON_FUNC( EGS_WEDDING_PROPOSE_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	//////////////////////////////////////////////////////////////////////////
	// 조건 검사
	//////////////////////////////////////////////////////////////////////////
#ifdef SERV_WEDING_PROPOSE_MSG_BUG_FIX
	// 문자열 검사
	if( KODBC::IsInvalidMarkInForLetter( kPacket_.m_wstrWeddingMsg ) )
	{
		KEGS_WEDDING_PROPOSE_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_ODBC_00;
		SendPacket( EGS_WEDDING_PROPOSE_ACK, kPacket );
		return;
	}
#endif // SERV_WEDING_PROPOSE_MSG_BUG_FIX

	// 커플 상태인가
	if( m_kUserRelationshipManager.IsCouple() == false )
	{
		KEGS_WEDDING_PROPOSE_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_RELATIONSHIP_15;
		SendPacket( EGS_WEDDING_PROPOSE_ACK, kPacket );
		return;
	}

	// 커플 된 시간
#ifdef SERV_RELATIONSHIP_SYSTEM_INT
#else
	if( m_kUserRelationshipManager.CheckCoupleDate() == false )
	{
		KEGS_WEDDING_PROPOSE_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_RELATIONSHIP_18;
		SendPacket( EGS_WEDDING_PROPOSE_ACK, kPacket );
		return;
	}
#endif SERV_RELATIONSHIP_SYSTEM_INT

	// 유효한 주례사 인가?	
	if( m_kUserRelationshipManager.IsValiedWeddingOfficiantNpc( kPacket_.m_cOfficiantNPC ) == false )
	{
		KEGS_WEDDING_PROPOSE_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_RELATIONSHIP_17;
		SendPacket( EGS_WEDDING_PROPOSE_ACK, kPacket );
		return;
	}

	// 유효한 결혼식장 인가?	
	if( m_kUserRelationshipManager.IsValiedWeddingHall( kPacket_.m_cWeddingHallType ) == false )
	{
		KEGS_WEDDING_PROPOSE_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_RELATIONSHIP_17;
		SendPacket( EGS_WEDDING_PROPOSE_ACK, kPacket );
		return;
	}

	// 삭제가 필요한 아이템 목록
	std::map< int, int > mapDeleteItem;

	// 초대한 인원 만큼의 초대권이 있는가?
	std::map< int, int > mapInviteTicketItem;
	int iInviteCount = static_cast<int>(kPacket_.m_vecInviteUnitList.size());
	mapInviteTicketItem.insert( std::make_pair( CXSLItem::SI_WEDDING_INVITE_TICKET_ITEM, iInviteCount ) );
	if( m_kInventory.IsEnoughItemExistOnlyInventory( mapInviteTicketItem ) == false )
	{
		KEGS_WEDDING_PROPOSE_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_RELATIONSHIP_21;
		SendPacket( EGS_WEDDING_PROPOSE_ACK, kPacket );
		return;
	}

	if( iInviteCount > 0 )
	{
		// 루벤이면 인원 15 이하
		if( kPacket_.m_cWeddingHallType == SEnum::WHT_CASH_A && iInviteCount > 15 )
		{
			KEGS_WEDDING_PROPOSE_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_RELATIONSHIP_20;
			SendPacket( EGS_WEDDING_PROPOSE_ACK, kPacket );
			return;
		}
		else if( kPacket_.m_cWeddingHallType == SEnum::WHT_CASH_B && iInviteCount > 30 )
		{
			KEGS_WEDDING_PROPOSE_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_RELATIONSHIP_20;
			SendPacket( EGS_WEDDING_PROPOSE_ACK, kPacket );
			return;
		}

		mapDeleteItem.insert( std::make_pair( CXSLItem::SI_WEDDING_INVITE_TICKET_ITEM, iInviteCount ) );
	}
	

	// 결혼반지를 가지고 있는가?
	if( m_kInventory.IsExistOnlyInventory( CXSLItem::SI_WEDDING_RING_ITEM ) == false )
	{
		KEGS_WEDDING_PROPOSE_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_RELATIONSHIP_14;
		SendPacket( EGS_WEDDING_PROPOSE_ACK, kPacket );
		return;
	}
	mapDeleteItem.insert( std::make_pair( CXSLItem::SI_WEDDING_RING_ITEM, 1 ) );

	// 결혼식장 아이템을 가지고 있는가?
	int iWeddingHallItemID = SiCXSLItemManager()->GetWeddingHallItem( kPacket_.m_cWeddingHallType );
	if( iWeddingHallItemID > 0 )
	{
		if( m_kInventory.IsExistOnlyInventory( iWeddingHallItemID ) == false )
		{
			KEGS_WEDDING_PROPOSE_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_RELATIONSHIP_16;
			SendPacket( EGS_WEDDING_PROPOSE_ACK, kPacket );
			return;
		}
	}
	else
	{
		KEGS_WEDDING_PROPOSE_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_RELATIONSHIP_17;
		SendPacket( EGS_WEDDING_PROPOSE_ACK, kPacket );
		return;
	}
	mapDeleteItem.insert( std::make_pair( iWeddingHallItemID, 1 ) );

	//////////////////////////////////////////////////////////////////////////
	// 삭제 하자(삽입 할 건 없다.)
	KInventoryItemInfo kResultItemInfo;
	KDBE_WEDDING_PROPOSE_REQ kPacketDB;

	std::map< int, KItemInfo> mapDummy;
	std::vector<KItemInfo> vecNewItem;
	if( m_kInventory.DeleteAndInsert( mapDeleteItem, mapDummy, kPacketDB.m_vecUpdatedInventorySlot, vecNewItem, true, KDeletedItemInfo::DR_RELATIONSHIP ) == false )
	{
		KEGS_WEDDING_PROPOSE_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_RELATIONSHIP_19;
		SendPacket( EGS_WEDDING_PROPOSE_ACK, kPacket );
		return;
	}

	// 아이템을 찾았으니 DB 에 삭제 요청 하고 커플 성사 시키자
	bool bAmIMan = CXSLUnit::IsMan_UnitType( static_cast< CXSLUnit::UNIT_TYPE >(GetUnitType()) );;
	if( bAmIMan == true )
	{
		kPacketDB.m_iManUID = GetCharUID();
		kPacketDB.m_iGirlUID = m_kUserRelationshipManager.GetLoverUnitUID();
	}
	else
	{
		kPacketDB.m_iManUID = m_kUserRelationshipManager.GetLoverUnitUID();
		kPacketDB.m_iGirlUID = GetCharUID();
	}

	CTime tCurTime = CTime( kPacket_.m_tWeddingDate );
	kPacketDB.m_wstrWeddingDate = ( std::wstring )( tCurTime.Format( _T( "%Y-%m-%d %H:%M:%S" ) ) );
	kPacketDB.m_cWeddingHallType = kPacket_.m_cWeddingHallType;
	kPacketDB.m_cOfficiantNPC = kPacket_.m_cOfficiantNPC;
	kPacketDB.m_vecInviteUnitList = kPacket_.m_vecInviteUnitList;
	kPacketDB.m_iUnitUID = GetCharUID();
	kPacketDB.m_wstrWeddingNickName = GetCharName();
	kPacketDB.m_wstrWeddingMsg = kPacket_.m_wstrWeddingMsg;

	m_kInventory.FlushQuantityChange( kPacketDB.m_kItemQuantityUpdate.m_mapQuantityChange );
	m_kInventory.FlushDeletedItem( kPacketDB.m_kItemQuantityUpdate.m_vecDeleted );

	SendToGameDB( DBE_WEDDING_PROPOSE_REQ, kPacketDB );
}

IMPL_ON_FUNC( DBE_WEDDING_PROPOSE_ACK )
{
	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		// 실패 했으니 롤백해야겠다.
		// DB쿼리 실패된 아이템이 있다면 롤백.
		m_kInventory.RollBackInitQuantity( kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange );
		m_kInventory.RollBackDeletedItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted );		

		KEGS_WEDDING_PROPOSE_ACK kPacket;
		kPacket.m_iOK = kPacket_.m_iOK;
		SendPacket( EGS_WEDDING_PROPOSE_ACK, kPacket );
		return;
	}

	KEGS_WEDDING_PROPOSE_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;
	kPacket.m_vecUpdatedInventorySlot = kPacket_.m_vecUpdatedInventorySlot;
	SendPacket( EGS_WEDDING_PROPOSE_ACK, kPacket );

	KELG_ADD_WEDDING_HALL_INFO_NOT kLogin;
	kLogin.m_kWeddingHallInfo.m_iWeddingUID = kPacket_.m_iWeddingUID;
	kLogin.m_kWeddingHallInfo.m_cWeddingHallType = kPacket_.m_cWeddingHallType;
	kLogin.m_kWeddingHallInfo.m_cOfficiantNPC = kPacket_.m_cOfficiantNPC;
	kLogin.m_kWeddingHallInfo.m_iGroom = kPacket_.m_iManUID;
	kLogin.m_kWeddingHallInfo.m_iBride = kPacket_.m_iGirlUID;
	kLogin.m_kWeddingHallInfo.m_wstrWeddingDate = kPacket_.m_wstrWeddingDate;
	kLogin.m_kWeddingHallInfo.m_wstrWeddingMsg = kPacket_.m_wstrWeddingMsg;
	kLogin.m_kWeddingHallInfo.m_iRoomUID = 0;
	kLogin.m_kWeddingHallInfo.m_bSuccess = false;
	kLogin.m_kWeddingHallInfo.m_bDelete = false;

	CTime tDate;
	if( KncUtil::ConvertStringToCTime( kPacket_.m_wstrWeddingDate, tDate ) == false )
	{
		START_LOG( cerr, L"결혼식 시간 변환 실패")
			<< BUILD_LOG( kPacket_.m_wstrWeddingDate )
			<< END_LOG;
	}

	kLogin.m_kWeddingHallInfo.m_tWeddingDate = tDate.GetTime();

	SendToLoginServer( ELG_ADD_WEDDING_HALL_INFO_NOT, kLogin );
}

IMPL_ON_FUNC( EGS_CHECK_NICK_NAME_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( kPacket_.m_wstrNickName.empty() == true )
	{
		KEGS_CHECK_NICK_NAME_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_NEW_ACCOUNT_06;
		SendPacket( EGS_CHECK_NICK_NAME_ACK, kPacketAck );
		return;
	}

	if( GetCharName().compare( kPacket_.m_wstrNickName ) == 0 )
	{
		KEGS_CHECK_NICK_NAME_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_NEW_ACCOUNT_06;
		SendPacket( EGS_CHECK_NICK_NAME_ACK, kPacketAck );
		return;
	}

	// 쿼리에 직접 들어갈 문자열이므로 injection 대비 검사를 한다.
	if( KODBC::IsInvalidMarkIn( kPacket_.m_wstrNickName ) )
	{
		KEGS_CHECK_NICK_NAME_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_ODBC_00;
		SendPacket( EGS_CHECK_NICK_NAME_ACK, kPacketAck );
		return;
	}

	SendToGameDB( DBE_CHECK_NICK_NAME_REQ, kPacket_ );
}

_IMPL_ON_FUNC( DBE_CHECK_NICK_NAME_ACK, KEGS_CHECK_NICK_NAME_ACK )
{
	SendPacket( EGS_CHECK_NICK_NAME_ACK, kPacket_ );
}

IMPL_ON_FUNC( EGS_WEDDING_ITEM_INFO_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	// 인벤토리에 존재하는 item 인지 검사 한다.
	BOOST_TEST_FOREACH( UidType, iItemUID, kPacket_.m_vecWeddingItemList )
	{
		KItemInfo kItem;
		if( m_kInventory.GetItemInfo( iItemUID, kItem ) == false )
		{
			START_LOG( cerr, L"존재하지 않는 아이템에 대한 정보를 요청합니다." )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( iItemUID )
				<< END_LOG;

			KEGS_WEDDING_ITEM_INFO_ACK kAck;
			kAck.m_iOK = NetError::ERR_RELATIONSHIP_22;
			SendPacket( EGS_WEDDING_ITEM_INFO_ACK, kAck );
			return;
		}

		char cWeddingStatus = SEnum::WS_NONE;
		if( m_kUserRelationshipManager.IsWeddingItem( kItem.m_iItemID, cWeddingStatus ) == false )
		{
			START_LOG( cerr, L"웨딩 관련 아이템이 아닌데 정보를 요청합니다." )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( iItemUID )
				<< END_LOG;

			KEGS_WEDDING_ITEM_INFO_ACK kAck;
			kAck.m_iOK = NetError::ERR_RELATIONSHIP_23;
			SendPacket( EGS_WEDDING_ITEM_INFO_ACK, kAck );
			return;
		}
	}

	SendToGameDB( DBE_WEDDING_ITEM_INFO_REQ, kPacket_ );
}

_IMPL_ON_FUNC( DBE_WEDDING_ITEM_INFO_ACK, KEGS_WEDDING_ITEM_INFO_ACK )
{
	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		KEGS_WEDDING_ITEM_INFO_ACK kAck;
		kAck.m_iOK = kPacket_.m_iOK;
		SendPacket( EGS_WEDDING_ITEM_INFO_ACK, kAck );
		return;
	}

	// 로그인 서버에 웨딩홀 정보를 받으로 가자
	KELG_WEDDING_ITEM_FIND_INFO_REQ kPacket;
	kPacket.m_iUnitUID = GetCharUID();
	kPacket.m_iOK = kPacket_.m_iOK;
	kPacket.m_mapWeddingItemInfo = kPacket_.m_mapWeddingItemInfo;
	SendToLoginServer( ELG_WEDDING_ITEM_FIND_INFO_REQ, kPacket );
}

_IMPL_ON_FUNC( ELG_WEDDING_ITEM_FIND_INFO_ACK, KELG_WEDDING_ITEM_FIND_INFO_REQ )
{
	// 신랑, 신부 정보가 없는 아이템은 벌써 결혼식을 완료한 아이템일 것이다
	// 한대 모아서 지우자
	std::map< UidType, int > mapDeleteItem;
	std::map< int, KItemInfo> mapDummy;
	std::vector<KItemInfo> vecNewItem;

	KDBE_WEDDING_ITEM_DELETE_REQ kPacket;
	kPacket.m_iUnitUID = GetCharUID();
	std::map< UidType, KWeddingItemInfo >::iterator mit = kPacket_.m_mapWeddingItemInfo.begin();
	for( ; mit != kPacket_.m_mapWeddingItemInfo.end() ; ++mit )
	{
		if( mit->second.m_iGroom <= 0 || mit->second.m_iBride <= 0 )
		{
			mapDeleteItem.insert( std::make_pair( mit->second.m_iItemUID, 1 ) );
		}
		else
		{
			kPacket.m_mapWeddingItemInfo.insert( std::make_pair(mit->first, mit->second) );
		}
	}

	if( m_kInventory.DeleteAndInsert( mapDeleteItem, mapDummy, kPacket.m_vecUpdatedInventorySlot, vecNewItem, KDeletedItemInfo::DR_RELATIONSHIP ) == false )
	{
		KEGS_WEDDING_ITEM_INFO_ACK kAck;
		kAck.m_iOK = NetError::ERR_RELATIONSHIP_24;
		SendPacket( EGS_WEDDING_ITEM_INFO_ACK, kAck );
		return;
	}

	m_kInventory.FlushDeletedItem( kPacket.m_vecDeleted );
	SendToGameDB( DBE_WEDDING_ITEM_DELETE_REQ, kPacket );
}

IMPL_ON_FUNC( DBE_WEDDING_ITEM_DELETE_ACK )
{
	KEGS_WEDDING_ITEM_INFO_ACK kAck;
	kAck.m_iOK = NetError::NET_OK;
	kAck.m_mapWeddingItemInfo = kPacket_.m_mapWeddingItemInfo;
	kAck.m_vecUpdatedInventorySlot = kPacket_.m_vecUpdatedInventorySlot;
	SendPacket( EGS_WEDDING_ITEM_INFO_ACK, kAck );
}

IMPL_ON_FUNC( EGS_CALL_MY_LOVER_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	// 검사
	// 인연 상태
	if( m_kUserRelationshipManager.IsSolo() == true )
	{
		KEGS_CALL_MY_LOVER_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_RELATIONSHIP_27;
		SendPacket( EGS_CALL_MY_LOVER_ACK, kPacket );
		return;
	}

	// 소환 가능 스테이트 상태
	if( GetStateID() == KGSFSM::S_ROOM )
	{
		switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
		{
		case CXSLRoom::RT_BATTLE_FIELD:
			{
				// 필드라면 센터로
				KERM_CALL_MY_LOVER_CHECK_ROOM_REQ kPacket;
				kPacket.m_iLoverUnitUID = m_kUserRelationshipManager.GetLoverUnitUID();
				kPacket.m_vPos = kPacket_.m_vPos;
				kPacket.m_LastTouchLineIndex = kPacket_.m_LastTouchLineIndex;
				kPacket.m_cCallType = KEGS_CALL_MY_LOVER_ROOM_NOT::CT_SAME_CHANNEL;
				kPacket.m_iChannelID = SiKChannelManager()->GetChannelID();
				kPacket.m_iCalledUserUID = GetUID();
				kPacket.m_iCalledGSUID = KBaseServer::GetKObj()->GetUID();
				SendToCnRoom( ERM_CALL_MY_LOVER_CHECK_ROOM_REQ, kPacket );
				return;
			}
			break;
		default:
			{
				KEGS_CALL_MY_LOVER_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_RELATIONSHIP_28;
				SendPacket( EGS_CALL_MY_LOVER_ACK, kPacket );
				return;
			}
		}
	}

	// 마을이라면 대상을 찾으러 로그인 서버로 간다.
	KELG_CALL_MY_LOVER_FIND_USER_VILLAGE_REQ kPacket;
	kPacket.m_iLoverUnitUID = m_kUserRelationshipManager.GetLoverUnitUID();
	kPacket.m_iMapID = GetMapID();
	kPacket.m_vPos = kPacket_.m_vPos;
	kPacket.m_LastTouchLineIndex = kPacket_.m_LastTouchLineIndex;
	kPacket.m_cCallType = KEGS_CALL_MY_LOVER_VILLAGE_NOT::CT_SAME_CHANNEL;
	kPacket.m_iCalledUserUID = GetUID();
	kPacket.m_iCalledGSUID = KBaseServer::GetKObj()->GetUID();
	kPacket.m_iChannelID = SiKChannelManager()->GetChannelID();
	SendToLoginServer( ELG_CALL_MY_LOVER_FIND_USER_VILLAGE_REQ, kPacket );
}

IMPL_ON_FUNC( ELG_CALL_MY_LOVER_FIND_USER_VILLAGE_ACK )
{
	KEGS_CALL_MY_LOVER_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	SendPacket( EGS_CALL_MY_LOVER_ACK, kPacket );
	return;
}

_IMPL_ON_FUNC( ELG_CALL_MY_LOVER_VILLAGE_NOT, KELG_CALL_MY_LOVER_FIND_USER_VILLAGE_REQ )
{
	// 소환 될 수 없는 위치라면 바로 거절로 하자
// 	if( GetStateID() == KGSFSM::S_ROOM )
// 	{
// 		if( CXSLRoom::GetRoomType( GetRoomUID() ) != CXSLRoom::RT_BATTLE_FIELD )
// 		{
// 			KELG_CALL_MY_LOVER_AGREE_RESULT_VILLAGE_NOT kPacket;
// 			kPacket.m_bAgree = false;
// 			kPacket.m_iMapID = kPacket_.m_iMapID;
// 			kPacket.m_iCalledGSUID = kPacket_.m_iCalledGSUID;
// 			kPacket.m_iCalledUserUID = kPacket_.m_iCalledUserUID;
// 			kPacket.m_vPos = kPacket_.m_vPos;
// 			kPacket.m_LastTouchLineIndex = kPacket_.m_LastTouchLineIndex;
// 			kPacket.m_iMyUserUID = GetUID();
// 			kPacket.m_iMyGSUID = KBaseServer::GetKObj()->GetUID();
// 			SendToLoginServer( ELG_CALL_MY_LOVER_AGREE_RESULT_VILLAGE_NOT, kPacket );
// 			return;
// 		}
// 	}

	// 소환 요청이 왔다
	KEGS_CALL_MY_LOVER_VILLAGE_NOT kPacket;
	kPacket.m_iMapID = kPacket_.m_iMapID;
	kPacket.m_vPos = kPacket_.m_vPos;
	kPacket.m_LastTouchLineIndex = kPacket_.m_LastTouchLineIndex;
	kPacket.m_cCallType = kPacket_.m_cCallType;
	kPacket.m_iCalledUserUID = kPacket_.m_iCalledUserUID;
	kPacket.m_iCalledGSUID = kPacket_.m_iCalledGSUID;

	SendPacket( EGS_CALL_MY_LOVER_VILLAGE_NOT, kPacket );
}

IMPL_ON_FUNC( EGS_CALL_MY_LOVER_AGREE_VILLAGE_NOT )
{
	KELG_CALL_MY_LOVER_AGREE_RESULT_VILLAGE_NOT kPacket;
	kPacket.m_bAgree = kPacket_.m_bAgree;
	kPacket.m_iMapID = kPacket_.m_iMapID;
	kPacket.m_iCalledGSUID = kPacket_.m_iCalledGSUID;
	kPacket.m_iCalledUserUID = kPacket_.m_iCalledUserUID;
	kPacket.m_vPos = kPacket_.m_vPos;
	kPacket.m_LastTouchLineIndex = kPacket_.m_LastTouchLineIndex;
	kPacket.m_iMyUserUID = GetUID();
	kPacket.m_iMyGSUID = KBaseServer::GetKObj()->GetUID();

	SendToLoginServer( ELG_CALL_MY_LOVER_AGREE_RESULT_VILLAGE_NOT, kPacket );
}

IMPL_ON_FUNC( ELG_CALL_MY_LOVER_AGREE_RESULT_VILLAGE_NOT )
{
	KEGS_CALL_MY_LOVER_AGREE_RESULT_NOT kPacket;
	kPacket = kPacket_.m_bAgree;
	SendPacket( EGS_CALL_MY_LOVER_AGREE_RESULT_NOT, kPacket );
}

IMPL_ON_FUNC( ELG_CALL_MY_LOVER_STATE_CHANGE_FIELD_NOT )
{
	KEGS_CALL_MY_LOVER_STATE_CHANGE_FIELD_NOT kPacketAck;

	//{{ 2012. 09. 21   김민성   마을 입장 오류 메시지 구분
	if( CheckEnterTheVillage( kPacket_.m_iMapID, kPacketAck.m_iLimitLevel, kPacketAck.m_iRequireDungeonID ) == false )
	{
		kPacketAck.m_iOK = NetError::GetLastError();

		START_LOG( cwarn, L"마을에 입장할 수 있는 조건이 안됨." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iMapID )
			<< BUILD_LOG( kPacketAck.m_iOK )
			<< BUILD_LOG( kPacketAck.m_iLimitLevel )
			<< BUILD_LOG( kPacketAck.m_iRequireDungeonID )
			<< END_LOG;
		SendPacket( EGS_CALL_MY_LOVER_STATE_CHANGE_FIELD_NOT, kPacketAck );
		return;
	}

	if( GetFieldUID() > 0 )
	{
		//  해당 필드에서 나오자~!
		KERM_CALL_MY_LOVER_LEAVE_FIELD_REQ kPacket;
		kPacket.m_iReason = NetError::NOT_LEAVE_FIELD_REASON_00;
		kPacket.m_iMapID  = kPacket_.m_iMapID;
		kPacket.m_vPos  = kPacket_.m_vPos;
		kPacket.m_LastTouchLineIndex  = kPacket_.m_LastTouchLineIndex;
		kPacket.m_iLimitLevel = kPacketAck.m_iLimitLevel;
		kPacket.m_iRequireDungeonID = kPacketAck.m_iRequireDungeonID;

		SendToGSField( ERM_CALL_MY_LOVER_LEAVE_FIELD_REQ, kPacket );
	}
	else
	{
		if( !SiCXSLMapData()->VerifyMapID( kPacket_.m_iMapID ) )
		{
			START_LOG( cerr, L"필드에 입장하려는데 MapID가 이상합니다." )
				<< BUILD_LOG( kPacket_.m_iMapID )
				<< BUILD_LOG( GetCharUID() )
				<< END_LOG;

			kPacketAck.m_iOK = NetError::ERR_FIELD_00; // map정보가 이상합니다.
			SendPacket( EGS_CALL_MY_LOVER_STATE_CHANGE_FIELD_NOT, kPacketAck );
			return;
		}

		// MapID 저장
		SetMapID( kPacket_.m_iMapID );

		kPacketAck.m_iOK = NetError::NET_OK;
		kPacketAck.m_iMapID = kPacket_.m_iMapID;
		kPacketAck.m_vPos = kPacket_.m_vPos;
		kPacketAck.m_LastTouchLineIndex = kPacket_.m_LastTouchLineIndex;
		SendPacket( EGS_CALL_MY_LOVER_STATE_CHANGE_FIELD_NOT, kPacketAck );

		//{{ 버프 관리 시스템 - 김민성
#ifdef SERV_SERVER_BUFF_SYSTEM
		KEGS_UPDATE_BUFF_INFO_NOT kPacket;
		m_kUserBuffManager.GetBuffList( kPacket.m_vecActivateBuffList );

		if( ( kPacket.m_vecActivateBuffList.empty() == false ) )
		{
			SendPacket( EGS_UPDATE_BUFF_INFO_NOT, kPacket );
		}
#endif SERV_SERVER_BUFF_SYSTEM
		//}
	}

	//////////////////////////////////////////////////////////////////////////
	// 필드로 이동할때 처리해야하는것..

	// PVP로비 이탈 처리
	SendLeavePVPLobby();

	// 개인거래 이탈 처리
	if( GetTradeUID() != 0 )
	{
		KEGS_BREAK_TRADE_REQ kPacket;
		kPacket.m_iReason	= NetError::NOT_LEAVE_ROOM_REASON_01;
		_SendToCnRoom( GetTradeUID(), ERM_BREAK_TRADE_REQ, kPacket );

		m_iTradeUID = 0;
	}

	// 개인상점 이탈 처리
	if( GetPersonalShopUID() != 0 )
	{
		if( CXSLRoom::GetRoomType( GetPersonalShopUID() ) == CXSLRoom::RT_PERSONAL_SHOP )
		{
			KEGS_BREAK_PERSONAL_SHOP_REQ kPacket;
			kPacket.m_iReason	= NetError::NOT_LEAVE_ROOM_REASON_01;
			_SendToCnRoom( GetPersonalShopUID(), ERM_BREAK_PERSONAL_SHOP_REQ, kPacket );
		}
		else
		{
			KERM_LEAVE_PERSONAL_SHOP_REQ kPacket;
			kPacket.m_iReason	= NetError::NOT_LEAVE_ROOM_REASON_01;
			_SendToLoginRoom( GetPersonalShopUID(), ERM_LEAVE_PSHOP_AGENCY_REQ, kPacket );
		}
		SetPersonalShopUID( 0 );
	}

	// 대전, 던전, 훈련소, 거래광장 이탈 처리
	if( GetStateID() == KGSFSM::S_ROOM )
	{
		switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
		{
		case CXSLRoom::RT_SQUARE:
			{
				KEGS_LEAVE_SQUARE_REQ kPacket;
				kPacket.m_iReason	= NetError::NOT_LEAVE_ROOM_REASON_01;
				SendToCnRoom( ERM_LEAVE_SQUARE_REQ, kPacket );
			}
			break;
		case CXSLRoom::RT_TRAININGCENTER:
			{
				KERM_LEAVE_ROOM_REQ kPacket;
				kPacket.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_01;
				SendToCnRoom( ERM_LEAVE_TC_ROOM_REQ, kPacket );
			}
			break;
		case CXSLRoom::RT_BATTLE_FIELD:
			{
				KERM_LEAVE_ROOM_REQ kPacket;
				if( kPacket_.m_bUseWarpItem )
				{
					kPacket.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_30;
				}
				else
				{
					kPacket.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_27;
				}
				SendToCnRoom( ERM_LEAVE_ROOM_REQ, kPacket );
			}
			break;
		default:
			{
				KERM_LEAVE_ROOM_REQ kPacket;
				kPacket.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_01;
				SendToCnRoom( ERM_LEAVE_ROOM_FOR_ENTER_THE_VILLAGE_REQ, kPacket );
			}
			break;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	m_kUserQuestManager.CheckEpicQuest_SuccessStateChange( kPacket_.m_iMapID, GetThisPtr<KGSUser>() );
}

IMPL_ON_FUNC( ERM_CALL_MY_LOVER_LEAVE_FIELD_ACK )
{
	// 상태는 어떻게 되나..?
	VERIFY_STATE( ( 3, KGSFSM::S_SERVER_SELECT, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( kPacket_.m_iReason == NetError::NOT_LEAVE_FIELD_REASON_00 )
	{
		KEGS_CALL_MY_LOVER_STATE_CHANGE_FIELD_NOT kPacketAck;

		if( kPacket_.m_iOK == NetError::NET_OK )
		{
			//{{ 2012. 02. 02	최육사	배틀필드 시스템
			SetMapID( kPacket_.m_iMapID );

			SetFieldUID( 0 ); // 필드에서 나오기 성공했으므로 0으로..

			// 이동할 필드의 FieldUId를 얻는다.			
			if( !SiCXSLMapData()->VerifyMapID( kPacket_.m_iMapID ) )
			{
				START_LOG( cerr, L"필드에 입장하려는데 MapID가 이상합니다." )
					<< BUILD_LOG( kPacket_.m_iMapID )
					<< BUILD_LOG( GetCharUID() )
					<< END_LOG;

				kPacketAck.m_iOK = NetError::ERR_FIELD_00; // map정보가 이상합니다.
				SendPacket( EGS_CALL_MY_LOVER_STATE_CHANGE_FIELD_NOT, kPacketAck );
				return;
			}
		}

		// 클라이언트로..
		kPacketAck.m_iOK = NetError::NET_OK;
		kPacketAck.m_iMapID = kPacket_.m_iMapID;
		kPacketAck.m_vPos = kPacket_.m_vPos;
		kPacketAck.m_LastTouchLineIndex = kPacket_.m_LastTouchLineIndex;
		kPacketAck.m_iLimitLevel = kPacket_.m_iLimitLevel;
		kPacketAck.m_iRequireDungeonID = kPacket_.m_iRequireDungeonID;
		SendPacket( EGS_CALL_MY_LOVER_STATE_CHANGE_FIELD_NOT, kPacketAck );
	}
	else
	{
		if( kPacket_.m_iOK == NetError::NET_OK )
		{
			SetFieldUID( 0 ); // 필드에서 나오기 성공했으므로 0으로..
		}
		else
		{
			START_LOG( cerr, L"필드 이탈 실패!" )
				<< BUILD_LOG( GetCharUID() )
				<< END_LOG;
		}
	}
}

IMPL_ON_FUNC( ERM_CALL_MY_LOVER_CHECK_ROOM_ACK )
{
	// 실패라면 소환자에게 알려준다
	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		KEGS_CALL_MY_LOVER_ACK kPacket;
		kPacket.m_iOK = kPacket_.m_iOK;
		SendPacket( EGS_CALL_MY_LOVER_ACK, kPacket );
		return;
	}

	// 입장 가능한 방을 찾았다.
	// 피소환자를 찾아 알려준다.
	KELG_CALL_MY_LOVER_FIND_USER_ROOM_REQ kPacket;
	kPacket.m_iLoverUnitUID = m_kUserRelationshipManager.GetLoverUnitUID();
	kPacket.m_vPos = kPacket_.m_vPos;
	kPacket.m_LastTouchLineIndex = kPacket_.m_LastTouchLineIndex;
	kPacket.m_kBTInfo = kPacket_.m_kBTInfo;
	kPacket.m_cCallType = KEGS_CALL_MY_LOVER_VILLAGE_NOT::CT_SAME_CHANNEL;
	kPacket.m_iCalledUserUID = GetUID();
	kPacket.m_iCalledGSUID = KBaseServer::GetKObj()->GetUID();
	kPacket.m_iChannelID = SiKChannelManager()->GetChannelID();
	SendToLoginServer( ELG_CALL_MY_LOVER_FIND_USER_ROOM_REQ, kPacket );
}

IMPL_ON_FUNC( ELG_CALL_MY_LOVER_FIND_USER_ROOM_ACK )
{
	// 소환자에게 알려준다
	KEGS_CALL_MY_LOVER_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	SendPacket( EGS_CALL_MY_LOVER_ACK, kPacket );
}

_IMPL_ON_FUNC( ELG_CALL_MY_LOVER_ROOM_NOT, KELG_CALL_MY_LOVER_FIND_USER_ROOM_REQ )
{
	// 현재 방에 입장해있는 상태라면 일단 이탈 처리부터 한다.
	if( GetRoomUID() > 0 )
	{
		if( CXSLRoom::GetRoomType( GetRoomUID() ) != CXSLRoom::RT_BATTLE_FIELD )
		{
			// 초대 거부로 하자
			KELG_CALL_MY_LOVER_AGREE_RESULT_ROOM_NOT kPacket;
			kPacket.m_bAgree = false;
			kPacket.m_iCalledGSUID = kPacket_.m_iCalledGSUID;
			kPacket.m_iCalledUserUID = kPacket_.m_iCalledUserUID;
			kPacket.m_iMyUserUID = GetUID();
			kPacket.m_iMyGSUID = KBaseServer::GetKObj()->GetUID();
			SendToLoginServer( ELG_CALL_MY_LOVER_AGREE_RESULT_ROOM_NOT, kPacket );
			return;
		}
	}

	// 연인이 당신을 부르고 있습니다.
	KEGS_CALL_MY_LOVER_ROOM_NOT kPacket;
	kPacket.m_iBattleFieldID = kPacket_.m_kBTInfo.m_iBattleFieldID;
	kPacket.m_RoomUID = kPacket_.m_kBTInfo.m_RoomUID;
	kPacket.m_vPos = kPacket_.m_vPos;
	kPacket.m_LastTouchLineIndex = kPacket_.m_LastTouchLineIndex;
	kPacket.m_cCallType = kPacket_.m_cCallType;
	kPacket.m_iCalledUserUID = kPacket_.m_iCalledUserUID;
	kPacket.m_iCalledGSUID = kPacket_.m_iCalledGSUID;
	if( kPacket_.m_iChannelID != SiKChannelManager()->GetChannelID() )
	{
		kPacket.m_cCallType = KEGS_CALL_MY_LOVER_ROOM_NOT::CT_OTHER_CHANNEL;
	}

	SendPacket( EGS_CALL_MY_LOVER_ROOM_NOT, kPacket );
}

IMPL_ON_FUNC( EGS_CALL_MY_LOVER_AGREE_ROOM_NOT )
{
	KELG_CALL_MY_LOVER_AGREE_RESULT_ROOM_NOT kPacket;
	kPacket.m_bAgree = kPacket_.m_bAgree;
	kPacket.m_iCalledGSUID = kPacket_.m_iCalledGSUID;
	kPacket.m_iCalledUserUID = kPacket_.m_iCalledUserUID;
	kPacket.m_iMyUserUID = GetUID();
	kPacket.m_iMyGSUID = KBaseServer::GetKObj()->GetUID();
	kPacket.m_iBattleFieldID = kPacket_.m_iBattleFieldID;
	kPacket.m_RoomUID = kPacket_.m_RoomUID;
	kPacket.m_vPos = kPacket_.m_vPos;
	kPacket.m_LastTouchLineIndex = kPacket_.m_LastTouchLineIndex;
	SendToLoginServer( ELG_CALL_MY_LOVER_AGREE_RESULT_ROOM_NOT, kPacket );
}

IMPL_ON_FUNC( ELG_CALL_MY_LOVER_AGREE_RESULT_ROOM_NOT )
{
	KEGS_CALL_MY_LOVER_AGREE_RESULT_NOT kPacket;
	kPacket = kPacket_.m_bAgree;
	SendPacket( EGS_CALL_MY_LOVER_AGREE_RESULT_NOT, kPacket );
}

IMPL_ON_FUNC( ELG_CALL_MY_LOVER_JOIN_BATTLE_FIELD_NOT )
{
	KERM_CALL_MY_LOVER_JOIN_BATTLE_FIELD_REQ kPacket;
	kPacket.m_iCalledUserUID = kPacket_.m_iCalledUserUID;
	kPacket.m_iCalledUnitUID = kPacket_.m_iCalledUnitUID;
	kPacket.m_iCalledGSUID = kPacket_.m_iCalledGSUID;
	kPacket.m_vPos = kPacket_.m_vPos;
	kPacket.m_RoomUID = kPacket_.m_RoomUID;
	kPacket.m_kBattleFieldJoinInfo.m_iBattleFieldID = kPacket_.m_iBattleFieldID;
	kPacket.m_kBattleFieldJoinInfo.m_iStartPosIndex = kPacket_.m_LastTouchLineIndex;
	kPacket.m_kBattleFieldJoinInfo.m_bMoveForMyParty = false;
#ifdef  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
    kPacket.m_kBattleFieldJoinInfo.m_bNowBattleFieldPositionInfoStartPosition = false;
    kPacket.m_kBattleFieldJoinInfo.m_usBattleFieldPositionValue = 0;
#endif  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
	GetRoomUserInfo( kPacket.m_kRoomUserInfo, CXSLRoom::RT_BATTLE_FIELD );
	GetTutorUnitUIDList( kPacket.m_vecStudentUnitUID );
	m_kUserUnitManager.GetAndDeleteResevedReturnToFieldInfo( kPacket.m_kReturnToFieldInfo );

	// 현재 방에 입장해있는 상태라면 일단 이탈 처리부터 한다.
	if( GetRoomUID() > 0 )
	{
		switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
		{
		case CXSLRoom::RT_BATTLE_FIELD:
			{
				SendToCnRoom( ERM_LEAVE_BATTLE_FIELD_FOR_CALL_MY_LOVER_REQ, kPacket );
				return;
			}
			break;
		}
	}

	_SendToCnRoom( kPacket_.m_RoomUID, ERM_CALL_MY_LOVER_JOIN_BATTLE_FIELD_REQ, kPacket );
}

_IMPL_ON_FUNC( ERM_LEAVE_BATTLE_FIELD_FOR_CALL_MY_LOVER_ACK, KERM_CALL_MY_LOVER_JOIN_BATTLE_FIELD_REQ )
{
	_SendToCnRoom( kPacket_.m_RoomUID, ERM_CALL_MY_LOVER_JOIN_BATTLE_FIELD_REQ, kPacket_ );
}

_IMPL_ON_FUNC( ERM_CALL_MY_LOVER_JOIN_BATTLE_FIELD_ACK, KEGS_CALL_MY_LOVER_JOIN_BATTLE_FIELD_NOT )
{
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		//////////////////////////////////////////////////////////////////////////
		// 입장 성공!

		// 배틀필드 MapID값 저장!
		m_kLastPos.Clear();
		SetMapID( kPacket_.m_RoomInfo.m_iBattleFieldID );

		// 배틀필드 RoomUID값 저장!
		SetRoomUID( kPacket_.m_RoomInfo.m_RoomUID );

		//{{ 2007. 11. 7  최육사  예외처리로 인해 소스코드 위치이동
		StateTransition( KGSFSM::I_TO_ROOM );
		m_kTimer[TM_ROOM].restart();
		//}}
		SendUpdateGuildMemberInfo();
		// 캐시샵 입장 정보 초기화
		m_kUserUnitManager.SetEnterCashShop( false );
		// 배틀필드에 입장 성공했는데 자신의 파티 객체가 존재한다면 자신의 RoomUID값을 배틀필드로 업데이트 하자!
	}
	else
	{
		KELG_CALL_MY_LOVER_JOIN_BATTLE_FIELD_FAILD_NOT kPacket;
		kPacket.m_iOK = kPacket_.m_iOK;
		kPacket.m_iCalledGSUID = kPacket_.m_iCalledGSUID;
		kPacket.m_iCalledUserUID = kPacket_.m_iCalledUserUID;
		SendToLoginServer( ELG_CALL_MY_LOVER_JOIN_BATTLE_FIELD_FAILD_NOT, kPacket );
	}

	SendPacket( EGS_CALL_MY_LOVER_JOIN_BATTLE_FIELD_NOT, kPacket_ );
}

IMPL_ON_FUNC( ELG_CALL_MY_LOVER_JOIN_BATTLE_FIELD_FAILD_NOT )
{
	KEGS_CALL_MY_LOVER_JOIN_BATTLE_FIELD_FAILD_NOT kPacket;
	kPacket.m_iOK = NetError::ERR_RELATIONSHIP_46;
	SendPacket( EGS_CALL_MY_LOVER_JOIN_BATTLE_FIELD_FAILD_NOT, kPacket );
}

IMPL_ON_FUNC( EGS_JOIN_WEDDING_HALL_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_JOIN_WEDDING_HALL_REQ, EGS_JOIN_WEDDING_HALL_ACK );

	// 이 아이템이 입장 가능한 아이템인지 검사 해야한다.
	// 1. 실제 인벤토리에 아이템이 존재하는가? // 물론 삭제는 하지 않는다.
	KItemInfo kItemInfo;
	if( m_kInventory.GetItemInfo( kPacket_.m_iItemUID, kItemInfo ) == false )
	{
		// 존재 하지 안는 아이템이다.
		START_LOG( cerr, L"존재하지 않는 아이템 입니다." )
			<< BUILD_LOG( kPacket_.m_iItemUID )
			<< BUILD_LOG( kPacket_.m_iWeddingUID )
			<< END_LOG;

		KEGS_JOIN_WEDDING_HALL_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_RELATIONSHIP_31;
		SendPacket( EGS_JOIN_WEDDING_HALL_ACK, kPacket );
		return;
	}

	// 2. 청첩장, 결혼식 입장권 인가?
	char cWeddingStatus = SEnum::WS_NONE;
	if( m_kUserRelationshipManager.IsWeddingItem( kItemInfo.m_iItemID, cWeddingStatus ) == false )
	{
		// 결혼식장 입장 가능 아이템이 아닙니다.
		START_LOG( cerr, L"결혼식장에 입장 가능한 아이템이 아닙니다." )
			<< BUILD_LOG( kPacket_.m_iItemUID )
			<< BUILD_LOG( kPacket_.m_iWeddingUID )
			<< BUILD_LOG( kItemInfo.m_iItemID )
			<< END_LOG;

		KEGS_JOIN_WEDDING_HALL_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_RELATIONSHIP_32;
		SendPacket( EGS_JOIN_WEDDING_HALL_ACK, kPacket );
		return;
	}

	if( cWeddingStatus == SEnum::WS_GROOM )
	{
		if( CXSLUnit::IsMan_UnitType( static_cast< CXSLUnit::UNIT_TYPE >(GetUnitType()) ) == false )
		{
			cWeddingStatus = SEnum::WS_BRIDE;
		}
	}

	// 3. 현재 나의 위치 확인 (마을, 휴식처, 필드에서 사용가능)
	if( GetStateID() == KGSFSM::S_ROOM )
	{
		switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
		{
		case CXSLRoom::RT_BATTLE_FIELD:
			{
				// 이동 가능하니 패스 - 방 이탈은 나중에
			}
			break;
		default:
			{
				// 결혼식장에 입장 가능한 위치가 아닙니다.
				START_LOG( cerr, L"결혼식장에 입장 가능한 위치가 아닙니다." )
					<< BUILD_LOG( kPacket_.m_iItemUID )
					<< BUILD_LOG( kPacket_.m_iWeddingUID )
					<< BUILD_LOG( kItemInfo.m_iItemID )
					<< END_LOG;

				KEGS_JOIN_WEDDING_HALL_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_RELATIONSHIP_33;
				SendPacket( EGS_JOIN_WEDDING_HALL_ACK, kPacket );
				return;
			}
		}
	}
	
	// 조건을 통과 했으니 로그인 서버로 가서 결혼식장 정보를 알아보자
	KELG_CHECK_WEDDING_HALL_INFO_REQ kPacket;
	kPacket.m_iWeddingUID = kPacket_.m_iWeddingUID;
	kPacket.m_cUnitWeddingStatus = cWeddingStatus;
	kPacket.m_iGSUID = KBaseServer::GetKObj()->GetUID();;
	kPacket.m_iGSUserUID = GetUID();
	SendToLoginServer( ELG_CHECK_WEDDING_HALL_INFO_REQ, kPacket );
}

IMPL_ON_FUNC( ELG_CHECK_WEDDING_HALL_INFO_ACK )
{
	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		KEGS_JOIN_WEDDING_HALL_ACK kPacket;
		kPacket.m_iOK = kPacket_.m_iOK;
		SendPacket( EGS_JOIN_WEDDING_HALL_ACK, kPacket );
		return;
	}

	// 정상적으로 결혼식장 정보를 받아 왔다. 그럼 입장 처리 하자
	// 결혼식장에 입장가능한 상태인지 검사한다.
	if( GetStateID() == KGSFSM::S_ROOM )
	{
		switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
		{
		case CXSLRoom::RT_BATTLE_FIELD:
			{
				// 필드에서 결혼식장로 이동하는경우 임시 인벤토리를 비운다!
				ClearTempInventory();
			}
			break;

		default:
			{
				START_LOG( cerr, L"해당 방에서 결혼식는 이동할 수 없습니다!" )
					<< BUILD_LOG( GetUID() )
					<< BUILD_LOG( GetCharUID() )
					<< BUILD_LOG( kPacket_.m_kWeddingInfo.m_iWeddingUID )
					<< END_LOG;

				KEGS_JOIN_WEDDING_HALL_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_RELATIONSHIP_33;
				SendPacket( EGS_JOIN_WEDDING_HALL_ACK, kPacket );
			}
			return;
		}
	}
	else
	{
		if( GetRoomUID() != 0 )
		{
			START_LOG( cerr, L"필드 상태인데 RoomUID값이 존재한다! 버그다!" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( kPacket_.m_kWeddingInfo.m_iWeddingUID )
				<< BUILD_LOG( GetRoomUID() )
				<< BUILD_LOG( CXSLRoom::GetRoomType( GetRoomUID() ) )
				<< END_LOG;

			KEGS_JOIN_WEDDING_HALL_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_RELATIONSHIP_33;
			SendPacket( EGS_JOIN_WEDDING_HALL_ACK, kPacket );
			return;
		}		
	}

	// 결혼식장 입장 시 캐시샵 입장 정보를 초기화 한다.
	m_kUserUnitManager.SetEnterCashShop( false );

	// 현재 방에 입장해있는 상태라면 일단 이탈 처리부터 한다.
	if( GetRoomUID() > 0 )
	{
		switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
		{
		case CXSLRoom::RT_BATTLE_FIELD:
			{
				KERM_LEAVE_BATTLE_FIELD_FOR_JOIN_WEDDING_HALL_REQ kPacket;
				kPacket.m_kWeddingInfo = kPacket_.m_kWeddingInfo;
				kPacket.m_cUnitWeddingStatus = kPacket_.m_cUnitWeddingStatus;
				SendToCnRoom( ERM_LEAVE_BATTLE_FIELD_FOR_JOIN_WEDDING_HALL_REQ, kPacket );
				return;
			}
			break;

		default:
			{
				START_LOG( cerr, L"이상한 방에 입장한 상태에서 배틀필드로 들어가려고 하네?" )
					<< BUILD_LOG( GetCharName() )
					<< BUILD_LOG( CXSLRoom::GetRoomType( GetRoomUID() ) )
					<< END_LOG;

				KEGS_JOIN_WEDDING_HALL_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_RELATIONSHIP_33;
				SendPacket( EGS_JOIN_WEDDING_HALL_ACK, kPacket );
				return;
			}
			break;
		}
	}

	// 결혼식장 정보를 찾아서 룸정보를 알아보자
	KGSWeddingHallPtr pWeddingHallInfo = SiKWeddingHallListManager()->GetWeddingHallInfo( kPacket_.m_kWeddingInfo.m_iWeddingUID );
	if( pWeddingHallInfo == NULL )
	{
		// 정보가 없다.
		START_LOG( cerr, L"[테스트]결혼식장 정보가 없다? 새로 만드는 것이 맞는거 같은데...." )
			<< BUILD_LOG( kPacket_.m_kWeddingInfo.m_iWeddingUID )
			<< END_LOG;

		KEGS_JOIN_WEDDING_HALL_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_RELATIONSHIP_34;
		SendPacket( EGS_JOIN_WEDDING_HALL_ACK, kPacket );
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	KWeddingHallInfo kInfo;
	pWeddingHallInfo->GetWeddingHallInfo( kInfo );
	START_LOG( cout, L"[테스트]결혼식 입장 정보 확인" )
		<< BUILD_LOG( kInfo.m_iWeddingUID )
		<< BUILD_LOG( kInfo.m_iRoomUID )
		<< BUILD_LOG( kInfo.m_iGroom )
		<< BUILD_LOG( kInfo.m_iBride )
		<< END_LOG;
	//////////////////////////////////////////////////////////////////////////


	// 정보는 있는데 방UID 는 없다. 새로 생성해야 한다.
	if( pWeddingHallInfo->GetRoomUID() <= 0 )
	{
		KERM_OPEN_WEDDING_HALL_REQ kPacket;
		pWeddingHallInfo->GetWeddingHallInfo( kPacket.m_kWeddingInfo );
		GetRoomUserInfo( kPacket.m_kRoomUserInfo, CXSLRoom::RT_WEDDING_HALL );
		kPacket.m_kRoomUserInfo.m_cWeddingStatus = kPacket_.m_cUnitWeddingStatus;
		m_kUserUnitManager.GetAndDeleteResevedReturnToFieldInfo( kPacket.m_kReturnToFieldInfo );
		SendToCnRoom( ERM_OPEN_WEDDING_HALL_REQ, kPacket );
		return;
	}
	
	KERM_JOIN_WEDDING_HALL_REQ kPacket;
	pWeddingHallInfo->GetWeddingHallInfo( kPacket.m_kWeddingInfo );
	GetRoomUserInfo( kPacket.m_kRoomUserInfo, CXSLRoom::RT_WEDDING_HALL );
	kPacket.m_kRoomUserInfo.m_cWeddingStatus = kPacket_.m_cUnitWeddingStatus;
	m_kUserUnitManager.GetAndDeleteResevedReturnToFieldInfo( kPacket.m_kReturnToFieldInfo );
	_SendToCnRoom( pWeddingHallInfo->GetRoomUID(), ERM_JOIN_WEDDING_HALL_REQ, kPacket );
}

_IMPL_ON_FUNC( ERM_LEAVE_BATTLE_FIELD_FOR_JOIN_WEDDING_HALL_ACK, KERM_LEAVE_BATTLE_FIELD_FOR_JOIN_WEDDING_HALL_REQ )
{
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_ROOM ), EGS_JOIN_WEDDING_HALL_ACK );

	// 성공적으로 방 이탈 처리!
	SetRoomUID( 0 );

	StateTransition( KGSFSM::I_TO_FIELD_MAP );

	if( GetTradeUID() != 0 )
	{
		KEGS_BREAK_TRADE_REQ kPacket;
		kPacket.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_16;
		_SendToCnRoom( GetTradeUID(), ERM_BREAK_TRADE_REQ, kPacket );
	}	

	// 결혼식장 정보를 찾아서 룸정보를 알아보자
	KGSWeddingHallPtr pWeddingHallInfo = SiKWeddingHallListManager()->GetWeddingHallInfo( kPacket_.m_kWeddingInfo.m_iWeddingUID );
	if( pWeddingHallInfo == NULL )
	{
		// 정보가 없다.
		START_LOG( cerr, L"[테스트]결혼식장 정보가 없다? 새로 만드는 것이 맞는거 같은데...." )
			<< BUILD_LOG( kPacket_.m_kWeddingInfo.m_iWeddingUID )
			<< END_LOG;

		KEGS_JOIN_WEDDING_HALL_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_RELATIONSHIP_34;
		SendPacket( EGS_JOIN_WEDDING_HALL_ACK, kPacket );
		return;
	}
	
	if( pWeddingHallInfo->GetRoomUID() == 0 )
	{
		if( GetRoomUID() != 0 )
		{
			START_LOG( cerr, L"새로운 배틀필드를 생성하려는데 지금 다른방에 입장해있다! 일어나서는 안되는 에러!" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( GetCharName() )
				<< BUILD_LOG( GetStateIDString() )
				<< BUILD_LOG( GetRoomUID() )
				<< BUILD_LOG( CXSLRoom::GetRoomType( GetRoomUID() ) )
				<< END_LOG;
		}

	
		KERM_OPEN_WEDDING_HALL_REQ kPacket;
		pWeddingHallInfo->GetWeddingHallInfo( kPacket.m_kWeddingInfo );
		GetRoomUserInfo( kPacket.m_kRoomUserInfo, CXSLRoom::RT_WEDDING_HALL );
		kPacket.m_kRoomUserInfo.m_cWeddingStatus = kPacket_.m_cUnitWeddingStatus;
		m_kUserUnitManager.GetAndDeleteResevedReturnToFieldInfo( kPacket.m_kReturnToFieldInfo );
		SendToCnRoom( ERM_OPEN_WEDDING_HALL_REQ, kPacket );
	}
	else
	{
		KERM_JOIN_WEDDING_HALL_REQ kPacket;
		pWeddingHallInfo->GetWeddingHallInfo( kPacket.m_kWeddingInfo );
		GetRoomUserInfo( kPacket.m_kRoomUserInfo, CXSLRoom::RT_WEDDING_HALL );
		kPacket.m_kRoomUserInfo.m_cWeddingStatus = kPacket_.m_cUnitWeddingStatus;
		m_kUserUnitManager.GetAndDeleteResevedReturnToFieldInfo( kPacket.m_kReturnToFieldInfo );
		_SendToCnRoom( pWeddingHallInfo->GetRoomUID(), ERM_JOIN_WEDDING_HALL_REQ, kPacket );
	}
}

IMPL_ON_FUNC( ERM_OPEN_WEDDING_HALL_ACK )
{
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_JOIN_WEDDING_HALL_ACK );
	
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		//{{ 2007. 11. 7  최육사  예외처리로 인해 소스코드 위치이동
		StateTransition( KGSFSM::I_TO_ROOM );
		m_kTimer[TM_ROOM].restart();
		//}}

		// 필드에서 이탈
		SendLeaveField( NetError::NOT_LEAVE_FIELD_REASON_00 );

		// 결혼식 RoomUID저장
		SetRoomUID( kPacket_.m_kWeddingInfo.m_iRoomUID );

		//{{ 2007. 11. 7  최육사  예외처리로 인해 소스코드 위치이동
		StateTransition( KGSFSM::I_TO_ROOM );
		m_kTimer[TM_ROOM].restart();
		//}}

		//{{ 2009. 10. 8  최육사	길드
		SendUpdateGuildMemberInfo();

	}
	else
	{
		START_LOG_WITH_NAME( clog )
			<< BUILD_LOG( kPacket_.m_iOK )
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
			<< BUILD_LOG( kPacket_.m_RoomInfo.m_wstrPassword )
			<< END_LOG;
	}

	KEGS_JOIN_WEDDING_HALL_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	kPacket.m_kWeddingInfo = kPacket_.m_kWeddingInfo;
	kPacket.m_RoomInfo = kPacket_.m_RoomInfo;
	kPacket.m_vecSlot = kPacket_.m_vecSlot;
	kPacket.m_wstrCNIP = kPacket_.m_wstrCNIP;
	//{{ 핑 상태체크 호스트 변경 - 김민성
	kPacket.m_wstrUDPRelayIP = kPacket_.m_wstrUDPRelayIP;
	kPacket.m_usUDPRelayPort = kPacket_.m_usUDPRelayPort;
	SendPacket( EGS_JOIN_WEDDING_HALL_ACK, kPacket );
}

_IMPL_ON_FUNC( ERM_LEAVE_WEDDING_HALL_NOT, KEGS_LEAVE_WEDDING_HALL_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_LEAVE_WEDDING_HALL_NOT, kPacket_ );
}

IMPL_ON_FUNC( ERM_JOIN_WEDDING_HALL_ACK )
{
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_JOIN_WEDDING_HALL_ACK );

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		//{{ 2007. 11. 7  최육사  예외처리로 인해 소스코드 위치이동
		StateTransition( KGSFSM::I_TO_ROOM );
		m_kTimer[TM_ROOM].restart();
		//}}

		// 필드에서 이탈
		SendLeaveField( NetError::NOT_LEAVE_FIELD_REASON_00 );

		// 결혼식 RoomUID저장
		SetRoomUID( kPacket_.m_kWeddingInfo.m_iRoomUID );

		//{{ 2007. 11. 7  최육사  예외처리로 인해 소스코드 위치이동
		StateTransition( KGSFSM::I_TO_ROOM );
		m_kTimer[TM_ROOM].restart();
		//}}

		//{{ 2009. 10. 8  최육사	길드
		SendUpdateGuildMemberInfo();

	}
	else
	{
		START_LOG_WITH_NAME( clog )
			<< BUILD_LOG( kPacket_.m_iOK )
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
			<< BUILD_LOG( kPacket_.m_RoomInfo.m_wstrPassword )
			<< END_LOG;
	}

	KEGS_JOIN_WEDDING_HALL_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	kPacket.m_kWeddingInfo = kPacket_.m_kWeddingInfo;
	kPacket.m_RoomInfo = kPacket_.m_RoomInfo;
	kPacket.m_vecSlot = kPacket_.m_vecSlot;
	kPacket.m_wstrCNIP = kPacket_.m_wstrCNIP;
	//{{ 핑 상태체크 호스트 변경 - 김민성
	kPacket.m_wstrUDPRelayIP = kPacket_.m_wstrUDPRelayIP;
	kPacket.m_usUDPRelayPort = kPacket_.m_usUDPRelayPort;
	SendPacket( EGS_JOIN_WEDDING_HALL_ACK, kPacket );
}

_IMPL_ON_FUNC( ERM_JOIN_WEDDING_HALL_NOT, KEGS_JOIN_WEDDING_HALL_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_JOIN_WEDDING_HALL_NOT, kPacket_ );
}

IMPL_ON_FUNC( EGS_LEAVE_WEDDING_HALL_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_ROOM ), EGS_LEAVE_WEDDING_HALL_REQ, EGS_LEAVE_WEDDING_HALL_ACK );

	START_LOG_WITH_NAME( clog )
		<< BUILD_LOG( GetRoomUID() );

	SendToCnRoom( ERM_LEAVE_WEDDING_HALL_REQ, kPacket_ );
}
_IMPL_ON_FUNC( ERM_LEAVE_WEDDING_HALL_ACK, KEGS_LEAVE_ROOM_ACK )
{
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		SetRoomUID( 0 );

		StateTransition( KGSFSM::I_TO_FIELD_MAP );

		if( GetTradeUID() != 0 )
		{
			KEGS_BREAK_TRADE_REQ kPacket;
			kPacket.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_16;
			_SendToCnRoom( GetTradeUID(), ERM_BREAK_TRADE_REQ, kPacket );
		}

		KEGS_LEAVE_WEDDING_HALL_ACK kPacket;
		kPacket.m_iOK = kPacket_.m_iReason;
		SendPacket( EGS_LEAVE_WEDDING_HALL_ACK, kPacket );
		return;
	}
	
	KEGS_LEAVE_WEDDING_HALL_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	SendPacket( EGS_LEAVE_WEDDING_HALL_ACK, kPacket_ );
}

IMPL_ON_FUNC_NOPARAM( EGS_CHECK_BREAK_UP_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_ROOM, KGSFSM::S_FIELD_MAP ) );

	KEGS_CHECK_BREAK_UP_ACK kPacket;

	// 이별 비용 구하기
	int iCost = m_kUserRelationshipManager.GetBreakUpCost();
	kPacket.m_iCost = iCost;
	kPacket.m_cRelationshipType = m_kUserRelationshipManager.GetRelationshipType();
	kPacket.m_wstrLoverNickName = m_kUserRelationshipManager.GetLoverNickName();

	if( m_kUserRelationshipManager.IsCouple() == false && m_kUserRelationshipManager.IsMarried() == false )
	{
		START_LOG( cerr, L"이별 가능한 상태가 아닙니다." )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOGc( m_kUserRelationshipManager.GetRelationshipType() )
			<< END_LOG;
        
		kPacket.m_iOK = NetError::ERR_RELATIONSHIP_36;
		SendPacket( EGS_CHECK_BREAK_UP_ACK, kPacket );
		return;
	}

	if( GetRoomUID() > 0 )
	{
		switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
		{
		case CXSLRoom::RT_BATTLE_FIELD:
			{
				// 정상
			}
			break;

		default:
			{
				START_LOG( cerr, L"이별 가능한 위치가 아닙니다." )
					<< BUILD_LOG( GetCharName() )
					<< BUILD_LOGc( m_kUserRelationshipManager.GetRelationshipType() )
					<< BUILD_LOG( CXSLRoom::GetRoomType( GetRoomUID() ) )
					<< END_LOG;

				kPacket.m_iOK = NetError::ERR_RELATIONSHIP_39;
				SendPacket( EGS_CHECK_BREAK_UP_ACK, kPacket );
				return;
			}
			break;
		}
	}

	// 비용 보다 소지금이 많은가?!
	if( GetED() < iCost )
	{
		kPacket.m_iOK = NetError::ERR_RELATIONSHIP_40;
		SendPacket( EGS_CHECK_BREAK_UP_ACK, kPacket );
		return;
	}

	kPacket.m_iOK = NetError::NET_OK;
    SendPacket( EGS_CHECK_BREAK_UP_ACK, kPacket );
}

IMPL_ON_FUNC_NOPARAM( EGS_BREAK_UP_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_ROOM, KGSFSM::S_FIELD_MAP ) );

	KEGS_BREAK_UP_ACK kPacket;

	if( m_kUserRelationshipManager.IsCouple() == false && m_kUserRelationshipManager.IsMarried() == false )
	{
		START_LOG( cerr, L"이별 가능한 상태가 아닙니다." )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOGc( m_kUserRelationshipManager.GetRelationshipType() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_RELATIONSHIP_36;
		SendPacket( EGS_BREAK_UP_ACK, kPacket );
		return;
	}

	if( GetRoomUID() > 0 )
	{
		switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
		{
		case CXSLRoom::RT_BATTLE_FIELD:
			{
				// 정상
			}
			break;

		default:
			{
				START_LOG( cerr, L"이별 가능한 위치가 아닙니다." )
					<< BUILD_LOG( GetCharName() )
					<< BUILD_LOGc( m_kUserRelationshipManager.GetRelationshipType() )
					<< BUILD_LOG( CXSLRoom::GetRoomType( GetRoomUID() ) )
					<< END_LOG;

				kPacket.m_iOK = NetError::ERR_RELATIONSHIP_39;
				SendPacket( EGS_BREAK_UP_ACK, kPacket );
				return;
			}
			break;
		}
	}

	int iCost = m_kUserRelationshipManager.GetBreakUpCost();

	// 비용 보다 소지금이 많은가?!
	if( GetED() < iCost )
	{
		kPacket.m_iOK = NetError::ERR_RELATIONSHIP_40;
		SendPacket( EGS_BREAK_UP_ACK, kPacket );
		return;
	}

	KDBE_BREAK_UP_REQ kPacketDB;
	kPacketDB.m_iCost = iCost;
	kPacketDB.m_iMyUnitUID = GetCharUID();
	kPacketDB.m_iLoverUnitUID = m_kUserRelationshipManager.GetLoverUnitUID();
	SendToGameDB( DBE_BREAK_UP_REQ, kPacketDB );
}

IMPL_ON_FUNC( DBE_BREAK_UP_ACK )
{
	KEGS_BREAK_UP_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	SendPacket( EGS_BREAK_UP_ACK, kPacket );

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		//{{ 2012. 10. 02	최육사		ED&아이템 모니터링 시스템
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
		DecreaseED( kPacket_.m_iCost, KUserEDManager::ER_OUT_ED_BREAK_UP_RELATIONSHIP );
#else
		int iBeforeED = m_iED;
		m_iED -= kPacket_.m_iCost;

		// ED 어뷰저
		m_kUserAbuserManager.CheckEDAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::ELS_BREAK_UP_RELATIONSHIP_ED_COST, iBeforeED );
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
		//}}

		// 인연 정보 초기화
		m_kUserRelationshipManager.SetBreakUp();

		// 헤어지자고 한 당사자에게 전달
		KEGS_BREAK_UP_NOT kPacketNot;
		kPacketNot.m_iCost = kPacket_.m_iCost;
		SendPacket( EGS_BREAK_UP_NOT, kPacketNot );

		// 솔로 상태로 변경
		CTime tDate = CTime::GetCurrentTime();
		std::wstring wstrEmpty;
		m_kUserRelationshipManager.SetAnniversaryDay( tDate );
		m_kUserRelationshipManager.SetRelationshipType( SEnum::RT_SOLO );
		m_kUserRelationshipManager.SetLoverUnitUID( 0 );
		m_kUserRelationshipManager.SetLoverNickName( wstrEmpty );
		m_kUserRelationshipManager.SetLoveWord( wstrEmpty );
		m_kUserRelationshipManager.SetLastRewardDate( tDate );
		m_kUserRelationshipManager.SetRewardTitleType( 0 );

		KELG_BREAK_UP_NOT kNot;
		kNot.m_iLoverUnitUID = kPacket_.m_iLoverUnitUID;
		SendToLoginServer( ELG_BREAK_UP_NOT, kNot );

		// 상대방에게 우편을 보내자잉
		KDBE_INSERT_REWARD_TO_POST_REQ kPacketToDB;
		kPacketToDB.m_iFromUnitUID	= GetCharUID();
		kPacketToDB.m_iToUnitUID	= kPacket_.m_iLoverUnitUID;
		kPacketToDB.m_iRewardType	= KPostItemInfo::LT_BREAK_UP;
		kPacketToDB.m_iRewardID		= 0;
		SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDB );

		// 로그 기록
		// 나
		CTime tCurr = CTime::GetCurrentTime();
		KDBE_WRITE_RELATIONSHIP_INFO_LOG_REQ kLog;
		kLog.m_iUserUID = GetUID();
		kLog.m_iUnitUID = GetCharUID();
		kLog.m_ucLevel = GetLevel();
		kLog.m_cUnitClass = GetUnitClass();
		kLog.m_cCoupleType = 3;
		kLog.m_wstrRegDate = ( std::wstring )( tCurr.Format( _T( "%Y-%m-%d %H:%M:%S" ) ) );
		SendToLogDB( DBE_WRITE_RELATIONSHIP_INFO_LOG_REQ, kLog );

		// 상대편
		kLog.m_iUserUID = 0;
		kLog.m_iUnitUID = kPacket_.m_iLoverUnitUID;
		kLog.m_ucLevel = 0;
		kLog.m_cUnitClass = 0;
		SendToLogDB( DBE_WRITE_RELATIONSHIP_INFO_LOG_REQ, kLog );

		// 신청자가 위치한 곳에 따라 그 주변인 에게 헤어지기 알림 전달(브로드캐스팅)
		if( GetStateID() == KGSFSM::S_ROOM )
		{
			SendToCnRoom( ERM_BREAK_UP_SUCCESS_NOT, char() );
		}
		else
		{
			if( GetFieldUID() > 0 )
			{
				SendToGSField( ERM_BREAK_UP_SUCCESS_NOT, char() );
			}
		}
	}
}

IMPL_ON_FUNC( ELG_BREAK_UP_NOT )
{
	// 솔로 상태로 변경
	CTime tDate = CTime::GetCurrentTime();
	std::wstring wstrEmpty;
	m_kUserRelationshipManager.SetAnniversaryDay( tDate );
	m_kUserRelationshipManager.SetRelationshipType( SEnum::RT_SOLO );
	m_kUserRelationshipManager.SetLoverUnitUID( 0 );
	m_kUserRelationshipManager.SetLoverNickName( wstrEmpty );
	m_kUserRelationshipManager.SetLoveWord( wstrEmpty );
	m_kUserRelationshipManager.SetLastRewardDate( tDate );
	m_kUserRelationshipManager.SetRewardTitleType( 0 );

	KEGS_BREAK_UP_NOT kPacket;
	kPacket.m_iCost = 0;
	SendPacket( EGS_BREAK_UP_NOT, kPacket );

	// 신청자가 위치한 곳에 따라 그 주변인 에게 헤어지기 알림 전달(브로드캐스팅)
	if( GetStateID() == KGSFSM::S_ROOM )
	{
		SendToCnRoom( ERM_BREAK_UP_SUCCESS_NOT, char() );
	}
	else
	{
		if( GetFieldUID() > 0 )
		{
			SendToGSField( ERM_BREAK_UP_SUCCESS_NOT, char() );
		}
	}
}

IMPL_ON_FUNC_NOPARAM( EGS_START_WEDDING_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	// 결혼 시작은 커플 상태에서만 가능
	if( m_kUserRelationshipManager.IsCouple() == false )
	{
		START_LOG( cerr, L"결혼 가능한 상태가 아닙니다." )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOGc( m_kUserRelationshipManager.GetRelationshipType() )
			<< END_LOG;
		KEGS_START_WEDDING_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_RELATIONSHIP_36;
		SendPacket( EGS_START_WEDDING_ACK, kPacket );
		return;
	}

	if( GetRoomUID() > 0 )
	{
		switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
		{
		case CXSLRoom::RT_WEDDING_HALL:
			{
				_SendToCnRoom( GetRoomUID(), ERM_CHECK_START_WEDDING_REQ, char() );
				return;
			}
			break;

		default:
			{
				START_LOG( cerr, L"가능한 위치가 아닙니다." )
					<< BUILD_LOG( GetCharName() )
					<< BUILD_LOGc( m_kUserRelationshipManager.GetRelationshipType() )
					<< BUILD_LOG( CXSLRoom::GetRoomType( GetRoomUID() ) )
					<< END_LOG;

				KEGS_START_WEDDING_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_RELATIONSHIP_39;
				SendPacket( EGS_START_WEDDING_ACK, kPacket );
				return;
			}
			break;
		}
	}

	KEGS_START_WEDDING_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_RELATIONSHIP_36;
	SendPacket( EGS_START_WEDDING_ACK, kPacket );
}

IMPL_ON_FUNC( ERM_CHECK_START_WEDDING_ACK )
{
	KEGS_START_WEDDING_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	SendPacket( EGS_START_WEDDING_ACK, kPacket );
}

IMPL_ON_FUNC_NOPARAM( ERM_CHECK_START_WEDDING_NOT )
{
	SendID( EGS_START_WEDDING_ASK_REQ );
}

IMPL_ON_FUNC( EGS_START_WEDDING_ASK_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	// 결혼 시작은 커플 상태에서만 가능
	if( m_kUserRelationshipManager.IsCouple() == false )
	{
		START_LOG( cerr, L"결혼 가능한 상태가 아닙니다." )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOGc( m_kUserRelationshipManager.GetRelationshipType() )
			<< END_LOG;
		return;
	}

	if( GetRoomUID() > 0 )
	{
		switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
		{
		case CXSLRoom::RT_WEDDING_HALL:
			{
				_SendToCnRoom( GetRoomUID(), ERM_START_WEDDING_ASK_NOT, kPacket_ );
				return;
			}
			break;
		default:
			{
				START_LOG( cerr, L"결혼식방에 입장한 상태가 아니다?!" )
					<< BUILD_LOG( GetCharName() )
					<< BUILD_LOGc( m_kUserRelationshipManager.GetRelationshipType() )
					<< END_LOG;
				return;
			}
		}
	}
}

_IMPL_ON_FUNC( ERM_START_WEDDING_AGREE_NOT, KEGS_START_WEDDING_AGREE_NOT )
{
	SendPacket( EGS_START_WEDDING_AGREE_NOT, kPacket_ );
}

IMPL_ON_FUNC( ERM_START_WEDDING_NOT )
{
	KEGS_START_WEDDING_NOT kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	SendPacket( EGS_START_WEDDING_NOT, kPacket );

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		if( kPacket_.m_iGroom == GetCharUID() || kPacket_.m_iBride == GetCharUID() )
		{
			// 결혼 상태 변경
			CTime tCurr = CTime::GetCurrentTime(); 
			m_kUserRelationshipManager.SetRelationshipType( SEnum::RT_MARRIED );
			m_kUserRelationshipManager.SetAnniversaryDay( tCurr );
			m_kUserRelationshipManager.SetLastRewardDate( tCurr );
			m_kUserRelationshipManager.SetRewardTitleType( 0 );
		}

		// 보상을 지급하자
		if( kPacket_.m_mapRewardItem.empty() == false )
		{
			std::map<int, int>::iterator mit = kPacket_.m_mapRewardItem.begin();
			for( ; mit != kPacket_.m_mapRewardItem.end() ; ++mit )
			{
				// DB로 보상 정보를 보낸다.
				KDBE_INSERT_REWARD_TO_POST_REQ kPacketToDB;
				kPacketToDB.m_iFromUnitUID	= GetCharUID();
				kPacketToDB.m_iToUnitUID	= GetCharUID(); 
				kPacketToDB.m_iRewardType	= KPostItemInfo::LT_WEDDING_REWARD; // 보상 타입
				kPacketToDB.m_iRewardID		= mit->first;
				kPacketToDB.m_sQuantity		= mit->second;
				SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDB );
			}
		}

		if( kPacket_.m_iGroom == GetCharUID() )
		{
			// 전체 공지 - 신랑이 공지 담당하자
			KEGS_NOTIFY_MSG_NOT	kNotify;
			kNotify.m_cNotifyType = KEGS_NOTIFY_MSG_NOT::NT_WEDDING_SUCCESS; // 결혼 성공 알림
			kNotify.m_Count = 1;
#ifdef SERV_RELATIONSHIP_SYSTEM_INT
			kNotify.m_wstrMSG = GetCharName() + L" & " + m_kUserRelationshipManager.GetLoverNickName();
#else
			kNotify.m_wstrMSG = GetCharName() + L"님과 " + m_kUserRelationshipManager.GetLoverNickName();
#endif SERV_RELATIONSHIP_SYSTEM_INT
			SendToCnServer( ECN_NOTIFY_MSG_NOT, kNotify );
		}
	}
}

IMPL_ON_FUNC( EGS_CHANGE_LOVE_WORD_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

#ifdef SERV_STRING_FILTER_USING_DB
	if( GetAuthLevel() < SEnum::UAL_GM )
	{
		kPacket_.m_wstrNewLoveWord = SiKStringFilterManager()->FilteringNoteString( kPacket_.m_wstrNewLoveWord.c_str(), L'♡' );
	}
#endif //SERV_STRING_FILTER_USING_DB

	// 인연 상태 확인
	if( m_kUserRelationshipManager.GetRelationshipType() != SEnum::RT_MARRIED )
	{
		KEGS_CHANGE_LOVE_WORD_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_RELATIONSHIP_45;
		SendPacket( EGS_CHANGE_LOVE_WORD_ACK, kPacket );
		return;
	}

	// 아이템이 애칭 변경권이 맞는가?
	int iItemID = m_kInventory.GetItemID( kPacket_.m_iItemUID );
	if( iItemID <= 0 )
	{
		// 가지고 있는 아이템이 이상하다
		KEGS_CHANGE_LOVE_WORD_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_RELATIONSHIP_44;
		SendPacket( EGS_CHANGE_LOVE_WORD_ACK, kPacket );
		return;
	}

	// 애칭 변경권이 맞는가?
	if( iItemID != CXSLItem::SI_LOVE_WORD_CHANGE_ITEM_ITEM )
	{
		KEGS_CHANGE_LOVE_WORD_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_RELATIONSHIP_44;
		SendPacket( EGS_CHANGE_LOVE_WORD_ACK, kPacket );
		return;
	}

	// 지우자
	std::map< UidType, int > mapDeleteItem;
	std::map< int, KItemInfo> mapDummy;
	std::vector<KItemInfo> vecNewItem;
	std::vector< KInventoryItemInfo > vecUpdatedInventorySlot;

	// 하나만 지우자
	mapDeleteItem.insert( std::make_pair( kPacket_.m_iItemUID, 1 ) );
	if( m_kInventory.DeleteAndInsert( mapDeleteItem, mapDummy, vecUpdatedInventorySlot, vecNewItem, KDeletedItemInfo::DR_RELATIONSHIP ) == false )
	{
		// 아이템 삭제 실패
		KEGS_CHANGE_LOVE_WORD_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_RELATIONSHIP_11;
		SendPacket( EGS_CHANGE_LOVE_WORD_ACK, kPacket );
		return;
	}

	KDBE_CHANGE_LOVE_WORD_REQ kPacketDB;
	kPacketDB.m_iUseUnitUID = GetCharUID();
	kPacketDB.m_iLoverUnitUID = m_kUserRelationshipManager.GetLoverUnitUID();
	kPacketDB.m_wstrNewLoveWord = kPacket_.m_wstrNewLoveWord;
	kPacketDB.m_vecUpdatedInventorySlot = vecUpdatedInventorySlot;
	m_kInventory.FlushDeletedItem( kPacketDB.m_kItemQuantityUpdate.m_vecDeleted );
	m_kInventory.FlushQuantityChange( kPacketDB.m_kItemQuantityUpdate.m_mapQuantityChange );
	SendToGameDB( DBE_CHANGE_LOVE_WORD_REQ, kPacketDB );
}

IMPL_ON_FUNC( DBE_CHANGE_LOVE_WORD_ACK )
{
	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		m_kInventory.RollBackDeletedItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted );
		m_kInventory.RollBackInitQuantity( kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange );

		KEGS_CHANGE_LOVE_WORD_ACK kPacket;
		kPacket.m_iOK = kPacket_.m_iOK;
		SendPacket( EGS_CHANGE_LOVE_WORD_ACK, kPacket );
		return;
	}

	// 애칭 변경 성공 했다
	m_kUserRelationshipManager.SetLoveWord( kPacket_.m_wstrNewLoveWord );

	KEGS_CHANGE_LOVE_WORD_ACK kAck;
	kAck.m_iOK = kPacket_.m_iOK;
	SendPacket( EGS_CHANGE_LOVE_WORD_ACK, kAck );

	KEGS_CHANGE_LOVE_WORD_NOT kPacket;
	kPacket.m_iUseItemUnitUID = kPacket_.m_iUseUnitUID;
	kPacket.m_wstrNewLoveWord = kPacket_.m_wstrNewLoveWord;
	kPacket.m_vecUpdatedInventorySlot = kPacket_.m_vecUpdatedInventorySlot;
	SendPacket( EGS_CHANGE_LOVE_WORD_NOT, kPacket );

	// 연인 에게도 알려주자
	KELG_CHANGE_LOVE_WORD_NOT kNot;
	kNot.m_iUseUnitUID = kPacket_.m_iUseUnitUID;
	kNot.m_iLoverUnitUID = kPacket_.m_iLoverUnitUID;
	kNot.m_wstrNewLoveWord = kPacket_.m_wstrNewLoveWord;
	SendToLoginServer( ELG_CHANGE_LOVE_WORD_NOT, kNot );

	// 로그 기록
	CTime tCurr = CTime::GetCurrentTime();
	// 나
	KDBE_WRITE_RELATIONSHIP_INFO_LOG_REQ kLog;
	kLog.m_iUserUID = GetUID();
	kLog.m_iUnitUID = GetCharUID();
	kLog.m_ucLevel = GetLevel();
	kLog.m_cUnitClass = GetUnitClass();
	kLog.m_cCoupleType = 4;
	kLog.m_wstrRegDate = ( std::wstring )( tCurr.Format( _T( "%Y-%m-%d %H:%M:%S" ) ) );
	SendToLogDB( DBE_WRITE_RELATIONSHIP_INFO_LOG_REQ, kLog );

	// 상대
	kLog.m_iUserUID = 0;
	kLog.m_iUnitUID = kPacket_.m_iLoverUnitUID;
	kLog.m_ucLevel = 0;
	kLog.m_cUnitClass = 0;
	SendToLogDB( DBE_WRITE_RELATIONSHIP_INFO_LOG_REQ, kLog );
}

IMPL_ON_FUNC( ELG_CHANGE_LOVE_WORD_NOT )
{
	// 애칭 변경 성공 했다
	m_kUserRelationshipManager.SetLoveWord( kPacket_.m_wstrNewLoveWord );

	KEGS_CHANGE_LOVE_WORD_NOT kPacket;
	kPacket.m_iUseItemUnitUID = kPacket_.m_iUseUnitUID;
	kPacket.m_wstrNewLoveWord = kPacket_.m_wstrNewLoveWord;
	SendPacket( EGS_CHANGE_LOVE_WORD_NOT, kPacket );
}

IMPL_ON_FUNC( DBE_INSERT_WEDDING_REWARD_ACK )
{
	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"결혼시스템 기념일, 타이틀 보상 지급 실패!" )
			<< END_LOG;

		return;
	}

	CTime tCurr = CTime::GetCurrentTime();

	// 지급 성공했으므로
	if( kPacket_.m_bTitleReward == true )
	{
		m_kUserRelationshipManager.SetRewardTitleType( kPacket_.m_iTitleRewardStep );

		// 타이틀 보상
		KDBE_WRITE_RELATIONSHIP_INFO_LOG_REQ kLog;
		kLog.m_iUserUID = GetUID();
		kLog.m_iUnitUID = GetCharUID();
		kLog.m_ucLevel = GetLevel();
		kLog.m_cUnitClass = GetUnitClass();
		kLog.m_cCoupleType = 6;
		kLog.m_wstrRegDate = ( std::wstring )( tCurr.Format( _T( "%Y-%m-%d %H:%M:%S" ) ) );
		SendToLogDB( DBE_WRITE_RELATIONSHIP_INFO_LOG_REQ, kLog );

	}
	else
	{
		CTime tCurr = CTime::GetCurrentTime();
		m_kUserRelationshipManager.SetLastRewardDate( tCurr );

		// 기념일 보상
		KDBE_WRITE_RELATIONSHIP_INFO_LOG_REQ kLog;
		kLog.m_iUserUID = GetUID();
		kLog.m_iUnitUID = GetCharUID();
		kLog.m_ucLevel = GetLevel();
		kLog.m_cUnitClass = GetUnitClass();
		kLog.m_cCoupleType = 5;
		kLog.m_wstrRegDate = ( std::wstring )( tCurr.Format( _T( "%Y-%m-%d %H:%M:%S" ) ) );
		SendToLogDB( DBE_WRITE_RELATIONSHIP_INFO_LOG_REQ, kLog );
	}
}

IMPL_ON_FUNC( DBE_WRITE_RELATIONSHIP_INFO_LOG_ACK )
{
	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"결혼시스템 기념일, 로그 기록 실패!" )
			<< END_LOG;

		return;
	}
}
#endif SERV_RELATIONSHIP_SYSTEM
//}