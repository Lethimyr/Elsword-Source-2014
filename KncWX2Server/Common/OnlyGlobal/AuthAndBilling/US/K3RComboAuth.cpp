#include "ActorManager.h"
#include "K3RComboauth.h"

#ifdef SERV_COUNTRY_US

#include "K3RComboAuthManager.h"
#include "NetError.h"


#include "SoapK3R/soapH.h" // or whatever it is called, you must already have it
#include "SoapK3R/gameEndPoint.nsmap" // this is what you have to add to fix the problem


#define CLASS_TYPE  KK3RComboAuth

std::map< int, KK3RComboAuth::KWebMethod >  KK3RComboAuth::ms_mapWebMethodPath;

_ImplementException( KK3RComboAuth );



KK3RComboAuth::KK3RComboAuth(std::string strWsdlPath)
{
	m_strWsdlPath = strWsdlPath;
//	m_WsmlFile = WsmlFile;

	p_gf_soap = NULL;
}

KK3RComboAuth::~KK3RComboAuth()
{
	End( 1500 );
}

// 2010.10.24 조효진. 
// 현재 GameForgeAuth의 매니저가 등록된 곳이 KChannelSimLayer의 m_vecpThreadMgr 이므로
// 구조상 Lua의 AddWebMethod가 KGameForgeAuth::Run() 보다 빠르게 수행된다. 
void KK3RComboAuth::Run()     // derived from KThread
{
	std::cout << "KSubThread::Run(), thread start." << std::endl;
	START_LOG( clog2, L"[Test Log] KK3RComboAuth Start" )
		<< END_LOG;


	srand( (unsigned) time( NULL ) );

	// 추가된 코드 //
	SoapInitialize();
	//


	START_LOG( clog2, L"KK3RComboAuth Soap Initialized" )
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

	START_LOG( clog2, L"[테스트로그] KGameForgeAuth Soap End" )
		<< END_LOG;
}




void KK3RComboAuth::SoapInitialize()
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


void KK3RComboAuth::SoapUninitialize()
{
	if( p_gf_soap != NULL )
	{
		soap_destroy(p_gf_soap);
		soap_end(p_gf_soap);
		p_gf_soap = NULL;
	}
}


/////////////////////////////////////////////////////////////////////////////
void KK3RComboAuth::AddWebMethod( int iSoapAction, std::string szMethod )
{
	if( iSoapAction < 0 || iSoapAction >= K3R_NUM )
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

bool KK3RComboAuth::GetWebMethod( int iSoapAction, KWebMethod& kInfo )
{
	// AddWebMethod 로 담아놨던 함수들을 가져오는 부분
	if( iSoapAction < 0 || iSoapAction >= K3R_NUM )
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

/*
DISPID KGameForgeAuth::GetIDOfName(LPOLESTR lpName)
{

	HRESULT hr;
	DISPID dispid;

	if( m_pSoapClient == NULL )
	{
		START_LOG( cerr, L"m_pSoapClient Not Initialize." )
			<< BUILD_LOG( lpName )
			<< END_LOG;

		return -1;
	}

	hr = m_pSoapClient->GetIDsOfNames(IID_NULL, &lpName, 1, LOCALE_SYSTEM_DEFAULT, &dispid);
	if( FAILED(hr) ) 
	{
		START_LOG( cerr, L"Soap GetIDsOfNames failed." )
			<< BUILD_LOG( lpName )
			<< END_LOG;

		return -1;
	}

	return dispid;
}


_bstr_t KGameForgeAuth::GetEndPointURL()
{
	return m_pSoapClient->ConnectorProperty["EndPointURL"];
}


void KGameForgeAuth::PutEndPointURL(_bstr_t EndPointURL)
{
	m_pSoapClient->ConnectorProperty["EndPointURL"] = EndPointURL;
}

//=========================================================================//
//			Web Method Call				
//=========================================================================//
// Return value : (-1 = function call error, 1 = OK, 2 = Wrong Password, 3 = User not found, 4 = User not validated) 
int KGameForgeAuth::ConfirmLoginOnWebDb(_bstr_t userName, _bstr_t password, int& iAccountID )
{
	HRESULT hr;
	DISPPARAMS params;
	VARIANT varg[2];
	_variant_t resultInfo;
	EXCEPINFO excepinfo;
	unsigned int uArgErr;



	KWebMethod kMethod;
	_JIF( GetWebMethod( GFO_AUTH, kMethod ), return -1 );
	

	static DISPID dispid = -1;
	if(dispid == -1) dispid = GetIDOfName( _bstr_t( kMethod.m_strMethod.c_str()) );
	

	varg[1].vt = VT_BSTR;
	varg[1].bstrVal = userName;

	varg[0].vt = VT_BSTR;
	varg[0].bstrVal = password;

    params.cArgs = 2;
	params.rgvarg = varg;
	params.cNamedArgs = 0;
	params.rgdispidNamedArgs = NULL;


	memset(&excepinfo, 0, sizeof(excepinfo));

	uArgErr = -1;

	hr = m_pSoapClient->Invoke(
		dispid, 
		IID_NULL, 
		LOCALE_SYSTEM_DEFAULT, 
		DISPATCH_METHOD, 
		&params,
		&resultInfo, 
		&excepinfo, 
		&uArgErr);

	
	if( FAILED(hr) )
	{
		START_LOG( cerr, L"Soap Invoke failed" )
			<< BUILD_LOG( kMethod.m_strMethod )
			<< BUILD_LOG( dispid )
			<< BUILD_LOG( userName )
			<< BUILD_LOG( password )
			<< BUILD_LOG( excepinfo.bstrSource )
			<< BUILD_LOG( excepinfo.bstrDescription )
			<< END_LOG;

			return -1;
	}

	
	if( (VT_BSTR|VT_ARRAY) != resultInfo.vt )
	{
		START_LOG( cerr, L"Invoke Return Type mismatched" )
			<< BUILD_LOG( kMethod.m_strMethod )
			<< BUILD_LOG( dispid )
			<< BUILD_LOG( resultInfo.vt )
			<< END_LOG;

		return -1;
	}

	UINT uDim        = 0;    // 결과값 문자열 배열의 차원 정보
	long lLBound     = 0l;   // 결과값 문자열 배열의 낮은 인덱스
	long lUBound     = 0l;   // 결과값 문자열 배열의 높은 인덱스
	long nBoundIndex = 0l;   // 결과값 문자열 배열 인덱스로 값을 읽어올때 사용합니다.
	BSTR bstrElement = NULL; // 결과값 문자열 배열의 하나의 값을 읽어올때 사용합니다.


	uDim = SafeArrayGetDim    (resultInfo.parray);                 // 몇 차원배열인지 확인합니다.
	hr   = SafeArrayGetLBound (resultInfo.parray, uDim, &lLBound); // 배열의 낮은 인덱스를 구합니다.
	hr   = SafeArrayGetUBound (resultInfo.parray, uDim, &lUBound); // 배열의 높은 인덱스를 구합니다.



	// 1차원 배열임을 보증합니다.
	_JIF( (uDim == 1 && lUBound == 1), return -1 );
	

	//confirmLoginOnWebDb(userName, password)	returns an array including two integers:
	//$arr[0] = Result (1 = OK, 2 = Wrong Password, 3 = User not found, 4 = User not validated) 
	//$arr[1] = Account-ID (includes the ID of the account in our database, only if Result = 1)
	//User not validated를 비밀 번호 체크보다 먼저함

	hr = SafeArrayGetElement(resultInfo.parray, &nBoundIndex, &bstrElement);
	int iLoginResult = _ttoi ( bstrElement );
	SysFreeString (bstrElement);
	bstrElement = NULL;

	hr = SafeArrayGetElement(resultInfo.parray, &nBoundIndex, &bstrElement);
	iAccountID = _ttoi( bstrElement );
	SysFreeString (bstrElement);
	bstrElement = NULL;


	// 배열의 갯수 만큼 반복
// 	for (nBoundIndex=lLBound; nBoundIndex<=lUBound; nBoundIndex++)
// 	{
// 		// 배열로 부터 인덱스에 해당하는 문자열을 읽어옵니다.
// 		hr = SafeArrayGetElement(resultInfo.parray, &nBoundIndex, &bstrElement);
// 
// 		START_LOG( cout, bstrElement)
// 			<< END_LOG;
// 		
// 		_ttoi ( bstrElement );
// 
// 
// 		SysFreeString (bstrElement);
// 		bstrElement = NULL;
// 	}


	return iLoginResult;

}
*/

//=========================================================================//
//			Process Event handle			
//=========================================================================//

void KK3RComboAuth::ProcessEvent( const KEventPtr& spEvent_ )
{
	DWORD dwElapTime = ::GetTickCount();

	switch( spEvent_->m_usEventID )
	{
	
	CASE( EPUBLISHER_AUTHENTICATION_REQ );
	CASE( EPUBLISHER_SECURITY_AUTH_REQ );

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

void KK3RComboAuth::testfunction()
{
	struct _ns3__Login tmp_input; 
	tmp_input.username = "aaa";
	tmp_input.password = "asdfasdf1";
	tmp_input.ip_USCOREaddress = "192.168.71.45";
	struct _ns3__LoginResponse tmp_output;


	KWebMethod tmp_web_path;
	if( GetWebMethod(K3R_AUTH, tmp_web_path) == false )
	{
		START_LOG( cerr, L"[ErrorLog] GetWebMethod ERROR. You must AddwebMethod GFO_AUTH. Check Connection." )
			<< BUILD_LOG( tmp_input.username )
			<< BUILD_LOG( tmp_input.password )
			<< BUILD_LOG( tmp_input.ip_USCOREaddress )
			<< END_LOG;

		return;
	}


	//{{ 2010/11/11 조효진	soap 함수 호출 전에는 혹시 모르니 항상 확인하자.
	if( p_gf_soap == NULL )
	{// soap 모듈 정상 생성 안된 경우 // 예외처리
		return;
	}



	int soap_call_result = soap_call___ns1__Login(p_gf_soap, 
		m_strWsdlPath.c_str(), tmp_web_path.m_strMethodPath.c_str(),
		&tmp_input, &tmp_output );

	// Soap 호출 오류 //
	if( 0 != soap_call_result )
	{
		char str_err[500];
		soap_sprint_fault(p_gf_soap, str_err, 500);

		std::string str = str_err;

		START_LOG( cerr, L"[ErrorLog] confirmLoginOnWebDB ERROR. Check Connection " )
			<< BUILD_LOG( m_strWsdlPath )
			<< BUILD_LOG( tmp_web_path.m_strMethodPath )
			<< BUILD_LOG( str)
			<< END_LOG;

		return;
	}
	int iResult = 1;
	// 비정상적인 경우에 리턴값 가져오다 서버가 죽는걸 방지 하기 위해서
	if( tmp_output.LoginResult != NULL )
	{
		std::string strResult = tmp_output.LoginResult;
		int iSplitPoint = strResult.find_first_of("|");
		//iResult =  std::atoi( strResult.substr(0, iSplitPoint).c_str() );

		std::string strTemp = strResult.substr(0, iSplitPoint).c_str();
		std::string strTemp2 = strResult.substr(iSplitPoint + 1, strResult.length() - iSplitPoint).c_str();
		iResult =  std::atoi( strTemp.c_str() );
		START_LOG( clog2, L"제대로 나뉘었나 확인" )
			<< BUILD_LOG( strResult )
			<< BUILD_LOG( strTemp )
			<< BUILD_LOG( strTemp2 )
			<< END_LOG;
		// 리턴 값 에러 : 아뒤가 없음//
		if( iResult == 2 )
		{
			START_LOG( cerr, L"[ErrorLog][Soap] Account does not exist" )
				<< END_LOG;

			return;
		}

		// 리턴 값 에러 : 패스워드 틀림//
		if( iResult == 3 )
		{
			START_LOG( cerr, L"[ErrorLog][Soap] Invalid password" )
				<< END_LOG;

			return;
		}

		// 리턴 값 에러 : 아직 계정 활성화가 안됬음 응?//
		if( iResult == 4 )
		{
			START_LOG( cerr, L"[ErrorLog][Soap] Not yet activated" )
				<< END_LOG;

			return;
		}

		// 리턴 값 에러 : 계정 블럭
		if( iResult == 5 )
		{
			START_LOG( cerr, L"[ErrorLog][Soap] Suspended Account" )
				<< END_LOG;

			return;
		}

		if( iResult == 6 )
		{
			START_LOG( cerr, L"[ErrorLog][Soap] IP Block" )
				<< END_LOG;

			return;
		}


	}
	// 비정상적인 경우 
	else
	{
		START_LOG( cerr, L"[ErrorLog] Test confirmLoginOnWebDBResult Return NULL!!!. SOAP ERROR!!! " )
			<< END_LOG;
		return;
	}
	
	START_LOG( clog2, L"[테스트로그] testFunction Seccess!!" )
		<< BUILD_LOG( iResult)
		<< END_LOG;

	return;
}

//////////////////////////////////////////////////////


//_IMPL_ON_FUNC( EGF_AUTHORIZATION_REQ, KGFResponseData )
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
		kPacketAck.m_iOK = NetError::ERR_BUY_CASH_ITEM_25;
		KEventPtr spEvent( new KEvent );
		spEvent->SetData( PI_CHANNEL_USER, NULL, EPUBLISHER_AUTHENTICATION_ACK, kPacketAck );
		KActorManager::GetKObj()->QueueingTo( kPacket_.m_iActorUID, spEvent );
		return;
	}
	//}}

#ifdef SERV_CHANNELING_AERIA
	// 테스트 로그
	START_LOG(clog, L"[TEST] ID, PW 확인")
		<< BUILD_LOG( kPacket_.m_wstrServiceAccountID)
		<< BUILD_LOG( kPacket_.m_wstrServicePassword)
		<< END_LOG;
#endif //SERV_CHANNELING_AERIA
	
	// 아래는 퍼블리셔와 통신해서 인증 하는 부분.
	// 사내 서버 경우는 이부분을 건너띄고, 바로 우리 Account DB 확인으로 넘어간다.
	int iResult = 1;
	std::string strResult = "";
	
	std::wstring wstrPurchTok = L"";
	UidType iActorUID = kPacket_.m_iActorUID;

	std::string strK3RUID = "";
	std::string strURL = "";


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

	iResult = 0;
	strResult = "";
	wstrPurchTok = L"";

	iActorUID = kPacket_.m_iActorUID;

	struct _ns3__Login tmp_input; 
	struct _ns3__LoginResponse tmp_output;

	// size 문제 
	strncpy(tmp_input_userName,KncUtil::toNarrowString(kPacket_.m_wstrServiceAccountID).c_str(), MaxNameSize ); 
	strncpy(tmp_input_password,KncUtil::toNarrowString(kPacket_.m_wstrServicePassword).c_str(), MaxPasswordSize ); 
	strncpy(tmp_input_ipadress,KncUtil::toNarrowString(kPacket_.m_wstrIP).c_str(), MaxIpadressSize ); 


	tmp_input.username = tmp_input_userName;
	tmp_input.password = tmp_input_password;
	tmp_input.ip_USCOREaddress = tmp_input_ipadress;

	KWebMethod tmp_web_path;
	if( GetWebMethod(K3R_AUTH, tmp_web_path) == false )
	{
		START_LOG( cerr, L"[ErrorLog] GetWebMethod ERROR. You must AddwebMethod GFO_AUTH" )
			<< BUILD_LOG( tmp_input_userName )
			<< BUILD_LOG( tmp_input_password )
			<< BUILD_LOG( tmp_input_ipadress )
			<< END_LOG;

		KEPUBLISHER_AUTHENTICATION_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_VERIFY_21;
					
		KEventPtr spEvent( new KEvent );
		spEvent->SetData( PI_CHANNEL_USER, NULL, EPUBLISHER_AUTHENTICATION_ACK, kPacketAck );
		KActorManager::GetKObj()->QueueingTo( iActorUID, spEvent );

		return;
	}


	//{{ 2010/11/11 조효진	soap 함수 호출 전에는 혹시 모르니 항상 확인하자.
	if( p_gf_soap == NULL )
	{// soap 모듈 정상 생성 안된 경우 // 예외처리
		return;
	}

	int soap_call_result = soap_call___ns1__Login(p_gf_soap, 
							m_strWsdlPath.c_str(), tmp_web_path.m_strMethodPath.c_str(),
							&tmp_input, &tmp_output );
	// Soap 호출 오류 //
	if( 0 != soap_call_result )
	{
		char str_err[500];
		soap_sprint_fault(p_gf_soap, str_err, 500);

		char str_username[1024];
		if(tmp_input.username != NULL)
			strncpy(str_username, tmp_input.username, 1024);

		char str_ip_USCOREaddress[1024];
		if(tmp_input.ip_USCOREaddress != NULL)
			strncpy(str_ip_USCOREaddress, tmp_input.ip_USCOREaddress, 1024);

		START_LOG( cerr, L"[ErrorLog] confirmLoginOnWebDB ERROR. " )
			<< BUILD_LOG( m_strWsdlPath )
			<< BUILD_LOG( tmp_web_path.m_strMethodPath )
			<< BUILD_LOG( str_err )
			<< BUILD_LOG( str_username )
			<< BUILD_LOG( str_ip_USCOREaddress )
			<< BUILD_LOG( soap_call_result )
			<< END_LOG;

		KEPUBLISHER_AUTHENTICATION_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_VERIFY_21;

		KEventPtr spEvent( new KEvent );
		spEvent->SetData( PI_CHANNEL_USER, NULL, EPUBLISHER_AUTHENTICATION_ACK, kPacketAck );
		KActorManager::GetKObj()->QueueingTo( iActorUID, spEvent );

		return;
	}

	// 비정상적인 경우에 리턴값 가져오다 서버가 죽는걸 방지 하기 위해서
	if( tmp_output.LoginResult != NULL )
	{
		// 리턴 값 
		//iResult = *(tmp_output.LoginResult);
		strResult = tmp_output.LoginResult;

		//int iSplitPoint = strResult.find_first_of("|");
		//std::string strTemp = strResult.substr(0, iSplitPoint).c_str();
		//strK3RUID = strResult.substr(iSplitPoint + 1, strResult.length() - iSplitPoint).c_str();
		//iResult =  std::atoi( strTemp.c_str() );
		//START_LOG( clog2, L"제대로 나뉘었나 확인" )
		//	<< BUILD_LOG( strResult )
		//	<< BUILD_LOG( strTemp )
		//	<< BUILD_LOG( strK3RUID )
		//	<< BUILD_LOG( iResult )
		//	<< END_LOG;

		START_LOG( clog2, L"제대로 나뉘었나 확인" )
			<< BUILD_LOG( strResult )
			<< END_LOG;
		std::vector< std::string > vecStringKey;
		vecStringKey.reserve( 3 );
		std::string strTemp = strResult;
		while( true )
		{
			int iSplitPoint = strTemp.find_first_of( "|" );
			if( iSplitPoint == std::string::npos )
			{
				vecStringKey.push_back( strTemp.c_str() );
				break;
			}

			vecStringKey.push_back( strTemp.substr( 0, iSplitPoint ).c_str() );
			strTemp = strTemp.substr( iSplitPoint + 1, strTemp.length() - iSplitPoint ).c_str();
		}

		iResult = std::atoi( vecStringKey[0].c_str() );
		strK3RUID = vecStringKey[1];

		if( vecStringKey.size() >= 3 )
			strURL = vecStringKey[2];


		START_LOG( clog2, L"제대로 나뉘었나 확인" )
			<< BUILD_LOG( strResult )
			<< BUILD_LOG( iResult )
			<< BUILD_LOG( strK3RUID )
			<< BUILD_LOG( strURL )
			<< END_LOG;
	}
	// 비정상적인 경우 
	else
	{
		START_LOG( cerr, L"[ErrorLog] confirmLoginOnWebDBResult Return NULL!!!. SOAP ERROR!!! " )
			<< END_LOG;

		KEPUBLISHER_AUTHENTICATION_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_VERIFY_21;

		KEventPtr spEvent( new KEvent );
		spEvent->SetData( PI_CHANNEL_USER, NULL, EPUBLISHER_AUTHENTICATION_ACK, kPacketAck );
		KActorManager::GetKObj()->QueueingTo( iActorUID, spEvent );

		return;
	}

	// SOAP에서 purchTok를 잘못 받아서 시스템이 뻗는 경우를 확인 하기 위해서
	if( wstrPurchTok.length() >= 50 )
	{
		START_LOG( cerr, L"[ERROR] wstrPurchTok String size error!" )
			<< BUILD_LOG( tmp_input_userName )
			<< BUILD_LOG( tmp_input_password )
			<< BUILD_LOG( wstrPurchTok )
			<< END_LOG;
	}

	// 성공 실패 여부에 상관없이 EPUBLISHER_AUTHENTICATION_ACK 보냄

	// Success Case //
	if( iResult == 1 || iResult == 101 
#ifdef SERV_STEAM
		|| iResult == 201 || iResult == 301 || iResult == 401 || iResult == 501
#endif //SERV_STEAM
#ifdef SERV_CHANNELING_AERIA
		|| iResult == 601 || iResult == 701
#endif //SERV_CHANNELING_AERIA
		)
	{
		KEPUBLISHER_AUTHENTICATION_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::NET_OK;
		kPacketAck.m_iResult = 1;
		kPacketAck.m_wstrIP = kPacket_.m_wstrIP;
		WCHAR wszK3RUID[ 100 ];
		MultiByteToWideChar( CP_ACP, 0, strK3RUID.c_str(), -1, wszK3RUID, 100 );
		kPacketAck.m_wstrID = wszK3RUID;
		kPacketAck.m_uiPublisherUID = (unsigned int)atol(strK3RUID.c_str());
#ifdef SERV_PURCHASE_TOKEN
		kPacketAck.wstrPurchTok = wstrPurchTok;
#endif SERV_PURCHASE_TOKEN
#ifdef SERV_STEAM
		// 채널링별 결과값
		// X(single integer) = 기존 북미
		// 10X  = 기존 남미
		// 20X  = 스팀 북미
		// 30X  = 스팀 남미
		// 40X  = 스팀(기존유저 변환) 북미
		// 50X  = 스팀(기존유저 변환 ) 남미
		// 60X	= 아리아 채널링 북미
		// 70X	= 아리아 채널링 남미
		// 확인이 편하게 이렇게 작업해 둡니다.
		switch(iResult)
		{
		case 1:// 기존 북미
			kPacketAck.m_iChannelingCode = 0;
			break;
		case 101:// 기존 남미
			kPacketAck.m_iChannelingCode = 1;
			break;
		case 201:// 스팀 북미
			kPacketAck.m_iChannelingCode = 2;
			break;
		case 301:// 스팀 남미
			kPacketAck.m_iChannelingCode = 3;
			break;
		case 401:
			kPacketAck.m_iChannelingCode = 4;
			break;
		case 501:
			kPacketAck.m_iChannelingCode = 5;
			break;
#ifdef SERV_CHANNELING_AERIA
		case 601:
			kPacketAck.m_iChannelingCode = 6;
			break;
		case 701:
			kPacketAck.m_iChannelingCode = 7;
			break;
#endif //SERV_CHANNELING_AERIA
		}
#else //SERV_STEAM
		kPacketAck.m_iChannelingCode = (int)(iResult/100);	// 0이면 북미, 1이면 남미
#endif //SERV_STEAM

		KEventPtr spEvent( new KEvent );
		spEvent->SetData( PI_CHANNEL_USER, NULL, EPUBLISHER_AUTHENTICATION_ACK, kPacketAck );
		KActorManager::GetKObj()->QueueingTo( iActorUID, spEvent );
	}

	// Fail Case //
	else
	{
		KEPUBLISHER_AUTHENTICATION_ACK kPacketAck;
		// SERV_STEAM
		// 2 : 스팀 북미 3: 스팀 남미 4: 전환 북미 5: 전환 남미 6: 아리아 북미 7: 아리아 남미
		kPacketAck.m_iChannelingCode = (int)(iResult/100);	// 0이면 북미, 1이면 남미

		//----- iResult -----//
		// 2 = User not found
		// 3 = Wrong Password
		// 4 = User not validated
		// 5 = User account is blocked 
		// 6 = IP Block
		// 7 = TOU Not Agreed 

		switch( iResult % 100 )
		{
		case 2:		kPacketAck.m_iOK = NetError::ERR_GAMEFORGE_01;	break;	// 2 = User not found
		case 3: 	kPacketAck.m_iOK = NetError::ERR_GAMEFORGE_00;	break;	// 3 = Wrong Password
		case 4: 	kPacketAck.m_iOK = NetError::ERR_GAMEFORGE_02;	break;	// 4 = User not validated
#ifdef SERV_ACCOUNT_BLOCK_MESSAGE_RENEWAL
			case 5: 	kPacketAck.m_iOK = NetError::ERR_ACCOUNT_BLOCK_04;		break;	// 5 = User account is blocked 
#else //SERV_ACCOUNT_BLOCK_MESSAGE_RENEWAL
			case 5: 	kPacketAck.m_iOK = NetError::ERR_VERIFY_11;		break;	// 5 = User account is blocked 
#endif //SERV_ACCOUNT_BLOCK_MESSAGE_RENEWAL
		case 6: 	kPacketAck.m_iOK = NetError::ERR_K3RCOMBO_AUTH_01;	break;	// 6 = IP Block
		case 7: // 7 = TOU Not Agreed 
			{
				kPacketAck.m_iOK = NetError::ERR_K3RCOMBO_AUTH_02;
#ifdef SERV_LOGIN_TOU
				kPacketAck.m_strAgreementURL = strURL;
#endif // SERV_LOGIN_TOU
			}
			break;

		default: // 인증 과정에서 시스템 오류가 발생하였습니다.
			{
				kPacketAck.m_iOK = NetError::ERR_VERIFY_21;

				START_LOG( cerr, L"[ErrorLog] confirmLoginOnWebDB Return ERROR. 인증 과정에서 시스템 오류가 발생하였습니다. " )
					<< BUILD_LOG( iResult )
					<< END_LOG;
			}
			break;
		}

		KEventPtr spEvent( new KEvent );
		spEvent->SetData( PI_CHANNEL_USER, NULL, EPUBLISHER_AUTHENTICATION_ACK, kPacketAck );
		KActorManager::GetKObj()->QueueingTo( iActorUID, spEvent );
	}

	START_LOG( clog, L"[테스트로그] K3R authentication result" )
		<< BUILD_LOG( kPacket_.m_iActorUID )
		<< BUILD_LOG( kPacket_.m_wstrServiceAccountID )
		<< BUILD_LOG( iResult )
		<< BUILD_LOG( wstrPurchTok )
		<< END_LOG;

}


IMPL_ON_FUNC( EPUBLISHER_SECURITY_AUTH_REQ )
{
	KEPUBLISHER_SECURITY_AUTH_ACK kPacketAck;

	//{{ 2010. 11. 25. 조효진	아무것도 입력되지 않은 경우 에러 처리함
	//if( m_mapUserUIDToK3RUID.find( kPacket_.m_iUserUID ) == m_mapUserUIDToK3RUID.end() )
	//{
	//	kPacketAck.m_iOK = NetError::ERR_GAMEFORGE_01;
	//	KEventPtr spEvent( new KEvent );
	//	spEvent->SetData( PI_CHANNEL_USER, NULL, EPUBLISHER_SECURITY_AUTH_ACK, kPacketAck );
	//	KActorManager::GetKObj()->QueueingTo( kPacket_.m_iUserUID, spEvent );

	//	return;
	//}
	//}}

	// 아래는 퍼블리셔와 통신해서 인증 하는 부분.
	// 사내 서버 경우는 이부분을 건너띄고, 바로 우리 Account DB 확인으로 넘어간다.
	int iResult = 1;
	std::string strResult = "";
	UidType iActorUID = kPacket_.m_iUserUID;

	std::string strK3RUID = "";
	std::string strURL = "";

	iResult = 0;
	strResult = "";

	struct _ns3__GetSecurityQuestionsStatus tmp_input;
	struct _ns3__GetSecurityQuestionsStatusResponse tmp_output;


	int iUserID = (int)kPacket_.m_uiPublisherUID;
	tmp_input.account_USCOREid = &iUserID;

	KWebMethod tmp_web_path;
	if( GetWebMethod(K3R_AUTH_SECOND_SECURITY, tmp_web_path) == false )
	{
		START_LOG( cerr, L"[ErrorLog] GetWebMethod ERROR. You must AddwebMethod K3R_AUTH_SECOND_SECURITY" )
			<< END_LOG;

		kPacketAck.m_iOK = NetError::ERR_VERIFY_21;

		KEventPtr spEvent( new KEvent );
		spEvent->SetData( PI_CHANNEL_USER, NULL, EPUBLISHER_SECURITY_AUTH_ACK, kPacketAck );
		KActorManager::GetKObj()->QueueingTo( iActorUID, spEvent );

		return;
	}
	
	//{{ 2010/11/11 조효진	soap 함수 호출 전에는 혹시 모르니 항상 확인하자.
	if( p_gf_soap == NULL )
	{// soap 모듈 정상 생성 안된 경우 // 예외처리
		return;
	}

	int soap_call_result = soap_call___ns1__GetSecurityQuestionsStatus( p_gf_soap,
		m_strWsdlPath.c_str(), tmp_web_path.m_strMethodPath.c_str(),
		&tmp_input, &tmp_output );

	// Soap 호출 오류 //
	if( 0 != soap_call_result )
	{
		char str_err[500];
		soap_sprint_fault(p_gf_soap, str_err, 500);

		START_LOG( cerr, L"[ErrorLog] soap_call___ns1__GetSecurityQuestionsStatus ERROR. " )
			<< BUILD_LOG( m_strWsdlPath )
			<< BUILD_LOG( tmp_web_path.m_strMethodPath )
			<< BUILD_LOG( str_err )
			<< END_LOG;

		kPacketAck.m_iOK = NetError::ERR_VERIFY_21;

		KEventPtr spEvent( new KEvent );
		spEvent->SetData( PI_CHANNEL_USER, NULL, EPUBLISHER_SECURITY_AUTH_ACK, kPacketAck );
		KActorManager::GetKObj()->QueueingTo( iActorUID, spEvent );

		return;
	}

	// 비정상적인 경우에 리턴값 가져오다 서버가 죽는걸 방지 하기 위해서
	if( tmp_output.GetSecurityQuestionsStatusResult != NULL )
	{
		// 리턴 값
		strResult = tmp_output.GetSecurityQuestionsStatusResult;

		START_LOG( clog2, L"제대로 나뉘었나 확인" )
			<< BUILD_LOG( strResult )
			<< END_LOG;
		std::vector< std::string > vecStringKey;
		vecStringKey.reserve( 3 );
		std::string strTemp = strResult;
		while( true )
		{
			int iSplitPoint = strTemp.find_first_of( "|" );
			if( iSplitPoint == std::string::npos )
			{
				vecStringKey.push_back( strTemp.c_str() );
				break;
			}

			vecStringKey.push_back( strTemp.substr( 0, iSplitPoint ).c_str() );
			strTemp = strTemp.substr( iSplitPoint + 1, strTemp.length() - iSplitPoint ).c_str();
		}

		iResult = std::atoi( vecStringKey[0].c_str() );
		strK3RUID = vecStringKey[1];
		strURL = vecStringKey[2];
		
		START_LOG( clog2, L"제대로 나뉘었나 확인" )
			<< BUILD_LOG( strResult )
			<< BUILD_LOG( iResult )
			<< BUILD_LOG( strK3RUID )
			<< BUILD_LOG( strURL )
			<< END_LOG;
	}
	// 비정상적인 경우 
	else
	{
		START_LOG( cerr, L"[ErrorLog] confirmLoginOnWebDBResult Return NULL!!!. SOAP ERROR!!! " )
			<< END_LOG;

		KEventPtr spEvent( new KEvent );
		spEvent->SetData( PI_CHANNEL_USER, NULL, EPUBLISHER_SECURITY_AUTH_ACK, kPacketAck );
		KActorManager::GetKObj()->QueueingTo( iActorUID, spEvent );

		return;
	}

	// 이미 보안패드 등록한 경우 Case //
	if( iResult == 1 )
	{
		kPacketAck.m_iOK = NetError::NET_OK;
		KEventPtr spEvent( new KEvent );
		spEvent->SetData( PI_CHANNEL_USER, NULL, EPUBLISHER_SECURITY_AUTH_ACK, kPacketAck );
		KActorManager::GetKObj()->QueueingTo( iActorUID, spEvent );
	}
	// Fail Case //
	else
	{
		kPacketAck.m_iOK = NetError::ERR_K3RCOMBO_AUTH_03;
		kPacketAck.m_strSecondSecurityURL = strURL;

		KEventPtr spEvent( new KEvent );
		spEvent->SetData( PI_CHANNEL_USER, NULL, EPUBLISHER_SECURITY_AUTH_ACK, kPacketAck );
		KActorManager::GetKObj()->QueueingTo( iActorUID, spEvent );
	}


	START_LOG( clog, L"[테스트로그] K3R authentication result" )
		<< BUILD_LOG( kPacket_.m_iUserUID )
		<< BUILD_LOG( iResult )
		<< END_LOG;

}

#endif // SERV_COUNTRY_US