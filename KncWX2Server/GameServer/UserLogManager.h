#pragma once
#include <queue>


SmartPointer(KGSUser);

class KUserLogManager
{
public:
	enum USER_LOG_INFO
	{
		QUEUE_SIZE = 5,
	};

	enum USER_LOG_TYPE
	{
		ULT_HEART_BEAT = 0,
		ULT_ABNORMAL_DICONNECT,

		ULT_MAX,
	};

	struct KEventLog
	{
        u_short		m_usEventID;
		__time64_t	m_tTime;

		KEventLog( u_short usEventID, __time64_t tTime )
		{
            m_usEventID = usEventID;
			m_tTime = tTime;
		}
	};

public:
	KUserLogManager(void);
	~KUserLogManager(void);

	void SaveSendPacket( u_short usEventID );
	void SaveProcessEvent( u_short usEventID );

	void DumpLogFile( IN KGSUserPtr spUser, IN USER_LOG_TYPE eUserLogType );

private:
	std::queue< KEventLog >	m_SendPacketQueue;
	std::queue< KEventLog >	m_ProcessPacketQueue;

	static std::wfstream		ms_wfsUserLog[ULT_MAX];
};


