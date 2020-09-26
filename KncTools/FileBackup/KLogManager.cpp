/** @file
@author cruise@kogsoft.com
*/

const static char* cvsid = "$Id: KLogManager.cpp,v 1.9 2003/02/14 09:04:28 cvs Exp $";

#include "KLogManager.h"
#include <sstream>

/** Default Constructor. */
KLogManager::KLogManager(void) : m_pvecBuffer(NULL), m_pFile(NULL), m_hEventLog(NULL)
{
    m_pvecBuffer = new std::vector<char>(BUFSIZ);
    m_apszEventString[0] = &(m_pvecBuffer->at(0));
    // make all defaults
    SetOutputMethod(ALL_LEVEL, F_FILE | F_DEBUG | F_CONSOLE);
    SetOutputPrefix(ALL_LEVEL, NULL);
    SetEventType(ALL_LEVEL, ET_INFORMATION);
    // special settings for level 0 and 1.
    SetOutputPrefix(0, "ERROR: ");
    SetOutputPrefix(1, "Warning: ");
    SetEventType(0, ET_ERROR);
    SetEventType(1, ET_WARNING);
    memset( m_szFileName, 0, sizeof(m_szFileName) );
}

/** Destructor. */
KLogManager::~KLogManager(void)
{
    EndFileOutput();
    EndEventLogOutput();
    if (m_pvecBuffer != NULL)
    {
        delete m_pvecBuffer;
        m_pvecBuffer = NULL;
    }
}

/** Begin log output to the file `pszFileName'.
@warning 해당 level의 F_FILE flag가 set되어 있어야 Log(...)의 결과가 해당 파일로 출력됨.
@param pszFileName [in] file name for log output
*/
void KLogManager::BeginFileOutput(const char* pszFileName)
{
    EndFileOutput();
    strcpy( m_szFileName, pszFileName );

    m_pFile = ::fopen(m_szFileName, "w");
}

/** End log output to the file. */
void KLogManager::EndFileOutput(void)
{
    if (m_pFile != NULL)
    {
        ::fclose(m_pFile);
        m_pFile = NULL;
    }
}

/** Begin log output to the system Event Log with program name.
@warning 해당 level의 F_EVENT flag가 set되어 있어야 Log(...)의 결과가 Event Log로 출력됨.
@param pszProgName [in] system Event Log에 등록할 program name
*/
void KLogManager::BeginEventLogOutput(const char* pszProgName)
{
    EndEventLogOutput();
    m_hEventLog = ::RegisterEventSource(NULL, pszProgName);
}

/** End log output to the system Event Log. */
void KLogManager::EndEventLogOutput(void)
{
    if (m_hEventLog != NULL)
    {
        ::DeregisterEventSource(m_hEventLog);
        m_hEventLog = NULL;
    }
}

/** Set the program name and register the Event Log source.
Log를 출력하는 프로그램의 이름을 등록하고, 동시에 EventLog를 출력하기
위해 시스템에 등록한다.
@param pszProgName [in] string for the program name
@warning system event log로의 호출을 위해서는 이 함수를 반드시 호출해야 한다.
@deprecated Use BeginEventLogOutput(...).
*/
void KLogManager::SetProgramName(const char* pszProgName)
{
    BeginEventLogOutput(pszProgName);
}

/** Set the output file for the log messages.
Log를 출력할 파일을 open한다.
@param pszFileName [in] string for the file name
@deprecated Use BeginFileOutput(...).
*/
void KLogManager::SetOutputFile(const char* pszFileName)
{
    BeginFileOutput(pszFileName);
}

/** Set the output method for a level.
하나의 log level에 대해서 log를 출력하는 방법을 설정한다.
<ul>
<li> F_NONE : 아무런 출력도 하지 않는다.
<li> F_CONSOLE : stderr 로 log를 출력 (console program에서만 작동함)
<li> F_DEBUG : (Visual Studio와 같은) Debugger 화면으로 출력
<li> F_FILE : 파일로 출력 (BeginFileOutput(...), EndFileOutput(...) 사이에서만 작동)
<li> F_EVENT : system event log로 출력 (BeginEventLogOutput(...),
                                        EndEventLogOutput(...) 사이에서만 작동)
<li> F_ALL : 모든 가능한 출력을 모두 설정
</ul>
초기에는 (F_CONSOLE | F_DEBUG | F_FILE) 로 설정되어 있다.
@param iLevel [in] ALL_LEVEL 또는 setting 하고자 하는 level (0 ~ NUM_LEVEL-1)
@param iFlag [in] F_NONE, F_ALL or ORing of { F_CONSOLE, F_DEBUG, F_FILE, F_EVENT }
*/
void KLogManager::SetOutputMethod(int iLevel, int iFlag)
{
    if (iLevel == ALL_LEVEL)
    {
        SetOutputMethod(0, NUM_LEVEL - 1, iFlag);
    }
    else if (iLevel >= 0 && iLevel < NUM_LEVEL)
    {
        m_bConOut[iLevel] = ((iFlag & F_CONSOLE) != 0);
        m_bDebugOut[iLevel] = ((iFlag & F_DEBUG) != 0);
        m_bFileOut[iLevel] = ((iFlag & F_FILE) != 0);
        m_bEventOut[iLevel] = ((iFlag & F_EVENT) != 0);
        m_bNeedOut[iLevel] = (m_bConOut[iLevel] || m_bDebugOut[iLevel] ||
                              m_bFileOut[iLevel] || m_bEventOut[iLevel]);
    }
}

/** Set the output method for a range of levels [iLevelStart..iLevelEnd].
@param iLevelStart,iLevelEnd [in] iLevelStart에서 iLevelEnd까지의 level들을 설정
@param iFlag [in] F_NONE, F_ALL or ORing of F_FILE, F_DEBUG, F_EVENT
@see SetOutputMethod(int iLevel, int iFlag)
*/
void KLogManager::SetOutputMethod(int iLevelStart, int iLevelEnd, int iFlag)
{
    if (iLevelStart < 0) iLevelStart = 0;
    if (iLevelEnd >= NUM_LEVEL) iLevelEnd = NUM_LEVEL - 1;
    bool bConOut = ((iFlag & F_CONSOLE) != 0);
    bool bDebugOut = ((iFlag & F_DEBUG) != 0);
    bool bFileOut = ((iFlag & F_FILE) != 0);
    bool bEventOut = ((iFlag & F_EVENT) != 0);
    bool bNeedOut = (bConOut || bDebugOut || bFileOut || bEventOut);
    for (int i = iLevelStart; i <= iLevelEnd; i++)
    {
        m_bConOut[i] = bConOut;
        m_bDebugOut[i] = bDebugOut;
        m_bFileOut[i] = bFileOut;
        m_bEventOut[i] = bEventOut;
        m_bNeedOut[i] = bNeedOut;
    }
}

/** Get the current output method for a given level.
@param iLevel [in] 묻고자 하는 level (0 ~ NUM_LEVEL-1)
@return ORing of F_CONSOLE, F_DEBUG, F_FILE, F_EVENT or 0 for invalid levels.
*/
int KLogManager::GetOutputMethod(int iLevel) const
{
    if (iLevel < 0 || iLevel >= NUM_LEVEL) return 0;
    return (m_bConOut[iLevel] ? F_CONSOLE : 0) |
           (m_bDebugOut[iLevel] ? F_DEBUG : 0) |
           (m_bFileOut[iLevel] ? F_FILE : 0) |
           (m_bEventOut[iLevel] ? F_EVENT : 0);
}

/** Set the output string prefix for a level.
특정 level의 log 메시지를 출력할 때, 항상 제일 앞에 어떤 prefix를
출력하게 할 때 사용한다. SetOutputPrefix(5, "--> ") 와 같은 설정은 level
5의 log message들은 log message들이 항상 "--> "를 제일 앞에 추가한
형태로 출력되도록 설정한다.
@warning pszPrefix는 내부 버퍼로 copy되지 않으므로, 설정에 사용된 string은
calling function에서 계속 유지하여야 한다.
@param iLevel [in] ALL_LEVEL 또는 setting 하고자 하는 level (0 ~ NUM_LEVEL-1)
@param pszPrefix [in] log message 출력 시에, 제일 앞에 올 prefix (예: "ERROR: "). 불필요시는 NULL
*/
void KLogManager::SetOutputPrefix(int iLevel, const char* pszPrefix)
{
    if (iLevel == ALL_LEVEL)
    {
        SetOutputPrefix(0, NUM_LEVEL - 1, pszPrefix);
    }
    else if (iLevel >= 0 && iLevel < NUM_LEVEL)
    {
        m_apszPrefix[iLevel] = pszPrefix;
        m_iPrefixLen[iLevel] = (pszPrefix == NULL) ? 0 : strlen(pszPrefix);
    }
}

/** Set the output string prefix for a range of levels [iLevelStart..iLevelEnd].
@warning pszPrefix는 내부 버퍼로 copy되지 않으므로, 설정에 사용된 string은
calling function에서 계속 유지하여야 한다.
@param iLevelStart,iLevelEnd [in] iLevelStart에서 iLevelEnd까지의 level들을 설정
@param pszPrefix [in] log message 출력 시에, 제일 앞에 올 prefix (예: "ERROR: "). 불필요시는 NULL
@see SetOutputPrefix(int iLevel, const char* pszPrefix)
*/
void KLogManager::SetOutputPrefix(int iLevelStart, int iLevelEnd, const char* pszPrefix)
{
    if (iLevelStart < 0) iLevelStart = 0;
    if (iLevelEnd >= NUM_LEVEL) iLevelEnd = NUM_LEVEL - 1;
    int iLen = (pszPrefix == NULL) ? 0 : strlen(pszPrefix);
    for (int i = iLevelStart; i <= iLevelEnd; i++)
    {
        m_apszPrefix[i] = pszPrefix;
        m_iPrefixLen[i] = iLen;
    }
}

/** Get the current output string prefix for a given level.
@warning return되는 pointer는 delete하면 안 됨.
@param iLevel [in] 묻고자 하는 level (0 ~ NUM_LEVEL-1)
@return current string prefix or NULL.
*/
const char* KLogManager::GetOutputPrefix(int iLevel) const
{
    return ((iLevel >= 0) && (iLevel < NUM_LEVEL)) ? m_apszPrefix[iLevel] : NULL;
}

/** Set the event type of system Event Log for a level.
시스템이 제공하는 Event Log에 등록시에 사용할 event type을 설정한다.
default 값은 level 0 와 level 1은 각각 ET_ERROR (EVENTLOG_ERROR_TYPE)와
ET_WARNING (EVENTLOG_WARNING_TYPE)이고, 나머지 level들은 모두
ET_INFORMATION (EVENTLOG_INFORMATION_TYPE) 이다.
@param iLevel [in] ALL_LEVEL 또는 setting 하고자 하는 level (0 ~ NUM_LEVEL-1)
@param wEventType [in] 설정하려는 event type: ET_ERROR, ET_WARNING, ET_INFORMATION
@see ::ReportEvent(...)
*/
void KLogManager::SetEventType(int iLevel, WORD wEventType)
{
    if (iLevel == ALL_LEVEL)
    {
        SetEventType(0, NUM_LEVEL - 1, wEventType);
    }
    else if (iLevel >= 0 && iLevel < NUM_LEVEL)
    {
        m_wEventType[iLevel] = wEventType;
    }
}

/** Set the event type of system Event Log for a range of levels [iLevelStart..iLevelEnd].
@param iLevelStart,iLevelEnd [in] iLevelStart에서 iLevelEnd까지의 level들을 설정
@param wEventType [in] 설정하려는 event type: ET_ERROR, ET_WARNING, ET_INFORMATION
@see ::ReportEvent(...)
*/
void KLogManager::SetEventType(int iLevelStart, int iLevelEnd, WORD wEventType)
{
    if (iLevelStart < 0) iLevelStart = 0;
    if (iLevelEnd >= NUM_LEVEL) iLevelEnd = NUM_LEVEL - 1;
    for (int i = iLevelStart; i <= iLevelEnd; i++)
    {
        m_wEventType[i] = wEventType;
    }
}

/** Get the current event type of system Event Log for a given level.
@param iLevel [in] 묻고자 하는 level (0 ~ NUM_LEVEL-1)
@return current event type of system Event Log or 0.
*/
WORD KLogManager::GetEventType(int iLevel) const
{
    return ((iLevel >= 0) && (iLevel < NUM_LEVEL)) ? m_wEventType[iLevel] : 0;
}

/** Internal function: iLevel에 대한 출력이 필요한지를 return. */
bool KLogManager::NeedOutput(int iLevel) const
{
    return (iLevel >= 0) && (iLevel < NUM_LEVEL) && m_bNeedOut[iLevel];
}

/** Internal function: m_pvecBuffer에 출력할 message를 만든다. */
void KLogManager::MakeMessage(int iLevel, const char* pszFormat, va_list vaList)
{
    int iOffset = 0;
    if (m_apszPrefix[iLevel] != NULL)
    {
        while (strlen(m_apszPrefix[iLevel]) + 1 > m_pvecBuffer->size() - iOffset)
        {
            m_pvecBuffer->resize(m_pvecBuffer->size() + BUFSIZ);
        }
        strcpy(&(m_pvecBuffer->at(iOffset)), m_apszPrefix[iLevel]);
        iOffset = m_iPrefixLen[iLevel];
    }
    while (::_vsnprintf(&(m_pvecBuffer->at(iOffset)), m_pvecBuffer->size() - iOffset,
                        pszFormat, vaList) < 0)
    {
        m_pvecBuffer->resize(m_pvecBuffer->size() + BUFSIZ);
    }
}

/** Internal function: m_pvecBuffer에 __FILE__, __LINE__에서 가져온 정보를 추가한다. */
void KLogManager::AppendMessage(const char* pszFile, int iLine)
{
    int iOffset = strlen(&(m_pvecBuffer->at(0)));
    while (::_snprintf(&(m_pvecBuffer->at(iOffset)), m_pvecBuffer->size() - iOffset,
                       " (%s,%d)", pszFile, iLine) < 0)
    {
        m_pvecBuffer->resize(m_pvecBuffer->size() + BUFSIZ);
    }
}

/** Internal function: m_pvecBuffer에 있는 message를 미리 설정된 output method들에 따라 출력한다. */
void KLogManager::OutputMessage(int iLevel)
{
    if (m_bConOut[iLevel] == true)
    {
        ::fputs(&(m_pvecBuffer->at(0)), stderr);
        ::fputs("\n", stderr);
        ::fflush(stderr);
    }
    if (m_bDebugOut[iLevel] == true)
    {
        ::OutputDebugString(&(m_pvecBuffer->at(0)));
        ::OutputDebugString("\n");
    }
    if (m_bFileOut[iLevel] == true && m_pFile != NULL)
    {
        ::fputs(&(m_pvecBuffer->at(0)), m_pFile);
        ::fputs("\n", m_pFile);
        ::fflush(m_pFile);
    }
    if (m_hEventLog != NULL && m_bEventOut[iLevel] == true)
    {
        m_apszEventString[0] = &(m_pvecBuffer->at(0));
        ::ReportEvent(m_hEventLog, m_wEventType[iLevel], 0, 0, NULL, 1, 0, m_apszEventString, NULL);
    }
}

/** Log message output, printf-style.
@param iLevel [in] 출력할 log message의 level (0 ~ NUM_LEVEL-1)
@param pszFormat [in] printf-style format string
@param ... [in] printf-style remaining parameters
@return NULL if no need to output, otherwise the output string
*/
const char* KLogManager::Log(int iLevel, const char* pszFormat, ...)
{
    if (! NeedOutput(iLevel)) return NULL;
    // make the string to the m_pvecBuffer.
    va_list vaList;
    va_start(vaList, pszFormat);
    MakeMessage(iLevel, pszFormat, vaList);
    va_end(vaList);
    // output the string
    OutputMessage(iLevel);
    // end
    return &(m_pvecBuffer->at(0));
}

/** Log message output, printf-style, with __FILE__ and __LINE__ information.
이 함수는 __FILE__, __LINE__ 정보를 Preset(...)에서 이미 저장했다고 가정한다.
@param iLevel [in] 출력할 log message의 level (0 ~ NUM_LEVEL-1)
@param pszFormat [in] printf-style format string
@param ... [in] printf-style remaining parameters
@return NULL if no need to output, otherwise the output string
*/
const char* KLogManager::LogL(int iLevel, const char* pszFormat, ...)
{
    if (! NeedOutput(iLevel)) return NULL;
    // make the string to the m_pvecBuffer.
    va_list vaList;
    va_start(vaList, pszFormat);
    MakeMessage(iLevel, pszFormat, vaList);
    va_end(vaList);
    // append file and line information
    AppendMessage(m_pszFile, m_iLine);
    // output the string
    OutputMessage(iLevel);
    // end
    return &(m_pvecBuffer->at(0));
}

/** String representation. */
std::string KLogManager::ToString(void) const
{
    std::ostringstream s;
    s << "KLogManager";
    for (int i = 0; i < NUM_LEVEL; i++)
    {
        s << std::endl;
        s << "  " << i;
        s << ": out=" << m_bNeedOut[i];
        s << ", con=" << m_bConOut[i];
        s << ", debug=" << m_bDebugOut[i];
        s << ", file=" << m_bFileOut[i];
        s << ", event=" << m_bEventOut[i];
        s << ", eventType=" << m_wEventType[i];
        if (m_apszPrefix[i] != NULL) s << ", prefix=\"" << m_apszPrefix[i] << "\"";
        else s << ", prefix=NULL";
    }
    return s.str();
}
