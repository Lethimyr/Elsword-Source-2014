#include "StdAfx.h"
#include ".\x2combotree.h"




#ifdef COMBO_TREE_UI_TEST

const CX2ComboTree::ComboSet* CX2ComboTree::GetUnitComboSet( CX2Unit::UNIT_CLASS eUnitClass )
{
	UnitComboMap::iterator it = m_mapUnitCombo.find( eUnitClass );
	if( m_mapUnitCombo.end() == it )
		return NULL;

	return &it->second;
}

int CX2ComboTree::GetComboIconLayer( COMBO_TREE_ICON eIcon )
{
	switch( eIcon )
	{
	case CTI_LINE_RIGHT:
	case CTI_LINE_UP_RIGHT:
	case CTI_LINE_DOWN:
	case CTI_LINE_DOWN_RIGHT:
	case CTI_LINE_DOUBLE_UP_RIGHT:
	case CTI_LINE_DOUBLE_DOWN_RIGHT:
//#ifdef BALANCE_ELEMENTAL_MASTER_20130117		/// 꺽인선 추가
	case CTI_LINE_DOWN_DOUBLE_RIGHT:
	case CTI_LINE_THREEWAY_INTERSECTION:
//#endif BALANCE_ELEMENTAL_MASTER_20130117
		{
			return 0;
		} break;

	case CTI_BACKGROUND:
		{
			return 1;
		} break;

//#ifdef SERV_RENA_NIGHT_WATCHER
	case CTI_ICON_ACCEL_DASH:
	case CTI_ICON_EXPLOSIVE_ARROW:
//#endif SERV_RENA_NIGHT_WATCHER
//#ifdef ARA_CHARACTER_BASE
	case CTI_ICON_SPEAR:
//#endif ARA_CHARACTER_BASE
//#ifdef BALANCE_ELEMENTAL_MASTER_20130117
	case CTI_ICON_HURRICANE:					/// 엘리멘탈 마스터 허리케인
//#endif BALANCE_ELEMENTAL_MASTER_20130117
#ifdef SERV_9TH_NEW_CHARACTER //JHKang
	case CTI_ICON_DYNAMO_Z:
	case CTI_ICON_DYNAMO_X:
	case CTI_ICON_DYNAMO_ROLLING:
	case CTI_ICON_DYNAMO_TELEPORT:
	case CTI_ICON_FIST_NA:
	case CTI_ICON_KICK_NA:
	case CTI_ICON_NA:
#endif //SERV_9TH_NEW_CHARACTER
		{
			return 2;
		} break;
	}

#ifdef CHUNG_SECOND_CLASS_CHANGE
	if( eIcon >= CTI_ICON_JUMP &&
		eIcon <= CTI_ICON_LANDING )
#else
	if( eIcon >= CTI_ICON_JUMP &&
		eIcon <= CTI_ICON_CANNON_BALL_CHARGE )	// kimhc // 2010.12.20 // 청 아이콘 추가로 변경함
#endif CHUNG_SECOND_CLASS_CHANGE
	{
		return 2;
	}

	return 3;
}

void CX2ComboTree::ParseComboSet( KLuaManager &luaManager, std::vector<ComboIcon>& vecComboSet )
{
	float fGridX = 0;
	float fGridY = 0;
	int iComboIcon = (int) CTI_INVALID;
	int iLayer = 0;

	const int MAGIC_NO_INFINITE_LOOP = 5000;
	for( int i=1; i < MAGIC_NO_INFINITE_LOOP ; i+=3 )
	{
		if( false == luaManager.GetValue( i, fGridX ) )
			break;

		if( false == luaManager.GetValue( i+1, fGridY ) )
			break;

		if( false == luaManager.GetValue( i+2, iComboIcon ) )
			break;

		iLayer = GetComboIconLayer( (COMBO_TREE_ICON) iComboIcon );
		vecComboSet.push_back( ComboIcon( fGridX, fGridY, (COMBO_TREE_ICON) iComboIcon, iLayer ) );


		switch( (COMBO_TREE_ICON) iComboIcon )
		{
		case CTI_ICON_JUMP:
		case CTI_ICON_DOUBLE_JUMP:
		case CTI_ICON_DASH:
		case CTI_ICON_DASH_JUMP:
		case CTI_ICON_SWORD:
		case CTI_ICON_STAFF:
		case CTI_ICON_FIST:
		case CTI_ICON_KICK:
		case CTI_ICON_GEAR:
		case CTI_ICON_FIRE_BALL:
		case CTI_ICON_ARROW:
		case CTI_ICON_TRIPLE_ARROW:
		case CTI_ICON_MAGIC_AREA:
		case CTI_ICON_DARK_FIRE_BALL:
		case CTI_ICON_ELECTRON_BALL:
		case CTI_ICON_EXPLOSION:
		case CTI_ICON_TRIANGLE:
		case CTI_ICON_RUNE_AREA:
		case CTI_ICON_SPARK:
			//{{ kimhc // 2010.12.20 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
		case CX2ComboTree::CTI_ICON_CANNON:
		case CX2ComboTree::CTI_ICON_CANNON_BALL:
		case CX2ComboTree::CTI_ICON_CANNON_BALL_CHARGE:
#endif	NEW_CHARACTER_CHUNG
			//}} kimhc // 2010.12.20 //  2010-12-23 New Character CHUNG

//{{ kimhc // 2011.1.24 // 청 1차 전직
#ifdef	CHUNG_FIRST_CLASS_CHANGE
		case CX2ComboTree::CTI_ICON_HANDGUN:
#endif	CHUNG_FIRST_CLASS_CHANGE
//}} kimhc // 2011.1.24 // 청 1차 전직
#ifdef CHUNG_SECOND_CLASS_CHANGE
		case CX2ComboTree::CTI_ICON_LANDING:
#endif CHUNG_SECOND_CLASS_CHANGE
#ifdef EVE_ELECTRA
		case CX2ComboTree::CTI_ICON_BEAM:
#endif EVE_ELECTRA
#ifdef SERV_ARME_DIMENSION_WITCH
		case CX2ComboTree::CTI_ICON_BLACKHOLE:
#endif
#ifdef SERV_RENA_NIGHT_WATCHER
		case CX2ComboTree::CTI_ICON_ACCEL_DASH:
		case CX2ComboTree::CTI_ICON_EXPLOSIVE_ARROW:
#endif SERV_RENA_NIGHT_WATCHER
		case CX2ComboTree::CTI_ICON_SIEGE:
		case CX2ComboTree::CTI_ICON_SIEGE_SHELLING:
#ifdef BALANCE_ELEMENTAL_MASTER_20130117
		case CX2ComboTree::CTI_ICON_HURRICANE:					/// 엘리멘탈 마스터 허리케인
#endif BALANCE_ELEMENTAL_MASTER_20130117
#ifdef SERV_9TH_NEW_CHARACTER //JHKang
		case CTI_ICON_DYNAMO_Z:
		case CTI_ICON_DYNAMO_X:
		case CTI_ICON_DYNAMO_ROLLING:
		case CTI_ICON_DYNAMO_TELEPORT:
		case CTI_ICON_FIST_NA:
		case CTI_ICON_KICK_NA:
		case CTI_ICON_NA:
#endif //SERV_9TH_NEW_CHARACTER
			{
				iLayer = GetComboIconLayer( (COMBO_TREE_ICON) CTI_BACKGROUND );
				vecComboSet.push_back( ComboIcon( fGridX, fGridY, CTI_BACKGROUND, iLayer ) );
			} break;

#ifdef ARA_CHARACTER_BASE
		case CX2ComboTree::CTI_ICON_SPEAR:
			{
				iLayer = GetComboIconLayer( (COMBO_TREE_ICON) CTI_BACKGROUND );
				vecComboSet.push_back( ComboIcon( fGridX, fGridY, CTI_BACKGROUND, iLayer ) );
			} break;
#endif
		}
	}

	// layer 오름 차순으로 정렬
	std::sort( vecComboSet.begin(), vecComboSet.end() );
}

bool CX2ComboTree::OpenScriptFile( const std::wstring& filename )
{
	if( true == filename.empty() )
		return false;

	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState(), 0, true );
// 	if( false == g_pKTDXApp->LoadLuaTinker( filename.c_str() ) )
// 		return false;
	if( false == g_pKTDXApp->LoadAndDoMemory( &luaManager, filename.c_str() ) )
		return false;

	if ( false == luaManager.ExportFunctionsToGlobalEnv() )
		return false;

	std::vector< UnitClassName > vecUnitClassName;
	vecUnitClassName.reserve( 50 );
	vecUnitClassName.push_back( UnitClassName( CX2Unit::UC_ELSWORD_SWORDMAN,			"ELSWORD_SWORDMAN"		) );
	vecUnitClassName.push_back( UnitClassName( CX2Unit::UC_ARME_VIOLET_MAGE,			"ARME_VIOLET_MAGE"     ) );
	vecUnitClassName.push_back( UnitClassName( CX2Unit::UC_LIRE_ELVEN_RANGER,			"LIRE_ELVEN_RANGER"    ) );
	vecUnitClassName.push_back( UnitClassName( CX2Unit::UC_RAVEN_FIGHTER,				"RAVEN_FIGHTER"        ) );
	vecUnitClassName.push_back( UnitClassName( CX2Unit::UC_EVE_NASOD,					"EVE_NASOD"			) );
	//{{ kimhc // 2010.11.24 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
	vecUnitClassName.push_back( UnitClassName( CX2Unit::UC_CHUNG_IRON_CANNON,			"CHUNG_IRON_CANNON"	) );
#endif	NEW_CHARACTER_CHUNG
	//}} kimhc // 2010.11.24 //  2010-12-23 New Character CHUNG
#ifdef ARA_CHARACTER_BASE
	vecUnitClassName.push_back( UnitClassName( CX2Unit::UC_ARA_MARTIAL_ARTIST,			"ARA_MARTIAL_ARTIST"	) );
#endif
#ifdef NEW_CHARACTER_EL
	vecUnitClassName.push_back( UnitClassName( CX2Unit::UC_ELESIS_KNIGHT,				"ELESIS_KNIGHT"	) );
#endif // NEW_CHARACTER_EL
#ifdef SERV_9TH_NEW_CHARACTER // 김태환 ( 캐릭터 추가용 )
	vecUnitClassName.push_back( UnitClassName( CX2Unit::UC_ADD_NASOD_RULER,				"ADD_NASOD_RULER"	) );
#endif //SERV_9TH_NEW_CHARACTER

	vecUnitClassName.push_back( UnitClassName( CX2Unit::UC_ELSWORD_KNIGHT,				"ELSWORD_KNIGHT"       ) );
	vecUnitClassName.push_back( UnitClassName( CX2Unit::UC_ELSWORD_MAGIC_KNIGHT,		"ELSWORD_MAGIC_KNIGHT" ) );
#ifdef ELSWORD_SHEATH_KNIGHT
	vecUnitClassName.push_back( UnitClassName( CX2Unit::UC_ELSWORD_SHEATH_KNIGHT,		"ELSWORD_SHEATH_KNIGHT"	) );
#endif ELSWORD_SHEATH_KNIGHT
	vecUnitClassName.push_back( UnitClassName( CX2Unit::UC_LIRE_COMBAT_RANGER,			"LIRE_COMBAT_RANGER"   ) );
	vecUnitClassName.push_back( UnitClassName( CX2Unit::UC_LIRE_SNIPING_RANGER,			"LIRE_SNIPING_RANGER"  ) );
#ifdef SERV_TRAPPING_RANGER_TEST
	vecUnitClassName.push_back( UnitClassName( CX2Unit::UC_LIRE_TRAPPING_RANGER,		"LIRE_TRAPPING_RANGER"  ) );
#endif SERV_TRAPPING_RANGER_TEST
	vecUnitClassName.push_back( UnitClassName( CX2Unit::UC_ARME_HIGH_MAGICIAN,			"ARME_HIGH_MAGICIAN"   ) );
	vecUnitClassName.push_back( UnitClassName( CX2Unit::UC_ARME_DARK_MAGICIAN,			"ARME_DARK_MAGICIAN"   ) );
#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
	vecUnitClassName.push_back( UnitClassName( CX2Unit::UC_ARME_BATTLE_MAGICIAN,		"ARME_BATTLE_MAGICIAN"   ) );
#endif
	vecUnitClassName.push_back( UnitClassName( CX2Unit::UC_RAVEN_SOUL_TAKER,			"RAVEN_SOUL_TAKER"     ) );
	vecUnitClassName.push_back( UnitClassName( CX2Unit::UC_RAVEN_OVER_TAKER,			"RAVEN_OVER_TAKER"     ) );
#ifdef RAVEN_WEAPON_TAKER
	vecUnitClassName.push_back( UnitClassName( CX2Unit::UC_RAVEN_WEAPON_TAKER,			"RAVEN_WEAPON_TAKER"   ) );
#endif RAVEN_WEAPON_TAKER
	vecUnitClassName.push_back( UnitClassName( CX2Unit::UC_EVE_EXOTIC_GEAR,				"EVE_EXOTIC_GEAR"      ) );
	vecUnitClassName.push_back( UnitClassName( CX2Unit::UC_EVE_ARCHITECTURE,			"EVE_ARCHITECTURE"     ) );
#ifdef EVE_ELECTRA
	vecUnitClassName.push_back( UnitClassName( CX2Unit::UC_EVE_ELECTRA,					"EVE_ELECTRA"     ) );
#endif EVE_ELECTRA

	//{{ kimhc // 2011.1.3 // 청 1차 전직
#ifdef	CHUNG_FIRST_CLASS_CHANGE
	vecUnitClassName.push_back( UnitClassName( CX2Unit::UC_CHUNG_FURY_GUARDIAN,			"CHUNG_FURY_GUARDIAN"  ) );
	vecUnitClassName.push_back( UnitClassName( CX2Unit::UC_CHUNG_SHOOTING_GUARDIAN,		"CHUNG_SHOOTING_GUARDIAN" ) );
#endif	CHUNG_FIRST_CLASS_CHANGE
	//}} kimhc // 2011.1.3 // 청 1차 전직
#ifdef SERV_ADD_CHUNG_SHELLING_GUARDIAN
	vecUnitClassName.push_back( UnitClassName( CX2Unit::UC_CHUNG_SHELLING_GUARDIAN,		"CHUNG_SHELLING_GUARDIAN"  ) );
#endif //SERV_ADD_CHUNG_SHELLING_GUARDIAN

	vecUnitClassName.push_back( UnitClassName( CX2Unit::UC_ELSWORD_LORD_KNIGHT,			"ELSWORD_LORD_KNIGHT"  ) );
	vecUnitClassName.push_back( UnitClassName( CX2Unit::UC_ELSWORD_RUNE_SLAYER,			"ELSWORD_RUNE_SLAYER"  ) );
	vecUnitClassName.push_back( UnitClassName( CX2Unit::UC_LIRE_WIND_SNEAKER,			"LIRE_WIND_SNEAKER"  	) );
	vecUnitClassName.push_back( UnitClassName( CX2Unit::UC_LIRE_GRAND_ARCHER,			"LIRE_GRAND_ARCHER"  	) );
	vecUnitClassName.push_back( UnitClassName( CX2Unit::UC_ARME_ELEMENTAL_MASTER,		"ARME_ELEMENTAL_MASTER") );
	vecUnitClassName.push_back( UnitClassName( CX2Unit::UC_ARME_VOID_PRINCESS,			"ARME_VOID_PRINCESS"	) );
#ifdef RAVEN_SECOND_CLASS_CHANGE
	vecUnitClassName.push_back( UnitClassName( CX2Unit::UC_RAVEN_BLADE_MASTER,			"RAVEN_BLADE_MASTER"		) );
	vecUnitClassName.push_back( UnitClassName( CX2Unit::UC_RAVEN_RECKLESS_FIST,			"RAVEN_RECKLESS_FIST"		) );
#endif
//{{ kimhc // 2010.1.22 // 이브 2차 전직
#ifdef EVE_SECOND_CLASS_CHANGE
	vecUnitClassName.push_back( UnitClassName( CX2Unit::UC_EVE_CODE_NEMESIS,			"EVE_CODE_NEMESIS"		) );
	vecUnitClassName.push_back( UnitClassName( CX2Unit::UC_EVE_CODE_EMPRESS,			"EVE_CODE_EMPRESS"		) );
#endif EVE_SECOND_CLASS_CHANGE
//}} kimhc // 2010.1.22 // 이브 2차 전직

	//{{ JHKang / 강정훈 / 2011.6.13 / 청 2차 전직
#ifdef CHUNG_SECOND_CLASS_CHANGE
	vecUnitClassName.push_back( UnitClassName( CX2Unit::UC_CHUNG_IRON_PALADIN,			"CHUNG_IRON_PALADIN"		) );
	vecUnitClassName.push_back( UnitClassName( CX2Unit::UC_CHUNG_DEADLY_CHASER,			"CHUNG_DEADLY_CHASER"		) );
#endif

#ifdef SERV_ELSWORD_INFINITY_SWORD
	vecUnitClassName.push_back( UnitClassName( CX2Unit::UC_ELSWORD_INFINITY_SWORD,		"ELSWORD_INFINITY_SWORD"	) );
#endif
#ifdef SERV_ARME_DIMENSION_WITCH
	vecUnitClassName.push_back( UnitClassName( CX2Unit::UC_ARME_DIMENSION_WITCH,		"ARME_DIMENSION_WITCH"	) );
#endif
#ifdef SERV_RENA_NIGHT_WATCHER
	vecUnitClassName.push_back( UnitClassName( CX2Unit::UC_LIRE_NIGHT_WATCHER,			"LIRE_NIGHT_WATCHER"		) );
#endif
#ifdef SERV_RAVEN_VETERAN_COMMANDER
	vecUnitClassName.push_back( UnitClassName( CX2Unit::UC_RAVEN_VETERAN_COMMANDER,		"RAVEN_VETERAN_COMMANDER"	) );
#endif
#ifdef SERV_EVE_BATTLE_SERAPH
	vecUnitClassName.push_back( UnitClassName( CX2Unit::UC_EVE_BATTLE_SERAPH,		"EVE_BATTLE_SERAPH"	) );
#endif
#ifdef SERV_CHUNG_TACTICAL_TROOPER
	vecUnitClassName.push_back( UnitClassName( CX2Unit::UC_CHUNG_TACTICAL_TROOPER,		"CHUNG_TACTICAL_TROOPER"	) );
#endif

#ifdef ARA_CHANGE_CLASS_FIRST
	vecUnitClassName.push_back( UnitClassName( CX2Unit::UC_ARA_LITTLE_HSIEN,			"ARA_LITTLE_HSIEN"	) );
	vecUnitClassName.push_back( UnitClassName( CX2Unit::UC_ARA_SAKRA_DEVANAM,			"ARA_SAKRA_DEVANAM"	) );
#endif //ARA_CHANGE_CLASS_FIRST

#ifdef NEW_CHARACTER_EL
	vecUnitClassName.push_back( UnitClassName( CX2Unit::UC_ELESIS_SABER_KNIGHT,			"ELESIS_SABER_KNIGHT"	) );
	vecUnitClassName.push_back( UnitClassName( CX2Unit::UC_ELESIS_PYRO_KNIGHT,			"ELESIS_PYRO_KNIGHT"	) );

#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE	  // 
	vecUnitClassName.push_back( UnitClassName( CX2Unit::UC_ELESIS_GRAND_MASTER,			"ELESIS_GRAND_MASTER"	) );
	vecUnitClassName.push_back( UnitClassName( CX2Unit::UC_ELESIS_BLAZING_HEART,		"ELESIS_BLAZING_HEART"	) );
#endif // SERV_ELESIS_SECOND_CLASS_CHANGE //


#endif //NEW_CHARACTER_EL

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
	vecUnitClassName.push_back( UnitClassName( CX2Unit::UC_ARA_LITTLE_DEVIL,			"ARA_LITTLE_DEVIL"	) );
	vecUnitClassName.push_back( UnitClassName( CX2Unit::UC_ARA_YAMA_RAJA,				"ARA_YAMA_RAJA"	) );
#endif // SERV_ARA_CHANGE_CLASS_SECOND

#ifdef SERV_9TH_NEW_CHARACTER // 김태환 ( 캐릭터 추가용 )
	vecUnitClassName.push_back( UnitClassName( CX2Unit::UC_ADD_PSYCHIC_TRACER,	"ADD_PSYCHIC_TRACER"	) );
#endif //SERV_9TH_NEW_CHARACTER

#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
	vecUnitClassName.push_back( UnitClassName( CX2Unit::UC_ADD_LUNATIC_PSYKER,	"ADD_LUNATIC_PSYKER"	) );
#endif //SERV_ADD_LUNATIC_PSYKER

	BOOST_TEST_FOREACH( UnitClassName&, unitClassName, vecUnitClassName )
	{
		CX2Unit::UNIT_CLASS eUnitClass = unitClassName.m_eUnitClass;
		if( true == luaManager.BeginTable( unitClassName.m_Name.c_str() ) )
		{
			ComboSet& unitComboSet = m_mapUnitCombo[ eUnitClass ];
			unitComboSet.Init();
			if( true == luaManager.BeginTable( "NORMAL_COMBO" ) )
			{
				ParseComboSet( luaManager, unitComboSet.m_vecNormalCombo );
				luaManager.EndTable(); // NORMAL_COMBO
			}

			if( true == luaManager.BeginTable( "JUMP_COMBO" ) )
			{
				ParseComboSet( luaManager, unitComboSet.m_vecDashCombo );
				luaManager.EndTable(); // JUMP_COMBO
			}

			if( true == luaManager.BeginTable( "DASH_COMBO" ) )
			{
				ParseComboSet( luaManager, unitComboSet.m_vecJumpCombo );	
				luaManager.EndTable(); // DASH_COMBO					   
			}															   

			if( true == luaManager.BeginTable( "DASH_JUMP_COMBO" ) )
			{
				ParseComboSet( luaManager, unitComboSet.m_vecDashJumpCombo );
				luaManager.EndTable(); // DASH_JUMP_COMBO
			}

#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
			if( true == luaManager.BeginTable( "SPECIAL_COMBO" ) )	/// 특수 콤보 트리
			{
				ParseComboSet( luaManager, unitComboSet.m_vecSpecialCombo );
				luaManager.EndTable(); // SPECIAL_COMBO
			}
#endif //SERV_ADD_LUNATIC_PSYKER

			luaManager.EndTable(); // tableName
		}
	}

	return true;
}



#endif COMBO_TREE_UI_TEST

