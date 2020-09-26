#include "StdAfx.h"
#include ".\x2lvupeventmgr.h"

CX2LVUpEventMgr::CX2LVUpEventMgr(void)
{
	m_pNowStage = NULL;
	if ( OpenScriptFile( L"LevelUpEvent.lua" ) == false )
	{
		ErrorLog( KEM_ERROR353 );
	}
}

CX2LVUpEventMgr::~CX2LVUpEventMgr(void)
{
	map< LEVEL_UP_EVENT_ID, LVUpEvent* >::iterator mit;
	for ( mit = m_mapLVUpEvent.begin(); mit != m_mapLVUpEvent.end(); mit++ )
	{
		CX2LVUpEventMgr::LVUpEvent* pLVUpEvent = mit->second;
		SAFE_DELETE( pLVUpEvent );
	}
	m_mapLVUpEvent.clear();
}

void CX2LVUpEventMgr::SetNowStage( CKTDXStage* pStage )
{
	m_pNowStage = pStage;

	map< LEVEL_UP_EVENT_ID, LVUpEvent* >::iterator mit;
	for ( mit = m_mapLVUpEvent.begin(); mit != m_mapLVUpEvent.end(); mit++ )
	{
		CX2LVUpEventMgr::LVUpEvent* pLVUpEvent = mit->second;
		if ( pLVUpEvent == NULL )
			continue;

		pLVUpEvent->SetNowStage( pStage );
	}

}

void CX2LVUpEventMgr::Reset( int nowLevel )
{
	map< LEVEL_UP_EVENT_ID, LVUpEvent* >::iterator mit;
	for ( mit = m_mapLVUpEvent.begin(); mit != m_mapLVUpEvent.end(); mit++ )
	{
		CX2LVUpEventMgr::LVUpEvent* pLVUpEvent = mit->second;
		if ( pLVUpEvent == NULL )
			continue;

		pLVUpEvent->Reset( nowLevel );
	}
}

void CX2LVUpEventMgr::OnFrameMove( double fTime, float fElapsedTime )
{
	map< LEVEL_UP_EVENT_ID, LVUpEvent* >::iterator mit;
	for ( mit = m_mapLVUpEvent.begin(); mit != m_mapLVUpEvent.end(); mit++ )
	{
		CX2LVUpEventMgr::LVUpEvent* pLVUpEvent = mit->second;
		if ( pLVUpEvent == NULL )
			continue;

		pLVUpEvent->OnFrameMove( fTime, fElapsedTime );
	}
}

bool CX2LVUpEventMgr::OpenScriptFile( const WCHAR* pFileName )
{
	lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(),  "g_pLVUpEventMgr", this );
	return g_pKTDXApp->LoadLuaTinker( pFileName );
}

void CX2LVUpEventMgr::AddLevelUpEvent_LUA()
{
//
//#ifdef LUA_TEST
//
//	CX2LVUpEventMgr::LVUpEvent* pLVUpEvent = new CX2LVUpEventMgr::LVUpEvent();
//
//	lua_tinker::table luaTable( g_pKTDXApp->GetLuaBinder()->GetLuaState(), -1 );
//	lua_tinker::table luaTable2;
//	lua_tinker::table luaTable3;
//	lua_tinker::table luaTable4;
//
//
//
//	luaTable.get_<CX2LVUpEventMgr::LEVEL_UP_EVENT_ID>( "EVENT_ID", pLVUpEvent->m_EventID, LUEI_NONE );
//
//	map< LEVEL_UP_EVENT_ID, LVUpEvent* >::iterator mit;
//	mit = m_mapLVUpEvent.find( pLVUpEvent->m_EventID );
//	if ( mit != m_mapLVUpEvent.end() )
//	{
//		wstringstream wstrstm;
//		wstrstm << pLVUpEvent->m_EventID << L" : LevelUpEventID 중복됨 ";
//		ErrorLogMsg( KEM_ERROR352, wstrstm.str().c_str() );
//#ifndef _SERVICE_
//		MessageBox( g_pKTDXApp->GetHWND(), wstrstm.str().c_str(), L"레벨업 이벤트 매니져 오류", MB_OK );
//#endif
//		SAFE_DELETE( pLVUpEvent );
//		return;
//	}
//
//
//	int typeIndex = 0;
//	while( true == LUA_GET_SUFFIX_NODEFAULT_( luaTable, "TYPE", typeIndex, luaTable2 ) )
//	//while( true == LuaTableGet_<lua_tinker::table>( luaTable, "TYPE", typeIndex, luaTable2 ) )
//	{
//		typeIndex++;
//		CX2LVUpEventMgr::LVUpEventCondNReact* pLVUpEventCondNReact = new CX2LVUpEventMgr::LVUpEventCondNReact();
//
//		if( true == luaTable2.get_<lua_tinker::table>( "COND", luaTable3 ) )
//		{
//			luaTable3.get_( "LEVEL", pLVUpEventCondNReact->m_Level, 9999 );
//
//			if( true == luaTable3.get_<lua_tinker::table>( "UNIT_TYPE", luaTable4 ) )
//			{
//				int tableIndex = 1;
//				int unitTypeID = 0;
//				while( true == luaTable4.get_<int>( tableIndex, unitTypeID ) )
//				{
//					tableIndex++;
//					CX2Unit::UNIT_TYPE unitType = (CX2Unit::UNIT_TYPE)unitTypeID;
//					pLVUpEventCondNReact->m_vecCondUnitType.push_back( unitType );
//				}
//			}
//
//			if( true == luaTable3.get_<lua_tinker::table>( "UNIT_CLASS", luaTable4 ) )
//			{
//				int tableIndex = 1;
//				int unitClassID = 0;
//				while( true == luaTable4.get_<int>( tableIndex, unitClassID ) )
//				{
//					tableIndex++;
//					CX2Unit::UNIT_CLASS unitClass = (CX2Unit::UNIT_CLASS)unitClassID;
//					pLVUpEventCondNReact->m_vecCondUnitClass.push_back( unitClass);
//				}
//
//			}
//
//		}
//
//
//		if( true == luaTable2.get_<lua_tinker::table>( "REACT", luaTable3 ) )
//		{
//			if( true == luaTable3.get_<lua_tinker::table>( "DIALOG_LIST", luaTable4 ) )
//			{
//				int tableIndex = 1;
//				wstring fileName;
//				string strFileName;
//				
//				while( true == luaTable4.get_<string>( tableIndex, strFileName ) )
//				{
//					tableIndex++;
//					ConvertCharToWCHAR( fileName, strFileName.c_str() );
//					
//					pLVUpEventCondNReact->m_vecReactDialogFile.push_back( fileName );
//				}
//			}
//		}
//
//		pLVUpEvent->m_vecCondNReact.push_back( pLVUpEventCondNReact );
//	}
//
//
//#else


	CX2LVUpEventMgr::LVUpEvent* pLVUpEvent = new CX2LVUpEventMgr::LVUpEvent();

	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
#ifndef X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
	TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING

	LUA_GET_VALUE_ENUM(	luaManager, "EVENT_ID",	pLVUpEvent->m_EventID, CX2LVUpEventMgr::LEVEL_UP_EVENT_ID, LUEI_NONE );
	map< LEVEL_UP_EVENT_ID, LVUpEvent* >::iterator mit;
	mit = m_mapLVUpEvent.find( pLVUpEvent->m_EventID );
	if ( mit != m_mapLVUpEvent.end() )
	{
		wstringstream wstrstm;
		wstrstm << pLVUpEvent->m_EventID << L" " << GET_STRING( STR_ID_344 ) << L" ";
		ErrorLogMsg( KEM_ERROR352, wstrstm.str().c_str() );
#ifndef _SERVICE_
		MessageBox( g_pKTDXApp->GetHWND(), wstrstm.str().c_str(), L"레벨업 이벤트 매니져 오류", MB_OK );
#endif
		SAFE_DELETE( pLVUpEvent );
		return;
	}

	
	int typeIndex = 0;

	
	while( luaManager.BeginTable( "TYPE", typeIndex ) == true )
	{
		typeIndex++;
		CX2LVUpEventMgr::LVUpEventCondNReact* pLVUpEventCondNReact = new CX2LVUpEventMgr::LVUpEventCondNReact();

		if( luaManager.BeginTable( "COND" ) == true )
		{
			LUA_GET_VALUE(	luaManager, "LEVEL",	pLVUpEventCondNReact->m_Level, 9999 );

			if( luaManager.BeginTable( "UNIT_TYPE" ) == true )
			{
				int tableIndex = 1;
				int unitTypeID = 0;
				while( luaManager.GetValue( tableIndex, unitTypeID ) == true )
				{
					tableIndex++;
					CX2Unit::UNIT_TYPE unitType = (CX2Unit::UNIT_TYPE)unitTypeID;
					pLVUpEventCondNReact->m_vecCondUnitType.push_back( unitType );
				}

				luaManager.EndTable();
			}

			if( luaManager.BeginTable( "UNIT_CLASS" ) == true )
			{
				int tableIndex = 1;
				int unitClassID = 0;
				while( luaManager.GetValue( tableIndex, unitClassID ) == true )
				{
					tableIndex++;
					CX2Unit::UNIT_CLASS unitClass = (CX2Unit::UNIT_CLASS)unitClassID;
					pLVUpEventCondNReact->m_vecCondUnitClass.push_back( unitClass);
				}

				luaManager.EndTable();
			}

			luaManager.EndTable();
		}

		if( luaManager.BeginTable( "REACT" ) == true )
		{
			if( luaManager.BeginTable( "DIALOG_LIST" ) == true )
			{
				int tableIndex = 1;
				wstring fileName;
				while( luaManager.GetValue( tableIndex, fileName ) == true )
				{
					tableIndex++;
					pLVUpEventCondNReact->m_vecReactDialogFile.push_back( fileName );
				}

				luaManager.EndTable();
			}

#ifdef ADD_PLAY_SOUND //김창한
			if( luaManager.BeginTable( "SOUND_LIST" ) == true )
			{
				int tableIndex = 1;
				wstring fileName;
				while( luaManager.GetValue( tableIndex, fileName ) == true )
				{
					tableIndex++;
					pLVUpEventCondNReact->m_vecSoundFileName.push_back( fileName );
				}

				luaManager.EndTable();
			}
#endif //ADD_PLAY_SOUND

			luaManager.EndTable();
		}

		pLVUpEvent->m_vecCondNReact.push_back( pLVUpEventCondNReact );

		luaManager.EndTable();
	}


//#endif LUA_TEST_TEST

	m_mapLVUpEvent.insert( std::make_pair( pLVUpEvent->m_EventID, pLVUpEvent ) );
}

bool CX2LVUpEventMgr::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( wParam )
	{
	case LUEMCUM_PVP_PR:
		{
			CX2LVUpEventMgr::LVUpEvent* pLVUpEvent = GetLVUpEvent( CX2LVUpEventMgr::LUEI_PVP_PR );
			if ( pLVUpEvent != NULL )
				pLVUpEvent->DeleteDialog();
				
		}
		break;

	case LUEMCUM_CHANGE_CLASS_GUIDE_FIRST:
		{
			CX2LVUpEventMgr::LVUpEvent* pLVUpEvent = GetLVUpEvent( CX2LVUpEventMgr::LUEI_CHANGE_CLASS_GUIDE_FIRST );
			if ( pLVUpEvent != NULL )
				pLVUpEvent->DeleteDialog();
		}
		break;
	case LUEMCUM_10LV_PR:
		{
			CX2LVUpEventMgr::LVUpEvent* pLVUpEvent = GetLVUpEvent( CX2LVUpEventMgr::LUEI_LEVEL_10 );
			if ( pLVUpEvent != NULL )
				pLVUpEvent->DeleteDialog();
		}
		break;
#ifdef LINK_CASH_SHOP_WHEN_JOB_CHANGE
	case LUEMCUM_ELSWORD_SWORD_KNIGHT:
	case LUEMCUM_ELSWORD_MAGIC_KNIGHT:
	case LUEMCUM_AISHA_HIGH_MAGICIAN:
	case LUEMCUM_AISHA_DARK_MAGICIAN:
	case LUEMCUM_RENA_SNIPING_RANGER:
	case LUEMCUM_RENA_COMBAT_RANGER:
	case LUEMCUM_RAVEN_SWORD_TAKER:
	case LUEMCUM_RAVEN_OVER_TAKER:
	case LUEMCUM_EVE_CODE_EXOTIC:
	case LUEMCUM_EVE_CODE_ARCHITECTURE:
	case LUEMCUM_CHUNG_FURY_GUARDIAN:
	case LUEMCUM_CHUNG_SHOOTING_GUARDIAN:
		//	1차 추가 전직
	case LUEMCUM_ELSWORD_SHEATH_KNIGHT:
	case LUEMCUM_ARME_BATTLE_MAGICIAN:
	case LUEMCUM_LIRE_TRAPPING_RANGER:
	case LUEMCUM_RAVEN_WEAPON_TAKER:
	case LUEMCUM_EVE_ELECTRA:
	case LUEMCUM_CHUNG_SHELLING_GUARDIAN:
	case LUEMCUM_ARA_LITTLE_HSIEN:
#ifdef SERV_ARA_CHANGE_CLASS_SECOND
	case LUEMCUM_ARA_LITTLE_DEVIL:
#endif SERV_ARA_CHANGE_CLASS_SECOND
#ifdef NEW_CHARACTER_EL
	case LUEMCUM_ELESIS_SABER_KNIGHT:
	case LUEMCUM_ELESIS_PYRO_KNIGHT:
#endif NEW_CHARACTER_EL
		{
			CX2LVUpEventMgr::LVUpEvent* pLVUpEvent = GetLVUpEvent( CX2LVUpEventMgr::LUEI_CHANGE_CLASS_GUIDE_FIRST );
			if ( pLVUpEvent != NULL )
				pLVUpEvent->DeleteDialog();

			CX2StateField*	pStateField	=	NULL;
			pStateField		=  static_cast< CX2StateField* >( g_pMain->GetNowState() );

			if( pStateField != NULL )
			{
				if( g_pTFieldGame != NULL )
				{
					g_pTFieldGame->CloseFieldName( 0.0f );
				}

				g_pData->GetCashShop()->SetMenuTypeCallingCashShop( (int)wParam );
				pStateField->ToggleCashShop();
			}
		} break;
#endif LINK_CASH_SHOP_WHEN_JOB_CHANGE
	}
	return false;
}

CX2LVUpEventMgr::LVUpEvent* CX2LVUpEventMgr::GetLVUpEvent( LEVEL_UP_EVENT_ID eventID )
{
	CX2LVUpEventMgr::LVUpEvent* pLVUpEvent = NULL;
	map< LEVEL_UP_EVENT_ID, LVUpEvent* >::iterator mit;
	mit = m_mapLVUpEvent.find( eventID );
	if ( mit != m_mapLVUpEvent.end() )
		pLVUpEvent = mit->second;

	return pLVUpEvent;
}

CX2LVUpEventMgr::LVUpEvent::LVUpEvent()
{
	m_LastCheckLevel = 0;
	m_EventID = LUEI_NONE;
	m_bCheckShow = false;
}
CX2LVUpEventMgr::LVUpEvent::~LVUpEvent()
{
	DeleteDialog();

	for ( int i = 0; i < (int)m_vecCondNReact.size(); i++ )
	{
		LVUpEventCondNReact* pLVUpEventCondNReact = m_vecCondNReact[i];
		SAFE_DELETE( pLVUpEventCondNReact );
	}
	m_vecCondNReact.clear();
}

void CX2LVUpEventMgr::LVUpEvent::Reset( int nowLevel )
{
	m_LastCheckLevel = nowLevel;
	m_bCheckShow = false;
}

void CX2LVUpEventMgr::LVUpEvent::SetNowStage( CKTDXStage* pStage )
{
	for ( int i = 0; i < (int)m_vecDialog.size(); i++ )
	{
		CKTDGUIDialogType pDialog = m_vecDialog[i];
		if ( pDialog == NULL )
			continue;
		
		pDialog->SetStage( pStage );
	}
}

void CX2LVUpEventMgr::LVUpEvent::OnFrameMove( double fTime, float fElapsedTime )
{
	//이제 여기서 검사해서 다이얼로그 자동으로 띄워져부럴까~~~?
	CheckLevel();
}

void CX2LVUpEventMgr::LVUpEvent::DeleteDialog()
{
	for ( int i = 0; i < (int)m_vecDialog.size(); i++ )
	{
		CKTDGUIDialogType pDialog = m_vecDialog[i];
		g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pDialog, NULL, false );
	}
	m_vecDialog.clear();
}

void CX2LVUpEventMgr::LVUpEvent::CheckLevel()
{
	if ( m_bCheckShow == true )
		return;

	if ( g_pData != NULL && g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetSelectUnit() != NULL )
	{
		CX2Unit* pUnit = g_pData->GetMyUser()->GetSelectUnit();

		for ( int i = 0; i < (int)m_vecCondNReact.size(); i++ )
		{
			CX2LVUpEventMgr::LVUpEventCondNReact* pLVUpEventCondNReact = m_vecCondNReact[i];
			if ( pLVUpEventCondNReact == NULL )
				continue;

			if ( pLVUpEventCondNReact->m_Level > m_LastCheckLevel &&
				pLVUpEventCondNReact->m_Level <= pUnit->GetUnitData().m_Level )
			{
				m_bCheckShow = true;

				bool bCheck = false;

				for ( int i = 0; i < (int)pLVUpEventCondNReact->m_vecCondUnitType.size(); i++ )
				{
					CX2Unit::UNIT_TYPE unitType = pLVUpEventCondNReact->m_vecCondUnitType[i];
					if ( unitType == CX2Unit::UT_NONE )
					{
						bCheck = true;
						break;
					}

					if ( pUnit->GetUnitTemplet() != NULL &&
						pUnit->GetUnitTemplet()->m_UnitType == unitType )
					{
						bCheck = true;
						break;
					}
				}

				if ( bCheck == false )
				{
					for ( int i = 0; i < (int)pLVUpEventCondNReact->m_vecCondUnitClass.size(); i++ )
					{
						CX2Unit::UNIT_CLASS unitClass = pLVUpEventCondNReact->m_vecCondUnitClass[i];
						if ( unitClass == CX2Unit::UC_NONE )
						{
							bCheck = true;
							break;
						}

						if ( pUnit->GetUnitTemplet() != NULL &&
							pUnit->GetUnitTemplet()->m_UnitClass == unitClass )
						{
							bCheck = true;
							break;
						}
					}
				}
				
				

				if ( bCheck == true )
				{
					DeleteDialog();

					for ( int i = 0; i < (int)pLVUpEventCondNReact->m_vecReactDialogFile.size(); i++ )
					{
						wstring dialogFileName = pLVUpEventCondNReact->m_vecReactDialogFile[i];
						CKTDGUIDialogType pDialog = new CKTDGUIDialog( g_pMain->GetNowState(), dialogFileName.c_str() );
						g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( pDialog );

						m_vecDialog.push_back( pDialog );
					}

#ifdef ADD_PLAY_SOUND //김창한
					if( !pLVUpEventCondNReact->m_vecSoundFileName.empty() && NULL != g_pKTDXApp->GetDeviceManager() )
					{
						int iSize = pLVUpEventCondNReact->m_vecSoundFileName.size();
						int iRandom = RandomInt() % iSize;

						g_pKTDXApp->GetDeviceManager()->PlaySound(pLVUpEventCondNReact->m_vecSoundFileName[iRandom].c_str(), false, false );
					}
#endif //ADD_PLAY_SOUND

					break;
				}
			}
		}

		
		m_LastCheckLevel = g_pData->GetSelectUnitLevel();
	}
}