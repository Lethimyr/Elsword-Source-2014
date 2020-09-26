#pragma once
#include <boost/timer.hpp>

#pragma pack( push, 1 )

//{{ 2009. 3. 6  최육사		타이머매니저
class KTimerManager
{
public:
	KTimerManager()	: m_fAddTime( 0.0f ) {} 
	void restart()						{ m_kTimer.restart(); m_fAddTime = 0.0f; }
	double elapsed() const				{ return ( m_kTimer.elapsed() + m_fAddTime ); }

	void SetAddTime( double fTime )		{ m_fAddTime = fTime; }
	double GetAddTime()					{ return m_fAddTime; }

private:
	double		 m_fAddTime;
	boost::timer m_kTimer;
};
//}}

//{{ 2009. 7. 21  최육사	동적 타이머
class KDynamicTimer
{
public:
	KDynamicTimer() { m_vecTimer.clear(); }
	~KDynamicTimer() {}

	bool CreateTimer()
	{
		// 이미 타이머가 있다면 타이머 생성 실패!
		if( !m_vecTimer.empty() ) return false;

		m_vecTimer.push_back( boost::timer() );
		return true;
	}

	bool KillTimer()
	{
		// 삭제할 타이머가 없다면 삭제 실패
		if( m_vecTimer.empty() ) return false;

		m_vecTimer.clear();
		return true;
	}

	bool IsExist() { return !m_vecTimer.empty(); }

	double elapsed() const
	{
		// 타이머가 없다면 0.0f를 반환
		std::vector< boost::timer >::const_iterator vit = m_vecTimer.begin();
		if( vit == m_vecTimer.end() )
			return 0.0f;
		
		return vit->elapsed();
	}

private:
	std::vector< boost::timer > m_vecTimer;
};
//}}

#pragma pack( pop )

