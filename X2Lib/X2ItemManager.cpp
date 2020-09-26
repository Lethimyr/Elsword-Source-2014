#include "StdAfx.h"
#include ".\x2itemmanager.h"
#include ".\X2LocationManager.h"

CX2ItemManager::CX2ItemManager(void)
{
	//{{ 2013.01.26 최민철 큐브패키지아이템 미리보기기능
#ifdef PACKAGE_IN_QUBE_PREVIEW
	m_iLastPackacgeID = 0;
#endif PACKAGE_IN_QUBE_PREVIEW
	//}} 2013.01.26 최민철 큐브패키지아이템 미리보기기능

#ifdef IN_ITEM_KIM_USE_HIDE_SET_DESC
	m_mapHideSetDescItem.clear();
#endif //IN_ITEM_KIM_USE_HIDE_SET_DESC
}

CX2ItemManager::~CX2ItemManager(void)
{
#ifdef  X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING

    CX2Item::KItemFormatTemplet::_ResetData();
    m_vecItemTempletInfo.resize( 0 );

#else   //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING

	const CX2Item::ItemTemplet* pItem;

	map<int,const CX2Item::ItemTemplet*>::iterator iter;
	iter = m_ItemTempletIDMap.begin();
	for( iter = m_ItemTempletIDMap.begin(); iter != m_ItemTempletIDMap.end(); ++iter )
	{
		pItem = iter->second;
		SAFE_DELETE( pItem );
	}
	m_ItemTempletIDMap.clear();

#endif  //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING



	map<int, ItemIDList*>::iterator it;
	for( it=m_ShopItemList.begin(); it!=m_ShopItemList.end(); it++ )
	{
		ItemIDList* pItemIDList = it->second;
		SAFE_DELETE( pItemIDList );
	}

	for( it=m_ShopItemGroup.begin(); it!=m_ShopItemGroup.end(); it++ )
	{
		ItemIDList* pItemIDList = it->second;
		SAFE_DELETE( pItemIDList );
	}

	map< std::pair<int,int>, NpcItemIDList*>::iterator nit;
	for( nit=m_ShopNpcItemList.begin(); nit!=m_ShopNpcItemList.end(); nit++ )
	{
		NpcItemIDList* pItemIDList = nit->second;
		SAFE_DELETE( pItemIDList );
	}		

	//{{ kimhc // 2009-12-22 // 랜덤 아이템에 같은 키값 쓸수있도록 추가
#ifdef	MULTI_KEY_RANDOM_ITEM
	std::multimap<int, RandomItemData* >::iterator mit;
	mit = m_multimapRandomItem.begin();
	for( mit = m_multimapRandomItem.begin(); mit != m_multimapRandomItem.end(); mit++ )
	{		
		SAFE_DELETE( mit->second );
	}
	m_multimapRandomItem.clear();
#else	MULTI_KEY_RANDOM_ITEM
	std::map<int, RandomItemData* >::iterator mit;
	mit = m_mapRandomItem.begin();
	for( mit = m_mapRandomItem.begin(); mit != m_mapRandomItem.end(); mit++ )
	{		
		SAFE_DELETE( mit->second );
	}
	m_mapRandomItem.clear();
#endif	MULTI_KEY_RANDOM_ITEM
	//}} kimhc // 2009-12-22 // 랜덤 아이템에 같은 키값 쓸수있도록 추가

	std::map< int, ManufactureData* >::iterator mit2;

	mit2 = m_mapManufacture.begin();
	for( mit2 = m_mapManufacture.begin(); mit2 != m_mapManufacture.end(); mit2++ )
	{		
		SAFE_DELETE( mit2->second );
	}
	m_mapManufacture.clear();

	ClearCashItemList();

#ifndef X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
	std::map< int, SetItemData* >::iterator mit3;
	mit3 = m_mapSetItemData.begin();
	for( mit3 = m_mapSetItemData.begin(); mit3 != m_mapSetItemData.end(); mit3++ )
	{		
		SAFE_DELETE( mit3->second );
	}
	m_mapSetItemData.clear();
#endif  //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
}

#ifdef ADD_SERVER_GROUP
void CX2ItemManager::ClearItemExchangeData()
{
	m_mapItemExchangeData.clear();
}
void CX2ItemManager::ClearManufactureData()
{
	std::map< int, ManufactureData* >::iterator mit2;

	mit2 = m_mapManufacture.begin();
	for( mit2 = m_mapManufacture.begin(); mit2 != m_mapManufacture.end(); mit2++ )
	{		
		SAFE_DELETE( mit2->second );
	}
	m_mapManufacture.clear();
}
void CX2ItemManager::ClearManufactureVillageData()
{
	m_mapManufactureVillage.clear();
}
void CX2ItemManager::ClearManufactureResultGroupData()
{
	m_mapManufactureResultGroupData.clear();
}
#endif

//{{ kimhc // 2010.12.9 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
/** @function : CreateDummyEquip
	@brief : 전달되는 정보에 따라 장비를 생성하여 리턴함(UID는 Invalid 함)
	@param : int iItemID_, CX2Unit* pUnit_, CKTDGXSkinAnimPtr pXSkinAnim_
	@return : CX2EqipPtr
*/

/*static*/ CX2EqipPtr CX2ItemManager::CreateDummyEquip( int iItemID_, CX2Unit* pUnit_, CKTDGXSkinAnimPtr pXSkinAnim_ )
{
    CX2Item* pX2Item = NULL;
	ASSERT( NULL != pUnit_ );

    {
	    CX2Item::ItemData kItemData;
	    kItemData.m_ItemID				= iItemID_;
	    kItemData.m_PeriodType			= CX2Item::PT_INFINITY;
	    kItemData.m_Endurance			= 0;
	    pX2Item				= new CX2Item( kItemData, pUnit_ );
        ASSERT( pX2Item != NULL );
    }
    int iEnchantLevel = pX2Item->GetItemData().m_EnchantLevel;
#ifdef COMMON_ITEM_TEMPLET_TEST
	if( pX2Item->GetItemTemplet()->GetUseCondition() == CX2Item::UC_ANYONE )
	{
		return CX2Eqip::CreateEquip( pX2Item, pXSkinAnim_, iEnchantLevel, true, true, pUnit_->GetType() );
	}
	else
#endif COMMON_ITEM_TEMPLET_TEST
	{
		return CX2Eqip::CreateEquip( pX2Item, pXSkinAnim_, iEnchantLevel, true, true );
	}
}

#endif	NEW_CHARACTER_CHUNG
//}} kimhc // 2010.12.9 //  2010-12-23 New Character CHUNG

bool CX2ItemManager::OpenScriptFile( const WCHAR* pFileName )
{
	lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(),  "g_pItemManager", this );
	lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(),  "g_pManufactureItemManager", this );
	lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(),  "g_pCashItemManager", this );
	lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(),  "g_pCX2SetItemManager", this );
	//{{ 2013.01.26 최민철 큐브패키지아이템 미리보기기능
#ifdef PACKAGE_IN_QUBE_PREVIEW
	lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(),  "g_pCX2CubePackageManager", this );
#endif PACKAGE_IN_QUBE_PREVIEW
	//}}
    if ( g_pKTDXApp->LoadLuaTinker( pFileName ) == false )
    {
		ErrorLogMsg( XEM_ERROR9, pFileName );
		return false;
    }

	return true;
}

#ifdef	X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING

bool    CX2ItemManager::DoMemoryNotEncrypt( const char* pBuffer, long nSize )
{
	if( pBuffer != NULL && nSize >= sizeof(CX2Item::KItemFormatHeader) )
    {
        CX2Item::KItemFormatHeader kHeader = *( (const CX2Item::KItemFormatHeader*) ( pBuffer ) );
        if ( kHeader.m_dwMagic == KX2ITEMMANAGER_FORMAT_MAGIC
            && kHeader.m_dwVersion == KX2ITEMMANAGER_FORMAT_VERSION )
        {
            DWORD dwNumItem = kHeader.m_dwNumItems;
            if ( nSize >= (int) ( sizeof(CX2Item::KItemFormatHeader) + dwNumItem * ( sizeof(DWORD) + sizeof(CX2Item::KItemFormatTemplet) ) ) )
            {
                m_vecItemTempletInfo.resize( 0 );
                const BYTE* pData = (const BYTE*) pBuffer;
                m_vecItemTempletInfo.assign( &pData[0], &pData[nSize] );
                CX2Item::KItemFormatTemplet::_SetData( &m_vecItemTempletInfo.front() );
                return true;
            }
        }
    }
    return false;
}

bool	CX2ItemManager::OpenItemScriptFile( const WCHAR* pKimFile, unsigned uNumScriptFiles, const WCHAR* apScriptFileName[] )
{

    bool bExistRealScript = false;
#ifndef MASS_FILE_FIRST
    if ( uNumScriptFiles != 0 && apScriptFileName != NULL )
    {
        for( unsigned u = 0; u != uNumScriptFiles; ++u )
        {
            if ( apScriptFileName[u] == NULL || apScriptFileName[u][0] == NULL )
            {
                bExistRealScript = false;
                break;
            }
            else if ( g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->IsValidFile_LocalFile( apScriptFileName[u] ) == true )
            {
                bExistRealScript = true;
            }
            else if ( g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->IsValidFile( apScriptFileName[u] ) == false )
            {
                bExistRealScript = false;
                break;
            }
        }
    }//if
#endif  MASS_FILE_FIRST

    if ( bExistRealScript == false )
    {
        bool bReturn = g_pKTDXApp->LoadAndDoMemory( this, pKimFile );
        if ( bReturn == true )
            return true;
    }

    if ( uNumScriptFiles == 0 || apScriptFileName == NULL )
        return false;

	{
		/* 오현빈 // X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING주석 // 제가 이해한 내용을 바탕으로 작성 해서 틀린 내용 일 수 있습니다.
		KProxy로 파싱하는 첫 번째 작업은 아이템의 개수 파악을 위한 파싱
		해당 아이템 ID와, 해당 아이템이 유효한지 체크하기 위한 값들을 검사 후 ItemID만 컨테이너에 등록*/

	    KProxy	kProxy;
	    lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(),  "g_pItemManager", &kProxy );
	    lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(),  "g_pCX2SetItemManager", &kProxy );

        for( unsigned u = 0; u != uNumScriptFiles; ++u )
        {
            bool bRet = g_pKTDXApp->LoadLuaTinker( apScriptFileName[u] );
	        if( bRet == false )
	        {
		        ErrorLogMsg( XEM_ERROR9, apScriptFileName[u] );
                return false;
	        }
        }

        {	
			/* 오현빈 // X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING주석 // 제가 이해한 내용을 바탕으로 작성 해서 틀린 내용 일 수 있습니다.
			KFileSerializer 클래스에 스크립트의 정보를 기록함
			순서대로 아이템개수와 세트아이템의 개수를 저장하는 CX2Item::KItemFormatHeader
			sizeof(DWORD) * 아이템개수,  sizeof(CX2Item::KItemFormatTemplet)* 아이템개수, 
			sizeof(DWORD) * 세트아이템개수, sizeof(CX2Item::KItemFormatSetItemData)* 세트아이템개수 */
#ifdef SERV_ITEM_LUA_TRANS_DEVIDE
			KProxy2 kProxy2( kProxy.m_setItemIDs, kProxy.m_setSetIDs, kProxy.m_mapItemTrans, kProxy.m_mapSetItemTrans );
#else //SERV_ITEM_LUA_TRANS_DEVIDE
            KProxy2 kProxy2( kProxy.m_setItemIDs, kProxy.m_setSetIDs );
#endif //SERV_ITEM_LUA_TRANS_DEVIDE

	        lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(),  "g_pItemManager", &kProxy2 );
	        lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(),  "g_pCX2SetItemManager", &kProxy2 );

			/* 오현빈 // X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING주석 // 제가 이해한 내용을 바탕으로 작성 해서 틀린 내용 일 수 있습니다.
			스크립트에 작성된 자료들을 실제로 등록하는 부분 */
            for( unsigned u = 0; u != uNumScriptFiles; ++u )
            {
                bool bRet = g_pKTDXApp->LoadLuaTinker( apScriptFileName[u] );
                if ( bRet == false )
	            {
		            ErrorLogMsg( XEM_ERROR9, apScriptFileName[u] );
                    return false;
	            }
            }

            kProxy2.PostProcess();

            if ( kProxy2.m_kFileSerializer.GetSize() < sizeof(CX2Item::KItemFormatHeader) )
            {
		        string strFileName;
		        ConvertWCHARToChar( strFileName, pKimFile );
		        ErrorLogMsg( XEM_ERROR9, strFileName.c_str() );
                return false;
            }
            //kProxy2.m_kFileSerializer.SaveFile( pKimFile );
            kProxy2.m_kFileSerializer.Swap( m_vecItemTempletInfo );
        }
        CX2Item::KItemFormatTemplet::_SetData( &m_vecItemTempletInfo.front() );

	    lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(),  "g_pItemManager", this );
	    lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(),  "g_pCX2SetItemManager", this );
    }

    return true;

}
#endif	X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING





#ifndef X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING

const CX2Item::ItemTemplet* CX2ItemManager::GetItemTemplet( const int itemID )
{
	map<int,const CX2Item::ItemTemplet*>::iterator iter;
	iter = m_ItemTempletIDMap.find( itemID );
	if( iter == m_ItemTempletIDMap.end() )
		return NULL;

	return iter->second;
	
}

int	CX2ItemManager::SerchByUnit( vector<const CX2Item::ItemTemplet*>& itemList, CX2Unit::UNIT_CLASS unitClass )
{
	int addCount = 0;
	const CX2Item::ItemTemplet* pItem;
	map<int,const CX2Item::ItemTemplet*>::iterator iter1;
	for( iter1 = m_ItemTempletIDMap.begin(); iter1 != m_ItemTempletIDMap.end(); iter1++ )
	{
		pItem = iter1->second;
		if( GetX2UnitClassCompatibility( unitClass, pItem->m_UnitClass ) == true )
		{
			itemList.push_back( pItem );
			addCount++;
		}
	}
	return addCount;
}

bool CX2ItemManager::AddItemTemplet_LUA()
{
	CX2Item::ItemTemplet* pItemTemplet = new CX2Item::ItemTemplet();



	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
#ifndef X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
	TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING

	LUA_GET_VALUE_ENUM(	luaManager, "m_UseCondition",		pItemTemplet->m_UseCondition,			CX2Item::USE_CONDITION,		CX2Item::UC_ANYONE );
	if( CX2Item::UC_NONE == pItemTemplet->m_UseCondition )
	{
		// 사용하지 못하는 아이템이다
		SAFE_DELETE( pItemTemplet );
		return false;
	}

#ifdef SERV_PVP_NEW_SYSTEM
	LUA_GET_VALUE_ENUM(	luaManager, "m_BuyPvpRankCondition",	pItemTemplet->m_BuyPvpRankCondition,	CX2PVPEmblem::PVP_RANK,	CX2PVPEmblem::PVPRANK_NONE );
#endif

	LUA_GET_VALUE_ENUM(	luaManager, "m_UseType",			pItemTemplet->m_UseType,				CX2Item::USE_TYPE,			CX2Item::UT_NONE );
	LUA_GET_VALUE(		luaManager, "m_UseLevel",			pItemTemplet->m_UseLevel,				0 );
	LUA_GET_VALUE_ENUM(	luaManager, "m_UnitType",			pItemTemplet->m_UnitType,				CX2Unit::UNIT_TYPE,			CX2Unit::UT_NONE );
	LUA_GET_VALUE_ENUM(	luaManager, "m_UnitClass",			pItemTemplet->m_UnitClass,				CX2Unit::UNIT_CLASS,		CX2Unit::UC_NONE );



	LUA_GET_VALUE_RETURN(	luaManager, "m_ItemID",						pItemTemplet->m_ItemID,					0,		SAFE_DELETE(pItemTemplet); return false; );
	
	//09. 05. 14 김정협 머지를 위해서 인덱스 읽는 부분 막음
	LUA_GET_VALUE_RETURN(	luaManager, "m_Name",						pItemTemplet->m_Name,					L"",	SAFE_DELETE(pItemTemplet); return false; );
	//LUA_GET_VALUE_RETURN(	luaManager, "m_iName_Index",			m_nString_Index,		0,		SAFE_DELETE(pItemTemplet); return false; );
	//pItemTemplet->m_Name = GET_SCRIPT_STRING(m_nString_Index);

	//09. 05. 14 김정협 머지를 위해서 인덱스 읽는 부분 막음
	LUA_GET_VALUE(			luaManager, "m_Description",				pItemTemplet->m_Description,			L"" );
	//LUA_GET_VALUE(	luaManager, "m_iDescription_Index",			m_nString_Index,		0);
	//pItemTemplet->m_Description = GET_SCRIPT_STRING(m_nString_Index);
	
	//09. 05. 14 김정협 머지를 위해서 인덱스 읽는 부분 막음
	LUA_GET_VALUE(			luaManager, "m_DescriptionInShop",			pItemTemplet->m_DescriptionInShop,		L"" );
	//LUA_GET_VALUE(	luaManager, "m_iDescriptionInShop_Index",			m_nString_Index,		0);
	//pItemTemplet->m_DescriptionInShop = GET_SCRIPT_STRING(m_nString_Index);
	
	LUA_GET_VALUE( luaManager, "m_ModelName",							pItemTemplet->m_ModelName[0],			L"" );
	LUA_GET_VALUE( luaManager, "m_ModelName1",							pItemTemplet->m_ModelName[1],			L"" );

	LUA_GET_VALUE( luaManager, "m_TextureChangeXETName",				pItemTemplet->m_TextureChangeXETName,	L"" );
	LUA_GET_VALUE( luaManager, "m_AniXETName",							pItemTemplet->m_AniXETName,				L"" );
	LUA_GET_VALUE( luaManager, "m_AniName",								pItemTemplet->m_AniName,				L"" );	
	LUA_GET_VALUE( luaManager, "m_ShopImage",							pItemTemplet->m_ShopImage,				L"" );
	LUA_GET_VALUE( luaManager, "m_DropViewer",							pItemTemplet->m_DropViewer,				L"" );
	
	LUA_GET_VALUE_RETURN_ENUM(	luaManager, "m_ItemType",				pItemTemplet->m_ItemType,				CX2Item::ITEM_TYPE,		CX2Item::IT_NONE, SAFE_DELETE(pItemTemplet); return false; );
	LUA_GET_VALUE_ENUM(			luaManager, "m_ItemGrade",				pItemTemplet->m_ItemGrade,				CX2Item::ITEM_GRADE,	CX2Item::IG_NORMAL );
	LUA_GET_VALUE(				luaManager, "m_bFashion",				pItemTemplet->m_bFashion,				false );
	LUA_GET_VALUE(				luaManager, "m_bVested",				pItemTemplet->m_bVested,				false );	
	LUA_GET_VALUE(				luaManager, "m_bCanEnchant",			pItemTemplet->m_bCanEnchant,			false );
	LUA_GET_VALUE(				luaManager, "m_bCanUseInventory",		pItemTemplet->m_bCanUseInventory,		false );	

	//{{ 허상형 : [2009/7/29] //	착용 불가능한 장비 루아 파싱
	LUA_GET_VALUE(				luaManager, "m_bNoEquip",				pItemTemplet->m_bNoEquip,				false );
	//}} 허상형 : [2009/7/29] //	착용 불가능한 장비 루아 파싱
	//{{ 2010. 01. 05  최육사	PC방 프리미엄
	LUA_GET_VALUE(				luaManager, "m_bPcBang",				pItemTemplet->m_bIsPcBang,				false );
	//}}
	//{{ 2011. 10. 14	최육사	배틀필드 아이템 레벨 시스템
#ifdef SERV_BATTLE_FIELD_ITEM_LEVEL
	LUA_GET_VALUE(				luaManager, "m_ItemLevel",				pItemTemplet->m_iItemLevel,				0 );
#endif SERV_BATTLE_FIELD_ITEM_LEVEL
	//}}

	//{{ kimhc // 2009-08-27 // 아이템 봉인(최대로 봉인 가능한 횟수, 봉인에 필요한 '봉인의 인장' 갯수)
#ifdef	SEAL_ITEM
	int iMaxSealCount		= 0;	

	LUA_GET_VALUE(				luaManager, "m_ucMaxSealCount",			iMaxSealCount,			0 );	

	pItemTemplet->m_ucMaxSealCount		= static_cast< BYTE >( iMaxSealCount );	
#endif	SEAL_ITEM
	//}} kimhc // 2009-08-27 // 아이템 봉인(최대로 봉인 가능한 횟수, 봉인에 필요한 '봉인의 인장' 갯수)

	

#ifdef TRIPLE_ENCHANT_TEST
	LUA_GET_VALUE(				luaManager, "m_iMaxAttribEnchantCount",		pItemTemplet->m_iMaxAttribEnchantCount,		0 );	
#else TRIPLE_ENCHANT_TEST
	//{{AFX
	switch( pItemTemplet->m_ItemType )
	{
	case CX2Item::IT_WEAPON:
		{
			pItemTemplet->m_iMaxAttribEnchantCount = 2;
		} break;

	case CX2Item::IT_DEFENCE:
		{
			pItemTemplet->m_iMaxAttribEnchantCount = 1;
		} break;

	default:
		{
			pItemTemplet->m_iMaxAttribEnchantCount = 0;

		} break;
	}
	//}}AFX
#endif TRIPLE_ENCHANT_TEST


	LUA_GET_VALUE_ENUM(			luaManager, "m_EqipPosition",			pItemTemplet->m_EqipPosition,			CX2Unit::EQIP_POSITION,		CX2Unit::EP_QUICK_SLOT );

	LUA_GET_VALUE(				luaManager, "m_AttachFrameName",		pItemTemplet->m_AttachFrameName[0],		L"" );
	LUA_GET_VALUE(				luaManager, "m_AttachFrameName1",		pItemTemplet->m_AttachFrameName[1],		L"" );

	
	LUA_GET_VALUE(				luaManager, "m_FrameOffsetElswordX",	pItemTemplet->m_FrameOffsetElsword.x,	0.0f );
	LUA_GET_VALUE(				luaManager, "m_FrameOffsetElswordY",	pItemTemplet->m_FrameOffsetElsword.y,	0.0f );
	LUA_GET_VALUE(				luaManager, "m_FrameOffsetElswordZ",	pItemTemplet->m_FrameOffsetElsword.z,	0.0f );
	LUA_GET_VALUE(				luaManager, "m_FrameOffsetRenaX",		pItemTemplet->m_FrameOffsetLena.x,		0.0f );
	LUA_GET_VALUE(				luaManager, "m_FrameOffsetRenaY",		pItemTemplet->m_FrameOffsetLena.y,		0.0f );
	LUA_GET_VALUE(				luaManager, "m_FrameOffsetRenaZ",		pItemTemplet->m_FrameOffsetLena.z,		0.0f );
	LUA_GET_VALUE(				luaManager, "m_FrameOffsetAishaX",		pItemTemplet->m_FrameOffsetAisha.x,		0.0f );
	LUA_GET_VALUE(				luaManager, "m_FrameOffsetAishaY",		pItemTemplet->m_FrameOffsetAisha.y,		0.0f );
	LUA_GET_VALUE(				luaManager, "m_FrameOffsetAishaZ",		pItemTemplet->m_FrameOffsetAisha.z,		0.0f );
	LUA_GET_VALUE(				luaManager, "m_FrameOffsetRavenX",		pItemTemplet->m_FrameOffsetRaven.x,		0.0f );
	LUA_GET_VALUE(				luaManager, "m_FrameOffsetRavenY",		pItemTemplet->m_FrameOffsetRaven.y,		0.0f );
	LUA_GET_VALUE(				luaManager, "m_FrameOffsetRavenZ",		pItemTemplet->m_FrameOffsetRaven.z,		0.0f );

	LUA_GET_VALUE(				luaManager, "m_FrameOffsetEveX",		pItemTemplet->m_FrameOffsetEve.x,		0.0f );
	LUA_GET_VALUE(				luaManager, "m_FrameOffsetEveY",		pItemTemplet->m_FrameOffsetEve.y,		0.0f );
	LUA_GET_VALUE(				luaManager, "m_FrameOffsetEveZ",		pItemTemplet->m_FrameOffsetEve.z,		0.0f );

	//{{ kimhc // 2010.11.18 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
	LUA_GET_VALUE(				luaManager, "m_FrameOffsetChungX",		pItemTemplet->m_FrameOffsetChung.x,		0.0f );
	LUA_GET_VALUE(				luaManager, "m_FrameOffsetChungY",		pItemTemplet->m_FrameOffsetChung.y,		0.0f );
	LUA_GET_VALUE(				luaManager, "m_FrameOffsetChungZ",		pItemTemplet->m_FrameOffsetChung.z,		0.0f );
#endif	NEW_CHARACTER_CHUNG
//}} kimhc // 2010.11.18 //  2010-12-23 New Character CHUNG

#ifdef ARA_CHARACTER_BASE
	LUA_GET_VALUE( luaManager, "m_FrameOffsetAraX",	pItemTemplet->m_FrameOffsetAra.x,	0.0f );
	LUA_GET_VALUE( luaManager, "m_FrameOffsetAraY",	pItemTemplet->m_FrameOffsetAra.y,	0.0f );
	LUA_GET_VALUE( luaManager, "m_FrameOffsetAraZ",	pItemTemplet->m_FrameOffsetAra.z,	0.0f );
#endif

	std::string strTemp = "";
	LUA_GET_VALUE( luaManager, "ELSWORD_ITEM_SCALE_ROTATE", strTemp, "" );
	TokenizeByScaleRotate( strTemp, pItemTemplet, CX2Unit::UT_ELSWORD );

	LUA_GET_VALUE( luaManager, "AISHA_ITEM_SCALE_ROTATE", strTemp, "" );
	TokenizeByScaleRotate( strTemp, pItemTemplet, CX2Unit::UT_ARME );

	LUA_GET_VALUE( luaManager, "RENA_ITEM_SCALE_ROTATE", strTemp, "" );
	TokenizeByScaleRotate( strTemp, pItemTemplet, CX2Unit::UT_LIRE );

	LUA_GET_VALUE( luaManager, "RAVEN_ITEM_SCALE_ROTATE", strTemp, "" );
	TokenizeByScaleRotate( strTemp, pItemTemplet, CX2Unit::UT_RAVEN );


	LUA_GET_VALUE( luaManager, "EVE_ITEM_SCALE_ROTATE", strTemp, "" );
	TokenizeByScaleRotate( strTemp, pItemTemplet, CX2Unit::UT_EVE );

	//{{ kimhc // 2010.11.24 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
	LUA_GET_VALUE( luaManager, "CHUNG_ITEM_SCALE_ROTATE", strTemp, "" );
	TokenizeByScaleRotate( strTemp, pItemTemplet, CX2Unit::UT_CHUNG );
#endif	NEW_CHARACTER_CHUNG
//}} kimhc // 2010.11.24 //  2010-12-23 New Character CHUNG

#ifdef ARA_CHARACTER_BASE
	LUA_GET_VALUE( luaManager, "ARA_ITEM_SCALE_ROTATE", strTemp, "" );
	TokenizeByScaleRotate( strTemp, pItemTemplet, CX2Unit::UT_ARA );
#endif

	LUA_GET_VALUE(				luaManager, "m_bCanHyperMode",			pItemTemplet->m_bCanHyperMode,			false );
	
	LUA_GET_VALUE_ENUM(			luaManager, "m_PeriodType",				pItemTemplet->m_PeriodType,				CX2Item::PERIOD_TYPE,		CX2Item::PT_INFINITY );
	LUA_GET_VALUE(				luaManager, "m_Endurance",				pItemTemplet->m_Endurance,				-1 );
	LUA_GET_VALUE(				luaManager, "m_EnduranceDamageMin",		pItemTemplet->m_EnduranceDamage.m_Min,	0 );
	LUA_GET_VALUE(				luaManager, "m_EnduranceDamageMax",		pItemTemplet->m_EnduranceDamage.m_Max,	0 );
	LUA_GET_VALUE(				luaManager, "m_RepairED",				pItemTemplet->m_RepairED,				0.0f );
	LUA_GET_VALUE(				luaManager, "m_RepairVP",				pItemTemplet->m_RepairVP,				0 );
	LUA_GET_VALUE(				luaManager, "m_Quantity",				pItemTemplet->m_Quantity,				0 );
	
	LUA_GET_VALUE_ENUM(	luaManager, "m_PriceType",			pItemTemplet->m_PriceType,				CX2Item::SHOP_PRICE_TYPE,	CX2Item::SPT_GP );
	LUA_GET_VALUE(		luaManager, "m_Price",				pItemTemplet->m_Price,					0 );
	LUA_GET_VALUE(		luaManager, "m_PricePvPPoint",		pItemTemplet->m_PricePvPPoint,			0 );


	//{{ 2010. 03. 22  최육사	기술의 노트
#ifdef SERV_SKILL_NOTE	
	LUA_GET_VALUE(				luaManager, "m_DescriptionInSkillNote",	pItemTemplet->m_DescriptionInSkillNote,	L"" );
#endif SERV_SKILL_NOTE
	//}}
#ifdef HIDE_SET_DESCRIPTION
	LUA_GET_VALUE(				luaManager, "m_bHideSetDesc",	pItemTemplet->m_bHideSetDesc,	false );
#endif HIDE_SET_DESCRIPTION
	
#ifdef PVP_SEASON2_SOCKET
	LUA_GET_VALUE( luaManager, "m_iAttributeLevel",	pItemTemplet->m_iAttributeLevel,	0 );	
#endif


#ifdef COMMON_ITEM_TEMPLET_TEST

	if( CX2Item::UC_ANYONE == pItemTemplet->m_UseCondition &&
		CX2Item::UT_NONE != pItemTemplet->m_UseType ) // 아무나 사용할 수 있는 공용템이고 장착장비라면
	{
		pItemTemplet->m_mapCommonItemModelName.clear();
		pItemTemplet->m_mapCommonItemXETName.clear();

		wstring wstrModelName	= L"";
		wstring wstrXETName		= L"";

		LUA_GET_VALUE( luaManager, "ELSWORD_MODEL_NAME",		wstrModelName,		L"");
		LUA_GET_VALUE( luaManager, "ELSWORD_XET_NAME",			wstrXETName,		L"");
		if( false == wstrModelName.empty() )
		{
			pItemTemplet->m_mapCommonItemModelName[ CX2Unit::UT_ELSWORD ]	= wstrModelName;
			pItemTemplet->m_mapCommonItemXETName[ CX2Unit::UT_ELSWORD ]		= wstrXETName;
		}


		LUA_GET_VALUE( luaManager, "AISHA_MODEL_NAME",			wstrModelName,		L"");
		LUA_GET_VALUE( luaManager, "AISHA_XET_NAME",			wstrXETName,		L"");
		if( false == wstrModelName.empty() )
		{
			pItemTemplet->m_mapCommonItemModelName[ CX2Unit::UT_ARME ]	= wstrModelName;
			pItemTemplet->m_mapCommonItemXETName[ CX2Unit::UT_ARME ]		= wstrXETName;
		}

		LUA_GET_VALUE( luaManager, "RENA_MODEL_NAME",			wstrModelName,		L"");
		LUA_GET_VALUE( luaManager, "RENA_XET_NAME",				wstrXETName,		L"");
		if( false == wstrModelName.empty() )
		{
			pItemTemplet->m_mapCommonItemModelName[ CX2Unit::UT_LIRE ]	= wstrModelName;
			pItemTemplet->m_mapCommonItemXETName[ CX2Unit::UT_LIRE ]		= wstrXETName;
		}


		LUA_GET_VALUE( luaManager, "RAVEN_MODEL_NAME",			wstrModelName,		L"");
		LUA_GET_VALUE( luaManager, "RAVEN_XET_NAME",			wstrXETName,		L"");
		if( false == wstrModelName.empty() )
		{
			pItemTemplet->m_mapCommonItemModelName[ CX2Unit::UT_RAVEN ]	= wstrModelName;
			pItemTemplet->m_mapCommonItemXETName[ CX2Unit::UT_RAVEN ]		= wstrXETName;
		}

		LUA_GET_VALUE( luaManager, "EVE_MODEL_NAME",			wstrModelName,		L"");
		LUA_GET_VALUE( luaManager, "EVE_XET_NAME",				wstrXETName,		L"");
		if( false == wstrModelName.empty() )
		{
			pItemTemplet->m_mapCommonItemModelName[ CX2Unit::UT_EVE ]	= wstrModelName;
			pItemTemplet->m_mapCommonItemXETName[ CX2Unit::UT_EVE ]		= wstrXETName;
		}

		//{{ kimhc // 2010.11.24 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
		LUA_GET_VALUE( luaManager, "CHUNG_MODEL_NAME",			wstrModelName,		L"");
		LUA_GET_VALUE( luaManager, "CHUNG_XET_NAME",			wstrXETName,		L"");
		if( false == wstrModelName.empty() )
		{
			pItemTemplet->m_mapCommonItemModelName[ CX2Unit::UT_CHUNG ]	= wstrModelName;
			pItemTemplet->m_mapCommonItemXETName[ CX2Unit::UT_CHUNG ] = wstrXETName;
		}
#endif	NEW_CHARACTER_CHUNG
//}} kimhc // 2010.11.24 //  2010-12-23 New Character CHUNG

#ifdef ARA_CHARACTER_BASE
		LUA_GET_VALUE( luaManager, "ARA_MODEL_NAME",	wstrModelName,	L"" );
		LUA_GET_VALUE( luaManager, "ARA_XET_NAME",		wstrXETName,	L"" );
		if( false == wstrModelName.empty() )
		{
			pItemTemplet->m_mapCommonItemModelName[ CX2Unit::UT_ARA ] = wstrModelName;
			pItemTemplet->m_mapCommonItemXETName[ CX2Unit::UT_ARA ] = wstrXETName;
		}
#endif
	}

#endif COMMON_ITEM_TEMPLET_TEST

	if( luaManager.BeginTable( "m_Stat" ) == true )
	{
		float fValue = 0.0f;

		LUA_GET_VALUE( luaManager, "m_fBaseHP",			fValue,			0.f );
		pItemTemplet->m_Stat.m_fBaseHP = fValue;
		LUA_GET_VALUE( luaManager, "m_fAtkPhysic",		fValue,			0.f );
		pItemTemplet->m_Stat.m_fAtkPhysic = fValue;
		LUA_GET_VALUE( luaManager, "m_fAtkMagic",		fValue,			0.f );
		pItemTemplet->m_Stat.m_fAtkMagic = fValue;

		LUA_GET_VALUE( luaManager, "m_fDefPhysic",		fValue,			0.f );
		pItemTemplet->m_Stat.m_fDefPhysic = fValue;
		LUA_GET_VALUE( luaManager, "m_fDefMagic",		fValue,			0.f );
		pItemTemplet->m_Stat.m_fDefMagic = fValue;

		luaManager.EndTable();
	}

	//{{ kimhc // 2009-10-19 // 최대 MP 증가 값 추가
	float fAddMaxMP = 0.0f;
	LUA_GET_VALUE( luaManager,		"m_fAddMaxMP",			fAddMaxMP,			0.f );
	pItemTemplet->m_fAddMaxMP = fAddMaxMP;

	//}} kimhc // 2009-10-19 // 최대 MP 증가 값 추가
	vector<UINT> vecTemp;
	CX2BuffTempletManager::GetInstance()->GetBuffFactorPtrFromBuffFactorList( luaManager, pItemTemplet->m_vecBuffFactorPtr, vecTemp );

	if( luaManager.BeginTable( "m_SpecialAbilityList" ) == true )
	{
		int index = 1;
		while( luaManager.BeginTable( index ) == true )
		{
			CX2Item::SpecialAbility sa;

			LUA_GET_VALUE_ENUM( luaManager, "TYPE",				sa.m_Type,		CX2Item::SPECIAL_ABILITY_TYPE, CX2Item::SAT_NONE );
#ifdef GUIDE_UNDEFINED_SAT_TYPE
			wstring strSatType = L"";
			switch(sa.m_Type) 
			{
			case CX2Item::SAT_TRANSFORM_MONSTER:
				{
					strSatType = L"SAT_TRANSFORM_MONSTER";
				} break;
			case CX2Item::SAT_WAY_OF_SWORD_GAUGE_UP:
				{
					strSatType = L"SAT_WAY_OF_SWORD_GAUGE_UP";
				} break;
			case CX2Item::SAT_FORCE_CRITICAL_MAX:
				{
					strSatType = L"SAT_FORCE_CRITICAL_MAX";
				} break;
			default:
				break;
			}

			if( false == strSatType.empty() )
			{
				sa.m_Type = CX2Item::SAT_NONE;
				MessageBox( g_pKTDXApp->GetHWND(), GET_REPLACED_STRING( ( STR_ID_18058, "L", strSatType) ), L"Error", MB_OK );
			} 
#endif //GUIDE_UNDEFINED_SAT_TYPE
			LUA_GET_VALUE(		luaManager, "COOL_TIME",		sa.m_CoolTime,	0 );
			LUA_GET_VALUE(		luaManager, "VALUE1",			sa.m_Value1,	0 );
			LUA_GET_VALUE(		luaManager, "VALUE2",			sa.m_Value2,	0 );
			LUA_GET_VALUE(		luaManager, "VALUE3",			sa.m_Value3,	0 );
#ifdef SUMMON_MONSTER_CARD_SYSTEM
			LUA_GET_VALUE(		luaManager, "STRING_VALUE1",	sa.m_StringValue1,	L"Group 0" );		//Group으로 된 정보를 받아 올 수 있는 인자
#endif SUMMON_MONSTER_CARD_SYSTEM

			pItemTemplet->m_SpecialAbilityList.push_back( sa );

			luaManager.EndTable();
			index++;
		}
		luaManager.EndTable();
	}


	pItemTemplet->m_vecSocketOption.resize(0);
	if( luaManager.BeginTable( "SOCKET_OPTION" ) == true )
	{
		int socketID = 0;
		int index = 1;
		while( true == luaManager.GetValue( index, socketID ) )
		{
			pItemTemplet->m_vecSocketOption.push_back( socketID );
			index++;
		}

		luaManager.EndTable();
	}


#ifdef USER_WEAPON_PARTICLE_SCRIPT_TEST
	pItemTemplet->m_vecAttachedParticleData.resize(0);
	if( luaManager.BeginTable( "ATTACHED_PARTICLE" ) == true )
	{
		int index = 1;
		while( luaManager.BeginTable( index ) == true )
		{
			CX2Item::AttachedParticleData particleData;
			
			LUA_GET_VALUE( luaManager, "PARTICLE_NAME", particleData.wstrParticleName, L"" );
			LUA_GET_VALUE( luaManager, "BONE_NAME",		particleData.wstrBoneName, L"" );


			if( false == particleData.wstrParticleName.empty() &&
				false == particleData.wstrBoneName.empty() )
			{
				pItemTemplet->m_vecAttachedParticleData.push_back( particleData );
			}

			luaManager.EndTable();
			index++;
		}
		luaManager.EndTable();	// ATTACHED_PARTICLE
	}
#endif USER_WEAPON_PARTICLE_SCRIPT_TEST





#ifdef SUB_EQUIP_TEST
	pItemTemplet->m_vecAttachedMeshData.resize(0);
	if( luaManager.BeginTable( "ATTACHED_MESH" ) == true )
	{
		int index = 1;
		while( luaManager.BeginTable( index ) == true )
		{
			CX2Item::AttachedMeshData data;

			LUA_GET_VALUE( luaManager, "MESH_NAME", data.wstrMeshName, L"" );
			LUA_GET_VALUE( luaManager, "BONE_NAME",	data.wstrBoneName, L"" );

#ifdef ATTACH_SKINMESH			
			data.bSkinMesh = false;
			if(	pItemTemplet->m_UseType == CX2Item::UT_SKIN )
			{
				data.bSkinMesh = true;
			}			

			data.bJiggle = false;
			wstring wstrJiggleToken = L"Jiggle_Bone";
			if( (int)data.wstrMeshName.find( wstrJiggleToken.c_str() ) != -1 ) // 찾았다
			{
				data.bJiggle = true;
			}
			//LUA_GET_VALUE( luaManager, "JIGGLE_BONE",	data.bJiggle,	false );
#endif

			if( false == data.wstrMeshName.empty() &&
				false == data.wstrBoneName.empty() )
			{
				pItemTemplet->m_vecAttachedMeshData.push_back( data );
			}

			luaManager.EndTable();
			index++;
		}
		luaManager.EndTable();	// ATTACHED_MESH
	}
#endif SUB_EQUIP_TEST


	LUA_GET_VALUE(		luaManager, "m_CoolTime",			pItemTemplet->m_CoolTime,				0 );
	LUA_GET_VALUE(		luaManager, "m_SetID",				pItemTemplet->m_SetID,					0 );

	//{{ kimhc // 2011-07-05 // 옵션데이타 수치화 작업
	// 셋트효과가 있고, 요구레벨도 있는 경우
	if ( 0 < pItemTemplet->m_SetID && 0 < pItemTemplet->m_UseLevel )
	{
		UpdateSetIDAndMaxLevelMap( pItemTemplet->m_SetID, pItemTemplet->m_UseLevel );
	}
	//}} kimhc // 2011-07-05 // 옵션데이타 수치화 작업



#ifdef ITEM_SLASH_TRACE_COLOR_TEST

	if( luaManager.BeginTable( "SLASH_TRACE_COLOR" ) == true )
	{
		float afValue[4] = { 1.f, };
		for( int valueIndex = 0; valueIndex < ARRAY_SIZE(afValue); valueIndex++ )
		{
			if( false == luaManager.GetValue( valueIndex+1, afValue[valueIndex] ) )
				break;

			afValue[valueIndex] /= 255.f;
		}

		pItemTemplet->m_coSlashTrace = D3DXCOLOR( &afValue[0] );
		luaManager.EndTable(); // SLASH_TRACE_COLOR
	}


	if( luaManager.BeginTable( "SLASH_TRACE_TIP_COLOR" ) == true )
	{
		float afValue[4] = { 1.f, };
		for( int valueIndex = 0; valueIndex < ARRAY_SIZE(afValue); valueIndex++ )
		{
			if( false == luaManager.GetValue( valueIndex+1, afValue[valueIndex] ) )
				break;
			afValue[valueIndex] /= 255.f;
		}

		pItemTemplet->m_coSlashTraceTip = D3DXCOLOR( &afValue[0] );
		luaManager.EndTable(); // SLASH_TRACE_TIP_COLOR
	}


	if( luaManager.BeginTable( "HYPER_SLASH_TRACE_COLOR" ) == true )
	{
		float afValue[4] = { 1.f, };
		for( int valueIndex = 0; valueIndex < ARRAY_SIZE(afValue); valueIndex++ )
		{
			if( false == luaManager.GetValue( valueIndex+1, afValue[valueIndex] ) )
				break;
			afValue[valueIndex] /= 255.f;
		}

		pItemTemplet->m_coSlashTraceHyper = D3DXCOLOR( &afValue[0] );
		luaManager.EndTable(); // HYPER_SLASH_TRACE_COLOR
	}

	if( luaManager.BeginTable( "HYPER_SLASH_TRACE_TIP_COLOR" ) == true )
	{
		float afValue[4] = { 1.f, };
		for( int valueIndex = 0; valueIndex < ARRAY_SIZE(afValue); valueIndex++ )
		{
			if( false == luaManager.GetValue( valueIndex+1, afValue[valueIndex] ) )
				break;
			afValue[valueIndex] /= 255.f;
		}

		pItemTemplet->m_coSlashTraceTipHyper = D3DXCOLOR( &afValue[0] );
		luaManager.EndTable(); // HYPER_SLASH_TRACE_TIP_COLOR
	}

#endif ITEM_SLASH_TRACE_COLOR_TEST

#ifdef BUFF_TEMPLET_SYSTEM
	LUA_GET_VALUE(		luaManager,	"m_Buff_ID",		pItemTemplet->m_Buff_ID,	0 );
#endif BUFF_TEMPLET_SYSTEM

#ifdef ADD_TRAININGGAME_NPC
	LUA_GET_VALUE_ENUM( luaManager, "m_Summon_Npc",		pItemTemplet->m_iSummonNpcID, CX2UnitManager::NPC_UNIT_ID, CX2UnitManager::NUI_NONE );
#endif


	if( m_ItemTempletIDMap.find(pItemTemplet->m_ItemID) != m_ItemTempletIDMap.end() )
	{
		SAFE_DELETE(pItemTemplet); 
		ErrorLog(XEM_ERROR12);
		return false;
	}
	else
	{
		map< int, std::vector<int> >::iterator mit = m_mapSetItemIDList.find( pItemTemplet->m_SetID );
		if ( mit != m_mapSetItemIDList.end() )
		{
			mit->second.push_back( pItemTemplet->m_ItemID );
		}
		else
		{
			vector< int > vecTempItemID;
			vecTempItemID.push_back( pItemTemplet->m_ItemID );
			m_mapSetItemIDList.insert( std::make_pair( pItemTemplet->m_SetID, vecTempItemID ) );
		}
		
		m_ItemTempletIDMap.insert( std::make_pair(pItemTemplet->m_ItemID, pItemTemplet) );
	}
	
	return true;
}

#endif  //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING

#ifndef X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
#ifdef SERV_ITEM_LUA_TRANS_DEVIDE
bool CX2ItemManager::AddItemTempletTrans_LUA()
{
	CX2Item::ItemTemplet* pItemTemplet = new CX2Item::ItemTemplet();

	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
	TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );

	LUA_GET_VALUE_RETURN(	luaManager, "m_ItemID",						pItemTemplet->m_ItemID,					0,		SAFE_DELETE(pItemTemplet); return false; );
	LUA_GET_VALUE_RETURN(	luaManager, "m_Name",						pItemTemplet->m_Name,					L"",	SAFE_DELETE(pItemTemplet); return false; );
	LUA_GET_VALUE(			luaManager, "m_Description",				pItemTemplet->m_Description,			L"" );
	LUA_GET_VALUE(			luaManager, "m_DescriptionInShop",			pItemTemplet->m_DescriptionInShop,		L"" );
	LUA_GET_VALUE(			luaManager, "m_DescriptionInSkillNote",		pItemTemplet->m_DescriptionInSkillNote,		L"" );	

	// 여기서 치환 해주어야 함.
	map<int, CX2Item::ItemTemplet*>::iterator mit;
	mit = m_ItemTempletIDMap.find( pItemTemplet->m_ItemID );
	if( mit == m_ItemTempletIDMap.end() )
	{
		wstringstream wstrStream;
		wstrStream << L"ItemTrans.Lua And Item.lua Not Equal ItemID..." << (pItemTemplet->m_ItemID) << L"_";
		ErrorLogMsg( XEM_ERROR105, wstrStream.str().c_str() );	
		// 에러로그 출력
		SAFE_DELETE(pItemTemplet); 
		return false;
	}
	else
	{
		mit->second->m_Name = pItemTemplet->m_Name;
		mit->second->m_Description = pItemTemplet->m_Description;
		mit->second->m_DescriptionInShop = pItemTemplet->m_DescriptionInShop;
		mit->second->m_DescriptionInSkillNote = pItemTemplet->m_DescriptionInSkillNote;		
	}

	return true;
}
#endif SERV_ITEM_LUA_TRANS_DEVIDE

#ifdef LUA_TRANS_DEVIDE
bool CX2ItemManager::AddSetItemDataTrans_LUA()
{
	int setID = 0;
	SetItemData* pSetItemData = new SetItemData();

	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
	TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );

	LUA_GET_VALUE(	luaManager, "m_SetID",						setID,					0	);
	LUA_GET_VALUE(	luaManager, "m_SetName",					pSetItemData->m_SetName,				L""	);

	// 여기서 치환 해주어야 함.
	map< int, SetItemData* >::iterator mit;
	mit = m_mapSetItemData.find( setID );
	if( mit == m_mapSetItemData.end() )
	{
		wstringstream wstrStream;
		wstrStream << L"SetItemTrans.Lua And SetItem.lua Not Equal ItemID..." << (setID) << L"_";
		ErrorLogMsg( XEM_ERROR105, wstrStream.str().c_str() );	
		// 에러로그 
		SAFE_DELETE(pSetItemData); 
		return false;
	}
	else
	{
		mit->second->m_SetName = pSetItemData->m_SetName;
	}

	return true;
}
#endif LUA_TRANS_DEVIDE
#endif X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING

#ifdef IN_ITEM_KIM_USE_HIDE_SET_DESC
bool CX2ItemManager::AddHideSetDesc_LUA()
{
	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
#ifndef X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
	TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );
#endif X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING

	int m_iItemID = 0;
	bool m_bHideSetDesc  = true;
	LUA_GET_VALUE_RETURN(	luaManager, "m_ItemID",						m_iItemID,					0,			return false; );

	m_mapHideSetDescItem.insert(std::make_pair(m_iItemID, m_bHideSetDesc));

	return true;
}

bool CX2ItemManager::GetHideSetDescItem(int itemID)
{
 		bool checkHideItem = false;


		std::map<int,bool>::const_iterator itMapHide = m_mapHideSetDescItem.find(itemID);

		if( itMapHide != m_mapHideSetDescItem.end())
		{
			checkHideItem = true;
		}

		return checkHideItem;
}
#endif //IN_ITEM_KIM_USE_HIDE_SET_DESC

bool CX2ItemManager::AddShopItemList_LUA()
{
	ItemIDList* pItemIDList = new ItemIDList();


	
	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
#ifndef X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
	TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING

	int villiageID;
	LUA_GET_VALUE_RETURN(	luaManager, "VILLIAGE_ID",	villiageID,	0, goto LFail );

	if( luaManager.BeginTable( "SALE_GROUP" ) == true )
	{
		int index = 1;
		int buf;
		while( luaManager.GetValue(index, buf) == true )
		{
			map<int, ItemIDList*>::iterator iter;
			iter = m_ShopItemGroup.find(buf);
			if( iter != m_ShopItemGroup.end() )
			{
				ItemIDList* pGroupItemIDList = iter->second;
				for( int i = 0; i < (int)pItemIDList->m_ItemIDList.size(); i++ )
				{
					int itemID = pItemIDList->m_ItemIDList[i];
					pItemIDList->m_ItemIDList.push_back( itemID );
				}				
			}

			index++;
		}
			
		luaManager.EndTable();
	}

	if( luaManager.BeginTable( "SALE_ITEM" ) == true )
	{
		int index = 1;
		int buf;
		while( luaManager.GetValue(index, buf) == true )
		{
			pItemIDList->m_ItemIDList.push_back( buf );
			index++;
		}
		luaManager.EndTable();
	}


	if( m_ShopItemList.find(villiageID) != m_ShopItemList.end() )
	{
		SAFE_DELETE(pItemIDList); 
		return false;
	}
	else
	{
		m_ShopItemList.insert( std::make_pair(villiageID, pItemIDList) );
	}

	// 특정 npc만이 판매하는 물품
	NpcItemIDList* pNpcItemIDList = NULL;
	if( luaManager.BeginTable( "SALE_ITEM_NPC" ) == true )
	{
		int index = 1;
		while( luaManager.BeginTable( index ) == true )
		{
			pNpcItemIDList = new NpcItemIDList();

			int npcID;
			LUA_GET_VALUE_RETURN(	luaManager, "NPC_ID",	npcID,	0, goto LFail01 );

			pNpcItemIDList->m_NpcId = npcID;

			if( luaManager.BeginTable( "ITEM_LIST" ) == true )
			{
				int index = 1;
				int buf;
				while( luaManager.GetValue(index, buf) == true )
				{
					pNpcItemIDList->m_ItemIDList.push_back( buf );
					index++;
				}
				luaManager.EndTable();
			}

			if( m_ShopNpcItemList.find( std::make_pair(villiageID, pNpcItemIDList->m_NpcId) ) != m_ShopNpcItemList.end() )
			{
				SAFE_DELETE(pNpcItemIDList); 
				return false;
			}
			else
			{
				m_ShopNpcItemList.insert( std::make_pair( std::make_pair(villiageID, pNpcItemIDList->m_NpcId), pNpcItemIDList) );
			}

			index++;
			luaManager.EndTable();
		}

		luaManager.EndTable();
	}

	return true;

LFail01:
	SAFE_DELETE( pNpcItemIDList );
LFail:
	SAFE_DELETE( pItemIDList );	
	return false;
}

bool CX2ItemManager::AddShopItemGroup_LUA()
{
	ItemIDList* pItemIDList = new ItemIDList();



	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
#ifndef X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
	TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING

	int groupID;
	LUA_GET_VALUE_RETURN(	luaManager, "GROUP_ID",	groupID,	0, goto LFail );

	if( luaManager.BeginTable( "ITEM_ID" ) == true )
	{
		int index = 1;
		int buf;
		while( luaManager.GetValue(index, buf) == true )
		{
			pItemIDList->m_ItemIDList.push_back( buf );
			index++;
		}

		luaManager.EndTable();
	}






	if( m_ShopItemGroup.find(groupID) != m_ShopItemGroup.end() )
	{
		SAFE_DELETE(pItemIDList); 
		return false;
	}
	else
	{
		m_ShopItemGroup.insert( std::make_pair(groupID, pItemIDList) );
	}



	return true;

LFail:
	SAFE_DELETE( pItemIDList );
	return false;
}

void CX2ItemManager::GetShopItemList( int mapID, vector<const CX2Item::ItemTemplet*>& itemTempletList )
{
	map<int, ItemIDList*>::iterator iter;
	iter = m_ShopItemList.find( mapID );
	if( m_ShopItemList.end() == iter )
	{
		// mapID가 이상하다 
		itemTempletList.clear();
		return;
	}

	ItemIDList* pItemIDList = iter->second;

	itemTempletList.clear();
	for( int i = 0; i < (int)pItemIDList->m_ItemIDList.size(); i++ )
	{
		const CX2Item::ItemTemplet* pItemTemplet = GetItemTemplet( pItemIDList->m_ItemIDList[i] );
		if( pItemTemplet != NULL )
			itemTempletList.push_back( pItemTemplet );
	}
}

bool CX2ItemManager::GetNpcShopItemList( int mapID, int npcID, vector<const CX2Item::ItemTemplet*>& itemTempletList )
{
	map< std::pair<int,int>, NpcItemIDList*>::iterator iter;
	iter = m_ShopNpcItemList.find( std::make_pair( mapID, npcID ) );
	if( m_ShopNpcItemList.end() == iter )
	{
		// mapID가 이상하다 
		itemTempletList.clear();
		return false;
	}
	
	NpcItemIDList* pItemIDList = iter->second;

	if(pItemIDList->m_NpcId != npcID)
	{
		itemTempletList.clear();
		return false;
	}

	itemTempletList.clear();
	for( int i = 0; i < (int)pItemIDList->m_ItemIDList.size(); i++ )
	{
		const CX2Item::ItemTemplet* pItemTemplet = GetItemTemplet( pItemIDList->m_ItemIDList[i] );
		if( pItemTemplet != NULL )
			itemTempletList.push_back( pItemTemplet );
	}

	if(itemTempletList.size() > 0 )
		return true;

	return false;
}







#ifdef EQUIP_STRIP_TEST
//{{AFX
void CX2ItemManager::UpdateEqip( CX2Unit* pUnit, CKTDGXSkinAnimPtr pAnim, 
									CKTDXDeviceXSkinMesh* pMotion, CKTDXDeviceXET* pAnimXET, 
									IN OUT vector<CX2EqipPtr>& viewEqipList, CX2ItemManager::EquipPositionMap& mapBasicEquip, bool bLoad )
{
	//기존 장비 삭제
	viewEqipList.resize( 0 );
	mapBasicEquip.clear();


	//장비 리스트 업데이트
	CX2Item* pItem = NULL;
	for( int i = 0; i < pUnit->GetViewEqipNum(); i++ )
	{
		int enchantLevel = 0;

		pItem = pUnit->GetInventory().GetItem( pUnit->GetViewEqipUID(i) );
		ASSERT( NULL != pItem );
		if( NULL == pItem )
			continue;

		{
			enchantLevel = pItem->GetItemData().m_EnchantLevel;
		}
		

		CX2EqipPtr pCX2Eqip;
#ifdef COMMON_ITEM_TEMPLET_TEST
		if( pItem->GetItemTemplet()->GetUseCondition() == CX2Item::UC_ANYONE )
		{
			pCX2Eqip = CX2Eqip::CreateEquip( pItem, pAnim, enchantLevel, false, bLoad, pUnit->GetType() );
		}
		else
#endif COMMON_ITEM_TEMPLET_TEST
		{
			pCX2Eqip = CX2Eqip::CreateEquip( pItem, pAnim, enchantLevel, false, bLoad );
		}
		
		viewEqipList.push_back( pCX2Eqip );
	}


	ReplaceEmptyToBasic( pUnit, pAnim, pMotion, pAnimXET, IN OUT viewEqipList, mapBasicEquip, bLoad );
}



void CX2ItemManager::ReplaceEmptyToBasic( CX2Unit* pUnit, CKTDGXSkinAnimPtr pAnim, 
										 CKTDXDeviceXSkinMesh* pMotion, CKTDXDeviceXET* pAnimXET, 
										 IN OUT vector<CX2EqipPtr>& viewEqipList, CX2ItemManager::EquipPositionMap& mapBasicEquip, bool bLoad )
{
	//빠진 부위가 있으면 기본 아이템으로 대체한다
	bool viewEqipPosition[50];
	CheckEmptyEqipPosition( viewEqipPosition, viewEqipList );

	ReplaceEmptyToBasic( pUnit, pAnim, pMotion, pAnimXET, viewEqipList, viewEqipPosition, CX2Unit::EP_DEFENCE_HAIR,	pUnit->GetUnitTemplet()->m_BasicHairItemID,		bLoad	);
	ReplaceEmptyToBasic( pUnit, pAnim, pMotion, pAnimXET, viewEqipList, viewEqipPosition, CX2Unit::EP_DEFENCE_FACE,	pUnit->GetUnitTemplet()->m_BasicFaceItemID,		bLoad	);


	CX2EqipPtr pEquipPtr;
	pEquipPtr = ReplaceEmptyToBasic( pUnit, pAnim, pMotion, pAnimXET, viewEqipList, viewEqipPosition, CX2Unit::EP_DEFENCE_BODY,	pUnit->GetUnitTemplet()->m_BasicBodyItemID,		bLoad	);
	if( pEquipPtr != NULL )
		mapBasicEquip[ CX2Unit::EP_DEFENCE_BODY ] = pEquipPtr;
	pEquipPtr = ReplaceEmptyToBasic( pUnit, pAnim, pMotion, pAnimXET, viewEqipList, viewEqipPosition, CX2Unit::EP_DEFENCE_LEG,	pUnit->GetUnitTemplet()->m_BasicLegItemID,		bLoad	);
	if( pEquipPtr != NULL )
		mapBasicEquip[ CX2Unit::EP_DEFENCE_LEG ] = pEquipPtr;
	pEquipPtr = ReplaceEmptyToBasic( pUnit, pAnim, pMotion, pAnimXET, viewEqipList, viewEqipPosition, CX2Unit::EP_DEFENCE_HAND,	pUnit->GetUnitTemplet()->m_BasicHandItemID,		bLoad	);
	if( pEquipPtr != NULL )
		mapBasicEquip[ CX2Unit::EP_DEFENCE_HAND ] = pEquipPtr;
	pEquipPtr = ReplaceEmptyToBasic( pUnit, pAnim, pMotion, pAnimXET, viewEqipList, viewEqipPosition, CX2Unit::EP_DEFENCE_FOOT,	pUnit->GetUnitTemplet()->m_BasicFootItemID,		bLoad	);
	if( pEquipPtr != NULL )
		mapBasicEquip[ CX2Unit::EP_DEFENCE_FOOT ] = pEquipPtr;


	ReplaceEmptyToBasic( pUnit, pAnim, pMotion, pAnimXET, viewEqipList, viewEqipPosition, CX2Unit::EP_WEAPON_HAND,	pUnit->GetUnitTemplet()->m_BasicWeaponItemID,	bLoad	);


	switch( pUnit->GetUnitTemplet()->m_UnitType )
	{
	case CX2Unit::UT_RAVEN:
		{
			ReplaceEmptyToBasic( pUnit, pAnim, pMotion, pAnimXET, viewEqipList, viewEqipPosition, CX2Unit::EP_RAVEN_LEFT_ARM, pUnit->GetUnitTemplet()->m_RavenLeftArmItemID,	bLoad	);
		} break;
	}
}

CX2EqipPtr CX2ItemManager::ReplaceEmptyToBasic( CX2Unit* pUnit, CKTDGXSkinAnimPtr pAnim, 
										 CKTDXDeviceXSkinMesh* pMotion, CKTDXDeviceXET* pAnimXET, 
										 IN OUT vector<CX2EqipPtr>& viewEqipList, bool* pViewEqipPosition,
										 CX2Unit::EQIP_POSITION eqipPos, int itemID, bool bLoad )
{
	CX2EqipPtr pCX2Eqip;


	if( itemID == 0 )
		return pCX2Eqip;


	// 기본 장비 생성

    CX2Item* pX2Item = NULL;
    {
	    CX2Item::ItemData kItemData;
	    kItemData.m_ItemID				= itemID;
	    //kItemData.m_OwnerUnitUID		= pUnit->GetUID();
	    //kItemData.m_OwnerUserUID		= pUnit->GetOwnerUserUID();
	    kItemData.m_Endurance			= 1;

	    pX2Item				= new CX2Item( kItemData, pUnit );
        ASSERT( pX2Item != NULL );
    }
	int enchantLevel = pX2Item->GetItemData().m_EnchantLevel;
	
#ifdef COMMON_ITEM_TEMPLET_TEST
	if( pX2Item->GetItemTemplet()->GetUseCondition() == CX2Item::UC_ANYONE )
	{
		pCX2Eqip = CX2Eqip::CreateEquip( pX2Item, pAnim, enchantLevel, true, bLoad, pUnit->GetType() );
	}
	else
#endif COMMON_ITEM_TEMPLET_TEST
	{
		pCX2Eqip = CX2Eqip::CreateEquip( pX2Item, pAnim, enchantLevel, true, bLoad );
	}



	if( pViewEqipPosition[eqipPos] == false )
	{
		viewEqipList.push_back( pCX2Eqip );
		return CX2EqipPtr();
	}

	
	
	return pCX2Eqip;
}
//}}AFX

#else EQUIP_STRIP_TEST

void CX2ItemManager::UpdateEqip( CX2Unit* pUnit, CKTDGXSkinAnimPtr pAnim, 
									CKTDXDeviceXSkinMesh* pMotion, CKTDXDeviceXET* pAnimXET, 
									IN OUT vector<CX2EqipPtr>& viewEqipList, bool bLoad )
{
	//기존 장비 삭제
	viewEqipList.resize( 0 );
	

	//장비 리스트 업데이트
	CX2Item* pItem = NULL;
	for( int i = 0; i < pUnit->GetViewEqipNum(); i++ )
	{
		int enchantLevel = 0;

		pItem = pUnit->GetInventory().GetItem( pUnit->GetViewEqipUID(i) );
		ASSERT( NULL != pItem );
		if( NULL == pItem )
			continue;

		enchantLevel = pItem->GetItemData().m_EnchantLevel;
		

		CX2EqipPtr pCX2Eqip;
#ifdef COMMON_ITEM_TEMPLET_TEST
		if( pItem->GetItemTemplet()->GetUseCondition() == CX2Item::UC_ANYONE )
		{
			pCX2Eqip = CX2Eqip::CreateEquip( pItem, pAnim, enchantLevel, false, bLoad, pUnit->GetType() );
		}
		else
#endif COMMON_ITEM_TEMPLET_TEST
		{
			pCX2Eqip = CX2Eqip::CreateEquip( pItem, pAnim, enchantLevel, false, bLoad );
		}
		
		viewEqipList.push_back( pCX2Eqip );
	}



	ReplaceEmptyToBasic( pUnit, pAnim, pMotion, pAnimXET, IN OUT viewEqipList, bLoad );
}


void CX2ItemManager::ReplaceEmptyToBasic( CX2Unit* pUnit, CKTDGXSkinAnimPtr pAnim, 
										 CKTDXDeviceXSkinMesh* pMotion, CKTDXDeviceXET* pAnimXET, 
										 IN OUT vector<CX2EqipPtr>& viewEqipList, bool bLoad )
{
	//빠진 부위가 있으면 기본 아이템으로 대체한다
	bool viewEqipPosition[50];
	CheckEmptyEqipPosition( viewEqipPosition, viewEqipList );

	ReplaceEmptyToBasic( pUnit, pAnim, pMotion, pAnimXET, viewEqipList, viewEqipPosition, CX2Unit::EP_DEFENCE_HAIR,	pUnit->GetUnitTemplet()->m_BasicHairItemID,		bLoad	);
	ReplaceEmptyToBasic( pUnit, pAnim, pMotion, pAnimXET, viewEqipList, viewEqipPosition, CX2Unit::EP_DEFENCE_FACE,	pUnit->GetUnitTemplet()->m_BasicFaceItemID,		bLoad	);
	ReplaceEmptyToBasic( pUnit, pAnim, pMotion, pAnimXET, viewEqipList, viewEqipPosition, CX2Unit::EP_DEFENCE_BODY,	pUnit->GetUnitTemplet()->m_BasicBodyItemID,		bLoad	);
	ReplaceEmptyToBasic( pUnit, pAnim, pMotion, pAnimXET, viewEqipList, viewEqipPosition, CX2Unit::EP_DEFENCE_LEG,	pUnit->GetUnitTemplet()->m_BasicLegItemID,		bLoad	);
	ReplaceEmptyToBasic( pUnit, pAnim, pMotion, pAnimXET, viewEqipList, viewEqipPosition, CX2Unit::EP_DEFENCE_HAND,	pUnit->GetUnitTemplet()->m_BasicHandItemID,		bLoad	);
	ReplaceEmptyToBasic( pUnit, pAnim, pMotion, pAnimXET, viewEqipList, viewEqipPosition, CX2Unit::EP_DEFENCE_FOOT,	pUnit->GetUnitTemplet()->m_BasicFootItemID,		bLoad	);
	ReplaceEmptyToBasic( pUnit, pAnim, pMotion, pAnimXET, viewEqipList, viewEqipPosition, CX2Unit::EP_WEAPON_HAND,	pUnit->GetUnitTemplet()->m_BasicWeaponItemID,	bLoad	);


	switch( pUnit->GetUnitTemplet()->m_UnitType )
	{
	case CX2Unit::UT_RAVEN:
		{
			ReplaceEmptyToBasic( pUnit, pAnim, pMotion, pAnimXET, viewEqipList, viewEqipPosition, CX2Unit::EP_RAVEN_LEFT_ARM, pUnit->GetUnitTemplet()->m_RavenLeftArmItemID,	bLoad	);
		} break;
	}
}

void CX2ItemManager::ReplaceEmptyToBasic( CX2Unit* pUnit, CKTDGXSkinAnimPtr pAnim, 
										 CKTDXDeviceXSkinMesh* pMotion, CKTDXDeviceXET* pAnimXET, 
										 IN OUT vector<CX2EqipPtr>& viewEqipList, bool* pViewEqipPosition,
										 CX2Unit::EQIP_POSITION eqipPos, int itemID, bool bLoad )
{
	if( pViewEqipPosition[eqipPos] == false
		&& itemID != 0 )
	{
        CX2Item* pX2Item = NULL;
        {
		    CX2Item::ItemData kItemData;
		    kItemData.m_ItemID				= itemID;
		    //kItemData.m_OwnerUnitUID		= pUnit->GetUID();
		    //kItemData.m_OwnerUserUID		= pUnit->GetOwnerUserUID();
		    kItemData.m_Endurance			= 1;

		    pX2Item				= new CX2Item( kItemData, pUnit );
            ASSERT( pX2Item != NULL );
        }
		int enchantLevel = pX2Item->GetItemData().m_EnchantLevel;



		CX2EqipPtr pCX2Eqip;
#ifdef COMMON_ITEM_TEMPLET_TEST
		if( pX2Item->GetItemTemplet()->GetUseCondition() == CX2Item::UC_ANYONE )
		{
			pCX2Eqip = CX2Eqip::CreateEquip( pX2Item, pAnim, enchantLevel, true, bLoad, pUnit->GetType() );
		}
		else
#endif COMMON_ITEM_TEMPLET_TEST
		{
			pCX2Eqip = CX2Eqip::CreateEquip( pX2Item, pAnim, enchantLevel, true, bLoad );
		}

		viewEqipList.push_back( pCX2Eqip );
	}
}

#endif EQUIP_STRIP_TEST






void CX2ItemManager::CheckEmptyEqipPosition( bool* pViewEqipPosition, IN OUT vector<CX2EqipPtr>& viewEqipList )
{
	ZeroMemory( pViewEqipPosition, sizeof(bool) * 50 );
	for( int i =0; i < (int)viewEqipList.size(); ++i )
	{
		CX2EqipPtr pEqip = viewEqipList[i];
		CX2Item* pEqipItem = pEqip->GetItem();

		pViewEqipPosition[pEqipItem->GetItemTemplet()->GetEqipPosition()] = true;
	}
}

#ifdef  X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING

void CX2ItemManager::GetItemList( vector<int>& vecItemID, int page, int quantityPerPage )
{
    unsigned int uNumItems = GetItemNum();

    unsigned    uItemIndex = 0;

	int repeatNum = 0;
	while( repeatNum <= (page-1)*quantityPerPage )
	{
		++uItemIndex;
		repeatNum++;
	}
	
	repeatNum = 0;
    
    unsigned    uItemIndex2 = uItemIndex;
	while( repeatNum <= quantityPerPage - 1 )
	{
		++uItemIndex2;
		repeatNum++;
	}
	
	for ( ; uItemIndex < uNumItems && uItemIndex != uItemIndex2; uItemIndex++ )
	{
		vecItemID.push_back( GetItemIDFromIndex( uItemIndex ) );
	}
}


#else   //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING

void CX2ItemManager::GetItemList( vector<int>& vecItemID, int page, int quantityPerPage )
{
	map<int, const CX2Item::ItemTemplet*>::iterator mit;
	mit = m_ItemTempletIDMap.begin();

	int repeatNum = 0;
	while( repeatNum <= (page-1)*quantityPerPage )
	{
		mit++;
		repeatNum++;
	}
	
	repeatNum = 0;

	map<int, const CX2Item::ItemTemplet*>::iterator mit2 = mit;
	while( repeatNum <= quantityPerPage - 1 )
	{
		mit2++;
		repeatNum++;
	}
	
	for ( ; mit != m_ItemTempletIDMap.end() && mit != mit2; mit++ )
	{
		vecItemID.push_back( mit->first );
	}
}

#endif  //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING


#ifdef SERV_CUBE_OPEN_ED_CONDITION
bool CX2ItemManager::AddRandomItemTemplet_LUA( int iItemID_, int iKeyItemID_, int iRequiredKeyCount_, int iRequiredED_ )
{
	RandomItemData* pRandomItemData = new RandomItemData();

	pRandomItemData->m_ItemID			= iItemID_;
	pRandomItemData->m_KeyItemID		= iKeyItemID_;
	pRandomItemData->m_RequiredKeyCount	= iRequiredKeyCount_;
	pRandomItemData->m_iRequiredED		= iRequiredED_;

	//{{ kimhc // 2009-12-22 // 랜덤 아이템에 같은 키값 쓸수있도록 추가
#ifdef	MULTI_KEY_RANDOM_ITEM
	m_multimapRandomItem.insert( std::make_pair( pRandomItemData->m_ItemID, pRandomItemData ) );
#else	MULTI_KEY_RANDOM_ITEM
	m_mapRandomItem.insert( std::make_pair( pRandomItemData->m_ItemID, pRandomItemData ) );
#endif	MULTI_KEY_RANDOM_ITEM
	//}} kimhc // 2009-12-22 // 랜덤 아이템에 같은 키값 쓸수있도록 추가
	return true;
}
#else
bool CX2ItemManager::AddRandomItemTemplet_LUA( int iItemID, int iKeyItemID, int iRequiredKeyCount )
{
	RandomItemData* pRandomItemData = new RandomItemData();

	pRandomItemData->m_ItemID			= iItemID;
	pRandomItemData->m_KeyItemID		= iKeyItemID;
	pRandomItemData->m_RequiredKeyCount	= iRequiredKeyCount;

	//{{ kimhc // 2009-12-22 // 랜덤 아이템에 같은 키값 쓸수있도록 추가
#ifdef	MULTI_KEY_RANDOM_ITEM
	m_multimapRandomItem.insert( std::make_pair( pRandomItemData->m_ItemID, pRandomItemData ) );
#else	MULTI_KEY_RANDOM_ITEM
	m_mapRandomItem.insert( std::make_pair( pRandomItemData->m_ItemID, pRandomItemData ) );
#endif	MULTI_KEY_RANDOM_ITEM
	//}} kimhc // 2009-12-22 // 랜덤 아이템에 같은 키값 쓸수있도록 추가
	return true;
}
#endif //SERV_CUBE_OPEN_ED_CONDITION

//{{ kimhc // 2009-12-22 // 랜덤 아이템에 같은 키값 쓸수있도록 추가
#ifdef	MULTI_KEY_RANDOM_ITEM
std::pair< CX2ItemManager::RandomItemItr, CX2ItemManager::RandomItemItr > CX2ItemManager::GetRandomItemData( int itemID )
{
	return m_multimapRandomItem.equal_range( itemID );
}

bool CX2ItemManager::IsRandomItem( std::pair< CX2ItemManager::RandomItemItr, CX2ItemManager::RandomItemItr >& pairValue )
{
	if ( ( pairValue.first == m_multimapRandomItem.end() ) && ( pairValue.second == m_multimapRandomItem.end() ) 
		 || ( pairValue.first == pairValue.second ) )
		return false;
	else 
		return true;
}

#else	MULTI_KEY_RANDOM_ITEM
std::RandomItemData* CX2ItemManager::GetRandomItemData( int itemID )
{
	std::multimap< int, RandomItemData* >::iterator mit;
	mit = m_multimapRandomItem.find( itemID );
	if( mit == m_multimapRandomItem.end() )
	return NULL;

	return mit->second;
}
#endif	MULTI_KEY_RANDOM_ITEM
//}} kimhc // 2009-12-22 // 랜덤 아이템에 같은 키값 쓸수있도록 추가

CX2ItemManager::ManufactureData* CX2ItemManager::GetManufactureData( int ManufactureID )
{
	std::map< int, ManufactureData* >::iterator mit;
	mit = m_mapManufacture.find( ManufactureID );
	if( mit == m_mapManufacture.end() )
		return NULL;

	return mit->second;
}

bool CX2ItemManager::AddManufactureTemplet_LUA()
{
	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
#ifndef X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
	TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING

	ManufactureData* pManufactureData = new ManufactureData();
	
	int iManufactureID = 0;


#ifdef ADD_SERVER_GROUP
	int iServerGroupId = -1;
	LUA_GET_VALUE(		  luaManager, "m_iServerGroupID",	iServerGroupId,							-1 );
	if( iServerGroupId != -1 && iServerGroupId != g_pInstanceData->GetServerGroupID() )
	{
		SAFE_DELETE( pManufactureData );
		return false;
	}

	pManufactureData->m_iServerGroupID = iServerGroupId;
#endif
	
	LUA_GET_VALUE_RETURN( luaManager, "m_ManufactureID",	iManufactureID,							0,		goto LoadFail; );
	LUA_GET_VALUE_RETURN( luaManager, "m_ResultGroupID",	pManufactureData->m_ResultGroupID,		0,		goto LoadFail; );
	LUA_GET_VALUE(		  luaManager, "m_Cost",			pManufactureData->m_Cost,				0 );
	LUA_GET_VALUE(		  luaManager, "m_ManufactureType",	pManufactureData->m_ManufactureType,	-1 );
	

	if( luaManager.BeginTable( "m_MaterialTable" ) == true )
	{
		int index = 1;
		while( luaManager.BeginTable( index ) == true )
		{
			MaterialData materialData;
			LUA_GET_VALUE_RETURN( luaManager, "m_MaterialID",		materialData.m_MaterialItemID,		0,		goto LoadFail; );
			LUA_GET_VALUE_RETURN( luaManager, "m_MaterialCount",	materialData.m_MaterialCount,		0,		goto LoadFail; );

			if( materialData.m_MaterialItemID == 0 || materialData.m_MaterialCount == 0 )
			{
				goto LoadFail;
			}

			pManufactureData->m_vecMaterials.push_back( materialData );

			luaManager.EndTable();
			index++;
		}
		luaManager.EndTable();
	}
	else
	{		
		goto LoadFail;
	}

	
	m_mapManufacture.insert( std::make_pair( iManufactureID, pManufactureData ) );

	return true;

LoadFail:
	wstringstream wstrStream;
	wstrStream << L"ManufactureDataLoadFail_" << iManufactureID << L"_";

	ErrorLogMsg( XEM_ERROR105, wstrStream.str().c_str() );
	SAFE_DELETE( pManufactureData );
	return false;
}

//{{ kimhc // 2009-11-23 // 제조 아이템 NPC 별로 가능하도록
#ifdef	ADD_HOUSE_ID_TO_MANUFACTURE
bool CX2ItemManager::AddManufactureVillageData_LUA( int iManufactureID, int iHouseID )
{
	if( iManufactureID <= 0 || iHouseID <= 0 )
	{
		wstringstream wstrStream;
		wstrStream << L"ManufactureVaillageDataLoadFail_" << iManufactureID << L"_";

		ErrorLogMsg( XEM_ERROR105, wstrStream.str().c_str() );
		return false;
	}

#ifdef ADD_SERVER_GROUP
	int iServerGroup = 0;
	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );	
	LUA_GET_VALUE( luaManager, "m_iServerGroupID",	iServerGroup,	-1 );
	if( iServerGroup != -1 && iServerGroup != g_pInstanceData->GetServerGroupID() )
	{
		return false;
	}
#endif

	map<int, std::vector<int> >::iterator mit;
	mit = m_mapManufactureVillage.find( iHouseID );

	if( mit == m_mapManufactureVillage.end() )
	{
		std::vector< int > vecManufactureData;
		vecManufactureData.push_back( iManufactureID );
		m_mapManufactureVillage.insert( std::make_pair( iHouseID, vecManufactureData ) );
	}	
	else
	{
		std::vector< int >::iterator vit;
		vit = std::find( mit->second.begin(), mit->second.end(), iManufactureID );
		if( vit == mit->second.end() )
		{
			mit->second.push_back( iManufactureID );
		}
	}
	return true;
}
#else	ADD_HOUSE_ID_TO_MANUFACTURE
bool CX2ItemManager::AddManufactureVillageData_LUA( int iManufactureID, int iVillageMapID )
{
	if( iManufactureID <= 0 || iVillageMapID <= 0 )
	{
		wstringstream wstrStream;
		wstrStream << L"ManufactureVaillageDataLoadFail_" << iManufactureID << L"_";

		ErrorLogMsg( XEM_ERROR105, wstrStream.str().c_str() );
		return false;
	}

#ifdef ADD_SERVER_GROUP
	int iServerGroup = 0;
	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );	
	LUA_GET_VALUE( luaManager, "m_iServerGroupID",	iServerGroup,	-1 );
	if( iServerGroup != -1 && iServerGroup != g_pInstanceData->GetServerGroupID() )
	{
		return false;
	}
#endif

	map<int, std::vector<int> >::iterator mit;
	mit = m_mapManufactureVillage.find( iVillageMapID );
	if( mit == m_mapManufactureVillage.end() )
	{
		std::vector< int > vecManufactureData;
		vecManufactureData.push_back( iManufactureID );
		m_mapManufactureVillage.insert( std::make_pair( iVillageMapID, vecManufactureData ) );
	}
	else
	{
		std::vector< int >::iterator vit;
		vit = std::find( mit->second.begin(), mit->second.end(), iManufactureID );
		if( vit == mit->second.end() )
		{
			mit->second.push_back( iManufactureID );
		}
	}

	return true;
}
#endif	ADD_HOUSE_ID_TO_MANUFACTURE
//}} kimhc // 2009-11-23 // 제조 아이템 NPC 별로 가능하도록




bool CX2ItemManager::AddManufactureResultGroup( int iGroupID, int iItemID )
{
	map<int, ManufactureResultGroupData>::iterator it =
		m_mapManufactureResultGroupData.find( iGroupID );
	if( it != m_mapManufactureResultGroupData.end() )
	{
		it->second.m_vecResultGroupItemID.push_back( iItemID );
	}
	else
	{
		ManufactureResultGroupData resultData;
		resultData.m_vecResultGroupItemID.push_back( iItemID );
		m_mapManufactureResultGroupData.insert( std::make_pair( iGroupID, resultData ) );
	}

	return true;
}

#ifdef SERV_GLOBAL_BILLING
#ifdef SERV_WISH_LIST_NO_ITEM
bool CX2ItemManager::AddCashItem( vector< KBillProductInfo >& vecKBillProductInfo, std::set< int >& setWishListNoItemList )
#else	//SERV_WISH_LIST_NO_ITEM
bool CX2ItemManager::AddCashItem( vector< KBillProductInfo >& vecKBillProductInfo )
#endif //SERV_WISH_LIST_NO_ITEM
{
	bool bCheckError = false;

	for ( int i = 0; i < (int)vecKBillProductInfo.size(); i++ )
	{
		KBillProductInfo& kBillProductInfo = vecKBillProductInfo[i];

		int itemID = kBillProductInfo.m_iProductID; 

		map<int, CX2ItemManager::CashItem* >::iterator mit;
		mit = m_mapCashItem.find( itemID );

		if ( mit == m_mapCashItem.end() )
		{
			CX2ItemManager::CashItem* pCashItem = new CX2ItemManager::CashItem();
			pCashItem->m_ItemID = itemID;

			if ( IsShowPackageItem( itemID ) == true )
				pCashItem->m_bShow = true;
			else
				pCashItem->m_bShow = false;

			pCashItem->m_vecKBillProductInfo.push_back( kBillProductInfo );

#ifdef SERV_WISH_LIST_NO_ITEM
			set<int>::iterator sit;
			sit = setWishListNoItemList.find( itemID );
			if( sit != setWishListNoItemList.end() )
				pCashItem->m_bIsWishListNotEnable = true;
#endif SERV_WISH_LIST_NO_ITEM

			m_mapCashItem.insert( std::make_pair( itemID, pCashItem ) );
			m_vecCashItem.push_back( itemID );
		}
		else
		{
			CX2ItemManager::CashItem* pCashItem = mit->second;
			if ( pCashItem != NULL )
			{
				bool bOverlap = false;
				for ( int i = 0; i < (int)pCashItem->m_vecKBillProductInfo.size(); i++ )
				{
					KBillProductInfo& tempKBillProductInfo = pCashItem->m_vecKBillProductInfo[i];
					if ( tempKBillProductInfo.m_iProductNo == kBillProductInfo.m_iProductNo )
					{
						bOverlap = true;
						bCheckError = true;
						//에러 로그 찍어보실까낭
						WCHAR buffer[32];
						_itow(kBillProductInfo.m_iProductID, buffer, 10);
						ErrorLogMsg( KEM_ERROR354, buffer );
						break;
					}
				}

				if ( bOverlap == false )
				{
					pCashItem->m_vecKBillProductInfo.push_back( kBillProductInfo );
					std::sort( pCashItem->m_vecKBillProductInfo.begin(), pCashItem->m_vecKBillProductInfo.end(), CX2ItemManager::CCashProductSort() );
				}
			}
		}
	}

	if ( bCheckError == true )
		return false;

	return true;
}

bool CX2ItemManager::AddCashItemEx()
{
	m_vecNewCashItem.resize(0);
	m_vecHotCashItem.resize(0);
	m_vecRecommendCashItem.resize(0);
	m_vecLimitedCashItem.resize(0);
	m_vecEventCashItem.resize(0);

	//{{ robobeg : 2008-10-28
	//KLuaManager kLuamanager;
	KLuaManager kLuamanager( g_pKTDXApp->GetLuaBinder()->GetLuaState(), 0, true );
	//}} robobeg : 2008-10-28
	g_pKTDXApp->LoadAndDoMemory( &kLuamanager, L"CashShopItemList.lua");

#ifdef	CASH_SHOP_ICON_USE_PRODUCTNO
	if( kLuamanager.BeginTable( "FIRST_PAGE_ITEM_LIST" ) == true )
	{
		if( kLuamanager.BeginTable( "NEW_ITEM" ) == true )
		{
			int tableIndex = 1;
			int itemID = -1;
			while( kLuamanager.BeginTable( tableIndex ) == true )
			{
				int iProductNO = -1;

				LUA_GET_VALUE( kLuamanager, 1,	itemID, -1 );
				LUA_GET_VALUE( kLuamanager, 2,	iProductNO, -1 );

				CX2ItemManager::CashItem* pCashItem = GetCashItem( itemID );
				if ( pCashItem != NULL )
				{
					if ( pCashItem->m_bShow == true )
					{
						if ( pCashItem->m_vecKBillProductInfo.empty() == false )
						{
							pCashItem->m_bIsNew = true;
							if ( std::find( m_vecNewCashItem.begin(), m_vecNewCashItem.end(), itemID ) == m_vecNewCashItem.end() )
							{
								if( iProductNO == -1 )	//	ProductNO가 -1이면 무조건 등록
								{
									m_vecNewCashItem.push_back( itemID );
								}
								else
								{
									std::vector< KBillProductInfo >::iterator vit;

									for( vit = pCashItem->m_vecKBillProductInfo.begin(); vit != pCashItem->m_vecKBillProductInfo.end(); vit++ )
									{
										if( vit->m_iProductNo == iProductNO )
										{
											m_vecNewCashItem.push_back( itemID );
										}
									}
								}
							}
						}
					}
				}
				tableIndex++;
				kLuamanager.EndTable();
			}

			kLuamanager.EndTable();
		}

		if( kLuamanager.BeginTable( "HOT_ITEM" ) == true )
		{
			int tableIndex = 1;
			int itemID = -1;
			while( kLuamanager.BeginTable( tableIndex ) == true )
			{
				int iProductNO = -1;

				LUA_GET_VALUE( kLuamanager, 1,	itemID, -1 );
				LUA_GET_VALUE( kLuamanager, 2,	iProductNO, -1 );

				CX2ItemManager::CashItem* pCashItem = GetCashItem( itemID );
				if ( pCashItem != NULL )
				{
					if ( pCashItem->m_bShow == true )
					{	
						if ( pCashItem->m_vecKBillProductInfo.empty() == false )
						{
							pCashItem->m_bIsHot = true;
							if ( std::find( m_vecHotCashItem.begin(), m_vecHotCashItem.end(), itemID ) == m_vecHotCashItem.end() )
							{
								if( iProductNO == -1 )	//	ProductNO가 -1이면 무조건 등록
								{
									m_vecHotCashItem.push_back( itemID );
								}
								else
								{
									std::vector< KBillProductInfo >::iterator vit;

									for( vit = pCashItem->m_vecKBillProductInfo.begin(); vit != pCashItem->m_vecKBillProductInfo.end(); vit++ )
									{
										if( vit->m_iProductNo == iProductNO )
										{
											m_vecHotCashItem.push_back( itemID );
										}
									}
								}
							}
						}
					}
				}
				tableIndex++;
				kLuamanager.EndTable();
			}

			kLuamanager.EndTable();
		}

		if( kLuamanager.BeginTable( "RECOMMEND_ITEM" ) == true )
		{
			int tableIndex = 1;
			int itemID = -1;
			while( kLuamanager.BeginTable( tableIndex ) == true )
			{
				int iProductNO = -1;

				LUA_GET_VALUE( kLuamanager, 1,	itemID, -1 );
				LUA_GET_VALUE( kLuamanager, 2,	iProductNO, -1 );

				CX2ItemManager::CashItem* pCashItem = GetCashItem( itemID );
				if ( pCashItem != NULL )
				{
					if ( pCashItem->m_bShow == true )
					{
						if ( pCashItem->m_vecKBillProductInfo.empty() == false )
						{
							pCashItem->m_bIsRecommend = true;
							if ( std::find( m_vecRecommendCashItem.begin(), m_vecRecommendCashItem.end(), itemID ) == m_vecRecommendCashItem.end() )
							{
								if( iProductNO == -1 )	//	ProductNO가 -1이면 무조건 등록
								{
									m_vecRecommendCashItem.push_back( itemID );
								}
								else
								{
									std::vector< KBillProductInfo >::iterator vit;

									for( vit = pCashItem->m_vecKBillProductInfo.begin(); vit != pCashItem->m_vecKBillProductInfo.end(); vit++ )
									{
										if( vit->m_iProductNo == iProductNO )
										{
											m_vecRecommendCashItem.push_back( itemID );
										}
									}
								}
							}
						}
					}
				}
				tableIndex++;
				kLuamanager.EndTable();
			}

			kLuamanager.EndTable();
		}

		kLuamanager.EndTable();
	}

	if( kLuamanager.BeginTable( "ICON_ITEM_LIST" ) == true )
	{
		if( kLuamanager.BeginTable( "ICON_LIMITED_ITEM" ) == true )
		{
			int tableIndex = 1;
			int itemID = -1;
			while( kLuamanager.BeginTable( tableIndex ) == true )
			{
				int iProductNO = -1;

				LUA_GET_VALUE( kLuamanager, 1,	itemID, -1 );
				LUA_GET_VALUE( kLuamanager, 2,	iProductNO, -1 );

				CX2ItemManager::CashItem* pCashItem = GetCashItem( itemID );
				if ( pCashItem != NULL )
				{
					if ( pCashItem->m_bShow == true )
					{
						if ( pCashItem->m_vecKBillProductInfo.empty() == false )
						{
							pCashItem->m_bIsLimited = true;
							if ( std::find( m_vecLimitedCashItem.begin(), m_vecLimitedCashItem.end(), itemID ) == m_vecLimitedCashItem.end() )
							{
								if( iProductNO == -1 )	//	ProductNO가 -1이면 무조건 등록
								{
									m_vecLimitedCashItem.push_back( itemID );
								}
								else
								{
									std::vector< KBillProductInfo >::iterator vit;

									for( vit = pCashItem->m_vecKBillProductInfo.begin(); vit != pCashItem->m_vecKBillProductInfo.end(); vit++ )
									{
										if( vit->m_iProductNo == iProductNO )
										{
											m_vecLimitedCashItem.push_back( itemID );
										}
									}
								}
							}
						}
					}
				}
				tableIndex++;
				kLuamanager.EndTable();
			}
			kLuamanager.EndTable();
		}

		if( kLuamanager.BeginTable( "ICON_EVENT_ITEM" ) == true )
		{
			int tableIndex = 1;
			int itemID = -1;
			while( kLuamanager.BeginTable( tableIndex ) == true )
			{
				int iProductNO = -1;

				LUA_GET_VALUE( kLuamanager, 1,	itemID, -1 );
				LUA_GET_VALUE( kLuamanager, 2,	iProductNO, -1 );

				CX2ItemManager::CashItem* pCashItem = GetCashItem( itemID );
				if ( pCashItem != NULL )
				{
					if ( pCashItem->m_bShow == true )
					{
						if ( pCashItem->m_vecKBillProductInfo.empty() == false )
						{
							pCashItem->m_bIsLimited = true;
							if ( std::find( m_vecEventCashItem.begin(), m_vecEventCashItem.end(), itemID ) == m_vecEventCashItem.end() )
							{
								if( iProductNO == -1 )	//	ProductNO가 -1이면 무조건 등록
								{
									m_vecEventCashItem.push_back( itemID );
								}
								else
								{
									std::vector< KBillProductInfo >::iterator vit;

									for( vit = pCashItem->m_vecKBillProductInfo.begin(); vit != pCashItem->m_vecKBillProductInfo.end(); vit++ )
									{
										if( vit->m_iProductNo == iProductNO )
										{
											m_vecEventCashItem.push_back( itemID );
										}
									}
								}
							}
						}
					}
				}
				tableIndex++;
				kLuamanager.EndTable();
			}

			kLuamanager.EndTable();
		}

#ifdef CASH_ITEM_HOT_ICON
		if( kLuamanager.BeginTable( "ICON_HOT_ITEM" ) == true )
		{
			int tableIndex = 1;
			int itemID = -1;

			while( kLuamanager.BeginTable( tableIndex ) == true )
			{
				int iProductNO = -1;

				LUA_GET_VALUE( kLuamanager, 1,	itemID, -1 );
				LUA_GET_VALUE( kLuamanager, 2,	iProductNO, -1 );

				CX2ItemManager::CashItem* pCashItem = GetCashItem( itemID );
				if ( pCashItem != NULL )
				{
					if ( pCashItem->m_bShow == true && pCashItem->m_vecKBillProductInfo.empty() == false )
					{
						pCashItem->m_bIsLimited = true;
						if ( std::find( m_vecHotIconCashItem.begin(), m_vecHotIconCashItem.end(), itemID ) == m_vecHotIconCashItem.end() )
						{
							if( iProductNO == -1 )	//	ProductNO가 -1이면 무조건 등록
							{
								m_vecHotIconCashItem.push_back( itemID );
							}
							else
							{
								std::vector< KBillProductInfo >::iterator vit;

								for( vit = pCashItem->m_vecKBillProductInfo.begin(); vit != pCashItem->m_vecKBillProductInfo.end(); vit++ )
								{
									if( vit->m_iProductNo == iProductNO )
									{
										m_vecHotIconCashItem.push_back( itemID );
									}
								}
							}
						}
					}
				}
				tableIndex++;
				kLuamanager.EndTable();
			}
			kLuamanager.EndTable();
		}
#endif CASH_ITEM_HOT_ICON
		kLuamanager.EndTable();
	}
#else	CASH_SHOP_ICON_USE_PRODUCTNO
	if( kLuamanager.BeginTable( "NEW_ITEM" ) == true )
	{
		int tableIndex = 1;
		int itemID = -1;
		while( kLuamanager.GetValue( tableIndex, itemID ) == true )
		{
			CX2ItemManager::CashItem* pCashItem = GetCashItem( itemID );
			if ( pCashItem != NULL )
			{
				if ( pCashItem->m_bShow == true )
				{
					if ( pCashItem->m_vecKBillProductInfo.empty() == false )
					{
						pCashItem->m_bIsNew = true;
						if ( std::find( m_vecNewCashItem.begin(), m_vecNewCashItem.end(), itemID ) == m_vecNewCashItem.end() )
							m_vecNewCashItem.push_back( itemID );
					}
				}
			}

			tableIndex++;
		}

		kLuamanager.EndTable();
	}

	if( kLuamanager.BeginTable( "HOT_ITEM" ) == true )
	{
		int tableIndex = 1;
		int itemID = -1;
		while( kLuamanager.GetValue( tableIndex, itemID ) == true )
		{
			CX2ItemManager::CashItem* pCashItem = GetCashItem( itemID );
			if ( pCashItem != NULL )
			{
				if ( pCashItem->m_bShow == true )
				{	
					if ( pCashItem->m_vecKBillProductInfo.empty() == false )
					{
						pCashItem->m_bIsHot = true;
						if ( std::find( m_vecHotCashItem.begin(), m_vecHotCashItem.end(), itemID ) == m_vecHotCashItem.end() )
							m_vecHotCashItem.push_back( itemID );
					}
				}
			}
			tableIndex++;
		}

		kLuamanager.EndTable();
	}

	if( kLuamanager.BeginTable( "RECOMMEND_ITEM" ) == true )
	{
		int tableIndex = 1;
		int itemID = -1;
		while( kLuamanager.GetValue( tableIndex, itemID ) == true )
		{
			CX2ItemManager::CashItem* pCashItem = GetCashItem( itemID );
			if ( pCashItem != NULL )
			{
				if ( pCashItem->m_bShow == true )
				{
					if ( pCashItem->m_vecKBillProductInfo.empty() == false )
					{
						pCashItem->m_bIsRecommend = true;
						if ( std::find( m_vecRecommendCashItem.begin(), m_vecRecommendCashItem.end(), itemID ) == m_vecRecommendCashItem.end() )
							m_vecRecommendCashItem.push_back( itemID );
					}
				}
			}
			tableIndex++;
		}

		kLuamanager.EndTable();
	}

	if( kLuamanager.BeginTable( "ICON_LIMITED_ITEM" ) == true )
	{
		int tableIndex = 1;
		int itemID = -1;
		while( kLuamanager.GetValue( tableIndex, itemID ) == true )
		{
			CX2ItemManager::CashItem* pCashItem = GetCashItem( itemID );
			if ( pCashItem != NULL )
			{
				if ( pCashItem->m_bShow == true )
				{
					if ( pCashItem->m_vecKBillProductInfo.empty() == false )
					{
						pCashItem->m_bIsLimited = true;
						if ( std::find( m_vecLimitedCashItem.begin(), m_vecLimitedCashItem.end(), itemID ) == m_vecLimitedCashItem.end() )
							m_vecLimitedCashItem.push_back( itemID );
					}
				}
			}
			tableIndex++;
		}

		kLuamanager.EndTable();
	}

	if( kLuamanager.BeginTable( "ICON_EVENT_ITEM" ) == true )
	{
		int tableIndex = 1;
		int itemID = -1;
		while( kLuamanager.GetValue( tableIndex, itemID ) == true )
		{
			CX2ItemManager::CashItem* pCashItem = GetCashItem( itemID );
			if ( pCashItem != NULL )
			{
				if ( pCashItem->m_bShow == true )
				{
					if ( pCashItem->m_vecKBillProductInfo.empty() == false )
					{
						pCashItem->m_bIsLimited = true;
						if ( std::find( m_vecEventCashItem.begin(), m_vecEventCashItem.end(), itemID ) == m_vecEventCashItem.end() )
							m_vecEventCashItem.push_back( itemID );
					}
				}
			}
			tableIndex++;
		}

		kLuamanager.EndTable();
	}

#ifdef CASH_ITEM_HOT_ICON
	if( kLuamanager.BeginTable( "ICON_HOT_ITEM" ) == true )
	{
		int tableIndex = 1;
		int itemID = -1;
		while( kLuamanager.GetValue( tableIndex, itemID ) == true )
		{
			CX2ItemManager::CashItem* pCashItem = GetCashItem( itemID );
			if ( pCashItem != NULL )
			{
				if ( pCashItem->m_bShow == true )
				{
					if ( pCashItem->m_vecKBillProductInfo.empty() == false )
					{
						pCashItem->m_bIsLimited = true;
						if ( std::find( m_vecHotIconCashItem.begin(), m_vecHotIconCashItem.end(), itemID ) == m_vecHotIconCashItem.end() )
							m_vecHotIconCashItem.push_back( itemID );
					}
				}
			}
			tableIndex++;
		}

		kLuamanager.EndTable();
	}
#endif CASH_ITEM_HOT_ICON
#endif CASH_SHOP_ICON_USE_PRODUCTNO

	return true;
}
#else // SERV_GLOBAL_BILLING
bool CX2ItemManager::AddCashItem( vector< KNXBTProductInfo >& vecKNXBTProductInfo )
{
	bool bCheckError = false;

	for ( int i = 0; i < (int)vecKNXBTProductInfo.size(); i++ )
	{
		KNXBTProductInfo& kNXBTProductInfo = vecKNXBTProductInfo[i];

		int itemID = _wtoi( kNXBTProductInfo.m_wstrProductID.c_str() ); 

		map<int, CX2ItemManager::CashItem* >::iterator mit;
		mit = m_mapCashItem.find( itemID );

#ifdef CHECK_INVALIDE_CASH_SHOP_ITEM
		static bool bHasInvalideData = false;
		if( false == bHasInvalideData )
		{
			const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( itemID );
			if( pItemTemplet == NULL )
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), 
					GET_REPLACED_STRING( ( STR_ID_28249, "i", itemID ) ), g_pMain->GetNowState() );

				bHasInvalideData = true;
			}
		}
#endif // CHECK_INVALIDE_CASH_SHOP_ITEM
		if ( mit == m_mapCashItem.end() )
		{
			CX2ItemManager::CashItem* pCashItem = new CX2ItemManager::CashItem();
			pCashItem->m_ItemID = itemID;

			if ( IsShowPackageItem( itemID ) == true )
				pCashItem->m_bShow = true;
			else
				pCashItem->m_bShow = false;

#if defined(SERV_TOONILAND_CHANNELING) && !defined(SERV_STEAM)
			if( NULL != g_pData && NULL != g_pData->GetMyUser() )
			{
				if( g_pData->GetMyUser()->GetUserData().m_uChannelCode == 3 )
				{
					// 투니랜드 유저에게 보여지면 안되는 아이템
					if ( IsShowItemTooniland( itemID ) == true )
						pCashItem->m_bShow = true;
					else
						pCashItem->m_bShow = false;
				}
			}
#endif
			
			pCashItem->m_vecKNXBTProductInfo.push_back( kNXBTProductInfo );
			

			m_mapCashItem.insert( std::make_pair( itemID, pCashItem ) );
			m_vecCashItem.push_back( itemID );
		}
		else
		{
			CX2ItemManager::CashItem* pCashItem = mit->second;
			if ( pCashItem != NULL )
			{
				bool bOverlap = false;
				for ( int i = 0; i < (int)pCashItem->m_vecKNXBTProductInfo.size(); i++ )
				{
					KNXBTProductInfo& tempKNXBTProductInfo = pCashItem->m_vecKNXBTProductInfo[i];
					if ( tempKNXBTProductInfo.m_ulProductNo == kNXBTProductInfo.m_ulProductNo )
					{
						bOverlap = true;
						bCheckError = true;
						//에러 로그 찍어보실까낭
						ErrorLogMsg( KEM_ERROR354, kNXBTProductInfo.m_wstrProductID.c_str() );
						break;
					}
				}

				if ( bOverlap == false )
				{
					
					pCashItem->m_vecKNXBTProductInfo.push_back( kNXBTProductInfo );


					
					std::sort( pCashItem->m_vecKNXBTProductInfo.begin(), pCashItem->m_vecKNXBTProductInfo.end(), CX2ItemManager::CCashProductSort() );
				}
			}
		}
	}

	if ( bCheckError == true )
		return false;

	return true;
}

bool CX2ItemManager::AddCashItemEx()
{
	m_vecNewCashItem.resize(0);
	m_vecHotCashItem.resize(0);
	m_vecRecommendCashItem.resize(0);
	m_vecLimitedCashItem.resize(0);
	m_vecEventCashItem.resize(0);


//{{ robobeg : 2008-10-28
	//KLuaManager kLuamanager;
    KLuaManager kLuamanager( g_pKTDXApp->GetLuaBinder()->GetLuaState(), 0, true );
//}} robobeg : 2008-10-28
	g_pKTDXApp->LoadAndDoMemory( &kLuamanager, L"CashShopItemList.lua");

	if( kLuamanager.BeginTable( "NEW_ITEM" ) == true )
	{
		int tableIndex = 1;
		int itemID = -1;
		while( kLuamanager.GetValue( tableIndex, itemID ) == true )
		{
			CX2ItemManager::CashItem* pCashItem = GetCashItem( itemID );
			if ( pCashItem != NULL )
			{
				if ( pCashItem->m_bShow == true )
				{
					if ( pCashItem->m_vecKNXBTProductInfo.empty() == false )
					{
						pCashItem->m_bIsNew = true;
						if ( std::find( m_vecNewCashItem.begin(), m_vecNewCashItem.end(), itemID ) == m_vecNewCashItem.end() )
							m_vecNewCashItem.push_back( itemID );
					}
				}
			}
			
			tableIndex++;
		}

		kLuamanager.EndTable();
	}

	if( kLuamanager.BeginTable( "HOT_ITEM" ) == true )
	{
		int tableIndex = 1;
		int itemID = -1;
		while( kLuamanager.GetValue( tableIndex, itemID ) == true )
		{
			CX2ItemManager::CashItem* pCashItem = GetCashItem( itemID );
			if ( pCashItem != NULL )
			{
				if ( pCashItem->m_bShow == true )
				{	
					if ( pCashItem->m_vecKNXBTProductInfo.empty() == false )
					{
						pCashItem->m_bIsHot = true;
						if ( std::find( m_vecHotCashItem.begin(), m_vecHotCashItem.end(), itemID ) == m_vecHotCashItem.end() )
							m_vecHotCashItem.push_back( itemID );
					}
				}
			}
			tableIndex++;
		}

		kLuamanager.EndTable();
	}

	if( kLuamanager.BeginTable( "RECOMMEND_ITEM" ) == true )
	{
		int tableIndex = 1;
		int itemID = -1;
		while( kLuamanager.GetValue( tableIndex, itemID ) == true )
		{
			CX2ItemManager::CashItem* pCashItem = GetCashItem( itemID );
			if ( pCashItem != NULL )
			{
				if ( pCashItem->m_bShow == true )
				{
					if ( pCashItem->m_vecKNXBTProductInfo.empty() == false )
					{
						pCashItem->m_bIsRecommend = true;
						if ( std::find( m_vecRecommendCashItem.begin(), m_vecRecommendCashItem.end(), itemID ) == m_vecRecommendCashItem.end() )
							m_vecRecommendCashItem.push_back( itemID );
					}
				}
			}
			tableIndex++;
		}

		kLuamanager.EndTable();
	}


	if( kLuamanager.BeginTable( "ICON_LIMITED_ITEM" ) == true )
	{
		int tableIndex = 1;
		int itemID = -1;
		while( kLuamanager.GetValue( tableIndex, itemID ) == true )
		{
			CX2ItemManager::CashItem* pCashItem = GetCashItem( itemID );
			if ( pCashItem != NULL )
			{
				if ( pCashItem->m_bShow == true )
				{
					if ( pCashItem->m_vecKNXBTProductInfo.empty() == false )
					{
						pCashItem->m_bIsLimited = true;
						if ( std::find( m_vecLimitedCashItem.begin(), m_vecLimitedCashItem.end(), itemID ) == m_vecLimitedCashItem.end() )
							m_vecLimitedCashItem.push_back( itemID );
					}
				}
			}
			tableIndex++;
		}

		kLuamanager.EndTable();
	}




	if( kLuamanager.BeginTable( "ICON_EVENT_ITEM" ) == true )
	{
		int tableIndex = 1;
		int itemID = -1;
		while( kLuamanager.GetValue( tableIndex, itemID ) == true )
		{
			CX2ItemManager::CashItem* pCashItem = GetCashItem( itemID );
			if ( pCashItem != NULL )
			{
				if ( pCashItem->m_bShow == true )
				{
					if ( pCashItem->m_vecKNXBTProductInfo.empty() == false )
					{
						pCashItem->m_bIsLimited = true;
						if ( std::find( m_vecEventCashItem.begin(), m_vecEventCashItem.end(), itemID ) == m_vecEventCashItem.end() )
							m_vecEventCashItem.push_back( itemID );
					}
				}
			}
			tableIndex++;
		}

		kLuamanager.EndTable();
	}

#ifdef CASH_ITEM_HOT_ICON
	if( kLuamanager.BeginTable( "ICON_HOT_ITEM" ) == true )
	{
		int tableIndex = 1;
		int itemID = -1;
		while( kLuamanager.GetValue( tableIndex, itemID ) == true )
		{
			CX2ItemManager::CashItem* pCashItem = GetCashItem( itemID );
			if ( pCashItem != NULL )
			{
				if ( pCashItem->m_bShow == true )
				{
					if ( pCashItem->m_vecKNXBTProductInfo.empty() == false )
					{
						pCashItem->m_bIsLimited = true;
						if ( std::find( m_vecHotIconCashItem.begin(), m_vecHotIconCashItem.end(), itemID ) == m_vecHotIconCashItem.end() )
							m_vecHotIconCashItem.push_back( itemID );
					}
				}
			}
			tableIndex++;
		}

		kLuamanager.EndTable();
	}
#endif CASH_ITEM_HOT_ICON

#ifdef IMPOSSIBLE_PRESENT_CASH_ITEM
	if( kLuamanager.BeginTable( "IMPOSSIBLE_PRESENT_ITEM" ) == true )
	{
		int tableIndex = 1;
		int itemID = -1;
		while( kLuamanager.GetValue( tableIndex, itemID ) == true )
		{
			m_setImpossiblePresentItem.insert( itemID );
			tableIndex++;
		}
		kLuamanager.EndTable();
	}
#endif //IMPOSSIBLE_PRESENT_CASH_ITEM

	return true;
}
#endif // SERV_GLOBAL_BILLING

//{{ 2008. 11. 18  최육사
void CX2ItemManager::AddItemExchangeGroup_LUA( int iSourceItemID, int iResultItemID, float fRate )
{
	m_mapItemExchange.insert( std::make_pair( iSourceItemID, iSourceItemID ) );
}

bool CX2ItemManager::IsCanItemExchange( int iItemID )
{
	map< int, int >::const_iterator mit;
	mit = m_mapItemExchange.find( iItemID );
	if( mit == m_mapItemExchange.end() )
		return false;

	return true;
}
//}}

bool CX2ItemManager::GetIsUpdateCashItem()
{
	if ( m_mapCashItem.empty() == false )
		return true;

	return false;
}

void CX2ItemManager::ClearCashItemList()
{
	std::map< int, CashItem* >::iterator mit3;
	mit3 = m_mapCashItem.begin();
	for( mit3 = m_mapCashItem.begin(); mit3 != m_mapCashItem.end(); mit3++ )
	{		
		SAFE_DELETE( mit3->second );
	}
	m_mapCashItem.clear();
	m_vecCashItem.clear();
}

CX2ItemManager::CashItem* CX2ItemManager::GetCashItem( int itemID )
{
	CX2ItemManager::CashItem* pCashItem = NULL;

	map<int, CX2ItemManager::CashItem* >::iterator mit;
	mit = m_mapCashItem.find( itemID );

	if ( mit != m_mapCashItem.end() )
		pCashItem = mit->second;
	

	return pCashItem;
}

#ifdef SERV_GLOBAL_BILLING
wstring CX2ItemManager::GetCashItemPeriod( int iProductNo )
{
	map<int, CX2ItemManager::CashItem* >::iterator mit;

	for ( mit = m_mapCashItem.begin(); mit != m_mapCashItem.end(); mit++ )
	{
		CX2ItemManager::CashItem* pCashItem = mit->second;
		if ( pCashItem == NULL )
			continue;

		for ( int i = 0; i < (int)pCashItem->m_vecKBillProductInfo.size(); i++ )
		{
			KBillProductInfo& kKBillProductInfo = pCashItem->m_vecKBillProductInfo[i];
			if ( kKBillProductInfo.m_iProductNo == iProductNo )
			{
				return pCashItem->GetPeriod( kKBillProductInfo );
			}
		}
	}

	return L"";
}

KBillProductInfo* CX2ItemManager::GetCashItemProduct( int iProductNo )
{
	map<int, CX2ItemManager::CashItem* >::iterator mit;

	for ( mit = m_mapCashItem.begin(); mit != m_mapCashItem.end(); mit++ )
	{
		CX2ItemManager::CashItem* pCashItem = mit->second;
		if ( pCashItem == NULL )
			continue;

		for ( int i = 0; i < (int)pCashItem->m_vecKBillProductInfo.size(); i++ )
		{
			KBillProductInfo& kKBillProductInfo = pCashItem->m_vecKBillProductInfo[i];
			if ( kKBillProductInfo.m_iProductNo == iProductNo )
			{
				return &kKBillProductInfo;
			}
		}
	}

	return NULL;
}
#else // SERV_GLOBAL_BILLING
wstring CX2ItemManager::GetCashItemPeriod( unsigned long ulProductNo )
{
	map<int, CX2ItemManager::CashItem* >::iterator mit;

	for ( mit = m_mapCashItem.begin(); mit != m_mapCashItem.end(); mit++ )
	{
		CX2ItemManager::CashItem* pCashItem = mit->second;
		if ( pCashItem == NULL )
			continue;

		for ( int i = 0; i < (int)pCashItem->m_vecKNXBTProductInfo.size(); i++ )
		{
			KNXBTProductInfo& kKNXBTProductInfo = pCashItem->m_vecKNXBTProductInfo[i];
			if ( kKNXBTProductInfo.m_ulProductNo == ulProductNo )
			{
				return pCashItem->GetPeriod( kKNXBTProductInfo );
			}
		}
	}

	return L"";
}

KNXBTProductInfo* CX2ItemManager::GetCashItemProduct( unsigned long ulProductNo )
{
	map<int, CX2ItemManager::CashItem* >::iterator mit;

	for ( mit = m_mapCashItem.begin(); mit != m_mapCashItem.end(); mit++ )
	{
		CX2ItemManager::CashItem* pCashItem = mit->second;
		if ( pCashItem == NULL )
			continue;

		for ( int i = 0; i < (int)pCashItem->m_vecKNXBTProductInfo.size(); i++ )
		{
			KNXBTProductInfo& kKNXBTProductInfo = pCashItem->m_vecKNXBTProductInfo[i];
			if ( kKNXBTProductInfo.m_ulProductNo == ulProductNo )
			{
				return &kKNXBTProductInfo;
			}
		}
	}

	return NULL;
}
#endif // SERV_GLOBAL_BILLING

vector<int> CX2ItemManager::GetNewCashItemList()
{
	return m_vecNewCashItem;
}

vector<int> CX2ItemManager::GetHotCashItemList()
{
	return m_vecHotCashItem;
}

vector<int> CX2ItemManager::GetRecommendCashItemList()
{
	return m_vecRecommendCashItem;
}

vector<int> CX2ItemManager::GetLimitedCashItemList()
{
	return m_vecLimitedCashItem;
}

// 한정판매 캐시아이템인지 체크
bool CX2ItemManager::CheckLimitedCashItem( int itemID )
{
	if ( std::find( m_vecLimitedCashItem.begin(), m_vecLimitedCashItem.end(), itemID ) != m_vecLimitedCashItem.end() )
		return true;
	else
		return false;
}


bool CX2ItemManager::CheckEventCashItem( int itemID )
{
	if ( std::find( m_vecEventCashItem.begin(), m_vecEventCashItem.end(), itemID ) != m_vecEventCashItem.end() )
		return true;
	else
		return false;
}

#ifdef CASH_ITEM_HOT_ICON
bool CX2ItemManager::CheckHotIconCashItem( int itemID )
{
	if ( std::find( m_vecHotIconCashItem.begin(), m_vecHotIconCashItem.end(), itemID ) != m_vecHotIconCashItem.end() )
		return true;
	else
		return false;
}
#endif CASH_ITEM_HOT_ICON

vector<int> CX2ItemManager::GetAllCashItemList()
{
	vector< int > vecItemID = m_vecCashItem;;

	map<int, CX2ItemManager::CashItem* >::iterator mit;

	for ( int i = 0; i < (int)vecItemID.size(); i++ )
	{
		int itemID = vecItemID[i];

		mit = m_mapCashItem.find( itemID );
		if ( mit != m_mapCashItem.end() )
		{
			CX2ItemManager::CashItem* pCashItem = mit->second;
			if ( pCashItem == NULL )
				continue;

			if ( pCashItem->m_bShow == false )
			{
				vecItemID.erase( vecItemID.begin() + i );
				i--;
			}
		}
	}

	return vecItemID;
}

#ifndef	X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
bool CX2ItemManager::AddSetItemData_LUA()
{
	int setID = 0;
	CX2ItemManager::SetItemData* pSetItemData = NULL;
	int needPartsNum = 0;
	vector< int > options;
	bool bCheckNew = false;



	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
#ifndef X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
	TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING

	LUA_GET_VALUE(	luaManager, "m_SetID",						setID,					0	);

	if ( setID == 0 )
		return false;

	map< int, SetItemData* >::iterator mit = m_mapSetItemData.find( setID );
	if ( mit != m_mapSetItemData.end() )
	{
		pSetItemData = mit->second;
	}
	else
	{
		bCheckNew = true;
		pSetItemData = new CX2ItemManager::SetItemData();
		pSetItemData->m_SetID = setID;
	}


	//09. 05. 14 김정협 머지를 위해서 인덱스 읽는 부분 막음
	LUA_GET_VALUE(	luaManager, "m_SetName",					pSetItemData->m_SetName,				L""	);
	//LUA_GET_VALUE(	luaManager, "m_iSetName_Index",			m_nString_Index,		0);
	//pSetItemData->m_SetName = GET_SCRIPT_STRING(m_nString_Index);


	LUA_GET_VALUE(	luaManager, "m_NeedPartsNum",				needPartsNum,			0 );
	

	for ( int i = 1; i <= 15; i++ )
	{
		stringstream strstm;
		int optionNum;
		strstm << "m_Option" << i;
		LUA_GET_VALUE(	luaManager, strstm.str().c_str(),						optionNum,		0 );
		if ( optionNum != 0 )
		{
			options.push_back( optionNum );
		}
	}

	pSetItemData->m_mapNeedPartsNumNOptions.insert( std::make_pair( needPartsNum, options ) );


	if ( bCheckNew == true )
	{
		m_mapSetItemData.insert( std::make_pair( pSetItemData->m_SetID, pSetItemData ) );
	}

	return true;
}


const CX2ItemManager::SetItemData* CX2ItemManager::GetSetItem( int setID )
{
	map< int, SetItemData* >::iterator mit = m_mapSetItemData.find( setID );
	if ( mit != m_mapSetItemData.end() )
		return mit->second;

	return NULL;
}

int CX2ItemManager::GetSetItemOptions( const int iSetID_, const int iNumOfEquippedItems_, IN vector< int >& vecOptions )
{
	map< int, CX2ItemManager::SetItemData* >::iterator mit = m_mapSetItemData.find( iSetID_ );
	if ( mit != m_mapSetItemData.end() )
	{
		CX2ItemManager::SetItemData* pSetItemData = mit->second;
		if ( pSetItemData != NULL )
		{
			map< int, vector< int > >::iterator mit2;
			vector< int > tempVecOption;
			mit2 = pSetItemData->m_mapNeedPartsNumNOptions.begin();
			for ( ; mit2 != pSetItemData->m_mapNeedPartsNumNOptions.end(); mit2++ )
			{
				if ( mit2->first <= iNumOfEquippedItems_ )
				{
					tempVecOption = mit2->second;
				}
			}

			vecOptions.insert( vecOptions.end(), tempVecOption.begin(), tempVecOption.end() );
			
			//{{ kimhc // 2011-07-05 // 옵션데이타 수치화 작업
			return pSetItemData->m_iMaxLevel;
			//}} kimhc // 2011-07-05 // 옵션데이타 수치화 작업			
		}	
	}
	
	return 0;
}



void CX2ItemManager::GetSetItemList( int setID, vector< int >& vecItemID )
{
	map< int, std::vector<int> >::iterator mit = m_mapSetItemIDList.find( setID );
	if ( mit != m_mapSetItemIDList.end() )
	{
		vecItemID = mit->second;
	}
}

#endif  //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING

//{{ kimhc // 2009-11-23 // 제조 아이템 NPC 별로 가능하도록
#ifdef	ADD_HOUSE_ID_TO_MANUFACTURE
bool CX2ItemManager::GetManufactureVillageData( int iHouseID, std::vector< int >& vecManufactureVillage )
{
	map<int, std::vector<int> >::iterator mit;
	mit = m_mapManufactureVillage.find( iHouseID );
	if( mit == m_mapManufactureVillage.end() )
		return false;	

#ifdef SERV_ITEM_ACTION_BY_DBTIME_SETTING // 2012.12.12 lygan_조성욱 // 석근이 작업 리뉴얼 ( DB에서 실시간 값 반영, 교환, 제조 쪽도 적용 )
	std::vector< int > vecTempManufactureVillageData;
	std::set< int > setBanManufactureItem;
	if ( g_pData != NULL && g_pData->GetUIManager() != NULL )
	{
		setBanManufactureItem = g_pData->GetUIManager()->GetBanManufactureItemList();
	}

	BOOST_TEST_FOREACH(int , iManufactureItemID, mit->second )
	{
		if ( setBanManufactureItem.find( iManufactureItemID ) != setBanManufactureItem.end() )
			continue;
		
		vecTempManufactureVillageData.push_back(iManufactureItemID);
	}

	vecManufactureVillage = vecTempManufactureVillageData;
#else //SERV_ITEM_ACTION_BY_DBTIME_SETTING
	vecManufactureVillage = mit->second;
#endif //SERV_ITEM_ACTION_BY_DBTIME_SETTING
	
	return true;
}
#else	ADD_HOUSE_ID_TO_MANUFACTURE
bool CX2ItemManager::GetManufactureVillageData( int iVillageMapID, std::vector< int >& vecManufactureVillage )
{
	map<int, std::vector<int> >::iterator mit;
	mit = m_mapManufactureVillage.find( iVillageMapID );
	if( mit == m_mapManufactureVillage.end() )
		return false;

	vecManufactureVillage = mit->second;

	return true;
}
#endif	ADD_HOUSE_ID_TO_MANUFACTURE
//}} kimhc // 2009-11-23 // 제조 아이템 NPC 별로 가능하도록

const CX2ItemManager::ManufactureResultGroupData* CX2ItemManager::GetManufactureResultGroupData( int iResultGroupID )
{
	map<int, ManufactureResultGroupData>::iterator mit;
	mit = m_mapManufactureResultGroupData.find( iResultGroupID );
	if( mit != m_mapManufactureResultGroupData.end() )
	{
		return &mit->second;
	}

	return NULL;
}

#ifdef SERV_GLOBAL_BILLING
int CX2ItemManager::CashItem::GetCash()
{
	unsigned long cashPrice = 0;

#ifdef ADD_CASH_SHOP_CATEGORY_EVENT_2
	BOOST_TEST_FOREACH(const KBillProductInfo, kInfo, m_vecKBillProductInfo)
	{
		if( true == g_pInstanceData->IsCurrentSubCategoryInNowCatagory( kInfo.m_cCategoryNo))
		{
			cashPrice = kInfo.m_iSalePrice;
			break;
		}
	}
#else //ADD_CASH_SHOP_CATEGORY_EVENT_2
	const KBillProductInfo* pKBillProductInfo = GetGateProduct();
	if ( pKBillProductInfo != NULL )
	{
		cashPrice = pKBillProductInfo->m_iSalePrice;
	}
#endif //ADD_CASH_SHOP_CATEGORY_EVENT_2

	return cashPrice;
}

const KBillProductInfo* CX2ItemManager::CashItem::GetGateProduct()
{
	// 	for ( int i = 0; i < (int)m_vecKBillProductInfo.size(); i++ )
	// 	{
	// 		KBillProductInfo& kBillProductInfo = m_vecKBillProductInfo[i];
	// 		if ( kBillProductInfo.m_ulProductNo == kBillProductInfo.m_ulRelationProductNo )
	// 		{
	// 			return &kBillProductInfo;
	// 		}
	// 	}

	if ( m_vecKBillProductInfo.empty() == false )
	{
		KBillProductInfo& kBillProductInfo = m_vecKBillProductInfo[0];
		return &kBillProductInfo;
	}

	return NULL;
}

#ifdef ADD_CASH_SHOP_CATEGORY_EVENT_2
const std::vector<int> CX2ItemManager::CashItem::GetGateListOfProduct()
{
	std::vector<int> vecGateList;

	BOOST_TEST_FOREACH(const KBillProductInfo, kInfo, m_vecKBillProductInfo )
	{
		vecGateList.push_back(kInfo.m_cCategoryNo);
	}

	return vecGateList;
}
#endif //ADD_CASH_SHOP_CATEGORY_EVENT_2

#ifdef SERV_REAL_TIME_SALE_PERIOD_DESCRIPTION
wstring CX2ItemManager::CashItem::GetPeriod( KBillProductInfo& kKBillProductInfo, bool bShowSalePeriod /* = true */ )
{
	wstringstream wstrstm;

	if ( (int)kKBillProductInfo.m_cPeriod != 0 )
	{
		wstrstm << (int)kKBillProductInfo.m_cPeriod << GET_STRING( STR_ID_14 ) << L"/"; 
	}
	else
	{
		wstrstm << (int)kKBillProductInfo.m_cQuantity << GET_STRING( STR_ID_24 ) << L"/";
	}

	wstrstm << kKBillProductInfo.m_iSalePrice << GET_STRING( STR_ID_34 ); 

	if( bShowSalePeriod )
	{
		std::set< pair< __int64, bool > >::iterator sit;

		for( sit = kKBillProductInfo.m_setSalePeriod.begin(); sit != kKBillProductInfo.m_setSalePeriod.end(); ++sit )
		{
			CTime tCurrentTime = CTime::GetCurrentTime();

			if( tCurrentTime.GetTime() <= sit->first && sit->second == 0 )
			{
				CTime tEndTime( sit->first );
				std::wstring wstrEndTime = tEndTime.Format( _T( "%Y-%m-%d %H:%M" ) );
				wstrstm << GET_REPLACED_STRING( ( STR_ID_30118, "L", wstrEndTime ) );
				break;
			}
		}
	}

	return wstrstm.str().c_str();
}
#else
wstring CX2ItemManager::CashItem::GetPeriod( KBillProductInfo& kKBillProductInfo )
{
	wstringstream wstrstm;

	if ( (int)kKBillProductInfo.m_cPeriod != 0 )
	{
		wstrstm << (int)kKBillProductInfo.m_cPeriod << GET_STRING( STR_ID_14 ) << L"/"; 
	}
	else
	{
		wstrstm << (int)kKBillProductInfo.m_cQuantity << GET_STRING( STR_ID_24 ) << L"/";
	}

	wstrstm << kKBillProductInfo.m_iSalePrice << GET_STRING( STR_ID_34 ); 

	return wstrstm.str().c_str();
}
#endif SERV_REAL_TIME_SALE_PERIOD_DESCRIPTION

#else // SERV_GLOBAL_BILLING
int CX2ItemManager::CashItem::GetCash()
{
	unsigned long cashPrice = 0; //
	
	const KNXBTProductInfo* pKNXBTProductInfo = GetGateProduct();
	if ( pKNXBTProductInfo != NULL )
	{
		cashPrice = pKNXBTProductInfo->m_ulSalePrice;
	}

	return cashPrice;
}

const KNXBTProductInfo* CX2ItemManager::CashItem::GetGateProduct()
{
	

	for ( int i = 0; i < (int)m_vecKNXBTProductInfo.size(); i++ )
	{
		KNXBTProductInfo& kNXBTProductInfo = m_vecKNXBTProductInfo[i];
		if ( kNXBTProductInfo.m_ulProductNo == kNXBTProductInfo.m_ulRelationProductNo )
		{
			return &kNXBTProductInfo;
		}
	}

	if ( m_vecKNXBTProductInfo.empty() == false )
	{
		KNXBTProductInfo& kNXBTProductInfo = m_vecKNXBTProductInfo[0];
		return &kNXBTProductInfo;
	}

	return NULL;
}

int	CX2ItemManager::CashItem::GetGateProductIndex()
{
	for ( int i = 0; i < (int)m_vecKNXBTProductInfo.size(); i++ )
	{
		KNXBTProductInfo& kNXBTProductInfo = m_vecKNXBTProductInfo[i];
		if ( kNXBTProductInfo.m_ulProductNo == kNXBTProductInfo.m_ulRelationProductNo )
		{
			return i;
		}
	}

	if ( m_vecKNXBTProductInfo.empty() == false )
	{
		return 0;
	}

	return -1;
}

wstring CX2ItemManager::CashItem::GetPeriod( KNXBTProductInfo& kKNXBTProductInfo )
{
	wstringstream wstrstm;
	//wstrstm << kNXBTProductInfo

	if ( kKNXBTProductInfo.GetProductExpire() != 0 )
	{
        wstrstm << kKNXBTProductInfo.GetProductExpire() << GET_STRING( STR_ID_14 ) << L"/"; 
	}
	else
	{
		wstrstm << kKNXBTProductInfo.m_usProductPieces << GET_STRING( STR_ID_24 ) << L"/";
	}

	//{{ kimhc // 2009-10-21 // PaymentType이 자동결제면 '원' 이라는 글씨로 붙여주기
#ifdef	AUTO_PAYMENT
	if ( kKNXBTProductInfo.m_ulPaymentType == AUTO_PAYMENT_TYPE )
		wstrstm << kKNXBTProductInfo.m_ulSalePrice << GET_STRING( STR_ID_4634 ); 
	else
#endif	AUTO_PAYMENT
	//}} kimhc // 2009-10-21 // PaymentType이 자동결제면 '원' 이라는 글씨로 붙여주기
	wstrstm << kKNXBTProductInfo.m_ulSalePrice << GET_STRING( STR_ID_34 ); 

	return wstrstm.str().c_str();
}
#endif // SERV_GLOBAL_BILLING

bool CX2ItemManager::CashItem::GetIsRecommend()
{
	/*
	for ( int i = 0; i < (int)m_vecKNXBTProductInfo.size(); i++ )
	{
		KNXBTProductInfo& kNXBTProductInfo = m_vecKNXBTProductInfo[i];
		
		if ( kNXBTProductInfo.m_ulRelationProductNo == kNXBTProductInfo.m_ulProductNo )
			return true;
		else
			return false;
	}
	*/
	return m_bIsRecommend;
}
//{{ 2013.01.26 최민철 큐브패키지아이템 미리보기기능
#ifdef PACKAGE_IN_QUBE_PREVIEW
bool CX2ItemManager::AddCube_PackageData_LUA( int iCubePackageItemID, int iPackageItemID1, int iPackageItemID2, int iUnitType){
	
	if( iCubePackageItemID <= 0 || iPackageItemID1 <= 0 )
	{
		ErrorLog(KEM_ERROR355);
		return false;
	}	
	// 패키지 정보
	CubePackageData kCubePackageData;
	kCubePackageData.m_iPackageID1		   = iPackageItemID1;
	kCubePackageData.m_iPackageID2		   = iPackageItemID2 ;
	std::map< int, CubePackageData > mapCubePackageData;
	mapCubePackageData.insert(std::make_pair(iUnitType,kCubePackageData));
	
	std::map< int, std::map< int,CubePackageData > >::iterator mit;
	mit = m_mapCube_PackageData.find( iCubePackageItemID );
	if( mit == m_mapCube_PackageData.end() )
	{	
		//std::vector< CubePackageData > vecCubePackageData;
		//vecCubePackageData.push_back( kCubePackageData );
		m_mapCube_PackageData.insert(std::make_pair(iCubePackageItemID,mapCubePackageData));
	}
	else
	{
		mit->second.insert(std::make_pair(iUnitType,kCubePackageData));
	}
	
	return true;
}
#endif PACKAGE_IN_QUBE_PREVIEW
//}}

//{{ 2008. 4. 24  최육사  패키지 아이템
bool CX2ItemManager::AddPackageItemData_LUA( int iPackageItemID, int iItemID, unsigned short usProductPieces, bool bShowItem )
{
	if( iPackageItemID <= 0 || iItemID <= 0 )
	{
		ErrorLog(KEM_ERROR355);
		return false;
	}	

	// 패키지 정보
	PackageItemData kPackageItemData;
	kPackageItemData.m_iItemID		   = iItemID;
	kPackageItemData.m_usProductPieces = usProductPieces;
	kPackageItemData.m_bShowItem	   = bShowItem;

	std::map< int, std::vector< PackageItemData > >::iterator mit;
	mit = m_mapPackageData.find( iPackageItemID );
	if( mit == m_mapPackageData.end() )
	{
		std::vector< PackageItemData > vecPackageData;
		vecPackageData.push_back( kPackageItemData );
		m_mapPackageData.insert( std::make_pair( iPackageItemID, vecPackageData ) );
	}
	else
	{
		mit->second.push_back( kPackageItemData );
	}

	// 패키지 멤버 아이템 안보여주는 리스트 [key값은 중복되지 않으므로 바로 insert]
	if( !bShowItem )
	{
		m_setShowPackageItem.insert( iItemID );
	}

	return true;
}
//{{ 2013.01.26 최민철 큐브패키지아이템 미리보기기능
#ifdef PACKAGE_IN_QUBE_PREVIEW
bool CX2ItemManager::GetPackageCubeItemData( int iPackageItemID, std::vector< PackageItemData >& vecPackageData)
{
	vecPackageData.clear();

	std::map< int, std::map< int, CubePackageData > >::const_iterator mit;
	mit = m_mapCube_PackageData.find( iPackageItemID );
	if( mit == m_mapCube_PackageData.end() )
		return false;
	//큐브 패키지 아이템을 AddCubePackageData.lua 를 참조하여 아바타 패키지 ID로 변환
	std::map< int, std::vector< PackageItemData > >::const_iterator mit1;
	//CubePackageData kCubePackageData = mit->second;
	std::map<int,CubePackageData> vecCubePackageData = mit->second;
	int iCheakUnitPackageItemID = -1;
	int iUnitType = g_pData->GetMyUser()->GetSelectUnit()->GetType();
	std::map< int, CubePackageData >::const_iterator CPmit;
		CPmit = vecCubePackageData.find( iUnitType );
		if( CPmit == vecCubePackageData.end() )
			return false;

		if(CPmit->second.m_iPackageID1 == m_iLastPackacgeID){
			iCheakUnitPackageItemID = CPmit->second.m_iPackageID2;		
			m_iLastPackacgeID = iCheakUnitPackageItemID;
		}
		else{
			iCheakUnitPackageItemID = CPmit->second.m_iPackageID1;
			m_iLastPackacgeID = iCheakUnitPackageItemID;
		}

	mit1 = m_mapPackageData.find( iCheakUnitPackageItemID );
	if( mit1 == m_mapPackageData.end() )
		return false;

	vecPackageData = mit1->second;
	return true;
}
#endif PACKAGE_IN_QUBE_PREVIEW
//}} 2013.01.26 최민철 큐브패키지아이템 미리보기기능
bool CX2ItemManager::GetPackageData( int iPackageItemID, std::vector< PackageItemData >& vecPackageData )
{
	//{{ 2013.01.26 최민철 큐브패키지아이템 미리보기기능
#ifdef PACKAGE_IN_QUBE_PREVIEW
	m_iLastPackacgeID = 0;
#endif PACKAGE_IN_QUBE_PREVIEW
	//}} 2013.01.26 최민철 큐브패키지아이템 미리보기기능
	vecPackageData.clear();

	std::map< int, std::vector< PackageItemData > >::const_iterator mit;
	mit = m_mapPackageData.find( iPackageItemID );
	if( mit == m_mapPackageData.end() )
		return false;

	vecPackageData = mit->second;
	return true;
}

bool CX2ItemManager::IsShowPackageItem( int iItemID )
{
	std::set< int >::const_iterator sit;
	sit = m_setShowPackageItem.find( iItemID );
	if( sit == m_setShowPackageItem.end() )
		return true;
	
	return false;
}
//}}

#ifdef SERV_TOONILAND_CHANNELING
bool CX2ItemManager::IsShowItemTooniland( int iItemId )
{
	// 나중에 스크립트로 빼자...
	switch( iItemId )
	{
	case 131134:	// 코보 서포터 서비스	
		return false;
	default:
		break;
	}

	return true;
}
#endif //SERV_TOONILAND_CHANNELING

#ifndef	X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING

bool CX2ItemManager::TokenizeByScaleRotate( std::string& strTokenInfo, CX2Item::ItemTemplet* pItemTemplet, CX2Unit::UNIT_TYPE unitType )
{
	//## 문자열은 항상 EX.) SX:1.0 SY:1.0 SZ:1.0 RX:1.0 RY:1.0 RZ:1.0 으로 와야함.

	if( strTokenInfo.size() < 19 || pItemTemplet == NULL )
	{
		return false;
	}

	//## 0. Scale X
	//## 1. Scale Y
	//## 2. Scale Z
	//## 3. Rotate X
	//## 4. Rotate Y
	//## 5. Rotate Z
	std::string strToken[6];
	std::string strTemp;

	const char* pstr = strTokenInfo.c_str();

	while( (*pstr) != 0 )
	{
		switch( (*pstr) )
		{
		case 'R':
		case 'S':
		case 'X':
		case 'Y':
		case 'Z':
			{
				strTemp += (*pstr);
			}
			break;

		case ' ':
			{
				strTemp.clear();
			}
			break;

		default:
			{
				if( ((*pstr) >= '0' && (*pstr) <= '9') || (*pstr) == '.' || (*pstr) == '-' )
				{
					if( strTemp.compare("SX") == 0 )
						strToken[0] += (*pstr);
					else if( strTemp.compare("SY") == 0 )
						strToken[1] += (*pstr);
					else if( strTemp.compare("SZ") == 0 )
						strToken[2] += (*pstr);
					else if( strTemp.compare("RX") == 0 )
						strToken[3] += (*pstr);
					else if( strTemp.compare("RY") == 0 )
						strToken[4] += (*pstr);
					else if( strTemp.compare("RZ") == 0 )
						strToken[5] += (*pstr);
				}
			}
		}

		++pstr;
	}

	if( strToken[0].size() > 0 )
		pItemTemplet->m_RanderScale[unitType].x = static_cast<float>(atof( strToken[0].c_str() ));
	if( strToken[1].size() > 0 )
		pItemTemplet->m_RanderScale[unitType].y = static_cast<float>(atof( strToken[1].c_str() ));
	if( strToken[2].size() > 0 )
		pItemTemplet->m_RanderScale[unitType].z = static_cast<float>(atof( strToken[2].c_str() ));

	if( strToken[3].size() > 0 )
		pItemTemplet->m_RanderRotate[unitType].x = static_cast<float>((atof( strToken[3].c_str() ) / 180.0f) * D3DX_PI);
	if( strToken[4].size() > 0 )
		pItemTemplet->m_RanderRotate[unitType].y = static_cast<float>((atof( strToken[4].c_str() ) / 180.0f) * D3DX_PI);
	if( strToken[5].size() > 0 )
		pItemTemplet->m_RanderRotate[unitType].z = static_cast<float>((atof( strToken[5].c_str() ) / 180.0f) * D3DX_PI);

	return true;
}

#endif  //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING



#ifdef NEW_ITEM_EXCHANGE_TEST

bool CX2ItemManager::AddItemExchangeData( int iHOUSEID, ITEM_EXCHANGE_TYPE eExchangeType, int iSrcItemID, int iDestItemID, int iQuantity, float fDestItemRate /*= 0.f*/ )
{
	if( CX2LocationManager::HI_INVALID == iHOUSEID )
		return false;

	if( IET_INVALID == eExchangeType )
		return false;

#ifdef ADD_SERVER_GROUP
	int iServerGroup = 0;
	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );	
	LUA_GET_VALUE( luaManager, "m_iServerGroupID",	iServerGroup,	-1 );
	if( iServerGroup != -1 && iServerGroup != g_pInstanceData->GetServerGroupID() )
	{
		return false;
	}
#endif

#ifdef SERV_ITEM_EXCHANGE_NEW // 디파인 잘 못 둘러져있는 것 해외팀 수정
	int iSrcQuantity = 0;
	LUA_GET_VALUE( luaManager, "m_iSourceQuantity",	iSrcQuantity,	1 );
#endif SERV_ITEM_EXCHANGE_NEW

#ifdef SERV_EXCHANGE_PERIOD_ITEM
	short sPeriod = 0;
	LUA_GET_VALUE( luaManager, "m_sPeriod",			sPeriod,		0 );
#endif //SERV_EXCHANGE_PERIOD_ITEM


	//ItemExchangeData& itemExchangeData = m_mapItemExchangeData[ iHOUSEID ];

	std::map< int, vector< ItemExchangeData > >::iterator	mItrItemExchangeData;

	mItrItemExchangeData		=	m_mapItemExchangeData.find( iHOUSEID );

	if ( mItrItemExchangeData	== m_mapItemExchangeData.end() )
	{
		m_mapItemExchangeData.insert( std::make_pair( iHOUSEID, std::vector< ItemExchangeData >() ) );
		mItrItemExchangeData	=	m_mapItemExchangeData.find( iHOUSEID );
	}

	ItemExchangeData	itemExchangeData;

	itemExchangeData.m_eExchangeType	=	eExchangeType;
	itemExchangeData.m_iSrcItemID		=	iSrcItemID;
	itemExchangeData.m_iDestItemID		=	iDestItemID;
#ifdef SERV_ITEM_EXCHANGE_NEW // 디파인 잘 못 둘러져있는 것 해외팀 수정
	itemExchangeData.m_iSrcQuantity		=	iSrcQuantity;
#endif SERV_ITEM_EXCHANGE_NEW
	itemExchangeData.m_iQuantity		=	iQuantity;
#ifdef SERV_EXCHANGE_PERIOD_ITEM
	itemExchangeData.m_sPeriod			=	sPeriod;
#endif //SERV_EXCHANGE_PERIOD_ITEM

	mItrItemExchangeData->second.push_back( itemExchangeData );

	return true;
}

bool CX2ItemManager::GetItemExchangeData( int iHouseID, std::vector< ItemExchangeData >& vecItemExchangeData )
{
	if ( iHouseID == CX2LocationManager::HI_INVALID )
		return false;

	std::map< int, vector< ItemExchangeData > >::iterator	mItrItemExchangeData;
	mItrItemExchangeData = m_mapItemExchangeData.find( iHouseID );

	if ( mItrItemExchangeData == m_mapItemExchangeData.end () )
		return false;

#ifdef SERV_ITEM_ACTION_BY_DBTIME_SETTING // 2012.12.12 lygan_조성욱 // 석근이 작업 리뉴얼 ( DB에서 실시간 값 반영, 교환, 제조 쪽도 적용 )
	std::set< int > setBanExchangeItem;
	std::vector< ItemExchangeData > vecTempExchangeData;

	if ( g_pData != NULL && g_pData->GetUIManager() != NULL )
	{
		setBanExchangeItem = g_pData->GetUIManager()->GetBanExchangeItemList();
	}

	BOOST_TEST_FOREACH(ItemExchangeData , iExchangeItemID, mItrItemExchangeData->second )
	{
		if ( setBanExchangeItem.find( iExchangeItemID.m_iSrcItemID ) != setBanExchangeItem.end() )
			continue;

		vecTempExchangeData.push_back(iExchangeItemID);
	}

	vecItemExchangeData.reserve( vecTempExchangeData.size() );
	vecItemExchangeData = vecTempExchangeData;
#else //SERV_ITEM_ACTION_BY_DBTIME_SETTING
	vecItemExchangeData.reserve( mItrItemExchangeData->second.size() );

	vecItemExchangeData = mItrItemExchangeData->second;
#endif //SERV_ITEM_ACTION_BY_DBTIME_SETTING
	// 	std::copy( mItrItemExchangeData->second.begin(), 
	// 		mItrItemExchangeData->second.begin() + mItrItemExchangeData->second.size(), vecItemExchangeData.begin() );
	// 
	// 	if ( mItrItemExchangeData->second.size() !=	vecItemExchangeData.size() )	// 임시 테스트용
	// 	{
	// 		ASSERT( false );
	// 		return false;
	// 	}

	return true;	
}

#endif NEW_ITEM_EXCHANGE_TEST

//{{ kimhc // 2011-07-05 // 옵션데이타 수치화 작업


#ifndef X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING




void CX2ItemManager::UpdateSetIDAndMaxLevelMap( const int iSetID_, const int iLevel_ )
{
	SetIDAndMaxLevelMap::iterator mItr = m_mapSetIDAndMaxLevel.find( iSetID_ );

	// 없는 SetID 이면 추가
	if ( m_mapSetIDAndMaxLevel.end() == mItr )
	{
		m_mapSetIDAndMaxLevel.insert( SetIDAndMaxLevelMap::value_type( iSetID_, iLevel_ ) );
	}
	// 기존에 있는 SetID 이면
	else
	{
		// 기존의 레벨보다 크면 갱신
		if ( mItr->second < iLevel_ )
			mItr->second = iLevel_;
	}
}



void CX2ItemManager::UpdateMaxLevelToSetItemData()
{
    const SetIDAndMaxLevelMap& mapSetIDAndMaxLevel = m_mapSetIDAndMaxLevel;

	SetIDAndMaxLevelMap::const_iterator mItrSetIDAndMaxLevel = mapSetIDAndMaxLevel.begin();

	while ( mItrSetIDAndMaxLevel != mapSetIDAndMaxLevel.end() )
	{
		map< int, SetItemData* >::iterator mItrSetItemData 
			= m_mapSetItemData.find( mItrSetIDAndMaxLevel->first );

		//ASSERT( m_mapSetItemData.end() != mItrSetItemData );
		// SetItemData에 MaxLevel을 넣어줌
		if ( m_mapSetItemData.end() != mItrSetItemData )
			mItrSetItemData->second->m_iMaxLevel = mItrSetIDAndMaxLevel->second;

		++mItrSetIDAndMaxLevel;
	}
	m_mapSetIDAndMaxLevel.clear();
}
//}} kimhc // 2011-07-05 // 옵션데이타 수치화 작업

#endif  //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING


#ifdef COOLTIME_SHARE_GROUP_ITEM
void CX2ItemManager::AddCoolTimeGroupItem_LUA( int iCoolTimeGroupID, int iSourceItemID )
{
	m_mapCoolTimeGroupID.insert( std::make_pair( iSourceItemID, iCoolTimeGroupID ) );

	map< int, std::vector<int> >::iterator mit = m_mapCoolTimeGroupItemIDList.find( iCoolTimeGroupID );
	if ( mit != m_mapCoolTimeGroupItemIDList.end() )
	{
		mit->second.push_back( iSourceItemID );
	}
	else
	{
		vector< int > vecTempItemID;
		vecTempItemID.push_back( iSourceItemID );
		m_mapCoolTimeGroupItemIDList.insert( std::make_pair( iCoolTimeGroupID, vecTempItemID ) );
	}
	return;
}
//  SERV_CHANGE_QUICK_SLOT_COOL_TIME_DB_SP 작업으로 ItemID에서 GroupID로 변경됨. 변수명만 변경되서 디파인 적용하지 않음.
int CX2ItemManager::GetCoolTimeGroupID( int iGroupID ) const
{ 
	map< int, int >::const_iterator mit = m_mapCoolTimeGroupID.find( iGroupID );
	if ( mit != m_mapCoolTimeGroupID.end() )
	{
		return mit->second;
	}
#ifdef NO_GROUP_ITEM_SHOW_COOLTIME_ITME
	return iGroupID;// 등록된 그룹이 없으면 아이템 하나가 하나의 그룹이 되도록 한다
#else //NO_GROUP_ITEM_SHOW_COOLTIME_ITME
	return 0;
#endif //NO_GROUP_ITEM_SHOW_COOLTIME_ITME
}
#ifndef SERV_CHANGE_QUICK_SLOT_COOL_TIME_DB_SP
void CX2ItemManager::GetCoolTimeGroupItemIDList( int iCoolTimeGroupID, vector< int >& vecItemID ) const
{
	map< int, std::vector<int> >::const_iterator mit = m_mapCoolTimeGroupItemIDList.find( iCoolTimeGroupID );
	if ( mit != m_mapCoolTimeGroupItemIDList.end() )
	{
		vecItemID = mit->second;
	}
	return;
}
#endif //SERV_CHANGE_QUICK_SLOT_COOL_TIME_DB_SP
#endif COOLTIME_SHARE_GROUP_ITEM

//{{ 2011. 10. 18	최육사	배틀필드 아이템 레벨 시스템
#ifdef SERV_BATTLE_FIELD_ITEM_LEVEL
bool CX2ItemManager::SetItemLevelEnchantData_LUA( int iAtkPhysic, int iAtkMagic, int iDefPhysic, int iDefMagic, int iBaseHP )
{
	if( iAtkPhysic <= 0  ||  iAtkMagic <= 0  ||  iDefPhysic <= 0  ||  iDefMagic <= 0  ||  iBaseHP <= 0 )
	{
		START_LOG( cerr, L"스크립트 정보가 이상합니다!" )
			<< BUILD_LOG( iAtkPhysic )
			<< BUILD_LOG( iAtkMagic )
			<< BUILD_LOG( iDefPhysic )
			<< BUILD_LOG( iDefMagic )
			<< BUILD_LOG( iBaseHP )
			<< END_LOG;
		return false;
	}

	m_kItemLevelEnchantData.m_iAtkPhysic = iAtkPhysic;
	m_kItemLevelEnchantData.m_iAtkMagic = iAtkMagic;
	m_kItemLevelEnchantData.m_iDefPhysic = iDefPhysic;
	m_kItemLevelEnchantData.m_iDefMagic = iDefMagic;
	m_kItemLevelEnchantData.m_iBaseHP = iBaseHP;
	return true;
}

bool CX2ItemManager::GetItemLevel( IN const KItemInfo& kItemInfo, OUT int& iItemLevel )
{
	//const CXSLItem::ItemTemplet* pItemTemplet = GetItemTemplet( kItemInfo.m_iItemID );
	//if( pItemTemplet == NULL )
	//{
	//	START_LOG( cerr, L"아이템 정보가 존재하지 않습니다!" )
	//		<< BUILD_LOG( kItemInfo.m_iItemID )
	//		<< END_LOG;
	//	return false;
	//}

	//// 해당 아이템의 스탯
	//KStat kItemBaseStat;
	//kItemBaseStat.Init();
	//kItemBaseStat.m_iBaseHP    += (int)( pItemTemplet->m_Stat.m_fBaseHP );
	//kItemBaseStat.m_iAtkPhysic += (int)( pItemTemplet->m_Stat.m_fAtkPhysic );
	//kItemBaseStat.m_iAtkMagic  += (int)( pItemTemplet->m_Stat.m_fAtkMagic );
	//kItemBaseStat.m_iDefPhysic += (int)( pItemTemplet->m_Stat.m_fDefPhysic );
	//kItemBaseStat.m_iDefMagic  += (int)( pItemTemplet->m_Stat.m_fDefMagic );

	//// 강화 적용
	//float fEnchantRate;
	//if( SiCXSLEnchantItemManager()->GetEnchantRate( ( int )kItemInfo.m_cEnchantLevel, fEnchantRate ) == false )
	//{
	//	fEnchantRate = 1.f;
	//}

	//// BaseHP 스텟도 강화 적용 되게 변경
	//KStat kItemEnchantStat;
	//kItemEnchantStat.m_iBaseHP			= (int)( kItemBaseStat.m_iBaseHP	* fEnchantRate ) - kItemBaseStat.m_iBaseHP;
	//kItemEnchantStat.m_iAtkPhysic		= (int)( kItemBaseStat.m_iAtkPhysic	* fEnchantRate ) - kItemBaseStat.m_iAtkPhysic;
	//kItemEnchantStat.m_iAtkMagic		= (int)( kItemBaseStat.m_iAtkMagic	* fEnchantRate ) - kItemBaseStat.m_iAtkMagic;
	//kItemEnchantStat.m_iDefPhysic		= (int)( kItemBaseStat.m_iDefPhysic	* fEnchantRate ) - kItemBaseStat.m_iDefPhysic;
	//kItemEnchantStat.m_iDefMagic		= (int)( kItemBaseStat.m_iDefMagic	* fEnchantRate ) - kItemBaseStat.m_iDefMagic;

	//// 강화에 따른 아이템 레벨 정보 얻기
	//int iEnchantItemLevel = 0;
	//GetCaculatedItemLevelByEnchantStat( kItemEnchantStat, iEnchantItemLevel );
	\
	//// 아이템 레벨 합산 결과
	//iItemLevel = pItemTemplet->m_iItemLevel + iEnchantItemLevel;
	return true;
}

void CX2ItemManager::GetCaculatedItemLevelByEnchantStat( IN const KStat& kEnchantItemStat, OUT int& iItemLevel )
{
	iItemLevel = 0;

	iItemLevel += ( kEnchantItemStat.m_iBaseHP / m_kItemLevelEnchantData.m_iBaseHP );
	iItemLevel += ( kEnchantItemStat.m_iAtkPhysic / m_kItemLevelEnchantData.m_iAtkPhysic );
	iItemLevel += ( kEnchantItemStat.m_iAtkMagic / m_kItemLevelEnchantData.m_iAtkMagic );
	iItemLevel += ( kEnchantItemStat.m_iDefPhysic / m_kItemLevelEnchantData.m_iDefPhysic );
	iItemLevel += ( kEnchantItemStat.m_iDefMagic / m_kItemLevelEnchantData.m_iDefMagic );
}
#endif SERV_BATTLE_FIELD_ITEM_LEVEL
//}}


#ifdef ICE_HEATER_PRE_VIEW

#ifdef ICE_HEATER_RANDOM_PRE_VIEW // 김태환
bool CX2ItemManager::AddIceHeaterItemData_LUA( int iIceHeaterItemID_, int iItemID_, int eUnitType_, int iItemGroup )
#else //ICE_HEATER_RANDOM_PRE_VIEW
bool CX2ItemManager::AddIceHeaterItemData_LUA( int iIceHeaterItemID_, int iItemID_, int eUnitType_ )
#endif //ICE_HEATER_RANDOM_PRE_VIEW
{
	if( iIceHeaterItemID_ <= 0 || iItemID_ <= 0 )
	{
		return false;
	}	

	// 가열기 아이템 정보
	IceHeaterItemData IceHeaterData;
	IceHeaterData.m_iItemID = iItemID_;
	IceHeaterData.m_eUnitType = static_cast<CX2Unit::UNIT_TYPE>(eUnitType_);

#ifdef ICE_HEATER_RANDOM_PRE_VIEW // 김태환
	IceHeaterData.m_iItemGroup = iItemGroup;
#endif //ICE_HEATER_RANDOM_PRE_VIEW

	std::map< int, std::vector< IceHeaterItemData > >::iterator mit;
	mit = m_mapIceHeaterItemData.find( iIceHeaterItemID_ );
	if( mit == m_mapIceHeaterItemData.end() )
	{
		std::vector< IceHeaterItemData > vecIceHeaterItemData;
		vecIceHeaterItemData.push_back( IceHeaterData );
		m_mapIceHeaterItemData.insert( std::make_pair( iIceHeaterItemID_, vecIceHeaterItemData ) );
	}
	else
	{
		mit->second.push_back( IceHeaterData );
	}

	return true;
}
bool CX2ItemManager::GetIceHeaterItemData( int iIceHeaterItemID_, CX2Unit::UNIT_TYPE eUnitType_, OUT std::vector< int >& vecIceHeaterItemData_ )
{
	vecIceHeaterItemData_.clear();

	std::map< int, std::vector< IceHeaterItemData > >::const_iterator mit;
	mit = m_mapIceHeaterItemData.find( iIceHeaterItemID_ );
	if( mit == m_mapIceHeaterItemData.end() )
		return false;

#ifdef ICE_HEATER_RANDOM_PRE_VIEW // 김태환
	typedef std::map<int, vector<int>> IceHeaterGroupMap;

	IceHeaterGroupMap mapIceHeaterItemDataByGroub;	/// 그룹별 아이템 아이디 리스트 저장용 컨테이너

	IceHeaterGroupMap::iterator mitGroup;

	/// 그룹별로 분류하여 저장 시키자.
	BOOST_FOREACH( IceHeaterItemData ItemData, mit->second )
	{
		/// 해당 캐릭터 타입이 아니면, 패스
		if( eUnitType_ != ItemData.m_eUnitType )
			continue;

		/// 저장할 그룹이 이미 들어있는지 검사
		mitGroup = mapIceHeaterItemDataByGroub.find( ItemData.m_iItemGroup );

		if ( mitGroup == mapIceHeaterItemDataByGroub.end() )	/// 신규 그룹 추가
		{
			/// 아이디를 저장할 벡터 생성 후 삽입
			vector<int> vecItemID;
			vecItemID.push_back( ItemData.m_iItemID );

			/// 새로운 그룹 추가
			mapIceHeaterItemDataByGroub.insert( make_pair( ItemData.m_iItemGroup, vecItemID ) );
		}
		else	/// 이미 그룹이 추가 되어 있으면, 해당 그룹의 벡터에 아이디 추가
		{
			mitGroup->second.push_back( ItemData.m_iItemID );	/// 아이디 저장 벡터에만 추가
		}
	}

	if ( true == mapIceHeaterItemDataByGroub.empty() )	/// 저장한 아이템이 없으면, 패스
		return false;


	UINT uiMapSize	= mapIceHeaterItemDataByGroub.size();
	mitGroup		= mapIceHeaterItemDataByGroub.begin();

	if ( 1 == uiMapSize )	/// 그룹이 하나뿐 이라면, 첫번째 그룹 출력
	{
		vecIceHeaterItemData_ = mitGroup->second;			/// 출력할 아이템 설정
	}
	else	/// 그룹이 여러개라면, 렌덤으로 출력
	{
		UINT uiRandomIndex	= rand() % uiMapSize;
		UINT uiIndex		= 0;
		
		for ( mitGroup; mitGroup != mapIceHeaterItemDataByGroub.end(); ++mitGroup )
		{
			if ( uiRandomIndex == uiIndex)
				vecIceHeaterItemData_ = mitGroup->second;	/// 출력할 아이템 설정
			++uiIndex;
		}
	}

#else //ICE_HEATER_RANDOM_PRE_VIEW
	BOOST_FOREACH( IceHeaterItemData ItemData, mit->second )
	{
		if( eUnitType_ == ItemData.m_eUnitType )
		{
			vecIceHeaterItemData_.push_back ( ItemData.m_iItemID );
		}
	}
#endif //ICE_HEATER_RANDOM_PRE_VIEW

	return true;
}

#endif ICE_HEATER_PRE_VIEW


#ifdef PET_PREVIEW
bool CX2ItemManager::AddPetPreViewData_LUA( int iCashShopItemID_, int iPetUnitID_, PREVIEW_PET_TYPE ePetType_ /*= PPT_PET*/)
{
#ifdef _IN_HOUSE_
	// 오류 팝업 1회만 보여주도록 수정
	static bool bHasInvalidData = false;

	if( true == bHasInvalidData )
		return false;
#endif // _IN_HOUSE_

	if( iCashShopItemID_ <= 0 || iPetUnitID_ <= 0 )
	{
#ifdef _IN_HOUSE_		
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), 
								L"PetPreview.lua 오류 : 아이템ID 또는 펫 ID를 확인 해 주세요.", 
								(CKTDXStage*)g_pMain->GetNowState() );	

		bHasInvalidData = true;
#endif // _IN_HOUSE_
		return false;
	}

	// 패키지 정보
	m_vecPetPreViewData.push_back( PREVIEW_PET_DATA( iCashShopItemID_, iPetUnitID_, ePetType_ ) );

#ifdef _IN_HOUSE_		
	const CX2Item::ItemTemplet* pItemTemplet = GetItemTemplet( iCashShopItemID_ );
	if ( NULL == pItemTemplet )
	{
		WCHAR strTemp[255];
		StringCchPrintfW( strTemp, 255, L"PetPreview.lua 에러 : ItemTemplet 에서 %d ItemID 에 대한 정보를 찾을 수 없습니다.", iCashShopItemID_ );
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), strTemp, (CKTDXStage*)g_pMain->GetNowState() );	

		bHasInvalidData = true;
	}
	
	// Pet Unit ID 유효성 체크
	bool bIsValidPetUnitID = true;
	if ( NULL != g_pData->GetPetManager() )
		bIsValidPetUnitID = (NULL != g_pData->GetPetManager()->GetPetTemplet ( static_cast <CX2PetManager::PET_UNIT_ID> ( iPetUnitID_ ) ));
	
#ifdef RIDING_SYSTEM	// 엘소드 해외팀 추가
	if( false == bIsValidPetUnitID && NULL != CX2RidingPetManager::GetInstance() )
		bIsValidPetUnitID = (NULL != CX2RidingPetManager::GetInstance()->GetRidingPetTemplet( static_cast<CX2RidingPetManager::RIDING_PET_UNIT_ID>(iPetUnitID_)));
#endif //RIDING_SYSTEM	

	if( false == bIsValidPetUnitID )
	{
		WCHAR strTemp[255];
		StringCchPrintfW( strTemp, 255, L"PetPreview.lua 에러 : PetID %d에 대한 정보를 찾을 수 없습니다.", iPetUnitID_ );
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), strTemp, (CKTDXStage*)g_pMain->GetNowState() );	

		bHasInvalidData = true;
	}
#endif // _IN_HOUSE_
	
	return true;
}
int CX2ItemManager::GetPreviewPetIdByItemID( int iItemID_ )
{
	return GetPreviewIDByItemID( iItemID_, PPT_PET );
}
int CX2ItemManager::GetPreviewRidingPetIdByItemID( int iItemID_ )
{
	return GetPreviewIDByItemID( iItemID_, PPT_RIDING_PET );
}
int	CX2ItemManager::GetPreviewIDByItemID( int iItemID_, PREVIEW_PET_TYPE ePetType_)
{
	if( true == m_vecPetPreViewData.empty() )
		return -1;

	std::vector<int> vecPetID;
	BOOST_FOREACH( PREVIEW_PET_DATA PreviewData, m_vecPetPreViewData )
	{
		if( ePetType_ == PreviewData.ePreviewPetType )
		{

			if( iItemID_ == PreviewData.iCashShopItemID )
				vecPetID.push_back( PreviewData.iPreviewUnitID );
		}
	}

	if( true == vecPetID.empty() )
		return -1;

	int iPetIDIndex = rand() % vecPetID.size() ;
	return vecPetID[iPetIDIndex];
}
#endif // PET_PREVIEW


#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
void CX2ItemManager::AddItemConvertInfo_LUA()
{
	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
#ifndef X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
	TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING

	// NPC ID 파싱	
	CX2UnitManager::NPC_UNIT_ID eNpcUnitID = CX2UnitManager::NUI_NONE;
	LUA_GET_VALUE_ENUM(	luaManager, "m_ConvertNPC",	eNpcUnitID,	CX2UnitManager::NPC_UNIT_ID, CX2UnitManager::NUI_NONE );
	if( CX2UnitManager::NUI_NONE == eNpcUnitID )
	{
		eNpcUnitID = CX2UnitManager::NUI_ARIEL;
	}

	// 결과 아이템
	int iResultItemID = 0;
	if( luaManager.BeginTable( "m_ResultItemID" ) == true )
	{
		luaManager.GetValue( 1, iResultItemID);
		luaManager.EndTable();
	}

	if( 0 == iResultItemID )
		return;

	// 재료 아이템
	if( luaManager.BeginTable( "m_TargetItemID" ) == true )
	{
		std::map< int, ItemConvertList>::iterator it = m_mapNpcItemConvertList.find( static_cast<int>(eNpcUnitID));
		if( it != m_mapNpcItemConvertList.end() )
		{
			int index = 1;
			int buf;
			while( luaManager.GetValue(index++, buf) == true )
			{
				it->second.insert( std::make_pair(buf, iResultItemID) );
			}
			luaManager.EndTable();
		}
		else
		{
			int index = 1;
			int buf;
			ItemConvertList itConverList;
			while( luaManager.GetValue(index++, buf) == true )
			{
				itConverList.insert( std::make_pair(buf, iResultItemID) );
			}
			luaManager.EndTable();

			m_mapNpcItemConvertList.insert( std::make_pair( static_cast<int>(eNpcUnitID), itConverList ) );
		}
	}

	return;
}
int	CX2ItemManager::GetConvertResultItemID( int iTargetItemID_ )
{
	if( NULL != g_pTFieldGame )
	{
		int iNpcID = g_pTFieldGame->GetJoinNpcId();

		std::map< int, ItemConvertList>::iterator it = m_mapNpcItemConvertList.find(iNpcID);
		if( it != m_mapNpcItemConvertList.end() )
		{
			ItemConvertList::iterator itConverList = it->second.find( iTargetItemID_ );
			if( itConverList != it->second.end() )
				return itConverList->second;
		}
	}
	return 0;
}
#endif // SERV_NEW_ITEM_SYSTEM_2013_05

#ifdef MODIFY_FIND_SHOP_IMAGE_FILE_NAME
void CX2ItemManager::CreateShopImageNameList()
{
	std::ofstream ofs;
	ofs.open( "ShopImageName.txt" );
	if( false == ofs.is_open() )
		return;

	ofs.clear();

	stringstream strmResourceList;

	unsigned int uiNumItems = g_pData->GetItemManager()->GetItemNum();
	for( unsigned i = 0; i != uiNumItems; ++i )
	{		
		const CX2Item::ItemTemplet* pItemTemplet = NULL;

#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
		pItemTemplet = g_pData->GetItemManager()->GetItemTempletFromIndex( i );
#endif // SERV_NEW_ITEM_SYSTEM_2013_05

		strmResourceList << pItemTemplet->GetItemID();

		string deviceName = "";
		ConvertWCHARToChar( deviceName, pItemTemplet->GetName() );
		strmResourceList << " " << deviceName << " ";

		ConvertWCHARToChar( deviceName, pItemTemplet->GetShopImage() );
		strmResourceList << deviceName << std::endl;
	}

	ofs << strmResourceList.str();

	ofs.flush();
	ofs.close();
}
#endif // MODIFY_FIND_SHOP_IMAGE_FILE_NAME

#ifdef SERV_KEEP_ITEM_SHOW_CASHSHOP
bool CX2ItemManager::CheckKeepShowItem( int TempItemID , std::vector<int>& GetItemID )
{
	std::map<int,std::vector<int>>::iterator mit;
	mit = m_mapKeepShowItemList.find( TempItemID );
	if( mit != m_mapKeepShowItemList.end() )
	{
		GetItemID = mit->second;
		return true;
	}
	return false;
}

bool CX2ItemManager::CheckDisCountItemList( int TempProDuct ,bool& bIdexReset )
{
	int nCount = 0;
	std::map< int,kDisCountItemInfo >::iterator mit;
	for( mit = m_DisCountInfoMap.begin(); mit != m_DisCountInfoMap.end(); ++mit)
	{
		if( mit->second.m_DisCountProNo == TempProDuct )
		{
			for(int i = 0; i < mit->second.m_DisCountKeepItem.size(); ++i)
			{
				if( g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItemByTID(mit->second.m_DisCountKeepItem[i]) == NULL )
				{
					bIdexReset = true;
					return false;
				}
			}
			return true;
		}
		else if( mit->second.m_BasicProNo == TempProDuct )
		{
			for(int i = 0; i < mit->second.m_DisCountKeepItem.size(); ++i)
			{
				if( g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItemByTID(mit->second.m_DisCountKeepItem[i]) != NULL )
				{
					nCount++;
				}
			}
			if( nCount == mit->second.m_DisCountKeepItem.size() )
			{
				return false;
			}
			return true;
		}
	}
	return true;
}

bool CX2ItemManager::DisCountItemID_Find( int TempID, std::vector<int>& nTempList,int& nTempSale )
{
	std::map< int , kDisCountItemInfo >::iterator mit;
	mit = m_DisCountInfoMap.find( TempID );
	if( mit != m_DisCountInfoMap.end() )
	{
		nTempList = mit->second.m_DisCountKeepItem;
		nTempSale = mit->second.m_BasicProNo;
		return true;
	}
	return false;
}

int	CX2ItemManager::GetChangeCashPoint(int TempItemID)
{
	map<int, CX2ItemManager::CashItem* >::iterator mit;

	for ( mit = m_mapCashItem.begin(); mit != m_mapCashItem.end(); mit++ )
	{
		CX2ItemManager::CashItem* pCashItem = mit->second;
		if ( pCashItem == NULL )
			continue;
		if(pCashItem->GetGateProduct()->m_iProductNo == TempItemID)
		{
			return pCashItem->GetCash();
		}
		
	}
	return 0;
}
#endif SERV_KEEP_ITEM_SHOW_CASHSHOP

#ifdef SERV_REAL_TIME_SALE_PERIOD_DESCRIPTION
wstring CX2ItemManager::CashItem::GetSalePeriod( KBillProductInfo& kKBillProductInfo )
{
	wstringstream wstrstm;
	std::set< pair< __int64, bool > >::iterator sit;

	for( sit = kKBillProductInfo.m_setSalePeriod.begin(); sit != kKBillProductInfo.m_setSalePeriod.end(); ++sit )
	{
		CTime tCurrentTime = CTime::GetCurrentTime();

		if( tCurrentTime.GetTime() <= sit->first && sit->second == 0 )
		{
			CTime tEndTime( sit->first );

			if( (int)kKBillProductInfo.m_cPeriod != 0 )
			{
				wstrstm << (int)kKBillProductInfo.m_cPeriod << GET_STRING( STR_ID_14 ); 
			}
			else
			{
				wstrstm << (int)kKBillProductInfo.m_cQuantity << GET_STRING( STR_ID_24 );
			}

			std::wstring wstrEndTime = tEndTime.Format( _T( "/Sale Period : ~ %Y-%m-%d %H:%M:%S" ) );
			wstrstm << wstrEndTime;
			break;
		}
	}

	return wstrstm.str().c_str();
}
#endif SERV_REAL_TIME_SALE_PERIOD_DESCRIPTION