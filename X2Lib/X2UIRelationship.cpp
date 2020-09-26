#include "StdAfx.h"
#ifdef ADDED_RELATIONSHIP_SYSTEM

#include ".\X2UIRelationship.h"


CX2UIRelationship::CX2UIRelationship () :
	m_pDLGInviteCoupleMessage ( NULL ),
	m_pDLGInviteWedding ( NULL ),
	m_pDLGInviteWeddingUserList ( NULL ),
	m_pDLGChangeLoveMessage ( NULL ),
	m_pDLGJoinWedding ( NULL )
{
//	m_vecInviteUserIDStringList.clear();
//	m_vecFriendNameList.clear();
	m_vecInviteWeddingUserList.clear();
	m_vecInvitedWeddingUserList.clear();	
	m_uiNowInviteUserPage = 1;
	m_iMaxWeddingInviteNumber = 0;
	m_uiMaxInviteUserPage = 1;
	m_eRelationInviteUserListType = RIT_ADDED;
	m_UidSelectedIItemUID = 0;
}

CX2UIRelationship::~CX2UIRelationship()
{
	SAFE_DELETE_DIALOG ( m_pDLGChangeLoveMessage );
	SAFE_DELETE_DIALOG ( m_pDLGInviteCoupleMessage );
	SAFE_DELETE_DIALOG ( m_pDLGInviteWedding );
	SAFE_DELETE_DIALOG ( m_pDLGInviteWeddingUserList );
	SAFE_DELETE_DIALOG ( m_pDLGJoinWedding );
}
			


HRESULT	CX2UIRelationship::OnFrameMove( double fTime, float fElapsedTime )
{

	return S_OK;
}
bool CX2UIRelationship::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch(wParam)
	{
	case RUM_COUPLE_INVITE_USE_ITEM_CANCEL : 
		{
			ShowInviteCoupleDlg( false );		
			return true;
		}
		break;

	case RUM_COUPLE_CHANGE_LOVE_MESSAGE :
		{
			ShowChangeLoveMessageDlg ( true );
			return true;
		}
		break;

	case RUM_COUPLE_CHANGE_LOVE_MESSAGE_OK :
		{
			if ( NULL != g_pData->GetRelationshipManager() &&
				NULL != g_pData->GetRelationshipManager()->GetMyRelationshipInfo() )			
			{
				if ( g_pData->GetRelationshipManager()->GetMyRelationshipInfo()->m_eRelationStateType
					> SEnum::RT_COUPLE ) 
				{
					CKTDGUIIMEEditBox * pEditBox = static_cast <CKTDGUIIMEEditBox *> ( m_pDLGChangeLoveMessage->GetControl( L"IME_Nicname") );
					if( NULL != pEditBox )
					{
						wstring wstrLoveMessage = pEditBox->GetText();
#ifdef SERV_RELATIONSHIP_SYSTEM_INT
						if ( wstrLoveMessage == GET_STRING( STR_ID_28123 ) )
#else
						if ( wstrLoveMessage == L"초기화" )
#endif SERV_RELATIONSHIP_SYSTEM_INT
						{
							wstrLoveMessage = L"";
							g_pData->GetRelationshipManager()->Send_EGS_CHANGE_LOVE_WORD_REQ ( m_UidSelectedIItemUID, wstrLoveMessage );
							g_pData->GetRelationshipManager()->GetMyRelationshipInfo()->m_wstrNotifyLoveMessage = wstrLoveMessage;

							ShowChangeLoveMessageDlg ( false );

						}
						else if ( TRUE == wstrLoveMessage.empty() )
						{
							g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_23609 ), g_pMain->GetNowState() );					
						}
						else
						{
							wstring wstrFilterMessage = g_pMain->GetStringFilter()->FilteringNoteString( wstrLoveMessage.c_str(), L'♡' );

							g_pData->GetRelationshipManager()->Send_EGS_CHANGE_LOVE_WORD_REQ ( m_UidSelectedIItemUID, wstrFilterMessage );
							g_pData->GetRelationshipManager()->GetMyRelationshipInfo()->m_wstrNotifyLoveMessage = wstrFilterMessage;

							ShowChangeLoveMessageDlg ( false );
						}
					}
				}
			}
			
			return true;
		}
		break;

	case RUM_COUPLE_CHANGE_LOVE_MESSAGE_CANCEL :
		{
			ShowChangeLoveMessageDlg ( false );
			return true;
		}
		break;

	case RUM_COUPLE_INVITE_USE_ITEM_OK :
		{
			CKTDGUIIMEEditBox* pIMEEditBox = static_cast <CKTDGUIIMEEditBox*> ( m_pDLGInviteCoupleMessage->GetControl( L"IMEEditBoxMessage" ) );
			if(pIMEEditBox != NULL)
			{
				wstring wstrMessage = pIMEEditBox->GetText();
				if( wstrMessage.length() < 2 )
				{
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_546 ), g_pMain->GetNowState() );
				}
				else
				{
					if ( NULL != g_pData->GetRelationshipManager() &&
						NULL != g_pData->GetRelationshipManager()->GetMyRelationshipInfo())
					{
						if ( g_pData->GetRelationshipManager()->GetMyRelationshipInfo()->m_eRelationStateType > 
							SEnum::RT_SOLO )
						{
							g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING ( STR_ID_24456 ), g_pMain->GetNowState() );
							return true;
						}
						else
						{
							g_pData->GetRelationshipManager()->Send_EGS_COUPLE_PROPOSE_REQ(wstrMessage);
							ShowInviteCoupleDlg( false );	
						}
					}
				}
			}
			return true;
		}
		break;

	case RUM_COUPLE_INVITE_OK :
		{
			ShowInviteCoupleDlg(false);
			return true;
		}
		break;

	case RUM_COUPLE_INVITE_CANCEL :
		{
			ShowInviteCoupleDlg(false);
			return true;
		}
		break;
	case RUM_COUPLE_FAREWELL_OK :
		{
			CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;
			if ( pControl == NULL )
				return true;

			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );

			g_pData->GetServerProtocol()->SendID ( EGS_BREAK_UP_REQ ); 
			g_pMain->AddServerPacket( EGS_BREAK_UP_ACK, 60.f );
			return true;
		}
		break;

	case RUM_COUPLE_FAREWELL_CANCEL :	// 사용되고 있지 않음, 삭제 필요
		{
			return true;
		}
		break;


	case RUM_COUPLE_BUY_RING :
		{		
			g_pMain->KTDGUIOkAndCancelMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_24597),
				RUM_COUPLE_BUY_RING_OK, g_pMain->GetNowState() );
			return true;
		}
		break;
	
	case RUM_COUPLE_BUY_RING_OK :
		{
			CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;
			if ( pControl == NULL )
				return true;

			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, pControl->GetDialog(), NULL, false );

			//마을이 아니면 채팅 로그 추가
			if( g_pX2Game != NULL && g_pMain->GetNowStateID() != CX2Main::XS_BATTLE_FIELD )
			{
				g_pChatBox->AddChatLog(  GET_STRING( STR_ID_16478 ), KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
				return true;
			}
#ifdef CHECK_INHERIT_STATE_MENU
			if( g_pMain->IsInheritStateMenu() )
#endif //CHECK_INHERIT_STATE_MENU
			{
				CX2StateMenu* pStateMenu = static_cast<CX2StateMenu*>( g_pMain->GetNowState() );
				if( NULL != pStateMenu)
					pStateMenu->ToggleCashShop();
			}
			return true;
		}
		break;


	case RUM_COUPLE_BUY_RING_CANCEL : // 사용되고 있지 않음, 삭제 필요
		{
			return true;
		}
		break;



	case RUM_COUPLE_INVITED_OK :
		{
			CX2Main::TimedMessagePopUp::TimedPopupUserData userData;
			g_pMain->RemoveTimedMessagePopup( CX2Main::TimedMessagePopUp::MT_AGREE_COUPLE, userData );

			if ( NULL != g_pData->GetRelationshipManager() )
			{
				g_pData->GetRelationshipManager()->Send_EGS_COUPLE_PROPOSE_AGREE_NOT( CX2RelationshipManager::PRM_OK );
			}
			return true;

		}
		break;

	case RUM_RELATIONSHIP_REFRESH_DATA :
		{
			if ( NULL != g_pData->GetRelationshipManager() )
			{
				CX2RelationshipManager * pRelationshipManager = g_pData->GetRelationshipManager();
				if ( NULL != pRelationshipManager->GetMyRelationshipInfo() )
				{
					if ( pRelationshipManager->GetMyRelationshipInfo()->m_eRelationStateType !=
						SEnum::RT_SOLO )
					{
						if ( TRUE == pRelationshipManager->GetIsPossibleRefreshData() )
						{
							pRelationshipManager->SetIsPossibleRefreshData( FALSE );
							pRelationshipManager->Send_EGS_RELATIONSHIP_INFO_REQ();								
						}
					}
				}
			}
			return true;
		}
		break;

	case RUM_COUPLE_INVITED_CANCEL :
		{	
			CX2Main::TimedMessagePopUp::TimedPopupUserData userData;
			g_pMain->RemoveTimedMessagePopup( CX2Main::TimedMessagePopUp::MT_AGREE_COUPLE, userData );

			if ( NULL != g_pData->GetRelationshipManager() &&
				 NULL != g_pData->GetRelationshipManager()->GetMyRelationshipInfo() )
			{
				g_pData->GetRelationshipManager()->Send_EGS_COUPLE_PROPOSE_AGREE_NOT( CX2RelationshipManager::PRM_DISAGREE );
			}
			return true;
		}
		break;

	case RUM_RELATIONSHIP_FAREWELL :
		{
			g_pData->GetServerProtocol()->SendID ( EGS_CHECK_BREAK_UP_REQ );
			g_pMain->AddServerPacket( EGS_CHECK_BREAK_UP_ACK, 60.f );
			return true;
		}
		break;

	case RUM_RELATIONSHIP_PROPOSE :
		{
			if ( NULL != g_pData->GetRelationshipManager() )
			{
				// 커플 경과 시간 체크
				if ( true == g_pData->GetRelationshipManager()->CheckWeddingQualification() )
				{
                    CX2Item * pInviteLetterItem = NULL;
					// 결혼 반지 소지 여부 체크
					if ( NULL != g_pData->GetMyUser() &&
						NULL != g_pData->GetMyUser()->GetSelectUnit() &&
						NULL != ( pInviteLetterItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItemByTID ( WEDDING_RING_ITEM_ID) ) )
					{	
						// 초대권 소지 개수 체크
						int iWeddingLetterMany = pInviteLetterItem->GetItemData().m_Quantity;
					
						// 소지중엔 예약권으로 예식장 선택
						SetSelecteWeddingHallByPropose( iWeddingLetterMany );
					}
					else
					{// 결혼 반지 없을 때
						const CX2Item::ItemTemplet * pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( WEDDING_RING_ITEM_ID );
						if ( NULL != pItemTemplet )
						{			
							wstring wstrTextNotice= GET_REPLACED_STRING( ( STR_ID_24451, "S", pItemTemplet->GetName() ) );													
							g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), wstrTextNotice.c_str(), g_pMain->GetNowState() );
						}	
					}					
				}
			}			
			return true;
		}
		break;

	case RUM_WEDDING_INVITE_USE_ITEM_OK :
		{
			if ( NULL != g_pData->GetRelationshipManager() &&
				true == g_pData->GetRelationshipManager()->Handler_EGS_JOIN_WEDDING_HALL_REQ(m_UidSelectedIItemUID))
			{
				ShowJoinWeddingDlg( false );
			}
			return true;
		}
		break;

	case RUM_WEDDING_INVITE_USE_ITEM_CANCEL :
		{
			ShowJoinWeddingDlg( false );
			return true;
		}
		break;

	case RUM_WEDDING_INVITE_OK	   :
		{
			if ( NULL != g_pData->GetRelationshipManager() )
			{
				KEGS_WEDDING_PROPOSE_REQ kEvent;

				if ( m_pDLGInviteWedding != NULL )
				{
					CKTDGUIComboBox * pOfficiantCombo = static_cast<CKTDGUIComboBox*> ( m_pDLGInviteWedding->GetControl ( L"g_pComboHireNPC" ) );		
					if ( pOfficiantCombo != NULL )
					{
						kEvent.m_cOfficiantNPC = static_cast <SEnum::WEDDING_OFFICIANT_TYPE> ( pOfficiantCombo->GetSelectedItemIndex() );
					}

					CKTDGUIComboBox * pWeddingHallCombo = static_cast<CKTDGUIComboBox*> ( m_pDLGInviteWedding->GetControl ( L"g_pPickedWeddingHall" ) );		
					if ( pWeddingHallCombo != NULL )
					{
						kEvent.m_cWeddingHallType = static_cast <SEnum::WEDDING_HALL_TYPE> ( pWeddingHallCombo->GetSelectedItemIndex() );
					}

					CKTDGUIComboBox * pReserveTimeCombo = static_cast<CKTDGUIComboBox*> ( m_pDLGInviteWedding->GetControl ( L"g_pPickedWeddingDay" ) );		
					if ( pReserveTimeCombo != NULL )
					{
						CTime ctNowTime = g_pData->GetServerCurrentTime();
						CTime ctNowTimeOnStarted = CTime ( ctNowTime.GetYear(), ctNowTime.GetMonth(), ctNowTime.GetDay(), 0, 0, 0, 0 );
						CTimeSpan ctsSpandTime = CTimeSpan( pReserveTimeCombo->GetSelectedItemIndex(), 23, 59, 0 );
						CTime ctReserveTime = ctNowTime + ctsSpandTime;
						kEvent.m_tWeddingDate = static_cast<__int64> ( ctReserveTime.GetTime() );
					}


					CKTDGUIIMEEditBox* pIMEEditBox = static_cast<CKTDGUIIMEEditBox*> ( m_pDLGInviteWedding->GetControl( L"IME_Present_Message" ) );
					if ( pIMEEditBox != NULL )
					{
						kEvent.m_wstrWeddingMsg = pIMEEditBox->GetText();				
					}
				}

				BOOST_FOREACH ( const InviteWeddingUserInfo & InviteUserInfo, m_vecInvitedWeddingUserList )
					kEvent.m_vecInviteUnitList.push_back ( InviteUserInfo.m_uiUserUID );
				
#ifdef SERV_WEDING_PROPOSE_MSG_BUG_FIX
				replace(kEvent.m_wstrWeddingMsg.begin(), kEvent.m_wstrWeddingMsg.end(), L'\'', L'`');
#endif // SERV_WEDING_PROPOSE_MSG_BUG_FIX

				g_pData->GetRelationshipManager()->Send_EGS_WEDDING_PROPOSE_REQ ( kEvent );
			}
			ShowInviteWeddingDlg( false );
			return true;
		}
		break;

	case RUM_WEDDING_INVITE_CHANGE_WEDDING_HALL :
		{	

			if( NULL != g_pData && 
				NULL != g_pData->GetMyUser() &&
				NULL != g_pData->GetMyUser()->GetSelectUnit() )
			{
			    const CX2Inventory& kInventory = g_pData->GetMyUser()->GetSelectUnit()->GetInventory();

				// 초대권 개수 체크
				int iWeddingLetterMany = 0;
				CX2Item * pInviteLetterItem = kInventory.GetItemByTID( INVITE_WEDDING_ITEM_ID );
				if( NULL != pInviteLetterItem )
				{
					iWeddingLetterMany = pInviteLetterItem->GetItemData().m_Quantity;
				}

				// 선택 된 예식장 체크
				if( NULL != m_pDLGInviteWedding )
				{
					// 예식장 섬네일 끄기, 
					// 썸네일 픽쳐 인덱스의 시작과 끝 // 예식장이 추가되면 변경해줘야 함.
					const int iWeddingThumbnailNumStart = 9;
					const int iWeddingThumbnailNumEnd = 10;
					CKTDGUIStatic* pStaticThumbnail = static_cast<CKTDGUIStatic*> ( m_pDLGInviteWedding->GetControl ( L"g_pStaticCashShop_PresentWindow" ) );		
					if( NULL != pStaticThumbnail )
					{
						for( int i = iWeddingThumbnailNumStart; i <= iWeddingThumbnailNumEnd; ++i )
						{
							if( NULL != pStaticThumbnail->GetPicture( i ) )
							{
								pStaticThumbnail->GetPicture(i)->SetShow(false);
							}
						}
					}

					// 변경한 예약권에 맞게 설정 변경
					CKTDGUIComboBox * pCombo = 
						static_cast<CKTDGUIComboBox*> ( m_pDLGInviteWedding->GetControl ( L"g_pPickedWeddingHall" ) );	
					if( NULL != pCombo )
					{
						UINT iSelectedIndex = pCombo->GetSelectedItemIndex();
						// 전체 웨딩홀 개수보다 iSelectedIndex값이 크다면 에러.
						if( iSelectedIndex >= ARRAY_SIZE(ITEM_ID_RESERVED_WEDDING_HALL) )
						{
							ASSERT(!"WeddingHall UI Error ( Statlc : g_pPickedWeddingHall ");
							return true;
						}
						else
						{
							if( NULL != kInventory.GetItemByTID( ITEM_ID_RESERVED_WEDDING_HALL[iSelectedIndex] ) )
							{// 예약권이 존재 한다면

#ifdef ADDED_RELATIONSHIP_SYSTEM_BUG_FIX
								int iWeddingLetterManyByWeddingHall = _CONST_UI_RELATIONSHIP_::g_iMaxInvitedUser;

								switch(ITEM_ID_RESERVED_WEDDING_HALL[iSelectedIndex])
								{
								case 269712:/*루벤*/
									iWeddingLetterManyByWeddingHall = _CONST_UI_RELATIONSHIP_::g_iInvitingRubenWeddingHallMany;
									break;
								case 269713:/*샌더*/
									iWeddingLetterManyByWeddingHall = _CONST_UI_RELATIONSHIP_::g_iInvitingSanderWeddingHallMany;
									break;
								default:
									iWeddingLetterManyByWeddingHall = _CONST_UI_RELATIONSHIP_::g_iMaxInvitedUser;
									break;
								}

								// 최대 초대 가능 수는 30명으로 제한
								SetMaxWeddingInviteNumber ( min(iWeddingLetterManyByWeddingHall, iWeddingLetterMany) );

								// UI 표시도 변경
								if(m_pDLGInviteWedding != NULL)
								{
									CKTDGUIStatic * pStatic_Many = static_cast<CKTDGUIStatic*> ( m_pDLGInviteWedding->GetControl( L"g_pStatic_RemainingMany" ) );	
									wstringstream wstrUserMany;

									if ( NULL != g_pData->GetRelationshipManager() &&
										NULL != g_pData->GetRelationshipManager()->GetMyRelationshipInfo() )
									{
										CX2RelationshipManager::RelationshipInfo * pRelationshipInfo = g_pData->GetRelationshipManager()->GetMyRelationshipInfo();
										wstrUserMany << static_cast<int> ( m_vecInvitedWeddingUserList.size() ) << L" / " << GetMaxWeddingInviteNumber();
									}

									pStatic_Many->SetString( 0, wstrUserMany.str().c_str() );
								}
#else //ADDED_RELATIONSHIP_SYSTEM_BUG_FIX
								// 최대 초대 가능 수는 30명으로 제한
								SetMaxWeddingInviteNumber ( min(_CONST_UI_RELATIONSHIP_::g_iMaxInvitedUser, iWeddingLetterMany) );
#endif //ADDED_RELATIONSHIP_SYSTEM_BUG_FIX
								m_vecInviteWeddingUserList.clear();

								if( NULL != pStaticThumbnail &&
									NULL != pStaticThumbnail->GetPicture( iSelectedIndex + iWeddingThumbnailNumStart ))
								{
									pStaticThumbnail->GetPicture( iSelectedIndex + iWeddingThumbnailNumStart )->SetShow(true);
								}
							}
							else
							{
#ifdef SERV_RELATIONSHIP_SYSTEM_INT
								g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_28126 ), g_pMain->GetNowState() );
#else
								g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), L"해당 예식장을 예약하기 위한 아이템이 존재하지 않습니다.", g_pMain->GetNowState() );
#endif SERV_RELATIONSHIP_SYSTEM_INT

								// 소지중엔 예약권으로 예식장 선택
								if( false == SetSelecteWeddingHallByPropose( iWeddingLetterMany ) )
								{
									// 예약권이 하나도 없는데 청첩장이 열림.
									ASSERT(!" WeddingHall Error");
								}
							}
						}
					}
				}
			}	
			return true;
		}	
		break;

	case RUM_WEDDING_INVITE_CANCEL :
		{
			ShowInviteWeddingDlg(false);
			return true;
		}
		break;
	case RUM_WEDDING_INVITE_ADD_INVITE_USER_ID :
		{	

			
			CKTDGUIIMEEditBox* pIMEEditBox = static_cast<CKTDGUIIMEEditBox*> ( m_pDLGInviteWedding->GetControl( L"IME_Nicname" ) );
			wstring wstrInputNickName = pIMEEditBox->GetText ();

			if ( NULL != g_pData->GetMyUser() &&
				 NULL != g_pData->GetMyUser()->GetSelectUnit() &&
				 NULL != g_pData->GetRelationshipManager() &&
				 NULL != g_pData->GetRelationshipManager()->GetMyRelationshipInfo() )
			{
				if ( wstrInputNickName == g_pData->GetMyUser()->GetSelectUnit()->GetNickName() )
				{
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING ( STR_ID_24495 ), g_pMain->GetNowState() );
				}
				

				else if ( wstrInputNickName == g_pData->GetRelationshipManager()->GetMyRelationshipInfo()->m_wstrRelationTargetUserNickname)
				{
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING ( STR_ID_24579 ), g_pMain->GetNowState() );					
				}
				else if ( wstrInputNickName.size() > 0 )
				{
					if ( NULL != g_pData->GetRelationshipManager() )
					{
						g_pData->GetRelationshipManager()->Send_EGS_CHECK_NICK_NAME_REQ( wstrInputNickName );
					}
				}
				else
				{
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING ( STR_ID_23609 ), g_pMain->GetNowState() );					
				}
			}
			return true;
		}
		break;
	

	case RUM_WEDDING_INVITE_POP_FRIEND_LIST :
		{
			GetFriendListForInvitingWeddingUser();
			SetNowInviteUserPage ( 1 );
			ShowInviteWeddingUserListDlg(true, false, RIT_FRIEND);
			return true;
		}
		break;

	case RUM_WEDDING_INVITE_POP_GUILD_LIST :
		{
			SetNowInviteUserPage ( 1 );
			if ( NULL != g_pData->GetRelationshipManager() )
			{	
				if ( NULL != g_pData->GetGuildManager() &&
					 NULL != g_pData->GetGuildManager()->GetMyGuildInfo() && 
					 NULL != g_pData->GetGuildManager()->GetMyGuildInfo()->m_iGuildUID > 0 )
				{
					g_pData->GetRelationshipManager()->Send_EGS_GET_GUILD_USER_LIST_REQ();
					ShowInviteWeddingUserListDlg(true, false, RIT_GUILD);
				}
				else
				{
					// STR_ID_24599
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999),  GET_STRING ( STR_ID_24599 ), g_pMain->GetNowState() );
				}
			}
			return true;
		}
		break;

	case RUM_WEDDING_INVITE_POP_FRIEND_LIST_OK :
		{
			ShowInviteWeddingUserListDlg(false, false, RIT_FRIEND);
			return true;
		}
		break;

	case RUM_WEDDING_INVITE_POP_FRIEND_LIST_CANCEL :
		{		
			ShowInviteWeddingUserListDlg(false, false, RIT_FRIEND);
			return true;
		}
		break;

	case RUM_WEDDING_INVITE_POP_ADD_LIST :
		{
			SetNowInviteUserPage ( 1 );
			ShowInviteWeddingUserListDlg (true, true, RIT_ADDED );
			return true;
		}
		break;

	case RUM_WEDDING_INVITE_POP_ADD_LIST_OK :
		{
			CKTDGUICheckBox * pCheckBox = static_cast <CKTDGUICheckBox * > ( m_pDLGInviteWeddingUserList->GetControl( L"FieldSD") );
			if ( NULL != pCheckBox )
				pCheckBox->SetChecked( false );

			ShowInviteWeddingUserListDlg (false, true, RIT_ADDED );
			return true;
		}
		break;


	case RUM_WEDDING_INVITE_USER_LIST_LEFT_ARROW :
		{
			m_uiNowInviteUserPage -= 1;
			if ( m_uiNowInviteUserPage < 1 )
				m_uiNowInviteUserPage = 1;
			else
			{
				switch ( m_eRelationInviteUserListType ) 
				{
					case RIT_FRIEND :
					{
						if ( NULL != g_pData->GetRelationshipManager() )
						{							
							GetInvitingWeddingUserListFromFriendList( GetNowInviteUserPage() - 1);
							ShowInvitedWeddingUserListControlListDlg ( false );
							ShowInvitedWeddingUserListControlListDlg ( true );
						}
					}
					break;
					case RIT_GUILD : 
					{
						if ( NULL != g_pData->GetRelationshipManager() )
						{
							g_pData->GetRelationshipManager()->Send_EGS_GET_GUILD_USER_LIST_REQ();
						}
					}
					break;
					case RIT_ADDED :
					{
						if ( NULL != g_pData->GetRelationshipManager() )
						{
							ShowInvitedWeddingUserListControlListDlg ( false );
							ShowInvitedWeddingUserListControlListDlg ( true );
						}
					}
					break;
				}
			}
			return true;
		}
		break;

	case RUM_WEDDING_INVITE_USER_LIST_RIGHT_ARROW :
		{
			CKTDGUICheckBox * pCheckBox = static_cast <CKTDGUICheckBox * > ( m_pDLGInviteWeddingUserList->GetControl( L"FieldSD") );
			if ( NULL != pCheckBox )
				pCheckBox->SetChecked( false );

			m_uiNowInviteUserPage += 1;
			if ( m_uiNowInviteUserPage > GetMaxInviteUserPage() )
				m_uiNowInviteUserPage = GetMaxInviteUserPage();
			else
			{
				switch ( m_eRelationInviteUserListType ) 
				{
				case RIT_FRIEND :
					{
						GetInvitingWeddingUserListFromFriendList( GetNowInviteUserPage() - 1);
						ShowInvitedWeddingUserListControlListDlg ( false );
						ShowInvitedWeddingUserListControlListDlg ( true );
					}
					break;
				case RIT_GUILD : 
					{
						if ( NULL != g_pData->GetRelationshipManager() )
						{
							g_pData->GetRelationshipManager()->Send_EGS_GET_GUILD_USER_LIST_REQ();
						}
					}
					break;
				case RIT_ADDED :
					{
						if ( NULL != g_pData->GetRelationshipManager() )
						{
							ShowInvitedWeddingUserListControlListDlg ( false );
							ShowInvitedWeddingUserListControlListDlg ( true );
						}
					}
					break;
				}
			}
			return true;
		}
		break;

	case RUM_WEDDING_INVITE_USER_LIST_ALL_CHECKED :
		{
			CKTDGUICheckBox * pCheckBox = static_cast <CKTDGUICheckBox * > ( m_pDLGInviteWeddingUserList->GetControl( L"FieldSD") );
			if ( NULL != pCheckBox )
				pCheckBox->SetChecked( false );

			m_pDLGInviteWeddingUserList->OpenScriptFile(L"DLG_UI_Invite_Wedding_ControlList.lua");
			for(int i = 0; i < (int) _CONST_UI_RELATIONSHIP_::g_iMaxInviteUserViewCount; ++i)
			{
				if ( i >= (int) m_vecInviteWeddingUserList.size() )		
				{
					break;
				}

				WCHAR wFriendID[256] = {0,};
				
				StringCchPrintf(wFriendID, 256, L"Map_CheckBox_%d", i);
				CKTDGUICheckBox *pCheckBox = (CKTDGUICheckBox *)m_pDLGInviteWeddingUserList->GetControl(wFriendID );	
				
				if ( FALSE == pCheckBox->GetChecked() )
				{
					CheckWeddingInviteUserList ( pCheckBox );
				}	
			}
			CKTDGUICheckBox* pMainCheckBox = reinterpret_cast<CKTDGUICheckBox *> ( lParam );
			pMainCheckBox->SetChecked(true);
			return true;
		}
		break;

	case RUM_WEDDING_INVITE_USER_LIST_ALL_UNCHECKED :
		{
			m_pDLGInviteWeddingUserList->OpenScriptFile(L"DLG_UI_Invite_Wedding_ControlList.lua");
			for(int i = 0; i < (int) _CONST_UI_RELATIONSHIP_::g_iMaxInviteUserViewCount; ++i)
			{
				if ( i >= (int) m_vecInviteWeddingUserList.size() )		
				{
					break;
				}

				WCHAR wFriendID[256] = {0,};

				StringCchPrintf(wFriendID, 256, L"Map_CheckBox_%d", i);
				CKTDGUICheckBox *pCheckBox = (CKTDGUICheckBox *)m_pDLGInviteWeddingUserList->GetControl(wFriendID );

				if ( TRUE == pCheckBox->GetChecked() )
				{
					UnCheckWeddingInviteUserList ( pCheckBox );
				}	
			}
			CKTDGUICheckBox* pMainCheckBox = reinterpret_cast<CKTDGUICheckBox *> ( lParam );
			pMainCheckBox->SetChecked(false);
			return true;
		}
		break;

	case RUM_WEDDING_INVITE_USER_LIST_CHECKED :
		{
			CKTDGUICheckBox* pCheckBox = reinterpret_cast<CKTDGUICheckBox *> ( lParam );
			CheckWeddingInviteUserList( pCheckBox );
			return true;
		}
		break;

	case RUM_WEDDING_INVITE_USER_LIST_UNCHECKED :
		{
			CKTDGUICheckBox* pCheckBox = reinterpret_cast<CKTDGUICheckBox *> ( lParam );
			UnCheckWeddingInviteUserList( pCheckBox );
			return true;
		}
		break;
	}
	return false;
}


void CX2UIRelationship::GetFriendListForInvitingWeddingUser ()
{
#ifdef FIXED_RELATIONSHIP_SYSTEM	
	m_vecFriendMemberInfoList.clear();
	// 친구 초대하는 목록이 이상이 있으면 아래 이 소스에 문제가 있는겁니다
	// 넥슨 모듈 사용
	if ( NULL != g_pData->GetMessenger() )
	{
		map<wstring, UidType> mapFriendNameListAndUID;
		g_pData->GetMessenger()->GetFriendNameListAndUID( mapFriendNameListAndUID );		
		
		map<wstring, UidType>::iterator it = mapFriendNameListAndUID.begin();

		for ( ; it != mapFriendNameListAndUID.end(); ++it )
		{
			m_vecFriendMemberInfoList.push_back( InviteWeddingUserInfo ( it->first, it->second ) );
		}
	}

#else // FIXED_RELATIONSHIP_SYSTEM
	m_vecFriendMemberInfoList.clear();
	//	친구 초대하는 목록이 이상이 있으면 아래 이 소스에 문제가 있는겁니다
	if ( NULL != g_pData->GetMessenger() )
	{
		vector <wstring> mapFriendName;
		g_pData->GetMessenger()->GetFriendNameList( mapFriendName );
		vector<wstring>::iterator it = mapFriendName.begin();
		for ( ; it != mapFriendName.end(); ++it )
		{
			UidType FriendUid = g_pData->GetMessenger()->GetFriendUID(*it);
			if ( FriendUid != 0 )
			{
				m_vecFriendMemberInfoList.push_back ( InviteWeddingUserInfo ( *it, FriendUid ) );
			}
		}
	}
#endif // FIXED_RELATIONSHIP_SYSTEM
}

void CX2UIRelationship::GetInvitingWeddingUserListFromFriendList ( int idx )
{
	ResetInviteWeddingUserList();
#ifdef FIXED_RELATIONSHIP_SYSTEM
	for (int i = idx * _CONST_UI_RELATIONSHIP_::g_iMaxInviteUserViewCount; i < static_cast<int> ( m_vecFriendMemberInfoList.size() ); ++i )
	{
		m_vecInviteWeddingUserList.push_back( m_vecFriendMemberInfoList[i] );
	}

#else // FIXED_RELATIONSHIP_SYSTEM
	for (int i = 0; i < _CONST_UI_RELATIONSHIP_::g_iMaxInviteUserViewCount; ++i )
	{
		int iPhase = idx * _CONST_UI_RELATIONSHIP_::g_iMaxInviteUserViewCount + i;
		if ( iPhase <= static_cast<int> ( m_vecFriendMemberInfoList.size() ) - 1 )
		{
			m_vecInviteWeddingUserList.push_back( m_vecFriendMemberInfoList[iPhase] );
		}
		else
		{
			break;
		}
	}
#endif // FIXED_RELATIONSHIP_SYSTEM

}

void CX2UIRelationship::UnCheckWeddingInviteUserList ( CKTDGUICheckBox* pCheckBox_ )
{
	CKTDGUIStatic * pStatic_Many = (CKTDGUIStatic*)m_pDLGInviteWedding->GetControl( L"g_pStatic_RemainingMany" );	
	CKTDGUIStatic * pStatic_ManyUserList = (CKTDGUIStatic*)m_pDLGInviteWeddingUserList->GetControl( L"InvitingNumber" );	
	int iInviteUserArrNum =  pCheckBox_->GetDummyInt(0);

	if ( NULL != g_pData->GetRelationshipManager() &&
		NULL != g_pData->GetRelationshipManager()->GetMyRelationshipInfo() )
	{
		CX2RelationshipManager::RelationshipInfo * pRelationshipInfo = g_pData->GetRelationshipManager()->GetMyRelationshipInfo();
		int iPossibleInviteMany = GetMaxWeddingInviteNumber();

		if ( m_eRelationInviteUserListType == RIT_ADDED )
		{	
			if ( pStatic_Many->GetDummyInt(0) + 1 <= iPossibleInviteMany )
				pStatic_Many->SetDummyInt(0, pStatic_Many->GetDummyInt(0) + 1 );
			pCheckBox_->SetChecked( false );
		}
		

		else
		{
			vector<InviteWeddingUserInfo>::iterator it = m_vecInviteWeddingUserList.begin() + iInviteUserArrNum;
			vector<InviteWeddingUserInfo>::iterator itUncheckedContainer = m_vecInvitedWeddingUserList.begin();

			for ( ; itUncheckedContainer != m_vecInvitedWeddingUserList.end(); ++itUncheckedContainer )
			{
				if ( it->m_uiUserUID == itUncheckedContainer->m_uiUserUID )
				{
					m_vecInvitedWeddingUserList.erase ( itUncheckedContainer );
					pCheckBox_->SetChecked ( false );
					break;
				}
			}
		}
		wstringstream wstrUserMany;
		wstrUserMany << static_cast<int> ( m_vecInvitedWeddingUserList.size() ) << L" / " << iPossibleInviteMany;
		pStatic_Many->SetString( 0, wstrUserMany.str().c_str() );

		pStatic_ManyUserList->SetString ( 0, wstrUserMany.str().c_str() );
	}
}
void CX2UIRelationship::CheckWeddingInviteUserList ( CKTDGUICheckBox* pCheckBox_ )
{

	CKTDGUIStatic * pStatic_Many = (CKTDGUIStatic*)m_pDLGInviteWedding->GetControl( L"g_pStatic_RemainingMany" );	
	CKTDGUIStatic * pStatic_ManyUserList = (CKTDGUIStatic*)m_pDLGInviteWeddingUserList->GetControl( L"InvitingNumber" );	

	if ( NULL != pStatic_Many )
	{
		if ( NULL != g_pData->GetRelationshipManager() &&
			NULL != g_pData->GetRelationshipManager()->GetMyRelationshipInfo() )
		{
			CX2RelationshipManager::RelationshipInfo * pRelationshipInfo = g_pData->GetRelationshipManager()->GetMyRelationshipInfo();
			int iPossibleInviteMany = GetMaxWeddingInviteNumber();

			switch ( m_eRelationInviteUserListType  )
			{
			case RIT_ADDED :
				{
					pStatic_Many->SetDummyInt(0, pStatic_Many->GetDummyInt(0) - 1 );
					pCheckBox_->SetChecked ( true );
				}
				break;

			case RIT_FRIEND :
			case RIT_GUILD :
				{
					int iInviteUserArrNum =  pCheckBox_->GetDummyInt(0);

					if ( NULL != g_pData->GetRelationshipManager() &&
						NULL != g_pData->GetRelationshipManager()->GetMyRelationshipInfo() )
					{
						vector<InviteWeddingUserInfo>::iterator it = m_vecInviteWeddingUserList.begin() + iInviteUserArrNum;

						if ( static_cast<int> ( m_vecInvitedWeddingUserList.size() ) + 1 > iPossibleInviteMany )
						{
							g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999),GET_STRING ( STR_ID_24580 ), g_pMain->GetNowState() );
							pCheckBox_->SetChecked ( false );
							break;
						}
						else if ( it->m_uiUserUID == g_pData->GetRelationshipManager()->GetMyRelationshipInfo()->m_iRelationTargetUserUid )
						{
							g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING ( STR_ID_24579 ), g_pMain->GetNowState() );
							pCheckBox_->SetChecked ( false );
							break;
						}
						else
						{
							pCheckBox_->SetChecked ( true );
							BOOST_FOREACH ( const InviteWeddingUserInfo & InvitedUserInfo , m_vecInvitedWeddingUserList )
							{
								if ( InvitedUserInfo.m_uiUserUID == it->m_uiUserUID )
								{
									if ( InvitedUserInfo.m_uiUserUID == g_pData->GetMyUser()->GetUID() )
									{
										g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING ( STR_ID_24496 ), g_pMain->GetNowState() );
										pCheckBox_->SetChecked ( false );
										break;
									}
								}

								if ( InvitedUserInfo.m_uiUserUID == g_pData->GetMyUser()->GetUID() )
								{
									g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999), GET_STRING ( STR_ID_24495 ), g_pMain->GetNowState() );
									pCheckBox_->SetChecked ( false );
									break;
								}								
							}

							if ( TRUE == pCheckBox_->GetChecked () )
							{
								m_vecInvitedWeddingUserList.push_back( InviteWeddingUserInfo ( it->m_wstrUserName, it->m_uiUserUID ) );
							}
						}
					}	
				}
				break;
			}
		
			wstringstream wstrUserMany;
			wstrUserMany << static_cast<int> ( m_vecInvitedWeddingUserList.size() ) << L" / " << iPossibleInviteMany;
			pStatic_Many->SetString( 0, wstrUserMany.str().c_str() );
			pStatic_ManyUserList->SetString ( 0, wstrUserMany.str().c_str() );
		}
	}
}
bool CX2UIRelationship::UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{

	return false;
}

bool CX2UIRelationship::ShowInvitedWeddingUserListControlListDlg ( bool bEnable )
{
	if ( NULL != m_pDLGInviteWeddingUserList )
	{
		CKTDGUIStatic * pStatic_Many = NULL;
		CKTDGUIStatic * pStatic_ManyUserList = (CKTDGUIStatic*)m_pDLGInviteWeddingUserList->GetControl( L"InvitingNumber" );	
		CKTDGUIContolList *pControlList = static_cast<CKTDGUIContolList*> ( m_pDLGInviteWeddingUserList->GetControl(L"PvpMapControlList") );

		if ( NULL != g_pData->GetRelationshipManager() &&
			NULL != g_pData->GetRelationshipManager()->GetMyRelationshipInfo() )
		{
			CX2RelationshipManager::RelationshipInfo * pRelationshipInfo = g_pData->GetRelationshipManager()->GetMyRelationshipInfo();
			pStatic_Many = (CKTDGUIStatic*)m_pDLGInviteWedding->GetControl( L"g_pStatic_RemainingMany" );	

		}
		
		if( pControlList != NULL && pStatic_Many != NULL )
		{
			if ( TRUE == bEnable )
			{
				if ( NULL != g_pData->GetRelationshipManager() &&
					 NULL != g_pData->GetRelationshipManager()->GetMyRelationshipInfo() )
				{
					CKTDGUIComboBox * pComboBox = static_cast<CKTDGUIComboBox*> ( m_pDLGInviteWedding->GetControl( L"g_pComboInviteUserList" ) );

					m_pDLGInviteWeddingUserList->OpenScriptFile(L"DLG_UI_Invite_Wedding_ControlList.lua");
					for(int i=0; i<(int) _CONST_UI_RELATIONSHIP_::g_iMaxInviteUserViewCount; ++i)
					{
						WCHAR wFriendID[256] = {0,};
						StringCchPrintf(wFriendID, 256, L"Map_Static_%d", i);
						CKTDGUIStatic *pStatic = (CKTDGUIStatic*)m_pDLGInviteWeddingUserList->GetControl( wFriendID );	

						StringCchPrintf(wFriendID, 256, L"Map_Button_%d", i);
						CKTDGUIButton *pButton = (CKTDGUIButton*)m_pDLGInviteWeddingUserList->GetControl(wFriendID );

						StringCchPrintf(wFriendID, 256, L"Map_CheckBox_%d", i);
						CKTDGUICheckBox *pCheckBox = (CKTDGUICheckBox *)m_pDLGInviteWeddingUserList->GetControl(wFriendID );	
						pStatic->SetString ( 0, L" " );
						pStatic->SetShowEnable( false, false );
						if ( NULL != pStatic->GetPicture( 0 ) )
							pStatic->GetPicture( 0 )->SetShow (false);
						pButton->SetShowEnable( false, false );

						pCheckBox->SetShowEnable( false, false );
						pCheckBox->SetChecked( false );

						int iPossibleInviteMany = GetMaxWeddingInviteNumber();
						CX2RelationshipManager::RelationshipInfo * pRelationshipInfo = g_pData->GetRelationshipManager()->GetMyRelationshipInfo();
						wstringstream wstrUserMany;
						wstrUserMany << static_cast<int> ( m_vecInvitedWeddingUserList.size() )  << L" / " << iPossibleInviteMany;
						pStatic_Many->SetString( 0, wstrUserMany.str().c_str() );

						pStatic_ManyUserList->SetString ( 0, wstrUserMany.str().c_str() );
					}
				}

				m_pDLGInviteWeddingUserList->OpenScriptFile(L"DLG_UI_Invite_Wedding_ControlList.lua");
				for(int i=0; i<(int) _CONST_UI_RELATIONSHIP_::g_iMaxInviteUserViewCount; ++i)
				{
					int iIndexNumber = 0;
					if ( m_eRelationInviteUserListType == RIT_ADDED )
						iIndexNumber = i + ( GetNowInviteUserPage() - 1 ) * 8;
					else
						iIndexNumber = i;

					if ( iIndexNumber < static_cast<int> ( m_vecInviteWeddingUserList.size() ) )
					{
						WCHAR wFriendID[256] = {0,};

						StringCchPrintf(wFriendID, 256, L"Map_Static_%d", i);
						CKTDGUIStatic *pStatic = (CKTDGUIStatic*)m_pDLGInviteWeddingUserList->GetControl( wFriendID );	

						StringCchPrintf(wFriendID, 256, L"Map_Button_%d", i);
						CKTDGUIButton *pButton = (CKTDGUIButton*)m_pDLGInviteWeddingUserList->GetControl(wFriendID );

						StringCchPrintf(wFriendID, 256, L"Map_CheckBox_%d", i);
						CKTDGUICheckBox *pCheckBox = (CKTDGUICheckBox *)m_pDLGInviteWeddingUserList->GetControl(wFriendID );	


						vector<InviteWeddingUserInfo>::iterator it = m_vecInvitedWeddingUserList.begin();

						if ( m_eRelationInviteUserListType != RIT_ADDED )
						{
							for ( ; it != m_vecInvitedWeddingUserList.end(); ++it )
							{
								if ( it->m_uiUserUID == m_vecInviteWeddingUserList[iIndexNumber].m_uiUserUID )
								{
									pCheckBox->SetChecked(true);
								}
							}
						}

						pStatic->SetString ( 0, m_vecInviteWeddingUserList[iIndexNumber].m_wstrUserName.c_str() );					
						pStatic->SetShowEnable ( true, true );
						if ( NULL != pStatic->GetPicture( 0 ) )
							pStatic->GetPicture( 0 )->SetShow (true);
						pButton->SetShowEnable ( true, true );
						pCheckBox->SetShowEnable ( true, true );
					}
					
					else
					{
						break;	
					}
				}
				pControlList->SetShow ( true );
				return true;
			}

			else
			{
				pControlList->SetShow ( false );
				return true;
			}
		}
	}
	return false;
}

bool CX2UIRelationship::ShowInviteWeddingUserListDlg  ( bool bEnable, bool bIsSave, RELATION_INVITE_USER_LIST_TYPE eType_ )
{
	if(bEnable == true)
	{
		m_pDLGInviteWeddingUserList = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Invite_Wedding_User_List.lua" );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGInviteWeddingUserList );
		m_pDLGInviteWeddingUserList->SetDisableUnderWindow( true );

		CKTDGUIButton *pOkButton = (CKTDGUIButton*)m_pDLGInviteWeddingUserList->GetControl( L"OK" );
		pOkButton->SetShowEnable(false, false);

		CKTDGUIButton *pDeleteButton = (CKTDGUIButton*)m_pDLGInviteWeddingUserList->GetControl( L"DELETE" );
		pDeleteButton->SetShowEnable(false, false);

		CKTDGUIContolList *pControlList = (CKTDGUIContolList*)m_pDLGInviteWeddingUserList->GetControl(L"PvpMapControlList");

		CKTDGUIStatic *pType = (CKTDGUIStatic*)m_pDLGInviteWeddingUserList->GetControl( L"InvitingType" );
		pType->SetShowEnable(true, true);
 
// 		

		for(int i=0; i< _CONST_UI_RELATIONSHIP_::g_iMaxInviteUserViewCount; ++i)
		{
			m_pDLGInviteWeddingUserList->OpenScriptFile(L"DLG_UI_Invite_Wedding_ControlList.lua");

			CKTDGUIButton *pButton = (CKTDGUIButton*)m_pDLGInviteWeddingUserList->GetControl( L"ListControl_Button" );
			CKTDGUIStatic *pStatic = (CKTDGUIStatic*)m_pDLGInviteWeddingUserList->GetControl( L"MAP_NAME" );	
			CKTDGUICheckBox *pCheckBox = (CKTDGUICheckBox *)m_pDLGInviteWeddingUserList->GetControl( L"Map_CheckBox" );

			WCHAR wFriendID[256] = {0,};
			StringCchPrintf(wFriendID, 256, L"Map_Static_%d", i);
			pStatic->SetName( wFriendID );
			pStatic->AddDummyInt(i);
			pStatic->GetString(0)->msg = L" "; //m_vecInviteWeddingUserList[i].m_wstrUserName.c_str();
			pControlList->InsertItem( pStatic,	i, 0 );

			StringCchPrintf(wFriendID, 256, L"Map_Button_%d", i);
			pButton->SetName( wFriendID );
			pButton->AddDummyInt(i);
			pButton->SetDownStateAtNormal(false);
			pControlList->InsertItem( pButton,	i, 1 );

			StringCchPrintf(wFriendID, 256, L"Map_CheckBox_%d", i);

			pCheckBox->SetName( wFriendID );
			pCheckBox->AddDummyInt(i);
			pCheckBox->SetChecked ( false );
			pCheckBox->SetCustomMsgChecked( static_cast<int> ( RUM_WEDDING_INVITE_USER_LIST_CHECKED ) );
			pCheckBox->SetCustomMsgUnChecked( static_cast<int> ( RUM_WEDDING_INVITE_USER_LIST_UNCHECKED ) );
			pControlList->InsertItem( pCheckBox,i, 2 );
		}

		pControlList->SetIndex( 0, 0 );
		pControlList->SetShow ( false );
		if( pControlList != NULL )
		{
			m_eRelationInviteUserListType = eType_;
			m_vecInviteWeddingUserList.clear();
			
			switch ( eType_)
			{
				case RIT_FRIEND :
					SetMaxInviteUserPage ( (int) ( m_vecFriendMemberInfoList.size() - 1 ) / 8 + 1 );
					GetInvitingWeddingUserListFromFriendList( GetNowInviteUserPage() - 1 );
					ShowInvitedWeddingUserListControlListDlg(true);					
					pOkButton->SetShowEnable( true, true );
					break;
				case RIT_GUILD :
					pOkButton->SetShowEnable( true, true );
					break;

				case RIT_ADDED :
					if ( false == m_vecInvitedWeddingUserList.empty() )
					{
						m_vecInviteWeddingUserList = m_vecInvitedWeddingUserList;
					}
					SetMaxInviteUserPage ( (int) ( m_vecInviteWeddingUserList.size() - 1 ) / 8 + 1 );
					ShowInvitedWeddingUserListControlListDlg(true);
					pDeleteButton->SetShowEnable( true , true );
					break;
			}

		
		//	ShowInvitedWeddingUserListControlListDlg ( true );
		}
		return true;
	}
	
	else
	{
		if(m_pDLGInviteWeddingUserList != NULL)
		{
			if ( TRUE == bIsSave )
			{
				switch ( eType_)
				{
					case RIT_FRIEND :
					{
					}
					break;

					case RIT_GUILD :
					{
					}
					break;

					case RIT_ADDED :
					{
						m_vecInvitedWeddingUserList.clear();

						CKTDGUIStatic * pStatic_Many = (CKTDGUIStatic*)m_pDLGInviteWedding->GetControl( L"g_pStatic_RemainingMany" );	
						CKTDGUIComboBox * pComboBox = (CKTDGUIComboBox*)m_pDLGInviteWedding->GetControl( L"g_pComboInviteUserList" );
						
						int iFixedValue = 0;
						m_pDLGInviteWeddingUserList->OpenScriptFile(L"DLG_UI_Invite_Wedding_ControlList.lua");
						
						for(int i = 0; i < _CONST_UI_RELATIONSHIP_::g_iMaxInviteUserViewCount; ++i)
						{
							WCHAR wFriendID[256] = {0,};
							int iArrayNumber = GetNowUserButtonIndexNumber (i); 
							
							if ( iArrayNumber - iFixedValue >= (int) m_vecInviteWeddingUserList.size() )		
							{
								break;
							}

							StringCchPrintf(wFriendID, 256, L"Map_Static_%d", i);

							CKTDGUIStatic *pStatic = (CKTDGUIStatic*)m_pDLGInviteWeddingUserList->GetControl( wFriendID );	

							StringCchPrintf(wFriendID, 256, L"Map_Button_%d", i);

							CKTDGUIButton *pButton = (CKTDGUIButton*)m_pDLGInviteWeddingUserList->GetControl( wFriendID );

							StringCchPrintf(wFriendID, 256, L"Map_CheckBox_%d", i);
							CKTDGUICheckBox *pCheckBox = (CKTDGUICheckBox *)m_pDLGInviteWeddingUserList->GetControl( wFriendID );
			
							if ( TRUE == pCheckBox->GetChecked() )
							{
								m_vecInviteWeddingUserList.erase( m_vecInviteWeddingUserList.begin() + iArrayNumber - iFixedValue );
								pCheckBox->SetChecked(false);
								iFixedValue += 1;
							}
						}
						m_vecInvitedWeddingUserList = m_vecInviteWeddingUserList;
						ShowInvitedWeddingUserListControlListDlg ( true );
//						ShowInviteWeddingUserListDlg ( true, true, RIT_ADDED );
//						ResetInvitingUserInfo ();
						return true;
					}
					break;
				}
			}

			else
			{
				if ( NULL != g_pData->GetRelationshipManager() &&
					NULL != g_pData->GetRelationshipManager()->GetMyRelationshipInfo() )
				{
					CX2RelationshipManager::RelationshipInfo * pRelationshipInfo = g_pData->GetRelationshipManager()->GetMyRelationshipInfo();
					CKTDGUIStatic * pStatic_Many = (CKTDGUIStatic*)m_pDLGInviteWedding->GetControl( L"g_pStatic_RemainingMany" );	
					CKTDGUIStatic * pStatic_ManyUserList = (CKTDGUIStatic*)m_pDLGInviteWeddingUserList->GetControl( L"InvitingNumber" );	


					if ( NULL != pStatic_Many )
					{
						int iPossibleInviteMany = GetMaxWeddingInviteNumber();
						wstringstream wstrUserMany;
						wstrUserMany << static_cast<int> ( m_vecInvitedWeddingUserList.size() )  << L" / " << iPossibleInviteMany;
						pStatic_Many->SetString( 0, wstrUserMany.str().c_str() );

						pStatic_ManyUserList->SetString ( 0, wstrUserMany.str().c_str() );

					}
				}
			}
		}
		g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGInviteWeddingUserList, NULL, false );
		m_pDLGInviteWeddingUserList = NULL;
		

		return true;
	}
		

	return false;
}

void CX2UIRelationship::ShowChangeLoveMessageDlg( bool bEnable, UidType ItemUid_ )
{
	if (bEnable == true)
	{
		m_UidSelectedIItemUID = ItemUid_;
		m_pDLGChangeLoveMessage = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Use_Change_Love_Message.lua" );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGChangeLoveMessage );
	}
	else
	{
		if(m_pDLGChangeLoveMessage != NULL)
		{
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGChangeLoveMessage, NULL, false );
			m_pDLGChangeLoveMessage = NULL;
		}
	}
}

bool CX2UIRelationship::ShowInviteCoupleDlg( bool bEnable )
{
	if (bEnable == true)
	{
		m_pDLGInviteCoupleMessage = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Use_Couple_Ring.lua" );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGInviteCoupleMessage );

		return true;
	}
	else
	{
		if(m_pDLGInviteCoupleMessage != NULL)
		{
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGInviteCoupleMessage, NULL, false );
			m_pDLGInviteCoupleMessage = NULL;
		}
		return true;
	}

	return false;
}


bool CX2UIRelationship::ShowJoinWeddingDlg( bool bEnable, bool bIsWeddingHeroine /*= false*/, UidType itemUID_ /*= 0*/)
{
	if(bEnable == true )
	{
		if ( itemUID_ > 0 )
		{
			if ( NULL != g_pData->GetRelationshipManager() )
			{	
				if( true == bIsWeddingHeroine &&
					false == g_pData->GetRelationshipManager()->IsEqipWeddingClothes() )
				{
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_24481 ), g_pMain->GetNowState() );
					// 마을에서는 아이템 착용을 하지 않아도 입장 할 수 있도록 예외처리.
					return false;
				}

				const CX2RelationshipManager::WeddingItemInfo * pWeddingItemInfo = g_pData->GetRelationshipManager()->GetWeddingInviteLetterInfoMapByItemUID ( itemUID_ );
				if ( NULL != pWeddingItemInfo )
				{
					m_pDLGJoinWedding = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Join_Wedding.lua" );
					g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGJoinWedding );

					m_UidSelectedIItemUID = itemUID_;

					CKTDGUIStatic * pStatic_Boy = static_cast<CKTDGUIStatic*> ( m_pDLGJoinWedding->GetControl( L"g_pStaticpost_window_Font3" ) );	
					pStatic_Boy->GetString(0)->msg = pWeddingItemInfo->GetWeddingMaleNickname();

					CKTDGUIStatic * pStatic_Girl = static_cast<CKTDGUIStatic*> ( m_pDLGJoinWedding->GetControl( L"g_pStaticGirlFont" ) );	
					pStatic_Girl->GetString(0)->msg = pWeddingItemInfo->GetWeddingFemaleNickname();
			
					CKTDGUIStatic * pStatic_WeddingDate = static_cast<CKTDGUIStatic*> ( m_pDLGJoinWedding->GetControl( L"g_pStaticpost_window_Font2" ) );	
					wstring wstrWeddingDate = pWeddingItemInfo->m_wstrWeddingDate;

					CTime ctWeddingDate;
					KncUtil::ConvertStringToCTime ( wstrWeddingDate, ctWeddingDate );

					CTime ctNowTime = CTime::GetCurrentTime ();			
				

					CTimeSpan ctpRemainingTime = ctWeddingDate - ctNowTime;
		
					CKTDGUIStatic * pStatic_Picture = static_cast<CKTDGUIStatic*> ( m_pDLGJoinWedding->GetControl( L"g_pStaticpaper_window" ) );	
					
					if ( NULL != pStatic_Picture->GetPicture(1) &&
						 NULL != pStatic_Picture->GetPicture(2) )
					{
						pStatic_Picture->GetPicture(1)->SetShow ( false );
						pStatic_Picture->GetPicture(2)->SetShow ( false );
						if ( pWeddingItemInfo->m_cWeddingHallType == 0 )
						{
							pStatic_Picture->GetPicture(2)->SetShow ( true );
						}
						else
						{
							pStatic_Picture->GetPicture(1)->SetShow ( true );
						}
					}				
					if ( wstrWeddingDate.size() > 2 )
						wstrWeddingDate.erase ( wstrWeddingDate.end() - 3, wstrWeddingDate.end() );
		


					pStatic_WeddingDate->GetString(0)->msg = wstrWeddingDate;
					wstringstream wstrRemainingTime;
				
					if ( ctWeddingDate > ctNowTime )
					{
						wstrRemainingTime << ctpRemainingTime.GetDays () << GET_STRING ( STR_ID_24466 ) << " " << ctpRemainingTime.GetHours() << GET_STRING ( STR_ID_24583 ) << " " <<
						ctpRemainingTime.GetMinutes() << GET_STRING ( STR_ID_24584 ) << " " << GET_STRING ( STR_ID_24585 );
					}
					else
					{
						wstrRemainingTime << GET_STRING ( STR_ID_24480 );
					}	

					CKTDGUIStatic * pStatic_WeddingDateRemaining = static_cast<CKTDGUIStatic*> ( m_pDLGJoinWedding->GetControl( L"g_pStaticpost_window_Font4" ) );	
					pStatic_WeddingDateRemaining->GetString(0)->msg = wstrRemainingTime.str().c_str();
#ifdef CLIENT_COUNTRY_EU
					// 초대장 보낼 때 뜨는 UI
					pStatic_WeddingDateRemaining->GetString(0)->msg = CWordLineHandler::GetStrByLineBreakInX2Main( pStatic_WeddingDateRemaining->GetString(0)->msg.c_str(), 200, XUF_DODUM_13_SEMIBOLD );
#endif CLIENT_COUNTRY_EU
					CKTDGUIStatic * pStatic_WeddingLetter = static_cast<CKTDGUIStatic*> ( m_pDLGJoinWedding->GetControl( L"g_pStaticpost_window_Font1" ) );	

					wstring LetterMessage = pWeddingItemInfo->m_wstrLetterMessage;				 
					g_pMain->GetStrByLienBreak ( LetterMessage.c_str(), 250, 1 );
					pStatic_WeddingLetter->GetString(0)->msg = LetterMessage;
				}
				else
				{
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING ( STR_ID_24607 ), g_pMain->GetNowState() );
				}
			}
		}	
		return true;
	}
	else
	{
		if(m_pDLGJoinWedding != NULL)
		{
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGJoinWedding, NULL, false );
			m_pDLGJoinWedding = NULL;
		}
		return true;
	}

	return false;
}

bool CX2UIRelationship::ShowInviteWeddingDlg ( bool bEnable )
{

	m_vecInviteWeddingUserList.clear();
	m_vecInvitedWeddingUserList.clear();
	m_uiNowInviteUserPage = 1;
	if(bEnable == true)
	{
	
		m_pDLGInviteWedding = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_Invite_Wedding.lua" );
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGInviteWedding );
		
		CKTDGUIStatic * pStatic_Many = static_cast<CKTDGUIStatic*> ( m_pDLGInviteWedding->GetControl( L"g_pStatic_RemainingMany" ) );	


		CKTDGUIStatic* pStatic = static_cast<CKTDGUIStatic*> ( m_pDLGInviteWedding->GetControl ( L"g_pStaticCashShop_PresentWindow" ) );		
		if ( NULL != pStatic->GetPicture(9) && NULL != pStatic->GetPicture(10) )
		{
			pStatic->GetPicture(9)->SetShow ( true );
			pStatic->GetPicture(10)->SetShow ( false );
		}

		wstringstream wstrUserMany;

		if ( NULL != g_pData->GetRelationshipManager() &&
			 NULL != g_pData->GetRelationshipManager()->GetMyRelationshipInfo() )
		{
			CX2RelationshipManager::RelationshipInfo * pRelationshipInfo = g_pData->GetRelationshipManager()->GetMyRelationshipInfo();
			wstrUserMany << static_cast<int> ( m_vecInvitedWeddingUserList.size() ) << L" / " << GetMaxWeddingInviteNumber();
		}
		
		pStatic_Many->SetString( 0, wstrUserMany.str().c_str() );


		CKTDGUIComboBox * pComboBox = (CKTDGUIComboBox*)m_pDLGInviteWedding->GetControl( L"g_pPickedWeddingDay" );

		CTime ctCurrentTime( g_pData->GetServerCurrentTime() );		
		CTimeSpan ctsOneDay(1, 0, 0, 0 );

		for ( int i = 0; i < _CONST_UI_RELATIONSHIP_::g_iMaxReserveDay; ++i )
		{
			const UINT iCurrentDayOfWeek = ctCurrentTime.GetDayOfWeek();
			wstringstream wstrStream;
			wstrStream << ctCurrentTime.GetYear() % 2000 << GET_STRING ( STR_ID_24464 ) << ctCurrentTime.GetMonth() << GET_STRING ( STR_ID_24465 ) << 
				ctCurrentTime.GetDay() <<  GET_STRING ( STR_ID_24466 );
			ctCurrentTime += ctsOneDay;

			switch ( iCurrentDayOfWeek )
			{
				case 1 :
					wstrStream << GET_STRING ( STR_ID_24467 );
					break;
				case 2 :
					wstrStream << GET_STRING ( STR_ID_24468 );
					break;
				case 3 :
					wstrStream << GET_STRING ( STR_ID_24469 );
					break;
				case 4 :
					wstrStream << GET_STRING ( STR_ID_24470 );
					break;
				case 5 :
					wstrStream << GET_STRING ( STR_ID_24471 );
					break;
				case 6 :
					wstrStream << GET_STRING ( STR_ID_24472 );;
					break;
				case 7 :
					wstrStream << GET_STRING ( STR_ID_24473 );
					break;

			}
			pComboBox->AddItem ( (const WCHAR * ) wstrStream.str().c_str(), NULL );
		}

		return true;
	}
	else
	{
		if(m_pDLGInviteWedding != NULL)
		{
			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGInviteWedding, NULL, false );
			m_pDLGInviteWedding = NULL;
		}
		return true;
	}

	return false;
}


void CX2UIRelationship::ResetInvitingUserInfo ( )
{

	CKTDGUIComboBox * pComboBox = (CKTDGUIComboBox*)m_pDLGInviteWedding->GetControl( L"g_pComboInviteUserList" );
	pComboBox->RemoveAllItems();

	BOOST_FOREACH ( const InviteWeddingUserInfo & UserInfo , m_vecInvitedWeddingUserList )
	{
		pComboBox->AddItem ( (const WCHAR * ) UserInfo.m_wstrUserName.c_str(), NULL );
	}
}

void CX2UIRelationship::AddInviteWeddingUserList ( InviteWeddingUserInfo UserInfo_ )
{
	m_vecInviteWeddingUserList.push_back( UserInfo_ );
}


void CX2UIRelationship::AddInvitingUserInfo ( InviteWeddingUserInfo UserInfo_, RELATION_INVITE_USER_LIST_TYPE eType_ )
{
	if ( static_cast<int> ( m_vecInvitedWeddingUserList.size() ) + 1 <= GetMaxWeddingInviteNumber() )
	{
		vector<InviteWeddingUserInfo>::iterator it = m_vecInvitedWeddingUserList.begin();
		for ( ; it != m_vecInvitedWeddingUserList.end(); ++it )
		{
			if ( it->m_uiUserUID == UserInfo_.m_uiUserUID )
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999),GET_STRING ( STR_ID_24496 ), g_pMain->GetNowState() );
				
				break;
			}
		}

		if ( it == m_vecInvitedWeddingUserList.end()  )
		{
			m_vecInvitedWeddingUserList.push_back( UserInfo_ );
			CKTDGUIComboBox * pComboBox = (CKTDGUIComboBox*)m_pDLGInviteWedding->GetControl( L"g_pComboInviteUserList" );

			if ( eType_ == RIT_NICKNAME )
			{
				if ( NULL != g_pData->GetRelationshipManager() && 
					NULL != g_pData->GetRelationshipManager()->GetMyRelationshipInfo() )
				{
					CKTDGUIStatic * pStatic_Many = (CKTDGUIStatic*)m_pDLGInviteWedding->GetControl( L"g_pStatic_RemainingMany" );
					CX2RelationshipManager::RelationshipInfo * pRelationshipInfo = g_pData->GetRelationshipManager()->GetMyRelationshipInfo();
					int iPossibleInviteMany = GetMaxWeddingInviteNumber();
					wstringstream wstrUserMany;
					wstrUserMany << static_cast<int> ( m_vecInvitedWeddingUserList.size() )  << L" / " << iPossibleInviteMany;
					pStatic_Many->SetString( 0, wstrUserMany.str().c_str() );
				}
			}
		}
	}
	else
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(-999,-999),GET_STRING ( STR_ID_24580 ), g_pMain->GetNowState() );
	}
}

/** @fucntion : SetSelecteWeddingHallByPropose
	@brief : 청첩장 UI에서 소지중인 예식장으로 자동 선택 하는 함수
	@reutrn : 예식장 예약권이 하나도 없으면 or 선택 실패하면 false 반환
*/
bool CX2UIRelationship::SetSelecteWeddingHallByPropose( int iWeddingLetterMany )
{

	if( NULL != g_pData && 
		NULL != g_pData->GetMyUser() &&
		NULL != g_pData->GetMyUser()->GetSelectUnit() )
	{
        const CX2Inventory& kInventory = g_pData->GetMyUser()->GetSelectUnit()->GetInventory();
		const int iWeddingHallItemNum = ARRAY_SIZE(ITEM_ID_RESERVED_WEDDING_HALL);
		for( int i = 0 ; i < iWeddingHallItemNum; ++i )
		{
			if( NULL != kInventory.GetItemByTID( ITEM_ID_RESERVED_WEDDING_HALL[i] ) )
			{
#ifdef ADDED_RELATIONSHIP_SYSTEM_BUG_FIX
				int iWeddingLetterManyByWeddingHall = _CONST_UI_RELATIONSHIP_::g_iMaxInvitedUser;

				switch(ITEM_ID_RESERVED_WEDDING_HALL[i])
				{
				case 269712:/*루벤*/
					iWeddingLetterManyByWeddingHall = _CONST_UI_RELATIONSHIP_::g_iInvitingRubenWeddingHallMany;
					break;
				case 269713:/*샌더*/
					iWeddingLetterManyByWeddingHall = _CONST_UI_RELATIONSHIP_::g_iInvitingSanderWeddingHallMany;
					break;
				default:
					iWeddingLetterManyByWeddingHall = _CONST_UI_RELATIONSHIP_::g_iMaxInvitedUser;
					break;
				}

				// 최대 초대 가능 수는 30명으로 제한
				SetMaxWeddingInviteNumber ( min(iWeddingLetterManyByWeddingHall, iWeddingLetterMany) );
#else //ADDED_RELATIONSHIP_SYSTEM_BUG_FIX
				// 최대 초대 가능 수는 30명으로 제한
				SetMaxWeddingInviteNumber ( min(_CONST_UI_RELATIONSHIP_::g_iMaxInvitedUser, iWeddingLetterMany) );
#endif //ADDED_RELATIONSHIP_SYSTEM_BUG_FIX

				if( NULL == m_pDLGInviteWedding || 
					false == m_pDLGInviteWedding->GetShow() )
				{
					ShowInviteWeddingDlg(true);
				}

				if( NULL != m_pDLGInviteWedding )
				{
					CKTDGUIComboBox* pWeddingHallComboBox = 
						static_cast <CKTDGUIComboBox *> ( m_pDLGInviteWedding->GetControl(L"g_pPickedWeddingHall") );
					if( NULL != pWeddingHallComboBox )
					{
						if( E_INVALIDARG == pWeddingHallComboBox->SetSelectedByIndex(i) )
						{
							ASSERT(!"WeddingHall UI Error ( Statlc : g_pPickedWeddingHall ");
						}
					}
				}
				return true;
				break;
			}
		}
	}


	// for문에서 return 하지 못하면 에러 팝업 출력
#ifdef SERV_RELATIONSHIP_SYSTEM_INT
	g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING ( STR_ID_28119 ), g_pMain->GetNowState() );
#else
	g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), L"예식장 예약권이 존재하지 않습니다.", g_pMain->GetNowState() );
#endif SERV_RELATIONSHIP_SYSTEM_INT
	return false;
}

#endif // ADDED_RELATIONSHIP_SYSTEM