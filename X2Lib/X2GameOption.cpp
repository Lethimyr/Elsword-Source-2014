#include "StdAfx.h"
#include ".\x2gameoption.h"

#ifdef X2OPTIMIZE_AUTOSETUP_GAMEOPTION
#include "KBenchmark_SUM.h"
#include "KBenchmark_LINPACK.h"
#include "KBenchmark_MXM.h"
#endif//X2OPTIMIZE_AUTOSETUP_GAMEOPTION

CX2GameOption::CX2GameOption(void):
m_bChangeFieldOption(false)
{
	//{{ JHKang // 2010.8.27 // 창 모드 전체 화면 구현
#ifdef TOGGLE_WINDOW_FULLSCREEN
	HWND	hwnd = g_pKTDXApp->GetHWND();	// 윈도우 핸들 얻기
	GetWindowRect(hwnd, &m_WindowPosition);	// 윈도우 좌표 얻기
#endif TOGGLE_WINDOW_FULLSCREEN
	//}} JHKang // 2010.8.27 // 창 모드 전체 화면 구현

#ifdef ADD_MUSIC_SUB_VOLUME
	m_fMusicSubVolume = 0.f;
#endif

#ifdef PVP_ZOOM_CAMERA
	ResetAutoCamera();
#endif

	m_bFriendOld		= false;
	m_bPartyOld			= false;
	m_eFieldLevelOld	= FL_HIGH;
}

CX2GameOption::~CX2GameOption(void)
{
}

bool CX2GameOption::SetAutoOption( int index )
{
	if( index < 0 || index >= (int)m_vecOptionListPreset.size() )
		return false;

	OptionList& optionList = m_vecOptionListPreset[index];
	m_OptionList.m_iAutoSetLevel = index;

	SetUnitDetail( optionList.m_UnitDetail );

	SetMapDetail( optionList.m_MapDetail );
#ifdef X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
	SetEffectDetail( optionList.m_eEffect );
#else//X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
	SetEffectDetail( optionList.m_bEffect );
#endif//X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION


	SetFieldDetail( optionList.m_FieldDetail );


	switch( g_pMain->GetNowStateID() )
	{
	case CX2Main::XS_DUNGEON_GAME:
	case CX2Main::XS_PVP_GAME:
	case CX2Main::XS_TRAINING_GAME:
		{
		} break;

	default:
		{
			SetTexDetail( optionList.m_TexDetail );
		} break;
	}


	return true;
}

bool CX2GameOption::UpAutoOption()
{
	return SetAutoOption( m_OptionList.m_iAutoSetLevel+1 );
}

bool CX2GameOption::DownAutoOption()
{
	return SetAutoOption( m_OptionList.m_iAutoSetLevel-1 );
}

void CX2GameOption::SetTexDetail( OptionLevel optionLevel, bool bForce /*= false*/ )
{
	if( optionLevel < m_OptionList.m_MaxTexDetail )
		optionLevel = m_OptionList.m_MaxTexDetail;

	if( false == bForce )
	{
		if( m_OptionList.m_TexDetail == optionLevel )
			return;
	}


	m_OptionList.m_TexDetail = optionLevel;

	switch( m_OptionList.m_TexDetail )
	{
	case OL_HIGH:
		{
			g_pKTDXApp->GetDeviceManager()->SetTextureDetailLevel( CKTDXDeviceTexture::DL_HIGH );
		}
		break;

	case OL_MEDIUM:
		{
			g_pKTDXApp->GetDeviceManager()->SetTextureDetailLevel( CKTDXDeviceTexture::DL_MEDIUM );
		}
		break;

	case OL_LOW:
		{
			g_pKTDXApp->GetDeviceManager()->SetTextureDetailLevel( CKTDXDeviceTexture::DL_LOW );
		}
		break;
	}
}

void CX2GameOption::UpTexDetail()
{
	if( m_OptionList.m_TexDetail == OL_HIGH )
		return;

	switch( m_OptionList.m_TexDetail )
	{
	case OL_MEDIUM:
		{
			SetTexDetail( OL_HIGH );
		}
		break;

	case OL_LOW:
		{
			SetTexDetail( OL_MEDIUM );
		}
		break;
	}
}

void CX2GameOption::DownTexDetail()
{
	if( m_OptionList.m_TexDetail == OL_LOW )
		return;

	switch( m_OptionList.m_TexDetail )
	{
	case OL_HIGH:
		{
			SetTexDetail( OL_MEDIUM );
		}
		break;

	case OL_MEDIUM:
		{
			SetTexDetail( OL_LOW );
		}
		break;
	}
}

void CX2GameOption::SetUnitDetail( OptionLevel optionLevel, bool bForce /*= false*/  )
{

	if( m_OptionList.m_MaxUnitDetail == OL_HIGH )
	{
		// depth buffer format이 16bit이면 unit의 외곽선을 그리지 않도록~
		DXUTDeviceSettings deviceSettings = DXUTGetDeviceSettings();
		if( deviceSettings.pp.AutoDepthStencilFormat == D3DFMT_D16_LOCKABLE ||
			deviceSettings.pp.AutoDepthStencilFormat == D3DFMT_D16 ||
			deviceSettings.pp.AutoDepthStencilFormat == D3DFMT_D15S1 )
		{
			m_OptionList.m_MaxUnitDetail = OL_MEDIUM;
		}
	}

	if( optionLevel < m_OptionList.m_MaxUnitDetail )
		optionLevel = m_OptionList.m_MaxUnitDetail;

	if( false == bForce )
	{
		if( m_OptionList.m_UnitDetail == optionLevel )
			return;
	}

	m_OptionList.m_UnitDetail = optionLevel;
}

void CX2GameOption::UpUnitDetail()
{
	if( m_OptionList.m_UnitDetail == OL_HIGH )
		return;

	switch( m_OptionList.m_UnitDetail )
	{
	case OL_MEDIUM:
		{
			SetUnitDetail( OL_HIGH );
		}
		break;

	case OL_LOW:
		{
			SetUnitDetail( OL_MEDIUM );
		}
		break;
	}
}

void CX2GameOption::DownUnitDetail()
{
	if( m_OptionList.m_UnitDetail == OL_LOW )
		return;

	switch( m_OptionList.m_UnitDetail )
	{
	case OL_HIGH:
		{
			SetUnitDetail( OL_MEDIUM );
		}
		break;

	case OL_MEDIUM:
		{
			SetUnitDetail( OL_LOW );
		}
		break;
	}
}

void CX2GameOption::SetMapDetail( OptionLevel optionLevel, bool bForce /*= false*/ )
{
	if( optionLevel < m_OptionList.m_MaxMapDetail )
		optionLevel = m_OptionList.m_MaxMapDetail;

	if( false == bForce )
	{
		if( m_OptionList.m_MapDetail == optionLevel )
			return;
	}

	m_OptionList.m_MapDetail = optionLevel;

	if( g_pX2Game != NULL )
	{
		if( g_pX2Game->GetWorld() != NULL )
		{
			g_pX2Game->GetWorld()->SetMapDetail( m_OptionList.m_MapDetail );
		}
	}
	if( g_pSquareGame != NULL )
	{
		if( g_pSquareGame->GetWorld() != NULL )
		{
			g_pSquareGame->GetWorld()->SetMapDetail( m_OptionList.m_MapDetail );
		}
	}
	else if(g_pTFieldGame != NULL)
	{
		if( g_pTFieldGame->GetWorld() != NULL )
		{
			g_pTFieldGame->GetWorld()->SetMapDetail( m_OptionList.m_MapDetail );
		}
	}	
}

void CX2GameOption::UpMapDetail()
{
	if( m_OptionList.m_MapDetail == OL_HIGH )
		return;

	switch( m_OptionList.m_MapDetail )
	{
	case OL_MEDIUM:
		{
			SetMapDetail( OL_HIGH );
		}
		break;

	case OL_LOW:
		{
			SetMapDetail( OL_MEDIUM );
		}
		break;
	}
}

void CX2GameOption::DownMapDetail()
{
	if( m_OptionList.m_MapDetail == OL_LOW )
		return;

	switch( m_OptionList.m_MapDetail )
	{
	case OL_HIGH:
		{
			SetMapDetail( OL_MEDIUM );
		}
		break;

	case OL_MEDIUM:
		{
			SetMapDetail( OL_LOW );
		}
		break;
	}
}

#ifdef X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
void CX2GameOption::SetEffectDetail( OptionLevel optionLevel, bool bForce /*= false*/ )
#else//X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
void CX2GameOption::SetEffectDetail( bool bEffect, bool bForce /*= false*/ )
#endif//X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
{
	if( false == bForce )
	{
#ifdef X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
		if( m_OptionList.m_eEffect == optionLevel )
#else//X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
		if( m_OptionList.m_bEffect == bEffect )
#endif//X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
			return;
	}

#ifdef X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
	m_OptionList.m_eEffect = optionLevel;
#else//X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
	m_OptionList.m_bEffect = bEffect;
#endif//X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION

#ifdef X2OPTIMIZE_GAMEOPTION_BUGFIX
	if( g_pData != NULL )
	{
		if( g_pData->GetGameMinorParticle() != NULL )
#ifdef X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
			g_pData->GetGameMinorParticle()->SetEnable( m_OptionList.m_eEffect != OL_LOW );
#else//X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
			g_pData->GetGameMinorParticle()->SetEnable( m_OptionList.m_bEffect );
#endif//X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION

		if( g_pData->GetGameMinorXMeshPlayer() != NULL )
		{
#ifdef X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
			g_pData->GetGameMinorXMeshPlayer()->SetEnable( m_OptionList.m_eEffect != OL_LOW );
			g_pData->GetGameMinorXMeshPlayer()->SetEnableSlashTrace( m_OptionList.m_eEffect != OL_LOW );
#else//X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
			g_pData->GetGameMinorXMeshPlayer()->SetEnable( m_OptionList.m_bEffect );
			g_pData->GetGameMinorXMeshPlayer()->SetEnableSlashTrace( m_OptionList.m_bEffect );
#endif//X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
		}

        if ( g_pX2Game != NULL )
        {
		    for( int i = 0; i < (int) g_pX2Game->GetUnitNum(); i++ )
		    {
			    CX2GameUnit* pCX2GameUnit = g_pX2Game->GetUnit(i);
			    //if( true == g_pX2Game->IsValidUnit( pCX2GameUnit ) )
			    if ( NULL != pCX2GameUnit )
			    {
				    pCX2GameUnit->InitEffect();
#ifdef  X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
                    if( m_OptionList.m_eEffect == OL_LOW )
#else   X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
				    if( m_OptionList.m_bEffect == false )
#endif  X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
				    {
					    pCX2GameUnit->DeleteMinorParticle();
				    }
			    }
		    }
        }
//#ifdef X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
//		//if( g_pData->GetGameMajorXMeshPlayer() != NULL )
//			//g_pData->GetGameMajorXMeshPlayer()->SetEnableSlashTrace( m_OptionList.m_eEffect != OL_LOW );
//#else//X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
//		if( g_pData->GetGameMajorXMeshPlayer() != NULL )
//			g_pData->GetGameMajorXMeshPlayer()->SetEnableSlashTrace( m_OptionList.m_bEffect );
//#endif//X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
	}
#else//X2OPTIMIZE_GAMEOPTION_BUGFIX
	if( g_pX2Game != NULL )
	{
		if( g_pX2Game->GetMinorParticle() != NULL )
		{
			g_pX2Game->GetMinorParticle()->SetEnable( m_OptionList.m_bEffect );
		}
		if( g_pX2Game->GetMinorXMeshPlayer() != NULL )
		{
			g_pX2Game->GetMinorXMeshPlayer()->SetEnable( m_OptionList.m_bEffect );
		}
		for( int i = 0; i < (int) g_pX2Game->GetUnitNum(); i++ )
		{
			CX2GameUnit* pCX2GameUnit = g_pX2Game->GetUnit(i);

			//if( true == g_pX2Game->IsValidUnit( pCX2GameUnit ) )
			if ( NULL != pCX2GameUnit )
			{
				pCX2GameUnit->InitEffect();
				if( m_OptionList.m_bEffect == false )
				{
					pCX2GameUnit->DeleteMinorParticle();
				}
			}
		}

		if( NULL != g_pX2Game->GetMajorXMeshPlayer() )
		{
			g_pX2Game->GetMajorXMeshPlayer()->SetEnableSlashTrace( m_OptionList.m_bEffect );
		}

		if( NULL != g_pX2Game->GetMinorXMeshPlayer() )
		{
			g_pX2Game->GetMinorXMeshPlayer()->SetEnableSlashTrace( m_OptionList.m_bEffect );
		}
		
	}
#endif//X2OPTIMIZE_GAMEOPTION_BUGFIX
}

void CX2GameOption::UpEffectDetail()
{
#ifdef X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
	if( m_OptionList.m_eEffect == OL_HIGH )
		return;

	switch( m_OptionList.m_eEffect )
	{
	case OL_MEDIUM:
		{
			SetEffectDetail( OL_HIGH );
		}
		break;

	case OL_LOW:
		{
			SetEffectDetail( OL_MEDIUM );
		}
		break;
	}
#else//X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
	if( m_OptionList.m_bEffect == true )
		return;

	SetEffectDetail( true );
#endif//X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
}

void CX2GameOption::DownEffectDetail()
{
#ifdef X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
	if( m_OptionList.m_eEffect == OL_LOW )
		return;

	switch( m_OptionList.m_eEffect )
	{
	case OL_HIGH:
		{
			SetEffectDetail( OL_MEDIUM );
		}
		break;

	case OL_MEDIUM:
		{
			SetEffectDetail( OL_LOW );
		}
		break;
	}
#else//X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
	if( m_OptionList.m_MapDetail == false )
		return;

	SetEffectDetail( false );
#endif//X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
}

void CX2GameOption::SetFieldDetail( FieldLevel optionLevel, bool bForce /*= false*/ )
{
	if( optionLevel < m_OptionList.m_MaxFieldDetail )
		optionLevel = m_OptionList.m_MaxFieldDetail;

	if( false == bForce )
	{
		if( m_OptionList.m_FieldDetail == optionLevel )
			return;
	}

	m_OptionList.m_FieldDetail = optionLevel;

	// 서버로 변경된 필드옵션을 보낸다.
	SendFieldOption();
	m_bChangeFieldOption = true;	
}

void CX2GameOption::UpFieldDetail()
{
	if( m_OptionList.m_FieldDetail == OL_HIGH )
		return;

	switch( m_OptionList.m_FieldDetail )
	{
	case FL_MEDIUM:
		{
			SetFieldDetail( FL_HIGH );
		}
		break;

	case FL_LOW:
		{
			SetFieldDetail( FL_MEDIUM );
		}
		break;	
	default:
		break;
	}
}

void CX2GameOption::DownFieldDetail()
{
	if( m_OptionList.m_FieldDetail == OL_LOW )
		return;

#ifdef X2OPTIMIZE_AUTOSETUP_GAMEOPTION
	switch( m_OptionList.m_FieldDetail )
#else//X2OPTIMIZE_AUTOSETUP_GAMEOPTION
	switch( m_OptionList.m_MapDetail )
#endif//X2OPTIMIZE_AUTOSETUP_GAMEOPTION
	{
	case FL_HIGH:
		{
			SetFieldDetail( FL_MEDIUM );
		}
		break;

	case FL_MEDIUM:
		{
			SetFieldDetail( FL_LOW );
		}
		break;
	default:
		break;
	}
}

void CX2GameOption::SetFieldParty( bool bParty )
{
	m_OptionList.m_bParty = bParty;

	// 서버로 변경된 옵션설정을 보낸다.
	//SendFieldOption();
	m_bChangeFieldOption = true;

	

#ifdef ADDED_RELATIONSHIP_SYSTEM
	if ( NULL != g_pData->GetRelationshipEffectManager() )
	{
		if ( TRUE == GetFieldSD() )
		{
			g_pData->GetRelationshipEffectManager()->SetShowRelationshipAttachEffect ( CX2RelationshipEffectManager::REST_HIDE_ALL );	
		}
		else
		{
			if ( TRUE == GetFieldParty() )
				g_pData->GetRelationshipEffectManager()->SetShowRelationshipAttachEffect( CX2RelationshipEffectManager::REST_HIDE_NOT_PARTY_ONLY );
			else
				g_pData->GetRelationshipEffectManager()->SetShowRelationshipAttachEffect( CX2RelationshipEffectManager::REST_SHOW_ALL );
		}
	}
#endif // ADDED_RELATIONSHIP_SYSTEM	
}

void CX2GameOption::SetFieldFriend( bool bFriend )
{
	//m_OptionList.m_bFriend = bFriend;

	// 서버로 변경된 옵션설정을 보낸다.
	//SendFieldOption();
	m_bChangeFieldOption = true;
}

void CX2GameOption::SetFieldSD( bool bSD )
{
	m_OptionList.m_bSD = bSD;

#ifdef ADDED_RELATIONSHIP_SYSTEM
	if ( NULL != g_pData->GetRelationshipEffectManager() )
	{
		if ( TRUE == GetFieldSD() )
		{
			g_pData->GetRelationshipEffectManager()->SetShowRelationshipAttachEffect ( CX2RelationshipEffectManager::REST_HIDE_ALL );	
		}
		else
		{
			if ( TRUE == GetFieldParty() )
				g_pData->GetRelationshipEffectManager()->SetShowRelationshipAttachEffect( CX2RelationshipEffectManager::REST_SHOW_PARTY_ONLY );
			else
				g_pData->GetRelationshipEffectManager()->SetShowRelationshipAttachEffect( CX2RelationshipEffectManager::REST_SHOW_ALL );
		}
	}
#endif // ADDED_RELATIONSHIP_SYSTEM	
}
void CX2GameOption::SetEnable3DSound( bool bEnable3DSound )
{
	m_OptionList.m_bEnable3DSound = bEnable3DSound;
	if( g_pKTDXApp->GetDSManager() != NULL &&
		g_pKTDXApp->GetDSManager()->GetInit() == true )
	{
		g_pKTDXApp->GetDSManager()->SetEnable3DSound( bEnable3DSound );
	}
}

void CX2GameOption::SetMusic( bool bMusic )
{
	m_OptionList.m_bMusic = bMusic;
	if( g_pKTDXApp->GetDSManager() != NULL && 
		g_pKTDXApp->GetDSManager()->GetMP3Play() != NULL && 
		g_pKTDXApp->GetDSManager()->GetInit() == true )
	{
		g_pKTDXApp->GetDSManager()->GetMP3Play()->SetPaused( !bMusic );
	}
}

void CX2GameOption::SetSound( bool bSound )
{
	m_OptionList.m_bSound = bSound;
	g_pKTDXApp->GetDSManager()->SetSoundMute( !bSound );


	if( NULL != g_pX2Game &&
		NULL != g_pX2Game->GetWorld() )
	{
		g_pX2Game->GetWorld()->PlayEffectBGM( bSound );
		g_pX2Game->GetWorld()->Play3DEffectBGM( bSound );
	}

	if( NULL != g_pTFieldGame &&
		NULL != g_pTFieldGame->GetWorld() )
	{
		g_pTFieldGame->GetWorld()->PlayEffectBGM( bSound );
		g_pTFieldGame->GetWorld()->Play3DEffectBGM( bSound );
	}

	//if( NULL != g_pSquareGame &&
	//	NULL != g_pSquareGame->GetWorld() )
	//{
	//	g_pSquareGame->GetWorld()->PlayEffectBGM( bSound );
	//}
}

void CX2GameOption::SetMusicVolume( float fMusicVolume )
{
#ifdef ADD_MUSIC_SUB_VOLUME
	if( fMusicVolume >= 0.f )
		fMusicVolume = -1.f;

	float fNewMusicVolume = fMusicVolume;
	fNewMusicVolume += m_fMusicSubVolume;
	if( fNewMusicVolume >= 0.f )
		fNewMusicVolume = -1.f;
	if( fNewMusicVolume < -3000.f )
		fNewMusicVolume = -3000.f;

	m_OptionList.m_fMusicVolume = fMusicVolume;
	if( g_pKTDXApp->GetDSManager() != NULL && g_pKTDXApp->GetDSManager()->GetMP3Play() != NULL && g_pKTDXApp->GetDSManager()->GetInit() == true )
		g_pKTDXApp->GetDSManager()->GetMP3Play()->SetVolume( (int)fNewMusicVolume );
#else

	if( fMusicVolume >= 0.f )
		fMusicVolume = -1.f;

	m_OptionList.m_fMusicVolume = fMusicVolume;
	if( g_pKTDXApp->GetDSManager() != NULL && g_pKTDXApp->GetDSManager()->GetMP3Play() != NULL && g_pKTDXApp->GetDSManager()->GetInit() == true )
		g_pKTDXApp->GetDSManager()->GetMP3Play()->SetVolume( (int)fMusicVolume );

#endif	
}

void CX2GameOption::SetSoundVolume( float fSoundVolume )
{
	m_OptionList.m_fSoundVolume = fSoundVolume;
	g_pKTDXApp->GetDSManager()->SetSoundVolume( fSoundVolume );
}

void CX2GameOption::SetResolution( DWORD dwWidth, DWORD dwHeight, int iColorBit /*=32*/ )
{
#ifdef ACTIVE_KOG_GAME_PERFORMANCE_CHECK
	KOGGamePerformanceCheck::GetInstance()->Pause();
#endif//ACTIVE_KOG_GAME_PERFORMANCE_CHECK

	float fNear = g_pKTDXApp->GetDGManager()->GetNear();
	float fFar = g_pKTDXApp->GetDGManager()->GetFar();
	float fPerspective = g_pKTDXApp->GetDGManager()->GetPerspectiveValue();
	bool bPerspective = g_pKTDXApp->GetDGManager()->GetPerspective();

	//{{ JHKang // 2010.8.30 // 창 모드 전체 화면 구현
#ifdef TOGGLE_WINDOW_FULLSCREEN
	m_OptionList.m_vResolution.x = (float)dwWidth;
	m_OptionList.m_vResolution.y = (float)dwHeight;

	if (m_OptionList.m_bFullScreen)
		return;

	ToggleWindowFullScreen(true);

#else
	DXUTDeviceSettings deviceSettings	= DXUTGetDeviceSettings();

	deviceSettings.pp.BackBufferWidth	= dwWidth;
	deviceSettings.pp.BackBufferHeight	= dwHeight;

	if( iColorBit == 16 )
	{
		deviceSettings.AdapterFormat		= D3DFMT_R5G6B5;
		deviceSettings.pp.BackBufferFormat	= D3DFMT_R5G6B5;
	}	

	if( deviceSettings.pp.MultiSampleType != D3DMULTISAMPLE_NONE )
	{	
		deviceSettings.pp.Flags &= ~D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
	}

	if( S_OK == DXUTCreateDeviceFromSettings( &deviceSettings ) )
	{
		DXUTSetFullscreenClientRectAtModeChange( (int)dwWidth, (int)dwHeight );
		DXUTSetWindowClientRectAtModeChange( (int)dwWidth, (int)dwHeight );

		m_OptionList.m_vResolution.x = (float)dwWidth;
		m_OptionList.m_vResolution.y = (float)dwHeight;
	}
	else
	{
		// 해상도 변경에 실패하면 게임옵션파일에 default 값으로 저장한다
		m_OptionList.m_vResolution = D3DXVECTOR2( 1024, 768 );
		SaveScriptFile();
	}
#endif TOGGLE_WINDOW_FULLSCREEN
	//{{ JHKang // 2010.8.30 // 창 모드 전체 화면 구현

	g_pKTDXApp->GetDGManager()->SetProjection( fNear, fFar, bPerspective );
	g_pKTDXApp->GetDGManager()->SetPerspectiveValue(fPerspective);	

#ifdef ACTIVE_KOG_GAME_PERFORMANCE_CHECK
	KOGGamePerformanceCheck::GetInstance()->Resume();
#endif//ACTIVE_KOG_GAME_PERFORMANCE_CHECK

#ifdef  SERV_KTDX_OPTIMIZE_NEW_UDP_CONNECTION_STRATEGY
    if ( g_pData->GetGameUDP() != NULL )
        g_pData->GetGameUDP()->RemoveAllPendingPingSends();
#endif  SERV_KTDX_OPTIMIZE_NEW_UDP_CONNECTION_STRATEGY
}

bool CX2GameOption::OpenScriptFile()
{
#ifdef  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
    const WCHAR pFileName[256] = L"GameOption.txt";			// X2/dat 폴더에 있는 default script
#else   X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
	const WCHAR pFileName[256] = L"GameOption.lua";			// X2/dat 폴더에 있는 default script
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
	const WCHAR pSaveFileName[256] = L"GameOptions.lua";	// 사용자가 설정을 바꾸고 게임을 끝냈을때 저장되는 script

	// gameoptions.lua 파일을 읽어서 UTF-8 포맷이 아니면 변환한다
#ifndef X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
	ConvertFileAnsiToUTF8( "GameOptions.lua", "GameOptions.lua" );
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING


	bool bDoneParsingUserGameOptionFile = true;

#ifdef  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING

    KLuaManagerProxy luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
    {
        if ( g_pKTDXApp->LoadAndDoMemory_LocalFile( &luaManager, pSaveFileName ) == false )
            bDoneParsingUserGameOptionFile = false;
        if ( bDoneParsingUserGameOptionFile == false )
        {
            if ( g_pKTDXApp->LoadAndDoMemory( &luaManager, pFileName ) == false )
            {
                return false;
            }
        }
    }

#else   X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING

    KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState(), 0, true );

	if( false == g_pKTDXApp->LoadLuaTinker_LocalFile( pSaveFileName ) )
		bDoneParsingUserGameOptionFile = false;

	if( true == bDoneParsingUserGameOptionFile )
	{
		if( false == g_pKTDXApp->LoadAndDoMemory_LocalFile( &luaManager, pSaveFileName ) )
			bDoneParsingUserGameOptionFile = false;
	}

	if( false == bDoneParsingUserGameOptionFile )
	{
		if( false == g_pKTDXApp->LoadLuaTinker( pFileName ) )
			return false;
		if( false == g_pKTDXApp->LoadAndDoMemory( &luaManager, pFileName  ) )
			return false;
	}

#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING


	if( true == luaManager.BeginTable( "GAME_OPTION_CURRENT" ) )
	{
		LUA_GET_VALUE( luaManager, "AUTOSET_LEVEL",				m_OptionList.m_iAutoSetLevel,		3 );
		LUA_GET_VALUE_ENUM( luaManager, "GRAPHIC_UNIT", 			m_OptionList.m_UnitDetail,			OptionLevel,		OL_HIGH	);
		LUA_GET_VALUE_ENUM( luaManager, "GRAPHIC_TEXTURE",			m_OptionList.m_TexDetail,			OptionLevel,		OL_HIGH	);
		LUA_GET_VALUE_ENUM( luaManager, "GRAPHIC_MAP",				m_OptionList.m_MapDetail,			OptionLevel,		OL_HIGH	);
#ifdef X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
		//기존의 GRAPHIC_EFFECT를 GRAPHIC_EFFECT_VER2로 변경 시도
		bool bEffect;
		if( luaManager.GetValue( "GRAPHIC_EFFECT", bEffect ) == true ) 
			m_OptionList.m_eEffect = ( bEffect ) ? CX2GameOption::OL_HIGH : CX2GameOption::OL_LOW;
		else
			LUA_GET_VALUE_ENUM( luaManager, "GRAPHIC_EFFECT_VER2",	m_OptionList.m_eEffect,		OptionLevel,		OL_HIGH	);
#else//X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
		LUA_GET_VALUE( luaManager, "GRAPHIC_EFFECT",				m_OptionList.m_bEffect,				true );
#endif//X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
		LUA_GET_VALUE( luaManager, "RESOLUTION_X",					m_OptionList.m_vResolution.x,		1024 );
		LUA_GET_VALUE( luaManager, "RESOLUTION_Y",					m_OptionList.m_vResolution.y,		768 );
#ifdef OPTIMIZED_DEFAULT_RESOLUTION
		if( false == bDoneParsingUserGameOptionFile )
			m_OptionList.m_vResolution = D3DXVECTOR2( GetSystemMetrics( SM_CXSCREEN ), GetSystemMetrics( SM_CYSCREEN ) );
#endif OPTIMIZED_DEFAULT_RESOLUTION

		LUA_GET_VALUE( luaManager, "ENABLE_3D_SOUND",				m_OptionList.m_bEnable3DSound,		true );
		LUA_GET_VALUE( luaManager, "SOUND",						m_OptionList.m_bSound,				true );
		LUA_GET_VALUE( luaManager, "MUSIC",						m_OptionList.m_bMusic,				true );
		LUA_GET_VALUE( luaManager, "SOUND_VOLUME",					m_OptionList.m_fSoundVolume,		-800.f );
		LUA_GET_VALUE( luaManager, "MUSIC_VOLUME",					m_OptionList.m_fMusicVolume,		-2000.f );

		LUA_GET_VALUE( luaManager, "DYNAMIC_CAMERA",				m_OptionList.m_bDynamicCamera,		true );
		LUA_GET_VALUE( luaManager, "CAMERA_ZOOM_LEVEL",			m_OptionList.m_iZoomLevel,			0 );

		LUA_GET_VALUE_ENUM( luaManager, "FIELD_LEVEL",				m_OptionList.m_FieldDetail,			FieldLevel,			FL_HIGH );
		LUA_GET_VALUE( luaManager, "FIELD_PARTY",					m_OptionList.m_bParty,				false );
		LUA_GET_VALUE( luaManager, "FIELD_SD",						m_OptionList.m_bSD,					false );


		LUA_GET_VALUE( luaManager, "CHARINFO_LEVEL",				m_OptionList.m_bLevel,				true );
		LUA_GET_VALUE( luaManager, "CHARINFO_PVP",					m_OptionList.m_bPvpRank,			true );
		LUA_GET_VALUE( luaManager, "CHARINFO_GUILD",				m_OptionList.m_bGuild,				true );
		LUA_GET_VALUE( luaManager, "CHARINFO_NOTHING",				m_OptionList.m_bNothing,			true );
#ifdef SERV_LOCAL_RANKING_SYSTEM
		LUA_GET_VALUE( luaManager, "RANKUP_UI_SHOW",				m_OptionList.m_bShowRankUpInDungeon,true );
#endif //SERV_LOCAL_RANKING_SYSTEM
		//LUA_GET_VALUE( luaManager, "FIELD_FRIEND",					m_OptionList.m_bFriend,				true );

#ifdef OPTIMIZED_DEFAULT_RESOLUTION
		LUA_GET_VALUE( luaManager, "FULLSCREEN",					m_OptionList.m_bFullScreen,			false );
#else OPTIMIZED_DEFAULT_RESOLUTION
		LUA_GET_VALUE( luaManager, "FULLSCREEN",					m_OptionList.m_bFullScreen,			true );
#endif OPTIMIZED_DEFAULT_RESOLUTION

#ifdef VERTICAL_SYNC_OPTION
		LUA_GET_VALUE( luaManager, "FLASHEFFECT",					m_OptionList.m_bFlashEffect,		false );
#endif

		//{{ kimhc // 2010.3.12 //	채팅창 개편
#ifdef	CHAT_WINDOW_IMPROV
		LUA_GET_VALUE( luaManager, "DISPLAY_OBTAINING_ED",			m_OptionList.m_bDisplayObtainingED,		true );
		LUA_GET_VALUE( luaManager, "DISPLAY_OBTAINING_EXP",		m_OptionList.m_bDisplayObtainingEXP,	true );
#endif	CHAT_WINDOW_IMPROV
		//}} kimhc // 2010.3.12 //	채팅창 개편

#ifdef SERV_KEY_MAPPING_INT
		LUA_GET_VALUE( luaManager, "JOY_ENABLE",					m_OptionList.m_bJoyEnable,			false );
#endif // SERV_KEY_MAPPING_INT

#ifdef KEY_MAPPING_INT_IN_KEYBOARD_SETTING
		// 옵션 셋팅과 무관하게 조이스틱 켬
		m_OptionList.m_bJoyEnable = true;
#endif KEY_MAPPING_INT_IN_KEYBOARD_SETTING

#ifdef VERTICAL_SYNC_OPTION
		LUA_GET_VALUE( luaManager, "VERTICALSYNC",					m_OptionList.m_bVerticalSync,		false );
#endif
		LUA_GET_VALUE_ENUM( luaManager, "SKILL_CUTIN_SHOW", 		m_OptionList.m_eSkillCutinShowState,	SKILL_CUTIN_SHOW_STATE,		SCSS_ONESELF );

#ifdef SKILL_SLOT_UI_TYPE_B
		LUA_GET_VALUE( luaManager, "SKILL_SLOT_UI_TYPE_A",			m_OptionList.m_bIsSkillUITypeA,		true );
#endif //SKILL_SLOT_UI_TYPE_B

#ifdef SERV_PVP_NEW_SYSTEM
		m_OptionList.m_vecPvpMap.clear();
		if( luaManager.BeginTable( "PVP_MAP" ) == true )
		{  
#ifdef LIMIT_PVP_MAPSETTING
			for( int tableIndex = 1, iPvpMapId = -1; tableIndex <= 2 ; ++tableIndex )
			{
				if ( luaManager.GetValue( tableIndex, iPvpMapId ) == true )
					m_OptionList.m_vecPvpMap.push_back( iPvpMapId );
			}				
#else
			int tableIndex = 1;
			int iPvpMapId = -1;
			while( luaManager.GetValue( tableIndex, iPvpMapId ) == true )
			{
				m_OptionList.m_vecPvpMap.push_back( iPvpMapId );
				++tableIndex;
			}
#endif LIMIT_PVP_MAPSETTING

			luaManager.EndTable();
		}
#endif

#ifdef RULE_AGREEMENT
		LUA_GET_VALUE( luaManager, "HACKSHIELD_RULE_AGREE",		m_OptionList.m_bRuleAgree,	false );
#ifdef NEW_RULE_AGREEMENT
		LUA_GET_VALUE( luaManager, "NEW_HACKSHIELD_RULE_AGREE",	m_OptionList.m_bNewRuleAgree, false );
#endif NEW_RULE_AGREEMENT
#endif RULE_AGREEMENT
		//{{ 2011.9.16 이준호 반디캠 작업
#ifdef BANDICAM_RECORDING
		LUA_GET_VALUE( luaManager, "GENERAL_VIDEO_TYPE",		m_OptionList.m_GeneralVideoType,	0 );

		/*
		사용자 세팅을 저장하지 않으므로 패스
		LUA_GET_VALUE( luaManager, "VIDEO_SIZE_W",		m_OptionList.m_VideoSizeW,	0 );
		LUA_GET_VALUE( luaManager, "VIDEO_SIZE_H",		m_OptionList.m_VideoSizeH,	0 );
		LUA_GET_VALUE( luaManager, "VIDEO_FPS",		m_OptionList.m_VideoFPS,	0 );
		LUA_GET_VALUE( luaManager, "VIDEO_QUAILTY",		m_OptionList.m_VideoQuality,	0 );
		LUA_GET_VALUE( luaManager, "VIDEO_CHANNELS",		m_OptionList.m_AudioChannels,	0 );
		LUA_GET_VALUE( luaManager, "VIDEO_SAMPLERATE",		m_OptionList.m_AudioSampleRate,	0 );
		*/
#endif BANDICAM_RECORDING

#ifdef SERVER_GROUP_ID_IN_GAME_OPTION
		int iServerGroupID = 0;
		LUA_GET_VALUE( luaManager, "SERVER_GROUP_ID",		iServerGroupID,	0 );
		g_pInstanceData->SetServerGroupID( iServerGroupID );
#endif // SERVER_GROUP_ID_IN_GAME_OPTION

#ifdef PLAYER_ID_IN_GAME_OPTION
		LUA_GET_VALUE( luaManager, "LOGIN_ID",		m_OptionList.m_wstrSavedLoginID,	L"" );
#endif // PLAYER_ID_IN_GAME_OPTION

		luaManager.EndTable();
	}

#ifdef X2OPTIMIZE_AUTOSETUP_GAMEOPTION
	if( false == bDoneParsingUserGameOptionFile )
	{
		//"최적의 게임 옵션을 자동으로 설정하려면 "예"를 그렇지 않으면 "아니요"를 눌러주세요." 메세지 출력
		if( MessageBoxW( g_pKTDXApp->GetHWND(), GET_STRING( STR_ID_29006 ), GET_STRING( STR_ID_29042 ), MB_YESNO | MB_TOPMOST ) == IDYES )
		{
			_DecideProperGraphicOption();
		}
	}
#endif//X2OPTIMIZE_AUTOSETUP_GAMEOPTION

	m_eFieldLevelOld = m_OptionList.m_FieldDetail;
	m_bPartyOld = m_OptionList.m_bParty;
	//m_bFriendOld = m_OptionList.m_bFriend;

#ifdef X2OPTIMIZE_AUTOSETUP_GAMEOPTION
	//시스템 메모리가 256 이하이거나 비디오 램이 64 이하이면 
	//텍스쳐, 이펙트 설정을 최하로 낮추고 해상도를 800/600 이하로 낮춘다.
	DWORD totalTextureMem		= 0;
	DWORD availableTextureMem	= 0;
	g_pKTDXApp->GetVideoRam( totalTextureMem, availableTextureMem );
	totalTextureMem = totalTextureMem/1024/1024;

	DWORD totalPhysicMem		= 0;
	DWORD availablePhysicMem	= 0;
	g_pKTDXApp->GetPhysicRam( totalPhysicMem, availablePhysicMem );
	totalPhysicMem = totalPhysicMem/1024/1024;

	if( totalTextureMem <= 60 || totalPhysicMem < 300 )
	{
		// 비디오 메모리가 일정 크기 이하이면 unit설정을 medium을 최대로
		m_OptionList.m_MaxUnitDetail	= OL_MEDIUM;
		// 텍스쳐 설정도 최하로 바꾸도록 하자
		m_OptionList.m_MaxTexDetail		= OL_MEDIUM;
		m_OptionList.m_MaxMapDetail		= OL_MEDIUM;
		m_OptionList.m_MaxFieldDetail	= FL_MEDIUM;
	}

	// depth buffer format이 16bit이면 unit의 외곽선을 그리지 않도록~	
	DXUTDeviceSettings deviceSettings = DXUTGetDeviceSettings();
	if( deviceSettings.pp.AutoDepthStencilFormat == D3DFMT_D16_LOCKABLE ||
		deviceSettings.pp.AutoDepthStencilFormat == D3DFMT_D16 ||
		deviceSettings.pp.AutoDepthStencilFormat == D3DFMT_D15S1 )
	{
		m_OptionList.m_MaxUnitDetail	= OL_MEDIUM;
	}
#else//X2OPTIMIZE_AUTOSETUP_GAMEOPTION
	//시스템 메모리가 256 이하이거나 비디오 램이 64 이하이면 
	//텍스쳐, 이펙트 설정을 최하로 낮추고 해상도를 800/600 이하로 낮춘다.
	DWORD totalTextureMem		= 0;
	DWORD availableTextureMem	= 0;
	g_pKTDXApp->GetVideoRam( totalTextureMem, availableTextureMem );
	totalTextureMem = totalTextureMem/1024/1024;

	DWORD totalPhysicMem		= 0;
	DWORD availablePhysicMem	= 0;
	g_pKTDXApp->GetPhysicRam( totalPhysicMem, availablePhysicMem );
	totalPhysicMem = totalPhysicMem/1024/1024;

	if( totalTextureMem <= 60 || totalPhysicMem < 300 )
	{
		// 비디오 메모리가 일정 크기 이하이면 unit설정을 medium을 최대로
		m_OptionList.m_MaxUnitDetail	= OL_MEDIUM;
		m_OptionList.m_UnitDetail		= OL_MEDIUM;

		// 텍스쳐 설정도 최하로 바꾸도록 하자
		m_OptionList.m_MaxTexDetail		= OL_MEDIUM;
		m_OptionList.m_TexDetail		= OL_LOW;

		m_OptionList.m_MaxMapDetail		= OL_MEDIUM;
		m_OptionList.m_MapDetail		= OL_MEDIUM;

		m_OptionList.m_MaxFieldDetail	= FL_MEDIUM;
		m_OptionList.m_FieldDetail		= FL_LOW;
		m_OptionList.m_bParty			= false;
		m_OptionList.m_bSD				= false;

		// 이펙트 끔
#ifdef X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
		m_OptionList.m_eEffect = OL_LOW;
#else//X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
		m_OptionList.m_bEffect = false;
#endif//X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION

		//해상도 800 600
		m_OptionList.m_vResolution.x = 800;
		m_OptionList.m_vResolution.y = 600;

		// 캐릭터정보 끔
		m_OptionList.m_bLevel = false;
		m_OptionList.m_bPvpRank = false;
		m_OptionList.m_bGuild = false;
		m_OptionList.m_bNothing = false;
	}


	if( totalPhysicMem <= 512 )
	{
		g_pKTDXApp->GetDSManager()->SetCapable3DSound( false );
	}

	// depth buffer format이 16bit이면 unit의 외곽선을 그리지 않도록~	
	DXUTDeviceSettings deviceSettings = DXUTGetDeviceSettings();
	if( deviceSettings.pp.AutoDepthStencilFormat == D3DFMT_D16_LOCKABLE ||
		deviceSettings.pp.AutoDepthStencilFormat == D3DFMT_D16 ||
		deviceSettings.pp.AutoDepthStencilFormat == D3DFMT_D15S1 )
	{
		m_OptionList.m_MaxUnitDetail	= OL_MEDIUM;
		m_OptionList.m_UnitDetail		= OL_MEDIUM;
	}
#endif//X2OPTIMIZE_AUTOSETUP_GAMEOPTION

	//ApplyAllOption();

#ifdef  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
    const WCHAR pPresetFileName[256] = L"GameOptionPreset.txt";
#else   X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
	const WCHAR pPresetFileName[256] = L"GameOptionPreset.lua";
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING

//{{ robobeg : 2008-10-28
#ifdef  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
    {
        if ( g_pKTDXApp->LoadAndDoMemory( &luaManager, pPresetFileName ) == false )
        {
            return false;
        }
    }
#else   X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
	if( false == g_pKTDXApp->LoadLuaTinker( pPresetFileName ) )
		return false;
	//if( false == g_pKTDXApp->LoadAndDoMemory( &luaManager, L"Enum.lua" ) )
		//return false;
	if( false == g_pKTDXApp->LoadAndDoMemory( &luaManager, pPresetFileName ) )
		return false;
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
//}} robobeg : 2008-10-28

	m_vecOptionListPreset.clear();
	int iTableIndex = 0;
	while( true == luaManager.BeginTable( "GAME_OPTION_PRESET", iTableIndex ) )
	{
		OptionList optionList;
		LUA_GET_VALUE( luaManager, "AUTOSET_LEVEL",				optionList.m_iAutoSetLevel,		3 );
		LUA_GET_VALUE_ENUM( luaManager, "GRAPHIC_UNIT", 			optionList.m_UnitDetail,		OptionLevel,		OL_HIGH	);
		LUA_GET_VALUE_ENUM( luaManager, "GRAPHIC_TEXTURE",			optionList.m_TexDetail,			OptionLevel,		OL_HIGH	);
		LUA_GET_VALUE_ENUM( luaManager, "GRAPHIC_MAP",				optionList.m_MapDetail,			OptionLevel,		OL_HIGH	);
#ifdef X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
		LUA_GET_VALUE_ENUM( luaManager, "GRAPHIC_EFFECT_VER2",	optionList.m_eEffect,			OptionLevel,		OL_HIGH );
#else//X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
		LUA_GET_VALUE( luaManager, "GRAPHIC_EFFECT",				optionList.m_bEffect,			true );		
#endif//X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION

		//LUA_GET_VALUE( luaManager, "RESOLUTION_X",					optionList.m_vResolution.x,		640 );
		//LUA_GET_VALUE( luaManager, "RESOLUTION_Y",					optionList.m_vResolution.y,		480 );

		m_vecOptionListPreset.push_back( optionList );

		luaManager.EndTable();
		iTableIndex++;
	}

	

	return true;
}




bool CX2GameOption::SaveScriptFile()
{
#ifdef X2TOOL
	return true;
#endif

	const string strFileName = "GameOptions.lua";


	FILE* file = NULL;
	file = fopen( strFileName.c_str(), "w" );		
	if( NULL == file )
	{
		ErrorLogMsg( XEM_ERROR7, strFileName.c_str() );
		return false;
	}

    fputc( 0xEF, file );
    fputc( 0xBB, file );
    fputc( 0xBF, file );

	fwprintf( file, L"  \n" );
	fwprintf( file, L"  \n" );
	fwprintf( file, L"GAME_OPTION_CURRENT = \n" );
	fwprintf( file, L"{ \n" );


	fwprintf( file, L"\t-- GRAPHIC OPTION \n" );
	fwprintf( file, L"\tAUTOSET_LEVEL		= %d, \n", m_OptionList.m_iAutoSetLevel );

	switch( m_OptionList.m_UnitDetail )
	{
	case OL_HIGH:
		fwprintf( file, L"\tGRAPHIC_UNIT		= OPTION_LEVEL[\"OL_HIGH\"], \n" );
		break;
	case OL_MEDIUM:
		fwprintf( file, L"\tGRAPHIC_UNIT		= OPTION_LEVEL[\"OL_MEDIUM\"], \n" );
		break;
	case OL_LOW:
		fwprintf( file, L"\tGRAPHIC_UNIT		= OPTION_LEVEL[\"OL_LOW\"], \n" );
		break;
	}

	switch( m_OptionList.m_TexDetail )
	{
	case OL_HIGH:
		fwprintf( file, L"\tGRAPHIC_TEXTURE		= OPTION_LEVEL[\"OL_HIGH\"], \n" );
		break;
	case OL_MEDIUM:
		fwprintf( file, L"\tGRAPHIC_TEXTURE		= OPTION_LEVEL[\"OL_MEDIUM\"], \n" );
		break;
	case OL_LOW:
		fwprintf( file, L"\tGRAPHIC_TEXTURE		= OPTION_LEVEL[\"OL_LOW\"], \n" );
		break;
	}

	switch( m_OptionList.m_MapDetail )
	{
	case OL_HIGH:
		fwprintf( file, L"\tGRAPHIC_MAP			= OPTION_LEVEL[\"OL_HIGH\"], \n" );
		break;
	case OL_MEDIUM:
		fwprintf( file, L"\tGRAPHIC_MAP			= OPTION_LEVEL[\"OL_MEDIUM\"], \n" );
		break;
	case OL_LOW:
		fwprintf( file, L"\tGRAPHIC_MAP			= OPTION_LEVEL[\"OL_LOW\"], \n" );
		break;
	}

#ifdef X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
	switch( m_OptionList.m_eEffect )
	{
		case OL_HIGH:
			fwprintf( file, L"\tGRAPHIC_EFFECT_VER2	= OPTION_LEVEL[\"OL_HIGH\"], \n" );
			break;
		case OL_MEDIUM:
			fwprintf( file, L"\tGRAPHIC_EFFECT_VER2	= OPTION_LEVEL[\"OL_MEDIUM\"], \n" );
			break;
		case OL_LOW:
			fwprintf( file, L"\tGRAPHIC_EFFECT_VER2	= OPTION_LEVEL[\"OL_LOW\"], \n" );
			break;
	}
#else//X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
	if( true == m_OptionList.m_bEffect )
		fwprintf( file, L"\tGRAPHIC_EFFECT		= TRUE, \n" );
	else
		fwprintf( file, L"\tGRAPHIC_EFFECT		= FALSE, \n" );
#endif//X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION


	fwprintf( file, L"\tRESOLUTION_X		= %d, \n", (int)m_OptionList.m_vResolution.x );
	fwprintf( file, L"\tRESOLUTION_Y		= %d, \n", (int)m_OptionList.m_vResolution.y );

	//{{ kimhc // 2010.3.12 //	채팅창 개편
#ifdef	CHAT_WINDOW_IMPROV
	if( true == m_OptionList.m_bFullScreen )
		fwprintf( file, L"\tFULLSCREEN			= TRUE, \n" );
	else
		fwprintf( file, L"\tFULLSCREEN			= FALSE, \n" );
#endif	CHAT_WINDOW_IMPROV
	//}} kimhc // 2010.3.12 //	채팅창 개편

#ifdef SERV_KEY_MAPPING_INT
	if( true == m_OptionList.m_bJoyEnable )
		fwprintf( file, L"\tJOY_ENABLE			= TRUE, \n" );
	else
		fwprintf( file, L"\tJOY_ENABLE			= FALSE, \n" );	
#endif // SERV_KEY_MAPPING_INT

#ifdef VERTICAL_SYNC_OPTION
	if ( true == m_OptionList.m_bFlashEffect )
		fwprintf( file, L"\tFLASHEFFECT			= TRUE, \n" );
	else
		fwprintf( file, L"\tFLASHEFFECT			= FALSE, \n" );
#endif


	fwprintf( file, L"\n\n\n" );
	fwprintf( file, L"\t-- SOUND OPTION \n" );


	if( true == m_OptionList.m_bSound )
		fwprintf( file, L"\tSOUND			= TRUE, \n" );
	else
		fwprintf( file, L"\tSOUND			= FALSE, \n" );

	if( true == m_OptionList.m_bMusic )
		fwprintf( file, L"\tMUSIC			= TRUE, \n" );
	else
		fwprintf( file, L"\tMUSIC			= FALSE, \n" );



	if( true == m_OptionList.m_bEnable3DSound )
		fwprintf( file, L"\tENABLE_3D_SOUND			= TRUE, \n" );
	else
		fwprintf( file, L"\tENABLE_3D_SOUND			= FALSE, \n" );




	fwprintf( file, L"\tSOUND_VOLUME		= %d, \n", (int)m_OptionList.m_fSoundVolume );
	fwprintf( file, L"\tMUSIC_VOLUME		= %d, \n", (int)m_OptionList.m_fMusicVolume );




	fwprintf( file, L"\n\n\n" );
	fwprintf( file, L"\t-- OTHER OPTION \n" );


//{{ kimhc // 2010.3.12 //	채팅창 개편
#ifndef	CHAT_WINDOW_IMPROV
	if( true == m_OptionList.m_bFullScreen )
		fwprintf( file, L"\tFULLSCREEN			= TRUE, \n" );
	else
		fwprintf( file, L"\tFULLSCREEN			= FALSE, \n" );
#endif	CHAT_WINDOW_IMPROV
//}} kimhc // 2010.3.12 //	채팅창 개편
	

	if( true == m_OptionList.m_bDynamicCamera )
		fwprintf( file, L"\tDYNAMIC_CAMERA		= TRUE, \n" );
	else
		fwprintf( file, L"\tDYNAMIC_CAMERA		= FALSE, \n" );

	fwprintf( file, L"\tCAMERA_ZOOM_LEVEL		= %d, \n", m_OptionList.m_iZoomLevel );


#ifdef VERTICAL_SYNC_OPTION
	if ( true == m_OptionList.m_bVerticalSync )
		fwprintf( file, L"\tVERTICALSYNC			= TRUE, \n" );
	else
		fwprintf( file, L"\tVERTICALSYNC			= FALSE, \n" );
#endif


	switch( m_OptionList.m_eSkillCutinShowState )
	{
	case SCSS_ONESELF:
		fwprintf( file, L"\tSKILL_CUTIN_SHOW		= SKILL_CUTIN_SHOW_STATE[\"SCSS_ONESELF\"], \n" );
		break;
	case SCSS_PARTY_MEMBER:
		fwprintf( file, L"\tSKILL_CUTIN_SHOW		= SKILL_CUTIN_SHOW_STATE[\"SCSS_PARTY_MEMBER\"], \n" );
		break;
	case SCSS_HIDE:
		fwprintf( file, L"\tSKILL_CUTIN_SHOW		= SKILL_CUTIN_SHOW_STATE[\"SCSS_HIDE\"], \n" );
		break;
	}

#ifdef SKILL_SLOT_UI_TYPE_B
	if( true == m_OptionList.m_bIsSkillUITypeA )
		fwprintf( file, L"\tSKILL_SLOT_UI_TYPE_A		= TRUE, \n" );
	else
		fwprintf( file, L"\tSKILL_SLOT_UI_TYPE_A		= FALSE, \n" );
#endif //SKILL_SLOT_UI_TYPE_B

#ifdef RULE_AGREEMENT
	// 2011.3.27 조효진 서비스 동의 창(동의 한번하면 이후에는 뜨지않음)
	fwprintf( file, L"\n\n" );
	fwprintf( file, L"\t-- HACKSHIELD RULL AGREE OPTION \n" );
	if( true == GetRuleAgree() )
		fwprintf( file, L"\tHACKSHIELD_RULE_AGREE			= TRUE, \n" );
	else
		fwprintf( file, L"\tHACKSHIELD_RULE_AGREE			= FALSE, \n" );
#ifdef NEW_RULE_AGREEMENT
	// 2011.5.30 임홍락 서비스 추가 동의 창(동의 한번하면 이후에는 뜨지않음)
	if( true == GetNewRuleAgree() )
		fwprintf( file, L"\tNEW_HACKSHIELD_RULE_AGREE		= TRUE, \n" );
	else
		fwprintf( file, L"\tNEW_HACKSHIELD_RULE_AGREE		= FALSE, \n" );
#endif NEW_RULE_AGREEMENT
#endif RULE_AGREEMENT

	//{{ 2011.9.16 이준호 반디캠 작업
#ifdef BANDICAM_RECORDING
	fwprintf( file, L"\n\n" );
	fwprintf( file, L"\t-- RECORD VIDEO AUTO OPTION \n" );
	fwprintf( file, L"\tGENERAL_VIDEO_TYPE		= %d, \n",GetGeneralVideoType() );
#endif BANDICAM_RECORDING
	//}}


	fwprintf( file, L"\n\n\n" );
	fwprintf( file, L"\t-- FIELD OPTION \n" );
	switch( m_OptionList.m_FieldDetail )
	{
	case FL_HIGH:
		fwprintf( file, L"\tFIELD_LEVEL			= FIELD_LEVEL[\"FL_HIGH\"], \n" );
		break;
	case FL_MEDIUM:
		fwprintf( file, L"\tFIELD_LEVEL			= FIELD_LEVEL[\"FL_MEDIUM\"], \n" );
		break;
	case FL_LOW:
		fwprintf( file, L"\tFIELD_LEVEL			= FIELD_LEVEL[\"FL_LOW\"], \n" );
		break;	
	}
	fwprintf( file, L"\tFIELD_PARTY		= %d, \n", m_OptionList.m_bParty );
	fwprintf( file, L"\tFIELD_SD		= %d, \n", m_OptionList.m_bSD );
	//fwprintf( file, L"\tFIELD_FRIEND		= %d, \n", m_OptionList.m_bFriend );		
		
	// 캐릭터 정보 저장
	fwprintf( file, L"\tCHARINFO_LEVEL		= %d, \n", m_OptionList.m_bLevel );
	fwprintf( file, L"\tCHARINFO_PVP		= %d, \n", m_OptionList.m_bPvpRank );
	fwprintf( file, L"\tCHARINFO_GUILD		= %d, \n", m_OptionList.m_bGuild );
	fwprintf( file, L"\tCHARINFO_NOTHING	= %d, \n", m_OptionList.m_bNothing );
#ifdef SERV_LOCAL_RANKING_SYSTEM
	fwprintf( file, L"\tRANKUP_UI_SHOW		= %d, \n", m_OptionList.m_bShowRankUpInDungeon );
#endif //SERV_LOCAL_RANKING_SYSTEM

#ifdef SERV_PVP_NEW_SYSTEM
	// 대전 맵 설정
	if( m_OptionList.m_vecPvpMap.size() > 0 )
	{
		fwprintf( file, L"\tPVP_MAP = { " );
		for(unsigned int i=0; i<m_OptionList.m_vecPvpMap.size(); ++i)
		{
			fwprintf( file, L"%d, ", m_OptionList.m_vecPvpMap[i] );
		}
		fwprintf( file, L"},\n" );
	}	
#endif

#ifdef SERVER_GROUP_ID_IN_GAME_OPTION
	fwprintf( file, L"\tSERVER_GROUP_ID		= %d, \n", g_pInstanceData->GetServerGroupID() );
#endif // SERVER_GROUP_ID_IN_GAME_OPTION

#ifdef PLAYER_ID_IN_GAME_OPTION
	fwprintf( file, L"\tLOGIN_ID		= \"%s\", \n", m_OptionList.m_wstrSavedLoginID.c_str() );
#endif // PLAYER_ID_IN_GAME_OPTION

	fwprintf( file, L"} \n" );
	
	fclose(file);

	return true;
}

void CX2GameOption::ApplyAllOption( bool bForce /*= false*/ )
{
#ifdef VERTICAL_SYNC_OPTION
	SetVerticalSync( m_OptionList.m_bVerticalSync );
#endif

	// GRAPHIC
	SetUnitDetail( m_OptionList.m_UnitDetail, bForce );
	SetTexDetail( m_OptionList.m_TexDetail, bForce );
	SetMapDetail( m_OptionList.m_MapDetail, bForce );
#ifdef X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
	SetEffectDetail( m_OptionList.m_eEffect, bForce);
#else//X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
	SetEffectDetail( m_OptionList.m_bEffect, bForce);
#endif//X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
	SetFieldDetail( m_OptionList.m_FieldDetail, bForce );

	SetResolution( (DWORD)m_OptionList.m_vResolution.x, (DWORD)m_OptionList.m_vResolution.y );

#ifdef VERTICAL_SYNC_OPTION
	SetFlashEffect( m_OptionList.m_bFlashEffect );
#endif

	// SOUND
	SetSound( m_OptionList.m_bSound );
	SetMusic( m_OptionList.m_bMusic );
	SetSoundVolume( m_OptionList.m_fSoundVolume );
	SetMusicVolume( m_OptionList.m_fMusicVolume );
	SetEnable3DSound( m_OptionList.m_bEnable3DSound );

	// OTHER
	SetDynamicCamera( m_OptionList.m_bDynamicCamera );
	SetPlayGuide( m_OptionList.m_bPlayGuide, false );
	CameraZoomIn( 0 );
	SetFullScreen( m_OptionList.m_bFullScreen );
	SetStateCutinShowState(m_OptionList.m_eSkillCutinShowState);
#ifdef SKILL_SLOT_UI_TYPE_B
	SetIsSkillUITypeA(m_OptionList.m_bIsSkillUITypeA);
#endif //SKILL_SLOT_UI_TYPE_B

#ifdef SERV_KEY_MAPPING_INT
	SetJoyEnable( m_OptionList.m_bJoyEnable );
#endif // SERV_KEY_MAPPING_INT
}



void CX2GameOption::SetPlayGuide( bool bGuide, bool bUpdateDB /* = true */ )
{
	if( m_OptionList.m_bPlayGuide == bGuide )
		return; 

	m_OptionList.m_bPlayGuide = bGuide; 

	if( bUpdateDB == true )
	{
		if( g_pMain != NULL )
		{
			CX2State* pNowState = (CX2State*) g_pMain->GetNowState();
			if( pNowState != NULL )
			{
				pNowState->Handler_EGS_CHANGE_OPTION_PLAY_GUIDE_REQ();
			}
		}
	}
}

void CX2GameOption::SendFieldOption()
{
	if( m_OptionList.m_FieldDetail == m_eFieldLevelOld && 
		m_OptionList.m_bParty == m_bPartyOld )		
		return;

	m_eFieldLevelOld = m_OptionList.m_FieldDetail;
	m_bPartyOld = m_OptionList.m_bParty;

	if( g_pMain != NULL )
	{
		CX2State* pNowState = (CX2State*) g_pMain->GetNowState();
		if( pNowState != NULL )
		{
			pNowState->Handler_EGS_OPTION_UPDATE_REQ();
		}
	}
}



void CX2GameOption::SetRefuseFriend(CX2GameOption::BlackListDenyState val)
{
	switch(val)
	{
	case BL_ON:
		SetRefuseFriend(true);
		break;
	case BL_OFF:
	default:
		SetRefuseFriend(false);	
		break;
	}	
}

void CX2GameOption::SetRefuseParty(CX2GameOption::BlackListDenyState val)
{
	switch(val)
	{
	case BL_ON:
		SetRefuseParty(true);
		break;
	case BL_OFF:
	default:
		SetRefuseParty(false);	
		break;
	}
}

void CX2GameOption::SetRefusePersonalTrade(CX2GameOption::BlackListDenyState val)
{
	switch(val)
	{
	case BL_ON:
		SetRefusePersonalTrade(true);
		break;
	case BL_OFF:
	default:
		SetRefusePersonalTrade(false);	
		break;
	}
}

void CX2GameOption::SetRefuseParty(bool val)
{
	m_OptionList.m_bRefuseParty = val;
}

void CX2GameOption::SetRefuseFriend(bool val)
{
	m_OptionList.m_bRefuseFriend = val;
}

void CX2GameOption::SetRefusePersonalTrade(bool val)
{
	m_OptionList.m_bRefusePersonalTrade = val;
}

//{{ kimhc // 2009-10-09 // 길드초대 거부 추가
#ifdef	GUILD_MANAGEMENT
void CX2GameOption::SetRefuseGuildInvitation( CX2GameOption::BlackListDenyState val )
{
	switch ( val )
	{
	case BL_ON:
		SetRefuseGuildInvitation( true );
		break;

	case BL_OFF:
	default:
		SetRefuseGuildInvitation( false );
		break;
	}
}

void CX2GameOption::SetRefuseGuildInvitation( bool val )
{
	m_OptionList.m_bRefuseGuildInvitation = val;
}
#endif	GUILD_MANAGEMENT
//}} kimhc // 2009-10-09 // 길드초대 거부 추가

#ifdef ADDED_RELATIONSHIP_SYSTEM		/// 커플신청 거부 추가
void CX2GameOption::SetRefuseRequestCouple(CX2GameOption::BlackListDenyState val)
{
	switch(val)
	{
	case BL_ON:
		SetRefuseRequestCouple(true);
		break;
	case BL_OFF:
	default:
		SetRefuseRequestCouple(false);	
		break;
	}
}

void CX2GameOption::SetRefuseRequestCouple(bool val)
{
	m_OptionList.m_bRefuseRequestCouple = val;
}
#endif // ADDED_RELATIONSHIP_SYSTEM

#ifdef FIX_INVITE_PVP_PLAYER // 김태환		/// 대전 초대 거부 추가

void CX2GameOption::SetRefuseInvitePVP(CX2GameOption::BlackListDenyState val)
{
	switch(val)
	{
	case BL_ON:
		SetRefuseInvitePVP(true);
		break;
	case BL_OFF:
	default:
		SetRefuseInvitePVP(false);	
		break;
	}
}

void CX2GameOption::SetRefuseInvitePVP(bool val)
{
	m_OptionList.m_bRefuseInvitePVP = val;
}

#endif // FIX_INVITE_PVP_PLAYER







void CX2GameOption::SetFullScreen( bool bFullScreen )
{
#ifdef FULL_SCREEN_OPTION_SAVE_TEST

	m_OptionList.m_bFullScreen = bFullScreen;

	//{{ JHKang // 2010.8.27 // 창 모드 전체 화면 구현
#ifdef TOGGLE_WINDOW_FULLSCREEN
	ToggleWindowFullScreen(!bFullScreen);
	::SetFocus( g_pKTDXApp->GetHWND() );
#else
	if( true == bFullScreen && true == DXUTIsWindowed() )
	{
		DXUTToggleFullScreen();
		::SetFocus( g_pKTDXApp->GetHWND() );
	}
	else if( false == bFullScreen && false == DXUTIsWindowed() )
	{
		DXUTToggleFullScreen();
		::SetFocus( g_pKTDXApp->GetHWND() );
	}
#endif TOGGLE_WINDOW_FULLSCREEN
	//{{ JHKang // 2010.8.27 // 창 모드 전체 화면 구현


#endif FULL_SCREEN_OPTION_SAVE_TEST
}

//{{ JHKang // 2010.8.27 // 창 모드 전체 화면 구현
#ifdef TOGGLE_WINDOW_FULLSCREEN
void CX2GameOption::ToggleWindowFullScreen( bool bFullScreen )
{
	HWND	hwnd = g_pKTDXApp->GetHWND();				// 윈도우 핸들 얻기
	int		screenX = GetSystemMetrics( SM_CXSCREEN );	// 현재 해상도 얻기
	int		screenY = GetSystemMetrics( SM_CYSCREEN );	// 현재 해상도 얻기
	
	// 기본 윈도우 스타일 얻기
	UINT style = GetWindowLong( hwnd, GWL_STYLE );
	
	if (false == bFullScreen)
	{
		// 윈도우 좌표 얻기
		GetWindowRect( hwnd, &m_WindowPosition );
		// 스타일을 변경한다.
		style = style | 0;
		style = style & ~( WS_THICKFRAME | WS_CAPTION | WS_SYSMENU );
		// 변경된 스타일을 Window Object에 넣는다.
		SetWindowLongW( hwnd, GWL_STYLE, style);
		// 위치에 따른 화면 이동(두 대의 모니터만 가능)
		if ( m_WindowPosition.left < screenX )
		{
			DXUTRestorBackBuffer(screenX, screenY);
			MoveWindow( hwnd, 0, 0, screenX, screenY, true );
			SetWindowPos( hwnd, HWND_TOPMOST, 0, 0, screenX, screenY, SWP_NOMOVE | SWP_NOSIZE );
		}
		else
		{
			DXUTRestorBackBuffer(screenX, screenY);
			MoveWindow( hwnd, screenX, 0, screenX, screenY, true );
			SetWindowPos( hwnd, HWND_TOPMOST, screenX, 0, screenX, screenY, SWP_NOMOVE | SWP_NOSIZE );
		}
	}
	else
	{
		// 윈도우 좌표 얻기
		GetWindowRect( hwnd, &m_WindowPosition );
		// 현재 해상도 얻기
		D3DXVECTOR2 vOriginalResolution = GetOptionList().m_vResolution;
		int iResX = static_cast<int>(vOriginalResolution.x);
		int iResY = static_cast<int>(vOriginalResolution.y);
#ifdef _IN_HOUSE_
		// 스타일을 변경한다.
		style = style | WS_OVERLAPPEDWINDOW;
		style = style;
#else
		style = style | WS_OVERLAPPEDWINDOW;
		style = style & ~(WS_SIZEBOX | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX);
#endif
		// 변경된 스타일을 Window Object에 넣는다.
		SetWindowLongW( hwnd, GWL_STYLE, style );
		if ( m_WindowPosition.left < screenX )
		{
			DXUTRestorBackBuffer(iResX, iResY);
			MoveWindow( hwnd, screenX/2 - iResX/2, screenY/2 - iResY/2, iResX, iResY, true );
			SetWindowPos( hwnd, HWND_NOTOPMOST, 0, 0, iResX, iResY, SWP_NOMOVE );
		}
		else
		{
			DXUTRestorBackBuffer(iResX, iResY);
			MoveWindow( hwnd, screenX + screenX/2 - iResX/2, screenY/2 - iResY/2, iResX, iResY, true );
			SetWindowPos( hwnd, HWND_NOTOPMOST, iResX, 0, iResX, iResY, SWP_NOMOVE );
		}
	}
	::SetFocus( g_pKTDXApp->GetHWND() );
}
#endif TOGGLE_WINDOW_FULLSCREEN
//}} JHKang // 2010.8.27 // 창 모드 전체 화면 구현

#ifdef VERTICAL_SYNC_OPTION
void CX2GameOption::SetFlashEffect( IN bool bVal_ )
{
	m_OptionList.m_bFlashEffect = bVal_;
}

void CX2GameOption::SetVerticalSync( IN bool bVal_ )
{
	m_OptionList.m_bVerticalSync = bVal_;

	if( true == bVal_ && D3DPRESENT_INTERVAL_IMMEDIATE == DXUTGetVerticalSync() )
	{
		DXUTSetVerticalSync( 0 );
		::SetFocus( g_pKTDXApp->GetHWND() );
	}
	else if( false == bVal_ && D3DPRESENT_INTERVAL_DEFAULT == DXUTGetVerticalSync() )
	{
		DXUTSetVerticalSync( 1 );
		::SetFocus( g_pKTDXApp->GetHWND() );
	}
}
#endif

#ifdef MODIFY_FRUSTUM
float CX2GameOption::GetCameraDistance()
{
#ifdef PVP_ZOOM_CAMERA
	if( m_OptionList.m_bAutoCamera == true && g_pX2Game != NULL )
	{
		float fBaseDist = m_fPvpZoom2;
		float fZoomOutDist = m_fPvpZoom3;
		if( g_pMain->GetNowStateID() == CX2Main::XS_DUNGEON_GAME )
		{
			fBaseDist = m_fDungeonZoom2;
			fZoomOutDist = m_fDungeonZoom3;
		}
				
		float fDistAct = 1.f;
		if( GetSubZoomCameraTimer() > 0.f )
		{
			if( g_pMain->GetNowStateID() == CX2Main::XS_DUNGEON_GAME )
				fDistAct = m_fDungeonZoom1;
			else
				fDistAct = m_fPvpZoom1;
		}

		switch( GetSubZoomCameraLv() )
		{
		case 0:
			return fBaseDist * fDistAct;
		case 1:
			return (fBaseDist + fZoomOutDist) * fDistAct;	
		default:
			return fBaseDist * fDistAct;
		}
	}
#endif

	float fCameraDistance = 1200.f;
	switch( m_OptionList.m_iZoomLevel )
	{
	default:
	case 0:
		{
			fCameraDistance = 1100.f;
		} break;

	case 1:
		{
			fCameraDistance = 600.f;
		} break;
#ifdef CAMERA_ZOOM_IN_RANGE_EXPAND
	case 2:
		{
			fCameraDistance = 200.f;
		} break;
#endif //CAMERA_ZOOM_IN_RANGE_EXPAND
	case -1:
		{
			fCameraDistance = 1500.f;
		} break;
	}

	float fBaseCameraDistance = 0.f;
	float fStepCameraDistance = 0.f;
	if( g_pX2Game != NULL )
	{
		if( g_pX2Game->GetWorld() != NULL )
		{
			fBaseCameraDistance = g_pX2Game->GetWorld()->GetBaseCameraDistance();
			
			//월드에서 지정하는 카메라는 '0, 1, 2' 3단계로 유지
			int iGetGamerStepDistance = (m_OptionList.m_iZoomLevel > 1 ) ? 1: m_OptionList.m_iZoomLevel;
			iGetGamerStepDistance  = max(-1, iGetGamerStepDistance );

			fStepCameraDistance = g_pX2Game->GetWorld()->GetCameraStepDistacne( iGetGamerStepDistance + 1 );
		}
	}

	return fBaseCameraDistance + fCameraDistance + fStepCameraDistance;
}
#endif //MODIFY_FRUSTUM

#ifdef PVP_ZOOM_CAMERA
void CX2GameOption::ResetAutoCamera()
{
#ifdef _IN_HOUSE_
	FILE *fp = fopen("ZoomCamera.ini", "r");
	if( fp != NULL )
	{
		fscanf(fp, "%f%f%f%f%f%f", &m_fDungeonZoom1, &m_fDungeonZoom2, &m_fDungeonZoom3, &m_fPvpZoom1, &m_fPvpZoom2, &m_fPvpZoom3 );

		fclose(fp);
	}
	else
#endif
	{
		m_iSubZoomCameraLv = 0;
		m_fSubZoomCameraTimer = 0.f;

		m_fDungeonZoom1 = 0.8f;
		m_fDungeonZoom2 = 1300.f;
		m_fDungeonZoom3 = 400.f;
		m_fPvpZoom1 = 0.8f;
		m_fPvpZoom2 = 1500.f;
		m_fPvpZoom3 = 500.f;
	}
	
}
#endif
#ifdef SKILL_SLOT_UI_TYPE_B
void CX2GameOption::SetIsSkillUITypeA( bool bIsSkillUITypeA_, bool bShowBuffIcon_/* = true */)
{
	m_OptionList.m_bIsSkillUITypeA = bIsSkillUITypeA_;

	//스킬 위치 변경
	if( NULL != g_pData && NULL != g_pData->GetUIManager() &&
		NULL != g_pData->GetUIManager()->GetUISkillTree() )
	{
		g_pData->GetUIManager()->GetUISkillTree()->SetSkillUIType(m_OptionList.m_bIsSkillUITypeA);
	}

#ifdef RIDING_SYSTEM
	//펫 스킬 슬롯 위치 변경
	if( NULL != CX2RidingPetManager::GetInstance() && NULL != CX2RidingPetManager::GetInstance()->GetSkillSlot() )
	{
		CX2RidingPetManager::GetInstance()->GetSkillSlot()->SetSkillUIType( m_OptionList.m_bIsSkillUITypeA );
	}
#endif //RIDING_SYSTEM

	//파티 게이지 위치 변경
	CX2GageManager* pGageManager = CX2GageManager::GetInstance();
	if( NULL != pGageManager )
	{
		pGageManager->ResetOtherPlayerUIPosition();
		pGageManager->SetShowBuffIcon( bShowBuffIcon_ );
	}

}
#endif //SKILL_SLOT_UI_TYPE_B

#ifdef X2OPTIMIZE_AUTOSETUP_GAMEOPTION
void CX2GameOption::_DecideProperGraphicOption()
{
	int iCpuScore = 0;
	int iSystemMemScore = 0;
	int iGpuMemScore = 0;

	//Cpu Score 계산
	iCpuScore = _CalcCpuScore();

	//System Memory Score 계산
	MEMORYSTATUS memStatus;
	memStatus.dwLength = sizeof( MEMORYSTATUS );
	GlobalMemoryStatus( &memStatus );
	UINT uiTotalSystemMem = (UINT)( (float)memStatus.dwTotalPhys / 1024.0f / 1024.0f );//MB로...

	//100점으로 환산...
	iSystemMemScore = (int)( ( 100.0f / 4096.0f ) * (float)uiTotalSystemMem );

	if( iSystemMemScore > 100 )
		iSystemMemScore = 100;

	//GPU Memory Score 계산(공유메모리 포함한 용량임)
	UINT uiAvailableTextureMem = (UINT)( (float)g_pKTDXApp->GetDevice()->GetAvailableTextureMem() / 1024.0f / 1024.0f );//MB로...

	//100점으로 환산...
	iGpuMemScore = (int)( ( 100.0f / 4096.0f ) * (float)uiAvailableTextureMem );

	if( iGpuMemScore > 100 )
		iGpuMemScore = 100;

	//FILE* fp = fopen( "AutoSetupGameOptionLog.txt", "w+" );
	//fprintf( fp, "iCpuScore = %d, iSystemMemScore=%d, iGpuMemScore=%d\n", iCpuScore, iSystemMemScore, iGpuMemScore );
	//fclose( fp );

	//현재 pc에 맞는 가장 적합한 그래픽옵션 설정
	_CalcBestGraphicOption( ( iCpuScore + iSystemMemScore ) / 2, iGpuMemScore );
}

int CX2GameOption::_CalcCpuScore()
{
	return ( KBenchMark_SUM::exec() + KBenchMark_LINPACK::exec() + KBenchMark_MXM::exec() ) / 3;
}

void CX2GameOption::_CalcBestGraphicOption( int iCpuTotalScore, int iGpuTotalScore )
{
	//AutoSetupGameOptionWeightConfig.xml를 읽기
	KGCMassFileManager::CMassFile::MASSFILE_MEMBERFILEINFO_POINTER Info;
	Info = g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->LoadDataFile( "AutoSetupGameOptionWeightConfig.xml" );
	if( Info == NULL )
		return;

	//AutoSetupGameOptionWeightConfig.xml를 로드
	xmlDoc* doc = xmlReadMemory( Info->pRealData, Info->size, "AutoSetupGameOptionWeightConfig.xml", NULL, 0 );
	if( doc == NULL )
		return;

	xmlNode* pkRoot = xmlDocGetRootElement(doc);
	if( pkRoot == NULL )
	{
		xmlFreeDoc(doc);
		return;
	}

	if(pkRoot->type == XML_ELEMENT_NODE && strcmp((char*)pkRoot->name,"AutoSetupGameOptionWeightConfig")==0)
	{
		xmlNode * pkElement = pkRoot->children;
		while ( pkElement )
		{
			//Graphic Unit에 대한 사양 측정
			if ( pkElement->type == XML_ELEMENT_NODE && strcmp((char*)pkElement->name,"GRAPHIC_UNIT")==0 )
			{
				//best 값 설정
				m_OptionList.m_UnitDetail = _CalcHighMiddleLow( pkElement, iCpuTotalScore, iGpuTotalScore );
			}
			else if ( pkElement->type == XML_ELEMENT_NODE && strcmp((char*)pkElement->name,"GRAPHIC_TEXTURE")==0 )//Graphic Texture에 대한 사양 측정
			{
				//best 값 설정
				m_OptionList.m_TexDetail = _CalcHighMiddleLow( pkElement, iCpuTotalScore, iGpuTotalScore );
			}
			else if ( pkElement->type == XML_ELEMENT_NODE && strcmp((char*)pkElement->name, "GRAPHIC_MAP" )==0 )//Graphic Map에 대한 사양 측정
			{
				//best 값 설정
				m_OptionList.m_MapDetail = _CalcHighMiddleLow( pkElement, iCpuTotalScore, iGpuTotalScore );
			}
			else if ( pkElement->type == XML_ELEMENT_NODE && strcmp((char*)pkElement->name, "GRAPHIC_EFFECT" )==0 )//Graphic Effect에 대한 사양 측정
			{
				//best 값 설정
				m_OptionList.m_eEffect = _CalcHighMiddleLow( pkElement, iCpuTotalScore, iGpuTotalScore );
			}
			//else if ( pkElement->type == XML_ELEMENT_NODE && strcmp((char*)pkElement->name, "GRAPHIC_VIEWDISTANCE" )==0 )//시야에 대한 사양 측정
			//{
			//	//best 값 설정
			//	m_OptionList.m_eViewDistance = _CalcHighMiddleLow( pkElement, iCpuTotalScore, iGpuTotalScore );
			//}
			else if ( pkElement->type == XML_ELEMENT_NODE && strcmp((char*)pkElement->name, "GRAPHIC_FIELD" )==0 )//Graphic Field에 대한 사양 측정
			{
				//best 값 설정
				OptionLevel iOptionLevel = _CalcHighMiddleLow( pkElement, iCpuTotalScore, iGpuTotalScore );
				switch( iOptionLevel )
				{
				case OL_HIGH:
					m_OptionList.m_FieldDetail = FL_HIGH;
					break;
				case OL_MEDIUM:
					m_OptionList.m_FieldDetail = FL_MEDIUM;
					break;
				case OL_LOW:
					m_OptionList.m_FieldDetail = FL_LOW;
					break;
				}
			}
			else if ( pkElement->type == XML_ELEMENT_NODE && strcmp((char*)pkElement->name, "ShowAllPlayer" )==0 )//Party에 대한 사양 측정
			{
				//best 값 설정
				m_OptionList.m_bParty = !_CalcOnOff( pkElement, iCpuTotalScore, iGpuTotalScore );
			}
			else if ( pkElement->type == XML_ELEMENT_NODE && strcmp((char*)pkElement->name, "NormalViewPlayer" )==0 )//SD에 대한 사양 측정
			{
				//best 값 설정
				m_OptionList.m_bSD = !_CalcOnOff( pkElement, iCpuTotalScore, iGpuTotalScore );
			}
			else if ( pkElement->type == XML_ELEMENT_NODE && strcmp((char*)pkElement->name, "NormalResolution" )==0 )//Resolution에 대한 사양 측정
			{
				//best 값 설정
				if( _CalcOnOff( pkElement, iCpuTotalScore, iGpuTotalScore ) == false )
				{
					m_OptionList.m_vResolution.x = 800;
					m_OptionList.m_vResolution.y = 600;
				}
			}
			else if ( pkElement->type == XML_ELEMENT_NODE && strcmp((char*)pkElement->name, "ShowLevel" )==0 )//Level에 대한 사양 측정
			{
				//best 값 설정
				m_OptionList.m_bLevel = _CalcOnOff( pkElement, iCpuTotalScore, iGpuTotalScore );
			}
			else if ( pkElement->type == XML_ELEMENT_NODE && strcmp((char*)pkElement->name, "ShowPvPRank" )==0 )//PvPRank에 대한 사양 측정
			{
				//best 값 설정
				m_OptionList.m_bPvpRank = _CalcOnOff( pkElement, iCpuTotalScore, iGpuTotalScore );
			}
			else if ( pkElement->type == XML_ELEMENT_NODE && strcmp((char*)pkElement->name, "ShowGuild" )==0 )//Guild에 대한 사양 측정
			{
				//best 값 설정
				m_OptionList.m_bGuild = _CalcOnOff( pkElement, iCpuTotalScore, iGpuTotalScore );
			}
			else if ( pkElement->type == XML_ELEMENT_NODE && strcmp((char*)pkElement->name, "ShowMyInfo" )==0 )//Nothing에 대한 사양 측정
			{
				//best 값 설정
				m_OptionList.m_bNothing = !_CalcOnOff( pkElement, iCpuTotalScore, iGpuTotalScore );
			}
			else if ( pkElement->type == XML_ELEMENT_NODE && strcmp((char*)pkElement->name, "ThreeDimensionSound" )==0 )//3DSound에 대한 사양 측정
			{
				//best 값 설정
				m_OptionList.m_bEnable3DSound = _CalcOnOff( pkElement, iCpuTotalScore, iGpuTotalScore );
			}
			else if ( pkElement->type == XML_ELEMENT_NODE && strcmp((char*)pkElement->name, "FLASHEFFECT" )==0 )//화면번쩍임에 대한 사양 측정
			{
				//best 값 설정
				m_OptionList.m_bFlashEffect = !_CalcOnOff( pkElement, iCpuTotalScore, iGpuTotalScore );
			}
			else if ( pkElement->type == XML_ELEMENT_NODE && strcmp((char*)pkElement->name, "DYNAMIC_CAMERA" )==0 )//다이나믹 카메라에 대한 사양 측정
			{
				//best 값 설정
				m_OptionList.m_bDynamicCamera = _CalcOnOff( pkElement, iCpuTotalScore, iGpuTotalScore );
			}

			pkElement = pkElement->next;
		}
	}

	xmlFreeDoc(doc);
}

CX2GameOption::OptionLevel CX2GameOption::_CalcHighMiddleLow( const xmlNode* pkElement, int iCpuTotalScore, int iGpuTotalScore )
{
	float fGpuWeight = 1.0f;
	int iHighScore = 100;
	int iMiddleScore = 100;
	int iLowScore = 100;

	xmlAttr * attribute =  pkElement->properties;
	if( attribute )
	{
		if ( strcmp ( (char*)attribute->name,"GpuWeight" ) == 0 && attribute->children )
		{
			fGpuWeight = (float)atof( (char*)attribute->children->content );
		}
	}

	xmlNode * pkChildElement = pkElement->children;
	while ( pkChildElement )
	{
		//Graphic Unit에 대한 사양 측정
		if ( pkChildElement->type == XML_ELEMENT_NODE && strcmp((char*)pkChildElement->name,"High")==0 )
		{
			xmlAttr * attribute =  pkChildElement->properties;
			if( attribute )
			{
				if ( strcmp ( (char*)attribute->name,"score" ) == 0 && attribute->children )
				{
					iHighScore = atoi( (char*)attribute->children->content );
				}
			}
		}
		else if ( pkChildElement->type == XML_ELEMENT_NODE && strcmp((char*)pkChildElement->name,"Middle")==0 )
		{
			xmlAttr * attribute =  pkChildElement->properties;
			if( attribute )
			{
				if ( strcmp ( (char*)attribute->name,"score" ) == 0 && attribute->children )
				{
					iMiddleScore = atoi( (char*)attribute->children->content );
				}
			}
		}
		else if ( pkChildElement->type == XML_ELEMENT_NODE && strcmp((char*)pkChildElement->name,"Low")==0 )
		{
			xmlAttr * attribute =  pkChildElement->properties;
			if( attribute )
			{
				if ( strcmp ( (char*)attribute->name,"score" ) == 0 && attribute->children )
				{
					iLowScore = atoi( (char*)attribute->children->content );
				}
			}
		}

		pkChildElement = pkChildElement->next;
	}

	//'높음', '보통', '낮음' 계산
	float fCpuWeight = 1.0f - fGpuWeight;
	int iMyScore = (int)( iCpuTotalScore * fCpuWeight + iGpuTotalScore * fGpuWeight );

	if( iMyScore >= iHighScore )
		return CX2GameOption::OL_HIGH;
	else if( iMyScore >= iMiddleScore )
		return CX2GameOption::OL_MEDIUM;
	else if( iMyScore >= iLowScore )
		return CX2GameOption::OL_LOW;

	return CX2GameOption::OL_LOW;
}

bool CX2GameOption::_CalcOnOff( const xmlNode* pkElement, int iCpuTotalScore, int iGpuTotalScore )
{
	float fGpuWeight = 1.0f;
	int iOnScore = 100;
	int iOffScore = 100;

	xmlAttr * attribute =  pkElement->properties;
	if( attribute )
	{
		if ( strcmp ( (char*)attribute->name,"GpuWeight" ) == 0 && attribute->children )
		{
			fGpuWeight = (float)atof( (char*)attribute->children->content );
		}
	}

	xmlNode * pkChildElement = pkElement->children;
	while ( pkChildElement )
	{
		//Graphic Unit에 대한 사양 측정
		if ( pkChildElement->type == XML_ELEMENT_NODE && strcmp((char*)pkChildElement->name,"On")==0 )
		{
			xmlAttr * attribute =  pkChildElement->properties;
			if( attribute )
			{
				if ( strcmp ( (char*)attribute->name,"score" ) == 0 && attribute->children )
				{
					iOnScore = atoi( (char*)attribute->children->content );
				}
			}
		}
		else if ( pkChildElement->type == XML_ELEMENT_NODE && strcmp((char*)pkChildElement->name,"Off")==0 )
		{
			xmlAttr * attribute =  pkChildElement->properties;
			if( attribute )
			{
				if ( strcmp ( (char*)attribute->name,"score" ) == 0 && attribute->children )
				{
					iOffScore = atoi( (char*)attribute->children->content );
				}
			}
		}

		pkChildElement = pkChildElement->next;
	}

	//'높음', '보통', '낮음' 계산
	float fCpuWeight = 1.0f - fGpuWeight;
	int iMyScore = (int)( iCpuTotalScore * fCpuWeight + iGpuTotalScore * fGpuWeight );

	if( iMyScore >= iOnScore )
		return true;
	else if( iMyScore >= iOffScore )
		return false;

	return false;
}
#endif//X2OPTIMIZE_AUTOSETUP_GAMEOPTION

#ifdef SERV_KEY_MAPPING_INT
void CX2GameOption::SetJoyEnable( bool bEnable )
{
	m_OptionList.m_bJoyEnable = bEnable;
	g_pKTDXApp->GetDIManager()->SetEnableJoyStick(bEnable);

}
#endif // SERV_KEY_MAPPING_INT