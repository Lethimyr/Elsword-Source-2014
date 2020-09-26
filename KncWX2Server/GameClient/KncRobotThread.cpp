#include "KncRobotThread.h"

#include <dbg/dbg.hpp>
#include "GSFSM.h"

ImplToStringW( KncRobotThread )
{
	char szMsg[MAX_PATH] = "";
	sprintf( szMsg, "[ID: %d]*** KncRobotThread ***", m_iID );

	return stm_ << szMsg << std::endl
		<< TOSTRINGW( GetNum() )
		<< TOSTRINGW( GetLiveNum() );
}

KncRobotThread::KncRobotThread( int iID, FSMclassPtr spKFSM )
:
m_iID( iID ),
m_spFSM( spKFSM )
{
	InitializeCriticalSection( &m_csRobot );
}

KncRobotThread::~KncRobotThread()
{
	DeleteCriticalSection( &m_csRobot );
}

bool KncRobotThread::Init()
{
	return KThread::Begin();
}

bool KncRobotThread::ShutDown()
{
	KThread::End();

	return true;
}

void KncRobotThread::Run()
{
	srand( ( unsigned int )time( NULL ) );      // 쓰레드마다 해줘야 한다.

	DWORD ret;

	m_kTimer.Restart();

	while( true )
	{
		ret = ::WaitForSingleObject( m_hKillEvent, 50 );       // 0.05s 간격

		if( ret == WAIT_OBJECT_0 ) break;

		if( ret == WAIT_TIMEOUT ) Tick();

		else std::cout << "*** WaitForSingleObject() - return : " << ret << std::endl;

	}

	return;
}

void KncRobotThread::Tick()
{
	m_kTimer.Begin();

	std::vector< KncRobotPtr >::iterator vit;

	KLocker lock( m_csRobot );
	for( vit = m_vecRobot.begin(); vit != m_vecRobot.end(); vit++ )
	{
		( *vit )->Tick();
	}

	m_kTimer.End();
}

int KncRobotThread::GetNum() const
{
	KLocker lock( m_csRobot );

	return ( int )m_vecRobot.size();
}

int KncRobotThread::GetLiveNum() const
{
	KLocker lock( m_csRobot );

	int iCount = 0;
	std::vector< KncRobotPtr >::const_iterator vit;
	for( vit = m_vecRobot.begin(); vit != m_vecRobot.end(); vit++ )
	{
		if( ( *vit )->GetStateID() >= KGSFSM::S_LOGINED && ( *vit )->GetStateID() < KGSFSM::STATE_SENTINEL )
		{
			iCount++;
		}
	}

	return iCount;
}

void KncRobotThread::SetNum(int iMin, int iMax )
{
	START_LOG( cout, L"로봇 생성 : " << m_iID << m_vecRobot.size() << " -> " << iMin << " <-> " << iMax );

	KLocker lock( m_csRobot );

	for( int i = iMin; i <= iMax; ++i )
	{
		KncRobotPtr spRobot( new KncRobot );

		spRobot->SetFSM( m_spFSM );
		spRobot->StateTransition( KGSFSM::I_TO_INIT );
		spRobot->Init( true );
		spRobot->SetIndex( i );
		m_vecRobot.push_back( spRobot );
	}

	//m_maxBotCnt = nMax;
}




