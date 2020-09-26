#pragma once


#include <KNCSingleton.h>
#include <ToString.h>
#include "DBLayer.h"
#include <KncSend.h>
#include "ServerPacket.h"

//{{ 2011. 12. 14	최육사	SimLayer Tick퍼포먼스 체크
#ifdef SERV_SIMLAYER_TICK_PERFORMANCE_CHECK
#include "TickDumper.h"
#endif SERV_SIMLAYER_TICK_PERFORMANCE_CHECK
//}}

#include "X2Data/XSLStringFilter.h"

#ifdef SERV_STRING_FILTER_USING_DB
class KStringFilterManager
{
	DeclareSingleton( KStringFilterManager );
	DeclDump;
	DeclToStringW;
	//{{ 2011. 12. 14	최육사	SimLayer Tick퍼포먼스 체크
#ifdef SERV_SIMLAYER_TICK_PERFORMANCE_CHECK
	DeclareTickDumper;
#endif SERV_SIMLAYER_TICK_PERFORMANCE_CHECK
	//}}

public:
	KStringFilterManager(void);
	~KStringFilterManager(void);

	static const int iReleaseTickType = 0;

	void SetStringFilter( IN const StringFilterVector& vecStringFilter );
	void GetStringFilter( OUT StringFilterVector& vecStringFilter );

	bool CheckIsValidPostWordString( CXSLStringFilter::FILTER_TYPE filterType, const WCHAR* pString, OUT std::wstring &pOutString );
	bool CheckIsValidString( CXSLStringFilter::FILTER_TYPE filterType, std::wstring& wstringForCheck );		//유효한 스트링인가?
	std::wstring FilteringChatString( const WCHAR* pString, WCHAR wcharForReplace );
	std::wstring FilteringNoteString( const WCHAR* pString, WCHAR wcharForReplace );

	void Tick();
	void Init();

	int GetReleaseTick() { return m_iReleaseTick; }
	void SetReleaseTick( int iReleaseTick ) { m_iReleaseTick = iReleaseTick; }

protected:
	inline void MakeUpperCase(std::wstring &str) 
	{
		for( std::wstring::iterator i = str.begin(); i != str.end(); i++ ) 
			*i = (WCHAR)towupper(*i);
	}

	template < class T > void SendToScriptDB( unsigned short usEventID, const T& data );
	void	SendToScriptDB( unsigned short usEventID ){ SendToScriptDB( usEventID, char() ); }

private:
	std::vector< std::wstring >		m_BanNickNameList;
	std::vector< std::wstring >		m_BanNickNameWordList;
	std::vector< std::wstring >		m_BanWordList;
	std::vector< std::wstring >		m_BanPostWordList;

	boost::timer					m_tReleaseTickTimer;
	int								m_iReleaseTick;
};

template < class T >
void KStringFilterManager::SendToScriptDB( unsigned short usEventID, const T& data )
{
	UidType anTrace[2] = { 0, -1 };

	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PC_SCRIPT_DB, anTrace, usEventID, data );
	LIF( spEvent->m_kDestPerformer.AddUID( 0 ) );

	SiKDBLayer()->QueueingEvent( spEvent );
}

DefSingletonInline( KStringFilterManager );

#endif //SERV_STRING_FILTER_USING_DB