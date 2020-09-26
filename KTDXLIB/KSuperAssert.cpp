// original code from seokhyun, SF 
// 변경사항: 
//			#undef USE_REMOTELOGGER // 일단 remotelogger랑 console 사용안함 
//			#undef USE_CONSOLE	
//
//			_fileout( bool bFlush = true ) // bool 변수 추가, 코드 변경
//			fileout_()			// 코드 변경


#include "stdafx.h"
#include "KSuperAssert.h"


#undef USE_REMOTELOGGER	// 일단 remotelogger랑 console 사용안함 
#undef USE_CONSOLE		//



#ifdef USE_CONSOLE
	#include "KConsole.h"
#endif // USE_CONSOLE

#ifdef USE_REMOTELOGGER
	#include "KRemoteLogger.h"
#endif // USE_REMOTELOGGER


#include <windows.h>
#include <assert.h>
#include <time.h>
#include <fstream>

KSuperAssert g_kSuperAssert;

//---------------------------------------------------------------------------
KSuperAssert::KSuperAssert()
{
}
//---------------------------------------------------------------------------
KSuperAssert& KSuperAssert::SetAssertInfo(const char* expr, 
										  unsigned int line, const char* func, const char* file)
{
	m_strExpression = expr;
	m_strFilename = file;
	m_uiLine = line;

	std::ostringstream os;
	os
		<< "[Assertion]" << std::endl
		<< "File : " << file << std::endl
		<< "Line : " << line << std::endl
		<< "Expr : " << expr << std::endl << std::endl;

	m_strBuf += os.str().c_str();

	return *this;
}
//---------------------------------------------------------------------------
void KSuperAssert::_initBuf()
{
	m_strBuf.clear();
	m_strInfo.clear();
}
//---------------------------------------------------------------------------
void KSuperAssert::_fatal()
{
	m_strBuf += m_strInfo;
	m_strInfo = "";

#ifdef USE_CONSOLE
	KCon << rcol << m_strBuf.c_str();
#endif // USE_CONSOLE

	while (true)
	{
		int iRes = MessageBoxA(NULL, m_strBuf.c_str(), 
			"Assertion", MB_ABORTRETRYIGNORE | MB_ICONSTOP | MB_TASKMODAL);
		switch (iRes)
		{
		case IDIGNORE:
			return;
		case IDRETRY:
			__asm int 3;
			return;
		case IDABORT:
#ifdef _D3D9_H_
			D3DXDebugMute(true);
#endif // _D3D9_H_
			_exit(3);
			break;
		}
	}
}
//---------------------------------------------------------------------------
void KSuperAssert::_trace()
{
	m_strBuf += m_strInfo + "\n";
	m_strInfo = "";
	::OutputDebugStringA(m_strBuf.c_str());

#ifdef USE_CONSOLE
	KCon << rcol << m_strBuf.c_str();
#endif // USE_CONSOLE
}


//---------------------------------------------------------------------------
void KSuperAssert::_fileout( bool bFlush = true )
{
	m_strBuf += m_strInfo + "\n";
	m_strInfo = "";

	if( true == bFlush )
	{
		if (!m_strLogFilename.empty())
		{
			time_t t0 = time(0);

#ifdef	CONVERSION_VS
			struct tm newTime;
			bool bOK = localtime_s( &newTime, &t0) == 0;
			tm* t = &newTime;
#else	CONVERSION_VS
			tm* t = localtime(&t0);
            bool bOK = ( t != NULL );
#endif	CONVERSION_VS

			std::fstream fout(m_strLogFilename.c_str(), std::ios::out | std::ios::app);
			//fout << "//---------------------------------------------------------------------------" << std::endl;
            if ( bOK == true )
#ifdef CHAT_LOG_TO_TXT
				// 미국 요청으로 형식을 조금 바꿨습니다.
				fout << 1900+t->tm_year << "/" << 1+t->tm_mon << "/" << t->tm_mday << " " << t->tm_hour << ":" << t->tm_min << ":" << t->tm_sec << "\t";
#else // CHAT_LOG_TO_TXT
			    fout << 1900+t->tm_year << "/" << 1+t->tm_mon << "/" << t->tm_mday << ", " << t->tm_hour << ":" << t->tm_min << ":" << t->tm_sec << " > ";
#endif //CHAT_LOG_TO_TXT
			//fout << __DATE__ << ", " << __TIME__ << " > ";
			//fout << "// file: " << __FILE__ << std::endl;
			//fout << "// line: " << __LINE__ << std::endl;
			//fout << "//---------------------------------------------------------------------------" << std::endl;
			fout << m_strBuf;
			//fout << std::endl;
			fout.close();
		}
	}
	
}
//---------------------------------------------------------------------------
void KSuperAssert::_remotelog()
{
#ifdef USE_REMOTELOGGER
	if (!m_strRemoteLoggerPagename.empty())
	{
		_KRemoteLogger()->AddLogValue(m_strRemoteLoggerPagename.c_str(), m_strBuf.c_str());
	}
#endif // USE_REMOTELOGGER
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
// manipication
//---------------------------------------------------------------------------
KSuperAssert& fatal_(KSuperAssert& kAssert)
{
	kAssert._fatal();

	return kAssert;
}
//---------------------------------------------------------------------------
KSuperAssert& trace_(KSuperAssert& kAssert)
{
	kAssert._trace();

	return kAssert;
}
//---------------------------------------------------------------------------
KSuperAssert& fileout_(KSuperAssert& kAssert)
{
	//kAssert._trace();
	kAssert._fileout( false );

	return kAssert;
}
//---------------------------------------------------------------------------
KSuperAssert& remotelog_(KSuperAssert& kAssert)
{
	return kAssert;
}
//---------------------------------------------------------------------------
KSuperAssert& fatal(KSuperAssert& kAssert)
{
	kAssert._fatal();
	kAssert._initBuf();

	return kAssert;
}
//---------------------------------------------------------------------------
KSuperAssert& trace(KSuperAssert& kAssert)
{
	kAssert._trace();
	kAssert._initBuf();

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
	return kAssert;
}
//---------------------------------------------------------------------------
KSuperAssert& flush(KSuperAssert& kAssert)
{
	kAssert._initBuf();

	return kAssert;
}
//---------------------------------------------------------------------------