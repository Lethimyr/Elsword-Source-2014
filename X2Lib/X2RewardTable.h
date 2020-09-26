#pragma once

#include <map>
#include <vector>

struct KItemInfo;
struct KPostItemInfo;
class CX2RewardTable
{
public:
#ifdef SERV_EVENT_DB_CONTROL_SYSTEM
#else //SERV_EVENT_DB_CONTROL_SYSTEM
	struct KRewardData
	{	
		int				m_iRewardItemID;
		int				m_iQuantity;
		short			m_sPeriod;

		KRewardData(){
			m_iRewardItemID	= -1;
			m_iQuantity		= 0;
			m_sPeriod		= 0;
		};
		KRewardData( const KRewardData& t ) { *this = t; }    // copy constructor
		KRewardData& operator=( const KRewardData& t )       // assign operator
		{			
			m_iRewardItemID = t.m_iRewardItemID;
			m_iQuantity = t.m_iQuantity;
			m_sPeriod = t.m_sPeriod;

			return *this;
		}
	};
#endif //SERV_EVENT_DB_CONTROL_SYSTEM	

public:
	CX2RewardTable(void);
	~CX2RewardTable(void);

	bool OpenScriptFile( const wchar_t* pFileName );
	bool AddRewardData_LUA( int iRewardID, int iItemID, int iQuantity, short sPeriod );
	//{{ 2011. 06. 22    ±è¹Î¼º    ÀÌÅ» ¹æÁö ¸ðµ¨ - NEXON ¼¼¼Ç ½Ã½ºÅÛ ¼öÁ¤
#ifdef SERV_NEXON_SESSION_PROTOCOL
	void SetOutUserRetainingRewardItemInfo_LUA( int iSlot, int iItemID, int iQuantity );
#endif SERV_NEXON_SESSION_PROTOCOL
	//}} 
	//{{ 2011. 10.26    ±è¹Î¼º	Ä³¸¯ÅÍ ·¹º§¾÷ ÀÌº¥Æ®
#ifdef SERV_CHAR_LEVEL_UP_EVENT
	void AddLevelUpRewardData_LUA( u_char ucLevel, int iRewardID );
#endif SERV_CHAR_LEVEL_UP_EVENT
	//}}

	void MakeSystemLetterTitle( IN const std::wstring& wstrNickName, IN OUT KPostItemTitleInfo& kPostTitle );
	void MakeSystemLetter( IN const std::wstring& wstrNickName, IN OUT KPostItemInfo& kPostItem );

	void AssignStringFromRewardTable( IN const int iRewardID, OUT std::wstring& wstrMessage, IN const bool bOnlyItemName = false );

	void GetRewardItem( IN const KPostItemInfo& kPostItemInfo, OUT std::map< int, KItemInfo >& mapRewardItem ) const;
	void GetRewardItem( IN const std::vector< KPostItemInfo >& vecPostItemInfo, OUT std::map< int, KItemInfo >& mapRewardItem ) const;
	bool GetRewardInfo( IN int iRewardID, OUT std::map< int, int >& mapRewardInfo ) const;
	void GetRewardInfo( IN int iRewardType, IN const std::vector< KPostItemInfo >& vecPostItemInfo, OUT std::map< int, int >& mapRewardInfo ) const;
	void GetRewardInfo( IN int iRewardType, IN const std::vector< KPostItemInfo >& vecPostItemInfo, OUT std::vector< std::wstring >& vecMessage ) const;
#ifdef SERV_EVENT_DB_CONTROL_SYSTEM
	void SetRewardDBData(IN std::map< int, std::vector< KRewardData > > mapRewardDBdata) { m_mapRewardData = mapRewardDBdata; }
#endif //SERV_EVENT_DB_CONTROL_SYSTEM

private:
	std::map< int, std::vector< KRewardData > > m_mapRewardData;
};