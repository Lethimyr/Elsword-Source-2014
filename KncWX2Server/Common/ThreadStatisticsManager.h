#pragma once

#include "Statistics.h"
#include "KStatisticsLocalLog.h"
#include "KLogReporter.h"
#include "ThreadManager.h"
#include "TSMEventProcess.h"


#ifdef SERV_STATISTICS_THREAD


class KThreadStatisticsManager : public KThread
{
	friend class KStatistics;
	friend class KStatisticsLocalLog;
	friend class KTSMEventProcess;

    DeclToStringW;
    DeclDump;

protected:
	virtual void Run(); // derived from KThread

public:
    KThreadStatisticsManager();
    virtual ~KThreadStatisticsManager();

	void Init();
    void Shutdown();
	void RegToLua();

	//////////////////////////////////////////////////////////////////////////	
	// DB통계 업데이트 함수
	void IncreaseCount( int iStatisticsID, const KStatisticsKey& kKey, int iIndex, int iCount );
	void IncreaseCount( const KE_STATISTICS_INFO_NOT& kInfo );

	//////////////////////////////////////////////////////////////////////////	
	// 통계 관련 이벤트 큐잉 함수
	template < class T >
	void QueueingEvent( unsigned short usEventID, const T& data );
	void QueueingEventID( unsigned short usEventID );

	//////////////////////////////////////////////////////////////////////////
	// 통계 관련 플래그
	bool IsUserLog() { return m_bUserLog; }
	//{{ 2010. 10. 11	최육사	넥슨 빌링 패킷 로그
#ifdef SERV_BILLING_PACKET_LOG
	bool IsBillingPacketLog() { return m_bBillingPacketLog; }
#endif SERV_BILLING_PACKET_LOG
	//}}

	//////////////////////////////////////////////////////////////////////////	
	// Time Util Func
	static int GetCurLocalTime();
	static void AttachTimeTag( std::wostringstream& os );
	static void AttachSvnameTag( std::wostringstream& os );

protected:
	void _QueueingEvent( const KEventPtr& spEvent );

	// for lua
	void AddStatistics_LUA( int iStatisticsID, float fFlushGap, bool bWriteToDB );
	//{{ 2011. 04. 13	최육사	아이템 통계 DB기록
#ifdef SERV_ITEM_STATISTICS_TO_DB
	void SetStatisticsFlushTime_LUA( int iStatisticsID, int iHour );
#endif SERV_ITEM_STATISTICS_TO_DB
	//}}
	void SetSendToFTPStatistics_LUA( bool bSendToFTPStatistics );	// FTP 로그 전송에 대한 flag값
	void SetUserLog_LUA( bool bVal );
	//{{ 2010. 10. 11	최육사	넥슨 빌링 패킷 로그
#ifdef SERV_BILLING_PACKET_LOG
	void SetBillingPacketLog_LUA( bool bVal );
#endif SERV_BILLING_PACKET_LOG
	//}}

	void Tick();
	void FlushData();
    
	// Statistics Function	
	void DumpStatistics( int iStatisticsID ) const;
	bool CheckStatistics( int iStatisticsID );
	bool UpdateStatistics( int iStatisticsID, const KStatisticsKey& kKey, int iIndex, int iCount );

	// Local Log
	KStatisticsLocalLog& GetLocalLog() { return m_kStatisticsLocalLog; }

	void CheckFlushLocalLog();
	void UpdateFlushLocalLog();
	void OnFlushLocalLog( bool bClearLocalLog = false );

	void InitLocalLogKeys();
	void WriteLocalLogData( std::wfstream& fout, int iStatisticsID );
	void ClearLocalLogData( int iStatisticsID );
	void ClearLocalLogData_();

	// local log backup
	void BackUpAndSendFTPLocalLog( bool bSendFileFtp );
	void SendHenirRankingLog( int iRankingType );
	void BackUpSendFTPFiles( const std::vector< KLogReporter::KFileInfo >& vecFileList ); // FTP전송 파일 백업

	//{{ 2011. 11. 21  김민성	통계 로그 수치
#ifdef SERV_STATISTICS_LOG_COUNT
	 void UpdateStatisticsLogCount( IN UidType iCount );
	 void WriteStatisticsLogCount();
#endif SERV_STATISTICS_LOG_COUNT
	//}}

protected:
	bool							m_bIsInit;
    std::map< int, KStatisticsPtr > m_mapStatistics;

	int								m_iLocalLogLastUpdateTime;
	int								m_iLocalLogLastCheckTime;

	KStatisticsLocalLog				m_kStatisticsLocalLog;

	bool							m_bSendToFTPStatistics; // 사내서버라면 로그를 전송하지 않음
	bool							m_bUserLog;				// 유저 로그
	//{{ 2010. 10. 11	최육사	넥슨 빌링 패킷 로그
#ifdef SERV_BILLING_PACKET_LOG
	bool							m_bBillingPacketLog;
#endif SERV_BILLING_PACKET_LOG
	//}}

	std::wstring					m_wstrFTPServerIP;
	int								m_iFTPPort;
	std::wstring					m_wstrFTPID;
	std::wstring					m_wstrFTPPW;

	KTSMEventProcess				m_kTSMEventProcess;

	//{{ 2011. 11. 21  김민성	통계 로그 수치
#ifdef SERV_STATISTICS_LOG_COUNT
	int										m_iStatisticsFileNo;
	boost::timer							m_tStatisticsLogCount;
	std::map< std::wstring, UidType >		m_mapStatisticsLogCount;
#endif SERV_STATISTICS_LOG_COUNT
	//}}
};

template < class T >
void KThreadStatisticsManager::QueueingEvent( unsigned short usEventID, const T& data )
{
	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_NULL, NULL, usEventID, data );
	_QueueingEvent( spEvent );
}

#define KSIManager	KBaseServer::GetKObj()->m_kTStatisticsManager



#endif SERV_STATISTICS_THREAD