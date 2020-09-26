#include "dxstdafx.h"
#include ".\x2dungeontool.h"
#include <math.h>

KX2DungeonTool* g_pDungeonTool;

typedef CX2DungeonSubStage::ClearCondtionData::MonsterCount MonsterCount;

struct MC_FIND_IF
{
	MC_FIND_IF( int iKey ) : m_iKey( iKey ) {}
	bool operator()( MonsterCount& monsterCount )
	{
		if( monsterCount.m_iKey == m_iKey ) return true;
		return false;
	}

private:
	int m_iKey;
};

void CALLBACK TimerBackUpScript()
{
	if( g_pDungeonTool->IsDungeonExist() == false )
		return;

	if( g_pDungeonTool->SaveDungeonLuaFile( g_pDungeonTool->GetAutoSaveFileName().c_str() ) == false )
	{
		MessageBox( g_pKTDXApp->GetHWND(), L"Auto Save Failed!", L"test", MB_OK );
	}
}

void CALLBACK OnMenuUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{	
	KX2DungeonTool* pDungeonTool = (KX2DungeonTool*) pUserContext;
	pDungeonTool->OnMenuUIEventCallBack( nEvent, nControlID, pControl );
}

void KX2DungeonTool::OnMenuUIEventCallBack(  UINT nEvent, int nControlID, CDXUTControl* pControl )
{
	std::wstring temp;
	WCHAR buf[128] = L"";
	OPENFILENAME ofn;        // common dialog box structure
	WCHAR wszFileName[512];  // path까지 포함한 파일 이름

	switch( nControlID )
	{
	case KX2DungeonTool::IDC_GO_TO_MAIN_BUTTON:
		{
			if( m_mainUI.GetVisible() )
			{
				if( m_pDungeonMapUI != NULL )
					m_pDungeonMapUI->SetShow( false );

				m_mainUI.SetVisible( false );
				m_mapUI.SetVisible( false );
				m_summaryUI.SetVisible( false );
				m_toolUI.SetVisible( true );
				m_LeftToolUI.SetVisible( true );				
				SetVisibleCheckBox( true );
				m_menuUI.GetCheckBox( IDC_TOOL_UI_SHOW_CHECK )->SetChecked( false );
				m_menuUI.GetCheckBox( IDC_MAP_UI_SHOW_CHECK )->SetChecked( false );
				m_menuUI.GetCheckBox( IDC_SUMMARY_UI_CHECK )->SetChecked( false );
			}
			else
			{
				if( m_pDungeonMapUI != NULL )
					m_pDungeonMapUI->SetShow( false );

                m_mapUI.SetVisible( false );
				m_summaryUI.SetVisible( false );
				m_toolUI.SetVisible( false );
				m_LeftToolUI.SetVisible( false );
				m_mainUI.SetVisible( true );
				SetVisibleCheckBox( false );
				m_menuUI.GetCheckBox( IDC_TOOL_UI_SHOW_CHECK )->SetChecked( false );
				m_menuUI.GetCheckBox( IDC_MAP_UI_SHOW_CHECK )->SetChecked( false );
				m_menuUI.GetCheckBox( IDC_SUMMARY_UI_CHECK )->SetChecked( false );
			}			
		}
		break;

	case KX2DungeonTool::IDC_LOAD_LUA_BUTTON:
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = g_pKTDXApp->GetHWND(); 
		ofn.lpstrFile = (LPWSTR)wszFileName;
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = sizeof(wszFileName);
		ofn.lpstrFilter = L"dungeon\0*.lua\0";
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
		if( GetOpenFileName(&ofn) == TRUE ) 
		{
			if( !LoadDungeonFromFile( ofn.lpstrFile ) )
			{
				MessageBox( g_pKTDXApp->GetHWND(), L"던전 로드 실패!", L"test", MB_OK );
			}
		}
		break;

	case KX2DungeonTool::IDC_SAVE_LUA_BUTTON:
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = g_pKTDXApp->GetHWND(); 
		ofn.lpstrFile = (LPWSTR)wszFileName;
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = sizeof(wszFileName);
		ofn.lpstrFilter = L"dungeon\0*.lua\0";
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
		if( GetSaveFileName(&ofn) == TRUE ) 
		{
			if( SaveDungeonLuaFile( ofn.lpstrFile ) )
			{
				wsprintf( buf, L"[%s] 스크립트 파일 저장 성공!", ofn.lpstrFile );
				MessageBox( g_pKTDXApp->GetHWND(), buf, L"test", MB_OK );
			}
			else
			{
				MessageBox( g_pKTDXApp->GetHWND(), L"스크립트 파일 저장 실패!", L"test", MB_OK );
			}
		}
		break;

	case KX2DungeonTool::IDC_STAGE_INFO_VISIBLE_CHECK:
		{
			if( m_menuUI.GetCheckBox( IDC_STAGE_INFO_VISIBLE_CHECK )->GetChecked() )			
				SetVisibleStageInfo( true );
			else
				SetVisibleStageInfo( false );
		}
		break;
		
	case KX2DungeonTool::IDC_CURTAIN_VISIBLE_CHECK:
		{
			if( m_menuUI.GetCheckBox( IDC_CURTAIN_VISIBLE_CHECK )->GetChecked() )
				SetVisibleCurtain( true );
			else
				SetVisibleCurtain( false );
		}
		break;

	case KX2DungeonTool::IDC_NPC_VISIBLE_CHECK:
		{
			if( m_menuUI.GetCheckBox( IDC_NPC_VISIBLE_CHECK )->GetChecked() )
				SetVisibleNPC( true );
			else
				SetVisibleNPC( false );
		}
		break;

	case KX2DungeonTool::IDC_TRIGGER_VISIBLE_CHECK:
		{
			if( m_menuUI.GetCheckBox( IDC_TRIGGER_VISIBLE_CHECK )->GetChecked() )
				SetVisibleTrigger( true );
			else
				SetVisibleTrigger( false );

		}
		break;
			
	case KX2DungeonTool::IDC_SUBSTAGE_VISIBLE_CHECK:
		{
			if( m_menuUI.GetCheckBox( IDC_SUBSTAGE_VISIBLE_CHECK )->GetChecked() )
				SetVisibleSubStageInfo( true );
			else
				SetVisibleSubStageInfo( false );
		}
		break;

	case KX2DungeonTool::IDC_CLEAR_CONDITION_VISIBLE_CHECK:
		{
			if( m_menuUI.GetCheckBox( IDC_CLEAR_CONDITION_VISIBLE_CHECK )->GetChecked() )
				SetVisibleClearCondition( true );
			else
				SetVisibleClearCondition( false );
		}
		break;

	case KX2DungeonTool::IDC_TOOL_UI_SHOW_CHECK:
		{
			if( m_menuUI.GetCheckBox( IDC_TOOL_UI_SHOW_CHECK )->GetChecked() )
			{
				if( m_pDungeonMapUI != NULL )
					m_pDungeonMapUI->SetShow( false );

				m_mainUI.SetVisible( false );
				m_mapUI.SetVisible( false );
				m_summaryUI.SetVisible( false );
				m_toolUI.SetVisible( true );
				m_LeftToolUI.SetVisible( true );
				m_menuUI.GetCheckBox( IDC_MAP_UI_SHOW_CHECK )->SetChecked( false );
				m_menuUI.GetCheckBox( IDC_SUMMARY_UI_CHECK )->SetChecked( false );
			}
			else
			{
				if( m_pDungeonMapUI != NULL )
					m_pDungeonMapUI->SetShow( false );

				m_mainUI.SetVisible( true );
				m_mapUI.SetVisible( false );
				m_summaryUI.SetVisible( false );
				m_toolUI.SetVisible( false );
				m_LeftToolUI.SetVisible( false );				
				m_menuUI.GetCheckBox( IDC_MAP_UI_SHOW_CHECK )->SetChecked( false );
				m_menuUI.GetCheckBox( IDC_SUMMARY_UI_CHECK )->SetChecked( false );
			}
		}
		break;

	case KX2DungeonTool::IDC_MAP_UI_SHOW_CHECK:
		{
			if( m_menuUI.GetCheckBox( IDC_MAP_UI_SHOW_CHECK )->GetChecked() )
			{
				if( m_pDungeonMapUI != NULL )
					m_pDungeonMapUI->SetShow( true );
				
				m_mainUI.SetVisible( false );
				m_mapUI.SetVisible( true );
				m_summaryUI.SetVisible( false );
				m_toolUI.SetVisible( false );
				m_LeftToolUI.SetVisible( false );
				m_menuUI.GetCheckBox( IDC_TOOL_UI_SHOW_CHECK )->SetChecked( false );
				m_menuUI.GetCheckBox( IDC_SUMMARY_UI_CHECK )->SetChecked( false );
			}
			else
			{
				if( m_pDungeonMapUI != NULL )
					m_pDungeonMapUI->SetShow( false );
				
				m_mainUI.SetVisible( true );				
				m_mapUI.SetVisible( false );
				m_summaryUI.SetVisible( false );
				m_toolUI.SetVisible( false );
				m_LeftToolUI.SetVisible( false );				
				m_menuUI.GetCheckBox( IDC_TOOL_UI_SHOW_CHECK )->SetChecked( false );
				m_menuUI.GetCheckBox( IDC_SUMMARY_UI_CHECK )->SetChecked( false );
			}
		}
		break;

	case KX2DungeonTool::IDC_SUMMARY_UI_CHECK:
		{
			if( m_menuUI.GetCheckBox( IDC_SUMMARY_UI_CHECK )->GetChecked() )
			{
				if( m_pDungeonMapUI != NULL )
					m_pDungeonMapUI->SetShow( false );

				m_mainUI.SetVisible( false );
				m_mapUI.SetVisible( false );
				m_summaryUI.SetVisible( true );
				m_toolUI.SetVisible( false );
				m_LeftToolUI.SetVisible( false );				
				m_menuUI.GetCheckBox( IDC_TOOL_UI_SHOW_CHECK )->SetChecked( false );
				m_menuUI.GetCheckBox( IDC_MAP_UI_SHOW_CHECK )->SetChecked( false );

				// Load Summary Info
				LoadSummaryInfo();
			}
			else
			{
				if( m_pDungeonMapUI != NULL )
					m_pDungeonMapUI->SetShow( false );

				m_mainUI.SetVisible( true );
				m_mapUI.SetVisible( false );
				m_summaryUI.SetVisible( false );
				m_toolUI.SetVisible( false );
				m_LeftToolUI.SetVisible( false );
				m_menuUI.GetCheckBox( IDC_TOOL_UI_SHOW_CHECK )->SetChecked( false );
				m_menuUI.GetCheckBox( IDC_MAP_UI_SHOW_CHECK )->SetChecked( false );
			}
		}
		break;

	case KX2DungeonTool::IDC_ONLY_SUBS_NPC_CHECK:
		switch( nEvent )
		{
		case EVENT_CHECKBOX_CHANGED:
			m_bOnlySubStageNPC = m_menuUI.GetCheckBox( IDC_ONLY_SUBS_NPC_CHECK )->GetChecked();
			break;
		}
		break;

	case KX2DungeonTool::IDC_WORLD_SHOW_CHECK:
		switch( nEvent )
		{
		case EVENT_CHECKBOX_CHANGED:
			if( m_pWorld != NULL )
				m_pWorld->SetRenderingWorld( m_menuUI.GetCheckBox( IDC_WORLD_SHOW_CHECK )->GetChecked() );
			break;
		}
		break;

	case KX2DungeonTool::IDC_LINE_MAP_SHOW_CHECK:
		switch( nEvent )
		{
		case EVENT_CHECKBOX_CHANGED:
			if( m_pWorld != NULL )
				m_pWorld->SetRenderingLineMap( m_menuUI.GetCheckBox( IDC_LINE_MAP_SHOW_CHECK )->GetChecked() );
			break;
		}
		break;

	case KX2DungeonTool::IDC_NPC_SHOW_CHECK:		
		{
		if( m_menuUI.GetCheckBox( IDC_NPC_SHOW_CHECK )->GetChecked() )
			m_bRenderingNPC = true;
		else
			m_bRenderingNPC = false;
		}
		break;

	case KX2DungeonTool::IDC_CURTAIN_SHOW_CHECK:
		{
		if( m_menuUI.GetCheckBox( IDC_CURTAIN_SHOW_CHECK )->GetChecked() )
			m_bRenderingCurtain = true;
		else
			m_bRenderingCurtain = false;
		}
		break;

	case KX2DungeonTool::IDC_START_POS_SHOW_CHECK:
		{
		if( m_menuUI.GetCheckBox( IDC_START_POS_SHOW_CHECK )->GetChecked() )
			m_bRenderingStartPos = true;
		else
            m_bRenderingStartPos = false;
		}
		break;

	case KX2DungeonTool::IDC_LINE_MAP_NUMBER_SHOW_CHECK:
		{
		if( m_menuUI.GetCheckBox( IDC_LINE_MAP_NUMBER_SHOW_CHECK )->GetChecked() )
			m_bRenderingLineMapNumber = true;
		else
			m_bRenderingLineMapNumber = false;
		}
		break;

	case KX2DungeonTool::IDC_AUTO_SAVE_CHECK:
		{
		if( m_menuUI.GetCheckBox( IDC_AUTO_SAVE_CHECK )->GetChecked() )
			::SetTimer( g_pKTDXApp->GetHWND(), 1, m_iAutoSaveTime, (TIMERPROC)TimerBackUpScript );
		else
			::KillTimer( g_pKTDXApp->GetHWND(), 1 );			
		}
		break;

	case KX2DungeonTool::IDC_NPC_COLOR_CHECK:
		{
			m_bNpcColor = m_menuUI.GetCheckBox( IDC_NPC_COLOR_CHECK )->GetChecked();
		}
		break;

	case KX2DungeonTool::IDC_NEAR_RANGE_CHECK:
		{
			m_bRenderingNearRange = m_menuUI.GetCheckBox( IDC_NEAR_RANGE_CHECK )->GetChecked();
		}
		break;

	case KX2DungeonTool::IDC_TARGET_RANGE_CHECK:
		{
			m_bRenderingTargetRange = m_menuUI.GetCheckBox( IDC_TARGET_RANGE_CHECK )->GetChecked();
		}
		break;

	case KX2DungeonTool::IDC_LOST_RANGE_CHECK:
		{
			m_bRenderingLostRange = m_menuUI.GetCheckBox( IDC_LOST_RANGE_CHECK )->GetChecked();
		}
		break;

	case KX2DungeonTool::IDC_ONLY_SELECT_CHECK:
		{
			m_bOnlySelect = m_menuUI.GetCheckBox( IDC_ONLY_SELECT_CHECK )->GetChecked();
		}
		break;		

	default:
		break;
	}
}

void CALLBACK OnMainUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
	KX2DungeonTool* pDungeonTool = (KX2DungeonTool*) pUserContext;
	pDungeonTool->OnMainUIEventCallback( nEvent, nControlID, pControl );
}

void KX2DungeonTool::OnMainUIEventCallback(  UINT nEvent, int nControlID, CDXUTControl* pControl )
{
	std::wstring temp;
	std::string strTemp;
	WCHAR buf[128] = L"";
	CDXUTListBox* pListBox;
	DXUTComboBoxItem* pComboBoxItem;

	switch( nControlID )
	{
	case KX2DungeonTool::IDC_DUNGEON_LIST:
		{
			switch( nEvent )
			{
			case EVENT_LISTBOX_ITEM_DBLCLK:
				{
					temp = ((DXUTListBoxItem*)((CDXUTListBox *)pControl)->GetSelectedItem())->strText;
					DungeonListIterator mit = m_mapDungeonList.find( temp );
					if( !LoadDungeon( mit->second ) )
					{						
						MessageBox( g_pKTDXApp->GetHWND(), L"스테이지 리스트 로드 실패!", L"test", MB_OK );
					}
				} break;
			}
		} break;

	case KX2DungeonTool::IDC_DUNGEON_LOAD_BUTTON:
		if( m_pDungeonListBox != NULL )
		{
			temp = ((DXUTListBoxItem*)(m_pDungeonListBox->GetSelectedItem()))->strText;
			DungeonListIterator mit = m_mapDungeonList.find( temp );
			if( mit == m_mapDungeonList.end() )
			{
				MessageBox( g_pKTDXApp->GetHWND(), L"불러올 던전을 선택하세요!", L"test", MB_OK );
				break;
			}
			if( !LoadDungeon( mit->second ) )
			{
				MessageBox( g_pKTDXApp->GetHWND(), L"던전 로드 실패!", L"test", MB_OK );
			}
		}
		break;

	case KX2DungeonTool::IDC_STAGE_LIST:
		{
			switch( nEvent )
			{
			case EVENT_LISTBOX_ITEM_DBLCLK:
				{
					if( !LoadStageInfo( m_mainUI.GetListBox( IDC_STAGE_LIST )->GetSelectedIndex() ) )
					{
						MessageBox( g_pKTDXApp->GetHWND(), L"스테이지 정보 로드 실패!", L"test", MB_OK );						
					}
					else
					{	
						m_mainUI.SetVisible( false );
						m_toolUI.SetVisible( true );
						m_LeftToolUI.SetVisible( true );
						SetVisibleCheckBox( true );
					}
				} break;
			}
		} break;

	case KX2DungeonTool::IDC_VIEW_ALL_WORLD_BUTTON:
		MessageBox( g_pKTDXApp->GetHWND(), L"기능 미구현", L"notice", MB_OK );
		break;

	case KX2DungeonTool::IDC_ADD_STAGE_BUTTON:
		IS_NULL_POINTER( m_pEditingDungeon );
		pComboBoxItem = m_mainUI.GetComboBox( IDC_WORLD_ID_COMBO )->GetSelectedItem();
		if( pComboBoxItem != NULL )
		{
			CDXUTListBox* pListBox = m_mainUI.GetListBox( IDC_STAGE_LIST );

			// 실제 데이터에 추가
			CX2DungeonStage::StageData* pNewStage = new CX2DungeonStage::StageData();
			pNewStage->m_WorldID	  = (CX2World::WORLD_ID)((int)pComboBoxItem->pData);
			pNewStage->m_bStartMotion = false;
			pNewStage->m_pWorld		  = NULL;            
			pNewStage->m_WorldColor	  = m_pEditingDungeon->GetWorldColor();
			pNewStage->m_UnitColor	  = m_pEditingDungeon->GetUnitColor();
			m_pEditingDungeon->GetStageDataList().push_back( pNewStage );
			
			// 던전 다시 로드
			LoadDungeon( m_pEditingDungeon );
		}
		break;

	case KX2DungeonTool::IDC_DEL_STAGE_BUTTON:
		pListBox = m_mainUI.GetListBox( IDC_STAGE_LIST );
		if( pListBox->GetSelectedIndex() != -1 )
		{
			int delNum = pListBox->GetSelectedIndex();
			
			CX2DungeonStage::StageData* pStage = m_pEditingDungeon->GetStageDataList()[delNum];
			vector<CX2DungeonStage::StageData*>::iterator mit = std::find(
				m_pEditingDungeon->GetStageDataList().begin(), 
				m_pEditingDungeon->GetStageDataList().end(), 
				pStage 
				);
			if( mit == m_pEditingDungeon->GetStageDataList().end() )
				break;
			m_pEditingDungeon->GetStageDataList().erase( mit );
			// 현재 작업중인 스테이지를 가진 던전을 지운다면 NULL포인터 처리
			if( m_pEditingStage == pStage )
				m_pEditingStage = NULL;

			SAFE_DELETE( pStage );

			// 던전 다시 로드
			LoadDungeon( m_pEditingDungeon );
		}
		break;

	case KX2DungeonTool::IDC_DUNGEON_WORLD_COLOR_R:
		switch( nEvent )
		{
		case EVENT_EDITBOX_CHANGE:
			IS_NULL_POINTER( m_pEditingDungeon );
			VERIFY_RGB_VALUE( m_mainUI.GetEditBox( IDC_DUNGEON_WORLD_COLOR_R )->GetText() );
			D3DXCOLOR worldColor = m_pEditingDungeon->GetWorldColor();
			EditingEditBoxNonBackup( &m_mainUI, IDC_DUNGEON_WORLD_COLOR_R, worldColor.r );
			m_pEditingDungeon->SetWorldColor( worldColor );
			break;
		}
		break;

	case KX2DungeonTool::IDC_DUNGEON_WORLD_COLOR_G:
		switch( nEvent )
		{
		case EVENT_EDITBOX_CHANGE:
			IS_NULL_POINTER( m_pEditingDungeon );
			VERIFY_RGB_VALUE( m_mainUI.GetEditBox( IDC_DUNGEON_WORLD_COLOR_G )->GetText() );
			D3DXCOLOR worldColor = m_pEditingDungeon->GetWorldColor();
			EditingEditBoxNonBackup( &m_mainUI, IDC_DUNGEON_WORLD_COLOR_G, worldColor.g );
			m_pEditingDungeon->SetWorldColor( worldColor );
			break;
		}
		break;

	case KX2DungeonTool::IDC_DUNGEON_WORLD_COLOR_B:
		switch( nEvent )
		{
		case EVENT_EDITBOX_CHANGE:
			IS_NULL_POINTER( m_pEditingDungeon );
			VERIFY_RGB_VALUE( m_mainUI.GetEditBox( IDC_DUNGEON_WORLD_COLOR_B )->GetText() );
			D3DXCOLOR worldColor = m_pEditingDungeon->GetWorldColor();
			EditingEditBoxNonBackup( &m_mainUI, IDC_DUNGEON_WORLD_COLOR_B, worldColor.b );
			m_pEditingDungeon->SetWorldColor( worldColor );
			break;
		}
		break;

	case KX2DungeonTool::IDC_DUNGEON_UNIT_COLOR_R:
		switch( nEvent )
		{
		case EVENT_EDITBOX_CHANGE:
			IS_NULL_POINTER( m_pEditingDungeon );
			VERIFY_RGB_VALUE( m_mainUI.GetEditBox( IDC_DUNGEON_UNIT_COLOR_R )->GetText() );
			D3DXCOLOR unitColor = m_pEditingDungeon->GetUnitColor();
			EditingEditBoxNonBackup( &m_mainUI, IDC_DUNGEON_UNIT_COLOR_R, unitColor.r );
			m_pEditingDungeon->SetUnitColor( unitColor );
			break;
		}
		break;

	case KX2DungeonTool::IDC_DUNGEON_UNIT_COLOR_G:
		switch( nEvent )
		{
		case EVENT_EDITBOX_CHANGE:
			IS_NULL_POINTER( m_pEditingDungeon );
			VERIFY_RGB_VALUE( m_mainUI.GetEditBox( IDC_DUNGEON_UNIT_COLOR_G )->GetText() );
			D3DXCOLOR unitColor = m_pEditingDungeon->GetUnitColor();
			EditingEditBoxNonBackup( &m_mainUI, IDC_DUNGEON_UNIT_COLOR_G, unitColor.g );
			m_pEditingDungeon->SetUnitColor( unitColor );
			break;
		}
		break;

	case KX2DungeonTool::IDC_DUNGEON_UNIT_COLOR_B:
		switch( nEvent )
		{
		case EVENT_EDITBOX_CHANGE:
			IS_NULL_POINTER( m_pEditingDungeon );
			VERIFY_RGB_VALUE( m_mainUI.GetEditBox( IDC_DUNGEON_UNIT_COLOR_B )->GetText() );
			D3DXCOLOR unitColor = m_pEditingDungeon->GetUnitColor();
			EditingEditBoxNonBackup( &m_mainUI, IDC_DUNGEON_UNIT_COLOR_B, unitColor.b );
			m_pEditingDungeon->SetUnitColor( unitColor );
			break;
		}
		break;
	
	default:
		break;
	}
}

void CALLBACK OnToolUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
	KX2DungeonTool* pDungeonTool = (KX2DungeonTool*) pUserContext;
	pDungeonTool->OnToolUIEventCallback( nEvent, nControlID, pControl );
}

void KX2DungeonTool::OnToolUIEventCallback(  UINT nEvent, int nControlID, CDXUTControl* pControl )
{
	WCHAR buf[128] = L"";
	std::wstring temp;
	std::string temp2;

	switch( nControlID )
	{
		////////////////////////////////////////////////////////////////// Load List Info
	case KX2DungeonTool::IDC_SUB_STAGE_LIST:
		switch( nEvent )
		{
		case EVENT_LISTBOX_SELECTION:
			if( !LoadSubStageInfo( ((CDXUTListBox *)pControl)->GetSelectedIndex() ) )
				MessageBox( g_pKTDXApp->GetHWND(), L"서브 스테이지 정보 로드 실패!", L"test", MB_OK );

			START_LOG( clog, L"Sub Stage 리스트 박스 선택" )				
				<< BUILD_LOG( ((CDXUTListBox *)pControl)->m_nSelected )
				<< BUILD_LOG( ((CDXUTListBox *)pControl)->m_nSelStart )
				<< BUILD_LOG( ((CDXUTListBox *)pControl)->GetSize() );
			break;
		}
		break;

	case KX2DungeonTool::IDC_CURTAIN_LIST:
		switch( nEvent )
		{
		case EVENT_LISTBOX_SELECTION:
			if( !LoadCurtainInfo( ((CDXUTListBox *)pControl)->GetSelectedIndex() ) )
				MessageBox( g_pKTDXApp->GetHWND(), L"커튼 정보 로드 실패!", L"test", MB_OK );

			START_LOG( clog, L"Curtain 리스트 박스 선택" )
				<< BUILD_LOG( ((CDXUTListBox *)pControl)->m_nSelected )
				<< BUILD_LOG( ((CDXUTListBox *)pControl)->m_nSelStart )
				<< BUILD_LOG( ((CDXUTListBox *)pControl)->GetSize() );
			break;
		}		
		break;

	case KX2DungeonTool::IDC_TRIGGER_LIST:
		switch( nEvent )
		{
		case EVENT_LISTBOX_SELECTION:
			if( !LoadTriggerInfo( ((CDXUTListBox *)pControl)->GetSelectedIndex() ) )
				MessageBox( g_pKTDXApp->GetHWND(), L"트리거 정보 로드 실패!", L"test", MB_OK );

			START_LOG( clog, L"Trigger 리스트 박스 선택" )
				<< BUILD_LOG( ((CDXUTListBox *)pControl)->m_nSelected )
				<< BUILD_LOG( ((CDXUTListBox *)pControl)->m_nSelStart )
				<< BUILD_LOG( ((CDXUTListBox *)pControl)->GetSize() );
			break;
		}
		break;

	case KX2DungeonTool::IDC_NPC_LIST:
		switch( nEvent )
		{
		case EVENT_LISTBOX_SELECTION:
			int nIndex = ((CDXUTListBox *)pControl)->GetSelectedIndex();
			if( LoadNpcInfo( nIndex ) )
			{
				IS_NULL_POINTER( m_pEditingSubStage );
				CX2DungeonSubStage::NPCData* pNpcData = m_pEditingSubStage->m_NPCDataList[nIndex];
				m_vecPickedNPCStartPosKey.clear();
				for( UINT i=0 ; i < pNpcData->m_vecStartPos.size(); i++ )
					m_vecPickedNPCStartPosKey.push_back( SNPCStartPos( pNpcData, pNpcData->m_vecStartPos[i] ) );				
			}
			else
			{
				MessageBox( g_pKTDXApp->GetHWND(), L"NPC 정보 로드 실패!", L"test", MB_OK );				
			}

			START_LOG( clog, L"Npc 리스트 박스 선택" )
				<< BUILD_LOG( ((CDXUTListBox *)pControl)->m_nSelected )
				<< BUILD_LOG( ((CDXUTListBox *)pControl)->m_nSelStart )
				<< BUILD_LOG( ((CDXUTListBox *)pControl)->GetSize() );
			break;
		}
		break;

		//////////////////////////////////////////////////////////////// Stage Info Editing
	case KX2DungeonTool::IDC_START_MOTION_CHECK:
		switch( nEvent )
		{
		case EVENT_CHECKBOX_CHANGED:
			IS_NULL_POINTER( m_pEditingStage );
			EditingCheckBox( IDC_START_MOTION_CHECK, m_pEditingStage->m_bStartMotion );
			break;
		}
		break;

	case KX2DungeonTool::IDC_ADD_SUB_STAGE_BUTTON:
		{
			IS_NULL_POINTER( m_pEditingStage );
			CX2DungeonSubStage::SubStageData* pNewSubStage = new CX2DungeonSubStage::SubStageData();
			pNewSubStage->m_GoType		 =  CX2DungeonSubStage::GT_NONE;
			pNewSubStage->m_StartLineSet = 0;
			pNewSubStage->m_MainLineSet	 = 0;
			pNewSubStage->m_EndLineSet	 = 0;
			m_pEditingStage->m_SubStageDataList.push_back( pNewSubStage );
			LoadStageInfo( m_pEditingStage );
			LoadSubStageInfo( pNewSubStage );
			m_toolUI.GetListBox( IDC_SUB_STAGE_LIST )->LastItemSelect();

			START_LOG( clog, L"Sub Stage 추가" );
		}
		break;

	case KX2DungeonTool::IDC_DEL_SUB_STAGE_BUTTON:
		{
			IS_NULL_POINTER( m_pEditingStage );
			int delNum = m_toolUI.GetListBox( IDC_SUB_STAGE_LIST )->GetSelectedIndex();
			if( delNum == -1 )
				break;
			CX2DungeonSubStage::SubStageData* pSubStage = m_pEditingStage->m_SubStageDataList[delNum];
			std::vector<CX2DungeonSubStage::SubStageData*>::iterator mit = std::find(
				m_pEditingStage->m_SubStageDataList.begin(),
				m_pEditingStage->m_SubStageDataList.end(),
				pSubStage
				);
			if( mit == m_pEditingStage->m_SubStageDataList.end() )
				break;
			m_pEditingStage->m_SubStageDataList.erase( mit );
            SAFE_DELETE( pSubStage );
			m_iDelSubStage = delNum; // 맨 아랫쪽 인덱스의 데이터 삭제시 포커스 바꿔주기용 변수
			LoadStageInfo( m_pEditingStage );
			RemoveSubStageInfoToolUI();

			START_LOG( clog, L"Sub Stage 삭제" );
		}
		break;

	case KX2DungeonTool::IDC_ADD_READY_NPC_BUTTON:
		{
			IS_NULL_POINTER( m_pEditingStage );
			DXUTComboBoxItem* item = m_toolUI.GetComboBox( IDC_NPC_ID_LIST_COMBO )->GetSelectedItem();
			if( item == NULL )
				break;

			std::vector<CX2UnitManager::NPC_UNIT_ID>::iterator mit = std::find(
				m_pEditingStage->m_ReadyNPCList.begin(),
				m_pEditingStage->m_ReadyNPCList.end(),
				(CX2UnitManager::NPC_UNIT_ID)((int)item->pData)
				);
			if( mit != m_pEditingStage->m_ReadyNPCList.end() )
			{
				MessageBox( g_pKTDXApp->GetHWND(), L"이미 등록된 NPC입니다!", L"알림", MB_OK );
			}
			else
			{
				m_pEditingStage->m_ReadyNPCList.push_back( (CX2UnitManager::NPC_UNIT_ID)((int)item->pData) );
				LoadStageInfo( m_pEditingStage );
				LoadSubStageInfo( m_pEditingSubStage );
				m_toolUI.GetListBox( IDC_READY_NPC_LIST )->LastItemSelect();
			}

			START_LOG( clog, L"Ready NPC 추가" );
		}
		break;

	case KX2DungeonTool::IDC_DEL_READY_NPC_BUTTON:
		{
			IS_NULL_POINTER( m_pEditingStage );
			int delNum = m_toolUI.GetListBox( IDC_READY_NPC_LIST )->GetSelectedIndex();
			if( delNum == -1 )
				break;
			std::vector<CX2UnitManager::NPC_UNIT_ID>::iterator mit = std::find(
				m_pEditingStage->m_ReadyNPCList.begin(),
				m_pEditingStage->m_ReadyNPCList.end(),
				m_pEditingStage->m_ReadyNPCList[delNum]
				);
			if( mit == m_pEditingStage->m_ReadyNPCList.end() )
				break;
			m_pEditingStage->m_ReadyNPCList.erase( mit );
			m_iDelReadyNpc = delNum; // 맨 아랫쪽 인덱스의 데이터 삭제시 포커스 바꿔주기용 변수
			LoadStageInfo( m_pEditingStage );
			LoadSubStageInfo( m_pEditingSubStage );

			START_LOG( clog, L"Ready NPC 삭제" );
		}
		break;

	case KX2DungeonTool::IDC_WORLD_COLOR_R:
		switch( nEvent )
		{
		case EVENT_EDITBOX_CHANGE:
			IS_NULL_POINTER( m_pEditingStage );
			VERIFY_RGB_VALUE( m_toolUI.GetEditBox( IDC_WORLD_COLOR_R )->GetText() );
			EditingEditBoxNonBackup( &m_toolUI, IDC_WORLD_COLOR_R, m_pEditingStage->m_WorldColor.r );
			break;
		}
		break;

	case KX2DungeonTool::IDC_WORLD_COLOR_G:
		switch( nEvent )
		{
		case EVENT_EDITBOX_CHANGE:
			IS_NULL_POINTER( m_pEditingStage );
			VERIFY_RGB_VALUE( m_toolUI.GetEditBox( IDC_WORLD_COLOR_G )->GetText() );
			EditingEditBoxNonBackup( &m_toolUI, IDC_WORLD_COLOR_G, m_pEditingStage->m_WorldColor.g );
			break;
		}
		break;

	case KX2DungeonTool::IDC_WORLD_COLOR_B:
		switch( nEvent )
		{
		case EVENT_EDITBOX_CHANGE:
			IS_NULL_POINTER( m_pEditingStage );
			VERIFY_RGB_VALUE( m_toolUI.GetEditBox( IDC_WORLD_COLOR_B )->GetText() );
			EditingEditBoxNonBackup( &m_toolUI, IDC_WORLD_COLOR_B, m_pEditingStage->m_WorldColor.b );
			break;
		}
		break;

	case KX2DungeonTool::IDC_UNIT_COLOR_R:
		switch( nEvent )
		{
		case EVENT_EDITBOX_CHANGE:
			IS_NULL_POINTER( m_pEditingStage );
			VERIFY_RGB_VALUE( m_toolUI.GetEditBox( IDC_UNIT_COLOR_R )->GetText() );
			EditingEditBoxNonBackup( &m_toolUI, IDC_UNIT_COLOR_R, m_pEditingStage->m_UnitColor.r );
			break;
		}
		break;

	case KX2DungeonTool::IDC_UNIT_COLOR_G:
		switch( nEvent )
		{
		case EVENT_EDITBOX_CHANGE:
			IS_NULL_POINTER( m_pEditingStage );
			VERIFY_RGB_VALUE( m_toolUI.GetEditBox( IDC_UNIT_COLOR_G )->GetText() );
			EditingEditBoxNonBackup( &m_toolUI, IDC_UNIT_COLOR_G, m_pEditingStage->m_UnitColor.g );
			break;
		}
		break;

	case KX2DungeonTool::IDC_UNIT_COLOR_B:
		switch( nEvent )
		{
		case EVENT_EDITBOX_CHANGE:
			IS_NULL_POINTER( m_pEditingStage );
			VERIFY_RGB_VALUE( m_toolUI.GetEditBox( IDC_UNIT_COLOR_B )->GetText() );
			EditingEditBoxNonBackup( &m_toolUI, IDC_UNIT_COLOR_B, m_pEditingStage->m_UnitColor.b );
			break;
		}
		break;

		//////////////////////////////////////////////////////////////// SubStage Info Editing
	case KX2DungeonTool::IDC_START_LINE_SET_EDIT:
		switch( nEvent )
		{
		case EVENT_EDITBOX_CHANGE:
			IS_NULL_POINTER( m_pEditingSubStage );
			EditingEditBox( IDC_START_LINE_SET_EDIT, m_pEditingSubStage->m_StartLineSet, m_backupSubStage.m_StartLineSet );
			break;
		}
		break;

	case KX2DungeonTool::IDC_MAIN_LINE_SET_EDIT:
		switch( nEvent )
		{
		case EVENT_EDITBOX_CHANGE:
			IS_NULL_POINTER( m_pEditingSubStage );
			EditingEditBox( IDC_MAIN_LINE_SET_EDIT, m_pEditingSubStage->m_MainLineSet, m_backupSubStage.m_MainLineSet );
			break;
		}
		break;

	case KX2DungeonTool::IDC_END_LINE_SET_EDIT:
		switch( nEvent )
		{
		case EVENT_EDITBOX_CHANGE:
			IS_NULL_POINTER( m_pEditingSubStage );
			EditingEditBox( IDC_END_LINE_SET_EDIT, m_pEditingSubStage->m_EndLineSet, m_backupSubStage.m_EndLineSet );
			break;
		}
		break;		

	case KX2DungeonTool::IDC_GO_TYPE_COMBO:
		switch( nEvent )
		{
			case EVENT_COMBOBOX_SELECTION_CHANGED:
			IS_NULL_POINTER( m_pEditingSubStage );
			EditingComboBox( IDC_GO_TYPE_COMBO, m_pEditingSubStage->m_GoType );
			break;
		}		
		break;

		//////////////////////////////////////////////////////////////// Curtain Editing
	case KX2DungeonTool::IDC_ADD_CURTAIN_BUTTON:
		{
			IS_NULL_POINTER( m_pEditingSubStage );
			CX2DungeonSubStage::CurtainData* pNewCurtain = new CX2DungeonSubStage::CurtainData();
			pNewCurtain->m_bAtStart	 = false;
			pNewCurtain->m_bLookLeft = false;
			pNewCurtain->m_LineIndex = 0;
			m_pEditingSubStage->m_CurtainDataList.push_back( pNewCurtain );
			LoadSubStageInfo( m_pEditingSubStage, false );
			LoadCurtainInfo( pNewCurtain );
			m_toolUI.GetListBox( IDC_CURTAIN_LIST )->LastItemSelect();

			START_LOG( clog, L"Curtain 추가" );
		}
		break;

	case KX2DungeonTool::IDC_DEL_CURTAIN_BUTTON:
		{
			IS_NULL_POINTER( m_pEditingSubStage );
			int delNum = m_toolUI.GetListBox( IDC_CURTAIN_LIST )->GetSelectedIndex();
			if( delNum == -1 )
				break;
			CX2DungeonSubStage::CurtainData* pCurtain = m_pEditingSubStage->m_CurtainDataList[delNum];
			std::vector<CX2DungeonSubStage::CurtainData*>::iterator mit = std::find(
				m_pEditingSubStage->m_CurtainDataList.begin(),
				m_pEditingSubStage->m_CurtainDataList.end(),
				pCurtain
				);
			if( mit == m_pEditingSubStage->m_CurtainDataList.end() )
				break;
			m_pEditingSubStage->m_CurtainDataList.erase( mit );
			m_iDelCurtain = delNum; // 맨 아랫쪽 인덱스의 데이터 삭제시 포커스 바꿔주기용 변수
			SAFE_DELETE( pCurtain );
			LoadSubStageInfo( m_pEditingSubStage, false );
			RemoveCurtainToolUI();

			START_LOG( clog, L"Curtain 삭제" );
		}
		break;

	case KX2DungeonTool::IDC_LINE_INDEX_EDIT:
		switch( nEvent )
		{
		case EVENT_EDITBOX_CHANGE:
			IS_NULL_POINTER( m_pEditingCurtain );
			EditingEditBox( IDC_LINE_INDEX_EDIT, m_pEditingCurtain->m_LineIndex, m_backupCurtain.m_LineIndex );
			break;
		}
		break;

	case KX2DungeonTool::IDC_AT_START_CHECK:
		switch( nEvent )
		{
		case EVENT_CHECKBOX_CHANGED:
			IS_NULL_POINTER( m_pEditingCurtain );
			EditingCheckBox( IDC_AT_START_CHECK, m_pEditingCurtain->m_bAtStart );
			break;
		}
		break;

	case KX2DungeonTool::IDC_LOOK_LEFT_CHECK:
		switch( nEvent )
		{
		case EVENT_CHECKBOX_CHANGED:
			IS_NULL_POINTER( m_pEditingCurtain );
			EditingCheckBox( IDC_LOOK_LEFT_CHECK, m_pEditingCurtain->m_bLookLeft );
			break;
		}
		break;

		//////////////////////////////////////////////////////////////// NPC Editing
	case KX2DungeonTool::IDC_ADD_NPC_BUTTON:
		{
			IS_NULL_POINTER( m_pEditingSubStage );

			DXUTComboBoxItem* item = m_toolUI.GetComboBox( IDC_NPC_LIST_COMBO )->GetSelectedItem();
			if( item == NULL )
				break;

			CX2DungeonSubStage::NPCData* pNewNpc = new CX2DungeonSubStage::NPCData();
			pNewNpc->m_KeyCode		 = -1;
			pNewNpc->m_StartPos		 = 1;
			pNewNpc->m_Level		 = 0;
			pNewNpc->m_bActive		 = true;
			pNewNpc->m_bFocusCamera	 = false;
			pNewNpc->m_bHasBossGage	 = false;
			pNewNpc->m_bShowGage	 = true;
			pNewNpc->m_bSubNpc		 = false;
			pNewNpc->m_Rate			 = 100;
			pNewNpc->m_AddPos.y		 = 0.0f;
			pNewNpc->m_AddPos.z		 = 0.0f;
			pNewNpc->m_bSiegeMode	 = false;
			pNewNpc->m_UnitID		 = (CX2UnitManager::NPC_UNIT_ID)((int)item->pData);
			pNewNpc->m_vecStartPos.push_back( 1 );
			pNewNpc->m_bIsRight		 = false;
			
			m_pEditingSubStage->m_NPCDataList.push_back( pNewNpc );
			LoadSubStageInfo( m_pEditingSubStage, false );
			LoadNpcInfo( pNewNpc );
			LoadNPCMeshs(); // 최적화 필요
			m_toolUI.GetListBox( IDC_NPC_LIST )->LastItemSelect();

			START_LOG( clog, L"NPC 추가" );
		}
		break;

	case KX2DungeonTool::IDC_DEL_NPC_BUTTON:
		{
			IS_NULL_POINTER( m_pEditingSubStage );
			int delNum = m_toolUI.GetListBox( IDC_NPC_LIST )->GetSelectedIndex();
			if( delNum == -1 )
				break;
			CX2DungeonSubStage::NPCData* pNpc = m_pEditingSubStage->m_NPCDataList[delNum];
			std::vector<CX2DungeonSubStage::NPCData*>::iterator mit = std::find(
				m_pEditingSubStage->m_NPCDataList.begin(),
				m_pEditingSubStage->m_NPCDataList.end(),
				pNpc
				);
			if( mit == m_pEditingSubStage->m_NPCDataList.end() )
				break;
			m_pEditingSubStage->m_NPCDataList.erase( mit );
			m_iDelNpc = delNum; // 맨 아랫쪽 인덱스의 데이터 삭제시 포커스 바꿔주기용 변수
			SAFE_DELETE( pNpc );
			LoadSubStageInfo( m_pEditingSubStage, false );
			RemoveNpcToolUI();
			LoadNPCMeshs(); // 최적화 필요

			START_LOG( clog, L"NPC 삭제" );
		}
		break;

	case KX2DungeonTool::IDC_NPC_ID_EDIT:
		switch( nEvent )
		{
		case EVENT_EDITBOX_CHANGE:
			IS_NULL_POINTER( m_pEditingNpc );
			EditingEditBox( IDC_NPC_ID_EDIT, m_pEditingNpc->m_UnitID, m_backupNpc.m_UnitID );
			break;
		}
		break;

	case KX2DungeonTool::IDC_START_POS1_EDIT:
		switch( nEvent )
		{
		case EVENT_EDITBOX_CHANGE:
			IS_NULL_POINTER( m_pEditingNpc );
			if( m_pEditingNpc->m_vecStartPos.size() >= 1 )
				EditingEditBox( IDC_START_POS1_EDIT, m_pEditingNpc->m_vecStartPos[0], m_backupNpc.m_vecStartPos[0] );
			break;
		}
		break;

	case KX2DungeonTool::IDC_START_POS2_EDIT:
		switch( nEvent )
		{
		case EVENT_EDITBOX_CHANGE:
			IS_NULL_POINTER( m_pEditingNpc );
			if( m_pEditingNpc->m_vecStartPos.size() == 1 )
			{
				EditingEditBoxInsert( IDC_START_POS2_EDIT, m_pEditingNpc->m_vecStartPos );
				LoadNPCMeshs(); // 최적화 필요
			}
			else if( m_pEditingNpc->m_vecStartPos.size() == 2 )
			{
				if( m_toolUI.GetEditBox( IDC_START_POS2_EDIT )->GetTextLength() == 0)
				{
					m_pEditingNpc->m_vecStartPos.pop_back();
					m_backupNpc.m_vecStartPos.pop_back();
				}
				else
				{
					EditingEditBox( IDC_START_POS2_EDIT, m_pEditingNpc->m_vecStartPos[1], m_backupNpc.m_vecStartPos[1] );
				}
			}
			break;
		}
		break;

	case KX2DungeonTool::IDC_ADD_POS_Y_EDIT:
		switch( nEvent )
		{
		case EVENT_EDITBOX_CHANGE:
			IS_NULL_POINTER( m_pEditingNpc );
			EditingEditBox( IDC_ADD_POS_Y_EDIT, m_pEditingNpc->m_AddPos.y, m_backupNpc.m_AddPos.y );
			break;
		}
		break;

	case KX2DungeonTool::IDC_LEVEL_EDIT:
		switch( nEvent )
		{
		case EVENT_EDITBOX_CHANGE:
			IS_NULL_POINTER( m_pEditingNpc );
			EditingEditBox( IDC_LEVEL_EDIT, m_pEditingNpc->m_Level, m_backupNpc.m_Level );			
			break;
		}
		break;

	case KX2DungeonTool::IDC_ACTIVE_CHECK:
		switch( nEvent )
		{
		case EVENT_CHECKBOX_CHANGED:
			IS_NULL_POINTER( m_pEditingNpc );
			EditingCheckBox( IDC_ACTIVE_CHECK, m_pEditingNpc->m_bActive );
			break;
		}
		break;

	case KX2DungeonTool::IDC_GAGE_BAR_CHECK:
		switch( nEvent )
		{
		case EVENT_CHECKBOX_CHANGED:
			IS_NULL_POINTER( m_pEditingNpc );
			EditingCheckBox( IDC_GAGE_BAR_CHECK, m_pEditingNpc->m_bShowGage );
			break;
		}
		break;

	case KX2DungeonTool::IDC_RATE_EDIT:
		switch( nEvent )
		{
		case EVENT_EDITBOX_CHANGE:
			IS_NULL_POINTER( m_pEditingNpc );
			EditingEditBox( IDC_RATE_EDIT, m_pEditingNpc->m_Rate, m_backupNpc.m_Rate );
			break;
		}
		break;

	case KX2DungeonTool::IDC_NPC_KEY_CODE_EDIT:
		switch( nEvent )
		{
		case EVENT_EDITBOX_CHANGE:
			IS_NULL_POINTER( m_pEditingNpc );
			EditingEditBox( IDC_NPC_KEY_CODE_EDIT, m_pEditingNpc->m_KeyCode, m_backupNpc.m_KeyCode );

			////////////////////////////////////////////////////////////////////////// 육사
			IS_NULL_POINTER( m_pEditingSubStage );
			CDXUTComboBox* pCombo = m_LeftToolUI.GetComboBox( IDC_CONDITION_ELEMENT_COMBO );
			pCombo->RemoveAllItems();
			std::vector< CX2DungeonSubStage::NPCData* >::iterator vitNpcData = m_pEditingSubStage->m_NPCDataList.begin();
			for( ; vitNpcData != m_pEditingSubStage->m_NPCDataList.end(); vitNpcData++ )
			{
				CX2DungeonSubStage::NPCData* pNpcData = *vitNpcData;
				if( pNpcData->m_KeyCode > 0 )
				{
					wsprintf( buf, L"NPC KEY CODE %2d", pNpcData->m_KeyCode );
					pCombo->AddItem( buf, reinterpret_cast<LPVOID>(pNpcData->m_KeyCode) );
				}
			}
			//////////////////////////////////////////////////////////////////////////			
			break;
		}
		break;

	case KX2DungeonTool::IDC_FOCUS_CAMERA_CHECK:
		switch( nEvent )
		{
		case EVENT_CHECKBOX_CHANGED:
			IS_NULL_POINTER( m_pEditingNpc );
			EditingCheckBox( IDC_FOCUS_CAMERA_CHECK, m_pEditingNpc->m_bFocusCamera );
			break;
		}
		break;

	case KX2DungeonTool::IDC_HAVE_BOSS_GAGE_CHECK:
		switch( nEvent )
		{
		case EVENT_CHECKBOX_CHANGED:
			IS_NULL_POINTER( m_pEditingNpc );
			EditingCheckBox( IDC_HAVE_BOSS_GAGE_CHECK, m_pEditingNpc->m_bHasBossGage );
			break;
		}
		break;

	case KX2DungeonTool::IDC_SUB_NPC_CHECK:
		switch( nEvent )
		{
		case EVENT_CHECKBOX_CHANGED:
            IS_NULL_POINTER( m_pEditingNpc );
			EditingCheckBox( IDC_SUB_NPC_CHECK, m_pEditingNpc->m_bSubNpc );
			break;
		}
		break;

	case KX2DungeonTool::IDC_SIEGE_MODE_CHECK:
		switch( nEvent )
		{
		case EVENT_CHECKBOX_CHANGED:
			IS_NULL_POINTER( m_pEditingNpc );
			EditingCheckBox( IDC_SIEGE_MODE_CHECK, m_pEditingNpc->m_bSiegeMode );
			break;
		}
		break;

	case KX2DungeonTool::IDC_IS_RIGHT_CHECK:
		switch( nEvent )
		{
		case EVENT_CHECKBOX_CHANGED:
			IS_NULL_POINTER( m_pEditingNpc );
			EditingCheckBox( IDC_IS_RIGHT_CHECK, m_pEditingNpc->m_bIsRight );
			break;
		}
		break;

		//////////////////////////////////////////////////////////////// Trigger Editing
	case KX2DungeonTool::IDC_ADD_TRIGGER_BUTTON:
		{
			IS_NULL_POINTER( m_pEditingSubStage );
			CX2DungeonSubStage::Trigger* pNewTrigger = new CX2DungeonSubStage::Trigger();
			pNewTrigger->m_bHost		 = false;
			pNewTrigger->m_bOneTime		 = false;
			pNewTrigger->m_ConditionFunc = L"";
			pNewTrigger->m_ReactFunc	 = L"";
			pNewTrigger->m_Interval		 = 0.0f;
			m_pEditingSubStage->m_TriggerList.push_back( pNewTrigger );
			LoadSubStageInfo( m_pEditingSubStage, false );
			LoadTriggerInfo( pNewTrigger );
			m_toolUI.GetListBox( IDC_TRIGGER_LIST )->LastItemSelect();

			START_LOG( clog, L"Trigger 추가" );
		}
		break;
		
	case KX2DungeonTool::IDC_DEL_TRIGGER_BUTTON:
		{
			IS_NULL_POINTER( m_pEditingSubStage );
			int delNum = m_toolUI.GetListBox( IDC_TRIGGER_LIST )->GetSelectedIndex();
			if( delNum == -1 )
				break;
			CX2DungeonSubStage::Trigger* pTrigger = m_pEditingSubStage->m_TriggerList[delNum];
			std::vector<CX2DungeonSubStage::Trigger*>::iterator mit = std::find(
				m_pEditingSubStage->m_TriggerList.begin(),
				m_pEditingSubStage->m_TriggerList.end(),
				pTrigger
				);
			if( mit ==  m_pEditingSubStage->m_TriggerList.end() )
				break;
			m_pEditingSubStage->m_TriggerList.erase( mit );
			m_iDelTrigger = delNum; // 맨 아랫쪽 인덱스의 데이터 삭제시 포커스 바꿔주기용 변수
			SAFE_DELETE( pTrigger );
			LoadSubStageInfo( m_pEditingSubStage, false );
			RemoveTriggerToolUI();

			START_LOG( clog, L"Trigger 삭제" );
		}
		break;

	case KX2DungeonTool::IDC_HOST_CHECK:
		switch( nEvent )
		{
		case EVENT_CHECKBOX_CHANGED:
			IS_NULL_POINTER( m_pEditingTrigger );
			EditingCheckBox( IDC_HOST_CHECK, m_pEditingTrigger->m_bHost );
			break;
		}
		break;

	case KX2DungeonTool::IDC_ONE_TIME_CHECK:
		switch( nEvent )
		{
		case EVENT_CHECKBOX_CHANGED:
			IS_NULL_POINTER( m_pEditingTrigger );
			EditingCheckBox( IDC_ONE_TIME_CHECK, m_pEditingTrigger->m_bOneTime );
			break;
		}
		break;

	case KX2DungeonTool::IDC_INTERVAL_EDIT:
		switch( nEvent )
		{
		case EVENT_EDITBOX_CHANGE:
			IS_NULL_POINTER( m_pEditingTrigger );
			EditingEditBox( IDC_INTERVAL_EDIT, m_pEditingTrigger->m_Interval, m_backupTrigger.m_Interval );
			break;
		}
		break;

	case KX2DungeonTool::IDC_CONDITION_FUNC_BUTTON:
		switch( nEvent )
		{
		case EVENT_BUTTON_CLICKED:
			IS_NULL_POINTER( m_pEditingTrigger );			
			m_toolUI.GetEditBox( IDC_FUNCTION_VIEWER_EDIT )->SetText( m_pEditingTrigger->m_ConditionFunc.c_str() );
			m_bIsConditionFunc = true;
			break;
		}
		break;

	case KX2DungeonTool::IDC_REACT_FUNG_BUTTON:
		switch( nEvent )
		{
		case EVENT_BUTTON_CLICKED:
			IS_NULL_POINTER( m_pEditingTrigger );			
			m_toolUI.GetEditBox( IDC_FUNCTION_VIEWER_EDIT )->SetText( m_pEditingTrigger->m_ReactFunc.c_str() );
			m_bIsConditionFunc = false;
			break;
		}
		break;

	case KX2DungeonTool::IDC_FUNCTION_VIEWER_EDIT:
		switch( nEvent )
		{
		case EVENT_EDITBOX_CHANGE:
			IS_NULL_POINTER( m_pEditingTrigger );
			if( m_bIsConditionFunc )
			{
				m_pEditingTrigger->m_ConditionFunc = m_toolUI.GetEditBox( IDC_FUNCTION_VIEWER_EDIT )->GetText();
			}
			else
			{
				m_pEditingTrigger->m_ReactFunc = m_toolUI.GetEditBox( IDC_FUNCTION_VIEWER_EDIT )->GetText();
			}
			break;
		}
		break;

	default:
		break;
	}
}

void CALLBACK OnLeftToolUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
	KX2DungeonTool* pDungeonTool = (KX2DungeonTool*) pUserContext;
	pDungeonTool->OnLeftToolUIEventCallback( nEvent, nControlID, pControl );
}

void KX2DungeonTool::OnLeftToolUIEventCallback(  UINT nEvent, int nControlID, CDXUTControl* pControl )
{
	WCHAR buf[128] = L"";
	std::wstring temp;
	std::string temp2;

	switch( nControlID )
	{
		////////////////////////////////////////////////////////////////// Load List Info
	case KX2DungeonTool::IDC_CLEAR_CONDITION_LIST:
		switch( nEvent )
		{
		case EVENT_LISTBOX_SELECTION:
			if( !LoadClearCondition( ((CDXUTListBox *)pControl)->GetSelectedIndex() ) )
				MessageBox( g_pKTDXApp->GetHWND(), L"클리어 조건 정보 로드 실패!", L"test", MB_OK );

			START_LOG( clog, L"Clear Condition 리스트 박스 선택" )
				<< BUILD_LOG( ((CDXUTListBox *)pControl)->m_nSelected )
				<< BUILD_LOG( ((CDXUTListBox *)pControl)->m_nSelStart )
				<< BUILD_LOG( ((CDXUTListBox *)pControl)->GetSize() );
			break;
		}
		break;

	case KX2DungeonTool::IDC_ADD_CLEAR_CONDITION_BUTTON:
		{
			IS_NULL_POINTER( m_pEditingSubStage );
			CX2DungeonSubStage::ClearCondtionData newClearCon;
			newClearCon.m_eClearCondition = CX2DungeonSubStage::CC_KILL_KEYCODE_NPC;
			
			m_pEditingSubStage->m_vecClearCondData.push_back( newClearCon );
			LoadSubStageInfo( m_pEditingSubStage, false );
			LoadClearCondition( m_pEditingSubStage->m_vecClearCondData.back() );
			m_LeftToolUI.GetListBox( IDC_CLEAR_CONDITION_LIST )->LastItemSelect();

			START_LOG( clog, L"Clear Condition 추가" );
		}
		break;

	case KX2DungeonTool::IDC_DEL_CLEAR_CONDITION_BUTTON:
		{
			IS_NULL_POINTER( m_pEditingSubStage );
			int delNum = m_LeftToolUI.GetListBox( IDC_CLEAR_CONDITION_LIST )->GetSelectedIndex();
			if( delNum == -1 )
				break;			
			std::vector<CX2DungeonSubStage::ClearCondtionData>::iterator vit = m_pEditingSubStage->m_vecClearCondData.begin();
			for( int iIdx = 0; vit != m_pEditingSubStage->m_vecClearCondData.end(); vit++, iIdx++ )
			{
				if( iIdx == delNum )
					break;				
			}
			if( vit == m_pEditingSubStage->m_vecClearCondData.end() )
				break;
			m_pEditingSubStage->m_vecClearCondData.erase( vit );
			m_iDelClearCon = delNum; // 맨 아랫쪽 인덱스의 데이터 삭제시 포커스 바꿔주기용 변수			
			LoadSubStageInfo( m_pEditingSubStage, false );
			RemoveClearConditionToolUI();

			START_LOG( clog, L"Clear Condition 삭제" );
		}
		break;
		
	case KX2DungeonTool::IDC_CLEAR_CONDITION_COMBO:
		switch( nEvent )
		{
		case EVENT_COMBOBOX_SELECTION_CHANGED:
			IS_NULL_POINTER( m_pEditingClearCon );
			EditingComboBox( m_LeftToolUI, IDC_CLEAR_CONDITION_COMBO, m_pEditingClearCon->m_eClearCondition, GET_CLEAR_CONDITION() );
			LoadConditionElements( m_pEditingClearCon->m_eClearCondition ); // 해당 조건에 맞는 UI로 변경
			break;
		}
		break;
			
		////////////////////////////////////////////////////////////////////////// 육사
	case KX2DungeonTool::IDC_ADD_CONDITION_ELEMENT:
		{
			IS_NULL_POINTER( m_pEditingClearCon );
			DXUTComboBoxItem* comboBoxItem = m_LeftToolUI.GetComboBox( IDC_CONDITION_ELEMENT_COMBO )->GetSelectedItem();
			if( comboBoxItem == NULL )
				break;
			
			switch( m_pEditingClearCon->m_eClearCondition )
			{
			case CX2DungeonSubStage::CC_KILL_KEYCODE_NPC:
			case CX2DungeonSubStage::CC_KILL_KEYCODE_NPC_ANY:
				{
					std::vector<MonsterCount>::iterator vit = std::find_if( 
						m_pEditingClearCon->m_vecNPCKeyCode.begin(), 
						m_pEditingClearCon->m_vecNPCKeyCode.end(), 
						MC_FIND_IF( reinterpret_cast<int>(comboBoxItem->pData) )
						);

					if( vit != m_pEditingClearCon->m_vecNPCKeyCode.end() )
					{				
						MessageBox( g_pKTDXApp->GetHWND(), L"이미 등록된 NPC Key Code 입니다!", L"test", MB_OK );
						return;
					}

					MonsterCount mcTemp;
					mcTemp.m_iKey			= reinterpret_cast<int>(comboBoxItem->pData);
					mcTemp.m_iOriginalCount = 0;
					m_pEditingClearCon->m_vecNPCKeyCode.push_back( mcTemp );

					wsprintf( buf, L"NPC KEY CODE %2d", reinterpret_cast<int>(comboBoxItem->pData) );
					m_LeftToolUI.GetListBox( IDC_CONDITION_ELEMENT_LIST )->AddItem( buf, comboBoxItem->pData );
				}
				break;

			case CX2DungeonSubStage::CC_KILL_NPC_TYPE:
				{
					std::vector<MonsterCount>::iterator vit = std::find_if( 
						m_pEditingClearCon->m_vecNPCID.begin(), 
						m_pEditingClearCon->m_vecNPCID.end(), 
						MC_FIND_IF( reinterpret_cast<int>(comboBoxItem->pData) )
						);

					if( vit != m_pEditingClearCon->m_vecNPCID.end() )
					{				
						MessageBox( g_pKTDXApp->GetHWND(), L"이미 등록된 NPC ID 입니다!", L"test", MB_OK );
						return;
					}

					MonsterCount mcNpcID;
					mcNpcID.m_iKey			 = reinterpret_cast<int>(comboBoxItem->pData);
					mcNpcID.m_iOriginalCount = 0;
					m_pEditingClearCon->m_vecNPCID.push_back( mcNpcID );
					
					NpcListIterator mit = m_mapNpcList.find( static_cast<CX2UnitManager::NPC_UNIT_ID>(mcNpcID.m_iKey) );
					m_LeftToolUI.GetListBox( IDC_CONDITION_ELEMENT_LIST )->AddItem( mit->second.c_str(), comboBoxItem->pData );
				}
				break;

			default:
				break;
			}
		}
		break;

	case KX2DungeonTool::IDC_DEL_CONDITION_ELEMENT:
		{
			IS_NULL_POINTER( m_pEditingClearCon );
			int iSelIndex = m_LeftToolUI.GetListBox( IDC_CONDITION_ELEMENT_LIST )->GetSelectedIndex();
			if( iSelIndex == -1 )
				break;

			switch( m_pEditingClearCon->m_eClearCondition )
			{
			case CX2DungeonSubStage::CC_KILL_KEYCODE_NPC:
			case CX2DungeonSubStage::CC_KILL_KEYCODE_NPC_ANY:
				{
					int iKeyCode = reinterpret_cast<int>(m_LeftToolUI.GetListBox( IDC_CONDITION_ELEMENT_LIST )->GetSelectedItem()->pData);

					std::vector<MonsterCount>::iterator vit = std::find_if( m_pEditingClearCon->m_vecNPCKeyCode.begin(), m_pEditingClearCon->m_vecNPCKeyCode.end(), MC_FIND_IF( iKeyCode ) );
					m_pEditingClearCon->m_vecNPCKeyCode.erase( vit );

					m_LeftToolUI.GetListBox( IDC_CONDITION_ELEMENT_LIST )->RemoveItem( iSelIndex );
				}
				break;

			case CX2DungeonSubStage::CC_KILL_NPC_TYPE:
				{
					int iNpcID = reinterpret_cast<int>(m_LeftToolUI.GetListBox( IDC_CONDITION_ELEMENT_LIST )->GetSelectedItem()->pData);

					std::vector<MonsterCount>::iterator vit = std::find_if( m_pEditingClearCon->m_vecNPCID.begin(), m_pEditingClearCon->m_vecNPCID.end(), MC_FIND_IF( iNpcID ) );
					m_pEditingClearCon->m_vecNPCID.erase( vit );

					m_LeftToolUI.GetListBox( IDC_CONDITION_ELEMENT_LIST )->RemoveItem( iSelIndex );
				}
				break;

			default:
				break;
			}
		}
		break;
		//////////////////////////////////////////////////////////////////////////		

	case KX2DungeonTool::IDC_NEXT_STAGE_LIST:
		switch( nEvent )
		{
		case EVENT_LISTBOX_SELECTION:
			if( !LoadClearConNextStage( ((CDXUTListBox *)pControl)->GetSelectedIndex() ) )
				MessageBox( g_pKTDXApp->GetHWND(), L"클리어 조건 다음 스테이지 정보 로드 실패!", L"test", MB_OK );

			START_LOG( clog, L"Clear Condition Next Stage 리스트 박스 선택" )
				<< BUILD_LOG( ((CDXUTListBox *)pControl)->m_nSelected )
				<< BUILD_LOG( ((CDXUTListBox *)pControl)->m_nSelStart )
				<< BUILD_LOG( ((CDXUTListBox *)pControl)->GetSize() );
			break;
		}
		break;

	case KX2DungeonTool::IDC_ADD_NEXT_STAGE_BUTTON:
		{
			IS_NULL_POINTER( m_pEditingClearCon );
			CX2DungeonSubStage::NextStageData newNextStageData;
			newNextStageData.m_eClearType	  = CX2DungeonSubStage::CT_STAGE;
			newNextStageData.m_iRate		  = 0;
			newNextStageData.m_iStageIndex	  = 0;
			newNextStageData.m_iSubStageIndex = 0;

			m_pEditingClearCon->m_vecNextStage.push_back( newNextStageData );
			LoadClearCondition( *m_pEditingClearCon );
			LoadClearConNextStage( m_pEditingClearCon->m_vecNextStage.back() );
			m_LeftToolUI.GetListBox( IDC_NEXT_STAGE_LIST )->LastItemSelect();

			START_LOG( clog, L"Clear Condition Next Stage 추가" );
		}
		break;

	case KX2DungeonTool::IDC_DEL_NEXT_STAGE_BUTTON:
		{
			IS_NULL_POINTER( m_pEditingClearCon );
			int delNum = m_LeftToolUI.GetListBox( IDC_NEXT_STAGE_LIST )->GetSelectedIndex();
			if( delNum == -1 )
				break;
			std::vector<CX2DungeonSubStage::NextStageData>::iterator vit = m_pEditingClearCon->m_vecNextStage.begin();
			for( int iIdx = 0; vit != m_pEditingClearCon->m_vecNextStage.end(); vit++, iIdx++ )
			{
				if( iIdx == delNum )
					break;
			}
			if( vit == m_pEditingClearCon->m_vecNextStage.end() )
				break;
			m_pEditingClearCon->m_vecNextStage.erase( vit );
			m_iDelNextStage = delNum; // 맨 아랫쪽 인덱스의 데이터 삭제시 포커스 바꿔주기용 변수
			LoadClearCondition( *m_pEditingClearCon );
			RemoveClearConNextStageToolUI();

			START_LOG( clog, L"Clear Condition Next Stage 삭제" );
		}
		break;

	case KX2DungeonTool::IDC_CLEAR_TYPE_COMBO:
		switch( nEvent )
		{
		case EVENT_COMBOBOX_SELECTION_CHANGED:
			IS_NULL_POINTER( m_pEditingNextStage );
			EditingComboBox( m_LeftToolUI, IDC_CLEAR_TYPE_COMBO, m_pEditingNextStage->m_eClearType, GET_CLEAR_TYPE() );
			break;
		}
		break;

	case KX2DungeonTool::IDC_CLEAR_CONDITION_STAGE_COMBO:
		switch( nEvent )
		{
		case EVENT_COMBOBOX_SELECTION_CHANGED:
			IS_NULL_POINTER( m_pEditingNextStage );
			EditingComboBox( m_LeftToolUI, IDC_CLEAR_CONDITION_STAGE_COMBO, m_pEditingNextStage->m_iStageIndex, GET_INDEX_NUMBER() );

			////////////////////////////////////////////////////////// Insert SubStage List
			CDXUTComboBox *pCombo;
			pCombo = m_LeftToolUI.GetComboBox( IDC_CLEAR_CONDITION_SUBSTAGE_COMBO );
			pCombo->RemoveAllItems();
			CX2DungeonStage::StageData* pStageData = m_pEditingDungeon->GetStageDataList()[m_pEditingNextStage->m_iStageIndex]; // 크래쉬 날수도 -ㅅ -..
			for( UINT iSubStage = 0; iSubStage < pStageData->m_SubStageDataList.size(); iSubStage++ )
			{
				wsprintf( buf, L"%2d", iSubStage );
				pCombo->AddItem( buf, reinterpret_cast<LPVOID>(iSubStage) );
			}
			//////////////////////////////////////////////////////////////////////////
			break;
		}
		break;

	case KX2DungeonTool::IDC_CLEAR_CONDITION_SUBSTAGE_COMBO:
		switch( nEvent )
		{
		case EVENT_COMBOBOX_SELECTION_CHANGED:
			IS_NULL_POINTER( m_pEditingNextStage );
			EditingComboBox( m_LeftToolUI, IDC_CLEAR_CONDITION_SUBSTAGE_COMBO, m_pEditingNextStage->m_iSubStageIndex, GET_INDEX_NUMBER() );
			break;
		}
		break;

	case KX2DungeonTool::IDC_CLEAR_CONDITION_RATE_EDIT:
		switch( nEvent )
		{
		case EVENT_EDITBOX_CHANGE:
			IS_NULL_POINTER( m_pEditingNextStage );
			EditingEditBox( m_LeftToolUI, IDC_CLEAR_CONDITION_RATE_EDIT, m_pEditingNextStage->m_iRate, m_backupNextStage.m_iRate );
			break;
		}
		break;

	default:
		break;
	}
}

void CALLBACK OnMapUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
	KX2DungeonTool* pDungeonTool = (KX2DungeonTool*) pUserContext;
	pDungeonTool->OnMapUIEventCallback( nEvent, nControlID, pControl );
}

void KX2DungeonTool::OnMapUIEventCallback( UINT nEvent, int nControlID, CDXUTControl* pControl )
{
	WCHAR buf[128] = L"";
	//OPENFILENAME ofn;        // common dialog box structure
	//WCHAR wszFileName[512];  // path까지 포함한 파일 이름

	switch( nControlID )
	{		
	case KX2DungeonTool::IDC_MAP_STAGE_LIST:
		switch( nEvent )
		{
		case EVENT_LISTBOX_SELECTION:
			IS_NULL_POINTER( m_pEditingDungeon );
			int index = ((CDXUTListBox *)pControl)->GetSelectedIndex();
			CX2DungeonStage::StageData* pStageData = m_pEditingDungeon->GetStageDataList()[index];
			m_mapUI.GetListBox( IDC_MAP_SUB_STAGE_LIST )->RemoveAllItems();
			std::vector< CX2DungeonSubStage::SubStageData* >::iterator mit2 = pStageData->m_SubStageDataList.begin();
			for(int i=0 ; mit2 != pStageData->m_SubStageDataList.end(); mit2++, i++ )
			{
				wsprintf( buf, L"Sub Stage %d", i );
				m_mapUI.GetListBox( IDC_MAP_SUB_STAGE_LIST )->AddItem( buf, NULL );
			}
			break;
		}
		break;

	case KX2DungeonTool::IDC_MAP_SUB_STAGE_LIST:
		switch( nEvent )
		{
		case EVENT_LISTBOX_SELECTION:
			IS_NULL_POINTER( m_pEditingDungeon );
			int iStage    = m_mapUI.GetListBox( IDC_MAP_STAGE_LIST )->GetSelectedIndex();
			int iSubStage = m_mapUI.GetListBox( IDC_MAP_SUB_STAGE_LIST )->GetSelectedIndex();
			DungeonMapIterator mit = m_mapDungeonMapList.find( std::make_pair( iStage, iSubStage ) );
			if( mit != m_mapDungeonMapList.end() )
			{
				m_pEditingMiniMap = &mit->second;
			}
			else
			{
				CX2Dungeon::DungeonMapData mapData;
				mapData.m_bIsBossStage = false;
				mapData.m_bOpen = false;
				mapData.m_vPos.x = 0.0f;
				mapData.m_vPos.y = 0.0f;
				m_mapDungeonMapList.insert( std::make_pair( std::make_pair( iStage, iSubStage ), mapData ) );

				mit = m_mapDungeonMapList.find( std::make_pair( iStage, iSubStage ) );

				m_pEditingMiniMap = &mit->second;
			}
			WCHAR buf[128] = L"";
			wsprintf( buf, L"%d", (int)m_pEditingMiniMap->m_vPos.x );
			m_mapUI.GetEditBox( IDC_MAP_X_EDIT )->SetText( buf );
			wsprintf( buf, L"%d", (int)m_pEditingMiniMap->m_vPos.y );
			m_mapUI.GetEditBox( IDC_MAP_Y_EDIT )->SetText( buf );
			m_mapUI.GetCheckBox( IDC_MAP_BOSS_CHECK )->SetChecked( m_pEditingMiniMap->m_bIsBossStage );
			m_mapUI.GetCheckBox( IDC_IS_NEXT_RANDOM_STAGE_CHECK )->SetChecked( m_pEditingMiniMap->m_bIsNextStageRandom );

			m_pDungeonMapUI->SetSelectedMiniMap( iStage, iSubStage );
			break;
		}
		break;

	case KX2DungeonTool::IDC_MAP_X_EDIT:
		switch( nEvent )
		{
		case EVENT_EDITBOX_CHANGE:
			IS_NULL_POINTER( m_pEditingMiniMap );
			std::string temp;
			ConvertWCHARToChar( temp, m_mapUI.GetEditBox( IDC_MAP_X_EDIT )->GetText() );
			m_pEditingMiniMap->m_vPos.x = (float)atoi( temp.c_str() );
			break;
		}
		break;

	case KX2DungeonTool::IDC_MAP_Y_EDIT:
		switch( nEvent )
		{
		case EVENT_EDITBOX_CHANGE:
			IS_NULL_POINTER( m_pEditingMiniMap );
			std::string temp;
			ConvertWCHARToChar( temp, m_mapUI.GetEditBox( IDC_MAP_Y_EDIT )->GetText() );
			m_pEditingMiniMap->m_vPos.y = (float)atoi( temp.c_str() );
			break;
		}
		break;

	case KX2DungeonTool::IDC_MAP_BOSS_CHECK:
		switch( nEvent )
		{
		case EVENT_CHECKBOX_CHANGED:
			IS_NULL_POINTER( m_pEditingMiniMap );
			m_pEditingMiniMap->m_bIsBossStage = m_mapUI.GetCheckBox( IDC_MAP_BOSS_CHECK )->GetChecked();
			break;		
		}
		break;

	case KX2DungeonTool::IDC_IS_NEXT_RANDOM_STAGE_CHECK:
		switch( nEvent )
		{
		case EVENT_CHECKBOX_CHANGED:
			IS_NULL_POINTER( m_pEditingMiniMap );
			m_pEditingMiniMap->m_bIsNextStageRandom = m_mapUI.GetCheckBox( IDC_IS_NEXT_RANDOM_STAGE_CHECK )->GetChecked();
			break;
		}
		break;

	//case KX2DungeonTool::IDC_TEXTURE_LOAD_BUTTON:
	//	switch( nEvent )
	//	{
	//	case EVENT_BUTTON_CLICKED:
	//		IS_NULL_POINTER( m_pEditingDungeon );
	//		ZeroMemory(&ofn, sizeof(ofn));
	//		ofn.lStructSize = sizeof(ofn);
	//		ofn.hwndOwner = g_pKTDXApp->GetHWND(); 
	//		ofn.lpstrFile = (LPWSTR)wszFileName;
	//		ofn.lpstrFile[0] = '\0';
	//		ofn.nMaxFile = sizeof(wszFileName);
	//		ofn.lpstrFilter = L"*.tga\0";
	//		ofn.nFilterIndex = 1;
	//		ofn.lpstrFileTitle = NULL;
	//		ofn.nMaxFileTitle = 0;
	//		ofn.lpstrInitialDir = NULL;
	//		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	//		if( GetOpenFileName(&ofn) == TRUE ) 
	//		{
	//			if( !LoadTexture( ofn.lpstrFile ) )
	//			{
	//				MessageBox( g_pKTDXApp->GetHWND(), L"텍스쳐 로드 실패!", L"test", MB_OK );
	//			}
	//		}
	//		break;
	//	}
	//	break;

	default:
		break;
	}	
}

void CALLBACK OnSummaryUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
	KX2DungeonTool* pDungeonTool = (KX2DungeonTool*) pUserContext;
	pDungeonTool->OnSummaryUIEventCallback( nEvent, nControlID, pControl );
}

void KX2DungeonTool::OnSummaryUIEventCallback( UINT nEvent, int nControlID, CDXUTControl* pControl )
{
	WCHAR buf[128] = L"";
	//OPENFILENAME ofn;        // common dialog box structure
	//WCHAR wszFileName[512];  // path까지 포함한 파일 이름

	switch( nControlID )
	{
	case KX2DungeonTool::IDC_SUMMARY_LIST:		
		break;

	default:
		break;
	}
}

void	KX2DungeonTool::InitUI()
{
	POINT rp;	

	// resolution : 1024 x 768
	//SET_POINT( m_mainUIPoint, 10, 10);
	//SET_POINT( m_stageInfoPoint, 814, 80 );
	//SET_POINT( m_curtainPoint, 814, 500);
	//SET_POINT( m_npcViewPoint, 814, 500);
	//SET_POINT( m_triggerPoint, 600, 500);

	// resolution : 1280 x 1023
	m_resolution.Width	= 1280;
	m_resolution.Height = 1023;
	SET_POINT( m_mainUIPoint,		  10,	 122 );
	SET_POINT( m_stageInfoPoint,	1070,	  80 );
	if( m_bUIWideMode )
	{
		SET_POINT( m_npcViewPoint,	 500,	 124 );
	}
	else
	{
		SET_POINT( m_npcViewPoint,	1070,	 582 );
	}
	SET_POINT( m_triggerPoint,		 840,	  10 );
	SET_POINT( m_curtainPoint,		 840,	 277 );
	SET_POINT( m_subStagePoint,		 840,	 487 );
	SET_POINT( m_clearConPoint,		  10,	 122 );
	SET_POINT( m_mapUIPoint,		 330,	 120 );
	SET_POINT( m_summaryPoint,		 330,	 120 );

	////////////////////////////////////////////////////////////////// MainUI
	extern CDXUTDialogResourceManager g_DialogResourceManager;
	m_mainUI.Init( &g_DialogResourceManager );
	m_mainUI.SetCallback( OnMainUIEvent, this );
	m_mainUI.SetSize( 340, 300 );
	m_mainUI.SetFont(0, L"굴림", 12, 12);

	SET_POINT( rp, 0, 0 );
	m_mainUI.AddListBox( IDC_DUNGEON_LIST										, m_mainUIPoint.x+rp.x	, m_mainUIPoint.y+rp.y	, 400, 300 );		ADD_POINT( rp, 50, 310 );
	m_pDungeonListBox = m_mainUI.GetListBox( IDC_DUNGEON_LIST );
	m_mainUI.AddButton( IDC_DUNGEON_LOAD_BUTTON	, L"던전 로드"					, m_mainUIPoint.x+rp.x	, m_mainUIPoint.y+rp.y	, 100, 20 );		ADD_POINT( rp, 400, -310 ); 
	m_mainUI.AddListBox( IDC_STAGE_LIST											, m_mainUIPoint.x+rp.x	, m_mainUIPoint.y+rp.y	, 400, 300 );		ADD_POINT( rp, 50, 310 );
	m_pStageListBox = m_mainUI.GetListBox( IDC_STAGE_LIST );
	m_mainUI.AddButton( IDC_STAGE_LOAD_BUTTON	, L"스테이지 로드"				, m_mainUIPoint.x+rp.x	, m_mainUIPoint.y+rp.y	, 100, 20 );		ADD_POINT( rp, 150, 0 ); 
	m_mainUI.AddButton( IDC_VIEW_ALL_WORLD_BUTTON, L"모든 스테이지 보기"		, m_mainUIPoint.x+rp.x	, m_mainUIPoint.y+rp.y	, 150, 20 );		ADD_POINT( rp, -200, 0/*20*/ ); 
	m_mainUI.AddComboBox( IDC_WORLD_ID_COMBO	,								  m_mainUIPoint.x+rp.x	, m_mainUIPoint.y+rp.y	, 400, 30 );		ADD_POINT( rp, 50, 30 );
	m_mainUI.GetComboBox( IDC_WORLD_ID_COMBO )->SetDropHeight( 400 );
	m_mainUI.AddButton( IDC_ADD_STAGE_BUTTON	, L"스테이지 추가"				, m_mainUIPoint.x+rp.x	, m_mainUIPoint.y+rp.y	, 100, 20 );		ADD_POINT( rp, 150, 0 );
	m_mainUI.AddButton( IDC_DEL_STAGE_BUTTON	, L"스테이지 삭제"				, m_mainUIPoint.x+rp.x	, m_mainUIPoint.y+rp.y	, 100, 20 );		ADD_POINT( rp, -200, 30 );
	m_mainUI.AddStatic( IDC_STATIC_DUNGEON_WORLD_COLOR, L"Dungeon World Color"	, m_mainUIPoint.x+rp.x	, m_mainUIPoint.y+rp.y	, 180 , 30 );		ADD_POINT( rp, 180, 0 );
	m_mainUI.AddEditBox( IDC_DUNGEON_WORLD_COLOR_R, L""							, m_mainUIPoint.x+rp.x	, m_mainUIPoint.y+rp.y	, 40 , 30 );		ADD_POINT( rp, 40, 0 );
	m_mainUI.AddEditBox( IDC_DUNGEON_WORLD_COLOR_G, L""							, m_mainUIPoint.x+rp.x	, m_mainUIPoint.y+rp.y	, 40 , 30 );		ADD_POINT( rp, 40, 0 );
	m_mainUI.AddEditBox( IDC_DUNGEON_WORLD_COLOR_B, L""							, m_mainUIPoint.x+rp.x	, m_mainUIPoint.y+rp.y	, 40 , 30 );		ADD_POINT( rp, -260, 30 );
	m_mainUI.AddStatic( IDC_STATIC_DUNGEON_UNIT_COLOR, L"Dungeon Unit Color"	, m_mainUIPoint.x+rp.x	, m_mainUIPoint.y+rp.y	, 180 , 30 );		ADD_POINT( rp, 180, 0 );
	m_mainUI.AddEditBox( IDC_DUNGEON_UNIT_COLOR_R, L""							, m_mainUIPoint.x+rp.x	, m_mainUIPoint.y+rp.y	, 40 , 30 );		ADD_POINT( rp, 40, 0 );
	m_mainUI.AddEditBox( IDC_DUNGEON_UNIT_COLOR_G, L""							, m_mainUIPoint.x+rp.x	, m_mainUIPoint.y+rp.y	, 40 , 30 );		ADD_POINT( rp, 40, 0 );
	m_mainUI.AddEditBox( IDC_DUNGEON_UNIT_COLOR_B, L""							, m_mainUIPoint.x+rp.x	, m_mainUIPoint.y+rp.y	, 40 , 30 );

	// 기능 미구현
	m_mainUI.GetButton( IDC_STAGE_LOAD_BUTTON )->SetVisible( false );
	m_mainUI.GetButton( IDC_VIEW_ALL_WORLD_BUTTON )->SetVisible( false );

	////////////////////////////////////////////////////////////////// ToolUI
	m_toolUI.Init( &g_DialogResourceManager );
	m_toolUI.SetCallback( OnToolUIEvent, this );
	m_toolUI.SetSize( 340, 300 );
	m_toolUI.SetFont(0, L"굴림", 12, 12);

	// StageInfo
	SET_POINT( rp, 0, 0 );
	m_toolUI.AddStatic( IDC_STATIC_STAGE_INFO	, L"------[ Stage Info ]------"	, m_stageInfoPoint.x	 , m_stageInfoPoint.y	 , 200, 22 );		ADD_POINT( rp, 50, 25 );
	m_toolUI.AddStatic( IDC_STATIC_STAGE_INFO_1	, L"SubStage List"				, m_stageInfoPoint.x+rp.x, m_stageInfoPoint.y+rp.y, 100, 22 );		ADD_POINT( rp, 0, 23 );
	m_toolUI.AddListBox( IDC_SUB_STAGE_LIST		,								  m_stageInfoPoint.x	 , m_stageInfoPoint.y+rp.y, 200, 120 );		ADD_POINT( rp, 0, 122 );
	m_toolUI.AddButton( IDC_ADD_SUB_STAGE_BUTTON, L"Add SubStage"				, m_stageInfoPoint.x	 , m_stageInfoPoint.y+rp.y,	100, 22 );		ADD_POINT( rp, 50, 0 );
	m_toolUI.AddButton( IDC_DEL_SUB_STAGE_BUTTON, L"Del SubStage"				, m_stageInfoPoint.x+rp.x, m_stageInfoPoint.y+rp.y, 100, 22 );		ADD_POINT( rp, -50, 22 );
	m_toolUI.AddStatic( IDC_STATIC_STAGE_INFO_2 , L"Ready NPC List"				, m_stageInfoPoint.x+rp.x, m_stageInfoPoint.y+rp.y, 100, 22 );		ADD_POINT( rp, 0, 20 );
	m_toolUI.AddListBox( IDC_READY_NPC_LIST		,								  m_stageInfoPoint.x	 , m_stageInfoPoint.y+rp.y, 200, 120 );		ADD_POINT( rp, 0, 125 );
	m_toolUI.AddComboBox( IDC_NPC_ID_LIST_COMBO	,								  m_stageInfoPoint.x	 , m_stageInfoPoint.y+rp.y, 200, 30 );		ADD_POINT( rp, 0, 30 );
	m_toolUI.AddButton( IDC_ADD_READY_NPC_BUTTON, L"Add Ready NPC"				, m_stageInfoPoint.x	 , m_stageInfoPoint.y+rp.y, 100, 22 );		ADD_POINT( rp, 50, 0 );
	m_toolUI.AddButton( IDC_DEL_READY_NPC_BUTTON, L"Del Ready NPC"				, m_stageInfoPoint.x+rp.x, m_stageInfoPoint.y+rp.y, 100, 22 );		ADD_POINT( rp, 0, 22 );
	m_toolUI.AddCheckBox( IDC_START_MOTION_CHECK, L"Start Motion Check"			, m_stageInfoPoint.x	 , m_stageInfoPoint.y+rp.y, 200, 22 );		ADD_POINT( rp, 0, 22 );
	m_toolUI.AddStatic( IDC_STATIC_WORLD_COLOR	, L"World Color"				, m_stageInfoPoint.x	 , m_stageInfoPoint.y+rp.y, 80, 30 );		ADD_POINT( rp, -20, 0 );
	m_toolUI.AddEditBox( IDC_WORLD_COLOR_R		, L""							, m_stageInfoPoint.x+rp.x, m_stageInfoPoint.y+rp.y, 40, 30 );		ADD_POINT( rp, 40, 0 );
	m_toolUI.AddEditBox( IDC_WORLD_COLOR_G		, L""							, m_stageInfoPoint.x+rp.x, m_stageInfoPoint.y+rp.y, 40, 30 );		ADD_POINT( rp, 40, 0 );
	m_toolUI.AddEditBox( IDC_WORLD_COLOR_B		, L""							, m_stageInfoPoint.x+rp.x, m_stageInfoPoint.y+rp.y, 40, 30 );		ADD_POINT( rp, -160, 30 );
	m_toolUI.AddStatic( IDC_STATIC_UNIT_COLOR	, L"Unit Color"					, m_stageInfoPoint.x	 , m_stageInfoPoint.y+rp.y, 80, 30 );		ADD_POINT( rp, 80, 0 );
	m_toolUI.AddEditBox( IDC_UNIT_COLOR_R		, L""							, m_stageInfoPoint.x+rp.x, m_stageInfoPoint.y+rp.y, 40, 30 );		ADD_POINT( rp, 40, 0 );
	m_toolUI.AddEditBox( IDC_UNIT_COLOR_G		, L""							, m_stageInfoPoint.x+rp.x, m_stageInfoPoint.y+rp.y, 40, 30 );		ADD_POINT( rp, 40, 0 );
	m_toolUI.AddEditBox( IDC_UNIT_COLOR_B		, L""							, m_stageInfoPoint.x+rp.x, m_stageInfoPoint.y+rp.y, 40, 30 );

	// Curtain
	SET_POINT( rp, 0, 0 );
	m_toolUI.AddStatic( IDC_STATIC_CURTAIN		, L"--------[ Curtain ]--------", m_curtainPoint.x		, m_curtainPoint.y		, 200, 22 );		ADD_POINT( rp, 0, 25 );
	m_toolUI.AddListBox( IDC_CURTAIN_LIST		,								  m_curtainPoint.x		, m_curtainPoint.y+rp.y	, 200, 100 );		ADD_POINT( rp, 0, 100 );
	m_toolUI.AddButton( IDC_ADD_CURTAIN_BUTTON	, L"Add Curtain"				, m_curtainPoint.x		, m_curtainPoint.y+rp.y	, 100, 22 );		ADD_POINT( rp, 100, 0 );
	m_toolUI.AddButton( IDC_DEL_CURTAIN_BUTTON	, L"Del Curtain"				, m_curtainPoint.x+rp.x	, m_curtainPoint.y+rp.y	, 100, 22 );		ADD_POINT( rp, -100, 22 );
	m_toolUI.AddStatic( IDC_STATIC_CURTAIN_1	, L"Line Index : "				, m_curtainPoint.x		, m_curtainPoint.y+rp.y	, 100, 30 );		ADD_POINT( rp, 100, 0 );
	m_toolUI.AddEditBox( IDC_LINE_INDEX_EDIT	, L""							, m_curtainPoint.x+rp.x , m_curtainPoint.y+rp.y	, 100, 30 );		ADD_POINT( rp, 0, 33 );
	((KX2DTEditBox*)m_toolUI.GetEditBox( IDC_LINE_INDEX_EDIT ))->SetIsOnlyNumber( true );	
	m_toolUI.AddCheckBox( IDC_AT_START_CHECK	, L"At Start"					, m_curtainPoint.x		, m_curtainPoint.y+rp.y	, 100, 22 );		ADD_POINT( rp, 0, 0 );
	m_toolUI.AddCheckBox( IDC_LOOK_LEFT_CHECK	, L"Look Left"					, m_curtainPoint.x+rp.x , m_curtainPoint.y+rp.y	, 100, 22 );

	// NPC
	SET_POINT( rp, 0, 0 );
	m_toolUI.AddStatic( IDC_STATIC_NPC			, L"----------[ NPC ]----------", m_npcViewPoint.x		, m_npcViewPoint.y		, 200, 22 );		ADD_POINT( rp, 0, 25 );
	m_toolUI.AddListBox( IDC_NPC_LIST			,								  m_npcViewPoint.x		, m_npcViewPoint.y+rp.y	, 200, 100 );		ADD_POINT( rp, 0, 100 );
	m_toolUI.AddComboBox( IDC_NPC_LIST_COMBO	,								  m_npcViewPoint.x		, m_npcViewPoint.y+rp.y, 200, 30 );		ADD_POINT( rp, 0, 30 );
	m_toolUI.AddButton( IDC_ADD_NPC_BUTTON		, L"Add NPC"					, m_npcViewPoint.x		, m_npcViewPoint.y+rp.y, 100, 22 );		ADD_POINT( rp, 100, 0 );
	m_toolUI.AddButton( IDC_DEL_NPC_BUTTON		, L"Del NPC"					, m_npcViewPoint.x+rp.x	, m_npcViewPoint.y+rp.y, 100, 22 );		ADD_POINT( rp, -100, 22 );
	m_toolUI.AddStatic( IDC_STATIC_NPC_1		, L"NPC ID : "					, m_npcViewPoint.x		, m_npcViewPoint.y+rp.y	, 100, 30 );		ADD_POINT( rp, 100, 0 );
	m_toolUI.AddEditBox( IDC_NPC_ID_EDIT		, L""							, m_npcViewPoint.x+rp.x	, m_npcViewPoint.y+rp.y	, 100, 30 );		ADD_POINT( rp, 0, 30 );
	((KX2DTEditBox*)m_toolUI.GetEditBox( IDC_NPC_ID_EDIT ))->SetIsOnlyNumber( true );
	m_toolUI.AddStatic( IDC_STATIC_NPC_2		, L"Start Pos : "				, m_npcViewPoint.x		, m_npcViewPoint.y+rp.y	, 100, 30 );		ADD_POINT( rp, 0, 0 );
	m_toolUI.AddEditBox( IDC_START_POS1_EDIT	, L""							, m_npcViewPoint.x+rp.x	, m_npcViewPoint.y+rp.y	, 50, 30 );			ADD_POINT( rp, 50, 0 );
	((KX2DTEditBox*)m_toolUI.GetEditBox( IDC_START_POS1_EDIT ))->SetIsOnlyNumber( true );
	m_toolUI.AddEditBox( IDC_START_POS2_EDIT	, L""							, m_npcViewPoint.x+rp.x	, m_npcViewPoint.y+rp.y	, 50, 30 );			ADD_POINT( rp, 0, 30 );
	((KX2DTEditBox*)m_toolUI.GetEditBox( IDC_START_POS2_EDIT ))->SetIsOnlyNumber( true );
	m_toolUI.AddStatic( IDC_STATIC_NPC_3		, L"Add Pos Y : "				, m_npcViewPoint.x		, m_npcViewPoint.y+rp.y	, 100, 30 );		ADD_POINT( rp, -50, 0 );
	m_toolUI.AddEditBox( IDC_ADD_POS_Y_EDIT		, L""							, m_npcViewPoint.x+rp.x	, m_npcViewPoint.y+rp.y	, 100, 30 );		ADD_POINT( rp, 0, 30 );
	((KX2DTEditBox*)m_toolUI.GetEditBox( IDC_ADD_POS_Y_EDIT ))->SetIsOnlyNumber( true );
	((KX2DTEditBox*)m_toolUI.GetEditBox( IDC_ADD_POS_Y_EDIT ))->SetMinusSign( true );
	m_toolUI.AddStatic( IDC_STATIC_NPC_4		, L"Level : "					, m_npcViewPoint.x		, m_npcViewPoint.y+rp.y	, 100, 30 );		ADD_POINT( rp, 0, 0 );
	m_toolUI.AddEditBox( IDC_LEVEL_EDIT			, L""							, m_npcViewPoint.x+rp.x	, m_npcViewPoint.y+rp.y	, 100, 30 );		ADD_POINT( rp, 0, 30 );
	((KX2DTEditBox*)m_toolUI.GetEditBox( IDC_LEVEL_EDIT ))->SetIsOnlyNumber( true );
	m_toolUI.AddCheckBox( IDC_ACTIVE_CHECK		, L"Active"						, m_npcViewPoint.x		, m_npcViewPoint.y+rp.y	, 100, 22 );		ADD_POINT( rp, 0, 0 );
	m_toolUI.AddCheckBox( IDC_GAGE_BAR_CHECK	, L"GageBar"					, m_npcViewPoint.x+rp.x	, m_npcViewPoint.y+rp.y	, 100, 22 );		ADD_POINT( rp, 0, 30 );
	m_toolUI.AddStatic( IDC_STATIC_NPC_5		, L"Rate : "					, m_npcViewPoint.x		, m_npcViewPoint.y+rp.y	, 100, 30 );		ADD_POINT( rp, 0, 0 );
	m_toolUI.AddEditBox( IDC_RATE_EDIT			, L""							, m_npcViewPoint.x+rp.x	, m_npcViewPoint.y+rp.y	, 100, 30 );		ADD_POINT( rp, 0, 30 );
	((KX2DTEditBox*)m_toolUI.GetEditBox( IDC_RATE_EDIT ))->SetIsOnlyNumber( true );		
	m_toolUI.AddStatic( IDC_STATIC_NPC_KEY_CODE	, L"Key Code : "				, m_npcViewPoint.x		, m_npcViewPoint.y+rp.y	, 100, 30 );		ADD_POINT( rp, 0, 0 );
	m_toolUI.AddEditBox( IDC_NPC_KEY_CODE_EDIT	, L""							, m_npcViewPoint.x+rp.x	, m_npcViewPoint.y+rp.y	, 100, 30 );		ADD_POINT( rp, 0, 30 );
	((KX2DTEditBox*)m_toolUI.GetEditBox( IDC_NPC_KEY_CODE_EDIT ))->SetIsOnlyNumber( true );
	m_toolUI.AddCheckBox( IDC_FOCUS_CAMERA_CHECK, L"Focus Camera"				, m_npcViewPoint.x		, m_npcViewPoint.y+rp.y	, 200, 22 );		ADD_POINT( rp, 0, 22 );
	m_toolUI.AddCheckBox( IDC_HAVE_BOSS_GAGE_CHECK, L"Have Boss Gage"			, m_npcViewPoint.x		, m_npcViewPoint.y+rp.y	, 200, 22 );		ADD_POINT( rp, 0, 22 );
	m_toolUI.AddCheckBox( IDC_SUB_NPC_CHECK		, L"Sub NPC"					, m_npcViewPoint.x		, m_npcViewPoint.y+rp.y	, 200, 22 );		ADD_POINT( rp, 0, 22 );
	m_toolUI.AddCheckBox( IDC_SIEGE_MODE_CHECK	, L"Siege Mode"					, m_npcViewPoint.x		, m_npcViewPoint.y+rp.y	, 100, 22 );		ADD_POINT( rp, 0, 22 );
	m_toolUI.AddCheckBox( IDC_IS_RIGHT_CHECK	, L"Is Right"					, m_npcViewPoint.x		, m_npcViewPoint.y+rp.y , 100, 22 );		

	// Trigger
	SET_POINT( rp, 0, 0 );
	m_toolUI.AddStatic( IDC_STATIC_TRIGGER		, L"--------[ Trigger ]--------", m_triggerPoint.x		, m_triggerPoint.y		, 200, 22 );		ADD_POINT( rp, 0, 25 );
	m_toolUI.AddListBox( IDC_TRIGGER_LIST		,								  m_triggerPoint.x		, m_triggerPoint.y+rp.y	, 200, 100 );		ADD_POINT( rp, 0, 100 );
	m_toolUI.AddButton( IDC_ADD_TRIGGER_BUTTON	, L"Add Trigger"				, m_triggerPoint.x		, m_triggerPoint.y+rp.y	, 100, 22 );		ADD_POINT( rp, 100, 0 );
	m_toolUI.AddButton( IDC_DEL_TRIGGER_BUTTON	, L"Del Trigger"				, m_triggerPoint.x+rp.x	, m_triggerPoint.y+rp.y	, 100, 22 );		ADD_POINT( rp, -100, 22 );
	m_toolUI.AddCheckBox( IDC_HOST_CHECK		, L"Host"						, m_triggerPoint.x		, m_triggerPoint.y+rp.y	, 100, 22 );		ADD_POINT( rp, 100, 0 );
	m_toolUI.AddCheckBox( IDC_ONE_TIME_CHECK	, L"One Time"					, m_triggerPoint.x+rp.x	, m_triggerPoint.y+rp.y	, 100, 22 );		ADD_POINT( rp, 0, 30 );
	m_toolUI.AddStatic( IDC_STATIC_TRIGGER_1	, L"Interval :"					, m_triggerPoint.x		, m_triggerPoint.y+rp.y , 100, 30 );		ADD_POINT( rp, 0, 0 );
	m_toolUI.AddEditBox( IDC_INTERVAL_EDIT		, L""							, m_triggerPoint.x+rp.x , m_triggerPoint.y+rp.y , 100, 30 );		ADD_POINT( rp, 0, 30 );
	((KX2DTEditBox*)m_toolUI.GetEditBox( IDC_INTERVAL_EDIT ))->SetIsOnlyNumber( true );
	m_toolUI.AddButton( IDC_CONDITION_FUNC_BUTTON, L"ConditionFunc"				, m_triggerPoint.x		, m_triggerPoint.y+rp.y , 100, 22 );		ADD_POINT( rp, 0, 0 );
	m_toolUI.AddButton( IDC_REACT_FUNG_BUTTON	, L"ReactFung"					, m_triggerPoint.x+rp.x , m_triggerPoint.y+rp.y , 100, 22 );		ADD_POINT( rp, -200, 20 );
	m_toolUI.AddEditBox( IDC_FUNCTION_VIEWER_EDIT, L""							, m_triggerPoint.x+rp.x , m_triggerPoint.y+rp.y , 300, 30 );

	// SubStageInfo
	SET_POINT( rp, 0, 0 );
	m_toolUI.AddStatic( IDC_STATIC_SUB_STAGE_INFO, L"-----[ SubStageInfo ]-----", m_subStagePoint.x		, m_subStagePoint.y		, 200, 22 );		ADD_POINT( rp, 0, 25 );
	m_toolUI.AddStatic( IDC_STATIC_SUB_STAGE_INFO_1, L"Start Line Set : "		, m_subStagePoint.x		, m_subStagePoint.y+rp.y, 100, 30 );		ADD_POINT( rp, 100, 0 );
	m_toolUI.AddEditBox( IDC_START_LINE_SET_EDIT, L""							, m_subStagePoint.x+rp.x, m_subStagePoint.y+rp.y, 100, 30 );		ADD_POINT( rp, 0, 30 );
	((KX2DTEditBox*)m_toolUI.GetEditBox( IDC_START_LINE_SET_EDIT ))->SetIsOnlyNumber( true );
	((KX2DTEditBox*)m_toolUI.GetEditBox( IDC_START_LINE_SET_EDIT ))->SetMinusSign( true );
	m_toolUI.AddStatic( IDC_STATIC_SUB_STAGE_INFO_2, L"Main Line Set : "		, m_subStagePoint.x		, m_subStagePoint.y+rp.y, 100, 30 );		ADD_POINT( rp, 0, 0 );
	m_toolUI.AddEditBox( IDC_MAIN_LINE_SET_EDIT	, L""							, m_subStagePoint.x+rp.x, m_subStagePoint.y+rp.y, 100, 30 );		ADD_POINT( rp, 0, 30 );
	((KX2DTEditBox*)m_toolUI.GetEditBox( IDC_MAIN_LINE_SET_EDIT ))->SetIsOnlyNumber( true );
	((KX2DTEditBox*)m_toolUI.GetEditBox( IDC_MAIN_LINE_SET_EDIT ))->SetMinusSign( true );
	m_toolUI.AddStatic( IDC_STATIC_SUB_STAGE_INFO_3, L"End Line Set : "			, m_subStagePoint.x		, m_subStagePoint.y+rp.y, 100, 30 );		ADD_POINT( rp, 0, 0 );
	m_toolUI.AddEditBox( IDC_END_LINE_SET_EDIT	, L""							, m_subStagePoint.x+rp.x, m_subStagePoint.y+rp.y, 100, 30 );		ADD_POINT( rp, 0, 30 );
	((KX2DTEditBox*)m_toolUI.GetEditBox( IDC_END_LINE_SET_EDIT ))->SetIsOnlyNumber( true );
	((KX2DTEditBox*)m_toolUI.GetEditBox( IDC_END_LINE_SET_EDIT ))->SetMinusSign( true );
	m_toolUI.AddStatic( IDC_STATIC_SUB_STAGE_INFO_4, L"Go Type : "				, m_subStagePoint.x		, m_subStagePoint.y+rp.y, 100, 30 );		ADD_POINT( rp, 0, 0 );
	CDXUTComboBox *pCombo = NULL;	
	m_toolUI.AddComboBox( IDC_GO_TYPE_COMBO										, m_subStagePoint.x+rp.x, m_subStagePoint.y+rp.y, 100, 30, 0, false, &pCombo );
	{
		pCombo->SetDropHeight( 50 );
		pCombo->AddItem( L"NONE", (LPVOID)"GT_NONE" );
		pCombo->AddItem( L"RIGHT", (LPVOID)"GT_RIGHT" );
		pCombo->AddItem( L"LEFT", (LPVOID)"GT_LEFT" );
		pCombo->AddItem( L"UP" , (LPVOID)"GT_UP" );
		pCombo->AddItem( L"DOWN", (LPVOID)"GT_DOWN" );
	}
	
	// Clear Condition
	m_LeftToolUI.Init( &g_DialogResourceManager );
	m_LeftToolUI.SetCallback( OnLeftToolUIEvent, this );
	m_LeftToolUI.SetSize( 340, 300 );
	m_LeftToolUI.SetFont(0, L"굴림", 12, 12);

	SET_POINT( rp, 0, 0 );
	m_LeftToolUI.AddStatic( IDC_STATIC_CLEAR_CONDITION_INFO, L"----[ ClearCondition ]----", m_clearConPoint.x, m_clearConPoint.y	, 200, 22 );		ADD_POINT( rp, 0, 25 );
	m_LeftToolUI.AddListBox( IDC_CLEAR_CONDITION_LIST,								  m_clearConPoint.x		, m_clearConPoint.y+rp.y, 200, 100 );		ADD_POINT( rp, 0, 100 );
	m_LeftToolUI.AddButton( IDC_ADD_CLEAR_CONDITION_BUTTON, L"Add Condition"		, m_clearConPoint.x		, m_clearConPoint.y+rp.y, 100, 22 );		ADD_POINT( rp, 100, 0 );
	m_LeftToolUI.AddButton( IDC_DEL_CLEAR_CONDITION_BUTTON, L"Del Condition"		, m_clearConPoint.x+rp.x, m_clearConPoint.y+rp.y, 100, 22 );		ADD_POINT( rp, -100, 22 );
	m_LeftToolUI.AddComboBox( IDC_CLEAR_CONDITION_COMBO								, m_clearConPoint.x+rp.x, m_clearConPoint.y+rp.y, 200, 30, 0, false, &pCombo ); ADD_POINT( rp, 0, 30 );
	{
		pCombo->SetDropHeight( 30 );
		pCombo->AddItem( L"KILL_ALL_ACTIVE_NPC", (LPVOID)"CC_KILL_ALL_ACTIVE_NPC" );
		pCombo->AddItem( L"KILL_KEYCODE_NPC", (LPVOID)"CC_KILL_KEYCODE_NPC" );
		pCombo->AddItem( L"KILL_KEYCODE_NPC_ANY", (LPVOID)"CC_KILL_KEYCODE_NPC_ANY" );
		pCombo->AddItem( L"KILL_NPC_TYPE", (LPVOID)"CC_KILL_NPC_TYPE" );
	}
	m_LeftToolUI.AddListBox( IDC_CONDITION_ELEMENT_LIST,							  m_clearConPoint.x		, m_clearConPoint.y+rp.y, 200, 100 );		ADD_POINT( rp, 0, 100 );
	m_LeftToolUI.AddComboBox( IDC_CONDITION_ELEMENT_COMBO							, m_clearConPoint.x+rp.x, m_clearConPoint.y+rp.y, 200, 30, 0, false, &pCombo ); ADD_POINT( rp, 0, 30 );
	{
		pCombo->SetDropHeight( 50 );		
	}
	m_LeftToolUI.AddButton( IDC_ADD_CONDITION_ELEMENT, L"Add Key Code"				, m_clearConPoint.x		, m_clearConPoint.y+rp.y, 100, 22 );		ADD_POINT( rp, 100, 0 );
	m_LeftToolUI.AddButton( IDC_DEL_CONDITION_ELEMENT, L"Del Key Code"				, m_clearConPoint.x+rp.x, m_clearConPoint.y+rp.y, 100, 22 );		ADD_POINT( rp, -100, 22 );
	m_LeftToolUI.AddListBox( IDC_NEXT_STAGE_LIST	 ,								  m_clearConPoint.x		, m_clearConPoint.y+rp.y, 200, 100 );		ADD_POINT( rp, 0, 100 );
	m_LeftToolUI.AddButton( IDC_ADD_NEXT_STAGE_BUTTON, L"Add Stage"					, m_clearConPoint.x		, m_clearConPoint.y+rp.y, 100, 22 );		ADD_POINT( rp, 100, 0 );
	m_LeftToolUI.AddButton( IDC_DEL_NEXT_STAGE_BUTTON, L"Del Stage"					, m_clearConPoint.x+rp.x, m_clearConPoint.y+rp.y, 100, 22 );		ADD_POINT( rp, -100, 22 );
	m_LeftToolUI.AddComboBox( IDC_CLEAR_TYPE_COMBO									, m_clearConPoint.x+rp.x, m_clearConPoint.y+rp.y, 200, 30, 0, false, &pCombo ); ADD_POINT( rp, 0, 30 );
	{
		pCombo->SetDropHeight( 30 );
		pCombo->AddItem( L"SUB_STAGE", (LPVOID)"CT_SUB_STAGE" );
		pCombo->AddItem( L"STAGE", (LPVOID)"CT_STAGE" );
		pCombo->AddItem( L"GAME", (LPVOID)"CT_GAME" );
	}
	m_LeftToolUI.AddStatic( IDC_STATIC_CLEAR_CONDITION_STAGE, L"Stage : "			, m_clearConPoint.x		, m_clearConPoint.y+rp.y, 100, 30 );		ADD_POINT( rp, 100, 0 );
	m_LeftToolUI.AddComboBox( IDC_CLEAR_CONDITION_STAGE_COMBO						, m_clearConPoint.x+rp.x, m_clearConPoint.y+rp.y, 100, 30, 0, false, &pCombo ); ADD_POINT( rp, -100, 30 );
	{
		pCombo->SetDropHeight( 50 );
	}
	m_LeftToolUI.AddStatic( IDC_STATIC_CLEAR_CONDITION_SUBSTAGE, L"SubStage : "		, m_clearConPoint.x		, m_clearConPoint.y+rp.y, 100, 30 );		ADD_POINT( rp, 100, 0 );
	m_LeftToolUI.AddComboBox( IDC_CLEAR_CONDITION_SUBSTAGE_COMBO					, m_clearConPoint.x+rp.x, m_clearConPoint.y+rp.y, 100, 30, 0, false, &pCombo ); ADD_POINT( rp, -100, 30 );
	{
		pCombo->SetDropHeight( 50 );
	}
	m_LeftToolUI.AddStatic( IDC_STATIC_CLEAR_CONDITION_RATE, L"Stage Rate : "		, m_clearConPoint.x		, m_clearConPoint.y+rp.y, 100, 30 );		ADD_POINT( rp, 100, 0 );
	m_LeftToolUI.AddEditBox( IDC_CLEAR_CONDITION_RATE_EDIT, L""						, m_clearConPoint.x+rp.x, m_clearConPoint.y+rp.y, 100, 30 );
	((KX2DTEditBox*)m_LeftToolUI.GetEditBox( IDC_CLEAR_CONDITION_RATE_EDIT ))->SetIsOnlyNumber( true );

	m_toolUI.SetVisible( false );
	m_LeftToolUI.SetVisible( false );

	////////////////////////////////////////////////////////////////// MenuUI
	m_menuUI.Init( &g_DialogResourceManager );
	m_menuUI.SetCallback( OnMenuUIEvent, this );
	m_menuUI.SetSize( 340, 300 );
	m_menuUI.SetFont(0, L"굴림", 12, 12);
	POINT menuPoint;
	
	SET_POINT( menuPoint, 10, 10 );
	SET_POINT( rp, 0, 0 );
	m_menuUI.AddButton( IDC_GO_TO_MAIN_BUTTON	, L"GoToMain"					, menuPoint.x			, menuPoint.y			, 100, 22 );		ADD_POINT( rp, 105, 0 );
	m_menuUI.AddButton( IDC_LOAD_LUA_BUTTON		, L"Load Lua"					, menuPoint.x+rp.x		, menuPoint.y			, 100, 22 );		ADD_POINT( rp, 100, 0 );
	m_menuUI.AddButton( IDC_SAVE_LUA_BUTTON		, L"Save Lua"					, menuPoint.x+rp.x		, menuPoint.y			, 100, 22 );		ADD_POINT( rp, 100, 0 );
	m_menuUI.AddCheckBox( IDC_STAGE_INFO_VISIBLE_CHECK, L"StageInfo"			, menuPoint.x+rp.x		, menuPoint.y			, 100, 22 );		ADD_POINT( rp, 100, 0 );
	m_menuUI.AddCheckBox( IDC_CURTAIN_VISIBLE_CHECK, L"Curtain"					, menuPoint.x+rp.x		, menuPoint.y			, 100, 22 );		ADD_POINT( rp, 100, 0 );
	m_menuUI.AddCheckBox( IDC_NPC_VISIBLE_CHECK	, L"NPC"						, menuPoint.x+rp.x		, menuPoint.y			, 100, 22 );		ADD_POINT( rp, 100, 0 );
	m_menuUI.AddCheckBox( IDC_TRIGGER_VISIBLE_CHECK, L"Trigger"					, menuPoint.x+rp.x		, menuPoint.y			, 100, 22 );		ADD_POINT( rp, 100, 0 );
	m_menuUI.AddCheckBox( IDC_SUBSTAGE_VISIBLE_CHECK, L"SubStage"				, menuPoint.x+rp.x		, menuPoint.y			, 100, 22 );		ADD_POINT( rp, -400, 22);

	m_menuUI.AddCheckBox( IDC_CLEAR_CONDITION_VISIBLE_CHECK, L"ClearCon"		, menuPoint.x+rp.x		, menuPoint.y+rp.y		, 100, 22 );		ADD_POINT( rp, 100, 0);

	m_menuUI.AddCheckBox( IDC_TOOL_UI_SHOW_CHECK, L"Tool UI"					, menuPoint.x+rp.x		, menuPoint.y+rp.y		, 100, 22 );		ADD_POINT( rp, 100, 0 );
	m_menuUI.AddCheckBox( IDC_MAP_UI_SHOW_CHECK,  L"Map UI"						, menuPoint.x+rp.x		, menuPoint.y+rp.y		, 100, 22 );		ADD_POINT( rp, 100, 0 );
	m_menuUI.AddCheckBox( IDC_WORLD_SHOW_CHECK	, L"World"						, menuPoint.x+rp.x		, menuPoint.y+rp.y		, 100, 22 );		ADD_POINT( rp, 100, 0 );
	m_menuUI.AddCheckBox( IDC_LINE_MAP_SHOW_CHECK, L"Line Map"					, menuPoint.x+rp.x		, menuPoint.y+rp.y		, 100, 22 );		ADD_POINT( rp, -400, 22 );
	m_menuUI.AddCheckBox( IDC_NPC_SHOW_CHECK	, L"NPC"						, menuPoint.x+rp.x		, menuPoint.y+rp.y		, 100, 22 );		ADD_POINT( rp, 100, 0 );
	m_menuUI.AddCheckBox( IDC_CURTAIN_SHOW_CHECK, L"Curtain"					, menuPoint.x+rp.x		, menuPoint.y+rp.y		, 100, 22 );		ADD_POINT( rp, 100, 0 );
	m_menuUI.AddCheckBox( IDC_START_POS_SHOW_CHECK,L"StartPos"					, menuPoint.x+rp.x		, menuPoint.y+rp.y		, 100, 22 );		ADD_POINT( rp, 100, 0 );
	m_menuUI.AddCheckBox( IDC_LINE_MAP_NUMBER_SHOW_CHECK,L"LineNumber"			, menuPoint.x+rp.x		, menuPoint.y+rp.y		, 100, 22 );		ADD_POINT( rp, 100, 0 );	
	m_menuUI.AddEditBox( IDC_LINE_NUMBER_EDIT	,L""							, menuPoint.x+rp.x		, menuPoint.y+rp.y		, 100, 30 );		ADD_POINT( rp, -400, 22 );	
	((KX2DTEditBox*)m_menuUI.GetEditBox( IDC_LINE_NUMBER_EDIT ))->SetIsOnlyNumber( true );
	m_menuUI.AddCheckBox( IDC_AUTO_SAVE_CHECK	, L"AutoSave"					, menuPoint.x+rp.x		, menuPoint.y+rp.y		, 100, 22 );		ADD_POINT( rp, 100, 0 );
	m_menuUI.AddCheckBox( IDC_NPC_COLOR_CHECK	, L"NPC Color"					, menuPoint.x+rp.x		, menuPoint.y+rp.y		, 100, 22 );		ADD_POINT( rp, 100, 0 );
	m_menuUI.AddCheckBox( IDC_SUMMARY_UI_CHECK	, L"Summary"					, menuPoint.x+rp.x		, menuPoint.y+rp.y		, 100, 22 );		ADD_POINT( rp, 100, 0 );
	m_menuUI.AddCheckBox( IDC_ONLY_SUBS_NPC_CHECK,L"SubSt NPC"					, menuPoint.x+rp.x		, menuPoint.y+rp.y		, 100, 22 );		ADD_POINT( rp, -300, 22 );

	m_menuUI.AddCheckBox( IDC_NEAR_RANGE_CHECK	, L"Near Range"					, menuPoint.x+rp.x		, menuPoint.y+rp.y		, 100, 22 );		ADD_POINT( rp, 100, 0 );
	m_menuUI.AddCheckBox( IDC_TARGET_RANGE_CHECK, L"Target Range"				, menuPoint.x+rp.x		, menuPoint.y+rp.y		, 100, 22 );		ADD_POINT( rp, 100, 0 );
	m_menuUI.AddCheckBox( IDC_LOST_RANGE_CHECK	, L"Lost Range"					, menuPoint.x+rp.x		, menuPoint.y+rp.y		, 100, 22 );		ADD_POINT( rp, 100, 0 );
	m_menuUI.AddCheckBox( IDC_ONLY_SELECT_CHECK	, L"Only Select"				, menuPoint.x+rp.x		, menuPoint.y+rp.y		, 100, 22 );		ADD_POINT( rp, 100, 0 );

	m_menuUI.AddEditBox( IDC_NOW_EDITING_DUNGEON_EDIT,L""						, menuPoint.x			, menuPoint.y+22		, 300, 30 );	
	m_menuUI.AddEditBox( IDC_NOW_EDITING_STAGE_EDIT,L""							, menuPoint.x			, menuPoint.y+52		, 300, 30 );			

	/////////////////////////////////////////////////////////////////// MapUI
	m_mapUI.Init( &g_DialogResourceManager );
	m_mapUI.SetCallback( OnMapUIEvent, this );
	m_mapUI.SetSize( 340, 300 );
	m_mapUI.SetFont( 0, L"굴림", 12, 12 );


	SET_POINT( rp, 0, 0 );
	m_mapUI.AddListBox( IDC_MAP_STAGE_LIST		,								  m_mapUIPoint.x		, m_mapUIPoint.y		, 300, 200 );		ADD_POINT( rp, 310, 0 );
	m_mapUI.AddListBox( IDC_MAP_SUB_STAGE_LIST  ,								  m_mapUIPoint.x+rp.x	, m_mapUIPoint.y		, 300, 200 );		ADD_POINT( rp, -310, 630 );
	m_mapUI.AddStatic( IDC_STATIC_MAP_X			, L"Map X : "					, m_mapUIPoint.x+rp.x	, m_mapUIPoint.y+rp.y	, 100, 30 );		ADD_POINT( rp, 100, 0 );
	m_mapUI.AddEditBox( IDC_MAP_X_EDIT			, L""							, m_mapUIPoint.x+rp.x	, m_mapUIPoint.y+rp.y	, 100, 30 );		ADD_POINT( rp, 100, 0 );
	((KX2DTEditBox*)m_mapUI.GetEditBox( IDC_MAP_X_EDIT ))->SetIsOnlyNumber( true );
	((KX2DTEditBox*)m_mapUI.GetEditBox( IDC_MAP_X_EDIT ))->SetMinusSign( true );
	m_mapUI.AddStatic( IDC_STATIC_MAP_Y			, L"Map Y : "					, m_mapUIPoint.x+rp.x	, m_mapUIPoint.y+rp.y	, 100, 30 );		ADD_POINT( rp, 100, 0 );
	m_mapUI.AddEditBox( IDC_MAP_Y_EDIT			, L""							, m_mapUIPoint.x+rp.x	, m_mapUIPoint.y+rp.y	, 100, 30 );		ADD_POINT( rp, 110, 0 );
	((KX2DTEditBox*)m_mapUI.GetEditBox( IDC_MAP_Y_EDIT ))->SetIsOnlyNumber( true );
	((KX2DTEditBox*)m_mapUI.GetEditBox( IDC_MAP_Y_EDIT ))->SetMinusSign( true );
	m_mapUI.AddCheckBox( IDC_MAP_BOSS_CHECK		, L"Boss Map"					, m_mapUIPoint.x+rp.x	, m_mapUIPoint.y+rp.y	, 100, 30 );		ADD_POINT( rp, 100, 0 );
	m_mapUI.AddCheckBox( IDC_IS_NEXT_RANDOM_STAGE_CHECK, L"Is Next Random Stage", m_mapUIPoint.x+rp.x	, m_mapUIPoint.y+rp.y	, 200, 30 );		ADD_POINT( rp, -410, 30 );
	//m_mapUI.AddEditBox( IDC_TEXTURE_FILE_NAME_EDIT, L""						, m_mapUIPoint.x+rp.x	, m_mapUIPoint.y+rp.y	, 300, 30 );		ADD_POINT( rp, 300, 0 );
	//m_mapUI.AddButton( IDC_TEXTURE_LOAD_BUTTON	, L"Texture Load"			, m_mapUIPoint.x+rp.x	, m_mapUIPoint.y+rp.y	, 100, 30 );		ADD_POINT( rp, 100, 0 );
	m_mapUI.AddCheckBox( IDC_MAP_ALIGN_CHECK	, L"Align"						, m_mapUIPoint.x+rp.x	, m_mapUIPoint.y+rp.y	, 100, 30 );

	m_mapUI.SetVisible( false );

	/////////////////////////////////////////////////////////////////// MapUI
	m_summaryUI.Init( &g_DialogResourceManager );
	m_summaryUI.SetCallback( OnSummaryUIEvent, this );
	m_summaryUI.SetSize( 340, 300 );
	m_summaryUI.SetFont( 0, L"굴림", 12, 12 );

	SET_POINT( rp, 0, 0 );
	m_summaryUI.AddStatic( IDC_STATIC_SUMMARY	, L"< Summary >"				, m_summaryPoint.x		, m_summaryPoint.y		, 600, 30 );		ADD_POINT( rp, 0, 30 );
	LPCTSTR wstrColumn = L"|    NPC ID    |  NPC Count  |      EXP      |       ED       |                          NPC Name                        |";
	m_summaryUI.AddStatic( IDC_STATIC_COLUMN	, wstrColumn					, m_summaryPoint.x+rp.x , m_summaryPoint.y+rp.y	, 600, 30 );		ADD_POINT( rp, 0, 30 );	
	m_summaryUI.AddListBox( IDC_SUMMARY_LIST	,								  m_summaryPoint.x+rp.x , m_summaryPoint.y+rp.y	, 600, 300 );		ADD_POINT( rp, 0, 300 );
	m_summaryUI.AddStatic( IDC_STATIC_TOTAL_COUNT, L"합  계 : "					, m_summaryPoint.x+rp.x , m_summaryPoint.y+rp.y	, 50, 30 );			ADD_POINT( rp, 80, 0 );
	m_summaryUI.AddEditBox( IDC_NPC_TOTAL_EDIT	, L""							, m_summaryPoint.x+rp.x , m_summaryPoint.y+rp.y	, 70, 30 );			ADD_POINT( rp, 80, 0 );
	m_summaryUI.AddEditBox( IDC_EXP_TOTAL_EDIT	, L""							, m_summaryPoint.x+rp.x , m_summaryPoint.y+rp.y	, 70, 30 );			ADD_POINT( rp, 80, 0 );
	m_summaryUI.AddEditBox( IDC_ED_TOTAL_EDIT	, L""							, m_summaryPoint.x+rp.x , m_summaryPoint.y+rp.y	, 70, 30 );

	m_summaryUI.SetVisible( false );
}

KX2DungeonTool::KX2DungeonTool(void)
{	
	g_pDungeonTool		= this;
	m_wsSaveFileName	= L"Test.lua";
	m_pEditingMiniMap	= NULL;
	m_pDungeonMapUI		= NULL;
	m_pWorld			= NULL;
	m_pDungeonListBox	= NULL;
	m_pStageListBox		= NULL;
	m_pEditingDungeon	= NULL;
	m_pEditingStage		= NULL;
	m_pEditingSubStage	= NULL;
	m_pEditingCurtain	= NULL;
	m_pEditingNpc		= NULL;
	m_pEditingTrigger	= NULL;
	m_pEditingClearCon  = NULL;
	m_pEditingNextStage = NULL;
	m_bIsConditionFunc  = true;
	m_bPickMiniMap		= false;

	m_bRenderingNPC		= false;
	m_bRenderingCurtain = false;	
	m_bRenderingStartPos = false;
	m_bRenderingLineMapNumber = false;
	m_bRenderingTargetRange = false;
	m_bOnlySelect		= false;
	m_bOnlySubStageNPC  = false;

	m_bNpcColor			= false;
	m_bUIWideMode		= false;
	m_bScreenShotMode	= false;

	ZeroMemory( &m_backupSubStage	, sizeof( CX2DungeonSubStage::SubStageData ) );
	ZeroMemory( &m_backupCurtain	, sizeof( CX2DungeonSubStage::CurtainData ) );
	ZeroMemory( &m_backupNpc		, sizeof( CX2DungeonSubStage::NPCData ) );
	ZeroMemory( &m_backupTrigger	, sizeof( CX2DungeonSubStage::Trigger ) );

	// 던전툴 옵션 읽기
	LoadDungeonToolOption();

	// UI 설정
	InitUI();

	// 스크립트 로드
	RegisterLuaBind();
	LoadDungeonScriptFile();
	
	// 메쉬 로드
	m_pMatrix = new CKTDGMatrix( g_pKTDXApp->GetDevice() );
	m_pXSkinMesh = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( L"Motion_Elsword_SwordMan.X" );
	m_pXCurtainMesh = g_pKTDXApp->GetDeviceManager()->OpenXMesh( L"dummyCurtain.x" );
	m_pXStartPosMesh = g_pKTDXApp->GetDeviceManager()->OpenXMesh( L"sphere0.x" );
	
	m_pXMeshSphere		= g_pKTDXApp->GetDeviceManager()->OpenXMesh( L"TargetRange.X" );	
	m_pMatrixSphere		= new CKTDGMatrix( g_pKTDXApp->GetDevice() );

	// ready 3d text font 
	WCHAR wszTemp[32];
	for( int i=0; i<10; i++ )
	{
		m_pMesh3DText[i] = NULL;		
		StringCchPrintf( wszTemp, sizeof(wszTemp), L"%d", i ); // 숫자
		CreateD3DXTextMesh( g_pKTDXApp->GetDevice(), &m_pMesh3DText[i], L"Arial", 12, FALSE, FALSE, wszTemp );
	}

	m_pMesh3DText[10] = NULL;
	swprintf( wszTemp, sizeof(wszTemp)/sizeof(WCHAR)-1, L"%s", L"-" ); // '-' 기호
	CreateD3DXTextMesh( g_pKTDXApp->GetDevice(), &m_pMesh3DText[10], L"Arial", 12, FALSE, FALSE, wszTemp );

	m_pMesh3DText[11] = NULL;
	swprintf( wszTemp, sizeof(wszTemp)/sizeof(WCHAR)-1, L"%s", L"*" ); // '*' 기호
	CreateD3DXTextMesh( g_pKTDXApp->GetDevice(), &m_pMesh3DText[11], L"Arial", 12, FALSE, FALSE, wszTemp );

	// create vertex buffer
	HRESULT	hr;
	m_nVBHighlightedSegmentSize = 1024;
		
	if(FAILED(hr = g_pKTDXApp->GetDevice()->CreateVertexBuffer( m_nVBHighlightedSegmentSize * sizeof(LINE_MAP_VERTEX), 
		D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, D3DFVF_LINE_MAP_VERTEX, 
		D3DPOOL_SYSTEMMEM, &m_pVBHighlightedSegment, NULL) ))
	{
		;
	}

	// 카메라 설정
	g_pKTDXApp->GetDGManager()->GetCamera().Point( 0,0,-1000, 0,0,0 );
	g_pKTDXApp->GetDGManager()->GetCamera().UpdateCamera( 1.0f );

	// 백업 타이머 설정
	::SetTimer( g_pKTDXApp->GetHWND(), 1, m_iAutoSaveTime, (TIMERPROC)TimerBackUpScript );
	m_menuUI.GetCheckBox( IDC_AUTO_SAVE_CHECK )->SetChecked( true );
}

KX2DungeonTool::~KX2DungeonTool(void)
{
	::KillTimer( g_pKTDXApp->GetHWND(), 1 );

	SAFE_DELETE( m_pMatrix );
	SAFE_DELETE( m_pDungeonMapUI );
	SAFE_DELETE( m_pEditingDungeon );
	SAFE_DELETE( m_pWorld );	

	SAFE_CLOSE( m_pXSkinMesh );
	SAFE_CLOSE( m_pXCurtainMesh );
	SAFE_CLOSE( m_pXStartPosMesh ); 
	SAFE_RELEASE( m_pVBHighlightedSegment );
	DeleteAllSkinMeshAndSkinAnim();

	g_pKTDXApp->GetDeviceManager()->CloseDevice( m_pXMeshSphere->GetDeviceID() );
	SAFE_DELETE( m_pMatrixSphere );

	for( int i=0; i<12 ; i++ )
	{
		SAFE_RELEASE( m_pMesh3DText[i] );
		m_pMesh3DText[i] = NULL;
	}
}

void KX2DungeonTool::RegisterLuaBind()
{
    lua_State* L = g_pKTDXApp->GetLuaBinder()->GetLuaState();

	lua_tinker::class_add<CX2UnitManager>(L, "CX2UnitManager");
	lua_tinker::class_def<CX2UnitManager>(L, "AddUnitTemplet",			CX2UnitManager::AddUnitTemplet_LUA);
	lua_tinker::class_def<CX2UnitManager>(L, "AddNPCTemplet",			CX2UnitManager::AddNPCTemplet_LUA);
		

	lua_tinker::class_add<CX2DungeonManager>(L, "CX2DungeonManager");
	lua_tinker::class_def<CX2DungeonManager>(L, "AddDungeonData",			CX2DungeonManager::AddDungeonData_LUA);
		

	lua_tinker::class_add<KX2DTWorldManager>(L, "KX2DTWorldManager");
	lua_tinker::class_inh<KX2DTWorldManager, CX2DungeonManager>(L);
	lua_tinker::class_def<KX2DTWorldManager>(L, "AddWorldData",			CX2WorldManager::AddWorldData_LUA);

	lua_tinker::class_add<KX2DTWorld>(L, "KX2DTWorld");
	lua_tinker::class_inh<KX2DTWorld, CX2World>(L);
	lua_tinker::class_def<KX2DTWorld>(L, "SetOriginColor_LUA",		CX2World::SetOriginColor_LUA);
	lua_tinker::class_def<KX2DTWorld>(L, "SetLightColor_LUA",		CX2World::SetLightColor_LUA);
	lua_tinker::class_def<KX2DTWorld>(L, "SetLightPos",				CX2World::SetLightPos);
	lua_tinker::class_def<KX2DTWorld>(L, "SetLineMap",				CX2World::SetLineMap_LUA);
	lua_tinker::class_def<KX2DTWorld>(L, "SetBGM",					CX2World::SetBGM_LUA);
	lua_tinker::class_def<KX2DTWorld>(L, "AddEffectBGM_LUA",		CX2World::AddEffectBGM_LUA);
	lua_tinker::class_def<KX2DTWorld>(L, "CreateSkyDome",			CX2World::CreateSkyDome);
	lua_tinker::class_def<KX2DTWorld>(L, "CreateObjectMesh",		CX2World::CreateObjectMesh);
	lua_tinker::class_def<KX2DTWorld>(L, "CreateLensFlare",			CX2World::CreateLensFlare);
	lua_tinker::class_def<KX2DTWorld>(L, "CreateObjectSkinMesh",	CX2World::CreateObjectSkinMesh);
	lua_tinker::class_def<KX2DTWorld>(L, "CreateObjectLightFlow",	CX2World::CreateObjectLightFlow);
	lua_tinker::class_def<KX2DTWorld>(L, "CreateObjectParticle",	CX2World::CreateObjectParticle);
	lua_tinker::class_def<KX2DTWorld>(L, "SetWorldColor",			CX2World::SetWorldColor);

    lua_tinker::class_add<CX2WorldObject>( L, "CX2WorldObject" );
    lua_tinker::class_inh<CX2WorldObject, CKTDGObject>( L );
    lua_tinker::class_def<CX2WorldObject>( L, "SetName",				CX2WorldObject::SetName );
    lua_tinker::class_def<CX2WorldObject>( L, "SetPos",					CX2WorldObject::SetPos );
    lua_tinker::class_def<CX2WorldObject>( L, "SetAngleDegree",			CX2WorldObject::SetAngleDegree );
    lua_tinker::class_def<CX2WorldObject>( L, "SetColorFactor",			CX2WorldObject::SetColorFactor );
    lua_tinker::class_def<CX2WorldObject>( L, "SetScale",				CX2WorldObject::SetScale );		
    lua_tinker::class_def<CX2WorldObject>( L, "SetUseWorldColor",		CX2WorldObject::SetUseWorldColor );		


    lua_tinker::class_add<CX2WorldObjectMesh>( L, "CX2WorldObjectMesh" );
    lua_tinker::class_inh<CX2WorldObjectMesh, CX2WorldObject>( L );
    lua_tinker::class_def<CX2WorldObjectMesh>( L, "SetXMesh",				CX2WorldObjectMesh::SetXMesh_LUA );
    lua_tinker::class_def<CX2WorldObjectMesh>( L, "SetXMeshLOD",			CX2WorldObjectMesh::SetXMeshLOD_LUA );
    lua_tinker::class_def<CX2WorldObjectMesh>( L, "SetMultiTexXET",			CX2WorldObjectMesh::SetMultiTexXET_LUA );
    lua_tinker::class_def<CX2WorldObjectMesh>( L, "SetChangeTexXET",		CX2WorldObjectMesh::SetChangeTexXET_LUA );
    lua_tinker::class_def<CX2WorldObjectMesh>( L, "SetCanHide",				CX2WorldObjectMesh::SetCanHide );
    lua_tinker::class_def<CX2WorldObjectMesh>( L, "SetBillboardType",		CX2WorldObjectMesh::SetBillboardType );
    lua_tinker::class_def<CX2WorldObjectMesh>( L, "GetRenderParam",			CX2WorldObjectMesh::GetRenderParam );
    lua_tinker::class_def<CX2WorldObjectMesh>( L, "SetTex0UVSpeed",			CX2WorldObjectMesh::SetTex0UVSpeed );
    lua_tinker::class_def<CX2WorldObjectMesh>( L, "SetTex1UVSpeed",			CX2WorldObjectMesh::SetTex1UVSpeed );
    lua_tinker::class_def<CX2WorldObjectMesh>( L, "SetTex2UVSpeed",			CX2WorldObjectMesh::SetTex2UVSpeed );


    lua_tinker::class_add<CX2WorldObjectSkinMesh>( L, "CX2WorldObjectSkinMesh" );
    lua_tinker::class_inh<CX2WorldObjectSkinMesh, CX2WorldObject>( L );
    lua_tinker::class_def<CX2WorldObjectSkinMesh>( L, "PlayAnim",				CX2WorldObjectSkinMesh::PlayAnim_LUA );
    lua_tinker::class_def<CX2WorldObjectSkinMesh>( L, "AddAnim",				CX2WorldObjectSkinMesh::AddAnim_LUA );
    lua_tinker::class_def<CX2WorldObjectSkinMesh>( L, "SetAnimAniXET",			CX2WorldObjectSkinMesh::SetAnimAniXET_LUA );
    lua_tinker::class_def<CX2WorldObjectSkinMesh>( L, "SetXSkinMotion",			CX2WorldObjectSkinMesh::SetXSkinMotion_LUA );
    lua_tinker::class_def<CX2WorldObjectSkinMesh>( L, "SetMeshAniXET",			CX2WorldObjectSkinMesh::SetMeshAniXET_LUA );
    lua_tinker::class_def<CX2WorldObjectSkinMesh>( L, "SetMultiTexXET",			CX2WorldObjectSkinMesh::SetMultiTexXET_LUA );
    lua_tinker::class_def<CX2WorldObjectSkinMesh>( L, "SetChangeTexXET",		CX2WorldObjectSkinMesh::SetChangeTexXET_LUA );
    lua_tinker::class_def<CX2WorldObjectSkinMesh>( L, "AddXSkinModel",			CX2WorldObjectSkinMesh::AddXSkinModel_LUA );
    lua_tinker::class_def<CX2WorldObjectSkinMesh>( L, "SetCanHide",				CX2WorldObjectSkinMesh::SetCanHide );
    lua_tinker::class_def<CX2WorldObjectSkinMesh>( L, "SetBillboardType",		CX2WorldObjectSkinMesh::SetBillboardType );
    lua_tinker::class_def<CX2WorldObjectSkinMesh>( L, "GetRenderParam",			CX2WorldObjectSkinMesh::GetRenderParam );
    lua_tinker::class_def<CX2WorldObjectSkinMesh>( L, "SetOccasionalEventTime",	CX2WorldObjectSkinMesh::SetOccasionalEventTime );
    lua_tinker::class_def<CX2WorldObjectSkinMesh>( L, "SetOccasionalPlayRate",	CX2WorldObjectSkinMesh::SetOccasionalPlayRate );
    lua_tinker::class_def<CX2WorldObjectSkinMesh>( L, "SetTex0UVSpeed",			CX2WorldObjectSkinMesh::SetTex0UVSpeed );
    lua_tinker::class_def<CX2WorldObjectSkinMesh>( L, "SetTex1UVSpeed",			CX2WorldObjectSkinMesh::SetTex1UVSpeed );
    lua_tinker::class_def<CX2WorldObjectSkinMesh>( L, "SetTex2UVSpeed",			CX2WorldObjectSkinMesh::SetTex2UVSpeed );


    lua_tinker::class_add<CX2WorldObjectLightFlow>( L, "CX2WorldObjectLightFlow" );
    lua_tinker::class_inh<CX2WorldObjectLightFlow, CX2WorldObject>( L );
    lua_tinker::class_def<CX2WorldObjectLightFlow>( L, "SetXMesh",				CX2WorldObjectLightFlow::SetXMesh_LUA );
    lua_tinker::class_def<CX2WorldObjectLightFlow>( L, "SetFlowMin",			CX2WorldObjectLightFlow::SetFlowMin );
    lua_tinker::class_def<CX2WorldObjectLightFlow>( L, "SetFlowMax",			CX2WorldObjectLightFlow::SetFlowMax );
    lua_tinker::class_def<CX2WorldObjectLightFlow>( L, "SetFlowSpeed",			CX2WorldObjectLightFlow::SetFlowSpeed );
    lua_tinker::class_def<CX2WorldObjectLightFlow>( L, "SetWide",				CX2WorldObjectLightFlow::SetWide );
    lua_tinker::class_def<CX2WorldObjectLightFlow>( L, "SetNowPoint",			CX2WorldObjectLightFlow::SetNowPoint );
    lua_tinker::class_def<CX2WorldObjectLightFlow>( L, "SetRenderType",			CX2WorldObjectLightFlow::SetRenderType );
    lua_tinker::class_def<CX2WorldObjectLightFlow>( L, "GetRenderParam",		CX2WorldObjectLightFlow::GetRenderParam );


    lua_tinker::class_add<CX2WorldObjectParticle>( L, "CX2WorldObjectParticle" );
    lua_tinker::class_inh<CX2WorldObjectParticle, CX2WorldObject>( L );
    lua_tinker::class_def<CX2WorldObjectParticle>( L, "SetCanHide",				CX2WorldObjectParticle::SetCanHide );
    lua_tinker::class_def<CX2WorldObjectParticle>( L, "SetParticlePos",			CX2WorldObjectParticle::SetParticlePos );
    lua_tinker::class_def<CX2WorldObjectParticle>( L, "SetEmitMinMax",			CX2WorldObjectParticle::SetEmitMinMax );
    lua_tinker::class_def<CX2WorldObjectParticle>( L, "SetTriggerCount",		CX2WorldObjectParticle::SetTriggerCount );
    lua_tinker::class_def<CX2WorldObjectParticle>( L, "SetTriggerTime",			CX2WorldObjectParticle::SetTriggerTime );
    lua_tinker::class_def<CX2WorldObjectParticle>( L, "SetDrawCount",			CX2WorldObjectParticle::SetDrawCount );
    lua_tinker::class_def<CX2WorldObjectParticle>( L, "SetCullCheck",			CX2WorldObjectParticle::SetCullCheck );
    lua_tinker::class_def<CX2WorldObjectParticle>( L, "SetParticleType",		CX2WorldObjectParticle::SetParticleType );

		

	lua_tinker::class_add<CX2Camera>(L, "CX2Camera");
		lua_tinker::class_def<CX2Camera>(L, "GetCamera",						CX2Camera::GetCamera);
		lua_tinker::class_def<CX2Camera>(L, "PartsLookTrackingCamera_LUA",		CX2Camera::PartsLookTrackingCamera_LUA);
		lua_tinker::class_def<CX2Camera>(L, "NomalDirectCamera_LUA",			CX2Camera::NomalDirectCamera_LUA);
		lua_tinker::class_def<CX2Camera>(L, "PartsLookDirectCamera_LUA",		CX2Camera::PartsLookDirectCamera_LUA);
		lua_tinker::class_def<CX2Camera>(L, "GetCamera",						CX2Camera::GetCamera);
		

	lua_tinker::class_add<KX2EnumManager>(L, "KX2EnumManager");
		lua_tinker::class_def<KX2EnumManager>(L, "AddWorldEnumData_LUA",			KX2EnumManager::AddWorldEnumData_LUA);
		lua_tinker::class_def<KX2EnumManager>(L, "AddNpcEnumData_LUA",				KX2EnumManager::AddNpcEnumData_LUA);
		

	lua_tinker::class_add<KDropTable>(L, "KDropTable");
	lua_tinker::class_def<KDropTable>(L, "AddToGroup",					KDropTable::AddToGroup );
	lua_tinker::class_def<KDropTable>(L, "AddMonsterDropInfo",			KDropTable::AddMonsterDropInfo );
	lua_tinker::class_def<KDropTable>(L, "dump",						KDropTable::Dump );
	lua_tinker::class_def<KDropTable>(L, "DumpGroup",					KDropTable::DumpGroup );
	lua_tinker::class_def<KDropTable>(L, "DumpMonsterDrop",				KDropTable::DumpMonsterDrop );

    lua_tinker::class_add<CX2GameUnit>( L, "CX2GameUnit" );
    lua_tinker::class_def<CX2GameUnit>( L, "PlaySound_LUA",				CX2GameUnit::PlaySound_LUA );
    lua_tinker::class_def<CX2GameUnit>( L, "GetPos",					CX2GameUnit::GetPos );
    lua_tinker::class_def<CX2GameUnit>( L, "ClearHitUnitList_LUA",		CX2GameUnit::ClearHitUnitList_LUA );
    lua_tinker::class_def<CX2GameUnit>( L, "SetSpeedX",					CX2GameUnit::SetSpeedX );
    lua_tinker::class_def<CX2GameUnit>( L, "SetSpeedY",					CX2GameUnit::SetSpeedY );
    lua_tinker::class_def<CX2GameUnit>( L, "SetDamageData_LUA",			CX2GameUnit::SetDamageData_LUA );
    lua_tinker::class_def<CX2GameUnit>( L, "GetNowHP",					CX2GameUnit::GetNowHp );
    lua_tinker::class_def<CX2GameUnit>( L, "GetMaxHP",					CX2GameUnit::GetMaxHp );
    lua_tinker::class_def<CX2GameUnit>( L, "SetNowHP",					CX2GameUnit::SetNowHp );
    lua_tinker::class_def<CX2GameUnit>( L, "GetNowMP",					CX2GameUnit::GetNowMp );
    lua_tinker::class_def<CX2GameUnit>( L, "SetNowMP",					CX2GameUnit::SetNowMp );
    lua_tinker::class_def<CX2GameUnit>( L, "GetMaxMP",					CX2GameUnit::GetMaxMp );
    lua_tinker::class_def<CX2GameUnit>( L, "SetColor",					CX2GameUnit::SetColor );
    lua_tinker::class_def<CX2GameUnit>( L, "GetColor",					CX2GameUnit::GetColor );
    lua_tinker::class_def<CX2GameUnit>( L, "GetImpactPoint_LUA",		CX2GameUnit::GetImpactPoint_LUA );
    lua_tinker::class_def<CX2GameUnit>( L, "GetAttackerUnit_LUA",		CX2GameUnit::GetAttackerUnit_LUA );
    lua_tinker::class_def<CX2GameUnit>( L, "GetDistanceFrom",			CX2GameUnit::GetDistanceFrom );
    lua_tinker::class_def<CX2GameUnit>( L, "GetXDistanceFrom",			CX2GameUnit::GetXDistanceFrom );
    lua_tinker::class_def<CX2GameUnit>( L, "GetWalkSpeed",				CX2GameUnit::GetWalkSpeed );
    lua_tinker::class_def<CX2GameUnit>( L, "SetShowGage",				CX2GameUnit::SetShowGage );
    lua_tinker::class_def<CX2GameUnit>( L, "SetCanNotInputTime_LUA",	CX2GameUnit::SetCanNotInputTime_LUA );
    lua_tinker::class_def<CX2GameUnit>( L, "SetUseWorldColor",			CX2GameUnit::SetUseWorldColor );		
    lua_tinker::class_def<CX2GameUnit>( L, "GetDirVector",				CX2GameUnit::GetDirVector );
    lua_tinker::class_def<CX2GameUnit>( L, "GetTeam",					CX2GameUnit::GetTeam );
    lua_tinker::class_def<CX2GameUnit>( L, "MultiplyStat",				CX2GameUnit::MultiplyStat );
    lua_tinker::class_def<CX2GameUnit>( L, "SetDamageReducePercent",	CX2GameUnit::SetDamageReducePercent );
    lua_tinker::class_def<CX2GameUnit>( L, "TwinkleMPGageBar",			CX2GameUnit::TwinkleMPGageBar );
    lua_tinker::class_def<CX2GameUnit>( L, "SetMajorMeshPlayer_LUA",	CX2GameUnit::SetMajorMeshPlayer_LUA );
    lua_tinker::class_def<CX2GameUnit>( L, "ClearMajorMeshPlayer_LUA",	CX2GameUnit::ClearMajorMeshPlayer_LUA );
    lua_tinker::class_def<CX2GameUnit>( L, "GetMajorMeshPlayer_LUA",	CX2GameUnit::GetMajorMeshPlayer_LUA );
    lua_tinker::class_def<CX2GameUnit>( L, "SetMinorMeshPlayer_LUA",	CX2GameUnit::SetMinorMeshPlayer_LUA );
    lua_tinker::class_def<CX2GameUnit>( L, "ClearMinorMeshPlayer_LUA",	CX2GameUnit::ClearMinorMeshPlayer_LUA );
    lua_tinker::class_def<CX2GameUnit>( L, "GetMinorMeshPlayer_LUA",	CX2GameUnit::GetMinorMeshPlayer_LUA );
    lua_tinker::class_def<CX2GameUnit>( L, "AddOnStatReq",				CX2GameUnit::AddOnStatReq );



    lua_tinker::class_add<CX2GUNPC>( L, "CX2GUNPC" );
    lua_tinker::class_inh<CX2GUNPC, CX2GameUnit>( L );
    lua_tinker::class_def<CX2GUNPC>( L, "AnimEventTimer_LUA",			CX2GUNPC::AnimEventTimer_LUA);
    lua_tinker::class_def<CX2GUNPC>( L, "AnimTimer_LUA",				CX2GUNPC::AnimTimer_LUA);
    lua_tinker::class_def<CX2GUNPC>( L, "GetNowAnimTime_LUA",			CX2GUNPC::GetNowAnimTime_LUA);
    lua_tinker::class_def<CX2GUNPC>( L, "EventTimer",					CX2GUNPC::EventTimer);
    lua_tinker::class_def<CX2GUNPC>( L, "GetLandPosition_LUA",			CX2GUNPC::GetLandPosition_LUA);
    lua_tinker::class_def<CX2GUNPC>( L, "SetPosition",					CX2GUNPC::SetPosition);
    lua_tinker::class_def<CX2GUNPC>( L, "GetPos",						CX2GUNPC::GetPos);
    lua_tinker::class_def<CX2GUNPC>( L, "GetRotateDegree",				CX2GUNPC::GetRotateDegree);
    lua_tinker::class_def<CX2GUNPC>( L, "GetXRotateDegree",				CX2GUNPC::GetXRotateDegree);
    lua_tinker::class_def<CX2GUNPC>( L, "GetYRotateDegree",				CX2GUNPC::GetYRotateDegree);
    lua_tinker::class_def<CX2GUNPC>( L, "GetZRotateDegree",				CX2GUNPC::GetZRotateDegree);
    lua_tinker::class_def<CX2GUNPC>( L, "SetIsRight",					CX2GUNPC::SetIsRight);        
    lua_tinker::class_def<CX2GUNPC>( L, "GetIsRight",					CX2GUNPC::GetIsRight);
    lua_tinker::class_def<CX2GUNPC>( L, "GetDirPos_LUA",				CX2GUNPC::GetDirPos_LUA);        
    lua_tinker::class_def<CX2GUNPC>( L, "SetAddRotateDegree_LUA",		CX2GUNPC::SetAddRotateDegree_LUA);
    lua_tinker::class_def<CX2GUNPC>( L, "GetXSkinAnim",					CX2GUNPC::GetXSkinAnim);
    lua_tinker::class_def<CX2GUNPC>( L, "GetBonePos_LUA",				CX2GUNPC::GetBonePos_LUA);
    lua_tinker::class_def<CX2GUNPC>( L, "GetWeaponBonePos_LUA",			CX2GUNPC::GetWeaponBonePos_LUA);
    lua_tinker::class_def<CX2GUNPC>( L, "AttachWeaponParticle_LUA",		CX2GUNPC::AttachWeaponParticle_LUA);
    lua_tinker::class_def<CX2GUNPC>( L, "GetUnitCondition_bHit",		CX2GUNPC::GetUnitCondition_bHit);
    lua_tinker::class_def<CX2GUNPC>( L, "SetKeyCode",					CX2GUNPC::SetKeyCode);
    lua_tinker::class_def<CX2GUNPC>( L, "GetKeyCode",					CX2GUNPC::GetKeyCode);
    lua_tinker::class_def<CX2GUNPC>( L, "SetFlag_LUA",					CX2GUNPC::SetFlag_LUA);
    lua_tinker::class_def<CX2GUNPC>( L, "GetFlag_LUA",					CX2GUNPC::GetFlag_LUA);
    lua_tinker::class_def<CX2GUNPC>( L, "SetInt_LUA",					CX2GUNPC::SetInt_LUA);
    lua_tinker::class_def<CX2GUNPC>( L, "GetInt_LUA",					CX2GUNPC::GetInt_LUA);
    lua_tinker::class_def<CX2GUNPC>( L, "SetVector_LUA",				CX2GUNPC::SetVector_LUA);
    lua_tinker::class_def<CX2GUNPC>( L, "GetVector_LUA",				CX2GUNPC::GetVector_LUA);
    lua_tinker::class_def<CX2GUNPC>( L, "SetAIEnable",					CX2GUNPC::SetAIEnable);
    lua_tinker::class_def<CX2GUNPC>( L, "SetMajorParticle_LUA",			CX2GUNPC::SetMajorParticle_LUA);
    lua_tinker::class_def<CX2GUNPC>( L, "SetMinorParticle_LUA",			CX2GUNPC::SetMinorParticle_LUA);
    lua_tinker::class_def<CX2GUNPC>( L, "GetMajorParticle_LUA",			CX2GUNPC::GetMajorParticle_LUA);
    lua_tinker::class_def<CX2GUNPC>( L, "GetMinorParticle_LUA",			CX2GUNPC::GetMinorParticle_LUA);
    lua_tinker::class_def<CX2GUNPC>( L, "ClearMinorParticle_LUA",		CX2GUNPC::ClearMinorParticle_LUA);
    lua_tinker::class_def<CX2GUNPC>( L, "ClearMajorParticle_LUA",		CX2GUNPC::ClearMajorParticle_LUA);
    lua_tinker::class_def<CX2GUNPC>( L, "GetDamageEffect_LUA",			CX2GUNPC::GetDamageEffect_LUA);
    lua_tinker::class_def<CX2GUNPC>( L, "SetDamageEffect_LUA",			CX2GUNPC::SetDamageEffect_LUA);
    lua_tinker::class_def<CX2GUNPC>( L, "ClearDamageEffect_LUA",		CX2GUNPC::ClearDamageEffect_LUA);
    lua_tinker::class_def<CX2GUNPC>( L, "SetInvincibleTime",			CX2GUNPC::SetInvincibleTime);
    lua_tinker::class_def<CX2GUNPC>( L, "SetSuperArmor",				CX2GUNPC::SetSuperArmorStateParam);
    lua_tinker::class_def<CX2GUNPC>( L, "CallOtherNPC",					CX2GUNPC::CallOtherNPC);
    lua_tinker::class_def<CX2GUNPC>( L, "GetNowAnimationTime",			CX2GUNPC::GetNowAnimationTime);
    lua_tinker::class_def<CX2GUNPC>( L, "SetPassiveSpeed",				CX2GUNPC::SetPassiveSpeed);
    lua_tinker::class_def<CX2GUNPC>( L, "SetSpeedX",					CX2GUNPC::SetSpeedX);
    lua_tinker::class_def<CX2GUNPC>( L, "SetSpeedY",					CX2GUNPC::SetSpeedY);
    lua_tinker::class_def<CX2GUNPC>( L, "GetSpeedY",					CX2GUNPC::GetSpeedY);
    lua_tinker::class_def<CX2GUNPC>( L, "GetDamagedThisFrame",			CX2GUNPC::GetDamagedThisFrame);
    lua_tinker::class_def<CX2GUNPC>( L, "HasTarget",					CX2GUNPC::HasTarget);
    lua_tinker::class_def<CX2GUNPC>( L, "SetDieSeq",					CX2GUNPC::SetDieSeq);
    lua_tinker::class_def<CX2GUNPC>( L, "GetHardLevel",					CX2GUNPC::GetHardLevel);		
    lua_tinker::class_def<CX2GUNPC>( L, "GetBoneRotateDegree_LUA",		CX2GUNPC::GetBoneRotateDegree_LUA);
    lua_tinker::class_def<CX2GUNPC>( L, "GetHyperModeCount",			CX2GUNPC::GetHyperModeCount);
    lua_tinker::class_def<CX2GUNPC>( L, "ChangeHyperMode_LUA",			CX2GUNPC::ChangeHyperMode_LUA);
    lua_tinker::class_def<CX2GUNPC>( L, "SetLockOnTarget",				CX2GUNPC::SetLockOnTarget);
    lua_tinker::class_def<CX2GUNPC>( L, "SetLockOn",					CX2GUNPC::SetLockOn);
    lua_tinker::class_def<CX2GUNPC>( L, "SetLockOnPos",					CX2GUNPC::SetLockOnPos);
    lua_tinker::class_def<CX2GUNPC>( L, "SetSiegeMode",					CX2GUNPC::SetSiegeMode);
    lua_tinker::class_def<CX2GUNPC>( L, "ReleaseSiegeMode",				CX2GUNPC::ReleaseSiegeMode);
    lua_tinker::class_def<CX2GUNPC>( L, "GetUID",						CX2GUNPC::GetUID);
    lua_tinker::class_def<CX2GUNPC>( L, "ClearEventCheck",				CX2GUNPC::ClearEventCheck);
    lua_tinker::class_def<CX2GUNPC>( L, "GetFollowHitter",				CX2GUNPC::GetFollowHitter);
    lua_tinker::class_def<CX2GUNPC>( L, "GetLineGroupStartPos",			CX2GUNPC::GetLineGroupStartPos);
    lua_tinker::class_def<CX2GUNPC>( L, "GetLineGroupEndPos",			CX2GUNPC::GetLineGroupEndPos);
    lua_tinker::class_def<CX2GUNPC>( L, "GetStateTime",					CX2GUNPC::GetStateTime);
    lua_tinker::class_def<CX2GUNPC>( L, "GetZVector",					CX2GUNPC::GetZVector);
    lua_tinker::class_def<CX2GUNPC>( L, "GetNowHP_LUA",					CX2GUNPC::GetNowHP_LUA);
    lua_tinker::class_def<CX2GUNPC>( L, "SetNowHP_LUA",					CX2GUNPC::SetNowHP_LUA);
    lua_tinker::class_def<CX2GUNPC>( L, "SetShow",						CX2GUNPC::SetShow);
    lua_tinker::class_def<CX2GUNPC>( L, "GetAITargetPos",				CX2GUNPC::GetAITargetPos);
    lua_tinker::class_def<CX2GUNPC>( L, "SetEnableLuaTargetingFunc",	CX2GUNPC::SetEnableLuaTargetingFunc);
    lua_tinker::class_def<CX2GUNPC>( L, "SetLuaTargetingFunc",			CX2GUNPC::SetLuaTargetingFunc);
    lua_tinker::class_def<CX2GUNPC>( L, "SetTargetUnitUID",				CX2GUNPC::SetTargetUnitUID);
    lua_tinker::class_def<CX2GUNPC>( L, "SetTargetUnitType",			CX2GUNPC::SetTargetUnitType);
    lua_tinker::class_def<CX2GUNPC>( L, "GetNearestUserPos",			CX2GUNPC::GetNearestUserPos);
    lua_tinker::class_def<CX2GUNPC>( L, "IsUnitID",			        	CX2GUNPC::IsUnitID);        
    lua_tinker::class_def<CX2GUNPC>( L, "GetStartState",				CX2GUNPC::GetStartState);
    lua_tinker::class_def<CX2GUNPC>( L, "GetDisabledStateID",			CX2GUNPC::GetDisabledStateID);
    lua_tinker::class_def<CX2GUNPC>( L, "StateChangeForce",				CX2GUNPC::StateChangeForce);
    lua_tinker::class_def<CX2GUNPC>( L, "SetRageForce",					CX2GUNPC::SetRageForce);
    lua_tinker::class_def<CX2GUNPC>( L, "GetNowStateID",				CX2GUNPC::GetNowStateID);
    lua_tinker::class_def<CX2GUNPC>( L, "SetWeaponEnable",				CX2GUNPC::SetWeaponEnable);
    lua_tinker::class_def<CX2GUNPC>( L, "StateChange_LUA",				CX2GUNPC::StateChange_LUA);
    lua_tinker::class_def<CX2GUNPC>( L, "SetTimerRestart",				CX2GUNPC::SetTimerRestart);
    lua_tinker::class_def<CX2GUNPC>( L, "IsSelfDestructing",			CX2GUNPC::IsSelfDestructing);
    lua_tinker::class_def<CX2GUNPC>( L, "StartSelfDestruction",			CX2GUNPC::StartSelfDestruction);
    lua_tinker::class_def<CX2GUNPC>( L, "StopSelfDestruction",			CX2GUNPC::StopSelfDestruction);
    lua_tinker::class_def<CX2GUNPC>( L, "AttachParticle_LUA",			CX2GUNPC::AttachParticle_LUA);
}

HRESULT KX2DungeonTool::OnFrameMove( double fTime, float fElapsedTime )
{
	// 카메라 이동
	m_FPSCamera.FrameMove( fElapsedTime * m_FPSCamera.m_fMoveSpeed );
	g_pKTDXApp->GetDGManager()->GetCamera().Move( m_FPSCamera.GetEyePt()->x, m_FPSCamera.GetEyePt()->y, m_FPSCamera.GetEyePt()->z );
	D3DXVECTOR3 vLookAt = *m_FPSCamera.GetWorldAhead() * g_pKTDXApp->GetDGManager()->GetFar()*0.1f + *m_FPSCamera.GetEyePt();
	g_pKTDXApp->GetDGManager()->GetCamera().LookAt( vLookAt.x, vLookAt.y, vLookAt.z );
	g_pKTDXApp->GetDGManager()->GetCamera().UpdateCamera( fElapsedTime );

	if( m_pWorld != NULL )
	{
		m_pWorld->OnFrameMove( fTime, fElapsedTime );

		if( m_bRenderingNPC )
		{
			// start pos 개수 만큼 skinanim 개수를 맞추어주자
			if( vecpXSkinAnim.size() < vecTeamStartPos.size() )
			{
				for( unsigned i=vecpXSkinAnim.size(); i<vecTeamStartPos.size(); i++ )
				{
					CKTDGXSkinAnim* pXSkinAnim = new CKTDGXSkinAnim( g_pKTDXApp->GetDevice() );
					pXSkinAnim->SetAnimXSkinMesh(  m_pXSkinMesh );
					pXSkinAnim->AddModelXSkinMesh( m_pXSkinMesh );
					pXSkinAnim->SetAlphaObject( true );
					pXSkinAnim->ChangeAnim( L"LobbyReadyAction", false );
					pXSkinAnim->Play( CKTDGXSkinAnim::XAP_ONE_WAIT );
					vecpXSkinAnim.push_back( pXSkinAnim );
				}
			}

			for( unsigned i=0; i<vecpXSkinAnim.size(); i++ )
			{	
				vecpXSkinAnim[i]->OnFrameMove(fTime, fElapsedTime);
			}

			map< std::pair<CX2DungeonSubStage::NPCData*, int>, CKTDGXSkinAnim* >::iterator it;
			for( it=m_mapNPCXSkinAnim.begin(); it!= m_mapNPCXSkinAnim.end(); it++ )
			{
				CKTDGXSkinAnim* pCKTDGXSkinAnim = (CKTDGXSkinAnim*)it->second;
				pCKTDGXSkinAnim->OnFrameMove( fTime, fElapsedTime );
			}
		}		
	}

	g_pKTDXApp->GetDGManager()->GetDialogManager()->OnFrameMove( fTime, fElapsedTime );

	return S_OK;
}

HRESULT KX2DungeonTool::OnFrameRender()
{
	HRESULT hr = S_OK;

	if( m_pWorld != NULL )
	{
		m_pWorld->OnFrameRender();

		if( m_bRenderingNPC )
			RenderNPCUnitStartPostion();

		if( m_bRenderingCurtain )
		{
			RenderCurtain();
			// render highlighted segment
			HighlightPickedSegments();
			if( m_veciPickedSegmentIndex.size() != (size_t) 0 )
			{
				if(FAILED(hr = g_pKTDXApp->GetDevice()->SetFVF( D3DFVF_LINE_MAP_VERTEX )))
					return hr;

				if(FAILED(hr = g_pKTDXApp->GetDevice()->SetStreamSource( 0, m_pVBHighlightedSegment, 0, sizeof(LINE_MAP_VERTEX) )))
					return hr;

				if(FAILED(hr = g_pKTDXApp->GetDevice()->DrawPrimitive( D3DPT_LINELIST, 0, (UINT)m_veciPickedSegmentIndex.size() )))
					return hr;
			}
		}

		if( m_bRenderingStartPos )
			RenderStartPos();

		if( m_bRenderingLineMapNumber )
			RenderLineNumber();
	}

	if( m_pDungeonMapUI != NULL )
	{
		m_pDungeonMapUI->OnFrameRender();
	}
	
	m_menuUI.OnRender( g_pKTDXApp->GetElapsedTime() );
	m_mainUI.OnRender( g_pKTDXApp->GetElapsedTime() );
	m_toolUI.OnRender( g_pKTDXApp->GetElapsedTime() );
	m_LeftToolUI.OnRender( g_pKTDXApp->GetElapsedTime() );
	m_mapUI.OnRender(  g_pKTDXApp->GetElapsedTime() );
	m_summaryUI.OnRender(  g_pKTDXApp->GetElapsedTime() );

	return S_OK;
}

bool	KX2DungeonTool::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( uMsg )
	{
	case WM_CHAR:
		{
			switch(wParam)
			{		
			case 'c':
				{
					D3DXVECTOR3 vLookAt, vEye;
					vLookAt = D3DXVECTOR3(0,0,0); 
					vEye = vLookAt + D3DXVECTOR3(0,0,-2500);
					g_pKTDXApp->GetDGManager()->GetCamera().Point(vEye.x, vEye.y, vEye.z, vLookAt.x, vLookAt.y, vLookAt.z);
					m_FPSCamera.SetViewParams( &vEye, &vLookAt );
				}
				break;
			}
		}		
		break;

	case WM_KEYDOWN:
		{
			switch(wParam)
			{
			case VK_F1:
				if( m_bScreenShotMode == false )
				{
					m_bUIVisibleBackUp[0] = m_menuUI.GetVisible();
					m_bUIVisibleBackUp[1] = m_mainUI.GetVisible();
					m_bUIVisibleBackUp[2] = m_toolUI.GetVisible();
					m_bUIVisibleBackUp[3] = m_mapUI.GetVisible();
					m_bUIVisibleBackUp[4] = g_HUD.GetVisible();

					m_menuUI.SetVisible( false );
					m_mainUI.SetVisible( false );
					m_toolUI.SetVisible( false );
					m_mapUI.SetVisible( false );
					g_HUD.SetVisible( false );

					m_bScreenShotMode = true;
				}
				else
				{
					m_menuUI.SetVisible( m_bUIVisibleBackUp[0] );
					m_mainUI.SetVisible( m_bUIVisibleBackUp[1] );
					m_toolUI.SetVisible( m_bUIVisibleBackUp[2] );
					m_mapUI.SetVisible( m_bUIVisibleBackUp[3] );
					g_HUD.SetVisible( m_bUIVisibleBackUp[4] );

					m_bScreenShotMode = false;
				}				  
				break;

			case VK_F5:
				{
					if( IsDungeonExist() == false )
						break;

					std::wstring wstrQuickSaveFile = m_wsQuickSaveFile;

					CTime tCurrentTime = CTime::GetCurrentTime();
					wstrQuickSaveFile += ( CStringW )( tCurrentTime.Format( _T( "_%Y-%m-%d_%H%M%S.lua" ) ) );

					if( SaveDungeonLuaFile( wstrQuickSaveFile.c_str() ) == false )
					{
						std::wstring wstrMessage = wstrQuickSaveFile;
						wstrMessage += L" : Quick Save Failed!";

						MessageBox( g_pKTDXApp->GetHWND(), wstrMessage.c_str(), L"실패!", MB_OK );
					}
				}				
				break;
			}
		}
		break;
	}

	bool bNoFurtherProcessing = false;

	bNoFurtherProcessing = m_menuUI.MsgProc( hWnd, uMsg, wParam, lParam );
	if( bNoFurtherProcessing )
		return true;

	bNoFurtherProcessing = m_mainUI.MsgProc( hWnd, uMsg, wParam, lParam );
	if( bNoFurtherProcessing )
		return true;

	bNoFurtherProcessing = m_toolUI.MsgProc( hWnd, uMsg, wParam, lParam );
	if( bNoFurtherProcessing )
		return true;

	bNoFurtherProcessing = m_LeftToolUI.MsgProc( hWnd, uMsg, wParam, lParam );
	if( bNoFurtherProcessing )
		return true;

	bNoFurtherProcessing = m_mapUI.MsgProc( hWnd, uMsg, wParam, lParam );
	if( bNoFurtherProcessing )
		return true;

	bNoFurtherProcessing = m_summaryUI.MsgProc( hWnd, uMsg, wParam, lParam );
	if( bNoFurtherProcessing )
		return true;

	m_FPSCamera.HandleMessages(hWnd, uMsg, wParam, lParam);	

	POINTS mousepos;
	switch( uMsg )
	{
	case WM_LBUTTONDOWN:
		{			
			mousepos = MAKEPOINTS(lParam);

			// map 클릭
			if( m_menuUI.GetCheckBox( IDC_MAP_UI_SHOW_CHECK )->GetChecked() )
			{				
				PickClickedMap( mousepos );
				break;
			}

			if( NULL == m_pWorld )
				break;

			// Line 클릭
			if( m_menuUI.GetCheckBox( IDC_CURTAIN_SHOW_CHECK )->GetChecked() )
			{
				PickClickedLine( mousepos, wParam );
			}

			// NPC 유닛 클릭
			if( m_menuUI.GetCheckBox( IDC_NPC_SHOW_CHECK )->GetChecked() )
			{
				if( wParam & MK_CONTROL )
				{
					m_bMouseDragging = false;
					PickClickedNPC( mousepos, false );
				}
				else
				{
					m_bMouseDragging = true;
					PickClickedNPC( mousepos );
				}
			}			
		} break;

	case WM_MOUSEMOVE:
		{
			mousepos = MAKEPOINTS(lParam);

			// map 클릭
			if( m_menuUI.GetCheckBox( IDC_MAP_UI_SHOW_CHECK )->GetChecked() && m_bPickMiniMap == true )
			{
				MovePickMap( mousepos );
				break;
			}
			/************************************************************************/
			/*  NPC클릭후 마우스를 끌어다 StartPos에 놓으면 그 NPC의 StartPos가 수정되는것으로 예정
			/************************************************************************/
			//if( m_vecPickedNPCStartPosKey.size() != 0 &&
			//	true == m_bMouseDragging )
			//{
			//	D3DXVECTOR3 vPos( 0, 0, -1 );
			//	GetMousePosOnLine( &vPos, mousepos );
			//	for( UINT i=0; i<m_vecPickedNPCStartPosKey.size(); i++ )
			//	{
			//		for( UINT j=0; j<vecTeamStartPos.size(); j++ )
			//		{
			//			SNPCStartPos& npcStartPos = m_vecPickedNPCStartPosKey[i];
			//			
			//			if( vecTeamStartPos.at(j).key == npcStartPos.m_iStartPosIndex )
			//			{
			//				if( vPos.z != -1.f )
			//				{
			//					vecTeamStartPos.at(j).vPos = vPos;
			//				}
			//				break;
			//			}						
			//		}
			//	}
			//}
		} break;

	case WM_LBUTTONUP:
		{
			mousepos = MAKEPOINTS(lParam);	

			m_bMouseDragging = false;

			// map 클릭
			if( m_menuUI.GetCheckBox( IDC_MAP_UI_SHOW_CHECK )->GetChecked() && m_bPickMiniMap == true )
			{
				m_bPickMiniMap = false;
				break;
			}

		} break;

	case WM_RBUTTONDOWN:
		break;

	default:
		break;
	}

	return g_pKTDXApp->GetDGManager()->GetDialogManager()->MsgProc( hWnd, uMsg, wParam, lParam );
}

void KX2DungeonTool::RenderCurtain()
{
	if( m_pEditingStage == NULL )
		return;

	CKTDGLineMap::LineData* pLineData = NULL;
	CKTDGMatrix matTemp( g_pKTDXApp->GetDevice() );
	CKTDGXRenderer::RenderParam* pRenderParam = g_pKTDXApp->GetDGManager()->GetXRenderer()->GetRenderParam();
	g_pKTDXApp->SetNULLTexture(0); // 노 텍스쳐 메쉬

	// Render Curtain
	if( m_pWorld->GetLineMap() != NULL && m_pXCurtainMesh != NULL )
	{
		float fIndicatorSize = 3.f;
		matTemp.Scale( fIndicatorSize, fIndicatorSize, fIndicatorSize );

		std::vector<CX2DungeonSubStage::SubStageData*> vecSubStageList =  m_pEditingStage->m_SubStageDataList;
		std::vector<CX2DungeonSubStage::SubStageData*>::iterator mit = vecSubStageList.begin();
		for( ; mit != vecSubStageList.end(); mit++ )
		{
			std::vector<CX2DungeonSubStage::CurtainData*> vecCurtainList = (*mit)->m_CurtainDataList;
			std::vector<CX2DungeonSubStage::CurtainData*>::iterator mit2 = vecCurtainList.begin();
			for( ; mit2 != vecCurtainList.end(); mit2++ )
			{
				CX2DungeonSubStage::CurtainData* pCurtainData = *mit2;
				
				pLineData = m_pWorld->GetLineMap()->GetLineData( pCurtainData->m_LineIndex );

				if( pCurtainData->m_bAtStart == true )
				{
					matTemp.Move( pLineData->startPos );

					// 커튼 Y축 회전
					D3DXVECTOR3 vDir = pLineData->dirVector;
					vDir.y = 0.f;
					D3DXVec3Normalize( &vDir, &vDir );
					D3DXVECTOR3 vXAxis(1,0,0);
					float fDotXAxis = D3DXVec3Dot( &vDir, &vXAxis );
					float fRad = acosf( fDotXAxis );
					if( vDir.z < 0.f )
					{
						fRad += D3DX_PI;
					}
					matTemp.Rotate( 0.f, fRad, 0.f );					

					pRenderParam->worldMatrix = matTemp.GetMatrix();
					pRenderParam->color	= D3DXCOLOR(0,0,1,1); 
					g_pKTDXApp->GetDGManager()->GetXRenderer()->BeginRender();
					g_pKTDXApp->GetDGManager()->GetXRenderer()->OnFrameRender( m_pXCurtainMesh );
					g_pKTDXApp->GetDGManager()->GetXRenderer()->EndRender();
				}
				else
				{
					matTemp.Move( pLineData->endPos );

					// 커튼 Y축 회전
					D3DXVECTOR3 vDir = pLineData->dirVector;
					vDir.y = 0.f;
					D3DXVec3Normalize( &vDir, &vDir );
					D3DXVECTOR3 vXAxis(1,0,0);
					float fDotXAxis = D3DXVec3Dot( &vDir, &vXAxis );
					float fRad = acosf( fDotXAxis );
					if( vDir.z < 0.f )
					{
						fRad += D3DX_PI;
					}
					matTemp.Rotate( 0.f, fRad, 0.f );					

					pRenderParam->worldMatrix = matTemp.GetMatrix();
					pRenderParam->color	= D3DXCOLOR(0,0,1,1); 
					g_pKTDXApp->GetDGManager()->GetXRenderer()->BeginRender();
					g_pKTDXApp->GetDGManager()->GetXRenderer()->OnFrameRender( m_pXCurtainMesh );
					g_pKTDXApp->GetDGManager()->GetXRenderer()->EndRender();
				}
			}			
		}		
	}
	g_pKTDXApp->SetNULLTexture(2);
}

void KX2DungeonTool::PickClickedLine( POINTS mousepos, WPARAM wParam )
{
	WCHAR buf[128] = L"";

	if( m_pEditingStage == NULL )
		return;

	int iPickedIndex = PickASegment(mousepos);
	if( iPickedIndex != -1 )
	{
		//m_bMouseDragging = false;
		bool bRemoved = false;
		vector<int>::iterator it;
		for( it = m_veciPickedSegmentIndex.begin(); it != m_veciPickedSegmentIndex.end(); it++ )
		{
			if( (*it) == iPickedIndex )
			{
				m_veciPickedSegmentIndex.erase(it);
				bRemoved = true;
				break;
			}
		}

		if( bRemoved == false )
		{
			m_veciPickedSegmentIndex.clear(); // 한개만 존재
			m_veciPickedSegmentIndex.push_back(iPickedIndex);
		}
	}

	if( wParam & MK_SHIFT )
	{
		if( m_pEditingCurtain == NULL )
			return;

        m_pEditingCurtain->m_LineIndex = iPickedIndex;
		wsprintf( buf, L"%d", iPickedIndex );
		m_toolUI.GetEditBox( IDC_LINE_INDEX_EDIT )->SetText( buf );
	}
	else
	{
		wsprintf( buf, L"%d", iPickedIndex );
		m_menuUI.GetEditBox( IDC_LINE_NUMBER_EDIT )->SetText( buf );

		std::vector<CX2DungeonSubStage::SubStageData*> vecSubStageList = m_pEditingStage->m_SubStageDataList;
		std::vector<CX2DungeonSubStage::SubStageData*>::iterator mit = vecSubStageList.begin();
		for( int iSStgIndex=0 ; mit != vecSubStageList.end(); mit++, iSStgIndex++ )
		{
			std::vector<CX2DungeonSubStage::CurtainData*> vecCurtainList = (*mit)->m_CurtainDataList;
			std::vector<CX2DungeonSubStage::CurtainData*>::iterator mit2 = vecCurtainList.begin();
			for( int iCurIndex=0 ; mit2 != vecCurtainList.end(); mit2++, iCurIndex++ )
			{
				CX2DungeonSubStage::CurtainData* pCurtainData = *mit2;
				if( pCurtainData->m_LineIndex == iPickedIndex )
				{
					LoadSubStageInfo( *mit );
					m_toolUI.GetListBox( IDC_SUB_STAGE_LIST )->SelectItem( iSStgIndex );
					LoadCurtainInfo( pCurtainData );
					m_toolUI.GetListBox( IDC_CURTAIN_LIST )->SelectItem( iCurIndex );
				}
			}
		}
	}
	
	return;
}

int KX2DungeonTool::PickASegment( const POINTS& mousepos )
{
	if( m_pWorld == NULL )
		return -1;

	if( m_pWorld->GetLineMap() == NULL || m_pWorld->GetLineMap()->GetNumLineData() <= 0 )
		return -1;

	const D3DXMATRIX& matView = g_pKTDXApp->GetDGManager()->GetCamera().GetViewMatrix();
	D3DXMATRIX matProj;
	g_pKTDXApp->GetProjectionTransform( &matProj );

	D3DXMATRIX matViewProj, matInvViewProj;
	D3DXMatrixMultiply(&matViewProj, &matView, &matProj);
	D3DXMatrixInverse(&matInvViewProj, NULL, &matViewProj);

	float w = (float)(int)g_pKTDXApp->GetViewport().Width;
	float h = (float)(int)g_pKTDXApp->GetViewport().Height;

	float fMouseX = (float)(mousepos.x-w/2.f)/w *2.f;
	float fMouseY = (float)(h/2.f-mousepos.y)/h *2.f;
	D3DXVECTOR3 vMouse(fMouseX, fMouseY, 0.f);

	CKTDGLineMap::LineData* pLineData = NULL;

	int iPickedSegmentIndex = 0;
	float fDistance = 0.f;
	float fMinDistance = -1.f;
	float fLerpCoef, fLerpCoefPicked;
	for( int i=0; i<this->m_pWorld->GetLineMap()->GetNumLineData(); i++ )
	{
		pLineData = m_pWorld->GetLineMap()->GetLineData(i);
		D3DXVECTOR3 vStartPos, vEndPos;
		D3DXVec3TransformCoord( &vStartPos, &pLineData->startPos, &matViewProj );
		D3DXVec3TransformCoord( &vEndPos, &pLineData->endPos, &matViewProj);
		vStartPos.z = 0.f;
		vEndPos.z   = 0.f;

		fDistance = DistanceBetweenPointAndSegment(fLerpCoef, &vMouse, &vStartPos, &vEndPos);

		if( -1.f == fMinDistance || fDistance < fMinDistance )
		{
			iPickedSegmentIndex = i;
			fMinDistance = fDistance;
			fLerpCoefPicked = fLerpCoef;
		}
	}

	float fDistanceBound = 0.1f;
	if( fMinDistance < fDistanceBound )
	{
		return iPickedSegmentIndex;
	}
	else
	{
		return -1;
	}
}

void KX2DungeonTool::RenderStartPos()
{
	if( m_pWorld == NULL )
		return;

	CKTDGMatrix matTemp( g_pKTDXApp->GetDevice() );
	CKTDGXRenderer::RenderParam* pRenderParam = g_pKTDXApp->GetDGManager()->GetXRenderer()->GetRenderParam();	
	g_pKTDXApp->SetNULLTexture(0); // 노 텍스쳐 메쉬

	// Render StartPos
	if( m_pWorld->GetLineMap() != NULL && m_pXStartPosMesh != NULL )
	{
		float fIndicatorSize = 5.f;
		matTemp.Scale( fIndicatorSize, fIndicatorSize, fIndicatorSize );

		std::vector<STeamStartPos>::iterator mit = vecTeamStartPos.begin();
		for(int i=0 ; mit != vecTeamStartPos.end(); mit++, i++ )
		{
			STeamStartPos startPos = *mit;
			
			matTemp.Move( startPos.vPos );
			pRenderParam->worldMatrix = matTemp.GetMatrix();

			switch( startPos.iTeamColor )
			{
			case TEAM_RED:
				pRenderParam->color	= D3DXCOLOR(0,0,1,1);
				break;
			case TEAM_BLUE:
				pRenderParam->color	= D3DXCOLOR(1,0,0,1);
				break;
			default:
				pRenderParam->color	= D3DXCOLOR(0,1,0,1);
				break;
			}
			g_pKTDXApp->GetDGManager()->GetXRenderer()->BeginRender();
			g_pKTDXApp->GetDGManager()->GetXRenderer()->OnFrameRender( m_pXStartPosMesh );
			g_pKTDXApp->GetDGManager()->GetXRenderer()->EndRender();

			RenderStartPosIndex( i, startPos.iLineIndex );
		}		
	}

	g_pKTDXApp->SetNULLTexture(2);
}

void KX2DungeonTool::RenderStartPosIndex( int index, int iLineIndex )
{
	CKTDGMatrix matTemp( g_pKTDXApp->GetDevice() );
	//g_pKTDXApp->GetDGManager()->GetXRenderer()->GetRenderParam()->color = D3DXCOLOR(1,1,1,1);
		
	D3DXVECTOR3 vPos = vecTeamStartPos[index].vPos;
	CKTDGStateManager::PushTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
	CKTDGStateManager::PushTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE );
	CKTDGStateManager::PushTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_CURRENT );

	// 렌더 위치 & 크기
	matTemp.Move( vPos );
	matTemp.MoveRel( 0, 200, 0 );
	//matTemp.Scale( 100, 100, 100 );
	matTemp.Scale( 50, 50, 50 );
	
	// 텍스쳐가 바라보는 각도 설정
	CKTDGLineMap::LineData* pLineData = m_pWorld->GetLineMap()->GetLineData( iLineIndex );
	D3DXVECTOR3 vDir = pLineData->dirVector;
	vDir.y = 0.f;
	D3DXVec3Normalize( &vDir, &vDir );
    //D3DXVECTOR3 vEye = g_pKTDXApp->GetDGManager()->GetCamera().GetEye();
	D3DXVECTOR3 vXAxis(1,0,0);
    

	float fDotXAxis = D3DXVec3Dot( &vDir, &vXAxis );
	float fRad = acosf( fDotXAxis );
	if( vDir.z < 0.f )
	{
		fRad += D3DX_PI;
	}
	matTemp.Rotate( 0.f, fRad, 0.f );


	D3DMATERIAL9 mtrl;
	ZeroMemory( &mtrl, sizeof(D3DMATERIAL9) );


	D3DXCOLOR coTeamColor = D3DXCOLOR( 0.5, 0.5, 0, 1 );
	memcpy( &mtrl.Diffuse, &coTeamColor, sizeof(D3DCOLORVALUE) );
	memcpy( &mtrl.Ambient, &coTeamColor, sizeof(D3DCOLORVALUE) );

	g_pKTDXApp->GetDevice()->SetMaterial( &mtrl );

	int iNumber = vecTeamStartPos[index].key;
	int iDigit;
	while( iNumber >= 0 )
	{
		iDigit = iNumber%10;

		matTemp.UpdateWorldMatrix();
		m_pMesh3DText[iDigit]->DrawSubset(0);

		//matTemp.MoveRel( -35, 0, 0 );
		matTemp.MoveRel( -35.f * vDir );
		iNumber = iNumber/10;
		if( iNumber == 0 )
			break;
	}

	CKTDGStateManager::PopTextureStageState( 0, D3DTSS_COLORARG2 );
	CKTDGStateManager::PopTextureStageState( 0, D3DTSS_COLORARG1 );
	CKTDGStateManager::PopTextureStageState( 0, D3DTSS_COLOROP );
}

void KX2DungeonTool::RenderLineNumber()
{
	if( m_pWorld == NULL )
		return;

	CKTDGMatrix matTemp( g_pKTDXApp->GetDevice() );
	//g_pKTDXApp->GetDGManager()->GetXRenderer()->GetRenderParam()->color = D3DXCOLOR(1,1,1,1);

	CKTDGStateManager::PushTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
	CKTDGStateManager::PushTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE );
	CKTDGStateManager::PushTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_CURRENT );
	CKTDGLineMap::LineData* pLineData;

	for( int index=0; index<this->m_pWorld->GetLineMap()->GetNumLineData(); index++ )
	{		
		pLineData = m_pWorld->GetLineMap()->GetLineData( index );

		D3DXVECTOR3 vPos = pLineData->startPos;

		matTemp.Move( vPos );
		matTemp.MoveRel( 0, 100, 0 );
		//matTemp.Scale( 100, 100, 100 );
		matTemp.Scale( 50, 50, 50 );

		// 텍스쳐가 바라보는 각도 설정		
		D3DXVECTOR3 vDir = pLineData->dirVector;        
		vDir.y = 0.f;
		D3DXVec3Normalize( &vDir, &vDir );
		D3DXVECTOR3 vXAxis(1,0,0);

		float fDotXAxis = D3DXVec3Dot( &vDir, &vXAxis );
		float fRad = acosf( fDotXAxis );
		if( vDir.z < 0.f )
		{
			fRad += D3DX_PI;
		}
		matTemp.Rotate( 0.f, fRad, 0.f );


		D3DMATERIAL9 mtrl;
		ZeroMemory( &mtrl, sizeof(D3DMATERIAL9) );


		D3DXCOLOR coTeamColor = D3DXCOLOR( 0.5, 0.5, 0, 1 );
		memcpy( &mtrl.Diffuse, &coTeamColor, sizeof(D3DCOLORVALUE) );
		memcpy( &mtrl.Ambient, &coTeamColor, sizeof(D3DCOLORVALUE) );

		g_pKTDXApp->GetDevice()->SetMaterial( &mtrl );

		int iNumber = index;
		int iDigit;
		while( iNumber >= 0 )
		{
			iDigit = iNumber%10;

			matTemp.UpdateWorldMatrix();
			m_pMesh3DText[iDigit]->DrawSubset(0);

			//matTemp.MoveRel( -35, 0, 0 );
			matTemp.MoveRel( -35.f * vDir );
			iNumber = iNumber/10;
			if( iNumber == 0 )
				break;
		}
	}

	CKTDGStateManager::PopTextureStageState( 0, D3DTSS_COLORARG2 );
	CKTDGStateManager::PopTextureStageState( 0, D3DTSS_COLORARG1 );
	CKTDGStateManager::PopTextureStageState( 0, D3DTSS_COLOROP );
}

bool KX2DungeonTool::PickClickedNPC( POINTS mousepos, bool bClear )
{
	D3DXVECTOR3 vRayStart, vRayDir;
	D3DXMATRIX matView, matProj;
	matView = *(m_FPSCamera.GetViewMatrix());
	g_pKTDXApp->GetDevice()->GetTransform( D3DTS_PROJECTION, &matProj );
	ComputeRay( &vRayStart, &vRayDir, mousepos, &matView, &matProj);

	const float MAGIC_RAY_RANGE = 1000000.f;
	D3DXVECTOR3 vRayEnd;
	vRayEnd = vRayStart + vRayDir * MAGIC_RAY_RANGE;

	float fShortestHitDistance = MAGIC_RAY_RANGE;
	float fHitDistance = MAGIC_RAY_RANGE;

	if( true == bClear )
		m_vecPickedNPCStartPosKey.clear();
	
	int iPickedNPCStartIndex = -1;
	CX2DungeonSubStage::NPCData* pPickedNPCData = NULL;
	CX2DungeonSubStage::SubStageData* pInNPCSubStage = NULL;
	
	if( m_pEditingStage != NULL )
	{
		for( UINT i=0; i< m_pEditingStage->m_SubStageDataList.size(); i++ )
		{
			CX2DungeonSubStage::SubStageData* pSubStageData = m_pEditingStage->m_SubStageDataList[i];
			CollisionCheckNPCStartPos( pSubStageData, vRayStart, vRayEnd, fHitDistance, fShortestHitDistance,
				pPickedNPCData, iPickedNPCStartIndex, pInNPCSubStage );
		}
	}

	if( NULL != pPickedNPCData && -1 != iPickedNPCStartIndex )
	{
		m_vecPickedNPCStartPosKey.push_back( SNPCStartPos( pPickedNPCData, iPickedNPCStartIndex ) );
		//m_vecPickedNPCStartPosKey.push_back( pPickedNPCData->m_StartPos );		
		LoadSubStageInfo( pInNPCSubStage );
		std::vector<CX2DungeonSubStage::SubStageData*>::iterator mit = m_pEditingStage->m_SubStageDataList.begin();
		for( int index=0 ; mit != m_pEditingStage->m_SubStageDataList.end(); mit++, index++ )
		{
			if( *mit == pInNPCSubStage )
			{
				m_toolUI.GetListBox( IDC_SUB_STAGE_LIST )->SelectItem( index );
				break;
			}
		}
		LoadNpcInfo( pPickedNPCData );
		std::vector<CX2DungeonSubStage::NPCData*>::iterator mit2 = m_pEditingSubStage->m_NPCDataList.begin();
		for( int index=0 ; mit2 != m_pEditingSubStage->m_NPCDataList.end(); mit2++, index++ )
		{
			if( *mit2 == pPickedNPCData )
			{
				m_toolUI.GetListBox( IDC_NPC_LIST )->SelectItem( index );
				break;
			}
		}
		return true;
	}

	return false;
}

void KX2DungeonTool::AddNewStartPosition(const POINTS& mousepos)
{
	if( m_pWorld == NULL )
		return;

	if( m_pWorld->GetLineMap() == NULL || m_pWorld->GetLineMap()->GetNumLineData() <= 0 )
		return;

	const D3DXMATRIX& matView = g_pKTDXApp->GetDGManager()->GetCamera().GetViewMatrix();
	D3DXMATRIX matProj;
	g_pKTDXApp->GetProjectionTransform( &matProj );

	D3DXMATRIX matViewProj, matInvViewProj;
	D3DXMatrixMultiply(&matViewProj, &matView, &matProj);
	D3DXMatrixInverse(&matInvViewProj, NULL, &matViewProj);

	float w = (float)(int)g_pKTDXApp->GetViewport().Width;
	float h = (float)(int)g_pKTDXApp->GetViewport().Height;

	float fMouseX = (float)(mousepos.x-w/2.f)/w *2.f;
	float fMouseY = (float)(h/2.f-mousepos.y)/h *2.f;
	D3DXVECTOR3 vMouse(fMouseX, fMouseY, 0.f);

	CKTDGLineMap::LineData* pLineData = NULL;	

	int iPickedSegmentIndex = 0;
	float fDistance = 0.f;
	float fMinDistance = -1.f;
	float fLerpCoef, fLerpCoefPicked;
	for( int i=0; i<this->m_pWorld->GetLineMap()->GetNumLineData(); i++ )
	{
		pLineData = m_pWorld->GetLineMap()->GetLineData(i);
		D3DXVECTOR3 vStartPos, vEndPos;
		D3DXVec3TransformCoord( &vStartPos, &pLineData->startPos, &matViewProj );
		D3DXVec3TransformCoord( &vEndPos, &pLineData->endPos, &matViewProj);
		vStartPos.z = 0.f;
		vEndPos.z   = 0.f;

		fDistance = DistanceBetweenPointAndSegment(fLerpCoef, &vMouse, &vStartPos, &vEndPos);

		if( -1.f == fMinDistance || fDistance < fMinDistance )
		{
			iPickedSegmentIndex = i;
			fMinDistance = fDistance;
			fLerpCoefPicked = fLerpCoef;
		}
	}

	float fDistanceBound = 0.1f;
	if( fMinDistance < fDistanceBound )
	{
		pLineData = m_pWorld->GetLineMap()->GetLineData(iPickedSegmentIndex);
		D3DXVECTOR3 vStartPos = pLineData->startPos;
		D3DXVECTOR3 vEndPos   = pLineData->endPos;
		D3DXVECTOR3 vPointOnSegmentPicked = vStartPos + (vEndPos-vStartPos) * fLerpCoefPicked;

		//if( TEAM_NONE == iTeamColor )
		//{	
		//	if( 0 == pLineData->lineSet%2 )
		//	{
		//		int iSubStageIndex = pLineData->lineSet/2;
		//		vector< CX2LineMapToolMain::SubStageData* >& vecpSubStageData = GetSubStageData( m_iShowDungeonLevel );
		//		while( (int)vecpSubStageData.size() <= iSubStageIndex )
		//		{
		//			vecpSubStageData.push_back( new CX2LineMapToolMain::SubStageData );
		//		}
		//		CX2LineMapToolMain::SubStageData* pSubStageData = vecpSubStageData.at( iSubStageIndex );				
		//		CX2LineMapToolMain::NPCData* pNPCData = new CX2LineMapToolMain::NPCData;
		//		
		//		DXUTComboBoxItem *pItem = m_DungeonUI.GetComboBox( IDC_COMBOBOX_NPC_UNIT_ID )->GetSelectedItem();
		//		pNPCData->m_UnitID = (CX2UnitManager::NPC_UNIT_ID) (int) pItem->pData;


		//		CDXUTComboBox* pCombo = NULL;
		//		CDXUTEditBox* pEdit = NULL;
		//		wstring wstrACTIVE = L"ACTIVE";

		//		pItem = m_DungeonUI.GetComboBox( IDC_COMBOBOX_NPC_ACTIVE )->GetSelectedItem();
		//		if( 0 == wstrACTIVE.compare( pItem->strText ) )
		//		{
		//			pNPCData->m_bActive = true;
		//		}
		//		else
		//		{
		//			pNPCData->m_bActive = false;
		//		}


		//		wstrACTIVE = L"GAGEBAR";
		//		pItem = m_DungeonUI.GetComboBox( IDC_COMBOBOX_NPC_GAGEBAR )->GetSelectedItem();
		//		if( 0 == wstrACTIVE.compare( pItem->strText ) )
		//		{
		//			pNPCData->m_bShowGage = true;
		//		}
		//		else
		//		{
		//			pNPCData->m_bShowGage = false;
		//		}


		//		wstrACTIVE = L"FOCUS_CAMERA";
		//		pItem = m_DungeonUI.GetComboBox( IDC_COMBOBOX_NPC_FOCUS_CAMERA )->GetSelectedItem();
		//		if( 0 == wstrACTIVE.compare( pItem->strText ) )
		//		{
		//			pNPCData->m_bFocusCamera = true;
		//		}
		//		else
		//		{
		//			pNPCData->m_bFocusCamera = false;
		//		}

		//		pEdit = m_DungeonUI.GetEditBox( IDC_EDITBOX_NPC_KEY_CODE );
		//		pNPCData->m_KeyCode = _wtoi( pEdit->GetText() );

		//		pEdit = m_DungeonUI.GetEditBox( IDC_EDITBOX_NPC_ADD_POS_X );
		//		pNPCData->m_AddPos.x = (float)_wtof( pEdit->GetText() );

		//		pEdit = m_DungeonUI.GetEditBox( IDC_EDITBOX_NPC_ADD_POS_Y );
		//		pNPCData->m_AddPos.y = (float)_wtof( pEdit->GetText() );

		//		pEdit = m_DungeonUI.GetEditBox( IDC_EDITBOX_NPC_ADD_POS_Z );
		//		pNPCData->m_AddPos.z = (float)_wtof( pEdit->GetText() );

		//		if( DIR_LEFT == this->iInitialDir )
		//			pNPCData->m_bIsRight = false;
		//		else
		//			pNPCData->m_bIsRight = true;

		//		pNPCData->m_vPos = vPointOnSegmentPicked;
		//		pNPCData->m_iLineIndex = iPickedSegmentIndex;


		//		if( NULL == pNPCData->m_pSkinAnim )
		//		{
		//			CKTDXDeviceXSkinMesh* pSkinMesh = GetNPCSkinMesh( pNPCData->m_UnitID );
		//			if( NULL != pSkinMesh )
		//			{
		//				CKTDGXSkinAnim* pXSkinAnim = new CKTDGXSkinAnim( g_pKTDXApp->GetDevice() );
		//				pXSkinAnim->SetAnimXSkinMesh(  pSkinMesh );
		//				pXSkinAnim->AddModelXSkinMesh( pSkinMesh );
		//				pXSkinAnim->SetAlphaObject( true );
		//				pXSkinAnim->ChangeAnim( L"Wait", false );
		//				pXSkinAnim->Play( CKTDGXSkinAnim::XAP_LOOP );
		//				pNPCData->SetSkinAnim( pXSkinAnim );
		//			}
		//		}
		//		
		//		pSubStageData->m_NPCDataList.push_back( pNPCData );
		//		UpdateNPCStartPosIndex();
		//	}
		//}
		//else
		{
			int key=-1;
			if( iTeamColor == TEAM_NONE )
			{
				key = GetNextUniqueNPCStartPosKey();
				if( key == -1 )
					return;
			}

			vecTeamStartPos.push_back( STeamStartPos( key, iTeamColor, vPointOnSegmentPicked, iInitialDir, iPickedSegmentIndex ) );
		}
	}
}

void KX2DungeonTool::GetMousePosOnLine( D3DXVECTOR3* pvPosOnLine, const POINTS& mousepos )
{
	if( m_pWorld == NULL )
		return;

	if( m_pWorld->GetLineMap() == NULL || m_pWorld->GetLineMap()->GetNumLineData() <= 0 )
		return;

	const D3DXMATRIX& matView = g_pKTDXApp->GetDGManager()->GetCamera().GetViewMatrix();
	D3DXMATRIX matProj;
	g_pKTDXApp->GetProjectionTransform( &matProj );

	D3DXMATRIX matViewProj, matInvViewProj;
	D3DXMatrixMultiply(&matViewProj, &matView, &matProj);
	D3DXMatrixInverse(&matInvViewProj, NULL, &matViewProj);

	float w = (float)(int)g_pKTDXApp->GetViewport().Width;
	float h = (float)(int)g_pKTDXApp->GetViewport().Height;

	float fMouseX = (float)(mousepos.x-w/2.f)/w *2.f;
	float fMouseY = (float)(h/2.f-mousepos.y)/h *2.f;
	D3DXVECTOR3 vMouse(fMouseX, fMouseY, 0.f);

	CKTDGLineMap::LineData* pLineData = NULL;	

	int iPickedSegmentIndex = 0;
	float fDistance = 0.f;
	float fMinDistance = -1.f;
	float fLerpCoef, fLerpCoefPicked;
	for( int i=0; i<this->m_pWorld->GetLineMap()->GetNumLineData(); i++ )
	{
		pLineData = m_pWorld->GetLineMap()->GetLineData(i);
		D3DXVECTOR3 vStartPos, vEndPos;
		D3DXVec3TransformCoord( &vStartPos, &pLineData->startPos, &matViewProj );
		D3DXVec3TransformCoord( &vEndPos, &pLineData->endPos, &matViewProj);
		vStartPos.z = 0.f;
		vEndPos.z   = 0.f;

		fDistance = DistanceBetweenPointAndSegment(fLerpCoef, &vMouse, &vStartPos, &vEndPos);

		if( -1.f == fMinDistance || fDistance < fMinDistance )
		{
			iPickedSegmentIndex = i;
			fMinDistance = fDistance;
			fLerpCoefPicked = fLerpCoef;
		}
	}

	float fDistanceBound = 0.1f;
	if( fMinDistance < fDistanceBound )
	{
		pLineData = m_pWorld->GetLineMap()->GetLineData(iPickedSegmentIndex);
		D3DXVECTOR3 vStartPos = pLineData->startPos;
		D3DXVECTOR3 vEndPos   = pLineData->endPos;

		D3DXVECTOR3 vPointOnSegmentPicked = vStartPos + (vEndPos-vStartPos) * fLerpCoefPicked;
		*pvPosOnLine = vPointOnSegmentPicked;
	}	
}

void KX2DungeonTool::ComputeRay( D3DXVECTOR3* pvRayOrigin, D3DXVECTOR3* pvRayDirection, const POINTS& mouspos, 
									const D3DXMATRIX* pmatView, const D3DXMATRIX* pmatProj )
{
	RECT rc;
	float w, h;
	GetClientRect( DXUTGetHWND(), &rc );
	w = (float)(rc.right - rc.left);
	h = (float)(rc.bottom - rc.top);

	float left, top;
	left = (float)(mouspos.x-w/2)/w*2.0f;
	top  = (float)(h/2-mouspos.y)/h*2.0f;

	D3DXMATRIX matViewProj, matInvViewProj;
	D3DXMatrixMultiply(&matViewProj, pmatView, pmatProj);
	D3DXMatrixInverse(&matInvViewProj, NULL, &matViewProj);

	D3DXVECTOR3 vFrom, vTo, vRay;
	vFrom = D3DXVECTOR3(left, top, -1.f);
	vTo = D3DXVECTOR3(left, top,  1.f);

	D3DXVec3TransformCoord(&vFrom, &vFrom, &matInvViewProj);
	D3DXVec3TransformCoord(&vTo, &vTo, &matInvViewProj);
	vRay = vTo-vFrom;
	D3DXVec3Normalize(&vRay, &vRay);

	*pvRayDirection = vRay;
	*pvRayOrigin = vFrom;
}

void KX2DungeonTool::CollisionCheckNPCStartPos( CX2DungeonSubStage::SubStageData* pSubStageData, D3DXVECTOR3 vRayStart, D3DXVECTOR3 vRayEnd, 
												   float fHitDistance, float fShortestHitDistance, CX2DungeonSubStage::NPCData* &pPickedNPCData, 
												   int &iPickedNPCStartIndex, CX2DungeonSubStage::SubStageData* &pInNPCSubStage )
{
	for( UINT j=0; j<pSubStageData->m_NPCDataList.size(); j++ )
	{
		CX2DungeonSubStage::NPCData* pNPCData = pSubStageData->m_NPCDataList[j];
		for( UINT k=0; k<pNPCData->m_vecStartPos.size(); k++ )
		{
			//D3DXVECTOR3 vPos = pLineMap->GetStartPosition( pNPCData->m_vecStartPos[k] );
			D3DXVECTOR3 vPos(0,0,0);

			for( UINT i=0; i<vecTeamStartPos.size(); i++ )
			{
				STeamStartPos& startPos = vecTeamStartPos[i];
				if( startPos.key == pNPCData->m_vecStartPos[k] )
				{
					vPos = startPos.vPos;
					break;
				}
			}

			if( true == NPCRayHit( vPos, vRayStart, vRayEnd, &fHitDistance ) )
			{
				if( fHitDistance < fShortestHitDistance )
				{
					pPickedNPCData = pNPCData;
					iPickedNPCStartIndex = pNPCData->m_vecStartPos[k];
					pInNPCSubStage = pSubStageData;
				}
			}
		}
	}
}

// return value는 화면(즉, 2차원 평면위)에서의 점과 선분 사이의 거리를 의미한다. 
float KX2DungeonTool::DistanceBetweenPointAndSegment(float& fLerpCoef, D3DXVECTOR3* pvPoint,
														 D3DXVECTOR3* pvSegmentStart, D3DXVECTOR3* pvSegmentEnd)
{
	D3DXVECTOR3 vTemp = *pvSegmentStart - *pvPoint;
	D3DXVECTOR3 vSegmentDir = *pvSegmentEnd - *pvSegmentStart;

	D3DXVECTOR3 vTangent;
	D3DXVec3Cross(&vTangent, &vSegmentDir, &vTemp);

	D3DXVECTOR3 vNormal;
	D3DXVec3Cross(&vNormal, &vSegmentDir, &vTangent);
	D3DXVec3Normalize(&vNormal, &vNormal);

	float fDistance = fabs( D3DXVec3Dot(&vNormal, &vTemp) );

	if( D3DXVec3Dot(&vTemp, &vNormal) < 0 )
	{
		vNormal = -vNormal;
	}

	D3DXVECTOR3 vPointOnSegment = *pvPoint + vNormal * fDistance;

	{
		D3DXVECTOR3 vTempA = vPointOnSegment - *pvSegmentStart;
		D3DXVECTOR3 vTempB;
		D3DXVec3Normalize( &vTempB, &vSegmentDir );
		fLerpCoef = D3DXVec3Dot( &vTempB, &vTempA ) / D3DXVec3Length( &vSegmentDir );
	}

	if( 0.f < fLerpCoef && fLerpCoef < 1.f )
	{
		return fDistance;
	}
	else
	{
		D3DXVECTOR3 vTempA = *pvSegmentStart - *pvPoint;
		D3DXVECTOR3 vTempB = *pvSegmentEnd   - *pvPoint;

		float fDistance1 = D3DXVec3Length( &vTempA );
		float fDistance2 = D3DXVec3Length( &vTempB );
		if( fDistance1 < fDistance2 )
		{
			fLerpCoef = 0.f;
			return fDistance1;
		}
		else
		{
			fLerpCoef = 1.f;
			return fDistance2;
		}
	}	
}

bool KX2DungeonTool::NPCRayHit( D3DXVECTOR3 vPos, D3DXVECTOR3 rayStart, D3DXVECTOR3 rayEnd, float* pHitDistance )
//bool CX2LineMapToolMain::NPCRayHit( CX2DungeonSubStage::NPCData* pNPCData, D3DXVECTOR3 rayStart, D3DXVECTOR3 rayEnd, float* pHitDistance )
{
	//D3DXVECTOR3 vPos = pLineMap->GetStartPosition( pNPCData->m_StartPos );
	m_pMatrix->Move( vPos );
	//m_pMatrix->MoveRel( pNPCData->m_AddPos );
	D3DXMATRIX matTransform = m_pMatrix->GetMatrix(); 

	float fRadius = m_pXSkinMesh->GetBoundingRadius();
	D3DXVECTOR3 vCenter = m_pXSkinMesh->GetCenter();


	D3DXVec3TransformCoord( &vCenter, &vCenter, &matTransform );
	bool bResult = false;
	D3DXVECTOR3 vCollisionPoint;

	if( g_pKTDXApp->GetCollision()->LineToSphere( rayStart, rayEnd, 1.f, 
		vCenter, fRadius, &vCollisionPoint ) == true )
	{
		bResult = true;
		if( NULL != pHitDistance )
		{
			vCollisionPoint -= rayStart;
			*pHitDistance = D3DXVec3Length( &vCollisionPoint );
		}		
	}
	else
	{
		bResult = false;
		if( NULL != pHitDistance )
		{			
			*pHitDistance = -1.f;
		}
	}

	return bResult;
}

HRESULT KX2DungeonTool::OnResetDevice()
{
	D3DVIEWPORT9 nowRes = g_pKTDXApp->GetViewport();	
	m_toolUI.MoveUIPos( nowRes.Width - m_resolution.Width , 0 );
	m_resolution = nowRes;

	// 카메라 설정
	D3DXVECTOR3 vEye	= g_pKTDXApp->GetDGManager()->GetCamera().GetEye();
	D3DXVECTOR3 vLookAt = g_pKTDXApp->GetDGManager()->GetCamera().GetLookAt();
	m_FPSCamera.SetViewParams( &vEye, &vLookAt );
	m_FPSCamera.SetProjParams( D3DX_PI/4, 1.f, g_pKTDXApp->GetDGManager()->GetNear(), g_pKTDXApp->GetDGManager()->GetFar() );
	m_FPSCamera.SetRotateButtons( false, false, true, false );

	return g_pKTDXApp->GetDGManager()->GetDialogManager()->OnResetDevice();
}

HRESULT KX2DungeonTool::OnLostDevice()
{
	return g_pKTDXApp->GetDGManager()->GetDialogManager()->OnLostDevice();
}

bool	KX2DungeonTool::LoadDungeonScriptFile()
{	
	// Enum 로드
	if( false == g_pKTDXApp->GetDeviceManager()->LoadLuaTinker( L"Enum.lua" ) )
		return false;

	// Enum String 로드
	if( !m_enumManager.OpenScriptFile( L"EnumString.lua" ) )
	{
		MessageBox( g_pKTDXApp->GetHWND(), L"EnumString.lua파일을 못읽음!", L"test", MB_OK );
		return false;
	}

	// 유닛 데이터 로드
	if( !m_unitManager.OpenScriptFile( L"UnitTemplet.lua" ) )
	{
		MessageBox( g_pKTDXApp->GetHWND(), L"UnitTemplet.lua파일을 못읽음!", L"test", MB_OK );
		return false;
	}

	// NPC 데이터 로드
	if( !m_unitManager.OpenNPCScriptFile( L"NPCTemplet.lua" ) )
	{
		MessageBox( g_pKTDXApp->GetHWND(), L"NPCTemplet.lua파일을 못읽음!", L"test", MB_OK );
		return false;
	}

	//월드 데이터 로드
	if( !m_worldManager.OpenScriptFile( L"WorldData.lua" ) )
	{
		MessageBox( g_pKTDXApp->GetHWND(), L"WorldData.lua파일을 못읽음!", L"test", MB_OK );
		return false;
	}
	
	// 던전 데이터 로드
	WCHAR buf[128] = L"";
	if( !m_dungeonManager.OpenScriptFile( L"DungeonData.lua" ) )	
	{
		MessageBox( g_pKTDXApp->GetHWND(), L"DungeonData.lua파일을 못읽음!", L"test", MB_OK );
		return false;
	}

	// Drop Table 로드
	if( !m_dropTable.LoadFromLua( "DropTable.lua" ) )
	{
		MessageBox( g_pKTDXApp->GetHWND(), L"DropTable.lua파일을 못읽음!", L"test", MB_OK );
		return false;
	}

	// NPC Scale 데이터 로드	
	std::map< CX2UnitManager::NPC_UNIT_ID, std::wstring > mapNpcList;
	m_unitManager.GetNPCUnitList( mapNpcList );
	NpcListIterator npcMit = mapNpcList.begin();
	for( ; npcMit != mapNpcList.end(); npcMit++ )
	{
		CX2UnitManager::NPCUnitTemplet* unitTemp = m_unitManager.GetNPCUnitTemplet( npcMit->first );
		if( unitTemp == NULL )
			continue;

		m_npcScaleManager.LoadScaleFromScriptFile( unitTemp->m_LuaFileName.c_str(), npcMit->first );
	}

	// 던전 리스트 업데이트 UI	
	m_dungeonManager.GetDungeonList( m_mapDungeonList );
	DungeonListIterator mit = m_mapDungeonList.begin();
	for( ; mit != m_mapDungeonList.end(); mit++ )
	{
		m_mainUI.GetListBox( IDC_DUNGEON_LIST )->AddItem( (mit->first).c_str(), NULL );
	}

	// NPC 리스트 업데이트 UI
	m_unitManager.GetNPCUnitList( m_mapNpcList );
	NpcListIterator mit2 = m_mapNpcList.begin();
	for( ; mit2 != m_mapNpcList.end(); mit2++ )
	{
		m_toolUI.GetComboBox( IDC_NPC_ID_LIST_COMBO )->AddItem( (mit2->second).c_str(), (LPVOID)(mit2->first) );
	}

	// World Name 리스트 업데이트 UI
	m_worldManager.GetWorldNameList( m_mapWorldNameList );
	WorldNameListIterator mit3 = m_mapWorldNameList.begin();
	for( ; mit3 != m_mapWorldNameList.end(); mit3++ )
	{
		m_mainUI.GetComboBox( IDC_WORLD_ID_COMBO )->AddItem( (mit3->second).c_str(), (LPVOID)(mit3->first) );
	}

	return true;
}

bool KX2DungeonTool::LoadDungeon( SEnum::DUNGEON_ID dungeonID )
{
	if( m_pEditingDungeon != NULL )
	{
		if( MessageBox( g_pKTDXApp->GetHWND(), L"기존의 던전정보가 남아있습니다. 지우고 로드하시겠습니까?", L"test", MB_OKCANCEL ) != IDOK )
			return false;
	}
	
	SAFE_DELETE( m_pEditingDungeon );
	m_pEditingStage	   = NULL;

	// 에디트할 던전 정보 생성
	return LoadDungeon( m_dungeonManager.CreateDungeon( dungeonID, true ) );
}

void KX2DungeonTool::LoadTriggerFunc(wstring szFileName)
{
    bool bLoop = false;
    wstring strFunc, strFile;
    WCHAR strbuf[1024];

    
    FILE *fp = _wfopen(szFileName.c_str() , L"r");

    if(fp == NULL)
    {        
        return;
    }


    while(!feof(fp))
    {
        fwscanf(fp, L"%s", strbuf);
        if(wcscmp(strbuf, L"function") == 0)    
        {
            bLoop = true;
            break;
        }        
    } 

    m_bTriggerFunc = false;
    if(bLoop)
    {
        m_bTriggerFunc = true;
        strFunc = L"function ";
        while(!feof(fp))
        {
            WCHAR *result;
            result = fgetws(strbuf, 1024, fp);
            if(result)            
                strFunc += strbuf;            
        }

        m_strFunc = strFunc;
    }  


    fclose(fp);
}

bool KX2DungeonTool::LoadDungeon( CX2Dungeon* pDungeon )
{
	WCHAR buf[256] = L"";	

	// 에디트할 던전 정보 생성
	m_pEditingDungeon = pDungeon;
	if( m_pEditingDungeon == NULL )
		return false;   

    wstring strFile;
    strFile = L"D:\\ProjectX2\\X2Project\\X2\\dat\\Script\\Dungeon\\";
    strFile += m_pEditingDungeon->GetDungeonData()->m_DataFileName.c_str();
    LoadTriggerFunc(strFile);

	// 던전 맵 정보 얻기
	m_mapDungeonMapList.clear();
	m_mapDungeonMapList = m_pEditingDungeon->GetMapDungeonMapData();
	//m_wsDungeonMapBG = m_pEditingDungeon->GetDungeonMapBG();
	//m_mapUI.GetEditBox( IDC_TEXTURE_FILE_NAME_EDIT )->SetText( m_wsDungeonMapBG.c_str() );

	// 던전 맵 생성
	SAFE_DELETE( m_pDungeonMapUI );
	m_pDungeonMapUI = new KX2DTDungeonMapUI( m_pEditingDungeon );
	if( m_pDungeonMapUI == NULL )
		return false;

	m_pDungeonMapUI->SetMapDungeonMapList( &m_mapDungeonMapList );

	// 던전 이름 출력
	DXUTListBoxItem* item = m_mainUI.GetListBox( IDC_DUNGEON_LIST )->GetSelectedItem();
	if( item == NULL )
		return false;

	m_menuUI.GetEditBox( IDC_NOW_EDITING_DUNGEON_EDIT )->SetText( item->strText );
	m_menuUI.GetEditBox( IDC_NOW_EDITING_STAGE_EDIT )->SetText( L"" );
	m_wsAutoSaveFile = m_wsAutoSavePath + std::wstring(item->strText) + L"_autosave";
	m_wsQuickSaveFile = m_wsQuickSavePath + std::wstring(item->strText) + L"_quicksave";

	//////////////////////////////////////////////////////////////////////////
	if( m_pEditingDungeon->GetStageDataList().empty() )
	{
		m_pEditingDungeon->SetWorldColor( D3DXCOLOR( -1, -1, -1, 1 ) );
		m_pEditingDungeon->SetUnitColor( D3DXCOLOR( -1, -1, -1, 1 ) );
	}
	//////////////////////////////////////////////////////////////////////////	

	// UI 초기화
	m_mainUI.GetListBox( IDC_STAGE_LIST )->RemoveAllItems();
	m_mapUI.GetListBox( IDC_MAP_STAGE_LIST )->RemoveAllItems();
	m_mapStageList.clear();

	// 스테이지 리스트 얻기
	std::vector< CX2DungeonStage::StageData* > vecStageList = m_pEditingDungeon->GetStageDataList();
	std::vector< CX2DungeonStage::StageData* >::iterator mit = vecStageList.begin();
	WorldNameListIterator worldNameIt;
	for(int i=0 ; mit != vecStageList.end(); mit++, i++ )
	{
		worldNameIt = m_mapWorldNameList.find( (*mit)->m_WorldID );
		wsprintf( buf, L"Stage %2d : %s", i, (worldNameIt->second).c_str() );
		m_mainUI.GetListBox( IDC_STAGE_LIST )->AddItem( buf, (LPVOID)(*mit)->m_WorldID );
		m_mapUI.GetListBox( IDC_MAP_STAGE_LIST )->AddItem( buf, (LPVOID)(*mit)->m_WorldID );
		m_mapStageList.insert( std::make_pair( std::wstring(buf), (*mit)->m_WorldID ));
	}

	// 던전 월드 컬러 및 유닛 컬러	
	m_mainUI.GetEditBox( IDC_DUNGEON_WORLD_COLOR_R )->SetText( INT_TO_WSTRING( m_pEditingDungeon->GetWorldColor().r ) );
	m_mainUI.GetEditBox( IDC_DUNGEON_WORLD_COLOR_G )->SetText( INT_TO_WSTRING( m_pEditingDungeon->GetWorldColor().g ) );
	m_mainUI.GetEditBox( IDC_DUNGEON_WORLD_COLOR_B )->SetText( INT_TO_WSTRING( m_pEditingDungeon->GetWorldColor().b ) );
	m_mainUI.GetEditBox( IDC_DUNGEON_UNIT_COLOR_R )->SetText( INT_TO_WSTRING( m_pEditingDungeon->GetUnitColor().r ) );
	m_mainUI.GetEditBox( IDC_DUNGEON_UNIT_COLOR_G )->SetText( INT_TO_WSTRING( m_pEditingDungeon->GetUnitColor().g ) );
	m_mainUI.GetEditBox( IDC_DUNGEON_UNIT_COLOR_B )->SetText( INT_TO_WSTRING( m_pEditingDungeon->GetUnitColor().b ) );
	return true;
}

bool KX2DungeonTool::LoadDungeonFromFile( WCHAR* wszFileName )
{
	WCHAR buf[256] = L"";

	if( m_pEditingDungeon != NULL )
	{
		if( MessageBox( g_pKTDXApp->GetHWND(), L"기존의 던전정보가 남아있습니다. 지우고 로드하시겠습니까?", L"test", MB_OKCANCEL ) == IDOK )
		{
			SAFE_DELETE( m_pEditingDungeon );
		}
		else
		{
			return false;
		}		
	}

	WCHAR FileName[256] = L"";
	WCHAR drive[_MAX_DRIVE];
	WCHAR dir[_MAX_DIR];
	WCHAR fname[_MAX_FNAME];
	WCHAR ext[_MAX_EXT];

	_wsplitpath( wszFileName, drive, dir, fname, ext);
	wcscat( FileName, fname);
	wcscat( FileName, ext);	

	// 에디트할 던전 정보 생성
	CX2Dungeon::DungeonData dungeonData;
	dungeonData.m_DataFileName = FileName;
	m_pEditingDungeon = new CX2Dungeon( &dungeonData );
	if( m_pEditingDungeon == NULL )
		return false;

	// UI 초기화
	m_mainUI.GetListBox( IDC_STAGE_LIST )->RemoveAllItems();
	m_mapStageList.clear();	

	// 스테이지 리스트 얻기
	std::vector< CX2DungeonStage::StageData* > vecStageList = m_pEditingDungeon->GetStageDataList();
	std::vector< CX2DungeonStage::StageData* >::iterator mit = vecStageList.begin();
	WorldNameListIterator worldNameIt;
	for(int i=0 ; mit != vecStageList.end(); mit++, i++ )
	{
		worldNameIt = m_mapWorldNameList.find( (*mit)->m_WorldID );

		wsprintf( buf, L"Stage %2d : %s", i, (worldNameIt->second).c_str() );
		m_mainUI.GetListBox( IDC_STAGE_LIST )->AddItem( buf, (LPVOID)(*mit)->m_WorldID );
		m_mapStageList.insert( std::make_pair( std::wstring(buf), (*mit)->m_WorldID ));
	}

    wstring fileName = wszFileName;
    LoadTriggerFunc(fileName);

	return true;
}

bool KX2DungeonTool::LoadStageInfo( int selNum )
{
	if( m_pEditingDungeon == NULL )
		return false;

	// 선택한 스테이지 정보 얻기
	m_pEditingStage = m_pEditingDungeon->GetStageDataList()[selNum];
		
	////////////////////////////////////////////////////////// Load World Info
	SAFE_DELETE( m_pWorld );
	m_pWorld = m_worldManager.CreateWorld( m_pEditingStage->m_WorldID );
	if( m_pWorld == NULL )
		return false;
	
	// dungeon script에서 지정된 color를 world에 적용
	if( m_pEditingDungeon->GetWorldColor().r != -1.f )
	{
		m_pWorld->SetOriginColor_LUA( m_pEditingDungeon->GetWorldColor().r, m_pEditingDungeon->GetWorldColor().g, m_pEditingDungeon->GetWorldColor().b );
	}
	else if( m_pEditingStage->m_WorldColor.r != -1.f )
	{
		m_pWorld->SetOriginColor_LUA( m_pEditingStage->m_WorldColor.r, m_pEditingStage->m_WorldColor.g, m_pEditingStage->m_WorldColor.b );
	}

	if( m_pEditingDungeon->GetUnitColor().r != -1.f )
	{
		m_pWorld->SetLightColor_LUA( m_pEditingDungeon->GetUnitColor().r, m_pEditingDungeon->GetUnitColor().g, m_pEditingDungeon->GetUnitColor().b );
	}

	else if( m_pEditingStage->m_UnitColor.r != -1.f )
	{
		m_pWorld->SetLightColor_LUA( m_pEditingStage->m_UnitColor.r, m_pEditingStage->m_UnitColor.g, m_pEditingStage->m_UnitColor.b );
	}

	m_veciPickedSegmentIndex.clear();
	
	return LoadStageInfo( m_pEditingStage );
}

bool KX2DungeonTool::LoadStageInfo( CX2DungeonStage::StageData* pStageData )
{
	WCHAR buf[256] = L"";

	if( pStageData == NULL )
		return false;

	// 스테이지 이름 출력
	DXUTListBoxItem* item = m_mainUI.GetListBox( IDC_STAGE_LIST )->GetSelectedItem();
	if( item == NULL )
		return false;

	m_menuUI.GetEditBox( IDC_NOW_EDITING_STAGE_EDIT )->SetText( item->strText );

	// UI 초기화
	RemoveAllItemsToolUI();

	////////////////////////////////////////////////////////// Update Stage Info	
	std::vector< CX2DungeonSubStage::SubStageData* >::iterator mit2 = pStageData->m_SubStageDataList.begin();
	for(int i=0 ; mit2 != pStageData->m_SubStageDataList.end(); mit2++, i++ )
	{
		wsprintf( buf, L"Sub Stage %d", i );
		m_toolUI.GetListBox( IDC_SUB_STAGE_LIST )->AddItem( buf, NULL );
	}

	// 선택된 행을 초기화 안해주면 삭제된 부분 클릭시 크래쉬남
	if( m_iDelSubStage == pStageData->m_SubStageDataList.size() )
	{
		m_toolUI.GetListBox( IDC_SUB_STAGE_LIST )->SelectItem( pStageData->m_SubStageDataList.size() - 1 );
	}

	std::vector< CX2UnitManager::NPC_UNIT_ID >::iterator mit3 = pStageData->m_ReadyNPCList.begin();
	for( ; mit3 != pStageData->m_ReadyNPCList.end(); mit3++ )
	{
		NpcListIterator npcIt = m_mapNpcList.find( *mit3 );
		if( npcIt != m_mapNpcList.end() )
		{			
			m_toolUI.GetListBox( IDC_READY_NPC_LIST )->AddItem( (npcIt->second).c_str(), (LPVOID)(npcIt->first) );
			m_toolUI.GetComboBox( IDC_NPC_LIST_COMBO )->AddItem( (npcIt->second).c_str(), (LPVOID)(npcIt->first) );
		}
	}
	 
	// 선택된 행을 초기화 안해주면 삭제된 부분 클릭시 크래쉬남
	if( m_iDelReadyNpc == pStageData->m_ReadyNPCList.size() )
	{
		m_toolUI.GetListBox( IDC_READY_NPC_LIST )->SelectItem( pStageData->m_ReadyNPCList.size() - 1 );
	}

	m_toolUI.GetCheckBox( IDC_START_MOTION_CHECK )->SetChecked( pStageData->m_bStartMotion );
	m_toolUI.GetEditBox( IDC_WORLD_COLOR_R )->SetText( INT_TO_WSTRING( pStageData->m_WorldColor.r ) );
	m_toolUI.GetEditBox( IDC_WORLD_COLOR_G )->SetText( INT_TO_WSTRING( pStageData->m_WorldColor.g ) );
	m_toolUI.GetEditBox( IDC_WORLD_COLOR_B )->SetText( INT_TO_WSTRING( pStageData->m_WorldColor.b ) );
	m_toolUI.GetEditBox( IDC_UNIT_COLOR_R )->SetText( INT_TO_WSTRING( pStageData->m_UnitColor.r ) );
	m_toolUI.GetEditBox( IDC_UNIT_COLOR_G )->SetText( INT_TO_WSTRING( pStageData->m_UnitColor.g ) );
	m_toolUI.GetEditBox( IDC_UNIT_COLOR_B )->SetText( INT_TO_WSTRING( pStageData->m_UnitColor.b ) );
	
	m_menuUI.GetCheckBox( IDC_WORLD_SHOW_CHECK )->SetChecked( m_pWorld->GetRenderingWorld() );
	m_menuUI.GetCheckBox( IDC_LINE_MAP_SHOW_CHECK )->SetChecked( m_pWorld->GetRenderingLineMap() );
	m_menuUI.GetEditBox( IDC_LINE_NUMBER_EDIT )->SetText( L"" );

	////////////////////////////////////////////////////////// Load NPC Mesh
	LoadNPCMeshs();

	// 에디트용 포인터 초기화
	m_pEditingSubStage	 = NULL;
	m_pEditingCurtain	 = NULL;
	m_pEditingNpc		 = NULL;
	m_pEditingTrigger	 = NULL;

	return true;
}

void KX2DungeonTool::LoadNPCMeshs()
{
	DeleteNPCSkinAnims();
	LoadNPCSkinAnims();

	if( m_pWorld != NULL && m_pWorld->GetLineMap() != NULL )
	{
		CKTDGLineMap* pLineMap = m_pWorld->GetLineMap();
		WCHAR wszOutput[4096];
		StringCchPrintf( wszOutput, 512, L"%.2f", pLineMap->GetLandHeight() );

		int i; 
		for( i=0; i<sizeof(m_abKeyUsed); i++ )
		{
			m_abKeyUsed[i] = false;
		}
		vecTeamStartPos.clear();
		int iTempInitialDir;
		for( i=0; i<pLineMap->GetRedTeamStartPosNum(); i++ )
		{
			if( pLineMap->GetRedTeamStartRight(i) == true )
				iTempInitialDir = DIR_RIGHT;
			else
				iTempInitialDir = DIR_LEFT;

			int iLineIndex = pLineMap->GetRedTeamStartLineIndex(i);

			vecTeamStartPos.push_back( STeamStartPos( -1, TEAM_RED, pLineMap->GetRedTeamStartPosition(i), iTempInitialDir, iLineIndex) );
		}

		for( i=0; i<pLineMap->GetBlueTeamStartPosNum(); i++ )
		{
			if( pLineMap->GetBlueTeamStartRight(i) == true )
				iTempInitialDir = DIR_RIGHT;
			else
				iTempInitialDir = DIR_LEFT;

			int iLineIndex = pLineMap->GetBlueTeamStartLineIndex(i);

			vecTeamStartPos.push_back( STeamStartPos( -1, TEAM_BLUE, pLineMap->GetBlueTeamStartPosition(i), iTempInitialDir, iLineIndex) );
		}

#ifdef  X2OPTIMIZE_LINEMAP_OPTIMIZE_AND_BUG_FIX
		const CKTDGLineMap::StartInfo& mapStartInfo = pLineMap->GetStartInfoMap();
		CKTDGLineMap::StartInfo::const_iterator it;
		for( it=mapStartInfo.begin(); it!=mapStartInfo.end(); it++ )
		{
			if( true == it->second.m_bStartRight )
				iTempInitialDir = DIR_RIGHT;
			else
				iTempInitialDir = DIR_LEFT;
			m_abKeyUsed[ (int) it->first ] = true;
			vecTeamStartPos.push_back( STeamStartPos( (int)it->first, TEAM_NONE, 
                it->second.m_vStartPosition, iTempInitialDir, it->second.m_iStartLineIndex ) );
		}
#else   X2OPTIMIZE_LINEMAP_OPTIMIZE_AND_BUG_FIX
		map<int, D3DXVECTOR3>& mapStartPos = pLineMap->GetStartPosMap();
		map<int, D3DXVECTOR3>::iterator it;
		for( it=mapStartPos.begin(); it!=mapStartPos.end(); it++ )
		{
			if( true == pLineMap->GetStartRight( (int)it->first ) )
				iTempInitialDir = DIR_RIGHT;
			else
				iTempInitialDir = DIR_LEFT;

			int iLineIndex = pLineMap->GetStartLineIndex( (int)it->first );

			m_abKeyUsed[ (int)it->first ] = true;
			vecTeamStartPos.push_back( STeamStartPos( (int)it->first, TEAM_NONE, pLineMap->GetStartPosition((int)it->first), iTempInitialDir, iLineIndex ) );
			//SNPCStartPos* pNPCStartPos = new SNPCStartPos( TEAM_NONE, pLineMap->GetStartPosition(i), iTempInitialDir, iLineIndex );
			//m_vecpNPCStartPos.push_back( pNPCStartPos );			
		}
#endif  X2OPTIMIZE_LINEMAP_OPTIMIZE_AND_BUG_FIX

		/*m_iPickedStartPosIndex = -1;		

		m_veciPickedSegmentIndex.clear();		

		if( m_nVBHighlightedSegmentSize < (unsigned)2*GetNumSegments() )
		{
		m_nVBHighlightedSegmentSize = (unsigned)2*GetNumSegments();

		SAFE_RELEASE(m_pVBHighlightedSegment);
		g_pKTDXApp->GetDevice()->CreateVertexBuffer( m_nVBHighlightedSegmentSize * sizeof(LINE_MAP_VERTEX), 
		D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, D3DFVF_LINE_MAP_VERTEX, 
		D3DPOOL_SYSTEMMEM, &m_pVBHighlightedSegment, NULL);
		}*/
	}
}

bool KX2DungeonTool::LoadSubStageInfo( int iStageNum, bool bIsInitSelected )
{
	if( m_pEditingStage == NULL )
		return false;

	return LoadSubStageInfo( m_pEditingStage->m_SubStageDataList[iStageNum], bIsInitSelected );
}

bool KX2DungeonTool::LoadSubStageInfo( CX2DungeonSubStage::SubStageData* pSubStageData, bool bIsInitSelected )
{
	WCHAR buf[256] = L"";

	if( m_pEditingStage == NULL || pSubStageData == NULL )
		return false;
	
	m_pEditingSubStage = pSubStageData;
	//////////////////////////////////////// 힙의 노멀블럭을 갉아 먹는다!?
	//m_backupSubStage.m_CurtainDataList	 = m_pEditingSubStage->m_CurtainDataList;
	m_backupSubStage.m_EndLineSet		 = m_pEditingSubStage->m_EndLineSet;
	m_backupSubStage.m_GoType			 = m_pEditingSubStage->m_GoType;
	m_backupSubStage.m_MainLineSet		 = m_pEditingSubStage->m_MainLineSet;
	//m_backupSubStage.m_NPCDataList	 = m_pEditingSubStage->m_NPCDataList; <--- 이녀석
	//m_backupSubStage.m_NPCList			 = m_pEditingSubStage->m_NPCList;
	m_backupSubStage.m_StartLineSet		 = m_pEditingSubStage->m_StartLineSet;
	//m_backupSubStage.m_TriggerList		 = m_pEditingSubStage->m_TriggerList;
	//////////////////////////////////////////////////////////////////////////
	
	/////////////////////////////////////////////////////// Update Curtain
	m_toolUI.GetListBox( IDC_CURTAIN_LIST )->RemoveAllItems( bIsInitSelected );
	vector<CX2DungeonSubStage::CurtainData*> vecCurtainList = m_pEditingSubStage->m_CurtainDataList;
	vector<CX2DungeonSubStage::CurtainData*>::iterator vit = vecCurtainList.begin();
	for(int i=0 ; vit != vecCurtainList.end(); vit++, i++ )
	{
		wsprintf( buf, L"%2d번 커튼", i );
		m_toolUI.GetListBox( IDC_CURTAIN_LIST )->AddItem( buf, NULL );
	}
	// 선택된 행을 초기화 안해주면 삭제된 부분 클릭시 크래쉬남
	if( m_iDelCurtain == vecCurtainList.size() )
	{
		m_toolUI.GetListBox( IDC_CURTAIN_LIST )->SelectItem( vecCurtainList.size() - 1 );
	}
	
	if( vecCurtainList.empty() == false )
	{
		LoadCurtainInfo( 0 );
		m_toolUI.GetListBox( IDC_CURTAIN_LIST )->SelectItem( 0 );
	}
	else
	{
		RemoveCurtainToolUI();
	}

	///////////////////////////////////////////////////////// Update NPC
	m_toolUI.GetListBox( IDC_NPC_LIST )->RemoveAllItems( bIsInitSelected );
	NpcListIterator npcIt;
	std::vector<CX2DungeonSubStage::NPCData*> vecNpcList = m_pEditingSubStage->m_NPCDataList;
	std::vector<CX2DungeonSubStage::NPCData*>::iterator vit2 = vecNpcList.begin();
	for(int i=0 ; vit2 != vecNpcList.end(); vit2++, i++ )
	{
		if( !(*vit2)->m_bSubNpc )
		{
			npcIt = m_mapNpcList.find( (*vit2)->m_UnitID );
			wsprintf( buf, L"%2d번 NPC : %s", i, (npcIt->second).c_str() );
		}
		else
		{
			npcIt = m_mapNpcList.find( (*vit2)->m_UnitID );
			wsprintf( buf, L"%2d번 Sub : %s", i, (npcIt->second).c_str() );
		}

		m_toolUI.GetListBox( IDC_NPC_LIST )->AddItem( buf, NULL );
	}
	// 선택된 행을 초기화 안해주면 삭제된 부분 클릭시 크래쉬남
	if( m_iDelNpc == vecNpcList.size() )
	{
		m_toolUI.GetListBox( IDC_NPC_LIST )->SelectItem( vecNpcList.size() - 1 );
	}
	
	if( vecNpcList.empty() == false )
	{
		LoadNpcInfo( 0 );
		m_toolUI.GetListBox( IDC_NPC_LIST )->SelectItem( 0 );
	}
	else
	{
		RemoveNpcToolUI();
	}

	///////////////////////////////////////////////////////// Update Trigger
	m_toolUI.GetListBox( IDC_TRIGGER_LIST )->RemoveAllItems( bIsInitSelected );
	std::vector<CX2DungeonSubStage::Trigger*> vecTriggerList = m_pEditingSubStage->m_TriggerList;
	std::vector<CX2DungeonSubStage::Trigger*>::iterator vit3 = vecTriggerList.begin();
	for(int i=0 ; vit3 != vecTriggerList.end(); vit3++, i++ )
	{
		wsprintf( buf, L"%2d번 trigger", i );
		m_toolUI.GetListBox( IDC_TRIGGER_LIST )->AddItem( buf, NULL );
	}
	// 선택된 행을 초기화 안해주면 삭제된 부분 클릭시 크래쉬남
	if( m_iDelTrigger == vecTriggerList.size() )
	{
		m_toolUI.GetListBox( IDC_TRIGGER_LIST )->SelectItem( vecTriggerList.size() - 1 );
	}
		
	if( vecTriggerList.empty() == false )
	{
        LoadTriggerInfo( 0 );
		m_toolUI.GetListBox( IDC_TRIGGER_LIST )->SelectItem( 0 );
	}
	else
	{
		RemoveTriggerToolUI();
	}

	///////////////////////////////////////////////////////// Update SubStageInfo
	wsprintf( buf, L"%d", m_pEditingSubStage->m_StartLineSet );
	m_toolUI.GetEditBox( IDC_START_LINE_SET_EDIT )->SetText( buf );
	m_toolUI.GetEditBox( IDC_START_LINE_SET_EDIT )->SetTextColor( (D3DCOLOR)BLACK_COLOR );
	wsprintf( buf, L"%d", m_pEditingSubStage->m_MainLineSet );
	m_toolUI.GetEditBox( IDC_MAIN_LINE_SET_EDIT )->SetText( buf );
	m_toolUI.GetEditBox( IDC_MAIN_LINE_SET_EDIT )->SetTextColor( (D3DCOLOR)BLACK_COLOR );
	wsprintf( buf, L"%d", m_pEditingSubStage->m_EndLineSet );
	m_toolUI.GetEditBox( IDC_END_LINE_SET_EDIT )->SetText( buf );
	m_toolUI.GetEditBox( IDC_END_LINE_SET_EDIT )->SetTextColor( (D3DCOLOR)BLACK_COLOR );
	m_toolUI.GetComboBox( IDC_GO_TYPE_COMBO )->SetSelectedByIndex( m_pEditingSubStage->m_GoType );

	////////////////////////////////////////////////////////// Insert Stage List
	CDXUTComboBox *pCombo;
	pCombo = m_LeftToolUI.GetComboBox( IDC_CLEAR_CONDITION_STAGE_COMBO );
	pCombo->RemoveAllItems();	
	for( int iStage = 0; iStage < m_pEditingDungeon->GetNumberOfStageData(); iStage++ )
	{
		wsprintf( buf, L"%2d", iStage );
		pCombo->AddItem( buf, reinterpret_cast<LPVOID>(iStage) );
	}
	
	///////////////////////////////////////////////////////// Update ClearCondition
	m_LeftToolUI.GetListBox( IDC_CLEAR_CONDITION_LIST )->RemoveAllItems( bIsInitSelected );
	vector<CX2DungeonSubStage::ClearCondtionData> vecClearConditionList = m_pEditingSubStage->m_vecClearCondData;
	vector<CX2DungeonSubStage::ClearCondtionData>::iterator vit4 = vecClearConditionList.begin();
	for(int i=0 ; vit4 != vecClearConditionList.end(); vit4++, i++ )
	{
		wsprintf( buf, L"CLEAR_CONDITION %2d", i );
		m_LeftToolUI.GetListBox( IDC_CLEAR_CONDITION_LIST )->AddItem( buf, NULL );
	}
	// 선택된 행을 초기화 안해주면 삭제된 부분 클릭시 크래쉬남
	if( m_iDelClearCon == vecClearConditionList.size() )
	{
		m_LeftToolUI.GetListBox( IDC_CLEAR_CONDITION_LIST )->SelectItem( vecClearConditionList.size() - 1 );
	}

	if( vecClearConditionList.empty() == false )
	{
		LoadClearCondition( 0 );
		m_LeftToolUI.GetListBox( IDC_CLEAR_CONDITION_LIST )->SelectItem( 0 );
	}
	else
	{
		RemoveClearConditionToolUI();
	}	

	return true;
}

bool KX2DungeonTool::LoadCurtainInfo( int iCurtainNum )
{
	if( m_pEditingSubStage == NULL )
		return false;

	return LoadCurtainInfo( m_pEditingSubStage->m_CurtainDataList[iCurtainNum] );
}

bool KX2DungeonTool::LoadCurtainInfo( CX2DungeonSubStage::CurtainData* pCurtainData )
{
	WCHAR buf[128] = L"";

	if( m_pEditingSubStage == NULL )
		return false;

	m_pEditingCurtain = pCurtainData;
	m_backupCurtain = *m_pEditingCurtain;

	wsprintf( buf, L"%d", m_pEditingCurtain->m_LineIndex );
	m_toolUI.GetEditBox( IDC_LINE_INDEX_EDIT )->SetText( buf );
	m_toolUI.GetEditBox( IDC_LINE_INDEX_EDIT )->SetTextColor( (D3DCOLOR)BLACK_COLOR );
	m_toolUI.GetCheckBox( IDC_AT_START_CHECK )->SetChecked( m_pEditingCurtain->m_bAtStart );
	m_toolUI.GetCheckBox( IDC_LOOK_LEFT_CHECK )->SetChecked( m_pEditingCurtain->m_bLookLeft );

	return true;
}

bool KX2DungeonTool::LoadNpcInfo( int iNpcNum )
{
	if( m_pEditingSubStage == NULL )
		return false;

	return LoadNpcInfo( m_pEditingSubStage->m_NPCDataList[iNpcNum] );
}

bool KX2DungeonTool::LoadNpcInfo( CX2DungeonSubStage::NPCData* pNpcData )
{
	WCHAR buf[128] = L"";

	if( m_pEditingSubStage == NULL )
		return false;

	m_pEditingNpc = pNpcData;
	m_backupNpc = *m_pEditingNpc;

	wsprintf( buf, L"%d", m_pEditingNpc->m_UnitID );
	m_toolUI.GetEditBox( IDC_NPC_ID_EDIT )->SetText( buf );
	m_toolUI.GetEditBox( IDC_NPC_ID_EDIT )->SetTextColor( (D3DCOLOR)BLACK_COLOR );
	m_toolUI.GetEditBox( IDC_START_POS2_EDIT )->SetText( L"" );
	m_toolUI.GetEditBox( IDC_START_POS2_EDIT )->SetTextColor( (D3DCOLOR)BLACK_COLOR );
	std::vector<int>::iterator mit3 = m_pEditingNpc->m_vecStartPos.begin();
	for(int i=0 ; mit3 != m_pEditingNpc->m_vecStartPos.end(); mit3++, i++ )
	{
		if( i == 0 )
		{
			wsprintf( buf, L"%d", *mit3 );
			m_toolUI.GetEditBox( IDC_START_POS1_EDIT )->SetText( buf );
			m_toolUI.GetEditBox( IDC_START_POS1_EDIT )->SetTextColor( (D3DCOLOR)BLACK_COLOR );
		}
		else if( i == 1 )
		{
			wsprintf( buf, L"%d", *mit3 );
			m_toolUI.GetEditBox( IDC_START_POS2_EDIT )->SetText( buf );
			m_toolUI.GetEditBox( IDC_START_POS2_EDIT )->SetTextColor( (D3DCOLOR)BLACK_COLOR );
		}
	}
	wsprintf( buf, L"%d", (int)m_pEditingNpc->m_AddPos.y );
	m_toolUI.GetEditBox( IDC_ADD_POS_Y_EDIT )->SetText( buf );
	m_toolUI.GetEditBox( IDC_ADD_POS_Y_EDIT )->SetTextColor( (D3DCOLOR)BLACK_COLOR );
	wsprintf( buf, L"%d", m_pEditingNpc->m_Level );
	m_toolUI.GetEditBox( IDC_LEVEL_EDIT )->SetText( buf );
	m_toolUI.GetEditBox( IDC_LEVEL_EDIT )->SetTextColor( (D3DCOLOR)BLACK_COLOR );
	m_toolUI.GetCheckBox( IDC_ACTIVE_CHECK )->SetChecked( m_pEditingNpc->m_bActive );
	m_toolUI.GetCheckBox( IDC_GAGE_BAR_CHECK )->SetChecked( m_pEditingNpc->m_bShowGage );
	wsprintf( buf, L"%d", m_pEditingNpc->m_Rate );
	m_toolUI.GetEditBox( IDC_RATE_EDIT )->SetText( buf );
	m_toolUI.GetEditBox( IDC_RATE_EDIT )->SetTextColor( (D3DCOLOR)BLACK_COLOR );
	wsprintf( buf, L"%d", m_pEditingNpc->m_KeyCode );
	m_toolUI.GetEditBox( IDC_NPC_KEY_CODE_EDIT )->SetText( buf );
	m_toolUI.GetEditBox( IDC_NPC_KEY_CODE_EDIT )->SetTextColor( (D3DCOLOR)BLACK_COLOR );
	m_toolUI.GetCheckBox( IDC_FOCUS_CAMERA_CHECK )->SetChecked( m_pEditingNpc->m_bFocusCamera );
	m_toolUI.GetCheckBox( IDC_HAVE_BOSS_GAGE_CHECK )->SetChecked( m_pEditingNpc->m_bHasBossGage );
	m_toolUI.GetCheckBox( IDC_SUB_NPC_CHECK )->SetChecked( m_pEditingNpc->m_bSubNpc );
	m_toolUI.GetCheckBox( IDC_SIEGE_MODE_CHECK )->SetChecked( m_pEditingNpc->m_bSiegeMode );
	m_toolUI.GetCheckBox( IDC_IS_RIGHT_CHECK )->SetChecked( m_pEditingNpc->m_bIsRight );

	return true;
}

bool KX2DungeonTool::LoadTriggerInfo( int iTriggerNum )
{
	if( m_pEditingSubStage == NULL )
		return false;

	return LoadTriggerInfo( m_pEditingSubStage->m_TriggerList[iTriggerNum] );	
}

bool KX2DungeonTool::LoadTriggerInfo( CX2DungeonSubStage::Trigger* pTrigger )
{
	WCHAR buf[128] = L"";

	if( m_pEditingSubStage == NULL )
		return false;

	m_pEditingTrigger = pTrigger;
	m_backupTrigger = *m_pEditingTrigger;

	m_toolUI.GetCheckBox( IDC_HOST_CHECK )->SetChecked( m_pEditingTrigger->m_bHost );
	m_toolUI.GetCheckBox( IDC_ONE_TIME_CHECK )->SetChecked( m_pEditingTrigger->m_bOneTime );
	wsprintf( buf, L"%d", m_pEditingTrigger->m_Interval );
	m_toolUI.GetEditBox( IDC_INTERVAL_EDIT )->SetText( buf );
	m_toolUI.GetEditBox( IDC_INTERVAL_EDIT )->SetTextColor( (D3DCOLOR)BLACK_COLOR );
	m_toolUI.GetEditBox( IDC_FUNCTION_VIEWER_EDIT )->SetText( m_pEditingTrigger->m_ConditionFunc.c_str() );
	m_toolUI.GetEditBox( IDC_FUNCTION_VIEWER_EDIT )->SetTextColor( (D3DCOLOR)BLACK_COLOR );

	return true;
}

bool KX2DungeonTool::LoadClearCondition( int iClearConNum )
{
	if( m_pEditingSubStage == NULL )
		return false;

	return LoadClearCondition( m_pEditingSubStage->m_vecClearCondData[iClearConNum] );
}

bool KX2DungeonTool::LoadClearCondition( CX2DungeonSubStage::ClearCondtionData& pClearCon )
{
	WCHAR buf[128] = L"";

	if( m_pEditingSubStage == NULL )
		return false;

	m_pEditingClearCon = &pClearCon;
	m_backupClearCon = pClearCon;

	m_LeftToolUI.GetComboBox( IDC_CLEAR_CONDITION_COMBO )->SetSelectedByIndex( static_cast<UINT>(m_pEditingClearCon->m_eClearCondition) );

	// Load Condition Elements
	LoadConditionElements( m_pEditingClearCon->m_eClearCondition );

	m_LeftToolUI.GetListBox( IDC_NEXT_STAGE_LIST )->RemoveAllItems();
	std::vector<CX2DungeonSubStage::NextStageData> vecNextStageList = m_pEditingClearCon->m_vecNextStage;
	std::vector<CX2DungeonSubStage::NextStageData>::iterator vit = vecNextStageList.begin();
	for(int i=0 ; vit != vecNextStageList.end(); vit++, i++ )
	{
		wsprintf( buf, L"NEXT_STAGE %2d", i );
		m_LeftToolUI.GetListBox( IDC_NEXT_STAGE_LIST )->AddItem( buf, NULL );
	}
	// 선택된 행을 초기화 안해주면 삭제된 부분 클릭시 크래쉬남
	if( m_iDelClearCon == vecNextStageList.size() )
	{
		m_LeftToolUI.GetListBox( IDC_NEXT_STAGE_LIST )->SelectItem( vecNextStageList.size() - 1 );
	}

	if( vecNextStageList.empty() == false )
	{
		LoadClearConNextStage( 0 );
		m_LeftToolUI.GetListBox( IDC_NEXT_STAGE_LIST )->SelectItem( 0 );
	}
	else
	{
		RemoveClearConNextStageToolUI();
	}

	return true;
}

bool KX2DungeonTool::LoadClearConNextStage( int iNextStageNum )
{
	if( m_pEditingClearCon == NULL )
		return false;

	return LoadClearConNextStage( m_pEditingClearCon->m_vecNextStage[iNextStageNum] );
}

bool KX2DungeonTool::LoadClearConNextStage( CX2DungeonSubStage::NextStageData& pNextStageData )
{
	WCHAR buf[128] = L"";

	if( m_pEditingClearCon == NULL )
		return false;

	m_pEditingNextStage = &pNextStageData;
	m_backupNextStage = pNextStageData;

	m_LeftToolUI.GetComboBox( IDC_CLEAR_TYPE_COMBO )->SetSelectedByIndex( static_cast<UINT>(m_pEditingNextStage->m_eClearType) );
	if( m_LeftToolUI.GetComboBox( IDC_CLEAR_CONDITION_STAGE_COMBO )->GetNumItems() >= static_cast<UINT>(m_pEditingNextStage->m_iStageIndex) )
	{
		m_LeftToolUI.GetComboBox( IDC_CLEAR_CONDITION_STAGE_COMBO )->SetSelectedByIndex( static_cast<UINT>(m_pEditingNextStage->m_iStageIndex) );
	}

	// Update SubStage List
	CDXUTComboBox *pCombo;
	pCombo = m_LeftToolUI.GetComboBox( IDC_CLEAR_CONDITION_SUBSTAGE_COMBO );
	pCombo->RemoveAllItems();
	CX2DungeonStage::StageData* pStageData = m_pEditingDungeon->GetStageDataList()[m_pEditingNextStage->m_iStageIndex]; // 크래쉬 날수도 -ㅅ -..
	if( pStageData == NULL )
		return false;

	for( UINT iSubStage = 0; iSubStage < pStageData->m_SubStageDataList.size(); iSubStage++ )
	{
		wsprintf( buf, L"%2d", iSubStage );
		pCombo->AddItem( buf, reinterpret_cast<LPVOID>(iSubStage) );
	}

	if( m_LeftToolUI.GetComboBox( IDC_CLEAR_CONDITION_SUBSTAGE_COMBO )->GetNumItems() >= static_cast<UINT>(m_pEditingNextStage->m_iSubStageIndex) )
	{
		m_LeftToolUI.GetComboBox( IDC_CLEAR_CONDITION_SUBSTAGE_COMBO )->SetSelectedByIndex( static_cast<UINT>(m_pEditingNextStage->m_iSubStageIndex) );
	}
	wsprintf( buf, L"%d", m_pEditingNextStage->m_iRate );
	m_LeftToolUI.GetEditBox( IDC_CLEAR_CONDITION_RATE_EDIT )->SetText( buf );
	m_LeftToolUI.GetEditBox( IDC_CLEAR_CONDITION_RATE_EDIT )->SetTextColor( (D3DCOLOR)BLACK_COLOR );

	return true;
}

void KX2DungeonTool::LoadNPCSkinAnims()
{
	map< std::pair<CX2DungeonSubStage::NPCData*, int>, CKTDGXSkinAnim* >::iterator it;

	if( m_pEditingStage != NULL )
	{
		for( UINT i=0; i<m_pEditingStage->m_SubStageDataList.size(); i++ )
		{
			CX2DungeonSubStage::SubStageData* pSubStageData = m_pEditingStage->m_SubStageDataList[i];
			for( UINT j=0; j< pSubStageData->m_NPCDataList.size(); j++ )
			{
				CX2DungeonSubStage::NPCData* pNPCData = pSubStageData->m_NPCDataList[j];
				for( UINT k=0; k<pNPCData->m_vecStartPos.size(); k++ )
				{
					it = m_mapNPCXSkinAnim.find( std::make_pair( pNPCData, k ) );
					if( it == m_mapNPCXSkinAnim.end() )
					{
						LoadNPCSkinAnim( pNPCData, k );
					}
				}
			}
		}
	}
}

CKTDGXSkinAnim* KX2DungeonTool::LoadNPCSkinAnim( CX2DungeonSubStage::NPCData* pNPCData, int iSubIndex )
{
	map< std::pair<CX2DungeonSubStage::NPCData*, int >, CKTDGXSkinAnim* >::iterator it;
	//it = m_mapNPCXSkinAnim.find( pNPCData );
	it = m_mapNPCXSkinAnim.find( std::make_pair( pNPCData, iSubIndex ) );
	if( it != m_mapNPCXSkinAnim.end() )
	{
		CKTDGXSkinAnim* pSkinAnim = (CKTDGXSkinAnim*) it->second;
		SAFE_DELETE( pSkinAnim );
		m_mapNPCXSkinAnim.erase(it);
	}

	CKTDXDeviceXSkinMesh* pSkinMesh = GetNPCSkinMesh( pNPCData->m_UnitID );
	if( NULL != pSkinMesh )
	{
		CKTDGXSkinAnim* pXSkinAnim = new CKTDGXSkinAnim( g_pKTDXApp->GetDevice() );
		pXSkinAnim->SetAnimXSkinMesh(  pSkinMesh );
		pXSkinAnim->AddModelXSkinMesh( pSkinMesh );
		pXSkinAnim->SetAlphaObject( true );
		pXSkinAnim->ChangeAnim( L"Wait", false );
		pXSkinAnim->Play( CKTDGXSkinAnim::XAP_ONE_WAIT );

		m_mapNPCXSkinAnim[ std::make_pair( pNPCData, iSubIndex) ] = pXSkinAnim;
		return pXSkinAnim;
	}
	return NULL;
}

CKTDXDeviceXSkinMesh* KX2DungeonTool::GetNPCSkinMesh( CX2UnitManager::NPC_UNIT_ID unitID )
{
	WCHAR buf[128] = L"";
	map< CX2UnitManager::NPC_UNIT_ID, CKTDXDeviceXSkinMesh* >::iterator itSkinMesh = m_mapXSkinMesh.find( unitID );
	if( m_mapXSkinMesh.end() == itSkinMesh )
	{
		CX2UnitManager::NPCUnitTemplet*	pNPCTemplet = m_unitManager.GetNPCUnitTemplet( unitID );
		if( pNPCTemplet == NULL )
		{
			if( unitID == 0 )
				wsprintf( buf, L"enum.lua에 로드하려는 NPC의 NPC_UNIT_ID가 등록되지 않았습니다!");
			else
				wsprintf( buf, L"NPCTemplet.lua에 NPC_UNIT_ID[%s]의 정보가 등록되지 않았습니다!", m_enumManager.GetNpcEnumString( unitID ).c_str() );
            MessageBox( g_pKTDXApp->GetHWND(), buf, L"error", MB_OK );
			return NULL;
		}

		KLuaManager luaManager; 
		g_pKTDXApp->GetDeviceManager()->LoadLuaTinker( pNPCTemplet->m_LuaFileName.c_str() );
		g_pKTDXApp->GetDeviceManager()->LoadLuaManager( &luaManager, L"Enum.lua" );
		g_pKTDXApp->GetDeviceManager()->LoadLuaManager( &luaManager, L"Unit_Common_Device.lua" );
		g_pKTDXApp->GetDeviceManager()->LoadLuaManager( &luaManager, pNPCTemplet->m_LuaFileName.c_str() );

		if( luaManager.BeginTable( "INIT_MOTION" ) == true )
		{
			wstring motion;
			LUA_GET_VALUE( luaManager, L"MOTION_FILE_NAME", motion, L"" );
			LoadNPCSkinMesh( unitID, motion.c_str() );

			//wstring changeTex;
			//LUA_GET_VALUE( luaManager, L"MOTION_CHANGE_TEX_XET", changeTex, L"" );

			//wstring aniTex;
			//LUA_GET_VALUE( luaManager, L"MOTION_ANI_TEX_XET", aniTex, L"" );

			//wstring multiTex;
			//LUA_GET_VALUE( luaManager, L"MOTION_MULTI_TEX_XET", multiTex, L"" );

			luaManager.EndTable();
		}

	}

	itSkinMesh = m_mapXSkinMesh.find( unitID );

	return (CKTDXDeviceXSkinMesh*)itSkinMesh->second;
}

void KX2DungeonTool::LoadNPCSkinMesh( CX2UnitManager::NPC_UNIT_ID unitID, const WCHAR* wszSkinMeshFileName )
{
	CKTDXDeviceXSkinMesh* pSkinMesh = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( wszSkinMeshFileName );
	m_mapXSkinMesh[ unitID ] = pSkinMesh;
}

void KX2DungeonTool::DeleteNPCSkinAnims()
{
	if( m_pEditingStage != NULL )
	{
		for( UINT i=0; i<m_pEditingStage->m_SubStageDataList.size(); i++ )
		{
			CX2DungeonSubStage::SubStageData* pSubStageData = m_pEditingStage->m_SubStageDataList[i];
			for( UINT j=0; j< pSubStageData->m_NPCDataList.size(); j++ )
			{
				CX2DungeonSubStage::NPCData* pNPCData = pSubStageData->m_NPCDataList[j];
				DeleteNPCXSkinAnim( pNPCData );
			}
		}
	}
}

void KX2DungeonTool::DeleteNPCXSkinAnim( CX2DungeonSubStage::NPCData* pNPCData )
{
	map< std::pair<CX2DungeonSubStage::NPCData*, int >, CKTDGXSkinAnim* >::iterator it;

	for( UINT i=0; i<pNPCData->m_vecStartPos.size(); i++ )
	{
		//it = m_mapNPCXSkinAnim.find( pNPCData );
		it = m_mapNPCXSkinAnim.find( std::make_pair( pNPCData, i ) );

		if( it != m_mapNPCXSkinAnim.end() )
		{
			CKTDGXSkinAnim* pSkinAnim = (CKTDGXSkinAnim*) it->second;
			SAFE_DELETE( pSkinAnim );
			m_mapNPCXSkinAnim.erase( it );
		}
	}
}

void KX2DungeonTool::RenderNPCStartPosIndex( CX2DungeonSubStage::NPCData* pNPCData, int iStageNPCIndex /*= -1*/, int iNPCSubIndex /*= -1*/, bool& bIsSelected )
{
	CKTDGMatrix matTemp( g_pKTDXApp->GetDevice() );

	//if( m_f3DTextAngle > 25.f )
	//	m_f3DTextAngleStep = -.25f;
	//else if( m_f3DTextAngle < -25.f )
	//	m_f3DTextAngleStep = .25f;
	//m_f3DTextAngle += m_f3DTextAngleStep;

	if( NULL != pNPCData )
	{
		int inpcstartindex = pNPCData->m_vecStartPos[iNPCSubIndex];
		D3DXVECTOR3 vPos = GetTeamStartPos( inpcstartindex )->vPos;
		CKTDGStateManager::PushTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
		CKTDGStateManager::PushTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE );
		CKTDGStateManager::PushTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_CURRENT );

		matTemp.Move( vPos );
		matTemp.MoveRel( 0, 200, 0 );
		//matTemp.Scale( 100, 100, 100 );
		matTemp.Scale( 50, 50, 50 );

		int iLineIndex = m_pWorld->GetLineMap()->GetStartLineIndex( pNPCData->m_vecStartPos[iNPCSubIndex] );
		CKTDGLineMap::LineData* pLineData = m_pWorld->GetLineMap()->GetLineData( iLineIndex );
		D3DXVECTOR3 vDir = pLineData->dirVector;
		vDir.y = 0.f;
		D3DXVec3Normalize( &vDir, &vDir );
		D3DXVECTOR3 vXAxis(1,0,0);

		float fDotXAxis = D3DXVec3Dot( &vDir, &vXAxis );
		float fRad = acosf( fDotXAxis );
		if( vDir.z < 0.f )
		{
			fRad += D3DX_PI;
		}
		matTemp.Rotate( 0.f, fRad, 0.f );


		D3DMATERIAL9 mtrl;
		ZeroMemory( &mtrl, sizeof(D3DMATERIAL9) );


		D3DXCOLOR coTeamColor = D3DXCOLOR( 0.5, 0.5, 0, 1 );
		memcpy( &mtrl.Diffuse, &coTeamColor, sizeof(D3DCOLORVALUE) );
		memcpy( &mtrl.Ambient, &coTeamColor, sizeof(D3DCOLORVALUE) );

		g_pKTDXApp->GetDevice()->SetMaterial( &mtrl );			

		int iNumber = inpcstartindex;
		int iDigit;
		while( iNumber >= 0 )
		{
			iDigit = iNumber%10;

			matTemp.UpdateWorldMatrix();			
			m_pMesh3DText[iDigit]->DrawSubset(0);

			//matTemp.MoveRel( -35, 0, 0 );
			matTemp.MoveRel( -35.f * vDir );
			iNumber = iNumber/10;
			if( iNumber == 0 )
				break;
		}

		// stage에서 몇번째 npc 인지 
		iNumber = iStageNPCIndex;
		matTemp.Move( vPos );
		matTemp.MoveRel( 0, 200+40, 0 );
		matTemp.MoveRel( -100.f * vDir );
		while( iNumber >= 0 )
		{
			iDigit = iNumber%10;

			matTemp.UpdateWorldMatrix();			
			m_pMesh3DText[iDigit]->DrawSubset(0);

			//matTemp.MoveRel( -35, 0, 0 );
			matTemp.MoveRel( -35.f * vDir );
			iNumber = iNumber/10;
			if( iNumber == 0 )
				break;
		}

		// '-'
		matTemp.Move( vPos );
		matTemp.MoveRel( 0, 200+40, 0 );
		matTemp.MoveRel( -70.f * vDir );
		matTemp.UpdateWorldMatrix();
		m_pMesh3DText[10]->DrawSubset(0);

		// 같은 npc의 몇번째 random start 위치 인지
		iNumber = iNPCSubIndex;
		matTemp.Move( vPos );
		matTemp.MoveRel( 0, 200+40, 0 );
		while( iNumber >= 0 )
		{
			iDigit = iNumber%10;

			matTemp.UpdateWorldMatrix();
			m_pMesh3DText[iDigit]->DrawSubset(0);

			//matTemp.MoveRel( -35, 0, 0 );
			matTemp.MoveRel( -35.f * vDir );
			iNumber = iNumber/10;
			if( iNumber == 0 )
				break;
		}

		bIsSelected = false;
		for( UINT i = 0; i<m_vecPickedNPCStartPosKey.size(); i++ )
		{
			SNPCStartPos& npcStart = m_vecPickedNPCStartPosKey[i];
			if( npcStart.m_pNPCData == pNPCData )
				//if( npcStart.m_iStartPosIndex == inpcstartindex )
			{
				// '*'
				matTemp.Move( vPos );
				matTemp.MoveRel( 0, 200+60, 0 );
				matTemp.Scale( 300, 300, 300 );

				matTemp.RotateDegree( m_f3DTextAngle * 5.f, m_f3DTextAngle * 25.f, m_f3DTextAngle * 15.f );

				//matTemp.MoveRel( -60.f * vDir );
				matTemp.UpdateWorldMatrix();
				m_pMesh3DText[11]->DrawSubset(0);

				bIsSelected = true;
				break;
			}
		}

		CKTDGStateManager::PopTextureStageState( 0, D3DTSS_COLORARG2 );
		CKTDGStateManager::PopTextureStageState( 0, D3DTSS_COLORARG1 );
		CKTDGStateManager::PopTextureStageState( 0, D3DTSS_COLOROP );
	}
}

void KX2DungeonTool::RenderNPCUnitStartPostion()
{
	CKTDGMatrix matTemp( g_pKTDXApp->GetDevice() );
	CKTDGXRenderer::RenderParam* pRenderParam = g_pKTDXApp->GetDGManager()->GetXRenderer()->GetRenderParam();

	vector< CX2DungeonSubStage::SubStageData* >* pvecSubStageData = NULL;
	
	if( NULL != m_pEditingStage )
		pvecSubStageData = &m_pEditingStage->m_SubStageDataList;

	if( pvecSubStageData == NULL )
		return;

	for( UINT i=0; i<pvecSubStageData->size(); i++ )
	{
		// 해당 SubStage의 NPC만 보여주기
		if( m_bOnlySubStageNPC )
		{
			if( m_toolUI.GetListBox( IDC_SUB_STAGE_LIST )->GetSelectedIndex() != i )
				continue;
		}

		vector<CX2DungeonSubStage::NPCData*>& npcDataList = pvecSubStageData->at(i)->m_NPCDataList;

		for( UINT j=0; j<npcDataList.size(); j++ )
		{
			CX2DungeonSubStage::NPCData* pNPCData = npcDataList.at(j);

			for( UINT k=0; k<pNPCData->m_vecStartPos.size(); k++ )
			{				
				CKTDGXSkinAnim* pNPCSkinAnim = m_mapNPCXSkinAnim[ std::make_pair( pNPCData, k) ];
				if( NULL == pNPCSkinAnim )
					continue;

				STeamStartPos* pTeamStartPos = GetTeamStartPos( pNPCData->m_vecStartPos[k] );				
				if( pTeamStartPos == NULL )
				{
					//WCHAR wszTempMsg[256];
					//StringCchPrintf( wszTempMsg, sizeof(wszTempMsg), L"라인맵에 없는 NPC start pos(%d) 입니다", pNPCData->m_StartPos );
					//MessageBox( g_pKTDXApp->GetHWND(), L"ERROR", wszTempMsg", MB_OK );
					continue;
				}

				D3DXVECTOR3 vPos = pTeamStartPos->vPos;

				bool bIsRight = false;
				if( pTeamStartPos->iInitialDir == DIR_RIGHT )
				{
					bIsRight = true;
				}

				pNPCSkinAnim->GetMatrix()->Move( vPos );
				pNPCSkinAnim->GetMatrix()->MoveRel( pNPCData->m_AddPos );

				if( true == bIsRight )
					pNPCSkinAnim->GetMatrix()->RotateDegree( 0, 0, 0 );
				else
					pNPCSkinAnim->GetMatrix()->RotateDegree( 0, 180, 0 );

				if( pNPCData->m_bIsRight == true )
					pNPCSkinAnim->GetMatrix()->RotateDegree( 0, 0, 0 );
				else
					pNPCSkinAnim->GetMatrix()->RotateDegree( 0, 180, 0 );


				if( m_bNpcColor == false )
				{
					if( k == 0 )
					{
						// 녹색
						pNPCSkinAnim->GetRenderParam()->color = D3DXCOLOR( 0, 1, 0, 1 );
					}
					else
					{
						// 노란색
						pNPCSkinAnim->GetRenderParam()->color = D3DXCOLOR( 1, 1, (float)k*0.02f, 1 );
					}
				}
				else
				{
					// 원래색
					pNPCSkinAnim->GetRenderParam()->color = D3DXCOLOR( 1, 1, 1, 1 );
				}
				
				// NPC 스케일 적용
				float scale = m_npcScaleManager.GetNpcScale( pNPCData->m_UnitID );
				if( scale != 1.0f )
					pNPCSkinAnim->GetMatrix()->Scale( scale, scale, scale );

				pNPCSkinAnim->OnFrameRender();

				bool bIsSelected = false;
				RenderNPCStartPosIndex( pNPCData, j, k, bIsSelected );

				// 시야 거리 출력
				CX2UnitManager::NPCUnitTemplet* pNpcUnitTemp = m_unitManager.GetNPCUnitTemplet( static_cast<CX2UnitManager::NPC_UNIT_ID>(pNPCData->m_UnitID) );

				if( m_bRenderingNearRange )
				{
					if( m_bOnlySelect )
						if( !bIsSelected ) continue;

					int iTargetRange = 0;
					m_gunpcManager.GetTargetNearRange( pNpcUnitTemp->m_LuaFileName, iTargetRange );

					CKTDGStateManager::SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );

					m_pMatrixSphere->Move( vPos );
					m_pMatrixSphere->MoveRel( pNPCData->m_AddPos );
					float fScale = static_cast<float>(iTargetRange / 100);
					m_pMatrixSphere->Scale( fScale, fScale, fScale );
					m_pMatrixSphere->UpdateWorldMatrix();
					m_pXMeshSphere->Render();

					CKTDGStateManager::SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
				}

				if( m_bRenderingTargetRange )
				{
					if( m_bOnlySelect )
						if( !bIsSelected ) continue;

					int iTargetRange = 0;
					m_gunpcManager.GetTargetRange( pNpcUnitTemp->m_LuaFileName, iTargetRange );

					CKTDGStateManager::SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );

					m_pMatrixSphere->Move( vPos );
					m_pMatrixSphere->MoveRel( pNPCData->m_AddPos );
					float fScale = static_cast<float>(iTargetRange / 100);
					m_pMatrixSphere->Scale( fScale, fScale, fScale );
					m_pMatrixSphere->UpdateWorldMatrix();
					m_pXMeshSphere->Render();

					CKTDGStateManager::SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
				}

				if( m_bRenderingLostRange )
				{
					if( m_bOnlySelect )
						if( !bIsSelected ) continue;

					int iTargetRange = 0;
					m_gunpcManager.GetTargetLostRange( pNpcUnitTemp->m_LuaFileName, iTargetRange );

					CKTDGStateManager::SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );

					m_pMatrixSphere->Move( vPos );
					m_pMatrixSphere->MoveRel( pNPCData->m_AddPos );
					float fScale = static_cast<float>(iTargetRange / 100);
					m_pMatrixSphere->Scale( fScale, fScale, fScale );
					m_pMatrixSphere->UpdateWorldMatrix();
					m_pXMeshSphere->Render();

					CKTDGStateManager::SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
				}
			}
		}
	}
}

KX2DungeonTool::STeamStartPos* KX2DungeonTool::GetTeamStartPos( int iNPCStartPosKey )
{
	for( UINT i=0; i<vecTeamStartPos.size(); i++ )
	{
		if( vecTeamStartPos[i].key == iNPCStartPosKey )
		{
			return &vecTeamStartPos[i];
		}
	}
	return NULL;
}

void KX2DungeonTool::RemoveAllItemsToolUI()
{
	// StageInfo
	m_toolUI.GetListBox( IDC_SUB_STAGE_LIST )->RemoveAllItems( false );
	m_toolUI.GetListBox( IDC_READY_NPC_LIST )->RemoveAllItems( false );
	m_toolUI.GetCheckBox( IDC_START_MOTION_CHECK )->SetChecked( false );
	m_toolUI.GetEditBox( IDC_WORLD_COLOR_R )->SetText( L"" );
	m_toolUI.GetEditBox( IDC_WORLD_COLOR_G )->SetText( L"" );
	m_toolUI.GetEditBox( IDC_WORLD_COLOR_B )->SetText( L"" );
	m_toolUI.GetEditBox( IDC_UNIT_COLOR_R )->SetText( L"" );
	m_toolUI.GetEditBox( IDC_UNIT_COLOR_G )->SetText( L"" );
	m_toolUI.GetEditBox( IDC_UNIT_COLOR_B )->SetText( L"" );

	m_toolUI.GetListBox( IDC_CURTAIN_LIST )->RemoveAllItems();
	RemoveCurtainToolUI();

	m_toolUI.GetListBox( IDC_NPC_LIST )->RemoveAllItems();
	m_toolUI.GetComboBox( IDC_NPC_LIST_COMBO )->RemoveAllItems();
	RemoveNpcToolUI();

	m_toolUI.GetListBox( IDC_TRIGGER_LIST )->RemoveAllItems();
	RemoveTriggerToolUI();

	RemoveSubStageInfoToolUI();

	m_LeftToolUI.GetListBox( IDC_CLEAR_CONDITION_LIST )->RemoveAllItems();
	RemoveClearConditionToolUI();
}

void KX2DungeonTool::RemoveCurtainToolUI()
{
	// Curtain	
	m_toolUI.GetEditBox( IDC_LINE_INDEX_EDIT )->SetText( L"" );
	m_toolUI.GetCheckBox( IDC_AT_START_CHECK )->SetChecked( false );
	m_toolUI.GetCheckBox( IDC_LOOK_LEFT_CHECK )->SetChecked( false );
}

void KX2DungeonTool::RemoveNpcToolUI()
{
	// NPC		
	m_toolUI.GetEditBox( IDC_NPC_ID_EDIT )->SetText( L"" );
	m_toolUI.GetEditBox( IDC_START_POS1_EDIT )->SetText( L"" );
	m_toolUI.GetEditBox( IDC_START_POS2_EDIT )->SetText( L"" );
	m_toolUI.GetEditBox( IDC_ADD_POS_Y_EDIT )->SetText( L"" );
	m_toolUI.GetEditBox( IDC_LEVEL_EDIT )->SetText( L"" );
	m_toolUI.GetCheckBox( IDC_ACTIVE_CHECK )->SetChecked( false );
	m_toolUI.GetCheckBox( IDC_GAGE_BAR_CHECK )->SetChecked( false );
	m_toolUI.GetEditBox( IDC_RATE_EDIT )->SetText( L"" );
	m_toolUI.GetEditBox( IDC_NPC_KEY_CODE_EDIT )->SetText( L"" );
	m_toolUI.GetCheckBox( IDC_FOCUS_CAMERA_CHECK )->SetChecked( false );
	m_toolUI.GetCheckBox( IDC_HAVE_BOSS_GAGE_CHECK )->SetChecked( false );
	m_toolUI.GetCheckBox( IDC_SUB_NPC_CHECK )->SetChecked( false );
	m_toolUI.GetCheckBox( IDC_SIEGE_MODE_CHECK )->SetChecked( false );
}

void KX2DungeonTool::RemoveTriggerToolUI()
{
	// Trigger		
	m_toolUI.GetCheckBox( IDC_HOST_CHECK )->SetChecked( false );
	m_toolUI.GetCheckBox( IDC_ONE_TIME_CHECK )->SetChecked( false );
	m_toolUI.GetEditBox( IDC_INTERVAL_EDIT )->SetText( L"" );
	m_toolUI.GetEditBox( IDC_FUNCTION_VIEWER_EDIT )->SetText( L"" );
}

void KX2DungeonTool::RemoveSubStageInfoToolUI()
{
	// SubStageInfo	
	m_toolUI.GetEditBox( IDC_START_LINE_SET_EDIT )->SetText( L"" );
	m_toolUI.GetEditBox( IDC_MAIN_LINE_SET_EDIT )->SetText( L"" );
	m_toolUI.GetEditBox( IDC_END_LINE_SET_EDIT )->SetText( L"" );
	m_toolUI.GetComboBox( IDC_GO_TYPE_COMBO )->SetSelectedByIndex( 0 );
}

void KX2DungeonTool::RemoveClearConditionToolUI()
{
	// ClearCondition
	m_LeftToolUI.GetComboBox( IDC_CLEAR_CONDITION_COMBO )->SetSelectedByIndex( 0 );
	m_LeftToolUI.GetListBox( IDC_CONDITION_ELEMENT_LIST )->RemoveAllItems( false );
	m_LeftToolUI.GetComboBox( IDC_CONDITION_ELEMENT_COMBO )->RemoveAllItems();
	m_LeftToolUI.GetListBox( IDC_NEXT_STAGE_LIST )->RemoveAllItems( false );
	m_LeftToolUI.GetComboBox( IDC_CLEAR_TYPE_COMBO )->SetSelectedByIndex( 0 );
	m_LeftToolUI.GetComboBox( IDC_CLEAR_CONDITION_STAGE_COMBO )->SetSelectedByIndex( 0 );
	m_LeftToolUI.GetComboBox( IDC_CLEAR_CONDITION_SUBSTAGE_COMBO )->SetSelectedByIndex( 0 );
	m_LeftToolUI.GetEditBox( IDC_CLEAR_CONDITION_RATE_EDIT )->SetText( L"" );
}

void KX2DungeonTool::RemoveClearConNextStageToolUI()
{
	// ClearCondition Next Stage	
	m_LeftToolUI.GetComboBox( IDC_CLEAR_TYPE_COMBO )->SetSelectedByIndex( 0 );
	m_LeftToolUI.GetComboBox( IDC_CLEAR_CONDITION_STAGE_COMBO )->SetSelectedByIndex( 0 );
	m_LeftToolUI.GetComboBox( IDC_CLEAR_CONDITION_SUBSTAGE_COMBO )->SetSelectedByIndex( 0 );
	m_LeftToolUI.GetEditBox( IDC_CLEAR_CONDITION_RATE_EDIT )->SetText( L"" );
}

bool KX2DungeonTool::SaveDungeonLuaFile( const WCHAR* wszFileName )
{
	// 저장할 데이터 검사
	if( m_pEditingDungeon == NULL )
		return false;

	// 파일 확장자가 없으면 붙여주자.
	std::string strFileName;
	strFileName.reserve(512);
	ConvertWCHARToChar( strFileName, wszFileName );
	MakeUpperCase( strFileName );

	int iStrLen = strFileName.length();
	if( 'L' != strFileName[iStrLen-3] || 'U' != strFileName[iStrLen-2] || 'A' != strFileName[iStrLen-1] ) 
	{
		ConvertWCHARToChar( strFileName, wszFileName );
		strFileName.append(".lua");
	}
	else
	{
		ConvertWCHARToChar( strFileName, wszFileName );	
	}

	std::wstring wstrFilePath;
	ConvertCharToWCHAR( wstrFilePath, strFileName.c_str() );

	std::ofstream ofs( strFileName.c_str() );
	if( ofs.is_open() == false )
	{
		ofs.close();
		return false;
	}

	std::wstring wsTempStr;
	std::string sWriteString;

	// STAGE_LIST [BEGIN]	
	ofs << std::endl << std::endl;
	ofs << "STAGE_LIST = " << std::endl;
	ofs << "{" << std::endl;

	// DUNGEON_COLOR [BEGIN]	
	if( m_pEditingDungeon->GetWorldColor().r != -1.f &&
		m_pEditingDungeon->GetWorldColor().g != -1.f &&
		m_pEditingDungeon->GetWorldColor().b != -1.f )
	{
		ofs << "	DUNGEON_WORLD_COLOR = { ";
		ofs << static_cast<int>( m_pEditingDungeon->GetWorldColor().r ) << ", ";
		ofs << static_cast<int>( m_pEditingDungeon->GetWorldColor().g ) << ", ";
		ofs << static_cast<int>( m_pEditingDungeon->GetWorldColor().b ) << ", },"<< std::endl;
	}
	
	if( m_pEditingDungeon->GetUnitColor().r != -1.f &&
		m_pEditingDungeon->GetUnitColor().g != -1.f &&
		m_pEditingDungeon->GetUnitColor().b != -1.f )
	{
		ofs << "	DUNGEON_UNIT_COLOR = { ";
		ofs << static_cast<int>( m_pEditingDungeon->GetUnitColor().r ) << ", ";
		ofs << static_cast<int>( m_pEditingDungeon->GetUnitColor().g ) << ", ";
		ofs << static_cast<int>( m_pEditingDungeon->GetUnitColor().b ) << ", },"<< std::endl;
	}

	if( m_mapDungeonMapList.size() != 0 )
	{
		// DUNGEON_MAP [BEGIN]
		ofs << std::endl;
		ofs << "	DUNGEON_MAP =" << std::endl;
		ofs << "	{"			   << std::endl;
		//ConvertWCHARToChar( sWriteString, m_mapUI.GetEditBox( IDC_TEXTURE_FILE_NAME_EDIT )->GetText() );
		//ofs << "		TEX_NAME = \"" << sWriteString << "\"," << std::endl << std::endl;
		
		DungeonMapIterator mit0 = m_mapDungeonMapList.begin();
		for( int i=1 ; mit0 != m_mapDungeonMapList.end(); mit0++, i++ )
		{
			if( (mit0->first).second == 0 && (mit0->first).first == 0 )
			{
				ofs << "		STAGE" << (mit0->first).first << " =" << std::endl;
				ofs << "		{" << std::endl;
			}
			else if( (mit0->first).second == 0 )
			{
				ofs << "		}," << std::endl;
				ofs << "		STAGE" << (mit0->first).first << " =" << std::endl;
				ofs << "		{" << std::endl;
			}

			ofs << "			SUB_STAGE" << (mit0->first).second << " = ";
			ofs << "{ ";

			ofs << (int)(mit0->second).m_vPos.x << ", " << (int)(mit0->second).m_vPos.y;
			if( (mit0->second).m_bIsBossStage == true  &&  (mit0->second).m_bIsNextStageRandom == true )
			{
				ofs << " , TRUE";
				ofs << " , TRUE";
			}
			else if( (mit0->second).m_bIsBossStage == true  &&  (mit0->second).m_bIsNextStageRandom == false )
			{
				ofs << " , TRUE";
			}
			else if( (mit0->second).m_bIsBossStage == false  &&  (mit0->second).m_bIsNextStageRandom == true )
			{
				ofs << " , FALSE";
				ofs << " , TRUE";
			}

			ofs << " }," << std::endl;

			if( i == m_mapDungeonMapList.size() )
			{
				ofs << "		}," << std::endl;
			}
		}

		// DUNGEON_MAP [END]
		ofs << "	},"			   << std::endl << std::endl << std::endl;
	}	

	std::vector<CX2DungeonStage::StageData*> vecStageList = m_pEditingDungeon->GetStageDataList();
	std::vector<CX2DungeonStage::StageData*>::iterator mit = vecStageList.begin();
	CX2DungeonStage::StageData* pStageData;
	if( mit == vecStageList.end() )
	{
		ofs.close();
		return false;
	}	
	
	for( int i=0; mit != vecStageList.end(); mit++, i++ )
	{
		pStageData = *mit;

		// STAGE [BEGIN]
		ofs << "	STAGE" << i << " =" << std::endl;
		ofs << "	{"					<< std::endl;

		wsTempStr = m_enumManager.GetWorldEnumString( pStageData->m_WorldID );
		ConvertWCHARToChar( sWriteString, wsTempStr.c_str() );
		ofs << "		WORLD_ID = WORLD_ID[\""	 << sWriteString << "\"]," << std::endl;

		// DUNGEON_COLOR [BEGIN]	
		if( pStageData->m_WorldColor.r != -1.f &&
			pStageData->m_WorldColor.g != -1.f &&
			pStageData->m_WorldColor.b != -1.f )
		{
			ofs << "		WORLD_COLOR = { ";
			ofs << static_cast<int>( pStageData->m_WorldColor.r ) << ", ";
			ofs << static_cast<int>( pStageData->m_WorldColor.g ) << ", ";
			ofs << static_cast<int>( pStageData->m_WorldColor.b ) << ", },"<< std::endl;
		}

		if( pStageData->m_UnitColor.r != -1.f &&
			pStageData->m_UnitColor.g != -1.f &&
			pStageData->m_UnitColor.b != -1.f )
		{
			ofs << "		UNIT_COLOR = { ";
			ofs << static_cast<int>( pStageData->m_UnitColor.r ) << ", ";
			ofs << static_cast<int>( pStageData->m_UnitColor.g ) << ", ";
			ofs << static_cast<int>( pStageData->m_UnitColor.b ) << ", },"<< std::endl;
		}

		if( pStageData->m_bStartMotion )		
			ofs << "		START_MOTION = TRUE"						 << "," << std::endl;

		// READY_NPC [BEGIN]
		ofs << std::endl;
		ofs << "		READY_NPC = "	<< std::endl;
		ofs << "		{"				<< std::endl;
		
		std::vector<CX2UnitManager::NPC_UNIT_ID> vecNpcList = pStageData->m_ReadyNPCList;
		std::vector<CX2UnitManager::NPC_UNIT_ID>::iterator mit2 = vecNpcList.begin();
		for( ; mit2 != vecNpcList.end(); mit2++ )
		{
			wsTempStr = m_enumManager.GetNpcEnumString( *mit2 );
			ConvertWCHARToChar( sWriteString, wsTempStr.c_str() );
			ofs << "			NPC_UNIT_ID[\"" << sWriteString << "\"]," << std::endl;
		}

		// READY_NPC [END]
		ofs << "		},"				<< std::endl << std::endl;


		std::vector<CX2DungeonSubStage::SubStageData*> vecSubStageList = pStageData->m_SubStageDataList;
		std::vector<CX2DungeonSubStage::SubStageData*>::iterator mit3 = vecSubStageList.begin();
		CX2DungeonSubStage::SubStageData* pSubStageData;
		for( int i=0; mit3 != vecSubStageList.end(); mit3++, i++ )
		{
			pSubStageData = *mit3;

			// SUB_STAGE [BEGIN]
			ofs << "		SUB_STAGE" << i << " =" << std::endl;
			ofs << "		{" << std::endl;

			ofs << "			START_LINE_SET      = " << pSubStageData->m_StartLineSet << "," << std::endl;
			ofs << "			MAIN_LINE_SET       = " << pSubStageData->m_MainLineSet  << "," << std::endl;
			ofs << "			END_LINE_SET        = " << pSubStageData->m_EndLineSet	 << "," << std::endl;
			ofs << std::endl;
			sWriteString = (char*)m_toolUI.GetComboBox( IDC_GO_TYPE_COMBO )->GetItemData( pSubStageData->m_GoType );
			ofs << "			GO_TYPE             = GO_TYPE[\"" << sWriteString		 << "\"]," << std::endl;
			ofs << std::endl;

			//////////////////////////////////////////////////////////////////////////

			// CLEAR CONDITION [BEGIN]
			std::vector<CX2DungeonSubStage::ClearCondtionData> vecClearConList = pSubStageData->m_vecClearCondData;
			std::vector<CX2DungeonSubStage::ClearCondtionData>::iterator vitClearCon = vecClearConList.begin();
			
			for( int i=0; vitClearCon != vecClearConList.end(); vitClearCon++, i++ )
			{
				CX2DungeonSubStage::ClearCondtionData& rClearConData = *vitClearCon;

				ofs << "			CLEAR_COND" << i << " =" << std::endl;
				ofs << "			{" << std::endl;
				sWriteString = GET_CLEAR_CONDITION::GetClearConditionString( rClearConData.m_eClearCondition );
				ofs << "				CLEAR_CONDITION[\"" << sWriteString << "\"]," << std::endl;
				switch( rClearConData.m_eClearCondition ) 
				{
				case CX2DungeonSubStage::CC_KILL_KEYCODE_NPC:
				case CX2DungeonSubStage::CC_KILL_KEYCODE_NPC_ANY:
					{
						ofs << "				NPC_KEYCODE = { ";
						std::vector<MonsterCount>::iterator vitKeyCode = rClearConData.m_vecNPCKeyCode.begin();
						bool bOnlyOne = true;
						for( ; vitKeyCode != rClearConData.m_vecNPCKeyCode.end(); vitKeyCode++ )
						{
							if( !bOnlyOne )
								ofs << ", ";

							ofs << vitKeyCode->m_iKey;							

							bOnlyOne = ( rClearConData.m_vecNPCKeyCode.size() == 1 );
						}
						ofs << " }," << std::endl;
					}
					break;

				case CX2DungeonSubStage::CC_KILL_NPC_TYPE:
					{
						ofs << "				NPC_ID	 =" << std::endl;
						ofs << "				{ "			<< std::endl;
						std::vector<MonsterCount>::iterator vitNpcID = rClearConData.m_vecNPCID.begin();
						for( ; vitNpcID != rClearConData.m_vecNPCID.end(); vitNpcID++ )
						{
							std::wstring wsTempStr = m_enumManager.GetNpcEnumString( static_cast<CX2UnitManager::NPC_UNIT_ID>(vitNpcID->m_iKey) );
							ConvertWCHARToChar( sWriteString, wsTempStr.c_str() );
							ofs << "					NPC_UNIT_ID[\"" << sWriteString << "\"]," << std::endl;
						}
						ofs << "				}," << std::endl;
					}
					break;

				default:
					break;
				}

				ofs << std::endl;
				std::vector< CX2DungeonSubStage::NextStageData >::iterator vitNextStage = rClearConData.m_vecNextStage.begin();
				for( int i=0; vitNextStage != rClearConData.m_vecNextStage.end(); vitNextStage++, i++ )
				{
					CX2DungeonSubStage::NextStageData& rNextStageData = *vitNextStage;

					ofs << "				NEXT_STAGE" << i << " = { ";
					sWriteString = GET_CLEAR_TYPE::GetClearTypeString( rNextStageData.m_eClearType );
					ofs << "CLEAR_TYPE[\"" << sWriteString << "\"], ";
					ofs << rNextStageData.m_iStageIndex		<< ", ";
					ofs << rNextStageData.m_iSubStageIndex	<< ", ";
					ofs << rNextStageData.m_iRate			<< " }," << std::endl;
				}

				// CLEAR CONDITION [END]
				ofs << "			}," << std::endl << std::endl;
			}

			//////////////////////////////////////////////////////////////////////////			

			std::vector<CX2DungeonSubStage::CurtainData*> vecCurtainList = pSubStageData->m_CurtainDataList;
			std::vector<CX2DungeonSubStage::CurtainData*>::iterator mit4 = vecCurtainList.begin();
			CX2DungeonSubStage::CurtainData* pCurtain;
			if( mit4 != vecCurtainList.end() )
			{
				// CURTAIN [BEGIN]
				ofs << "			CURTAIN_GROUP =" << std::endl;
				ofs << "			{"				 << std::endl;

				for( ; mit4 != vecCurtainList.end(); mit4++ )
				{
					pCurtain = *mit4;

					ofs << "				{"			 << std::endl;
					ofs << "					LINE_INDEX	= " << pCurtain->m_LineIndex << "," << std::endl;
					BOOL_TO_STRING( pCurtain->m_bAtStart, sWriteString );
					ofs << "					AT_START	= " << sWriteString			 << "," << std::endl;
					BOOL_TO_STRING( pCurtain->m_bLookLeft, sWriteString );
					ofs << "					LOOK_LEFT	= " << sWriteString			 << "," << std::endl;
					ofs << "				},"			 << std::endl;
				}			

				// CURTAIN [END]
				ofs << "			},"				 << std::endl << std::endl;
			}

			// NPC_GROUP [BEGIN]
			ofs << "			NPC_GROUP ="	 << std::endl;
			ofs << "			{"				 << std::endl;

			int iSubNpcCount = 0;
			bool bBeforeSubNpc = false;
			vector<CX2DungeonSubStage::NPCData*> vecNpcList = pSubStageData->m_NPCDataList;
			vector<CX2DungeonSubStage::NPCData*>::iterator mit5 = vecNpcList.begin();
			CX2DungeonSubStage::NPCData* pNpcData;
			for( ; mit5 != vecNpcList.end(); mit5++ )
			{
				pNpcData = *mit5;
				if( !pNpcData->m_bSubNpc )
				{
					if( bBeforeSubNpc ) 
					{
						// write for SUB_NPC [END]
						ofs << "				},"			 << std::endl << std::endl;
					}

					ofs << "				{"		 << std::endl;

					wsTempStr = m_enumManager.GetNpcEnumString( pNpcData->m_UnitID );
					ConvertWCHARToChar( sWriteString, wsTempStr.c_str() );					
					ofs << "					NPC_ID		= NPC_UNIT_ID[\"" << sWriteString << "\"]," << std::endl;
					std::vector<int> vecStartPos = pNpcData->m_vecStartPos;
					std::vector<int>::iterator mit7 = vecStartPos.begin();				
					ofs << "					START_POS	= { ";
					for( ; mit7 != vecStartPos.end(); mit7++ )
					{
						ofs << *mit7 << ", ";
					}
					ofs << "}," << std::endl;
					ofs << "					LEVEL		= " << pNpcData->m_Level	 << "," << std::endl;
					if( !pNpcData->m_bActive )				
						ofs << "					ACTIVE		= FALSE," << std::endl;
					if( !pNpcData->m_bShowGage )
						ofs << "					GAGE_BAR	= FALSE," << std::endl;
					if( pNpcData->m_bHasBossGage )
						ofs << "					HAVE_BOSS_GAGE = TRUE," << std::endl;
					if( pNpcData->m_AddPos.y != 0.0f )
						ofs << "					ADD_POS_Y	= " << pNpcData->m_AddPos.y << "," << std::endl;
					if( pNpcData->m_Rate != 100 )
						ofs << "					RATE		= " << pNpcData->m_Rate		<< "," << std::endl;
					if( pNpcData->m_KeyCode > 0 )
						ofs << "					KEY_CODE	= " << pNpcData->m_KeyCode	<< "," << std::endl;
					if( pNpcData->m_bSiegeMode )
						ofs << "					SIEGE_MODE	= TRUE," << std::endl;
					if( pNpcData->m_bIsRight )
						ofs << "					IS_RIGHT	= TRUE," << std::endl;
					if( pNpcData->m_bFocusCamera )
						ofs << "					FOCUS_CAMERA	= TRUE," << std::endl;
					ofs << "				},"		 << std::endl;
					bBeforeSubNpc = false;
				}
				else
				{	
					if( !bBeforeSubNpc )
					{
						// write for SUB_NPC [BEGIN]
						ofs << std::endl;
						ofs << "				{"		 << std::endl;
					}

					ofs << "					SUB_NPC" << iSubNpcCount << " =" << std::endl;
					ofs << "					{"	 << std::endl;
					if( pNpcData->m_Rate != 100 )
						ofs << "						SUB_NPC_RATE	= " << pNpcData->m_Rate		<< "," << std::endl;

					wsTempStr = m_enumManager.GetNpcEnumString( pNpcData->m_UnitID );
					ConvertWCHARToChar( sWriteString, wsTempStr.c_str() );
					ofs << "						NPC_ID			= NPC_UNIT_ID[\"" << sWriteString << "\"]," << std::endl;
					std::vector<int> vecStartPos = pNpcData->m_vecStartPos;
					std::vector<int>::iterator mit7 = vecStartPos.begin();				
					ofs << "						START_POS		= { ";
					for( ; mit7 != vecStartPos.end(); mit7++ )
					{
						ofs << *mit7 << ", ";
					}
					ofs << "}," << std::endl;
					ofs << "						LEVEL			= " << pNpcData->m_Level	 << "," << std::endl;
					if( !pNpcData->m_bActive )				
						ofs << "						ACTIVE			= FALSE," << std::endl;
					if( !pNpcData->m_bShowGage )
						ofs << "						GAGE_BAR		= FALSE," << std::endl;
					if( pNpcData->m_bHasBossGage )
						ofs << "						HAVE_BOSS_GAGE	 = TRUE," << std::endl;
					if( pNpcData->m_AddPos.y != 0.0f )
						ofs << "						ADD_POS_Y		= " << pNpcData->m_AddPos.y << "," << std::endl;
					if( pNpcData->m_KeyCode > 0 )
						ofs << "						KEY_CODE		= " << pNpcData->m_KeyCode	<< "," << std::endl;
					if( pNpcData->m_bSiegeMode )
						ofs << "						SIEGE_MODE		= TRUE," << std::endl;
					if( pNpcData->m_bIsRight )
						ofs << "						IS_RIGHT		= TRUE," << std::endl;
					if( pNpcData->m_bFocusCamera )
						ofs << "					FOCUS_CAMERA	= TRUE," << std::endl;
					ofs << "					},"		 << std::endl;
					bBeforeSubNpc = true;
					++iSubNpcCount;
				}			
			}

			// NPC_GROUP [END]
			ofs << "			},"				 << std::endl << std::endl;

			// TRIGGER_LIST [BEGIN]
			ofs << "			TRIGGER_LIST ="  << std::endl;
			ofs << "			{"				 << std::endl;

			vector<CX2DungeonSubStage::Trigger*> vecTirggerList = pSubStageData->m_TriggerList;
			vector<CX2DungeonSubStage::Trigger*>::iterator mit6 = vecTirggerList.begin();
			CX2DungeonSubStage::Trigger* pTrigger;
			for( int i=0 ; mit6 != vecTirggerList.end(); mit6++, i++ )
			{
				pTrigger = *mit6;

				ofs << "				TRIGGER" << i << " =" << std::endl;
				ofs << "				{"					  << std::endl;
				BOOL_TO_STRING( pTrigger->m_bHost, sWriteString );
				ofs << "					HOST			= " << sWriteString			<< "," << std::endl;
				BOOL_TO_STRING( pTrigger->m_bOneTime, sWriteString );
				ofs << "					ONE_TIME		= " << sWriteString			<< "," << std::endl;
				ofs << "					INTERVAL		= " << pTrigger->m_Interval << "," << std::endl;
				ConvertWCHARToChar( sWriteString, pTrigger->m_ConditionFunc.c_str() );
				ofs << "					CONDITION_FUNC	= \"" << sWriteString		<< "\"," << std::endl;
				ConvertWCHARToChar( sWriteString, pTrigger->m_ReactFunc.c_str() );
				ofs << "					REACT_FUNG		= \"" << sWriteString		<< "\"," << std::endl;
				ofs << "				},"					  << std::endl;
			}

			// TRIGGER_LIST [END]
			ofs << "			},"				 << std::endl;

			// SUB_STAGE [END]
			ofs << "		},"				<< std::endl << std::endl;
		}

		// STAGE [END]
		ofs << "	},"				<< std::endl << std::endl;
	}
	
	// STAGE_LIST [END]
	ofs << "}" << std::endl;

    if(m_bTriggerFunc)
    {
        ConvertWCHARToChar( sWriteString, m_strFunc.c_str() );
        ofs << sWriteString << std::endl;
    }

	// TRIGGER
	ofs.close();

	return true;
}

void KX2DungeonTool::SetVisibleStageInfo( bool bVal )
{
	m_toolUI.GetStatic( IDC_STATIC_STAGE_INFO )->SetVisible( bVal );
	m_toolUI.GetStatic( IDC_STATIC_STAGE_INFO_1 )->SetVisible( bVal );
	m_toolUI.GetStatic( IDC_STATIC_STAGE_INFO_2 )->SetVisible( bVal );
	m_toolUI.GetListBox( IDC_SUB_STAGE_LIST )->SetVisible( bVal );
	m_toolUI.GetButton( IDC_ADD_SUB_STAGE_BUTTON )->SetVisible( bVal );
	m_toolUI.GetButton( IDC_DEL_SUB_STAGE_BUTTON )->SetVisible( bVal );
	m_toolUI.GetListBox( IDC_READY_NPC_LIST )->SetVisible( bVal );
	m_toolUI.GetComboBox( IDC_NPC_ID_LIST_COMBO )->SetVisible( bVal );
	m_toolUI.GetButton( IDC_ADD_READY_NPC_BUTTON )->SetVisible( bVal );
	m_toolUI.GetButton( IDC_DEL_READY_NPC_BUTTON )->SetVisible( bVal );
	m_toolUI.GetCheckBox( IDC_START_MOTION_CHECK )->SetVisible( bVal );
	m_toolUI.GetStatic( IDC_STATIC_WORLD_COLOR )->SetVisible( bVal );
	m_toolUI.GetEditBox( IDC_WORLD_COLOR_R )->SetVisible( bVal );
	m_toolUI.GetEditBox( IDC_WORLD_COLOR_G )->SetVisible( bVal );
	m_toolUI.GetEditBox( IDC_WORLD_COLOR_B )->SetVisible( bVal );
	m_toolUI.GetStatic( IDC_STATIC_UNIT_COLOR )->SetVisible( bVal );
	m_toolUI.GetEditBox( IDC_UNIT_COLOR_R )->SetVisible( bVal );
	m_toolUI.GetEditBox( IDC_UNIT_COLOR_G )->SetVisible( bVal );
	m_toolUI.GetEditBox( IDC_UNIT_COLOR_B )->SetVisible( bVal );
}

void KX2DungeonTool::SetVisibleCurtain( bool bVal )
{
	m_toolUI.GetStatic( IDC_STATIC_CURTAIN )->SetVisible( bVal );
	m_toolUI.GetStatic( IDC_STATIC_CURTAIN_1 )->SetVisible( bVal );
	m_toolUI.GetListBox( IDC_CURTAIN_LIST )->SetVisible( bVal );
	m_toolUI.GetButton( IDC_ADD_CURTAIN_BUTTON )->SetVisible( bVal );
	m_toolUI.GetButton( IDC_DEL_CURTAIN_BUTTON )->SetVisible( bVal );
	m_toolUI.GetEditBox( IDC_LINE_INDEX_EDIT )->SetVisible( bVal );
	m_toolUI.GetCheckBox( IDC_AT_START_CHECK )->SetVisible( bVal );
	m_toolUI.GetCheckBox( IDC_LOOK_LEFT_CHECK )->SetVisible( bVal );
}

void KX2DungeonTool::SetVisibleNPC( bool bVal )
{
	m_toolUI.GetStatic( IDC_STATIC_NPC )->SetVisible( bVal );
	m_toolUI.GetStatic( IDC_STATIC_NPC_1 )->SetVisible( bVal );
	m_toolUI.GetStatic( IDC_STATIC_NPC_2 )->SetVisible( bVal );
	m_toolUI.GetStatic( IDC_STATIC_NPC_3 )->SetVisible( bVal );
	m_toolUI.GetStatic( IDC_STATIC_NPC_4 )->SetVisible( bVal );
	m_toolUI.GetStatic( IDC_STATIC_NPC_5 )->SetVisible( bVal );
	m_toolUI.GetListBox( IDC_NPC_LIST )->SetVisible( bVal );
	m_toolUI.GetComboBox( IDC_NPC_LIST_COMBO )->SetVisible( bVal );
	m_toolUI.GetButton( IDC_ADD_NPC_BUTTON )->SetVisible( bVal );
	m_toolUI.GetButton( IDC_DEL_NPC_BUTTON )->SetVisible( bVal );
	m_toolUI.GetEditBox( IDC_NPC_ID_EDIT )->SetVisible( bVal );
	m_toolUI.GetEditBox( IDC_START_POS1_EDIT )->SetVisible( bVal );
	m_toolUI.GetEditBox( IDC_START_POS2_EDIT )->SetVisible( bVal );
	m_toolUI.GetEditBox( IDC_ADD_POS_Y_EDIT )->SetVisible( bVal );
	m_toolUI.GetEditBox( IDC_LEVEL_EDIT )->SetVisible( bVal );
	m_toolUI.GetCheckBox( IDC_ACTIVE_CHECK )->SetVisible( bVal );
	m_toolUI.GetCheckBox( IDC_GAGE_BAR_CHECK )->SetVisible( bVal );
	m_toolUI.GetEditBox( IDC_RATE_EDIT )->SetVisible( bVal );
	m_toolUI.GetStatic( IDC_STATIC_NPC_KEY_CODE )->SetVisible( bVal );
	m_toolUI.GetEditBox( IDC_NPC_KEY_CODE_EDIT )->SetVisible( bVal );
	m_toolUI.GetCheckBox( IDC_FOCUS_CAMERA_CHECK )->SetVisible( bVal );
	m_toolUI.GetCheckBox( IDC_HAVE_BOSS_GAGE_CHECK )->SetVisible( bVal );
	m_toolUI.GetCheckBox( IDC_SUB_NPC_CHECK )->SetVisible( bVal );
	m_toolUI.GetCheckBox( IDC_SIEGE_MODE_CHECK )->SetVisible( bVal );
	m_toolUI.GetCheckBox( IDC_IS_RIGHT_CHECK )->SetVisible( bVal );	
}

void KX2DungeonTool::SetVisibleTrigger( bool bVal )
{
	m_toolUI.GetStatic( IDC_STATIC_TRIGGER )->SetVisible( bVal );
	m_toolUI.GetStatic( IDC_STATIC_TRIGGER_1 )->SetVisible( bVal );
	m_toolUI.GetListBox( IDC_TRIGGER_LIST )->SetVisible( bVal );
	m_toolUI.GetButton( IDC_ADD_TRIGGER_BUTTON )->SetVisible( bVal );
	m_toolUI.GetButton( IDC_DEL_TRIGGER_BUTTON )->SetVisible( bVal );
	m_toolUI.GetCheckBox( IDC_HOST_CHECK )->SetVisible( bVal );
	m_toolUI.GetCheckBox( IDC_ONE_TIME_CHECK )->SetVisible( bVal );
	m_toolUI.GetEditBox( IDC_INTERVAL_EDIT )->SetVisible( bVal );
	m_toolUI.GetEditBox( IDC_FUNCTION_VIEWER_EDIT )->SetVisible( bVal );
	m_toolUI.GetButton( IDC_CONDITION_FUNC_BUTTON )->SetVisible( bVal );
	m_toolUI.GetButton( IDC_REACT_FUNG_BUTTON )->SetVisible( bVal );
}

void KX2DungeonTool::SetVisibleSubStageInfo( bool bVal )
{
	m_toolUI.GetStatic( IDC_STATIC_SUB_STAGE_INFO )->SetVisible( bVal );
	m_toolUI.GetStatic( IDC_STATIC_SUB_STAGE_INFO_1 )->SetVisible( bVal );
	m_toolUI.GetStatic( IDC_STATIC_SUB_STAGE_INFO_2 )->SetVisible( bVal );
	m_toolUI.GetStatic( IDC_STATIC_SUB_STAGE_INFO_3 )->SetVisible( bVal );
	m_toolUI.GetStatic( IDC_STATIC_SUB_STAGE_INFO_4 )->SetVisible( bVal );
	m_toolUI.GetEditBox( IDC_START_LINE_SET_EDIT )->SetVisible( bVal );
	m_toolUI.GetEditBox( IDC_MAIN_LINE_SET_EDIT )->SetVisible( bVal );
	m_toolUI.GetEditBox( IDC_END_LINE_SET_EDIT )->SetVisible( bVal );
	m_toolUI.GetComboBox( IDC_GO_TYPE_COMBO )->SetVisible( bVal );
}

void KX2DungeonTool::SetVisibleClearCondition( bool bVal )
{
	m_LeftToolUI.GetStatic( IDC_STATIC_CLEAR_CONDITION_INFO )->SetVisible( bVal );
	m_LeftToolUI.GetListBox( IDC_CLEAR_CONDITION_LIST )->SetVisible( bVal );
	m_LeftToolUI.GetButton( IDC_ADD_CLEAR_CONDITION_BUTTON )->SetVisible( bVal );
	m_LeftToolUI.GetButton( IDC_DEL_CLEAR_CONDITION_BUTTON )->SetVisible( bVal );
	m_LeftToolUI.GetComboBox( IDC_CLEAR_CONDITION_COMBO )->SetVisible( bVal );
	m_LeftToolUI.GetListBox( IDC_CONDITION_ELEMENT_LIST )->SetVisible( bVal );
	m_LeftToolUI.GetComboBox( IDC_CONDITION_ELEMENT_COMBO )->SetVisible( bVal );
	m_LeftToolUI.GetButton( IDC_ADD_CONDITION_ELEMENT )->SetVisible( bVal );
	m_LeftToolUI.GetButton( IDC_DEL_CONDITION_ELEMENT )->SetVisible( bVal );
	m_LeftToolUI.GetListBox( IDC_NEXT_STAGE_LIST )->SetVisible( bVal );
	m_LeftToolUI.GetButton( IDC_ADD_NEXT_STAGE_BUTTON )->SetVisible( bVal );
	m_LeftToolUI.GetButton( IDC_DEL_NEXT_STAGE_BUTTON )->SetVisible( bVal );
	m_LeftToolUI.GetComboBox( IDC_CLEAR_TYPE_COMBO )->SetVisible( bVal );
	m_LeftToolUI.GetStatic( IDC_STATIC_CLEAR_CONDITION_STAGE )->SetVisible( bVal );
	m_LeftToolUI.GetComboBox( IDC_CLEAR_CONDITION_STAGE_COMBO )->SetVisible( bVal );
	m_LeftToolUI.GetStatic( IDC_STATIC_CLEAR_CONDITION_SUBSTAGE )->SetVisible( bVal );
	m_LeftToolUI.GetComboBox( IDC_CLEAR_CONDITION_SUBSTAGE_COMBO )->SetVisible( bVal );
	m_LeftToolUI.GetStatic( IDC_STATIC_CLEAR_CONDITION_RATE )->SetVisible( bVal );
	m_LeftToolUI.GetEditBox( IDC_CLEAR_CONDITION_RATE_EDIT )->SetVisible( bVal );
}

void KX2DungeonTool::SetVisibleCheckBox( bool bVal )
{
	int checkAllBox = 0;
	if( m_toolUI.GetStatic( IDC_STATIC_STAGE_INFO )->GetVisible() == bVal )
	{
		m_menuUI.GetCheckBox( IDC_STAGE_INFO_VISIBLE_CHECK )->SetChecked( bVal );
		++checkAllBox;
	}
	if( m_toolUI.GetStatic( IDC_STATIC_CURTAIN )->GetVisible() == bVal )
	{
		m_menuUI.GetCheckBox( IDC_CURTAIN_VISIBLE_CHECK )->SetChecked( bVal );
		++checkAllBox;
	}
	if( m_toolUI.GetStatic( IDC_STATIC_NPC )->GetVisible() == bVal )
	{
		m_menuUI.GetCheckBox( IDC_NPC_VISIBLE_CHECK )->SetChecked( bVal );
		++checkAllBox;
	}
	if( m_toolUI.GetStatic( IDC_STATIC_TRIGGER )->GetVisible() == bVal )
	{
		m_menuUI.GetCheckBox( IDC_TRIGGER_VISIBLE_CHECK )->SetChecked( bVal );
		++checkAllBox;
	}
	if( m_toolUI.GetStatic( IDC_STATIC_SUB_STAGE_INFO )->GetVisible() == bVal )
	{
		m_menuUI.GetCheckBox( IDC_SUBSTAGE_VISIBLE_CHECK )->SetChecked( bVal );
		++checkAllBox;
	}
	if( m_LeftToolUI.GetStatic( IDC_STATIC_CLEAR_CONDITION_INFO )->GetVisible() == bVal )
	{
		m_menuUI.GetCheckBox( IDC_CLEAR_CONDITION_VISIBLE_CHECK )->SetChecked( bVal );
		++checkAllBox;
	}

	if( checkAllBox == 6 )
		m_menuUI.GetCheckBox( IDC_TOOL_UI_SHOW_CHECK )->SetChecked( true );
	else
		m_menuUI.GetCheckBox( IDC_TOOL_UI_SHOW_CHECK )->SetChecked( false );
}

template<typename T>
void KX2DungeonTool::EditingEditBox( KX2DungeonTool::DIALOG_CONTROL_ID id, T& data, T& backUpData )
{
	return EditingEditBox( m_toolUI, id, data, backUpData );
}

template<typename T>
void KX2DungeonTool::EditingEditBox( CDXUTDialog& dialog, KX2DungeonTool::DIALOG_CONTROL_ID id, T& data, T& backUpData )
{
	std::wstring wsTemp;
	std::string sTemp;

	CDXUTEditBox* pEditBox = dialog.GetEditBox( id );
	wsTemp = pEditBox->GetText();
	ConvertWCHARToChar( sTemp, wsTemp.c_str() );
	data = static_cast<T>(atoi( sTemp.c_str() ));

	if( data != backUpData )
		dialog.GetEditBox( id )->SetTextColor( (D3DCOLOR)RED_COLOR );
	else
		dialog.GetEditBox( id )->SetTextColor( (D3DCOLOR)BLACK_COLOR );
}

template<typename T>
void KX2DungeonTool::EditingEditBoxNonBackup( CDXUTDialog* dialog, KX2DungeonTool::DIALOG_CONTROL_ID id, T& data )
{
	std::wstring wsTemp;
	std::string sTemp;

	CDXUTEditBox* pEditBox = dialog->GetEditBox( id );	
	wsTemp = pEditBox->GetText();
	ConvertWCHARToChar( sTemp, wsTemp.c_str() );
	data = static_cast<T>(atoi( sTemp.c_str() ));
}

template<typename T>
void KX2DungeonTool::EditingEditBoxInsert( KX2DungeonTool::DIALOG_CONTROL_ID id, std::vector<T>& data )
{
	std::wstring wsTemp;
	std::string sTemp;
	T temp;

	CDXUTEditBox* pEditBox = m_toolUI.GetEditBox( id );
	wsTemp = pEditBox->GetText();
	ConvertWCHARToChar( sTemp, wsTemp.c_str() );
	temp = static_cast<T>(atoi( sTemp.c_str() ));
	data.push_back( temp );
}

void KX2DungeonTool::EditingCheckBox( KX2DungeonTool::DIALOG_CONTROL_ID id, bool& boolData )
{
    boolData = m_toolUI.GetCheckBox( id )->GetChecked();
}

template<typename T>
void KX2DungeonTool::EditingComboBox( KX2DungeonTool::DIALOG_CONTROL_ID id, T& data )
{
	return EditingComboBox( m_toolUI, id, data, GET_GO_TYPE() );
}

template<typename T, typename T2>
void KX2DungeonTool::EditingComboBox( CDXUTDialog& dialog, KX2DungeonTool::DIALOG_CONTROL_ID id, T& data, T2& GetValue )
{
	std::wstring wsTemp;

	DXUTComboBoxItem* item =  dialog.GetComboBox( id )->GetSelectedItem();
	wsTemp = item->strText;

	data = GetValue( wsTemp );
}

HRESULT KX2DungeonTool::CreateD3DXTextMesh( IDirect3DDevice9* pd3dDevice, LPD3DXMESH* ppMesh, 
											   TCHAR* pstrFont, DWORD dwSize,
											   BOOL bBold, BOOL bItalic, WCHAR* wszText)
{
	HRESULT hr;
	LPD3DXMESH pMeshNew = NULL;
	HDC hdc = CreateCompatibleDC( NULL );
	if( hdc == NULL )
		return E_OUTOFMEMORY;
	INT nHeight = -MulDiv( dwSize, GetDeviceCaps(hdc, LOGPIXELSY), 72 );
	HFONT hFont;
	HFONT hFontOld;

	hFont = CreateFont(nHeight, 0, 0, 0, bBold ? FW_BOLD : FW_NORMAL, bItalic, FALSE, FALSE, DEFAULT_CHARSET, 
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, pstrFont);

	hFontOld = (HFONT)SelectObject(hdc, hFont); 

	hr = D3DXCreateText( pd3dDevice, hdc, wszText, 0.001f, 0.01f, &pMeshNew, NULL, NULL);

	SelectObject(hdc, hFontOld);
	DeleteObject( hFont );
	DeleteDC( hdc );

	if( SUCCEEDED( hr ) )
		*ppMesh = pMeshNew;

	return hr;
}

void KX2DungeonTool::DeleteAllSkinMeshAndSkinAnim()
{
	//SAFE_CLOSE(m_pXSkinMesh);
	for( unsigned i=0; i<vecpXSkinAnim.size(); i++ )
	{
		SAFE_DELETE( (CKTDGXSkinAnim*)vecpXSkinAnim[i] );
	}
	vecpXSkinAnim.clear();

	map< CX2UnitManager::NPC_UNIT_ID, CKTDXDeviceXSkinMesh* >::iterator it;
	for( it=m_mapXSkinMesh.begin(); it!=m_mapXSkinMesh.end(); it++ )
	{
		CKTDXDeviceXSkinMesh* pSkinMesh = (CKTDXDeviceXSkinMesh*)it->second;
		SAFE_CLOSE(pSkinMesh);
	}
	m_mapXSkinMesh.clear();

	map< std::pair<CX2DungeonSubStage::NPCData*, int >, CKTDGXSkinAnim* >::iterator it2;
	for( it2=m_mapNPCXSkinAnim.begin(); it2 !=m_mapNPCXSkinAnim.end(); it2++ )
	{
		CKTDGXSkinAnim* pCKTDGXSkinAnim = (CKTDGXSkinAnim*)it2->second;
		SAFE_DELETE( pCKTDGXSkinAnim );
	}
	m_mapNPCXSkinAnim.clear();
}

bool KX2DungeonTool::HighlightPickedSegments()
{
	float fColor = fabs( sinf( (float)g_pKTDXApp->GetTime()*4.f ) );
	m_coHighlight = D3DXCOLOR(fColor, fColor, fColor, 0.5f);

	HRESULT				hr;
	LINE_MAP_VERTEX*	pVertices = NULL;

	if(FAILED(hr = m_pVBHighlightedSegment->Lock( 0, m_nVBHighlightedSegmentSize * sizeof(LINE_MAP_VERTEX), (void**) &pVertices, D3DLOCK_DISCARD )))
		return false;
	for( unsigned i=0; i<m_veciPickedSegmentIndex.size(); i++ )
	{
		CKTDGLineMap::LineData* pLineData = m_pWorld->GetLineMap()->GetLineData( (int)m_veciPickedSegmentIndex[i] );
		pVertices[i*2].pos		= pLineData->startPos;
		pVertices[i*2+1].pos	= pLineData->endPos;
		pVertices[i*2].color	= D3DCOLOR_COLORVALUE( m_coHighlight.r, m_coHighlight.g, m_coHighlight.b, m_coHighlight.a );
		pVertices[i*2+1].color	= D3DCOLOR_COLORVALUE( m_coHighlight.r, m_coHighlight.g, m_coHighlight.b, m_coHighlight.a );
	}
	if(FAILED(hr = m_pVBHighlightedSegment->Unlock()))
		return false;

	return true;
}

bool KX2DungeonTool::LoadDungeonToolOption()
{	
	KLuaManager lm;
	u_int time;
	int iUIWideMode;
	std::wstring temp;
	HRESULT hr;

	if( lm.DoFile( "DungeonToolOpt.lua" ) == false )
	{
		if( lm.DoFile( "D:\\projectx2\\Bin\\던전툴\\DungeonToolOpt.lua" ) == false )
		{
			if( lm.DoFile( "D:\\projectx2\\X2Project\\X2DungeonTool\\DungeonToolOpt.lua" ) == false )
			{
				MessageBox( NULL, L"DungeonToolOpt.lua파일을 읽지 못하였습니다!", L"", MB_OK );
				return false;
			}
		}
	}

	hr = lm.GetValue( L"MESH_FILE_PATH", temp );
	if( hr != E_FAIL )
	{		
		std::string addDir;
		ConvertWCHARToChar( addDir, temp.c_str() );
		g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->AddDataDirectory( addDir );
	}

	hr = lm.GetValue( L"QUICK_SAVE_PATH", temp );
	if( hr == E_FAIL )
	{
		MessageBox( NULL, L"DungeonToolOpt.lua파일을 읽지 못하였습니다!", L"", MB_OK );
		m_wsQuickSavePath = L"D:\\";
	}
	else
	{
		m_wsQuickSavePath = temp;
	}
	
	hr = lm.GetValue( L"AUTO_SAVE_PATH", temp );
	if( hr == E_FAIL )
	{
		MessageBox( NULL, L"DungeonToolOpt.lua파일을 읽지 못하였습니다!", L"", MB_OK );
		m_wsAutoSavePath = L"D:\\";
	}
	else
	{
		m_wsAutoSavePath = temp;
	}

	hr = lm.GetValue( L"AUTO_SAVE_TIME", time );
	if( hr == E_FAIL )
	{
		MessageBox( NULL, L"DungeonToolOpt.lua파일을 읽지 못하였습니다!", L"", MB_OK );
		m_iAutoSaveTime = 3 * 60000;
	}
	else
	{
		m_iAutoSaveTime  = time * 60000;
	}

	hr = lm.GetValue( L"UI_WIDE_MODE", iUIWideMode );
	if( hr == E_FAIL )
	{
		MessageBox( NULL, L"DungeonToolOpt.lua파일을 읽지 못하였습니다!", L"", MB_OK );
		m_bUIWideMode = false;
	}
	else
	{
		if( iUIWideMode == 1 )
		{
			m_bUIWideMode = true;
		}
		else
		{
			m_bUIWideMode = false;
		}
	}

	return true;
}

bool KX2DungeonTool::PickClickedMap( POINTS mousepos )
{
	WCHAR buf[128] = L"";
	CX2Dungeon::DungeonMapData* pMapData;

	if( m_pEditingDungeon == NULL )
		return false;

	// 텍스쳐좌표를 화면좌표로 변환
	D3DXVECTOR2 vecTex = g_pKTDXApp->ConvertByResolution( (float)(m_pDungeonMapUI->GetStageTexture()->GetWidth())*0.5f,
									(float)(m_pDungeonMapUI->GetStageTexture()->GetHeight())*0.5f );	

	// 마우스좌표를 미니맵 출력 기준 좌표에 맞춤
	mousepos.x = mousepos.x - (SHORT)(250.f * g_pKTDXApp->GetResolutionScaleX());
	mousepos.y = mousepos.y - (SHORT)(250.f * g_pKTDXApp->GetResolutionScaleY());

	DungeonMapIterator mit = m_mapDungeonMapList.begin();
	for( ; mit != m_mapDungeonMapList.end(); mit++ )
	{
		pMapData = &mit->second;

		// 맵 좌표를 화면좌표로 변환
		D3DXVECTOR2 mapPos = g_pKTDXApp->ConvertByResolution( pMapData->m_vPos );

		if( (mapPos.x-vecTex.x) < mousepos.x && (mapPos.x+vecTex.x) > mousepos.x &&
			(mapPos.y-vecTex.y) < mousepos.y && (mapPos.y+vecTex.y) > mousepos.y )
		{
			m_pEditingMiniMap = pMapData;
			m_beforeMousePos = mousepos;
			m_bPickMiniMap = true;

			wsprintf( buf, L"%d", (int)m_pEditingMiniMap->m_vPos.x );
			m_mapUI.GetEditBox( IDC_MAP_X_EDIT )->SetText( buf );
			wsprintf( buf, L"%d", (int)m_pEditingMiniMap->m_vPos.y );
			m_mapUI.GetEditBox( IDC_MAP_Y_EDIT )->SetText( buf );
			m_mapUI.GetCheckBox( IDC_MAP_BOSS_CHECK )->SetChecked( m_pEditingMiniMap->m_bIsBossStage );
			m_mapUI.GetCheckBox( IDC_IS_NEXT_RANDOM_STAGE_CHECK )->SetChecked( m_pEditingMiniMap->m_bIsNextStageRandom );

			m_pDungeonMapUI->SetSelectedMiniMap( (mit->first).first, (mit->first).second );
			return true;
		}
	}

	return false;
}

bool KX2DungeonTool::MovePickMap( POINTS mousepos )
{
	if( m_pEditingMiniMap == NULL )
		return false;

	// 마우스좌표를 미니맵 출력 기준 좌표에 맞춤
	mousepos.x = mousepos.x - (SHORT)(250.f * g_pKTDXApp->GetResolutionScaleX());
	mousepos.y = mousepos.y - (SHORT)(250.f * g_pKTDXApp->GetResolutionScaleY());

	if( (mousepos.x - m_beforeMousePos.x) != 0 )
	{
		m_pEditingMiniMap->m_vPos.x = m_pEditingMiniMap->m_vPos.x + ((mousepos.x - m_beforeMousePos.x) / g_pKTDXApp->GetResolutionScaleX());
	}

	if( (mousepos.y - m_beforeMousePos.y) != 0 )
	{
		m_pEditingMiniMap->m_vPos.y = m_pEditingMiniMap->m_vPos.y + ((mousepos.y - m_beforeMousePos.y) / g_pKTDXApp->GetResolutionScaleY());
	}	
	m_beforeMousePos = mousepos;

	if( m_mapUI.GetCheckBox( IDC_MAP_ALIGN_CHECK )->GetChecked() )
	{
		// 근접한 미니맵을 찾자
		DungeonMapIterator mit = m_mapDungeonMapList.begin();
		for( ; mit != m_mapDungeonMapList.end(); mit++ )
		{
			if( &mit->second == m_pEditingMiniMap )
				continue;

			if( abs( (mit->second).m_vPos.x - m_pEditingMiniMap->m_vPos.x ) < 5.0f )
			{
				m_pEditingMiniMap->m_vPos.x = (mit->second).m_vPos.x;
			}

			if( abs( (mit->second).m_vPos.y - m_pEditingMiniMap->m_vPos.y ) < 5.0f )
			{
				m_pEditingMiniMap->m_vPos.y = (mit->second).m_vPos.y;
			}
		}
	}	

	WCHAR buf[8] = L"";
	wsprintf( buf, L"%d", (int)m_pEditingMiniMap->m_vPos.x );
	m_mapUI.GetEditBox( IDC_MAP_X_EDIT )->SetText( buf );
	wsprintf( buf, L"%d", (int)m_pEditingMiniMap->m_vPos.y );
	m_mapUI.GetEditBox( IDC_MAP_Y_EDIT )->SetText( buf );

	return true;
}

bool KX2DungeonTool::LoadTexture( WCHAR* wszFileName )
{
	WCHAR FileName[256] = L"";
	WCHAR drive[_MAX_DRIVE];
	WCHAR dir[_MAX_DIR];
	WCHAR fname[_MAX_FNAME];
	WCHAR ext[_MAX_EXT];

	_wsplitpath( wszFileName, drive, dir, fname, ext);
	wcscat( FileName, fname);
	wcscat( FileName, ext);

	if( m_pEditingDungeon == NULL )
		return false;

	

	SAFE_DELETE( m_pDungeonMapUI );
	m_pDungeonMapUI = new KX2DTDungeonMapUI( m_pEditingDungeon );
	if( m_pDungeonMapUI == NULL )
		return false;

	//m_mapUI.GetEditBox( IDC_TEXTURE_FILE_NAME_EDIT )->SetText( FileName );
	m_pDungeonMapUI->SetShow( true );
	return true;
}

int KX2DungeonTool::GetNextUniqueNPCStartPosKey()
{
	for( int i=1; i<sizeof(m_abKeyUsed); i++ )
	{
		m_CurrKeyUsed = (m_CurrKeyUsed + 1) % (int)sizeof(m_abKeyUsed);
		if( false == m_abKeyUsed[m_CurrKeyUsed] )
		{
			return m_CurrKeyUsed;
		}
	}
	return -1;
}

bool KX2DungeonTool::LoadConditionElements( CX2DungeonSubStage::CLEAR_CONDITION eClearCon )
{
	WCHAR buf[128] = L"";

	switch( eClearCon ) 
	{
	case CX2DungeonSubStage::CC_KILL_ALL_ACTIVE_NPC:
		{
			// 컨트롤 설정
            m_LeftToolUI.GetButton( IDC_ADD_CONDITION_ELEMENT )->SetEnabled( false );
			m_LeftToolUI.GetButton( IDC_DEL_CONDITION_ELEMENT )->SetEnabled( false );
			m_LeftToolUI.GetListBox( IDC_CONDITION_ELEMENT_LIST )->SetEnabled( false );
			m_LeftToolUI.GetComboBox( IDC_CONDITION_ELEMENT_COMBO )->SetEnabled( false );
			m_LeftToolUI.GetListBox( IDC_CONDITION_ELEMENT_LIST )->RemoveAllItems();
			m_LeftToolUI.GetComboBox( IDC_CONDITION_ELEMENT_COMBO )->RemoveAllItems();
		}
		break;

	case CX2DungeonSubStage::CC_KILL_KEYCODE_NPC:
	case CX2DungeonSubStage::CC_KILL_KEYCODE_NPC_ANY:
		{
			// 컨트롤 설정
			m_LeftToolUI.GetButton( IDC_ADD_CONDITION_ELEMENT )->SetEnabled( true );
			m_LeftToolUI.GetButton( IDC_DEL_CONDITION_ELEMENT )->SetEnabled( true );
			m_LeftToolUI.GetListBox( IDC_CONDITION_ELEMENT_LIST )->SetEnabled( true );
			m_LeftToolUI.GetComboBox( IDC_CONDITION_ELEMENT_COMBO )->SetEnabled( true );
			m_LeftToolUI.GetListBox( IDC_CONDITION_ELEMENT_LIST )->RemoveAllItems();
			m_LeftToolUI.GetComboBox( IDC_CONDITION_ELEMENT_COMBO )->RemoveAllItems();

			m_LeftToolUI.GetButton( IDC_ADD_CONDITION_ELEMENT )->SetText( L"Add Key Code" );
			m_LeftToolUI.GetButton( IDC_DEL_CONDITION_ELEMENT )->SetText( L"Del Key Code" );

			// 리스트 추가
			if( m_pEditingClearCon == NULL )
				return false;
			
			std::vector< MonsterCount >::iterator vitKeyCode = m_pEditingClearCon->m_vecNPCKeyCode.begin();
			for( ; vitKeyCode != m_pEditingClearCon->m_vecNPCKeyCode.end(); vitKeyCode++ )
			{
				wsprintf( buf, L"NPC KEY CODE %2d", vitKeyCode->m_iKey );
				m_LeftToolUI.GetListBox( IDC_CONDITION_ELEMENT_LIST )->AddItem( buf, reinterpret_cast<LPVOID>( vitKeyCode->m_iKey ) );
			}

			// 콤보박스 리스트 추가
			if( m_pEditingSubStage == NULL )
				return false;

			CDXUTComboBox* pCombo = m_LeftToolUI.GetComboBox( IDC_CONDITION_ELEMENT_COMBO );
			std::vector< CX2DungeonSubStage::NPCData* >::iterator vitNpcData = m_pEditingSubStage->m_NPCDataList.begin();
			for( ; vitNpcData != m_pEditingSubStage->m_NPCDataList.end(); vitNpcData++ )
			{
				CX2DungeonSubStage::NPCData* pNpcData = *vitNpcData;
				if( pNpcData->m_KeyCode > 0 )
				{
					wsprintf( buf, L"NPC KEY CODE %2d", pNpcData->m_KeyCode );
					pCombo->AddItem( buf, reinterpret_cast<LPVOID>(pNpcData->m_KeyCode) );
				}
			}
		}
		break;

	case CX2DungeonSubStage::CC_KILL_NPC_TYPE:
		{
			// 컨트롤 설정
			m_LeftToolUI.GetButton( IDC_ADD_CONDITION_ELEMENT )->SetEnabled( true );
			m_LeftToolUI.GetButton( IDC_DEL_CONDITION_ELEMENT )->SetEnabled( true );
			m_LeftToolUI.GetListBox( IDC_CONDITION_ELEMENT_LIST )->SetEnabled( true );
			m_LeftToolUI.GetComboBox( IDC_CONDITION_ELEMENT_COMBO )->SetEnabled( true );
			m_LeftToolUI.GetListBox( IDC_CONDITION_ELEMENT_LIST )->RemoveAllItems();
			m_LeftToolUI.GetComboBox( IDC_CONDITION_ELEMENT_COMBO )->RemoveAllItems();

			m_LeftToolUI.GetButton( IDC_ADD_CONDITION_ELEMENT )->SetText( L"Add Npc ID" );
			m_LeftToolUI.GetButton( IDC_DEL_CONDITION_ELEMENT )->SetText( L"Del Npc ID" );

			// 리스트 추가
			if( m_pEditingClearCon == NULL )
				return false;
			
			std::vector< MonsterCount >::iterator vitNpcID = m_pEditingClearCon->m_vecNPCID.begin();
			for( ; vitNpcID != m_pEditingClearCon->m_vecNPCID.end(); vitNpcID++ )
			{
				NpcListIterator mit = m_mapNpcList.find( static_cast<CX2UnitManager::NPC_UNIT_ID>(vitNpcID->m_iKey) );
				m_LeftToolUI.GetListBox( IDC_CONDITION_ELEMENT_LIST )->AddItem( mit->second.c_str(), reinterpret_cast<LPVOID>( vitNpcID->m_iKey ) );
			}

			// 콤보박스 리스트 추가
			if( m_pEditingSubStage == NULL )
				return false;

			CDXUTComboBox* pCombo = m_LeftToolUI.GetComboBox( IDC_CONDITION_ELEMENT_COMBO );
			std::vector< CX2UnitManager::NPC_UNIT_ID >::iterator vitNpcUnitID = m_pEditingStage->m_ReadyNPCList.begin();
			for( ; vitNpcUnitID != m_pEditingStage->m_ReadyNPCList.end(); vitNpcUnitID++ )
			{
				NpcListIterator mit = m_mapNpcList.find( *vitNpcUnitID );
				pCombo->AddItem( mit->second.c_str(), reinterpret_cast<LPVOID>(*vitNpcUnitID) );
			}
		}
		break;

	default:
		return false;
	}

	return true;
}

bool KX2DungeonTool::LoadSummaryInfo()
{
	if( m_pEditingStage == NULL )
	{
		MessageBox( NULL, L"현재 편집중인 스테이지가 없습니다!", L"오류!", MB_OK );
		return false;
	}

	// summary list 초기화
	m_mapSummaryList.clear();
	
	// summary info 얻기
	std::vector< CX2DungeonSubStage::SubStageData* >::iterator vit;
	vit = m_pEditingStage->m_SubStageDataList.begin();
	for( ; vit != m_pEditingStage->m_SubStageDataList.end(); ++vit )
	{
		CX2DungeonSubStage::SubStageData* pSubStageData = *vit;
		std::vector< CX2DungeonSubStage::NPCData* >::iterator vitNpc = pSubStageData->m_NPCDataList.begin();
		for( ; vitNpc != pSubStageData->m_NPCDataList.end(); ++vitNpc )
		{
			CX2DungeonSubStage::NPCData* pNpcData = *vitNpc;

			// active가 아니면 통과
			if( pNpcData->m_bActive == false )
				continue;

			SummaryIterator sit = m_mapSummaryList.find( static_cast<int>(pNpcData->m_UnitID) );
			if( sit == m_mapSummaryList.end() )
			{
				SUMMARY_INFO summaryInfo;
				summaryInfo.m_iNpcID	  = static_cast<int>(pNpcData->m_UnitID);

				NpcListIterator npcIt = m_mapNpcList.find( pNpcData->m_UnitID );
				if( npcIt != m_mapNpcList.end() )
				{
					summaryInfo.m_wstrNpcName = (npcIt->second).c_str();
				}
				else
				{
					summaryInfo.m_wstrNpcName = L"NPC 이름 정보가 없습니다!";
				}

				KDropTable::DROP_DATA dropData;
				if( m_dropTable.DropItem( m_pEditingDungeon->GetDungeonData()->m_DungeonID, summaryInfo.m_iNpcID, dropData ) == false )
				{
					MessageBox( NULL, L"드롭 테이블 정보 얻기 실패!", L"오류!", MB_OK );
				}

				// 하드레벨 얻기
				CX2UnitManager::NPCUnitTemplet* pNpcUnitTemp = m_unitManager.GetNPCUnitTemplet( static_cast<CX2UnitManager::NPC_UNIT_ID>(summaryInfo.m_iNpcID) );

				int iHardLevelExp = 0;
				m_gunpcManager.GetHardLevelExp( pNpcUnitTemp->m_LuaFileName, iHardLevelExp );
                
				summaryInfo.m_iNpcCount = 1;
				if( dropData.m_iExp != -1 )
					summaryInfo.m_iNpcExp	= ( dropData.m_iExp + ( pNpcData->m_Level * iHardLevelExp ) );
				if( dropData.m_iED != -1 )
					summaryInfo.m_iNpcED	= dropData.m_iED;

				m_mapSummaryList.insert( std::make_pair( static_cast<int>(pNpcData->m_UnitID), summaryInfo ) );
			}
			else
			{
				SUMMARY_INFO& summaryInfo = sit->second;

				KDropTable::DROP_DATA dropData;
				if( m_dropTable.DropItem( m_pEditingDungeon->GetDungeonData()->m_DungeonID, summaryInfo.m_iNpcID, dropData ) == false )
				{
					MessageBox( NULL, L"드롭 테이블 정보 얻기 실패!", L"오류!", MB_OK );
				}

				// 하드레벨 얻기
				CX2UnitManager::NPCUnitTemplet* pNpcUnitTemp = m_unitManager.GetNPCUnitTemplet( static_cast<CX2UnitManager::NPC_UNIT_ID>(summaryInfo.m_iNpcID) );

				int iHardLevelExp = 0;
				m_gunpcManager.GetHardLevelExp( pNpcUnitTemp->m_LuaFileName, iHardLevelExp );

				summaryInfo.m_iNpcCount	+= 1;
				if( dropData.m_iExp != -1 )
					summaryInfo.m_iNpcExp	+= ( dropData.m_iExp + ( pNpcData->m_Level * iHardLevelExp ) );
				if( dropData.m_iED != -1 )
					summaryInfo.m_iNpcED	+= dropData.m_iED;
			}
		}
	}

	// summary list 채우기
	KX2DTListBox* pListBox = m_summaryUI.GetListBox( IDC_SUMMARY_LIST );
	pListBox->RemoveAllItems();
	
	SUMMARY_INFO totalSummary;
	WCHAR buf[1024] = L"";
	SummaryIterator sit = m_mapSummaryList.begin();
	for( ; sit != m_mapSummaryList.end(); ++sit )
	{
		const SUMMARY_INFO& summaryInfo = sit->second;

		wsprintf( buf, L"%15d | %15d | %15d | %15d | %30s", summaryInfo.m_iNpcID, summaryInfo.m_iNpcCount, summaryInfo.m_iNpcExp, summaryInfo.m_iNpcED, summaryInfo.m_wstrNpcName.c_str() );
		pListBox->AddItem( buf, NULL );

		totalSummary.m_iNpcCount += summaryInfo.m_iNpcCount;
		totalSummary.m_iNpcExp	 += summaryInfo.m_iNpcExp;
		totalSummary.m_iNpcED	 += summaryInfo.m_iNpcED;
	}

	wsprintf( buf, L"%d", totalSummary.m_iNpcCount );
	m_summaryUI.GetEditBox( IDC_NPC_TOTAL_EDIT )->SetText( buf );
	wsprintf( buf, L"%d", totalSummary.m_iNpcExp );
	m_summaryUI.GetEditBox( IDC_EXP_TOTAL_EDIT )->SetText( buf );
	wsprintf( buf, L"%d", totalSummary.m_iNpcED );
	m_summaryUI.GetEditBox( IDC_ED_TOTAL_EDIT )->SetText( buf );

	return true;
}
