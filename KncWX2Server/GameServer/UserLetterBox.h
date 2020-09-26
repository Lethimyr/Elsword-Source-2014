#pragma once
#include "Event.h"
#include "CommonPacket.h"


class KUserLetterBox
{
public:
	enum
	{
		ONE_PAGE_LETTER_COUNT = 6,
		NEW_LETTER_TITLE_COUNT = 5,
	};

public:
	KUserLetterBox(void);
	~KUserLetterBox(void);

	void Clear();
	void Init( IN KGSUserPtr spUser,
			   IN const std::vector< KPostItemInfo >& vecPostItems, 
			   OUT int& iNewUserLetterCount, 
			   OUT int& iNewSystemLetterCount, 
			   //{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
			   OUT int& iNewRelationshipLetterCount,
#endif SERV_RELATIONSHIP_SYSTEM
			   //}
			   OUT std::vector< KPostSmallTitleInfo >& vecNewLetterTitle );

	void GetRenewalLetter( IN KGSUserPtr spUser,
						   OUT int& iNewUserLetterCount, 
						   OUT int& iNewSystemLetterCount, 
						   //{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
						   OUT int& iNewRelationshipLetterCount, 
#endif SERV_RELATIONSHIP_SYSTEM
						   //}
						   OUT std::vector< KPostSmallTitleInfo >& vecNewLetterTitle );


	void AddLetter( IN const KPostItemInfo& kPostItem );	

	bool IsExist( IN UidType iPostNo );
	bool IsExist( IN const std::vector< UidType >& vecPostNo );
	bool IsSystemLetter( IN UidType iPostNo );

	//{{ 2012. 04. 17	최육사	퀘스트 조건 마을 입장
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
	bool IsItemIDLetter( IN const UidType iPostNo );
#endif SERV_ENTER_FIELD_QUEST_CLEAR
	//}}
	//{{ 2011. 08. 29	최육사	캐릭터 레벨업 이벤트
#ifdef SERV_CHAR_LEVEL_UP_EVENT
	bool IsExistRewardInLetter( IN const int iRewardID ) const;
#else
		//{{ 2012. 04. 30	박세훈	현자의 주문서 접속 이벤트 ( 우편함 중복 체크 )
	#ifdef SERV_SCROLL_OF_SAGE_CHECK_THE_LETTER_BOX
		bool IsExistRewardInLetter( IN const int iRewardID ) const;
	#endif SERV_SCROLL_OF_SAGE_CHECK_THE_LETTER_BOX
		//}}
#endif SERV_CHAR_LEVEL_UP_EVENT
	//}}

	bool GetLetter( IN UidType iPostNo, OUT KPostItemInfo& kPostItem );
	bool ReadLetter( IN UidType iPostNo, OUT KPostItemInfo& kPostItem, OUT bool& bReadUpdated );

	//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
	int  GetTotalLetterCount( IN char cLetterListType );
#else
	int  GetTotalLetterCount( IN bool bSystemLetter );
#endif SERV_RELATIONSHIP_SYSTEM
	//}

	bool GetItemInfo( IN UidType iPostNo, OUT KItemInfo& kNewItemInfo );
	//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
	char GetLetterType( IN UidType iPageNo );
	bool GetLetterList( IN char cLetterListType, IN int iPageNo, OUT std::vector< KPostItemTitleInfo >& vecLetterList );
#else
	bool GetLetterList( IN bool bSystemLetter, IN int iPageNo, OUT std::vector< KPostItemTitleInfo >& vecLetterList );
#endif SERV_RELATIONSHIP_SYSTEM
	//}

	void DeleteLetter( IN UidType iPostNo );
	void DeleteLetter( IN const std::vector< UidType >& vecPostNo );

	void SetReceivedFlag( IN UidType iPostNo );

	// 스팸 편지 방지 블랙리스트
	void InitBlackList( IN const std::vector< std::wstring >& vecBlackList );
	void GetBlackList( OUT std::vector< std::wstring >& vecBlackList );
	bool IsBlackList( IN std::wstring& wstrNickName );
	void AddBlackList( IN std::wstring& wstrNickName );
	void DelBlackList( IN std::wstring& wstrNickName );

private:
	std::map< UidType, KPostItemInfo >	m_mapLetters;
	std::vector< KPostItemTitleInfo >	m_vecUserLetterList;
	std::vector< KPostItemTitleInfo >	m_vecSystemLetterList;
	//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
	std::vector< KPostItemTitleInfo >	m_vecRelationshipLetterList;
#endif SERV_RELATIONSHIP_SYSTEM
	//}
	std::set< std::wstring >			m_setBlackList;

	std::wstring						m_wstrNickName;
};
