#pragma once

#define _SECURE_SCL 0
#define _HAS_ITERATOR_DEBUGGING 0

#include <Windows.h>
#include <MMSystem.h>

class KAvgFpsCheckManager
{
public:
	class KAvgFps
	{
	public:
		KAvgFps() { Init( -1, INT_MAX ); }

		void Init( int iGameSpace, int iMinSampleNum )
		{
			m_fValue = -1.0f;
			m_fValue_StdDev = -1.0f;

			m_fLastCheckTime = -1.0f;
			m_dSumOfValue = 0.0f;
			m_uiNumOfFps = 0;

			//
			m_iLastGameSpace = iGameSpace;
			m_iMinSampleNum = iMinSampleNum;
		}

		int GetLastGameSpace() { return m_iLastGameSpace; }

		void SetValue( float fValue ) { m_fValue = fValue; }
		float GetValue() const { return ( m_uiNumOfFps < m_iMinSampleNum ) ? -1.0f : m_fValue; }

		void SetValue_StdDev( float fValue_StdDev ) { m_fValue_StdDev = fValue_StdDev; }
		float GetValue_StdDev() const { return ( m_uiNumOfFps < m_iMinSampleNum ) ? -1.0f : m_fValue_StdDev; }

		void Update();
		
	private:
		float m_fValue;		//평균 프레임률
		float m_fValue_StdDev;		//표준 편차

		float m_fLastCheckTime;//마지막 체크 시간
		double m_dSumOfValue;
		unsigned int m_uiNumOfFps;//

		//
		int m_iLastGameSpace;
		int m_iMinSampleNum;
	};

	//
	KAvgFpsCheckManager();
	~KAvgFpsCheckManager();

	//
	void Init( float fUpdateTime, int iMinSampleNum );

	//
	void UpdateAvgFps();

	void Start();
	void Pause() { m_bPause = true; m_iSkipFrame = 2; }//현재 FPS가 렌더링 후에 갱신되는 구조임으로 2프레임은 skip 해줘야 된다.
	void Resume() { m_bPause = false; }
	void End();

	const KAvgFps& GetAvgFps() { return m_kAvgFps; }


private:
	//
	float m_fUpdateTime;
	int m_iMinSampleNum;

	KAvgFps m_kAvgFps;//평균 프레임률

	//checking 여부
	bool m_bChecking;
	//pause 여부
	bool m_bPause;
	//skip할 프레임 수
	int m_iSkipFrame;
};
