#pragma once

#include "KTCPSocket.h"
#include "KPacket.h"
#include "KQuery.h"
#include "map"

#define SET_TIME( sysTime, hour, minute, sec, milSec ) \
sysTime.wHour = hour; \
sysTime.wMinute = minute; \
sysTime.wSecond = sec; \
sysTime.wMilliseconds = milSec; \

#define YESTERDAY_CODE( id )	id+1000

#define DECLARE_FINDIF( className, keyType, key ) \
struct FIND_IF \
{ \
	keyType m_iKey; \
	FIND_IF( keyType iKey ){ m_iKey = iKey; } \
	bool operator()( className& servInfo ) \
	{ \
		return ( m_iKey == servInfo.key ); \
	} \
}; \

struct AccountList
{
	std::wstring	m_id;
	std::wstring	m_pw;
	std::wstring	m_description;
	int				m_authLevel;
};

struct CCUInfo
{
	SYSTEMTIME  m_date;
	int			m_serverUID;
	int			m_CCUcount;	
};

struct OldTime
{
	int			m_iOldHour;
	int			m_iOldMinute;

	OldTime()
	{
		m_iOldHour	  = 0;
		m_iOldMinute  = 0;
	}
};

struct KFileDsnData
{
	int			m_iFileDsnID;
	std::string m_strFileDsnName;
	KQuery		m_kQuery;
	bool		m_bIsLog;
};

struct KESDDateStatistics 
{
	std::pair< SYSTEMTIME, std::wstring >	m_iDate;
	std::pair< int, std::wstring >			m_iTopCCU;
	std::pair< SYSTEMTIME, std::wstring >	m_iTopCCUTime;
	std::pair< int, std::wstring >			m_iDayAvgCCU;
	std::pair< int, std::wstring >			m_iWeekAvgCCU;
	std::pair< int, std::wstring >			m_iMonthAvgCCU;
	std::pair< int, std::wstring >			m_iJoinUser;
	std::pair< int, std::wstring >			m_iUnicUser;
	std::pair< int, std::wstring >			m_iTotalUnicUser;

	void clear()
	{		
		m_iTopCCU.first				= 0;
		m_iTopCCUTime.first.wHour	= 0;
		m_iTopCCUTime.first.wMinute = 0;
		m_iDayAvgCCU.first			= 0;
		m_iWeekAvgCCU.first			= 0;
		m_iMonthAvgCCU.first		= 0;
		m_iJoinUser.first			= 0;
		m_iUnicUser.first			= 0;
		m_iTotalUnicUser.first		= 0;
	}
};

struct KESDServerGroupInfo
{
	int				m_iServerGroupID;
	std::wstring	m_wstrGroupName;

	DECLARE_FINDIF( KESDServerGroupInfo, int, m_iServerGroupID );
};

struct KESDServerInfo
{
	std::wstring	m_wstrName;
	std::wstring	m_wstrIP;
	int				m_iNowTimeCCU;	
	int				m_serverUID;
	int				m_iFileDsnID;
	int				m_iServerGroupID;

	bool m_bIsRemoteShutdown;
	bool m_bConnected;
	bool m_bChecked;
	bool m_bGraphSelected;
	bool m_bIsLoginServer;
	int m_iLastRecvTime;

	bool m_bOnlineLgServer;
	bool m_bOnlineCnServer;
	bool m_bOnlineGmServer;
	bool m_bOnlineChServer;

	bool m_bBeforeLgState;
	bool m_bBeforeCnState;
	bool m_bBeforeGmState;
	bool m_bBeforeChState;

	bool m_bCrashLgServer;
	bool m_bCrashCnServer;
	bool m_bCrashGmServer;
	bool m_bCrashChServer;

	int				m_iYesterdayLineColorID;
	int				m_iTodayLineColorID;	
	std::wstring	m_strYesterdayColor;
	std::wstring	m_strTodayColor;

	KESDServerInfo() {};
	DECLARE_FINDIF( KESDServerInfo, int, m_serverUID );
};

class CKEsdRemoteController_ConsoleDlg;
class KEsdRemoteController_ConsoleImp
{
public:

	enum SERVER_STATE
	{
		Login_Server_Crash,
		Center_Server_Crash,
		Game_Server_Crash,
		Channel_Server_Crash,

		Login_Server_Restore,
		Center_Server_Restore,
		Game_Server_Restore,
		Channel_Server_Restore,
	};

	enum CCU_SP
	{
		TODAY_CCU = 0,
		YESTERDAY_CCU,
		GUESTID_CCU,
	};

public:
	KEsdRemoteController_ConsoleImp();
	virtual ~KEsdRemoteController_ConsoleImp();

	bool Initialize();
	void Destroy();
	void LoadConfig();

	void Update();
	void UpdateRecv();
	bool Select( SOCKET kSocket );

	void OnConnect();
	void OnEsdLaunch();
	void OnEsdLaunch_CO();
	void OnEsdLaunch_LG();
	void OnEsdLaunch_CH();

	void OnEsdWindowsUpdate();
	void OnEsdSVNUpdate();	
	void OnEsdHSBUpdate();
	void OnEsdVirusCheck();
	void OnEsdDBConnectionIDPW( const std::string strID, const std::string strPW );
	
	void OnEsdShutdown();

	void OnEsdStatus( KPacket& kPacket );
	void OnRecv( KPacket& kPacket );

	int DoQueryLogin( const std::wstring& wstrID, const std::wstring& wstrPassword );

	// 운영자 계정
	int	GetAuthLevel() { return m_authLevel; }
	AccountList& GetAccountInfo( int index ) { return m_vecAccountList[index]; }

	int GetEsdServerInfoCount();
	KESDServerInfo* GetEsdServerInfoAt( int iServerGroupID, int iIdx );
	std::vector< KESDServerInfo >& GetEsdServerList() { return m_vecEsdServerInfo; }
	const std::vector< KESDServerGroupInfo >& GetServerGroupList() const { return m_vecServerGroupInfo; }

	void SetEsdServerSelectClear();

	// 동접 그래프	
	bool GetConcurrentUserFromDB( IN CCU_SP eState, IN int iServerUID, IN int iFileDsnID, OUT std::vector< CCUInfo >& vecCCU, OUT int& iCCU );
	bool GetTodayConcurrentUserFromDB( IN int iServerUID, IN int iFileDsnID, OUT std::vector< CCUInfo >& vecTodayCCU, OUT int& iCCU );
	bool GetYesterdayConcurrentUserFromDB( IN int iServerUID, IN int iFileDsnID, OUT std::vector< CCUInfo >& vecYesterdayCCU, OUT int& iCCU );
	bool GetGuestIDConcurrentUserFromDB( IN int iServerUID, IN int iFileDsnID, OUT std::vector< CCUInfo >& vecGuestUserCCU, OUT int& iCCU );

	bool GetDateConcurrentUserFromDB( IN int iServerUID, IN std::wstring wstrDate, IN int iFileDsnID, OUT std::vector< CCUInfo >& vecCCU, OUT int& iCCU );
	bool GetTimeConcurrentUserFromDB( IN int iServerUID, IN std::wstring wstrDate, IN int iFileDsnID, OUT std::vector< CCUInfo >& vecCCU, OUT int& iCCU );
	bool GetDateStatisticsFromDB( IN std::wstring wstrDate, IN int iFileDsnID, OUT KESDDateStatistics& dateStatistics );
	void SetLastCheckTime( SYSTEMTIME& lastCheckTime ) { m_lastCheckTime = lastCheckTime; }
	
	COLORREF GetLineColor( int iLineColorID );
	
	// 서버 인증
	int AuthenticateServer( const std::wstring& wstrIPAddr, int iFileDsnID );

	// 서버 크래쉬 후처리
	void ServerCrash( std::wstring& serverName, SERVER_STATE serverState );
	void ServerRestore( std::wstring& serverName, SERVER_STATE serverState );
	
	// ETC
	int GetGraphMaxValue() { return m_iMaxValue; }
	void SetGraphMaxValue( int iGraphMaxValue ) { m_iMaxValue = iGraphMaxValue; }
	int GetGraphRefreshTime() { return m_iGraphRefreshTime; }

	// Script
	bool InitMornitoringToolOption_Lua();
	bool AddServerGroupInfo_Lua();
	bool AddServerInfo_Lua();
	bool AddLineColor_Lua();
	bool AddFileDsn_Lua();	

	// 전체동접 그래프
	int GetTotalCCUTodayLineColor() { return m_iTotalCCUTodayCID; }
	int GetTotalCCUYesLineColor() { return m_iTotalCCUYesCID; }
	int GetAverageCCULineColor() { return m_iAverageCCUColorID; }
	int GetGuestUserCCULineColor() { return m_iGuestUserLineColorID; }

	// DBConnection 파일 만들기
	bool CreateDBConnectionFile( std::wstring wstrTargetFilePath );

public:	
	bool IsRefreshListBoxAndReset() { bool bTemp = m_bIsRefreshListBox; m_bIsRefreshListBox = false; return bTemp; }

protected:
	// 소켓
	std::vector< KTCPSocket >		 m_vecSocket;
	std::vector< KESDServerGroupInfo > m_vecServerGroupInfo;
	std::vector< KESDServerInfo >	 m_vecEsdServerInfo;
	std::vector< AccountList >		 m_vecAccountList;
	int								 m_iEsdServerPort;

	// 그래프
	SYSTEMTIME						 m_lastCheckTime;
	std::map< int, COLORREF >		 m_mapLineColor;
	int						 		 m_iMaxValue;

	std::vector< int >				 m_vecAuthServer;	
	int								 m_iGraphRefreshTime; // 분당 그래프갱신
	
	// ODBC
	std::map< int, KFileDsnData* >	 m_mapFileDsn;
	typedef std::map< int, KFileDsnData* >::iterator KFileDsnDataIterator;
	//KQuery						 	 m_kQuery;
	KQuery							 m_kStQuery; // 통계DB용
	int								 m_iLoginAccountDsnID;

	// acount
    int								 m_authLevel;
	int								 count;

	// 전체 동접 그래프
	int								 m_iTotalCCUTodayCID;
	int								 m_iTotalCCUYesCID;
	int								 m_iAverageCCUColorID;
	int								 m_iGuestUserLineColorID;
	
	bool							 m_bIsRefreshListBox;
};