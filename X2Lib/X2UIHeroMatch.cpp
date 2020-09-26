#include "StdAfx.h"

#ifdef SERV_HERO_PVP_MANAGE_LIST
#include ".\X2UIHeroMatch.h"


CX2UIHeroMatch::CX2UIHeroMatch()
{
	m_pStage			= g_pMain->GetNowState();

	m_pDlgHeroMatch		= NULL;
	m_pDlgHeroAdd		= NULL;
	m_pDlgHeroPopup		= NULL;

	m_bShow				= false;
	m_SumDelta			= 0;
	m_iInfoMaxNum		= 0;
	InitUI();
}

CX2UIHeroMatch::~CX2UIHeroMatch()
{
	SAFE_DELETE_DIALOG( m_pDlgHeroMatch );
	SAFE_DELETE_DIALOG( m_pDlgHeroAdd );
	SAFE_DELETE_DIALOG( m_pDlgHeroPopup );
	m_vecHeroMatchUserListControls.clear();
}

void CX2UIHeroMatch::InitUI()
{
	CKTDGUIStatic* pStatic = NULL;

	D3DXVECTOR2 vPos = D3DXVECTOR2( 510.0f, 140.0f );

	m_pDlgHeroMatch = new CKTDGUIDialog( m_pStage, L"DLG_UI_Observ.lua", 0.07f );
	if( NULL != m_pDlgHeroMatch)
	{
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDlgHeroMatch );
		m_pDlgHeroMatch->SetPos( vPos );
		m_pDlgHeroMatch->SetShowEnable( false, false );
		m_pDlgHeroMatch->SetDisableUnderWindow( true );

		pStatic = static_cast<CKTDGUIStatic*>(m_pDlgHeroMatch->GetControl(L"Static_ALL"));
		if( NULL != pStatic)
			pStatic->GetString(0)->msg = GET_STRING(STR_ID_28991);
		pStatic = static_cast<CKTDGUIStatic*>(m_pDlgHeroMatch->GetControl(L"Static_Nickname"));
		if( NULL != pStatic)
			pStatic->GetString(0)->msg = GET_STRING(STR_ID_1127);
		pStatic = static_cast<CKTDGUIStatic*>(m_pDlgHeroMatch->GetControl(L"Static_Server"));
		if( NULL != pStatic)
			pStatic->GetString(0)->msg = GET_STRING(STR_ID_28992);
		pStatic = static_cast<CKTDGUIStatic*>(m_pDlgHeroMatch->GetControl(L"Static_Login"));
		if( NULL != pStatic)
			pStatic->GetString(0)->msg = GET_STRING(STR_ID_28993);
		pStatic = static_cast<CKTDGUIStatic*>(m_pDlgHeroMatch->GetControl(L"Static_Contact"));
		if( NULL != pStatic)
			pStatic->GetString(0)->msg = GET_STRING(STR_ID_28994);

		CKTDGUIListBox* pListBox = static_cast<CKTDGUIListBox*>(m_pDlgHeroMatch->GetControl( L"GameEdit_Help_Dev" ) );
		if( NULL != pListBox )
		{
			pListBox->AddItem(GET_STRING(STR_ID_28995), NULL);
			pListBox->AddItem(GET_STRING(STR_ID_28996), NULL);
			pListBox->AddItem(GET_STRING(STR_ID_28997),  NULL);
			pListBox->AddItem(L"", NULL);
			pListBox->AddItem(L"", NULL);
			pListBox->AddItem(L"", NULL);
			pListBox->AddItem(GET_STRING(STR_ID_28998), NULL);
			pListBox->AddItem(GET_STRING(STR_ID_28999), NULL);
			pListBox->AddItem(GET_STRING(STR_ID_29000), NULL);
			pListBox->AddItem(GET_STRING(STR_ID_29001), NULL);
			pListBox->AddItem(GET_STRING(STR_ID_29002), NULL);
		}
	}
	

	m_pDlgHeroAdd = new CKTDGUIDialog( m_pStage, L"DLG_UI_Observ2.lua", 0.07f );
	if( NULL != m_pDlgHeroAdd )
	{
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDlgHeroAdd );
		vPos.x = 200.0f;
		m_pDlgHeroAdd->SetPos( vPos );
		m_pDlgHeroAdd->SetShowEnable( false, false );
		m_pDlgHeroAdd->SetDisableUnderWindow( true );

		pStatic = static_cast<CKTDGUIStatic*>(m_pDlgHeroAdd->GetControl(L"Static_Nickname"));
		if( NULL != pStatic)
			pStatic->GetString(0)->msg = GET_STRING(STR_ID_1127);
		pStatic = static_cast<CKTDGUIStatic*>(m_pDlgHeroAdd->GetControl(L"Static_Server"));
		if( NULL != pStatic)
			pStatic->GetString(0)->msg = GET_STRING(STR_ID_28992);

		CKTDGUIComboBox* pComboBox = NULL;
		for(int i = 0;i < 9;i++)
		{
			pComboBox = static_cast<CKTDGUIComboBox*>(m_pDlgHeroAdd->GetControl(GET_REPLACED_STRING( (STR_ID_3738, "Li", std::wstring(L"Server_choice"), i+1 ))));
			if( NULL != pComboBox )
			{
				pComboBox->AddItem(GET_STRING(STR_ID_4836),(LPVOID)0);
				pComboBox->AddItem(GET_STRING(STR_ID_4835),(LPVOID)1);
			}
		}
	}
	

	m_pDlgHeroPopup = new CKTDGUIDialog( m_pStage, L"Observ_Menu.lua", 0.07f );
	if( NULL != m_pDlgHeroPopup )
	{
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDlgHeroPopup );
		m_pDlgHeroPopup->SetPos( vPos );
		m_pDlgHeroPopup->SetShowEnable( false, false );
		m_pDlgHeroPopup->SetDisableUnderWindow( true );
	}
		
}

bool CX2UIHeroMatch::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	bool bFlag = false;

	if( true == g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckFrontModalDlg( m_pDlgHeroMatch ) )
		return false;

	switch( uMsg )
	{
	case WM_MOUSEWHEEL:
		{ 
			if( m_pDlgHeroMatch != NULL && m_pDlgHeroMatch->GetIsMouseOver() ) 
			{
				CKTDGUIContolList* pControlList = static_cast<CKTDGUIContolList*>(m_pDlgHeroMatch->GetControl( L"ControlList" ));
				if( pControlList == NULL )
					return false;
				CKTDGUIScrollBar* pScroolBarY = pControlList->GetYScroolBar();
				if(pScroolBarY == NULL)
					return false;

				short zDelta = static_cast<short>(HIWORD(wParam));
				m_SumDelta += zDelta;

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
			}
		}
		break;

	case WM_MOUSEMOVE:
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_LBUTTONDBLCLK:
	case WM_MBUTTONDBLCLK:
	case WM_RBUTTONDBLCLK:
		{
			if( false == m_pDlgHeroMatch->GetIsMouseOver() )
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
			if( false == m_pDlgHeroMatch->GetIsMouseOver() )
			{
				return false;
			}
			g_pData->GetUIManager()->SetUILayerIndex(CX2UIManager::UI_MENU_HERO, true);
			bFlag = true;
		}
		break;
	default:
		break;
	}

	return bFlag;
}

bool CX2UIHeroMatch::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch(wParam)
	{
	case XHMUM_CLOSE:
		{
			SetOpen(false);
		} return true;

	case XHMUM_ADD_USER:
		{
			WCHAR wstrTemp[256];
			CKTDGUIIMEEditBox* pIMEName;
			for( int i = 0; i < 9; i++)
			{
				StringCchPrintf(wstrTemp, 256, L"%dst_Nickname", i+1);
				pIMEName = static_cast<CKTDGUIIMEEditBox*>( m_pDlgHeroAdd->GetControl( wstrTemp ) );
				if( NULL != pIMEName )
					pIMEName->ClearText();
			}

			m_pDlgHeroAdd->SetShowEnable( true, true );
		} return true;

	case XHMUM_DELETE_USER:
		{
			Handler_EGS_ADMIN_HERO_PVP_USER_REMOVE_REQ();
		} return true;

	case XHMUM_SHOW_COMMAND:
		{
			ToggleHelpList();
			//Handler_EGS_ADMIN_NOTIFY_HERO_PVP_USER_REQ();
		} return true;

	case XHMUM_CLOSE_ADD_USER:
		{
			m_pDlgHeroAdd->SetShowEnable( false, false );
		} return true;

	case XHMUM_ADD_USER_OK:
		{
			Handler_EGS_ADMIN_HERO_PVP_USER_ADD_REQ( hWnd, uMsg, wParam, lParam );
			m_pDlgHeroAdd->SetShowEnable( false, false );
		} return true;

	case XHMUM_SHOW_MENU:
		{
			if( NULL != m_pDlgHeroPopup )
			{
				m_iPickedUser = reinterpret_cast<CKTDGUIButton*>(lParam)->GetDummyInt(0);
				
				D3DXVECTOR2 vMousePos;
				vMousePos.x = static_cast<float>( g_pKTDXApp->GetDIManager()->GetMouse()->GetXPos() );
				vMousePos.y = static_cast<float>( g_pKTDXApp->GetDIManager()->GetMouse()->GetYPos() );
				vMousePos = g_pKTDXApp->MouseConvertByResolution( vMousePos );
				vMousePos.x -= 30.0f;
				vMousePos.y -= 30.0f;

				m_pDlgHeroPopup->SetPos( vMousePos );
				m_pDlgHeroPopup->SetShowEnable( true, true );
			}
		} return true;

	case XHMUM_WHISPER:
		{
			if( m_iPickedUser <= m_vecUserList.size() )
			{
				CX2GameEdit* pGameEdit = g_pMain->GetGameEdit() ;
				if( NULL != pGameEdit && NULL != pGameEdit->GetEditChatWindow() )
				{
					CKTDGUIIMEEditBox* pImeEdit = static_cast<CKTDGUIIMEEditBox*>( pGameEdit->GetEditChatWindow()->GetControl( L"IMEEditBoxGameEditCommand" ) );
					if( NULL != pImeEdit )
					{
						WCHAR temp[256] = {0,};
						int Server = static_cast<int>( m_vecUserList[m_iPickedUser].m_iServerGroupID );
						wstring NickName = m_vecUserList[m_iPickedUser].m_wstrNickName;
						StringCchPrintfW( temp, ARRAY_SIZE(temp), L"/obsw %i %s ", Server, NickName.c_str() );

						pGameEdit->SetShow( true );
						pGameEdit->SetEnable( true );
						pImeEdit->SetText( temp );
					}
				}
			}

		} return true;
	}

	return false;
}

bool CX2UIHeroMatch::UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch (wParam)
	{
	case EGS_ADMIN_HERO_PVP_USER_LIST_ACK:
		Handler_EGS_ADMIN_HERO_PVP_USER_LIST_ACK( hWnd, uMsg, wParam, lParam );
		return true;
		
	case EGS_ADMIN_HERO_PVP_USER_ADD_ACK:
		Handler_EGS_ADMIN_HERO_PVP_USER_ADD_ACK( hWnd, uMsg, wParam, lParam );
		return true;
		
	case EGS_ADMIN_HERO_PVP_USER_REMOVE_ACK:
		Handler_EGS_ADMIN_HERO_PVP_USER_REMOVE_ACK( hWnd, uMsg, wParam, lParam );
		return true;

	case EGS_ADMIN_HERO_PVP_USER_CONNECTION_NOT:
		Handler_EGS_ADMIN_HERO_PVP_USER_CONNECTION_NOT( hWnd, uMsg, wParam, lParam );
		return true;
	}

	return false;
}

void CX2UIHeroMatch::SetOpen( bool bCheck )
{
	m_bShow = bCheck;

	if( bCheck == true )
	{
		Handler_EGS_ADMIN_HERO_PVP_USER_LIST_REQ();
	}
	else
	{
		m_pDlgHeroMatch->SetShowEnable( false, false );
		m_pDlgHeroAdd->SetShowEnable( false, false );
		m_pDlgHeroPopup->SetShowEnable( false, false );
	}
}

bool CX2UIHeroMatch::Handler_EGS_ADMIN_NOTIFY_HERO_PVP_USER_REQ()
{
	KEGS_ADMIN_NOTIFY_HERO_PVP_USER_REQ kPacket;

	WCHAR temp[256] = {0,};
	CKTDGUICheckBox* pCheckBox = NULL;
	CKTDGUIStatic* pStatic = NULL;

	for(UINT i = 0;i < m_vecUserList.size(); i++)
	{
		StringCchPrintf( temp, 256, L"checkBox_%d", i );
		pCheckBox = static_cast<CKTDGUICheckBox*>( m_pDlgHeroMatch->GetControl( temp ) );
		if( true == pCheckBox->GetChecked() )
		{
			StringCchPrintf( temp, 256, L"Nickname_%d", i);
			pStatic = static_cast<CKTDGUIStatic*>( m_pDlgHeroMatch->GetControl( temp ) );
			if( NULL != pStatic )
			{
				kPacket.m_wstrNickName = pStatic->GetString( 0 )->msg;
			}

			StringCchPrintf( temp, 256, L"Server_%d", i);
			pStatic = static_cast<CKTDGUIStatic*>( m_pDlgHeroMatch->GetControl( temp ) );
			if( NULL != pStatic )
			{
				kPacket.m_iServerGroupID = pStatic->GetDummyInt( 0 );
			}
			break;
		}
	}
	
	if ( g_pData->GetServerProtocol() != NULL && 
#ifdef  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
        g_pData->GetServerProtocol()->IsUserProxyValid() == true
#else   X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
		g_pData->GetServerProtocol()->GetUserProxy() != NULL
#endif  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
        )
		g_pData->GetServerProtocol()->SendPacket( EGS_ADMIN_NOTIFY_HERO_PVP_USER_REQ, kPacket );
	
	return true;
}


bool CX2UIHeroMatch::Handler_EGS_ADMIN_HERO_PVP_USER_LIST_REQ()
{
	g_pData->GetServerProtocol()->SendID( EGS_ADMIN_HERO_PVP_USER_LIST_REQ );
	g_pMain->AddServerPacket( EGS_ADMIN_HERO_PVP_USER_LIST_ACK, 60.f ); 

	return true;
}

bool CX2UIHeroMatch::Handler_EGS_ADMIN_HERO_PVP_USER_LIST_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = reinterpret_cast<KSerBuffer*>(lParam);
	KEGS_ADMIN_HERO_PVP_USER_LIST_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	m_iInfoMaxNum = m_vecUserList.size();
	ClearList();
	m_vecUserList.clear();

	if( true == g_pMain->DeleteServerPacket( EGS_ADMIN_HERO_PVP_USER_LIST_ACK ) )
	{
		if( NULL != m_pDlgHeroMatch )
		{
			int i;
			for( i = 0; i < static_cast<int>(kEvent.m_vecUserList.size()); i++)
			{
				m_vecUserList.push_back(kEvent.m_vecUserList[i]);
				m_iInfoMaxNum++;
			}

			UpdateUserList();

		}
	}

	m_pDlgHeroMatch->SetShowEnable( true, true );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDlgHeroMatch, true );

	if( NULL != g_pData && NULL != g_pData->GetUIManager() )
	{
		g_pData->GetUIManager()->UIOpened(CX2UIManager::UI_MENU_HERO);
	}

	return false;
}


bool CX2UIHeroMatch::Handler_EGS_ADMIN_HERO_PVP_USER_ADD_REQ( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KEGS_ADMIN_HERO_PVP_USER_ADD_REQ kPacket;

	wstring wstrNickname = L"";
	UidType uidServer = -1;

	WCHAR wstrTemp[256] = {0,};
	CKTDGUIIMEEditBox* pIMEName = NULL;
	CKTDGUIComboBox* pComboServer = NULL;

	for( int i = 0; i < 9; i++)
	{
		StringCchPrintf(wstrTemp, 256, L"%dst_Nickname", i+1);
		pIMEName = static_cast<CKTDGUIIMEEditBox*>( m_pDlgHeroAdd->GetControl( wstrTemp ) );
		if( NULL != pIMEName )
			wstrNickname = pIMEName->GetText();

		StringCchPrintf(wstrTemp, 256, L"Server_choice%d", i+1);
		pComboServer = static_cast<CKTDGUIComboBox*>( m_pDlgHeroAdd->GetControl( wstrTemp ) );
		if( NULL != pComboServer )
			uidServer = pComboServer->GetSelectedItemIndex();


		if( L"" != wstrNickname && -1 != uidServer )
			kPacket.m_vecUserList.push_back( std::make_pair( uidServer, wstrNickname ) );

		pIMEName = NULL;
		pComboServer = NULL;
		wstrNickname = L"";
		uidServer = -1;
	}

	g_pData->GetServerProtocol()->SendPacket( EGS_ADMIN_HERO_PVP_USER_ADD_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_ADMIN_HERO_PVP_USER_ADD_ACK, 60.f ); 

	return true;
}


bool CX2UIHeroMatch::Handler_EGS_ADMIN_HERO_PVP_USER_ADD_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = reinterpret_cast<KSerBuffer*>(lParam);
	KEGS_ADMIN_HERO_PVP_USER_ADD_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	KHeroPvpUserInfo UserInfo;

	ClearList();

	if( true == g_pMain->DeleteServerPacket( EGS_ADMIN_HERO_PVP_USER_ADD_ACK ) )
	{
		for( UINT i = 0; i < kEvent.m_vecUserList.size(); i++ )
		{
			UserInfo.m_wstrNickName = kEvent.m_vecUserList[i].second;
			UserInfo.m_iServerGroupID = kEvent.m_vecUserList[i].first;
			m_vecUserList.push_back( UserInfo );
			m_iInfoMaxNum++;
		}
		UpdateUserList();

		return true;
	}

	return false;
}

bool CX2UIHeroMatch::Handler_EGS_ADMIN_HERO_PVP_USER_REMOVE_REQ()
{
	WCHAR temp[256] = {0,};
	CKTDGUICheckBox* pCheckBox = NULL;
	CKTDGUIStatic* pStatic = NULL;

	UidType uidServer = -1;
	wstring wstrNickname = L"";

	KEGS_ADMIN_HERO_PVP_USER_REMOVE_REQ kPacket;

	for(UINT i = 0;i < m_vecUserList.size(); i++)
	{
		StringCchPrintf( temp, 256, L"checkBox_%d", i );
		pCheckBox = static_cast<CKTDGUICheckBox*>( m_pDlgHeroMatch->GetControl( temp ) );
		if( true == pCheckBox->GetChecked() )
		{
			StringCchPrintf( temp, 256, L"Nickname_%d", i);
			pStatic = static_cast<CKTDGUIStatic*>( m_pDlgHeroMatch->GetControl( temp ) );
			if( NULL != pStatic )
			{
				wstrNickname = pStatic->GetString( 0 )->msg;
			}

			StringCchPrintf( temp, 256, L"Server_%d", i);
			pStatic = static_cast<CKTDGUIStatic*>( m_pDlgHeroMatch->GetControl( temp ) );
			if( NULL != pStatic )
			{
				uidServer = pStatic->GetDummyInt( 0 );
			}

			kPacket.m_vecUserList.push_back( make_pair( uidServer, wstrNickname ) );
		}
	}

	g_pData->GetServerProtocol()->SendPacket( EGS_ADMIN_HERO_PVP_USER_REMOVE_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_ADMIN_HERO_PVP_USER_REMOVE_ACK, 60.f ); 

	return true;
}

bool CX2UIHeroMatch::Handler_EGS_ADMIN_HERO_PVP_USER_REMOVE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = reinterpret_cast<KSerBuffer*>(lParam);
	KEGS_ADMIN_HERO_PVP_USER_REMOVE_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	m_iInfoMaxNum = m_vecUserList.size();
	ClearList();

	if( true == g_pMain->DeleteServerPacket( EGS_ADMIN_HERO_PVP_USER_REMOVE_ACK ) )
	{
		std::vector< KHeroPvpUserInfo >::const_iterator vitUser;
		for( UINT i = 0; i < kEvent.m_vecUserList.size(); i++ )
		{
			for( vitUser = m_vecUserList.begin(); vitUser != m_vecUserList.end(); )
			{
				if( 0 == StrCmp( vitUser->m_wstrNickName.c_str(), kEvent.m_vecUserList[i].second.c_str() ) )
					vitUser = m_vecUserList.erase( vitUser );
				else
					++vitUser;
			}
		}

		UpdateUserList();
		return true;
	}

	return false;
}

bool CX2UIHeroMatch::Handler_EGS_ADMIN_HERO_PVP_USER_CONNECTION_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = reinterpret_cast<KSerBuffer*>(lParam);
	KEGS_ADMIN_HERO_PVP_USER_CONNECTION_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	m_iInfoMaxNum = m_vecUserList.size();
	ClearList();

	UINT eventSize = kEvent.m_vecUserList.size();
	std::vector<UINT> IsCheck;

	UINT count = 0;

	for( UINT i = 0; i < eventSize; i++ )
	{
		IsCheck.push_back(0);
	}

	std::vector< KHeroPvpUserInfo > vecAddUser;
	std::vector< KHeroPvpUserInfo >::iterator vitUser;

	for( vitUser = m_vecUserList.begin(); vitUser != m_vecUserList.end(); )
	{
		for( UINT i = 0; i < eventSize; i++ )
		{
			if( 0 == StrCmp( vitUser->m_wstrNickName.c_str(), kEvent.m_vecUserList[i].second.m_wstrNickName.c_str() ) )
			{
				IsCheck[i] = 2;

				if( true == kEvent.m_vecUserList[i].first )
				{
					vitUser->m_bOn = kEvent.m_vecUserList[i].second.m_bOn;
					vitUser->m_bConnect = kEvent.m_vecUserList[i].second.m_bConnect;
					++vitUser;
					break;
				}
				else
				{
					vitUser = m_vecUserList.erase( vitUser );
					break;
				}		
			}
			else
			{
				if( true == kEvent.m_vecUserList[i].first && 2 > IsCheck[i] )
				{
					IsCheck[i] = 1;
				}
				++count;
			}
		}

		if( count == eventSize )
			++vitUser;

		count = 0;
	}

	for( UINT i = 0; i < eventSize; ++i )
	{
		if( 1 == IsCheck[i] )
		{
			m_vecUserList.push_back( kEvent.m_vecUserList[i].second );
		}
	}

	UpdateUserList();

	return true;

}

void CX2UIHeroMatch::ClearList()
{
	if( NULL == m_pDlgHeroMatch )
		return;

	CKTDGUIContolList* pControlList = static_cast<CKTDGUIContolList*>( m_pDlgHeroMatch->GetControl( L"ControlList" ) );
	if( NULL == pControlList )
		return;

	m_pDlgHeroMatch->DeleteControl( L"ControlList" );
	pControlList = NULL;
	WCHAR temp[256] = {0,};

	CKTDGUICheckBox* pCheck = NULL;
	CKTDGUIStatic* pStaticNickname = NULL;
	CKTDGUIStatic* pStaticServer = NULL;
	CKTDGUICheckBox* pCheckLogin = NULL;
	CKTDGUICheckBox* pCheckChannel = NULL;
	CKTDGUIButton* pButton = NULL;

	for(UINT i = 0;i < m_iInfoMaxNum ;i++)
	{
		StringCchPrintf( temp, 256, L"checkBox_%d", i );
		pCheck = static_cast<CKTDGUICheckBox*>( m_pDlgHeroMatch->GetControl( temp ) );
		if( NULL !=	 pCheck )
		{
			pCheck->SetOutOfControlList(true);
			m_pDlgHeroMatch->DeleteControl( temp );
		}
		
	
		StringCchPrintf( temp, 256, L"Nickname_%d", i );
		pStaticNickname = static_cast<CKTDGUIStatic*>( m_pDlgHeroMatch->GetControl( temp ) );
		if( NULL != pStaticNickname )
		{
			pStaticNickname->SetOutOfControlList( true );
			m_pDlgHeroMatch->DeleteControl( temp );
		}
		
		StringCchPrintf( temp, 256, L"MenuButton_%d", i );
		pButton = static_cast<CKTDGUIButton*>( m_pDlgHeroMatch->GetControl( temp ) );
		if( NULL != pButton )
		{
			pButton->SetOutOfControlList( true );
			m_pDlgHeroMatch->DeleteControl( temp );
		}

		StringCchPrintf( temp, 256, L"Server_%d", i );	
		pStaticServer = static_cast<CKTDGUIStatic*>( m_pDlgHeroMatch->GetControl( temp ) );
		if( NULL != pStaticServer)
		{
			pStaticServer->SetOutOfControlList( true );
			m_pDlgHeroMatch->DeleteControl( temp );
		}
		

		StringCchPrintf( temp, 256, L"Login_%d", i );
		pCheckLogin = static_cast<CKTDGUICheckBox*>( m_pDlgHeroMatch->GetControl( temp ) );
		if( NULL != pCheckLogin)
		{
			pCheckLogin->SetOutOfControlList( true );
			m_pDlgHeroMatch->DeleteControl( temp );
		}
		

		StringCchPrintf( temp, 256, L"Channel_%d", i );
		pCheckChannel = static_cast<CKTDGUICheckBox*>( m_pDlgHeroMatch->GetControl( temp ) );
		if( NULL != pCheckChannel)
		{
			pCheckChannel->SetOutOfControlList( true );
			m_pDlgHeroMatch->DeleteControl( temp );
		}
	}

	m_iInfoMaxNum = 0;

	pControlList = m_pDlgHeroMatch->CreateControlList();
	if( NULL != pControlList )
	{
		m_pDlgHeroMatch->AddControl(pControlList);
		pControlList->SetName( L"ControlList" );
		pControlList->SetYScollBarUpButtonTex_LUA( "DLG_Common_New_Texture61.tga", "Up_Arrow_Normal" );
		pControlList->SetYScollBarUpButtonDownTex_LUA( "DLG_Common_New_Texture61.tga", "Up_Arrow_Over" );
		pControlList->SetYScollBarDownButtonTex_LUA( "DLG_Common_New_Texture61.tga", "Down_Arrow_Normal" );
		pControlList->SetYScollBarDownButtonDownTex_LUA( "DLG_Common_New_Texture61.tga", "Down_Arrow_Over" );
		pControlList->SetYScollBarThumbTex_LUA( "DLG_Common_New_Texture61.tga", "Scroll_Bar" );
		pControlList->SetYScollBarTrackTex_LUA( "DLG_Room_Button0.tga", "Invisible" );
		pControlList->InitYScrollBar_LUA();
		pControlList->SetYScrollBarPos( 382, 110 );
		pControlList->SetYScrollBarSize( 20, 304 );		
		pControlList->SetScrollOffset( 0, 29 );
		pControlList->SetPos( D3DXVECTOR2( 0, 0 ) );
		pControlList->SetViewSize( 6, 9, true );
	}
}

void CX2UIHeroMatch::UpdateUserList()
{
	if( NULL == m_pDlgHeroMatch )
		return;

	//ClearList();

	WCHAR temp[256] = {0,};

	CKTDGUIContolList* pControlList = static_cast<CKTDGUIContolList*>( m_pDlgHeroMatch->GetControl( L"ControlList" ) );
	if( NULL == pControlList )
		return;
	

	for(UINT i = 0; i < m_vecUserList.size(); i++ )
	{
		m_pDlgHeroMatch->OpenScriptFile( L"DLG_UI_Observ_ControlList.lua" );

		CKTDGUICheckBox* pCheck = static_cast<CKTDGUICheckBox*>( m_pDlgHeroMatch->GetControl( L"st_check" ) );
		if( NULL == pCheck )
			return;

		CKTDGUIStatic* pStaticNickname = static_cast<CKTDGUIStatic*>( m_pDlgHeroMatch->GetControl( L"st_Nickname" ) );
		if( NULL == pStaticNickname )
			return;

		CKTDGUIStatic* pStaticServer = static_cast<CKTDGUIStatic*>( m_pDlgHeroMatch->GetControl( L"st_Server" ) );
		if( NULL == pStaticServer )
			return;

		CKTDGUICheckBox* pCheckLogin = static_cast<CKTDGUICheckBox*>( m_pDlgHeroMatch->GetControl( L"st_login" ) );
		if( NULL == pCheckLogin )
			return;

		CKTDGUICheckBox* pCheckChannel = static_cast<CKTDGUICheckBox*>( m_pDlgHeroMatch->GetControl( L"st_channel" ) );
		if( NULL == pCheckChannel )
			return;
		CKTDGUIButton* pButtonMenu = static_cast<CKTDGUIButton*>( m_pDlgHeroMatch->GetControl( L"st_button" ) );
		if( NULL == pButtonMenu )
			return;

		StringCchPrintf( temp, 256, L"checkBox_%d", i );
		pCheck->SetName( temp );
		pCheck->AddDummyInt( i );

		StringCchPrintf( temp, 256, L"Nickname_%d", i );
		pStaticNickname->SetName( temp );
		pStaticNickname->AddDummyInt( i );
		pStaticNickname->SetString( 0, m_vecUserList[i].m_wstrNickName.c_str() );

		StringCchPrintf( temp, 256, L"Server_%d", i );
		pStaticServer->SetName( temp );
#ifdef EXTEND_SERVER_GROUP_MASK
#else EXTEND_SERVER_GROUP_MASK
		switch( m_vecUserList[i].m_iServerGroupID )
		{
		case SEnum::SGI_SOLES:
			pStaticServer->SetString( 0, GET_STRING(STR_ID_4836) );
			pStaticServer->AddDummyInt( SEnum::SGI_SOLES );
			break;
		case SEnum::SGI_GAIA:
			pStaticServer->SetString( 0, GET_STRING(STR_ID_4835) );
			pStaticServer->AddDummyInt( SEnum::SGI_GAIA );
			break;
		default:
			break;
		}
#endif EXTEND_SERVER_GROUP_MASK
		
		StringCchPrintf( temp, 256, L"MenuButton_%d", i );
		pButtonMenu->SetName( temp );
		pButtonMenu->AddDummyInt( i );

		StringCchPrintf( temp, 256, L"Login_%d", i );
		pCheckLogin->SetName( temp );
		pCheckLogin->AddDummyInt( i );
		pCheckLogin->SetEnable( false );
		pCheckLogin->SetChecked( m_vecUserList[i].m_bOn );

		StringCchPrintf( temp, 256, L"Channel_%d", i );
		pCheckChannel->SetName( temp );
		pCheckChannel->SetEnable( false );
		pCheckChannel->AddDummyInt( i );

		pControlList->InsertItem( pCheck, i ,0 );
		pControlList->InsertItem( pStaticNickname, i, 1 );
		pControlList->InsertItem( pButtonMenu, i, 2 );
		pControlList->InsertItem( pStaticServer, i, 3 );
		pControlList->InsertItem( pCheckLogin, i, 4 );
		pControlList->InsertItem( pCheckChannel, i, 5 );

		m_iInfoMaxNum++;
	}

	pControlList->SetIndex( 0, 0);
}

void CX2UIHeroMatch::ToggleHelpList()
{	
	CKTDGUIListBox* pListBox = static_cast<CKTDGUIListBox*>(m_pDlgHeroMatch->GetControl( L"GameEdit_Help_Dev" ) );
	CKTDGUIStatic* pStatic = static_cast<CKTDGUIStatic*>(m_pDlgHeroMatch->GetControl( L"StaticGameEditCommandBG" ) );

	if( pStatic != NULL )
	{
		pStatic->SetShow( !pStatic->GetShow() );
		pStatic->SetEnable( !pStatic->GetEnable() );
	}

	if( NULL != pListBox )
	{
		pListBox->SetShow( !pListBox->GetShow() );
		pListBox->SetEnable( !pListBox->GetEnable() );
	}
}

#endif //SERV_HERO_PVP_MANAGE_LIST