/** @file   KFileVerifier.h
    @desc   interface of KFileVerifier
            백그라운드로 file들의 checksum을 검사한다.
            엘소드 패쳐의 속도 개선을 도와주기 위해 서진택이 처음으로 작성하다.
    @author seojt@kogstudios.com
    @since  2008-1-5, 12:12

    @date   jintaeks on 2008-01-05, 15:01
            - 구현시작
            - 구현 완료
                --  thread에서 checksum을 검사하고, hack 리스트를 만드는 것 까지 구현
    @date   jintaeks on 2008-01-06, 18:27
            -   checksum 검사가 끝나기 전에 다른 디스크 IO작업이 시작될 수 있으므로,
                thread를 suspend/resume 시키는 기능을 추가하는 작업을 시작
*/

#ifndef _KFILEVERIFIER_H
#define _KFILEVERIFIER_H

//#include "AssertTrace.h"
#include <string>
#include <map>
#include <vector>
#include "KJohnThread.h"
#include "CSLock.h"

//{{ robobeg : 2008-05-02
#include "KStringUtil.h"
//}} robobeg : 2008-05-02


///-----------------------------------------------------------------------------
/// @class  KFileInfo
/// @brief  KFileVerifier가 처리하는 하나의 파일에 대한 정보를 나타내며,
///         std::map<>의 노드로 사용한다.
///
struct KFileInfo
{
public:
    std::string     m_strFilename;  ///< 파일 이름
    DWORD           m_dwSize;       ///< 파일 크기(단위: byte)
    USHORT          m_usVersion;    ///< 파일 버전
    std::string     m_strCheckSum;  ///< 파일 binary의 checksum

public:
    /// constructor.
    KFileInfo()
    {
        m_strFilename.clear();
        m_dwSize        = 0L;
        m_usVersion     = 0;
        m_strCheckSum.clear();
    }//KFileInfo()

    /** KFileInfo구조체는 std::map<>의 노드로 insert되므로
        복사 생성자와 operator=()을 overload해 주어야 한다. */

    /// copy constructor.
    KFileInfo(const KFileInfo& rhs_)
    {
        m_strFilename   = rhs_.m_strFilename;
        m_dwSize        = rhs_.m_dwSize;
        m_usVersion     = rhs_.m_usVersion;
        m_strCheckSum   = rhs_.m_strCheckSum;
    }//KFileInfo()

    KFileInfo& operator=(const KFileInfo& rhs_)
    {
        m_strFilename   = rhs_.m_strFilename;
        m_dwSize        = rhs_.m_dwSize;
        m_usVersion     = rhs_.m_usVersion;
        m_strCheckSum   = rhs_.m_strCheckSum;

        return *this;
    }//operator=()
};//struct KFileInfo

//{{ robobeg : 2008-05-02
// 맵에서 파일명의 대소문자 구분없이 비교하기 위해 수정

//typedef std::map<std::string, KFileInfo>    FileInfoMap;
typedef std::map<std::string, KFileInfo, KCaseInsensitiveLessA>    FileInfoMap;
//}} robobeg : 2008-05-02
typedef std::vector<KFileInfo>              FileInfoArr;


// thread를 종료하기 직전에 호출하는 사용자 함수의 protype에 대한 타입 정의
typedef void (*PREENDTHREAD_CALLBACK)(DWORD dwUserParam_, void* pFileVerifierObject_);


/** @class  KFileVerifier
    @brief  백그라운드로 file들의 checksum을 검사한다.
            엘소드 패쳐의 속도 개선을 도와주기 위해 서진택seojt@kogstudios.com이 처음으로 작성하다.
            - jintaeks on 2008-01-05, 12:21
    @see    CKTDXThread, tinyxml.h, CRC_32.h|cpp

    @desc   [전체적인 동작 구현 전 계획] - jintaeks on 2008-01-05, 12:20

            1)  게임 시작 폴더에서 Ver.xml 파일을 읽어서 checksum을 검사할 파일들의 리스트를 맵
                으로 만든다.
            2)  로그인 후 처음 한번 리소스를 로드하는 시점 혹은 로드가 끝난 후,
                verifier thread를 실행한다.
                    -   thread 구현은 KTDSLIB에 포함된 CKTDXThread 클래스(KTDXThread.h|cpp)를
                        사용한다.
                (파일을 읽으므로 CPU bound하지는 않으나, 전반적으로 frame이 떨어질 것이다)
            3)  verify가 실패하면 윈도우 메시지를 날려서 verify가 실패했음을 알린다.
            4)  3) 까지의 구현은 파일에 대해서만 가능하므로, .kom에 압축된 파일들의 checksum
                검사를 위해 별도의 파일 리스트를 유지한다.
                이 파일들은 .kom에서 파일을 읽기 위해, 처음 접근할 때 checksum 검사를 해 주어야
                하는데, 처음 접근하는 곳에서 특별한 코드를 삽입할 지, 관련 함수를 hook해서
                자동으로 처리할지는 고민중이다.

            [구현 후 동작 정리]

*/
class KFileVerifier : public KJohnThread
{
private:
    char                    m_szProgramPath[1024];  ///< 게임 client가 위치한 곳의 경로. 끝에 \\를 포함
    FileInfoMap             m_fileMap;              ///< 기존의 <File> 섹션에 명시된 파일의 리스트
    /** .kom 파일 전체에 대해서 checksum을 비교하는 것이 효율적이지 않다고 판단되면,
        .kom 파일에 package된 파일에 대해서 checksum 비교를 시도해야 하는데,
        그러한 정보는 Ver.xml에 별도의 섹션에 넣어두고,
        thread에서 .kom 파일을 unpack하여 checksum 검사를 하도록 논리를 구성해야 한다.

        @note   현재 - jintaeks on 2008-01-06, 18:25 - 사용하지 않는다.

        - jintaeks on 2008-01-06, 18:24 */
    FileInfoMap             m_komFileMap;           ///< 새롭게 <KomFile> 섹션에 명시된 파일의 리스트

    /** thread가 종료되었을 때 m_hackFileArr에 있는 파일들은 checksum이 일치하지 않는 hack된
        파일일 것이다. */
    FileInfoArr             m_hackFileArr;          ///< 해킹된(checksum이 일치하지 않는) 파일 배열

    // thread 관련 --------------------------------------------------
    //
    HANDLE                  m_hThreadEvent;         ///< thread의 suspend/resume을 위한 event
    bool                    m_bThreadSuspended;
    MemberCriticalSection   m_cs;                   ///< CRITICAL_SECTION의 wrapper
    DWORD                   m_dwThreadUserParam;    ///< 'm_fpPreEndThreadCallback'의 첫번재 파라미터
    PREENDTHREAD_CALLBACK   m_fpPreEndThreadCallback;   ///< thread를 종료하기 직전에 호출하는 사용자 정의 콜백 함수
    bool                    m_bForceExistThread;    ///< thread를 강제로 종료시켜야 하는 경우 이 값을 참으로 만든다.
        /// checksum 검사가 완료하지 않았지만, 사용자가 게임을 빠져나가는 경우등에 사용한다.

public:
    /// constructor.
    KFileVerifier();
    /// destructor.
    virtual ~KFileVerifier();

    ///-----------------------------------------------------------------------------
    // thread related functions
    //
public:
    /// @note   override KJohnThread::RunThread()
    /// 메인 thread 함수이다. 'm_fileMap'에 설정된 파일의 checksum을 검사하고,
    /// 결과를 'm_hackFileArr'에 저장한다. thread는 종료하기 직전에,
    /// 'm_fpPreEndThreadCallback' 콜백함수를 호출한다.
    virtual DWORD RunThread();

    /// end thread 콜백함수를 설정하고, thread를 시작한다.
    /// @param  pPreEndThreadCallback_: [in] RunThread() 가 종료하기 직전에 호출하는 사용자 함수
    /// @param  dwUserParam_: [in] pPreEndThreadCallback_의 첫번째 파라미터로 전달되는 값
    /// @return bool: 
    bool BeginThreadWithPreEndCB(PREENDTHREAD_CALLBACK pPreEndThreadCallback_, DWORD dwUserParam_=0L);

    /// thread에 필요한 event 객체를 생성한다.
    void _InitializeObject();

    /// thread에 사용된 event 객체를 파괴한다.
    void _FinalizeObject();

    /// thread를 종료하기 직전에 호출하는 사용자 정의 콜백 함수를 설정한다.
    /// @param  dwUserParam_: pPreEndThreadCallback_()함수의 첫번째 파라미터로 전달되는 custom 값
    /// @note   pPreEndThreadCallback_의 두번재 파라미터는 이 함수를 호출한 객체의 시작 주소이다.
    ///         즉 this가 전달된다.
    void _SetPreEndThreadCallback(PREENDTHREAD_CALLBACK pPreEndThreadCallback_, DWORD dwUserParam_=0L);

    /// thread를 suspend 시킨다.
    void SuspendThread();

    /// thread를 resume시킨다.
    void ResumeThread();

    /// thread가 suspend되어 있는 경우 true를 리턴한다.
    bool IsThreadSuspended() const { return m_bThreadSuspended; }

    ///-----------------------------------------------------------------------------
    // KFileVerifier interfaces
    //
public:
    /// Ver.xml 파일에서 파일 리스트를 읽어서 맵에 구성한다.
    /// @param  pszFullPathFilename_: 경로를 포함한 Ver.xml 파일 이름
    /// @return bool: 성공하면 true를 리턴한다.
    ///                 실패하면 false를 리턴한다.
    bool LoadFileList(const char* pszFullPathFilename_);

    /// 컨테이너의 내용을 모두 clear한다.
    void Clear();

    /// hack된 파일(checksum일 일치하지 않는 파일)의 개수를 리턴한다.
    inline int GetNumHackFiles() const { return (int)m_hackFileArr.size(); }

    ///-----------------------------------------------------------------------------
    // debug functions
    //
public:
    /// container의 내용을 디버그 화면에 출력한다.
    void Debug_DumpToOutputWindow();

    /// 'm_hackFileArr'의 내용을 디버그 덤프한다.
    void Debug_DumpHackFiles();
};//class KFileVerifier

#endif // _KFILEVERIFIER_H


/** @example    class KFileVerifier

  1) checksum을 모두 검사하고 thread를 종료하기 직전의 callback 함수 작성

    void PreEndThreadCallback(DWORD dwUserParam_, void* pFileVerifierObject_)
    {
        ASSERT( pFileVerifierObject_ );
        KFileVerifier*  pFileVerifier = reinterpret_cast<KFileVerifier*>( pFileVerifierObject_ );
        ASSERT( pFileVerifier );
        pFileVerifier->Debug_DumpHackFiles();
    }//PreEndThreadCallback()

  2) CKTDXApp의 생성자에서 객체를 만들고, thread 시작

    m_pFileVerifier     = new KFileVerifier();
    ASSERT( m_pFileVerifier );
    m_pFileVerifier->LoadFileList( "Ver.xml" );
    m_pFileVerifier->Debug_DumpToOutputWindow();
    m_pFileVerifier->_SetPreEndThreadCallback( PreEndThreadCallback );
    m_pFileVerifier->BeginThread();

  3) CKTDXApp의 파괴자에서 객체 파괴

    ASSERT( m_pFileVerifier );
    if ( m_pFileVerifier )
    {
        m_pFileVerifier->EndThread( 1000 );
        SAFE_DELETE( m_pFileVerifier );
    }//if

*/
