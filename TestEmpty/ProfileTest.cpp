#include "dxstdafx.h"
#include ".\profiletest.h"
//
//
//
//using namespace SANDBOX;
///********************************************************************
//	created:	2006/10/30
//	created:	30:10:2006   11:29
//	filename: 	d:\projectX2\X2Project\TestEmpty\ProfileTest.cpp
//	file path:	d:\projectX2\X2Project\TestEmpty
//	file base:	ProfileTest
//	file ext:	cpp
//	author:		dmlee
//	
//	purpose:	
//*********************************************************************/
//
//
////void CALLBACK OnMyTestDialogEvent0( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
////{
//////	OPENFILENAME ofn;        // common dialog box structure
//////	WCHAR wszFileName[512];  // path까지 포함한 파일 이름
////
////	CMyTest* pFileTool = (CMyTest*) pUserContext;
////
////	switch( nControlID )
////	{
////	case CMyTest::IDC_EDIT_FILE_EXTENSION:
////		switch( nEvent )
////		{
////		case EVENT_EDITBOX_STRING:
////			{
////				pFileTool->SetFileExtension( ((CDXUTEditBox*)pControl)->GetText() );				
////			} break;
////
////		case EVENT_EDITBOX_CHANGE:
////			{
////				pFileTool->SetFileExtension( ((CDXUTEditBox*)pControl)->GetText() );
////			} break;
////		} break;
////
////	case CMyTest::IDC_EDIT_PREFIX_OR_SUFFIX:
////		switch( nEvent )
////		{
////		case EVENT_EDITBOX_STRING:
////			{
////				pFileTool->SetPrefixOrSuffix( ((CDXUTEditBox*)pControl)->GetText() );				
////			} break;
////
////		case EVENT_EDITBOX_CHANGE:
////			{
////				pFileTool->SetPrefixOrSuffix( ((CDXUTEditBox*)pControl)->GetText() );
////			} break;
////		} break;
////
////	case CMyTest::IDC_BUTTON_ATTACH_OR_DETACH:
////		{
////			if( CMyTest::MODE_ATTACH == pFileTool->GetMode() )
////			{
////				pFileTool->SetMode( CMyTest::MODE_DETACH );
////				((CDXUTButton*)pControl)->SetText( L"DETACH" );
////			}				
////			else
////			{
////				pFileTool->SetMode( CMyTest::MODE_ATTACH );
////				((CDXUTButton*)pControl)->SetText( L"ATTACH" );
////			}				
////		} break;
////
////	case CMyTest::IDC_BUTTON_PREFIX_OR_SUFFIX:
////		{
////			if( CMyTest::STATE_PREFIX == pFileTool->GetState() )
////			{
////				pFileTool->SetState( CMyTest::STATE_SUFFIX );
////				((CDXUTButton*)pControl)->SetText( L"SUFFIX" );
////			}
////			else
////			{
////				pFileTool->SetState( CMyTest::STATE_PREFIX );			
////				((CDXUTButton*)pControl)->SetText( L"PREFIX" );
////			}				
////		} break;
////
////	case CMyTest::IDC_BUTTON_HELP:
////		{
////			wstring wstrHelp;
////			wstrHelp = L"somebody help me~ \n";
////			MessageBox( g_pKTDXApp->GetHWND(), wstrHelp.c_str(), L"HELP", MB_OK );
////		} break;
////
////	}
////}
////
////
//void CMyTest::FinalDialog()
//{
////	for( unsigned i=0; i<m_vecDialog.size(); i++ )
////	{
////		SAFE_DELETE( m_vecDialog[i] );
////	}
////	m_vecDialog.clear();
//}
//
//void CMyTest::InitDialog()
//{
////	// initialize UI
////	extern CDXUTDialogResourceManager g_DialogResourceManager;
////	CDXUTDialog* pDlg = NULL;
////
////	pDlg = new CDXUTDialog();
////	m_vecDialog.push_back( pDlg );
////	pDlg->Init( &g_DialogResourceManager );
////	pDlg->SetCallback( OnProfileTestDialogEvent0, this );
////	pDlg->SetSize( 100, 300 );
////	pDlg->SetFont(0, L"굴림", 12, 12);
////
////	int iX = 10;
////	int iY = 100;
////	int iDX = 100;	
////	CDXUTEditBox* pEditBox = NULL;
////	pDlg->AddStatic( IDC_STATIC_DUMMY, L"Prefix or Suffix", iX, iY, 100, 22);
////	iY += 25;
////	pDlg->AddEditBox( IDC_EDIT_PREFIX_OR_SUFFIX, L"", iX, iY, 100, 30, false, &pEditBox );
////	pEditBox->SetText( m_wstrPrefixOrSuffix.c_str() );
////	iY += 35;
////	pDlg->AddStatic( IDC_STATIC_DUMMY, L"File Extension", iX, iY, 100, 22);
////	iY += 25;
////	pDlg->AddEditBox( IDC_EDIT_FILE_EXTENSION, L"", iX, iY, 100, 30, false, &pEditBox );
////	pEditBox->SetText( m_wstrFileExtension.c_str() );
////	iY += 35;
////	CDXUTButton* pButton = NULL;
////	pDlg->AddButton( IDC_BUTTON_PREFIX_OR_SUFFIX, L"", iX, iY, 100, 22, 0, false, &pButton);
////	{
////		if( STATE_PREFIX == m_iState )
////			pButton->SetText( L"PREFIX" );
////		else
////			pButton->SetText( L"SUFFIX" );
////	}
////	iY += 25;
////
////	pDlg->AddButton( IDC_BUTTON_ATTACH_OR_DETACH, L"", iX, iY, 100, 22, 0, false, &pButton);
////	{
////		if( MODE_ATTACH == m_iMode )
////			pButton->SetText( L"ATTACH" );
////		else
////			pButton->SetText( L"DETACH" );
////	}	
////	iY += 25;
////	pDlg->AddButton( IDC_BUTTON_HELP,      L"HELP", iX, iY, 100, 22);
//}
//
//
//CMyTest::~CMyTest(void)
//{
//	FinalDialog();
////	if( NULL != m_pDungeonGame )
////		m_pDungeonGame->GameEnd();
////	SAFE_DELETE( m_pDungeonGame );
//}
//
//CMyTest::CMyTest(void)
//{
//	InitDialog();
////	m_pDungeonGame = NULL;
//
//	//UI 파티클 시스템 생성
//	g_pData->ResetUIMajorParticle( L"UIMajorParticle.txt" );
//	g_pData->ResetUIMinorParticle( L"UIMinorParticle.txt" );
//
//	//UI 메시플레이어
//	g_pData->ResetUIMajorXMeshPlayer( L"UIMajorXMeshPlayer.txt" );
//	g_pData->ResetUIMinorXMeshPlayer( L"UIMinorXMeshPlayer.txt" );
//
//	//유닛 데이타 로드
//	g_pData->ResetUnitManager( L"UnitTemplet.lua", L"NPCTemplet.lua" );
//
//	//아이템 데이타 로드
//	g_pData->ResetItemManager( L"Item.lua" );
//
//	//월드 데이타 로드
//	g_pData->ResetWorldManager( L"WorldData.lua" );
//
//	g_pData->ResetDungeonManager( L"DungeonData.lua" );
//
//	//g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_PVP_GAME, NULL, false );
//
//	//
//	//g_pKTDXApp->RemoveStage( m_pNowState );	
//	//m_pNowState = new CX2StateStartUp();
//	//SAFE_DELETE( m_pNowState );
//	//m_pNowState = new CX2StatePVPGame();	
//	//g_pKTDXApp->InsertStage( m_pNowState );	
//
//	//g_pKTDXApp->SendGameMessage()
////	StateChange( CX2Main::XS_START_UP, 0 );
////	StateChange( CX2Main::XS_PVP_GAME, 0 );
//
//	CreateNewGame();
//}
//
//HRESULT CMyTest::OnLostDevice()
//{
//	//if( NULL != m_pDungeonGame )
//	//	m_pDungeonGame->OnLostDevice();
//
//	return g_pKTDXApp->GetDGManager()->GetDialogManager()->OnLostDevice();
//}
//
//
//HRESULT CMyTest::OnResetDevice()
//{
//	int iX = 100;
//	for( unsigned i=0; i<m_vecDialog.size(); i++ )
//	{
//		m_vecDialog[i]->SetLocation( iX, 10 );
//		iX += m_vecDialog[i]->GetWidth()+10;
//	}	
//
//	//if( NULL != m_pDungeonGame )
//	//	m_pDungeonGame->OnResetDevice();
//
//	//const WCHAR* pFileName = L"FSM_TEST.lua";
//
//	//KGCMassFileManager::CMassFile::MASSFILE_MEMBERFILEINFO_POINTER Info;
//	//Info = g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->LoadDataFile( pFileName );
//	//if( Info == NULL )
//	//	return false;
//
//	//if( g_pKTDXApp->GetLuaBinder()->DoMemory( Info->pRealData, Info->size ) == E_FAIL )
//	//	return false;
//
//	//bool bTableOpen = m_LuaManager.BeginTable( "AI_STATE" );
//	//{	
//	//			
//
//	//}
//	//if( bTableOpen == true )
//	//	m_LuaManager.EndTable();
//
//	return g_pKTDXApp->GetDGManager()->GetDialogManager()->OnResetDevice();
//}
//
//
//HRESULT CMyTest::OnFrameMove( double fTime, float fElapsedTime )
//{
//	if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_F1) == TRUE )
//	{		
//	}
//
//	//if( NULL != m_pDungeonGame )
//	//	m_pDungeonGame->OnFrameMove( fTime, fElapsedTime );
//
//	g_pKTDXApp->GetDGManager()->GetDialogManager()->OnFrameMove( fTime, fElapsedTime );
//
//	return S_OK;
//}
//
//HRESULT CMyTest::OnFrameRender()
//{
//	for( unsigned i=0; i<m_vecDialog.size(); i++ )
//	{
//		m_vecDialog[i]->OnRender( g_pKTDXApp->GetElapsedTime() );
//	}
//
//	//if( NULL != m_pDungeonGame )
//	//	m_pDungeonGame->OnFrameRender();
//
//	//g_pKTDXApp->GetDGManager()->ObjectChainSort();
//	//g_pKTDXApp->GetDGManager()->ObjectChainNonAlphaRender();
//	//g_pKTDXApp->GetDGManager()->ObjectChainAlphaRender();
//
//	return S_OK;
//}
//
//bool CMyTest::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
//{
//	for( unsigned i=0; i<m_vecDialog.size(); i++ )
//	{
//		bool bNoFurtherProcessing = m_vecDialog[i]->MsgProc( hWnd, uMsg, wParam, lParam );
//		if( bNoFurtherProcessing == true )
//			return true;
//	}
//
//	bool bNoFurtherProcessing = false;
//
//	//CX2Main::MsgProc( hWnd, uMsg, wParam, lParam );
//
//	//if( NULL != m_pDungeonGame )
//	//	bNoFurtherProcessing = m_pDungeonGame->MsgProc(hWnd, uMsg, wParam, lParam);
//
//	switch( uMsg )
//	{	
//	case WM_CHAR:
//		{
//			switch( wParam )
//			{
//			case 't':
//				{
//					//CX2DungeonManager dungeonManager;
//					//dungeonManager.OpenScriptFile( L"DungeonData.lua" );
//				} break;
//
//			} // end of switch( wParam )
//		}
//
//	//case WM_DROPFILES:
//	//	{
//	//		WCHAR wszFilePath[MAX_PATH] = L"";
//	//		DragQueryFile((HDROP) wParam, 0, wszFilePath, MAX_PATH);			
//	//	} break;
//	} // end of switch( uMsg )
//	
//	//return g_pKTDXApp->GetDGManager()->GetDialogManager()->MsgProc( hWnd, uMsg, wParam, lParam );
//
//	return bNoFurtherProcessing;
//}
//
//
//void CMyTest::CreateNewGame()
//{
//	const int MAGIC_USER_UID = 777;
//	const int MAGIC_UNIT_UID = 777;
//
//	const CX2Unit::UNIT_CLASS MAGIC_UNIT_CLASS = CX2Unit::UC_ELSWORD_SWORDMAN;
//	//const CX2Unit::UNIT_CLASS MAGIC_UNIT_CLASS = CX2Unit::UC_ARME_VIOLET_MAGE;	
//	//const CX2Unit::UNIT_CLASS MAGIC_UNIT_CLASS = CX2Unit::UC_LIRE_ELVEN_RANGER;
//
//	const int MAGIC_WORLD_ID = CX2World::WI_TREE_OF_LIGHT_TOP;
//
//	// user data setup
//	CX2User::UserData* pUserData = new CX2User::UserData;
//	{
//		pUserData->userUID		= MAGIC_USER_UID;
//		pUserData->userID		= L"WorldToolUserID";
//		pUserData->userNickName	= L"WorldToolUserNickName";
//		pUserData->maxUnitCount	= 1;	
//	}
//	g_pData->ResetMyUser( pUserData );
//
//	/*
//	CX2Unit::UnitData* pUnitData = new CX2Unit::UnitData();
//	{		
//		pUnitData->userUID		= g_pData->GetMyUser()->GetUID();
//		pUnitData->unitUID		= MAGIC_UNIT_UID;
//		pUnitData->unitClass	= MAGIC_UNIT_CLASS;
//		pUnitData->IP			= L"0.0.0.0"; // L"127.0.0.1";
//		pUnitData->nickName		= L"*^^*♡";
//		pUnitData->GP			= 0;
//		pUnitData->level		= 1;
//		pUnitData->EXP			= 0;
//	}
//	*/
//
//	KUnitInfo kUnitInfo;
//	kUnitInfo.m_nUnitUID			= MAGIC_UNIT_UID;
//	kUnitInfo.m_iUnitClass		= MAGIC_UNIT_CLASS;
//	//kUnitInfo.m_vecIP			= L"0,0,0,0"; // L"127.0.0.1";
//	kUnitInfo.m_wstrNickName	= L"*^^*♡";
//	kUnitInfo.m_iGP				= 0;
//	kUnitInfo.m_ucLevel			= 1;
//	kUnitInfo.m_iEXP			= 0;
//
//	CX2Unit* pUnit	= new CX2Unit( kUnitInfo );
//	g_pData->GetMyUser()->AddUnit( pUnit );
//
//	// room data setup
//	CX2PVPRoom::PVPRoomData* pRoomData = new CX2PVPRoom::PVPRoomData();
//	{
//		pRoomData->roomName				= L"WorldToolRoomName";
//		pRoomData->worldID				= (CX2World::WORLD_ID) MAGIC_WORLD_ID;
//		CX2Room::SlotData* pSlotData	= new CX2Room::SlotData();
//		pSlotData->slotState			= CX2Room::SS_JOIN_HOST;
//		pSlotData->pUnit				= g_pData->GetMyUser()->GetUnitByUID( MAGIC_UNIT_UID );
//		pRoomData->slotDataList.push_back( pSlotData );
//	}
//	g_pData->ResetPVPRoom( pRoomData );
//
////	g_pX2Game = NULL; 
////	SAFE_DELETE( m_pDungeonGame );
//
//	//m_pDungeonGame = new CX2DungeonGame();
//	//g_pX2Game = m_pDungeonGame;
//	//m_pDungeonGame->HostReady( g_pData->GetMyUser()->GetUID() );
//	//m_pDungeonGame->GameLoading( g_pData->GetPVPRoom() );
//	//m_pDungeonGame->GameStart();
//	//m_pDungeonGame->GetWorld()->StopBGM();
//	//g_pKTDXApp->SkipFrame();
//}
//
///*
//
//
//CAITest::~CAITest(void)
//{
//m_pGame->GameEnd();
//SAFE_DELETE(m_pGame);
//}
//
//CAITest::CAITest(void)
//{
//m_pGame			= NULL;
//
////UI 파티클 시스템 생성
//g_pData->ResetUIMajorParticle( L"UIMajorParticle.txt" );
//g_pData->ResetUIMinorParticle( L"UIMinorParticle.txt" );
//
////UI 메시플레이어
//g_pData->ResetUIMajorXMeshPlayer( L"UIMajorXMeshPlayer.txt" );
//g_pData->ResetUIMinorXMeshPlayer( L"UIMinorXMeshPlayer.txt" );
//
////유닛 데이타 로드
//g_pData->ResetUnitManager( L"UnitTemplet.lua", L"NPCTemplet.lua" );
//
////아이템 데이타 로드
//g_pData->ResetItemManager( L"Item.lua" );
//
////월드 데이타 로드
//g_pData->ResetWorldManager( L"WorldData.lua" );	
//
//CreateNewGame();	
//}
//
//HRESULT CAITest::OnLostDevice()
//{
//HRESULT hr = S_OK;
//if( NULL != m_pGame )
//hr = m_pGame->OnLostDevice();
//
//hr = g_pKTDXApp->GetDGManager()->GetDialogManager()->OnLostDevice();
//return hr;
//}
//
//HRESULT CAITest::OnResetDevice()
//{
//HRESULT hr = S_OK;
//if( NULL != m_pGame )
//hr = m_pGame->OnResetDevice();
//
//hr = g_pKTDXApp->GetDGManager()->GetDialogManager()->OnResetDevice();
//return hr;
//}
//
//HRESULT CAITest::OnFrameMove( double fTime, float fElapsedTime )
//{
//HRESULT hr = S_OK;
//if( NULL != m_pGame )
//hr = m_pGame->OnFrameMove( fTime, fElapsedTime );
//return hr;
//}
//
//HRESULT CAITest::OnFrameRender()
//{
//HRESULT hr = S_OK;
//if( NULL != m_pGame )
//{
//hr = m_pGame->OnFrameRender();
//
//if( NULL != m_pGame->GetWorld() &&
//NULL != m_pGame->GetWorld()->GetLineMap() )
//{
//m_pGame->GetWorld()->GetLineMap()->OnFrameRender();
//}
//}
//return hr;
//}
//
//bool CAITest::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
//{
//bool bNoFurtherProcessing = false;
//
//if( NULL != m_pGame )
//{	
//bNoFurtherProcessing = m_pGame->MsgProc(hWnd, uMsg, wParam, lParam);
//}
//
//return bNoFurtherProcessing;
//}
//
//
//*/
//
//
