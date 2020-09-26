#include "dxstdafx.h"
#include "X2DTDropTable.h"

#include <dbg/dbg.hpp>

KDropTable::KDropTable()
{	
}

KDropTable::~KDropTable()
{
}

bool KDropTable::LoadFromLua( const char* szFileName )
{
	lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(), "DropTable", this );

	//if( 0 == luaL_dofile( g_pKTDXApp->GetLuaBinder()->GetLuaState(), szFileName ) )
	//	return true;

	//파일 로드
	KGCMassFileManager::CMassFile::MASSFILE_MEMBERFILEINFO_POINTER Info;
	Info = g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->LoadDataFile( szFileName );
	if( Info == NULL )
	{		
		ErrorLogMsg( XEM_ERROR0, szFileName );

		return false;
	}

	if( g_pKTDXApp->GetLuaBinder()->DoMemory( Info->pRealData, Info->size ) == E_FAIL )
	{
		ErrorLogMsg( XEM_ERROR72, szFileName );

		return false;
	}

	return true;
}

void KDropTable::DumpGroup()
{
    std::wostringstream stm;

    stm << L"  -- Item Group -- " << std::endl;
    std::map<int,KLottery>::const_iterator mit;
    for( mit = m_mapDropGroup.begin(); mit != m_mapDropGroup.end(); mit++ )
    {
        stm << L"  Group ID : " << mit->first << std::endl;
        mit->second.ToString( stm );
    }

    dbg::cout << stm.str();
}

void KDropTable::DumpMonsterDrop()
{
    std::wostringstream stm;

    stm << std::endl << L"  -- Monster Drop Info -- " << std::endl;
    std::map<std::pair<int,int>,KMonsterDropInfo>::const_iterator mit;
    for( mit = m_mapMonsterDrop.begin(); mit != m_mapMonsterDrop.end(); mit++ )
    {
        stm << L"  Dungeon,Monster key : " << mit->first.first << L", " << mit->first.second << std::endl
            << TOSTRINGW( mit->second.m_iExp )
            << TOSTRINGW( mit->second.m_iED )
            << TOSTRINGW( mit->second.m_fEDProperty );

        mit->second.m_kLottery.ToString( stm );
    }

    dbg::cout << stm.str();
}

ImplToStringW( KDropTable )
{
	return START_TOSTRING2W( KDropTable )
        << L"  Use DumpGroup(), DumpMonsterDrop()" << std::endl
        << TOSTRINGW( m_mapDropGroup.size() )
        << TOSTRINGW( m_mapMonsterDrop.size() );
}

void KDropTable::AddToGroup( int nGroupID, int nItemID, float fProbability )
{
    std::map<int,KLottery>::iterator mit;
    mit = m_mapDropGroup.find( nGroupID );

    if( mit == m_mapDropGroup.end() )  // 기존에 등록된 동일 그룹 데이터가 없음 - 새로 추가.
    {
        m_mapDropGroup.insert( std::make_pair( nGroupID, KLottery() ) );
        mit = m_mapDropGroup.find( nGroupID );
    }

    LIF( mit->second.AddCase( nItemID, fProbability ) );
}

void KDropTable::AddMonsterDropInfo()
{
    KLuaManager luaMgr( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
	TableBind( &luaMgr, g_pKTDXApp->GetLuaBinder() );

    KMonsterDropInfo kInfo;
    LUA_GET_VALUE( luaMgr, "DungeonID",     kInfo.m_nDungeonID, -1 );
    LUA_GET_VALUE( luaMgr, "MonsterID",     kInfo.m_nMonsterID, -1 );
    LUA_GET_VALUE( luaMgr, "Exp",           kInfo.m_iExp, -1 );
    LUA_GET_VALUE( luaMgr, "ED",            kInfo.m_iED, -1 );
    LUA_GET_VALUE( luaMgr, "EDProperty",    kInfo.m_fEDProperty, 0.0f );

    if( kInfo.m_nDungeonID == -1 && kInfo.m_nMonsterID == -1 )
    {
        START_LOG( cerr, L"Invalid key" )
            << BUILD_LOG( kInfo.m_nDungeonID )
            << BUILD_LOG( kInfo.m_nMonsterID );
        return;
    }

	//이벤트 테이블 중복 검사.
	if( kInfo.m_nDungeonID == 0 && kInfo.m_nMonsterID == 0 )
	{
		std::map<std::pair<int,int>,KMonsterDropInfo>::iterator mit;
		mit = m_mapMonsterDrop.find( std::make_pair( 0, 0 ) );

		if( mit != m_mapMonsterDrop.end() )
		{
			START_LOG( cerr, L"이벤트 아이디가 중복으로 들어옴." )
				<< END_LOG;

			return;
		}
	}

    // 단품 아이템 리스트와 아이템 그룹 리스트 로딩 처리가 매우 유사하게 되어있지만,
    // 스크립트에서의 magic number 난무 방지를 위해 이같이 처리하였다.

    if( luaMgr.BeginTable( "DropItemList" ) == true )   // 단품 아이템 리스트 확률 설정
    {
        int nTableIndex = 1;
        while( luaMgr.BeginTable( nTableIndex ) == true )
        {
            int nItemID         = -1;
            float fProperty     = 0.0f;

            LUA_GET_VALUE( luaMgr, 1, nItemID, -1 );
            LUA_GET_VALUE( luaMgr, 2, fProperty, 0.0f );

            if( kInfo.m_kLottery.AddCase( nItemID, fProperty, DCP_UNARY_ITEM ) == false )
			{
				START_LOG( cerr, "Drop Item List" )
					<< BUILD_LOG( kInfo.m_nDungeonID )
					<< BUILD_LOG( kInfo.m_nMonsterID );
			}

            luaMgr.EndTable();

            nTableIndex++;
        } // end while

        luaMgr.EndTable();
    }

    if( luaMgr.BeginTable( "DropGroupList" ) == true ) // 아이템 그룹 리스트 확률 설정
    {
        int nTableIndex = 1;
        while( luaMgr.BeginTable( nTableIndex ) == true )
        {
            int nGroupID    = -1;
            float fProperty = 0.0f;

            LUA_GET_VALUE( luaMgr, 1, nGroupID, -1 );
            LUA_GET_VALUE( luaMgr, 2, fProperty, 0.0f );

            if( kInfo.m_kLottery.AddCase( nGroupID, fProperty, DCP_ITEM_GROUP ) == false )
			{
				START_LOG( cerr, "Drop Group List" )
					<< BUILD_LOG( kInfo.m_nDungeonID )
					<< BUILD_LOG( kInfo.m_nMonsterID );					
			}

            luaMgr.EndTable();

            nTableIndex++;
        } // end while

        luaMgr.EndTable();
    }

    // operator[]의 의도적인 사용 - 기존에 있던 동일 데이터를 덮어써 버린다.
    m_mapMonsterDrop[ std::make_pair( kInfo.m_nDungeonID, kInfo.m_nMonsterID ) ] = kInfo;

}

int KDropTable::GetEDItemID( int nED )
{
    // 추후 이 함수만 동적 수정이 용이하도록 script 화 한다.

    if( nED <= 0 ) return 0;

    if( nED < 60 ) return 127000;   // 소량의 ED

    return 127010;                  // ED 많이 받을 때.
}

bool KDropTable::DropItem( IN int nDungeonID, IN int nMonsterID/*, IN int nMonsterLevel*/, OUT DROP_DATA& sDropData )
{
    sDropData.m_iItemID     = -1;
    sDropData.m_iExp        = -1;
    sDropData.m_iGPItemID   = -1;
    sDropData.m_iED         = -1;
    sDropData.m_nGPNum      = -1;

    // 우선 해당 DungeonID + MonsterID에 해당하는 설정을 검색한다.
    std::map<std::pair<int,int>,KMonsterDropInfo>::iterator mit;
    mit = m_mapMonsterDrop.find( std::make_pair( nDungeonID, nMonsterID ) );
    if( mit == m_mapMonsterDrop.end() )    // 해당 경우에 대한 설정이 존재하지 않음.
    {
        // 던전 아이디가 정해져 있지 않은 설정이 존재하는지 다시 한번 확인.
        mit = m_mapMonsterDrop.find( std::make_pair( 0, nMonsterID ) );

        if( mit == m_mapMonsterDrop.end() ) // 그것마저 없다면?
            return true;    // 성공적인 종료. 에러일때만 false.
    }

    KMonsterDropInfo& kDropInfo = mit->second; // for easy typing.

    // 경험치와 ED를 우선 처리하자.
    sDropData.m_iExp        = kDropInfo.m_iExp;// + ((nMonsterLevel/2)*10);
    if( (float)rand() / RAND_MAX < kDropInfo.m_fEDProperty )
    {
        sDropData.m_iED         = kDropInfo.m_iED;
        sDropData.m_iGPItemID   = GetEDItemID( kDropInfo.m_iED );
        sDropData.m_nGPNum      = rand() % 3 + 1;    // [1,3]
    }

    // 다음은 아이템.
    int nCaseID = kDropInfo.m_kLottery.Decision();
    if( nCaseID == KLottery::CASE_BLANK )   // 획득한 아이템 없음.
        return true;

    int nCaseParam = kDropInfo.m_kLottery.GetParam( nCaseID );

    switch( nCaseParam )    // 단품 아이템, 아이템 그룹, 혹은 예상 못한 에러.
    {
    case DCP_UNARY_ITEM:    // 단품 아이템
        sDropData.m_iItemID = nCaseID;
        break;
    case DCP_ITEM_GROUP:    // 아이템 그룹.
        {
            std::map<int,KLottery>::iterator mit;
            mit = m_mapDropGroup.find( nCaseID );
            if( mit == m_mapDropGroup.end() )
            {
                START_LOG( cerr, L"아이템 그룹 정보 오류" )
                    << BUILD_LOG( nCaseID )
                    << BUILD_LOG( nCaseParam )
                    << BUILD_LOG( m_mapDropGroup.size() );
                return false;
            }

            // 아이템 그룹 중에서 실제 아이템 하나 선택.
            sDropData.m_iItemID = mit->second.Decision();
        }
        break;
    default:
        START_LOG( cerr, L"Never Get Here ~" )
            << BUILD_LOG( nCaseParam )
            << BUILD_LOG( nCaseID )
            << BUILD_LOG( kDropInfo.m_kLottery.GetCaseNum() )
            << BUILD_LOG( kDropInfo.m_kLottery.GetTotalProb() )
            << BUILD_LOG( nDungeonID )
            << BUILD_LOG( nMonsterID )
            //<< BUILD_LOG( nMonsterLevel )
			;
        return false;
    }

    return true;
}

bool KDropTable::EventDropItem( OUT DROP_DATA& sDropData )
{
	return DropItem( 0, 0, sDropData );
}