#include "dxstdafx.h"
#include ".\x2linemaptoolmain.h"

/********************************************************************
created:	2006/09/08
created:	8:9:2006   10:26
filename: 	d:\projectX2\X2Project\TestEmpty\LineMapTool.cpp
file path:	d:\projectX2\X2Project\TestEmpty
file base:	LineMapTool
file ext:	cpp
author:		

to do: 		- line의 끝을 나타내는 sphere의 색이 제대로 안 보인다
			- 임시 script file을 프로그램이 종료할 때 삭제해야 한다.
			- listbox에서 item을 삭제할때 scroll 위치가 초기화 된다
			- 엘소드 클릭해서 start pos 위치 옮기기


purpose:	
*********************************************************************/




void CALLBACK OnLineMapToolUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
	OPENFILENAME ofn;        
	WCHAR wszFileName[512] = L"";  // path까지 포함한 파일 이름
	WCHAR wszOutput[512] = L"";

	CX2LineMapToolMain* pLineMapTool = (CX2LineMapToolMain*) pUserContext;

	switch( nControlID )
	{
    case CX2LineMapToolMain::IDC_CHECK_SECTOR:
        {
            pLineMapTool->SetToggleSector();
            pLineMapTool->CreateSector();
        } break;        
    case CX2LineMapToolMain::IDC_EDIT_WIDTH:
        {
            switch( nEvent )
            {
            case EVENT_EDITBOX_STRING:
                {
                    int iWidth = _wtoi( ((CDXUTEditBox*)pControl)->GetText() );
                    pLineMapTool->SetSectorWidth(iWidth);
                } break;
            }

        } break;
    case CX2LineMapToolMain::IDC_EDIT_HEIGHT:
        {
            switch( nEvent )
            {
            case EVENT_EDITBOX_STRING:
                {
                    int iHeight = _wtoi( ((CDXUTEditBox*)pControl)->GetText() );
                    pLineMapTool->SetSectorHeight(iHeight);
                } break;
            }

        } break;
    case CX2LineMapToolMain::IDC_EDIT_DEPTH:
        {
            switch( nEvent )
            {
            case EVENT_EDITBOX_STRING:
                {
                    int iDepth = _wtoi( ((CDXUTEditBox*)pControl)->GetText() );
                    pLineMapTool->SetSectorDepth(iDepth);
                } break;
            }

        } break;
    case CX2LineMapToolMain::IDC_BUTTON_FILE_TILE_SAVE:
        {
            ZeroMemory(&ofn, sizeof(ofn));
            ofn.lStructSize = sizeof(ofn);
            ofn.hwndOwner = g_pKTDXApp->GetHWND(); 
            ofn.lpstrFile = (LPWSTR)wszFileName;
            ofn.lpstrFile[0] = '\0';
            ofn.nMaxFile = sizeof(wszFileName);
            ofn.lpstrFilter = L"Sector\0*.lua\0";
            ofn.nFilterIndex = 1;
            ofn.lpstrFileTitle = NULL;
            ofn.nMaxFileTitle = 0;
            ofn.lpstrInitialDir = NULL;
            ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
            if( GetSaveFileName(&ofn) == TRUE ) 
            {
                if( false == pLineMapTool->SaveSectorFile( ofn.lpstrFile ) )
                {
                    MessageBox( g_pKTDXApp->GetHWND(), L"읽기전용파일이 아닌지 확인해주세요.", L"파일저장실패", MB_OK );
                }
            }
        }
        break;
    case CX2LineMapToolMain::IDC_BUTTON_TEXT_TILE_SAVE:
        {
            string strFilePath;
            pLineMapTool->SaveSectorFile( pLineMapTool->GetTempFilePath(), false );
            ConvertWCHARToChar( strFilePath, pLineMapTool->GetTempFilePath() );
            _spawnlp( _P_NOWAIT, "notepad.exe", "notepad.exe", strFilePath.c_str(), NULL );
        } break;        

	case CX2LineMapToolMain::IDC_BUTTON_FILE_OPEN:
		{
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = g_pKTDXApp->GetHWND(); 
			ofn.lpstrFile = (LPWSTR)wszFileName;
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(wszFileName);
			ofn.lpstrFilter = L"linemap\0*.lua\0";
			ofn.nFilterIndex = 1;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			if( GetOpenFileName(&ofn) == TRUE ) 
			{
				pLineMapTool->LoadLineMapFromFile( ofn.lpstrFile );
			}
		} break;
	case CX2LineMapToolMain::IDC_BUTTON_FILE_SAVE:
		{
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = g_pKTDXApp->GetHWND(); 
			ofn.lpstrFile = (LPWSTR)wszFileName;
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(wszFileName);
			ofn.lpstrFilter = L"linemap\0*.lua\0";
			ofn.nFilterIndex = 1;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			if( GetSaveFileName(&ofn) == TRUE ) 
			{
				if( false == pLineMapTool->SaveLineMapToFile( ofn.lpstrFile ) )
				{
					MessageBox( g_pKTDXApp->GetHWND(), L"읽기전용파일이 아닌지 확인해주세요.", L"파일저장실패", MB_OK );
				}
			}
		} break;
	case CX2LineMapToolMain::IDC_BUTTON_TO_TEXT:
		{
			string strFilePath;
			pLineMapTool->SaveLineMapToFile( pLineMapTool->GetTempFilePath(), false );
			ConvertWCHARToChar( strFilePath, pLineMapTool->GetTempFilePath() );
			_spawnlp( _P_NOWAIT, "notepad.exe", "notepad.exe", strFilePath.c_str(), NULL );
		} break;
	case CX2LineMapToolMain::IDC_BUTTON_FROM_TEXT:
		{
			pLineMapTool->LoadLineMapFromFile( pLineMapTool->GetTempFilePath() );			
		} break;
	case CX2LineMapToolMain::IDC_BUTTON_TEAM_COLOR:
		if( pLineMapTool->GetTeamColor() == CX2LineMapToolMain::TEAM_RED )
		{
			pLineMapTool->GetTeamColor() = CX2LineMapToolMain::TEAM_BLUE;
			pLineMapTool->GetMainUI().GetButton(CX2LineMapToolMain::IDC_BUTTON_TEAM_COLOR)->SetText(L"BLUE");
		}
		else if( pLineMapTool->GetTeamColor() == CX2LineMapToolMain::TEAM_BLUE )
		{
			pLineMapTool->GetTeamColor() = CX2LineMapToolMain::TEAM_NONE;
			pLineMapTool->GetMainUI().GetButton(CX2LineMapToolMain::IDC_BUTTON_TEAM_COLOR)->SetText(L"NONE");
		}
		//else if( pLineMapTool->GetTeamColor() == CX2LineMapToolMain::TEAM_NONE )
		//{
		//	pLineMapTool->GetTeamColor() = CX2LineMapToolMain::TEAM_DUNGEON_USER;
		//	pLineMapTool->GetMainUI().GetButton(CX2LineMapToolMain::IDC_BUTTON_TEAM_COLOR)->SetText(L"Dungeon");
		//}
		else
		{
			pLineMapTool->GetTeamColor() = CX2LineMapToolMain::TEAM_RED;
			pLineMapTool->GetMainUI().GetButton(CX2LineMapToolMain::IDC_BUTTON_TEAM_COLOR)->SetText(L"RED");
		}
		break;
	case CX2LineMapToolMain::IDC_BUTTON_INITIAL_DIR:
		if( pLineMapTool->GetDir() == CX2LineMapToolMain::DIR_LEFT )
		{
			pLineMapTool->GetDir() = CX2LineMapToolMain::DIR_RIGHT;
			pLineMapTool->GetMainUI().GetButton(CX2LineMapToolMain::IDC_BUTTON_INITIAL_DIR)->SetText(L"RIGHT");
		}
		else
		{
			pLineMapTool->GetDir() = CX2LineMapToolMain::DIR_LEFT;
			pLineMapTool->GetMainUI().GetButton(CX2LineMapToolMain::IDC_BUTTON_INITIAL_DIR)->SetText(L"LEFT");
		}
		break;
	case CX2LineMapToolMain::IDC_CHECK_NAVIGATE:
		{
			pLineMapTool->ToggleNavigate();
		} break;
	case CX2LineMapToolMain::IDC_EDIT_LAND_HEIGHT:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				//case EVENT_EDITBOX_CHANGE:
				pLineMapTool->SetLandHeight( (float)_wtof( ((CDXUTEditBox*)pControl)->GetText() ) );
				StringCchPrintf( wszOutput, 512, L"%.2f", pLineMapTool->GetLandHeight() );
				pLineMapTool->GetMainUI().GetEditBox( CX2LineMapToolMain::IDC_EDIT_LAND_HEIGHT )->SetText( wszOutput );
				break;
			}
		} break;
	case CX2LineMapToolMain::IDC_LISTBOX_START_POS:
		{
			switch( nEvent )
			{
			case EVENT_LISTBOX_SELECTION:
				{
					pLineMapTool->GetPickedStartPosIndex() = ((CDXUTListBox *)pControl)->GetSelectedIndex();
					pLineMapTool->UpdateStartPosComboBox();
				} break;
			}
		} break;
	case CX2LineMapToolMain::IDC_COMBOBOX_START_POS_TEAM_COLOR:
		{
			if( pLineMapTool->GetPickedStartPosIndex() < 0 )
				break;

			DXUTComboBoxItem *pItem = ((CDXUTComboBox*)pControl)->GetSelectedItem();
			if( pItem )
			{
				int iNewTeamColor;
				if( 0 == StrCmp( pItem->strText, L"RED" ) )
					iNewTeamColor = CX2LineMapToolMain::TEAM_RED;
				else if( 0 == StrCmp( pItem->strText, L"BLUE" ) )
					iNewTeamColor = CX2LineMapToolMain::TEAM_BLUE;
				else if( 0 == StrCmp( pItem->strText, L"NONE" ) )
					iNewTeamColor = CX2LineMapToolMain::TEAM_NONE;
				//else
				//	iNewTeamColor = CX2LineMapToolMain::TEAM_DUNGEON_USER;

				CX2LineMapToolMain::STeamStartPos& sPos = pLineMapTool->GetStartPos( pLineMapTool->GetPickedStartPosIndex() );
				sPos.iTeamColor = iNewTeamColor;
				
				pLineMapTool->UpdateStartPosListBox(true);
			}
		} break;
	case CX2LineMapToolMain::IDC_COMBOBOX_START_POS_DIRECTION:
		{            	
			if( pLineMapTool->GetPickedStartPosIndex() < 0 )
				break;

			DXUTComboBoxItem *pItem = ((CDXUTComboBox*)pControl)->GetSelectedItem();
			if( pItem )
			{
				int iNewDirection;
				if( 0 == StrCmp( pItem->strText, L"LEFT" ) )
					iNewDirection = CX2LineMapToolMain::DIR_LEFT;
				else 
					iNewDirection = CX2LineMapToolMain::DIR_RIGHT;

				CX2LineMapToolMain::STeamStartPos& sPos = pLineMapTool->GetStartPos( pLineMapTool->GetPickedStartPosIndex() );
				sPos.iInitialDir = iNewDirection;
				pLineMapTool->UpdateStartPosListBox(true);
			}
		} break;
	case CX2LineMapToolMain::IDC_LISTBOX_SEGMENT_INFO:
		{
			switch( nEvent )
			{
			case EVENT_LISTBOX_SELECTION:
				{
					pLineMapTool->GetPickedSegmentVector().clear();
					int nSelected = -1;
					while( ( nSelected = ((CDXUTListBox *)pControl)->GetSelectedIndex( nSelected ) ) != -1 )
					{
						pLineMapTool->GetPickedSegmentVector().push_back( nSelected );
					}

					pLineMapTool->UpdateSegmentInfoComboBox( false );
				} break;
			}
		} break;
	case CX2LineMapToolMain::IDC_EDIT_SEGMENT_INDEX:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
			case EVENT_EDITBOX_CHANGE:
				{
					int iPicked = _wtoi( ((CDXUTEditBox*)pControl)->GetText() );
					if( iPicked < 0 || iPicked > pLineMapTool->GetNumSegments()-1 )
					{
						((CDXUTEditBox*)pControl)->SetText( L"" );
						break;
					}

					pLineMapTool->GetPickedSegmentVector().clear();
					pLineMapTool->GetPickedSegmentVector().push_back( iPicked );
					pLineMapTool->UpdateSegmentInfoListBox( true );
					pLineMapTool->UpdateSegmentInfoComboBox( false );
				}
				break;
			}
		} break;

	case CX2LineMapToolMain::IDC_COMBOBOX_SEGMENT_ENABLE:
		{
			DXUTComboBoxItem *pItem = ((CDXUTComboBox*)pControl)->GetSelectedItem();
			if( pItem )
			{
				for( unsigned i=0; i<pLineMapTool->GetPickedSegmentVector().size(); i++ )
				{
					CKTDGLineMap::LineData* pLineData = pLineMapTool->GetLineData( pLineMapTool->GetPickedSegmentVector().at(i) );

					if( 0 == StrCmp( pItem->strText, L"ENABLE" ) )
						pLineData->bEnable = true;
					else
						pLineData->bEnable = false;
				}				
				pLineMapTool->UpdateSegmentInfoListBox(true);
			}

		} break;

	case CX2LineMapToolMain::IDC_COMBOBOX_SEGMENT_LINE_TYPE:
		{
			DXUTComboBoxItem *pItem = ((CDXUTComboBox*)pControl)->GetSelectedItem();
			if( pItem )
			{
				for( unsigned i=0; i<pLineMapTool->GetPickedSegmentVector().size(); i++ )
				{
					CKTDGLineMap::LineData* pLineData = pLineMapTool->GetLineData( pLineMapTool->GetPickedSegmentVector().at(i) );
					pLineData->lineType = (CKTDGLineMap::LINE_TYPE) (int) pItem->pData;
				}
				pLineMapTool->UpdateSegmentInfoListBox(true);
			}

		} break;


	case CX2LineMapToolMain::IDC_COMBOBOX_LINE_DUST_TYPE:
		{
			DXUTComboBoxItem *pItem = ((CDXUTComboBox*)pControl)->GetSelectedItem();
			if( NULL != pItem )
			{
				for( unsigned i=0; i<pLineMapTool->GetPickedSegmentVector().size(); i++ )
				{
					CKTDGLineMap::LineData* pLineData = pLineMapTool->GetLineData( pLineMapTool->GetPickedSegmentVector().at(i) );
					pLineData->eDustType = (CKTDGLineMap::LINE_DUST_TYPE) (int) pItem->pData;
				}
				pLineMapTool->UpdateSegmentInfoListBox(true);
			}
		} break;

	case CX2LineMapToolMain::IDC_COMBOBOX_SEGMENT_BEFORE_LINE:
		{
			DXUTComboBoxItem *pItem = ((CDXUTComboBox*)pControl)->GetSelectedItem();
			if( pItem )
			{
				for( unsigned i=0; i<pLineMapTool->GetPickedSegmentVector().size(); i++ )
				{
					CKTDGLineMap::LineData* pLineData = pLineMapTool->GetLineData( pLineMapTool->GetPickedSegmentVector().at(i) );
					pLineData->beforeLine = _wtoi( pItem->strText );
				}
				pLineMapTool->UpdateSegmentInfoListBox(true);
			}

		} break;
	case CX2LineMapToolMain::IDC_COMBOBOX_SEGMENT_NEXT_LINE:
		{
			DXUTComboBoxItem *pItem = ((CDXUTComboBox*)pControl)->GetSelectedItem();
			if( pItem )
			{
				for( unsigned i=0; i<pLineMapTool->GetPickedSegmentVector().size(); i++ )
				{
					CKTDGLineMap::LineData* pLineData = pLineMapTool->GetLineData( pLineMapTool->GetPickedSegmentVector().at(i) );
					pLineData->nextLine = _wtoi( pItem->strText );
				}
				pLineMapTool->UpdateSegmentInfoListBox(true);
			}
		} break;


	case CX2LineMapToolMain::IDC_COMBOBOX_CAN_PASS:
		{
			DXUTComboBoxItem *pItem = ((CDXUTComboBox*)pControl)->GetSelectedItem();
			if( NULL != pItem )
			{
				for( unsigned i=0; i<pLineMapTool->GetPickedSegmentVector().size(); i++ )
				{
					CKTDGLineMap::LineData* pLineData = pLineMapTool->GetLineData( pLineMapTool->GetPickedSegmentVector().at(i) );

					if( 0 == (int) pItem->pData )
					{
						pLineData->m_bCanPass = false;
					}
					else 
					{
						pLineData->m_bCanPass = true;
					}
				}				
				pLineMapTool->UpdateSegmentInfoListBox(true);
			}

		} break;


	case CX2LineMapToolMain::IDC_COMBOBOX_MONSTER_ALLOWED:
		{
			DXUTComboBoxItem *pItem = ((CDXUTComboBox*)pControl)->GetSelectedItem();
			if( NULL != pItem )
			{
				for( unsigned i=0; i<pLineMapTool->GetPickedSegmentVector().size(); i++ )
				{
					CKTDGLineMap::LineData* pLineData = pLineMapTool->GetLineData( pLineMapTool->GetPickedSegmentVector().at(i) );

					if( 0 == (int) pItem->pData )
					{
						pLineData->m_bMonsterAllowed = false;
					}
					else 
					{
						pLineData->m_bMonsterAllowed = true;
					}
				}				
				pLineMapTool->UpdateSegmentInfoListBox(true);
			}

		} break;



	case CX2LineMapToolMain::IDC_COMBOBOX_ENABLE_WALL_JUMP:
		{
			DXUTComboBoxItem *pItem = ((CDXUTComboBox*)pControl)->GetSelectedItem();
			if( NULL != pItem )
			{
				for( unsigned i=0; i<pLineMapTool->GetPickedSegmentVector().size(); i++ )
				{
					CKTDGLineMap::LineData* pLineData = pLineMapTool->GetLineData( pLineMapTool->GetPickedSegmentVector().at(i) );

					if( 0 == (int) pItem->pData )
					{
						pLineData->m_bEnableWallJump = false;
					}
					else 
					{
						pLineData->m_bEnableWallJump = true;
					}
				}				
				pLineMapTool->UpdateSegmentInfoListBox(true);
			}

		} break;

	case CX2LineMapToolMain::IDC_EDIT_SEGMENT_PORTAL_GROUP:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				{
					int iPortalGroup = _wtoi( ((CDXUTEditBox*)pControl)->GetText() );
					for( unsigned i=0; i<pLineMapTool->GetPickedSegmentVector().size(); i++ )
					{				
						CKTDGLineMap::LineData* pLineData = pLineMapTool->GetLineData( pLineMapTool->GetPickedSegmentVector().at(i) );
						pLineData->potalGroup = iPortalGroup;
					}
					pLineMapTool->UpdateSegmentInfoListBox(true);			
				} break;
			}

		} break;

	case CX2LineMapToolMain::IDC_EDIT_SEGMENT_STOP_FACTOR:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				{
					for( unsigned i=0; i<pLineMapTool->GetPickedSegmentVector().size(); i++ )
					{
						CKTDGLineMap::LineData* pLineData = pLineMapTool->GetLineData( pLineMapTool->GetPickedSegmentVector().at(i) );
						pLineData->fStopFactor = (float)_wtof( ((CDXUTEditBox*)pControl)->GetText() );
					}
					pLineMapTool->UpdateSegmentInfoListBox(true);
				} break;
			}
		} break;
		
	case CX2LineMapToolMain::IDC_EDIT_LINE_START_POS:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				{
					if( pLineMapTool->GetPickedSegmentVector().size() > 0 )
					{
						CKTDGLineMap::LineData* pLineData = pLineMapTool->GetLineData( pLineMapTool->GetPickedSegmentVector().at(0) );
						((CDXUTEditBox*)pControl)->ParseFloatArray( (float*)&pLineData->startPos, 3 );
						pLineMapTool->UpdateSegmentInfoListBox(true);
					}
				} break;
			}
		} break;

	case CX2LineMapToolMain::IDC_EDIT_LINE_END_POS:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				{
					if( pLineMapTool->GetPickedSegmentVector().size() > 0 )
					{
						CKTDGLineMap::LineData* pLineData = pLineMapTool->GetLineData( pLineMapTool->GetPickedSegmentVector().at(0) );
						((CDXUTEditBox*)pControl)->ParseFloatArray( (float*)&pLineData->endPos, 3 );
						pLineMapTool->UpdateSegmentInfoListBox(true);	
					}
				} break;
			}
		} break;

	case CX2LineMapToolMain::IDC_EDIT_LINE_SET_INDEX:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				{
					int lineSetIndex = _wtoi( ((CDXUTEditBox*)pControl)->GetText() );
					vector<int>& pickedSegment = pLineMapTool->GetPickedSegmentVector();
					if( pickedSegment.size() <= 0 )
					{						
						pLineMapTool->PickLineSet( lineSetIndex );

					}
					else
					{
						for( unsigned i=0; i<pickedSegment.size(); i++ )
						{
							CKTDGLineMap::LineData* pLineData = pLineMapTool->GetLineData( pickedSegment.at(i) );
							pLineData->lineSet = lineSetIndex;						
						}
						pLineMapTool->UpdateSegmentInfoListBox(true);	
					}
					
				} break;
			}
		} break;
			

	case CX2LineMapToolMain::IDC_EDIT_LINE_SPEED:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				{
					
					float fValue = (float)_wtof( ((CDXUTEditBox*)pControl)->GetText() );
					vector<int>& pickedSegment = pLineMapTool->GetPickedSegmentVector();
					if( pickedSegment.size() > 0 )
					{
						for( unsigned i=0; i<pickedSegment.size(); i++ )
						{
							CKTDGLineMap::LineData* pLineData = pLineMapTool->GetLineData( pickedSegment.at(i) );
							pLineData->m_fSpeed = fValue;
						}
						pLineMapTool->UpdateSegmentInfoListBox(true);	
					}

				} break;
			}
		} break;


	case CX2LineMapToolMain::IDC_EDIT_JUMP_SPEED:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				{
					if( pLineMapTool->GetPickedSegmentVector().size() > 0 )
					{
						CKTDGLineMap::LineData* pLineData = pLineMapTool->GetLineData( pLineMapTool->GetPickedSegmentVector().at(0) );
						((CDXUTEditBox*)pControl)->ParseFloatArray( (float*)&pLineData->m_vJumpSpeed, 3 );
						pLineMapTool->UpdateSegmentInfoListBox(true);	
					}
				} break;
			}
		} break;


	case CX2LineMapToolMain::IDC_EDIT_TELEPORT_LINE_INDEX:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				{
					int iValue = _wtoi( ((CDXUTEditBox*)pControl)->GetText() );
					for( unsigned i=0; i<pLineMapTool->GetPickedSegmentVector().size(); i++ )
					{				
						CKTDGLineMap::LineData* pLineData = pLineMapTool->GetLineData( pLineMapTool->GetPickedSegmentVector().at(i) );
						pLineData->m_iTeleportLineIndex = iValue;
					}
					pLineMapTool->UpdateSegmentInfoListBox(true);	

				} break;
			}
		} break;


	case CX2LineMapToolMain::IDC_BUTTON_HELP:
		{
			wstring wstrHelp;
			wstrHelp += L"shift + left click			: 시작 위치 지정 \n";
			wstrHelp += L"listbox에서 선택 + delete		: 시작 위치 삭제 \n";
			wstrHelp += L"left click				: 라인 선택 \n";
			wstrHelp += L"\n";
			wstrHelp += L"카메라 체크박스를 체크하고 카메라를 조작할 수 있습니다. \n";
			wstrHelp += L"w, a, s, d, q, e			: 카메라 이동 \n";			
			wstrHelp += L"right click + mouse move		: 카메라 회전 \n";
			wstrHelp += L"c				: 카메라 초기화 \n";
			wstrHelp += L"\n";
			wstrHelp += L"선끝에 빨간공은 통과할 수 없는 길 (-2)을, \n";
			wstrHelp += L"선끝에 파란공은 통과할 수 있는 길 (-1)을 나타낸다. \n";
			wstrHelp += L"land_height에 숫자입력 후 반드시 enter를 눌러야... \n";
			wstrHelp += L"선택된 line 하나의 시작 위치와 끝 위치를 숫자로 입력할 수 있다 \n";
			wstrHelp += L"X2의 dat폴더를 복사해서 넣어주세요.(enum.lua, npc mesh 파일 등등 때문에) \n";
			wstrHelp += L"작업할 라인맵 파일과, 던전 스크립트 파일을 dat폴더에서 삭제해주세요. (파일이름이 중복되면 안되니까~) \n";
			wstrHelp += L"작업할 라인맵 스크립트와 월드 스크립트, 던전 스크립트 세 가지를 모두 linemaptool 폴더에 복사해서 작업해주세요~ \n";

			MessageBox( g_pKTDXApp->GetHWND(), wstrHelp.c_str(), L"HELP", MB_OK );
		} break;

	case CX2LineMapToolMain::IDC_BUTTON_PICKED_LINE_INDEX:
		{
			vector<int>& vecPickedIndex = pLineMapTool->GetPickedSegmentVector();
			if( vecPickedIndex.size() <= 0 )
				break;

			string strFilePath = "C:\\temp_line_indices.txt";
			std::ofstream ofs( strFilePath.c_str() );

			std::sort( vecPickedIndex.begin(), vecPickedIndex.end() );

			for( unsigned i=0; i<vecPickedIndex.size(); i++ )
			{
				ofs << vecPickedIndex[i] << ", ";				
			}
			ofs << std::endl;
			ofs.close();

			_spawnlp( _P_NOWAIT, "notepad.exe", "notepad.exe", strFilePath.c_str(), NULL );

		} break;

	case CX2LineMapToolMain::IDC_BUTTON_BUILD_FLOOR_MESH:
		{
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = g_pKTDXApp->GetHWND(); 
			ofn.lpstrFile = (LPWSTR)wszFileName;
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(wszFileName);
			ofn.lpstrFilter = L"Xmesh\0*.x\0";
			ofn.nFilterIndex = 1;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			if( GetSaveFileName(&ofn) == TRUE ) 
			{
				pLineMapTool->BuildFloorMesh( ofn.lpstrFile, 240.f, 60.f );
			}
		} break;
	default:
		break;
	}
}


void CALLBACK OnLineMapToolUIEvent2( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
	OPENFILENAME ofn;        
	WCHAR wszFileName[512] = L"";  // path까지 포함한 파일 이름


	CX2LineMapToolMain* pLineMapTool = (CX2LineMapToolMain*) pUserContext;

	switch( nControlID )
	{
	case CX2LineMapToolMain::IDC_BUTTON_LOAD_NPC_FROM_DUNGEON_SCRIPT:
		{
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = g_pKTDXApp->GetHWND(); 
			ofn.lpstrFile = (LPWSTR)wszFileName;
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(wszFileName);
			ofn.lpstrFilter = L"linemap\0*.lua\0";
			ofn.nFilterIndex = 1;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			if( GetOpenFileName(&ofn) == TRUE ) 
			{
				pLineMapTool->LoadNPCPostionFromDungeonScript( ofn.lpstrFile );
			}
			
		} break;

	case CX2LineMapToolMain::IDC_COMBOBOX_DUNGEON_DIFFICULTY:
		{
			DXUTComboBoxItem *pItem = ((CDXUTComboBox*)pControl)->GetSelectedItem();
			if( pItem )
			{
				if( 0 == StrCmp( pItem->strText, L"ALL" ) )
					pLineMapTool->GetShowDungeonLevel() = 0;
				else if( 0 == StrCmp( pItem->strText, L"NORMAL" ) )
					pLineMapTool->GetShowDungeonLevel() = 1;
				else if( 0 == StrCmp( pItem->strText, L"HARD" ) )
					pLineMapTool->GetShowDungeonLevel() = 2;
				else if( 0 == StrCmp( pItem->strText, L"EXPERT" ) )
					pLineMapTool->GetShowDungeonLevel() = 3;
			}
		} break;

	case CX2LineMapToolMain::IDC_BUTTON_MOUSE_CLICK_TARGET:
		{
			switch( pLineMapTool->m_MouseClickTarget )
			{
			case CX2LineMapToolMain::MCT_LINE:
				{
					pLineMapTool->m_MouseClickTarget = CX2LineMapToolMain::MCT_NPC;
					((CDXUTButton*)pControl)->SetText( L"PICK NPC" );
				} break;

			case CX2LineMapToolMain::MCT_NPC:
				{
					pLineMapTool->m_MouseClickTarget = CX2LineMapToolMain::MCT_LINE;
					((CDXUTButton*)pControl)->SetText( L"PICK LINE" );
				} break;
			}
		} break;	

	case CX2LineMapToolMain::IDC_BUTTON_SHOW_USER_UNIT_START_POS:
		{
			pLineMapTool->m_bShowUserUnitStartPos = !pLineMapTool->m_bShowUserUnitStartPos;
			if( true == pLineMapTool->m_bShowUserUnitStartPos )
			{
				((CDXUTButton*)pControl)->SetText( L"HIDE USER UNIT" );
			}
			else
			{
				((CDXUTButton*)pControl)->SetText( L"SHOW USER UNIT" );
			}
		} break;

	case CX2LineMapToolMain::IDC_BUTTON_USE_NPC_MESH:
		{
			pLineMapTool->m_bUseNPCMesh = !pLineMapTool->m_bUseNPCMesh;
			if( true == pLineMapTool->m_bUseNPCMesh )
			{
				((CDXUTButton*)pControl)->SetText( L"USING NPC MESH" );
			}
			else
			{
				((CDXUTButton*)pControl)->SetText( L"USING ELSWORD MESH" );
			}
		} break;


	case CX2LineMapToolMain::IDC_EDIT_ALL_NPC_START_POS_DISPLACE:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				//case EVENT_EDITBOX_CHANGE:
				{
					D3DXVECTOR3 vDisplace(0,0,0);
					((CDXUTEditBox*)pControl)->ParseFloatArray( (float*)&vDisplace, 3 );
					pLineMapTool->DisplaceAllUserStartPos( vDisplace );
					pLineMapTool->DisplaceAllNPCStartPos( vDisplace );
					((CDXUTEditBox*)pControl)->SetText( L"0.0   0.0   0.0" );

				} break;
			} 
		} break;

	case CX2LineMapToolMain::IDC_EDIT_ALL_LINE_POS_DISPLACE:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				//case EVENT_EDITBOX_CHANGE:
				{
					D3DXVECTOR3 vDisplace(0,0,0);
					((CDXUTEditBox*)pControl)->ParseFloatArray( (float*)&vDisplace, 3 );
					((CDXUTEditBox*)pControl)->SetText( L"0.0   0.0   0.0" );

					for( int i=0; i<pLineMapTool->GetNumSegments(); i++ )
					{
						pLineMapTool->GetLineData(i)->startPos	+= vDisplace;
						pLineMapTool->GetLineData(i)->endPos	+= vDisplace;
					}

				} break;
			} 
		} break;

	case CX2LineMapToolMain::IDC_CHECK_SHOW_START_POS_SIMPLE:
		{
			pLineMapTool->m_bHideStartPosMesh = !pLineMapTool->m_bHideStartPosMesh;

		} break;
	}
}

void CX2LineMapToolMain::InitUI()
{
	// initialize UI
	extern CDXUTDialogResourceManager g_DialogResourceManager;
	MainUI.Init( &g_DialogResourceManager );
	MainUI.SetCallback( OnLineMapToolUIEvent, this );
	MainUI.SetSize( 340, 300 );
	MainUI.SetFont(0, L"굴림", 12, 12);

	int iX = 110;
	int iY = 10;
	int iDX = 100;

    MainUI.AddButton(IDC_BUTTON_TEXT_TILE_SAVE, L"text tile *.lua", iX-iDX-20, iY-25, 120, 22);
    MainUI.AddButton(IDC_BUTTON_FILE_TILE_SAVE, L"save tile *.lua", iX-iDX-20, iY, 120, 22);
	MainUI.AddButton(IDC_BUTTON_FILE_OPEN, L"load *.lua", iX, iY, 80, 22);
	MainUI.AddButton(IDC_BUTTON_FILE_SAVE, L"save *.lua", iX+iDX, iY, 80, 22);

	iY += 25;
    MainUI.AddStatic( IDC_STATIC_DUMMY, L"Width", iX-iDX-20, iY, 40, 30);
    MainUI.AddEditBox( IDC_EDIT_WIDTH, L"768", iX-iDX+30, iY, 60, 30);
    m_iSectorWidth = 768;

	MainUI.AddButton(IDC_BUTTON_FROM_TEXT, L"from txt", iX, iY, 80, 22);
	MainUI.AddButton(IDC_BUTTON_TO_TEXT, L"to txt", iX+iDX, iY, 80, 22);

	iY += 25;
    MainUI.AddStatic( IDC_STATIC_DUMMY, L"Height", iX-iDX-20, iY+10, 40, 30);
    MainUI.AddEditBox( IDC_EDIT_HEIGHT, L"768", iX-iDX+30, iY+10, 60, 30);
    m_iSectorHeight = 768;

	MainUI.AddButton( IDC_BUTTON_HELP, L"HELP", iX, iY, 80, 22);
	MainUI.AddCheckBox( IDC_CHECK_NAVIGATE, L"camera", iX+iDX, iY, 80, 22);
	MainUI.GetCheckBox( IDC_CHECK_NAVIGATE )->SetChecked( bNavigate );

	iY += 25;    
    MainUI.AddStatic( IDC_STATIC_DUMMY, L"Depth", iX-iDX-20, iY+20, 40, 30);
    MainUI.AddEditBox( IDC_EDIT_DEPTH, L"768", iX-iDX+30, iY+20, 60, 30);
    m_iSectorDepth = 768;

	MainUI.AddButton( IDC_BUTTON_PICKED_LINE_INDEX, L"picked_line", iX, iY, 80, 22);
	MainUI.AddButton( IDC_BUTTON_BUILD_FLOOR_MESH, L"build floor", iX+iDX, iY, 80, 22);

	iY += 25;
    m_bTileRender = false;
    MainUI.AddCheckBox( IDC_CHECK_SECTOR, L"Tile Render", iX-iDX-20, iY+30, 80, 22);
    MainUI.GetCheckBox( IDC_CHECK_SECTOR )->SetChecked( m_bTileRender );

	MainUI.AddStatic(IDC_STATIC_TEAM_COLOR, L"team color: ", iX, iY, 125, 22);
	MainUI.AddButton(IDC_BUTTON_TEAM_COLOR, L"RED", iX+iDX, iY, 80, 22);
	iY += 25;
	MainUI.AddStatic(IDC_STATIC_INITIAL_DIR, L"initial dir: ", iX, iY, 125, 22);
	MainUI.AddButton(IDC_BUTTON_INITIAL_DIR, L"LEFT", iX+iDX, iY, 80, 22);

	iY += 25;
	MainUI.AddStatic( IDC_STATIC_LAND_HEIGHT, L"land height: ", iX, iY, 125, 22 );
	MainUI.AddEditBox( IDC_EDIT_LAND_HEIGHT, L"0", iX+iDX, iY, 80, 30 );
	iY += 35;	
	MainUI.AddStatic( IDC_STATIC_SEGMENT_INFO, L"번호    팀    위치   방향", iX, iY, 225, 22);	
	iY += 25;
	MainUI.AddListBox( IDC_LISTBOX_START_POS, iX, iY, 220, 100, 0 );
	iY += 110;
	CDXUTComboBox *pCombo = NULL;
	MainUI.AddComboBox( IDC_COMBOBOX_START_POS_TEAM_COLOR, iX+30, iY, 100, 30, 0, false, &pCombo );
	{
		pCombo->SetDropHeight( 40 );
		pCombo->AddItem( L"RED", (LPVOID) 0 );
		pCombo->AddItem( L"BLUE", (LPVOID) 0 );
		pCombo->AddItem( L"NONE", (LPVOID) 0 );
		pCombo->AddItem( L"DungeonUser", (LPVOID) 0 );
	}
	MainUI.AddComboBox( IDC_COMBOBOX_START_POS_DIRECTION, iX+130, iY, 90, 30, 0, false, &pCombo );
	{
		pCombo->SetDropHeight( 30 );
		pCombo->AddItem( L"LEFT", (LPVOID) 0 );
		pCombo->AddItem( L"RIGHT", (LPVOID) 0 );
	}
	iX = 10;
	iY += 35;
	MainUI.AddStatic( IDC_STATIC_START_POS, L"번호  O/X  linetype  potal  stop  dustname  <-  ->", iX, iY, 320, 22);
	iY += 25;	
	MainUI.AddListBox( IDC_LISTBOX_SEGMENT_INFO, iX, iY, 320, 100, 0 );
	MainUI.GetListBox( IDC_LISTBOX_SEGMENT_INFO )->SetStyle( CDXUTListBox::MULTISELECTION );
	iX = 110;
	iY += 110;
	int iTempY = iY;
	
	MainUI.AddStatic( IDC_STATIC_DUMMY, L"line num: ", iX, iY, 100, 30);
	MainUI.AddEditBox( IDC_EDIT_SEGMENT_INDEX, L"", iX+iDX, iY, 120, 30);
	
	iY += 32;
	MainUI.AddStatic( IDC_STATIC_DUMMY, L"enable: ", iX, iY, 100, 30);
	MainUI.AddComboBox( IDC_COMBOBOX_SEGMENT_ENABLE, iX+iDX, iY, 120, 30, 0, false, &pCombo );
	{
		pCombo->SetDropHeight( 30 );
		pCombo->AddItem( L"ENABLE", (LPVOID) 0 );
		pCombo->AddItem( L"DISABLE", (LPVOID) 0 );
	}



	iY += 32;
	MainUI.AddStatic( IDC_STATIC_DUMMY, L"line type: ", iX, iY, 100, 30);
	MainUI.AddComboBox( IDC_COMBOBOX_SEGMENT_LINE_TYPE, iX+iDX, iY, 120, 30, 0, false, &pCombo );
	{
		pCombo->SetDropHeight( 100 );
		pCombo->AddItem( L"NORMAL",				(LPVOID) (int) CKTDGLineMap::LT_NORMAL			);
		pCombo->AddItem( L"WALL",				(LPVOID) (int) CKTDGLineMap::LT_WALL			);
		pCombo->AddItem( L"POTAL",				(LPVOID) (int) CKTDGLineMap::LT_POTAL			);
		pCombo->AddItem( L"HOLE",				(LPVOID) (int) CKTDGLineMap::LT_HOLE			);
		pCombo->AddItem( L"JUMP_UP_REL",		(LPVOID) (int) CKTDGLineMap::LT_JUMP_UP_REL		);
		pCombo->AddItem( L"JUMP_UP_ABS",		(LPVOID) (int) CKTDGLineMap::LT_JUMP_UP_ABS		);
		pCombo->AddItem( L"BUNGEE",				(LPVOID) (int) CKTDGLineMap::LT_BUNGEE			);
		pCombo->AddItem( L"MONSTER_ROAD",		(LPVOID) (int) CKTDGLineMap::LT_MONSTER_ROAD	);
		pCombo->AddItem( L"UNIT_ROAD",			(LPVOID) (int) CKTDGLineMap::LT_UNIT_ROAD		);
	}

	iY += 32;
	MainUI.AddStatic( IDC_STATIC_DUMMY, L"dust type: ", iX, iY, 100, 30);
	MainUI.AddComboBox( IDC_COMBOBOX_LINE_DUST_TYPE, iX+iDX, iY, 120, 30, 0, false, &pCombo );
	{
		pCombo->SetDropHeight( 40 );
		pCombo->AddItem( L"DEFALT_DUST", (LPVOID) (int) CKTDGLineMap::LDT_DEFALT_DUST );
		pCombo->AddItem( L"WATER_SPLASH", (LPVOID) (int) CKTDGLineMap::LDT_WATER_SPLASH );
	}

	iY += 32;
	MainUI.AddStatic( IDC_STATIC_DUMMY, L"portal group: ", iX, iY, 100, 30);
	MainUI.AddEditBox( IDC_EDIT_SEGMENT_PORTAL_GROUP, L"", iX+iDX, iY, 120, 30);
	iY += 32;
	MainUI.AddStatic( IDC_STATIC_DUMMY, L"stop factor: ", iX, iY, 100, 30);
	MainUI.AddEditBox( IDC_EDIT_SEGMENT_STOP_FACTOR, L"", iX+iDX, iY, 120, 30);
	iY += 32;

	iX = -150;
	iY = iTempY;
	MainUI.AddStatic( IDC_STATIC_DUMMY, L"before line: ", iX, iY, 100, 30);
	MainUI.AddComboBox( IDC_COMBOBOX_SEGMENT_BEFORE_LINE, iX+iDX, iY, 120, 30, 0, false, &pCombo );
	{
		pCombo->SetDropHeight( 40 );	
	}
	iY += 32;
	MainUI.AddStatic( IDC_STATIC_DUMMY, L"next line: ", iX, iY, 100, 30);
	MainUI.AddComboBox( IDC_COMBOBOX_SEGMENT_NEXT_LINE, iX+iDX, iY, 120, 30, 0, false, &pCombo );
	{
		pCombo->SetDropHeight( 40 );
	}


	iY += 32;
	MainUI.AddStatic( IDC_STATIC_DUMMY, L"line set: ", iX, iY, 100, 30);
	MainUI.AddEditBox( IDC_EDIT_LINE_SET_INDEX, L"", iX+iDX, iY, 120, 30 );

	iY += 32;
	MainUI.AddStatic( IDC_STATIC_DUMMY, L"speed: ", iX, iY, 100, 30);
	MainUI.AddEditBox( IDC_EDIT_LINE_SPEED, L"", iX+iDX, iY, 120, 30 );


	iY += 32;
	MainUI.AddStatic( IDC_STATIC_DUMMY, L"start pos: ", iX-100, iY, 100, 30);
	MainUI.AddEditBox( IDC_EDIT_LINE_START_POS, L"", iX+iDX-100, iY, 120+100, 30 );
	iY += 32;
	MainUI.AddStatic( IDC_STATIC_DUMMY, L"end pos: ", iX-100, iY, 100, 30);
	MainUI.AddEditBox( IDC_EDIT_LINE_END_POS, L"", iX+iDX-100, iY, 120+100, 30 );




	//////////////////////////////////////////////////////////////////////////
	iX = -450;
	iY = iTempY;
	MainUI.AddStatic( IDC_STATIC_DUMMY, L"canDown: ", iX, iY, 100, 30);
	MainUI.AddComboBox( IDC_COMBOBOX_CAN_DOWN, iX+iDX, iY, 120, 30, 0, false, &pCombo );
	{
		pCombo->SetDropHeight( 30 );
		pCombo->AddItem( L"TRUE", (LPVOID) 0 );
		pCombo->AddItem( L"FALSE", (LPVOID) 1 );
	}

	iY += 32;
	MainUI.AddStatic( IDC_STATIC_DUMMY, L"canPassUp: ", iX, iY, 100, 30);
	MainUI.AddComboBox( IDC_COMBOBOX_CAN_PASS, iX+iDX, iY, 120, 30, 0, false, &pCombo );
	{
		pCombo->SetDropHeight( 30 );
		pCombo->AddItem( L"TRUE", (LPVOID) 0 );
		pCombo->AddItem( L"FALSE", (LPVOID) 1 );
	}

	iY += 32;
	MainUI.AddStatic( IDC_STATIC_DUMMY, L"monsterAllowed: ", iX, iY, 100, 30);
	MainUI.AddComboBox( IDC_COMBOBOX_MONSTER_ALLOWED, iX+iDX, iY, 120, 30, 0, false, &pCombo );
	{
		pCombo->SetDropHeight( 30 );
		pCombo->AddItem( L"TRUE", (LPVOID) 0 );
		pCombo->AddItem( L"FALSE", (LPVOID) 1 );
	}


	iY += 32;
	MainUI.AddStatic( IDC_STATIC_DUMMY, L"wallJump: ", iX, iY, 100, 30);
	MainUI.AddComboBox( IDC_COMBOBOX_ENABLE_WALL_JUMP, iX+iDX, iY, 120, 30, 0, false, &pCombo );
	{
		pCombo->SetDropHeight( 30 );
		pCombo->AddItem( L"TRUE", (LPVOID) 0 );
		pCombo->AddItem( L"FALSE", (LPVOID) 1 );
	}

	iY += 32;
	MainUI.AddStatic( IDC_STATIC_DUMMY, L"teleportLine: ", iX, iY, 125, 22 );
	MainUI.AddEditBox( IDC_EDIT_TELEPORT_LINE_INDEX, L"-1", iX+iDX, iY, 80, 30 );

	iY += 32;
	MainUI.AddStatic( IDC_STATIC_DUMMY, L"jumpSpeed: ", iX-100, iY, 100, 30);
	MainUI.AddEditBox( IDC_EDIT_JUMP_SPEED, L"", iX+iDX-100, iY, 120+100, 30 );





	//////////////////////////////////////////////////////////////////////////
	m_DungeonUI.Init( &g_DialogResourceManager );
	m_DungeonUI.SetCallback( OnLineMapToolUIEvent2, this );
	m_DungeonUI.SetSize( 340, 300 );
	m_DungeonUI.SetFont(0, L"굴림", 12, 12);

	CDXUTEditBox* pEdit = NULL;
	CDXUTCheckBox* pCheckBox = NULL;

	iX = 10;
	iY = 10;
	int iWidth = 220;
	int iHeight = 30;



	m_DungeonUI.AddCheckBox( IDC_CHECK_SHOW_START_POS_SIMPLE, L"Hide Start Pos mesh", iX, iY, 160, 22, false, 0, 0, &pCheckBox );
	pCheckBox->SetChecked( m_bHideStartPosMesh );
	iY += 32;
	
	m_DungeonUI.AddButton( IDC_BUTTON_SHOW_USER_UNIT_START_POS, L"HIDE USER UNIT", iX, iY, iWidth, iHeight);
	iY += 32;
	m_DungeonUI.AddButton( IDC_BUTTON_MOUSE_CLICK_TARGET, L"PICK LINE", iX, iY, iWidth, iHeight);
	iY += 32;
	m_DungeonUI.AddButton( IDC_BUTTON_USE_NPC_MESH, L"USING ELSWORD MESH", iX, iY, iWidth, iHeight);
	iY += 32;
	m_DungeonUI.AddButton( IDC_BUTTON_LOAD_NPC_FROM_DUNGEON_SCRIPT, L"LOAD DUNGEON SCRIPT", iX, iY, iWidth, iHeight);
	iY += 32;
	m_DungeonUI.AddComboBox( IDC_COMBOBOX_DUNGEON_DIFFICULTY, iX, iY, iWidth, iHeight, 0, false, &pCombo );
	{
		pCombo->SetDropHeight( 40 );
		pCombo->AddItem( L"ALL",		(LPVOID) NULL );
		pCombo->AddItem( L"NORMAL",		(LPVOID) NULL );
		pCombo->AddItem( L"HARD",		(LPVOID) NULL );
		pCombo->AddItem( L"EXPERT",		(LPVOID) NULL );
	}

	iY += 32;
	m_DungeonUI.AddStatic( IDC_STATIC_DUMMY, L"MOVE ALL START POS", iX, iY, iWidth, iHeight );
	iY += 32;
	m_DungeonUI.AddEditBox( IDC_EDIT_ALL_NPC_START_POS_DISPLACE, L"", iX, iY, iWidth, iHeight, false, &pEdit );
	pEdit->SetText( L"0.0    0.0    0.0" );
	iY += 32;
	m_DungeonUI.AddStatic( IDC_STATIC_DUMMY, L"MOVE ALL LINE POS", iX, iY, iWidth, iHeight );
	iY += 32;
	m_DungeonUI.AddEditBox( IDC_EDIT_ALL_LINE_POS_DISPLACE, L"", iX, iY, iWidth, iHeight, false, &pEdit );
	pEdit->SetText( L"0.0    0.0    0.0" );
	iY += 32;

	m_DungeonUI.AddListBox( IDC_LIST_ANY_INFO, iX-iWidth*2-20, 30, iWidth*2, iHeight*8 ); 

    m_pBox[0] = NULL;
}



CX2LineMapToolMain::~CX2LineMapToolMain(void)
{
	SAFE_DELETE( m_pMatrix );

	SAFE_DELETE( g_pData );

	//SAFE_DELETE(pLineMap);
	SAFE_DELETE_KTDGOBJECT( pLineMap );

	SAFE_CLOSE( m_pXSkinMesh );
	SAFE_CLOSE(m_pXMeshCube);
	SAFE_RELEASE( m_pVBHighlightedSegment );

	DeleteAllSkinMeshAndSkinAnim();
	
	
	for( int i=0; i<12 ; i++ )
	{
		SAFE_RELEASE( m_pMesh3DText[i] );
		m_pMesh3DText[i] = NULL;
	}

    if(m_pBox[0] != NULL)
        m_pBox[0]->Release();
}

CX2LineMapToolMain::CX2LineMapToolMain(void)
{
	
	char szWindowTitle[1024];
	sprintf( szWindowTitle, "X2LineMapTool; compiled at %s, %s", __DATE__, __TIME__ );
	SetWindowTextA( g_pKTDXApp->GetHWND(), szWindowTitle );

	m_CurrKeyUsed = 0;
	for( int i=0; i<sizeof(m_abKeyUsed); i++ )
	{
		m_abKeyUsed[i] = false;
	}

	m_bShowUserUnitStartPos = true;
	m_bUseNPCMesh = false;


	m_pNormalDungeon	= NULL;
	m_pHardDungeon	= NULL;
	m_pExpertDungeon = NULL;

	m_pNormalStageData = NULL;
	m_pHardStageData = NULL;
	m_pExpertStageData = NULL;


	m_pMatrix = new CKTDGMatrix( g_pKTDXApp->GetDevice() );



	m_MouseClickTarget = MCT_LINE;

	m_bEnableUI = true;
	m_bMouseDragging = false;
	m_f3DTextAngle = 0.f;
	m_f3DTextAngleStep = 0.2f;

	m_coHighlight = D3DXCOLOR(1,1,1,1);

	m_iPickedStartPosIndex = -1;
	m_veciPickedSegmentIndex.clear();
	bNavigate = true;
	iTeamColor = TEAM_RED;
	iInitialDir = DIR_LEFT;

	pLineMap = NULL;
	m_wstrLineMapFileName = L"";
	StringCchPrintf( wszTempFilePath, sizeof(wszTempFilePath), L"C:\\temp_line_map_lua.lua" );

	m_bHideStartPosMesh = false;

	m_pXSkinMesh = NULL;
	m_pXMeshCube = NULL;

	m_pXSkinMesh = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( L"StartPos_Elsword.x" );
	if( NULL == m_pXSkinMesh )
		MessageBox( g_pKTDXApp->GetHWND(), L"StartPos_Elsword.x 파일이 없습니다.", L"죄송^^", MB_OK );

	m_pXMeshCube = g_pKTDXApp->GetDeviceManager()->OpenXMesh( L"sphere0.x" );
	if( NULL == m_pXMeshCube )
		MessageBox( g_pKTDXApp->GetHWND(), L"sphere0.x 파일이 없습니다.", L"죄송^^", MB_OK );

	
	// ready 3d text font 
	WCHAR wszTemp[32];
	for( int i=0; i<10; i++ )
	{
		m_pMesh3DText[i] = NULL;		
		StringCchPrintf( wszTemp, sizeof(wszTemp), L"%d", i );
		CreateD3DXTextMesh( g_pKTDXApp->GetDevice(), &m_pMesh3DText[i], L"Arial", 12, FALSE, FALSE, wszTemp );			
	}

	m_pMesh3DText[10] = NULL;
	swprintf( wszTemp, sizeof(wszTemp)/sizeof(WCHAR)-1, L"%s", L"-" );
	CreateD3DXTextMesh( g_pKTDXApp->GetDevice(), &m_pMesh3DText[10], L"Arial", 12, FALSE, FALSE, wszTemp );

	m_pMesh3DText[11] = NULL;
	swprintf( wszTemp, sizeof(wszTemp)/sizeof(WCHAR)-1, L"%s", L"*" );
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

	
	
	m_iShowDungeonLevel = 0;
	
	RegisterLuabind();
	OpenScriptFile( L"Enum.lua" );
	OpenScriptFile( L"Config.lua" );

	if( NULL == g_pData )
		g_pData = new CX2Data;

	InitX2Data();
	InitUI();	
	
}

HRESULT CX2LineMapToolMain::OnFrameMove( double fTime, float fElapsedTime )
{
	HRESULT hr;
	g_pKTDXApp->GetDGManager()->GetDialogManager()->OnFrameMove( fTime, fElapsedTime );

	const float MAGIC_CAMERA_SPEED = 600.f;
	m_FPSCamera.FrameMove( fElapsedTime * MAGIC_CAMERA_SPEED );
	g_pKTDXApp->GetDGManager()->GetCamera().Move( m_FPSCamera.GetEyePt()->x, m_FPSCamera.GetEyePt()->y, m_FPSCamera.GetEyePt()->z );
	g_pKTDXApp->GetDGManager()->GetCamera().LookAt( m_FPSCamera.GetLookAtPt()->x, m_FPSCamera.GetLookAtPt()->y, m_FPSCamera.GetLookAtPt()->z );
	g_pKTDXApp->GetDGManager()->GetCamera().UpdateCamera( fElapsedTime );

	// start pos 개수 만큼 skinanim 개수를 맞추어주자
	if( vecpXSkinAnim.size() < vecTeamStartPos.size() )
	{
		for( unsigned i=vecpXSkinAnim.size(); i<vecTeamStartPos.size(); i++ )
		{
			//CKTDGXSkinAnim* pXSkinAnim = new CKTDGXSkinAnim( g_pKTDXApp->GetDevice() );
			CKTDGXSkinAnim* pXSkinAnim = CKTDGXSkinAnim::CreateSkinAnim();

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
		hr = vecpXSkinAnim[i]->OnFrameMove(fTime, fElapsedTime);
	}

	map< std::pair<CX2DungeonSubStage::NPCData*, int>, CKTDGXSkinAnim* >::iterator it;
	for( it=m_mapNPCXSkinAnim.begin(); it!= m_mapNPCXSkinAnim.end(); it++ )
	{
		CKTDGXSkinAnim* pCKTDGXSkinAnim = (CKTDGXSkinAnim*)it->second;
		pCKTDGXSkinAnim->OnFrameMove( fTime, fElapsedTime );
	}

	return S_OK;
}



HRESULT CX2LineMapToolMain::OnFrameRender()
{
	HRESULT hr = S_OK;

	if( pLineMap != NULL )
	{
		CKTDGStateManager::PushTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
		CKTDGStateManager::PushTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE );
		CKTDGStateManager::PushTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_CURRENT );

		pLineMap->OnFrameRender();

		CKTDGStateManager::PopTextureStageState( 0, D3DTSS_COLORARG2 );
		CKTDGStateManager::PopTextureStageState( 0, D3DTSS_COLORARG1 );
		CKTDGStateManager::PopTextureStageState( 0, D3DTSS_COLOROP );
	}

	int i;
	CKTDGLineMap::LineData* pLineData = NULL;	
	CKTDGMatrix matTemp( g_pKTDXApp->GetDevice() );
	
	//CKTDGXRenderer::RenderParam* pRenderParam = g_pKTDXApp->GetDGManager()->GetXRenderer()->GetRenderParam();
	CKTDGXRenderer::RenderParam renderParam;
	D3DXMATRIX worldMatrix;

	// render line end point
	if( pLineMap != NULL && m_pXMeshCube != NULL )
	{
		float fIndicatorSize = 10.f;
		matTemp.Scale( fIndicatorSize, fIndicatorSize, fIndicatorSize );

		for( i=0; i<pLineMap->GetNumLineData(); i++ )
		{
			pLineData = pLineMap->GetLineData(i);
			matTemp.Move( pLineData->startPos );	
			
			//pRenderParam->worldMatrix = matTemp.GetMatrix();
			worldMatrix = matTemp.GetMatrix();

			if( pLineData->beforeLine == -1 )
			{
				//pRenderParam->color	= D3DXCOLOR(0,0,1,1); 
				renderParam.color		= D3DXCOLOR(0,0,1,1);
				//g_pKTDXApp->GetDGManager()->GetXRenderer()->BeginRender();
				//g_pKTDXApp->GetDGManager()->GetXRenderer()->OnFrameRender( m_pXMeshCube );
				//g_pKTDXApp->GetDGManager()->GetXRenderer()->EndRender();
				g_pKTDXApp->GetDGManager()->GetXRenderer()->OnFrameRender( renderParam, worldMatrix, *m_pXMeshCube );
			}
			else if( pLineData->beforeLine == -2 )
			{
				//pRenderParam->color	= D3DXCOLOR(1,0,0,1);
				renderParam.color		= D3DXCOLOR(1,0,0,1);
				//g_pKTDXApp->GetDGManager()->GetXRenderer()->BeginRender();
				//g_pKTDXApp->GetDGManager()->GetXRenderer()->OnFrameRender( m_pXMeshCube );
				//g_pKTDXApp->GetDGManager()->GetXRenderer()->EndRender();
				g_pKTDXApp->GetDGManager()->GetXRenderer()->OnFrameRender( renderParam, worldMatrix, *m_pXMeshCube );
			}

			matTemp.Move( pLineData->endPos );
			//pRenderParam->worldMatrix = matTemp.GetMatrix();
			worldMatrix = matTemp.GetMatrix();

			if( pLineData->nextLine == -1 )
			{
				//pRenderParam->color	= D3DXCOLOR(0,0,1,1); 
				renderParam.color		= D3DXCOLOR(0,0,1,1);
				//g_pKTDXApp->GetDGManager()->GetXRenderer()->BeginRender();
				//g_pKTDXApp->GetDGManager()->GetXRenderer()->OnFrameRender( m_pXMeshCube );
				//g_pKTDXApp->GetDGManager()->GetXRenderer()->EndRender();
				g_pKTDXApp->GetDGManager()->GetXRenderer()->OnFrameRender( renderParam, worldMatrix, *m_pXMeshCube );
			}
			else if( pLineData->nextLine == -2 )
			{
				//pRenderParam->color	= D3DXCOLOR(1,0,0,1);
				renderParam.color		= D3DXCOLOR(1,0,0,1);
				//g_pKTDXApp->GetDGManager()->GetXRenderer()->BeginRender();
				//g_pKTDXApp->GetDGManager()->GetXRenderer()->OnFrameRender( m_pXMeshCube );
				//g_pKTDXApp->GetDGManager()->GetXRenderer()->EndRender();
				g_pKTDXApp->GetDGManager()->GetXRenderer()->OnFrameRender( renderParam, worldMatrix, *m_pXMeshCube );
			}
		}
	}

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


	if( m_f3DTextAngle > 25.f )
		m_f3DTextAngleStep = -.25f;
	else if( m_f3DTextAngle < -25.f )
		m_f3DTextAngleStep = .25f;
	m_f3DTextAngle += m_f3DTextAngleStep;

	int iRedTeamStartIndex		= -1;
	int iBlueTeamStartIndex		= -1;
	int iDungeonStartIndex		= -1;
	int iStartIndex				= -1;
	int iNumber					= 0; // 화면에 출력되는 start pos index


	if( true == m_bShowUserUnitStartPos )
	{
		RenderUserUnitStartPos();	
	}

	if( m_iShowDungeonLevel == 0 )
	{
		RenderNPCUnitStartPostion( CX2Dungeon::DL_NORMAL );
		RenderNPCUnitStartPostion( CX2Dungeon::DL_HARD );
		RenderNPCUnitStartPostion( CX2Dungeon::DL_EXPERT );
	}
	else if( m_iShowDungeonLevel == 1 )
	{
		RenderNPCUnitStartPostion( CX2Dungeon::DL_NORMAL );
	}
	else if( m_iShowDungeonLevel == 2 )
	{
		RenderNPCUnitStartPostion( CX2Dungeon::DL_HARD );
	}
	else if( m_iShowDungeonLevel == 3 )
	{
		RenderNPCUnitStartPostion( CX2Dungeon::DL_EXPERT );
	}

    if(m_bTileRender == true)
    {
        OnRenderTile();
    }

	if( true == m_bEnableUI )
	{
		MainUI.OnRender( g_pKTDXApp->GetElapsedTime() );
		m_DungeonUI.OnRender( g_pKTDXApp->GetElapsedTime() );
	}

	return hr;
}

void CX2LineMapToolMain::RenderUserUnitStartPos()
{
	CKTDGMatrix matTemp( g_pKTDXApp->GetDevice() );
	//CKTDGXRenderer::RenderParam* pRenderParam = g_pKTDXApp->GetDGManager()->GetXRenderer()->GetRenderParam();
	CKTDGXRenderer::RenderParam renderParam;
	D3DXMATRIX worldMatrix;

	// render start pos
	for( unsigned i=0; i<vecTeamStartPos.size(); i++ )
	{	
		if( false == m_bHideStartPosMesh )
		{
			vecpXSkinAnim[i]->GetMatrix().Move( vecTeamStartPos[i].vPos );

			if( vecTeamStartPos[i].iInitialDir == DIR_LEFT )
				vecpXSkinAnim[i]->GetMatrix().RotateDegree( 0, 180, 0 );
			else
				vecpXSkinAnim[i]->GetMatrix().RotateDegree( 0, 0, 0 );

			if( vecTeamStartPos[i].iTeamColor == TEAM_RED )
				vecpXSkinAnim[i]->GetRenderParam()->color = D3DXCOLOR( 1, 0, 0, 1 );
			else if( vecTeamStartPos[i].iTeamColor == TEAM_BLUE )
				vecpXSkinAnim[i]->GetRenderParam()->color = D3DXCOLOR( 0, 0, 1, 1 );
			else if( vecTeamStartPos[i].iTeamColor == TEAM_NONE )
				continue;

			vecpXSkinAnim[i]->OnFrameRender();
		}
		else
		{
			if( vecTeamStartPos[i].iTeamColor == TEAM_RED )
				//pRenderParam->color	= D3DXCOLOR(0.5f, 0, 0, 1 );
				renderParam.color		= D3DXCOLOR(0.5f, 0, 0, 1 );
			else if( vecTeamStartPos[i].iTeamColor == TEAM_BLUE )
				//pRenderParam->color	= D3DXCOLOR(0, 0, 0.5f, 1 );
				renderParam.color		= D3DXCOLOR(0, 0, 0.5f, 1 );
			else if( vecTeamStartPos[i].iTeamColor == TEAM_NONE )
				continue;

			const float MAGIC_DOT_SIZE = 5.f;

			matTemp.Move( vecTeamStartPos[i].vPos );
			matTemp.Scale( MAGIC_DOT_SIZE, MAGIC_DOT_SIZE, MAGIC_DOT_SIZE );
			
			//pRenderParam->worldMatrix = matTemp.GetMatrix();
			worldMatrix = matTemp.GetMatrix();

			//g_pKTDXApp->GetDGManager()->GetXRenderer()->BeginRender();
			//g_pKTDXApp->GetDGManager()->GetXRenderer()->OnFrameRender( m_pXMeshCube );
			//g_pKTDXApp->GetDGManager()->GetXRenderer()->EndRender();
			g_pKTDXApp->GetDGManager()->GetXRenderer()->OnFrameRender( renderParam, worldMatrix, *m_pXMeshCube );
		}

	}

	if( m_f3DTextAngle > 25.f )
		m_f3DTextAngleStep = -.25f;
	else if( m_f3DTextAngle < -25.f )
		m_f3DTextAngleStep = .25f;
	m_f3DTextAngle += m_f3DTextAngleStep;

	int iRedTeamStartIndex		= -1;
	int iBlueTeamStartIndex		= -1;
	int iDungeonStartIndex		= -1;
	int iStartIndex				= -1;
	int iNumber					= 0; // 화면에 출력되는 start pos index

	CKTDGStateManager::PushTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
	CKTDGStateManager::PushTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE );
	CKTDGStateManager::PushTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_CURRENT );
	for( unsigned i=0; i<vecTeamStartPos.size(); i++ )
	{
		matTemp.Move( vecTeamStartPos[i].vPos );
		matTemp.MoveRel( 0, 160, 0 );
		matTemp.Scale( 100, 100, 100 );
		//matTemp.RotateDegree( 0, m_f3DTextAngle, 0 );

		int iLineIndex = vecTeamStartPos[i].iLineIndex;
		CKTDGLineMap::LineData* pLineData = pLineMap->GetLineData( iLineIndex );
		D3DXVECTOR3 vDir = pLineData->dirVector;
		vDir.y = 0.f;
		D3DXVec3Normalize( &vDir, &vDir );
		D3DXVECTOR3 vXAxis(1,0,0);

		float fDotXAxis = D3DXVec3Dot( &vDir, &vXAxis );
		float fRad = acosf( fDotXAxis );
		if( vDir.z < 0.f )
		{
			fRad = -fRad;
		}
		matTemp.Rotate( 0.f, -fRad, 0.f );



		D3DMATERIAL9 mtrl;
		ZeroMemory( &mtrl, sizeof(D3DMATERIAL9) );

		if( vecTeamStartPos[i].iTeamColor == TEAM_RED )
		{
			iRedTeamStartIndex++;
			iNumber = iRedTeamStartIndex;
			D3DXCOLOR coTeamColor = D3DXCOLOR( 1, 0, 0, 1 );
			memcpy( &mtrl.Diffuse, &coTeamColor, sizeof(D3DCOLORVALUE) );
			memcpy( &mtrl.Ambient, &coTeamColor, sizeof(D3DCOLORVALUE) );
		}
		else if( vecTeamStartPos[i].iTeamColor == TEAM_BLUE )
		{
			iBlueTeamStartIndex++;
			iNumber = iBlueTeamStartIndex; 			
			D3DXCOLOR coTeamColor = D3DXCOLOR( 0, 0, 1, 1 );
			memcpy( &mtrl.Diffuse, &coTeamColor, sizeof(D3DCOLORVALUE) );
			memcpy( &mtrl.Ambient, &coTeamColor, sizeof(D3DCOLORVALUE) );
		}
		else
			continue;

		g_pKTDXApp->GetDevice()->SetMaterial( &mtrl );			

		int iDigit;
		while( iNumber >= 0 )
		{
			iDigit = iNumber%10;

			matTemp.UpdateWorldMatrix();			
			m_pMesh3DText[iDigit]->DrawSubset(0);

			//matTemp.MoveRel( -50, 0, 0 );
			matTemp.MoveRel( -50.f * vDir );
			iNumber = iNumber/10;
			if( iNumber == 0 )
				break;
		}		
	}
	CKTDGStateManager::PopTextureStageState( 0, D3DTSS_COLORARG2 );
	CKTDGStateManager::PopTextureStageState( 0, D3DTSS_COLORARG1 );
	CKTDGStateManager::PopTextureStageState( 0, D3DTSS_COLOROP );
}





void CX2LineMapToolMain::RenderNPCStartPosIndex( CX2DungeonSubStage::NPCData* pNPCData, int iStageNPCIndex /*= -1*/, int iNPCSubIndex /*= -1*/ )
{
	CKTDGMatrix matTemp( g_pKTDXApp->GetDevice() );

	if( false == m_bUseNPCMesh )
	{
		int iNumber					= 0; // 화면에 출력되는 start pos index

		CKTDGStateManager::PushTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
		CKTDGStateManager::PushTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE );
		CKTDGStateManager::PushTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_CURRENT );
		for( unsigned i=0; i<vecTeamStartPos.size(); i++ )
		{
			matTemp.Move( vecTeamStartPos[i].vPos );
			matTemp.MoveRel( 0, 160, 0 );
			matTemp.Scale( 100, 100, 100 );

			int iLineIndex = vecTeamStartPos[i].iLineIndex;
			CKTDGLineMap::LineData* pLineData = pLineMap->GetLineData( iLineIndex );
			D3DXVECTOR3 vDir = pLineData->dirVector;
			vDir.y = 0.f;
			D3DXVec3Normalize( &vDir, &vDir );
			D3DXVECTOR3 vXAxis(1,0,0);

			float fDotXAxis = D3DXVec3Dot( &vDir, &vXAxis );
			float fRad = acosf( fDotXAxis );
			if( vDir.z < 0.f )
			{
				fRad = -fRad;
			}
			matTemp.Rotate( 0.f, -fRad, 0.f );



			D3DMATERIAL9 mtrl;
			ZeroMemory( &mtrl, sizeof(D3DMATERIAL9) );

			if( vecTeamStartPos[i].iTeamColor == TEAM_NONE )
			{
				D3DXCOLOR coTeamColor = D3DXCOLOR( 0.5, 0.5, 0, 1 );
				memcpy( &mtrl.Diffuse, &coTeamColor, sizeof(D3DCOLORVALUE) );
				memcpy( &mtrl.Ambient, &coTeamColor, sizeof(D3DCOLORVALUE) );
			}
			else
				continue;

			g_pKTDXApp->GetDevice()->SetMaterial( &mtrl );

			iNumber = vecTeamStartPos[i].key;
			int iDigit;
			while( iNumber >= 0 )
			{
				iDigit = iNumber%10;

				matTemp.UpdateWorldMatrix();			
				m_pMesh3DText[iDigit]->DrawSubset(0);

				matTemp.MoveRel( -50.f * vDir );
				iNumber = iNumber/10;
				if( iNumber == 0 )
					break;
			}		
		}
		CKTDGStateManager::PopTextureStageState( 0, D3DTSS_COLORARG2 );
		CKTDGStateManager::PopTextureStageState( 0, D3DTSS_COLORARG1 );
		CKTDGStateManager::PopTextureStageState( 0, D3DTSS_COLOROP );
	}
	else
	{
		if( NULL != pNPCData )
		{
			int inpcstartindex = pNPCData->m_vecStartPos[iNPCSubIndex];
			D3DXVECTOR3 vPos = GetTeamStartPos( inpcstartindex )->vPos;
			CKTDGStateManager::PushTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
			CKTDGStateManager::PushTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE );
			CKTDGStateManager::PushTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_CURRENT );

			matTemp.Move( vPos );
			matTemp.MoveRel( 0, 200, 0 );
			matTemp.Scale( 50, 50, 50 );

			int iLineIndex = pLineMap->GetStartLineIndex( pNPCData->m_vecStartPos[iNPCSubIndex] );
			CKTDGLineMap::LineData* pLineData = pLineMap->GetLineData( iLineIndex );
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




			for( UINT i = 0; i<m_vecPickedNPCStartPosKey.size(); i++ )
			{
				SNPCStartPos& npcStart = m_vecPickedNPCStartPosKey[i];
				if( npcStart.m_pNPCData == pNPCData )
				{
					// '*'
					matTemp.Move( vPos );
					matTemp.MoveRel( 0, 200+60, 0 );
					matTemp.Scale( 300, 300, 300 );

					matTemp.RotateDegree( m_f3DTextAngle * 5.f, m_f3DTextAngle * 25.f, m_f3DTextAngle * 15.f );
					
					matTemp.UpdateWorldMatrix();
					m_pMesh3DText[11]->DrawSubset(0);

					break;
				}
			}

			CKTDGStateManager::PopTextureStageState( 0, D3DTSS_COLORARG2 );
			CKTDGStateManager::PopTextureStageState( 0, D3DTSS_COLORARG1 );
			CKTDGStateManager::PopTextureStageState( 0, D3DTSS_COLOROP );
		}

	}
}


void CX2LineMapToolMain::RenderNPCUnitStartPostion( CX2Dungeon::DIFFICULTY_LEVEL dungeonDifficulty )
{
	CKTDGMatrix matTemp( g_pKTDXApp->GetDevice() );
	//CKTDGXRenderer::RenderParam* pRenderParam = g_pKTDXApp->GetDGManager()->GetXRenderer()->GetRenderParam();
	CKTDGXRenderer::RenderParam renderParam;
	

	if( false == m_bHideStartPosMesh )
	{
		if( false == m_bUseNPCMesh )
		{
			// render start pos
			for( unsigned i=0; i<vecTeamStartPos.size(); i++ )
			{	
				vecpXSkinAnim[i]->GetMatrix().Move( vecTeamStartPos[i].vPos );

				if( vecTeamStartPos[i].iInitialDir == DIR_LEFT )
					vecpXSkinAnim[i]->GetMatrix().RotateDegree( 0, 180, 0 );
				else
					vecpXSkinAnim[i]->GetMatrix().RotateDegree( 0, 0, 0 );

				if( vecTeamStartPos[i].iTeamColor == TEAM_NONE )
				{
					vecpXSkinAnim[i]->GetRenderParam()->color = D3DXCOLOR( 0, 1, 0, 1 );
					vecpXSkinAnim[i]->OnFrameRender();
				}		
			}

			RenderNPCStartPosIndex( NULL );
		}
		else
		{
			CKTDGMatrix matTemp( g_pKTDXApp->GetDevice() );
			//CKTDGXRenderer::RenderParam* pRenderParam = g_pKTDXApp->GetDGManager()->GetXRenderer()->GetRenderParam();
			CKTDGXRenderer::RenderParam renderParam;

			vector< CX2DungeonSubStage::SubStageData* >* pvecSubStageData = NULL;
			switch( dungeonDifficulty )
			{
			case CX2Dungeon::DL_NORMAL:
				if( NULL != m_pNormalStageData )
					pvecSubStageData = &m_pNormalStageData->m_SubStageDataList;
				break;
			case CX2Dungeon::DL_HARD:
				if( NULL != m_pHardStageData )
					pvecSubStageData = &m_pHardStageData->m_SubStageDataList;
				break;
			case CX2Dungeon::DL_EXPERT:
				if( NULL != m_pExpertStageData )
					pvecSubStageData = &m_pExpertStageData->m_SubStageDataList;
				break;
			}

			if( pvecSubStageData == NULL )
				return;

			for( UINT i=0; i<pvecSubStageData->size(); i++ )
			{
				vector<CX2DungeonSubStage::NPCData*>& npcDataList = pvecSubStageData->at(i)->m_NPCDataList;

				for( UINT j=0; j<npcDataList.size(); j++ )
				{
					CX2DungeonSubStage::NPCData* pNPCData = npcDataList.at(j);

					for( UINT k=0; k<pNPCData->m_vecStartPos.size(); k++ )
					{
						//CKTDGXSkinAnim* pNPCSkinAnim = m_mapNPCXSkinAnim[pNPCData];				
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

						pNPCSkinAnim->GetMatrix().Move( vPos );
						pNPCSkinAnim->GetMatrix().MoveRel( pNPCData->m_AddPos );

						if( true == bIsRight )
							pNPCSkinAnim->GetMatrix().RotateDegree( 0, 0, 0 );					
						else
							pNPCSkinAnim->GetMatrix().RotateDegree( 0, 180, 0 );

						if( k == 0 )
						{
							pNPCSkinAnim->GetRenderParam()->color = D3DXCOLOR( 0, 1, 0, 1 );
						}
						else
						{
							pNPCSkinAnim->GetRenderParam()->color = D3DXCOLOR( 1, 1, (float)k*0.02f, 1 );
						}

						pNPCSkinAnim->OnFrameRender();

						RenderNPCStartPosIndex( pNPCData, j, k );	
					}

				}
			}
		}

	}
	else
	{
		// render start pos
		for( unsigned i=0; i<vecTeamStartPos.size(); i++ )
		{	
			if( vecTeamStartPos[i].iTeamColor == TEAM_NONE )
			{
				//pRenderParam->color	= D3DXCOLOR(0, 0.5f, 0, 1 );
				renderParam.color = D3DXCOLOR(0, 0.5f, 0, 1);

				const float MAGIC_DOT_SIZE = 5.f;

				matTemp.Move( vecTeamStartPos[i].vPos );
				matTemp.Scale( MAGIC_DOT_SIZE, MAGIC_DOT_SIZE, MAGIC_DOT_SIZE );
				//pRenderParam->worldMatrix = matTemp.GetMatrix();
				D3DXMATRIX worldMatrix = matTemp.GetMatrix();

				//g_pKTDXApp->GetDGManager()->GetXRenderer()->BeginRender();
				//g_pKTDXApp->GetDGManager()->GetXRenderer()->OnFrameRender( m_pXMeshCube );
				//g_pKTDXApp->GetDGManager()->GetXRenderer()->EndRender();
				g_pKTDXApp->GetDGManager()->GetXRenderer()->OnFrameRender( renderParam, worldMatrix, *m_pXMeshCube );
			}
		}

		RenderNPCStartPosIndex( NULL );
	}
}


CX2LineMapToolMain::STeamStartPos* CX2LineMapToolMain::GetTeamStartPos( int iNPCStartPosKey )
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


HRESULT CX2LineMapToolMain::OnResetDevice()
{
	HRESULT hr;

	MainUI.SetLocation( DXUTGetBackBufferSurfaceDesc()->Width - MainUI.GetWidth(), 80 );
	m_DungeonUI.SetLocation( DXUTGetBackBufferSurfaceDesc()->Width - MainUI.GetWidth() - m_DungeonUI.GetWidth(), 10 );

	g_pKTDXApp->GetDGManager()->GetCamera().Point(1400, 600, -3000, 1400, 600, 10);
	D3DXVECTOR3 vEye = g_pKTDXApp->GetDGManager()->GetCamera().GetEye();
	D3DXVECTOR3 vLookAt = g_pKTDXApp->GetDGManager()->GetCamera().GetLookAt();
	m_FPSCamera.SetViewParams( &vEye, &vLookAt );
	m_FPSCamera.SetProjParams( D3DX_PI/4, 1.f, g_pKTDXApp->GetDGManager()->GetNear(), g_pKTDXApp->GetDGManager()->GetFar() );
	m_FPSCamera.SetRotateButtons( false, false, true, false );


	for( unsigned i=0; i<vecpXSkinAnim.size(); i++ )
	{	
		hr = vecpXSkinAnim[i]->OnResetDevice();
	}

	hr = g_pKTDXApp->GetDGManager()->GetDialogManager()->OnResetDevice();	

	return hr;
}

HRESULT CX2LineMapToolMain::OnLostDevice()
{
	HRESULT hr;
	for( unsigned i=0; i<vecpXSkinAnim.size(); i++ )
	{	
		hr = vecpXSkinAnim[i]->OnLostDevice();
	}

	hr = g_pKTDXApp->GetDGManager()->GetDialogManager()->OnLostDevice();	
	return hr;
}

bool CX2LineMapToolMain::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	bool bNoFurtherProcessing = false;

	if( true == m_bEnableUI )
	{
		bNoFurtherProcessing = MainUI.MsgProc( hWnd, uMsg, wParam, lParam );
		if( bNoFurtherProcessing )
			return true;

		bNoFurtherProcessing = m_DungeonUI.MsgProc( hWnd, uMsg, wParam, lParam );
		if( bNoFurtherProcessing )
			return true;
	}

	if( bNavigate == true )
		m_FPSCamera.HandleMessages(hWnd, uMsg, wParam, lParam);

	POINTS mousepos;
	switch( uMsg )
	{	
	case WM_KEYDOWN:
		switch( wParam )
		{
		case VK_SPACE:
			{
				//if( m_vecpPickedNPCData.size() > 1 )
				//{
				//	CX2LineMapToolMain::NPCData* pNPCData = m_vecpPickedNPCData.at(0);
				//	for( UINT i=1; i<m_vecpPickedNPCData.size(); i++ )
				//	{		
				//		CX2LineMapToolMain::NPCData* pTempNPCData = m_vecpPickedNPCData.at(i);
				//		pNPCData->m_vPos = pNPCData->m_vPos;
				//	}
				//}
			} break;

		case VK_DELETE:
			{
				if( m_MouseClickTarget == MCT_LINE )
				{
					if( 0 <= m_iPickedStartPosIndex && m_iPickedStartPosIndex < (int)vecTeamStartPos.size() )
					{
						vector<STeamStartPos>::iterator it = vecTeamStartPos.begin();
						for( int i=0; i<m_iPickedStartPosIndex; i++ )
						{
							it++;				
						}
						vecTeamStartPos.erase(it);
						m_iPickedStartPosIndex = -1;

						UpdateStartPosListBox(true);
						UpdateStartPosComboBox();
					}
				}

				if( MCT_NPC == m_MouseClickTarget )
				{
					for( UINT i=0; i<m_vecPickedNPCStartPosKey.size(); i++ )
					{
						for( UINT j=0; j<vecTeamStartPos.size(); j++ )
						{
							SNPCStartPos& npcStartPos = m_vecPickedNPCStartPosKey[i];
							if( vecTeamStartPos.at(j).key == npcStartPos.m_iStartPosIndex )
							{
								vecTeamStartPos.erase( vecTeamStartPos.begin()+j );
								break;
							}
						}
					}

					UpdateStartPosListBox(true);
					UpdateStartPosComboBox();
				}
			} break;

		} break;

	case WM_CHAR:
		switch(wParam)
		{
		case '1':
			{
				m_bEnableUI = !m_bEnableUI;
			} break;
		case 'c':
			{
				D3DXVECTOR3 vLookAt, vEye;
				if( NULL != pLineMap )
				{
					vLookAt = pLineMap->GetLineData(0)->startPos; 
					vEye = vLookAt + D3DXVECTOR3(0,0,-2500);
				}
				else
				{
					vLookAt = D3DXVECTOR3(0,0,0); 
					vEye = vLookAt + D3DXVECTOR3(0,0,-2500);
				}

				g_pKTDXApp->GetDGManager()->GetCamera().Point(vEye.x, vEye.y, vEye.z, vLookAt.x, vLookAt.y, vLookAt.z);
				m_FPSCamera.SetViewParams( &vEye, &vLookAt );

			} break;
		} break;

	case WM_DROPFILES:
		{
			UINT nDroppedFiles = 0;
			nDroppedFiles = DragQueryFile( (HDROP)wParam, 0xFFFFFFFF, NULL, MAX_PATH );

			wstring wstrFilePath; 
			WCHAR wszFilePath[MAX_PATH] = L"";	
			for( UINT i=0; i<nDroppedFiles; i++ )
			{
				DragQueryFile( (HDROP)wParam, i, wszFilePath, MAX_PATH);
			
				wstrFilePath = wszFilePath;			
				MakeUpperCase( wstrFilePath );
				int iResult = wstrFilePath.find( L".LUA" );
				if( 0 < iResult && iResult < (int)wstrFilePath.length() )
				{
					string strFilePath;
					ConvertWCHARToChar( strFilePath, wstrFilePath.c_str() );
					std::ifstream ifs( strFilePath.c_str() );
					int iIndex;
					CHAR szData[1024];					
					string strData;
					while( !ifs.eof() )
					{
						ifs.read( szData, sizeof(szData) );
						strData = szData;
						iIndex = strData.find( "g_pLineMap" );
						if( -1 != iIndex )		// 라인맵 스크립트
						{
							ifs.close();
							if( false == LoadLineMapFromFile( wszFilePath ) )
							{
								MessageBox( g_pKTDXApp->GetHWND(), L"라인맵 스크립트 열기 실패", L"ERROR", MB_OK );
							}
							break;
						}

						iIndex = strData.find( "STAGE_LIST" );
						if( -1 != iIndex )		// 던전 스크립트
						{
							ifs.close();
							if( false == LoadNPCPostionFromDungeonScript( wszFilePath ) )
							{
								//MessageBox( g_pKTDXApp->GetHWND(), L"던전 스크립트 열기 실패", L"ERROR", MB_OK );
							}
							break;
						}
					}
					if( true == ifs.is_open() )
					{
						MessageBox( g_pKTDXApp->GetHWND(), L"이건 라인맵도 아니고 던전스크립트도 아녀~ 이건 라인맵도 아니고 던전스크립트도 아녀~ ^^", L"ERROR", MB_OK );
						ifs.close();
					}
				}
			}
		} break;

	case WM_LBUTTONDOWN:
		{
			//m_bMouseDragging = true;

			if( NULL == pLineMap )
				break;
			mousepos = MAKEPOINTS(lParam);

			// line 클릭
			if( m_MouseClickTarget == MCT_LINE )
			{
				if( wParam & MK_SHIFT )
				{
					//m_bMouseDragging = false;
					AddNewStartPosition(mousepos);
				}
				else
				{
					int iPickedIndex = PickASegment(mousepos);
					if( -1 == iPickedIndex )
					{
						m_bMouseDragging = true;

						float w = (float)(int)g_pKTDXApp->GetViewport().Width;
						float h = (float)(int)g_pKTDXApp->GetViewport().Height;

						m_vDragStart.x = (float)(mousepos.x-w/2.f)/w *2.f;
						m_vDragStart.y = (float)(h/2.f-mousepos.y)/h *2.f;
					}

					if( wParam & MK_CONTROL )
					{
						if( iPickedIndex != -1 )
						{
							//m_bMouseDragging = false;
							bool bRemoved = false;
							vector<int>::iterator it;
							for( it = m_veciPickedSegmentIndex.begin(); it != m_veciPickedSegmentIndex.end(); it++ )
							{
								if( (*it) == iPickedIndex )
								{
									MainUI.GetListBox( IDC_LISTBOX_SEGMENT_INFO )->GetItem(iPickedIndex)->bSelected = false;
									m_veciPickedSegmentIndex.erase(it);
									bRemoved = true;
									break;
								}						
							}

							if( bRemoved == false )
							{					
								m_veciPickedSegmentIndex.push_back(iPickedIndex);
								MainUI.GetListBox( IDC_LISTBOX_SEGMENT_INFO )->GetItem(iPickedIndex)->bSelected = true;
							}
						}				
					}
					else 
					{
						for( int i=0; i<pLineMap->GetNumLineData(); i++ )
						{
							MainUI.GetListBox( IDC_LISTBOX_SEGMENT_INFO )->GetItem(i)->bSelected = false;
						}				
						m_veciPickedSegmentIndex.clear();

						if( iPickedIndex != -1 )
						{
							m_veciPickedSegmentIndex.push_back(iPickedIndex);
							MainUI.GetListBox( IDC_LISTBOX_SEGMENT_INFO )->GetItem(iPickedIndex)->bSelected = true;
						}
					}

					UpdateSegmentInfoListBox(true);
					UpdateSegmentInfoComboBox(false);
				}
			}


			// NPC 유닛 클릭
			if( m_MouseClickTarget == MCT_NPC )
			{
				if( wParam & MK_SHIFT )
				{
					m_bMouseDragging = false;
					AddNewStartPosition( mousepos );
					PickClickedNPC( m_iShowDungeonLevel, mousepos );
					UpdateNPCInfoUI();
				}
				else if( wParam & MK_CONTROL )
				{
					m_bMouseDragging = false;
					PickClickedNPC( m_iShowDungeonLevel, mousepos, false );
					UpdateNPCInfoUI();
				}
				else
				{
					m_bMouseDragging = true;
					PickClickedNPC( m_iShowDungeonLevel, mousepos );
					UpdateNPCInfoUI();
				}				
			}			

		} break;

	case WM_MOUSEMOVE:
		{
			mousepos = MAKEPOINTS(lParam);
			
			if( m_MouseClickTarget == MCT_NPC )
			{
				if( m_vecPickedNPCStartPosKey.size() != 0 &&
					true == m_bMouseDragging )
				{
					D3DXVECTOR3 vPos;
					if( true == GetMousePosOnLine( &vPos, mousepos ) )
					{
						for( UINT i=0; i<m_vecPickedNPCStartPosKey.size(); i++ )
						{
							for( UINT j=0; j<vecTeamStartPos.size(); j++ )
							{
								SNPCStartPos& npcStartPos = m_vecPickedNPCStartPosKey[i];

								if( vecTeamStartPos.at(j).key == npcStartPos.m_iStartPosIndex )
								{
									if( vPos.y != -1.f )
									{
										vecTeamStartPos.at(j).vPos = vPos;
										UpdateStartPosComboBox();
										UpdateStartPosListBox( true );
									}
									break;
								}
							}
						}
					}
				}
			}			

		} break;

	case WM_LBUTTONUP:
		{	
			mousepos = MAKEPOINTS(lParam);
			
			if( m_MouseClickTarget == MCT_LINE && 
				true == m_bMouseDragging &&
				NULL != pLineMap )
			{
				float w = (float)(int)g_pKTDXApp->GetViewport().Width;
				float h = (float)(int)g_pKTDXApp->GetViewport().Height;

				float MouseX = (float)(mousepos.x-w/2.f)/w *2.f;
				float MouseY = (float)(h/2.f-mousepos.y)/h *2.f;

				GetLineIndexInRect( m_vDragStart.x, m_vDragStart.y, MouseX, MouseY );

				for( int i=0; i<pLineMap->GetNumLineData(); i++ )
				{
					MainUI.GetListBox( IDC_LISTBOX_SEGMENT_INFO )->GetItem(i)->bSelected = false;
				}				

				for( unsigned i=0; i<m_veciPickedSegmentIndex.size() ; i++ )
				{
					MainUI.GetListBox( IDC_LISTBOX_SEGMENT_INFO )->GetItem(m_veciPickedSegmentIndex.at(i))->bSelected = true;
				}

				UpdateSegmentInfoListBox(true);
				UpdateSegmentInfoComboBox(false);
			}
			
			m_bMouseDragging = false;

		} break;

	case WM_RBUTTONDOWN:
		break;
	default:
		break;
	}

	return g_pKTDXApp->GetDGManager()->GetDialogManager()->MsgProc( hWnd, uMsg, wParam, lParam );
}

void CX2LineMapToolMain::LoadNPCSkinMesh( CX2UnitManager::NPC_UNIT_ID unitID, const WCHAR* wszSkinMeshFileName )
{
	CKTDXDeviceXSkinMesh* pSkinMesh = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( wszSkinMeshFileName );
	m_mapXSkinMesh[ unitID ] = pSkinMesh;
}

CKTDXDeviceXSkinMesh* CX2LineMapToolMain::GetNPCSkinMesh( CX2UnitManager::NPC_UNIT_ID unitID )
{
	map< CX2UnitManager::NPC_UNIT_ID, CKTDXDeviceXSkinMesh* >::iterator itSkinMesh = m_mapXSkinMesh.find( unitID );
	if( m_mapXSkinMesh.end() == itSkinMesh )
	{
		CX2UnitManager::NPCUnitTemplet*	pNPCTemplet = g_pData->GetUnitManager()->GetNPCUnitTemplet( unitID );

		//KLuaManager luaManager; 
		KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState(), 0, true );

		g_pKTDXApp->GetDeviceManager()->LoadLuaTinker( pNPCTemplet->m_LuaFileName.c_str() );
		g_pKTDXApp->GetDeviceManager()->LoadLuaManager( &luaManager, L"Enum.lua" );
		g_pKTDXApp->GetDeviceManager()->LoadLuaManager( &luaManager, pNPCTemplet->m_LuaFileName.c_str() );

		if( luaManager.BeginTable( "INIT_MOTION" ) == true )
		{
			wstring motion;
			LUA_GET_VALUE( luaManager, L"MOTION_FILE_NAME", motion, L"" );
			LoadNPCSkinMesh( unitID, motion.c_str() );

			luaManager.EndTable();
		}

	}

	itSkinMesh = m_mapXSkinMesh.find( unitID );

	return (CKTDXDeviceXSkinMesh*)itSkinMesh->second;
}



bool CX2LineMapToolMain::PickClickedNPC( int iDungeonLevel, POINTS mousepos, bool bClear )
{
	D3DXVECTOR3 vRayStart, vRayDir;
	D3DXMATRIX matView, matProj;
	matView = *m_FPSCamera.GetViewMatrix();			
	g_pKTDXApp->GetDevice()->GetTransform( D3DTS_PROJECTION, &matProj );
	ComputeRay( &vRayStart, &vRayDir, mousepos, &matView, &matProj);

	const float MAGIC_RAY_RANGE = 1000000.f;
	D3DXVECTOR3 vRayEnd;
	vRayEnd = vRayStart + vRayDir * MAGIC_RAY_RANGE;

	float fShortestHitDistance = MAGIC_RAY_RANGE;
	float fHitDistance = MAGIC_RAY_RANGE;
		
	if( true == bClear )
		m_vecPickedNPCStartPosKey.clear();

	if( true == m_bUseNPCMesh )
	{
		int iPickedNPCStartIndex = -1;
		CX2DungeonSubStage::NPCData* pPickedNPCData = NULL;
		if( 0 == iDungeonLevel )
		{
			if( m_pNormalStageData != NULL )
			{
				for( UINT i=0; i< m_pNormalStageData->m_SubStageDataList.size(); i++ )
				{
					CX2DungeonSubStage::SubStageData* pSubStageData = m_pNormalStageData->m_SubStageDataList[i];
					CollisionCheckNPCStartPos( pSubStageData, vRayStart, vRayEnd, fHitDistance, fShortestHitDistance,
												pPickedNPCData, iPickedNPCStartIndex );
	
				}			
			}

			if( m_pHardStageData != NULL )
			{
				for( UINT i=0; i< m_pHardStageData->m_SubStageDataList.size(); i++ )
				{
					CX2DungeonSubStage::SubStageData* pSubStageData = m_pHardStageData->m_SubStageDataList[i];
					CollisionCheckNPCStartPos( pSubStageData, vRayStart, vRayEnd, fHitDistance, fShortestHitDistance,
						pPickedNPCData, iPickedNPCStartIndex );
	
				}			
			}

			if( m_pExpertStageData != NULL )
			{
				for( UINT i=0; i< m_pExpertStageData->m_SubStageDataList.size(); i++ )
				{
					CX2DungeonSubStage::SubStageData* pSubStageData = m_pExpertStageData->m_SubStageDataList[i];
					CollisionCheckNPCStartPos( pSubStageData, vRayStart, vRayEnd, fHitDistance, fShortestHitDistance,
						pPickedNPCData, iPickedNPCStartIndex );
		
				}			
			}

		}
		else if( 1 == iDungeonLevel ) // NORMAL
		{
			if( m_pNormalStageData != NULL )
			{
				for( UINT i=0; i< m_pNormalStageData->m_SubStageDataList.size(); i++ )
				{
					CX2DungeonSubStage::SubStageData* pSubStageData = m_pNormalStageData->m_SubStageDataList[i];
					CollisionCheckNPCStartPos( pSubStageData, vRayStart, vRayEnd, fHitDistance, fShortestHitDistance,
						pPickedNPCData, iPickedNPCStartIndex );

				}			
			}
		}
		else if( 2 == iDungeonLevel )
		{
			if( m_pHardStageData != NULL )
			{
				for( UINT i=0; i< m_pHardStageData->m_SubStageDataList.size(); i++ )
				{
					CX2DungeonSubStage::SubStageData* pSubStageData = m_pHardStageData->m_SubStageDataList[i];
					CollisionCheckNPCStartPos( pSubStageData, vRayStart, vRayEnd, fHitDistance, fShortestHitDistance,
						pPickedNPCData, iPickedNPCStartIndex );
	
				}			
			}
		}
		else if( 3 == iDungeonLevel )
		{
			if( m_pExpertStageData != NULL )
			{
				for( UINT i=0; i< m_pExpertStageData->m_SubStageDataList.size(); i++ )
				{
					CX2DungeonSubStage::SubStageData* pSubStageData = m_pExpertStageData->m_SubStageDataList[i];
					CollisionCheckNPCStartPos( pSubStageData, vRayStart, vRayEnd, fHitDistance, fShortestHitDistance,
						pPickedNPCData, iPickedNPCStartIndex );

				}			
			}
		}



		if( NULL != pPickedNPCData && -1 != iPickedNPCStartIndex )
		{
			m_vecPickedNPCStartPosKey.push_back( SNPCStartPos( pPickedNPCData, iPickedNPCStartIndex ) );
			//m_vecPickedNPCStartPosKey.push_back( pPickedNPCData->m_StartPos );
			UpdateNPCInfoUI();
			return true;
		}
	}
	else
	{
		int iPickedNPCKey = -1;
		
		for( UINT i=0; i<vecTeamStartPos.size(); i++ )
		{
			STeamStartPos& npcStartPos = vecTeamStartPos[i];
			if( npcStartPos.iTeamColor == TEAM_NONE )
			{
				if( true == NPCRayHit( npcStartPos.vPos, vRayStart, vRayEnd, &fHitDistance ) )
				{
					if( fHitDistance < fShortestHitDistance )
					{
						iPickedNPCKey = npcStartPos.key;
					}
				}
			}
		}

		m_vecPickedNPCStartPosKey.push_back( SNPCStartPos( NULL, iPickedNPCKey ) );
		UpdateNPCInfoUI();
		return true;
	}

	return false;
}


bool CX2LineMapToolMain::NPCRayHit( D3DXVECTOR3 vPos, D3DXVECTOR3 rayStart, D3DXVECTOR3 rayEnd, float* pHitDistance )
{
	m_pMatrix->Move( vPos );
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


void CX2LineMapToolMain::DeleteAllSkinMeshAndSkinAnim()
{
	for( unsigned i=0; i < vecpXSkinAnim.size(); i++ )
	{
		//SAFE_DELETE( (CKTDGXSkinAnim*)vecpXSkinAnim[i] );
		SAFE_DELETE_KTDGOBJECT( (CKTDGXSkinAnim*)vecpXSkinAnim[i] );
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
		//SAFE_DELETE( pCKTDGXSkinAnim );
		SAFE_DELETE_KTDGOBJECT( pCKTDGXSkinAnim );
	}
	m_mapNPCXSkinAnim.clear();

}


bool CX2LineMapToolMain::LoadNPCPostionFromDungeonScript( const WCHAR* wszFileFullPath )
{
	if( pLineMap == NULL )
		return false;

	WCHAR wszFileName[_MAX_FNAME+_MAX_EXT] = L"";
	WCHAR drive[_MAX_DRIVE]		= L"";
	WCHAR dir[_MAX_DIR]			= L"";	 
	WCHAR fname[_MAX_FNAME]		= L"";
	WCHAR ext[_MAX_EXT]			= L"";

	_wsplitpath( wszFileFullPath, drive, dir, fname, ext);
	wcscat( wszFileName, fname);
	wcscat( wszFileName, ext);

	wstring wstrTempFileName;
	wstrTempFileName = fname;
	MakeUpperCase( wstrTempFileName );


	CX2Dungeon::DIFFICULTY_LEVEL dungeonDifficultyLevel = (CX2Dungeon::DIFFICULTY_LEVEL) -1;
	if( -1 != wstrTempFileName.find( L"_NORMAL" ) )
		dungeonDifficultyLevel = CX2Dungeon::DL_NORMAL;
	else if( -1 != wstrTempFileName.find( L"_HARD" ) )
		dungeonDifficultyLevel = CX2Dungeon::DL_HARD;
	else if( -1 != wstrTempFileName.find( L"_EXPERT" ) )
		dungeonDifficultyLevel = CX2Dungeon::DL_EXPERT;
	else
		dungeonDifficultyLevel = CX2Dungeon::DL_NORMAL;

	CX2Dungeon::DungeonData* pDungeonData = new CX2Dungeon::DungeonData;
	pDungeonData->m_DataFileName = wszFileName;
	CX2Dungeon* pDungeon = new CX2Dungeon( pDungeonData, true ); 
	if( NULL == pDungeon )
	{
		WCHAR wszPath[_MAX_DIR] = L"";
		wcscat( wszPath, drive );
		wcscat( wszPath, dir );

		string	strDir;
		ConvertWCHARToChar( strDir, wszPath );

		g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->AddDataDirectory( strDir );
		pDungeon = new CX2Dungeon( pDungeonData );
		if( NULL == pDungeon )
			return false;
	}

	CX2World::WORLD_ID worldID = g_pData->GetWorldManager()->GetWorldIDUsingLineMapFileName( m_wstrLineMapFileName.c_str() );

	// dungeon script에 해당하는 worldID가 있는지 찾는다
	vector<CX2DungeonStage::StageData*>& vecStageData = pDungeon->GetStageDataList();
	for( UINT i=0; i<vecStageData.size(); i++ )
	{
		if( vecStageData[i]->m_WorldID == worldID )
		{
			DeleteNPCSkinAnims( dungeonDifficultyLevel );

			switch( dungeonDifficultyLevel )
			{
			case CX2Dungeon::DL_NORMAL:
				{
					SAFE_DELETE( m_pNormalDungeon );
					m_pNormalDungeon = pDungeon;
					m_pNormalStageData = vecStageData[i];
					if( false == CheckDungeonScriptError( dungeonDifficultyLevel ) )
					{
						m_pNormalDungeon = NULL;
						m_pNormalStageData = NULL;
					}
					else
					{
						m_bUseNPCMesh = true;
						m_DungeonUI.GetButton( IDC_BUTTON_USE_NPC_MESH )->SetText( L"USE_NPC_MESH" );
						m_wstrDungeonFileName[ dungeonDifficultyLevel ] = wszFileName;
					}
				} break;

			case CX2Dungeon::DL_HARD:
				{
					SAFE_DELETE( m_pHardDungeon );
					m_pHardDungeon = pDungeon;
					m_pHardStageData = vecStageData[i];
					if( false == CheckDungeonScriptError( dungeonDifficultyLevel ) )
					{
						m_pHardDungeon = NULL;
						m_pHardStageData = NULL;
					}
					else
					{
						m_bUseNPCMesh = true;
						m_DungeonUI.GetButton( IDC_BUTTON_USE_NPC_MESH )->SetText( L"USE_NPC_MESH" );
						m_wstrDungeonFileName[ dungeonDifficultyLevel ] = wszFileName;
					}
				} break;

			case CX2Dungeon::DL_EXPERT:
				{
					SAFE_DELETE( m_pExpertDungeon );
					m_pExpertDungeon = pDungeon;
					m_pExpertStageData = vecStageData[i];
					if( false == CheckDungeonScriptError( dungeonDifficultyLevel ) )
					{
						m_pExpertDungeon = NULL;
						m_pExpertStageData = NULL;
					}
					else
					{
						m_bUseNPCMesh = true;
						m_DungeonUI.GetButton( IDC_BUTTON_USE_NPC_MESH )->SetText( L"USE_NPC_MESH" );
						m_wstrDungeonFileName[ dungeonDifficultyLevel ] = wszFileName;
					}
				} break;
			}

			LoadNPCSkinAnims( dungeonDifficultyLevel );	
			UpdateAnyInfoUI();
			return true; // OK
		}
	}

	
	MessageBox( g_pKTDXApp->GetHWND(), L"던전에 이 라인맵이 없습니다", L"ERROR", MB_OK );
	return false;
}



bool CX2LineMapToolMain::CheckDungeonScriptError( CX2Dungeon::DIFFICULTY_LEVEL dungeonDifficultyLevel )
{
	CX2DungeonStage::StageData*	pStageData = NULL;

	switch( dungeonDifficultyLevel )
	{
	case CX2Dungeon::DL_NORMAL:
		{
			pStageData = m_pNormalStageData;
		} break;

	case CX2Dungeon::DL_HARD:
		{
			pStageData = m_pHardStageData;
		} break;

	case CX2Dungeon::DL_EXPERT:
		{
			pStageData = m_pExpertStageData;
		} break;
	}

	bool bOK = true;

	for( UINT i=0; i<pStageData->m_SubStageDataList.size(); i++ )
	{
		CX2DungeonSubStage::SubStageData* pSubStageData = pStageData->m_SubStageDataList.at(i);	
		for( UINT j=0; j<pSubStageData->m_NPCDataList.size(); j++ )
		{
			CX2DungeonSubStage::NPCData* pNPCData = pSubStageData->m_NPCDataList.at(j);
			

			for( UINT k=0; k<pNPCData->m_vecStartPos.size(); k++ )
			{
				STeamStartPos* pTeamStartPos = GetTeamStartPos( pNPCData->m_vecStartPos[k] );

				if( pTeamStartPos == NULL )
				{
					WCHAR wszTempMsg[256];
					StringCchPrintf( wszTempMsg, sizeof(wszTempMsg), L"라인맵에 없는 NPC start pos(%d) 입니다", pNPCData->m_vecStartPos[k] );
					MessageBox( g_pKTDXApp->GetHWND(), wszTempMsg, L"ERROR", MB_OK );
					bOK = false;
				}
			}
		}
	}

	return bOK;
}





void CX2LineMapToolMain::CloseDungeonScript( CX2Dungeon::DIFFICULTY_LEVEL dungeonDifficultyLevel )
{

	switch( dungeonDifficultyLevel )
	{
	case CX2Dungeon::DL_NORMAL:
		{
			SAFE_DELETE( m_pNormalDungeon );
			m_pNormalStageData = NULL;
		} break;

	case CX2Dungeon::DL_HARD:
		{
			SAFE_DELETE( m_pHardDungeon );
			m_pHardStageData = NULL;
		} break;

	case CX2Dungeon::DL_EXPERT:
		{
			SAFE_DELETE( m_pExpertDungeon );
			m_pExpertStageData = NULL;
		} break;
	}
	m_wstrDungeonFileName[ dungeonDifficultyLevel ] = L"";
}

bool CX2LineMapToolMain::LoadLineMapFromFile( WCHAR* wszFileName )
{
	CompulsorybyConvertFileAnsiToUTF8W( wszFileName );

	if( wszFileName[0] == L'\0' )
		return false;
	
	DeleteAllSkinMeshAndSkinAnim();
	CloseDungeonScript( CX2Dungeon::DL_NORMAL );
	CloseDungeonScript( CX2Dungeon::DL_HARD );
	CloseDungeonScript( CX2Dungeon::DL_EXPERT );
	UpdateAnyInfoUI();

	m_CurrKeyUsed = 0;
	m_bUseNPCMesh = false;


	//SAFE_DELETE(pLineMap);
	SAFE_DELETE_KTDGOBJECT(pLineMap);
	//pLineMap = new CKTDGLineMap( g_pKTDXApp->GetDevice() );
	pLineMap = CKTDGLineMap::CreateLineMap();


	KGCMassFileManager::CMassFile::MASSFILE_MEMBERFILEINFO_POINTER Info;
	Info = g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->LoadDataFile( L"Enum.lua" );
	if( Info == NULL )
	{
		SAFE_DELETE_KTDGOBJECT(pLineMap);
		return false;
	}

	if( g_pKTDXApp->GetLuaBinder()->DoMemory( Info->pRealData, Info->size ) == E_FAIL )
	{
		SAFE_DELETE_KTDGOBJECT(pLineMap);
		return false;
	}

	m_wstrLineMapFileName = wszFileName;

	WCHAR FileName[256] = L"";
	WCHAR drive[_MAX_DRIVE]= L"";
	WCHAR dir[_MAX_DIR]= L"";
	WCHAR fname[_MAX_FNAME]= L"";
	WCHAR ext[_MAX_EXT]= L"";

	_wsplitpath( wszFileName, drive, dir, fname, ext);
	wcscat( FileName, fname);
	wcscat( FileName, ext);	

	m_wstrLineMapFileName = FileName;

	bool bResult = pLineMap->LoadLineMap( FileName );
	if( false == bResult )
	{
		string	strDir;
		char	csTemp[MAX_PATH] = "";

		WideCharToMultiByte( CP_ACP, 0, drive, -1, csTemp, MAX_PATH, NULL, NULL );
		strDir += csTemp;
		WideCharToMultiByte( CP_ACP, 0, dir, -1, csTemp, MAX_PATH, NULL, NULL );
		strDir += csTemp;

		g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->AddDataDirectory( strDir );

		bResult = pLineMap->LoadLineMap( FileName );
		if( false == bResult )
			//SAFE_DELETE(pLineMap);
			SAFE_DELETE_KTDGOBJECT(pLineMap);
	}

	if( pLineMap != NULL )
	{
		WCHAR wszOutput[4096];
		StringCchPrintf( wszOutput, 512, L"%.2f", pLineMap->GetLandHeight() );
		MainUI.GetEditBox( IDC_EDIT_LAND_HEIGHT )->SetText( wszOutput );
		
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
			//SNPCStartPos* pNPCStartPos = new SNPCStartPos( TEAM_NONE, pLineMap->GetStartPosition(i), iTempInitialDir, iLineIndex );
			//m_vecpNPCStartPos.push_back( pNPCStartPos );			
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

		m_iPickedStartPosIndex = -1;
		UpdateStartPosListBox(true);

		m_veciPickedSegmentIndex.clear();
		UpdateSegmentInfoListBox( true );
		UpdateSegmentInfoComboBox( true );

		if( m_nVBHighlightedSegmentSize < (unsigned)2*GetNumSegments() )
		{
			m_nVBHighlightedSegmentSize = (unsigned)2*GetNumSegments();

			SAFE_RELEASE(m_pVBHighlightedSegment);
			g_pKTDXApp->GetDevice()->CreateVertexBuffer( m_nVBHighlightedSegmentSize * sizeof(LINE_MAP_VERTEX), 
				D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, D3DFVF_LINE_MAP_VERTEX, 
				D3DPOOL_SYSTEMMEM, &m_pVBHighlightedSegment, NULL);
		}
	}

	UpdateAnyInfoUI();
	return bResult;
}


void CX2LineMapToolMain::DisplaceAllUserStartPos( D3DXVECTOR3 vDisplace )
{
	for( UINT i=0; i< vecTeamStartPos.size(); i++ )
	{
		STeamStartPos& sp = vecTeamStartPos[i];
		if( sp.iTeamColor != TEAM_NONE )
		{
			sp.vPos += vDisplace;
		}
	}
}

void CX2LineMapToolMain::DisplaceAllNPCStartPos( D3DXVECTOR3 vDisplace )
{
	for( UINT i=0; i< vecTeamStartPos.size(); i++ )
	{
		STeamStartPos& sp = vecTeamStartPos[i];
		if( sp.iTeamColor == TEAM_NONE )
		{
			sp.vPos += vDisplace;
		}
	}
}


// start pos의 line index를 새로 모두 계산
void CX2LineMapToolMain::RecomputeStartPosLineIndex()
{
	if( pLineMap == NULL )
		return;

	for( UINT i=0; i<vecTeamStartPos.size(); i++ )
	{
		STeamStartPos& sp = vecTeamStartPos.at(i);
		sp.vPos = pLineMap->GetLandPosition( sp.vPos, 30.f, &sp.iLineIndex );
	}
}


float CX2LineMapToolMain::ComputeLandHeight()
{
	if( NULL == pLineMap )
		return 0.f;

	float fLandHeight = pLineMap->GetLineData(0)->startPos.y;


	for( int i=0; i<pLineMap->GetNumLineData(); i++ )
	{
		CKTDGLineMap::LineData* pLineData = pLineMap->GetLineData(i);
		if( pLineData->startPos.y < fLandHeight )
			fLandHeight = pLineData->startPos.y;
		if( pLineData->endPos.y < fLandHeight )
			fLandHeight = pLineData->endPos.y;
	}

	return fLandHeight;
	
}

bool CX2LineMapToolMain::SaveLineMapToFile( WCHAR* wszFileName, bool bUpdateFileName )
{
	if( pLineMap == NULL )
		return false;
	
	bool bResult = true;

	LineMapFloatToInt();
	
	std::sort( vecTeamStartPos.begin(), vecTeamStartPos.end(), CTeamStartPosSort() );

	RecomputeStartPosLineIndex();

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

	if( true == bUpdateFileName )
		m_wstrLineMapFileName = wstrFilePath;

	ofs << std::endl << std::endl;
	//ofs << "g_pLineMap:SetLandHeight( " << pLineMap->GetLandHeight() << " )" << std::endl;
	float fLandHeight = ComputeLandHeight();
	ofs << "g_pLineMap:SetLandHeight( " << fLandHeight << " )" << std::endl;
	ofs << std::endl << std::endl;
	ofs << std::endl << std::endl;
	ofs << "-- RED, BLUE, TEAM START POS" << std::endl;

	// red, blue startpos을 출력하자
	string strDirection, strTeamColor;
	for( unsigned j=0; j<vecTeamStartPos.size(); j++ )
	{
		STeamStartPos& spos = vecTeamStartPos[j];

		if( spos.iInitialDir == DIR_LEFT )
			strDirection = "false";
		else 
			strDirection = "true";

		if( spos.iTeamColor == TEAM_RED )
			strTeamColor = "true";
		else if( spos.iTeamColor == TEAM_BLUE )
			strTeamColor = "false";

		if( spos.iTeamColor != TEAM_NONE )
		{
			ofs << "g_pLineMap:SetTeamStartPos( " << strTeamColor << ", D3DXVECTOR3( " 
				<< spos.vPos.x << ", " << spos.vPos.y << ", " << spos.vPos.z << " ), " 
				<< strDirection << ", " << spos.iLineIndex << " )" << std::endl;
		}
	}
	
	ofs << std::endl << std::endl;
	ofs << std::endl << std::endl;
	ofs << "-- NPC (or else) START POS" << std::endl;
	for( unsigned j=0; j<vecTeamStartPos.size(); j++ )
	{
		STeamStartPos& spos = vecTeamStartPos[j];

		if( spos.iInitialDir == DIR_LEFT )
			strDirection = "false";
		else 
			strDirection = "true";

		if( spos.iTeamColor == TEAM_NONE )
		{
			ofs << "g_pLineMap:AddStartPos( "
				<< spos.key << ", D3DXVECTOR3( " 
				<< spos.vPos.x << ", " << spos.vPos.y << ", " << spos.vPos.z << " ), " 
				<< strDirection << ", " << spos.iLineIndex << " )" << std::endl;
		}
	}

	ofs << std::endl << std::endl;
	ofs << std::endl << std::endl;
	ofs << std::endl << std::endl;
	ofs << "-- LINE DATA -- " << std::endl;

	bool bThereAreInvalidLineSet = false;

	// 라인정보 출력
	CKTDGLineMap::LineData* pLineData = NULL;	
	std::string strFootDustName;
	strFootDustName.reserve(512);
	for( int i=0; i<pLineMap->GetNumLineData(); i++ )
	{
		pLineData = pLineMap->GetLineData(i);

		if( pLineData->lineSet < 0 )
			bThereAreInvalidLineSet = true;

		ofs << "g_pLineMap:AddLine_LUA" << std::endl;
		ofs << "{" << std::endl;
		ofs << "	LINE_NUM		= " << i << ", " << std::endl;
		if( true == pLineData->bEnable )
		{
			//ofs << "	ENABLE			= TRUE," << std::endl;
		}
		else
		{
			ofs << "	ENABLE			= FALSE," << std::endl;
		}

		ofs << "	LINE_TYPE 		= ";
		
		switch( pLineData->lineType )
		{
		default:
		case CKTDGLineMap::LT_NORMAL:
			ofs << "LINE_TYPE[\"LT_NORMAL\"], " << std::endl;
			break;
		case CKTDGLineMap::LT_WALL:
			ofs << "LINE_TYPE[\"LT_WALL\"], " << std::endl;
			break;
		case CKTDGLineMap::LT_POTAL:
			ofs << "LINE_TYPE[\"LT_POTAL\"], " << std::endl;
			break;
		case CKTDGLineMap::LT_HOLE:
			ofs << "LINE_TYPE[\"LT_HOLE\"], " << std::endl;
			break;
		case CKTDGLineMap::LT_JUMP_UP_REL:
			ofs << "LINE_TYPE[\"LT_JUMP_UP_REL\"], " << std::endl;
			break;
		case CKTDGLineMap::LT_JUMP_UP_ABS:
			ofs << "LINE_TYPE[\"LT_JUMP_UP_ABS\"], " << std::endl;
			break;
		case CKTDGLineMap::LT_BUNGEE:
			ofs << "LINE_TYPE[\"LT_BUNGEE\"], " << std::endl;
			break;
		case CKTDGLineMap::LT_MONSTER_ROAD:
			ofs << "LINE_TYPE[\"LT_MONSTER_ROAD\"], " << std::endl;
			break;
		case CKTDGLineMap::LT_UNIT_ROAD:
			ofs << "LINE_TYPE[\"LT_UNIT_ROAD\"], " << std::endl;
			break;
		case CKTDGLineMap::LT_OTHER_ROAD:
			ofs << "LINE_TYPE[\"LT_OTHER_ROAD\"], " << std::endl;
			break;
		case CKTDGLineMap::LT_CLIFF:
			ofs << "LINE_TYPE[\"LT_CLIFF\"], " << std::endl;
			break;			
		}

		if( -1 != pLineData->potalGroup )
		{
			ofs << "	POTAL_GROUP		= " << pLineData->potalGroup << ", " << std::endl;
		}

		if( 1.f != pLineData->fStopFactor )
		{
			ofs << "	STOP_FACTOR		= " << pLineData->fStopFactor << ", " << std::endl;
		}
				

		switch( pLineData->eDustType )
		{
		default:
		case CKTDGLineMap::LDT_DEFALT_DUST:
			{
				//ofs << "	DUST_TYPE   = LINE_DUST_TYPE[\"LDT_DEFALT_DUST\"]," << std::endl;
			} break;

		case CKTDGLineMap::LDT_WATER_SPLASH:
			{
				ofs << "    DUST_TYPE   = LINE_DUST_TYPE[\"LDT_WATER_SPLASH\"]," << std::endl;
			} break;
		}

		ofs << "	BEFORE_LINE		= " << pLineData->beforeLine	<< ", " << std::endl;
		ofs << "	NEXT_LINE		= " << pLineData->nextLine		<< ", " << std::endl;
		ofs << "	LINE_SET		= " << pLineData->lineSet		<< ", " << std::endl;

		if( 0.f != pLineData->m_fSpeed )
		{
			ofs << "	SPEED			= " << pLineData->m_fSpeed << ", " << std::endl;
		}


		if( 0.f != pLineData->m_vJumpSpeed.x )
		{
			ofs << "	JUMP_SPEED_X			= " << pLineData->m_vJumpSpeed.x << ", " << std::endl;
		}


		if( 0.f != pLineData->m_fSpeed )
		{
			ofs << "	JUMP_SPEED_Y			= " << pLineData->m_vJumpSpeed.y << ", " << std::endl;
		}

		
		if( false == pLineData->bCanDown )
		{
			ofs << "	CAN_DOWN		= FALSE, " << std::endl;
		}

		if( false == pLineData->m_bCanPass )
		{
			ofs << "	CAN_PASS		= FALSE, " << std::endl;
		}

		if( -1 != pLineData->m_iTeleportLineIndex )
		{
			ofs << "	TELEPORT_LINE		= " << pLineData->m_iTeleportLineIndex << ", " << std::endl;
		}

		if( true == pLineData->m_bEnableWallJump )
		{
			ofs << "	ENABLE_WALL_JUMP		= TRUE, " << std::endl;
		}

		if( false == pLineData->m_bMonsterAllowed )
		{
			ofs << "	MONSTER_ALLOWED		= FALSE, " << std::endl;
		}

		if( pLineData->m_iCameraIndex != -1 )
		{
			ofs << "	CAMERA_INDEX	= " << pLineData->m_iCameraIndex << ", " << std::endl;
		}


		ofs << "	\"START_POS		= D3DXVECTOR3(" << pLineData->startPos.x << ", " << pLineData->startPos.y << ", " << pLineData->startPos.z << ")\", " << std::endl;
		ofs << "	\"END_POS		= D3DXVECTOR3(" << pLineData->endPos.x << ", " << pLineData->endPos.y << ", " << pLineData->endPos.z << ")\", " << std::endl;

#ifdef UNDERWATER_LINEMAP
		if( true == pLineData->m_bUnderWater )
		{
			ofs << "	IS_UNDERWATER			= TRUE," << std::endl;
			ofs << "	UNDERWATER_HEIGHT		= " << pLineData->m_fWaterHeight << ", " << std::endl;
			ofs << "	UNDERWATER_BUOYANCY		= " << pLineData->m_fBuoyancy << ", " << std::endl;
			ofs << "	UNDERWATER_RESISTANCE	= " << pLineData->m_fResistance << ", " << std::endl;
			ofs << "	UNDERWATER_ANIMSPEED	= " << pLineData->m_fAnimSpeed << ", " << std::endl;
			ofs << "	UNDERWATER_VARIANCEOXYEN= " << pLineData->m_fVarianceOxyen << ", " << std::endl;
		}		

		if( pLineData->m_bForceChangeColor == true )
		{
			ofs << "	FORCE_CHANGE_COLOR		= TRUE," << std::endl;
		}

		if( pLineData->m_bUnderWater == true || pLineData->m_bForceChangeColor == true )
		{
			ofs << "	UNIT_COLOR_R			= " << pLineData->m_cLineColor.r << ", " << std::endl;
			ofs << "	UNIT_COLOR_G			= " << pLineData->m_cLineColor.g << ", " << std::endl;
			ofs << "	UNIT_COLOR_B			= " << pLineData->m_cLineColor.b << ", " << std::endl;
		}		
#endif

		ofs << "}" << std::endl;
		ofs << std::endl;
	}

	// 카메라 정보 출력
	ofs << std::endl;
	ofs << "-- CAMERA DATA " << std::endl;
	ofs << std::endl;

	const CKTDGLineMap::CameraData * pCameraData = NULL;	
	
	for( i = 0; i < pLineMap->GetNumCameraData(); ++i )
	{
		pCameraData = pLineMap->GetCameraDataList(i);

		ofs << "g_pLineMap:AddCameraData_LUA" << std::endl;
		ofs << "{" << std::endl;

		//{{ 내용출력 시작.		
		if( 0.f == pCameraData->m_vRelativeEye.x &&
			0.f == pCameraData->m_vRelativeEye.y &&
			0.f == pCameraData->m_vRelativeEye.z )
		{
			if( pCameraData->m_bFocusUnit == false )
			{
				ofs << "	FOCUS_UNIT	= FALSE," << std::endl;
			}
			ofs << "	\"EYE_POS	= D3DXVECTOR3(" << pCameraData->m_vEye.x << ", " << pCameraData->m_vEye.y << ", " << pCameraData->m_vEye.z << ")\", " << std::endl;
			ofs << "	\"LOOKAT_POS	= D3DXVECTOR3(" << pCameraData->m_vLookAt.x << ", " << pCameraData->m_vLookAt.y << ", " << pCameraData->m_vLookAt.z << ")\", " << std::endl;
		}
		else
		{
			ofs << "	RELATIVE_EYE_POS_X	= " << pCameraData->m_vRelativeEye.x << ", " << std::endl;
			ofs << "	RELATIVE_EYE_POS_Y	= " << pCameraData->m_vRelativeEye.y << ", " << std::endl;
			ofs << "	RELATIVE_EYE_POS_Z	= " << pCameraData->m_vRelativeEye.z << ", " << std::endl;

			ofs << "	CAMERA_REPOSITION_SPEED = " << pCameraData->m_fCameraRepositionSpeed << ", " << std::endl;
		}
		//}} 내용출력 끝.
		ofs << "}" << std::endl;
		ofs << std::endl;
	}


	UpdateAnyInfoUI();
	ofs.close();

	ConvertFullPathFileAnsiToUTF8( strFileName, strFileName );

	if( true == bThereAreInvalidLineSet )
	{
		MessageBox( g_pKTDXApp->GetHWND(), L"LINE_SET이 잘못된 LINE_DATA가 있습니다.", L"ERROR", MB_OK );
	}

	return bResult;

}


// return value는 화면(즉, 2차원 평면위)에서의 점과 선분 사이의 거리를 의미한다. 
float CX2LineMapToolMain::DistanceBetweenPointAndSegment(float& fLerpCoef, D3DXVECTOR3* pvPoint,
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


void CX2LineMapToolMain::AddNewStartPosition(const POINTS& mousepos)
{
	if( pLineMap == NULL || pLineMap->GetNumLineData() <= 0 )
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
	for( int i=0; i<this->pLineMap->GetNumLineData(); i++ )
	{
		pLineData = pLineMap->GetLineData(i);
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
		pLineData = pLineMap->GetLineData(iPickedSegmentIndex);
		D3DXVECTOR3 vStartPos = pLineData->startPos;
		D3DXVECTOR3 vEndPos   = pLineData->endPos;
		D3DXVECTOR3 vPointOnSegmentPicked = vStartPos + (vEndPos-vStartPos) * fLerpCoefPicked;

		{
			int key=-1;
			if( iTeamColor == TEAM_NONE )
			{
				key = GetNextUniqueNPCStartPosKey();
				if( key == -1 )
					return;
			}

			vecTeamStartPos.push_back( STeamStartPos( key, iTeamColor, vPointOnSegmentPicked, iInitialDir, iPickedSegmentIndex ) );
			UpdateStartPosListBox(true);
			MainUI.GetListBox( IDC_LISTBOX_START_POS )->SelectItem( (int)vecTeamStartPos.size()-1 );		
			UpdateStartPosComboBox();		
		}
	}
}




bool CX2LineMapToolMain::GetMousePosOnLine( D3DXVECTOR3* pvPosOnLine, const POINTS& mousepos )
{
	if( pLineMap == NULL || pLineMap->GetNumLineData() <= 0 )
		return false;

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
	for( int i=0; i<this->pLineMap->GetNumLineData(); i++ )
	{
		pLineData = pLineMap->GetLineData(i);
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
		pLineData = pLineMap->GetLineData(iPickedSegmentIndex);
		D3DXVECTOR3 vStartPos = pLineData->startPos;
		D3DXVECTOR3 vEndPos   = pLineData->endPos;

		D3DXVECTOR3 vPointOnSegmentPicked = vStartPos + (vEndPos-vStartPos) * fLerpCoefPicked;
		*pvPosOnLine = vPointOnSegmentPicked;
		return true;
	}	
	return false;
}


bool CX2LineMapToolMain::HighlightPickedSegments()
{
	float fColor = fabs( sinf( (float)g_pKTDXApp->GetTime()*4.f ) );
	m_coHighlight = D3DXCOLOR(fColor, fColor, fColor, 0.5f);

	HRESULT				hr;
	LINE_MAP_VERTEX*	pVertices = NULL;

	if(FAILED(hr = m_pVBHighlightedSegment->Lock( 0, m_nVBHighlightedSegmentSize * sizeof(LINE_MAP_VERTEX), (void**) &pVertices, D3DLOCK_DISCARD )))
		return false;
	for( unsigned i=0; i<m_veciPickedSegmentIndex.size(); i++ )
	{
		CKTDGLineMap::LineData* pLineData = pLineMap->GetLineData( (int)m_veciPickedSegmentIndex[i] );
		pVertices[i*2].pos		= pLineData->startPos;
		pVertices[i*2+1].pos	= pLineData->endPos;
		pVertices[i*2].color	= D3DCOLOR_COLORVALUE( m_coHighlight.r, m_coHighlight.g, m_coHighlight.b, m_coHighlight.a );
		pVertices[i*2+1].color	= D3DCOLOR_COLORVALUE( m_coHighlight.r, m_coHighlight.g, m_coHighlight.b, m_coHighlight.a );
	}
	if(FAILED(hr = m_pVBHighlightedSegment->Unlock()))
		return false;

	return true;
}

int CX2LineMapToolMain::PickASegment(const POINTS& mousepos)
{
	if( pLineMap == NULL || pLineMap->GetNumLineData() <= 0 )
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
	for( int i=0; i<this->pLineMap->GetNumLineData(); i++ )
	{
		pLineData = pLineMap->GetLineData(i);
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


void CX2LineMapToolMain::UpdateStartPosListBox( bool bClearAll )
{
	bClearAll = true; // 무조건 전체 리스트를 갱신하자
	std::sort( vecTeamStartPos.begin(), vecTeamStartPos.end(), CTeamStartPosSort() );

	WCHAR wszOutput[512];
	wstring wstrTeamColor, wstrDirection;

	int iRedTeamStartIndex		= -1;
	int iBlueTeamStartIndex		= -1;
	int iDungeonStartIndex		= -1;
	int iStartIndex				= -1;	
	int iOutputIndex			= -1;

	if( bClearAll == true )
	{
		MainUI.GetListBox( IDC_LISTBOX_START_POS )->RemoveAllItems();	

		for( unsigned i=0; i<vecTeamStartPos.size(); i++ )
		{
			if( vecTeamStartPos[i].iInitialDir == DIR_LEFT )
				wstrDirection = L"LEFT";
			else
				wstrDirection = L"RIGHT";

			if( vecTeamStartPos[i].iTeamColor == TEAM_RED )
			{
				iRedTeamStartIndex++;
				iOutputIndex = iRedTeamStartIndex;
				wstrTeamColor = L"RED ";
			}
			else if( vecTeamStartPos[i].iTeamColor == TEAM_BLUE )
			{
				iBlueTeamStartIndex++;
				iOutputIndex = iBlueTeamStartIndex;
				wstrTeamColor = L"BLUE";
			}
			else if( vecTeamStartPos[i].iTeamColor == TEAM_NONE ) 
			{
				iOutputIndex = vecTeamStartPos[i].key;
				wstrTeamColor = L"NONE";
			}
			else
			{
				continue;
			}

			StringCchPrintf( wszOutput, sizeof(wszOutput), L"%2d - %s, <%.0f, %.0f, %.0f>, %s", iOutputIndex, wstrTeamColor.c_str(), 
				vecTeamStartPos[i].vPos.x, vecTeamStartPos[i].vPos.y, vecTeamStartPos[i].vPos.z, 
				wstrDirection.c_str() );

			MainUI.GetListBox( IDC_LISTBOX_START_POS )->AddItem( wszOutput, NULL );
		}	
	}

	MainUI.GetListBox( IDC_LISTBOX_START_POS )->SelectItem( m_iPickedStartPosIndex );
}

void CX2LineMapToolMain::UpdateStartPosComboBox()
{
	wstring wstrDirection, wstrTeamColor;
	if( m_iPickedStartPosIndex < 0 || m_iPickedStartPosIndex > (int)vecTeamStartPos.size()-1 )
		wstrTeamColor = L"----";
	else if( vecTeamStartPos[m_iPickedStartPosIndex].iTeamColor == TEAM_RED )
		wstrTeamColor = L"RED";
	else if( vecTeamStartPos[m_iPickedStartPosIndex].iTeamColor == TEAM_BLUE )
		wstrTeamColor = L"BLUE";
	else if( vecTeamStartPos[m_iPickedStartPosIndex].iTeamColor == TEAM_NONE )
		wstrTeamColor = L"NONE";

	MainUI.GetComboBox( IDC_COMBOBOX_START_POS_TEAM_COLOR )->SetSelectedByText( wstrTeamColor.c_str() );

	if( m_iPickedStartPosIndex < 0 || m_iPickedStartPosIndex > (int)vecTeamStartPos.size()-1 )
		wstrDirection = L"----";
	else if( vecTeamStartPos[m_iPickedStartPosIndex].iInitialDir == DIR_LEFT )
		wstrDirection = L"LEFT";
	else
		wstrDirection = L"RIGHT";
	MainUI.GetComboBox( IDC_COMBOBOX_START_POS_DIRECTION )->SetSelectedByText( wstrDirection.c_str() );
}

void CX2LineMapToolMain::UpdateNPCStartPosIndex()
{
	
}


void CX2LineMapToolMain::UpdateNPCInfoUI()
{
	CDXUTComboBox* pCombo = NULL;
	CDXUTEditBox* pEdit = NULL;

	//if( m_vecpPickedNPCData.size() <= 0 )
	//{
	//	pCombo = m_DungeonUI.GetComboBox( IDC_COMBOBOX_NPC_ACTIVE );
	//	pCombo->SetSelectedByText( L"ACTIVE" );
	//	pEdit = m_DungeonUI.GetEditBox( IDC_EDITBOX_NPC_UNIT_ID );
	//	pEdit->SetText( L"" );
	//	pEdit = m_DungeonUI.GetEditBox( IDC_EDITBOX_NPC_KEY_CODE );
	//	pEdit->SetText( L"" );
	//	pEdit = m_DungeonUI.GetEditBox( IDC_EDITBOX_NPC_ADD_POS_X );
	//	pEdit->SetText( L"" );
	//	pEdit = m_DungeonUI.GetEditBox( IDC_EDITBOX_NPC_ADD_POS_Y );
	//	pEdit->SetText( L"" );
	//	pEdit = m_DungeonUI.GetEditBox( IDC_EDITBOX_NPC_ADD_POS_Z );
	//	pEdit->SetText( L"" );
	//}
	//else
	//{
	//	CX2LineMapToolMain::NPCData* pNPCData = m_vecpPickedNPCData[0];
	//	
	//	pCombo = m_DungeonUI.GetComboBox( IDC_COMBOBOX_NPC_ACTIVE );
	//	if( true == pNPCData->m_bActive )
	//		pCombo->SetSelectedByText( L"ACTIVE" );
	//	else
	//		pCombo->SetSelectedByText( L"NON-ACTIVE" );


	//	pCombo = m_DungeonUI.GetComboBox( IDC_COMBOBOX_NPC_GAGEBAR );
	//	if( true == pNPCData->m_bActive )
	//		pCombo->SetSelectedByText( L"GAGEBAR" );
	//	else
	//		pCombo->SetSelectedByText( L"NO-GAGEBAR" );


	//	pCombo = m_DungeonUI.GetComboBox( IDC_COMBOBOX_NPC_FOCUS_CAMERA );
	//	if( true == pNPCData->m_bActive )
	//		pCombo->SetSelectedByText( L"FOCUS_CAMERA" );
	//	else
	//		pCombo->SetSelectedByText( L"NO-FOCUS_CAMERA" );


	//	pEdit = m_DungeonUI.GetEditBox( IDC_EDITBOX_NPC_UNIT_ID );		
	//	CX2UnitManager::NPCUnitTemplet*	pNPCTemplet = g_pData->GetUnitManager()->GetNPCUnitTemplet( pNPCData->m_UnitID );
	//	pEdit->SetText( pNPCTemplet->name.c_str() );

	//	pEdit = m_DungeonUI.GetEditBox( IDC_EDITBOX_NPC_KEY_CODE );
	//	{
	//		std::wstringstream strm; 
	//		strm << pNPCData->m_KeyCode;
	//		pEdit->SetText( strm.str().c_str() );
	//	}

	//	pEdit = m_DungeonUI.GetEditBox( IDC_EDITBOX_NPC_ADD_POS_X );
	//	{
	//		std::wstringstream strm; 
	//		strm << pNPCData->m_AddPos.x;
	//		pEdit->SetText( strm.str().c_str() );
	//	}

	//	pEdit = m_DungeonUI.GetEditBox( IDC_EDITBOX_NPC_ADD_POS_Y );
	//	{
	//		std::wstringstream strm; 
	//		strm << pNPCData->m_AddPos.y;
	//		pEdit->SetText( strm.str().c_str() );
	//	}

	//	pEdit = m_DungeonUI.GetEditBox( IDC_EDITBOX_NPC_ADD_POS_Z );
	//	{
	//		std::wstringstream strm; 
	//		strm << pNPCData->m_AddPos.z;
	//		pEdit->SetText( strm.str().c_str() );
	//	}
	//}
}


void CX2LineMapToolMain::UpdateSegmentInfoListBox( bool bClearAll )
{
	WCHAR wszOutput[512];
	wstring wstrEnable, wstrLineType, wstrLineDustType;

	if( bClearAll == true ) 
	{
		MainUI.GetListBox( IDC_LISTBOX_SEGMENT_INFO )->RemoveAllItems();

		for( int i=0; i<pLineMap->GetNumLineData(); i++ )
		{
			CKTDGLineMap::LineData* pLineData = pLineMap->GetLineData(i);
			if( pLineData->bEnable == true )
				wstrEnable = L"on";
			else
				wstrEnable = L"off";

			switch( pLineData->lineType )
			{
			default:
			case CKTDGLineMap::LT_NORMAL:
				wstrLineType = L"LT_NORMAL";
				break;
			case CKTDGLineMap::LT_WALL:
				wstrLineType = L"LT_WALL";
				break;
			case CKTDGLineMap::LT_POTAL:
				wstrLineType = L"LT_POTAL";
				break;
			case CKTDGLineMap::LT_HOLE:
				wstrLineType = L"LT_HOLE";
				break;
			case CKTDGLineMap::LT_JUMP_UP_REL:
				wstrLineType = L"LT_JUMP_UP_REL";
				break;
			case CKTDGLineMap::LT_JUMP_UP_ABS:
				wstrLineType = L"LT_JUMP_UP_ABS";
				break;
			case CKTDGLineMap::LT_BUNGEE:
				wstrLineType = L"LT_BUNGEE";
				break;
			case CKTDGLineMap::LT_MONSTER_ROAD:
				wstrLineType = L"LT_MONSTER_ROAD";
				break;
			case CKTDGLineMap::LT_UNIT_ROAD:
				wstrLineType = L"LT_UNIT_ROAD";
				break;
			}

			switch( pLineData->eDustType )
			{
			default:
			case CKTDGLineMap::LDT_DEFALT_DUST:
				{
					wstrLineDustType = L"default_dust";
				} break;

			case CKTDGLineMap::LDT_WATER_SPLASH:
				{
					wstrLineDustType = L"water_splash";
				} break;
			}

			StringCchPrintf( wszOutput, sizeof(wszOutput), L"%d - %s, %s, %d, %d, %s, %d, %.1f,", i, wstrEnable.c_str(), 
				wstrLineType.c_str(), 
				pLineData->beforeLine, pLineData->nextLine, wstrLineDustType.c_str(), 
				pLineData->potalGroup, pLineData->fStopFactor );

			MainUI.GetListBox( IDC_LISTBOX_SEGMENT_INFO )->AddItem( wszOutput, NULL );
		}	

		for( unsigned j=0; j<m_veciPickedSegmentIndex.size(); j++ )
		{
			MainUI.GetListBox( IDC_LISTBOX_SEGMENT_INFO )->GetItem( m_veciPickedSegmentIndex[j] )->bSelected = true;
		}
	}
	else
	{
		assert(false);
	}
}


void CX2LineMapToolMain::UpdateSegmentInfoComboBox( bool bClearAll )
{
	WCHAR wszOutput[512];

	if( bClearAll == true )
	{
		CDXUTComboBox *pCombo = MainUI.GetComboBox( IDC_COMBOBOX_SEGMENT_BEFORE_LINE );
		pCombo->RemoveAllItems();
		pCombo->AddItem( L"-2", (LPVOID) 0 );
		pCombo->AddItem( L"-1", (LPVOID) 0 );
		for( int i=0; i<pLineMap->GetNumLineData(); i++ )
		{
			StringCchPrintf( wszOutput, sizeof(wszOutput), L"%d", i );
			pCombo->AddItem( wszOutput, (LPVOID) 0 );
		}

		pCombo = MainUI.GetComboBox( IDC_COMBOBOX_SEGMENT_NEXT_LINE );
		pCombo->RemoveAllItems();
		pCombo->AddItem( L"-2", (LPVOID) 0 );
		pCombo->AddItem( L"-1", (LPVOID) 0 );
		for( i=0; i<pLineMap->GetNumLineData(); i++ )
		{
			StringCchPrintf( wszOutput, sizeof(wszOutput), L"%d", i );
			pCombo->AddItem( wszOutput, (LPVOID) 0 );
		}	
	}


	if( m_veciPickedSegmentIndex.size() == 0 )
		return;

	int iFirstOne = 0;
	StringCchPrintf( wszOutput, sizeof(wszOutput), L"%d", m_veciPickedSegmentIndex[iFirstOne] );
	MainUI.GetEditBox( IDC_EDIT_SEGMENT_INDEX )->SetText( wszOutput );

	CKTDGLineMap::LineData* pLineData = pLineMap->GetLineData( m_veciPickedSegmentIndex[iFirstOne] );
	if( pLineData->bEnable )
		MainUI.GetComboBox( IDC_COMBOBOX_SEGMENT_ENABLE )->SetSelectedByText( L"ENABLE" );
	else
		MainUI.GetComboBox( IDC_COMBOBOX_SEGMENT_ENABLE )->SetSelectedByText( L"DISABLE" );

	MainUI.GetComboBox( IDC_COMBOBOX_SEGMENT_LINE_TYPE )->SetSelectedByData( (void*) (int) pLineData->lineType );

	MainUI.GetComboBox( IDC_COMBOBOX_LINE_DUST_TYPE )->SetSelectedByData( (void*) (int) pLineData->eDustType );
	

	StringCchPrintf( wszOutput, sizeof(wszOutput), L"%d", pLineData->potalGroup );
	MainUI.GetEditBox( IDC_EDIT_SEGMENT_PORTAL_GROUP )->SetText( wszOutput );
	StringCchPrintf( wszOutput, sizeof(wszOutput), L"%.1f", pLineData->fStopFactor );
	MainUI.GetEditBox( IDC_EDIT_SEGMENT_STOP_FACTOR )->SetText( wszOutput );

	StringCchPrintf( wszOutput, sizeof(wszOutput), L"%d", pLineData->beforeLine );
	MainUI.GetComboBox( IDC_COMBOBOX_SEGMENT_BEFORE_LINE )->SetSelectedByText( wszOutput );

	StringCchPrintf( wszOutput, sizeof(wszOutput), L"%d", pLineData->nextLine );
	MainUI.GetComboBox( IDC_COMBOBOX_SEGMENT_NEXT_LINE )->SetSelectedByText( wszOutput );	

	MainUI.GetEditBox( IDC_EDIT_LINE_START_POS )->SetTextFloatArray( (float*)&pLineData->startPos, 3 );	
	MainUI.GetEditBox( IDC_EDIT_LINE_END_POS )->SetTextFloatArray( (float*)&pLineData->endPos, 3 );	

	StringCchPrintf( wszOutput, sizeof(wszOutput), L"%d", pLineData->lineSet );
	MainUI.GetEditBox( IDC_EDIT_LINE_SET_INDEX )->SetText( wszOutput );   

	StringCchPrintf( wszOutput, sizeof(wszOutput), L"%f", pLineData->m_fSpeed );
	MainUI.GetEditBox( IDC_EDIT_LINE_SPEED )->SetText( wszOutput );   




	if( true == pLineData->bCanDown )
		MainUI.GetComboBox( IDC_COMBOBOX_CAN_DOWN )->SetSelectedByData( (void*) 0 );
	else
		MainUI.GetComboBox( IDC_COMBOBOX_CAN_DOWN )->SetSelectedByData( (void*) 1 );

	if( true == pLineData->m_bCanPass )
		MainUI.GetComboBox( IDC_COMBOBOX_CAN_PASS )->SetSelectedByData( (void*) 0 );
	else
		MainUI.GetComboBox( IDC_COMBOBOX_CAN_PASS )->SetSelectedByData( (void*) 1 );


	if( true == pLineData->m_bMonsterAllowed )
		MainUI.GetComboBox( IDC_COMBOBOX_MONSTER_ALLOWED )->SetSelectedByData( (void*) 0 );
	else
		MainUI.GetComboBox( IDC_COMBOBOX_MONSTER_ALLOWED )->SetSelectedByData( (void*) 1 );


	if( true == pLineData->m_bEnableWallJump )
		MainUI.GetComboBox( IDC_COMBOBOX_ENABLE_WALL_JUMP )->SetSelectedByData( (void*) 0 );
	else
		MainUI.GetComboBox( IDC_COMBOBOX_ENABLE_WALL_JUMP )->SetSelectedByData( (void*) 1 );



	StringCchPrintf( wszOutput, sizeof(wszOutput), L"%d", pLineData->m_iTeleportLineIndex );
	MainUI.GetEditBox( IDC_EDIT_TELEPORT_LINE_INDEX )->SetText( wszOutput );   

	MainUI.GetEditBox( IDC_EDIT_JUMP_SPEED )->SetTextFloatArray( (float*)&pLineData->m_vJumpSpeed, 3 );	

}


HRESULT CX2LineMapToolMain::CreateD3DXTextMesh( IDirect3DDevice9* pd3dDevice, LPD3DXMESH* ppMesh, 
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

void CX2LineMapToolMain::GetLineIndexInRect( float fLeft, float fTop, float fRight, float fBottom )
{
	float fTemp;
	if( fLeft > fRight )
	{
		fTemp = fLeft;
		fLeft = fRight;
		fRight = fTemp;
	}

	if( fTop < fBottom )
	{	
		fTemp = fTop;
		fTop = fBottom;
		fBottom = fTemp;
	}

	m_veciPickedSegmentIndex.clear();

	D3DXMATRIX matView, matProj, matViewProj;
	matView = *m_FPSCamera.GetViewMatrix();
	matProj = *m_FPSCamera.GetProjMatrix();
	D3DXMatrixMultiply( &matViewProj, &matView, &matProj );

	D3DXVECTOR3 vTemp;
	for( int i=0; i<pLineMap->GetNumLineData(); i++ )
	{
		CKTDGLineMap::LineData* pLineData = pLineMap->GetLineData(i);
		D3DXVec3TransformCoord( &vTemp, &pLineData->startPos, &matViewProj );		
		if( vTemp.x > fLeft && vTemp.x < fRight && vTemp.y < fTop && vTemp.y > fBottom )
		{
			m_veciPickedSegmentIndex.push_back(i);			
			continue;
		}

		D3DXVec3TransformCoord( &vTemp, &pLineData->endPos, &matViewProj );		
		if( vTemp.x > fLeft && vTemp.x < fRight && vTemp.y < fTop && vTemp.y > fBottom )
		{
			m_veciPickedSegmentIndex.push_back(i);			
			continue;
		}
	}
}

void CX2LineMapToolMain::BuildFloorMesh( WCHAR* wszFileName, float floorWidth, float floorHeight )
{
	if( NULL == pLineMap )
		return;

	int nLineData = pLineMap->GetNumLineData();

	LPD3DXMESH pMesh = NULL;
	D3DXMATERIAL* pMaterials = NULL;      // Array of material structs in the mesh
	DWORD         dwNumMaterials = 0;	// Number of material structs in the mesh

	DWORD dwFormat = D3DXF_FILEFORMAT_TEXT; // Text-format .x file
	DWORD dwFVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE ;
	D3DXCreateMeshFVF( 12*nLineData, 8*nLineData, D3DXMESH_SYSTEMMEM, dwFVF, g_pKTDXApp->GetDevice(), &pMesh );

	DWORD dwColor = D3DCOLOR_RGBA(255, 255, 255, 255);
	CUSTOMVERTEX* pVertices=0;
	if( S_OK == pMesh->LockVertexBuffer( D3DLOCK_DISCARD, (VOID**)&pVertices ) )
	{
		for( int i=0; i<nLineData; i++ )
		{
			// 8 개씩
			D3DXVECTOR3 vStartPos = pLineMap->GetLineData(i)->startPos;
			D3DXVECTOR3 vEndPos = pLineMap->GetLineData(i)->endPos;

			D3DXVECTOR3 vNormal = D3DXVECTOR3( vStartPos.z - vEndPos.z, 0.f, vEndPos.x - vStartPos.x );
			D3DXVec3Normalize( &vNormal, &vNormal );
			vNormal *= floorWidth / 2.f;

			pVertices[8*i + 0].p = vStartPos + vNormal;
			pVertices[8*i + 1].p = vEndPos   + vNormal;
			pVertices[8*i + 2].p = vEndPos   - vNormal;
			pVertices[8*i + 3].p = vStartPos - vNormal;

			pVertices[8*i + 4].p = vStartPos + vNormal - D3DXVECTOR3(0, floorHeight, 0);
			pVertices[8*i + 5].p = vEndPos   + vNormal - D3DXVECTOR3(0, floorHeight, 0);
			pVertices[8*i + 6].p = vEndPos   - vNormal - D3DXVECTOR3(0, floorHeight, 0);
			pVertices[8*i + 7].p = vStartPos - vNormal - D3DXVECTOR3(0, floorHeight, 0);


			// normal vector를 대략 정해주자
			D3DXVECTOR3 vCenter(0, 0, 0); 
			for( int j=0; j<8; j++ )
			{
				vCenter += pVertices[8*i+j].p;				
			}
			vCenter /= 8.f;

			for( int j=0; j<8; j++ )
			{
				D3DXVECTOR3 vPoint = (D3DXVECTOR3)pVertices[8*i+j].p;
				D3DXVECTOR3 vNormal = vPoint - vCenter;
				D3DXVec3Normalize( &vNormal, &vNormal );
				pVertices[8*i+j].normal = vNormal;
			}			

			for( int j=0; j<8; j++ )
				pVertices[8*i + j].color = dwColor;			
		}

		pMesh->UnlockVertexBuffer();
	}	

	WORD* pIndices=0;
	if( S_OK == pMesh->LockIndexBuffer( D3DLOCK_DISCARD, (VOID**)&pIndices) )
	{
		for( int i=0; i<nLineData; i++ )
		{		
			*(pIndices++) = 8*i + 3;	*(pIndices++) = 8*i + 2;	*(pIndices++) = 8*i + 1;
			*(pIndices++) = 8*i + 3;	*(pIndices++) = 8*i + 1;	*(pIndices++) = 8*i + 0;
			*(pIndices++) = 8*i + 2;	*(pIndices++) = 8*i + 6;	*(pIndices++) = 8*i + 5;
			*(pIndices++) = 8*i + 2;	*(pIndices++) = 8*i + 5;	*(pIndices++) = 8*i + 1;
			*(pIndices++) = 8*i + 6;	*(pIndices++) = 8*i + 7;	*(pIndices++) = 8*i + 4;
			*(pIndices++) = 8*i + 6;	*(pIndices++) = 8*i + 4;	*(pIndices++) = 8*i + 5;
			*(pIndices++) = 8*i + 7;	*(pIndices++) = 8*i + 3;	*(pIndices++) = 8*i + 0;
			*(pIndices++) = 8*i + 7;	*(pIndices++) = 8*i + 0;	*(pIndices++) = 8*i + 4;
			*(pIndices++) = 8*i + 7;	*(pIndices++) = 8*i + 6;	*(pIndices++) = 8*i + 2;
			*(pIndices++) = 8*i + 7;	*(pIndices++) = 8*i + 2;	*(pIndices++) = 8*i + 3;
			*(pIndices++) = 8*i + 0;	*(pIndices++) = 8*i + 1;	*(pIndices++) = 8*i + 5;
			*(pIndices++) = 8*i + 0;	*(pIndices++) = 8*i + 5;	*(pIndices++) = 8*i + 4;
		}

		pMesh->UnlockIndexBuffer();
	}

	// 파일 확장자가 없으면 붙여주자.
	std::string strFileName;	
	strFileName.reserve(512);
	ConvertWCHARToChar( strFileName, wszFileName );	

	int iStrLen = strFileName.length();
	if( '.' != strFileName[iStrLen-2] && ('x' != strFileName[iStrLen-1] || 'X' != strFileName[iStrLen-1]) ) 
	{
		strFileName.append(".X");
	}

	std::wstring wstrFilePath;
	ConvertCharToWCHAR( wstrFilePath, strFileName.c_str() );

	D3DXSaveMeshToX( wstrFilePath.c_str(), pMesh, NULL, pMaterials, NULL, dwNumMaterials, dwFormat );

	SAFE_RELEASE( pMesh );
	SAFE_DELETE_ARRAY( pMaterials );

}

void CX2LineMapToolMain::PickLineSet( int iLineSetIndex )
{
	m_veciPickedSegmentIndex.clear();

	for( int i=0; i<pLineMap->GetNumLineData(); i++ )
	{
		CKTDGLineMap::LineData* pLineData = pLineMap->GetLineData(i);
		if( iLineSetIndex == pLineData->lineSet )
		{
			m_veciPickedSegmentIndex.push_back(i);
		}
	}
}



void CX2LineMapToolMain::InitX2Data()
{
	////UDP 초기화
	g_pData->ResetGameUDP( 1 );

	////UI 파티클 시스템 생성
	//g_pData->ResetUIMajorParticle( L"UIMajorParticle.txt" );
	//g_pData->ResetUIMinorParticle( L"UIMinorParticle.txt" );

	//UI 메시플레이어
	//g_pData->ResetUIMajorXMeshPlayer( L"UIMajorXMeshPlayer.txt" );
	//g_pData->ResetUIMinorXMeshPlayer( L"UIMinorXMeshPlayer.txt" );

	//유닛 데이타 로드
	g_pData->ResetUnitManager( L"UnitTemplet.lua", L"NPCTemplet.lua", L"NPCStat.lua" );

	//아이템 데이타 로드
	g_pData->ResetItemManager();

	//월드 데이타 로드
	g_pData->ResetWorldManager( L"WorldData.lua" );	

	//던전 데이타 로드
	g_pData->ResetDungeonManager( L"DungeonData.lua" );




	const int MAGIC_USER_UID			= 777;

	// user data setup
    {
	    //CX2User::UserData* pUserData = new CX2User::UserData;
        CX2User::UserData kUserData;
	    {
		    kUserData.userUID			= MAGIC_USER_UID;
		    kUserData.userID			= L"WorldToolUserID";
		    kUserData.userAuthLevel	= CX2User::XUAL_DEV;
		    kUserData.IP				= L"59.25.186.1";
		    kUserData.port				= 8765;
	    }
	    g_pData->ResetMyUser( kUserData );
    }
	g_pData->ResetDungeonRoom();

}




bool CX2LineMapToolMain::OpenScriptFile( const WCHAR* pFileName )
{
	lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(), "g_pMain", this );

	KGCMassFileManager::CMassFile::MASSFILE_MEMBERFILEINFO_POINTER Info;
	Info = g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->LoadDataFile( pFileName );
	if( Info == NULL )
		return false;

	if( g_pKTDXApp->GetLuaBinder()->DoMemory( Info->pRealData, Info->size ) == E_FAIL )
		return false;

	return true;
}


void CX2LineMapToolMain::RegisterLuabind()
{

	lua_State* L= g_pKTDXApp->GetLuaBinder()->GetLuaState();



	lua_tinker::class_add<CX2Room>( L, "CX2Room" );
	lua_tinker::class_def<CX2Room>( L, "GetSlotData",				CX2Room::GetSlotData );
	lua_tinker::class_def<CX2Room>( L, "GetSlotNum",				CX2Room::GetSlotNum );
	lua_tinker::class_def<CX2Room>( L, "GetRoomPublic",				CX2Room::GetRoomPublic );
	lua_tinker::class_def<CX2Room>( L, "GetRoomState",				CX2Room::GetRoomState_LUA );
	lua_tinker::class_def<CX2Room>( L, "GetMySlot",					CX2Room::GetMySlot );
	;

	lua_tinker::class_add<CX2SlideShot>( L, "CX2SlideShot" );
	lua_tinker::class_def<CX2SlideShot>( L, "ScenStart_LUA",					CX2SlideShot::ScenStart_LUA);
	lua_tinker::class_def<CX2SlideShot>( L, "ScenWait",							CX2SlideShot::ScenWait);
	lua_tinker::class_def<CX2SlideShot>( L, "ScenEnd",							CX2SlideShot::ScenEnd);
	lua_tinker::class_def<CX2SlideShot>( L, "GoNextScen",						CX2SlideShot::GoNextScen);
	lua_tinker::class_def<CX2SlideShot>( L, "GetNowTime",						CX2SlideShot::GetNowTime);
	lua_tinker::class_def<CX2SlideShot>( L, "EventTimer",						CX2SlideShot::EventTimer);
	lua_tinker::class_def<CX2SlideShot>( L, "CannotInput",						CX2SlideShot::CannotInput );
	lua_tinker::class_def<CX2SlideShot>( L, "AddText_LUA",						CX2SlideShot::AddText_LUA );
	lua_tinker::class_def<CX2SlideShot>( L, "AddText2_LUA",						CX2SlideShot::AddText2_LUA);
	lua_tinker::class_def<CX2SlideShot>( L, "AddSeqMap_LUA",					CX2SlideShot::AddSeqMap_LUA);
	lua_tinker::class_def<CX2SlideShot>( L, "CrashSeq_LUA",						CX2SlideShot::CrashSeq_LUA);
	lua_tinker::class_def<CX2SlideShot>( L, "ChangeTex_LUA",					CX2SlideShot::ChangeTex_LUA);
	lua_tinker::class_def<CX2SlideShot>( L, "ChangeSize_LUA",					CX2SlideShot::ChangeSize_LUA);
	lua_tinker::class_def<CX2SlideShot>( L, "DeleteSeq_LUA",					CX2SlideShot::DeleteSeq_LUA);
	lua_tinker::class_def<CX2SlideShot>( L, "SetTextBoxShow",					CX2SlideShot::SetTextBoxShow);
	lua_tinker::class_def<CX2SlideShot>( L, "BlackLayOutMove",					CX2SlideShot::BlackLayOutMove );
	lua_tinker::class_def<CX2SlideShot>( L, "SetTextSpread",					CX2SlideShot::SetTextSpread);
	lua_tinker::class_def<CX2SlideShot>( L, "IsTextSpreading",					CX2SlideShot::IsTextSpreading );
	lua_tinker::class_def<CX2SlideShot>( L, "GetElapsedTimeAfterTextSpread",	CX2SlideShot::GetElapsedTimeAfterTextSpread );
	lua_tinker::class_def<CX2SlideShot>( L, "IsPresentNow",						CX2SlideShot::IsPresentNow);
	;



	lua_tinker::class_add<CX2TrainingGameUI>( L, "CX2TrainingGameUI" );
	lua_tinker::class_def<CX2TrainingGameUI>( L, "ScenStart_LUA",						CX2TrainingGameUI::ScenStart_LUA);
	lua_tinker::class_def<CX2TrainingGameUI>( L, "ScenWait",							CX2TrainingGameUI::ScenWait);
	lua_tinker::class_def<CX2TrainingGameUI>( L, "ScenEnd",								CX2TrainingGameUI::ScenEnd);
	lua_tinker::class_def<CX2TrainingGameUI>( L, "GoNextScen",							CX2TrainingGameUI::GoNextScen);
	lua_tinker::class_def<CX2TrainingGameUI>( L, "GetNowTime",							CX2TrainingGameUI::GetNowTime);
	lua_tinker::class_def<CX2TrainingGameUI>( L, "EventTimer",							CX2TrainingGameUI::EventTimer);
	lua_tinker::class_def<CX2TrainingGameUI>( L, "AddText_LUA",							CX2TrainingGameUI::AddText_LUA );
	lua_tinker::class_def<CX2TrainingGameUI>( L, "IsKeyPressed",						CX2TrainingGameUI::IsKeyPressed);

	lua_tinker::class_def<CX2TrainingGameUI>( L, "ClearWaitingKeyPress",				CX2TrainingGameUI::ClearWaitingKeyPress );
	lua_tinker::class_def<CX2TrainingGameUI>( L, "AddWaitingKeyPress",					CX2TrainingGameUI::AddWaitingKeyPress );
	lua_tinker::class_def<CX2TrainingGameUI>( L, "AddWaitingKeyPressScaled",			CX2TrainingGameUI::AddWaitingKeyPressScaled );

	lua_tinker::class_def<CX2TrainingGameUI>( L, "SetTextSpread",						CX2TrainingGameUI::SetTextSpread);
	lua_tinker::class_def<CX2TrainingGameUI>( L, "IsTextSpreading",						CX2TrainingGameUI::IsTextSpreading );
	lua_tinker::class_def<CX2TrainingGameUI>( L, "GetElapsedTimeAfterTextSpread",		CX2TrainingGameUI::GetElapsedTimeAfterTextSpread );

	lua_tinker::class_def<CX2TrainingGameUI>( L, "SetEnableWaitingUserState",			CX2TrainingGameUI::SetEnableWaitingUserState );
	lua_tinker::class_def<CX2TrainingGameUI>( L, "AddWaitingUserState_LUA",				CX2TrainingGameUI::AddWaitingUserState_LUA );
	lua_tinker::class_def<CX2TrainingGameUI>( L, "IsWaitingUserState",					CX2TrainingGameUI::IsWaitingUserState );
	lua_tinker::class_def<CX2TrainingGameUI>( L, "ClearWaitingUserState",				CX2TrainingGameUI::ClearWaitingUserState );
	lua_tinker::class_def<CX2TrainingGameUI>( L, "AddSimpleWaitingUserState",			CX2TrainingGameUI::AddSimpleWaitingUserState );
	lua_tinker::class_def<CX2TrainingGameUI>( L, "IsWaitingSimpleUserState",			CX2TrainingGameUI::IsWaitingSimpleUserState );
	lua_tinker::class_def<CX2TrainingGameUI>( L, "ClearSimpleWaitingUserState",			CX2TrainingGameUI::ClearSimpleWaitingUserState );
	lua_tinker::class_def<CX2TrainingGameUI>( L, "GetElapsedTimeAfterCompletingSimpleUserState",		CX2TrainingGameUI::GetElapsedTimeAfterCompletingSimpleUserState );

	lua_tinker::class_def<CX2TrainingGameUI>( L, "ClearGatherItem",						CX2TrainingGameUI::ClearGatherItem );
	lua_tinker::class_def<CX2TrainingGameUI>( L, "AddGatherItem_LUA",					CX2TrainingGameUI::AddGatherItem_LUA );
	lua_tinker::class_def<CX2TrainingGameUI>( L, "IsWaitingGatherItem",					CX2TrainingGameUI::IsWaitingGatherItem );
	lua_tinker::class_def<CX2TrainingGameUI>( L, "ClearWaitingSpecialAttack",			CX2TrainingGameUI::ClearWaitingSpecialAttack );
	lua_tinker::class_def<CX2TrainingGameUI>( L, "AddWaitingSpecialAttack_LUA",			CX2TrainingGameUI::AddWaitingSpecialAttack_LUA );
	lua_tinker::class_def<CX2TrainingGameUI>( L, "IsWaitingSpecialAttack",				CX2TrainingGameUI::IsWaitingSpecialAttack );

	lua_tinker::class_def<CX2TrainingGameUI>( L, "AddWaitingSeqUserState_LUA",			CX2TrainingGameUI::AddWaitingSeqUserState_LUA );
	lua_tinker::class_def<CX2TrainingGameUI>( L, "SetWaitingSeqUserState",				CX2TrainingGameUI::SetWaitingSeqUserState );
	lua_tinker::class_def<CX2TrainingGameUI>( L, "ClearWaitingSeqUserState",			CX2TrainingGameUI::ClearWaitingSeqUserState );
	lua_tinker::class_def<CX2TrainingGameUI>( L, "IsWaitingSeqUserState",				CX2TrainingGameUI::IsWaitingSeqUserState );
	lua_tinker::class_def<CX2TrainingGameUI>( L, "ClearKeySequence",					CX2TrainingGameUI::ClearKeySequence );

	lua_tinker::class_def<CX2TrainingGameUI>( L, "ClearTrainingObjective",				CX2TrainingGameUI::ClearTrainingObjective );
	lua_tinker::class_def<CX2TrainingGameUI>( L, "AddTrainingObjective",				CX2TrainingGameUI::AddTrainingObjective );
	lua_tinker::class_def<CX2TrainingGameUI>( L, "AddTrainingObjectivePicture",			CX2TrainingGameUI::AddTrainingObjectivePicture );
	lua_tinker::class_def<CX2TrainingGameUI>( L, "AddTrainingObjectivePictureScaled",	CX2TrainingGameUI::AddTrainingObjectivePictureScaled );

	lua_tinker::class_def<CX2TrainingGameUI>( L, "SetShowEnterMark",					CX2TrainingGameUI::SetShowEnterMark );
	lua_tinker::class_def<CX2TrainingGameUI>( L, "SetTwinkleEnterMark",					CX2TrainingGameUI::SetTwinkleEnterMark );
	lua_tinker::class_def<CX2TrainingGameUI>( L, "SetShowSuccessCountEffect",			CX2TrainingGameUI::SetShowSuccessCountEffect );

	lua_tinker::class_def<CX2TrainingGameUI>( L, "GetFTNPCID",							CX2TrainingGameUI::GetFTNPCID );
	lua_tinker::class_def<CX2TrainingGameUI>( L, "GetFTNPCLevel",						CX2TrainingGameUI::GetFTNPCLevel );
	lua_tinker::class_def<CX2TrainingGameUI>( L, "GetFTNPCCount",						CX2TrainingGameUI::GetFTNPCCount );
	lua_tinker::class_def<CX2TrainingGameUI>( L, "GetFTEnableNPCAttack",				CX2TrainingGameUI::GetFTEnableNPCAttack );
	;



	lua_tinker::class_add<CX2Room::SlotData>( L, "CX2Room::SlotData" );
	lua_tinker::class_def<CX2Room::SlotData>( L, "GetIsHost",				CX2Room::SlotData::GetIsHost );
	lua_tinker::class_def<CX2Room::SlotData>( L, "GetIsReady",				CX2Room::SlotData::GetIsReady );
	lua_tinker::class_def<CX2Room::SlotData>( L, "GetIsPitIn",				CX2Room::SlotData::GetIsPitIn );
	lua_tinker::class_def<CX2Room::SlotData>( L, "GetSlotState",			CX2Room::SlotData::GetSlotState );
	lua_tinker::class_def<CX2Room::SlotData>( L, "GetTeamNum",				CX2Room::SlotData::GetTeamNum );
	lua_tinker::class_def<CX2Room::SlotData>( L, "IsMyUnit",				CX2Room::SlotData::IsMyUnit );
	;

	lua_tinker::class_add<CX2User>( L, "CX2User" );
	lua_tinker::class_def<CX2User>( L, "GetUnitNum",		CX2User::GetUnitNum );
	lua_tinker::class_def<CX2User>( L, "GetUnitByIndex",	CX2User::GetUnitByIndex );
	;


	lua_tinker::class_add<CX2Unit>( L, "CX2Unit" );
	lua_tinker::class_def<CX2Unit>( L, "GetClass",			CX2Unit::GetClass );
	lua_tinker::class_def<CX2Unit>( L, "GetNickName",		CX2Unit::GetNickName );
	;




	lua_tinker::class_add<CX2PVPRoom>( L, "CX2PVPRoom" );
	lua_tinker::class_inh< CX2PVPRoom, CX2Room>( L );
	lua_tinker::class_def<CX2PVPRoom>( L, "GetPVPGameType",				CX2PVPRoom::GetPVPGameType_LUA );
	lua_tinker::class_def<CX2PVPRoom>( L, "GetWinKillNum",				CX2PVPRoom::GetWinKillNum );
	lua_tinker::class_def<CX2PVPRoom>( L, "GetPlayTime",				CX2PVPRoom::GetPlayTimeLimit );
	lua_tinker::class_def<CX2PVPRoom>( L, "GetCanIntrude",				CX2PVPRoom::GetCanIntrude );
	lua_tinker::class_def<CX2PVPRoom>( L, "GetWorldID",					CX2PVPRoom::GetWorldID );
	lua_tinker::class_def<CX2PVPRoom>( L, "SetPVPGameType",				CX2PVPRoom::SetPVPGameType );
	lua_tinker::class_def<CX2PVPRoom>( L, "SetWinKillNum",				CX2PVPRoom::SetWinKillNum );
	lua_tinker::class_def<CX2PVPRoom>( L, "SetPlayTime",				CX2PVPRoom::SetPlayTime );
	lua_tinker::class_def<CX2PVPRoom>( L, "SetCanIntrude",				CX2PVPRoom::SetCanIntrude );
	lua_tinker::class_def<CX2PVPRoom>( L, "SetWorldID",					CX2PVPRoom::SetWorldID );
	;

	lua_tinker::class_add<CX2DungeonRoom>( L, "CX2DungeonRoom" );
	lua_tinker::class_inh< CX2DungeonRoom, CX2Room>( L );
	lua_tinker::class_def<CX2DungeonRoom>( L, "GetDifficulty",				CX2DungeonRoom::GetDifficulty );
	lua_tinker::class_def<CX2DungeonRoom>( L, "GetDungeonID",				CX2DungeonRoom::GetDungeonID );
	lua_tinker::class_def<CX2DungeonRoom>( L, "SetDifficulty",				CX2DungeonRoom::SetDifficulty );
	lua_tinker::class_def<CX2DungeonRoom>( L, "SetDungeonID",				CX2DungeonRoom::SetDungeonID );
	;

	lua_tinker::class_add<CX2Data>( L, "CX2Data" );
	lua_tinker::class_def<CX2Data>( L, "GetPVPRoom",				CX2Data::GetPVPRoom );
	lua_tinker::class_def<CX2Data>( L, "GetDungeonRoom",			CX2Data::GetDungeonRoom );
	lua_tinker::class_def<CX2Data>( L, "GetMyUser",					CX2Data::GetMyUser );
	lua_tinker::class_def<CX2Data>( L, "GetUIMajorParticle",		CX2Data::GetUIMajorParticle );
	lua_tinker::class_def<CX2Data>( L, "GetUIMajorXMeshPlayer",		CX2Data::GetUIMajorXMeshPlayer );
	lua_tinker::class_def<CX2Data>( L, "GetUIMinorParticle",		CX2Data::GetUIMinorParticle );
	lua_tinker::class_def<CX2Data>( L, "GetUIMinorXMeshPlayer",		CX2Data::GetUIMinorXMeshPlayer );
	lua_tinker::class_def<CX2Data>( L, "GetUIUnitManager",			CX2Data::GetUIUnitManager );
	lua_tinker::class_def<CX2Data>( L, "LoadPicCharAll_LUA",		CX2Data::LoadPicCharAll_LUA );
	;


	lua_tinker::class_add<CX2State>( L, "CX2State" );
	lua_tinker::class_inh< CX2State, CKTDXStage>( L );
	lua_tinker::class_def<CX2State>( L, "Handler_EGS_NOTIFY_MSG_REQ",					CX2State::Handler_EGS_NOTIFY_MSG_REQ_LUA );
	lua_tinker::class_def<CX2State>( L, "Handler_EGS_ADMIN_MODIFY_UNIT_LEVEL_REQ",		CX2State::Handler_EGS_ADMIN_MODIFY_UNIT_LEVEL_REQ_LUA );
	lua_tinker::class_def<CX2State>( L, "Handler_EGS_ADMIN_CHANGE_ED_REQ",				CX2State::Handler_EGS_ADMIN_CHANGE_ED_REQ_LUA );
	lua_tinker::class_def<CX2State>( L, "Handler_EGS_ADMIN_KICK_USER_REQ",				CX2State::Handler_EGS_ADMIN_KICK_USER_REQ_LUA );
	lua_tinker::class_def<CX2State>( L, "Handler_EGS_ADMIN_SET_PC_BANG_REQ",			CX2State::Handler_EGS_ADMIN_SET_PC_BANG_REQ );
	;



	lua_tinker::class_add<CX2StatePVPRoom>( L, "CX2StatePVPRoom" );
	lua_tinker::class_inh< CX2StatePVPRoom, CX2StateMenu>( L );
	lua_tinker::class_def<CX2StatePVPRoom>( L, "Handler_EGS_CHANGE_SLOT_OPEN_REQ",		CX2StatePVPRoom::Handler_EGS_CHANGE_SLOT_OPEN_REQ );
	;

	lua_tinker::class_add<CX2Main>( L, "CX2Main" );

	//lua_tinker::class_def<CX2Main>( L, "SetLoadBalancerIP",				CX2Main::SetLoadBalancerIP_LUA );
	//lua_tinker::class_def<CX2Main>( L, "SetLoadBalancerPort",			CX2Main::SetLoadBalancerPort );
	//lua_tinker::class_def<CX2Main>( L, "AddServerIP_LUA",				CX2Main::AddServerIP_LUA );
	//lua_tinker::class_def<CX2Main>( L, "SetServerIP",					CX2Main::SetServerIP_LUA );
	//lua_tinker::class_def<CX2Main>( L, "SetPlannerServerIP_LUA",		CX2Main::SetPlannerServerIP_LUA );
	//lua_tinker::class_def<CX2Main>( L, "SetServerPort",					CX2Main::SetServerPort );

	lua_tinker::class_def<CX2Main>( L, "SetServerUDPPort",				CX2Main::SetServerUDPPort );
	lua_tinker::class_def<CX2Main>( L, "SerServerTimeOut",				CX2Main::SetServerTimeOut_LUA );
	lua_tinker::class_def<CX2Main>( L, "SetAutoLoginUnitIndex",			CX2Main::SetAutoLoginUnitIndex );
	lua_tinker::class_def<CX2Main>( L, "SetSafetyKey",					CX2Main::SetSafetyKey_LUA );
	lua_tinker::class_def<CX2Main>( L, "ResetFullScreenResolution",		CX2Main::ResetFullScreenResolution );
	lua_tinker::class_def<CX2Main>( L, "KTDGUIOKMsgBox",				CX2Main::KTDGUIOKMsgBox_LUA );
	lua_tinker::class_def<CX2Main>( L, "KTDGUIOkAndCancelMsgBox",		CX2Main::KTDGUIOkAndCancelMsgBox_LUA );
	lua_tinker::class_def<CX2Main>( L, "InstallFont",					CX2Main::InstallFont );
	lua_tinker::class_def<CX2Main>( L, "SetClientVersion_LUA",			CX2Main::SetClientVersion_LUA );
	lua_tinker::class_def<CX2Main>( L, "DeleteFile",					CX2Main::DeleteFile_LUA );
	lua_tinker::class_def<CX2Main>( L, "BuyItemSuper_LUA",				CX2Main::BuyItemSuper_LUA );
	lua_tinker::class_def<CX2Main>( L, "BuySuperEqipLevel_LUA",			CX2Main::BuySuperEqipLevel_LUA );
	lua_tinker::class_def<CX2Main>( L, "SetAuthLevel_LUA",				CX2Main::SetAuthLevel_LUA );
	lua_tinker::class_def<CX2Main>( L, "GetAuthUserList_LUA",			CX2Main::GetAuthUserList_LUA );
	lua_tinker::class_def<CX2Main>( L, "SetSubClientVersion_LUA",		CX2Main::SetSubClientVersion_LUA );
	lua_tinker::class_def<CX2Main>( L, "InitMyUnitSkillTree",			CX2Main::InitMyUnitSkillTree );
	lua_tinker::class_def<CX2Main>( L, "SetMyUnitSP",					CX2Main::SetMyUnitSP );
	lua_tinker::class_def<CX2Main>( L, "SendStatisticsLocLog",			CX2Main::SendStatisticsLocLog );
	lua_tinker::class_def<CX2Main>( L, "SendStatisticsDBLog",			CX2Main::SendStatisticsDBLog );
	lua_tinker::class_def<CX2Main>( L, "IntentionalCrash",				CX2Main::IntentionalCrash );
	lua_tinker::class_def<CX2Main>( L, "SetFreeCamera",					CX2Main::SetFreeCamera );
	//{{ 2008. 1. 2  최육사  근성도 치트
	lua_tinker::class_def<CX2Main>( L, "ChangeSpirit",					CX2Main::ChangeSpirit );
	lua_tinker::class_def<CX2Main>( L, "ChangeVSPoint",					CX2Main::ChangeVSPoint );
	//}}
	lua_tinker::class_def<CX2Main>( L, "SetShowUnitViewerUI",			CX2Main::SetShowUnitViewerUI );
	//{{ 2008. 2. 18  최육사  유닛 클래스 치트
	lua_tinker::class_def<CX2Main>( L, "ChangeUnitClass",				CX2Main::ChangeUnitClass );
	//}}
	lua_tinker::class_def<CX2Main>( L, "SetSquareMaxUserNum",			CX2Main::SetSquareMaxUserNum );

	lua_tinker::class_add<CX2QuestManager>( L, "CX2QuestManager" );
	lua_tinker::class_def<CX2QuestManager>( L, "AddQuestTemplet_LUA",			CX2QuestManager::AddQuestTemplet_LUA );
	lua_tinker::class_def<CX2QuestManager>( L, "AddSubQuestTemplet_LUA",		CX2QuestManager::AddSubQuestTemplet_LUA );
	;

	lua_tinker::class_add<CX2ItemManager>( L, "CX2ItemManager" );
	lua_tinker::class_def<CX2ItemManager>( L, "AddItemTemplet",				CX2ItemManager::AddItemTemplet_LUA );
	lua_tinker::class_def<CX2ItemManager>( L, "AddShopItemList_LUA",		CX2ItemManager::AddShopItemList_LUA );
	lua_tinker::class_def<CX2ItemManager>( L, "AddShopItemGroup_LUA",		CX2ItemManager::AddShopItemGroup_LUA );
	lua_tinker::class_def<CX2ItemManager>( L, "AddRandomItemTemplet",		CX2ItemManager::AddRandomItemTemplet_LUA );
	lua_tinker::class_def<CX2ItemManager>( L, "AddManufactureTemplet",		CX2ItemManager::AddManufactureTemplet_LUA );
	lua_tinker::class_def<CX2ItemManager>( L, "AddManufactureVillageData",	CX2ItemManager::AddManufactureVillageData_LUA );
	//{{ 2008. 4. 24  최육사  
	lua_tinker::class_def<CX2Game>( L, "AddPackageItemData",			CX2ItemManager::AddPackageItemData_LUA)
		//}}		
		;	

	lua_tinker::class_add<CX2UnitManager>( L, "CX2UnitManager" );
	lua_tinker::class_def<CX2UnitManager>( L, "AddUnitTemplet",			CX2UnitManager::AddUnitTemplet_LUA );
	lua_tinker::class_def<CX2UnitManager>( L, "AddNPCTemplet",			CX2UnitManager::AddNPCTemplet_LUA );
	lua_tinker::class_def<CX2UnitManager>( L, "AddNPCStat",				CX2UnitManager::AddNPCStat_LUA );


	lua_tinker::class_add<CX2UIUnitManager>( L, "CX2UIUnitManager" );
	lua_tinker::class_def<CX2UIUnitManager>( L, "ClearAllUIUnit",			CX2UIUnitManager::ClearAllUIUnit );
	;

	lua_tinker::class_add<CX2WorldManager>( L, "CX2WorldManager" );
	lua_tinker::class_def<CX2WorldManager>( L, "AddWorldData",			CX2WorldManager::AddWorldData_LUA );
	;

	lua_tinker::class_add<CX2World>( L, "CX2World" );
	lua_tinker::class_def<CX2World>( L, "SetOriginColor_LUA",			CX2World::SetOriginColor_LUA );
	lua_tinker::class_def<CX2World>( L, "SetLightColor_LUA",			CX2World::SetLightColor_LUA );
	lua_tinker::class_def<CX2World>( L, "SetLightPos",					CX2World::SetLightPos );
	lua_tinker::class_def<CX2World>( L, "SetLineMap",					CX2World::SetLineMap_LUA );
	lua_tinker::class_def<CX2World>( L, "SetBGM",						CX2World::SetBGM_LUA );
	lua_tinker::class_def<CX2World>( L, "PlayBGM",						CX2World::PlayBGM );
	lua_tinker::class_def<CX2World>( L, "AddEffectBGM_LUA",				CX2World::AddEffectBGM_LUA );
	lua_tinker::class_def<CX2World>( L, "CreateSkyDome",				CX2World::CreateSkyDome );
	lua_tinker::class_def<CX2World>( L, "CreateObjectMesh",				CX2World::CreateObjectMesh );
	lua_tinker::class_def<CX2World>( L, "CreateLensFlare",				CX2World::CreateLensFlare );
	lua_tinker::class_def<CX2World>( L, "CreateObjectSkinMesh",			CX2World::CreateObjectSkinMesh );
	lua_tinker::class_def<CX2World>( L, "CreateObjectLightFlow",		CX2World::CreateObjectLightFlow );
	lua_tinker::class_def<CX2World>( L, "CreateObjectParticle",			CX2World::CreateObjectParticle );
	lua_tinker::class_def<CX2World>( L, "SetWorldColor",				CX2World::SetWorldColor );
	lua_tinker::class_def<CX2World>( L, "GetRenderLineMap",				CX2World::GetRenderLineMap );
	lua_tinker::class_def<CX2World>( L, "SetRenderLineMap",				CX2World::SetRenderLineMap );
	;

	lua_tinker::class_add<CX2WorldObject>( L, "CX2WorldObject" );
	lua_tinker::class_inh< CX2WorldObject, CKTDGObject>( L );
	lua_tinker::class_def<CX2WorldObject>( L, "SetName",				CX2WorldObject::SetName );
	lua_tinker::class_def<CX2WorldObject>( L, "SetPos",					CX2WorldObject::SetPos );
	lua_tinker::class_def<CX2WorldObject>( L, "SetAngleDegree",			CX2WorldObject::SetAngleDegree );
	lua_tinker::class_def<CX2WorldObject>( L, "SetColorFactor",			CX2WorldObject::SetColorFactor );
	lua_tinker::class_def<CX2WorldObject>( L, "SetScale",				CX2WorldObject::SetScale );		
	lua_tinker::class_def<CX2WorldObject>( L, "SetUseWorldColor",		CX2WorldObject::SetUseWorldColor );		
	;

	lua_tinker::class_add<CX2WorldObjectMesh>( L, "CX2WorldObjectMesh" );
	lua_tinker::class_inh< CX2WorldObjectMesh, CX2WorldObject>( L );
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
	;

	lua_tinker::class_add<CX2WorldObjectSkinMesh>( L, "CX2WorldObjectSkinMesh" );
	lua_tinker::class_inh< CX2WorldObjectSkinMesh, CX2WorldObject>( L );
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
	;

	lua_tinker::class_add<CX2WorldObjectLightFlow>( L, "CX2WorldObjectLightFlow" );
	lua_tinker::class_inh< CX2WorldObjectLightFlow, CX2WorldObject>( L );
	lua_tinker::class_def<CX2WorldObjectLightFlow>( L, "SetXMesh",				CX2WorldObjectLightFlow::SetXMesh_LUA );
	lua_tinker::class_def<CX2WorldObjectLightFlow>( L, "SetFlowMin",			CX2WorldObjectLightFlow::SetFlowMin );
	lua_tinker::class_def<CX2WorldObjectLightFlow>( L, "SetFlowMax",			CX2WorldObjectLightFlow::SetFlowMax );
	lua_tinker::class_def<CX2WorldObjectLightFlow>( L, "SetFlowSpeed",			CX2WorldObjectLightFlow::SetFlowSpeed );
	lua_tinker::class_def<CX2WorldObjectLightFlow>( L, "SetWide",				CX2WorldObjectLightFlow::SetWide );
	lua_tinker::class_def<CX2WorldObjectLightFlow>( L, "SetNowPoint",			CX2WorldObjectLightFlow::SetNowPoint );
	lua_tinker::class_def<CX2WorldObjectLightFlow>( L, "SetRenderType",			CX2WorldObjectLightFlow::SetRenderType );
	lua_tinker::class_def<CX2WorldObjectLightFlow>( L, "GetRenderParam",		CX2WorldObjectLightFlow::GetRenderParam );
	;

	lua_tinker::class_add<CX2WorldObjectParticle>( L, "CX2WorldObjectParticle" );
	lua_tinker::class_inh< CX2WorldObjectParticle, CX2WorldObject>( L );
	lua_tinker::class_def<CX2WorldObjectParticle>( L, "SetCanHide",				CX2WorldObjectParticle::SetCanHide );
	lua_tinker::class_def<CX2WorldObjectParticle>( L, "SetParticlePos",			CX2WorldObjectParticle::SetParticlePos );
	lua_tinker::class_def<CX2WorldObjectParticle>( L, "SetEmitMinMax",			CX2WorldObjectParticle::SetEmitMinMax );
	lua_tinker::class_def<CX2WorldObjectParticle>( L, "SetTriggerCount",		CX2WorldObjectParticle::SetTriggerCount );
	lua_tinker::class_def<CX2WorldObjectParticle>( L, "SetTriggerTime",			CX2WorldObjectParticle::SetTriggerTime );
	lua_tinker::class_def<CX2WorldObjectParticle>( L, "SetDrawCount",			CX2WorldObjectParticle::SetDrawCount );
	lua_tinker::class_def<CX2WorldObjectParticle>( L, "SetCullCheck",			CX2WorldObjectParticle::SetCullCheck );
	lua_tinker::class_def<CX2WorldObjectParticle>( L, "SetParticleType",		CX2WorldObjectParticle::SetParticleType );
	;

	lua_tinker::class_add<CX2Dungeon>( L, "CX2Dungeon" );
	lua_tinker::class_def<CX2Dungeon>( L, "GetNowStage",						CX2Dungeon::GetNowStage );
	;

	lua_tinker::class_add<CX2DungeonStage>( L, "CX2DungeonStage" );
	lua_tinker::class_def<CX2DungeonStage>( L, "GetNowSubStage",				CX2DungeonStage::GetNowSubStage );
	lua_tinker::class_def<CX2DungeonStage>( L, "StartGoEffect",					CX2DungeonStage::StartGoEffect );
	lua_tinker::class_def<CX2DungeonStage>( L, "StopGoEffect",					CX2DungeonStage::StopGoEffect );
	lua_tinker::class_def<CX2DungeonStage>( L, "IsGoEffectOn",					CX2DungeonStage::IsGoEffectOn );
	;

	lua_tinker::class_add<CX2DungeonSubStage>( L, "CX2DungeonSubStage" );
	lua_tinker::class_def<CX2DungeonSubStage>( L, "ClearSubStage",				CX2DungeonSubStage::ClearSubStage );
	lua_tinker::class_def<CX2DungeonSubStage>( L, "GetWinWhenTimeOut",			CX2DungeonSubStage::GetWinWhenTimeOut );
	lua_tinker::class_def<CX2DungeonSubStage>( L, "SetWinWhenTimeOut",			CX2DungeonSubStage::SetWinWhenTimeOut );
	lua_tinker::class_def<CX2DungeonSubStage>( L, "GetStarted",					CX2DungeonSubStage::GetStarted );
	;


	lua_tinker::class_add<CX2DungeonManager>( L, "CX2DungeonManager" );
	lua_tinker::class_def<CX2DungeonManager>( L, "AddDungeonData",				CX2DungeonManager::AddDungeonData_LUA );
	;

	lua_tinker::class_add<CX2Camera>( L, "CX2Camera" );
	lua_tinker::class_def<CX2Camera>( L, "NomalTrackingCamera_LUA",				CX2Camera::NomalTrackingCamera_LUA );
	lua_tinker::class_def<CX2Camera>( L, "PartsLookTrackingCamera_LUA",			CX2Camera::PartsLookTrackingCamera_LUA );
	lua_tinker::class_def<CX2Camera>( L, "NomalDirectCamera_LUA",				CX2Camera::NomalDirectCamera_LUA );
	lua_tinker::class_def<CX2Camera>( L, "PartsLookDirectCamera_LUA",			CX2Camera::PartsLookDirectCamera_LUA );
	lua_tinker::class_def<CX2Camera>( L, "GetCamera",							CX2Camera::GetCamera );
	;

	lua_tinker::class_add<CX2Game>( L, "CX2Game" );
	lua_tinker::class_def<CX2Game>( L, "IsHost",								CX2Game::IsHost );
	lua_tinker::class_def<CX2Game>( L, "GetMajorXMeshPlayer",					CX2Game::GetMajorXMeshPlayer );
	lua_tinker::class_def<CX2Game>( L, "GetMinorXMeshPlayer",					CX2Game::GetMinorXMeshPlayer );
	lua_tinker::class_def<CX2Game>( L, "GetMajorParticle",						CX2Game::GetMajorParticle );
	lua_tinker::class_def<CX2Game>( L, "GetMinorParticle",						CX2Game::GetMinorParticle );
	lua_tinker::class_def<CX2Game>( L, "GetDamageEffect",						CX2Game::GetDamageEffect );
	lua_tinker::class_def<CX2Game>( L, "GetX2Camera",							CX2Game::GetX2Camera );
	lua_tinker::class_def<CX2Game>( L, "GetFocusUnitPos_LUA",					CX2Game::GetFocusUnitPos_LUA );
	lua_tinker::class_def<CX2Game>( L, "LiveUserUnitNum",						CX2Game::LiveUserUnitNum );
	lua_tinker::class_def<CX2Game>( L, "LiveActiveNPCNum",						CX2Game::LiveActiveNPCNum );
	lua_tinker::class_def<CX2Game>( L, "LiveNPCNumType_LUA",					CX2Game::LiveNPCNumType_LUA );
	lua_tinker::class_def<CX2Game>( L, "IsKillNPCKeyCode_LUA",					CX2Game::IsKillNPCKeyCode_LUA );
	lua_tinker::class_def<CX2Game>( L, "LiveKeyCodeNPCNum",						CX2Game::LiveKeyCodeNPCNum );
	lua_tinker::class_def<CX2Game>( L, "GetKeyCodeNPC",							CX2Game::GetKeyCodeNPC );
	lua_tinker::class_def<CX2Game>( L, "SetWorldColor_LUA",						CX2Game::SetWorldColor_LUA );
	lua_tinker::class_def<CX2Game>( L, "GetLineMap",							CX2Game::GetLineMap );
	lua_tinker::class_def<CX2Game>( L, "CreateNPCReq_LUA",						CX2Game::CreateNPCReq_LUA );
	lua_tinker::class_def<CX2Game>( L, "PushCreateNPCReq_Lua",					CX2Game::PushCreateNPCReq_Lua );
	lua_tinker::class_def<CX2Game>( L, "FlushCreateNPCReq",						CX2Game::FlushCreateNPCReq );
	lua_tinker::class_def<CX2Game>( L, "GetMyUnit",								CX2Game::GetMyUnit );
	lua_tinker::class_def<CX2Game>( L, "IsKeyPressed",							CX2Game::IsKeyPressed );
	lua_tinker::class_def<CX2Game>( L, "IsSkipKeyPressed",						CX2Game::IsSkipKeyPressed );
	lua_tinker::class_def<CX2Game>( L, "GetGameTime",							CX2Game::GetGameTime );
	lua_tinker::class_def<CX2Game>( L, "GetElapsedTimeAfterLastKeyboardInput",	CX2Game::GetElapsedTimeAfterLastKeyboardInput );
	lua_tinker::class_def<CX2Game>( L, "PauseNPCAI",							CX2Game::PauseNPCAI );
	lua_tinker::class_def<CX2Game>( L, "KillAllNPC_LUA",						CX2Game::KillAllNPC_LUA );
	lua_tinker::class_def<CX2Game>( L, "KillNPC",								CX2Game::KillNPC );
	lua_tinker::class_def<CX2Game>( L, "EnableAllNPCAI",						CX2Game::EnableAllNPCAI );
	lua_tinker::class_def<CX2Game>( L, "SetFreeCamera",							CX2Game::SetFreeCamera);
	lua_tinker::class_def<CX2Game>( L, "GoStage",								CX2Game::GoStage);
	lua_tinker::class_def<CX2Game>( L, "GetWorld",								CX2Game::GetWorld);
	lua_tinker::class_def<CX2Game>( L, "SetShowAllUnitGageBar",					CX2Game::SetShowAllUnitGageBar);
	lua_tinker::class_def<CX2Game>( L, "GetUnitNum",							CX2Game::GetUnitNum);
	lua_tinker::class_def<CX2Game>( L, "GetUnit",								CX2Game::GetUnit);
	lua_tinker::class_def<CX2Game>( L, "GetNPCUnitListSize",					CX2Game::GetNPCUnitListSize);
	lua_tinker::class_def<CX2Game>( L, "GetNPCUnit",							CX2Game::GetNPCUnit);
	lua_tinker::class_def<CX2Game>( L, "GetNPCUnitByKeyCode",					CX2Game::GetNPCUnitByKeyCode);
	lua_tinker::class_def<CX2Game>( L, "GetGameLevel",							CX2Game::GetGameLevel);
	lua_tinker::class_def<CX2Game>( L, "GetSlideShot",							CX2Game::GetSlideShot);
	lua_tinker::class_def<CX2Game>( L, "SetEnableAllNPCConditionTable",			CX2Game::SetEnableAllNPCConditionTable);
	lua_tinker::class_def<CX2Game>( L, "EnableAllNPCEventProcess",				CX2Game::EnableAllNPCEventProcess);
	lua_tinker::class_def<CX2Game>( L, "SetEnableAllKeyProcess",				CX2Game::SetEnableAllKeyProcess);
	lua_tinker::class_def<CX2Game>( L, "SetEnableCommandKeyProcess",			CX2Game::SetEnableCommandKeyProcess);
	lua_tinker::class_def<CX2Game>( L, "GetDropItemManager",					CX2Game::GetDropItemManager);
	lua_tinker::class_def<CX2Game>( L, "GetNPCStartPos",						CX2Game::GetNPCStartPos);
	lua_tinker::class_def<CX2Game>( L, "AnyUserUnitInRange",					CX2Game::AnyUserUnitInRange);
	lua_tinker::class_def<CX2Game>( L, "GetNearNPCUnitUIDInStart_LUA",			CX2Game::GetNearNPCUnitUIDInStart_LUA);
	lua_tinker::class_def<CX2Game>( L, "GetNearNPCUnitUID_LUA",	        		CX2Game::GetNearNPCUnitUID_LUA);
	lua_tinker::class_def<CX2Game>( L, "ChangeRebirthStateNPCUnit_LUA",			CX2Game::ChangeRebirthStateNPCUnit_LUA);
	lua_tinker::class_def<CX2Game>( L, "GetEmptyStartPos",						CX2Game::GetEmptyStartPos);
	lua_tinker::class_def<CX2Game>( L, "SetAllNPCMPFull",						CX2Game::SetAllNPCMPFull);


	lua_tinker::class_add<CX2DungeonGame>( L, "CX2DungeonGame" );
	lua_tinker::class_inh< CX2DungeonGame, CX2Game>( L );
	lua_tinker::class_def<CX2DungeonGame>( L, "TextOutTutorial",					CX2DungeonGame::TextOutTutorial_LUA );
	lua_tinker::class_def<CX2DungeonGame>( L, "GetIsThereTutorialMessage",			CX2DungeonGame::GetIsThereTutorialMessage );
	lua_tinker::class_def<CX2DungeonGame>( L, "IsTutorialMessage",					CX2DungeonGame::IsTutorialMessage_LUA );
	lua_tinker::class_def<CX2DungeonGame>( L, "GetDungeon",							CX2DungeonGame::GetDungeon );
	lua_tinker::class_def<CX2DungeonGame>( L, "GetGameLevel",						CX2DungeonGame::GetGameLevel );
	lua_tinker::class_def<CX2DungeonGame>( L, "ClearSubStage",						CX2DungeonGame::ClearSubStage );
	lua_tinker::class_def<CX2DungeonGame>( L, "GetStartRealTraining",				CX2DungeonGame::GetStartRealTraining );
	lua_tinker::class_def<CX2DungeonGame>( L, "SetStartRealTraining",				CX2DungeonGame::SetStartRealTraining );
	lua_tinker::class_def<CX2DungeonGame>( L, "GetRemainPlayTime",					CX2DungeonGame::GetRemainPlayTime );
	lua_tinker::class_def<CX2DungeonGame>( L, "GetTrainingGameUI",					CX2DungeonGame::GetTrainingGameUI );
	lua_tinker::class_def<CX2DungeonGame>( L, "SetRemainPlayTime",					CX2DungeonGame::SetRemainPlayTime );
	lua_tinker::class_def<CX2DungeonGame>( L, "ShowBossName_LUA",					CX2DungeonGame::ShowBossName_LUA );
	lua_tinker::class_def<CX2DungeonGame>( L, "ResetUnitPositionAtTrainingGame",	CX2DungeonGame::ResetUnitPositionAtTrainingGame );



	lua_tinker::class_add<CX2SquareGame>( L, "CX2SquareGame" );
	lua_tinker::class_def<CX2SquareGame>( L, "SetFreeCamera",				CX2DungeonGame::SetFreeCamera );
	;

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
	;


	lua_tinker::class_add<CX2GUNPC>( L, "CX2GUNPC" );
	lua_tinker::class_inh< CX2GUNPC, CX2GameUnit>( L );
	lua_tinker::class_def<CX2Game>( L, "AnimEventTimer_LUA",			CX2GUNPC::AnimEventTimer_LUA);
	lua_tinker::class_def<CX2Game>( L, "AnimTimer_LUA",					CX2GUNPC::AnimTimer_LUA);
	lua_tinker::class_def<CX2Game>( L, "GetNowAnimTime_LUA",			CX2GUNPC::GetNowAnimTime_LUA);
	lua_tinker::class_def<CX2Game>( L, "EventTimer",					CX2GUNPC::EventTimer);
	lua_tinker::class_def<CX2Game>( L, "GetLandPosition_LUA",			CX2GUNPC::GetLandPosition_LUA);
	lua_tinker::class_def<CX2Game>( L, "SetPosition",					CX2GUNPC::SetPosition);
	lua_tinker::class_def<CX2Game>( L, "GetPos",						CX2GUNPC::GetPos);
	lua_tinker::class_def<CX2Game>( L, "GetRotateDegree",				CX2GUNPC::GetRotateDegree);
	lua_tinker::class_def<CX2Game>( L, "GetXRotateDegree",				CX2GUNPC::GetXRotateDegree);
	lua_tinker::class_def<CX2Game>( L, "GetYRotateDegree",				CX2GUNPC::GetYRotateDegree);
	lua_tinker::class_def<CX2Game>( L, "GetZRotateDegree",				CX2GUNPC::GetZRotateDegree);
	lua_tinker::class_def<CX2Game>( L, "SetIsRight",					CX2GUNPC::SetIsRight);        
	lua_tinker::class_def<CX2Game>( L, "GetIsRight",					CX2GUNPC::GetIsRight);
	lua_tinker::class_def<CX2Game>( L, "GetDirPos_LUA",					CX2GUNPC::GetDirPos_LUA);        
	lua_tinker::class_def<CX2Game>( L, "SetAddRotateDegree_LUA",		CX2GUNPC::SetAddRotateDegree_LUA);
	//lua_tinker::class_def<CX2Game>( L, "GetXSkinAnim",					CX2GUNPC::GetXSkinAnim);
	lua_tinker::class_def<CX2Game>( L, "GetBonePos_LUA",				CX2GUNPC::GetBonePos_LUA);
	lua_tinker::class_def<CX2Game>( L, "GetWeaponBonePos_LUA",			CX2GUNPC::GetWeaponBonePos_LUA);
	lua_tinker::class_def<CX2Game>( L, "AttachWeaponParticle_LUA",		CX2GUNPC::AttachWeaponParticle_LUA);
	lua_tinker::class_def<CX2Game>( L, "GetUnitCondition_bHit",			CX2GUNPC::GetUnitCondition_bHit);
	lua_tinker::class_def<CX2Game>( L, "SetKeyCode",					CX2GUNPC::SetKeyCode);
	lua_tinker::class_def<CX2Game>( L, "GetKeyCode",					CX2GUNPC::GetKeyCode);
	lua_tinker::class_def<CX2Game>( L, "SetFlag_LUA",					CX2GUNPC::SetFlag_LUA);
	lua_tinker::class_def<CX2Game>( L, "GetFlag_LUA",					CX2GUNPC::GetFlag_LUA);
	lua_tinker::class_def<CX2Game>( L, "SetInt_LUA",					CX2GUNPC::SetInt_LUA);
	lua_tinker::class_def<CX2Game>( L, "GetInt_LUA",					CX2GUNPC::GetInt_LUA);
	lua_tinker::class_def<CX2Game>( L, "SetVector_LUA",					CX2GUNPC::SetVector_LUA);
	lua_tinker::class_def<CX2Game>( L, "GetVector_LUA",					CX2GUNPC::GetVector_LUA);
	lua_tinker::class_def<CX2Game>( L, "SetAIEnable",					CX2GUNPC::SetAIEnable);
	lua_tinker::class_def<CX2Game>( L, "SetMajorParticle_LUA",			CX2GUNPC::SetMajorParticle_LUA);
	lua_tinker::class_def<CX2Game>( L, "SetMinorParticle_LUA",			CX2GUNPC::SetMinorParticle_LUA);
	lua_tinker::class_def<CX2Game>( L, "GetMajorParticle_LUA",			CX2GUNPC::GetMajorParticle_LUA);
	lua_tinker::class_def<CX2Game>( L, "GetMinorParticle_LUA",			CX2GUNPC::GetMinorParticle_LUA);
	lua_tinker::class_def<CX2Game>( L, "ClearMinorParticle_LUA",		CX2GUNPC::ClearMinorParticle_LUA);
	lua_tinker::class_def<CX2Game>( L, "ClearMajorParticle_LUA",		CX2GUNPC::ClearMajorParticle_LUA);

	lua_tinker::class_def<CX2Game>( L, "GetDamageEffect_LUA",			CX2GUNPC::GetDamageEffect_LUA);
	lua_tinker::class_def<CX2Game>( L, "SetDamageEffect_LUA",			CX2GUNPC::SetDamageEffect_LUA);
	lua_tinker::class_def<CX2Game>( L, "ClearDamageEffect_LUA",			CX2GUNPC::ClearDamageEffect_LUA);

	lua_tinker::class_def<CX2Game>( L, "SetInvincibleTime",				CX2GUNPC::SetInvincibleTime);
	lua_tinker::class_def<CX2Game>( L, "SetSuperArmor",					CX2GUNPC::SetSuperArmorStateParam);
	lua_tinker::class_def<CX2Game>( L, "CallOtherNPC",					CX2GUNPC::CallOtherNPC);
	lua_tinker::class_def<CX2Game>( L, "GetNowAnimationTime",			CX2GUNPC::GetNowAnimationTime);
	lua_tinker::class_def<CX2Game>( L, "SetPassiveSpeed",				CX2GUNPC::SetPassiveSpeed);
	lua_tinker::class_def<CX2Game>( L, "SetSpeedX",						CX2GUNPC::SetSpeedX);
	lua_tinker::class_def<CX2Game>( L, "SetSpeedY",						CX2GUNPC::SetSpeedY);
	lua_tinker::class_def<CX2Game>( L, "GetSpeedY",						CX2GUNPC::GetSpeedY);
	lua_tinker::class_def<CX2Game>( L, "GetDamagedThisFrame",			CX2GUNPC::GetDamagedThisFrame);
	lua_tinker::class_def<CX2Game>( L, "HasTarget",						CX2GUNPC::HasTarget);
	lua_tinker::class_def<CX2Game>( L, "SetDieSeq",						CX2GUNPC::SetDieSeq);
	lua_tinker::class_def<CX2Game>( L, "GetHardLevel",					CX2GUNPC::GetHardLevel);		
	lua_tinker::class_def<CX2Game>( L, "GetBoneRotateDegree_LUA",		CX2GUNPC::GetBoneRotateDegree_LUA);
	lua_tinker::class_def<CX2Game>( L, "GetHyperModeCount",				CX2GUNPC::GetHyperModeCount);
	lua_tinker::class_def<CX2Game>( L, "ChangeHyperMode_LUA",			CX2GUNPC::ChangeHyperMode_LUA);
	lua_tinker::class_def<CX2Game>( L, "SetLockOnTarget",				CX2GUNPC::SetLockOnTarget);
	lua_tinker::class_def<CX2Game>( L, "SetLockOn",						CX2GUNPC::SetLockOn);
	lua_tinker::class_def<CX2Game>( L, "SetLockOnPos",					CX2GUNPC::SetLockOnPos);
	lua_tinker::class_def<CX2Game>( L, "SetSiegeMode",					CX2GUNPC::SetSiegeMode);
	lua_tinker::class_def<CX2Game>( L, "ReleaseSiegeMode",				CX2GUNPC::ReleaseSiegeMode);
	lua_tinker::class_def<CX2Game>( L, "GetUID",						CX2GUNPC::GetUID);
	lua_tinker::class_def<CX2Game>( L, "ClearEventCheck",				CX2GUNPC::ClearEventCheck);
	lua_tinker::class_def<CX2Game>( L, "GetFollowHitter",				CX2GUNPC::GetFollowHitter);

	lua_tinker::class_def<CX2Game>( L, "GetLineGroupStartPos",			CX2GUNPC::GetLineGroupStartPos);
	lua_tinker::class_def<CX2Game>( L, "GetLineGroupEndPos",			CX2GUNPC::GetLineGroupEndPos);
	lua_tinker::class_def<CX2Game>( L, "GetStateTime",					CX2GUNPC::GetStateTime);
	lua_tinker::class_def<CX2Game>( L, "GetZVector",					CX2GUNPC::GetZVector);
	lua_tinker::class_def<CX2Game>( L, "GetNowHP_LUA",					CX2GUNPC::GetNowHP_LUA);
	lua_tinker::class_def<CX2Game>( L, "SetNowHP_LUA",					CX2GUNPC::SetNowHP_LUA);

	
	//lua_tinker::class_def<CX2GUNPC>( L, "SetShow",						&CX2GUNPC::SetShow);
	lua_tinker::class_def<CX2GUNPC>( L, "SetShow",						CX2GUNPC::SetShowObject);
	
	lua_tinker::class_def<CX2Game>( L, "GetAITargetPos",				CX2GUNPC::GetAITargetPos);
	lua_tinker::class_def<CX2Game>( L, "SetEnableLuaTargetingFunc",		CX2GUNPC::SetEnableLuaTargetingFunc);
	lua_tinker::class_def<CX2Game>( L, "SetLuaTargetingFunc",			CX2GUNPC::SetLuaTargetingFunc);
	lua_tinker::class_def<CX2Game>( L, "SetTargetUnitUID",				CX2GUNPC::SetTargetUnitUID);
	lua_tinker::class_def<CX2Game>( L, "SetTargetUnitType",				CX2GUNPC::SetTargetUnitType);
	lua_tinker::class_def<CX2Game>( L, "GetNearestUserPos",				CX2GUNPC::GetNearestUserPos);
	lua_tinker::class_def<CX2Game>( L, "IsUnitID",			        	CX2GUNPC::IsUnitID);        

	lua_tinker::class_def<CX2Game>( L, "GetStartState",					CX2GUNPC::GetStartState);
	lua_tinker::class_def<CX2Game>( L, "GetDisabledStateID",			CX2GUNPC::GetDisabledStateID);
	lua_tinker::class_def<CX2Game>( L, "StateChangeForce",				CX2GUNPC::StateChangeForce);

	lua_tinker::class_def<CX2Game>( L, "SetRageForce",					CX2GUNPC::SetRageForce);
	lua_tinker::class_def<CX2Game>( L, "GetNowStateID",					CX2GUNPC::GetNowStateID);

	lua_tinker::class_def<CX2Game>( L, "SetWeaponEnable",				CX2GUNPC::SetWeaponEnable);
	lua_tinker::class_def<CX2Game>( L, "StateChange_LUA",				CX2GUNPC::StateChange_LUA);



	lua_tinker::class_add<CX2GUUser>( L, "CX2GUUser" );
	lua_tinker::class_inh< CX2GUUser, CX2GameUnit>( L );
	lua_tinker::class_def<CX2GUUser>( L, "GetHyperModeUsed",			CX2GUUser::GetHyperModeUsed );
	lua_tinker::class_def<CX2GUUser>( L, "SetDevAddDamage",				CX2GUUser::SetDevAddDamage );
	lua_tinker::class_def<CX2GUUser>( L, "ForceChangeHyperMode",		CX2GUUser::ForceChangeHyperMode );
	lua_tinker::class_def<CX2GUUser>( L, "GetNowSpecialAttack",			CX2GUUser::GetNowSpecialAttack );
	lua_tinker::class_def<CX2GUUser>( L, "GetHoldSpecialAttack",		CX2GUUser::GetHoldSpecialAttack );
	lua_tinker::class_def<CX2GUUser>( L, "GetHoldingSpecialAttack",		CX2GUUser::GetHoldingSpecialAttack );
	lua_tinker::class_def<CX2GUUser>( L, "GetAttackSuccess",			CX2GUUser::GetAttackSuccess );
	lua_tinker::class_def<CX2GUUser>( L, "GetRotateDegree",				CX2GUUser::GetRotateDegree );
	;



	lua_tinker::class_add<CX2DamageEffect>( L, "CX2DamageEffect" );
	lua_tinker::class_def<CX2DamageEffect>( L, "CreateInstance_LUA",			CX2DamageEffect::CreateInstance_LUA );
	lua_tinker::class_def<CX2DamageEffect>( L, "CreateInstance_LUA2",			CX2DamageEffect::CreateInstance_LUA2 );
	lua_tinker::class_def<CX2DamageEffect>( L, "IsLiveInstance",				CX2DamageEffect::IsLiveInstance );
	lua_tinker::class_def<CX2DamageEffect>( L, "DestroyInstance",				CX2DamageEffect::DestroyInstance );
	;

	lua_tinker::class_add<CX2GameEdit>( L, "CX2GameEdit" );
	lua_tinker::class_def<CX2GameEdit>( L, "AddCommandChange",			CX2GameEdit::AddCommandChange );
	lua_tinker::class_def<CX2GameEdit>( L, "AddString",					CX2GameEdit::AddString_LUA );
	lua_tinker::class_def<CX2GameEdit>( L, "SetCommandListHelperText",	CX2GameEdit::SetCommandListHelperText );
	lua_tinker::class_def<CX2GameEdit>( L, "ToggleCommandList",			CX2GameEdit::ToggleCommandList );
	lua_tinker::class_def<CX2GameEdit>( L, "SaveFileLoadedDevices",		CX2GameEdit::SaveFileLoadedDevices  );
	lua_tinker::class_def<CX2GameEdit>( L, "ChangeSpirit",				CX2GameEdit::SaveFileLoadedDevices  );
	;

	lua_tinker::class_add<CX2TextManager::CX2Text>( L, "CX2Text" );
	lua_tinker::class_def<CX2TextManager::CX2Text>( L, "Move",				CX2TextManager::CX2Text::Move_LUA );
	;

	lua_tinker::class_add<CX2TextManager>( L, "CX2TextManager" );
	lua_tinker::class_def<CX2TextManager>( L, "AddText",					CX2TextManager::AddText_LUA );
	;

	lua_tinker::class_add<CX2SlotManager>( L, "CX2SlotManager" );
	lua_tinker::class_def<CX2SlotManager>( L, "AddSlot",					CX2SlotManager::AddSlot );
	lua_tinker::class_def<CX2SlotManager>( L, "CreateItemSlot",				CX2SlotManager::CreateItemSlot );
	;

	lua_tinker::class_add<CX2CouponBox>( L, "CX2CouponBox" );
	lua_tinker::class_inh< CX2CouponBox, CX2SlotManager>( L );
	;

	//lua_tinker::class_add<CX2PersonalTrade>( L, "CX2PersonalTrade" );
	//lua_tinker::class_inh< CX2PersonalTrade, CX2SlotManager>( L );
	//;

	//lua_tinker::class_add<CX2PersonalShop>( L, "CX2PersonalShop" );
	//lua_tinker::class_inh< CX2PersonalShop, CX2SlotManager>( L );
	//;

	//lua_tinker::class_add<CX2CharacterRoom>( L, "CX2CharacterRoom" );
	//lua_tinker::class_inh< CX2CharacterRoom, CX2SlotManager>( L );
	//;

	//lua_tinker::class_add<CX2NewQuestUI>( L, "CX2NewQuestUI" );
	//lua_tinker::class_inh< CX2NewQuestUI, CX2SlotManager>( L );
	//;

	lua_tinker::class_add<CX2SlotManager::CX2Slot>( L, "CX2Slot" );
	lua_tinker::class_def<CX2SlotManager::CX2Slot>( L, "SetSlotData",					CX2SlotManager::CX2Slot::SetSlotData_LUA );
	lua_tinker::class_def<CX2SlotManager::CX2Slot>( L, "SetClickable",				CX2SlotManager::CX2Slot::SetClickable );
	;

	lua_tinker::class_add<CX2SlotItem>( L, "CX2SlotItem" );
	lua_tinker::class_inh< CX2SlotItem, CX2SlotManager::CX2Slot>( L );
	;

	//lua_tinker::class_add<CX2ShopInventory>( L, "CX2ShopInventory" );
	//lua_tinker::class_inh< CX2ShopInventory, CX2SlotManager>( L );
	//;

	//lua_tinker::class_add<CX2ShopCharStatus>( L, "CX2ShopCharStatus" );
	//lua_tinker::class_inh< CX2ShopCharStatus, CX2SlotManager>( L );
	//;

	lua_tinker::class_add<CX2SquareUnit>( L, "CX2SquareUnit" );
	lua_tinker::class_def<CX2SquareUnit>( L, "StateChange_LUA",				CX2SquareUnit::StateChange_LUA );
	lua_tinker::class_def<CX2SquareUnit>( L, "SetRight_LUA",				CX2SquareUnit::SetRight_LUA );
	lua_tinker::class_def<CX2SquareUnit>( L, "GetPureLeft_LUA",				CX2SquareUnit::GetPureLeft_LUA );
	lua_tinker::class_def<CX2SquareUnit>( L, "GetOneLeft_LUA",				CX2SquareUnit::GetOneLeft_LUA );
	lua_tinker::class_def<CX2SquareUnit>( L, "GetPureDoubleLeft_LUA",		CX2SquareUnit::GetPureDoubleLeft_LUA );
	lua_tinker::class_def<CX2SquareUnit>( L, "GetOneDoubleLeft_LUA",		CX2SquareUnit::GetOneDoubleLeft_LUA );
	lua_tinker::class_def<CX2SquareUnit>( L, "GetPureRight_LUA",			CX2SquareUnit::GetPureRight_LUA );
	lua_tinker::class_def<CX2SquareUnit>( L, "GetOneRight_LUA",				CX2SquareUnit::GetOneRight_LUA );
	lua_tinker::class_def<CX2SquareUnit>( L, "GetPureDoubleRight_LUA",		CX2SquareUnit::GetPureDoubleRight_LUA );
	lua_tinker::class_def<CX2SquareUnit>( L, "GetOneDoubleRight_LUA",		CX2SquareUnit::GetOneDoubleRight_LUA );
	lua_tinker::class_def<CX2SquareUnit>( L, "GetPureUp_LUA",				CX2SquareUnit::GetPureUp_LUA );
	lua_tinker::class_def<CX2SquareUnit>( L, "GetOneUp_LUA",				CX2SquareUnit::GetOneUp_LUA );
	lua_tinker::class_def<CX2SquareUnit>( L, "GetPureDoubleUp_LUA",			CX2SquareUnit::GetPureDoubleUp_LUA );
	lua_tinker::class_def<CX2SquareUnit>( L, "GetOneDoubleUp_LUA",			CX2SquareUnit::GetOneDoubleUp_LUA );
	lua_tinker::class_def<CX2SquareUnit>( L, "GetPureDown_LUA",				CX2SquareUnit::GetPureDown_LUA );
	lua_tinker::class_def<CX2SquareUnit>( L, "GetOneDown_LUA",				CX2SquareUnit::GetOneDown_LUA );
	lua_tinker::class_def<CX2SquareUnit>( L, "GetPureDoubleDown_LUA",		CX2SquareUnit::GetPureDoubleDown_LUA );
	lua_tinker::class_def<CX2SquareUnit>( L, "GetOneDoubleDown_LUA",		CX2SquareUnit::GetOneDoubleDown_LUA );
	lua_tinker::class_def<CX2SquareUnit>( L, "GetFootOnLine_LUA",			CX2SquareUnit::GetFootOnLine_LUA );
	lua_tinker::class_def<CX2SquareUnit>( L, "CanDown_LUA",					CX2SquareUnit::CanDown_LUA );
	lua_tinker::class_def<CX2SquareUnit>( L, "AddPosY_LUA",					CX2SquareUnit::AddPosY_LUA );
	lua_tinker::class_def<CX2SquareUnit>( L, "GetYSpeed_LUA",				CX2SquareUnit::GetYSpeed_LUA );
	lua_tinker::class_def<CX2SquareUnit>( L, "SetXSpeed_LUA",				CX2SquareUnit::SetXSpeed_LUA );
	lua_tinker::class_def<CX2SquareUnit>( L, "SetYSpeed_LUA",				CX2SquareUnit::SetYSpeed_LUA );
	;

	lua_tinker::class_add<CX2SkillTree>( L, "CX2SkillTree" );
	lua_tinker::class_def<CX2SkillTree>( L, "AddSkillTemplet_LUA",			CX2SkillTree::AddSkillTemplet_LUA );
	//lua_tinker::class_def<CX2SkillTree>( L, "SetGridSize",					CX2SkillTree::SetGridSize );
	//lua_tinker::class_def<CX2Game>( L, "SetCanNotResetSkill",			CX2SkillTree::SetCanNotResetSkill);

	lua_tinker::class_add<CX2EXPTable>( L, "CX2EXPTable" );
	lua_tinker::class_def<CX2EXPTable>( L, "AddExpTable_LUA",				CX2EXPTable::AddExpTable_LUA );
	;


	lua_tinker::class_add<CX2DamageEffect::CEffect>( L, "CEffect" );
	lua_tinker::class_def<CX2DamageEffect::CEffect>( L, "SetLockOnNPCUID",					CX2DamageEffect::CEffect::SetLockOnNPCUID );
	lua_tinker::class_def<CX2DamageEffect::CEffect>( L, "GetMainEffect",					CX2DamageEffect::CEffect::GetMainEffect );
	;


	lua_tinker::class_add<CX2TrainingCenterTable>( L, "KTrainingCenterTable" );
	lua_tinker::class_def<CX2TrainingCenterTable>( L, "AddTCTemplet",						CX2TrainingCenterTable::AddTCTemplet_LUA );
	lua_tinker::class_def<CX2TrainingCenterTable>( L, "SetTrainingListByUnitClass_LUA",		CX2TrainingCenterTable::SetTrainingListByUnitClass_LUA );
	;

	lua_tinker::class_add<CX2DropItemManager>( L, "CX2DropItemManager" );
	lua_tinker::class_def<CX2DropItemManager>( L, "AddDropItem_LUA",						CX2DropItemManager::AddDropItem_LUA );
	lua_tinker::class_def<CX2DropItemManager>( L, "CountDropItemByID",						CX2DropItemManager::CountDropItemByID );
	;

	lua_tinker::class_add<CX2PVPEmblem>( L, "CX2PVPEmblem" );
	lua_tinker::class_def<CX2PVPEmblem>( L, "AddEmblemData_LUA",							CX2PVPEmblem::AddEmblemData_LUA );
	;


	lua_tinker::class_add<CX2SocketItem>( L, "CX2SocketItem" );
	lua_tinker::class_def<CX2SocketItem>( L, "AddSocketData_LUA",							CX2SocketItem::AddSocketData_LUA );

	lua_tinker::class_add<CX2StringFilter>( L, "CXSLStringFilter" );
	lua_tinker::class_def<CX2StringFilter>( L, "AddNickNameFilter",			CX2StringFilter::AddNickNameFilter_LUA );
	lua_tinker::class_def<CX2StringFilter>( L, "AddWordFilter",				CX2StringFilter::AddWordFilter_LUA );
	;


	lua_tinker::class_add<CX2LVUpEventMgr>( L, "CX2LVUpEventMgr" );
	lua_tinker::class_def<CX2Game>( L, "AddLevelUpEvent_LUA",			CX2LVUpEventMgr::AddLevelUpEvent_LUA);
}


void CX2LineMapToolMain::ComputeRay( D3DXVECTOR3* pvRayOrigin, D3DXVECTOR3* pvRayDirection, const POINTS& mouspos, 
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



void CX2LineMapToolMain::DeleteNPCXSkinAnim( CX2DungeonSubStage::NPCData* pNPCData )
{
	map< std::pair<CX2DungeonSubStage::NPCData*, int >, CKTDGXSkinAnim* >::iterator it;

	for( UINT i=0; i<pNPCData->m_vecStartPos.size(); i++ )
	{
		it = m_mapNPCXSkinAnim.find( std::make_pair( pNPCData, i ) );
		
		if( it != m_mapNPCXSkinAnim.end() )
		{
			CKTDGXSkinAnim* pSkinAnim = (CKTDGXSkinAnim*) it->second;

			//SAFE_DELETE( pSkinAnim );
			SAFE_DELETE_KTDGOBJECT(pSkinAnim);

			m_mapNPCXSkinAnim.erase( it );
		}
	}
}

void CX2LineMapToolMain::DeleteNPCSkinAnims( CX2Dungeon::DIFFICULTY_LEVEL dungeonDifficultyLevel )
{
	switch( dungeonDifficultyLevel )
	{
	case CX2Dungeon::DL_NORMAL:
		{
			if( m_pNormalStageData != NULL )
			{
				for( UINT i=0; i<m_pNormalStageData->m_SubStageDataList.size(); i++ )
				{
					CX2DungeonSubStage::SubStageData* pSubStageData = m_pNormalStageData->m_SubStageDataList[i];
					for( UINT j=0; j< pSubStageData->m_NPCDataList.size(); j++ )
					{
						CX2DungeonSubStage::NPCData* pNPCData = pSubStageData->m_NPCDataList[j];
						DeleteNPCXSkinAnim( pNPCData );
					}
				}
			}
		} break;

	case CX2Dungeon::DL_HARD:
		{
			if( m_pHardStageData != NULL )
			{
				for( UINT i=0; i<m_pHardStageData->m_SubStageDataList.size(); i++ )
				{
					CX2DungeonSubStage::SubStageData* pSubStageData = m_pHardStageData->m_SubStageDataList[i];
					for( UINT j=0; j< pSubStageData->m_NPCDataList.size(); j++ )
					{
						CX2DungeonSubStage::NPCData* pNPCData = pSubStageData->m_NPCDataList[j];
						DeleteNPCXSkinAnim( pNPCData );
					}
				}	
			}
		} break;

	case CX2Dungeon::DL_EXPERT:
		{
			if( m_pExpertStageData != NULL )
			{
				for( UINT i=0; i<m_pExpertStageData->m_SubStageDataList.size(); i++ )
				{
					CX2DungeonSubStage::SubStageData* pSubStageData = m_pExpertStageData->m_SubStageDataList[i];
					for( UINT j=0; j< pSubStageData->m_NPCDataList.size(); j++ )
					{
						CX2DungeonSubStage::NPCData* pNPCData = pSubStageData->m_NPCDataList[j];
						DeleteNPCXSkinAnim( pNPCData );
					}
				}		
			}
		} break;
	}
}

CKTDGXSkinAnim* CX2LineMapToolMain::LoadNPCSkinAnim( CX2DungeonSubStage::NPCData* pNPCData, int iSubIndex )
{
	map< std::pair<CX2DungeonSubStage::NPCData*, int >, CKTDGXSkinAnim* >::iterator it;
	it = m_mapNPCXSkinAnim.find( std::make_pair( pNPCData, iSubIndex ) );
	if( it != m_mapNPCXSkinAnim.end() )
	{
		CKTDGXSkinAnim* pSkinAnim = (CKTDGXSkinAnim*) it->second;

		//SAFE_DELETE( pSkinAnim );
		SAFE_DELETE_KTDGOBJECT( pSkinAnim );

		m_mapNPCXSkinAnim.erase(it);
	}
	
	CKTDXDeviceXSkinMesh* pSkinMesh = GetNPCSkinMesh( pNPCData->m_UnitID );
	if( NULL != pSkinMesh )
	{
		//CKTDGXSkinAnim* pXSkinAnim = new CKTDGXSkinAnim( g_pKTDXApp->GetDevice() );
		CKTDGXSkinAnim* pXSkinAnim = CKTDGXSkinAnim::CreateSkinAnim();

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

void CX2LineMapToolMain::LoadNPCSkinAnims( CX2Dungeon::DIFFICULTY_LEVEL dungeonDifficultyLevel )
{
	map< std::pair<CX2DungeonSubStage::NPCData*, int>, CKTDGXSkinAnim* >::iterator it;


	switch( dungeonDifficultyLevel )
	{
	case CX2Dungeon::DL_NORMAL:
		{
			if( m_pNormalStageData != NULL )
			{

				for( UINT i=0; i<m_pNormalStageData->m_SubStageDataList.size(); i++ )
				{
					CX2DungeonSubStage::SubStageData* pSubStageData = m_pNormalStageData->m_SubStageDataList[i];
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
		} break;

	case CX2Dungeon::DL_HARD:
		{
			if( NULL != m_pHardStageData )
			{

				for( UINT i=0; i<m_pHardStageData->m_SubStageDataList.size(); i++ )
				{
					CX2DungeonSubStage::SubStageData* pSubStageData = m_pHardStageData->m_SubStageDataList[i];
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
		} break;

	case CX2Dungeon::DL_EXPERT:
		{
			if( NULL != m_pExpertStageData )
			{

				for( UINT i=0; i<m_pExpertStageData->m_SubStageDataList.size(); i++ )
				{
					CX2DungeonSubStage::SubStageData* pSubStageData = m_pExpertStageData->m_SubStageDataList[i];
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
		} break;
	}
}


void CX2LineMapToolMain::UpdateAnyInfoUI()
{
	CDXUTListBox* pListBox = m_DungeonUI.GetListBox( IDC_LIST_ANY_INFO );
	pListBox->RemoveAllItems();
	if( m_wstrLineMapFileName.length() > 0 )
	{
		pListBox->AddItem( m_wstrLineMapFileName.c_str(), (VOID*) NULL ); 
	}
	for( UINT i=0; i<3; i++ )
	{
		if( m_wstrDungeonFileName[i].length() > 0 )
		{
			pListBox->AddItem( m_wstrDungeonFileName[i].c_str(), (VOID*) NULL ); 
		}
	}
}





// 라인맵의 line start, end pos 좌표를 float에서 int로 바꾼다.
void CX2LineMapToolMain::LineMapFloatToInt()
{
	if( NULL == pLineMap )
		return;

	for( int i=0; i<pLineMap->GetNumLineData(); i++ )
	{
		CKTDGLineMap::LineData* pLineData = pLineMap->GetLineData(i);

		pLineData->startPos.x = (float)(long)pLineData->startPos.x;
		pLineData->startPos.y = (float)(long)pLineData->startPos.y;
		pLineData->startPos.z = (float)(long)pLineData->startPos.z;

		pLineData->endPos.x = (float)(long)pLineData->endPos.x;
		pLineData->endPos.y = (float)(long)pLineData->endPos.y;
		pLineData->endPos.z = (float)(long)pLineData->endPos.z;
	}
}

void CX2LineMapToolMain::CollisionCheckNPCStartPos( CX2DungeonSubStage::SubStageData* pSubStageData, D3DXVECTOR3 vRayStart, D3DXVECTOR3 vRayEnd, 
												   float fHitDistance, float fShortestHitDistance, CX2DungeonSubStage::NPCData* &pPickedNPCData, 
												   int &iPickedNPCStartIndex )
{
	for( UINT j=0; j<pSubStageData->m_NPCDataList.size(); j++ )
	{
		CX2DungeonSubStage::NPCData* pNPCData = pSubStageData->m_NPCDataList[j];
		for( UINT k=0; k<pNPCData->m_vecStartPos.size(); k++ )
		{
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
				}
			}
		}
	}
}

bool CX2LineMapToolMain::SaveSectorFile( WCHAR* wszFileName, bool bUpdateFileName )
{
    if( pLineMap == NULL )
        return false;

    bool bResult = true;

    LineMapFloatToInt();

    std::sort( vecTeamStartPos.begin(), vecTeamStartPos.end(), CTeamStartPosSort() );

    RecomputeStartPosLineIndex();

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

    if( true == bUpdateFileName )
        m_wstrLineMapFileName = wstrFilePath;
    
    // 라인정보로 섹터영역을 계산한다.
    int minX, maxX, minY, maxY, minZ, maxZ;
    D3DXVECTOR3 vsp, vep;

    maxX = maxY = maxZ = -99999;
    minX = minY = minZ =  99999;

    CKTDGLineMap::LineData* pLineData = NULL;	

    for( int i=0; i<pLineMap->GetNumLineData(); i++ )
    {
        pLineData = pLineMap->GetLineData(i);
    
        vsp = pLineData->startPos;
        vep = pLineData->endPos;

        minX = min((int)vsp.x, minX);
        minY = min((int)vsp.y, minY);
        minZ = min((int)vsp.z, minZ);
        maxX = max((int)vsp.x, maxX);
        maxY = max((int)vsp.y, maxY);
        maxZ = max((int)vsp.z, maxZ);

        minX = min((int)vep.x, minX);
        minY = min((int)vep.y, minY);
        minZ = min((int)vep.z, minZ);
        maxX = max((int)vep.x, maxX);
        maxY = max((int)vep.y, maxY);
        maxZ = max((int)vep.z, maxZ);       
    }

    // 주위 여분의 타일을 만들어 둔다.
#if 1 
    //minX -= 500;
    //maxX += 500;
    //minY -= 500;
    maxY += 500;
    //minZ -= 500;
    //maxZ += 500;
#endif

    // Create Sector
    int mapWidth, mapHeight, mapDepth;
    mapWidth = maxX - minX;
    mapHeight = maxY - minY;
    mapDepth = maxZ - minZ;

    int tileXCount = (mapWidth / m_iSectorWidth) + 1; 
    int tileYCount = (mapHeight / m_iSectorHeight) + 1; 
    int tileZCount = (mapDepth / m_iSectorDepth) + 1; 

    if(tileXCount < 0 || tileXCount > 255)
    {
        MessageBox( g_pKTDXApp->GetHWND(), L"X Sector수가 범위를 벗어났습니다.", L"ERROR", MB_OK );
        return false;
    }
    if(tileYCount < 0 || tileYCount > 255)
    {
        MessageBox( g_pKTDXApp->GetHWND(), L"Y Sector수가 범위를 벗어났습니다.", L"ERROR", MB_OK );
        return false;
    }
    if(tileZCount < 0 || tileZCount > 255)
    {
        MessageBox( g_pKTDXApp->GetHWND(), L"Z Sector수가 범위를 벗어났습니다.", L"ERROR", MB_OK );
        return false;
    }

    ofs << std::endl << std::endl;
    ofs << "-- MAP SIZE" << std::endl;
    ofs << "MAP_WIDHT   = " << mapWidth << std::endl;
    ofs << "MAP_HEIGHT  = " << mapHeight << std::endl;
    ofs << "MAP_DEPTH   = " << mapDepth << std::endl;

    ofs << std::endl << std::endl;
    ofs << "-- TILE SIZE" << std::endl;
    ofs << "TILE_X = " << m_iSectorWidth << std::endl;
    ofs << "TILE_Y = " << m_iSectorHeight << std::endl;
    ofs << "TILE_Z = " << m_iSectorDepth << std::endl;        

    ofs << std::endl << std::endl;
    ofs << "-- TILE COUNT" << std::endl;
    ofs << "TILE_COUNT_X = " << tileXCount << std::endl;
    ofs << "TILE_COUNT_Y = " << tileYCount << std::endl;
    ofs << "TILE_COUNT_Z = " << tileZCount << std::endl;

    ofs << std::endl << std::endl;
    ofs << "-- TILE START" << std::endl;
    ofs << "TILE_MIN_X = " << minX << std::endl;
    ofs << "TILE_MIN_Y = " << minY << std::endl;
    ofs << "TILE_MIN_Z = " << minZ << std::endl;

    ofs << std::endl << std::endl;
    ofs << "-- TILE INFO" << std::endl;

    int sX, sY, sZ, eX, eY, eZ;    
    for(int i=0; i<tileYCount; ++i) // Y
    {
        sY = i * m_iSectorHeight;
        eY = sY + m_iSectorHeight;

        for(int j=0; j<tileZCount; ++j) // Z
        {
            sZ = j * m_iSectorDepth;
            eZ = sZ + m_iSectorDepth;

            for(int k=0; k<tileXCount; ++k) // X
            {
                sX = k * m_iSectorWidth;
                eX = sX + m_iSectorWidth;

                ofs << "g_pSector:AddSector_LUA" << std::endl;
                ofs << "{" << std::endl;
                
                // index info
                ofs << "    INDEX_X = " << k << std::endl;
                ofs << "    INDEX_Y = " << i << std::endl;
                ofs << "    INDEX_Z = " << j << std::endl << std::endl;

                // sector cube info
                ofs << "    SECTOR_MIN_X = " << minX + sX << std::endl;
                ofs << "    SECTOR_MIN_Y = " << minY + sY << std::endl;
                ofs << "    SECTOR_MIN_Z = " << minZ + sZ << std::endl;
                ofs << "    SECTOR_MAX_X = " << minX + eX << std::endl;
                ofs << "    SECTOR_MAX_Y = " << minY + eY << std::endl;
                ofs << "    SECTOR_MAX_Z = " << minZ + eZ << std::endl;
                ofs << "}" << std::endl;
            }
        }
    }

    ofs.close();

    return bResult;
}

bool CX2LineMapToolMain::CreateSector()
{
    if( pLineMap == NULL )
        return false;

    if(m_bTileRender == false)
        return false;

    bool bResult = true;

    LineMapFloatToInt();

    std::sort( vecTeamStartPos.begin(), vecTeamStartPos.end(), CTeamStartPosSort() );

    RecomputeStartPosLineIndex();


    // 라인정보로 섹터영역을 계산한다.
    int minX, maxX, minY, maxY, minZ, maxZ;
    D3DXVECTOR3 vsp, vep;

    maxX = maxY = maxZ =   -99999;
    minX = minY = minZ =    99999;

    CKTDGLineMap::LineData* pLineData = NULL;	

    for( int i=0; i<pLineMap->GetNumLineData(); i++ )
    {
        pLineData = pLineMap->GetLineData(i);

        vsp = pLineData->startPos;
        vep = pLineData->endPos;

        minX = min((int)vsp.x, minX);
        minY = min((int)vsp.y, minY);
        minZ = min((int)vsp.z, minZ);
        maxX = max((int)vsp.x, maxX);
        maxY = max((int)vsp.y, maxY);
        maxZ = max((int)vsp.z, maxZ);

        minX = min((int)vep.x, minX);
        minY = min((int)vep.y, minY);
        minZ = min((int)vep.z, minZ);
        maxX = max((int)vep.x, maxX);
        maxY = max((int)vep.y, maxY);
        maxZ = max((int)vep.z, maxZ);       
    }

    // 주위 여분의 타일을 만들어 둔다.
#if 1 
    //minX -= 500;
    //maxX += 500;
    //minY -= 500;
    maxY += 500;
    //minZ -= 500;
    //maxZ += 500;
#endif

    if(m_pBox[0] != NULL)
        m_pBox[0]->Release();
    m_vecCenter.clear();

    D3DXCreateBox(g_pKTDXApp->GetDevice(), (float)m_iSectorWidth, (float)m_iSectorHeight, (float)m_iSectorDepth, &m_pBox[0], NULL);

    // Create Sector
    int mapWidth, mapHeight, mapDepth;
    mapWidth = maxX - minX;
    mapHeight = maxY - minY;
    mapDepth = maxZ - minZ;

    int tileXCount = (mapWidth / m_iSectorWidth) + 1; 
    int tileYCount = (mapHeight / m_iSectorHeight) + 1; 
    int tileZCount = (mapDepth / m_iSectorDepth) + 1; 

    if(tileXCount < 0 || tileXCount > 255)
    {
        MessageBox( g_pKTDXApp->GetHWND(), L"X Sector수가 범위를 벗어났습니다.", L"ERROR", MB_OK );
        return false;
    }
    if(tileYCount < 0 || tileYCount > 255)
    {
        MessageBox( g_pKTDXApp->GetHWND(), L"Y Sector수가 범위를 벗어났습니다.", L"ERROR", MB_OK );
        return false;
    }
    if(tileZCount < 0 || tileZCount > 255)
    {
        MessageBox( g_pKTDXApp->GetHWND(), L"Z Sector수가 범위를 벗어났습니다.", L"ERROR", MB_OK );
        return false;
    }

    int sX, sY, sZ, eX, eY, eZ;    
    for(int i=0; i<tileYCount; ++i) // Y
    {
        sY = i * m_iSectorHeight;
        eY = sY + m_iSectorHeight;

        for(int j=0; j<tileZCount; ++j) // Z
        {
            sZ = j * m_iSectorDepth;
            eZ = sZ + m_iSectorDepth;

            for(int k=0; k<tileXCount; ++k) // X
            {
                sX = k * m_iSectorWidth;
                eX = sX + m_iSectorWidth;
                
                m_vecCenter.push_back(D3DXVECTOR3(minX + sX + (m_iSectorWidth/2.f), minY + sY + (m_iSectorHeight/2.f), minZ + sZ + (m_iSectorDepth/2.f)));                
            }
        }
    }

    return bResult;
}

void CX2LineMapToolMain::OnRenderTile()
{    
    D3DXMATRIX matTrans; 

    g_pKTDXApp->GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
    //for(int i=0; i<2; ++i)
    for(int i=0; i<(int)m_vecCenter.size(); ++i)
    {
        D3DXMatrixTranslation( &matTrans, m_vecCenter[i].x, m_vecCenter[i].y, m_vecCenter[i].z );

        g_pKTDXApp->GetDevice()->SetTransform(D3DTS_WORLD, &matTrans);
        m_pBox[0]->DrawSubset(0);
    }    
    g_pKTDXApp->GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}
