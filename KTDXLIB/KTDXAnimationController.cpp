#include <StdAfx.h>

#include "KTDXAnimationController.h"

#pragma warning (disable: 4200)


#define KAC_DEFAULT_MAX_NUM_TRACKS  2
#define KAC_DEFAULT_MAX_NUM_EVENTS  30
#define KAC_TOLERANCE               0.00001f

#define	KAC_LINEAR_COMPRESSION_TOLERANCE	0.001f
#define KAC_LINEAR_COMPRESSION_RETRY    10


static const D3DXVECTOR3    s_v3UnitScale(1,1,1);
static const D3DXVECTOR3    s_v3ZeroTrans(0,0,0);
static const D3DXQUATERNION s_qUnitRot(0,0,0,1);

static HRESULT _WriteToFileSerializer( KFileSerializer& kInOutFile_, const CKTDXDeviceXSkinMesh* pInSkinMesh_, ID3DXAnimationSet* pInAnimSet_ );
static HRESULT _ReadFromMemoryDeserializer( const KMemoryDeserializer& kInMemory_, DWORD dwInOffset_, CKTDXDeviceXSkinMesh::XSkinMeshProxy& proxy_, LPD3DXANIMATIONCONTROLLER pInAC_ );
static HRESULT _Optimize( ID3DXAnimationSet* pAnimSet, ID3DXAnimationSet** ppOptimizedAnimSet );


enum    EACAnimationSetType
{
    EAC_KEYFRAMED = 0,
    EAC_COMPRESSED = 1,
};//enum    EACAnimationSetType

#pragma pack( push, 1 )

struct  KACHeader
{
    DWORD   m_dwMagic;
    DWORD   m_dwVersion;
    DWORD   m_dwNumAnimationSets;
    DWORD   m_adwOffset_AnimationSets[];
};//struct  KACHeader


struct  KACAnimationSet
{
    DWORD   m_dwOffset_Name;
    DWORD   m_dwType;
    double  m_dTicksPerSecond;
    DWORD   m_dwPlaybackType;
};//struct  KACAnimationSet


struct  KACKey
{
    float   m_fTime;
    BYTE    m_byType;
};//struct  KACKey


struct  KACKeyframedAnimation
{
    int     m_iIndex_Frame;
    DWORD   m_dwNumKeys;
    BYTE    m_abKeys[];
};//struct  KACKeyframedAnimation


struct  KACKeyframedAnimationSet : public KACAnimationSet
{
    DWORD   m_dwNumAnimations;
    DWORD   m_adwSize_Animations[];
};//struct  KACKeyframedAnimationSet


struct  KACCompressedAnimationSet : public KACAnimationSet
{
    DWORD   m_dwCompressedDataSize;
    BYTE    m_abCompressedData[];
};//struct  KACCompressedAnimationSet

#pragma pack( pop )


enum    EKeyframeType
{
    KEYTYPE_XSCALE_CHANGE = 1<<0,
    KEYTYPE_YSCALE_CHANGE = 1<<1,
    KEYTYPE_ZSCALE_CHANGE = 1<<2,
    KEYTYPE_XTRANSLATION_CHANGE = 1<<3,
    KEYTYPE_YTRANSLATION_CHANGE = 1<<4,
    KEYTYPE_ZTRANSLATION_CHANGE = 1<<5,
    KEYTYPE_ROTATION_CHANGE = 1<<6,
    KEYTYPE_ALLKEYS = KEYTYPE_XSCALE_CHANGE|KEYTYPE_YSCALE_CHANGE|KEYTYPE_ZSCALE_CHANGE
        |KEYTYPE_XTRANSLATION_CHANGE|KEYTYPE_YTRANSLATION_CHANGE|KEYTYPE_ZTRANSLATION_CHANGE
        |KEYTYPE_ROTATION_CHANGE,
    KEYTYPE_NOCHANGE_NOKEY = 1<<7,
};//enum    EKeyframeType


static HRESULT  _UnpackKeys( DWORD dwInNumKeys_, const void* pInBuffer_, DWORD dwInSize_
        , DWORD& dwOutScales_, D3DXKEY_VECTOR3 aOutScales_[]
        , DWORD& dwOutTrans_, D3DXKEY_VECTOR3 aOutTranslations_[]
        , DWORD& dwOutRots_, D3DXKEY_QUATERNION aOutRotations_[] )
{
    ASSERT( dwInNumKeys_ > 0 );
    ASSERT( pInBuffer_ != NULL && dwInSize_ > 0 );
    ASSERT( aOutScales_ != NULL && aOutTranslations_ != NULL && aOutRotations_ != NULL );

    dwOutScales_ = 0;
    dwOutTrans_ = 0;
    dwOutRots_ = 0;

    const KACKey* pkacKeys = static_cast<const KACKey*>( pInBuffer_ );
    const float* pfData = (const float*)  ( (const BYTE*)( pInBuffer_ ) + sizeof(KACKey) * dwInNumKeys_ );

    D3DXVECTOR3     vScale = s_v3UnitScale;
    D3DXVECTOR3     vTranslation = s_v3ZeroTrans;
    D3DXQUATERNION  qRotation = s_qUnitRot;

    DWORD   dwTmp = dwInSize_ - sizeof(KACKey) * dwInNumKeys_;
    DWORD   dwDataSize = dwTmp / sizeof(float);
    if ( dwDataSize * sizeof(float) != dwTmp )
    {
        ASSERT( "invalid packed keys format" );
        return E_FAIL;
    }//if

    DWORD   dwIndex = 0;
    BYTE    byType;
    for( DWORD dwKey = 0; dwKey < dwInNumKeys_; dwKey++ )
    {
        byType = pkacKeys[ dwKey ].m_byType;
        if ( byType & KEYTYPE_XSCALE_CHANGE )
            vScale.x = pfData[ dwIndex++ ];
        if ( byType & KEYTYPE_YSCALE_CHANGE )
            vScale.y = pfData[ dwIndex++ ];
        if ( byType & KEYTYPE_ZSCALE_CHANGE )
            vScale.z = pfData[ dwIndex++ ];
        if ( byType & KEYTYPE_XTRANSLATION_CHANGE )
            vTranslation.x = pfData[ dwIndex++ ];
        if ( byType & KEYTYPE_YTRANSLATION_CHANGE )
            vTranslation.y = pfData[ dwIndex++ ];
        if ( byType & KEYTYPE_ZTRANSLATION_CHANGE )
            vTranslation.z = pfData[ dwIndex++ ];
        if ( byType & KEYTYPE_ROTATION_CHANGE )
        {
            qRotation = *((const D3DXQUATERNION*) &pfData[ dwIndex ]);
            dwIndex += 4;
        }//if
        if ( !( byType & KEYTYPE_NOCHANGE_NOKEY ) || ( byType & (KEYTYPE_XSCALE_CHANGE|KEYTYPE_YSCALE_CHANGE|KEYTYPE_ZSCALE_CHANGE) ) )
        {
            aOutScales_[ dwOutScales_ ].Time = pkacKeys[ dwKey ].m_fTime;
            aOutScales_[ dwOutScales_++ ].Value = vScale;
        }
        if ( !( byType & KEYTYPE_NOCHANGE_NOKEY ) || ( byType & (KEYTYPE_XTRANSLATION_CHANGE|KEYTYPE_YTRANSLATION_CHANGE|KEYTYPE_ZTRANSLATION_CHANGE) ) )
        {
            aOutTranslations_[ dwOutTrans_ ].Time = pkacKeys[ dwKey ].m_fTime;
            aOutTranslations_[ dwOutTrans_++ ].Value = vTranslation;
        }
        if ( !( byType & KEYTYPE_NOCHANGE_NOKEY ) || ( byType & KEYTYPE_ROTATION_CHANGE ) )
        {
            aOutRotations_[ dwOutRots_ ].Time = pkacKeys[ dwKey ].m_fTime;
            aOutRotations_[ dwOutRots_++ ].Value = qRotation;
        }//if.. else..
        ASSERT( dwIndex <= dwDataSize );
    }//for

    if ( dwIndex != dwDataSize )
    {
        ASSERT( "invalid packed keys format" );
        return E_FAIL;
    }//if

    return S_OK;
}//UnpackKeys()


static DWORD   _EstimateNumKeys( KACKey* pkacInKeys_
        , DWORD dwInNumScales_, const D3DXKEY_VECTOR3 aInScales_[]
        , DWORD dwInNumTrans_, const D3DXKEY_VECTOR3 aInTranslations_[]
        , DWORD dwInNumRots_, const D3DXKEY_QUATERNION aInRotations_[] )
{
    ASSERT( pkacInKeys_ != NULL );
    ASSERT( dwInNumScales_ == 0 || aInScales_ != NULL );
    ASSERT( dwInNumTrans_ == 0 || aInTranslations_ != NULL );
    ASSERT( dwInNumRots_ == 0 || aInRotations_ != NULL );

    DWORD   dwNumKeys = 0;
    DWORD   dwScale = 0;
    DWORD   dwTrans = 0;
    DWORD   dwRot = 0;
    float   fKey;
    bool    bKey;

    while( dwScale < dwInNumScales_ || dwTrans < dwInNumTrans_ || dwRot < dwInNumRots_ )
    {
        bKey = false;
        if ( dwScale < dwInNumScales_ )
        {
            fKey = aInScales_[dwScale].Time;
            bKey = true;
        }//if
        if ( dwTrans < dwInNumTrans_ )
        {
            if ( !bKey )
            {
                fKey = aInTranslations_[dwTrans].Time;
                bKey = true;
            }
            else
                fKey = min( fKey, aInTranslations_[dwTrans].Time );
        }//if
        if ( dwRot < dwInNumRots_ )
        {
            if ( !bKey )
            {
                fKey = aInRotations_[dwRot].Time;
                bKey = true;
            }
            else
                fKey = min( fKey, aInRotations_[dwRot].Time );
        }//if
        ASSERT( bKey );
        pkacInKeys_[dwNumKeys].m_fTime = fKey;
        pkacInKeys_[dwNumKeys].m_byType = 0;
        if ( dwScale < dwInNumScales_ && pkacInKeys_[dwNumKeys].m_fTime == aInScales_[dwScale].Time )
        {
            pkacInKeys_[dwNumKeys].m_byType |= KEYTYPE_XSCALE_CHANGE|KEYTYPE_YSCALE_CHANGE|KEYTYPE_ZSCALE_CHANGE;
            dwScale++;
        }//if
        if ( dwTrans < dwInNumTrans_ && pkacInKeys_[dwNumKeys].m_fTime == aInTranslations_[dwTrans].Time )
        {
            pkacInKeys_[dwNumKeys].m_byType |= KEYTYPE_XTRANSLATION_CHANGE|KEYTYPE_YTRANSLATION_CHANGE|KEYTYPE_ZTRANSLATION_CHANGE;
            dwTrans++;
        }//if
        if ( dwRot < dwInNumRots_ && pkacInKeys_[dwNumKeys].m_fTime == aInRotations_[dwRot].Time )
        {
            pkacInKeys_[dwNumKeys].m_byType |= KEYTYPE_ROTATION_CHANGE;
            dwRot++;
        }//if
        ASSERT( pkacInKeys_[dwNumKeys].m_byType != 0 );
        if ( pkacInKeys_[dwNumKeys].m_byType != KEYTYPE_ALLKEYS )
            pkacInKeys_[dwNumKeys].m_byType |= KEYTYPE_NOCHANGE_NOKEY;
        dwNumKeys++;
    }//while

    return dwNumKeys;
}//_EstimateNumKeys()



static DWORD   _PackKeys( DWORD& dwOutNumKeys_, void* pOutBuffer_
        , DWORD dwInNumScales_, const D3DXKEY_VECTOR3 aInScales_[]
        , DWORD dwInNumTrans_, const D3DXKEY_VECTOR3 aInTranslations_[]
        , DWORD dwInNumRots_, const D3DXKEY_QUATERNION aInRotations_[] )
{
    ASSERT( pOutBuffer_ != NULL );
    ASSERT( dwInNumScales_ == 0 || aInScales_ != NULL );
    ASSERT( dwInNumTrans_ == 0 || aInTranslations_ != NULL );
    ASSERT( dwInNumRots_ == 0 || aInRotations_ != NULL );

    KACKey* pkacKeys = static_cast<KACKey*>( pOutBuffer_ );
    dwOutNumKeys_ = _EstimateNumKeys( pkacKeys, dwInNumScales_, aInScales_
        , dwInNumTrans_, aInTranslations_
        , dwInNumRots_, aInRotations_ );

    float*  pfData = (float*)( (BYTE*)( pOutBuffer_ ) + sizeof(KACKey) * dwOutNumKeys_ );

    D3DXVECTOR3     vScale = s_v3UnitScale;
    D3DXVECTOR3     vTranslation = s_v3ZeroTrans;
    D3DXQUATERNION  qRotation = s_qUnitRot;
    DWORD           dwScale = 0;
    DWORD           dwTrans = 0;
    DWORD           dwRot = 0;
    DWORD           dwDataSize = 0;
    BYTE            byType, byCheck;
    bool            bChanged;

    for( DWORD dwKey = 0; dwKey < dwOutNumKeys_; dwKey++ )
    {
        byCheck = pkacKeys[ dwKey ].m_byType;
        byType = 0;
        if ( byCheck & KEYTYPE_XSCALE_CHANGE )
        {
            ASSERT( dwScale < dwInNumScales_ );
            ASSERT( aInScales_[dwScale].Time == pkacKeys[ dwKey ].m_fTime );
            const D3DXVECTOR3& vScale2 = aInScales_[dwScale++].Value;
            bChanged = false;
            if ( vScale.x != vScale2.x )
            {
                byType |= KEYTYPE_XSCALE_CHANGE;
                pfData[ dwDataSize++ ] = vScale.x = vScale2.x;
                bChanged = true;
            }//if
            if ( vScale.y != vScale2.y )
            {
                byType |= KEYTYPE_YSCALE_CHANGE;
                pfData[ dwDataSize++ ] = vScale.y = vScale2.y;
                bChanged = true;
            }//if
            if ( vScale.z != vScale2.z )
            {
                byType |= KEYTYPE_ZSCALE_CHANGE;
                pfData[ dwDataSize++ ] = vScale.z = vScale2.z;
                bChanged = true;
            }//if
            if ( !bChanged && ( byCheck & KEYTYPE_NOCHANGE_NOKEY ) )
            {
                byType |= KEYTYPE_XSCALE_CHANGE;
                pfData[ dwDataSize++ ] = vScale2.x;
            }//if
        }//if
        if ( byCheck & KEYTYPE_XTRANSLATION_CHANGE )
        {
            ASSERT( dwTrans < dwInNumTrans_ );
            ASSERT(  aInTranslations_[dwTrans].Time == pkacKeys[ dwKey ].m_fTime );
            const D3DXVECTOR3& vTranslation2 = aInTranslations_[dwTrans++].Value;
            bChanged = false;
            if ( vTranslation.x != vTranslation2.x )
            {
                byType |= KEYTYPE_XTRANSLATION_CHANGE;
                pfData[ dwDataSize++ ] = vTranslation.x = vTranslation2.x;
                bChanged = true;
            }//if
            if ( vTranslation.y != vTranslation2.y )
            {
                byType |= KEYTYPE_YTRANSLATION_CHANGE;
                pfData[ dwDataSize++ ] = vTranslation.y = vTranslation2.y;
                bChanged = true;
            }//if
            if ( vTranslation.z != vTranslation2.z )
            {
                byType |= KEYTYPE_ZTRANSLATION_CHANGE;
                pfData[ dwDataSize++ ] = vTranslation.z = vTranslation2.z;
                bChanged = true;
            }//if
            if ( !bChanged && ( byCheck & KEYTYPE_NOCHANGE_NOKEY ) )
            {
                byType |= KEYTYPE_XTRANSLATION_CHANGE;
                pfData[ dwDataSize++ ] = vTranslation2.x;
            }//if
        }//if
        if ( byCheck & KEYTYPE_ROTATION_CHANGE )
        {
            ASSERT( dwRot < dwInNumRots_ );
            ASSERT(  aInRotations_[dwRot].Time == pkacKeys[ dwKey ].m_fTime );
            const D3DXQUATERNION& qRotation2 = aInRotations_[dwRot++].Value;
            if ( qRotation.w != qRotation2.w || qRotation.x != qRotation2.x || qRotation.y != qRotation2.y || qRotation.z != qRotation2.z
                || ( byCheck & KEYTYPE_NOCHANGE_NOKEY ) )
            {
                byType |= KEYTYPE_ROTATION_CHANGE;
                *( (D3DXQUATERNION*)( pfData + dwDataSize ) ) = qRotation = qRotation2;
                dwDataSize += 4;
            }//if
        }//if
        //pkacKeys[ dwKey ].m_fTime = aInScales_[dwKey].Time;
        pkacKeys[ dwKey ].m_byType = byType|(byCheck & KEYTYPE_NOCHANGE_NOKEY);
    }//for
    ASSERT( dwScale == dwInNumScales_ );
    ASSERT( dwTrans == dwInNumTrans_ );
    ASSERT( dwRot == dwInNumRots_ );

    return  sizeof(KACKey)*dwOutNumKeys_ + dwDataSize * sizeof(float);
}//_PackKeys()


__forceinline   bool _IsWithinTolerance( float a, float b )
{
    float   fDiff = a - b;
    if ( fDiff >= - KAC_TOLERANCE && fDiff <= KAC_TOLERANCE )
        return true;
    if ( b > 1.f )
        fDiff /= b;
    else if ( b < -1.f )
        fDiff /= -b;
    else 
        return false;
    return ( fDiff >= - KAC_TOLERANCE && fDiff <= KAC_TOLERANCE );
}//_IsWithinTolerance()

__forceinline bool _IsWithinTolerance( const D3DXVECTOR3& a, const D3DXVECTOR3& b )
{
    return _IsWithinTolerance( a.x, b.x ) && _IsWithinTolerance( a.y, b.y ) && _IsWithinTolerance( a.z, b.z );
}//

__forceinline bool _IsWithinTolerance( const D3DXQUATERNION& a, const D3DXQUATERNION& b )
{
    return _IsWithinTolerance( a.w, b.w ) && _IsWithinTolerance( a.x, b.x ) && _IsWithinTolerance( a.y, b.y ) && _IsWithinTolerance( a.z, b.z ); 
}//


#ifdef	X2OPTIMIZE_ANIMATION_LOSSY_COMPRESSION

__forceinline static bool _IsWithinTolerance( float a, float b, float fTol )
{
    float   fDiff = ( a >= b ) ? a - b : b - a;
    if ( fDiff <= fTol )
        return true;
    if ( a < 0.f )  a = -a;
    if ( b < 0.f )  b = -b;
    return fDiff <= fTol * __max( a, b );
}//_IsWithinTolerance()

__forceinline static bool _IsWithinTolerance( const D3DXVECTOR3& a, const D3DXVECTOR3& b, float fTol )
{
    return _IsWithinTolerance( a.x, b.x, fTol ) && _IsWithinTolerance( a.y, b.y, fTol ) && _IsWithinTolerance( a.z, b.z, fTol );
}//

__forceinline static bool _IsWithinTolerance( const D3DXQUATERNION& a, const D3DXQUATERNION& b, float fTol )
{
    return _IsWithinTolerance( a.w, b.w, fTol ) && _IsWithinTolerance( a.x, b.x, fTol ) && _IsWithinTolerance( a.y, b.y, fTol ) && _IsWithinTolerance( a.z, b.z, fTol ); 
}//

__forceinline static bool _IsWithinTolerance( float a, float b, float fTol, float& fFailRatio )
{
    float   fDiff = ( a >= b ) ? a - b : b - a;
    if ( fDiff <= fTol )
    {
        fFailRatio = 1.f;
        return true;
    }
    if ( a < 0.f )  a = -a;
    if ( b < 0.f )  b = -b;
    if ( b > a )
        a = b;
    a = __max( a, 1.f ) * fTol;
    if ( fDiff <= a )
    {
        fFailRatio = 1.f;
        return  true;
    }
    fFailRatio = fDiff / a;
    return false;
}//_IsWithinTolerance()


__forceinline static bool _IsWithinTolerance( const D3DXVECTOR3& a, const D3DXVECTOR3& b, float fTol, float& fFailRatio )
{
    float   fRatio1 = 1.f, fRatio2 = 1.f, fRatio3 = 1.f;
    bool    bRet = _IsWithinTolerance( a.x, b.x, fTol, fRatio1 ) && _IsWithinTolerance( a.y, b.y, fTol, fRatio2 ) && _IsWithinTolerance( a.z, b.z, fTol, fRatio3 );
    if ( bRet == true )
    {
        fFailRatio = 1.f;
        return true;
    }//if
    fRatio2 = __max( fRatio2, fRatio3 );
    fFailRatio = __max( fRatio1, fRatio2 );
    return  false;
}//


__forceinline static bool _IsWithinTolerance( const D3DXQUATERNION& a, const D3DXQUATERNION& b, float fTol, float& fFailRatio )
{
    float   fRatio1 = 1.f, fRatio2 = 1.f, fRatio3 = 1.f, fRatio4 = 1.f;
    bool bRet = _IsWithinTolerance( a.w, b.w, fTol, fRatio1 ) && _IsWithinTolerance( a.x, b.x, fTol, fRatio2 ) 
        && _IsWithinTolerance( a.y, b.y, fTol, fRatio3 ) && _IsWithinTolerance( a.z, b.z, fTol, fRatio4 ); 
    if ( bRet == true )
    {
        fFailRatio = 1.f;
        return true;
    }
    fRatio1 = __max( fRatio1, fRatio2 );
    fRatio3 = __max( fRatio3, fRatio4 );
    fFailRatio = __max( fRatio1, fRatio3 );
    return false;
}//


struct D3DXKEY_VECTOR3_COMPARE
{
    bool operator() ( const D3DXKEY_VECTOR3& a, const D3DXKEY_VECTOR3& b ) const
    {
        return a.Time < b.Time;
    }
};

struct D3DXKEY_VECTOR3_EQUAL
{
    bool operator () ( const D3DXKEY_VECTOR3& a, const D3DXKEY_VECTOR3& b ) const
    {
        return a.Time == b.Time;
    }
};

struct D3DXKEY_QUATERNION_COMPARE
{
    bool operator() ( const D3DXKEY_QUATERNION& a, const D3DXKEY_QUATERNION& b ) const
    {
        return a.Time < b.Time;
    }
};

struct D3DXKEY_QUATERNION_EQUAL
{
    bool operator () ( const D3DXKEY_QUATERNION& a, const D3DXKEY_QUATERNION& b ) const
    {
        return a.Time == b.Time;
    }
};//


// 리턴 값
// 0 : 이상 없음
// 1 : unique 처리는 해야 함
// 2 : sort부터 새로 해야 함
static int _CheckVectorKeyOrder( D3DXKEY_VECTOR3*& pStart, D3DXKEY_VECTOR3* pEnd)
{
    ASSERT( pStart != NULL && pEnd != NULL );
    ASSERT( pStart <= pEnd );
    if ( pStart == pEnd )
    {
        return 0;
    }
    else if ( pStart + 1 == pEnd )
    {
        if ( pStart->Time < 0.f )
            pStart->Time = 0.f;
        return 0;
    }

    D3DXKEY_VECTOR3* pCur = pStart;
    while( pCur != pEnd && pCur->Time < 0.f )
        pCur++;

    if ( pCur == pEnd )
    {
        pCur--;
        pCur->Time = 0.f;
        pStart = pCur;
        return 0;
    }
    else if ( pCur + 1 == pEnd )
    {
        if ( pCur->Time == 0.f )
        {
            pStart = pCur;
            return 0;
        }
        else
        {
            pCur--;
            pCur->Time = 0.f;
            pStart = pCur;
            return  0;
        }//if.. else..
    }
    // pCur + 1 < pEnd

    D3DXKEY_VECTOR3* pActualStart = pCur;
    D3DXKEY_VECTOR3* pZeroCandidate = NULL;
    if ( pActualStart != pStart && pActualStart->Time > 0.f )
    {
        pZeroCandidate = pActualStart;
        pZeroCandidate--;
    }//if.. else..

    bool    bDuplicated = false;
    D3DXKEY_VECTOR3* pPrev = pCur;
    for( pCur++; pCur != pEnd; pPrev = pCur, pCur++ )
    {
        if ( pPrev->Time > pCur->Time )
            return 2;
        else if ( pPrev->Time == pCur->Time )
            bDuplicated = true;
    }//for

    if ( pZeroCandidate != NULL )
    {
        pZeroCandidate->Time = 0.f;
        pStart = pZeroCandidate;
    }
    else
    {
        pStart = pActualStart;
    }//if.. else..
    return ( bDuplicated == true ) ? 1 : 0;
}//_CheckVectorKeyOrder()


static int _CheckQuaternionKeyOrder( D3DXKEY_QUATERNION*& pStart, D3DXKEY_QUATERNION* pEnd)
{
    ASSERT( pStart != NULL && pEnd != NULL );
    ASSERT( pStart <= pEnd );
    if ( pStart == pEnd )
    {
        return 0;
    }
    else if ( pStart + 1 == pEnd )
    {
        if ( pStart->Time < 0.f )
            pStart->Time = 0.f;
        return 0;
    }

    D3DXKEY_QUATERNION* pCur = pStart;
    while( pCur != pEnd && pCur->Time < 0.f )
        pCur++;

    if ( pCur == pEnd )
    {
        pCur--;
        pCur->Time = 0.f;
        pStart = pCur;
        return 0;
    }
    else if ( pCur + 1 == pEnd )
    {
        if ( pCur->Time == 0.f )
        {
            pStart = pCur;
            return 0;
        }
        else
        {
            pCur--;
            pCur->Time = 0.f;
            pStart = pCur;
            return  0;
        }//if.. else..
    }
    // pCur + 1 < pEnd

    D3DXKEY_QUATERNION* pActualStart = pCur;
    D3DXKEY_QUATERNION* pZeroCandidate = NULL;
    if ( pActualStart != pStart && pActualStart->Time > 0.f )
    {
        pZeroCandidate = pActualStart;
        pZeroCandidate--;
    }//if.. else..

    bool    bDuplicated = false;
    D3DXKEY_QUATERNION* pPrev = pCur;
    for( pCur++; pCur != pEnd; pPrev = pCur, pCur++ )
    {
        if ( pPrev->Time > pCur->Time )
            return 2;
        else if ( pPrev->Time == pCur->Time )
            bDuplicated = true;
    }//for

    if ( pZeroCandidate != NULL )
    {
        pZeroCandidate->Time = 0.f;
        pStart = pZeroCandidate;
    }
    else
    {
        pStart = pActualStart;
    }//if.. else..
    return ( bDuplicated == true ) ? 1 : 0;
}//_CheckVectorKeyOrder()


static void _CorrectVectorKeyOrder( D3DXKEY_VECTOR3*& pStart, D3DXKEY_VECTOR3*& pEnd)
{
    ASSERT( pStart != NULL && pEnd != NULL );
    ASSERT( pStart <= pEnd );

    int iRet = _CheckVectorKeyOrder( pStart, pEnd );
    switch( iRet )
    {
    case 0:
        break;
    case 1:
        pEnd = std::unique( pStart, pEnd, D3DXKEY_VECTOR3_EQUAL() );
        break;
    default: // 2
        std::sort( pStart, pEnd, D3DXKEY_VECTOR3_COMPARE() );
        pEnd = std::unique( pStart, pEnd, D3DXKEY_VECTOR3_EQUAL() );
        if ( pStart < pEnd && pStart->Time < 0.f )
        {
            pStart++;
            while( pStart != pEnd && pStart->Time < 0.f )
                pStart++;
        }//if
    }//switch
}

static void _CorrectQuaternionKeyOrder( D3DXKEY_QUATERNION*& pStart, D3DXKEY_QUATERNION*& pEnd)
{
    ASSERT( pStart != NULL && pEnd != NULL );
    ASSERT( pStart <= pEnd );

    int iRet = _CheckQuaternionKeyOrder( pStart, pEnd );
    switch( iRet )
    {
    case 0:
        break;
    case 1:
        pEnd = std::unique( pStart, pEnd, D3DXKEY_QUATERNION_EQUAL() );
        break;
    default: // 2
        std::sort( pStart, pEnd, D3DXKEY_QUATERNION_COMPARE() );
        pEnd = std::unique( pStart, pEnd, D3DXKEY_QUATERNION_EQUAL() );
        if ( pStart < pEnd && pStart->Time < 0.f )
        {
            pStart++;
            while( pStart != pEnd && pStart->Time < 0.f )
                pStart++;
        }//if
    }//switch
}

inline	static unsigned	_SignCode( float fValue )
{
	return	 ( fValue > 0.f ) ? 1 : ( ( fValue < 0.f ) ? 2 : 0 );
}

inline  static bool		_IsEqualSign( unsigned uiFlags1, unsigned uiFlags2 )
{
	unsigned uiFlags = uiFlags1 | uiFlags;
	return	false == ( ( ( uiFlags & 0x3 ) == 0x3 ) || ( ( uiFlags & 0xc ) == 0xc ) || ( ( uiFlags & 0x30 ) == 0x30 ) );
}

static void _OptimizeKeys( D3DXKEY_VECTOR3*& pScalesStart_, D3DXKEY_VECTOR3*& pScalesEnd_,
	D3DXKEY_VECTOR3*& pTransStart_, D3DXKEY_VECTOR3*& pTransEnd_,
	D3DXKEY_QUATERNION*& pRotsStart_, D3DXKEY_QUATERNION*& pRotsEnd_,
	float fOptTolerance )
{
	if ( pScalesStart_ < pScalesEnd_ )
	{
		_CorrectVectorKeyOrder( pScalesStart_, pScalesEnd_ );
		int	iNumScaleKeys = (int) ( pScalesEnd_ - pScalesStart_ );
		if ( iNumScaleKeys > 1 )
		{
			unsigned	NumScaleKeys = (unsigned) iNumScaleKeys;
			D3DXKEY_VECTOR3* pSrcScaleKey = pScalesStart_;
			D3DXKEY_VECTOR3* pDestScaleKey = pScalesStart_;
			unsigned uDestFlags = _SignCode(pDestScaleKey->Value.x)|(_SignCode(pDestScaleKey->Value.y)<<2)|(_SignCode(pDestScaleKey->Value.z)<<4);
			unsigned uPrevSrcFlags = 0;
			unsigned uSrcFlags = 0;
			float       fInvInterval = 0.f;
			D3DXVECTOR3 vDiff(0,0,0);
			D3DXVECTOR3 vEst(0,0,0);
			unsigned    uNumRetries = 0;
			unsigned    uNumRetriesAccum = 0;
			float       fTolerance = 0.f;
			float fToleranceRatio = 1.f;

			int iState = 0;
			// 0 : 1개 base 만 저장된 상태
			// 1 : 2개 이상 pending 된 상태
			// 2 : diff 상태

			uPrevSrcFlags = uDestFlags;
			pSrcScaleKey++;
			uSrcFlags = _SignCode(pSrcScaleKey->Value.x)|(_SignCode(pSrcScaleKey->Value.y)<<2)|(_SignCode(pSrcScaleKey->Value.z)<<4);
			for( unsigned u = 1; u != NumScaleKeys; u++ )
			{
				if ( iState == 0 || iState == 1 )
				{
					bool	bEqualSign = _IsEqualSign( uSrcFlags, uDestFlags );
					if ( bEqualSign == true
						&& _IsWithinTolerance( pSrcScaleKey->Value, pDestScaleKey->Value, fOptTolerance ) == true )
					{
						iState = 1;
						uPrevSrcFlags = uSrcFlags;
						pSrcScaleKey++;
						if ( u + 1 < NumScaleKeys )
							uSrcFlags = _SignCode(pSrcScaleKey->Value.x)|(_SignCode(pSrcScaleKey->Value.y)<<2)|(_SignCode(pSrcScaleKey->Value.z)<<4);
					}
					else
					{
						if ( iState == 1 )
						{
							pDestScaleKey++;
							*pDestScaleKey = *(pSrcScaleKey-1);
						}
						if ( bEqualSign == true )
						{
							fInvInterval = 1.f / ( pSrcScaleKey->Time - pDestScaleKey->Time );
							vDiff = pSrcScaleKey->Value - pDestScaleKey->Value;
							iState = 2;
							uDestFlags = uPrevSrcFlags;
							uPrevSrcFlags = uSrcFlags;
							pSrcScaleKey++;
							if ( u + 1 < NumScaleKeys )
								uSrcFlags = _SignCode(pSrcScaleKey->Value.x)|(_SignCode(pSrcScaleKey->Value.y)<<2)|(_SignCode(pSrcScaleKey->Value.z)<<4);
							uNumRetriesAccum = uNumRetries = 1;
							fTolerance = fOptTolerance / KAC_LINEAR_COMPRESSION_RETRY;
						}
						else
						{
							pDestScaleKey++;
							*pDestScaleKey = *pSrcScaleKey;
							uDestFlags = uSrcFlags;
							iState = 0;
							uPrevSrcFlags = uSrcFlags;
							pSrcScaleKey++;
							if ( u + 1 < NumScaleKeys )
								uSrcFlags = _SignCode(pSrcScaleKey->Value.x)|(_SignCode(pSrcScaleKey->Value.y)<<2)|(_SignCode(pSrcScaleKey->Value.z)<<4);
						}//if.. else..
					}//if.. else..
				}
				else
				{
					bool	bEqualSign = _IsEqualSign( uSrcFlags, uDestFlags );
					if ( bEqualSign == true )
					{
						vEst = pDestScaleKey->Value + ( ( pSrcScaleKey->Time - pDestScaleKey->Time ) * fInvInterval ) * vDiff;
						fToleranceRatio = 1.f;
						if ( _IsWithinTolerance( pSrcScaleKey->Value, vEst, fTolerance, fToleranceRatio ) == true )
						{
							uPrevSrcFlags = uSrcFlags;
							pSrcScaleKey++;
							if ( u + 1 < NumScaleKeys )
								uSrcFlags = _SignCode(pSrcScaleKey->Value.x)|(_SignCode(pSrcScaleKey->Value.y)<<2)|(_SignCode(pSrcScaleKey->Value.z)<<4);
							continue;
						}
						else if ( uNumRetries < KAC_LINEAR_COMPRESSION_RETRY )
						{
							float   fNewRetries = fToleranceRatio * uNumRetries;
							if ( fNewRetries <= KAC_LINEAR_COMPRESSION_RETRY )
							{
								unsigned uNewNumRetries = (unsigned) ceilf( fNewRetries );
								uNewNumRetries = __max( uNumRetries + 1, uNewNumRetries );
								uNumRetriesAccum += uNewNumRetries;
								if ( uNumRetriesAccum <= KAC_LINEAR_COMPRESSION_RETRY )
								{
									uNumRetries = uNewNumRetries;
									fTolerance = ( fOptTolerance / KAC_LINEAR_COMPRESSION_RETRY ) * uNewNumRetries;
									fInvInterval = 1.f / ( pSrcScaleKey->Time - pDestScaleKey->Time );
									vDiff = pSrcScaleKey->Value - pDestScaleKey->Value;
									uPrevSrcFlags = uSrcFlags;
									pSrcScaleKey++;
									if ( u + 1 < NumScaleKeys )
										uSrcFlags = _SignCode(pSrcScaleKey->Value.x)|(_SignCode(pSrcScaleKey->Value.y)<<2)|(_SignCode(pSrcScaleKey->Value.z)<<4);
									continue;
								}//if
							}//if
						}//if
					}//if
					pDestScaleKey++;
					*pDestScaleKey = *(pSrcScaleKey-1);
					uDestFlags = uPrevSrcFlags;
					iState = 0;
					u--;
				}//if.. 
			}
			switch( iState )
			{
			case 0:
				break;
			default: // 1, 2
				{
					pDestScaleKey++;
					*pDestScaleKey = *(pSrcScaleKey-1);
				}//if
				break;
			}//switch
			pDestScaleKey++;
			pScalesEnd_ = pDestScaleKey;
		}
	}
	if ( pTransStart_ < pTransEnd_ )
	{
		_CorrectVectorKeyOrder( pTransStart_, pTransEnd_ );
		int	iNumTransKeys = (int) ( pTransEnd_ - pTransStart_ );
		if ( iNumTransKeys > 1 )
		{
			unsigned	NumTransKeys = (unsigned) iNumTransKeys;
			D3DXKEY_VECTOR3* pSrcTransKey = pTransStart_;
			D3DXKEY_VECTOR3* pDestTransKey = pTransStart_;
			float       fInvInterval = 0.f;
			D3DXVECTOR3 vDiff(0,0,0);
			D3DXVECTOR3 vEst(0,0,0);
			unsigned    uNumRetries = 0;
			unsigned    uNumRetriesAccum = 0;
			float       fTolerance = 0.f;
			float fToleranceRatio = 1.f;

			int iState = 0;
			// 0 : 1개 base 만 저장된 상태
			// 1 : 2개 이상 pending 된 상태
			// 2 : diff 상태

			pSrcTransKey++;
			for( unsigned u = 1; u != NumTransKeys; u++ )
			{
				if ( iState == 0 || iState == 1 )
				{
					if (_IsWithinTolerance( pSrcTransKey->Value, pDestTransKey->Value, fOptTolerance ) == true )
					{
						iState = 1;
						pSrcTransKey++;
					}
					else
					{
						if ( iState == 1 )
						{
							pDestTransKey++;
							*pDestTransKey = *(pSrcTransKey-1);
						}
						fInvInterval = 1.f / ( pSrcTransKey->Time - pDestTransKey->Time );
						vDiff = pSrcTransKey->Value - pDestTransKey->Value;
						iState = 2;
						pSrcTransKey++;
						uNumRetriesAccum = uNumRetries = 1;
						fTolerance = fOptTolerance / KAC_LINEAR_COMPRESSION_RETRY;
					}//if.. else..
				}
				else
				{
					vEst = pDestTransKey->Value + ( ( pSrcTransKey->Time - pDestTransKey->Time ) * fInvInterval ) * vDiff;
					fToleranceRatio = 1.f;
					if ( _IsWithinTolerance( pSrcTransKey->Value, vEst, fTolerance, fToleranceRatio ) == true )
					{
						pSrcTransKey++;
						continue;
					}
					else if ( uNumRetries < KAC_LINEAR_COMPRESSION_RETRY )
					{
						float   fNewRetries = fToleranceRatio * uNumRetries;
						if ( fNewRetries <= KAC_LINEAR_COMPRESSION_RETRY )
						{
							unsigned uNewNumRetries = (unsigned) ceilf( fNewRetries );
							uNewNumRetries = __max( uNumRetries + 1, uNewNumRetries );
							uNumRetriesAccum += uNewNumRetries;
							if ( uNumRetriesAccum <= KAC_LINEAR_COMPRESSION_RETRY )
							{
								uNumRetries = uNewNumRetries;
								fTolerance = ( fOptTolerance / KAC_LINEAR_COMPRESSION_RETRY ) * uNewNumRetries;
								fInvInterval = 1.f / ( pSrcTransKey->Time - pDestTransKey->Time );
								vDiff = pSrcTransKey->Value - pDestTransKey->Value;
								pSrcTransKey++;
								continue;
							}//if
						}//if
					}//if
					pDestTransKey++;
					*pDestTransKey = *(pSrcTransKey-1);
					iState = 0;
					u--;
				}//if.. 
			}
			switch( iState )
			{
			case 0:
				break;
			default: // 1, 2
				{
					pDestTransKey++;
					*pDestTransKey = *(pSrcTransKey-1);
				}//if
				break;
			}//switch
			pDestTransKey++;
			pTransEnd_ = pDestTransKey;
		}
	}
	if ( pRotsStart_ < pRotsEnd_ )
	{
		_CorrectQuaternionKeyOrder( pRotsStart_, pRotsEnd_ );
		int	iNumRotKeys = (int) ( pRotsEnd_ - pRotsStart_ );
		if ( iNumRotKeys > 1 )
		{
			unsigned	NumRotKeys = (unsigned) iNumRotKeys;
			D3DXKEY_QUATERNION*	pSrcRotKey = pRotsStart_;
			D3DXKEY_QUATERNION* pDestRotKey = pRotsStart_;
			D3DXQuaternionNormalize( &pDestRotKey->Value, &pDestRotKey->Value );
			float	fInvInterval = 0.f;
			D3DXQUATERNION	qDiffNext(0,0,0,1);
			D3DXQUATERNION	qPrev(0,0,0,1);
			D3DXQUATERNION	qIdentity(0,0,0,1);
			float	fDiffOmega = 0.f;
			float	fDiffInvSinom = 0.f;
			float	fPrevOmega = 0.f;
			float	fPrevInvSinom = 0.f;
			unsigned	uNumRetries = 0;
			unsigned	uNumRetriesAccum = 0;
			float	fTolerance = 0.f;
			float	fToleranceRatio = 1.f;
			
			int iState = 0;
			// 0 : 1개 base 만 저장된 상태
			// 1 : 2개 이상 pending 된 상태
			// 2 : diff 상태
			pSrcRotKey++;
			if ( pDestRotKey->Value.w < 0.f )
				pDestRotKey->Value = -pDestRotKey->Value;
			float	fSinom, fCosom;
	        D3DX_ALIGN16    D3DXQUATERNION  qs, iqd, q, q2, qEst;
			for( unsigned u = 1; u != NumRotKeys; u++ )
			{
				D3DXQuaternionNormalize( &qs, &pSrcRotKey->Value );
				D3DXQuaternionConjugate( &iqd, &pDestRotKey->Value );
				D3DXQuaternionMultiply( &q, &qs, &iqd );
				fCosom = q.w;
				if ( fCosom < 0.f )
				{
					fCosom = -fCosom;
					q = -q;
					qs = -qs;
				}//if

				if ( iState == 0 || iState == 1 )
				{
					if ( _IsWithinTolerance( q, qIdentity, fOptTolerance ) == true )
					{
						iState = 1;
						qPrev = qs;
						pSrcRotKey++;
					}
					else
					{
						if ( iState == 1 )
						{
							pDestRotKey++;
							pDestRotKey->Value = qPrev;
							pDestRotKey->Time = (pSrcRotKey-1)->Time;
						}
						fInvInterval = 1.f / ( pSrcRotKey->Time - pDestRotKey->Time );
						qDiffNext = qPrev = qs;
						fSinom = sqrtf( (q.x)*(q.x) + (q.y)*(q.y) + (q.z)*(q.z) );
						fDiffOmega = fPrevOmega = atan2( fSinom, fCosom );
						if ( fSinom >= 9.765625002015999999791910092843e-4f )
							fDiffInvSinom = fPrevInvSinom = 1.f / fSinom;
						else
							fDiffInvSinom = fPrevInvSinom = 0.f;  
						iState = 2;
						pSrcRotKey++;
						uNumRetriesAccum = uNumRetries = 1;
						fTolerance = fOptTolerance / KAC_LINEAR_COMPRESSION_RETRY;
					}//if.. else..
				}
				else
				{
					D3DXQuaternionConjugate( &iqd, &qPrev );                
					D3DXQuaternionMultiply( &q2, &qs, &iqd );
					if ( q2.w > 0.f )
					{   
						float   fRatio = ( pSrcRotKey->Time - pDestRotKey->Time ) * fInvInterval;
						if ( fDiffInvSinom == 0.f )
						{
							qEst = ( 1.f - fRatio ) * pDestRotKey->Value + fRatio * qDiffNext;
						}
						else
						{
							float   fCoeff1 = sinf( ( 1.f - fRatio ) * fDiffOmega ) * fDiffInvSinom;
							float   fCoeff2 = sinf( fRatio * fDiffOmega ) * fDiffInvSinom;
							qEst = fCoeff1 * pDestRotKey->Value + fCoeff2 * qDiffNext;
						}//if.. else..
						fToleranceRatio = 1.f;
						if ( _IsWithinTolerance( qEst, qs, fTolerance, fToleranceRatio ) == true )
						{
							qPrev = qs;
							fSinom = sqrtf( (q.x)*(q.x) + (q.y)*(q.y) + (q.z)*(q.z) );
							fPrevOmega = atan2( fSinom, fCosom );
							if ( fSinom >= 9.765625002015999999791910092843e-4f )
							{
								fPrevInvSinom = 1.f / fSinom;
								if ( fDiffInvSinom == 0.f )
								{
									fInvInterval = 1.f / ( pSrcRotKey->Time - pDestRotKey->Time );
									qDiffNext = qs;
									fDiffOmega = fPrevOmega;
									fDiffInvSinom = fPrevInvSinom;
								}//if
							}
							else
								fPrevInvSinom = 0.f;
							pSrcRotKey++;
							continue;
						}
						else if ( uNumRetries < KAC_LINEAR_COMPRESSION_RETRY )
						{
							float   fNewRetries = fToleranceRatio * uNumRetries;
							if ( fNewRetries <= KAC_LINEAR_COMPRESSION_RETRY )
							{
								unsigned uNewNumRetries = (unsigned) ceilf( fNewRetries );
								uNewNumRetries = __max( uNumRetries + 1, uNewNumRetries );
								uNumRetriesAccum += uNewNumRetries;
								if ( uNumRetriesAccum <= KAC_LINEAR_COMPRESSION_RETRY )
								{
									uNumRetries = uNewNumRetries;
									fTolerance = ( fOptTolerance / KAC_LINEAR_COMPRESSION_RETRY ) * uNewNumRetries;
									qPrev = qs;
									fSinom = sqrtf( (q.x)*(q.x) + (q.y)*(q.y) + (q.z)*(q.z) );
									fPrevOmega = atan2( fSinom, fCosom );
									if ( fSinom >= 9.765625002015999999791910092843e-4f )
										fPrevInvSinom = 1.f / fSinom;
									else
										fPrevInvSinom = 0.f;
									fInvInterval = 1.f / ( pSrcRotKey->Time - pDestRotKey->Time );
									qDiffNext = qs;
									fDiffOmega = fPrevOmega;
									fDiffInvSinom = fPrevInvSinom;
									pSrcRotKey++;
									continue;
								}//if
							}//if
						}//if.. 
					}//if
//	out:
					pDestRotKey++;
					pDestRotKey->Value = qPrev;
					pDestRotKey->Time = (pSrcRotKey-1)->Time;
					iState = 0;
					u--;
				}//if.. else..
			}//for

			switch( iState )
			{
			case 0:
				break;
			default: // 1, 2
				pDestRotKey++;
				pDestRotKey->Value = qPrev;
				pDestRotKey->Time = (pSrcRotKey-1)->Time;
				break;
			}//switch

			pDestRotKey++;
			pRotsEnd_ = pDestRotKey;

		}
	}

}
#endif	X2OPTIMIZE_ANIMATION_LOSSY_COMPRESSION


static void _OptimizeKeys( 
        DWORD& dwInOutScales_, D3DXKEY_VECTOR3 aInOutScales_[]
        , DWORD& dwInOutTrans_, D3DXKEY_VECTOR3 aInOutTrans_[]
        , DWORD& dwInOutRots_, D3DXKEY_QUATERNION aInOutRots_[] )
{
    DWORD   dwOrigNumScales = dwInOutScales_;
    DWORD   dwOrigNumTrans = dwInOutTrans_;
    DWORD   dwOrigNumRots = dwInOutRots_;

    int     iTimeMaxComp = -1;
    float   fTimeMax = -FLT_MAX;

    if ( dwOrigNumTrans >= 1 )
    {
        ASSERT( aInOutTrans_ != NULL );
        DWORD   dwCurSize = 1;
        D3DXKEY_VECTOR3& vFirst = aInOutTrans_[ 0 ];
        if ( fTimeMax < vFirst.Time )
        {
            fTimeMax = vFirst.Time;
            iTimeMaxComp = 1;
        }//if
        if ( _IsWithinTolerance( vFirst.Value.x, s_v3ZeroTrans.x ) )
            vFirst.Value.x = s_v3ZeroTrans.x;
        if ( _IsWithinTolerance( vFirst.Value.y, s_v3ZeroTrans.y ) )
            vFirst.Value.y = s_v3ZeroTrans.y;
        if ( _IsWithinTolerance( vFirst.Value.z, s_v3ZeroTrans.z ) )
            vFirst.Value.z = s_v3ZeroTrans.z;
        for( DWORD dwKey = 1; dwKey < dwOrigNumTrans; dwKey++ )
        {
            const D3DXKEY_VECTOR3& vCur = aInOutTrans_[ dwCurSize - 1 ];
            const D3DXKEY_VECTOR3& vNew = aInOutTrans_[ dwKey ];
            if ( fTimeMax < vNew.Time )
            {
                fTimeMax = vNew.Time;
                iTimeMaxComp = 1;
            }//if
            if ( _IsWithinTolerance( vCur.Value, vNew.Value ) )
                continue;
            aInOutTrans_[ dwCurSize ] = vNew;
            if ( _IsWithinTolerance( vCur.Value.x, vNew.Value.x ) )
                aInOutTrans_[ dwCurSize ].Value.x = vCur.Value.x;
            if ( _IsWithinTolerance( vCur.Value.y, vNew.Value.y ) )
                aInOutTrans_[ dwCurSize ].Value.y = vCur.Value.y;
            if ( _IsWithinTolerance( vCur.Value.z, vNew.Value.z ) )
                aInOutTrans_[ dwCurSize ].Value.z = vCur.Value.z;
            dwCurSize++;
        }//for
        if ( dwCurSize == 1 && _IsWithinTolerance( vFirst.Value, s_v3ZeroTrans ) )
            dwInOutTrans_ = 0;
        else
            dwInOutTrans_ = dwCurSize;
    }//if

   if ( dwOrigNumScales >= 1 )
    {
        ASSERT( aInOutScales_ != NULL );
        DWORD   dwCurSize = 1;
        D3DXKEY_VECTOR3& vFirst = aInOutScales_[ 0 ];
        if ( fTimeMax < vFirst.Time )
        {
            fTimeMax = vFirst.Time;
            iTimeMaxComp = 0;
        }//if
        if ( _IsWithinTolerance( vFirst.Value.x, s_v3UnitScale.x ) )
            vFirst.Value.x = s_v3UnitScale.x;
        if ( _IsWithinTolerance( vFirst.Value.y, s_v3UnitScale.y ) )
            vFirst.Value.y = s_v3UnitScale.y;
        if ( _IsWithinTolerance( vFirst.Value.z, s_v3UnitScale.z ) )
            vFirst.Value.z = s_v3UnitScale.z;
        for( DWORD dwKey = 1; dwKey < dwOrigNumScales; dwKey++ )
        {
            const D3DXKEY_VECTOR3& vCur = aInOutScales_[ dwCurSize - 1 ];
            const D3DXKEY_VECTOR3& vNew = aInOutScales_[ dwKey ];
            if ( fTimeMax < vNew.Time )
            {
                fTimeMax = vNew.Time;
                iTimeMaxComp = 0;
            }//if
            if ( _IsWithinTolerance( vCur.Value, vNew.Value ) )
                continue;
            aInOutScales_[ dwCurSize ] = vNew;
            if ( _IsWithinTolerance( vCur.Value.x, vNew.Value.x ) )
                aInOutScales_[ dwCurSize ].Value.x = vCur.Value.x;
            if ( _IsWithinTolerance( vCur.Value.y, vNew.Value.y ) )
                aInOutScales_[ dwCurSize ].Value.y = vCur.Value.y;
            if ( _IsWithinTolerance( vCur.Value.z, vNew.Value.z ) )
                aInOutScales_[ dwCurSize ].Value.z = vCur.Value.z;
            dwCurSize++;
        }//for
        if ( dwCurSize == 1 && _IsWithinTolerance( vFirst.Value, s_v3UnitScale ) )
            dwInOutScales_ = 0;
        else
            dwInOutScales_ = dwCurSize;
    }//if

    if ( dwOrigNumRots >= 1 )
    {
        ASSERT( aInOutRots_ != NULL );
        DWORD   dwCurSize = 1;
        D3DXKEY_QUATERNION& vFirst = aInOutRots_[ 0 ];
        if ( fTimeMax < vFirst.Time )
        {
            fTimeMax = vFirst.Time;
            iTimeMaxComp = 2;
        }//if
        if ( _IsWithinTolerance( vFirst.Value, s_qUnitRot ) )
            vFirst.Value = s_qUnitRot;
        for( DWORD dwKey = 1; dwKey < dwOrigNumRots; dwKey++ )
        {
            const D3DXKEY_QUATERNION& vCur = aInOutRots_[ dwCurSize - 1 ];
            const D3DXKEY_QUATERNION& vNew = aInOutRots_[ dwKey ];
            if ( fTimeMax < vNew.Time )
            {
                fTimeMax = vNew.Time;
                iTimeMaxComp = 2;
            }//if
            if ( _IsWithinTolerance( vCur.Value, vNew.Value ) )
                continue;
            aInOutRots_[ dwCurSize ] = vNew;
            dwCurSize++;
        }//for
        if ( dwCurSize == 1 && _IsWithinTolerance( vFirst.Value, s_qUnitRot ) )
            dwInOutRots_ = 0;
        else
            dwInOutRots_ = dwCurSize;
    }//if

    if ( dwInOutScales_ <= 1 && dwInOutTrans_ <= 1 && dwInOutRots_ <= 1 )
    {
        if ( dwOrigNumScales <= 1 && dwOrigNumTrans <= 1 && dwOrigNumRots <= 1 )
            return;
    }//if
    ASSERT( iTimeMaxComp >= 0 );

    float fNewTimeMax = -FLT_MAX;
    if ( dwInOutTrans_ > 1 && aInOutTrans_[ dwInOutTrans_ - 1 ].Time > fNewTimeMax )
        fNewTimeMax = aInOutTrans_[ dwInOutTrans_ - 1 ].Time;
    if ( dwInOutScales_ > 1 && aInOutScales_[ dwInOutScales_ - 1 ].Time > fNewTimeMax )
        fNewTimeMax = aInOutScales_[ dwInOutScales_ - 1 ].Time;
    if ( dwInOutRots_ > 1 && aInOutRots_[ dwInOutRots_ - 1 ].Time > fNewTimeMax )
        fNewTimeMax = aInOutRots_[ dwInOutRots_ - 1 ].Time;

    if ( fNewTimeMax >= fTimeMax )
    {
        ASSERT( fNewTimeMax == fTimeMax );
        return;
    }//if

    if ( dwInOutTrans_ > 1 && dwInOutTrans_ < dwOrigNumTrans )
    {
        aInOutTrans_[ dwInOutTrans_ ].Value = aInOutTrans_[ dwInOutTrans_ - 1 ].Value;
        aInOutTrans_[ dwInOutTrans_++ ].Time = fTimeMax;
        return;
    }//if
    if ( dwInOutScales_ > 1 && dwInOutScales_ < dwOrigNumScales )
    {
        aInOutScales_[ dwInOutScales_ ].Value = aInOutScales_[ dwInOutScales_ - 1 ].Value;
        aInOutScales_[ dwInOutScales_++ ].Time = fTimeMax;
        return;
    }//if
    if ( dwInOutRots_ > 1 && dwInOutRots_ < dwOrigNumRots )
    {
        aInOutRots_[ dwInOutRots_ ].Value = aInOutRots_[ dwInOutRots_ - 1 ].Value;
        aInOutRots_[ dwInOutRots_++ ].Time = fTimeMax;
        return;
    }//if

    switch( iTimeMaxComp )
    {
    case 1:
        ASSERT( dwInOutTrans_ < dwOrigNumTrans );
        if ( dwInOutTrans_ == 0 )
        {
            aInOutTrans_[ 1 ].Value = aInOutTrans_[ 0 ].Value;
            aInOutTrans_[ 1 ].Time = fTimeMax;
            dwInOutTrans_ = 2;
        }
        else
        {
            aInOutTrans_[ dwInOutTrans_ ].Value = aInOutTrans_[ dwInOutTrans_ - 1 ].Value;
            aInOutTrans_[ dwInOutTrans_++ ].Time = fTimeMax;
        }//if.. else..
        break;
    case 2:
        ASSERT( dwInOutRots_ < dwOrigNumRots );
        if ( dwInOutRots_ == 0 )
        {
            aInOutRots_[ 1 ].Value = aInOutRots_[ 0 ].Value;
            aInOutRots_[ 1 ].Time = fTimeMax;
            dwInOutRots_ = 2;
        }
        else
        {
            aInOutRots_[ dwInOutRots_ ].Value = aInOutRots_[ dwInOutRots_ - 1 ].Value;
            aInOutRots_[ dwInOutRots_++ ].Time = fTimeMax;
        }//if.. else..
        break;
    default: // case 0:
        ASSERT( dwInOutScales_ < dwOrigNumScales );
        if ( dwInOutScales_ == 0 )
        {
            aInOutScales_[ 1 ].Value = aInOutScales_[ 0 ].Value;
            aInOutScales_[ 1 ].Time = fTimeMax;
            dwInOutScales_ = 2;
        }
        else
        {
            aInOutScales_[ dwInOutScales_ ].Value = aInOutScales_[ dwInOutScales_ - 1 ].Value;
            aInOutScales_[ dwInOutScales_++ ].Time = fTimeMax;
        }//if.. else..
        break;
    };//
}//_OptimizeKeys()



HRESULT KTDXAnimationController::WriteToFileSerializer( KFileSerializer& kInOutFile_, const CKTDXDeviceXSkinMesh* pInSkinMesh_, LPD3DXANIMATIONCONTROLLER pInAC_ )
{
    ASSERT( pInAC_ != NULL );
    ASSERT( pInSkinMesh_ != NULL );

    HRESULT hr;

    DWORD   dwNumAnimSets = pInAC_->GetNumAnimationSets();
    ASSERT( dwNumAnimSets > 0 );
    DWORD   dwStructSize = sizeof(KACHeader) + dwNumAnimSets * sizeof(DWORD);
    DWORD   dwOffset_Header = kInOutFile_.AppendSpace( dwStructSize );

    KACHeader*  pkacHeader = (KACHeader*) malloc( dwStructSize );

    pkacHeader->m_dwMagic = KOGANIMATIONCONTROLLER_FORMAT_MAGIC;
    pkacHeader->m_dwVersion = KOGANIMATIONCONTROLLER_FORMAT_VERSION;
    pkacHeader->m_dwNumAnimationSets = dwNumAnimSets;

    if ( dwNumAnimSets > 0 )
    {
        ZeroMemory( pkacHeader->m_adwOffset_AnimationSets, sizeof(DWORD) * dwNumAnimSets );
        for( DWORD dwAnimSet = 0; dwAnimSet < dwNumAnimSets; dwAnimSet++ )
        {   
            CComPtr<ID3DXAnimationSet> pAnimSet;
            hr = pInAC_->GetAnimationSet( dwAnimSet, &pAnimSet );
            ASSERT( SUCCEEDED( hr ) );
            if ( SUCCEEDED( hr ) )
            {
                ASSERT( pAnimSet != NULL );
                pkacHeader->m_adwOffset_AnimationSets[ dwAnimSet ] = kInOutFile_.GetSize();
                hr = _WriteToFileSerializer( kInOutFile_, pInSkinMesh_, pAnimSet );
                ASSERT( SUCCEEDED( hr ) );
                if ( FAILED( hr ) )
                    pkacHeader->m_adwOffset_AnimationSets[ dwAnimSet ] = NULL;
            }//if
        }//for
    }//for

    hr = kInOutFile_.CopyData( dwOffset_Header, dwStructSize, pkacHeader );
    ASSERT( SUCCEEDED( hr ) );
    SAFE_FREE( pkacHeader );

    return S_OK;
}//KTDXAnimationController::WriteToFileSerializer()


LPD3DXANIMATIONCONTROLLER KTDXAnimationController::CloneOptimizedAC( const CKTDXDeviceXSkinMesh* pInSkinMesh_, LPD3DXANIMATIONCONTROLLER pInAC_ )
{
    ASSERT( pInSkinMesh_ != NULL );
    ASSERT( pInAC_ != NULL );

    HRESULT hr;
    DWORD   dwNumAnimSets = pInAC_->GetNumAnimationSets();
    ASSERT( dwNumAnimSets > 0 );
    DWORD   dwNumNamedFrames = D3DXFrameNumNamedMatrices( pInSkinMesh_->GetFrameRoot() );
    LPD3DXANIMATIONCONTROLLER   pOutAC = NULL;

    hr = D3DXCreateAnimationController( 
        dwNumNamedFrames
        , dwNumAnimSets
        , KAC_DEFAULT_MAX_NUM_TRACKS
        , KAC_DEFAULT_MAX_NUM_EVENTS
        , &pOutAC );
    if ( FAILED( hr ) )
    {
        ErrorLog( KEM_ERROR395 );
        ASSERT( !"KTDXAnimationController::CloneOptimizedAC failed" );
        return NULL;
    }//if

    for( DWORD dwAnimSet = 0; dwAnimSet < dwNumAnimSets; dwAnimSet++ )
    {   
        CComPtr<ID3DXAnimationSet>  pAnimSet;
        hr = pInAC_->GetAnimationSet( dwAnimSet, &pAnimSet );
        ASSERT( SUCCEEDED( hr ) );
        if ( FAILED( hr ) )
        {
            ErrorLog( KEM_ERROR396 );
        }
        else
        {
            ASSERT( pAnimSet != NULL );
            CComPtr<ID3DXAnimationSet>  pNewAnimSet;
            hr = _Optimize( pAnimSet, &pNewAnimSet );
            ASSERT( SUCCEEDED( hr ) );
            if ( FAILED( hr ) )
            {
                ErrorLog( KEM_ERROR397 );
            }
            else
            {
                ASSERT( pNewAnimSet != NULL );
                hr = pOutAC->RegisterAnimationSet( pNewAnimSet );
                ASSERT( SUCCEEDED( hr ) );
                if ( FAILED( hr ) )
                {
                    ErrorLog( KEM_ERROR398 );
                }
            }
        }//if.. else..
    }//for

    hr = D3DXFrameRegisterNamedMatrices( pInSkinMesh_->GetFrameRoot(), pOutAC );
    ASSERT( SUCCEEDED( hr ) );
    if ( FAILED( hr ) )
    {
        ErrorLog( KEM_ERROR398 );
    }//if

    return pOutAC;
}//KTDXAnimationController::CloneOptimizedAC()


HRESULT KTDXAnimationController::ReadFromMemory( DWORD dwInSize_, const void* pInData_, CKTDXDeviceXSkinMesh::XSkinMeshProxy& proxy_, LPD3DXANIMATIONCONTROLLER* ppInAC_ )
{
    ASSERT( pInData_ != NULL && dwInSize_ > 0 );
    //ASSERT( pInSkinMesh_ != NULL );
    ASSERT( ppInAC_ != NULL );

    HRESULT hr;
    *ppInAC_ = NULL;
    KMemoryDeserializer kMemory( dwInSize_, pInData_ );

    KACHeader kacHeader;
//#ifdef	X2OPTIMIZE_KFILESERIALIZER_MODIFIED
    hr = kMemory.RetrieveData( &kacHeader, 0, sizeof(KACHeader) );
//#else	X2OPTIMIZE_KFILESERIALIZER_MODIFIED
//    hr = kMemory.RetrieveData( 0, sizeof(KACHeader), &kacHeader );
//#endif	X2OPTIMIZE_KFILESERIALIZER_MODIFIED
    if ( FAILED( hr ) )
    {
        ErrorLog( KEM_ERROR399 );
        ASSERT( !"invalid header" );
        return E_FAIL;
    }//if

    if ( kacHeader.m_dwMagic != KOGANIMATIONCONTROLLER_FORMAT_MAGIC )
    {
        ErrorLog( KEM_ERROR400 );
        ASSERT( !"invalid format" );
        return E_FAIL;
    }//if

    if ( kacHeader.m_dwVersion > KOGANIMATIONCONTROLLER_FORMAT_VERSION )
    {
        ErrorLog( KEM_ERROR401 );
        ASSERT( !"invalid version" );
        return E_FAIL;
    }//if

    if ( kacHeader.m_dwNumAnimationSets == 0 )
    {
        ErrorLog( KEM_ERROR402 );
        ASSERT( !"no animation sets" );
        return E_FAIL;
    }//if

    const DWORD* pdwOffset_AnimSets = (const DWORD*) kMemory.AccessData( sizeof(KACHeader)
        , sizeof(kacHeader.m_dwNumAnimationSets) * sizeof(DWORD) );
    if ( pdwOffset_AnimSets == NULL )
    {
        ErrorLog( KEM_ERROR403 );
        ASSERT( !"invalid header" );
        return E_FAIL;
    }//if

    DWORD   dwNumNamedFrames = D3DXFrameNumNamedMatrices( proxy_.GetFrameRoot() );

    hr = D3DXCreateAnimationController( 
        dwNumNamedFrames
        , kacHeader.m_dwNumAnimationSets
        , KAC_DEFAULT_MAX_NUM_TRACKS
        , KAC_DEFAULT_MAX_NUM_EVENTS
        , ppInAC_ );
    if ( FAILED( hr ) )
    {
        ErrorLog( KEM_ERROR404 );
        ASSERT( !"D3DXCreateAnimationController failed" );
        goto error;
    }//if

    for( DWORD dwAnimSet = 0; dwAnimSet < kacHeader.m_dwNumAnimationSets; dwAnimSet++ )
    {
        hr = _ReadFromMemoryDeserializer( kMemory, pdwOffset_AnimSets[dwAnimSet], proxy_, *ppInAC_ );
        if ( FAILED( hr ) )
        {
            ErrorLog( KEM_ERROR405 );
            ASSERT( !"animation set creation failed" );
            goto error;
        }//if
    }//for

    hr = D3DXFrameRegisterNamedMatrices( proxy_.GetFrameRoot(), *ppInAC_ );
    if ( FAILED( hr ) )
    {
        ErrorLog( KEM_ERROR406 );
        ASSERT( !"D3DXFrameRegisterNamedMatrices failed" );
        goto error;
    }//if

    return S_OK;

error:

    SAFE_RELEASE( *ppInAC_ );
    *ppInAC_ = NULL;
    return hr;
}//KTDXAnimationController::ReadFromMemoryDeserializer()


static HRESULT _ReadFromMemoryDeserializer( const KMemoryDeserializer& kInMemory_, DWORD dwInOffset_, CKTDXDeviceXSkinMesh::XSkinMeshProxy& proxy_, LPD3DXANIMATIONCONTROLLER pInAC_ )
{
    ASSERT( pInAC_ != NULL );
    HRESULT hr;

    KACAnimationSet kacAnimSet;
//#ifdef	X2OPTIMIZE_KFILESERIALIZER_MODIFIED
	hr = kInMemory_.RetrieveData( &kacAnimSet, dwInOffset_, sizeof(KACAnimationSet) );
//#else	X2OPTIMIZE_KFILESERIALIZER_MODIFIED
//    hr = kInMemory_.RetrieveData( dwInOffset_, sizeof(KACAnimationSet), &kacAnimSet );
//#endif	X2OPTIMIZE_KFILESERIALIZER_MODIFIED
    if ( FAILED( hr ) )
    {
        ErrorLog( KEM_ERROR407 );        
        ASSERT( !"invalid KACAnimationSet format" );
        return E_FAIL;
    }//if

    if ( kacAnimSet.m_dwType == EAC_KEYFRAMED )
    {
        const KACKeyframedAnimationSet* pkacKeyframed = (const KACKeyframedAnimationSet*) kInMemory_.AccessData( dwInOffset_, sizeof(KACKeyframedAnimationSet) );
        if ( pkacKeyframed == NULL )
        {
            ErrorLog( KEM_ERROR408 );
            ASSERT( !"invalid KACKeyframedAnimationSet format" );
            return E_FAIL;
        }//if

        DWORD dwNumAnims = pkacKeyframed->m_dwNumAnimations;
        ASSERT( dwNumAnims > 0 );
        pkacKeyframed = (const KACKeyframedAnimationSet*) kInMemory_.AccessData( dwInOffset_, sizeof(KACKeyframedAnimationSet)
            + sizeof(DWORD) * dwNumAnims );
        if ( pkacKeyframed == NULL )
        {
            ErrorLog( KEM_ERROR409 );
            ASSERT( !"invalid KACKeyframedAnimationSet format" );
            return E_FAIL;
        }//if
        const char*   pszName = NULL;
        if ( pkacKeyframed->m_dwOffset_Name != NULL )
        {
            pszName = kInMemory_.RetrieveDupString( pkacKeyframed->m_dwOffset_Name );
            if ( pszName == NULL )
            {
                ErrorLog( KEM_ERROR410 );
                ASSERT( !"invalid name format" );
                return E_FAIL;
            }//if
        }//if

        CComPtr<ID3DXKeyframedAnimationSet> pdxKeyframedAnimSet;
        hr = D3DXCreateKeyframedAnimationSet( 
            pszName
            , pkacKeyframed->m_dTicksPerSecond
            , (D3DXPLAYBACK_TYPE) pkacKeyframed->m_dwPlaybackType
            , dwNumAnims
            , 0
            , NULL
            , &pdxKeyframedAnimSet );
        SAFE_DELETE_ARRAY( pszName );
        if ( FAILED( hr ) )
        {
            ErrorLog( KEM_ERROR411 );
            ASSERT( !"D3DXCreateKeyframedAnimationSet failed" );
            return hr;
        }//if
        
        DWORD dwOffsetAnim = dwInOffset_ + sizeof(KACKeyframedAnimationSet) + sizeof(DWORD) * dwNumAnims;
        DWORD dwSizeAnim;

#if	defined(X2OPTIMIZE_ANIMATION_LOSSY_COMPRESSION) //&& defined(X2OPTIMIZE_MASS_FILE_BUFFER_MANAGER)
		DWORD	dwMaxNumKeys = 1;
		DWORD	dwOffset = dwOffsetAnim;
		for( DWORD dwAnim = 0; dwAnim < dwNumAnims; dwAnim++, dwOffset += dwSizeAnim )
		{
            dwSizeAnim = pkacKeyframed->m_adwSize_Animations[ dwAnim ];
            const KACKeyframedAnimation* pkcsAnim = (const KACKeyframedAnimation*) kInMemory_.AccessData( dwOffset, dwSizeAnim );
            if ( pkcsAnim == NULL )
            {
                ErrorLog( KEM_ERROR412 );
                ASSERT( !"invalid KACKeyframedAnimation format" );
                return E_FAIL;
            }//
            if ( !( pkcsAnim->m_iIndex_Frame >= 0 && pkcsAnim->m_iIndex_Frame < (int) proxy_.GetFrameNum() ) )
            {
                ErrorLog( KEM_ERROR413 );
                ASSERT( !"invalid frame index" );
                return E_FAIL;
            }//if
			if ( pkcsAnim->m_dwNumKeys > dwMaxNumKeys )
				dwMaxNumKeys = pkcsAnim->m_dwNumKeys;
		}
		KGCMassFileBufferPtr spScales = KGCMassFileManager::GetMassFileBufMan().GetBuffer( dwMaxNumKeys * sizeof(D3DXKEY_VECTOR3) );
		KGCMassFileBufferPtr spTranslations = KGCMassFileManager::GetMassFileBufMan().GetBuffer( dwMaxNumKeys * sizeof(D3DXKEY_VECTOR3) );
		KGCMassFileBufferPtr spRotations = KGCMassFileManager::GetMassFileBufMan().GetBuffer( dwMaxNumKeys * sizeof(D3DXKEY_QUATERNION) );
		D3DXKEY_VECTOR3*	pScales = (D3DXKEY_VECTOR3*) spScales->GetBuffer();
		D3DXKEY_VECTOR3*	pTranslations = (D3DXKEY_VECTOR3*) spTranslations->GetBuffer();
		D3DXKEY_QUATERNION*	pRotations = (D3DXKEY_QUATERNION*) spRotations->GetBuffer();
#else
        std::vector<D3DXKEY_VECTOR3>      vecScales;
        std::vector<D3DXKEY_VECTOR3>      vecTranslations;
        std::vector<D3DXKEY_QUATERNION>   vecRotations;
#endif

        for( DWORD dwAnim = 0; dwAnim < dwNumAnims; dwAnim++, dwOffsetAnim += dwSizeAnim )
        {
            dwSizeAnim = pkacKeyframed->m_adwSize_Animations[ dwAnim ];
            const KACKeyframedAnimation* pkcsAnim = (const KACKeyframedAnimation*) kInMemory_.AccessData( dwOffsetAnim, dwSizeAnim );
            if ( pkcsAnim == NULL )
            {
                ErrorLog( KEM_ERROR412 );
                ASSERT( !"invalid KACKeyframedAnimation format" );
                return E_FAIL;
            }//
            if ( !( pkcsAnim->m_iIndex_Frame >= 0 && pkcsAnim->m_iIndex_Frame < (int) proxy_.GetFrameNum() ) )
            {
                ErrorLog( KEM_ERROR413 );
                ASSERT( !"invalid frame index" );
                return E_FAIL;
            }//if

			LastErrorLog( proxy_.GetDeviceID().c_str() ); // GetFrame() 함수 안에서 파일 이름을 사용하기 위해.
            ASSERT( proxy_.GetFrame( pkcsAnim->m_iIndex_Frame ) != NULL );

#if	defined(X2OPTIMIZE_ANIMATION_LOSSY_COMPRESSION) //&& defined(X2OPTIMIZE_MASS_FILE_BUFFER_MANAGER)
			D3DXKEY_VECTOR3*	pScalesStart = pScales;
			D3DXKEY_VECTOR3*	pTranslationsStart = pTranslations;
			D3DXKEY_QUATERNION*	pRotationsStart = pRotations;
#else
            vecScales.resize( pkcsAnim->m_dwNumKeys );
            vecTranslations.resize( pkcsAnim->m_dwNumKeys );
            vecRotations.resize( pkcsAnim->m_dwNumKeys );
			D3DXKEY_VECTOR3*	pScalesStart = &vecScales[0];
			D3DXKEY_VECTOR3*	pTranslationsStart = &vecTranslations[0];
			D3DXKEY_QUATERNION*	pRotationsStart = &vecRotations[0];
#endif

            DWORD   dwNumScales = 0;
            DWORD   dwNumTrans = 0;
            DWORD   dwNumRots = 0;

            hr = _UnpackKeys( pkcsAnim->m_dwNumKeys, pkcsAnim->m_abKeys, dwSizeAnim - sizeof(KACKeyframedAnimation)              
                , dwNumScales, pScalesStart, dwNumTrans, pTranslationsStart, dwNumRots, pRotationsStart );
            if ( FAILED( hr ) )
            {
                ErrorLog( KEM_ERROR413 );
                ASSERT( !"invalid KACKey format" );
                return E_FAIL;
            }//if

#ifdef	X2OPTIMIZE_ANIMATION_LOSSY_COMPRESSION
			D3DXKEY_VECTOR3*	pScalesEnd = pScalesStart + dwNumScales;
			D3DXKEY_VECTOR3*	pTranslationsEnd = pTranslationsStart + dwNumTrans;
			D3DXKEY_QUATERNION*	pRotationsEnd = pRotationsStart + dwNumRots;

			_OptimizeKeys( pScalesStart, pScalesEnd, pTranslationsStart, pTranslationsEnd, pRotationsStart, pRotationsEnd, KAC_LINEAR_COMPRESSION_TOLERANCE );
			dwNumScales = (DWORD)( pScalesEnd - pScalesStart );
			dwNumTrans = (DWORD)( pTranslationsEnd - pTranslationsStart );
			dwNumRots = (DWORD)( pRotationsEnd - pRotationsStart );
#endif	X2OPTIMIZE_ANIMATION_LOSSY_COMPRESSION

            DWORD dwIndex = 0;
            hr = pdxKeyframedAnimSet->RegisterAnimationSRTKeys( proxy_.GetFrame( pkcsAnim->m_iIndex_Frame )->Name
                , dwNumScales
                , dwNumRots
                , dwNumTrans
                , pScalesStart
				, pRotationsStart
                , pTranslationsStart
                , &dwIndex );
            if ( FAILED( hr ) )
            {
                ErrorLog( KEM_ERROR414 );
                ASSERT( !"ID3DXKeyframedAnimationSet::RegisterAnimationSRTKeys failed" );
                return E_FAIL;
            }//if

        }//for

        hr = pInAC_->RegisterAnimationSet( pdxKeyframedAnimSet );
        if ( FAILED( hr ) )
        {
            ErrorLog( KEM_ERROR415 );
            ASSERT( !"ID3DXAnimationController::RegisterAnimationSet failed" );
            return E_FAIL;
        }//if
        return S_OK;
    }
    else if ( kacAnimSet.m_dwType == EAC_COMPRESSED )
    {
        const KACCompressedAnimationSet* pkacCompressed = (const KACCompressedAnimationSet*) kInMemory_.AccessData( dwInOffset_, sizeof(KACCompressedAnimationSet) );
        if ( pkacCompressed == NULL )
        {
            ErrorLog( KEM_ERROR416 );
            ASSERT( !"invalid KACCompressedAnimationSet format" );
            return E_FAIL;
        }//if
        const void* pCompressedData = (const KACCompressedAnimationSet*) kInMemory_.AccessData( dwInOffset_ + sizeof(KACCompressedAnimationSet), pkacCompressed->m_dwCompressedDataSize );
        if ( pCompressedData == NULL )
        {
            ErrorLog( KEM_ERROR417 );
            ASSERT( !"invalid KACCompressedAnimationSet format" );
            return E_FAIL;
        }//if

        const char*   pszName = NULL;
        if ( pkacCompressed->m_dwOffset_Name != NULL )
        {
            pszName = kInMemory_.RetrieveDupString( pkacCompressed->m_dwOffset_Name );
            if ( pszName == NULL )
            {
                ErrorLog( KEM_ERROR418 );
                ASSERT( !"invalid name format" );
                return E_FAIL;
            }//if
        }//if
        CComPtr<ID3DXBuffer> pBuffer;
        hr = D3DXCreateBuffer( pkacCompressed->m_dwCompressedDataSize, &pBuffer );
        if ( FAILED( hr ) )
        {
            SAFE_DELETE_ARRAY( pszName );
            ErrorLog( KEM_ERROR419 );
            ASSERT( !"D3DXCreateBuffer failed" );
            return hr;
        }//if
        ASSERT( pBuffer != NULL );
        CopyMemory( pBuffer->GetBufferPointer(), pCompressedData, pkacCompressed->m_dwCompressedDataSize );

        CComPtr<ID3DXCompressedAnimationSet> pdxCompressedAnimSet;
        hr = D3DXCreateCompressedAnimationSet(
            pszName
            , pkacCompressed->m_dTicksPerSecond
            , (D3DXPLAYBACK_TYPE) pkacCompressed->m_dwPlaybackType
            , pBuffer
            , 0
            , NULL
            , &pdxCompressedAnimSet );
        SAFE_DELETE_ARRAY( pszName );
        if ( FAILED( hr ) )
        {
            ErrorLog( KEM_ERROR420 );
            ASSERT( !"D3DXCreateCompressedAnimationSet failed" );
            return hr;
        }//if

        hr = pInAC_->RegisterAnimationSet( pdxCompressedAnimSet );
        if ( FAILED( hr ) )
        {
            ErrorLog( KEM_ERROR421 );
            ASSERT( !"ID3DXAnimationController::RegisterAnimationSet failed" );
            return hr;
        }//if
        return S_OK;
    }//if.. else..

    ErrorLog( KEM_ERROR422 );
    ASSERT( !"unsupported animation set type" );
    return E_FAIL;
}//_ReadFromMemoryDeserializer()



static HRESULT _WriteToFileSerializer( KFileSerializer& kInOutFile_, const CKTDXDeviceXSkinMesh* pInSkinMesh_, ID3DXAnimationSet* pInAnimSet_ )
{
    ASSERT( pInAnimSet_ != NULL );
    ASSERT( pInSkinMesh_ != NULL );

    HRESULT hr;
    
    CComQIPtr<ID3DXKeyframedAnimationSet, &IID_ID3DXKeyframedAnimationSet>      pKeyframed( pInAnimSet_ );
    CComQIPtr<ID3DXCompressedAnimationSet, &IID_ID3DXCompressedAnimationSet>    pCompressed( pInAnimSet_ );
    if ( pKeyframed != NULL )
    {
        DWORD   dwNumAnimations = pKeyframed->GetNumAnimations();
        DWORD   dwStructSize = sizeof(KACKeyframedAnimationSet) + sizeof(DWORD) * dwNumAnimations;
        KACKeyframedAnimationSet* pkacKeyframe = (KACKeyframedAnimationSet*) malloc( dwStructSize );
        ASSERT( pkacKeyframe != NULL );
        DWORD   dwOffset_Keyframed = kInOutFile_.AppendSpace( dwStructSize );

        pkacKeyframe->m_dwOffset_Name = NULL;
        pkacKeyframe->m_dwType = EAC_KEYFRAMED;
        pkacKeyframe->m_dTicksPerSecond = pKeyframed->GetSourceTicksPerSecond();
        pkacKeyframe->m_dwPlaybackType = pKeyframed->GetPlaybackType();
        pkacKeyframe->m_dwNumAnimations = dwNumAnimations;

        std::vector<D3DXKEY_VECTOR3>    vecScales;
        std::vector<D3DXKEY_VECTOR3>    vecTranslations;
        std::vector<D3DXKEY_QUATERNION> vecRotations;
        std::vector<BYTE>               vecAnimation;
        float   fMinTime = 0;

        for( DWORD dwAnim = 0; dwAnim < dwNumAnimations; dwAnim++ )
        {
            LPCSTR  pszName = NULL;
            hr = pKeyframed->GetAnimationNameByIndex( dwAnim, &pszName );
            ASSERT( SUCCEEDED( hr ) );
            int     iFrameIndex = -1;
            if ( SUCCEEDED( hr ) )
            {
                CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame = pInSkinMesh_->FindFrame( pszName );
                ASSERT( pFrame != NULL );
                iFrameIndex = pFrame->m_iIndex;
                ASSERT( iFrameIndex >= 0 && iFrameIndex < (int) pInSkinMesh_->GetFrameNum() );
            }//if
            DWORD   dwNumKeysScale = pKeyframed->GetNumScaleKeys( dwAnim );
            DWORD   dwNumKeysTrans = pKeyframed->GetNumTranslationKeys( dwAnim );
            DWORD   dwNumKeysRot = pKeyframed->GetNumRotationKeys( dwAnim );

            ASSERT( !( dwNumKeysScale == 0 && dwNumKeysTrans == 0 && dwNumKeysRot == 0 ) );
            
            vecScales.resize( dwNumKeysScale );
            vecTranslations.resize( dwNumKeysTrans );
            vecRotations.resize( dwNumKeysRot );

            if ( dwNumKeysScale > 0 )
            {
                hr = pKeyframed->GetScaleKeys( dwAnim, &vecScales[0] );
                ASSERT( SUCCEEDED( hr ) );
            }//if
            if ( dwNumKeysTrans > 0 )
            {
                hr = pKeyframed->GetTranslationKeys( dwAnim, &vecTranslations[0] );
                ASSERT( SUCCEEDED( hr ) );
            }//if
            if ( dwNumKeysRot > 0 )
            {
                hr = pKeyframed->GetRotationKeys( dwAnim, &vecRotations[0] );
                ASSERT( SUCCEEDED( hr ) );
            }//if

            _OptimizeKeys( dwNumKeysScale,  &vecScales[0], dwNumKeysTrans, &vecTranslations[0], dwNumKeysRot, &vecRotations[0] );

            vecAnimation.resize( sizeof(KACKeyframedAnimation) + ( sizeof(KACKey) + sizeof(float) * 10 ) * ( dwNumKeysScale + dwNumKeysTrans + dwNumKeysRot ) );

            KACKeyframedAnimation* pkacAnim = (KACKeyframedAnimation*)( &vecAnimation[0] );
            ASSERT( pkacAnim != NULL );
            pkacAnim->m_iIndex_Frame = iFrameIndex;
            DWORD   dwNumKeys = 0;
            DWORD   dwPackSize = _PackKeys( dwNumKeys, pkacAnim->m_abKeys, dwNumKeysScale, &vecScales[0], dwNumKeysTrans, &vecTranslations[0], dwNumKeysRot, &vecRotations[0] );
            pkacAnim->m_dwNumKeys = dwNumKeys;
            pkacKeyframe->m_adwSize_Animations[ dwAnim ] = sizeof(KACKeyframedAnimation) + dwPackSize;

            DWORD dwOffset = kInOutFile_.AppendData( sizeof(KACKeyframedAnimation) + dwPackSize, pkacAnim );
            ASSERT( dwOffset != NULL );
        }//for

//#ifdef	X2OPTIMIZE_KFILESERIALIZER_MODIFIED
        pkacKeyframe->m_dwOffset_Name = kInOutFile_.AppendString( false, false, pKeyframed->GetName() );
//#else	X2OPTIMIZE_KFILESERIALIZER_MODIFIED
//		pkacKeyframe->m_dwOffset_Name = kInOutFile_.AppendString( pKeyframed->GetName() );
//#endif	X2OPTIMIZE_KFILESERIALIZER_MODIFIED
        hr = kInOutFile_.CopyData( dwOffset_Keyframed, dwStructSize, pkacKeyframe );
        ASSERT( SUCCEEDED( hr ) );
        
        SAFE_FREE( pkacKeyframe );
        return S_OK;
    }
    else if ( pCompressed != NULL )
    {
        CComPtr<ID3DXBuffer>    pCompressedData;
        hr = pCompressed->GetCompressedData( &pCompressedData );
        ASSERT( SUCCEEDED( hr ) && pCompressedData != NULL );
        DWORD   dwCompressedDataSize = pCompressedData->GetBufferSize();
        DWORD   dwStructSize = sizeof(KACCompressedAnimationSet) + dwCompressedDataSize;
        DWORD   dwOffset_Compressed = kInOutFile_.AppendSpace( dwStructSize );
//#ifdef	X2OPTIMIZE_KFILESERIALIZER_MODIFIED
		DWORD   dwOffset_Name = kInOutFile_.AppendString( false, false, pCompressed->GetName() );
//#else	X2OPTIMIZE_KFILESERIALIZER_MODIFIED
//        DWORD   dwOffset_Name = kInOutFile_.AppendString( pCompressed->GetName() );
//#endif	X2OPTIMIZE_KFILESERIALIZER_MODIFIED

        kInOutFile_.Lock();
//#ifdef	X2OPTIMIZE_KFILESERIALIZER_MODIFIED
        KACCompressedAnimationSet* pkacCompressed = (KACCompressedAnimationSet*) kInOutFile_.AccessData( 
            dwOffset_Compressed, dwStructSize );
//#else	#define X2OPTIMIZE_KFILESERIALIZER_MODIFIED
//        KACCompressedAnimationSet* pkacCompressed = (KACCompressedAnimationSet*) kInOutFile_.AccessSpace( 
//            dwOffset_Compressed, dwStructSize );
//#endif	#define X2OPTIMIZE_KFILESERIALIZER_MODIFIED
        pkacCompressed->m_dwOffset_Name = dwOffset_Name;
        pkacCompressed->m_dwType = EAC_COMPRESSED;
        pkacCompressed->m_dTicksPerSecond = pCompressed->GetSourceTicksPerSecond();
        pkacCompressed->m_dwPlaybackType = pCompressed->GetPlaybackType();
        pkacCompressed->m_dwCompressedDataSize = dwCompressedDataSize;
        CopyMemory( pkacCompressed->m_abCompressedData, pCompressedData->GetBufferPointer(), dwCompressedDataSize );
        kInOutFile_.Unlock();
        return S_OK;
    }//if

    ASSERT( !"KTDXAnimationController::WriteToFileSerializer : unsupported ID3DXAnimationSet type" );

    return E_FAIL;
}//_WriteToFileSerializer()


static HRESULT _Optimize( ID3DXAnimationSet* pAnimSet, ID3DXAnimationSet** ppOptimizedAnimSet )
{
    ASSERT( pAnimSet != NULL && ppOptimizedAnimSet != NULL );
    if ( pAnimSet == NULL || ppOptimizedAnimSet == NULL )
        return E_FAIL;

    HRESULT hr;
    
    CComQIPtr<ID3DXKeyframedAnimationSet, &IID_ID3DXKeyframedAnimationSet>  pKeyframed( pAnimSet );
    if ( pKeyframed != NULL )
    {
        DWORD   dwNumAnimations = pKeyframed->GetNumAnimations();

        std::vector<D3DXKEY_VECTOR3>    vecScales;
        std::vector<D3DXKEY_VECTOR3>    vecTranslations;
        std::vector<D3DXKEY_QUATERNION> vecRotations;

        CComPtr<ID3DXKeyframedAnimationSet> pdxKeyframedAnimSet;
        hr = D3DXCreateKeyframedAnimationSet( 
            pKeyframed->GetName()
            , pKeyframed->GetSourceTicksPerSecond()
            , pKeyframed->GetPlaybackType()
            , dwNumAnimations
            , 0
            , NULL
            , &pdxKeyframedAnimSet );
        ASSERT( SUCCEEDED( hr ) );
        if ( FAILED( hr ) )
            return hr;

        for( DWORD dwAnim = 0; dwAnim < dwNumAnimations; dwAnim++ )
        {
            LPCSTR  pszName = NULL;
            hr = pKeyframed->GetAnimationNameByIndex( dwAnim, &pszName );
            ASSERT( SUCCEEDED( hr ) );
            if ( FAILED( hr ) )
                continue;

            DWORD   dwNumKeysScale = pKeyframed->GetNumScaleKeys( dwAnim );
            DWORD   dwNumKeysTrans = pKeyframed->GetNumTranslationKeys( dwAnim );
            DWORD   dwNumKeysRot = pKeyframed->GetNumRotationKeys( dwAnim );

            ASSERT( !( dwNumKeysScale == 0 && dwNumKeysTrans == 0 && dwNumKeysRot == 0 ) );
            
            vecScales.resize( dwNumKeysScale );
            vecTranslations.resize( dwNumKeysTrans );
            vecRotations.resize( dwNumKeysRot );

            if ( dwNumKeysScale > 0 )
            {
                hr = pKeyframed->GetScaleKeys( dwAnim, &vecScales[0] );
                ASSERT( SUCCEEDED( hr ) );
            }//if
            if ( dwNumKeysTrans > 0 )
            {
                hr = pKeyframed->GetTranslationKeys( dwAnim, &vecTranslations[0] );
                ASSERT( SUCCEEDED( hr ) );
            }//if
            if ( dwNumKeysRot > 0 )
            {
                hr = pKeyframed->GetRotationKeys( dwAnim, &vecRotations[0] );
                ASSERT( SUCCEEDED( hr ) );
            }//if

            _OptimizeKeys( dwNumKeysScale, &vecScales[0], dwNumKeysTrans, &vecTranslations[0], dwNumKeysRot,  &vecRotations[0] );

            DWORD dwIndex = 0;
            hr = pdxKeyframedAnimSet->RegisterAnimationSRTKeys( pszName
                , dwNumKeysScale
                , dwNumKeysRot
                , dwNumKeysTrans
                , &vecScales[0]
                , &vecRotations[0]
                , &vecTranslations[0]
                , &dwIndex );
            ASSERT( SUCCEEDED( hr ) );
        }//for

        *ppOptimizedAnimSet = pdxKeyframedAnimSet;
        (*ppOptimizedAnimSet)->AddRef();
        return S_OK;
    }//if  

    return E_FAIL;
}//
