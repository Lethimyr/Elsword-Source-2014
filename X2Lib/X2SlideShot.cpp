#include "StdAfx.h"
#include ".\x2slideshot.h"

CX2SlideShot::CX2SlideShot(void):
m_pNowState(NULL),		
m_bNowPresent(false),	
m_TextBoxShow(false),	
m_NowScenNum(0),
m_fScenTimeBefore(0.f),
m_fScenTime(0.f),
m_fElapsedTimeAfterTextSpread(0.f),
m_pDLGSlideShot(NULL),
m_bSetCursor(false)
#ifdef CHECK_VOICE_IN_SLIDESHOT
, m_fElapsedTimeAfterVoiceStopped(0.f)
#endif //CHECK_VOICE_IN_SLIDESHOT
{
	m_pDLGSlideShot		= new CKTDGUIDialog( (CKTDXStage*) m_pNowState, L"DLG_NewSlideShot.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGSlideShot );
	m_pDLGSlideShot->SetShowEnable( m_TextBoxShow, m_TextBoxShow );
}

CX2SlideShot::~CX2SlideShot(void)
{
	ClearSeq();
	m_TextBoxShow = false;
	SAFE_DELETE_DIALOG( m_pDLGSlideShot );
}

#ifdef CHECK_VOICE_IN_SLIDESHOT
float CX2SlideShot::GetElapsedTimeAfterVoiceStopped()
{
	if( !m_vecSoundList.empty() )
		return 0.0f;

	return m_fElapsedTimeAfterVoiceStopped;
}
#endif //CHECK_VOICE_IN_SLIDESHOT
void CX2SlideShot::OnFrameMove( float fElapsedTime )
{
#ifdef CHECK_VOICE_IN_SLIDESHOT
	vector<wstring>::iterator vitSoundList;
	for( vitSoundList = m_vecSoundList.begin(); vitSoundList != m_vecSoundList.end(); )
	{
		if( g_pKTDXApp->GetDeviceManager()->IsPlaying( vitSoundList->c_str() ) == false )
		{
			m_vecSoundList.erase( vitSoundList );
			if( m_vecSoundList.empty() )
				m_fElapsedTimeAfterVoiceStopped = 0.0f;
		}
		else
		{
			++vitSoundList;
		}
	}
	m_fElapsedTimeAfterVoiceStopped += fElapsedTime;
#endif //CHECK_VOICE_IN_SLIDESHOT

	if( m_bNowPresent == true )
	{
		if( NULL == g_pX2Game )
			return;

		if( NULL != g_pX2Game && CX2Game::GT_DUNGEON == g_pX2Game->GetGameType() )
		{

			if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_RETURN) == TRUE 
				|| g_pKTDXApp->GetDIManager()->GetMouse()->GetButtonState(MOUSE_LBUTTON) == TRUE )
			{
				if( NULL != g_pX2Game )
					g_pX2Game->SetSkipKeyPress(true);
			}
			else
			{
				if( NULL != g_pX2Game )
					g_pX2Game->SetSkipKeyPress(false);
			}
			//씬 종료
#ifdef FIX_BESMA_SECRET_DUNGEON_EVENT_SCENE			/// 튜토리얼 외에는 이벤트신 스킵되면 않된다.
			if( ( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_ESCAPE) == TRUE 
				|| g_pKTDXApp->GetDIManager()->GetMouse()->GetButtonState(MOUSE_RBUTTON) == TRUE )
				&& NULL != g_pMain && true == g_pMain->GetIsPlayingTutorial() )
#else  FIX_BESMA_SECRET_DUNGEON_EVENT_SCENE
			if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_ESCAPE) == TRUE 
				|| g_pKTDXApp->GetDIManager()->GetMouse()->GetButtonState(MOUSE_RBUTTON) == TRUE )
#endif FIX_BESMA_SECRET_DUNGEON_EVENT_SCENE
			{
				ScenEnd();
			}

			if( g_pX2Game != NULL )
			{
				g_pX2Game->SetShowAllUnitGageBar( false );
				g_pX2Game->SetRenderUserName( false );
			}

			CKTDGUIStatic* pStatic_Speech	= (CKTDGUIStatic*) m_pDLGSlideShot->GetControl( L"Speech" );
			if( NULL != pStatic_Speech )
			{
				if( true == pStatic_Speech->GetString(0)->bSpread )
				{
					m_fElapsedTimeAfterTextSpread = 0.f;
				}
				else
				{
					m_fElapsedTimeAfterTextSpread += fElapsedTime;
				}
			}

			m_pDLGSlideShot->SetShowEnable( m_TextBoxShow, m_TextBoxShow );

#ifdef SERV_CATCH_HACKUSER_INFO
			{
				std::wstringstream strstm;
				strstm << " m_SceneName = " << m_ScenName.c_str() << m_NowScenNum;

				if(g_pX2Game == NULL)
				{
					strstm << " g_pX2Game = NULL";
				}

				ErrorLogMsg( 0, strstm.str().c_str() );
			}
#endif SERV_CATCH_HACKUSER_INFO

			m_fScenTimeBefore = m_fScenTime;
			m_fScenTime += fElapsedTime;
			stringstream funcName;
			funcName << m_ScenName.c_str() << m_NowScenNum;
			lua_tinker::call<void>( g_pKTDXApp->GetLuaBinder()->GetLuaState(),  funcName.str().c_str(), g_pKTDXApp, static_cast<CX2DungeonGame*>(g_pX2Game), g_pData, this );
		}
	}	
}

void CX2SlideShot::ScenStart_LUA( char* pScenName, int scenNum )
{
    wstring wstrScenName;
    ConvertUtf8ToWCHAR( wstrScenName, pScenName );
    ConvertWCHARToChar( m_ScenName, wstrScenName.c_str() );
	m_bNowPresent	= true;
	m_NowScenNum	= scenNum;
	m_fScenTimeBefore	= 0.0f;
	m_fScenTime			= 0.0f;	

	if( false == m_bSetCursor)
	{
		CX2State*	pState	= static_cast< CX2State* >( g_pMain->GetNowState() );
		CX2Cursor*	pCursor = pState->GetCursor();
		pCursor->ChangeCursorState( CX2Cursor::XCS_EVENT_SCENE );
		m_bSetCursor = true;
	}

	if( NULL != g_pX2Game &&
		NULL != g_pX2Game->GetWorld() )
	{
		g_pX2Game->GetWorld()->StopAllEffectBGM();
	}
}

void CX2SlideShot::ScenWait()
{
	//엔터키 표시가 나와야 한다.
}

void CX2SlideShot::ScenEnd()
{
	ClearSeq();
	m_bNowPresent		= false;
	m_TextBoxShow		= false;
	m_NowScenNum		= 0;
	m_fScenTimeBefore	= 0.0f;
	m_fScenTime			= 0.0f;

	ClearText();
	BlackLayOutMove( true, 1 );
	m_pDLGSlideShot->SetShowEnable( m_TextBoxShow, m_TextBoxShow );

	if( g_pX2Game != NULL )
	{
		g_pX2Game->SetShowAllUnitGageBar( true );
		g_pX2Game->SetRenderUserName( true );
	}
	StopAllSound_LUA();
	ResetBGM();

	if( true == m_bSetCursor)
	{
		CX2State*	pState	= static_cast< CX2State* >( g_pMain->GetNowState() );
		CX2Cursor*	pCursor = pState->GetCursor();
		pCursor->ChangeCursorState( CX2Cursor::XCS_NORMAL );
		m_bSetCursor = false;
	}

	if( NULL != g_pX2Game &&
		NULL != g_pX2Game->GetWorld() )
	{
		g_pX2Game->GetWorld()->PlayAllEffectBGM();
	}
}


void CX2SlideShot::GoNextScen()
{
#ifdef CHECK_VOICE_IN_SLIDESHOT
	m_fElapsedTimeAfterVoiceStopped = 0.0f;
#endif //CHECK_VOICE_IN_SLIDESHOT

	//ClearSeq();
	ScenStart_LUA( (char*)m_ScenName.c_str(), m_NowScenNum+1 );	
}

void CX2SlideShot::ClearSeq()
{
	map<string, CKTDGParticleSystem::CParticleEventSequenceHandle>::iterator iter;
	for( iter = m_SeqMap.begin(); iter != m_SeqMap.end(); ++iter )
	{
		CKTDGParticleSystem::CParticleEventSequenceHandle hSeq = (CKTDGParticleSystem::CParticleEventSequenceHandle) iter->second;
		g_pData->GetUIMajorParticle()->DestroyInstanceHandle( hSeq );
	}	
	m_SeqMap.clear();
}

bool CX2SlideShot::EventTimer( float fTime )
{
	if( fTime > m_fScenTimeBefore && fTime <= m_fScenTime )
	{
		return true;
	}
	return false;
}

void CX2SlideShot::CannotInput( float fTime )
{
	if( g_pX2Game != NULL )
	{
		if( g_pX2Game->GetMyUnit() != NULL )
		{
			g_pX2Game->GetMyUnit()->SetCanNotInputTime_LUA( fTime );
		}
	}
}

void CX2SlideShot::AddText_LUA( bool bNameLeft, int NameID, int TextID, float fFullTime, char* pColor )
{
	wstring text = GET_STRING( TextID );
	wstring name = GET_STRING( NameID );
	wstring color;
	
	ConvertUtf8ToWCHAR( color, pColor );

	
	if( fFullTime <= 0.0f )
		AddText( bNameLeft, name.c_str(), text.c_str() );
	else
	{
		float fTime = fFullTime / (float)(text.size());
		AddText( bNameLeft, name.c_str(), text.c_str(), color.c_str(), true, fTime );
	}
}

void CX2SlideShot::AddText2_LUA( bool bNameLeft, int NameID, int TextID, float fTime, char* pColor )
{
	wstring text = GET_STRING( TextID );
	wstring name = GET_STRING( NameID );
	wstring color;
	
	ConvertUtf8ToWCHAR( color, pColor );

	AddText( bNameLeft, name.c_str(), text.c_str(), color.c_str(), true, fTime );
}

void CX2SlideShot::AddText( bool bNameLeft, const WCHAR* wszName, const WCHAR* wszMsg,
							wstring wstrColor /*= L""*/, bool bSpread /*= true*/, float fSpreadTime /*= 0.1f*/ )
{
	if( NULL == m_pDLGSlideShot )
		return;

	D3DXVECTOR2 vTemp = g_pKTDXApp->ConvertByResolution( 750, 512 );
	const int CHAT_LINE_WIDTH = (int)vTemp.x;

	wstring nextLineString = L"\n";
	wstring chatContent = wszMsg;

#ifdef CLIENT_GLOBAL_LINEBREAK
	CKTDGUIStatic* pStatic_Speech	= (CKTDGUIStatic*) m_pDLGSlideShot->GetControl( L"Speech" );
	if( NULL == pStatic_Speech )
		return;

	CKTDGFontManager::CUKFont* pFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( pStatic_Speech->GetString(0)->fontIndex );
	if( NULL == pFont )
		return; 
		
	int addRow = CWordLineHandler::LineBreakInX2Main(chatContent, pFont, CHAT_LINE_WIDTH, wstrColor, true, false);
#else //#ifdef CLIENT_GLOBAL_LINEBREAK
	int addRow = LineBreak( CHAT_LINE_WIDTH, chatContent, nextLineString.c_str(), wstrColor );
	CKTDGUIStatic* pStatic_Speech	= (CKTDGUIStatic*) m_pDLGSlideShot->GetControl( L"Speech" );
#endif //CLIENT_GLOBAL_LINEBREAK
	
	if( -1 == addRow )
		return;


	CKTDGUIStatic* pStatic_Name		= (CKTDGUIStatic*) m_pDLGSlideShot->GetControl( L"Name" );

	pStatic_Name->GetString(0)->msg		= wszName;
	pStatic_Speech->GetString(0)->msg	= chatContent;

	if( true == bSpread )
	{
		pStatic_Speech->GetString(0)->bSpread = bSpread;
		pStatic_Speech->GetString(0)->fNowSpreadTime = 0.f;
		pStatic_Speech->GetString(0)->fSpreadTimeGap = fSpreadTime;
		pStatic_Speech->GetString(0)->iSpreadCount = 0;
	}

	CKTDGUIStatic* pStatic_NameTab	= (CKTDGUIStatic*) m_pDLGSlideShot->GetControl( L"NameTab" );
	if( NULL != pStatic_NameTab && NULL != pStatic_Name )
	{
		if( true == bNameLeft )
		{
			D3DXVECTOR3 vPos = m_pDLGSlideShot->GetDummyPos(0);
			pStatic_NameTab->SetOffsetPos( D3DXVECTOR2( vPos.x, vPos.y ) );
			pStatic_Name->SetOffsetPos( D3DXVECTOR2( vPos.x, vPos.y ) );
		}
		else
		{
			D3DXVECTOR3 vPos = m_pDLGSlideShot->GetDummyPos(1);
			pStatic_NameTab->SetOffsetPos( D3DXVECTOR2( vPos.x, vPos.y ) );
			pStatic_Name->SetOffsetPos( D3DXVECTOR2( vPos.x, vPos.y ) );
		}
	}
}



void CX2SlideShot::SetTextSpread()
{
	if( NULL == m_pDLGSlideShot )
		return;

	CKTDGUIStatic* pStatic_Speech	= (CKTDGUIStatic*) m_pDLGSlideShot->GetControl( L"Speech" );
	if( pStatic_Speech != NULL )
	{
		pStatic_Speech->GetString(0)->bSpread = false;
	}
}

bool CX2SlideShot::IsTextSpreading()
{
	if( NULL == m_pDLGSlideShot )
		return false;

	CKTDGUIStatic* pStatic_Speech	= (CKTDGUIStatic*) m_pDLGSlideShot->GetControl( L"Speech" );
	if( pStatic_Speech != NULL )
	{
		return pStatic_Speech->GetString(0)->bSpread;
	}

	return false;
}


void CX2SlideShot::ClearText()
{
	if( NULL == m_pDLGSlideShot )
		return;

	CKTDGUIStatic* pStatic_Name		= (CKTDGUIStatic*) m_pDLGSlideShot->GetControl( L"Name" );
	CKTDGUIStatic* pStatic_Speech	= (CKTDGUIStatic*) m_pDLGSlideShot->GetControl( L"Speech" );

	if( pStatic_Name != NULL )
		pStatic_Name->GetString(0)->msg		= L"";
	if( pStatic_Speech != NULL )
		pStatic_Speech->GetString(0)->msg	= L"";
}

int CX2SlideShot::LineBreak( int iWidth, wstring& wstrSpeech, const WCHAR* pNextLineString, wstring wstrColor /*= L""*/ )
{
	if( NULL == m_pDLGSlideShot )
		return -1;
	

	const wstring wstrNextLine = pNextLineString;

	int addRow = 0;
	int iUniCharSize = 0;		// pixel 단위??
	int iStringSize = 0;		// pixel 단위??

	//정해진 크기(m_Size)에 따라 문장을 여러 행으로 나눈다
	for( UINT i=0; i<wstrSpeech.length(); i++ )
	{
		WCHAR wChar = wstrSpeech[i];

		CKTDGUIStatic* pStatic_Speech	= (CKTDGUIStatic*) m_pDLGSlideShot->GetControl( L"Speech" );
		if( NULL == pStatic_Speech )
			return -1;

		CKTDGFontManager::CUKFont* pFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( pStatic_Speech->GetString(0)->fontIndex );
		if( NULL == pFont )
			return -1; 

		iUniCharSize = pFont->GetWidth( wChar );

		iStringSize += iUniCharSize;

		if( iStringSize >= iWidth  )
		{
			wstrSpeech.insert( wstrSpeech.begin()+i, wstrNextLine.begin(), wstrNextLine.end() );
			i++;
			addRow++;
			iStringSize = 0;

			wstrSpeech.insert( wstrSpeech.begin()+i, wstrColor.begin(), wstrColor.end() );
			i += wstrColor.length();
		}
	}


	if( 0 != wstrColor.length() )
	{
		wstrSpeech.insert( wstrSpeech.begin(), wstrColor.begin(), wstrColor.end() );
	}

	return addRow;
}


void CX2SlideShot::BlackLayOutMove( bool bInning, float fTime )
{
	if ( bInning == true )
	{
		CKTDGUIControl* pBlackLayOutUp = m_pDLGSlideShot->GetControl( L"BlackLayOutUp" );
		if ( pBlackLayOutUp != NULL )
			pBlackLayOutUp->Move( D3DXVECTOR2(0,0), D3DXCOLOR(1,1,1,1), fTime );

		CKTDGUIControl* pBlackLayOutDown = m_pDLGSlideShot->GetControl( L"BlackLayOutDown" );
		if ( pBlackLayOutDown != NULL )
			pBlackLayOutDown->Move( D3DXVECTOR2(0,0), D3DXCOLOR(1,1,1,1), fTime );


	}
	else
	{
		CKTDGUIControl* pBlackLayOutUp = m_pDLGSlideShot->GetControl( L"BlackLayOutUp" );
		if ( pBlackLayOutUp != NULL )
			pBlackLayOutUp->Move( D3DXVECTOR2(0,-90), D3DXCOLOR(1,1,1,1), fTime );

		CKTDGUIControl* pBlackLayOutDown = m_pDLGSlideShot->GetControl( L"BlackLayOutDown" );
		if ( pBlackLayOutDown != NULL )
			pBlackLayOutDown->Move( D3DXVECTOR2(0, 90), D3DXCOLOR(1,1,1,1), fTime );
	}
	
}

void CX2SlideShot::CrashSeq_LUA( char* ID, float fTime, float fGap )
{
	map<string, CKTDGParticleSystem::CParticleEventSequenceHandle>::iterator iter;
	iter = m_SeqMap.find( ID );
	if( iter != m_SeqMap.end() )
	{
		CKTDGParticleSystem::CParticleEventSequenceHandle hSeq = iter->second;
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( hSeq );
		if( NULL != pSeq )
		{
			CKTDGParticleSystem::CParticleEvent_Crash* pCrash = new CKTDGParticleSystem::CParticleEvent_Crash();
			pCrash->SetFade( true );
			pCrash->SetCrash( CMinMax<D3DXVECTOR3>(D3DXVECTOR3(-fGap,-fGap,0),D3DXVECTOR3(fGap,fGap,0)) );
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            const CKTDGParticleSystem::CParticle* pCParticle = pSeq->GetFrontParticle();
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			const CKTDGParticleSystem::CParticle* pCParticle = ( pSeq->m_ParticleList.empty() == false ) ? pSeq->m_ParticleList.front() : NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			if ( pCParticle != NULL )
				pCrash->SetActualTime( CMinMax<float>(pCParticle->GetEventTimer(), pCParticle->GetEventTimer() + fTime) );
			pSeq->m_EventList.push_back( pCrash );
		}
	}
}

void CX2SlideShot::ChangeColor_LUA(char* ID, float fTime, D3DXCOLOR d3dColor)
{
	map<string, CKTDGParticleSystem::CParticleEventSequenceHandle>::iterator iter;
	iter = m_SeqMap.find( ID );
	if( iter != m_SeqMap.end() )
	{
		CKTDGParticleSystem::CParticleEventSequenceHandle hSeq = iter->second;
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( hSeq );
		if( NULL != pSeq )
		{
			CKTDGParticleSystem::CParticleEvent_Color* pColor = new CKTDGParticleSystem::CParticleEvent_Color();
			pColor->SetFade( true );
 			pColor->SetColor( CMinMax<D3DXCOLOR>(d3dColor, d3dColor) );

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            CKTDGParticleSystem::CParticle* pCParticle = pSeq->GetFrontParticle();
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			CKTDGParticleSystem::CParticle* pCParticle = ( pSeq->m_ParticleList.empty() == false ) ? pSeq->m_ParticleList.front() : NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			if ( pCParticle != NULL )
			{
				pColor->SetActualTime( CMinMax<float>(pCParticle->GetEventTimer(), pCParticle->GetEventTimer() + fTime) );
				pCParticle->SetColorFinal( d3dColor );	// ㅡㅡ;; 초기설정이 안되서 직접넣어줘본다. 아놔
			}
			//if( pSeq->m_Events.size() > 0 )
			//{
			//	for( int i = 0; i < static_cast<int>(pSeq->m_Events.size()); ++i )
			//	{
			//		if( pSeq->m_Events[i] != NULL )
			//			delete pSeq->m_Events[i];
			//	}
			//	pSeq->m_Events.clear();
			//}
			pSeq->m_EventList.push_back( pColor );
		}
	}
}

void CX2SlideShot::ChangeTex_LUA( char* ID, char* pTexName )
{
	map<string,CKTDGParticleSystem::CParticleEventSequenceHandle>::iterator iter;
	iter = m_SeqMap.find( ID );
	if( iter != m_SeqMap.end() )
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( iter->second );
		if( NULL != pSeq )
		{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            const CKTDGParticleSystem::CParticle* pCParticle = pSeq->GetFrontParticle();
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			const CKTDGParticleSystem::CParticle* pCParticle = ( pSeq->m_ParticleList.empty() == false ) ? pSeq->m_ParticleList.front() : NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			if( pCParticle != NULL )
			{
				map<int,CKTDXDeviceTexture*>::iterator iter;
				iter = pSeq->m_TextureMap.find( pCParticle->GetTextureID() );
				CKTDXDeviceTexture* pTex = iter->second;
				SAFE_CLOSE( pTex );
				wstring texName;
				ConvertUtf8ToWCHAR( texName, pTexName );
				pTex = g_pKTDXApp->GetDeviceManager()->OpenTexture( texName.c_str() );
				iter->second = pTex;
			}
		}
	}
}

void CX2SlideShot::ChangeSize_LUA( char* ID, float fX, float fY )
{
	map<string,CKTDGParticleSystem::CParticleEventSequenceHandle>::iterator iter;
	iter = m_SeqMap.find( ID );
	if( iter != m_SeqMap.end() )
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( iter->second );
		if( NULL != pSeq )
		{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            CKTDGParticleSystem::CParticle* pCParticle = pSeq->GetFrontParticle();
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			CKTDGParticleSystem::CParticle* pCParticle = ( pSeq->m_ParticleList.empty() == false ) ? pSeq->m_ParticleList.front() : NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			if ( pCParticle != NULL )
			{
				pCParticle->SetSize( D3DXVECTOR3(fX, fY, pCParticle->GetSize().z) );
			}//if
		}
	}
}

void CX2SlideShot::DeleteSeq_LUA( char* ID )
{
	map<string,CKTDGParticleSystem::CParticleEventSequenceHandle>::iterator iter;
	iter = m_SeqMap.find( ID );
	if( iter != m_SeqMap.end() )
	{
		g_pData->GetUIMajorParticle()->DestroyInstanceHandle( iter->second );
		m_SeqMap.erase( iter );
	}
}


void CX2SlideShot::SetNameTabShow( bool bShow )
{
	if( NULL == m_pDLGSlideShot )
		return;


	CKTDGUIStatic* pStatic_NameTab		= (CKTDGUIStatic*) m_pDLGSlideShot->GetControl( L"NameTab" );
	if( NULL != pStatic_NameTab )
	{
		pStatic_NameTab->SetShowEnable( bShow, bShow );
	}

	CKTDGUIStatic* pStatic_Name		= (CKTDGUIStatic*) m_pDLGSlideShot->GetControl( L"Name" );
	if( NULL != pStatic_Name )
	{
		pStatic_Name->SetShowEnable( bShow, bShow );
	}
}

void CX2SlideShot::SetSpeechBallonShow( bool bShow )
{
	if( NULL == m_pDLGSlideShot )
		return;

	// 2010.04.28 / 박교현 / 말풍선 안꺼지는 버그 수정
//	CKTDGUIStatic* pStatic_SpeechBallon = (CKTDGUIStatic*) m_pDLGSlideShot->GetControl( L"SpeechBallon" );
	CKTDGUIStatic* pStatic_SpeechBallon = (CKTDGUIStatic*) m_pDLGSlideShot->GetControl( L"Background" );
	
	if( NULL != pStatic_SpeechBallon )
	{
		pStatic_SpeechBallon->SetShowEnable( bShow, bShow );
	}

	CKTDGUIStatic* pStatic_Speech	= (CKTDGUIStatic*) m_pDLGSlideShot->GetControl( L"Speech" );
	if( NULL != pStatic_Speech )
	{
		pStatic_Speech->SetShowEnable( bShow, bShow );
	}


}

//{{ 2010.04.28 / 박교현 / 베스마 비던 개편
//	CKTDGUIStatic* pStatic_SpeechBallon = (CKTDGUIStatic*) m_pDLGSlideShot->GetControl( L"SpeechBallon" );
void CX2SlideShot::SetEnterShow( bool bShow )
{
	if( NULL == m_pDLGSlideShot )
		return;

	CKTDGUIStatic* pStatic_Enter = (CKTDGUIStatic*) m_pDLGSlideShot->GetControl( L"Static_Enter" );

	if( NULL != pStatic_Enter )
	{
		pStatic_Enter->SetShowEnable( bShow, bShow );
	}
}
//}}
void CX2SlideShot::PlaySound2D_LUA( char* pFileName )
{
	if( NULL != g_pKTDXApp && NULL != g_pKTDXApp->GetDeviceManager() )
	{
		wstring fileName;
		ConvertUtf8ToWCHAR( fileName, pFileName );
		g_pKTDXApp->GetDeviceManager()->PlaySound( fileName.c_str(), false, false );

		m_vecSoundList.push_back( fileName );
	}
}

void CX2SlideShot::StopAllSound_LUA()
{
	if( true == m_vecSoundList.empty() )
		return;

	if( NULL != g_pKTDXApp && NULL != g_pKTDXApp->GetDeviceManager() )
	{
		vector<wstring>::iterator it = m_vecSoundList.begin();
		for( ; it != m_vecSoundList.end(); ++it)
		{
			g_pKTDXApp->GetDeviceManager()->StopSound( (*it).c_str() );
		}

		m_vecSoundList.clear();
	}
}
void CX2SlideShot::SetSlideBGM_LUA( char* pFileName )
{
	if( NULL != g_pX2Game && NULL != g_pX2Game->GetWorld() )
	{
		if( true == m_preBGMName.empty() )
		{
			m_preBGMName = g_pX2Game->GetWorld()->GetBGMName();
		}
		g_pX2Game->GetWorld()->SetBGM_LUA(pFileName);
		g_pX2Game->GetWorld()->PlayBGM();
	}
}
void CX2SlideShot::ResetBGM()
{
	if( true == m_preBGMName.empty() )
		return;

	if( NULL != g_pX2Game && NULL != g_pX2Game->GetWorld() )
	{
		g_pX2Game->GetWorld()->SetBGMName(m_preBGMName);
		g_pX2Game->GetWorld()->PlayBGM();
		m_preBGMName.clear();
	}
}


#ifdef  X2OPTIMIZE_SLIDE_SHOT_NPC_SELF_CRASH_BUG_FIX
void        CX2SlideShot::SetNPC( CX2GUNPC* pNPC )
{
    m_coNPC = pNPC;
}

void        CX2SlideShot::ResetNPC()
{
    m_coNPC.Reset();
}

CX2GUNPC*   CX2SlideShot::GetNPC()
{
    return  m_coNPC.GetObservable();
}
#endif  X2OPTIMIZE_SLIDE_SHOT_NPC_SELF_CRASH_BUG_FIX