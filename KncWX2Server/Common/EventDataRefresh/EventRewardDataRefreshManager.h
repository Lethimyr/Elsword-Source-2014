#pragma once
#include "ServerDefine.h"
#include "KncUtil.h"

//{{ 2012. 02. 03	박세훈	이벤트 관련정보 처리방법 변경 ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
#include <KNCSingleton.h>
#include <ToString.h>
#include <map>
#include <vector>

#include "ClientPacket.h"

struct KPostItemInfo;
class KEventRewardDataRefreshManager
{
	DeclareSingleton( KEventRewardDataRefreshManager );
	DeclToStringW;
	DeclDump;

public:
	KEventRewardDataRefreshManager(void);
	~KEventRewardDataRefreshManager(void);

	void GetRewardItem( IN const KPostItemInfo& kPostItemInfo, OUT std::map< int, KItemInfo >& mapRewardItem ) const;
	void GetRewardItem( IN const std::vector< KPostItemInfo >& vecPostItemInfo, OUT std::map< int, KItemInfo >& mapRewardItem ) const;
	bool GetRewardInfo( IN int iRewardID, OUT std::map< int, int >& mapRewardInfo ) const;
	void GetRewardInfo( IN int iRewardType, IN const std::vector< KPostItemInfo >& vecPostItemInfo, OUT std::map< int, int >& mapRewardInfo ) const;
	void GetRewardInfo( IN int iRewardType, IN const std::vector< KPostItemInfo >& vecPostItemInfo, OUT std::vector< std::wstring >& vecMessage ) const;

	void UpdateRewardData( void );
	void SetRewardData( IN const std::map< int, std::vector< KRewardData > >& mapRewardData );

private:
	std::map< int, std::vector< KRewardData > > m_mapRewardData;
};

DefSingletonInline( KEventRewardDataRefreshManager );
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
//}}