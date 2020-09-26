/** @file   KHandle.h
    @desc   interface of KHandle
            KHandleArray<>에서 사용하는 노드의 재사용 여부를 판단하기 위해 magic number를 포함하는
            핸들값을 저장하기 위해 처음 구현하다.
    @author jintaeks@kogstudios.com
    @since  2008-3-31, 16:13
*/

#ifndef _KHandle_h
#define _KHandle_h

#ifdef  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
#include    <lua_tinker.h>
#endif  X2OPTIMIZE_HANDLE_VALIDITY_CHECK

//#include "KJohnGen.h"

///-----------------------------------------------------------------------------
/// @class	KHandle
/// @brief  magic number(일반적으로 reference counter)를 포함하는 핸들을 저장하기
///         위한 일반 목적의 handle wrapper이다.
///         KHandleArray<>가 리턴하는 노드에 대해, handle은 유효하지만 다른 곳에서
///         생성된 잘못된 노드를 판단하기 위해 처음으로 사용한다.
/// @date   jintaeks on 2008-3-31, 16:13
///
struct KHandle
{
public:
    enum KHANDLE_VALUES
    {
        KHandleType_Invalid_HANDLE  = -1,
    };//enum KHANDLE_VALUES

private:
    int     m_iHandle;  ///< handle to something(node or etc.)
    UINT    m_uiMagic;  ///< magic number(reference counter and etc.)

public:
    /// constructor.
    KHandle() : m_iHandle(-1), m_uiMagic(0) {}

    KHandle(int iHandle_, UINT uiMagic_)
    {
        m_iHandle = iHandle_;
        m_uiMagic = uiMagic_;
    }//KHandle()

    /// copy constructor.
    KHandle( const KHandle& rhs_ )
    {
        m_iHandle = rhs_.m_iHandle;
        m_uiMagic = rhs_.m_uiMagic;
    }//KHandle()

    void Invalidate()
    {
        m_iHandle = -1;
        m_uiMagic = 0;
    }//Invalidate()

    bool IsValid() const
    {
        return m_iHandle >= 0 && m_uiMagic > 0;
    }//IsValid()

    /// handle number part를 얻는다.
    int GetHandlePart() const { return m_iHandle; }

    /// magic number part를 얻는다.
    UINT GetMagicPart() const { return m_uiMagic; }

    /// handle number part를 설정한다.
    void SetHandlePart(int iHandle_) { m_iHandle = iHandle_; }

    /// magic number part를 설정한다.
    void SetMagicPart(UINT uiMagic_) { m_uiMagic = uiMagic_; }

    /// handle과 magic number part를 동시에 설정한다.
    void SetHandleAndMagicPart(int iHandle_, UINT uiMagic_) { m_iHandle = iHandle_; m_uiMagic = uiMagic_; }

    /** int와 호화되는 handle처럼 사용하기 위해, operator int()에 대해서
        handle part를 리턴한다.
        하지만, operator int&()는 제공하지 않는데 이것은 잘못된 사용으로 magic number를 유지하면서
        handle을 변경시키는 것을 막기 위함이다.
        - jintaeks on 2008-04-01, 10:06 */
    //operator int() const
    //{
    //    return m_iHandle;
    //}//operator int()

    /// handle part와 magic number part가 모두 같은지 검사한다.
    bool operator==(const KHandle& rhs_) const
    {
        return m_iHandle == rhs_.m_iHandle && m_uiMagic == rhs_.m_uiMagic;
    }//operator==()

    bool operator!=(const KHandle& rhs_) const
    {
        return m_iHandle != rhs_.m_iHandle || m_uiMagic != rhs_.m_uiMagic;
    }//operator==()

    //bool operator!=(const int& rhs_) const
    //{
    //    return m_iHandle != rhs_;
    //}//operator==()

    /// KHandle이 container의 node로 사용된 경우를 위해서 만든 함수이다.
    /// @note   stl container의 equivalence를 만족시키기 위해서 아래의 구현처럼
    ///         먼저 핸들에 대해서 비교하고, 별도로 magic number를 비교해야 함을
    ///         주의하자.
    bool operator<(const KHandle& rhs_) const
    {
        /** 동등성(equivalence)을 만족시키기 위해서는 반드시 별도의 if문으로 구현되어야 한다.
            - jintaeks on 2008-03-31, 16:21 */
        if ( m_iHandle != rhs_.m_iHandle )
            return m_iHandle < rhs_.m_iHandle;

        return m_uiMagic < rhs_.m_uiMagic;
    }//operator<()

    //int& operator=(int rhs_)
    //{
    //    if( rhs_ == NULL )
    //    {
    //        Invalidate();
    //    }
    //    else
    //    {
    //        //m_uiMagic = 0;
    //        m_iHandle = rhs_;
    //    }//if.. else..

    //    return m_iHandle;
    //}//operator=()

    //inline friend bool operator==( int iHandle_, const KHandle& handle_ )
    //{
    //    return iHandle_ == handle_.m_iHandle;
    //}//operator==()

    //inline friend bool operator!=( int iHandle_, const KHandle& handle_ )
    //{
    //    return iHandle_ != handle_.m_iHandle;
    //}//operator!=()

};//class KHandle




template<typename TYPE, typename TAG>
class   KHandleType;

template<typename TYPE, typename TAG>
struct  KHandleType_Invalid
{
    typedef TYPE        value_type;
    static const TYPE   value = TYPE(-1);

    bool                operator == ( const KHandleType<TYPE,TAG>& rhs_ ) const;
    bool                operator != ( const KHandleType<TYPE,TAG>& rhs_ ) const;
};

template<typename TYPE, typename TAG>
class   KHandleType
{
public:

    typedef TYPE        value_type;
    typedef KHandleType_Invalid<TYPE,TAG>
                        invalid_handle;

    explicit            KHandleType(const TYPE& value )
                            : m_value( value )                      {}
    /*CONSTRUCTOR*/     KHandleType()
                            : m_value( invalid_handle::value )                   {}
    /*CONSTRUCTOR*/     KHandleType( invalid_handle )
                            : m_value( invalid_handle::value )                   {}
    /*CONSTRUCTOR*/     KHandleType( const KHandleType& rhs_ ) 
                            : m_value( rhs_.m_value )               {}

    KHandleType&        operator = ( const KHandleType& rhs_ )          { m_value = rhs_.m_value; return *this; }
    KHandleType&        operator = ( invalid_handle )              { m_value = invalid_handle::value; return *this; }
    const TYPE&         GetValue() const                            { return m_value; }
    void                SetValue( const TYPE& value )               { m_value = value; }
    bool                IsValid() const                             { return m_value != invalid_handle::value; }
    bool                operator == ( const KHandleType& rhs_ ) const   { return m_value == rhs_.m_value; }
    bool                operator != ( const KHandleType& rhs_ ) const   { return m_value != rhs_.m_value; }
    bool                operator == ( invalid_handle ) const       { return m_value == invalid_handle::value; }
    bool                operator != ( invalid_handle ) const       { return m_value != invalid_handle::value; }

    // map 등에 넣을 수 있도록
    bool                operator < ( const KHandleType& rhs_ ) const   { return m_value < rhs_.m_value; }
    bool                operator <= ( const KHandleType& rhs_ ) const   { return m_value <= rhs_.m_value; }
    bool                operator > ( const KHandleType& rhs_ ) const   { return m_value > rhs_.m_value; }
    bool                operator >= ( const KHandleType& rhs_ ) const   { return m_value >= rhs_.m_value; }

    // lua 에 export 하는 용도로 사용
    bool                IsEqual( const KHandleType& rhs_ ) const { return this->operator == (rhs_); }

private:

    TYPE                m_value;
};

template<typename TYPE, typename TAG>
inline bool             KHandleType_Invalid<TYPE,TAG>::operator == ( const KHandleType<TYPE,TAG>& rhs_ ) const   
{ 
    return rhs_.operator == ( *this ); 
}
template<typename TYPE, typename TAG>
inline bool            KHandleType_Invalid<TYPE,TAG>:: operator != ( const KHandleType<TYPE,TAG>& rhs_ ) const   
{ 
    return rhs_.operator != ( *this ); 
}


namespace   lua_tinker
{
    template<typename TYPE, typename TAG>
	void push(lua_State *L, KHandleType<TYPE,TAG> ret)					
    { 
        if ( ret.IsValid() == true )
            type2lua<TYPE>(L, ret.GetValue()); 
        else
            lua_pushnil(L);		
    }

    template<typename TYPE, typename TAG>
	void push(lua_State *L, KHandleType_Invalid<TYPE,TAG> ret)					
    { 
        lua_pushnil(L);		
    }
}



#endif // _KHandle_h


/** @example    KHandle

  1) KGOCar의 멤버 변수로 KHandle을 선언한다:

    KHandle         m_khSparkParticleSystem1;


  2) 생성자에서 초기화한다.

    m_khSparkParticleSystem1.SetHandleAndMagicPart( -1, 0 );


  3) frame move에서 핸들이 무효한지 적절하게 판단한다:

    void KGOCar::__UpdateParticleSystemHandle(KHandle& khInOutSystem_)
    {
        // handle이 valid한 range에 있는 경우, node의 magic number가 다르면
        // 파괴된 노드이므로 새로 생성해야 한다.
        if ( g_kParticleSystemSuite.GetParticleSystemMan().IsValidHandleRange( khInOutSystem_ ) )
        {
            if ( khInOutSystem_.GetMagicPart() !=
                g_kParticleSystemSuite.GetParticleSystemMan().GetMagicNumber( khInOutSystem_ ) )
            {
                khInOutSystem_.SetHandlePart( -1 );
            }//if
        }
        // handle이 valid한 range가 아니면, particle system을 새로 생성해야 한다.
        else
        {
            khInOutSystem_.SetHandlePart( -1 );
        }//if.. else..
    }//KGOCar::__UpdateParticleSystemHandle()


  4) frame move에서 target 객체를 적절하게 생성한다.

    KParticleSystemEx* KGOCar::__CreateParticleSystem(KHandle& khInOutSystem_, const char* pszTemplate_)
    {
        // particle system을 생성하고, handle을 얻는다.
        khInOutSystem_.SetHandlePart( g_kParticleSystemSuite.CreateParticleSystem( pszTemplate_ ) );
        ASSERT( khInOutSystem_ >= 0 );
        // 새로 얻은 노드의 magic number를 저장해 둔다.
        khInOutSystem_.SetMagicPart( 
            g_kParticleSystemSuite.GetParticleSystemMan().GetMagicNumber(khInOutSystem_) );
        return g_kParticleSystemSuite.GetParticleSystemEx( khInOutSystem_ );
    }//KGOCar::__CreateParticleSystem()

*/
