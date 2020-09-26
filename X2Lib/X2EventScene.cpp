#include "StdAfx.h"
#include ".\X2EventScene.h"


#ifdef EVENT_SCENE
//{{ oasis907 : 김상윤 [2010.6.17] // 
CX2EventScene::CX2EventScene(void)
{
	m_bIsPlayingEventScene = false;
	m_pNowPlayingEventScene = NULL;

	m_bIsWaitNextEventScene	= false;
	m_wstrNextEventScene	= L"";

	m_fTextPlayElapsedTime	= 0.f;


	m_pNowState			= NULL;
	m_bNowPresent		= false;
	m_TextBoxShow		= false;
	m_NowScenNum		= 0;
	m_fScenTimeBefore	= 0.0f;
	m_fScenTime			= 0.0f;
	m_fElapsedTimeAfterTextSpread	= 0.0f;


	m_bBlackOutStart = false;
	m_bBlackOutEnd = false;

 	m_fBlackOutStartLeftTime = 1.f;
 	m_fBlackOutStartDuration = 0.f;
 	m_fBlackOutEndLeftTime = 0.f;
	m_fBlackOutEndDuration = 0.f;

#ifdef ELSWORD_NEW_BEGINNING
	m_fBlackOutDestAlpha	= 0.75f;
	m_fWhiteOutTime			= 0.5f;
#endif ELSWORD_NEW_BEGINNING

	m_fEventScenePlayDelayTime = 0.f;
	m_bWaitDelayEventScene = false;
	
	m_bNotifyNewEpicQuest = false;

#ifdef EVENT_SCENE_TOOL
	m_pParticleSystem = new CKTDGParticleSystem( g_pKTDXApp->GetDevice() );
	m_pXMeshPlayer = new CKTDGXMeshPlayer();

	lua_tinker::class_add<CX2EventScene>( g_pKTDXApp->GetLuaBinder()->GetLuaState(), "CX2EventScene" );
	lua_tinker::class_def<CX2EventScene>( g_pKTDXApp->GetLuaBinder()->GetLuaState(), "AddEventSceneObject_LUA",			&CX2EventScene::AddEventSceneObject_LUA );
	lua_tinker::class_def<CX2EventScene>( g_pKTDXApp->GetLuaBinder()->GetLuaState(), "AddEventScene_LUA",				&CX2EventScene::AddEventScene_LUA );


	//{{begin} CX2EventScene 에서 필요힌 함수 바인딩 및 루아 데이터 처리
	if( g_pKTDXApp->LoadLuaTinker(L"Enum.lua") == false )
	{
		return;
	}
	if( g_pKTDXApp->LoadLuaTinker(L"DungeonEnum.lua") == false )
	{
		return;
	}
	if( g_pKTDXApp->LoadLuaTinker(L"StringID_def.lua") == false )
	{
		return;
	}
	//{{end} CX2EventScene 에서 필요힌 함수 바인딩 및 루아 데이터 처리


	g_pKTDXApp->GetDGManager()->GetFontManager()->CreateNewUIFont_LUA( XF_DODUM_11_NORMAL, "2002L_KOG", 12, 400		);
	g_pKTDXApp->GetDGManager()->GetFontManager()->CreateNewUIFont_LUA( XF_DODUM_15_NORMAL, "2002L_KOG", 16, 400		);

	g_pKTDXApp->GetDGManager()->GetFontManager()->CreateNewUIUKFont_LUA( XUF_DODUM_11_NORMAL,	"2002L_KOG", 12, 400,	1	);


	g_pKTDXApp->GetDGManager()->GetFontManager()->CreateNewUIUKFont_LUA( XUF_DODUM_13_SEMIBOLD,	"2002L_KOG",	14, 400, 1	);
	g_pKTDXApp->GetDGManager()->GetFontManager()->CreateNewUIUKFont_LUA( XUF_DODUM_15_BOLD,		"2002L_KOG",	16, 400, 1	);
	g_pKTDXApp->GetDGManager()->GetFontManager()->CreateNewUIUKFont_LUA( XUF_DODUM_20_BOLD,		"2002L_KOG",	21, 400, 1	);

	g_pKTDXApp->GetDGManager()->GetFontManager()->CreateNewUIUKFont_LUA( XUF_HEADLINE_30_NORMAL,	"HY헤드라인M",	30, 400, 1	);

	m_pDLGSceneID		= new CKTDGUIDialog( NULL, NULL );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGSceneID );
	
	m_pDLGSceneID->SetShow( true );
	m_pDLGSceneID->SetFront( true );
	m_pDLGSceneID->SetLayer(XDL_DIRECTING + 5);


	CKTDGUIStatic* pStatic = new CKTDGUIStatic();
	pStatic->SetName( L"SceneID" );
	m_pDLGSceneID->AddControl( pStatic );
	CKTDGUIControl::CPictureData* pPicture = new CKTDGUIControl::CPictureData();
	pStatic->AddPicture( pPicture );
	pPicture->SetEdge( false, 2, D3DXCOLOR(1,1,1,1) );
	pPicture->SetTex( L"FadeInOut.dds" );

	CKTDGUIControl::UIStringData* pString = new CKTDGUIControl::UIStringData();
	pStatic->AddString( pString );
	pString->fontIndex = XUF_HEADLINE_30_NORMAL;
	pString->color = D3DXCOLOR( 1,1,1,1 );
	pString->outlineColor = D3DXCOLOR( 1,1,1,1 );
	pString->msg = L"";
	pString->fontStyle = CKTDGFontManager::FS_NONE;
	pString->sortFlag = DT_LEFT;
	pString->pos = D3DXVECTOR2( 450, 120 );
	


#endif EVENT_SCENE_TOOL

	m_pDLGSlideShot		= NULL;
	m_pDLGSlideShot		= new CKTDGUIDialog( NULL, L"DLG_EventScene.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGSlideShot );
	m_pDLGSlideShot->SetShowEnable( m_TextBoxShow, m_TextBoxShow );

	m_pDLGSlideShot->SetLayer(XDL_DIRECTING + 3);

	m_pDLGBlackLayOut	= NULL;
#ifdef ELSWORD_NEW_BEGINNING
	m_pDLGBlackLayOut	= new CKTDGUIDialog( NULL, L"DLG_EventScene_BlackLayOut.lua" , 0.f);
#else
	m_pDLGBlackLayOut	= new CKTDGUIDialog( NULL, L"DLG_EventScene_BlackLayOut.lua" );
#endif ELSWORD_NEW_BEGINNING
	m_pDLGBlackLayOut->SetLayer(XDL_DIRECTING + 1);

	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGBlackLayOut );
#ifdef ELSWORD_NEW_BEGINNING
	m_pDLGBlackLayOut->SetShowEnable( false, false );
#else
	m_pDLGBlackLayOut->SetShowEnable( m_TextBoxShow, m_TextBoxShow );
#endif ELSWORD_NEW_BEGINNING

#ifndef EVENT_SCENE_TOOL
	m_bSetCursor = false;
#endif EVENT_SCENE_TOOL
/*
	CKTDGUIStatic* pStatic_Speech	= (CKTDGUIStatic*) m_pDLGSlideShot->GetControl( L"INVISIBLE" );

	pStatic_Speech->SetSize_LUA(2000.f,2000.f);
*/
}

CX2EventScene::~CX2EventScene(void)
{
	m_TextBoxShow = false;
	SAFE_DELETE_DIALOG( m_pDLGSlideShot );
	SAFE_DELETE_DIALOG( m_pDLGBlackLayOut );
#ifdef EVENT_SCENE_TOOL
	SAFE_DELETE_DIALOG( m_pDLGSceneID );
	SAFE_DELETE(m_pParticleSystem);
	SAFE_DELETE(m_pXMeshPlayer);
#endif EVENT_SCENE_TOOL
	m_mapEventScene.clear();
	m_mapEventSceneObject.clear();
}
void CX2EventScene::TextPlayFrameMove( double fTime, float fElapsedTime )
{
	if(m_bIsPlayingEventScene)
	{
		m_fTextPlayElapsedTime += fElapsedTime;
		
		
		if(m_iterTE->m_fWaitTime > 0.f)
		{
			if(m_iterTE->m_fWaitTime < m_fTextPlayElapsedTime)
			{
				m_iterTE++;
				PlayText();
			}
		}
	}
	return;
}


void CX2EventScene::BlackOutFrameMove( double fTime, float fElapsedTime )
{
	if(m_bBlackOutStart)
	{
		m_fBlackOutStartLeftTime = 0.f;
#ifdef ELSWORD_NEW_BEGINNING
		m_fBlackOutStartDuration = m_fWhiteOutTime;
#else
		m_fBlackOutStartDuration = 0.5f;
#endif ELSWORD_NEW_BEGINNING
		m_bBlackOutStart = false;
	}

	if(m_bBlackOutEnd)
	{
#ifdef ELSWORD_NEW_BEGINNING
		m_fBlackOutEndLeftTime = m_fWhiteOutTime;
		m_fBlackOutEndDuration = m_fWhiteOutTime;
#else
		m_fBlackOutEndLeftTime = 0.5f;
		m_fBlackOutEndDuration = 0.5f;
#endif ELSWORD_NEW_BEGINNING
		m_bBlackOutEnd = false;
	}

	if(m_fBlackOutStartLeftTime < m_fBlackOutStartDuration)
	{
		m_fBlackOutStartLeftTime += fElapsedTime;
		CKTDGUIStatic* pStaticInvisible	= (CKTDGUIStatic*) m_pDLGBlackLayOut->GetControl( L"INVISIBLE" );
#ifdef ELSWORD_NEW_BEGINNING
		if(m_fBlackOutStartLeftTime > m_fBlackOutStartDuration)
		{
			m_fBlackOutStartLeftTime = m_fBlackOutStartDuration;
		}

		float fColorAlpha = (m_fBlackOutStartLeftTime/m_fBlackOutStartDuration) * m_fBlackOutDestAlpha;
#else
		float fColorAlpha = (m_fBlackOutStartLeftTime/m_fBlackOutStartDuration) * 0.75f;
#endif ELSWORD_NEW_BEGINNING
		pStaticInvisible->GetPicture(0)->SetColor(D3DXCOLOR(1.f,1.f,1.f,fColorAlpha));
	}

	if(m_fBlackOutEndLeftTime > 0.f)
	{
		m_fBlackOutEndLeftTime -= fElapsedTime;

		CKTDGUIStatic* pStaticInvisible	= (CKTDGUIStatic*) m_pDLGBlackLayOut->GetControl( L"INVISIBLE" );
#ifdef ELSWORD_NEW_BEGINNING
		if(m_fBlackOutEndLeftTime <= 0.f)
		{
			m_fBlackOutEndLeftTime = 0.f;
			m_pDLGBlackLayOut->SetShowEnable(false, false);
		}

		float fColorAlpha = (m_fBlackOutEndLeftTime/m_fBlackOutEndDuration) * m_fBlackOutDestAlpha;
#else
		float fColorAlpha = (m_fBlackOutEndLeftTime/m_fBlackOutEndDuration) * 0.75f;
#endif ELSWORD_NEW_BEGINNING


		pStaticInvisible->GetPicture(0)->SetColor(D3DXCOLOR(1.f,1.f,1.f,fColorAlpha));
	}

	return;
}


void CX2EventScene::OnFrameMove( double fTime, float fElapsedTime )
{
#ifdef EVENT_SCENE_TOOL
	if( m_pXMeshPlayer != NULL )
		m_pXMeshPlayer->OnFrameMove( fTime, fElapsedTime );

	if( m_pParticleSystem != NULL )
		m_pParticleSystem->OnFrameMove( fTime, fElapsedTime );
#endif EVENT_SCENE_TOOL


	BlackOutFrameMove(fTime, fElapsedTime);

	TextPlayFrameMove(fTime, fElapsedTime);

	if(m_bWaitDelayEventScene)
	{
		if(m_fEventScenePlayDelayTime > 0.f)
		{
			m_fEventScenePlayDelayTime -= fElapsedTime;
		}

		if(m_fEventScenePlayDelayTime <= 0.f)
		{

			if(m_wstrWaitEventScene.size() != 0)
			{
				wstring wstrPlayEventScene = m_wstrWaitEventScene[0];
				m_wstrWaitEventScene.pop_front();
				PlayEventScene(wstrPlayEventScene);
			}
			m_bWaitDelayEventScene = false;
			m_fEventScenePlayDelayTime = 0.f;
		}
	}

	if( m_bNowPresent == true )
	{

// 		if( g_pX2Game != NULL )
// 		{
// 			g_pX2Game->SetShowAllUnitGageBar( false );
// 			g_pX2Game->SetRenderUserName( false );
// 		}

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

		m_fScenTimeBefore = m_fScenTime;
		m_fScenTime += fElapsedTime;
	}


	if(m_bIsPlayingEventScene)
	{
		if(false == g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckFrontModalDlg( m_pDLGSlideShot ) )	// 앞에 모달 다이얼로그가 없으면
		{
			GET_KEY_STATE( GA_UP );
			GET_KEY_STATE( GA_DOWN );
			GET_KEY_STATE( GA_LEFT );
			GET_KEY_STATE( GA_RIGHT );
			g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_F1 );
#ifndef NOT_APPLY_F8
			g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_F8 ); // 파티시작 키
#endif
			
			GET_KEY_STATE( GA_ATTACK_FAST );

			if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_RETURN ) == TRUE )
			{
				if(IsTextSpreading() == true)
				{
					SetTextSpread();
				}
				else
				{
					m_iterTE++;
					PlayText();
				}
			}
			else if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( DIK_ESCAPE ) == TRUE )
			{
				EndEventScene(true);
			}
		}
	}
}

bool CX2EventScene::EventTimer( float fTime )
{
	if( fTime > m_fScenTimeBefore && fTime <= m_fScenTime )
	{
		return true;
	}
	return false;
}

void CX2EventScene::CannotInput( float fTime )
{
// 	if( g_pX2Game != NULL )
// 	{
// 		if( g_pX2Game->GetMyUnit() != NULL )
// 		{
// 			g_pX2Game->GetMyUnit()->SetCanNotInputTime_LUA( fTime );
// 		}
// 	}
}




void CX2EventScene::AddText( bool bTagLeft, int NameID, const WCHAR* wszMsg,
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
	CKTDGFontManager::CUKFont* pFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( pStatic_Speech->GetString(0)->fontIndex );
	if( NULL == pFont )
		return; 

	int addRow = CWordLineHandler::LineBreakInX2Main(chatContent, pFont, CHAT_LINE_WIDTH, wstrColor, true, false);

	if( -1 == addRow )
		return;

	CKTDGUIStatic* pStatic_Name		= (CKTDGUIStatic*) m_pDLGSlideShot->GetControl( L"Name" );
#else //#ifdef CLIENT_GLOBAL_LINEBREAK
	int addRow = LineBreak( CHAT_LINE_WIDTH, chatContent, nextLineString.c_str(), wstrColor );
	if( -1 == addRow )
		return;


	CKTDGUIStatic* pStatic_Name		= (CKTDGUIStatic*) m_pDLGSlideShot->GetControl( L"Name" );
	CKTDGUIStatic* pStatic_Speech	= (CKTDGUIStatic*) m_pDLGSlideShot->GetControl( L"Speech" );
#endif //CLIENT_GLOBAL_LINEBREAK


	wstring wstrName = GET_STRING( NameID );

	pStatic_Name->GetString(0)->msg		= wstrName;
	pStatic_Speech->GetString(0)->msg	= chatContent;

#ifdef FIX_EVENT_SCENE_LINE_BREAK_BUG
	wstring::size_type sizeType = 0;
	while(true)
	{
		// 개행문자 위치 찾기
		sizeType = pStatic_Speech->GetString(0)->msg.find( L"\n", sizeType ) + 1;

		// 더 이상 개행문자가 없으면 break
		if( 0 == sizeType )
		{
			break;
		}

		// 색상코드가 들어가있지 않으면 색상 추가 
		if( sizeType != pStatic_Speech->GetString(0)->msg.find(L"#", sizeType-1) )
		{
			pStatic_Speech->GetString(0)->msg.insert( sizeType, L"#C000000");
		}
	}
#endif // FIX_EVENT_SCENE_LINE_BREAK_BUG


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
		if( true == bTagLeft )
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



void CX2EventScene::SetTextSpread()
{
	if( NULL == m_pDLGSlideShot )
		return;

	CKTDGUIStatic* pStatic_Speech	= (CKTDGUIStatic*) m_pDLGSlideShot->GetControl( L"Speech" );
	if( pStatic_Speech != NULL )
	{
		pStatic_Speech->GetString(0)->bSpread = false;
	}
}

bool CX2EventScene::IsTextSpreading()
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


void CX2EventScene::ClearText()
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

int CX2EventScene::LineBreak( int iWidth, wstring& wstrSpeech, const WCHAR* pNextLineString, wstring wstrColor /*= L""*/ )
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

#ifdef FIX_EVENT_SCENE_LINE_BREAK_BUG
		if( *pNextLineString == wChar )
			iStringSize = 0;
#endif // FIX_EVENT_SCENE_LINE_BREAK_BUG

		//{{ 허상형 : [2010/12/20/] //	이벤트씬에서 '\n' 안먹히는 문제 수정
#ifdef FIX_EVENT_SCENE_ENTER_FLAG
		if( wChar == L'\n' )
		{
			i++;
			addRow++;
			iStringSize = 0;

			wstrSpeech.insert( wstrSpeech.begin()+i, wstrColor.begin(), wstrColor.end() );
			i += wstrColor.length();
		}
#endif	//	FIX_EVENT_SCENE_ENTER_FLAG
		//}} 허상형 : [2010/12/20/] //	이벤트씬에서 '\n' 안먹히는 문제 수정

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

#ifdef ELSWORD_NEW_BEGINNING
void CX2EventScene::BlackLayOutMove( bool bInning, float fTime, bool bPanorama /*= true*/, bool WhiteOut /*= false*/, float WhiteOutTime /*=0.5f*/, float DestAlpha /*= 0.75f*/)
#else
void CX2EventScene::BlackLayOutMove( bool bInning, float fTime )
#endif ELSWORD_NEW_BEGINNING
{
#ifdef ELSWORD_NEW_BEGINNING

	m_fBlackOutDestAlpha = DestAlpha;
	m_fWhiteOutTime = WhiteOutTime;

	CKTDGUIStatic* pStaticInvisible	= (CKTDGUIStatic*) m_pDLGBlackLayOut->GetControl( L"INVISIBLE" );
	if(WhiteOut == true)
		pStaticInvisible->GetPicture(0)->SetTex(L"DLG_Common_Texture01.TGA", L"White");
	else
		pStaticInvisible->GetPicture(0)->SetTex(L"DLG_Common_Texture01.TGA", L"Black");

	if( bPanorama == true)
	{
		if ( bInning == true )
		{
			m_bBlackOutStart = true;

			m_pDLGBlackLayOut->SetShowEnable(true, true);
			CKTDGUIControl* pBlackLayOutUp = m_pDLGBlackLayOut->GetControl( L"BlackLayOutUp" );
			if ( pBlackLayOutUp != NULL )
				pBlackLayOutUp->Move( D3DXVECTOR2(0,-20), D3DXCOLOR(1,1,1,1), fTime , true);

			CKTDGUIControl* pBlackLayOutDown = m_pDLGBlackLayOut->GetControl( L"BlackLayOutDown" );
			if ( pBlackLayOutDown != NULL )
				pBlackLayOutDown->Move( D3DXVECTOR2(0,20), D3DXCOLOR(1,1,1,1), fTime ,true);

			pBlackLayOutUp->SetShowEnable(true, true);
			pBlackLayOutDown->SetShowEnable(true, true);
		}
		else
		{
			m_bBlackOutEnd = true;

			m_pDLGBlackLayOut->SetShowEnable(true, true);
			CKTDGUIControl* pBlackLayOutUp = m_pDLGBlackLayOut->GetControl( L"BlackLayOutUp" );
			if ( pBlackLayOutUp != NULL )
				pBlackLayOutUp->Move( D3DXVECTOR2(0,-90), D3DXCOLOR(1,1,1,1), fTime ,true);

			CKTDGUIControl* pBlackLayOutDown = m_pDLGBlackLayOut->GetControl( L"BlackLayOutDown" );
			if ( pBlackLayOutDown != NULL )
				pBlackLayOutDown->Move( D3DXVECTOR2(0, 90), D3DXCOLOR(1,1,1,1), fTime ,true);

			pBlackLayOutUp->SetShowEnable(true, true);
			pBlackLayOutDown->SetShowEnable(true, true);
		}
		
	}
	else
	{

		if ( bInning == true )
		{
			m_bBlackOutStart = true;
			CKTDGUIStatic* pStaticInvisible	= (CKTDGUIStatic*) m_pDLGBlackLayOut->GetControl( L"INVISIBLE" );
			pStaticInvisible->GetPicture(0)->SetColor(D3DXCOLOR(1.f,1.f,1.f,0.f));
		}
		else
		{
			m_bBlackOutEnd = true;
			CKTDGUIStatic* pStaticInvisible	= (CKTDGUIStatic*) m_pDLGBlackLayOut->GetControl( L"INVISIBLE" );
			pStaticInvisible->GetPicture(0)->SetColor(D3DXCOLOR(1.f,1.f,1.f,m_fBlackOutDestAlpha));
		}
		m_pDLGBlackLayOut->SetShowEnable(true, true);
		CKTDGUIControl* pBlackLayOutUp = m_pDLGBlackLayOut->GetControl( L"BlackLayOutUp" );
		CKTDGUIControl* pBlackLayOutDown = m_pDLGBlackLayOut->GetControl( L"BlackLayOutDown" );
		pBlackLayOutUp->SetShowEnable(false, false);
		pBlackLayOutDown->SetShowEnable(false, false);
	}


#else
	if ( bInning == true )
	{
		m_bBlackOutStart = true;
#ifdef FIX_EVENT_SCENE_BLACK_LAYOUT_BUG
		m_bBlackOutEnd = false;
#endif //FIX_EVENT_SCENE_BLACK_LAYOUT_BUG

		m_pDLGBlackLayOut->SetShowEnable(true, true);
		CKTDGUIControl* pBlackLayOutUp = m_pDLGBlackLayOut->GetControl( L"BlackLayOutUp" );
		if ( pBlackLayOutUp != NULL )
			pBlackLayOutUp->Move( D3DXVECTOR2(0,-20), D3DXCOLOR(1,1,1,1), fTime , true);

		CKTDGUIControl* pBlackLayOutDown = m_pDLGBlackLayOut->GetControl( L"BlackLayOutDown" );
		if ( pBlackLayOutDown != NULL )
			pBlackLayOutDown->Move( D3DXVECTOR2(0,20), D3DXCOLOR(1,1,1,1), fTime ,true);
	}
	else
	{
		m_bBlackOutEnd = true;

		m_pDLGBlackLayOut->SetShowEnable(true, true);
		CKTDGUIControl* pBlackLayOutUp = m_pDLGBlackLayOut->GetControl( L"BlackLayOutUp" );
		if ( pBlackLayOutUp != NULL )
			pBlackLayOutUp->Move( D3DXVECTOR2(0,-90), D3DXCOLOR(1,1,1,1), fTime ,true);

		CKTDGUIControl* pBlackLayOutDown = m_pDLGBlackLayOut->GetControl( L"BlackLayOutDown" );
		if ( pBlackLayOutDown != NULL )
			pBlackLayOutDown->Move( D3DXVECTOR2(0, 90), D3DXCOLOR(1,1,1,1), fTime ,true);
	}
#endif ELSWORD_NEW_BEGINNING
	return;
}


void CX2EventScene::SetNameTabShow( bool bShow )
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


bool CX2EventScene::PlaySceneObjectParticle(EventSceneObject* pEventSceneObject, SceneObectPlay* pSceneObjectPlay)
{
#ifdef EVENT_SCENE_TOOL
	CKTDGParticleSystem::CParticleEventSequence* newSeq = CKTDGParticleSystem::CParticleEventSequence::CreateParticleEventSequence( m_pParticleSystem, NULL );
#else
//// 1. 빈 CParticleEventSequence 객체 생성 /////////////////////////////////////////////
	CKTDGParticleSystem::CParticleEventSequence* newSeq = CKTDGParticleSystem::CParticleEventSequence::CreateParticleEventSequence( g_pData->GetUIMajorParticle(), NULL );
#endif EVENT_SCENE_TOOL
//// 2. 파리미터 설정 //////////////////////////////

	newSeq->SetParticleType(CKTDGParticleSystem::PT_3D_PLANE);
	newSeq->SetLayer(15);
	newSeq->SetSrcBlendMode(D3DBLEND_SRCALPHA);
	newSeq->SetDestBlendMode(D3DBLEND_INVSRCALPHA);
	newSeq->SetForceLayer(false);
	//newSeq->SetResolutionConvert(true);
	newSeq->SetMaxParticleNum(1);
	newSeq->SetLifetime(CMinMax<float>(99999, 99999));
	newSeq->SetEmitRate(1000.f,1000.f);
	//renderType = CKTDGXRenderer::RT_REAL_COLOR
	newSeq->SetMeshRenderType(CKTDGXRenderer::RT_REAL_COLOR);


//// 3. 타임 이벤트 설정 /////////////////////////////////
	// 텍스쳐 설정
	CKTDGParticleSystem::CParticleEvent_Texture* pEventTexture = new CKTDGParticleSystem::CParticleEvent_Texture();
	
	pEventTexture->SetTextureName(pEventSceneObject->m_wstrFileName.c_str());
	pEventTexture->SetActualTime( CMinMax<float>(0, 0) );
	pEventTexture->SetFade( false );



	newSeq->m_EventList.push_back(pEventTexture);


	// 텍스쳐 사이즈 파싱 필요함
	// 사이즈 설정
	CKTDGParticleSystem::CParticleEvent_Size* pEventSize = new CKTDGParticleSystem::CParticleEvent_Size();

	D3DXVECTOR3 TextureSize = D3DXVECTOR3(pEventSceneObject->m_Size.x, pEventSceneObject->m_Size.y, 1.f);

	pEventSize->SetSize(CMinMax<D3DXVECTOR3> (TextureSize, TextureSize) );
	pEventSize->SetActualTime( CMinMax<float>(0, 0) );
	pEventSize->SetFade( false );

	newSeq->m_EventList.push_back(pEventSize);



	// Position 설정
	SCENE_OBJECT_POSITION eDrawStartPos = pSceneObjectPlay->m_eStartPosition;
	D3DXVECTOR3 StartPos = pEventSceneObject->m_vecPosSet[eDrawStartPos];

	// SCREEN OUT 기능
	if(eDrawStartPos == CX2EventScene::OP_LEFT_SCREEN_OUT || eDrawStartPos == CX2EventScene::OP_RIGHT_SCREEN_OUT)
	{
		SCENE_OBJECT_POSITION eDrawEndPos = pSceneObjectPlay->m_eMovePosition;
		D3DXVECTOR3 EndPos = pEventSceneObject->m_vecPosSet[eDrawEndPos];
		StartPos.y = EndPos.y;
	}

	float fMoveStartTime = pSceneObjectPlay->m_fMoveStartTime;


	CKTDGParticleSystem::CParticleEvent_Position* pEventPosition = new CKTDGParticleSystem::CParticleEvent_Position();

	StartPos.x = StartPos.x + 0.5f;
	StartPos.y = StartPos.y + 0.5f;

	pEventPosition->SetPosition(CMinMax<D3DXVECTOR3> (StartPos, StartPos) );
	pEventPosition->SetActualTime( CMinMax<float>(fMoveStartTime, fMoveStartTime) );
	pEventPosition->SetFade( false );

	newSeq->m_EventList.push_back(pEventPosition);

	SCENE_OBJECT_POSITION eFinalPos = CX2EventScene::OP_NONE;
	if(pSceneObjectPlay->m_eMovePosition != CX2EventScene::OP_NONE)
	{

		SCENE_OBJECT_POSITION eDrawEndPos = pSceneObjectPlay->m_eMovePosition;
		D3DXVECTOR3 EndPos = pEventSceneObject->m_vecPosSet[eDrawEndPos];
		
		// SCREEN OUT 기능
		if(eDrawEndPos == CX2EventScene::OP_LEFT_SCREEN_OUT || eDrawEndPos == CX2EventScene::OP_RIGHT_SCREEN_OUT)
		{
			EndPos.y = StartPos.y;
		}
		

		float fMoveDuration = pSceneObjectPlay->m_fMoveDuration;
		

		// Position 설정
		CKTDGParticleSystem::CParticleEvent_Position* pEventPosition2 = new CKTDGParticleSystem::CParticleEvent_Position();

		EndPos.x = EndPos.x + 0.5f;
		EndPos.y = EndPos.y + 0.5f;

		pEventPosition2->SetPosition(CMinMax<D3DXVECTOR3> (EndPos, EndPos) );

		pEventPosition2->SetFade(true);

		pEventPosition2->SetActualTime( CMinMax<float>(0, fMoveStartTime + fMoveDuration) );

		newSeq->m_EventList.push_back(pEventPosition2);

		eFinalPos = eDrawEndPos;
	}
	else
	{
		eFinalPos = pSceneObjectPlay->m_eStartPosition;;
	}

	// Shake 이벤트
	if(pSceneObjectPlay->m_fShakeDuration > 0.f)
	{
		float fShakeDuration = pSceneObjectPlay->m_fShakeDuration;
		float fShakeGapX = pSceneObjectPlay->m_fShakeGap / 2;
		float fShakeGapY = pSceneObjectPlay->m_fShakeGap;


		CKTDGParticleSystem::CParticleEvent_Crash* pEventCrash = new CKTDGParticleSystem::CParticleEvent_Crash();

		pEventCrash->SetFade( true );
		pEventCrash->SetCrash( CMinMax<D3DXVECTOR3>(D3DXVECTOR3(-fShakeGapX,-fShakeGapY,0),D3DXVECTOR3(fShakeGapX,fShakeGapY,0)) );


		pEventCrash->SetActualTime( CMinMax<float>(0.0f, fShakeDuration) );

		newSeq->m_EventList.push_back(pEventCrash);
	}

	//  MirrorVertical
	if(pSceneObjectPlay->m_bMirrorVertical == true)
	{
		CKTDGParticleSystem::CParticleEvent_Rotate* pEventRotate = new CKTDGParticleSystem::CParticleEvent_Rotate();

		pEventRotate->SetRotate(CMinMax<D3DXVECTOR3> (D3DXVECTOR3(0,180,0), D3DXVECTOR3(0,180,0)) );
		pEventRotate->SetActualTime( CMinMax<float>(0, 0) );
		pEventRotate->SetFade( false );

		newSeq->m_EventList.push_back(pEventRotate);
	}


	newSeq->SetShowObject(false);

    CKTDGParticleSystem::CParticleEventSequenceHandle newSeqHandle =
#ifdef EVENT_SCENE_TOOL
	m_pParticleSystem->CreateInstanceNonTemplet(newSeq, D3DXVECTOR3(0,0,0), D3DXVECTOR2(-1,-1), D3DXVECTOR2(-1,-1) ); 
#else
//// 4. 파티클 생성, 재생 /////////////////////////////////
	g_pData->GetUIMajorParticle()->CreateInstanceNonTemplet(newSeq, D3DXVECTOR3(0,0,0), D3DXVECTOR2(-1,-1), D3DXVECTOR2(-1,-1) ); 
#endif EVENT_SCENE_TOOL	
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    if ( newSeqHandle == INVALID_PARTICLE_SEQUENCE_HANDLE )
    {
        SAFE_DELETE_KTDGOBJECT( newSeq );
        return false;
    }
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	CKTDGParticleSystem::CParticleEventSequenceHandle newSeqHandle = newSeq->GetHandle();
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

	// Layer 다이얼로그 생성, 벡터에 등록
	CKTDGUIDialogType pDlgObjectLayer = NULL;
	pDlgObjectLayer	= new CKTDGUIDialog( NULL, NULL );


	pDlgObjectLayer->SetFront( true );
	pDlgObjectLayer->SetLayer(XDL_DIRECTING + 2);

	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( pDlgObjectLayer );
	pDlgObjectLayer->SetShowEnable( true, true );


	CSceneObject* pSceneObject = new CSceneObject();

#ifdef EVENT_SCENE_TOOL
	pSceneObject->m_pParticleSystem = m_pParticleSystem;
	pSceneObject->m_pXMeshPlayer = m_pXMeshPlayer;
#endif EVENT_SCENE_TOOL

	pSceneObject->m_wstrObjectName = pSceneObjectPlay->m_wstrObjectName;
	pSceneObject->m_eSceneObjectType = OT_TEXTURE;
#ifdef  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
    pSceneObject->m_hSceneObjectParticleHandle = newSeqHandle;
    pSceneObject->m_hSceneObjectMeshHandle = INVALID_MESH_INSTANCE_HANDLE;
#else   X2OPTIMIZE_HANDLE_VALIDITY_CHECK
	pSceneObject->m_iSceneObjectHandle = newSeqHandle;
#endif  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
	pSceneObject->m_DlgHandle = pDlgObjectLayer;

	pSceneObject->m_eLastPosition = eFinalPos;
	pSceneObject->m_bMirrorVertical = pSceneObjectPlay->m_bMirrorVertical;

	m_listPlayingSceneObject.push_back(pSceneObject);

	pDlgObjectLayer->SetHasUnit((CKTDGObject*) pSceneObject );


	return true;
}

bool CX2EventScene::PlaySceneObjectXMesh(EventSceneObject* pEventSceneObject, SceneObectPlay* pSceneObjectPlay)
{

	// play entire

	// oasis907 : 김상윤 [2010.6.10] 
	// 아래의 예제는 Templet 없이 XMeshPlayer의 스킨 메시를 생성하는 예제입니다.
	// 씬 연출 기능에 쓰일 NPC의 X스킨 메시 파일을 XMeshPlayer로 그립니다.
	// XMeshPlayer의 다양한 기능을 줄 수 있는 인자값들을 기존의 GameMajorXMeshPlayer.txt에서 파싱해오지 않고,
	// 추후에 구현될 씬 연출 클래스의 스크립트에서 전달받게 하기위해 작업합니다.

	//// 1. 빈 XMeshTemplet 객체 생성 /////////////////////////////////////////////
	CKTDGXMeshPlayer::XMeshTemplet *newTemplet = new CKTDGXMeshPlayer::XMeshTemplet();


	CSceneObject* pSceneObject = new CSceneObject();


	pSceneObject->m_pXMeshTemplet = newTemplet;

	newTemplet->globalTimeEventList.clear();
	newTemplet->animTimeEventList.clear();
	newTemplet->dieEventList.clear();

	//// 2. 파리미터 설정 //////////////////////////////
	newTemplet->layer = 1;
	newTemplet->lifeType = CKTDGXMeshPlayer::LT_TIME;
	newTemplet->lifeTime = CMinMax<float>(99999, 99999);
	newTemplet->meshType = CKTDGXMeshPlayer::X_SKIN_MESH;
	newTemplet->meshName = pEventSceneObject->m_wstrFileName; //L"UI_NPC_ARIEL.x";
	newTemplet->renderType = CKTDGXRenderer::RT_REAL_COLOR;
	newTemplet->bZEnable = false; // true
	newTemplet->bUseDieEvent = false;
	newTemplet->srcBlend = D3DBLEND_SRCALPHA;
	newTemplet->destBlend = D3DBLEND_INVSRCALPHA;
	newTemplet->aniXETName = pEventSceneObject->m_wstrXETName;

	//// 3. 타임 이벤트 설정 /////////////////////////////////
	// 애니메이션 변경
	CKTDGXMeshPlayer::CXMeshEvent_AniName *pEventAniname = new CKTDGXMeshPlayer::CXMeshEvent_AniName();

	pEventAniname->SetAniName(pSceneObjectPlay->m_wstrAnimName.c_str());
	pEventAniname->SetActualTime(CMinMax<float>(0, 0));
	pEventAniname->SetPlayType(pSceneObjectPlay->m_eAnimPlayType);
	pEventAniname->SetFade(false);
	pEventAniname->SetFinal(false);

	newTemplet->globalTimeEventList.push_back(pEventAniname);


	// 애니메이션 속도
	float fPlaySpeed = pSceneObjectPlay->m_fPlaySpeed;

	CKTDGXMeshPlayer::CXMeshEvent_AniSpeed *pEventAnispeed = new CKTDGXMeshPlayer::CXMeshEvent_AniSpeed();

	pEventAnispeed->SetActualTime(CMinMax<float>(0, 0));
	pEventAnispeed->SetAniSpeed(CMinMax<float> (fPlaySpeed, fPlaySpeed));
	pEventAnispeed->SetFade(false);
	pEventAnispeed->SetFinal(false);

	newTemplet->globalTimeEventList.push_back(pEventAnispeed);

	

	D3DXVECTOR3 ScaleFactor = D3DXVECTOR3(pEventSceneObject->m_Size.x, pEventSceneObject->m_Size.y, 1.f);

	// 사이즈 설정
	CKTDGXMeshPlayer::CXMeshEvent_Size* pEventSize = new CKTDGXMeshPlayer::CXMeshEvent_Size();

	pEventSize->SetSize(CMinMax<D3DXVECTOR3> (ScaleFactor, ScaleFactor) );
	pEventSize->SetActualTime( CMinMax<float>(0, 0) );
	pEventSize->SetFade(false);
	pEventSize->SetFinal(false);

	newTemplet->globalTimeEventList.push_back(pEventSize);


	// Position 설정
	SCENE_OBJECT_POSITION eDrawStartPos = pSceneObjectPlay->m_eStartPosition;
	D3DXVECTOR3 StartPos = pEventSceneObject->m_vecPosSet[eDrawStartPos];

	// SCREEN OUT 기능
	if(eDrawStartPos == CX2EventScene::OP_LEFT_SCREEN_OUT || eDrawStartPos == CX2EventScene::OP_RIGHT_SCREEN_OUT)
	{
		SCENE_OBJECT_POSITION eDrawEndPos = pSceneObjectPlay->m_eMovePosition;
		D3DXVECTOR3 EndPos = pEventSceneObject->m_vecPosSet[eDrawEndPos];
		StartPos.y = EndPos.y;
	}


	float fMoveStartTime = pSceneObjectPlay->m_fMoveStartTime;

	
	CKTDGXMeshPlayer::CXMeshEvent_Position* pEventPosition = new CKTDGXMeshPlayer::CXMeshEvent_Position();

	pEventPosition->SetPosition(CMinMax<D3DXVECTOR3> (StartPos, StartPos) );
	pEventPosition->SetActualTime( CMinMax<float>(fMoveStartTime, fMoveStartTime) );
	pEventPosition->SetFade(false);
	pEventPosition->SetFinal(false);

	newTemplet->globalTimeEventList.push_back(pEventPosition);


	SCENE_OBJECT_POSITION eFinalPos = CX2EventScene::OP_NONE;
	if(pSceneObjectPlay->m_eMovePosition != CX2EventScene::OP_NONE)
	{
		SCENE_OBJECT_POSITION eDrawEndPos = pSceneObjectPlay->m_eMovePosition;
		D3DXVECTOR3 EndPos = pEventSceneObject->m_vecPosSet[eDrawEndPos];

		// SCREEN OUT 기능
		if(eDrawEndPos == CX2EventScene::OP_LEFT_SCREEN_OUT || eDrawEndPos == CX2EventScene::OP_RIGHT_SCREEN_OUT)
		{
			EndPos.y = StartPos.y;
		}

		float fMoveDuration = pSceneObjectPlay->m_fMoveDuration;

		// Position 설정
		CKTDGXMeshPlayer::CXMeshEvent_Position* pEventPosition2 = new CKTDGXMeshPlayer::CXMeshEvent_Position();

		pEventPosition2->SetPosition(CMinMax<D3DXVECTOR3> (EndPos, EndPos) );
		pEventPosition2->SetActualTime( CMinMax<float>(0, fMoveStartTime + fMoveDuration) );
		pEventPosition2->SetFade(true);
		pEventPosition2->SetFinal(false);

		newTemplet->globalTimeEventList.push_back(pEventPosition2);

		eFinalPos = eDrawEndPos;
	}
	else
	{
		eFinalPos = pSceneObjectPlay->m_eStartPosition;
	}

	// Shake 이벤트
	if(pSceneObjectPlay->m_fShakeDuration > 0.f )
	{
		float fShakeDuration = pSceneObjectPlay->m_fShakeDuration;
		float fShakeGapX = pSceneObjectPlay->m_fShakeGap / 2;
		float fShakeGapY = pSceneObjectPlay->m_fShakeGap;

		CKTDGXMeshPlayer::CXMeshEvent_Crash* pEventCrash = new CKTDGXMeshPlayer::CXMeshEvent_Crash();

		pEventCrash->SetFade( true );
		pEventCrash->SetCrash( CMinMax<D3DXVECTOR3>(D3DXVECTOR3(-fShakeGapX,-fShakeGapY,0),D3DXVECTOR3(fShakeGapX,fShakeGapY,0)) );
		pEventCrash->SetFinal(false);

		pEventCrash->SetActualTime( CMinMax<float>(0.0f, fShakeDuration) );

		newTemplet->globalTimeEventList.push_back(pEventCrash);
	}

	//  MirrorVertical
	if(pSceneObjectPlay->m_bMirrorVertical == true)
	{
		CKTDGXMeshPlayer::CXMeshEvent_Rotate* pEventRotate = new CKTDGXMeshPlayer::CXMeshEvent_Rotate();

		pEventRotate->SetRotate(CMinMax<D3DXVECTOR3> (D3DXVECTOR3(0,180,0), D3DXVECTOR3(0,180,0)) );
		pEventRotate->SetActualTime( CMinMax<float>(0, 0) );
		pEventRotate->SetFade( false );
		pEventRotate->SetFinal(false);


		newTemplet->globalTimeEventList.push_back(pEventRotate);
	}

#ifdef EVENT_SCENE_TOOL
	CKTDGXMeshPlayer::CXMeshInstance* newXmesh = m_pXMeshPlayer->CreateInstanceNonTemplet(m_pXMeshPlayer, newTemplet, D3DXVECTOR3(0,0,0), D3DXVECTOR3(0,0,0), D3DXVECTOR3(0,0,0) );
#else
	//// 4. 파티클 생성, 재생 /////////////////////////////////
	CKTDGXMeshPlayer::CXMeshInstance* newXmesh = g_pData->GetUIMajorXMeshPlayer()->CreateInstanceNonTemplet(g_pData->GetUIMajorXMeshPlayer(), newTemplet, D3DXVECTOR3(0,0,0), D3DXVECTOR3(0,0,0), D3DXVECTOR3(0,0,0) );
#endif EVENT_SCENE_TOOL
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    if ( newXmesh == NULL )
    {
        SAFE_DELETE( newTemplet );
        SAFE_DELETE( pSceneObject );
        return false;
    }
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

	newXmesh->SetShowObject(false);




	CKTDGXMeshPlayer::CXMeshInstanceHandle newXmeshHandle = newXmesh->GetHandle();


	// Layer 다이얼로그 생성, 벡터에 등록
	CKTDGUIDialogType pDlgObjectLayer = NULL;
	pDlgObjectLayer	= new CKTDGUIDialog( NULL, NULL );


	pDlgObjectLayer->SetFront( true );
	pDlgObjectLayer->SetLayer(XDL_DIRECTING + 2);
	
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( pDlgObjectLayer );
	pDlgObjectLayer->SetShowEnable( true, true );



#ifdef EVENT_SCENE_TOOL
	pSceneObject->m_pParticleSystem = m_pParticleSystem;
	pSceneObject->m_pXMeshPlayer = m_pXMeshPlayer;
#endif EVENT_SCENE_TOOL

	pSceneObject->m_wstrObjectName = pSceneObjectPlay->m_wstrObjectName;
	pSceneObject->m_eSceneObjectType = OT_XSKINMESH;
#ifdef  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
    pSceneObject->m_hSceneObjectParticleHandle = INVALID_PARTICLE_SEQUENCE_HANDLE;
    pSceneObject->m_hSceneObjectMeshHandle = newXmeshHandle;
#else   X2OPTIMIZE_HANDLE_VALIDITY_CHECK
	pSceneObject->m_iSceneObjectHandle = newXmeshHandle;
#endif  X2OPTIMIZE_HANDLE_VALIDITY_CHECK

	pSceneObject->m_DlgHandle = pDlgObjectLayer;


	pSceneObject->m_eLastPosition = eFinalPos;
	pSceneObject->m_bMirrorVertical = pSceneObjectPlay->m_bMirrorVertical;



	m_listPlayingSceneObject.push_back(pSceneObject);

	pDlgObjectLayer->SetHasUnit((CKTDGObject*) pSceneObject );

	return true;
}


bool CX2EventScene::LoadTextPlay( KLuaManager& luaManager, std::list<TextPlay>& listTextPlay )
{
	int idxTextPlay = 0;
	while( luaManager.BeginTable( "TEXT_PLAY", idxTextPlay ) == true )
	{
		TextPlay kTextPlay;

		//{{ 허상형 : [2010/11/5/] //	에픽 퀘스트 텍스트를 인덱스를사용해 출력할때 사용
#ifdef	EVENT_SCENE_TEXT_USED_INDEX
		LUA_GET_VALUE(	luaManager, "TEXT",	kTextPlay.m_iTextIndex, 	STR_ID_EMPTY);
#else	EVENT_SCENE_TEXT_USED_INDEX
		LUA_GET_VALUE( luaManager, "TEXT",	kTextPlay.m_wstrText,	L"" );	
#endif	EVENT_SCENE_TEXT_USED_INDEX
		//}} 허상형 : [2010/11/5/] //	에픽 퀘스트 텍스트를 인덱스를사용해 출력할때 사용

		int index = 0;
		while( true )
		{
			char key[100] = {0,};
			//wsprintf( key, L"PLAYER_TEXT%d", index );
			StringCchPrintfA( key, 100, "PLAYER_TEXT%d", index );

#ifdef	EVENT_SCENE_TEXT_USED_INDEX
			int iTextIndex;
			if( luaManager.GetValue( key, iTextIndex) == false )
				break;

			kTextPlay.m_PlayerTextIndex.push_back(iTextIndex);
#else	EVENT_SCENE_TEXT_USED_INDEX
			wstring wstrPlayerText;
			if( luaManager.GetValue( key, wstrPlayerText) == false )
				break;

			kTextPlay.m_PlayerText.push_back(wstrPlayerText);
#endif	EVENT_SCENE_TEXT_USED_INDEX
			index++;
		}

		LUA_GET_VALUE( luaManager, "TAG_NAME",	kTextPlay.m_iTagName,	0 );	

		LUA_GET_VALUE( luaManager, "TAG_LEFT", kTextPlay.m_bTagLeft,	true	);

		LUA_GET_VALUE( luaManager, "WAIT", kTextPlay.m_fWaitTime,		-1.0f	);

		LUA_GET_VALUE( luaManager, "SPREAD", kTextPlay.m_bTextSpread,	true	);
		
		LUA_GET_VALUE( luaManager, "SPREAD_SPEED", kTextPlay.m_fTextSpreadSpeed,	0.07f	);

		LUA_GET_VALUE( luaManager, "TEXT_COLOR",	kTextPlay.m_wstrTextColor,	L"#C000000" );	

		LUA_GET_VALUE( luaManager, "HIDE_TAG_NAME", kTextPlay.m_bHideTagName,	false	);

		LUA_GET_VALUE( luaManager, "HIDE_TEXT_BOX", kTextPlay.m_bHideTextBox,	false	);

		LUA_GET_VALUE( luaManager, "USE_TAG_NAME_FORCE", kTextPlay.m_bUseTagNameForce,	false	);

		listTextPlay.push_back(kTextPlay);

		idxTextPlay++;
		luaManager.EndTable();
	}

	return true;
}



bool CX2EventScene::LoadSceneObjectPlay( KLuaManager& luaManager, std::list<SceneObectPlay>& listSceneObectPlay )
{

	int idxSceneObjectPlay = 0;
	while( luaManager.BeginTable( "SCENE_OBJECT_PLAY", idxSceneObjectPlay ) == true )
	{
		SceneObectPlay kSceneObectPlay;


		LUA_GET_VALUE_RETURN( luaManager, "SCENE_OBJECT_NAME", kSceneObectPlay.m_wstrObjectName,	L"",	return false; );	

		LUA_GET_VALUE_ENUM(	luaManager, "START_POSITION", kSceneObectPlay.m_eStartPosition,	SCENE_OBJECT_POSITION,	OP_NONE );


		if( luaManager.BeginTable( "MOVE_POSITION") == true )
		{

			LUA_GET_VALUE_ENUM(	luaManager, 1,	kSceneObectPlay.m_eMovePosition,	SCENE_OBJECT_POSITION,	OP_NONE );

			LUA_GET_VALUE( luaManager, 2, kSceneObectPlay.m_fMoveStartTime,		0.f	);

			LUA_GET_VALUE( luaManager, 3, kSceneObectPlay.m_fMoveDuration,		1.f	);

			luaManager.EndTable();
		}


		LUA_GET_VALUE( luaManager, "ANIM_NAME", kSceneObectPlay.m_wstrAnimName, L"Wait" );

		LUA_GET_VALUE_ENUM ( luaManager, "PLAY_TYPE", kSceneObectPlay.m_eAnimPlayType, CKTDGXSkinAnim::XSKIN_ANIM_PLAYTYPE, CKTDGXSkinAnim::XAP_LOOP );

		LUA_GET_VALUE( luaManager, "PLAY_SPEED", kSceneObectPlay.m_fPlaySpeed, 0.3f );

		if( luaManager.BeginTable( "SHAKE" ) == true )
		{
			LUA_GET_VALUE( luaManager, 1, kSceneObectPlay.m_fShakeGap,		10.f	);
			LUA_GET_VALUE( luaManager, 2, kSceneObectPlay.m_fShakeDuration,		0.5f	);

			luaManager.EndTable();
		}

		LUA_GET_VALUE( luaManager, "MIRROR_VERTICAL", kSceneObectPlay.m_bMirrorVertical,	false	);


		listSceneObectPlay.push_back(kSceneObectPlay);

		idxSceneObjectPlay++;
		luaManager.EndTable();
	}


	return true;
}



bool CX2EventScene::LoadEventSequence( KLuaManager& luaManager, std::list<EventSequece>& listEventSequence )
{

	int idxEventSequence = 0;
	while( luaManager.BeginTable( "EVENT_SEQUENCE", idxEventSequence ) == true )
	{
		EventSequece kEventSequece;


		int idxRemoveObject = 0;
		while( true )
		{
			char key[100] = {0,};
			//wsprintf( key, L"REMOVE_OBJECT%d", idxRemoveObject );
			StringCchPrintfA( key, 100, "REMOVE_OBJECT%d", idxRemoveObject );

			wstring wstrRemoveObjectName;
			if( luaManager.GetValue( key, wstrRemoveObjectName ) == false )
				break;

			kEventSequece.m_vecRemoveObjectName.push_back( wstrRemoveObjectName );

			idxRemoveObject++;
		}


		if( LoadSceneObjectPlay( luaManager, kEventSequece.m_listSceneObectPlay ) == false )
			return false;


		int idxSoundPlay = 0;
		while( luaManager.BeginTable( "SOUND_PLAY", idxSoundPlay ) == true )
		{
			SoundPlayData sndPlayData;

			LUA_GET_VALUE( luaManager, 1, sndPlayData.m_SoundPlayTime,		-1.0f	);
			LUA_GET_VALUE( luaManager, 2, sndPlayData.m_SoundPlayName,		L""		);
			LUA_GET_VALUE( luaManager, 3, sndPlayData.m_SoundPlayRate,		100		);
			LUA_GET_VALUE( luaManager, 4, sndPlayData.m_bOnlyIfMyUnit,		false	);
			LUA_GET_VALUE( luaManager, 5, sndPlayData.m_fMaxSoundDistance,	-1.f	);

			kEventSequece.m_vecSoundPlayData.push_back( sndPlayData );

			idxSoundPlay++;
			luaManager.EndTable();
		}


		if( LoadTextPlay( luaManager, kEventSequece.m_listTextPlay ) == false )
			return false;



		listEventSequence.push_back(kEventSequece);

		idxEventSequence++;
		luaManager.EndTable();
	}

	return true;
}



bool CX2EventScene::AddEventSceneObject_LUA()
{

	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
#ifndef X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
	TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING

	EventSceneObject kEventSceneObject;
	std::map<wstring, EventSceneObject>::iterator mit;

	LUA_GET_VALUE_RETURN(	luaManager, "SCENE_OBJECT_NAME",			kEventSceneObject.m_wstrObjectName,		L"",		goto LoadFail; );	

	LUA_GET_VALUE_RETURN_ENUM(	luaManager, "SCENE_OBJECT_TYPE",		kEventSceneObject.m_eOjectType,	SCENE_OBJECT_TYPE,	OT_TEXTURE,	goto LoadFail; );

	LUA_GET_VALUE_RETURN(	luaManager, "FILE_NAME",			kEventSceneObject.m_wstrFileName,		L"",		goto LoadFail; );	

	LUA_GET_VALUE(	luaManager, "XET_NAME",			kEventSceneObject.m_wstrXETName,		L"" );	

	
	if(kEventSceneObject.m_eOjectType == OT_TEXTURE)
	{


		if( luaManager.BeginTable( "TEXTURE_SIZE" ) == true )
		{
			LUA_GET_VALUE( luaManager, 1, kEventSceneObject.m_Size.x, 256.f );
			LUA_GET_VALUE( luaManager, 2, kEventSceneObject.m_Size.y, 256.f );
			luaManager.EndTable();
		}
		else
		{
			kEventSceneObject.m_Size = D3DXVECTOR2(256.f, 256.f);
		}


		LUA_GET_VALUE( luaManager, "TEXTURE_SIZE ", kEventSceneObject.m_vecPosSet[OP_CENTER_POS].x, 0.0f );
	}
	else
	{
		if( luaManager.BeginTable( "SCALE" ) == true )
		{
			LUA_GET_VALUE( luaManager, 1, kEventSceneObject.m_Size.x, 1.f );
			LUA_GET_VALUE( luaManager, 2, kEventSceneObject.m_Size.y, 1.f );
			luaManager.EndTable();
		}
		else
		{
			kEventSceneObject.m_Size = D3DXVECTOR2(1.f, 1.f);
		}
	}



	if( luaManager.BeginTable( "LEFT_POS" ) == true )
	{
		LUA_GET_VALUE( luaManager, 1, kEventSceneObject.m_vecPosSet[OP_LEFT_POS].x, 0.0f );
		LUA_GET_VALUE( luaManager, 2, kEventSceneObject.m_vecPosSet[OP_LEFT_POS].y, 0.0f );
		LUA_GET_VALUE( luaManager, 3, kEventSceneObject.m_vecPosSet[OP_LEFT_POS].z, 0.0f );
		luaManager.EndTable();
	}
	else
	{
		kEventSceneObject.m_vecPosSet[OP_LEFT_POS] = D3DXVECTOR3(-300.f, -100.f, 0.f);
	}


	if( luaManager.BeginTable( "RIGHT_POS" ) == true )
	{
		LUA_GET_VALUE( luaManager, 1, kEventSceneObject.m_vecPosSet[OP_RIGHT_POS].x, 0.0f );
		LUA_GET_VALUE( luaManager, 2, kEventSceneObject.m_vecPosSet[OP_RIGHT_POS].y, 0.0f );
		LUA_GET_VALUE( luaManager, 3, kEventSceneObject.m_vecPosSet[OP_RIGHT_POS].z, 0.0f );
		luaManager.EndTable();
	}
	else
	{
		kEventSceneObject.m_vecPosSet[OP_RIGHT_POS] = D3DXVECTOR3(300.f, -100.f, 0.f);
	}

	if( luaManager.BeginTable( "CENTER_POS" ) == true )
	{
		LUA_GET_VALUE( luaManager, 1, kEventSceneObject.m_vecPosSet[OP_CENTER_POS].x, 0.0f );
		LUA_GET_VALUE( luaManager, 2, kEventSceneObject.m_vecPosSet[OP_CENTER_POS].y, 0.0f );
		LUA_GET_VALUE( luaManager, 3, kEventSceneObject.m_vecPosSet[OP_CENTER_POS].z, 0.0f );
		luaManager.EndTable();
	}
	else
	{
		kEventSceneObject.m_vecPosSet[OP_CENTER_POS] = D3DXVECTOR3(0.f, -100.f, 0.f);
	}

	if( luaManager.BeginTable( "CUSTOM_POS" ) == true )
	{
		LUA_GET_VALUE( luaManager, 1, kEventSceneObject.m_vecPosSet[OP_CUSTOM_POS].x, 0.0f );
		LUA_GET_VALUE( luaManager, 2, kEventSceneObject.m_vecPosSet[OP_CUSTOM_POS].y, 0.0f );
		LUA_GET_VALUE( luaManager, 3, kEventSceneObject.m_vecPosSet[OP_CUSTOM_POS].z, 0.0f );
		luaManager.EndTable();
	}
	else
	{
		kEventSceneObject.m_vecPosSet[OP_CUSTOM_POS] = D3DXVECTOR3(0.f, 0.f, 0.f);
	}

	LUA_GET_VALUE( luaManager, "LEFT_SCREEN_OUT_POS_X", kEventSceneObject.m_vecPosSet[OP_LEFT_SCREEN_OUT].x, -800.f );
	LUA_GET_VALUE( luaManager, "RIGHT_SCREEN_OUT_POS_X", kEventSceneObject.m_vecPosSet[OP_RIGHT_SCREEN_OUT].x, 800.f );

	// 이미 있는 이벤트 있는데 새로 호출되면 삭제함.
	mit = m_mapEventSceneObject.find( kEventSceneObject.m_wstrObjectName );
	if( mit != m_mapEventSceneObject.end() )
	{
		m_mapEventSceneObject.erase(mit);
	}


	m_mapEventSceneObject.insert(std::make_pair( kEventSceneObject.m_wstrObjectName, kEventSceneObject ));
	return true;

LoadFail:

	return false;
}
bool CX2EventScene::AddEventScene_LUA()
{


	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
#ifndef X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
	TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING

	EventScene kEventScene;
	std::map<wstring, EventScene>::iterator mit;

	LUA_GET_VALUE_RETURN(	luaManager, "EVENT_SCENE_ID",	kEventScene.m_iEventSceneID,	L"",	goto LoadFail; );

	if( LoadEventSequence( luaManager, kEventScene.m_listEventSequence ) == false )
		goto LoadFail;

	// 이미 있는 이벤트 있는데 새로 호출되면 삭제함.
	mit = m_mapEventScene.find( kEventScene.m_iEventSceneID );
	if( mit != m_mapEventScene.end() )
	{
		m_mapEventScene.erase(mit);
	}


	m_mapEventScene.insert(std::make_pair( kEventScene.m_iEventSceneID, kEventScene ));
	return true;


LoadFail:
	wstringstream wstrStream;
	wstrStream << L"EventSceneLoadFail m_iEventSceneID = " << kEventScene.m_iEventSceneID;

	ErrorLogMsg( XEM_ERROR79, wstrStream.str().c_str() );

	return false;
}

bool CX2EventScene::OpenScriptFile( const WCHAR* pFileName )
{
	lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(),  "g_pEventScene", this );
	return g_pKTDXApp->LoadLuaTinker( pFileName );
}

void CX2EventScene::PlayText()
{
	m_fTextPlayElapsedTime = 0.f;
	if(m_iterTE == m_iterES->m_listTextPlay.end())
	{
		// 다음 플레이할 EventSequence
		m_iterES++;
		PlayEventSequence();
	}
	else
	{
		SetTextBoxShow(true);
		SetNameTabShow(true);
		if(m_iterTE->m_bHideTextBox)
		{
			SetTextBoxShow(false);
			return;
		}

		if(m_iterTE->m_bHideTagName)
		{
			SetNameTabShow(false);
		}

		bool bTagLeft		= m_iterTE->m_bTagLeft;
		wstring wstrTextColor	= m_iterTE->m_wstrTextColor;
		bool bTextSpread		= m_iterTE->m_bTextSpread;
		float fTextSpreadSpeed	= m_iterTE->m_fTextSpreadSpeed;

#ifdef	EVENT_SCENE_TEXT_USED_INDEX
		if(m_iterTE->m_PlayerTextIndex.size() != 0)
#else	EVENT_SCENE_TEXT_USED_INDEX
		if(m_iterTE->m_PlayerText.size() != 0)
#endif	EVENT_SCENE_TEXT_USED_INDEX
		{	
			int iTagName = 0;
			// 강제로 TageName사용해주도록 지정하면 TageName사용
			if( true == m_iterTE->m_bUseTagNameForce )
				iTagName = m_iterTE->m_iTagName;
			else
				iTagName = GetPlayerTagName();

			wstring wstrText	= GetPlayerText();
			AddText( bTagLeft, iTagName, wstrText.c_str(), wstrTextColor.c_str(), bTextSpread, fTextSpreadSpeed );
		}
		else
		{
			int iTagName		= m_iterTE->m_iTagName;

			//{{ 허상형 : [2010/11/5/] //	에픽 퀘스트 텍스트를 인덱스를사용해 출력할때 사용
#ifdef	EVENT_SCENE_TEXT_USED_INDEX
			int iTextIndex		= m_iterTE->m_iTextIndex;
			AddText( bTagLeft, iTagName, GET_STRING(iTextIndex), wstrTextColor.c_str(), bTextSpread, fTextSpreadSpeed );
#else	EVENT_SCENE_TEXT_USED_INDEX
			wstring wstrText	= m_iterTE->m_wstrText;
			AddText( bTagLeft, iTagName, wstrText.c_str(), wstrTextColor.c_str(), bTextSpread, fTextSpreadSpeed );
#endif	EVENT_SCENE_TEXT_USED_INDEX
			//}} 허상형 : [2010/11/5/] //	에픽 퀘스트 텍스트를 인덱스를사용해 출력할때 사용
		}
	}
	return;
}

#ifndef REFORM_NOVICE_GUIDE
void CX2EventScene::NoviceGuideTrigger()
{
	if( g_pTFieldGame != NULL && g_pTFieldGame->GetNoviceGuide() != NULL && g_pTFieldGame->GetNoviceGuide()->GetIsPlayGuide() == true )
	{
		if( g_pData != NULL && g_pData->GetQuestManager()->GetUnitCompleteQuest(11000) == false )
		{
			g_pTFieldGame->GetNoviceGuide()->CreateEpicGuideDlg();
		}
	}
	

	return;
}
#endif //REFORM_NOVICE_GUIDE


void CX2EventScene::EndEventScene(bool bIgnoreWaitEventScene )
{
	m_bNowPresent		= false;
	m_TextBoxShow		= false;
	m_NowScenNum		= 0;
	m_fScenTimeBefore	= 0.0f;
	m_fScenTime			= 0.0f;




	std::list<CSceneObject*>::iterator lit;
	for( lit = m_listPlayingSceneObject.begin() ; lit != m_listPlayingSceneObject.end(); lit++)
	{
		CSceneObject *pCSceneObject = (*lit);

		if(pCSceneObject->m_eSceneObjectType == OT_XSKINMESH)
		{
#ifdef  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
#ifdef EVENT_SCENE_TOOL
			m_pXMeshPlayer->DestroyInstanceHandle(pCSceneObject->m_hSceneObjectMeshHandle);
#else
			g_pData->GetUIMajorXMeshPlayer()->DestroyInstanceHandle(pCSceneObject->m_hSceneObjectMeshHandle);
#endif EVENT_SCENE_TOOL
#else   X2OPTIMIZE_HANDLE_VALIDITY_CHECK
#ifdef EVENT_SCENE_TOOL
			m_pXMeshPlayer->DestroyInstanceHandle(pCSceneObject->m_iSceneObjectHandle);
#else
			g_pData->GetUIMajorXMeshPlayer()->DestroyInstanceHandle(pCSceneObject->m_iSceneObjectHandle);
#endif EVENT_SCENE_TOOL
#endif  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
			SAFE_DELETE((CKTDGXMeshPlayer::XMeshTemplet*)pCSceneObject->m_pXMeshTemplet);// XMesh의 경우 임시 템플릿 메모리 해제
		}
		else if(pCSceneObject->m_eSceneObjectType == OT_TEXTURE) 
		{
#ifdef  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
#ifdef EVENT_SCENE_TOOL
			m_pParticleSystem->DestroyInstanceHandle(pCSceneObject->m_hSceneObjectParticleHandle);
#else
			g_pData->GetUIMajorParticle()->DestroyInstanceHandle(pCSceneObject->m_hSceneObjectParticleHandle);
#endif EVENT_SCENE_TOOL
#else   X2OPTIMIZE_HANDLE_VALIDITY_CHECK
#ifdef EVENT_SCENE_TOOL
			m_pParticleSystem->DestroyInstanceHandle(pCSceneObject->m_iSceneObjectHandle);
#else
			g_pData->GetUIMajorParticle()->DestroyInstanceHandle(pCSceneObject->m_iSceneObjectHandle);
#endif EVENT_SCENE_TOOL
#endif  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
		}
		SAFE_DELETE_DIALOG(pCSceneObject->m_DlgHandle);

		SAFE_DELETE(pCSceneObject);
	}
	m_listPlayingSceneObject.clear();

	SetIsPlayingEventScene(false);


	if(!bIgnoreWaitEventScene && m_wstrWaitEventScene.size() != 0)
	{
		m_bWaitDelayEventScene = false; // oasis907 : 김상윤 [2010.7.11] // 
		wstring wstrPlayEventScene = m_wstrWaitEventScene[0];
		m_wstrWaitEventScene.pop_front();
		PlayEventScene(wstrPlayEventScene, true);
	}
	else
	{
		ClearText();
		BlackLayOutMove( false, 0.5f );
		m_pDLGSlideShot->SetShowEnable( m_TextBoxShow, m_TextBoxShow );
		m_wstrWaitEventScene.clear();

#ifndef EVENT_SCENE_TOOL
		CX2State*	pState	= static_cast< CX2State* >( g_pMain->GetNowState() );
		CX2Cursor*	pCursor = pState->GetCursor();
		pCursor->ChangeCursorState( CX2Cursor::XCS_NORMAL );
		m_bSetCursor = false;

#ifndef REFORM_NOVICE_GUIDE
		if(m_pNowPlayingEventScene->m_iEventSceneID == L"11000_01")
		{
			NoviceGuideTrigger();
		}
#endif //REFORM_NOVICE_GUIDE

		if(true == m_bNotifyNewEpicQuest)
		{
			m_bNotifyNewEpicQuest = false;
			if(g_pData->GetUIManager() != NULL &&
				g_pData->GetUIManager()->GetUIQuestNew() != NULL)
			{
				g_pData->GetUIManager()->GetUIQuestNew()->NewEpicStartEffect();
			}

	#ifdef GUIDE_FIRST_EPIC_QUEST
			GuideFirstEpicQuest();
	#endif //GUIDE_FIRST_EPIC_QUEST
		}

#endif EVENT_SCENE_TOOL
	}
	return;
}

int CX2EventScene::GetPlayerTagName()
{
	int iPlayerTagName = 0;
	CX2Unit::UNIT_TYPE ePlayerUnitType;
#ifdef EVENT_SCENE_TOOL
	ePlayerUnitType = CX2Unit::UT_ELSWORD;
#else
	ePlayerUnitType = g_pData->GetMyUser()->GetSelectUnit()->GetType();	
#endif EVENT_SCENE_TOOL

	switch(ePlayerUnitType)
	{
	case CX2Unit::UT_ELSWORD:
		iPlayerTagName = STR_ID_614;
		break;
	case CX2Unit::UT_ARME:
		iPlayerTagName = STR_ID_615;
		break;
	case CX2Unit::UT_LIRE:
		iPlayerTagName = STR_ID_616;
		break;
	case CX2Unit::UT_RAVEN:
		iPlayerTagName = STR_ID_617;
		break;
	case CX2Unit::UT_EVE:
		iPlayerTagName = STR_ID_618;
		break;

//{{ kimhc // 2010.11.24 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
	case CX2Unit::UT_CHUNG:
		iPlayerTagName = STR_ID_9739;
		break;
#endif	NEW_CHARACTER_CHUNG
//}} kimhc // 2010.11.24 //  2010-12-23 New Character CHUNG

#ifdef ARA_CHARACTER_BASE
	case CX2Unit::UT_ARA:
		iPlayerTagName = STR_ID_21181;
		break;
#endif
#ifdef NEW_CHARACTER_EL
	case CX2Unit::UT_ELESIS:
		iPlayerTagName = STR_ID_25873;
		break;
#endif // NEW_CHARACTER_EL
#ifdef SERV_9TH_NEW_CHARACTER // 김태환 ( 캐릭터 추가용 )
	case CX2Unit::UT_ADD:
		iPlayerTagName = STR_ID_29422;
		break;
#endif //SERV_9TH_NEW_CHARACTER
	}
	return iPlayerTagName;
}


wstring CX2EventScene::GetPlayerText()
{
	wstring wstrPlayerText = L"";
	int iPlayerIndex;
#ifdef EVENT_SCENE_TOOL
	iPlayerIndex = (int)CX2Unit::UT_ELSWORD - 1;
#else
	iPlayerIndex = (int)g_pData->GetMyUser()->GetSelectUnit()->GetType() - 1;	
#endif EVENT_SCENE_TOOL
	
	//{{ 허상형 : [2010/11/5/] //	에픽 퀘스트 텍스트를 인덱스를사용해 출력할때 사용
#ifdef	EVENT_SCENE_TEXT_USED_INDEX
	if(iPlayerIndex <= (int)m_iterTE->m_PlayerTextIndex.size() -1)
	{
		return GET_STRING(m_iterTE->m_PlayerTextIndex[iPlayerIndex]);
	}
	else
	{
		return GET_STRING(m_iterTE->m_iTextIndex);
	}
#else	EVENT_SCENE_TEXT_USED_INDEX
	if(iPlayerIndex <= (int)m_iterTE->m_PlayerText.size() -1)
	{
		return m_iterTE->m_PlayerText[iPlayerIndex];
	}
	else
	{
		return m_iterTE->m_wstrText;
	}
#endif	EVENT_SCENE_TEXT_USED_INDEX
	//}} 허상형 : [2010/11/5/] //	에픽 퀘스트 텍스트를 인덱스를사용해 출력할때 사용
	return wstrPlayerText;
}


wstring CX2EventScene::GetPlayerObjectName()
{
	wstring wstrPlayerObjectName = L"";
	int iPlayerIndex;
#ifdef EVENT_SCENE_TOOL
	iPlayerIndex = (int)CX2Unit::UT_ELSWORD - 1;
#else
	iPlayerIndex = (int)g_pData->GetMyUser()->GetSelectUnit()->GetType() - 1;	
#endif EVENT_SCENE_TOOL

	WCHAR wcName[100] = {0,};
	//wsprintf( wcName, L"PLAYER%d", iPlayerIndex );
	StringCchPrintf( wcName, 100, L"PLAYER%d", iPlayerIndex );

	wstrPlayerObjectName = wcName;
	return wstrPlayerObjectName;
}



bool CX2EventScene::PlayEventSequence()
{

	if(m_iterES == m_pNowPlayingEventScene->m_listEventSequence.end())
	{
		EndEventScene();
		return true;
	}


	
	for( UINT i = 0; i<m_iterES->m_vecRemoveObjectName.size(); i++)
	{
		wstring wstrRemoveObjectName = m_iterES->m_vecRemoveObjectName[i];



		std::list<CSceneObject*>::iterator lit;

		for( lit = m_listPlayingSceneObject.begin() ; lit != m_listPlayingSceneObject.end(); lit++)
		{
			CSceneObject *pCSceneObject = (*lit);

			if( pCSceneObject->m_wstrObjectName == wstrRemoveObjectName)
			{
				
				if(pCSceneObject->m_eSceneObjectType == OT_XSKINMESH)
				{
#ifdef  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
#ifdef EVENT_SCENE_TOOL
					m_pXMeshPlayer->DestroyInstanceHandle(pCSceneObject->m_hSceneObjectMeshHandle);
#else
					g_pData->GetUIMajorXMeshPlayer()->DestroyInstanceHandle(pCSceneObject->m_hSceneObjectMeshHandle);
#endif EVENT_SCENE_TOOL
#else   X2OPTIMIZE_HANDLE_VALIDITY_CHECK
#ifdef EVENT_SCENE_TOOL
					m_pXMeshPlayer->DestroyInstanceHandle(pCSceneObject->m_iSceneObjectHandle);
#else
					g_pData->GetUIMajorXMeshPlayer()->DestroyInstanceHandle(pCSceneObject->m_iSceneObjectHandle);
#endif EVENT_SCENE_TOOL
#endif  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
					SAFE_DELETE((CKTDGXMeshPlayer::XMeshTemplet*) pCSceneObject->m_pXMeshTemplet);// XMesh의 경우 임시 템플릿 메모리 해제

				}
				else if(pCSceneObject->m_eSceneObjectType == OT_TEXTURE) 
				{
#ifdef  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
#ifdef EVENT_SCENE_TOOL
					m_pParticleSystem->DestroyInstanceHandle(pCSceneObject->m_hSceneObjectParticleHandle);
#else
					g_pData->GetUIMajorParticle()->DestroyInstanceHandle(pCSceneObject->m_hSceneObjectParticleHandle);
#endif EVENT_SCENE_TOOL
#else   X2OPTIMIZE_HANDLE_VALIDITY_CHECK
#ifdef EVENT_SCENE_TOOL
					m_pParticleSystem->DestroyInstanceHandle(pCSceneObject->m_iSceneObjectHandle);
#else
					g_pData->GetUIMajorParticle()->DestroyInstanceHandle(pCSceneObject->m_iSceneObjectHandle);
#endif EVENT_SCENE_TOOL
#endif  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
				}
				SAFE_DELETE_DIALOG(pCSceneObject->m_DlgHandle);

				SAFE_DELETE(pCSceneObject);

				m_listPlayingSceneObject.erase(lit);

				break;
			}

		}

	}
	


	m_bNowPresent	= true;
	m_fScenTimeBefore	= 0.0f;
	m_fScenTime			= 0.0f;	

	// Text 플레이
	m_iterTE = m_iterES->m_listTextPlay.begin();
	if(m_iterTE != m_iterES->m_listTextPlay.end())
	{
		PlayText();
	}
	else
	{
		SetTextBoxShow( false );
	}



	// 다음 차례의 SceneObject 플레이
	m_iterSO = m_iterES->m_listSceneObectPlay.begin();
	for(; m_iterSO !=  m_iterES->m_listSceneObectPlay.end(); m_iterSO++) // for each Scene Object
	{
		SceneObectPlay* pSceneObjectPlay = &(*m_iterSO);
		SceneObectPlay SceneObjectPlay;
		
		SceneObjectPlay.m_wstrObjectName = pSceneObjectPlay->m_wstrObjectName;
		SceneObjectPlay.m_wstrAnimName = pSceneObjectPlay->m_wstrAnimName;
		SceneObjectPlay.m_bMirrorVertical = pSceneObjectPlay->m_bMirrorVertical;
		SceneObjectPlay.m_eAnimPlayType = pSceneObjectPlay->m_eAnimPlayType;
		SceneObjectPlay.m_eMovePosition = pSceneObjectPlay->m_eMovePosition;
		SceneObjectPlay.m_eStartPosition = pSceneObjectPlay->m_eStartPosition;
		SceneObjectPlay.m_fMoveDuration = pSceneObjectPlay->m_fMoveDuration;
		SceneObjectPlay.m_fMoveStartTime = pSceneObjectPlay->m_fMoveStartTime;
		SceneObjectPlay.m_fPlaySpeed = pSceneObjectPlay->m_fPlaySpeed;
		SceneObjectPlay.m_fShakeDuration = pSceneObjectPlay->m_fShakeDuration;
		SceneObjectPlay.m_fShakeGap = pSceneObjectPlay->m_fShakeGap;


		// XMesh, Particle로 분기
		std::map<wstring, EventSceneObject>::iterator mit;


		wstring wstrObjectName = L"";
		// Player 캐릭터 처리
		if(pSceneObjectPlay->m_wstrObjectName == L"PLAYER")
		{
			wstrObjectName = GetPlayerObjectName();
		}
		else
		{
			wstrObjectName = pSceneObjectPlay->m_wstrObjectName;
		}

		mit = m_mapEventSceneObject.find(wstrObjectName);
		if( mit == m_mapEventSceneObject.end())
		{
			return false;
		}


		EventSceneObject* pEventSceneObject = &(mit->second);

		if(pEventSceneObject == NULL)
			return false;


		// oasis907 : 김상윤 [2010.6.16] // 
		// 같은 이름의 플레이중인 SceneObject가 있는지 체크하여, 
		// 있으면 전의 것을 지우고, 전에 사용했던 기본 파라미터값을 물려받는다.

		std::list<CSceneObject*>::iterator lit;

		for( lit = m_listPlayingSceneObject.begin() ; lit != m_listPlayingSceneObject.end(); lit++)
		{
			CSceneObject *pCSceneObject = (*lit);

			if( pCSceneObject->m_wstrObjectName == SceneObjectPlay.m_wstrObjectName)
			{
				// 기본값일 경우 
				if(SceneObjectPlay.m_eStartPosition == OP_NONE)
				{
					SceneObjectPlay.m_eStartPosition = pCSceneObject->m_eLastPosition;
				}

				if(pCSceneObject->m_eSceneObjectType == OT_XSKINMESH)
				{
#ifdef  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
#ifdef EVENT_SCENE_TOOL
					m_pXMeshPlayer->DestroyInstanceHandle(pCSceneObject->m_hSceneObjectMeshHandle);
#else
					g_pData->GetUIMajorXMeshPlayer()->DestroyInstanceHandle(pCSceneObject->m_hSceneObjectMeshHandle);
#endif EVENT_SCENE_TOOL
					SAFE_DELETE((CKTDGXMeshPlayer::XMeshTemplet*)pCSceneObject->m_pXMeshTemplet);// XMesh의 경우 임시 템플릿 메모리 해제
#else   X2OPTIMIZE_HANDLE_VALIDITY_CHECK
#ifdef EVENT_SCENE_TOOL
					m_pXMeshPlayer->DestroyInstanceHandle(pCSceneObject->m_iSceneObjectHandle);
#else
					g_pData->GetUIMajorXMeshPlayer()->DestroyInstanceHandle(pCSceneObject->m_iSceneObjectHandle);

					SAFE_DELETE((CKTDGXMeshPlayer::XMeshTemplet*)pCSceneObject->m_pXMeshTemplet);// XMesh의 경우 임시 템플릿 메모리 해제
#endif EVENT_SCENE_TOOL
#endif  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
				}
				else if(pCSceneObject->m_eSceneObjectType == OT_TEXTURE) 
				{
#ifdef  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
#ifdef EVENT_SCENE_TOOL
					m_pParticleSystem->DestroyInstanceHandle(pCSceneObject->m_hSceneObjectParticleHandle);
#else
					g_pData->GetUIMajorParticle()->DestroyInstanceHandle(pCSceneObject->m_hSceneObjectParticleHandle);
#endif EVENT_SCENE_TOOL
#else   X2OPTIMIZE_HANDLE_VALIDITY_CHECK
#ifdef EVENT_SCENE_TOOL
					m_pParticleSystem->DestroyInstanceHandle(pCSceneObject->m_iSceneObjectHandle);
#else
					g_pData->GetUIMajorParticle()->DestroyInstanceHandle(pCSceneObject->m_iSceneObjectHandle);
#endif EVENT_SCENE_TOOL
#endif  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
				}
				SAFE_DELETE_DIALOG(pCSceneObject->m_DlgHandle);

				SAFE_DELETE(pCSceneObject);

				m_listPlayingSceneObject.erase(lit);

				break;
			}

		}

		switch(pEventSceneObject->m_eOjectType)
		{
		case CX2EventScene::OT_XSKINMESH:
			PlaySceneObjectXMesh(pEventSceneObject, &SceneObjectPlay);
			break;
		case CX2EventScene::OT_TEXTURE:
			PlaySceneObjectParticle(pEventSceneObject, &SceneObjectPlay);
			break;
		default:
			break;
		}
	}
	return true;
}

bool CX2EventScene::PlayDelayedEventScene(wstring wstrEventSceneID, float fDelayTime)
{
	if(m_wstrWaitEventScene.size() != 0)
	{
		PlayEventScene(wstrEventSceneID);
		return true;
	}

	m_bWaitDelayEventScene = true;
	m_fEventScenePlayDelayTime = fDelayTime;
	m_wstrWaitEventScene.push_back(wstrEventSceneID);
	return true;
}


bool CX2EventScene::PlayEventScene(wstring wstrEventSceneID, bool bWaitEventScene)
{
	map<wstring, EventScene>::iterator iter;
	iter = m_mapEventScene.find(wstrEventSceneID);
	if( iter == m_mapEventScene.end())
	{
		return false;
	}

	if(GetIsPlayingEventScene() == true && bWaitEventScene == false)
	{
		m_wstrWaitEventScene.push_back(wstrEventSceneID);
		return true;
	}
	

	

	SetTextBoxShow( true );

	if(bWaitEventScene == false)
	{
		BlackLayOutMove( true, 0.5f );
	}

	m_pNowPlayingEventScene = &(iter->second);

	
	m_iterES = m_pNowPlayingEventScene->m_listEventSequence.begin();

	PlayEventSequence();

	SetIsPlayingEventScene(true);

#ifndef EVENT_SCENE_TOOL
	if(!m_bSetCursor)
	{
		CX2State*	pState	= static_cast< CX2State* >( g_pMain->GetNowState() );
		CX2Cursor*	pCursor = pState->GetCursor();
		pCursor->ChangeCursorState( CX2Cursor::XCS_EVENT_SCENE );
		m_bSetCursor = true;
	}
#endif EVENT_SCENE_TOOL


#ifdef EVENT_SCENE_TOOL
	CKTDGUIStatic* pStaticSceneID = (CKTDGUIStatic*) m_pDLGSceneID->GetControl( L"SceneID" );
	pStaticSceneID->SetString(0, wstrEventSceneID.c_str());


#endif EVENT_SCENE_TOOL
	

	return true;
}

void CX2EventScene::Clear()
{
	m_mapEventScene.clear();
	m_mapEventSceneObject.clear();
}


bool CX2EventScene::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{

	if(!m_bIsPlayingEventScene)
	{
		return false;
	}

	bool bFlag = false;

	switch( uMsg )
	{
	case WM_LBUTTONUP:
		{

			// 앞에 모달 다이얼로그가 있으면 메시지를 처리하지 않는다
			if( true == g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckFrontModalDlg( m_pDLGSlideShot ) )
				return false;

			if(IsTextSpreading() == true)
			{
				SetTextSpread();
			}
			else
			{
				m_iterTE++;
				PlayText();
			}
			bFlag = true;

		}
		break;
	case WM_LBUTTONDOWN:
		{

			//bFlag = true;
		}
		break;
	case WM_RBUTTONDOWN:
		{
			bFlag = true;
		}
		break;

	case WM_RBUTTONUP:
		{
			EndEventScene(true);
		}
		break;
	}

	return bFlag;
}





void CX2EventScene::SetIsPlayingEventScene( bool bPlaying ) 
{ 
	m_bIsPlayingEventScene = bPlaying; 
}
const bool CX2EventScene::GetIsPlayingEventScene() 
{ 
	return m_bIsPlayingEventScene; 
}



void CX2EventScene::CSceneObject::OnFrameRender_Draw()
{
	if(m_eSceneObjectType == OT_XSKINMESH)
	{
#ifdef  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
#ifdef EVENT_SCENE_TOOL
		CKTDGXMeshPlayer::CXMeshInstance* pXmesh = m_pXMeshPlayer->GetMeshInstance( m_hSceneObjectMeshHandle );
#else
		CKTDGXMeshPlayer::CXMeshInstance* pXmesh = g_pData->GetUIMajorXMeshPlayer()->GetMeshInstance( m_hSceneObjectMeshHandle );
#endif EVENT_SCENE_TOOL
#else   X2OPTIMIZE_HANDLE_VALIDITY_CHECK
#ifdef EVENT_SCENE_TOOL
		CKTDGXMeshPlayer::CXMeshInstance* pXmesh = m_pXMeshPlayer->GetMeshInstance( m_iSceneObjectHandle );
#else
		CKTDGXMeshPlayer::CXMeshInstance* pXmesh = g_pData->GetUIMajorXMeshPlayer()->GetMeshInstance( m_iSceneObjectHandle );
#endif EVENT_SCENE_TOOL
#endif  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
		if(pXmesh != NULL)
		{
			CKTDGStateManager::PushStates( pXmesh->GetRenderStateID() );
			pXmesh->OnFrameRender_Draw();
		}
	}
	else if(m_eSceneObjectType == OT_TEXTURE) 
	{
#ifdef  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
#ifdef EVENT_SCENE_TOOL
		CKTDGParticleSystem::CParticleEventSequence* pSeq = m_pParticleSystem->GetInstanceSequence( m_hSceneObjectParticleHandle );
#else
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSceneObjectParticleHandle );
#endif EVENT_SCENE_TOOL
#else   X2OPTIMIZE_HANDLE_VALIDITY_CHECK
#ifdef EVENT_SCENE_TOOL
		CKTDGParticleSystem::CParticleEventSequence* pSeq = m_pParticleSystem->GetInstanceSequence( m_iSceneObjectHandle );
#else
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_iSceneObjectHandle );
#endif EVENT_SCENE_TOOL
#endif  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
		if(pSeq != NULL)
		{
			CKTDGStateManager::PushStates( pSeq->GetRenderStateID() );
			//CKTDGStateManager::PushRenderState( D3DRS_ZENABLE,	TRUE );
			pSeq->OnFrameRender_Draw();
		}
	}

	return;
}


RENDER_HINT CX2EventScene::CSceneObject::OnFrameRender_Prepare()
{
	RENDER_HINT renderHintResult;
	if(m_eSceneObjectType == OT_XSKINMESH)
	{
#ifdef  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
#ifdef EVENT_SCENE_TOOL
		CKTDGXMeshPlayer::CXMeshInstance* pXmesh = m_pXMeshPlayer->GetMeshInstance( m_hSceneObjectMeshHandle );
#else
		CKTDGXMeshPlayer::CXMeshInstance* pXmesh = g_pData->GetUIMajorXMeshPlayer()->GetMeshInstance( m_hSceneObjectMeshHandle );
#endif EVENT_SCENE_TOOL
#else   X2OPTIMIZE_HANDLE_VALIDITY_CHECK
#ifdef EVENT_SCENE_TOOL
		CKTDGXMeshPlayer::CXMeshInstance* pXmesh = m_pXMeshPlayer->GetMeshInstance( m_iSceneObjectHandle );
#else
		CKTDGXMeshPlayer::CXMeshInstance* pXmesh = g_pData->GetUIMajorXMeshPlayer()->GetMeshInstance( m_iSceneObjectHandle );
#endif EVENT_SCENE_TOOL
#endif  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
		if(pXmesh == NULL)
		{
			renderHintResult = RENDER_HINT::NORENDER;
		}
		else
		{
			renderHintResult = pXmesh->OnFrameRender_Prepare();
		}

	}
	else if(m_eSceneObjectType == OT_TEXTURE) 
	{
#ifdef  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
#ifdef EVENT_SCENE_TOOL
		CKTDGParticleSystem::CParticleEventSequence* pSeq = m_pParticleSystem->GetInstanceSequence( m_hSceneObjectParticleHandle );
#else
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSceneObjectParticleHandle );
#endif EVENT_SCENE_TOOL
#else   X2OPTIMIZE_HANDLE_VALIDITY_CHECK
#ifdef EVENT_SCENE_TOOL
		CKTDGParticleSystem::CParticleEventSequence* pSeq = m_pParticleSystem->GetInstanceSequence( m_iSceneObjectHandle );
#else
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_iSceneObjectHandle );
#endif EVENT_SCENE_TOOL
#endif  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
		if(pSeq == NULL)
		{
			renderHintResult = RENDER_HINT::NORENDER;
		}
		else
		{
			renderHintResult = pSeq->OnFrameRender_Prepare();
		}
	}
	return renderHintResult;
}
//}} oasis907 : 김상윤 [2010.6.17] // 
#endif EVENT_SCENE

#ifdef GUIDE_FIRST_EPIC_QUEST
void CX2EventScene::GuideFirstEpicQuest()
{
	//노아스 호수 입장 가이드
	if(m_pNowPlayingEventScene->m_iEventSceneID == L"11005_01" ||
		m_pNowPlayingEventScene->m_iEventSceneID == L"11000_02")
	{
		g_pData->GetPlayGuide()->SetFirstEpicQuestGuide(CX2PlayGuide::EQGS_VISIT_NOAS);
	}
	//엘의 나무 입장 가이드
	else if(m_pNowPlayingEventScene->m_iEventSceneID == L"11005_02" ||
		m_pNowPlayingEventScene->m_iEventSceneID == L"11010_01")
	{
		g_pData->GetPlayGuide()->SetFirstEpicQuestGuide(CX2PlayGuide::EQGS_ENTER_EL_TREE);
	}
	//던전 입장 가이드(공용)
	else if(m_pNowPlayingEventScene->m_iEventSceneID == L"11010_02"	||
			m_pNowPlayingEventScene->m_iEventSceneID == L"11040_02" ||
			m_pNowPlayingEventScene->m_iEventSceneID == L"11030_01" ||
			m_pNowPlayingEventScene->m_iEventSceneID == L"11050_01")
	{
		g_pData->GetPlayGuide()->SetFirstEpicQuestGuide(CX2PlayGuide::EQGS_ENTER_DUNGEON);
	}
	//도적 뽀루 킬 가이드
	else if(m_pNowPlayingEventScene->m_iEventSceneID == L"11030_02"||
		m_pNowPlayingEventScene->m_iEventSceneID == L"11040_01"||
		m_pNowPlayingEventScene->m_iEventSceneID == L"30120_01")
	{
		g_pData->GetPlayGuide()->SetFirstEpicQuestGuide(CX2PlayGuide::EQGS_KILL_THIEF_PPORU);
	}
	//보르 킬 가이드
	else if(m_pNowPlayingEventScene->m_iEventSceneID == L"11050_02"||
			m_pNowPlayingEventScene->m_iEventSceneID == L"11060_01")
	{
		g_pData->GetPlayGuide()->SetFirstEpicQuestGuide(CX2PlayGuide::EQGS_KILL_BORR);
	}
	else if ( m_pNowPlayingEventScene->m_iEventSceneID == L"11060_02"|| //엘더 마을의 대상인
			m_pNowPlayingEventScene->m_iEventSceneID == L"12000_01")
	{
		g_pData->GetPlayGuide()->SetFirstEpicQuestGuide(CX2PlayGuide::EQGS_QMARK_GUIDE);
	}
}
#endif //GUIDE_FIRST_EPIC_QUEST