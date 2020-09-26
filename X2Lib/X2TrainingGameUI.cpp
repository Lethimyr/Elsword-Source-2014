#include "StdAfx.h"
#include ".\X2TrainingGameUI.h"




CX2TrainingGameUI::CX2TrainingGameUI( CX2State* pNowState )
{
	m_pNowState			= pNowState;
	m_NowScenNum		= 0;
	m_fScenTimeBefore	= 0.0f;
	m_fScenTime			= 0.0f;
	m_bSceneStarted		= false;
	m_fElapsedTimeAfterTextSpread	= 0.f;

	m_iNowKeySequenceIndex = 0;
	m_bEnableWaitingUserState = false;

	m_iNowObjectiveCount = 0;
	m_bWasWaitState			= true;
	m_bWasSimpleWaitState	= true;

	m_fElapsedTimeAfterCompletingSimpleUserState = 0.f;
	m_fRemainTimeEnterMarkTwinkle = 0.f;

	m_iLastIndexWaitingKeyPress = 0;

	m_bShowSuccessCountEffect = true;

	m_pDLGSlideShot		= NULL;
	m_pDLGSlideShot		= new CKTDGUIDialog( (CKTDXStage*) m_pNowState, L"DLG_Training_GameUI.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGSlideShot );

	CKTDGUIStatic* pStatic_Key	= (CKTDGUIStatic*) m_pDLGSlideShot->GetControl( L"Static_Key_Sequence" );
	if( NULL != pStatic_Key )
	{
		for( int i=0; i< pStatic_Key->GetPictureNum(); i++ )
		{
			KEY_SEQUENCE keySeq;
			m_vecKeySequence.push_back( keySeq );
		}
	}


	// 자유 훈련
	m_eFTNPCID		= CX2UnitManager::NUI_MUSHROOM_WOODEN;
	m_iFTNPCCount 	= 1;
	m_iFTNPCLevel = g_pData->GetSelectUnitLevel();
	m_bFTEnableNPCAttack = true;

	m_pDLGFreeTrainingMenu = NULL;

	if( true == g_pMain->GetIsPlayingFreeTraining() )
	{
		OpenFreeTrainingMainMenu( true );
		OpenSlideWindow( false );
	}
}


CX2TrainingGameUI::~CX2TrainingGameUI(void)
{
	SAFE_DELETE_DIALOG( m_pDLGSlideShot );
	SAFE_DELETE_DIALOG( m_pDLGFreeTrainingMenu );
}

void CX2TrainingGameUI::OnFrameMove( double fTime, float fElapsedTime )
{
	const float MAGIC_MISSION_TEXT_SPREAD_TIME = 0.04f;


	if( true == m_bSceneStarted )
	{
		if( NULL != g_pX2Game &&
			NULL != g_pX2Game->GetMyUnit() )
		{
			CX2GUUser* pMyUnit = g_pX2Game->GetMyUnit();
			if( m_bWasSimpleWaitState == false )
			{
				m_bWasSimpleWaitState = true;
				for( UINT i=0; i<m_vecWaitingSimpleUserState.size(); i++ )
				{
					WAITING_SIMPLE_USER_STATE& waitState = m_vecWaitingSimpleUserState[i];
					if( pMyUnit->GetNowStateID() == waitState.iStateID )
					{
						m_bWasSimpleWaitState = false;
					}
				}
			}

			// waiting simple user state
			for( UINT i=0; i<m_vecWaitingSimpleUserState.size(); i++ )
			{
				WAITING_SIMPLE_USER_STATE& waitState = m_vecWaitingSimpleUserState[i];
				if( waitState.iNowCount < waitState.iMaxCount )
				{
					if( m_bWasSimpleWaitState == true &&
						pMyUnit->GetNowStateID() == (const char) waitState.iStateID )
					{
						if( -1 != waitState.iIsRight )
						{
							if( (waitState.iIsRight == 0 && pMyUnit->GetSyncData().bIsRight == true) ||
								(waitState.iIsRight == 1 && pMyUnit->GetSyncData().bIsRight == false) )
							{
								continue;
							}
						}

						m_bWasSimpleWaitState = false;
						waitState.iNowCount += 1;
					}
				}
				else
				{
					m_vecWaitingSimpleUserState.erase( m_vecWaitingSimpleUserState.begin() + i );
					i--;
				}
			}

			if( m_vecWaitingSimpleUserState.size() == 0 )
			{
				if( m_fElapsedTimeAfterCompletingSimpleUserState < 300.f )
					m_fElapsedTimeAfterCompletingSimpleUserState += fElapsedTime;
			}
			else if( m_fElapsedTimeAfterCompletingSimpleUserState != 0.f )
			{
				m_fElapsedTimeAfterCompletingSimpleUserState = 0.f;
			}

		}

		// scene 

		if( NULL != g_pX2Game && g_pX2Game->GetGameType() == CX2Game::GT_DUNGEON )
		{
			m_fScenTimeBefore = m_fScenTime;
			m_fScenTime += fElapsedTime;
			stringstream funcName;
			funcName << m_ScenName.c_str() << m_NowScenNum;
			lua_tinker::call<void>( g_pKTDXApp->GetLuaBinder()->GetLuaState(),  funcName.str().c_str(), g_pKTDXApp, static_cast<CX2DungeonGame*>(g_pX2Game), g_pData, this );

			if( NULL != m_pDLGSlideShot )
			{
				CKTDGUIStatic* pStatic_Speech	= (CKTDGUIStatic*) m_pDLGSlideShot->GetControl( L"Talk" );
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
			}
		}
	}



	if( NULL != g_pX2Game && NULL != g_pX2Game->GetMyUnit() )
	{

		if( NULL != m_pDLGSlideShot )
		{
			CKTDGUIStatic* pStatic_Key	= (CKTDGUIStatic*) m_pDLGSlideShot->GetControl( L"Static_Key_Sequence" );
			if( NULL != pStatic_Key )
			{
				bool bResetPosition = true;
				for( UINT i=0; i<m_vecKeySequence.size(); i++ )
				{
					KEY_SEQUENCE& keySeq = m_vecKeySequence[i];
					if( keySeq.m_fRemainTime > 0.f )
					{
						keySeq.m_fRemainTime -= fElapsedTime;
					}
					else
					{
						if( pStatic_Key->GetPicture( i )->GetShow() == true )
						{
							if( pStatic_Key->GetPicture( i )->pPoint->color.a > 0.3f )
							{
								pStatic_Key->GetPicture( i )->pPoint->color.a -= 0.04f;
							}
							else
							{
								pStatic_Key->GetPicture( i )->SetShow( false );
							}
						}
					}

					if( pStatic_Key->GetPicture(i)->GetShow() == true )
					{
						bResetPosition = false;
					}
				}


				if( true == bResetPosition )
				{
					m_iNowKeySequenceIndex = 0;
				}
			}
		}

		
		// key sequence
		if( true == g_pX2Game->GetMyUnit()->GetInputData().oneZ ||
			true == g_pX2Game->GetMyUnit()->GetInputData().oneDoubleZ )
		{
			AddKeySequence( DIK_Z );
		}

		if( true == g_pX2Game->GetMyUnit()->GetInputData().oneX ||
			true == g_pX2Game->GetMyUnit()->GetInputData().oneDoubleX )
		{
			AddKeySequence( DIK_X );
		}

		if( true == g_pX2Game->GetMyUnit()->GetInputData().oneC ||
			true == g_pX2Game->GetMyUnit()->GetInputData().oneDoubleC )
		{
			AddKeySequence( DIK_C );
		}

		if( true == g_pX2Game->GetMyUnit()->GetInputData().oneLeft ||
			true == g_pX2Game->GetMyUnit()->GetInputData().oneDoubleLeft )
		{
			AddKeySequence( DIK_LEFT );
		}


		if( true == g_pX2Game->GetMyUnit()->GetInputData().oneRight ||
			true == g_pX2Game->GetMyUnit()->GetInputData().oneDoubleRight )
		{
			AddKeySequence( DIK_RIGHT );
		}

		if( true == g_pX2Game->GetMyUnit()->GetInputData().oneUp ||
			true == g_pX2Game->GetMyUnit()->GetInputData().oneDoubleUp )
		{
			AddKeySequence( DIK_UP );
		}

		if( true == g_pX2Game->GetMyUnit()->GetInputData().oneDown ||
			true == g_pX2Game->GetMyUnit()->GetInputData().oneDoubleDown )
		{
			AddKeySequence( DIK_DOWN );
		}


		// waiting user unit state
		{
			CX2GUUser* pMyUnit = g_pX2Game->GetMyUnit();
			if( m_bWasWaitState == false )
			{
				m_bWasWaitState = true;
				for( UINT i=0; i<m_vecWaitingUserState.size(); i++ )
				{
					WAITING_USER_STATE& waitState = m_vecWaitingUserState[i];
					if( pMyUnit->GetNowStateID() == waitState.iStateID )
					{
						m_bWasWaitState = false;
					}
				}
			}



			for( UINT i=0; i<m_vecWaitingUserState.size(); i++ )
			{
				WAITING_USER_STATE& waitState = m_vecWaitingUserState[i];
				if( waitState.iNowCount < waitState.iMaxCount )
				{
					if( m_bWasWaitState == true &&
						pMyUnit->GetNowStateID() == (const char) waitState.iStateID )
					{
						m_bWasWaitState = false;
						waitState.iNowCount += 1;

						if( -1 != waitState.iObjectiveIndex && NULL != m_pDLGSlideShot )
						{
							SetTrainingObjectiveSuccessCount( waitState.iObjectiveIndex, waitState.iNowCount );
						}
					}
				}
			}
		}


		// waiting special attack
		{
			CX2GUUser* pMyUnit = g_pX2Game->GetMyUnit();
	
			for( UINT i=0; i<m_vecWaitingSpecialAttack.size(); i++ )
			{
				WAITING_SPECIAL_ATTACK& waitSpecialAttack = m_vecWaitingSpecialAttack[i];

				if( waitSpecialAttack.iNowCount < waitSpecialAttack.iMaxCount )
				{
					if( m_bWasWaitState == true &&
						pMyUnit->GetNowSpecialAttack() > 0 )
					{
						if( waitSpecialAttack.bShouldHitTarget == false || pMyUnit->GetAttackSuccess( false ) == true )
						{
							m_bWasWaitState = false;
							pMyUnit->SetNowSpecialAttack( -2 );
							waitSpecialAttack.iNowCount += 1;

							if( -1 != waitSpecialAttack.iStringIndex && NULL != m_pDLGSlideShot )
							{
								SetTrainingObjectiveSuccessCount( waitSpecialAttack.iStringIndex, waitSpecialAttack.iNowCount );

							}
						}
					}
				}
			}
		}


		
		// waiting sequential user unit state
		{
			CX2GUUser* pMyUnit = g_pX2Game->GetMyUnit();
		
			for( UINT i=0; i<m_vecWaitingSequentialUserState.size(); i++ )
			{
				WAITING_SEQUENTIAL_USER_STATE& waitSeqState = m_vecWaitingSequentialUserState[i];
				if( waitSeqState.iNowCount < waitSeqState.iMaxCount )
				{
					if( waitSeqState.iStateIndex > 0 )
					{
						waitSeqState.fElapsedTimeAfterLastState += fElapsedTime;
						if( waitSeqState.fElapsedTimeAfterLastState > waitSeqState.fStateTimeGap )
						{
							waitSeqState.iStateIndex = 0;
							waitSeqState.fElapsedTimeAfterLastState = 0.f;
						}
					}
					
					if( pMyUnit->GetNowStateID() == (const char) waitSeqState.vecStateID[ waitSeqState.iStateIndex ] )
					{
						if( waitSeqState.iStateIndex == waitSeqState.vecStateID.size()-1 )
						{
							// 성공~
							waitSeqState.iStateIndex = 0;
							waitSeqState.fElapsedTimeAfterLastState = 0.f;
							waitSeqState.iNowCount += 1;


							if( -1 != waitSeqState.iStringIndex && NULL != m_pDLGSlideShot )
							{
								SetTrainingObjectiveSuccessCount( waitSeqState.iStringIndex, waitSeqState.iNowCount );

							}
						}
						else
						{
							// 다음 state가 성공하기를 기다리자~
							waitSeqState.iStateIndex += 1;
							waitSeqState.fElapsedTimeAfterLastState = 0.f;
						}
					}
				}
			}
		}



	}

}


void CX2TrainingGameUI::ScenStart_LUA( char* pScenName, int scenNum )
{
    wstring wstrScenName;
    ConvertUtf8ToWCHAR( wstrScenName, pScenName );
    ConvertWCHARToChar( m_ScenName, wstrScenName.c_str() );
	m_bSceneStarted = true;
	m_NowScenNum	= scenNum;
	m_fScenTimeBefore	= 0.0f;
	m_fScenTime			= 0.0f;	

	//ClearText();
}

void CX2TrainingGameUI::ScenWait()
{
	//엔터키 표시가 나와야 한다.
}

void CX2TrainingGameUI::ScenEnd()
{
	m_NowScenNum		= 0;
	m_fScenTimeBefore	= 0.0f;
	m_fScenTime			= 0.0f;

	m_bSceneStarted = false;
}


void CX2TrainingGameUI::GoNextScen()
{
	ScenStart_LUA( (char*)m_ScenName.c_str(), m_NowScenNum+1 );	
}



bool CX2TrainingGameUI::EventTimer( float fTime )
{
	if( fTime > m_fScenTimeBefore && fTime <= m_fScenTime )
	{
		return true;
	}
	return false;
}


void CX2TrainingGameUI::AddText_LUA( int str_id, float fFullTime )
{
	
	wstring text = GET_STRING( str_id );
	//ConvertUtf8ToWCHAR( text, pText );

	if( fFullTime <= 0.0f )
		AddText( text.c_str() );
	else
	{
		float fTime = fFullTime / (float)(text.size());
		AddText( text.c_str(), L"", true, fTime );
	}
}

void CX2TrainingGameUI::AddText( const WCHAR* wszMsg, wstring wstrColor /*= L""*/, bool bSpread /*= true*/, float fSpreadTime /*= 0.1f*/ )
{
	if( NULL == m_pDLGSlideShot )
		return;

	ClearText();

	D3DXVECTOR2 vTemp = g_pKTDXApp->ConvertByResolution( 750, 750 );
	const int CHAT_LINE_WIDTH = (int)vTemp.x;

	wstring nextLineString = L"\n";
	wstring chatContent = wszMsg;

	CKTDGUIStatic* pStatic_Speech	= (CKTDGUIStatic*) m_pDLGSlideShot->GetControl( L"Talk" );	// 해외팀 위치 변경
	
#ifdef CLIENT_GLOBAL_LINEBREAK
	CKTDGFontManager::CUKFont* pFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( pStatic_Speech->GetString(0)->fontIndex );
	if( NULL == pFont )
		return; 
		
	int addRow = CWordLineHandler::LineBreakInX2Main(chatContent, pFont, CHAT_LINE_WIDTH, wstrColor, true, false);
#else //#ifdef CLIENT_GLOBAL_LINEBREAK
	int addRow = LineBreak( CHAT_LINE_WIDTH, chatContent, nextLineString.c_str(), wstrColor );
#endif //CLIENT_GLOBAL_LINEBREAK
	if( -1 == addRow )
		return;


	
	if( NULL != pStatic_Speech )
	{
		pStatic_Speech->GetString(0)->msg	= chatContent;

		if( true == bSpread )
		{
			pStatic_Speech->GetString(0)->bSpread = bSpread;
			pStatic_Speech->GetString(0)->fNowSpreadTime = 0.f;
			pStatic_Speech->GetString(0)->fSpreadTimeGap = fSpreadTime;
			pStatic_Speech->GetString(0)->iSpreadCount = 0;
		}
	}
}

void CX2TrainingGameUI::ClearText()
{
	if( NULL == m_pDLGSlideShot )
		return;

	CKTDGUIStatic* pStatic_Speech	= (CKTDGUIStatic*) m_pDLGSlideShot->GetControl( L"Talk" );
	if( NULL != pStatic_Speech )
	{
		pStatic_Speech->GetString(0)->msg	= L"";

	}
}

int CX2TrainingGameUI::LineBreak( int iWidth, wstring& wstrSpeech, const WCHAR* pNextLineString, wstring wstrColor /*= L""*/ )
{
	if( NULL == m_pDLGSlideShot )
		return -1;


	const wstring wstrNextLine = pNextLineString;

	int addRow = 0;
	int iUniCharSize = 0;		// pixel 단위??
	int iStringSize = 0;		// pixel 단위??


	CKTDGUIStatic* pStatic_Speech	= (CKTDGUIStatic*) m_pDLGSlideShot->GetControl( L"Talk" );
	if( NULL == pStatic_Speech )
		return -1;

	CKTDGFontManager::CUKFont* pFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( pStatic_Speech->GetString(0)->fontIndex );
	if( NULL == pFont )
		return -1; 

	//정해진 크기(m_Size)에 따라 문장을 여러 행으로 나눈다
	for( UINT i=0; i<wstrSpeech.length(); i++ )
	{
		WCHAR wChar = wstrSpeech[i];
		iUniCharSize = pFont->GetWidth( wChar );
		iStringSize += iUniCharSize;

		if( wChar == L'\n' )
		{
			iStringSize = 0;
		}

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


void CX2TrainingGameUI::AddWaitingKeyPress( int key, bool bUseTextureWidth/* = false */ )
{
	if( NULL == m_pDLGSlideShot )
		return;

	CKTDGUIStatic* pStatic_Key	= (CKTDGUIStatic*) m_pDLGSlideShot->GetControl( L"Static_Key_Input" );
	if( NULL == pStatic_Key )
		return;


	const float MAGIC_KEY_SIZE = 52.f;


	if( NULL != pStatic_Key->GetPicture( m_iLastIndexWaitingKeyPress ) )
	{
		SetPictureTex( pStatic_Key->GetPicture( m_iLastIndexWaitingKeyPress ), (COMMAND_PICTURE) key );
		pStatic_Key->GetPicture( m_iLastIndexWaitingKeyPress )->SetShow( true );
		pStatic_Key->GetPicture( m_iLastIndexWaitingKeyPress )->SetTwinkle( 1.4f );

		if( true == bUseTextureWidth && NULL != pStatic_Key->GetPicture( m_iLastIndexWaitingKeyPress )->pTexture ) 
		{
			float fWidth = pStatic_Key->GetPicture( m_iLastIndexWaitingKeyPress )->pTexture->texSize.x;
			pStatic_Key->GetPicture( m_iLastIndexWaitingKeyPress )->SetSizeX( fWidth );
		}
		else
		{
			pStatic_Key->GetPicture( m_iLastIndexWaitingKeyPress )->SetSizeX( MAGIC_KEY_SIZE );
		}

		m_iLastIndexWaitingKeyPress += 1;
	}


	int nCount = m_iLastIndexWaitingKeyPress;
	D3DXVECTOR2 vStartPos = D3DXVECTOR2( 570, 655 );

	for( int i=0; i<nCount; i++ )
	{
		vStartPos.x -= (pStatic_Key->GetPicture(i)->GetSize().x + 4.f) * 0.5f; 
	}

	for( int i=0; i<nCount; i++ )
	{
		pStatic_Key->GetPicture(i)->SetPos( vStartPos );
		vStartPos.x += pStatic_Key->GetPicture(i)->GetSize().x + 4.f;
	}
}



void CX2TrainingGameUI::AddWaitingKeyPressScaled( int key, bool bUseTextureSize/* = false */, D3DXVECTOR2 vScale /*= D3DXVECTOR2(1, 1)*/ )
{
	if( NULL == m_pDLGSlideShot )
		return;

	CKTDGUIStatic* pStatic_Key	= (CKTDGUIStatic*) m_pDLGSlideShot->GetControl( L"Static_Key_Input" );
	if( NULL == pStatic_Key )
		return;


	const float MAGIC_KEY_SIZE = 52.f;


	if( NULL != pStatic_Key->GetPicture( m_iLastIndexWaitingKeyPress ) )
	{
		SetPictureTex( pStatic_Key->GetPicture( m_iLastIndexWaitingKeyPress ), (COMMAND_PICTURE) key );
		pStatic_Key->GetPicture( m_iLastIndexWaitingKeyPress )->SetShow( true );
		pStatic_Key->GetPicture( m_iLastIndexWaitingKeyPress )->SetTwinkle( 1.4f );

		if( true == bUseTextureSize && NULL != pStatic_Key->GetPicture( m_iLastIndexWaitingKeyPress )->pTexture ) 
		{
			D3DXVECTOR2 vSize;
			vSize.x = pStatic_Key->GetPicture( m_iLastIndexWaitingKeyPress )->pTexture->texSize.x * vScale.x;
			vSize.y = pStatic_Key->GetPicture( m_iLastIndexWaitingKeyPress )->pTexture->texSize.y * vScale.y;
			pStatic_Key->GetPicture( m_iLastIndexWaitingKeyPress )->SetSize( vSize );
		}
		else
		{
			D3DXVECTOR2 vSize = vScale * MAGIC_KEY_SIZE;
			pStatic_Key->GetPicture( m_iLastIndexWaitingKeyPress )->SetSize( vSize );
		}

		m_iLastIndexWaitingKeyPress += 1;
	}

	
	int nCount = m_iLastIndexWaitingKeyPress;
	D3DXVECTOR2 vStartPos = D3DXVECTOR2( 570, 655 );

	for( int i=0; i<nCount; i++ )
	{
		vStartPos.x -= (pStatic_Key->GetPicture(i)->GetSize().x + 4.f) * 0.5f; 
	}

	for( int i=0; i<nCount; i++ )
	{
		pStatic_Key->GetPicture(i)->SetPos( vStartPos );
		vStartPos.x += pStatic_Key->GetPicture(i)->GetSize().x + 4.f;
	}
}



void CX2TrainingGameUI::ClearKeySequence()
{
	if( NULL == m_pDLGSlideShot )
		return;

	CKTDGUIStatic* pStatic_Key	= (CKTDGUIStatic*) m_pDLGSlideShot->GetControl( L"Static_Key_Sequence" );
	if( NULL != pStatic_Key )
	{
		for( int i=0; i<pStatic_Key->GetPictureNum(); i++ )
		{
			pStatic_Key->GetPicture(i)->SetShow( false );
		}
	}
}


void CX2TrainingGameUI::AddKeySequence( unsigned char key )
{
	if( NULL == m_pDLGSlideShot )
		return;

	CKTDGUIStatic* pStatic_Key	= (CKTDGUIStatic*) m_pDLGSlideShot->GetControl( L"Static_Key_Sequence" );
	if( NULL == pStatic_Key )
		return;

	if( m_iNowKeySequenceIndex > pStatic_Key->GetPictureNum()-1 )
	{
		ClearKeySequence();
		m_iNowKeySequenceIndex = 0;
	}


	m_vecKeySequence[ m_iNowKeySequenceIndex ].m_fRemainTime = 3.f;
	if( NULL != pStatic_Key->GetPicture( m_iNowKeySequenceIndex ) && 
		NULL != pStatic_Key->GetPicture( m_iNowKeySequenceIndex )->pPoint )
	{
		pStatic_Key->GetPicture( m_iNowKeySequenceIndex )->pPoint->color.a = 1.f; 
	}
	
	

	switch( key )
	{
	case DIK_LEFT:
		{
			
			pStatic_Key->GetPicture( m_iNowKeySequenceIndex )->SetShow( true );
			pStatic_Key->GetPicture( m_iNowKeySequenceIndex )->SetTex_LUA( "DLG_KEYS.TGA", "KEY_LEFT" );
		} break;

	case DIK_RIGHT:
		{
			pStatic_Key->GetPicture( m_iNowKeySequenceIndex )->SetShow( true );
			pStatic_Key->GetPicture( m_iNowKeySequenceIndex )->SetTex_LUA( "DLG_KEYS.TGA", "KEY_RIGHT" );
		} break;


	case DIK_UP:
		{
			pStatic_Key->GetPicture( m_iNowKeySequenceIndex )->SetShow( true );
			pStatic_Key->GetPicture( m_iNowKeySequenceIndex )->SetTex_LUA( "DLG_KEYS.TGA", "KEY_UP" );
		} break;


	case DIK_DOWN:
		{
			pStatic_Key->GetPicture( m_iNowKeySequenceIndex )->SetShow( true );
			pStatic_Key->GetPicture( m_iNowKeySequenceIndex )->SetTex_LUA( "DLG_KEYS.TGA", "KEY_DOWN" );
		} break;


	case DIK_Z:
		{
			pStatic_Key->GetPicture( m_iNowKeySequenceIndex )->SetShow( true );
			pStatic_Key->GetPicture( m_iNowKeySequenceIndex )->SetTex_LUA( "DLG_KEYS.TGA", "KEY_Z" );
		} break;


	case DIK_X:
		{
			pStatic_Key->GetPicture( m_iNowKeySequenceIndex )->SetShow( true );
			pStatic_Key->GetPicture( m_iNowKeySequenceIndex )->SetTex_LUA( "DLG_KEYS.TGA", "KEY_X" );
		} break;


	case DIK_C:
		{
			pStatic_Key->GetPicture( m_iNowKeySequenceIndex )->SetShow( true );
			pStatic_Key->GetPicture( m_iNowKeySequenceIndex )->SetTex_LUA( "DLG_KEYS.TGA", "KEY_C" );
		} break;

	case DIK_S:
		{
			pStatic_Key->GetPicture( m_iNowKeySequenceIndex )->SetShow( true );
			pStatic_Key->GetPicture( m_iNowKeySequenceIndex )->SetTex_LUA( "DLG_KEYS.TGA", "KEY_S" );
		} break;
	}


	m_iNowKeySequenceIndex += 1;

}

bool CX2TrainingGameUI::IsTextSpreading()
{
	if( NULL == m_pDLGSlideShot )
		return false;

	CKTDGUIStatic* pStatic_Speech	= (CKTDGUIStatic*) m_pDLGSlideShot->GetControl( L"Talk" );
	if( pStatic_Speech != NULL )
	{
		return pStatic_Speech->GetString(0)->bSpread;
	}

	return false;
}

void CX2TrainingGameUI::SetTextSpread()
{
	if( NULL == m_pDLGSlideShot )
		return;

	CKTDGUIStatic* pStatic_Speech	= (CKTDGUIStatic*) m_pDLGSlideShot->GetControl( L"Talk" );
	if( pStatic_Speech != NULL )
	{
		pStatic_Speech->GetString(0)->bSpread = false;
	}
}



void CX2TrainingGameUI::ClearWaitingKeyPress()
{
	m_iLastIndexWaitingKeyPress = 0;

	if( NULL == m_pDLGSlideShot )
		return;

	CKTDGUIStatic* pStatic_Key	= (CKTDGUIStatic*) m_pDLGSlideShot->GetControl( L"Static_Key_Input" );
	if( NULL != pStatic_Key )
	{
		for( int i=0; i<pStatic_Key->GetPictureNum(); i++ )
		{
			pStatic_Key->GetPicture(i)->SetShow( false );
		}
	}
}

bool CX2TrainingGameUI::IsKeyPressed( int key ) 
{
	if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( (unsigned char) key ) == TRUE )
		return true;
	else
		return false;
}

void CX2TrainingGameUI::AddWaitingUserState_LUA( int iStateID, int iMaxCount, int iStringIndex, const char* szTitle )
{
	wstring wstrTitle = L"";
	ConvertUtf8ToWCHAR( wstrTitle, szTitle );

	AddWaitingUserState( iStateID, iMaxCount, iStringIndex, wstrTitle.c_str() );
}

void CX2TrainingGameUI::AddWaitingUserState( int iStateID, int iMaxCount, int iStringIndex, const WCHAR* wszTitle )
{
	WAITING_USER_STATE waitState;
	waitState.iStateID = iStateID;
	waitState.iMaxCount = iMaxCount;
	waitState.iObjectiveIndex = iStringIndex;
	waitState.wstrObjective = wszTitle;
	m_vecWaitingUserState.push_back( waitState );
}

bool CX2TrainingGameUI::IsWaitingUserState()
{
	if( false == m_bEnableWaitingUserState )
		return false;

	if( m_vecWaitingUserState.size() == 0 )
		return false;

	for( UINT i=0; i<m_vecWaitingUserState.size(); i++ )
	{
		WAITING_USER_STATE& waitState = m_vecWaitingUserState[i];
		if( waitState.iNowCount < waitState.iMaxCount )
		{
			return true;
		}
	}

	return false;
}

void CX2TrainingGameUI::AddSimpleWaitingUserState( int iStateID, int iMaxCount, int iIsRight )
{
	WAITING_SIMPLE_USER_STATE waitState;
	waitState.iStateID = iStateID;
	waitState.iMaxCount = iMaxCount;
	waitState.iIsRight = iIsRight;
	m_vecWaitingSimpleUserState.push_back( waitState );
}

bool CX2TrainingGameUI::IsWaitingSimpleUserState()
{
	if( m_vecWaitingSimpleUserState.size() > 0 )
		return true;
	else
		return false;
}

void CX2TrainingGameUI::AddGatherItem_LUA( int iItemID, int iMaxCount, int iStringIndex, const char* szTitle )
{
	wstring wstrObjective = L"";
	ConvertUtf8ToWCHAR( wstrObjective, szTitle );
	
	AddGatherItem( iItemID, iMaxCount, iStringIndex, wstrObjective.c_str() );
}


void CX2TrainingGameUI::AddGatherItem( int iItemID, int iMaxCount, int iStringIndex, const WCHAR* wszTitle )
{
	GATHER_ITEM gatherItem;
	gatherItem.iItemID = iItemID; 
	gatherItem.iMaxCount = iMaxCount;
	gatherItem.iObjectiveIndex = iStringIndex;
	gatherItem.wstrObjective = wszTitle;
	m_vecGatherItem.push_back( gatherItem );
}

bool CX2TrainingGameUI::IsWaitingGatherItem()
{
	if( m_vecGatherItem.size() == 0 )
		return false;

	for( UINT i=0; i<m_vecGatherItem.size(); i++ )
	{
		GATHER_ITEM& gatherItem = m_vecGatherItem[i];
		if( gatherItem.iNowCount < gatherItem.iMaxCount )
		{
			return true;
		}
	}

	return false;
}

void CX2TrainingGameUI::UpdateGatherItem( int iItemID, int iCount )
{
	for( UINT i=0; i<m_vecGatherItem.size(); i++ )
	{
		GATHER_ITEM& gatherItem = m_vecGatherItem[i];
		if( gatherItem.iItemID == iItemID )
		{
			gatherItem.iNowCount += iCount;

			if( -1 != gatherItem.iObjectiveIndex && NULL != m_pDLGSlideShot )
			{
				SetTrainingObjectiveSuccessCount( gatherItem.iObjectiveIndex, gatherItem.iNowCount );
							
			}
		}
	}
}

void CX2TrainingGameUI::AddWaitingSpecialAttack_LUA( int iMaxCount, int iStringIndex, const char* szTitle, bool bShouldHitTarget )
{
	wstring wstrObjective = L"";
	ConvertUtf8ToWCHAR( wstrObjective, szTitle );

	AddWaitingSpecialAttack( iMaxCount, iStringIndex, wstrObjective.c_str(), bShouldHitTarget );
}

void CX2TrainingGameUI::AddWaitingSpecialAttack( int iMaxCount, int iStringIndex, const WCHAR* wszTitle, bool bShouldHitTarget )
{
	WAITING_SPECIAL_ATTACK waitSpecialAttack;
	waitSpecialAttack.iMaxCount				= iMaxCount;
	waitSpecialAttack.iStringIndex			= iStringIndex;
	waitSpecialAttack.wstrObjective			= wszTitle;
	waitSpecialAttack.bShouldHitTarget		= bShouldHitTarget;
	m_vecWaitingSpecialAttack.push_back( waitSpecialAttack );
}

bool CX2TrainingGameUI::IsWaitingSpecialAttack()
{
	if( m_vecWaitingSpecialAttack.size() == 0 )
		return false;

	for( UINT i=0; i<m_vecWaitingSpecialAttack.size(); i++ )
	{
		WAITING_SPECIAL_ATTACK& waitSpecialAttack = m_vecWaitingSpecialAttack[i];
		if( waitSpecialAttack.iNowCount < waitSpecialAttack.iMaxCount )
		{
			return true;
		}
	}

	return false;
}

int CX2TrainingGameUI::AddWaitingSeqUserState_LUA( float fTimeGap, int iMaxCount, int iStringIndex, const char* szTitle )
{
	wstring wstrObjective = L"";
	ConvertUtf8ToWCHAR( wstrObjective, szTitle );

	return AddWaitingSeqUserState( fTimeGap, iMaxCount, iStringIndex, wstrObjective.c_str() );
}

int CX2TrainingGameUI::AddWaitingSeqUserState( float fTimeGap, int iMaxCount, int iStringIndex, const WCHAR* wszTitle )
{
	WAITING_SEQUENTIAL_USER_STATE waitSeqUserState;
	waitSeqUserState.fStateTimeGap			= fTimeGap;
	waitSeqUserState.iMaxCount				= iMaxCount;
	waitSeqUserState.iStringIndex			= iStringIndex;
	waitSeqUserState.wstrObjective			= wszTitle;
	m_vecWaitingSequentialUserState.push_back( waitSeqUserState );

	return (int) (m_vecWaitingSequentialUserState.size()-1);
}

void CX2TrainingGameUI::SetWaitingSeqUserState( int iSeqUserStateIndex, int iStateID0, int iStateID1, int iStateID2 )
{
	if( iSeqUserStateIndex < 0 || iSeqUserStateIndex > (int) m_vecWaitingSequentialUserState.size()-1 )
		return;
	
	WAITING_SEQUENTIAL_USER_STATE& waitSeqUserState = m_vecWaitingSequentialUserState[iSeqUserStateIndex];

	if( iStateID0 != -1 )
	{
		waitSeqUserState.vecStateID.push_back( iStateID0 );
	}

	if( iStateID1 != -1 )
	{
		waitSeqUserState.vecStateID.push_back( iStateID1 );
	}

	if( iStateID2 != -1 )
	{
		waitSeqUserState.vecStateID.push_back( iStateID2 );
	}
}

bool CX2TrainingGameUI::IsWaitingSeqUserState()
{
	if( m_vecWaitingSequentialUserState.size() == 0 )
		return false;

	for( UINT i=0; i<m_vecWaitingSequentialUserState.size(); i++ )
	{
		WAITING_SEQUENTIAL_USER_STATE& waitSeqUserState = m_vecWaitingSequentialUserState[i];
		if( waitSeqUserState.iNowCount < waitSeqUserState.iMaxCount )
		{
			return true;
		}
	}

	return false;
}



void CX2TrainingGameUI::ClearTrainingObjective()
{
	if( NULL != m_pDLGSlideShot )
	{
		for( int i=0; i<(int)m_vecTrainingObjective.size(); i++ )
		{
			WCHAR wszText[64] = L"";
			StringCchPrintfW( wszText, ARRAY_SIZE(wszText), L"Static_Objective%d", i );
			CKTDGUIStatic* pStatic_Objective = (CKTDGUIStatic*) m_pDLGSlideShot->GetControl( wszText );
			if( NULL == pStatic_Objective )
				break;

			m_pDLGSlideShot->DeleteControl( wszText );
		}
	}

	m_vecTrainingObjective.clear();
}

int CX2TrainingGameUI::AddTrainingObjective( D3DXVECTOR2 vPos, D3DXVECTOR2 vPictureSize, int iIndexOfCount )
{
	TRAINING_OBJECTIVE trainingObjective;
	trainingObjective.m_iPictureIndexOfNowSuccessCount = iIndexOfCount;
	trainingObjective.m_vPictureSize = vPictureSize;
	trainingObjective.m_vFirstPicturePos = vPos;
	trainingObjective.m_vNowPicturePos = vPos;
	m_vecTrainingObjective.push_back( trainingObjective );


	int iObjectiveIndex = (int) m_vecTrainingObjective.size()-1;
	if( NULL != m_pDLGSlideShot )
	{
		CKTDGUIStatic* pStatic = m_pDLGSlideShot->CreateStatic();
		WCHAR wszText[64] = {0,};
		//wsprintf( wszText, L"Static_Objective%d", iObjectiveIndex );
		StringCchPrintf(wszText, 64, L"Static_Objective%d", iObjectiveIndex );
		m_pDLGSlideShot->AddControl( (CKTDGUIControl*) pStatic );
		pStatic->SetName( wszText );
	}

	return iObjectiveIndex;
}


bool CX2TrainingGameUI::AddTrainingObjectivePictureScaled( int iObjectiveIndex, int iKeyPicture, D3DXVECTOR2 vScale /*= D3DXVECTOR2(1,1)*/, bool bUseTextureWidth /*= true */ )
{
	if( NULL == m_pDLGSlideShot )
		return false;

	if( iObjectiveIndex > (int)m_vecTrainingObjective.size()-1 )
		return false;

	WCHAR wszText[64] = {0,};
	//wsprintf( wszText, L"Static_Objective%d", iObjectiveIndex );
	StringCchPrintf(wszText, 64, L"Static_Objective%d", iObjectiveIndex );
	CKTDGUIStatic* pStatic_Objective = (CKTDGUIStatic*) m_pDLGSlideShot->GetControl( wszText );
	if( NULL == pStatic_Objective )
		return false;


	TRAINING_OBJECTIVE& trainingObjective = m_vecTrainingObjective[iObjectiveIndex];

	CKTDGUIStatic::CPictureData* pPicture =  new CKTDGUIControl::CPictureData();
	pStatic_Objective->AddPicture( pPicture );
	SetPictureTex( pPicture, (COMMAND_PICTURE) iKeyPicture );


	CKTDGUIControl::UIPointData* pPoint = new CKTDGUIControl::UIPointData();
	pPoint->fChangeTime			= 0.0f;
	pPoint->bUseTextureSize		= bUseTextureWidth;
	pPoint->leftTopPoint		= trainingObjective.m_vNowPicturePos;
	pPoint->rightTopPoint		= D3DXVECTOR2( trainingObjective.m_vNowPicturePos.x+trainingObjective.m_vPictureSize.x, trainingObjective.m_vNowPicturePos.y );
	pPoint->leftBottomPoint		= D3DXVECTOR2( trainingObjective.m_vNowPicturePos.x, trainingObjective.m_vNowPicturePos.y + trainingObjective.m_vPictureSize.y );
	pPoint->rightBottomPoint	= D3DXVECTOR2( trainingObjective.m_vNowPicturePos.x+trainingObjective.m_vPictureSize.x, trainingObjective.m_vNowPicturePos.y + trainingObjective.m_vPictureSize.y );
	pPoint->color				= D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );	
	pPicture->SetPoint( pPoint );
	
	
	
	if( true == bUseTextureWidth && NULL != pPicture->pTexture )
	{
		D3DXVECTOR2 vSize;
		vSize.x = pPicture->pTexture->texSize.x * vScale.x;
		vSize.y = pPicture->pTexture->texSize.y * vScale.y;
		pPicture->SetSize( vSize );
		trainingObjective.m_vNowPicturePos.x += (vSize.x + 4.f);
	}
	else
	{
		D3DXVECTOR2 vSize;
		vSize.x = trainingObjective.m_vPictureSize.x * vScale.x;
		vSize.y = trainingObjective.m_vPictureSize.y * vScale.y;

		pPicture->SetSize( vSize );
		trainingObjective.m_vNowPicturePos.x += (vSize.x + 4.f);
	}

	return true;
}

bool CX2TrainingGameUI::AddTrainingObjectivePicture( int iObjectiveIndex, int iKeyPicture, bool bNextLine, bool bUseTextureWidth /*= false */ )
{
	if( NULL == m_pDLGSlideShot )
		return false;


	if( iObjectiveIndex > (int)m_vecTrainingObjective.size()-1 )
		return false;


	WCHAR wszText[64] = {0,};
	//wsprintf( wszText, L"Static_Objective%d", iObjectiveIndex );
	StringCchPrintf(wszText, 64, L"Static_Objective%d", iObjectiveIndex );
	CKTDGUIStatic* pStatic_Objective = (CKTDGUIStatic*) m_pDLGSlideShot->GetControl( wszText );
	if( NULL == pStatic_Objective )
		return false;


	TRAINING_OBJECTIVE& trainingObjective = m_vecTrainingObjective[iObjectiveIndex];

	CKTDGUIStatic::CPictureData* pPicture =  new CKTDGUIControl::CPictureData();
	pStatic_Objective->AddPicture( pPicture );
	SetPictureTex( pPicture, (COMMAND_PICTURE) iKeyPicture );


	if( true == bNextLine )
	{
		trainingObjective.m_vNowPicturePos.x = trainingObjective.m_vFirstPicturePos.x;
		trainingObjective.m_vNowPicturePos.y += (trainingObjective.m_vPictureSize.y + 2.f);
	}


	CKTDGUIControl::UIPointData* pPoint = new CKTDGUIControl::UIPointData();
	pPoint->fChangeTime			= 0.0f;
	pPoint->bUseTextureSize		= bUseTextureWidth;
	pPoint->leftTopPoint		= trainingObjective.m_vNowPicturePos;
	pPoint->rightTopPoint		= D3DXVECTOR2( trainingObjective.m_vNowPicturePos.x+trainingObjective.m_vPictureSize.x, trainingObjective.m_vNowPicturePos.y );
	pPoint->leftBottomPoint		= D3DXVECTOR2( trainingObjective.m_vNowPicturePos.x, trainingObjective.m_vNowPicturePos.y + trainingObjective.m_vPictureSize.y );
	pPoint->rightBottomPoint	= D3DXVECTOR2( trainingObjective.m_vNowPicturePos.x+trainingObjective.m_vPictureSize.x, trainingObjective.m_vNowPicturePos.y + trainingObjective.m_vPictureSize.y );
	pPoint->color				= D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );	
	pPicture->SetPoint( pPoint );



	if( true == bUseTextureWidth && NULL != pPicture->pTexture )
	{
		trainingObjective.m_vNowPicturePos.x += (pPicture->pTexture->texSize.x + 4.f);
	}
	else
	{
		trainingObjective.m_vNowPicturePos.x += (trainingObjective.m_vPictureSize.x + 4.f);
	}

	return true;
	

}


void CX2TrainingGameUI::SetTrainingObjectiveSuccessCount( int iObjectiveIndex, int nCount )
{
	if( NULL == m_pDLGSlideShot )
		return;

	if( iObjectiveIndex > (int)m_vecTrainingObjective.size()-1 )
		return;


	WCHAR wszText[64] = {0,};
	//wsprintf( wszText, L"Static_Objective%d", iObjectiveIndex );
	StringCchPrintf(wszText, 64, L"Static_Objective%d", iObjectiveIndex );
	CKTDGUIStatic* pStatic_Objective = (CKTDGUIStatic*) m_pDLGSlideShot->GetControl( wszText );
	if( NULL == pStatic_Objective )
		return;


	TRAINING_OBJECTIVE& trainingObjective = m_vecTrainingObjective[iObjectiveIndex];
	
	//wsprintf( wszText, L"HQ_RankPC_%d.dds", nCount%10 );
	StringCchPrintf(wszText, 64, L"HQ_RankPC_%d.dds", nCount%10 );
	if( NULL != pStatic_Objective->GetPicture( trainingObjective.m_iPictureIndexOfNowSuccessCount ) )
	{
		pStatic_Objective->GetPicture( trainingObjective.m_iPictureIndexOfNowSuccessCount )->SetTex( wszText );
	}

	if( nCount/10 > 0 )
	{
		//wsprintf( wszText, L"HQ_RankPC_%d.dds", nCount/10 );
		StringCchPrintf(wszText, 64, L"HQ_RankPC_%d.dds", nCount/10 );
		if( NULL != pStatic_Objective->GetPicture( trainingObjective.m_iPictureIndexOfNowSuccessCount-1 ) )
		{
			pStatic_Objective->GetPicture( trainingObjective.m_iPictureIndexOfNowSuccessCount-1 )->SetTex( wszText );
		}
	}

	if( m_bShowSuccessCountEffect == true )
	{
		if( NULL != pStatic_Objective->GetPicture( trainingObjective.m_iPictureIndexOfNowSuccessCount ) )
		{
			D3DXVECTOR2 vPos = pStatic_Objective->GetPicture( trainingObjective.m_iPictureIndexOfNowSuccessCount )->GetPos();
			vPos += pStatic_Objective->GetPicture( trainingObjective.m_iPictureIndexOfNowSuccessCount )->GetSize() * 0.5f;

			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMinorParticle()->CreateSequence( NULL,  L"HyperBallTick", 
				vPos.x, vPos.y, 0.0f, 1000,1000, -1, 10 );
			if( pSeq != NULL )
			{
				pSeq->SetOverUI( true );
				pSeq->SetBlackHolePosition( pSeq->GetPosition() );
				pSeq->UseLookPoint( true );
				pSeq->SetLookPoint( pSeq->GetPosition() );
			}
		}


		CX2GUUser* pGUUser = g_pX2Game->GetMyUnit();
		if( NULL != pGUUser )
		{
			D3DXVECTOR3 vPos = pGUUser->GetPos();
			vPos.y += 200.f;
			vPos.z -= 200.f;

			g_pX2Game->GetMinorParticle()->CreateSequence( NULL,  L"TrainingSuccess", vPos, 100, 100, 1, 1, -1, false, 0.5f );
		}
	}


}


void CX2TrainingGameUI::SetShowEnterMark( bool bShow )
{
	if( NULL == m_pDLGSlideShot )
		return;

	CKTDGUIStatic* pStatic_Enter = (CKTDGUIStatic*) m_pDLGSlideShot->GetControl( L"Static_Enter" );
	if( NULL == pStatic_Enter )
		return;

	pStatic_Enter->SetShow( bShow );
}


void CX2TrainingGameUI::SetTwinkleEnterMark( float fTime )
{
	if( NULL == m_pDLGSlideShot )
		return;

	CKTDGUIStatic* pStatic_Enter = (CKTDGUIStatic*) m_pDLGSlideShot->GetControl( L"Static_Enter" );
	if( NULL == pStatic_Enter )
		return;

	if( NULL != pStatic_Enter->GetPicture(1) )
	{
		pStatic_Enter->GetPicture(1)->SetTwinkle( fTime );
	}
}



void CX2TrainingGameUI::SetPictureTex( CKTDGUIStatic::CPictureData* pPicture, COMMAND_PICTURE eKey )
{
	if( NULL == pPicture )
		return;


	switch( eKey )
	{
	case CP_LEFT:
		pPicture->SetTex( L"DLG_KEYS.TGA", L"KEY_LEFT" );
		break;

	case CP_RIGHT:
		pPicture->SetTex( L"DLG_KEYS.TGA", L"KEY_RIGHT" );
		break;

	case CP_UP:
		pPicture->SetTex( L"DLG_KEYS.TGA", L"KEY_UP" );
		break;

	case CP_DOWN:
		pPicture->SetTex( L"DLG_KEYS.TGA", L"KEY_DOWN" );
		break;

	case CP_Z:
		pPicture->SetTex( L"DLG_KEYS.TGA", L"KEY_Z" );
		break;

	case CP_X:
		pPicture->SetTex( L"DLG_KEYS.TGA", L"KEY_X" );
		break;

	case CP_C:
		pPicture->SetTex( L"DLG_KEYS.TGA", L"KEY_C" );
		break;

	case CP_S:
		pPicture->SetTex( L"DLG_KEYS.TGA", L"KEY_S" );
		break;

		//case CP_A:
		//	pPicture->SetTex( L"DLG_KEYS.TGA", "KEY_RIGHT" );
		//	break;

	case CP_BLANK:
		pPicture->SetTex( L"NoAlphaImage.dds" );
		break;


	case CP_0:
		pPicture->SetTex( L"HQ_RankPC_0.dds" );
		break;

	case CP_1:
		pPicture->SetTex( L"HQ_RankPC_1.dds" );
		break;

	case CP_2:
		pPicture->SetTex( L"HQ_RankPC_2.dds" );
		break;

	case CP_3:
		pPicture->SetTex( L"HQ_RankPC_3.dds" );
		break;

	case CP_4:
		pPicture->SetTex( L"HQ_RankPC_4.dds" );
		break;

	case CP_5:
		pPicture->SetTex( L"HQ_RankPC_5.dds" );
		break;

	case CP_6:
		pPicture->SetTex( L"HQ_RankPC_6.dds" );
		break;

	case CP_7:
		pPicture->SetTex( L"HQ_RankPC_7.dds" );
		break;

	case CP_8:
		pPicture->SetTex( L"HQ_RankPC_8.dds" );
		break;

	case CP_9:
		pPicture->SetTex( L"HQ_RankPC_9.dds" );
		break;

	case CP_OPEN_PARENTHESIS:
		pPicture->SetTex( L"PC_(.dds" );
		break;

	case CP_CLOSE_PARENTHESIS:
		pPicture->SetTex( L"PC_).dds" );
		break;

	case CP_SLASH:	
		pPicture->SetTex( L"PC_I.dds" );
		break;

	case CP_ED:
		pPicture->SetTex( L"DLG_Common_Texture01.tga", L"ED_COIN" );
		break;

	case CP_MINUS:
		pPicture->SetTex( L"PC_-.dds" );
		break;

	case CP_OR:
		pPicture->SetTex( L"DLG_KEYS.TGA", L"Or" );
		break;

	case CP_PRESSED:
		pPicture->SetTex( L"DLG_KEYS.TGA", L"Down_Condition" );
		break;

	case CP_SPECIAL_ATTACK:
		pPicture->SetTex( L"DLG_Common_Texture08.TGA", L"Hard_Death_Skill" );
		break;

	case CP_DASH_RIGHT:
		pPicture->SetTex( L"DLG_Common_Texture08.TGA", L"Dash" );
		break;

	case CP_DASH_LEFT:
		pPicture->SetTex( L"DLG_Common_Texture08.TGA", L"Dash_Left" );
		break;

	case CP_SIEGE:
		pPicture->SetTex( L"DLG_Common_Texture08.TGA", L"S" );
		break;

	case CP_SPACE:
		pPicture->SetTex( L"DLG_Common_Texture13.tga", L"Space" );
		break;

	case CP_REVENGE:
		pPicture->SetTex( L"DLG_Common_Texture13.tga", L"Hit_Back" );
		break;

	case CP_LEFT_PRESSED:
		pPicture->SetTex( L"DLG_KEYS.TGA", L"Left2" );
		break;

	case CP_RIGHT_PRESSED:
		pPicture->SetTex( L"DLG_KEYS2.TGA", L"Right2" );
		break;

	case CP_UP_PRESSED:
		pPicture->SetTex( L"DLG_KEYS.TGA", L"Up2" );
		break;

	case CP_DOWN_PRESSED:
		pPicture->SetTex( L"DLG_KEYS2.TGA", L"Down2" );
		break;

	case CP_BURNING_RUSH:
		pPicture->SetTex( L"DLG_Common_Texture13.TGA", L"Burning_Rush" );
		break;

	case CP_SUCCESSIVE_HITS:
		pPicture->SetTex( L"DLG_KEYS2.TGA", L"Combo_Hit" );
		break;

	//case CP_PLUS:
	//	//pPicture->SetTex( L"DLG_KEYS.TGA", L"Down2" );
	//	break;

	//case CP_PRESSED_SMALL:
	//	pPicture->SetTex( L"DLG_KEYS.TGA", L"Text_Down" );
	//	break;

	default:
		pPicture->SetTex( L"FadeInOut.dds" );
		break;
	}
}




void CX2TrainingGameUI::OpenSlideWindow( bool bOpen )
{
	if( true == bOpen )
	{
		if( NULL == m_pDLGSlideShot )
		{
			m_pDLGSlideShot = new CKTDGUIDialog( (CKTDXStage*)m_pNowState, L"DLG_Training_GameUI.lua" );
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGSlideShot );
		}

		m_pDLGSlideShot->SetShowEnable( true, true );

	}	
	else
	{
		m_pDLGSlideShot->SetShowEnable( false, false );
	}
}

void CX2TrainingGameUI::OpenFreeTrainingMainMenu( bool bOpen )
{
	if( true == bOpen )
	{
		if( NULL == m_pDLGFreeTrainingMenu )
		{
			m_pDLGFreeTrainingMenu = new CKTDGUIDialog( (CKTDXStage*) m_pNowState, L"DLG_Free_Training_Menu.lua" );
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGFreeTrainingMenu );
		}

		m_pDLGFreeTrainingMenu->SetShowEnable( true, true );

		CKTDGUIComboBox* pCombo = (CKTDGUIComboBox*)m_pDLGFreeTrainingMenu->GetControl(L"TNpcList");
		if( pCombo != NULL )
		{
			pCombo->RemoveAllItems();

			m_vecNpc.clear();
			m_vecNpc.push_back( CX2UnitManager::NUI_MUSHROOM_WOODEN );
			m_vecNpc.push_back( CX2UnitManager::NUI_MONKEY_WOOD_SMALL );
			m_vecNpc.push_back( CX2UnitManager::NUI_MONKEY_WOOD_BIG );
	#ifdef _IN_HOUSE_
			//오현빈//2012-11-27//사내 테스트를 위해 자유훈련장 NPC 4종 추가
			m_vecNpc.push_back( CX2UnitManager::NUI_DUMMY_MOVE );
			m_vecNpc.push_back( CX2UnitManager::NUI_DUMMY_WAIT );
			m_vecNpc.push_back( CX2UnitManager::NUI_DUMMY_BIG_MOVE );
			m_vecNpc.push_back( CX2UnitManager::NUI_DUMMY_BIG_WAIT );
	#endif //_IN_HOUSE_

			for(int iItem=0; iItem<ARRAY_SIZE(NPC_CARD); ++iItem)
			{
				//CX2Inventory *pInven = g_pData->GetMyUser()->GetSelectUnit()->GetInventory();
				CX2Item* pItem = g_pData->GetMyUser()->GetSelectUnit()->GetInventory().GetItemByTID( NPC_CARD[iItem] );
				if( pItem == NULL )
					continue;

#ifdef DUPLICATE_NPC_PROCESSING_BY_TRAINING
				vector<CX2UnitManager::NPC_UNIT_ID>::iterator vit = m_vecNpc.begin();
				
				for( ; vit != m_vecNpc.end(); ++vit )
				{
					if( pItem->GetItemTemplet()->GetSummonNpcID() == *vit )
						break;
				}

				if(  vit != m_vecNpc.end() )	//만약 중복되는 NPCID가 있다면 삽입 금지
					continue;
#endif DUPLICATE_NPC_PROCESSING_BY_TRAINING

				m_vecNpc.push_back( pItem->GetItemTemplet()->GetSummonNpcID() );
			}

			for(UINT i=0; i<m_vecNpc.size(); ++i)
			{
				const CX2UnitManager::NPCUnitTemplet *pNpcTemplet = g_pData->GetUnitManager()->GetNPCUnitTemplet( m_vecNpc[i] );
				if( pNpcTemplet != NULL )
				{
					pCombo->AddItem( pNpcTemplet->m_Name.c_str(), NULL );
				}
			}			
		}

		pCombo = (CKTDGUIComboBox*)m_pDLGFreeTrainingMenu->GetControl(L"TNpcLevel");
		if( pCombo != NULL )
		{
			pCombo->RemoveAllItems();

			int iCount = 0;
			int iSelectedIndex = 0;
			int iLevel = g_pData->GetSelectUnitLevel();
			for(int i= -10; i<=10; ++i)
			{
				int iNpcLv = iLevel + i;

				if( iNpcLv <= 0 )
					continue;

				WCHAR wszText[64] = L"";
#ifdef HARDCODING_STRING_BR
				StringCchPrintfW( wszText, ARRAY_SIZE(wszText), L"Nivel. %d", iNpcLv );
#else // HARDCODING_STRING_BR
				StringCchPrintfW( wszText, ARRAY_SIZE(wszText), L"Lv. %d", iNpcLv );
#endif // HARDCODING_STRING_BR
				pCombo->AddItem(wszText, NULL);

				if( iNpcLv == iLevel )
					iSelectedIndex = iCount;

				++iCount;
			}			
			pCombo->SetSelectedByIndex(iSelectedIndex);
		}	

	}	
	else
	{
		m_pDLGFreeTrainingMenu->SetShowEnable( false, false );
	}
}
