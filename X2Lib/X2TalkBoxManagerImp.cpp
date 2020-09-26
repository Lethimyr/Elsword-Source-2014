#include "StdAfx.h"
#include ".\x2talkboxmanagerimp.h"

#define CX2TalkBoxManagerImp_NUM_FACEUV  10
#define CX2TalkBoxManagerImp_EffectTalkBox_NUM_FACEUV  2


//{{ robobeg : 2008-10-13
static const CKTDGStateManager::KState s_akStates[] = 
{
	KRenderState( D3DRS_ALPHATESTENABLE,	FALSE ),

	KRenderState( D3DRS_ALPHABLENDENABLE,	TRUE ),
	KRenderState( D3DRS_SRCBLEND,			D3DBLEND_SRCALPHA ),
	KRenderState( D3DRS_DESTBLEND,			D3DBLEND_INVSRCALPHA ),

	KRenderState( D3DRS_ZENABLE,			FALSE ),
	KRenderState( D3DRS_ZWRITEENABLE,		FALSE ),
	KRenderState( D3DRS_CULLMODE,			D3DCULL_CW ),

	KTextureStageState( 0, D3DTSS_ALPHAOP,		D3DTOP_MODULATE ),
	KTextureStageState( 0, D3DTSS_ALPHAARG1,	D3DTA_DIFFUSE ),
	KTextureStageState( 0, D3DTSS_ALPHAARG2,	D3DTA_TEXTURE ),
};//s_akStates[]
//}} robobeg : 2008-10-13


CX2TalkBoxManagerImp::CX2TalkBoxManagerImp(void)
{

	m_FontWidth		= 0;
	m_FontHeight	= 16;	//임시로 무조건 16다
#ifdef KTDGDEVICEFONT_SIMULATE_DIRECTX_FONT
	m_pFont			= g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( XUF_DODUM_15_NORMAL_MAPPED );
#else
	m_pFont			= g_pKTDXApp->GetDGManager()->GetDialogManager()->GetFont( XF_DODUM_15_NORMAL );
#endif
	m_pUKFont		= g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( XUF_DODUM_15_BOLD );

#ifdef TALKBOX_USE_BLAMBOT_FONT
	m_pFont			= g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( XUF_BLAM_NPC_TALK );
	m_pUKFont		= g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( XUF_BLAM_NPC_TALK );
#endif //TALKBOX_USE_BLAMBOT_FONT

	m_pTalkBoxTexture				= g_pKTDXApp->GetDeviceManager()->OpenTexture( L"HQ_talkbox12.tga" );

#ifdef ADDED_RELATIONSHIP_SYSTEM
	m_pBackEffectTexture				= g_pKTDXApp->GetDeviceManager()->OpenTexture( L"HQ_talkbox12.tga" );
#endif // ADDED_RELATIONSHIP_SYSTEM
	//{{ robobeg : 2008-10-13
	m_RenderStateID = s_akStates;
	//}} robobeg : 2008-10-13


	m_OffsetPosX		= 5;

	//MAX_ROOM_CHAT_STR_LEN		= 80;		// WCHAR 개수

	/* JHKang / 2010-9-10 / m_Size 값은 이전 개발자가 1024X768에 맞춘
	 * 값인 것 같다. x는 143정도가 이 해상도에서 "한글" 한 글자의 크기가
	 * 14로 9~10글자를 한 줄로 정의한 것 같다.
	 * y 값은 사용하지 않고 m_FontHeight 값을 사용한다.
	 */
	m_Size.x = 175 - 32;		// ?
	m_Size.y = 53;		// ?

#ifdef CLIENT_GLOBAL_LINEBREAK // 해상도에 맞게 변경된 사이즈 ( 해외팀 라인 브레이커에 해당 부분 고려가 안되서 말풍선 안에 글자 공백이 비여 보이는 경우가 있었음 )
	m_ResSize.x = 143;					
	m_ResSize.y = 53;
#endif //CLIENT_GLOBAL_LINEBREAK

	m_fMarginLeft		= 10.f;
	m_fMarginRight		= 10.f;
	m_fMarginTop		= 10.f;
	m_fMarginBottom		= 10.f;

	m_fTalkBoxCornerWidth	= 10.f;	// const	
	m_fTalkBoxCornerHeight	= 10.f; // const

	m_pSoundOver = g_pKTDXApp->GetDeviceManager()->OpenSound( L"X2_Button_Mouse_Over.ogg" );
	m_NowOverTalkBoxUnitUID = 0;	

	m_Offset = D3DXVECTOR2(0.f, 0.f);
	m_bOverTalkBox = false;
	m_bTalkBoxOverImg = false;
	m_bExtraVariable = false;

	m_nMaxStrLen = 0;	// 관련 함수들 사용하지 않는듯.. 필요없다면 삭제해야함

//#ifndef DYNAMIC_VERTEX_BUFFER_OPT
//	m_pVB = NULL;
//	m_pIB = NULL;
//	HRESULT hr = g_pKTDXApp->GetDevice()->CreateVertexBuffer( 4 * CX2TalkBoxManagerImp_NUM_FACEUV * sizeof(DRAWFACE_RHW_VERTEX), 
//		D3DUSAGE_WRITEONLY, D3DFVF_DRAWFACE_RHW_VERTEX, D3DPOOL_MANAGED, &m_pVB, NULL );
//	ASSERT( SUCCEEDED( hr ) );
//	hr = g_pKTDXApp->GetDevice()->CreateIndexBuffer( 6 * CX2TalkBoxManagerImp_NUM_FACEUV * sizeof(WORD),
//		D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIB, NULL);
//	ASSERT( SUCCEEDED( hr ) );
//
//	if ( m_pIB != NULL )
//	{
//		WORD*    pIndices = NULL;
//		hr = m_pIB->Lock(0, 0, (void**)&pIndices, 0 );
//		ASSERT( SUCCEEDED( hr ) );
//		if ( SUCCEEDED( hr ) )
//		{
//			int iFace = 0;
//			for( int i = 0; i < CX2TalkBoxManagerImp_NUM_FACEUV; i++, iFace += 4 )
//			{
//				*(pIndices++) = iFace + 0;
//				*(pIndices++) = iFace + 1;
//				*(pIndices++) = iFace + 2;
//				*(pIndices++) = iFace + 2;
//				*(pIndices++) = iFace + 1;
//				*(pIndices++) = iFace + 3;
//			}
//			m_pIB->Unlock();
//		}//if
//	}
//
//	m__fX_VB = 0.f;
//	m__fY_VB = 0.f;
//	m_fX_VB = 0.f;
//	m_fY_VB = 0.f;
//	m_fCenterWidth_VB = 0.f;
//	m_fCenterHeight_VB = 0.f;
//	m_fWidthScale_VB = 0.f;
//	m_fHeightScale_VB = 0.f;
//	m_d3dxBackTexColor_VB = D3DXCOLOR(0,0,0,0);
//#endif
}

CX2TalkBoxManagerImp::~CX2TalkBoxManagerImp(void)
{
	for ( int i = 0; i < (int)m_TalkBoxList.size(); i++ )
	{
		TalkBox& talkBox = m_TalkBoxList[i];
		m_pFont->Delete2DFont( talkBox.m_iFontArticleID );

	}
	m_TalkBoxList.clear();

	SAFE_CLOSE( m_pTalkBoxTexture );
#ifdef ADDED_RELATIONSHIP_SYSTEM
	SAFE_CLOSE ( m_pBackEffectTexture );
#endif // ADDED_RELATIONSHIP_SYSTEM
	SAFE_CLOSE( m_pSoundOver );

//#ifndef DYNAMIC_VERTEX_BUFFER_OPT
//	SAFE_RELEASE( m_pVB );
//	SAFE_RELEASE( m_pIB );
//#endif
}

bool CX2TalkBoxManagerImp::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( uMsg )
	{
	case WM_MOUSEMOVE:
		{
			POINT mousePoint = { short(LOWORD(lParam)), short(HIWORD(lParam)) };
			D3DXVECTOR2 mousePos = D3DXVECTOR2( (float)mousePoint.x, (float)mousePoint.y );

			MouseMove( mousePos );
		}
		break;
	}

	return false;
}

HRESULT CX2TalkBoxManagerImp::OnFrameMove( double fTime, float fElapsedTime )
{
	KTDXPROFILE();

	for ( int i = 0; i < (int)m_TalkBoxList.size(); i++ )
	{
		TalkBox& talkBox = m_TalkBoxList[i];

		bool bEraseTalkBox = false;

		if( true == talkBox.m_bTraceUnit && -1 != talkBox.m_OwnerUnitUID )
		{
			switch( talkBox.m_GameUnitType )
			{
			case CX2GameUnit::GUT_USER:
				{
#ifdef SERV_PET_SYSTEM
					if( talkBox.m_bPetTalk == true )
					{
						if( g_pTFieldGame != NULL && g_pTFieldGame->GetSquareUnitByUID( talkBox.m_OwnerUnitUID ) == NULL )
						{
							bEraseTalkBox = true;
						}
						else if( g_pX2Game != NULL && g_pX2Game->GetUserUnitByUID( talkBox.m_OwnerUnitUID ) == NULL )
						{
							bEraseTalkBox = true;
						}

						break;
					}
#endif

					switch( g_pMain->GetNowStateID() )
					{
					case CX2Main::XS_VILLAGE_MAP:
					case CX2Main::XS_SQUARE_GAME:
#ifdef ADDED_RELATIONSHIP_SYSTEM
					case CX2Main::XS_WEDDING_GAME:
#endif // ADDED_RELATIONSHIP_SYSTEM
						{

							//CX2SquareUnit* pSquareUnit = g_pSquareGame->GetSquareUnitByUID( talkBox.m_OwnerUnitUID );
							//if( NULL != pSquareUnit )
							//{
							//}

						} break;
					default:
						{
							CX2GUUser* pCX2GUUser = g_pX2Game->GetUserUnitByUID( talkBox.m_OwnerUnitUID );
							if( NULL != pCX2GUUser )
							{
								if( CX2GameUnit::GUSI_DIE == pCX2GUUser->GetGameUnitState() )
								{
									bEraseTalkBox = true;
								}
							}
							else
							{
								bEraseTalkBox = true;
							}
						} break;
					}
				} break;

			case CX2GameUnit::GUT_NPC:
				{
#ifdef TALK_BOX_VILLAGE_NPC
					if( talkBox.m_bExtraVariable == true )
					{
						CX2TFieldNpc* pCX2TFieldNpc = g_pTFieldGame->GetHouseFieldNPC( (int)talkBox.m_OwnerUnitUID );

						if ( NULL != pCX2TFieldNpc )
							bEraseTalkBox = false;
						else
							bEraseTalkBox = true;

						break;
					}
#endif
					CX2GUNPC* pCX2GUNPC = g_pX2Game->GetNPCUnitByUID( (int)talkBox.m_OwnerUnitUID );
					if( NULL != pCX2GUNPC )
					{		
						if( CX2GameUnit::GUSI_DIE == pCX2GUNPC->GetGameUnitState() )
						{
							bEraseTalkBox = true;
						}
					}
					else
					{
						bEraseTalkBox = true;
					}
				} break;
			}
		}

		if( true == bEraseTalkBox )
		{
			m_pFont->Delete2DFont( talkBox.m_iFontArticleID );
			m_TalkBoxList.erase( m_TalkBoxList.begin() + i );
			i--;
			continue;
		}


		if ( m_pFont->Get2DArticle( talkBox.m_iFontArticleID ) != NULL && m_pFont->Get2DArticle( talkBox.m_iFontArticleID )->bSpread == true )
		{
			if ( m_pFont->Get2DArticle( talkBox.m_iFontArticleID )->iSpreadCount == (int)talkBox.m_wstrTalkContent.size() )
			{
				talkBox.m_fRemainTime -= fElapsedTime;
			}
		}
		else
		{
			talkBox.m_fRemainTime -= fElapsedTime;
		}

		if ( talkBox.m_fRemainTime <= 0 )
		{
			m_pFont->Delete2DFont( talkBox.m_iFontArticleID );
			m_TalkBoxList.erase( m_TalkBoxList.begin() + i );
			i--;
		}
#ifdef ADD_TALK_BOX_DELAY_TIME
		if( talkBox.m_fDelayTime >= 0 )
		{
			talkBox.m_fDelayTime -= fElapsedTime;
		}
#endif //ADD_TALK_BOX_DELAY_TIME
	}

	for ( int i = 0; i < (int)m_TalkBoxList.size(); i++ )
	{
		TalkBox& talkBox = m_TalkBoxList[i];
		talkBox.Update( m_pFont, m_fMarginLeft, m_fMarginRight, m_Offset );
	}

	return S_OK;
}
HRESULT CX2TalkBoxManagerImp::OnFrameRender()
{
	KTDXPROFILE();

	//float boxWidth = 186;
	float boxBottomHeight = 39;
	float boxUpperHeight = 13;
	//float boxCenterHeight = 53;

	KD3DBEGIN()

	CKTDGStateManager::PushSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
	CKTDGStateManager::PushSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
	CKTDGStateManager::PushSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR );

	CKTDGStateManager::PushSamplerState( 0, D3DSAMP_ADDRESSU,  D3DTADDRESS_MIRROR );
	CKTDGStateManager::PushSamplerState( 0, D3DSAMP_ADDRESSV,  D3DTADDRESS_MIRROR );

	CKTDGStateManager::PushSamplerState( 1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
	CKTDGStateManager::PushSamplerState( 1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
	CKTDGStateManager::PushSamplerState( 1, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR );

	CKTDGStateManager::PushSamplerState( 1, D3DSAMP_ADDRESSU,  D3DTADDRESS_MIRROR );
	CKTDGStateManager::PushSamplerState( 1, D3DSAMP_ADDRESSV,  D3DTADDRESS_MIRROR );


	int incSize = m_FontHeight;

	//{{ robobeg : 2008-10-24
	//g_pKTDXApp->SetNULLTexture( 0 );
	CKTDGStateManager::SetTexture( 0, NULL );
	//}} robobeg : 2008-10-24

	for ( int i = 0; i < (int)m_TalkBoxList.size(); i++ )
	{
		TalkBox& talkBox = m_TalkBoxList[i];

#ifdef ADD_TALK_BOX_DELAY_TIME
		if( talkBox.m_fDelayTime >= 0 )
		{
			continue;
		}
#endif //ADD_TALK_BOX_DELAY_TIME

		if(talkBox.m_bShow == false)
			continue;

#ifdef DIALOG_SHOW_TOGGLE		
		if( g_pKTDXApp->GetDGManager()->GetDialogManager()->GetHideDialog() == true )
		{
			if( talkBox.m_OwnerUnitUID <= 0 || talkBox.m_bPetTalk == true )
				continue;
		}
#endif

		m_bTalkBoxOverImg = talkBox.m_bOverTalkBox;
		m_bExtraVariable = talkBox.m_bExtraVariable;

#ifdef ADDED_RELATIONSHIP_SYSTEM
		if ( talkBox.m_TalkBoxType == TBT_TALKBOX_EFFECT )
		{
			DrawTalkBoxEffect( talkBox.m_TalkBoxType, 
				talkBox.m_vPos.x * g_pKTDXApp->GetResolutionScaleX(),
				talkBox.m_vPos.y * g_pKTDXApp->GetResolutionScaleY(),
				talkBox.m_fBacklayerTextureWidth  * g_pKTDXApp->GetResolutionScaleX(),
				talkBox.m_fBacklayerTextureHeight * g_pKTDXApp->GetResolutionScaleY(), 
				talkBox.m_vTalkBoxSizeScale.x, talkBox.m_vTalkBoxSizeScale.y, 0.1f, talkBox.m_BackTexColor,	
				talkBox.m_wstrBacklayerTexture ); 		
		}
		else
#endif // ADDED_RELATIONSHIP_SYSTEM
		{

#ifdef ALWAYS_SCREEN_SHOT_TEST
			if( g_pInstanceData != NULL && g_pInstanceData->GetScreenShotTest() == false )
			{
				DrawBalloon( talkBox.m_TalkBoxType, 
					talkBox.m_vPos.x * g_pKTDXApp->GetResolutionScaleX(),
					talkBox.m_vPos.y * g_pKTDXApp->GetResolutionScaleY(),
					(talkBox.m_vTalkBoxSize.x - m_fTalkBoxCornerWidth * 2.f) * g_pKTDXApp->GetResolutionScaleX(),
					(talkBox.m_vTalkBoxSize.y - m_fTalkBoxCornerHeight * 2.f) * g_pKTDXApp->GetResolutionScaleY(), 
					talkBox.m_vTalkBoxSizeScale.x, talkBox.m_vTalkBoxSizeScale.y, talkBox.m_fTalkBoxTipLerpCoef, talkBox.m_BackTexColor 
					);
			}
#else
			DrawBalloon( talkBox.m_TalkBoxType, 
			talkBox.m_vPos.x * g_pKTDXApp->GetResolutionScaleX(),
			talkBox.m_vPos.y * g_pKTDXApp->GetResolutionScaleY(),
			(talkBox.m_vTalkBoxSize.x - m_fTalkBoxCornerWidth * 2.f) * g_pKTDXApp->GetResolutionScaleX(),
			(talkBox.m_vTalkBoxSize.y - m_fTalkBoxCornerHeight * 2.f) * g_pKTDXApp->GetResolutionScaleY(), 
			talkBox.m_vTalkBoxSizeScale.x, talkBox.m_vTalkBoxSizeScale.y, talkBox.m_fTalkBoxTipLerpCoef, talkBox.m_BackTexColor 
			);
#endif ALWAYS_SCREEN_SHOT_TEST
		}
		
		CKTDGFontManager::FontArticle* pFontArticle = m_pFont->Get2DArticle( talkBox.m_iFontArticleID );
		if ( pFontArticle != NULL )
		{	
#ifdef ALWAYS_SCREEN_SHOT_TEST
			if( g_pInstanceData != NULL && g_pInstanceData->GetScreenShotTest() ==  false )
			{
				//{{ 허상형 : [2009/10/28] //	m_bUseOutLine이 true 면 FS_SHELL로 그리도록 조건문 추가
				if( talkBox.m_bUseOutLine == true )
				{
					m_pFont->Draw2DText( talkBox.m_wstrTalkContent.c_str(), (int)pFontArticle->pos.x, (int)pFontArticle->pos.y, pFontArticle->color, pFontArticle->outLineColor,
						DT_LEFT, CKTDGFontManager::FS_SHELL, pFontArticle->iSpreadCount );
				}
				else
				{
					m_pFont->Draw2DText( talkBox.m_wstrTalkContent.c_str(), (int)pFontArticle->pos.x, (int)pFontArticle->pos.y, pFontArticle->color, pFontArticle->outLineColor,
						DT_LEFT, CKTDGFontManager::FS_NONE, pFontArticle->iSpreadCount );
				}
			}
#else 
			//{{ 허상형 : [2009/10/28] //	m_bUseOutLine이 true 면 FS_SHELL로 그리도록 조건문 추가
			if( talkBox.m_bUseOutLine == true )
			{
				m_pFont->Draw2DText( talkBox.m_wstrTalkContent.c_str(), (int)pFontArticle->pos.x, (int)pFontArticle->pos.y, pFontArticle->color, pFontArticle->outLineColor,
					DT_LEFT, CKTDGFontManager::FS_SHELL, pFontArticle->iSpreadCount );
			}
			else
			{
				m_pFont->Draw2DText( talkBox.m_wstrTalkContent.c_str(), (int)pFontArticle->pos.x, (int)pFontArticle->pos.y, pFontArticle->color, pFontArticle->outLineColor,
				DT_LEFT, CKTDGFontManager::FS_NONE, pFontArticle->iSpreadCount );
			}
#endif ALWAYS_SCREEN_SHOT_TEST
		}
	}



	//	m_pFont->Flush();
	//{{ robobeg : 2008-10-24
	//g_pKTDXApp->SetNULLTexture( 0 );
	CKTDGStateManager::SetTexture( 0, NULL );
	//}} robobeg : 2008-10-24


	KD3DEND()

		return S_OK;
}



void CX2TalkBoxManagerImp::Push( TalkBox& talkBox )
{
	KTDXPROFILE();

#ifdef NUMBER_TO_LANGUAGE
	//해외팀 라인브레이커
#ifdef CLIENT_GLOBAL_LINEBREAK
	D3DXVECTOR2 vMaxTalkBoxSize = g_pKTDXApp->ConvertByResolution( m_Size );
#ifdef CLIENT_GLOBAL_EU_WORDWRAP
	m_ResSize = vMaxTalkBoxSize;
#endif //CLIENT_GLOBAL_EU_WORDWRAP

	if( vMaxTalkBoxSize.x > m_Size.x )
		vMaxTalkBoxSize.x = m_Size.x;
	
	CWordLineHandler::LineBreakInTalkBoxManagerImp( m_pUKFont, m_Size, talkBox.m_nFontArticleRow, talkBox.m_iFontArticleColumnSize, talkBox.m_wstrTalkContent, talkBox.m_bTrade
#ifdef CLIENT_GLOBAL_EU_WORDWRAP
													, m_ResSize
#endif //CLIENT_GLOBAL_EU_WORDWRAP
		);
#else //CLIENT_GLOBAL_LINEBREAK
	LineBreak( talkBox.m_nFontArticleRow, talkBox.m_iFontArticleColumnSize, talkBox.m_wstrTalkContent, talkBox.m_bTrade );
#endif //CLIENT_GLOBAL_LINEBREAK

#else
	//해외팀 라인브레이커
#ifdef CLIENT_GLOBAL_LINEBREAK
	D3DXVECTOR2 vMaxTalkBoxSize = g_pKTDXApp->ConvertByResolution( m_Size );
	if( vMaxTalkBoxSize.x > m_Size.x )
		vMaxTalkBoxSize.x = m_Size.x;
	CWordLineHandler::LineBreakInTalkBoxManagerImp( m_pUKFont, m_Size, talkBox.m_nFontArticleRow, talkBox.m_iFontArticleColumnSize, talkBox.m_wstrTalkContent );
#else //CLIENT_GLOBAL_LINEBREAK
	LineBreak( talkBox.m_nFontArticleRow, talkBox.m_iFontArticleColumnSize, talkBox.m_wstrTalkContent );
#endif //CLIENT_GLOBAL_LINEBREAK

#endif NUMBER_TO_LANGUAGE
	talkBox.m_AddLineNum = talkBox.m_nFontArticleRow - 4; // ?

#ifdef RESIZE_BALLON
	//{{ 허상형 : [2011/6/22] //	TALKBOX 툴팁에 글씨가 삐져나가고 첫번째줄 글자가 한자 적게 출력되는 문제 수정
#ifdef FIX_TALKBOX_SIZE
	int iWidth = m_pUKFont->GetWidth( talkBox.m_wstrTalkContent.c_str() );
	talkBox.m_vTalkBoxSize.x = (float) ( iWidth /  g_pKTDXApp->GetResolutionScaleX()  + m_fMarginLeft + m_fMarginRight );
#else FIX_TALKBOX_SIZE
	talkBox.m_vTalkBoxSize.x = (float) ( talkBox.m_iFontArticleColumnSize / g_pKTDXApp->GetResolutionScaleX() + m_fMarginLeft + m_fMarginRight );
#endif FIX_TALKBOX_SIZE
	//}} 허상형 : [2011/6/22] //	
#else
	talkBox.m_vTalkBoxSize.x = (float) ( talkBox.m_iFontArticleColumnSize + m_fMarginLeft + m_fMarginRight );
#endif
	talkBox.m_vTalkBoxSize.y = (float) ( talkBox.m_nFontArticleRow * (float)(m_FontHeight) + m_fMarginTop + m_fMarginBottom );

	talkBox.m_vTalkBoxSizeScale		= D3DXVECTOR2( 1, 1 );
	talkBox.m_vTalkBoxSizeScaleDelta	= D3DXVECTOR2( 0, 0	);

	InitTalkBox( talkBox );
	//중복되지 않게 유닛 아이디 비교해서 이미 올라온거 없애고.. 올려야함..
	for ( int i = 0;  i < (int)m_TalkBoxList.size(); i++ )
	{
		//TalkBox* pTempTalkBox = m_TalkBoxList[i];
		TalkBox& tempTalkBox = m_TalkBoxList[i];
		if ( tempTalkBox.m_OwnerUnitUID == talkBox.m_OwnerUnitUID &&
			tempTalkBox.m_bTraceUnit == talkBox.m_bTraceUnit &&
			tempTalkBox.m_GameUnitType == talkBox.m_GameUnitType 
#ifdef ADDED_RELATIONSHIP_SYSTEM
			&& talkBox.m_TalkBoxType != CX2TalkBoxManagerImp::TBT_TALKBOX_EFFECT
#endif // ADDED_RELATIONSHIP_SYSTEM
			
			)
		{
			m_pFont->Delete2DFont( tempTalkBox.m_iFontArticleID );
			//SAFE_DELETE(pTempTalkBox);
			m_TalkBoxList.erase( m_TalkBoxList.begin() + i );
			break;
		}
	}

	//m_TalkBoxList.push_back( pTalkBox );
	m_TalkBoxList.push_back( talkBox );
}

bool CX2TalkBoxManagerImp::Delete( UidType unitUID, bool bTraceUnit, CX2GameUnit::GAME_UNIT_TYPE gameUnitType )
{
	for ( int i = 0; i < (int)m_TalkBoxList.size(); i++ )
	{
		TalkBox& tempTalkBox = m_TalkBoxList[i];
		if ( tempTalkBox.m_OwnerUnitUID == unitUID &&
			tempTalkBox.m_bTraceUnit == bTraceUnit &&
			tempTalkBox.m_GameUnitType == gameUnitType )
		{
			m_pFont->Delete2DFont( tempTalkBox.m_iFontArticleID );
			//SAFE_DELETE(pTempTalkBox);
			m_TalkBoxList.erase( m_TalkBoxList.begin() + i );

			return true;
		}
	}
	return false;
}

bool CX2TalkBoxManagerImp::DeleteByIndex( int index )
{
	if ( index < 0 || index >= (int)m_TalkBoxList.size() )
	{
		return false;
	}

	TalkBox& tempTalkBox = m_TalkBoxList[index];

	m_pFont->Delete2DFont( tempTalkBox.m_iFontArticleID );
	m_TalkBoxList.erase( m_TalkBoxList.begin() + index );

	return true;
}

CX2TalkBoxManagerImp::TalkBox* CX2TalkBoxManagerImp::GetTalkBoxByUID( UidType unitUID )
{
	for ( int i = 0; i < (int)m_TalkBoxList.size(); i++ )
	{
		TalkBox& tempTalkBox = m_TalkBoxList[i];
		if ( tempTalkBox.m_OwnerUnitUID == unitUID )
			return &m_TalkBoxList[i];
	}

	return NULL;
}

bool CX2TalkBoxManagerImp::CheckTalkbox( UidType unitUID )
{
	for ( int i = 0; i < (int)m_TalkBoxList.size(); i++ )
	{
		TalkBox& tempTalkBox = m_TalkBoxList[i];
		if ( tempTalkBox.m_OwnerUnitUID == unitUID )
			return true;
	}

	return false;
}

void CX2TalkBoxManagerImp::SetPos( UidType unitUID, D3DXVECTOR2 pos, int boxType, bool bTraceUnit, CX2GameUnit::GAME_UNIT_TYPE gameUnitType )
{


	for ( int i = 0; i < (int)m_TalkBoxList.size(); i++ )
	{
		TalkBox& talkBox = m_TalkBoxList[i];
		if( talkBox.m_OwnerUnitUID == unitUID &&
			talkBox.m_bTraceUnit == bTraceUnit &&
			talkBox.m_GameUnitType == gameUnitType )
		{
			talkBox.m_vPos = pos;
			if( boxType != -1 )
				talkBox.m_TalkBoxType = (TALK_BOX_TYPE)boxType;

			ResetTalkBox( talkBox );


			break;
		}
	}
}

void CX2TalkBoxManagerImp::SetPos( int talkBoxID, D3DXVECTOR2 pos, int boxType, bool bTraceUnit /* = false */ , CX2GameUnit::GAME_UNIT_TYPE gameUnitType /* = CX2GameUnit::GUT_USER */ )
{
	for ( int i = 0; i < (int)m_TalkBoxList.size(); i++ )
	{
		TalkBox& talkBox = m_TalkBoxList[i];
		if( talkBox.m_TalkBoxID == talkBoxID )
		{
			talkBox.m_vPos = pos;
			if( boxType != -1 )
				talkBox.m_TalkBoxType = (TALK_BOX_TYPE)boxType;

			ResetTalkBox( talkBox );

			break;
		}
	}
}

bool CX2TalkBoxManagerImp::CheckMousePointInTalkbox( D3DXVECTOR2 mousePos, UidType& uidType )
{


	D3DXVECTOR2 _pos;

	D3DXVECTOR2 _size;

	for ( int i = (int)m_TalkBoxList.size() - 1; i >= 0; i-- )
	{
		TalkBox& talkBox = m_TalkBoxList[i];

		_pos = talkBox.m_vPos;

		_size.x = talkBox.m_vTalkBoxSize.x - m_fTalkBoxCornerWidth * 2.f;
		_size.y = talkBox.m_vTalkBoxSize.y - m_fTalkBoxCornerHeight * 2.f;

		float fTipLerpCoeff = talkBox.m_fTalkBoxTipLerpCoef;

		_pos = g_pKTDXApp->ConvertByResolution( _pos );
		_size = g_pKTDXApp->ConvertByResolution( _size );

		const float fCornerWidthScaled  = m_fTalkBoxCornerWidth * talkBox.m_vTalkBoxSizeScale.x;
		const float fCornerHeightScaled = m_fTalkBoxCornerHeight * talkBox.m_vTalkBoxSizeScale.y;

		const float fCenterWidthScaled  = _size.x * talkBox.m_vTalkBoxSizeScale.x;
		const float fCenterHeightScaled = _size.y * talkBox.m_vTalkBoxSizeScale.y;

		switch( talkBox.m_TalkBoxType )
		{
		case TBT_FROM_DOWN_LEFT: 
			{
				_pos.x = _pos.x - fCenterWidthScaled * fTipLerpCoeff - fCornerWidthScaled;
				_pos.y = _pos.y - fCornerHeightScaled; 
			} 
			break;

		case TBT_FROM_DOWN_RIGHT:
			{
				_pos.x = _pos.x - fCenterWidthScaled * fTipLerpCoeff - fCornerWidthScaled;
				_pos.y = _pos.y - fCornerHeightScaled; 
			} 
			break;

		case TBT_FROM_UP_RIGHT: 
			{
				_pos.x = _pos.x - fCenterWidthScaled * fTipLerpCoeff - fCornerWidthScaled;
				_pos.y = _pos.y + 3.f * fCornerHeightScaled + fCenterHeightScaled;
			} 
			break;

		case TBT_FROM_UP_LEFT:
			{
				_pos.x = _pos.x - fCenterWidthScaled * fTipLerpCoeff - fCornerWidthScaled;
				_pos.y = _pos.y + 3.f * fCornerHeightScaled + fCenterHeightScaled;
			}
			break;
		}

		talkBox.m_bOverTalkBox = false;

//#ifdef DYNAMIC_VERTEX_BUFFER_OPT
		if ( mousePos.x >= _pos.x + fCornerWidthScaled && mousePos.x <= _pos.x + fCenterWidthScaled &&
			mousePos.y >= _pos.y - fCenterHeightScaled - fCornerHeightScaled && mousePos.y <= _pos.y + fCenterHeightScaled - fCornerHeightScaled - fCornerHeightScaled )
//#else
//		if ( mousePos.x >= _pos.x + fCornerWidthScaled - m_OffsetPosX  && mousePos.x <= _pos.x + fCenterWidthScaled + m_OffsetPosX * 2 &&
//			mousePos.y >= _pos.y - fCenterHeightScaled - fCornerHeightScaled && mousePos.y <= _pos.y + fCenterHeightScaled - fCornerHeightScaled - fCornerHeightScaled )
//#endif
		{
			uidType = talkBox.m_OwnerUnitUID;
			if(m_bOverTalkBox == true)
			{
				talkBox.m_bOverTalkBox = true;
			}
			return true;
		}		
	}

	return false;
}

void CX2TalkBoxManagerImp::Clear()
{
	for ( int i = 0; i < (int)m_TalkBoxList.size(); i++ )
	{
		TalkBox& talkBox = m_TalkBoxList[i];
		m_pFont->Delete2DFont( talkBox.m_iFontArticleID );
	}

	m_TalkBoxList.clear();
}










// 말풍선의 뾰족한 끝을 중심(fX,fY)으로 말풍선 전체를 그린다
void CX2TalkBoxManagerImp::DrawBalloon( int iType, float fX, float fY, float fCenterWidth, float fCenterHeight, float fWidthScale, 
									   float fHeightScale, float fTipLerpCoeff, D3DXCOLOR backTexColor )
{
	KTDXPROFILE();

	ASSERT( NULL != m_pTalkBoxTexture );
	if( NULL != m_pTalkBoxTexture )
	{
		m_pTalkBoxTexture->SetDeviceTexture();
	}

	const float fCornerWidthScaled  = m_fTalkBoxCornerWidth * fWidthScale;
	const float fCornerHeightScaled = m_fTalkBoxCornerHeight * fHeightScale;

	const float fCenterWidthScaled  = fCenterWidth * fWidthScale;
	const float fCenterHeightScaled = fCenterHeight * fHeightScale;

	float _fX = 0.0f, _fY = 0.0f;
	switch( iType )
	{
	case TBT_FROM_DOWN_LEFT: 
		{
			_fX = fX - fCenterWidthScaled * fTipLerpCoeff - fCornerWidthScaled;
			_fY = fY - fCornerHeightScaled; 
		} 
		break;

	case TBT_FROM_DOWN_RIGHT:
		{
			_fX = fX - fCenterWidthScaled * fTipLerpCoeff - fCornerWidthScaled;
			_fY = fY - fCornerHeightScaled; 
		} 
		break;

	case TBT_FROM_UP_RIGHT: 
		{
			_fX = fX - fCenterWidthScaled * fTipLerpCoeff - fCornerWidthScaled;
			_fY = fY + 3.f * fCornerHeightScaled + fCenterHeightScaled;
		} 
		break;

	case TBT_FROM_UP_LEFT:
		{
			_fX = fX - fCenterWidthScaled * fTipLerpCoeff - fCornerWidthScaled;
			_fY = fY + 3.f * fCornerHeightScaled + fCenterHeightScaled;
		}
		break;
		/*
		case TBT_FROM_LEFT: 
		{
		_fX = fX + fCornerWidthScaled;
		_fY = fY + fCenterHeightScaled * (1.f-fTipLerpCoeff) + fCornerHeightScaled;
		} break;

		case TBT_FROM_RIGHT: 
		{
		_fX = fX - 3.f * fCornerWidthScaled - fCenterWidthScaled;
		_fY = fY + fCenterHeightScaled * (1.f-fTipLerpCoeff) + fCornerHeightScaled;
		} break;
		*/
	}

//#ifdef DYNAMIC_VERTEX_BUFFER_OPT
	DrawBalloonBodyAndTip( iType, _fX, _fY, fX, fY, fCenterWidth, fCenterHeight, fWidthScale, fHeightScale, backTexColor );
//#else
//	DrawBalloonBody( _fX, _fY, fCenterWidth, fCenterHeight, fWidthScale, fHeightScale, backTexColor );
//	DrawBalloonTip( iType, fX, fY, fCenterWidth, fCenterHeight, fWidthScale, fHeightScale, backTexColor );
//#endif

	//{{ robobeg : 2008-10-24
	//g_pKTDXApp->SetNULLTexture( 0 );
	CKTDGStateManager::SetTexture( 0, NULL );
	//}} robobeg : 2008-10-24
}

#ifdef ADDED_RELATIONSHIP_SYSTEM
void CX2TalkBoxManagerImp::DrawTalkBoxEffect( int iType, float fX, float fY, float fCenterWidth, float fCenterHeight, float fWidthScale, 
	float fHeightScale, float fTipLerpCoeff, D3DXCOLOR backTexColor, wstring backTexName )
{
	KTDXPROFILE();

	ASSERT( NULL != m_pBackEffectTexture );
	if( NULL != m_pBackEffectTexture )
	{
		m_pBackEffectTexture = g_pKTDXApp->GetDeviceManager()->OpenTexture( backTexName );
		m_pBackEffectTexture->SetDeviceTexture();
	}
		

	const float fCenterWidthScaled  = fCenterWidth * fWidthScale;
	const float fCenterHeightScaled = fCenterHeight * fHeightScale;
	float _fX = fX - ( fCenterWidthScaled / 2 ); // - fCenterWidthScaled * 0.5;
	float _fY = fY - ( fCenterHeightScaled / 5 ) - 30;// + fCornerHeightScaled + 0; //fY - ( fCenterHeightScaled / 2 ) / 2;
	
	DrawTalkBoxEffectInnerFrame ( iType, _fX, _fY, fX, fY, fCenterWidth, fCenterHeight, fWidthScale, fHeightScale, backTexColor );

	//{{ robobeg : 2008-10-24
	//g_pKTDXApp->SetNULLTexture( 0 );
	

	CKTDGStateManager::SetTexture( 0, NULL );
	//}} robobeg : 2008-10-24
}

void CX2TalkBoxManagerImp::DrawTalkBoxEffectInnerFrame ( int iType, float _fX, float _fY, float fX, float fY, float fCenterWidth, float fCenterHeight, 
	float fWidthScale, float fHeightScale, D3DXCOLOR backTexColor )
{
	KTDXPROFILE();

	DRAWFACE_RHW_VERTEX aFaces[CX2TalkBoxManagerImp_EffectTalkBox_NUM_FACEUV * 4];
	ZeroMemory( aFaces, sizeof(aFaces) );

	const float fCornerWidthScaled  = 0.1f * fWidthScale;
	const float fCornerHeightScaled = 0.1f * fHeightScale;

	const float fCenterWidthScaled  = fCenterWidth * fWidthScale;
	const float fCenterHeightScaled = fCenterHeight * fHeightScale;

	// bottom_left
	SetFaceUV( &aFaces[ 0 * 4 ], _fX , _fY, 
		fCornerWidthScaled * 2 + fCenterWidthScaled, fCornerHeightScaled * 2 + fCenterHeightScaled,
		0.f, 0.f, 1.f, 1.f, 0.f, 0, backTexColor );

	KD3DPUSH( m_RenderStateID );

//#ifdef DYNAMIC_VERTEX_BUFFER_OPT
	BOOST_STATIC_ASSERT( D3DFVF_DRAWFACE_RHW_VERTEX == D3DFVF_XYZRHW_DIFFUSE_TEX1 );
		g_pKTDXApp->GetDVBManager()->DrawPrimitive( CKTDGDynamicVBManager::DVB_TYPE_XYZRHW_DIFFUSE_TEX1, 
			D3DPT_TRIANGLESTRIP, 2, &aFaces[ 0 ] );
//#else
//	g_pKTDXApp->GetDevice()->SetFVF( D3DFVF_DRAWFACE_RHW_VERTEX );
//	for( int i = 0; i < CX2TalkBoxManagerImp_EffectTalkBox_NUM_FACEUV; i++ )
//	{
//		g_pKTDXApp->GetDevice()->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, &aFaces[ i * 4 ], sizeof(DRAWFACE_RHW_VERTEX) );
//	}//for
//#endif

	KD3DEND();

}
#endif // ADDED_RELATIONSHIP_SYSTEM

//#ifdef DYNAMIC_VERTEX_BUFFER_OPT
// fx, fy는 left bottom의 좌표, 말풍선의 사각형을 그린다
void CX2TalkBoxManagerImp::DrawBalloonBodyAndTip( int iType, float _fX, float _fY, float fX, float fY, float fCenterWidth, float fCenterHeight, 
										   float fWidthScale, float fHeightScale, D3DXCOLOR backTexColor )
{
	KTDXPROFILE();

 	DRAWFACE_RHW_VERTEX aFaces[CX2TalkBoxManagerImp_NUM_FACEUV * 4];
	ZeroMemory( aFaces, sizeof(aFaces) );

	const float MAGIC_TEX_OFFSET = 0.f / 512.f;
	const float MAGIC_TEX_BLOCK_WIDTH  = 85.f / 512.f;
	const float MAGIC_TEX_BLOCK_HEIGHT = 85.f / 512.f;

	const float fCornerWidthScaled  = m_fTalkBoxCornerWidth * fWidthScale;
	const float fCornerHeightScaled = m_fTalkBoxCornerHeight * fHeightScale;

	const float fCenterWidthScaled  = fCenterWidth * fWidthScale;
	const float fCenterHeightScaled = fCenterHeight * fHeightScale;

	// bottom_left_corner
	SetFaceUV( &aFaces[ 0 * 4 ], _fX, _fY, fCornerWidthScaled, fCornerHeightScaled, 
		MAGIC_TEX_OFFSET + 0.f, MAGIC_TEX_OFFSET + 2.f * MAGIC_TEX_BLOCK_HEIGHT, 
		MAGIC_TEX_BLOCK_WIDTH, MAGIC_TEX_BLOCK_HEIGHT, 0.f, 0, backTexColor );

	// bottom_center
	SetFaceUV( &aFaces[ 1 * 4 ], _fX+fCornerWidthScaled, _fY, fCenterWidthScaled, fCornerHeightScaled, 
		MAGIC_TEX_OFFSET + MAGIC_TEX_BLOCK_HEIGHT, MAGIC_TEX_OFFSET + 2.f * MAGIC_TEX_BLOCK_HEIGHT,
		MAGIC_TEX_BLOCK_WIDTH, MAGIC_TEX_BLOCK_HEIGHT, 0.f, 0, backTexColor );

	// bottom_right_corner
	SetFaceUV( &aFaces[ 2 * 4 ], _fX+fCornerWidthScaled+fCenterWidthScaled, _fY, fCornerWidthScaled, fCornerHeightScaled, 
		MAGIC_TEX_OFFSET + 2.f * MAGIC_TEX_BLOCK_HEIGHT, MAGIC_TEX_OFFSET + 2.f * MAGIC_TEX_BLOCK_HEIGHT,
		MAGIC_TEX_BLOCK_WIDTH, MAGIC_TEX_BLOCK_HEIGHT, 0.f, 0, backTexColor );

	// center_left
	SetFaceUV( &aFaces[ 3 * 4 ], _fX, _fY-fCenterHeightScaled, fCornerWidthScaled, fCenterHeightScaled, 
		MAGIC_TEX_OFFSET + 0.f, MAGIC_TEX_OFFSET + 1.f * MAGIC_TEX_BLOCK_HEIGHT, 
		MAGIC_TEX_BLOCK_WIDTH, MAGIC_TEX_BLOCK_HEIGHT, 0.f, 0, backTexColor );

	// center_center	
	SetFaceUV( &aFaces[ 4 * 4 ], _fX+fCornerWidthScaled, _fY-fCenterHeightScaled, fCenterWidthScaled, fCenterHeightScaled, 
		MAGIC_TEX_OFFSET + MAGIC_TEX_BLOCK_WIDTH, MAGIC_TEX_OFFSET + 1.f * MAGIC_TEX_BLOCK_HEIGHT, 
		MAGIC_TEX_BLOCK_WIDTH, MAGIC_TEX_BLOCK_HEIGHT, 0.f, 0, backTexColor );

	// center_right
	SetFaceUV( &aFaces[ 5 * 4 ], _fX+fCornerWidthScaled+fCenterWidthScaled, _fY-fCenterHeightScaled, fCornerWidthScaled, fCenterHeightScaled, 
		MAGIC_TEX_OFFSET + 2.f * MAGIC_TEX_BLOCK_WIDTH, MAGIC_TEX_OFFSET + 1.f * MAGIC_TEX_BLOCK_HEIGHT, 
		MAGIC_TEX_BLOCK_WIDTH, MAGIC_TEX_BLOCK_HEIGHT, 0.f, 0, backTexColor );

	// upper_left
	SetFaceUV( &aFaces[ 6 * 4 ], _fX, _fY-fCornerHeightScaled-fCenterHeightScaled, fCornerWidthScaled, fCornerHeightScaled,
		MAGIC_TEX_OFFSET + 0.f, MAGIC_TEX_OFFSET + 0.f, 
		MAGIC_TEX_BLOCK_WIDTH, MAGIC_TEX_BLOCK_HEIGHT, 0.f, 0, backTexColor );

	// upper_center
	SetFaceUV( &aFaces[ 7 * 4 ], _fX+fCornerWidthScaled, _fY-fCornerHeightScaled-fCenterHeightScaled, fCenterWidthScaled, fCornerHeightScaled,
		MAGIC_TEX_OFFSET + MAGIC_TEX_BLOCK_WIDTH, MAGIC_TEX_OFFSET + 0.f, 
		MAGIC_TEX_BLOCK_WIDTH, MAGIC_TEX_BLOCK_HEIGHT, 0.f, 0, backTexColor );

	// upper_right
	SetFaceUV( &aFaces[ 8 * 4 ], _fX+fCornerWidthScaled+fCenterWidthScaled, _fY-fCornerHeightScaled-fCenterHeightScaled, fCornerWidthScaled, fCornerHeightScaled,
		MAGIC_TEX_OFFSET + 2.f * MAGIC_TEX_BLOCK_WIDTH, MAGIC_TEX_OFFSET + 0.f, 
		MAGIC_TEX_BLOCK_WIDTH, MAGIC_TEX_BLOCK_HEIGHT, 0.f, 0, backTexColor );

    SetBalloonTip( &aFaces[ 9 * 4 ], iType, fX, fY, fCenterWidth, fCenterHeight, fWidthScale, fHeightScale, backTexColor );

	KD3DPUSH( m_RenderStateID );

//#ifdef DYNAMIC_VERTEX_BUFFER_OPT
	BOOST_STATIC_ASSERT( D3DFVF_DRAWFACE_RHW_VERTEX == D3DFVF_XYZRHW_DIFFUSE_TEX1 );
	for( int i = 0; i < CX2TalkBoxManagerImp_NUM_FACEUV; i++ )
	{
		g_pKTDXApp->GetDVBManager()->DrawPrimitive( CKTDGDynamicVBManager::DVB_TYPE_XYZRHW_DIFFUSE_TEX1
			, D3DPT_TRIANGLESTRIP, 2, &aFaces[ i * 4 ] );
	}//for
//#else
//	g_pKTDXApp->GetDevice()->SetFVF( D3DFVF_DRAWFACE_RHW_VERTEX );
//	for( int i = 0; i < CX2TalkBoxManagerImp_NUM_FACEUV; i++ )
//	{
//		g_pKTDXApp->GetDevice()->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, &aFaces[ i * 4 ], sizeof(DRAWFACE_RHW_VERTEX) );
//	}//for
//#endif

    KD3DEND();
}

// 말풍선의 뾰족한 부분을 뾰족한 부분의 시작점(fX, fY)를 중심으로 그린다
void CX2TalkBoxManagerImp::SetBalloonTip( DRAWFACE_RHW_VERTEX aFaces[4], int iType, float fX, float fY, float fCenterWidth, float fCenterHeight, 
										  float fWidthScale, float fHeightScale, D3DXCOLOR backTexColor )
{
	KTDXPROFILE();


	const float MAGIC_TEX_BLOCK_WIDTH  = 85.f / 512.f;
	const float MAGIC_TEX_BLOCK_HEIGHT = 85.f / 512.f;

	const float fCornerWidthScaled  = m_fTalkBoxCornerWidth  * fWidthScale;
	const float fCornerHeightScaled = m_fTalkBoxCornerHeight * fHeightScale;


	const float MAGIC_OFFSET_Y = fCornerHeightScaled * 0.2f; // tip이 balloon 사각형을 약간 덮을 수 있도록

	float _fX = 0.0f;
	float _fY = 0.0f; 
	float _fRotAngle = 0.0f;
	int _flip = 0;
	switch( iType )
	{
	case TBT_FROM_DOWN_LEFT: 
		{
			_fX = fX - fCornerWidthScaled * 0.5f;
			_fY = fY - MAGIC_OFFSET_Y;
			_fRotAngle = 0.f;
		} 
		break;

	case TBT_FROM_DOWN_RIGHT: 
		{
			_fX = fX - fCornerWidthScaled * 0.5f;
			_fY = fY - MAGIC_OFFSET_Y;
			_flip = 2;
			_fRotAngle = 0.0f;

		} 
		break;

	case TBT_FROM_UP_RIGHT: 
		{
			_fX = fX - fCornerWidthScaled * 0.5f;
			_fY = fY + fCornerHeightScaled + MAGIC_OFFSET_Y;
			_fRotAngle = 180.f;
		} 
		break;

	case TBT_FROM_UP_LEFT: 
		{
			_fX = fX - fCornerWidthScaled * 0.5f;
			_fY = fY + fCornerHeightScaled + MAGIC_OFFSET_Y;
			_fRotAngle = 180.f;
			_flip = 2;
		} 
		break;
		/*
		case TBT_FROM_LEFT: // LEFT
		{
		_fX = fX;
		_fY = fY + fCornerHeightScaled * 0.5f;
		_fRotAngle = 90.f;
		} break;

		case TBT_FROM_RIGHT: // RIGHT
		{
		_fX = fX - fCornerWidthScaled;
		_fY = fY + fCornerHeightScaled * 0.5f;
		_fRotAngle = 270.f;
		} break;
		*/
	}

	SetFaceUV( aFaces, _fX, _fY, fCornerWidthScaled, fCornerHeightScaled, 259.f/512.f, 1.f/512.f, 88.f/512.f, 121.f/512.f, _fRotAngle, _flip, backTexColor );

}

void CX2TalkBoxManagerImp::SetFaceUV( DRAWFACE_RHW_VERTEX vertex[4], float nX, float nY, float nWidth, float nHeight, float fU, float fV, float fTexWidth, float fTexHeight,								    
									  float fClockWiseRotate, int iFlipType, D3DCOLOR color /* = 0xffffffff  */)
{
	KTDXPROFILE();

	float X1 = (float)nX - 0.5f;
	float Y1 = (float)nY - 0.5f;
	float X2 = (float)(nX + nWidth) - 0.5f;
	float Y2 = (float)(nY + nHeight) - 0.5f;

	//float temp =  1.0f / nHeight;

	float UMin = fU;
	float VMin = fV;
	float UMax = fU + fTexWidth;
	float VMax = fV + fTexHeight;

	//Set up the 4 corners of a small square
	vertex[0].x = X1;     vertex[0].y = Y1;
	vertex[0].u = UMin;   vertex[0].v = VMin;
	vertex[0].z = 1.0f;   vertex[0].rhw = 1.0f;	

	vertex[1].x = X1;     vertex[1].y = Y2;
	vertex[1].u = UMin;   vertex[1].v = VMax;
	vertex[1].z = 1.0f;   vertex[1].rhw = 1.0f;

	vertex[2].x = X2;     vertex[2].y = Y1;
	vertex[2].u = UMax;   vertex[2].v = VMin;
	vertex[2].z = 1.0f;   vertex[2].rhw = 1.0f;

	vertex[3].x = X2;     vertex[3].y = Y2;
	vertex[3].u = UMax;   vertex[3].v = VMax;
	vertex[3].z = 1.0f;   vertex[3].rhw = 1.0f;

	if ( fClockWiseRotate == 0.0f )
	{
		if( iFlipType == 1 ) // x축을 기준으로 flip
		{
		}
		else if( iFlipType == 2 ) // y축을 기준으로 flip
		{		
			vertex[2].u = UMin;   vertex[2].v = VMin;
			vertex[3].u = UMin;   vertex[3].v = VMax;
			vertex[0].u = UMax;   vertex[0].v = VMin;
			vertex[1].u = UMax;   vertex[1].v = VMax;
		}
		else // no flip
		{		
			vertex[0].u = UMin;   vertex[0].v = VMin;
			vertex[1].u = UMin;   vertex[1].v = VMax;
			vertex[2].u = UMax;   vertex[2].v = VMin;
			vertex[3].u = UMax;   vertex[3].v = VMax;
		}
	}
	else if( 90.f == fClockWiseRotate )
	{
		if( iFlipType == 1 ) // x축을 기준으로 flip
		{
			vertex[0].u = UMin;   vertex[0].v = VMin;
			vertex[2].u = UMin;   vertex[2].v = VMax;
			vertex[1].u = UMax;   vertex[1].v = VMin;
			vertex[3].u = UMax;   vertex[3].v = VMax;
		}
		else if( iFlipType == 2 ) // y축을 기준으로 flip
		{		
			vertex[3].u = UMin;   vertex[3].v = VMin;
			vertex[1].u = UMin;   vertex[1].v = VMax;
			vertex[2].u = UMax;   vertex[2].v = VMin;
			vertex[0].u = UMax;   vertex[0].v = VMax;
		}
		else // no flip
		{		
			vertex[2].u = UMin;   vertex[2].v = VMin;
			vertex[0].u = UMin;   vertex[0].v = VMax;
			vertex[3].u = UMax;   vertex[3].v = VMin;
			vertex[1].u = UMax;   vertex[1].v = VMax;
		}
	}
	else if( 180.f == fClockWiseRotate )
	{
		if( iFlipType == 1 ) // x축을 기준으로 flip
		{

		}
		else if( iFlipType == 2 ) // y축을 기준으로 flip
		{		
			vertex[1].u = UMin;   vertex[1].v = VMin;
			vertex[0].u = UMin;   vertex[0].v = VMax;
			vertex[3].u = UMax;   vertex[3].v = VMin;
			vertex[2].u = UMax;   vertex[2].v = VMax;
		}
		else // no flip
		{		
			// fix!! flip 구현 덜 됬음
			vertex[3].u = UMin;   vertex[3].v = VMin;
			vertex[2].u = UMin;   vertex[2].v = VMax;
			vertex[1].u = UMax;   vertex[1].v = VMin;
			vertex[0].u = UMax;   vertex[0].v = VMax;
		}

	}
	else if ( 270.f == fClockWiseRotate )
	{
		vertex[1].u = UMin;   vertex[1].v = VMin;
		vertex[3].u = UMin;   vertex[3].v = VMax;
		vertex[0].u = UMax;   vertex[0].v = VMin;
		vertex[2].u = UMax;   vertex[2].v = VMax;
	}

	vertex[0].color = vertex[1].color = vertex[2].color = vertex[3].color = color;

}


//#else
//void CX2TalkBoxManagerImp::DrawFaceUV( float nX, float nY, float nWidth, float nHeight, float fU, float fV, float fTexWidth, float fTexHeight,								    
//									  float fClockWiseRotate, int iFlipType, D3DCOLOR color /* = 0xffffffff  */)
//{
//	KTDXPROFILE();
//
//	KD3DPUSH( m_RenderStateID );
//
//	DRAWFACE_RHW_VERTEX vertex[4];
//	ZeroMemory( vertex, sizeof(DRAWFACE_RHW_VERTEX) * 4 );
//
//	float X1 = (float)nX - 0.5f;
//	float Y1 = (float)nY - 0.5f;
//	float X2 = (float)(nX + nWidth) - 0.5f;
//	float Y2 = (float)(nY + nHeight) - 0.5f;
//
//	//float temp =  1.0f / nHeight;
//
//	float UMin = fU;
//	float VMin = fV;
//	float UMax = fU + fTexWidth;
//	float VMax = fV + fTexHeight;
//
//	//Set up the 4 corners of a small square
//	vertex[0].x = X1;     vertex[0].y = Y1;
//	vertex[0].u = UMin;   vertex[0].v = VMin;
//	vertex[0].z = 1.0f;   vertex[0].rhw = 1.0f;	
//
//	vertex[1].x = X1;     vertex[1].y = Y2;
//	vertex[1].u = UMin;   vertex[1].v = VMax;
//	vertex[1].z = 1.0f;   vertex[1].rhw = 1.0f;
//
//	vertex[2].x = X2;     vertex[2].y = Y1;
//	vertex[2].u = UMax;   vertex[2].v = VMin;
//	vertex[2].z = 1.0f;   vertex[2].rhw = 1.0f;
//
//	vertex[3].x = X2;     vertex[3].y = Y2;
//	vertex[3].u = UMax;   vertex[3].v = VMax;
//	vertex[3].z = 1.0f;   vertex[3].rhw = 1.0f;
//
//	if ( fClockWiseRotate == 0.0f )
//	{
//		if( iFlipType == 1 ) // x축을 기준으로 flip
//		{
//		}
//		else if( iFlipType == 2 ) // y축을 기준으로 flip
//		{		
//			vertex[2].u = UMin;   vertex[2].v = VMin;
//			vertex[3].u = UMin;   vertex[3].v = VMax;
//			vertex[0].u = UMax;   vertex[0].v = VMin;
//			vertex[1].u = UMax;   vertex[1].v = VMax;
//		}
//		else // no flip
//		{		
//			vertex[0].u = UMin;   vertex[0].v = VMin;
//			vertex[1].u = UMin;   vertex[1].v = VMax;
//			vertex[2].u = UMax;   vertex[2].v = VMin;
//			vertex[3].u = UMax;   vertex[3].v = VMax;
//		}
//	}
//	else if( 90.f == fClockWiseRotate )
//	{
//		if( iFlipType == 1 ) // x축을 기준으로 flip
//		{
//			vertex[0].u = UMin;   vertex[0].v = VMin;
//			vertex[2].u = UMin;   vertex[2].v = VMax;
//			vertex[1].u = UMax;   vertex[1].v = VMin;
//			vertex[3].u = UMax;   vertex[3].v = VMax;
//		}
//		else if( iFlipType == 2 ) // y축을 기준으로 flip
//		{		
//			vertex[3].u = UMin;   vertex[3].v = VMin;
//			vertex[1].u = UMin;   vertex[1].v = VMax;
//			vertex[2].u = UMax;   vertex[2].v = VMin;
//			vertex[0].u = UMax;   vertex[0].v = VMax;
//		}
//		else // no flip
//		{		
//			vertex[2].u = UMin;   vertex[2].v = VMin;
//			vertex[0].u = UMin;   vertex[0].v = VMax;
//			vertex[3].u = UMax;   vertex[3].v = VMin;
//			vertex[1].u = UMax;   vertex[1].v = VMax;
//		}
//	}
//	else if( 180.f == fClockWiseRotate )
//	{
//		if( iFlipType == 1 ) // x축을 기준으로 flip
//		{
//
//		}
//		else if( iFlipType == 2 ) // y축을 기준으로 flip
//		{		
//			vertex[1].u = UMin;   vertex[1].v = VMin;
//			vertex[0].u = UMin;   vertex[0].v = VMax;
//			vertex[3].u = UMax;   vertex[3].v = VMin;
//			vertex[2].u = UMax;   vertex[2].v = VMax;
//		}
//		else // no flip
//		{		
//			// fix!! flip 구현 덜 됬음
//			vertex[3].u = UMin;   vertex[3].v = VMin;
//			vertex[2].u = UMin;   vertex[2].v = VMax;
//			vertex[1].u = UMax;   vertex[1].v = VMin;
//			vertex[0].u = UMax;   vertex[0].v = VMax;
//		}
//
//	}
//	else if ( 270.f == fClockWiseRotate )
//	{
//		vertex[1].u = UMin;   vertex[1].v = VMin;
//		vertex[3].u = UMin;   vertex[3].v = VMax;
//		vertex[0].u = UMax;   vertex[0].v = VMin;
//		vertex[2].u = UMax;   vertex[2].v = VMax;
//	}
//
//	vertex[0].color = vertex[1].color = vertex[2].color = vertex[3].color = color;
//
//	g_pKTDXApp->GetDevice()->SetFVF( D3DFVF_DRAWFACE_RHW_VERTEX );
//	g_pKTDXApp->GetDevice()->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, vertex, sizeof(DRAWFACE_RHW_VERTEX) );
//
//
//	KD3DEND()
//}
//
//
//// fx, fy는 left bottom의 좌표, 말풍선의 사각형을 그린다
//void CX2TalkBoxManagerImp::DrawBalloonBody( float fX, float fY, float fCenterWidth, float fCenterHeight, 
//										   float fWidthScale, float fHeightScale, D3DXCOLOR backTexColor )
//{
//	KTDXPROFILE();
//
//#ifdef DYNAMIC_VERTEX_BUFFER_OPT
//    DRAWFACE_RHW_VERTEX aFaces[CX2TalkBoxManagerImp_NUM_FACEUV * 4];
//#endif
//
//	const float MAGIC_TEX_OFFSET = 0.f / 512.f;
//	const float MAGIC_TEX_BLOCK_WIDTH  = 85.f / 512.f;
//	const float MAGIC_TEX_BLOCK_HEIGHT = 85.f / 512.f;
//
//	const float fCornerWidthScaled  = m_fTalkBoxCornerWidth * fWidthScale;
//	const float fCornerHeightScaled = m_fTalkBoxCornerHeight * fHeightScale;
//
//	const float fCenterWidthScaled  = fCenterWidth * fWidthScale;
//	const float fCenterHeightScaled = fCenterHeight * fHeightScale;
//
//#ifdef DYNAMIC_VERTEX_BUFFER_OPT
//	// bottom_left_corner
//	DrawFaceUV( &aFaces[ 0 * 4 ], fX, fY, fCornerWidthScaled, fCornerHeightScaled, 
//		MAGIC_TEX_OFFSET + 0.f, MAGIC_TEX_OFFSET + 2.f * MAGIC_TEX_BLOCK_HEIGHT, 
//		MAGIC_TEX_BLOCK_WIDTH, MAGIC_TEX_BLOCK_HEIGHT, 0.f, 0, backTexColor );
//
//	// bottom_center
//	DrawFaceUV( &aFaces[ 1 * 4 ], fX+fCornerWidthScaled, fY, fCenterWidthScaled, fCornerHeightScaled, 
//		MAGIC_TEX_OFFSET + MAGIC_TEX_BLOCK_HEIGHT, MAGIC_TEX_OFFSET + 2.f * MAGIC_TEX_BLOCK_HEIGHT,
//		MAGIC_TEX_BLOCK_WIDTH, MAGIC_TEX_BLOCK_HEIGHT, 0.f, 0, backTexColor );
//
//	// bottom_right_corner
//	DrawFaceUV( &aFaces[ 2 * 4 ], fX+fCornerWidthScaled+fCenterWidthScaled, fY, fCornerWidthScaled, fCornerHeightScaled, 
//		MAGIC_TEX_OFFSET + 2.f * MAGIC_TEX_BLOCK_HEIGHT, MAGIC_TEX_OFFSET + 2.f * MAGIC_TEX_BLOCK_HEIGHT,
//		MAGIC_TEX_BLOCK_WIDTH, MAGIC_TEX_BLOCK_HEIGHT, 0.f, 0, backTexColor );
//
//	// center_left
//	DrawFaceUV( &aFaces[ 3 * 4 ], fX, fY-fCenterHeightScaled, fCornerWidthScaled, fCenterHeightScaled, 
//		MAGIC_TEX_OFFSET + 0.f, MAGIC_TEX_OFFSET + 1.f * MAGIC_TEX_BLOCK_HEIGHT, 
//		MAGIC_TEX_BLOCK_WIDTH, MAGIC_TEX_BLOCK_HEIGHT, 0.f, 0, backTexColor );
//
//	// center_center	
//	DrawFaceUV( &aFaces[ 4 * 4 ], fX+fCornerWidthScaled, fY-fCenterHeightScaled, fCenterWidthScaled, fCenterHeightScaled, 
//		MAGIC_TEX_OFFSET + MAGIC_TEX_BLOCK_WIDTH, MAGIC_TEX_OFFSET + 1.f * MAGIC_TEX_BLOCK_HEIGHT, 
//		MAGIC_TEX_BLOCK_WIDTH, MAGIC_TEX_BLOCK_HEIGHT, 0.f, 0, backTexColor );
//
//	// center_right
//	DrawFaceUV( &aFaces[ 5 * 4 ], fX+fCornerWidthScaled+fCenterWidthScaled, fY-fCenterHeightScaled, fCornerWidthScaled, fCenterHeightScaled, 
//		MAGIC_TEX_OFFSET + 2.f * MAGIC_TEX_BLOCK_WIDTH, MAGIC_TEX_OFFSET + 1.f * MAGIC_TEX_BLOCK_HEIGHT, 
//		MAGIC_TEX_BLOCK_WIDTH, MAGIC_TEX_BLOCK_HEIGHT, 0.f, 0, backTexColor );
//
//	// upper_left
//	DrawFaceUV( &aFaces[ 6 * 4 ], fX, fY-fCornerHeightScaled-fCenterHeightScaled, fCornerWidthScaled, fCornerHeightScaled,
//		MAGIC_TEX_OFFSET + 0.f, MAGIC_TEX_OFFSET + 0.f, 
//		MAGIC_TEX_BLOCK_WIDTH, MAGIC_TEX_BLOCK_HEIGHT, 0.f, 0, backTexColor );
//
//	// upper_center
//	DrawFaceUV( &aFaces[ 7 * 4 ], fX+fCornerWidthScaled, fY-fCornerHeightScaled-fCenterHeightScaled, fCenterWidthScaled, fCornerHeightScaled,
//		MAGIC_TEX_OFFSET + MAGIC_TEX_BLOCK_WIDTH, MAGIC_TEX_OFFSET + 0.f, 
//		MAGIC_TEX_BLOCK_WIDTH, MAGIC_TEX_BLOCK_HEIGHT, 0.f, 0, backTexColor );
//
//	// upper_right
//	DrawFaceUV( &aFaces[ 8 * 4 ], fX+fCornerWidthScaled+fCenterWidthScaled, fY-fCornerHeightScaled-fCenterHeightScaled, fCornerWidthScaled, fCornerHeightScaled,
//		MAGIC_TEX_OFFSET + 2.f * MAGIC_TEX_BLOCK_WIDTH, MAGIC_TEX_OFFSET + 0.f, 
//		MAGIC_TEX_BLOCK_WIDTH, MAGIC_TEX_BLOCK_HEIGHT, 0.f, 0, backTexColor );
//#else
//	// bottom_left_corner
//	DrawFaceUV( fX, fY, fCornerWidthScaled, fCornerHeightScaled, 
//		MAGIC_TEX_OFFSET + 0.f, MAGIC_TEX_OFFSET + 2.f * MAGIC_TEX_BLOCK_HEIGHT, 
//		MAGIC_TEX_BLOCK_WIDTH, MAGIC_TEX_BLOCK_HEIGHT, 0.f, 0, backTexColor );
//
//	// bottom_center
//	DrawFaceUV( fX+fCornerWidthScaled, fY, fCenterWidthScaled, fCornerHeightScaled, 
//		MAGIC_TEX_OFFSET + MAGIC_TEX_BLOCK_HEIGHT, MAGIC_TEX_OFFSET + 2.f * MAGIC_TEX_BLOCK_HEIGHT,
//		MAGIC_TEX_BLOCK_WIDTH, MAGIC_TEX_BLOCK_HEIGHT, 0.f, 0, backTexColor );
//
//	// bottom_right_corner
//	DrawFaceUV( fX+fCornerWidthScaled+fCenterWidthScaled, fY, fCornerWidthScaled, fCornerHeightScaled, 
//		MAGIC_TEX_OFFSET + 2.f * MAGIC_TEX_BLOCK_HEIGHT, MAGIC_TEX_OFFSET + 2.f * MAGIC_TEX_BLOCK_HEIGHT,
//		MAGIC_TEX_BLOCK_WIDTH, MAGIC_TEX_BLOCK_HEIGHT, 0.f, 0, backTexColor );
//
//	// center_left
//	DrawFaceUV( fX, fY-fCenterHeightScaled, fCornerWidthScaled, fCenterHeightScaled, 
//		MAGIC_TEX_OFFSET + 0.f, MAGIC_TEX_OFFSET + 1.f * MAGIC_TEX_BLOCK_HEIGHT, 
//		MAGIC_TEX_BLOCK_WIDTH, MAGIC_TEX_BLOCK_HEIGHT, 0.f, 0, backTexColor );
//
//	// center_center	
//	DrawFaceUV( fX+fCornerWidthScaled, fY-fCenterHeightScaled, fCenterWidthScaled, fCenterHeightScaled, 
//		MAGIC_TEX_OFFSET + MAGIC_TEX_BLOCK_WIDTH, MAGIC_TEX_OFFSET + 1.f * MAGIC_TEX_BLOCK_HEIGHT, 
//		MAGIC_TEX_BLOCK_WIDTH, MAGIC_TEX_BLOCK_HEIGHT, 0.f, 0, backTexColor );
//
//	// center_right
//	DrawFaceUV( fX+fCornerWidthScaled+fCenterWidthScaled, fY-fCenterHeightScaled, fCornerWidthScaled, fCenterHeightScaled, 
//		MAGIC_TEX_OFFSET + 2.f * MAGIC_TEX_BLOCK_WIDTH, MAGIC_TEX_OFFSET + 1.f * MAGIC_TEX_BLOCK_HEIGHT, 
//		MAGIC_TEX_BLOCK_WIDTH, MAGIC_TEX_BLOCK_HEIGHT, 0.f, 0, backTexColor );
//
//	// upper_left
//	DrawFaceUV( fX, fY-fCornerHeightScaled-fCenterHeightScaled, fCornerWidthScaled, fCornerHeightScaled,
//		MAGIC_TEX_OFFSET + 0.f, MAGIC_TEX_OFFSET + 0.f, 
//		MAGIC_TEX_BLOCK_WIDTH, MAGIC_TEX_BLOCK_HEIGHT, 0.f, 0, backTexColor );
//
//	// upper_center
//	DrawFaceUV( fX+fCornerWidthScaled, fY-fCornerHeightScaled-fCenterHeightScaled, fCenterWidthScaled, fCornerHeightScaled,
//		MAGIC_TEX_OFFSET + MAGIC_TEX_BLOCK_WIDTH, MAGIC_TEX_OFFSET + 0.f, 
//		MAGIC_TEX_BLOCK_WIDTH, MAGIC_TEX_BLOCK_HEIGHT, 0.f, 0, backTexColor );
//
//	// upper_right
//	DrawFaceUV( fX+fCornerWidthScaled+fCenterWidthScaled, fY-fCornerHeightScaled-fCenterHeightScaled, fCornerWidthScaled, fCornerHeightScaled,
//		MAGIC_TEX_OFFSET + 2.f * MAGIC_TEX_BLOCK_WIDTH, MAGIC_TEX_OFFSET + 0.f, 
//		MAGIC_TEX_BLOCK_WIDTH, MAGIC_TEX_BLOCK_HEIGHT, 0.f, 0, backTexColor );
//#endif
//}
//
//// 말풍선의 뾰족한 부분을 뾰족한 부분의 시작점(fX, fY)를 중심으로 그린다
//void CX2TalkBoxManagerImp::DrawBalloonTip( int iType, float fX, float fY, float fCenterWidth, float fCenterHeight, 
//										  float fWidthScale, float fHeightScale, D3DXCOLOR backTexColor )
//{
//	KTDXPROFILE();
//
//
//	const float MAGIC_TEX_BLOCK_WIDTH  = 85.f / 512.f;
//	const float MAGIC_TEX_BLOCK_HEIGHT = 85.f / 512.f;
//
//	const float fCornerWidthScaled  = m_fTalkBoxCornerWidth  * fWidthScale;
//	const float fCornerHeightScaled = m_fTalkBoxCornerHeight * fHeightScale;
//
//
//	const float MAGIC_OFFSET_Y = fCornerHeightScaled * 0.2f; // tip이 balloon 사각형을 약간 덮을 수 있도록
//
//	float _fX = 0.0f;
//	float _fY = 0.0f; 
//	float _fRotAngle = 0.0f;
//	int _flip = 0;
//	switch( iType )
//	{
//	case TBT_FROM_DOWN_LEFT: 
//		{
//			_fX = fX - fCornerWidthScaled * 0.5f;
//			_fY = fY - MAGIC_OFFSET_Y;
//			_fRotAngle = 0.f;
//		} 
//		break;
//
//	case TBT_FROM_DOWN_RIGHT: 
//		{
//			_fX = fX - fCornerWidthScaled * 0.5f;
//			_fY = fY - MAGIC_OFFSET_Y;
//			_flip = 2;
//			_fRotAngle = 0.0f;
//
//		} 
//		break;
//
//	case TBT_FROM_UP_RIGHT: 
//		{
//			_fX = fX - fCornerWidthScaled * 0.5f;
//			_fY = fY + fCornerHeightScaled + MAGIC_OFFSET_Y;
//			_fRotAngle = 180.f;
//		} 
//		break;
//
//	case TBT_FROM_UP_LEFT: 
//		{
//			_fX = fX - fCornerWidthScaled * 0.5f;
//			_fY = fY + fCornerHeightScaled + MAGIC_OFFSET_Y;
//			_fRotAngle = 180.f;
//			_flip = 2;
//		} 
//		break;
//		/*
//		case TBT_FROM_LEFT: // LEFT
//		{
//		_fX = fX;
//		_fY = fY + fCornerHeightScaled * 0.5f;
//		_fRotAngle = 90.f;
//		} break;
//
//		case TBT_FROM_RIGHT: // RIGHT
//		{
//		_fX = fX - fCornerWidthScaled;
//		_fY = fY + fCornerHeightScaled * 0.5f;
//		_fRotAngle = 270.f;
//		} break;
//		*/
//	}
//
//	DrawFaceUV( _fX, _fY, fCornerWidthScaled, fCornerHeightScaled,
//		259.f/512.f, 1.f/512.f, 88.f/512.f, 121.f/512.f, 
//		_fRotAngle, _flip, backTexColor );
//
//}
//#endif

#ifdef NUMBER_TO_LANGUAGE
void CX2TalkBoxManagerImp::LineBreak( int& nRow, int& iColumnSize, wstring& wstrSpeech, bool bTrade  )
#else
void CX2TalkBoxManagerImp::LineBreak( int& nRow, int& iColumnSize, wstring& wstrSpeech )
#endif NUMBER_TO_LANGUAGE
{
	KTDXPROFILE();

	const wstring wstrNextLine = L"\n";

	UINT uMaxStringLength = wstrSpeech.length();
	if( (int)uMaxStringLength > MAX_ROOM_CHAT_STR_LEN )
	{
		uMaxStringLength = MAX_ROOM_CHAT_STR_LEN;
	}

	nRow = 1;
	int iUniCharSize = 0;		// pixel 단위??
	int iStringSize = 0;		// pixel 단위??

#ifdef RESIZE_BALLON
	D3DXVECTOR2 vMaxTalkBoxSize;
#ifdef REMOVE_TEXT_SCALING
	vMaxTalkBoxSize.x = static_cast<float> ( 10 * m_pUKFont->GetWidth( L'가' ) );
#else //REMOVE_TEXT_SCALING
	vMaxTalkBoxSize.x = static_cast<float> ( 10 * m_pUKFont->GetWidth( L'가' ) / g_pKTDXApp->GetResolutionScaleY() * g_pKTDXApp->GetResolutionScaleX() );
#endif //REMOVE_TEXT_SCALING
#else
	D3DXVECTOR2 vMaxTalkBoxSize = g_pKTDXApp->ConvertByResolution( m_Size );

	if( vMaxTalkBoxSize.x > m_Size.x )
		vMaxTalkBoxSize.x = m_Size.x;
#endif RESIZE_BALLON

	//정해진 크기(m_Size)에 따라 문장을 여러 행으로 나눈다
	for( UINT i=0; i<uMaxStringLength; i++ )
	{
		WCHAR wChar = wstrSpeech[i];

#ifdef RESIZE_BALLON
#ifdef REMOVE_TEXT_SCALING
		iUniCharSize = static_cast<int>( m_pUKFont->GetWidth( wChar ) );
#else //REMOVE_TEXT_SCALING
		iUniCharSize = static_cast<int>( m_pUKFont->GetWidth( wChar ) / g_pKTDXApp->GetResolutionScaleY() * g_pKTDXApp->GetResolutionScaleX() );
#endif //REMOVE_TEXT_SCALING
#else
		iUniCharSize = m_pUKFont->GetWidth( wChar );		//	'a' - 7, '가' - 14
#endif RESIZE_BALLON

		iStringSize += iUniCharSize;

		//if( iStringSize >= m_Size.x - m_fMarginLeft - m_fMarginRight )
#ifdef NUMBER_TO_LANGUAGE
		if( (iStringSize >= vMaxTalkBoxSize.x || wChar == L'\n') && false == bTrade )
#else
		if( iStringSize >= vMaxTalkBoxSize.x || wChar == L'\n' ) // fix!!
#endif NUMBER_TO_LANGUAGE
		{
			// 다음 줄의 첫 글자가 공백이 나오는 경우 제외

			if( wChar != L'\n' )
			{
				wstrSpeech.insert( wstrSpeech.begin()+i, wstrNextLine.begin(), wstrNextLine.end() );
#ifdef RESIZE_BALLON
				if ( wstrSpeech[i+1] == L' ' )
				{
					wstrSpeech.erase(i+1, 1);
				}
				else
				{
					++i;
					++uMaxStringLength;
				}
#else
				i++;
				uMaxStringLength +=1;
#endif RESIZE_BALLON
			}
			nRow++;
			iStringSize = 0;
		}
	}

	// 문장이 길면 뒤를 자른다
	if( wstrSpeech.length() > (UINT)MAX_ROOM_CHAT_STR_LEN )
	{
		wstrSpeech = wstrSpeech.substr( 0, MAX_ROOM_CHAT_STR_LEN );
	}

	//{{ JHKang / 강정훈 / 2010.08.30 / 전체화면 방식 변화에 따른 해상도별 말풍선 크기 조절
	// JHKang / 강정훈 / 2010.9.10 / 설명 및 코드 수정
	/**	@brief	: 해상도마다 달라지는 한 줄에 나오는 글자 수를 일치하도록 변경
				  m_size.x에 정해져있는 고정을 사용하지 않음
	*/
#ifdef RESIZE_BALLON
	if( nRow > 1 )
	{
		iColumnSize = static_cast<int>( vMaxTalkBoxSize.x );
	}
	else
	{
		iColumnSize = static_cast<int>( iStringSize );
	}
#else
	if( nRow > 1 )
	{
		iColumnSize = (int)m_Size.x;
	}
	else
	{
		iColumnSize = iStringSize; // + (int)m_fMarginLeft + (int)m_fMarginRight;
	}
#endif RESIZE_BALLON
	//}} JHKang / 강정훈 / 2010.08.30 / 전체화면 방식 변화에 따른 해상도별 말풍선 크기 조절
}

void CX2TalkBoxManagerImp::InitTalkBox( TalkBox& talkBox )
{
	int index = -1;

	switch( talkBox.m_TalkBoxType )
	{
	case TBT_FROM_DOWN_LEFT:
	case TBT_FROM_UP_LEFT:
		{
			talkBox.m_fTalkBoxTipLerpCoef = 0.2f;
		}
		break;

	case TBT_TALKBOX_EFFECT :
		{
#ifdef ADDED_RELATIONSHIP_SYSTEM
			talkBox.m_fTalkBoxTipLerpCoef = 0.5f;
#endif // ADDED_RELATIONSHIP_SYSTEM
		}
		break;

	case TBT_FROM_DOWN_RIGHT:
	case TBT_FROM_UP_RIGHT:
		{
			talkBox.m_fTalkBoxTipLerpCoef = 0.8f;
		}
		break;
	}

	if( true == talkBox.m_bTraceUnit )
	{
		talkBox.m_vTalkBoxSizeScale			= D3DXVECTOR2( 0, 0 );
		talkBox.m_vTalkBoxSizeScaleDelta	= D3DXVECTOR2( 0.12f, 0.12f );

		index = m_pFont->Push2DText(talkBox.m_wstrTalkContent.c_str(), 
			-5000, -5000,
			//	talkBox.m_coTextColor, talkBox.m_coTextColor, 
			talkBox.m_coTextColor, talkBox.m_OutLineTextColor, 
			DT_LEFT, CKTDGFontManager::FS_NONE, false, true, 0.1f );
	}
	else
	{
		const float fCenterWidth  = talkBox.m_vTalkBoxSize.x - m_fMarginLeft - m_fMarginLeft;
		const float fCenterHeight = talkBox.m_vTalkBoxSize.y - m_fMarginTop - m_fMarginTop;
		const float fCornerWidth  = m_fMarginLeft;
		const float fCornerHeight = m_fMarginTop;
		const float fCornerWidthScaled  = fCornerWidth * talkBox.m_vTalkBoxSizeScale.x;
		const float fCornerHeightScaled = fCornerHeight * talkBox.m_vTalkBoxSizeScale.y;
		const float fCenterWidthScaled  = fCenterWidth * talkBox.m_vTalkBoxSizeScale.x;
		const float fCenterHeightScaled = fCenterHeight * talkBox.m_vTalkBoxSizeScale.y;

		D3DXVECTOR2 pos = D3DXVECTOR2( 0, 0 );

		if ( talkBox.m_TalkBoxType == TBT_FROM_DOWN_LEFT || talkBox.m_TalkBoxType == TBT_FROM_DOWN_RIGHT )
		{
			pos.x = talkBox.m_vPos.x - fCenterWidthScaled * talkBox.m_fTalkBoxTipLerpCoef;
			pos.y = talkBox.m_vPos.y - fCenterHeightScaled - fCornerHeightScaled / g_pKTDXApp->GetResolutionScaleY(); // ??????
		}
		else
		{
			pos.x = talkBox.m_vPos.x - fCenterWidthScaled * talkBox.m_fTalkBoxTipLerpCoef;
#ifdef RESIZE_BALLON
			pos.y = talkBox.m_vPos.y + (fCornerHeight * 3 / g_pKTDXApp->GetResolutionScaleY());
#else
			pos.y = talkBox.m_vPos.y + fCornerHeight + 10 /* 미세 조정 값 */ + fCornerHeightScaled / g_pKTDXApp->GetResolutionScaleY(); // ??????
#endif RESIZE_BALLON
		}

		index = m_pFont->Push2DText(talkBox.m_wstrTalkContent.c_str(), 
			(int)pos.x, (int)pos.y, talkBox.m_coTextColor, talkBox.m_OutLineTextColor, //talkBox.m_coTextColor, 
			DT_LEFT, CKTDGFontManager::FS_NONE, false, true, 0.1f );
	}

	talkBox.m_iFontArticleID = index;
}

void CX2TalkBoxManagerImp::ResetTalkBox( TalkBox& talkBox )
{
	int index = -1;

	switch( talkBox.m_TalkBoxType )
	{
	case TBT_FROM_DOWN_LEFT:
	case TBT_FROM_UP_LEFT:
		{
			talkBox.m_fTalkBoxTipLerpCoef = 0.2f;
		}
		break;
	case TBT_TALKBOX_EFFECT :
		{
#ifdef ADDED_RELATIONSHIP_SYSTEM
			// 0 으로 하면 0 을 나누는 경우가 있어 크래시 남
			m_fMarginLeft		= 20.0f;
			m_fMarginRight		= 0.1f;
			m_fMarginTop		= 0.1f;
			m_fMarginBottom		= 0.1f;

			m_fTalkBoxCornerWidth	= 0.1f;		
			m_fTalkBoxCornerHeight	= 0.1f; 

			talkBox.m_fTalkBoxTipLerpCoef = 0.5f;
#endif // 		ADDED_RELATIONSHIP_SYSTEM
		}
		break;
	case TBT_FROM_DOWN_RIGHT:
	case TBT_FROM_UP_RIGHT:
		{
			talkBox.m_fTalkBoxTipLerpCoef = 0.8f;
		}
		break;
	}

	if( true == talkBox.m_bTraceUnit )
	{
		talkBox.m_vTalkBoxSizeScale			= D3DXVECTOR2( 0, 0 );
		talkBox.m_vTalkBoxSizeScaleDelta	= D3DXVECTOR2( 0.12f, 0.12f );
	}
	else
	{
		const float fCenterWidth  = talkBox.m_vTalkBoxSize.x - m_fMarginLeft - m_fMarginLeft;
		const float fCenterHeight = talkBox.m_vTalkBoxSize.y - m_fMarginTop - m_fMarginTop;
		const float fCornerWidth  = m_fMarginLeft;
		const float fCornerHeight = m_fMarginTop;
		const float fCornerWidthScaled  = fCornerWidth * talkBox.m_vTalkBoxSizeScale.x;
		const float fCornerHeightScaled = fCornerHeight * talkBox.m_vTalkBoxSizeScale.y;
		const float fCenterWidthScaled  = fCenterWidth * talkBox.m_vTalkBoxSizeScale.x;
		const float fCenterHeightScaled = fCenterHeight * talkBox.m_vTalkBoxSizeScale.y;

		D3DXVECTOR2 pos = D3DXVECTOR2( 0, 0 );

		if ( talkBox.m_TalkBoxType == TBT_FROM_DOWN_LEFT || talkBox.m_TalkBoxType == TBT_FROM_DOWN_RIGHT )
		{
			pos.x = talkBox.m_vPos.x - fCenterWidthScaled * talkBox.m_fTalkBoxTipLerpCoef;
			pos.y = talkBox.m_vPos.y - fCenterHeightScaled - fCornerHeightScaled / g_pKTDXApp->GetResolutionScaleY(); // ??????
		}
		else
		{
			pos.x = talkBox.m_vPos.x - fCenterWidthScaled * talkBox.m_fTalkBoxTipLerpCoef;
#ifdef RESIZE_BALLON
			pos.y = talkBox.m_vPos.y + (fCornerHeight * 3 / g_pKTDXApp->GetResolutionScaleY());
#else
			pos.y = talkBox.m_vPos.y + fCornerHeight + 10 /* 미세 조정 값 */ + fCornerHeightScaled / g_pKTDXApp->GetResolutionScaleY(); // ??????
#endif RESIZE_BALLON
		}

		talkBox.ResetFontPos( m_pFont, m_fMarginLeft, m_fMarginTop );
	}
}

//#ifndef DYNAMIC_VERTEX_BUFFER_OPT
//void CX2TalkBoxManagerImp::DrawFace( float nX, float nY, float nWidth, float nHeight, D3DCOLOR color /* = 0xffffffff  */)
//{
//	KTDXPROFILE();
//
//	KD3DPUSH( m_RenderStateID )
//
//		DRAWFACE_RHW_VERTEX vertex[4];
//	ZeroMemory( vertex, sizeof(DRAWFACE_RHW_VERTEX) * 4 );
//
//	float X1 = (float)nX - 0.5f;
//	float Y1 = (float)nY - 0.5f;
//	float X2 = (float)(nX + nWidth) - 0.5f;
//	float Y2 = (float)(nY + nHeight) - 0.5f;
//
//	float temp =  1.0f / nHeight;
//
//	float UMax = 1.0f;
//	float VMax = 1.0f - temp;
//
//	//Set up the 4 corners of a small square
//	vertex[0].x = X1;     vertex[0].y = Y1;
//	vertex[0].z = 1.0f;   vertex[0].rhw = 1.0f;
//	vertex[0].u = 0.0f;   vertex[0].v = temp;
//
//	vertex[1].x = X1;     vertex[1].y = Y2;
//	vertex[1].u = 0.0f;   vertex[1].v = VMax;
//	vertex[1].z = 1.0f;   vertex[1].rhw = 1.0f;
//
//	vertex[2].x = X2;     vertex[2].y = Y1;
//	vertex[2].u = UMax;   vertex[2].v = temp;
//	vertex[2].z = 1.0f;   vertex[2].rhw = 1.0f;
//
//	vertex[3].x = X2;     vertex[3].y = Y2;
//	vertex[3].u = UMax;   vertex[3].v = VMax;
//	vertex[3].z = 1.0f;   vertex[3].rhw = 1.0f;
//
//	vertex[0].color = vertex[1].color = vertex[2].color = vertex[3].color = color;
//
//	g_pKTDXApp->GetDevice()->SetFVF( D3DFVF_DRAWFACE_RHW_VERTEX );
//	g_pKTDXApp->GetDevice()->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, vertex, sizeof(DRAWFACE_RHW_VERTEX) );
//
//	KD3DEND()
//}
//
//void CX2TalkBoxManagerImp::DrawReverseFace( float nX, float nY, float nWidth, float nHeight, D3DCOLOR color /* = 0xffffffff  */)
//{
//	KTDXPROFILE();
//
//	KD3DPUSH( m_RenderStateID )
//
//		DRAWFACE_RHW_VERTEX vertex[4];
//	ZeroMemory( vertex, sizeof(DRAWFACE_RHW_VERTEX) * 4 );
//
//	float X1 = (float)nX - 0.5f;
//	float Y1 = (float)nY - 0.5f;
//	float X2 = (float)(nX + nWidth);
//	float Y2 = (float)(nY + nHeight);
//
//	float temp =  1.0f / nHeight;
//
//	float UMax = 1.0f;
//	float VMax = 1.0f - temp;
//
//	//Set up the 4 corners of a small square
//	vertex[0].x = X2;     vertex[0].y = Y2;
//	vertex[0].z = 1.0f;   vertex[0].rhw = 1.0f;
//	vertex[0].u = 0.0f;   vertex[0].v = temp;
//
//	vertex[1].x = X2;     vertex[1].y = Y1;
//	vertex[1].z = 1.0f;   vertex[1].rhw = 1.0f;
//	vertex[1].u = 0.0f;   vertex[1].v = VMax;
//
//	vertex[2].x = X1;     vertex[2].y = Y2;
//	vertex[2].z = 1.0f;   vertex[2].rhw = 1.0f;
//	vertex[2].u = UMax;   vertex[2].v = temp;
//
//	vertex[3].x = X1;     vertex[3].y = Y1;
//	vertex[3].z = 1.0f;   vertex[3].rhw = 1.0f;
//	vertex[3].u = UMax;   vertex[3].v = VMax;
//
//	vertex[0].color = vertex[1].color = vertex[2].color = vertex[3].color = color;
//
//	g_pKTDXApp->GetDevice()->SetFVF( D3DFVF_DRAWFACE_RHW_VERTEX );
//	g_pKTDXApp->GetDevice()->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, vertex, sizeof(DRAWFACE_RHW_VERTEX) );
//
//	KD3DEND()
//}
//#endif




#ifdef KTDGDEVICEFONT_SIMULATE_DIRECTX_FONT
void CX2TalkBoxManagerImp::TalkBox::Update( CKTDGFontManager::CUKFont* pKTDGFont, const float fMarginLeft, const float fMarginTop, const D3DXVECTOR2 vOffset )
#else
void CX2TalkBoxManagerImp::TalkBox::Update( CKTDGFontManager::CKTDGFont* pKTDGFont, const float fMarginLeft, const float fMarginTop, const D3DXVECTOR2 vOffset )
#endif
{	
	KTDXPROFILE();

	const float MAGIC_SHRINKING_TIME = 0.4f;
	if( true == m_bTraceUnit && m_fRemainTime < MAGIC_SHRINKING_TIME )
	{
		pKTDGFont->Delete2DFont( m_iFontArticleID );
		m_vTalkBoxSizeScaleDelta = D3DXVECTOR2( -0.05f, -0.05f );
	}

	// 크기 변경
	m_vTalkBoxSizeScale += m_vTalkBoxSizeScaleDelta;
	if( m_vTalkBoxSizeScale.x < 0.f )
	{
		m_vTalkBoxSizeScale.x = 0.f;
		m_vTalkBoxSizeScaleDelta.x = 0.f;
	}
	if( m_vTalkBoxSizeScale.x > 1.f )
	{
		m_vTalkBoxSizeScale.x = 1.f;
		m_vTalkBoxSizeScaleDelta.x = 0.f;
	}
	if( m_vTalkBoxSizeScale.y < 0.f )
	{
		m_vTalkBoxSizeScale.y = 0.f;
		m_vTalkBoxSizeScaleDelta.y = 0.f;
	}
	if( m_vTalkBoxSizeScale.y > 1.f )
	{
		m_vTalkBoxSizeScale.y = 1.f;
		m_vTalkBoxSizeScaleDelta.y = 0.f;
	}



	// 이부분 전체 fix!!
#ifndef RESIZE_BALLON
	const float fCenterWidth  = m_vTalkBoxSize.x - fMarginLeft - fMarginLeft;
	const float fCenterHeight = m_vTalkBoxSize.y - fMarginTop - fMarginTop;
	const float fCornerWidth  = fMarginLeft;
	const float fCornerHeight = fMarginTop;
	const float fCornerWidthScaled  = fCornerWidth * m_vTalkBoxSizeScale.x;
	const float fCornerHeightScaled = fCornerHeight * m_vTalkBoxSizeScale.y;
	const float fCenterWidthScaled  = fCenterWidth * m_vTalkBoxSizeScale.x;
	const float fCenterHeightScaled = fCenterHeight * m_vTalkBoxSizeScale.y;
#endif RESIZE_BALLON

	// 위치 변경 
	if( true == m_bTraceUnit )
	{
		D3DXVECTOR3 vPos(0, 0, 0) ;

		if( -1 != m_OwnerUnitUID )
			GetTalkBoxPosByUnitUID( m_OwnerUnitUID, vPos );
#ifdef ADDED_RELATIONSHIP_SYSTEM
		else
			GetTalkBoxPosByCostomPos( vPos );
#endif // ADDED_RELATIONSHIP_SYSTEM

		if( false == m_bShow )
			return;


#ifdef ADDED_RELATIONSHIP_SYSTEM			
		if ( m_TalkBoxType != TBT_TALKBOX_EFFECT )
#endif // ADDED_RELATIONSHIP_SYSTEM
		{
			vPos.y += 70.f; // MAGIC
		}
		m_vPos = g_pKTDXApp->GetProj3DPos( vPos );

#ifdef ADDED_RELATIONSHIP_SYSTEM
		if ( m_TalkBoxType != TBT_TALKBOX_EFFECT )
#endif // ADDED_RELATIONSHIP_SYSTEM
		{
			m_vPos.x += 30.f; // MAGIC
		}
		m_vPos.x /= g_pKTDXApp->GetResolutionScaleX();
 		m_vPos.y /= g_pKTDXApp->GetResolutionScaleY();

		m_vPos += vOffset;

		ResetFontPos( pKTDGFont, fMarginLeft, fMarginTop );

	}

}

#ifdef KTDGDEVICEFONT_SIMULATE_DIRECTX_FONT
void CX2TalkBoxManagerImp::TalkBox::ResetFontPos( CKTDGFontManager::CUKFont* pKTDGFont, const float fMarginLeft, const float fMarginTop)
#else
void CX2TalkBoxManagerImp::TalkBox::ResetFontPos( CKTDGFontManager::CKTDGFont* pKTDGFont, const float fMarginLeft, const float fMarginTop)
#endif
{
	// 이부분 전체 fix!!
	const float fCenterWidth  = m_vTalkBoxSize.x - fMarginLeft - fMarginLeft;
	const float fCenterHeight = m_vTalkBoxSize.y - fMarginTop - fMarginTop;
	const float fCornerWidth  = fMarginLeft;
	const float fCornerHeight = fMarginTop;
	const float fCornerWidthScaled  = fCornerWidth * m_vTalkBoxSizeScale.x;
	const float fCornerHeightScaled = fCornerHeight * m_vTalkBoxSizeScale.y;
	const float fCenterWidthScaled  = fCenterWidth * m_vTalkBoxSizeScale.x;
	const float fCenterHeightScaled = fCenterHeight * m_vTalkBoxSizeScale.y;

	// font article의 위치 변경
	if( m_vTalkBoxSizeScale == D3DXVECTOR2(1.f,1.f) )
	{					
		CKTDGFontManager::FontArticle* pArticle = pKTDGFont->Get2DArticle( m_iFontArticleID );
		if( NULL != pArticle )
		{
			switch( m_TalkBoxType )
			{

			case TBT_TALKBOX_EFFECT :
#ifdef ADDED_RELATIONSHIP_SYSTEM
				{	
					pArticle->pos.x = m_vPos.x - fCenterWidthScaled * m_fTalkBoxTipLerpCoef;
					pArticle->pos.y = m_vPos.y - fCenterHeightScaled - fCornerHeightScaled / g_pKTDXApp->GetResolutionScaleY(); // ??????
//					pArticle->pos.y = m_vPos.y + (fCornerHeight * 3 / g_pKTDXApp->GetResolutionScaleY());
				} 
#endif // ADDED_RELATIONSHIP_SYSTEM
				break;

			case TBT_FROM_DOWN_LEFT:
			case TBT_FROM_DOWN_RIGHT:
				{	
					pArticle->pos.x = m_vPos.x - fCenterWidthScaled * m_fTalkBoxTipLerpCoef;
					pArticle->pos.y = m_vPos.y - fCenterHeightScaled - fCornerHeightScaled / g_pKTDXApp->GetResolutionScaleY(); // ??????
				} 
				break;

			case TBT_FROM_UP_RIGHT:
			case TBT_FROM_UP_LEFT:
				{
					pArticle->pos.x = m_vPos.x - fCenterWidthScaled * m_fTalkBoxTipLerpCoef;
#ifdef RESIZE_BALLON
					pArticle->pos.y = m_vPos.y + (fCornerHeight * 3 / g_pKTDXApp->GetResolutionScaleY());
#else
					pArticle->pos.y = m_vPos.y + fCornerHeight + 10 /* 미세 조정 값 */ + fCornerHeightScaled / g_pKTDXApp->GetResolutionScaleY(); // ??????
#endif RESIZE_BALLON
				} 
				break;
				/*
				case TBT_FROM_LEFT:
				{
				//fix!!
				//pArticle->pos.x = pos.x - fCornerWidthScaled;
				//pArticle->pos.y = pos.y + fCenterHeightScaled * m_fTalkBoxTipLerpCoef ; - fCornerHeightScaled / g_pKTDXApp->GetResolutionScaleY(); // ??????/						
				} break;

				case TBT_FROM_RIGHT:
				{
				//pArticle->pos.x = pos.x - fCenterWidthScaled * m_fTalkBoxTipLerpCoef;
				//pArticle->pos.y = pos.y + fCenterHeightScaled - fCornerHeightScaled / g_pKTDXApp->GetResolutionScaleY(); // ??????/	
				} break;
				*/
			}
		}
	}
}


void CX2TalkBoxManagerImp::MouseMove( D3DXVECTOR2 mousePos )
{
	UidType unitUID = 0;
	if ( CheckMousePointInTalkbox( mousePos, unitUID ) == true )
	{
		CX2TalkBoxManagerImp::TalkBox* pTalkBox = GetTalkBoxByUID( unitUID );
		if ( pTalkBox != NULL )
		{
			if ( m_NowOverTalkBoxUnitUID != unitUID )
			{
				CX2TalkBoxManagerImp::TalkBox* pTalkBox = GetTalkBoxByUID( m_NowOverTalkBoxUnitUID );
				if ( pTalkBox != NULL )
				{
					pTalkBox->m_BackTexColor = D3DXCOLOR( 1,1,1,1 );
				}

				if ( m_pSoundOver != NULL )
					m_pSoundOver->Play( false, false );
			}

			pTalkBox->m_BackTexColor = D3DXCOLOR( 1,1,0,1 );

			m_NowOverTalkBoxUnitUID = unitUID;
		}
	}
	else
	{
		if ( m_NowOverTalkBoxUnitUID != 0 )
		{
			CX2TalkBoxManagerImp::TalkBox* pTalkBox = GetTalkBoxByUID( m_NowOverTalkBoxUnitUID );
			if ( pTalkBox != NULL )
			{
				pTalkBox->m_BackTexColor = D3DXCOLOR( 1,1,1,1 );
			}
		}
		 
		m_NowOverTalkBoxUnitUID = 0;
	}
}

void CX2TalkBoxManagerImp::TalkBox::GetTalkBoxPosByUnitUID( IN UidType uiOwnerUnitUID_, OUT D3DXVECTOR3& vPos_ )
{
	switch( m_GameUnitType )
	{
	case CX2GameUnit::GUT_USER:
		{
#ifdef SERV_PET_SYSTEM
			if( m_bPetTalk == true )
			{
				CX2PET *pPet = g_pData->GetPetManager()->GetPet(uiOwnerUnitUID_);
				if( pPet != NULL )
				{
					if( pPet->GetShowObject() != m_bShow )
						m_bShow = pPet->GetShowObject();

					if( pPet->GetXSkinAnim() != NULL )
						vPos_ = pPet->GetBonePos( L"Bip01_Head" );
					else
						m_bShow = false;						
				}
				else
				{
					m_bShow = false;
				}
				break;
			}
#endif
			switch( g_pMain->GetNowStateID() )
			{

			case CX2Main::XS_VILLAGE_MAP:
				{
					CX2SquareUnit* pSquareUnit = g_pTFieldGame->GetSquareUnitByUID( uiOwnerUnitUID_ );
					if( NULL != pSquareUnit )
					{
						{
							if( CX2SquareUnit::PSS_SHOP == pSquareUnit->GetPersonalShopState() || g_pMain->GetGameOption().GetFieldSD() == true )
							{
								vPos_ = pSquareUnit->GetPos() + D3DXVECTOR3(0, 150, 0);
							}
							else
							{
								vPos_ = pSquareUnit->GetBonePos( L"Bip01_Head" );
							}
							if ( pSquareUnit->GetShowObject() != m_bShow )
								m_bShow = pSquareUnit->GetShowObject();
						}
					}
				} break;

			case CX2Main::XS_SQUARE_GAME:
				{
					CX2SquareUnit* pSquareUnit = g_pSquareGame->GetSquareUnitByUID( uiOwnerUnitUID_ );
					if( NULL != pSquareUnit )
					{
						if( CX2SquareUnit::PSS_SHOP == pSquareUnit->GetPersonalShopState() )
						{
							vPos_ = pSquareUnit->GetPos() + D3DXVECTOR3(0, 150, 0);
						}
						else
						{
							vPos_ = pSquareUnit->GetBonePos( L"Bip01_Head" );
						}
					}
				} break;

#ifdef ADDED_RELATIONSHIP_SYSTEM
			case CX2Main::XS_WEDDING_GAME:
				{
					if( NULL != g_pWeddingGame )
					{
						CX2SquareUnit* pSquareUnit = g_pWeddingGame->GetSquareUnitByUID( uiOwnerUnitUID_ );
						if( NULL != pSquareUnit )
						{
							vPos_ = pSquareUnit->GetBonePos( L"Bip01_Head" );
						}
					}
				} break;
#endif // ADDED_RELATIONSHIP_SYSTEM
			default:
				{
					CX2GUUser* pCX2GUUser = g_pX2Game->GetUserUnitByUID( uiOwnerUnitUID_ );
					if( NULL != pCX2GUUser )
					{
						// talkbox 그림 위치 변경
						vPos_ = pCX2GUUser->GetBonePos( L"Bip01_Head" );

						//{{ robobeg : 2008-10-21
						//if( pCX2GUUser->GetShow() != m_bShow )
						//{
						//	m_bShow = pCX2GUUser->GetShow();
						//}
						if ( pCX2GUUser->GetShowObject() != m_bShow )
							m_bShow = pCX2GUUser->GetShowObject();
						//}} robobeg : 2008-10-21
					}
				} break;
			}
		} break;

	case CX2GameUnit::GUT_NPC:
		{							
#ifdef TALK_BOX_VILLAGE_NPC
			if( m_bExtraVariable == true )
			{
				CX2TFieldNpc* pCX2TFieldNpc = g_pTFieldGame->GetHouseFieldNPC( (int)uiOwnerUnitUID_ );

				if ( NULL != pCX2TFieldNpc )
				{
					vPos_ = pCX2TFieldNpc->GetHeadBonePos();
					vPos_.y -= 30.0f; // MAGIC
					vPos_.x -= 80.0f; // MAGIC

					if ( pCX2TFieldNpc->GetShowObject() != m_bShow )
						m_bShow = pCX2TFieldNpc->GetShowObject();
				}
				break;
			}
#endif
			CX2GUNPC* pCX2GUNPC = g_pX2Game->GetNPCUnitByUID( (int)uiOwnerUnitUID_ );
			if( NULL != pCX2GUNPC )
			{		
				// talkbox 그림 위치 변경
				vPos_ = pCX2GUNPC->GetBonePos( pCX2GUNPC->GetHeadBoneName().c_str() );

				//{{ robobeg : 2008-10-21
				//if( pCX2GUNPC->GetShow() != m_bShow )
				//{
				//	m_bShow = pCX2GUNPC->GetShow();
				//}
				if ( pCX2GUNPC->GetShowObject() != m_bShow )
					m_bShow = pCX2GUNPC->GetShowObject();
				//}} robobeg : 2008-10-21

			}
		} break;
	}		
}

#ifdef ADDED_RELATIONSHIP_SYSTEM
void CX2TalkBoxManagerImp::TalkBox::GetTalkBoxPosByCostomPos( OUT D3DXVECTOR3 & vPos_ ) const
{
	vPos_ = m_vTalkBoxPosition;
}
#endif // ADDED_RELATIONSHIP_SYSTEM