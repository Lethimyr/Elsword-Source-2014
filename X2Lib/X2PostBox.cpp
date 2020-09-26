#include "StdAfx.h"
#include ".\x2postbox.h"

//#define FREE_FEE // 수수료 무료

CX2PostBox::CX2PostBox( CKTDXStage* pNowState )
: CX2UIDragable( pNowState, NULL )
//{{ 허상형 : [2010/8/25/] //	우편 수량 UI 개편
#ifdef POST_QUANTITY_NEW
, m_pDLGItemEnroll(NULL)
, m_EnrollItemUid( 0 )
#endif	//	POST_QUANTITY_NEW
//}} 허상형 : [2010/8/25/] //	우편 수량 UI 개편
{
    m_pDLGQuery = NULL;
    m_pProcessDlg = NULL;

    m_pNowState = (CX2State*) pNowState;


    RegisterLuaBind();
    
    m_pTalkBoxManager = CX2TalkBoxManager::CreateTalkBoxManger();
    g_pKTDXApp->GetDGManager()->AddObjectChain( m_pTalkBoxManager );
    m_pTalkBoxManager->SetOverUI( true );

    // Post Box
// 	m_pDlgPostBox = new CKTDGUIDialog( m_pNowState, L"DLG_UI_Mini_Post.lua" ); //L"DLG_Post_Box.lua" );
//     g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDlgPostBox );    
//     m_pDlgPostBox->SetShowEnable(false, false);

    // Send
    m_pDLGMailSend = new CKTDGUIDialog( m_pNowState, L"DLG_Mail_Shop_Popup_Send.lua" );
    g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGMailSend );     

#ifdef POSTBOX_NOTICE
	m_pDLGMailSend->SetDisableUnderWindow(true);
#endif POSTBOX_NOTICE

    m_pDLGMailSend->SetHasSlot(true);

    // Receive
    m_pDlgMailReceive = new CKTDGUIDialog( m_pNowState, L"DLG_Post_Window_Confirm.lua" );
    g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDlgMailReceive );         
	m_pDlgMailReceive->SetDisableUnderWindow(true);
    m_pDlgMailReceive->SetHasSlot(true);

    // Mail List
    m_pDlgMailListSlot = new CKTDGUIDialog( m_pNowState, L"DLG_Post_Window_Confirm_Slot.lua" );	
    g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDlgMailListSlot );      
	m_pDlgMailListSlot->SetDisableUnderWindow(true);

    // settingf
    m_pDlgMailSetting = new CKTDGUIDialog( m_pNowState, L"DLG_Post_Window_Config.lua" );
    g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDlgMailSetting );
    //m_pDlgMailSetting->SetPos(D3DXVECTOR2(624, 140));
	m_pDlgMailSetting->SetPos(D3DXVECTOR2(100, 140));
	
    // Mail View Common
    m_pDlgMailViewCommon = new CKTDGUIDialog( m_pNowState, L"DLG_Post_Letter_Window.lua" );
    g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDlgMailViewCommon );    
    //m_pDlgMailView->SetPos(D3DXVECTOR2(250, 144));
	m_pDlgMailViewCommon->SetPos(D3DXVECTOR2(524, 144));
	m_pDlgMailViewCommon->SetShowEnable(false, false);

	// System Mail View
	m_pDlgMailViewSystem = new CKTDGUIDialog( m_pNowState, L"DLG_Post_Letter_Window_System.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDlgMailViewSystem );    
	//m_pDlgMailView->SetPos(D3DXVECTOR2(250, 144));
	m_pDlgMailViewSystem->SetPos(D3DXVECTOR2(524, 144));
	m_pDlgMailViewSystem->SetShowEnable(false, false);

#ifdef SERV_RELATIONSHIP_SYSTEM
	// System Mail View
	m_pDlgMailViewWedding = new CKTDGUIDialog( m_pNowState, L"DLG_Post_InviteLetter_Window_System.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDlgMailViewWedding );    
	//m_pDlgMailView->SetPos(D3DXVECTOR2(250, 144));
	m_pDlgMailViewWedding->SetPos(D3DXVECTOR2(524, 144));
	m_pDlgMailViewWedding->SetShowEnable(false, false);
#endif // SERV_RELATIONSHIP_SYSTEM
	// Mail View
	m_pDlgMailView = m_pDlgMailViewCommon;

    m_pDLGAddBlackList = new CKTDGUIDialog( pNowState, L"DLG_Post_Popup_Intercept.lua" );
    g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGAddBlackList );
	m_pDLGAddBlackList->SetDisableUnderWindow(true);
    //m_pDLGAddBlackList->SetPos(D3DXVECTOR2(150, 100));
	m_pDLGAddBlackList->SetPos(D3DXVECTOR2(500, 200));
    m_pDLGAddBlackList->SetShowEnable( false, false );

    m_pDLGMyTradeWindow = new CKTDGUIDialog( pNowState, L"DLG_Quantity_Window.lua" );
    g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGMyTradeWindow );
    //m_pDLGMyTradeWindow->SetPos(D3DXVECTOR2(322, 297));
	m_pDLGMyTradeWindow->SetDisableUnderWindow(true);
	m_pDLGMyTradeWindow->SetPos(D3DXVECTOR2(522, 297));
    m_pDLGMyTradeWindow->SetShowEnable( false, false );

    ReadPostOption();

    SetSlotManagerLayer(XDL_NORMAL_4);

    InitPostBox();
	SetDialogShow(false);	
}

CX2PostBox::~CX2PostBox(void)
{
    m_pNowState = NULL;

    //SAFE_DELETE( m_pTalkBoxManager );
	CX2TalkBoxManager::DeleteKTDGObject( m_pTalkBoxManager );

    //SAFE_DELETE_DIALOG( m_pDlgPostBox );

    SAFE_DELETE_DIALOG( m_pDLGQuery );
    SAFE_DELETE_DIALOG( m_pProcessDlg );

    SAFE_DELETE_DIALOG( m_pDLGMailSend );
    SAFE_DELETE_DIALOG( m_pDlgMailReceive );
    SAFE_DELETE_DIALOG( m_pDlgMailListSlot );    
    SAFE_DELETE_DIALOG( m_pDlgMailSetting );
    SAFE_DELETE_DIALOG( m_pDlgMailViewCommon );
	SAFE_DELETE_DIALOG( m_pDlgMailViewSystem );	
#ifdef SERV_RELATIONSHIP_SYSTEM
	SAFE_DELETE_DIALOG( m_pDlgMailViewWedding );	
#endif // SERV_RELATIONSHIP_SYSTEM


    SAFE_DELETE_DIALOG( m_pDLGMyTradeWindow ); 
    SAFE_DELETE_DIALOG( m_pDLGAddBlackList );

	//{{ 허상형 : [2010/8/25/] //	우편 수량 UI 개편
#ifdef POST_QUANTITY_NEW
	SAFE_DELETE_DIALOG(m_pDLGItemEnroll);
#endif	//	POST_QUANTITY_NEW
	//}} 허상형 : [2010/8/25/] //	우편 수량 UI 개편
}

void CX2PostBox::RegisterLuaBind()
{
    lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(),  "g_pPostBoxInventory", this );
}

void CX2PostBox::InitPostBox()
{
    m_pDLGQuery = NULL;
    m_pProcessDlg = NULL;

    m_pPostSlot = NULL;    

    m_bMouseDown = false;
    m_bRegisteredTrade = false;
    m_nQuantity = 0;
    m_nED = 0;

#ifdef FREE_FEE
    m_nFee = 0;
#else
    m_nFee = 200;
#endif
    m_nTabIndex = -1;
    m_bProcess = false;
    m_bAttachItem = true;

    m_bPostBox = false;   


    m_NowInventorySortType = CX2Inventory::ST_EQUIP;
    m_NowInvenSortTypePageNum = 1;
    m_NowInvenSortTypePageMaxNum = 1;        


    m_nMaxPage      = 1;
    m_nCurrentPage  = 1;
    m_nMaxSlot      = 0;

    m_bSystemMail = true;
    m_bNewLetterisSystem = true;
    m_CurrentState = 0;

    m_bBuffBList = false;

    m_bAlarm = true;
    m_bNewLetterNotify = true;

    m_bCheckPitIn = false;   

    m_nUnReadDeleteMail = 0;
    m_nUnReadMail = 0;
    m_nTotalMail = 0;

    m_iNewLetter = 0;

    m_nUnReadCommon = 0;
    m_nUnReadSystem = 0;

    m_fBlinkTime = 0;
    m_nowColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

    m_bSwitch = false;
    m_bEditTitle = false;

	m_bReNewalLetter = false;
	m_bInit = false;

#ifdef SERV_RELATIONSHIP_SYSTEM
	m_nUnReadWedding = 0;
	m_cLetterListType = 0;
#endif // SERV_RELATIONSHIP_SYSTEM

#ifdef POST_SELECT_ALL_MAILS
	//m_bSelectedAll = false;
#endif POST_SELECT_ALL_MAILS

    CKTDGUIComboBox* pComboBox;

    //m_vecMyUserName.clear();
    if(m_pDLGMailSend != NULL && g_pData != NULL && g_pData->GetMyUser() != NULL)
    {
        pComboBox = (CKTDGUIComboBox*)m_pDLGMailSend->GetControl(L"_to");
        if(pComboBox != NULL)
        {
            pComboBox->RemoveAllItems();           
            
            for(int i=0; i<g_pData->GetMyUser()->GetUnitNum(); ++i)
            {
				wstring wstrOtherUserName = GetUserName(i);
                if( false == wstrOtherUserName.empty() &&
					g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_NickName != wstrOtherUserName )
				{
                    pComboBox->AddItem(GetUserName(i).c_str(), NULL);
				}
            }
        }        
    }
   

    // Init
    ClearMailList();
    ClearListSlot();
}

void CX2PostBox::ChangeAlarm()
{
    CKTDGUICheckBox *pDlgAlarm = (CKTDGUICheckBox*)m_pDlgMailSetting->GetControl(L"g_pCheckBoxpost_window_Slot2");
    if(pDlgAlarm)
        m_bAlarm = !pDlgAlarm->GetChecked();

    SavePostOption();
}

void CX2PostBox::ChangeNewNotify()
{
    CKTDGUICheckBox *pDlgNotify = (CKTDGUICheckBox*)m_pDlgMailSetting->GetControl(L"g_pCheckBoxpost_window_Slot3");
    if(pDlgNotify)
        m_bNewLetterNotify = !pDlgNotify->GetChecked();

    SavePostOption();
}

void CX2PostBox::ReadPostOption()
{
#if 0 // 우편함 옵션이 필요없으므로 옵션파일에서 읽지 않고 디폴트로 설정한다.
    const string strFileName = "PostOptions.cfg";

    FILE* file = NULL;
    file = fopen( strFileName.c_str(), "r" );		
    if( NULL == file )
    {
        ErrorLogMsg( XEM_ERROR7, strFileName.c_str() );
        m_bAlarm = true;
        m_bNewLetterNotify = true;
        return;
    }

    int iAlarm, iNotify;
    fscanf(file, "%d%d", &iAlarm, &iNotify);
    fclose(file);

    m_bAlarm = false;
    m_bNewLetterNotify = false;

    if(iAlarm > 0)
        m_bAlarm = true;
    if(iNotify > 0)
        m_bNewLetterNotify = true;	
#endif

	m_bAlarm = false;
	m_bNewLetterNotify = false;

    CKTDGUICheckBox *pDlgAlarm = (CKTDGUICheckBox*)m_pDlgMailSetting->GetControl(L"g_pCheckBoxpost_window_Slot2");
    if(pDlgAlarm)
        pDlgAlarm->SetChecked( !m_bAlarm );
    CKTDGUICheckBox *pDlgNotify = (CKTDGUICheckBox*)m_pDlgMailSetting->GetControl(L"g_pCheckBoxpost_window_Slot3");
    if(pDlgNotify)
        pDlgNotify->SetChecked( !m_bNewLetterNotify );
}

void CX2PostBox::SavePostOption()
{
#if 0 // 우편함 옵션파일 저장할 필요 없으므로 삭제
    const string strFileName = "PostOptions.cfg";


    FILE* file = NULL;
    file = fopen( strFileName.c_str(), "w" );		
    if( NULL == file )
    {
        ErrorLogMsg( XEM_ERROR7, strFileName.c_str() );
        return;
    }

    if(m_bAlarm)
        fwprintf( file, L"1 " );
    else
        fwprintf( file, L"0 " );

    if(m_bNewLetterNotify)
        fwprintf( file, L"1" );
    else
        fwprintf( file, L"0" );

    fclose(file);
#endif
}

void CX2PostBox::SetNowState( CX2State* pState )
{
    m_pNowState = pState;

    if( m_pDLGMailSend != NULL )
    {
        m_pDLGMailSend->SetStage( (CKTDXStage*) pState );
    }    

    //if( m_pDLGInventory != NULL )
    //{
    //    m_pDLGInventory->SetStage((CKTDXStage*) pState);
    //}
}

HRESULT CX2PostBox::OnFrameMove(double fTime, float fElapsedTime)
{
    // 보관된 새로온 편지 뿌리기
    if(m_pDlgMailReceive->GetShow() == true)
    {
        CKTDGUIButton* pSystem	= (CKTDGUIButton*) m_pDlgMailReceive->GetControl( L"system_mail" );
        CKTDGUIButton* pCommon	= (CKTDGUIButton*) m_pDlgMailReceive->GetControl( L"common_mail" );

#ifdef SERV_RELATIONSHIP_SYSTEM
		CKTDGUIButton* pWedding	= (CKTDGUIButton*) m_pDlgMailReceive->GetControl( L"wedding_mail" );
#endif // SERV_RELATIONSHIP_SYSTEM

        if(m_fBlinkTime >= 0.0 && m_fBlinkTime <= 1.0)
        {
            m_nowColor.g -= 1 * fElapsedTime;
            m_nowColor.b -= 1 * fElapsedTime;
            m_fBlinkTime += fElapsedTime;
            if(m_nowColor.g < 0.f)
                m_nowColor.g = 0.f;
            if(m_nowColor.b < 0.f)
                m_nowColor.b = 0.f;            
        }
        else if(m_fBlinkTime > 1.0 && m_fBlinkTime <= 2.0)
        {
            m_nowColor.g += 1 * fElapsedTime;
            m_nowColor.b += 1 * fElapsedTime;
            m_fBlinkTime += fElapsedTime;
            if(m_nowColor.g > 1.f)
                m_nowColor.g = 1.f;
            if(m_nowColor.b > 1.f)
                m_nowColor.b = 1.f;
        }
        else
        {
            m_fBlinkTime = 0.0f;
        }               

        if(m_nUnReadCommon > 0)
        {            
            pCommon->SetColor(m_nowColor);
        }
        else
        {
            pCommon->SetColor(D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
        }

        if(m_nUnReadSystem > 0)
        {
            pSystem->SetColor(m_nowColor);
        }
        else
        {
            pSystem->SetColor(D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
        }
#ifdef SERV_RELATIONSHIP_SYSTEM
		if ( m_nUnReadWedding > 0 )
		{
			pWedding->SetColor ( m_nowColor );
		}
		else
		{
			pWedding->SetColor(D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
		}
#endif // SERV_RELATIONSHIP_SYSTEM
	}
    else
    {
        m_fBlinkTime = 0.0f;
        m_nowColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
    }

	//{{ 김상훈 : 2010.10.06 전체선택 체크박스
#ifdef POST_SELECT_ALL_MAILS 

	CKTDGUICheckBox *pCheckDlg;
	WCHAR t_ControlName[128];

	pCheckDlg   = (CKTDGUICheckBox*)m_pDlgMailListSlot->GetControl(L"g_pCheckBoxpost_window_Slot1");  
	if ( pCheckDlg->GetEnable() == true )  // 편지가 하나라도 있을 때      
	{
		pCheckDlg   = (CKTDGUICheckBox*)m_pDlgMailListSlot->GetControl(L"g_pCheckBoxpost_window_Slot7");   // 전체선택 체크박스
		if ( pCheckDlg->GetChecked() != m_bSelectedAll )  // 전체선택 체크 박스를 클릭 한 경우.
		{
			for (int i=0;i<6;++i)
			{
				swprintf( t_ControlName, L"g_pCheckBoxpost_window_Slot%d" , i+1 );
				pCheckDlg = (CKTDGUICheckBox*)m_pDlgMailListSlot->GetControl( t_ControlName );        // 체크박스

				if ( pCheckDlg->GetEnable() == true )
				{
					pCheckDlg->SetChecked( !m_bSelectedAll , false );
					m_listSlot[i].m_bChecked = !m_bSelectedAll;
				}
			}
			m_bSelectedAll = !m_bSelectedAll;
		}
		else
		{ // 직접 편지 체크박스를 온/오프 해서 전체선택 체크박스가 바뀌게 되는 경우 처리 
			bool bIsChecked = true;
			for (int i=0;i<6;++i)  
			{
				swprintf( t_ControlName, L"g_pCheckBoxpost_window_Slot%d" , i+1 );
				pCheckDlg = (CKTDGUICheckBox*)m_pDlgMailListSlot->GetControl( t_ControlName );        // 체크박스

				if ( pCheckDlg->GetEnable() == true && pCheckDlg->GetChecked() == false )  // 체크 되지 않은 편지가 있는 경우
				{
					bIsChecked = false;
					break; 
				}
			}
			m_bSelectedAll = bIsChecked;
			pCheckDlg   = (CKTDGUICheckBox*)m_pDlgMailListSlot->GetControl(L"g_pCheckBoxpost_window_Slot7");  	
			pCheckDlg->SetChecked( m_bSelectedAll , false );
		}
	}
	else  // 편지가 하나도 없으면 전체선택 체크박스는 Disable 해야 함.
	{
		pCheckDlg   = (CKTDGUICheckBox*)m_pDlgMailListSlot->GetControl(L"g_pCheckBoxpost_window_Slot7");   // 전체선택 체크박스
		pCheckDlg->SetEnable( false );
		pCheckDlg->SetChecked( false, false );
	}
#endif POST_SELECT_ALL_MAILS 
	//}} 김상훈 : 2010.10.06 전체선택 체크박스

    if( false == GetEnable() )
        return S_OK;

	if( (m_pDLGMailSend != NULL && m_pDLGMailSend->GetIsOverControl() == true) ||
		(m_pDlgMailReceive != NULL && m_pDlgMailReceive->GetIsOverControl() == true) ||
		(m_pDlgMailListSlot != NULL && m_pDlgMailListSlot->GetIsOverControl() == true) ||
		(m_pDlgMailSetting != NULL && m_pDlgMailSetting->GetIsOverControl() == true) ||
		(m_pDlgMailView != NULL && m_pDlgMailView->GetIsOverControl() == true) )
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

// void CX2PostBox::ClearAlarmNewLetter(double fTime, float fElapsedTime)
// {
//     // 알림창 시간되면 끄기
//     if(m_pDlgPostBox != NULL && m_bPostBox == true)
//     {           
//         if(m_TimerPostBox.elapsed() > 6.0f)   
//         {
//             m_pDlgPostBox->SetShowEnable(false, false);
//             m_bPostBox = false;
// 
//             m_pTalkBoxManager->Clear();
//         }
// 
//         m_pTalkBoxManager->OnFrameMove( fTime, fElapsedTime );
//     }
// }

void CX2PostBox::InitPostSend()
{   
	m_nED = 0;

    CX2SlotItem* pSlotItem = (CX2SlotItem*)GetSlot( CX2SlotItem::ST_POST_SEND, 100 );
    if ( pSlotItem != NULL )
    {
        pSlotItem->DestroyItemUI();      
    }
    
    CKTDGUIIMEEditBox *pDialogTo = (CKTDGUIIMEEditBox*)m_pDLGMailSend->GetControl(L"IME_EditMail_To");
    if(pDialogTo != NULL)
    {
        pDialogTo->SetText(L"");
    }    
    
    CKTDGUIIMEEditBox *pDialogTitle = (CKTDGUIIMEEditBox*)m_pDLGMailSend->GetControl(L"IME_EditMail_Name");
    if(pDialogTitle != NULL)
    {
        pDialogTitle->SetText(L"");
    }

    CKTDGUIIMEEditBox *pDialogMessage = (CKTDGUIIMEEditBox*)m_pDLGMailSend->GetControl(L"IME_Edit_Message");
    if(pDialogMessage != NULL)
    {
        pDialogMessage->SetText(L"");
    }

    CKTDGUIIMEEditBox *pDialogEd = (CKTDGUIIMEEditBox*)m_pDLGMailSend->GetControl(L"IME_Add_Ed");
    if(pDialogEd != NULL)
    {
        pDialogEd->SetText(L"0");  

		//{{ kimhc // 2009-11-10 // ED 입력 시 색 바뀌도록 수정
#ifdef	PRICE_COLOR
		pDialogEd->SetTextColor( g_pMain->GetPriceColor( m_nED ) );
#endif	PRICE_COLOR
		//}} kimhc // 2009-11-10 // ED 입력 시 색 바뀌도록 수정


        pDialogEd->SetEnable(true);
    }

    CX2ItemSlotManager::SetEnable(CX2SlotItem::ST_POST_SEND, true);
    m_bAttachItem = true;

    CKTDGUIStatic *pStaticDlg = m_pDLGMailSend->GetStatic_LUA("g_pStaticPost_SendWindow");
    CKTDGUIControl::CPictureData *pItemPic1, *pItemPic2;
    pItemPic1 = pStaticDlg->GetPictureIndex(3);  
    if(pItemPic1 != NULL)
        pItemPic1->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
    pItemPic2 = pStaticDlg->GetPictureIndex(4);  
    if(pItemPic2 != NULL)
        pItemPic2->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
    CKTDGUIControl::CPictureData *pEdPic1, *pEdPic2;
    pEdPic1 = pStaticDlg->GetPictureIndex(1);  
    if(pEdPic1 != NULL)
        pEdPic1->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f,1.0f));
    pEdPic2 = pStaticDlg->GetPictureIndex(2);  
    if(pEdPic2 != NULL)
        pEdPic2->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f,1.0f));

    WCHAR bufFee[20];
#ifdef SERV_FREE_MAIL_TICKET
	int attachFee = GetPostFee( 0, NULL );
	StringCchPrintf( bufFee,20, L"%d", attachFee );
#else //SERV_FREE_MAIL_TICKET
#ifdef FREE_FEE
	StringCchPrintf(bufFee,20, L"%d", 0);
    //wsprintf(bufFee, L"%d", 0);
#else
	StringCchPrintf(bufFee,20, L"%d", 200);
    //wsprintf(bufFee, L"%d", 200);
#endif
#endif //SERV_FREE_MAIL_TICKET
    CKTDGUIStatic *pDlgFee = m_pDLGMailSend->GetStatic_LUA("g_pStaticpost_window_Font");
    pDlgFee->GetString(0)->msg = bufFee;

    m_pPostSlot = NULL;
	m_iItemUid = -1;
	
    m_bRegisteredTrade = false;
    m_nQuantity = 0;

#ifdef FREE_FEE
    m_nFee = 0;
#else
    m_nFee = 200;    
#endif

    m_bProcess = false;    

    m_strNickName = L"";
    m_strTitle = L"";

    ClearMailList();
}

void CX2PostBox::AddNewLetter( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    if(m_bNewLetterNotify == true)
    {        
        ++m_iNewLetter;              
    }        

    // 패킷 처리
    KSerBuffer pBuff = *((KSerBuffer*)lParam);
    KEGS_RECEIVE_LETTER_NOT kEvent;

    DeSerialize( &pBuff, &kEvent );	

    if(kEvent.m_cScriptType == KPostItemInfo::LT_POST_OFFICE || kEvent.m_cScriptType == KPostItemInfo::LT_NONE)
    {        
        IncTotalMail();
        ++m_nUnReadCommon;
    }
#ifdef SERV_RELATIONSHIP_SYSTEM	
	else if(kEvent.m_cScriptType == KPostItemInfo::LT_WEDDING_INVITATION || kEvent.m_cScriptType == KPostItemInfo::LT_WEDDING_RESERVE || kEvent.m_cScriptType == KPostItemInfo::LT_WEDDING_REWARD )
	{        
		IncTotalMail();
		++m_nUnReadWedding;
	}
#endif // SERV_RELATIONSHIP_SYSTEM	
	
	else
    {
        ++m_nUnReadSystem;
    }
    IncUnReadMail();     
    

#ifdef SERV_SECOND_SECURITY
	if( g_pChatBox != NULL )
	{	
#ifdef CLIENT_COUNTRY_JP
		const int iTempEventIndex = 650;		// 해외팀 일본 변경
#else
		const int iTempEventIndex = 10135;
#endif

		if ( kEvent.m_iScriptIndex == iTempEventIndex)
		{
			wstring wstrBuff = GET_STRING( STR_ID_12546 );
			g_pChatBox->AddChatLog( wstrBuff.c_str(), KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(0.92549f, 0.92549f, 0.53333f, 1.f), L"#CECEC88" );
		}
	}
#endif SERV_SECOND_SECURITY


    //Handler_EGS_RECEIVE_LETTER_NOT( hWnd, uMsg, wParam, lParam );    
}

void CX2PostBox::ClearNewLetter()
{
    m_iNewLetter = 0;
}

bool CX2PostBox::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{    
	
    switch(wParam)
    {
    case PBQM_NEW_MAIL:        

        if(g_pData != NULL && g_pData->GetCashShop() != NULL && g_pData->GetCashShop()->GetOpen() == true)
        {
            // 캐시샵에서는 우편함 열리지 않도록......
            g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(270,350), GET_STRING( STR_ID_391 ), (CKTDXStage*) m_pNowState );
            break;
        }

        CX2State* pNowState = (CX2State*)g_pMain->GetNowState();
        if(pNowState->GetPersonalTrade() == true)
        {
            // 캐시샵에서는 우편함 열리지 않도록......
            g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(270,350), GET_STRING( STR_ID_392 ), (CKTDXStage*) m_pNowState );
            break;
        }
        
#ifndef NEW_SKILL_TREE
		if( NULL != g_pInstanceData->GetSkillTreeUI() && 
			true == g_pInstanceData->GetSkillTreeUI()->GetShow() )
		{
			g_pInstanceData->GetSkillTreeUI()->SetShow( false );
		}
#endif NEW_SKILL_TREE

        m_bPostBox = false;
        //m_pDlgPostBox->SetShowEnable(false, false);
        m_pTalkBoxManager->Clear();

        m_bSystemMail = m_bNewLetterisSystem;
        m_nCurrentPage = 1;        
        m_pDlgMailView->SetShowEnable(false, false);


        m_bShow = true;
        m_bEnable = true;        
        m_nTabIndex = -1;
        SetDialogShow(true, 0);

		CX2ItemSlotManager::SetShow(CX2Slot::ST_POST_RECEIVE, false);
		CX2ItemSlotManager::SetEnable(CX2Slot::ST_POST_RECEIVE, false);

		// note!! special case
#ifdef CHECK_INHERIT_STATE_MENU
		if( g_pMain->IsInheritStateMenu() )
#endif //CHECK_INHERIT_STATE_MENU
		{
			CX2StateMenu* pStateMenu = (CX2StateMenu*) g_pMain->GetNowState();
			if( NULL != pStateMenu )
			{
				pStateMenu->DeleteMouseClickUI();
			}
		}

        break;
    }

    if(GetEnable() == false )
        return false;

    // Dialog Event
    switch(wParam)
    {
    case PBQM_SEND_ACCEPT:
		{
			if(m_pDLGQuery != NULL)
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGQuery, NULL, false );            
			m_pDLGQuery = NULL;
			//{{ kimhc // 2009-09-03 // 봉인된 아이템 보낼때 확인 창 뜨도록 처리
#ifdef	SEAL_ITEM

			CX2Item* pItemData = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( m_iItemUid );
			if ( pItemData != NULL && 
				pItemData->GetItemTemplet() != NULL &&
				pItemData->GetItemData().m_bIsSealed == true )
			{
				m_pDLGQuery = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(250,300), 
                    GET_REPLACED_STRING( ( STR_ID_4471, "SL", pItemData->GetItemTemplet()->GetFullName_(), m_strNickName ) ), 
					PBQM_SEAL_ITEM_SEND_ACCEPT, (CKTDXStage*) m_pNowState, PBQM_SEAL_ITEM_SEND_CANCEL ); 			
			}
			else		
#endif	SEAL_ITEM
			//}} kimhc // 2009-09-03 // 봉인된 아이템 보낼때 확인 창 뜨도록 처리
			{
				m_bProcess = true;
				Handler_EGS_SEND_LETTER_REQ();
			}
		}
        break;

		//{{ kimhc // 2009-09-03 // 봉인된 아이템 보낼때 확인 창 OK
#ifdef	SEAL_ITEM
	case PBQM_SEAL_ITEM_SEND_ACCEPT:
		if(m_pDLGQuery != NULL)
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGQuery, NULL, false );            
		m_pDLGQuery = NULL;
		m_bProcess = true;

		Handler_EGS_SEND_LETTER_REQ();
		break;
#endif	SEAL_ITEM
		//}} kimhc // 2009-09-03 // 봉인된 아이템 보낼때 확인 창 OK

		//{{ kimhc // 2009-09-03 // 봉인된 아이템 보낼때 확인 창 캔슬
#ifdef	SEAL_ITEM
	case PBQM_SEAL_ITEM_SEND_CANCEL:
#endif	SEAL_ITEM
		//}} kimhc // 2009-09-03 // 봉인된 아이템 보낼때 확인 창 캔슬
    case PBQM_SEND_CANCEL:
    case PBQM_DELETE_CANCEL:
        if(m_pDLGQuery != NULL)
            g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGQuery, NULL, false );
        m_pDLGQuery = NULL;
        break;
    case PBQM_DELETE_OK:
        if(m_pDLGQuery != NULL)
            g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGQuery, NULL, false );            
        m_pDLGQuery = NULL;
        m_bProcess = true;
        Handler_EGS_DELETE_LETTER_REQ();
        break;        
    }

    // Manager //////////////////////////////////////////////////////////////////////////////
    switch(wParam)
    {
    case PBMUM_RECEIVE:          
        m_nCurrentPage = 1;            
        if(m_bRegisteredTrade == false)
        {                
            SetDialogShow(true, 0);
        }                
        break;
    case PBMUM_SEND:        
        m_nCurrentPage = 1;
        if(g_pData->GetMyUser()->GetUserData().m_bIsGuestUser == true)
        {
            g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(270,350), GET_STRING( STR_ID_40 ), (CKTDXStage*) m_pNowState );
            return false;
        }        
        if(m_bRegisteredTrade == false)
        {                
            SetDialogShow(true, 1);
        }        
        break;
    case PBMUM_SETTING:        
        if(m_bRegisteredTrade == false)
        {                
            SetDialogShow(true, 2);
        }
                
        break;
    case PBMUM_CLOSE:
		{            
			if(m_bRegisteredTrade == false
#ifdef FIX_GET_ITEM_FROM_LETTER_BUG
				&& false == m_bProcess     
#endif //FIX_GET_ITEM_FROM_LETTER_BUG		
				)
            {   
				SetDialogShow(false);        	
            }
        } 
        break;        
    }

    // SendMail //////////////////////////////////////////////////////////////////////////////
    switch(wParam)
    {    
    case PBSUM_GET_MYUSER_NAME:        
        // 자신이 보유한 캐릭터들중 하나의 이름을 선택
        {
            CKTDGUIComboBox* pComboBox = (CKTDGUIComboBox*)lParam;
            if ( pComboBox != NULL && pComboBox->GetNumItems() > 0)
            {
                CKTDGUIComboBox::ComboBoxItem *pItem = pComboBox->GetItem(pComboBox->GetSelectedItemIndex());
                if(pItem != NULL)
                {
                    CKTDGUIIMEEditBox *pDialogTo = (CKTDGUIIMEEditBox*)m_pDLGMailSend->GetControl(L"IME_EditMail_To");
                    if(pDialogTo != NULL)
                    {
                        pDialogTo->SetText(pItem->strText);
						m_strNickName = pItem->strText;
                    }
                }
            }
        }
        break;   
    case PBSUM_MAIL_SEND:
        SendMail();        
        break;
    case PBSUM_ED_CHANGE:
        CheckEdChange();
        break;
    case PBSUM_TITLE_CHANGE:
        ChangeTitle();    
        break;

	case PBSUM_RECEIVER_NAME_CHANGE:
		{
			ChangeReceiverName();
		} break;
    }

    // ReceiveMail //////////////////////////////////////////////////////////////////////////////
    switch(wParam)
    {    
    case PBRUM_SYSTEM_MAIL:
        m_pDlgMailView->SetShowEnable(false, false);
		CX2ItemSlotManager::SetShow(CX2Slot::ST_POST_RECEIVE, false);
        CX2ItemSlotManager::SetEnable(CX2Slot::ST_POST_RECEIVE, false);
        m_nCurrentPage = 1;
        m_bSystemMail = true;
#ifdef SERV_RELATIONSHIP_SYSTEM
		m_cLetterListType = PLT_SYSTEM;
#endif // SERV_RELATIONSHIP_SYSTEM
		RequestList();
        break;
    case PBRUM_COMMON_MAIL:  
        m_pDlgMailView->SetShowEnable(false, false);
        CX2ItemSlotManager::SetShow(CX2Slot::ST_POST_RECEIVE, false);
        CX2ItemSlotManager::SetEnable(CX2Slot::ST_POST_RECEIVE, false);
        m_nCurrentPage = 1;
        if(g_pData->GetMyUser()->GetUserData().m_bIsGuestUser == true)
        {
            g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(270,350), GET_STRING( STR_ID_40 ), (CKTDXStage*) m_pNowState );
            return false;
        }
        m_bSystemMail = false;

#ifdef SERV_RELATIONSHIP_SYSTEM
		m_cLetterListType = PLT_NORMAL;
#endif // SERV_RELATIONSHIP_SYSTEM
		RequestList();
        break;

#ifdef SERV_RELATIONSHIP_SYSTEM 
	case PBRUM_WEDDING_MAIL:  
		m_pDlgMailView->SetShowEnable(false, false);
		CX2ItemSlotManager::SetShow(CX2Slot::ST_POST_RECEIVE, false);
		CX2ItemSlotManager::SetEnable(CX2Slot::ST_POST_RECEIVE, false);
		m_nCurrentPage = 1;
		if(g_pData->GetMyUser()->GetUserData().m_bIsGuestUser == true)
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(270,350), GET_STRING( STR_ID_40 ), (CKTDXStage*) m_pNowState );
			return false;
		}
		m_bSystemMail = false;
//		m_bWeddingMail = true;

		m_cLetterListType = PLT_WEDDING; // 
		RequestList();		
		break;
#endif //  SERV_RELATIONSHIP_SYSTEM 

	case PBRUM_PREV_PAGE:
        ChangePage(-1);
		//{{ kimhc // 2009-09-28 // 받은 편지 목록 페이지 넘길때, 편지읽기 UI 닫도록 수정
		if( m_pDlgMailView != NULL && m_pDlgMailView->GetShow() == true )
		{
			m_pDlgMailView->SetShowEnable(false, false);
			CX2ItemSlotManager::SetEnable( false );
			CX2ItemSlotManager::SetShow( false );
		}
		//}} kimhc // 2009-09-28 // 받은 편지 목록 페이저 넘길때, 편지읽기 UI 닫도록 수정
        break;
    case PBRUM_NEXT_PAGE:
        ChangePage(1);
		//{{ kimhc // 2009-09-28 // 받은 편지 목록 페이지 넘길때, 편지읽기 UI 닫도록 수정
		if( m_pDlgMailView != NULL && m_pDlgMailView->GetShow() == true )
		{
			m_pDlgMailView->SetShowEnable(false, false);
			CX2ItemSlotManager::SetEnable( false );
			CX2ItemSlotManager::SetShow( false );
		}
		//}} kimhc // 2009-09-28 // 받은 편지 목록 페이저 넘길때, 편지읽기 UI 닫도록 수정
        break;
    case PBRUM_OPEN_MAIL:
        {
            CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;              
            OpenMail( pControl->GetDummyInt( 0 ) );
        }        
        break;
    case PBRUM_DELETE_MAIL:
        {
            CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;        
            CheckDeleteMail(pControl->GetDummyInt( 0 ));
        }        
        break;
		//{{ 허상형 : [2010/8/25/] //	우편 수량 UI 개편
#ifdef POST_QUANTITY_NEW
	case PBSUM_REGISTER_QUANTITY_PLUS:
		{
			m_nQuantity++;
			UpdateRegisterQuantityDLG(false);
			return true;
		}break;
	case PBSUM_REGISTER_QUANTITY_MINUS:
		{
			m_nQuantity--;
			UpdateRegisterQuantityDLG(false);
			return true;
		}break;
	case PBSUM_REGISTER_QUANTITY_UPDATE:
		{
			UpdateRegisterQuantityDLG(true);
			return true;
		}break;
	case PBSUM_REGISTER_QUANTITY_CONFIRM:
		{
			if(m_pDLGItemEnroll != NULL)
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGItemEnroll, NULL, false );

			m_pDLGItemEnroll = NULL;
			m_EnrollItemUid = 0;
			m_bRegisteredTrade = false;

			AttachItem();

			return true;
		}break;
	case PBSUM_REGISTER_QUANTITY_CANCEL:
		{
			if(m_pDLGItemEnroll != NULL)
				g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGItemEnroll, NULL, false );

			m_pDLGItemEnroll = NULL;
			m_nQuantity = 1;		// 수량성 아이템을 버리는 Dialog Text 1로 고정
			m_EnrollItemUid = 0;
			m_bRegisteredTrade = false;

			//{{ 11.09.26 임규수 우편 수량성 아이템 첨부 취소 오류 수정
#ifdef POSTBOX_CANCEL_QUANTITY_ITEM_FIX
			UnRegisterItem();
#endif
			//}}

			return true;
		}break;
#endif	//	POST_QUANTITY_NEW
		//}} 허상형 : [2010/8/25/] //	우편 수량 UI 개편

#ifdef ADD_POST_BOX_PAGE_BUTTON //2013.07.15
	case PBRUM_FIRST_PAGE:
		{
			MoveFirstPage( true );
			//{{ kimhc // 2009-09-28 // 받은 편지 목록 페이지 넘길때, 편지읽기 UI 닫도록 수정
			if( m_pDlgMailView != NULL && m_pDlgMailView->GetShow() == true )
			{
				m_pDlgMailView->SetShowEnable(false, false);
				CX2ItemSlotManager::SetEnable( false );
				CX2ItemSlotManager::SetShow( false );
			}
			//}} kimhc // 2009-09-28 // 받은 편지 목록 페이저 넘길때, 편지읽기 UI 닫도록 수정
		} break;

	case PBRUM_LAST_PAGE:
		{
			MoveFirstPage( false );
			//{{ kimhc // 2009-09-28 // 받은 편지 목록 페이지 넘길때, 편지읽기 UI 닫도록 수정
			if( m_pDlgMailView != NULL && m_pDlgMailView->GetShow() == true )
			{
				m_pDlgMailView->SetShowEnable(false, false);
				CX2ItemSlotManager::SetEnable( false );
				CX2ItemSlotManager::SetShow( false );
			}
			//}} kimhc // 2009-09-28 // 받은 편지 목록 페이저 넘길때, 편지읽기 UI 닫도록 수정
		} break;
#endif //ADD_POST_BOX_PAGE_BUTTON
    }

    // View //////////////////////////////////////////////////////////////////////////////////
    switch(wParam)
    {
    case PBVUM_CLOSE:        
        if(m_pDlgMailView != NULL)
            m_pDlgMailView->SetShowEnable(false, false);
        CX2ItemSlotManager::SetEnable( false );
        CX2ItemSlotManager::SetShow( false );
        break;
    case PBVUM_GET_ITEM:        
#ifdef ADDED_RELATIONSHIP_SYSTEM
		for(int i=0; i<6; ++i)
		{
			if(m_listSlot[i].m_bSelected == true)
			{
				GetItemLetter(i);
				break;
			}
		}     
#endif // ADDED_RELATIONSHIP_SYSTEM
        break;
    case PBVUM_DELETE:
        for(int i=0; i<6; ++i)
        {
            if(m_listSlot[i].m_bSelected == true)
            {
                CheckDeleteMail(i+1);
                break;
            }
        }        
        break;
    case PBVUM_REPLY:
        for(int i=0; i<6; ++i)
        {
            if(m_listSlot[i].m_bSelected == true)
            {
                ReplyMail(i);
                break;
            }
        }                
        break;
    }

    // Quantity //////////////////////////////////////////////////////////////////////////////
    switch(wParam)
    {    
    case PBSUM_QUANTITY_CANCEL:
        if(m_pDLGMyTradeWindow != NULL)
        {
            m_bRegisteredTrade = false;
            //m_pDLGMailSend->SetModal(false);
            m_pDLGMyTradeWindow->SetShowEnable(false, false);            
        }
        break;

    case PBSUM_QUANTITY_OK:
        AttachItem();
        break;
    case PBSUM_QUANTITY_CHANGE:
        CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;
        SetItemNumChange( pControl->GetDummyInt( 0 ) );
        break;
    }


	

    
    
    // Option
    switch(wParam)
    {
    case PBOUM_BLIST_SELECT:
        //CKTDGUIListBox *pListbox = (CKTDGUIListBox*)m_pDlgMailSetting->GetControl(L"ListBoxContent");
        break;
    case PBOUM_BLIST_ACCEPT:
        {
            CKTDGUIIMEEditBox *pDlgUser = (CKTDGUIIMEEditBox*)m_pDLGAddBlackList->GetControl(L"IMEID"); 
            m_pDLGAddBlackList->SetShowEnable(true, true);
            pDlgUser->SetText(L"");			
        }        
        break;
    case PBOUM_BLIST_DELETE:
        if(m_bProcess == true)
            return false;
        m_bProcess = true;
        Handler_EGS_DEL_POST_BLACK_LIST_REQ();
        break;                
    case PBOUM_BLIST_OK:
        if(m_bProcess == true)
            return false;
        m_bProcess = true;
        Handler_EGS_NEW_POST_BLACK_LIST_REQ();
        break;
    case PBOUM_CLOSE:
        m_pDLGAddBlackList->SetShowEnable(false, false);
        break;
    case PBOUM_CHECK_ALARM:
        ChangeAlarm();
        break;
    case PBOUM_CHECK_NEWLETTER:
        ChangeNewNotify();
        break;
    }

    return false;
}

bool CX2PostBox::UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{

    switch( wParam)
    {
#if 1 
    case EGS_RECEIVE_LETTER_NOT:
        //IncUnReadMail();
        //IncTotalMail();        
#ifdef FIX_MISSING_MAIL
		if( m_bNewLetterNotify == true && g_pData != NULL && g_pData->GetCashShop() != NULL )
		{
			if( g_pData->GetCashShop()->GetOpen() == false )
			{
				m_iNewLetter = 0;
				return Handler_EGS_RECEIVE_LETTER_NOT( hWnd, uMsg, wParam, lParam );
			}
			else
			{
				KSerBuffer* pBuff = (KSerBuffer*)lParam;
				KEGS_RECEIVE_LETTER_NOT kEvent;

				DeSerialize( pBuff, &kEvent );
				g_pData->GetCashShop()->AddDelayedMail( kEvent );
			}
		}
#else FIX_MISSING_MAIL
        if(m_bNewLetterNotify == true && g_pData != NULL && g_pData->GetCashShop() != NULL && g_pData->GetCashShop()->GetOpen() == false)
        {            
            m_iNewLetter = 0;
            return Handler_EGS_RECEIVE_LETTER_NOT( hWnd, uMsg, wParam, lParam );
        }
#endif FIX_MISSING_MAIL
        break;
#endif
    case EGS_NEW_POST_LETTER_INFO_NOT:
        return Handler_EGS_NEW_POST_LETTER_INFO_NOT( hWnd, uMsg, wParam, lParam );
        break;
	case EGS_RENEWAL_LETTER_ACK:
		return Handler_EGS_RENEWAL_LETTER_ACK( hWnd, uMsg, wParam, lParam );
    }

    if( false == GetEnable() )
        return false;

    switch( wParam )
    {    
    case EGS_SEND_LETTER_ACK:
        return Handler_EGS_SEND_LETTER_ACK( hWnd, uMsg, wParam, lParam );
    case EGS_DELETE_LETTER_ACK:
        return Handler_EGS_DELETE_LETTER_ACK( hWnd, uMsg, wParam, lParam );
    case EGS_READ_LETTER_ACK:
        return Handler_EGS_READ_LETTER_ACK( hWnd, uMsg, wParam, lParam );
    case EGS_GET_POST_LETTER_LIST_ACK:
        return Handler_EGS_GET_POST_LETTER_LIST_ACK( hWnd, uMsg, wParam, lParam );
    case EGS_GET_ITEM_FROM_LETTER_ACK:
        return Handler_EGS_GET_ITEM_FROM_LETTER_ACK( hWnd, uMsg, wParam, lParam );
    case EGS_GET_POST_BLACK_LIST_ACK:
        return Handler_EGS_GET_POST_BLACK_LIST_ACK( hWnd, uMsg, wParam, lParam );
    case EGS_DEL_POST_BLACK_LIST_ACK:
        return Handler_EGS_DEL_POST_BLACK_LIST_ACK( hWnd, uMsg, wParam, lParam );
    case EGS_NEW_POST_BLACK_LIST_ACK:
        return Handler_EGS_NEW_POST_BLACK_LIST_ACK( hWnd, uMsg, wParam, lParam );	
    }

    return false;
}

void CX2PostBox::AttachItem()
{
    if(m_pDLGMyTradeWindow != NULL)
    {
        if(m_nQuantity <= 0)
            m_nQuantity = 1;

        m_bRegisteredTrade = false;
        //m_pDLGMailSend->SetModal(false);
        m_pDLGMyTradeWindow->SetShowEnable(false, false);

		CX2Item* pItemData = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( m_iItemUid );

        // 미리보기 아이템 셋팅
        CX2SlotItem* pSlotItem = (CX2SlotItem*)GetSlot( CX2SlotItem::ST_POST_SEND, 100 );
        if ( pSlotItem != NULL )
        {
            pSlotItem->DestroyItemUI();

			if( pItemData != NULL )
			{
				pSlotItem->CreateItemUI( pItemData, m_nQuantity );
			}
        }

		if( pItemData != NULL )
		{
			CKTDGUIIMEEditBox *pDialogEd = (CKTDGUIIMEEditBox*)m_pDLGMailSend->GetControl(L"IME_Add_Ed");       
			pDialogEd->SetEnable(false); 

			CKTDGUIStatic *pStaticDlg = m_pDLGMailSend->GetStatic_LUA("g_pStaticPost_SendWindow");
			CKTDGUIControl::CPictureData *pEdPic1, *pEdPic2;
			pEdPic1 = pStaticDlg->GetPictureIndex(1);  
			if(pEdPic1 != NULL)
				pEdPic1->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));
			pEdPic2 = pStaticDlg->GetPictureIndex(2);  
			if(pEdPic2 != NULL)
				pEdPic2->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));

#ifdef SERV_FREE_MAIL_TICKET
			int attachFee = GetPostFee( 0, pItemData );
			WCHAR bufFee[20];
			StringCchPrintf( bufFee, 20, L"%d", attachFee );
#else //SERV_FREE_MAIL_TICKET
			int price = (int)(pItemData->GetItemTemplet()->GetPrice() * 0.2f);
			int quantity = m_nQuantity;
			int attachFee = (int)((price * quantity) * 0.05f);

			WCHAR bufFee[20];
#ifdef FREE_FEE
			StringCchPrintf(bufFee, 20, L"0");
			//wsprintf(bufFee, L"0");
#else
			StringCchPrintf(bufFee, 20, L"%d", 200 + attachFee);
			//wsprintf(bufFee, L"%d", 200 + attachFee);
#endif
#endif //SERV_FREE_MAIL_TICKET

			CKTDGUIStatic *pDlgFee = m_pDLGMailSend->GetStatic_LUA("g_pStaticpost_window_Font");
			pDlgFee->GetString(0)->msg = bufFee;

			g_pKTDXApp->GetDeviceManager()->PlaySound( L"Post_ItemDrop.ogg", false, false );    
		}       
    }        
}

void CX2PostBox::CheckEdChange()
{
    bool bChange = false;
    CKTDGUIIMEEditBox *pDialogEd = (CKTDGUIIMEEditBox*)m_pDLGMailSend->GetControl(L"IME_Add_Ed");       

    wstring						wstrMessage;

	m_nED = g_pMain->GetEDFromString( pDialogEd->GetText() );
    if( m_nED < 0 )
    {
        m_nED = 0;
        bChange = true;
    }
    else if( m_nED > MAX_ED_FOR_LETTER )
    {
        m_nED = MAX_ED_FOR_LETTER;     
        bChange = true;
    }

    if(m_nED >= g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_ED)
    {
        m_nED = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_ED;
        bChange = true;
    }

    //if(bChange) // 숫자 입력할 때마다 쉼표위치 변경을 위해서
    {
		wstring wstrED = g_pMain->GetEDString( m_nED );
        pDialogEd->SetText( wstrED.c_str() );

		//{{ kimhc // 2009-11-10 // ED 입력 시 색 바뀌도록 수정
#ifdef	PRICE_COLOR
		pDialogEd->SetTextColor( g_pMain->GetPriceColor( m_nED ) );
#endif	PRICE_COLOR
		//}} kimhc // 2009-11-10 // ED 입력 시 색 바뀌도록 수정

    }

    if(m_nED > 0)
    {
        CX2ItemSlotManager::SetEnable(CX2SlotItem::ST_POST_SEND, false);  
        m_bAttachItem = false;

        CKTDGUIStatic *pStaticDlg = m_pDLGMailSend->GetStatic_LUA("g_pStaticPost_SendWindow");
        CKTDGUIControl::CPictureData *pItemPic1, *pItemPic2;
        pItemPic1 = pStaticDlg->GetPictureIndex(3);  
        if(pItemPic1 != NULL)
            pItemPic1->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));
        pItemPic2 = pStaticDlg->GetPictureIndex(4);  
        if(pItemPic2 != NULL)
            pItemPic2->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));

		wstring wstrFee = L"";
#ifdef SERV_FREE_MAIL_TICKET
		wstrFee = g_pMain->GetEDString( GetPostFee( m_nED, NULL ) );
#else //SERV_FREE_MAIL_TICKET
#ifdef FREE_FEE
		wstrFee = L"0";
#else
		wstrFee = g_pMain->GetEDString( 200 + (int)(m_nED*0.05) );
#endif
#endif //SERV_FREE_MAIL_TICKET

		CKTDGUIStatic *pDlgFee = m_pDLGMailSend->GetStatic_LUA("g_pStaticpost_window_Font");
        pDlgFee->GetString(0)->msg = wstrFee;
    }
    else
    {
        CX2ItemSlotManager::SetEnable(CX2SlotItem::ST_POST_SEND, true);
        m_bAttachItem = true;

        CKTDGUIStatic *pStaticDlg = m_pDLGMailSend->GetStatic_LUA("g_pStaticPost_SendWindow");
        CKTDGUIControl::CPictureData *pItemPic1, *pItemPic2;
        pItemPic1 = pStaticDlg->GetPictureIndex(3);  
        if(pItemPic1 != NULL)
            pItemPic1->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
        pItemPic2 = pStaticDlg->GetPictureIndex(4);  
        if(pItemPic2 != NULL)
            pItemPic2->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		CX2Item* pItemData = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( m_iItemUid );
		if( pItemData == NULL && m_nED == 0 )
        {
			wstring wstrFee = L"";
#ifdef SERV_FREE_MAIL_TICKET
			wstrFee = g_pMain->GetEDString( GetPostFee( 0, NULL ) );
#else //SERV_FREE_MAIL_TICKET
#ifdef FREE_FEE
			wstrFee = L"0";
#else
			wstrFee = L"200";
#endif
#endif //SERV_FREE_MAIL_TICKET

			CKTDGUIStatic *pDlgFee = m_pDLGMailSend->GetStatic_LUA("g_pStaticpost_window_Font");
            pDlgFee->GetString(0)->msg = wstrFee;
        }
    }
}

void CX2PostBox::ChangeTitle()
{
    CKTDGUIIMEEditBox *pDialogTitle = (CKTDGUIIMEEditBox*)m_pDLGMailSend->GetControl(L"IME_EditMail_Name");
    m_strTitle = pDialogTitle->GetText();

    if(m_strTitle.length() <= 0)
    {
        m_bEditTitle = false;
    }
    else
    {
        m_bEditTitle = true;
    }
}



void CX2PostBox::ChangeReceiverName()
{
	CKTDGUIIMEEditBox *pDialogTo = (CKTDGUIIMEEditBox*)m_pDLGMailSend->GetControl( L"IME_EditMail_To" );
	if(pDialogTo != NULL)
	{
		m_strNickName = pDialogTo->GetText();
		pDialogTo->SetText( m_strNickName.c_str() );
	}
}






void CX2PostBox::EnablePostQuantityButton( bool bEnable )
{
    CKTDGUIButton* pButton_Quantity_Up		= (CKTDGUIButton*) m_pDLGMyTradeWindow->GetControl( L"Quantity_Increase" );
    CKTDGUIButton* pButton_Quantity_Down	= (CKTDGUIButton*) m_pDLGMyTradeWindow->GetControl( L"Quantity_Decrease" );

    CKTDGUIButton* pButton_Quantity_Up2		= (CKTDGUIButton*) m_pDLGMyTradeWindow->GetControl( L"Quantity_Increase2" );
    CKTDGUIButton* pButton_Quantity_Down2	= (CKTDGUIButton*) m_pDLGMyTradeWindow->GetControl( L"Quantity_Decrease2" );

    pButton_Quantity_Up->SetShowEnable( bEnable, bEnable );
    pButton_Quantity_Down->SetShowEnable( bEnable, bEnable );

    pButton_Quantity_Up2->SetShowEnable( bEnable, bEnable );
    pButton_Quantity_Down2->SetShowEnable( bEnable, bEnable );

    CKTDGUIControl* pStaticMy_TradeBox_10Goods = m_pDLGMyTradeWindow->GetControl( L"g_pStaticMy_TradeBox_10Goods" );
    if ( pStaticMy_TradeBox_10Goods != NULL )
    {
        pStaticMy_TradeBox_10Goods->SetShowEnable( bEnable, bEnable );
    }
}

bool CX2PostBox::OnRClickedItem( D3DXVECTOR2 mousePos )
{
	return OnRClickedItemInInven(mousePos);
}


bool CX2PostBox::OnRClickedItemInInven( D3DXVECTOR2 mousePos )
{

    // 마우스업된 위치가 인벤 안일 경우
    CX2SlotItem* pItemSlot = NULL;
    pItemSlot = (CX2SlotItem*) GetSlotInMousePos( mousePos );
    if ( pItemSlot == NULL )
        return false;

    if( pItemSlot->GetSlotType() == CX2SlotManager::CX2Slot::ST_POST_SEND )
    {
        if(m_pPostSlot != NULL)
        {
            m_pPostSlot = NULL;
            CX2SlotItem* pSlotItem = (CX2SlotItem*)GetSlot( CX2SlotItem::ST_POST_SEND, 100 );
            if ( pSlotItem != NULL )
            {
                pSlotItem->DestroyItemUI();     
                
				m_iItemUid = -1;
                g_pKTDXApp->GetDeviceManager()->PlaySound( L"Post_ItemDrop.ogg", false, false );  

                CKTDGUIIMEEditBox *pDialogEd = (CKTDGUIIMEEditBox*)m_pDLGMailSend->GetControl(L"IME_Add_Ed");
                pDialogEd->SetEnable(true);  

                CKTDGUIStatic *pStaticDlg = m_pDLGMailSend->GetStatic_LUA("g_pStaticPost_SendWindow");
                CKTDGUIControl::CPictureData *pEdPic1, *pEdPic2;
                pEdPic1 = pStaticDlg->GetPictureIndex(1);  
                if(pEdPic1 != NULL)
                    pEdPic1->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f,1.0f));
                pEdPic2 = pStaticDlg->GetPictureIndex(2);  
                if(pEdPic2 != NULL)
                    pEdPic2->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f,1.0f));

                if(m_bEditTitle == false)
                {
                    CKTDGUIIMEEditBox *pDialogTitle = (CKTDGUIIMEEditBox*)m_pDLGMailSend->GetControl(L"IME_EditMail_Name");
                    if(pDialogTitle != NULL)
                        pDialogTitle->SetText(L"");
                }

                CheckEdChange();
				InvalidSlotDesc();
            }
        }

        SetInventorySort();
        return false;
    }
    else if( pItemSlot->GetSlotType() == CX2SlotManager::CX2Slot::ST_POST_RECEIVE )
    {
        int inx = -1;
#ifdef CONVERSION_VS
		int i = 0; //selected index of slot
		for(i=0; i<6; ++i)
#else CONVERSION_VS
        for(int i=0; i<6; ++i)
#endif CONVERSION_VS
        {
            if(m_listSlot[i].m_bSelected == true)
            {
                inx = i;
                break;
            }
        }

        if( i >= 0 && i < 6 && 
			m_listSlot[i].m_pItem != NULL)
        {
            // 아이템을 가져온다.
            if(inx >= 0 && inx < 6)
			{
                GetItemLetter(inx);
				InvalidSlotDesc();
			}
            else
                return false;
        }
    }


    if ( pItemSlot->GetSlotType() != CX2SlotManager::CX2Slot::ST_INVENTORY )
        return false;

	return false;
}

void CX2PostBox::RegisterItem(CX2SlotItem* pItemSlot)
{
	if(pItemSlot == NULL)
		return;

    CX2Item* pkItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( pItemSlot->GetItemUID() );
    if ( pkItem == NULL || 
        NULL == pkItem->GetItemTemplet() 
        )
        return;

	//{{ kimhc // 2009-09-03 // 봉인 아이템 우편 전송 가능하도록
#ifdef	SEAL_ITEM

	if ( pkItem->GetItemData().m_Period > 0 ||
		 ( pkItem->GetItemTemplet()->GetVested() == true && pkItem->GetItemData().m_bIsSealed == false ) )
#else	SEAL_ITEM
	if( true == pkItem->GetItemTemplet()->GetVested() ||
		( pkItem->GetItemData().m_Period > 0 ) )
#endif	SEAL_ITEM
	//}} kimhc // 2009-09-03 // 봉인 아이템 우편 전송 가능하도록
    
    {
        g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250, 300), GET_STRING( STR_ID_393 ), (CKTDXStage*) m_pNowState );
        return;
    }

#ifdef ITEM_RECOVERY_TEST
	// 강화 레벨 검사 (사용불능인가?)
	if(true == pkItem->IsDisabled())
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_394 ), m_pNowState );
		return; 
	}
#endif //ITEM_RECOVERY_TEST

	switch( pkItem->GetItemTemplet()->GetPeriodType() )
	{
	default:
		{
			ASSERT( !"invalid period type" );
		} break;
	case CX2Item::PT_INFINITY:
	case CX2Item::PT_QUANTITY:
		{
			// 내구도 확인 하지 않음.
		} break;
	case CX2Item::PT_ENDURANCE:
		{
			if( pkItem->GetItemData().m_Endurance < pkItem->GetItemTemplet()->GetEndurance() )
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250, 300), GET_STRING( STR_ID_395 ), (CKTDXStage*) m_pNowState );
				return;
			}
		} break;
	}
    
    SetInventorySort();

    m_pPostSlot = pItemSlot;
    SetSlotItem(pkItem);
}

//{{ 11.09.26 임규수 우편 수량성 아이템 첨부 취소 오류 수정
#ifdef POSTBOX_CANCEL_QUANTITY_ITEM_FIX

void CX2PostBox::UnRegisterItem()
{
	if(m_pPostSlot != NULL)
	{
		m_pPostSlot = NULL;
		CX2SlotItem* pSlotItem = (CX2SlotItem*)GetSlot( CX2SlotItem::ST_POST_SEND, 100 );
		if ( pSlotItem != NULL )
		{
			pSlotItem->DestroyItemUI();     
			
			m_iItemUid = -1;
			g_pKTDXApp->GetDeviceManager()->PlaySound( L"Post_ItemDrop.ogg", false, false );  

			CKTDGUIIMEEditBox *pDialogEd = (CKTDGUIIMEEditBox*)m_pDLGMailSend->GetControl(L"IME_Add_Ed");
			pDialogEd->SetEnable(true);  

			CKTDGUIStatic *pStaticDlg = m_pDLGMailSend->GetStatic_LUA("g_pStaticPost_SendWindow");
			CKTDGUIControl::CPictureData *pEdPic1, *pEdPic2;
			pEdPic1 = pStaticDlg->GetPictureIndex(1);  
			if(pEdPic1 != NULL)
				pEdPic1->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f,1.0f));
			pEdPic2 = pStaticDlg->GetPictureIndex(2);  
			if(pEdPic2 != NULL)
				pEdPic2->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f,1.0f));

			if(m_bEditTitle == false)
			{
				CKTDGUIIMEEditBox *pDialogTitle = (CKTDGUIIMEEditBox*)m_pDLGMailSend->GetControl(L"IME_EditMail_Name");
				if(pDialogTitle != NULL)
					pDialogTitle->SetText(L"");
			}

			CheckEdChange();
			InvalidSlotDesc();
		}
	}

	SetInventorySort();
}

#endif //POSTBOX_CANCEL_QUANTITY_ITEM_FIX
//}}

void CX2PostBox::SetSlotItem(CX2Item *pkItem)
{
    CKTDGUIIMEEditBox *pDialogEd = (CKTDGUIIMEEditBox*)m_pDLGMailSend->GetControl(L"IME_Add_Ed");       

    bool                        bChange = false;
    wstring						wstrMessage;

	m_nED = g_pMain->GetEDFromString( pDialogEd->GetText() );
    if( m_nED < 0 )
    {
        m_nED = 0;
        bChange = true;
    }
    else if( m_nED > MAX_ED_FOR_LETTER )
    {
        m_nED = MAX_ED_FOR_LETTER;     
        bChange = true;
    }

    if(m_nED >= g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_ED)
    {
        m_nED = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_ED;
        bChange = true;
    }

    if(m_nED > 0)
        return;

    m_bRegisteredTrade = true;
	m_iItemUid = pkItem->GetUID();
	
    
    if(pkItem->GetItemData().m_PeriodType == CX2Item::PT_QUANTITY)
    {
		//{{ 허상형 : [2010/8/25/] //	우편 수량 UI 개편
#ifdef POST_QUANTITY_NEW
		D3DXVECTOR2 Pos;
		Pos.x = static_cast< float >( g_pKTDXApp->GetDIManager()->GetMouse()->GetXPos() );
		Pos.y = static_cast< float >( g_pKTDXApp->GetDIManager()->GetMouse()->GetYPos() );

		m_nQuantity	= pkItem->GetItemData().m_Quantity;
		m_EnrollItemUid = pkItem->GetItemData().m_ItemUID;

		OpenRegisterQuantityDLG( Pos );
#else	//	POST_QUANTITY_NEW
		// 수량확인
		m_pDLGMyTradeWindow->SetShowEnable( true, true );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDLGMyTradeWindow, true );
		OpenItemQuantityDLG(pkItem->GetUID());
#endif	//	POST_QUANTITY_NEW
		//}} 허상형 : [2010/8/25/] //	우편 수량 UI 개편
    }
    else
    {
        m_nQuantity	= 1;
        AttachItem();
    }

    // 제목확인
    CKTDGUIIMEEditBox *pDialogTitle = (CKTDGUIIMEEditBox*)m_pDLGMailSend->GetControl(L"IME_EditMail_Name");
    m_strTitle = pDialogTitle->GetText();

    //if(m_strTitle.length() <= 0)
    if(m_bEditTitle == false || m_strTitle.length() <= 0)
    {   
		wstring wstrItemName( pkItem->GetFullName() );
		//{{ kimhc // 2009-11-26 // 아이템 등록시 제목에서 아이템 이름에 포함된 ' 제거
		wstring::iterator wstrItr = std::remove( wstrItemName.begin(), wstrItemName.end(), L'\'' );

		if ( wstrItr != wstrItemName.end() )
			wstrItemName.erase( wstrItr, wstrItemName.end() );
		//}} kimhc // 2009-11-26 // 아이템 등록시 제목에서 아이템 이름에 포함된 ' 제거

        pDialogTitle->SetText(wstrItemName.c_str());
        m_bEditTitle = false;
    }
}

bool CX2PostBox::MouseRButtonUp( D3DXVECTOR2 mousePos )
{    
    if(m_bProcess || m_bRegisteredTrade || m_bAttachItem == false)
        return false;

#ifdef NEW_VILLAGE_UI
    *m_pSlotBeforeDragging = NULL;
    *m_DraggingItemUID = -1;
#else
	m_pSlotBeforeDragging = NULL;
	m_DraggingItemUID = -1;
#endif
    m_DraggingItemTID = -1;
    
    return OnRClickedItemInInven( mousePos );
    
}

void CX2PostBox::MouseMove( D3DXVECTOR2 mousePos )
{
	CX2ItemSlotManager::MouseMove(mousePos);
}
 
bool CX2PostBox::MouseUp( D3DXVECTOR2 mousePos )
{     
    if(m_bProcess || m_bRegisteredTrade || m_bAttachItem == false || m_CurrentState == 0)
        return false;

    //we-need-any-personal-item-slot-find

    // 마우스업된 위치가 임의의 인벤토리 슬롯인 경우
    // No Change Inven

	if ( *m_pSlotBeforeDragging != NULL )
	{
		int slotIndex = IsInPostItemReact( mousePos );
	    
		if(slotIndex != -1)
		{
			//m_PickedShopItemIndex = slotIndex + (m_iNowPageIndex-1)*3;
	#ifdef NEW_VILLAGE_UI
			CX2SlotItem* pSlotItem = (CX2SlotItem*)*m_pSlotBeforeDragging;
	#else
			CX2SlotItem* pSlotItem = (CX2SlotItem*)m_pSlotBeforeDragging;
	#endif

			//if ( CheckDoubleShopItem( pSlotItem->GetItemUID(), m_PickedShopItemIndex ) == true )
			//{
			//    g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_396 ), m_pNowState );
			//}
			//else
			{
				bool bCheck = true;
				CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( pSlotItem->GetItemUID() );

				switch( pItem->GetItemTemplet()->GetPeriodType() )
				{
				default:
					{
						ASSERT( !"invalid period type" );
					} break;
				case CX2Item::PT_INFINITY:
				case CX2Item::PT_QUANTITY:
					{
						// 내구도 확인 하지 않음.
					} break;
				case CX2Item::PT_ENDURANCE:
					{
						if( pItem->GetItemData().m_Endurance < pItem->GetItemTemplet()->GetEndurance() )
						{
							bCheck = false;
							g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250, 300), GET_STRING( STR_ID_395 ), (CKTDXStage*) m_pNowState );
							//return false;
						}
					} break;
				}

                if ( bCheck == true )
                {
	//{{ kimhc // 2009-09-03 // 봉인 아이템 우편 전송 가능하도록
#ifdef	SEAL_ITEM
					if ( pItem->GetItemData().m_Period > 0 ||
						( pItem->GetItemTemplet()->GetVested() == true && pItem->GetItemData().m_bIsSealed == false ) )
#else	SEAL_ITEM
					if( pItem->GetItemTemplet()->GetVested() == true ||
						( pItem->GetItemData().m_Period > 0 ) )
#endif	SEAL_ITEM
	//}} kimhc // 2009-09-03 // 봉인 아이템 우편 전송 가능하도록
					{
						bCheck = false;
						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_393 ),m_pNowState );
					}
                }   

#ifdef ITEM_RECOVERY_TEST
				// 강화 레벨 검사 (사용불능인가?)
				if(true == pItem->IsDisabled())
				{
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_394 ), m_pNowState );
					bCheck = false;
				}
#endif //ITEM_RECOVERY_TEST

				if(bCheck == true)
				{
					// 미리보기 아이템 셋팅
					SetSlotItem(pItem);   
					m_pPostSlot = pSlotItem;
				}
			}

			SetInventorySort();
			
			//*m_pSlotBeforeDragging = NULL;
			//*m_DraggingItemUID = -1;
			return true;
	        
		}           
		else
		{
	#ifdef NEW_VILLAGE_UI
			if((*m_pSlotBeforeDragging)->GetSlotType() == CX2SlotItem::ST_POST_SEND)
	#else
			if(m_pSlotBeforeDragging->GetSlotType() == CX2SlotItem::ST_POST_SEND)
	#endif
			{
				if(m_pPostSlot != NULL)
				{
					m_pPostSlot = NULL;
					CX2SlotItem* pSlotItem = (CX2SlotItem*)GetSlot( CX2SlotItem::ST_POST_SEND, 100 );
					if ( pSlotItem != NULL )
					{
						pSlotItem->DestroyItemUI();
						m_iItemUid = -1;
						g_pKTDXApp->GetDeviceManager()->PlaySound( L"Post_ItemDrop.ogg", false, false );  
						CheckEdChange();
					}
	                
					m_bRegisteredTrade = false;
					CKTDGUIIMEEditBox *pDialogEd = (CKTDGUIIMEEditBox*)m_pDLGMailSend->GetControl(L"IME_Add_Ed");
					pDialogEd->SetEnable(true);

					CKTDGUIStatic *pStaticDlg = m_pDLGMailSend->GetStatic_LUA("g_pStaticPost_SendWindow");
					CKTDGUIControl::CPictureData *pEdPic1, *pEdPic2;
					pEdPic1 = pStaticDlg->GetPictureIndex(1);  
					if(pEdPic1 != NULL)
						pEdPic1->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
					pEdPic2 = pStaticDlg->GetPictureIndex(2);  
					if(pEdPic2 != NULL)
						pEdPic2->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

				}
				SetInventorySort();
				
				//*m_pSlotBeforeDragging = NULL;
				//*m_DraggingItemUID = -1;
				return true;
			}
		}
	}
	
    return false;
}


bool CX2PostBox::MouseDown( D3DXVECTOR2 mousePos )
{   
    // 현재 메일관련 기능 수행중이거나 수량등록, 아이템등록, 메일확인/뷰 중일때는 처리하지 않는다.
    if(m_bProcess || m_bRegisteredTrade || m_bAttachItem == false || m_CurrentState == 0)
        return false;
	
	return CX2ItemSlotManager::MouseDown( mousePos );
}

int CX2PostBox::IsInPostItemReact( D3DXVECTOR2 mousePos )
{    
    for ( int i = 0; i < (int)m_SlotList.size(); i++ )
    {
        CX2SlotItem* pSlotItem = (CX2SlotItem*)m_SlotList[i];
        if ( pSlotItem != NULL )
        {
            if ( pSlotItem->GetSlotType() == CX2SlotItem::ST_POST_SEND )
            {
                if ( pSlotItem->IsInMousePoint( mousePos ) == true )
                {
                    return pSlotItem->GetSlotID();
                }
            }			
        }
    }

    return -1;
}

CX2SlotItem* CX2PostBox::GetPostSlot()
{
    for ( int i = 0; i < (int)m_SlotList.size(); i++ )
    {
        CX2SlotItem* pSlotItem = (CX2SlotItem*)m_SlotList[i];
        if ( pSlotItem != NULL )
        {
            if ( pSlotItem->GetSlotType() == CX2SlotItem::ST_POST_SEND )
            {                
                return pSlotItem;
            }			
        }
    }

    return NULL;
}

void CX2PostBox::DrawSlotMouseOverImage()
{
	CX2ItemSlotManager::DrawSlotMouseOverImage(); //DrawSlotMouseOverImageInEquipRect();
}

bool CX2PostBox::DrawSlotMouseOverImageInEquipRect()
{
    bool bCheck = false;
    
    // 마우스 커서가 장비창 영역안에 있을때 처리
    if ( IsInEquippedItemSlotReact( m_NowMousePos ) == true )
    {
#ifdef NEW_VILLAGE_UI
		//{{ 2008.11.13 김태완 : UI 예외처리
		if(!(*m_pSlotBeforeDragging)->IsItem()) return false;
		//*m_DraggingItemUID = ((CX2SlotItem*)(*m_pSlotBeforeDragging))->GetItemUID();
		//}}

        if(*m_DraggingItemUID == -1)
        {            
            return false;
        }
        const CX2Item::ItemTemplet* pkItemTemplet = 
            g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( *m_DraggingItemUID )->GetItemTemplet();
        if ( pkItemTemplet == NULL )
            return false;
       

#else
		if(m_DraggingItemUID == -1)
		{            
			return false;
		}

        const CX2Item::ItemTemplet* pkItemTemplet =
			g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( m_DraggingItemUID )->GetItemTemplet();
		if ( pkItemTemplet == NULL )
			return false;

#endif

        CX2Item::ITEM_TYPE itemType = pkItemTemplet->GetItemType();
        if ( itemType == CX2Item::IT_WEAPON || itemType == CX2Item::IT_DEFENCE || itemType == CX2Item::IT_ACCESSORY ||
            itemType == CX2Item::IT_SKILL )
        {
            CX2Unit::EQIP_POSITION equipPosition = pkItemTemplet->GetEqipPosition();
            for ( int i = 0; i < (int)m_SlotList.size(); i++ )
            {
                CX2SlotItem* pItemSlot = (CX2SlotItem*)GetSlot(i);
                if ( pItemSlot->GetSlotType() == CX2Slot::ST_SHOP_PREVIEW )
                {
                    m_pDLGSelectedItem->SetPos( pItemSlot->GetPos() );
                    m_pDLGSelectedItem->GetStatic_LUA( "SelectedItem" )->GetPicture(0)->SetSize( pItemSlot->GetSize() );

                    bCheck = true;
                }
            }
        }
        else
        {
            if ( m_pNowOverItemSlot != NULL )
            {
                m_pDLGSelectedItem->SetPos( m_pNowOverItemSlot->GetPos() );
                m_pDLGSelectedItem->GetStatic_LUA( "SelectedItem" )->GetPicture(0)->SetSize( m_pNowOverItemSlot->GetSize() );

                bCheck = true;
            }
        }
    }

    return bCheck;
}



bool CX2PostBox::MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{   
    if(m_bShow == false)
        return false;

    CX2SlotManager::MsgProc( hWnd, uMsg, wParam, lParam );

    switch( uMsg )
    {
    case WM_RBUTTONDOWN:
        {
            POINT mousePoint = { short(LOWORD(lParam)), short(HIWORD(lParam)) };
            D3DXVECTOR2 mousePos = D3DXVECTOR2( (float)mousePoint.x, (float)mousePoint.y );

            MouseRButtonUp( mousePos );
        }
        break;
    }



#ifdef POSTBOX_NOTICE
	bool bFlag = false;

	CKTDGUIDialog* pDialog = g_pData->GetUIManager()->GetUIPostBoxWarning();
	if( pDialog != NULL)
	{
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

				if ( false == pDialog->GetIsMouseOver())
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
				if ( false == pDialog->GetIsMouseOver())
				{
					return false;
				}
				bFlag = true;

				g_pData->GetUIManager()->SetUILayerIndex(CX2UIManager::UI_MENU_POSTBOX_NOTICE, true);		
			}
			break;
		default:
			break;
		}
	}
#endif POSTBOX_NOTICE
#if 0
	
	switch( uMsg )
	{
	case WM_RBUTTONUP:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
		{
			// 드래깅 끝날 때의 처리 : 임시 코드
			CX2SlotManager::CX2Slot** pSlotBeforeDragging = (g_pData->GetSlotBeforeDragging());
			{
				if ( *pSlotBeforeDragging != NULL )
				{					
					(*pSlotBeforeDragging)->ResetPos();

					if ( (*pSlotBeforeDragging)->GetDialog() != NULL )
						(*pSlotBeforeDragging)->GetDialog()->SetColor( D3DXCOLOR(1,1,1,1) );
				}

				*pSlotBeforeDragging = NULL;				
			}
			SetInventorySort();
		}
	default:
		break;
	}	
#endif

#ifdef POSTBOX_NOTICE
    return bFlag;
#else
	return false;
#endif POSTBOX_NOTICE

}

void CX2PostBox::SetSlotShowEnable(bool bShow, int flag)
{
    // 우선 bShow는 true만 온다..

	CX2ItemSlotManager::SetShow(false);
	CX2ItemSlotManager::SetEnable(false);   

    switch(flag)
    {
    case 0: // receive        
        break;
    case 1: // send
        //SetShow(CX2Slot::ST_INVENTORY, bShow);

        CX2ItemSlotManager::SetShow(CX2Slot::ST_POST_SEND, bShow);        
        //SetEnable(CX2Slot::ST_INVENTORY, bShow);
        CX2ItemSlotManager::SetEnable(CX2Slot::ST_POST_SEND, bShow);        
        break;
    case 2: // setting
        break;
    case 3:
        CX2ItemSlotManager::SetShow(CX2Slot::ST_POST_RECEIVE, bShow);
        CX2ItemSlotManager::SetEnable(CX2Slot::ST_POST_RECEIVE, bShow);
        break;    
    default:
        break;
    }    
}

void CX2PostBox::SetDialogShow( bool bShow, int flag )
{
    if(m_nTabIndex == flag)
        return;

    m_nTabIndex = flag;

    CKTDGUIDialogType pDialog;    
    CKTDGUIButton *pEnter, *pTrans, *pOption;
    
	if(g_pData != NULL && g_pData->GetUIManager() != NULL && g_pData->GetUIManager()->GetUIInventory() != NULL)
	{
		g_pData->GetUIManager()->GetUIInventory()->UpdateUIButton();
	}

    switch(flag)
    {
    case 0: // Recieve
        pDialog = m_pDlgMailReceive;
        m_pDLGMailSend->SetShowEnable(false, false);
        m_pDlgMailListSlot->SetShowEnable(true, true);
        m_pDlgMailView->SetShowEnable(false, false);
        m_pDlgMailSetting->SetShowEnable(false, false); 
        m_CurrentState = 0;

        pEnter = (CKTDGUIButton*)m_pDlgMailReceive->GetControl(L"enter");
        pTrans = (CKTDGUIButton*)m_pDlgMailReceive->GetControl(L"trans");
        pOption = (CKTDGUIButton*)m_pDlgMailReceive->GetControl(L"option");

        pEnter->SetDownStateAtNormal(true);
        pTrans->SetDownStateAtNormal(false);
        pOption->SetDownStateAtNormal(false);		

		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer(m_pDlgMailListSlot, XDL_NORMAL_4);
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDlgMailListSlot, false );
		
        break;
    case 1: // Send
        pDialog = m_pDLGMailSend;
        m_pDlgMailReceive->SetShowEnable(false, false);
        m_pDlgMailListSlot->SetShowEnable(false, false);
        m_pDlgMailView->SetShowEnable(false, false);
        m_pDlgMailSetting->SetShowEnable(false, false);
        m_CurrentState = 1;

        pEnter = (CKTDGUIButton*)m_pDLGMailSend->GetControl(L"enter");
        pTrans = (CKTDGUIButton*)m_pDLGMailSend->GetControl(L"trans");
        pOption = (CKTDGUIButton*)m_pDLGMailSend->GetControl(L"option");

        pEnter->SetDownStateAtNormal(false);
        pTrans->SetDownStateAtNormal(true);
        pOption->SetDownStateAtNormal(false);

		//SetSlotManagerLayer( XDL_NORMAL_4 );
		//SetSlotManagerChangeSequence(true);		
		//g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer(m_pDLGMailSend, XDL_NORMAL_3);
		//g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDLGMailSend, false );			
        break;
    case 2: // Setting
        pDialog = m_pDlgMailSetting;
        m_pDLGMailSend->SetShowEnable(false, false);
        m_pDlgMailReceive->SetShowEnable(false, false);
        m_pDlgMailListSlot->SetShowEnable(false, false);
        m_pDlgMailView->SetShowEnable(false, false);
        m_CurrentState = 2;

        pEnter = (CKTDGUIButton*)m_pDlgMailSetting->GetControl(L"enter");
        pTrans = (CKTDGUIButton*)m_pDlgMailSetting->GetControl(L"trans");
        pOption = (CKTDGUIButton*)m_pDlgMailSetting->GetControl(L"option");

        pEnter->SetDownStateAtNormal(false);
        pTrans->SetDownStateAtNormal(false);
        pOption->SetDownStateAtNormal(true);	

		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer(m_pDLGAddBlackList, XDL_NORMAL_4);
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDLGAddBlackList, false );
        break;
    default:
        return;
    }    

    SetSlotShowEnable(bShow, flag);

    if(pDialog != NULL)
    {
        pDialog->SetShowEnable( bShow, bShow );

		SetSlotManagerLayer( XDL_NORMAL_4 );
		SetSlotManagerChangeSequence(true);		
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer(pDialog, XDL_NORMAL_3);
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( pDialog, false );
    }

    if(m_pDLGMyTradeWindow != NULL && bShow == false)
    {
        m_pDLGMyTradeWindow->SetShowEnable(bShow, bShow);
    }

    if(bShow == true)
    {
        if(flag == 1)
        {
            InitPostSend();
			if(g_pData->GetUIManager()->GetUIInventory() != NULL)
			{
                g_pData->GetUIManager()->GetUIInventory()->SetInventorySort( CX2Inventory::ST_EQUIP );
			}
            //SetInventorySort( CX2Inventory::ST_EQUIP );        

			//{{ 허상형 : [2009/7/31] //	없는 컨트롤 부름으로 인한 주석처리
            //CKTDGUIRadioButton* pRadioButton = (CKTDGUIRadioButton*) m_pDLGMailSend->GetControl( L"RadioButtonUnit_Inventory_Equip" );
            //if( NULL != pRadioButton )
            //{
            //    pRadioButton->SetChecked( true );
            //}

            //CKTDGUIStatic* pStatic_ED = (CKTDGUIStatic*)m_pDLGMailSend->GetControl( L"Static_ED" );
            //if ( pStatic_ED != NULL && pStatic_ED->GetString(0) != NULL)
            //{
            //    pStatic_ED->GetString(0)->msg = g_pMain->GetEDString( g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_ED );
            //}
			//}} 허상형 : [2009/7/31] //	없는 컨트롤 부름으로 인한 주석처리

			//g_pData->GetUIManager()->GetUIInventory()->SetPosition(D3DXVECTOR2(-500.f, 0.f), false);
			g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU_INVEN, true);
			//g_pData->GetUIManager()->GetUIInventory()->SetShow(true);
#ifdef POSTBOX_NOTICE
			g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU_POSTBOX_NOTICE, true);
#endif POSTBOX_NOTICE
        }
		else
		{
			//g_pData->GetUIManager()->GetUIInventory()->SetPosition(D3DXVECTOR2(0.f, 0.f), false);
			g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU_INVEN, false);
			//g_pData->GetUIManager()->GetUIInventory()->SetShow(false);
#ifdef POSTBOX_NOTICE
			g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU_POSTBOX_NOTICE, true);
#endif POSTBOX_NOTICE
		}
	}
	else
	{
		//g_pData->GetUIManager()->GetUIInventory()->SetPosition(D3DXVECTOR2(0.f, 0.f), false);
		g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU_INVEN, false);
		//g_pData->GetUIManager()->GetUIInventory()->SetShow(false);
#ifdef POSTBOX_NOTICE
		g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU_POSTBOX_NOTICE, false);
#endif POSTBOX_NOTICE
	}


    switch(flag)
    {
    case 0:
        // 수신 목록 뿌리기
#ifdef SERV_RELATIONSHIP_SYSTEM
		if(m_nUnReadSystem > 0)
			m_cLetterListType = PLT_SYSTEM;
		else if(m_nUnReadCommon > 0)
			m_cLetterListType = PLT_NORMAL;
		else if ( m_nUnReadWedding > 0 )
			m_cLetterListType = PLT_SYSTEM; // PLT_WEDDING;
#else  // SERV_RELATIONSHIP_SYSTEM
		if(m_nUnReadSystem > 0)
			m_bSystemMail = true;
		else if(m_nUnReadCommon > 0)
			m_bSystemMail = false;
#endif // SERV_RELATIONSHIP_SYSTEM    

 
        RequestList();
        break;
    case 1:
        if(bShow == true)
        {
            CKTDGUICheckBox *pCheckDlg;
            pCheckDlg = (CKTDGUICheckBox*)m_pDLGMailSend->GetControl(L"Check_SystemMessage");
            CKTDGUIStatic *pStaticDlg = m_pDLGMailSend->GetStatic_LUA("g_pStaticPost_SendWindow");
            CKTDGUIControl::CPictureData *pCheckPic;

            if(pStaticDlg != NULL)
            {
                pCheckPic = pStaticDlg->GetPictureIndex(5);              

                if(pCheckDlg != NULL && pCheckPic != NULL)
                {
                    if ( g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_OPERATOR )
                    {
                        pCheckDlg->SetShow( true );
                        pCheckDlg->SetEnable( true );   
                        pCheckPic->SetShow(true);
                    }
                    else
                    {
                        pCheckDlg->SetShow( false );
                        pCheckDlg->SetEnable( false );
                        pCheckPic->SetShow(false);
                    }
                }
            }

        }
        break;
    case 2:
        // 차단목록 뿌리기
        if(m_bBuffBList == false)
            BlackListReq();
        else
            ShowBlackList();

        break;
    }
}

void CX2PostBox::SetDialogShow( bool bShow )
{
	// 편지알림 갱신
	if(m_bInit == true && m_bShow == true && bShow == false)
	{
		Handler_EGS_RENEWAL_LETTER_REQ();		
	}

	m_bInit = bShow;

    m_bShow = bShow;
    m_bEnable = bShow; 

    if(bShow == true)
    {
        // note!! special case

#ifdef CHECK_INHERIT_STATE_MENU
		if( g_pMain->IsInheritStateMenu() )
#endif //CHECK_INHERIT_STATE_MENU
		{
			CX2StateMenu* pStateMenu = (CX2StateMenu*) g_pMain->GetNowState();
			if( NULL != pStateMenu )
			{
				pStateMenu->DeleteMouseClickUI();
			}
		}

        SetDialogShow(bShow, 0);                    
    }
    else
    {
        m_bSwitch = false;

        if( g_pMain->GetNowStateID() != CX2Main::XS_DUNGEON_ROOM &&
            g_pMain->GetNowStateID() != CX2Main::XS_PVP_ROOM )
        {
            m_bCheckPitIn = false;
        }

        m_nTabIndex = -1;
        
        if(m_pDLGQuery != NULL)
        {
            g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGQuery, NULL, false );
            m_pDLGQuery = NULL;
        }
        
        if(m_pProcessDlg != NULL)
        {
            g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pProcessDlg, NULL, false );
            m_pProcessDlg = NULL;
        }
        
        m_bProcess = false;

        if(m_pDLGMailSend != NULL)
            m_pDLGMailSend->SetShowEnable(false, false);
        if(m_pDlgMailReceive != NULL)
            m_pDlgMailReceive->SetShowEnable(false, false);
        if(m_pDlgMailSetting != NULL)
            m_pDlgMailSetting->SetShowEnable(false, false);        
        if(m_pDlgMailListSlot != NULL)
            m_pDlgMailListSlot->SetShowEnable(false, false);
        if(m_pDlgMailView != NULL)
            m_pDlgMailView->SetShowEnable(false, false);
//         if(m_pDlgPostBox)
//             m_pDlgPostBox->SetShowEnable(false, false);
        if(m_pTalkBoxManager)
            m_pTalkBoxManager->Clear();      

        if(m_pDLGMyTradeWindow != NULL)
            m_pDLGMyTradeWindow->SetShowEnable(false, false);
        if(m_pDLGAddBlackList != NULL)
            m_pDLGAddBlackList->SetShowEnable(false, false);

#ifdef SERV_RELATIONSHIP_SYSTEM
		m_cLetterListType = PLT_SYSTEM;
#else  // SERV_RELATIONSHIP_SYSTEM
		m_bSystemMail = true;
#endif // SERV_RELATIONSHIP_SYSTEM    
        

		CX2ItemSlotManager::SetEnable( false );
        CX2ItemSlotManager::SetShow( false );

    
        // 툴팁 제거
		SetNowOverItemSlotAndDesc( NULL );
        m_pDLGSelectedItem->SetShow( false );
        if ( m_pDLGItemDesc != NULL )
            m_pDLGItemDesc->SetShow( false );

        if ( m_pDLGItemDescCompare != NULL )
            m_pDLGItemDescCompare->SetShow( false );        

		if(g_pData != NULL && g_pData->GetUIManager() != NULL && g_pData->GetUIManager()->GetUIInventory() != NULL)
		{
			//g_pData->GetUIManager()->GetUIInventory()->SetPosition(D3DXVECTOR2(0.f, 0.f), false);			
			g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU_INVEN, false);
			//g_pData->GetUIManager()->GetUIInventory()->SetShow(false);
			g_pData->GetUIManager()->GetUIInventory()->UpdateUIButton();

#ifdef POSTBOX_NOTICE			
			g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU_POSTBOX_NOTICE, false);
#endif POSTBOX_NOTICE
		}
    }    
}


int CX2PostBox::AddMultiLine()
{    
    CKTDGUIIMEEditBox *newLine = NULL;
    CKTDGUIIMEEditBox *currentLine = (CKTDGUIIMEEditBox*)(m_pDLGMailSend->GetControl(L"g_pIMEEditBox_Comment"));    

    for ( int i = 0; i < m_pDLGMailSend->GetControlSize(); i++ )
    {
        if ( m_pDLGMailSend->GetControl(i) != NULL &&
            m_pDLGMailSend->GetControl(i)->GetType() == CKTDGUIControl::UCT_IME_EDITBOX )
        {
            CKTDGUIIMEEditBox* pIMEEditBox = (CKTDGUIIMEEditBox*)m_pDLGMailSend->GetControl(i);
            if ( pIMEEditBox->GetGroupID() == currentLine->GetGroupID() )
            {
                if ( pIMEEditBox->GetGroupSequence() == currentLine->GetGroupSequence() + 1 )
                {
                    newLine = pIMEEditBox;
                    break;
                }
            }
        }
    }

    if(newLine != NULL)
    {
        newLine->RequestFocus();
        newLine->OnFocusIn();
    }

    return newLine->GetGroupSequence();
}

void CX2PostBox::ResetInvenPage( int nowPage, int maxPage )
{
    m_NowInvenSortTypePageNum = nowPage;
    m_NowInvenSortTypePageMaxNum = maxPage;

    //ResetInvenPageUI();
}

void CX2PostBox::ResetInvenPageUI()
{
    //m_NowInvenSortTypePageMaxNum = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetInvenSlot() m_NowInventorySortType
    //int invenMaxSize = GetInvenMaxSize( m_NowInventorySortType );

    /*if ( m_pDLGMailSend != NULL )
    {
        CKTDGUIStatic* pStatic = (CKTDGUIStatic*)m_pDLGMailSend->GetControl( L"StaticMy_Inventory_Page2" );
        if ( pStatic != NULL )
        {
            wstringstream wstrstm;
            wstrstm << m_NowInvenSortTypePageNum << L"/" << m_NowInvenSortTypePageMaxNum;
            pStatic->SetString( 0, wstrstm.str().c_str() );
        }
    }*/
}


wstring CX2PostBox::GetSlotItemDesc()
{
    wstring itemDesc=  L"";

    if ( m_pNowOverItemSlot != NULL )
    {
        CX2Item* pkItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( m_pNowOverItemSlot->GetItemUID() );
        if ( m_CurrentState == 1 && pkItem != NULL )
            itemDesc = GetSlotItemDescByUID( m_pNowOverItemSlot->GetItemUID() );
        else if(m_CurrentState != 1)
        {
            int inx = -1;

            for(int i=0; i<6; ++i)
            {
                if(m_listSlot[i].m_bSelected == true)
                {
                    inx = i;
                    break;
                }
            }

            if( inx > -1 && m_listSlot[inx].m_ItemId > 0 )
            {                
#ifdef SERV_RELATIONSHIP_SYSTEM
				if( IsSystemMail() == true)
#else  // SERV_RELATIONSHIP_SYSTEM
				if( m_bSystemMail == true)
#endif // SERV_RELATIONSHIP_SYSTEM    
                {   
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
					if( KPostItemInfo::IsItemIDLetter( static_cast<char>(m_listSlot[inx].m_iKind) ) == false )
#else
					if(m_listSlot[inx].m_iKind != KPostItemInfo::LT_MESSAGE && 
						m_listSlot[inx].m_iKind != KPostItemInfo::LT_BROKEN_ITEM &&
						m_listSlot[inx].m_iKind != KPostItemInfo::LT_WEB_POINT_EVENT)
#endif //SERV_ENTER_FIELD_QUEST_CLEAR
					{
						itemDesc = GET_STRING( STR_ID_397 );
					}   
                    else
					{
						if(m_listSlot[inx].m_ItemId == 127000 || m_listSlot[inx].m_ItemId == 127040)
						{
							wstring wstrED = g_pMain->GetEDString( m_listSlot[inx].m_nAttachMoney );
							itemDesc = GET_REPLACED_STRING( ( STR_ID_398, "L", wstrED ) );
						}
						else
							itemDesc = GetSlotItemDescByTID( m_listSlot[inx].m_pItem, m_listSlot[inx].m_ItemId, true );
					}
                }
                else
                {
                    if(m_listSlot[inx].m_ItemId == 127000 || m_listSlot[inx].m_ItemId == 127040)
                    {
						wstring wstrED = g_pMain->GetEDString( m_listSlot[inx].m_nAttachMoney );
                        itemDesc = GET_REPLACED_STRING( ( STR_ID_398, "L", wstrED ) );;
                    }
                    else
                    {
                        itemDesc = GetSlotItemDescByTID( m_listSlot[inx].m_pItem, m_listSlot[inx].m_ItemId, true );
                    }
                }                
            }
            else
                itemDesc = GetSlotItemDescByTID( pkItem, m_pNowOverItemSlot->GetItemTID() );
        }
    }

    return itemDesc;
}

/*vitual*/ std::wstring CX2PostBox::GetSlotItemDescExtra_RBtn( int itemTID, CX2Item* pItem )
{
    std::wstring slotItemDesc = L"";

    if ( m_bShowRBDownMessage == true )
    {        
        if(m_CurrentState == 0)
        {
            slotItemDesc = GET_STRING( STR_ID_399 );        
        }        
    }

    return slotItemDesc;
}

bool CX2PostBox::OnDropAnyItem( D3DXVECTOR2 mousePos )
{

	if(m_bProcess || m_bRegisteredTrade || m_bAttachItem == false || m_CurrentState == 0)
		return false;

	// 아이템 드래깅중일때
#ifdef NEW_VILLAGE_UI
	if ( *m_pSlotBeforeDragging != NULL )
	{
		// 드래깅 아이템 복원
		(*m_pSlotBeforeDragging)->ResetPos();
		if ( (*m_pSlotBeforeDragging)->GetDialog() != NULL )
		{
			g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer( (*m_pSlotBeforeDragging)->GetDialog(), (*m_pSlotBeforeDragging)->GetNormalLayer() );
			(*m_pSlotBeforeDragging)->GetDialog()->SetColor( D3DXCOLOR(1,1,1,1) );
		}
#else
	if ( m_pSlotBeforeDragging != NULL )
	{
		// 드래깅 아이템 복원
		m_pSlotBeforeDragging->ResetPos();
		if ( m_pSlotBeforeDragging->GetDialog() != NULL )
		{
			g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer( m_pSlotBeforeDragging->GetDialog(), XDL_NORMAL_1 );
			m_pSlotBeforeDragging->GetDialog()->SetColor( D3DXCOLOR(1,1,1,1) );
		}
#endif

		//we-need-any-personal-item-slot-find

		// 마우스업된 위치가 임의의 인벤토리 슬롯인 경우
		// No Change Inven

		int slotIndex = IsInPostItemReact( mousePos );

		if(slotIndex != -1)
		{
			//m_PickedShopItemIndex = slotIndex + (m_iNowPageIndex-1)*3;
#ifdef NEW_VILLAGE_UI
			CX2SlotItem* pSlotItem = (CX2SlotItem*)*m_pSlotBeforeDragging;
#else
			CX2SlotItem* pSlotItem = (CX2SlotItem*)m_pSlotBeforeDragging;
#endif

			//if ( CheckDoubleShopItem( pSlotItem->GetItemUID(), m_PickedShopItemIndex ) == true )
			//{
			//    g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), L"같은 아이템을 중복해서 판매할 수 없습니다.", m_pNowState );
			//}
			//else
			{
				bool bCheck = true;
				CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( pSlotItem->GetItemUID() );

				switch( pItem->GetItemTemplet()->GetPeriodType() )
				{
				default:
					{
						ASSERT( !"invalid period type" );
					} break;
				case CX2Item::PT_INFINITY:
				case CX2Item::PT_QUANTITY:
					{
						// 내구도 확인 하지 않음.
					} break;
				case CX2Item::PT_ENDURANCE:
					{
						if( pItem->GetItemData().m_Endurance < pItem->GetItemTemplet()->GetEndurance() )
						{
							bCheck = false;
							g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250, 300), GET_STRING( STR_ID_395 ), (CKTDXStage*) m_pNowState );
							return false;
						}
					} break;
				}

	//{{ kimhc // 2009-09-03 // 봉인된 아이템 우편 전송 가능 하도록
#ifdef	SEAL_ITEM
				if ( bCheck == true && pItem->GetItemTemplet()->GetVested() == true 
					&& pItem->GetItemData().m_bIsSealed == false )
#else	SEAL_ITEM
				if ( bCheck == true && pItem->GetItemTemplet()->GetVested() == true )
#endif	SEAL_ITEM
	//}} kimhc // 2009-09-03 // 봉인된 아이템 우편 전송 가능 하도록
				{
					bCheck = false;
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_393 ),m_pNowState );
				}   

				if(bCheck == true)
				{
					// 미리보기 아이템 셋팅
					SetSlotItem(pItem);   
					m_pPostSlot = pSlotItem;
				}
			}
			
		}           
		else
		{
#ifdef NEW_VILLAGE_UI
			if((*m_pSlotBeforeDragging)->GetSlotType() == CX2SlotItem::ST_POST_SEND)
#else
			if(m_pSlotBeforeDragging->GetSlotType() == CX2SlotItem::ST_POST_SEND)
#endif
			{
				if(m_pPostSlot != NULL)
				{
					m_pPostSlot = NULL;
					CX2SlotItem* pSlotItem = (CX2SlotItem*)GetSlot( CX2SlotItem::ST_POST_SEND, 100 );
					if ( pSlotItem != NULL )
					{
						pSlotItem->DestroyItemUI();
						m_iItemUid = -1;
						g_pKTDXApp->GetDeviceManager()->PlaySound( L"Post_ItemDrop.ogg", false, false );  
						CheckEdChange();
					}

					m_bRegisteredTrade = false;
					CKTDGUIIMEEditBox *pDialogEd = (CKTDGUIIMEEditBox*)m_pDLGMailSend->GetControl(L"IME_Add_Ed");
					pDialogEd->SetEnable(true);

					CKTDGUIStatic *pStaticDlg = m_pDLGMailSend->GetStatic_LUA("g_pStaticPost_SendWindow");
					CKTDGUIControl::CPictureData *pEdPic1, *pEdPic2;
					pEdPic1 = pStaticDlg->GetPictureIndex(1);  
					if(pEdPic1 != NULL)
						pEdPic1->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
					pEdPic2 = pStaticDlg->GetPictureIndex(2);  
					if(pEdPic2 != NULL)
						pEdPic2->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

				}
			}
		}

		SetInventorySort();
#ifdef NEW_VILLAGE_UI
		//*m_pSlotBeforeDragging = NULL;
		//*m_DraggingItemUID = -1;

		return true;
	}

	*m_pSlotBeforeDragging = NULL;
	*m_DraggingItemUID = -1;
#else
		m_pSlotBeforeDragging = NULL;
		m_DraggingItemUID = -1;

		return true;
	}

	m_pSlotBeforeDragging = NULL;
	m_DraggingItemUID = -1;

#endif
	return false;

}

bool CX2PostBox::SetInventorySort()
{
	if(g_pData->GetUIManager()->GetUIInventory() == NULL)
	{
		g_pData->GetUIManager()->CreateUIInventory();
	}

	bool bResult = g_pData->GetUIManager()->GetUIInventory()->UpdateInventorySlot();
	
	//bResult = g_pData->GetUIManager()->GetUIInventory()->SetInventorySort(g_pData->GetUIManager()->GetUIInventory()->GetSortType());//m_NowInventorySortType);	
    return bResult;
}

bool CX2PostBox::UpdateInventorySlotList( std::vector< KInventoryItemInfo >& vecInventorySlotInfo )
{
    for ( int i = 0; i < (int)vecInventorySlotInfo.size(); i++ )
    {
        KInventoryItemInfo& kInventorySlotInfo = vecInventorySlotInfo[i];
        if ( kInventorySlotInfo.m_cSlotCategory == CX2Inventory::ST_E_EQUIP )
        {
            CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( (CX2Inventory::SORT_TYPE)kInventorySlotInfo.m_cSlotCategory, kInventorySlotInfo.m_sSlotID );
            if ( pItem != NULL )
            {				
                RemoveEqip( pItem->GetUID() );
            }
        }
    }

    for ( int i = 0; i < (int)vecInventorySlotInfo.size(); i++ )
    {
        KInventoryItemInfo& kInventorySlotInfo = vecInventorySlotInfo[i];
        g_pData->GetMyUser()->GetSelectUnit()->AccessInventory().RemoveItem( (CX2Inventory::SORT_TYPE)kInventorySlotInfo.m_cSlotCategory, kInventorySlotInfo.m_sSlotID );
    }

    for ( int i = 0; i < (int)vecInventorySlotInfo.size(); i++ )
    {
        KInventoryItemInfo& kInventorySlotInfo = vecInventorySlotInfo[i];
        if ( kInventorySlotInfo.m_iItemUID > 0 )
        {
            CX2Item::ItemData kItemData( kInventorySlotInfo );
            g_pData->GetMyUser()->GetSelectUnit()->AccessInventory().AddItem( (CX2Inventory::SORT_TYPE)kInventorySlotInfo.m_cSlotCategory, kInventorySlotInfo.m_sSlotID, kItemData );

            if ( kInventorySlotInfo.m_cSlotCategory == CX2Inventory::ST_E_EQUIP )
            {
                CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( (CX2Inventory::SORT_TYPE)kInventorySlotInfo.m_cSlotCategory, kInventorySlotInfo.m_sSlotID );
                if ( pItem != NULL )
                {
                    AddEqip( pItem->GetUID() );
                }
            }
        }
    }

#if 0 
    if ( SetInventorySort( m_NowInventorySortType ) == false )
        return false;
#endif



	if( NULL != g_pData->GetQuestManager() )
	{
		g_pData->GetQuestManager()->GiveUpForbiddenQuest();
	}

    return true;
}

void CX2PostBox::RemoveEqip(UidType ItemUID)
{
    g_pData->GetMyUser()->GetSelectUnit()->RemoveEqip( ItemUID );
}

void CX2PostBox::AddEqip(UidType ItemUID)
{
    g_pData->GetMyUser()->GetSelectUnit()->AddEqip( ItemUID );

    
}

void CX2PostBox::OpenItemQuantityDLG( UidType sellItemUID )
{   
    m_nQuantity	= 1;   

    CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( sellItemUID );
    if ( pItem != NULL )
    {
        WCHAR buff[256] = {0};
        CKTDGUIStatic* pStaticItemName = (CKTDGUIStatic*)m_pDLGMyTradeWindow->GetControl( L"Item_Name" );
        if ( pStaticItemName != NULL && pStaticItemName->GetString(0) != NULL )
        {
            wstring wstrItemName( pItem->GetItemTemplet()->GetFullName_() );
			//{{ kimhc // 2009-11-26 // 아이템 등록시 제목에서 아이템 이름에 포함된 ' 제거
			wstring::iterator wstrItr = std::remove( wstrItemName.begin(), wstrItemName.end(), L'\'' );

			if ( wstrItr != wstrItemName.end() )
				wstrItemName.erase( wstrItr, wstrItemName.end() );
			//}} kimhc // 2009-11-26 // 아이템 등록시 제목에서 아이템 이름에 포함된 ' 제거

			pStaticItemName->GetString(0)->msg = wstrItemName;
        }

        WCHAR buff1[256] = {0};        

        CKTDGUIStatic *pStaticDlg = (CKTDGUIStatic*)m_pDLGMyTradeWindow->GetControl(L"Item_Num");
        if ( pStaticDlg != NULL )
        {
			StringCchPrintf( buff1, 256, L"%d", 1 );
            //wsprintf( buff1, L"%d", 1 );
            pStaticDlg->GetString(0)->msg = buff1;
        }

    }
}


void CX2PostBox::SetItemNumChange( int iNum )
{
	CX2Item* pItemData = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( m_iItemUid );
	if( pItemData != NULL )
    {
        int maxItemSellNum = pItemData->GetItemData().m_Quantity;
        m_nQuantity += iNum;

        if ( m_nQuantity <= 1 )
            m_nQuantity = 1;

        if ( m_nQuantity >= maxItemSellNum )
            m_nQuantity = maxItemSellNum;

        WCHAR buff[256] = {0};
        CKTDGUIStatic *pStaticDlg = (CKTDGUIStatic*)m_pDLGMyTradeWindow->GetControl(L"Item_Num");
        if ( pStaticDlg != NULL )
        {
			StringCchPrintf( buff, 255, L"%d", m_nQuantity );
            //wsprintf( buff, L"%d", m_nQuantity );
            pStaticDlg->GetString(0)->msg = buff;
        }       
    }
}

// 리스트 슬롯 관련
void CX2PostBox::ClearMailList()
{
    for(int i=0; i<6; ++i)
    {
        m_listSlot[i].m_bAttachItem = false;
        m_listSlot[i].m_nAttachMoney = 0;
        m_listSlot[i].m_bCache = false;
        m_listSlot[i].m_bChecked = false;
        m_listSlot[i].m_bRead = false;
        m_listSlot[i].m_bSelected = false;
        m_listSlot[i].m_ItemId = 0;
        m_listSlot[i].m_strDate = L"";
        m_listSlot[i].m_strNickName = L"";
        m_listSlot[i].m_strTitle = L"";
        m_listSlot[i].m_postID = 0;
        m_listSlot[i].m_iKind = 0;

        SAFE_DELETE( m_listSlot[i].m_pItem );
        m_listSlot[i].m_pItem = NULL;
    }

    m_nMaxSlot = 0;    
}
	
void CX2PostBox::ClearListSlot()
{    

    CKTDGUIStatic *pStaticDlg1, *pStaticDlg2, *pStaticDlg3;
    CKTDGUIButton *pButtonDlg;
    CKTDGUICheckBox *pCheckDlg;
    CKTDGUIStatic *pStaticDlgRead = m_pDlgMailListSlot->GetStatic_LUA("Staticpost_ItemSlot_Read");
    CKTDGUIStatic *pStaticDlgNotRead = m_pDlgMailListSlot->GetStatic_LUA("Staticpost_ItemSlot_NotRead");
    CKTDGUIControl::CPictureData *pReadPic, *pNotReadPic;


//{{ 김상훈 : 2010.10.05 전체선택 체크박스
#ifdef POST_SELECT_ALL_MAILS  
	pCheckDlg   = (CKTDGUICheckBox*)m_pDlgMailListSlot->GetControl(L"g_pCheckBoxpost_window_Slot7");   
	pCheckDlg->SetChecked(false);
	pCheckDlg->SetEnable(true);
#endif POST_SELECT_ALL_MAILS
//}} 김상훈 : 2010.10.05 전체선택 체크박스

    for(int i=0; i<6; ++i)
    {
        switch(i)
        {
        case 0:                        
            pStaticDlg1 = (CKTDGUIStatic*)m_pDlgMailListSlot->GetControl(L"g_pStatic_message1");    // 날짜 + 보낸이            
            pStaticDlg2 = (CKTDGUIStatic*)m_pDlgMailListSlot->GetControl(L"g_pStatic_message2");    // 제목            
            pStaticDlg3 = (CKTDGUIStatic*)m_pDlgMailListSlot->GetControl(L"g_pStatic_message3");    // 첨부
            pButtonDlg  = (CKTDGUIButton*)m_pDlgMailListSlot->GetControl(L"Slot_Button1");          // 버튼
            pCheckDlg   = (CKTDGUICheckBox*)m_pDlgMailListSlot->GetControl(L"g_pCheckBoxpost_window_Slot1");        // 체크박스
            pReadPic    = pStaticDlgRead->GetPictureIndex(1);
            pNotReadPic = pStaticDlgNotRead->GetPictureIndex(1);
            break;
        case 1:            
            pStaticDlg1 = (CKTDGUIStatic*)m_pDlgMailListSlot->GetControl(L"g_pStatic_message4");            
            pStaticDlg2 = (CKTDGUIStatic*)m_pDlgMailListSlot->GetControl(L"g_pStatic_message5");            
            pStaticDlg3 = (CKTDGUIStatic*)m_pDlgMailListSlot->GetControl(L"g_pStatic_message6");
            pButtonDlg  = (CKTDGUIButton*)m_pDlgMailListSlot->GetControl(L"Slot_Button2");
            pCheckDlg   = (CKTDGUICheckBox*)m_pDlgMailListSlot->GetControl(L"g_pCheckBoxpost_window_Slot2");        // 체크박스
            pReadPic    = pStaticDlgRead->GetPictureIndex(2);
            pNotReadPic = pStaticDlgNotRead->GetPictureIndex(2);
            break;
        case 2:            
            pStaticDlg1 = (CKTDGUIStatic*)m_pDlgMailListSlot->GetControl(L"g_pStatic_message7");            
            pStaticDlg2 = (CKTDGUIStatic*)m_pDlgMailListSlot->GetControl(L"g_pStatic_message8");            
            pStaticDlg3 = (CKTDGUIStatic*)m_pDlgMailListSlot->GetControl(L"g_pStatic_message9");
            pButtonDlg  = (CKTDGUIButton*)m_pDlgMailListSlot->GetControl(L"Slot_Button3");
            pCheckDlg   = (CKTDGUICheckBox*)m_pDlgMailListSlot->GetControl(L"g_pCheckBoxpost_window_Slot3");        // 체크박스
            pReadPic    = pStaticDlgRead->GetPictureIndex(3);
            pNotReadPic = pStaticDlgNotRead->GetPictureIndex(3);
            break;
        case 3:            
            pStaticDlg1 = (CKTDGUIStatic*)m_pDlgMailListSlot->GetControl(L"g_pStatic_message10");            
            pStaticDlg2 = (CKTDGUIStatic*)m_pDlgMailListSlot->GetControl(L"g_pStatic_message11");            
            pStaticDlg3 = (CKTDGUIStatic*)m_pDlgMailListSlot->GetControl(L"g_pStatic_message12");
            pButtonDlg  = (CKTDGUIButton*)m_pDlgMailListSlot->GetControl(L"Slot_Button4");
            pCheckDlg   = (CKTDGUICheckBox*)m_pDlgMailListSlot->GetControl(L"g_pCheckBoxpost_window_Slot4");        // 체크박스
            pReadPic    = pStaticDlgRead->GetPictureIndex(4);
            pNotReadPic = pStaticDlgNotRead->GetPictureIndex(4);
            break;
        case 4:            
            pStaticDlg1 = (CKTDGUIStatic*)m_pDlgMailListSlot->GetControl(L"g_pStatic_message13");            
            pStaticDlg2 = (CKTDGUIStatic*)m_pDlgMailListSlot->GetControl(L"g_pStatic_message14");            
            pStaticDlg3 = (CKTDGUIStatic*)m_pDlgMailListSlot->GetControl(L"g_pStatic_message15");
            pButtonDlg  = (CKTDGUIButton*)m_pDlgMailListSlot->GetControl(L"Slot_Button5");
            pCheckDlg   = (CKTDGUICheckBox*)m_pDlgMailListSlot->GetControl(L"g_pCheckBoxpost_window_Slot5");        // 체크박스
            pReadPic    = pStaticDlgRead->GetPictureIndex(5);
            pNotReadPic = pStaticDlgNotRead->GetPictureIndex(5);
            break;
        case 5:            
            pStaticDlg1 = (CKTDGUIStatic*)m_pDlgMailListSlot->GetControl(L"g_pStatic_message16");            
            pStaticDlg2 = (CKTDGUIStatic*)m_pDlgMailListSlot->GetControl(L"g_pStatic_message17");            
            pStaticDlg3 = (CKTDGUIStatic*)m_pDlgMailListSlot->GetControl(L"g_pStatic_message18");
            pButtonDlg  = (CKTDGUIButton*)m_pDlgMailListSlot->GetControl(L"Slot_Button6");
            pCheckDlg   = (CKTDGUICheckBox*)m_pDlgMailListSlot->GetControl(L"g_pCheckBoxpost_window_Slot6");        // 체크박스
            pReadPic    = pStaticDlgRead->GetPictureIndex(6);
            pNotReadPic = pStaticDlgNotRead->GetPictureIndex(6);
            break;
        }       
        
        pStaticDlg1->GetString(0)->msg = L"";
        pStaticDlg2->GetString(0)->msg = L"";
        pStaticDlg3->GetString(0)->msg = L"";
        
        pButtonDlg->SetEnable(false);
        pButtonDlg->SetDownStateAtNormal(false);
        pButtonDlg->SetColor(D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

        pCheckDlg->SetChecked(false);
        pCheckDlg->SetEnable(false);

        pReadPic->SetShow(false);
        pNotReadPic->SetShow(false);
        // disable
    }
}

void CX2PostBox::SetListSlot(int inx)
{
    if(inx >= m_nMaxSlot)
        return;

    //ClearListSlot();

    CKTDGUIStatic	*pStaticDlg1 = NULL;
	CKTDGUIStatic	*pStaticDlg2 = NULL;
	CKTDGUIStatic	*pStaticDlg3 = NULL;
    CKTDGUIButton	*pButtonDlg = NULL;
    CKTDGUICheckBox *pCheckDlg = NULL;
    CKTDGUIStatic *pStaticDlg = m_pDLGMailSend->GetStatic_LUA("g_pStaticPost_SendWindow");

    CKTDGUIStatic *pStaticDlgRead = m_pDlgMailListSlot->GetStatic_LUA("Staticpost_ItemSlot_Read");
    CKTDGUIStatic *pStaticDlgNotRead = m_pDlgMailListSlot->GetStatic_LUA("Staticpost_ItemSlot_NotRead");
    CKTDGUIControl::CPictureData *pReadPic = NULL;
	CKTDGUIControl::CPictureData *pNotReadPic = NULL;
    
    switch(inx)
    {
    case 0:                        
        pStaticDlg1 = (CKTDGUIStatic*)m_pDlgMailListSlot->GetControl(L"g_pStatic_message1"); // 날짜 + 보낸이            
        pStaticDlg2 = (CKTDGUIStatic*)m_pDlgMailListSlot->GetControl(L"g_pStatic_message2"); // 제목            
        pStaticDlg3 = (CKTDGUIStatic*)m_pDlgMailListSlot->GetControl(L"g_pStatic_message3"); // 첨부
        pButtonDlg  = (CKTDGUIButton*)m_pDlgMailListSlot->GetControl(L"Slot_Button1");
        pCheckDlg   = (CKTDGUICheckBox*)m_pDlgMailListSlot->GetControl(L"g_pCheckBoxpost_window_Slot1");        // 체크박스
        pReadPic    = pStaticDlgRead->GetPictureIndex(1);
        pNotReadPic = pStaticDlgNotRead->GetPictureIndex(1);
        break;
    case 1:            
        pStaticDlg1 = (CKTDGUIStatic*)m_pDlgMailListSlot->GetControl(L"g_pStatic_message4");            
        pStaticDlg2 = (CKTDGUIStatic*)m_pDlgMailListSlot->GetControl(L"g_pStatic_message5");            
        pStaticDlg3 = (CKTDGUIStatic*)m_pDlgMailListSlot->GetControl(L"g_pStatic_message6");
        pButtonDlg  = (CKTDGUIButton*)m_pDlgMailListSlot->GetControl(L"Slot_Button2");
        pCheckDlg   = (CKTDGUICheckBox*)m_pDlgMailListSlot->GetControl(L"g_pCheckBoxpost_window_Slot2");        // 체크박스
        pReadPic    = pStaticDlgRead->GetPictureIndex(2);
        pNotReadPic = pStaticDlgNotRead->GetPictureIndex(2);
        break;
    case 2:            
        pStaticDlg1 = (CKTDGUIStatic*)m_pDlgMailListSlot->GetControl(L"g_pStatic_message7");            
        pStaticDlg2 = (CKTDGUIStatic*)m_pDlgMailListSlot->GetControl(L"g_pStatic_message8");            
        pStaticDlg3 = (CKTDGUIStatic*)m_pDlgMailListSlot->GetControl(L"g_pStatic_message9");
        pButtonDlg  = (CKTDGUIButton*)m_pDlgMailListSlot->GetControl(L"Slot_Button3");
        pCheckDlg   = (CKTDGUICheckBox*)m_pDlgMailListSlot->GetControl(L"g_pCheckBoxpost_window_Slot3");        // 체크박스
        pReadPic    = pStaticDlgRead->GetPictureIndex(3);
        pNotReadPic = pStaticDlgNotRead->GetPictureIndex(3);
        break;
    case 3:            
        pStaticDlg1 = (CKTDGUIStatic*)m_pDlgMailListSlot->GetControl(L"g_pStatic_message10");            
        pStaticDlg2 = (CKTDGUIStatic*)m_pDlgMailListSlot->GetControl(L"g_pStatic_message11");            
        pStaticDlg3 = (CKTDGUIStatic*)m_pDlgMailListSlot->GetControl(L"g_pStatic_message12");
        pButtonDlg  = (CKTDGUIButton*)m_pDlgMailListSlot->GetControl(L"Slot_Button4");
        pCheckDlg   = (CKTDGUICheckBox*)m_pDlgMailListSlot->GetControl(L"g_pCheckBoxpost_window_Slot4");        // 체크박스
        pReadPic    = pStaticDlgRead->GetPictureIndex(4);
        pNotReadPic = pStaticDlgNotRead->GetPictureIndex(4);
        break;
    case 4:            
        pStaticDlg1 = (CKTDGUIStatic*)m_pDlgMailListSlot->GetControl(L"g_pStatic_message13");            
        pStaticDlg2 = (CKTDGUIStatic*)m_pDlgMailListSlot->GetControl(L"g_pStatic_message14");            
        pStaticDlg3 = (CKTDGUIStatic*)m_pDlgMailListSlot->GetControl(L"g_pStatic_message15");
        pButtonDlg  = (CKTDGUIButton*)m_pDlgMailListSlot->GetControl(L"Slot_Button5");
        pCheckDlg   = (CKTDGUICheckBox*)m_pDlgMailListSlot->GetControl(L"g_pCheckBoxpost_window_Slot5");        // 체크박스
        pReadPic    = pStaticDlgRead->GetPictureIndex(5);
        pNotReadPic = pStaticDlgNotRead->GetPictureIndex(5);
        break;
    case 5:            
        pStaticDlg1 = (CKTDGUIStatic*)m_pDlgMailListSlot->GetControl(L"g_pStatic_message16");            
        pStaticDlg2 = (CKTDGUIStatic*)m_pDlgMailListSlot->GetControl(L"g_pStatic_message17");            
        pStaticDlg3 = (CKTDGUIStatic*)m_pDlgMailListSlot->GetControl(L"g_pStatic_message18");
        pButtonDlg  = (CKTDGUIButton*)m_pDlgMailListSlot->GetControl(L"Slot_Button6");
        pCheckDlg   = (CKTDGUICheckBox*)m_pDlgMailListSlot->GetControl(L"g_pCheckBoxpost_window_Slot6");        // 체크박스
        pReadPic    = pStaticDlgRead->GetPictureIndex(6);
        pNotReadPic = pStaticDlgNotRead->GetPictureIndex(6);
        break;
    }

    // enable


	//{{ 2011.9.7 이준호 Year-Month-day 를 Day-Month-Year 순으로 출력되도록 함 //최민철 유럽만
#ifdef MODIFY_PRINT_DATE_SEQ
	wstring L_temp;
	WCHAR wcDashChar = L'-';
	if(m_listSlot[inx].m_strDate[4]== wcDashChar )
	{
		//YYYY-MM-DD이므로 반대 순서로 짤라내는 부분이다
		L_temp=m_listSlot[inx].m_strDate.substr(8,2);
		L_temp+=L"-";
		L_temp+=m_listSlot[inx].m_strDate.substr(5,2);
		L_temp+=L"-";
		L_temp+=m_listSlot[inx].m_strDate.substr(0,4);
		L_temp+=L" ";
		m_listSlot[inx].m_strDate.swap(L_temp);
		L_temp.clear();//마무리
	}
#endif
	//}}

    WCHAR strDate[256] = {0,};
	StringCchPrintf( strDate, 256, L"%s %s", m_listSlot[inx].m_strDate.c_str(), m_listSlot[inx].m_strNickName.c_str() );
    //wsprintf( strDate, L"%s %s", m_listSlot[inx].m_strDate.c_str(), m_listSlot[inx].m_strNickName.c_str() );
    pStaticDlg1->GetString(0)->msg = strDate;
    pStaticDlg2->GetString(0)->msg = GET_REPLACED_STRING( ( STR_ID_400, "L", m_listSlot[inx].m_strTitle ) );

    wstring itemName;
    if(m_listSlot[inx].m_ItemId == 0)
    {
        itemName = GET_STRING( STR_ID_401 );
    }
    else
    {

#ifdef SERV_ENTER_FIELD_QUEST_CLEAR

#ifdef SERV_RELATIONSHIP_SYSTEM
		if( IsSystemMail() == true &&
#else  // SERV_RELATIONSHIP_SYSTEM
		if( m_bSystemMail == true &&
#endif // SERV_RELATIONSHIP_SYSTEM    
			KPostItemInfo::IsItemIDLetter( static_cast<char>(m_listSlot[inx].m_iKind) ) == false )
#else
        if(m_bSystemMail == true && 
			m_listSlot[inx].m_iKind != KPostItemInfo::LT_MESSAGE && 
			m_listSlot[inx].m_iKind != KPostItemInfo::LT_BROKEN_ITEM &&
			m_listSlot[inx].m_iKind != KPostItemInfo::LT_WEB_POINT_EVENT )
#endif //SERV_ENTER_FIELD_QUEST_CLEAR
        {
            itemName = GET_STRING( STR_ID_402 );
        }
        else
		{
            const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet(m_listSlot[inx].m_ItemId);
			if(pItemTemplet != NULL)
				itemName = pItemTemplet->GetFullName_();
		}
    }        

    wstring wstrItemDesc;
    if(m_listSlot[inx].m_ItemId != 0 && m_listSlot[inx].m_EnchantLevel > 0)
    {
        wstrItemDesc = GET_REPLACED_STRING( ( STR_ID_403, "iL", m_listSlot[inx].m_EnchantLevel, itemName ) );
    }
    else
    {
        if( itemName.compare( GET_STRING( STR_ID_404 ) ) == 0 )
            wstrItemDesc = GET_STRING( STR_ID_405 );
        else
            wstrItemDesc = GET_REPLACED_STRING( ( STR_ID_406, "L", itemName ) );
    }

    pStaticDlg3->GetString(0)->msg = wstrItemDesc;

    pButtonDlg->SetEnable(true);
    pCheckDlg->SetEnable(true);    

    if(m_listSlot[inx].m_bRead == true)
    {       
        pReadPic->SetShow(true);
        pNotReadPic->SetShow(false);
        pButtonDlg->SetColor(D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
    }
    else
    {
        pReadPic->SetShow(false);
        pNotReadPic->SetShow(true);
        pButtonDlg->SetColor(D3DXCOLOR(0.243f, 0.925f, 0.929f, 1.f));
    }
}

void CX2PostBox::ChangePage(int step)
{
    int tempPage = m_nCurrentPage + step;

    if(tempPage > m_nMaxPage)
    {
        m_nCurrentPage = m_nMaxPage;
        return;
    }

    if(tempPage < 1)
    {
        m_nCurrentPage = 1;
        return;
    }

    m_nCurrentPage = tempPage;
    
    RequestList();
}

#ifdef ADD_POST_BOX_PAGE_BUTTON //2013.07.15
void CX2PostBox::MoveFirstPage( IN bool bFirst_ )
{
	if ( bFirst_ )
	{
		if ( 1 == m_nCurrentPage )
			return;

		m_nCurrentPage = 1;
	}
	else
	{
		if ( m_nMaxPage == m_nCurrentPage )
			return;

		m_nCurrentPage = m_nMaxPage;
	}

	RequestList();
}
#endif //ADD_POST_BOX_PAGE_BUTTON

void CX2PostBox::SetListPage()
{
    CKTDGUIStatic *pStaticDlg1 = (CKTDGUIStatic*)m_pDlgMailReceive->GetControl(L"g_pStaticpost_window_Font"); 
    WCHAR strPage[256] = {0,};
	StringCchPrintf( strPage, 256, L"%d / %d", m_nCurrentPage, m_nMaxPage);
    //wsprintf( strPage, L"%d / %d", m_nCurrentPage, m_nMaxPage);
    pStaticDlg1->GetString(0)->msg = strPage;
}

void CX2PostBox::RequestList()
{
    CKTDGUIButton *pButtonDlgSystem = (CKTDGUIButton*)m_pDlgMailReceive->GetControl(L"system_mail");
    CKTDGUIButton *pButtonDlgNormal = (CKTDGUIButton*)m_pDlgMailReceive->GetControl(L"common_mail");

#ifdef SERV_RELATIONSHIP_SYSTEM 
	CKTDGUIButton *pButtonDlgWedding = (CKTDGUIButton*)m_pDlgMailReceive->GetControl(L"wedding_mail");
#endif // SERV_RELATIONSHIP_SYSTEM 


    // 메일 종류 설정
#ifdef SERV_RELATIONSHIP_SYSTEM
	if( IsSystemMail() == true )
#else  // SERV_RELATIONSHIP_SYSTEM
	if(m_bSystemMail)
#endif // SERV_RELATIONSHIP_SYSTEM    
    {
        // 시스템 메일
        pButtonDlgSystem->SetDownStateAtNormal(true);
        pButtonDlgNormal->SetDownStateAtNormal(false);
    
#ifdef SERV_RELATIONSHIP_SYSTEM 
		pButtonDlgWedding->SetDownStateAtNormal(false);
#endif // SERV_RELATIONSHIP_SYSTEM 

	}
#ifdef SERV_RELATIONSHIP_SYSTEM 
	else if ( IsWeddingMail() == true )
	{
		pButtonDlgSystem->SetDownStateAtNormal(false);
		pButtonDlgNormal->SetDownStateAtNormal(false);
		pButtonDlgWedding->SetDownStateAtNormal(true);
	}
#endif // SERV_RELATIONSHIP_SYSTEM
	else
    {
        // 일반 메일
        pButtonDlgSystem->SetDownStateAtNormal(false);
        pButtonDlgNormal->SetDownStateAtNormal(true);
#ifdef SERV_RELATIONSHIP_SYSTEM 
		pButtonDlgWedding->SetDownStateAtNormal(false);
#endif // SERV_RELATIONSHIP_SYSTEM 
	}



    if(m_pDLGQuery != NULL)
    {
        g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGQuery, NULL, false );
        m_pDLGQuery = NULL;
    }

    // 서버로 리스트 요청한다.
    m_nMaxPage = 1;
    m_bProcess = true;
    Handler_EGS_GET_POST_LETTER_LIST_REQ();
}

void CX2PostBox::OpenMail(int inx)
{
    int index = inx-1;
    if(index < 0 || index > 6)
        return;
	
    if(m_bProcess == true)
    {
        // 이미 메일 열기중...
        return;
    }

    if(m_pDLGQuery != NULL)
    {
        g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGQuery, NULL, false );
        m_pDLGQuery = NULL;
    }

    CKTDGUIButton *pButtonDlg;
    
    for(int i=0; i<6; ++i)
    {
        switch(i)
        {
        case 0:
            pButtonDlg  = (CKTDGUIButton*)m_pDlgMailListSlot->GetControl(L"Slot_Button1");
            break;
        case 1:
            pButtonDlg  = (CKTDGUIButton*)m_pDlgMailListSlot->GetControl(L"Slot_Button2");
            break;
        case 2:
            pButtonDlg  = (CKTDGUIButton*)m_pDlgMailListSlot->GetControl(L"Slot_Button3");
            break;
        case 3:
            pButtonDlg  = (CKTDGUIButton*)m_pDlgMailListSlot->GetControl(L"Slot_Button4");
            break;
        case 4:
            pButtonDlg  = (CKTDGUIButton*)m_pDlgMailListSlot->GetControl(L"Slot_Button5");
            break;
        case 5:
            pButtonDlg  = (CKTDGUIButton*)m_pDlgMailListSlot->GetControl(L"Slot_Button6");
            break;
        }

        if(index == i)
            pButtonDlg->SetDownStateAtNormal(true);
        else
            pButtonDlg->SetDownStateAtNormal(false);
        m_listSlot[i].m_bSelected = false;
    }
        
    m_listSlot[index].m_bSelected = true;

    if(m_listSlot[index].m_bCache == true)
    {
        
        // 이미 캐쉬된 메일이므로 서버로부터 새로 데이터를 받지않고 바로 오픈한다.
        SetMailView(index);
        return;
    }

    // 메일 열기
    m_bProcess = true;
    Handler_EGS_READ_LETTER_REQ(m_listSlot[index].m_postID);
}

void CX2PostBox::SetMailView(int inx)
{
    if(inx < 0 || inx >= 6)
        return;   

	m_pDlgMailView->SetShowEnable(false, false);
#ifdef SERV_RELATIONSHIP_SYSTEM
	if( IsSystemMail() == true)
#else  // SERV_RELATIONSHIP_SYSTEM
	if( m_bSystemMail == true)
#endif // SERV_RELATIONSHIP_SYSTEM    
		
	{
		m_pDlgMailView = m_pDlgMailViewSystem;
	}

#ifdef SERV_RELATIONSHIP_SYSTEM
	else if( IsWeddingMail() == true)
	{
		m_pDlgMailView = m_pDlgMailViewWedding;
	}
#endif // SERV_RELATIONSHIP_SYSTEM
	else
	{
		m_pDlgMailView = m_pDlgMailViewCommon;
	}

	SetSlotManagerLayer( XDL_NORMAL_4 );
	SetSlotManagerChangeSequence(true);		
	g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer(m_pDlgMailView, XDL_NORMAL_3);
	g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDlgMailView, false );	

	m_pDlgMailView->SetShowEnable(true, true);

#ifdef POSTBOX_NOTICE
	g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU_POSTBOX_NOTICE, false);
#endif POSTBOX_NOTICE

    CKTDGUIButton *pButtonDlg = (CKTDGUIButton*)m_pDlgMailView->GetControl(L"Reply");
#ifdef SERV_RELATIONSHIP_SYSTEM
	if( IsSystemMail() == true)
#else  // SERV_RELATIONSHIP_SYSTEM
	if( m_bSystemMail == true)
#endif // SERV_RELATIONSHIP_SYSTEM    
    {
        pButtonDlg->SetEnable(false);
    }
    else
    {
        pButtonDlg->SetEnable(true);
    }


    CKTDGUIStatic *pStaticDlg1, *pStaticDlg2, *pStaticDlg3;
    CKTDGUIIMEEditBox *pMessageDlg;

    pStaticDlg1 = (CKTDGUIStatic*)m_pDlgMailView->GetControl(L"g_pStaticpost_window_Font1");    // 날짜 + 보낸이
    pStaticDlg2 = (CKTDGUIStatic*)m_pDlgMailView->GetControl(L"g_pStaticpost_window_Font2");    // 제목
    pStaticDlg3 = (CKTDGUIStatic*)m_pDlgMailView->GetControl(L"g_pStaticpost_window_Font3");    // 첨부
    pMessageDlg = (CKTDGUIIMEEditBox*)m_pDlgMailView->GetControl(L"IME_Edit_Message");          // 본문
    
#ifdef SERV_RELATIONSHIP_SYSTEM
	if( true == IsWeddingMail() )
		pStaticDlg1->GetString(0)->msg = wstring( m_listSlot[inx].m_strNickName );
	else
#endif // SERV_RELATIONSHIP_SYSTEM

	{
		pStaticDlg1->GetString(0)->msg = wstring(m_listSlot[inx].m_strDate + m_listSlot[inx].m_strNickName);
	}
    pStaticDlg2->GetString(0)->msg = m_listSlot[inx].m_strTitle;

    wstring itemName;
    if(m_listSlot[inx].m_ItemId > 0)
    {
#ifdef SERV_RELATIONSHIP_SYSTEM
		if(IsSystemMail() == true && m_listSlot[inx].m_pItem->GetItemData().m_ItemID == 127050)
#else  // SERV_RELATIONSHIP_SYSTEM
		if(m_bSystemMail == true && m_listSlot[inx].m_pItem->GetItemData().m_ItemID == 127050)

#endif // SERV_RELATIONSHIP_SYSTEM    
        {            
            itemName = GET_STRING( STR_ID_402 );
        }
        else
        {
            itemName = g_pData->GetItemManager()->GetItemTemplet(m_listSlot[inx].m_ItemId)->GetFullName_();

            if(itemName.compare( GET_STRING( STR_ID_404 ) ) == 0 || itemName.compare( GET_STRING( STR_ID_407 ) ) == 0)
            {
                itemName = GET_STRING( STR_ID_407 );
            }
            else if(m_listSlot[inx].m_pItem != NULL && m_listSlot[inx].m_pItem->GetItemData().m_EnchantLevel > 0)
            {
                wstringstream wstrstm;
                wstrstm << L"+" << m_listSlot[inx].m_pItem->GetItemData().m_EnchantLevel << L" ";

                itemName = wstrstm.str().c_str() + itemName;
            }
        }        
    }
    else
    {
        itemName = GET_STRING( STR_ID_401 );
    }
    pStaticDlg3->GetString(0)->msg = itemName;     // 첨부물

#ifdef SERV_RELATIONSHIP_SYSTEM
	if( IsSystemMail() == true)
#else  // SERV_RELATIONSHIP_SYSTEM
	if(m_bSystemMail == true)
#endif // SERV_RELATIONSHIP_SYSTEM    
    {
#ifdef CLIENT_GLOBAL_LINEBREAK
		wstring wstrMessage = CWordLineHandler::GetStrByLineBreakInX2Main( m_listSlot[inx].m_strMessage.c_str(), 320, XUF_DODUM_13_SEMIBOLD );
#else //CLIENT_GLOBAL_LINEBREAK
#ifdef SIMPLE_BUG_FIX
		wstring wstrMessage = g_pMain->GetStrByLienBreak( m_listSlot[inx].m_strMessage.c_str(), 260, XUF_DODUM_13_SEMIBOLD );
#else // SIMPLE_BUG_FIX
        wstring wstrMessage = g_pMain->GetStrByLienBreak( m_listSlot[inx].m_strMessage.c_str(), 280, XUF_DODUM_13_SEMIBOLD );
#endif // SIMPLE_BUG_FIX
#endif //CLIENT_GLOBAL_LINEBREAK
        pMessageDlg->SetText(wstrMessage.c_str());
    }
#ifdef SERV_RELATIONSHIP_SYSTEM
	else if ( IsWeddingMail() == true )
	{
		wstring wstrMessage = g_pMain->GetStrByLienBreak( m_listSlot[inx].m_strMessage.c_str(), 240, XUF_DODUM_13_SEMIBOLD );
		pMessageDlg->SetText(wstrMessage.c_str());
	}
#endif // SERV_RELATIONSHIP_SYSTEM
	
	else
    {
        pMessageDlg->SetText(m_listSlot[inx].m_strMessage.c_str());
    }

    pMessageDlg->SetEnable(false);

    // 미리보기 아이템 셋팅
    if(m_listSlot[inx].m_pItem != NULL)
    {
        CX2SlotItem* pSlotItem = (CX2SlotItem*)GetSlot( CX2SlotItem::ST_POST_RECEIVE, 101 );
        if ( pSlotItem != NULL )
        {
            pSlotItem->DestroyItemUI();        
            if ( m_listSlot[inx].m_pItem != NULL )
            {
                if(m_listSlot[inx].m_bAttachItem == true && m_listSlot[inx].m_pItem->GetItemData().m_ItemID != 127050)                
                    pSlotItem->CreateItemUI( m_listSlot[inx].m_pItem );                
                else
                    pSlotItem->CreateItemUI( m_listSlot[inx].m_pItem->GetItemTemplet() );
                SetSlotShowEnable(true, 3);
            }
        }
    }
    else
    {
        CX2SlotItem* pSlotItem = (CX2SlotItem*)GetSlot( CX2SlotItem::ST_POST_RECEIVE, 101 );
        if ( pSlotItem != NULL )
        {
            pSlotItem->DestroyItemUI();        
        }
    }

	D3DXVECTOR2 vOffsetItemSlot = D3DXVECTOR2(643.f, 515.f);
#ifdef SERV_RELATIONSHIP_SYSTEM
	if( IsSystemMail() == false)
#else  // SERV_RELATIONSHIP_SYSTEM
	if( m_bSystemMail == false)
#endif // SERV_RELATIONSHIP_SYSTEM    
	{
#ifdef SERV_RELATIONSHIP_SYSTEM
		if ( IsWeddingMail() == true )
		{
			vOffsetItemSlot.x += 44.f;
			vOffsetItemSlot.y += 10.f;
		}
		else
#endif // SERV_RELATIONSHIP_SYSTEM
		{
			vOffsetItemSlot.y += 27.f;
		}
	}
	CX2SlotItem* pSlotItem = (CX2SlotItem*)GetSlot( CX2SlotItem::ST_POST_RECEIVE, 101 );
	if( pSlotItem != NULL )
		pSlotItem->SetPos( vOffsetItemSlot );


    // 읽은메일 아이콘 표시
    SetListSlot(inx);

	UpdateUnReadMailNotice();
}

bool CX2PostBox::GetCheckMail(int inx)
{
    if(inx < 0 || inx >= 6)
        return false;

    CKTDGUICheckBox *pCheckDlg;

    switch(inx)
    {
    case 0:
        pCheckDlg = (CKTDGUICheckBox*)m_pDlgMailListSlot->GetControl(L"g_pCheckBoxpost_window_Slot1");        // 체크박스
        break;
    case 1:
        pCheckDlg = (CKTDGUICheckBox*)m_pDlgMailListSlot->GetControl(L"g_pCheckBoxpost_window_Slot2");        // 체크박스
        break;
    case 2:
        pCheckDlg = (CKTDGUICheckBox*)m_pDlgMailListSlot->GetControl(L"g_pCheckBoxpost_window_Slot3");        // 체크박스
        break;
    case 3:
        pCheckDlg = (CKTDGUICheckBox*)m_pDlgMailListSlot->GetControl(L"g_pCheckBoxpost_window_Slot4");        // 체크박스
        break;
    case 4:
        pCheckDlg = (CKTDGUICheckBox*)m_pDlgMailListSlot->GetControl(L"g_pCheckBoxpost_window_Slot5");        // 체크박스
        break;
    case 5:
        pCheckDlg = (CKTDGUICheckBox*)m_pDlgMailListSlot->GetControl(L"g_pCheckBoxpost_window_Slot6");        // 체크박스
        break;
    }
    
	
    return pCheckDlg->GetChecked();
}

void CX2PostBox::CheckDeleteMail(int inx)
{
    if(m_bProcess == true)
    {
        // 이미 메일 삭제중...
        return;
    }

    if(m_pDLGQuery != NULL)
    {
        g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGQuery, NULL, false );
        m_pDLGQuery = NULL;

    }

    int index = inx - 1;
    bool bAttach = false;
    bool bFind = false;


    if(index < 0)
    {        
        for(int i=0; i<6; ++i)
        {
            if( GetCheckMail(i) == true )
            {
                bFind = true;
                if(m_listSlot[i].m_bAttachItem == true || m_listSlot[i].m_nAttachMoney > 0)
                {
                    bAttach = true;
                }
                m_listSlot[i].m_bChecked = true;
            }
            else 
                m_listSlot[i].m_bChecked = false;
        }
    }
    else
    {
        bFind = true;
        for(int i=0; i<6; ++i)
        {
            m_listSlot[i].m_bChecked = false;
        }

        m_listSlot[index].m_bChecked = true;
        if(m_listSlot[index].m_bAttachItem == true || m_listSlot[index].m_nAttachMoney > 0)
        {
            bAttach = true;
            m_listSlot[index].m_bChecked = true;
        }
    }
    
    if(bFind == false)
    {
        g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250, 300), GET_STRING( STR_ID_408 ), (CKTDXStage*) m_pNowState );
        return;
    }

    WCHAR wszText[512] = L"";

    if(bAttach == true)
        swprintf( wszText, sizeof(wszText)/sizeof(WCHAR)-1, GET_STRING( STR_ID_409 ) );
    else
        swprintf( wszText, sizeof(wszText)/sizeof(WCHAR)-1, GET_STRING( STR_ID_410 ) );

    m_pDLGQuery = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(250,300), wszText, 
        PBQM_DELETE_OK, (CKTDXStage*) m_pNowState, PBQM_DELETE_CANCEL ); 
}

void CX2PostBox::GetItemLetter(int inx)
{
    if(m_bProcess == true)
    {
        // 이미 아이템 가져오는중...
        return;
    }

    if(m_pDLGQuery != NULL)
    {
        g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGQuery, NULL, false );
        m_pDLGQuery = NULL;
    }

    if(inx < 0 || inx >= 6)
        return;

    if(m_listSlot[inx].m_ItemId <= 0 || m_listSlot[inx].m_postID <= 0)
        return;

	//{{ 09.04.28 태완 : ED 한계초과 체크
	if(m_listSlot[inx].m_ItemId == 127000 || m_listSlot[inx].m_ItemId == 127040)
	{
		INT64 EDTotalAfterGet = 0;
		EDTotalAfterGet += g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_ED;
		EDTotalAfterGet += m_listSlot[inx].m_nAttachMoney;
		if(EDTotalAfterGet < 0 || EDTotalAfterGet > MAX_ED_FOR_PLAYER)
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_2551 ), g_pMain->GetNowState() );
			return;
		}
	}	
	//}}

    m_bProcess = true;

    Handler_EGS_GET_ITEM_FROM_LETTER_REQ(m_listSlot[inx].m_postID);
}

void CX2PostBox::ReplyMail(int inx)
{
    if(m_bProcess == true)
    {
        // 먼가 다른 뻘짓중...
        return;
    }

    if(m_pDLGQuery != NULL)
    {
        g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGQuery, NULL, false );
        m_pDLGQuery = NULL;
    }

    if(inx < 0 || inx >= 6)
        return;

    if(m_listSlot[inx].m_postID <= 0)
        return;

    wstring replyTo, replyTitle;
    replyTitle = GET_STRING( STR_ID_411 );

    replyTo = m_listSlot[inx].m_strNickName;
    replyTitle = replyTitle + m_listSlot[inx].m_strTitle;

    m_nCurrentPage = 1;
    SetDialogShow(true, 1);


    CKTDGUIIMEEditBox *pDialogTo = (CKTDGUIIMEEditBox*)m_pDLGMailSend->GetControl(L"IME_EditMail_To");
    if(pDialogTo != NULL)
    {
        pDialogTo->SetText(replyTo.c_str());
		m_strNickName = replyTo;
    }        
    
    CKTDGUIIMEEditBox *pDialogTitle = (CKTDGUIIMEEditBox*)m_pDLGMailSend->GetControl(L"IME_EditMail_Name");
    if(pDialogTitle != NULL)
    {
        pDialogTitle->SetText(replyTitle.c_str());
    }

	m_iItemUid = -1;   
    m_pPostSlot = NULL;
    CX2SlotItem* pSlotItem = (CX2SlotItem*)GetSlot( CX2SlotItem::ST_POST_SEND, 100 );
    if ( pSlotItem != NULL )
    {        
        pSlotItem->DestroyItemUI();     
    }

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CX2PostBox::SendMail()
{   
    if(m_bProcess == true)
    {
        // 이미 메일 전송중...
        return;
    }

    if(m_pDLGQuery != NULL)
    {
        g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGQuery, NULL, false );
        m_pDLGQuery = NULL;

    }

    CKTDGUIIMEEditBox *pDialogTo = (CKTDGUIIMEEditBox*)m_pDLGMailSend->GetControl(L"IME_EditMail_To");
    CKTDGUIIMEEditBox *pDialogTitle = (CKTDGUIIMEEditBox*)m_pDLGMailSend->GetControl(L"IME_EditMail_Name");

	CKTDGUIIMEEditBox *pDialogBody = (CKTDGUIIMEEditBox*)m_pDLGMailSend->GetControl(L"IME_Edit_Message");
	if( pDialogBody != NULL )
	{
#ifdef SERV_POST_BAN_WORD_FILTER
		wstring wstrPostBody = pDialogBody->GetText();
		wstring wstrBanPostBodyWord = L"";

		if( g_pMain->GetStringFilter()->CheckIsValidPostWordString( CX2StringFilter::FT_POST, wstrPostBody.c_str(), wstrBanPostBodyWord ) == false )
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250, 300), GET_REPLACED_STRING( ( STR_ID_17230, "L", wstrBanPostBodyWord ) ), (CKTDXStage*) m_pNowState ); 
			return;
		}
#else //SERV_POST_BAN_WORD_FILTER
		wstring wstrFilterName[31] = 
		{
			L"비밀번호",
			L"비밀 번호",
			L"비번",
			L"패스워드",
			L"패스 워드",
			L"PASSWORD",
			L"PASS WORD",
			L"PASS",
			L"PAS",
			L"아이디",
			L"ID",
			L"계정",
			L"인증",
			L"보안패드",
			L"보안 패드",
			L"암호",
			L"운영자",
			L"운영팀",
			L"넥슨",
			L"NEXON",
			L"KOG",
			L"GM",
			L"GM고등어",
			L"GM캅",
			L"GM리엘",
			L"GM아이린",
			L"GM너구리",
			L"관리자",
			L"이벤트",
			L"당첨",
			L"넥슨캐시"
		};

		wstring wstrPostBody = pDialogBody->GetText();
		for(int i=0; i<31; ++i)
		{
			if( wcsstr(wstrPostBody.c_str(), wstrFilterName[i].c_str()) != NULL )
			{
				

				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250, 300), GET_REPLACED_STRING( ( STR_ID_17230, "L", wstrFilterName[i] ) ), (CKTDXStage*) m_pNowState ); 
				return;
			}
		}		
#endif //SERV_POST_BAN_WORD_FILTER
	}

    m_strNickName = pDialogTo->GetText();    
    m_strTitle = pDialogTitle->GetText();

    if(m_strNickName.length() <= 0)
    {
        // 필수 입력사항
        g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250, 300), GET_STRING( STR_ID_412 ), (CKTDXStage*) m_pNowState );        
        //pDialogTo->RequestFocus();
        return;
    }   
    if(m_strTitle.length() <= 0)
    {
        // 필수 입력사항
        g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250, 300), GET_STRING( STR_ID_413 ), (CKTDXStage*) m_pNowState );
        //pDialogTo->RequestFocus();
        return;
    }

	CX2Item* pItemData = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( m_iItemUid );

#ifdef SERV_FREE_MAIL_TICKET
	int attachFee = GetPostFee( m_nED, pItemData );
	wstring wstrText;
	if( attachFee <= 200 )
	{
		wstrText = GET_REPLACED_STRING( ( STR_ID_415, "Li", m_strNickName, attachFee ) );
	}
	else
	{
		wstrText = GET_REPLACED_STRING( ( STR_ID_416, "Lii", m_strNickName, 200, attachFee - 200 ) );
	}
#else //SERV_FREE_MAIL_TICKET
    int attachFee = 0;
    if(m_nED > 0)
    {
        attachFee = (int)(m_nED * 0.05f);
    }
	else if(pItemData != NULL)
	{
		int price = (int)(pItemData->GetItemTemplet()->GetPrice() * 0.2f);
		int quantity = m_nQuantity; //pItemData->GetItemData().m_Quantity;

		attachFee = (int)((price * quantity) * 0.05f);
	}

    wstring wstrText;
#ifdef FREE_FEE
    wstrText = GET_REPLACED_STRING( ( STR_ID_414, "L", m_strNickName ) );
#else
    if(attachFee == 0)
    {
        wstrText = GET_REPLACED_STRING( ( STR_ID_415, "L", m_strNickName ) );
    }
    else
    {
        wstrText = GET_REPLACED_STRING( ( STR_ID_416, "Li", m_strNickName, attachFee ) );
    }
#endif
#endif //SERV_FREE_MAIL_TICKET

    m_pDLGQuery = g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(250,300), wstrText.c_str(), 
        PBQM_SEND_ACCEPT, (CKTDXStage*) m_pNowState, PBQM_SEND_CANCEL ); 
}


bool CX2PostBox::SetMailHeader(int inx, KPostItemTitleInfo mailHeader)
{    
    if(inx < 0 || inx >= 6)
        return false;

    m_listSlot[inx].m_postID = mailHeader.m_iPostNo;    
	//{{ 2011.09.16 조효진  넷 에러 스트링을 클라이언트쪽에서 가져와서 출력
#ifdef SERV_NETERROR_STR_GET_FROM_CLIENT
	if( true == mailHeader.m_bSenderDeleted )
		m_listSlot[inx].m_strNickName = NetError::GetErrStrF( NetError::STR_DELETED_NICKNAME );
	else
		m_listSlot[inx].m_strNickName = mailHeader.m_wstrFromNickName;
#else SERV_NETERROR_STR_GET_FROM_CLIENT
	m_listSlot[inx].m_strNickName = mailHeader.m_wstrFromNickName;
#endif SERV_NETERROR_STR_GET_FROM_CLIENT
	//}}
    m_listSlot[inx].m_strTitle = mailHeader.m_wstrTitle;
    m_listSlot[inx].m_bRead = mailHeader.m_bRead;
    m_listSlot[inx].m_EnchantLevel = mailHeader.m_cEnchantLevel;

    wstring strDate = mailHeader.m_wstrRegDate;
    strDate = strDate.substr(0, 10);
    strDate = strDate + L" ";
    m_listSlot[inx].m_strDate = strDate;

    if(mailHeader.m_iScriptIndex == 0)
    {
        m_listSlot[inx].m_bAttachItem = false;
        m_listSlot[inx].m_ItemId = 0;
        m_listSlot[inx].m_nAttachMoney = 0;
    }
    else if(mailHeader.m_iScriptIndex == 127000 || 
		    mailHeader.m_iScriptIndex == 127010 || 
			mailHeader.m_iScriptIndex == 127020 || 
			mailHeader.m_iScriptIndex == 127041 )
    {
        m_listSlot[inx].m_bAttachItem = false;
        m_listSlot[inx].m_ItemId = mailHeader.m_iScriptIndex;
        m_listSlot[inx].m_nAttachMoney = 1;  // 헤더에서는 아이템/돈의 수량/액수를 모르므로 우선 돈이라는 표시를 해주자(AttachItem은 false, nAttachMoney는 0보다 클경우 돈이다.)
    }
    else
    {
        m_listSlot[inx].m_ItemId = mailHeader.m_iScriptIndex;
        m_listSlot[inx].m_bAttachItem = true;   
        m_listSlot[inx].m_nAttachMoney = 1;
    }

    m_listSlot[inx].m_iKind = mailHeader.m_cScriptType;
    return true;
}

int CX2PostBox::SetMailBody( KPostItemInfo& mailBody)
{
    int index = -1;
    UidType postId = mailBody.m_iPostNo;

#ifdef CONVERSION_VS
	int i = 0;
	for( i = 0; i < 6; ++i )
#else CONVERSION_VS
    for(int i=0; i<6; ++i)
#endif CONVERSION_VS
    {
        if(m_listSlot[i].m_postID == postId)
        {
            index = i;
            break;
        }
    }

    if(index < 0 || index >= 6)
        return -1;

    if(m_listSlot[index].m_bRead == false)
    {
        --m_nUnReadMail;
        if(m_nUnReadMail < 0)
            m_nUnReadMail = 0;

        if(mailBody.m_cScriptType == KPostItemInfo::LT_POST_OFFICE || mailBody.m_cScriptType == KPostItemInfo::LT_NONE )
        {
            --m_nUnReadCommon;
            if(m_nUnReadCommon < 0)
                m_nUnReadCommon = 0;
        }

#ifdef SERV_RELATIONSHIP_SYSTEM
		else if(mailBody.m_cScriptType == KPostItemInfo::LT_WEDDING_INVITATION || mailBody.m_cScriptType == KPostItemInfo::LT_WEDDING_RESERVE
			|| mailBody.m_cScriptType == KPostItemInfo::LT_WEDDING_REWARD )
		{
			--m_nUnReadWedding;
			if(m_nUnReadWedding < 0)
				m_nUnReadWedding = 0;
		}
#endif // SERV_RELATIONSHIP_SYSTEM
        else
        {
            --m_nUnReadSystem;
            if(m_nUnReadSystem < 0)
                m_nUnReadSystem = 0;
        }
    }

    m_listSlot[index].m_iKind = mailBody.m_cScriptType;
    m_listSlot[index].m_bRead = true;
    m_listSlot[index].m_bCache = true;
    m_listSlot[index].m_strMessage = mailBody.m_wstrMessage;    
    m_listSlot[index].m_ItemId = mailBody.m_iScriptIndex;
    m_listSlot[index].m_nAttachMoney = mailBody.m_iQuantity; // 액수

    if(mailBody.m_iScriptIndex == 127000 || 
	   mailBody.m_iScriptIndex == 127010 || 
	   mailBody.m_iScriptIndex == 127020 || 
	   mailBody.m_iScriptIndex == 127041 )
    {
        // 돈첨부        
        m_listSlot[index].m_bAttachItem = false;        
    }
    else if(mailBody.m_iScriptIndex > 0)
    {
        // 아이템 첨부
        m_listSlot[index].m_bAttachItem = true;
    }    

    // 아이템 생성        
    if(m_listSlot[index].m_ItemId > 0)
    {
#ifdef SERV_RELATIONSHIP_SYSTEM
		if( IsSystemMail() == true)
#else  // SERV_RELATIONSHIP_SYSTEM
		if(m_bSystemMail == true)
#endif // SERV_RELATIONSHIP_SYSTEM    
		{
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
			if( KPostItemInfo::IsItemIDLetter( static_cast<char>(m_listSlot[index].m_iKind) ) == false )
#else
            if(m_listSlot[index].m_iKind != KPostItemInfo::LT_MESSAGE && 
				m_listSlot[index].m_iKind != KPostItemInfo::LT_BROKEN_ITEM &&
				m_listSlot[index].m_iKind != KPostItemInfo::LT_WEB_POINT_EVENT )
#endif //SERV_ENTER_FIELD_QUEST_CLEAR
            {
                mailBody.m_iScriptIndex = 127050;
            }            

			if(m_listSlot[index].m_bAttachItem == false)
				mailBody.m_iScriptIndex = 127040;

            //m_listSlot[index].m_pItem = new CX2Item( CX2Item::CreateItemData(mailBody), NULL );
            CX2Item::ItemData kItemData;
            kItemData.Initialize( mailBody );
            m_listSlot[index].m_pItem = new CX2Item( kItemData, NULL );
        }
        else
        {
            if(m_listSlot[index].m_bAttachItem == false)
                mailBody.m_iScriptIndex = 127040;
            //m_listSlot[index].m_pItem = new CX2Item( CX2Item::CreateItemData(mailBody), NULL );
            CX2Item::ItemData kItemData;
            kItemData.Initialize( mailBody );
            m_listSlot[index].m_pItem = new CX2Item( kItemData, NULL );
        }        
    }
    else
    {
        if(m_listSlot[index].m_pItem != NULL)        
        {
            SAFE_DELETE( m_listSlot[i].m_pItem );
        }
    }


    return index;
}

void CX2PostBox::BlackListReq()
{
    // 차단목록 리스트 요청

    m_bProcess = true;
    Handler_EGS_GET_POST_BLACK_LIST_REQ();

}

void CX2PostBox::ShowBlackList()
{
    CKTDGUIListBox *pDlgListbox = (CKTDGUIListBox*)m_pDlgMailSetting->GetControl(L"ListBoxChatContent");
    pDlgListbox->RemoveAllItems();
    
    for(int i=0; i < (int)m_vecBList.size(); ++i)
    {        
        pDlgListbox->AddItem(m_vecBList[i].c_str(), NULL);        
    }
}

void CX2PostBox::DeleteUser(wstring userName)
{
    for(int i=0; i<(int)m_vecBList.size(); ++i)
    {
        if(userName.compare(m_vecBList[i]) == 0)
        {
            m_vecBList.erase(m_vecBList.begin() + i);
            return;
        }
    }
}

void CX2PostBox::AddUser(wstring userName)
{
    m_vecBList.push_back(userName);
}

wstring CX2PostBox::GetUserName(int inx)
{
    //g_pData->GetMyUser()->GetUnitNum()
    CX2Unit *pUnit = g_pData->GetMyUser()->GetUnitByIndex(inx);
    if(pUnit != NULL)
    {
		if( g_pData->GetSelectUnitServerGroupID() == pUnit->GetServerGroupID() )
			return pUnit->GetUnitData().m_NickName;;
    }

    return L"";
}

//{{ 2009. 6. 24  최육사	편지스트링조합
wstring CX2PostBox::GetSelectedUserName()
{
	if( g_pData->GetMyUser() == NULL )
		return L"";

	if( g_pData->GetMyUser()->GetSelectUnit() == NULL )
		return L"";

	return g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_NickName;
}
//}}


bool CX2PostBox::Handler_EGS_SEND_LETTER_REQ()
{
    KEGS_SEND_LETTER_REQ kPacket;    
    
    CKTDGUIIMEEditBox *pDialogMessage = (CKTDGUIIMEEditBox*)m_pDLGMailSend->GetControl(L"IME_Edit_Message");
    CKTDGUIIMEEditBox *pDialogEd = (CKTDGUIIMEEditBox*)m_pDLGMailSend->GetControl(L"IME_Add_Ed");       
    CKTDGUIIMEEditBox *pDialogTo = (CKTDGUIIMEEditBox*)m_pDLGMailSend->GetControl(L"IME_EditMail_To");
    CKTDGUIIMEEditBox *pDialogTitle = (CKTDGUIIMEEditBox*)m_pDLGMailSend->GetControl(L"IME_EditMail_Name");
    

    m_strNickName = pDialogTo->GetText();
    m_strTitle = pDialogTitle->GetText();       

    wstring	wstrMessage = L"";

	wstrMessage = pDialogMessage->GetText();
	m_nED = g_pMain->GetEDFromString( pDialogEd->GetText() );

	//{{ kimhc // 2009-11-10 // ED 입력 시 색 바뀌도록 수정
#ifdef	PRICE_COLOR
	pDialogEd->SetTextColor( g_pMain->GetPriceColor( m_nED ) );
#endif	PRICE_COLOR
	//}} kimhc // 2009-11-10 // ED 입력 시 색 바뀌도록 수정


    if( m_nED < 0 )
    {
        m_nED = 0;
    }
    else if( m_nED > MAX_ED_FOR_LETTER )
    {
        m_nED = MAX_ED_FOR_LETTER;
        g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250, 300), GET_STRING( STR_ID_417 ), (CKTDXStage*) m_pNowState );
    }
    
    CKTDGUICheckBox *pCheckDlg;
    pCheckDlg = (CKTDGUICheckBox*)m_pDLGMailSend->GetControl(L"Check_SystemMessage");
    if(pCheckDlg != NULL)
    {        
        if(pCheckDlg->GetChecked() == true)
            kPacket.m_bIsGmMessage = true;
    }

    

    kPacket.m_wstrToNickName = m_strNickName;    
    kPacket.m_wstrTitle = g_pMain->GetStringFilter()->FilteringNoteString( m_strTitle.c_str(), L'♡' );    
    kPacket.m_wstrMessage = g_pMain->GetStringFilter()->FilteringNoteString( wstrMessage.c_str(), L'♡' );
        
    //kPacket.m_wstrTitle = m_strTitle;
    //kPacket.m_wstrMessage = wstrMessage;

    kPacket.m_iED = m_nED;

	CX2Item* pItemData = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( m_iItemUid );
	if(m_nED == 0 && pItemData != NULL)
	{
		kPacket.m_iItemUID = pItemData->GetUID(); // 없을경우 0으로 넣으면 되는지 확인필요!
		kPacket.m_iQuantity = m_nQuantity;
	}
	else
	{
		kPacket.m_iItemUID = 0; // 없을경우 0으로 넣으면 되는지 확인필요!
		kPacket.m_iQuantity = 0;
	}
#if 0 
    if(m_nED == 0 && m_pPostSlot != NULL)
    {
        kPacket.m_iItemUID = m_pPostSlot->GetItemUID();
        kPacket.m_iQuantity = m_nQuantity;
    }
    else
    {
        kPacket.m_iItemUID = 0; // 없을경우 0으로 넣으면 되는지 확인필요!
        kPacket.m_iQuantity = 0;
    }        
#endif
    
    g_pData->GetServerProtocol()->SendPacket( EGS_SEND_LETTER_REQ, kPacket );
    g_pMain->AddServerPacket( EGS_SEND_LETTER_ACK, 60.0f );    

    if(m_bProcess)
    {
        m_pProcessDlg = g_pMain->KTDGUIMsgBox(D3DXVECTOR2(250, 300), GET_STRING( STR_ID_418 ), (CKTDXStage*) m_pNowState );
        //m_pProcessDlg->SetLayer(XDL_POP_UP);

        CX2ItemSlotManager::SetEnable(CX2Slot::ST_INVENTORY, false);
        CX2ItemSlotManager::SetEnable(CX2Slot::ST_POST_SEND, false);
    }    

    return true;
}

bool CX2PostBox::Handler_EGS_SEND_LETTER_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    m_bProcess = false;

    if(m_pProcessDlg != NULL)
    {
        g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pProcessDlg, NULL, false );

        CX2ItemSlotManager::SetEnable(CX2Slot::ST_INVENTORY, true);
        CX2ItemSlotManager::SetEnable(CX2Slot::ST_POST_SEND, true);
    }
    m_pProcessDlg = NULL;  

    // 패킷 처리
    KSerBuffer* pBuff = (KSerBuffer*)lParam;
    KEGS_SEND_LETTER_ACK kEvent;
    DeSerialize( pBuff, &kEvent );	

    if(g_pMain->DeleteServerPacket( EGS_SEND_LETTER_ACK ))
    {
        if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
        {
            g_pData->GetMyUser()->GetSelectUnit()->AccessInventory().UpdateInventorySlotList( kEvent.m_vecKInventorySlotInfo );
            g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_ED		= kEvent.m_iED;

            UpdateInventorySlotList( kEvent.m_vecKInventorySlotInfo );

            SetInventorySort();            

            InitPostSend();

            g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250, 300), GET_STRING( STR_ID_419 ), (CKTDXStage*) m_pNowState );

			if ( NULL != g_pData && NULL != g_pData->GetUIManager() && NULL != g_pData->GetUIManager()->GetUIInventory() )
			{
				g_pData->GetUIManager()->GetUIInventory()->SetEDString( kEvent.m_iED);
			}
            return true;
        }
    }

    return false;
}

bool CX2PostBox::Handler_EGS_DELETE_LETTER_REQ()
{
    KEGS_DELETE_LETTER_REQ kPacket;

    UidType postID = 0;
    m_nUnReadDeleteMail = 0;
    for(int i=0; i<6; ++i)
    {      
        if(m_listSlot[i].m_bChecked == true)
        {
            kPacket.m_vecPostNo.push_back(m_listSlot[i].m_postID);
            if(m_listSlot[i].m_bRead == false)
            {
                ++m_nUnReadDeleteMail;
            }
        }        
    }
    
    g_pData->GetServerProtocol()->SendPacket( EGS_DELETE_LETTER_REQ, kPacket );
    g_pMain->AddServerPacket( EGS_DELETE_LETTER_ACK, 60.0f );    

    if(m_bProcess)
    {
        m_pProcessDlg = g_pMain->KTDGUIMsgBox(D3DXVECTOR2(250, 300), GET_STRING( STR_ID_420 ), (CKTDXStage*) m_pNowState );
        //m_pProcessDlg->SetLayer(XDL_POP_UP);        
    }    

    return true;
}

bool CX2PostBox::Handler_EGS_DELETE_LETTER_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    m_bProcess = false;

    if(m_pProcessDlg != NULL)
    {
        g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pProcessDlg, NULL, false );        
    }
    m_pProcessDlg = NULL;  

    // 패킷 처리
    KSerBuffer* pBuff = (KSerBuffer*)lParam;
    KEGS_DELETE_LETTER_ACK kEvent;
    DeSerialize( pBuff, &kEvent );	

    if(g_pMain->DeleteServerPacket( EGS_DELETE_LETTER_ACK ))
    {
        if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
        {
            g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250, 300), GET_STRING( STR_ID_421 ), (CKTDXStage*) m_pNowState );

            m_pDlgMailView->SetShowEnable(false, false);
            SetSlotShowEnable(false, 3);
            m_nCurrentPage = 1;
            RequestList();

            m_nUnReadMail -= m_nUnReadDeleteMail;
            m_nUnReadDeleteMail = 0;            
            if(m_nUnReadMail < 0)
                m_nUnReadMail = 0;


#ifdef SERV_RELATIONSHIP_SYSTEM
			if( TRUE == IsSystemMail() )
#else  // SERV_RELATIONSHIP_SYSTEM
			if(m_bSystemMail)
#endif // SERV_RELATIONSHIP_SYSTEM          
            {

                --m_nUnReadSystem;
                if(m_nUnReadSystem < 0)
                    m_nUnReadSystem = 0;
            }
#ifdef SERV_RELATIONSHIP_SYSTEM			
			else if ( TRUE == IsWeddingMail() )
			{
				--m_nUnReadWedding;
				if(m_nUnReadWedding < 0)
					m_nUnReadWedding = 0;
			}
#endif // SERV_RELATIONSHIP_SYSTEM

            else
            {
                --m_nUnReadCommon;
                if(m_nUnReadCommon < 0)
                    m_nUnReadCommon = 0;
            }
            
            CX2SlotItem* pSlotItem = (CX2SlotItem*)GetSlot( CX2SlotItem::ST_POST_RECEIVE, 101 );
            if ( pSlotItem != NULL )
            {
                pSlotItem->DestroyItemUI();   
            }            

			UpdateUnReadMailNotice();

            return true;
        }
    }

    return false;
}

bool CX2PostBox::Handler_EGS_READ_LETTER_REQ(UidType postId)
{
    KEGS_READ_LETTER_REQ kPacket;

    kPacket.m_iPostNo = postId;

    g_pData->GetServerProtocol()->SendPacket( EGS_READ_LETTER_REQ, kPacket );
    g_pMain->AddServerPacket( EGS_READ_LETTER_ACK, 60.0f );    

    if(m_bProcess)
    {
        m_pProcessDlg = g_pMain->KTDGUIMsgBox(D3DXVECTOR2(250, 300), GET_STRING( STR_ID_422 ), (CKTDXStage*) m_pNowState );
        //m_pProcessDlg->SetLayer(XDL_POP_UP);        
    }   

    return true;
}

bool CX2PostBox::Handler_EGS_READ_LETTER_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    m_bProcess = false;
	
    if(m_pProcessDlg != NULL)
    {
        g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pProcessDlg, NULL, false );        
    }
    m_pProcessDlg = NULL;  

    // 패킷 처리
    KSerBuffer* pBuff = (KSerBuffer*)lParam;
    KEGS_READ_LETTER_ACK kEvent;
    
    DeSerialize( pBuff, &kEvent );

    if(g_pMain->DeleteServerPacket( EGS_READ_LETTER_ACK ))
    {
        if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
        {
			//{{ 2009. 6. 24  최육사	편지스트링조합
			std::wstring wstrNickName = GetSelectedUserName();
			
			g_pData->GetRewardTable()->MakeSystemLetter( wstrNickName, kEvent.m_kLetter );
			//}}

            int index = SetMailBody(kEvent.m_kLetter);
            if(index >= 0)
            {
                SetMailView(index);
            }
            else
            {
                g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250, 300), GET_STRING( STR_ID_423 ), (CKTDXStage*) m_pNowState );
            }
            return true;
        }
    }

    return false;
}

bool CX2PostBox::Handler_EGS_GET_POST_LETTER_LIST_REQ()
{
    KEGS_GET_POST_LETTER_LIST_REQ kPacket;

#ifdef SERV_RELATIONSHIP_SYSTEM
	kPacket.m_cLetterListType = m_cLetterListType;
#else // SERV_RELATIONSHIP_SYSTEM
	kPacket.m_bSystemLetter = m_bSystemMail;
#endif // SERV_RELATIONSHIP_SYSTEM
	kPacket.m_iPostPageNo = m_nCurrentPage;

	g_pData->GetServerProtocol()->SendPacket( EGS_GET_POST_LETTER_LIST_REQ, kPacket );
    g_pMain->AddServerPacket( EGS_GET_POST_LETTER_LIST_ACK, 60.0f );    

    if(m_bProcess)
    {
        m_pProcessDlg = g_pMain->KTDGUIMsgBox(D3DXVECTOR2(250, 300), GET_STRING( STR_ID_424 ), (CKTDXStage*) m_pNowState );
        //m_pProcessDlg->SetLayer(XDL_POP_UP);           
    }    

    return true;
}

bool CX2PostBox::Handler_EGS_GET_POST_LETTER_LIST_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    m_bProcess = false;

    if(m_pProcessDlg != NULL)
    {
        g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pProcessDlg, NULL, false );        
    }
    m_pProcessDlg = NULL;  

    // 패킷 처리
    KSerBuffer* pBuff = (KSerBuffer*)lParam;
    KEGS_GET_POST_LETTER_LIST_ACK kEvent;

    DeSerialize( pBuff, &kEvent );
    
    if(g_pMain->DeleteServerPacket( EGS_GET_POST_LETTER_LIST_ACK ))
    {
        if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
        {
			//{{ 2009. 6. 24  최육사	편지스트링조합
			std::wstring wstrNickName = GetSelectedUserName();
			//}}

            ClearMailList();
            ClearListSlot();

            m_nMaxPage = ((kEvent.m_iTotalLetterCount - 1) / 6) + 1;
            m_nMaxSlot = kEvent.m_vecPostItemTitle.size();

            SetListPage();            


			int iIndex = 0;
			BOOST_FOREACH( OUT KPostItemTitleInfo& kPostInfo, kEvent.m_vecPostItemTitle )
            {
				//{{ 2009. 6. 24  최육사	편지스트링조합
				g_pData->GetRewardTable()->MakeSystemLetterTitle( wstrNickName, kPostInfo );
				//}}
				LimitPostStringLength( kPostInfo.m_wstrTitle, kPostInfo.m_wstrFromNickName );

                if(SetMailHeader(iIndex, kPostInfo))
                    SetListSlot(iIndex);

				++iIndex;
            }

            WCHAR mailCount[10];
            CKTDGUIStatic *pStaticDlg = (CKTDGUIStatic*)m_pDlgMailReceive->GetControl(L"g_pStaticpost_Mail_Num");    // 메일카운트            
            
#ifdef SERV_RELATIONSHIP_SYSTEM
            if( IsSystemMail() == false )
#else  // SERV_RELATIONSHIP_SYSTEM
			if( m_bSystemMail == false)
#endif // SERV_RELATIONSHIP_SYSTEM            
			{
				StringCchPrintf(mailCount, 10, L"%d/30", kEvent.m_iTotalLetterCount);
                //wsprintf(mailCount, L"%d/30", kEvent.m_iTotalLetterCount);
                pStaticDlg->GetString(0)->msg = mailCount;
                m_nTotalMail = kEvent.m_iTotalLetterCount;
            }
            else
            {
                pStaticDlg->GetString(0)->msg = L"";
            }

            return true;
        }
    }    

    return false;
}

bool CX2PostBox::Handler_EGS_GET_ITEM_FROM_LETTER_REQ(UidType postId)
{
    KEGS_GET_ITEM_FROM_LETTER_REQ kPacket;

    kPacket.m_iPostNo = postId;
    
    g_pData->GetServerProtocol()->SendPacket( EGS_GET_ITEM_FROM_LETTER_REQ, kPacket );
    g_pMain->AddServerPacket( EGS_GET_ITEM_FROM_LETTER_ACK, 60.0f );    

    if(m_bProcess)
    {
        m_pProcessDlg = g_pMain->KTDGUIMsgBox(D3DXVECTOR2(250, 300), GET_STRING( STR_ID_425 ), (CKTDXStage*) m_pNowState );
        //m_pProcessDlg->SetLayer(XDL_POP_UP);        
    }    

    return true;
}

bool CX2PostBox::Handler_EGS_GET_ITEM_FROM_LETTER_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    m_bProcess = false;

    if(m_pProcessDlg != NULL)
    {
        g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pProcessDlg, NULL, false );
    }
    m_pProcessDlg = NULL;  

    // 패킷 처리
    KSerBuffer* pBuff = (KSerBuffer*)lParam;
    KEGS_GET_ITEM_FROM_LETTER_ACK kEvent;
    DeSerialize( pBuff, &kEvent );	

    if(g_pMain->DeleteServerPacket( EGS_GET_ITEM_FROM_LETTER_ACK ))
    {
        if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
        {
            g_pData->GetMyUser()->GetSelectUnit()->AccessInventory().UpdateInventorySlotList( kEvent.m_vecKInventorySlotInfo );  
            g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_ED		= kEvent.m_iED;

			if ( NULL != g_pData && NULL != g_pData->GetUIManager() && NULL != g_pData->GetUIManager()->GetUIInventory() )
			{
				g_pData->GetUIManager()->GetUIInventory()->SetEDString( kEvent.m_iED);
			}
            UpdateInventorySlotList( kEvent.m_vecKInventorySlotInfo );
            //SetInventorySort();               


            CX2SlotItem* pSlotItem = (CX2SlotItem*)GetSlot( CX2SlotItem::ST_POST_RECEIVE, 101 );
            if ( pSlotItem != NULL )
            {
                pSlotItem->DestroyItemUI();   
            }            
#ifdef SERV_RELATIONSHIP_SYSTEM
			if( kEvent.m_cLetterType != KPostItemInfo::LT_POST_OFFICE )		/// 시스템 우편일 때
#else // SERV_RELATIONSHIP_SYSTEM
			if(kEvent.m_bSystemLetter == true)
#endif // SERV_RELATIONSHIP_SYSTEM
			{
                // 시스템 보상 획득
                wstring strResultItem = L"";
                std::map< int, int >::iterator iter;
                
#ifdef DIVIDE_MAIL_ITEM_NAME
				bool bIsFirstCheck = true;			/// 처음 검사 여부

				for ( iter = kEvent.m_mapResultItem.begin(); iter != kEvent.m_mapResultItem.end(); ++iter )
				{
					if( false == bIsFirstCheck )	/// 만약 두번 이상 검사한다면 아이템이 두개 이상이라는 뜻이므로, 띄어쓰기로 구분해 주자.
						strResultItem = strResultItem + L",\n";
					else 
						bIsFirstCheck = false;		/// 처음 검사 여부 해제
#else  DIVIDE_MAIL_ITEM_NAME
				for ( iter = kEvent.m_mapResultItem.begin(); iter != kEvent.m_mapResultItem.end(); ++iter )
				{
#endif DIVIDE_MAIL_ITEM_NAME
					int resultItem1 = iter->first;
					int resultItem2 = iter->second;

                    wstring itemName = g_pData->GetItemManager()->GetItemTemplet(resultItem1)->GetFullName_();
					strResultItem = strResultItem + itemName;
#ifndef DIVIDE_MAIL_ITEM_NAME	/// 띄어쓰기로 구분하지 말자.
					strResultItem = strResultItem + L" ";
#endif DIVIDE_MAIL_ITEM_NAME
				}

                strResultItem += GET_STRING( STR_ID_426 );                

                g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250, 300), strResultItem.c_str(), (CKTDXStage*) m_pNowState );
                m_pDlgMailView->SetShowEnable(false, false);
                SetSlotShowEnable(false, 3);
                m_nCurrentPage = 1;
                RequestList();
            }
            else
            {
                // 일반 아이템 획득
                g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250, 300), GET_STRING( STR_ID_427 ), (CKTDXStage*) m_pNowState );            

                CKTDGUIStatic *pStaticDlg = (CKTDGUIStatic*)m_pDlgMailView->GetControl(L"g_pStaticpost_window_Font3");    // 첨부
                wstring itemName = GET_STRING( STR_ID_401 );        
                pStaticDlg->GetString(0)->msg = itemName;     // 첨부물

                int inx = -1;
                for(int i=0; i<6; ++i)
                {
                    if(m_listSlot[i].m_postID == kEvent.m_iPostNo)
                    {                        
                        if(m_listSlot[i].m_pItem != NULL)
                            SAFE_DELETE( m_listSlot[i].m_pItem );
                        m_listSlot[i].m_ItemId = 0;
                        m_listSlot[i].m_bAttachItem = false;
                        m_listSlot[i].m_nAttachMoney = 0;
                        SetListSlot(i);
                        break;
                    }
                }
            }            
            
            return true;
        }
    }

    return false;
}

bool CX2PostBox::Handler_EGS_RECEIVE_LETTER_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{   
    // 패킷 처리
    KSerBuffer* pBuff = (KSerBuffer*)lParam;
    KEGS_RECEIVE_LETTER_NOT kEvent;

    DeSerialize( pBuff, &kEvent );	

    if(kEvent.m_cScriptType == KPostItemInfo::LT_POST_OFFICE || kEvent.m_cScriptType == KPostItemInfo::LT_NONE )
	{        
        m_bNewLetterisSystem = false;
	}
    else
    {
		m_bNewLetterisSystem = true;
	}  

	KPostSmallTitleInfo unReadMailInfo;
	unReadMailInfo.m_cScriptType = kEvent.m_cScriptType;
	unReadMailInfo.m_wstrLetterTitle = kEvent.m_wstrLetterTitle;

	if(m_vecUnReadMailTitle.size() >= 5)
	{
		m_vecUnReadMailTitle.erase(m_vecUnReadMailTitle.end() - 1);
	}

	m_vecUnReadMailTitle.insert(m_vecUnReadMailTitle.begin(), unReadMailInfo);

	UpdateUnReadMailNotice();

	//{{ kimhc // 2009-11-18 // 2009.11.19일 패치 이벤트용으로 임시 코드 추가( 끝나고 삭제! )
	const int iTempEventIndex = 179;

	if ( kEvent.m_iScriptIndex == iTempEventIndex )
	{
		switch ( g_pMain->GetNowStateID() )
		{
		case CX2Main::XS_SQUARE_GAME:
		case CX2Main::XS_PVP_GAME:
		case CX2Main::XS_DUNGEON_GAME:
			{
				CX2Main::ReservedMessagePopUp EventPopUp;

				EventPopUp.m_vecTargetState.push_back( CX2Main::XS_PVP_LOBBY );
				EventPopUp.m_vecTargetState.push_back( CX2Main::XS_PVP_ROOM );
				EventPopUp.m_vecTargetState.push_back( CX2Main::XS_DUNGEON_ROOM );
				EventPopUp.m_vecTargetState.push_back( CX2Main::XS_VILLAGE_MAP );
				EventPopUp.m_vecTargetState.push_back( CX2Main::XS_SQUARE_GAME );
				EventPopUp.m_vecTargetState.push_back( CX2Main::XS_TRAINING_SCHOOL );

				EventPopUp.m_Message = GET_STRING( STR_ID_4774 );

				g_pMain->AddReservedMessagePopup( EventPopUp );
			}
			break;

		default:
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999, -999), GET_STRING( STR_ID_4774 ), (CKTDXStage*) m_pNowState ); 
			}
			break;
		}
	}
	//}} kimhc // 2009-11-18 // 2009.11.19일 패치 이벤트용으로 임시 코드 추가( 끝나고 삭제! )
	



    return true;
}

// 블랙리스트 등록
bool CX2PostBox::Handler_EGS_NEW_POST_BLACK_LIST_REQ()
{
    KEGS_NEW_POST_BLACK_LIST_REQ kPacket;

    CKTDGUIIMEEditBox *pDlgUser = (CKTDGUIIMEEditBox*)m_pDLGAddBlackList->GetControl(L"IMEID"); 
    wstring strUser = L"";
    
    if(pDlgUser != NULL)
        strUser = pDlgUser->GetText();

    kPacket.m_wstrNickName = strUser;

    g_pData->GetServerProtocol()->SendPacket( EGS_NEW_POST_BLACK_LIST_REQ, kPacket );
    g_pMain->AddServerPacket( EGS_NEW_POST_BLACK_LIST_ACK, 30.0f );    
    
    if(m_bProcess)
    {
        m_pProcessDlg = g_pMain->KTDGUIMsgBox(D3DXVECTOR2(250, 300), GET_STRING( STR_ID_428 ), (CKTDXStage*) m_pNowState );
        //m_pProcessDlg->SetLayer(XDL_POP_UP);        
    } 

    return true;
}

bool CX2PostBox::Handler_EGS_NEW_POST_BLACK_LIST_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    m_bProcess = false;

    if(m_pProcessDlg != NULL)
    {
        g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pProcessDlg, NULL, false );        
    }
    m_pProcessDlg = NULL;  

    // 패킷 처리
    KSerBuffer* pBuff = (KSerBuffer*)lParam;
    KEGS_NEW_POST_BLACK_LIST_ACK kEvent;

    DeSerialize( pBuff, &kEvent );	


    if(g_pMain->DeleteServerPacket( EGS_NEW_POST_BLACK_LIST_ACK ))
    {
        if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
        {
            g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250, 300), GET_REPLACED_STRING( ( STR_ID_429, "L", kEvent.m_wstrNickName ) ), (CKTDXStage*) m_pNowState ); 

            // 차단목록 갱신
            AddUser(kEvent.m_wstrNickName);
            ShowBlackList();
            return true;
        }
        else
        {
            m_bBuffBList = false;
            m_vecBList.clear();
            m_bProcess = true;
            Handler_EGS_GET_POST_BLACK_LIST_REQ();
        }
    }

    return false;
}

// 블랙리스트 삭제
bool CX2PostBox::Handler_EGS_DEL_POST_BLACK_LIST_REQ()
{
    KEGS_DEL_POST_BLACK_LIST_REQ kPacket;

    CKTDGUIListBox *pDlgListbox = (CKTDGUIListBox*)m_pDlgMailSetting->GetControl(L"ListBoxChatContent");
    int selectedIndex = -1;
    wstring strNickName;

    if(pDlgListbox)
    {
        selectedIndex = pDlgListbox->GetSelectedIndex();
        if(selectedIndex < 0)
        {
            g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250, 300), GET_STRING( STR_ID_430 ), (CKTDXStage*) m_pNowState ); 
            m_bProcess = false;
            return false;
        }

        strNickName = pDlgListbox->GetSelectedItem(selectedIndex)->strText;

        kPacket.m_wstrNickName = strNickName;

        g_pData->GetServerProtocol()->SendPacket( EGS_DEL_POST_BLACK_LIST_REQ, kPacket );
        g_pMain->AddServerPacket( EGS_DEL_POST_BLACK_LIST_ACK, 30.0f ); 

        if(m_bProcess)
        {
            m_pProcessDlg = g_pMain->KTDGUIMsgBox(D3DXVECTOR2(250, 300), GET_STRING( STR_ID_431 ), (CKTDXStage*) m_pNowState );
            //m_pProcessDlg->SetLayer(XDL_POP_UP);        
        }        

        return true;
    }

    return false;
}

bool CX2PostBox::Handler_EGS_DEL_POST_BLACK_LIST_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    m_bProcess = false;

    if(m_pProcessDlg != NULL)
    {
        g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pProcessDlg, NULL, false );        
    }
    m_pProcessDlg = NULL;  

    // 패킷 처리
    KSerBuffer* pBuff = (KSerBuffer*)lParam;
    KEGS_DEL_POST_BLACK_LIST_ACK kEvent;

    DeSerialize( pBuff, &kEvent );	


    if(g_pMain->DeleteServerPacket( EGS_DEL_POST_BLACK_LIST_ACK ))
    {
        if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
        {
            g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250, 300), GET_REPLACED_STRING( ( STR_ID_432, "L", kEvent.m_wstrNickName ) ), (CKTDXStage*) m_pNowState ); 

            // 차단목록 갱신
            DeleteUser(kEvent.m_wstrNickName);
            ShowBlackList();
            return true;
        }
        else
        {
            m_bBuffBList = false;
            m_vecBList.clear();
            m_bProcess = true;
            Handler_EGS_GET_POST_BLACK_LIST_REQ();
        }
    }

    return false;
}


bool CX2PostBox::Handler_EGS_GET_POST_BLACK_LIST_REQ()
{        
    g_pData->GetServerProtocol()->SendID( EGS_GET_POST_BLACK_LIST_REQ );
    g_pMain->AddServerPacket( EGS_GET_POST_BLACK_LIST_ACK, 30.0f ); 

    if(m_bProcess)
    {
        m_pProcessDlg = g_pMain->KTDGUIMsgBox(D3DXVECTOR2(250, 300), GET_STRING( STR_ID_433 ), (CKTDXStage*) m_pNowState );
        //m_pProcessDlg->SetLayer(XDL_POP_UP);        
    }

    return true;
}

bool CX2PostBox::Handler_EGS_GET_POST_BLACK_LIST_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    m_bProcess = false;

    if(m_pProcessDlg != NULL)
    {
        g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pProcessDlg, NULL, false );        
    }
    m_pProcessDlg = NULL;  

    // 패킷 처리
    KSerBuffer* pBuff = (KSerBuffer*)lParam;
    KEGS_GET_POST_BLACK_LIST_ACK kEvent;

    DeSerialize( pBuff, &kEvent );	

    if(g_pMain->DeleteServerPacket( EGS_GET_POST_BLACK_LIST_ACK ))
    {        
        m_vecBList.clear();
        m_vecBList = kEvent.m_vecBlackList;
        m_bBuffBList = true;

        ShowBlackList();
        return true;        
    }

    return false;
}

bool CX2PostBox::Handler_EGS_NEW_POST_LETTER_INFO_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    // 패킷 처리
    KSerBuffer* pBuff = (KSerBuffer*)lParam;
    KEGS_NEW_POST_LETTER_INFO_NOT kEvent;

    DeSerialize( pBuff, &kEvent );	

    m_nUnReadCommon = kEvent.m_iNewUserLetterCount;
    m_nUnReadSystem = kEvent.m_iNewSystemLetterCount;

#ifdef SERV_RELATIONSHIP_SYSTEM
	m_nUnReadWedding = kEvent.m_iNewRelationshipLetterCount;
	m_nUnReadMail = m_nUnReadCommon + m_nUnReadSystem + m_nUnReadWedding;
#else // SERV_RELATIONSHIP_SYSTEM
	m_nUnReadMail = m_nUnReadCommon + m_nUnReadSystem;
#endif // SERV_RELATIONSHIP_SYSTEM

    m_nTotalMail = kEvent.m_iTotalLetterCount;

	m_vecUnReadMailTitle.clear();	
	m_vecUnReadMailTitle = kEvent.m_vecNewLetterTitle;
    return true;
}

bool CX2PostBox::Handler_EGS_RENEWAL_LETTER_REQ()
{
	if( m_bReNewalLetter == false )
	{
		g_pData->GetServerProtocol()->SendID( EGS_RENEWAL_LETTER_REQ );
		g_pMain->AddServerPacket( EGS_RENEWAL_LETTER_ACK, 30.0f ); 

		m_bReNewalLetter = true;
	}

	return true;
}

bool CX2PostBox::Handler_EGS_RENEWAL_LETTER_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// 패킷 처리
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_NEW_POST_LETTER_INFO_NOT kEvent;

	m_bReNewalLetter = false;

	DeSerialize( pBuff, &kEvent );	

	if(g_pMain->DeleteServerPacket( EGS_RENEWAL_LETTER_ACK ))
	{        
		m_nUnReadCommon = kEvent.m_iNewUserLetterCount;
		m_nUnReadSystem = kEvent.m_iNewSystemLetterCount;

#ifdef SERV_RELATIONSHIP_SYSTEM
		m_nUnReadWedding = kEvent.m_iNewRelationshipLetterCount;
		m_nUnReadMail = m_nUnReadCommon + m_nUnReadSystem + m_nUnReadWedding;
#else // SERV_RELATIONSHIP_SYSTEM
		m_nUnReadMail = m_nUnReadCommon + m_nUnReadSystem;
#endif // SERV_RELATIONSHIP_SYSTEM

		m_nTotalMail = kEvent.m_iTotalLetterCount;

		m_vecUnReadMailTitle.clear();			
		m_vecUnReadMailTitle = kEvent.m_vecNewLetterTitle;

		UpdateUnReadMailNotice();

		return true;        
	}

	return false;
}

void CX2PostBox::UpdateUnReadMailNotice()
{
	if( NULL != g_pInstanceData && NULL != g_pInstanceData->GetMiniMapUI()  )
	{
		g_pInstanceData->GetMiniMapUI()->UpdateMailNotice( m_vecUnReadMailTitle );
	}
}

//{{ 허상형 : [2010/8/25/] //	우편 수량 UI 개편
#ifdef POST_QUANTITY_NEW
void CX2PostBox::OpenRegisterQuantityDLG( D3DXVECTOR2 pos )
{

	SAFE_DELETE_DIALOG(m_pDLGItemEnroll);

	m_pDLGItemEnroll = new CKTDGUIDialog(g_pMain->GetNowState(), L"DLG_UI_An.lua" );

	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGItemEnroll );
	m_pDLGItemEnroll->SetPos( g_pMain->GetWindowPos( D3DXVECTOR2(1024-865, 768-633), D3DXVECTOR2(75,20)) );

	CKTDGUIStatic* pStaticPrice			= static_cast < CKTDGUIStatic* >( m_pDLGItemEnroll->GetControl( L"Static_Price" ) );
	CKTDGUIStatic* pStaticEDMark		= static_cast < CKTDGUIStatic* >( m_pDLGItemEnroll->GetControl( L"Static_EDMark" ) );

	pStaticPrice->SetShow(false);
	pStaticEDMark->SetShow(false);

	CKTDGUIButton*		pButtonOK		= static_cast< CKTDGUIButton* >( m_pDLGItemEnroll->GetControl( L"Button_OK" ) );
	CKTDGUIButton*		pButtonCancel	= static_cast< CKTDGUIButton* >( m_pDLGItemEnroll->GetControl( L"Button_Cancle" ) );
	CKTDGUIButton*		pButtonPlus		= static_cast< CKTDGUIButton* >( m_pDLGItemEnroll->GetControl( L"Button_Plus" ) );
	CKTDGUIButton*		pButtonMinus	= static_cast< CKTDGUIButton* >( m_pDLGItemEnroll->GetControl( L"Button_Minus" ) );
	CKTDGUIIMEEditBox*	pQuantity		= static_cast< CKTDGUIIMEEditBox* >( m_pDLGItemEnroll->GetControl( L"IME_Edit_Quantity" ) );

	pQuantity->SetText( GET_REPLACED_STRING( ( STR_ID_2632, "i", m_nQuantity ) ), true);
	pButtonOK->SetCustomMsgMouseUp( PBSUM_REGISTER_QUANTITY_CONFIRM );
	pButtonCancel->SetCustomMsgMouseUp( PBSUM_REGISTER_QUANTITY_CANCEL );
	pButtonPlus->SetCustomMsgMouseUp( PBSUM_REGISTER_QUANTITY_PLUS );
	pButtonMinus->SetCustomMsgMouseUp( PBSUM_REGISTER_QUANTITY_MINUS );
	pQuantity->SetCustomMsgChange( PBSUM_REGISTER_QUANTITY_UPDATE );
	pQuantity->SetCustomMsgEnter( PBSUM_REGISTER_QUANTITY_CONFIRM );
	m_pDLGItemEnroll->SetCloseCustomUIEventID( PBSUM_REGISTER_QUANTITY_CANCEL );
	pQuantity->RequestFocus();

}

void CX2PostBox::UpdateRegisterQuantityDLG( bool bReadIME )
{
	ASSERT( NULL != m_pDLGItemEnroll );
	if( NULL == m_pDLGItemEnroll )
		return; 

	CKTDGUIIMEEditBox* pQuantity = static_cast< CKTDGUIIMEEditBox* >( m_pDLGItemEnroll->GetControl( L"IME_Edit_Quantity" ) );

	if(bReadIME)
	{
		m_nQuantity = g_pMain->GetEDFromString( pQuantity->GetText() );
	}	

	CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItem( m_EnrollItemUid );

	if ( m_nQuantity <= 0 )
	{
		if(bReadIME)				// 입력창으로 받아온 경우
			m_nQuantity = 0;
		else						// << 를 누른 경우 : 0 아래면 max로 순환시킨다.
			m_nQuantity = pItem->GetItemTemplet()->GetQuantity();	
	}

	if ( m_nQuantity >= pItem->GetItemData().m_Quantity )
		m_nQuantity = pItem->GetItemData().m_Quantity;

	WCHAR buff[256] = {0};
	_itow( m_nQuantity, buff, 10 );

	wstring wstrNum = buff;
	pQuantity->SetText( wstrNum.c_str() );

}

#endif	//	POST_QUANTITY_NEW
//}} 허상형 : [2010/8/25/] //	우편 수량 UI 개편
// 우편 제목, 보낸이 길이 제한
void CX2PostBox::LimitPostStringLength( OUT wstring& wstrTitle_, OUT wstring& wstrFromNickName_ )
{
	const UINT uiTitleMaxLength = 14;
	const UINT uiFromNickNameMaxLength = 14;

	// 제한 길이보다 길다면
	if( wstrTitle_.length() >= uiTitleMaxLength )
	{
		wstrTitle_.erase( wstring::size_type(uiTitleMaxLength) );
		wstrTitle_ += L"...";
	}

	if( wstrFromNickName_.length() >= uiFromNickNameMaxLength )
	{
		wstrFromNickName_.erase( wstring::size_type(uiFromNickNameMaxLength) );
		wstrFromNickName_ += L"...";
	}
}

#ifdef SERV_FREE_MAIL_TICKET
int CX2PostBox::GetPostFee( IN int iED, IN const CX2Item* pItemData )
{
	int iFee = 0;

	if( NULL != g_pData && NULL != g_pData->GetMyUser() )
	{
		CX2Unit* pMyUnit = g_pData->GetMyUser()->GetSelectUnit();
		if( NULL != pMyUnit )
		{
			for( int iFreeMailTicketIDIndex = 0; iFreeMailTicketIDIndex < ARRAY_SIZE( _CONST_FREE_MAIL_TICKET_::arriFreeMailTicketID ); ++iFreeMailTicketIDIndex )
			{
				if( NULL != pMyUnit->GetInventory().GetItemByTID( _CONST_FREE_MAIL_TICKET_::arriFreeMailTicketID[iFreeMailTicketIDIndex] ) )
				{
					return iFee;
				}
			}
		}
	}

	if( iED > 0 )
	{
		iFee = (int)( iED * 0.05f );
	}
	else if( pItemData != NULL )
	{
		int price = (int)( pItemData->GetItemTemplet()->GetPrice() * 0.2f );
		int quantity = m_nQuantity; //pItemData->GetItemData().m_Quantity;

		iFee = (int)( ( price * quantity ) * 0.05f );
	}

	iFee += 200;

	return iFee;
}
#endif //SERV_FREE_MAIL_TICKET
