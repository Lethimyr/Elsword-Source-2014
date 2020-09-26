#pragma once

namespace std { using ::clock_t; using ::clock; }

class CKTDXTimer
{
public:
	CKTDXTimer() { _start_time = std::clock(); } // postcondition: elapsed()==0
	//         timer( const timer& src );      // post: elapsed()==src.elapsed()
	//        ~timer(){}
	//  timer& operator=( const timer& src );  // post: elapsed()==src.elapsed()
	void   restart() { _start_time = std::clock(); } // post: elapsed()==0
	double elapsed() const                  // return elapsed time in seconds
	{ return  double(std::clock() - _start_time) / CLOCKS_PER_SEC; }

	//{{ kimhc // 진행시간을 현재 시간으로 셋팅
	void	ResetStartTime() { _start_time = 0; }	// 진행시간을 현재 시간으로 셋팅
	//}} kimhc // 진행시간을 현재 시간으로 셋팅

	void    SetTime(long lTime) { _start_time = std::clock() - lTime; }


private:
	std::clock_t _start_time;
}; // timer

class CKTDXCheckElapsedTime {

public:

	CKTDXCheckElapsedTime() : m_fTargetTime( 0.0f ), m_fSumOfElapsedTime( 0.0f )
	{}
	
	CKTDXCheckElapsedTime( const float fTargetTime_ ) : m_fTargetTime( fTargetTime_ ), m_fSumOfElapsedTime( fTargetTime_ )
	{}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    void    OnFrameMove( float fElapsedTime );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	void    OnFrameMove();// { m_fSumOfElapsedTime += g_pKTDXApp->GetElapsedTime(); }
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE


	
	void ResetSumOfElapsedTime( const float fSumOfElapsedTime = 0 ) { m_fSumOfElapsedTime = fSumOfElapsedTime; }

	bool CheckElapsedTime() { return ( m_fTargetTime > m_fSumOfElapsedTime ? false : true ); }
	
	bool CheckAndResetElapsedTime() 
	{
		// 목표로 한 시간 만큼 되었으면
		if ( CheckElapsedTime() )
		{
			ResetSumOfElapsedTime();
			return true;
		}
		else	// 아직 목표로 한 시간 만큼 지나지 않았으면
			return false;
	}

	float GetTargetTime() const { return m_fTargetTime; }
	void SetTargetTime( const float fTargetTime_ ) { m_fTargetTime = fTargetTime_; }
	
	float GetSumOfElapsedTime() const { return m_fSumOfElapsedTime; }

private:
	float m_fTargetTime;		/// 목표로 정한 시간
	float m_fSumOfElapsedTime;	/// 경과시간의 합
	
	

};