#pragma once

#include "ServerDefine.h"

// 2013.07.09 lygan_조성욱 // 해외팀 서버 관리툴(KogServerManagementStudio)과 각 서버간에 통신을 위한 작업
#ifdef SERV_PROCESS_COMMUNICATION_KSMS
#include "BaseServer.h"
#include "Enum/Enum.h"

class KProcessCommunicationManager : public KThread
{
	DeclareSingleton( KProcessCommunicationManager );


public:
	KProcessCommunicationManager();
	~KProcessCommunicationManager(){}

	virtual	bool	Begin( void );

	void	InitRead( LPTSTR lpszPipeName );
	void	InitWrite( LPTSTR lpszPipeName );
	void	ShutDown( void );
	

	void	SetPrecessCommunicationWriteData( std::string _strWriteData ) { m_strWriteData = _strWriteData; }
	

	void	QueueingProcessWrite(std::wstring _strQueueing);

protected:
	
	virtual	void	Tick( void );
	// derived from KThread
	virtual	void	Run( void );
	void			ReadTickData(void);
	void			WriteData(void);

	bool			GetQueuingWriteData(WCHAR *wchReply, DWORD  &cbReplyBytes);
	


	mutable KncCriticalSection			m_csPipeNameRead;
	mutable KncCriticalSection			m_csPipeNameWrite;

	std::queue<std::wstring>            m_queWriteMessage;

private:

	LPTSTR				m_lpszPipeNameRead;
	LPTSTR				m_lpszPipeNameWrite;

	HANDLE				m_hPipeRead;
	HANDLE				m_hPipeWrite;

	boost::shared_ptr< KTThread< KProcessCommunicationManager > >	m_spThreadRead;
	boost::shared_ptr< KTThread< KProcessCommunicationManager > >	m_spThreadWrite;
	boost::shared_ptr< KTThread< KProcessCommunicationManager > >	m_spThreadWritePipe;

	std::string m_strWriteData;
	
};

DefSingletonInline( KProcessCommunicationManager );


#endif //SERV_PROCESS_COMMUNICATION_KSMS