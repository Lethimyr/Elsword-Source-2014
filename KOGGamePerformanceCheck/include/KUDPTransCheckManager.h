#pragma once

#define _SECURE_SCL 0
#define _HAS_ITERATOR_DEBUGGING 0

#include <Windows.h>
#include <MMSystem.h>
#include <math.h>
#include <map>

class KUDPTransCheckManager
{
public:
	class KTrans
	{
	public:
		KTrans() { Init( -1 ); }

		void Init( int iGameSpace )
		{
			m_dwTransCount_Host = 0;
			m_dwTransAmount_Host = 0;
			m_dwTransTime_Host = 0;
			m_dwTransCount_NonHost = 0;
			m_dwTransAmount_NonHost = 0;
			m_dwTransTime_NonHost = 0;

			//
			m_iLastGameSpace = iGameSpace;
			//
			m_dwTransStartTime = timeGetTime();
		}

		int GetLastGameSpace() { return m_iLastGameSpace; }

		void Update( DWORD dwTransSize );

		void GetValuePerSec( float& fTransCountPerSec_Host, float& fTransAmountPerSec_Host, float& fTransCountPerSec_NonHost, float& fTransAmountPerSec_NonHost ) const;

	private:
		DWORD m_dwTransCount_Host;
		DWORD m_dwTransAmount_Host;
		DWORD m_dwTransTime_Host;
		DWORD m_dwTransCount_NonHost;
		DWORD m_dwTransAmount_NonHost;
		DWORD m_dwTransTime_NonHost;

		//
		int m_iLastGameSpace;
		//
		DWORD m_dwTransStartTime;
	};

	class KUnitSyncPacketLossRate
	{
	public:
		KUnitSyncPacketLossRate() { Init( -1 ); }

		void Init( int iGameSpace )
		{
            m_kSyncInfo.Init();

			//
			m_iLastGameSpace = iGameSpace;
		}

		int GetLastGameSpace() { return m_iLastGameSpace; }

		void Update( unsigned uSentCnt, unsigned uRecvCnt );

		float GetValue() const;

	private:
		struct KSyncInfo
		{
			KSyncInfo()
				: m_uRecvCount( 0 )
				, m_uSentCount( 0 )
			{}
            void Init()
            {
                m_uRecvCount = 0;
                m_uSentCount = 0;
            }

			unsigned    m_uRecvCount;
            unsigned    m_uSentCount;
		};

        KSyncInfo   m_kSyncInfo;
		//
		int m_iLastGameSpace;
	};

	//
	KUDPTransCheckManager();
	~KUDPTransCheckManager();

	//
	void Init();

	void Start();
	void End();
	
	void UpdateSend( DWORD dwSendSize );
	void UpdateRecv( DWORD dwRecvSize );

	void UpdateUserSyncPacketLossRate( unsigned uSentCnt, unsigned uRecvCnt );
	void UpdateNpcSyncPacketLossRate( unsigned uSentCnt, unsigned uRecvCnt );

	//
	const KTrans& GetSend() { return m_kSend; }
	const KTrans& GetRecv() { return m_kRecv; }

	const KUnitSyncPacketLossRate& GetUserSyncPacketLossRate() { return m_kUserSyncPacketLossRate; }
	const KUnitSyncPacketLossRate& GetNpcSyncPacketLossRate() { return m_kNpcSyncPacketLossRate; }

private:

	//checking ¿©ºÎ
	bool m_bChecking;

	//
	KTrans m_kSend;
	KTrans m_kRecv;

	//
	KUnitSyncPacketLossRate m_kUserSyncPacketLossRate;
	KUnitSyncPacketLossRate m_kNpcSyncPacketLossRate;
};
