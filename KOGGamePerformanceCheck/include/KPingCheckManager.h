#pragma once

#define _SECURE_SCL 0
#define _HAS_ITERATOR_DEBUGGING 0

#include <Windows.h>
#include <MMSystem.h>
#include <math.h>

class KPingCheckManager
{
public:
	class KPing
	{
	public:
		KPing()
		{
			m_fMinPingTime = -1.0f;
			m_fMaxPingTime = -1.0f;
			m_fAvgPingTime = -1.0f;
			m_fAvgPingTime_StdDev = -1.0f;

			m_dSumOfAvgPingTime = 0.0f;
			m_uiNumOfFps = 0;

			m_iPingLossWindowSize = 0;
			m_iPingTimeOut = 0;
			m_iMinSampleNum = 0;

			m_akPingWindow = NULL;
			m_uiSendCounter = 0;
			m_uiNumSent = 0;
			m_uiNumLost = 0;
			m_uiNumReceived = 0;
		}

		~KPing()
		{
			if( m_akPingWindow )
				delete[] m_akPingWindow;
		}
		void Init( int iGameSpace, int iPingLossWindowSize, int iPingTimeOut, int iMinSampleNum )
		{
			//
			m_fMinPingTime = -1.0f;
			m_fMaxPingTime = -1.0f;
			m_fAvgPingTime = -1.0f;
			m_fAvgPingTime_StdDev = -1.0f;

			m_dSumOfAvgPingTime = 0.0f;
			m_uiNumOfFps = 0;


			m_iPingLossWindowSize = iPingLossWindowSize;
			m_iPingTimeOut = iPingTimeOut;
			m_iMinSampleNum = iMinSampleNum;

			//기존것 제거
			if( m_akPingWindow )
			{
				delete[] m_akPingWindow;
				m_akPingWindow = NULL;
			}

			if( m_iPingLossWindowSize > 0 )
			{
				m_akPingWindow = new KPingWindow[m_iPingLossWindowSize];

				ZeroMemory( m_akPingWindow, sizeof( KPingWindow ) * m_iPingLossWindowSize );
			}

			m_uiSendCounter = 0;
			m_uiNumSent = 0;
			m_uiNumLost = 0;
			m_uiNumReceived = 0;

			//
			m_iLastGameSpace = iGameSpace;
		}

		int GetLastGameSpace() { return m_iLastGameSpace; }

		UINT SendPingCheckPacket();
		void SendPingCheckPacket( UINT uiSendCounter );
		void UpdatePingTime( float fPingTime );
		void UpdatePingLossRate( UINT uiSendCounter );

		float GetMinPingTime() const { return m_fMinPingTime; }
		float GetMaxPingTime() const { return m_fMaxPingTime; }
		float GetAvgPingTime() const { return ( m_uiNumOfFps < m_iMinSampleNum ) ? -1.0f : m_fAvgPingTime; }
		float GetAvgPingTime_StdDev() const { return ( m_uiNumOfFps < m_iMinSampleNum ) ? -1.0f : m_fAvgPingTime_StdDev; }
		float GetAvgPingLossRate() const 
		{ 
			//추가로 lost 개수를 계산( m_akPingWindow내에 timeout된 것들이 있는지 파악 )
			UINT uiAdditionalLostNum = 0;
			DWORD dwCurTime = timeGetTime();
			for( int i = 0; i < m_iPingLossWindowSize; i++ )
			{
				//timeout인지 확인
				if( m_akPingWindow[i].m_uiSendCounter != 0 &&
					dwCurTime - m_akPingWindow[i].m_dwSendTime > m_iPingTimeOut )
				{
					uiAdditionalLostNum++;
				}
			}

			//최소 샘플 개수를 만족하는지 확인
			if( uiAdditionalLostNum + m_uiNumLost + m_uiNumReceived >= m_iMinSampleNum )
			{
				//보내고 받기(왕복)에 대한 로스율
				float p = 1.f - ( (float)m_uiNumReceived / (float)( uiAdditionalLostNum + m_uiNumLost + m_uiNumReceived ) );
				//편도에 대한 로스율로 재계산
				return 1.0f - sqrt( 1.0f - p );
			}

			return -1.0f;
		}

	private:
		struct KPingWindow
		{
			UINT m_uiSendCounter;
			DWORD m_dwSendTime;
		};

		//ping time 관련
		float m_fMinPingTime;
		float m_fMaxPingTime;
		float m_fAvgPingTime;
		float m_fAvgPingTime_StdDev;

		double m_dSumOfAvgPingTime;
		unsigned int m_uiNumOfFps;

		//ping loss 관련
		int m_iPingLossWindowSize;
		int m_iPingTimeOut;
		int m_iMinSampleNum;

		KPingWindow* m_akPingWindow;
		UINT m_uiSendCounter;
		UINT m_uiNumSent;
		UINT m_uiNumLost;
		UINT m_uiNumReceived;

		//
		int m_iLastGameSpace;
	};

	//
	KPingCheckManager();
	~KPingCheckManager();

	//
	void Init( int iPingLossWindowSize, int iPingTimeOut, int iMinSampleNum );

	UINT SendPingCheckPacket();
	void SendPingCheckPacket( UINT uiSendCounter );
	void ReceivePingCheckPacket( DWORD dwPingTime, UINT uiSendCounter );

	void InValidateCheck() 
	{ 
		if( m_bChecking ) 
			m_bInValidateCheck = true; 
	}

	void Start( bool bMainThread );
	void Pause() { m_bPause = true; m_bSkipOneFrame = true; }
	void Resume() 	{ m_bPause = false; }
	void End();
	
	//
	const KPing& GetPing() { return m_kPing; }

private:
	//
	KPing m_kPing;

	//checking 여부
	bool m_bChecking;
	//pause 여부
	bool m_bPause;
	//1프레임 skip여부
	bool m_bSkipOneFrame;

	//main thread에서 수행하는 ping check인지 여부
	bool m_bMainThread;
	//
	bool m_bInValidateCheck;

	//
	int m_iPingLossWindowSize;
	int m_iPingTimeOut;
	int m_iMinSampleNum;
};
