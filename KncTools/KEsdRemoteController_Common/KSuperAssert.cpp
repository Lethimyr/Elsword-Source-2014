// Precompile Header
#include "stdafx.h"

#include "KSuperAssert.h"
#ifdef USE_BUGSLAYER
#include "BugslayerUtil.h"
#endif // USE_BUGSLAYER

#ifdef USE_CONSOLE
#include "KConsole.h"
#endif // USE_CONSOLE

#ifdef USE_REMOTELOGGER
#include "KRemoteLogger.h"
#endif // USE_REMOTELOGGER

#ifdef SUPERASSERT_EMAIL
#undef SUPERASSERT_EMAIL
#define SUPERASSERT_EMAIL "msdn@kogstudios.com; jsg0425@kogstudios.com"
#endif

#include <assert.h>
#include <time.h>

KSuperAssert g_kSuperAssert;

//---------------------------------------------------------------------------
KSuperAssert::KSuperAssert()
{
	StartGlobalLog();

	// Lock
	m_bLockGlobalFileStream[0] = false;
	m_bLockGlobalFileStream[1] = false;
	m_bLockLogFileStream = false;
	m_bLockConsole = false;
	m_bLockOutputDebugString = false;
}
//---------------------------------------------------------------------------
KSuperAssert::~KSuperAssert()
{
	unsigned int iG;
	for (iG = 0; iG < KGLOBALLOGBUFSIZE; ++iG)
	{
		if (m_kGlobalFileStream[iG].is_open())
			m_kGlobalFileStream[iG].close();
	}
}
//---------------------------------------------------------------------------
KSuperAssert& KSuperAssert::SetAssertInfo(const char* expr, 
										  unsigned int line, const char* func, const char* file)
{
	m_strExpression = expr;
	m_strFilename = file;
	m_strFuncname = func;
	m_uiLine = line;

	std::ostringstream os;
	os
		<< "[Assertion]" << std::endl
		<< "File : " << file << std::endl
		<< "Func : " << func << std::endl
		<< "Line : " << line << std::endl
		<< "Expr : " << expr << std::endl << std::endl;

	m_strBuf += os.str().c_str();

	return *this;
}
//---------------------------------------------------------------------------
KSuperAssert& KSuperAssert::_initBuf()
{
	m_strBuf.clear();
	m_strInfo.clear();

	return *this;
}
//---------------------------------------------------------------------------
bool KSuperAssert::_fatal(int sIgnoreCount, int sFailCount, DWORD dwStack, DWORD dwStackFrame)
{
	m_strBuf += m_strInfo;

#ifdef USE_CONSOLE
	KCon << rcol << m_strBuf.c_str();
#endif // USE_CONSOLE

#ifdef USE_BUGSLAYER
	if (TRUE == SuperAssertion( TEXT ( "Assertion" ),
		(m_strExpression + " " + m_strInfo).c_str(),
		m_strFuncname.c_str(),
		m_strFilename.c_str(),
		m_uiLine,
		SUPERASSERT_EMAIL,
		(DWORD64) dwStack,
		(DWORD64) dwStackFrame,
		&sFailCount,
		&sIgnoreCount))
	{
		return true;
	}
#else // USE_BUGSLAYER
	while (true)
	{
		int iRes = MessageBoxA(NULL, m_strBuf.c_str(), 
			"Assertion", MB_ABORTRETRYIGNORE | MB_ICONSTOP | MB_TASKMODAL);
		switch (iRes)
		{
		case IDIGNORE:
			return false;
		case IDRETRY:
			__asm int 3;
			return false;
		case IDABORT:
#ifdef _D3D9_H_
			D3DXDebugMute(true);
#endif // _D3D9_H_
			_exit(3);
			return false;
		}
	}
#endif // USE_BUGSLAYER

	return false;
}
//---------------------------------------------------------------------------
KSuperAssert& KSuperAssert::_trace(bool bFlush, int iLogIdx)
{
	m_strBuf += m_strInfo + "\n";

	if (iLogIdx == 0 && m_bLockOutputDebugString == false)
	{
		::OutputDebugStringA(m_strBuf.c_str());
	}

	if (m_bLockGlobalFileStream[iLogIdx] == false &&
		m_kGlobalFileStream[iLogIdx].is_open())
	{
		m_kGlobalFileStream[iLogIdx] << m_strBuf;
		m_kGlobalFileStream[iLogIdx].flush();
	}

#ifdef USE_CONSOLE
	if (iLogIdx == 0 && m_bLockConsole == false)
	{
		KCon << rcol << m_strBuf.c_str();
	}
#endif // USE_CONSOLE

	if (bFlush)
	{
		_initBuf();
	}

	return *this;
}
//---------------------------------------------------------------------------
KSuperAssert& KSuperAssert::_fileout()
{
	if (m_bLockLogFileStream == false && !m_strLogFilename.empty())
	{
		m_strBuf += m_strInfo + "\n";

		time_t t0 = time(0);
		tm* t = localtime(&t0);

		std::fstream fout(m_strLogFilename.c_str(), std::ios::out | std::ios::app);
		//fout << "//---------------------------------------------------------------------------" << std::endl;
		//fout << "// Log Info - " << t->tm_mon << "/" << t->tm_mday << ", " << t->tm_hour << ":" << t->tm_min << std::endl;
		//fout << "//---------------------------------------------------------------------------" << std::endl;
		fout << m_strBuf;
		fout << std::endl;
		fout.close();
	}

	return *this;
}
//---------------------------------------------------------------------------
KSuperAssert& KSuperAssert::_remotelog()
{
#ifdef USE_REMOTELOGGER
	if (!m_strRemoteLoggerPagename.empty())
	{
		m_strBuf += m_strInfo + "\n";

		_KRemoteLogger()->AddLogValue(m_strRemoteLoggerPagename.c_str(), m_strBuf.c_str());
	}
#endif // USE_REMOTELOGGER

	return *this;
}
//---------------------------------------------------------------------------
void KSuperAssert::SetLogFilename(std::string strFilename)
{
	m_strLogFilename = strFilename;
}
//---------------------------------------------------------------------------
void KSuperAssert::SetRemoteLoggerPagename(std::string strPagename)
{
	m_strRemoteLoggerPagename = strPagename;
}
//---------------------------------------------------------------------------
void KSuperAssert::StartGlobalLog()
{
	//m_strGlobalFilename = strFilename;

	m_kGlobalFileStream[0].open("ConsoleLog.txt", std::ios::out);
	m_kGlobalFileStream[0] << "[ Start Console Log ] \n";

	m_kGlobalFileStream[1].open("FlowTrace.txt", std::ios::out);
	m_kGlobalFileStream[1] << "[ Start Error Trace ] \n";
}
//---------------------------------------------------------------------------
// manipication
//---------------------------------------------------------------------------
KSuperAssert& trace(KSuperAssert& kAssert)
{
	kAssert._trace();

	return kAssert;
}
//---------------------------------------------------------------------------
KSuperAssert& fileout(KSuperAssert& kAssert)
{
	kAssert._fileout();
	kAssert._initBuf();

	return kAssert;
}
//---------------------------------------------------------------------------
KSuperAssert& remotelog(KSuperAssert& kAssert)
{
	kAssert._remotelog();
	kAssert._initBuf();

	return kAssert;
}
//---------------------------------------------------------------------------
KSuperAssert& flush(KSuperAssert& kAssert)
{
	kAssert._initBuf();

	return kAssert;
}
//---------------------------------------------------------------------------
// Lock
//---------------------------------------------------------------------------
void KSuperAssert::LockGlobalFileStream(int iIdx, bool bEnable)
{
	m_bLockGlobalFileStream[iIdx] = bEnable;
}
//---------------------------------------------------------------------------
void KSuperAssert::LockLogFileStream(bool bEnable)
{
	m_bLockLogFileStream = bEnable;
}
//---------------------------------------------------------------------------
void KSuperAssert::LockLockConsole(bool bEnable)
{
	m_bLockConsole = bEnable;
}
//---------------------------------------------------------------------------
void KSuperAssert::LockOutputDebugString(bool bEnable)
{
	m_bLockOutputDebugString = bEnable;
}
//---------------------------------------------------------------------------
bool KSuperAssert::IsLockGlobalFileStream(int iIdx)
{
	return m_bLockGlobalFileStream[iIdx];
}
//---------------------------------------------------------------------------
bool KSuperAssert::IsLockLogFileStream()
{
	return m_bLockLogFileStream;
}
//---------------------------------------------------------------------------
bool KSuperAssert::IsLockConsole()
{
	return m_bLockConsole;
}
//---------------------------------------------------------------------------
bool KSuperAssert::IsLockOutputDebugString()
{
	return m_bLockOutputDebugString;
}
//--------------------------------------------------------------------------- 