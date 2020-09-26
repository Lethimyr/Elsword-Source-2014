#pragma once

class CKTDNPacket
{
	public:
		enum PACKET_TYPE
		{
			PT_BASIC = 0,
			PT_MERGE,

			PT_OPEN_HOST,
			PT_CONNECT_COMPLETE,
			PT_NEW_CONNECT,
			PT_DESTROY_CONNECT,
			PT_HOST_MIGRATE,

			PT_END,
		};

		struct SPT_BASIC
		{
			char			packetType;

			SPT_BASIC()
			{
				packetType		= PT_BASIC;
			}
		};

		struct PacketData
		{
			bool			bLocal;
			PACKET_TYPE		packetType;
			int				senderID;
			int				size;
			float			fLatencyTime;
			SPT_BASIC*		pPacket;

			PacketData()
			{
				bLocal			= false;
				packetType		= PT_BASIC;
				senderID		= 0;
				size			= 0;
				fLatencyTime	= 0.0f;
				pPacket			= NULL;
			}
			~PacketData()
			{
				BYTE* pBuffer = (BYTE*)pPacket;
				SAFE_DELETE_ARRAY( pBuffer );
			}
			void SetData( int INmyID, int INsenderID, int INsize, SPT_BASIC* pData )
			{
				if( INmyID == INsenderID )
					bLocal = true;
				else
					bLocal = false;

				packetType		= (PACKET_TYPE)pData->packetType;
				senderID		= INsenderID;
				size			= INsize;

				BYTE* pBuffer		= new BYTE[INsize];
				memcpy( pBuffer, pData, sizeof(BYTE) * INsize );
				pPacket	= (SPT_BASIC*)pBuffer;
			}
			void SetData( bool INlocal, int INsenderID, int INsize, SPT_BASIC* pData )
			{
				packetType		= (PACKET_TYPE)pData->packetType;
				bLocal			= INlocal;
				senderID		= INsenderID;
				size			= INsize;

				BYTE* pBuffer		= new BYTE[INsize];
				memcpy( pBuffer, pData, sizeof(BYTE) * INsize );
				pPacket	= (SPT_BASIC*)pBuffer;
			}
		};

		
		struct SPT_MERGE : SPT_BASIC
		{
			int				packetCount;

			SPT_MERGE()
			{
				packetType	= PT_MERGE;
				packetCount	= 0;
			}
		};

		struct SPT_OPEN_HOST : SPT_BASIC
		{
			int		hostID;

			SPT_OPEN_HOST()
			{
				packetType	= PT_OPEN_HOST;				
				hostID	= 0;
			}
		};

		struct SPT_CONNECT_COMPLETE : SPT_BASIC
		{
			int		userID;

			SPT_CONNECT_COMPLETE()
			{
				packetType	= PT_CONNECT_COMPLETE;				
				userID	= 0;
			}
		};

		struct SPT_NEW_CONNECT : SPT_BASIC
		{
			int		newUserID;

			SPT_NEW_CONNECT()
			{
				packetType	= PT_NEW_CONNECT;				
				newUserID	= 0;
			}
		};

		struct SPT_DESTROY_CONNECT : SPT_BASIC
		{
			int		destroyUserID;

			SPT_DESTROY_CONNECT()
			{
				packetType		= PT_NEW_CONNECT;				
				destroyUserID	= 0;
			}
		};

		struct SPT_HOST_MIGRATE : SPT_BASIC
		{
			int		newHostID;

			SPT_HOST_MIGRATE()
			{
				packetType	= PT_NEW_CONNECT;				
				newHostID	= 0;
			}
		};


	public:
		CKTDNPacket(void);
		~CKTDNPacket(void);

};
