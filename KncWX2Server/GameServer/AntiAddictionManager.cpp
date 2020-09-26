#include "AntiAddictionManager.h"

#include "GSUser.h"
#include "KncSend.h"

#ifdef SERV_ANTI_ADDICTION_SYSTEM

int CalculateTime(const int day, const int hour, const int min, const int sec)
{
	return (sec + 60*( min + 60*( hour + 24*day )));
}

// 박교현 : [2010/03/09] //	중국 중독방지 방침미 시스템
AntiAddictionManager::AntiAddictionManager()
: m_OwnerUID(0)
, m_bEnable(false)
, m_RestrictionType(AART_NONE)
, m_iCurrentPlayTime(0)
{
}

AntiAddictionManager::~AntiAddictionManager()
{
	m_OwnerUID = 0;
}

void AntiAddictionManager::Init(UidType OwnerUID, const AntiAddictionInfo& info)
{
	m_OwnerUID = OwnerUID;
	m_Info = info;

	CTimeSpan tRestTime = CTimeSpan(m_Info.m_iRestTime) + (m_Info.m_tLastLogin - m_Info.m_tLastLogout);

	// 일정시간 이상 휴식했으면 플레이 시간 초기화
	// TODO: 추후 스크립트 로딩으로 변경
	CTimeSpan tMaxRestTime = CTimeSpan(0, 5, 0, 0);

	if(tRestTime > tMaxRestTime)
	{
		START_LOG( clog, L"최대 휴식시간 도달로 인한 방침미 데이터 리셋!" )
			<< BUILD_LOG( m_OwnerUID )
			<< BUILD_LOG( m_Info.m_tLastLogin.Format(_T( "%Y-%m-%d %H:%M:%S" )).GetBuffer() ) 
			<< BUILD_LOG( m_Info.m_tLastLogout.Format(_T( "%Y-%m-%d %H:%M:%S" )).GetBuffer() ) 
			<< BUILD_LOG( m_Info.m_iPlayTime ) 
			<< BUILD_LOG( m_Info.m_iRestTime )
			<< BUILD_LOG( tRestTime.Format(L"%D:%H:%M:%S").GetBuffer() )
			<< BUILD_LOG( tMaxRestTime.Format(L"%D:%H:%M:%S").GetBuffer() )
			<< END_LOG;

		UpdateTime(0, 0);
	}
	else
	{
		int iRestTime = CalculateTime(tRestTime.GetDays(), tRestTime.GetHours(), tRestTime.GetMinutes(), tRestTime.GetSeconds());
		UpdateTime(m_Info.m_iPlayTime, iRestTime);
	}

	m_iCurrentPlayTime = 0;
	m_NotifyTimer.restart();

	// 틱처리
	TickTypeCheck();
}

void AntiAddictionManager::Finalize()
{
	if(!m_bEnable)
		return;

	m_Info.m_tLastLogout = CTime::GetCurrentTime();
	int iPlayTime = m_Info.m_iPlayTime + static_cast<int>(m_Info.m_tLastLogout.GetTime() - m_Info.m_tLastLogin.GetTime());
	UpdateTime(iPlayTime, m_Info.m_iRestTime);
}

void AntiAddictionManager::Tick()
{
	if(!m_bEnable)
		return;

	// 60초에 한번씩 체크
	const static double check_time = 1/*mins*/ * 60/*secs*/;

	// 1분마다 체크
	if( m_RefreshTimer.elapsed() < check_time )
		return;

	// 타이머 초기화	
	m_RefreshTimer.restart();

	// 틱처리
	TickTypeCheck();
	TickNotifyCheck();
}

void AntiAddictionManager::TickTypeCheck()
{
	CTime tTime	= CTime::GetCurrentTime();
	CTimeSpan tCurrentPlayTime = tTime - m_Info.m_tLastLogin;
	m_iCurrentPlayTime = static_cast<int>(tCurrentPlayTime.GetTimeSpan());
	int iTotalPlayTime = m_iCurrentPlayTime + m_Info.m_iPlayTime;

	// TODO: 추후 스크립트 로딩으로 변경
	static const int FullTime	= CalculateTime(0, 5, 0, 0);
//	static const int FullTime	= CalculateTime(0, 0, 20, 0);
	static const int HalfTime	= CalculateTime(0, 3, 0, 0);
//	static const int HalfTime	= CalculateTime(0, 0, 10, 0);

	AA_RESTRICTION_TYPE NewRestrictionType = AART_NONE;
	// 보상타입 변경 처리
	if(iTotalPlayTime > FullTime)
	{
		NewRestrictionType = AART_FULL;
	}
	else if(iTotalPlayTime > HalfTime)
	{
		NewRestrictionType = AART_HALF;
	}
	else
		NewRestrictionType = AART_NONE;

	// 타입이 바뀌면 공지
	if(NewRestrictionType != m_RestrictionType)
	{
		START_LOG( clog, L"방침미 타입 변경!" )
			<< BUILD_LOG( m_OwnerUID )
			<< BUILD_LOG( m_Info.m_tLastLogin.Format(_T( "%Y-%m-%d %H:%M:%S" )).GetBuffer() ) 
			<< BUILD_LOG( m_Info.m_tLastLogout.Format(_T( "%Y-%m-%d %H:%M:%S" )).GetBuffer() ) 
			<< BUILD_LOG( m_Info.m_iPlayTime ) 
			<< BUILD_LOG( m_Info.m_iRestTime )
			<< BUILD_LOG( tCurrentPlayTime.GetTimeSpan() )
			<< BUILD_LOG( m_RestrictionType )
			<< BUILD_LOG( NewRestrictionType )
			<< END_LOG;

		m_RestrictionType = NewRestrictionType;
		Notify();
	}
}

void AntiAddictionManager::TickNotifyCheck()
{
	// 경고 패킷 전송
	// TODO: 추후 스크립트 로딩으로 변경
//	static const double UnhealthyTimer = CalculateTime(0, 0, 15, 0);
	static const double UnhealthyTimer = CalculateTime(0, 0, 5, 0);
//	static const double HelthyTimer	= CalculateTime(0, 0, 30, 0);
	static const double HelthyTimer	= CalculateTime(0, 0, 5, 0);

	double timer = 0;
	switch(m_RestrictionType)
	{
	case AART_FULL:
		timer = UnhealthyTimer;						// 불건강한 시간이면 15분마다
		break;
	case AART_HALF:
	case AART_NONE:
		timer = HelthyTimer;						// 그외에는 30분마다
		break;
	default:
		START_LOG( cerr, L"방침미 타입이 이상함!" )
			<< BUILD_LOG( m_OwnerUID )
			<< BUILD_LOG( m_Info.m_tLastLogin.Format(_T( "%Y-%m-%d %H:%M:%S" )).GetBuffer() ) 
			<< BUILD_LOG( m_Info.m_tLastLogout.Format(_T( "%Y-%m-%d %H:%M:%S" )).GetBuffer() ) 
			<< BUILD_LOG( m_Info.m_iPlayTime ) 
			<< BUILD_LOG( m_Info.m_iRestTime )
			<< BUILD_LOG( m_RestrictionType )
			<< BUILD_LOG( m_iCurrentPlayTime )
			<< END_LOG;
		break;
	}

	if(m_NotifyTimer.elapsed() > timer)
	{
		Notify();
	}
}

float AntiAddictionManager::GetRewardRatio() const
{
	if(!m_bEnable)
		return 1.0f;

	switch(m_RestrictionType)
	{
	case AART_NONE:
		return 1.0f;
	case AART_HALF:
		return 0.5f;
	case AART_FULL:
		return 0.0f;
	default:
		START_LOG( cerr, L"방침미 타입이 이상함!" )
			<< BUILD_LOG( m_OwnerUID )
			<< BUILD_LOG( m_Info.m_tLastLogin.Format(_T( "%Y-%m-%d %H:%M:%S" )).GetBuffer() ) 
			<< BUILD_LOG( m_Info.m_tLastLogout.Format(_T( "%Y-%m-%d %H:%M:%S" )).GetBuffer() ) 
			<< BUILD_LOG( m_Info.m_iPlayTime ) 
			<< BUILD_LOG( m_Info.m_iRestTime )
			<< BUILD_LOG( m_RestrictionType )
			<< BUILD_LOG( m_iCurrentPlayTime )
			<< END_LOG;

		return 1.0f;
	}
}

bool AntiAddictionManager::IsEnable() const
{
	return m_bEnable;
}

void AntiAddictionManager::UpdateTime(int iPlayTime, int iRestTime)
{
	m_Info.m_iPlayTime = iPlayTime;
	m_Info.m_iRestTime = iRestTime;

	// DB에도 적용하자
	KDBE_UPDATE_ANTI_ADDICTION_INFO kPacket;
	kPacket.m_iUserUID = m_OwnerUID;
	kPacket.m_iPlayTime = iPlayTime;
	kPacket.m_iRestTime = iRestTime;
	kPacket.m_wstrLastLogoutTime = m_Info.m_tLastLogout.Format( _T( "%Y-%m-%d %H:%M:%S" ) );

	UidType anTrace[2] = { m_OwnerUID, -1 };
	KncSend( PI_GS_USER, m_OwnerUID, PI_ACCOUNT_DB, 0, anTrace, DBE_UPDATE_ANTI_ADDICTION_INFO, kPacket );
}

void AntiAddictionManager::Notify()
{
	KEGS_ANTI_ADDICTION_NOT kPacket;
	if(m_RestrictionType == AART_FULL)
		kPacket.m_iType = KEGS_ANTI_ADDICTION_NOT::NT_CENTER;
	else
		kPacket.m_iType = KEGS_ANTI_ADDICTION_NOT::NT_CHAT;
	kPacket.m_iPlayTime = m_Info.m_iPlayTime + m_iCurrentPlayTime;

	UidType anTrace[2] = { m_OwnerUID, -1 };
	KncSend( PI_GS_USER, m_OwnerUID, PI_GS_USER, m_OwnerUID, anTrace, EGS_ANTI_ADDICTION_NOT, kPacket );

	m_NotifyTimer.restart();
}

#endif SERV_ANTI_ADDICTION_SYSTEM