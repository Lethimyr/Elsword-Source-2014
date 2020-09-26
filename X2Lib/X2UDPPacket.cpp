//#ifdef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

template<typename T>
static inline void _Serialize( BYTE*& pbyBuffer, const T& tData_ )
{
    *( (T*) pbyBuffer ) = tData_;
    pbyBuffer += sizeof(T);
}

template<typename T>
static inline bool _Deserialize( const BYTE*& pbyBuffer, const BYTE* pbyEnd, T& tData_ )
{
    if ( pbyEnd - pbyBuffer >= sizeof(T) )
    {
        tData_ =  *( (const T*) pbyBuffer );
        pbyBuffer += sizeof(T);
        return true;
    }
    return false;
}

//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK


//#ifndef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//
//
//
////------------------------------------------------------------------------------------------------------
////SERIALIZE_DEFINE_PUT( KXPT_CONNECT_SERVER_REQ, obj, ks )
////{
////	return  PUT( m_bDummy )
////		;
////}
////
////SERIALIZE_DEFINE_GET( KXPT_CONNECT_SERVER_REQ, obj, ks )
////{
////	return  GET( m_bDummy )
////		;
////}
//
//
////------------------------------------------------------------------------------------------------------
////SERIALIZE_DEFINE_PUT( KXPT_CONNECT_SERVER_ACK, obj, ks )
////{
////	return  PUT( m_IP )
////		&&	PUT( m_Port )
////		&&  PUT( m_UDPPort )
////		;
////}
////
////SERIALIZE_DEFINE_GET( KXPT_CONNECT_SERVER_ACK, obj, ks )
////{
////	return  GET( m_IP )
////		&&	GET( m_Port )
////		&&  GET( m_UDPPort )
////		;
////}
//
//
////------------------------------------------------------------------------------------------------------
////SERIALIZE_DEFINE_PUT( KXPT_CHECK_SERVER_ALIVE_REQ, obj, ks )
////{
////	return  PUT( UID )
////		;
////}
////
////SERIALIZE_DEFINE_GET( KXPT_CHECK_SERVER_ALIVE_REQ, obj, ks )
////{
////	return  GET( UID )
////		;
////}
//
//
////------------------------------------------------------------------------------------------------------
////SERIALIZE_DEFINE_PUT( KXPT_CHECK_SERVER_ALIVE_ACK, obj, ks )
////{
////	return  PUT( UID )
////		;
////}
////
////SERIALIZE_DEFINE_GET( KXPT_CHECK_SERVER_ALIVE_ACK, obj, ks )
////{
////	return  GET( UID )
////		;
////}
//
//
////------------------------------------------------------------------------------------------------------
//SERIALIZE_DEFINE_PUT( KXPT_PORT_CHECK_REQ, obj, ks )
//{
//	return  PUT( m_UserUID )
//		//{{ 2013. 1. 11	박세훈	Merge 공인IP 연결 실패시 내부IP로 시도( 박진웅 )
////#ifdef SERV_KTDX_RETRY_USING_INTERNAL_IP
//		&&  PUT( m_wstrInternalIP )
//		&&  PUT( m_usInternalPort )
////#endif SERV_KTDX_RETRY_USING_INTERNAL_IP
//		//}}
//		;
//}
//
//SERIALIZE_DEFINE_GET( KXPT_PORT_CHECK_REQ, obj, ks )
//{
//	return  GET( m_UserUID )
//		//{{ 2013. 1. 11	박세훈	Merge 공인IP 연결 실패시 내부IP로 시도( 박진웅 )
////#ifdef SERV_KTDX_RETRY_USING_INTERNAL_IP
//		&&  GET( m_wstrInternalIP )
//		&&  GET( m_usInternalPort )
////#endif SERV_KTDX_RETRY_USING_INTERNAL_IP
//		//}}
//		;
//}
//
////------------------------------------------------------------------------------------------------------
//SERIALIZE_DEFINE_PUT( KXPT_PORT_CHECK_ACK, obj, ks )
//{
//	return  PUT( m_IP )
//		&&	PUT( m_Port )
//		;
//}
//
//SERIALIZE_DEFINE_GET( KXPT_PORT_CHECK_ACK, obj, ks )
//{
//	return  GET( m_IP )
//		&&	GET( m_Port )
//		;
//}
//
////------------------------------------------------------------------------------------------------------
//SERIALIZE_DEFINE_PUT( KXPT_PING_TEST_REQ, obj, ks )
//{
//	return  PUT( m_UnitUID )
//		&&	PUT( m_SendTime )
//#ifdef ACTIVE_KOG_GAME_PERFORMANCE_CHECK
//		&& PUT( m_bRelay )
//		&& PUT( m_uiSendCounter )
//#endif//ACTIVE_KOG_GAME_PERFORMANCE_CHECK
//		;
//}
//
//SERIALIZE_DEFINE_GET( KXPT_PING_TEST_REQ, obj, ks )
//{
//	return  GET( m_UnitUID )
//		&&	GET( m_SendTime )
//#ifdef ACTIVE_KOG_GAME_PERFORMANCE_CHECK
//		&& GET( m_bRelay )
//		&& GET( m_uiSendCounter )
//#endif//ACTIVE_KOG_GAME_PERFORMANCE_CHECK
//		;
//}
//
////------------------------------------------------------------------------------------------------------
//SERIALIZE_DEFINE_PUT( KXPT_PING_TEST_ACK, obj, ks )
//{
//	return  PUT( m_UnitUID )
//		&&	PUT( m_SendTime )
//#ifdef ACTIVE_KOG_GAME_PERFORMANCE_CHECK
//		&& PUT( m_bRelay )
//		&& PUT( m_uiSendCounter )
//#ifdef  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
//        && PUT( m_bIgnoreAck )
//#endif  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
//#endif//ACTIVE_KOG_GAME_PERFORMANCE_CHECK
//		;
//}
//
//SERIALIZE_DEFINE_GET( KXPT_PING_TEST_ACK, obj, ks )
//{
//	return  GET( m_UnitUID )
//		&&	GET( m_SendTime )
//#ifdef ACTIVE_KOG_GAME_PERFORMANCE_CHECK
//		&& GET( m_bRelay )
//		&& GET( m_uiSendCounter )
//#ifdef  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
//        && GET( m_bIgnoreAck )
//#endif  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
//#endif//ACTIVE_KOG_GAME_PERFORMANCE_CHECK
//		;
//}
//
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK



//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK



void    KDYNAMIC_UNIT_USER_SYNC::Serialize( BYTE*& pbyBuffer, const KDYNAMIC_UNIT_USER_SYNC* pPrevSync )
{
    m_usDataFieldFlag = eUnitUserSync_DataField_No;
	//이전 정보가 없거나 이전 정보와 현재 정보가 다를 경우...
	if( pPrevSync == NULL || pPrevSync->m_ucNowState != m_ucNowState )
		m_usDataFieldFlag |= eUnitUserSync_DataField_nowState;
	if( pPrevSync == NULL || pPrevSync->m_ucNowAction != m_ucNowAction )
		m_usDataFieldFlag |= eUnitUserSync_DataField_nowAction;
	if( pPrevSync == NULL || pPrevSync->m_cStateChangeNum != m_cStateChangeNum )
		m_usDataFieldFlag |= eUnitUserSync_DataField_stateChangeNum;
	if( pPrevSync == NULL || pPrevSync->m_usPosX != m_usPosX )
		m_usDataFieldFlag |= eUnitUserSync_DataField_PosX;
//#ifdef  X2OPTIMIZE_USER_USER_PUSH_PASS_SYNC
	if( pPrevSync == NULL || pPrevSync->m_fPosY != m_fPosY )
		m_usDataFieldFlag |= eUnitUserSync_DataField_PosY;
//#else   X2OPTIMIZE_USER_USER_PUSH_PASS_SYNC
//	if( pPrevSync == NULL || pPrevSync->m_usPosY != m_usPosY )
//		m_usDataFieldFlag |= eUnitUserSync_DataField_PosY;
//#endif  X2OPTIMIZE_USER_USER_PUSH_PASS_SYNC
	if( pPrevSync == NULL || pPrevSync->m_usPosZ != m_usPosZ )
		m_usDataFieldFlag |= eUnitUserSync_DataField_PosZ;
	if( pPrevSync == NULL || pPrevSync->m_ucLastTouchLineIndex != m_ucLastTouchLineIndex )
		m_usDataFieldFlag |= eUnitUserSync_DataField_lastTouchLineIndex;
	if( pPrevSync == NULL || pPrevSync->m_fNowHp != m_fNowHp )
		m_usDataFieldFlag |= eUnitUserSync_DataField_NowHpPercent;
	if( pPrevSync == NULL || pPrevSync->m_fNowMp != m_fNowMp )
		m_usDataFieldFlag |= eUnitUserSync_DataField_NowMpPercent;
	if( pPrevSync == NULL || pPrevSync->m_cEncodedData != m_cEncodedData )
		m_usDataFieldFlag |= eUnitUserSync_DataField_EncodedData;
#ifdef SERV_9TH_NEW_CHARACTER // 김태환
	if( pPrevSync == NULL || pPrevSync->m_sEncodedDataFromCannonBallCountAndEtc != m_sEncodedDataFromCannonBallCountAndEtc )
		m_usDataFieldFlag |= eUnitUserSync_DataField_EncodedDataFromCannonBallCountAndEtc;
#else // SERV_9TH_NEW_CHARACTER
	if( pPrevSync == NULL || pPrevSync->m_cEncodedDataFromCannonBallCountAndEtc != m_cEncodedDataFromCannonBallCountAndEtc )
		m_usDataFieldFlag |= eUnitUserSync_DataField_EncodedDataFromCannonBallCountAndEtc;
#endif // SERV_9TH_NEW_CHARACTER
	if( pPrevSync == NULL || pPrevSync->m_usRandomTableIndex != m_usRandomTableIndex )
		m_usDataFieldFlag |= eUnitUserSync_DataField_RandomTableIndex;
	if( pPrevSync == NULL || pPrevSync->m_ucHitCount != m_ucHitCount )
		m_usDataFieldFlag |= eUnitUserSync_DataField_HitCount;
	if( pPrevSync == NULL || pPrevSync->m_ucHittedCount != m_ucHittedCount )
		m_usDataFieldFlag |= eUnitUserSync_DataField_HittedCount;
	if( pPrevSync == NULL || pPrevSync->m_ucNumOfDeBuff != m_ucNumOfDeBuff )
		m_usDataFieldFlag |= eUnitUserSync_DataField_NumOfDeBuff;
//#ifdef  X2OPTIMIZE_USER_USER_PUSH_PASS_SYNC
	if( pPrevSync == NULL || pPrevSync->m_dwRelativePos != m_dwRelativePos )
		m_usDataFieldFlag |= eUnitUserSync_DataField_RelativePos;
//#endif  X2OPTIMIZE_USER_USER_PUSH_PASS_SYNC
    
    _Serialize( pbyBuffer, m_usDataFieldFlag );
    if ( m_usDataFieldFlag & eUnitUserSync_DataField_nowState )
        _Serialize( pbyBuffer, m_ucNowState );
	if ( m_usDataFieldFlag & eUnitUserSync_DataField_nowAction )
        _Serialize( pbyBuffer, m_ucNowAction );
    if ( m_usDataFieldFlag & eUnitUserSync_DataField_stateChangeNum )
	    _Serialize( pbyBuffer, m_cStateChangeNum );
    if ( m_usDataFieldFlag & eUnitUserSync_DataField_PosX )
	    _Serialize( pbyBuffer, m_usPosX );
//#ifdef  X2OPTIMIZE_USER_USER_PUSH_PASS_SYNC
    if ( m_usDataFieldFlag & eUnitUserSync_DataField_PosY )
	    _Serialize( pbyBuffer, m_fPosY  );
//#else   X2OPTIMIZE_USER_USER_PUSH_PASS_SYNC
//    if ( m_usDataFieldFlag & eUnitUserSync_DataField_PosY )
//	    _Serialize( pbyBuffer, m_usPosY );
//#endif  X2OPTIMIZE_USER_USER_PUSH_PASS_SYNC
    if ( m_usDataFieldFlag & eUnitUserSync_DataField_PosZ )
	    _Serialize( pbyBuffer, m_usPosZ );
    if ( m_usDataFieldFlag & eUnitUserSync_DataField_lastTouchLineIndex )
	    _Serialize( pbyBuffer, m_ucLastTouchLineIndex );
    if ( m_usDataFieldFlag & eUnitUserSync_DataField_NowHpPercent )
	    _Serialize( pbyBuffer, m_fNowHp );
    if ( m_usDataFieldFlag & eUnitUserSync_DataField_NowMpPercent )
	    _Serialize( pbyBuffer, m_fNowMp );
    if ( m_usDataFieldFlag & eUnitUserSync_DataField_EncodedData )
	    _Serialize( pbyBuffer, m_cEncodedData );
#ifdef SERV_9TH_NEW_CHARACTER // 김태환
	if ( m_usDataFieldFlag & eUnitUserSync_DataField_EncodedDataFromCannonBallCountAndEtc )
		_Serialize( pbyBuffer, m_sEncodedDataFromCannonBallCountAndEtc );
#else // SERV_9TH_NEW_CHARACTER
	if ( m_usDataFieldFlag & eUnitUserSync_DataField_EncodedDataFromCannonBallCountAndEtc )
		_Serialize( pbyBuffer, m_cEncodedDataFromCannonBallCountAndEtc );
#endif // SERV_9TH_NEW_CHARACTER
    if ( m_usDataFieldFlag & eUnitUserSync_DataField_RandomTableIndex )
	    _Serialize( pbyBuffer, m_usRandomTableIndex );
    if ( m_usDataFieldFlag & eUnitUserSync_DataField_HitCount )
	    _Serialize( pbyBuffer, m_ucHitCount );
    if ( m_usDataFieldFlag & eUnitUserSync_DataField_HittedCount )
	    _Serialize( pbyBuffer, m_ucHittedCount );
    if ( m_usDataFieldFlag & eUnitUserSync_DataField_NumOfDeBuff )
	    _Serialize( pbyBuffer, m_ucNumOfDeBuff );
//#ifdef  X2OPTIMIZE_USER_USER_PUSH_PASS_SYNC
	if( m_usDataFieldFlag & eUnitUserSync_DataField_RelativePos )
        _Serialize( pbyBuffer, m_dwRelativePos );
//#endif  X2OPTIMIZE_USER_USER_PUSH_PASS_SYNC
}

bool    KDYNAMIC_UNIT_USER_SYNC::Deserialize( const BYTE*& pbyBuffer, const BYTE* pbyEnd )
{
    USHORT  usDataFieldFlag = eUnitUserSync_DataField_No;
    if ( false == _Deserialize( pbyBuffer, pbyEnd, usDataFieldFlag ) )
        return false;
    m_usDataFieldFlag |= usDataFieldFlag;

	if( usDataFieldFlag & eUnitUserSync_DataField_nowState )
    {
		if ( false == _Deserialize( pbyBuffer, pbyEnd, m_ucNowState ) )
            return false;
    }
	if( usDataFieldFlag & eUnitUserSync_DataField_nowAction )
    {
		if ( false == _Deserialize( pbyBuffer, pbyEnd, m_ucNowAction ) )
            return false;
    }
	if( usDataFieldFlag & eUnitUserSync_DataField_stateChangeNum )
    {
		if ( false == _Deserialize( pbyBuffer, pbyEnd, m_cStateChangeNum ) )
            return false;
    }
	if( usDataFieldFlag & eUnitUserSync_DataField_PosX )
    {
		if ( false == _Deserialize( pbyBuffer, pbyEnd, m_usPosX ) )
            return false;
    }
//#ifdef  X2OPTIMIZE_USER_USER_PUSH_PASS_SYNC
	if( usDataFieldFlag & eUnitUserSync_DataField_PosY )
    {
		if ( false == _Deserialize( pbyBuffer, pbyEnd, m_fPosY ) )
            return false;
    }
//#else   X2OPTIMIZE_USER_USER_PUSH_PASS_SYNC
//	if( usDataFieldFlag & eUnitUserSync_DataField_PosY )
//    {
//		if ( false == _Deserialize( pbyBuffer, pbyEnd, m_usPosY ) )
//            return false;
//    }
//#endif  X2OPTIMIZE_USER_USER_PUSH_PASS_SYNC
	if( usDataFieldFlag & eUnitUserSync_DataField_PosZ )
    {
		if ( false == _Deserialize( pbyBuffer, pbyEnd, m_usPosZ ) )
            return false;
    }
	if( usDataFieldFlag & eUnitUserSync_DataField_lastTouchLineIndex )
    {
		if ( false == _Deserialize( pbyBuffer, pbyEnd, m_ucLastTouchLineIndex ) )
            return false;
    }
	if( usDataFieldFlag & eUnitUserSync_DataField_NowHpPercent )
    {
		if ( false == _Deserialize( pbyBuffer, pbyEnd, m_fNowHp ) )
            return false;
    }
	if( usDataFieldFlag & eUnitUserSync_DataField_NowMpPercent )
    {
		if ( false == _Deserialize( pbyBuffer, pbyEnd, m_fNowMp ) )
            return false;
    }
	if( usDataFieldFlag & eUnitUserSync_DataField_EncodedData )
    {
		if ( false == _Deserialize( pbyBuffer, pbyEnd, m_cEncodedData ) )
            return false;
    }
	if( usDataFieldFlag & eUnitUserSync_DataField_EncodedDataFromCannonBallCountAndEtc )
    {
#ifdef SERV_9TH_NEW_CHARACTER // 김태환
		if ( false == _Deserialize( pbyBuffer, pbyEnd, m_sEncodedDataFromCannonBallCountAndEtc ) )
			return false;
#else // SERV_9TH_NEW_CHARACTER
		if ( false == _Deserialize( pbyBuffer, pbyEnd, m_cEncodedDataFromCannonBallCountAndEtc ) )
			return false;
#endif // SERV_9TH_NEW_CHARACTER
    }
	if( usDataFieldFlag & eUnitUserSync_DataField_RandomTableIndex )
    {
		if ( false == _Deserialize( pbyBuffer, pbyEnd, m_usRandomTableIndex ) )
            return false;
    }
	if( usDataFieldFlag & eUnitUserSync_DataField_HitCount )
    {
		if ( false == _Deserialize( pbyBuffer, pbyEnd, m_ucHitCount ) )
            return false;
    }
	if( usDataFieldFlag & eUnitUserSync_DataField_HittedCount )
    {
		if ( false == _Deserialize( pbyBuffer, pbyEnd, m_ucHittedCount ) )
            return false;
    }
	if( usDataFieldFlag & eUnitUserSync_DataField_NumOfDeBuff )
    {
		if ( false == _Deserialize( pbyBuffer, pbyEnd, m_ucNumOfDeBuff ) )
            return false;
    }
//#ifdef  X2OPTIMIZE_USER_USER_PUSH_PASS_SYNC
    if ( usDataFieldFlag & eUnitUserSync_DataField_RelativePos )
    {
		if ( false == _Deserialize( pbyBuffer, pbyEnd, m_dwRelativePos ) )
            return false;
    }
//#endif  X2OPTIMIZE_USER_USER_PUSH_PASS_SYNC

    return true;
}

//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//
//#ifdef OPTIMIZED_P2P
//
////------------------------------------------------------------------------------------------------------
//SERIALIZE_DEFINE_PUT( KXPT_UNIT_USER_SYNC, obj, ks )
//{
//	return  PUT( m_ucNowState )
//		&&  PUT( nowAction )
//		&&	PUT( m_usPosX )
//		&&	PUT( m_usPosY )
//		&&	PUT( m_ucLastTouchLineIndex )
//		&&	PUT( m_cEncodedData )
//#ifdef NEW_RANDOM_TABLE_TEST
//		&&	PUT( m_usRandomTableIndex )
//#else NEW_RANDOM_TABLE_TEST
////{{AFX
//		&&	PUT( m_RandSeed )
//		&&	PUT( m_RandSeed2 )
//
////}}AFX
//#endif NEW_RANDOM_TABLE_TEST
//		;
//}
//
//SERIALIZE_DEFINE_GET( KXPT_UNIT_USER_SYNC, obj, ks )
//{
//	return  GET( m_ucNowState )
//		&&  GET( nowAction )
//		&&	GET( m_usPosX )
//		&&	GET( m_usPosY )
//		&&	GET( m_ucLastTouchLineIndex )
//		&&	GET( m_cEncodedData )
//#ifdef NEW_RANDOM_TABLE_TEST
//		&&	GET( m_usRandomTableIndex )
//#else NEW_RANDOM_TABLE_TEST
////{{AFX
//		&&	GET( m_RandSeed )
//		&&	GET( m_RandSeed2 )
//
////}}AFX
//#endif NEW_RANDOM_TABLE_TEST
//		;
//}
//
////------------------------------------------------------------------------------------------------------
//SERIALIZE_DEFINE_PUT( KXPT_UNIT_USER_SYNC_PACK, obj, ks )
//{
//	return  PUT( m_iUnitUID )
//		&&	PUT( m_vecUserSyncList )
//        &&  PUT( m_usNowHP )
//        &&  PUT( m_usNowMP )
//		;
//}
//
//SERIALIZE_DEFINE_GET( KXPT_UNIT_USER_SYNC_PACK, obj, ks )
//{
//    return  GET( m_iUnitUID )
//        &&	GET( m_vecUserSyncList )
//        &&  GET( m_usNowHP )
//        &&  GET( m_usNowMP )
//        ;
//}
//SERIALIZE_DEFINE_PUT( KXPT_UNIT_USER_SYNC_PACK_FOR_WEDDING_HALL, obj, ks )
//{
//	return  PUT( m_iUnitUID )
//		&&	PUT( m_StateID )
//		&&  PUT( m_StateChangeNum )
//		&&  PUT( m_fPosX )
//		&&  PUT( m_fPosY )
//		&&  PUT( m_fPosZ )
//		&&	PUT( m_LastTouchLineIndex )
//		&&  PUT( m_bIsRight )
//		;
//}
//
//SERIALIZE_DEFINE_GET( KXPT_UNIT_USER_SYNC_PACK_FOR_WEDDING_HALL, obj, ks )
//{
//	return  GET( m_iUnitUID )
//		&&	GET( m_StateID )
//		&&  GET( m_StateChangeNum )
//		&&  GET( m_fPosX )
//		&&  GET( m_fPosY )
//		&&  GET( m_fPosZ )
//		&&	GET( m_LastTouchLineIndex )
//		&&  GET( m_bIsRight )
//		;
//}
//#else
////{{AFX
//
////------------------------------------------------------------------------------------------------------
//SERIALIZE_DEFINE_PUT( KXPT_UNIT_USER_SYNC, obj, ks )
//{
//	return  PUT( dwFrameMoveCount )
//		&&	PUT( nowState )
//		&&  PUT( nowAction )
//		&&	PUT( stateChangeNum )
//		&&	PUT( posX )
//		&&	PUT( posY )
//		&&	PUT( posZ )
//		&&	PUT( lastTouchLineIndex )
//		&&	PUT( fNowHP )
//		&&	PUT( fNowMP )
//		&&	PUT( m_EncodedData )
//
//		//{{ kimhc // 2010.12.13 // 2010-12-23 New Character CHUNG
//#ifdef	NEW_CHARACTER_CHUNG
//		&&	PUT( m_EncodedDataFromCannonBallCountAndEtc	)
//#endif	NEW_CHARACTER_CHUNG
//		//}} kimhc // 2010.12.13 //  2010-12-23 New Character CHUNG
//
//#ifdef NEW_RANDOM_TABLE_TEST
//		&&	PUT( m_usRandomTableIndex )
//#else NEW_RANDOM_TABLE_TEST
//		&&	PUT( m_RandSeed )
//		&&	PUT( m_RandSeed2 )
//#endif NEW_RANDOM_TABLE_TEST
//		&&	PUT( ucHitCount )
//		&&	PUT( ucHittedCount )
//		&&	PUT( ucNumOfDeBuff )
//		;
//}
//
//SERIALIZE_DEFINE_GET( KXPT_UNIT_USER_SYNC, obj, ks )
//{
//	return  GET( dwFrameMoveCount )
//		&&	GET( nowState )
//		&&  GET( nowAction )
//		&&	GET( stateChangeNum )
//		&&	GET( posX )
//		&&	GET( posY )
//		&&	GET( posZ )
//		&&	GET( lastTouchLineIndex )
//		&&	GET( fNowHP )
//		&&	GET( fNowMP )
//		&&	GET( m_EncodedData )
//
//		//{{ kimhc // 2010.12.13 // 2010-12-23 New Character CHUNG
//#ifdef	NEW_CHARACTER_CHUNG
//		&&	GET( m_EncodedDataFromCannonBallCountAndEtc	)
//#endif	NEW_CHARACTER_CHUNG
//		//}} kimhc // 2010.12.13 //  2010-12-23 New Character CHUNG
//
//#ifdef NEW_RANDOM_TABLE_TEST
//		&&	GET( m_usRandomTableIndex )
//#else NEW_RANDOM_TABLE_TEST
//		&&	GET( m_RandSeed )
//		&&	GET( m_RandSeed2 )
//#endif NEW_RANDOM_TABLE_TEST
//		&&	GET( ucHitCount )
//		&&	GET( ucHittedCount )
//		&&	GET( ucNumOfDeBuff )
//		;
//}
//
////------------------------------------------------------------------------------------------------------
////SERIALIZE_DEFINE_PUT( KXPT_UNIT_USER_SYNC_FOR_DUNGEON, obj, ks )
////{
////	return  PUT( nowState )
////		&& PUT( nowSubState )
////		//&&	PUT( bDirectChange )
////		&&	PUT( stateChangeNum )
////		&&	PUT( posX )
////		&&	PUT( posY )
////		&&	PUT( posZ )
////		&&	PUT( lastTouchLineIndex )
////		//&&	PUT( bIsRight )
////		//&&	PUT( bFrameStop )
////		&&	PUT( fNowHP )
////		&&	PUT( fNowMP )
////		&&	PUT( fHoldMP )
////		&&	PUT( m_HyperModeCount )
////		&&	PUT( m_RandSeed )
////		&&	PUT( m_RandSeed2 )
////		;
////}
////
////SERIALIZE_DEFINE_GET( KXPT_UNIT_USER_SYNC_FOR_DUNGEON, obj, ks )
////{
////	return  GET( nowState )
////		&& GET( nowSubState )
////		//&&	GET( bDirectChange )
////		&&	GET( stateChangeNum )
////		&&	GET( posX )
////		&&	GET( posY )
////		&&	GET( posZ )
////		&&	GET( lastTouchLineIndex )
////		//&&	GET( bIsRight )
////		//&&	GET( bFrameStop )
////		&&	GET( fNowHP )
////		&&	GET( fNowMP )
////		&&	GET( fHoldMP )
////		&&	GET( m_HyperModeCount )
////		&&	GET( m_RandSeed )
////		&&	GET( m_RandSeed2 )
////		;
////}
//
//
////------------------------------------------------------------------------------------------------------
//SERIALIZE_DEFINE_PUT( KXPT_UNIT_USER_SYNC_PACK, obj, ks )
//{
//    return  PUT( m_iUnitUID )
//        &&	PUT( m_vecUserSyncList )
//		;
//}
//
//SERIALIZE_DEFINE_GET( KXPT_UNIT_USER_SYNC_PACK, obj, ks )
//{
//    return  GET( m_iUnitUID )
//        &&	GET( m_vecUserSyncList )
//		;
//}
//
////------------------------------------------------------------------------------------------------------
////SERIALIZE_DEFINE_PUT( KXPT_UNIT_USER_SYNC_PACK_FOR_DUNGEON, obj, ks )
////{
////	return  PUT( unitUID )
////		&&	PUT( unitUserSyncList )
////		;
////}
////
////SERIALIZE_DEFINE_GET( KXPT_UNIT_USER_SYNC_PACK_FOR_DUNGEON, obj, ks )
////{
////	return  GET( unitUID )
////		&&	GET( unitUserSyncList )
////		;
////}
//
////}}AFX
//
//SERIALIZE_DEFINE_PUT( KXPT_UNIT_USER_SYNC_PACK_FOR_WEDDING_HALL, obj, ks )
//{
//	return  PUT( m_iUnitUID )
//		&&	PUT( m_StateID )
//		&&  PUT( m_StateChangeNum )
//		&&  PUT( m_fPosX )
//		&&  PUT( m_fPosY )
//		&&  PUT( m_fPosZ )
//		&&	PUT( m_LastTouchLineIndex )
//		&&  PUT( m_bIsRight )
//		;
//}
//
//SERIALIZE_DEFINE_GET( KXPT_UNIT_USER_SYNC_PACK_FOR_WEDDING_HALL, obj, ks )
//{
//	return  GET( m_iUnitUID )
//		&&	GET( m_StateID )
//		&&  GET( m_StateChangeNum )
//		&&  GET( m_fPosX )
//		&&  GET( m_fPosY )
//		&&  GET( m_fPosZ )
//		&&	GET( m_LastTouchLineIndex )
//		&&  GET( m_bIsRight )
//		;
//}
//#endif // OPTIMIZED_P2P
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

//#ifdef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK


void    KDYNAMIC_UNIT_NPC_SYNC::Serialize( BYTE*& pbyBuffer ) const
{
#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION
    CopyMemory( pbyBuffer, this, sizeof(KDYNAMIC_UNIT_NPC_SYNC) - sizeof(char) * 3 );
    pbyBuffer += sizeof(KDYNAMIC_UNIT_NPC_SYNC) - sizeof(char) * 3;
    if ( ExtractReactionState() == true )
    {
        *pbyBuffer = (BYTE) ( ( m_cReactionStateUnitIndex & 0x7f ) | ( ( ExtractRelaxReactionStateCheck() == true ) ? 0x80 : 0 ) );
        pbyBuffer++;
    }
    if ( ExtractTargetUnit() == true )
    {
        *pbyBuffer = (BYTE) m_cTargetUnitIndex;
        pbyBuffer++;
    }
#else   X2OPTIMIZE_NPC_NONHOST_SIMULATION
    CopyMemory( pbyBuffer, this, sizeof(KDYNAMIC_UNIT_NPC_SYNC) );
    pbyBuffer += sizeof(KDYNAMIC_UNIT_NPC_SYNC);
#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION
//#endif  X2OPTIMIZE_UDP_PACKET_NPC_INCREMENTAL
}

bool    KDYNAMIC_UNIT_NPC_SYNC::Deserialize( const BYTE*& pbyBuffer, const BYTE* pbyEnd )
{
#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION

    if ( pbyBuffer + sizeof(KDYNAMIC_UNIT_NPC_SYNC) - sizeof(char) * 3 > pbyEnd )
        return false;
    CopyMemory( this, pbyBuffer, sizeof(KDYNAMIC_UNIT_NPC_SYNC) - sizeof(char) * 3 );
    pbyBuffer += sizeof(KDYNAMIC_UNIT_NPC_SYNC) - sizeof(char) * 3;
    m_cReactionStateUnitIndex = -1;
    m_cReactionFlag = 0;
    m_cTargetUnitIndex = -1;
    if ( ExtractReactionState() == true )
    {
        if ( pbyBuffer + 1 > pbyEnd )
            return false;
        m_cReactionStateUnitIndex = *( (const char*) pbyBuffer );
        m_cReactionFlag = ( m_cReactionStateUnitIndex & 0x80 ) ? 1 : 0;
        m_cReactionStateUnitIndex &= 0x7f;
        pbyBuffer++;
    }
    if ( ExtractTargetUnit() == true )
    {
        if ( pbyBuffer + 1 > pbyEnd )
            return false;
        m_cTargetUnitIndex = *( (const char*) pbyBuffer );
        pbyBuffer++;
    }
    return true;
#else   X2OPTIMIZE_NPC_NONHOST_SIMULATION

    if ( pbyBuffer + sizeof(KDYNAMIC_UNIT_NPC_SYNC) > pbyEnd )
        return false;
    CopyMemory( this, pbyBuffer, sizeof(KDYNAMIC_UNIT_NPC_SYNC) );
    pbyBuffer += sizeof(KDYNAMIC_UNIT_NPC_SYNC);
    return true;

#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION
}


#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION

void    KDYNAMIC_UNIT_NPC_MINISYNC::Serialize( BYTE*& pbyBuffer ) const
{
    CopyMemory( pbyBuffer, this, sizeof(KDYNAMIC_UNIT_NPC_MINISYNC) );
    pbyBuffer += sizeof(KDYNAMIC_UNIT_NPC_MINISYNC);
}

bool    KDYNAMIC_UNIT_NPC_MINISYNC::Deserialize( const BYTE*& pbyBuffer, const BYTE* pbyEnd )
{
    if ( pbyBuffer + sizeof(KDYNAMIC_UNIT_NPC_MINISYNC) > pbyEnd )
        return false;
    CopyMemory( this, pbyBuffer, sizeof(KDYNAMIC_UNIT_NPC_MINISYNC) );
    pbyBuffer += sizeof(KDYNAMIC_UNIT_NPC_MINISYNC);
    return true;
}

#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION


//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//
////------------------------------------------------------------------------------------------------------
//SERIALIZE_DEFINE_PUT( KXPT_UNIT_NPC_SYNC, obj, ks )
//{
//	return  PUT( unitUID )
//		&&	PUT( nextState )
//		&&	PUT( nowSpeedX )
//		&&	PUT( nowSpeedY )
//		&&	PUT( fNowHP )
//		&&	PUT( fNowMP )
//		&&	PUT( nowState )
//		&&	PUT( posX )
//		&&	PUT( posY )
//		&&	PUT( posZ )
//		&&	PUT( lastTouchLineIndex )
//		//&&	PUT( bIsRight )
//		//&&	PUT( bStateChange )
//		&&	PUT( mindFlag )
//
//#ifdef NEW_RANDOM_TABLE_TEST
//		&&	PUT( m_usRandomTableIndex )
//#else NEW_RANDOM_TABLE_TEST
////{{AFX
//		&&	PUT( m_StateRandomSeed )
//		&&	PUT( m_StateRandomSeed2 )
////}}AFX
//#endif NEW_RANDOM_TABLE_TEST
//;
//}
//
//SERIALIZE_DEFINE_GET( KXPT_UNIT_NPC_SYNC, obj, ks )
//{
//	return  GET( unitUID )
//		&&	GET( nextState )
//		&&	GET( nowSpeedX )
//		&&	GET( nowSpeedY )
//		&&	GET( fNowHP )
//		&&	GET( fNowMP )
//		&&	GET( nowState )
//		&&	GET( posX )
//		&&	GET( posY )
//		&&	GET( posZ )
//		&&	GET( lastTouchLineIndex )
//		//&&	GET( bIsRight )
//		//&&	GET( bStateChange )
//		&&	GET( mindFlag )
//#ifdef NEW_RANDOM_TABLE_TEST
//		&&	GET( m_usRandomTableIndex )
//#else NEW_RANDOM_TABLE_TEST
////{{AFX
//		&&	GET( m_StateRandomSeed )
//		&&	GET( m_StateRandomSeed2 )
////}}AFX
//#endif NEW_RANDOM_TABLE_TEST
//		;
//}
//
//
//
////------------------------------------------------------------------------------------------------------
//SERIALIZE_DEFINE_PUT( KXPT_UNIT_NPC_SYNC_PACK, obj, ks )
//{
//	return  PUT( unitNPCSyncList )
//		;
//}
//
//SERIALIZE_DEFINE_GET( KXPT_UNIT_NPC_SYNC_PACK, obj, ks )
//{
//	return  GET( unitNPCSyncList )
//		;
//}
//
//
////------------------------------------------------------------------------------------------------------
////SERIALIZE_DEFINE_PUT( KXPT_SQUARE_UNIT_SYNC, obj, ks )
////{
////	return  PUT( m_UnitUID )
////		&&	PUT( m_StateID )
////		&&	PUT( m_StateChangeNum )
////		&&	PUT( m_PosX )
////		&&	PUT( m_PosY )
////		&&	PUT( m_PosZ )
////		&&	PUT( m_LastTouchLineIndex )
////		&&	PUT( m_bIsRight )
////		;
////}
////
////SERIALIZE_DEFINE_GET( KXPT_SQUARE_UNIT_SYNC, obj, ks )
////{
////	return  GET( m_UnitUID )
////		&&	GET( m_StateID )
////		&&	GET( m_StateChangeNum )
////		&&	GET( m_PosX )
////		&&	GET( m_PosY )
////		&&	GET( m_PosZ )
////		&&	GET( m_LastTouchLineIndex )
////		&&	GET( m_bIsRight )
////		;
////}
//
//
////------------------------------------------------------------------------------------------------------
////SERIALIZE_DEFINE_PUT( KXPT_SQUARE_UNIT_SYNC_RIGHT, obj, ks )
////{
////	return  PUT( m_UnitUID )
////		&&	PUT( m_PosX )
////		&&	PUT( m_PosY )
////		&&	PUT( m_PosZ )
////		&&	PUT( m_LastTouchLineIndex )
////		&&	PUT( m_bIsRight )
////		&&	PUT( m_bPureRight )
////		&&	PUT( m_bPureLeft )
////		;
////}
////
////SERIALIZE_DEFINE_GET( KXPT_SQUARE_UNIT_SYNC_RIGHT, obj, ks )
////{
////	return  GET( m_UnitUID )
////		&&	GET( m_PosX )
////		&&	GET( m_PosY )
////		&&	GET( m_PosZ )
////		&&	GET( m_LastTouchLineIndex )
////		&&	GET( m_bIsRight )
////		&&	GET( m_bPureRight )
////		&&	GET( m_bPureLeft )
////		;
////}
//
//
////------------------------------------------------------------------------------------------------------
////SERIALIZE_DEFINE_PUT( KXPT_SQUARE_UNIT_SYNC_REQ, obj, ks )
////{
////	return  PUT( m_UnitUID )
////		;
////}
////
////SERIALIZE_DEFINE_GET( KXPT_SQUARE_UNIT_SYNC_REQ, obj, ks )
////{
////	return  GET( m_UnitUID )
////		;
////}
//

//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK










#ifdef SERV_PET_SYSTEM


//#ifndef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//
//	SERIALIZE_DEFINE_PUT( KXPT_UNIT_PET_SYNC, obj, ks )
//	{
//		return  PUT( unitUID )
//			&&	PUT( nextState )
//			&&	PUT( nowSpeedX )
//			&&	PUT( nowSpeedY )			
//			&&	PUT( fNowMP )
//			&&	PUT( nowState )
//			&&	PUT( posX )
//			&&	PUT( posY )
//			&&	PUT( posZ )
//			&&	PUT( lastTouchLineIndex )
//			&&	PUT( bIsRight )
//			&&	PUT( m_usRandomTableIndex )
//			;
//	}
//
//	SERIALIZE_DEFINE_GET( KXPT_UNIT_PET_SYNC, obj, ks )
//	{
//		return  GET( unitUID )
//			&&	GET( nextState )
//			&&	GET( nowSpeedX )
//			&&	GET( nowSpeedY )
//			&&	GET( fNowMP )
//			&&	GET( nowState )
//			&&	GET( posX )
//			&&	GET( posY )
//			&&	GET( posZ )
//			&&	GET( lastTouchLineIndex )
//			&&	GET( bIsRight )
//			&&	GET( m_usRandomTableIndex )
//			;
//	}
//
//	SERIALIZE_DEFINE_PUT( KXPT_UNIT_PET_SYNC_PACK, obj, ks )
//	{
//		return  PUT( unitPetSyncList )
//			;
//	}
//
//	SERIALIZE_DEFINE_GET( KXPT_UNIT_PET_SYNC_PACK, obj, ks )
//	{
//		return  GET( unitPetSyncList )
//			;
//	}
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

#endif
