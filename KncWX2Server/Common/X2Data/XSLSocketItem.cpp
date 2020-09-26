#include ".\xslsocketitem.h"
#include "neterror.h"
#include "X2Data/XSLItemManager.h"
#include "X2Data/XSLEnchantItemManager.h"

ImplementRefreshSingleton( CXSLSocketItem );

CXSLSocketItem::CXSLSocketItem(void)
{
}

CXSLSocketItem::~CXSLSocketItem(void)
{	
}

ImplToStringW( CXSLSocketItem )
{
	stm_	<< L"----------[ Socket Item ]----------" << std::endl
			<< TOSTRINGW( m_mapSocketData.size() )
			<< TOSTRINGW( m_mapSocketRandom.size() )
			<< TOSTRINGW( m_mapSocketGroup.size() )
			<< TOSTRINGW( m_mapSetItemData.size() )			
			//{{ 2013. 05. 15	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
			<< TOSTRINGW( m_mapRandomSocketGroup.size() )
			<< TOSTRINGW( m_mapMiracleCubeTable.size() )
#endif SERV_NEW_ITEM_SYSTEM_2013_05
			//}}
		;

	return stm_;
}

ImplementLuaScriptParser( CXSLSocketItem )
{
	lua_tinker::class_add<CXSLSocketItem>( GetLuaState(), "CXSLSocketItem" );
	lua_tinker::class_def<CXSLSocketItem>( GetLuaState(), "AddSocketData_LUA",		 &CXSLSocketItem::AddSocketData_LUA );
	lua_tinker::class_def<CXSLSocketItem>( GetLuaState(), "AddSocketRandomData",	 &CXSLSocketItem::AddSocketRandomData_LUA );
	lua_tinker::class_def<CXSLSocketItem>( GetLuaState(), "AddSocketGroup",			 &CXSLSocketItem::AddSocketGroup_LUA );
	lua_tinker::class_def<CXSLSocketItem>( GetLuaState(), "AddSetItemData_LUA",		 &CXSLSocketItem::AddSetItemData_LUA );
	//{{ 2012. 11. 28 캐시 아이템 소켓 옵션 기능 수정 - 김민성
#ifdef SERV_CASH_ITEM_SOCKET_OPTION
	lua_tinker::class_def<CXSLSocketItem>( GetLuaState(), "AddSocektGroupDataForCashAvatar_LUA",		&CXSLSocketItem::AddSocektGroupDataForCashAvatar_LUA );
#endif SERV_CASH_ITEM_SOCKET_OPTION
	//}}
	//{{ 2013. 05. 15	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
	lua_tinker::class_def<CXSLSocketItem>( GetLuaState(), "AddRandomSocketGroupInfo",	&CXSLSocketItem::AddRandomSocketGroupInfo_LUA );
	lua_tinker::class_def<CXSLSocketItem>( GetLuaState(), "AddMiracleCubeInfo",			&CXSLSocketItem::AddMiracleCubeInfo_LUA );
#endif SERV_NEW_ITEM_SYSTEM_2013_05
	//}}
	lua_tinker::class_def<CXSLSocketItem>( GetLuaState(), "dump",					 &CXSLSocketItem::Dump );

	lua_tinker::decl( GetLuaState(), "g_pCX2SocketItem", this );
	lua_tinker::decl( GetLuaState(), "g_pCX2SetItemManager", this );
}

const CXSLSocketItem::SocketData* CXSLSocketItem::GetSocketData( IN int iSocketOptionID ) const
{
	std::map< int, SocketData >::const_iterator mit;
	mit = m_mapSocketData.find( iSocketOptionID );
	if( mit == m_mapSocketData.end() )
		return NULL;

	return &mit->second;
}

int CXSLSocketItem::GetSocketCount( IN int iItemGrade, IN int iItemType ) const
{
	bool bIsWeapon = ( CXSLItem::IT_WEAPON == iItemType ? true : false );
	SOCKET_COUNT eSocketCount;
	switch( iItemGrade )
	{
	case CXSLItem::IG_LOW:
		if( bIsWeapon )
		{
			eSocketCount = SC_WEAPON_LOW;
		}
		else
		{
			eSocketCount = SC_DEFENCE_LOW;
		}
		break;

	case CXSLItem::IG_NORMAL:
		if( bIsWeapon )
		{
			eSocketCount = SC_WEAPON_NORMAL;
		}
		else
		{
			eSocketCount = SC_DEFENCE_NORMAL;
		}
		break;

	case CXSLItem::IG_RARE:
		if( bIsWeapon )
		{
			eSocketCount = SC_WEAPON_RARE;
		}
		else
		{
			eSocketCount = SC_DEFENCE_RARE;
		}
		break;

	case CXSLItem::IG_ELITE:
		if( bIsWeapon )
		{
			eSocketCount = SC_WEAPON_ELITE;
		}
		else
		{
			eSocketCount = SC_DEFENCE_ELITE;
		}
		break;

	case CXSLItem::IG_UNIQUE:
		if( bIsWeapon )
		{
			eSocketCount = SC_WEAPON_UNIQUE;
		}
		else
		{
			eSocketCount = SC_DEFENCE_UNIQUE;
		}
		break;

	default:
		START_LOG( cerr, L"존재하지 않는 ItemGrade입니다." )
			<< BUILD_LOG( iItemGrade )
			<< BUILD_LOG( bIsWeapon )
			<< END_LOG;

		eSocketCount = SC_NONE;
	}

	return static_cast<int>( eSocketCount );
}

bool CXSLSocketItem::VerifySocketCount( IN int iItemGrade, IN bool bIsWeapon, IN int iSocketCount ) const
{	
	switch( iItemGrade )
	{
	case CXSLItem::IG_LOW:
		if( bIsWeapon )
		{
			return (iSocketCount == (int)SC_WEAPON_LOW);
		}
		else
		{
			return (iSocketCount == (int)SC_DEFENCE_LOW);
		}
		break;

	case CXSLItem::IG_NORMAL:
		if( bIsWeapon )
		{
			return (iSocketCount == (int)SC_WEAPON_NORMAL);
		}
		else
		{
			return (iSocketCount == (int)SC_DEFENCE_NORMAL);
		}
		break;

	case CXSLItem::IG_RARE:
		if( bIsWeapon )
		{
			return (iSocketCount == (int)SC_WEAPON_RARE);
		}
		else
		{
			return (iSocketCount == (int)SC_DEFENCE_RARE);
		}
		break;

	case CXSLItem::IG_ELITE:
		if( bIsWeapon )
		{
			return (iSocketCount == (int)SC_WEAPON_ELITE);
		}
		else
		{
			return (iSocketCount == (int)SC_DEFENCE_ELITE);
		}
		break;

	case CXSLItem::IG_UNIQUE:
		if( bIsWeapon )
		{
			return (iSocketCount == (int)SC_WEAPON_UNIQUE);
		}
		else
		{
			return (iSocketCount == (int)SC_DEFENCE_UNIQUE);
		}
		break;

	default:
		START_LOG( cerr, L"존재하지 않는 ItemGrade입니다." )
			<< BUILD_LOG( iItemGrade )
			<< BUILD_LOG( bIsWeapon )
			<< BUILD_LOG( iSocketCount )
			<< END_LOG;
	}

	return false;
}

CXSLSocketItem::SOCKET_DATA_TYPE CXSLSocketItem::GetSocketDataType( IN int iItemID, IN int iItemType ) const
{
	SOCKET_DATA_TYPE eSocketDataType;
	switch( iItemID )
	{
	case MSI_MAGIC_STONE:
		if( iItemType == CXSLItem::IT_WEAPON )
		{
			eSocketDataType = SDT_WEAPON_NORMAL;
		}
		else
		{
			eSocketDataType = SDT_DEFENCE_NORMAL;
		}
		break;

	case MSI_RARE_MAGIC_STONE:
		if( iItemType == CXSLItem::IT_WEAPON )
		{
			eSocketDataType = SDT_WEAPON_RARE;
		}
		else
		{
			eSocketDataType = SDT_DEFENCE_RARE;
		}
		break;

		//{{ 2010. 04. 22  최육사	소켓 개편
	case MSI_HARMONY_MAGIC_STONE:
		if( iItemType == CXSLItem::IT_WEAPON )
		{
			eSocketDataType = SDT_WEAPON_HARMONY;
		}
		else
		{
			eSocketDataType = SDT_DEFENCE_HARMONY;
		}
		break;

	case MSI_MYSTERY_MAGIC_STONE:
		if( iItemType == CXSLItem::IT_WEAPON )
		{
			eSocketDataType = SDT_WEAPON_MYSTERY;
		}
		else
		{
			eSocketDataType = SDT_DEFENCE_MYSTERY;
		}
		break;

	case MSI_SHINE_MAGIC_STONE:
		if( iItemType == CXSLItem::IT_WEAPON )
		{
			eSocketDataType = SDT_WEAPON_SHINE;
		}
		else
		{
			eSocketDataType = SDT_DEFENCE_SHINE;
		}
		break;

	case MSI_BATTLE_MAGIC_STONE:
		if( iItemType == CXSLItem::IT_WEAPON )
		{
			eSocketDataType = SDT_WEAPON_BATTLE;
		}
		else
		{
			eSocketDataType = SDT_DEFENCE_BATTLE;
		}
		break;

	case MSI_INCREASE_MAGIC_STONE:
		if( iItemType == CXSLItem::IT_WEAPON )
		{
			eSocketDataType = SDT_WEAPON_INCREASE;
		}
		else
		{
			eSocketDataType = SDT_DEFENCE_INCREASE;
		}
		break;

	case MSI_HIT_MAGIC_STONE:
		if( iItemType == CXSLItem::IT_WEAPON )
		{
			eSocketDataType = SDT_WEAPON_HIT;
		}
		else
		{
			eSocketDataType = SDT_DEFENCE_HIT;
		}
		break;
		//}}
		//{{ 2010. 04. 30  최육사	소켓 개편
	case MSI_ATK_PHYSIC_MAGIC_STONE:
		if( iItemType == CXSLItem::IT_WEAPON )
		{
			eSocketDataType = SDT_WEAPON_ATK_PHYSIC;
		}
		else
		{
			eSocketDataType = SDT_DEFENCE_ATK_PHYSIC;
		}
		break;
	case MSI_ATK_MAGIC_MAGIC_STONE:
		if( iItemType == CXSLItem::IT_WEAPON )
		{
			eSocketDataType = SDT_WEAPON_ATK_MAGIC;
		}
		else
		{
			eSocketDataType = SDT_DEFENCE_ATK_MAGIC;
		}
		break;
	case MSI_DEF_PHYSIC_MAGIC_STONE:
		if( iItemType == CXSLItem::IT_WEAPON )
		{
			eSocketDataType = SDT_WEAPON_DEF_PHYSIC;
		}
		else
		{
			eSocketDataType = SDT_DEFENCE_DEF_PHYSIC;
		}
		break;
	case MSI_DEF_MAGIC_MAGIC_STONE:
		if( iItemType == CXSLItem::IT_WEAPON )
		{
			eSocketDataType = SDT_WEAPON_DEF_MAGIC;
		}
		else
		{
			eSocketDataType = SDT_DEFENCE_DEF_MAGIC;
		}
		break;
	case MSI_HYPER_TIME_MAGIC_STONE:
		if( iItemType == CXSLItem::IT_WEAPON )
		{
			eSocketDataType = SDT_WEAPON_HYPER_TIME;
		}
		else
		{
			eSocketDataType = SDT_DEFENCE_HYPER_TIME;
		}
		break;
	case MSI_INCREASE_HP_MAGIC_STONE:
		if( iItemType == CXSLItem::IT_WEAPON )
		{
			eSocketDataType = SDT_WEAPON_INCREASE_HP;
		}
		else
		{
			eSocketDataType = SDT_DEFENCE_INCREASE_HP;
		}
		break;
	case MSI_HYPER_CHARGE_MAGIC_STONE:
		if( iItemType == CXSLItem::IT_WEAPON )
		{
			eSocketDataType = SDT_WEAPON_HYPER_CHARGE;
		}
		else
		{
			eSocketDataType = SDT_DEFENCE_HYPER_CHARGE;
		}
		break;
	case MSI_SPEED_MAGIC_STONE:
		if( iItemType == CXSLItem::IT_WEAPON )
		{
			eSocketDataType = SDT_WEAPON_SPEED;
		}
		else
		{
			eSocketDataType = SDT_DEFENCE_SPEED;
		}
		break;
	case MSI_CRITICAL_MAGIC_STONE:
		if( iItemType == CXSLItem::IT_WEAPON )
		{
			eSocketDataType = SDT_WEAPON_CRITICAL;
		}
		else
		{
			eSocketDataType = SDT_DEFENCE_CRITICAL;
		}
		break;
	case MSI_MOVE_SPEED_MAGIC_STONE:
		if( iItemType == CXSLItem::IT_WEAPON )
		{
			eSocketDataType = SDT_WEAPON_MOVE_SPEED;
		}
		else
		{
			eSocketDataType = SDT_DEFENCE_MOVE_SPEED;
		}
		break;
	case MSI_STRIKE_MAGIC_STONE:
		if( iItemType == CXSLItem::IT_WEAPON )
		{
			eSocketDataType = SDT_WEAPON_STRIKE;
		}
		else
		{
			eSocketDataType = SDT_DEFENCE_STRIKE;
		}
		break;
	case MSI_AVOID_MAGIC_STONE:
		if( iItemType == CXSLItem::IT_WEAPON )
		{
			eSocketDataType = SDT_WEAPON_AVOID;
		}
		else
		{
			eSocketDataType = SDT_DEFENCE_AVOID;
		}
		break;
	case MSI_JUMP_SPEED_MAGIC_STONE:
		if( iItemType == CXSLItem::IT_WEAPON )
		{
			eSocketDataType = SDT_WEAPON_JUMP_SPEED;
		}
		else
		{
			eSocketDataType = SDT_DEFENCE_JUMP_SPEED;
		}
		break;
		//}}
		//{{ 2011. 07. 27 김민성   소켓 아이템 추가
	case MSI_INCREASE_DAMEGE_STONE:
		if( iItemType == CXSLItem::IT_WEAPON )
		{
			eSocketDataType = SDT_WEAPON_ADD_DAMAGE;
		}
		else
		{
			eSocketDataType = SDT_DEFENCE_ADD_DAMAGE;
		}
		break;
	case MSI_DECREASE_DAMEGE_STONE:
		if( iItemType == CXSLItem::IT_WEAPON )
		{
			eSocketDataType = SDT_WEAPON_ADD_DAMAGE_REDUCE;
		}
		else
		{
			eSocketDataType = SDT_DEFENCE_ADD_DAMAGE_REDUCE;
		}
		break;
		//}}
		//{{ 2011. 08. 30	최육사	듀얼 마법석 개편
#ifdef SERV_NEW_DUAL_MAGIC_STONE
	case MSI_DESTROY_MAGIC_STONE:
		if( iItemType == CXSLItem::IT_WEAPON )
		{
			eSocketDataType = SDT_WEAPON_DESTROY;
		}
		else
		{
			eSocketDataType = SDT_DEFENCE_DESTROY;
		}
		break;
	case MSI_EXPLOSION_MAGIC_STONE:
		if( iItemType == CXSLItem::IT_WEAPON )
		{
			eSocketDataType = SDT_WEAPON_EXPLOSION;
		}
		else
		{
			eSocketDataType = SDT_DEFENCE_EXPLOSION;
		}
		break;
	case MSI_BURN_MAGIC_STONE:
		if( iItemType == CXSLItem::IT_WEAPON )
		{
			eSocketDataType = SDT_WEAPON_BURN;
		}
		else
		{
			eSocketDataType = SDT_DEFENCE_BURN;
		}
		break;
	case MSI_QUICK_MAGIC_STONE:
		if( iItemType == CXSLItem::IT_WEAPON )
		{
			eSocketDataType = SDT_WEAPON_QUICK;
		}
		else
		{
			eSocketDataType = SDT_DEFENCE_QUICK;
		}
		break;
	case MSI_FURY_MAGIC_STONE:
		if( iItemType == CXSLItem::IT_WEAPON )
		{
			eSocketDataType = SDT_WEAPON_FURY;
		}
		else
		{
			eSocketDataType = SDT_DEFENCE_FURY;
		}
		break;
#endif SERV_NEW_DUAL_MAGIC_STONE
		//}}
#ifdef SERV_LURIEL_MAGIC_STONE
	case MSI_LURIEL_INCREASE_DAMEGE_STONE:
		if( iItemType == CXSLItem::IT_WEAPON )
		{
			eSocketDataType = SDT_WEAPON_ADD_DAMAGE;
		}
		else
		{
			eSocketDataType = SDT_DEFENCE_ADD_DAMAGE;
		}
		break;
	case MSI_LURIEL_DECREASE_DAMEGE_STONE:
		if( iItemType == CXSLItem::IT_WEAPON )
		{
			eSocketDataType = SDT_WEAPON_ADD_DAMAGE_REDUCE;
		}
		else
		{
			eSocketDataType = SDT_DEFENCE_ADD_DAMAGE_REDUCE;
		}
		break;
	case MSI_LURIEL_HYPER_TIME_MAGIC_STONE:
		if( iItemType == CXSLItem::IT_WEAPON )
		{
			eSocketDataType = SDT_WEAPON_HYPER_TIME;
		}
		else
		{
			eSocketDataType = SDT_DEFENCE_HYPER_TIME;
		}
		break;
	case MSI_LURIEL_INCREASE_HP_MAGIC_STONE:
		if( iItemType == CXSLItem::IT_WEAPON )
		{
			eSocketDataType = SDT_WEAPON_INCREASE_HP;
		}
		else
		{
			eSocketDataType = SDT_DEFENCE_INCREASE_HP;
		}
		break;
	case MSI_LURIEL_HYPER_CHARGE_MAGIC_STONE:
		if( iItemType == CXSLItem::IT_WEAPON )
		{
			eSocketDataType = SDT_WEAPON_HYPER_CHARGE;
		}
		else
		{
			eSocketDataType = SDT_DEFENCE_HYPER_CHARGE;
		}
		break;
	case MSI_LURIEL_SPEED_MAGIC_STONE:
		if( iItemType == CXSLItem::IT_WEAPON )
		{
			eSocketDataType = SDT_WEAPON_SPEED;
		}
		else
		{
			eSocketDataType = SDT_DEFENCE_SPEED;
		}
		break;		
	case MSI_LURIEL_CRITICAL_MAGIC_STONE:
		if( iItemType == CXSLItem::IT_WEAPON )
		{
			eSocketDataType = SDT_WEAPON_CRITICAL;
		}
		else
		{
			eSocketDataType = SDT_DEFENCE_CRITICAL;
		}
		break;	
	case MSI_LURIEL_MOVE_SPEED_MAGIC_STONE:
		if( iItemType == CXSLItem::IT_WEAPON )
		{
			eSocketDataType = SDT_WEAPON_MOVE_SPEED;
		}
		else
		{
			eSocketDataType = SDT_DEFENCE_MOVE_SPEED;
		}
		break;		
	case MSI_LURIEL_STRIKE_MAGIC_STONE:
		if( iItemType == CXSLItem::IT_WEAPON )
		{
			eSocketDataType = SDT_WEAPON_STRIKE;
		}
		else
		{
			eSocketDataType = SDT_DEFENCE_STRIKE;
		}
		break;		
	case MSI_LURIEL_AVOID_MAGIC_STONE:
		if( iItemType == CXSLItem::IT_WEAPON )
		{
			eSocketDataType = SDT_WEAPON_AVOID;
		}
		else
		{
			eSocketDataType = SDT_DEFENCE_AVOID;
		}
		break;	
	case MSI_LURIEL_JUMP_SPEED_MAGIC_STONE:
		if( iItemType == CXSLItem::IT_WEAPON )
		{
			eSocketDataType = SDT_WEAPON_JUMP_SPEED;
		}
		else
		{
			eSocketDataType = SDT_DEFENCE_JUMP_SPEED;
		}
		break;
	case MSI_LURIEL_DESTROY_MAGIC_STONE:
		if( iItemType == CXSLItem::IT_WEAPON )
		{
			eSocketDataType = SDT_WEAPON_DESTROY;
		}
		else
		{
			eSocketDataType = SDT_DEFENCE_DESTROY;
		}
		break;
	case MSI_LURIEL_EXPLOSION_MAGIC_STONE:
		if( iItemType == CXSLItem::IT_WEAPON )
		{
			eSocketDataType = SDT_WEAPON_EXPLOSION;
		}
		else
		{
			eSocketDataType = SDT_DEFENCE_EXPLOSION;
		}
		break;
	case MSI_LURIEL_BURN_MAGIC_STONE:
		if( iItemType == CXSLItem::IT_WEAPON )
		{
			eSocketDataType = SDT_WEAPON_BURN;
		}
		else
		{
			eSocketDataType = SDT_DEFENCE_BURN;
		}
		break;
	case MSI_LURIEL_QUICK_MAGIC_STONE:
		if( iItemType == CXSLItem::IT_WEAPON )
		{
			eSocketDataType = SDT_WEAPON_QUICK;
		}
		else
		{
			eSocketDataType = SDT_DEFENCE_QUICK;
		}
		break;
	case MSI_LURIEL_FURY_MAGIC_STONE:
		if( iItemType == CXSLItem::IT_WEAPON )
		{
			eSocketDataType = SDT_WEAPON_FURY;
		}
		else
		{
			eSocketDataType = SDT_DEFENCE_FURY;
		}
		break;
#endif //SERV_LURIEL_MAGIC_STONE

		//{{ 2012. 07. 09	김민성       현자의 마법석
//#ifdef SERV_DUNGEON_CLEAR_PAYMENT_STONE_EVENT
	case MSI_WISE_MAN_STONE_ITEM:
		if( iItemType == CXSLItem::IT_WEAPON )
		{
			eSocketDataType = SDT_WEAPON_SAGE;
		}
		else
		{
			eSocketDataType = SDT_DEFENCE_SAGE;
		}
		break;
//#endif SERV_DUNGEON_CLEAR_PAYMENT_STONE_EVENT
			//}}

		//{{ 2012. 09. 19	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
	case MSI_MOVE_SPEED_PVP:
		if( iItemType == CXSLItem::IT_WEAPON )
		{
			eSocketDataType = SDT_WEAPON_MOVE_SPEED_PVP;
		}
		else
		{
			eSocketDataType = SDT_DEFENCE_MOVE_SPEED_PVP;
		}
		break;

	case MSI_JUMP_SPEED_PVP:
		if( iItemType == CXSLItem::IT_WEAPON )
		{
			eSocketDataType = SDT_WEAPON_JUMP_SPEED_PVP;
		}
		else
		{
			eSocketDataType = SDT_DEFENCE_JUMP_SPEED_PVP;
		}
		break;

	case MSI_INCREASE_HP_PVP:
		if( iItemType == CXSLItem::IT_WEAPON )
		{
			eSocketDataType = SDT_WEAPON_INCREASE_HP_PVP;
		}
		else
		{
			eSocketDataType = SDT_DEFENCE_INCREASE_HP_PVP;
		}
		break;

	case MSI_STRIKE_PVP:
		if( iItemType == CXSLItem::IT_WEAPON )
		{
			eSocketDataType = SDT_WEAPON_STRIKE_PVP;
		}
		else
		{
			eSocketDataType = SDT_DEFENCE_STRIKE_PVP;
		}
		break;

	case MSI_AVOID_PVP:
		if( iItemType == CXSLItem::IT_WEAPON )
		{
			eSocketDataType = SDT_WEAPON_AVOID_PVP;
		}
		else
		{
			eSocketDataType = SDT_DEFENCE_AVOID_PVP;
		}
		break;

	case MSI_HYPER_CHARGE_PVP:
		if( iItemType == CXSLItem::IT_WEAPON )
		{
			eSocketDataType = SDT_WEAPON_HYPER_CHARGE_PVP;
		}
		else
		{
			eSocketDataType = SDT_DEFENCE_HYPER_CHARGE_PVP;
		}
		break;

	case MSI_HYPER_TIME_PVP:
		if( iItemType == CXSLItem::IT_WEAPON )
		{
			eSocketDataType = SDT_WEAPON_HYPER_TIME_PVP;
		}
		else
		{
			eSocketDataType = SDT_DEFENCE_HYPER_TIME_PVP;
		}
		break;

	case MSI_CRITICAL_PVP:
		if( iItemType == CXSLItem::IT_WEAPON )
		{
			eSocketDataType = SDT_WEAPON_CRITICAL_PVP;
		}
		else
		{
			eSocketDataType = SDT_DEFENCE_CRITICAL_PVP;
		}
		break;

	case MSI_SPEED_PVP:
		if( iItemType == CXSLItem::IT_WEAPON )
		{
			eSocketDataType = SDT_WEAPON_SPEED_PVP;
		}
		else
		{
			eSocketDataType = SDT_DEFENCE_SPEED_PVP;
		}
		break;

	case MSI_ADD_DAMAGE_PVP:
		if( iItemType == CXSLItem::IT_WEAPON )
		{
			eSocketDataType = SDT_WEAPON_ADD_DAMAGE_PVP;
		}
		else
		{
			eSocketDataType = SDT_DEFENCE_ADD_DAMAGE_PVP;
		}
		break;

	case MSI_ADD_DAMAGE_REDUCE_PVP:
		if( iItemType == CXSLItem::IT_WEAPON )
		{
			eSocketDataType = SDT_WEAPON_ADD_DAMAGE_REDUCE_PVP;
		}
		else
		{
			eSocketDataType = SDT_DEFENCE_ADD_DAMAGE_REDUCE_PVP;
		}
		break;

	case MSI_EXPLOSION_PVP:
		if( iItemType == CXSLItem::IT_WEAPON )
		{
			eSocketDataType = SDT_WEAPON_EXPLOSION_PVP;
		}
		else
		{
			eSocketDataType = SDT_DEFENCE_EXPLOSION_PVP;
		}
		break;

	case MSI_DESTROY_PVP:
		if( iItemType == CXSLItem::IT_WEAPON )
		{
			eSocketDataType = SDT_WEAPON_DESTROY_PVP;
		}
		else
		{
			eSocketDataType = SDT_DEFENCE_DESTROY_PVP;
		}
		break;

	case MSI_QUICK_PVP:
		if( iItemType == CXSLItem::IT_WEAPON )
		{
			eSocketDataType = SDT_WEAPON_QUICK_PVP;
		}
		else
		{
			eSocketDataType = SDT_DEFENCE_QUICK_PVP;
		}
		break;

	case MSI_BURN_PVP:
		if( iItemType == CXSLItem::IT_WEAPON )
		{
			eSocketDataType = SDT_WEAPON_BURN_PVP;
		}
		else
		{
			eSocketDataType = SDT_DEFENCE_BURN_PVP;
		}
		break;

	case MSI_FURY_PVP:
		if( iItemType == CXSLItem::IT_WEAPON )
		{
			eSocketDataType = SDT_WEAPON_FURY_PVP;
		}
		else
		{
			eSocketDataType = SDT_DEFENCE_FURY_PVP;
		}
		break;
#endif SERV_2012_PVP_SEASON2
		//}}
		//{{ 2012. 12. 11  고정 효과 마법석 추가 - 김민성
#ifdef SERV_FIXED_EFFECTS_SOCKET_ITEM
	case MSI_SPEED_MAGIC_STONE_FIXED_EFFECT:
		if( iItemType == CXSLItem::IT_WEAPON )
		{
			eSocketDataType = SDT_WEAPON_SPEED_5P_FIXED;
		}
		else
		{
			eSocketDataType = SDT_DEFFENCE_SPEED_2P5_FIXED;
		}
		break;
	case MSI_CRITICAL_MAGIC_STONE_FIXED_EFFECT:
		if( iItemType == CXSLItem::IT_WEAPON )
		{
			eSocketDataType = SDT_WEAPON_CRITICAL_5P_FIXED;
		}
		else
		{
			eSocketDataType = SDT_DEFFENCE_CRITICAL_2P5_FIXED;
		}
		break;
	case MSI_AVOID_MAGIC_STONE_FIXED_EFFECT:
		if( iItemType == CXSLItem::IT_WEAPON )
		{
			eSocketDataType = SDT_WEAPON_AVOID_5P_FIXED;
		}
		else
		{
			eSocketDataType = SDT_DEFFECNE_AVOID_2P5_FIXED;
		}
		break;
	case MSI_INCREASE_DAMEGE_STONE_FIXED_EFFECT:
		if( iItemType == CXSLItem::IT_WEAPON )
		{
			eSocketDataType = SDT_WEAPON_ADD_DAMAGE_5P_FIXED;
		}
		else
		{
			eSocketDataType = SDT_DEFFENCE_ADD_DAMAGE_2P5_FIXED;
		}
		break;
	case MSI_HYPER_CHARGE_MAGIC_STONE_FIXED_EFFECT:
		if( iItemType == CXSLItem::IT_WEAPON )
		{
			eSocketDataType = SDT_WEAPON_HYPER_CHARGE_10P_FIXED;
		}
		else
		{
			eSocketDataType = SDT_DEFFENCE_HYPER_CHARGE_5P_FIXED;
		}
		break;
	case MSI_DECREASE_DAMEGE_STONE_FIXED_EFFECT:
		if( iItemType == CXSLItem::IT_WEAPON )
		{
			eSocketDataType = SDT_WEAPON_DAMAGE_REDUCE_5P_FIXED;
		}
		else
		{
			eSocketDataType = SDT_DEFFECNE_DAMAGE_REDUCE_2P5_FIXED;
		}
		break;
#endif SERV_FIXED_EFFECTS_SOCKET_ITEM
		//}}
#ifdef SERV_FISHING_HOLE_MAGIC_STONE
	case MSI_EVENT_MAGIC_STONE_1:
		if( iItemType == CXSLItem::IT_WEAPON )
		{
			eSocketDataType = SDT_WEAPON_DESTROY;
		}
		else
		{
			eSocketDataType = SDT_DEFENCE_DESTROY;
		}
		break;
	case MSI_EVENT_MAGIC_STONE_2:
		if( iItemType == CXSLItem::IT_WEAPON )
		{
			eSocketDataType = SDT_WEAPON_EXPLOSION;
		}
		else
		{
			eSocketDataType = SDT_DEFENCE_EXPLOSION;
		}
		break;
	case MSI_EVENT_MAGIC_STONE_3:
		if( iItemType == CXSLItem::IT_WEAPON )
		{
			eSocketDataType = SDT_WEAPON_BURN;
		}
		else
		{
			eSocketDataType = SDT_DEFENCE_BURN;
		}
		break;
	case MSI_EVENT_MAGIC_STONE_4:
		if( iItemType == CXSLItem::IT_WEAPON )
		{
			eSocketDataType = SDT_WEAPON_QUICK;
		}
		else
		{
			eSocketDataType = SDT_DEFENCE_QUICK;
		}
		break;
	case MSI_EVENT_MAGIC_STONE_5:
		if( iItemType == CXSLItem::IT_WEAPON )
		{
			eSocketDataType = SDT_WEAPON_FURY;
		}
		else
		{
			eSocketDataType = SDT_DEFENCE_FURY;
		}
		break;
#endif SERV_FISHING_HOLE_MAGIC_STONE
#ifdef SERV_MISS_ELIOS_STONE_ITEM
	case MSI_MISS_ELIOS_STONE_ITEM:
		if( iItemType == CXSLItem::IT_WEAPON )
		{
			eSocketDataType = SDT_WEAPON_SAGE;
		}
		else
		{
			eSocketDataType = SDT_DEFENCE_SAGE;
		}
		break;
#endif SERV_MISS_ELIOS_STONE_ITEM
#ifdef SERV_BC_EVENT
	case MSI_VC_EVENT_STONE_ITEM:
		if( iItemType == CXSLItem::IT_WEAPON )
		{
			eSocketDataType = SDT_WEAPON_SAGE;
		}
		else
		{
			eSocketDataType = SDT_DEFENCE_SAGE;
		}
		break;
#endif SERV_BC_EVENT
	default:
		START_LOG( cerr, L"등록되지 않은 마법석 ItemID입니다." )
			<< BUILD_LOG( iItemID )
			<< BUILD_LOG( iItemType )
			<< END_LOG;

		eSocketDataType = SDT_NONE;
	}

	return eSocketDataType;
}

int CXSLSocketItem::GetMagicStoneItemID( IN bool bIsRare ) const
{
	if( !bIsRare )
	{
		return MSI_MAGIC_STONE;
	}
	else
	{
		return MSI_RARE_MAGIC_STONE;
	}
}

//{{ 2012. 09. 26  김민성  대전 장비 소켓 고정 강화 레벨
#ifdef SERV_2012_PVP_SEASON2_1
bool CXSLSocketItem::GetResultItem( IN int iMagicStoneItemID, IN int iItemType, IN int iPeriodType, IN int iUseLevel, IN bool bFashion, IN int iEnchantLevel, OUT int& iResultSocketOption ) const
#else
//{{ 2011. 07. 25    김민성    아이템 옵션ID 데이터 사이즈 증가
//#ifdef SERV_ITEM_OPTION_DATA_SIZE
//bool CXSLSocketItem::GetResultItem( IN int iMagicStoneItemID, IN int iItemType, IN int iPeriodType, IN bool bFashion, IN int iEnchantLevel, OUT int& iResultSocketOption ) const
//#else
//bool CXSLSocketItem::GetResultItem( IN int iMagicStoneItemID, IN int iItemType, IN int iPeriodType, IN bool bFashion, IN int iEnchantLevel, OUT short& sResultSocketOption ) const
//#endif SERV_ITEM_OPTION_DATA_SIZE
//}} 
#endif SERV_2012_PVP_SEASON2_1
//}}
{
	// 1. 소켓 랜덤 그룹을 얻기 위한 타입 정보
	SOCKET_DATA_TYPE eSocketDataType = GetSocketDataType( iMagicStoneItemID, iItemType );
	if( eSocketDataType == SDT_NONE )
	{
		START_LOG( cerr, L"잘못된 마법석 ItemID임." )
			<< BUILD_LOG( iMagicStoneItemID )
			<< END_LOG;

		SET_ERROR( ERR_SOCKET_ITEM_01 );
		return false;
	}

	// 2. 소켓 랜덤 그룹 얻기
	std::map< int, KLottery >::const_iterator mit;
	mit = m_mapSocketRandom.find( eSocketDataType );
	if( mit == m_mapSocketRandom.end() )
	{
		START_LOG( cerr, L"아이템 소켓 랜덤 정보 얻기 실패.!" )
			<< BUILD_LOG( eSocketDataType )
			<< END_LOG;

		SET_ERROR( ERR_SOCKET_ITEM_01 );
		return false;
	}

	// 3. 소켓 랜덤 그룹 풀
	int iGroupID = mit->second.Decision();
	if( iGroupID == KLottery::CASE_BLANK )
	{
		START_LOG( cerr, L"아이템 소켓 그룹ID 얻기 실패.!" )
			<< BUILD_LOG( eSocketDataType )			
			<< END_LOG;

		SET_ERROR( ERR_SOCKET_ITEM_01 );
		return false;
	}

	std::map< int, KSocketGroup >::const_iterator mitSG;
	mitSG = m_mapSocketGroup.find( iGroupID );
	if( mitSG == m_mapSocketGroup.end() )
	{
		START_LOG( cerr, L"아이템 소켓 그룹 정보 얻기 실패.!" )
			<< BUILD_LOG( eSocketDataType )
			<< BUILD_LOG( iGroupID )
			<< END_LOG;

		SET_ERROR( ERR_SOCKET_ITEM_01 );
		return false;
	}

	// 4. 패션 아이템이라면 가장 좋은 옵션풀로..
	if( bFashion )
	{
		iEnchantLevel = static_cast<int>(CXSLEnchantItemManager::MAX_ENCHANT_LEVEL);
	}
	//{{ 2012. 09. 26  김민성  대전 장비 소켓 고정 강화 레벨
#ifdef SERV_2012_PVP_SEASON2_1
	else if( bFashion == false && iUseLevel == 0 )
	{
		iEnchantLevel = 8;
	}
#endif SERV_2012_PVP_SEASON2_1
	//}}

	// 5. 소켓 옵션 결과 얻기
	//{{ 2011. 07. 25    김민성    아이템 옵션ID 데이터 사이즈 증가
#ifdef SERV_ITEM_OPTION_DATA_SIZE
	KSocketGroup::const_iterator mitGroup;
	for( mitGroup = mitSG->second.begin(); mitGroup != mitSG->second.end(); ++mitGroup )
	{
		const KBeginEndEnchantLv& kEnchantLv = mitGroup->first;

		if( iEnchantLevel < kEnchantLv.first  ||  iEnchantLevel > kEnchantLv.second )
			continue;

		iResultSocketOption = mitGroup->second.Decision() ;
		break;
	}

	if( iResultSocketOption == KLotterySameProb< int >::CASE_BLANK )
	{
		START_LOG( cerr, L"일어날 수 없는에러!" )
			<< BUILD_LOG( eSocketDataType )
			<< BUILD_LOG( iGroupID )
			<< BUILD_LOG( iEnchantLevel )
			<< BUILD_LOG( bFashion )
			<< END_LOG;

		SET_ERROR( ERR_SOCKET_ITEM_01 );
		return false;
	}
#else
	KSocketGroup::const_iterator mitGroup;
	for( mitGroup = mitSG->second.begin(); mitGroup != mitSG->second.end(); ++mitGroup )
	{
		const KBeginEndEnchantLv& kEnchantLv = mitGroup->first;

		if( iEnchantLevel < kEnchantLv.first  ||  iEnchantLevel > kEnchantLv.second )
			continue;

		sResultSocketOption = static_cast<short>( mitGroup->second.Decision() );
		break;
	}

	if( sResultSocketOption == KLotterySameProb< int >::CASE_BLANK )
	{
		START_LOG( cerr, L"일어날 수 없는에러!" )
			<< BUILD_LOG( eSocketDataType )
			<< BUILD_LOG( iGroupID )
			<< BUILD_LOG( iEnchantLevel )
			<< BUILD_LOG( bFashion )
			<< END_LOG;

		SET_ERROR( ERR_SOCKET_ITEM_01 );
		return false;
	}
#endif SERV_ITEM_OPTION_DATA_SIZE
	//}} 
	
	return true;
}

//{{ 2010. 04. 15  최육사	소켓 개편
#ifdef SERV_SOCKET_NEW

bool CXSLSocketItem::IsMagicStoneItemID( IN int iItemID )
{
	switch( iItemID )
	{
	case MSI_MAGIC_STONE:
	case MSI_RARE_MAGIC_STONE:
		//{{ 2010. 04. 22  최육사	소켓 개편
	case MSI_HARMONY_MAGIC_STONE:
	case MSI_MYSTERY_MAGIC_STONE:
	case MSI_SHINE_MAGIC_STONE:
	case MSI_BATTLE_MAGIC_STONE:
	case MSI_INCREASE_MAGIC_STONE:
	case MSI_HIT_MAGIC_STONE:
	case MSI_ATK_PHYSIC_MAGIC_STONE:
	case MSI_ATK_MAGIC_MAGIC_STONE:
	case MSI_DEF_PHYSIC_MAGIC_STONE:
	case MSI_DEF_MAGIC_MAGIC_STONE:
	case MSI_HYPER_TIME_MAGIC_STONE:
	case MSI_INCREASE_HP_MAGIC_STONE:
	case MSI_HYPER_CHARGE_MAGIC_STONE:
	case MSI_SPEED_MAGIC_STONE:
	case MSI_CRITICAL_MAGIC_STONE:
	case MSI_MOVE_SPEED_MAGIC_STONE:
	case MSI_STRIKE_MAGIC_STONE:
	case MSI_AVOID_MAGIC_STONE:
	case MSI_JUMP_SPEED_MAGIC_STONE:
		//}}
		//{{ 2011. 07. 27 김민성   소켓 아이템 추가
	case MSI_INCREASE_DAMEGE_STONE:
	case MSI_DECREASE_DAMEGE_STONE:
		//}}
		//{{ 2011. 08. 30	최육사	듀얼 마법석 개편
#ifdef SERV_NEW_DUAL_MAGIC_STONE
	case MSI_DESTROY_MAGIC_STONE:
	case MSI_EXPLOSION_MAGIC_STONE:
	case MSI_BURN_MAGIC_STONE:
	case MSI_QUICK_MAGIC_STONE:
	case MSI_FURY_MAGIC_STONE:
#endif SERV_NEW_DUAL_MAGIC_STONE
		//}}
		//{{ 2012. 07. 09	김민성       현자의 마법석
//#ifdef SERV_DUNGEON_CLEAR_PAYMENT_STONE_EVENT
	case MSI_WISE_MAN_STONE_ITEM:
//#endif SERV_DUNGEON_CLEAR_PAYMENT_STONE_EVENT
		//}}

		//{{ 2012. 09. 19	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
	case MSI_MOVE_SPEED_PVP:
	case MSI_JUMP_SPEED_PVP:
	case MSI_INCREASE_HP_PVP:
	case MSI_STRIKE_PVP:
	case MSI_AVOID_PVP:
	case MSI_HYPER_CHARGE_PVP:
	case MSI_HYPER_TIME_PVP:
	case MSI_CRITICAL_PVP:
	case MSI_SPEED_PVP:
	case MSI_ADD_DAMAGE_PVP:
	case MSI_ADD_DAMAGE_REDUCE_PVP:
	case MSI_EXPLOSION_PVP:
	case MSI_DESTROY_PVP:
	case MSI_QUICK_PVP:
	case MSI_BURN_PVP:
	case MSI_FURY_PVP:
#endif SERV_2012_PVP_SEASON2
		//}}
		//{{ 2012. 12. 11  고정 효과 마법석 추가 - 김민성
#ifdef SERV_FIXED_EFFECTS_SOCKET_ITEM
	case MSI_SPEED_MAGIC_STONE_FIXED_EFFECT:
	case MSI_CRITICAL_MAGIC_STONE_FIXED_EFFECT:
	case MSI_INCREASE_DAMEGE_STONE_FIXED_EFFECT:
	case MSI_HYPER_CHARGE_MAGIC_STONE_FIXED_EFFECT:
	case MSI_AVOID_MAGIC_STONE_FIXED_EFFECT:
	case MSI_DECREASE_DAMEGE_STONE_FIXED_EFFECT:
#endif SERV_FIXED_EFFECTS_SOCKET_ITEM
		//}}
#ifdef SERV_FISHING_HOLE_MAGIC_STONE
	case MSI_EVENT_MAGIC_STONE_1:
	case MSI_EVENT_MAGIC_STONE_2:
	case MSI_EVENT_MAGIC_STONE_3:
	case MSI_EVENT_MAGIC_STONE_4:
	case MSI_EVENT_MAGIC_STONE_5:
#endif SERV_FISHING_HOLE_MAGIC_STONE
#ifdef SERV_MISS_ELIOS_STONE_ITEM
	case MSI_MISS_ELIOS_STONE_ITEM:
#endif SERV_MISS_ELIOS_STONE_ITEM
#ifdef SERV_BC_EVENT
	case MSI_VC_EVENT_STONE_ITEM:
#endif SERV_BC_EVENT
#ifdef SERV_LURIEL_MAGIC_STONE
	case MSI_LURIEL_INCREASE_DAMEGE_STONE:
	case MSI_LURIEL_DECREASE_DAMEGE_STONE:
	case MSI_LURIEL_HYPER_TIME_MAGIC_STONE:
	case MSI_LURIEL_INCREASE_HP_MAGIC_STONE:
	case MSI_LURIEL_HYPER_CHARGE_MAGIC_STONE:
	case MSI_LURIEL_SPEED_MAGIC_STONE:
	case MSI_LURIEL_CRITICAL_MAGIC_STONE:
	case MSI_LURIEL_MOVE_SPEED_MAGIC_STONE:
	case MSI_LURIEL_STRIKE_MAGIC_STONE:
	case MSI_LURIEL_AVOID_MAGIC_STONE:
	case MSI_LURIEL_JUMP_SPEED_MAGIC_STONE:
	case MSI_LURIEL_DESTROY_MAGIC_STONE:
	case MSI_LURIEL_EXPLOSION_MAGIC_STONE:
	case MSI_LURIEL_BURN_MAGIC_STONE:
	case MSI_LURIEL_QUICK_MAGIC_STONE:
	case MSI_LURIEL_FURY_MAGIC_STONE:
#endif //SERV_LURIEL_MAGIC_STONE

		return true;
	}

	return false;
}

bool CXSLSocketItem::CalcInsertSocketCost( IN int iItemID, IN int iMagicStoneItemID, IN int iAssignedSocketCount, IN bool bEmptySlot, OUT int& iCost )
{
	iCost = 0;

	const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iItemID );
	if( pItemTemplet == NULL )
	{
		START_LOG( cerr, L"존재하지 않는 아이템 정보 입니다." )
			<< BUILD_LOG( iItemID )
			<< END_LOG;

		return false;
	}

	const CXSLItem::ItemTemplet* pMagicStoneTemplet = SiCXSLItemManager()->GetItemTemplet( iMagicStoneItemID );
	if( pMagicStoneTemplet == NULL )
	{
		START_LOG( cerr, L"존재하지 않는 아이템 정보 입니다." )
			<< BUILD_LOG( iMagicStoneItemID )
			<< END_LOG;

		return false;
	}

	double fItemGradeFactor = 0.0f;

	switch( pMagicStoneTemplet->m_ItemGrade )
	{
	case CXSLItem::IG_UNIQUE:	fItemGradeFactor = 2.5; break;
	case CXSLItem::IG_ELITE:	fItemGradeFactor = 2.0; break;
	case CXSLItem::IG_RARE:		fItemGradeFactor = 1.5; break;
	case CXSLItem::IG_NORMAL:	fItemGradeFactor = 1.0; break;
	default:
		START_LOG( cerr, L"이상한 등급의 아이템에다가 소켓을 박을려고 하네?" )
			<< BUILD_LOGc( pMagicStoneTemplet->m_ItemGrade )
			<< END_LOG;
		return false;
	}

	// 패션템인가?
	if( pItemTemplet->m_bFashion )
	{
		if( bEmptySlot )
		{
			iCost = (int)( (double)1000 * fItemGradeFactor );
		}
		else
		{
			iCost = (int)( (double)1000 * fItemGradeFactor ) * 3;
		}
		return true;
	}
	
	// 박혀있는 소켓 갯수가 0이라면 1로 계산
	if( iAssignedSocketCount == 0 )
		iAssignedSocketCount = 1;

	// 박는 비용	
	if( bEmptySlot )
	{
		iCost = (int)( ( (double)pItemTemplet->m_Price * 0.01 ) * (double)iAssignedSocketCount * fItemGradeFactor );
	}
	else
	{
		iCost = (int)( ( (double)pItemTemplet->m_Price * 0.01 ) * (double)iAssignedSocketCount * fItemGradeFactor ) * 3;
	}

	return true;
}

#endif SERV_SOCKET_NEW
//}}

////{{ 2011. 07. 25    김민성    아이템 옵션ID 데이터 사이즈 증가
//#ifdef SERV_ITEM_OPTION_DATA_SIZE
bool CXSLSocketItem::GetSocketOptionStat(	IN const std::vector< int >& vecSocketOption,
											IN bool bIsDungeon,
											IN const KStat& kBaseStat,
											IN OUT float& fTotalIncHPRate,
											OUT KStat& kAddStat,
											IN unsigned int& uiHP_OnePoint,
											IN unsigned int& iItemLevel
											//{{ 2012. 09. 25	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2_1
											, IN const bool bLevelLinkedStat /*= false*/
#endif SERV_2012_PVP_SEASON2_1
											//}}
											)
//#else
//void CXSLSocketItem::GetSocketOptionStat( IN const std::vector< short >& vecSocketOption, IN bool bIsDungeon, IN const KStat& kBaseStat, IN OUT float& fTotalIncHPRate, OUT KStat& kAddStat, IN unsigned int& uiHP_OnePoint, IN unsigned int& iItemLevel )
//#endif SERV_ITEM_OPTION_DATA_SIZE
////}}
{
	bool bSuccess = true;
	kAddStat.Init();

	//{{ 2011. 07. 08    김민성    옵션 수치화
#ifdef SERV_USE_PERCENT_IN_OPTION_DATA
	if( iItemLevel <= 0 )	// 잘못된 레벨의 값
	{
		uiHP_OnePoint = 0;
		START_LOG( cerr, L"옵션 수치화 레벨 값이 잘못되었다" );
	}
	else
	{
		uiHP_OnePoint = CXSLUnit::UPHP_INIT + ( CXSLUnit::UPHP_INCREASE_ONE_POINT * (iItemLevel - 1)); // 현재 레벨당 1단위 값
	}
#endif SERV_USE_PERCENT_IN_OPTION_DATA
	//}} 

	//{{ 2011. 07. 25    김민성    아이템 옵션ID 데이터 사이즈 증가
#ifdef SERV_ITEM_OPTION_DATA_SIZE
	BOOST_TEST_FOREACH( const int, sSocketOptionID, vecSocketOption )
#else
	BOOST_TEST_FOREACH( const short, sSocketOptionID, vecSocketOption )
#endif SERV_ITEM_OPTION_DATA_SIZE
	//}} 
	{
		if( sSocketOptionID == 0 )
			continue;

		const SocketData* pSocketData = GetSocketData( sSocketOptionID );
		if( pSocketData == NULL )
		{
			START_LOG( cerr, L"존재하지않는 소켓옵션이 세팅되어 있습니다." )
				<< BUILD_LOG( sSocketOptionID )
				<< END_LOG;

			bSuccess = false;
			continue;
		}

		//{{ 2012. 09. 25	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2_1
		if( bLevelLinkedStat != pSocketData->m_bIsLevelLinkedStat )
		{
			continue;
		}
#endif SERV_2012_PVP_SEASON2_1
		//}}

		//{{ 2009. 4. 1  최육사		던전온리
		if( pSocketData->m_bDungeonOnly == true  &&  bIsDungeon == false )
			continue;
		//}}

		if( pSocketData->m_bPVPOnly == true  &&  bIsDungeon == true )
			continue;

		//{{ 2008. 11. 20  최육사	HP증가율 50% 제한
		float fIncreaseHPRate = pSocketData->m_IncreaseHPRate;

		if( ( fTotalIncHPRate + fIncreaseHPRate ) > 0.5f )
		{
			fIncreaseHPRate = 0.5f - fTotalIncHPRate;
		}

		fTotalIncHPRate += fIncreaseHPRate;
		//}}

		//{{ 2011. 07. 08    김민성    옵션 수치화
#ifdef SERV_USE_PERCENT_IN_OPTION_DATA
		int iIncreaseHPRate = (int)( ( fIncreaseHPRate + 0.0005f ) * 100 );			// % 단위로 변환 (0%~100%)   // float 하위단위 보정 
		kAddStat.m_iBaseHP		+= (int)( iIncreaseHPRate * uiHP_OnePoint );
#else
		kAddStat.m_iBaseHP		+= (int)( static_cast<float>(kBaseStat.m_iBaseHP) * fIncreaseHPRate );
#endif SERV_USE_PERCENT_IN_OPTION_DATA
		//}}

		//{{ 2012. 09. 25	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2_1
		int iMultiplyLevelValue = 1;

		if( pSocketData->m_bIsLevelLinkedStat == true )
		{
			iMultiplyLevelValue = iItemLevel + pSocketData->m_cAddLevel;
		}

		kAddStat.m_iBaseHP		+= (int)( pSocketData->m_Stat.m_fBaseHP * iMultiplyLevelValue );
		kAddStat.m_iAtkPhysic	+= (int)( pSocketData->m_Stat.m_fAtkPhysic * iMultiplyLevelValue );
		kAddStat.m_iAtkMagic	+= (int)( pSocketData->m_Stat.m_fAtkMagic * iMultiplyLevelValue );
		kAddStat.m_iDefPhysic	+= (int)( pSocketData->m_Stat.m_fDefPhysic * iMultiplyLevelValue );
		kAddStat.m_iDefMagic	+= (int)( pSocketData->m_Stat.m_fDefMagic * iMultiplyLevelValue );
#else
		kAddStat.m_iBaseHP		+= (int)( pSocketData->m_Stat.m_fBaseHP );
		kAddStat.m_iAtkPhysic	+= (int)( pSocketData->m_Stat.m_fAtkPhysic );
		kAddStat.m_iAtkMagic	+= (int)( pSocketData->m_Stat.m_fAtkMagic );
		kAddStat.m_iDefPhysic	+= (int)( pSocketData->m_Stat.m_fDefPhysic );
		kAddStat.m_iDefMagic	+= (int)( pSocketData->m_Stat.m_fDefMagic );
#endif SERV_2012_PVP_SEASON2_1
		//}}
	}

	return bSuccess;
}

//{{ 2011. 07. 25    김민성    아이템 옵션ID 데이터 사이즈 증가
//#ifdef SERV_ITEM_OPTION_DATA_SIZE
void CXSLSocketItem::GetSocketOptionIncreaseRate( IN const std::vector< int >& vecSocketOption, IN bool bIsDungeon, IN OUT KStatIncreaseRate& kStatIncreaseRateInfo )
{
	BOOST_TEST_FOREACH( const int, iSocketOptionID, vecSocketOption )
	{
		const SocketData* pSocketData = GetSocketData( iSocketOptionID );
		if( pSocketData == NULL )
		{
			START_LOG( cerr, L"존재하지않는 소켓옵션이 세팅되어 있습니다." )
				<< BUILD_LOG( iSocketOptionID )
				<< END_LOG;
			continue;
		}
//#else
//void CXSLSocketItem::GetSocketOptionIncreaseRate( IN const std::vector< short >& vecSocketOption, IN bool bIsDungeon, IN OUT KStatIncreaseRate& kStatIncreaseRateInfo )
//{
//	BOOST_TEST_FOREACH( const short, sSocketOptionID, vecSocketOption )
//	{
//		const SocketData* pSocketData = GetSocketData( sSocketOptionID );
//		if( pSocketData == NULL )
//		{
//			START_LOG( cerr, L"존재하지않는 소켓옵션이 세팅되어 있습니다." )
//				<< BUILD_LOG( sSocketOptionID )
//				<< END_LOG;
//			continue;
//		}
//#endif SERV_ITEM_OPTION_DATA_SIZE
//}} 
		//{{ 2009. 4. 1  최육사		던전온리
		if( pSocketData->m_bDungeonOnly == true  &&  bIsDungeon == false )
			continue;
		//}}

		if( pSocketData->m_bPVPOnly == true  &&  bIsDungeon == true )
			continue;
		
		kStatIncreaseRateInfo.m_fIncreaseAtkPhysicRate	+= pSocketData->m_fIncreaseAtkPhysicRate;
		kStatIncreaseRateInfo.m_fIncreaseAtkMagicRate	+= pSocketData->m_fIncreaseAtkMagicRate;
		kStatIncreaseRateInfo.m_fIncreaseDefPhysicRate	+= pSocketData->m_fIncreaseDefPhysicRate;
		kStatIncreaseRateInfo.m_fIncreaseDefMagicRate	+= pSocketData->m_fIncreaseDefMagicRate;
	}
}
//#endif SERV_PET_AURA_SKILL
//}}

bool CXSLSocketItem::AddSocketData_LUA()
{
	KLuaManager luaManager( GetLuaState() );
	TableBind( &luaManager );

	std::wstring wstrLuaErr;
	SocketData kSocketData;

	//데이터 로드
	LUA_GET_VALUE_RETURN_ERR(		luaManager, L"ID",		kSocketData.m_ID,		0,							wstrLuaErr, return false; );
	if( kSocketData.m_ID <= 0 )
	{
		START_LOG( cerr, L"소켓 옵션ID에 0 이하의 값을 사용할 수 없습니다." )
			<< BUILD_LOG( kSocketData.m_ID )
			<< END_LOG;
		return false;
	}

	LUA_GET_VALUE_RETURN_ENUM_ERR(  luaManager, L"TYPE",	kSocketData.m_Type,		SOCKET_DATA_TYPE, SDT_NONE, wstrLuaErr, return false; );

	//{{ 2010. 10. 18	최육사	socket option값 범위 체크
	//{{ 2011. 07. 25    김민성    아이템 옵션ID 데이터 사이즈 증가
#ifdef SERV_ITEM_OPTION_DATA_SIZE
	if( kSocketData.m_ID > INT_MAX )
#else
	if( kSocketData.m_ID > SHRT_MAX )
#endif SERV_ITEM_OPTION_DATA_SIZE
	//}}
	{
		START_LOG( cerr, L"short타입의 범위를 넘어서는 소켓 옵션ID입니다!" )
			<< BUILD_LOG( kSocketData.m_ID )
			<< END_LOG;
	}
	//}}

	//스탯정보
	if( luaManager.BeginTable( L"STAT" ) == S_OK )
	{
		LUA_GET_VALUE( luaManager,		"m_fBaseHP",		kSocketData.m_Stat.m_fBaseHP,			0.0f	);
		LUA_GET_VALUE( luaManager,		"m_fAtkPhysic",		kSocketData.m_Stat.m_fAtkPhysic,		0.0f	);
		LUA_GET_VALUE( luaManager,		"m_fAtkMagic",		kSocketData.m_Stat.m_fAtkMagic,			0.0f	);
		LUA_GET_VALUE( luaManager,		"m_fDefPhysic",		kSocketData.m_Stat.m_fDefPhysic,		0.0f	);
		LUA_GET_VALUE( luaManager,		"m_fDefMagic",		kSocketData.m_Stat.m_fDefMagic,			0.0f	);

		luaManager.EndTable();
	}

	//{{ 2013. 06. 04	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
	if( luaManager.BeginTable( L"STAT_RELATION_ADD_LEVEL" ) == S_OK )
	{
		LUA_GET_VALUE( luaManager,		"m_fBaseHPRelLV",		kSocketData.m_kStatRelLVData.m_fBaseHPRelLV,		0.f );
		LUA_GET_VALUE( luaManager,		"m_fAtkPhysicRelLV",	kSocketData.m_kStatRelLVData.m_fAtkPhysicRelLV,		0.f );
		LUA_GET_VALUE( luaManager,		"m_fAtkMagicRelLV",		kSocketData.m_kStatRelLVData.m_fAtkMagicRelLV,		0.f );
		LUA_GET_VALUE( luaManager,		"m_fDefPhysicRelLV",	kSocketData.m_kStatRelLVData.m_fDefPhysicRelLV,		0.f );
		LUA_GET_VALUE( luaManager,		"m_fDefMagicRelLV",		kSocketData.m_kStatRelLVData.m_fDefMagicRelLV,		0.f );

		luaManager.EndTable();
	}
#endif SERV_NEW_ITEM_SYSTEM_2013_05
	//}}

	//{{ 2008. 11. 15  최육사		
	if( luaManager.BeginTable( L"RESIST_ENCHANT" ) == S_OK )
	{
		LUA_GET_VALUE( luaManager,		"BLAZE",				kSocketData.m_fResistBlaze,				0.0f	);
		LUA_GET_VALUE( luaManager,		"WATER",				kSocketData.m_fResistWater,				0.0f	);
		LUA_GET_VALUE( luaManager,		"NATURE",				kSocketData.m_fResistNature,			0.0f	);
		LUA_GET_VALUE( luaManager,		"WIND",					kSocketData.m_fResistWind,				0.0f	);
		LUA_GET_VALUE( luaManager,		"LIGHT",				kSocketData.m_fResistLight,				0.0f	);
		LUA_GET_VALUE( luaManager,		"DARK",					kSocketData.m_fResistDark,				0.0f	);

		luaManager.EndTable();
	}
	//}}

	//{{ 2010. 10. 18	최육사	펫 오라 스킬	
#ifdef SERV_PET_AURA_SKILL
	if( luaManager.BeginTable( L"ATTACK_ENCHANT" ) == S_OK )
	{
		LUA_GET_VALUE( luaManager,		"BLAZE",				kSocketData.m_fAttackBlaze,				0.f		);
		LUA_GET_VALUE( luaManager,		"WATER",				kSocketData.m_fAttackWater,				0.f		);
		LUA_GET_VALUE( luaManager,		"NATURE",				kSocketData.m_fAttackNature,			0.f		);
		LUA_GET_VALUE( luaManager,		"WIND",					kSocketData.m_fAttackWind,				0.f		);
		LUA_GET_VALUE( luaManager,		"LIGHT",				kSocketData.m_fAttackLight,				0.f		);
		LUA_GET_VALUE( luaManager,		"DARK",					kSocketData.m_fAttackDark,				0.f		);

		luaManager.EndTable();
	}
#endif SERV_PET_AURA_SKILL
	//}}

	//기타 옵션
	switch( kSocketData.m_Type )
	{
	case SDT_WEAPON_NORMAL:			
		//LUA_GET_VALUE( luaManager,	"m_fHyperModeTime",			kSocketData.m_fHyperModeTime,		0.0f	);
		LUA_GET_VALUE( luaManager,		"m_fRepairPriceSale",		kSocketData.m_fRepairPriceSale,		0.0f	);
		break;

	case SDT_WEAPON_RARE:
		//LUA_GET_VALUE( luaManager,	"m_fHyperModeTime",			kSocketData.m_fHyperModeTime,		0.0f	);
		LUA_GET_VALUE( luaManager,		"m_fRepairPriceSale",		kSocketData.m_fRepairPriceSale,		0.0f	);
		break;

	case SDT_DEFENCE_NORMAL:
		//LUA_GET_VALUE( luaManager,	"m_fMoveSpeed",				kSocketData.m_fMoveSpeed,			0.0f	);
		//LUA_GET_VALUE( luaManager,	"m_fJumpSpeed",				kSocketData.m_fJumpSpeed,			0.0f	);
		LUA_GET_VALUE( luaManager,		"m_fRepairPriceSale",		kSocketData.m_fRepairPriceSale,		0.0f	);
		break;

	case SDT_DEFENCE_RARE:
		//LUA_GET_VALUE( luaManager,	"m_fMoveSpeed",				kSocketData.m_fMoveSpeed,			0.0f	);
		//LUA_GET_VALUE( luaManager,	"m_fJumpSpeed",				kSocketData.m_fJumpSpeed,			0.0f	);
		//LUA_GET_VALUE( luaManager,	"m_fHyperModeChargeRate",	kSocketData.m_fHyperModeChargeRate,	0.0f	);
		LUA_GET_VALUE( luaManager,		"m_fRepairPriceSale",		kSocketData.m_fRepairPriceSale,		0.0f	);
		break;

	case SDT_SET_ITEM:
		LUA_GET_VALUE( luaManager,		"m_fRepairPriceSale",		kSocketData.m_fRepairPriceSale,		0.0f	);
		break;
	}

	// 특수 데미지 : 서버에서는 파싱하지 않음.
	LUA_GET_VALUE( luaManager,			"m_IncreaseHP",				kSocketData.m_IncreaseHPRate,		0.f		);

	//{{ 2010. 10. 18	최육사	펫 오라 스킬
#ifdef SERV_PET_AURA_SKILL
	LUA_GET_VALUE(	luaManager,			"m_IncreaseMP",				kSocketData.m_fIncreaseMPRate,			0.f );
	LUA_GET_VALUE(	luaManager,			"m_IncreaseAtkPhysic",		kSocketData.m_fIncreaseAtkPhysicRate,	0.f );
	LUA_GET_VALUE(	luaManager,			"m_IncreaseAtkMagic",		kSocketData.m_fIncreaseAtkMagicRate,	0.f );
	LUA_GET_VALUE(	luaManager,			"m_IncreaseDefPhysic",		kSocketData.m_fIncreaseDefPhysicRate,	0.f );
	LUA_GET_VALUE(	luaManager,			"m_IncreaseDefMagic",		kSocketData.m_fIncreaseDefMagicRate,	0.f );
#endif SERV_PET_AURA_SKILL
	//}}

	// 기타 설정
	LUA_GET_VALUE( luaManager,			"DUNGEON_ONLY",				kSocketData.m_bDungeonOnly,			false	);
	LUA_GET_VALUE( luaManager,			"PVP_ONLY",					kSocketData.m_bPVPOnly,				false	);
	LUA_GET_VALUE( luaManager,			"OVER_LAP",					kSocketData.m_bOverlap,				true	);

	LUA_GET_VALUE( luaManager,			"m_fIncreaseEDPercent",		kSocketData.m_fIncreaseEDPer,		0.0f	);
	LUA_GET_VALUE( luaManager,			"m_fIncreaseDCBExpPercent",	kSocketData.m_fIncreaseExpPer,		0.0f	);

	//{{ 2012. 06. 27	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2_1
	LUA_GET_VALUE( luaManager,			"m_bIsLevelLinkedStat",		kSocketData.m_bIsLevelLinkedStat,	false	);
	LUA_GET_VALUE( luaManager,			"m_iAddLevel",				kSocketData.m_cAddLevel,			0		);
#endif SERV_2012_PVP_SEASON2_1
	//}}

	if( m_mapSocketData.find( kSocketData.m_ID ) != m_mapSocketData.end() )
	{
		START_LOG( cerr, L"이미 등록된 소켓 정보입니다! 소켓 아이템 ID 중복 발견!" )
			<< BUILD_LOG( kSocketData.m_ID )
			<< END_LOG;
		return false;
	}
	else
	{
		// 소켓 옵션 정보
		m_mapSocketData.insert( std::make_pair( kSocketData.m_ID, kSocketData ) );
		return true;
	}
}

bool CXSLSocketItem::AddSocketRandomData_LUA( int iSocketDataType, int iGroupID, float fRate )
{
	if( iSocketDataType <= 0 || iGroupID <= 0 || fRate <= 0.0f )
	{
		START_LOG( cerr, L"소켓 옵션 정보가 이상합니다." )
			<< BUILD_LOG( iSocketDataType )
			<< BUILD_LOG( iGroupID )
			<< BUILD_LOG( fRate )
			<< END_LOG;
		return false;
	}

	std::map< int, KLottery >::iterator mit;
	mit = m_mapSocketRandom.find( iSocketDataType );
	if( mit == m_mapSocketRandom.end() )
	{
		KLottery kLot;
		kLot.AddCase( iGroupID, fRate );
		m_mapSocketRandom.insert( std::make_pair( iSocketDataType, kLot ) );
	}
	else
	{
		if( !mit->second.AddCase( iGroupID, fRate ) )
		{
			START_LOG( cerr, L"소켓 랜덤 정보 확률 오버!" )
				<< BUILD_LOG( iSocketDataType )
				<< BUILD_LOG( iGroupID )
				<< BUILD_LOG( fRate )
				<< END_LOG;
			return false;
		}
	}

	return true;
}

bool CXSLSocketItem::AddSocketGroup_LUA( int iGroupID, int iBeginEnchantLv, int iEndEnchantLv, int iOptionID, float fRate )
{
	if( iGroupID <= 0 || iBeginEnchantLv < 0 || iEndEnchantLv <= 0 || iOptionID <= 0 || fRate <= 0.0f )
	{
		START_LOG( cerr, L"소켓 옵션 정보가 이상합니다." )
			<< BUILD_LOG( iGroupID )
			<< BUILD_LOG( iBeginEnchantLv )
			<< BUILD_LOG( iEndEnchantLv )
			<< BUILD_LOG( iOptionID )
			<< BUILD_LOG( fRate )			
			<< END_LOG;
		return false;
	}

	KBeginEndEnchantLv kEnchantLv;
	kEnchantLv.first  = iBeginEnchantLv;
	kEnchantLv.second = iEndEnchantLv;

	std::map< int, KSocketGroup >::iterator mit;
	mit = m_mapSocketGroup.find( iGroupID );
	if( mit == m_mapSocketGroup.end() )
	{
		KLottery kLot;
		kLot.AddCase( iOptionID, fRate );

		KSocketGroup kGroupInfo;
		kGroupInfo.insert( std::make_pair( kEnchantLv, kLot ) );
		m_mapSocketGroup.insert( std::make_pair( iGroupID, kGroupInfo ) );
	}
	else
	{
		KSocketGroup::iterator mitSG;
		mitSG = mit->second.find( kEnchantLv );
		if( mitSG == mit->second.end() )
		{
			KLottery kLot;
			kLot.AddCase( iOptionID, fRate );
			mit->second.insert( std::make_pair( kEnchantLv, kLot ) );
		}
		else
		{
            if( !mitSG->second.AddCase( iOptionID, fRate ) )
			{
				START_LOG( cerr, L"소켓 그룹 정보 확률 오버!" )
					<< BUILD_LOG( iGroupID )
					<< BUILD_LOG( iBeginEnchantLv )
					<< BUILD_LOG( iEndEnchantLv )
					<< BUILD_LOG( iOptionID )
					<< BUILD_LOG( fRate )
					<< END_LOG;
				return false;
			}
		}
	}

	return true;
}

bool CXSLSocketItem::AddSetItemData_LUA()
{
	KLuaManager luaManager( GetLuaState() );
	TableBind( &luaManager );
		
	SetItemData kSetItemData;	
	//{{ 2011. 07. 25    김민성    아이템 옵션ID 데이터 사이즈 증가
#ifdef SERV_ITEM_OPTION_DATA_SIZE
	std::vector< int > options;
#else
	std::vector< short > options;
#endif SERV_ITEM_OPTION_DATA_SIZE
	//}} 
	int needPartsNum = 0;

	LUA_GET_VALUE(	luaManager, L"m_SetID",						kSetItemData.m_SetID,		0 );

	if( kSetItemData.m_SetID <= 0 )
	{
		START_LOG( cerr, L"setID값이 이상합니다." )
			<< BUILD_LOG( kSetItemData.m_SetID )
			<< END_LOG;
		return false;
	}

	LUA_GET_VALUE(	luaManager, L"m_SetName",					kSetItemData.m_SetName,		L""	);
	LUA_GET_VALUE(	luaManager, L"m_NeedPartsNum",				needPartsNum,				0 );

	std::map< int, SetItemData >::iterator mit = m_mapSetItemData.find( kSetItemData.m_SetID );
	if( mit == m_mapSetItemData.end() )
	{
		m_mapSetItemData.insert( std::make_pair( kSetItemData.m_SetID, kSetItemData ) );
		mit = m_mapSetItemData.find( kSetItemData.m_SetID );
	}

#ifdef SERV_SET_OPTION_LINE_EXTEND
	for ( int i = 1; i <= 15; i++ )
#else
	for ( int i = 1; i <= 9; i++ )
#endif SERV_SET_OPTION_LINE_EXTEND
	{
		std::wstringstream wstrstm;
		int optionNum;
		wstrstm << L"m_Option" << i;
		LUA_GET_VALUE(	luaManager, wstrstm.str().c_str(),						optionNum,		0 );
		if ( optionNum != 0 )
		{
			//{{ 2010. 10. 18	최육사	socket option값 범위 체크
			//{{ 2011. 07. 25    김민성    아이템 옵션ID 데이터 사이즈 증가
#ifdef SERV_ITEM_OPTION_DATA_SIZE
			if( optionNum > INT_MAX )
			{
				START_LOG( cerr, L"int타입의 범위를 넘어서는 소켓 옵션ID입니다!" )
					<< BUILD_LOG( kSetItemData.m_SetID )
					<< BUILD_LOG( kSetItemData.m_SetName )
					<< BUILD_LOG( optionNum )
					<< END_LOG;
			}
#else
			if( optionNum > SHRT_MAX )
			{
				START_LOG( cerr, L"short타입의 범위를 넘어서는 소켓 옵션ID입니다!" )
					<< BUILD_LOG( kSetItemData.m_SetID )
					<< BUILD_LOG( kSetItemData.m_SetName )
					<< BUILD_LOG( optionNum )
					<< END_LOG;
			}
#endif SERV_ITEM_OPTION_DATA_SIZE
			//}} 
			//}}

			//{{ 2011. 07. 25    김민성    아이템 옵션ID 데이터 사이즈 증가
#ifdef SERV_ITEM_OPTION_DATA_SIZE
			options.push_back( optionNum );
#else
			options.push_back( static_cast<short>(optionNum) );
#endif SERV_ITEM_OPTION_DATA_SIZE
			//}} 
		}
	}

	mit->second.m_mapNeedPartsNumNOptions.insert( std::make_pair( needPartsNum, options ) );
	return true;
}

const CXSLSocketItem::SetItemData* CXSLSocketItem::GetSetItem( int setID )
{
	std::map< int, SetItemData >::const_iterator mit = m_mapSetItemData.find( setID );
	if ( mit == m_mapSetItemData.end() )
		return NULL;

	return &mit->second;
}

//{{ 2011. 07. 25    김민성    아이템 옵션ID 데이터 사이즈 증가
#ifdef SERV_ITEM_OPTION_DATA_SIZE
void CXSLSocketItem::GetSetItemOptions( int setID, int partsNum, std::vector< int >& vecOptions )
#else
void CXSLSocketItem::GetSetItemOptions( int setID, int partsNum, std::vector< short >& vecOptions )
#endif SERV_ITEM_OPTION_DATA_SIZE
//}}
{
	vecOptions.clear();

	std::map< int, SetItemData >::const_iterator mit = m_mapSetItemData.find( setID );
	if ( mit != m_mapSetItemData.end() )
	{
		const SetItemData& kSetItemData = mit->second;
		
		//{{ 2011. 07. 25    김민성    아이템 옵션ID 데이터 사이즈 증가
#ifdef SERV_ITEM_OPTION_DATA_SIZE
		std::map< int, std::vector< int > >::const_iterator mit2;
#else
		std::map< int, std::vector< short > >::const_iterator mit2;
#endif SERV_ITEM_OPTION_DATA_SIZE
		//}} 
		mit2 = kSetItemData.m_mapNeedPartsNumNOptions.begin();
		for( ; mit2 != kSetItemData.m_mapNeedPartsNumNOptions.end(); ++mit2 )
		{
			if( mit2->first <= partsNum )
			{
				vecOptions = mit2->second;
			}
		}		
	}
}

//{{ 2012. 11. 28 캐시 아이템 소켓 옵션 기능 수정 - 김민성
#ifdef SERV_CASH_ITEM_SOCKET_OPTION
void CXSLSocketItem::AddSocektGroupDataForCashAvatar_LUA()
{

	KLuaManager luaManager( GetLuaState() );
	TableBind( &luaManager );

	int iItemType = 0;
	int iGroupID = 0;

	LUA_GET_VALUE_ENUM(		luaManager, L"ITEM_TYPE",			iItemType,			CXSLItem::ITEM_TYPE,	CXSLItem::IT_NONE );
	LUA_GET_VALUE(			luaManager, L"GROUP_ID",			iGroupID,			0 );

	std::set< int >	setSocketList;
	
	if( luaManager.BeginTable( L"SOCKET_ID_LIST" ) == S_OK )
	{
		int index	= 1; 
		int iSocketID		= 0;
		while( luaManager.GetValue( index, iSocketID ) == S_OK )
		{
			if( iSocketID > 0 )
			{
				setSocketList.insert( iSocketID );
			}
				
			index++;
		}

		luaManager.EndTable();
	}

	if( setSocketList.size() <= 0 )
	{
		START_LOG( cerr, L"[테스트]소켓ID 정보가 없다." )
			<< BUILD_LOG( iItemType )
			<< BUILD_LOG( static_cast<int>(setSocketList.size()) )
			<< END_LOG;
		return;
	}

	// 그룹 id 는 - 값 일 수 있음
	if( iGroupID == 0 || iItemType == CXSLItem::IT_NONE )
	{
		START_LOG( cerr, L"[테스트]파싱이 이상합니당." )
			<< BUILD_LOG( iGroupID )
			<< BUILD_LOG( iItemType )
			<< END_LOG;
		return;
	}

	{
		std::map< int, std::set< int > >::iterator mit = m_mapItemTypeSocketList.find( iItemType );
		if( mit == m_mapItemTypeSocketList.end() )
		{
			m_mapItemTypeSocketList.insert( std::make_pair( iItemType, setSocketList ) );
		}
		else
		{
			std::set< int >::const_iterator sit = setSocketList.begin();
			for( ; sit != setSocketList.end() ; ++sit )
			{
				mit->second.insert( (*sit) );
			}
		}
	}

	{
		std::map< int, std::set< int > >::iterator mit = m_mapItemGroupSocketList.find( iGroupID );
		if( mit == m_mapItemGroupSocketList.end() )
		{
			m_mapItemGroupSocketList.insert( std::make_pair( iGroupID, setSocketList ) );
		}
		else
		{
			std::set< int >::const_iterator sit = setSocketList.begin();
			for( ; sit != setSocketList.end() ; ++sit )
			{
				mit->second.insert( (*sit) );
			}
		}
	}
}

bool CXSLSocketItem::CheckCashItemSocketID( IN int iType, IN int iSocketID )
{
	std::map< int, std::set< int > >::iterator mit = m_mapItemTypeSocketList.find( iType );
	if( mit == m_mapItemTypeSocketList.end() )
	{
		START_LOG( cerr, L"[테스트]존재 하지 않는 아이템 타입입니다." )
			<< BUILD_LOG( iType )
			<< BUILD_LOG( iSocketID )
			<< END_LOG;
		return false;
	}

	std::set< int >::iterator sit = mit->second.find( iSocketID );
	if( sit != mit->second.end() )
	{
		return true;
	}

	return false;
}

bool CXSLSocketItem::CheckCashItemGroupID( IN int iSocketID, IN int iGroupID )
{
	std::map< int, std::set< int > >::iterator mit = m_mapItemGroupSocketList.find( iGroupID );
	if( mit == m_mapItemGroupSocketList.end() )
	{
		START_LOG( cerr, L"[테스트]존재 하지 않는 그룹 ID 입니다." )
			<< BUILD_LOG( iGroupID )
			<< BUILD_LOG( iSocketID )
			<< END_LOG;
		return false;
	}

	std::set< int >::iterator sit = mit->second.find( iSocketID );
	if( sit != mit->second.end() )
	{
		return true;
	}

	return false;
}
#endif SERV_CASH_ITEM_SOCKET_OPTION
//}

//{{ 2013. 05. 15	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
bool CXSLSocketItem::AddRandomSocketGroupInfo_LUA( int iGroupID, int iOptionID, float fRate )
{
	if( iGroupID <= 0 || iOptionID <= 0 || fRate <= 0.0f )
	{
		START_LOG( cerr, L"소켓 옵션 정보가 이상합니다." )
			<< BUILD_LOG( iGroupID )
			<< BUILD_LOG( iOptionID )
			<< BUILD_LOG( fRate )
			<< END_LOG;
		return false;
	}

	std::map< int, KLottery >::iterator mit;
	mit = m_mapRandomSocketGroup.find( iGroupID );
	if( mit == m_mapRandomSocketGroup.end() )
	{
		KLottery kLot;
		kLot.AddCase( iOptionID, fRate );
		m_mapRandomSocketGroup.insert( std::make_pair( iGroupID, kLot ) );
	}
	else
	{
		mit->second.AddCase( iOptionID, fRate );
	}

	return true;
}

bool CXSLSocketItem::EvaluateItemRandomSocket( IN const int iItemID, OUT std::vector< int >& vecRandomSocket )
{
	const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iItemID );
	if( IS_NULL( pItemTemplet ) )
	{
		START_LOG( cerr, L"존재 하지 않는 아이템 입니다!" )
			<< BUILD_LOG( iItemID )
			<< END_LOG;
		return false;
	}

	BOOST_TEST_FOREACH( const int, iRandomSocketGroupID, pItemTemplet->m_vecRandomSocketGroupID )
	{
		std::map< int, KLottery >::const_iterator mit;
		mit = m_mapRandomSocketGroup.find( iRandomSocketGroupID );
		if( mit == m_mapRandomSocketGroup.end() )
		{
			START_LOG( cerr, L"존재 하지 않는 랜덤 소켓 그룹 id입니다!" )
				<< BUILD_LOG( iItemID )
				<< BUILD_LOG( iRandomSocketGroupID )
				<< END_LOG;
			return false;
		}

        int iResultSocketOptionID = mit->second.Decision();
		if( iResultSocketOptionID == KLottery::CASE_BLANK )
		{
			iResultSocketOptionID = mit->second.GetFirstCase();
		}

		vecRandomSocket.push_back( iResultSocketOptionID );
	}
    
    return true;    
}

bool CXSLSocketItem::AddMiracleCubeInfo_LUA( int iItemID, int iMinLevel, int iMaxLevel )
{
	if( iItemID <= 0 || iMinLevel < 0 || iMaxLevel < 0 )
	{
		START_LOG( cerr, L"미라클 큐브 정보가 이상합니다." )
			<< BUILD_LOG( iItemID )
			<< BUILD_LOG( iMinLevel )
			<< BUILD_LOG( iMaxLevel )
			<< END_LOG;
		return false;
	}

	std::map< int, SMiracleCubeLevel >::iterator mit;
	mit = m_mapMiracleCubeTable.find( iItemID );
	if( mit != m_mapMiracleCubeTable.end() )
	{
		START_LOG( cerr, L"이미 등록된 미라클 큐브 정보입니다." )
			<< BUILD_LOG( iItemID )
			<< BUILD_LOG( iMinLevel )
			<< BUILD_LOG( iMaxLevel )
			<< END_LOG;	
		return false;
	}
	
	SMiracleCubeLevel kInfo;
	kInfo.m_iMinLevel = iMinLevel;
	kInfo.m_iMaxLevel = iMaxLevel;
	m_mapMiracleCubeTable.insert( std::make_pair( iItemID, kInfo ) );
	return true;
}

bool CXSLSocketItem::IsCanUseMiracleCube( IN const int iMiracleCubeItemID, IN const int iItemLevel ) const
{
	std::map< int, SMiracleCubeLevel >::const_iterator mit;
	mit = m_mapMiracleCubeTable.find( iMiracleCubeItemID );
	if( mit == m_mapMiracleCubeTable.end() )
	{
		START_LOG( cerr, L"미라클 큐브가 아닙니다." )
			<< BUILD_LOG( iMiracleCubeItemID )
			<< END_LOG;
		return false;
	}

	return ( mit->second.m_iMinLevel <= iItemLevel  &&  iItemLevel <= mit->second.m_iMaxLevel );
}

void CXSLSocketItem::GetAndAddStatRelationLevel( IN const std::vector< int >& vecSocketOption, IN OUT KStatRelationLevel& kAddStatRelLevel ) const
{
    BOOST_TEST_FOREACH( const int, iOptionID, vecSocketOption )
	{
		if( iOptionID == 0 )
		{
			continue;
		}

		const SocketData* pSocketData = GetSocketData( iOptionID );
		if( IS_NULL( pSocketData ) )
		{
			START_LOG( cerr, L"존재 하지 않는 옵션 ID입니다." )
				<< BUILD_LOG( iOptionID )
				<< END_LOG;
            continue;
		}

		// 모두 합산 하자!
		kAddStatRelLevel.AddStat( pSocketData->m_kStatRelLVData );
	}
}
#endif SERV_NEW_ITEM_SYSTEM_2013_05
//}}


