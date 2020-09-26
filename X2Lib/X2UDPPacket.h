#pragma once


//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
#pragma pack( push, 1 )
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

#define P2P_COPYCON_ASSIGNOP( ID ) K##ID(const K##ID& kRight) { *this = kRight; } \
									K##ID& operator=(const K##ID& kRight)

#define P2P_CON_COPYCON_ASSIGNOP( ID ) K##ID() {} \
										K##ID(const K##ID& kRight) { *this = kRight; } \
										K##ID& operator=(const K##ID& kRight)

//#ifndef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//#define DECL_P2P_PACKET(ID) struct K##ID; \
//							SERIALIZE_DEFINE_TAG(K##ID, eTAG_USERCLASS); \
//							SERIALIZE_DECLARE_PUTGET(K##ID); \
//							struct K##ID
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

enum X2_PACKET_TYPE
{
	XPT_BASIC				= CKTDNUDP::SP_END,
	XPT_PORT_CHECK_REQ,
	XPT_PORT_CHECK_ACK,
	XPT_PING_TEST_REQ,
	XPT_PING_TEST_ACK,
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
    XPT_UDP_PACKET_PACK,
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
	XPT_UNIT_NPC_SYNC_PACK,
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
    XPT_UNIT_NPC_MINISYNC_PACK,
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	XPT_UNIT_NPC_FIRST_SYNC_PACK_BY_BATTLE_FIELD,	/// 배틀 필드 에 난입시 방장이 난입 유저에게 처음으로 보내주는 npc 싱크 (구조체 없이 ID만 사용)
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
#ifdef  SERV_PET_SYSTEM
	XPT_UNIT_PET_SYNC_PACK,
//#ifndef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	XPT_UNIT_PET_FIRST_SYNC_PACK_BY_BATTLE_FIELD,	/// 배틀 필드 에 난입시 방장이 난입 유저에게 처음으로 보내주는 pet 싱크 (구조체 없이 ID만 사용)
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
#endif  SERV_PET_SYSTEM
	XPT_UNIT_USER_SYNC_PACK,
	XPT_UNIT_USER_SYNC_PACK_FOR_WEDDING_HALL,		/// 예식장 싱크패킷.
};


//-------------------------------------------------------------------------------------------------------------------------------
//DECL_P2P_PACKET( XPT_CONNECT_SERVER_REQ )
//{
//	bool		m_bDummy;
//
//	P2P_CON_COPYCON_ASSIGNOP( XPT_CONNECT_SERVER_REQ )
//	{
//		m_bDummy	= kRight.m_bDummy;
//
//		return *this;
//	}
//};

//-------------------------------------------------------------------------------------------------------------------------------
//DECL_P2P_PACKET( XPT_CONNECT_SERVER_ACK )
//{
//	std::wstring	m_IP;
//	int				m_Port;
//	int				m_UDPPort;
//
//	P2P_CON_COPYCON_ASSIGNOP( XPT_CONNECT_SERVER_ACK )
//	{
//		m_IP	= kRight.m_IP;
//		m_Port	= kRight.m_Port;
//		m_UDPPort = kRight.m_UDPPort;
//
//		return *this;
//	}
//};

//-------------------------------------------------------------------------------------------------------------------------------
//DECL_P2P_PACKET( XPT_CHECK_SERVER_ALIVE_REQ )
//{
//	int		UID;
//
//	P2P_CON_COPYCON_ASSIGNOP( XPT_CHECK_SERVER_ALIVE_REQ )
//	{
//		UID		= kRight.UID;
//
//		return *this;
//	}
//};
//-------------------------------------------------------------------------------------------------------------------------------
//DECL_P2P_PACKET( XPT_CHECK_SERVER_ALIVE_ACK )
//{
//	int		UID;
//
//	P2P_CON_COPYCON_ASSIGNOP( XPT_CHECK_SERVER_ALIVE_ACK )
//	{
//		UID		= kRight.UID;
//
//		return *this;
//	}
//};

//-------------------------------------------------------------------------------------------------------------------------------
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
struct  KXPT_PORT_CHECK_REQ
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//DECL_P2P_PACKET( XPT_PORT_CHECK_REQ )
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
{
	UidType		m_UserUID;
	//{{ 2013. 1. 9	박세훈	Merge 공인IP 연결 실패시 내부IP로 시도( 박진웅 )
//#ifdef SERV_KTDX_RETRY_USING_INTERNAL_IP
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
    u_long          m_InternalIPAddress;
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	std::wstring	m_wstrInternalIP;
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
	USHORT			m_usInternalPort;
//#endif SERV_KTDX_RETRY_USING_INTERNAL_IP
	//}}

	P2P_CON_COPYCON_ASSIGNOP( XPT_PORT_CHECK_REQ )
	{
		m_UserUID			= kRight.m_UserUID;
		//{{ 2013. 1. 9	박세훈	Merge 공인IP 연결 실패시 내부IP로 시도( 박진웅 )
//#ifdef SERV_KTDX_RETRY_USING_INTERNAL_IP
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
        m_InternalIPAddress	= kRight.m_InternalIPAddress;
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//		m_wstrInternalIP	= kRight.m_wstrInternalIP;
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
		m_usInternalPort	= kRight.m_usInternalPort;
//#endif SERV_KTDX_RETRY_USING_INTERNAL_IP
		//}}
		return *this;
	}
};

//-------------------------------------------------------------------------------------------------------------------------------

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
struct  KXPT_PORT_CHECK_ACK
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//DECL_P2P_PACKET( XPT_PORT_CHECK_ACK )
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
{
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
    u_long                      m_IPAddress;
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	std::wstring				m_IP;
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
	int							m_Port;

	P2P_CON_COPYCON_ASSIGNOP( XPT_PORT_CHECK_ACK )
	{
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
        m_IPAddress	= kRight.m_IPAddress;
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//		m_IP	= kRight.m_IP;
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
		m_Port	= kRight.m_Port;

		return *this;
	}

};

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
struct  KXPT_PING_TEST_REQ
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//DECL_P2P_PACKET( XPT_PING_TEST_REQ )
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
{
	UidType					m_UnitUID;
	DWORD					m_SendTime;
#ifdef ACTIVE_KOG_GAME_PERFORMANCE_CHECK
	bool						m_bRelay;
	UINT						m_uiSendCounter;
#endif//ACTIVE_KOG_GAME_PERFORMANCE_CHECK

	P2P_CON_COPYCON_ASSIGNOP( XPT_PING_TEST_REQ )
	{
		m_UnitUID			= kRight.m_UnitUID;
		m_SendTime			= kRight.m_SendTime;
#ifdef ACTIVE_KOG_GAME_PERFORMANCE_CHECK
		m_bRelay			= kRight.m_bRelay;
		m_uiSendCounter		= kRight.m_uiSendCounter;
#endif//ACTIVE_KOG_GAME_PERFORMANCE_CHECK

		return *this;
	}
};

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
struct  KXPT_PING_TEST_ACK
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//DECL_P2P_PACKET( XPT_PING_TEST_ACK )
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
{
	UidType					m_UnitUID;
	DWORD					m_SendTime;
#ifdef ACTIVE_KOG_GAME_PERFORMANCE_CHECK
	bool						m_bRelay;
	UINT						m_uiSendCounter;
#endif//ACTIVE_KOG_GAME_PERFORMANCE_CHECK
#ifdef  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
    bool                        m_bIgnoreAck;
#endif  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE

	P2P_CON_COPYCON_ASSIGNOP( XPT_PING_TEST_ACK )
	{
		m_UnitUID			= kRight.m_UnitUID;
		m_SendTime			= kRight.m_SendTime;
#ifdef ACTIVE_KOG_GAME_PERFORMANCE_CHECK
		m_bRelay				= kRight.m_bRelay;
		m_uiSendCounter		= kRight.m_uiSendCounter;
#endif//ACTIVE_KOG_GAME_PERFORMANCE_CHECK
#ifdef  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
        m_bIgnoreAck        = kRight.m_bIgnoreAck;
#endif  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
		return *this;
	}
};



//#ifdef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

struct  XPT_UDP_PACKET_PACK_PREFIX
{
	UidType m_iUnitUID;
    DWORD   m_dwFrameMoveCount;
};


struct KDYNAMIC_UNIT_USER_SYNC
{
	enum EUnitUserSync_DataField
	{
		eUnitUserSync_DataField_No = 0,
		eUnitUserSync_DataField_nowState = ( 1 << 0 ),
		eUnitUserSync_DataField_nowAction = ( 1 << 1 ),
		eUnitUserSync_DataField_stateChangeNum = ( 1 << 2 ),
		eUnitUserSync_DataField_PosX = ( 1 << 3 ),
		eUnitUserSync_DataField_PosY = ( 1 << 4 ),
		eUnitUserSync_DataField_PosZ = ( 1 << 5 ),
		eUnitUserSync_DataField_lastTouchLineIndex = ( 1 << 6 ),
		eUnitUserSync_DataField_NowHpPercent = ( 1 << 7 ),
		eUnitUserSync_DataField_NowMpPercent = ( 1 << 8 ),
		eUnitUserSync_DataField_EncodedData = ( 1 << 9 ),
		eUnitUserSync_DataField_EncodedDataFromCannonBallCountAndEtc = ( 1 << 10 ),
		eUnitUserSync_DataField_RandomTableIndex = ( 1 << 11 ),
		eUnitUserSync_DataField_HitCount = ( 1 << 12 ),
		eUnitUserSync_DataField_HittedCount = ( 1 << 13 ),
		eUnitUserSync_DataField_NumOfDeBuff = ( 1 << 14 ),
//#ifdef  X2OPTIMIZE_USER_USER_PUSH_PASS_SYNC
        eUnitUserSync_DataField_RelativePos = ( 1 << 15 ),
//#endif  X2OPTIMIZE_USER_USER_PUSH_PASS_SYNC
		eUnitUserSync_DataField_All = eUnitUserSync_DataField_nowState | 
													eUnitUserSync_DataField_nowAction | 
													eUnitUserSync_DataField_stateChangeNum | 
													eUnitUserSync_DataField_PosX | 
													eUnitUserSync_DataField_PosY | 
													eUnitUserSync_DataField_PosZ | 
													eUnitUserSync_DataField_lastTouchLineIndex | 
													eUnitUserSync_DataField_NowHpPercent | 
													eUnitUserSync_DataField_NowMpPercent | 
													eUnitUserSync_DataField_EncodedData | 
													eUnitUserSync_DataField_EncodedDataFromCannonBallCountAndEtc | 
													eUnitUserSync_DataField_RandomTableIndex |
													eUnitUserSync_DataField_HitCount |
													eUnitUserSync_DataField_HittedCount |
													eUnitUserSync_DataField_NumOfDeBuff
//#ifdef  X2OPTIMIZE_USER_USER_PUSH_PASS_SYNC
                                                    | eUnitUserSync_DataField_RelativePos
//#endif  X2OPTIMIZE_USER_USER_PUSH_PASS_SYNC
	};

    USHORT              m_usDataFieldFlag;
	UCHAR				m_ucNowState;
	UCHAR				m_ucNowAction;
	char				m_cStateChangeNum;

	USHORT              m_usPosX;
//#ifdef  X2OPTIMIZE_USER_USER_PUSH_PASS_SYNC
    float               m_fPosY;
//#else   X2OPTIMIZE_USER_USER_PUSH_PASS_SYNC
//    USHORT              m_usPosY;
//#endif  X2OPTIMIZE_USER_USER_PUSH_PASS_SYNC
	USHORT              m_usPosZ;
	UCHAR               m_ucLastTouchLineIndex;

	//UCHAR				m_ucNowHpPercent;
	//UCHAR				m_ucNowMpPercent;
	float				m_fNowHp;
	float				m_fNowMp;

	char				m_cEncodedData;     // bool	bIsRight(1bit); bool bFrameStop(1bit), directchange(1bit), statechangenum(3bit), hypermodecount(last 2bit)

#ifdef SERV_9TH_NEW_CHARACTER // 김태환
	SHORT				m_sEncodedDataFromCannonBallCountAndEtc;	/// 1바이트에서 2바이트로 확대 ( 신캐릭터 애드의 DP 수치 동기화 때문에 )
#else // SERV_9TH_NEW_CHARACTER
	char				m_cEncodedDataFromCannonBallCountAndEtc;	// int iCannonBallCount(4bit),	// AAAA0000
#endif // SERV_9TH_NEW_CHARACTER

	USHORT				m_usRandomTableIndex;

	/// kimhc // 4바이트로 끊어지게 맞추는게 좋으려나...
	UCHAR				m_ucHitCount;			/// 타격횟수
	UCHAR				m_ucHittedCount;		/// 피격횟수
	UCHAR				m_ucNumOfDeBuff;		/// 디버프개수
    DWORD               m_dwRelativePos;

//#ifdef  X2OPTIMIZE_USER_USER_PUSH_PASS_SYNC
    KDYNAMIC_UNIT_USER_SYNC()
    {
        m_usDataFieldFlag = eUnitUserSync_DataField_No;
        m_ucNowState = 0;
        m_ucNowAction = 0;
        m_cStateChangeNum = 0;
        m_usPosX = 0;
        m_fPosY = 0.f;
        m_usPosZ = 0;
        m_ucLastTouchLineIndex = 0;
        m_fNowHp = 0.f;
        m_fNowMp = 0.f;
        m_cEncodedData = 0;
#ifdef SERV_9TH_NEW_CHARACTER // 김태환
		m_sEncodedDataFromCannonBallCountAndEtc = 0;
#else // SERV_9TH_NEW_CHARACTER
		m_cEncodedDataFromCannonBallCountAndEtc = 0;
#endif // SERV_9TH_NEW_CHARACTER
        m_usRandomTableIndex = 0;
        m_ucHitCount = 0;
        m_ucHittedCount = 0;
        m_ucNumOfDeBuff = 0;
        m_dwRelativePos = 0;
    }
//#endif  X2OPTIMIZE_USER_USER_PUSH_PASS_SYNC


	//{{ kimhc // 2010.12.13 // 2010-12-23 New Character CHUNG
	int DecodeCannonBallCount() const
	{
#ifdef SERV_9TH_NEW_CHARACTER // 김태환
		/// 상위 7bit 사용		이동 포격용 케논볼 추가로 인해 들어가야 할 값이 최대 48로 커져서, 7비트로 변경
		int iCannonBallCount = static_cast<int>( m_sEncodedDataFromCannonBallCountAndEtc );
		return iCannonBallCount;
#else // SERV_9TH_NEW_CHARACTER
		/// 상위 7bit 사용		이동 포격용 케논볼 추가로 인해 들어가야 할 값이 최대 48로 커져서, 7비트로 변경
		int iCannonBallCount = static_cast<int>( m_cEncodedDataFromCannonBallCountAndEtc >> 1 );
		return iCannonBallCount;
#endif // SERV_9TH_NEW_CHARACTER
	}

#ifdef SERV_9TH_NEW_CHARACTER // 김태환
	/// 신캐릭터 애드 DP 수치로 인하여, 2바이트로 확대
	void EncodeCannonBallCount( IN const SHORT usCannonBallCount_ )
	{
		/// 기존의 변수를 완전히 비우고, 대입
		m_sEncodedDataFromCannonBallCountAndEtc =  0;
		m_sEncodedDataFromCannonBallCountAndEtc |= usCannonBallCount_;
	}
#else // SERV_9TH_NEW_CHARACTER
	void EncodeCannonBallCount( char chCannonBallCount_ )
	{
		/// 상위 7bit 사용		이동 포격용 케논볼 추가로 인해 들어가야 할 값이 최대 48로 커져서, 7비트로 변경
		char chCannonBallCount = chCannonBallCount_ & 0x7F;
		// 상위 7비트로 이동
		chCannonBallCount = chCannonBallCount << 1;

		// 기존의 상위 7비트 삭제
		m_cEncodedDataFromCannonBallCountAndEtc &= 0x01;
		// 기존의 상위 7비트에 추가
		m_cEncodedDataFromCannonBallCountAndEtc |= chCannonBallCount;
	}
#endif // SERV_9TH_NEW_CHARACTER

	//}} kimhc // 2010.12.13 //  2010-12-23 New Character CHUNG
	bool DecodeIsRight()  const
	{  
		if ( m_cEncodedData & 0x80 )
			return true;
		else
			return false;
	}


	void EncodeIsRight( bool bIsRight )
	{
		if( true == bIsRight )
		{
			m_cEncodedData |= 0x80;	// 10000000
		}
		else
		{
			m_cEncodedData &= 0x7F;	// 01111111
		}
	}


	bool DecodeFrameStop()  const
	{  
		if ( m_cEncodedData & 0x40 )
			return true;
		else
			return false;
	}

	void EncodeFrameStop( bool bFrameStop )
	{
		if( true == bFrameStop )
		{
			m_cEncodedData |= 0x40;	// 01000000
		}
		else
		{
			m_cEncodedData &= 0xBF;	// 10111111
		}
	}


	bool DecodeDirectChange()  const
	{  
		if ( m_cEncodedData & 0x20 )
			return true;
		else
			return false;
	}

	void EncodeDirectChange( bool bDirectChange )
	{
		if( true == bDirectChange )
		{
			m_cEncodedData |= 0x20;	// 00100000
		}
		else
		{
			m_cEncodedData &= 0xDF;	// 11011111
		}
	}

	int DecodeHyperModeCount()  const
	{  
		return (int) (m_cEncodedData & 0x03);	
	}

	void EncodeHyperModeCount( char cHyperModeCount )	
	{
		cHyperModeCount &= 0x03;	// 00000011
		m_cEncodedData &= 0xFC;		// 11111100
		m_cEncodedData |= cHyperModeCount;
	}

    void    Reset()
    {
//#ifdef  X2OPTIMIZE_USER_USER_PUSH_PASS_SYNC
        *this = KDYNAMIC_UNIT_USER_SYNC();
//#else   X2OPTIMIZE_USER_USER_PUSH_PASS_SYNC
//        ZeroMemory( this, sizeof(KDYNAMIC_UNIT_USER_SYNC) );
//#endif  X2OPTIMIZE_USER_USER_PUSH_PASS_SYNC
        m_usDataFieldFlag = eUnitUserSync_DataField_No;
    }
    void    Serialize( BYTE*& pbyBuffer, const KDYNAMIC_UNIT_USER_SYNC* pPrevSync );
    bool    Deserialize( const BYTE*& pbyBuffer, const BYTE* pbyEnd );
};



struct KXPT_UNIT_USER_SYNC_PACK_FOR_WEDDING_HALL
{
	UidType				m_iUnitUID;
	char				m_StateID;
	char				m_StateChangeNum;
	float				m_fPosX;
	float				m_fPosY;
	float				m_fPosZ;
	unsigned char		m_LastTouchLineIndex;
	bool				m_bIsRight;
	P2P_CON_COPYCON_ASSIGNOP( XPT_UNIT_USER_SYNC_PACK_FOR_WEDDING_HALL )
	{
		m_iUnitUID				= kRight.m_iUnitUID;	
		m_StateID				= kRight.m_StateID;
		m_StateChangeNum		= kRight.m_StateChangeNum;
		m_fPosX					= m_fPosX;
		m_fPosY					= m_fPosY;
		m_fPosZ					= m_fPosZ;
		m_LastTouchLineIndex	= kRight.m_LastTouchLineIndex;
		m_bIsRight				= kRight.m_bIsRight;
		return *this;
	}
};

//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//
//#ifdef OPTIMIZED_P2P
////{{AFX
////-------------------------------------------------------------------------------------------------------------------------------
//DECL_P2P_PACKET( XPT_UNIT_USER_SYNC )
//{
//    UCHAR				m_ucNowState;
//	UCHAR				nowAction;
//
//    USHORT              m_usPosX;
//    USHORT              m_usPosY;
//    UCHAR               m_ucLastTouchLineIndex;
//
//    char				m_cEncodedData;     // bool	bIsRight(1bit); bool bFrameStop(1bit), directchange(1bit), statechangenum(3bit), hypermodecount(last 2bit)
//
//#ifdef NEW_RANDOM_TABLE_TEST
//	USHORT				m_usRandomTableIndex;
//#else NEW_RANDOM_TABLE_TEST
//
//
//	char				m_RandSeed;
//	char				m_RandSeed2;
//
//#endif NEW_RANDOM_TABLE_TEST
//
//
//	P2P_COPYCON_ASSIGNOP( XPT_UNIT_USER_SYNC )
//	{
//        m_ucNowState            = kRight.m_ucNowState; 
//		nowAction				= kRight.nowAction;
//
//        m_usPosX                = kRight.m_usPosX;
//        m_usPosY                = kRight.m_usPosY;
//        m_ucLastTouchLineIndex  = kRight.m_ucLastTouchLineIndex;
//
//        m_cEncodedData          = kRight.m_cEncodedData;
//
//#ifdef NEW_RANDOM_TABLE_TEST
//
//		m_usRandomTableIndex = kRight.m_usRandomTableIndex;
//		
//#else NEW_RANDOM_TABLE_TEST
//
//
//		m_RandSeed			= kRight.m_RandSeed;
//		m_RandSeed2			= kRight.m_RandSeed2;
//
//#endif NEW_RANDOM_TABLE_TEST
//
//        return *this;
//	}
//
//	bool DecodeIsRight() 
//	{  
//		if ( m_cEncodedData & 0x80 )
//			return true;
//		else
//			return false;
//	}
//
//
//	void EncodeIsRight( bool bIsRight )
//	{
//		if( true == bIsRight )
//		{
//			m_cEncodedData |= 0x80;	// 10000000
//		}
//		else
//		{
//			m_cEncodedData &= 0x7F;	// 01111111
//		}
//	}
//
//
//	bool DecodeFrameStop() 
//	{  
//		if ( m_cEncodedData & 0x40 )
//			return true;
//		else
//			return false;
//	}
//
//	void EncodeFrameStop( bool bFrameStop )
//	{
//		if( true == bFrameStop )
//		{
//			m_cEncodedData |= 0x40;	// 01000000
//		}
//		else
//		{
//			m_cEncodedData &= 0xBF;	// 10111111
//		}
//	}
//
//
//	bool DecodeDirectChange() 
//	{  
//		if ( m_cEncodedData & 0x20 )
//			return true;
//		else
//			return false;
//	}
//
//	void EncodeDirectChange( bool bDirectChange )
//	{
//		if( true == bDirectChange )
//		{
//			m_cEncodedData |= 0x20;	// 00100000
//		}
//		else
//		{
//			m_cEncodedData &= 0xDF;	// 11011111
//		}
//	}
//
//    int DecodeStateChangeNum()
//    {
//        return ( int )( ( ( m_cEncodedData & 0x1C ) >> 2 ) & 0x07 );
//    }
//
//    void EncodeStateChangeNum( int iStateChangeNum )
//    {
//        iStateChangeNum &= 0x00000007;
//        iStateChangeNum = iStateChangeNum << 2;
//        iStateChangeNum &= 0x0000001C;
//        m_cEncodedData |= ( char )iStateChangeNum;
//    }
//
//	int DecodeHyperModeCount() 
//	{  
//		return (int) (m_cEncodedData & 0x03);	
//	}
//
//	void EncodeHyperModeCount( int iHyperModeCount )	
//	{
//		iHyperModeCount &= 0x00000003;      // 00000011
//		m_cEncodedData |= ( char )iHyperModeCount;
//	}
//
//    void DecodeEncodedData( bool& bIsRight, bool& bFrameStop, bool& bDirectChange, int& iStateChangeNum, int& iHyperModeCount )
//    {
//        bIsRight = DecodeIsRight();
//        bFrameStop = DecodeFrameStop();
//        bDirectChange = DecodeDirectChange();
//        iStateChangeNum = DecodeStateChangeNum();
//        iHyperModeCount = DecodeHyperModeCount();
//    }
//
//    void EncodeEncodedData( bool bIsRight, bool bFrameStop, bool bDirectChange, int iStateChangeNum, int iHyperModeCount )
//    {
//        m_cEncodedData = 0;
//        EncodeIsRight( bIsRight );
//        EncodeFrameStop( bFrameStop );
//        EncodeDirectChange( bDirectChange );
//        EncodeStateChangeNum( iStateChangeNum );
//        EncodeHyperModeCount( iHyperModeCount );
//    }
//
//
//};
//
//DECL_P2P_PACKET( XPT_UNIT_USER_SYNC_PACK )
//{
//    UidType                         m_iUnitUID;
//    vector< KXPT_UNIT_USER_SYNC >   m_vecUserSyncList;
//
//    USHORT                          m_usNowHP;
//    USHORT                          m_usNowMP;
//
//    P2P_CON_COPYCON_ASSIGNOP( XPT_UNIT_USER_SYNC_PACK )
//    {
//        m_iUnitUID              = kRight.m_iUnitUID;
//        m_vecUserSyncList       = kRight.m_vecUserSyncList;
//        m_usNowHP               = kRight.m_usNowHP;
//        m_usNowMP               = kRight.m_usNowMP;
//
//        return *this;
//    }
//};
//
//DECL_P2P_PACKET( XPT_UNIT_USER_SYNC_PACK_FOR_WEDDING_HALL )
//{
//	UidType				m_iUnitUID;
//	char				m_StateID;
//	char				m_StateChangeNum;
//	float				m_fPosX;
//	float				m_fPosY;
//	float				m_fPosZ;
//	unsigned char		m_LastTouchLineIndex;
//	bool				m_bIsRigh
//		P2P_CON_COPYCON_ASSIGNOP( XPT_UNIT_USER_SYNC_PACK_FOR_WEDDING_HALL )
//	{
//		m_iUnitUID				= kRight.m_iUnitUID;	
//		m_StateID				= kRight.m_StateID;
//		m_StateChangeNum		= kRight.m_StateChangeNum;
//		m_fPosX					= m_fPosX;
//		m_fPosY					= m_fPosY;
//		m_fPosZ					= m_fPosZ;
//		m_LastTouchLineIndex	= kRight.m_LastTouchLineIndex;
//		m_bIsRight				= kRight.m_bIsRigh		
//		return *this;
//	}
//};
//
////}}AFX
//#else OPTIMIZED_P2P
//
//DECL_P2P_PACKET( XPT_UNIT_USER_SYNC )
//{
//	DWORD				dwFrameMoveCount;
//	UCHAR				nowState;
//	UCHAR				nowAction;
//	char				stateChangeNum;
//
//	float				posX;
//	float				posY;
//	float				posZ;
//	unsigned char		lastTouchLineIndex;
//
//	//{{ kimhc // 2010.12.7 // 던전, 대전 등에서 상대방 또는 파티원의 HP가 줄지 않는 현상 수정
//	float				fNowHP;
//	float				fNowMP;
////}} kimhc // 2010.12.7 // 던전, 대전 등에서 상대방 또는 파티원의 HP가 줄지 않는 현상 수정
//
//	char				m_EncodedData;		// bool	bIsRight(1bit); bool bFrameStop(1bit), directchange(1bit), hypermodecount(last 2bit)	// AAA000AA
//
//	//{{ kimhc // 2010.12.13 // 2010-12-23 New Character CHUNG
//#ifdef	NEW_CHARACTER_CHUNG
//	char				m_EncodedDataFromCannonBallCountAndEtc;	// int iCannonBallCount(4bit),	// AAAA0000
//#endif	NEW_CHARACTER_CHUNG
//	//}} kimhc // 2010.12.13 //  2010-12-23 New Character CHUNG
//
//#ifdef NEW_RANDOM_TABLE_TEST
//	USHORT				m_usRandomTableIndex;
//#else NEW_RANDOM_TABLE_TEST
//	char				m_RandSeed;
//	char				m_RandSeed2;
//#endif NEW_RANDOM_TABLE_TEST
//
//	/// kimhc // 4바이트로 끊어지게 맞추는게 좋으려나...
//	UCHAR				ucHitCount;			/// 타격횟수
//	UCHAR				ucHittedCount;		/// 피격횟수
//	UCHAR				ucNumOfDeBuff;		/// 디버프개수
//	
//	P2P_CON_COPYCON_ASSIGNOP( XPT_UNIT_USER_SYNC )
//	{
//		dwFrameMoveCount	= kRight.dwFrameMoveCount;
//		nowState			= kRight.nowState; 
//		nowAction			= kRight.nowAction;
//		stateChangeNum		= kRight.stateChangeNum;
//
//		posX				= kRight.posX;
//		posY				= kRight.posY;
//		posZ				= kRight.posZ;
//		lastTouchLineIndex	= kRight.lastTouchLineIndex;
//
//		fNowHP				= kRight.fNowHP;
//		fNowMP				= kRight.fNowMP;
//
//		m_EncodedData		= kRight.m_EncodedData;
//
////{{ kimhc // 2010.12.13 // 2010-12-23 New Character CHUNG
//#ifdef	NEW_CHARACTER_CHUNG
//		m_EncodedDataFromCannonBallCountAndEtc	= kRight.m_EncodedDataFromCannonBallCountAndEtc;
//#endif	NEW_CHARACTER_CHUNG
////}} kimhc // 2010.12.13 //  2010-12-23 New Character CHUNG
//
//#ifdef NEW_RANDOM_TABLE_TEST
//		m_usRandomTableIndex = kRight.m_usRandomTableIndex;
//#else NEW_RANDOM_TABLE_TEST
//		m_RandSeed			= kRight.m_RandSeed;
//		m_RandSeed2			= kRight.m_RandSeed2;
//#endif NEW_RANDOM_TABLE_TEST
//		ucHitCount			= kRight.ucHitCount;
//		ucHittedCount		= kRight.ucHittedCount;
//		ucNumOfDeBuff		= kRight.ucNumOfDeBuff;
//		return *this;
//	}
//
////{{ kimhc // 2010.12.13 // 2010-12-23 New Character CHUNG
//#ifdef	NEW_CHARACTER_CHUNG
//	int DecodeCannonBallCount()
//	{
//#ifdef SERV_CHUNG_TACTICAL_TROOPER
//		/// 상위 7bit 사용		이동 포격용 케논볼 추가로 인해 들어가야 할 값이 최대 48로 커져서, 7비트로 변경
//		int iCannonBallCount = static_cast<int>( m_EncodedDataFromCannonBallCountAndEtc >> 1 );
//#else
//		// 상위 5bit 사용
//		int iCannonBallCount = static_cast<int>( m_EncodedDataFromCannonBallCountAndEtc >> 3 );
//#endif SERV_CHUNG_TACTICAL_TROOPER
//		return iCannonBallCount;
//	}
//
//	void EncodeCannonBallCount( char chCannonBallCount_ )
//	{
//#ifdef SERV_CHUNG_TACTICAL_TROOPER
//		/// 상위 7bit 사용		이동 포격용 케논볼 추가로 인해 들어가야 할 값이 최대 48로 커져서, 7비트로 변경
//		char chCannonBallCount = chCannonBallCount_ & 0x7F;
//		// 상위 7비트로 이동
//		chCannonBallCount = chCannonBallCount << 1;
//
//		// 기존의 상위 7비트 삭제
//		m_EncodedDataFromCannonBallCountAndEtc &= 0x01;
//		// 기존의 상위 7비트에 추가
//		m_EncodedDataFromCannonBallCountAndEtc |= chCannonBallCount;
//#else
//		// 하위 5비트만 추출
//		char chCannonBallCount = chCannonBallCount_ & 0x1F;
//		// 상위 5비트로 이동
//		chCannonBallCount = chCannonBallCount << 3;
//		
//		// 기존의 상위 5비트 삭제
//		m_EncodedDataFromCannonBallCountAndEtc &= 0x08;
//		// 기존의 상위 5비트에 추가
//		m_EncodedDataFromCannonBallCountAndEtc |= chCannonBallCount;
//#endif SERV_CHUNG_TACTICAL_TROOPER
//	}
//#endif	NEW_CHARACTER_CHUNG
////}} kimhc // 2010.12.13 //  2010-12-23 New Character CHUNG
//	bool DecodeIsRight() 
//	{  
//		if ( m_EncodedData & 0x80 )
//			return true;
//		else
//			return false;
//	}
//
//
//	void EncodeIsRight( bool bIsRight )
//	{
//		if( true == bIsRight )
//		{
//			m_EncodedData |= 0x80;	// 10000000
//		}
//		else
//		{
//			m_EncodedData &= 0x7F;	// 01111111
//		}
//	}
//
//
//	bool DecodeFrameStop() 
//	{  
//		if ( m_EncodedData & 0x40 )
//			return true;
//		else
//			return false;
//	}
//
//	void EncodeFrameStop( bool bFrameStop )
//	{
//		if( true == bFrameStop )
//		{
//			m_EncodedData |= 0x40;	// 01000000
//		}
//		else
//		{
//			m_EncodedData &= 0xBF;	// 10111111
//		}
//	}
//
//
//	bool DecodeDirectChange() 
//	{  
//		if ( m_EncodedData & 0x20 )
//			return true;
//		else
//			return false;
//	}
//
//	void EncodeDirectChange( bool bDirectChange )
//	{
//		if( true == bDirectChange )
//		{
//			m_EncodedData |= 0x20;	// 00100000
//		}
//		else
//		{
//			m_EncodedData &= 0xDF;	// 11011111
//		}
//	}
//
//	int DecodeHyperModeCount() 
//	{  
//		return (int) (m_EncodedData & 0x03);	
//	}
//
//	void EncodeHyperModeCount( char cHyperModeCount )	
//	{
//		cHyperModeCount &= 0x03;	// 00000011
//		m_EncodedData &= 0xFC;		// 11111100
//		m_EncodedData |= cHyperModeCount;
//	}
//};
//
//DECL_P2P_PACKET( XPT_UNIT_USER_SYNC_PACK )
//{
//    UidType						m_iUnitUID;
//    vector<KXPT_UNIT_USER_SYNC>	m_vecUserSyncList;
//
//	P2P_CON_COPYCON_ASSIGNOP( XPT_UNIT_USER_SYNC_PACK )
//	{
//        m_iUnitUID              = kRight.m_iUnitUID;
//        m_vecUserSyncList       = kRight.m_vecUserSyncList;
//
//		return *this;
//	}
//};
//
//DECL_P2P_PACKET( XPT_UNIT_USER_SYNC_PACK_FOR_WEDDING_HALL )
//{
//	UidType				m_iUnitUID;
//	char				m_StateID;
//	char				m_StateChangeNum;
//	float				m_fPosX;
//	float				m_fPosY;
//	float				m_fPosZ;
//	unsigned char		m_LastTouchLineIndex;
//	bool				m_bIsRight;
//	P2P_CON_COPYCON_ASSIGNOP( XPT_UNIT_USER_SYNC_PACK_FOR_WEDDING_HALL )
//	{
//		m_iUnitUID				= kRight.m_iUnitUID;	
//		m_StateID				= kRight.m_StateID;
//		m_StateChangeNum		= kRight.m_StateChangeNum;
//		m_fPosX					= m_fPosX;
//		m_fPosY					= m_fPosY;
//		m_fPosZ					= m_fPosZ;
//		m_LastTouchLineIndex	= kRight.m_LastTouchLineIndex;
//		m_bIsRight				= kRight.m_bIsRight;
//		return *this;
//	}
//};
//
//#endif // OPTIMIZED_P2P
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

//-------------------------------------------------------------------------------------------------------------------------------
//DECL_P2P_PACKET( XPT_UNIT_USER_SYNC_FOR_DUNGEON )
//{
//	char				nowState;
//	char				nowSubState;
//	//bool				bDirectChange;
//	char				stateChangeNum;
//	
//
//	float				posX;
//	float				posY;
//	float				posZ;
//	unsigned char		lastTouchLineIndex;
//	
//	USHORT				fNowHP;
//	USHORT				fNowMP;
//	USHORT				fHoldMP;
//	char				m_HyperModeCount; // bool bIsRight, bool bFrameStop;
//
//
//	char				m_RandSeed;
//	char				m_RandSeed2;
//
//	P2P_CON_COPYCON_ASSIGNOP( XPT_UNIT_USER_SYNC_FOR_DUNGEON )
//	{
//		nowState			= kRight.nowState; //음수면 다이렉트 체인지
//		nowSubState			= kRight.nowSubState;
//		//bDirectChange		= kRight.bDirectChange;
//		stateChangeNum		= kRight.stateChangeNum;
//
//		posX				= kRight.posX;
//		posY				= kRight.posY;
//		posZ				= kRight.posZ;
//		lastTouchLineIndex	= kRight.lastTouchLineIndex;
//
//
//		fNowHP				= kRight.fNowHP;
//		fNowMP				= kRight.fNowMP;
//		fHoldMP				= kRight.fHoldMP;
//		m_HyperModeCount	= kRight.m_HyperModeCount;
//		//bIsRight			= kRight.bIsRight;
//		//bFrameStop		= kRight.bFrameStop;
//
//
//		m_RandSeed			= kRight.m_RandSeed;
//		m_RandSeed2			= kRight.m_RandSeed2;
//
//		return *this;
//	}
//
//	bool ExtractIsRight( const char& dataToExtract ) 
//	{  
//		if ( dataToExtract & 0x80 )
//			return true;
//		else
//			return false;
//	}
//
//	bool ExtractFrameStop( const char& dataToExtract ) 
//	{  
//		if ( dataToExtract & 0x40 )
//			return true;
//		else
//			return false;
//	}
//
//	int	 ExtractHyperModeCount( const char& dataToExtract ) 
//	{  
//		return dataToExtract & 63;
//	}
//};
//
//DECL_P2P_PACKET( XPT_UNIT_USER_SYNC_PACK_FOR_DUNGEON )
//{
//	UidType									unitUID;
//	vector<KXPT_UNIT_USER_SYNC_FOR_DUNGEON>	unitUserSyncList;
//
//	P2P_CON_COPYCON_ASSIGNOP( XPT_UNIT_USER_SYNC_PACK_FOR_DUNGEON )
//	{
//		unitUserSyncList.clear();
//		for( int i = 0; i < (int)kRight.unitUserSyncList.size(); i++ )
//		{
//			unitUserSyncList.push_back( kRight.unitUserSyncList[i] );
//		}
//
//		return *this;
//	}
//};

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

struct KTWO_PARTS
{
    unsigned char       stateChangeNum:4;
    unsigned char       stateModifyNum:4;
};

struct KDYNAMIC_UNIT_NPC_SYNC
{
	int					m_iUnitUID;
    KTWO_PARTS          m_stateChangeParts;

	//float				m_fNowSpeedX;
	//float				m_fNowSpeedY;
    USHORT              m_usNowSpeedX;
    USHORT              m_usNowSpeedY;
	//UCHAR				m_ucNowHpPercent;
	//UCHAR				m_ucNowMpPercent;
	float				m_fNowHP;
	USHORT				m_usNowMP;

	char				m_cNowState;

	USHORT				m_usPosX;
	USHORT				m_usPosY;
	USHORT				m_usPosZ;
	unsigned char		m_ucLastTouchLineIndex;

	char				m_cMindFlag; // bool bIsRight; bool bStateChange;

	USHORT				m_usRandomTableIndex;
#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION
    USHORT				m_usDefenceRandomTableIndex;
    DWORD               m_dwRelativePos;
    // 자료구조의 맨 마지막이어야 함. optional
    char                m_cReactionStateUnitIndex;
    char                m_cReactionFlag;
    char                m_cTargetUnitIndex;
#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION

    void SetIsRight( bool bSet )
    {
        if ( bSet )
            m_cMindFlag|= 0x80;
        else
            m_cMindFlag &= ~(0x80);
    }
	bool ExtractIsRight() const
	{  
        return  ( m_cMindFlag & 0x80 ) != 0;
	}
#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION
    void SetReactionState( bool bSet, char cReactionStateUnitIndex, bool bRelaxCheck )
    {
        if ( bSet )
        {
            m_cMindFlag |= 0x20;
            m_cReactionStateUnitIndex = cReactionStateUnitIndex;
            m_cReactionFlag = ( bRelaxCheck == true ) ? 1 : 0;
        }
        else
        {
            m_cMindFlag &= ~(0x20);
            m_cReactionStateUnitIndex = -1;
            m_cReactionFlag = 0;
        }
    }
    bool ExtractReactionState() const
    {
        return  ( m_cMindFlag & 0x20 ) != 0;
    }
    bool ExtractRelaxReactionStateCheck() const
    {
        return ( m_cReactionFlag & 1 ) != 0;
    }
    void SetFootOnLine( bool bSet )
    {
        if ( bSet )
            m_cMindFlag |= 0x10;
        else
            m_cMindFlag &= ~(0x10);
    }
    bool ExtractFootOnLine() const
    {
        return ( m_cMindFlag & 0x10 ) != 0;
    }
    void SetTargetUnit( char cTargetUnitIndex )
    {
        if ( cTargetUnitIndex >= 0 )
        {
            m_cMindFlag |= 0x8;
            m_cTargetUnitIndex = cTargetUnitIndex;
        }
        else
        {
            m_cMindFlag &= ~(0x8);
            m_cTargetUnitIndex = -1;
        }
    }
    bool ExtractTargetUnit() const
    {
        return ( m_cMindFlag & 0x8 ) != 0;
    }
#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION
    void SetIsComeBackState( bool bSet )
    {
        if ( bSet )
            m_cMindFlag |= 0x40;
        else
            m_cMindFlag &= ~(0x40);
    }
	bool ExtractIsComeBackState() const
	{
        return  ( m_cMindFlag & 0x40 ) != 0;
	}
    void SetMindFlag( char mindFlag )
    {
        m_cMindFlag = ( m_cMindFlag & 0xf8 ) | ( mindFlag & 0x07 );
    }
	int	 ExtractMindFlag()  const
	{  
        return m_cMindFlag & 0x07;
	}
    void    Serialize( BYTE*& pbyBuffer ) const;
    bool    Deserialize( const BYTE*& pbyBuffer, const BYTE* pbyEnd );
};

#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION

struct  KDYNAMIC_UNIT_NPC_MINISYNC
{
	int		m_iUnitUID;
    DWORD   m_dwRelativePos;
    char    m_cTargetUnitIndex;

    void    Serialize( BYTE*& pbyBuffer ) const;
    bool    Deserialize( const BYTE*& pbyBuffer, const BYTE* pbyEnd );
};//struct  KDYNAMIC_UNIT_NPC_MINISYNC

#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION


//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//
//DECL_P2P_PACKET( XPT_UNIT_NPC_SYNC )
//{
//	int						unitUID;
//
//	char					nextState;
//	float					nowSpeedX;
//	float					nowSpeedY;
//	float					fNowHP;
//	USHORT					fNowMP;
//
//	char					nowState;
//
//	float					posX;
//	float					posY;
//	float					posZ;
//	unsigned char			lastTouchLineIndex;
//
//	char					mindFlag; // bool bIsRight; bool bStateChange;
//
//#ifdef NEW_RANDOM_TABLE_TEST
//
//	USHORT					m_usRandomTableIndex;
//	
//#else NEW_RANDOM_TABLE_TEST
////{{AFX
//	char					m_StateRandomSeed;
//	char					m_StateRandomSeed2;	
////}}AFX
//#endif NEW_RANDOM_TABLE_TEST
//	P2P_CON_COPYCON_ASSIGNOP( XPT_UNIT_NPC_SYNC )
//	{
//		unitUID				= kRight.unitUID;
//		nextState			= kRight.nextState;
//		nowSpeedX			= kRight.nowSpeedX;
//		nowSpeedY			= kRight.nowSpeedY;
//		fNowHP				= kRight.fNowHP;
//		fNowMP				= kRight.fNowMP;
//
//		nowState			= kRight.nowState;
//
//		posX				= kRight.posX;
//		posY				= kRight.posY;
//		posZ				= kRight.posZ;
//		lastTouchLineIndex	= kRight.lastTouchLineIndex;
//
//		//bIsRight			= kRight.bIsRight;
//		//bStateChange		= kRight.bStateChange;
//
//
//
//		mindFlag			= kRight.mindFlag;
//
//
//#ifdef NEW_RANDOM_TABLE_TEST
//		m_usRandomTableIndex	= kRight.m_usRandomTableIndex;		
//#else NEW_RANDOM_TABLE_TEST
////{{AFX
//		m_StateRandomSeed	= kRight.m_StateRandomSeed;
//		m_StateRandomSeed2	= kRight.m_StateRandomSeed2;		
////}}AFX
//#endif NEW_RANDOM_TABLE_TEST
//		return *this;
//	}
//
//	bool ExtractIsRight() const
//	{  
//		if ( mindFlag & 0x80 )
//			return true;
//		else
//			return false;
//	}
//
//	bool ExtractStateChange() const
//	{  
//		if ( mindFlag & 0x40 )
//			return true;
//		else
//			return false;
//	}
//
//	bool ExtractIsComeBackState() const
//	{
//		if ( mindFlag & 0x20 )
//			return true;
//		else
//			return false;		
//	}
//
//	int	 ExtractMindFlag() 
//	{  
//		return mindFlag & 63;
//	}
//};
//
//
//DECL_P2P_PACKET( XPT_UNIT_NPC_SYNC_PACK )
//{
//	vector<KXPT_UNIT_NPC_SYNC>	unitNPCSyncList;
//
//	P2P_CON_COPYCON_ASSIGNOP( XPT_UNIT_NPC_SYNC_PACK )
//	{
//		unitNPCSyncList.clear();
//		for( int i = 0; i < (int)kRight.unitNPCSyncList.size(); i++ )
//		{
//			unitNPCSyncList.push_back( kRight.unitNPCSyncList[i] );
//		}
//
//		return *this;
//	}
//};
//
////DECL_P2P_PACKET( XPT_SQUARE_UNIT_SYNC )
////{
////	UidType					m_UnitUID;
////	char					m_StateID;
////	char					m_StateChangeNum;
////	float					m_PosX;
////	float					m_PosY;
////	float					m_PosZ;
////	unsigned char			m_LastTouchLineIndex;
////	bool					m_bIsRight;
////
////	P2P_CON_COPYCON_ASSIGNOP( XPT_SQUARE_UNIT_SYNC )
////	{
////		m_UnitUID				= kRight.m_UnitUID;
////		m_StateID				= kRight.m_StateID;
////		m_StateChangeNum		= kRight.m_StateChangeNum;
////		m_PosX					= kRight.m_PosX;
////		m_PosY					= kRight.m_PosY;
////		m_PosZ					= kRight.m_PosZ;
////		m_LastTouchLineIndex	= kRight.m_LastTouchLineIndex;
////		m_bIsRight				= kRight.m_bIsRight;
////
////		return *this;
////	}
////};
//
////DECL_P2P_PACKET( XPT_SQUARE_UNIT_SYNC_RIGHT )
////{
////	UidType					m_UnitUID;
////	float					m_PosX;
////	float					m_PosY;
////	float					m_PosZ;
////	unsigned char			m_LastTouchLineIndex;
////	bool					m_bIsRight;
////	bool					m_bPureRight;
////	bool					m_bPureLeft;
////
////	P2P_CON_COPYCON_ASSIGNOP( XPT_SQUARE_UNIT_SYNC_RIGHT )
////	{
////		m_UnitUID				= kRight.m_UnitUID;
////		m_PosX					= kRight.m_PosX;
////		m_PosY					= kRight.m_PosY;
////		m_PosZ					= kRight.m_PosZ;
////		m_LastTouchLineIndex	= kRight.m_LastTouchLineIndex;
////		m_bIsRight				= kRight.m_bIsRight;
////		m_bPureRight			= kRight.m_bPureRight;
////		m_bPureLeft				= kRight.m_bPureLeft;
////
////		return *this;
////	}
////};
//
////DECL_P2P_PACKET( XPT_SQUARE_UNIT_SYNC_REQ )
////{
////	UidType					m_UnitUID;
////
////	P2P_CON_COPYCON_ASSIGNOP( XPT_SQUARE_UNIT_SYNC_REQ )
////	{
////		m_UnitUID				= kRight.m_UnitUID;
////
////		return *this;
////	}
////};

//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK






#ifdef SERV_PET_SYSTEM

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
    struct  KXPT_UNIT_PET_SYNC
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	DECL_P2P_PACKET( XPT_UNIT_PET_SYNC )
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
	{
		UidType					unitUID;

		char					nextState;
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
        USHORT                  usNowSpeedX;
        USHORT                  usNowSpeedY;
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//		float					nowSpeedX;
//		float					nowSpeedY;
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
		USHORT					fNowMP;

		char					nowState;

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
        USHORT                  ucPosX;
        USHORT                  ucPosY;
        USHORT                  ucPosZ;
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//		float					posX;
//		float					posY;
//		float					posZ;
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
		unsigned char			lastTouchLineIndex;

		bool					bIsRight;
		USHORT					m_usRandomTableIndex;

		P2P_CON_COPYCON_ASSIGNOP( XPT_UNIT_PET_SYNC )
		{
			unitUID				= kRight.unitUID;
			nextState			= kRight.nextState;
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
            usNowSpeedX         = kRight.usNowSpeedX;
            usNowSpeedY         = kRight.usNowSpeedY;
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//			nowSpeedX			= kRight.nowSpeedX;
//			nowSpeedY			= kRight.nowSpeedY;			
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
			fNowMP				= kRight.fNowMP;

			nowState			= kRight.nowState;

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
			ucPosX				= kRight.ucPosX;
			ucPosY				= kRight.ucPosY;
			ucPosZ				= kRight.ucPosZ;
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//			posX				= kRight.posX;
//			posY				= kRight.posY;
//			posZ				= kRight.posZ;
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
			lastTouchLineIndex	= kRight.lastTouchLineIndex;

			bIsRight			= kRight.bIsRight;

			m_usRandomTableIndex	= kRight.m_usRandomTableIndex;		

			return *this;
		}

		//bool ExtractIsRight( const char& dataToExtract )
		//{  
		//	if ( dataToExtract & 0x80 )
		//		return true;
		//	else
		//		return false;
		//}

		//bool ExtractStateChange( const char& dataToExtract ) 
		//{  
		//	if ( dataToExtract & 0x40 )
		//		return true;
		//	else
		//		return false;
		//}

		//int	 ExtractMindFlag( const char& dataToExtract ) 
		//{  
		//	return dataToExtract & 63;
		//}
	};
	
//#ifndef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	DECL_P2P_PACKET( XPT_UNIT_PET_SYNC_PACK )
//	{
//		vector<KXPT_UNIT_PET_SYNC>	unitPetSyncList;
//
//		P2P_CON_COPYCON_ASSIGNOP( XPT_UNIT_PET_SYNC_PACK )
//		{
//			unitPetSyncList.clear();
//			for( int i = 0; i < (int)kRight.unitPetSyncList.size(); i++ )
//			{
//				unitPetSyncList.push_back( kRight.unitPetSyncList[i] );
//			}
//
//			return *this;
//		}
//	};
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

//#ifdef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
#pragma pack( pop )
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK


#endif