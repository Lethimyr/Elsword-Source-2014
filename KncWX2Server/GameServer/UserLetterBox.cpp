#include "GSUser.h"
#include ".\userletterbox.h"
#include "X2Data/XSLItemManager.h"
#include "RewardTable.h"
#include "NetError.h"


KUserLetterBox::KUserLetterBox(void)
{
}

KUserLetterBox::~KUserLetterBox(void)
{
}

void KUserLetterBox::Clear()
{
	m_mapLetters.clear();
	m_vecUserLetterList.clear();
	m_vecSystemLetterList.clear();
	//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
	m_vecRelationshipLetterList.clear();
#endif SERV_RELATIONSHIP_SYSTEM
	//}
	m_setBlackList.clear();
	m_wstrNickName.clear();
}

void KUserLetterBox::Init( IN KGSUserPtr spUser,
						   IN const std::vector< KPostItemInfo >& vecPostItems, 
						   OUT int& iNewUserLetterCount, 
						   OUT int& iNewSystemLetterCount,
						   //{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
						   OUT int& iNewRelationshipLetterCount,
#endif SERV_RELATIONSHIP_SYSTEM
						   //}
						   OUT std::vector< KPostSmallTitleInfo >& vecNewLetterTitle )
{
	if( !spUser )
	{
		START_LOG( cerr, L"GSUser포인터값이 이상합니다." )
			<< END_LOG;
		return;
	}

	m_wstrNickName = spUser->GetCharName();

	// [참고] vecPostItems에는 postNo가 작은 순으로 담겨져 옵니다. ( PostNo가 작을수록 옛날 편지임 )

	std::vector< KPostItemInfo >::const_iterator vit;
	for( vit = vecPostItems.begin(); vit != vecPostItems.end(); ++vit )
	{
		AddLetter( *vit );
	}

	//{{ 2009. 5. 18  최육사	읽지 않은 편지 제목
	// 1th..rivision.hoons.09.05.20.시스템 메일은 AddLetter에서 문장을 완성하기 때문에.
	//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
	GetRenewalLetter( spUser, iNewUserLetterCount, iNewSystemLetterCount, iNewRelationshipLetterCount, vecNewLetterTitle );
#else
	GetRenewalLetter( spUser, iNewUserLetterCount, iNewSystemLetterCount, vecNewLetterTitle );
#endif SERV_RELATIONSHIP_SYSTEM
	//}
	//}}
}

//{{ 09.05.18.hoons.
//des : GetRenewalLetter() 안에서 날자 비교를 위해 사용되어짐.
bool ComRenewalLetter( const KPostItemTitleInfo& _First, const KPostItemTitleInfo& _Last )
{
	CTime ctFirst, ctLast;
	std::wstring wstrTemp;

	// 날짜 컨버팅 처리가 실패해도 계산은 계속 진행 시킴
	wstrTemp = _First.m_wstrRegDate;
	if( KncUtil::ConvertStringToCTime(wstrTemp, ctFirst) == false )
	{
		START_LOG(cwarn, L"편지 등록날짜가 이상함.!")
			<< BUILD_LOG( _First.m_wstrRegDate )
			<< END_LOG;
	}
	wstrTemp = _Last.m_wstrRegDate;
	if( KncUtil::ConvertStringToCTime(wstrTemp, ctLast) == false )
	{
		START_LOG( cwarn, L"편지 등록날짜가 이상함.!" )
			<< BUILD_LOG( _Last.m_wstrRegDate )
			<< END_LOG;
	}
	return (ctFirst > ctLast) ? true : false;
}
//}} hoons.

//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
void KUserLetterBox::GetRenewalLetter( IN KGSUserPtr spUser, OUT int& iNewUserLetterCount, OUT int& iNewSystemLetterCount, OUT int& iNewRelationshipLetterCount, OUT std::vector< KPostSmallTitleInfo >& vecNewLetterTitle )
{
	iNewUserLetterCount = 0;
	iNewSystemLetterCount = 0;
	iNewRelationshipLetterCount = 0;
	vecNewLetterTitle.clear();

	std::vector< KPostItemTitleInfo > vecTemp;
	std::vector< KPostItemTitleInfo >::iterator vit;

	//#1.유저메일 리스트중 읽지않은 메일을 찾아낸다.
	for( vit = m_vecUserLetterList.begin(); vit != m_vecUserLetterList.end(); ++vit )
	{
		if( vit->m_bRead == false )
		{
			vecTemp.push_back( *vit );
			++iNewUserLetterCount;
		}
	}
	//#2.시스템메일 리스트중 읽지않은 메일을 찾아낸다.
	for( vit = m_vecSystemLetterList.begin(); vit != m_vecSystemLetterList.end(); ++vit )
	{
		if( vit->m_bRead == false )
		{
			vecTemp.push_back( *vit );
			++iNewSystemLetterCount;
		}
	}
	//#3.웨딩메일 리스트중 읽지않은 메일을 찾아낸다.
	for( vit = m_vecRelationshipLetterList.begin(); vit != m_vecRelationshipLetterList.end(); ++vit )
	{
		if( vit->m_bRead == false )
		{
			vecTemp.push_back( *vit );
			++iNewRelationshipLetterCount;
		}
	}
	//{{ 2011. 12. 21	최육사	처리 지연 패킷 모니터링
#ifdef SERV_DELAY_EVENT_MORNITOING
	const DWORD dwBeginTick = ::GetTickCount();
#endif SERV_DELAY_EVENT_MORNITOING
	//}}

	//#3.읽지않은 메일을 최근순으로 정렬한다.
	std::sort(vecTemp.begin(), vecTemp.end(), &ComRenewalLetter );

	//{{ 2011. 12. 21	최육사	처리 지연 패킷 모니터링
#ifdef SERV_DELAY_EVENT_MORNITOING
	const DWORD dwLatency = ::GetTickCount() - dwBeginTick;
	if( dwLatency > 3000 )
	{
		START_LOG( cout, L"[모니터링] 우편 리스트 정렬하는데 무려 3초이상 걸렸네! 헐!!!" )
			<< BUILD_LOG( spUser->GetUID() )
			<< BUILD_LOG( spUser->GetCharUID() )
			<< BUILD_LOG( spUser->GetCharName() )
			<< BUILD_LOG( dwLatency )
			<< BUILD_LOG( m_vecUserLetterList.size() )
			<< BUILD_LOG( m_vecSystemLetterList.size() )
			<< BUILD_LOG( m_vecRelationshipLetterList.size() )
			<< BUILD_LOG( vecTemp.size() );
	}
#endif SERV_DELAY_EVENT_MORNITOING
	//}}

	//#4.읽지않은 5개의 메일을 뽑아내어 넣어준다.
	//int i = 0;
	//for( vit = vecTemp.begin(); vit != vecTemp.end(); ++vit )
	for( int i = 0; i < static_cast<int>(vecTemp.size()) && i < NEW_LETTER_TITLE_COUNT; ++i )
	{
		KPostSmallTitleInfo kData;
		kData.m_cScriptType		= vecTemp[i].m_cScriptType;
		kData.m_wstrLetterTitle	= vecTemp[i].m_wstrTitle;
		vecNewLetterTitle.push_back( kData );
		//++i;
	}
}

#else	// SERV_RELATIONSHIP_SYSTEM

void KUserLetterBox::GetRenewalLetter( IN KGSUserPtr spUser, OUT int& iNewUserLetterCount, OUT int& iNewSystemLetterCount, OUT std::vector< KPostSmallTitleInfo >& vecNewLetterTitle )
{
	iNewUserLetterCount = 0;
	iNewSystemLetterCount = 0;
	vecNewLetterTitle.clear();

	std::vector< KPostItemTitleInfo > vecTemp;
	std::vector< KPostItemTitleInfo >::iterator vit;

	//#1.유저메일 리스트중 읽지않은 메일을 찾아낸다.
	for( vit = m_vecUserLetterList.begin(); vit != m_vecUserLetterList.end(); ++vit )
	{
		if( vit->m_bRead == false )
		{
			vecTemp.push_back( *vit );
			++iNewUserLetterCount;
		}
	}
	//#2.시스템메일 리스트중 읽지않은 메일을 찾아낸다.
	for( vit = m_vecSystemLetterList.begin(); vit != m_vecSystemLetterList.end(); ++vit )
	{
		if( vit->m_bRead == false )
		{
			vecTemp.push_back( *vit );
			++iNewSystemLetterCount;
		}
	}

	//{{ 2011. 12. 21	최육사	처리 지연 패킷 모니터링
#ifdef SERV_DELAY_EVENT_MORNITOING
	const DWORD dwBeginTick = ::GetTickCount();
#endif SERV_DELAY_EVENT_MORNITOING
	//}}

	//#3.읽지않은 메일을 최근순으로 정렬한다.
	std::sort(vecTemp.begin(), vecTemp.end(), &ComRenewalLetter );

	//{{ 2011. 12. 21	최육사	처리 지연 패킷 모니터링
#ifdef SERV_DELAY_EVENT_MORNITOING
	const DWORD dwLatency = ::GetTickCount() - dwBeginTick;
	if( dwLatency > 3000 )
	{
		START_LOG( cout, L"[모니터링] 우편 리스트 정렬하는데 무려 3초이상 걸렸네! 헐!!!" )
			<< BUILD_LOG( spUser->GetUID() )
			<< BUILD_LOG( spUser->GetCharUID() )
			<< BUILD_LOG( spUser->GetCharName() )
			<< BUILD_LOG( dwLatency )
			<< BUILD_LOG( m_vecUserLetterList.size() )
			<< BUILD_LOG( m_vecSystemLetterList.size() )
			<< BUILD_LOG( vecTemp.size() );
	}
#endif SERV_DELAY_EVENT_MORNITOING
	//}}

	//#4.읽지않은 5개의 메일을 뽑아내어 넣어준다.
	//int i = 0;
	//for( vit = vecTemp.begin(); vit != vecTemp.end(); ++vit )
	for( int i = 0; i < static_cast<int>(vecTemp.size()) && i < NEW_LETTER_TITLE_COUNT; ++i )
	{
		KPostSmallTitleInfo kData;
		kData.m_cScriptType		= vecTemp[i].m_cScriptType;
		kData.m_wstrLetterTitle	= vecTemp[i].m_wstrTitle;
		vecNewLetterTitle.push_back( kData );
		//++i;
	}
}
#endif SERV_RELATIONSHIP_SYSTEM
//}

void KUserLetterBox::AddLetter( const KPostItemInfo& kPostItem )
{
	KPostItemInfo kInsertLetter = kPostItem;

	//{{ 2010. 11. 8	최육사	중복 닉네임 버그 해결
#ifdef SERV_UNIQUE_NICKNAME_FIX

	// 닉네임에 언더바가 있으면 삭제되거나 변경된 닉네임 입니다.
	if( kPostItem.m_wstrFromNickName.find( '_' ) != -1 )
	{
		//{{ 2011.09.16 조효진  넷 에러 스트링을 클라이언트쪽에서 가져와서 출력
#ifdef SERV_NETERROR_STR_GET_FROM_CLIENT
		kInsertLetter.m_wstrFromNickName = L"";
		kInsertLetter.m_bSenderDeleted = true;
#else SERV_NETERROR_STR_GET_FROM_CLIENT
		kInsertLetter.m_wstrFromNickName = NetError::GetErrStrF( NetError::STR_DELETED_NICKNAME );
#endif SERV_NETERROR_STR_GET_FROM_CLIENT
		//}}
	}
#endif SERV_UNIQUE_NICKNAME_FIX
	//}}

	// 복구 이벤트 편지에 대한 추가 처리
	if( kInsertLetter.m_cScriptType == KPostItemInfo::LT_BROKEN_ITEM )
	{
		kInsertLetter.m_cEnchantLevel = -( kInsertLetter.m_cEnchantLevel );
	}

	// map
	m_mapLetters.insert( std::make_pair( kPostItem.m_iPostNo, kInsertLetter ) );

	// list
	KPostItemTitleInfo kPostTitle;
	kPostTitle.m_iPostNo		  = kInsertLetter.m_iPostNo;
	kPostTitle.m_cScriptType	  = kInsertLetter.m_cScriptType;
	kPostTitle.m_iScriptIndex	  = kInsertLetter.m_iScriptIndex;
	kPostTitle.m_cEnchantLevel	  = kInsertLetter.m_cEnchantLevel;
	kPostTitle.m_wstrRegDate	  = kInsertLetter.m_wstrRegDate;
	kPostTitle.m_wstrFromNickName = kInsertLetter.m_wstrFromNickName;
	kPostTitle.m_wstrTitle		  = kInsertLetter.m_wstrTitle;
	kPostTitle.m_bRead			  = kInsertLetter.m_bRead;
	//{{ 2011.09.16 조효진  넷 에러 스트링을 클라이언트쪽에서 가져와서 출력
#ifdef SERV_NETERROR_STR_GET_FROM_CLIENT
	kPostTitle.m_bSenderDeleted	  = kInsertLetter.m_bSenderDeleted;
#endif SERV_NETERROR_STR_GET_FROM_CLIENT
	//}}
	//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
	switch( kPostTitle.m_cScriptType )
	{
	case KPostItemInfo::LT_POST_OFFICE:
		{
			m_vecUserLetterList.push_back( kPostTitle );
		}
		break;
	case KPostItemInfo::LT_WEDDING_REWARD:
	case KPostItemInfo::LT_WEDDING_RESERVE:
	case KPostItemInfo::LT_WEDDING_INVITATION:
		{
			m_vecRelationshipLetterList.push_back( kPostTitle );
		}
		break;
	default:
		{
			m_vecSystemLetterList.push_back( kPostTitle );
		}
	}
#else
	if( kPostTitle.m_cScriptType == KPostItemInfo::LT_POST_OFFICE )
	{
		m_vecUserLetterList.push_back( kPostTitle );
	}
	else
	{
		m_vecSystemLetterList.push_back( kPostTitle );
	}
#endif SERV_RELATIONSHIP_SYSTEM
	//}
}

bool KUserLetterBox::IsExist( UidType iPostNo )
{
	std::map< UidType, KPostItemInfo >::const_iterator mit;
	mit = m_mapLetters.find( iPostNo );
	if( mit == m_mapLetters.end() )
		return false;

	return true;
}

bool KUserLetterBox::IsExist( const std::vector< UidType >& vecPostNo )
{
	std::vector< UidType >::const_iterator vit;
	for( vit = vecPostNo.begin(); vit != vecPostNo.end(); ++vit )
	{
		if( !IsExist( *vit ) )
			return false;
	}

	return true;
}

bool KUserLetterBox::IsSystemLetter( IN UidType iPostNo )
{
	std::map< UidType, KPostItemInfo >::const_iterator mit;
	mit = m_mapLetters.find( iPostNo );
	if( mit == m_mapLetters.end() )
	{
		START_LOG( cerr, L"편지가 없네.." )
			<< BUILD_LOG( iPostNo )
			<< END_LOG;
		return false;
	}

	return mit->second.IsSystemLetter();
}

//{{ 2012. 04. 17	최육사	퀘스트 조건 마을 입장
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
bool KUserLetterBox::IsItemIDLetter( IN const UidType iPostNo )
{
	std::map< UidType, KPostItemInfo >::const_iterator mit;
	mit = m_mapLetters.find( iPostNo );
	if( mit == m_mapLetters.end() )
	{
		START_LOG( cerr, L"편지가 없네.." )
			<< BUILD_LOG( iPostNo )
			<< END_LOG;
		return false;
	}

	return KPostItemInfo::IsItemIDLetter( mit->second.m_cScriptType );
}
#endif SERV_ENTER_FIELD_QUEST_CLEAR
//}}

//{{ 2011. 08. 29	최육사	캐릭터 레벨업 이벤트
#ifdef SERV_CHAR_LEVEL_UP_EVENT
bool KUserLetterBox::IsExistRewardInLetter( IN const int iRewardID ) const
{
	std::map< UidType, KPostItemInfo >::const_iterator mit;
	for( mit = m_mapLetters.begin(); mit != m_mapLetters.end(); ++mit )
	{
        if( mit->second.IsSystemLetter() == false )
			continue;

		if( mit->second.m_iScriptIndex == iRewardID )
			return true;
	}

	return false;
}
#else
//{{ 2012. 04. 30	박세훈	현자의 주문서 접속 이벤트 ( 우편함 중복 체크 )
#ifdef SERV_SCROLL_OF_SAGE_CHECK_THE_LETTER_BOX
bool KUserLetterBox::IsExistRewardInLetter( IN const int iRewardID ) const
{
	std::map< UidType, KPostItemInfo >::const_iterator mit;
	for( mit = m_mapLetters.begin(); mit != m_mapLetters.end(); ++mit )
	{
		if( mit->second.IsSystemLetter() == false )
			continue;

		if( mit->second.m_iScriptIndex == iRewardID )
			return true;
	}

	return false;
}
#endif SERV_SCROLL_OF_SAGE_CHECK_THE_LETTER_BOX
//}}
#endif SERV_CHAR_LEVEL_UP_EVENT
//}}

bool KUserLetterBox::GetLetter( IN UidType iPostNo, OUT KPostItemInfo& kPostItem )
{
	std::map< UidType, KPostItemInfo >::const_iterator mit;
	mit = m_mapLetters.find( iPostNo );
	if( mit == m_mapLetters.end() )
		return false;

	kPostItem = mit->second;
	return true;
}

//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
bool KUserLetterBox::ReadLetter( UidType iPostNo, KPostItemInfo& kPostItem, bool& bReadUpdated )
{
	std::map< UidType, KPostItemInfo >::iterator mit;
	mit = m_mapLetters.find( iPostNo );
	if( mit == m_mapLetters.end() )
		return false;

	bReadUpdated = mit->second.m_bRead;
	mit->second.m_bRead = true;
	kPostItem = mit->second;

	// 시스템 우편과 일반 우편을 나눈다.
	switch( kPostItem.GetLetterType() )
	{
	case  KPostItemInfo::LT_POST_OFFICE:
		{
			std::vector< KPostItemTitleInfo >::iterator vit;
			for( vit = m_vecUserLetterList.begin(); vit != m_vecUserLetterList.end(); ++vit )
			{
				if( vit->m_iPostNo == iPostNo )
				{
					vit->m_bRead = true;
					break;
				}
			}
		}
		break;
	case  KPostItemInfo::LT_WEDDING_REWARD:
	case  KPostItemInfo::LT_WEDDING_INVITATION:
	case  KPostItemInfo::LT_WEDDING_RESERVE:
		{
			std::vector< KPostItemTitleInfo >::iterator vit;
			for( vit = m_vecRelationshipLetterList.begin(); vit != m_vecRelationshipLetterList.end(); ++vit )
			{
				if( vit->m_iPostNo == iPostNo )
				{
					vit->m_bRead = true;
					break;
				}
			}		
		}
		break;
	default:
		{
			std::vector< KPostItemTitleInfo >::iterator vit;
			for( vit = m_vecSystemLetterList.begin(); vit != m_vecSystemLetterList.end(); ++vit )
			{
				if( vit->m_iPostNo == iPostNo )
				{
					vit->m_bRead = true;
					break;
				}
			}
		}
	}

	return true;	
}

#else // SERV_RELATIONSHIP_SYSTEM

bool KUserLetterBox::ReadLetter( UidType iPostNo, KPostItemInfo& kPostItem, bool& bReadUpdated )
{
	std::map< UidType, KPostItemInfo >::iterator mit;
	mit = m_mapLetters.find( iPostNo );
	if( mit == m_mapLetters.end() )
		return false;

	bReadUpdated = mit->second.m_bRead;
	mit->second.m_bRead = true;
	kPostItem = mit->second;

	// 시스템 우편과 일반 우편을 나눈다.
	if( kPostItem.IsSystemLetter() == true )
	{
		std::vector< KPostItemTitleInfo >::iterator vit;
		for( vit = m_vecSystemLetterList.begin(); vit != m_vecSystemLetterList.end(); ++vit )
		{
			if( vit->m_iPostNo == iPostNo )
			{
				vit->m_bRead = true;
				break;
			}
		}
	}
	else
	{
		std::vector< KPostItemTitleInfo >::iterator vit;
		for( vit = m_vecUserLetterList.begin(); vit != m_vecUserLetterList.end(); ++vit )
		{
			if( vit->m_iPostNo == iPostNo )
			{
				vit->m_bRead = true;
				break;
			}
		}		
	}

	return true;	
}
#endif SERV_RELATIONSHIP_SYSTEM
//}

//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
int KUserLetterBox::GetTotalLetterCount( IN char cLetterListType )
{
	switch( cLetterListType )
	{
	case KEGS_GET_POST_LETTER_LIST_REQ::LLT_SYSTEM:
		{
			return m_vecSystemLetterList.size();
		}break;
	case KEGS_GET_POST_LETTER_LIST_REQ::LLT_WEDDING:
		{
			return m_vecRelationshipLetterList.size();
		}break;
	default:
		{

		}		
	}

	return m_vecUserLetterList.size();
}
#else // SERV_RELATIONSHIP_SYSTEM
int KUserLetterBox::GetTotalLetterCount( IN bool bSystemLetter )
{
	return ( bSystemLetter ? m_vecSystemLetterList.size() : m_vecUserLetterList.size() );
}
#endif SERV_RELATIONSHIP_SYSTEM
//}


bool KUserLetterBox::GetItemInfo( UidType iPostNo, KItemInfo& kNewItemInfo )
{
	std::map< UidType, KPostItemInfo >::const_iterator mit;
	mit = m_mapLetters.find( iPostNo );
	if( mit == m_mapLetters.end() )
		return false;

	const KPostItemInfo& kPostItemInfo = mit->second;

	//{{ 2012. 04. 17	최육사	퀘스트 조건 마을 입장
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
	if( KPostItemInfo::IsItemIDLetter( kPostItemInfo.m_cScriptType ) == false )
	{
		START_LOG( cerr, L"일반편지 또는 GM메시지가 아닌데 아이템정보를 얻으려 합니다." )
			<< BUILD_LOG( iPostNo )
			<< BUILD_LOG( kPostItemInfo.m_cScriptType )
			<< END_LOG;
		return false;
	}
#else
	switch( kPostItemInfo.m_cScriptType )
	{
	case KPostItemInfo::LT_POST_OFFICE:
	case KPostItemInfo::LT_MESSAGE:
	case KPostItemInfo::LT_BROKEN_ITEM:
	case KPostItemInfo::LT_WEB_POINT_EVENT:
		//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
	case KPostItemInfo::LT_WEDDING_REWARD:
	case KPostItemInfo::LT_WEDDING_INVITATION:
	case KPostItemInfo::LT_WEDDING_RESERVE:
#endif SERV_RELATIONSHIP_SYSTEM
		//}
		break;

	default:
		{
			START_LOG( cerr, L"일반편지 또는 GM메시지가 아닌데 아이템정보를 얻으려 합니다." )
				<< BUILD_LOG( iPostNo )
				<< BUILD_LOG( kPostItemInfo.m_cScriptType )
				<< END_LOG;
			return false;
		}
		break;
	}
#endif SERV_ENTER_FIELD_QUEST_CLEAR
	//}}

	const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( kPostItemInfo.m_iScriptIndex );
	if( pItemTemplet == NULL )
	{
		START_LOG( cerr, L"아이템 템플릿이 존재하지 않습니다." )
			<< BUILD_LOG( iPostNo )
			<< BUILD_LOG( kPostItemInfo.m_iScriptIndex )
			<< END_LOG;
		return false;
	}

	// 아이템 정보 담기
	kNewItemInfo.m_iItemID		   = kPostItemInfo.m_iScriptIndex;
	kNewItemInfo.m_iQuantity	   = kPostItemInfo.m_iQuantity;
	kNewItemInfo.m_cUsageType	   = pItemTemplet->m_PeriodType;
	kNewItemInfo.m_sEndurance	   = pItemTemplet->m_Endurance;
	kNewItemInfo.m_cEnchantLevel   = kPostItemInfo.m_cEnchantLevel;
	kNewItemInfo.m_kAttribEnchantInfo = kPostItemInfo.m_kAttribEnchantInfo;
	kNewItemInfo.m_vecItemSocket   = kPostItemInfo.m_vecItemSocket;
#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-20	// 박세훈
	kNewItemInfo.m_byteExpandedSocketNum	= kPostItemInfo.m_byteExpandedSocketNum;
#endif // SERV_BATTLE_FIELD_BOSS
	//{{ 2013. 06. 04	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
	kNewItemInfo.m_vecRandomSocket = kPostItemInfo.m_vecRandomSocket;
#endif SERV_NEW_ITEM_SYSTEM_2013_05
	//}}
	//{{ 2009. 9. 7  최육사		밀봉
	kNewItemInfo.m_ucSealData	   = kPostItemInfo.m_ucSealData;
	//}}
	return true;
}

//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
char KUserLetterBox::GetLetterType( IN UidType iPostNo )
{
	std::map< UidType, KPostItemInfo >::const_iterator mit;
	mit = m_mapLetters.find( iPostNo );
	if( mit == m_mapLetters.end() )
	{
		START_LOG( cerr, L"편지가 없네.." )
			<< BUILD_LOG( iPostNo )
			<< END_LOG;
		return false;
	}

	return mit->second.GetLetterType();
}
bool KUserLetterBox::GetLetterList( IN char cLetterListType, IN int iPageNo, OUT std::vector< KPostItemTitleInfo >& vecLetterList )
{
	vecLetterList.clear();	

	if( iPageNo <= 0 )
	{
		START_LOG( cerr, L"페이지 요청이 이상합니다." )
			<< BUILD_LOGc( cLetterListType )
			<< BUILD_LOG( iPageNo )
			<< END_LOG;
		return false;
	}

	u_int iBeginIndex = ( iPageNo - 1 ) * ONE_PAGE_LETTER_COUNT;

	switch( cLetterListType )
	{	
	case  KEGS_GET_POST_LETTER_LIST_REQ::LLT_SYSTEM:
		{
			u_int iCnt = 0;
			std::vector< KPostItemTitleInfo >::reverse_iterator vit;
			for( vit = m_vecSystemLetterList.rbegin(); vit != m_vecSystemLetterList.rend(); ++vit, ++iCnt )
			{
				if( iBeginIndex > iCnt )
					continue;

				vecLetterList.push_back( *vit );

				if( vecLetterList.size() >= ONE_PAGE_LETTER_COUNT )
					break;
			}
		}
		break;
	case  KEGS_GET_POST_LETTER_LIST_REQ::LLT_WEDDING:
		{
			u_int iCnt = 0;
			std::vector< KPostItemTitleInfo >::reverse_iterator vit;
			for( vit = m_vecRelationshipLetterList.rbegin(); vit != m_vecRelationshipLetterList.rend(); ++vit, ++iCnt )
			{
				if( iBeginIndex > iCnt )
					continue;

				vecLetterList.push_back( *vit );

				if( vecLetterList.size() >= ONE_PAGE_LETTER_COUNT )
					break;
			}
		}
		break;
	default:
		{
			u_int iCnt = 0;
			std::vector< KPostItemTitleInfo >::reverse_iterator vit;
			for( vit = m_vecUserLetterList.rbegin(); vit != m_vecUserLetterList.rend(); ++vit, ++iCnt )
			{
				if( iBeginIndex > iCnt )
					continue;

				vecLetterList.push_back( *vit );

				if( vecLetterList.size() >= ONE_PAGE_LETTER_COUNT )
					break;
			}
		}
	}

	return true;
}

#else	// SERV_RELATIONSHIP_SYSTEM

bool KUserLetterBox::GetLetterList( IN bool bSystemLetter, IN int iPageNo, OUT std::vector< KPostItemTitleInfo >& vecLetterList )
{
	vecLetterList.clear();	

	if( iPageNo <= 0 )
	{
		START_LOG( cerr, L"페이지 요청이 이상합니다." )
			<< BUILD_LOG( bSystemLetter )
			<< BUILD_LOG( iPageNo )
			<< END_LOG;
		return false;
	}

	u_int iBeginIndex = ( iPageNo - 1 ) * ONE_PAGE_LETTER_COUNT;

	if( bSystemLetter )
	{	
		u_int iCnt = 0;
		std::vector< KPostItemTitleInfo >::reverse_iterator vit;
		for( vit = m_vecSystemLetterList.rbegin(); vit != m_vecSystemLetterList.rend(); ++vit, ++iCnt )
		{
			if( iBeginIndex > iCnt )
				continue;

			vecLetterList.push_back( *vit );

			if( vecLetterList.size() >= ONE_PAGE_LETTER_COUNT )
				break;
		}
	}
	else
	{
		u_int iCnt = 0;
		std::vector< KPostItemTitleInfo >::reverse_iterator vit;
		for( vit = m_vecUserLetterList.rbegin(); vit != m_vecUserLetterList.rend(); ++vit, ++iCnt )
		{
			if( iBeginIndex > iCnt )
				continue;

			vecLetterList.push_back( *vit );

			if( vecLetterList.size() >= ONE_PAGE_LETTER_COUNT )
				break;
		}
	}

	return true;
}
#endif SERV_RELATIONSHIP_SYSTEM
//}

void KUserLetterBox::DeleteLetter( UidType iPostNo )
{
	std::map< UidType, KPostItemInfo >::const_iterator mit;
	mit = m_mapLetters.find( iPostNo );
	if( mit == m_mapLetters.end() )
		return;

	//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
	// 우편 리스트에서 삭제
	switch( mit->second.GetLetterType() )
	{
	case KPostItemInfo::LT_POST_OFFICE:
		{
			std::vector< KPostItemTitleInfo >::iterator vit;
			for( vit = m_vecUserLetterList.begin(); vit != m_vecUserLetterList.end(); ++vit )
			{
				if( vit->m_iPostNo == iPostNo )
					break;
			}

			if( vit != m_vecUserLetterList.end() )
			{
				m_vecUserLetterList.erase( vit );
			}	
		}
		break;
	case KPostItemInfo::LT_WEDDING_REWARD:
	case KPostItemInfo::LT_WEDDING_RESERVE:
	case KPostItemInfo::LT_WEDDING_INVITATION:
		{
			std::vector< KPostItemTitleInfo >::iterator vit;
			for( vit = m_vecRelationshipLetterList.begin(); vit != m_vecRelationshipLetterList.end(); ++vit )
			{
				if( vit->m_iPostNo == iPostNo )
					break;
			}

			if( vit != m_vecRelationshipLetterList.end() )
			{
				m_vecRelationshipLetterList.erase( vit );
			}	
		}
		break;
	default:
		{
			std::vector< KPostItemTitleInfo >::iterator vit;
			for( vit = m_vecSystemLetterList.begin(); vit != m_vecSystemLetterList.end(); ++vit )
			{
				if( vit->m_iPostNo == iPostNo )
					break;
			}

			if( vit != m_vecSystemLetterList.end() )
			{
				m_vecSystemLetterList.erase( vit );
			}	
		}
	}
#else
	// 우편 리스트에서 삭제
	if( mit->second.IsSystemLetter() == true )
	{
		std::vector< KPostItemTitleInfo >::iterator vit;
		for( vit = m_vecSystemLetterList.begin(); vit != m_vecSystemLetterList.end(); ++vit )
		{
			if( vit->m_iPostNo == iPostNo )
				break;
		}

		if( vit != m_vecSystemLetterList.end() )
		{
			m_vecSystemLetterList.erase( vit );
		}
	}
	else
	{
		std::vector< KPostItemTitleInfo >::iterator vit;
		for( vit = m_vecUserLetterList.begin(); vit != m_vecUserLetterList.end(); ++vit )
		{
			if( vit->m_iPostNo == iPostNo )
				break;
		}

		if( vit != m_vecUserLetterList.end() )
		{
			m_vecUserLetterList.erase( vit );
		}		
	}
#endif SERV_RELATIONSHIP_SYSTEM
	//}

	// 우편 map에서 삭제
	m_mapLetters.erase( iPostNo );
}

void KUserLetterBox::DeleteLetter( const std::vector< UidType >& vecPostNo )
{
	std::vector< UidType >::const_iterator vit;
	for( vit = vecPostNo.begin(); vit != vecPostNo.end(); ++vit )
	{
		DeleteLetter( *vit );
	}
}

void KUserLetterBox::SetReceivedFlag( IN UidType iPostNo )
{
	std::map< UidType, KPostItemInfo >::iterator mit;
	mit = m_mapLetters.find( iPostNo );
	if( mit == m_mapLetters.end() )
		return;

	KPostItemInfo& kPostItemInfo = mit->second;

	// 첨부물 정보 초기화
	kPostItemInfo.m_iScriptIndex    = 0;
	kPostItemInfo.m_iQuantity	    = 0;
	kPostItemInfo.m_cEnchantLevel   = 0;
	kPostItemInfo.m_kAttribEnchantInfo.Init();
	kPostItemInfo.m_vecItemSocket.clear();
	//{{ 2009. 9. 7  최육사		밀봉
	kPostItemInfo.m_ucSealData		= 0;
	//}}

	//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
	switch( mit->second.GetLetterType() )
	{
	case KPostItemInfo::LT_POST_OFFICE:
		{
			std::vector< KPostItemTitleInfo >::iterator vit;
			for( vit = m_vecUserLetterList.begin(); vit != m_vecUserLetterList.end(); ++vit )
			{
				if( vit->m_iPostNo == iPostNo )
					break;
			}

			if( vit != m_vecUserLetterList.end() )
			{
				vit->m_iScriptIndex = 0;
			}
		}
		break;
	case KPostItemInfo::LT_WEDDING_RESERVE:
	case KPostItemInfo::LT_WEDDING_INVITATION:
		{
			std::vector< KPostItemTitleInfo >::iterator vit;
			for( vit = m_vecRelationshipLetterList.begin(); vit != m_vecRelationshipLetterList.end(); ++vit )
			{
				if( vit->m_iPostNo == iPostNo )
					break;
			}

			if( vit != m_vecRelationshipLetterList.end() )
			{
				vit->m_iScriptIndex = 0;
			}
		}
		break;
	default:
		{
			std::vector< KPostItemTitleInfo >::iterator vit;
			for( vit = m_vecSystemLetterList.begin(); vit != m_vecSystemLetterList.end(); ++vit )
			{
				if( vit->m_iPostNo == iPostNo )
					break;
			}

			if( vit != m_vecSystemLetterList.end() )
			{
				vit->m_iScriptIndex = 0;
			}
		}
	}
#else
	if( mit->second.IsSystemLetter() == true )
	{
		std::vector< KPostItemTitleInfo >::iterator vit;
		for( vit = m_vecSystemLetterList.begin(); vit != m_vecSystemLetterList.end(); ++vit )
		{
			if( vit->m_iPostNo == iPostNo )
				break;
		}

		if( vit != m_vecSystemLetterList.end() )
		{
			vit->m_iScriptIndex = 0;
		}
	}
	else
	{
		std::vector< KPostItemTitleInfo >::iterator vit;
		for( vit = m_vecUserLetterList.begin(); vit != m_vecUserLetterList.end(); ++vit )
		{
			if( vit->m_iPostNo == iPostNo )
				break;
		}

		if( vit != m_vecUserLetterList.end() )
		{
			vit->m_iScriptIndex = 0;
		}		
	}
#endif SERV_RELATIONSHIP_SYSTEM
	//}
}

void KUserLetterBox::InitBlackList( IN const std::vector< std::wstring >& vecBlackList )
{
	std::vector< std::wstring >::const_iterator vit;
	for( vit = vecBlackList.begin(); vit != vecBlackList.end(); ++vit )
	{
		m_setBlackList.insert( *vit );
	}	
}

void KUserLetterBox::GetBlackList( OUT std::vector< std::wstring >& vecBlackList )
{
	std::set< std::wstring >::const_iterator sit;
	for( sit = m_setBlackList.begin(); sit != m_setBlackList.end(); ++sit )
	{
		vecBlackList.push_back( *sit );
	}
}

bool KUserLetterBox::IsBlackList( IN std::wstring& wstrNickName )
{
	std::set< std::wstring >::const_iterator sit;
	sit = m_setBlackList.find( wstrNickName );
	if( sit == m_setBlackList.end() )
		return false;

	return true;
}

void KUserLetterBox::AddBlackList( IN std::wstring& wstrNickName )
{
	m_setBlackList.insert( wstrNickName );
}

void KUserLetterBox::DelBlackList( IN std::wstring& wstrNickName )
{
	m_setBlackList.erase( wstrNickName );
}


