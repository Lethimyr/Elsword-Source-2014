#include "ActorManager.h"
#include "LevelUpauth.h"

#ifdef SERV_COUNTRY_BR

#include "LevelUpAuthManager.h"
#include "NetError.h"

#include "SoapLevelUp/soapH.h" // or whatever it is called, you must already have it
#include "SoapLevelUp/BasicHttpBinding_USCOREIAuthenticationService.nsmap" // this is what you have to add to fix the problem


#include "SHA1.h"

#define CLASS_TYPE  KLevelUpAuth

std::map< int, KLevelUpAuth::KWebMethod >  KLevelUpAuth::ms_mapWebMethodPath;

_ImplementException( KLevelUpAuth );



KLevelUpAuth::KLevelUpAuth(std::string strWsdlPath)
{
	m_strWsdlPath = strWsdlPath;
	p_gf_soap = NULL;
}

KLevelUpAuth::~KLevelUpAuth()
{
	End( 1500 );
}

// 2010.10.24 조효진. 
// 현재 GameForgeAuth의 매니저가 등록된 곳이 KChannelSimLayer의 m_vecpThreadMgr 이므로
// 구조상 Lua의 AddWebMethod가 KGameForgeAuth::Run() 보다 빠르게 수행된다. 
void KLevelUpAuth::Run()     // derived from KThread
{
	std::cout << "KSubThread::Run(), thread start." << std::endl;
	START_LOG( cout2, L"[Test Log] KLevelUpAuth Start" )
		<< END_LOG;


	srand( (unsigned) time( NULL ) );

	// 추가된 코드 //
	SoapInitialize();
	//

	START_LOG( cout2, L"[테스트로그] KLevelUpAuth Soap Initialized" )
		<< END_LOG;
	
	testfunction();
 
	while( true )
	{
		//{{ 2010/11/11 조효진 SOAP 모듈 정상 생성 실패 시 다시 생성을 위해 //
		if( p_gf_soap == NULL ){
			SoapInitialize();
		}
		//}}

		DWORD ret = ::WaitForSingleObject( m_hKillEvent, 1 );

		if( ret == WAIT_OBJECT_0 ) break;

		if( ret == WAIT_TIMEOUT ) Loop();

		else std::cout << "*** WaitForSingleObject() - return :" << ret << std::endl;

	}

	// 추가된 코드 //
	SoapUninitialize();
	//

	START_LOG( cout2, L"[테스트로그] KLevelUpAuth Soap End" )
		<< END_LOG;
}

 
void KLevelUpAuth::SoapInitialize()
{
	p_gf_soap = soap_new();
	if( !p_gf_soap )	//// couldn't allocate: stop
	{
		p_gf_soap = NULL;

		START_LOG( cerr, L"Soap_new Error" )
			<< END_LOG;

		return;
	}
}


void KLevelUpAuth::SoapUninitialize()
{
	if( p_gf_soap != NULL )
	{
		soap_destroy(p_gf_soap);
		soap_end(p_gf_soap);
		p_gf_soap = NULL;
	}
}


/////////////////////////////////////////////////////////////////////////////
void KLevelUpAuth::AddWebMethod( int iSoapAction, std::string szMethod )
{
	if( iSoapAction < 0 || iSoapAction >= LU_NUM )
	{
		START_LOG( cerr, L"이상한 SOAP Action." )
			<< BUILD_LOG( iSoapAction )
			<< END_LOG;

		return;
	}

	if( szMethod == "" )
	{
		START_LOG( cerr, L"method 이상." )
			<< BUILD_LOG( iSoapAction )
			<< END_LOG;

		return;
	}

	KWebMethod kMethod;
	kMethod.m_iSoapAction = iSoapAction;
	kMethod.m_strMethodPath = szMethod;

	std::map< int, KWebMethod >::iterator mit = ms_mapWebMethodPath.find( iSoapAction );
	if( mit != ms_mapWebMethodPath.end() )
	{
		START_LOG( cerr, L"이미 존재하는 soap action." )
			<< BUILD_LOG( iSoapAction )
			<< END_LOG;

		return;
	}

	// 무조건 넣는다.
	ms_mapWebMethodPath[iSoapAction] = kMethod;
}

bool KLevelUpAuth::GetWebMethod( int iSoapAction, KWebMethod& kInfo )
{
	if( iSoapAction < 0 || iSoapAction >= LU_NUM )
	{
		START_LOG( cerr, L"이상한 SOAP Action." )
			<< BUILD_LOG( iSoapAction )
			<< END_LOG;

		return false;
	}

	std::map< int, KWebMethod >::iterator mit = ms_mapWebMethodPath.find( iSoapAction );
	if( mit == ms_mapWebMethodPath.end() )
	{
		START_LOG( cerr, L"존재하지 않는 soap action." )
			<< BUILD_LOG( iSoapAction )
			<< END_LOG;

		return false;
	}

	kInfo = mit->second;

	return true;
}

//=========================================================================//
//			Process Event handle			
//=========================================================================//

void KLevelUpAuth::ProcessEvent( const KEventPtr& spEvent_ )
{
	DWORD dwElapTime = ::GetTickCount();

	switch( spEvent_->m_usEventID )
	{
	
	CASE( EPUBLISHER_AUTHENTICATION_REQ );
	default:
		START_LOG( cerr, "핸들러가 지정되지 않은 이벤트." )
			<< BUILD_LOG( spEvent_->m_usEventID )
			<< BUILD_LOG( spEvent_->GetIDStr() )
			<< END_LOG;
	}

	dwElapTime = ::GetTickCount() - dwElapTime;
	if( dwElapTime > 3000 )
	{
		START_LOG( cwarn, L"이벤트 처리 소요 시간." )
			<< BUILD_LOG( spEvent_->GetIDStr() )
			<< BUILD_LOG( spEvent_->m_usEventID )
			<< BUILD_LOG( dwElapTime );
	}

	return;
}


//////////////////////////////////////////////////////



void KLevelUpAuth::testfunction()
{
	//{{ 2010/11/11 조효진	soap 함수 호출 전에는 혹시 모르니 항상 확인하자.
	if( p_gf_soap == NULL )
	{// soap 모듈 정상 생성 안된 경우 // 예외처리
		return;
	}

	KWebMethod tmp_web_path;

	if( GetWebMethod(LU_SALT, tmp_web_path) == false )
	{
		START_LOG( cerr, L"[ErrorLog] GetWebMethod ERROR. You must AddwebMethod LU_SALT. Check Connection." )
		<< END_LOG;
		return;
	}

	struct _ns1__GetSalt GetSaltInput;
	GetSaltInput.username = "kog004";

	struct _ns1__GetSaltResponse GetSaltOutput;

	int soap_call_result = soap_call___ns1__GetSalt(p_gf_soap, m_strWsdlPath.c_str(), tmp_web_path.m_strMethodPath.c_str() , &GetSaltInput , &GetSaltOutput );

	// Soap 호출 오류 //
	if( 0 != soap_call_result )
	{
		char str_err[500];
		soap_sprint_fault(p_gf_soap, str_err, 500);

		std::string str = str_err;

		START_LOG( cerr, L"[ErrorLog] Salt Error. Check Connection " )
			<< BUILD_LOG( m_strWsdlPath )
			<< BUILD_LOG( tmp_web_path.m_strMethodPath )
			<< BUILD_LOG( str)
			<< END_LOG;

		return;
	}
	
	int iErrorCode = *(GetSaltOutput.GetSaltResult->ErrorCode);
	if( iErrorCode != -1 )
	{
		START_LOG( cerr, L"[ErrorLog] Salt Not Succeeded." )
		<< BUILD_LOG( iErrorCode )
		<< END_LOG;

		return;
	}

	std::wstring wstrClearPassword = L"!KOGtest004";
	std::string strPasswordMD5 = KncUtil::GetMD5( KncUtil::toNarrowString( wstrClearPassword ) );
	MakeUpperCase( strPasswordMD5 );
	std::string strSalt = GetSaltOutput.GetSaltResult->Salt;
	std::string strAddMD5Salt;
	strAddMD5Salt = strPasswordMD5 + strSalt;
	CSHA1 Sha1;
	Sha1.Update( ( unsigned char* )strAddMD5Salt.c_str(),strAddMD5Salt.size() );
	char cResultLevelUpPW[50] = { 0, };
	Sha1.Final();
	Sha1.ReportHash( cResultLevelUpPW, CSHA1::REPORT_HEX, true );
	MakeUpperCase( cResultLevelUpPW );

	struct _ns1__Authenticate AuthInput;
	AuthInput.username = GetSaltInput.username;
	AuthInput.password = cResultLevelUpPW;
	AuthInput.ipaddress = "14.45.79.161";

	struct _ns1__AuthenticateResponse Authoutput;

	if( GetWebMethod(LU_AUTH, tmp_web_path) == false )
	{
		START_LOG( cerr, L"[ErrorLog] GetWebMethod ERROR. You must AddwebMethod LU_AUTH. Check Connection." )
		<< END_LOG;

		return;
	}

	soap_call_result = soap_call___ns1__Authenticate(p_gf_soap, 
							m_strWsdlPath.c_str(), tmp_web_path.m_strMethodPath.c_str(),
							&AuthInput, &Authoutput );

	// Soap 호출 오류 //
	if( 0 != soap_call_result )
	{
		char str_err[500];
		soap_sprint_fault(p_gf_soap, str_err, 500);

		std::string str = str_err;

		START_LOG( cerr, L"[ErrorLog] Authenticate ERROR. Check Connection " )
			<< BUILD_LOG( m_strWsdlPath )
			<< BUILD_LOG( tmp_web_path.m_strMethodPath )
			<< BUILD_LOG( str)
			<< END_LOG;

		return;
	}

	iErrorCode = *(Authoutput.AuthenticateResult->ErrorCode);
	if( iErrorCode != -1 )
	{
		START_LOG( cerr, L"[ErrorLog] Authenticate Not Succeeded." )
			<< BUILD_LOG( iErrorCode )
			<< END_LOG;
		return;
	}
	
	START_LOG( clog2, L"[테스트로그] testFunction Seccess!!" )
		<< END_LOG;

	return;
}


IMPL_ON_FUNC( EPUBLISHER_AUTHENTICATION_REQ )
{

	//{{ 2010. 11. 25. 조효진	아무것도 입력되지 않은 경우 에러 처리함
	if( kPacket_.m_wstrServiceAccountID == L"" )
	{
		KEPUBLISHER_AUTHENTICATION_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_GAMEFORGE_01;
		KEventPtr spEvent( new KEvent );
		spEvent->SetData( PI_CHANNEL_USER, NULL, EPUBLISHER_AUTHENTICATION_ACK, kPacketAck );
		KActorManager::GetKObj()->QueueingTo( kPacket_.m_iActorUID, spEvent );
		return;
	}
	else if ( kPacket_.m_wstrServicePassword == L"" )
	{
		KEPUBLISHER_AUTHENTICATION_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_GAMEFORGE_00;
		KEventPtr spEvent( new KEvent );
		spEvent->SetData( PI_CHANNEL_USER, NULL, EPUBLISHER_AUTHENTICATION_ACK, kPacketAck );
		KActorManager::GetKObj()->QueueingTo( kPacket_.m_iActorUID, spEvent );
		return;
	}
	//}}
	
	// 아래는 퍼블리셔와 통신해서 인증 하는 부분.
	// 사내 서버 경우는 이부분을 건너띄고, 바로 우리 Account DB 확인으로 넘어간다.
	std::wstring wstrPurchTok = L"";
	UidType iActorUID = kPacket_.m_iActorUID;

	const int MaxNameSize = 100;
	const int MaxPasswordSize = 100;
	const int MaxIpadressSize = 100;
	
	//{{ 2010. 11. 24. 조효진 마지막 NULL값 보장해 주기 위해 수정
	char tmp_input_userName[MaxNameSize+1] = {0,};
	char tmp_input_password[MaxPasswordSize+1] = {0,};
	char tmp_input_ipadress[MaxIpadressSize+1] = {0,};
	tmp_input_userName[MaxNameSize] = NULL;
	tmp_input_password[MaxPasswordSize] = NULL;
	tmp_input_ipadress[MaxIpadressSize] = NULL;
	//}}

	// size 문제 
	strncpy(tmp_input_userName,KncUtil::toNarrowString(kPacket_.m_wstrServiceAccountID).c_str(), MaxNameSize ); 
	strncpy(tmp_input_password,KncUtil::toNarrowString(kPacket_.m_wstrServicePassword).c_str(), MaxPasswordSize ); 
	strncpy(tmp_input_ipadress,KncUtil::toNarrowString(kPacket_.m_wstrIP).c_str(), MaxIpadressSize ); 


	//{{ 2010/11/11 조효진	soap 함수 호출 전에는 혹시 모르니 항상 확인하자.
	if( p_gf_soap == NULL )
	{// soap 모듈 정상 생성 안된 경우 // 예외처리
		return;
	}

	KWebMethod tmp_web_path;
	if( GetWebMethod(LU_SALT, tmp_web_path) == false )
	{
		START_LOG( cerr, L"[ErrorLog] GetWebMethod ERROR. You must AddwebMethod LU_SALT. Check Connection." )
		<< END_LOG;

		KEPUBLISHER_AUTHENTICATION_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_VERIFY_21;

		KEventPtr spEvent( new KEvent );
		spEvent->SetData( PI_CHANNEL_USER, NULL, EPUBLISHER_AUTHENTICATION_ACK, kPacketAck );
		KActorManager::GetKObj()->QueueingTo( iActorUID, spEvent );

		return;
	}

	struct _ns1__GetSalt GetSaltInput;
	GetSaltInput.username = tmp_input_userName;
	
	struct _ns1__GetSaltResponse GetSaltOutput;

	int soap_call_result = soap_call___ns1__GetSalt(p_gf_soap, m_strWsdlPath.c_str(), tmp_web_path.m_strMethodPath.c_str(), &GetSaltInput , &GetSaltOutput );

	// Soap 호출 오류 //

	if( 0 != soap_call_result )
	{
		char str_err[500];
		soap_sprint_fault(p_gf_soap, str_err, 500);
		
		std::string str = str_err;

		char str_username[1024];
		if(GetSaltInput.username != NULL)
			strncpy(str_username, GetSaltInput.username, 1024);

		START_LOG( cerr, L" 숍 통신 에러" )
			<< BUILD_LOG( m_strWsdlPath )
			<< BUILD_LOG( tmp_web_path.m_strMethodPath )
			<< BUILD_LOG( str )
			<< BUILD_LOG( str_username )
			<< END_LOG;

		KEPUBLISHER_AUTHENTICATION_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_VERIFY_21;

		KEventPtr spEvent( new KEvent );
		spEvent->SetData( PI_CHANNEL_USER, NULL, EPUBLISHER_AUTHENTICATION_ACK, kPacketAck );
		KActorManager::GetKObj()->QueueingTo( iActorUID, spEvent );

		return;
	}

	int iErrorCode = *(GetSaltOutput.GetSaltResult->ErrorCode);
	if( iErrorCode != -1 )
	{
		char str_username[1024];
		if(GetSaltInput.username != NULL)
			strncpy(str_username, GetSaltInput.username, 1024);

		START_LOG( cerr, L"아이디가 올바르지 않습니다. SALT ERROR " )
			<< BUILD_LOG( str_username )
			<< END_LOG;

		KEPUBLISHER_AUTHENTICATION_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_GAMEFORGE_01;

		KEventPtr spEvent( new KEvent );
		spEvent->SetData( PI_CHANNEL_USER, NULL, EPUBLISHER_AUTHENTICATION_ACK, kPacketAck );
		KActorManager::GetKObj()->QueueingTo( iActorUID, spEvent );

		return;
	}

	std::string strClearPassword = tmp_input_password;
	std::string strPasswordMD5 = KncUtil::GetMD5( strClearPassword );
	MakeUpperCase( strPasswordMD5 );
	std::string strSalt = GetSaltOutput.GetSaltResult->Salt;
	std::string strAddMD5Salt;
	strAddMD5Salt = strPasswordMD5 + strSalt;
	CSHA1 Sha1;
	Sha1.Update( ( unsigned char* )strAddMD5Salt.c_str(),strAddMD5Salt.size() );
	char cResultLevelUpPW[50] = { 0, };
	Sha1.Final();
	Sha1.ReportHash( cResultLevelUpPW, CSHA1::REPORT_HEX, true );
	MakeUpperCase( cResultLevelUpPW );

	struct _ns1__Authenticate AuthInput;
	AuthInput.username = GetSaltInput.username;
	AuthInput.password = cResultLevelUpPW;
	AuthInput.ipaddress = tmp_input_ipadress;

	struct _ns1__AuthenticateResponse Authoutput;

	if( GetWebMethod(LU_AUTH, tmp_web_path) == false )
	{
		START_LOG( cerr, L"[ErrorLog] GetWebMethod ERROR. You must AddwebMethod LU_AUTH. Check Connection." )
			<< BUILD_LOG( AuthInput.username )
			<< BUILD_LOG( AuthInput.password )
			<< BUILD_LOG( AuthInput.ipaddress )
			<< END_LOG;

		KEPUBLISHER_AUTHENTICATION_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_VERIFY_21;

		KEventPtr spEvent( new KEvent );
		spEvent->SetData( PI_CHANNEL_USER, NULL, EPUBLISHER_AUTHENTICATION_ACK, kPacketAck );
		KActorManager::GetKObj()->QueueingTo( iActorUID, spEvent );

		return;
	}

	soap_call_result = soap_call___ns1__Authenticate(p_gf_soap, 
		m_strWsdlPath.c_str(), tmp_web_path.m_strMethodPath.c_str(),
		&AuthInput, &Authoutput );

	// Soap 호출 오류 //
	if( 0 != soap_call_result )
	{
		char str_err[500];
		soap_sprint_fault(p_gf_soap, str_err, 500);
		
		std::string str = str_err;

		char str_username[1024];
		if(AuthInput.username != NULL)
			strncpy(str_username, AuthInput.username, 1024);

		START_LOG( cerr, L" 숍 통신 에러" )
			<< BUILD_LOG( m_strWsdlPath )
			<< BUILD_LOG( tmp_web_path.m_strMethodPath )
			<< BUILD_LOG( str )
			<< BUILD_LOG( str_username )
			<< END_LOG;

		KEPUBLISHER_AUTHENTICATION_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_VERIFY_21;

		KEventPtr spEvent( new KEvent );
		spEvent->SetData( PI_CHANNEL_USER, NULL, EPUBLISHER_AUTHENTICATION_ACK, kPacketAck );
		KActorManager::GetKObj()->QueueingTo( iActorUID, spEvent );

		return;
	}

	iErrorCode = *(Authoutput.AuthenticateResult->ErrorCode);
	if( iErrorCode != -1 )
	{
		char str_username[1024];
		if(AuthInput.username != NULL)
			strncpy(str_username, AuthInput.username, 1024);

		KEPUBLISHER_AUTHENTICATION_ACK kPacketAck;

		if( iErrorCode == 999 )
		{
			START_LOG( cerr, L"999 AUTH ERROR " )
				<< BUILD_LOG( iErrorCode )
				<< BUILD_LOG( str_username )
				<< END_LOG;

			kPacketAck.m_iOK = NetError::ERR_LEVELUP_01;
		}
		else
		{
			START_LOG( cerr, L"비밀번호가 올바르지 않습니다. AUTH ERROR " )
				<< BUILD_LOG( iErrorCode )
				<< BUILD_LOG( str_username )
				<< BUILD_LOG( cResultLevelUpPW )
				<< END_LOG;

			kPacketAck.m_iOK = NetError::ERR_GAMEFORGE_00;
		}

		KEventPtr spEvent( new KEvent );
		spEvent->SetData( PI_CHANNEL_USER, NULL, EPUBLISHER_AUTHENTICATION_ACK, kPacketAck );
		KActorManager::GetKObj()->QueueingTo( iActorUID, spEvent );

		return;
	}

	// 성공 실패 여부에 상관없이 EPUBLISHER_AUTHENTICATION_ACK 보냄

	KEPUBLISHER_AUTHENTICATION_ACK kPacketAck;
	kPacketAck.m_iOK = NetError::NET_OK;
	kPacketAck.m_iResult = 1;
	kPacketAck.m_wstrIP = kPacket_.m_wstrIP;
	kPacketAck.m_wstrID = kPacket_.m_wstrServiceAccountID;
	kPacketAck.m_uiPublisherUID = *(Authoutput.AuthenticateResult->UniqueUserKey);
#ifdef SERV_PURCHASE_TOKEN
	kPacketAck.wstrPurchTok = wstrPurchTok;
#endif SERV_PURCHASE_TOKEN

	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_CHANNEL_USER, NULL, EPUBLISHER_AUTHENTICATION_ACK, kPacketAck );
	KActorManager::GetKObj()->QueueingTo( iActorUID, spEvent );


	START_LOG( clog, L"[테스트로그] LU authentication result" )
		<< BUILD_LOG( iActorUID )
		<< BUILD_LOG( kPacketAck.m_wstrID )
		<< BUILD_LOG( kPacketAck.m_uiPublisherUID )
		<< BUILD_LOG( kPacketAck.m_wstrIP )
		<< END_LOG;
}


#endif // SERV_COUNTRY_BR
