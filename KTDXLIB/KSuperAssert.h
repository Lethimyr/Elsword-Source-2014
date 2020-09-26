#pragma once

#include <iostream>
#include <sstream>
#include <string>

class KSuperAssert
{

public:
    KSuperAssert();

	KSuperAssert& SetAssertInfo(const char* expr, unsigned int line, 
		const char* func, const char* file);

	void _initBuf();
	void _fatal();
	void _trace();
	void _fileout(bool bFlush);
	void _remotelog();

	template<typename T>
	KSuperAssert& operator << (T val)
	{
		std::ostringstream os;
		os << val;
		m_strInfo += os.str().c_str();

		return *this;
	}

	template<>
		KSuperAssert& operator << (const WCHAR* val)
	{
		char charBuf[255] = {0,};
		WideCharToMultiByte( CP_ACP, 0, val, -1, charBuf, 255, NULL, NULL );


		std::ostringstream os;
		os << charBuf;
		m_strInfo += os.str().c_str();

		std::cout << charBuf << std::endl;

		return *this;
	}

	template<>
		KSuperAssert& operator << (WCHAR* val)
	{
		char charBuf[255] = {0,};
		WideCharToMultiByte( CP_ACP, 0, val, -1, charBuf, 255, NULL, NULL );


		std::ostringstream os;
		os << charBuf;
		m_strInfo += os.str().c_str();

		std::cout << charBuf << std::endl;

		return *this;
	}

	// manipication
	template<>
	KSuperAssert& operator << (KSuperAssert& (*_manip) (KSuperAssert&))
	{
		return _manip( *this );
	}

	friend KSuperAssert& flush(KSuperAssert& kAssert);

	// Flush
	friend KSuperAssert& fatal(KSuperAssert& kAssert);
	friend KSuperAssert& trace(KSuperAssert& kAssert);
	friend KSuperAssert& fileout(KSuperAssert& kAssert);
	friend KSuperAssert& remotelog(KSuperAssert& kAssert);

	// Not Flush
	friend KSuperAssert& fatal_(KSuperAssert& kAssert);
	friend KSuperAssert& trace_(KSuperAssert& kAssert);
	friend KSuperAssert& fileout_(KSuperAssert& kAssert);
	friend KSuperAssert& remotelog_(KSuperAssert& kAssert);

	// Names
public:
	void SetLogFilename(std::string strFilename);
	void SetRemoteLoggerPagename(std::string strPagename);
	
protected:
	std::string m_strLogFilename;
	std::string m_strRemoteLoggerPagename;

protected:
	std::string m_strExpression;
	std::string m_strFilename;
	unsigned int m_uiLine;

	std::string m_strInfo;
	std::string m_strBuf;

};

extern KSuperAssert g_kSuperAssert;


// additional macros

#define KLOG(filename)			KDbgStm.SetLogFilename(filename); KDbgStm

#ifndef _SERVICE_

#ifdef BUG_TRAP
	#define InHouseLog( msg )
	
	#define StateLog( msg ) \
    {	\
        g_CKTDXLog.PushStateLog( msg );	\
    }

    #define ErrorLogMsg( errorEnum, errorMsg )		\
    {	\
        g_CKTDXLog.PushErrorLog( errorEnum, errorMsg );	\
    }

    #define ErrorLog( errorEnum )					\
    {	\
        g_CKTDXLog.PushErrorLog( errorEnum );	\
    }

    #define DialogLog( msg )

    #define LastErrorLog( errorMsg )					\
    {	\
        g_CKTDXLog.SetLastErrorLog( errorMsg );	\
    }

    #define ChecksumLog( msg ) 

#else BUG_TRAP

	#define InHouseLog( msg ) KLOG("InHouse.log")  << msg << fileout;
	#define StateLog( msg ) KLOG("State.log")  << msg << fileout;
	

	#define ErrorLogMsg( errorEnum, errorMsg )		\
	{	\
				KLOG("ErrorLog.txt") << "ErrorEnum(" << errorEnum << "),\t" << "ErrorMsg(\"" << errorMsg << "\")," << "\t( " <<__FILE__ << ", " << __FUNCTION__  << ", " << __LINE__ << " )" << fileout;  \
				std::cout << "ErrorEnum(" << errorEnum << "),\t" << "ErrorMsg(\"" << errorMsg << "\")," << "\t( " <<__FILE__ << ", " << __FUNCTION__  << ", " << __LINE__ << " )" << std::endl; \
				fprintf( stderr, "\nㅁ ErrorEnum( %d ), %s, %s, %d\n", errorEnum, __FILE__, __FUNCTION__, __LINE__ );	\
	}

	#define ErrorLog( errorEnum )					\
	{	\
				KLOG("ErrorLog.txt") << "ErrorEnum(" << errorEnum << "),\t( " <<__FILE__ << ", " << __FUNCTION__ << ", " << __LINE__ << " )" << fileout; \
				std::cout << "ErrorEnum(" << errorEnum << "),\t( " <<__FILE__ << ", " << __FUNCTION__ << ", " << __LINE__ << " )" << std::endl; \
				fprintf( stderr, "\nㅁ ErrorEnum( %d ), %s, %s, %d\n", errorEnum, __FILE__, __FUNCTION__, __LINE__ );	\
	}
	#define DialogLog( msg )		\
	{	\
		KLOG("DialogLog.txt")  << msg << ",\t\t" << __FILE__ << ", " << __FUNCTION__ << ", " << __LINE__ << fileout; \
	}

	#define LastErrorLog( errorMsg )


	#define ChecksumLog( msg ) KLOG("Checksum.log")  << msg << fileout;

	//{{ kimhc // 2010.10.5 // 홈페이지 개편에서 아바타 이미지 표현으로 사용될 캐릭터 이미지 캡쳐 기능 테스트
#ifdef	AVATAR_IMAGE_TEST
	#define CheckUploadFileLog( msg )		\
	{	\
		KLOG("UploadFile.txt")  << msg << fileout; \
	}
#endif	AVATAR_IMAGE_TEST
//}} kimhc // 2010.10.5 // 홈페이지 개편에서 아바타 이미지 표현으로 사용될 캐릭터 이미지 캡쳐 기능 테스트

#endif BUG_TRAP

#else _SERVICE_

	#define InHouseLog( msg )

	#define StateLog( msg ) \
	{	\
		g_CKTDXLog.PushStateLog( msg );	\
	}

	#define ErrorLogMsg( errorEnum, errorMsg )		\
	{	\
		g_CKTDXLog.PushErrorLog( errorEnum, errorMsg );	\
	}

	#define ErrorLog( errorEnum )					\
	{	\
		g_CKTDXLog.PushErrorLog( errorEnum );	\
	}

	#define DialogLog( msg )

	#define LastErrorLog( errorMsg )					\
	{	\
		g_CKTDXLog.SetLastErrorLog( errorMsg );	\
	}

	#define ChecksumLog( msg ) 

//{{ kimhc // 2010.10.5 // 홈페이지 개편에서 아바타 이미지 표현으로 사용될 캐릭터 이미지 캡쳐 기능 테스트
#ifdef	AVATAR_IMAGE_TEST
	#define CheckUploadFileLog( msg )		\
	{	\
		KLOG("UploadFile.txt")  << msg << fileout; \
	}
#endif	AVATAR_IMAGE_TEST
//}} kimhc // 2010.10.5 // 홈페이지 개편에서 아바타 이미지 표현으로 사용될 캐릭터 이미지 캡쳐 기능 테스트

#endif _SERVICE_


/*
#define P2PLogW( msg )			\
	{ \
		string strP2PLogW; \
		ConvertWCHARToChar( strP2PLogW, msg ); \
		KLOG("P2PLog.txt")  << strP2PLogW.c_str() << fileout; std::cout << strP2PLogW.c_str() << std::endl; \
	}
*/
// Macros
//#ifdef _DEBUG

	#define KDbgStm g_kSuperAssert
	#define KAssert(mExpr) \
		if (!(mExpr)) \
			g_kSuperAssert.SetAssertInfo(#mExpr, __LINE__, __FUNCTION__, __FILE__)._fatal(); \
		else \
			g_kSuperAssert._initBuf();

//#else // _DEBUG
//
//	#define KDbgStm /##/
//	#define KAssert(mExpr) /##/
//
//#endif // _DEBUG

/*
	* 사용 방법
	- Trace :
		KDbgStm << Debug Info << trace;
	- Assert : 
		KDbgStm << Debug Info;
		KAssert( exp );

	* 주의
	- KDbgStm 사용 시 여러 라인일 경우 매 라인마다 KDbgStm를 적어주어야 함
		KDbgStm << "Value1";
		KDbgStm << "Value2";
		.
		.
		.
	  
	  ( 다음과 같은 사용 금지 )
		KDbgStm
				<< "Value1"
				<< "Value2"
				.
				.
				.
	  이유는 릴리즈 모드에서의 부하를 모두 없애기 위해 ( 2번째 방법일 경우 함수 호출의 비용이 생김 )
*/
