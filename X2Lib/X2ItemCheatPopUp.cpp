#include "StdAfx.h"
#include ".\x2itemcheatpopup.h"


#ifdef ITEM_CHEAT_POPUP_TEST


//////////////////////////////////////////////////////////////////////////
CX2ItemCheatPopUp::CX2ItemCheatPopUp() 
: m_SearchKeyword( L"" )
, m_iFashionOnly( -1 )
, m_eUnitType( CX2Unit::UT_NONE )
, m_eUnitClass( CX2Unit::UC_NONE )
, m_eEquipPosition( CX2Unit::EP_NONE )
, m_iMinLevel( 0 )
, m_iMaxLevel( 0 )
, m_pDLGItemList( NULL )
, m_eSortType( CX2ItemCheatPopUp::ST_NONE )
, m_iItemCount( 1 )
{
}

void CX2ItemCheatPopUp::OpenItemList( bool bOpen )
{
	if( false == bOpen )
	{
		if( NULL != m_pDLGItemList )
		{
			m_pDLGItemList->SetShowEnable( bOpen, bOpen );

			//g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGItemList, NULL, false );
			//m_pDLGItemList = NULL;
		}
	}
	else
	{
		if( NULL == m_pDLGItemList )
		{
			m_pDLGItemList = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_ITEM_LIST_DEV_ONLY.lua" );
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGItemList );

			std::map< CX2Unit::UNIT_TYPE, const CX2Unit::UnitTemplet* > mapUnitType;

			const CX2UnitManager::UnitTempletMap& mapUnitTemplet = g_pData->GetUnitManager()->GetUnitTempletMap();
			{
				for( CX2UnitManager::UnitTempletMap::const_iterator it = mapUnitTemplet.begin(); it != mapUnitTemplet.end(); it++ )
				{
					const CX2Unit::UnitTemplet* pUnitTemplet = it->second;
					mapUnitType[ pUnitTemplet->m_UnitType ] = pUnitTemplet;
				}
			}

			CKTDGUIComboBox* pComboUnitType = (CKTDGUIComboBox*) m_pDLGItemList->GetControl( L"COMBO_UNIT_TYPE" );
			if( NULL != pComboUnitType )
			{
				pComboUnitType->AddItem( L"NONE", (void*) NULL );

				std::map< CX2Unit::UNIT_TYPE, const CX2Unit::UnitTemplet* >::const_iterator it;
				for( it = mapUnitType.begin(); it != mapUnitType.end(); it++ )
				{
					const CX2Unit::UnitTemplet* pUnitTemplet = it->second;
					pComboUnitType->AddItem( pUnitTemplet->m_Name.c_str(), (void*) pUnitTemplet );
				}
			}


			CKTDGUIComboBox* pComboUnitClass = (CKTDGUIComboBox*) m_pDLGItemList->GetControl( L"COMBO_UNIT_CLASS" );
			if( NULL != pComboUnitClass )
			{
				pComboUnitClass->AddItem( L"NONE", (void*) NULL );

				for( CX2UnitManager::UnitTempletMap::const_iterator it = mapUnitTemplet.begin(); it != mapUnitTemplet.end(); it++ )
				{
					const CX2Unit::UnitTemplet* pUnitTemplet = it->second;
					pComboUnitClass->AddItem( pUnitTemplet->m_Description.c_str(), (void*) pUnitTemplet );
				}
			}

			UpdateItemList();

		}

		m_pDLGItemList->SetShowEnable( bOpen, bOpen );
	}
}

void CX2ItemCheatPopUp::UpdateItemList()
{
	if( NULL == m_pDLGItemList )
		return;

	//if( false == m_bUpdateItemList )
	//	return; 

	//m_bUpdateItemList = false;


	// 아이템 리스트 갱신 시작

	std::vector< const CX2Item::ItemTemplet*> vecItemList;
	vecItemList.reserve( 512 );
#ifdef  X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
    unsigned int uiNumItems = g_pData->GetItemManager()->GetItemNum();
    for( unsigned i = 0; i != uiNumItems; ++i )
#else   //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
	const map<int, const CX2Item::ItemTemplet*>& mapItemTemplet = g_pData->GetItemManager()->GetItemTempletIDMap();
	map<int, const CX2Item::ItemTemplet*>::const_iterator it;
	for( it = mapItemTemplet.begin(); it != mapItemTemplet.end() ; it++ )
#endif  //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
	{		
#ifdef  X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
        const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTempletFromIndex( i );
#else   //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
		const CX2Item::ItemTemplet* pItemTemplet = (*it).second;
#endif  //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING

		if( -1 != m_iFashionOnly )
		{
			if( 0 == m_iFashionOnly )
			{
				if( true == pItemTemplet->GetFashion() )
					continue;
			}
			else
			{
				if( false == pItemTemplet->GetFashion() )
					continue;
			}
		}

		if( CX2Unit::UT_NONE != m_eUnitType )
		{
			if( CX2Unit::UT_NONE != pItemTemplet->GetUnitType() &&
				m_eUnitType != pItemTemplet->GetUnitType() )
			{
				continue;
			}
		}

		if( CX2Unit::UC_NONE != m_eUnitClass )
		{
			if( CX2Unit::UC_NONE != pItemTemplet->GetUnitClass() &&
				m_eUnitClass != pItemTemplet->GetUnitClass() )
			{
				continue;
			}
		}

		if( CX2Unit::EP_NONE != m_eEquipPosition )
		{
			if( m_eEquipPosition != pItemTemplet->GetEqipPosition() )
			{
				continue;
			}
		}

		if( 0 != m_iMinLevel )
		{
			if( pItemTemplet->GetUseLevel() < m_iMinLevel )
				continue;
		}

		if( 0 != m_iMaxLevel )
		{
			if( pItemTemplet->GetUseLevel() > m_iMaxLevel )
				continue;
		}


		if( false == m_SearchKeyword.empty() )
		{
			if( NULL == wcsstr( pItemTemplet->GetName(), m_SearchKeyword.c_str() ) &&
				NULL == wcsstr( pItemTemplet->GetDescription(), m_SearchKeyword.c_str() ) )
			{
				continue;
			}
		}

		vecItemList.push_back( pItemTemplet );
	}



	switch( m_eSortType )
	{
	case ST_ID:
		{
			std::sort( vecItemList.begin(), vecItemList.end(), CX2Item::ItemTemplet::LessID() );
		} break;

	case ST_NAME:
		{
			std::sort( vecItemList.begin(), vecItemList.end(), CX2Item::ItemTemplet::LessName() );
		} break;
	}



	CKTDGUIListBox* pListBox = (CKTDGUIListBox*) m_pDLGItemList->GetControl( L"ListBox_Item" );
	if( NULL == pListBox )
		return;


	pListBox->RemoveAllItems();
	WCHAR wszListBoxItemName[1024] = L"";
	BOOST_FOREACH( const CX2Item::ItemTemplet* pItemTemplet, vecItemList )
	{
		StringCchPrintfW( wszListBoxItemName, ARRAY_SIZE(wszListBoxItemName), L"%-8d %-20s %-25s %-20s", 
			pItemTemplet->GetItemID(),
			pItemTemplet->GetName(),
			pItemTemplet->GetModelName(0),
			pItemTemplet->GetTextureChangeXETName() );


		//switch( m_eSortType )
		//{
		//default:
		//case ST_ID:
		//	{
		//		StringCchPrintfW( wszListBoxItemName, ARRAY_SIZE(wszListBoxItemName), L"%10d %30s", pItemTemplet->m_ItemID, pItemTemplet->m_Name.c_str() );
		//	} break;

		//case ST_NAME:
		//	{
		//		StringCchPrintfW( wszListBoxItemName, ARRAY_SIZE(wszListBoxItemName), L"%30s %10d", pItemTemplet->m_Name.c_str(), pItemTemplet->m_ItemID );
		//	} break;
		//}
		pListBox->AddItem( wszListBoxItemName, (void*) pItemTemplet );
	}

}

bool CX2ItemCheatPopUp::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( wParam )
	{
	case ICPCM_COMBO_SORT_TYPE:		
		{
			CKTDGUIComboBox* pComboBox = (CKTDGUIComboBox*) lParam;
			if( (int) m_eSortType != pComboBox->GetSelectedItemIndex() )
			{
				m_eSortType = (CX2ItemCheatPopUp::SORT_TYPE) pComboBox->GetSelectedItemIndex();
				UpdateItemList();
			}

			return true;
		} break;

	case ICPCM_COMBO_FASHION_ONLY:
		{
			CKTDGUIComboBox* pComboBox = (CKTDGUIComboBox*) lParam;
			switch( pComboBox->GetSelectedItemIndex() )
			{
			case 0:	// none
				{
					if( -1 != m_iFashionOnly )
					{	
						m_iFashionOnly = -1;
						UpdateItemList();
					}
				} break;

			case 1: // fashion only
				{
					if( 1 != m_iFashionOnly )
					{	
						m_iFashionOnly = 1;
						UpdateItemList();
					}
				} break;

			case 2: // not fashion only
				{

					if( 0 != m_iFashionOnly )
					{	
						m_iFashionOnly = 0;
						UpdateItemList();
					}
				} break;
			}

			return true;

		} break;

	case ICPCM_COMBO_UNIT_TYPE:	
		{
			CKTDGUIComboBox* pComboBox = (CKTDGUIComboBox*) lParam;
			CKTDGUIComboBox::ComboBoxItem* pComboBoxItem = pComboBox->GetSelectedItem();
			if( NULL != pComboBoxItem )
			{
				CX2Unit::UnitTemplet* pUnitTemplet = (CX2Unit::UnitTemplet*) pComboBoxItem->pData;
				if( NULL != pUnitTemplet )
				{
					if( m_eUnitType != pUnitTemplet->m_UnitType )
					{
						m_eUnitType = pUnitTemplet->m_UnitType;
						UpdateItemList();
					}
				}
				else
				{
					if( m_eUnitType != CX2Unit::UT_NONE )
					{
						m_eUnitType = CX2Unit::UT_NONE;
						UpdateItemList();
					}
				}
			}

			return true;
		} break;

	case ICPCM_COMBO_UNIT_CLASS:	
		{
			CKTDGUIComboBox* pComboBox = (CKTDGUIComboBox*) lParam;
			CKTDGUIComboBox::ComboBoxItem* pComboBoxItem = pComboBox->GetSelectedItem();
			if( NULL != pComboBoxItem )
			{
				CX2Unit::UnitTemplet* pUnitTemplet = (CX2Unit::UnitTemplet*) pComboBoxItem->pData;
				if( NULL != pUnitTemplet )
				{
					if( m_eUnitClass != pUnitTemplet->m_UnitClass )
					{
						m_eUnitClass = pUnitTemplet->m_UnitClass;
						UpdateItemList();
					}
				}
				else
				{
					if( m_eUnitClass != CX2Unit::UC_NONE )
					{
						m_eUnitClass = CX2Unit::UC_NONE;
						UpdateItemList();
					}
				}
			}

			return true;
		} break;

	case ICPCM_COMBO_EQUIP_POSITION:
		{
			CKTDGUIComboBox* pComboBox = (CKTDGUIComboBox*) lParam;
			if( (int) m_eEquipPosition != pComboBox->GetSelectedItemIndex() )
			{
				m_eEquipPosition = (CX2Unit::EQIP_POSITION) pComboBox->GetSelectedItemIndex();
				UpdateItemList();
			}

			return true;
		} break;

	case ICPCM_EDIT_MIN_LEVEL:
		{
			CKTDGUIIMEEditBox* pEditBox = (CKTDGUIIMEEditBox*) lParam;
			int iLevel = 0;
			if( pEditBox->GetTextLength() > 0 )
			{
				iLevel = _wtoi( pEditBox->GetText() );
			}

			if( iLevel != m_iMinLevel )
			{
				m_iMinLevel = iLevel;
				UpdateItemList();
			}
			pEditBox->GetDialog()->ClearFocus();

			return true;
		} break;

	case ICPCM_EDIT_MAX_LEVEL:
		{
			CKTDGUIIMEEditBox* pEditBox = (CKTDGUIIMEEditBox*) lParam;
			int iLevel = 0;
			if( pEditBox->GetTextLength() > 0 )
			{
				iLevel = _wtoi( pEditBox->GetText() );
			}

			if( iLevel != m_iMaxLevel )
			{
				m_iMaxLevel = iLevel;
				UpdateItemList();
			}
			pEditBox->GetDialog()->ClearFocus();

			return true;
		} break;



	case ICPCM_EDIT_ITEM_COUNT:
		{
			CKTDGUIIMEEditBox* pEditBox = (CKTDGUIIMEEditBox*) lParam;

			if( pEditBox->GetTextLength() > 0 )
			{
				m_iItemCount = _wtoi( pEditBox->GetText() );
			}
			else
			{
				m_iItemCount = 1;
			}

			pEditBox->GetDialog()->ClearFocus();

			return true;
		} break;

	case ICPCM_EDIT_SEARCH_KEYWORD:
		{
			CKTDGUIIMEEditBox* pEditBox = (CKTDGUIIMEEditBox*) lParam;
			std::wstring searchKeyword = pEditBox->GetText();
			if( 0 != m_SearchKeyword.compare( searchKeyword ) )
			{
				m_SearchKeyword = searchKeyword;
				UpdateItemList();
			}
			pEditBox->GetDialog()->ClearFocus();

			return true;
		} break;


	case ICPCM_BUY_ITEM:
		{
			CKTDGUIListBox* pListBox = (CKTDGUIListBox*) lParam;
			CKTDGUIListBox::ListBoxItem* pListBoxItem = pListBox->GetSelectedItem();
			if( NULL != pListBoxItem )
			{
                const CX2Item::ItemTemplet*  kItemTemplet = (const CX2Item::ItemTemplet*) pListBoxItem->pData;
                if ( kItemTemplet != NULL )
                {
					g_pMain->BuyItemSuper_LUA( kItemTemplet->GetItemID(), m_iItemCount );
                }
			}

			return true;
		} break;


	case ICPCM_CLOSE_WINDOW:
		{
			OpenItemList( false );

			//if( NULL != m_pDLGItemList )
			//{
			//	m_pDLGItemList->SetShowEnable( false, false );
			//}
			return true;

		} break;

	}

	return false;
}


#endif ITEM_CHEAT_POPUP_TEST