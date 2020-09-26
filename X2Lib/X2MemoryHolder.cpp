#include "StdAfx.h"
#include ".\x2memoryholder.h"

CX2MemoryHolder::CX2MemoryHolder(void)
{
	m_VillageID = -1;

	m_pDungeonRoomBackDLG	= NULL;
	m_pDungeonRoomFrontDLG	= NULL;

	m_pPVPRoomTeamBackDLG		= NULL;
	m_pPVPRoomSurvivorBackDLG	= NULL;
	m_pPVPRoomFrontDLG			= NULL;

	m_pPVPLobbyBackDLG	= NULL;
	m_pPVPLobbyFrontDLG = NULL;

	m_pVillageMapBackDLG	= NULL;
	m_pVillageMapFrontDLG	= NULL;
	m_bIsParsedVillageMapDLG = false;

	m_pLocalMapBackDLG		= NULL;
	m_pLocalMapFrontDLG		= NULL;
	m_bIsParsedLocalMapDLG	= false;

#ifdef XMAS_UI
	m_pXMasDLG = NULL;
#endif

#ifdef SERV_CHINA_SPIRIT_EVENT
	m_pUseSpiritEventDLG = NULL;
	m_bShow = false;
#endif SERV_CHINA_SPIRIT_EVENT

#ifdef SERV_EVENT_COBO_DUNGEON_AND_FIELD
	m_pUseCoboEventDLG = NULL;
	m_pCoboEventCountDLG = NULL;
#endif SERV_EVENT_COBO_DUNGEON_AND_FIELD

#ifdef SERV_NEW_YEAR_EVENT_2014
	m_pNewYear2014EventDLG = NULL;
	m_bShowNewYear2014Event = false;
	m_bShowNewYear2014EventBoard = false;
#endif SERV_NEW_YEAR_EVENT_2014

#ifdef SERV_EVENT_CHECK_POWER
	m_pCheckPowerEventDLG = NULL;
	m_bShowCheckPowerEvent = false;
	m_bShowCheckPowerGuide = false;
#endif SERV_EVENT_CHECK_POWER

	m_pMenuDLG = NULL;
	m_pCommonDLG = NULL;

	m_pDLGOptionWindow = NULL;

#ifdef SERV_EVENT_CHUNG_GIVE_ITEM
	m_pChungGiveItem = NULL;
#endif SERV_EVENT_CHUNG_GIVE_ITEM

#ifdef ALWAYS_EVENT_ADAMS_UI_SHOP
	m_pUseAdamsEventShopDLG = NULL;
#endif ALWAYS_EVENT_ADAMS_UI_SHOP

#ifdef SERV_4TH_ANNIVERSARY_EVENT
	m_p4thYearEventDLG = NULL;
	m_bShow4thYearEvent = false;
#endif //SERV_4TH_ANNIVERSARY_EVENT
}

CX2MemoryHolder::~CX2MemoryHolder(void)
{
	ReleaseVillageMemory();

	SAFE_DELETE_DIALOG( m_pDungeonRoomBackDLG );
	SAFE_DELETE_DIALOG( m_pDungeonRoomFrontDLG );

	SAFE_DELETE_DIALOG( m_pPVPRoomTeamBackDLG );
	SAFE_DELETE_DIALOG( m_pPVPRoomSurvivorBackDLG );
	SAFE_DELETE_DIALOG( m_pPVPRoomFrontDLG );

	SAFE_DELETE_DIALOG( m_pPVPLobbyBackDLG );
	SAFE_DELETE_DIALOG( m_pPVPLobbyFrontDLG );

	SAFE_DELETE_DIALOG( m_pVillageMapBackDLG );
	SAFE_DELETE_DIALOG( m_pVillageMapFrontDLG );

	SAFE_DELETE_DIALOG( m_pLocalMapBackDLG );
	SAFE_DELETE_DIALOG( m_pLocalMapFrontDLG );

	SAFE_DELETE_DIALOG( m_pCommonDLG );
	SAFE_DELETE_DIALOG( m_pMenuDLG );

	SAFE_DELETE_DIALOG( m_pDLGOptionWindow );

#ifdef XMAS_UI
	SAFE_DELETE_DIALOG( m_pXMasDLG );
#endif

#ifdef SERV_EVENT_CHUNG_GIVE_ITEM
	if( m_pChungGiveItem != NULL )
	{
		SAFE_DELETE_DIALOG( m_pChungGiveItem );
	}
#endif SERV_EVENT_CHUNG_GIVE_ITEM

#ifdef SERV_EVENT_COBO_DUNGEON_AND_FIELD
	SAFE_DELETE_DIALOG( m_pUseCoboEventDLG );
	SAFE_DELETE_DIALOG( m_pCoboEventCountDLG );
#endif SERV_EVENT_COBO_DUNGEON_AND_FIELD

#ifdef ALWAYS_EVENT_ADAMS_UI_SHOP
	SAFE_DELETE_DIALOG( m_pUseAdamsEventShopDLG );
#endif ALWAYS_EVENT_ADAMS_UI_SHOP
	ClearEquip();
}


CKTDGUIDialogType CX2MemoryHolder::GetOptionDLG( CKTDXStage* pStage )
{
	if ( m_pDLGOptionWindow == NULL )
	{

#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh 진입 구조 개편
#ifdef CLIENT_COUNTRY_TW
		m_pDLGOptionWindow = new CKTDGUIDialog( pStage, L"DLG_UI_Option_Back_New_TW.lua", 0.07f, XDL_OPTION );
#else
		m_pDLGOptionWindow = new CKTDGUIDialog( pStage, L"DLG_UI_Option_Back_New.lua", 0.07f, XDL_OPTION );	
#endif CLIENT_COUNTRY_TW
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh 진입 구조 개편
#ifdef CLIENT_COUNTRY_TW
		m_pDLGOptionWindow = new CKTDGUIDialog( pStage, L"DLG_UI_Option_Back_TW.lua", 0.07f, XDL_OPTION );	
#else //CLIENT_COUNTRY_TW
		m_pDLGOptionWindow = new CKTDGUIDialog( pStage, L"DLG_UI_Option_Back.lua", 0.07f, XDL_OPTION );	
#endif //CLIENT_COUNTRY_TW
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh 진입 구조 개편
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGOptionWindow );
		m_pDLGOptionWindow->SetModal( true );

		DXUTDeviceSettings deviceSettings = DXUTGetDeviceSettings();

		CD3DEnumeration* pD3DEnum = DXUTGetEnumeration();
		CD3DEnumAdapterInfo* pAdapterInfo = pD3DEnum->GetAdapterInfo( deviceSettings.AdapterOrdinal ); // GetCurrentAdapterInfo

		// 해상도 콤보 박스 초기화
		if( pAdapterInfo != NULL )
		{
			CKTDGUIComboBox* pCombo_Graphic_Resoultion = (CKTDGUIComboBox*) m_pDLGOptionWindow->GetControl( L"Option_Resolution" );
			pCombo_Graphic_Resoultion->RemoveAllItems();

			for( int idm = 0; idm < pAdapterInfo->displayModeList.GetSize(); idm++ )
			{
				D3DDISPLAYMODE displayMode = pAdapterInfo->displayModeList.GetAt( idm );
				DWORD dwWidth = displayMode.Width;
				DWORD dwHeight = displayMode.Height;

				if( displayMode.Format == deviceSettings.AdapterFormat )
				{
					DWORD dwResolutionData;
					WCHAR strResolution[50];
					dwResolutionData = MAKELONG( dwWidth, dwHeight );
					StringCchPrintf( strResolution, 50, L"%d x %d", dwWidth, dwHeight );

					if( false == pCombo_Graphic_Resoultion->ContainsItem( strResolution ) )
						pCombo_Graphic_Resoultion->AddItem( strResolution, ULongToPtr( dwResolutionData ) );
				}		
			}

			pCombo_Graphic_Resoultion->SetDropHeight( 10 * pCombo_Graphic_Resoultion->GetNumItems() );


			CX2GameOption::OptionList* pOptionList = &g_pMain->GetGameOption().GetOptionList();
			if( NULL != pOptionList )
			{
				WCHAR strResolution[50] = { 0, };
				StringCchPrintf( strResolution, ARRAY_SIZE(strResolution), L"%d x %d", (int)pOptionList->m_vResolution.x, (int)pOptionList->m_vResolution.y );
				pCombo_Graphic_Resoultion->SetSelectedByText( strResolution );
			}

		}	
	}


	DefualtSettingDLG( m_pDLGOptionWindow, pStage );


	return m_pDLGOptionWindow;
}


CKTDGUIDialogType CX2MemoryHolder::GetDungeonRoomBackDLG( CKTDXStage* pStage )
{
	if ( m_pDungeonRoomBackDLG == NULL )
	{
		m_pDungeonRoomBackDLG = new CKTDGUIDialog( pStage, L"DLG_Dungeon_Room_Back.lua" );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDungeonRoomBackDLG );
	}

	DefualtSettingDLG( m_pDungeonRoomBackDLG, pStage );

	return m_pDungeonRoomBackDLG;
}

CKTDGUIDialogType CX2MemoryHolder::GetDungeonRoomFrontDLG( CKTDXStage* pStage )
{
	if ( m_pDungeonRoomFrontDLG == NULL )
	{
		m_pDungeonRoomFrontDLG = new CKTDGUIDialog( pStage, L"DLG_Dungeon_Room_Front.lua" );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDungeonRoomFrontDLG );
	}

	DefualtSettingDLG( m_pDungeonRoomFrontDLG, pStage );

	return m_pDungeonRoomFrontDLG;

}

CKTDGUIDialogType CX2MemoryHolder::GetPVPRoomTeamBackDLG( CKTDXStage* pStage )
{
	if ( m_pPVPRoomTeamBackDLG == NULL )
	{
#ifdef DUNGEON_ITEM
		m_pPVPRoomTeamBackDLG = new CKTDGUIDialog( pStage, L"DLG_PVP_Room_Team_Back_Itemmode.lua" );
#else
		m_pPVPRoomTeamBackDLG = new CKTDGUIDialog( pStage, L"DLG_PVP_Room_Team_Back.lua" );
#endif
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pPVPRoomTeamBackDLG );
	}

	DefualtSettingDLG( m_pPVPRoomTeamBackDLG, pStage );

	return m_pPVPRoomTeamBackDLG;

}

CKTDGUIDialogType CX2MemoryHolder::GetPVPRoomSurvivorBackDLG( CKTDXStage* pStage )
{
	if ( m_pPVPRoomSurvivorBackDLG == NULL )
	{
#ifdef DUNGEON_ITEM
		m_pPVPRoomSurvivorBackDLG = new CKTDGUIDialog( pStage, L"DLG_PVP_Room_Death_Back_Itemmode.lua" );
#else
		m_pPVPRoomSurvivorBackDLG = new CKTDGUIDialog( pStage, L"DLG_PVP_Room_Death_Back.lua" );
#endif
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pPVPRoomSurvivorBackDLG );
	}

	DefualtSettingDLG( m_pPVPRoomSurvivorBackDLG, pStage );

	return m_pPVPRoomSurvivorBackDLG;

}

CKTDGUIDialogType CX2MemoryHolder::GetPVPRoomFrontDLG( CKTDXStage* pStage )
{
	if ( m_pPVPRoomFrontDLG == NULL )
	{
#ifdef DUNGEON_ITEM
		m_pPVPRoomFrontDLG = new CKTDGUIDialog( pStage, L"DLG_PVP_Room_Front_ItemMode.lua" );
#else
		m_pPVPRoomFrontDLG = new CKTDGUIDialog( pStage, L"DLG_PVP_Room_Front.lua" );
#endif
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pPVPRoomFrontDLG );
	}

	DefualtSettingDLG( m_pPVPRoomFrontDLG, pStage );

	return m_pPVPRoomFrontDLG;
}


CKTDGUIDialogType CX2MemoryHolder::GetPVPLobbyBack( CKTDXStage* pStage )
{
	if ( m_pPVPLobbyBackDLG == NULL )
	{
// 2008.12.2 김태완 : 새 UI 대련장용 배경그림 적용
#ifdef NEW_VILLAGE_UI
		m_pPVPLobbyBackDLG = new CKTDGUIDialog( pStage, L"DLG_UI_PvP_Lobby_BG.lua" );
#else
		m_pPVPLobbyBackDLG = new CKTDGUIDialog( pStage, L"DLG_PVP_Lobby_Back.lua" );
#endif
		
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pPVPLobbyBackDLG );
	}

	DefualtSettingDLG( m_pPVPLobbyBackDLG, pStage );

	return m_pPVPLobbyBackDLG;
}

CKTDGUIDialogType CX2MemoryHolder::GetPVPLobbyFront( CKTDXStage* pStage )
{
	if ( m_pPVPLobbyFrontDLG == NULL )
	{
#ifdef NEW_VILLAGE_UI
	#ifdef DUNGEON_ITEM
		#ifdef SERV_PVP_NEW_SYSTEM
				m_pPVPLobbyFrontDLG = new CKTDGUIDialog( pStage, L"DLG_UI_Emulate_ItemMode_Ver1.lua" );
		#else //SERV_PVP_NEW_SYSTEM
				m_pPVPLobbyFrontDLG = new CKTDGUIDialog( pStage, L"DLG_UI_Emulate_ItemMode.lua" );
		#endif //SERV_PVP_NEW_SYSTEM
	#else
			m_pPVPLobbyFrontDLG = new CKTDGUIDialog( pStage, L"DLG_UI_Emulate.lua" );
	#endif
#else
		m_pPVPLobbyFrontDLG = new CKTDGUIDialog( pStage, L"DLG_PVP_Lobby_Front.lua" );
#endif
		m_pPVPLobbyFrontDLG->SetFront(true);
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pPVPLobbyFrontDLG );
	}

	DefualtSettingDLG( m_pPVPLobbyFrontDLG, pStage );

	return m_pPVPLobbyFrontDLG;
}


CKTDGUIDialogType CX2MemoryHolder::GetVillageMapBackDLG( CKTDXStage* pStage )
{
	if ( m_pVillageMapBackDLG == NULL )
	{
		m_pVillageMapBackDLG = new CKTDGUIDialog( pStage, L"DLG_Map_Village_Back.lua" );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pVillageMapBackDLG );
	}

	DefualtSettingDLG( m_pVillageMapBackDLG, pStage );

	return m_pVillageMapBackDLG;
}

CKTDGUIDialogType CX2MemoryHolder::GetVillageMapFrontDLG( CKTDXStage* pStage )
{
	if ( m_pVillageMapFrontDLG == NULL )
	{
		m_pVillageMapFrontDLG = new CKTDGUIDialog( pStage, L"DLG_Map_Village_Front.lua" );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pVillageMapFrontDLG );
	}

	DefualtSettingDLG( m_pVillageMapFrontDLG, pStage );

	return m_pVillageMapFrontDLG;
}

void CX2MemoryHolder::ReleaseVillageMapDLG()
{
	SAFE_DELETE_DIALOG( m_pVillageMapBackDLG );
	SAFE_DELETE_DIALOG( m_pVillageMapFrontDLG );

	SetIsParsedVillageMapDLG( false );
}


CKTDGUIDialogType CX2MemoryHolder::GetLocalMapBackDLG( CKTDXStage* pStage )
{
	if ( m_pLocalMapBackDLG == NULL )
	{
		m_pLocalMapBackDLG = new CKTDGUIDialog( pStage, L"DLG_Map_Local_Back.lua" );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pLocalMapBackDLG );
	}

	DefualtSettingDLG( m_pLocalMapBackDLG, pStage );

	return m_pLocalMapBackDLG;
}

CKTDGUIDialogType CX2MemoryHolder::GetLocalMapFrontDLG( CKTDXStage* pStage )
{
	if ( m_pLocalMapFrontDLG == NULL )
	{
		m_pLocalMapFrontDLG = new CKTDGUIDialog( pStage, L"DLG_Map_Local_Front.lua" );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pLocalMapFrontDLG );
	}

	DefualtSettingDLG( m_pLocalMapFrontDLG, pStage );

	return m_pLocalMapFrontDLG;
}

void CX2MemoryHolder::ReleaseLocalMapDLG()
{

	SAFE_DELETE_DIALOG( m_pLocalMapBackDLG );
	SAFE_DELETE_DIALOG( m_pLocalMapFrontDLG );

	SetIsParsedLocalMapDLG( false );
}

CKTDGUIDialogType CX2MemoryHolder::GetMenuDLG( CKTDXStage* pStage )
{
	if ( m_pMenuDLG == NULL )
	{
#ifdef POSTBOX

	#ifdef NEW_VILLAGE_UI
		#ifdef SERV_PET_SYSTEM
			#ifdef SERV_PVP_NEW_SYSTEM
				#ifdef SERV_RECOMMEND_LIST_EVENT	// 해외팀은 번호 변경
						m_pMenuDLG = new CKTDGUIDialog( pStage, L"DLG_Menu_Button_NEW2.lua" ); // 친구 추천 이벤트 때 사용한 스크립트
				#else
						m_pMenuDLG = new CKTDGUIDialog( pStage, L"DLG_Menu_Button_NEW.lua" ); //현재 사용 하고 있는 스크립트 
				#endif //SERV_RECOMMEND_LIST_EVENT
			#else
				m_pMenuDLG = new CKTDGUIDialog( pStage, L"DLG_Menu_Button_Pet.lua" );
			#endif
		#else
	        m_pMenuDLG = new CKTDGUIDialog( pStage, L"DLG_Menu_Button.lua" );
		#endif
	#else
	        m_pMenuDLG = new CKTDGUIDialog( pStage, L"DLG_Menu_Button_New1.lua" );
	#endif NEW_VILLAGE_UI

#else
			m_pMenuDLG = new CKTDGUIDialog( pStage, L"DLG_Menu_Button.lua" );

#endif POSTBOX
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pMenuDLG );
	}

	DefualtSettingDLG( m_pMenuDLG, pStage );

	return m_pMenuDLG;
}

#ifdef XMAS_UI
CKTDGUIDialogType CX2MemoryHolder::GetXMasDLG( CKTDXStage* pStage )
{
	if ( m_pXMasDLG == NULL )
	{
		m_pXMasDLG = new CKTDGUIDialog( pStage, L"DLG_UI_CHRISTMAS.lua" );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pXMasDLG );
	}

	DefualtSettingDLG( m_pXMasDLG, pStage );
	return m_pXMasDLG;
}
#endif

#ifdef SERV_EVENT_CHUNG_GIVE_ITEM
CKTDGUIDialogType CX2MemoryHolder::GetChungGiveItemDLG( CKTDXStage* pStage )
{
	if( m_pChungGiveItem == NULL )
	{
		m_pChungGiveItem = new CKTDGUIDialog(pStage,L"DLG_UI_Chung_Give_Item.lua");
		if( m_pChungGiveItem != NULL)
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pChungGiveItem );
	}
	DefualtSettingDLG( m_pChungGiveItem, pStage );
	return m_pChungGiveItem;
}
#endif SERV_EVENT_CHUNG_GIVE_ITEM

CKTDGUIDialogType CX2MemoryHolder::GetCommonDLG( CKTDXStage* pStage )
{
	if ( m_pCommonDLG == NULL )
	{
		m_pCommonDLG = new CKTDGUIDialog( pStage, L"DLG_Common_BG.lua" );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pCommonDLG );
	

		CKTDGUIStatic* pCommonCharBG = NULL;
		int controlIndex = 0;
		wstring texName = L"";

		// note!! 전직 추가되면 수정, 혹은 unit_type으로
		switch ( g_pData->GetMyUser()->GetSelectUnit()->GetType() )
		{
		case CX2Unit::UT_ELSWORD:
			{
				controlIndex = 0;
				texName = L"DLG_Background_Elsword.tga";
			}
			break;

		case CX2Unit::UT_ARME:
			{
				controlIndex = 1;
				texName = L"DLG_Background_Aisha.tga";
			}
			break;

		case CX2Unit::UT_LIRE:
			{
				controlIndex = 2;
				texName = L"DLG_Background_Rena.tga";
			}
			break;

		case CX2Unit::UT_RAVEN:
			{
				controlIndex = 3;
				texName = L"DLG_Background_Raven.tga";
			}
			break;

		case CX2Unit::UT_EVE:
			{
				controlIndex = 4;								
				texName = L"DLG_Background_Eve.tga";			
			}
			break;

		//{{ kimhc // 2010.11.24 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
		case CX2Unit::UT_CHUNG:
			{
				controlIndex = 5;								
				texName = L"DLG_Background_Chung.tga";			
			}
			break;
#endif	NEW_CHARACTER_CHUNG
//}} kimhc // 2010.11.24 //  2010-12-23 New Character CHUNG

#ifdef ARA_CHARACTER_BASE
		case CX2Unit::UT_ARA:
			{
				controlIndex = 6;								
				texName = L"DLG_Background_Ara.tga";
			}
			break;
#endif

#ifdef NEW_CHARACTER_EL
		case CX2Unit::UT_ELESIS:
			{
				controlIndex = 7;								
				texName = L"DLG_Background_ELESIS.tga";
			} break;
#endif // NEW_CHARACTER_EL

#ifdef SERV_9TH_NEW_CHARACTER // 김태환 ( 캐릭터 추가용 )
		case CX2Unit::UT_ADD:
			{
				controlIndex = 8;								
				texName = L"DLG_Background_Add.tga";
			} break;
#endif //SERV_9TH_NEW_CHARACTER

		default:
			{
				controlIndex = 0;
				texName = L"DLG_Background_Elsword.tga";
			}
			break;
		}

		CKTDGUIStatic* pChararcterBG = (CKTDGUIStatic*)m_pCommonDLG->GetControl( L"ChararcterBG" );
		if ( NULL != pChararcterBG && NULL != pChararcterBG->GetPicture(controlIndex) )
			pChararcterBG->GetPicture(controlIndex)->SetShow( true );
		else
			ASSERT("DLG_Common_BG.lua Error");

		CKTDGUIStatic* pCharacter = (CKTDGUIStatic*)m_pCommonDLG->GetControl( L"Character" );
		if ( NULL != pCharacter && NULL != pCharacter->GetPicture(0) )
			pCharacter->GetPicture(0)->SetTex( texName.c_str() );
	}

	DefualtSettingDLG( m_pCommonDLG, pStage );

	g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pCommonDLG, false );

	return m_pCommonDLG;
}

void CX2MemoryHolder::RelaseCommonDLG()
{
	SAFE_DELETE_DIALOG( m_pCommonDLG );
}


void CX2MemoryHolder::LoadVillageMemory( int villageID )
{
	if ( m_VillageID == villageID )
		return;

	ReleaseVillageMapDLG();
	ReleaseLocalMapDLG();

	m_VillageID = villageID;

	vector<CKTDXDevice*>					vecVillageDevice;
/*	
	// 샵에서 사용하는 리소스 미리 로딩
	vector<const CX2Item::ItemTemplet*> vecShopItemList;
	g_pData->GetItemManager()->GetShopItemList( villageID, vecShopItemList );

	for( UINT i=0; i<vecShopItemList.size(); i++ )
	{
		CX2Item::ItemTemplet* pItemTemplet = vecShopItemList[i];
		if ( pItemTemplet != NULL )
		{
			CKTDXDevice* pDevice = TextureReady( pItemTemplet->m_ShopImage.c_str() );
			if ( pDevice != NULL )
				vecVillageDevice.push_back( pDevice );


			if ( pItemTemplet->m_EqipType == CX2Item::ET_ATTACH_NORMAL )
			{
				pDevice = XMeshReady( pItemTemplet->m_ModelName.c_str() );
				if ( pDevice != NULL )
					vecVillageDevice.push_back( pDevice );
			}
			else
			{
				pDevice = XSkinMeshReady( pItemTemplet->m_ModelName.c_str() );
				if ( pDevice != NULL )
					vecVillageDevice.push_back( pDevice );
			}
			
		}
	}
*/
	ReleaseVillageMemory();
	m_vecVillageDevice = vecVillageDevice;
}
void CX2MemoryHolder::ReleaseVillageMemory()
{
	for ( int i = 0; i < (int)m_vecVillageDevice.size(); i++ )
	{
		CKTDXDevice* pDevice = m_vecVillageDevice[i];
		SAFE_CLOSE( pDevice );
	}

	m_vecVillageDevice.resize(0);
}

void CX2MemoryHolder::LoadEquip( const WCHAR* pDeviceID )
{
//#ifdef EQUIP_BACKGROUND_LOADING_TEST // 2008-12-13
    /** Unit이 사용하는 장비들을 로드할 때는 m_EqipDeviceList를 사용하지 않아야 한다.
        - jintaeks on 2008-12-13, 17:12 */
    return;
//#endif // EQUIP_BACKGROUND_LOADING_TEST // 2008-12-13

	if( NULL == pDeviceID )
		return; 

	
	CKTDXDeviceXSkinMesh* pEqip = (CKTDXDeviceXSkinMesh*)g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( pDeviceID );
	if( pEqip != NULL )
	{
		if( pEqip->GetRefCount() == 2 )
		{
			m_EqipDeviceList.push_back( (CKTDXDevice*)pEqip );
			if( m_EqipDeviceList.size() > 200 )
			{
				CKTDXDeviceXSkinMesh* pTempEqip = (CKTDXDeviceXSkinMesh*)m_EqipDeviceList[0];
				SAFE_CLOSE( pTempEqip );
				m_EqipDeviceList.erase( m_EqipDeviceList.begin() );
			}
		}
		else
		{
			SAFE_CLOSE( pEqip );
		}
	}
	
}

void CX2MemoryHolder::ClearEquip()
{
	for( int i = 0; i < (int)m_EqipDeviceList.size(); i++ )
	{
		CKTDXDevice* pCKTDXDevice = m_EqipDeviceList[i];
		SAFE_CLOSE( pCKTDXDevice );
	}

	m_EqipDeviceList.resize(0);
	
}

void CX2MemoryHolder::DefualtSettingDLG( CKTDGUIDialogType pDialog, CKTDXStage* pStage )
{
	if ( pDialog == NULL )
		return;

	pDialog->SetStage( pStage );

	pDialog->SetShow( true );
	pDialog->SetEnable( true );

	//g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( pDialog, true );
}

CKTDXDeviceXMesh* CX2MemoryHolder::XMeshReady( const WCHAR* pFileName )
{
	CKTDXDeviceXMesh* pDevice = g_pKTDXApp->GetDeviceManager()->OpenXMesh( pFileName );

	return pDevice;
}

CKTDXDeviceXSkinMesh* CX2MemoryHolder::XSkinMeshReady( const WCHAR* pFileName )
{
	CKTDXDeviceXSkinMesh* pDevice = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( pFileName );
	
	return pDevice;
}

CKTDXDeviceTexture* CX2MemoryHolder::TextureReady( const WCHAR* pFileName )
{
	CKTDXDeviceTexture* pDevice = g_pKTDXApp->GetDeviceManager()->OpenTexture( pFileName );
	
	return pDevice;
}

#ifdef SERV_CHINA_SPIRIT_EVENT
void CX2MemoryHolder::SetShowUseSpiritEvent( bool bShow )
{
	if( m_bShow == bShow )
		return;

	m_bShow = bShow;

	if( NULL != g_pData && NULL != g_pData->GetMyUser() && NULL != g_pData->GetMyUser()->GetSelectUnit() )
	{
		UpdateUseSpiritEvent();

		if( m_bShow == false )
			m_pUseSpiritEventDLG->SetShow( false );
	}
}

void CX2MemoryHolder::GetUseSpiritEventDLG( CKTDXStage* pStage, bool b )
{
	// 게이지 바 설정
	if ( m_pUseSpiritEventDLG == NULL )
	{
		m_pUseSpiritEventDLG = new CKTDGUIDialog( pStage, L"DLG_UI_Use_Spirit_Event.lua" );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pUseSpiritEventDLG );
	}

	DefualtSettingDLG( m_pUseSpiritEventDLG, pStage );

	/////////////////////////////////
	// state에 의한 뷰 설정
	if( false == b )
	{
		if( NULL != m_pUseSpiritEventDLG )
			m_pUseSpiritEventDLG->SetShow(false);
		return;
	}
	else
	{
		if( NULL != m_pUseSpiritEventDLG )
			m_pUseSpiritEventDLG->SetShow(true);
	}

	UpdateUseSpiritEvent();
}

void CX2MemoryHolder::UpdateUseSpiritEvent()
{
	if( m_pUseSpiritEventDLG == NULL )
		return;

	// 이벤트 꺼져있으면 끈다
	IF_EVENT_ENABLED( CEI_2013_CHINA_SPIRIT_EVENT )
	{
	}
	ELSE
	{
		m_pUseSpiritEventDLG->SetShow( false );
		return;
	}

	// 캐릭터 없을 때 끈다
	if( g_pData == NULL || g_pData->GetMyUser() == NULL || g_pData->GetMyUser()->GetSelectUnit() == NULL )
	{
		m_pUseSpiritEventDLG->SetShow( false );
		return;
	}

	// 타이틀 안 맞을 때 끈다
	int iSelectedLocationIndex = g_pData->GetMyUser()->GetSelectUnit()->GetChinaSpiritIndex();
	if( iSelectedLocationIndex > 5 || iSelectedLocationIndex < 0 )
	{
		m_pUseSpiritEventDLG->SetShow( false );
		return;
	}
	int iSelectedChinaSpirit = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_arrChinaSpirit[ iSelectedLocationIndex ];
	
	// 켜고
	m_pUseSpiritEventDLG->SetShow( true );

	// 사람 위치 셋팅
	CKTDGUIStatic* pStaticSpirit1 = static_cast<CKTDGUIStatic*>( m_pUseSpiritEventDLG->GetControl(L"Red_Bar") );
	if( pStaticSpirit1 != NULL )
	{
		pStaticSpirit1->SetOffsetPos_LUA( iSelectedChinaSpirit * 240.0f / 2350.0f, 0 );
	}

	// 일단 버튼 다 끄고
	for( int iLocationIndex = 0; iLocationIndex <= 5; ++iLocationIndex )
	{
		std::wstringstream wstrstmButtonName;
		wstrstmButtonName << L"Button" << iLocationIndex;
		CKTDGUIButton* kButton = static_cast<CKTDGUIButton*>( m_pUseSpiritEventDLG->GetControl( wstrstmButtonName.str().c_str() ) );

		if( NULL != kButton )
			kButton->SetShowEnable( false, false );
	}

	// 하나만 켠다
	{
		bool bReward = ( iSelectedChinaSpirit >= 2350 );

		std::wstringstream wstrstmButtonName;
		wstrstmButtonName << L"Button" << iSelectedLocationIndex;
		CKTDGUIButton* kSelectedButton = static_cast<CKTDGUIButton*>( m_pUseSpiritEventDLG->GetControl( wstrstmButtonName.str().c_str() ) );

		if( NULL != kSelectedButton )
			kSelectedButton->SetShowEnable( true, bReward );
	}
}

void CX2MemoryHolder::SetUseSpiritEventToolTip( bool bShow )
{
	if( m_pUseSpiritEventDLG != NULL )
	{
		CKTDGUIStatic* pStaticRedButton = 
			static_cast<CKTDGUIStatic*>( m_pUseSpiritEventDLG->GetControl(L"Button_Invisible_SpiritGuage1") );

		if ( NULL != pStaticRedButton )
		{
			pStaticRedButton->SetShow( !bShow );
		}
	}
}

void CX2MemoryHolder::ShowRedPicture(bool bShow)
{
	if( m_pUseSpiritEventDLG == NULL )
		return;

	CKTDGUIStatic *  staticCtrol= static_cast<CKTDGUIStatic*>( m_pUseSpiritEventDLG->GetControl(L"AlramRed") );
	if( NULL != staticCtrol )
	{
		staticCtrol->SetShow(bShow);		
	}
}
#endif SERV_CHINA_SPIRIT_EVENT

#ifdef SERV_EVENT_COBO_DUNGEON_AND_FIELD
CKTDGUIDialogType CX2MemoryHolder::GetUseCoboEventDLG( CKTDXStage* pStage )
{
	if(m_pUseCoboEventDLG == NULL)
	{
		m_pUseCoboEventDLG = new CKTDGUIDialog(pStage,L"DLG_UI_Cobo_Event.lua"); 
		if( m_pUseCoboEventDLG != NULL)
		{
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pUseCoboEventDLG );
		}
	}
	DefualtSettingDLG( m_pUseCoboEventDLG, pStage );
	return m_pUseCoboEventDLG;
}
CKTDGUIDialogType CX2MemoryHolder::GetCoboEventCountDLG( CKTDXStage* pStage )
{
	if(m_pCoboEventCountDLG == NULL)
	{
		m_pCoboEventCountDLG = new CKTDGUIDialog(pStage,L"DLG_UI_COBO_COUNT.lua"); 
		if( m_pCoboEventCountDLG != NULL )
		{
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pCoboEventCountDLG );
		}
	}
	DefualtSettingDLG( m_pCoboEventCountDLG, pStage );
	return m_pCoboEventCountDLG;
}
#endif SERV_EVENT_COBO_DUNGEON_AND_FIELD

#ifdef SERV_NEW_YEAR_EVENT_2014
void CX2MemoryHolder::SetShowNewYear2014Event( bool bShow )
{
	if( m_bShowNewYear2014Event == bShow )
	{
		m_pNewYear2014EventDLG->SetShow( m_bShowNewYear2014Event );
		return;
	}

	m_bShowNewYear2014Event = bShow;

	if( NULL != g_pData && NULL != g_pData->GetMyUser() && NULL != g_pData->GetMyUser()->GetSelectUnit() )
	{
		UpdateNewYear2014Event();
	}

	m_pNewYear2014EventDLG->SetShow( m_bShowNewYear2014Event );
}

void CX2MemoryHolder::SetShowNewYear2014EventBoard( bool bShow )
{
	m_bShowNewYear2014EventBoard = bShow;
	UpdateNewYear2014Event();
}

bool CX2MemoryHolder::GetShowNewYear2014EventBoard() const
{
	return m_bShowNewYear2014EventBoard;
}

CKTDGUIDialogType CX2MemoryHolder::GetNewYear2014EventDLG( CKTDXStage* pStage )
{
	// 게이지 바 설정
	if ( m_pNewYear2014EventDLG == NULL )
	{
		m_pNewYear2014EventDLG = new CKTDGUIDialog( pStage, L"DLG_UI_New_Year_2014_Event.lua" );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pNewYear2014EventDLG );
	}

	DefualtSettingDLG( m_pNewYear2014EventDLG, pStage );

	return m_pNewYear2014EventDLG;
}

void CX2MemoryHolder::UpdateNewYear2014Event()
{
	if( m_pNewYear2014EventDLG == NULL )
		return;

	// 캐릭터 없을 때 끈다
	if( g_pData == NULL || g_pData->GetMyUser() == NULL || g_pData->GetMyUser()->GetSelectUnit() == NULL || m_bShowNewYear2014Event == false )
	{
		m_pNewYear2014EventDLG->SetShow( false );
		return;
	}

	CX2Unit* pUnit = g_pData->GetMyUser()->GetSelectUnit();
	int iCurrentLevel = pUnit->GetUnitData().m_Level;

	// 일단 켜고
	m_pNewYear2014EventDLG->SetShow( true );

	// 2013년 이벤트용 UI
	CKTDGUIButton* pSpecialButton = static_cast<CKTDGUIButton*>( m_pNewYear2014EventDLG->GetControl( L"Special" ) );
	CKTDGUIButton* pCompleteButton = static_cast<CKTDGUIButton*>( m_pNewYear2014EventDLG->GetControl( L"Complete" ) );
	CKTDGUIButton* pSupportButton = static_cast<CKTDGUIButton*>( m_pNewYear2014EventDLG->GetControl( L"Support" ) );

	// 2014년 이벤트용 UI
	CKTDGUIButton* pNewYearButton = static_cast<CKTDGUIButton*>( m_pNewYear2014EventDLG->GetControl( L"NewYear" ) );

	IF_EVENT_ENABLED( CEI_OLD_YEAR_EVENT_2013 )
	{
		// 정상적인 경우엔 2014년 이벤트를 끄고 2013년을 켜지는 않으나 혹시 모르니 꺼 둡시다.
		pNewYearButton->SetShowEnable( false, false );
		m_pNewYear2014EventDLG->SetShowEnableControlsWithDummyInt( 0, 1, false, false );

		int iOldYearMissionRewardedLevel = static_cast< int >( pUnit->GetOldYearMissionRewardedLevel() );

		if( iOldYearMissionRewardedLevel > g_pInstanceData->GetMaxLevel() )
		{
			pSpecialButton->SetShowEnable( false, false );
		}
		else
		{
			CX2Inventory& kInventory = pUnit->AccessInventory();
			CX2Item* pItem = kInventory.GetItemByTID( _CONST_SERV_NEW_YEAR_EVENT_2014_::iMaxLevelSpecialMissionItemID );

			// 특별 미션은 모든 유저에게 보여주지만 조건을 달성한 유저만 enable
			if( pItem != NULL && iOldYearMissionRewardedLevel == g_pInstanceData->GetMaxLevel() )
			{
				pSpecialButton->SetShowEnable( true, true );
			}
			else
			{
				pSpecialButton->SetShowEnable( true, false );
			}
		}

		// 만렙 서포트와 달성 미션은 만렙이냐 아니냐에 따라 다르게 표시해 주어야 한다.
		// 만렙 직전 렙 서포트 보상을 안 받았는데 만렙보상을 받으면 해당 서포트 보상이 꺼지는건 현구조상 고칠 수 없으므로,
		// 직전 렙 보상을 안 받으면 만렙 보상을 못 받도록 설정하였습니다. by 박진웅
		if( iOldYearMissionRewardedLevel >= g_pInstanceData->GetMaxLevel() )
		{
			pSupportButton->SetShowEnable( false, false );
			pCompleteButton->SetShowEnable( false, false );
		}
		else if( iOldYearMissionRewardedLevel == g_pInstanceData->GetMaxLevel() - 1 )
		{
			pSupportButton->SetShowEnable( false, false );

			bool bClassChange = false;
			if( pUnit->GetClassLevel() >= 2 )
				bClassChange = true;

			IF_EVENT_ENABLED( CEI_ALLOW_EVE_FIRST_CLASS_OLD_YEAR_EVENT )
			{
				if( pUnit->GetClassLevel() >= 1 && pUnit->GetType() == CX2Unit::UT_EVE )
					bClassChange = true;
			}
			IF_EVENT_ENABLED( CEI_ALLOW_ARA_FIRST_CLASS_OLD_YEAR_EVENT )
			{
				if( pUnit->GetClassLevel() >= 1 && pUnit->GetType() == CX2Unit::UT_ARA )
					bClassChange = true;
			}
			IF_EVENT_ENABLED( CEI_ALLOW_ELESIS_FIRST_CLASS_OLD_AND_NEW_YEAR_EVENT )
			{
				if( pUnit->GetClassLevel() >= 1 && pUnit->GetType() == CX2Unit::UT_ELESIS )
					bClassChange = true;
			}

			if( iCurrentLevel == g_pInstanceData->GetMaxLevel() && bClassChange == true )
			{
				// 조건 달성한 상태 : 이벤트 시작한 이후 만렙 달성했고, 2차 전직 조건도 달성 중
				pCompleteButton->SetShowEnable( true, true );
			}
			else
			{
				// 조건 달성 못한 상태 : 전직을 못 한 유저
				pCompleteButton->SetShowEnable( true, false );
			}
		}
		else
		{
			pCompleteButton->SetShowEnable( true, false );
			if( iCurrentLevel > iOldYearMissionRewardedLevel &&
				iCurrentLevel >= 26 &&
				pUnit->GetClassLevel() >= 1 )
			{
				// 조건 달성한 상태 : 이벤트 시작한 이후 렙업을 한 유저
				pSupportButton->SetShowEnable( true, true );
			}
			else
			{
				// 조건 달성 못한 상태 : 아직 (추가로) 렙업을 못 한 유저
				pSupportButton->SetShowEnable( true, false );
			}
		}
	}
	ELSE_IF_EVENT_ENABLED( CEI_NEW_YEAR_EVENT_2014 )
	{
		// 2013년 이벤트 off
		pSpecialButton->SetShowEnable( false, false );
		pCompleteButton->SetShowEnable( false, false );
		pSupportButton->SetShowEnable( false, false );

		int iNewYearMissionStepID = pUnit->GetNewYearMissionStepID();		// 2014년 이벤트 on

		if( iNewYearMissionStepID < 0 )
		{
			// 대상자가 아닌 경우
			pNewYearButton->SetShowEnable( false, false );
			m_pNewYear2014EventDLG->SetShowEnableControlsWithDummyInt( 0, 1, false, false );
		}
		else
		{
			CKTDGUIButton* pNewYearMission1Button = static_cast<CKTDGUIButton*>( m_pNewYear2014EventDLG->GetControl( L"NewYearMission1" ) );
			CKTDGUIButton* pNewYearMission2Button = static_cast<CKTDGUIButton*>( m_pNewYear2014EventDLG->GetControl( L"NewYearMission2" ) );
			CKTDGUIButton* pNewYearMission3Button = static_cast<CKTDGUIButton*>( m_pNewYear2014EventDLG->GetControl( L"NewYearMission3" ) );
			CKTDGUIButton* pNewYearMission4Button = static_cast<CKTDGUIButton*>( m_pNewYear2014EventDLG->GetControl( L"NewYearMission4" ) );
			CKTDGUIButton* pNewYearMission5Button = static_cast<CKTDGUIButton*>( m_pNewYear2014EventDLG->GetControl( L"NewYearMission5" ) );
			CKTDGUIButton* pNewYearMission1Progress = static_cast<CKTDGUIButton*>( m_pNewYear2014EventDLG->GetControl( L"NewYearMission1Progress" ) );
			CKTDGUIButton* pNewYearMission2Progress = static_cast<CKTDGUIButton*>( m_pNewYear2014EventDLG->GetControl( L"NewYearMission2Progress" ) );
			CKTDGUIButton* pNewYearMission3Progress = static_cast<CKTDGUIButton*>( m_pNewYear2014EventDLG->GetControl( L"NewYearMission3Progress" ) );
			CKTDGUIButton* pNewYearMission4Progress = static_cast<CKTDGUIButton*>( m_pNewYear2014EventDLG->GetControl( L"NewYearMission4Progress" ) );
			CKTDGUIButton* pNewYearMission5Progress = static_cast<CKTDGUIButton*>( m_pNewYear2014EventDLG->GetControl( L"NewYearMission5Progress" ) );
			CKTDGUIButton* pNewYearMission1Complete = static_cast<CKTDGUIButton*>( m_pNewYear2014EventDLG->GetControl( L"NewYearMission1Complete" ) );
			CKTDGUIButton* pNewYearMission2Complete = static_cast<CKTDGUIButton*>( m_pNewYear2014EventDLG->GetControl( L"NewYearMission2Complete" ) );
			CKTDGUIButton* pNewYearMission3Complete = static_cast<CKTDGUIButton*>( m_pNewYear2014EventDLG->GetControl( L"NewYearMission3Complete" ) );
			CKTDGUIButton* pNewYearMission4Complete = static_cast<CKTDGUIButton*>( m_pNewYear2014EventDLG->GetControl( L"NewYearMission4Complete" ) );
			CKTDGUIButton* pNewYearMission5Complete = static_cast<CKTDGUIButton*>( m_pNewYear2014EventDLG->GetControl( L"NewYearMission5Complete" ) );

			// 시작 버튼은 켜고
			pNewYearButton->SetShowEnable( true, true );

			if( m_bShowNewYear2014EventBoard == false )
			{
				// 미션 보드 UI를 끔
				m_pNewYear2014EventDLG->SetShowEnableControlsWithDummyInt( 0, 1, false, false );
				return;
			}

			// 보드 켜진 상태이므로 미션 보드 UI를 켜 놓고
			m_pNewYear2014EventDLG->SetShowEnableControlsWithDummyInt( 0, 1, true, true );

			// 미션 버튼은 일단 다 꺼놓는다.
			m_pNewYear2014EventDLG->SetShowEnableControlsWithDummyInt( 1, 2, true, false );		// 미션 버튼
			m_pNewYear2014EventDLG->SetShowEnableControlsWithDummyInt( 1, 3, false, false );	// 진행 버튼
			m_pNewYear2014EventDLG->SetShowEnableControlsWithDummyInt( 1, 4, false, false );	// 완료 버튼

			// 현재 진행도
			int iNewYearMissionStepID = pUnit->GetNewYearMissionStepID();
				
			if( iNewYearMissionStepID <= 1 )
			{
				// 1번 미션
				if( iNewYearMissionStepID == 0 )
				{
					// 1번 미션 수락 가능한 상태
					pNewYearMission1Button->SetShowEnable( true, true );
				}
				else if( pUnit->GetClassLevel() >= 1 )
				{
					// 1번 미션 완료 가능한 상태
					pNewYearMission1Complete->SetShowEnable( true, true );
				}
				else
				{
					// 1번 미션 진행 중
					pNewYearMission1Progress->SetShowEnable( true, true );
				}
			}
			else if( iNewYearMissionStepID <= 3 )
			{
				// 2번 미션
				pNewYearMission1Button->SetShowEnable( false, false );
				pNewYearMission1Complete->SetShowEnable( true, false );

				if( iNewYearMissionStepID == 2 )
				{
					// 2번 미션 수락 가능한 상태
					pNewYearMission2Button->SetShowEnable( true, true );
				}
				else if( iCurrentLevel >= 20 )
				{
					// 2번 미션 완료 가능한 상태
					pNewYearMission2Complete->SetShowEnable( true, true );
				}
				else
				{
					// 2번 미션 진행 중
					pNewYearMission2Progress->SetShowEnable( true, true );
				}
			}
			else if( iNewYearMissionStepID <= 5 )
			{
				// 3번 미션
				pNewYearMission1Button->SetShowEnable( false, false );
				pNewYearMission2Button->SetShowEnable( false, false );
				pNewYearMission1Complete->SetShowEnable( true, false );
				pNewYearMission2Complete->SetShowEnable( true, false );

				if( iNewYearMissionStepID == 4 )
				{
					// 3번 미션 수락 가능한 상태
					pNewYearMission3Button->SetShowEnable( true, true );
				}
				else if( iCurrentLevel >= 30 )
				{
					// 3번 미션 완료 가능한 상태
					pNewYearMission3Complete->SetShowEnable( true, true );
				}
				else
				{
					// 3번 미션 진행 중
					pNewYearMission3Progress->SetShowEnable( true, true );
				}
			}
			else if( iNewYearMissionStepID <= 7 )
			{
				// 4번 미션
				pNewYearMission1Button->SetShowEnable( false, false );
				pNewYearMission2Button->SetShowEnable( false, false );
				pNewYearMission3Button->SetShowEnable( false, false );
				pNewYearMission1Complete->SetShowEnable( true, false );
				pNewYearMission2Complete->SetShowEnable( true, false );
				pNewYearMission3Complete->SetShowEnable( true, false );

				bool bElesis = false;
				IF_EVENT_ENABLED( CEI_ALLOW_ELESIS_FIRST_CLASS_OLD_AND_NEW_YEAR_EVENT )
				{
					if( iCurrentLevel >= 35 && pUnit->GetType() == CX2Unit::UT_ELESIS )
						bElesis = true;
				}

				if( iNewYearMissionStepID == 6 )
				{
					// 4번 미션 수락 가능한 상태
					pNewYearMission4Button->SetShowEnable( true, true );
				}
				else if( pUnit->GetClassLevel() >= 2 || true == bElesis )
				{
					// 4번 미션 완료 가능한 상태
					pNewYearMission4Complete->SetShowEnable( true, true );
				}
				else
				{
					// 4번 미션 진행 중
					pNewYearMission4Progress->SetShowEnable( true, true );
				}
			}
			else if( iNewYearMissionStepID <= 9 )
			{
				// 5번 미션
				pNewYearMission1Button->SetShowEnable( false, false );
				pNewYearMission2Button->SetShowEnable( false, false );
				pNewYearMission3Button->SetShowEnable( false, false );
				pNewYearMission4Button->SetShowEnable( false, false );
				pNewYearMission1Complete->SetShowEnable( true, false );
				pNewYearMission2Complete->SetShowEnable( true, false );
				pNewYearMission3Complete->SetShowEnable( true, false );
				pNewYearMission4Complete->SetShowEnable( true, false );

				if( iNewYearMissionStepID == 8 )
				{
					// 5번 미션 수락 가능한 상태
					pNewYearMission5Button->SetShowEnable( true, true );
				}
				else if( iCurrentLevel >= 45 )
				{
					// 5번 미션 완료 가능한 상태
					pNewYearMission5Complete->SetShowEnable( true, true );
				}
				else
				{
					// 5번 미션 진행 중
					pNewYearMission5Progress->SetShowEnable( true, true );
				}
			}
			else
			{
				m_pNewYear2014EventDLG->SetShowEnableControlsWithDummyInt( 1, 2, false, false );	// 미션 버튼
				m_pNewYear2014EventDLG->SetShowEnableControlsWithDummyInt( 1, 4, true, false );		// 완료 버튼
			}
		}
	}
	ELSE
	{
		m_pNewYear2014EventDLG->SetShow( false );
	}
}
#endif SERV_NEW_YEAR_EVENT_2014

#ifdef SERV_EVENT_CHECK_POWER
void CX2MemoryHolder::SetShowCheckPowerEvent( bool bShow )
{
	if( m_bShowCheckPowerEvent == bShow )
	{
		m_pCheckPowerEventDLG->SetShow( m_bShowCheckPowerEvent );
		return;
	}

	m_bShowCheckPowerEvent = bShow;

	if( NULL != g_pData && NULL != g_pData->GetMyUser() && NULL != g_pData->GetMyUser()->GetSelectUnit() )
	{
		UpdateCheckPowerEvent();
	}

	m_pCheckPowerEventDLG->SetShow( m_bShowCheckPowerEvent );
}

CKTDGUIDialogType CX2MemoryHolder::GetCheckPowerEventDLG( CKTDXStage* pStage )
{
	// 게이지 바 설정
	if ( m_pCheckPowerEventDLG == NULL )
	{
		m_pCheckPowerEventDLG = new CKTDGUIDialog( pStage, L"DLG_UI_Check_Power_Event.lua" );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pCheckPowerEventDLG );

		CKTDGUIStatic* pRecruitPage = static_cast<CKTDGUIStatic*>( m_pCheckPowerEventDLG->GetControl( L"RecruitPage" ) );
		CKTDGUIStatic* pGuidePage = static_cast<CKTDGUIStatic*>( m_pCheckPowerEventDLG->GetControl( L"GuidePage" ) );

#ifdef CLIENT_GLOBAL_LINEBREAK
		pRecruitPage->GetString( 0 )->msg = CWordLineHandler::GetStrByLineBreakInX2Main( pRecruitPage->GetString( 0 )->msg.c_str(), 244, pRecruitPage->GetString(0)->fontIndex );
		pGuidePage->GetString( 0 )->msg = CWordLineHandler::GetStrByLineBreakInX2Main( pGuidePage->GetString( 0 )->msg.c_str(), 244, pGuidePage->GetString(0)->fontIndex );
#else //CLIENT_GLOBAL_LINEBREAK
		pRecruitPage->GetString( 0 )->msg = g_pMain->GetStrByLienBreak( pRecruitPage->GetString( 0 )->msg.c_str(), 244, pRecruitPage->GetString(0)->fontIndex );
		pGuidePage->GetString( 0 )->msg = g_pMain->GetStrByLienBreak( pGuidePage->GetString( 0 )->msg.c_str(), 244, pGuidePage->GetString(0)->fontIndex );
#endif //CLIENT_GLOBAL_LINEBREAK
	}

	DefualtSettingDLG( m_pCheckPowerEventDLG, pStage );

	return m_pCheckPowerEventDLG;
}

void CX2MemoryHolder::UpdateCheckPowerEvent()
{
	if( m_pCheckPowerEventDLG == NULL )
		return;

	// 캐릭터 없을 때 끈다
	if( g_pData == NULL || g_pData->GetMyUser() == NULL || g_pData->GetMyUser()->GetSelectUnit() == NULL || m_bShowCheckPowerEvent == false )
	{
		m_pCheckPowerEventDLG->SetShow( false );
		return;
	}

	CX2Unit* pUnit = g_pData->GetMyUser()->GetSelectUnit();
	int iCurrentLevel = pUnit->GetUnitData().m_Level;

	m_pCheckPowerEventDLG->SetShow( false );
	IF_EVENT_ENABLED( CEI_CHECK_POWER )
	{
		if( pUnit->GetClassLevel() >= 1 )
		{
			m_pCheckPowerEventDLG->SetShow( true );

			// 버튼 및 스태틱
			CKTDGUIButton* pAllegroButton = static_cast<CKTDGUIButton*>( m_pCheckPowerEventDLG->GetControl( L"Allegro" ) );
			CKTDGUIStatic* pStaticProgress = static_cast<CKTDGUIStatic*>( m_pCheckPowerEventDLG->GetControl( L"Progress" ) );

			m_pCheckPowerEventDLG->SetShowEnableControlsWithDummyInt( 0, 1, false, false );
			m_pCheckPowerEventDLG->SetShowEnableControlsWithDummyInt( 0, 2, false, false );
			m_pCheckPowerEventDLG->SetShowEnableControlsWithDummyInt( 0, 3, false, false );

			bool bShowTimePage = true;
			{
				__int64 iCurrentTime = g_pData->GetServerCurrentTime();
				__int64 iStartTime = pUnit->GetCheckPowerTime();
				CTimeSpan tPlayTime = CTime( iCurrentTime ) - CTime( iStartTime );

				if( tPlayTime < CTimeSpan( 0, 0, 20, 0 ) )
				{
					pStaticProgress->SetShowEnable( true, true );
				}
				else
				{
					pStaticProgress->SetShowEnable( false, false );
					bShowTimePage = false;
				}
			}

			if( g_pMain->GetNowStateID() != CX2Main::XS_DUNGEON_GAME )
			{
				if( pUnit->IsShowCheckPowerPopUp() == true )
				{
					m_pCheckPowerEventDLG->SetShowEnableControlsWithDummyInt( 0, 1, true, true );
				}
				else
				{
					unsigned char ucMaxCount = 2;
					if( g_pData->GetMyUser()->GetSelectUnit()->GetType() == CX2Unit::UT_ELESIS )
					{
						ucMaxCount = 3;
					}

					if( m_bShowCheckPowerGuide )
					{
						pAllegroButton->SetShowEnable( false, false );
						m_pCheckPowerEventDLG->SetShowEnableControlsWithDummyInt( 0, 2, true, true );
					}
					else if( bShowTimePage )
					{
						pAllegroButton->SetShowEnable( false, false );
					}
					else
					{
						if( pUnit->GetCheckPowerCount() < ucMaxCount )
							pAllegroButton->SetShowEnable( true, true );
						else
							pAllegroButton->SetShowEnable( true, false );
					}
				}
			}
		}
	}
}

void CX2MemoryHolder::UpdateCheckPowerEventTimer()
{
	if( m_pCheckPowerEventDLG != NULL )
	{
		CKTDGUIStatic* pStaticProgress = static_cast<CKTDGUIStatic*>( m_pCheckPowerEventDLG->GetControl( L"Progress" ) );
		if( pStaticProgress->GetShow() == true && g_pData != NULL && g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetSelectUnit() != NULL )
		{
			CX2Unit* pUnit = g_pData->GetMyUser()->GetSelectUnit();
			__int64 iCurrentTime = g_pData->GetServerCurrentTime();
			__int64 iStartTime = pUnit->GetCheckPowerTime();
			CTimeSpan tPlayTime = CTime( iCurrentTime ) - CTime( iStartTime );

			if( tPlayTime < CTimeSpan( 0, 0, 10, 0 ) )
			{
				CTimeSpan tRemainTime = CTimeSpan( 0, 0, 10, 0 ) - tPlayTime;
				pStaticProgress->GetString( 0 )->msg = GET_REPLACED_STRING( ( STR_ID_29775, "S", tRemainTime.Format( L"%M:%S" ) ) );
			}
			else
			{
				CTimeSpan tRemainTime = CTimeSpan( 0, 0, 20, 0 ) - tPlayTime;
				pStaticProgress->GetString( 0 )->msg = GET_REPLACED_STRING( ( STR_ID_29776, "S", tRemainTime.Format( L"%M:%S" ) ) );
			}
		}
	}
}
#endif SERV_EVENT_CHECK_POWER

#ifdef ALWAYS_EVENT_ADAMS_UI_SHOP
CKTDGUIDialogType CX2MemoryHolder::GetUseAdamsEventShop(CKTDXStage* pStage )
{
	if(m_pUseAdamsEventShopDLG == NULL)
	{
		m_pUseAdamsEventShopDLG = new CKTDGUIDialog(pStage,L"DLG_UI_Event_AdamsShop_Button.lua"); 
		if( m_pUseAdamsEventShopDLG != NULL)
		{
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pUseAdamsEventShopDLG );
		}
	}
	DefualtSettingDLG( m_pUseAdamsEventShopDLG, pStage );
	return m_pUseAdamsEventShopDLG;
}
#endif ALWAYS_EVENT_ADAMS_UI_SHOP

#ifdef SERV_4TH_ANNIVERSARY_EVENT
void CX2MemoryHolder::SetShow4thEvent( bool bShow )
{
	if( m_bShow4thYearEvent == bShow )
	{
		m_p4thYearEventDLG->SetShow( m_bShow4thYearEvent );
		return;
	}

	m_bShow4thYearEvent = bShow;
	Update4thEvent();
}

CKTDGUIDialogType CX2MemoryHolder::Get4thEventDLG( CKTDXStage* pStage )
{
	if ( m_p4thYearEventDLG == NULL )
	{
		m_p4thYearEventDLG = new CKTDGUIDialog( pStage, L"DLG_UI_4th_Year_Event.lua" );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_p4thYearEventDLG );
	}

	return m_p4thYearEventDLG;
}

void CX2MemoryHolder::Update4thEvent()
{
	if( Get4thEventDLG( (CKTDXStage*)g_pMain->GetNowState() ) == NULL )
		return;

	// 캐릭터가 없거나 꺼 놓은 상태
	if( g_pData == NULL || g_pData->GetMyUser() == NULL || g_pData->GetMyUser()->GetSelectUnit() == NULL || m_bShow4thYearEvent == false )
	{
		m_p4thYearEventDLG->SetShow( false );
		return;
	}

	// 켜도 되는 상태
	m_p4thYearEventDLG->SetShow( true );
	m_p4thYearEventDLG->SetDisableUnderWindow( true );

	// 사용 정보 가져옴
	for( int iRewardIndex = 0; iRewardIndex < 12; ++iRewardIndex )
	{
		std::wstringstream wstrstm;
		wstrstm << L"Button" << ( iRewardIndex + 1 );
		CKTDGUIButton* pButton = static_cast<CKTDGUIButton*>( m_p4thYearEventDLG->GetControl( wstrstm.str().c_str() ) );
		if( pButton != NULL )
		{
			if( g_pInstanceData->Is4thRewarded( iRewardIndex ) == true )
				pButton->SetShowEnable( false, false );
			else
				pButton->SetShowEnable( true, true );
		}
	}

	// 유저 정보 가져옴
	K4ThAnnivEventInfo k4ThAnnivEventInfo;
	g_pInstanceData->GetK4ThAnnivEventInfo( k4ThAnnivEventInfo );

	// 펫 생성일
	CKTDGUIStatic* pStaticPic1 = static_cast<CKTDGUIStatic*>( m_p4thYearEventDLG->GetControl( L"pic1" ) );
	if( pStaticPic1 != NULL )
	{
		std::wstring wstrTime( CTime( k4ThAnnivEventInfo.m_tTimeFirstPet ).Format( _T( "%Y-%m-%d %H:%M:%S" ) ) );
		pStaticPic1->GetString( 0 )->msg = GET_REPLACED_STRING( ( STR_ID_30415, "L", wstrTime ) );
	}

	// 퀘스트 완료수
	CKTDGUIStatic* pStaticPic2 = static_cast<CKTDGUIStatic*>( m_p4thYearEventDLG->GetControl( L"pic2" ) );
	if( pStaticPic2 != NULL )
	{
		std::wstringstream wstrstm;
		wstrstm << k4ThAnnivEventInfo.m_iCountQuestComplete;
		pStaticPic2->GetString( 0 )->msg = GET_REPLACED_STRING( ( STR_ID_30420, "L", wstrstm.str() ) );
	}

	// 첫 엘소드 플레이날
	CKTDGUIStatic* pStaticPic3 = static_cast<CKTDGUIStatic*>( m_p4thYearEventDLG->GetControl( L"pic3" ) );
	if( pStaticPic3 != NULL )
	{
		std::wstring wstrTime( CTime( k4ThAnnivEventInfo.m_tTimeFirstPlay ).Format( _T( "%Y-%m-%d %H:%M:%S" ) ) );
		pStaticPic3->GetString( 0 )->msg = GET_REPLACED_STRING( ( STR_ID_30414, "L", wstrTime ) );
	}

	// 총부활 횟수
	CKTDGUIStatic* pStaticPic4 = static_cast<CKTDGUIStatic*>( m_p4thYearEventDLG->GetControl( L"pic4" ) );
	if( pStaticPic4 != NULL )
	{
		std::wstringstream wstrstm;
		wstrstm << k4ThAnnivEventInfo.m_iCountResurrect;
		pStaticPic4->GetString( 0 )->msg = GET_REPLACED_STRING( ( STR_ID_30424, "L", wstrstm.str() ) );
	}

	// 최장 접속 시간
	CKTDGUIStatic* pStaticPic5 = static_cast<CKTDGUIStatic*>( m_p4thYearEventDLG->GetControl( L"pic5" ) );
	if( pStaticPic5 != NULL )
	{
		std::wstringstream wstrstm;
		int iHour = k4ThAnnivEventInfo.m_iLongestConnectTime / 3600;
		int iMin = (k4ThAnnivEventInfo.m_iLongestConnectTime % 3600) / 60;
		int iSec = k4ThAnnivEventInfo.m_iLongestConnectTime % 60;

		wstrstm << iHour << L":";
		if( iMin < 10 )
			wstrstm << 0;
		wstrstm << iMin << L":";
		if( iSec < 10 )
			wstrstm << 0;
		wstrstm << iSec;

		pStaticPic5->GetString( 0 )->msg = GET_REPLACED_STRING( ( STR_ID_30419, "L", wstrstm.str() ) );
	}

	// 첫 헤니르 플레이날
	CKTDGUIStatic* pStaticPic6 = static_cast<CKTDGUIStatic*>( m_p4thYearEventDLG->GetControl( L"pic6" ) );
	if( pStaticPic6 != NULL )
	{
		std::wstring wstrTime( CTime( k4ThAnnivEventInfo.m_tTimeFirstHenir ).Format( _T( "%Y-%m-%d %H:%M:%S" ) ) );
		pStaticPic6->GetString( 0 )->msg = GET_REPLACED_STRING( ( STR_ID_30416, "L", wstrTime ) );
	}

	// 계정 총 접속 일 수
	CKTDGUIStatic* pStaticPic7 = static_cast<CKTDGUIStatic*>( m_p4thYearEventDLG->GetControl( L"pic7" ) );
	if( pStaticPic7 != NULL )
	{
		std::wstringstream wstrstm;
		wstrstm << k4ThAnnivEventInfo.m_iDayTotalConnect;
		pStaticPic7->GetString( 0 )->msg = GET_REPLACED_STRING( ( STR_ID_30422, "L", wstrstm.str() ) );
	}

	// 첫 캐릭터 삭제일
	CKTDGUIStatic* pStaticPic8 = static_cast<CKTDGUIStatic*>( m_p4thYearEventDLG->GetControl( L"pic8" ) );
	if( pStaticPic8 != NULL )
	{
		std::wstring wstrTime( CTime( k4ThAnnivEventInfo.m_tTimeFirstDeleteChar ).Format( _T( "%Y-%m-%d %H:%M:%S" ) ) );
		pStaticPic8->GetString( 0 )->msg = GET_REPLACED_STRING( ( STR_ID_30417, "L", wstrTime ) );
	}

	// 총 던전 클리어 횟수
	CKTDGUIStatic* pStaticPic9 = static_cast<CKTDGUIStatic*>( m_p4thYearEventDLG->GetControl( L"pic9" ) );
	if( pStaticPic9 != NULL )
	{
		std::wstringstream wstrstm;
		wstrstm << k4ThAnnivEventInfo.m_iCountDungeonClear;
		pStaticPic9->GetString( 0 )->msg = GET_REPLACED_STRING( ( STR_ID_30425, "L", wstrstm.str() ) );
	}

	// 우편 받은 횟수
	CKTDGUIStatic* pStaticPic10 = static_cast<CKTDGUIStatic*>( m_p4thYearEventDLG->GetControl( L"pic10" ) );
	if( pStaticPic10 != NULL )
	{
		std::wstringstream wstrstm;
		wstrstm << k4ThAnnivEventInfo.m_iCountReceivedPost;
		pStaticPic10->GetString( 0 )->msg = GET_REPLACED_STRING( ( STR_ID_30421, "L", wstrstm.str() ) );
	}

	// 대전 총 패배 횟수
	CKTDGUIStatic* pStaticPic11 = static_cast<CKTDGUIStatic*>( m_p4thYearEventDLG->GetControl( L"pic11" ) );
	if( pStaticPic11 != NULL )
	{
		std::wstringstream wstrstm;
		wstrstm << k4ThAnnivEventInfo.m_iCountPvpLose;
		pStaticPic11->GetString( 0 )->msg = GET_REPLACED_STRING( ( STR_ID_30423, "L", wstrstm.str() ) );
	}

	// 첫 구매 아이템
	CKTDGUIStatic* pStaticPic12 = static_cast<CKTDGUIStatic*>( m_p4thYearEventDLG->GetControl( L"pic12" ) );
	if( pStaticPic12 != NULL )
	{
		const CX2Item::ItemTemplet* pItemTempet = g_pData->GetItemManager()->GetItemTemplet( k4ThAnnivEventInfo.m_iItemIDFirstBuy );

		if( pItemTempet != NULL )
			pStaticPic12->GetString( 0 )->msg = GET_REPLACED_STRING( ( STR_ID_30418, "L", pItemTempet->GetName() ) );
		else
			pStaticPic12->GetString( 0 )->msg = GET_REPLACED_STRING( ( STR_ID_30418, "L", std::wstring( GET_STRING( STR_ID_30428 ) ) ) );
	}
}
#endif SERV_4TH_ANNIVERSARY_EVENT
