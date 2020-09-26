#include ".\tutorialmanager.h"
#include "NetError.h"

#include <dbg/dbg.hpp>


ImplementSingleton( KTutorialManager );

KTutorialManager::KTutorialManager(void)
{
	//{{ 2011.09.16 조효진  넷 에러 스트링을 클라이언트쪽에서 가져와서 출력
#ifdef SERV_NETERROR_STR_GET_FROM_CLIENT
	m_iDelReason[TDR_DISCONNECT_DAY]	= NetError::ERR_TUTORIAL_07; // 제자가 7일동안 접속을 하지 않아 사제관계가 종료 되었습니다.
	m_iDelReason[TDR_LEVEL_UP_END]	= NetError::ERR_TUTORIAL_08; // 제자가 레벨업을 완료하여 사제관계가 종료 되었습니다.
	m_iDelReason[TDR_DELETE_UNIT]	= NetError::ERR_TUTORIAL_09; // 유닛이 삭제되어 사제관계가 종료 되었습니다.
	m_iDelReason[TDR_REFUSAL_USER]	= NetError::ERR_TUTORIAL_10; // 유저에 의해 사제관계가 종료되었습니다.
#else SERV_NETERROR_STR_GET_FROM_CLIENT
	m_wstrDelReason[TDR_DISCONNECT_DAY]	= NetError::GetErrStrF( NetError::ERR_TUTORIAL_07 ); // 제자가 7일동안 접속을 하지 않아 사제관계가 종료 되었습니다.
	m_wstrDelReason[TDR_LEVEL_UP_END]	= NetError::GetErrStrF( NetError::ERR_TUTORIAL_08 ); // 제자가 레벨업을 완료하여 사제관계가 종료 되었습니다.
	m_wstrDelReason[TDR_DELETE_UNIT]	= NetError::GetErrStrF( NetError::ERR_TUTORIAL_09 ); // 유닛이 삭제되어 사제관계가 종료 되었습니다.
	m_wstrDelReason[TDR_REFUSAL_USER]	= NetError::GetErrStrF( NetError::ERR_TUTORIAL_10 ); // 유저에 의해 사제관계가 종료되었습니다.
#endif SERV_NETERROR_STR_GET_FROM_CLIENT
	//}}
}

KTutorialManager::~KTutorialManager(void)
{
}

ImplToStringW( KTutorialManager )
{
	return START_TOSTRING2W( KTutorialManager );
}

bool KTutorialManager::IsExistbylist( UidType iUnitUID )
{
	std::vector<KTutorialUnitInfo>::iterator vit;

	for( vit = m_vecUnitInfo.begin(); vit != m_vecUnitInfo.end(); ++vit )
	{
		if( vit->m_iUnitUID == iUnitUID )
			return true;
	}

	return false;
}

bool KTutorialManager::AddUnit( KTutorialUnitInfo& kInfo )
{
	std::vector<KTutorialUnitInfo>::iterator vit;
	bool bIsAdd = false;

	for( vit = m_vecUnitInfo.begin(); vit != m_vecUnitInfo.end(); ++vit )
	{
		if( vit->m_iUnitUID == kInfo.m_iUnitUID )
			bIsAdd = true;
	}

	//미등록 상태이면..
	if( bIsAdd == false )
	{
		//제자 레벨조건이 맞을경우.
		if( kInfo.m_ucLevel <= UNIT_LEVEL_STUDENT_MAX )
		{
			//m_mapUnitInfo.insert( std::make_pair( kInfo.m_iUnitUID, kInfo ) );
			m_vecUnitInfo.push_back( kInfo );

			START_LOG( clog, L"제자 유저 등록.." )
				<< BUILD_LOG( kInfo.m_iUnitUID )
				<< BUILD_LOGc( kInfo.m_ucLevel )
				<< BUILD_LOG( kInfo.m_wstrNickName )
				;

			return true;
		}
		//레벨조건이 틀릴경우.
		else
		{
			START_LOG( cerr, L"제자등록중 레벨조건이 맞지 않음." )
				<< BUILD_LOG( kInfo.m_iUnitUID )
				<< BUILD_LOGc( kInfo.m_ucLevel )
				<< BUILD_LOG( kInfo.m_wstrNickName )
				<< END_LOG
				;
		}
	}
	else
	{
		START_LOG( cerr, L"이미 등록된 유저임." )
			<< BUILD_LOG( kInfo.m_iUnitUID )
			<< BUILD_LOGc( kInfo.m_ucLevel )
			<< BUILD_LOG( kInfo.m_wstrNickName )
			<< END_LOG
			;
	}

	return false;
}

bool KTutorialManager::DeleteUnit( UidType iUnitUID )
{
	std::vector<KTutorialUnitInfo>::iterator vit;

	for( vit = m_vecUnitInfo.begin(); vit != m_vecUnitInfo.end(); ++vit )
	{
		if( vit->m_iUnitUID == iUnitUID )
		{
			START_LOG( clog, L"제자유닉 삭제.!" )
				<< BUILD_LOG( vit->m_iUnitUID )
				<< BUILD_LOGc( vit->m_ucLevel )
				<< BUILD_LOG( vit->m_wstrNickName )
				;

			m_vecUnitInfo.erase( vit );

			return true;
		}
	}

	START_LOG( cerr, L"삭제하려는 제자유닛이 없음.!" )
		<< BUILD_LOG( iUnitUID )
		<< END_LOG;

	return false;
}

bool KTutorialManager::GetTutorPageInfo( IN KEGS_TUTORIAL_STUDENT_LIST_REQ& kReq, OUT KEGS_TUTORIAL_STUDENT_LIST_ACK& kAck )
{
	if( m_vecUnitInfo.empty() == true )
	{
		kAck.m_iTotalPage = 1;
		kAck.m_iViewPage = 1;

		return true;
	}

	std::map<UidType,KTutorialUnitInfo>::iterator mit;

	int iTotalPage = m_vecUnitInfo.size() / STUDENT_PAGE_NUM;

	if( iTotalPage <= 0 )
		iTotalPage = 1;

	//마지막 페이지의 유저정보가 한페이지의 수보다 적을 경우 페이지 추가.
	if( ((m_vecUnitInfo.size() % STUDENT_PAGE_NUM) > 0) &&
		m_vecUnitInfo.size() > STUDENT_PAGE_NUM )
	{
		++iTotalPage;
	}

	//시간차로 인해 요청한 페이지가 마지막페이지보다 높을경우.
	if( kReq.m_iViewPage > iTotalPage )
		kReq.m_iViewPage = iTotalPage;
	else if( kReq.m_iViewPage < 1 )
		kReq.m_iViewPage = 1;

	int iS = STUDENT_PAGE_NUM * ( kReq.m_iViewPage - 1 );
	int iE = iS + STUDENT_PAGE_NUM;

	kAck.m_iTotalPage = iTotalPage;
	kAck.m_iViewPage = kReq.m_iViewPage;
	for( ; iS < iE; ++iS )
	{
		if( iS < (int)m_vecUnitInfo.size() && iS >= 0 )
		{
			kAck.m_vecTutorialStudentList.push_back( m_vecUnitInfo[iS] );
		}
		else
		{
			if( iS != (int)m_vecUnitInfo.size() )
			{
				START_LOG( cerr, L"제자 리스트 페이지 요청실패.!" )
					<< BUILD_LOG( (int)STUDENT_PAGE_NUM )
					<< BUILD_LOG( iS )
					<< BUILD_LOG( iE )
					<< BUILD_LOG( m_vecUnitInfo.size() )
					<< END_LOG;
			}

			break;
		}
	}

	if( kAck.m_vecTutorialStudentList.size() > STUDENT_PAGE_NUM )
	{
		START_LOG( cerr, L"사제 페이시 개수이상.!" )
			<< BUILD_LOG( kAck.m_vecTutorialStudentList.size() )
			<< BUILD_LOG( (int)STUDENT_PAGE_NUM )
			<< END_LOG;
	}

	return true;
}

void KTutorialManager::SetLevelData( UidType iUnitUID, u_char ucLevel )
{
	std::vector<KTutorialUnitInfo>::iterator vit;

	for( vit = m_vecUnitInfo.begin(); vit != m_vecUnitInfo.end(); ++vit )
	{
		if( vit->m_iUnitUID == iUnitUID )
		{
			vit->m_ucLevel = ucLevel;

			if( ucLevel == UNIT_LEVEL_TEACHER_MIN )
			{
				m_vecUnitInfo.erase( vit );
			}

			break;
		}
	}
}

//{{ 2011.09.16 조효진  넷 에러 스트링을 클라이언트쪽에서 가져와서 출력
#ifdef SERV_NETERROR_STR_GET_FROM_CLIENT
int KTutorialManager::GetDelReason( TUTORIAL_DEL_REASON eReason )
{
	if( eReason < TDR_DISCONNECT_DAY || eReason >= TDR_TOTAL_NUM )
	{
		START_LOG( cerr, L"사제관계 삭제 타입이 이상함.!" )
			<< BUILD_LOG( eReason )
			<< END_LOG;

		int iTemp = NetError::ERR_UNKNOWN;
		return iTemp;
	}

	return m_iDelReason[eReason];
}
#else SERV_NETERROR_STR_GET_FROM_CLIENT
std::wstring KTutorialManager::GetDelReason( TUTORIAL_DEL_REASON eReason )
{
	if( eReason < TDR_DISCONNECT_DAY || eReason >= TDR_TOTAL_NUM )
	{
		START_LOG( cerr, L"사제관계 삭제 타입이 이상함.!" )
			<< BUILD_LOG( eReason )
			<< END_LOG;

		std::wstring wstrTemp = L"";
		return wstrTemp;
	}

	return m_wstrDelReason[eReason];
}
#endif SERV_NETERROR_STR_GET_FROM_CLIENT
//}}

