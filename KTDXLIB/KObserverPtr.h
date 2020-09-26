/** @file   KObserverPtr.h
    @desc   interface of KObserverPtr
    @author robobeg@kogstudios.com
*/
#pragma once
#ifndef _KOBSERVERPTR_H
#define _KOBSERVERPTR_H



#define ASSERT(exp)         _ASSERTE(exp)

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)			{ if(p) { delete (p);     (p)=NULL; } }
#endif   

template<typename T>class KObservable;
template<typename T>class KObserverPtr;

class   KObserverHandle;

class   KObservableBase
{
public:

    KObservableBase() : m_pkObserverHandle( NULL )   
                        {
                            _CustomInitialize();
                        }//KObservable()

    virtual  ~KObservableBase()
                        {
                            _CustomFinalize();
                        }//~KObservable()

    KObserverHandle*    _GetObserverHandle() { return m_pkObserverHandle; }

protected:

    void                _CustomInitialize();
    void                _CustomFinalize();


private:

    KObserverHandle*    m_pkObserverHandle;

};




class KObserverHandle : private boost::noncopyable
{
public:

    KObservableBase*    GetObservable()  { return m_pkObservable; }

    const KObservableBase*
                        GetObservable() const  { return m_pkObservable; }

    class KObservableAccess
    {
        friend class    KObservableBase;

        static KObserverHandle*   
                        Constructor( KObservableBase& refkInObservable_ )
                        {
                            return new KObserverHandle( refkInObservable_ );
                        }//Constructor()

        static void     Invalidate(  KObserverHandle& refkInHandle_ )
                        {
                            refkInHandle_.m_pkObservable = NULL;
                            refkInHandle_.Release();
                        }//Constructor()

    };//class KObservableAccess


    class KObserverPtrAccess
    {
        template<typename T>
        friend class    KObserverPtr;

        static void     AddRef(  KObserverHandle& refkInHandle_ )
                        {
                            refkInHandle_.AddRef();
                        }//

        static void     Release(  KObserverHandle& refkInHandle_ )
                        {
                            refkInHandle_.Release();
                        }//

    };//class KObserverPtrAccess

private:

    KObserverHandle( KObservableBase& refkInObservable_ )
                            : m_pkObservable( &refkInObservable_ ), m_uRefCount(1) {}

    ~KObserverHandle() {}

    void                AddRef() 
                        {
                            m_uRefCount++;
                        }//AddRef()

    void                Release() 
                        {
                            ASSERT( m_uRefCount > 0 );
                            if( --m_uRefCount == 0 )
                            {
                                KObserverHandle* pThis = this;
                                SAFE_DELETE( pThis );
                                return;
                            }//if   
                        }//Release()

private:

    KObservableBase*    m_pkObservable;

    unsigned            m_uRefCount;

};//class KObserverHandle


inline void    KObservableBase::_CustomInitialize()
{
    if ( m_pkObserverHandle == NULL )
    {
        m_pkObserverHandle = KObserverHandle::KObservableAccess::Constructor( *this );
        ASSERT( m_pkObserverHandle != NULL );
    }//if
}//

inline void    KObservableBase::_CustomFinalize()
{
    if ( m_pkObserverHandle != NULL )
    {
        KObserverHandle::KObservableAccess::Invalidate( *m_pkObserverHandle );
        m_pkObserverHandle = NULL;
    }//if
}//


template<typename T>
class KObservable : public KObservableBase, private boost::noncopyable
{
public:

    class KObserverPtrAccess
    {
        template<typename U>
        friend class    KObserverPtr;

        template<typename U>
        static KObserverHandle*
                        GetSmartHandle( U* pkInObservable_ )
                        {
                            BOOST_STATIC_ASSERT( ( boost::is_base_of< T, U >::value == true ) );
                            BOOST_STATIC_ASSERT( ( boost::is_base_of< KObservableBase, U >::value == true ) );
                            if ( pkInObservable_ != NULL )
                            {
                                ASSERT( pkInObservable_->_GetObserverHandle() != NULL );
                                return  pkInObservable_->_GetObserverHandle();
                            }
                            return NULL;
                        }

    };//class KObserverPtrAccess

};//KObservable()


template<typename T>
class KObserverPtr
{
private:

    typedef KObserverPtr<T>
                        this_type;

public:

    typedef T           value_type;

                        KObserverPtr() : m_pkObserverHandle( NULL ), m_bInitiallyValid( false )
                        {
                        }

    template<typename U>
    explicit KObserverPtr( U* pkInObservable_ ) 
                        : m_pkObserverHandle( KObservable<T>::KObserverPtrAccess::GetSmartHandle( pkInObservable_ ) )
                        , m_bInitiallyValid( false )
                        {
                            if ( m_pkObserverHandle != NULL )
                            {
                                KObserverHandle::KObserverPtrAccess::AddRef( *m_pkObserverHandle );
                                if ( m_pkObserverHandle->GetObservable() != NULL )
                                {
                                    m_bInitiallyValid = true;
                                }//if
                            }//if
                        }

    template<typename U>
    explicit KObserverPtr( const KObserverPtr<U>& refkIn_ )
                        : m_pkObserverHandle( KObservable<T>::KObserverPtrAccess::GetSmartHandle( refkIn_.GetObservable() ) )
                        , m_bInitiallyValid( false )
                        {
                            if ( m_pkObserverHandle != NULL )
                            {
                                KObserverHandle::KObserverPtrAccess::AddRef( *m_pkObserverHandle );
                                if ( m_pkObserverHandle->GetObservable() != NULL )
                                {
                                    m_bInitiallyValid = true;
                                }//if
                            }//if
                        }//KObserverPtr()

    KObserverPtr( const KObserverPtr& refkIn_ ) 
                        : m_pkObserverHandle( refkIn_.m_pkObserverHandle )
                        , m_bInitiallyValid( false )
                        {
                            if ( m_pkObserverHandle != NULL )
                            {
                                KObserverHandle::KObserverPtrAccess::AddRef( *m_pkObserverHandle );
                                if ( m_pkObserverHandle->GetObservable() != NULL )
                                {
                                    m_bInitiallyValid = true;
                                }//if
                            }//if
                        }

    ~KObserverPtr()
                        {
                            if( m_pkObserverHandle != NULL )
                            {
                                KObserverHandle* pkOldHandle = m_pkObserverHandle;
                                m_pkObserverHandle = NULL;
                                KObserverHandle::KObserverPtrAccess::Release( *pkOldHandle );
                            }//if
                        }//~KObserverPtr()

    bool                IsInitiallyValid() const { return m_bInitiallyValid; }

    T*                  operator -> () const
                        {
                            T* pkObservable = _GetObservable();
                            ASSERT( pkObservable != NULL );
                            return pkObservable;
                        }//operator -> ()

    T&                  operator * () const
                        {
                            T* pkObservable = _GetObservable();
                            ASSERT( pkObservable != NULL );
                            return *pkObservable;
                        }//operator * ()

    T*                  GetObservable() const 
                        {
                            return _GetObservable();
                        }//GetObservable()

    KObserverHandle*    _GetObserverHandle() const { return m_pkObserverHandle; }       

    void                Swap(  KObserverPtr& refkIn_ ) 
                        {
                            KObserverHandle* pkTemp = m_pkObserverHandle;
                            bool            bInitiallyValid = m_bInitiallyValid;
                            m_pkObserverHandle = refkIn_.m_pkObserverHandle;
                            m_bInitiallyValid = refkIn_.m_bInitiallyValid;

                            refkIn_.m_pkObserverHandle = pkTemp;
                            refkIn_.m_bInitiallyValid = bInitiallyValid;
                        }//Swap()

    void                Reset()
                        {
                            if( m_pkObserverHandle != NULL )
                            {
                                KObserverHandle* pkOldHandle = m_pkObserverHandle;
                                m_pkObserverHandle = NULL;
                                m_bInitiallyValid = false;
                                KObserverHandle::KObserverPtrAccess::Release( *pkOldHandle );
                            }//if
                        }//Reset()

    typedef T* (this_type::*unspecified_bool_type)() const;

    /*unspecified_bool_type*/
                        operator unspecified_bool_type() const
                        {
                            return _GetObservable() == NULL ? NULL : &this_type::_GetObservable;
                        }//

    bool                operator ! () const
                        {
                            return _GetObservable() == NULL ? true : false;
                        }//

    template<typename U>
    KObserverPtr&       operator=( U* pkInObservable_ )
                        {
                            KObserverHandle*   pkOldHandle =  m_pkObserverHandle;
                            if( pkInObservable_ != NULL )
                            {
                                m_pkObserverHandle = KObservable<T>::KObserverPtrAccess::GetSmartHandle( pkInObservable_ );
                                ASSERT( m_pkObserverHandle != NULL );
                                KObserverHandle::KObserverPtrAccess::AddRef( *m_pkObserverHandle );
                                m_bInitiallyValid = ( m_pkObserverHandle->GetObservable() != NULL );
                            }
                            else
                            {
                                m_pkObserverHandle = NULL;
                                m_bInitiallyValid = false;
                            }//if.. else..
                            if( pkOldHandle != NULL )
                            {
                                KObserverHandle::KObserverPtrAccess::Release( *pkOldHandle );
                            }//if
                            return *this;
                        }//operator=()

    template<typename U>
    KObserverPtr&       operator=( const KObserverPtr<U>& refkIn_ ) 
                        {
                            KObserverHandle*   pkOldHandle =  m_pkObserverHandle;
                            if( refkIn_._GetObserverHandle() != NULL )
                            {
                                m_pkObserverHandle = KObservable<T>::KObserverPtrAccess::GetSmartHandle( refkIn_.GetObservable() );
                                KObserverHandle::KObserverPtrAccess::AddRef( *m_pkObserverHandle );
                                m_bInitiallyValid = ( m_pkObserverHandle->GetObservable() != NULL );
                            }
                            else
                            {
                                m_pkObserverHandle = NULL;
                                m_bInitiallyValid = false;
                            }//if.. else..
                            if( pkOldHandle != NULL )
                            {
                                KObserverHandle::KObserverPtrAccess::Release( *pkOldHandle );
                            }//if
                            return *this;
                        }////operator=()

    KObserverPtr&       operator=( const KObserverPtr& refkIn_ ) 
                        {
                            KObserverHandle*   pkOldHandle =  m_pkObserverHandle;
                            if( refkIn_.m_pkObserverHandle != NULL )
                            {
                                m_pkObserverHandle = refkIn_.m_pkObserverHandle;
                                KObserverHandle::KObserverPtrAccess::AddRef( *m_pkObserverHandle );
                                m_bInitiallyValid = ( m_pkObserverHandle->GetObservable() != NULL );
                            }
                            else
                            {
                                m_pkObserverHandle = NULL;
                                m_bInitiallyValid = false;
                            }//if.. else..
                            if( pkOldHandle != NULL )
                            {
                                KObserverHandle::KObserverPtrAccess::Release( *pkOldHandle );
                            }//if
                            return *this;
                        }////operator=()

    template<typename U>
    bool                operator==( const  KObserverPtr<U>& B ) const 
                        {
                            return  _GetObservable() == B._GetObservable();
                        }//

    template<typename U>
    bool                operator==( U* B ) const 
                        {
                            return  _GetObservable() == B;
                        }//

    template<typename U>
    bool                operator != ( const  KObserverPtr<U>& B ) const 
                        {
                            return  _GetObservable() != B._GetObservable();
                        }//

    template<typename U>
    bool                operator != ( U* B ) const 
                        {
                            return  _GetObservable() != B;
                        }//

    template<typename U>
    bool                operator<( const  KObserverPtr<U>& B ) const 
                        {
                            return _GetObservable () < B._GetObservable();
                        }//

    template<typename U>
    bool                operator<( U* B ) const 
                        {
                            return _GetObservable () < B;
                        }//

    template<typename U>
    bool                operator>( const  KObserverPtr<U>& B ) const 
                        {
                            return _GetObservable () > B._GetObservable();
                        }//

    template<typename U>
    bool                operator>( U* B ) const 
                        {
                            return _GetObservable () > B;
                        }//

    template<typename U>
    bool                operator<=( const  KObserverPtr<U>& B ) const 
                        {
                            return _GetObservable () <= B._GetObservable();
                        }//

    template<typename U>
    bool                operator<=( U* B ) const 
                        {
                            return _GetObservable () <= B;
                        }//

    template<typename U>
    bool                operator>=( const  KObserverPtr<U>& B ) const 
                        {
                            return _GetObservable () >= B._GetObservable();
                        }//

    template<typename U>
    bool                operator>=( U* B ) const 
                        {
                            return _GetObservable () >= B;
                        }//


private:

    T*                  _GetObservable() const 
                        {
                            if( m_pkObserverHandle == NULL )
                                return NULL;
                            return static_cast<T*>( m_pkObserverHandle->GetObservable() );
                        }//GetObservable()

    template<typename U>
    operator   U*  ();

    bool                operator == ( int ) const;
    bool                operator != ( int ) const;


private:

    mutable KObserverHandle*       
                        m_pkObserverHandle;
    bool                m_bInitiallyValid;

};//class KObserverPtr


#define KOBSERVERPTR_DECLARE( CLASS ) \
    class   CLASS; \
    typedef  KObserverPtr< CLASS >    CLASS##oPtr;



template<typename T, typename U>
inline bool             operator == ( T* A, const  KObserverPtr<U>& B )
                        {
                            return B == A;
                        }//

template<typename T, typename U>
inline bool             operator != ( T* A, const  KObserverPtr<U>& B )
                        {
                            return  B != A;
                        }//

template<typename U>
bool             operator == (int, const  KObserverPtr<U>& );

template<typename U>
bool             operator != (int, const  KObserverPtr<U>& );


template<typename T, typename U>
inline bool             operator<( T* A, const  KObserverPtr<U>& B )
                        {
                            return B > A;
                        }//

template<typename T, typename U>
inline bool             operator>( T* A, const  KObserverPtr<U>& B )
                        {
                            return B < A;
                        }//

template<typename T, typename U>
inline bool             operator<=( T* A, const  KObserverPtr<U>& B )
                        {
                            return B >= A;
                        }//

template<typename T, typename U>
inline bool             operator>=( T* A, const  KObserverPtr<U>& B )
                        {
                            return B <= A;
                        }//

template<typename T, typename U>
inline  KObserverPtr<T>   static_pointer_cast( const KObserverPtr<U>& refkIn_ )
                        {
                            return KObserverPtr<T>( static_cast<T*>( refkIn_.GetObservable() ) );
                        }//static_pointer_cast()

template<typename T, typename U>
inline  KObserverPtr<T>   dynamic_pointer_cast( const KObserverPtr<U>& refkIn_ )
                        {
                            return KObserverPtr<T>( dynamic_cast<T*>( refkIn_.GetObservable() ) );
                        }//dynamic_pointer_cast()

template<typename T, typename U>
inline KObserverPtr<T>   const_pointer_cast( const KObserverPtr<U>& refkIn_ )
                        {
                            return KObserverPtr<T>( const_cast<T*>( refkIn_.GetObservable() ) );
                        }//const_pointer_cast()


_KSMARTNULL_COMPARISON_ONE_PARAM( KObserverPtr, typename, !In_ == true )

namespace   kog
{
    _KSMARTNULL_NULLIFY_ONE_PARAM( KObserverPtr, typename, Out_.Reset() )
}//namespace   kog


#endif // #ifndef _KOBSERVERPTR_H
