#pragma once

#include <KNCSingleton.h>
#include <ToString.h>
#include <map>

//{{ 버프 관리 시스템 - 김민성
#ifdef SERV_SERVER_BUFF_SYSTEM


class KBuffManager
{
	DeclareSingleton( KBuffManager );
	DeclDump;
	DeclToStringW;

public:
	KBuffManager(void);
	~KBuffManager(void);

	// 버프 정보 갱신
	bool AddUnitBuffInfo( IN const UidType& iUnitUID, IN const std::vector<KBuffInfo>& vecBuffInfo );
	bool DeleteUnitBuffInfo( IN UidType& iUnitUID, IN std::vector<int>& vecBuffID );
#ifdef _CONVERT_VS_2010
	bool DeleteUnitAllBuffInfo( IN UidType iUnitUID );
#else
	bool DeleteUnitAllBuffInfo( IN UidType& iUnitUID );
#endif _CONVERT_VS_2010
	
	// 갱신된 버프 시간 추가
	bool AddRenewBuffTime( IN const UidType& iUnitUID, IN const __int64& iNewTime, IN const __int64 iOleTime = 0 );

	// 버프 시간 삭제
	void DeleteBuffTime( IN __int64& iTime, IN UidType iUnitUID );

	// 유닛의 버프 정보 얻기
	bool GetUnitAllBuffInfo( IN UidType& iUnitUID, OUT KUnitBuffInfo& kUnitBuffInfo );

	// 일정 시간 마다 버프 지워주기
	void CheckEndBuff();

private:

	std::map< UidType, KUnitBuffInfo >					m_mapUnitBuffList;			// 유니별 버프 정보					<유닛, 유닛버프정보>
	std::map< __int64, std::vector<UidType> >			m_mapUnitTime;				// 최초 셋팅된 시간별 유닛의 정보	<최초 등록 시간, 유닛들>

};

DefSingletonInline( KBuffManager );


#endif SERV_SERVER_BUFF_SYSTEM
//}