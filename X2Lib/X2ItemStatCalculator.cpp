#include "StdAfx.h"
#include ".\X2ItemStatCalculator.h"

#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
CX2ItemStatCalculator::CX2ItemStatCalculator():
m_eUnitType(CX2Unit::UT_NONE),
m_eUnitClass(CX2Unit::UC_NONE)
{

}

CX2ItemStatCalculator::~CX2ItemStatCalculator()
{

}

void CX2ItemStatCalculator::OpenScriptFile()
{
	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState(), 0, false );
	if( true == g_pKTDXApp->LoadLuaTinker( L"CharacterStatTable.lua" ) )
	{
		// 장비 장착 부위 별 능력치
		if( true == luaManager.BeginTable( "EQIP_POSITION_RELATIVE_STAT_TABLE" ) )
		{
			int iTableIndex = 1;
			while( true == luaManager.BeginTable( iTableIndex  ) )
			{
				CX2Unit::EQIP_POSITION eEqipPosition = CX2Unit::EP_NONE;

				LUA_GET_VALUE_ENUM( luaManager,		TDT_TYPE,		eEqipPosition,		CX2Unit::EQIP_POSITION,		CX2Unit::EP_NONE );
				if( CX2Unit::EP_NONE == eEqipPosition )
				{
					luaManager.EndTable();
					ASSERT(!"EQIP_POSITION_RELATIVE_STAT_TABLE Error");
					break;
				}

				CX2Item::KItemFormatStatData sStatData;

				LUA_GET_VALUE( luaManager,		TDT_ATK_PHYSIC,		sStatData.m_fAtkPhysic,		0.f );
				LUA_GET_VALUE( luaManager,		TDT_DEF_PHYSIC,		sStatData.m_fDefPhysic,		0.f );
				LUA_GET_VALUE( luaManager,		TDT_ATK_MAGIC,		sStatData.m_fAtkMagic,		0.f );
				LUA_GET_VALUE( luaManager,		TDT_DEF_MAGIC,		sStatData.m_fDefMagic,		0.f );
				LUA_GET_VALUE( luaManager,		TDT_HP,				sStatData.m_fBaseHP,		0.f );

				m_mapEqipStatDataTable.insert( std::make_pair( eEqipPosition, sStatData ) );

				++iTableIndex;
				luaManager.EndTable();
			}

			luaManager.EndTable();
		}

		// 캐릭터별 능력치
		if( true == luaManager.BeginTable( "CHARACTER_RELATIVE_STAT_TABLE" ) )
		{
			int iTableIndex = 1;
			while( true == luaManager.BeginTable( iTableIndex  ) )
			{
				CX2Unit::UNIT_TYPE eUnitType = CX2Unit::UT_NONE;

				LUA_GET_VALUE_ENUM( luaManager,		TDT_TYPE,		eUnitType,		CX2Unit::UNIT_TYPE,		CX2Unit::UT_NONE );
				if( CX2Unit::UC_NONE == eUnitType )
				{
					luaManager.EndTable();
					ASSERT(!"CHARACTER_RELATIVE_STAT_TABLE Error");
					break;
				}

				CX2Item::KItemFormatStatData sStatData;

				LUA_GET_VALUE( luaManager,		TDT_ATK_PHYSIC,		sStatData.m_fAtkPhysic,		0.f );
				LUA_GET_VALUE( luaManager,		TDT_DEF_PHYSIC,		sStatData.m_fDefPhysic,		0.f );
				LUA_GET_VALUE( luaManager,		TDT_ATK_MAGIC,		sStatData.m_fAtkMagic,		0.f );
				LUA_GET_VALUE( luaManager,		TDT_DEF_MAGIC,		sStatData.m_fDefMagic,		0.f );
				LUA_GET_VALUE( luaManager,		TDT_HP,				sStatData.m_fBaseHP,		0.f );

				m_mapCharStatDataTable.insert( std::make_pair( eUnitType, sStatData ) );

				++iTableIndex;
				luaManager.EndTable();
			}

			luaManager.EndTable();
		}

		// 2차 전직별 능력치( 2차전직이 아니라면 캐릭터별 능력치 사용 )
		if( true == luaManager.BeginTable( "SECOND_CLASS_RELATIVE_STAT_TABLE" ) )
		{
			int iTableIndex = 1;
			while( true == luaManager.BeginTable( iTableIndex  ) )
			{
				CX2Unit::UNIT_CLASS eUnitClass = CX2Unit::UC_NONE;

				LUA_GET_VALUE_ENUM( luaManager,		TDT_TYPE,		eUnitClass,		CX2Unit::UNIT_CLASS,		CX2Unit::UC_NONE );
				if( CX2Unit::UT_NONE == eUnitClass )
				{
					luaManager.EndTable();
					ASSERT(!"SECOND_CLASS_RELATIVE_STAT_TABLE Error");
					break;
				}

				CX2Item::KItemFormatStatData sStatData;

				LUA_GET_VALUE( luaManager,		TDT_ATK_PHYSIC,		sStatData.m_fAtkPhysic,		0.f );
				LUA_GET_VALUE( luaManager,		TDT_DEF_PHYSIC,		sStatData.m_fDefPhysic,		0.f );
				LUA_GET_VALUE( luaManager,		TDT_ATK_MAGIC,		sStatData.m_fAtkMagic,		0.f );
				LUA_GET_VALUE( luaManager,		TDT_DEF_MAGIC,		sStatData.m_fDefMagic,		0.f );
				LUA_GET_VALUE( luaManager,		TDT_HP,				sStatData.m_fBaseHP,		0.f );

				m_mapClassStatDataTable.insert( std::make_pair( eUnitClass, sStatData ) );

				++iTableIndex;
				luaManager.EndTable();
			}

			luaManager.EndTable();
		}
	}

	return;
}

void CX2ItemStatCalculator::ResetItemStatCalculator()
{
	m_mapEqipStatDataTable.clear();
	m_mapCharStatDataTable.clear();
	m_mapClassStatDataTable.clear();

	OpenScriptFile();

	if( false == InvalideDataCheck() )
	{
		ErrorLog( XEM_ERROR148 )
		ASSERT(!"CharacterStatTable.lua Error");
	}
}

/** @function : InvalideDataCheck
	@brief : 스크립트로부터 파싱한 데이터의 유효성 체크
			 -> 전직이 추가 되었을 때, 스크립트에 전직을 추가하지 않는 상황 등에서
				경고를 할 수 있도록 하는 기능
*/
bool CX2ItemStatCalculator::InvalideDataCheck() const
{
	for( int iUnitType = (CX2Unit::UT_NONE + 1); iUnitType < CX2Unit::UT_END; ++iUnitType )
	{
		if( m_mapCharStatDataTable.find( static_cast<CX2Unit::UNIT_TYPE>(iUnitType) ) 
			== m_mapCharStatDataTable.end() )
			return false;
	}

	// 2차 전직 ID 첫번 째 값 CX2Unit::UC_ELSWORD_LORD_KNIGHT
	for( int iUnitClass = CX2Unit::UC_ELSWORD_LORD_KNIGHT ; iUnitClass < CX2Unit::UC_VALIDE_END; ++iUnitClass )
	{
		if( m_mapClassStatDataTable.find( static_cast<CX2Unit::UNIT_CLASS>(iUnitClass) ) 
			== m_mapClassStatDataTable.end() )
			return false;
	}

	if( m_mapEqipStatDataTable.find( CX2Unit::EP_WEAPON_HAND ) == m_mapEqipStatDataTable.end() )
		return false;

	if( m_mapEqipStatDataTable.find( CX2Unit::EP_DEFENCE_BODY ) == m_mapEqipStatDataTable.end() )
		return false;

	if( m_mapEqipStatDataTable.find( CX2Unit::EP_DEFENCE_LEG ) == m_mapEqipStatDataTable.end() )
		return false;

	if( m_mapEqipStatDataTable.find( CX2Unit::EP_DEFENCE_HAND ) == m_mapEqipStatDataTable.end() )
		return false;

	if( m_mapEqipStatDataTable.find( CX2Unit::EP_DEFENCE_FOOT ) == m_mapEqipStatDataTable.end() )
		return false;

	return true;
}

/** @function : GetCalculateItemStat
	@brief : ->아이템템플릿의 정보와 내 캐릭터 정보를 연동하여 스탯 정보 계산
			 ->멤버변수로 가지고 있는 내 클래스 정보로 스탯 계산 할 경우 
			   m_mapItemTemplmetStat에 계산 결과 담고, 이 후 체크 할 때는 계산된 결과로 부터 값 설정
			 ->스탯연동 추가 레벨이 0일 경우 일반 아이템으로 판단하고, 
			   item.lua에 설정된 스탯으로 넘겨줌
			   */
void CX2ItemStatCalculator::CalculateItemStat( OUT CX2Item::KItemFormatStatData& sRealStatData_, IN const CX2Item::ItemTemplet* pItemTemplet_ ,
												IN const CX2SocketItem::KItemStatRelLVData& sStatRelLVData_ /*= CX2SocketItem::ms_KDummyItemStatRelLVData*/,
												IN CX2Unit::UNIT_TYPE eUnitType_ /*= CX2Unit::UT_NONE*/, IN CX2Unit::UNIT_CLASS eUnitClass_ /*= CX2Unit::UC_NONE*/ )
{
	if( NULL == pItemTemplet_ )
		return;

	if( CX2Unit::UC_NONE == eUnitType_ || CX2Unit::UC_NONE == eUnitClass_ )
	{ // UNIT_TYPE or UNIT_CLASS를 받지 못 했다면 저장된 내 캐릭터 정보로 설정
		eUnitType_ = m_eUnitType;
		eUnitClass_ = m_eUnitClass;
	}

	CX2SocketItem::KItemStatRelLVData kStatRelationAddLV; 
	// 소켓 옵션과, 아이템에 고정된 추가 레벨 값 합산
	{
		// 소켓 옵션에 의한 추가 설정
		kStatRelationAddLV.AddStat( sStatRelLVData_ );

		// 아이템 레벨에 의한 추가 레벨 설정
		const CX2Item::KItemFormatStatRelLVData& kItemTempletStatRelationLV = pItemTemplet_->GetStatRelLVData();
		kStatRelationAddLV.m_fBaseHPRelLV		+= static_cast<float>(kItemTempletStatRelationLV.m_byBaseHPRelLV);
		kStatRelationAddLV.m_fAtkPhysicRelLV	+= static_cast<float>(kItemTempletStatRelationLV.m_byAtkPhysicRelLV);
		kStatRelationAddLV.m_fAtkMagicRelLV		+= static_cast<float>(kItemTempletStatRelationLV.m_byAtkMagicRelLV);
		kStatRelationAddLV.m_fDefPhysicRelLV	+= static_cast<float>(kItemTempletStatRelationLV.m_byDefPhysicRelLV);
		kStatRelationAddLV.m_fDefMagicRelLV		+= static_cast<float>(kItemTempletStatRelationLV.m_byDefMagicRelLV);
	}

	// 연동 레벨이 설정되어있지 않다면 기본 스탯 구하기
	if( false == kStatRelationAddLV.IsValideCheck() || 
		( CX2Unit::UC_NONE == eUnitType_ || CX2Unit::UC_NONE == eUnitClass_ ) )
	{
		sRealStatData_ = pItemTemplet_->GetStatData();
		return;
	}
	else
	{	
		// 키값 생성
		ItemStatKey sItemStatKey( pItemTemplet_->GetItemID(), eUnitClass_, kStatRelationAddLV );

		// 한 번 계산한 아이템이라면 더이상 계산하지 않고 map에 담겨진 정보 이용
		map< ItemStatKey, CX2Item::KItemFormatStatData >::iterator it = m_mapItemTemplmetStat.find( sItemStatKey );
		if ( m_mapItemTemplmetStat.end() != it )
		{
			sRealStatData_ = it->second;
			return;
		}

		// 캐릭터별, 전직별, 장착부위별 캐릭터 스탯 테이블 상수 얻기
		CX2Item::KItemFormatStatData sCharStatTable, sClassStatTable, sEqipStatTable ;
		GetStatDataTable( eUnitType_, eUnitClass_, pItemTemplet_->GetEqipPosition(), 
			sCharStatTable, sClassStatTable, sEqipStatTable );

		int iItemLevel = pItemTemplet_->GetUseLevel();  

		// 얻은 정보를 토대로 계산하기
		CalculateRealStatData(sRealStatData_, sClassStatTable, sCharStatTable, 
			sEqipStatTable,	iItemLevel, kStatRelationAddLV);

		if( m_mapItemTemplmetStat.size() >= _CONST_::MAX_ITEM_TEMPLET_STAT_COUNT )
			m_mapItemTemplmetStat.clear();

		m_mapItemTemplmetStat.insert( std::make_pair( sItemStatKey, sRealStatData_ ));
	}
}

/** @function : GetStatDataTable
	@brief : 스크립트로부터 파싱한 정보를 토대로 스탯테이블 설정
*/
void CX2ItemStatCalculator::GetStatDataTable( IN CX2Unit::UNIT_TYPE eUnitType_,
											IN CX2Unit::UNIT_CLASS eUnitClass_,
											IN CX2Unit::EQIP_POSITION eEqipPosition_, 
											OUT CX2Item::KItemFormatStatData& sCharStatTable_, 
											OUT CX2Item::KItemFormatStatData& sClassStatTable_, 
											OUT CX2Item::KItemFormatStatData& sEqipStatTable_ ) const
{
	// 캐릭터별 스탯 테이블
	map< CX2Unit::UNIT_TYPE, CX2Item::KItemFormatStatData >::const_iterator itChar = m_mapCharStatDataTable.find(eUnitType_);  
	if( m_mapCharStatDataTable.end() != itChar )
	{
		sCharStatTable_ = itChar->second;
	}
	else
	{
		ASSERT(!"GetStatDataTable Error");
	}

	// 전직별 스탯 테이블
	if( 2 == GetX2UnitClassGrade( eUnitClass_ ) )
	{
		map< CX2Unit::UNIT_CLASS, CX2Item::KItemFormatStatData >::const_iterator itClass = m_mapClassStatDataTable.find(eUnitClass_);
		if( m_mapClassStatDataTable.end() != itClass )
		{
			sClassStatTable_ = itClass->second;
		}
	}
	else
	{
		// 2차 전직이 아니라면 기본 캐릭터별 스탯 테이블 사용
		sClassStatTable_ = sCharStatTable_;
	}

	// 장비 장착 부위별 스탯 테이블	
	map< CX2Unit::EQIP_POSITION, CX2Item::KItemFormatStatData >::const_iterator itEqip = m_mapEqipStatDataTable.find( eEqipPosition_ );
	if( m_mapEqipStatDataTable.end() != itEqip )
	{
		sEqipStatTable_ = itEqip->second;
	}
	else
	{
		ASSERT(!"GetStatDataTable Error");
	}
}

/** @function : SetUnitData
	@brief : 스탯테이블 자료를 기반으로 CharacterStatTable.lua 스크립트의 계산 함수를 통해 스탯 계산
*/
void CX2ItemStatCalculator::CalculateRealStatData( OUT CX2Item::KItemFormatStatData& sRealStatData_,
													IN const CX2Item::KItemFormatStatData& sClassStatTable_, 
													IN const CX2Item::KItemFormatStatData& sCharStatTable_, 
													IN const CX2Item::KItemFormatStatData& sEqipStat_, 
													IN int iItemLevel_,
													IN const CX2SocketItem::KItemStatRelLVData& sStatRelLVData_ ) const
{
	//물공 
	sRealStatData_.m_fAtkPhysic = 
		lua_tinker::call<float>( g_pKTDXApp->GetLuaBinder()->GetLuaState(),	"CalculatePhysicalAndMagicalStat", 
		sClassStatTable_.m_fAtkPhysic, sCharStatTable_.m_fAtkPhysic, 
		sEqipStat_.m_fAtkPhysic, iItemLevel_, sStatRelLVData_.m_fAtkPhysicRelLV );

	//물방
	sRealStatData_.m_fDefPhysic = 
		lua_tinker::call<float>( g_pKTDXApp->GetLuaBinder()->GetLuaState(),	"CalculatePhysicalAndMagicalStat", 
		sClassStatTable_.m_fDefPhysic, sCharStatTable_.m_fDefPhysic, 
		sEqipStat_.m_fDefPhysic, iItemLevel_, sStatRelLVData_.m_fDefPhysicRelLV );

	//마공
	sRealStatData_.m_fAtkMagic = 
		lua_tinker::call<float>( g_pKTDXApp->GetLuaBinder()->GetLuaState(),	"CalculatePhysicalAndMagicalStat", 
		sClassStatTable_.m_fAtkMagic, sCharStatTable_.m_fAtkMagic, 
		sEqipStat_.m_fAtkMagic, iItemLevel_, sStatRelLVData_.m_fAtkMagicRelLV );

	//마방
	sRealStatData_.m_fDefMagic =
		lua_tinker::call<float>( g_pKTDXApp->GetLuaBinder()->GetLuaState(),	"CalculatePhysicalAndMagicalStat", 
		sClassStatTable_.m_fDefMagic, sCharStatTable_.m_fDefMagic, 
		sEqipStat_.m_fDefMagic, iItemLevel_, sStatRelLVData_.m_fDefMagicRelLV );

	//체력
	sRealStatData_.m_fBaseHP = 
		lua_tinker::call<float>( g_pKTDXApp->GetLuaBinder()->GetLuaState(),	"CalculateHPStat", 
		sClassStatTable_.m_fBaseHP, sCharStatTable_.m_fBaseHP, 
		sEqipStat_.m_fBaseHP, iItemLevel_, sStatRelLVData_.m_fBaseHPRelLV );
}

/** @function : SetUnitData
	@brief : 현재 선택한 유닛의 정보를 설정.
	    	 g_pData->GetMyUser()->SelectUnit( pUnit ) 가 호출되는 경우 반드시 불려져야 한다.
*/
void CX2ItemStatCalculator::SetUnitData( CX2Unit::UNIT_TYPE eUnitType_, CX2Unit::UNIT_CLASS eUnitClass_ )
{
	m_eUnitType = eUnitType_;
	m_eUnitClass = eUnitClass_;

	// 타입, 클래스가 변경되면 스탯 정보도 변경되기 때문에 맵 초기화
	m_mapItemTemplmetStat.clear();
}
/** @function : GetItemDescDev
	@brief : 사내 테스트를 위해 보조 디스크립션
*/
void CX2ItemStatCalculator::GetItemDescDev( OUT wstring& wstrItemDesc_, 
	IN const CX2Item::ItemTemplet* pItemTemplet ) const
{
#ifndef _IN_HOUSE_
	return;
#endif //_IN_HOUSE_

	if( NULL == pItemTemplet )
		return;

	if ( g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_OPERATOR )
	{
		const CX2Item::KItemFormatStatRelLVData kStatRelLV = pItemTemplet->GetStatRelLVData();
		if( 0 == (	kStatRelLV.m_byAtkPhysicRelLV + kStatRelLV.m_byAtkMagicRelLV + kStatRelLV.m_byDefPhysicRelLV +
					kStatRelLV.m_byDefMagicRelLV + kStatRelLV.m_byBaseHPRelLV ) )
		{
			// 모든 값이 0이라면 출력해주지 않기.
			return;
		}

		wstrItemDesc_ += L"#CFFFF0";
		wstrItemDesc_ += L"----------DEV-----------\n";
		wstrItemDesc_ += L"아이템 스크립트의 능력치\n";

		WCHAR buff[256] = {0};
		vector<int> vecRandomSocketID;
		pItemTemplet->GetRandomSocketOptions(vecRandomSocketID);
		if( false == vecRandomSocketID.empty() )
		{
			wstrItemDesc_ += L"Random Socket GroupID : ";
			BOOST_FOREACH( int iSocketGroupID, vecRandomSocketID )
			{
				StringCchPrintf( buff, 256, L"%d,", iSocketGroupID );
				wstrItemDesc_ += buff;
			}
			wstrItemDesc_ += L"\n";
		}
		
		if( 0 != kStatRelLV.m_byAtkPhysicRelLV )
		{
			StringCchPrintf( buff, 256, L"AtkPhysic LV : %d\n", kStatRelLV.m_byAtkPhysicRelLV);
			wstrItemDesc_ += buff;
		}
		if( 0 != kStatRelLV.m_byAtkMagicRelLV )
		{
			StringCchPrintf( buff, 256, L"AtkMagic LV : %d\n", kStatRelLV.m_byAtkMagicRelLV);
			wstrItemDesc_ += buff;
		}
		if( 0 != kStatRelLV.m_byDefPhysicRelLV )
		{
			StringCchPrintf( buff, 256, L"DefPhysic LV : %d\n", kStatRelLV.m_byDefPhysicRelLV);
			wstrItemDesc_ += buff;
		}
		if( 0 != kStatRelLV.m_byDefMagicRelLV )
		{
			StringCchPrintf( buff, 256, L"DefMagic LV : %d\n", kStatRelLV.m_byDefMagicRelLV);
			wstrItemDesc_ += buff;
		}
		if( 0 != kStatRelLV.m_byBaseHPRelLV )
		{
			StringCchPrintf( buff, 256, L"HP LV : %d\n", kStatRelLV.m_byBaseHPRelLV );
			wstrItemDesc_ += buff;
		}

		wstrItemDesc_ += L"----------DEV-----------\n";
		wstrItemDesc_ += L"#CX";
	}
}

bool CX2ItemStatCalculator::IsRandomSocketOptionItem( const CX2Item::ItemTemplet* pItemTemplet ) const
{
	if( NULL == pItemTemplet )
		return false;

	if( 0 == pItemTemplet->GetNumRandomSocketOption() )
		return false;

	if( true == pItemTemplet->GetFashion() )
		return false;

	return true;
};

/** @function : GetSocketOptionStatRelLV
	@brief : 스탯 추가 레벨 적용 옵션 값 얻어오기
*/
void CX2ItemStatCalculator::GetSocketOptionStatRelLV( OUT CX2SocketItem::KItemStatRelLVData& kItemStatRelLVData_,
	IN const CX2Item::ItemData& kItemData_, IN const CX2Item::ItemTemplet* pItemTemplet_) const
{
	kItemStatRelLVData_.Init();
	if( NULL != g_pData->GetSocketItem() )
	{
		// 아이템에 고정되어 있는 소켓 옵션
		if( NULL != pItemTemplet_ )
		{
			vector<int> vecSocketOption;
			pItemTemplet_->GetSocketOptions( vecSocketOption );
			BOOST_FOREACH( int iSocketID, vecSocketOption )
			{
				kItemStatRelLVData_.AddStat( g_pData->GetSocketItem()->GetStatRelLVDataBySocktID( iSocketID ) );
			}
		}

		{
			// 마법석으로 추가하는 소켓 옵션
			BOOST_FOREACH( int iSocketID, kItemData_.m_SocketOption )
			{
				kItemStatRelLVData_.AddStat( g_pData->GetSocketItem()->GetStatRelLVDataBySocktID( iSocketID ) );
			}

			// 감정을 통해 얻는 랜덤 소켓 옵션
			BOOST_FOREACH( int iSocketID, kItemData_.m_vecRandomSocket )
			{
				kItemStatRelLVData_.AddStat( g_pData->GetSocketItem()->GetStatRelLVDataBySocktID( iSocketID ) );
			}
		}
	}
}

#endif //SERV_NEW_ITEM_SYSTEM_2013_05