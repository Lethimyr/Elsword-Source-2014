#pragma once

typedef __int64 UidType;

/*    64 bit ( 8 byte )

| ff ff ff ff | ff ff ff ff |

| 00 00 00 ff | ff ff ff ff | 순수 uid 영역. ( 5byte = 40bit. 2^40 = 1 0995 1162 7776 )
| 80 00 00 00 | 00 00 00 00 | 부호비트로 남겨둠
| 40 00 00 00 | 00 00 00 00 | 로컬에서 임시로 발행되는 uid.
| 3f 00 00 00 | 00 00 00 00 | 서버군 uid.
| 00 ff 00 00 | 00 00 00 00 | 서버(채널) uid.
| 00 00 ff 00 | 00 00 00 00 | reserved.

*/

/*		메신저SN

| F0 00 00 00 |	// 서버군 UID 영역
| 0F FF FF FF |	// 메신져 UID 영역

서버군 UID 식별
0F FF FF FF 솔레스
1F FF FF FF 가이아

*/

namespace KncUid {

    unsigned int    Die32();
    UidType         GetTempUID();

    UidType         ExtractPureUID( UidType iUID_ );
    UidType         ExtractServerGroupID( UidType iUID_ );
    UidType         ExtractServerID( UidType iUID_ );
    UidType         ExtractReservedID( UidType iUID_ );

	//{{ 2010. 01. 05  최육사	임시발급UID
	void			SetTempUID( UidType& iDestUID_ );
	//}}
    void            SetPureUID( UidType& iDestUID_, UidType iSrcUID_ );
    void            SetServerGroupID( UidType& iDestUID_, UidType iSrcUID_ );
    void            SetServerID( UidType& iDestUID_, UidType iSrcUID_ );
    void            SetReservedID( UidType& iDestUID_, UidType iSrcUID_ );

	//serverid & reservedid 통합.(GS 에서 필드서버 사용시 마을 아이디로 사용하기위해)
	UidType			ExtractCodeID( UidType iUID_ );
	void			SetCodeID( UidType& iDestUID_, UidType iSrcUID_ );
}