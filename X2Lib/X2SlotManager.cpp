#include "StdAfx.h"
#include ".\x2slotmanager.h"

#ifdef NEW_VILLAGE_UI
CX2SlotManager::CX2Slot** CX2SlotManager::m_pSlotBeforeDragging = g_pData->GetSlotBeforeDragging();
#else
CX2SlotManager::CX2Slot* CX2SlotManager::m_pSlotBeforeDragging = NULL;
#endif

CX2SlotManager::CX2SlotManager( CKTDXStage* pNowStage, const WCHAR* pFileName )
: m_pNowOverItemSlot( NULL )
, m_pDLGItemDesc( NULL )
, m_pDLGItemDescCompare( NULL )
#ifdef FIX_TOOLTIP
, m_pDLGItemDescAdd( NULL )
, m_pDLGItemDescCompareAdd( NULL )
#endif FIX_TOOLTIP
{
	m_pNowState = (CX2State*)pNowStage;

	m_Pos = D3DXVECTOR2( 0, 0 );

	m_iDummyValue = -1;
	m_SlotManagerLayer = XDL_NORMAL_1;

	if ( pFileName != NULL )
		OpenScriptFile( pFileName );

#ifdef NEW_VILLAGE_UI
	//2008.11.13 김태완
	m_pSlotBeforeDragging	= g_pData->GetSlotBeforeDragging();
#else
	m_pSlotBeforeDragging	= NULL;
#endif
    	
}

CX2SlotManager::~CX2SlotManager(void)
{

#ifdef FIX_TOOLTIP
	SAFE_DELETE_DIALOG( m_pDLGItemDescCompareAdd );
	SAFE_DELETE_DIALOG( m_pDLGItemDescAdd );
#endif FIX_TOOLTIP
    SAFE_DELETE_DIALOG( m_pDLGItemDesc );
	SAFE_DELETE_DIALOG( m_pDLGItemDescCompare );

	for ( int i = 0; i < (int)m_SlotList.size(); i++ )
	{
		CX2Slot* pSlot = m_SlotList[i];
		SAFE_DELETE( pSlot );
	}
	m_SlotList.clear();

#ifdef NEW_VILLAGE_UI
	*m_pSlotBeforeDragging = NULL;
#else
	m_pSlotBeforeDragging = NULL;
#endif
}

void CX2SlotManager::SetSlotManagerLayer( int slotManagerLayer )
{
	m_SlotManagerLayer = slotManagerLayer;

	for ( int i = 0; i < (int)m_SlotList.size(); i++ )
	{
		CX2Slot* pSlot = m_SlotList[i];
		if ( pSlot == NULL )
			continue;

		//{{ 09.03.18
		pSlot->SetNormalLayer( (X2_DIALOG_LAYER) slotManagerLayer );
		//}}

		if ( pSlot->GetDialog() != NULL )
			g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer( pSlot->GetDialog(), slotManagerLayer );

		if ( pSlot->GetDLGSlotFixBack() != NULL )
			g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer( pSlot->GetDLGSlotFixBack(), slotManagerLayer );

	}
}


// 2008.12.15 김태완
void CX2SlotManager::SetSlotManagerChangeSequence( bool bFront )
{
	for ( int i = 0; i < (int)m_SlotList.size(); i++ )
	{
		CX2Slot* pSlot = m_SlotList[i];
		if ( pSlot == NULL )
			continue;

		if ( pSlot->GetDialog() != NULL )
			g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( pSlot->GetDialog(), bFront );

		if ( pSlot->GetDLGSlotFixBack() != NULL )
			g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( pSlot->GetDLGSlotFixBack(), bFront );

	}
}

bool CX2SlotManager::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( uMsg )
	{

	case WM_LBUTTONDOWN:
		{
			POINT mousePoint = { short(LOWORD(lParam)), short(HIWORD(lParam)) };
			D3DXVECTOR2 mousePos = D3DXVECTOR2( (float)mousePoint.x, (float)mousePoint.y );

			return MouseDown( mousePos );


		}
		break;

	case WM_MOUSEMOVE:
		{
			POINT mousePoint = { short(LOWORD(lParam)), short(HIWORD(lParam)) };
			D3DXVECTOR2 mousePos = D3DXVECTOR2( (float)mousePoint.x, (float)mousePoint.y );
			
			MouseMove( mousePos );
		}
		break;

	case WM_LBUTTONUP:
		{
			POINT mousePoint = { short(LOWORD(lParam)), short(HIWORD(lParam)) };
			D3DXVECTOR2 mousePos = D3DXVECTOR2( (float)mousePoint.x, (float)mousePoint.y );

			MouseUp( mousePos );
		}
		break;

	}

	return false;
}

void CX2SlotManager::AddSlot( CX2Slot* pSlot )
{
	if ( pSlot == NULL )
		return;

	pSlot->SetSlotManager( this );
	m_SlotList.push_back( pSlot );
}

CX2SlotItem* CX2SlotManager::CreateItemSlot()
{ 
	CX2SlotItem* pSlotItem = new CX2SlotItem(); 
	
	return pSlotItem; 
}

void CX2SlotManager::SetEnable( CX2Slot::SLOT_TYPE slotType, bool bEnable )
{
	for ( int i = 0; i < (int)m_SlotList.size(); i++ )
	{
		CX2Slot* pSlot = m_SlotList[i];
		if ( pSlot == NULL )
			continue;

		if ( pSlot->GetSlotType() == slotType )
		{
			pSlot->SetEnable( bEnable );
		}
	}
}

void CX2SlotManager::SetShow( CX2Slot::SLOT_TYPE slotType, bool bShow )
{
	for ( int i = 0; i < (int)m_SlotList.size(); i++ )
	{
		CX2Slot* pSlot = m_SlotList[i];
		if ( pSlot == NULL )
			continue;

		if ( pSlot->GetSlotType() == slotType )
		{
			pSlot->SetShow( bShow );
		}
	}
}

void CX2SlotManager::SetEnable( bool bEnable )
{
	for ( int i = 0; i < (int)m_SlotList.size(); i++ )
	{
		CX2Slot* pSlot = m_SlotList[i];
		if ( pSlot == NULL )
			continue;

		pSlot->SetEnable( bEnable );
	}
}

void CX2SlotManager::SetShow( bool bShow )
{
	for ( int i = 0; i < (int)m_SlotList.size(); i++ )
	{
		CX2Slot* pSlot = m_SlotList[i];
		if ( pSlot == NULL )
			continue;

		pSlot->SetShow( bShow );
	}
}

void CX2SlotManager::RegisterLuaBind()
{
	lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(),  "g_pCX2SlotManager",		this );
}




bool CX2SlotManager::MouseDown( D3DXVECTOR2 mousePos )
#ifdef NEW_VILLAGE_UI
{
	*m_pSlotBeforeDragging = GetSlotInMousePos( mousePos );

	if( NULL == *m_pSlotBeforeDragging )
	{
		return false;
	}

	if( false == (*m_pSlotBeforeDragging)->GetClickable() )
	{
		*m_pSlotBeforeDragging = NULL;
		return false;
	}

	if ( *m_pSlotBeforeDragging != NULL && (*m_pSlotBeforeDragging)->GetDialog() != NULL )
		(*m_pSlotBeforeDragging)->GetDialog()->SetColor( D3DXCOLOR(1,1,1,0.7f) );

	return true;
}
#else
{
	m_pSlotBeforeDragging = GetSlotInMousePos( mousePos );

	if( NULL == m_pSlotBeforeDragging )
	{
		return false;
	}

	if( false == m_pSlotBeforeDragging->GetClickable() )
	{
		m_pSlotBeforeDragging = NULL;
		return false;
	}

	if ( m_pSlotBeforeDragging != NULL && m_pSlotBeforeDragging->GetDialog() != NULL )
		m_pSlotBeforeDragging->GetDialog()->SetColor( D3DXCOLOR(1,1,1,0.7f) );

	return true;
}
#endif



bool CX2SlotManager::MouseUp( D3DXVECTOR2 mousePos )
#ifdef NEW_VILLAGE_UI
{
	bool bRetVal = false;
	CX2Slot* pSlot = NULL;

	pSlot = GetSlotInMousePos( mousePos );

	if ( pSlot == NULL )
	{
		goto e_Exit;
	}

	if ( *m_pSlotBeforeDragging != NULL )
	{
		if ( pSlot == *m_pSlotBeforeDragging )
		{
			*m_pSlotBeforeDragging = NULL;
			goto e_Exit;
		}

		if ( (*m_pSlotBeforeDragging)->GetDialog() != NULL )
			(*m_pSlotBeforeDragging)->GetDialog()->SetColor( D3DXCOLOR( 1,1,1,1 ) );

		CKTDGUIDialogType pTempStaticControl = (*m_pSlotBeforeDragging)->GetDialog();
		(*m_pSlotBeforeDragging)->SetDialog( pSlot->GetDialog() );
		pSlot->SetDialog( pTempStaticControl );
		bRetVal = true;
		*m_pSlotBeforeDragging = NULL;
	}


e_Exit:

	
	//*m_pSlotBeforeDragging = NULL;

	if ( bRetVal == false )
	{
		if ( *m_pSlotBeforeDragging != NULL && (*m_pSlotBeforeDragging)->GetDialog() != NULL )
			(*m_pSlotBeforeDragging)->GetDialog()->SetColor( D3DXCOLOR( 1,1,1,1 ) );

		return false;
	}
	

	return true;
}
#else
{
	bool bRetVal = false;
	CX2Slot* pSlot = NULL;

	pSlot = GetSlotInMousePos( mousePos );

	if ( pSlot == NULL )
	{
		goto e_Exit;
	}

	if ( m_pSlotBeforeDragging != NULL )
	{
		if ( pSlot == m_pSlotBeforeDragging )
		{
			goto e_Exit;
		}

		if ( m_pSlotBeforeDragging->GetDialog() != NULL )
			m_pSlotBeforeDragging->GetDialog()->SetColor( D3DXCOLOR( 1,1,1,1 ) );

		CKTDGUIDialogType pTempStaticControl = m_pSlotBeforeDragging->GetDialog();
		m_pSlotBeforeDragging->SetDialog( pSlot->GetDialog() );
		pSlot->SetDialog( pTempStaticControl );
		bRetVal = true;
	}


e_Exit:


	m_pSlotBeforeDragging = NULL;

	if ( bRetVal == false )
	{
		if ( m_pSlotBeforeDragging != NULL && m_pSlotBeforeDragging->GetDialog() != NULL )
			m_pSlotBeforeDragging->GetDialog()->SetColor( D3DXCOLOR( 1,1,1,1 ) );

		return false;
	}


	return true;
}
#endif


void CX2SlotManager::MouseMove( D3DXVECTOR2 mousePos )
#ifdef NEW_VILLAGE_UI
{
	if ( *m_pSlotBeforeDragging != NULL )
	{
		CKTDGUIDialogType pDialog = (*m_pSlotBeforeDragging)->GetDialog();
		if ( pDialog != NULL )
		{
			D3DXVECTOR2 pos = mousePos;
			pos = g_pKTDXApp->MouseConvertByResolution( pos );
			pos.x -= (*m_pSlotBeforeDragging)->GetSize().x/2.0f;
			pos.y -= (*m_pSlotBeforeDragging)->GetSize().y/2.0f;
			pDialog->SetPos( pos );
		}
	}
}

#else
{
	if ( m_pSlotBeforeDragging != NULL )
	{
		CKTDGUIDialogType pDialog = m_pSlotBeforeDragging->GetDialog();
		if ( pDialog != NULL )
		{
			D3DXVECTOR2 pos = mousePos;
			pos = g_pKTDXApp->MouseConvertByResolution( pos );
			pos.x -= m_pSlotBeforeDragging->GetSize().x/2.0f;
			pos.y -= m_pSlotBeforeDragging->GetSize().y/2.0f;
			pDialog->SetPos( pos );
		}
	}
}
#endif

CX2SlotManager::CX2Slot* CX2SlotManager::GetSlotInMousePos( D3DXVECTOR2 mousePos )
{
	CX2SlotManager::CX2Slot* pSlot = NULL;

	for ( int i = 0; i < (int)m_SlotList.size(); i++ )
	{
		CX2SlotManager::CX2Slot* pSlotToCheck = m_SlotList[i];
		if ( pSlotToCheck == NULL || pSlotToCheck->GetEnable() == false )
			continue;

		if ( pSlotToCheck->IsInMousePoint( mousePos ) == true )
		{
			pSlot = pSlotToCheck;
			break;
		}
	}

	return pSlot;
}

bool CX2SlotManager::OpenScriptFile( const WCHAR* pFilename )
{
	KTDXPROFILE();

	RegisterLuaBind();

    if ( g_pKTDXApp->LoadLuaTinker( pFilename ) == false )
    {
		return false;
    }

	return true;
}

CX2SlotManager::CX2Slot* CX2SlotManager::GetSlot( int index )
{
	if ( index < 0 || m_SlotList.size() <= (unsigned int ) index )
		return NULL;

	return m_SlotList[index];
}

CX2SlotManager::CX2Slot* CX2SlotManager::GetSlot( CX2Slot::SLOT_TYPE slotType, int slotID )
{
	for ( unsigned int iS = 0; iS < m_SlotList.size(); ++iS )
	{
		CX2Slot* pSlot= m_SlotList[iS];
		if ( pSlot->GetSlotType() == slotType && pSlot->GetSlotID() == slotID )
		{
			return pSlot;
		}
	}

	return NULL;
}


void CX2SlotManager::SetNowDragItemReturn()
#ifdef NEW_VILLAGE_UI
{
	if ( (*m_pSlotBeforeDragging) != NULL )
	{
		(*m_pSlotBeforeDragging)->ResetPos();

		if ( (*m_pSlotBeforeDragging)->GetDialog() != NULL )
			(*m_pSlotBeforeDragging)->GetDialog()->SetColor( D3DXCOLOR(1,1,1,1) );
	}

	*m_pSlotBeforeDragging = NULL;	
}
#else
{
	if ( m_pSlotBeforeDragging != NULL )
	{
		m_pSlotBeforeDragging->ResetPos();

		if ( m_pSlotBeforeDragging->GetDialog() != NULL )
			m_pSlotBeforeDragging->GetDialog()->SetColor( D3DXCOLOR(1,1,1,1) );
	}

	m_pSlotBeforeDragging = NULL;	
}
#endif

//{{ mauntain : 김태환 [2012.07.16] 인벤토리 및 슬롯을 고려한 아이템 구입 갯수 연산
#ifdef SERV_AUTOMATICALLY_REGISTER_FOR_CONSUMABLE_ITEM
int CX2SlotManager::GetUsedSlotMaxNum()		/// 현재 사용 가능한 퀵슬롯 갯수
{
	int slotCount = 0;

	for ( int i = 0; i < GetSlotMaxNum(); i++ )
	{	
		CX2SlotManager::CX2Slot* pSlot = GetSlot( i );

		if( NULL != pSlot && true == pSlot->GetEnable() )
			slotCount++;		/// 활성화 된 슬롯 Count
	}

	return slotCount;
}

int CX2SlotManager::GetUsedSlotNum()		/// 현재 사용중인 퀵슬롯 갯수
{
	int slotCount = 0;

	for ( int i = 0; i < GetSlotMaxNum(); i++ )
	{	
		CX2SlotManager::CX2Slot* pSlot = GetSlot( i );
		if ( NULL != pSlot && true == pSlot->GetEnable() && NULL != pSlot->GetDialog() )
		{
			slotCount++;		/// 활성화 되었고, 아이템이 들어있는 슬롯 Count
		}
	}

	return slotCount;
}
#endif SERV_AUTOMATICALLY_REGISTER_FOR_CONSUMABLE_ITEM
//}}

CX2SlotManager::CX2Slot::CX2Slot()
{
	m_SlotID			= -1;

	m_pSlotManager		= NULL;

	m_bEnable			= true;
	m_bShow				= true;
	m_bClickable		= true;

	m_SlotType			= ST_NONE;
	m_Pos				= D3DXVECTOR2( 0, 0 );
	m_Size				= D3DXVECTOR2( 0, 0 );

	m_pDLGSlotFixBack	= NULL;
	m_pDLGSlot			= NULL;
	m_NormalLayer		= XDL_NORMAL_1;
}

CX2SlotManager::CX2Slot::~CX2Slot(void)
{
	SAFE_DELETE_DIALOG( m_pDLGSlot );
	SAFE_DELETE_DIALOG( m_pDLGSlotFixBack );
}

void CX2SlotManager::CX2Slot::SetSlotManager( CX2SlotManager* pSlotManager )
{
	m_pSlotManager = pSlotManager;
}

void CX2SlotManager::CX2Slot::SetEnable( bool bEnable, bool bEffectItemImage )
{
	m_bEnable = bEnable;

	if ( bEffectItemImage == true && m_pDLGSlot != NULL )
	{
		m_pDLGSlot->SetEnable( bEnable );
	}

	if ( m_pDLGSlotFixBack != NULL )
		m_pDLGSlotFixBack->SetEnable( bEnable );
}

void CX2SlotManager::CX2Slot::SetShow( bool bShow, bool bEffectItemImage )
{
	m_bShow = bShow;

	if ( bEffectItemImage == true && m_pDLGSlot != NULL )
	{
		m_pDLGSlot->SetShow( bShow );
	}

	if ( m_pDLGSlotFixBack != NULL )
		m_pDLGSlotFixBack->SetShow( bShow );
}

bool CX2SlotManager::CX2Slot::GetClickable() const
{
	return m_bClickable;
}

void CX2SlotManager::CX2Slot::SetClickable( bool val )
{
	m_bClickable = val;
}

D3DXVECTOR2 CX2SlotManager::CX2Slot::GetPos()
{
	return m_Pos;
}

void CX2SlotManager::CX2Slot::SetPos( D3DXVECTOR2 pos )
{
	m_Pos = pos;
	ResetPos();
	if ( m_pDLGSlotFixBack != NULL )
		m_pDLGSlotFixBack->SetPos( m_Pos + m_pSlotManager->GetPos() );
}

void CX2SlotManager::CX2Slot::SetSlotData_LUA()
{
	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
#ifndef X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
    TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING

	LuaGetValue( luaManager );

#ifdef  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
    LUA_GET_USER_DEFINED_TYPE_VALUE( luaManager, "POS", m_Pos, D3DXVECTOR2(0,0) );
#else   X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
	m_Pos	= lua_tinker::get<D3DXVECTOR2>( luaManager.GetLuaState(),  "POS" );
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING

	if ( m_pDLGSlotFixBack != NULL )
		m_pDLGSlotFixBack->SetPos( m_Pos + m_pSlotManager->GetPos() );

#ifdef  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
    LUA_GET_USER_DEFINED_TYPE_VALUE( luaManager, "SIZE", m_Size, D3DXVECTOR2(0,0) );
#else   X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
	m_Size	= lua_tinker::get<D3DXVECTOR2>( luaManager.GetLuaState(),  "SIZE" );
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING


	
}

bool CX2SlotManager::CX2Slot::IsInMousePoint( D3DXVECTOR2 mousePos )
{
	if ( m_bEnable == false )
		return false;

	D3DXVECTOR2 m_ConvertPos = g_pKTDXApp->ConvertByResolution( m_Pos + m_pSlotManager->GetPos() );
	D3DXVECTOR2 m_ConvertSize = g_pKTDXApp->ConvertByResolution( m_Size );

	if ( mousePos.x >= m_ConvertPos.x && mousePos.x <= m_ConvertPos.x + m_ConvertSize.x &&
		mousePos.y >= m_ConvertPos.y && mousePos.y <= m_ConvertPos.y + m_ConvertSize.y )
	{
		return true;
	}

	return false;
}

void CX2SlotManager::CX2Slot::SetDialog( CKTDGUIDialogType pDLGSlot )
{
	m_pDLGSlot = pDLGSlot;

	if ( m_pDLGSlot != NULL )
	{
		if ( m_bShow == false )
			m_pDLGSlot->SetShow( false );

		if ( m_bEnable == false )
			m_pDLGSlot->SetEnable( false );
	}

	ResetPos();
}

void CX2SlotManager::CX2Slot::ResetPos()
{
	if ( m_pDLGSlot != NULL )
	{
		m_pDLGSlot->SetPos( m_Pos + m_pSlotManager->GetPos() );
	}
}

void CX2SlotManager::CX2Slot::LuaGetValue( KLuaManager& luaManager )
{
	LUA_GET_VALUE( luaManager, "SLOT_ID", m_SlotID, 0 );

	LUA_GET_VALUE( luaManager, "ENABLE", m_bEnable, true );
	LUA_GET_VALUE( luaManager, "SHOW",	m_bShow, true );

	LUA_GET_VALUE_ENUM( luaManager, "SLOT_TYPE", m_SlotType, CX2Slot::SLOT_TYPE, ST_NONE );
	wstring dlgFileName;
	LUA_GET_VALUE( luaManager, "SLOT_BACK_DLG_FILE", dlgFileName, L"" );

	if ( dlgFileName.empty() == false )
	{
		SAFE_DELETE_DIALOG( m_pDLGSlotFixBack );
		m_pDLGSlotFixBack = new CKTDGUIDialog( g_pMain->GetNowState(), dlgFileName.c_str(), 0.0f, m_pSlotManager->GetSlotManagerLayer() );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGSlotFixBack );
	}
}	

//void CX2SlotManager::CX2Slot::LuaTinkerCall()
//{
//	m_Pos	= lua_tinker::get<D3DXVECTOR2>( g_pKTDXApp->GetLuaBinder()->GetLuaState(),  "POS" );
//
//	if ( m_pDLGSlotFixBack != NULL )
//		m_pDLGSlotFixBack->SetPos( m_Pos + m_pSlotManager->GetPos() );
//
//	m_Size	= lua_tinker::get<D3DXVECTOR2>( g_pKTDXApp->GetLuaBinder()->GetLuaState(),  "SIZE" );
//}

#ifdef NEW_VILLAGE_UI
bool CX2SlotManager::CX2Slot::IsItem()
{
	switch(m_SlotType)
	{
	case ST_EQUIPPED:		// 장비슬롯
	case ST_INVENTORY:		// 인벤토리슬롯
	case ST_SHOP_BUY:		// 상점_구매
	case ST_SHOP_PREVIEW:	// 상점_프리뷰
	case ST_RECIPE:			// 레시피
	case ST_SUPPLEMENT:		//보조제
	case ST_MATERIAL:		// 재료
	case ST_CASH_SHOP_MAIN_NEW:		//캐쉬샵메인
	case ST_CASH_SHOP_MAIN_HOT:
	case ST_CASH_SHOP_MAIN_RECOMMEND:
	case ST_CASH_SHOP_SUB:
	case ST_CASH_SHOP_SUB_SPECIAL:
	case ST_CASH_SHOP_DEPOSIT:
	case ST_CASH_SHOP_CHOICED:
	case ST_POST_SEND:		// 보낸 편지 슬롯
	case ST_POST_RECEIVE:	// 받은 편지 슬롯
	case ST_BANK:			// 아래 REPOSITORY를 BANK로 변경하여 사용
#ifdef SERV_PET_SYSTEM
	case ST_PET:
#endif SERV_PET_SYSTEM
		return true;
	case ST_NONE:
	case ST_END:
	//case ST_REPOSITORY:		// 창고(안씀)
	default:
		return false;
	};
}
#endif

#ifdef SERV_UPGRADE_TRADE_SYSTEM // 김태환

/** @function	: GetSlotByItemUID
	@brief		: ItemUID를 통해 SlotItem 객체를 반환하는 함수
	@param		: ItemUID
	@return		: SlotItem 객체
*/
CX2SlotItem* CX2SlotManager::GetSlotByItemUID( IN UidType uidItemUID_ )
{
	for ( UINT i = 0; i < m_SlotList.size(); i++ )
	{
		CX2SlotItem* pItemSlot = static_cast<CX2SlotItem*>( GetSlot( i ) );

		if ( pItemSlot == NULL )
			continue;

		if ( pItemSlot->GetItemUID() == uidItemUID_ )
		{
			return pItemSlot;
		}
	}

	return NULL;
}

#endif //SERV_UPGRADE_TRADE_SYSTEM