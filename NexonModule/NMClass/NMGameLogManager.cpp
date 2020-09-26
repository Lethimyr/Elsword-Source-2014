#include "StdAfx.h"
#include "NMManager.h"
#include "NMGameLogManager.h"

BOOL NMGameLogManager::Initialize
(
	LPCTSTR			szGameName
,	LPCTSTR			szBaseURL
,	LPCTSTR			szSessionID
)
{
	CNMGameLogInitializeFunc func( szGameName, szBaseURL, szSessionID );
	
	return CNMManager::GetInstance().CallNMFunc( &func );
}

BOOL NMGameLogManager::Finalize
(
	void
)
{
	CNMGameLogFinalizeFunc func;
	
	return CNMManager::GetInstance().CallNMFunc( &func );
}

BOOL NMGameLogManager::WriteStageLog
(
	INT32			nStage
,	LPCTSTR			szComment
)
{
	CNMGameLogWriteStageLogFunc func( nStage, szComment );

	return CNMManager::GetInstance().CallNMFunc( &func );
}

BOOL NMGameLogManager::WriteErrorLog
(
	INT32			nErrorType
,	LPCTSTR			szComment
)
{
	CNMGameLogWriteErrorLogFunc func( nErrorType, szComment );

	return CNMManager::GetInstance().CallNMFunc( &func );
}

LPCTSTR NMGameLogManager::GetSessionID( LPTSTR szSessionIDOut )
{
	CNMGameLogGetSessionIDFunc	func;

	if ( CNMManager::GetInstance().CallNMFunc( &func ) )
	{
		NMSAFE_STRCOPY( szSessionIDOut, func.szSessionID, 32 );

		return szSessionIDOut;
	}

	if ( szSessionIDOut )
	{
		szSessionIDOut[0] = 0;
	}

	return szSessionIDOut;
}