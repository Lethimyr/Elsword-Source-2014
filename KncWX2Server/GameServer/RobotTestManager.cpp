#include ".\RobotTestManager.h"

#include "BaseServer.h"
#include <KncLua.h>
#include <lua_tinker.h>
#include "lua/KLuaManager.h"
#include "X2Data/XSLItemManager.h"

UidType KRobotTestManager::ms_dummpyItemUID = 2000000000;

ImplementSingleton( KRobotTestManager );

KRobotTestManager::KRobotTestManager(void)
:
m_iPatternIndex(-1),
m_bIsRobotPet(true)
{
	lua_tinker::class_add<KRobotTestManager>( g_pLua, "KRobotTestManager" );
	lua_tinker::class_def<KRobotTestManager>( g_pLua, "open",		&KRobotTestManager::FileOpen );
	lua_tinker::class_def<KRobotTestManager>( g_pLua, "close",		&KRobotTestManager::FileClose );
	lua_tinker::class_def<KRobotTestManager>( g_pLua, "RobotPet",	&KRobotTestManager::RobotPet_LUA );

	lua_tinker::decl( g_pLua, "RobotTest", this );
}

KRobotTestManager::~KRobotTestManager(void)
{
	FileClose();
}

void KRobotTestManager::FileOpen( int iPatternIndex )
{
	if( m_wfsRobotScript.is_open() )
	{
		START_LOG( cerr, L"이미 파일이 오픈되어 있습니다!" )
			<< END_LOG;
		return;
	}

	::CreateDirectoryA( "RobotPattern", NULL );

	CStringA cstrTemp;
	cstrTemp.Format( "%d", iPatternIndex );	

	std::string strFilename;
	strFilename  = "RobotPattern\\RobotActionScript_";
	strFilename += cstrTemp.GetBuffer();
	strFilename += ".lua";

	m_wfsRobotScript.imbue(std::locale("korean", LC_CTYPE));
	m_wfsRobotScript.open( strFilename.c_str(), std::ios::out | std::ios::app );

	m_wfsRobotScript.flush();

	m_iPatternIndex = iPatternIndex;

	START_LOG( cout, L"로봇 패턴 스크립트 파일 오픈 성공!" )
		<< BUILD_LOG( strFilename );
}

void KRobotTestManager::FileClose()
{
	if( !m_wfsRobotScript.is_open() )
		return;

	m_wfsRobotScript.close();

	START_LOG( cout, L"로봇 패턴 스크립트 파일 닫기 성공!" );
}

void KRobotTestManager::RobotPet_LUA( bool bVal )
{
    m_bIsRobotPet = bVal;

	START_LOG( cout, L"로봇 펫 적용 여부 저장!" )
		<< BUILD_LOG( m_bIsRobotPet );
}

void KRobotTestManager::PrintRobotScript( const KEGS_FIELD_UNIT_SYNC_DATA_NOT& kEvent, int iMapID )
{
	if( !m_wfsRobotScript.is_open() )
		return;

	std::wfstream& wfs = m_wfsRobotScript;

	// STAGE_LIST [BEGIN]	
	wfs << std::endl << std::endl;
	wfs << "RobotManager:AddRobotPattern" << std::endl;
	wfs << "{" << std::endl;

	//////////////////////////////////////////////////////////////////////////

	wfs << "	m_iMapID = " << iMapID << "," << std::endl;
	wfs << "	m_iPatternIndex = " << m_iPatternIndex << "," << std::endl << std::endl;

	//////////////////////////////////////////////////////////////////////////

	wfs << "	m_bIsRight = " << kEvent.m_bIsRight << "," << std::endl;
	wfs << "	m_vPos_X = " << kEvent.m_vPos.x << "," << std::endl;
	wfs << "	m_vPos_Y = " << kEvent.m_vPos.y << "," << std::endl;
	wfs << "	m_vPos_Z = " << kEvent.m_vPos.z << "," << std::endl << std::endl;
	
	wfs << "	FrameTable = " << std::endl;
	wfs << "	{" << std::endl;

	std::vector< KFieldFrameInfo >::const_iterator vit;
	for( vit = kEvent.m_vecFieldFrame.begin(); vit != kEvent.m_vecFieldFrame.end(); ++vit )
	{
		wfs << "		{" << std::endl;
		wfs << "			m_FrameCount = " << vit->m_FrameCount << "," << std::endl;
		wfs << "			m_StateID = " << vit->m_StateID << "," << std::endl;
		wfs << "		}," << std::endl;
	}	

	wfs << "	}," << std::endl;

	//////////////////////////////////////////////////////////////////////////

	// STAGE_LIST [END]
	wfs << "}";

	wfs.flush();
}

bool KRobotTestManager::InitRobotEquipItemList()
{
	const std::map< int, CXSLItem::ItemTemplet >& mapItemTemplet = SiCXSLItemManager()->GetItemTempletContainer();
	std::map< int, CXSLItem::ItemTemplet >::const_iterator mit = mapItemTemplet.begin();

	for( ; mit != mapItemTemplet.end(); ++mit )
	{
		const CXSLItem::ItemTemplet& kItemTemplet = mit->second;

		int iSlotID = GetEquipSlotID( kItemTemplet.m_EqipPosition, kItemTemplet.m_bFashion );
		if( iSlotID < 0 )
			continue;

		int iUnitType = 0;
		if( iSlotID < CXSLUnit::NESI_AC_FACE1 )
		{
			iUnitType = kItemTemplet.m_UnitType;
		}

		std::map< int, KRandomItemGroup >::iterator mit;
		mit = m_mapRandomEquip[iSlotID].find( iUnitType );
		if( mit == m_mapRandomEquip[iSlotID].end() )
		{
			KRandomItemGroup kRandomGroup;
			kRandomGroup.push_back( kItemTemplet.m_ItemID );
			m_mapRandomEquip[iSlotID].insert( std::make_pair( iUnitType, kRandomGroup ) );
		}
		else
		{
			mit->second.push_back( kItemTemplet.m_ItemID );
		}
	}

	return true;
}

bool KRobotTestManager::IsRobot( const std::wstring& wstrNickName )
{
	std::wstring wstrRobotCheck;
	wstrRobotCheck.assign( wstrNickName.c_str(), 5 );

	return ( wstrRobotCheck == L"RTest" );
}

void KRobotTestManager::RobotFullEquip( const std::wstring& wstrNickName, int iUnitType, std::vector< KInventoryItemSimpleInfo >& vecItem )
{
	if( !IsRobot( wstrNickName ) )
		return;

	std::set< int > setEquipSlotID;

	BOOST_TEST_FOREACH( const KInventoryItemSimpleInfo&, kInfo, vecItem )
	{
		if( kInfo.m_cSlotCategory != CXSLInventory::ST_E_EQUIP )
			continue;

		//{{ 2011. 12. 15	최육사	인벤토리 슬롯ID크기 늘리기
#ifdef SERV_EXPAND_SLOT_ID_DATA_SIZE
		setEquipSlotID.insert( kInfo.m_sSlotID );
#else
		setEquipSlotID.insert( kInfo.m_cSlotID );
#endif SERV_EXPAND_SLOT_ID_DATA_SIZE
		//}}
	}

	for( int idx = CXSLUnit::NESI_DEFENCE_HAIR_FASHION; idx < CXSLUnit::NESI_END; ++idx )
	{
        if( setEquipSlotID.find( idx ) != setEquipSlotID.end() )
			continue;

		// 해당 슬롯에 들어갈만한 아이템을 랜덤으로 얻어오자!
		if( idx >= CXSLUnit::NESI_AC_FACE1 )
		{
            iUnitType = 0;
		}

		int iItemID = GetRandomEquipItem( idx, iUnitType );
		const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iItemID );
		if( pItemTemplet == NULL )
		{
			START_LOG( cerr, L"아이템이 없을리가 없는데?" )
				<< BUILD_LOG( iItemID )
				<< END_LOG;
			continue;
		}
		
		KInventoryItemSimpleInfo kEquipItem;
		kEquipItem.m_iItemUID = ms_dummpyItemUID++;
		kEquipItem.m_cSlotCategory = CXSLInventory::ST_E_EQUIP;
		//{{ 2011. 12. 15	최육사	인벤토리 슬롯ID크기 늘리기
#ifdef SERV_EXPAND_SLOT_ID_DATA_SIZE
		kEquipItem.m_sSlotID = idx;
#else
		kEquipItem.m_cSlotID = idx;
#endif SERV_EXPAND_SLOT_ID_DATA_SIZE
		//}}
		kEquipItem.m_iItemID = iItemID;
		if( !pItemTemplet->m_bFashion  &&  idx < CXSLUnit::NESI_AC_FACE1 )
		{
			kEquipItem.m_EnchantLevel = rand() % 21;
		}
		else
		{
			kEquipItem.m_EnchantLevel = 0;
		}

		vecItem.push_back( kEquipItem );
	}
}

#ifdef SERV_PET_SYSTEM

void KRobotTestManager::RobotPet( const std::wstring& wstrNickName, UidType& iSummonedPetUID, std::vector< KPetInfo >& vecPet )
{
	if( !IsRobot( wstrNickName ) )
		return;

	if( m_bIsRobotPet == false )
		return;

    KPetInfo kRobotPet;
	kRobotPet.m_iPetUID = rand() % 100000000;
#ifdef SERV_PETID_DATA_TYPE_CHANGE //2013.07.02
	kRobotPet.m_iPetID = ( rand() % 4 ) + 1;
#else //SERV_PETID_DATA_TYPE_CHANGE
	kRobotPet.m_cPetID = ( rand() % 4 ) + 1;
#endif //SERV_PETID_DATA_TYPE_CHANGE
	kRobotPet.m_wstrPetName = L"로봇펫";
	kRobotPet.m_cEvolutionStep = rand() % 4;
	vecPet.push_back( kRobotPet );

	iSummonedPetUID = kRobotPet.m_iPetUID;
}

#endif SERV_PET_SYSTEM

int KRobotTestManager::GetEquipSlotID( int iEquipPosition, bool bFashion )
{
	if( bFashion )
	{
		switch( iEquipPosition )
		{
		case CXSLUnit::EP_DEFENCE_HAIR: return CXSLUnit::NESI_DEFENCE_HAIR_FASHION;
		case CXSLUnit::EP_DEFENCE_BODY: return CXSLUnit::NESI_DEFENCE_BODY_FASHION;
		case CXSLUnit::EP_DEFENCE_LEG:	return CXSLUnit::NESI_DEFENCE_LEG_FASHION;
		case CXSLUnit::EP_DEFENCE_HAND: return CXSLUnit::NESI_DEFENCE_HAND_FASHION;
		case CXSLUnit::EP_DEFENCE_FOOT: return CXSLUnit::NESI_DEFENCE_FOOT_FASHION;
		case CXSLUnit::EP_WEAPON_HAND:	return CXSLUnit::NESI_WEAPON_HAND_FASHION;		
		}
	}
	else
	{
		switch( iEquipPosition )
		{
		case CXSLUnit::EP_DEFENCE_BODY: return CXSLUnit::NESI_DEFENCE_BODY;
		case CXSLUnit::EP_DEFENCE_LEG:	return CXSLUnit::NESI_DEFENCE_LEG;
		case CXSLUnit::EP_DEFENCE_HAND: return CXSLUnit::NESI_DEFENCE_HAND;
		case CXSLUnit::EP_DEFENCE_FOOT: return CXSLUnit::NESI_DEFENCE_FOOT;
		case CXSLUnit::EP_WEAPON_HAND:	return CXSLUnit::NESI_WEAPON_HAND;
		case CXSLUnit::EP_AC_FACE1:		return CXSLUnit::NESI_AC_FACE1;
		case CXSLUnit::EP_AC_FACE2:		return CXSLUnit::NESI_AC_FACE2;
		case CXSLUnit::EP_AC_FACE3:		return CXSLUnit::NESI_AC_FACE3;
		case CXSLUnit::EP_AC_BODY:		return CXSLUnit::NESI_AC_BODY;
		case CXSLUnit::EP_AC_ARM:		return CXSLUnit::NESI_AC_ARM;
		case CXSLUnit::EP_AC_LEG:		return CXSLUnit::NESI_AC_LEG;
		case CXSLUnit::EP_AC_RING:		return CXSLUnit::NESI_AC_RING;
		case CXSLUnit::EP_AC_NECKLESS:	return CXSLUnit::NESI_AC_NECKLESS;
		case CXSLUnit::EP_AC_WEAPON:	return CXSLUnit::NESI_AC_WEAPON;
		}
	}

	// 장착 아이템이 아니다!
	return -1;
}

int KRobotTestManager::GetRandomEquipItem( int iSlotID, int iUnitType )
{
	std::map< int, KRandomItemGroup >::const_iterator mit;
	mit = m_mapRandomEquip[iSlotID].find( iUnitType );
	if( mit == m_mapRandomEquip[iSlotID].end() )
	{
		START_LOG( cerr, L"어라? 랜덤장착그룹이 없네?" )
			<< BUILD_LOG( iSlotID )
			<< BUILD_LOG( iUnitType )
			<< END_LOG;
		return -1;
	}

	int iIndex = rand() % mit->second.size();
	return mit->second[iIndex];
}


