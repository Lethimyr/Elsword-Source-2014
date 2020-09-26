#include "StdAfx.h"

#ifdef NEW_SKILL_TREE_UI
#ifdef GUILD_SKILL
#ifdef GUILD_BOARD
#include ".\x2uiguildskilltreeInfo.h"

const int MAGIC_SKILL_DESC_WIDTH = 260;


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/// CX2SkillTreeSlot : 스킬 트리에 나오는 슬롯
//////////////////////////////////////////////////////////////////////////
CX2GuildSkillTreeInfoSlotData::CX2GuildSkillTreeInfoSlotData()
{
	m_eSlotDataType = SDT_SKILL_TREE;
	m_eSkillID = CX2SkillTree::SI_NONE;
}
/*virtual*/ CX2GuildSkillTreeInfoSlotData::~CX2GuildSkillTreeInfoSlotData()
{


}
/*virtual*/ void CX2GuildSkillTreeInfoSlotData::UpdateUI( CKTDGUISlot* pSlot )
{    
	// 1. 지금 익힐 수 있는 스킬 여부 : 버튼의 활성화/비활성화. bool 값에 따라서
	// 2. 1레벨이라도 익힌 스킬 여부 : 스킬 슬롯의 드래그 활성화 / 비활성화(패시브면 드래그 안 되게), 텍스쳐 변경. 스트링 설정 (레벨/최대레벨)

	CX2SkillTree* pSkillTree = g_pData->GetSkillTree();

	CX2UIGuildBoard* refUIGuildBoard = g_pData->GetUIManager()->GetUIGuildBoard(); // 슬롯 B 관련

	int iSkillLevel = refUIGuildBoard->GetGuildSkillLevel( m_eSkillID );

#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
	const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( m_eSkillID );
#else // UPGRADE_SKILL_SYSTEM_2013
	const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( m_eSkillID, (iSkillLevel != 0 ? iSkillLevel : 1) );
#endif // UPGRADE_SKILL_SYSTEM_2013
	
	if(pSkillTree == NULL || pSkillTemplet == NULL)
		return;

	const map<CX2SkillTree::SKILL_ID, CX2UIGuildSkillTreeInfo::SkillSlotUI>& refmapSkillSlotUISet = g_pData->GetUIManager()->GetUIGuildBoard()->GetUIGuildSkillTreeInfo()->GetMapSkillSlotUISet();

	map<CX2SkillTree::SKILL_ID, CX2UIGuildSkillTreeInfo::SkillSlotUI>::const_iterator it = refmapSkillSlotUISet.find(m_eSkillID);

	if(it == refmapSkillSlotUISet.end())
	{
		return;
	}
	//const CX2UIGuildSkillTreeInfo::SkillSlotUI& refSkillSlotUI = it->second; // oasis
	const CX2UIGuildSkillTreeInfo::SkillSlotUI& refSkillSlotUI = it->second; 

	bool bRemoveMode = g_pData->GetUIManager()->GetUIGuildBoard()->GetUIGuildSkillTreeInfo()->GetRemoveMode();


	// 1. 슬롯 업데이트
	pSlot->SetItemTex( pSkillTemplet->m_wstrIconName.c_str(), pSkillTemplet->m_wstrIconPieceName.c_str() );
	pSlot->SetDragable(false);

	if( iSkillLevel >= 1 )
	{
		if(pSkillTemplet->m_bShowSkillLevel)
			ShowSlotPicture( true, pSlot, STSAPT_LEVEL, iSkillLevel );
	}
	else
	{
		ShowSlotPicture( false, pSlot, STSAPT_LEVEL );
	}
/*
	// 1-2. 슬롯 상태 업데이트
	if( refUserSkillTree.CanILearnThisSkill(m_eSkillID, true) && false == refUserSkillTree.DoIHaveThisSkill(m_eSkillID) )
	{
		// 배울 수 있고, 안 가지고 있는 스킬
		ShowSlotPicture( false, pSlot, STSAPT_DISABLE );
		ShowSlotPicture( true, pSlot, STSAPT_CANLEARN );
	}
	else if(true == refUserSkillTree.IsMaxSkillLevel(m_eSkillID) || refUserSkillTree.CanILearnThisSkill(m_eSkillID, false ) )
	{
		// 이미 익히고 있는 스킬
		ShowSlotPicture( false, pSlot, STSAPT_DISABLE );
		ShowSlotPicture( false, pSlot, STSAPT_CANLEARN );
	}
	else
	{				
		// 없는 스킬
		ShowSlotPicture( true, pSlot, STSAPT_DISABLE );
		ShowSlotPicture( false, pSlot, STSAPT_CANLEARN );
	}
*/
	// 2. 버튼 업데이트 : refUserSkillTree의 정보를 바탕으로 한다.
	
	refSkillSlotUI.m_pButtonPlus->SetShow(true);
	refSkillSlotUI.m_pButtonMinus->SetShow(false);
	refSkillSlotUI.m_pButtonMaster->SetShow(false);

	if(true == refUIGuildBoard->IsMaxGuildSkillLevel(m_eSkillID))
	{
		refSkillSlotUI.m_pButtonMaster->SetShow(true);
		refSkillSlotUI.m_pButtonPlus->SetShow(false);
	}
	refSkillSlotUI.m_pButtonPlus->SetEnable(false);		
		
	// 3. 스킬 레벨 업데이트
	CKTDGUIControl::UIStringData* pString = refSkillSlotUI.m_pStaticLevelString->GetString(0);
	if(pString != NULL)
	{
		int level = refUIGuildBoard->GetGuildSkillLevel( m_eSkillID );
		int maxlevel = pSkillTree->GetMaxGuildSkillLevel( 0, m_eSkillID );

		WCHAR buf[32] = {0,};
		int csp = 0;
		refUIGuildBoard->GetGuildSkillLevelAndCSP(m_eSkillID, level, csp);

		if(refUIGuildBoard->IsMaxGuildSkillLevel(m_eSkillID) == true)
		{
			if(csp > 0)
			{
				//wsprintf( buf, L"#CEC008C%d#CX#C94f412/%d#CX", level, maxlevel );
				StringCchPrintf( buf, 32, L"#CEC008C%d#CX#C94f412/%d#CX", level, maxlevel );

			}
			else
			{
				//wsprintf( buf, L"#C94f412%d/%d#CX", level, maxlevel );
				StringCchPrintf( buf, 32, L"#C94f412%d/%d#CX", level, maxlevel );
			}

		}
		else
		{
			if(csp > 0)
			{
				//wsprintf( buf, L"#CEC008C%d#CX/%d", level, maxlevel );
				StringCchPrintf( buf, 32, L"#CEC008C%d#CX/%d", level, maxlevel );
			}
			else
			{
				//wsprintf( buf, L"%d/%d", level, maxlevel );
				StringCchPrintf( buf, 32, L"%d/%d", level, maxlevel );
			}				
		}

		pString->msg = buf;

	}

	// 4. 화살표 업데이트
	if(true == refUIGuildBoard->IsMaxGuildSkillLevel(m_eSkillID))
	{
		g_pData->GetUIManager()->GetUIGuildBoard()->GetUIGuildSkillTreeInfo()->SetArrowColor(m_eSkillID, D3DXCOLOR(1,1,1,1));
	}
	else
	{
		g_pData->GetUIManager()->GetUIGuildBoard()->GetUIGuildSkillTreeInfo()->SetArrowColor(m_eSkillID, D3DXCOLOR(0.5f,0.5f,0.5f,1));
	}


	// 5. 봉인표시 자물쇠
	refSkillSlotUI.m_pStaticLock->SetShowEnable( false, false );

	return;
}


void CX2GuildSkillTreeInfoSlotData::ShowSlotPicture(bool bShow, CKTDGUISlot* pSlot, SKILL_TREE_SLOT_ADD_PICTURE_TYPE eType, int val /*= -1*/ )
{
	if(pSlot == NULL)
		return;

	CKTDGUIControl::CPictureData* pPicture = pSlot->GetPicture((int)eType);
	if(pPicture == NULL )
	{
		if(bShow)
		{
			pPicture = new CKTDGUIControl::CPictureData();
			CKTDGUIControl::UIPointData* pPoint = new CKTDGUIControl::UIPointData(pSlot->GetNowPoint());
			pPicture->SetPoint( pPoint );

			switch(eType)
			{
			case STSAPT_DISABLE:
				{
					pPicture->SetTex( L"HQ_ImpossibleEqip.tga" );			
					pPicture->pPoint->color = D3DXCOLOR( 1,1,1,0.6f );
				} break;
			case STSAPT_SEALED:
				{
					pPicture->SetTex( L"DLG_UI_Common_Texture02.TGA", L"lock" );
					pPicture->pPoint->color = D3DXCOLOR( 1,1,1,1 );
				} break;
			case STSAPT_LEVEL:
				{
					// 레벨 텍스쳐. 리소스 나오면...
					wstring wstrLevelTextureName = g_pData->GetSkillTree()->GetSkillLevelTextureName( val );
					wstring wstrLevelTextureKey = g_pData->GetSkillTree()->GetSkillLevelTextureKey( val );
					if( false == pPicture->SetTex( wstrLevelTextureName.c_str(), wstrLevelTextureKey.c_str() ) )
					{
						pPicture->SetTex( L"NoAlphaImage.dds" );
					}
					pPicture->pPoint->color = D3DXCOLOR( 1,1,1,1 );

					/// 레벨 텍스처 위치 조정
					D3DXVECTOR2 vecPos					= pPicture->GetPos();
					pPicture->SetPos( D3DXVECTOR2( vecPos.x, vecPos.y + 33.f ) );

					D3DXVECTOR2 vecPicturePos			= pPicture->pPoint->leftTopPoint;
					pPicture->pPoint->leftBottomPoint	= D3DXVECTOR2( vecPicturePos.x, vecPicturePos.y + 14.f );
					pPicture->pPoint->rightTopPoint		= D3DXVECTOR2( vecPicturePos.x + 25.f, vecPicturePos.y );
					pPicture->pPoint->rightBottomPoint	= D3DXVECTOR2( vecPicturePos.x + 25.f, vecPicturePos.y + 14.f );
				} break;
			case STSAPT_EQUIPPED:
				{
					pPicture->SetTex( L"DLG_Common_New_Texture26.tga", L"Skill_Mount" );
					pPicture->SetFlicker( 2.0f, 1.4f, 0.2f );

				} break;
			case STSAPT_CANLEARN:
				{
					pPicture->SetTex( L"DLG_Common_New_Texture26.tga", L"acquire_Skill" );
					pPicture->SetFlicker( 2.0f, 1.4f, 0.2f );

				} break;
			default:
				{

				} break;
			}

			pSlot->AddPicture( (int)eType, pPicture );
			pPicture->SetShow(bShow);
		}		
	}
	else
	{
		if(bShow)
		{
			switch(eType)
			{
			case STSAPT_LEVEL:
				{
					wstring wstrLevelTextureName = g_pData->GetSkillTree()->GetSkillLevelTextureName( val );
					wstring wstrLevelTextureKey = g_pData->GetSkillTree()->GetSkillLevelTextureKey( val );
					if( false == pPicture->SetTex( wstrLevelTextureName.c_str(), wstrLevelTextureKey.c_str() ) )
					{
						pPicture->SetTex( L"NoAlphaImage.dds" );
					}
					pPicture->pPoint->color = D3DXCOLOR( 1,1,1,1 );

				} break;
			default:
				break;
			}

		}

		pPicture->SetShow(bShow);
	}

}

//////////////////////////////////////////////////////////////////////////


CX2UIGuildSkillTreeInfo::CX2UIGuildSkillTreeInfo( CKTDXStage* pNowStage ) :
m_bShow(false),
m_bSkillRemoveMode(false),
m_bClassInfoVaild(false),
//m_MovedPosition(0,0),
//m_DLGPosition(0,0),
m_DLGSize(0,0),
m_pDLGUISkillTree(NULL),
m_pDLGSkillToolTip(NULL),
m_pDLGSkillUnsealInform(NULL),
m_pDLGSkillUnsealed( NULL ),
m_SumDelta(0),
m_iPrevSP(-1),
m_pUserSkillTree(NULL),
m_pTalkBoxManager(NULL),
m_bUsingCSP( false )		// 이것도 안쓰이는거 같은데...
{
	m_pTalkBoxManager = CX2TalkBoxManager::CreateTalkBoxManger();
	g_pKTDXApp->GetDGManager()->AddObjectChain( m_pTalkBoxManager );
	m_pTalkBoxManager->SetOverUI( true );
}


CX2UIGuildSkillTreeInfo::~CX2UIGuildSkillTreeInfo(void)
{
	SAFE_DELETE_DIALOG( m_pDLGUISkillTree );	
	SAFE_DELETE_DIALOG( m_pDLGSkillToolTip );
	SAFE_DELETE_DIALOG( m_pDLGSkillUnsealInform );

	SAFE_DELETE_DIALOG( m_pDLGSkillUnsealed );

	SAFE_DELETE_KTDGOBJECT( m_pTalkBoxManager );

	m_mapSkillSlotUISet.clear();
	m_mapArrowUISet.clear();

}

void CX2UIGuildSkillTreeInfo::SetLayer( X2_DIALOG_LAYER layer )
{
	if(m_pDLGUISkillTree != NULL) 
	{
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer(m_pDLGUISkillTree, layer);
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDLGUISkillTree, false );
	}
}

void CX2UIGuildSkillTreeInfo::SetPosition( D3DXVECTOR2 vec )
{
	// 슬롯 다이얼로그
	m_pDLGUISkillTree->SetPos(vec);

	//m_MovedPosition = vec;
	
}

HRESULT CX2UIGuildSkillTreeInfo::OnFrameMove( double fTime, float fElapsedTime )
{

	return S_OK;
}

bool CX2UIGuildSkillTreeInfo::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// 앞에 모달 다이얼로그가 있으면 메시지를 처리하지 않는다
	if( true == g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckFrontModalDlg( m_pDLGUISkillTree ) )
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
			if ( false == m_pDLGUISkillTree->GetIsMouseOver())
			{
				return false;
			}
			//bFlag = true;
		}
		break;
	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
		{
			if ( false == m_pDLGUISkillTree->GetIsMouseOver())
			{
				return false;
			}
			//g_pData->GetUIManager()->SetUILayerIndex(CX2UIManager::UI_MENU_SKILL, true);
			//bFlag = true;			
		}
		break;
	default:
		break;
	}

	switch( uMsg )
	{
	case WM_MOUSEWHEEL:
		{ 
			if( m_pDLGUISkillTree->GetIsMouseOver() ) 
				OnMouseWheel(hWnd, uMsg, wParam,lParam);
		}break;
	}

	return bFlag;
}

bool CX2UIGuildSkillTreeInfo::OnMouseWheel(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CKTDGUIContolList* pControlList = (CKTDGUIContolList*) m_pDLGUISkillTree->GetControl( L"SkillTreeControlList" );
	if( pControlList == NULL )
		return false;
	CKTDGUIScrollBar* pScroolBarY = pControlList->GetYScroolBar();
	if(pScroolBarY == NULL)
		return false;

	short zDelta = (short)HIWORD(wParam);
	m_SumDelta += zDelta;

#if 0
	if(abs(m_SumDelta) < WHEEL_DELTA)
		return true;

	pScroolBarY->Scroll(-m_SumDelta / WHEEL_DELTA);
	m_SumDelta %= WHEEL_DELTA;
#else
	while (abs(m_SumDelta) >= WHEEL_DELTA)
	{
		if(m_SumDelta>0)
		{
			pScroolBarY->Scroll(-1);
			m_SumDelta -= WHEEL_DELTA;
		}
		else
		{	
			pScroolBarY->Scroll(1);
			m_SumDelta += WHEEL_DELTA;
		}	

	}
#endif
	return true;
}

bool CX2UIGuildSkillTreeInfo::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch(wParam)
	{
	case GSTICM_UNSEAL_SKILL_CANCLE:
		{
			return true;
		}
	case GSTICM_UNSEAL_SKILL_BUY:
		{
			//** 스킬 구매 버튼을 눌렀을 때 여기에 작업해 주세요.
			return true;
		} break;
	case GSTICM_CSP_GUIDE_MOUSEIN:
		{	
			CKTDGUIButton* pControl = (CKTDGUIButton*)lParam;
			D3DXVECTOR2 pos = pControl->GetPos() + pControl->GetDialog()->GetPos();
			pos.x += pControl->GetWidth() / 2.f;
			pos.y += pControl->GetHeight();
						
			if(g_pData->GetMyUser() != NULL &&
				g_pData->GetMyUser()->GetSelectUnit() != NULL )
			{
				int iSPoint = 0;
				int iCSPoint = 0;

				// oasis907 : 김상윤 [2009.12.3] // 길드 기간제 포인트 출력
				int iMaxCSPoint = g_pData->GetUIManager()->GetUIGuildBoard()->m_iMaxGuildCSPoint; 
				WCHAR wszText[32] = L"";
				iMaxCSPoint = 5; // 임시
				StringCchPrintfW( wszText, ARRAY_SIZE(wszText), L"(%dGSP)\n", iMaxCSPoint );
				
				wstring wstr = L"";
				wstr += GET_STRING(STR_ID_4815);
				wstr += wszText;
				// oasis907 : 김상윤 [2009.12.3] //
				wstr += GetExpirationDateDesc( g_pData->GetUIManager()->GetUIGuildBoard()->m_wstrGuildCSPointEndDate, g_pData->GetServerCurrentTime() );

				ShowUIDesc(true, wstr, pos, CX2TalkBoxManagerImp::TBT_FROM_UP_LEFT, D3DXCOLOR(1,1,1,1), D3DXCOLOR(0.97f, 0.23f, 0.06f, 1) );			
			}
			
			return true;
		} break;
	case GSTICM_CSP_GUIDE_MOUSEOUT:
		{
			ShowUIDesc(false);
			return true;
		} break;
	case GSTICM_SKILLTREESLOTB_GUIDE_MOUSEIN: // oasis907
		{
			return true;
		} break;
	case GSTICM_SKILLTREESLOTB_GUIDE_MOUSEOUT:
		{
			return true;
		} break;
	case GSTICM_SKILL_RESET_MODE_CHANGE_MOUSEOVER:
		{

			return true;
		} break;
	case GSTICM_SKILL_RESET_MODE_CHANGE_MOUSEOUT:
		{
			return true;
		} break;
	case GSTICM_BUTTON_SKILL_LEVEL_PLUS_MOUSEOVER:
		{
			return true;
		} break;
	case GSTICM_BUTTON_SKILL_LEVEL_PLUS_MOUSEOUT:
		{
			return true;
		} break;

	case GSTICM_BUTTON_SKILL_LEVEL_MINUS_MOUSEOVER:
		{
			return true;
		} break;
	case GSTICM_BUTTON_SKILL_LEVEL_MINUS_MOUSEOUT:
		{
			return true;
		} break;

	case GSTICM_BUTTON_SKILL_MASTER_MOUSEOVER:
		{
			return true;
		} break;
	case GSTICM_BUTTON_SKILL_MASTER_MOUSEOUT:
		{
			return true;
		} break;

	case GSTICM_NAVI_LEFT_CLICKED:
	case GSTICM_NAVI_DRAGGING:
		{
			return true;		
		} break;
	case GSTICM_CONTROLLIST_SCROLL:
		{
			UpdateBlind();
		
			return true;
		} break;
	case GSTICM_SKILLTREESLOT_MOUSEIN:
		{
			CKTDGUISlot* pSlot = (CKTDGUISlot*) lParam;
			CX2GuildSkillTreeInfoSlotData* pSlotData = (CX2GuildSkillTreeInfoSlotData*) pSlot->GetSlotData();
			if(pSlotData == NULL || pSlotData->m_eSkillID == CX2SkillTree::SI_NONE )
			{
				ShowSkillDesc(false);
				return true;
			}

			D3DXVECTOR2 pos = pSlot->GetPos() + pSlot->GetOffsetPos() + pSlot->GetDialog()->GetPos();
			D3DXVECTOR2 size = pSlot->GetSize();

			ShowSkillDesc(true, pSlotData->m_eSkillID, pos, size, false );		
			return true;

		} break;
	case GSTICM_EQUIPSLOT_MOUSEIN:
		{
			return true;
		} break;
	case GSTICM_SKILLTREESLOT_MOUSEOUT:
	case GSTICM_EQUIPSLOT_MOUSEOUT:
		{			
			ShowUIDesc( false );
			ShowSkillDesc(false);
			return true;
		} break;
	case GSTICM_SKILLTREESLOT_RMOUSEUP: // oasis907
		{
			return true;		
		} break;

	case GSTICM_EXIT:
		{
			SetShow(false);
			return true;
		} break;

	case GSTICM_EQUIPSLOT_DROPPED:
		{	
			return true;
		} break;
	case GSTICM_BUTTON_SKILL_LEVEL_PLUS:
		{
			return true;
		} break;
	case GSTICM_BUTTON_SKILL_LEVEL_MINUS:
		{			
			return true;
		} break;
	case GSTICM_EQUIPSLOT_RMOUSEUP:
		{
			return true;
		} break;

	case GSTICM_SKILL_RESET_MODE_CHANGE:
		{ 
			return true;
		} break;
		


	case GSTICM_GUIDE_BOOK:
		{
			return true;
		} break;

	default:
		break;
	}
	return false;
}

void CX2UIGuildSkillTreeInfo::SetShow(bool val)
{
	m_bShow = val;

	if(val)	// 켜질 때 처리해야 할 부분
	{

		//{{ oasis907 : 김상윤 [2009.12.2] GuildInfo, GuildMemberInfo 다이얼로그와 GuildSkill 다이얼로그가 공유 되지 않는 상태의 처리 
		// (추후의 두 다이얼로그의 사이즈가 같아지면 공유되는것이 바람직)

		CKTDGUIRadioButton*	pRadio_GuildInfo = static_cast<CKTDGUIRadioButton*>( m_pDLGUISkillTree->GetControl( L"Radio_Guild_Info" ) );
		CKTDGUIRadioButton*	pRadio_GuildMemberInfo = static_cast<CKTDGUIRadioButton*>( m_pDLGUISkillTree->GetControl( L"Radio_Guild_Member_Info" ) ); 
		CKTDGUIRadioButton*	pRadio_GuildSkill = static_cast<CKTDGUIRadioButton*>( m_pDLGUISkillTree->GetControl( L"RadioButtonGuild_Skill" ) ); 

		if( pRadio_GuildInfo == NULL || 
			pRadio_GuildMemberInfo == NULL  ||
			pRadio_GuildSkill == NULL )
		{
			ASSERT( !"Check Radio buttons at DLG_UI_Guild_Skill_Tree.lua" );
			return;
		}
		pRadio_GuildInfo->SetChecked( false );
		pRadio_GuildMemberInfo->SetChecked( false );
		pRadio_GuildSkill->SetChecked( true );

		//}} oasis907 : 김상윤 [2009.12.2] //

		pRadio_GuildInfo->SetEnable( false );
		pRadio_GuildMemberInfo->SetEnable( false );
		pRadio_GuildSkill->SetEnable( false );

		m_bSkillRemoveMode = false;
		UpdateUI(true, true, true);

		//g_pData->GetUIManager()->UIOpened(CX2UIManager::UI_MENU_SKILL);
	}
	else	// 꺼질 때 처리해야 할 부분
	{
		ShowSkillDesc(false);
		ShowUIDesc(false);
		SAFE_DELETE_DIALOG(m_pDLGSkillUnsealInform);

		//g_pData->GetUIManager()->UIClosed(CX2UIManager::UI_MENU_SKILL);
	}

	m_pDLGUISkillTree->SetShowEnable(m_bShow, m_bShow);
}

// 캐릭터가 선택됐을 때 / 전직했을 때만 해 준다.
void CX2UIGuildSkillTreeInfo::InitSkillTreeUI()
{
	SAFE_DELETE_DIALOG(m_pDLGUISkillTree);
	
	m_mapSkillSlotUISet.clear();
	m_mapArrowUISet.clear();
	m_pDLGUISkillTree = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Guild_Skill_Tree_Info.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGUISkillTree );
	m_pDLGUISkillTree->SetDisableUnderWindow(true);
	m_pDLGUISkillTree->SetShow(false);



// 	m_pDLGUISkillTree->CreateGuideDesc();
// 	CKTDGUIStatic* pGuideStatic = (CKTDGUIStatic*) m_pDLGUISkillTree->GetGuideControl();
// 	pGuideStatic->GetString(0)->fontIndex = XUF_DODUM_11_NORMAL;

	D3DXVECTOR3 tmp;
// 	tmp = m_pDLGUISkillTree->GetDummyPos(0);
// 	m_DLGPosition.x = tmp.x;
// 	m_DLGPosition.y = tmp.y;
	tmp = m_pDLGUISkillTree->GetDummyPos(1);
 	m_DLGSize.x = tmp.x;
 	m_DLGSize.y = tmp.y;


	CreateSkillTree( CX2Unit::UC_NONE ); // oasis

}

void CX2UIGuildSkillTreeInfo::CreateSkillTree( CX2Unit::UNIT_CLASS eUnitClass )
{

	if(m_pDLGUISkillTree == NULL)
		return;
	CKTDGUIContolList* pControlList = (CKTDGUIContolList*) m_pDLGUISkillTree->GetControl( L"SkillTreeControlList" );
	if( pControlList == NULL )
		return;

	//1. 해당 클래스의 스킬트리 템플릿을 받아서
	//2. iteration을 죽 해나가면서 각각의 스킬마다 하나의 슬롯과 버튼을 만들어서
	// "Slot_SKILLID" "Button_SKILLID" 라는 이름을 붙여주고 슬롯 ID에는 스킬 ID를 붙여준다.
	// 슬롯에 필요한 정보들을 기록한다. 
	//3. 해당하는 위치의 ControlList에 집어넣어 준다
	//4. 선행스킬이 있으면 y축을 위쪽으로 올라가면서 해당 선행스킬 위치까지의 화살표를 만든다.

	// Unit Class를 이용해서 해당 Map을 어떻게든 받아온다. 왔다고 치자.
	const CX2SkillTree::SkillTreeTempletMap& refSkillTreeTempletMap = g_pData->GetSkillTree()->GetGuildSkillTreeTempletMap( (int)eUnitClass );


	// 임시 예외
	if(refSkillTreeTempletMap.size() == 0)
	{
		return;
	}
	//////

	for( CX2SkillTree::SkillTreeTempletMap::const_iterator mapit = refSkillTreeTempletMap.begin(); mapit != refSkillTreeTempletMap.end(); ++mapit )
	{
		CX2SkillTree::SKILL_ID eSkillID = mapit->first;
		const CX2SkillTree::SkillTreeTemplet& refSkillTreeTemplet = mapit->second;

	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( eSkillID );
	#else // UPGRADE_SKILL_SYSTEM_2013
		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( eSkillID, 1 );
	#endif // UPGRADE_SKILL_SYSTEM_2013
		
		if(pSkillTemplet == NULL)
		{
#ifndef _SERVICE_
			WCHAR buf[256] = {0,};
			//wsprintf(buf, L"Skill ID : %d\nExist in SkillTreeTemplet but NULL SkillTemplet", (int)eSkillID);
			StringCchPrintf(buf, 256, L"Skill ID : %d\nExist in SkillTreeTemplet but NULL SkillTemplet", (int)eSkillID);
			MessageBox( g_pKTDXApp->GetHWND(), buf, L"Error", MB_OK );
#endif
			continue;
		}

		int row = refSkillTreeTemplet.m_iTier;
		int col = refSkillTreeTemplet.m_iIndexInTier;

		SkillSlotUI UISet;
		CreateSlotUIPreset( UISet, (int)eSkillID );

		// 스킬ID 넣어 주고
		UISet.m_pSlot->SetID((int)eSkillID);
		UISet.m_pButtonPlus->AddDummyInt((int)eSkillID);
		UISet.m_pButtonMinus->AddDummyInt((int)eSkillID);

		// 집어넣고
		pControlList->InsertItem( UISet.m_pStaticBackGround, row, col );
		pControlList->InsertItem( UISet.m_pSlot, row, col );
		pControlList->InsertItem( UISet.m_pButtonPlus, row, col );
		pControlList->InsertItem( UISet.m_pButtonMinus, row, col );
		pControlList->InsertItem( UISet.m_pButtonMaster, row, col );
		pControlList->InsertItem( UISet.m_pStaticLevelString, row, col );
		pControlList->InsertItem( UISet.m_pStaticActiveMark, row, col );
		pControlList->InsertItem( UISet.m_pStaticLock, row, col );

		// 관리용 Map에 집어넣고
		m_mapSkillSlotUISet.insert(std::make_pair(eSkillID, UISet));

		// 선행스킬이 존재하면 
#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
		CX2SkillTree::SKILL_ID PreccedSkillID = static_cast<CX2SkillTree::SKILL_ID>( refSkillTreeTemplet.m_iPrecedingSkill );
		{
			if ( CX2SkillTree::SI_NONE != PreccedSkillID )
			{
#else // UPGRADE_SKILL_SYSTEM_2013
		if(refSkillTreeTemplet.m_vecPrecedingSkill.empty() == false)
		{
			// 화살표를 만들어낸다
			for(vector<int>::const_iterator itPre = refSkillTreeTemplet.m_vecPrecedingSkill.begin(); itPre != refSkillTreeTemplet.m_vecPrecedingSkill.end(); ++itPre)
			{
				CX2SkillTree::SKILL_ID PreccedSkillID = (CX2SkillTree::SKILL_ID)*itPre;
#endif // UPGRADE_SKILL_SYSTEM_2013

				const CX2SkillTree::SkillTreeTemplet* pPreccedSkillTreeTemplet = g_pData->GetSkillTree()->GetGuildSkillTreeTemplet( (int)eUnitClass, PreccedSkillID );
				if ( pPreccedSkillTreeTemplet == NULL )
				{
#ifndef _SERVICE_
					WCHAR buf[256] = {0,};
					//wsprintf(buf, L"Skill ID : %d Precceding Skill ID : %d\nPrecceding Skill Exist in SkillTreeTemplet but NULL SkillTemplet", (int)eSkillID, (int)PreccedSkillID);
					StringCchPrintf(buf, 256, L"Skill ID : %d Precceding Skill ID : %d\nPrecceding Skill Exist in SkillTreeTemplet but NULL SkillTemplet", (int)eSkillID, (int)PreccedSkillID);
					MessageBox( g_pKTDXApp->GetHWND(), buf, L"Error", MB_OK );
#endif
					continue;						
				}

				ArrowUI ArrowUISet;
				if(pPreccedSkillTreeTemplet->m_iIndexInTier == col)
				{
					int LineLength = row - pPreccedSkillTreeTemplet->m_iTier - 1;
					CreateVerticalArrowUIPreset( ArrowUISet, LineLength, (int)PreccedSkillID);

					// 꼬리
					pControlList->InsertItem( ArrowUISet.m_pStaticArrowEnd, pPreccedSkillTreeTemplet->m_iTier, col );
					
					// 몸
					int LineBodyBegin = pPreccedSkillTreeTemplet->m_iTier + 1;
					for( int i = 0; i < LineLength; ++i )
					{	
						CKTDGUIStatic* pStaticArrowLine = ArrowUISet.m_vpStaticArrowLine[i];
						pControlList->InsertItem( pStaticArrowLine, i+LineBodyBegin, col );
					}
					// 머리
					pControlList->InsertItem( ArrowUISet.m_pStaticArrowHead, row, col );

				}
				else if(pPreccedSkillTreeTemplet->m_iTier == row)
				{
					// 가로 화살표
					if( col > pPreccedSkillTreeTemplet->m_iIndexInTier )
					{
						// 선행 스킬이 왼쪽에 있다 : 오른쪽으로 향하는 화살표
						int LineLength = col - pPreccedSkillTreeTemplet->m_iIndexInTier - 1;
						CreateHorizonalArrowUIPreset( ArrowUISet, LineLength, (int)PreccedSkillID, true );

						// 몸을 달자
						int LineBodyBegin = pPreccedSkillTreeTemplet->m_iIndexInTier + 1;
						for( int i = 0; i < LineLength; ++i )
						{	
							CKTDGUIStatic* pStaticArrowLine = ArrowUISet.m_vpStaticArrowLine[i];
							pControlList->InsertItem( pStaticArrowLine, row, i+LineBodyBegin );
						}

					}
					else
					{
						// 선행 스킬이 오른쪽에 있다 : 왼쪽으로 향하는 화살표
						int LineLength = pPreccedSkillTreeTemplet->m_iIndexInTier - col - 1;
						CreateHorizonalArrowUIPreset( ArrowUISet, LineLength, (int)PreccedSkillID, false );

						// 몸을 달자
						int LineBodyBegin = col + 1;
						for( int i = 0; i < LineLength; ++i )
						{	
							CKTDGUIStatic* pStaticArrowLine = ArrowUISet.m_vpStaticArrowLine[i];
							pControlList->InsertItem( pStaticArrowLine, row, i+LineBodyBegin );
						}
					}
					
					// 꼬리
					pControlList->InsertItem( ArrowUISet.m_pStaticArrowEnd, row, pPreccedSkillTreeTemplet->m_iIndexInTier);
					// 머리
					pControlList->InsertItem( ArrowUISet.m_pStaticArrowHead, row, col );
					 
				}
				// 관리용 맵에 집어넣~자~
				m_mapArrowUISet.insert(std::make_pair(PreccedSkillID, ArrowUISet));
			}
		}
		//}} UI 집어넣는 코드는 제작은 여기까지~
		// 여기서는 집어넣기만 하고, 각 Control의 On/Off 같은 세부적인 사항은 이후 UpdateUI 해 줄 때 다룬다.

		// 이제 Data를 만들어 넣을 차례				
		CX2GuildSkillTreeInfoSlotData* pSkillTreeSlotData = new CX2GuildSkillTreeInfoSlotData;
		pSkillTreeSlotData->m_eSkillID = eSkillID;

		UISet.m_pSlot->SetID((int)eSkillID);
		UISet.m_pSlot->SetSlotData( pSkillTreeSlotData );

		pSkillTreeSlotData->m_eSkillID = eSkillID;
		pSkillTreeSlotData->m_bDataUpdated = true;
	
	}

	pControlList->SetIndex(0, 0);

	// Blind Static을 제일 앞으로 당겨주자
	CKTDGUIStatic* pStaticBlind = (CKTDGUIStatic*) m_pDLGUISkillTree->GetControl( L"g_pStaticBlind" );
	m_pDLGUISkillTree->ChangeSequence( pStaticBlind, true );
	CKTDGUIStatic* pStaticBlindSPNotice = (CKTDGUIStatic*) m_pDLGUISkillTree->GetControl( L"g_pStaticBlind_Notice" );
	m_pDLGUISkillTree->ChangeSequence( pStaticBlindSPNotice, true );


}

void CX2UIGuildSkillTreeInfo::UpdateUI( bool bEquipped, bool bSP /*= true*/, bool bSkillTree /*= false*/ )
{

	if(bSP)
	{
		UpdateSPInfo();
	}

	if(bSkillTree)
	{
		UpdateSkillTree();
	}

	

}

void CX2UIGuildSkillTreeInfo::UpdateSkillTree()
{
	// ControlList의 Map을 차례로 Iteration해 가면서 (이쪽이 빠르다!)
	// 슬롯이랑 버튼이 존재하면 해당 슬롯의 데이터를 읽어와서
	// 그냥 데이타 바뀌었다라고 눌러주면 되네? -_-
	// 1. 지금 익힐 수 있는 스킬 여부 : 버튼의 활성화/비활성화. bool 값에 따라서
	// 2. 1레벨이라도 익힌 스킬 여부 : 스킬 슬롯의 드래그 활성화 / 비활성화(패시브면 드래그 안 되게), 텍스쳐 변경. 스트링 설정 (레벨/최대레벨)

	for(map<CX2SkillTree::SKILL_ID, SkillSlotUI>::iterator it =	m_mapSkillSlotUISet.begin(); it != m_mapSkillSlotUISet.end(); ++it)
	{
		SkillSlotUI& refSkillSlotUI = it->second;
		if ( refSkillSlotUI.m_pSlot != NULL )
		{
			if(refSkillSlotUI.m_pSlot->GetSlotData() != NULL)
			{
				refSkillSlotUI.m_pSlot->GetSlotData()->m_bDataUpdated = true;			
			}		 
		}
	}
	UpdateBlind();
}

void CX2UIGuildSkillTreeInfo::UpdateSkillTree( CX2SkillTree::SKILL_ID eSkillID )
{
	map<CX2SkillTree::SKILL_ID, SkillSlotUI>::iterator it =	m_mapSkillSlotUISet.find(eSkillID);
	if( it != m_mapSkillSlotUISet.end() )
	{
		SkillSlotUI& refSkillSlotUI = it->second;
		if ( refSkillSlotUI.m_pSlot != NULL )
		{
			refSkillSlotUI.m_pSlot->GetSlotData()->m_bDataUpdated = true;			
		}
		const CX2SkillTree::SkillTreeTemplet* pSkillTreeTemplet = g_pData->GetSkillTree()->GetGuildSkillTreeTemplet((int)0, eSkillID);
		if(pSkillTreeTemplet != NULL)
		{
#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
			CX2SkillTree::SKILL_ID eFollowingSkillID = static_cast<CX2SkillTree::SKILL_ID>( pSkillTreeTemplet->m_iFollowingSkill );
			map<CX2SkillTree::SKILL_ID, SkillSlotUI>::iterator itfol =	m_mapSkillSlotUISet.find(eFollowingSkillID);
			if( itfol != m_mapSkillSlotUISet.end())
			{
				SkillSlotUI& refFollowingSkillSlotUI = itfol->second;
				if ( refFollowingSkillSlotUI.m_pSlot != NULL )
				{
					refFollowingSkillSlotUI.m_pSlot->GetSlotData()->m_bDataUpdated = true;			
				}

			}
#else // UPGRADE_SKILL_SYSTEM_2013
			for(vector<int>::const_iterator it = pSkillTreeTemplet->m_vecFollowingSkill.begin(); it != pSkillTreeTemplet->m_vecFollowingSkill.end(); ++it)
			{
				CX2SkillTree::SKILL_ID eFollowingSkillID = (CX2SkillTree::SKILL_ID) *it;
				map<CX2SkillTree::SKILL_ID, SkillSlotUI>::iterator itfol =	m_mapSkillSlotUISet.find(eFollowingSkillID);
				if( itfol != m_mapSkillSlotUISet.end())
				{
					SkillSlotUI& refFollowingSkillSlotUI = itfol->second;
					if ( refFollowingSkillSlotUI.m_pSlot != NULL )
					{
						refFollowingSkillSlotUI.m_pSlot->GetSlotData()->m_bDataUpdated = true;			
					}

				}

			}
#endif // UPGRADE_SKILL_SYSTEM_2013
		}

	}
	UpdateBlind();

}

void CX2UIGuildSkillTreeInfo::UpdateBlind()
{
	if( m_pDLGUISkillTree == NULL )
		return;
    
	CKTDGUIContolList* pControlList = (CKTDGUIContolList*) m_pDLGUISkillTree->GetControl( L"SkillTreeControlList" );
	CKTDGUIStatic* pStaticBlind = (CKTDGUIStatic*) m_pDLGUISkillTree->GetControl( L"g_pStaticBlind" );
	CKTDGUIStatic* pStaticBlindSPNotice = (CKTDGUIStatic*) m_pDLGUISkillTree->GetControl( L"g_pStaticBlind_Notice" );

	if( pStaticBlindSPNotice != NULL )
	{
		for(int i=0; i<pStaticBlindSPNotice->GetPictureNum(); ++i)
		{
			if( NULL != pStaticBlindSPNotice->GetPicture(i) )
			{
				pStaticBlindSPNotice->GetPicture(i)->SetShow(false);
			}
			if( NULL != pStaticBlindSPNotice->GetString(i) )
			{
				pStaticBlindSPNotice->GetString(i)->msg = L"";
			}
		}
	}
	
	if( pStaticBlind != NULL && pControlList != NULL )
	{
		int TopTier = pControlList->GetIndexY();
		int iUsedSP = 0;
		int iUsedCP = 0;
		g_pData->GetUIManager()->GetUIGuildBoard()->CalcUsedGuildSPointAndCSPoint( iUsedSP, iUsedCP );
		int iTotalUsedSP = iUsedCP + iUsedSP;

		const int MAGIC_SP_NEED_PER_TIER = 5;
		int OpenedLastTier = iTotalUsedSP / MAGIC_SP_NEED_PER_TIER;

		for(int i=0; i<pControlList->GetViewSizeY(); ++i)
		{
			if(TopTier + i > OpenedLastTier)
			{
				if(pStaticBlind->GetPicture(i) != NULL)
					pStaticBlind->GetPicture(i)->SetShow(true);
				if( TopTier + i == OpenedLastTier+1  )
				{
					if( pStaticBlindSPNotice->GetPicture(i) != NULL && pStaticBlindSPNotice->GetString(i) != NULL )
					{
						pStaticBlindSPNotice->GetPicture(i)->SetShow(true);
						pStaticBlindSPNotice->GetString(i)->msg = GET_REPLACED_STRING( ( STR_ID_4807, "i", (OpenedLastTier+1)*MAGIC_SP_NEED_PER_TIER) );
					}					
				}
			}
			else
			{
				if(pStaticBlind->GetPicture(i) != NULL)
					pStaticBlind->GetPicture(i)->SetShow(false);
			}
		}
	}
}


void CX2UIGuildSkillTreeInfo::UpdateSPInfo()
{
	if(m_pDLGUISkillTree == NULL)
		return;

	CKTDGUIStatic* pStatic_SP = (CKTDGUIStatic*)m_pDLGUISkillTree->GetControl( L"g_pStaticSP" );
	CKTDGUIStatic* pStatic_UsedSP = (CKTDGUIStatic*)m_pDLGUISkillTree->GetControl( L"g_pStaticUsedSP" );
	CKTDGUIStatic* pStatic_SPUndo = (CKTDGUIStatic*)m_pDLGUISkillTree->GetControl( L"g_pStaticUndo" );

	bool bUsingCSP = false;
	CTime cTime;
	// oasis907 : 김상윤 [2009.12.3] //
	KncUtil::ConvertStringToCTime( g_pData->GetUIManager()->GetUIGuildBoard()->m_wstrGuildCSPointEndDate, cTime );
	CTime tCurrentTime = g_pData->GetServerCurrentTime();

	if( tCurrentTime >= cTime )
	{
		bUsingCSP = false;
	}
	else
	{
		bUsingCSP = true;
	}

	CKTDGUIButton* pButton = (CKTDGUIButton*) m_pDLGUISkillTree->GetControl( L"Button_Notice_Notice_SP" );
	if(pButton != NULL)
	{

		CTime cTime;
		// oasis907 : 김상윤 [2009.12.3] //
		KncUtil::ConvertStringToCTime( g_pData->GetUIManager()->GetUIGuildBoard()->m_wstrGuildCSPointEndDate, cTime );
		CTime tCurrentTime = g_pData->GetServerCurrentTime();

		if( bUsingCSP )
		{
			pButton->SetShowEnable(true, true);
		}
		else
		{
			pButton->SetShowEnable(false, false);			
		}
	}

	if( g_pData->GetUIManager()->GetUIGuildBoard() != NULL )
	{
		int iNewSP = g_pData->GetUIManager()->GetUIGuildBoard()->m_iGuildSPoint + g_pData->GetUIManager()->GetUIGuildBoard()->m_iGuildCSPoint;
		int iUsedSP = 0;
		int iUsedCP = 0;
		g_pData->GetUIManager()->GetUIGuildBoard()->CalcUsedGuildSPointAndCSPoint( iUsedSP, iUsedCP );
		int iTotalUsedSP = iUsedCP + iUsedSP;

		if( NULL != pStatic_SP )
		{		
			wstring wstrText;
			if(bUsingCSP)
			{
				wstrText = GET_REPLACED_STRING( ( STR_ID_5301, "i", iNewSP ) );
			}
			else
			{
				wstrText = GET_REPLACED_STRING( ( STR_ID_5300, "i", iNewSP ) );
			}
			
			pStatic_SP->GetString(0)->msg = wstrText.c_str();
		}

		if ( NULL != pStatic_UsedSP )
		{
			wstring wstrText;
			if(bUsingCSP)
			{
				wstrText = GET_REPLACED_STRING( ( STR_ID_5303, "i", iTotalUsedSP ) );
			}
			else
			{
				wstrText = GET_REPLACED_STRING( ( STR_ID_5302, "i", iTotalUsedSP ) );
			}
			
			pStatic_UsedSP->GetString(0)->msg = wstrText.c_str();
		}

		if( NULL != pStatic_SPUndo )		
		{		
			wstring wstrText;
			wstrText = GET_REPLACED_STRING( ( STR_ID_5304, "i", 0 ) );
		}

		if( (m_iPrevSP == 0 && iNewSP > 0) || // SP가 0에서 0이상으로 변한 상황
			(m_iPrevSP > 0 && iNewSP <= 0 ) )	// SP가 0이상에서 0으로 변한 상황
		{
			UpdateSkillTree();
		}
		m_iPrevSP = iNewSP;
	}
	else
	{
		if( NULL != pStatic_SP )
			pStatic_SP->GetString(0)->msg = L"";
		if( NULL != pStatic_SPUndo )
			pStatic_SPUndo->GetString(0)->msg = L"";
	}

	// oasis907 : 김상윤 [2009.12.7] //
	CKTDGUIStatic* pStaticNoticeTop = (CKTDGUIStatic*)m_pDLGUISkillTree->GetControl( L"g_pStaticNoticeTop" );
	if( NULL != pStaticNoticeTop )
	{		
		pStaticNoticeTop->GetString(0)->msg = GET_STRING(STR_ID_4814);
	}

	CKTDGUIButton* pSkill_Reset = static_cast<CKTDGUIButton*>( m_pDLGUISkillTree->GetControl( L"Skill_Reset" ) );
	pSkill_Reset->SetEnable(false);

	CKTDGUIStatic* pStaticUndo = static_cast<CKTDGUIStatic*>( m_pDLGUISkillTree->GetControl( L"g_pStaticUndo" ) );
	pStaticUndo->GetString(0)->color = D3DXCOLOR( 0.95f,0.95f, 0.95f, 1.0f );
	pStaticUndo->GetString(0)->outlineColor = D3DXCOLOR( 0.5f,0.5f, 0.5f, 1.0f );
	
}

void CX2UIGuildSkillTreeInfo::CreateSlotUIPreset(SkillSlotUI& UISet, int eSkillID )
{
	if( m_pDLGUISkillTree == NULL )
		return;

	WCHAR buf[255] = {0,};
	
	m_pDLGUISkillTree->OpenScriptFile( L"DLG_Guild_Skill_Info_Icon.lua" );
	UISet.m_pStaticBackGround =			(CKTDGUIStatic*) m_pDLGUISkillTree->GetControl( L"g_pStaticSkill_Background" );
	UISet.m_pStaticActiveMark =			(CKTDGUIStatic*) m_pDLGUISkillTree->GetControl( L"StaticActive" );
	if(UISet.m_pStaticBackGround != NULL && UISet.m_pStaticActiveMark != NULL)
	{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( (CX2SkillTree::SKILL_ID)eSkillID );
	#else // UPGRADE_SKILL_SYSTEM_2013
		const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( (CX2SkillTree::SKILL_ID)eSkillID, 1 );
	#endif // UPGRADE_SKILL_SYSTEM_2013
		
		if(pSkillTemplet != NULL)
		{
			UISet.m_pStaticBackGround->GetPicture(0)->SetShow(false);
			UISet.m_pStaticBackGround->GetPicture(1)->SetShow(false);
			UISet.m_pStaticBackGround->GetPicture(2)->SetShow(false);
			UISet.m_pStaticBackGround->GetPicture(3)->SetShow(false);

#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
			UISet.m_pStaticBackGround->GetPicture(4)->SetShow(true);	/// 스킬 레벨 백판 분리로 인하여, 항상 표기
#endif // UPGRADE_SKILL_SYSTEM_2013

			UISet.m_pStaticActiveMark->GetPicture(0)->SetShow(false);
			UISet.m_pStaticActiveMark->GetPicture(1)->SetShow(false);

			switch(pSkillTemplet->m_eType)
			{
			case CX2SkillTree::ST_ACTIVE:
				{
					UISet.m_pStaticBackGround->GetPicture(0)->SetShow(true);
					UISet.m_pStaticActiveMark->GetPicture(0)->SetShow(true);
				} break;
			case CX2SkillTree::ST_SPECIAL_ACTIVE:
#ifdef FINALITY_SKILL_SYSTEM //JHKang
			case CX2SkillTree::ST_HYPER_ACTIVE_SKILL:
#endif //FINALITY_SKILL_SYSTEM
				{
					UISet.m_pStaticBackGround->GetPicture(1)->SetShow(true);
					UISet.m_pStaticActiveMark->GetPicture(0)->SetShow(true);
				} break;
				
			case CX2SkillTree::ST_BUFF:
				{
					UISet.m_pStaticBackGround->GetPicture(2)->SetShow(true);
					UISet.m_pStaticActiveMark->GetPicture(0)->SetShow(true);					
				} break;
#ifdef ADDED_RELATIONSHIP_SYSTEM
			case CX2SkillTree::ST_RELATIONSHIP_SKILL:
				{
					UISet.m_pStaticBackGround->GetPicture(3)->SetShow(true);
					UISet.m_pStaticActiveMark->GetPicture(0)->SetShow(true);
				} break;
#endif // ADDED_RELATIONSHIP_SYSTEM

			default:
				{
					UISet.m_pStaticBackGround->GetPicture(3)->SetShow(true);
					UISet.m_pStaticActiveMark->GetPicture(1)->SetShow(true);
				} break;
			}
		}
		else
		{
			ASSERT( !"NULL SKILLTEMPLET!!!" );
		}
		//wsprintf( buf, L"g_pStaticSkill_Background_%d", eSkillID );
		StringCchPrintf( buf, 255, L"g_pStaticSkill_Background_%d", eSkillID );
		UISet.m_pStaticBackGround->SetName(buf);
		//wsprintf( buf, L"StaticActive_%d", eSkillID );
		StringCchPrintf( buf, 255, L"StaticActive_%d", eSkillID );
		UISet.m_pStaticActiveMark->SetName(buf);
		
	}
	
	UISet.m_pSlot =						(CKTDGUISlot*) m_pDLGUISkillTree->GetControl( L"g_pSlot_SkillTree" );
	if(UISet.m_pSlot != NULL)
	{
		//wsprintf( buf, L"g_pSlot_SkillTree_%d", eSkillID );
		StringCchPrintf( buf, 255, L"g_pSlot_SkillTree_%d", eSkillID );
		UISet.m_pSlot->SetName(buf);
	}
	

	UISet.m_pButtonPlus =				(CKTDGUIButton*) m_pDLGUISkillTree->GetControl( L"Button_Plus" );
	if(UISet.m_pButtonPlus != NULL)
	{
		//wsprintf( buf, L"Button_Plus_%d", eSkillID );
		StringCchPrintf( buf, 255, L"Button_Plus_%d", eSkillID );
		UISet.m_pButtonPlus->SetName(buf);
		UISet.m_pButtonPlus->ChangeMouseUpSound( L"" );
		UISet.m_pButtonPlus->ChangeMouseOverSound( L"" );

	}
	
	UISet.m_pButtonMinus =				(CKTDGUIButton*) m_pDLGUISkillTree->GetControl( L"Button_Minus" );
	if(UISet.m_pButtonMinus != NULL)
	{
		//wsprintf( buf, L"Button_Minus_%d", eSkillID );
		StringCchPrintf( buf, 255, L"Button_Minus_%d", eSkillID );
		UISet.m_pButtonMinus->SetName(buf);
		UISet.m_pButtonMinus->ChangeMouseUpSound( L"" );
		UISet.m_pButtonMinus->ChangeMouseOverSound( L"" );
	}
	

	UISet.m_pButtonMaster =	(CKTDGUIButton*) m_pDLGUISkillTree->GetControl( L"Button_Master" );
	if( UISet.m_pButtonMaster != NULL )
	{
		//wsprintf( buf, L"Button_Master_%d", eSkillID );
		StringCchPrintf( buf, 255, L"Button_Master_%d", eSkillID );
		UISet.m_pButtonMaster->SetName(buf);
	}
	
	UISet.m_pStaticLevelString =		(CKTDGUIStatic*) m_pDLGUISkillTree->GetControl( L"StaticLevel" );
	if( UISet.m_pStaticLevelString != NULL)
	{
		//wsprintf( buf, L"StaticLevel_%d", eSkillID );
		StringCchPrintf( buf, 255, L"StaticLevel_%d", eSkillID );
		UISet.m_pStaticLevelString->SetName(buf);

	}


	UISet.m_pStaticLock =		(CKTDGUIStatic*) m_pDLGUISkillTree->GetControl( L"g_pStaticSkill_Lock" );
	if( UISet.m_pStaticLock != NULL)
	{
		//wsprintf( buf, L"StaticLock_%d", eSkillID );
		StringCchPrintf( buf, 255, L"StaticLock_%d", eSkillID );
		UISet.m_pStaticLock->SetName(buf);
	}


	return;
}

void CX2UIGuildSkillTreeInfo::CreateVerticalArrowUIPreset( ArrowUI& UISet, int iLineLength, int eSkillID )
{
	if( m_pDLGUISkillTree == NULL )
		return;

	m_pDLGUISkillTree->OpenScriptFile( L"DLG_Skill_Tree_Arrow_HeadTail.lua" );

	WCHAR buf[255] = {0,};

	UISet.m_pStaticArrowEnd =	(CKTDGUIStatic*) m_pDLGUISkillTree->GetControl( L"StaticSkill_Arrow_End" );
	if( UISet.m_pStaticArrowEnd != NULL )
	{
		//wsprintf( buf, L"StaticSkill_Arrow_End_%d", eSkillID );
		StringCchPrintf( buf, 255, L"StaticSkill_Arrow_End_%d", eSkillID );
		UISet.m_pStaticArrowEnd->SetName(buf);
	}

	UISet.m_pStaticArrowHead =	(CKTDGUIStatic*) m_pDLGUISkillTree->GetControl( L"StaticSkill_Arrow_Head" );
	if( UISet.m_pStaticArrowHead != NULL )
	{
		//wsprintf( buf, L"StaticSkill_Arrow_Head_%d", eSkillID );
		StringCchPrintf( buf, 255, L"StaticSkill_Arrow_Head_%d", eSkillID );
		UISet.m_pStaticArrowHead->SetName(buf);
	}

	for( int i = 0; i < iLineLength; ++i )
	{
		m_pDLGUISkillTree->OpenScriptFile( L"DLG_Skill_Tree_Arrow_Line.lua" );
		CKTDGUIStatic* pStaticArrowLine 		= (CKTDGUIStatic*) m_pDLGUISkillTree->GetControl( L"StaticSkill_Arrow_Line" );
		if( pStaticArrowLine != NULL )
		{
			//wsprintf( buf, L"StaticSkill_Arrow_Line_%d_%d", eSkillID, i );
			StringCchPrintf( buf, 255, L"StaticSkill_Arrow_Line_%d_%d", eSkillID, i );
			pStaticArrowLine->SetName(buf);
			UISet.m_vpStaticArrowLine.push_back(pStaticArrowLine);
		}		
	}
}

void CX2UIGuildSkillTreeInfo::CreateHorizonalArrowUIPreset( ArrowUI& UISet, int iLineLength, int eSkillID, bool bRight )
{

	if( m_pDLGUISkillTree == NULL )
		return;

	if(bRight)
	{
		m_pDLGUISkillTree->OpenScriptFile( L"DLG_Skill_Tree_Arrow_HeadTail_Right.lua" );
	}
	else
	{
		m_pDLGUISkillTree->OpenScriptFile( L"DLG_Skill_Tree_Arrow_HeadTail_Left.lua" );
	}
	

	WCHAR buf[255] = {0,};

	UISet.m_pStaticArrowEnd =	(CKTDGUIStatic*) m_pDLGUISkillTree->GetControl( L"StaticSkill_Arrow_End_H" );
	if( UISet.m_pStaticArrowEnd != NULL )
	{
		//wsprintf( buf, L"StaticSkill_Arrow_End_H_%d", eSkillID );
		StringCchPrintf( buf, 255, L"StaticSkill_Arrow_End_H_%d", eSkillID );
		UISet.m_pStaticArrowEnd->SetName(buf);
	}

	UISet.m_pStaticArrowHead =	(CKTDGUIStatic*) m_pDLGUISkillTree->GetControl( L"StaticSkill_Arrow_Head_H" );
	if( UISet.m_pStaticArrowHead != NULL )
	{
		//wsprintf( buf, L"StaticSkill_Arrow_Head_H_%d", eSkillID );
		StringCchPrintf( buf, 255, L"StaticSkill_Arrow_Head_H_%d", eSkillID );
		UISet.m_pStaticArrowHead->SetName(buf);
	}

	for( int i = 0; i < iLineLength; ++i )
	{
		m_pDLGUISkillTree->OpenScriptFile( L"DLG_Skill_Tree_Arrow_Line_Horizonal.lua" );
		CKTDGUIStatic* pStaticArrowLine 		= (CKTDGUIStatic*) m_pDLGUISkillTree->GetControl( L"StaticSkill_Arrow_Line_H" );
		if( pStaticArrowLine != NULL )
		{
			//wsprintf( buf, L"StaticSkill_Arrow_Line_H_%d_%d", eSkillID, i );
			StringCchPrintf( buf, 255, L"StaticSkill_Arrow_Line_H_%d_%d", eSkillID, i );
			pStaticArrowLine->SetName(buf);
			UISet.m_vpStaticArrowLine.push_back(pStaticArrowLine);
		}		
	}

}



void CX2UIGuildSkillTreeInfo::SetArrowColor( CX2SkillTree::SKILL_ID eSkillID, D3DXCOLOR cCol )
{
	std::pair< std::multimap<CX2SkillTree::SKILL_ID, ArrowUI>::iterator, std::multimap<CX2SkillTree::SKILL_ID, ArrowUI>::iterator> range = m_mapArrowUISet.equal_range(eSkillID);
	
	for (std::multimap<CX2SkillTree::SKILL_ID, ArrowUI>::iterator it = range.first; it != range.second; ++it)
	{
		ArrowUI& refArrow = it->second;

		refArrow.m_pStaticArrowEnd->SetColor( cCol );
		refArrow.m_pStaticArrowHead->SetColor( cCol );
		for(vector<CKTDGUIStatic*>::iterator itstatic =	refArrow.m_vpStaticArrowLine.begin(); itstatic != refArrow.m_vpStaticArrowLine.end(); ++itstatic)
		{
			CKTDGUIStatic* pStatic = *itstatic;
			pStatic->SetColor( cCol );
		}
		
	}
}



// CX2UIGuildSkillTreeInfo::SkillSlotUI& CX2UIGuildSkillTreeInfo::GetSkillSlotUI( CX2SkillTree::SKILL_ID eSkillID )
// {
// 	map<CX2SkillTree::SKILL_ID, SkillSlotUI>::iterator it = m_mapSkillSlotUISet.find(eSkillID);
// 	if(it != m_mapSkillSlotUISet.end())
// 	{
// 		return it->second;
// 	}
// 
// 	ASSERT( !"Wrong Skill ID" );
// 	return m_mapSkillSlotUISet.begin()->second;
// 
// }

// CX2UIGuildSkillTreeInfo::ArrowUI& CX2UIGuildSkillTreeInfo::GetArrowUI( CX2SkillTree::SKILL_ID eSkillID )
// {
// 	map<CX2SkillTree::SKILL_ID, ArrowUI>::iterator it = m_mapArrowUISet.find(eSkillID);
// 	if(it != m_mapArrowUISet.end())
// 	{
// 		return it->second;
// 	}
// 
// 	ASSERT( !"Wrong Skill ID" );
// 	return m_mapArrowUISet.begin()->second;
// }


// 잠긴 스킬에 우클릭을 하면 나타나는 다이얼로그를 열기 위한 함수.
// 아직 스킬 봉인해제 자체가 미구현이므로 기반만 닦아 둔다.
// 다이얼로그 닫는 경우까지만 만들어 뒀습니다. 구매 버튼을 눌렀을 때는 아무것도 안 하게 되어 있으니 다음 작업자 분께서는 
// UICustomMsgProc에서 case GSTICM_UNSEAL_SKILL_BUY 항목에 그 부분을 작업해 주세요.
void CX2UIGuildSkillTreeInfo::OpenSkillUnsealInformDLG( const CX2SkillTree::SkillTemplet* pSkillTemplet, D3DXVECTOR2 SlotCenterPos )
{	
	SAFE_DELETE_DIALOG( m_pDLGSkillUnsealInform );
		
	m_pDLGSkillUnsealInform = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Skill_Purchase_Window.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGSkillUnsealInform );
	m_pDLGSkillUnsealInform->SetDisableUnderWindow(true);

	// 다이얼로그가 열려야 할 위치(슬롯 중심에서부터의 상대값) : 스크립트에 넣어 둠. DLG_Skill_Purchase_Window.lua 최상단 참조.
	D3DXVECTOR2 OffsetPos((float)m_pDLGSkillUnsealInform->GetDummyInt(0), (float)m_pDLGSkillUnsealInform->GetDummyInt(1));	
	m_pDLGSkillUnsealInform->SetPos( SlotCenterPos + OffsetPos );
	
#if 0		//** fix!! 스킬 봉인해제 기능이 들어갈 때 여기에 추가작업 해 주세요. 
//{{AFX
	CKTDGUIStatic* pStaticRequireItem = (CKTDGUIStatic*) m_pDLGSkillUnsealInform->GetControl( L"StaticRequireItemName" );
	CKTDGUIStatic* pStaticRequireCash = (CKTDGUIStatic*) m_pDLGSkillUnsealInform->GetControl( L"StaticPrice" );

	if( NULL != pStaticRequireCash->GetString(0) )
	{
		//** 여기에 해당 스킬의 봉인해제서 가격을 써 주세요
		int iRequireCash = pSkillTemplet->가격;
		WCHAR buf[256] = {0,};
		//wsprintf( buf, L"(%d%s)", iRequireCash, GET_STRING(STR_ID_34) );
		StringCchPrintf( buf, 256, L"(%d%s)", iRequireCash, GET_STRING(STR_ID_34) );
		pStaticRequireCash->GetString(0)->msg = buf;
	}
	else
	{
		SAFE_DELETE_DIALOG(m_pDLGSkillUnsealInform);
		ASSERT( "Script Error." );
		return;
	}

	if( NULL != pStaticRequireItem->GetString(0) )
	{

		//** 여기에 해당 스킬의 봉인해제서 이름을 써 주세요
		WCHAR buf[256] = {0,};
		//wsprintf( buf, L"%s", pSkillTemplet->필요한아이템이름 );
		StringCchPrintf( buf, 256, L"%s", pSkillTemplet->필요한아이템이름 );
		pStaticRequireCash->GetString(0)->msg = buf;
	}
	else
	{
		SAFE_DELETE_DIALOG(m_pDLGSkillUnsealInform);
		ASSERT( "Script Error." );
		return;
	}

	m_pDLGSkillUnsealInform->SetShowEnable(true, true);
//}}AFX
#endif

		
}

// SlotManager의 ItemDesc 쪽 코드를 긁어왔음
void CX2UIGuildSkillTreeInfo::ShowSkillDesc( bool bShow, CX2SkillTree::SKILL_ID eSkillID /*= CX2SkillTree::SI_NONE*/, D3DXVECTOR2 pos /*= D3DXVECTOR2(0,0)*/, 
								    D3DXVECTOR2 size /*= D3DXVECTOR2(0,0)*/, bool bEquipped /*= false*/ )
{
	if(m_pDLGSkillToolTip == NULL )
	{
		m_pDLGSkillToolTip = new CKTDGUIDialog( g_pMain->GetNowState(), L"", 0.07f, XDL_POP_UP );
		m_pDLGSkillToolTip->SetFront(true);
		m_pDLGSkillToolTip->SetCloseOnFocusOut(true);

		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGSkillToolTip );
		m_pDLGSkillToolTip->SetFront( true );
		m_pDLGSkillToolTip->SetModal( true );

		CKTDGUIStatic* pStatic = new CKTDGUIStatic();
		pStatic->SetName( L"SkillDesc" );
		m_pDLGSkillToolTip->AddControl( pStatic );
		CKTDGUIControl::CPictureData* pPicture = new CKTDGUIControl::CPictureData();
		pStatic->AddPicture( pPicture );
		pPicture->SetEdge( false, 2, D3DXCOLOR(1,1,1,1) );
		pPicture->SetTex( L"FadeInOut.dds" );

		CKTDGUIControl::UIStringData* pString = new CKTDGUIControl::UIStringData();
		pStatic->AddString( pString );
		pString->fontIndex = SLOT_MANAGER_FONT_INDEX;
		pString->color = D3DXCOLOR( 1,1,1,1 );
		pString->outlineColor = D3DXCOLOR( 1,1,1,1 );
		pString->msg = L"";
		pString->fontStyle = CKTDGFontManager::FS_NONE;
		pString->sortFlag = DT_LEFT;
		pString->pos = D3DXVECTOR2( 20, 20 );
	}

	// 여기부터..

	if(bShow)
	{		
		wstring SkillDesc = GetSkillDesc(eSkillID, bEquipped);
		if( SkillDesc.empty() == true )
		{
			m_pDLGSkillToolTip->SetShow(false);
			return;
		}

		CKTDGUIStatic* pItemDesc = (CKTDGUIStatic*)m_pDLGSkillToolTip->GetControl(L"SkillDesc");

		const CKTDGFontManager::CUKFont* pFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( pItemDesc->GetString(0)->fontIndex );
		
#ifdef CLIENT_GLOBAL_LINEBREAK
		CWordLineHandler::LineBreakInX2Main( SkillDesc, pFont, (int)((float)MAGIC_SKILL_DESC_WIDTH*g_pKTDXApp->GetResolutionScaleX()), L"", true, false );
#else //#ifdef CLIENT_GLOBAL_LINEBREAK
		CX2Main::LineBreak( SkillDesc, pFont, (int)((float)MAGIC_SKILL_DESC_WIDTH*g_pKTDXApp->GetResolutionScaleX()), L"", true, false );
#endif //CLIENT_GLOBAL_LINEBREAK
			
		CKTDGFontManager::CUKFont* pItemDescFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( SLOT_MANAGER_FONT_INDEX );
		int itemDescWidth = (int)( pItemDescFont->GetWidth( SkillDesc.c_str() ) / g_pKTDXApp->GetResolutionScaleX() ) + ITEM_DESC_ADD_SIZE_X;
		int itemDescHeight = (int)( pItemDescFont->GetHeight( SkillDesc.c_str() ) / g_pKTDXApp->GetResolutionScaleY() ) + ITEM_DESC_ADD_SIZE_Y;
		
		// 아이템 툴팁의 길이가 너무 길면 폰트 크기를 줄이자
		
		if( NULL != pItemDesc && 
			NULL != pItemDesc->GetString(0) )
		{
			const int MAGIC_ITEM_TOOL_TIP_MAX_HEIGHT = 600;
			
			if( itemDescHeight > MAGIC_ITEM_TOOL_TIP_MAX_HEIGHT )
			{
				pItemDesc->GetString(0)->fontIndex = SLOT_MANAGER_FONT_INDEX_SMALL;
				pItemDescFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( SLOT_MANAGER_FONT_INDEX_SMALL );
				itemDescWidth = (int)( pItemDescFont->GetWidth( SkillDesc.c_str() ) / g_pKTDXApp->GetResolutionScaleX() ) + ITEM_DESC_ADD_SIZE_X;
				itemDescHeight = (int)( pItemDescFont->GetHeight( SkillDesc.c_str() ) / g_pKTDXApp->GetResolutionScaleY() ) + ITEM_DESC_ADD_SIZE_Y;
			}
			else
			{
				pItemDesc->GetString(0)->fontIndex = SLOT_MANAGER_FONT_INDEX;
			}
		}
		
		int offsetSize = 0;
		int lineNum = 0;
		offsetSize = SkillDesc.find( L"\n", offsetSize );
		while( offsetSize != -1 )
		{
			offsetSize++;
			lineNum++;
			offsetSize = SkillDesc.find( L"\n", offsetSize );
		}
		itemDescHeight += lineNum;

		int itemDescPosX = 0;
		int itemDescPosY = 0;

		if ( pos.x + ( size.x / 2 ) - ( itemDescWidth / 2 ) >= 0 ) //왼쪽에 공간이 충분한 경우
		{
			if ( pos.x+ ( size.x / 2 ) - ( itemDescWidth / 2 ) + itemDescWidth > 1024 )
			{
				itemDescPosX = (int)(1024 - itemDescWidth);
			}
			else
			{
				itemDescPosX = (int)(pos.x + ( size.x / 2 ) - ( itemDescWidth / 2 ));
			}
		}
		else
		{
			itemDescPosX = 0;
		}

		if ( pos.y - itemDescHeight >= 0 ) //슬롯위로 표시할 수 있는경우
		{
			itemDescPosY = (int)(pos.y - itemDescHeight);
		}
		else
		{
			itemDescPosY = (int)(pos.y + size.y);

			int overSize = 768 - (itemDescPosY + itemDescHeight);
			if ( overSize < 0 )
			{
				itemDescPosY += overSize;
			}
		}

		// 설명 위부분이 화면 밖으로 나가지 않도록 제한
		if( itemDescPosY < 0 )
			itemDescPosY = 0;
		
		m_pDLGSkillToolTip->SetPos( D3DXVECTOR2( (float)itemDescPosX, (float)itemDescPosY ) );
		m_pDLGSkillToolTip->GetStatic_LUA( "SkillDesc" )->GetPicture(0)->SetSize( D3DXVECTOR2( (float)itemDescWidth, (float)itemDescHeight) );
		m_pDLGSkillToolTip->GetStatic_LUA( "SkillDesc" )->GetPicture(0)->pPoint->color = D3DXCOLOR( 1,1,1,0.7f );

		bool bCheckEquipping = false;
		

		m_pDLGSkillToolTip->GetStatic_LUA( "SkillDesc" )->GetString(0)->msg = SkillDesc.c_str();

		m_pDLGSkillToolTip->SetShow( true );
	}
	else
	{
		m_pDLGSkillToolTip->SetShow( false );
	}
	
	
}

wstring CX2UIGuildSkillTreeInfo::GetSkillDesc( CX2SkillTree::SKILL_ID eSkillID, bool bEquipped /*= false*/ )
{
	CX2SkillTree* pSkillTree = g_pData->GetSkillTree();

	CX2UIGuildBoard* refUIGuildBoard = g_pData->GetUIManager()->GetUIGuildBoard();
	int iSkillLevel = refUIGuildBoard->GetGuildSkillLevel( eSkillID );
	//int iUnitclass = (int) g_pData->GetMyUser()->GetSelectUnit()->GetClass(); /
	int iUnitclass = 0; // oasis
	
	//{{  [11/11/2009 : oasis907 ]
	// 길드 스킬 레벨 아이템으로 인한 증가 X
	/*
	if( g_pX2Game != NULL )
	{
		int levelUpNum = g_pX2Game->GetMyUnit()->GetSkillLevelUpNum( eSkillID );
		iSkillLevel = iSkillLevel + levelUpNum;
	}
	*/
	
#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
	const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( eSkillID );
#else // UPGRADE_SKILL_SYSTEM_2013
	const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( eSkillID, (iSkillLevel != 0 ? iSkillLevel : 1) );
#endif // UPGRADE_SKILL_SYSTEM_2013
	
	if(pSkillTree == NULL || pSkillTemplet == NULL)
		return L"";

	wstringstream wstrm;
	wstrm << pSkillTemplet->m_wstrName << L" [";

	switch(pSkillTemplet->m_eType)
	{	
	case CX2SkillTree::ST_SPECIAL_ACTIVE:
		{
			wstrm << GET_STRING(STR_ID_2672);		// 스페셜 액티브

		} break;
#ifdef FINALITY_SKILL_SYSTEM //JHKang
	case CX2SkillTree::ST_HYPER_ACTIVE_SKILL:
		{
			wstrm << GET_STRING( STR_ID_26134 );		// 하이퍼 액티브

		} break;
#endif //FINALITY_SKILL_SYSTEM
	case CX2SkillTree::ST_ACTIVE:
		{
			wstrm << GET_STRING(STR_ID_2673);		// 액티브

		} break;
	case CX2SkillTree::ST_BUFF:
		{
			wstrm << GET_STRING(STR_ID_2674);		// 버프

		} break;
#ifdef ADDED_RELATIONSHIP_SYSTEM
	case CX2SkillTree::ST_RELATIONSHIP_SKILL:
		{
			wstrm << GET_STRING(STR_ID_2673);		// 액티브
//			wstrm << L"인연 스킬, String 추가 요망";
		} break;
#endif // ADDED_RELATIONSHIP_SYSTEM
	default:
		{
			wstrm << GET_STRING(STR_ID_2675);		// 패시브

		} break;
	}
	wstrm << L"]\n";




	/* 길드스킬에는 봉인 스킬 없음
	// 봉인된 스킬
	if( true == pSkillTemplet->m_bBornSealed && 
		false == refUserSkillTree.IsSkillUnsealed( eSkillID ) )
	{
		wstrm << L"\n[" << GET_STRING(STR_ID_3858) << L"]\n\n";
		wstrm << GET_STRING(STR_ID_3857);
		return wstrm.str();
	}

	*/




	if( bEquipped == false )
	{
		wstrm << L"(" << iSkillLevel << L"/" << pSkillTree->GetMaxGuildSkillLevel( iUnitclass, eSkillID ) << L")" << L"\n";
		wstrm << L"\n";
	}

	if(iSkillLevel >= 1)
	{
		if(pSkillTemplet->m_bShowSkillLevel == true)
		{
			if( bEquipped == false )
			{
				wstrm << GET_STRING(STR_ID_2647) << L"\n";		// 현재 레벨
			}
			wstrm << GET_STRING(STR_ID_489) << L" " << iSkillLevel << L"\n";	// Lv %d
		}
		wstrm << L"#CFFFF00";
		wstrm << pSkillTemplet->m_wstrMainDesc << L"\n";
		wstrm << L"\n";

	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
		if( pSkillTemplet->GetSkillMPConsumptionValue( 1 ) > 0 )
		{
			wstrm << GET_STRING(STR_ID_2648) << pSkillTemplet->GetSkillMPConsumptionValue( 1 ) << L"\n";		// 소모 MP
		}		
		if( pSkillTemplet->GetSkillCoolTimeValue( 1 ) > 0 )
		{
			wstrm <<  GET_REPLACED_STRING( ( STR_ID_315, "i", (int)ceil(pSkillTemplet->GetSkillCoolTimeValue( 1 )) ) );		// 쿨타임 %d초
		}
	#else // UPGRADE_SKILL_SYSTEM_2013
		if( pSkillTemplet->m_fMPConsumption > 0 )
		{
			wstrm << GET_STRING(STR_ID_2648) << pSkillTemplet->m_fMPConsumption << L"\n";		// 소모 MP
		}		
		if( pSkillTemplet->m_fSkillCoolTime > 0 )
		{
			wstrm <<  GET_REPLACED_STRING( ( STR_ID_315, "i", (int)ceil(pSkillTemplet->m_fSkillCoolTime ) ) );		// 쿨타임 %d초
		}
	#endif // UPGRADE_SKILL_SYSTEM_2013
		
		wstrm << L"#CX";
	}

	if(false == refUIGuildBoard->IsMaxGuildSkillLevel( eSkillID ) && false == bEquipped )
	{
		if(iSkillLevel >= 1)
		{
			wstrm << L"\n\n";
		}

	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
		const CX2SkillTree::SkillTemplet* pNextSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( eSkillID );

		wstrm << GET_STRING(STR_ID_2649) << L"\n";		// 다음 레벨(배우지 않음)
		wstrm << L"#CC0C0C0";
		wstrm << pNextSkillTemplet->m_wstrMainDesc << L"\n";
		wstrm << L"\n";

		if( pNextSkillTemplet->GetSkillMPConsumptionValue( 1 ) > 0 )
		{
			wstrm << GET_STRING(STR_ID_2648) << pNextSkillTemplet->GetSkillMPConsumptionValue( 1 ) << L"\n";		// 소모 MP
		}				
		if( pNextSkillTemplet->GetSkillCoolTimeValue( 1 ) > 0 )
		{
			wstrm <<  GET_REPLACED_STRING( ( STR_ID_315, "i", (int)ceil(pNextSkillTemplet->GetSkillCoolTimeValue( 1 )) ) );		// 쿨타임 %d초
		}
		wstrm << L"#CX";
	#else // UPGRADE_SKILL_SYSTEM_2013
		const CX2SkillTree::SkillTemplet* pNextSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( eSkillID, iSkillLevel+1 );

		wstrm << GET_STRING(STR_ID_2649) << L"\n";		// 다음 레벨(배우지 않음)
		wstrm << L"#CC0C0C0";
		wstrm << pNextSkillTemplet->m_wstrMainDesc << L"\n";
		wstrm << L"\n";

		if( pNextSkillTemplet->m_fMPConsumption > 0 )
		{
			wstrm << GET_STRING(STR_ID_2648) << pNextSkillTemplet->m_fMPConsumption << L"\n";		// 소모 MP
		}				
		if( pNextSkillTemplet->m_fSkillCoolTime > 0 )
		{
			wstrm <<  GET_REPLACED_STRING( ( STR_ID_315, "i", (int)ceil(pNextSkillTemplet->m_fSkillCoolTime) ) );		// 쿨타임 %d초
		}
		wstrm << L"#CX";
	#endif // UPGRADE_SKILL_SYSTEM_2013
	}
/*
	if( false == bEquipped && false == refUserSkillTree.CanILearnThisSkill( eSkillID, false ) )
	{
		const CX2SkillTree::SkillTreeTempletMap& refSkillTreeTempletMap = g_pData->GetSkillTree()->GetGuildSkillTreeTempletMap( iUnitclass );
		CX2SkillTree::SkillTreeTempletMap::const_iterator it = refSkillTreeTempletMap.find(eSkillID);
		if( it != refSkillTreeTempletMap.end() )
		{		
			const CX2SkillTree::SkillTreeTemplet& refSkillTreeTemplet = it->second;
			if(refSkillTreeTemplet.m_vecPrecedingSkill.empty() == false)
			{
				bool bPreMastered = true;
				for(vector<int>::const_iterator itPre = refSkillTreeTemplet.m_vecPrecedingSkill.begin(); itPre != refSkillTreeTemplet.m_vecPrecedingSkill.end(); ++itPre)
				{	
					CX2SkillTree::SKILL_ID PreccedSkillID = (CX2SkillTree::SKILL_ID)*itPre;
					if(false == refUserSkillTree.IsMaxSkillLevel( PreccedSkillID ))
					{
						bPreMastered = false;
					}
				}

				if(bPreMastered == false)
				{
					wstrm << L"#CE00000" << L"\n"<< GET_STRING(STR_ID_2671) << L"#CX\n";			// 선행 스킬
					bool bFirst = true;
					wstrm << L"#CE00000";
					for(vector<int>::const_iterator itPre = refSkillTreeTemplet.m_vecPrecedingSkill.begin(); itPre != refSkillTreeTemplet.m_vecPrecedingSkill.end(); ++itPre)
					{	
						CX2SkillTree::SKILL_ID PreccedSkillID = (CX2SkillTree::SKILL_ID)*itPre;
						if(bFirst == false)
						{
							wstrm<< L", ";
						}

						const CX2SkillTree::SkillTemplet* pPreccedSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( PreccedSkillID, 1 );
						if(pPreccedSkillTemplet != NULL)
						{
							wstrm << pPreccedSkillTemplet->m_Name;
						}
					}
					wstrm << GET_STRING(STR_ID_2676) << L"#CX";		// 의 마스터가 필요합니다.
				}
			}
		}
	}
*/
	return wstrm.str();
}


void CX2UIGuildSkillTreeInfo::ShowUIDesc( bool bShow, wstring wstr /*= L""*/, D3DXVECTOR2 pos /*= D3DXVECTOR2(0,0)*/, CX2TalkBoxManagerImp::TALK_BOX_TYPE TBT /*= CX2TalkBoxManagerImp::TBT_FROM_UP_RIGHT*/, D3DXCOLOR coTextColor /*= D3DXCOLOR(0,0,0,1)*/, D3DXCOLOR coBackColor /*= D3DXCOLOR(1, 0.96f, 0.6f, 1 )*/)
{
	m_pTalkBoxManager->Clear();

	if(bShow)
	{
		CX2TalkBoxManagerImp::TalkBox talkBox;

		talkBox.m_vPos				= pos;
		talkBox.m_GameUnitType		= CX2GameUnit::GUT_USER;
		talkBox.m_fRemainTime		= 2.0f;

		talkBox.m_wstrTalkContent	= wstr.c_str();
		talkBox.m_bTraceUnit		= false;
		talkBox.m_TalkBoxType		= TBT;
		talkBox.m_coTextColor		= coTextColor;
		talkBox.m_BackTexColor		= coBackColor;
	
		m_pTalkBoxManager->Push( talkBox );

	}
	
}





void CX2UIGuildSkillTreeInfo::OpenDLGSkillUnsealed( bool bOpen )
{
	if( false == bOpen )
	{
		if( m_pDLGSkillUnsealed != NULL )
		{
			m_pDLGSkillUnsealed->SetShowEnable( bOpen, bOpen );
		}
	}
	else
	{
		if( m_pDLGSkillUnsealed == NULL )
		{
			m_pDLGSkillUnsealed = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Skill_Acquisition.lua" );
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGSkillUnsealed );
		}

		m_pDLGSkillUnsealed->SetShowEnable( bOpen, bOpen );
	}
}


void CX2UIGuildSkillTreeInfo::UpdateDLGSkillUnsealed( CX2SkillTree::SKILL_ID eSkillID )
{
	if( m_pDLGSkillUnsealed == NULL )
		return;

#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
	const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( eSkillID );
#else // UPGRADE_SKILL_SYSTEM_2013
	const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( eSkillID, 1 );
#endif // UPGRADE_SKILL_SYSTEM_2013
	
	if( NULL == pSkillTemplet )
		return;
	

	CKTDGUIStatic* pStaticSkillSlot = (CKTDGUIStatic*) m_pDLGSkillUnsealed->GetControl( L"g_pStaticSkill_Acquisition_Slot" );
	if( NULL != pStaticSkillSlot &&
		NULL != pStaticSkillSlot->GetPicture(1) )
	{
		std::string textureName = "";
		std::string texturePieceName = "";

		ConvertWCHARToChar( textureName, pSkillTemplet->m_wstrIconName.c_str() );
		ConvertWCHARToChar( texturePieceName, pSkillTemplet->m_wstrIconPieceName.c_str() );

		pStaticSkillSlot->GetPicture(1)->SetTex_LUA( textureName.c_str(), texturePieceName.c_str() );
	}


	CKTDGUIStatic* pStaticSkillName = (CKTDGUIStatic*) m_pDLGSkillUnsealed->GetControl( L"Static_Skill_Name" );
	if( NULL != pStaticSkillName )
	{
		pStaticSkillName->GetString(0)->msg = pSkillTemplet->m_wstrName;
	}

}
#endif GUILD_BOARD
#endif GUILD_SKILL

#endif NEW_SKILL_TREE_UI






