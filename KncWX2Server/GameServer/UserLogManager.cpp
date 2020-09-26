#include "GSUser.h"
#include ".\userlogmanager.h"


#include <iomanip>
#include <sys/stat.h>


std::wfstream KUserLogManager::ms_wfsUserLog[ULT_MAX];


KUserLogManager::KUserLogManager(void)
{
}

KUserLogManager::~KUserLogManager(void)
{
}

void KUserLogManager::SaveSendPacket( u_short usEventID )
{
    if( m_SendPacketQueue.size() >= QUEUE_SIZE )
	{
		m_SendPacketQueue.pop();
	}

	CTime tTime = CTime::GetCurrentTime();
	m_SendPacketQueue.push( KEventLog( usEventID, tTime.GetTime() ) );
}

void KUserLogManager::SaveProcessEvent( u_short usEventID )
{
	if( m_ProcessPacketQueue.size() >= QUEUE_SIZE )
	{
		m_ProcessPacketQueue.pop();
	}

	CTime tTime = CTime::GetCurrentTime();
	m_ProcessPacketQueue.push( KEventLog( usEventID, tTime.GetTime() ) );
}

void KUserLogManager::DumpLogFile( IN KGSUserPtr spUser, IN USER_LOG_TYPE eUserLogType )
{
	if( !spUser )
		return;

	if( eUserLogType < 0  ||  eUserLogType >= ULT_MAX )
		return;

	std::wfstream& wfsUserLog = ms_wfsUserLog[eUserLogType];
	CTime tCurTime = CTime::GetCurrentTime();

	if( !wfsUserLog.is_open() )
	{
		std::string strFilename;

		switch( eUserLogType )
		{
		case ULT_HEART_BEAT:
			strFilename += "Log\\HeartBeatLog_";
			break;

		case ULT_ABNORMAL_DICONNECT:
			strFilename += "Log\\AbnormalDisconnectLog_";
			break;

		default:
			START_LOG( cerr, L"이상한 user log type입니다." )
				<< BUILD_LOG( eUserLogType )
				<< END_LOG;
            return;
		}
		
		strFilename += ( CStringA )( tCurTime.Format( _T( "%Y-%m-%d_%H%M%S" ) ) );
		strFilename += ".txt";

		struct _stat info;
		int result = _stat( strFilename.c_str(), &info );

		wfsUserLog.imbue(std::locale("korean", LC_CTYPE));
		wfsUserLog.open( strFilename.c_str(), std::ios::out | std::ios::app );

		// 헤더 출력
		wfsUserLog << L"   Time    " << L"\t";
		wfsUserLog << L"   UserUID " << L"\t";
		wfsUserLog << L"   CharUID " << L"\t";
		wfsUserLog << L" GSUserFSM " << L"\t";
		wfsUserLog << L"   MapID   " << L"\t";
		wfsUserLog << L" RoomListID" << L"\t";
		wfsUserLog << L"   RoomUID " << L"\t";
		wfsUserLog << L"   RoomType" << L"\t";
		wfsUserLog << L"   TradeUID" << L"\t";
		wfsUserLog << L"   PShopUID" << L"\t";
		wfsUserLog << L"   PartyUID" << L"\t";
		wfsUserLog << L" GetLastErr" << L"\t";
		wfsUserLog << L"   RecvGap " << L"\t";
		wfsUserLog << L"SendEvent 1" << L"\t";
		wfsUserLog << L"SendTime  1" << L"\t";
		wfsUserLog << L"SendEvent 2" << L"\t";
		wfsUserLog << L"SendTime  2" << L"\t";
		wfsUserLog << L"SendEvent 3" << L"\t";
		wfsUserLog << L"SendTime  3" << L"\t";
		wfsUserLog << L"SendEvent 4" << L"\t";
		wfsUserLog << L"SendTime  4" << L"\t";
		wfsUserLog << L"SendEvent 5" << L"\t";
		wfsUserLog << L"SendTime  5" << L"\t";
		wfsUserLog << L"ProcEvent 1" << L"\t";
		wfsUserLog << L"ProcTime  1" << L"\t";
		wfsUserLog << L"ProcEvent 2" << L"\t";
		wfsUserLog << L"ProcTime  2" << L"\t";
		wfsUserLog << L"ProcEvent 3" << L"\t";
		wfsUserLog << L"ProcTime  3" << L"\t";
		wfsUserLog << L"ProcEvent 4" << L"\t";
		wfsUserLog << L"ProcTime  4" << L"\t";
		wfsUserLog << L"ProcEvent 5" << L"\t";
		wfsUserLog << L"ProcTime  5" << L"\t";
	}

	DWORD dwRecvGap = ::GetTickCount() - spUser->GetHBTick();

#define KLocAlign std::setw(8) << std::setiosflags( std::ios::left )

	wfsUserLog << L"\n";
	wfsUserLog << KLocAlign << (const wchar_t*)tCurTime.Format(_T("%Y-%m-%d %H:%M:%S")) << L"\t";
	wfsUserLog << KLocAlign << spUser->GetUID() << L"\t";
	wfsUserLog << KLocAlign << spUser->GetCharUID() << L"\t";
	wfsUserLog << KLocAlign << spUser->GetStateIDString() << L"\t";
	wfsUserLog << KLocAlign << spUser->GetMapID() << L"\t";
	wfsUserLog << KLocAlign << spUser->GetRoomListID() << L"\t";
	wfsUserLog << KLocAlign << spUser->GetRoomUID() << L"\t";
	wfsUserLog << KLocAlign << (int)CXSLRoom::GetRoomType( spUser->GetRoomUID() ) << L"\t";
	wfsUserLog << KLocAlign << spUser->GetTradeUID() << L"\t";
	wfsUserLog << KLocAlign << spUser->GetPersonalShopUID() << L"\t";
	wfsUserLog << KLocAlign << spUser->GetPartyUID() << L"\t";
	wfsUserLog << KLocAlign << spUser->GetLastIocpFailedError() << L"\t";
	wfsUserLog << KLocAlign << dwRecvGap << L"\t";

	int iEmptyCount = 5;
	while( !m_SendPacketQueue.empty() )
	{
		KEventLog kLog = m_SendPacketQueue.front();
		m_SendPacketQueue.pop();

		CTime tTime( kLog.m_tTime );
		wfsUserLog << KLocAlign << KEvent::GetIDStr( kLog.m_usEventID ) << L"\t";
		wfsUserLog << KLocAlign << (const wchar_t*)tTime.Format(_T("%Y-%m-%d %H:%M:%S")) << L"\t";

		--iEmptyCount;
	}
	for( int i = 0; i < iEmptyCount; ++i )
	{
		wfsUserLog << KLocAlign << L"EMPTY_EVENT" << L"\t";
		wfsUserLog << KLocAlign << L"EMPTY_TIME" << L"\t";
	}

	iEmptyCount = 5;
	while( !m_ProcessPacketQueue.empty() )
	{
		KEventLog kLog = m_ProcessPacketQueue.front();
		m_ProcessPacketQueue.pop();

		CTime tTime( kLog.m_tTime );
		wfsUserLog << KLocAlign << KEvent::GetIDStr( kLog.m_usEventID ) << L"\t";
		wfsUserLog << KLocAlign << (const wchar_t*)tTime.Format(_T("%Y-%m-%d %H:%M:%S")) << L"\t";

		--iEmptyCount;
	}
	for( int i = 0; i < iEmptyCount; ++i )
	{
		wfsUserLog << KLocAlign << L"EMPTY_EVENT" << L"\t";
		wfsUserLog << KLocAlign << L"EMPTY_TIME" << L"\t";
	}

	wfsUserLog.flush();
}

