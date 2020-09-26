#pragma once

#define _SECURE_SCL 0
#define _HAS_ITERATOR_DEBUGGING 0

#include <Windows.h>
#include <MMSystem.h>
#include <math.h>

class KNonHostNpcReactionCheckManager
{
public:
	class KDelay
	{
	public:
		KDelay()
		{
			m_fHostUdpAvgPingTime = -1.0f;

			//SimulOff
			m_fMinDelayTime_SimulOff = -1.0f;
			m_fMaxDelayTime_SimulOff = -1.0f;
			m_fAvgDelayTime_SimulOff = -1.0f;
			m_fAvgDelayTime_StdDev_SimulOff = -1.0f;
			m_dSumOfAvgDelayTime_SimulOff = 0.0f;

			//SimulOn
			m_fMinDelayTime_SimulOn = -1.0f;
			m_fMaxDelayTime_SimulOn = -1.0f;
			m_fAvgDelayTime_SimulOn = -1.0f;
			m_fAvgDelayTime_StdDev_SimulOn = -1.0f;
			m_dSumOfAvgDelayTime_SimulOn = 0.0f;

			//
			m_uiNumOfFps = 0;
			m_iMinSampleNum = 0;
		}

		~KDelay()
		{
		}
		void Init( int iGameSpace, int iMinSampleNum )
		{
			//
			m_fHostUdpAvgPingTime = -1.0f;

			//SimulOff
			m_fMinDelayTime_SimulOff = -1.0f;
			m_fMaxDelayTime_SimulOff = -1.0f;
			m_fAvgDelayTime_SimulOff = -1.0f;
			m_fAvgDelayTime_StdDev_SimulOff = -1.0f;
			m_dSumOfAvgDelayTime_SimulOff = 0.0f;

			//SimulOn
			m_fMinDelayTime_SimulOn = -1.0f;
			m_fMaxDelayTime_SimulOn = -1.0f;
			m_fAvgDelayTime_SimulOn = -1.0f;
			m_fAvgDelayTime_StdDev_SimulOn = -1.0f;
			m_dSumOfAvgDelayTime_SimulOn = 0.0f;

			//
			m_uiNumOfFps = 0;
			m_iMinSampleNum = iMinSampleNum;

			//
			m_iLastGameSpace = iGameSpace;
		}

		int GetLastGameSpace() { return m_iLastGameSpace; }

		void UpdateDelayTime( float fHostUdpPingTime, float fDelayTime_SimulOff, float fDelayTime_SimulOn );

		float GetHostUdpAvgPingTime() const { return ( m_uiNumOfFps < m_iMinSampleNum ) ? -1.0f : m_fHostUdpAvgPingTime; }
		//SimulOff
		float GetMinDelayTime_SimulOff() const { return m_fMinDelayTime_SimulOff; }
		float GetMaxDelayTime_SimulOff() const { return m_fMaxDelayTime_SimulOff; }
		float GetAvgDelayTime_SimulOff() const { return ( m_uiNumOfFps < m_iMinSampleNum ) ? -1.0f : m_fAvgDelayTime_SimulOff; }
		float GetAvgDelayTime_StdDev_SimulOff() const { return ( m_uiNumOfFps < m_iMinSampleNum ) ? -1.0f : m_fAvgDelayTime_StdDev_SimulOff; }
		//SimulOn
		float GetMinDelayTime_SimulOn() const { return m_fMinDelayTime_SimulOn; }
		float GetMaxDelayTime_SimulOn() const { return m_fMaxDelayTime_SimulOn; }
		float GetAvgDelayTime_SimulOn() const { return ( m_uiNumOfFps < m_iMinSampleNum ) ? -1.0f : m_fAvgDelayTime_SimulOn; }
		float GetAvgDelayTime_StdDev_SimulOn() const { return ( m_uiNumOfFps < m_iMinSampleNum ) ? -1.0f : m_fAvgDelayTime_StdDev_SimulOn; }

	private:
		//Host UDP 평균 Ping time 관련
		float m_fHostUdpAvgPingTime;

		//SimulOff에 대한 Delay time 관련
		float m_fMinDelayTime_SimulOff;
		float m_fMaxDelayTime_SimulOff;
		float m_fAvgDelayTime_SimulOff;
		float m_fAvgDelayTime_StdDev_SimulOff;
		double m_dSumOfAvgDelayTime_SimulOff;

		//SimulOn에 대한 Delay time 관련
		float m_fMinDelayTime_SimulOn;
		float m_fMaxDelayTime_SimulOn;
		float m_fAvgDelayTime_SimulOn;
		float m_fAvgDelayTime_StdDev_SimulOn;
		double m_dSumOfAvgDelayTime_SimulOn;

		//
		unsigned int m_uiNumOfFps;
		int m_iMinSampleNum;

		//
		int m_iLastGameSpace;
	};

	//
	KNonHostNpcReactionCheckManager();
	~KNonHostNpcReactionCheckManager();

	//
	void Init( int iMinSampleNum );

	void UpdateHostUdpPingTime( float fHostUdpPingTime ) { m_fHostUdpPingTime = fHostUdpPingTime; }
	void UpdateDelayTime( float fDelayTime_SimulOff, float fDelayTime_SimulOn );

	void InValidateCheck() 
	{ 
		if( m_bChecking ) 
			m_bInValidateCheck = true; 
	}

	void Start();
	void Pause() { m_bPause = true; m_bSkipOneFrame = true; }
	void Resume() 	{ m_bPause = false; }
	void End();
	
	//
	const KDelay& GetDelay() { return m_kDelay; }

private:
	//
	float m_fHostUdpPingTime;

	//
	KDelay m_kDelay;

	//checking 여부
	bool m_bChecking;
	//pause 여부
	bool m_bPause;
	//1프레임 skip여부
	bool m_bSkipOneFrame;

	//
	bool m_bInValidateCheck;
	int m_iMinSampleNum;
};
