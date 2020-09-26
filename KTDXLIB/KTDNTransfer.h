#pragma once

#ifdef CONVERSION_VS
//더이상 사용되지 않는 클래스
#else CONVERSION_VS

class CKTDNDP;
class CKTDNTransfer
{
	public:
		enum NETWORK_TYPE
		{
			NT_DIRECT_PLAY,
			NT_TCP_RELAY,
		};

		struct MergePacketData
		{
			bool	broadCast;
			int		destUserID;
			int		size;
			BYTE*	pData;

			MergePacketData()
			{
				broadCast	= false;
				destUserID	= -1;
				size		= 0;
				pData		= NULL;
			}
			~MergePacketData()
			{
				broadCast	= false;
				destUserID	= -1;
				size		= 0;

				SAFE_DELETE_ARRAY( pData );
			}
			void SetData( CKTDNPacket::SPT_BASIC* pPacket, int INsize )
			{
				size		= INsize;
				pData		= new BYTE[size];
				memcpy( pData, pPacket, sizeof(BYTE) * size );
			}
		};

	public:
		CKTDNTransfer();
		~CKTDNTransfer(void);

		virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
	
		void SetNetwork( CKTDNDP* pKTDNDP );
		void SetNetwork( NETWORK_TYPE networkType );
		void Receive( int myID, int senderID, int size, CKTDNPacket::SPT_BASIC* pData );

		void Send( const int userID, const int size, CKTDNPacket::SPT_BASIC* pPacket );
		void BroadCast( const int size, CKTDNPacket::SPT_BASIC* pPacket );

		void PushSend( const int userID, const int size, CKTDNPacket::SPT_BASIC* pPacket );
		void PushBroadCast( const int size, CKTDNPacket::SPT_BASIC* pPacket );

	protected:
		virtual void PacketHandler( CKTDNPacket::PacketData* pPacketData );
		void FlushPacket();

		void PacketSplit( CKTDNPacket::PacketData* pPacketData );
		void PacketMergeSend( const bool bBroadCast, const int destUserID );

		NETWORK_TYPE						m_NetworkType;
		CKTDNDP*							m_pKTDNDP;
		CRITICAL_SECTION					m_PacketLock;

		float								m_MyLatencyTime;
		float								m_fElapsedTime;
		vector<CKTDNPacket::PacketData*>	m_ReceivePacketList;
		vector<MergePacketData*>			m_MergePacketList;
		vector<MergePacketData*>			m_NowSendList;

};
#endif CONVERSION_VS