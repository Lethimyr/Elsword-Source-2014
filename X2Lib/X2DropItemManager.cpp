#include "StdAfx.h"
#include ".\x2dropitemmanager.h"



const float	CX2DropItemManager::ms_MaximumDistanceSquareToPickUp = 40000.f;


CX2DropItemManager::CX2DropItemManager(void)
{
	m_fElapsedTime	= 0.0f;
	m_apSoundGetItem[0]	= g_pKTDXApp->GetDeviceManager()->OpenSound( L"GetCoin.ogg" );
	m_apSoundGetItem[1]	= g_pKTDXApp->GetDeviceManager()->OpenSound( L"GetItem_Common.ogg" );
	m_apSoundGetItem[2]	= g_pKTDXApp->GetDeviceManager()->OpenSound( L"GetItem_Rare.ogg" );
	m_apSoundGetItem[3]	= g_pKTDXApp->GetDeviceManager()->OpenSound( L"GetItem_Elite.ogg" );
	m_apSoundGetItem[4]	= g_pKTDXApp->GetDeviceManager()->OpenSound( L"GetItem_Unique.ogg" );
	m_apSoundGetItem[5]	= g_pKTDXApp->GetDeviceManager()->OpenSound( L"Item_Take.ogg" );

	//m_pPicCharRed	= new CKTDGPicChar( L"PC_Red_", m_pMajorParticle->CreateSequence( NULL,  L"PicChar", 0.0f, 0.0f, 0.0f,  0.0f, 0.0f ) );
	//m_pPicCharBlue	= new CKTDGPicChar( L"PC_Blue_", m_pMajorParticle->CreateSequence( NULL,  L"PicChar", 0.0f, 0.0f, 0.0f,  0.0f, 0.0f ) );

	m_DropItemDataList.reserve( 64 );
	//m_LoadDropSeq.reserve( 256 );

#ifdef FIX_TRYGETDROPITEM
	m_fGetDist = ms_MaximumDistanceSquareToPickUp;
#endif

#ifdef PET_DROP_ITEM_PICKUP
	m_bIsSummonItemPickupPet = false;
#endif //PET_DROP_ITEM_PICKUP
}

CX2DropItemManager::~CX2DropItemManager(void)
{
	for( int i = 0; i < (int)m_DropItemDataList.size(); i++ )
	{
		DropItemData* pDropItemData = m_DropItemDataList[i];
		SAFE_DELETE( pDropItemData );
	}	
	m_DropItemDataList.clear();

	//for( int i = 0; i < (int)m_LoadDropSeq.size(); i++ )
	//{
	//	g_pX2Game->GetMajorParticle()->DestroyInstanceHandle( m_LoadDropSeq[i] );
	//}	


	for( int i=0; i<ARRAY_SIZE( m_apSoundGetItem ); i++ )
	{
		SAFE_CLOSE( m_apSoundGetItem[i] );
	}
}



//#ifdef THEMIDA_BY_TOOL_TEAM
//#	pragma optimize("",off)  
//#endif THEMIDA_BY_TOOL_TEAM
HRESULT CX2DropItemManager::OnFrameMove( double fTime, float fElapsedTime )
{

	KTDXPROFILE();

	m_fElapsedTime = fElapsedTime;

#ifdef FIX_TRYGETDROPITEM
	if( m_fGetDist > ms_MaximumDistanceSquareToPickUp )
	{		
		if( g_pMain != NULL && g_pData != NULL )
		{
			if( g_pData->GetMyUser()->GetUserData().hackingUserType != CX2User::HUT_AGREE_HACK_USER &&
				g_pKTDXApp->GetFindHacking() == false )
			{
				g_pData->GetServerProtocol()->SendID( EGS_REPORT_HACK_USER_NOT );
			}
			
#ifdef ADD_COLLECT_CLIENT_INFO
			g_pMain->SendHackInfo1( ANTIHACKING_ID::ANTIHACKING_GAME_00, "", false, false );
#else
			char strTitle[100];
			StringCchPrintfA( strTitle, 100 , ANTI_HACK_STRING_AntiHacking_DropItemManager_Dist_Verify, sqrt(m_fGetDist) );			
			//sprintf( strTitle, ANTI_HACK_STRING_AntiHacking_DropItemManager_Dist_Verify, sqrt(m_fGetDist) );		
			g_pMain->SendHackMail_DamageHistory( strTitle );
#endif
		}

		m_fGetDist = ms_MaximumDistanceSquareToPickUp;
	}
#endif



	for( int i = 0; i < (int)m_DropItemDataList.size(); i++ )
	{
		DropItemData* pDropItemData = m_DropItemDataList[i];
        CKTDGParticleSystem::CParticle* pParticle = NULL;

		if( NULL == pDropItemData 
            || INVALID_PARTICLE_SEQUENCE_HANDLE == pDropItemData->m_hSeq
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            || NULL == ( pParticle = g_pX2Game->GetMajorParticle()->ValidateParticleHandle( pDropItemData->m_hParticle ) )
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            || NULL == ( pParticle = pDropItemData->m_pParticle )   
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            )
        {
			SAFE_DELETE( pDropItemData );
			m_DropItemDataList.erase( m_DropItemDataList.begin() + i );
			i--;
            continue;
        }

		if( g_pKTDXApp->GetIsNowVeryfy() == true )
		{
			pDropItemData->Verify();
		}

		pDropItemData->m_fCoolTime -= m_fElapsedTime;
		if( pDropItemData->m_fCoolTime < 0.0f )
		{
			pDropItemData->m_fCoolTime = 0.0f;
		}


		if( pDropItemData->m_bGet == true )
		{
			pParticle->SetVelocity( D3DXVECTOR3( 0.0f, pParticle->GetVelocity().y, 0.f ) );

			pDropItemData->m_fScale -= 0.3f * fElapsedTime;
			if( pDropItemData->m_fScale < 0.0f )
			{
				SAFE_DELETE( pDropItemData );
				m_DropItemDataList.erase( m_DropItemDataList.begin() + i );
				i--;
			}
            else
            {
			    pParticle->SetSize( pParticle->GetSize() * pDropItemData->m_fScale );
            }
		}
		else
		{
			pDropItemData->m_fRemainTime -= m_fElapsedTime;
			if( pDropItemData->m_fRemainTime < 0.0f )
			{
				SAFE_DELETE( pDropItemData );
				m_DropItemDataList.erase( m_DropItemDataList.begin() + i );
				i--;
			}
			else
			{
				D3DXVECTOR3 outPos(0, 0, 0);
				int			lineIndex = 0;

				CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( pDropItemData->m_hSeq );
				if( g_pX2Game->GetWorld() != NULL && NULL != pSeq )
				{						
					if( pDropItemData->m_fNowSpeed > 0.0f )
					{
						const float MAGIC_DROP_ITEM_ACCELERATION = -500.f;
						pDropItemData->m_fNowSpeed += MAGIC_DROP_ITEM_ACCELERATION * m_fElapsedTime;
					}

					if( pDropItemData->m_fNowSpeed < 0.0f )
					{
						pDropItemData->m_fNowSpeed = 0.0f;
						if( g_pX2Game->GetWorld()->GetLineMap()->CanDown( pDropItemData->m_Pos, LINE_RADIUS * 2.3f, &outPos, &lineIndex, false ) == false )
						{
							lineIndex = g_pX2Game->GetWorld()->GetLineMap()->GetNearestLine( pDropItemData->m_Pos, &outPos );
							if( -1 != lineIndex )
							{
								pDropItemData->m_Pos = outPos;
								pSeq->SetLandPosition( outPos.y );
							}
						}
						else
						{
							pDropItemData->m_Pos = outPos;
							pSeq->SetLandPosition( outPos.y );
						}
					}

					if( pDropItemData->m_fNowSpeed != 0.0f )
					{
						if( g_pX2Game->GetWorld()->GetLineMap()->CanDown( pDropItemData->m_Pos, LINE_RADIUS * 2.3f, &outPos, &lineIndex, false ) == false )
						{
							lineIndex = g_pX2Game->GetWorld()->GetLineMap()->GetNearestLine( pDropItemData->m_Pos, &outPos );
							pDropItemData->m_Pos = outPos;
							pDropItemData->m_bLeft = !pDropItemData->m_bLeft;
						}

						const CKTDGLineMap::LineData* pLineData = g_pX2Game->GetWorld()->GetLineMap()->GetLineData( lineIndex );
						if( pLineData != NULL )
						{
							if( pDropItemData != NULL && pDropItemData->m_hSeq != INVALID_PARTICLE_SEQUENCE_HANDLE )
							{
								pSeq->SetLandPosition( outPos.y );
							}


							if( pDropItemData->m_bLeft == true )
							{
								pDropItemData->m_Pos -= pLineData->dirVector * pDropItemData->m_fNowSpeed * m_fElapsedTime;
							}
							else
							{
								pDropItemData->m_Pos += pLineData->dirVector * pDropItemData->m_fNowSpeed * m_fElapsedTime;
							}	
						}
					}

                    float fPosY = pParticle->GetPos().y;
					pDropItemData->m_Pos.y					= fPosY;
					pParticle->SetPos( pDropItemData->m_Pos );
				}		
			}
		}




	}

	return S_OK;
}
//#ifdef THEMIDA_BY_TOOL_TEAM
//#	pragma optimize("",on)  
//#endif THEMIDA_BY_TOOL_TEAM



void CX2DropItemManager::RenderName()
{
#ifdef DIALOG_SHOW_TOGGLE
	if( g_pKTDXApp->GetDGManager()->GetDialogManager()->GetHideDialog() == true )
		return;
#endif

	if( g_pX2Game->GetRenderNPCName() == false )
		return;

	for( int i = 0; i < (int)m_DropItemDataList.size(); i++ )
	{
		DropItemData* pDropItemData = m_DropItemDataList[i];
        CKTDGParticleSystem::CParticle* pParticle = NULL;

		if( pDropItemData != NULL 
			&& pDropItemData->m_hSeq != INVALID_PARTICLE_SEQUENCE_HANDLE 
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            && NULL != ( pParticle = g_pX2Game->GetMajorParticle()->ValidateParticleHandle( pDropItemData->m_hParticle ) )
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            && NULL != ( pParticle = pDropItemData->m_pParticle )   
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			&& pDropItemData->m_fCoolTime <= 0.0f
			&& pDropItemData->m_pItemTemplet != NULL
			&& pDropItemData->m_bGet == false )
		{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            CKTDGParticleSystem::CParticleEventSequence* pSeq = pParticle->GetMasterSequence();
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( pDropItemData->m_hSeq );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			if( NULL == pSeq )
				continue;
			
			D3DXVECTOR3 pos = pParticle->GetPos();
			pos.y = pSeq->GetLandPosition();

			D3DXCOLOR color;
			switch( pDropItemData->m_pItemTemplet->GetItemGrade() )
			{
			case CX2Item::IG_NONE:
				color = 0xffffffff;
				break;
			case CX2Item::IG_UNIQUE:
				color = 0xffffcc99;
				break;
			case CX2Item::IG_ELITE:
				color = 0xffff00ff;
				break;
			case CX2Item::IG_RARE:
				color = 0xffffff00;
				break;
			case CX2Item::IG_NORMAL:
				color = 0xffffffff;
				break;
			case CX2Item::IG_LOW:
				color = 0xff555555;
				break;

			default:
				color = 0xffffffff;
				break;
			}

#ifdef SERV_ENCHANT_ITEM_DROP_EVENT ///드랍되는 장비 아이템에 강화수치 추가

			wstring wstrItemFullName;

			if( pDropItemData->m_cEnchantLevel > 0 )
			{
				WCHAR buff[256] = L"";
				StringCchPrintfW( buff, ARRAY_SIZE(buff), L"+%d ",  static_cast<int>( pDropItemData->m_cEnchantLevel )  );

                wstrItemFullName = std::wstring(buff) + pDropItemData->m_pItemTemplet->GetFullName_();
			}				
			else
			{
				wstrItemFullName = pDropItemData->m_pItemTemplet->GetFullName_();
			}

			g_pX2Game->GetFontForUnitName()->OutProjectionText( pos, wstrItemFullName.c_str(), color, 
				CKTDGFontManager::FS_SHELL, 0xff000000 , NULL, DT_CENTER  );	
#else  SERV_ENCHANT_ITEM_DROP_EVENT
			g_pX2Game->GetFontForUnitName()->OutProjectionText( pos, 
                pDropItemData->m_pItemTemplet->GetFullName_(), 
                color, 
				CKTDGFontManager::FS_SHELL, 0xff000000 , NULL, DT_CENTER  );			
#endif SERV_ENCHANT_ITEM_DROP_EVENT
		}
	}
}

// TODO: 예전 훈련소에 사용하던 함수입니다. 이제 이 함수를 사용하지 않습니다. 스크립트에서 이 함수를 사용하는 곳을 찾아서 확인한후에 이 함수도 삭제해주세요.
// 가짜동전 즉, uid가 없는 아이템을 떨어뜨릴 때 사용합니다. 훈련소에서 가짜 동전 드롭할 때 사용.
void CX2DropItemManager::AddDropItem_LUA( int iItemID, D3DXVECTOR3 pos, float fSpeed, float fRemainTime )
{
	AddDropItem( iItemID, -1, pos, false, fSpeed, fRemainTime );
}

// @fSpeed : 서버에서 1~1000 사이의 값을 준다
#ifdef SERV_ENCHANT_ITEM_DROP_EVENT ///드랍되는 장비 아이템에 강화수치 추가
void CX2DropItemManager::AddDropItem( int iItemID, UidType ItemUID, const D3DXVECTOR3 &pos, bool bLeft, float fSpeed, float fRemainTime/*= 20.f*/, char cEnchantLevel/*= 0 */ )
#else
void CX2DropItemManager::AddDropItem( int iItemID, UidType ItemUID, D3DXVECTOR3 pos, bool bLeft, float fSpeed, float fRemainTime /*= 20.f*/ )
#endif SERV_ENCHANT_ITEM_DROP_EVENT
{
	// themida_checked
//	THEMIDA_VM_START

	const CX2Item::ItemTemplet* pTemplet = g_pData->GetItemManager()->GetItemTemplet( iItemID );
	if( pTemplet != NULL )
	{
		DropItemData* pDropItemData		= new DropItemData();
		pDropItemData->m_ItemID			= iItemID;
		pDropItemData->m_ItemUID		= ItemUID;
		pDropItemData->m_Pos			= pos;
		pDropItemData->m_bGet			= false;
		pDropItemData->m_bLeft			= bLeft;
		pDropItemData->m_fNowSpeed		= fSpeed * 0.8f;

		pDropItemData->m_fScale			= 1.0f;
		pDropItemData->m_fRemainTime	= fRemainTime;

		pDropItemData->m_pItemTemplet	= pTemplet;
		pDropItemData->m_hSeq			= g_pX2Game->GetMajorParticle()->CreateSequenceHandle( NULL,  
            pTemplet->GetDropViewer(), 
            pos, 0,0 );

#ifdef SERV_ENCHANT_ITEM_DROP_EVENT ///드랍되는 장비 아이템에 강화수치 추가
		pDropItemData->m_cEnchantLevel	= cEnchantLevel;
#endif SERV_ENCHANT_ITEM_DROP_EVENT

		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( pDropItemData->m_hSeq );
		if( NULL != pSeq )
		{
			pSeq->SetAlphaObject( false );
			pSeq->SetLandPosition( pos.y );
            CKTDGParticleSystem::CParticle* pParticle = pSeq->CreateNewParticle( pos );
            if ( pParticle != NULL )
            {
			    // 랜덤한 모양으로 아이템이 드롭되는 것처럼 보이게 하기 위한 trick
			    int iSpeedY = ((int)fSpeed)%9 + 12;
			    //pParticle->SetVelocity( D3DXVECTOR3( 0.f, (float)iSpeedY * 63.f, 0.f ) );
			    // oasis907 : 김상윤 [2010.9.29] // y 이동 줄임
			    pParticle->SetVelocity( D3DXVECTOR3( 0.f, (float)iSpeedY * 43.f, 0.f ) );
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
                pDropItemData->m_hParticle	= pParticle->GetHandle();
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
                pDropItemData->m_pParticle	= pParticle;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            }
            else
            {
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
                pDropItemData->m_hParticle = INVALID_PARTICLE_HANDLE;
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
                pDropItemData->m_pParticle = NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            }
		}
#ifdef _IN_HOUSE_
		else
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), L"There is no drop viewer", g_pMain->GetNowState() );
		}
#endif // IN_HOUSE


		m_DropItemDataList.push_back( pDropItemData );
	}	

//	THEMIDA_VM_END
}


UidType CX2DropItemManager::TryGetDropItem( D3DXVECTOR3 pos )
{
	KTDXPROFILE();


	THEMIDA_ENCODE_START

	UidType itemUID = -1;

	for( int i = 0; i < (int)m_DropItemDataList.size(); i++ )
	{


		DropItemData* pDropItemData = m_DropItemDataList[i];
		if( NULL == pDropItemData )
			continue;

		if( true == pDropItemData->m_bGet )
			continue;

		if( true == pDropItemData->m_bGetTry )
			continue;

		if( pDropItemData->m_fCoolTime > 0.f )
			continue;

		if( -1 == pDropItemData->m_ItemUID )
			continue;

#ifdef FIX_TRYGETDROPITEM

#ifdef PET_DROP_ITEM_PICKUP		

		if( true == m_bIsSummonItemPickupPet )//아이템 줍기가 가능한 펫을 소환한 상태일 떄
		{
			if( true == CX2ItemManager::IsEDItem( pDropItemData->m_ItemID ) || //ED 아이템 이거나
				CX2Item::IT_OUTLAY != pDropItemData->m_pItemTemplet->GetItemType() ) //순간 소비성이 아닐 경우
			{
				pDropItemData->m_bGetTry = true;
				itemUID = pDropItemData->m_ItemUID;
				break;
			}
		}
#endif //PET_DROP_ITEM_PICKUP
		
		// NOTE : 이 함수는 자주 호출 되기 때문에 성능저하를 피하기 위해 themida_vm 을 적용하지 않고 themida_encode를 적용했습니다. 
		ELSWORD_VIRTUALIZER_START

		D3DXVECTOR3 vDist = pos - pDropItemData->m_Pos;
		if( D3DXVec3LengthSq( &vDist ) < m_fGetDist )
		{
			pDropItemData->m_bGetTry = true;
			itemUID = pDropItemData->m_ItemUID;
			break;
		}

		ELSWORD_VIRTUALIZER_END

#else
		if( GetDistance( pDropItemData->m_Pos, pos ) < 100.0f )
		{
			pDropItemData->m_bGetTry = true;
			itemUID = pDropItemData->m_ItemUID;
			break;
		}
#endif FIX_TRYGETDROPITEM

	}

	THEMIDA_ENCODE_END

	return itemUID;
}

bool CX2DropItemManager::GetDropItem( IN const UidType itemUID_, IN const UidType unitUID_, 
	OUT int& iItemID_, 
    OUT const CX2Item::ItemTemplet*& pItemTemplet_ 
    )
{
    pItemTemplet_ = NULL;

	for( UINT i = 0; i < m_DropItemDataList.size(); ++i )
	{
		DropItemData* pDropItemData = m_DropItemDataList[i];
		if( NULL == pDropItemData )
			return false;

		if( itemUID_ != pDropItemData->m_ItemUID )
			continue;

#ifdef NEW_HENIR_TEST
		if( true == pDropItemData->m_bGet && pDropItemData->m_ItemID != NEW_HENIR_TIMESPACE_PIECE)
			continue;
#else
		if( true == pDropItemData->m_bGet )
			continue;
#endif NEW_HENIR_TEST

		//THEMIDA_VM_START

		pDropItemData->m_bGet = true;
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        CKTDGParticleSystem::CParticle* pParticle = g_pX2Game->GetMajorParticle()->ValidateParticleHandle( pDropItemData->m_hParticle );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        CKTDGParticleSystem::CParticle* pParticle = pDropItemData->m_pParticle;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

		if( pParticle != NULL )
		{
			pParticle->SetVelocity( D3DXVECTOR3( 0.0f, 800.f, 0.0f ) );
		}
		//pDropItemData의 m_pItemTemplet가 NULL이 아닐 경우 새롭게 찾지 않도록 변경
		if( NULL != pDropItemData->m_pItemTemplet )
			pItemTemplet_ = pDropItemData->m_pItemTemplet;
		else
			pItemTemplet_ = g_pData->GetItemManager()->GetItemTemplet( pDropItemData->m_ItemID );

		CX2GUUser* pCX2GUUser = g_pX2Game->GetUserUnitByUID( unitUID_ );

		if( pCX2GUUser != NULL )
		{
// 			if( true == pDropItemData->m_pItemTemplet->m_SpecialAbilityList.empty() && //특별한 동작이 없는 아이템일 경우
// 				true == pDropItemData->m_pItemTemplet->m_vecBuffFactorPtr.empty() )
			if( CX2Item::IT_OUTLAY != pItemTemplet_->GetItemType() )
			{
				if( pDropItemData->m_hSeq != INVALID_PARTICLE_SEQUENCE_HANDLE )
				{
					CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( pDropItemData->m_hSeq );
					if( NULL != pSeq )
					{
						pSeq->SetBlackHolePosition( pCX2GUUser->GetPos() );
                        if ( pParticle != NULL )
						    pParticle->SetBlackHoleTime( 0.1f );
					}
				}
			}
			g_pX2Game->GetMajorParticle()->CreateSequence( static_cast<CKTDGObject*>( pCX2GUUser ),  L"DamageImpact", 
				pDropItemData->m_Pos, 100, 200, 2, 10 );
			g_pX2Game->GetMajorParticle()->CreateSequence( static_cast<CKTDGObject*>( pCX2GUUser ),  L"DamageImpactCore", 
				pDropItemData->m_Pos, 100, 200, 2, 10 );
			g_pX2Game->GetMajorParticle()->CreateSequence( static_cast<CKTDGObject*>( pCX2GUUser ),  L"DamageImpactRingRed", 
				pDropItemData->m_Pos, 100, 200, 1, 1 );
		}
		/*
		D3DXVECTOR2 pos2 = g_pKTDXApp->GetProj3DPos( pDropItemData->m_Pos );
		g_pX2Game->GetMinorParticle()->CreateSequence( NULL,  L"DropGetBall", 
		pos2.x, pos2.y, 0.0f, 1000, 1000, -1, 1 );
		*/
		//THEMIDA_VM_END

		if( true == CX2ItemManager::IsEDItem( pDropItemData->m_ItemID ) )
		{
			if( NULL != m_apSoundGetItem[0] )
			{
				m_apSoundGetItem[0]->Play( false, false ); 
			}
		}
		else
		{
			bool bSpecialItem = false;
#ifdef DUNGEON_ITEM
			if(g_pX2Game != NULL )
			{
				CX2Item::SPECIAL_ITEM_TYPE eType = g_pX2Game->GetSpecialItemGroupType(pItemTemplet_);

				// 던전&대전 아이템인지 검사
				if( eType == CX2Item::SIT_NOSTRUM || eType == CX2Item::SIT_GENIUS || eType == CX2Item::SIT_CREST || eType == CX2Item::SIT_SUMMON_MONSTER )
				{
					if(m_apSoundGetItem[5] != NULL)
						m_apSoundGetItem[5]->Play( false, false ); 
					bSpecialItem = true;
				}
			}				
#endif
			if( bSpecialItem == false )
			{
				switch( pItemTemplet_->GetItemGrade() )
				{
				case CX2Item::IG_UNIQUE:
					{
						if( NULL != m_apSoundGetItem[4] )
						{
							m_apSoundGetItem[4]->Play( false, false ); 
						}
					} break;
				case CX2Item::IG_ELITE:
					{
						if( NULL != m_apSoundGetItem[3] )
						{
							m_apSoundGetItem[3]->Play( false, false ); 
						}
					} break;
				case CX2Item::IG_RARE:
					{
						if( NULL != m_apSoundGetItem[2] )
						{
							m_apSoundGetItem[2]->Play( false, false ); 
						}
					} break;

				default:
					//case CX2Item::IG_NORMAL:
					//case CX2Item::IG_LOW:
					{
						if( NULL != m_apSoundGetItem[1] )
						{
							m_apSoundGetItem[1]->Play( false, false ); 
						}
					} break;
				}
			}				
		}

		iItemID_ = pDropItemData->m_ItemID;
		return true;
	
	}
	return false;
}

int CX2DropItemManager::GetDropItemID( UidType itemUID )
{
	
	// NOTE : 성능저하를 피하기 위해 themida_vm 적용하지 않았습니다.
	ELSWORD_VIRTUALIZER_START

	for( int i = 0; i < (int)m_DropItemDataList.size(); i++ )
	{
		DropItemData* pDropItemData = m_DropItemDataList[i];
		if( pDropItemData->m_ItemUID == itemUID )
		{
			return pDropItemData->m_ItemID;
		}
	}

	ELSWORD_VIRTUALIZER_END

	return -1;
}

void CX2DropItemManager::DeleteAllItem()
{
	for( int i = 0; i < (int)m_DropItemDataList.size(); i++ )
	{
		DropItemData* pDropItemData = m_DropItemDataList[i];
		SAFE_DELETE( pDropItemData );
	}	

	m_DropItemDataList.resize(0);
}



int CX2DropItemManager::CountDropItemByID( int iItemID )
{
	int nCount = 0;
	for( UINT i=0; i<m_DropItemDataList.size(); i++ )
	{
		DropItemData* pDropItem = m_DropItemDataList[i];
		if( pDropItem->m_ItemID == iItemID && false == pDropItem->m_bGet )
		{
			nCount++;
		}
	}

	return nCount;
}

bool CX2DropItemManager::GetDropItemPosition( int iIndex, D3DXVECTOR3& vPos )
{
	if( iIndex < 0 || iIndex > (int)m_DropItemDataList.size()-1 )
	{
		return false;
	}	
	
	DropItemData* pDropItem = m_DropItemDataList[iIndex];
	if( true == pDropItem->m_bGet )
		return false;

	vPos = pDropItem->m_Pos;
	return true;
}


#ifdef  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
const D3DXVECTOR3*  CX2DropItemManager::GetDropItemPosition_LUA( int iIndex )
{
	if( iIndex < 0 || iIndex >= (int)m_DropItemDataList.size() )
	{
		return NULL;
	}	
	
	DropItemData* pDropItem = m_DropItemDataList[iIndex];
	if( true == pDropItem->m_bGet )
		return NULL;

	return &pDropItem->m_Pos;
}
#endif  X2OPTIMIZE_HANDLE_VALIDITY_CHECK

////////////////////////////////////////////////////////////////////////////////
CX2DropItemManager::DropItemData::~DropItemData()
{
	m_pItemTemplet	= NULL;
	//m_pParticle		= NULL;

	g_pX2Game->GetMajorParticle()->DestroyInstanceHandle( m_hSeq );
}

void CX2DropItemManager::DropItemData::Verify()
{
	if( m_ItemID.Verify()		== false 
		|| m_ItemUID.Verify()	== false 
		|| m_bGet.Verify()		== false 
		|| m_bGetTry.Verify()	== false  )
	{
		g_pKTDXApp->SetFindHacking( true );
	}
}

#ifdef DUNGEON_ITEM

bool CX2DropItemManager::ChangeDropItemPositionByUid(int itemUID, D3DXVECTOR3 vPos )
{
	for( int i = 0; i < (int)m_DropItemDataList.size(); i++ )
	{
		DropItemData* pDropItemData = m_DropItemDataList[i];
		if( pDropItemData->m_ItemUID == itemUID )
		{
			pDropItemData->m_Pos = vPos;
			return true;
		}
	}
	return false;
}

bool CX2DropItemManager::GetDropItemPositionByUid( int itemUID, D3DXVECTOR3& vPos )
{
	for( int i = 0; i < (int)m_DropItemDataList.size(); i++ )
	{
		DropItemData* pDropItemData = m_DropItemDataList[i];
		if( pDropItemData->m_ItemUID == itemUID )
		{
			vPos = pDropItemData->m_Pos;
			return true;
		}
	}
	return false;
}

// TODO: AddDropItem 함수가 2개 있습니다. refactoring
void CX2DropItemManager::AddDropItem( int iItemID, UidType ItemUID, D3DXVECTOR3 pos, float fCoolTime, float fRemainTime )
{

	// themida_checked
	//THEMIDA_VM_START 

	const CX2Item::ItemTemplet* pTemplet = g_pData->GetItemManager()->GetItemTemplet( iItemID );
	if( pTemplet != NULL )
	{
		DropItemData* pDropItemData		= new DropItemData();
		pDropItemData->m_ItemID			= iItemID;
		pDropItemData->m_ItemUID		= ItemUID;
		pDropItemData->m_Pos			= pos;
		pDropItemData->m_bGet			= false;
		pDropItemData->m_bLeft			= false;
		pDropItemData->m_fNowSpeed		= 0.f;

		pDropItemData->m_fScale			= 1.0f;
		pDropItemData->m_fCoolTime		= 0.f;
		pDropItemData->m_fRemainTime	= fRemainTime;		

		pDropItemData->m_pItemTemplet	= pTemplet;

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION

        pDropItemData->m_hSeq = INVALID_PARTICLE_SEQUENCE_HANDLE;
        pDropItemData->m_hParticle = INVALID_PARTICLE_HANDLE;
        CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->CreateSequence( NULL, pTemplet->GetDropViewer(),
            pos.x, pos.y, pos.z, 0.0 );
        if ( pSeq != NULL )
        {
            pDropItemData->m_hSeq = pSeq->GetHandle();
			pSeq->SetAlphaObject( false );
			pSeq->SetLandPosition( pos.y );
            pSeq->SetPerFrameSimulation( true );
			CKTDGParticleSystem::CParticle* pParticle = pSeq->CreateNewParticle( pos );
            if ( pParticle != NULL )
            {
			    pParticle->SetVelocity( D3DXVECTOR3( 0.f, (float)0.f, 0.f ) );
                pDropItemData->m_hParticle = pParticle->GetHandle();
            }
        }

#else   X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION

		pDropItemData->m_hSeq			= g_pX2Game->GetMajorParticle()->CreateSequenceHandle( NULL,
            pTemplet->GetDropViewer(),
            pos, 0,0 );
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( pDropItemData->m_hSeq );
		if( NULL != pSeq )
		{
			pSeq->SetAlphaObject( false );
			pSeq->SetLandPosition( pos.y );
			pDropItemData->m_pParticle	= pSeq->CreateNewParticle( pos );
            if ( pDropItemData->m_pParticle != NULL )
			    pDropItemData->m_pParticle->SetVelocity( D3DXVECTOR3( 0.f, (float)0.f, 0.f ) );
		}

#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION

		m_DropItemDataList.push_back( pDropItemData );
	}	

	//THEMIDA_VM_END
}
#endif DUNGEON_ITEM

















