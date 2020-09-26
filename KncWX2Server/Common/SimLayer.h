#pragma     once

#include <iostream>
#include "Actor.h"          // KActorPtr
#include <KNCSingleton.h>
#include "ThreadManager.h"
//{{ 2011. 12. 14	최육사	SimLayer Tick퍼포먼스 체크
#ifdef SERV_SIMLAYER_TICK_PERFORMANCE_CHECK
	#include "TickDumper.h"
#endif SERV_SIMLAYER_TICK_PERFORMANCE_CHECK
//}}
//{{ 2013. 02. 	최육사	Tick퍼포먼스 DB로그 기록
#ifdef SERV_SIMLAYER_TICK_PERFORMANCE_DB_LOG
	#include "SimLayerTickChecker.h"
#endif SERV_SIMLAYER_TICK_PERFORMANCE_DB_LOG
//}}

#include "CommonPacket.h"	// 해외 통합 시 집어넣음

//////////////////////////////////////////////////////////////////////////
// 스크립트 파싱 매크로
#define OPEN_SCRIPT_FILE( classname ) \
	{ \
		if( Si##classname()->OpenScriptFile( g_pLua ) == false ) \
			START_LOG( cerr, L#classname L" 정보 로드 실패.!" ); \
		else \
			START_LOG( cout, L#classname L" 정보 로드 성공.!" ); \
		if( Si##classname()->IsLuaPasingSuccess() == false ) \
		{ \
			KBaseServer::GetKObj()->AddFailScriptFileName( Si##classname()->GetParsingLuaFileName().c_str() ); \
		} \
	} \


class KSimLayer : public KThread
{
    DeclInstanceKeeping( KSimLayer );
    DeclToStringW;
    NiDeclareRootRTTI( KSimLayer );
	//{{ 2011. 12. 14	최육사	SimLayer Tick퍼포먼스 체크
#ifdef SERV_SIMLAYER_TICK_PERFORMANCE_CHECK
	DeclareTickDumper;
#endif SERV_SIMLAYER_TICK_PERFORMANCE_CHECK
	//}}
protected:
    KSimLayer();
public:
    virtual ~KSimLayer();

	//{{ 2012. 05. 10	최육사	서버 버전
//#ifdef SERV_VERSION_FLAG
	enum VersionFlag
	{
		VF_NONE = 0,
		VF_INTERNAL,
		VF_OPEN_TEST,
		VF_SERVICE,
	};
//#endif SERV_VERSION_FLAG
	//}}

    enum CommonFlag
    {
        CF_NONE         = 0x00000000,
        CF_NPGG         = 0x00000001,   // nProtect Game Guard.
        CF_CHECK_IP     = 0x00000002,   // check IP
		CF_HSHIELD		= 0x00000004,	// HackShield
// #ifdef SERV_USE_XTRAP
		CF_XTRAP		= 0x00000008,	// X-Trap
// #endif SERV_USE_XTRAP
    };

    enum AuthFlag                       // 060411. 인증 방식. 한번에 하나의 방식만 설정할 수 있어야 한다.
    {
        AF_NONE             = 0x00000000,
        AF_INTERNAL         = 0x00000001,   // 사내 인증.
        AF_NEXON_KOREA      = 0x00000002,   // 넥슨 한국
        AF_GLOBAL_SERVICE   = 0x00000003,   // 해외는 서비스 이걸로 통일 (퍼블 인증 시스템 연결여부)
    };

    enum BillingFlag
    {
        BF_NONE				= 0x00000000,
		BF_INTERNAL			= 0x00000001,
		BF_NEXON_KOREA_TEST = 0x00000002,	// KR 용
        BF_NEXON_KOREA		= 0x00000003,	// KR 용
        BF_GLOBAL_SERVICE   = 0x00000004,	// 해외는 서비스 이걸로 통일 (퍼블 빌링 시스템 연결여부)
    };	

	//{{ 2013.03.13 조효진	해외 전국가 Nation Flag 사용 결정
#ifdef SERV_USE_NATION_FLAG
	enum NationFlag
	{
		NF_NONE			= 0x00000000,
		NF_DE			= 0x00000001,
		NF_FR			= 0x00000002,
		NF_IT			= 0x00000003,
		NF_PL			= 0x00000004,
		NF_ES			= 0x00000005,
		NF_TR			= 0x00000006,
		NF_EN			= 0x00000007,

		NF_KR			= 0x00000100,
		NF_JP			= 0x00000200,
		NF_TW			= 0x00000300,
		NF_HK			= 0x00000301,
		NF_US			= 0x00000400,
		NF_CN			= 0x00000600,
		NF_VN			= 0x00000700,
		NF_TH			= 0x00000800,
		NF_ID			= 0x00000900,
		NF_BR			= 0x00001000,
		NF_PH			= 0x00001100,
		NF_IN			= 0x00001200,
	};
#endif SERV_USE_NATION_FLAG
	//}}
#ifdef SERV_COUNTRY_JP
	enum HanInitFlag
	{
		HIF_NONE				= 0x00000000,
		HIF_ALPHA				= 0x00000001,
		HIF_REAL				= 0x00000002,
	};
#endif //SERV_COUNTRY_JP

#ifdef SERV_GLOBAL_BILLING
	enum PublisherBillingConnectType
	{
		PBCT_NONE				= 0,	
		// kncSend 떄문에 DBLayer와 일반 Manager 는 구분되어야 함
		PBCT_NORMAL		= 1,	// 일반적인 Manager 통해 (보통은 TCP, SOAP 등)
		PBCT_DB			= 2,	// DB 통신 Manager를 통해
		PBCT_UNUSUAL	= 3,	// 섞어 쓰던가 특이 사항 있는 경우
	};

#endif //SERV_GLOBAL_BILLING

    // override from KPerformer
    virtual void Init();
    virtual void Tick();
    virtual void ShutDown();

    // derive to Child...
    virtual void RegToLua();

    // for lua.
	//{{ 2012. 05. 10	최육사	서버 버전
	//#ifdef SERV_VERSION_FLAG
	DWORD GetVersionFlag() const					{ return m_dwVersionFlag; }
	void SetVersionFlag( DWORD dwFlag )				{ m_dwVersionFlag = dwFlag; }
	//#endif SERV_VERSION_FLAG
	//}}

    void AddCommonFlag( DWORD dwFlag );
    void DeleteCommonFlag( DWORD dwFlag );
    bool CheckCommonFlag( DWORD dwFlag ) const      { KLocker lock( m_csCommonFlag ); return ( m_dwCommonFlag & dwFlag ) > 0; }

    DWORD GetAuthFlag() const                       { return m_dwAuthFlag; }
    void SetAuthFlag( DWORD dwFlag )                { m_dwAuthFlag = dwFlag; }

#ifdef SERV_COUNTRY_TH
	void SetAuthAccount( const char* pStrID, const char* pStrPassword, const char* pStrDomain )
	{
		m_wstrAuthID = KncUtil::toWideString(pStrID);
		m_wstrAuthPassword = KncUtil::toWideString(pStrPassword);
		m_wstrDomain = KncUtil::toWideString(pStrDomain);
	}
	std::wstring GetAuthID()		{	return m_wstrAuthID; }
	std::wstring GetAuthPassword()	{	return m_wstrAuthPassword; }
	std::wstring GetAuthDomain()	{	return m_wstrDomain; }
#endif // SERV_COUNTRY_TH


    DWORD GetBillingFlag() const                    { return m_dwBillingFlag; }
    void SetBillingFlag( DWORD dwFlag )             { m_dwBillingFlag = dwFlag; }

	//{{ 2009. 7. 13  최육사	하드코딩 제거
	void AddPath( const char* pStrPath );
	//}}

	//{{ 2013. 02. 	최육사	Tick퍼포먼스 DB로그 기록
#ifdef SERV_SIMLAYER_TICK_PERFORMANCE_DB_LOG
	DWORD GetSimLayerMaxTick() const				{ return m_kTickChecker.GetSimLayerMaxTick(); }
	const std::wstring GetSimLayerMaxTickRegDate() const	{ return m_kTickChecker.GetSimLayerMaxTickRegDate(); }
	void TickDumpReset()							{ m_kTickChecker.Reset(); }
#endif SERV_SIMLAYER_TICK_PERFORMANCE_DB_LOG
	//}}
	//{{ 2009. 12. 15  최육사	서버관리
	virtual void DumpToLogFile();
	//}}
    const std::wstring GetAuthTypeStr( DWORD dwFlag ) const;
	
#ifdef SERV_KOG_OTP_VERIFY
	bool GetUseKogOTP() { return m_bUseKogOTP; }
	void SetUseKogOTP(bool bUseKogOTP) { m_bUseKogOTP = bUseKogOTP; }
#endif // SERV_KOG_OTP_VERIFY

#ifdef SERV_GLOBAL_BILLING
	DWORD GetPublisherBillingConnectType() const { return m_dwPublisherBillingConnectType; }
	void SetPublisherBillingConnectType(DWORD dwPublisherBillingConnectType) { m_dwPublisherBillingConnectType = dwPublisherBillingConnectType; }

	bool IsCheckCouponByPublisher() { return m_bCheckCouponByPublisher; }	// 쿠폰번호 퍼블리셔 체크 갔다오는지 여부
	void SetCheckCouponByPublisher(bool bCheckCouponByPublisher) { m_bCheckCouponByPublisher = bCheckCouponByPublisher; }
#endif // SERV_GLOBAL_BILLING

protected:  // util function
    virtual void Run(); // derived from KThread

    std::vector<KThreadManager*>    m_vecpThreadMgr;
	//{{ 2012. 05. 10	최육사	서버 버전
	//#ifdef SERV_VERSION_FLAG
	DWORD							m_dwVersionFlag;
	//#endif SERV_VERSION_FLAG
	//}}
    DWORD                           m_dwCommonFlag;
    mutable KncCriticalSection      m_csCommonFlag;
    DWORD                           m_dwAuthFlag;           // 060411. florist. 서버 인증 방식.
    DWORD                           m_dwBillingFlag;
	//{{ 2013. 02. 	최육사	Tick퍼포먼스 DB로그 기록
#ifdef SERV_SIMLAYER_TICK_PERFORMANCE_DB_LOG
	KSimLayerTickChecker			m_kTickChecker;
#endif SERV_SIMLAYER_TICK_PERFORMANCE_DB_LOG
	//}}
	//{{ 2013.03.13 조효진	해외 전국가 Nation Flag 사용 결정
#ifdef SERV_USE_NATION_FLAG
	DWORD							m_dwNationFlag;
public:
	void							SetDwNationFlag(DWORD dwNationFlag){ m_dwNationFlag = dwNationFlag; }
	DWORD							GetDwNationFlag(){ return m_dwNationFlag; }
#endif SERV_USE_NATION_FLAG
	//}}

#ifdef SERV_COUNTRY_JP
	DWORD                           m_dwHanInitFlag;

	//{{ 2010. 10  김정협 일본 빌링
	DWORD GetHanInitFlag() const                    { return m_dwHanInitFlag; }
	void SetHanInitFlag( DWORD dwFlag )             { m_dwHanInitFlag = dwFlag; }	
	//}} 2010. 10  김정협 일본 빌링
#endif //SERV_COUNTRY_JP

#ifdef SERV_KOG_OTP_VERIFY
	bool							m_bUseKogOTP;
#endif // SERV_KOG_OTP_VERIFY

#ifdef SERV_COUNTRY_TH
	std::wstring					m_wstrAuthID;
	std::wstring					m_wstrAuthPassword;
	std::wstring					m_wstrDomain;
#endif // SERV_COUNTRY_

#ifdef SERV_GLOBAL_BILLING
	DWORD							m_dwPublisherBillingConnectType;
	
	bool							m_bCheckCouponByPublisher;
#endif // SERV_GLOBAL_BILLING

    std::map< DWORD, std::wstring > m_mapAuthTypeStrings;   // 인증 타입의 문자열들.

};

//임시 로딩타임 계산용 구조체를 만든다.
struct LoadingTimer
{
	LoadingTimer( std::wstring wstrMSG ){ m_wstrMSG = wstrMSG; m_dwTime = GetTickCount(); }
	~LoadingTimer()
	{
		m_wstrMSG += L" 로딩시간 : ";
		START_LOG( cout2, m_wstrMSG )
			<< BUILD_LOG( GetTickCount() - m_dwTime );
	}

	std::wstring	m_wstrMSG;
	DWORD			m_dwTime;
};
