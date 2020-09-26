#pragma once

class CX2DummyServer : public CKTDXStage
{
	public:
		

	public:
		CX2DummyServer(void);
		virtual ~CX2DummyServer(void);

		virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
		bool			SendToServer( CX2ServerPacket::SPBasic* pPacket, int size );

	private:
		void MsgHandler();
		void LoginProcess( CX2ServerPacket::SPLoginReq* pLoginPacket );
		void UserInfoProcess( CX2ServerPacket::SPUserInfoReq* pUserInfoPacket );
		void LoginUnitInfoProcess( CX2ServerPacket::SPLoginUnitInfoReq* pLoginUnitInfoPacket );

		queue<char*>	m_RecvMsgQ;
};
