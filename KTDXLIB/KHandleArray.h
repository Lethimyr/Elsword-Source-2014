/** @file   KHandleArray.h
    @brief  interface of KHandleArray
    @author seojt@kogstudios.com
    @since  2005-12-23, 15:08:21

    @desc   std::vector<> 배열의 index를 핸들로 사용하는 일반적인 리소스 관리자
            이다. std::vector<>를 사용하는 경우, 노드를 삽입/삭제하는 경우
            동적 메모리 할당이 일어난다. KHandleArray<>는 이러한 동적 메모리
            할당 동작을 최소화하기 위하여 설계하였다.
            std::vector<>에 삽입/삭제가 일어난는 경우, 노드를 재 사용하기 위해
            할당된 노드의 리스트와 해제된 노드의 리스트를 유지한다. 그리고 노드
            가 필요하거나 필요 없어지는 경우, 이러한 두개의 리스트를 갱신하여
            유지함으로써 동적 메모리 할당을 최소화 할 수 있다.
            핸들은 실제로 std::vector<>에 대한 index인데, 삽입/삭제시에 일관된
            방법을 사용하므로, 한 번 정해진 핸들은 삭제되지 않는 한 계속해서
            유효하도록 설계하였다.

    @exam   모든 노드들을 iterate하는 예

            int iHandle = harr.GetFirst();
            while ( iHandle != -1 )
            {
                harr[iHandle].Test();
                iHandle = harr.GetNext( iHandle );
            }//while

    @date   jintaeks on 2005-12-27, 10:35:00
            - complex type example added
    @date   jintaeks on 2005-12-30, 17:34:47
            - IsValidUsedHandle()
            - IsValidUnusedHandle()
    @date   jintaeks on 2006-2-9, 15:39:27
            - operator[]() added
            - example revised for modified API
    @date   jintaeks on 2006-2-10, 21:00:39
            - GetFirst(), GetNext()
    @date   jintaeks on 2006-7-31, 15:42:23
            - Magic Number added to classify different nodes which have same handle
    @date   jintaeks on 2007-2-13, 15:37
            - const T& operator[]() 추가
    @date   jintaeks on 2007-08-06, 11:13
            - UseNode2()를 추가
                --  단지 사용하겠다는 의도로 노드를 할당하지만, 노드 자체를 초기화할 필요는 없을
                    수도 있으므로, 파라미터를 가지지 않는 UseNode2()를 추가함
    @date   jintaeks on 2007-08-14, 15:20
            - GetFirstUnused()를 추가
            -   노드의 데이터가 힙에 할당된 포인터를 가지는 경우, free 노드일지라도 heap에서
                delete할 필요가 있으므로, free node list를 iterate할 필요가 생기다.
    @date   jintaeks on 2007-10-17, 12:07
            - 함수 At()의 이름을 _AccessHandleNode()로 변경
                --  At()은 handle 자체를 접근하는 의미로 사용하므로,
                    일관성있는 API를 위해 이름을 변경하다.
    @date   jintaeks on 2007-10-26, 10:14
            - GetCandidateHandle() 추가
                --  다음에 추가될 핸들을 미리 얻어 내는 기능. 핸들값으로 유일한 key를 미리 결정하기
                    위해서 추가함
            - IsValidHandleRange()
                -- used/free 여부에 상관없이 valid한 index 범위인지를 검사하는 함수 추가
    @date   jintaeks on 2007-10-29, 10:23
            - HANDLE_NODE<>의 iHandle을 멤버에서 제외함
                -- 노드를 알 때 핸들값을 알기 위해서 추가한 것 같으나, 노드를 알려고 하는 순간에
                    이미 핸들을 알아야 하므로, 필요없을 것 같고, 소스코드에서
                    직접 참조하는 일이 없고 디버그 용으로만 사용하고 있음
    @date   jintaeks on 2008-03-25, 13:56
            - m_freeHead와 m_usedHead구조체를 삭제하고 int를 사용하여 head 노드를 가리키도록 수정.
    @date   jintaeks on 2008-05-16, 12:00
            - 설명문 추가
            - HANDLE_NODE<>에 typename NKEY를 추가.
                -- handle을 알 때 handle에 해당하는 key를 바로 접근할 수 있도록 기능을 추가
*/

#ifndef _KHANDLEARRAY_H
#define _KHANDLEARRAY_H

//#include "KJohnGen.h"
#include <vector>
#include "CSLock.h"
#include "KHandle.h"


#define KMHA_ENDLINK            -1              ///< end of link
#define KHA_INVALID_HANDLE      KMHA_ENDLINK    ///< invalid handle


#define KHandleArrayT           template< typename T, typename NKEY, int t_iNumInitialReserve, bool t_isMultiThreaded, template <class> class ALLOCATOR >
#define KHandleArrayC           KHandleArray< T, NKEY, t_iNumInitialReserve, t_isMultiThreaded, ALLOCATOR >


///-----------------------------------------------------------------------------
/// @class  KHandleArray
/// @brief  manages handles for all types of resources
/// @param  T: custom data for handle node
/// @param  NKEY: node의 키 값을 지정한다.
/// @param  t_iNumInitialReserve: initial node array size
/// @NOTE   you must define below operators for type 'T'
///             - T& operator=(const T&);
///             - bool operator==(const T&);
///             - T& operator=(int);
///
///         Do not use push_back(), pop_back() directly.
///         You must use UseNode() or ReturnNode() instead of push_back() and pop_back().
/// @see    HANDLE_NODE
/**
    @page   KHandleArray Docs for KHandleArray
    @htmlonly
        <br clear=all>
        <a href="./KHandleArray_20051227_seojt.htm">The Structures of class KHandleArray</a>
        <br clear=all>
    @endhtmlonly
*/
template< typename T
            , typename NKEY
            , int t_iNumInitialReserve = 20
            , bool t_isMultiThreaded = false
            , template <class> class ALLOCATOR=std::allocator >
class KHandleArray
{
protected:
    ///-----------------------------------------------------------------------------
    /// @enum   HANDLE_NODE_FLAG
    /// @brief  flag for memory handle node
    /// @note   last bit: used flag
    ///
    enum HANDLE_NODE_FLAG
    {
        HANDLE_NODE_CLEAR   = 0x0000,   ///< clear node
        HANDLE_NODE_USED    = 0x0001,   ///< node is using
    };//enum HANDLE_NODE_FLAG


    ///-----------------------------------------------------------------------------
    /// @struct HANDLE_NODE
    ///         handle node for KHandleArray
    /// @param  T: user defined target data type
    ///         generally pointer to the game object
    /// @note   there must be defined operator=() for data type 'T'
    ///
    template<typename T,typename NKEY>
    struct HANDLE_NODE
    {
    public:
        T       tData;      ///< custom data
                            ///< ex) memory offset from begin of memory chunk
        /** KHandleArray<>를 std::map등과 같이 사용할 때, key에 해당하는 handle을 찾는
            시간은 O(log n)이지만, handle로 key를 찾는 것은 O(n)이 걸린다.
            이 문제를 해결하기 위해 key를 node에 저장하여 두다.
            key가 차지하는 메모리 공간의 overhead는 감사하기로 하다.
            - jintaeks on 2008-05-16, 12:02 */
        NKEY    nkey;       ///< 노드가 key값을 가지는 map에 삽입된 경우의 key값.

        USHORT  uiFlag;     ///< memory flag, @see HANDLE_NODE_FLAG
        /** magic number는 핸들은 유효하지만 다른 노드임을 구분하기 위해 사용한다.
            예를 들어 어떤 객체 A가 핸들 1을 유효하게 가진 경우,
            프로그램의 어떤 곳에서 다른 객체 B가 핸들 1에 해당하는 객체를 지웠다가 다시 만들었다고
            하자. 그러면 객체 A입장에서 핸들 1은 여전히 유효하지만, 사실은 유효하지 않은 노드에
            대한 핸들이다. 이러한 경우 객체 A는 핸들 1이 가리키는 노드의  magic number를 저장해 두었다가
            magic number가 틀리면 자신이 가리키는 노드가 invalid함을 알 수 있다.
            - jintaeks on 2008-03-31, 16:36 */
        UINT    uiMagic;    ///< magic number to classify abnormal node or not

        // doubly linked list
        //
        int     iPrev;      ///< link to previous node index(handle)
        int     iNext;      ///< link to next node index(handle)
                            ///< -1 for end of link(KMHA_ENDLINK)
    public:
        ///-----------------------------------------------------------------------------
        /// constructor.
        HANDLE_NODE()
            //: tData(NULL), iHandle(-1), uiFlag(0), iPrev(-1), iNext(-1)
        {
            Clear();
        }//HANDLE_NODE()

        HANDLE_NODE(const HANDLE_NODE& right_)
        {
            operator=(right_);
        }//HANDLE_NODE()

        HANDLE_NODE& operator=(const HANDLE_NODE& right_)
        {
            tData = right_.tData;
            nkey = right_.nkey;
            uiFlag = right_.uiFlag;
            uiMagic = right_.uiMagic;
            iPrev = right_.iPrev;
            iNext = right_.iNext;
            return *this;
        }//operator=()

        inline bool operator==(const HANDLE_NODE& right) const
        {
            return tData == right.tData && uiMagic == right.uiMagic;
                //&& nkey == right.nkey && uiFlag == right.uiFlag && uiMagic == right.uiMagic;
        }//operator==()

        inline bool operator<(const HANDLE_NODE& right_) const
        {
            return tData < right_.tData;
        }//operator<()

        /// clear node data.
        inline void Clear()
        {
            /** @NOTE   
                you must define operator=(int) for 'tData' type
                - 2006-2-11, 19:30

                data 자체를 NULL로 초기화하지 않아도 될 것 같아,
                데이터를 NULL로 초기화하는 문장을 주석 처리하다.
                - jintaeks on 2008-04-25, 10:34 */
            //tData   = NULL;

            //nkey = NULL;
            uiFlag  = 0x0000;
            uiMagic = 0;
            iPrev   = -1;
            iNext   = -1;
        }//Clear()

        /// get 'used' flag.
        inline bool IsUsedFlag() const { return (uiFlag & HANDLE_NODE_USED) ? true : false; }

        /// set 'used' flag.
        inline void SetUsedFlag() { uiFlag |= HANDLE_NODE_USED; }

        /// clear 'used' flag.
        inline void ClearUsedFlag() { uiFlag &= ~HANDLE_NODE_USED; }

        /// access data.
        inline T& AccessData() { return tData; }

        inline const T& GetData() const { return tData; }
    };//struct HANDLE_NODE

public:
    /** ReturnNode()의 마지막에 호출하는 콜백 함수의 type
        node를 리턴하기 전에, client 프로그램에서 필요한 처리가 발생하는 경우, callback 함수를
        설치하고, callback 함수의 내부에서 필요한 처리를 한다.
        처음 설계 의도는 node를 리턴할 때, return하는 node에 대한 handle을 얻어서,
        client 프로그램의 모든 handle 참조를 무효화시키기 위해서 제작했다.
        - jintaeks on 2008-05-16, 12:05 */
    typedef void (CALLBACK *INT_CALLBACK)(const KHandle& iReturnedHandle_);
    typedef typename HANDLE_NODE<T,NKEY>                                NODE_TYPE;
    typedef typename std::vector<NODE_TYPE, ALLOCATOR<NODE_TYPE > >     VEC_NODES;

public:
    static UINT     s_uiMagicNumber;
    static KHandle  s_invalidHandle;

private:
    VEC_NODES       m_vecNodes;

    // free handle list --------------------------------------------------
    //
    /// 'm_vecNodes[]'에 대한 index이다. -1이면 가리키는 노드가 없음을 나타낸다. 
    int             m_iFreeHead;
    /// 'm_vecNodes[]'에 대한 index이다. -1이면 가리키는 노드가 없음을 나타낸다. 
    int             m_iFreeTail;    ///< index to tail free node
                                    ///< -1 to indicate 'm_freeHead'
    int             m_uiNumFree;    ///< number of free unused nodes

    // used handle list --------------------------------------------------
    //
    /// 'm_vecNodes[]'에 대한 index이다. -1이면 가리키는 노드가 없음을 나타낸다. 
    int             m_iUsedHead;
    /// 'm_vecNodes[]'에 대한 index이다. -1이면 가리키는 노드가 없음을 나타낸다. 
    int             m_iUsedTail;    ///< pointer to tail used node
                                    ///< -1 to indicate 'm_usedHead'
    int             m_uiNumUsed;    ///< number of used nodes

    /** m_fpPostReturnNode값이 NULL이 아니면 ReturnNode()의 제일 마지막에 'm_fpPostReturnNode'
        를 호출한다. - jintaeks on 2008-03-27, 13:29 */
    INT_CALLBACK    m_fpPostReturnNode;

protected:
    mutable MemberCriticalSectionTemplate<t_isMultiThreaded>   m_cs;

public:
    /// constructor.
    KHandleArray();
    /// destructor.
    ~KHandleArray();

private:
    void _Init();

    inline KHandle _MakeHandle( int iHandle_ ) const
    {
        return (IsValidUsedHandle(iHandle_) == true)
            ? KHandle( iHandle_, GetMagicNumber(iHandle_) )
            : s_invalidHandle;
    }//_MakeHandle()

public:
    /// safely delete handle node buffers.
    void Clear();

	// added by wonpok. 20090820.
	inline KHandle MakeValidHandle( int iHandle_, UINT uiMagic_ ) const
	{
		return (IsValidUsedHandle( iHandle_, uiMagic_ ) == true)
			? KHandle( iHandle_, uiMagic_ )
			: s_invalidHandle;
	}

    inline KHandle MakeValidHandle( int iHandle_ ) const
    {
        return (IsValidUsedHandle(iHandle_) == true)
            ? KHandle( iHandle_, GetMagicNumber(iHandle_) )
            : s_invalidHandle;
    }//MakeValidHandle()

private:
    /// access a node.
    /// @param  iIndex_: handle, index to array std::vector.
    /// @return NODE_TYPE&: reference to target node
    /// @note   use AccessData() to directly access type 'T'
    /// @see    AccessData(), GetData()
    inline NODE_TYPE& _AccessHandleNode(int iIndex_)
    {
        ASSERT( iIndex_ >= 0 && iIndex_ < GetSize() );
        return m_vecNodes[iIndex_];
    }//_AccessHandleNode()

    inline const NODE_TYPE& _GetHandleNode(int iIndex_) const
    {
        ASSERT( iIndex_ >= 0 && iIndex_ < GetSize() );
        return m_vecNodes[iIndex_];
    }//_GetHandleNode()

public:
    /** handle이 가키키는 노드의 magic number를 얻는다.
        magic number는 UseNode() 혹은 UseNode2()를 호출할 때마다 1씩 증가하고,
        4바이트 unsigned integer를 사용하므로, 한 게임 프로그램에서 magic number가
        같을 확률은 0에 가깞다.
        - jintaeks on 2008-03-31, 16:38 */
    inline UINT GetMagicNumber(int iHandle_) const
    {
        ASSERT( iHandle_ >= 0 && iHandle_ < GetSize() );
        return m_vecNodes[iHandle_].uiMagic;
    }//GetMagicNumber()

    /// iHandle_의 키 값 .nkey를 얻는다.
    inline const NKEY& GetKey(int iHandle_) const
    {
        ASSERT( iHandle_ >= 0 && iHandle_ < GetSize() );
        return m_vecNodes[iHandle_].nkey;
    }//GetKey()

    /// get reference to data.
    inline T& AccessData(int iHandle_)
    {
        ASSERT( IsValidHandleRange(iHandle_) == true );
        return m_vecNodes[iHandle_].tData;
    }//AccessData()

    inline const T& GetData(int iHandle_) const
    {
        ASSERT( IsValidHandleRange(iHandle_) == true );
        return m_vecNodes[iHandle_].tData;
    }//GetData()

    inline T& operator[](int iHandle_)
    {
        ASSERT( IsValidHandleRange(iHandle_) == true );
        return m_vecNodes[iHandle_].tData;
    }//operator[]()

    inline const T& operator[](int iHandle_) const
    {
        ASSERT( IsValidHandleRange(iHandle_) == true );
        return m_vecNodes[iHandle_].tData;
    }//operator[]()

    inline T& AccessData(const KHandle& iHandle_)
    {
        ASSERT( IsValidHandleRange(iHandle_.GetHandlePart()) == true );
        return m_vecNodes[iHandle_.GetHandlePart()].tData;
    }//AccessData()

    inline const T& GetData(const KHandle& iHandle_) const
    {
        ASSERT( IsValidHandleRange(iHandle_.GetHandlePart()) == true );
        return m_vecNodes[iHandle_.GetHandlePart()].tData;
    }//GetData()

    inline T& operator[](const KHandle& iHandle_)
    {
        ASSERT( IsValidHandleRange(iHandle_.GetHandlePart()) == true );
        return m_vecNodes[iHandle_.GetHandlePart()].tData;
    }//operator[]()

    inline const T& operator[](const KHandle& iHandle_) const
    {
        ASSERT( IsValidHandleRange(iHandle_.GetHandlePart()) == true );
        return m_vecNodes[iHandle_.GetHandlePart()].tData;
    }//operator[]()

    /// get size of this std::vector.
    inline int GetSize() const
    {
        CSLockTemplate<t_isMultiThreaded> lock(m_cs);
        return (int)m_vecNodes.size();
    }//GetSize()

    INT_CALLBACK GetPostReturnNodeCB() { return m_fpPostReturnNode; }
    void SetPostReturnNodeCB(INT_CALLBACK fpCb_) { m_fpPostReturnNode = fpCb_; }

protected:
    /// onetime setup handle array.
    /// reserve handle array by size 't_iNumInitialReserve' and initialize links
    void Setup();

public:
    ///-----------------------------------------------------------------------------
    // link helpers
    //
    /// safely get next link of 'iHandle_' node
    inline int _GetNextLink(int iHandle_)
    {
        return ( iHandle_ >= 0 && iHandle_ < GetSize() ) ? m_vecNodes[iHandle_].iNext : -1;
    }//_GetNextLink()

    /// safely get previous link of 'iHandle_' node
    inline int _GetPrevLink(int iHandle_)
    {
        return ( iHandle_ >= 0 && iHandle_ < GetSize() ) ? m_vecNodes[iHandle_].iPrev : -1;
    }//_GetPrevLink()

//    /// safely set next link of 'iHandle_' node
//    inline bool SetNextLink(int iHandle_, int iLink_)
//    {
//        if ( iHandle_ >= 0 && iHandle_ < GetSize() )
//        {
//            m_vecNodes[iHandle_].iNext = iLink_;
//            return true;
//        }//if
//        return false;
//    }//SetNextLink()
//
//    /// safely set previous link of 'iHandle_' node
//    inline bool SetPrevLink(int iHandle_, int iLink_)
//    {
//        if ( iHandle_ >= 0 && iHandle_ < GetSize() )
//        {
//            m_vecNodes[iHandle_].iPrev = iLink_;
//            return true;
//        }//if
//        return false;
//    }//SetPrevLink()

    ///-----------------------------------------------------------------------------
    // use & return
    //
public:
    /// use a free node and set that node as 'used'.
    /// tail이 가리키는 곳에 새로운 노드를 할당하고 사용한다.
    /// 이 함수 호출전에 head가 -1이 아니었다면, 이 함수 호출후에도 head의 값은 변하지 않는다.
    /// @param  pbOutNewAlloc_: [out] 새로 할당한 노드이면 true가 채워진다.
    ///         free node list를 다시 재 사용한지 어떤지의 여부를 알 수 있다.
    /// @note   노드를 할당만 하고 초기화하지는 않는다.
    ///         반환된 핸들값을 이용하여 노드 초기화등의 작업을 한다.
    KHandle UseNode2( OUT bool& pbOutNewAlloc_ );
    KHandle UseNode2()
    {
        bool bTemp;
        return UseNode2( bTemp );
    }//UseNode2()

    /// use a free node and set that node as 'used'.
    /// tail이 가리키는 곳에 새로운 노드를 할당하고 사용한다.
    /// 이 함수 호출전에 head가 -1이 아니었다면, 이 함수 호출후에도 head의 값은 변하지 않는다.
    /// @param  tData_: node custom data
    /// @param  pbOutNewAlloc_: [out] 새로 할당한 노드이면 true가 채워진다.
    ///         free node list를 다시 재 사용한지 어떤지의 여부를 알 수 있다.
    /// @return node handle(index to this std::vector).
    KHandle UseNode( OUT bool& pbOutNewAlloc_, IN const T& tData_ );
    KHandle UseNode( IN const T& tData_ )
    {
        bool bTemp;
        return UseNode( bTemp, tData_ );
    }//UseNode()

    /// return a used node and set that node as 'free'.
    /// @param  iHandle_: node handle(index to this std::vector).
    /// @param  bClearNode_: clear node before return
    /// @return bool: true if succeeded
    bool ReturnNode( IN const KHandle& iHandle_, bool bClearNode_ = true );

    /// 노드의 key값을 설정한다.
    void SetKey( IN int iHandle_, IN const NKEY& nkey_ )
    {
        CSLockTemplate<t_isMultiThreaded> lock(m_cs);
        ASSERT( iHandle_ >= 0 && iHandle_ < GetSize() );
        m_vecNodes[iHandle_].nkey = nkey_;
    }//SetKey()

private:
    /// OBSOLETE!! - seojt on 2005-12-30, 9:38:15
    /// add a free node to the free node list.
    /// @param[in]  tData_: node data
    /// @return handle to node
    ///         -1 if failed
    //{{AFX: by jintaeks on 2009-2-26
    int _AddFreeNode( IN T tData_ ); // OBSOLETE!!
    //}}AFX: by jintaeks on 2009-2-26

public:
    /// return all used nodes.
    /// @return number of nodes returned
    int ReturnAllNodes();

    ///-----------------------------------------------------------------------------
    // iteration
    //
public:
    /// get next link of head of used node list.
    /** @example    GetHeadNext() iteration
        @code
            int iHandle;
            iHandle = GetHeadNext(); // get first used handle(==index)
            while ( -1 != iHandle )
            {
                // process '_GetHandleNode(iHandle).tData'
                iHandle = _GetHandleNode(iHandle).iNext; // visit next handle(==index)
            }//while
        @endcode
    */
    //inline int GetHeadNext() const { return m_iUsedHead; }
    /// head 노드에 대한 핸들을 얻는다.
    /// head 노드가 없다면(사용중인 노드가 하나도 없다면) -1을 리턴한다.
    //inline KHandle GetHead() const { return _MakeHandle(m_iUsedHead); }

    /// tail 노드에 대한 핸들을 리턴한다.
    /// tail 노드가 없다면(사용중인 노드가 하나도 없다면) -1을 리턴한다.
    //inline KHandle GetTail() const { return _MakeHandle(m_iUsedTail); }

    /** @example    iteration using GetFirst() and GetNext()
        KHandleArray<T>     harr;
        int iHandle = harr.GetFirst();
        while ( iHandle != -1 )
        {
            harr[iHandle].Test();
            iHandle = harr.GetNext( iHandle );
        }//while
    */
    inline KHandle GetFirst() const { return _MakeHandle(m_iUsedHead); }
    inline KHandle GetNext( IN const KHandle& iHandle_) const
    {
        return _MakeHandle(_GetHandleNode(iHandle_).iNext);
    }//GetNext()
    inline KHandle GetLast() const { return _MakeHandle(m_iUsedTail); }
    inline KHandle GetPrev( IN const KHandle& iHandle_) const
    {
        return _MakeHandle(_GetHandleNode(iHandle_).iPrev);
    }//GetPrev()

    /// unused free node의 첫번째 핸들을 구한다.
    /// unused free node들을 iterate하기 위해, GetNext()를 호출한다.
    /** @example    iterating free node list
        int iHandle = harr.GetFirstUnused();
        while ( iHandle != -1 )
        {
            harr[iHandle].Test();
            iHandle = harr.GetNext( iHandle ); // GetNextUnused()
        }//while
    */
    inline int GetFirstUnused() const { return m_iFreeHead; }
    
    // 이름의 일관성을 위해 추가한 함수들로, GetNext(), GetPrev()와 기능은 같다.
    //
    inline int GetNextUnused(int iHandle_) const { return _GetHandleNode(iHandle_).iNext; }
    inline int GetPrevUnused(int iHandle_) const { return _GetHandleNode(iHandle_).iPrev; }

    ///-----------------------------------------------------------------------------
    // helpers
    //
public:
    /// get number of free nodes.
    inline int GetNumUnused() const { return m_uiNumFree; }

    /// get number of used nodes.
    inline int GetNumUsed() const { return m_uiNumUsed; }

    /// find template data.
    /// @return handle to 'tData' if found
    ///         -1 if not found
    /// @NOTE   time complexity is O(n), where 'n' is the number of used nodes
    KHandle FindData( IN const T& tData_ );

    /// check whether iHandle_ is in valid range without it's used/free state.
    inline bool IsValidHandleRange(int iHandle_) const
    {
        return iHandle_ >= 0 && iHandle_ < GetSize();
    }//IsValidHandleRange()

    inline bool IsValidHandleRange( IN const KHandle& iHandle_ ) const
    {
        return iHandle_.GetHandlePart() >= 0 && iHandle_.GetHandlePart() < GetSize();
    }//IsValidHandleRange()

    /// check whether 'iHandle_' is valid and using.
    inline bool IsValidUsedHandle( IN int iHandle_ ) const
    {
        CSLockTemplate<t_isMultiThreaded> lock(m_cs);
        if( iHandle_ >= 0 && iHandle_ < GetSize() )
        {
            return m_vecNodes[iHandle_].IsUsedFlag();
        }
        else
        {
            return false;
        }//if.. else..
    }//IsValidUsedHandle()

	// added by wonpok. 20090820.
	inline bool IsValidUsedHandle( IN int iHandle_, IN UINT uiMagic_ ) const
	{
		CSLockTemplate<t_isMultiThreaded> lock(m_cs);
		if( iHandle_ >= 0 && iHandle_ < GetSize() )
		{
			return m_vecNodes[iHandle_].IsUsedFlag() && m_vecNodes[iHandle_].uiMagic == uiMagic_;
		}
		else
		{
			return false;
		}//if.. else..
	}

    inline bool IsValidUsedHandle( IN const KHandle& iHandle_ ) const
    {
        CSLockTemplate<t_isMultiThreaded> lock(m_cs);
        if( iHandle_.GetHandlePart() >= 0 && iHandle_.GetHandlePart() < GetSize() )
        {
            return m_vecNodes[iHandle_.GetHandlePart()].IsUsedFlag() 
                && m_vecNodes[iHandle_.GetHandlePart()].uiMagic == iHandle_.GetMagicPart();
        }
        else
        {
            return false;
        }//if.. else..
    }//IsValidUsedHandle()

    /// check whether 'iHandle_' is valid and free.
    bool IsValidUnusedHandle(int iHandle_) const
    {
        if ( iHandle_ >= 0 && iHandle_ < GetSize() )
        {
            return m_vecNodes[iHandle_].IsUsedFlag() == false;
        }
        else
        {
            return false;
        }//if.. else..
    }//KHandleArray<T,t_iNumInitialReserve>::IsValidUnusedHandle()

    /// 다음에 사용될 후보 핸들을 구한다.
    /// @note   time complexity is O(1)
    int GetCandidateHandle() const;

    ///-----------------------------------------------------------------------------
    // debug functions
    //
public:
    /// dump used nodes to debug console.
    void Debug_DumpUsedNodes();

    /// dump free nodes to debug console.
    void Debug_DumpFreeNodes();

    /// dump used and free nodes to debug console.
    void Debug_DumpNodes();
};//class KHandleArray


//static UINT s_uiMagicNumber;
KHandleArrayT UINT KHandleArrayC::s_uiMagicNumber;
//static KHandle  s_invalidHandle;
KHandleArrayT KHandle KHandleArrayC::s_invalidHandle;


///-----------------------------------------------------------------------------
// members for KHandleArray
//
// constructor.
KHandleArrayT KHandleArrayC::KHandleArray()
{
    _Init();
    m_fpPostReturnNode = NULL;
    Setup();
}//KHandleArray<T,t_iNumInitialReserve>::::KHandleArray()


// destructor.
KHandleArrayT KHandleArrayC::~KHandleArray()
{
    Clear();
}//KHandleArray<T,t_iNumInitialReserve>::~KHandleArray()


KHandleArrayT void KHandleArrayC::_Init()
{
    m_iFreeTail = -1;
    m_uiNumFree = 0;
    m_iUsedTail = -1;
    m_uiNumUsed = 0;

    //m_freeHead.Clear();
    m_iFreeHead = -1;
    //m_usedHead.Clear();
    m_iUsedHead = -1;
    ASSERT( GetSize() == 0 );
}//KHandleArray<T,t_iNumInitialReserve>::_Init()


KHandleArrayT void KHandleArrayC::Clear()
{
    CSLockTemplate<t_isMultiThreaded> lock(m_cs);

    m_vecNodes.clear();
    _Init();
}//KHandleArray<T,t_iNumInitialReserve>::Clear()


KHandleArrayT void KHandleArrayC::Setup()
{
    CSLockTemplate<t_isMultiThreaded> lock(m_cs);

    ASSERT( 0 == m_uiNumUsed );
    ASSERT( 0 == m_uiNumFree );
    m_vecNodes.reserve( t_iNumInitialReserve );

    // set-up doubly linked list
    //
    m_iFreeHead = -1;
    m_iFreeTail = -1;//&m_freeHead;
    m_uiNumFree = 0;
    m_iUsedHead = -1;
    m_iUsedTail = -1;//&m_usedHead;
    m_uiNumUsed = 0;
}//KHandleArray<T,t_iNumInitialReserve>::Setup()


///-----------------------------------------------------------------------------
// use & return
//
KHandleArrayT KHandle KHandleArrayC::UseNode2( OUT bool& pbOutNewAlloc_ )
{
    CSLockTemplate<t_isMultiThreaded> lock(m_cs);

    // increase magic number
    //
    ++s_uiMagicNumber;
    if ( s_uiMagicNumber == 0 )
        s_uiMagicNumber = 1;

    int     iRetUID         = -1;
    int     iSavedUsedTail  = m_iUsedTail; // current used end node

    // if there is no free node.
    if ( -1 == m_iFreeHead )
    {
        NODE_TYPE   node;

        // insert a new node
        //
        //node.tData   = tData_; // commented on jintaeks on 2007-08-06, 11:21
        node.uiMagic = s_uiMagicNumber;
        m_vecNodes.push_back( node );
        pbOutNewAlloc_ = true;

        // set UID, previous and next link of new node.
        //
        int iNewNode = GetSize() - 1;
        ASSERT( iNewNode >= 0 );
//        _AccessHandleNode(iNewNode).iHandle   = iNewNode;
        _AccessHandleNode(iNewNode).iPrev     = iSavedUsedTail;
        _AccessHandleNode(iNewNode).iNext     = -1;

        // save [return] handle
        iRetUID = iNewNode;

        // set new tail used node.
        m_iUsedTail = iNewNode;

        // update next link of previous used node
        //
        if ( -1 == m_iUsedHead )
        {
            // first node
            ASSERT( iNewNode == 0 );
            m_iUsedHead = iNewNode;
        }
        else
        {
            // update object node link
            ASSERT( iSavedUsedTail >= 0 && iSavedUsedTail < GetSize() );
            _AccessHandleNode(iSavedUsedTail).iNext = iNewNode;
        }//if.. else..
    }
    else // if there is some free node(s)
    {
        ASSERT( m_iFreeTail >= 0 && m_iFreeTail < GetSize() );
        int iSavedFreeTail       = m_iFreeTail;
        int iPrevOfSavedFreeTail = _GetHandleNode(m_iFreeTail).iPrev;

        // update tail free node('m_iFreeTail')
        //
        if ( -1 == iPrevOfSavedFreeTail ) // if previous node of free tail is head
        {
            m_iFreeHead = -1;
            m_iFreeTail = -1;
        }
        else
        {
            m_iFreeTail = iPrevOfSavedFreeTail;//&_GetHandleNode(iPrevOfSavedFreeTail);
            _AccessHandleNode(m_iFreeTail).iNext = -1; // cut off link
        }//if.. else..

        // update next link of tail used node
        //
        if ( -1 == m_iUsedHead ) // if there is no used node
        {
            m_iUsedHead = iSavedFreeTail;
        }
        else
        {
            ASSERT( m_iUsedTail >= 0 && m_iUsedTail < GetSize() );
            _AccessHandleNode(m_iUsedTail).iNext = iSavedFreeTail;
        }//if.. else..

        // update previous link of new used tail node
        //
        ASSERT( iSavedFreeTail >= 0 && iSavedFreeTail < GetSize() );
        m_iUsedTail            = iSavedFreeTail;
        //_AccessHandleNode(m_iUsedTail).tData  = tData_; // commented on jintaeks on 2007-08-06, 11:21
        _AccessHandleNode(m_iUsedTail).iPrev  = iSavedUsedTail;
        ASSERT( -1 == _GetHandleNode(m_iUsedTail).iNext );
        _AccessHandleNode(m_iUsedTail).uiMagic  = s_uiMagicNumber;

        --m_uiNumFree; // decrease number of free node
        iRetUID = m_iUsedTail; // save return handle

        pbOutNewAlloc_ = false;
    }//if.. else..

    ++m_uiNumUsed; // increase number of free node
    _AccessHandleNode(iRetUID).SetUsedFlag(); // set used flag

    return KHandle( iRetUID, _AccessHandleNode(iRetUID).uiMagic );
}//KHandleArray<T,t_iNumInitialReserve>::UseNode2()


KHandleArrayT KHandle KHandleArrayC::UseNode( OUT bool& pbOutNewAlloc_, const T& tData_ )
{
    CSLockTemplate<t_isMultiThreaded> lock(m_cs);

    KHandle iRetUID = UseNode2( pbOutNewAlloc_ );
    ASSERT( IsValidUsedHandle(iRetUID) );
    _AccessHandleNode(iRetUID.GetHandlePart()).tData  = tData_;

    return iRetUID;
}//KHandleArray<T,t_iNumInitialReserve>::UseNode()


KHandleArrayT bool KHandleArrayC::ReturnNode( IN const KHandle& iHandle_, bool bClearNode_ )
{
    CSLockTemplate<t_isMultiThreaded> lock(m_cs);

    ASSERT( iHandle_.GetHandlePart() >= 0 && iHandle_.GetHandlePart() < GetSize() );
    ASSERT( _GetHandleNode(iHandle_.GetHandlePart()).IsUsedFlag() );

    // magic number를 증가시켜서 이 노드가 invalidate되었다고 mark한다.
    //
    ++s_uiMagicNumber;
    if ( s_uiMagicNumber == 0 )
        s_uiMagicNumber = 1;
    _AccessHandleNode(iHandle_.GetHandlePart()).uiMagic = s_uiMagicNumber;

    int iPrevNode = _GetHandleNode(iHandle_.GetHandlePart()).iPrev;
    int iNextNode = _GetHandleNode(iHandle_.GetHandlePart()).iNext;

    // update next link of previous node
    //
    if ( -1 == iPrevNode ) // if previous node is head
    {
        m_iUsedHead = iNextNode;
    }
    else
    {
        ASSERT( iPrevNode >= 0 && iPrevNode < GetSize() );
        _AccessHandleNode(iPrevNode).iNext = iNextNode;
    }//if.. else..

    int iSavedFreeTail       = m_iFreeTail;
    int iNextOfSavedFreeTail = _GetNextLink( m_iFreeTail );

    // update previous link of used next node
    //
    if ( iNextNode >= 0 )
    {
        ASSERT( iNextNode < GetSize() );
        _AccessHandleNode(iNextNode).iPrev = iPrevNode;
    }//if

    // update next link of free previous node
    //
    if ( -1 == m_iFreeHead ) // if free node is empty
    {
        m_iFreeHead = iHandle_.GetHandlePart();
    }
    else
    {
        ASSERT( iSavedFreeTail >= 0 && iSavedFreeTail < GetSize() );
        _AccessHandleNode(iSavedFreeTail).iNext = iHandle_.GetHandlePart();
    }//if.. else..

    m_iFreeTail = iHandle_.GetHandlePart();

    // if new tail free node is tail used node, update tail used node
    //
    if ( m_iFreeTail == m_iUsedTail )
    {
        if ( -1 == iPrevNode )
        {
            m_iUsedTail = -1;
        }
        else
        {
            ASSERT( iPrevNode >= 0 && iPrevNode < GetSize() );
            m_iUsedTail = iPrevNode;
        }//if.. else..
    }//if

    ASSERT( m_iFreeTail >= 0 && m_iFreeTail < GetSize() );
    if ( bClearNode_ )
        _AccessHandleNode(m_iFreeTail).tData = NULL;
    _AccessHandleNode(m_iFreeTail).ClearUsedFlag();
    _AccessHandleNode(m_iFreeTail).iPrev  = iSavedFreeTail;
    ASSERT( -1 == iNextOfSavedFreeTail );
    _AccessHandleNode(m_iFreeTail).iNext  = iNextOfSavedFreeTail;

    ++m_uiNumFree; // increase number of free node
    --m_uiNumUsed; // decrease number of used node

//{{ seojt // 2008-3-27, 12:26
    if ( m_fpPostReturnNode )
        m_fpPostReturnNode( iHandle_ );
//}} seojt // 2008-3-27, 12:26

    return true;
}//KHandleArray<T,t_iNumInitialReserve>::ReturnNode()


KHandleArrayT int KHandleArrayC::_AddFreeNode(T tData_)
{
    ASSERT( !"OBSOLETE - seojt on 2005-12-30, 9:40:05" );
    CSLockTemplate<t_isMultiThreaded> lock(m_cs);

    int             iRetUID         = -1;
    int             iSavedFreeTail  = m_iFreeTail; // current free end node
    NODE_TYPE       node;

    // insert a new node
    //
    node.tData = tData_;
    m_vecNodes.push_back( node );

    // set UID, previous and next link of new node.
    //
    int iNewNode = GetSize() - 1;
    ASSERT( iNewNode >= 0 );
//    _AccessHandleNode(iNewNode).iHandle   = iNewNode;
    _AccessHandleNode(iNewNode).iPrev     = iSavedFreeTail;
    _AccessHandleNode(iNewNode).iNext     = -1;

    // save [return] handle
    iRetUID = iNewNode;

    // set new free tail node.
    m_iFreeTail = iNewNode;

    // update next link of previous free node
    //
    if ( -1 == m_iFreeHead )
    {
        // first node
        ASSERT( iNewNode == 0 );
        m_iFreeHead = iNewNode;
    }
    else
    {
        // update object node link
        ASSERT( iSavedFreeTail >= 0 && iSavedFreeTail < GetSize() );
        _AccessHandleNode(iSavedFreeTail).iNext = iNewNode;
    }//if.. else..

    ++m_uiNumFree; // increase number of free node

    return iRetUID;
}//KHandleArray<T,t_iNumInitialReserve>::_AddFreeNode()


KHandleArrayT int KHandleArrayC::ReturnAllNodes()
{
    CSLockTemplate<t_isMultiThreaded> lock(m_cs);

    int iNumReturned = 0;
    KHandle iNext = MakeValidHandle( m_iUsedTail );
    while( iNext.IsValid() )
    {
//        ASSERT( iNext == _GetHandleNode(iNext).iHandle );
        ReturnNode( iNext );
        ++iNumReturned;
        iNext = MakeValidHandle( m_iUsedTail );
    }//while

    ASSERT( m_uiNumUsed == 0 );
    ASSERT( m_uiNumFree == (int)m_vecNodes.size() );
    return iNumReturned;
}//KHandleArray<T,t_iNumInitialReserve>::ReturnAllNodes()


///-----------------------------------------------------------------------------
// helpers
//
KHandleArrayT KHandle KHandleArrayC::FindData( IN const T& tData_ )
{
    CSLockTemplate<t_isMultiThreaded> lock(m_cs);

    int i;
    i = GetFirst().GetHandlePart();
    while( i != -1 )
    {
        if( tData_ == _GetHandleNode(i).tData )
        {
            return KHandle( i, _GetHandleNode(i).uiMagic );
        }//if

        i = _GetHandleNode(i).iNext;
    }//while

    return KHandle();
}//KHandleArray<T,t_iNumInitialReserve>::FindData()


KHandleArrayT int KHandleArrayC::GetCandidateHandle() const
{
    if ( -1 == m_iFreeHead )
    {
        return (int)m_vecNodes.size();
    }
    else
    {
        return m_iFreeTail;
    }//if.. else..
}//KHandleArray<T,t_iNumInitialReserve>::GetCandidateHandle()


///-----------------------------------------------------------------------------
// debug functions
//
KHandleArrayT void KHandleArrayC::Debug_DumpUsedNodes()
{
    CSLockTemplate<t_isMultiThreaded> lock(m_cs);

    ASSERT( sizeof(T) == sizeof(int) );
    TRACEA( "Used: " );
    int iNext = m_iUsedHead;
    while ( iNext != -1 )
    {
//        ASSERT( iNext == _GetHandleNode(iNext).iHandle );
        TRACEA( "%i[%i],", iNext, _GetHandleNode(iNext).tData );
        iNext = _GetHandleNode(iNext).iNext;
    }//while
    TRACEA( "\n" );
}//KHandleArray<T,t_iNumInitialReserve>::Debug_DumpUsedNodes()


KHandleArrayT void KHandleArrayC::Debug_DumpFreeNodes()
{
    CSLockTemplate<t_isMultiThreaded> lock(m_cs);

    TRACEA( "Free: " );
    int iNext = m_iFreeHead;
    while ( iNext != -1 )
    {
//        ASSERT( iNext == _GetHandleNode(iNext).iHandle );
        TRACEA( "%i[%i],", iNext, _GetHandleNode(iNext).tData );
        iNext = _GetHandleNode(iNext).iNext;
    }//while
    TRACEA( "\n" );
}//KHandleArray<T,t_iNumInitialReserve>::Debug_DumpFreeNodes()


KHandleArrayT void KHandleArrayC::Debug_DumpNodes()
{
    CSLockTemplate<t_isMultiThreaded> lock(m_cs);

    TRACEA( "[KHandleArray<T>] size=%i\n", m_vecNodes.size() );
    Debug_DumpUsedNodes();
    Debug_DumpFreeNodes();
}//KHandleArray<T,t_iNumInitialReserve>::Debug_DumpNodes()

#endif // _KHANDLEARRAY_H


/** @example    KHandleArray
    @code

  ///-----------------------------------------------------------------------------
  //
  1) simple type example

    #include "KJohnGen.h"
    #include <iostream>
    #include "KHandleArray.h"

    using namespace std;

    void main(int argc, char* argv[])
    {
        INIT_CRTDEBUG();

        KHandleArray<int>   harray;
        int                 iHandle[10000];
        int                 i;

        ///-----------------------------------------------------------------------------
        // general example
        //
        iHandle[0] = harray.UseNode( 100 );
        iHandle[1] = harray.UseNode( 200 );
        iHandle[2] = harray.UseNode( 300 );
        harray.ReturnNode( iHandle[1] );
        iHandle[3] = harray.UseNode( 400 );
        harray.ReturnNode( iHandle[0] );
        harray.Debug_DumpNodes();
        // debug output
        //
        //size=3
        //Used: 2[300],1[400],
        //Free: 0[0],

        ///-----------------------------------------------------------------------------
        // more example
        //
        for (i=0; i < 10; ++i)
        {
            iHandle[4+i] = harray.UseNode( 500 + i*100 );
        }//for
        harray.ReturnNode( iHandle[10] );
        harray.ReturnNode( iHandle[11] );
        harray.Debug_DumpNodes();
        // debug output
        //
        //size=12
        //Used: 2[300],1[400],0[500],3[600],4[700],5[800],6[900],7[1000],10[1300],11[1400],
        //Free: 8[0],9[0],

        ///-----------------------------------------------------------------------------
        // iteration
        //
        i = harray.GetHead();
        while ( i != -1 )
        {
            ASSERT( i == harray._GetHandleNode(i).iHandle );
            printf( "[%i]=%i,%i(%i,%i)\n",
                i, harray._GetHandleNode(i).iHandle,
                harray._GetHandleNode(i).tData, harray._GetHandleNode(i).iPrev, harray._GetHandleNode(i).iNext );
            i = harray._GetHandleNode(i).iNext;
        }//while
        harray.ReturnAllNodes();
        harray.Debug_DumpNodes();
        // debug output
        //
        //size=12
        //Used:
        //Free: 8[0],9[0],11[0],10[0],7[0],6[0],5[0],4[0],3[0],0[0],1[0],2[0],

        // console output
        //
        //[2]=2,300(-1,1)
        //[1]=1,400(2,0)
        //[0]=0,500(1,3)
        //[3]=3,600(0,4)
        //[4]=4,700(3,5)
        //[5]=5,800(4,6)
        //[6]=6,900(5,7)
        //[7]=7,1000(6,10)
        //[10]=10,1300(7,11)
        //[11]=11,1400(10,-1)
        //Press any key to continue
    }//main()


  ///-----------------------------------------------------------------------------
  //
  2) complex type example

    #include "KJohnGen.h"
    #include <iostream>
    #include "KHandleArray.h"
    #include "KSimpleString.h"

    using namespace std;

    void main(int argc, char* argv[])
    {
        INIT_CRTDEBUG();

        KHandleArray<KSimpleString80>  harray;

        int iHello;
        int iWorld;

        iHello = harray.UseNode( "hello" );
        iWorld = harray.UseNode( "world" );

        cout << harray.GetData(iHello)->GetBuffer() << endl;
    }//main()
    // console output
    //
    // hello


  ///-----------------------------------------------------------------------------
  //
  3) Magic number example

    #include "KJohnGen.h"
    #include <iostream>
    #include "KHandleArray.h"
    #include "KSimpleString.h"

    using namespace std;

    void main(int argc, char* argv[])
    {
        INIT_CRTDEBUG();

        KHandleArray<KSimpleString80>  harray;

        int iHello;
        int iWorld;
        int iHelloMagic;

        // save magic number if 'iHello' node
        //
        iHello = harray.UseNode( "hello" );
        iHelloMagic = harray._GetHandleNode(iHello).uiMagic; // save magic number
        iWorld = harray.UseNode( "world" );

        // return 'iHello' node
        //
        cout << harray.GetData(iHello)->GetBuffer() << endl;
        harray.ReturnNode( iHello );

        // use another 'iHello2' node
        // node is different but may have same handle to 'iHello'
        int iHello2;
        iHello2 = harray.UseNode( "hello2" );

        // check magic number of 'iHello' if required
        //
        if ( iHelloMagic == harray._GetHandleNode(iHello).uiMagic )
            cout << harray.GetData(iHello)->GetBuffer() << endl;
        else
            iHello = -1; // set invalid number if magic number is different
    }//main()
    // console output
    //
    // hello
    // Press any key to continue

    @endcode
*/
