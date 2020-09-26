/// @file   ProtectedType.h
/// @author pch413@kogstudios.com
/// @since  2005-09-09 오전 12:03:51

// Ver1.0
// < Note by Park Chang-hyeon >
// 아래의 KProtectedType/KProtectedArray는 템플릿 인자에 대해서
// Built-in types(Primitive types), 즉 int/char/float... 등의
// 기본 타입에 대해서만 유효함을 보장한다.
// T 인자에 사용자 정의 자료형을 넣을 경우 내부에 pointer 변수 등이 있다면
// 객체 복사가 제대로 이루어지지 않거나 체크섬의 유효성이 깨진다.
// 그러므로 기본 타입에 대해서만 사용할 것을 권고한다.

// Ver1.1
// @modify 2007/3/7. iridology.
// 거의 사용하지 않는 ProtectedTypeArray, ProtectedTypeVector를 제거하고,
// 성능상의 문제가 있는 부분을 간단한 아이디어로 교체했음.
// ProtectedRefTable( new로 생성, size가 일정하지 않고, 내용도 일정하지 않은 테이블 )
// 을 만들고 table index, 계산후 값을 가짐으로써 데이터의 유효성을 체크한다.
// Primitive types만 사용하기를 권고하는 것은 변경되지 않았다.

// @comment 2007/3/8. iridology. 
// 특정 타입을 ProtectedType로 만들려면
// 클래스 템플릿 특수화(?)를 이용해서 만드세요.
// ( class KProtectedType<bool> 참조 )

// Ver1.2
// @comment 2007/4/6 iridology
// m_data와 verifyData에 같은 값을 더해서 프리즈 하면 뚫리는거 수정용.
// m_data를 >> 1 연산해서 계산
/*************************************************************************/
/*
                            New( >> )	    Old	        Primitive
1,000,000 	    A	            58 	        59 	        29 
                B	            13 	        13 	        9 
100,000 	    A	            6 	        6 	        3 
                B	            1 	        1 	        0 
                                                                단위 : ms

                                                                A : P3-600, 128Ram, Win98
                                                                B : P4-3.2 1G WinXp-sp2
                                                                * 모든 측정은 Release 모드를 기준으로 하였음.

/************************************************************************/

// Ver1.3
// @comment 2007/4/6 iridology
// KProtectedType<int> a = 50 이런류의 변수에서 발생하는
// m_data의 실제주소를 찾아내서 Freeze 시키면 뚫리는거 수정 버전
// m_data로 계산하는 것이 아니고 operator에 파라메터로 넘어오는
// rhs를 이용해서 계산
/************************************************************************/
/*
                            New( rhs )	    Old( >> )
1,000,000 	    A	            58 	        59 
                B	            13 	        13 
100,000 	    A	            6 	        6 
                B	            1 	        1 
                                                                단위 : ms

                                                                A : P3-600, 128Ram, Win98
                                                                B : P4-3.2 1G WinXp-sp2
                                                                * 모든 측정은 Release 모드를 기준으로 하였음.

/************************************************************************/

// Ver1.4
// @comment 2007/5/10. daeuk = iridology ^^;
// g_bDetectedHack -> g_bHackSafe로 바꿨음.. 빠른 이해를 위해
// |= 로 연산하던거를 &= 로 변경.
// 이유 : 한번 실패하면 그 다음이 성공하던 말던 실패하도록 하기 위해서임.

#ifndef _PROTECTEDTYPE_H_
#define _PROTECTEDTYPE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(push)
#pragma warning(disable:4786)

#include <sstream>
#include <vector>
#include <assert.h>
#include "KProtectRefTable.h"

using namespace ProtectTable;

// KProtectedType : 프리미티브 타입을 인자로 받는다.
// usage 1 : KProtectedType< int > obj;
// usage 2 : KProtectedType< float > obj;
template< class T >
class KProtectedType
{
public:
    // constructors
    KProtectedType( void )
    {
        m_data = T();
        ConstructorUpdate( m_data );
    }
    KProtectedType( const T& src )
    {
        m_data = src;
        ConstructorUpdate( src );
    }
    KProtectedType( const KProtectedType< T >& src )
    {
        m_data = src.m_data;
        m_verifyData = src.m_verifyData;
        m_ucTableIndex = src.m_ucTableIndex;
    }

    // destructor
    ~KProtectedType( void )
    {
        // empty
    }

    // type-casting operator
    operator T( void ) const
    {
        return m_data;
    }

    // assignment operators
    const KProtectedType< T >& operator = ( const T& rhs )
    {
        Update( rhs );
        return *this;
    }
    const KProtectedType< T >& operator = ( const KProtectedType< T >& rhs )
    {
        m_data = rhs.m_data;
        m_verifyData = rhs.m_verifyData;
        m_ucTableIndex = rhs.m_ucTableIndex;
        return *this;
    }
    const KProtectedType< T >& operator += ( const T& rhs )
    {
        Update( m_data + rhs );
        return *this;
    }
    const KProtectedType< T >& operator -= ( const T& rhs )
    {
        Update( m_data - rhs );
        return *this;
    }
    const KProtectedType< T >& operator *= ( const T& rhs )
    {
        Update( m_data * rhs );
        return *this;
    }
    const KProtectedType< T >& operator /= ( const T& rhs )
    {
        Update( m_data / rhs );
        return *this;
    }
    const KProtectedType< T >& operator %= ( const T& rhs )
    {
        Update( m_data % rhs );
        return *this;
    }
    const KProtectedType< T >& operator <<= ( const T& rhs )
    {
        Update( m_data << rhs );
        return *this;
    }
    const KProtectedType< T >& operator >>= ( const T& rhs )
    {
        Update( m_data >> rhs );
        return *this;
    }

    // prefix increment operator
    const KProtectedType< T >& operator ++ ( void )
    {
        Update( m_data + 1 );
        return *this;
    }
    // postfix increment operator
    const KProtectedType< T >& operator ++ ( int )
    {
        Update( m_data + 1 );
        return *this;
    }
    // prefix decrement operator
    const KProtectedType< T >& operator -- ( void )
    {
        Update( m_data - 1 );
        return *this;
    }
    // postfix decrement operator
    const KProtectedType< T >& operator -- ( int )
    {
        Update( m_data - 1 );
        return *this;
    }

public:
    // verify data
    inline bool Verify( void ) const
    {
#ifdef _DEBUG
        ProtectDebugHelper::g_dwVerifyCall++;
#endif
        bool bRet = ( m_verifyData == static_cast<T>( (m_data << 1) - g_kProtectTable[ m_ucTableIndex ] ) );
        ProtectTableHelper::g_bHackSafe &= bRet;

        assert( bRet != false );

        return bRet;
    }

    // convert informations to a string
    std::wstring ToString( void ) const
    {
        std::wostringstream strmStr;

        strmStr << "value : " << m_data
            << ", verify value : " << m_verifyData
            << ", ref value : " << g_kProtectTable[ m_ucTableIndex ] << "\n";

            return strmStr.str();
    }

private:
    // update
    inline void Update( T rhs )
    {
        ProtectTableHelper::g_bHackSafe &= Verify();

        m_ucTableIndex = rand() % g_iTableSize;
        m_verifyData = static_cast<T>( (rhs << 1) - g_kProtectTable[ m_ucTableIndex ] );

#ifdef _DEBUG
        ProtectDebugHelper::g_dwUpdateCall++;
#endif

        m_data = rhs;

    }

    // for constructor
    inline void ConstructorUpdate( T rhs )
    {
        m_ucTableIndex = rand() % g_iTableSize;
        m_verifyData = static_cast<T>( (rhs << 1) - g_kProtectTable[ m_ucTableIndex ] );

#ifdef _DEBUG
        ProtectDebugHelper::g_dwUpdateCall++;
#endif

        m_data = rhs;

    }

private:
    T       m_data;             // data field
    T       m_verifyData;       // for Verify
    UCHAR   m_ucTableIndex;     // index of ref table
};

template<>
class KProtectedType<bool>
{
public:
    // constructors
    KProtectedType( void )
    {
        m_data = bool();
        ConstructorUpdate( m_data );
    }
    KProtectedType( const bool& src )
    {
        m_data = src;
        ConstructorUpdate( m_data );
    }
    KProtectedType( const KProtectedType< bool >& src )
    {
        m_data = src.m_data;
        m_verifyData = src.m_verifyData;
    }

    // destructor
    ~KProtectedType( void )
    {
        // empty
    }

    // type-casting operator
    operator bool( void ) const
    {
        return m_data;
    }

    // assignment operators
    const KProtectedType< bool >& operator = ( const bool& rhs )
    {
        Update( rhs );
        return *this;
    }
    const KProtectedType< bool >& operator = ( const KProtectedType< bool >& rhs )
    {
        m_data = rhs.m_data;
        m_verifyData = rhs.m_verifyData;
        return *this;
    }

public:
    // verify data
    inline bool Verify( void ) const
    {
#ifdef _DEBUG
        ProtectDebugHelper::g_dwVerifyCall++;
#endif

        bool bRet = ( m_verifyData == (m_data ^ 0) );
        ProtectTableHelper::g_bHackSafe &= bRet;

        assert( bRet != false );

        return bRet;
    }

    // convert informations to a string
    std::wstring ToString( void ) const
    {
        std::wostringstream strmStr;

        strmStr << "value : " << m_data
            << ", verify value : " << m_verifyData << "\n";

        return strmStr.str();
    }

private:
    // update
    inline void Update( bool rhs )
    {
        ProtectTableHelper::g_bHackSafe &= Verify();

        m_verifyData = rhs ^ 0;
#ifdef _DEBUG
        ProtectDebugHelper::g_dwUpdateCall++;
#endif

        m_data = rhs;
    }
    // for constructor
    inline void ConstructorUpdate( bool rhs )
    {
        m_verifyData = rhs ^ 0;

#ifdef _DEBUG
        ProtectDebugHelper::g_dwUpdateCall++;
#endif

        m_data = rhs;
    }

private:
    bool    m_data;             // data field
    bool    m_verifyData;       // for Verify
};

template<>
class KProtectedType< float >
{
public:
    // constructors
    KProtectedType( void )
    {
        m_data = float();
        ConstructorUpdate( m_data );
    }
    KProtectedType( const float& src )
    {
        m_data = src;
        ConstructorUpdate( src );
    }
    KProtectedType( const KProtectedType< float >& src )
    {
        m_data = src.m_data;
        m_verifyData = src.m_verifyData;
        m_ucTableIndex = src.m_ucTableIndex;
    }

    // destructor
    ~KProtectedType( void )
    {
        // empty
    }

    // type-casting operator
    operator float( void ) const
    {
        return m_data;
    }

    // assignment operators
    const KProtectedType< float >& operator = ( const float& rhs )
    {
        Update( rhs );
        return *this;
    }
    const KProtectedType< float >& operator = ( const KProtectedType< float >& rhs )
    {
        m_data = rhs.m_data;
        m_verifyData = rhs.m_verifyData;
        m_ucTableIndex = rhs.m_ucTableIndex;
        return *this;
    }
    const KProtectedType< float >& operator += ( const float& rhs )
    {
        Update( m_data + rhs );
        return *this;
    }
    const KProtectedType< float >& operator -= ( const float& rhs )
    {
        Update( m_data - rhs );
        return *this;
    }
    const KProtectedType< float >& operator *= ( const float& rhs )
    {
        Update( m_data * rhs );
        return *this;
    }
    const KProtectedType< float >& operator /= ( const float& rhs )
    {
        Update( m_data / rhs );
        return *this;
    }

    // prefix increment operator
    const KProtectedType< float >& operator ++ ( void )
    {
        Update( m_data + 1 );
        return *this;
    }
    // postfix increment operator
    const KProtectedType< float >& operator ++ ( int )
    {
        Update( m_data + 1 );
        return *this;
    }
    // prefix decrement operator
    const KProtectedType< float >& operator -- ( void )
    {
        Update( m_data - 1 );
        return *this;
    }
    // postfix decrement operator
    const KProtectedType< float >& operator -- ( int )
    {
        Update( m_data - 1 );
        return *this;
    }

public:
    // verify data
    inline bool Verify( void ) const
    {
#ifdef _DEBUG
        ProtectDebugHelper::g_dwVerifyCall++;
#endif

        float fTempData = m_data;
        int* pInt;
        pInt = (int*)( &fTempData );
        (*pInt) = ((*pInt) << 1) - g_kProtectTable[ m_ucTableIndex ];
        bool bRet = ( m_verifyData == (*pInt) );
        ProtectTableHelper::g_bHackSafe &= bRet;

        assert( bRet != false );

        return bRet;
    }

    // convert informations to a string
    std::wstring ToString( void ) const
    {
        std::wostringstream strmStr;

        strmStr << "value : " << m_data
            << ", verify value : " << m_verifyData
            << ", ref value : " << g_kProtectTable[ m_ucTableIndex ] << "\n";

            return strmStr.str();
    }

private:
    // update
    inline void Update( float rhs )
    {
        ProtectTableHelper::g_bHackSafe &= Verify();

        m_ucTableIndex = rand() % g_iTableSize;

        float fTempData = rhs;
        int* pInt;
        pInt = (int*)( &fTempData );
        (*pInt) = ((*pInt) << 1) - g_kProtectTable[ m_ucTableIndex ];
        m_verifyData = (*pInt);

#ifdef _DEBUG
        ProtectDebugHelper::g_dwUpdateCall++;
#endif

        m_data = rhs;

    }

    // for constructor
    inline void ConstructorUpdate( float rhs )
    {

        m_ucTableIndex = rand() % g_iTableSize;

        float fTempData = rhs;
        int* pInt;
        pInt = (int*)( &fTempData );
        (*pInt) = ((*pInt) << 1) - g_kProtectTable[ m_ucTableIndex ];
        m_verifyData = (*pInt);

#ifdef _DEBUG
        ProtectDebugHelper::g_dwUpdateCall++;
#endif

        m_data = rhs;

    }

private:
    float   m_data;             // data field
    int     m_verifyData;       // for Verify
    UCHAR   m_ucTableIndex;     // index of ref table
};


struct PROTECT_VECTOR2
{
	KProtectedType<float>		m_fX;
	KProtectedType<float>		m_fY;

	PROTECT_VECTOR2()
	{
		m_fX = 0.0f;
		m_fY = 0.0f;
	}
	bool Verify()
	{
		if( m_fX.Verify() == false
			|| m_fY.Verify() == false )
		{
			return false;
		}
		return true;
	}
};

struct PROTECT_VECTOR3
{
	KProtectedType<float>		x;
	KProtectedType<float>		y;
	KProtectedType<float>		z;

	PROTECT_VECTOR3()
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}

#ifdef VERIFY_STAT_BY_BUFF
	PROTECT_VECTOR3( const float fX_, const float fY_, const float fZ_ )
		: x( fX_ ), y( fY_ ), z( fZ_ )
	{}

	PROTECT_VECTOR3( const PROTECT_VECTOR3& protectVec3_ )
		: x( protectVec3_.x ), y( protectVec3_.y ), z( protectVec3_.z )
	{}

	PROTECT_VECTOR3( const D3DXVECTOR3& vec3_ )
		: x( vec3_.x ), y( vec3_.y ), z( vec3_.z )
	{}

	bool operator!=( const D3DXVECTOR3& vec3_ ) const
	{
		if ( vec3_.x != x && vec3_.y != y && vec3_.z != z )
			return true;	/// 같지 않음
		else
			return false;	/// 같음
	}

	void operator+=( const PROTECT_VECTOR3& rhs_ )
	{
		x += rhs_.x;
		y += rhs_.y;
		z += rhs_.z;
	}

	friend static PROTECT_VECTOR3 operator-( const PROTECT_VECTOR3& lhs_, const D3DXVECTOR3& rhs_ )
	{
		return PROTECT_VECTOR3( lhs_.x - rhs_.x, lhs_.y - rhs_.y, lhs_.z - rhs_.z );
	}

	friend static PROTECT_VECTOR3 operator-( const PROTECT_VECTOR3& lhs_, const PROTECT_VECTOR3& rhs_ )
	{
		return PROTECT_VECTOR3( lhs_.x - rhs_.x, lhs_.y - rhs_.y, lhs_.z - rhs_.z );
	}

	friend static PROTECT_VECTOR3 operator*( const PROTECT_VECTOR3& lhs_, const float rhs_ )
	{
		return PROTECT_VECTOR3( lhs_.x * rhs_, lhs_.y * rhs_, lhs_.z * rhs_ );
	}

	friend static PROTECT_VECTOR3 operator/( const PROTECT_VECTOR3& lhs_, const float rhs_ )
	{
		return PROTECT_VECTOR3( lhs_.x / rhs_, lhs_.y / rhs_, lhs_.z / rhs_ );
	}

	D3DXVECTOR3 GetVector3() const { return D3DXVECTOR3( x, y, z ); }
#endif // VERIFY_STAT_BY_BUFF

	
	bool Verify()
	{
		if( x.Verify() == false
			|| y.Verify() == false
			|| z.Verify() == false )
		{
			return false;
		}
		return true;
	}
};


#pragma warning(pop)
#endif //end of _PROTECTEDTYPE_H_

// Performance

// Ver1.2
/*************************************************************************/
/*
New( >> )	    Old	        Primitive
1,000,000 	    A	            58 	        59 	        29 
B	            13 	        13 	        9 
100,000 	    A	            6 	        6 	        3 
B	            1 	        1 	        0 
단위 : ms

A : P3-600, 128Ram, Win98
B : P4-3.2 1G WinXp-sp2
* 모든 측정은 Release 모드를 기준으로 하였음.

/************************************************************************/

// Ver1.3
/************************************************************************/
/*
New( rhs )	    Old( >> )
1,000,000 	    A	            58 	        59 
B	            13 	        13 
100,000 	    A	            6 	        6 
B	            1 	        1 
단위 : ms

A : P3-600, 128Ram, Win98
B : P4-3.2 1G WinXp-sp2
* 모든 측정은 Release 모드를 기준으로 하였음.

/************************************************************************/
