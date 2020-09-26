/** @file   KJohnObjMan.h
	@brief  interface and implementation of KJohnObjMan
	@author seojt@kogstudios.com
	@since  2005-01-07

    @date   2005-06-17, Friday
            - added comments
    @date   jintaeks on 2005-6-29, 15:06:05
            - GetObjectByUID()
    @date   jintaeks on 2005-7-2, 15:25:09
            - Next() and Prev() added for linked list like access
    @date   jintaeks on 2005-8-12, 11:45:44
            - support nested iteration
    @date   jintaeks on 2005-10-5, 17:19:24
            - memory overrun detection
            - dangling pointer detection
    @date   jintaeks on 2005-12-26, 16:52:05
            - implicit dangling pointer bug fixed
            - refactoring using 'KHandleArray'
    @date   jintaeks on 2007-10-17, 12:17
            - KHandleArray<>의 At()함수 이름이 AccessHandleNode()로 바뀐것 적용
*/

#ifndef _KJOHNOBJMAN_H
#define _KJOHNOBJMAN_H

//------------------------------------------------------------------------------
//
//#define USE_MALLOC_FREE // use malloc() and free() instead of ::new and ::delete
    // if memory leak report tool (like bounds checker or memory validator) reports memory leaks
    // enable define USE_MALLOC_FREE - jintaeks on 2005-7-15, 12:27:15

//------------------------------------------------------------------------------
//
#pragma warning(push)
#pragma warning(disable: 4786) // identifier was truncated to
#pragma warning(disable: 4995) // 'function': name was marked as #pragma deprecated
//#include "KJohnGen.h"
#include <vector>
#include "KHandleArray.h"
#include <stdlib.h>


#ifndef SAFE_DESTROY
#define SAFE_DESTROY(p_)     { if(p_!=NULL) { (p_)->Destroy(); (p_)=NULL; } }
#endif // SAFE_DESTROY


/// 전역변수 초기화 순서와 무관한 안전한 스태틱 멤버 변수를 정의하는 매크로
/// 스태틱 멤버 변수 접근은 Access##Name(), Get##Name() 으로 한다.
/** 전역변수 초기화 순서와 무관하다는 의미는, SAFE_STATIC_MEMBER()로 선언된 변수들은
    프로그램의 시작 전에 무조건 메모리할당과 초기화가 진행된다는 의미이다.
    이렇게 구현되지 않은 스태틱 변수는 변수를 처음 접근할 때 초기화된다.

    @param  Type: 변수의 type을 지정한다.
    @param  Name: Access##Name(), Get##Name()에 해당하는 변수이름의 일부를 지정한다.
            클래스 내부에서 실제 이름은 s_member이지만, 매크로가 변수이름에 따라
            유일하게 클래스들을 생성하도록 설계하였다.
    - jintaeks on 2008-07-22, 15:15 */
#ifndef SAFE_STATIC_MEMBER
#define SAFE_STATIC_MEMBER( Type, Name ) \
    class _KSafeStaticMember##Name \
    { \
    private: \
        static  _KSafeStaticMember##Name    ms_staticMember; \
        __declspec(noinline) static Type& _NonlinearAccess()   { static Type s_member; ms_staticMember._DoNothing();  return s_member; } \
        inline void           _DoNothing()       {} \
    public: \
        inline _KSafeStaticMember##Name()   { Access(); } \
        inline static Type&   Access()        { static Type& s_ref = _NonlinearAccess(); return s_ref; } \
    };\
    _KSafeStaticMember##Name    _m_safeStaticMemberInitializer##Name; \
    inline  static Type&   Access##Name()   { return _KSafeStaticMember##Name::Access(); } \
    inline  static const Type& Get##Name()  { return _KSafeStaticMember##Name::Access(); }
#endif // SAFE_STATIC_MEMBER


//------------------------------------------------------------------------------
//
#ifdef _DEBUG // 2005-10-5, 17:18:41
#define KJOM_DETECT_MEMORYOVERRUN       // enable memory overrun detection
//#define KJOM_DUMPLIST_WHEN_DELETE_ALL   // enable list dump when delete all
#endif // _DEBUG

#pragma warning(disable: 4100) // unreferenced formal parameter



///-----------------------------------------------------------------------------
/// @class  KJohnObjMan<>
/// @brief  automatic object manager which is constructed in heap
/// @param  OBJECT_TYPE: class name which derives this class
/// @param  CUSTOM_DATA: custom data type
/// @param  t_iNumCursor: number of cursors
/// @see    KHandleArray
/// @note   you can use custom memory manager to reduce dynamic memory
///         allocation of game object.
///         In this implementation, we use just ::operator new()
///
template<typename OBJECT_TYPE, typename CUSTOM_DATA=int, int t_iNumCursor=3>
class KJohnObjMan
{
public:
    typedef OBJECT_TYPE*                            POBJECT_TYPE;
    typedef KHandleArray<POBJECT_TYPE,CUSTOM_DATA>  PBOEJCT_TYPE_HANDLEARRAY;

private:

    SAFE_STATIC_MEMBER( PBOEJCT_TYPE_HANDLEARRAY, HandleArray )  ///< handle array

    static int              ms_iCursor[t_iNumCursor];   ///< cursors for multiple iteration
    static unsigned int     ms_uiGuard;                 ///< memory overrun detect sentinel

private:
    KHandle         m_UID;  ///< unique ID of this object
                            ///< actually 'index' to std::vector
                            ///< start from 0 not 1(-1 means invalid UID)

protected:
    CUSTOM_DATA     m_custom;   ///< custom data member

protected:

    /// constructor.
    /// @note   must be used as base class
    KJohnObjMan()
    {
        /// @warning   do not initialize 'm_UID'
        /*m_UID = 0;*/

        m_UID = _InsertObjectNode( (POBJECT_TYPE)this );
        ASSERT( m_UID.IsValid() == true );
    }//KJohnObjMan()

    /// destructor.
    virtual ~KJohnObjMan(){}

public:
    /// get UID(unique ID) of this object.
    /// -1 means invalid UID
    inline const KHandle& GetUID() const { return m_UID; }

    /// access custom data.
    virtual CUSTOM_DATA& AccessCustomData() { return m_custom; }
    virtual const CUSTOM_DATA& GetCustomData() const { return m_custom; }

    /// called between destructor and memory de-allocation.
    /// @note   'm_UID' is still valid in this function
    virtual void OnPreFreeMem(){}

    /// 자기 자신을 파괴한다.
    void Destroy()
    {
        /*virtual*/OnPreFreeMem();
        delete this;
    }//Destroy()

    static bool Destroy( IN const KHandle& iUID_ )
    {
        POBJECT_TYPE pObject = GetObjectByUID( iUID_ );
        if( pObject != NULL )
        {
            pObject->OnPreFreeMem();
            delete pObject;
            return true;
        }//if

        return false;
    }//Destroy()

private:
    /// insert object node and set it's UID.
    static KHandle _InsertObjectNode(POBJECT_TYPE pObject)
    {
        return AccessHandleArray().UseNode( pObject );
    }//_InsertObjectNode()

    /// delete object node by UID.
    /// @param  iUID_: UID of object
    /// @note   this function will not delete the object referenced by iUID
    /// @pre    iUID >= 0
    /// @see    _DeleteObject()
    /// @NOTE   이 함수는 node만을 지우고 객체를 delete하지는 않는다. 노드를 지우고 객체도
    ///         지우기 위해서는 _DeleteObject()를 사용해야 한다.
    static bool _DeleteObjectNode(const KHandle& iUID)
    {
        //ASSERT( iUID >= 0 && iUID < AccessHandleArray().GetSize() );
        return AccessHandleArray().ReturnNode( iUID );
    }//_DeleteObjectNode()

public:
    /// safely get next object of 'this' object.
    /// @return pointer to OBJECT
    ///         NULL if object isn't exist
    inline POBJECT_TYPE Next()
    {
        if( m_UID.IsValid() == true ) // if handle is valid
            return GetObjectByUID( AccessHandleArray().GetNext(m_UID) );
        else
            return NULL;
    }//Next()

    /// safely get previous object of 'this' object.
    /// @return pointer to OBJECT
    ///         NULL if object isn't exist
    inline POBJECT_TYPE Prev()
    {
        if( m_UID.IsValid() == true ) // if handle is valid
            return GetObjectByUID( AccessHandleArray().At(m_UID).iPrev );
        else
            return NULL;
    }//Next()

    //------------------------------------------------------------------------------
    // static iteration
    //
public:
    static int GetCursor( int _iIndex = 0 )
    {
        ASSERT( _iIndex >= 0 && _iIndex < t_iNumCursor );
        return ms_iCursor[_iIndex];
    }//GetCursor()

    //static int GetNowCursor(int _iIndex = 0)
    //{
    //    ASSERT( _iIndex >= 0 && _iIndex < t_iNumCursor );
    //    return ms_iCursor[_iIndex];
    //}//GetNowCursor()

    static void SetCursor( int _iCursor, int _iIndex = 0 )
    {
        ASSERT( _iIndex >= 0 && _iIndex < t_iNumCursor );
        ms_iCursor[_iIndex] = _iCursor;
    }//SetCursor()

    //static void SetNowCursor(int _iCursor, int _iIndex = 0)
    //{
    //    ASSERT( _iIndex >= 0 && _iIndex < t_iNumCursor );
    //    ms_iCursor[_iIndex] = _iCursor;
    //}//SetNowCursor()

    inline static int GetNumObjects() { return AccessHandleArray().GetNumUsed(); }

    /// safely get object by UID.
    /// @param  iUID_: object UID(handle)
    /// @return pointer to object
    inline static POBJECT_TYPE GetObjectByUID( IN const KHandle& iUID_ )
    {
        return AccessHandleArray().IsValidUsedHandle( iUID_ ) ? AccessHandleArray().GetData(iUID_) : NULL;
    }//GetObjectByUID()

	// added by wonpok. 20090820.
	inline static KHandle MakeValidHandle( const int iHandle_, const UINT uiMagic_ )
	{
		return AccessHandleArray().MakeValidHandle( iHandle_, uiMagic_ );
	}

    inline static KHandle MakeValidHandle( const int iHandle_ )
    {
        return AccessHandleArray().MakeValidHandle( iHandle_ );
    }//MakeValidHandle()

    ///-----------------------------------------------------------------------------
    // iteration related
    //
    /// @return handle(UID) to first object
    ///         -1 if valid object isn't exist
    /// @see    GetNextObjectNode()
    /// @note   handle is 'index' to std::vector
    /** @example    GetFirstObjectNode()
        CBaseGameObject*    pObject;
        KHandle             iNode = CBaseGameObject::GetFirstObjectNode( &pObject );
        while( iNode.IsValid() )
        {
            pObject->Print();
            iNode = CBaseGameObject::GetNextObjectNode( &pObject );
        }//while
    */
    inline static KHandle GetFirstObjectNode( OUT POBJECT_TYPE* ppOutObject, int _iCursorIndex = 0 )
    {
        ASSERT( _iCursorIndex >= 0 && _iCursorIndex < t_iNumCursor );
        ASSERT( ppOutObject );

        KHandle iNext = AccessHandleArray().GetFirst();
        if( iNext.IsValid() )
        {
            ASSERT( iNext.IsValid() && iNext.GetHandlePart() < AccessHandleArray().GetSize() );
            *ppOutObject = AccessHandleArray().GetData(iNext); // set [out] param
        }
        else
        {
            *ppOutObject = NULL; // set [out] param
        }//if.. else..

        // save cursor
        ms_iCursor[_iCursorIndex] = iNext.GetHandlePart();
        return iNext;
    }//GetFirstObjectNode()

    /// @return handle(UID) to next object
    inline static KHandle GetNextObjectNode( OUT POBJECT_TYPE* ppOutObject, int _iCursorIndex = 0 )
    {
        ASSERT( _iCursorIndex >= 0 && _iCursorIndex < t_iNumCursor );

        KHandle iUIDHandle = MakeValidHandle( ms_iCursor[_iCursorIndex] ); // get saved next link

        ASSERT( ppOutObject != NULL );
        ASSERT( iUIDHandle.IsValid() && iUIDHandle.GetHandlePart() < AccessHandleArray().GetSize() );
        if( AccessHandleArray().GetNext(iUIDHandle).IsValid() == true )
        {
            ASSERT( AccessHandleArray().IsValidHandleRange( iUIDHandle.GetHandlePart() ) );
            //ASSERT( AccessHandleArray().GetNext(iUIDHandle) >= 0 );
            //ASSERT( AccessHandleArray().GetNext(iUIDHandle) < AccessHandleArray().GetSize() );
            *ppOutObject = AccessHandleArray().GetData(AccessHandleArray().GetNext(iUIDHandle) );
        }
        else
        {
            *ppOutObject = NULL;
        }//if.. else..

        //ASSERT( iUIDHandle >= 0 );
        //ASSERT( iUIDHandle < AccessHandleArray().GetSize() );
        ASSERT( AccessHandleArray().IsValidHandleRange( iUIDHandle.GetHandlePart() ) );

        // update cursor
        KHandle nextHandle = AccessHandleArray().GetNext(iUIDHandle);
        ms_iCursor[_iCursorIndex] = nextHandle.GetHandlePart();
        return nextHandle;
    }//GetNextObjectNode()

    /// get handle(UID) to first free node.
    /// @param[out] ppOutObject: pointer to object(why do you need this?^^;)
    static int GetFirstFreeNode( OUT POBJECT_TYPE* ppOutObject )
    {
        ASSERT( ppOutObject );

        int iNext = AccessHandleArray().GetFreeHeadNext();
        if( iNext != -1 )
        {
            ASSERT( iNext >= 0 && iNext < AccessHandleArray().GetSize() );
            *ppOutObject = AccessHandleArray().GetData(iNext); // set [out] param
        }
        else
        {
            *ppOutObject = NULL; // set [out] param
        }//if.. else..

        return iNext;
    }//GetFirstFreeNode()

    /// get handle(UID) to next free node.
    /// @param  iCurUIDHandle_: current UID(handle)
    ///         return value of GetFirstFreeNode() or out param of 'GetNextFreeNode'
    static int GetNextFreeNode( OUT POBJECT_TYPE* ppOutObject, int iCurUIDHandle_ )
    {
        ASSERT( ppOutObject );
        ASSERT( iCurUIDHandle_ >= 0 && iCurUIDHandle_ < AccessHandleArray().GetSize() );
        *ppOutObject = AccessHandleArray().GetData(iCurUIDHandle_);
        return AccessHandleArray().GetNext(iCurUIDHandle_);
    }//GetNextFreeNode()

    //------------------------------------------------------------------------------
    // static operations
    //
public:
    /// create new object.
    static void* operator new(size_t size)
    {
#ifdef KJOM_DETECT_MEMORYOVERRUN // 2005-10-5, 14:46:31
        size_t  iAllocSize = size + sizeof(unsigned int)*3;
            //                      ^ allocate additional guard bytes
#else
        size_t  iAllocSize = size;
#endif // KJOM_DETECT_MEMORYOVERRUN

#ifdef USE_MALLOC_FREE
        void*   pMemory = ::malloc( iAllocSize );
#else
        void*   pMemory = ::operator new( iAllocSize );
#endif // USE_MALLOC_FREE

#ifdef KJOM_DETECT_MEMORYOVERRUN // 2005-10-5, 14:46:31
        // set guard and size
        //
        unsigned char* pByte = (unsigned char*)pMemory;
        *((unsigned int*)pByte) = ms_uiGuard; // set Begin Guard
        pByte += sizeof(unsigned int);        // skip Guard

        *((unsigned int*)pByte) = (unsigned int)size;       // set Size
        pByte += sizeof(unsigned int);        // skip Size

        *((unsigned int*)(pByte+size)) = ms_uiGuard; // set End Guard
        pMemory = (void*)pByte;
#endif // KJOM_DETECT_MEMORYOVERRUN

//{{ seojt // 2009-2-27, 21:50
//        POBJECT_TYPE    pObject = (POBJECT_TYPE)pMemory;
//        pObject->_InsertObjectNode( pObject ); // self referencing *^^*
        ///*virtual*/pObject->OnPostAllocMem();
//}} seojt // 2009-2-27, 21:50
        return pMemory;

        /** @note   이 함수가 리턴된 후에 생성자가 호출될 것이다.
                    : jintaeks on 2009-02-27, 21:50 */
    }//operator new()

    static void* operator new[](size_t size)
    {
        ASSERT( false );
    }//operator new[]()

//{{ seojt // 2009-8-20, 12:03
public:
//}} seojt // 2009-8-20, 12:03
    void operator delete(void* pBlock)
    {
        POBJECT_TYPE pObject = (POBJECT_TYPE)pBlock;
        ASSERT( pObject != NULL );
        if( pBlock == NULL )
        {
            return;
        }//if

        // 이 함수는 이미 파괴자가 실행되고 나서, 파괴자 블록을 탈출하기 전에 호출되기
        // 때문에 이곳에서 가상 함수를 호출하는 것은 의미가 없다.
        // - jintaeks on 2008-11-19, 17:54
        ///*virtual*/pObject->OnPreFreeMem(); // commented out on jintaeks on 2008-11-19, 17:53

        KJohnObjMan<OBJECT_TYPE,CUSTOM_DATA>::_DeleteObject( pObject->GetUID(), pObject );
        //::operator delete( pBlock ); // @note do not call this! 2005-7-15, 12:20 by jintaeks
    }//operator delete()

//    void operator delete[](void* pBlock)
//    {
//        ASSERT( false );
//    }//operator delete[]()

private:
    /// delete object by UID.
    /// @param  iUID_: object handle(UID)
    ///         must range from 0 to n(positive integer greater than 0)
    static bool _DeleteObject( IN const KHandle& iUID_, POBJECT_TYPE pOriginObject = NULL )
    {
        ASSERT( AccessHandleArray().IsValidHandleRange(iUID_) );
        if( AccessHandleArray().GetData(iUID_) )
        {
            // delete node
            //
            POBJECT_TYPE    pObject = AccessHandleArray().GetData(iUID_);
            pObject->_DeleteObjectNode( iUID_ );

#ifdef KJOM_DETECT_MEMORYOVERRUN // 2005-10-5, 14:56:05
            if( pOriginObject != NULL )
            {
                ASSERT( "Memory Overrun: Pointer Mismatch" && pOriginObject == pObject );
            }//if

            unsigned char* pByte = (unsigned char*)pObject;
            pByte -= sizeof(unsigned int)*2;
            if( *((unsigned int*)(pByte)) != ms_uiGuard )
            {
                ASSERT( !"Memory Overrun: Guard Corrupted" );
            }//if

            if( *((unsigned int*)(pByte+sizeof(unsigned int))) <= 0 )
            {
                ASSERT( !"Memory Overrun: Invalid Block Size" );
            }//if

            pObject = (POBJECT_TYPE)pByte;
#endif // KJOM_DETECT_MEMORYOVERRUN

            // original
#ifdef USE_MALLOC_FREE
            free( pObject );
#else
            ::operator delete( pObject );
#endif // USE_MALLOC_FREE
            return true;
        }//if

        return false;
    }//_DeleteObject()

public:
    /// safely delete all resources.
    static void Clear()
    {
        POBJECT_TYPE    pObject;
        int             iNode = GetFirstObjectNode( &pObject );

        while( iNode != -1 )
        {
            ASSERT( pObject );

            /*virtual*/pObject->Destroy();

#ifdef KJOM_DUMPLIST_WHEN_DELETE_ALL
            KJohnObjMan<OBJECT_TYPE,CUSTOM_DATA>::Debug_DumpAll();
            KJohnObjMan<OBJECT_TYPE,CUSTOM_DATA>::Debug_DumpObjectList();
            KJohnObjMan<OBJECT_TYPE,CUSTOM_DATA>::Debug_DumpFreeList();
#endif // _DEBUG
            //iNode = GetNextObjectNode( &pObject ); // don't use like this!
            iNode = GetFirstObjectNode( &pObject );
        }//while

        // delete vector itself
        AccessHandleArray().Clear();
    }//Clear()

    inline static bool IsValidUID( IN const KHandle& iUID_ )
    {
        return AccessHandleArray().IsValidUsedHandle(iUID_);
    }//IsValidUID()

    //------------------------------------------------------------------------------
    // Debug function
    //
public:
    /// output all links to debug console.
    static void Debug_DumpAll()
    {
        int     iSize = AccessHandleArray().GetSize();
        char    buffer[2048];

        for( int i = 0; i < iSize; ++i )
        {
            StringCchPrintfA( buffer, ARRAY_SIZE( buffer ), "[KJohnObjMan] [%i] (%i<-(%p)->%i),"
                , i
                , AccessHandleArray().GetPrev(i).GetHandlePart()
                , AccessHandleArray().GetData(i)
                , AccessHandleArray().GetNext(i).GetHandlePart() );
            TRACEA( buffer );
        }//for
        TRACEA( "\r\n" );
    }//Debug_DumpAll()

    /// output allocated node to debug console.
    static void Debug_DumpObjectList()
    {
        char            buffer[2048];
        POBJECT_TYPE    pObject;
        KHandle         iNode = GetFirstObjectNode( &pObject );

        TRACEA( "[KJohnObjMan] Objects: " );
        while( iNode.IsValid() )
        {
            ASSERT( AccessHandleArray().GetData(iNode) );
            StringCchPrintfA( buffer, ARRAY_SIZE( buffer ), "%i: (%i<-%i->%i)--> "
                , iNode.GetHandlePart()
                , AccessHandleArray().GetPrev(iNode).GetHandlePart()
                , iNode.GetHandlePart()
                //, AccessHandleArray().GetHandleNode(iNode).iHandle
                , AccessHandleArray().GetNext(iNode).GetHandlePart() );
            TRACEA( buffer );
            iNode = GetNextObjectNode( &pObject );
        }//while
        TRACEA( "\r\n" );
    }//Debug_DumpObjectList()

    /// output free node to debug console.
    static void Debug_DumpFreeList()
    {
        char            buffer[2048];
        POBJECT_TYPE    pObject;
        int             iNode = GetFirstFreeNode( &pObject );

        TRACEA( "[KJohnObjMan] Free: " );
        while( iNode != -1 )
        {
            ASSERT( NULL == AccessHandleArray().GetData(iNode) );
            StringCchPrintfA( buffer, ARRAY_SIZE( buffer ), "%i: (%i<-%i->%i)--> "
                , iNode
                , AccessHandleArray().GetPrev(iNode).GetHandlePart()
                , iNode
                //, AccessHandleArray().GetHandleNode(iNode).iHandle
                , AccessHandleArray().GetNext(iNode).GetHandlePart() );
            TRACEA( buffer );
            iNode = GetNextFreeNode( &pObject, iNode );
        }//while
        TRACEA( "\r\n" );
    }//Debug_DumpFreeList()
};//class KJohnObjMan

// [format]
//
// template
//  type
//   variable name = 'initialization'


template<typename OBJECT_TYPE,typename CUSTOM_DATA,int t_iNumCursor>
    int
        KJohnObjMan<OBJECT_TYPE,CUSTOM_DATA,t_iNumCursor>::ms_iCursor[t_iNumCursor];// = 0;

template<typename OBJECT_TYPE,typename CUSTOM_DATA,int t_iNumCursor>
    unsigned int
        KJohnObjMan<OBJECT_TYPE,CUSTOM_DATA,t_iNumCursor>::ms_uiGuard = 0xDEADC0DE;

template<typename OBJECT_TYPE,typename CUSTOM_DATA,int t_iNumCursor>
    typename KJohnObjMan<OBJECT_TYPE,CUSTOM_DATA,t_iNumCursor>::_KSafeStaticMemberHandleArray 
        KJohnObjMan<OBJECT_TYPE,CUSTOM_DATA,t_iNumCursor>::_KSafeStaticMemberHandleArray::ms_staticMember;    


#pragma warning(pop)

#endif // _KJOHNOBJMAN_H


/** @example    KJohnObjMan<>
    @code

  //------------------------------------------------------------------------------
  //
  1) general usage:

    #include <conio.h>
    #include <vector>
    #include <string>
    #include <iostream>
    #include "KJohnObjMan.h"

    #define PRINT(s)    TRACEA(s); std::cout << (s);// << std::endl;

    char    buffer[256];

    ////class CBaseGameObject : public KJohnObjMan<CBaseGameObject,int>
    ////class CBaseGameObject : public KJohnObjMan<CBaseGameObject,char[80]>
    class CBaseGameObject : public KJohnObjMan<CBaseGameObject,std::string>
    {
    public:
        CBaseGameObject() : KJohnObjMan<CBaseGameObject,std::string>()
        {
            ////m_custom = 1;
            ////StringCchCopyA(m_custom, 80, "CBaseGameObject");
            m_custom = "CBaseGameObject";
        }//CBaseGameObject()
        virtual ~CBaseGameObject(){}
        virtual void Print(){}
    };//class CBaseGameObject : public KJohnObjMan<>

    class CCar : public CBaseGameObject
    {
    private:
        char    m_szName[80];

    public:
        CCar(LPCTSTR szName) : CBaseGameObject()
        {
            ////m_custom = 2;
            ////StringCchCopyA( m_custom, ARRAY_SIZE( m_custom ), "CCar" );
            m_custom = "CCar";
            StringCchCopyA( m_szName, ARRAY_SIZE( m_szName), szName );
        }//CCar()

        virtual ~CCar()
        {
            StringCchPrintfA( buffer, ARRAY_SIZE( buffer ), "CCar::~CCar(): %s\n", m_szName );
            PRINT( buffer );
        }//~CCar()

        virtual void Print()
        {
            StringCchPrintfA( buffer, ARRAY_SIZE( buffer ), "CCar::Print(): %s\n", m_szName );
            PRINT( buffer);
        }//Print()
    };//class CCar : public CBaseGameObject

    class CBuilding : public CBaseGameObject
    {
    private:
        char    m_szName[80];

    public:
        CBuilding(LPCTSTR szName) : CBaseGameObject()
        {
            ////m_custom = 3;
            ////StringCchCopyA( m_custom, ARRAY_SIZE( m_custom ), "CBuilding" );
            m_custom = "CBuilding";
            StringCchCopyA( m_szName, ARRAY_SIZE( m_szName ), szName );
        }//CBuilding()

        virtual ~CBuilding()
        {
            StringCchPrintfA( buffer, ARRAY_SIZE( buffer ), "CBuilding::~CBuilding(): %s\n", m_szName );
            PRINT( buffer );
        }//~CBuilding()

        virtual void Print()
        {
            StringCchPrintfA( buffer, ARRAY_SIZE( buffer ), "CBuilding::Print(): %s\n", m_szName );
            PRINT( buffer);
        }//Print()
    };//class CBuilding : public CBaseGameObject

    void IterateAllGameObjects()
    {
        ////CBaseGameObject*    pObject;
        ////int                 iNode = CBaseGameObject::GetFirstObjectNode( &pObject );
        ////while ( 0 != iNode )
        ////{
        ////    pObject->Print();
        ////    iNode = CBaseGameObject::GetNextObjectNode( &pObject );
        ////}//while

        CBaseGameObject*    pObject;
        CBaseGameObject::GetFirstObjectNode( &pObject );

        while ( pObject )
        {
            pObject->Print();
            pObject = pObject->Next();
        }//while
    }//IterateAllGameObjects()

    void main()
    {
        //// create game objects
        //
        std::cout << std::endl << "// create game objects" << std::endl;
        CCar*       pCar1 = new CCar( "first" );
        CCar*       pCar2 = new CCar( "second" );
        CBuilding*  pBuilding = new CBuilding( "building" );
        CCar*       pCar3 = new CCar( "third" );

        int         uid = pCar2->GetUID();

        //// iterate all objects
        //
        std::cout << std::endl << "// iterate all objects" << std::endl;
        IterateAllGameObjects();

        //// access by UID
        //
        std::cout << std::endl << "// access by UID" << std::endl;
        CBaseGameObject*    pObject = CBaseGameObject::GetObjectByUID( uid );
        if ( pObject )
            pObject->Print();

        //// delete a object
        //
        std::cout << std::endl << "// delete a object" << std::endl;
        delete pCar2;
        IterateAllGameObjects();

        //// access by UID
        //
        std::cout << std::endl << "// access by UID" << std::endl;
        pObject = CBaseGameObject::GetObjectByUID( uid );
        if ( pObject )
            pObject->Print();
        else
            std::cout << uid << " is invalid" << std::endl;

        //// Custom Data
        //
        std::cout << std::endl << "// Custom Data" << std::endl;
        pObject = pCar1;
        std::cout << pObject->GetObjectName() << std::endl;
        pObject = pBuilding;
        std::cout << pObject->GetObjectName() << std::endl;

        //// safely delete all heap objects
        //
        std::cout << std::endl << "// safely delete all heap objects" << std::endl;
        CBaseGameObject::Clear();

        // console output
        //
        //// create game objects

        //// iterate all objects
        //CCar::Print(): first
        //CCar::Print(): second
        //CBuilding::Print(): building
        //CCar::Print(): third

        //// access by UID
        //CCar::Print(): second

        //// delete a object
        //CCar::~CCar(): second
        //CCar::Print(): first
        //CBuilding::Print(): building
        //CCar::Print(): third

        //// access by UID
        //1 is invalid

        //// Custom Data
        //CCar
        //CBuilding

        //// safely delete all heap objects
        //CCar::~CCar(): first
        //CBuilding::~CBuilding(): building
        //CCar::~CCar(): third
        //Press any key to continue

        // debug output
        //
        //CCar::Print(): first
        //CCar::Print(): second
        //CBuilding::Print(): building
        //CCar::Print(): third
        //CCar::Print(): second
        //CCar::~CCar(): second
        //CCar::Print(): first
        //CBuilding::Print(): building
        //CCar::Print(): third
        //CCar::~CCar(): first
        //CBuilding::~CBuilding(): building
        //CCar::~CCar(): third
    }//main()


  ////------------------------------------------------------------------------------
  2) developer usage:

    #include <windows.h>
    #include <stdio.h>
    #include <vector>
    #include <conio.h>
    #include <string.h>
    #include "KJohnObjMan.h"

    #ifndef DEBUG_DUMP_ALL
    #if defined(_DEBUG) // 2005-01-07 오후 1:24:08
    #define DEBUG_DUMP_ALL(t)   KJohnObjMan<t>::Debug_DumpAll();\
            KJohnObjMan<t>::Debug_DumpObjectList();\
            KJohnObjMan<t>::Debug_DumpFreeList()
    #else
    #define DEBUG_DUMP_ALL(t)
    #endif // _DEBUG: 2005-01-07 오후 1:24:08
    #endif // #ifndef DEBUG_DUMP_ALL

    char    buffer[256];

    class CBaseGameObject : public KJohnObjMan<CBaseGameObject>
    {
    public:
        CBaseGameObject(){}
        virtual ~CBaseGameObject(){}
        virtual void Print(){}
    };//class CBaseGameObject : public KJohnObjMan<>

    //// first game class
    class CCar : public CBaseGameObject
    {
    private:
        char    m_szName[80];

    public:
        CCar(LPCTSTR szName){ StringCchCopyA(m_szName, 80, szName); }
        virtual ~CCar(){}

        virtual void Print()
        {
            StringCchPrintfA( buffer, ARRAY_SIZE( buffer ), "\nCCar::Print(): %s", m_szName );
            TRACEA( buffer);
        }//Print()
    };//class CCar : public CBaseGameObject

    // second game class
    class CBuilding : public CBaseGameObject
    {
    private:
        char    m_szName[80];

    public:
        CBuilding(LPCTSTR szName){ StringCchCopyA(m_szName, 80, szName); }
        virtual ~CBuilding(){}

        virtual void Print()
        {
            StringCchPrintfA( buffer, ARRAY_SIZE( buffer ), "\nCBuilding::Print(): %s", m_szName );
            TRACEA( buffer);
        }//Print()
    };//class CBuilding : public CBaseGameObject


    void IterateAllGameObjects()
    {
        CBaseGameObject*    pObject;
        int                 iNode = CBaseGameObject::GetFirstObjectNode( &pObject );

        while ( -1 != iNode )
        {
            pObject->Print();
            iNode = CBaseGameObject::GetNextObjectNode( &pObject );
        }//while
    }//IterateAllGameObjects()

    void main()
    {
        INIT_CRTDEBUG();

        typedef std::vector<CBaseGameObject*>               OBJ_LIST;
        typedef std::vector<CBaseGameObject*>::iterator     OBJ_LIST_ITOR;

        const int           c_iNumObjects   = 5;
        OBJ_LIST            objVector;
        CBaseGameObject*    pObject         = NULL;

        ///////////////////////////////////////////////////////////////////////////////
        //// Test 1: heap object
        //// create heap objects
        for (int i=0; i < c_iNumObjects; ++i)
        {
            pObject = new CBaseGameObject();
            objVector.push_back( pObject );
            DEBUG_DUMP_ALL(CBaseGameObject);
        }//for

        //// delete object by pointer
        delete objVector[c_iNumObjects/2];
        DEBUG_DUMP_ALL(CBaseGameObject);

        //// delete first object
        delete objVector[0];
        DEBUG_DUMP_ALL(CBaseGameObject);

        //// delete last object
        delete objVector[c_iNumObjects-1];
        DEBUG_DUMP_ALL(CBaseGameObject);

        ///////////////////////////////////////////////////////////////////////////////
        //// Test 2: stack object
        CBaseGameObject stackObject;
        CBaseGameObject stackObjects[10];
        DEBUG_DUMP_ALL(CBaseGameObject);

        //// Test 1: cont. create additional heap objects
        for (i = 0; i < 6; ++i )
        {
            pObject = new CBaseGameObject();
            DEBUG_DUMP_ALL(CBaseGameObject);
        }//for

        //// delete object by UID
        CBaseGameObject::Destroy( 1 );
        DEBUG_DUMP_ALL(CBaseGameObject);

        ///////////////////////////////////////////////////////////////////////////////
        //// Test 3: iterate all heap objects

        //// safely delete all heap objects
        CBaseGameObject::Clear();
        ////STL_CLEAR( objVector );

        ///////////////////////////////////////////////////////////////////////////////
        //// Test 4: operator new[]()
        ////pObject = CBaseGameObject[10]; // doesn't work!

        ///////////////////////////////////////////////////////////////////////////////
        //// Test 5: various heap objects
        pObject = new CCar( "FirstCar" );
        int iUID = pObject->GetUID();
        DEBUG_DUMP_ALL(CBaseGameObject);
        IterateAllGameObjects();

        pObject = new CCar( "SecondCar" );
        DEBUG_DUMP_ALL(CBaseGameObject);
        IterateAllGameObjects();

        new CBuilding( "FirstBuilding" );
        DEBUG_DUMP_ALL(CBaseGameObject);
        IterateAllGameObjects();

        delete pObject; // delete second CCar object
        DEBUG_DUMP_ALL(CBaseGameObject);
        IterateAllGameObjects();

        new CBuilding( "SecondBuildig" );
        DEBUG_DUMP_ALL(CBaseGameObject);
        IterateAllGameObjects();

        //// access first CCar object by UID
        pObject = CBaseGameObject::GetObjectByUID( iUID );
        pObject->Print();

        CBaseGameObject::Destroy( iUID );
        DEBUG_DUMP_ALL(CBaseGameObject);
        IterateAllGameObjects();

        new CBuilding( "ThirdBuilding" );
        DEBUG_DUMP_ALL(CBaseGameObject);
        IterateAllGameObjects();

        //// safely delete all heap objects
        CBaseGameObject::Clear();
    }//main()

    @endcode

  ////------------------------------------------------------------------------------
  3) developer comments:

    ◆ class KJohnObjMan<>

    KJohnObjMan<>는 게임내에서 생성되거나 삭제되는 게임 객체들을 별다른 코드없이
    리스트로 유지하고자 하는 것입니다.<br><br>
    이러한 생성 및 구조 패턴은 이미 소개된 적이 있지만(Board, GPG GemsIII, 2003)
    KJohnObjMan<>은 아래와 같이 보다 향상된 기능을 가집니다. KJohnObjMan<>는
    아래의 요구 조건들을 만족합니다.<br><br>

    1. 게임 객체들을 new로 생성할 때 별다른 코드없이 자동으로 리스트로 유지해야
    한다.<br><br>
    2. 힙에 생성되는 객체들만을 유지하고, 스택에 생성되는 객체는 유지하면 안된다.
    (공통 조상을 상속 받는 임시 객체를 잠시 만들 필요가 있기 때문입니다)<br><br>
    3. 게임 객체들은 자신만의 유일한 ID인 UID를 유지해야 한다. 이것은 P2P 게임에서
    상대방 객체를 판별하기 위해서 반드시 필요한 것입니다.<br><br>
    4. 생성과 삭제시에도 객체의 UID는 변하면 안되고, 생성과 삭제의 오버헤드는 최소
    화되어야 한다.<br><br>
    5. UID를 이용해서 객체를 상수 시간에 접근 가능해야 한다. 예를 들면 P2P 네트웍으
    로 100번 UID의 객체의 정보를 전송한 경우, 패킷을 받는 쪽에서는 100번 UID를
    이용해서 객체를 상수 시간에 접근 가능해야 합니다.<br><br><br>

    KJohnObjMan<>는 위 조건들을 모두 만족합니다. 위 요구 조건들은 다음과 같이
    구현하였습니다.<br><br>

    조건 1.<br>

    템플릿 클래스의 정적 멤버를 구현함으로서, 각 타입마다 유일한 리스트를 가지게
    되며, operator new()를 overloading하여 new 연산자의 호출때 객체의 시작 주소는
    자동으로 static list에 추가됩니다.<br><br>

    조건 2.<br>

    operator new()의 호출시에 리스트에 추가되므로, operator new()를 호출하지 않는
    스택 객체는 리스트에 추가되지 않습니다.<br><br>

    조건 3,4.<br>

    UID는 연속된 메모리 배열의 index로 동작합니다. 또한 객체의 생성 삭제시 UID의
    일관성을 유지하기 위해, Object List와 Free List를 유지하므로, 객체가 추가/삭
    제 될때 리스트의 유지 비용은 O(1)입니다. std::vector<>의 경우 push_back()이
    일어날때마다 pointer가 무효화되므로, double linked list 역시 UID로 유지됩니다.<br><br>

    조건 5.<br>

    UID는 std::vector<>의 인덱스입니다. 또한 객체가 삭제되거나 추가될 때도
    각 객체의 UID는 여전히 유효합니다.<br><br>

*/
