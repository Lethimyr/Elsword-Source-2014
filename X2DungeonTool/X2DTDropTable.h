#pragma once

#include <ToString.h>
#include <KNCSingleton.h>
#include "Lottery.h"

class KDropTable
{
public:
    struct DROP_DATA
    {
        int		m_iItemID;
        int		m_iExp;
        int		m_iGPItemID;
        int		m_iED;		//GP 량
        int		m_nGPNum;	//GP 개수
    };

protected:

    enum EDropCaseParam {   // head - DCP. 단품 아이템 or 아이템 그룹
        DCP_UNARY_ITEM = 0, // 단품 아이템
        DCP_ITEM_GROUP = 1, // 아이템 그룹
    };


	struct KMonsterDropInfo
	{
        int				m_nDungeonID;
		int				m_nMonsterID;
		bool			m_bKillCheck;
		int				m_iExp;
        int		        m_iED;
        float	        m_fEDProperty;

        KLottery        m_kLottery;
	};
	

public:
	KDropTable(void);
	~KDropTable(void);

	DeclToStringW;
	DeclDump;

	bool LoadFromLua( const char* szFileName );
	bool DropItem( IN int nDungeonID, IN int nMonsterID/*, IN int nMonsterLevel*/, OUT DROP_DATA& sDropData ); 

	bool EventDropItem( OUT DROP_DATA& sDropData );

public: // for lua
    void AddToGroup( int nGroupID, int nItemID, float fProbability );
    void AddMonsterDropInfo();  // table을 받는다.

    // load된 데이터 확인용 함수.
    void DumpGroup();
    void DumpMonsterDrop();

protected:	
    int GetEDItemID( int nED ); // ED양에 맞는 ItemID를 반환한다.

protected:
	std::map<std::pair<int,int>, KMonsterDropInfo>	m_mapMonsterDrop;   //Key(1:DungeonID,2:MonsterID)
    std::map<int,KLottery>                          m_mapDropGroup;

    friend void TestItemDrop(); // for unit testing.
};
