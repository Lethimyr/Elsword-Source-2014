
#include "stdafx.h"
//#include ".\x2petmanager.h"

#ifdef SERV_PET_SYSTEM

#ifdef PRINT_INGAMEINFO_TO_EXCEL
#include "BasicExcel.hpp"
using namespace YExcel;
#endif PRINT_INGAMEINFO_TO_EXCEL

const float CX2PetManager::MAX_OF_SATIETY = 4800.0f;
const float CX2PetManager::SATIETY_RATIO_TO_BE_ABLE_TO_APPLY_AURA = 0.4f;


#ifdef	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD
CX2PetManager::PetTemplet::~PetTemplet()
{
	BOOST_FOREACH( PetInitLuaTemplet& kInitLuaTemplet, m_Evolution_Step_InitLuaTemplet )
	{
		SAFE_DELETE( kInitLuaTemplet.m_pInitTemplet );
		SAFE_DELETE( kInitLuaTemplet.m_pLuaManager );
	}
	m_Evolution_Step_InitLuaTemplet.clear();
}
#endif	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD


CX2PetManager::CX2PetManager()
	: m_fSummonedPetMp( 0.0f )
{
	m_bShow = true;	
	m_mapPet.clear();
	m_vecRemovePet.clear();

	m_pTalkBoxMgr = CX2TalkBoxManager::CreateTalkBoxManger();	

	m_bCreatePet = false;

	m_iMakePetId	= 0;
	m_pDlgMakePetName = NULL;
	m_pDlgPetPopupMenu = NULL;
#ifdef CHANGE_PET_NAME
	m_iChangeItemUID		= 0;
	m_pDlgChangePetName = NULL;
#endif CHANGE_PET_NAME

	m_iEnabledPetAi = 0;
}

CX2PetManager::~CX2PetManager(void)
{
	AllRemovePet();

	map<PET_UNIT_ID, PetTemplet*>::iterator iter;
	for( iter = m_PetTemplet.begin(); iter != m_PetTemplet.end(); ++iter )
	{
		PetTemplet *pPetTemplet = iter->second;
		SAFE_DELETE( pPetTemplet );
	}
	m_PetTemplet.clear();

	SAFE_DELETE_KTDGOBJECT( m_pTalkBoxMgr );	

	SAFE_DELETE_DIALOG_HANDLE( m_pDlgMakePetName );	
	SAFE_DELETE_DIALOG_HANDLE( m_pDlgPetPopupMenu );
#ifdef CHANGE_PET_NAME
	SAFE_DELETE_DIALOG_HANDLE( m_pDlgChangePetName ); 
#endif CHANGE_PET_NAME
}

HRESULT	CX2PetManager::OnFrameMove( double fTime, float fElapsedTime )
{
	bool bEnableNextAi = false;
	bool bEnabledAi = false;

	map<UidType, CX2PETPtr>::iterator iter;
	int iIndex = 0;

	if( m_pTalkBoxMgr != NULL )
	m_pTalkBoxMgr->OnFrameMove( fTime, fElapsedTime );

	if( m_mapPet.size() > 0 )
	{
		if( (UINT)m_iEnabledPetAi >= m_mapPet.size() )
		{
			m_iEnabledPetAi = 0;
		}

		for( iter = m_mapPet.begin(); iter != m_mapPet.end();  )
		{
			CX2PETPtr pPet = iter->second;

			if( pPet == NULL )
			{
				iter = m_mapPet.erase( iter );
				continue;
			}
#if 1
			if( g_pX2Game != NULL )
			{
				CX2GUUser* pMasterGUUser = g_pX2Game->GetUserUnitByUID( pPet->GetMasterUid() );

				if( NULL == pMasterGUUser )
				{				
					iter = RemovePet( pPet->GetMasterUid() );
					//iter = m_mapPet.erase( iter );
					continue;
				}
				else if ( pPet->GetAuraSuspendedTargetTime() > 0.0f &&
					pPet->CheckAndResetAuraSuspendedSumOfElapsedTime() )	/// 오오라 지연 적용 시간이 있다면
				{
					UpdateMasterUserSocketAndEnchantData( pMasterGUUser );
					pPet->SetAuraSuspendedTargetTime( 0.0f );	// 적용을 시도 하였으므로 지연 적용을 사용하지 않음 (시도 라는 것은 포만감에 의해 적용되지 않을 수도 있기 때문)

					// 내 유닛이고, 오오라가 적용 될 수 있는 포만도 였다면 채팅창에 출력
					if ( pMasterGUUser->IsMyUnit() && pPet->GetPetInfo().m_Satiety / CX2PetManager::MAX_OF_SATIETY >= CX2PetManager::SATIETY_RATIO_TO_BE_ABLE_TO_APPLY_AURA )
					{
						g_pChatBox->AddChatLog( L"오오라 적용!", KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(0.92549f, 0.92549f, 0.53333f, 1.f), L"#CECEC88" );

						if( g_pData != NULL && 
							g_pData->GetUIManager() != NULL && 
							g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_CHARINFO))
							g_pData->GetUIManager()->GetUICharInfo()->ResetStat();
					}
				}

				if( NULL != g_pData && NULL != g_pData->GetUIManager() )
				{
					pPet->SetShowGageBar( g_pData->GetUIManager()->GetShowUI() );					
				}

			}
			else if( g_pTFieldGame != NULL )
			{
				if( g_pTFieldGame->GetSquareUnitByUID( pPet->GetMasterUid() ) == NULL )
				{
					iter = RemovePet( pPet->GetMasterUid() );
					//iter = m_mapPet.erase( iter );
					continue;
				}
			}
			//else if( g_pX2Room != NULL )
			//{
			//	if( g_pX2Room->GetSlotDataByUnitUID( pPet->GetMasterUid() ) == NULL )
			//	{
			//		//pPet.reset();
			//		iter = m_mapPet.erase( iter );
			//		//RemovePet( pPet->GetMasterUid() );
			//		continue;
			//	}
			//}		
#endif

			if( g_pTFieldGame != NULL && pPet != NULL )
			{
				// 펫이 안켜지는 경우가 있어 강제로 켜준다.
				if( g_pTFieldGame->GetSquareUnitByUID( pPet->GetMasterUid() ) != NULL )
				{
					bool bShow = g_pTFieldGame->GetSquareUnitByUID( pPet->GetMasterUid() )->GetShowObject();
					pPet->SetShowObject( bShow );
					pPet->SetInfo( pPet->GetPetInfo() );
					pPet->SetShowGageBar( bShow );
				}			
			}

			if( pPet != NULL && ( pPet->GetShowObject() == true || g_pX2Game != NULL ) )
			{
				if( g_pX2Game != NULL && g_pX2Game->IsHost() == true )
				{
					pPet->SetEnableAi( true );
				}			
				else if( g_pX2Game == NULL )
				{
					if( pPet->IsMyPet() == true )
					{
						pPet->SetEnableAi(true);
					}
					else
					{
						if( m_iEnabledPetAi == iIndex )
						{
							pPet->SetEnableAi(true);
						}
						else
						{
							pPet->SetEnableAi(false);
						}			
						++iIndex;
					}				
				}

				pPet->OnFrameMove( fTime, fElapsedTime );
			}

			if( pPet != NULL )
			{
				pPet->SetShowEffect( pPet->GetShowObject() );
			}

			++iter;
		}

		++m_iEnabledPetAi;	
	}
	
	return S_OK;
}

HRESULT	CX2PetManager::OnFrameRender()
{
	RenderName();

	if( m_pTalkBoxMgr != NULL )
		m_pTalkBoxMgr->OnFrameRender();

	if( m_vecRemovePet.size() > 0 )
		RemovePetList();

	return S_OK;
}

bool CX2PetManager::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch(wParam)
	{
#ifdef CHANGE_PET_NAME
	case PCUM_CHANGE_NAME_OK:
		{ 
			if( m_iChangeItemUID > 0 )
			{
				if(m_pDlgChangePetName == NULL)
					return false;
				CKTDGUIIMEEditBox* pEditBox = (CKTDGUIIMEEditBox*) m_pDlgChangePetName->GetControl( L"IME_NewNick" );
				if( NULL != pEditBox )
				{ 
					wstring wstrPetName = pEditBox->GetText();

					if( wstrPetName == L"" )
					{
						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(270,350), GET_STRING( STR_ID_717 ), g_pMain->GetNowState() );							
						return false;
					}

					if( g_pMain->GetStringFilter()->CheckIsValidString( CX2StringFilter::FT_NICKNAME, wstrPetName.c_str() ) == false )
					{
						g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_717 ), g_pMain->GetNowState() );
						return false;
					} 
					
					if( g_pData != NULL && g_pData->GetPetManager() != NULL )
					{
						CX2PET* pPet = g_pData->GetPetManager()->GetMyPet(); 
						if( pPet != NULL)
							Handler_EGS_CHANGE_PET_NAME_REQ( m_iChangeItemUID ,wstrPetName.c_str()); 
					}   
				} 
			}
			return true; 
		}
		break;
	case PCUM_CHANGE_NAME_CLOSE:
		{ 
			ShowChangePetName(false);
			return true;
		}
		break;
#endif CHANGE_PET_NAME
	case PCUM_MAKE_NAME_CLOSE:
		{
			ShowMakePetName(false);
			return true;
		}
		break;
	case PCUM_MAKE_NAME_OK:
		{
			if( m_iMakePetId > 0 )
			{
				if( m_pDlgMakePetName != NULL )
				{
					CKTDGUIIMEEditBox* pIMEEditBox = (CKTDGUIIMEEditBox*)m_pDlgMakePetName->GetControl( L"MakePetNameEditBox" );
					if ( pIMEEditBox != NULL )
					{
						wstring wstrPetName = pIMEEditBox->GetText();

						if( wstrPetName == L"" )
						{
							// 펫 이름을 입력하세요
							g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(270,350), GET_STRING( STR_ID_717 ), g_pMain->GetNowState() );							
							return false;
						}

						if( g_pMain->GetStringFilter()->CheckIsValidString( CX2StringFilter::FT_NICKNAME, wstrPetName.c_str() ) == false )
						{
							g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_717 ), g_pMain->GetNowState() );
							return false;
						}

						g_pData->GetPetManager()->Handler_EGS_CREATE_PET_REQ( m_iMakePetId, pIMEEditBox->GetText() );
						ShowMakePetName(false);

						return true;
					}
				}
			}
			else
			{
				// 펫을 생성할 수 없습니다.
				ShowMakePetName(false);
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(270,350), GET_STRING( STR_ID_5343 ), g_pMain->GetNowState() );				
				return false;
			}			
		}
		break;
	case PCUM_SUMMON_CANCEL:
		{
			Handler_EGS_SUMMON_PET_REQ( 0 );
			if( m_pDlgPetPopupMenu != NULL )
				m_pDlgPetPopupMenu->SetShowEnable(false, false);
			if ( NULL != m_pDlgPopup )
				m_pDlgPopup->SetShowEnable(false, false);
			return true;
		}
		break;
	case PCUM_BASIC_INFO:
		{
			if( g_pData != NULL && g_pData->GetUIManager() != NULL )
			{
				g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU_PET_LIST, true, 1);
				if( m_pDlgPetPopupMenu != NULL )
					m_pDlgPetPopupMenu->SetShowEnable(false, false);
				if ( NULL != m_pDlgPopup )
					m_pDlgPopup->SetShowEnable(false, false);
				return true;
			}			
		}
		break;
	case PCUM_DETAIL:
		{
			if( g_pData != NULL && g_pData->GetUIManager() != NULL )
			{
				g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU_PET_LIST, true, 2);
				if( m_pDlgPetPopupMenu != NULL )
					m_pDlgPetPopupMenu->SetShowEnable(false, false);
				if ( NULL != m_pDlgPopup )
					m_pDlgPopup->SetShowEnable(false, false);
				return true;
			}			
		}
		break;
	case PCUM_INVEN:
		{
			if( g_pData != NULL && g_pData->GetUIManager() != NULL )
			{
				// oasis907 : 김상윤 [2010.9.7] //  펫 인벤토리
				g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU_INVEN, true );
				g_pData->GetUIManager()->GetUIInventory()->OpenPetInventory();

				if( m_pDlgPetPopupMenu != NULL )
					m_pDlgPetPopupMenu->SetShowEnable(false, false);
				if ( NULL != m_pDlgPopup )
					m_pDlgPopup->SetShowEnable(false, false);
					return true;
			}
		}
		break;
	case FCUM_FOOD:
		{
			if( g_pData != NULL && g_pData->GetUIManager() != NULL )
			{
				g_pData->GetUIManager()->ToggleUI(CX2UIManager::UI_MENU_INVEN, true );
				g_pData->GetUIManager()->GetUIInventory()->FeedPet();

				if( m_pDlgPetPopupMenu != NULL )
					m_pDlgPetPopupMenu->SetShowEnable(false, false);
				if ( NULL != m_pDlgPopup )
					m_pDlgPopup->SetShowEnable(false, false);
				return true;
			}
		}
		break;
	default:
		break;
	}

	return false;
}

bool CX2PetManager::UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( wParam )
	{
		
	case EGS_COMMANDS_FOR_PETS_ACK:
		return Handler_EGS_COMMANDS_FOR_PETS_ACK( hWnd, uMsg, wParam, lParam );
	case EGS_PET_ACTION_NOT:
		return Handler_EGS_PET_ACTION_NOT( hWnd, uMsg, wParam, lParam );
	case EGS_SUMMON_PET_ACK:
		return Handler_EGS_SUMMON_PET_ACK( hWnd, uMsg, wParam, lParam );
	case EGS_SUMMON_PET_NOT:
		return Handler_EGS_SUMMON_PET_NOT( hWnd, uMsg, wParam, lParam );
	case EGS_PET_EVOLUTION_ACK:
		return Handler_EGS_PET_EVOLUTION_ACK( hWnd, uMsg, wParam, lParam );
	case EGS_PET_EVOLUTION_NOT:
		return Handler_EGS_PET_EVOLUTION_NOT( hWnd, uMsg, wParam, lParam );
	case EGS_CHANGED_PET_SATIETY_NOT:
		return Handler_EGS_CHANGED_PET_SATIETY_NOT( hWnd, uMsg, wParam, lParam );
	case EGS_CHANGED_PET_INTIMACY_NOT:
		return Handler_EGS_CHANGED_PET_INTIMACY_NOT( hWnd, uMsg, wParam, lParam );
	case EGS_CHANGED_PET_SATIETY_INTIMACY_NOT:
		return Handler_EGS_CHANGED_PET_SATIETY_INTIMACY_NOT( hWnd, uMsg, wParam, lParam );
	case EGS_CREATE_PET_ACK:
		return Handler_EGS_CREATE_PET_ACK( hWnd, uMsg, wParam, lParam );
	case EGS_FEED_PETS_ACK:
		return Handler_EGS_FEED_PETS_ACK( hWnd, uMsg, wParam, lParam );
	case EGS_AUTO_FEED_NOT:	// oasis907 : 김상윤 [2010.9.9] // 자동 먹이 NOT
		return Handler_EGS_AUTO_FEED_NOT( hWnd, uMsg, wParam, lParam );
	case EGS_FIRST_FEED_PETS_NOT:
		return Handler_EGS_FIRST_FEED_PETS_NOT( hWnd, uMsg, wParam, lParam );
		//{{ 2011. 05. 23  김민성	큐피엘 젤리 친밀도 증감
#ifdef SERV_ADD_KEWPIEL_JELLY
	case EGS_EAT_SPECIAL_FEED_NOT:
		return Handler_EGS_EAT_SPECIAL_FEED_NOT( hWnd, uMsg, wParam, lParam );
#endif SERV_ADD_KEWPIEL_JELLY
#ifdef CHANGE_PET_NAME
	case EGS_CHANGE_PET_NAME_ACK:
		return Handler_EGS_CHANGE_PET_NAME_ACK(hWnd, uMsg, wParam, lParam );
#endif CHANGE_PET_NAME
		//}}
#ifdef SERV_HALLOWEEN_PUMPKIN_FAIRY_PET
	case EGS_CHANGE_PET_ID_NOT:
		return Handler_EGS_CHANGE_PET_ID_NOT(hWnd, uMsg, wParam, lParam );
#endif //SERV_HALLOWEEN_PUMPKIN_FAIRY_PET
#ifdef SERV_PERIOD_PET
	case EGS_RELEASE_PET_NOT:
		return Handler_EGS_RELEASE_PET_NOT( hWnd, uMsg, wParam, lParam );
#endif SERV_PERIOD_PET
#ifdef SERV_EVENT_VC
	case EGS_USE_INTIMACY_UP_ITEM_ACK:
		return Handler_EGS_USE_INTIMACY_UP_ITEM_ACK(hWnd, uMsg, wParam, lParam );
#endif //SERV_EVENT_VC
	default:
		break;
	}

	return false;
}

void CX2PetManager::AddPet(CX2PETPtr pPet)
{
	if( pPet == NULL )
		return;

	map<UidType, CX2PETPtr>::iterator iter;
	iter = m_mapPet.find( pPet->GetMasterUid() );
	if( iter == m_mapPet.end() )
	{
		m_mapPet.insert( std::make_pair( pPet->GetMasterUid(), pPet ) );
	}
}

void CX2PetManager::ChangePet(CX2PETPtr pPetPtr)
{
	if( pPetPtr == NULL )
		return;

	map<UidType, CX2PETPtr>::iterator iter;
	iter = m_mapPet.find( pPetPtr->GetMasterUid() );
	if( iter == m_mapPet.end() )
	{
		m_mapPet.insert( std::make_pair( pPetPtr->GetMasterUid(), pPetPtr ) );
	}
	else
	{
		CX2PETPtr pPet = iter->second;				
		pPet->SetShowObject(false);		
		RemovePet( pPet->GetMasterUid() );
		//m_mapPet.erase(iter);
		
		m_mapPet.insert( std::make_pair( pPetPtr->GetMasterUid(), pPetPtr ) );
	}
}

bool CX2PetManager::IsValidPet(CX2PET *pPet)
{
	map<UidType, CX2PETPtr>::iterator iter;
	for( iter = m_mapPet.begin(); iter != m_mapPet.end(); ++iter )
	{
		CX2PETPtr pPetPtr = (CX2PETPtr)iter->second;
		if( pPetPtr.get() && pPet ==  pPetPtr.get() )
		{
			return true;
		}
	}
	return false;
}


map<UidType, CX2PETPtr>::iterator CX2PetManager::RemovePet(UidType uMasterUid)
{
#if 0 // 즉시 지우지 말고 매프레임 정상적으로 지울수 있을때 마다 실시간으로 지운다.(크래쉬 관련 수정)
	map<UidType, CX2PETPtr>::iterator iter;
	iter = m_mapPet.find( uMasterUid );
	if( iter != m_mapPet.end() )
	{
		CX2PETPtr pPet = iter->second;
		//SAFE_DELETE_KTDGOBJECT(pPet);
		//pPet.reset();
		m_mapPet.erase(iter);
	}
#else
	map<UidType, CX2PETPtr>::iterator iter = m_mapPet.find( uMasterUid );
	if( iter != m_mapPet.end() )
	{		
		bool bFind = false;
		CX2PETPtr pPet = iter->second;
		for(UINT i=0; i<m_vecRemovePet.size(); ++i)
		{
			CX2PETPtr pPetPtr = m_vecRemovePet[i];
			if( pPetPtr == pPet )
			{
				bFind = true;
				break;
			}
		}
#ifdef HIDE_PET_GAGE
		pPet->SetShowGageBar(false);
#endif //HIDE_PET_GAGE
		pPet->SetShowObject(false);
		if( bFind == false )
			m_vecRemovePet.push_back( pPet );
		iter = m_mapPet.erase( iter );
	}

	return iter;
#endif
}

void CX2PetManager::AllRemovePet()
{
	map<UidType, CX2PETPtr>::iterator iter;
	for( iter = m_mapPet.begin(); iter != m_mapPet.end(); ++iter )
	{
		CX2PETPtr pPet = iter->second;
//		if( pPet.get() != NULL )
//			pPet.reset();
		//SAFE_DELETE_KTDGOBJECT( pPet );
	}
	m_mapPet.clear();

	RemovePetList();
}

void CX2PetManager::RemovePetList()
{
	for( UINT i=0; i<m_vecRemovePet.size(); ++i)
	{
		CX2PETPtr pPet = m_vecRemovePet[i];
//		if( pPet.get() != NULL )
//			pPet.reset();
	}
	m_vecRemovePet.clear();
}

CX2PET* CX2PetManager::GetMyPet()
{
	UidType uMasterUid = -1;

	if ( NULL != g_pData && NULL != g_pData->GetMyUser() && NULL != g_pData->GetMyUser()->GetSelectUnit() )
		uMasterUid = g_pData->GetMyUser()->GetSelectUnit()->GetUID();

	return GetPet( uMasterUid );
}

CX2PET* CX2PetManager::GetPet(UidType uMasterUid)
{
	map<UidType, CX2PETPtr>::iterator iter;
	iter = m_mapPet.find( uMasterUid );
	if( iter == m_mapPet.end() )
	{
		return NULL;
	}
	else
	{		
		CX2PETPtr pPet = iter->second;
		return pPet.get();
	}

	return NULL;
}

CX2PET *CX2PetManager::GetPetInx(int iInx)
{
	int tempInx = 0;
	map<UidType, CX2PETPtr>::iterator iter;
	for( iter = m_mapPet.begin(); iter != m_mapPet.end(); ++iter )
	{
		CX2PETPtr pPet = iter->second;
		if( tempInx == iInx )
			return pPet.get();
		++tempInx;
	}

	return NULL;
}

CX2PET *CX2PetManager::GetPetByUid( UidType iPetUid )
{
	
	map<UidType, CX2PETPtr>::iterator iter;
	for( iter = m_mapPet.begin(); iter != m_mapPet.end(); ++iter )
	{
		CX2PETPtr pPet = iter->second;
		if( pPet->GetPetInfo().m_PetUid == iPetUid )
			return pPet.get();
	}

	return NULL;
}

char CX2PetManager::GetPetStatus( int petId, int iLv )
{
	PetTemplet *pPetTemplet = GetPetTemplet((CX2PetManager::PET_UNIT_ID)petId);

	if( pPetTemplet == NULL )
		return 0;

	if( (int)pPetTemplet->m_vecPetStatus.size() <= iLv )
		return 0;

	return pPetTemplet->m_vecPetStatus[ iLv ];
}

char CX2PetManager::GetPetStatus(  CX2PET::PetInfo& petInfo )
{
	PetTemplet *pPetTemplet = GetPetTemplet((CX2PetManager::PET_UNIT_ID)petInfo.m_PetId);

	if( pPetTemplet == NULL )
		return 0;

	if( (int)pPetTemplet->m_vecPetStatus.size() <= petInfo.m_Evolution_Step )
		return 0;

	return pPetTemplet->m_vecPetStatus[ petInfo.m_Evolution_Step ];
}

void CX2PetManager::CreatePet(UidType uMasterUid, CX2PET::PetInfo& petInfo, bool bUpgrade, D3DXVECTOR3 vInitPos)
{
	PetTemplet *pPetTemplet = GetPetTemplet((CX2PetManager::PET_UNIT_ID)petInfo.m_PetId);

	switch( g_pMain->GetNowStateID() )
	{
	case CX2Main::XS_PVP_LOBBY:
	case CX2Main::XS_PVP_ROOM:
	case CX2Main::XS_PVP_GAME:
	case CX2Main::XS_PVP_RESULT:
	case CX2Main::XS_SQUARE_GAME:
	case CX2Main::XS_DUNGEON_RESULT:
#ifdef ADDED_RELATIONSHIP_SYSTEM
	case CX2Main::XS_WEDDING_GAME:
#endif // ADDED_RELATIONSHIP_SYSTEM
		{
			return;
		} break;
	default:
		break;
	}

#ifdef SERV_PVP_NEW_SYSTEM
	if( g_pX2Room != NULL )
	{
		CX2PVPRoom *pPvpRoom = (CX2PVPRoom*)g_pX2Room;
		if( pPvpRoom->IsOfficialPvp() == true )
			return;
	}
#endif

	if( pPetTemplet == NULL )
		return;

#ifdef	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD
	if ( (int)pPetTemplet->m_Evolution_Step_InitLuaTemplet.size() <= petInfo.m_Evolution_Step )
		return;
	KLuaManager* pLuaManager = pPetTemplet->m_Evolution_Step_InitLuaTemplet[ petInfo.m_Evolution_Step ].m_pLuaManager;
	if ( pLuaManager == NULL )
		return;
#else	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD
	if( (int)pPetTemplet->m_Evolution_Step_ScriptName.size() <= petInfo.m_Evolution_Step )
		return;
	wstring wstrPetScript = pPetTemplet->m_Evolution_Step_ScriptName[petInfo.m_Evolution_Step];
#endif	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD
    
	//background!!!
	//g_pKTDXApp->GetDeviceManager()->EnableThreadLoading( true );

	CX2PETPtr pPet = CX2PET::CreatePet();
#ifdef	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD
	pPet->SetPet( pLuaManager->GetLuaState(), uMasterUid );
#else	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD
	pPet->SetPet( wstrPetScript.c_str(), uMasterUid );
#endif	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD
	pPet->SetInfo( petInfo);

	if( bUpgrade == true )
	{
		pPet->SetInitPosFlag(true);
		pPet->SetPosition(vInitPos, true);		
	}

	g_pKTDXApp->GetDGManager()->AddObjectChain( pPet.get() );
	
	ChangePet( pPet );

	//g_pKTDXApp->GetDeviceManager()->EnableThreadLoading( false );

// 	if( g_pKTDXApp->GetDeviceManager()->IsThreadSuspended() == true )
// 		g_pKTDXApp->GetDeviceManager()->RequestResumeThread();
	
}

CX2PETPtr CX2PetManager::CreateGamePet(UidType uMasterUid, CX2PET::PetInfo& petInfo)
{
	PetTemplet *pPetTemplet = GetPetTemplet((CX2PetManager::PET_UNIT_ID)petInfo.m_PetId);


	switch( g_pMain->GetNowStateID() )
	{
	case CX2Main::XS_PVP_LOBBY:
	case CX2Main::XS_PVP_ROOM:
	case CX2Main::XS_PVP_GAME:
	case CX2Main::XS_PVP_RESULT:
	case CX2Main::XS_SQUARE_GAME:
	case CX2Main::XS_DUNGEON_RESULT:
#ifdef ADDED_RELATIONSHIP_SYSTEM
	case CX2Main::XS_WEDDING_GAME:
#endif // ADDED_RELATIONSHIP_SYSTEM
		{
			//정의된 게임상태에서는 펫관련 작업하지 않는다.
			return CX2PETPtr();
		} break;
	default:
		break;
	}

	if( pPetTemplet == NULL )
		return CX2PETPtr();

#ifdef	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD
	if ( (int)pPetTemplet->m_Evolution_Step_InitLuaTemplet.size() <= petInfo.m_Evolution_Step )
		return CX2PETPtr();
	KLuaManager* pLuaManager = pPetTemplet->m_Evolution_Step_InitLuaTemplet[ petInfo.m_Evolution_Step ].m_pLuaManager;
	if ( pLuaManager == NULL )
		return CX2PETPtr();
#else	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD
	if( (int)pPetTemplet->m_Evolution_Step_ScriptName.size() <= petInfo.m_Evolution_Step )
		return CX2PETPtr();
	const wstring& wstrPetScript = pPetTemplet->m_Evolution_Step_ScriptName[petInfo.m_Evolution_Step];
#endif	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD
	
	//g_pKTDXApp->GetDeviceManager()->EnableThreadLoading( true );

	CX2PETPtr pPet = CX2PET::CreatePet();
#ifdef	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD
	pPet->SetGamePet( pLuaManager->GetLuaState(), uMasterUid );	
#else	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD
	pPet->SetGamePet( wstrPetScript.c_str(), uMasterUid );	
#endif	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD
	pPet->SetInfo( petInfo);
	g_pKTDXApp->GetDGManager()->AddObjectChain( pPet.get() );
	pPet->OnFrameMove( g_NowTime, 0.F );

	//AddPet( pPet );
	ChangePet( pPet );

	return pPet;
	//g_pKTDXApp->GetDeviceManager()->EnableThreadLoading( false );
	
	//if( g_pKTDXApp->GetDeviceManager()->IsThreadSuspended() == true )
	//	g_pKTDXApp->GetDeviceManager()->RequestResumeThread();
	
}
#ifdef CHANGE_PET_NAME
void CX2PetManager::ShowChangePetName(bool bVal, UidType iChangeItemId)
{
	if( bVal == true )
	{
		if( m_pDlgChangePetName == NULL )
		{
			m_pDlgChangePetName = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Change_PetName.lua" );
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDlgChangePetName ); 
			m_pDlgChangePetName->SetLayer(XDL_POP_UP); 			
		}
		m_iChangeItemUID = iChangeItemId;
		m_pDlgChangePetName->SetShow( true );  
	}
	else
	{
		g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDlgChangePetName, NULL, false );
		m_pDlgChangePetName = NULL;
		m_iChangeItemUID = -1;
	} 
} 
#endif CHANGE_PET_NAME
void CX2PetManager::ShowMakePetName(bool bVal, UidType iMakePetId)
{
	if( m_pDlgMakePetName == NULL )
	{
		m_pDlgMakePetName = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Pet_Name_Make.lua" );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDlgMakePetName );
		//m_pDlgMakePetName->SetDisableUnderWindow(true);
	}
	
	m_pDlgMakePetName->SetShow( bVal );	

	if( bVal == false )
	{
		g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDlgMakePetName, NULL, false );
		m_pDlgMakePetName = NULL;
		m_iMakePetId = -1;
	}
	else
	{
		m_iMakePetId = iMakePetId;
	}
}

bool CX2PetManager::CanMyPetEvolution()
{
	if( m_vecMyPetEvolutionList.size() > 0 )
		return true;
	return false;
}

bool CX2PetManager::GetNerverSummonPet()
{
	if( m_vecNeverSummonPetList.size() > 0 )
		return true;
	return false;
}

bool CX2PetManager::ForceUpgradePet()
{
	// 테스트를 위한 강제 진화
	CX2PET *pPet = GetMyPet();

	if( pPet == NULL )
		return false;

	g_pData->GetServerProtocol()->SendID( EGS_PET_EVOLUTION_REQ );

#if 0
	KPetInfo *pKPetInfo = g_pData->GetMyUser()->GetSelectUnit()->GetPetInfo();
	pKPetInfo->m_cEvolutionStep += 1;
	pKPetInfo->m_sIntimacy = 0;

#ifdef SERV_PETID_DATA_TYPE_CHANGE //2013.07.02
	PetTemplet *pPetTemplet = GetPetTemplet((CX2PetManager::PET_UNIT_ID)pKPetInfo->m_iPetID);
#else //SERV_PETID_DATA_TYPE_CHANGE
	PetTemplet *pPetTemplet = GetPetTemplet((CX2PetManager::PET_UNIT_ID)pKPetInfo->m_cPetID);
#endif //SERV_PETID_DATA_TYPE_CHANGE
	if( (int)pPetTemplet->m_Evolution_Step_ScriptName.size() <= (int)pKPetInfo->m_cEvolutionStep )
	{		
		pKPetInfo->m_cEvolutionStep = 0;
	}

	CX2PET::PetInfo petInfo;

	petInfo.m_PetUid = pKPetInfo->m_iPetUID;
#ifdef SERV_PETID_DATA_TYPE_CHANGE //2013.07.02
	petInfo.m_PetId = pKPetInfo->m_iPetID;
#else //SERV_PETID_DATA_TYPE_CHANGE
	petInfo.m_PetId = pKPetInfo->m_cPetID;
#endif //SERV_PETID_DATA_TYPE_CHANGE
	petInfo.m_Evolution_Step = pKPetInfo->m_cEvolutionStep;
	petInfo.m_wstrPetName = pKPetInfo->m_wstrPetName;		
	petInfo.m_Intimacy = (float)pKPetInfo->m_iIntimacy;
	petInfo.m_Satiety = pKPetInfo->m_sSatiety;
	petInfo.m_Extroversion = pKPetInfo->m_sExtroversion;
	petInfo.m_Emotion = pKPetInfo->m_sEmotion;
	petInfo.m_bSummon = true;
#ifdef SERV_PET_SYSTEM_EX1
	petInfo.m_bAlwaysMaxSatiety = pKPetInfo->m_bAlwaysMaxSatiety;
#endif //SERV_PET_SYSTEM_EX1

	D3DXVECTOR3 vInitPosBackup = pPet->GetPos();

	RemovePet( g_pData->GetMyUser()->GetSelectUnit()->GetUID() );

	
	g_pData->GetPetManager()->CreatePet( g_pData->GetMyUser()->GetSelectUnit()->GetUID(), petInfo, true, vInitPosBackup );
#endif

	return true;
}

void CX2PetManager::SetAllShow(bool bVal) 
{ 
	m_bShow = bVal; 
	
	map<UidType, CX2PETPtr>::iterator iter;

	for( iter = m_mapPet.begin(); iter != m_mapPet.end(); ++iter )
	{
		CX2PETPtr pPet = iter->second;
		pPet->SetShowObject( m_bShow );			
	}
}

void CX2PetManager::SetShow(UidType uMasterUid, bool bVal)
{
	CX2PET* pPet = GetPet(uMasterUid);
	if( pPet != NULL )
	{
		pPet->SetShowObject( bVal );		
	}
}

bool CX2PetManager::GetShow(UidType uMasterUid)
{
	CX2PET* pPet = GetPet(uMasterUid);
	if( pPet != NULL )
	{
		return pPet->GetShowObject();
	}

	return false;
}

void CX2PetManager::NotifyShowObjectChanged(UidType uMasterUid, bool bVal)
{	
	CX2PET* pPet = GetPet(uMasterUid);

	if( pPet != NULL && bVal == true)
	{		
		pPet->InitPos();
	}
	if( pPet != NULL )
	{
		pPet->SetShowObject( bVal );		
	}	
}

void CX2PetManager::InitPos()
{
	map<UidType, CX2PETPtr>::iterator iter;
	for( iter = m_mapPet.begin(); iter != m_mapPet.end(); ++iter )
	{
		CX2PETPtr pPet = iter->second;
		if( pPet != NULL )
		{
			pPet->SetInitPosFlag(false);
			pPet->InitPos();
		}
	}
}

void CX2PetManager::SendPetActionCommand( CX2PET::PET_ACTION_COMMAND ePetAction )
{
	Handler_EGS_COMMANDS_FOR_PETS_REQ( ePetAction );
}

CX2PET::PET_ACTION_COMMAND CX2PetManager::GetPetActionCommand( wstring &wstrMsg )
{
	if ( NULL != g_pData && NULL != g_pData->GetMyUser() && NULL != g_pData->GetMyUser()->GetSelectUnit() )
	{
		CX2PET *pPet = GetPet( g_pData->GetMyUser()->GetSelectUnit()->GetUID() );
		PetTemplet *pTemplet = GetPetTemplet( (PET_UNIT_ID)pPet->GetPetInfo().m_PetId );

		if( pPet == NULL )
			return CX2PET::PAC_NONE;	
		if( pTemplet == NULL )
			return CX2PET::PAC_NONE;

		int nowState = pPet->GetNowState();

		std::vector<wstring> vecTokStr;
		WCHAR *token = NULL;	

		token = wcstok((WCHAR*)wstrMsg.c_str(), L" ");
		if( token == NULL )
			return CX2PET::PAC_NONE;

		vecTokStr.push_back(token);
		while( (token = wcstok(NULL, L" ")) != NULL )
		{
			vecTokStr.push_back(token);
		}

		for(UINT i=0; i<vecTokStr.size(); ++i)
		{
#ifdef GLOBAL_CHAT_FUNCTION
		wstring  wstrToken = vecTokStr[i].c_str();

		if( wstrToken.compare(GET_STRING(STR_ID_10413)) == 0 || wstrToken.compare(GET_STRING(STR_ID_10414)) == 0 ||
			wstrToken.compare(GET_STRING(STR_ID_18198)) == 0 || wstrToken.compare(GET_STRING(STR_ID_10415)) == 0 )
		{
			// hello			
			return CX2PET::PAC_HELLO;
		}
		if( wstrToken.compare(GET_STRING(STR_ID_10416)) == 0 || wstrToken.compare(GET_STRING(STR_ID_10417)) == 0 ||
			wstrToken.compare(GET_STRING(STR_ID_10418)) == 0 )
		{
			// sitstart			
			return CX2PET::PAC_SIT;
		}
		if( wstrToken.compare(GET_STRING(STR_ID_10419)) == 0 || wstrToken.compare(GET_STRING(STR_ID_10420)) == 0 ||
			wstrToken.compare(GET_STRING(STR_ID_10421)) == 0 || wstrToken.compare(GET_STRING(STR_ID_10422)) == 0 ||
			wstrToken.compare(GET_STRING(STR_ID_10423)) == 0 || wstrToken.compare(GET_STRING(STR_ID_10424)) == 0 )
		{
			// sitend || sleepend
			if( nowState == pPet->m_SitLoopState )
			{			
				return CX2PET::PAC_SIT_END;
			}
			else if( nowState == pPet->m_SleepLoopState )
			{				
				return CX2PET::PAC_SLEEP_END;
			}

			return CX2PET::PAC_NONE;
		}
		if( wstrToken.compare(GET_STRING(STR_ID_10425)) == 0 || wstrToken.compare(GET_STRING(STR_ID_10428)) == 0 )
		{
			// cute			
			return CX2PET::PAC_CUTE;
		}
		if( wstrToken.compare(GET_STRING(STR_ID_10429)) == 0 || wstrToken.compare(GET_STRING(STR_ID_10430)) == 0 ||  
			wstrToken.compare(GET_STRING(STR_ID_10431)) == 0 || wstrToken.compare(GET_STRING(STR_ID_10432)) == 0 )
		{
			// afraid			
			return CX2PET::PAC_AFRAID;
		}
		if( wstrToken.compare(GET_STRING(STR_ID_10433)) == 0 || wstrToken.compare(GET_STRING(STR_ID_10434)) == 0 ||
			wstrToken.compare(GET_STRING(STR_ID_10441)) == 0 )
		{
			// sleepstart			
			return CX2PET::PAC_SLEEP;
		}
		if( wstrToken.compare(GET_STRING(STR_ID_10436)) == 0 || wstrToken.compare(GET_STRING(STR_ID_10437)) == 0 ||
			wstrToken.compare(GET_STRING(STR_ID_10438)) == 0 )
		{
			// happy			
			return CX2PET::PAC_HAPPY;
		}

		if( g_pData->GetPetManager()->GetPetStatus( pPet->GetPetInfo() ) > 1 )
		{
			for(UINT j=0; j<pTemplet->m_SpecialActionCommand.size(); ++j)
			{
				wstring wstrActionCommand = pTemplet->m_SpecialActionCommand[j];
				if( wstrToken.compare( wstrActionCommand.c_str() ) == 0 )
				{
					// special			
					return CX2PET::PAC_SPECIAL;
				}
			}
		}		

#ifdef _IN_HOUSE_
		if( wstrToken.compare(GET_STRING(STR_ID_10439)) == 0 )
		{
			// happy			
			return CX2PET::PAC_ATTACK;
		}
		if( wstrToken.compare(GET_STRING(STR_ID_10440)) == 0 )
		{
			// happy			
			return CX2PET::PAC_CHEER;
		}
#endif
#else
			wstring  wstrToken = vecTokStr[i].c_str();

			if( wstrToken.compare(L"인사!") == 0 || wstrToken.compare(L"안녕!") == 0 || wstrToken.compare(L"하이!") == 0 || wstrToken.compare(L"헬로!") == 0 )
			{
				// hello			
				return CX2PET::PAC_HELLO;
			}
			if( wstrToken.compare(L"앉아!") == 0 || wstrToken.compare(L"기다려!") == 0 || wstrToken.compare(L"쉬어!") == 0 )
			{
				// sitstart			
				return CX2PET::PAC_SIT;
			}
			if( wstrToken.compare(L"서!") == 0 || wstrToken.compare(L"일어나!") == 0 || wstrToken.compare(L"일어서!") == 0 ||
				wstrToken.compare(L"따라와!") == 0 || wstrToken.compare(L"이리와!") == 0 || wstrToken.compare(L"컴온!") == 0 )
			{
				// sitend || sleepend
				if( nowState == pPet->m_SitLoopState )
				{			
					return CX2PET::PAC_SIT_END;
				}
				else if( nowState == pPet->m_SleepLoopState )
				{				
					return CX2PET::PAC_SLEEP_END;
				}

				return CX2PET::PAC_NONE;
			}
			if( wstrToken.compare(L"애교!") == 0 || wstrToken.compare(L"메리메리!") == 0 || wstrToken.compare(L"쫑!") == 0 || wstrToken.compare(L"이쁜짓!") == 0)
			{
				// cute			
				return CX2PET::PAC_CUTE;
			}
			if( wstrToken.compare(L"때찌!") == 0 || wstrToken.compare(L"하지마!") == 0 ||  
				wstrToken.compare( L"혼난다!") == 0 || wstrToken.compare(L"까분다!") == 0 )
			{
				// afraid			
				return CX2PET::PAC_AFRAID;
			}
			if( wstrToken.compare( L"자자!") == 0 || wstrToken.compare(L"잘자!") == 0 || wstrToken.compare(L"쿨쿨!") == 0 || wstrToken.compare(L"코자자!") == 0 )
			{
				// sleepstart			
				return CX2PET::PAC_SLEEP;
			}
			if( wstrToken.compare( L"사랑해!") == 0 || wstrToken.compare( L"좋아해!") == 0 || wstrToken.compare(L"착하지!") == 0 )
			{
				// happy			
				return CX2PET::PAC_HAPPY;
			}

			if( g_pData->GetPetManager()->GetPetStatus( pPet->GetPetInfo() ) > 1 )
			{
				for(UINT j=0; j<pTemplet->m_SpecialActionCommand.size(); ++j)
				{
					wstring wstrActionCommand = pTemplet->m_SpecialActionCommand[j];
					if( wstrToken.compare( wstrActionCommand.c_str() ) == 0 )
					{
						// special			
						return CX2PET::PAC_SPECIAL;
					}
				}
			}		

	#ifdef _IN_HOUSE_
			if( wstrToken.compare( L"공격!") == 0 )
			{
				// happy			
				return CX2PET::PAC_ATTACK;
			}
			if( wstrToken.compare( L"격려!") == 0 )
			{
				// happy			
				return CX2PET::PAC_CHEER;
			}
	#endif
#endif GLOBAL_CHAT_FUNCTION	
		}
	}

	return CX2PET::PAC_NONE;
}


void CX2PetManager::PlayEmotion(UidType userid, CX2PET::PET_ACTION_COMMAND ePetAction)
{
	CX2PET *pPet = GetPet(userid);

	if( pPet == NULL )
		return;

	switch( ePetAction )
	{
	case CX2PET::PAC_DONOTKNOW:
		pPet->StateChange( pPet->m_NotKnowState, false, false );
		break;
	case CX2PET::PAC_HUNGRY:
		pPet->StateChange( pPet->m_HungryState, false, false );
		break;
	case CX2PET::PAC_HELLO:
		pPet->StateChange( pPet->m_HelloState, false, false );
		break;
	case CX2PET::PAC_SIT:
		{
			if( pPet->GetFlyUnit() == true && pPet->GetFootOnLine() == false )
				pPet->StateChange( pPet->m_SitFallState, false, false );
			else
				pPet->StateChange( pPet->m_SitStartState, false, false );
		}
		break;
	case CX2PET::PAC_SIT_END:
		pPet->StateChange( pPet->m_SitEndState, false, false );
		break;
	case CX2PET::PAC_CUTE:
		{
			int irand = rand() % 2;
			if( irand == 0 )
			{
				pPet->StateChange( pPet->m_Cute1State, true, false );
			}
			else
			{
				pPet->StateChange( pPet->m_Cute2State, true, false );
			}
		}
		break;
	case CX2PET::PAC_AFRAID:
		pPet->StateChange( pPet->m_AfraidState, false, false );
		break;
	case CX2PET::PAC_SLEEP:	
		{
			if( pPet->GetFlyUnit() == true && pPet->GetFootOnLine() == false )
				pPet->StateChange( pPet->m_SleepFallState, false, false );
			else
				pPet->StateChange( pPet->m_SleepStartState, false, false );
		}
		break;
	case CX2PET::PAC_SLEEP_END:
		pPet->StateChange( pPet->m_SleepEndState, false, false );
		break;
	case CX2PET::PAC_HAPPY:
		if( rand() %2 == 0 )
			pPet->StateChange( pPet->m_Happy1State, false, false );
		else
			pPet->StateChange( pPet->m_Happy2State, false, false );
		break;
	case CX2PET::PAC_EAT:
		pPet->StateChange( pPet->m_EatState, false, false );
		break;
	case CX2PET::PAC_STUFFED:
		pPet->StateChange( pPet->m_StuffedState, false, false );
		break;
	case CX2PET::PAC_FORCE_SLEEP:
		{
			if( pPet->GetFlyUnit() == true && pPet->GetFootOnLine() == false )
				pPet->StateChange( pPet->m_SleepFallState, false, false );
			else
				pPet->StateChange( pPet->m_SleepStartState, false, false );
		}
		break;
	case CX2PET::PAC_SPECIAL:
		pPet->StateChange( pPet->m_SpecialState, false, false );
		break;
	default:
		break;
	}

}





bool CX2PetManager::OpenScriptFile( const WCHAR* pFileName )
{
	lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(),  "g_pPetManager", this );


    if ( g_pKTDXApp->LoadLuaTinker( pFileName ) == false )
    {
		ErrorLogMsg( XEM_ERROR68, pFileName );

		return false;
    }

#ifdef	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD
	LoadInitPetData();
#endif	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD

	return true;
}

#ifdef	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD
void CX2PetManager::LoadInitPetData()
{
	for( std::map<PET_UNIT_ID, PetTemplet*>::const_iterator iter = m_PetTemplet.begin();
		iter != m_PetTemplet.end();
		++iter )
	{
		PetTemplet* pPetTemplet = iter->second;
		if ( pPetTemplet != NULL )
		{
			pPetTemplet->m_Evolution_Step_InitLuaTemplet.resize( 0 );
			unsigned	uSize = pPetTemplet->m_Evolution_Step_ScriptName.size();
			pPetTemplet->m_Evolution_Step_InitLuaTemplet.resize( uSize );
			for( unsigned uIV = 0; uIV != uSize; ++uIV )
			{
				pPetTemplet->m_Evolution_Step_InitLuaTemplet[ uIV ].m_pInitTemplet = new PetInitTemplet;
				pPetTemplet->m_Evolution_Step_InitLuaTemplet[ uIV ].m_pLuaManager 
					= new KLuaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState(), 0, true );
				//g_pKTDXApp->LoadLuaTinker( pUnitTypeTemplet->m_wstrLuaScriptFile.c_str() );
				if ( g_pKTDXApp->LoadAndDoMemory( 
					pPetTemplet->m_Evolution_Step_InitLuaTemplet[ uIV ].m_pLuaManager, 
					pPetTemplet->m_Evolution_Step_ScriptName[ uIV ].c_str() ) == false )
					continue;
				
				CX2PET::InitInit( pPetTemplet->m_Evolution_Step_InitLuaTemplet[ uIV ].m_pInitTemplet->m_init, 
					*pPetTemplet->m_Evolution_Step_InitLuaTemplet[ uIV ].m_pLuaManager );
			}//for
		}
	}
}
#endif	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD


void CX2PetManager::RenderName()
{
	map<UidType, CX2PETPtr>::iterator iter;
	for( iter = m_mapPet.begin(); iter != m_mapPet.end(); ++iter )
	{
		CX2PETPtr pPet = iter->second;
		if( pPet != NULL )
			pPet->RenderName();
	}
}

bool CX2PetManager::AddPetTemplet_LUA()
{
	PetTemplet* pPetTemplet = new PetTemplet();
	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
#ifndef X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
	TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING

	LUA_GET_VALUE_RETURN_ENUM(	luaManager, "PET_ID",				pPetTemplet->m_PetId,				PET_UNIT_ID,	PUI_NONE,	SAFE_DELETE(pPetTemplet); return false; );
	LUA_GET_VALUE(				luaManager, "COMMON_NAME",			pPetTemplet->m_wstrCommonName ,		L"" );
	LUA_GET_VALUE(				luaManager, "CAPRICE",				pPetTemplet->m_caprice ,			1.f );
	LUA_GET_VALUE(				luaManager, "APPETITE",			pPetTemplet->m_appetite ,			1.f );
	LUA_GET_VALUE(				luaManager, "EVOLUTION_ALL_STEP",	pPetTemplet->m_Evolution_All_Step ,	4 );

#ifdef SERV_PET_SYSTEM_EX1
	LUA_GET_VALUE(				luaManager, "ALWAYS_MAX_SATIETY",	pPetTemplet->m_bAlwaysMaxSatiety,	false );
#endif //SERV_PET_SYSTEM_EX1
	
	if( luaManager.BeginTable( "PET_STATUS" ) == true )
	{
		// 0: 알, 1: 유체, 2: 기타(성체,완전체 등등)
		int tableIndex = 1;
		int iStatus = 2;
		while( luaManager.GetValue( tableIndex, iStatus ) == true )
		{
			pPetTemplet->m_vecPetStatus.push_back( iStatus );
			tableIndex++;
		}
		luaManager.EndTable();
	}	

	if( luaManager.BeginTable( "INVENSLOT_WIDTH_SIZE" ) == true )
	{
		int tableIndex = 1;
		int iInvenSlotWidthSize = -1;
		while( luaManager.GetValue( tableIndex, iInvenSlotWidthSize ) == true )
		{
			pPetTemplet->m_InvenSlot_Width_Size.push_back( iInvenSlotWidthSize );
			tableIndex++;
		}
		luaManager.EndTable();
	}

	if( luaManager.BeginTable( "INVENSLOT_HEIGHT_SIZE" ) == true )
	{
		int tableIndex = 1;
		int iInvenSlotHeightSize = -1;
		while( luaManager.GetValue( tableIndex, iInvenSlotHeightSize ) == true )
		{
			pPetTemplet->m_InvenSlot_Height_Size.push_back( iInvenSlotHeightSize );
			tableIndex++;
		}
		luaManager.EndTable();
	}	

	if( luaManager.BeginTable( "EVOLUTION_STEP_MAX" ) == true )
	{
		int tableIndex = 1;
		float fEvolutionStepMax = -1.f;
		while( luaManager.GetValue( tableIndex, fEvolutionStepMax ) == true )
		{
			pPetTemplet->m_Evolution_Step_Max.push_back( fEvolutionStepMax );
			tableIndex++;
		}
		luaManager.EndTable();
	}

	if( luaManager.BeginTable( "EVOLUTION_STEP_SCRIPTNAME" ) == true )
	{
		int tableIndex = 1;
		wstring wstrScriptName = L"";
		while( luaManager.GetValue( tableIndex, wstrScriptName ) == true )
		{
			pPetTemplet->m_Evolution_Step_ScriptName.push_back( wstrScriptName );
			tableIndex++;
		}
		luaManager.EndTable();
	}

	if( luaManager.BeginTable( "EVOLUTION_STEP_IMAGE" ) == true )
	{
		int index = 1;
		while( luaManager.BeginTable( index ) == true )
		{
			PetStepImage petStepImage;
			
			LUA_GET_VALUE( luaManager, 1, petStepImage.m_wstrImageName,				L""	);
			LUA_GET_VALUE( luaManager, 2, petStepImage.m_wstrKeyName,				L""	);
			
			pPetTemplet->m_Evolution_Step_Image.push_back( petStepImage );
			luaManager.EndTable();

			index++;
		}
		luaManager.EndTable();
	}

	if( luaManager.BeginTable( "EVOLUTION_STEP_GAME_IMAGE" ) == true )
	{
		int index = 1;
		while( luaManager.BeginTable( index ) == true )
		{
			PetStepImage petStepImage;

			LUA_GET_VALUE( luaManager, 1, petStepImage.m_wstrImageName,				L""	);
			LUA_GET_VALUE( luaManager, 2, petStepImage.m_wstrKeyName,				L""	);

			pPetTemplet->m_Evolution_Step_Game_Image.push_back( petStepImage );
			luaManager.EndTable();

			index++;
		}
		luaManager.EndTable();
	}

	// 진화별 공격스킬 아이콘 및 설명
	if( luaManager.BeginTable( "ATTACK_SKILL_INFO" ) == true )
	{
		int index = 1;
		while( luaManager.BeginTable( index ) == true )
		{
			PetSkillInfo petSkillInfo;

			int iSkillDesc = STR_ID_27;
			LUA_GET_VALUE( luaManager, 1, petSkillInfo.m_wstrImageName,				L""	);
			LUA_GET_VALUE( luaManager, 2, petSkillInfo.m_wstrKeyName,				L""	);
#ifdef CHANGE_PET_STRING_ID //2013.07.08
			wstring wstrTemp = L"";
			LUA_GET_VALUE( luaManager, 3, wstrTemp,	L""	);
			pPetTemplet->m_vecSkillName.push_back( wstrTemp );
			petSkillInfo.m_wstrSkillDesc = wstrTemp;
			LUA_GET_VALUE( luaManager, 4, wstrTemp,	L""	);
			petSkillInfo.m_wstrSkillDesc += L"\n\n";
			petSkillInfo.m_wstrSkillDesc += wstrTemp;
			petSkillInfo.m_wstrSkillDesc += L"\n";
#else //CHANGE_PET_STRING_ID
			LUA_GET_VALUE( luaManager, 3, iSkillDesc,				STR_ID_27	);
			petSkillInfo.m_wstrSkillDesc = GET_STRING( iSkillDesc );
			pPetTemplet->m_vecSkillName.push_back( GET_STRING( iSkillDesc ) );
			LUA_GET_VALUE( luaManager, 4, iSkillDesc,				STR_ID_27	);
			petSkillInfo.m_wstrSkillDesc += L"\n\n";
			petSkillInfo.m_wstrSkillDesc += GET_STRING( iSkillDesc );
			petSkillInfo.m_wstrSkillDesc += L"\n";
#endif //CHANGE_PET_STRING_ID

			pPetTemplet->m_AttackSkill_Step.push_back( petSkillInfo );
			luaManager.EndTable();

			index++;
		}
		luaManager.EndTable();
	}

	// 격려스킬 아이콘 및 설명 (공통)
	if( luaManager.BeginTable( "CHEER_SKILL_INFO" ) == true )
	{
		int index = 1;
		while( luaManager.BeginTable( index ) == true )
		{
			PetSkillInfo petSkillInfo;

			int iSkillDesc = STR_ID_27;
			LUA_GET_VALUE( luaManager, 1, petSkillInfo.m_wstrImageName,			L""	);
			LUA_GET_VALUE( luaManager, 2, petSkillInfo.m_wstrKeyName,				L""	);
#ifdef CHANGE_PET_STRING_ID //2013.07.08
			wstring wstrTemp = L"";
			LUA_GET_VALUE( luaManager, 3, wstrTemp,	L""	);
			petSkillInfo.m_wstrSkillDesc = wstrTemp;
			LUA_GET_VALUE( luaManager, 4, wstrTemp,	L""	);
			petSkillInfo.m_wstrSkillDesc += L"\n\n";
			petSkillInfo.m_wstrSkillDesc += wstrTemp;
			petSkillInfo.m_wstrSkillDesc += L"\n";
#else //CHANGE_PET_STRING_ID
			LUA_GET_VALUE( luaManager, 3, iSkillDesc,			STR_ID_27	);
			petSkillInfo.m_wstrSkillDesc = GET_STRING( iSkillDesc );
			LUA_GET_VALUE( luaManager, 4, iSkillDesc,			STR_ID_27	);
			petSkillInfo.m_wstrSkillDesc += L"\n\n";
			petSkillInfo.m_wstrSkillDesc += GET_STRING( iSkillDesc );
			petSkillInfo.m_wstrSkillDesc += L"\n";
#endif //CHANGE_PET_STRING_ID

			pPetTemplet->m_CheerSkill.push_back( petSkillInfo );
			luaManager.EndTable();

			index++;
		}
		luaManager.EndTable();
	}

#ifdef PET_AURA_SKILL

	if( luaManager.BeginTable( "AURA_SKILL_INFO" ) == true )
	{
		int index = 1;
		while( luaManager.BeginTable( index ) == true )
		{
			PetSkillInfo petSkillInfo;

			int iSkillDesc = STR_ID_27;
			LUA_GET_VALUE( luaManager, 1, petSkillInfo.m_wstrImageName,				L""	);
			LUA_GET_VALUE( luaManager, 2, petSkillInfo.m_wstrKeyName,				L""	);
#ifdef CHANGE_PET_STRING_ID //2013.07.08
			wstring wstrTemp = L"";
			LUA_GET_VALUE( luaManager, 3, wstrTemp,	L""	);
			petSkillInfo.m_wstrSkillDesc = wstrTemp;
#else //CHANGE_PET_STRING_ID
			LUA_GET_VALUE( luaManager, 3, iSkillDesc,				STR_ID_27	);
			petSkillInfo.m_wstrSkillDesc = GET_STRING( iSkillDesc );
#endif //CHANGE_PET_STRING_ID

			pPetTemplet->m_AuraSkill.push_back( petSkillInfo );
			luaManager.EndTable();

			index++;
		}
		luaManager.EndTable();
	}

	if( luaManager.BeginTable( "AURA_SKILL_1" ) == true )
	{
		int tableIndex = 1;
		int iAuraSillOptionId = 0;
		while( luaManager.GetValue( tableIndex, iAuraSillOptionId ) == true )
		{
			pPetTemplet->m_vecAuraSkillOption[0].push_back( iAuraSillOptionId );
			tableIndex++;
		}
		luaManager.EndTable();
	}
	if( luaManager.BeginTable( "AURA_SKILL_2" ) == true )
	{
		int tableIndex = 1;
		int iAuraSillOptionId = 0;
		while( luaManager.GetValue( tableIndex, iAuraSillOptionId ) == true )
		{
			pPetTemplet->m_vecAuraSkillOption[1].push_back( iAuraSillOptionId );
			tableIndex++;
		}
		luaManager.EndTable();
	}
	if( luaManager.BeginTable( "AURA_SKILL_3" ) == true )
	{
		int tableIndex = 1;
		int iAuraSillOptionId = 0;
		while( luaManager.GetValue( tableIndex, iAuraSillOptionId ) == true )
		{
			pPetTemplet->m_vecAuraSkillOption[2].push_back( iAuraSillOptionId );
			tableIndex++;
		}
		luaManager.EndTable();
	}
#endif
	
	if( luaManager.BeginTable( "SPECIAL_ACTION_COMMAND" ) == true )
	{
		int tableIndex = 1;
#ifdef GLOBAL_PET_COMMAND
		int iSpeicalActionCommand = STR_ID_27;
		while( luaManager.BeginTable( tableIndex ) == true )
		{
			LUA_GET_VALUE( luaManager, 1, iSpeicalActionCommand, STR_ID_27	);

			pPetTemplet->m_SpecialActionCommand.push_back(GET_STRING(iSpeicalActionCommand));
			luaManager.EndTable();

			tableIndex++;
		}
#else GLOBAL_PET_COMMAND
		wstring wstrScriptName = L"";
		while( luaManager.GetValue( tableIndex, wstrScriptName ) == true )
		{
			pPetTemplet->m_SpecialActionCommand.push_back( wstrScriptName );
			tableIndex++;
		}
#endif GLOBAL_PET_COMMAND
		luaManager.EndTable();
	}
	if( m_PetTemplet.find(pPetTemplet->m_PetId) != m_PetTemplet.end() )
	{
		SAFE_DELETE(pPetTemplet); 
		return false;
	}
	else
	{
		m_PetTemplet.insert( std::make_pair(pPetTemplet->m_PetId, pPetTemplet) );
	}

	return true;
}

CX2PetManager::PetTemplet *CX2PetManager::GetPetTemplet(PET_UNIT_ID pid)
{
	map<PET_UNIT_ID, PetTemplet*>::iterator mit;
	mit = m_PetTemplet.find( pid );
	if( mit == m_PetTemplet.end() )
		return NULL;

	PetTemplet* pPetTemplet = mit->second;

	return pPetTemplet;
}

int	CX2PetManager::GetPetInventorySlotSize( PET_UNIT_ID pid, char cEvolutionStep )
{
	PetTemplet *pPetTemplet = GetPetTemplet( pid );
	if( pPetTemplet == NULL )
	{
		return 0;
	}

	if( cEvolutionStep < 0  ||
		static_cast<u_int>(cEvolutionStep) >= pPetTemplet->m_InvenSlot_Width_Size.size()  ||
		static_cast<u_int>(cEvolutionStep) >= pPetTemplet->m_InvenSlot_Height_Size.size() )
	{
		return 0;
	}

	return ( pPetTemplet->m_InvenSlot_Width_Size[cEvolutionStep] * pPetTemplet->m_InvenSlot_Height_Size[cEvolutionStep] );
}


int	CX2PetManager::GetPetInventorySlotWidthSize( PET_UNIT_ID pid, char cEvolutionStep )
{
	PetTemplet *pPetTemplet = GetPetTemplet( pid );
	if( pPetTemplet == NULL )
	{
		return 0;
	}

	if( cEvolutionStep < 0  ||
		static_cast<u_int>(cEvolutionStep) >= pPetTemplet->m_InvenSlot_Width_Size.size() )
	{
		return 0;
	}

	return pPetTemplet->m_InvenSlot_Width_Size[cEvolutionStep];
}

int	CX2PetManager::GetPetInventorySlotHeightSize( PET_UNIT_ID pid, char cEvolutionStep )
{
	PetTemplet *pPetTemplet = GetPetTemplet( pid );
	if( pPetTemplet == NULL )
	{
		return 0;
	}

	if( cEvolutionStep < 0  ||
		static_cast<u_int>(cEvolutionStep) >= pPetTemplet->m_InvenSlot_Height_Size.size() )
	{
		return 0;
	}

	return pPetTemplet->m_InvenSlot_Height_Size[cEvolutionStep];
}


float CX2PetManager::GetIntimacyPercent( int petId, char lv, float fIntimacy )
{
	PetTemplet *pTemplet = GetPetTemplet( (PET_UNIT_ID)petId );
	if( pTemplet != NULL )
	{
		return fIntimacy / pTemplet->m_Evolution_Step_Max[lv];

	}

	return 0.f;
}


// {{ 김상훈 : 2010.10.07 : 캐릭터 우클릭 팝업메뉴 UI 개선 
#ifndef CHARACTER_MOUSE_RBUTTON
bool CX2PetManager::GetMyPetPick()
#else
bool CX2PetManager::GetMyPetPick( bool bOpenPopup )
#endif
{
	if( g_pMain->GetNowStateID() == CX2Main::XS_VILLAGE_MAP ||
		g_pMain->GetNowStateID() == CX2Main::XS_PVP_ROOM 
		|| g_pMain->GetNowStateID() == CX2Main::XS_BATTLE_FIELD
		|| g_pMain->GetNowStateID() == CX2Main::XS_DUNGEON_RESULT
		)
	{
		CX2PET *pMyPet = GetMyPet();
		if( pMyPet != NULL && g_pKTDXApp->GetDIManager()->GetMouse()->GetPureButtonState( MOUSE_RBUTTON ) == TRUE )
		{			
			if( pMyPet->GetShowObject() == true )
			{
				D3DXVECTOR3 pos = pMyPet->GetMatrix().GetPos(); //pMyPet->GetPos();
				pos.y += 10.0f;

				D3DXVECTOR3 startRay, endRay;
				g_pKTDXApp->Make2DPosToRay( g_pKTDXApp->GetDIManager()->GetMouse()->GetXPos(),
					g_pKTDXApp->GetDIManager()->GetMouse()->GetYPos(), 10000, startRay, endRay );

				// 팝업메뉴를 띄운다.
				if( g_pKTDXApp->GetCollision()->LineToSphere( startRay, endRay, 30.0f, pos,	pMyPet->GetBoundingRadius()/2.0f ) == true )
				{
#ifdef CHARACTER_MOUSE_RBUTTON
					if ( bOpenPopup )
#endif CHARACTER_MOUSE_RBUTTON
					{
						g_pData->GetUserMenu()->ClosePopupMenu();
						OpenPetPopupMenu();
					}

					return true;
				}	
			}
		}
	}

	return false;
}
// }} 김상훈 : 2010.10.07 : 캐릭터 우클릭 팝업메뉴 UI 개선 

#ifdef PRINT_INGAMEINFO_TO_EXCEL
void CX2PetManager::PrintPetInfo_ToExcel(void)
{
	BasicExcel e;
	e.New(3);

	BasicExcelWorksheet* pSheet0 = e.GetWorksheet((size_t)0);
	BasicExcelWorksheet* pSheet1 = e.GetWorksheet((size_t)1);
	BasicExcelWorksheet* pSheet2 = e.GetWorksheet((size_t)2);
	int cal = 1;

	// 0. Title 출력
	pSheet0->Cell(0,0)->SetWString(L"PET_ID");	
	pSheet0->Cell(0,1)->SetWString(L"PET_이름");	
	pSheet0->Cell(0,2)->SetWString(L"PET_CAPRICE(성향)");	
	pSheet0->Cell(0,3)->SetWString(L"PET_APPETITE");	
	pSheet0->Cell(0,4)->SetWString(L"PET_EVOLUTION_ALL_STEP");	
	pSheet0->Cell(0,5)->SetWString(L"SPECIAL_ACTION_COMMAND");	

	// 1 Sheet에 Title 출력
	pSheet1->Cell(0,0)->SetWString(L"PET_ID");	
	pSheet1->Cell(0,1)->SetWString(L"PET_STATUS");	
	pSheet1->Cell(0,2)->SetWString(L"WIDTH_SIZE");	
	pSheet1->Cell(0,3)->SetWString(L"HEIGHT_SIZE");	
	pSheet1->Cell(0,4)->SetWString(L"EVOLUTION_STEP_MAX");	
	pSheet1->Cell(0,5)->SetWString(L"SCRIPTNAME");	

	pSheet1->Cell(0,6)->SetWString(L"IMAGE_FILE");
	pSheet1->Cell(0,7)->SetWString(L"IMAGE_TET");

	pSheet1->Cell(0,8)->SetWString(L"GAME_IMAGE_FILE");
	pSheet1->Cell(0,9)->SetWString(L"GAME_IMAGE_TET");


	// 2 Sheet에 Title 출력
	pSheet2->Cell(0,0)->SetWString(L"PetID");	
	pSheet2->Cell(0,1)->SetWString(L"PetSkillType");	
	pSheet2->Cell(0,2)->SetWString(L"Number");	
	pSheet2->Cell(0,3)->SetWString(L"SkillDesc1");	
	pSheet2->Cell(0,4)->SetWString(L"SkillDesc2");	
	pSheet2->Cell(0,5)->SetWString(L"IMAGE_FILE");	
	pSheet2->Cell(0,6)->SetWString(L"IMAGE_TET");	

	pSheet2->Cell(0,7)->SetWString(L"AuraOption");


	// 내용 출력 
	int iStatusRow = 1;
	int iSkillRow = 1;
	map<PET_UNIT_ID, PetTemplet*>::iterator iter;
	for( iter = m_PetTemplet.begin(); iter != m_PetTemplet.end(); ++iter )
	{
		PetTemplet *pPetTemplet = iter->second;
		if(pSheet0 != NULL)
		{
			//{{ 일반 정보
			pSheet0->Cell(cal,0)->SetInteger( pPetTemplet->m_PetId );
			pSheet0->Cell(cal,1)->SetWString( pPetTemplet->m_wstrCommonName.c_str() );
			pSheet0->Cell(cal,2)->SetDouble( pPetTemplet->m_caprice );
			pSheet0->Cell(cal,3)->SetDouble( pPetTemplet->m_appetite );
			pSheet0->Cell(cal,4)->SetInteger( pPetTemplet->m_Evolution_All_Step );

			std::wstring wstrSpecialCommand = L"";
			for(std::vector<wstring>::iterator iter1 = pPetTemplet->m_SpecialActionCommand.begin();
				iter1 != pPetTemplet->m_SpecialActionCommand.end(); 
				iter1++)
			{
				if(iter1 != pPetTemplet->m_SpecialActionCommand.begin())
					wstrSpecialCommand += L", ";

				wstrSpecialCommand += (*iter1);
			}
			pSheet0->Cell(cal,5)->SetWString( wstrSpecialCommand.c_str() );
			//}}

			//{{ 진화 관련 정보
			if(pSheet1 != NULL)
			{
				int iSizeEvolution = (int)pPetTemplet->m_vecPetStatus.size();
				for(int iIdx = 0; iIdx < iSizeEvolution; iIdx++)
				{
					int iPetStatus = pPetTemplet->m_vecPetStatus[iIdx];

					pSheet1->Cell(iStatusRow, 0)->SetInteger(pPetTemplet->m_PetId);
					pSheet1->Cell(iStatusRow, 1)->SetInteger(iPetStatus);
					pSheet1->Cell(iStatusRow, 2)->SetInteger(pPetTemplet->m_InvenSlot_Width_Size[iIdx]);
					pSheet1->Cell(iStatusRow, 3)->SetInteger(pPetTemplet->m_InvenSlot_Height_Size[iIdx]);
					pSheet1->Cell(iStatusRow, 4)->SetDouble(pPetTemplet->m_Evolution_Step_Max[iIdx]);
					pSheet1->Cell(iStatusRow, 5)->SetWString(pPetTemplet->m_Evolution_Step_ScriptName[iIdx].c_str());

					pSheet1->Cell(iStatusRow, 6)->SetWString(pPetTemplet->m_Evolution_Step_Image[iIdx].m_wstrImageName.c_str());
					pSheet1->Cell(iStatusRow, 7)->SetWString(pPetTemplet->m_Evolution_Step_Image[iIdx].m_wstrKeyName.c_str());

					pSheet1->Cell(iStatusRow, 8)->SetWString(pPetTemplet->m_Evolution_Step_Game_Image[iIdx].m_wstrImageName.c_str());
					pSheet1->Cell(iStatusRow, 9)->SetWString(pPetTemplet->m_Evolution_Step_Game_Image[iIdx].m_wstrKeyName.c_str());

					++iStatusRow;
				}
			}
			//}}

			//{{ 펫 스킬 정보
			if(pSheet2 != NULL)
			{
				// Attack Skill
				int iSizeAttackSkill = (int)pPetTemplet->m_AttackSkill_Step.size();
				for(int iIdx = 0; iIdx < iSizeAttackSkill; iIdx++)
				{
					pSheet2->Cell(iSkillRow, 0)->SetInteger(pPetTemplet->m_PetId);
					pSheet2->Cell(iSkillRow, 1)->SetInteger(0); // Attack Skill : 0
					pSheet2->Cell(iSkillRow, 2)->SetInteger(iIdx); // SubIndex (여러스킬 존재시 구분용)

					// DESC 첫번째 개행으로 구분해서 스킬 Name과 Desc 구분해야 함 
					std::wstring wstrTmpDesc = pPetTemplet->m_AttackSkill_Step[iIdx].m_wstrSkillDesc;
					int iTmpIdx = wstrTmpDesc.find_first_of(L"\n", 0, wstrTmpDesc.size() -1);
					std::wstring wstrTmpName = wstrTmpDesc.substr(0, iTmpIdx);
					pSheet2->Cell(iSkillRow, 3)->SetWString(wstrTmpName.c_str());

					if(iTmpIdx >= 0 || iTmpIdx < wstrTmpDesc.size()-1)
					{
						wstrTmpDesc = wstrTmpDesc.substr(iTmpIdx+1, wstrTmpDesc.size()-1);
						pSheet2->Cell(iSkillRow, 4)->SetWString(wstrTmpDesc.c_str());
					}

					pSheet2->Cell(iSkillRow, 5)->SetWString(pPetTemplet->m_AttackSkill_Step[iIdx].m_wstrImageName.c_str());
					pSheet2->Cell(iSkillRow, 6)->SetWString(pPetTemplet->m_AttackSkill_Step[iIdx].m_wstrKeyName.c_str());

					++iSkillRow;
				}

				// Cheer Skill
				int iSizeCheerSkill = (int)pPetTemplet->m_CheerSkill.size();
				for(int iIdx = 0; iIdx < iSizeCheerSkill; iIdx++)
				{
					pSheet2->Cell(iSkillRow, 0)->SetInteger(pPetTemplet->m_PetId);
					pSheet2->Cell(iSkillRow, 1)->SetInteger(1); // Cheer Skill : 1
					pSheet2->Cell(iSkillRow, 2)->SetInteger(iIdx); // SubIndex (여러스킬 존재시 구분용)

					// DESC 첫번째 개행으로 구분해서 스킬 Name과 Desc 구분해야 함 
					std::wstring wstrTmpDesc = pPetTemplet->m_CheerSkill[iIdx].m_wstrSkillDesc;
					int iTmpIdx = wstrTmpDesc.find_first_of(L"\n", 0, wstrTmpDesc.size() -1);
					std::wstring wstrTmpName = wstrTmpDesc.substr(0, iTmpIdx);
					pSheet2->Cell(iSkillRow, 3)->SetWString(wstrTmpName.c_str());

					if(iTmpIdx >= 0 || iTmpIdx < wstrTmpDesc.size()-1)
					{
						wstrTmpDesc = wstrTmpDesc.substr(iTmpIdx+1, wstrTmpDesc.size()-1);
						pSheet2->Cell(iSkillRow, 4)->SetWString(wstrTmpDesc.c_str());
					}

					pSheet2->Cell(iSkillRow, 3)->SetWString(wstrTmpName.c_str());
					pSheet2->Cell(iSkillRow, 4)->SetWString(wstrTmpDesc.c_str());
					pSheet2->Cell(iSkillRow, 5)->SetWString(pPetTemplet->m_CheerSkill[iIdx].m_wstrImageName.c_str());
					pSheet2->Cell(iSkillRow, 6)->SetWString(pPetTemplet->m_CheerSkill[iIdx].m_wstrKeyName.c_str());

					++iSkillRow;
				}

				// Aura Skill
				int iSizeAuraSkill = (int)pPetTemplet->m_AuraSkill.size();
				for(int iIdx = 0; iIdx < iSizeAuraSkill; iIdx++)
				{
					pSheet2->Cell(iSkillRow, 0)->SetInteger(pPetTemplet->m_PetId);
					pSheet2->Cell(iSkillRow, 1)->SetInteger(2); // Aura Skill : 2
					pSheet2->Cell(iSkillRow, 2)->SetInteger(iIdx); // SubIndex (여러스킬 존재시 구분용)

					// DESC 첫번째 개행으로 구분해서 스킬 Name과 Desc 구분해야 함 
					std::wstring wstrTmpDesc = pPetTemplet->m_AuraSkill[iIdx].m_wstrSkillDesc;
					int iTmpIdx = wstrTmpDesc.find_first_of(L"\n", 0, wstrTmpDesc.size() -1);
					std::wstring wstrTmpName = wstrTmpDesc.substr(0, iTmpIdx);
					pSheet2->Cell(iSkillRow, 3)->SetWString(wstrTmpName.c_str());

					if(iTmpIdx >= 0 || iTmpIdx < wstrTmpDesc.size()-1)
					{
						wstrTmpDesc = wstrTmpDesc.substr(iTmpIdx+1, wstrTmpDesc.size()-1);
						pSheet2->Cell(iSkillRow, 4)->SetWString(wstrTmpDesc.c_str());
					}
					else
						wstrTmpDesc = L"";

					pSheet2->Cell(iSkillRow, 3)->SetWString(wstrTmpName.c_str());
					pSheet2->Cell(iSkillRow, 4)->SetWString(wstrTmpDesc.c_str());
					pSheet2->Cell(iSkillRow, 5)->SetWString(pPetTemplet->m_AuraSkill[iIdx].m_wstrImageName.c_str());
					pSheet2->Cell(iSkillRow, 6)->SetWString(pPetTemplet->m_AuraSkill[iIdx].m_wstrKeyName.c_str());

					std::wstring strAuraOption = L"";
					for(std::vector<int>::iterator vec_iter = pPetTemplet->m_vecAuraSkillOption[iIdx].begin();
						vec_iter != pPetTemplet->m_vecAuraSkillOption[iIdx].end();
						vec_iter++)
					{
						if(vec_iter != pPetTemplet->m_vecAuraSkillOption[iIdx].begin())
							strAuraOption += L", ";

						strAuraOption += boost::str( boost::wformat( L"%d" ) % (*vec_iter) );
					}

					pSheet2->Cell(iSkillRow, 7)->SetWString(strAuraOption.c_str());

					++iSkillRow;
				}
			}
			//}}

		}
		cal++;
	}
	e.SaveAs("PET_INFO.xls");

}
#endif PRINT_INGAMEINFO_TO_EXCEL

void CX2PetManager::ClosePetPopupMenu()
{
	if ( NULL != m_pDlgPopup )
		m_pDlgPopup->SetShowEnable(false, false);

	if( m_pDlgPetPopupMenu != NULL )
	{
		m_pDlgPetPopupMenu->SetShowEnable( false, false );
	}
}

void CX2PetManager::OpenPetPopupMenu()
{
	CX2PET *pPet = GetMyPet();

	if( pPet == NULL )
		return;
	
	CX2PetManager::PetTemplet *pTemplet = g_pData->GetPetManager()->GetPetTemplet( (CX2PetManager::PET_UNIT_ID)pPet->GetPetInfo().m_PetId );
	if( pTemplet == NULL )
		return;

	if( m_pDlgPetPopupMenu == NULL )
	{
		m_pDlgPetPopupMenu = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Pet_R_Menu_NEW.lua" );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDlgPetPopupMenu );
		m_pDlgPetPopupMenu->SetDisableUnderWindow(true);		
	}	

	// 팝업메뉴 띄울 위치 지정
	D3DXVECTOR2 vMousePos;
	vMousePos.x = (float)g_pKTDXApp->GetDIManager()->GetMouse()->GetXPos();
	vMousePos.y = (float)g_pKTDXApp->GetDIManager()->GetMouse()->GetYPos();
	vMousePos = g_pKTDXApp->MouseConvertByResolution( vMousePos );

	// 팝업 메뉴가 화면 밖으로 벗어나지 않게 수정	
	m_pDlgPetPopupMenu->SetPos( D3DXVECTOR2( (float)vMousePos.x-30.f, (float)vMousePos.y - 40.f ) );

	m_pDlgPopup->SetPos( D3DXVECTOR2( (float)vMousePos.x-30.f, (float)vMousePos.y - 40.f ) );
	m_pDlgPopup->SetShowEnable( true, true );
	m_pDlgPetPopupMenu->SetShowEnable(true, true);


	// 펫 아이콘
	CKTDGUIControl::CPictureData *pPictureIcon = NULL;
	CKTDGUIStatic *pStaticImg		=  (CKTDGUIStatic*)m_pDlgPetPopupMenu->GetControl( L"Pet_image" );			// 펫이름
	if( pStaticImg != NULL )
	{
		pPictureIcon = pStaticImg->GetPicture(0);
		CX2PetManager::PetStepImage petStepImage = pTemplet->m_Evolution_Step_Image[ pPet->GetPetInfo().m_Evolution_Step ];

		wstring wstrImageName = petStepImage.m_wstrImageName;
		if( petStepImage.m_wstrKeyName == L"" )
		{
			pPictureIcon->SetTex( wstrImageName.c_str() );
		}
		else
		{
			pPictureIcon->SetSize( D3DXVECTOR2( 20.f, 20.f ) );
			pPictureIcon->SetTex( wstrImageName.c_str(), petStepImage.m_wstrKeyName.c_str() );
		}
	}
	

	// 펫 이름
	CKTDGUIStatic *pStaticName		=  (CKTDGUIStatic*)m_pDlgPetPopupMenu->GetControl( L"pet_name" );			// 펫이름
	if( pStaticName != NULL )
		pStaticName->GetString(0)->msg = pPet->GetPetInfo().m_wstrPetName;


	// oasis907 : 김상윤 [2010.9.14] // 알 상태의 펫일 경우 소지품 버튼 지우고 먹이주기 버튼 위치 올리기
	int EvolutionStep = pPet->GetPetInfo().m_Evolution_Step;
	int MyPetID = pPet->GetPetInfo().m_PetId;
	int Invenslot_Size = g_pData->GetPetManager()->GetPetInventorySlotSize((CX2PetManager::PET_UNIT_ID) MyPetID, EvolutionStep);
		
	float fButtonWidth = 60.f;
	float fButtonHeight = 22.f;
	D3DXVECTOR2 offsetPos = D3DXVECTOR2(0.f, 0.f);

	CKTDGUIButton* pButtonFeeding = (CKTDGUIButton*)m_pDlgPetPopupMenu->GetControl( L"Pet_R_Feeding" );
	CKTDGUIButton* pButtonInfo = (CKTDGUIButton*)m_pDlgPetPopupMenu->GetControl( L"Pet_R_Basic_Info" );
	CKTDGUIButton* pButtonPropensity = (CKTDGUIButton*)m_pDlgPetPopupMenu->GetControl( L"Pet_R_Propensity" );
	CKTDGUIButton* pButtonInven = (CKTDGUIButton*)m_pDlgPetPopupMenu->GetControl( L"Pet_R_Inven" );
	CKTDGUIButton* pButtonCancel = (CKTDGUIButton*)m_pDlgPetPopupMenu->GetControl( L"Pet_R_Dissmiss" );
	CKTDGUIStatic* pStaticText = (CKTDGUIStatic*)m_pDlgPetPopupMenu->GetControl( L"PetPopUpGage" );
	CKTDGUIStatic* pStaticBar = (CKTDGUIStatic*)m_pDlgPetPopupMenu->GetControl( L"PetPopUp" );

	
	if ( NULL != pStaticImg )
		pStaticImg->SetShowEnable( true, true );
	if ( NULL != pStaticName )
		pStaticName->SetShowEnable( true, true );
	if ( NULL != pButtonFeeding )
		pButtonFeeding->SetShowEnable( true, true );
	if ( NULL != pButtonInfo )
		pButtonInfo->SetShowEnable( true, true );
	if ( NULL != pButtonPropensity )
		pButtonPropensity->SetShowEnable( true, true );
	if ( NULL != pButtonInven )
		pButtonInven->SetShowEnable( true, true );
	if ( NULL != pButtonCancel )
		pButtonCancel->SetShowEnable( true, true );

	if ( NULL != pStaticText )
	{
		pStaticText->SetShowEnable( true, true );
		pStaticText->GetString( 0 )->msg = GET_STRING( STR_ID_20841 );
		pStaticText->GetString( 1 )->msg = GET_STRING( STR_ID_5348 );
		pStaticText->GetString( 2 )->msg = GET_STRING( STR_ID_20842 );
		pStaticText->GetString( 3 )->msg = GET_STRING( STR_ID_20843 );
		pStaticText->GetString( 4 )->msg = GET_STRING( STR_ID_20844 );
	}

	if(Invenslot_Size == 0)
	{
		float fStringYPos = 0.f;

		if ( NULL != pButtonInven )
			pButtonInven->SetShowEnable( false, false );
		if ( NULL != pButtonCancel )
		{
			pButtonCancel->SetOffsetPos( D3DXVECTOR2( 0.f, -fButtonHeight ) );
			fStringYPos = pButtonCancel->GetPos().y;
		}
		if ( NULL != pStaticText )
		{
			pStaticText->GetString( 3 )->msg = L"";
			pStaticText->GetString( 4 )->pos.y = fStringYPos + 4.f;
		}
		if ( NULL != pStaticBar )
			pStaticBar->GetPicture( 4 )->SetShow( false );

		m_nMenuCount = 3;
	}
	else
	{
		float fStringYPos = 0.f;

		if ( NULL != pButtonCancel )
		{
			pButtonCancel->SetOffsetPos( D3DXVECTOR2( 0.f, 0.f ) );
			fStringYPos = pButtonCancel->GetPos().y;
		}
		if ( NULL != pStaticText )
		{
			pStaticText->GetString( 4 )->pos.y = fStringYPos + 4.f;;
		}
		if ( NULL != pStaticBar )
			//pStaticBar->GetPicture( 4 )->SetShow( true );

		m_nMenuCount = 4;
	}

	//}}

	m_pPicCenterTop->SetSizeX( fButtonWidth );
	m_pPicLeftMiddle->SetSizeY( m_nMenuCount * fButtonHeight );
	m_pPicCenterMiddle->SetSizeX( fButtonWidth );
	m_pPicCenterMiddle->SetSizeY( m_nMenuCount * fButtonHeight );
	m_pPicRightMiddle->SetSizeY( m_nMenuCount * fButtonHeight );
	m_pPicCenterBottom->SetSizeX( fButtonWidth );

	offsetPos = m_pPicCenterTop->GetPos();
	offsetPos.x += fButtonWidth;
	m_pPicRightTop->SetPos( offsetPos );

	offsetPos = m_pPicCenterMiddle->GetPos();
	offsetPos.x += fButtonWidth;
	m_pPicRightMiddle->SetPos( offsetPos );

	offsetPos = m_pPicLeftMiddle->GetPos();
	offsetPos.y += (m_nMenuCount * fButtonHeight);
	m_pPicLeftBottom->SetPos( offsetPos );

	offsetPos = m_pPicCenterMiddle->GetPos();
	offsetPos.y += (m_nMenuCount * fButtonHeight);
	m_pPicCenterBottom->SetPos( offsetPos );

	offsetPos = m_pPicCenterBottom->GetPos();
	offsetPos.x += fButtonWidth;
	m_pPicRightBottom->SetPos( offsetPos );
}

bool CX2PetManager::Handler_EGS_COMMANDS_FOR_PETS_REQ( CX2PET::PET_ACTION_COMMAND ePetAction )
{
	if ( g_pData->GetServerProtocol() == NULL || 
#ifdef  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
        g_pData->GetServerProtocol()->IsUserProxyValid() == false
#else   X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
		g_pData->GetServerProtocol()->GetUserProxy() == NULL
#endif  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
        )
		return false; 

	KEGS_COMMANDS_FOR_PETS_REQ kPacket;
	kPacket.m_cActionType = ePetAction;

	g_pData->GetServerProtocol()->SendPacket( EGS_COMMANDS_FOR_PETS_REQ, kPacket );

	return true;
}

bool CX2PetManager::Handler_EGS_COMMANDS_FOR_PETS_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_COMMANDS_FOR_PETS_ACK kEvent;
	DeSerialize( pBuff, &kEvent );
	
	if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
	{
		return true;
	}	

	return false;
}

bool CX2PetManager::Handler_EGS_PET_ACTION_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_PET_ACTION_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	PlayEmotion(kEvent.m_iUnitUID, (CX2PET::PET_ACTION_COMMAND)kEvent.m_cActionType);

	return true;
}

void CX2PetManager::Handler_EGS_SUMMON_PET_REQ( UidType petUid )
{
	if ( g_pData->GetServerProtocol() == NULL || 
#ifdef  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
        g_pData->GetServerProtocol()->IsUserProxyValid() == false
#else   X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
		g_pData->GetServerProtocol()->GetUserProxy() == NULL
#endif  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
        )
		return;  

	KEGS_SUMMON_PET_REQ kPacket;
	kPacket.m_iSummonPetUID = petUid;

	g_pData->GetServerProtocol()->SendPacket( EGS_SUMMON_PET_REQ, kPacket );

	return;
}

bool CX2PetManager::Handler_EGS_SUMMON_PET_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_SUMMON_PET_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
	{
		if( kEvent.m_kSummonedPetInfo.m_iPetUID == 0 )
		{
			// 소환해제
			if( g_pData != NULL && g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetSelectUnit() != NULL )
			{
				g_pData->GetMyUser()->GetSelectUnit()->ClearPetInfo();
				return true;
			}
		}

		if( g_pData != NULL && g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetSelectUnit() != NULL )
		{
			// 소환
			g_pData->GetMyUser()->GetSelectUnit()->SetFullPetInfo( kEvent.m_kSummonedPetInfo );			
			// 인벤토리 셋팅

			//{{ oasis907 : 김상윤 [2010.9.8] // 펫 인벤토리 셋팅 
			int EvolutionStep = kEvent.m_kSummonedPetInfo.m_cEvolutionStep;
#ifdef SERV_PETID_DATA_TYPE_CHANGE //2013.07.02
			int iInvenSlotSize = g_pData->GetPetManager()->GetPetInventorySlotSize((CX2PetManager::PET_UNIT_ID) kEvent.m_kSummonedPetInfo.m_iPetID, EvolutionStep);
#else //SERV_PETID_DATA_TYPE_CHANGE
			int iInvenSlotSize = g_pData->GetPetManager()->GetPetInventorySlotSize((CX2PetManager::PET_UNIT_ID) kEvent.m_kSummonedPetInfo.m_cPetID, EvolutionStep);
#endif //SERV_PETID_DATA_TYPE_CHANGE

			g_pData->GetMyUser()->GetSelectUnit()->AccessInventory().SetPetInventory( kEvent.m_vecPetInventorySlotInfo, iInvenSlotSize );
			//}}

			for(UINT i=0; i<m_vecNeverSummonPetList.size(); ++i)
			{
				if( kEvent.m_kSummonedPetInfo.m_iPetUID == m_vecNeverSummonPetList[i] )
				{
					m_vecNeverSummonPetList.erase( m_vecNeverSummonPetList.begin() + i );
					break;
				}
			}
			
			if( NULL != g_pX2Game && NULL != g_pX2Game->GetMyUnit() )
			{
				g_pX2Game->GetMyUnit()->UpdateSocketDataAndEnchantData();
				g_pX2Game->GetMyUnit()->UpdatePassiveAndActiveSkillState();
			}
			return true;
		}		
	}	

	return false;
}

bool CX2PetManager::Handler_EGS_SUMMON_PET_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_SUMMON_PET_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	const UidType uidMasterUser = kEvent.m_iUnitUID;

	if ( 0 > uidMasterUser )
		return true;

	bool bMyPet = false;	

	if( g_pData != NULL && g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetSelectUnit() != NULL )
	{
		if( uidMasterUser == g_pData->GetMyUser()->GetSelectUnit()->GetUID() )
		{
			bMyPet = true;
			SetSummonedPetMp( 0.0f );
		}
	}


	if( kEvent.m_vecPetInfo.size() <= 0 )
	{
		// 소환해제				
		RemovePet( uidMasterUser );

		if ( NULL != g_pX2Game )
		{
			CX2GUUser* pMasterUser = g_pX2Game->GetUserUnitByUID( uidMasterUser );
			if ( NULL != pMasterUser )
			{
				// 자신의 펫이 아니면 clear, 자신의 펫이면 ack에서 clear 했을 것임
				if ( false == bMyPet )
					pMasterUser->GetUnit()->ClearPetInfo();

				UpdateMasterUserSocketAndEnchantData( pMasterUser );
			}
		}

		if( true == bMyPet )
		{
			CX2UIManager* pUIManager = g_pData->GetUIManager();

			if ( NULL != pUIManager )
			{
				if ( NULL != pUIManager->GetUIPetList() )
					pUIManager->GetUIPetList()->UpdateView();

#ifdef SIMPLE_BUG_FIX
				CKTDGParticleSystem::CParticleEventSequenceHandle mhParticle = pUIManager->GetUIPetList()->GethParticle();
				if( mhParticle != INVALID_PARTICLE_SEQUENCE_HANDLE )
				{
					g_pData->GetUIMajorParticle()->DestroyInstanceHandle(mhParticle);
					mhParticle = INVALID_PARTICLE_SEQUENCE_HANDLE;

					pUIManager->GetUIPetList()->SethParticle(mhParticle);
				}
#endif SIMPLE_BUG_FIX

				CX2UIInventory* pUIInventory = pUIManager->GetUIInventory();

				if ( NULL != pUIInventory && true == pUIInventory->GetShow() )
				{
					pUIInventory->UpdateUIButton();
					
					//{{ oasis907 : 김상윤 [2010.9.8] // 펫 인벤토리 열려있는 상황에서 펫 소환해제시 펫 인벤창 닫음
					CX2UIPetInventory* pPetInventory = pUIInventory->GetUIPetInventory();
					if ( NULL != pPetInventory && true == pPetInventory->GetShow() )
					{
						pPetInventory->SetShow( false );

						if ( NULL != pUIInventory->GetUISocketItem() )
							pUIInventory->GetUISocketItem()->SetShow( false );
					}
					//}}

#ifdef SERV_HALLOWEEN_PUMPKIN_FAIRY_PET
					if( uidMasterUser == g_pData->GetMyUser()->GetSelectUnit()->GetUID() )
					{
						pUIInventory->UseTransformPetItem();
					}						
#endif //SERV_HALLOWEEN_PUMPKIN_FAIRY_PET
				}				

		#ifdef PET_AURA_SKILL	
				if ( true == pUIManager->GetShow( CX2UIManager::UI_MENU_CHARINFO ) )
					pUIManager->GetUICharInfo()->ResetStat();
		#endif
			}
		}
	}
	else	// 소환
	{
		const KPetInfo& pPetInfo = kEvent.m_vecPetInfo[0];
		CX2PET::PetInfo petInfo;

		petInfo.m_PetUid = pPetInfo.m_iPetUID;
#ifdef SERV_PETID_DATA_TYPE_CHANGE //2013.07.02
		petInfo.m_PetId = pPetInfo.m_iPetID;
#else //SERV_PETID_DATA_TYPE_CHANGE
		petInfo.m_PetId = pPetInfo.m_cPetID;
#endif //SERV_PETID_DATA_TYPE_CHANGE
		petInfo.m_Evolution_Step = pPetInfo.m_cEvolutionStep;
		petInfo.m_wstrPetName = pPetInfo.m_wstrPetName;		
		petInfo.m_Intimacy = (float)pPetInfo.m_iIntimacy;
		petInfo.m_Satiety = pPetInfo.m_sSatiety;
		petInfo.m_Extroversion = pPetInfo.m_sExtroversion;		// 외향성
		petInfo.m_Emotion = pPetInfo.m_sEmotion;				// 감성
#ifdef PET_DROP_ITEM_PICKUP
		petInfo.m_bIsDropItemPickup = pPetInfo.m_bAutoLooting;
#endif //PET_DROP_ITEM_PICKUP
#ifdef SERV_PET_SYSTEM_EX1
		petInfo.m_bAlwaysMaxSatiety = pPetInfo.m_bAlwaysMaxSatiety;
#endif //SERV_PET_SYSTEM_EX1
		petInfo.m_bSummon = true;
#ifdef SERV_EVENT_PET_INVENTORY		
		//여기서 이벤트 펫 인지 확인한다.
		if( kEvent.m_vecPetInfo[0].m_bIsEventPetID ) //이벤트 펫이 맞다.
		{			
			petInfo.m_bEventFoodEat = kEvent.m_vecPetInfo[0].m_bEventFoodEat;
			petInfo.m_bIsEventPetID = kEvent.m_vecPetInfo[0].m_bIsEventPetID;
		}		
#endif SERV_EVENT_PET_INVENTORY
		bool InitFlag = false;
		D3DXVECTOR3 vInitPosBackup = D3DXVECTOR3(0.f, 0.f, 0.f);
		CX2PET *pPet = GetPet( uidMasterUser );
		if( pPet != NULL )
		{
			vInitPosBackup = pPet->GetPos();
			InitFlag = true;
		}


		if( g_pX2Game != NULL )
		{
			// 소환시 오오라 효과 적용이 되기까지의 딜레이 타임 셋팅
			CX2PETPtr ptrPet = CreateGamePet( uidMasterUser, petInfo );
			if ( NULL != ptrPet )
				ptrPet->SetAuraSuspendedTargetTime( 10.0f );	// 소환 후 10초 후 오오라 적용

			CX2GUUser* pMasterUser = g_pX2Game->GetUserUnitByUID( uidMasterUser );
			if ( NULL != pMasterUser )
			{
				// 자신의 펫이면 ack에서 Set 했을 것임
				if ( false == bMyPet )
					pMasterUser->GetUnit()->SetFullPetInfo( pPetInfo );

				
				//UpdateMasterUserSocketAndEnchantData( pMasterUser );

				// 오현빈 // 2013-10-31 // 펫에 의한 공격력/방어력 증가분이 동기화 되지 않는 문제 수정
				pMasterUser->UpdateSocketDataAndEnchantData();
				pMasterUser->UpdatePassiveAndActiveSkillState();
			}
		}
		else 
		{
			CreatePet( uidMasterUser, petInfo, InitFlag, vInitPosBackup );

			if ( true == bMyPet )
			{
#ifdef PET_AURA_SKILL
				if( g_pChatBox != NULL )
				{
					float fNewSatiety = petInfo.m_Satiety;

					// 포만도 상태 
					wstring wstrSatiety = L"";
					if( petInfo.m_Satiety >= 0 && petInfo.m_Satiety <= 960 )
					{
						// 굶주림
						wstrSatiety += GET_STRING( STR_ID_9643 );
					}
					else if( petInfo.m_Satiety > 960 && petInfo.m_Satiety <= 1920 )
					{
						// 배고픔
						wstrSatiety += GET_STRING( STR_ID_9637 );
					}
					else if( petInfo.m_Satiety > 1920 && petInfo.m_Satiety <= 2880 )
					{
						// 허전함
						wstrSatiety += GET_STRING( STR_ID_9638 );
					}
					else if( petInfo.m_Satiety > 2880 && petInfo.m_Satiety <= 3840 )
					{
						// 만족함
						wstrSatiety += GET_STRING( STR_ID_9639 );
					}
					else
					{
						// 가득참
						wstrSatiety += GET_STRING( STR_ID_9640 );
					}

					wstring wstrBuff = L"";
					if( fNewSatiety / 4800.f >= 0.4f )
					{
						wstrBuff = GET_REPLACED_STRING( ( STR_ID_9642, "L", wstrSatiety ) );
					}
					else
					{
						wstrBuff = GET_REPLACED_STRING( ( STR_ID_9641, "L", wstrSatiety ) );				
					}
					g_pChatBox->AddChatLog( wstrBuff.c_str(), KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(0.92549f, 0.92549f, 0.53333f, 1.f), L"#CECEC88" );
				}
#endif
			}
		}

		if( bMyPet == true )
		{
			if( g_pData != NULL && g_pData->GetUIManager() != NULL && g_pData->GetUIManager()->GetUIPetList() != NULL )
			{
				g_pData->GetUIManager()->GetUIPetList()->UpdateView();
			}

			if( g_pData->GetUIManager()->GetUIInventory() != NULL &&
				g_pData->GetUIManager()->GetUIInventory()->GetShow() == true )
				g_pData->GetUIManager()->GetUIInventory()->UpdateUIButton();


			//{{ oasis907 : 김상윤 [2010.9.8] // 펫 인벤토리 열려있는 상황에서 펫 소환시, 펫 인벤창 갱신
			if( g_pData->GetUIManager()->GetShow( CX2UIManager::UI_MENU_INVEN ) == true &&
				g_pData->GetUIManager()->GetUIInventory()->GetUIPetInventory() != NULL &&
				g_pData->GetUIManager()->GetUIInventory()->GetUIPetInventory()->GetShow() == true )
			{
				if(g_pData->GetUIManager()->GetUIInventory()->GetUISocketItem() != NULL)
				{
					g_pData->GetUIManager()->GetUIInventory()->GetUISocketItem()->SetShow(false);
				}

				g_pData->GetUIManager()->GetUIInventory()->OpenPetInventory();
			}
			//}}

			// oasis907 : 김상윤 [2010.9.8] // 자동 먹이 주기 기능
			if( g_pData->GetUIManager()->GetUIInventory() != NULL)
			{
				g_pData->GetUIManager()->GetUIInventory()->SetAutoFeed(pPetInfo.m_bAutoFeed);
			}
			//}}
		}	
	}
	

	return true;
}

void CX2PetManager::Handler_EGS_PET_EVOLUTION_REQ( UidType petUid )
{
	if( petUid <= 0 )
		return;

	if ( g_pData->GetServerProtocol() == NULL || 
#ifdef  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
        g_pData->GetServerProtocol()->IsUserProxyValid() == false
#else   X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
		g_pData->GetServerProtocol()->GetUserProxy() == NULL
#endif  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
        )
		return;  

	KEGS_PET_EVOLUTION_REQ kPacket;
	kPacket.m_iPetUID = petUid;

	g_pData->GetServerProtocol()->SendPacket( EGS_PET_EVOLUTION_REQ, kPacket );

	return;
}

bool CX2PetManager::Handler_EGS_PET_EVOLUTION_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_PET_EVOLUTION_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
	{		
		bool bMyPet = false;
		// 내 펫 진화성공
		if( GetMyPet() == NULL ||
			( GetMyPet() != NULL && GetMyPet()->GetPetInfo().m_PetUid == kEvent.m_kEvolutionResultPetInfo.m_iPetUID ) )
		{
			bMyPet = true;
			g_pData->GetMyUser()->GetSelectUnit()->SetFullPetInfo( kEvent.m_kEvolutionResultPetInfo );
		}

		for(UINT i=0; i<m_vecMyPetEvolutionList.size(); ++i)
		{
			if( m_vecMyPetEvolutionList[i] == kEvent.m_kEvolutionResultPetInfo.m_iPetUID )
			{
				m_vecMyPetEvolutionList.erase( m_vecMyPetEvolutionList.begin() + i );
				break;
			}
		}

		if( g_pData->GetUIManager()->GetUIPetList() != NULL )
		{
			g_pData->GetUIManager()->GetUIPetList()->SetPetInfo( kEvent.m_kEvolutionResultPetInfo );
			if( bMyPet == true )
			{
				g_pData->GetUIManager()->GetUIPetList()->UpdateView();
			}
			
			
		}


		// oasis907 : 김상윤 [2010.9.17] // 현재 소환되어 있는 펫을 진화시켰을 경우에만 펫인벤 확장
		if( GetMyPet() != NULL && GetMyPet()->GetPetInfo().m_PetUid == kEvent.m_kEvolutionResultPetInfo.m_iPetUID )
		{
			// oasis907 : 김상윤 [2010.9.8] // 펫 인벤토리 확장
			CX2Inventory& kInventory = g_pData->GetMyUser()->GetSelectUnit()->AccessInventory();
			{
				KPetInfo kPetInfo = kEvent.m_kEvolutionResultPetInfo;
				int EvolutionStep = kPetInfo.m_cEvolutionStep;
#ifdef SERV_PETID_DATA_TYPE_CHANGE //2013.07.02
				int iInvenSlotSize = g_pData->GetPetManager()->GetPetInventorySlotSize((CX2PetManager::PET_UNIT_ID) kPetInfo.m_iPetID, EvolutionStep);
#else //SERV_PETID_DATA_TYPE_CHANGE
				int iInvenSlotSize = g_pData->GetPetManager()->GetPetInventorySlotSize((CX2PetManager::PET_UNIT_ID) kPetInfo.m_cPetID, EvolutionStep);
#endif //SERV_PETID_DATA_TYPE_CHANGE

				kInventory.SetItemMaxNum(CX2Inventory::ST_PET, iInvenSlotSize);
			}
			//}}
		}
		//}}
		

		return true;
	}	

	return false;
}

bool CX2PetManager::Handler_EGS_PET_EVOLUTION_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_PET_EVOLUTION_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	UidType iMasterUid = kEvent.m_iUnitUID;	

	//if( iMasterUid == g_pData->GetMyUser()->GetSelectUnit()->GetUID() )
	//	return true;

	CX2PET *pPet = GetPet(iMasterUid);
	if( pPet != NULL )
	{
		CX2PET::PetInfo petInfo;

		petInfo.m_PetUid = kEvent.m_kPetInfo.m_iPetUID;
#ifdef SERV_PETID_DATA_TYPE_CHANGE //2013.07.02
		petInfo.m_PetId = kEvent.m_kPetInfo.m_iPetID;
#else //SERV_PETID_DATA_TYPE_CHANGE
		petInfo.m_PetId = kEvent.m_kPetInfo.m_cPetID;
#endif //SERV_PETID_DATA_TYPE_CHANGE
		petInfo.m_Evolution_Step = kEvent.m_kPetInfo.m_cEvolutionStep;
		petInfo.m_wstrPetName = kEvent.m_kPetInfo.m_wstrPetName;		
		petInfo.m_Intimacy = (float)kEvent.m_kPetInfo.m_iIntimacy;
		petInfo.m_Satiety = kEvent.m_kPetInfo.m_sSatiety;
		petInfo.m_Extroversion = kEvent.m_kPetInfo.m_sExtroversion;
		petInfo.m_Emotion = kEvent.m_kPetInfo.m_sEmotion;
#ifdef PET_DROP_ITEM_PICKUP
		petInfo.m_bIsDropItemPickup = kEvent.m_kPetInfo.m_bAutoLooting;
#endif //PET_DROP_ITEM_PICKUP
#ifdef SERV_PET_SYSTEM_EX1
		petInfo.m_bAlwaysMaxSatiety = kEvent.m_kPetInfo.m_bAlwaysMaxSatiety;
#endif //SERV_PET_SYSTEM_EX1
		petInfo.m_bSummon = true;

		D3DXVECTOR3 vInitPosBackup = pPet->GetPos();

		RemovePet( iMasterUid );

		if( g_pTFieldGame != NULL || g_pX2Game != NULL )
			g_pData->GetPetManager()->CreatePet( iMasterUid, petInfo, true, vInitPosBackup );

		if( g_pX2Room != NULL )
		{
			CX2Room::SlotData *pSlotData = g_pX2Room->GetSlotDataByUnitUID( iMasterUid );
			if( pSlotData != NULL )
			{
				if( pSlotData->m_pUnit != NULL )
				{
					pSlotData->m_pUnit->SetFullPetInfo( kEvent.m_kPetInfo );
				}
				if( pSlotData->m_pUnitViewer != NULL && pSlotData->m_pUnitViewer->GetUnit() != NULL )
				{
					pSlotData->m_pUnitViewer->GetUnit()->SetFullPetInfo( kEvent.m_kPetInfo );
				}
			}
		}
	}

	CX2PET *pEvolutionPet = GetPet(iMasterUid);
	if( pEvolutionPet != NULL )
		pEvolutionPet->SetEvolution(true);

	if( g_pData != NULL && g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetSelectUnit() != NULL )
	{
		//{{ oasis907 : 김상윤 [2010.9.8] // 펫 인벤토리 열려있는 상황에서 펫 확장 실시간 갱신
		if( iMasterUid == g_pData->GetMyUser()->GetSelectUnit()->GetUID() )
		{
			if( g_pData->GetUIManager()->GetShow( CX2UIManager::UI_MENU_INVEN ) == true &&
				g_pData->GetUIManager()->GetUIInventory()->GetUIPetInventory() != NULL &&
				g_pData->GetUIManager()->GetUIInventory()->GetUIPetInventory()->GetShow() == true )
			{
				g_pData->GetUIManager()->GetUIInventory()->OpenPetInventory();
			}
		}
		//}}

#ifdef PET_AURA_SKILL
		if( iMasterUid == g_pData->GetMyUser()->GetSelectUnit()->GetUID() )
		{
			if( g_pData != NULL && 
				g_pData->GetUIManager() != NULL && 
				g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_CHARINFO))
				g_pData->GetUIManager()->GetUICharInfo()->ResetStat();
		}
#endif
	}

	return true;
}

bool CX2PetManager::Handler_EGS_CHANGED_PET_SATIETY_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// 포만도 변경
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHANGED_PET_SATIETY_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
	
	return ChangeSatiety( kEvent.m_iUnitUID, kEvent.m_iPetUID, kEvent.m_sCurrentSatiety );
}

bool CX2PetManager::Handler_EGS_CHANGED_PET_INTIMACY_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// 친밀도 변경
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHANGED_PET_INTIMACY_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
	
	return ChangeIntimacy( kEvent.m_iUnitUID, kEvent.m_iPetUID, kEvent.m_iCurrentIntimacy );
}

bool CX2PetManager::ChangeSatiety(UidType iUnitUID, UidType iPetUID, short sCurrentSatiety )
{
	UidType iMasterUid = iUnitUID;
	UidType iPetUid = iPetUID;	
	bool bMyPet = false;
	
	if( g_pData != NULL && g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetSelectUnit() != NULL )
#ifdef PET_CHANGESATIETY_NULL_CHECK
	if( NULL != g_pData->GetMyUser()->GetSelectUnit()->GetPetInfo())
#endif //PET_CHANGESATIETY_NULL_CHECK
	{
		if( iMasterUid == g_pData->GetMyUser()->GetSelectUnit()->GetUID() )
			bMyPet = true;

		CX2PET *pPet = GetPet( iMasterUid );
		if( pPet != NULL && pPet->GetPetInfo().m_PetUid == iPetUid )
		{		
#ifdef SERV_PET_SYSTEM_EX1
			if( true == pPet->GetPetInfo().m_bAlwaysMaxSatiety )
				return true;
#endif //SERV_PET_SYSTEM_EX1

			float fOldSatiety = pPet->GetPetInfo().m_Satiety;
			pPet->GetPetInfo().m_Satiety = (float)sCurrentSatiety;

			if( bMyPet == true )
			{
				g_pData->GetMyUser()->GetSelectUnit()->GetPetInfo()->m_sSatiety = sCurrentSatiety;
				if( g_pData->GetUIManager()->GetUIPetList() != NULL )
				{
					g_pData->GetUIManager()->GetUIPetList()->ChangeInfo();
				}

				if( g_pChatBox != NULL )
				{
					float fNewSatiety = pPet->GetPetInfo().m_Satiety;
					int iNewSatiety = (int)(fNewSatiety / 48);
					float fSatietyGap = (pPet->GetPetInfo().m_Satiety - fOldSatiety) / 48.f;

					if( fSatietyGap > 0.f )
					{
						wstring wstrBuff = GET_REPLACED_STRING( ( STR_ID_6542, "Lfi", pPet->GetPetInfo().m_wstrPetName, fSatietyGap, iNewSatiety ) );
						g_pChatBox->AddChatLog( wstrBuff.c_str(), KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(0.92549f, 0.92549f, 0.53333f, 1.f), L"#CECEC88" );
					}				
				}
			}
			else
			{
				if( g_pX2Room != NULL )
				{
					CX2Room::SlotData *pSlotData = g_pX2Room->GetSlotDataByUnitUID( iMasterUid );
					if( pSlotData != NULL )
					{
						if( pSlotData->m_pUnit != NULL && pSlotData->m_pUnit->GetPetInfo() != NULL )
						{
							pSlotData->m_pUnit->GetPetInfo()->m_sSatiety = sCurrentSatiety;
						}
						if( pSlotData->m_pUnitViewer != NULL && 
							pSlotData->m_pUnitViewer->GetUnit() != NULL &&
							pSlotData->m_pUnitViewer->GetUnit()->GetPetInfo() != NULL )
						{
							pSlotData->m_pUnitViewer->GetUnit()->GetPetInfo()->m_sSatiety = sCurrentSatiety;
						}
					}
				}
			}

	#ifdef PET_AURA_SKILL
			// 포만도 40%이상이면 기운스킬 적용시키도록 한다.
			bool bChangeAura = false;
			if ( g_pX2Game != NULL && g_pMain != NULL )
			{
				switch ( g_pMain->GetNowStateID() )
				{
				case CX2Main::XS_DUNGEON_GAME:
				case CX2Main::XS_TRAINING_GAME:
				case CX2Main::XS_BATTLE_FIELD:
					{
						CX2GUUser *pUser = g_pX2Game->GetUserUnitByUID( iUnitUID );
						if ( pUser != NULL )
						{
							bool bApply = false;				
							if( (float)sCurrentSatiety / 4800.f >= 0.4f )
								bApply = true; 

							if( bApply != pUser->GetApplyPetAura() )
							{
								bChangeAura = true;

								pUser->UpdateSocketDataAndEnchantData();
								pUser->UpdatePassiveAndActiveSkillState();

								pUser->ResetMaxMP();
								if( pUser->GetNowMp() > pUser->GetMaxMp() )
								{
									pUser->SetNowMp( pUser->GetMaxMp() );
								}

								if( g_pChatBox != NULL )
								{
									float fNewSatiety = (float)sCurrentSatiety;

									// 포만도 상태 
									wstring wstrSatiety = L"";
									if( sCurrentSatiety >= 0 && sCurrentSatiety <= 960 )
									{
										// 굶주림
										wstrSatiety += GET_STRING( STR_ID_9643 );
									}
									else if( sCurrentSatiety > 960 && sCurrentSatiety <= 1920 )
									{
										// 배고픔
										wstrSatiety += GET_STRING( STR_ID_9637 );
									}
									else if( sCurrentSatiety > 1920 && sCurrentSatiety <= 2880 )
									{
										// 허전함
										wstrSatiety += GET_STRING( STR_ID_9638 );
									}
									else if( sCurrentSatiety > 2880 && sCurrentSatiety <= 3840 )
									{
										// 만족함
										wstrSatiety += GET_STRING( STR_ID_9639 );
									}
									else
									{
										// 가득참
										wstrSatiety += GET_STRING( STR_ID_9640 );
									}

									if( bMyPet == true )
									{
										if( fNewSatiety / 4800.f >= 0.4f )
										{
											wstring wstrBuff = GET_REPLACED_STRING( ( STR_ID_9642, "L", wstrSatiety ) );
											g_pChatBox->AddChatLog( wstrBuff.c_str(), KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(0.92549f, 0.92549f, 0.53333f, 1.f), L"#CECEC88" );
										}
										else if( fNewSatiety / 4800.f < 0.4f )
										{
											wstring wstrBuff = GET_REPLACED_STRING( ( STR_ID_9641, "L", wstrSatiety ) );
											g_pChatBox->AddChatLog( wstrBuff.c_str(), KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(0.92549f, 0.92549f, 0.53333f, 1.f), L"#CECEC88" );
										}
									}
								}
							}

							pUser->SetApplyPetAura( bApply );
						}
					} break;

				default:
					break;
				}	// switch	
			}	

			if( bMyPet == true &&
				bChangeAura == true && 
				g_pData != NULL && 
				g_pData->GetUIManager() != NULL && 
				g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_CHARINFO))
				g_pData->GetUIManager()->GetUICharInfo()->ResetStat();
	#endif

			return true;
		}
	}

	return false;
}

bool CX2PetManager::ChangeIntimacy(UidType iUnitUID, UidType iPetUID, int iCurrentIntimacy )
{
	UidType iMasterUid = iUnitUID;
	UidType iPetUid = iPetUID;	
	bool bMyPet = false;

	if( g_pData != NULL && g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetSelectUnit() != NULL &&
		g_pData->GetMyUser()->GetSelectUnit()->GetPetInfo() != NULL )
	{
		if( iMasterUid == g_pData->GetMyUser()->GetSelectUnit()->GetUID() )
			bMyPet = true;

		CX2PET *pPet = GetPet( iMasterUid );
		if( pPet != NULL && pPet->GetPetInfo().m_PetUid == iPetUid )
		{
			pPet->GetPetInfo().m_Intimacy = (float)iCurrentIntimacy;

			if( bMyPet == true )
			{
	#ifdef NOTIFY_PET_INFO
				int iOldIntimacy = g_pData->GetMyUser()->GetSelectUnit()->GetPetInfo()->m_iIntimacy;
				//감소 되었다면 친밀도 감소 알림
				if( iOldIntimacy > iCurrentIntimacy ) 
				{  
					int iIntimacyGap	= iOldIntimacy - iCurrentIntimacy ;
					int iCurrentSatiety	= g_pData->GetMyUser()->GetSelectUnit()->GetPetInfo()->m_sSatiety;

					if( g_pChatBox != NULL )
					{ 
						// 포만도 상태 
						wstring wstrSatiety = L"";
						if( iCurrentSatiety >= 0 && iCurrentSatiety <= 960 )
						{
							// 굶주림
							wstrSatiety += GET_STRING( STR_ID_9643 );
						}
						else if( iCurrentSatiety > 960 && iCurrentSatiety <= 1920 )
						{
							// 배고픔
							wstrSatiety += GET_STRING( STR_ID_9637 );
						}
						else if( iCurrentSatiety > 1920 && iCurrentSatiety <= 2880 )
						{
							// 허전함
							wstrSatiety += GET_STRING( STR_ID_9638 );
						}
						else if( iCurrentSatiety > 2880 && iCurrentSatiety <= 3840 )
						{
							// 만족함
							wstrSatiety += GET_STRING( STR_ID_9639 );
						}
						else
						{
							// 가득참
							wstrSatiety += GET_STRING( STR_ID_9640 );
						}

						CX2PetManager::PetTemplet *pTemplet = GetPetTemplet( (PET_UNIT_ID)pPet->GetPetInfo().m_PetId ); 

						//진화단계 확인
						int iEvolutionStep =  pPet->GetPetInfo().m_Evolution_Step;
						//진화단계 별 최대 친밀도 값 확인
						float fMaxIntimacy = pTemplet->m_Evolution_Step_Max[iEvolutionStep];
						//감소된 친밀도를 백분율로 변환
						float fIntimacyGapPercent = (float)(iIntimacyGap /fMaxIntimacy) * 100.f;  

						wstring wstrBuff = GET_REPLACED_STRING( ( STR_ID_16157, "LfL", pPet->GetPetInfo().m_wstrPetName, fIntimacyGapPercent, wstrSatiety) );
						g_pChatBox->AddChatLog( wstrBuff.c_str(), KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
					} 
				}
	#endif NOTIFY_PET_INFO
				g_pData->GetMyUser()->GetSelectUnit()->GetPetInfo()->m_iIntimacy = iCurrentIntimacy;			

				if( g_pData->GetUIManager()->GetUIPetList() != NULL )
				{
					g_pData->GetUIManager()->GetUIPetList()->ChangeInfo();
				}

				// 진화가능 리스트 갱신
				PetTemplet *pTemplet = GetPetTemplet( (PET_UNIT_ID)pPet->GetPetInfo().m_PetId );
				if( pTemplet == NULL )
					return true;

				float fNowIntimacy = GetIntimacyPercent( pPet->GetPetInfo().m_PetId, pPet->GetPetInfo().m_Evolution_Step, pPet->GetPetInfo().m_Intimacy );
				if( fNowIntimacy >= 1.f &&  pPet->GetPetInfo().m_Evolution_Step < pTemplet->m_Evolution_All_Step )
				{
					bool bFind = false;
					for(UINT i=0; i<m_vecMyPetEvolutionList.size(); ++i)
					{
						if( m_vecMyPetEvolutionList[i] == iPetUid )
						{
							bFind = true;
							break;
						}
					}

					if( bFind == false )
					{
						m_vecMyPetEvolutionList.push_back(iPetUid);
					}
				}
				else
				{
					for(UINT i=0; i<m_vecMyPetEvolutionList.size(); ++i)
					{
						if( m_vecMyPetEvolutionList[i] == iPetUid )
						{
							m_vecMyPetEvolutionList.erase( m_vecMyPetEvolutionList.begin() + i );
							break;
						}
					}
				}
			}
			else
			{
				if( g_pX2Room != NULL )
				{
					CX2Room::SlotData *pSlotData = g_pX2Room->GetSlotDataByUnitUID( iMasterUid );
					if( pSlotData != NULL )
					{
						if( pSlotData->m_pUnit != NULL && pSlotData->m_pUnit->GetPetInfo() != NULL )
						{
							pSlotData->m_pUnit->GetPetInfo()->m_iIntimacy = iCurrentIntimacy;
						}
						if( pSlotData->m_pUnitViewer != NULL && 
							pSlotData->m_pUnitViewer->GetUnit() != NULL &&
							pSlotData->m_pUnitViewer->GetUnit()->GetPetInfo() != NULL )
						{
							pSlotData->m_pUnitViewer->GetUnit()->GetPetInfo()->m_iIntimacy = iCurrentIntimacy;
						}
					}
				}
			}

			return true;
		}
	}

	return false;
}

bool CX2PetManager::Handler_EGS_CHANGED_PET_SATIETY_INTIMACY_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// 포만도 변경
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHANGED_PET_SATIETY_INTIMACY_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	bool bResultSatiety = true;
	bool bResultIntimacy = true;

	bResultSatiety = ChangeSatiety( kEvent.m_iUnitUID, kEvent.m_iPetUID, kEvent.m_sCurrentSatiety );
	bResultIntimacy = ChangeIntimacy( kEvent.m_iUnitUID, kEvent.m_iPetUID, kEvent.m_iCurrentIntimacy );

	if( bResultSatiety == false || bResultIntimacy == false )
		return false;

	return true;
}

#ifdef CHANGE_PET_NAME 
void CX2PetManager::Handler_EGS_CHANGE_PET_NAME_REQ( UidType itemUid, wstring wstrPetName )
{
	if ( g_pData->GetServerProtocol() == NULL || 
#ifdef  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
        g_pData->GetServerProtocol()->IsUserProxyValid() == false
#else   X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
		g_pData->GetServerProtocol()->GetUserProxy() == NULL
#endif  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
        )
		return;  

	KEGS_CREATE_PET_REQ kPacket;
	kPacket.m_iItemUID = itemUid;
	kPacket.m_wstrPetName = wstrPetName;

	g_pData->GetServerProtocol()->SendPacket( EGS_CHANGE_PET_NAME_REQ, kPacket ); 

	return;
}

bool CX2PetManager::Handler_EGS_CHANGE_PET_NAME_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CREATE_PET_ACK kEvent;
	DeSerialize( pBuff, &kEvent );
	
	if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
	{		 
		g_pData->GetMyUser()->GetSelectUnit()->AccessInventory().UpdateInventorySlotList( kEvent.m_vecKInventorySlotInfo );
		// 켜져있는지 확인 후 업데이트 
		if( g_pData->GetUIManager()->GetShow( CX2UIManager::UI_MENU_INVEN ) == true )
		{
			g_pData->GetUIManager()->GetUIInventory()->UpdateInventorySlotList( kEvent.m_vecKInventorySlotInfo );
		} 
		//펫이름 변경 메세지박스 필요.
		wstring wstrComment = GET_REPLACED_STRING( ( STR_ID_16194, "L", kEvent.m_kCreatedPetInfo.m_wstrPetName ) );
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), wstrComment.c_str(), g_pMain->GetNowState() );			
		
		CX2PET::PetInfo petInfo = g_pData->GetPetManager()->GetMyPet()->GetPetInfo();		
		petInfo.m_wstrPetName = kEvent.m_kCreatedPetInfo.m_wstrPetName;

		CX2PET* pPet = g_pData->GetPetManager()->GetMyPet();		
		if( pPet != NULL)
		{ 
			pPet->SetInfo(petInfo);
		}

		KPetInfo* pPetInfo = g_pData->GetMyUser()->GetSelectUnit()->GetPetInfo();
		if( pPetInfo != NULL)
		{
			pPetInfo->m_wstrPetName = petInfo.m_wstrPetName;
		}

		ShowChangePetName(false);
		return true;	
	}
	return false;
}  
#endif CHANGE_PET_NAME
void CX2PetManager::Handler_EGS_CREATE_PET_REQ( UidType itemUid, wstring wstrPetName )
{
	if ( g_pData->GetServerProtocol() == NULL || 
#ifdef  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
        g_pData->GetServerProtocol()->IsUserProxyValid() == false
#else   X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
		g_pData->GetServerProtocol()->GetUserProxy() == NULL
#endif  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
        )
		return;  

	if( m_bCreatePet == true )
		return;

	KEGS_CREATE_PET_REQ kPacket;
	kPacket.m_iItemUID = itemUid;
	kPacket.m_wstrPetName = wstrPetName;

	g_pData->GetServerProtocol()->SendPacket( EGS_CREATE_PET_REQ, kPacket );

	m_bCreatePet = true;

	return;
}

bool CX2PetManager::Handler_EGS_CREATE_PET_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CREATE_PET_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	m_bCreatePet = false;

	if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
	{		
		// 펫 생성 성공

		g_pData->GetMyUser()->GetSelectUnit()->AccessInventory().UpdateInventorySlotList( kEvent.m_vecKInventorySlotInfo );
		// 켜져있는지 확인 후 업데이트 
		if( g_pData->GetUIManager()->GetShow( CX2UIManager::UI_MENU_INVEN ) == true )
		{
			g_pData->GetUIManager()->GetUIInventory()->UpdateInventorySlotList( kEvent.m_vecKInventorySlotInfo );
		}
		
		wstring wstrComment = GET_REPLACED_STRING( ( STR_ID_5350, "L", kEvent.m_kCreatedPetInfo.m_wstrPetName ) );
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), wstrComment.c_str(), g_pMain->GetNowState() );				

		if( g_pData != NULL && g_pData->GetUIManager() != NULL && g_pData->GetUIManager()->GetUIPetList() != NULL )
		{
			if( g_pData->GetUIManager()->GetUIPetList()->GetShow() == true && g_pData->GetUIManager()->GetUIPetList()->GetViewTab() == CX2UIPetInfo::PIT_LIST )
			{
				g_pData	 ->GetUIManager()->GetUIPetList()->ViewListTab();
			}
		}
		
		if( g_pData != NULL && g_pData->GetPetManager() != NULL && g_pData->GetPetManager()->GetMyPet() == NULL )
		{
			// 소환된 펫이 없다면 생성후 바로 펫 소환
			Handler_EGS_SUMMON_PET_REQ( kEvent.m_kCreatedPetInfo.m_iPetUID );
		}
		else
		{
			m_vecNeverSummonPetList.push_back( kEvent.m_kCreatedPetInfo.m_iPetUID );
		}

		return true;
	}

	return false;
}

void CX2PetManager::Handler_EGS_FEED_PETS_REQ( UidType iItemUid )
{
	if( GetMyPet() == NULL )
	{
		// 소환된 펫이 없습니다.
		return;
	}

	if ( g_pData->GetServerProtocol() == NULL || 
#ifdef  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
        g_pData->GetServerProtocol()->IsUserProxyValid() == false
#else   X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
		g_pData->GetServerProtocol()->GetUserProxy() == NULL
#endif  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
        )
		return;
	
	KEGS_FEED_PETS_REQ kPacket;
	kPacket.m_iItemUID = iItemUid;

	g_pData->GetServerProtocol()->SendPacket( EGS_FEED_PETS_REQ, kPacket );

	return;

}

bool CX2PetManager::Handler_EGS_FEED_PETS_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_FEED_PETS_ACK kEvent;
	DeSerialize( pBuff, &kEvent );
	

	//CX2State* pState = (CX2State*)g_pMain->GetNowState();
	//if( pState != NULL )
	//{
	//	CX2Cursor* pCursor	= NULL;
	//	pCursor				= pState->GetCursor();

	//	if ( pCursor != NULL && pCursor->GetCurorState() == CX2Cursor::XCS_PET_FOOD )
	//	{
	//		pCursor->ChangeCursorState( CX2Cursor::XCS_NORMAL );
	//	}
	//}	

	if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
	{		
		CX2PET *pPet = GetMyPet();
		if( pPet != NULL )
		{
#ifdef SERV_EVENT_PET_INVENTORY
			//여기서 이벤트 먹이의 사용 유무를 가져온다.
			if( kEvent.m_EventFoodEat ) //먹이를 먹었다.인벤토리 열어주자.
			{			
				pPet->GetPetInfo().m_bEventFoodEat = kEvent.m_EventFoodEat;
				g_pData->GetUIManager()->GetUIInventory()->UpdateButtonPetInventory();
				g_pData->GetMyUser()->GetSelectUnit()->AccessInventory().UpdateInventorySlotList( kEvent.m_vecInventorySlotInfo );
				// 켜져있는지 확인 후 업데이트 
				if( g_pData->GetUIManager()->GetShow( CX2UIManager::UI_MENU_INVEN ) == true )
				{
					g_pData->GetUIManager()->GetUIInventory()->UpdateInventorySlotList( kEvent.m_vecInventorySlotInfo );
				}
				return true;
			}
#endif SERV_EVENT_PET_INVENTORY
			float fOldSatiety = pPet->GetPetInfo().m_Satiety;
			pPet->GetPetInfo().m_Satiety = (float)kEvent.m_sSatiety;
			
			g_pData->GetMyUser()->GetSelectUnit()->GetPetInfo()->m_sSatiety = kEvent.m_sSatiety;
			if( g_pData->GetUIManager()->GetUIPetList() != NULL )
			{
				g_pData->GetUIManager()->GetUIPetList()->ChangeInfo();
			}

			g_pData->GetMyUser()->GetSelectUnit()->AccessInventory().UpdateInventorySlotList( kEvent.m_vecInventorySlotInfo );
			// 켜져있는지 확인 후 업데이트 
			if( g_pData->GetUIManager()->GetShow( CX2UIManager::UI_MENU_INVEN ) == true )
			{
				g_pData->GetUIManager()->GetUIInventory()->UpdateInventorySlotList( kEvent.m_vecInventorySlotInfo );
			}

			if( g_pChatBox != NULL )
			{
				float fNewSatiety = pPet->GetPetInfo().m_Satiety;
				int iNewSatiety = (int)(fNewSatiety / 48);
				float fSatietyGap = (pPet->GetPetInfo().m_Satiety - fOldSatiety) / 48.f;				
				wstring wstrBuff = GET_REPLACED_STRING( ( STR_ID_6542, "Lfi", pPet->GetPetInfo().m_wstrPetName, fSatietyGap, iNewSatiety ) );
				g_pChatBox->AddChatLog( wstrBuff.c_str(), KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(0.92549f, 0.92549f, 0.53333f, 1.f), L"#CECEC88" );
			}			

			return true;
		}		
	}

	return false;
}
//{{ oasis907 : 김상윤 [2010.9.9] // 자동 먹이 NOT
bool CX2PetManager::Handler_EGS_AUTO_FEED_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_AUTO_FEED_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pData != NULL && g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetSelectUnit() != NULL )
	{
		g_pData->GetMyUser()->GetSelectUnit()->AccessInventory().UpdateInventorySlotList( kEvent.m_vecPetInventorySlotInfo );
		// 켜져있는지 확인 후 업데이트 
		if( g_pData->GetUIManager()->GetShow( CX2UIManager::UI_MENU_INVEN ) == true &&
			g_pData->GetUIManager()->GetUIInventory()->GetUIPetInventory() != NULL &&
			g_pData->GetUIManager()->GetUIInventory()->GetUIPetInventory()->GetShow() == true )
		{
			g_pData->GetUIManager()->GetUIInventory()->GetUIPetInventory()->UpdateUI();
		}
	}
	return true;
}
//}}

bool CX2PetManager::Handler_EGS_FIRST_FEED_PETS_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_FIRST_FEED_PETS_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
	
	CX2PET *pPet = GetMyPet();
	if( pPet != NULL )
	{		
		if( g_pChatBox != NULL )
		{					
			wstring wstrBuff = GET_REPLACED_STRING( ( STR_ID_6546, "L", pPet->GetPetInfo().m_wstrPetName ) );
			g_pChatBox->AddChatLog( wstrBuff.c_str(), KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(0.92549f, 0.92549f, 0.53333f, 1.f), L"#CECEC88" );

			if( kEvent.m_bAutoFeed == false )
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(270,350), wstrBuff.c_str(), g_pMain->GetNowState() );
			}
		}
	}
	
	return true;
}

//{{ 2011. 05. 23  김민성	큐피엘 젤리 친밀도 증감
#ifdef SERV_ADD_KEWPIEL_JELLY
bool CX2PetManager::Handler_EGS_EAT_SPECIAL_FEED_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_EAT_SPECIAL_FEED_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	CX2PET *pPet = GetMyPet();
	if( pPet != NULL )
	{		
		if( g_pChatBox != NULL )
		{	
            if( kEvent.m_sPercent > 0 ) // 친밀도 증가
			{
				wstring wstrBuff = GET_REPLACED_STRING( ( STR_ID_12269, "Li", pPet->GetPetInfo().m_wstrPetName, kEvent.m_sPercent ) );
				g_pChatBox->AddChatLog( wstrBuff.c_str(), KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(0.92549f, 0.92549f, 0.53333f, 1.f), L"#CECEC88" );
				ChangeIntimacy( kEvent.m_iUnitUID, kEvent.m_iPetUID, kEvent.m_iCurrentIntimacy );
			}
			else			// 친밀도 감소
			{
				kEvent.m_sPercent = kEvent.m_sPercent * (-1);
				wstring wstrBuff = GET_REPLACED_STRING( ( STR_ID_12270, "Li", pPet->GetPetInfo().m_wstrPetName, kEvent.m_sPercent ) );
				g_pChatBox->AddChatLog( wstrBuff.c_str(), KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(0.92549f, 0.92549f, 0.53333f, 1.f), L"#CECEC88" );
				ChangeIntimacy( kEvent.m_iUnitUID, kEvent.m_iPetUID, kEvent.m_iCurrentIntimacy );
			}
		}
	}

	return true;
}
#endif SERV_ADD_KEWPIEL_JELLY
//}}

#ifdef SERV_HALLOWEEN_PUMPKIN_FAIRY_PET
bool CX2PetManager::Handler_EGS_CHANGE_PET_ID_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHANGE_PET_ID_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
	{
		if( kEvent.m_iPetUID <= 0 )
			return false;

		if( NULL != g_pData->GetUIManager() && NULL != g_pData->GetUIManager()->GetUIPetList() )
		{
			CX2UIPetInfo* pUIPetInfo = g_pData->GetUIManager()->GetUIPetList();

			if( true == pUIPetInfo->GetShow() )
			{
				pUIPetInfo->SetShow( false );
			}
		}

		g_pData->GetPetManager()->Handler_EGS_SUMMON_PET_REQ( kEvent.m_iPetUID );

		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_28198 ), g_pMain->GetNowState() );		
	}

	return true;
}
#endif //SERV_HALLOWEEN_PUMPKIN_FAIRY_PET

/** @function : UpdateMasterUserSocketAndEnchantData
	@brief : 던전 및 필드 등에서 펫을 소환/해제 했을 때 기운효과에 의해서 변경되는 Socket및 Enchant 데이타를 업데이트
	@param : CX2GUUser* pMasterUser_
*/
void CX2PetManager::UpdateMasterUserSocketAndEnchantData( CX2GUUser* pMasterUser_ )
{
	if ( NULL != pMasterUser_ )
	{
		pMasterUser_->UpdateSocketDataAndEnchantData();
		pMasterUser_->UpdatePassiveAndActiveSkillState();
		pMasterUser_->ResetMaxHP();
		pMasterUser_->ResetMaxMP();
	}
}

#ifdef SERV_PERIOD_PET
bool CX2PetManager::Handler_EGS_RELEASE_PET_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_RELEASE_PET_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if( NULL != GetPetByUid(kEvent.m_iPetUID) )
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250,300 ), GET_REPLACED_STRING( ( STR_ID_195, "L", GetPetByUid(kEvent.m_iPetUID)->GetPetInfo().m_wstrPetName ) ), g_pMain->GetNowState() );
	}
		
	const UidType uidMasterUser = kEvent.m_iUnitUID;

	if ( 0 > uidMasterUser )
		return true;

	bool bMyPet = false;
	if( g_pData != NULL && g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetSelectUnit() != NULL )
	{
		if( uidMasterUser == g_pData->GetMyUser()->GetSelectUnit()->GetUID() )
		{
			bMyPet = true;
			SetSummonedPetMp( 0.0f );
		}
	}

	// 소환해제
	RemovePet( uidMasterUser );

	if ( NULL != g_pX2Game )
	{
		CX2GUUser* pMasterUser = g_pX2Game->GetUserUnitByUID( uidMasterUser );
		if ( NULL != pMasterUser )
		{
			pMasterUser->GetUnit()->ClearPetInfo();
			UpdateMasterUserSocketAndEnchantData( pMasterUser );
		}
	}

	// 여기까지
	if( false == bMyPet )
		return true;

	CX2UIManager* pUIManager = g_pData->GetUIManager();
	if ( NULL != pUIManager )
	{
		if ( NULL != pUIManager->GetUIPetList() )
			pUIManager->GetUIPetList()->UpdateView();

		CX2UIInventory* pUIInventory = pUIManager->GetUIInventory();

		if ( NULL != pUIInventory && true == pUIInventory->GetShow() )
		{
			pUIInventory->UpdateUIButton();

			//{{ oasis907 : 김상윤 [2010.9.8] // 펫 인벤토리 열려있는 상황에서 펫 소환해제시 펫 인벤창 닫음
			CX2UIPetInventory* pPetInventory = pUIInventory->GetUIPetInventory();
			if ( NULL != pPetInventory && true == pPetInventory->GetShow() )
			{
				pPetInventory->SetShow( false );

				if ( NULL != pUIInventory->GetUISocketItem() )
					pUIInventory->GetUISocketItem()->SetShow( false );
			}
			//}}
		}				

#ifdef PET_AURA_SKILL	
		if ( true == pUIManager->GetShow( CX2UIManager::UI_MENU_CHARINFO ) )
			pUIManager->GetUICharInfo()->ResetStat();
#endif
	}

	// 펫리스트에서 제거
	return true;
}
#endif SERV_PERIOD_PET

#ifdef SERV_EVENT_VC
void CX2PetManager::UseIntimacyUpItem( UidType itemUid_ )
{
	if( NULL == GetMyPet()  )
	{
		PetTemplet* pTemplet = GetPetTemplet(CX2PetManager::PUI_PET_MERCENARY_PPORU_EVENT_INT);
		if( NULL != pTemplet )
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_26366 ), g_pMain->GetNowState() );
		}
	}
	else
	{
		if( GetMyPet()->GetPetInfo().m_PetId == CX2PetManager::PUI_PET_MERCENARY_PPORU_EVENT_INT )
		{
			
			if( GetMyPet()->GetPetInfo().m_Evolution_Step != 0 )
				Handler_EGS_USE_INTIMACY_UP_ITEM_REQ( itemUid_ );
			else
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_813 ), g_pMain->GetNowState() );
		}
		else
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_26366 ), g_pMain->GetNowState() );
		}
	}
}

void CX2PetManager::Handler_EGS_USE_INTIMACY_UP_ITEM_REQ( UidType itemUid_ )
{
	if ( g_pData->GetServerProtocol() == NULL || 
		g_pData->GetServerProtocol()->GetUserProxy() == NULL )
		return;

	KEGS_USE_INTIMACY_UP_ITEM_REQ kPacket;
	kPacket.m_iItemUID = itemUid_;

	g_pData->GetServerProtocol()->SendPacket( EGS_USE_INTIMACY_UP_ITEM_REQ, kPacket );
}

bool CX2PetManager::Handler_EGS_USE_INTIMACY_UP_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_USE_INTIMACY_UP_ITEM_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
	{
		g_pData->GetMyUser()->GetSelectUnit()->GetInventory().UpdateInventorySlotList( kEvent.m_vecInventorySlotInfo );
		// 켜져있는지 확인 후 업데이트 
		if( g_pData->GetUIManager()->GetShow( CX2UIManager::UI_MENU_INVEN ) == true )
		{
			g_pData->GetUIManager()->GetUIInventory()->UpdateInventorySlotList( kEvent.m_vecInventorySlotInfo );
		}

		if( NULL != GetMyPet() )
		{
			// 친밀도 상승량이 유동적으로 되면, 2를 그 값으로 변경해주면 됩니다.
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ),  GET_REPLACED_STRING( ( STR_ID_26367, "i", kEvent.m_iUpPercent )), g_pMain->GetNowState() );			
		}
		
		return true;	
	}
	return false;
}  
#endif //SERV_EVENT_VC

#endif //SERV_PET_SYSTEM
