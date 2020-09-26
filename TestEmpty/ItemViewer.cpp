#include "dxstdafx.h"
#include ".\itemviewer.h"

CItemViewer::CItemViewer(void)
{
	OpenScriptFile( L"Enum.lua" );
	OpenScriptFile( L"Config.lua" );

	lua_tinker::class_add<CX2ItemManager>( g_pKTDXApp->GetLuaBinder()->GetLuaState(), "CX2ItemManager" );
	lua_tinker::class_def<CX2ItemManager>( g_pKTDXApp->GetLuaBinder()->GetLuaState(), "AddItemTemplet",				CX2ItemManager::AddItemTemplet_LUA );

	m_pDialog = new CKTDGUIDialog( this, L"DLG_ItemViewer.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDialog );

	m_NowViewPage = 1;
	m_NowPrintPage = 1;

	m_QuantityPerPage = 24;

	m_pItemManager = new CX2ItemManager();
	m_pItemManager->OpenScriptFile( L"Item.lua" );

	for ( int widthIndex = 0; widthIndex < 2; widthIndex++ )
	{
		for ( int heightIndex = 0; heightIndex < 12; heightIndex++ )
		{
			CKTDGUIDialogType pCKTDGUIDialog = new CKTDGUIDialog( this, L"DLG_ItemInfo.lua" );
			pCKTDGUIDialog->SetShowEnable( false, false );

			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( pCKTDGUIDialog );
			m_vecDLG.push_back( pCKTDGUIDialog );

			pCKTDGUIDialog->SetPos( D3DXVECTOR2( 30 + (widthIndex*500),180 + (heightIndex*50) ) );
		}
	}
/*
	for ( int i = 0; i < m_QuantityPerPage; i++ )
	{
		
	}
*/
	m_NowViewMaxPage = 1; 
	if ( (m_pItemManager->GetItemNum() % m_QuantityPerPage) == 0 )
	{
		m_NowViewMaxPage += ((m_pItemManager->GetItemNum() / m_QuantityPerPage) - 1);
	}
	else
	{
		m_NowViewMaxPage = ((m_pItemManager->GetItemNum() / m_QuantityPerPage) + 1);
	}


	m_pFont = g_pKTDXApp->GetDGManager()->GetFontManager()->CreateNewFont( L"굴림체" );

	ResetUI();
}

CItemViewer::~CItemViewer(void)
{
	SAFE_DELETE_DIALOG( m_pDialog );
	SAFE_DELETE( m_pItemManager );

	g_pKTDXApp->GetDGManager()->GetFontManager()->DestroyFont( m_pFont );
}

HRESULT CItemViewer::OnFrameMove( double fTime, float fElapsedTime )
{
	g_pKTDXApp->GetDGManager()->GetDialogManager()->OnFrameMove( fTime, fElapsedTime );

	if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_SYSRQ) == TRUE )
	{
		m_NowViewPage = 1;
		for ( ; m_NowViewPage < m_NowViewMaxPage; m_NowViewPage++ )
		{
			ResetUI();
			wstringstream wstrstm;
			wstrstm << L"ItemInfo" << m_NowViewPage << L".jpg";
			g_pKTDXApp->GetDGManager()->ScreenCapture( wstrstm.str().c_str() );

			g_pKTDXApp->OnFrameRender();
		}
	}

	return true;	
}

HRESULT CItemViewer::OnFrameRender()
{
	g_pKTDXApp->GetDGManager()->GetDialogManager()->OnFrameRender();

	return true;
}

HRESULT CItemViewer::OnResetDevice()
{
	return g_pKTDXApp->GetDGManager()->GetDialogManager()->OnResetDevice();
}

HRESULT CItemViewer::OnLostDevice()
{
	return g_pKTDXApp->GetDGManager()->GetDialogManager()->OnLostDevice();
}

bool CItemViewer::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( uMsg )
	{
	case CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT:
		{
			switch( wParam )
			{
			case VDM_PREV:
				{
					if ( m_NowViewPage > 1 )
					{
						m_NowViewPage--;
						ResetUI();
					}

					
				}
				break;

			case VDM_NEXT:
				{
					if ( m_NowViewPage < m_NowViewMaxPage )
					{
						m_NowViewPage++;
						ResetUI();
					}
				}
				break;

			case VDM_PRINT:
				{
					m_NowViewPage = 1;
					for ( ; m_NowViewPage < m_NowViewMaxPage; m_NowViewPage++ )
					{
						ResetUI();
						wstringstream wstrstm;
						wstrstm << L"ItemInfo" << m_NowViewPage << L".jpg";
						g_pKTDXApp->GetDGManager()->ScreenCapture( wstrstm.str().c_str() );

						g_pKTDXApp->OnFrameRender();
					}
				}
				break;
			}
		}
		break;
	}

	return g_pKTDXApp->GetDGManager()->GetDialogManager()->MsgProc( hWnd, uMsg, wParam, lParam );
}

void CItemViewer::ResetUI()
{
	//여기서 부터 또 겁나게 해볼까..


	for ( int i = 0; i < (int)m_vecDLG.size(); i++ )
	{
		CKTDGUIDialogType pCKTDGUIDialog = m_vecDLG[i];
		if ( pCKTDGUIDialog == NULL )
			continue;

		pCKTDGUIDialog->SetShowEnable( false, false );
	}

	vector< int > vecItemID;
	m_pItemManager->GetItemList( vecItemID, m_NowViewPage, m_QuantityPerPage );

	for ( int i = 0; i < (int)vecItemID.size(); i++ )
	{
		CX2Item::ItemTemplet* pItemTemplet = m_pItemManager->GetItemTemplet( vecItemID[i] );
		if ( pItemTemplet != NULL )
		{
			if ( i >= 0 && i < (int)m_vecDLG.size() )
			{
				CKTDGUIDialogType pDialog = m_vecDLG[i];
				if ( pDialog != NULL )
				{
					pDialog->SetShowEnable( true, true );
					//이름이랑 이미지랑 설명하고 구입가격 판매가격 표시

					CKTDGUIStatic* pStaticImage = (CKTDGUIStatic*)pDialog->GetControl( L"ItemImage" );
					if ( pStaticImage != NULL )
					{
                        if ( pStaticImage->GetPicture(0)->SetTex( pItemTemplet->GetShopImage() ) == false )
						{
							pStaticImage->GetPicture(0)->SetTex( L"HQ_Shop_Ui_Noimage.dds" );
						}
					}

					CKTDGUIStatic* pStaticName = (CKTDGUIStatic*)pDialog->GetControl( L"ItemName" );
					if ( pStaticName != NULL )
					{
						pStaticName->GetString(0)->msg = pItemTemplet->m_Name;	
					}

					CKTDGUIStatic* pStaticDesc = (CKTDGUIStatic*)pDialog->GetControl( L"ItemDesc" );
					if ( pStaticDesc != NULL )
					{
						if ( pItemTemplet->m_ItemType == CX2Item::IT_WEAPON )
						{
							bool bCheck = false;
							pStaticDesc->GetString(0)->msg = L"";
							
							//if ( pItemTemplet->m_Stat.m_fAtkPhysic != 0 )
							//{
								wstringstream wstrstm;
								wstrstm << L"물리 공격력 +" << pItemTemplet->m_Stat.m_fAtkPhysic;
								pStaticDesc->GetString(0)->msg += wstrstm.str().c_str();
								bCheck = true;
							//}
							//if ( pItemTemplet->m_Stat.m_fAtkMagic != 0 )
							//{
								if ( bCheck == true )
								{
									pStaticDesc->GetString(0)->msg += L"\n";
									
								}

								wstrstm.str( L"" );
								//wstringstream wstrstm;
								wstrstm << L"마법 공격력 +" << pItemTemplet->m_Stat.m_fAtkMagic;
								pStaticDesc->GetString(0)->msg += wstrstm.str().c_str();
							//}
						}
						else if (pItemTemplet->m_ItemType == CX2Item::IT_DEFENCE )
						{
							bool bCheck = false;
							pStaticDesc->GetString(0)->msg = L"";
							//if ( pItemTemplet->m_Stat.m_fDefPhysic != 0 )
							//{
								wstringstream wstrstm;
								wstrstm << L"물리 방어력 +" << pItemTemplet->m_Stat.m_fDefPhysic;
								pStaticDesc->GetString(0)->msg += wstrstm.str().c_str();
								bCheck = true;
							//}
							//if ( pItemTemplet->m_Stat.m_fDefMagic != 0 )
							//{
								if ( bCheck == true )
								{
									pStaticDesc->GetString(0)->msg += L"\n";

								}

								wstrstm.str( L"" );
								//wstringstream wstrstm;
								wstrstm << L"물리 방어력 +" << pItemTemplet->m_Stat.m_fDefMagic;
								pStaticDesc->GetString(0)->msg += wstrstm.str().c_str();
							//}
						}
						else
						{
							if ( pItemTemplet->m_Description.empty() == true )
							{
								pStaticDesc->GetString(0)->msg = L"설명 없음";
							}
							else
							{
								pStaticDesc->GetString(0)->msg = pItemTemplet->m_Description;	
							}
						}
						
					}


					
					CKTDGUIStatic* pStaticSellED = (CKTDGUIStatic*)pDialog->GetControl( L"ItemSellED" );
					if ( pStaticSellED != NULL )
					{
						wstringstream wstrstm;
						wstrstm << pItemTemplet->m_Price/2;
						pStaticSellED->GetString(0)->msg = wstrstm.str().c_str();
					}


				}
			}
		}
	}
}

bool CItemViewer::OpenScriptFile( const WCHAR* pFileName )
{
	KGCMassFileManager::CMassFile::MASSFILE_MEMBERFILEINFO_POINTER Info;
	Info = g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->LoadDataFile( pFileName );
	if( Info == NULL )
		return false;

	if( g_pKTDXApp->GetLuaBinder()->DoMemory( Info->pRealData, Info->size ) == E_FAIL )
		return false;

	return true;
}

