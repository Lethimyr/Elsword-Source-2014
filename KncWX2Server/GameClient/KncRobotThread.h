#pragma once

#include <vector>
#include <ToString.h>

#include "KncRobot.h"
#include "SubThread.h"

SmartPointer( KGSFSM );

class KTickTimer
{
	enum TIMER_ID
	{
		TI_PRINT_TIME	= 0,
		TI_ELAPSED_TIME,
		TI_TOTAL_NUM,
	};

public:
	KTickTimer(){ m_bIsStart = false; m_fTickSec = 10.0f; }
	~KTickTimer(){}

	void	Restart()
	{
		m_Timer[TI_PRINT_TIME].restart();
	}
	void	SetTime( float fSec ){ m_fTickSec = fSec; }

	void	Begin()
	{
		if( m_Timer[TI_PRINT_TIME].elapsed() > 10.0f )
		{
			m_Timer[TI_PRINT_TIME].restart();

			m_Timer[TI_ELAPSED_TIME].restart();
			m_bIsStart = true;
		}
	}

	void	End()
	{
		if( m_bIsStart == true )
		{
			m_bIsStart = false;

			START_LOG( cout2, L"Tick time(sec) : " )
				<< BUILD_LOG( static_cast<float>(m_Timer[TI_ELAPSED_TIME].elapsed()) )
				;
		}
	}

protected:
	boost::timer	m_Timer[TI_TOTAL_NUM];

	bool			m_bIsStart;
	float			m_fTickSec;
};

class KncRobotThread : public KThread
{
	DeclDump;
	DeclToStringW;

public:
	KncRobotThread( int iID, FSMclassPtr spKFSM );
	virtual ~ KncRobotThread();

	bool	Init();
	bool	ShutDown();

	virtual	void	Run();
			void	Tick();

	int		GetNum() const;
	int		GetLiveNum() const;
	void	SetNum( int iMin, int iMax );

protected:
	int								m_iID;
	FSMclassPtr                     m_spFSM;

	KTickTimer						m_kTimer;
	mutable CRITICAL_SECTION        m_csRobot;
	std::vector< KncRobotPtr >      m_vecRobot;
};