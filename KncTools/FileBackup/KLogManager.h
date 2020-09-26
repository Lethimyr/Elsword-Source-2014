/** @file
@author cruise@kogsoft.com
*/

#ifndef _KOG_KLOGMANAGER_H_
#define _KOG_KLOGMANAGER_H_ "$Id: KLogManager.h,v 1.8 2003/02/11 03:13:24 cvs Exp $"
#pragma once // for _MSC_VER > 1000

#include <string>
#include <cstdio>
#include <cstdarg>
#include <windows.h>
#include <vector>
#include "KSingleton.h"

/** Log Message Manager.
프로그램 작성 시에 출력이 필요한 log message들을 일관적으로 다루기 위한
manager class이다. 기본적으로, log message는 그 중요도에 따라, 0 (가장
중요) ~ NUM_LEVEL-1 (가장 덜 중요) 의 레벨을 가진다. level 0 와 level
1은 각각 error (시스템에 critical한 error를 의미)와 warning (사용자가 꼭
알아야 할 warning을 의미)으로 간주하고, 나머지 level들은 프로그래머가
디버깅이나 기타 목적으로 출력하고자 하는 log message라고 가정한다.

log message를 프로그래머에게 알리는 방법은 크게 4가지를 고려하였다.
<ul>
<li> 콘솔 출력 (console output, F_CONSOLE) : console program에서만
     출력이 일어나고, stderr로 log message를 출력한다.
<li> 디버거 출력 (debugger output, F_DEBUG) : MS Visual Studio
     환경에서는 디버거 창에 메시지를 출력한다.
<li> 파일 출력 (file output, F_FILE) : 어떠한 프로그램에서도
     사용가능하고, 지정된 파일에 메시지를 출력한다. 파일 이름은
     BeginFileOutput(...) 함수에서 설정한다.
<li> 시스템 이벤트 로그 (system Event Log, F_EVENT) : Windows 시스템의
     이벤트 관리자에게 메시지를 등록해 둔다. 이 경우는 다른 시스템에서
     remote로 접근해서 메시지를 보는 것도 가능하다. 등록할 메시지는 어느
     프로그램이 등록하는지, 프로그램 식별자가 필요한 데, 이 식별자는
     BeginEventLogOutput(...) 함수에서 설정한다.
</ul>
각 메시지는 대응되는 level에 따라, 어떤 출력 방법을 쓸 것인지 정할 수
있고, log를 생성할 때에는 어떤 level의 log인지에 따라, 해당 level에서
미리 지정된 출력 방법들에 따라, log 메시지를 출력한다.

기본 설정은 모든 level에서, 콘솔, 디버거, 파일 출력이 on 되고, 시스템
이벤트 출력은 off 되어 있다. 파일이나 시스템 이벤트 출력은 flag를 on
시키는 것 이외에, BeginFileOuput(...) 이나 BeginEventLogOutput(...)
함수의 호출을 필요로 한다.

메시지의 출력 형식은 <BLOCKQUOTE> (Prefix) (User-provided message)
[(FileName,LineNumber)] </BLOCKQUOTEIP> 형태를 가진다. <b>(Prefix)</b>는
해당 level의 메시지들이 공통적으로 가지는 prefix를 의미하고, level 0와
level 1에 대해서는 "ERROR: "와 "Warning: "을 기본값으로 가지고, 나머지
level들은 NULL string을 기본값으로 가진다. <b>(User-provided
message)</b>는 사용자가 실제로 출력하고자 하는 message로, printf-style의
함수들을 제공한다. 이들 함수들은 대응되는 long-type함수들 (함수 이름의
제일 끝에 'L'이 첨가된 함수들)을 가지고, 이들 long-type 함수들은 출력의
마지막 부분에 C preprocessor에서 제공하는 __FILE__과 __LINE__ 매크로
값을 추가로 출력한다.

@verbinclude testKLogManager.cpp
*/
class KLogManager
{
public:
    /** Default constructor. */
    _DLLAPI KLogManager(void);
    /** Destructor. */
    _DLLAPI ~KLogManager(void);
private:
    /** Copy constructor: DISABLED. */
    _DLLAPI KLogManager(const KLogManager&);
    /** Assignment operator: DISABLED. */
    _DLLAPI void operator=(const KLogManager&);

public:
    /** String representation. */
    _DLLAPI std::string ToString(void) const;

    // BEGIN / END FOR FILE AND SYSTEM EVENT LOG OUTPUT ****************

    /** Begin log ouptut to the file pszFileName. */
    _DLLAPI void BeginFileOutput(const char* pszFileName);
    /** End log output to the file. */
    _DLLAPI void EndFileOutput(void);
    /** Begin log output to the system Event Log with program name. */
    _DLLAPI void BeginEventLogOutput(const char* pszProgName);
    /** End log output to the system Event Log. */
    _DLLAPI void EndEventLogOutput(void);

    // OUTPUT METHOD ***************************************************

    /** Set the output method for a level. */
    _DLLAPI void SetOutputMethod(int iLevel, int iFlag);
    /** Set the output method for a range of levels [iLevelStart..iLevelEnd]. */
    _DLLAPI void SetOutputMethod(int iLevelStart, int iLevelEnd, int iFlag);
    /** Get the current output method for a given level. */
    _DLLAPI int GetOutputMethod(int iLevel) const;

    // OUTPUT PREFIX ***************************************************

    /** Set the output string prefix for a level. */
    _DLLAPI void SetOutputPrefix(int iLevel, const char* szPrefix);
    /** Set the output string prefix for a range of levels [iLevelStart..iLevelEnd]. */
    _DLLAPI void SetOutputPrefix(int iLevelStart, int iLevelEnd, const char* szPrefix);
    /** Get the current output string prefix for a given level. */
    _DLLAPI const char* GetOutputPrefix(int iLevel) const;

    // EVENT TYPE FOR SYSTEM EVENT LOG OUTPUT **************************

    /** Set the event type of system Event Log for a level. */
    _DLLAPI void SetEventType(int iLevel, WORD wEventType);
    /** Set the event type of system Event Log for a range of levels [iLevelStart..iLevelEnd]. */
    _DLLAPI void SetEventType(int iLevelStart, int iLevelEnd, WORD wEventType);
    /** Get the current event type of system Event Log for a given level. */
    _DLLAPI WORD GetEventType(int iLevel) const;

    // LOG MESSAGE OUTPUT **********************************************

    /** Log message output, printf-style. */
    _DLLAPI const char* Log(int iLevel, const char* szFormat, ...);
    /** Log message output, for a HRESULT value. */
    _DLLAPI const char* Log(int iLevel, HRESULT hr, ...);

    /** Preset __FILE__ and __LINE__ for LogL(...) functions. */
    _DLLAPI inline void Preset(const char* szFile, int iLine) { m_pszFile = szFile; m_iLine = iLine; }
    /** Log message output, printf-style, with __FILE__ and __LINE__ information. */
    _DLLAPI const char* LogL(int iLevel, const char* szFormat, ...);
    /** Log message output, for a HRESULT value, with __FILE__ and __LINE__ information. */
    _DLLAPI const char* LogL(int iLevel, HRESULT hr, ...);

    // DEPRECATED ******************************************************

    /** Set the program name and register the Event Log source. */
    _DLLAPI void SetProgramName(const char* szProgName);
    /** Set the output file for the log messages. */
    _DLLAPI void SetOutputFile(const char* szFileName);
private:
    /** Internal function: iLevel에 대한 출력이 필요한지를 return. */
    _DLLAPI bool NeedOutput(int iLevel) const;
    /** Internal function: m_aszBuffer에 출력할 message를 만든다. */
    _DLLAPI void MakeMessage(int iLevel, const char* szFormat, va_list vaList);
    /** Internal function: m_aszBuffer에 __FILE__, __LINE__에서 가져온 정보를 추가한다. */
    _DLLAPI void AppendMessage(const char* szFile, int iLine);
    /** Internal function: m_aszBuffer에 있는 message를 output method들에 따라 출력한다. */
    _DLLAPI void OutputMessage(int iLevel);
public:
    enum
    {
        ALL_LEVEL = -1,		///< When used for level, indicating all levels.
        NUM_LEVEL = 10,		///< Number of valid levels.
        // flags for SetOutputMethod(...)
        F_NONE = 0,		///< No output method.
        F_CONSOLE = 0x01,	///< Console output method is turning on.
        F_DEBUG = 0x02,		///< Debugger window output method is turning on.
        F_FILE = 0x04,		///< File output method is turning on.
        F_EVENT = 0x08,		///< Event Log output method is turning on.
        F_ALL = F_FILE | F_DEBUG | F_EVENT | F_CONSOLE, ///< Turning on all output methods.
        // event types for SetEventType(...)
        ET_ERROR = EVENTLOG_ERROR_TYPE,
        ET_WARNING = EVENTLOG_WARNING_TYPE,
        ET_INFORMATION = EVENTLOG_INFORMATION_TYPE,
    };
private:
    std::vector<char>* m_pvecBuffer; ///< Main buffer for printf-style processing.
    const char* m_apszEventString[1]; ///< Directing m_aszBuffer for ::ReportEvent(...).
    const char* m_apszPrefix[NUM_LEVEL]; ///< Prefix strings for each level.
    int m_iPrefixLen[NUM_LEVEL]; ///< Length of prefix strigns for each level.
    bool m_bNeedOut[NUM_LEVEL]; ///< True iff a level needs any output.
    bool m_bConOut[NUM_LEVEL]; ///< True iff a level needs console output.
    bool m_bDebugOut[NUM_LEVEL]; ///< True iff a level needs debugger window output.
    bool m_bFileOut[NUM_LEVEL]; ///< True iff a level needs file output.
    bool m_bEventOut[NUM_LEVEL]; ///< True iff a level needs system event log output.
    WORD m_wEventType[NUM_LEVEL]; ///< Event type for system event log.
    FILE* m_pFile; ///< File pointer for the output.
    HANDLE m_hEventLog; ///< Handle for system event log.
    const char* m_pszFile; ///< Temporary storage for __FILE__
    int m_iLine; ///< Temporary storage for __LINE__
    char m_szFileName[_MAX_FNAME];
};

#if ! defined(KLOG_OFF)
#define pSKLogManager KSingleton<KLogManager>::GetInstance() 
/** Log message output, printf-style.
매크로 함수로 정의되고, printf-style로 쓰기 때문에, 부를 때는 반드시
KLOG((1, "message %d", number)) 와 같이, 두 개의 괄호로 둘러 싸야 한다.
*/
#define KLOG(x)  pSKLogManager->Log x 
/** Log message output, printf-style, with __FILE__ and __LINE__ information.
매크로 함수로 정의되고, printf-style로 쓰기 때문에, 부를 때는 반드시
KLOGL((1, "message %d", number)) 와 같이, 두 개의 괄호로 둘러 싸야 한다.
*/
#define KLOGL(x) pSKLogManager->Preset(__FILE__, __LINE__); pSKLogManager->LogL x
#else
#define pSKLogManager KSingleton<KLogManager>::GetInstance()
#define KLOG(x)  ((void)0)
#define KLOGL(x) ((void)0)
#endif

#endif // _KOG_KLOGMANAGER_H_
