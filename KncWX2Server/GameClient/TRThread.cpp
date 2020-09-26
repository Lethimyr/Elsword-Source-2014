#include "TRThread.h"
#include "dbg/dbg.hpp"
#include "Event.h"

_ImplementException( KTRThread )

KTRThread::KTRThread(int iMin, int iMax, const char* strIP)
{
	Init(iMin, iMax, strIP);
}

KTRThread::~KTRThread(void)
{
	End();
}

void KTRThread::Init(const int iCreateMin_, const int iCreateMax_, const char* strIP)
{
	for( int i = iCreateMin_; i <= iCreateMax_; ++i )
	{
		KTRUserPtr spTRUser = KTRUserPtr( new KTRUser() );

		spTRUser->Init();

		if( !spTRUser->Connect( strIP, 9301, static_cast<UidType>(i) ) )
		{
			START_LOG( cerr, L"立加 角菩.!" )
				<< BUILD_LOG( i )
				<< END_LOG;

			continue;
		}
		else
		{
			START_LOG( clog, L"立加 己傍.!" )
				<< BUILD_LOG( i )
				;

			UidType iUID = spTRUser->GetUID();
			if( spTRUser->SendPacket( ETR_REG_UID, iUID ) == false )
			{
				START_LOG( cerr, L"REG_UID send failed.!" )
					<< END_LOG;
			}
		}

		m_vecRobot.push_back( spTRUser );
	}
}

void KTRThread::Run()
{
	START_LOG( clog, L"Enter Run()" );

	DWORD ret;

	while( true )
	{
		ret = ::WaitForSingleObject( m_hKillEvent, 1 );     // 0.00s 埃拜

		if( ret == WAIT_OBJECT_0 ) break;

		if( ret == WAIT_TIMEOUT ) Loop();

		else std::cout <<  "*** WaitForSingleObject() - return : "<< ret << std::endl;

	}// while
}

void KTRThread::Loop() 
{
	_BEGIN_TRY_BLOCK;

	std::vector<KTRUserPtr>::iterator vit;

    for( vit = m_vecRobot.begin(); vit != m_vecRobot.end(); )
	{
		if( (*vit)->GetUID() != -1 )
		{
			(*vit)->Tick();
			 ++vit;
		}
		else
		{
			START_LOG( cout, L"蜡历 昏力.!" );
			m_vecRobot.erase(vit);
		}
	}

	_END_TRY_BLOCK( L"" );
}