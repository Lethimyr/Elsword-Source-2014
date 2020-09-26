#include "StdAfx.h"
#include ".\x2statetrainingschool.h"

CX2StateTrainingSchool::CX2StateTrainingSchool(void) :
m_iTrainPerPage(0),
m_iRewardSlotNum(0),
m_iTopIndex(0),
m_iTrainListSize(0),
m_iPickedTrainID(-1),
//m_vecTrainingListSlot,
m_pDLGUITrainingSchool(NULL), 
m_pDLGSchoolBack(NULL),
m_pUINPC(NULL),
m_vNpcNamePos(0,0),			// D3DXVECTOR2
m_SumDelta(0)
{

	RegisterLuaBind();	

	m_pDLGSchoolBack = new CKTDGUIDialog( this, L"DLG_Training_BG.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGSchoolBack );

	//g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer( m_pDLGMenu, XDL_NORMAL_1 );

	//{{
	
	m_pDLGUITrainingSchool = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Training.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGUITrainingSchool );
	
	m_iTrainPerPage = m_pDLGUITrainingSchool->GetDummyInt(0);
	m_iRewardSlotNum = m_pDLGUITrainingSchool->GetDummyInt(1);

	ResetTrainUI();
	//}}


	//g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer( m_pDLGMenu, XDL_NORMAL_1 );


//	CX2StateHouse::CreateHouseNPCName(CX2LocationManager::HI_LOW_TRAINNING_ZONE);
	m_pUINPC = g_pData->GetUIUnitManager()->GetUIUnit( CX2UnitManager::NUI_LOW );
	if( NULL != m_pUINPC )
	{
		m_pUINPC->SetAlphaObject(true);										
		m_pUINPC->StateChange( m_pUINPC->GetWaitStateID() );
		g_pKTDXApp->GetDGManager()->AddObjectChain( m_pUINPC );		
		m_pUINPC->SetShowObject( true );
		m_pUINPC->SetCameIn( false );
	}	

// 	m_eSchoolMode = TSM_TRAINING;
// 	if( g_pMain->GetLastStageWasTrainingGame() == true )
// 	{
// 		g_pMain->SetLastStageWasTrainingGame( false );
// 		ChangeSchoolMode( TSM_TRAINING );
// 	}
// 	else
// 	{
// #if 0 
// 		ChangeSchoolMode( TSM_INTRO );
// 
// 		// 로우의 호출 퀘스트  
// 		const int MAGIC_QUEST_ID = 13;
// 		CX2QuestManager::QuestInst* pQuestInst = g_pData->GetQuestManager()->GetUnitQuest( MAGIC_QUEST_ID );
// 		if( NULL != pQuestInst )
// 		{
// 			CreateMouseClickUI( (CKTDXStage*) this, D3DXVECTOR2(530, 310) );
// 		}
// #endif
// 	}

	if( true == g_pMain->GetAutoStartNextTraining() )
	{
		g_pMain->SetAutoStartNextTraining( false );

		CX2Unit::UNIT_CLASS eUnitClass = g_pData->GetMyUser()->GetSelectUnit()->GetClass();
		int iNextTrainingID = g_pData->GetTrainingCenterTable()->GetNextTrainingID( eUnitClass, g_pMain->GetNowTrainingID() );


		CX2TrainingCenterTable::TC_TABLE_INFO trainingInfo;
		if( true == g_pData->GetTrainingCenterTable()->GetTrainingInfo( trainingInfo, iNextTrainingID ) &&
			true == g_pData->GetTrainingCenterTable()->CheckIfEnter( g_pData->GetMyUser()->GetSelectUnit()->GetMapTCClear(), iNextTrainingID ) )
		{
			m_iPickedTrainID = iNextTrainingID;
			Handler_EGS_CREATE_TC_ROOM_REQ( m_iPickedTrainID );
		}
	}
	
	g_pKTDXApp->SkipFrame();

 	
// 	m_vNpcNamePos = GetNpcNamePos();
// 	m_vNpcNamePos.x += 600.f;
// 	SetNpcNamePos(m_vNpcNamePos);



	m_pDlgNpcMessage = new CX2TFieldNpcShop();
	//m_pDlgNpcMessage->SetLayer(XDL_MESSENGER);
	m_pDlgNpcMessage->SetShopType(CX2TFieldNpcShop::NSBT_CLOSE);
	m_pDlgNpcMessage->SetHouseName( GET_STRING( STR_ID_740 ) );
	m_pDlgNpcMessage->SetNpcMessage( GET_STRING( STR_ID_741 ) );
	m_pDlgNpcMessage->SetShow(true);
	m_pDlgNpcMessage->SetStage(this);
}

CX2StateTrainingSchool::~CX2StateTrainingSchool(void)
{
	SAFE_DELETE_DIALOG( m_pDLGUITrainingSchool );
	SAFE_DELETE_DIALOG( m_pDLGSchoolBack );

	if( NULL != m_pUINPC )
	{
		g_pKTDXApp->GetDGManager()->RemoveObjectChain( m_pUINPC );
		m_pUINPC->SetShowObject( false );
	}

	
	SAFE_DELETE( m_pDlgNpcMessage );
}

HRESULT CX2StateTrainingSchool::OnFrameMove( double fTime, float fElapsedTime )
{
	CX2StateMenu::OnFrameMove( fTime, fElapsedTime );

	if(m_pDlgNpcMessage != NULL)
		m_pDlgNpcMessage->OnFrameMove( fTime, fElapsedTime );
	if( NULL != m_pUINPC )
	{
		m_pUINPC->OnFrameMove( fTime, fElapsedTime );
	}

	return S_OK;
}

bool CX2StateTrainingSchool::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( uMsg )
	{
	case WM_MOUSEWHEEL:
		{ 
			return OnMouseWheel(hWnd, uMsg, wParam,lParam);
		}break;
	default:
		break;
	}


	return CX2StateMenu::MsgProc(hWnd, uMsg, wParam, lParam);
}

bool CX2StateTrainingSchool::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if( CX2StateMenu::UICustomEventProc( hWnd, uMsg, wParam, lParam ) == true )
		return true;

	if(m_pDlgNpcMessage->UICustomEventProc(hWnd, uMsg, wParam, lParam) == 1)
	{
		Handler_EGS_STATE_CHANGE_FIELD_REQ();
		return true;
	}

	switch(wParam)
	{
	case TSUCM_EXIT:
		{
			StateChangeToVillage();			
			return true;

		}break;
	case TSUCM_SELECT_TRAIN:
		{
			CKTDGUIRadioButton* pRadioButton = (CKTDGUIRadioButton*)lParam;

			int SlotNum = pRadioButton->GetDummyInt(0);
			if(SlotNum < (int)m_vecTrainingListSlot.size())
			{
				TrainingListSlot TrainList = m_vecTrainingListSlot[SlotNum];
				SelectTrain( TrainList.m_iTrainingID );
			}
		}break;
#ifdef CATEGORY_FOR_TRAINING
	case TSUCM_CATEGORY_BUTTON:
		{

		}break;
#endif
	case TSUCM_LIST_UP:
		{
			m_iTopIndex--;
			UpdateTrainUI();

		}break;
	case TSUCM_LIST_DOWN:
		{
			m_iTopIndex++;
			UpdateTrainUI();

		}break;
	case TSUCM_LIST_NAVI_LCLICKED:
	case TSUCM_LIST_NAVI_DRAGGING:
		{
			CKTDGUINavigation* pNavi	= (CKTDGUINavigation*)lParam;
			D3DXVECTOR2 vRelativePos = pNavi->GetRelativeWindowPos();

			// 0.674 : Navi Size에 따른 값. MAGIC VALUE
			m_iTopIndex = (int)((vRelativePos.y / 0.674f) * (m_iTrainListSize - m_iTrainPerPage));

			UpdateTrainUI( false );

		}break;
	case TSUCM_START_TRAIN:
		{
			return Handler_EGS_CREATE_TC_ROOM_REQ( m_iPickedTrainID );
		}break;
	default:
		break;
	}
	return false;
}




bool CX2StateTrainingSchool::UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if( CX2StateMenu::UIServerEventProc( hWnd, uMsg, wParam, lParam ) == true )
		return true;


	switch( wParam )
	{
	//case EGS_STATE_CHANGE_VILLAGE_MAP_ACK:
	//	{
	//		return Handler_EGS_STATE_CHANGE_VILLAGE_MAP_ACK( hWnd, uMsg, wParam, lParam );
	//	} break;

	case EGS_CREATE_TC_ROOM_ACK:
		{
			return Handler_EGS_CREATE_TC_ROOM_ACK( hWnd, uMsg, wParam, lParam );
		} break;
	}

	return false;
}

//bool CX2StateTrainingSchool::ClickMenuBackButton()
//{
//	switch( m_eSchoolMode )
//	{
//	case TSM_INTRO:
//		{
//			StateChangeToVillage();
//		} break;
//
//	default:
//		{
//			ChangeSchoolMode( TSM_INTRO );
//		} break;
//	}
//
//	return true;
//}


bool CX2StateTrainingSchool::LastMsgByESC()
{
	OpenLastMsgPopUp( GET_STRING( STR_ID_742 ) );
	return true;
}

void CX2StateTrainingSchool::LastMsgPopOk()
{
	StateChangeToVillage();
}


void CX2StateTrainingSchool::SetShowStateDLG( bool bShow )
{
	if( NULL != m_pDLGSchoolBack )
	{
		m_pDLGSchoolBack->SetShow( bShow );
		m_pDLGSchoolBack->SetEnable( bShow );
	}

	if( NULL != m_pUINPC )
	{
		m_pUINPC->SetShowObject( bShow );
	}

	if ( NULL != m_pDLGUITrainingSchool )
	{
		m_pDLGUITrainingSchool->SetShowEnable( bShow, bShow );
	}

	CX2GageManager::GetInstance()->SetShow( bShow );
}


bool CX2StateTrainingSchool::Handler_EGS_STATE_CHANGE_VILLAGE_MAP_REQ( UINT nMapID )
{
#if 0
	KStateChangeReq kPacket;
	kPacket.m_nMapID = nMapID;		

	SAFE_DELETE_DIALOG(m_pDLGMsgBox);
	g_pData->GetServerProtocol()->SendPacket( EGS_STATE_CHANGE_VILLAGE_MAP_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_STATE_CHANGE_VILLAGE_MAP_ACK );
	m_pDLGMsgBox = g_pMain->KTDGUIMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_743 ), this );
#endif
	return true;
}

bool CX2StateTrainingSchool::Handler_EGS_STATE_CHANGE_VILLAGE_MAP_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{	
#if 0 
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KStateChangeAck kEvent;
	DeSerialize( pBuff, &kEvent );
	CX2PacketLog::PrintLog( &kEvent );

	SAFE_DELETE_DIALOG( m_pDLGMsgBox );
	if( g_pMain->DeleteServerPacket( EGS_STATE_CHANGE_VILLAGE_MAP_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			g_pMain->CreateStateChangeDLG( GET_STRING( STR_ID_541 ) );
			g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_VILLAGE_MAP, kEvent.m_nMapID, false );
			return true;
		}
	}

#endif
	return false;
}

bool CX2StateTrainingSchool::Handler_EGS_CREATE_TC_ROOM_REQ( int iTrainingID )
{
	if( -1 != iTrainingID &&
		true == g_pData->GetTrainingCenterTable()->CheckIfEnter( g_pData->GetMyUser()->GetSelectUnit()->GetMapTCClear(), iTrainingID ) )
	{
		KEGS_CREATE_TC_ROOM_REQ kPacket;
		kPacket.m_iTCID = iTrainingID;

		g_pData->GetServerProtocol()->SendPacket( EGS_CREATE_TC_ROOM_REQ, kPacket );
		g_pMain->AddServerPacket( EGS_CREATE_TC_ROOM_ACK, 40.f );

		return true;

	}
	return false;
}



bool CX2StateTrainingSchool::Handler_EGS_CREATE_TC_ROOM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CREATE_TC_ROOM_ACK kEvent;
	DeSerialize( pBuff, &kEvent );	

	
	if( g_pMain->DeleteServerPacket( EGS_CREATE_TC_ROOM_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			//g_pMain->SetConnectedChannelID( kEvent.m_kTCInfo.m_uiChannelID );			// fix!!

			g_pMain->SetNowTrainingID( kEvent.m_kTCInfo.m_iTCID );

			KRoomInfo roomInfo;
			roomInfo.m_RoomType			= (CX2Room::ROOM_TYPE) CX2Room::RT_TRAININGCENTER;
			roomInfo.m_RoomUID			= kEvent.m_kTCInfo.m_RoomUID;
			roomInfo.m_wstrUDPRelayIP	= kEvent.m_kTCInfo.m_wstrUDPRelayIP;
			roomInfo.m_usUDPRelayPort	= kEvent.m_kTCInfo.m_usUDPRelayPort;
			//roomInfo.m_uiChannelID		= kEvent.m_kTCInfo.m_uiChannelID;			// fix!!
			roomInfo.m_RoomName			= L"TRAINING";
			roomInfo.m_RoomState		= (CX2Room::ROOM_STATE) CX2Room::RS_INIT;
			roomInfo.m_bPublic			= false;
			roomInfo.m_DifficultyLevel	= (CX2Dungeon::DIFFICULTY_LEVEL) CX2Dungeon::DL_NORMAL;
			roomInfo.m_iDungeonID		= (SEnum::DUNGEON_ID)kEvent.m_kTCInfo.m_iDungeonID;


			CX2TrainingCenterTable::TC_TABLE_INFO trainingInfo;
			if( true == g_pData->GetTrainingCenterTable()->GetTrainingInfo( trainingInfo, kEvent.m_kTCInfo.m_iTCID ) )
			{
				roomInfo.m_fPlayTime = trainingInfo.m_fPlayTime;
			}
			else
			{
				roomInfo.m_fPlayTime = 0.f;
			}
		
			
			KRoomSlotInfo roomSlotInfo;
			roomSlotInfo.m_bHost										= true;
			roomSlotInfo.m_bReady										= true;
			roomSlotInfo.m_Index										= 0;
			roomSlotInfo.m_SlotState									= CX2Room::SS_LOADING;
			roomSlotInfo.m_TeamNum										= (int) CX2Room::TN_RED;
			
			roomSlotInfo.m_kRoomUserInfo								= kEvent.m_kRoomUserInfo;
			//roomSlotInfo.m_kRoomUserInfo.m_iGSUID						= -1; // warning!!
			roomSlotInfo.m_kRoomUserInfo.m_nUnitUID						= g_pData->GetMyUser()->GetSelectUnit()->GetUID();
			roomSlotInfo.m_kRoomUserInfo.m_iOwnerUserUID				= g_pData->GetMyUser()->GetUID();

			//CX2Stat::Stat unitStat = g_pData->GetMyUser()->GetSelectUnit()->GetUnitStat();

			vector<KRoomSlotInfo> vecRoomSlotInfo;
			vecRoomSlotInfo.push_back( roomSlotInfo );

#ifdef HEAP_BROKEN_BY_ROOM
			CX2Room::InitializeRoomPacketData();
			CX2Room::SetRoomPacketData( roomInfo, vecRoomSlotInfo, 
				vector<KRoomSlotInfo>(), wstring( L"" ) );
			
			g_pData->DeleteAllRooms();
			g_pData->ResetDungeonRoom();
			if ( NULL != g_pX2Room && CX2Room::IsNewDataSet() )		/// 새로운 데이터가 셋팅 되었으면
				g_pX2Room->ApplyRoomPacketData();
#else	HEAP_BROKEN_BY_ROOM
			//방 데이터 제작
			CX2DungeonRoom* pCX2DungeonRoom = g_pData->ResetDungeonRoom();
			pCX2DungeonRoom->Set_KRoomInfo( roomInfo );
			pCX2DungeonRoom->Set_KRoomSlotInfoList( vecRoomSlotInfo );
			pCX2DungeonRoom->ConnectRelayServer( kEvent.m_kTCInfo.m_wstrUDPRelayIP.c_str(), kEvent.m_kTCInfo.m_usUDPRelayPort );

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
            if ( g_pData != NULL && g_pData->GetGameUDP() != NULL && g_pMain != NULL )
                g_pData->GetGameUDP()->SetForceConnectMode( g_pMain->GetUDPMode( CX2Game::GT_DUNGEON ) );
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

#endif // HEAP_BROKEN_BY_ROOM


			// 게임 바로 시작
			g_pMain->CreateStateChangeDLG( GET_STRING( STR_ID_744 ) );
			g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_TRAINING_GAME, NULL, false );


			const int MAGIC_FREE_TRAINING_ID = 1;
			if( trainingInfo.m_iID != MAGIC_FREE_TRAINING_ID )
			{
				g_pMain->SetLastStageWasTrainingGame( true );
				g_pMain->SetIsPlayingFreeTraining( false );
			}
			else
			{
				g_pMain->SetLastStageWasTrainingGame( false );
				g_pMain->SetIsPlayingFreeTraining( true );
			}
			
			return true;
		}
	}

	return false;
}


// 말 그대로 리셋
void CX2StateTrainingSchool::ResetTrainUI()
{
	// 	// 보상 EXP/ED/SP String 0으로 만들어 주고
	// 	CKTDGUIStatic* pStaticED = (CKTDGUIStatic*) m_pDLGUITrainingSchool->GetControl( L"Static_ED" );
	// 	pStaticED->GetString(0)->msg = L"";
	// 	CKTDGUIStatic* pStaticExp = (CKTDGUIStatic*) m_pDLGUITrainingSchool->GetControl( L"Static_Exp" );
	// 	pStaticExp->GetString(0)->msg = L"";
	// 	CKTDGUIStatic* pStaticSP = (CKTDGUIStatic*) m_pDLGUITrainingSchool->GetControl( L"Static_SP" );
	// 	pStaticSP->GetString(0)->msg = L"";

	// 슬롯 다 안 보이게 만들고
// 	for(UINT i=0; i<m_SlotList.size(); i++)
// 	{
// 		CX2SlotItem* pSlot = (CX2SlotItem*) m_SlotList[i];
// 		pSlot->SetShow(false);
// 	}
	// 	
	// 		// 1. 일단 다 지운다
	// 		for(UINT i=0; i<m_vQuestList.size(); i++)
	// 		{
	// 			QuestListSlot* pQuestList = m_vQuestList[i];
	// 			SAFE_DELETE(pQuestList);
	// 		}
	// 		m_vQuestList.clear();
	// 		//2. 새로 만든-_-다
	// 		WCHAR buff[256] = {0,};
	// 		for(int i=0; i<m_iQuestPerPage; i++)
	// 		{
	// 			QuestListSlot* pQuestList = new QuestListSlot;
	// 	
	// 			// 버튼 세팅해줄것
	// 			//wsprintf( buff, L"RadioButton_List%d", i);
	//			StringCchPrintf( buff, 256, L"RadioButton_List%d", i);
	// 			pQuestList->m_pButton = (CKTDGUIRadioButton*) m_pDLGUIQuestReceive->GetControl( buff );
	// 			pQuestList->m_pButton->SetShow(false);
	// 			// 스태틱 세팅해줄것
	// 			//wsprintf( buff, L"Static_QuestList%d", i);
	//			StringCchPrintf( buff, 256, L"RadioButton_List%d", i);
	// 			pQuestList->m_pStatic = (CKTDGUIStatic*) m_pDLGUIQuestReceive->GetControl( buff );
	// 			pQuestList->m_pStatic->SetShow(false);
	// 	
	// 			pQuestList->m_QuestID = -1;
	// 			m_vQuestList.push_back(pQuestList);
	// 		}

	m_iTopIndex = 0;
	m_iPickedTrainID = -1;
	// 리스트 받아오고
	//UpdateTrainList();

	// 선택된 상태를 취소해주고
	UncheckAllTrainList();

	// UI를 리셋해주면 끗
	UpdateTrainUI();

}

void CX2StateTrainingSchool::UpdateTrainDesc( int iTrainingID )
{
	if( NULL == m_pDLGUITrainingSchool )
		return;

	CKTDGUIStatic* pStatic_EXP = (CKTDGUIStatic*) m_pDLGUITrainingSchool->GetControl( L"Static_Exp" );
	CKTDGUIStatic* pStatic_ED = (CKTDGUIStatic*) m_pDLGUITrainingSchool->GetControl( L"Static_ED" );
	//CKTDGUIStatic* pStatic_SP = (CKTDGUIStatic*) m_pDLGUITrainingSchool->GetControl( L"Static_SP" );
	CKTDGUIStatic* pStatic_Difficulty_Star = (CKTDGUIStatic*) m_pDLGUITrainingSchool->GetControl( L"Static_DifficultyStar" );
	CKTDGUIStatic* pStatic_Title = (CKTDGUIStatic*) m_pDLGUITrainingSchool->GetControl( L"Static_Title" );
	CKTDGUIStatic* pStatic_Desc = (CKTDGUIStatic*) m_pDLGUITrainingSchool->GetControl( L"Static_Desc" );
	CKTDGUIStatic* pStatic_Image = (CKTDGUIStatic*) m_pDLGUITrainingSchool->GetControl( L"Static_Image" );
	CKTDGUIStatic* pStatic_Difficulty = (CKTDGUIStatic*) m_pDLGUITrainingSchool->GetControl( L"Static_Difficulty" );

	pStatic_EXP->GetString(0)->msg = L"";
	pStatic_ED->GetString(0)->msg = L"";
	//pStatic_SP->GetString(0)->msg = L"";
	pStatic_Difficulty_Star->GetString(0)->msg = L"";
	pStatic_Title->GetString(0)->msg = L"";
	pStatic_Desc->GetString(0)->msg = L"";
	pStatic_Image->SetShow(false);
	pStatic_Difficulty->SetShow(false);

	WCHAR wszText[64] = {0,};

	CX2TrainingCenterTable::TC_TABLE_INFO trainingInfo;
	if( true == g_pData->GetTrainingCenterTable()->GetTrainingInfo( trainingInfo, iTrainingID ) )
	{
		pStatic_Title->GetString(0)->msg = trainingInfo.m_wstrTitle;
		pStatic_Desc->GetString(0)->msg = trainingInfo.m_wstrDesc;
		pStatic_Image->GetPicture(0)->SetTex( trainingInfo.m_wstrDescTextureName.c_str(), trainingInfo.m_wstrDescTexturePieceName.c_str() );
		pStatic_Image->SetShow(true);

		//wsprintf( wszText, L"%d", trainingInfo.m_iRewardEXP );
		StringCchPrintf( wszText, 64, L"%d", trainingInfo.m_iRewardEXP );
		pStatic_EXP->GetString(0)->msg = wszText;

		//wsprintf( wszText, L"%d", trainingInfo.m_iRewardED );
		StringCchPrintf( wszText, 64, L"%d", trainingInfo.m_iRewardED );
		pStatic_ED->GetString(0)->msg = wszText;

		pStatic_Difficulty->SetShow(true);
		switch( trainingInfo.m_iDifficulty )
		{
		case 0:
		case 1:
			{
				pStatic_Difficulty_Star->GetString(0)->msg = L"★"; 
			} break;
		case 2:
			{
				pStatic_Difficulty_Star->GetString(0)->msg = L"★★";
			} break;

		case 3:
			{
				pStatic_Difficulty_Star->GetString(0)->msg = L"★★★";
			} break;
		case 4:
			{
				pStatic_Difficulty_Star->GetString(0)->msg = L"★★★★";
			} break;

		case 5:
			{
				pStatic_Difficulty_Star->GetString(0)->msg = L"★★★★★";
			} break;
		default:
			{
				// 이걸로 5가 넘어도 안심 =3=
				//wsprintf( wszText, L"★%d", trainingInfo.m_iDifficulty );
				StringCchPrintf( wszText, 64, L"★%d", trainingInfo.m_iDifficulty );
				pStatic_Difficulty_Star->GetString(0)->msg = wszText;
			} break;
		}
	}
}


void CX2StateTrainingSchool::UpdateTrainUI( bool bUpdateNavi )
{
	// call 되는 시점에서 모든 훈련 리스트를 다 받았다고 생각
	// 이후에 카데고리 추가할 때는 UpdateTrainList에서 받고 여기서 UI에 표시하는 식으로 한다

	if( NULL == m_pDLGUITrainingSchool )
		return;

	CX2Unit::UNIT_CLASS eUnitClass = g_pData->GetMyUser()->GetSelectUnit()->GetClass() ;
	vector<int>& vecEasyTrainingList = g_pData->GetTrainingCenterTable()->GetTrainingListByUnitClass( eUnitClass );

	m_iTrainListSize = (int)vecEasyTrainingList.size();

	if(m_iTopIndex > m_iTrainListSize - m_iTrainPerPage)
		m_iTopIndex = m_iTrainListSize - m_iTrainPerPage;

	if(m_iTopIndex < 0)
		m_iTopIndex = 0;

	m_vecTrainingListSlot.clear();
	for(int i=0; i<m_iTrainPerPage; i++)
	{

		WCHAR wszListName[64] = L"";
		swprintf( wszListName, sizeof(wszListName)/sizeof(WCHAR)-1, L"RadioButton_List%d", i );
		CKTDGUIRadioButton* pRadioButton_List = (CKTDGUIRadioButton*) m_pDLGUITrainingSchool->GetControl( wszListName );

		swprintf( wszListName, sizeof(wszListName)/sizeof(WCHAR)-1, L"Static_List%d", i );
		CKTDGUIStatic* pStatic_List = (CKTDGUIStatic*) m_pDLGUITrainingSchool->GetControl( wszListName );

		// 일단 꺼주고
		pRadioButton_List->SetShowEnable( false, false );
		pStatic_List->SetShowEnable(false, false);

		if(m_iTopIndex + i < m_iTrainListSize)
		{
			// ID 받아오고

			int iTrainingID = vecEasyTrainingList[m_iTopIndex+i];
			TrainingListSlot TrainSlot;
			TrainSlot.m_iTrainingID = iTrainingID;

			m_vecTrainingListSlot.push_back(TrainSlot);
			if(m_iPickedTrainID == iTrainingID && m_iPickedTrainID != -1)
			{
				pRadioButton_List->SetChecked(true);
			}
			else
			{
				pRadioButton_List->SetChecked(false);
			}

			// 제대로 된 훈련인지 확인해서
			CX2TrainingCenterTable::TC_TABLE_INFO trainingInfo;
			if( true == g_pData->GetTrainingCenterTable()->GetTrainingInfo( trainingInfo, iTrainingID ) )
			{
				// 여기서 버튼 설정해 주자~

				// 켜고
				pRadioButton_List->SetShowEnable( true, true );
				pStatic_List->SetShowEnable(true, true);
				pStatic_List->GetPicture(0)->SetShow(false);

				// 플레이 가능한 훈련인가 보고 (false면 물음표)
				bool bEnabledTraining = false;
				bEnabledTraining = g_pData->GetTrainingCenterTable()->CheckIfEnter( g_pData->GetMyUser()->GetSelectUnit()->GetMapTCClear(), trainingInfo.m_iID );

				if(bEnabledTraining)
				{
					// 표시되는 훈련
					// 깬 훈련인가 보고
					bool bClearedTraining = false;
					std::map< int, KTCClearInfo >::iterator it;
					it = g_pData->GetMyUser()->GetSelectUnit()->GetMapTCClear().find( trainingInfo.m_iID );
					if( it != g_pData->GetMyUser()->GetSelectUnit()->GetMapTCClear().end() )
					{
						// 클리어했다는 표시를 해 주자
						pStatic_List->GetPicture(0)->SetShow(true);
					}

					wstringstream wstrm;
					wstrm  << trainingInfo.m_wstrTitle << " " << trainingInfo.m_iTrainingNumber;
					// 제목 쓰고~
					pStatic_List->GetString(0)->msg = wstrm.str().c_str();

					pRadioButton_List->SetEnable(true);
				}
				else
				{
					// ??
					pStatic_List->GetString(0)->msg = L"???";
					pRadioButton_List->SetEnable(false);
					

				}

			}			
		}

	}

	if(true == bUpdateNavi)
	{
		UpdateNavi();
	}


}


void CX2StateTrainingSchool::UncheckAllTrainList()
{
	m_iPickedTrainID = -1;
	UpdateTrainUI();
	UpdateTrainDesc(-1);
}

void CX2StateTrainingSchool::UpdateNavi()
{
	if(m_pDLGUITrainingSchool != NULL)
	{
		CKTDGUINavigation* pNavi = (CKTDGUINavigation*) m_pDLGUITrainingSchool->GetControl( L"Navi_TrainList" );
		if( NULL != pNavi )
		{
			if( m_iTrainListSize <= m_iTrainPerPage )
			{
				pNavi->SetShow(false);
				return;
			}

			pNavi->SetShow(true);

			// 위치 정해주고~
			D3DXVECTOR2 vPos;
			vPos.x = 0.0f;
			// 0.674 : Navi Size에 따른 값. MAGIC VALUE
			vPos.y = (float)m_iTopIndex / ((float)m_iTrainListSize - m_iTrainPerPage ) * 0.674f;

			pNavi->SetRelativeWindowPos( vPos );
		}
	}

}

void CX2StateTrainingSchool::SelectTrain( int SelectedTrainID )
{
	CX2TrainingCenterTable::TC_TABLE_INFO trainingInfo;
	if( true == g_pData->GetTrainingCenterTable()->GetTrainingInfo( trainingInfo, SelectedTrainID ) )
	{
		bool bEnabledTraining = false;
		bEnabledTraining = g_pData->GetTrainingCenterTable()->CheckIfEnter( g_pData->GetMyUser()->GetSelectUnit()->GetMapTCClear(), trainingInfo.m_iID );
		if(true == bEnabledTraining)
		{
			m_iPickedTrainID = SelectedTrainID;
			UpdateTrainDesc(m_iPickedTrainID);
			UpdateTrainUI();
		}

	}
}

void CX2StateTrainingSchool::StateChangeToVillage()
{
	//Handler_EGS_STATE_CHANGE_VILLAGE_MAP_REQ( SEnum::VMI_RUBEN );
	Handler_EGS_STATE_CHANGE_FIELD_REQ();
}

bool CX2StateTrainingSchool::OnMouseWheel( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	short zDelta = (short)HIWORD(wParam);
	m_SumDelta += zDelta;
	while (abs(m_SumDelta) >= WHEEL_DELTA)
	{
		if(m_SumDelta>0)
		{
			m_iTopIndex--;

			UpdateTrainUI();
			m_SumDelta -= WHEEL_DELTA;
		}
		else
		{
			m_iTopIndex++;
			UpdateTrainUI();
			m_SumDelta += WHEEL_DELTA;
		}
	}
	return true;
}