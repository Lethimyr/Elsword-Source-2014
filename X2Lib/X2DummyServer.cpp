#include "StdAfx.h"
#include ".\x2dummyserver.h"

CX2DummyServer::CX2DummyServer(void)
{
}

CX2DummyServer::~CX2DummyServer(void)
{
	while( m_RecvMsgQ.empty() != true )
	{
		char* pBuffer = m_RecvMsgQ.front();
		SAFE_DELETE_ARRAY( pBuffer );
		m_RecvMsgQ.pop();
	}
}

HRESULT CX2DummyServer::OnFrameMove( double fTime, float fElapsedTime )
{
	MsgHandler();

	return S_OK;
}

bool CX2DummyServer::SendToServer( CX2ServerPacket::SPBasic* pPacket, int size )
{
	char* pBuffer = new char[size];
	memcpy( pBuffer, pPacket, sizeof(char)*size );
	m_RecvMsgQ.push( pBuffer );

	return true;
}

void CX2DummyServer::MsgHandler()
{
	while( m_RecvMsgQ.empty() != true )
	{
		char* pBuffer = m_RecvMsgQ.front();

		CX2ServerPacket::SPBasic* pBasicPacket = (CX2ServerPacket::SPBasic*)pBuffer;
		switch( pBasicPacket->serverPacketType )
		{
			case CX2ServerPacket::SPT_LOGIN_REQ:
				{
					LoginProcess( (CX2ServerPacket::SPLoginReq*)pBasicPacket );
				}
				break;

			case CX2ServerPacket::SPT_USER_INFO_REQ:
				{
					UserInfoProcess( (CX2ServerPacket::SPUserInfoReq*)pBasicPacket );
				}
				break;

			case CX2ServerPacket::SPT_LOGIN_UNIT_INFO_REQ:
				{
					LoginUnitInfoProcess( (CX2ServerPacket::SPLoginUnitInfoReq*)pBasicPacket );
				}
				break;

			default:
				break;
		}

		SAFE_DELETE_ARRAY( pBuffer );
		m_RecvMsgQ.pop();
	}
}

void CX2DummyServer::LoginProcess( CX2ServerPacket::SPLoginReq* pLoginPacket )
{
	CX2ServerPacket::SPLoginAck* pLoginAck = new CX2ServerPacket::SPLoginAck;
	pLoginAck->serverPacketType	= CX2ServerPacket::SPT_LOGIN_ACK;
	pLoginAck->bAcceptID		= true;
	pLoginAck->bAcceptPassword	= true;

	PostMessage( g_pKTDXApp->GetHWND(), XGM_RECEIVE_FROM_SERVER, (WPARAM)pLoginAck, NULL );
}

void CX2DummyServer::UserInfoProcess( CX2ServerPacket::SPUserInfoReq* pUserInfoPacket )
{
	//UID로 유저 찾았다 치고
	CX2ServerPacket::SPUserInfoAck* pUserInfoAck = new CX2ServerPacket::SPUserInfoAck;
	pUserInfoAck->serverPacketType	= CX2ServerPacket::SPT_USER_INFO_ACK;
	
	pUserInfoAck->userData.userUID					= pUserInfoPacket->userUID;
	swprintf( pUserInfoAck->userData.userID,		L"shice" );
	swprintf( pUserInfoAck->userData.userNickName,	L"아키캐드" );
	swprintf( pUserInfoAck->userData.userName,		L"류금태" );
	pUserInfoAck->userData.haveUnitCount			= 1;		//내가 가진 캐릭터 수
	pUserInfoAck->userData.maxUnitCount				= 3;		//내가 만들 수 있는 캐릭터 수(가지고 있는것 포함)
	pUserInfoAck->userData.cash						= 10000;	//캐쉬

	PostMessage( g_pKTDXApp->GetHWND(), XGM_RECEIVE_FROM_SERVER, (WPARAM)pUserInfoAck, NULL );
}

void CX2DummyServer::LoginUnitInfoProcess( CX2ServerPacket::SPLoginUnitInfoReq* pLoginUnitInfoPacket )
{
	CX2ServerPacket::SPLoginUnitInfoAck* pLoginUnitInfoAck = new CX2ServerPacket::SPLoginUnitInfoAck;
	pLoginUnitInfoAck->serverPacketType	= CX2ServerPacket::SPT_LOGIN_UNIT_INFO_ACK;

	pLoginUnitInfoAck->unitData.unitUID				= 0;
	swprintf( pLoginUnitInfoAck->unitData.nickName,	L"아키엘소드" );
	pLoginUnitInfoAck->unitData.unitType			= CX2Unit::UT_ELSWORD;
	pLoginUnitInfoAck->unitData.unitClass			= CX2Unit::UC_ELSWORD_SWORDMAN;
	pLoginUnitInfoAck->unitData.level				= 1;
	pLoginUnitInfoAck->unitData.EXP					= 0;
	pLoginUnitInfoAck->unitData.GP					= 10000;

	PostMessage( g_pKTDXApp->GetHWND(), XGM_RECEIVE_FROM_SERVER, (WPARAM)pLoginUnitInfoAck, NULL );
}





