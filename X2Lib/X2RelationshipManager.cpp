#include "StdAfx.h"

#ifdef ADDED_RELATIONSHIP_SYSTEM

#include ".\X2RelationshipManager.h"

CX2RelationshipManager::CX2RelationshipManager () :
m_pMyRelationshipInfo ( NULL ),
m_pUIRelationship ( NULL ),
m_bIsPossibleRefreshData ( true ),
m_fRefreshDataCoolTime ( 0 ),
m_pMyWeddingItemInfo ( NULL ),
m_optrGameUnitRelationPartner (),
m_pSquareUnitRelationPartner ()
#ifdef ADJUST_THIRD_PERSON_BUG
,m_bPropose(false)
#endif //ADJUST_THIRD_PERSON_BUG
#ifdef RELATIONSHIP_SHOW_GUILD_MEMBER_FIX
,m_bInviteGuildMember(false)
#endif //RELATIONSHIP_SHOW_GUILD_MEMBER_FIX
{
	m_pMyRelationshipInfo = new RelationshipInfo;
	m_pUIRelationship = new CX2UIRelationship;
	m_pMyWeddingItemInfo = new WeddingItemInfo;
}


CX2RelationshipManager::~CX2RelationshipManager ()
{
	SAFE_DELETE ( m_pMyRelationshipInfo );
	SAFE_DELETE ( m_pUIRelationship );
	SAFE_DELETE ( m_pMyWeddingItemInfo );

}

HRESULT CX2RelationshipManager::OnFrameMove( double fTime, float fElapsedTime )
{
	CX2RelationshipEffectManager * pRelationshipEffectManager = g_pData->GetRelationshipEffectManager();
	CX2RelationshipManager::RelationshipInfo * pMyRelationInfo = GetMyRelationshipInfo();
	
	if ( pRelationshipEffectManager != NULL )
	{
		CX2RelationshipEffectManager::RelationEffectInfo * pMyRelationEffectInfo = g_pData->GetRelationshipEffectManager()->GetMyRelationEffectInfo();

		if ( NULL != pMyRelationEffectInfo)
		{
			if ( FALSE == GetIsPossibleRefreshData() )
			{
				float fRemainingRefreshDataTime = GetRefreshDataCoolTime() + fElapsedTime;
				if ( fRemainingRefreshDataTime > _CONST_RELATIONSHIP_MANAGER_::g_fRefreshRelationDataCoolTime )
				{
					SetRefreshDataCoolTime( 0.f );
					SetIsPossibleRefreshData( TRUE );
				}
				else
					SetRefreshDataCoolTime( fRemainingRefreshDataTime );

			}

			if ( NULL != pMyRelationInfo )
			{
				if ( pMyRelationInfo->m_eRelationStateType > SEnum::RT_SOLO )
				{
					float fRelationEffectCoolTime = pRelationshipEffectManager->GetMyRelationCloseEffectCooltime();
					fRelationEffectCoolTime += fElapsedTime;
					CX2RelationshipEffectManager::RelationEffectInfo * pMyRelationEffectInfo = pRelationshipEffectManager->GetMyRelationEffectInfo();
				
					if ( NULL != pMyRelationEffectInfo )
					{
						if ( TRUE == pRelationshipEffectManager->IsPartnerClose() && fRelationEffectCoolTime >= pMyRelationEffectInfo->GetRelationEffectPlayingTime () )
						{
							fRelationEffectCoolTime = 0;
							D3DXVECTOR3 vEffectPos;
							if ( pRelationshipEffectManager->GetMyRelationCloseEffectPosition (vEffectPos) == true )
							{
								pMyRelationEffectInfo->SetCloseEffectPosition( vEffectPos );
								pMyRelationEffectInfo->PrepareDrawRelationshipEffect(CX2RelationshipEffectManager::RelationEffectInfo::RET_CLOSE_EFFECT);
								pRelationshipEffectManager->SetMyRealtionEffectPopNotifyCheck(false);

								if ( NULL != g_pData->GetMyUser() &&
									NULL != g_pData->GetMyUser()->GetSelectUnit() )
								{
									if ( TRUE == g_pData->GetMyUser()->GetSelectUnit()->IsManCharacter() )
									{									
										Send_EGS_RELATIONSHIP_EFFECT_NOT ( pMyRelationEffectInfo->GetIndex() );
									}
								}
							}
						}

						if ( false == pRelationshipEffectManager->GetMyRealtionEffectPopNotifyCheck() )
						{
							if ( fRelationEffectCoolTime >= pMyRelationEffectInfo->GetPopNotifyEffectTime() )
							{								
								pMyRelationEffectInfo->SetUserInputMessage( pMyRelationInfo->m_wstrNotifyLoveMessage );
								pMyRelationEffectInfo->PrepareDrawRelationshipEffect(CX2RelationshipEffectManager::RelationEffectInfo::RET_NOTIFY_EFFECT );
								pRelationshipEffectManager->SetMyRealtionEffectPopNotifyCheck(true);
							}
						}

						pRelationshipEffectManager->SetMyRelationCloseEffectCooltime( fRelationEffectCoolTime );
					}
				}
			}


			vector<CX2RelationshipEffectManager::RelationEffectInfo>::iterator it = pRelationshipEffectManager->m_vecRelationEffectInfo.begin();

			for ( vector<CX2RelationshipEffectManager::RelationEffectInfo>::iterator it = pRelationshipEffectManager->m_vecRelationEffectInfo.begin();
				it != pRelationshipEffectManager->m_vecRelationEffectInfo.end(); ++it )
			{
				float fRelationEffectCoolTime = it->GetRelationEffectCooltime();

				if ( fRelationEffectCoolTime == 0 )
				{
					it->SetRealtionEffectPopNotifyCheck(false);				
					it->PrepareDrawRelationshipEffect(CX2RelationshipEffectManager::RelationEffectInfo::RET_CLOSE_EFFECT);
				}

				fRelationEffectCoolTime += fElapsedTime;

				if ( false == it->GetRealtionEffectPopNotifyCheck()  )
				{
					if ( fRelationEffectCoolTime >= it->GetPopNotifyEffectTime() )
					{
						it->PrepareDrawRelationshipEffect(CX2RelationshipEffectManager::RelationEffectInfo::RET_NOTIFY_EFFECT );
						it->SetRealtionEffectPopNotifyCheck(true);
					}
				}

				if ( fRelationEffectCoolTime >= it->GetRelationEffectPlayingTime() )
				{
					if ( (int) pRelationshipEffectManager->m_vecRelationEffectInfo.size() <= 1 )
					{
						pRelationshipEffectManager->m_vecRelationEffectInfo.clear();
						break;
					}
					else	
					{
						it = pRelationshipEffectManager->m_vecRelationEffectInfo.erase( it );
					}
				}

				else
				{
					it->SetRelationEffectCooltime( fRelationEffectCoolTime );
				}
			}
		}	
	}
	return S_OK;
}
bool CX2RelationshipManager::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if ( NULL != m_pUIRelationship )
	{
		if ( true == m_pUIRelationship->UICustomEventProc( hWnd, uMsg, wParam, lParam ) )
		{
			return true;
		}
	}


	return false;
}
bool CX2RelationshipManager::UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( wParam )
	{	
		case EGS_GET_GUILD_USER_LIST_ACK:
		{
			return Handler_EGS_GET_GUILD_USER_LIST_ACK ( hWnd, uMsg, wParam, lParam );
		}
		break;
		
		case EGS_CHECK_BREAK_UP_ACK :
		{
			return Handler_EGS_CHECK_BREAK_UP_ACK ( hWnd, uMsg, wParam, lParam );
		}
		break;

		case EGS_CHECK_NICK_NAME_ACK :
		{
			return Handler_EGS_CHECK_NICK_NAME_ACK ( hWnd, uMsg, wParam, lParam );
		}
		break;

		case EGS_COUPLE_PROPOSE_ACK :
		{
			return Handler_EGS_COUPLE_PROPOSE_ACK ( hWnd, uMsg, wParam, lParam );
		}
		break;

		case EGS_COUPLE_PROPOSE_NOT :
		{
			return Handler_EGS_COUPLE_PROPOSE_NOT ( hWnd, uMsg, wParam, lParam );
		}
		break;
	
		case EGS_COUPLE_PROPOSE_RESULT_NOT :
		{
			return Handler_EGS_COUPLE_PROPOSE_RESULT_NOT ( hWnd, uMsg, wParam, lParam );
		}
		break;


		case EGS_WEDDING_PROPOSE_ACK :
		{
			return Handler_EGS_WEDDING_PROPOSE_ACK ( hWnd, uMsg, wParam, lParam );
		}
		break;

		case EGS_WEDDING_ITEM_INFO_ACK :
		{
			return Handler_EGS_WEDDING_ITEM_INFO_ACK ( hWnd, uMsg, wParam, lParam );
		}
		break;

		case EGS_RELATIONSHIP_EFFECT_NOT :
		{
			return Handler_EGS_RELATIONSHIP_EFFECT_NOT( hWnd, uMsg, wParam, lParam );
		}
		break;
		case EGS_JOIN_WEDDING_HALL_ACK:
		{
			return Handler_EGS_JOIN_WEDDING_HALL_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;


		case EGS_BREAK_UP_ACK :
		{
			return Handler_EGS_BREAK_UP_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;

		case EGS_BREAK_UP_NOT :
		{
			return 	Handler_EGS_BREAK_UP_NOT( hWnd, uMsg, wParam, lParam );
		}
		break;


		case EGS_CHANGE_LOVE_WORD_ACK :
		{
			return Handler_EGS_CHANGE_LOVE_WORD_ACK( hWnd, uMsg, wParam, lParam );
		}
		break;

		case EGS_CHANGE_LOVE_WORD_NOT :
		{
			return Handler_EGS_CHANGE_LOVE_WORD_NOT( hWnd, uMsg, wParam, lParam );
		}
		break;


	}	

	return false;	
	
}

bool CX2RelationshipManager::Handler_EGS_CHANGE_LOVE_WORD_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = reinterpret_cast <KSerBuffer*> (lParam);
	KEGS_CHANGE_LOVE_WORD_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
	
	if ( NULL != g_pData->GetRelationshipManager() &&
		 NULL != g_pData->GetRelationshipManager()->GetMyRelationshipInfo() &&
		 NULL != g_pData->GetMyUser() &&
		 NULL != g_pData->GetMyUser()->GetSelectUnit() )
	{
		g_pData->GetRelationshipManager()->GetMyRelationshipInfo()->m_wstrNotifyLoveMessage 
			= kEvent.m_wstrNewLoveWord;
					
		if ( g_pData->GetMyUser()->GetSelectUnit()->GetUID() == kEvent.m_iUseItemUnitUID )
		{	
			/// 인벤토리 갱신
			if ( NULL != g_pData->GetUIManager() 
				&& g_pData->GetUIManager()->GetUIInventory() )
				g_pData->GetUIManager()->GetUIInventory()->UpdateInventorySlotList( kEvent.m_vecUpdatedInventorySlot );
		}
		return true;
	}

	return false;
}






bool CX2RelationshipManager::Handler_EGS_CHANGE_LOVE_WORD_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = reinterpret_cast <KSerBuffer*> (lParam);
	KEGS_COUPLE_PROPOSE_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if(g_pMain->DeleteServerPacket( EGS_CHANGE_LOVE_WORD_ACK ) )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			return true;
		}
	}
	return false;
}

bool CX2RelationshipManager::Send_EGS_CHANGE_LOVE_WORD_REQ ( UidType ItemUID_, wstring wstrMessage )
{
	KEGS_CHANGE_LOVE_WORD_REQ kPacket;
	kPacket.m_iItemUID = ItemUID_;
	kPacket.m_wstrNewLoveWord = wstrMessage;
	
	g_pData->GetServerProtocol()->SendPacket( EGS_CHANGE_LOVE_WORD_REQ, kPacket ); 
	g_pMain->AddServerPacket( EGS_CHANGE_LOVE_WORD_ACK, 60.f );

	return true;
}



bool CX2RelationshipManager::Handler_EGS_COUPLE_PROPOSE_ACK ( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = reinterpret_cast <KSerBuffer*> (lParam);
	KEGS_COUPLE_PROPOSE_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if(g_pMain->DeleteServerPacket( EGS_COUPLE_PROPOSE_ACK ) )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{

			return true;	
		}
	}

	return false;

}
bool CX2RelationshipManager::Send_EGS_COUPLE_PROPOSE_REQ ( wstring wstrNickname_ )
{
	CX2Main::TimedMessagePopUp::TimedPopupUserData userData;

	userData.iMyUID			= g_pData->GetMyUser()->GetSelectUnit()->GetUID();
	userData.iOpponentUID	= g_pData->GetMyUser()->GetSelectUnit()->GetUID();
	
	if ( NULL != g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItemByTID ( COUPLE_RING_ITEM_ID, true ) )
	{
		wstring wstrTextNotice= GET_REPLACED_STRING( ( STR_ID_24459, "L", wstrNickname_ ) );										

		g_pMain->AddTimedMessagePopup( CX2Main::TimedMessagePopUp::MT_WAIT_CHOICE_COUPLE,
			CX2Main::TimedMessagePopUp::MBT_NO_BUTTON, userData, 10.0f,
			wstrTextNotice.c_str(),
			(CKTDXStage*) g_pMain->GetNowState(), -1, -1 );

		KEGS_COUPLE_PROPOSE_REQ kPacket;
		kPacket.m_wstrUnitName = wstrNickname_;

		g_pData->GetServerProtocol()->SendPacket( EGS_COUPLE_PROPOSE_REQ, kPacket ); 
		g_pMain->AddServerPacket( EGS_COUPLE_PROPOSE_ACK, 60.f );

#ifdef ADJUST_THIRD_PERSON_BUG
		m_bPropose = true;
#endif //ADJUST_THIRD_PERSON_BUG

		return true;
	}
	else
	{
		const CX2Item::ItemTemplet * pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( COUPLE_RING_ITEM_ID );
		if ( NULL != pItemTemplet )
		{			
			wstring wstrTextNotice= GET_REPLACED_STRING( ( STR_ID_24451, "S", pItemTemplet->GetName() ) );													
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), wstrTextNotice.c_str(), g_pMain->GetNowState() );
		}	
	}
	return false;
}




bool CX2RelationshipManager::Handler_EGS_BREAK_UP_NOT ( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = reinterpret_cast <KSerBuffer*> (lParam);
	KEGS_BREAK_UP_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if ( NULL != GetMyRelationshipInfo() )
	{
		wstring wstrRelationType = L"";
		if ( g_pData->GetRelationshipManager()->GetMyRelationshipInfo()->m_eRelationStateType == SEnum::RT_COUPLE )
		{
			wstrRelationType = GET_STRING ( STR_ID_24461 );
		}

		else if ( g_pData->GetRelationshipManager()->GetMyRelationshipInfo()->m_eRelationStateType == SEnum::RT_MARRIED )
		{
			wstrRelationType = GET_STRING ( STR_ID_24462 );
		}
		if ( NULL != g_pData->GetRelationshipManager() &&
			NULL != g_pData->GetRelationshipManager()->GetMyRelationshipInfo() )
		{
			wstring wstrTextNotice = 
				GET_REPLACED_STRING( ( STR_ID_24484, "LL", g_pData->GetRelationshipManager()->GetMyRelationshipInfo()->m_wstrRelationTargetUserNickname, wstrRelationType ) );

			
			if ( NULL != g_pData->GetRelationshipEffectManager() &&
				 NULL != g_pData->GetMyUser() &&
				 NULL != g_pData->GetMyUser()->GetSelectUnit() )
			{
				g_pData->GetRelationshipEffectManager()->DeleteRelationshipAttachEffectHandleByUID
					( g_pData->GetRelationshipManager()->GetMyRelationshipInfo()->m_iRelationTargetUserUid );
				g_pData->GetRelationshipEffectManager()->DeleteRelationshipAttachEffectHandleByUID
					( g_pData->GetMyUser()->GetSelectUnit()->GetUID () );
			}

			// 장착중인 인연 스킬 장착 해제
			if( NULL != g_pData && NULL != g_pData->GetSkillTree() )
			{
				g_pData->GetSkillTree()->UnequipRelationSkill();
			}

			g_pData->GetRelationshipManager()->GetMyRelationshipInfo()->ClearRelationInfo();

			g_pData->GetRelationshipManager()->ResetRelationPartnerInVillage();
			g_pData->GetRelationshipManager()->ResetRelationPartnerInGame();

			if ( NULL != g_pData->GetMyUser() && 
				NULL != g_pData->GetMyUser()->GetSelectUnit() )
			{
				int iNowED = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_ED;
				g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_ED = iNowED - kEvent.m_iCost;
			}




			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), wstrTextNotice.c_str(), g_pMain->GetNowState() );				
			Send_EGS_RELATIONSHIP_EFFECT_NOT ( 0 );
			if( NULL != g_pData->GetMessenger() )
			{
				g_pData->GetMessenger()->SetRelationshipUI ( SEnum::RT_SOLO );

				if (false == g_pData->GetMessenger()->GetHideWindow() 
					&& true == g_pData->GetMessenger()->GetOpen() 
					&& CX2Community::XMUT_RELATION == g_pData->GetMessenger()->GetOpenTab() )
					g_pData->GetMessenger()->SetOpen( true );				
					
				if ( NULL != g_pData->GetMessenger() )
					g_pData->GetMessenger()->ClearUnitViewerUIInRelationshipTab ();
				return true;
			}
		}
	}
	
	return false;
}





bool CX2RelationshipManager::Handler_EGS_BREAK_UP_ACK ( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = reinterpret_cast <KSerBuffer*> (lParam);
	KEGS_BREAK_UP_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if(g_pMain->DeleteServerPacket( EGS_BREAK_UP_ACK ) )
	{
/*		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{

			return true;
		}
*/
	}
	return false;
}

bool CX2RelationshipManager::Handler_EGS_CHECK_BREAK_UP_ACK ( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = reinterpret_cast <KSerBuffer*> (lParam);
	KEGS_CHECK_BREAK_UP_ACK kEvent;
	DeSerialize( pBuff, &kEvent );
	if(g_pMain->DeleteServerPacket( EGS_CHECK_BREAK_UP_ACK ) )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			if ( NULL != GetMyRelationshipInfo() )
			{
				if ( kEvent.m_iOK == NetError::NET_OK )
				{
					wstring wstrRelationType = L"";
					if ( kEvent.m_cRelationshipType == SEnum::RT_COUPLE )
					{
						wstrRelationType = GET_STRING ( STR_ID_24461 );
					}

					else if (  kEvent.m_cRelationshipType == SEnum::RT_MARRIED )
					{
						wstrRelationType = GET_STRING ( STR_ID_24462 );
					}

					wstring wstrTextNotice = 
						GET_REPLACED_STRING( ( STR_ID_24482, "LiL", wstrRelationType, kEvent.m_iCost, kEvent.m_wstrLoverNickName  ) );

					g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), wstrTextNotice.c_str(), CX2UIRelationship::RUM_COUPLE_FAREWELL_OK, g_pMain->GetNowState() );				
				}
				
				else// RELATIONSHIP 40번
				{
					wstring wstrTextNotice = 
						GET_REPLACED_STRING( ( STR_ID_24485, "Li", kEvent.m_wstrLoverNickName, kEvent.m_iCost ) );

					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), wstrTextNotice.c_str(), g_pMain->GetNowState() );
				}
				
				return true;
			}
		}
	}
	return false;
}


/** @function : Handler_EGS_JOIN_WEDDING_HALL_REQ
	@brief : 결혼식장 입장 패킷
	@param : iItemUID(입장하기 위해 사용한 아이템UID)
*/
bool CX2RelationshipManager::Handler_EGS_JOIN_WEDDING_HALL_REQ ( UidType iItemUID_  )
{
	KEGS_JOIN_WEDDING_HALL_REQ kPacket;
	kPacket.m_iItemUID = iItemUID_;

	map<UidType, WeddingItemInfo>::iterator mit = m_mapMyWeddingInviteLetterItemInfo.find(iItemUID_);
	if( mit != m_mapMyWeddingInviteLetterItemInfo.end() )
		kPacket.m_iWeddingUID = mit->second.m_iWeddingHallUID;


	g_pData->GetServerProtocol()->SendPacket( EGS_JOIN_WEDDING_HALL_REQ, kPacket ); 
	g_pMain->AddServerPacket( EGS_JOIN_WEDDING_HALL_ACK, 60.0f );			

	return true;
}

/** @function : Handler_EGS_JOIN_WEDDING_HALL_ACK
	@brief : 결혼식장 입장 패킷
*/
bool CX2RelationshipManager::Handler_EGS_JOIN_WEDDING_HALL_ACK  ( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = reinterpret_cast <KSerBuffer*> (lParam);
	KEGS_JOIN_WEDDING_HALL_ACK kEvent;
	DeSerialize( pBuff, &kEvent );
	
	if(g_pMain->DeleteServerPacket( EGS_JOIN_WEDDING_HALL_ACK ) )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			CX2WeddingGame::UDPData sUDPData;
			sUDPData.m_Port = kEvent.m_RoomInfo.m_usUDPRelayPort;
			sUDPData.m_RelayIP = kEvent.m_RoomInfo.m_wstrUDPRelayIP;

			g_pWeddingGame = new CX2WeddingGame( &sUDPData, 
												static_cast<SEnum::WEDDING_HALL_TYPE>(kEvent.m_kWeddingInfo.m_cWeddingHallType), 
												static_cast<SEnum::WEDDING_OFFICIANT_TYPE>(kEvent.m_kWeddingInfo.m_cOfficiantNPC) );

			// 결혼식 끝난 후 돌아갈 위치에 대한 정보를 저장.( 마을 or 필드 )
			g_pWeddingGame->SetReturnStateID( static_cast<int>(g_pMain->GetNowStateID()) );
			if( NULL != g_pData && g_pData->GetLocationManager() &&
				NULL != g_pTFieldGame && NULL != g_pTFieldGame->GetMyUnit() )
			{
				g_pData->GetLocationManager()->SetLastPos( g_pTFieldGame->GetMyUnit()->GetLandPosition() );
				g_pData->GetLocationManager()->SetLastPosLineIndex( g_pTFieldGame->GetMyUnit()->GetLastTouchLineIndex() );
			}


			UINT uiWeddingHeroineNum = 0;
			BOOST_FOREACH( KRoomSlotInfo kSlotInfo, kEvent.m_vecSlot )
			{
				if( kSlotInfo.m_kRoomUserInfo.m_nUnitUID <= 0 )
					continue;

				// 주인공 숫자 체크( 신랑,신부 입장 시 시작 버튼 출력 해주기 위해 체크 )
				switch( static_cast<SEnum::WEDDING_STATUS>(kSlotInfo.m_kRoomUserInfo.m_cWeddingStatus) )
				{
				case SEnum::WS_GROOM:
				case SEnum::WS_BRIDE:
					{
						++uiWeddingHeroineNum;
					} break;
				case SEnum::WS_GUEST:
					break;
				default:
					ASSERT(!" Wedding Status 오류");
					continue;
					break;
				}
				// 유닛 생성
				g_pWeddingGame->JoinSquareUnit( &kSlotInfo.m_kRoomUserInfo );
				
			}
			// 주인공 숫자 체크( 신랑,신부 입장 시 시작 버튼 출력 해주기 위해 체크 )
			g_pWeddingGame->SetWeddingHeroineNum( uiWeddingHeroineNum );

			g_pKTDXApp->SendGameMessage( XGM_STATE_CHANGE, CX2Main::XS_WEDDING_GAME, 0, false ); 
		}
	}
	return true;
}

bool CX2RelationshipManager::Send_EGS_RELATIONSHIP_INFO_REQ ()
{
	if ( NULL != GetMyRelationshipInfo() )
	{
		KEGS_RELATIONSHIP_INFO_REQ kPacket;
		kPacket.m_iAcceptUnitUID =  GetMyRelationshipInfo()->m_iRelationTargetUserUid;
		g_pData->GetServerProtocol()->SendPacket( EGS_RELATIONSHIP_INFO_REQ, kPacket ); 
		g_pMain->AddServerPacket( EGS_RELATIONSHIP_INFO_ACK, 60.0f );			
	}


	return true;
}

bool CX2RelationshipManager::Send_EGS_CHECK_NICK_NAME_REQ ( wstring wstrNickname_ )
{
	KEGS_CHECK_NICK_NAME_REQ kPacket;
	kPacket.m_wstrNickName = wstrNickname_;
	g_pData->GetServerProtocol()->SendPacket( EGS_CHECK_NICK_NAME_REQ, kPacket ); 
	g_pMain->AddServerPacket( EGS_CHECK_NICK_NAME_ACK, 60.0f );

	return true;
}

bool CX2RelationshipManager::Handler_EGS_CHECK_NICK_NAME_ACK  ( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = reinterpret_cast <KSerBuffer*> (lParam);
	KEGS_CHECK_NICK_NAME_ACK kEvent;
	DeSerialize( pBuff, &kEvent );


	if(g_pMain->DeleteServerPacket( EGS_CHECK_NICK_NAME_ACK ) )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			m_pUIRelationship->AddInvitingUserInfo ( CX2UIRelationship::InviteWeddingUserInfo (kEvent.m_wstrNickName, kEvent.m_iUnitUID), CX2UIRelationship::RIT_NICKNAME );
			return true;
		}
	}
	return false;
}


bool CX2RelationshipManager::Send_EGS_COUPLE_PROPOSE_AGREE_NOT ( PROPOSE_RETURNED_MESSAGE eAgreeCouple)
{
	if ( NULL != GetMyRelationshipInfo() && SEnum::RT_SOLO == GetMyRelationshipInfo()->m_eRelationStateType &&
		NULL != g_pData->GetMyUser() && NULL != g_pData->GetMyUser()->GetSelectUnit() )
	{
		KEGS_COUPLE_PROPOSE_AGREE_NOT kPacket;
		kPacket.m_cAnswer = static_cast<char>( eAgreeCouple );

		kPacket.m_iRequestUnitUID =		GetMyRelationshipInfo()->m_iRelationTargetUserUid;
		kPacket.m_wstrRequestUnitName = GetMyRelationshipInfo()->m_wstrRelationTargetUserNickname;
		kPacket.m_iAcceptUnitUID	  = g_pData->GetMyUser()->GetSelectUnit()->GetUID();
		kPacket.m_wstrAcceptUnitName = g_pData->GetMyUser()->GetSelectUnit()->GetNickName();
		g_pData->GetServerProtocol()->SendPacket( EGS_COUPLE_PROPOSE_AGREE_NOT, kPacket ); 
		
		if ( PRM_OK == eAgreeCouple )
			GetMyRelationshipInfo()->ClearRelationInfo();
	}

	return true;
}


bool CX2RelationshipManager::Send_EGS_WEDDING_PROPOSE_REQ ( KEGS_WEDDING_PROPOSE_REQ & kEvent )
{
	g_pData->GetServerProtocol()->SendPacket( EGS_WEDDING_PROPOSE_REQ, kEvent ); 
	g_pMain->AddServerPacket( EGS_WEDDING_PROPOSE_ACK, 60.0f );			

	return true;
}


bool CX2RelationshipManager::Send_EGS_RELATIONSHIP_EFFECT_NOT  ( int idx_ )
{
	KEGS_RELATIONSHIP_EFFECT_NOT kEvent;
	CX2RelationshipEffectManager * pRelationshipEffectManager = g_pData->GetRelationshipEffectManager();
	RelationshipInfo * pRelationshipInfo = GetMyRelationshipInfo();

	if ( NULL != pRelationshipEffectManager && 
		NULL != pRelationshipEffectManager->GetMyRelationEffectInfo() &&
		NULL != pRelationshipInfo &&
		NULL != g_pData->GetMyUser() )
	{
		kEvent.m_iEffectIndex = idx_; // 
		kEvent.m_iGirlUID = pRelationshipInfo->m_iRelationTargetUserUid;
		kEvent.m_iManUID = g_pData->GetMyUser()->GetUID();
		kEvent.m_wstrLoveWord = pRelationshipInfo->m_wstrNotifyLoveMessage;
		
		g_pData->GetServerProtocol()->SendPacket( EGS_RELATIONSHIP_EFFECT_NOT, kEvent ); 
	}
	return true;
}

bool CX2RelationshipManager::Handler_EGS_RELATIONSHIP_EFFECT_NOT  ( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{

	KSerBuffer* pBuff = reinterpret_cast <KSerBuffer*> (lParam);
	KEGS_RELATIONSHIP_EFFECT_NOT kEvent;
	DeSerialize( pBuff, &kEvent );
	
	if ( NULL != g_pData->GetMyUser() &&
		 NULL != g_pData->GetMyUser()->GetSelectUnit() )
	{
		UidType MyUId = g_pData->GetMyUser()->GetSelectUnit()->GetUID ();
		if ( MyUId == kEvent.m_iGirlUID ||  MyUId == kEvent.m_iManUID )
			return true;
	}

	if ( NULL != g_pData->GetRelationshipEffectManager() )
	{	
		CX2RelationshipEffectManager::RelationEffectInfo RelationEffectInfo_;
		RelationEffectInfo_ = ( *(g_pData->GetRelationshipEffectManager()->GetRelationEffectInfoIndex (kEvent.m_iEffectIndex ) ) );
		// 이 부분 수정할 것	
		if ( RelationEffectInfo_.GetRelationCloseEffectDistance() > 0 )
		// 거리로 체크하는 중!! 수정 바람
		{
			if ( TRUE == g_pMain->GetGameOption().GetOptionList().m_bParty )
			{
				if ( FALSE == g_pData->GetPartyManager()->IsMyPartyMember( kEvent.m_iManUID ) ||
					 FALSE == g_pData->GetPartyManager()->IsMyPartyMember( kEvent.m_iGirlUID ) )
				{
					return true;
				}
			}

			RelationEffectInfo_.SetUserInputMessage( kEvent.m_wstrLoveWord );

			D3DXVECTOR3 vPos;
			if ( TRUE == g_pData->GetRelationshipEffectManager()->CalculateCloseEffectPosition( kEvent.m_iGirlUID, kEvent.m_iManUID, vPos ) )
			{
				RelationEffectInfo_.SetCloseEffectPosition( vPos );
			}
			else
			{
				return true;
			}

			g_pData->GetRelationshipEffectManager()->m_vecRelationEffectInfo.push_back ( RelationEffectInfo_ );
			return true;
		}
		else
		{			
			if ( kEvent.m_iEffectIndex == 0 )
			{
				if ( NULL != g_pData->GetRelationshipEffectManager() )
				{
					g_pData->GetRelationshipEffectManager()->DeleteRelationshipAttachEffectHandleByUID ( kEvent.m_iManUID );
					g_pData->GetRelationshipEffectManager()->DeleteRelationshipAttachEffectHandleByUID ( kEvent.m_iGirlUID );
				}
			}
			else if ( kEvent.m_iEffectIndex == 2 )
			{
				RelationEffectInfo_.PrepareDrawRelationshipEffect( CX2RelationshipEffectManager::RelationEffectInfo::RET_COUPLE_MATCHING_EFFECT, kEvent.m_iManUID );
				RelationEffectInfo_.PrepareDrawRelationshipEffect( CX2RelationshipEffectManager::RelationEffectInfo::RET_COUPLE_MATCHING_EFFECT, kEvent.m_iGirlUID );
			}
			else
			{
				RelationEffectInfo_.PrepareDrawRelationshipEffect( CX2RelationshipEffectManager::RelationEffectInfo::RET_ATTACH_EFFECT, kEvent.m_iManUID );
				RelationEffectInfo_.PrepareDrawRelationshipEffect( CX2RelationshipEffectManager::RelationEffectInfo::RET_ATTACH_EFFECT, kEvent.m_iGirlUID );
			}
		}		
	}
	return false;
}

bool CX2RelationshipManager::Handler_EGS_COUPLE_PROPOSE_RESULT_NOT ( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = reinterpret_cast <KSerBuffer*> (lParam);
	KEGS_COUPLE_PROPOSE_RESULT_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	CX2Main::TimedMessagePopUp::TimedPopupUserData userData;
	userData.iMyUID			= g_pData->GetMyUser()->GetSelectUnit()->GetUID();
	userData.iOpponentUID	= g_pData->GetMyUser()->GetSelectUnit()->GetUID();

	if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
	{
		if ( NULL != GetMyRelationshipInfo() )
		{
			PROPOSE_RETURNED_MESSAGE eProposeReturnedMessage = static_cast<PROPOSE_RETURNED_MESSAGE>( kEvent.m_cResult );
			switch ( eProposeReturnedMessage )
			{
				case PRM_OK :
				{
					GetMyRelationshipInfo()->m_eRelationStateType = SEnum::RT_COUPLE;
					GetMyRelationshipInfo()->m_iRelationTargetUserUid = kEvent.m_iUnitUID;
					GetMyRelationshipInfo()->m_ctsDurationDay = 0;
					GetMyRelationshipInfo()->m_wstrRelationTargetUserNickname = kEvent.m_wstrUnitName;
					GetMyRelationshipInfo()->m_wstrNotifyLoveMessage = L"";
					
					if ( NULL != g_pData->GetMyUser() &&
						NULL != g_pData->GetMyUser()->GetSelectUnit() )
					{
						/// 인벤토리 갱신
						if ( NULL != g_pData->GetUIManager() 
							 && g_pData->GetUIManager()->GetUIInventory() )
							 g_pData->GetUIManager()->GetUIInventory()->UpdateInventorySlotList( kEvent.m_vecUpdatedInventorySlot );

						// 커뮤니티 탭이 열려있는 경우 갱신
						if( NULL != g_pData->GetMessenger() )
						{
							g_pData->GetMessenger()->SetRelationshipUI ( SEnum::RT_COUPLE );

							if (false == g_pData->GetMessenger()->GetHideWindow() 
								&& true == g_pData->GetMessenger()->GetOpen() 
								&& CX2Community::XMUT_RELATION == g_pData->GetMessenger()->GetOpenTab() )
								g_pData->GetMessenger()->SetOpen( true );				
						}

						Send_EGS_RELATIONSHIP_EFFECT_NOT ( 2 );

						CX2RelationshipEffectManager::RelationEffectInfo RelationEffectInfo_CoupleMatching;
						RelationEffectInfo_CoupleMatching = ( *(g_pData->GetRelationshipEffectManager()->GetRelationEffectInfoIndex ( 2 ) ) );				
						RelationEffectInfo_CoupleMatching.PrepareDrawRelationshipEffect(CX2RelationshipEffectManager::RelationEffectInfo::RET_COUPLE_MATCHING_EFFECT );

						Send_EGS_RELATIONSHIP_EFFECT_NOT ( 3 );

						CX2RelationshipEffectManager::RelationEffectInfo RelationEffectInfo_AttachParticle;
						RelationEffectInfo_AttachParticle = ( *(g_pData->GetRelationshipEffectManager()->GetRelationEffectInfoIndex ( 3 ) ) );
						RelationEffectInfo_AttachParticle.PrepareDrawRelationshipEffect(CX2RelationshipEffectManager::RelationEffectInfo::RET_ATTACH_EFFECT );


						CX2RelationshipEffectManager::RelationEffectInfo RelationEffectInfo_CoupleMatchingPartner;
						RelationEffectInfo_CoupleMatchingPartner = ( *(g_pData->GetRelationshipEffectManager()->GetRelationEffectInfoIndex ( 2 ) ) );
						RelationEffectInfo_CoupleMatchingPartner.PrepareDrawRelationshipEffect(CX2RelationshipEffectManager::RelationEffectInfo::RET_COUPLE_MATCHING_EFFECT, kEvent.m_iUnitUID );


						switch ( g_pMain->GetNowStateID() )
						{
						case CX2Main::XS_BATTLE_FIELD :
						case CX2Main::XS_PVP_GAME :
						case CX2Main::XS_DUNGEON_GAME :
							g_pData->GetRelationshipManager()->FindAndSetRelationPartnerInGame ();
							break;
						case CX2Main::XS_VILLAGE_MAP :
							g_pData->GetRelationshipManager()->FindAndSetRelationPartnerInVillage();
							break;
						default :
							break;
						}
						
						RelationEffectInfo_AttachParticle.PrepareDrawRelationshipEffect(CX2RelationshipEffectManager::RelationEffectInfo::RET_ATTACH_EFFECT, GetMyRelationshipInfo()->m_iRelationTargetUserUid );

					}
				}
				break;

				case RPM_TIME_OUT :
				{
					if ( g_pMain->IsThereTimedMessagePopup ( CX2Main::TimedMessagePopUp::MT_WAIT_CHOICE_COUPLE ) )
					{
						wstring wstrTextNotice = GET_REPLACED_STRING ( ( STR_ID_24457, "L", kEvent.m_wstrUnitName ) );

						g_pMain->AddTimedMessagePopup( CX2Main::TimedMessagePopUp::MT_NONE,
							CX2Main::TimedMessagePopUp::MBT_NO_BUTTON, userData, 2.0f,					
							wstrTextNotice.c_str(),
							(CKTDXStage*) g_pMain->GetNowState(), -1, -1 );		
					}
					GetMyRelationshipInfo()->ClearRelationInfo();
				}
				break;

				case RPM_NOT_THIS_PLACE :
				{
					if ( g_pMain->IsThereTimedMessagePopup ( CX2Main::TimedMessagePopUp::MT_WAIT_CHOICE_COUPLE ) )
					{
						wstring wstrTextNotice = GET_REPLACED_STRING ( ( STR_ID_24457, "L", kEvent.m_wstrUnitName ) );
						
							g_pMain->AddTimedMessagePopup( CX2Main::TimedMessagePopUp::MT_NONE,
							CX2Main::TimedMessagePopUp::MBT_NO_BUTTON, userData, 2.0f,
							wstrTextNotice.c_str(),	(CKTDXStage*) g_pMain->GetNowState(), -1, -1 );		
					}
					GetMyRelationshipInfo()->ClearRelationInfo();

				}
				break;

				case PRM_DISAGREE :
				{
#ifdef RELATIONSHIP_DISAGREE_MESSAGE_FIX
#else // RELATIONSHIP_DISAGREE_MESSAGE_FIX
					if ( g_pMain->IsThereTimedMessagePopup ( CX2Main::TimedMessagePopUp::MT_WAIT_CHOICE_COUPLE ) )
#endif // RELATIONSHIP_DISAGREE_MESSAGE_FIX
					{
						wstring wstrTextNotice = GET_STRING( STR_ID_24602 );

						g_pMain->AddTimedMessagePopup( CX2Main::TimedMessagePopUp::MT_NONE,
							CX2Main::TimedMessagePopUp::MBT_NO_BUTTON, userData, 2.0f,
							wstrTextNotice.c_str(),	(CKTDXStage*) g_pMain->GetNowState(), -1, -1 );		
					}
					GetMyRelationshipInfo()->ClearRelationInfo();

				} break;
				case PRM_OPTION_DISAGREE :		/// 옵션창의 커플요청 거부 설정으로 인한 리턴 타입
				{
					if ( g_pMain->IsThereTimedMessagePopup ( CX2Main::TimedMessagePopUp::MT_WAIT_CHOICE_COUPLE ) )
					{
						wstring wstrTextNotice = GET_STRING( STR_ID_24671 );

						g_pMain->AddTimedMessagePopup( CX2Main::TimedMessagePopUp::MT_NONE,
							CX2Main::TimedMessagePopUp::MBT_NO_BUTTON, userData, 2.0f,
							wstrTextNotice.c_str(),	(CKTDXStage*) g_pMain->GetNowState(), -1, -1 );		
					}
					GetMyRelationshipInfo()->ClearRelationInfo();

				} break;
			}
			g_pData->GetMessenger()->SetRelationshipUI ( GetMyRelationshipInfo()->m_eRelationStateType );
		}
	}
	
	g_pMain->RemoveTimedMessagePopup( CX2Main::TimedMessagePopUp::MT_WAIT_CHOICE_COUPLE, userData );
	g_pMain->RemoveTimedMessagePopup( CX2Main::TimedMessagePopUp::MT_AGREE_COUPLE, userData );

#ifdef ADJUST_THIRD_PERSON_BUG
	m_bPropose = false;
#endif //ADJUST_THIRD_PERSON_BUG

	return true;
}

bool CX2RelationshipManager::Handler_EGS_WEDDING_PROPOSE_ACK ( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = reinterpret_cast <KSerBuffer*> (lParam);
	KEGS_WEDDING_PROPOSE_ACK kEvent;
	DeSerialize( pBuff, &kEvent );


	if(g_pMain->DeleteServerPacket( EGS_WEDDING_PROPOSE_ACK ) )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			//인벤토리 갱신
			if( NULL != g_pData->GetMyUser() &&
				NULL != g_pData->GetMyUser()->GetSelectUnit() )
			{
				g_pData->GetMyUser()->GetSelectUnit()->AccessInventory().UpdateInventorySlotList( kEvent.m_vecUpdatedInventorySlot );	
			}

			if ( NULL != g_pData && 
				 NULL != g_pData->GetUIManager() && 
				 NULL != g_pData->GetUIManager()->GetUIInventory() )
			{
				g_pData->GetUIManager()->GetUIInventory()->SetShow(false);
			}

			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(270,350), GET_STRING( STR_ID_24478 ), g_pMain->GetNowState() );

			return true;
		}
	}
	return false;
}



bool CX2RelationshipManager::Handler_EGS_WEDDING_ITEM_INFO_ACK ( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = reinterpret_cast <KSerBuffer*> (lParam);
	KEGS_WEDDING_ITEM_INFO_ACK kEvent;
	DeSerialize( pBuff, &kEvent );
	
	if ( NULL != g_pData->GetMyUser()->GetUID() )
	{
		UidType MyUid = g_pData->GetMyUser()->GetUID();
		if(g_pMain->DeleteServerPacket( EGS_WEDDING_ITEM_INFO_ACK ) )
		{
			if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
			{
				std::map< UidType, KWeddingItemInfo >::iterator it = kEvent.m_mapWeddingItemInfo.begin();
			
				/// 인벤토리 갱신
				if ( NULL != g_pData->GetUIManager() &&
					 NULL != g_pData->GetUIManager()->GetUIInventory() )
				{
					g_pData->GetUIManager()->GetUIInventory()->UpdateInventorySlotList( kEvent.m_vecUpdatedInventorySlot );
				}


				for ( ; it != kEvent.m_mapWeddingItemInfo.end(); ++it )
				{
					WeddingItemInfo WeddingItemInfo_;
					WeddingItemInfo_.Clear();
					WeddingItemInfo_.m_iWeddingHallUID = it->second.m_iWeddingUID;
					WeddingItemInfo_.SetWeddingMaleNickname ( it->second.m_wstrGroom );
					WeddingItemInfo_.SetWeddingFemaleNickname ( it->second.m_wstrBride ); 
					WeddingItemInfo_.m_cOfficiantID = it->second.m_cOfficiantNPC;
					WeddingItemInfo_.m_cWeddingHallType = it->second.m_cWeddingHallType;
					WeddingItemInfo_.m_wstrWeddingDate = it->second.m_wstrWeddingDate;
					WeddingItemInfo_.m_iMaleID = it->second.m_iBride;
					WeddingItemInfo_.m_iFemaleID = it->second.m_iGroom;
					WeddingItemInfo_.m_wstrLetterMessage = it->second.m_wstrWeddingMsg;

					m_mapMyWeddingInviteLetterItemInfo.insert ( make_pair ( it->first, WeddingItemInfo_ ) );
					m_mapMyWeddingInviteLetterCoupleNickNameInfo.insert ( make_pair ( it->first, WeddingItemInfo_.GetWeddingCoupleNickName() ) );

				}
			}
		}
	}
	return true;
}


bool CX2RelationshipManager::Send_EGS_WEDDING_ITEM_INFO_REQ ( std::vector< UidType > & m_vecRelationItemUID_)
{
	g_pData->GetServerProtocol()->SendPacket( EGS_WEDDING_ITEM_INFO_REQ, m_vecRelationItemUID_ ); 
	g_pMain->AddServerPacket( EGS_WEDDING_ITEM_INFO_ACK, 60.0f );			

	return true;
}




bool CX2RelationshipManager::Handler_EGS_COUPLE_PROPOSE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = reinterpret_cast <KSerBuffer*> (lParam);
	KEGS_COUPLE_PROPOSE_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	CX2Main::TimedMessagePopUp::TimedPopupUserData userData;

	if ( NULL != GetMyRelationshipInfo() )
	{
		/// 커플신청 거부 상태이면, 바로 거부 의사 전송
		if ( NULL != g_pMain )
		{
			CX2GameOption::OptionList* pOptionList = &g_pMain->GetGameOption().GetOptionList();

			if ( NULL != pOptionList && true == pOptionList->m_bRefuseRequestCouple )
			{
				GetMyRelationshipInfo()->m_iRelationTargetUserUid = kEvent.m_iRequestUnitUID;
				GetMyRelationshipInfo()->m_wstrRelationTargetUserNickname = kEvent.m_wstrRequestUnitName;
				Send_EGS_COUPLE_PROPOSE_AGREE_NOT( PRM_OPTION_DISAGREE );

				return false;
			}
		}

		if ( GetMyRelationshipInfo()->m_eRelationStateType == SEnum::RT_SOLO 
#ifdef ADJUST_THIRD_PERSON_BUG
			&& m_bPropose == false
#endif //ADJUST_THIRD_PERSON_BUG
			)
		{				
			if ( g_pMain->GetNowStateID() == CX2Main::XS_VILLAGE_MAP ||
				 g_pMain->GetNowStateID() == CX2Main::XS_BATTLE_FIELD )
			{
				{
					GetMyRelationshipInfo()->m_iRelationTargetUserUid = kEvent.m_iRequestUnitUID;
					GetMyRelationshipInfo()->m_wstrRelationTargetUserNickname = kEvent.m_wstrRequestUnitName;
	
					wstring wstrTextNotice= GET_REPLACED_STRING( ( STR_ID_24453, "L", kEvent.m_wstrRequestUnitName ) );										

#ifdef CLIENT_COUNTRY_EU
					// 커플 신청 받았을 때 뜨는 UI
					wstrTextNotice = CWordLineHandler::GetStrByLineBreakInX2Main( wstrTextNotice.c_str(), 200, XUF_DODUM_11_NORMAL );
#endif CLIENT_COUNTRY_EU

					g_pMain->AddTimedMessagePopup( CX2Main::TimedMessagePopUp::MT_AGREE_COUPLE,
						CX2Main::TimedMessagePopUp::MBT_OK_CANCEL, userData, 10.f, 
						wstrTextNotice.c_str(), g_pMain->GetNowState(),
						CX2UIRelationship::RUM_COUPLE_INVITED_OK, 
						CX2UIRelationship::RUM_COUPLE_INVITED_CANCEL, 
						CX2UIRelationship::RUM_COUPLE_INVITED_CANCEL );
					return true;
				}
			}
			else
			{
				GetMyRelationshipInfo()->m_iRelationTargetUserUid = kEvent.m_iRequestUnitUID;
				GetMyRelationshipInfo()->m_wstrRelationTargetUserNickname = kEvent.m_wstrRequestUnitName;
			
				Send_EGS_COUPLE_PROPOSE_AGREE_NOT ( RPM_NOT_THIS_PLACE ); // enum 추가 요망
			}
		}
		else
		{
	//		Send_EGS_COUPLE_PROPOSE_AGREE_NOT ( 2 ); // enum 추가 요망
		}
	}

	return true;
}


bool CX2RelationshipManager::Send_EGS_GET_GUILD_USER_LIST_REQ ()
{
	if ( NULL != m_pUIRelationship )
	{
		KEGS_GET_GUILD_USER_LIST_REQ kPacket;
		kPacket.m_uiViewPage			= m_pUIRelationship->GetNowInviteUserPage();
		kPacket.m_cGuildUserListType	= CX2Community::GVI_GUILD_MEMBER_INFO;

#ifdef RELATIONSHIP_SHOW_GUILD_MEMBER_FIX
		m_bInviteGuildMember = true;
#endif //RELATIONSHIP_SHOW_GUILD_MEMBER_FIX

		g_pData->GetServerProtocol()->SendPacket( EGS_GET_GUILD_USER_LIST_REQ, kPacket ); 
		g_pMain->AddServerPacket( EGS_GET_GUILD_USER_LIST_ACK, 60.f );
		
	}
	return true;
}

bool CX2RelationshipManager::Handler_EGS_GET_GUILD_USER_LIST_ACK ( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    if ( NULL != g_pData->GetMessenger() )
	{
#ifdef RELATIONSHIP_SHOW_GUILD_MEMBER_FIX
		if( m_bInviteGuildMember == true )
#else
		if ( g_pData->GetMessenger()->GetOpenTab() == CX2Community::XMUT_RELATION )
#endif //RELATIONSHIP_SHOW_GUILD_MEMBER_FIX	
		{
			if ( NULL != m_pUIRelationship )
			{
				KSerBuffer* pBuff = (KSerBuffer*)lParam;
				KEGS_GET_GUILD_USER_LIST_ACK kEvent;
				DeSerialize( pBuff, &kEvent );

#ifdef RELATIONSHIP_SHOW_GUILD_MEMBER_FIX
				if ( g_pMain->DeleteServerPacket( EGS_GET_GUILD_USER_LIST_ACK ) == true )
#endif //RELATIONSHIP_SHOW_GUILD_MEMBER_FIX
				{
					if ( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
					{	
						m_pUIRelationship->ResetInviteWeddingUserList();
						m_pUIRelationship->SetMaxInviteUserPage ( static_cast< UINT >( kEvent.m_uiTotalPage ) );
						m_pUIRelationship->SetNowInviteUserPage ( static_cast< UINT >( kEvent.m_uiViewPage ) );
						BOOST_FOREACH ( KGuildMemberMessageInfo pGuildMemberInfo, kEvent.m_vecMemberMessageList )
						{
							m_pUIRelationship->AddInviteWeddingUserList
								( CX2UIRelationship::InviteWeddingUserInfo ( pGuildMemberInfo.m_wstrNickName, pGuildMemberInfo.m_iUnitUID ) );
						}
	//					m_pUIRelationship->ResetInvitingUserInfo(); // m_vecInviteWeddingUserList.clear();		
						m_pUIRelationship->ShowInvitedWeddingUserListControlListDlg ( false );
						m_pUIRelationship->ShowInvitedWeddingUserListControlListDlg ( true );
				
						return true;
					}
				}
			}
#ifdef RELATIONSHIP_SHOW_GUILD_MEMBER_FIX
			m_bInviteGuildMember = false;
#endif //RELATIONSHIP_SHOW_GUILD_MEMBER_FIX
		}
	}

	return false;
}



CX2RelationshipManager::WeddingCoupleNickName CX2RelationshipManager::GetWeddingInviteLetterCoupleNickNameByItemUID ( UidType ItemUID_ )
{
	map<UidType, WeddingCoupleNickName>::iterator it = m_mapMyWeddingInviteLetterCoupleNickNameInfo.find  ( ItemUID_ );
	if(  it == m_mapMyWeddingInviteLetterCoupleNickNameInfo.end() )
		return WeddingCoupleNickName();
	
	return it->second;
}


void CX2RelationshipEffectManager::SetRelationEffectInfoIndex ( )
{
	RelationEffectInfo pRelationEffectInfo;
	// 후에 스크립트로 빼거나 해야 할 듯, 소스 정리 필요
	pRelationEffectInfo.m_fPopNotifyEffectTime = 0;
	pRelationEffectInfo.m_fRelationEffectPlayingTime = 0;
	pRelationEffectInfo.m_wstrAttachEffectName = L"";
	pRelationEffectInfo.m_wstrCloseEffectName = L"";
	pRelationEffectInfo.m_wstrNotifyEffectName = L"";
	pRelationEffectInfo.m_wstrUserInputMessage = L"";
	pRelationEffectInfo.m_fRelationCloseEffectDistance = 0;

	m_vecRelationEffectInfoIndex.push_back ( pRelationEffectInfo );


	pRelationEffectInfo.m_fPopNotifyEffectTime = .01f;
	pRelationEffectInfo.m_fRelationEffectPlayingTime = 10.f;
	pRelationEffectInfo.m_wstrAttachEffectName = L"EffectSet_Wedding_Heart_06";
	pRelationEffectInfo.m_wstrCloseEffectName = L"EffectSet_Wedding_Heart_09";
	pRelationEffectInfo.m_wstrNotifyEffectName = L"Weading_Heart_Back.dds";
	pRelationEffectInfo.m_wstrUserInputMessage = L"";
	pRelationEffectInfo.m_fRelationCloseEffectDistance = 250.f;
	m_vecRelationEffectInfoIndex.push_back ( pRelationEffectInfo );


	pRelationEffectInfo.m_fPopNotifyEffectTime = 10.f;
	pRelationEffectInfo.m_fRelationEffectPlayingTime = 5.f;
	pRelationEffectInfo.m_wstrAttachEffectName = L"EffectSet_Wedding_Heart_01";
	pRelationEffectInfo.m_wstrCloseEffectName = L"";
	pRelationEffectInfo.m_wstrNotifyEffectName = L"";
	pRelationEffectInfo.m_wstrUserInputMessage = L"";
	pRelationEffectInfo.m_fRelationCloseEffectDistance = 0.f;
	m_vecRelationEffectInfoIndex.push_back ( pRelationEffectInfo );


	pRelationEffectInfo.m_fPopNotifyEffectTime = .5f;
	pRelationEffectInfo.m_fRelationEffectPlayingTime = 5.f;
	pRelationEffectInfo.m_wstrAttachEffectName = L"EffectSet_Wedding_Heart_06";
	pRelationEffectInfo.m_wstrCloseEffectName = L"";
	pRelationEffectInfo.m_wstrNotifyEffectName = L"";
	pRelationEffectInfo.m_wstrUserInputMessage = L"";
	pRelationEffectInfo.m_fRelationCloseEffectDistance = 0.f;
	m_vecRelationEffectInfoIndex.push_back ( pRelationEffectInfo );


}

void CX2RelationshipEffectManager::SetShowRelationshipAttachEffect ( RELATION_EFFECT_SHOW_TYPE eREST_ )
{

	if ( NULL != g_pData->GetRelationshipManager() &&
		 NULL != g_pData->GetRelationshipManager()->GetMyRelationshipInfo() )
	{
		CX2RelationshipManager::RelationshipInfo * pMyRelationshipInfo =  g_pData->GetRelationshipManager()->GetMyRelationshipInfo();
		// 해당 소스는 마을 및 결혼식장에서만 처리해주는 기능 입니다.
		switch ( eREST_ )
		{
			case REST_SHOW_PARTY_ONLY :
			case REST_SHOW_ALL :
			{
				RelationEffectInfo RelationEffectInfo_;
				RelationEffectInfo_ = ( *(GetRelationEffectInfoIndex ( 3 ) ) );				

				switch ( g_pMain->GetNowStateID () )
				{
					case CX2Main::XS_WEDDING_GAME :
					{
						if ( NULL != g_pWeddingGame &&
							NULL != g_pData->GetPartyManager() )
						{
							for ( int i = 0; i < g_pWeddingGame->GetSquareUnitNum(); ++i )
							{
								CX2SquareUnit* pSquareUnit = g_pWeddingGame->GetSquareUnit( i );
								if ( NULL != pSquareUnit ) 
								{

									if ( eREST_ == REST_SHOW_PARTY_ONLY )
									{
										if ( TRUE == g_pData->GetPartyManager()->IsMyPartyMember ( pSquareUnit->GetUnit()->GetUID() ) )
										{
											if ( pSquareUnit->GetUnit()->GetLoverUnitUID() > 0 )
											{
												RelationEffectInfo_.PrepareDrawRelationshipEffect( CX2RelationshipEffectManager::RelationEffectInfo::RET_ATTACH_EFFECT, pSquareUnit->GetUnit()->GetUID() );
											}		
										}								
									}

									else	// REST_SHOW_ALL
									{
										if ( pSquareUnit->GetUnit()->GetLoverUnitUID() > 0 )
										{
											RelationEffectInfo_.PrepareDrawRelationshipEffect( CX2RelationshipEffectManager::RelationEffectInfo::RET_ATTACH_EFFECT, pSquareUnit->GetUnit()->GetUID() );
										}												
									}

									if ( pSquareUnit->GetUnit()->GetUID() == g_pData->GetMyUser()->GetSelectUnit()->GetUID() )											    
									{											
										if ( pMyRelationshipInfo->m_iRelationTargetUserUid > SEnum::RT_SOLO )
										{
											RelationEffectInfo_.PrepareDrawRelationshipEffect( CX2RelationshipEffectManager::RelationEffectInfo::RET_ATTACH_EFFECT, pSquareUnit->GetUnit()->GetUID() );
										}												
									}

									if ( pSquareUnit->GetUnit()->GetUID() == pMyRelationshipInfo->m_iRelationTargetUserUid )
									{
										RelationEffectInfo_.PrepareDrawRelationshipEffect( CX2RelationshipEffectManager::RelationEffectInfo::RET_ATTACH_EFFECT, pSquareUnit->GetUnit()->GetUID() );
									}
								}			
							}
						}
					}
					break;
				

					case CX2Main::XS_VILLAGE_MAP :
					{
						if ( NULL != g_pTFieldGame &&
							 NULL != g_pData->GetPartyManager() )
						{
							for ( int i = 0; i < g_pTFieldGame->GetSquareUnitNum(); ++i )
							{
								CX2SquareUnit* pSquareUnit = g_pTFieldGame->GetSquareUnit( i );
								if ( NULL != pSquareUnit ) 
								{
									
									if ( eREST_ == REST_SHOW_PARTY_ONLY )
									{
										if ( TRUE == g_pData->GetPartyManager()->IsMyPartyMember ( pSquareUnit->GetUnit()->GetUID() ) )
										{
											if ( pSquareUnit->GetUnit()->GetLoverUnitUID() > 0 )
											{
												RelationEffectInfo_.PrepareDrawRelationshipEffect( CX2RelationshipEffectManager::RelationEffectInfo::RET_ATTACH_EFFECT, pSquareUnit->GetUnit()->GetUID() );
											}		
										}								
									}

									else	// REST_SHOW_ALL
									{
										if ( pSquareUnit->GetUnit()->GetLoverUnitUID() > 0 )
										{
											RelationEffectInfo_.PrepareDrawRelationshipEffect( CX2RelationshipEffectManager::RelationEffectInfo::RET_ATTACH_EFFECT, pSquareUnit->GetUnit()->GetUID() );
										}												
									}

									if ( pSquareUnit->GetUnit()->GetUID() == g_pData->GetMyUser()->GetSelectUnit()->GetUID() )											    
									{											
										if ( pMyRelationshipInfo->m_iRelationTargetUserUid > SEnum::RT_SOLO )
										{
											RelationEffectInfo_.PrepareDrawRelationshipEffect( CX2RelationshipEffectManager::RelationEffectInfo::RET_ATTACH_EFFECT, pSquareUnit->GetUnit()->GetUID() );
										}												
									}

									if ( pSquareUnit->GetUnit()->GetUID() == pMyRelationshipInfo->m_iRelationTargetUserUid )
									{
										RelationEffectInfo_.PrepareDrawRelationshipEffect( CX2RelationshipEffectManager::RelationEffectInfo::RET_ATTACH_EFFECT, pSquareUnit->GetUnit()->GetUID() );
									}
								}			
							}
						}
					}
					break;
				}
			}
			break;
			

			case REST_HIDE_ALL :
			case REST_HIDE_NOT_PARTY_ONLY :
			{
				// 추후에 이펙트 셋 종류가 늘어난다면 Index 값을 따로 추가해야함
				map<UidType, CX2EffectSet::Handle>::iterator it = m_mapRelationshipAttachEffectHandleMap.begin();

				for ( ; it != m_mapRelationshipAttachEffectHandleMap.end(); )
				{		
					if( eREST_ == REST_HIDE_NOT_PARTY_ONLY )
					{
						if ( TRUE == g_pData->GetPartyManager()->IsMyPartyMember( it->first ) )
						{
							++it;
							continue;			
						}
						if ( pMyRelationshipInfo->m_eRelationStateType > SEnum::RT_SOLO  )
						{
							if ( pMyRelationshipInfo->m_iRelationTargetUserUid == it->first )
							{
								++it;
								continue;
							}
							
							if ( g_pData->GetMyUser()->GetSelectUnit()->GetUID() == it->first )
							{
								++it;
								continue;
							}
						}
					}

					g_pData->GetUIEffectSet()->StopEffectSet ( it->second );
					it = m_mapRelationshipAttachEffectHandleMap.erase(it);
				}
			}
			break;
		}
	}
}

void CX2RelationshipEffectManager::DeleteRelationshipAttachEffectHandleByUID ( UidType UnitUID_ )
{
	map<UidType, CX2EffectSet::Handle>::iterator it = m_mapRelationshipAttachEffectHandleMap.begin();
	it = m_mapRelationshipAttachEffectHandleMap.find ( UnitUID_ );
	if ( it != m_mapRelationshipAttachEffectHandleMap.end() )
	{
		g_pData->GetUIEffectSet()->StopEffectSet( it->second );
		m_mapRelationshipAttachEffectHandleMap.erase( it );
	}
}


bool CX2RelationshipEffectManager::IsPartnerClose ()
{
	switch(g_pMain->GetNowStateID())
	{
		case CX2Main::XS_VILLAGE_MAP:
			{
				if ( NULL != g_pTFieldGame && NULL != g_pTFieldGame->GetMyUnit() )
				{
					CX2SquareUnitPtr pSquareUnit = g_pData->GetRelationshipManager()->GetRelationPartnerInVillage();
					
					if ( null != pSquareUnit)
					{
						D3DXVECTOR3 vPos1 = g_pTFieldGame->GetMyUnit()->GetPos();
						D3DXVECTOR3 vPos2 = pSquareUnit->GetPos();

						if ( GetDistance( vPos1, vPos2 ) <= GetMyRelationEffectInfo()->GetRelationCloseEffectDistance() )
						{
							return true;
						}
					}
				}
			}
			break;

		case CX2Main::XS_PVP_GAME:
		case CX2Main::XS_DUNGEON_GAME:
		case CX2Main::XS_BATTLE_FIELD:
			{
				if ( NULL != g_pX2Game && NULL != g_pX2Game->GetMyUnit() )
				{
					CX2GameUnitoPtr optrGameUnit = g_pData->GetRelationshipManager()->GetRelationPartnerInGame();

					if (null !=  optrGameUnit )
					{
						D3DXVECTOR3 vPos1 = g_pX2Game->GetMyUnit()->GetPos();
						D3DXVECTOR3 vPos2 = optrGameUnit->GetPos();

						if ( GetDistance( vPos1, vPos2 ) <= GetMyRelationEffectInfo()->GetRelationCloseEffectDistance() )
						{
							return true;
						}
					}
				}
			}
			break;
	}

	return false;
}

void CX2RelationshipEffectManager::RelationEffectInfo::DrawRelationshipEffect ( wstring wstrEffectName_, UidType Uid, CX2RelationshipEffectManager::RelationEffectInfo::RELATION_EFFECT_PLAY_TYPE eEffectType_, D3DXVECTOR3 vEffectPos )
{
	CX2Main::X2_STATE eNowState_ = g_pMain->GetNowStateID();
	switch ( eEffectType_ )
	{
	case CX2RelationshipEffectManager::RelationEffectInfo::RET_NOTIFY_EFFECT  :
		{
			D3DXCOLOR coTextColor(1.f, 1.f, 1.f, 1.f );			// 255, 255, 255
			D3DXCOLOR coBackColor(1.f, 1.f, 1.f, 1);			// 109, 207, 246
			wstring wstrMsg = GetUserInputMessage();
			

			if ( wstrMsg == L"" )
				return ;

			vEffectPos.y += 100.f;
			PopTalkBoxEffect( Uid, wstrMsg, wstrEffectName_, vEffectPos, coTextColor, D3DXCOLOR(0, 0, 0, 0), coBackColor );			
		}
		break;

	case CX2RelationshipEffectManager::RelationEffectInfo::RET_CLOSE_EFFECT :
		{			
			if ( eNowState_ == CX2Main::XS_VILLAGE_MAP )
			{
				CX2SquareUnitPtr pSquareUnitPtr = g_pTFieldGame->GetMyUnitBySquareUnitPtr();	//  삭제 요망, 오버로드 함수로 넘길 것
				if ( NULL != pSquareUnitPtr )
				{
					CX2EffectSet::Handle hEffectSet = g_pData->GetUIEffectSet()->PlayEffectSet( wstrEffectName_, pSquareUnitPtr);	
					if ( CX2EffectSet::EffectSetInstance* pEffectSetInstance = g_pData->GetUIEffectSet()->GetEffectSetInstance( hEffectSet ) )
					{
						pEffectSetInstance->SetEffectPosition( vEffectPos );
					}
				}

			}
			else
			{
				CX2GameUnit * pMyUnit = static_cast <CX2GameUnit *> ( g_pX2Game->GetMyUnit() );
				if ( NULL != pMyUnit )
					g_pData->GetUIEffectSet()->PlayEffectSet ( wstrEffectName_, pMyUnit,
						NULL, false, -1.f, -1.f, D3DXVECTOR3 ( 1, 1, 1 ), true, vEffectPos );
			}
		}	
		break;
	case CX2RelationshipEffectManager::RelationEffectInfo::RET_COUPLE_MATCHING_EFFECT :
	case CX2RelationshipEffectManager::RelationEffectInfo::RET_ATTACH_EFFECT :
		{
			if ( eNowState_ == CX2Main::XS_VILLAGE_MAP )
			{
				CX2SquareUnitPtr pSquareUnitPtr;

				if ( g_pTFieldGame->GetMyUnitBySquareUnitPtr()->GetUID() == Uid )
				{
					pSquareUnitPtr = g_pTFieldGame->GetMyUnitBySquareUnitPtr();
				}
				else
				{
//					CX2SquareUnit * pSquareUnit = GetSquareUnitByUID ( Uid );
					CX2SquareUnit * pSquareUnitPtr1 = g_pTFieldGame->GetSquareUnitByUID ( Uid );

					const vector<CX2SquareUnitPtr> & UserUnitListSquareUnitPtr = g_pTFieldGame->GetUserUnitListBySquareUnitPtr();
					for( int i = 0; i < (int)UserUnitListSquareUnitPtr.size(); i++ )
					{
						CX2SquareUnitPtr pCX2SquareUnit = UserUnitListSquareUnitPtr[i];
						if( pCX2SquareUnit != NULL )
						{
							if( pCX2SquareUnit->GetUID() == Uid )
							{
								pSquareUnitPtr = pCX2SquareUnit;
								break;
							}
						}
					}
				}	
				if ( NULL != pSquareUnitPtr.get() )
				{
					map<UidType, CX2EffectSet::Handle> ::iterator it = g_pData->GetRelationshipEffectManager()->m_mapRelationshipAttachEffectHandleMap.find ( Uid );
					if ( it == g_pData->GetRelationshipEffectManager()->m_mapRelationshipAttachEffectHandleMap.end() )
					{
						CX2EffectSet::Handle hRelationEffect = g_pData->GetUIEffectSet()->PlayEffectSet( wstrEffectName_, pSquareUnitPtr );
						if ( eEffectType_ == CX2RelationshipEffectManager::RelationEffectInfo::RET_ATTACH_EFFECT )
							g_pData->GetRelationshipEffectManager()->m_mapRelationshipAttachEffectHandleMap.insert ( make_pair (Uid, hRelationEffect ) );
					}
				}
			}
			else
			{
				if ( NULL != g_pX2Game &&
					 NULL != g_pX2Game->GetMyUnit() )
				{
					CX2GameUnit * pUnit = NULL;

					if ( g_pX2Game->GetMyUnit()->GetUnitUID() == Uid )
					{
						pUnit = static_cast<CX2GameUnit *> ( g_pX2Game->GetMyUnit() );
					}
					else
					{
						pUnit = static_cast<CX2GameUnit *> ( g_pX2Game->GetUserUnitByUID (Uid) );						
					}

					if ( NULL != pUnit )
					{
						map<UidType, CX2EffectSet::Handle> ::iterator it = g_pData->GetRelationshipEffectManager()->m_mapRelationshipAttachEffectHandleMap.find ( Uid );
						if ( it == g_pData->GetRelationshipEffectManager()->m_mapRelationshipAttachEffectHandleMap.end() )
						{
							CX2EffectSet::Handle hRelationEffect = g_pData->GetUIEffectSet()->PlayEffectSet ( wstrEffectName_, pUnit,
								NULL, false, -1.f, -1.f, D3DXVECTOR3 ( 1, 1, 1 ),false, pUnit->GetLandPos() );
							
							if ( eEffectType_ == CX2RelationshipEffectManager::RelationEffectInfo::RET_ATTACH_EFFECT )
								g_pData->GetRelationshipEffectManager()->m_mapRelationshipAttachEffectHandleMap.insert ( make_pair (Uid, hRelationEffect ) );
						}
					}
				}
			}
		}
		break;
	}
	
}

void CX2RelationshipEffectManager::RelationEffectInfo::PopTalkBoxEffect( UidType iUnitUID, wstring& wstrMsg, wstring wstrTexName, D3DXVECTOR3 vEffectPos, D3DXCOLOR color /*= 0xff000000 */, D3DXCOLOR outcolor /*= 0x00000000*/, D3DXCOLOR backColor /*= 0xffffffff*/ )
{
	CX2Main::X2_STATE eNowState_ = g_pMain->GetNowStateID();

	if ( eNowState_ == CX2Main::XS_VILLAGE_MAP )
	{
		if ( NULL != g_pTFieldGame )
		{
			CX2SquareUnit* pCX2SquareUnit = g_pTFieldGame->GetSquareUnitByUID( iUnitUID );
			if( pCX2SquareUnit != NULL && pCX2SquareUnit->GetInit() == true && pCX2SquareUnit->GetShowObject() == true &&
				pCX2SquareUnit->GetPersonalShopState() == CX2SquareUnit::PSS_NONE )
			{
				D3DXVECTOR3 center;
				pCX2SquareUnit->GetTransformCenter( &center );

                if ( pCX2SquareUnit->GetBoundingRadius() > 0 )
                {

				//컬링

#ifdef  X2OPTIMIZE_CULLING_WORLDOBJECTMESH_SUBSET
                    float fScaledBoundingRadius = pCX2SquareUnit->GetScaledBoundingRadius();
#else   X2OPTIMIZE_CULLING_WORLDOBJECTMESH_SUBSET
				    float fScale;
				    if( pCX2SquareUnit->GetMatrix().GetXScale() > pCX2SquareUnit->GetMatrix().GetYScale() )
				    {			
					    if( pCX2SquareUnit->GetMatrix().GetXScale() > pCX2SquareUnit->GetMatrix().GetZScale() )
					    {
						    //X가 제일 큼
						    fScale = pCX2SquareUnit->GetMatrix().GetXScale();
					    }
					    else
					    {
						    //Z가 제일 큼
						    fScale = pCX2SquareUnit->GetMatrix().GetZScale();
					    }
				    }
				    else
				    {
					    if( pCX2SquareUnit->GetMatrix().GetYScale() > pCX2SquareUnit->GetMatrix().GetZScale() )
					    {
						    //Y가 제일 큼
						    fScale = pCX2SquareUnit->GetMatrix().GetYScale();
					    }
					    else
					    {
						    //Z가 제일 큼
						    fScale = pCX2SquareUnit->GetMatrix().GetZScale();
					    }
				    }
                    float fScaledBoundingRadius = pCX2SquareUnit->GetBoundingRadius() * fScale;
#endif  X2OPTIMIZE_CULLING_WORLDOBJECTMESH_SUBSET

				    if( g_pKTDXApp->GetDGManager()->GetFrustum().CheckSphere( center, fScaledBoundingRadius ) == false )
					    return;
                }


				CX2TalkBoxManagerImp::TalkBox talkBox;
				talkBox.m_OwnerUnitUID		= -1;
				talkBox.m_wstrTalkContent	= wstrMsg;
				talkBox.m_bTraceUnit		= true;
				talkBox.m_TalkBoxType		= CX2TalkBoxManagerImp::TBT_TALKBOX_EFFECT;
				talkBox.m_coTextColor		= color;
				talkBox.m_OutLineTextColor	= outcolor;
				talkBox.m_BackTexColor		= backColor;
				talkBox.m_bUseOutLine		= true;
				talkBox.m_wstrBacklayerTexture		 = wstrTexName;
				talkBox.m_fRemainTime		= 2.0f;
				talkBox.m_fBacklayerTextureHeight = 200.f;
				talkBox.m_fBacklayerTextureWidth = 200.f;
				talkBox.m_vTalkBoxPosition = vEffectPos;

				if( outcolor == 0x00000000 )	//	입력 없을 경우는 글씨색 그대로 쓰기
				{
					talkBox.m_bUseOutLine = false;
				}

				if ( g_pTFieldGame->GetTalkBoxManager() != NULL )
				{
					g_pTFieldGame->GetTalkBoxManager()->Push( talkBox );
				}
			}
		}
	}

	else		// 필드, 던전, pvp ..
	{
		CX2Room::SlotData* pkSlotData = NULL;
		switch ( eNowState_ )
		{
			case CX2Main::XS_PVP_GAME :
				if( NULL == g_pData->GetPVPRoom() )
					return; 
				pkSlotData = g_pData->GetPVPRoom()->GetSlotDataByUnitUID( iUnitUID );
				if ( pkSlotData == NULL )
					return;

				break;
			case CX2Main::XS_DUNGEON_GAME :
				if( NULL == g_pData->GetDungeonRoom() )
					return; 
				pkSlotData = g_pData->GetDungeonRoom()->GetSlotDataByUnitUID( iUnitUID );
				break;
			case CX2Main::XS_BATTLE_FIELD :
				if( NULL == g_pData->GetBattleFieldRoom() )
					return; 
				pkSlotData = g_pData->GetBattleFieldRoom()->GetSlotDataByUnitUID( iUnitUID );
				if ( pkSlotData == NULL )
					return;
				break;
		}

		if  ( pkSlotData == NULL )
			return ;

		int iSlotIndex = pkSlotData->m_Index;
		if( iSlotIndex < 0 )
			return;	

		if( NULL != g_pX2Game )
		{
			CX2GUUser* pGUUser = g_pX2Game->GetMyUnit();
			if( NULL != pGUUser )
			{
				D3DXVECTOR3 center;
				pGUUser->GetTransformCenter( &center );

				//컬링
				if( pGUUser->GetBoundingRadius() > 0 )
                {
#ifdef  X2OPTIMIZE_CULLING_WORLDOBJECTMESH_SUBSET
                    float   fScaledBoundingRadius = pGUUser->GetScaledBoundingRadius();
#else   X2OPTIMIZE_CULLING_WORLDOBJECTMESH_SUBSET
				    float fScale;
				    if( pGUUser->GetMatrix().GetXScale() > pGUUser->GetMatrix().GetYScale() )
				    {
					    if( pGUUser->GetMatrix().GetXScale() > pGUUser->GetMatrix().GetZScale() )
					    {
						    //X가 제일 큼
						    fScale = pGUUser->GetMatrix().GetXScale();
					    }
					    else
					    {
						    //Z가 제일 큼
						    fScale = pGUUser->GetMatrix().GetZScale();
					    }
				    }
				    else
				    {
					    if( pGUUser->GetMatrix().GetYScale() > pGUUser->GetMatrix().GetZScale() )
					    {
						    //Y가 제일 큼
						    fScale = pGUUser->GetMatrix().GetYScale();
					    }
					    else
					    {
						    //Z가 제일 큼
						    fScale = pGUUser->GetMatrix().GetZScale();
					    }
				    }
                     float   fScaledBoundingRadius = pGUUser->GetBoundingRadius() * fScale;
#endif  X2OPTIMIZE_CULLING_WORLDOBJECTMESH_SUBSET
					if ( g_pKTDXApp->GetDGManager()->GetFrustum().CheckSphere( center, fScaledBoundingRadius ) == false )
					    return;
                }

				CX2TalkBoxManagerImp::TalkBox talkBox;
				talkBox.m_OwnerUnitUID		= -1;
				talkBox.m_wstrTalkContent	= wstrMsg;
				talkBox.m_bTraceUnit		= true;
				talkBox.m_TalkBoxType		= CX2TalkBoxManagerImp::TBT_TALKBOX_EFFECT;
				talkBox.m_coTextColor		= color;
				talkBox.m_OutLineTextColor	= outcolor;
				talkBox.m_BackTexColor		= backColor;
				talkBox.m_bUseOutLine		= true;
				talkBox.m_wstrBacklayerTexture		 = wstrTexName;
				talkBox.m_fRemainTime		= 2.0f;
				talkBox.m_fBacklayerTextureHeight = 200.f;
				talkBox.m_fBacklayerTextureWidth = 200.f;
				talkBox.m_vTalkBoxPosition = vEffectPos;



				if( outcolor == 0x00000000 )	//	입력 없을 경우는 글씨색 그대로 쓰기
				{
					talkBox.m_bUseOutLine = false;
				}

				if ( NULL != g_pX2Game->GetTalkBoxManager() )
					g_pX2Game->GetTalkBoxManager()->Push( talkBox );
			}
		}
	}
}

void CX2RelationshipEffectManager::RelationEffectInfo::PrepareDrawRelationshipEffect( RELATION_EFFECT_PLAY_TYPE ePlayingEffectType_, UidType OwnerUID_ )
{
	{		
		if ( TRUE == g_pMain->GetGameOption().GetFieldSD() )
		{
			return ;
		}

		/// 파티원만 보이는 옵션이 켜져있을 때
		if ( TRUE == g_pMain->GetGameOption().GetFieldParty() )
		{
			/// 내 파티원도 아니고
			if ( false == g_pData->GetPartyManager()->IsMyPartyMember( OwnerUID_ ) )
			{
				/// 내 유닛도 아니고 파트너도 아니면
				if ( 0 != OwnerUID_ && // 0 인 경우 자신의 유닛으로 가정 하는데, 0도 아니고
					!( NULL != g_pData->GetMyUser()->GetSelectUnit() && g_pData->GetMyUser()->GetSelectUnit()->GetUID() == OwnerUID_ ) /// 자신의 유닛도 아니고
					&& !( NULL != g_pData->GetRelationshipManager() && NULL != g_pData->GetRelationshipManager()->GetMyRelationshipInfo()
						&& g_pData->GetRelationshipManager()->GetMyRelationshipInfo()->m_iRelationTargetUserUid == OwnerUID_ ) ) /// 파트너도 아니면
					return ;
			}
		}
	}

	if ( ePlayingEffectType_ == RET_ATTACH_EFFECT )
	{
		if ( NULL != g_pX2Game && NULL != g_pX2Game->GetMyUnit() && g_pX2Game->GetMyUnit()->GetUnitUID() == OwnerUID_ )
		{
			if ( NULL != g_pData->GetRelationshipEffectManager() )
			{
				map<UidType, CX2EffectSet::Handle>::iterator it = g_pData->GetRelationshipEffectManager()->m_mapRelationshipAttachEffectHandleMap.find ( OwnerUID_ );
				if ( it != g_pData->GetRelationshipEffectManager()->m_mapRelationshipAttachEffectHandleMap.end() )
					return ;
			}
		}
	}

	wstring wstrEffectName = L"";
	D3DXVECTOR3 vEffectPos = D3DXVECTOR3 ( 0, 0, 0 );
	switch ( ePlayingEffectType_ )
	{
	case RET_COUPLE_MATCHING_EFFECT :
	case RET_ATTACH_EFFECT :
		{
			wstrEffectName = GetAttachEffectName();
		}
		break;

	case RET_CLOSE_EFFECT :
		{
			wstrEffectName = GetCloseEffectName();

		}
		break;

	case RET_NOTIFY_EFFECT :
		{
			wstrEffectName = GetNotifyEffectName();
		}

		break;
	}

	switch(g_pMain->GetNowStateID())
	{
	case CX2Main::XS_VILLAGE_MAP:
		{
			CX2StateField * pStateFieldGame = static_cast<CX2StateField*>( g_pMain->GetNowState() );
			
			if ( NULL != g_pTFieldGame && 
				NULL != g_pTFieldGame->GetMyUnit() )
			{
				UidType TargetUID = g_pTFieldGame->GetMyUnit()->GetUID();;

				if ( ePlayingEffectType_ == RET_ATTACH_EFFECT || ePlayingEffectType_ == RET_COUPLE_MATCHING_EFFECT )
				{
					CX2SquareUnit * pSquareUnit =  NULL;
					if ( OwnerUID_ > 0 )
					{
						pSquareUnit = g_pTFieldGame->GetSquareUnitByUID ( OwnerUID_ );
						TargetUID = OwnerUID_;
					}
					if ( NULL != pSquareUnit )
					{
						vEffectPos = pSquareUnit->GetPos();
					}
				}				
				else 
				{
					vEffectPos = GetCloseEffectPosition ();
				}
				
				DrawRelationshipEffect ( wstrEffectName, TargetUID, ePlayingEffectType_, vEffectPos );
			}	
		}
		break;


	case CX2Main::XS_TRAINING_GAME :
	case CX2Main::XS_DUNGEON_GAME:
	case CX2Main::XS_PVP_GAME:
	case CX2Main::XS_BATTLE_FIELD:
		{
			if ( NULL != g_pX2Game &&
				 NULL != g_pX2Game->GetMyUnit() && 
				 NULL != g_pX2Game->GetMyUnit()->GetUnit() )
			{
				UidType TargetUID = g_pX2Game->GetMyUnit()->GetUnit()->GetUID();
				
				if ( ePlayingEffectType_ == RET_ATTACH_EFFECT || ePlayingEffectType_ == RET_COUPLE_MATCHING_EFFECT)
				{	
					CX2GUUser * pCX2GUUser = NULL;
					
					if ( OwnerUID_ > 0 )
					{
						pCX2GUUser = g_pX2Game->GetUserUnitByUID( OwnerUID_ );
						TargetUID = OwnerUID_;
					}
					if ( NULL != pCX2GUUser )
					{
						vEffectPos = pCX2GUUser->GetPos();
					}
				}				

				else 
				{
					vEffectPos = GetCloseEffectPosition ();
				}

				DrawRelationshipEffect ( wstrEffectName, TargetUID, ePlayingEffectType_, vEffectPos );
			}
		}
		break;
	}
}








bool CX2RelationshipEffectManager::CalculateCloseEffectPosition ( UidType Uid1_, UidType Uid2_, D3DXVECTOR3 & vPos )
{
	D3DXVECTOR3 vResult = D3DXVECTOR3 ( 0, 0, 0 );
	switch(g_pMain->GetNowStateID())
	{

	case CX2Main::XS_VILLAGE_MAP:
		{
			if ( NULL != g_pTFieldGame )
			{
				CX2SquareUnit * pSquareUnit1 = g_pTFieldGame->GetSquareUnitByUID( Uid1_);
				CX2SquareUnit * pSquareUnit2 = g_pTFieldGame->GetSquareUnitByUID( Uid2_);

				if ( pSquareUnit1 != NULL && pSquareUnit2 != NULL )
				{
					D3DXVECTOR3 vPos1 = pSquareUnit1->GetPos();
					D3DXVECTOR3 vPos2 = pSquareUnit2->GetPos();
					vPos = ( vPos1 + vPos2 ) / 2;
					vPos.y += 100;
					return true;
				}
			}
		}
		break;

	case CX2Main::XS_DUNGEON_GAME :
	case CX2Main::XS_PVP_GAME :
	case CX2Main::XS_BATTLE_FIELD :
		{
			if ( NULL != g_pX2Game )
			{
				CX2GUUser * pUserUnit1 = g_pX2Game->GetUserUnitByUID ( Uid1_);
				CX2GUUser * pUserUnit2 = g_pX2Game->GetUserUnitByUID ( Uid2_);

				if ( pUserUnit1 != NULL && pUserUnit2 != NULL )
				{
					D3DXVECTOR3 vPos1 = pUserUnit1->GetPos();
					D3DXVECTOR3 vPos2 = pUserUnit2->GetPos();

					vPos = ( vPos1 + vPos2 ) / 2;
					vPos.y += 100;
					return true;
				}
			}
		}
		break;
	}
	return false;
}



bool CX2RelationshipManager::FindAndSetRelationPartnerInGame ( )
{
	if (g_pMain->GetNowStateID() == CX2Main::XS_BATTLE_FIELD ||
		g_pMain->GetNowStateID() == CX2Main::XS_DUNGEON_GAME ||
		g_pMain->GetNowStateID() == CX2Main::XS_PVP_GAME )
	{
		if ( NULL != GetMyRelationshipInfo() && NULL != g_pX2Game )
		{
			RelationshipInfo * pRelationshipInfo = GetMyRelationshipInfo();
			
			if ( pRelationshipInfo->m_eRelationStateType != SEnum::RT_SOLO )
			{					
				CX2GUUser * pTargetUser= g_pX2Game->GetUserUnitByUID( pRelationshipInfo->m_iRelationTargetUserUid );
				CX2GameUnit * pGameUnitTargetUser = static_cast<CX2GameUnit *> ( pTargetUser );
				m_optrGameUnitRelationPartner = CX2GameUnitoPtr ( pGameUnitTargetUser );
				return true;
			}
		}	
	}
	return false;
}


bool CX2RelationshipManager::FindAndSetRelationPartnerInVillage ( )
{
	// 찾아보니 검색하는 부분, 이미 만들어진거 있음! ( CX2SquareUnitPtr ( CX2SquareUnit * value ) ) 형태로 감싸서 변경할 것!
	if (g_pMain->GetNowStateID() == CX2Main::XS_VILLAGE_MAP )
	{
		if ( NULL != GetMyRelationshipInfo() && NULL != g_pTFieldGame )
		{
			RelationshipInfo * pRelationshipInfo = GetMyRelationshipInfo();
			if ( pRelationshipInfo->m_eRelationStateType != SEnum::RT_SOLO )
			{					
				const vector<CX2SquareUnitPtr> & UserUnitListSquareUnitPtr = g_pTFieldGame->GetUserUnitListBySquareUnitPtr();
				for( int i = 0; i < (int)UserUnitListSquareUnitPtr.size(); i++ )
				{
					CX2SquareUnitPtr pCX2SquareUnit = UserUnitListSquareUnitPtr[i];
					if( pCX2SquareUnit != NULL )
					{
						if( pCX2SquareUnit->GetUID() == pRelationshipInfo->m_iRelationTargetUserUid )
						{
							m_pSquareUnitRelationPartner = pCX2SquareUnit;
							return true;
						}
					}
				}
			}
		}	
	}
	return false;
}


bool CX2RelationshipEffectManager::GetMyRelationCloseEffectPosition ( D3DXVECTOR3 & vec )
{
	switch(g_pMain->GetNowStateID())
	{

	case CX2Main::XS_VILLAGE_MAP:
		{
			if ( NULL != g_pTFieldGame )
			{
				CX2SquareUnitPtr optrSquareUnit = g_pData->GetRelationshipManager()->GetRelationPartnerInVillage();

				if ( NULL != g_pTFieldGame->GetMyUnit() &&
					 null != optrSquareUnit )
				{
					D3DXVECTOR3 vPos1 = g_pTFieldGame->GetMyUnit()->GetPos();
					D3DXVECTOR3 vPos2 = optrSquareUnit->GetPos();

					vec = ( vPos1 + vPos2 ) / 2;
					vec.y += 100;
					return true;
				}
			}
		}		
		break;


	case CX2Main::XS_DUNGEON_GAME :
	case CX2Main::XS_PVP_GAME :
	case CX2Main::XS_BATTLE_FIELD :
		{
			if ( NULL != g_pX2Game )
			{
				CX2GameUnitoPtr optrGameUnit = g_pData->GetRelationshipManager()->GetRelationPartnerInGame();

				if ( NULL != g_pX2Game->GetMyUnit() &&
					null != optrGameUnit )
				{
					D3DXVECTOR3 vPos1 = g_pX2Game->GetMyUnit()->GetPos();
					D3DXVECTOR3 vPos2 = optrGameUnit->GetPos();

					vec = ( vPos1 + vPos2 ) / 2;
					vec.y += 100;
					return true;
				}
			}
		}
		break;
	}
	
	return false;
}

bool CX2RelationshipManager::IsEqipWeddingClothes() const
{
	bool bIsReady = true;

	if( NULL != g_pData && 
		NULL != g_pData->GetMyUser() &&
		NULL != g_pData->GetMyUser()->GetSelectUnit() )
	{
#ifdef SERV_NEW_ONE_PIECE_AVATAR_SLOT
		// 한벌 아바타 체크
		CX2Item* pItemBody = 
			g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetEquippingItemByEquipPos( CX2Unit::EP_ONEPIECE_FASHION, true );
#else //SERV_NEW_ONE_PIECE_AVATAR_SLOT
		// 상의 아바타 체크
		CX2Item* pItemBody = 
			g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetEquippingItemByEquipPos( CX2Unit::EP_DEFENCE_BODY, true );
#endif //SERV_NEW_ONE_PIECE_AVATAR_SLOT

		if( NULL != pItemBody &&
			NULL != pItemBody->GetItemTemplet() )
		{
			bIsReady = IsWeddingClothesItem( pItemBody->GetItemTemplet()->GetItemID() );
		}
		else
		{
			bIsReady = false;
		}

	}
	return bIsReady;
}

/** @function : CheckWeddingQualification //오현빈
	@brieft : 웨딩 가능 여부 체크
			  커플 상태 유지 일자 기준
*/
bool CX2RelationshipManager::CheckWeddingQualification() const// 웨딩 가능 여부 체크
{
	if( NULL != GetMyRelationshipInfo() )
	{
		int iDurationDay = GetMyRelationshipInfo()->GetDurationDay();
#ifdef SERV_RELATIONSHIP_SYSTEM_INT
		if( iDurationDay >= 0)
#else
		if( iDurationDay >= 3)
#endif SERV_RELATIONSHIP_SYSTEM_INT
		{
			return true;
		}
	}

	g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING ( STR_ID_24476 ), g_pMain->GetNowState() );
	return false;
};
#endif // ADDED_RELATIONSHIP_SYSTEM