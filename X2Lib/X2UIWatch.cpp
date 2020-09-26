#include "StdAfx.h"

#ifdef NEW_VILLAGE_UI

#include "X2UIWatch.h"

CX2UIWatch::CX2UIWatch( CKTDXStage* pNowStage, const WCHAR* pFileName )
: CX2ItemSlotManager( pNowStage, NULL ),
m_bShow(false),
m_DLGPosition(0,0),				// D3DXVECTOR2
m_DLGSize(0,0),					// D3DXVECTOR2
m_pDLGWatch(NULL),
m_pUnitViewerUI(NULL),
m_fRemainRotY(0),
m_fMouseSensitivity(0),
m_bRotateReturn(false),
m_UnitClickPos(0,0),				// D3DXVECTOR2
m_UnitClickSize(0,0),				// D3DXVECTOR2
m_bUnitClicked(false),
m_pTargetUnit(NULL),
m_TargetUnitUID(0),
m_MovedPosition(0,0)				// D3DXVECTOR2
{

	//////////////////////////////////////////////////////////////////////////

	RegisterLuaBind();	

	m_pDLGWatch = new CKTDGUIDialog( g_pMain->GetNowState(), pFileName );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGWatch );
	m_pDLGWatch->SetDisableUnderWindow(true);
	m_pDLGWatch->SetFront(true);



	// 유닛뷰어 관련 값들
	m_UnitClickPos = D3DXVECTOR2( 120, 130 );
	m_UnitClickSize = D3DXVECTOR2( 214, 253 );
	m_fMouseSensitivity = 0.01f;
	m_fRemainRotY = 0.0f;
	m_bRotateReturn = true;

	for ( int i = 0; i < (int)m_SlotList.size(); i++ )
	{
		CX2SlotItem* pItemSlot = (CX2SlotItem*)m_SlotList[i];
		pItemSlot->LostItemUI();
		pItemSlot->SetShow( false );
		pItemSlot->SetEnable( false );
	}
	m_pDLGWatch->SetShowEnable(false, false);
	D3DXVECTOR3 tmp;
	tmp = m_pDLGWatch->GetDummyPos(0);
	m_DLGPosition.x = tmp.x;
	m_DLGPosition.y = tmp.y;
	tmp = m_pDLGWatch->GetDummyPos(1);
	m_DLGSize.x = tmp.x;
	m_DLGSize.y = tmp.y;


	//{{ 09.03.24 김태완
	// 임시 코드 : 이 안에 들어있는 닉네임에 대해서 엿보기를 할 수 없게 한다
// 	m_setNoOpenNickNameList.insert( L"일반테스트" );
// 	m_setNoOpenNickNameList.insert( L"얍" );
	m_setNoOpenNickNameList.insert( L"GM키에란" );
	m_setNoOpenNickNameList.insert( L"GM리엘" );
// 	m_setNoOpenNickNameList.insert( L"12" );
// 	m_setNoOpenNickNameList.insert( L"23" );	
	//}}

}
CX2UIWatch::~CX2UIWatch(void)
{
	SAFE_DELETE_DIALOG( m_pDLGWatch );
	SAFE_DELETE_KTDGOBJECT( m_pUnitViewerUI );
}
 
HRESULT CX2UIWatch::OnFrameMove( double fTime, float fElapsedTime )
{

	KTDXPROFILE();

	// 목표 유저가 없어졌으면 닫히게 하자 : 아 이거 참...
	
	CX2Unit* pUnit = GetUnitByUid( m_TargetUnitUID );
	
	if(pUnit == NULL || m_pTargetUnit != pUnit )
	{
        SetShow(false);
		return S_OK;
	}

	UnitHandling( fTime, fElapsedTime );

	if(m_pDLGWatch != NULL && m_pDLGWatch->GetIsMouseOver() == true)
	{
		return CX2ItemSlotManager::OnFrameMove( fTime, fElapsedTime );
	}
	else
	{
		CX2ItemSlotManager::InvalidSelectedItem();
		CX2ItemSlotManager::InvalidSlotDesc();
	}

	return S_OK;
}

bool CX2UIWatch::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// 앞에 모달 다이얼로그가 있으면 메시지를 처리하지 않는다
	if( true == g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckFrontModalDlg( m_pDLGWatch ) )
		return false;

	bool bFlag = false;
	switch( uMsg )
	{
		// 마우스 메시지들에 대해
	case WM_MOUSEMOVE:
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_LBUTTONDBLCLK:
	case WM_MBUTTONDBLCLK:
	case WM_RBUTTONDBLCLK:
	//case WM_MOUSEWHEEL:
		{
			if ( false == m_pDLGWatch->GetIsMouseOver())
			{
				return false;
			}
			
			bFlag = true;
		}
		break;
	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
		{
			if ( false == m_pDLGWatch->GetIsMouseOver())
			{
				return false;
			}
			g_pData->GetUIManager()->SetUILayerIndex(CX2UIManager::UI_MENU_WATCH, true);
			bFlag = true;			
		}
		break;
	default:
		break;
	}

	switch( uMsg )
	{
	case WM_LBUTTONUP:
		{
			if ( m_pUnitViewerUI != NULL && m_bUnitClicked == true )
			{
				D3DXVECTOR3 rot = m_pUnitViewerUI->GetMatrix().GetRotate();
				int degree = (int)D3DXToDegree( rot.y );
				m_fRemainRotY = D3DXToRadian( degree % 360 );
			}

			m_bUnitClicked = false;
		}
		break;

	case WM_LBUTTONDOWN:
		{
			POINT mousePoint = { short(LOWORD(lParam)), short(HIWORD(lParam)) };
			D3DXVECTOR2 mousePos = D3DXVECTOR2( (float)mousePoint.x, (float)mousePoint.y );

			if ( m_bShow == true && IsInUnitClickReact( mousePos ) == true )
			{
				m_bUnitClicked = true;
				return true;
			}
		}
		break;

	case WM_MOUSEMOVE:
		{
			POINT mousePoint = { short(LOWORD(lParam)), short(HIWORD(lParam)) };

			D3DXVECTOR2 mousePos = D3DXVECTOR2( (float)mousePoint.x, (float)mousePoint.y );
			m_NowMousePos = mousePos;

			MouseMove( mousePos );
		}
		break;
	}
	return bFlag;
}

bool CX2UIWatch::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{

	switch(wParam)
	{
	case UWCM_EXIT:
		{
			SetShow(false);
			return true;
		}
	}
	return false;
}

void CX2UIWatch::SetShow(bool val)
{
	if( false == val )
	{
		// 아이템 마우스 오버 이미지를 지우자
		m_pDLGSelectedItem->SetShow( false ); 
		InvalidSlotDesc();
		

		m_pDLGWatch->SetHasUnit( NULL );
		SAFE_DELETE_KTDGOBJECT( m_pUnitViewerUI );
		
		
		m_pDLGWatch->SetShowEnable(false, false);
		m_bShow = false;

		// 슬롯을 끄자
		for ( int i = 0; i < (int)m_SlotList.size(); i++ )
		{
			CX2SlotItem* pItemSlot = (CX2SlotItem*)m_SlotList[i];
			pItemSlot->LostItemUI();
			pItemSlot->SetShow( false );
			pItemSlot->SetEnable( false );
		}
		g_pData->GetUIManager()->UIClosed(CX2UIManager::UI_MENU_WATCH);
	}
}

CX2Unit* CX2UIWatch::GetUnitByUid( UidType uid )
{
	CX2Unit* pRetUnit = NULL;
	// State에 따라 다른 방식으로 Unit 정보를 가져오도록 하자.
	switch( g_pMain->GetNowStateID() )
	{
	case CX2Main::XS_VILLAGE_MAP:
		{
			if(g_pTFieldGame != NULL)
			{
				CX2SquareUnit* pSUnit = g_pTFieldGame->GetSquareUnitByUID( uid );
				if(pSUnit != NULL)
					pRetUnit = pSUnit->GetUnit();
			}
		} break;

	case CX2Main::XS_SQUARE_GAME:
		{
			if(g_pSquareGame != NULL)
			{
				CX2SquareUnit* pSUnit = g_pSquareGame->GetSquareUnitByUID( uid );
				if(pSUnit != NULL)
					pRetUnit = pSUnit->GetUnit();
			}
		} break;

	case CX2Main::XS_PVP_ROOM:
		{
			if( NULL != g_pX2Room )
			{
				CX2Room::SlotData* pSlotData = g_pX2Room->GetSlotDataByUnitUID( uid );
				if(pSlotData != NULL)
					pRetUnit = pSlotData->m_pUnit;
			}
		} break;

	case CX2Main::XS_BATTLE_FIELD:
		{
			CX2BattleFieldRoom::SlotData* pSlotData = g_pData->GetBattleFieldRoom()->GetSlotDataByUnitUID( uid );
			if( NULL != pSlotData )
			{
				pRetUnit = pSlotData->m_pUnit;
			}
		} break;

	default:
		break;
	}

	return pRetUnit;

}

bool CX2UIWatch::OpenDialog(UidType uid)
{
	m_TargetUnitUID = uid;

    m_pTargetUnit = GetUnitByUid( uid );
	if(m_pTargetUnit == NULL) return false;

	//{{ 09.03.24 김태완 : 임시코드
	wstring NickName = m_pTargetUnit->GetNickName();
	if( m_setNoOpenNickNameList.find(NickName.c_str()) != m_setNoOpenNickNameList.end() )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_934 ), g_pMain->GetNowState() );
		return false;
	}
	//}}
	

	SAFE_DELETE_KTDGOBJECT( m_pUnitViewerUI );

	ResetUnitViewerUI();
	m_pUnitViewerUI->PlayByMotionType( CX2UnitViewerUI::UVVMT_DOWN_LANDING ); 

	// 캐릭터 정보 읽어와서 이름이랑 레벨 전적 대전순위 대전등급을 예쁘게 적어넣고
	WCHAR buff[256] = {0,};
	const CX2Unit::UnitData* pUnitData = &m_pTargetUnit->GetUnitData();

	// 이름
	CKTDGUIStatic* pStaticUserInfo = (CKTDGUIStatic*)m_pDLGWatch->GetControl( L"g_pStaticName" );
	pStaticUserInfo->GetString(0)->msg = m_pTargetUnit->GetNickName();

	// 레벨
	CKTDGUIStatic* pStaticUserLv = (CKTDGUIStatic*)m_pDLGWatch->GetControl( L"g_pStaticLv" );
	//wsprintf( buff, L"%d", pUnitData->m_Level );
	StringCchPrintf( buff, 256, L"%d", pUnitData->m_Level );
	pStaticUserLv->GetString(0)->msg = buff;

	// 전적
	CKTDGUIStatic* pStaticPVP_Room_Unit_WinNum = (CKTDGUIStatic*)m_pDLGWatch->GetControl( L"g_pStaticPvPWin" );
	if( NULL != pStaticPVP_Room_Unit_WinNum &&
		NULL != pStaticPVP_Room_Unit_WinNum->GetString(0) )
	{
#ifdef SERV_PVP_NEW_SYSTEM
		if ( -1 != pUnitData->m_Win )
		{
			pStaticPVP_Room_Unit_WinNum->GetString(0)->msg = GET_REPLACED_STRING( ( STR_ID_669, "i", ( int )pUnitData->m_Win ) );
		}
		else
		{
			pStaticPVP_Room_Unit_WinNum->GetString(0)->msg = GET_STRING(STR_ID_13617);
		}
#else
		pStaticPVP_Room_Unit_WinNum->GetString(0)->msg = GET_REPLACED_STRING( ( STR_ID_790, "ii", ( int )pUnitData->m_Win, ( int )pUnitData->m_Lose ) );
#endif
	}

#ifdef FIX_WATCH_UI
	CKTDGUIStatic* pStatic_RankPoint = (CKTDGUIStatic*)m_pDLGWatch->GetControl( L"g_pStaticinfo_rank" );
	if( NULL != pStatic_RankPoint &&
		NULL != pStatic_RankPoint->GetString(0) )
	{
		pStatic_RankPoint->GetString( 0 )->msg = g_pMain->GetEDString( (int)pUnitData->m_iRPoint );
	}
#endif

	// PVP랭크
	CKTDGUIStatic* pStaticPVP_Rank = (CKTDGUIStatic*)m_pDLGWatch->GetControl( L"g_pStaticPvPRank" );
#ifdef SERV_PVP_NEW_SYSTEM
#ifdef PVP_SEASON2
	char cRank = pUnitData->m_cRank;
	CX2PVPEmblem::PVPEmblemData* pPVPEmblemData = g_pMain->GetPVPEmblem()->GetPVPEmblemData( static_cast<CX2PVPEmblem::PVP_RANK>(cRank) );
	
	if ( pStaticPVP_Rank != NULL && pPVPEmblemData != NULL )
	{
		pStaticPVP_Rank->GetPicture(0)->SetTex( pPVPEmblemData->m_TextureName.c_str(), pPVPEmblemData->m_TextureKey.c_str() );
	}
#else
	if ( pStaticPVP_Rank != NULL && g_pMain->GetPVPEmblem()->GetPVPEmblemData( (pUnitData->m_iRating) ) != NULL )
	{
		pStaticPVP_Rank->GetPicture(0)->SetTex( g_pMain->GetPVPEmblem()->GetPVPEmblemData( pUnitData->m_iRating )->m_TextureName.c_str(),
			g_pMain->GetPVPEmblem()->GetPVPEmblemData( pUnitData->m_iRating )->m_TextureKey.c_str() );
	}
#endif
#else
	if ( pStaticPVP_Rank != NULL && g_pMain->GetPVPEmblem()->GetPVPEmblemData( pUnitData->m_PVPEmblem ) != NULL )
	{
		//pStaticPVP_Room_Unit_Title->GetString(0)->msg = g_pMain->GetPVPEmblem()->GetMyNowUnitPVPEmblem()->m_EmblemName;
		pStaticPVP_Rank->GetPicture(0)->SetTex( g_pMain->GetPVPEmblem()->GetPVPEmblemData( pUnitData->m_PVPEmblem )->m_TextureName.c_str(),
												g_pMain->GetPVPEmblem()->GetPVPEmblemData( pUnitData->m_PVPEmblem )->m_TextureKey.c_str() );
	}
#endif

#ifdef SERV_PVP_NEW_SYSTEM
	CKTDGUIStatic* pStaticInfo_Rank = (CKTDGUIStatic*)m_pDLGWatch->GetControl( L"g_pStaticinfo_rank" );
	if( pStaticInfo_Rank != NULL )
	{
		if( -1 != pUnitData->m_iRPoint )
		{
			pStaticInfo_Rank->GetString(0)->msg = g_pMain->GetEDString( (int)pUnitData->m_iRPoint );
		}
		else
		{
			pStaticInfo_Rank->GetString(0)->msg = GET_STRING(STR_ID_13617);
		}
	}	
#endif

	// 캐릭터 아이콘
	CKTDGUIStatic* pStaticCharIcon = (CKTDGUIStatic*)m_pDLGWatch->GetControl( L"Static_CharIcon" );

	wstring fileName;
	wstring pieceName;
	if( true == CX2Data::GetCharacterImageName( fileName, pieceName, pUnitData->m_UnitClass, CX2Data::CIT_20by20 ) )
	{
		pStaticCharIcon->GetPicture(0)->SetTex( fileName.c_str(), pieceName.c_str() );
	}
	else
	{
		ASSERT( !"NO" );
	}


	// 타이틀
	CKTDGUIStatic* pStaticTitleIcon = (CKTDGUIStatic*)m_pDLGWatch->GetControl( L"Info_Title" );
	if(pStaticTitleIcon != NULL)
	{
		bool bIsEmptyTitle = true;
		if(g_pData != NULL && g_pData->GetTitleManager() != NULL)
		{
			const CX2TitleManager::TitleInfo *pTitle = g_pData->GetTitleManager()->GetTitleInfo(pUnitData->m_iTitleId);
			if(pTitle != NULL)
			{
				wstring wstrImgName = pTitle->m_wstrParticleName;
				wstrImgName += L".tga";
				pStaticTitleIcon->GetPicture(0)->SetTex( wstrImgName.c_str() );

				bIsEmptyTitle = false;

#ifdef TITLE_SHOWING_PVP_RESULT
				CKTDGUIStatic* pStaticTitleScore = (CKTDGUIStatic*)m_pDLGWatch->GetControl( L"g_pStaticInfo_TitleScore" );
				if( pStaticTitleScore != NULL )
				{
					if( g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetSelectUnit() != NULL )
					{
						if( pUnitData->m_iTitleId == _CONST_TITLE_SHOWING_PVP_RESULT_::iTitleID )
						{
							char szCount[10];
							::itoa( g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_iAccountPVPLoseCount, szCount, 10 );
							pStaticTitleScore->GetString(0)->msg = KncUtil::toWideString(szCount).c_str();
							pStaticTitleScore->SetShow( true );
						}
						else
							pStaticTitleScore->SetShow( false );
					}
				}
#endif //TITLE_SHOWING_PVP_RESULT

#ifdef ADD_TITLE_DESCRIPTION
				// 타이틀 디스크립션 추가
				{
					CKTDGUIButton* pTitleButton = static_cast<CKTDGUIButton*>(m_pDLGWatch->GetControl( L"TitleButton" ));
					if( NULL != pTitleButton )
					{
						pTitleButton->SetShowEnable(true, true);
						wstring wstrDesc = L"#C00FFFF";
						wstrDesc += pTitle->m_wstrTitleName;
						wstrDesc += L"\n";
						wstrDesc += pTitle->m_wstrDescription;
						wstrDesc += g_pData->GetTitleManager()->GetTitleAbilityDesc( pTitle->m_iTitleID );

						pTitleButton->SetGuideDesc( wstrDesc.c_str() );
					}
				}
#endif // ADD_TITLE_DESCRIPTION

			}
		}

		if( true == bIsEmptyTitle )
		{
			pStaticTitleIcon->GetPicture(0)->SetTex( L"Title_Empty.tga" );

#ifdef ADD_TITLE_DESCRIPTION
			// 타이틀 디스크립션 숨기기
			{
				CKTDGUIButton* pTitleButton = static_cast<CKTDGUIButton*>(m_pDLGWatch->GetControl( L"TitleButton" ));
				if( NULL != pTitleButton )
				{
					pTitleButton->SetShowEnable( false, false );
				}
			}
#endif // ADD_TITLE_DESCRIPTION

		}
	}

	// 아이템 슬롯을 리셋해주고
	ResetEquipUI();

	// 슬롯을 켜자
	for ( int i = 0; i < (int)m_SlotList.size(); i++ )
	{
		CX2SlotItem* pItemSlot = (CX2SlotItem*)m_SlotList[i];
		pItemSlot->SetShow( true );
		pItemSlot->SetEnable( true );
	}


	m_bShow = true;
	m_pUnitViewerUI->SetShowObject(true);
	m_pDLGWatch->SetShowEnable(true, true);

	// 나 열렸어요~
	g_pData->GetUIManager()->UIOpened(CX2UIManager::UI_MENU_WATCH);
	return true;

}

void CX2UIWatch::ResetEquipUI()
{
	if( NULL == m_pTargetUnit )
		return;

	const CX2Unit::UnitData* pUnitData = &m_pTargetUnit->GetUnitData();
	const CX2Inventory& kInventory = m_pTargetUnit->GetInventory();

	for ( int i = 0; i < (int)m_SlotList.size(); i++ )
	{
		CX2SlotItem* pItemSlot = (CX2SlotItem*)GetSlot(i);
		if ( pItemSlot != NULL )
		{
			pItemSlot->DestroyItemUI();
		}
	}

	{
		for ( int i = 0; i < (int)pUnitData->m_NowEqipItemUIDList.size(); i++ )
		{
			UidType uidType = pUnitData->m_NowEqipItemUIDList[i];
			CX2Item* pItem = kInventory.GetItem( uidType );
			if ( pItem == NULL )
				continue;

			for ( int j = 0; j < (int)m_SlotList.size(); j++ )
			{
				CX2SlotItem* pItemSlot = (CX2SlotItem*)GetSlot(j);
				//{{ 09.06.02 태완 : 악세사리 류의 경우에는 fashion 조건 체크를 하지 않도록 예외처리 :
 				if ( pItemSlot->GetEquipPos() == pItem->GetItemTemplet()->GetEqipPosition() &&
 					(pItem->GetItemTemplet()->GetItemType() == CX2Item::IT_ACCESSORY || pItemSlot->GetFashion() == pItem->GetItemTemplet()->GetFashion()) )
				{
					if ( pItemSlot->IsCreatedItemUI() == true )
						pItemSlot->ResetItemUI( pItem );
					else
						pItemSlot->CreateItemUI( pItem );
					
					pItemSlot->SetShowImpossiblePicture(false);

					break;
				}
			}
		}
	}

}

void CX2UIWatch::SetPosition(D3DXVECTOR2 vec)
{
	if( NULL == m_pUnitViewerUI )
		return;
	// 슬롯 다이얼로그 유닛뷰어
	m_pDLGWatch->SetPos( vec );

	m_pUnitViewerUI->SetFixed( true );
	D3DXVECTOR3 UnitviewerPos = m_pDLGWatch->GetDummyPos(2);
	m_pUnitViewerUI->SetPositionOnScr( UnitviewerPos.x+vec.x , UnitviewerPos.y+vec.y, UnitviewerPos.z, 1.15f );		// 유닛뷰어 위치는 스크립트에 있음
	
	for ( int i = 0; i < (int)m_SlotList.size(); i++ )
	{
		CX2SlotItem* pItemSlot = (CX2SlotItem*)m_SlotList[i];
		D3DXVECTOR2 pos;
		pos = pItemSlot->GetPos();

		pos -= m_MovedPosition;
		pos += vec;

		pItemSlot->SetPos(pos);
	}
	m_MovedPosition = vec;
}

void CX2UIWatch::SetLayer(X2_DIALOG_LAYER layer)
{
	SetSlotManagerLayer(layer);
	SetSlotManagerChangeSequence(false);

	if(m_pDLGWatch != NULL)
	{
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer(m_pDLGWatch, layer);
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDLGWatch, false );
	}
	
	// 유닛뷰어는 다이얼로그랑 같이 그려진다.

}

// 유닛뷰어 관련
void CX2UIWatch::ResetUnitViewerUI()
{
	if( NULL == m_pTargetUnit )
	{
		return;
	}


	if ( m_pUnitViewerUI == NULL )
	{
		m_pUnitViewerUI = CX2UnitViewerUI::CreateUnitViewerUI();
#ifdef SEPARATION_MOTION
		m_pUnitViewerUI->SetUnit( m_pTargetUnit, CX2UnitViewerUI::UVS_WATCH ); 
#else
		m_pUnitViewerUI->SetUnit(m_pTargetUnit);
#endif

		if( m_pDLGWatch != NULL )
		{
			m_pDLGWatch->SetHasUnit( (CKTDGObject*)m_pUnitViewerUI );
		}

		//g_pKTDXApp->GetDGManager()->AddObjectChain( m_pUnitViewerUI );

		m_pUnitViewerUI->GetMatrix().Rotate(D3DXToRadian(0),D3DXToRadian(0),D3DXToRadian(0));
		
		if( NULL != m_pUnitViewerUI->GetXSkinAnim() )
		{
			m_pUnitViewerUI->GetXSkinAnim()->SetApplyMotionOffset( false );
		}
				
		// 위치정보 : SetPosition에도 있음. 주의!!
		m_pUnitViewerUI->SetFixed( true );
		D3DXVECTOR3 UnitviewerPos = m_pDLGWatch->GetDummyPos(2);
		m_pUnitViewerUI->SetPositionOnScr( UnitviewerPos.x + m_MovedPosition.x, UnitviewerPos.y + m_MovedPosition.y, UnitviewerPos.z, 1.15f );		// 유닛뷰어 위치는 스크립트에 있음
		D3DXVECTOR3 UnitviewerLightPos = m_pDLGWatch->GetDummyPos(3);	// 광원 위치도 스크립트로 뺐음
		m_pUnitViewerUI->SetLightPos( UnitviewerLightPos );

		m_pUnitViewerUI->OnFrameMove( g_pKTDXApp->GetTime(),g_pKTDXApp->GetElapsedTime() );
	}
	else
	{
#ifdef SEPARATION_MOTION
		m_pUnitViewerUI->SetUnit( m_pTargetUnit, CX2UnitViewerUI::UVS_WATCH ); 
#else
		m_pUnitViewerUI->SetUnit( m_pTargetUnit );
#endif
	}

	if ( m_pUnitViewerUI != NULL )
	{
		m_pUnitViewerUI->SetShowObject( m_bShow );
		m_pUnitViewerUI->SetHideSelectUnitWeapon();
	}
}

bool CX2UIWatch::IsInUnitClickReact( D3DXVECTOR2 mousePos )
{
	D3DXVECTOR2 m_ConvertPos		= g_pKTDXApp->ConvertByResolution( m_UnitClickPos + m_MovedPosition );
	D3DXVECTOR2 m_ConvertSize	= g_pKTDXApp->ConvertByResolution( m_UnitClickSize );

	if ( mousePos.x >= m_ConvertPos.x   && mousePos.x <= m_ConvertPos.x + m_ConvertSize.x &&
		mousePos.y >= m_ConvertPos.y && mousePos.y <= m_ConvertPos.y + m_ConvertSize.y )
	{
		return true;
	}

	return false;
}



void CX2UIWatch::UnitHandling( double fTime, float fElapsedTime )
{
	if( NULL == m_pTargetUnit )
		return;

	if( m_pUnitViewerUI != NULL )
	{
		float fRotSpeed = 13.0f;


		if ( m_bUnitClicked == true )
		{
			D3DXVECTOR3 RotateY = D3DXVECTOR3(0, -((float)g_pKTDXApp->GetDIManager()->GetMouse()->GetXDelta() * m_fMouseSensitivity), 0);
			m_pUnitViewerUI->GetMatrix().RotateRel(RotateY);
			m_bRotateReturn = false;

			fRotSpeed = 13.0f;
		}
		else
		{
			D3DXVECTOR3 rot = m_pUnitViewerUI->GetMatrix().GetRotate();



			m_pUnitViewerUI->GetMatrix().Rotate( D3DXVECTOR3(0,0,0) );
			int addSpeed = 20;

			if ( m_bRotateReturn == false )
			{
				fRotSpeed -= (fElapsedTime * addSpeed);

				if ( fRotSpeed <= 0 )
				{
					fRotSpeed = fElapsedTime;
				}

				if (m_fRemainRotY >= 0.0f &&  m_fRemainRotY < 3.14f )
				{

					if ( m_fRemainRotY <= 0.0f )
					{
						m_pUnitViewerUI->GetMatrix().Rotate( D3DXVECTOR3(0,0,0) );
						m_fRemainRotY = 0.0f;
						m_bRotateReturn = true;
					}
					else
					{
						m_fRemainRotY -= (fElapsedTime*fRotSpeed);
						m_pUnitViewerUI->GetMatrix().Rotate(D3DXVECTOR3(0,m_fRemainRotY,0));

						if ( m_fRemainRotY <= 0.0f )
						{
							m_pUnitViewerUI->GetMatrix().Rotate( D3DXVECTOR3(0,0,0) );
							m_fRemainRotY = 0.0f;
							m_bRotateReturn = true;
						}
					}
				}
				else if ( m_fRemainRotY > 3.14f && m_fRemainRotY <= 6.18f  )
				{
					if ( m_fRemainRotY >= 6.18f )
					{
						m_bRotateReturn = true;
						m_fRemainRotY = 0.0f;
						m_pUnitViewerUI->GetMatrix().Rotate( D3DXVECTOR3(0,0,0) );
					}
					else
					{
						m_fRemainRotY += (fElapsedTime*fRotSpeed);
						m_pUnitViewerUI->GetMatrix().Rotate(D3DXVECTOR3(0,m_fRemainRotY,0));

						if ( m_fRemainRotY >= 6.18f )
						{
							m_bRotateReturn = true;
							m_fRemainRotY = 0.0f;
							m_pUnitViewerUI->GetMatrix().Rotate( D3DXVECTOR3(0,0,0) );
						}
					}
				}
				else if ( m_fRemainRotY >= -3.14f && m_fRemainRotY <= 0.0f )
				{
					if ( m_fRemainRotY >= 0.0f )
					{
						m_bRotateReturn = true;
						m_fRemainRotY = 0.0f;
						m_pUnitViewerUI->GetMatrix().Rotate( D3DXVECTOR3(0,0,0) );
					}
					else
					{
						m_fRemainRotY += (fElapsedTime*fRotSpeed);
						m_pUnitViewerUI->GetMatrix().Rotate(D3DXVECTOR3(0,m_fRemainRotY,0));

						if ( m_fRemainRotY >= 0.0f )
						{
							m_bRotateReturn = true;
							m_fRemainRotY = 0.0f;
							m_pUnitViewerUI->GetMatrix().Rotate( D3DXVECTOR3(0,0,0) );
						}
					}
				}
				else if (  m_fRemainRotY >= -6.28f && m_fRemainRotY <= -3.14f )
				{
					if ( m_fRemainRotY <= -6.28f )
					{
						m_bRotateReturn = true;
						m_fRemainRotY = 0.0f;
						m_pUnitViewerUI->GetMatrix().Rotate( D3DXVECTOR3(0,0,0) );
					}
					else
					{
						m_fRemainRotY -= (fElapsedTime*fRotSpeed);
						m_pUnitViewerUI->GetMatrix().Rotate(D3DXVECTOR3(0,m_fRemainRotY,0));

						if ( m_fRemainRotY <= -6.28f )
						{
							m_bRotateReturn = true;
							m_fRemainRotY = 0.0f;
							m_pUnitViewerUI->GetMatrix().Rotate( D3DXVECTOR3(0,0,0) );
						}
					}

				}
			}
		}

		m_pUnitViewerUI->OnFrameMove( fTime, fElapsedTime );

		if ( NULL != m_pUnitViewerUI->GetXSkinAnim() && m_pUnitViewerUI->GetXSkinAnim()->GetState() == CKTDGXSkinAnim::XAP_ONE_WAIT )
		{
			m_pUnitViewerUI->PlayByMotionType( CX2UnitViewerUI::UVUMT_WAIT );
		}
	}

}



wstring CX2UIWatch::GetSlotItemDesc()
{
	
	wstring itemDesc = L"";

	if ( m_pNowOverItemSlot != NULL )
	{
		if ( m_pNowOverItemSlot->GetDialog() == NULL )
		{
			itemDesc = m_pNowOverItemSlot->GetSlotDesc();
		}
		else
		{	
			// SlotItemDescByTID 새로 만듬 : 소켓 정보, 기간 정보 보여주지 않음
			if( m_pTargetUnit != NULL )
			{
				const CX2Inventory& kInventory = m_pTargetUnit->GetInventory();
				{
					CX2Item* pItem = kInventory.GetItem( m_pNowOverItemSlot->GetItemUID() );
					if(pItem != NULL)
					{
						//pItem->GetItemData().m_Endurance = pItem->GetItemTemplet()->m_Endurance;
						itemDesc = GetSlotItemDescByTID( pItem, pItem->GetItemData().m_ItemID, false );
					}
				}
			}
		}
	}

	return itemDesc;
}


// ItemSlotManager 것 쓰지 않고 새로 만듬 : 소켓 정보와 아이템 기간을 보여주지 않기 위해
std::wstring CX2UIWatch::GetSlotItemDescByTID( CX2Item* pItem, int itemTID, bool bItemDescExtra /*= true*/, bool bMyUnitItem_ /*= false*/ )
{
	if( NULL == m_pTargetUnit )
		return L"";

	if( NULL == pItem)
		return L"";
	
	wstring slotItemDesc = L"";
#ifdef FIX_LONG_ITEM_DESCRIPTION
	WCHAR buff[512] = {0,};
#else FIX_LONG_ITEM_DESCRIPTION
	WCHAR buff[256] = {0,};
#endif FIX_LONG_ITEM_DESCRIPTION

	CX2Unit* pkUnit = g_pData->GetMyUser()->GetSelectUnit();
	const CX2Item::ItemTemplet* pkItemTemplet = g_pData->GetItemManager()->GetItemTemplet( itemTID );
	if ( pkItemTemplet != NULL )
	{
		slotItemDesc += pkItemTemplet->GetNameColor_();

		// 강화 레벨

        CX2Item::ITEM_TYPE eItemType = pkItemTemplet->GetItemType();

		if ( ( eItemType == CX2Item::IT_WEAPON || eItemType == CX2Item::IT_DEFENCE ) &&
			pkItemTemplet->GetFashion() == false && pItem != NULL )
		{
			{
#ifdef ITEM_RECOVERY_TEST
				StringCchPrintfW( buff, ARRAY_SIZE(buff), L"+%d ", abs(pItem->GetItemData().m_EnchantLevel) );
#else
				StringCchPrintfW( buff, ARRAY_SIZE(buff), L"+%d ", pItem->GetItemData().m_EnchantLevel );
#endif
				slotItemDesc += buff;
			}


		}

		// 아이템 이름
        slotItemDesc += pkItemTemplet->GetFullName_();
		slotItemDesc += L"#CX\n";


		// 아이템 등급
		switch( pkItemTemplet->GetItemGrade() )
		{
		case CX2Item::IG_UNIQUE:
			{
				slotItemDesc += L"[";
                slotItemDesc += GET_STRING( STR_ID_935 );
                slotItemDesc += L"]";
			} break;
		case CX2Item::IG_ELITE:
			{
                slotItemDesc += L"[";
                slotItemDesc += GET_STRING( STR_ID_936 );
                slotItemDesc += L"]";
			} break;
		case CX2Item::IG_RARE:
			{
                slotItemDesc += L"[";
                slotItemDesc += GET_STRING( STR_ID_937 );
                slotItemDesc += L"]";
			} break;
		case CX2Item::IG_NORMAL:
			{
                slotItemDesc += L"[";
                slotItemDesc += GET_STRING( STR_ID_938 );
                slotItemDesc += L"]";
			} break;
		case CX2Item::IG_LOW:
			{
                slotItemDesc += L"[";
                slotItemDesc += GET_STRING( STR_ID_939 );
                slotItemDesc += L"]";
			} break;
		default:
			{
				// nothing
			} break;
		}
#ifdef ITEM_RECOVERY_TEST
		if(pItem != NULL && true == pItem->IsDisabled())
		{
            slotItemDesc += L" ";
            slotItemDesc += GET_STRING( STR_ID_940 );
		}			
#endif //ITEM_RECOVERY_TEST
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05 //
		else if ( NULL != pItem && false == pItem->GetIsEvaluation() )
		{
			slotItemDesc += L" ";
			slotItemDesc += GET_STRING( STR_ID_940 );
		}
#endif // SERV_NEW_ITEM_SYSTEM_2013_05

		slotItemDesc += L"#CX\n\n";

#ifdef SERV_NEW_ITEM_SYSTEM_2013_05 // 오현빈
		CX2Unit::UNIT_TYPE eUnitType = CX2Unit::UT_NONE;
		CX2Unit::UNIT_CLASS eUnitClass = CX2Unit::UC_NONE;
		if( true == bMyUnitItem_ )
		{
			eUnitType = pkUnit->GetType();
			eUnitClass = pkUnit->GetClass();
		}
		else
		{
			eUnitType = m_pTargetUnit->GetType();
			eUnitClass = m_pTargetUnit->GetClass();
		}
#endif // SERV_NEW_ITEM_SYSTEM_2013_05

		if ( eItemType == CX2Item::IT_WEAPON || 
			eItemType == CX2Item::IT_DEFENCE ||
			eItemType == CX2Item::IT_ACCESSORY )
		{
			// - - - - - - - - - - - - - - - - - - - - - - - - -
			// 장착 위치
			wstring equipPos;

			//{{ 2009.01.19 김태완 : 코드정리 elseif -> switch
			switch(pkItemTemplet->GetEqipPosition())
			{
			case CX2Unit::EP_DEFENCE_HAIR:
				{
					equipPos = GET_STRING( STR_ID_263 );
				} break;
			case CX2Unit::EP_DEFENCE_BODY:
				{
					equipPos = GET_STRING( STR_ID_264 );
				} break;
			case CX2Unit::EP_AC_BODY:
				{	
					equipPos = GET_STRING( STR_ID_265 );
				} break;
			case CX2Unit::EP_DEFENCE_LEG:
				{
					equipPos = GET_STRING( STR_ID_266 );
				} break;
			case CX2Unit::EP_AC_LEG:
				{
					equipPos = GET_STRING( STR_ID_267 );
				} break;
			case CX2Unit::EP_DEFENCE_HAND:
				{
					equipPos = GET_STRING( STR_ID_268 );
				} break;
			case CX2Unit::EP_DEFENCE_FOOT:
				{
					equipPos = GET_STRING( STR_ID_269 );
				} break;
			case CX2Unit::EP_WEAPON_HAND:
				{
					equipPos = GET_STRING( STR_ID_270 );
				} break;
			case CX2Unit::EP_AC_FACE1:
				{
					equipPos = GET_STRING( STR_ID_271 );
				} break;
			case CX2Unit::EP_AC_FACE2:
				{
					equipPos = GET_STRING( STR_ID_272 );
				} break;
			case CX2Unit::EP_AC_FACE3:
				{
					equipPos = GET_STRING( STR_ID_273 );
				} break;
			case CX2Unit::EP_AC_RING:
				{
					equipPos = GET_STRING( STR_ID_274 );
				} break;
			case CX2Unit::EP_AC_NECKLESS:
				{
					equipPos = GET_STRING( STR_ID_275 );
				} break;
			case CX2Unit::EP_AC_ARM:
				{
					equipPos = GET_STRING( STR_ID_276 );
				} break;
			case CX2Unit::EP_AC_WEAPON:
				{
					equipPos = GET_STRING( STR_ID_277 );
				} break;
#ifdef SERV_NEW_ONE_PIECE_AVATAR_SLOT
			case CX2Unit::EP_ONEPIECE_FASHION:
				{
					equipPos = GET_STRING( STR_ID_28362 );
				} break;
#endif //SERV_NEW_ONE_PIECE_AVATAR_SLOT
			default:
				break;
			}//}} elseif -> switch

			wstring fashionItem = L"";
			if ( pkItemTemplet->GetFashion() == true )
			{
				fashionItem = GET_STRING( STR_ID_251 );
                fashionItem += L" ";
			}


			// - - - - - - - - - - - - - - - - - - - - - - - - -
			// 유닛 클래스
			const CX2Unit::UnitTemplet* pUnitTemplet = g_pData->GetUnitManager()->GetUnitTemplet( pkItemTemplet->GetUnitClass() );
			if( NULL != pUnitTemplet )
			{
				StringCchPrintfW( buff, ARRAY_SIZE(buff), GET_REPLACED_STRING( ( STR_ID_278, "LLL", pUnitTemplet->m_Description, fashionItem, equipPos ) ) );
			}
			else
			{
				StringCchPrintfW( buff, ARRAY_SIZE(buff), L"%s%s\n", fashionItem.c_str(), equipPos.c_str() );
			}


			if ( pkItemTemplet->GetUnitClass() != CX2Unit::UC_NONE
				&& GetX2UnitClassCompatibility( pkUnit->GetClass(), pkItemTemplet->GetUnitClass() ) == false )
			{
				slotItemDesc += L"#CFF0000";
				slotItemDesc += buff;
				slotItemDesc += L"#CX";
			}
			else
			{
				slotItemDesc += buff;
				slotItemDesc += L"#CX";
			}

			// 아이템 레벨
			if ( 0 != pkItemTemplet->GetItemLevel() )
			{
				int iIncreasedLevelByEnchant = 0;
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
				iIncreasedLevelByEnchant = ( NULL != pItem ? pItem->GetIEchantedItemLevel(eUnitType, eUnitClass) : 0 );
#else
				iIncreasedLevelByEnchant = ( NULL != pItem ? pItem->GetIEchantedItemLevel() : 0 );
#endif // SERV_NEW_ITEM_SYSTEM_2013_05

				slotItemDesc += L"\n";
#ifdef HARDCODING_STRING_TO_INDEX
				slotItemDesc += GET_STRING(STR_ID_24591);
#else
				slotItemDesc += L"아이템 레벨: ";
#endif HARDCODING_STRING_TO_INDEX
				slotItemDesc += GET_REPLACED_STRING( (STR_ID_2632, "i", pkItemTemplet->GetItemLevel() + iIncreasedLevelByEnchant ) );
			}

			// - - - - - - - - - - - - - - - - - - - - - - - - -
			// 스탯 관련
			//wsprintf( buff, GET_REPLACED_STRING( ( STR_ID_279, "i", ( int )pkItemTemplet->m_UseLevel ) ) );
			StringCchPrintf( buff, 256, GET_REPLACED_STRING( ( STR_ID_279, "i", ( int )pkItemTemplet->GetUseLevel() ) ) );
			if ( pkUnit->GetUnitData().m_Level < pkItemTemplet->GetUseLevel() ) 
			{
				slotItemDesc += L"\n";
				slotItemDesc += L"#CFF0000";
				slotItemDesc += buff;
				slotItemDesc += L"#CX";
			}
			else
			{
				slotItemDesc += L"\n";
				slotItemDesc += buff;
				slotItemDesc += L"#CX";
			}

			//{{ kimhc // 2011-07-05 // 옵션데이타 수치화 작업
			// 요구 레벨이 없고, 착용 할 수 있으면
			if ( 0 == pkItemTemplet->GetUseLevel() && false == pkItemTemplet->GetNoEquip() )
			{
				// 연동레벨을 보여줌
				if( NULL != m_pTargetUnit )
					slotItemDesc += GET_REPLACED_STRING( ( STR_ID_12772, "i", static_cast<int>( m_pTargetUnit->GetUnitData().m_Level ) ) );
			}
			//}} kimhc // 2011-07-05 // 옵션데이타 수치화 작업

			// 내구도 보여주지 않는다.
// 			if ( pkItemTemplet->m_PeriodType == CX2Item::PT_ENDURANCE )
// 			{
// 				if ( pItem != NULL )
// 				{
// 					int endurancePer = (int)( (pItem->GetItemData().m_Endurance / (float)pkItemTemplet->m_Endurance)  * 100.0f );
// 					//wsprintf( buff, L"내구도 : %d%%\n\n", endurancePer );
//					StringCchPrintf( buff, 256, L"내구도 : %d%%\n\n", endurancePer );
// 					if ( endurancePer <= 10 )
// 					{
// 						slotItemDesc += L"#CFF0000";
// 					}
// 					slotItemDesc += buff;
// 				}
// 				else
// 				{
// 					slotItemDesc += L"내구도 : 100%\n\n";
// 				}
// 			}
// 			slotItemDesc += L"#CX";


			bool bCheckIsEqiupping = false;

			CX2Item* pCompareItem = NULL;

			pCompareItem = GetCompareItem( pkItemTemplet, pItem, bCheckIsEqiupping );

			//임시로 만약에 능력치 보려고 하는 아이템이 
			//자기 아이템 타입의 아이템이 아니라면 장비중인걸로 표시해버리자

			if ( pkItemTemplet->GetUseCondition() != CX2Item::UC_ANYONE &&
				g_pData->GetMyUser()->GetSelectUnit()->GetUnitTemplet()->m_UnitType != pkItemTemplet->GetUnitType() )
				bCheckIsEqiupping = true;

            wstring wstrBuff;

#ifdef	X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
	#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
			// 스탯 추가 레벨 소켓 옵션 적용
			CX2SocketItem::KItemStatRelLVData kItemStatRelLVData;
			if( NULL != pItem )
				g_pData->GetItemStatCalculator().GetSocketOptionStatRelLV(kItemStatRelLVData, pItem->GetItemData(), pkItemTemplet);

			// 스탯테이블을 통해 스탯 계산
			CX2Item::KItemFormatStatData kCalculateStat;
			g_pData->GetItemStatCalculator().CalculateItemStat(kCalculateStat, pkItemTemplet, eUnitType, eUnitClass );

			const CX2Item::KItemFormatStatData& kTempletStat = kCalculateStat;
	#else
			const CX2Item::KItemFormatStatData& kTempletStat = pkItemTemplet->GetStatData();
	#endif //SERV_NEW_ITEM_SYSTEM_2013_05
#else   //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
            const CX2Stat::Stat& kTempletStat = pkItemTemplet->GetStat();
#endif  //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING


#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
			// 물리 공격력
			int iAtkPhysic = static_cast<int>(pItem->GetStat(eUnitType, eUnitClass ).m_fAtkPhysic);
			if ( 0 != iAtkPhysic )
			{
				if ( bCheckIsEqiupping == true )
				{
					wstrBuff = GET_REPLACED_STRING( ( STR_ID_281, "h", iAtkPhysic ) );
				}
				else
				{
					if ( pCompareItem != NULL )
					{

						if ( iAtkPhysic > (int)pCompareItem->GetStat().m_fAtkPhysic  )
						{
							wstrBuff = GET_REPLACED_STRING( ( STR_ID_282, "h", iAtkPhysic ) );

						}
						else if ( (int)pCompareItem->GetStat().m_fAtkPhysic == iAtkPhysic )
						{
							wstrBuff = GET_REPLACED_STRING( ( STR_ID_283, "h", iAtkPhysic ) );
						}
						else
						{
							wstrBuff = GET_REPLACED_STRING( ( STR_ID_284, "h", iAtkPhysic ) );
						}
					}
					else
					{
						if( iAtkPhysic < 0.f)
						{
							wstrBuff = GET_REPLACED_STRING( ( STR_ID_284, "h", iAtkPhysic) );
						}
						else
						{
							wstrBuff = GET_REPLACED_STRING( ( STR_ID_282, "h", iAtkPhysic ) );
						}	                        
					}
				}

				slotItemDesc += wstrBuff;
				slotItemDesc += L"#CX";
			}

			// 마법 공격력
			int iAtkMagic = static_cast<int>(pItem->GetStat(eUnitType, eUnitClass ).m_fAtkMagic);
			if ( 0 != iAtkMagic )
			{
				if ( bCheckIsEqiupping == true )
				{
					wstrBuff = GET_REPLACED_STRING( ( STR_ID_285, "h", iAtkMagic ) );
				}
				else
				{
					if ( pCompareItem != NULL )
					{

						if ( iAtkMagic > (int)pCompareItem->GetStat().m_fAtkMagic  )
						{
							wstrBuff = GET_REPLACED_STRING( ( STR_ID_286, "h", iAtkMagic ) );

						}
						else if ( (int)pCompareItem->GetStat().m_fAtkMagic == iAtkMagic )
						{
							wstrBuff = GET_REPLACED_STRING( ( STR_ID_287, "h", iAtkMagic ) );
						}
						else
						{
							wstrBuff = GET_REPLACED_STRING( ( STR_ID_288, "h", iAtkMagic ) );
						}
					}
					else
					{
						if( iAtkMagic < 0.f)
						{
							wstrBuff = GET_REPLACED_STRING( ( STR_ID_288, "h", iAtkMagic) );
						}
						else
						{
							wstrBuff = GET_REPLACED_STRING( ( STR_ID_286, "h", iAtkMagic ) );
						}	                        
					}
				}

				slotItemDesc += wstrBuff;
				slotItemDesc += L"#CX";
			}


			// 물리 방어력
			int iDefPhysic = static_cast<int>(pItem->GetStat(eUnitType, eUnitClass ).m_fDefPhysic);
			if ( 0 != iDefPhysic )
			{
				if ( bCheckIsEqiupping == true )
				{
					wstrBuff = GET_REPLACED_STRING( ( STR_ID_289, "h", iDefPhysic ) );
				}
				else
				{
					if ( pCompareItem != NULL )
					{

						if ( iDefPhysic > (int)pCompareItem->GetStat().m_fDefPhysic  )
						{
							wstrBuff = GET_REPLACED_STRING( ( STR_ID_290, "h", iDefPhysic ) );

						}
						else if ( (int)pCompareItem->GetStat().m_fDefPhysic == iDefPhysic )
						{
							wstrBuff = GET_REPLACED_STRING( ( STR_ID_291, "h", iDefPhysic ) );
						}
						else
						{
							wstrBuff = GET_REPLACED_STRING( ( STR_ID_292, "h", iDefPhysic ) );
						}
					}
					else
					{
						if( iDefPhysic < 0.f)
						{
							wstrBuff = GET_REPLACED_STRING( ( STR_ID_292, "h", iDefPhysic) );
						}
						else
						{
							wstrBuff = GET_REPLACED_STRING( ( STR_ID_290, "h", iDefPhysic ) );
						}	                        
					}
				}

				slotItemDesc += wstrBuff;
				slotItemDesc += L"#CX";
			}

			// 마법 방어력
			int iDefMagic = static_cast<int>(pItem->GetStat(eUnitType, eUnitClass ).m_fDefMagic);
			if ( 0 != iDefMagic )
			{
				if ( bCheckIsEqiupping == true )
				{
					wstrBuff = GET_REPLACED_STRING( ( STR_ID_293, "h", iDefMagic ) );
				}
				else
				{
					if ( pCompareItem != NULL )
					{

						if ( iDefMagic > (int)pCompareItem->GetStat().m_fDefMagic  )
						{
							wstrBuff = GET_REPLACED_STRING( ( STR_ID_294, "h", iDefMagic ) );

						}
						else if ( (int)pCompareItem->GetStat().m_fDefMagic == iDefMagic )
						{
							wstrBuff = GET_REPLACED_STRING( ( STR_ID_295, "h", iDefMagic ) );
						}
						else
						{
							wstrBuff = GET_REPLACED_STRING( ( STR_ID_296, "h", iDefMagic ) );
						}
					}
					else
					{
						if( iDefMagic < 0.f)
						{
							wstrBuff = GET_REPLACED_STRING( ( STR_ID_296, "h", iDefMagic) );
						}
						else
						{
							wstrBuff = GET_REPLACED_STRING( ( STR_ID_294, "h", iDefMagic ) );
						}	                        
					}
				}

				slotItemDesc += wstrBuff;
				slotItemDesc += L"#CX";
			}

			// 체력
			int iBaseHP = static_cast<int>(pItem->GetStat(eUnitType, eUnitClass ).m_fBaseHP);
			if ( 0 != iBaseHP )
			{
				if ( bCheckIsEqiupping == true )
				{
					wstrBuff = GET_REPLACED_STRING( ( STR_ID_297, "h", iBaseHP ) );
				}
				else
				{
					if ( pCompareItem != NULL )
					{

						if ( iBaseHP > (int)pCompareItem->GetStat().m_fBaseHP  )
						{
							wstrBuff = GET_REPLACED_STRING( ( STR_ID_298, "h", iBaseHP ) );

						}
						else if ( (int)pCompareItem->GetStat().m_fBaseHP == iBaseHP )
						{
							wstrBuff = GET_REPLACED_STRING( ( STR_ID_299, "h", iBaseHP ) );
						}
						else
						{
							wstrBuff = GET_REPLACED_STRING( ( STR_ID_300, "h", iBaseHP ) );
						}
					}
					else
					{
						if( iBaseHP < 0.f)
						{
							wstrBuff = GET_REPLACED_STRING( ( STR_ID_300, "h", iBaseHP) );
						}
						else
						{
							wstrBuff = GET_REPLACED_STRING( ( STR_ID_298, "h", iBaseHP ) );
						}	                        
					}
				}

				slotItemDesc += wstrBuff;
			}
#else
			//////////////////////////////////////////////////////////////////////////
			
            if ( pItem != NULL && pItem->GetStat().m_fAtkPhysic != 0 )
            {
                if ( bCheckIsEqiupping == true )
                {
                    wstrBuff = GET_REPLACED_STRING( ( STR_ID_281, "h", ( int )pItem->GetStat().m_fAtkPhysic ) );
                }
                else
                {
                    if ( pCompareItem != NULL )
                    {

                        if ( (int)pItem->GetStat().m_fAtkPhysic > (int)pCompareItem->GetStat().m_fAtkPhysic  )
                        {
                            wstrBuff = GET_REPLACED_STRING( ( STR_ID_282, "h", ( int )pItem->GetStat().m_fAtkPhysic ) );

                        }
                        else if ( (int)pCompareItem->GetStat().m_fAtkPhysic == (int)pItem->GetStat().m_fAtkPhysic )
                        {
                            wstrBuff = GET_REPLACED_STRING( ( STR_ID_283, "h", ( int )pItem->GetStat().m_fAtkPhysic ) );
                        }
                        else
                        {
                            wstrBuff = GET_REPLACED_STRING( ( STR_ID_284, "h", ( int )pItem->GetStat().m_fAtkPhysic ) );
                        }
                    }
                    else
                    {
						if(pItem->GetStat().m_fAtkPhysic < 0.f)
						{
							wstrBuff = GET_REPLACED_STRING( ( STR_ID_284, "h", ( int )pItem->GetStat().m_fAtkPhysic ) );
						}
						else
						{
							wstrBuff = GET_REPLACED_STRING( ( STR_ID_282, "h", ( int )pItem->GetStat().m_fAtkPhysic ) );
						}	                        
                    }
                }

                slotItemDesc += wstrBuff;
				slotItemDesc += L"#CX";
            }
            else if ( kTempletStat.m_fAtkPhysic != 0 )
            {
                if ( bCheckIsEqiupping == true )
                {
                    wstrBuff = GET_REPLACED_STRING( ( STR_ID_281, "h", ( int )kTempletStat.m_fAtkPhysic ) );
                }
                else
                {
                    if ( pCompareItem != NULL )
                    {

                        if ( (int)kTempletStat.m_fAtkPhysic > (int)pCompareItem->GetStat().m_fAtkPhysic  )
                        {
                            wstrBuff = GET_REPLACED_STRING( ( STR_ID_282, "h", ( int )kTempletStat.m_fAtkPhysic ) );

                        }
                        else if ( (int)pCompareItem->GetStat().m_fAtkPhysic == (int)kTempletStat.m_fAtkPhysic )
                        {
                            wstrBuff = GET_REPLACED_STRING( ( STR_ID_283, "h", ( int )kTempletStat.m_fAtkPhysic ) );
                        }
                        else
                        {
                            wstrBuff = GET_REPLACED_STRING( ( STR_ID_284, "h", ( int )kTempletStat.m_fAtkPhysic ) );
                        }
                    }
                    else
                    {
						if(kTempletStat.m_fAtkPhysic < 0.f)
						{
							wstrBuff = GET_REPLACED_STRING( ( STR_ID_284, "h", ( int )kTempletStat.m_fAtkPhysic ) );
						}
						else
						{
							wstrBuff = GET_REPLACED_STRING( ( STR_ID_282, "h", ( int )kTempletStat.m_fAtkPhysic ) );
						}	                        
                    }
                }

                slotItemDesc += wstrBuff;
				slotItemDesc += L"#CX";
            }

            if ( pItem != NULL && pItem->GetStat().m_fAtkMagic != 0 )
            {
                if ( bCheckIsEqiupping == true )
                {
                    wstrBuff = GET_REPLACED_STRING( ( STR_ID_285, "h", ( int )pItem->GetStat().m_fAtkMagic ) );
                }
                else
                {
                    if ( pCompareItem != NULL )
                    {
                        if ( (int)pItem->GetStat().m_fAtkMagic > (int)pCompareItem->GetStat().m_fAtkMagic )
                        {
                            wstrBuff = GET_REPLACED_STRING( ( STR_ID_286, "h", ( int )pItem->GetStat().m_fAtkMagic ) );

                        }
                        else if ( (int)pCompareItem->GetStat().m_fAtkMagic == (int)pItem->GetStat().m_fAtkMagic )
                        {
                            wstrBuff = GET_REPLACED_STRING( ( STR_ID_287, "h", ( int )pItem->GetStat().m_fAtkMagic ) );

                        }
                        else
                        {	
                            wstrBuff = GET_REPLACED_STRING( ( STR_ID_288, "h", ( int )pItem->GetStat().m_fAtkMagic ) );
                        }
                    }
                    else
                    {
						if(pItem->GetStat().m_fAtkMagic < 0.f)
						{
							wstrBuff = GET_REPLACED_STRING( ( STR_ID_288, "h", ( int )pItem->GetStat().m_fAtkMagic ) );
						}
						else
						{
							wstrBuff = GET_REPLACED_STRING( ( STR_ID_286, "h", ( int )pItem->GetStat().m_fAtkMagic ) );
						}
                        
                    }
                }

                slotItemDesc += wstrBuff;
				slotItemDesc += L"#CX";
            }
            else if ( kTempletStat.m_fAtkMagic != 0 )
            {
                if ( bCheckIsEqiupping == true )
                {
                    wstrBuff = GET_REPLACED_STRING( ( STR_ID_285, "h", ( int )kTempletStat.m_fAtkMagic ) );
                }
                else
                {
                    if ( pCompareItem != NULL )
                    {
                        if ( (int)kTempletStat.m_fAtkMagic > (int)pCompareItem->GetStat().m_fAtkMagic )
                        {
                            wstrBuff = GET_REPLACED_STRING( ( STR_ID_286, "h", ( int )kTempletStat.m_fAtkMagic ) );

                        }
                        else if ( (int)pCompareItem->GetStat().m_fAtkMagic == (int)kTempletStat.m_fAtkMagic )
                        {
                            wstrBuff = GET_REPLACED_STRING( ( STR_ID_287, "h", ( int )kTempletStat.m_fAtkMagic ) );

                        }
                        else
                        {
                            wstrBuff = GET_REPLACED_STRING( ( STR_ID_288, "h", ( int )kTempletStat.m_fAtkMagic ) );
                        }
                    }
                    else
                    {
						if(kTempletStat.m_fAtkMagic < 0.f)
						{
							wstrBuff = GET_REPLACED_STRING( ( STR_ID_288, "h", ( int )kTempletStat.m_fAtkMagic ) );
						}
						else
						{
							wstrBuff = GET_REPLACED_STRING( ( STR_ID_286, "h", ( int )kTempletStat.m_fAtkMagic ) );
						}
                    }
                }

                slotItemDesc += wstrBuff;
				slotItemDesc += L"#CX";
            }

            if ( pItem != NULL && pItem->GetStat().m_fDefPhysic != 0 )
            {
                if ( bCheckIsEqiupping == true )
                {
                    wstrBuff = GET_REPLACED_STRING( ( STR_ID_289, "h", ( int )pItem->GetStat().m_fDefPhysic ) );
                }
                else
                {
                    if ( pCompareItem != NULL )
                    {
                        if ( (int)pItem->GetStat().m_fDefPhysic > (int)pCompareItem->GetStat().m_fDefPhysic )
                        {
                            wstrBuff = GET_REPLACED_STRING( ( STR_ID_290, "h", ( int )pItem->GetStat().m_fDefPhysic ) );
                        }
                        else if ( (int)pCompareItem->GetStat().m_fDefPhysic == (int)pItem->GetStat().m_fDefPhysic )
                        {
                            wstrBuff = GET_REPLACED_STRING( ( STR_ID_291, "h", ( int )pItem->GetStat().m_fDefPhysic ) );
                        }
                        else
                        {	
                            wstrBuff = GET_REPLACED_STRING( ( STR_ID_292, "h", ( int )pItem->GetStat().m_fDefPhysic ) );
                        }
                    }
                    else
                    {
						if(pItem->GetStat().m_fDefPhysic < 0.f)
						{
							wstrBuff = GET_REPLACED_STRING( ( STR_ID_292, "h", ( int )pItem->GetStat().m_fDefPhysic ) );
						}
						else
						{
							wstrBuff = GET_REPLACED_STRING( ( STR_ID_290, "h", ( int )pItem->GetStat().m_fDefPhysic ) );
						}
                        
                    }
                }

                slotItemDesc += wstrBuff;
				slotItemDesc += L"#CX";
            }
            else if ( kTempletStat.m_fDefPhysic != 0 )
            {
                if ( bCheckIsEqiupping == true )
                {
                    wstrBuff = GET_REPLACED_STRING( ( STR_ID_289, "h", ( int )kTempletStat.m_fDefPhysic ) );
                }
                else
                {
                    if ( pCompareItem != NULL )
                    {
                        if ( (int)kTempletStat.m_fDefPhysic > (int)pCompareItem->GetStat().m_fDefPhysic )
                        {
                            wstrBuff = GET_REPLACED_STRING( ( STR_ID_290, "h", ( int )kTempletStat.m_fDefPhysic ) );

                        }
                        else if ( (int)pCompareItem->GetStat().m_fDefPhysic == (int)kTempletStat.m_fDefPhysic )
                        {
                            wstrBuff = GET_REPLACED_STRING( ( STR_ID_291, "h", ( int )kTempletStat.m_fDefPhysic ) );

                        }
                        else
                        {	
                            wstrBuff = GET_REPLACED_STRING( ( STR_ID_292, "h", ( int )kTempletStat.m_fDefPhysic ) );

                        }
                    }
                    else
                    {
						if(kTempletStat.m_fDefPhysic < 0.f)
						{
							wstrBuff = GET_REPLACED_STRING( ( STR_ID_292, "h", ( int )kTempletStat.m_fDefPhysic ) );
						}
						else
						{
							wstrBuff = GET_REPLACED_STRING( ( STR_ID_290, "h", ( int )kTempletStat.m_fDefPhysic ) );
						}
                        
                    }
                }

                slotItemDesc += wstrBuff;
				slotItemDesc += L"#CX";
            }

            if ( pItem != NULL && pItem->GetStat().m_fDefMagic != 0 )
            {
                if ( bCheckIsEqiupping == true )
                {
                    wstrBuff = GET_REPLACED_STRING( ( STR_ID_293, "h", ( int )pItem->GetStat().m_fDefMagic ) );
                }
                else
                {
                    if ( pCompareItem != NULL )
                    {
                        if ( (int)pItem->GetStat().m_fDefMagic > (int)pCompareItem->GetStat().m_fDefMagic )
                        {
                            wstrBuff = GET_REPLACED_STRING( ( STR_ID_294, "h", ( int )pItem->GetStat().m_fDefMagic ) );
                        }
                        else if ( (int)pCompareItem->GetStat().m_fDefMagic == (int)pItem->GetStat().m_fDefMagic )
                        {
                            wstrBuff = GET_REPLACED_STRING( ( STR_ID_295, "h", ( int )pItem->GetStat().m_fDefMagic ) );
                        }
                        else
                        {
                            wstrBuff = GET_REPLACED_STRING( ( STR_ID_296, "h", ( int )pItem->GetStat().m_fDefMagic ) );
                        }
                    }
                    else
                    {
						if(pItem->GetStat().m_fDefMagic < 0.f)
						{
							wstrBuff = GET_REPLACED_STRING( ( STR_ID_296, "h", ( int )pItem->GetStat().m_fDefMagic ) );
						}
						else
						{
							wstrBuff = GET_REPLACED_STRING( ( STR_ID_294, "h", ( int )pItem->GetStat().m_fDefMagic ) );
						}
                        
                    }
                }

                slotItemDesc += wstrBuff;
				slotItemDesc += L"#CX";
            }
            else if ( kTempletStat.m_fDefMagic != 0 )
            {
                if ( bCheckIsEqiupping == true )
                {
                    wstrBuff = GET_REPLACED_STRING( ( STR_ID_293, "h", ( int )kTempletStat.m_fDefMagic ) );
                }
                else
                {
                    if ( pCompareItem != NULL )
                    {
                        if ( (int)kTempletStat.m_fDefMagic > (int)pCompareItem->GetStat().m_fDefMagic )
                        {
                            wstrBuff = GET_REPLACED_STRING( ( STR_ID_294, "h", ( int )kTempletStat.m_fDefMagic ) );

                        }
                        else if ( (int)pCompareItem->GetStat().m_fDefMagic == (int)kTempletStat.m_fDefMagic )
                        {
                            wstrBuff = GET_REPLACED_STRING( ( STR_ID_295, "h", ( int )kTempletStat.m_fDefMagic ) );

                        }
                        else
                        {	
                            wstrBuff = GET_REPLACED_STRING( ( STR_ID_296, "h", ( int )kTempletStat.m_fDefMagic ) );
                        }
                    }
                    else
                    {
						if(kTempletStat.m_fDefMagic < 0.f)
						{
							wstrBuff = GET_REPLACED_STRING( ( STR_ID_296, "h", ( int )kTempletStat.m_fDefMagic ) );
						}
						else
						{
							wstrBuff = GET_REPLACED_STRING( ( STR_ID_294, "h", ( int )kTempletStat.m_fDefMagic ) );
						}                        
                    }
                }

                slotItemDesc += wstrBuff;
				slotItemDesc += L"#CX";
            }

            if ( pItem != NULL && pItem->GetStat().m_fBaseHP != 0 )
            {
                if ( bCheckIsEqiupping == true )
                {
                    wstrBuff = GET_REPLACED_STRING( ( STR_ID_297, "h", ( int )pItem->GetStat().m_fBaseHP ) );
                }
                else
                {
                    if ( pCompareItem != NULL )
                    {
                        if ( (int)pItem->GetStat().m_fBaseHP > (int)pCompareItem->GetStat().m_fBaseHP )
                        {
                            wstrBuff = GET_REPLACED_STRING( ( STR_ID_298, "h", ( int )pItem->GetStat().m_fBaseHP ) );

                        }
                        else if ( (int)pCompareItem->GetStat().m_fBaseHP == (int)pItem->GetStat().m_fBaseHP )
                        {
                            wstrBuff = GET_REPLACED_STRING( ( STR_ID_299, "h", ( int )pItem->GetStat().m_fBaseHP ) );
                        }
                        else
                        {	
                            wstrBuff = GET_REPLACED_STRING( ( STR_ID_300, "h", ( int )pItem->GetStat().m_fBaseHP ) );
                        }
                    }
                    else
                    {
						if(pItem->GetStat().m_fBaseHP < 0.f)
						{
							wstrBuff = GET_REPLACED_STRING( ( STR_ID_300, "h", ( int )pItem->GetStat().m_fBaseHP ) );
						}
						else
						{
							wstrBuff = GET_REPLACED_STRING( ( STR_ID_298, "h", ( int )pItem->GetStat().m_fBaseHP ) );
						}                     
                    }
                }

                slotItemDesc += wstrBuff;
            }
            else if ( kTempletStat.m_fBaseHP != 0 )
            {
                if ( bCheckIsEqiupping == true )
                {
                    wstrBuff = GET_REPLACED_STRING( ( STR_ID_297, "h", ( int )kTempletStat.m_fBaseHP ) );
                }
                else
                {
                    if ( pCompareItem != NULL )
                    {
                        if ( (int)kTempletStat.m_fBaseHP > (int)pCompareItem->GetStat().m_fBaseHP )
                        {
                            wstrBuff = GET_REPLACED_STRING( ( STR_ID_298, "h", ( int )kTempletStat.m_fBaseHP ) );

                        }
                        else if ( (int)pCompareItem->GetStat().m_fBaseHP == (int)kTempletStat.m_fBaseHP )
                        {
                            wstrBuff = GET_REPLACED_STRING( ( STR_ID_299, "h", ( int )kTempletStat.m_fBaseHP ) );

                        }
                        else
                        {	
                            wstrBuff = GET_REPLACED_STRING( ( STR_ID_300, "h", ( int )kTempletStat.m_fBaseHP ) );

                        }
                    }
                    else
                    {
						if(kTempletStat.m_fBaseHP < 0.f)
						{
							wstrBuff = GET_REPLACED_STRING( ( STR_ID_300, "h", ( int )kTempletStat.m_fBaseHP ) );
						}
						else
						{
							wstrBuff = GET_REPLACED_STRING( ( STR_ID_298, "h", ( int )kTempletStat.m_fBaseHP ) );
						}
                        
                    }
                }

                slotItemDesc += wstrBuff;
			}
#endif // SERV_NEW_ITEM_SYSTEM_2013_05
		}

		slotItemDesc += L"#CX";

#ifdef	X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
        std::vector<int> vecSocketOption;
        pkItemTemplet->GetSocketOptions( vecSocketOption );
#else   //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
        const std::vector<int>& vecSocketOption = pkItemTemplet->m_vecSocketOption;
#endif  //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING

		//{{ kimhc // 2011-07-05 // 옵션데이타 수치화 작업
		// 아이템의 소켓 레벨 정의
		// 아이템에 요구레벨이 있는 경우에는 소켓 레벨을 아이템의 요구레벨로 하고
		// 아이템에 요구레벨이 없는 경우에는 소켓 레벨을 유저(타겟유저)의 레벨 함

		if( NULL != m_pTargetUnit )
		{
	#ifdef SERV_NEW_ITEM_SYSTEM_2013_05 // 미감정 아이템 툴팁
				if( false == pItem->GetIsEvaluation() )
				{
					slotItemDesc += L"\n#C00FF00";
					slotItemDesc += GET_STRING( STR_ID_301 );
					slotItemDesc += L"#CFF0000 : ";
					slotItemDesc += GET_STRING( STR_ID_24609 );
					slotItemDesc += L"\n\n#C00FF00";
					slotItemDesc += GET_STRING( STR_ID_24610 );
					slotItemDesc += L"\n";
				}
				else
				{
					std::vector<int> vecNormalAndRandomSocketOption;// 일반 소켓과 랜덤 옵션을 같이 닮을 컨테이너
					// 랜덤 소켓 옵션 담기
					bool bHasRandomSocket = false;
					if( NULL != pItem )
					{
						if( false == pItem->GetItemData().m_vecRandomSocket.empty() )
						{
							vecNormalAndRandomSocketOption.assign( pItem->GetItemData().m_vecRandomSocket.begin(), 
								pItem->GetItemData().m_vecRandomSocket.end() );

							bHasRandomSocket = true;
						}
					}
					// 일반 소켓 옵션 담기
					std::vector<int> vecNormalSocketOption;
					pkItemTemplet->GetSocketOptions(vecNormalSocketOption);

					vecNormalAndRandomSocketOption.insert( vecNormalAndRandomSocketOption.end(), vecNormalSocketOption.begin(), vecNormalSocketOption.end() );

					// 소켓 레벨 얻기
					const int iSocketLevel = 0 < pkItemTemplet->GetUseLevel() ? pkItemTemplet->GetUseLevel() : m_pTargetUnit->GetUnitData().m_Level;

					// 소켓 설명 얻기
					if( true == bHasRandomSocket )
						slotItemDesc += GetSocketDesc( vecNormalAndRandomSocketOption,
							GET_STRING( STR_ID_25081 ), iSocketLevel, eUnitClass );
					else
						slotItemDesc += GetSocketDesc( vecNormalAndRandomSocketOption,
						GET_STRING( STR_ID_301 ), iSocketLevel, eUnitClass );
				}
	#else
			const int iSocketLevel = 0 < pkItemTemplet->GetUseLevel() ? pkItemTemplet->GetUseLevel() : m_pTargetUnit->GetUnitData().m_Level;
			slotItemDesc += GetSocketDesc( vecSocketOption, GET_STRING( STR_ID_301 ), iSocketLevel );
#endif //SERV_NEW_ITEM_SYSTEM_2013_05
		}
		//}} kimhc // 2011-07-05 // 옵션데이타 수치화 작업

		slotItemDesc += L"#CX";


		// 소켓효과 보여주지 않는다
// 		if( NULL != pItem )
// 		{
// 			slotItemDesc += GetSocketDesc( pItem->GetItemData().m_SocketOption, L"[소켓 효과]" );
// 			slotItemDesc += L"#CX";
// 		}

#ifdef FIX_TOOLTIP
		slotItemDesc += GetEnchantDesc( pItem, true );
#else
		slotItemDesc += GetEnchantDesc( pItem );
#endif FIX_TOOLTIP
		slotItemDesc += L"#CX";

        DWORD   dwItemID = pkItemTemplet->GetItemID();

		//스페셜 어빌리티 능력치 표시
		slotItemDesc += GetSpecialAbilityDesc( dwItemID );
		//{{ kimhc // 2011-07-05 // 옵션데이타 수치화 작업
		// 요구 레벨이 없고, 착용 할 수 있으면
		if ( 0 == pkItemTemplet->GetUseLevel() && 
			(CX2Item::IT_WEAPON == pkItemTemplet->GetItemType() || CX2Item::IT_DEFENCE == pkItemTemplet->GetItemType() ) )
		{
			slotItemDesc += L"\n";
			slotItemDesc += GET_STRING( STR_ID_13628 );
		}			
		//}} kimhc // 2011-07-05 // 옵션데이타 수치화 작업

		//{{ megagame : 박교현 : [2010-04-18] (마우스 오른쪽 버튼으로 사용)
        const wchar_t* pwszDescription = pkItemTemplet->GetDescription();
		if ( pwszDescription[0] != NULL )
		{
			//wsprintf( buff, L"\n\n%s", pkItemTemplet->m_Description.c_str() );
#ifdef FIX_LONG_ITEM_DESCRIPTION
			StringCchPrintf( buff, 512, L"\n\n%s", pwszDescription );
#else FIX_LONG_ITEM_DESCRIPTION
			StringCchPrintf( buff, 256, L"\n\n%s", pwszDescription );
#endif FIX_LONG_ITEM_DESCRIPTION
			slotItemDesc += buff;
			slotItemDesc += L"\n";
		}

		// 엿보기에서는 사용 가능 보여주지 않는다.
/*
		if( pkItemTemplet->GetCanUseInventory() == true )
		{
			slotItemDesc += L"\n";
			slotItemDesc += GET_STRING( STR_ID_5016 );
			slotItemDesc += L"\n";
		}
*/

		wstring tempCashItemDesc = GetSlotCashItemDesc( dwItemID );
		if ( tempCashItemDesc.empty() == false )
		{
			slotItemDesc += L"\n";
			slotItemDesc += tempCashItemDesc;
			slotItemDesc += L"\n";
		}
		//}} megagame : 박교현 : [2010-04-18]

// 		CX2ItemManager::ManufactureData* pManufactureData = g_pData->GetItemManager()->GetManufactureData( pkItemTemplet->m_ItemID );
// 		if ( pManufactureData != NULL )
// 		{
// 			slotItemDesc += GetManufactureMaterialDesc( pManufactureData, pkItemTemplet );
// 		}

		slotItemDesc += L"#CX\n";

		//{{ 2009.01.19 김태완 : 코드정리 elseif -> switch
		switch(pkItemTemplet->GetShopPriceType())
		{
		case CX2Item::SPT_GP:
			{
				// 무조건 정상적인 되팔기 가격을 보여 준다.
// 				if ( pkItemTemplet->m_PeriodType == CX2Item::PT_ENDURANCE && pItem != NULL )
// 				{
// 					//wsprintf( buff, L"되팔기 가격 : %s ED\n", g_pMain->GetEDString( pItem->GetEDToSell() ).c_str()  );
//					StringCchPrintf( buff, 256, L"되팔기 가격 : %s ED\n", g_pMain->GetEDString( pItem->GetEDToSell() ).c_str()  );
// 				}
// 				else
// 				{
//				}
				slotItemDesc += GET_REPLACED_STRING( ( STR_ID_303, "L", g_pMain->GetEDString( (int )( pkItemTemplet->GetPrice() * 0.2f ) ) ) );
			} break;
		case CX2Item::SPT_CASH:
			{
				slotItemDesc += GET_REPLACED_STRING( ( STR_ID_304, "i", pkItemTemplet->GetPrice() ) );
			} break;
		case CX2Item::SPT_NONE:
			{
				slotItemDesc += GET_STRING( STR_ID_305 );
			} break;
		default:
			{
				ASSERT( !"Unexpected Price Type" );
			} break;
		}//}} elseif -> switch


		if( pkItemTemplet->GetVested() == true ||
			( NULL != pItem &&
			pItem->GetItemData().m_Period > 0 ) )
		{
			slotItemDesc += GET_STRING( STR_ID_306 );
			slotItemDesc += L"#CX";
		}

		// 남은 기간 정보 보여주지 않는다
//		slotItemDesc += GetSlotCashItemPeriodDesc( pkItemTemplet->m_ItemID );
//		slotItemDesc += GetExpirationDateDesc( pItem );

		//{{ kimhc // 2011-07-05 // 옵션데이타 수치화 작업
		if( NULL != m_pTargetUnit )
			slotItemDesc += GetSetItemDesc( pItem, dwItemID, m_pTargetUnit->GetUnitData().m_Level );

		//}} kimhc // 2011-07-05 // 옵션데이타 수치화 작업

		if ( g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_OPERATOR )
		{
			//wsprintf( buff, L"\nItemID = %d\n", pkItemTemplet->m_ItemID );
			StringCchPrintf( buff, 256, L"\nItemID = %d\n", dwItemID );
			slotItemDesc += buff;
		}

		if( true == bItemDescExtra )
		{
			slotItemDesc += GetSlotItemDescExtra_RBtn( itemTID, pItem );
		}
	}

	return slotItemDesc;
}


 

#endif
