#pragma once

#include "SecureTypedef.h"
#include "ByteStream.h"

namespace KncSecurity
{
    class KSecurityAssociation;
}

SERIALIZE_DEFINE_TAG( KncSecurity::KSecurityAssociation, eTAG_USERCLASS );
SERIALIZE_DECLARE_PUTGET( KncSecurity::KSecurityAssociation );

NAMESPACE_BEGIN( KncSecurity )

typedef unsigned int SeqType;

#define MAX_SEQUENCE_NUM	0xFFFFFFFF

class KSecurityAssociation
{
public:
    enum {
        AUTH_KEY_SIZE = 8,
        CRYPTO_KEY_SIZE = 8,
    };

    KSecurityAssociation();
    KSecurityAssociation( const KSecurityAssociation& );
    KSecurityAssociation& operator=( const KSecurityAssociation& );
    bool operator==(const KSecurityAssociation& right) const;
    ~KSecurityAssociation();
    void Swap( KSecurityAssociation& sa );

    void ResetRandomizeKey();
    void SetAuthKey( const KByteStream& bsKey );
    void SetCryptoKey( const KByteStream& bsKey );

    const KByteStream& GetAuthKey() const;
    const KByteStream& GetCipherKey() const;
    SeqType GetSequenceNum() const;

	//////////////////////////////////////////////////////////////////////////
	//{{ 2009. 10. 15  최육사	Sequence Number ( not have replay window mask )
	void SetLastSequenceNum_notRWM( SeqType nSequenceNum ) { m_nLastSequenceNum = nSequenceNum; }
	void IncrSequenceNum_notRWM();
	bool IsValidSequenceNum_notRWM( SeqType nSequenceNum );
	//}}
	//////////////////////////////////////////////////////////////////////////

    void IncrSequenceNum();	
    bool IsValidSequenceNum( SeqType nSequenceNum ) const;
    void UpdateReplayWindow( SeqType nSequenceNum );

    // all size unit of data is byte. ( not bit )
    size_t GetIVSize() const        { return 8; }
    size_t GetICVSize() const       { return 10; }
    size_t GetBlockSize() const     { return 8; }
    // 1블록은 무조건 패딩되어야 한다. 데이터 길이가 블록단위로 딱 맞으면 온전한 블록 하나를,
    // 딱 맞지 않으면 부족한 만큼을 채우기위한 불완전 블록 하나를 채운다.
    // 아래에 지정해 주는 값은 그 이후 데이터 사이즈를 숨기기 위한 여분의 패딩블록 수를 지정한다.
    // 값이 0일 경우 추가패딩하지 않는다.
    size_t GetMaxExtraPadBlocks() const  { return 1; }

    KByteStream Encryption( const KByteStream& payload, const KByteStream& IV ) const;
    KByteStream GenerateICV( const KByteStream& auth ) const;
    KByteStream Decryption( const KByteStream& crypt, const KByteStream& IV ) const;

protected:

    typedef unsigned int ReplayWinType; // Any unsigned integer type

    KByteStream         m_bsAuthKey;
    KByteStream         m_bsCryptoKey;
    SeqType             m_nSequenceNum;
    SeqType             m_nLastSequenceNum;
    ReplayWinType       m_nReplayWindowMask;

    SERIALIZE_DECLARE_FRIEND( KSecurityAssociation );
};

NAMESPACE_END