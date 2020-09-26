#include "StdAfx.h"


#ifndef SERV_EPIC_QUEST
#include ".\x2uiQuest.h"

CX2UIQuest::CX2UIQuest( CKTDXStage* pNowStage, const WCHAR* pFileName )
: CX2ItemSlotManager( pNowStage, pFileName ),
m_iQuestPerPage(0),
m_iRewardSlotNum(0),
m_iQuestListSize(0),
m_iQuestListUnderTop(0),
m_bShow(false),
m_MovedPosition(0,0),						// D3DXVECTOR2
m_DLGPosition(0,0),							// D3DXVECTOR2
m_DLGSize(0,0),								// D3DXVECTOR2
m_pDLGUIQuest(NULL),
m_pDLGMsgBox(NULL),
//m_vQuestListSlot.clear();
//m_vQuestList.clear();
m_iTopIndex(0),
m_iPickedQuestID(-1),
m_iQuestNum(0),
m_SumDelta(0),
m_vQuestDescSize(0,0),						// D3DXVECTOR2
//m_vecQuestDesc.clear();
m_iQuestDescIndex(0),
m_iQuestDescLinePerPage(1),
//m_mapGetItem.clear();
m_pDLGQuickQuest(NULL),
m_bQuickQuestOpenedUP(true),
m_bQuickQuestOpen(true)
{

	// 초기화 ////////////////////////////////////////////////////////////////////////

	m_vQuestListSlot.clear();
	m_vQuestList.clear();
	m_vecQuestDesc.clear();

	//////////////////////////////////////////////////////////////////////////

	RegisterLuaBind();	

	m_pDLGUIQuest = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Quest.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGUIQuest );
	m_pDLGUIQuest->SetShowEnable(false, false);
	m_pDLGUIQuest->SetDisableUnderWindow(true);

	D3DXVECTOR3 tmp;
	tmp = m_pDLGUIQuest->GetDummyPos(0);
	m_DLGPosition.x = tmp.x;
	m_DLGPosition.y = tmp.y;
	tmp = m_pDLGUIQuest->GetDummyPos(1);
	m_DLGSize.x = tmp.x;
	m_DLGSize.y = tmp.y;

	m_iQuestPerPage = m_pDLGUIQuest->GetDummyInt(0);
	m_iRewardSlotNum = m_pDLGUIQuest->GetDummyInt(1);
}


CX2UIQuest::~CX2UIQuest(void)
{
	SAFE_DELETE_DIALOG( m_pDLGUIQuest );
	SAFE_DELETE_DIALOG( m_pDLGMsgBox );
	SAFE_DELETE_DIALOG( m_pDLGQuickQuest );

// 	for(UINT i=0; i<m_vQuestListSlot.size(); i++)
// 	{
// 		MyQuestListSlot* pQuestSlot = m_vQuestListSlot[i];
// 		SAFE_DELETE(pQuestSlot);
// 	}
	m_vQuestListSlot.clear();

// 	for(UINT i=0; i<m_vQuestList.size(); i++)
// 	{
// 		MyQuestList* pQuest = m_vQuestList[i];
// 		SAFE_DELETE(pQuest);
// 	}


	m_vQuestList.clear();
}

void CX2UIQuest::SetLayer( X2_DIALOG_LAYER layer )
{
	SetSlotManagerLayer(layer);
	SetSlotManagerChangeSequence(false);

	if(m_pDLGUIQuest != NULL) 
	{
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer(m_pDLGUIQuest, layer);
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDLGUIQuest, false );
	}
}

void CX2UIQuest::SetPosition( D3DXVECTOR2 vec )
{
	// 슬롯 다이얼로그 유닛뷰어
	if(m_pDLGUIQuest != NULL) m_pDLGUIQuest->SetPos(vec);

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

HRESULT CX2UIQuest::OnFrameMove( double fTime, float fElapsedTime )
{

	KTDXPROFILE();



	if(m_pDLGUIQuest != NULL && m_pDLGUIQuest->GetIsMouseOver() == true)
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

bool CX2UIQuest::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
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
			if ( false == m_pDLGUIQuest->GetIsMouseOver())
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
			if ( false == m_pDLGUIQuest->GetIsMouseOver())
			{
				return false;
			}
			g_pData->GetUIManager()->SetUILayerIndex(CX2UIManager::UI_MENU_QUEST, true);
			bFlag = true;			
		}
		break;
	default:
		break;
	}


	switch( uMsg )
	{
	case WM_MOUSEMOVE:
		{

			POINT mousePoint = { short(LOWORD(lParam)), short(HIWORD(lParam)) };

			D3DXVECTOR2 mousePos = D3DXVECTOR2( (float)mousePoint.x, (float)mousePoint.y );
			m_NowMousePos = mousePos;

			MouseMove( mousePos );
		}
		break;
	case WM_MOUSEWHEEL:
		{ 
			return OnMouseWheel(hWnd, uMsg, wParam,lParam);
		}break;
	}

	return bFlag;
}

bool CX2UIQuest::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch(wParam)
	{
	case QUCM_EXIT:
		{
			SetShow(false);
		}
		return true;
	case QUCM_SELECT_QUEST:
		{
			CKTDGUIRadioButton* pQuestButton = (CKTDGUIRadioButton*)lParam;
			MyQuestListSlot& QuestSlotList = m_vQuestListSlot[pQuestButton->GetDummyInt(0)];
			int QuestIndex = QuestSlotList.m_iQuestIndex;

			if( QuestIndex < (int)m_vQuestList.size() )
			{
				MyQuestList& Quest = m_vQuestList[QuestIndex];				
				SelectQuest( Quest.m_QuestID );
			}
		}
		return true;
	case QUCM_CATEGORY_BUTTON:
		{
			CKTDGUIButton* pCategoryButton = (CKTDGUIButton*)lParam;
			MyQuestListSlot& QuestSlotList = m_vQuestListSlot[pCategoryButton->GetDummyInt(0)];
			int QuestIndex = QuestSlotList.m_iQuestIndex;

			if( QuestIndex < (int)m_vQuestList.size() )
			{
				MyQuestList& Quest = m_vQuestList[QuestIndex];				
				Quest.m_bIsOpen = !Quest.m_bIsOpen;
				SelectCategory( Quest.m_DungeonID, Quest.m_bIsOpen );
			}

		}
		return true;
	case QUCM_LIST_UP:
		{
			SetNextTopIndex(true);
			UpdateQuestUI();
		}
		return true;
	case QUCM_LIST_DOWN:
		{
			SetNextTopIndex(false);
			UpdateQuestUI();
		}
		return true;
	case QUCM_COMMENT_UP:
		{
			m_iQuestDescIndex--;
			UpdateQuestDescPage();
		}
		return true;
	case QUCM_COMMENT_DOWN:
		{
			m_iQuestDescIndex++;
			UpdateQuestDescPage();
		}
		return true;
	case QUCM_QUEST_GIVEUP:
		{
			if( NULL != m_pDLGMsgBox )
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGMsgBox, NULL, false );
				m_pDLGMsgBox = NULL;
			}

			m_pDLGMsgBox = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_888 ), QUCM_QUEST_GIVEUP_CONFIRM, g_pMain->GetNowState(), QUCM_QUEST_GIVEUP_CANCLE );
			CKTDGUIButton* pButton_Cancle = (CKTDGUIButton*) m_pDLGMsgBox->GetControl( L"MsgBoxOkAndCancelCancelButton" );
			pButton_Cancle->RequestFocus();
		}
		return true;
	case QUCM_QUEST_GIVEUP_CONFIRM:
		{
			if( NULL != m_pDLGMsgBox )
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGMsgBox, NULL, false );
				m_pDLGMsgBox = NULL;
			}

			// 초심자 가이드
			if( m_iPickedQuestID == 11010 && 
				g_pTFieldGame != NULL && 
				g_pTFieldGame->GetNoviceGuide() != NULL && 
				g_pTFieldGame->GetNoviceGuide()->GetIsPlayGuide() == true )
			{			
				g_pTFieldGame->GetNoviceGuide()->SetNotifyGuide(CX2NoviceGuide::GE_PREV);
			}

			if( g_pData->GetUIManager()->GetUIQuestReceive() == NULL )
			{
				ASSERT( !"QuestReceive is NULL" );		// 최초 생성된 후 지워질 일이 없는 class.
				g_pData->GetUIManager()->CreateUIQuestReceive();
			}
			g_pData->GetUIManager()->GetUIQuestReceive()->Handler_EGS_GIVE_UP_QUEST_REQ( m_iPickedQuestID );

		}
		return true;
	case QUCM_QUEST_GIVEUP_CANCLE:
		{
			if( NULL != m_pDLGMsgBox )
			{
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGMsgBox, NULL, false );
				m_pDLGMsgBox = NULL;
			}

		}
		return true;
	case QUCM_LIST_NAVI_LCLICKED:
	case QUCM_LIST_NAVI_DRAGGING:
		{
			CKTDGUINavigation* pNavi	= (CKTDGUINavigation*)lParam;
			D3DXVECTOR2 vRelativePos = pNavi->GetRelativeWindowPos();

			if(vRelativePos.y <= 0.01)
			{
				m_iTopIndex = 0;
			}
			else
			{
				// 0.68 : Navi Size에 따른 값. MAGIC VALUE

				m_iTopIndex = (int)((vRelativePos.y / 0.68f) * (m_iQuestListSize - m_iQuestPerPage)) + 1;

			}

			UpdateQuestUI( false );

		}
		return true;
	case QUCM_COMMENT_NAVI_LCLICKED:
	case QUCM_COMMENT_NAVI_DRAGGING:
		{
			CKTDGUINavigation* pNavi	= (CKTDGUINavigation*)lParam;
			D3DXVECTOR2 vRelativePos = pNavi->GetRelativeWindowPos();

			// 0.55 : Navi Size에 따른 값. MAGIC VALUE
			m_iQuestDescIndex = (int)((vRelativePos.y / 0.55f) * ((int) m_vecQuestDesc.size() - m_iQuestDescLinePerPage));

			UpdateQuestDescPage( false );
		}
		return true;
	case QUCM_QUICK_QUEST_CHECKBOX:
		{
			CKTDGUICheckBox* pCheckBox = (CKTDGUICheckBox*) lParam;
			m_bQuickQuestOpen = pCheckBox->GetChecked();
			SetShowQuickQuestDLG( pCheckBox->GetChecked() );
		}
		return true;
	case QUCM_QUICK_QUEST_SIZE_BUTTON:
		{
			CKTDGUIButton* pButton = (CKTDGUIButton*)lParam;
			bool bQuickQuestDLGState = !GetQuickQuestDLGOpenedUP();
			OpenUpQuickQuesstDLG( bQuickQuestDLGState );
		}
		return true;
	default:
		break;
	}
	return false;
}

bool CX2UIQuest::UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{

	return false;
}

void CX2UIQuest::SetShow(bool val)
{
	m_bShow = val;

	if(val)	// 켜질 때 처리해야 할 부분
	{
		// UI의 퀘 리스트를 세팅해 주고
		ResetQuestUI();

		g_pData->GetUIManager()->UIOpened(CX2UIManager::UI_MENU_QUEST);
#ifdef DISABLE_STATEMENU_IN_DUNGEON
// 		switch(g_pMain->GetNowStateID())
// 		{
// 		case CX2Main::XS_ARCADE_GAME:
// 		case CX2Main::XS_DUNGEON_GAME:
// 		case CX2Main::XS_PVP_GAME:
// 			{
// 				//SetPosition( D3DXVECTOR2(270, 60) );
// 				//SetLayer(XDL_POP_UP);
// 			} break;
// 		default:
// 			break;
// 		}
#endif
	}
	else	// 꺼질 때 처리해야 할 부분
	{
		// 아이템 마우스 오버 이미지를 지우자
		m_pDLGSelectedItem->SetShow( false ); 
		InvalidSlotDesc();

		// 슬롯을 끄자
		for ( int i = 0; i < (int)m_SlotList.size(); i++ )
		{
			CX2SlotItem* pItemSlot = (CX2SlotItem*)m_SlotList[i];
			pItemSlot->LostItemUI();
			pItemSlot->SetShow( false );
			pItemSlot->SetEnable( false );
		}

		g_pData->GetUIManager()->UIClosed(CX2UIManager::UI_MENU_QUEST);
	}

	m_pDLGUIQuest->SetShowEnable(m_bShow, m_bShow);
}

//퀘스트 UI를 완전히 리셋한다
void CX2UIQuest::ResetQuestUI( bool bResetTopIndex )
{
	if(m_bShow)
	{
		// 보상 EXP/ED/SP String 0으로 만들어 주고
		CKTDGUIStatic* pStaticED = (CKTDGUIStatic*) m_pDLGUIQuest->GetControl( L"Static_ED" );
		pStaticED->GetString(0)->msg = L"";
		CKTDGUIStatic* pStaticExp = (CKTDGUIStatic*) m_pDLGUIQuest->GetControl( L"Static_Exp" );
		pStaticExp->GetString(0)->msg = L"";
		CKTDGUIStatic* pStaticSP = (CKTDGUIStatic*) m_pDLGUIQuest->GetControl( L"Static_SP" );
		pStaticSP->GetString(0)->msg = L"";

		// 수행 퀘스트 표시 체크박스 : 작은 퀘스트 창의 상태를 받아와서 켜졌는지 꺼졌는지 확인해 줘야 함.
		CKTDGUICheckBox* pCheckBox = (CKTDGUICheckBox*)m_pDLGUIQuest->GetControl(L"CheckBox_ShowQuickQuest"); 
		if ( pCheckBox != NULL )
		{
			pCheckBox->SetCheckedPure( m_bQuickQuestOpen );
		}

		// 슬롯 다 안 보이게 만들고
		for(UINT i=0; i<m_SlotList.size(); i++)
		{
			CX2SlotItem* pSlot = (CX2SlotItem*) m_SlotList[i];
			pSlot->SetShow(false);
			pSlot->SetEnable( false );
		}
		// 퀘스트 슬롯리스트 싹 리셋해준다 :
		// 1. 일단 다 지운다
		m_vQuestListSlot.clear();

		//2. 새로 만든다
		WCHAR buff[256] = {0,};
		for(int i=0; i<m_iQuestPerPage; i++)
		{
			MyQuestListSlot QuestListSlot;

			// 리스트 버튼 세팅
			//wsprintf( buff, L"RadioButton_List%d", i);
			StringCchPrintf( buff, 256, L"RadioButton_List%d", i);

			QuestListSlot.m_pListButton = (CKTDGUIRadioButton*) m_pDLGUIQuest->GetControl( buff );
			QuestListSlot.m_pListButton->SetShow(false);
			// 리스트 스태틱 세팅
			//wsprintf( buff, L"Static_QuestList%d", i);
			StringCchPrintf( buff, 256, L"Static_QuestList%d", i);
			QuestListSlot.m_pListStatic = (CKTDGUIStatic*) m_pDLGUIQuest->GetControl( buff );
			QuestListSlot.m_pListStatic ->SetShow(false);

			// 카데고리 버튼 세팅
			//wsprintf( buff, L"Button_Category%d", i);
			StringCchPrintf( buff, 256, L"Button_Category%d", i);
			QuestListSlot.m_pCategoryButton = (CKTDGUIButton*) m_pDLGUIQuest->GetControl( buff );
			QuestListSlot.m_pCategoryButton->SetShow(false);
			// 카데고리 스태틱 세팅
			//wsprintf( buff, L"Static_Category%d", i);
			StringCchPrintf( buff, 256, L"Static_Category%d", i);
			QuestListSlot.m_pCategoryStatic = (CKTDGUIStatic*) m_pDLGUIQuest->GetControl( buff );
			QuestListSlot.m_pCategoryStatic ->SetShow(false);

			m_vQuestListSlot.push_back(QuestListSlot);
		}

		if(bResetTopIndex)
		{
			m_iTopIndex = 0;
		}		
		// 이제 다시 퀘스트 리스트 받아오고
		UpdateQuestList();

		// UI를 업데이트 해 준다
		UpdateQuestUI();

	}
	else
	{
		if(m_bQuickQuestOpen)
		{
			UpdateQuestList();
		}
	}

}

void CX2UIQuest::UpdateQuestList()
{
	bool bLastSelectedQuestExist = false;
	// 열려있던 카데고리 ID를 저장한다
	vector<int> OpenedCategoryID;
	for(vector<MyQuestList>::iterator it = m_vQuestList.begin(); it < m_vQuestList.end(); advance(it, 1))
	{
		MyQuestList& QuestList = *it;
		if(QuestList.m_bIsCategory == true && QuestList.m_bIsOpen == true)
		{
			OpenedCategoryID.push_back(QuestList.m_DungeonID);
		}
	}


	// 싹 지우고
// 	for(UINT j=0; j<m_vQuestList.size(); j++)
// 	{
// 		MyQuestList* pQuest = m_vQuestList[j];
// 		SAFE_DELETE(pQuest);	
// 	}

	m_vQuestList.clear();

	// 퀘 리스트 받아오고
	const map<int,CX2QuestManager::QuestInst*> mapUnitQuest = g_pData->GetQuestManager()->GetUnitQuestMap();
	//{{ kimhc // 2010.1.29 //	PC방이 아닌 경우 PC방 퀘스트 목록에서 제외
#ifdef	PC_BANG_QUEST
	m_iQuestNum = 0;
#else	PC_BANG_QUEST
	m_iQuestNum = mapUnitQuest.size();
#endif	PC_BANG_QUEST
	//}} kimhc // 2010.1.29 //	PC방이 아닌 경우 PC방 퀘스트 목록에서 제외
	
	if ( mapUnitQuest.empty() == true )
		return;


	//vector<MyQuestList*> vQuestList;
	// map DungeonID, MyQuestlist.
	std::multimap<int, MyQuestList> mapQuestList;
	map<int, CX2QuestManager::QuestInst*>::const_iterator i;

	for ( i = mapUnitQuest.begin(); i != mapUnitQuest.end(); i++ )
	{
		int questID = i->first;
		if( questID == m_iPickedQuestID )
		{
			bLastSelectedQuestExist = true;
		}
		CX2QuestManager::QuestInst* pQuestInst = i->second;
		const CX2QuestManager::QuestTemplet* pQuestTemplet = g_pData->GetQuestManager()->GetQuestTemplet( questID );

		if ( pQuestInst == NULL || 
			pQuestInst->m_OwnorUnitUID != g_pData->GetMyUser()->GetSelectUnit()->GetUID() ||
			pQuestTemplet == NULL )
			continue;

		//{{ kimhc // 2010.1.29 //	PC방이 아닌 경우 PC방 퀘스트 목록에서 제외
#ifdef	PC_BANG_QUEST
		m_iQuestNum++;
#endif	PC_BANG_QUEST
		//}} kimhc // 2010.1.29 //	PC방이 아닌 경우 PC방 퀘스트 목록에서 제외

		// 퀘스트가 있단 걸 확인했으면 만들어 넣자..
		MyQuestList QuestList;
		QuestList.m_QuestID = questID;
		QuestList.m_bIsCategory = false;
		QuestList.m_bIsOpen = false;

		// 해당 던전 아이디를 어떻게 구해오자 : 첫번째 서브퀘스트를 기준으로 한다. 여러 던전에서 수행해야 하는 경우는 고려되어 있지 않다.
		// 전직퀘 or 이벤트퀘는 특수로 분류한다
		if( pQuestTemplet->m_vecSubQuest.size() > 0 && 
			( pQuestTemplet->m_eQuestType == CX2QuestManager::QT_NORMAL || pQuestTemplet->m_eQuestType == CX2QuestManager::QT_SKILL ) )
		{
			if(false == pQuestTemplet->m_vecSubQuest.empty())
			{
				int SubQuestID = pQuestTemplet->m_vecSubQuest[0];
				QuestList.m_DungeonID = GetSubQuestDungeonID( SubQuestID );				
			}
			else
			{
				ASSERT( !"Quest with No Subquest!" );
				QuestList.m_DungeonID = SEnum::DI_NONE;
			}
		}
		else
		{
			QuestList.m_DungeonID = SEnum::DI_NONE;
		}

		mapQuestList.insert(std::pair<int, MyQuestList>(QuestList.m_DungeonID, QuestList));
		//vQuestList.push_back(pQuestList);
	}

	// 맵으로 받았으니 DungeonID 순으로 정렬되어 있으므로
	// 카데고리 잘 집어넣어서
	// 벡터 하나로 묶어내자
	int LastDID = -1;
	bool bCurrentCategoryOpened = false;
	for(std::multimap<int, MyQuestList>::iterator i = mapQuestList.begin();	i != mapQuestList.end(); advance(i, 1))
	{
		// DID가 바뀐 경우 : 새 카테고리 탭을 넣어준다
		if( (i->first) != LastDID )
		{
			LastDID = i->first;
			MyQuestList QuestCategory;
			QuestCategory.m_bIsCategory = true;
// 			if(i->first == SEnum::DI_END)
// 				pQuestCategory->m_bIsOpen = true;
			QuestCategory.m_DungeonID = i->first;

			// 아~까 저장해뒀던 열려있던 카데고리 리스트를 이용해서 다시 열어줄지 닫아줄지 결정해주자..
			if( find(OpenedCategoryID.begin(), OpenedCategoryID.end(), QuestCategory.m_DungeonID) != OpenedCategoryID.end() )				
			{
				// 열려 있었으면
				QuestCategory.m_bIsOpen = true;
				bCurrentCategoryOpened= true;
			}
			else
			{
				// 아니면
				QuestCategory.m_bIsOpen = false;
				bCurrentCategoryOpened = false;
			}

			m_vQuestList.push_back(QuestCategory);
			m_iQuestListSize++;

		}

		MyQuestList QuestList = i->second;
		QuestList.m_bIsOpen = bCurrentCategoryOpened;
		m_vQuestList.push_back(QuestList);
		m_iQuestListSize++;
	}


	if(false == bLastSelectedQuestExist)
	{
		m_iPickedQuestID = -1;
	}	

	//m_vQuestList.insert( m_vQuestList.end(), vQuestList.begin(), vQuestList.end());
	UpdateQuickQuestDLG();

}

int CX2UIQuest::GetSubQuestDungeonID( int iSubQuestID )
{
	const CX2QuestManager::SubQuestTemplet* pSubQuestTemplet = g_pData->GetQuestManager()->GetSubQuestTemplet( iSubQuestID );

	if(pSubQuestTemplet != NULL )
	{
		switch( pSubQuestTemplet->m_eClearType )
		{
		case CX2QuestManager::SQT_NPC_HUNT:
		case CX2QuestManager::SQT_QUEST_ITEM_COLLECTION:
		case CX2QuestManager::SQT_DUNGEON_TIME:
		case CX2QuestManager::SQT_DUNGEON_RANK:
		case CX2QuestManager::SQT_DUNGEON_DAMAGE:
		case CX2QuestManager::SQT_DUNGEON_CLEAR_COUNT:
#ifdef SERV_INTEGRATION
		case CX2QuestManager::SQT_WITH_DIF_SERV_USER:
#endif SERV_INTEGRATION
#ifdef SERV_POINT_COUNT_SYSTEM
		case CX2QuestManager::SQT_POINT_COUNT:
#endif SERV_POINT_COUNT_SYSTEM
			{
				// DID는 normal-hard-expert가 붙어 있고, 30000부터 시작하고, 각 던전마다 10단위로 분류되어 있다.
				// 따라서 그냥 10자리를 자른 값을 넣어 준다.
				// 해당 던전이 존재하는 던전이면 던전 ID로 묶어주고
				if( NULL != g_pData->GetDungeonManager()->GetDungeonData((SEnum::DUNGEON_ID)pSubQuestTemplet->m_ClearCondition.m_eDungeonID) )
				{
					return (int)(pSubQuestTemplet->m_ClearCondition.m_eDungeonID / 10) * 10;
				}
				else
				{
					// 아니면 일반으로 묶는다
					return SEnum::DI_END;
				}				
			}
			break;
		case CX2QuestManager::SQT_NPC_TALK:
		case CX2QuestManager::SQT_ITEM_COLLECTION:
		case CX2QuestManager::SQT_PVP_PLAY:
		case CX2QuestManager::SQT_PVP_WIN:
		case CX2QuestManager::SQT_PVP_KILL:
#ifdef SERV_NEW_PVP_QUEST
		case CX2QuestManager::SQT_PVP_NPC_HUNT:
#endif SERV_NEW_PVP_QUEST
#ifdef SERV_PVP_NPC_HUNT_QUEST_EXTEND
		case CX2QuestManager::SQT_PVP_HERO_NPC_KILL:
		case CX2QuestManager::SQT_PVP_HERO_NPC_PLAY:
#endif //SERV_PVP_NPC_HUNT_QUEST_EXTEND

		default:
			{
				// 없다
				return SEnum::DI_END;
			} break;
		}
	}
	else
	{
		// 아예 해당 서브퀘스트가 없다
		return SEnum::DI_END;
	}

}

void CX2UIQuest::UpdateQuestUI( bool bUpdateNavi )
{
	if(m_iQuestListSize == 0) return;

	bool bLastCategoryWasOpen = true;
	int SlotIndex = 0;

	// 퀘스트 몇 개 받았나~
	WCHAR buff[64] = {0,};
	CKTDGUIStatic* pStaticQuestNum = (CKTDGUIStatic*) m_pDLGUIQuest->GetControl( L"Static_QuestNum" );
	//wsprintf( buff, L"%d/%d", m_iQuestNum, MY_QUEST_LIST_MAX_SIZE );
	StringCchPrintf( buff, 64, L"%d/%d", m_iQuestNum, MY_QUEST_LIST_MAX_SIZE );
	pStaticQuestNum->GetString(0)->msg = buff;

	m_iQuestListSize = 0;
	m_iQuestListUnderTop = 0;
	// 퀘스트 정보는 m_vQuestList에 다 들어와있다고 생각하고 m_iTopIndex에서부터 차례로 넣어서 세팅해 주자
	for(int i = 0; i<(int)m_vQuestList.size(); i++)
	{
		if(i<m_iTopIndex || SlotIndex >= m_iQuestPerPage)
		{
			MyQuestList& Quest = m_vQuestList[i];
			if(Quest.m_bIsCategory || Quest.m_bIsOpen)
			{
				m_iQuestListSize++;
				if(i>m_iTopIndex) m_iQuestListUnderTop++;
			}
			continue;
		}
		MyQuestListSlot& QuestListSlot = m_vQuestListSlot[SlotIndex];

		// 일단 다 끄자..
		QuestListSlot.m_pListButton->SetShow(false);
		QuestListSlot.m_pListStatic->SetShow(false);
		QuestListSlot.m_pCategoryStatic->SetShow(false);
		QuestListSlot.m_pCategoryButton->SetShow(false);

		MyQuestList& Quest = m_vQuestList[i];
		QuestListSlot.m_iQuestIndex = i;

		// 퀘 ID를 이용해서 퀘스트 정보를 얻고
		const CX2QuestManager::QuestTemplet* pQuestTemplet = g_pData->GetQuestManager()->GetQuestTemplet( Quest.m_QuestID );

		if( Quest.m_bIsCategory )
		{
			m_iQuestListSize++;
			if(i>m_iTopIndex) m_iQuestListUnderTop++;
			// 카데고리인 경우
			bLastCategoryWasOpen = Quest.m_bIsOpen;

			// 일단 픽쳐들을 다 끄고..
			QuestListSlot.m_pCategoryStatic->GetPicture(0)->SetShow(false);
			QuestListSlot.m_pCategoryStatic->GetPicture(1)->SetShow(false);
			QuestListSlot.m_pCategoryStatic->GetPicture(2)->SetShow(false);
			QuestListSlot.m_pCategoryStatic->GetPicture(3)->SetShow(false);
			QuestListSlot.m_pCategoryStatic->GetPicture(4)->SetShow(false);
			QuestListSlot.m_pCategoryStatic->GetPicture(5)->SetShow(false);
			QuestListSlot.m_pCategoryStatic->GetPicture(6)->SetShow(false);
			QuestListSlot.m_pCategoryStatic->GetPicture(7)->SetShow(false);
			// 던전 아이디를 이용해서 던전 이름을 세팅해 주고
			// DungeonData.m_DungeonName으로 얻어주자
			if(Quest.m_DungeonID == SEnum::DI_END)
			{
				QuestListSlot.m_pCategoryStatic->GetString(0)->msg = L"";
				QuestListSlot.m_pCategoryStatic->GetPicture(5)->SetShow(true);
			}
			else if(Quest.m_DungeonID ==SEnum::DI_NONE)
			{
				QuestListSlot.m_pCategoryStatic->GetString(0)->msg = L"";
				QuestListSlot.m_pCategoryStatic->GetPicture(6)->SetShow(true);
			}
			else if(NULL != g_pData->GetDungeonManager()->GetDungeonData((SEnum::DUNGEON_ID)Quest.m_DungeonID) )
			{
				wstring dungeonName = g_pData->GetDungeonManager()->GetDungeonData((SEnum::DUNGEON_ID)Quest.m_DungeonID)->m_DungeonName;
				QuestListSlot.m_pCategoryStatic->GetString(0)->msg = dungeonName;
			}
			else
			{
				ASSERT( !"던전 못 찾았음" );
				QuestListSlot.m_pCategoryStatic->GetString(0)->msg = L"";
			}

			// 던전 아이디->지역 아이디->지역 아이콘을 세팅해 주고 예아아아아

			CX2LocationManager::LOCAL_MAP_ID LMapID = g_pData->GetLocationManager()->GetLocalMapID( (SEnum::DUNGEON_ID)Quest.m_DungeonID );
			switch(LMapID)
			{
			case CX2LocationManager::LMI_RUBEN:
				{
					QuestListSlot.m_pCategoryStatic->GetPicture(0)->SetShow(true);
				} break;
			case CX2LocationManager::LMI_ELDER:
				{
					QuestListSlot.m_pCategoryStatic->GetPicture(1)->SetShow(true);
				} break;
			case CX2LocationManager::LMI_BESMA:
				{
					QuestListSlot.m_pCategoryStatic->GetPicture(2)->SetShow(true);
				} break;
			case CX2LocationManager::LMI_ALTERA_ISLAND:
				{
					QuestListSlot.m_pCategoryStatic->GetPicture(3)->SetShow(true);
				} break;
			case CX2LocationManager::LMI_PEITA:
				{
					QuestListSlot.m_pCategoryStatic->GetPicture(4)->SetShow(true);
				} break;
			case CX2LocationManager::LMI_VELDER:
				{
					QuestListSlot.m_pCategoryStatic->GetPicture(7)->SetShow(true);
				} break;
			case CX2LocationManager::LMI_INVALID:
			default:
				{

				} break;
			}
// 			wstring TextureName = GetVillageIconFileName( (int)LMapID );        
// 			wstring TextureKey = GetVillageIconKey( (int)LMapID );
// 			QuestListSlot.m_pCategoryStatic->GetPicture(0)->SetTex( TextureName.c_str(), TextureKey.c_str() );

			// 버튼이 눌러진 상태를 바꿔넣어 준다
			QuestListSlot.m_pCategoryButton->SetDownStateAtNormal(Quest.m_bIsOpen);

			QuestListSlot.m_pCategoryButton->SetShow(true);
			QuestListSlot.m_pCategoryStatic->SetShow(true);

		}
		else
		{
			// 퀘스트 버튼인 경우
			if(false == Quest.m_bIsOpen)
				continue;

			m_iQuestListSize++;
			if(i>m_iTopIndex) m_iQuestListUnderTop++;
			//제목 넣고
			QuestListSlot.m_pListStatic->GetString(0)->msg = pQuestTemplet->m_wstrTitle;
			// 색깔 넣고
			QuestListSlot.m_pListStatic->GetString(0)->color = g_pData->GetQuestManager()->GetQuestColor( Quest.m_QuestID, g_pData->GetSelectUnitLevel() );
			// 종류 정하고
			QuestListSlot.m_pListButton->SetShow(true);
			QuestListSlot.m_pListStatic->SetShow(true);
			QuestListSlot.m_pListStatic->GetPicture(0)->SetShow(false);
			QuestListSlot.m_pListStatic->GetPicture(1)->SetShow(false);
			QuestListSlot.m_pListStatic->GetPicture(2)->SetShow(false);
			QuestListSlot.m_pListStatic->GetPicture(3)->SetShow(false);
			//{{ kimhc // 2009-08-03 // 스킬 picture 추가
			QuestListSlot.m_pListStatic->GetPicture( 5 )->SetShow( false );
			//}} kimhc // 2009-08-03 // 스킬 picture 추가
			
			//{{ kimhc // 2010.2.1 //	PC방 퀘스트
#ifdef	PC_BANG_QUEST
			QuestListSlot.m_pListStatic->GetPicture( 6 )->SetShow( false );
#endif	PC_BANG_QUEST
				//}} kimhc // 2010.2.1 //	PC방 퀘스트

//{{ kimhc // 2010.4.27 // 비밀던전 작업(일일퀘스트 시스템)
#ifdef SERV_SECRET_HELL
			QuestListSlot.m_pListStatic->GetPicture( 7 )->SetShow( false );
#endif SERV_SECRET_HELL
//}} kimhc // 2010.4.27 // 비밀던전 작업(일일퀘스트 시스템)

			switch(pQuestTemplet->m_eQuestType)
			{
			case CX2QuestManager::QT_NORMAL:
				{
					//{{ kimhc // 2010.3.30 // 비밀던전 작업(일일퀘스트 시스템)
#ifdef	SERV_DAILY_QUEST
					switch ( pQuestTemplet->m_eRepeatType )
					{
					case  CX2QuestManager::QRT_NORMAL:
						QuestListSlot.m_pListStatic->GetPicture(0)->SetShow(true);
						break;

					case CX2QuestManager::QRT_REPEAT:
						QuestListSlot.m_pListStatic->GetPicture(3)->SetShow(true);
						break;

					case CX2QuestManager::QRT_DAY:
						QuestListSlot.m_pListStatic->GetPicture(7)->SetShow(true);
						break;


					default:
						break;
					}
#else	SERV_DAILY_QUEST
					if(pQuestTemplet->m_bRepeat == false)	// 일반퀘
					{
						QuestListSlot.m_pListStatic->GetPicture(0)->SetShow(true);

					}
					else	// 반복퀘
					{
						QuestListSlot.m_pListStatic->GetPicture(3)->SetShow(true);
					}
#endif	SERV_DAILY_QUEST
					//}} kimhc // 2010.3.30 // 비밀던전 작업(일일퀘스트 시스템)
				}break;
			case CX2QuestManager::QT_CHANGE_JOB:
				{
					QuestListSlot.m_pListStatic->GetPicture(1)->SetShow(true);
				}break;
			case CX2QuestManager::QT_EVENT:
				{
					QuestListSlot.m_pListStatic->GetPicture(2)->SetShow(true);
				}break;

				//{{ kimhc // 2009-08-03 // 스킬 picture 추가
			case CX2QuestManager::QT_SKILL:
				{
					QuestListSlot.m_pListStatic->GetPicture( 5 )->SetShow( true );
				} break;
				//}} kimhc // 2009-08-03 // 스킬 picture 추가

				//{{ kimhc // 2010.2.1 //	PC방 퀘스트
#ifdef	PC_BANG_QUEST
			case CX2QuestManager::QT_PCBANG:
				{
					QuestListSlot.m_pListStatic->GetPicture( 6 )->SetShow( true );
				} break;
#endif	PC_BANG_QUEST
				//}} kimhc // 2010.2.1 //	PC방 퀘스트

			default:
				break;
			}

			// 켜주고
			if(Quest.m_QuestID == m_iPickedQuestID && m_iPickedQuestID != -1 )
			{
				QuestListSlot.m_pListButton->SetChecked(true);
			}
			else
			{
				QuestListSlot.m_pListButton->SetChecked(false);
			}

		}

		// 완료된 퀘스트면
		QuestListSlot.m_pListStatic->GetPicture(4)->SetShow(false);
		const CX2QuestManager::QuestInst* pQuestInst = g_pData->GetQuestManager()->GetUnitQuest( Quest.m_QuestID );
		if( pQuestInst != NULL && true == pQuestInst->IsComplete() )
		{
			QuestListSlot.m_pListStatic->GetPicture(4)->SetShow(true);
		}

		SlotIndex++;

	}

	for(int i=SlotIndex; i<m_iQuestPerPage; i++)
	{
		MyQuestListSlot& QuestListSlot = m_vQuestListSlot[i];

		// 다 끄자
		QuestListSlot.m_pListButton->SetShow(false);
		QuestListSlot.m_pListStatic->SetShow(false);
		QuestListSlot.m_pCategoryStatic->SetShow(false);
		QuestListSlot.m_pCategoryButton->SetShow(false);
	}

	// 보상이랑 퀘스트 정보들
	const CX2QuestManager::QuestTemplet* pQuestTemplet = g_pData->GetQuestManager()->GetQuestTemplet( m_iPickedQuestID );
	const CX2QuestManager::QuestInst* pQuestInst = g_pData->GetQuestManager()->GetUnitQuest( m_iPickedQuestID );
	// 골라진 퀘가 있으면..
	if( pQuestTemplet != NULL && pQuestInst != NULL)
	{
		// 퀘가 완료 되었거나 게임 중일 때는 포기할 수 없게 한다
		CKTDGUIButton* pGiveUpButton = (CKTDGUIButton*)m_pDLGUIQuest->GetControl( L"Button_Giveup" );
		if( true == pQuestInst->IsComplete() || 
			g_pMain->GetNowStateID() == CX2Main::XS_DUNGEON_GAME ||
			g_pMain->GetNowStateID() == CX2Main::XS_PVP_GAME )
		{
			pGiveUpButton->SetShow(false);
		}
		else
		{
			pGiveUpButton->SetShow(true);
		}

		// 보상 정보 넣어주고
		WCHAR wszText[64] = {0,};
		CKTDGUIStatic* pStaticED = (CKTDGUIStatic*) m_pDLGUIQuest->GetControl( L"Static_ED" );
		//wsprintf( wszText, L"%d", pQuestTemplet->m_Reward.m_iED );
		StringCchPrintf( wszText, 64, L"%d", pQuestTemplet->m_Reward.m_iED );
		pStaticED->GetString(0)->msg = wszText;

		CKTDGUIStatic* pStaticExp = (CKTDGUIStatic*) m_pDLGUIQuest->GetControl( L"Static_Exp" );
		//wsprintf( wszText, L"%d", pQuestTemplet->m_Reward.m_iEXP );
		StringCchPrintf( wszText, 64, L"%d", pQuestTemplet->m_Reward.m_iEXP );
		pStaticExp->GetString(0)->msg = wszText;

		CKTDGUIStatic* pStaticSP = (CKTDGUIStatic*) m_pDLGUIQuest->GetControl( L"Static_SP" );
		//wsprintf( wszText, L"%d", pQuestTemplet->m_Reward.m_iSP );
		StringCchPrintf( wszText, 64, L"%d", pQuestTemplet->m_Reward.m_iSP );
		pStaticSP->GetString(0)->msg = wszText;

		// 퀘스트 설명을 만들고
		const wstring wstrDesc = CreateQuestDesc();
		UpdateQuestDesc( wstrDesc );
		m_iQuestDescIndex = 0;
		UpdateQuestDescPage();

		// 보상 아이템 슬롯 넣어주고
		SetRewardItemSlot();

	}
	else
	{
		// 골라진 퀘가 없다
		CKTDGUIButton* pGiveUpButton = (CKTDGUIButton*)m_pDLGUIQuest->GetControl( L"Button_Giveup" );
		pGiveUpButton->SetShow(false);

		// 보상 정보를 비우자
		CKTDGUIStatic* pStaticED = (CKTDGUIStatic*) m_pDLGUIQuest->GetControl( L"Static_ED" );
		pStaticED->GetString(0)->msg = L"";
		CKTDGUIStatic* pStaticExp = (CKTDGUIStatic*) m_pDLGUIQuest->GetControl( L"Static_Exp" );
		pStaticExp->GetString(0)->msg = L"";
		CKTDGUIStatic* pStaticSP = (CKTDGUIStatic*) m_pDLGUIQuest->GetControl( L"Static_SP" );
		pStaticSP->GetString(0)->msg = L"";

		// 퀘스트 정보 스태틱을..
		const wstring wstrDesc = CreateQuestDesc();
		UpdateQuestDesc( wstrDesc );
		m_iQuestDescIndex = 0;
		UpdateQuestDescPage();

		// 보상 아이템 정보를 비우자
		SetRewardItemSlot();
	}

	// 네비게이터 업뎃 해 주고
	if(bUpdateNavi)
		UpdateQuestListNavi();


}

void CX2UIQuest::ClearQuestSlotList()
{
	for(UINT i=0; i<m_vQuestListSlot.size(); i++)
	{
		MyQuestListSlot& QuestSlot = m_vQuestListSlot[i];
		QuestSlot.m_pListButton->SetShow(false);
		QuestSlot.m_pListStatic->SetShow(false);
	}

	for ( int i = 0; i < (int)m_SlotList.size(); i++ )
	{
		CX2SlotItem* pItemSlot = (CX2SlotItem*)m_SlotList[i];
		pItemSlot->DestroyItemUI();
		pItemSlot->SetShow( false );
		pItemSlot->SetEnable( false );
	}
}

void CX2UIQuest::SelectQuest( int SelectedQuestID )
{
	m_iPickedQuestID = SelectedQuestID;
	UpdateQuestUI();
}

void CX2UIQuest::SelectCategory( int SelectedCategoryDID, bool forOpen )
{
	for(UINT i=0; i<m_vQuestList.size(); i++)
	{
		MyQuestList& Quest = m_vQuestList[i];
		if(Quest.m_DungeonID == SelectedCategoryDID)
			Quest.m_bIsOpen = forOpen;
	}

	UpdateQuestUI();
}

void CX2UIQuest::SetRewardItemSlot()
{
	m_mapItemIDAndSocketID.clear();

	const CX2QuestManager::QuestTemplet* pQuestTemplet = g_pData->GetQuestManager()->GetQuestTemplet( m_iPickedQuestID );
	if( pQuestTemplet != NULL )
	{
		for(int i=0; i<m_iRewardSlotNum; i++)
		{
			CX2SlotItem* pSlotItem = (CX2SlotItem*) m_SlotList[i];

			if( i < (int)pQuestTemplet->m_Reward.m_vecItem.size() )
			{
				// 보상슬롯
				const CX2QuestManager::ITEM_DATA& itemData = pQuestTemplet->m_Reward.m_vecItem[i];
                const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( itemData.m_iItemID );
				if ( itemData.m_iSocketOption1 != 0 )
				{
					m_mapItemIDAndSocketID.insert( std::make_pair( itemData.m_iItemID, itemData.m_iSocketOption1 ) );
				}
				if(pItemTemplet != NULL)
				{
					CX2Item::ItemData kItemData;
					kItemData.m_PeriodType = pItemTemplet->GetPeriodType();
					kItemData.m_SocketOption.push_back(itemData.m_iSocketOption1);
					kItemData.m_ItemID = itemData.m_iItemID;
					kItemData.m_Endurance = pItemTemplet->GetEndurance();
					kItemData.m_Period = itemData.m_iPeriod;
					CX2Item* pItem = new CX2Item(kItemData, NULL);
					if ( pItem != NULL )
					{
						if ( pSlotItem != NULL )
						{
							pSlotItem->CreateItemUI( pItem, itemData.m_iQuantity );
							pSlotItem->SetShow(true);
							pSlotItem->SetEnable( true );
						}
					}
					SAFE_DELETE(pItem);
				}

			}
			else
			{
				// 비우자
				if(pSlotItem != NULL)
				{
					pSlotItem->DestroyItemUI();
					pSlotItem->SetShow(false);
					pSlotItem->SetEnable( false );
				}
			}
		}

		// 선택보상
		int SlotIndex = 0;
		for(int i=0; SlotIndex < m_iRewardSlotNum; i++)
		{
			CX2SlotItem* pSlotItem = (CX2SlotItem*) m_SlotList[SlotIndex+m_iRewardSlotNum];

			if( i < (int)pQuestTemplet->m_SelectReward.m_vecSelectItem.size())
			{
				// 선택보상슬롯
				const CX2QuestManager::ITEM_DATA& itemData = pQuestTemplet->m_SelectReward.m_vecSelectItem[i];
				const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( itemData.m_iItemID );
				//{{10.01.14 김정협 아이템 없을 시 문제 처리
				if( pItemTemplet == NULL )
					continue;
				//}}10.01.14 김정협 아이템 없을 시 문제 처리

				// fix!! 일단 선택보상에서는 unit_type만 체크
				if( pItemTemplet->GetUnitType() != CX2Unit::UT_NONE )
				{
					CX2Unit::UNIT_TYPE eUnitType = g_pData->GetMyUser()->GetSelectUnit()->GetType();
					if( pItemTemplet->GetUnitType() != eUnitType )
					{
						continue;
					}
				}
				if ( itemData.m_iSocketOption1 != 0 )
				{
					m_mapItemIDAndSocketID.insert( std::make_pair( itemData.m_iItemID, itemData.m_iSocketOption1 ) );
				}
				if(pItemTemplet != NULL)
				{
					CX2Item::ItemData kItemData;
					kItemData.m_PeriodType = pItemTemplet->GetPeriodType();
					kItemData.m_SocketOption.push_back(itemData.m_iSocketOption1);
					kItemData.m_ItemID = itemData.m_iItemID;
					kItemData.m_Period = itemData.m_iPeriod;
					kItemData.m_Endurance = pItemTemplet->GetEndurance();
					CX2Item* pItem = new CX2Item(kItemData, NULL);
					if ( pItem != NULL )
					{
						if ( pSlotItem != NULL )
						{
							pSlotItem->CreateItemUI( pItem, itemData.m_iQuantity );
							pSlotItem->SetShow(true);
							pSlotItem->SetEnable( true );
						}
					}
					SAFE_DELETE(pItem);

				}
			}
			else
			{
				// 비우자
				if(pSlotItem != NULL)
				{
					pSlotItem->DestroyItemUI();
					pSlotItem->SetEnable( false );
					pSlotItem->SetShow(false);
				}
			}
			SlotIndex++;
		}

	}
	else
	{
		// 다 비워버리자
		for(UINT i=0; i<m_SlotList.size(); i++)
		{
			CX2SlotItem* pSlotItem = (CX2SlotItem*) m_SlotList[i];
			pSlotItem->DestroyItemUI();
			pSlotItem->SetShow(false);
			pSlotItem->SetEnable(false);

		}
	}
}




bool CX2UIQuest::OnMouseWheel( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	D3DXVECTOR2 mousePos;

	mousePos.x = (float)g_pKTDXApp->GetDIManager()->GetMouse()->GetXPos();
	mousePos.y = (float)g_pKTDXApp->GetDIManager()->GetMouse()->GetYPos();
	//mousePos = g_pKTDXApp->MouseConvertByResolution( mousePos );

	short zDelta = (short)HIWORD(wParam);
	m_SumDelta += zDelta;
	while (abs(m_SumDelta) >= WHEEL_DELTA)
	{
		if(m_SumDelta>0)
		{
			if( MouseOnQuestListArea(mousePos) )
			{
				SetNextTopIndex(true);
				UpdateQuestUI();
			}
			else if(MouseOnQuestCommentArea(mousePos))
			{
				m_iQuestDescIndex--;
				UpdateQuestDescPage();
			}
			m_SumDelta -= WHEEL_DELTA;
		}
		else
		{
			if( MouseOnQuestListArea(mousePos) )
			{
				SetNextTopIndex(false);
				UpdateQuestUI();
			}
			else if(MouseOnQuestCommentArea(mousePos))
			{
				m_iQuestDescIndex++;
				UpdateQuestDescPage();
			}
			m_SumDelta += WHEEL_DELTA;

		}	
	}

	return true;
}
bool CX2UIQuest::MouseOnQuestListArea( D3DXVECTOR2 mousePos )
{
	// 퀘스트 목록의 위치와 크기
	D3DXVECTOR2 m_ConvertPos	= g_pKTDXApp->ConvertByResolution( D3DXVECTOR2(15, 75) + m_MovedPosition );
	D3DXVECTOR2 m_ConvertSize	= g_pKTDXApp->ConvertByResolution( D3DXVECTOR2(409, 172) );

	if ( mousePos.x >= m_ConvertPos.x   && mousePos.x <= m_ConvertPos.x + m_ConvertSize.x &&
		mousePos.y >= m_ConvertPos.y && mousePos.y <= m_ConvertPos.y + m_ConvertSize.y )
	{
		return true;
	}

	return false;

}
bool CX2UIQuest::MouseOnQuestCommentArea( D3DXVECTOR2 mousePos )
{
	// 퀘스트 내용 부분의 위치와 크기
	D3DXVECTOR2 m_ConvertPos	= g_pKTDXApp->ConvertByResolution( D3DXVECTOR2(15, 270) + m_MovedPosition );
	D3DXVECTOR2 m_ConvertSize	= g_pKTDXApp->ConvertByResolution( D3DXVECTOR2(406, 112) );

	if ( mousePos.x >= m_ConvertPos.x   && mousePos.x <= m_ConvertPos.x + m_ConvertSize.x &&
		mousePos.y >= m_ConvertPos.y && mousePos.y <= m_ConvertPos.y + m_ConvertSize.y )
	{
		return true;
	}

	return false;

}

// m_iTopIndex를 상하로 한 칸 움직이는 함수
// isUp이 true면 위로 false면 아래로
void CX2UIQuest::SetNextTopIndex(bool isUp)
{
	if(m_iQuestListSize <= m_iQuestPerPage)
	{
		m_iTopIndex = 0;
		return;
	}
	if(isUp)
	{
		if(m_iTopIndex == 0) return;
		for(int i=m_iTopIndex-1; i>=0; i--)
		{
			MyQuestList& Quest = m_vQuestList[i];
			if(Quest.m_bIsCategory || Quest.m_bIsOpen)
			{
				m_iTopIndex = i;
				return;
			}
		}
		m_iTopIndex = 0;
		return;
	}
	else
	{
		if(m_iQuestListUnderTop < m_iQuestPerPage)
		{
			// 더 못 내려가게
			return;
		}
		for(UINT i=m_iTopIndex+1; i<m_vQuestList.size(); i++)
		{
			MyQuestList& Quest = m_vQuestList[i];
			if(Quest.m_bIsCategory || Quest.m_bIsOpen)
			{
				m_iTopIndex = i;
				return;
			}
		}
		return;
	}
	ASSERT(!"SetNextTopIndex Failed");
	m_iTopIndex = 0;
}

void CX2UIQuest::UpdateQuestListNavi()
{
	if(m_pDLGUIQuest != NULL)
	{
		CKTDGUINavigation* pNavi = (CKTDGUINavigation*) m_pDLGUIQuest->GetControl( L"Navi_QuestList" );
		if( NULL != pNavi )
		{
			if( m_iQuestListSize <= m_iQuestPerPage )
			{
				pNavi->SetShow(false);
				return;
			}

			pNavi->SetShow(true);

			// 위치 정해주고~
			D3DXVECTOR2 vPos;
			vPos.x = 0.0f;
			// 0.68 : Navi Size에 따른 값. MAGIC VALUE
			vPos.y = (float)(m_iQuestListSize - m_iQuestListUnderTop - 1) / ((float)m_iQuestListSize - m_iQuestPerPage ) * 0.68f;

			pNavi->SetRelativeWindowPos( vPos );
		}
	}
}

void CX2UIQuest::UpdateQuestDesc( const wstring& wstrDesc )
{
	// 일단 wstrDesc를 만들어내자-_-
	// 클리어 조건 -> 
	enum CHAR_STATE
	{
		Q_NORMAL,
		Q_SHARP,
		Q_SHARP_C,
	};

	m_vQuestDescSize = D3DXVECTOR2( (float) m_pDLGUIQuest->GetDummyInt( 2 ), (float) m_pDLGUIQuest->GetDummyInt( 3 ) );
	m_vQuestDescSize = g_pKTDXApp->ConvertByResolution( m_vQuestDescSize );

	CKTDGUIStatic* pStatic_QuestDesc = (CKTDGUIStatic*) m_pDLGUIQuest->GetControl( L"Static_ClearCondition" );
	if( NULL == pStatic_QuestDesc )
		return;

	CKTDGFontManager::CUKFont* pFont 
		= g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( pStatic_QuestDesc->GetString(0)->fontIndex );
	if( NULL == pFont )
		return; 

	const int MAGIC_MAX_WIDTH = (int) m_vQuestDescSize.x;

	m_vecQuestDesc.resize(0);

	if( wstrDesc.length() == 0 )
	{
		m_iQuestDescIndex = 0;
		return;
	}

	int iNowCharWidth = 0;		
	int iNowLineWidth = 0;
	int iNowCharIndex = 0;
	wstring wstrLineBuf = L"";
	wstring wstrPageBuf = L"";
	wstring wstrColorCode = L"";

	CHAR_STATE eNowCharState = Q_NORMAL;
	bool bIsFirstCharOfLine = true; 

	int iDescLength = (int) wstrDesc.length();
	while( iNowCharIndex < iDescLength )
	{
		WCHAR wszCharBuf = wstrDesc[iNowCharIndex];
		wstring wstrCharBuf = wstrDesc.substr( iNowCharIndex, 1 ); 

		switch( eNowCharState )
		{
		case Q_NORMAL:
			{
				if( 0 == wstrCharBuf.compare( L"#" ) )
				{
					wstrLineBuf += wstrCharBuf;
					eNowCharState = Q_SHARP;
				}
				else if( 0 == wstrCharBuf.compare( L"\n" ) )
				{
					wstrLineBuf += wstrCharBuf;
					wstrPageBuf += wstrLineBuf;
					wstrLineBuf = L"";
					iNowLineWidth = 0;

					if( 0 != wstrColorCode.length() )
					{
						wstrLineBuf += wstrColorCode;
					}
					if(false == bIsFirstCharOfLine)
					{
						m_vecQuestDesc.push_back( wstrPageBuf );
					}

					bIsFirstCharOfLine = true;
					wstrPageBuf = L"";

				}
				else
				{
					if( 0 != wstrCharBuf.compare( L" " ) || false == bIsFirstCharOfLine )
					{
						if( true == bIsFirstCharOfLine )
							bIsFirstCharOfLine = false;
						wstrLineBuf += wstrCharBuf;					
						iNowCharWidth = pFont->GetWidth( wszCharBuf );
						iNowLineWidth += iNowCharWidth;
					}
				}

			} break;

		case Q_SHARP:
			{
				if( 0 == wstrCharBuf.compare( L"C" ) )
				{
					wstrLineBuf += wstrCharBuf;
					eNowCharState = Q_SHARP_C;
				}
				else if( 0 == wstrCharBuf.compare( L"m" ) )
				{
					wstrLineBuf.erase( wstrLineBuf.end()-1 ); // '#'을 지우자
					wstrPageBuf += wstrLineBuf;

					m_vecQuestDesc.push_back( wstrPageBuf );

					wstrLineBuf = L"";
					wstrPageBuf = L"";
					bIsFirstCharOfLine = true;
					if( 0 != wstrColorCode.length() )
					{
						wstrLineBuf += wstrColorCode;
					}

					eNowCharState = Q_NORMAL;
				}
				else
				{
					wstrLineBuf += wstrCharBuf;
					iNowCharWidth = pFont->GetWidth( wszCharBuf );
					iNowLineWidth += iNowCharWidth;

					eNowCharState = Q_NORMAL;
				}

			} break;

		case Q_SHARP_C:
			{
				if( 0 == wstrCharBuf.compare( L"X" ) )
				{
					wstrLineBuf += wstrCharBuf;
					wstrColorCode = L"";
					eNowCharState = Q_NORMAL;
				}
				else // RGB 
				{
					wstrColorCode = L"#C";
					wstrColorCode += wstrDesc.substr( iNowCharIndex, 6 );
					wstrLineBuf += wstrDesc.substr( iNowCharIndex, 6 );
					iNowCharIndex += 5;
					eNowCharState = Q_NORMAL;
				}

			} break;
		}

		if( iNowLineWidth >= MAGIC_MAX_WIDTH && Q_NORMAL == eNowCharState )
		{
			wstrLineBuf += L"\n";
			wstrPageBuf += wstrLineBuf;
			wstrLineBuf = L"";
			iNowLineWidth = 0;
			if( 0 != wstrColorCode.length() )
			{
				wstrLineBuf += wstrColorCode;
			}

			if(false == bIsFirstCharOfLine)
			{
				m_vecQuestDesc.push_back( wstrPageBuf );
			}
			bIsFirstCharOfLine = true;
			wstrPageBuf = L"";
		}

		iNowCharIndex++;
	}

	if( wstrLineBuf.length() > 0 )
	{
		wstrPageBuf += wstrLineBuf;
	}

	if( wstrPageBuf.length() > 0 )
	{
		m_vecQuestDesc.push_back( wstrPageBuf );
	}

	m_iQuestDescIndex = 0;

}

wstring CX2UIQuest::CreateQuestDesc()
{
	const CX2QuestManager::QuestTemplet* pQuestTemplet = g_pData->GetQuestManager()->GetQuestTemplet( m_iPickedQuestID );
	const CX2QuestManager::QuestInst* pQuestInst = g_pData->GetQuestManager()->GetUnitQuest( m_iPickedQuestID );

	wstringstream wstrm;

	// 일단 퀘스트 완료조건 넣는 부분
	if( pQuestTemplet != NULL && pQuestInst != NULL )
	{
		// 선택된 퀘가 있으면~
		bool bThereWasNPCTalkSubQuest = false;

		// sub-quest
		for( UINT i=0; i<pQuestTemplet->m_vecSubQuest.size(); i++ )
		{
			const CX2QuestManager::SubQuestTemplet* pSubQuestTemplet = g_pData->GetQuestManager()->GetSubQuestTemplet( pQuestTemplet->m_vecSubQuest[i] );
			if( NULL == pSubQuestTemplet )
				continue;

			const CX2QuestManager::ClearCondition& clearCond = pSubQuestTemplet->m_ClearCondition;
			if( CX2QuestManager::SQT_NPC_TALK == pSubQuestTemplet->m_eClearType )
			{
				bThereWasNPCTalkSubQuest = true;
			}

			wstring wstrSubQuestDesc = L"";
			wstrSubQuestDesc = g_pData->GetQuestManager()->MakeSubQuestString( pSubQuestTemplet->m_bAutomaticDescription, pSubQuestTemplet, pQuestInst );

			if( 0 != wstrSubQuestDesc.length() )
			{
				if( CX2QuestManager::SQT_NPC_TALK != pSubQuestTemplet->m_eClearType &&
					NULL != pQuestInst && true == pQuestInst->GetSubQuest(pQuestTemplet->m_vecSubQuest[i])->IsComplete() )
				{
					// 완료 된 거
					wstrm << L"#CAA0000" << wstrSubQuestDesc << L"#CX\n" ;
				}
				else
				{
					// 완료 안 된 거
					wstrm << L"#C281CC0" << wstrSubQuestDesc << L"#CX\n";
				}
			}
		}

		if( false == bThereWasNPCTalkSubQuest )
		{
			wstringstream wstrmQuestCond;

			if( NULL != pQuestInst && true == pQuestInst->IsComplete() )
			{
                wstring wstrText;
				const CX2UnitManager::NPCUnitTemplet* pNPCTemplet = g_pData->GetUnitManager()->GetNPCUnitTemplet( pQuestTemplet->m_eEndNPCID );
				if( NULL != pNPCTemplet )
				{
					wstrText = GET_REPLACED_STRING( ( STR_ID_889, "LLI", pNPCTemplet->m_Description, pNPCTemplet->m_Name, KHanSungPostWordUnicodeWrapper( ( WCHAR* )pNPCTemplet->m_Name.c_str(), STR_ID_453, STR_ID_454 ) ) );
				}
				wstrm << wstrText << "\n";
			}
		}
		// 한줄 띄우고 메인 텍스트를 넣어 주자

		wstrm << L"#CX\n" << L" \n" << pQuestTemplet->m_wstrMainText;
	}

	return wstrm.str().c_str();
}

void CX2UIQuest::UpdateQuestDescPage( bool bUpdateNavi )
{
	if(m_iQuestDescIndex > (int)m_vecQuestDesc.size() - m_iQuestDescLinePerPage) m_iQuestDescIndex = m_vecQuestDesc.size() - m_iQuestDescLinePerPage;
	if(m_iQuestDescIndex < 0) m_iQuestDescIndex = 0;

	CKTDGUIStatic* pStatic_QuestDesc = (CKTDGUIStatic*) m_pDLGUIQuest->GetControl( L"Static_ClearCondition" );
	if( NULL == pStatic_QuestDesc )
		return;

	CKTDGFontManager::CUKFont* pFont 
		= g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( pStatic_QuestDesc->GetString(0)->fontIndex );
	if( NULL == pFont )
		return; 

	m_iQuestDescLinePerPage = (int) (m_vQuestDescSize.y / (float) pFont->GetHeight() * 0.5f);


	wstringstream wstrm;

	if( m_vecQuestDesc.size() == 0 )
	{
		m_iQuestDescIndex = 0;

	}
	else
	{
		int j = 0;
		for(UINT i=m_iQuestDescIndex; i<m_vecQuestDesc.size(); i++, j++)
		{
			wstrm << m_vecQuestDesc[i];
			if(j>m_iQuestDescLinePerPage) break;
		}

	}

	pStatic_QuestDesc->GetString(0)->msg = wstrm.str().c_str();

	if(bUpdateNavi)
		UpdateQuestCommentNavi();

}

void CX2UIQuest::UpdateQuestCommentNavi()
{
	if(m_pDLGUIQuest != NULL)
	{
		CKTDGUINavigation* pNavi = (CKTDGUINavigation*) m_pDLGUIQuest->GetControl( L"Navi_QuestComment" );

		if( NULL != pNavi )
		{
			if( (int)m_vecQuestDesc.size() <= m_iQuestDescLinePerPage )
			{
				pNavi->SetShow(false);
				return;
			}

			pNavi->SetShow(true);

			// 위치 정해주고~
			D3DXVECTOR2 vPos;
			vPos.x = 0.0f;
			// 0.55 : Navi Size에 따른 값. MAGIC VALUE
			vPos.y = m_iQuestDescIndex / ((float) m_vecQuestDesc.size() - m_iQuestDescLinePerPage) * 0.55f;


			pNavi->SetRelativeWindowPos( vPos );
		}

	}
}



void CX2UIQuest::SetShowQuickQuestDLG( bool forOpen )
{
	if( forOpen && m_bQuickQuestOpen)
	{
		// 열자
		if(m_pDLGQuickQuest == NULL)
		{
			RegisterLuaBind();
			m_pDLGQuickQuest = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Quest_Restor.lua" );
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGQuickQuest );
		}

		//UpdateQuickQuestDLG();
		OpenUpQuickQuesstDLG( true );
		m_pDLGQuickQuest->SetShow(true);
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDLGQuickQuest, false );

	}
	else
	{
		// 닫자
		if(m_pDLGQuickQuest != NULL)
		{
			m_pDLGQuickQuest->SetShow(false);
			SAFE_DELETE_DIALOG(m_pDLGQuickQuest);
			m_pDLGQuickQuest = NULL;
		}
	}
}
bool CX2UIQuest::GetShowQuickQuestDLG()
{
	if( m_pDLGQuickQuest == NULL ) return false;
	return (m_bQuickQuestOpen && m_pDLGQuickQuest->GetShow());
}
void CX2UIQuest::UpdateQuickQuestDLG()
{
	if( m_pDLGQuickQuest == NULL || 
		m_bQuickQuestOpenedUP == false ||
		m_bQuickQuestOpen == false ) return;
	CKTDGUIStatic* pStatic = (CKTDGUIStatic*) m_pDLGQuickQuest->GetControl( L"Static_QuickQuest" );
	if(pStatic == NULL || pStatic->GetString(0) == NULL)
	{
		ASSERT( !"QuickQuest Static Read Error" );
		return;
	}

	CKTDGFontManager::CUKFont* pFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( pStatic->GetString(0)->fontIndex );
	if( NULL == pFont )
		return; 

	m_pDLGQuickQuest->SetStage( g_pMain->GetNowState() );

	wstring wstr;
	// "특수" 카테고리 탭의 이름이 적힌 적이 있나?
	// 초기값을 true로 잡고, 특수 카데고리가 존재하면 false로 바꿔서 그려주도록 한다.
	bool bSpecialQuestCategoryWritten = true;	
	// 퀘스트 리스트 죽 읽어서
	for(UINT i=0; i<m_vQuestList.size(); i++)
	{
		MyQuestList& Quest = m_vQuestList[i];
		if(Quest.m_bIsCategory)
		{
			if(Quest.m_DungeonID == SEnum::DI_END)
				continue;
			if(Quest.m_DungeonID == SEnum::DI_NONE)
			{
				// "특수" 탭 카테고리 존재
				bSpecialQuestCategoryWritten = false;
				continue;
			}

			// 던전 아이디가 "특수" 가 아니고, 특수탭 퀘스트는 있는데 카데고리 이름이 적힌 적이 없으면 (즉 해당 던전 퀘스트가 없으면)
			if(false == bSpecialQuestCategoryWritten)
			{
				// 카데고리 이름 써주고
				wstr += L"#C23AF1E+ ";
                wstr += GET_STRING( STR_ID_890 );
                wstr += L"#CX\n";
				bSpecialQuestCategoryWritten = true;
			}

			// 카데고리에 던전 이름 쓰자~
			if(g_pMain->GetNowStateID() == CX2Main::XS_DUNGEON_GAME && 
				g_pData->GetDungeonRoom() != NULL && g_pData->GetDungeonRoom()->GetDungeonID() / 10 == Quest.m_DungeonID / 10)
			{
				wstr += L"#CFFFF00- ";
			}
			else
			{
				wstr += L"#C23AF1E+ ";
			}
			wstr += g_pData->GetDungeonManager()->GetDungeonData((SEnum::DUNGEON_ID)Quest.m_DungeonID)->m_DungeonName;
			wstr += L"#CX\n";
		}
		else if( Quest.m_DungeonID == SEnum::DI_NONE )
		{
			// "특수" 로 분류된, 이벤트/전직 퀘스트
			// 일단 해당 퀘스트가 지금 이 던전에서 처리되는지 확인해보자.
			if(g_pMain->GetNowStateID() == CX2Main::XS_DUNGEON_GAME && g_pData->GetDungeonRoom() != NULL )
			{
				const CX2QuestManager::QuestTemplet* pQuestTemplet = g_pData->GetQuestManager()->GetQuestTemplet( Quest.m_QuestID );
				if(pQuestTemplet != NULL && false == pQuestTemplet->m_vecSubQuest.empty())
				{
					// 첫번째 서브퀘스트 기준으로
					int SubQuestDungeonID = GetSubQuestDungeonID( pQuestTemplet->m_vecSubQuest[0] );
					int CurrentDungeonID = g_pData->GetDungeonRoom()->GetDungeonID();
					// 해당 던전이면 ( 난이도 무시하려고 10으로 나눈다 )
					if(SubQuestDungeonID / 10 == CurrentDungeonID / 10)
					{
						// 퀘스트 표시 해 주자.                        
						// 카데고리 이름이 적힌 적이 없으면
						if(false == bSpecialQuestCategoryWritten)
						{
							// 카데고리 이름 써주고
							wstr += L"#CFFFF00- ";
                            wstr += GET_STRING( STR_ID_890 );
                            wstr += L"#CX\n";
							bSpecialQuestCategoryWritten = true;
						}

						//{{ 이제 퀘스트 내용을 써준다
						const CX2QuestManager::QuestInst* pQuestInst = g_pData->GetQuestManager()->GetUnitQuest( Quest.m_QuestID );

						// 퀘스트 완료조건 넣는 부분
						if( pQuestInst != NULL )
						{
							// sub-quest
							for( UINT i=0; i<pQuestTemplet->m_vecSubQuest.size(); i++ )
							{
								const CX2QuestManager::SubQuestTemplet* pSubQuestTemplet = g_pData->GetQuestManager()->GetSubQuestTemplet( pQuestTemplet->m_vecSubQuest[i] );
								if( NULL == pSubQuestTemplet )
									continue;

								const CX2QuestManager::ClearCondition& clearCond = pSubQuestTemplet->m_ClearCondition;

								wstring wstrSubQuestDesc = L"";
								wstrSubQuestDesc = g_pData->GetQuestManager()->MakeSubQuestString( pSubQuestTemplet->m_bAutomaticDescription, pSubQuestTemplet, pQuestInst );

								if( 0 != wstrSubQuestDesc.length() )
								{
									if( CX2QuestManager::SQT_NPC_TALK != pSubQuestTemplet->m_eClearType &&
										NULL != pQuestInst && true == pQuestInst->GetSubQuest(pQuestTemplet->m_vecSubQuest[i])->IsComplete() )
									{
										wstr += L"#CFBAF5D";
										wstr += wstrSubQuestDesc;
										wstr += L" (";
                                        wstr += GET_STRING( STR_ID_759 );
                                        wstr += L")#CX";
									}
									else
									{
										wstr += wstrSubQuestDesc;
									}

									wstr += L"\n";
								}
							}
						}
						//}} 퀘스트 내용 다 썼다

					}
				}
			}
		}
		else if(g_pMain->GetNowStateID() == CX2Main::XS_DUNGEON_GAME && 
			g_pData->GetDungeonRoom() != NULL && g_pData->GetDungeonRoom()->GetDungeonID() / 10 == Quest.m_DungeonID / 10)
		{
			// 퀘스트 내용 쓰자~
			const CX2QuestManager::QuestTemplet* pQuestTemplet = g_pData->GetQuestManager()->GetQuestTemplet( Quest.m_QuestID );
			const CX2QuestManager::QuestInst* pQuestInst = g_pData->GetQuestManager()->GetUnitQuest( Quest.m_QuestID );

			// 일단 퀘스트 완료조건 넣는 부분
			if( pQuestTemplet != NULL && pQuestInst != NULL )
			{
				// sub-quest
				for( UINT i=0; i<pQuestTemplet->m_vecSubQuest.size(); i++ )
				{
					const CX2QuestManager::SubQuestTemplet* pSubQuestTemplet = g_pData->GetQuestManager()->GetSubQuestTemplet( pQuestTemplet->m_vecSubQuest[i] );
					if( NULL == pSubQuestTemplet )
						continue;

					const CX2QuestManager::ClearCondition& clearCond = pSubQuestTemplet->m_ClearCondition;

					wstring wstrSubQuestDesc = L"";
					wstrSubQuestDesc = g_pData->GetQuestManager()->MakeSubQuestString( pSubQuestTemplet->m_bAutomaticDescription, pSubQuestTemplet, pQuestInst );

					if( 0 != wstrSubQuestDesc.length() )
					{
						if( CX2QuestManager::SQT_NPC_TALK != pSubQuestTemplet->m_eClearType &&
							NULL != pQuestInst && true == pQuestInst->GetSubQuest(pQuestTemplet->m_vecSubQuest[i])->IsComplete() )
						{
							wstr += L"#CFBAF5D";
							wstr += wstrSubQuestDesc;
							wstr += L" (";
                            wstr += GET_STRING( STR_ID_759 );
                            wstr += L")#CX";
						}
						else
						{
							wstr += wstrSubQuestDesc;
						}

						wstr += L"\n";
					}
				}
			}
		}
	}

	// 끝까지 다 돌았는데 "특수" 텍스트가 생기지 않은 경우 : 특수탭 퀘스트"만" 있는 경우에 대비해서
	if(false == bSpecialQuestCategoryWritten)
	{
		// 카데고리 이름 써주고
		wstr += L"#C23AF1E+ ";
        wstr += GET_STRING( STR_ID_890 );
        wstr += L"#CX\n";
		bSpecialQuestCategoryWritten = true;
	}

	// 여기서 일단 string은 완성됐고. 다음 Line Break 해주자.


	int nLine = 0;
	const int MAGIC_TEXT_WIDTH = 220;
	const int MAGIC_DESC_ADD_SIZE_Y = 20;

#ifdef CLIENT_GLOBAL_LINEBREAK
	nLine = CWordLineHandler::LineBreakInX2Main( wstr, pFont, (int)((float)MAGIC_TEXT_WIDTH*g_pKTDXApp->GetResolutionScaleX()), L"", true );
#else //#ifdef CLIENT_GLOBAL_LINEBREAK
	nLine = CX2Main::LineBreak( wstr, pFont, (int)((float)MAGIC_TEXT_WIDTH*g_pKTDXApp->GetResolutionScaleX()), L"", true );
#endif //CLIENT_GLOBAL_LINEBREAK

	CKTDGUIStatic* pStatic_BG = (CKTDGUIStatic*) m_pDLGQuickQuest->GetControl( L"Static_QuickQuest" );

	int DescHeight = (int)( pFont->GetHeight( wstr.c_str() ) / g_pKTDXApp->GetResolutionScaleY() )+ MAGIC_DESC_ADD_SIZE_Y;
	int DescWidth = 0;

	// Y 값의 최소값을 보장해 준다. MinSize는 원래의 Lua 파일에 있던 크기
	const int MinSizeY = 126;
	DescHeight = max(DescHeight, MinSizeY);
	DescWidth = (int)pStatic_BG->GetPicture(1)->GetSize().x;

	// 배경
	pStatic_BG->GetPicture(1)->SetSize( D3DXVECTOR2( (float)DescWidth, (float)DescHeight ) );

	// 바닥
	pStatic_BG->GetPicture(2)->SetPos( D3DXVECTOR2( 0, (float)DescHeight+20 ) );


	pStatic->GetString(0)->msg = wstr.c_str();

}

void CX2UIQuest::OpenUpQuickQuesstDLG( bool forOpen )
{
	if( NULL == m_pDLGQuickQuest )
		return;

	CKTDGUIStatic* pStatic = (CKTDGUIStatic*) m_pDLGQuickQuest->GetControl( L"Static_QuickQuest" );
	//CKTDGUIButton* pButton = (CKTDGUIButton*) m_pDLGQuickQuest->GetControl( L"Button_QuickQuest" );

	m_bQuickQuestOpenedUP = forOpen;
	if(forOpen) 
		UpdateQuickQuestDLG();

	//pButton->SetDownStateAtNormal(forOpen);
	if( NULL != pStatic )
		pStatic->SetShow(forOpen);

}

bool CX2UIQuest::GetOverMouseQuest()
{
	if( GetShow() == true && m_pDLGUIQuest != NULL && m_pDLGUIQuest->GetIsMouseOver() == true )
		return true;
	return false;		
}

// wstring CX2UIQuest::GetVillageIconFileName( int LocalMapID )
// {
// 	switch(LocalMapID)
// 	{
// 	case CX2LocationManager::LMI_VELDER_NORTH:
// 	case CX2LocationManager::LMI_VELDER_EAST:
// 	case CX2LocationManager::LMI_VELDER_SOUTH:
// 	case CX2LocationManager::LMI_ALTERA_ISLAND:
// 	case CX2LocationManager::LMI_PEITA:
// 	case CX2LocationManager::LMI_INVALID:
// 		return L"DLG_UI_Common_Texture02.TGA";
// 	default:
// 		break;
// 	}
// 	return L"";
// }
// wstring CX2UIQuest::GetVillageIconKey( int LocalMapID )
// {
// 	switch(LocalMapID)
// 	{
// 	case CX2LocationManager::LMI_VELDER_NORTH:
// 		return L"ruben";
// 	case CX2LocationManager::LMI_VELDER_EAST:
// 		return L"eldu";	
// 	case CX2LocationManager::LMI_VELDER_SOUTH:
// 		return L"besma";	
// 	case CX2LocationManager::LMI_ALTERA_ISLAND:
// 		return L"altera";	
// 	case CX2LocationManager::LMI_PEITA:
// 		return L"besma";	
// 	case CX2LocationManager::LMI_INVALID:
// 		return L"feel";
// 	default:
// 		break;
// 	}
// 	return L"";
// 	
// }

wstring CX2UIQuest::GetSlotItemDesc()
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
			CX2Item* pkItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( 
				m_pNowOverItemSlot->GetItemUID() );
			if ( pkItem != NULL )
				itemDesc = GetSlotItemDescByUID( m_pNowOverItemSlot->GetItemUID() );
			else
			{
				CX2Item* pItem = NULL;
				map< int, int >::iterator mit;
				mit = m_mapItemIDAndSocketID.find( m_pNowOverItemSlot->GetItemTID() );
				if ( mit != m_mapItemIDAndSocketID.end() )
				{

					const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( m_pNowOverItemSlot->GetItemTID() );
					if ( pItemTemplet != NULL )
					{
					    CX2Item::ItemData kItemData;
						kItemData.m_PeriodType = pItemTemplet->GetPeriodType();
						kItemData.m_Endurance = pItemTemplet->GetEndurance();
						int socketID = mit->second; 
						kItemData.m_SocketOption.push_back( (short)socketID );
						kItemData.m_ItemID = m_pNowOverItemSlot->GetItemTID();
#ifdef QUEST_REWARD_PERIOD
						if(m_pNowOverItemSlot->GetPeriod() != 0)
						{
							kItemData.m_Period = m_pNowOverItemSlot->GetPeriod();
						}
#endif QUEST_REWARD_PERIOD
						CX2Item* pItem = new CX2Item( kItemData, NULL );
						itemDesc = GetSlotItemDescByTID( pItem, m_pNowOverItemSlot->GetItemTID() );
						SAFE_DELETE( pItem );
					}

				}
#ifdef QUEST_REWARD_PERIOD	
				else if(m_pNowOverItemSlot->GetPeriod() != 0)
				{
					const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( m_pNowOverItemSlot->GetItemTID() );
					if ( pItemTemplet != NULL )
					{
					    CX2Item::ItemData kItemData;
						kItemData.m_PeriodType = pItemTemplet->GetPeriodType();
						kItemData.m_Period = m_pNowOverItemSlot->GetPeriod();
						kItemData.m_Endurance = pItemTemplet->GetEndurance();
						kItemData.m_ItemID = m_pNowOverItemSlot->GetItemTID();
						CX2Item* pItem = new CX2Item( kItemData, NULL );
						itemDesc = GetSlotItemDescByTID( pItem, m_pNowOverItemSlot->GetItemTID() );
						SAFE_DELETE( pItem );
					}
				}
#endif QUEST_REWARD_PERIOD
				else
				{
					itemDesc = GetSlotItemDescByTID( pkItem, m_pNowOverItemSlot->GetItemTID() );
				}

			}
		}

	}

	return itemDesc;	
}
#endif SERV_EPIC_QUEST

