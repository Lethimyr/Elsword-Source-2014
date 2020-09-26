#include "Stdafx.h"
#include ".\X2UserListPopUp.h"

//{{ 김상훈 : [2010.10.13] 캐릭터 우클릭 팝업 메뉴 UI 개선
#ifdef CHARACTER_MOUSE_RBUTTON

CX2UserListPopUp::CX2UserListPopUp()
{
	m_UserUidList.clear();
	m_iNowPage = 1;
	m_bShow = 0;

	m_pDlgMenu = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_User_List.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDlgMenu );
	
	CKTDGUIStatic* pStaticFaceImage = (CKTDGUIStatic*) m_pDlgMenu->GetControl( L"character_face_image" );
	CKTDGUIStatic* pStaticLevel = NULL;
	CKTDGUIControl::CPictureData* pPicture = NULL;
	char buff[128] = {0,};
	
	for (int i=0;i<4;++i)
	{
		m_UserListSlot[i].m_Enable = false;
		m_UserListSlot[i].m_UserUid = 0;
		m_UserListSlot[i].m_bIsPet = 0;

		//sprintf( buff, "User_%d", i+1 );
		StringCchPrintfA(  buff, 128, "User_%d", i+1 );
		m_UserListSlot[i].m_pButton = m_pDlgMenu->GetButton_LUA( buff );
		m_UserListSlot[i].m_pButton->SetEnable( false );
		m_UserListSlot[i].m_pButton->SetEnableClick( false );

		pPicture = pStaticFaceImage->GetPictureIndex(i+1);
		pPicture->SetShow(false);

		m_UserListSlot[i].m_pLevel = new CKTDGUIDialog( g_pMain->GetNowState() , L"DLG_UI_User_List_Level.lua"  );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_UserListSlot[i].m_pLevel );	
		
		
		pStaticLevel = (CKTDGUIStatic*)	m_UserListSlot[i].m_pLevel->GetControl( L"one" );

		for(UINT j=0; j < ARRAY_SIZE(m_UserListSlot[i].m_pLevelOnePic); ++j)
		{
			m_UserListSlot[i].m_pLevelOnePic[j] = pStaticLevel->GetPictureIndex(j+1);
			m_UserListSlot[i].m_pLevelOnePic[j]->SetPos( D3DXVECTOR2( (float)28 , (float)37 + ( 30 * i ) ));
		}
		
		pStaticLevel = (CKTDGUIStatic*)	m_UserListSlot[i].m_pLevel->GetControl( L"ten" );
		for(UINT j=0; j < ARRAY_SIZE(m_UserListSlot[i].m_pLevelTenPic); ++j)
		{
			m_UserListSlot[i].m_pLevelTenPic[j] = pStaticLevel->GetPictureIndex(j+1);
			m_UserListSlot[i].m_pLevelTenPic[j]->SetPos( D3DXVECTOR2( (float)22 , (float)37 + ( 30 * i ) ));
		}
	}

	m_pDlgMenu->SetShowEnable( false, false );
}

CX2UserListPopUp::~CX2UserListPopUp( void )
{
	m_UserUidList.clear();

	for (int i=0;i<4;++i)
		SAFE_DELETE_DIALOG_HANDLE( m_UserListSlot[i].m_pLevel );
	SAFE_DELETE_DIALOG_HANDLE(m_pDlgMenu);
}

void CX2UserListPopUp::SetStage( CKTDXStage* pNowState )
{
	m_pNowState = (CX2State*) pNowState;

	m_pDlgMenu->SetStage(m_pNowState);
}

bool CX2UserListPopUp::SetPopupMenu()
{

	CKTDGUIStatic* pStaticFaceImage = (CKTDGUIStatic*) m_pDlgMenu->GetControl( L"character_face_image" );
	CKTDGUIStatic* pStaticPage = (CKTDGUIStatic*) m_pDlgMenu->GetControl( L"g_pStaticPage_number_Font" );
	wstring fileName;
	wstring pieceName;
	char buff[128] = {0,};
	WCHAR wbuff[128] = {0,};
	wstring wsbuff;

	//wsprintf( wbuff, L"%d / %d", m_iNowPage,  (m_UserUidList.size()-1) / 4 + 1);
	StringCchPrintf(  wbuff, 128, L"%d / %d", m_iNowPage,  (m_UserUidList.size()-1) / 4 + 1);
	wsbuff = wbuff;
	
	pStaticPage->GetString(0)->msg  = wsbuff;
	pStaticPage->SetShowEnable( true , true );

	pStaticFaceImage->SetShowEnable( true , true );
	pStaticFaceImage->SetSize( D3DXVECTOR2( 20.f, 20.f ) );
		
	for ( int i = 0;i < 4; i++ )
	{
		for (UINT j=0;j<ARRAY_SIZE( m_UserListSlot[i].m_pLevelOnePic );++j)
			m_UserListSlot[i].m_pLevelOnePic[j]->SetShow(false);
		for (UINT j=0;j<ARRAY_SIZE( m_UserListSlot[i].m_pLevelTenPic );++j)
			m_UserListSlot[i].m_pLevelTenPic[j]->SetShow(false);

		m_UserListSlot[i].m_pLevel->SetShowEnable(true,true);

		StringCchPrintfA(  buff, 128, "User_Name_%d", i+1 );
		CKTDGUIStatic* pStaticUserName	= m_pDlgMenu->GetStatic_LUA( buff );
		if ( NULL != pStaticUserName )
			pStaticUserName->GetString(0)->msg = L"";
		
		CKTDGUIControl::CPictureData* pPicture = pStaticFaceImage->GetPictureIndex( i + 1);
		if ( NULL != pPicture )
			pPicture->SetShow(false);

		if ( (UINT)i + ( m_iNowPage-1 ) * 4 < m_UserUidList.size() )
		{
			m_UserListSlot[i].m_UserUid = m_UserUidList[i + (m_iNowPage-1)*4].first;
			m_UserListSlot[i].m_bIsPet = m_UserUidList[i+(m_iNowPage-1)*4].second;
			m_UserListSlot[i].m_Enable = true;
			m_UserListSlot[i].m_pButton->SetEnable( true );
			m_UserListSlot[i].m_pButton->SetEnableClick( true );
			
		}
		else
		{
			m_UserListSlot[i].m_Enable = false;
			m_UserListSlot[i].m_pButton->SetEnable(false);
			m_UserListSlot[i].m_pButton->SetEnableClick(false);
		}
	}

	for (int i=0;i<4;i++)
	{
		if ( m_UserListSlot[i].m_Enable == false )
			break;

		if ( m_UserListSlot[i].m_bIsPet == false )  // 유저
		{
			const CX2Unit* pUnit = GetUnitByUserUid( m_UserListSlot[i].m_UserUid );			

			if ( NULL != pUnit )
			{
				//sprintf( buff,  "User_Name_%d", i+1 );
				StringCchPrintfA(  buff, 128, "User_Name_%d", i+1 );
				CKTDGUIStatic* pStaticUserName = m_pDlgMenu->GetStatic_LUA( buff );
				if ( NULL != pStaticUserName )
					pStaticUserName->GetString(0)->msg = pUnit->GetNickName();
				
				const int iUnitLevel = pUnit->GetUnitData().m_Level;
				int tenL = iUnitLevel / 10;
				int oneL = iUnitLevel % 10;

				if(tenL > 0)
					m_UserListSlot[i].m_pLevelTenPic[tenL-1]->SetShow(true);
				else if(tenL == 0)
					m_UserListSlot[i].m_pLevelTenPic[9]->SetShow(true);
				
				if(oneL == 0)
					oneL = 10;

				m_UserListSlot[i].m_pLevelOnePic[oneL-1]->SetShow(true);


				if( true == CX2Data::GetCharacterImageName( fileName, pieceName, pUnit->GetClass(), CX2Data::CIT_20by20 ) )
				{
					CKTDGUIControl::CPictureData* pPicture = pStaticFaceImage->GetPictureIndex( i + 1 );
					if ( NULL != pPicture )
					{
						pPicture->SetTex( fileName.c_str(), pieceName.c_str() );
						pPicture->SetShow(true);
					}					
				}
				else
				{
					ASSERT( !"NO" );
				}
			}
		}
#ifdef SERV_PET_SYSTEM
		else  // 펫 
		{
			CX2PET *pPet = g_pData->GetPetManager()->GetPetByUid( m_UserListSlot[i].m_UserUid );

			//sprintf( buff,  "User_Name_%d", i+1 );
			StringCchPrintfA(  buff, 128, "User_Name_%d", i+1 );
			CKTDGUIStatic* pStaticPetName = m_pDlgMenu->GetStatic_LUA( buff );
			if ( NULL != pStaticPetName )
				pStaticPetName->GetString(0)->msg = pPet->GetPetInfo().m_wstrPetName;
						
			CKTDGUIControl::CPictureData* pPicture = pStaticFaceImage->GetPictureIndex( i + 1 );
			if ( NULL != pPicture )
			{
				CX2PetManager::PetTemplet *pTemplet = g_pData->GetPetManager()->GetPetTemplet( (CX2PetManager::PET_UNIT_ID)pPet->GetPetInfo().m_PetId );
	#ifdef DEFENCE_CODE_FOR_CRASH
				if( pPet->GetPetInfo().m_Evolution_Step < pTemplet->m_Evolution_Step_Image.size() )
				{
	#endif //DEFENCE_CODE_FOR_CRASH
					CX2PetManager::PetStepImage petStepImage = pTemplet->m_Evolution_Step_Image[ pPet->GetPetInfo().m_Evolution_Step ];
					wstring wstrImageName = petStepImage.m_wstrImageName;

					if( petStepImage.m_wstrKeyName == L"" )
						pPicture->SetTex( wstrImageName.c_str() );
					else
						pPicture->SetTex( wstrImageName.c_str(), petStepImage.m_wstrKeyName.c_str() );

					pPicture->SetShow(true);
	#ifdef DEFENCE_CODE_FOR_CRASH
				}
	#endif //DEFENCE_CODE_FOR_CRASH
			}
		}
#endif SERV_PET_SYSTEM
	}
	

	return true;
}


void CX2UserListPopUp::AddList( UidType iUnitUid , bool isPet)
{
	m_UserUidList.push_back( make_pair(iUnitUid, isPet ));
}

int CX2UserListPopUp::GetListSize()
{
	return (int)m_UserUidList.size();
}



void CX2UserListPopUp::PrevPage()
{
	if ( m_iNowPage > 1 ) 
	{
		m_iNowPage --;
		OpenPopupMenu( true );
	}
}

void CX2UserListPopUp::NextPage()
{
	if ( (UINT)m_iNowPage < (m_UserUidList.size()-1)/4 +1)
	{
		m_iNowPage ++;		
		OpenPopupMenu( true );
	}
}

bool CX2UserListPopUp::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch(wParam)
	{
	case UPM_PREV_PAGE:
		PrevPage();
		break;
	case UPM_NEXT_PAGE:
		NextPage();
		break;
	case UPM_CLICK1 :
		if ( m_UserListSlot[0].m_bIsPet == false ) // 사람
			g_pData->GetUserMenu()->OpenUserPopupMenu( m_UserListSlot[0].m_UserUid , false );// 사람 한 명 	
#ifdef SERV_PET_SYSTEM
		else // 펫
			g_pData->GetPetManager()->OpenPetPopupMenu();
#endif SERV_PET_SYSTEM
		ClosePopupMenu();
		break;
	case UPM_CLICK2 :
		g_pData->GetUserMenu()->OpenUserPopupMenu( m_UserListSlot[1].m_UserUid , false );// 사람 한 명 	
		ClosePopupMenu();
		break;
	case UPM_CLICK3 :
		g_pData->GetUserMenu()->OpenUserPopupMenu( m_UserListSlot[2].m_UserUid , false );// 사람 한 명 	
		ClosePopupMenu();
		break;
	case UPM_CLICK4 :
		g_pData->GetUserMenu()->OpenUserPopupMenu( m_UserListSlot[3].m_UserUid , false );// 사람 한 명 	
		ClosePopupMenu();
		break;
	case UPM_QUIT:
		ClosePopupMenu();
		break;
	}
	return false;
}

bool CX2UserListPopUp::UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{	
	return false;
}


void CX2UserListPopUp::OpenPopupMenu( bool bNotOpen )	
{
	if ( m_bShow && bNotOpen != true )
	{
		if ( m_pDlgMenu->GetShow() )
			m_bShow = m_bShow;
		else
		{
			m_bShow = false;
			ClosePopupMenu();
		}
		return; 
	}

	switch( m_UserUidList.size() )
	{
	case 0: return;
	case 1:
		{
			if ( m_UserUidList[0].second == false )
			{
				CX2PartyManager* pPartyManager = g_pData->GetPartyManager();
				bool bIsParty = false;

				if ( NULL != pPartyManager && true == pPartyManager->DoIHaveParty() )
					bIsParty = true;

				g_pData->GetUserMenu()->OpenUserPopupMenu( m_UserUidList[0].first , bIsParty );// 사람 한 명 				
				ClosePopupMenu();
			}
#ifdef SERV_PET_SYSTEM
			else
			{
				g_pData->GetPetManager()->GetMyPetPick( true );	// 펫 한 마리
				ClosePopupMenu();
			}
#endif
		}
		break;
	default:
		SetPopupMenu();
		g_pData->GetUserMenu()->ClosePopupMenu();

		m_pDlgMenu->SetFront(true);


		if ( bNotOpen == false )
		{

			D3DXVECTOR2 vMousePos;
			vMousePos.x = (float)g_pKTDXApp->GetDIManager()->GetMouse()->GetXPos();
			vMousePos.y = (float)g_pKTDXApp->GetDIManager()->GetMouse()->GetYPos();
			vMousePos = g_pKTDXApp->MouseConvertByResolution( vMousePos );


			// 팝업 메뉴가 화면 밖으로 벗어나지 않게 수정
			if( vMousePos.x > 860.f )
			{
				vMousePos.x = 860.f;
			}

			if( vMousePos.x < 0.f )
			{
				vMousePos.x = 0.f;
			}



			if( vMousePos.y > 768.f - (float) 4 * 30.f )
			{
				vMousePos.y = 768.f - (float) 4 * 30.f;
			}

			if( vMousePos.y < 0.f )
			{
				vMousePos.y = 0.f;
			}

			m_pDlgPopup->SetPos( D3DXVECTOR2( (float)vMousePos.x-30.f, (float)vMousePos.y - 40.f ) );
			m_pDlgPopup->SetShowEnable( true, true );

			m_pDlgMenu->SetPos( D3DXVECTOR2( (float)vMousePos.x-30.f, (float)vMousePos.y - 40.f ) );

			for (int i=0;i<4;++i)
			{
				D3DXVECTOR2 Pos = m_pDlgMenu->GetPos();
				m_UserListSlot[i].m_pLevel->SetPos( Pos );
				m_UserListSlot[i].m_pLevel->SetFront( true );
			}
		}

		float fButtonWidth = 103.f;
		float fButtonHeight = 22.f;
		D3DXVECTOR2 offsetPos = D3DXVECTOR2(0.f, 0.f);
		m_nMenuCount = 5;

		m_pPicCenterTop->SetSizeX( fButtonWidth );
		m_pPicLeftMiddle->SetSizeY( m_nMenuCount * fButtonHeight );
		m_pPicCenterMiddle->SetSizeX( fButtonWidth );
		m_pPicCenterMiddle->SetSizeY( m_nMenuCount * fButtonHeight );
		m_pPicRightMiddle->SetSizeY( m_nMenuCount * fButtonHeight );
		m_pPicCenterBottom->SetSizeX( fButtonWidth );

		offsetPos = m_pPicCenterTop->GetPos();
		offsetPos.x += fButtonWidth;
		m_pPicRightTop->SetPos( offsetPos );

		offsetPos = m_pPicCenterMiddle->GetPos();
		offsetPos.x += fButtonWidth;
		m_pPicRightMiddle->SetPos( offsetPos );

		offsetPos = m_pPicLeftMiddle->GetPos();
		offsetPos.y += (m_nMenuCount * fButtonHeight);
		m_pPicLeftBottom->SetPos( offsetPos );

		offsetPos = m_pPicCenterMiddle->GetPos();
		offsetPos.y += (m_nMenuCount * fButtonHeight);
		m_pPicCenterBottom->SetPos( offsetPos );

		offsetPos = m_pPicCenterBottom->GetPos();
		offsetPos.x += fButtonWidth;
		m_pPicRightBottom->SetPos( offsetPos );

		m_pDlgMenu->SetShowEnable( true, true );
		
		m_bShow = true;
	}
}

void CX2UserListPopUp::ClosePopupMenu()
{
	if ( NULL != m_pDlgPopup )
		m_pDlgPopup->SetShowEnable(false, false);

	m_pDlgMenu->SetShowEnable( false, false );
	m_bShow = false;
	m_iNowPage = 1;

	for (int i=0;i<4;i++)
		m_UserListSlot[i].m_pLevel->SetShow(false);

	m_UserUidList.clear();
}

const CX2Unit* CX2UserListPopUp::GetUnitByUserUid( const UidType uid_ )
{
	switch ( g_pMain->GetNowStateID() )
	{
	case CX2Main::XS_BATTLE_FIELD:
		{
			if ( NULL != g_pX2Game )
			{
				CX2GUUser* pGUUser = g_pX2Game->GetUserUnitByUID( uid_ );
				if ( NULL != pGUUser )
					return pGUUser->GetUnit();
				else
					NULL;
			}
		} break;

	case CX2Main::XS_VILLAGE_MAP:
		{
			if ( NULL != g_pTFieldGame )
			{
				CX2SquareUnit* pSquareUnit = g_pTFieldGame->GetSquareUnitByUID( uid_ );
				if ( NULL != pSquareUnit )
					return pSquareUnit->GetUnit();
				else
					return NULL;
			}
		} break;

	default:
		ASSERT( !L"Wrong Path" );
		return NULL;
		break;
	}

	return NULL;
}
#endif CHARACTER_MOUSE_RBUTTON
//}} 김상훈 : [2010.10.13] 캐릭터 우클릭 팝업 메뉴 UI 개선