#include "StdAfx.h"
#include ".\x2itemtoolx2itemmanager.h"

CX2ItemManager::CX2ItemManager(void)
{
}

CX2ItemManager::~CX2ItemManager(void)
{
	CX2Item::ItemTemplet* pItem;
	map<int,CX2Item::ItemTemplet*>::iterator iter;

	iter = m_ItemTempletIDMap.begin();
	for( iter = m_ItemTempletIDMap.begin(); iter != m_ItemTempletIDMap.end(); ++iter )
	{
		pItem = iter->second;
		SAFE_DELETE( pItem );
	}
	m_ItemTempletIDMap.clear();
	m_ItemTempletNameMap.clear();
}

bool CX2ItemManager::OpenScriptFile( const WCHAR* pFileName )
{
	lua_tinker::decl( "g_pItemManager", this );

	KGCMassFileManager::CMassFile::MASSFILE_MEMBERFILEINFO_POINTER Info;

	Info = g_pItemToolCommon->GetMassFileManager()->LoadDataFile( pFileName );
	if( Info == NULL )
	{
		return false;
	}

	if( g_pItemToolCommon->GetLuaBinder()->DoMemory( Info->pRealData, Info->size ) == E_FAIL )
	{
		return false;
	}

	return true;
}

CX2Item::ItemTemplet* CX2ItemManager::GetItemTemplet( const WCHAR* pItemName )
{
	map<wstring,CX2Item::ItemTemplet*>::iterator iter;
	iter = m_ItemTempletNameMap.find( pItemName );
	if( iter == m_ItemTempletNameMap.end() )
		return NULL;

	CX2Item::ItemTemplet* pItem = iter->second;
	return pItem;
}

CX2Item::ItemTemplet* CX2ItemManager::GetItemTemplet( const int itemID )
{
	map<int,CX2Item::ItemTemplet*>::iterator iter;
	iter = m_ItemTempletIDMap.find( itemID );
	if( iter == m_ItemTempletIDMap.end() )
		return NULL;

	CX2Item::ItemTemplet* pItem = iter->second;
	return pItem;
}


bool CX2ItemManager::AddItemTemplet_LUA()
{
	KLuaManager luaManager( g_pItemToolCommon->GetLuaBinder()->GetLuaState() );
	TableBind( &luaManager, g_pItemToolCommon->GetLuaBinder() );

	CX2Item::ItemTemplet* pItemTemplet = new CX2Item::ItemTemplet();

	LUA_GET_VALUE_RETURN(	luaManager, L"m_ItemID",				pItemTemplet->m_ItemID,					0,		SAFE_DELETE(pItemTemplet); return false; );
	LUA_GET_VALUE_RETURN(	luaManager, L"m_Name",					pItemTemplet->m_Name,					L"",	SAFE_DELETE(pItemTemplet); return false; );
	LUA_GET_VALUE_RETURN(	luaManager, L"m_Description",			pItemTemplet->m_Description,			L"",	SAFE_DELETE(pItemTemplet); return false; );
	
	LUA_GET_VALUE( luaManager, L"m_ModelName",						pItemTemplet->m_ModelName,				L"" );
	LUA_GET_VALUE( luaManager, L"m_TextureChangeXETName",			pItemTemplet->m_TextureChangeXETName,	L"" );
	LUA_GET_VALUE( luaManager, L"m_AniXETName",						pItemTemplet->m_AniXETName,				L"" );
	LUA_GET_VALUE( luaManager, L"m_AniName",						pItemTemplet->m_AniName,				L"" );	
	LUA_GET_VALUE( luaManager, L"m_ShopImage",						pItemTemplet->m_ShopImage,				L"" );
	LUA_GET_VALUE( luaManager, L"m_DropViewer",						pItemTemplet->m_DropViewer,				L"" );
	
	LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"m_ItemType",			pItemTemplet->m_ItemType,				CX2Item::ITEM_TYPE,	CX2Item::IT_NONE, SAFE_DELETE(pItemTemplet); return false; );
	LUA_GET_VALUE_RETURN(		luaManager, L"m_bFashion",			pItemTemplet->m_bFashion,				false,	SAFE_DELETE(pItemTemplet); return false; );
	LUA_GET_VALUE_RETURN(		luaManager, L"m_bVested",			pItemTemplet->m_bVested,				false,	SAFE_DELETE(pItemTemplet); return false; );
	LUA_GET_VALUE_RETURN(		luaManager, L"m_bRare",				pItemTemplet->m_bRare,					false,	SAFE_DELETE(pItemTemplet); return false; );
	LUA_GET_VALUE_RETURN(		luaManager, L"m_bCanEnchant",		pItemTemplet->m_bCanEnchant,			false,	SAFE_DELETE(pItemTemplet); return false; );
	
	LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"m_EqipType",			pItemTemplet->m_EqipType,				CX2Item::EQIP_TYPE,	CX2Item::ET_NONE, SAFE_DELETE(pItemTemplet); return false; );
	LUA_GET_VALUE(				luaManager, L"m_AttachFrameName",	pItemTemplet->m_AttachFrameName,		L"" );
	LUA_GET_VALUE_RETURN(		luaManager, L"m_bCanHyperMode",		pItemTemplet->m_bCanHyperMode,			false,	SAFE_DELETE(pItemTemplet); return false; );
	
	LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"m_PeriodType",			pItemTemplet->m_PeriodType,				CX2Item::PERIOD_TYPE,		CX2Item::PT_INFINITY,	SAFE_DELETE(pItemTemplet); return false; );
	LUA_GET_VALUE_RETURN(		luaManager, L"m_Endurance",			pItemTemplet->m_Endurance,				0,		SAFE_DELETE(pItemTemplet); return false; );
	LUA_GET_VALUE_RETURN(		luaManager, L"m_Quantity",			pItemTemplet->m_Quantity,				0,		SAFE_DELETE(pItemTemplet); return false; );

	LUA_GET_VALUE_RETURN(		luaManager, L"m_bSale",				pItemTemplet->m_bSale,					false,	SAFE_DELETE(pItemTemplet); return false; );
	LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"m_PriceType",			pItemTemplet->m_PriceType,				CX2Item::SHOP_PRICE_TYPE,	CX2Item::SPT_NONE,		SAFE_DELETE(pItemTemplet); return false; );
	LUA_GET_VALUE_RETURN(		luaManager, L"m_Price",				pItemTemplet->m_Price,					0,		SAFE_DELETE(pItemTemplet); return false; );
	LUA_GET_VALUE_RETURN(		luaManager, L"m_PricePvPPoint",		pItemTemplet->m_PricePvPPoint,			0,		SAFE_DELETE(pItemTemplet); return false; );

	LUA_GET_VALUE_RETURN(		luaManager, L"m_bUseInUI",			pItemTemplet->m_bUseInUI,				false,	SAFE_DELETE(pItemTemplet); return false; );

	LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"m_EqipConditon",		pItemTemplet->m_EqipConditon,			CX2Item::EQIP_CONDITION,	CX2Item::EC_EVERYONE,			SAFE_DELETE(pItemTemplet); return false; );
	LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"m_UnitType",			pItemTemplet->m_UnitType,				CX2Unit::UNIT_TYPE,			CX2Unit::UT_ELSWORD,			SAFE_DELETE(pItemTemplet); return false; );
	LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"m_UnitClass",			pItemTemplet->m_UnitClass,				CX2Unit::UNIT_CLASS,		CX2Unit::UC_ELSWORD_SWORDMAN,	SAFE_DELETE(pItemTemplet); return false; );
	LUA_GET_VALUE_RETURN(		luaManager, L"m_EqipLevel",			pItemTemplet->m_EqipLevel,				0,		SAFE_DELETE(pItemTemplet); return false; );
	
	LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"m_EqipPosition",		pItemTemplet->m_EqipPosition,			CX2Unit::EQIP_POSITION,		CX2Unit::EP_QUICK_SLOT,	SAFE_DELETE(pItemTemplet); return false; );

	if( luaManager.BeginTable( "m_Stat" ) == true )
	{
		float fBuffer = 0.0f;

		LUA_GET_VALUE( luaManager, "m_fBaseHP",			fBuffer,			0 );
		pItemTemplet->m_Stat.m_fBaseHP = fBuffer;
		LUA_GET_VALUE( luaManager, "m_fAtkPhysic",		fBuffer,			0 );
		pItemTemplet->m_Stat.m_fAtkPhysic = fBuffer;
		LUA_GET_VALUE( luaManager, "m_fAtkMagic",		fBuffer,			0 );
		pItemTemplet->m_Stat.m_fAtkMagic = fBuffer;
		LUA_GET_VALUE( luaManager, "m_fAtkFire",		fBuffer,			0 );
		pItemTemplet->m_Stat.m_fAtkFire = fBuffer;
		LUA_GET_VALUE( luaManager, "m_fAtkIce",			fBuffer,			0 );
		pItemTemplet->m_Stat.m_fAtkIce = fBuffer;
		LUA_GET_VALUE( luaManager, "m_fAtkLightning",	fBuffer,			0 );
		pItemTemplet->m_Stat.m_fAtkLightning = fBuffer;
		LUA_GET_VALUE( luaManager, "m_fAtkEarth",		fBuffer,			0 );
		pItemTemplet->m_Stat.m_fAtkEarth = fBuffer;
		LUA_GET_VALUE( luaManager, "m_fAtkLight",		fBuffer,			0 );
		pItemTemplet->m_Stat.m_fAtkLight = fBuffer;
		LUA_GET_VALUE( luaManager, "m_fAtkDark",		fBuffer,			0 );
		pItemTemplet->m_Stat.m_fAtkDark = fBuffer;
		LUA_GET_VALUE( luaManager, "m_fAtkUniversal",	fBuffer,			0 );
		pItemTemplet->m_Stat.m_fAtkUniversal = fBuffer;

		LUA_GET_VALUE( luaManager, "m_fDefPhysic",		fBuffer,			0 );
		pItemTemplet->m_Stat.m_fDefPhysic = fBuffer;
		LUA_GET_VALUE( luaManager, "m_fDefMagic",		fBuffer,			0 );
		pItemTemplet->m_Stat.m_fDefMagic = fBuffer;
		LUA_GET_VALUE( luaManager, "m_fDefFire",		fBuffer,			0 );
		pItemTemplet->m_Stat.m_fDefFire = fBuffer;
		LUA_GET_VALUE( luaManager, "m_fDefIce",			fBuffer,			0 );
		pItemTemplet->m_Stat.m_fDefIce = fBuffer;
		LUA_GET_VALUE( luaManager, "m_fDefLightning",	fBuffer,			0 );
		pItemTemplet->m_Stat.m_fDefLightning = fBuffer;
		LUA_GET_VALUE( luaManager, "m_fDefEarth",		fBuffer,			0 );
		pItemTemplet->m_Stat.m_fDefEarth = fBuffer;
		LUA_GET_VALUE( luaManager, "m_fDefLight",		fBuffer,			0 );
		pItemTemplet->m_Stat.m_fDefLight = fBuffer;
		LUA_GET_VALUE( luaManager, "m_fDefDark",		fBuffer,			0 );
		pItemTemplet->m_Stat.m_fDefDark = fBuffer;
		LUA_GET_VALUE( luaManager, "m_fDefUniversal",	fBuffer,			0 );
		pItemTemplet->m_Stat.m_fDefUniversal = fBuffer;

		luaManager.EndTable();
	}
	else
	{
		SAFE_DELETE(pItemTemplet); 
		return false;
	}

	if( luaManager.BeginTable( "m_SpecialAbilityList" ) == true )
	{
		int index = 1;
		while( luaManager.BeginTable( index ) == true )
		{
			CX2Item::SpecialAbility sa;

			LUA_GET_VALUE_ENUM( luaManager, L"TYPE",		sa.m_Type,		CX2Item::SPECIAL_ABILITY_TYPE, CX2Item::SAT_NONE );
			LUA_GET_VALUE(		luaManager, L"COOL_TIME",	sa.m_CoolTime,	0 );
			LUA_GET_VALUE(		luaManager, L"VALUE1",		sa.m_Value1,	0 );
			LUA_GET_VALUE(		luaManager, L"VALUE2",		sa.m_Value2,	0 );
			LUA_GET_VALUE(		luaManager, L"VALUE3",		sa.m_Value3,	0 );

			pItemTemplet->m_SpecialAbilityList.push_back( sa );

			luaManager.EndTable();
			index++;
		}
		luaManager.EndTable();
	}
	else
	{
		SAFE_DELETE(pItemTemplet); 
		return false;
	}

	if( m_ItemTempletNameMap.find(pItemTemplet->m_Name) != m_ItemTempletNameMap.end()
		|| m_ItemTempletIDMap.find(pItemTemplet->m_ItemID) != m_ItemTempletIDMap.end() )
	{
		SAFE_DELETE(pItemTemplet); 
		return false;
	}
	else
	{
		m_ItemTempletNameMap.insert( std::make_pair(pItemTemplet->m_Name, pItemTemplet) );
		m_ItemTempletIDMap.insert( std::make_pair(pItemTemplet->m_ItemID, pItemTemplet) );
	}
	
	return true;
}


