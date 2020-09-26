#include "StdAfx.h"
#include ".\ktdsmanager.h"


CKTDSManager::CKTDSManager( HWND hWnd, LPDIRECT3DDEVICE9 pd3dDevice )
{
	m_bInit				= true;
	m_pMP3Play			= NULL;

#ifdef ADD_CHANGE_BACKGROUND_MUSIC		/// 디파인 인식 불가
	m_pSecondMP3Play	= NULL;		/// 두번째 배경음악
	m_bUseSecondBGM		= false;	/// 현재 사용중인 배경음악 경로
	m_iNowBGMVolume		= 3000;		/// 현재 설정되어 있는 볼륨 값 ( 옵션창 )
#endif // ADD_CHANGE_BACKGROUND_MUSIC

	m_pSystem			= NULL;
	m_bCapable3DSound	= true;
	m_bEnable3DSound	= true; 

	m_fDistanceUnit = 800.f; // note!! 속도에 의한 도플러효과 등에 영향을 주는 듯

	m_bSoundMute		= false;
	m_fSoundVolume		= 0.f;

	FMOD_RESULT      result;
	unsigned int     version;
	FMOD_SPEAKERMODE speakermode;
	FMOD_CAPS        caps;

	//Create a System object and initialize.
	result = FMOD_System_Create( &m_pSystem );
	if( FMOD_OK != result )
	{
		ErrorLog( KEM_ERROR360 );
		
		m_bInit = false;
		return;
	}

	result = FMOD_System_GetVersion( m_pSystem, &version );
	if( FMOD_OK != result )
	{
		ErrorLog( KEM_ERROR361 );

		m_bInit = false;
		return;
	}

	if(version < FMOD_VERSION)
	{
		//printf("Error!  You are using an old version of FMOD %08x.  This program requires %08x\n", version, FMOD_VERSION);
		ErrorLog( KEM_ERROR362 );	

		m_bInit = false;
		return;
	}

	
	
	result = FMOD_System_GetDriverCaps( m_pSystem, 0, &caps, 0, &speakermode );
	if( FMOD_OK != result )
	{
		ErrorLog( KEM_ERROR363 );

		//m_bInit = false;
		//return;
	}
	else
	{

		result = FMOD_System_SetSpeakerMode( m_pSystem, speakermode );	/* Set the user selected speaker mode. */
		if( FMOD_OK != result )
		{
			ErrorLogMsg( KEM_ERROR365, L"__1" );

			m_bInit = false;
			return;
		}

		if( caps & FMOD_CAPS_HARDWARE_EMULATED )             		/* The user has the 'Acceleration' slider set to off!  This is really bad for latency!. */
		{                                                   		/* You might want to warn the user about this. */
			result = FMOD_System_SetDSPBufferSize( m_pSystem, 1024, 10 );	/* At 48khz, the latency between issuing an fmod command and hearing it will now be about 213ms. */
			if( FMOD_OK != result )
			{
				ErrorLog( KEM_ERROR364 );

				m_bInit = false;
				return;
			}
		}
	}


#ifdef FIX_STOLEN_MUSIC_CHANNEL
	const int MAGIC_MAX_CHANNEL_COUNT = 128;
#else FIX_STOLEN_MUSIC_CHANNEL
	const int MAGIC_MAX_CHANNEL_COUNT = 64;
#endif FIX_STOLEN_MUSIC_CHANNEL
	result = FMOD_System_Init( m_pSystem, MAGIC_MAX_CHANNEL_COUNT, FMOD_INIT_NORMAL, NULL );	/* Replace with whatever channel count and flags you use! */
	if( FMOD_OK != result )
	{
		WCHAR wszText[256] = L"";
		swprintf( wszText, sizeof(wszText) / sizeof(WCHAR) - 1, L"1-%d", (int)result );
		ErrorLogMsg( KEM_ERROR366, wszText );
	}

	if( result != FMOD_OK || 
		result == FMOD_ERR_OUTPUT_CREATEBUFFER )				/* Ok, the speaker mode selected isn't supported by this soundcard.  Switch it back to stereo... */
	{
		result = FMOD_System_SetSpeakerMode( m_pSystem, FMOD_SPEAKERMODE_STEREO );
		if( FMOD_OK != result )
		{
			ErrorLogMsg( KEM_ERROR365, L"__2" );

			m_bInit = false;
			return;
		}

		result = FMOD_System_Init( m_pSystem, MAGIC_MAX_CHANNEL_COUNT, FMOD_INIT_NORMAL, NULL );	/* Replace with whatever channel count and flags you use! */
		if( FMOD_OK != result )
		{
			WCHAR wszText[256] = L"";
			swprintf( wszText, sizeof(wszText) / sizeof(WCHAR) - 1, L"2-%d", (int)result );
			ErrorLogMsg( KEM_ERROR366, wszText );

			m_bInit = false;
			return;
		}
	}

	// 3d sound setting 
	// Set the distance units. (meters/feet etc).
	result = FMOD_System_Set3DSettings( m_pSystem, 1.f, m_fDistanceUnit, 1.f ); 
	if( FMOD_OK != result )
	{
		ErrorLog( KEM_ERROR367 );

		m_bCapable3DSound	= false;
		m_bEnable3DSound	= false;
	}

	if( NULL != m_pSystem )
	{
		m_pMP3Play = new CKTDSMP3Play( m_pSystem );
	}

#ifdef ADD_CHANGE_BACKGROUND_MUSIC		/// 디파인 인식 불가
	if( NULL != m_pSystem )
	{
		m_pSecondMP3Play	= new CKTDSMP3Play( m_pSystem );
	}
#endif // ADD_CHANGE_BACKGROUND_MUSIC
}

CKTDSManager::~CKTDSManager(void)
{
	SAFE_DELETE( m_pMP3Play );

#ifdef ADD_CHANGE_BACKGROUND_MUSIC		/// 디파인 인식 불가
	SAFE_DELETE( m_pSecondMP3Play );
#endif // ADD_CHANGE_BACKGROUND_MUSIC

	FMOD_RESULT result;
	if( NULL != m_pSystem && true == m_bInit )
	{
		result = FMOD_System_Close( m_pSystem );
		if( FMOD_OK != result )
		{
			ErrorLog( KEM_ERROR368 );

			return;
		}
		
		result = FMOD_System_Release( m_pSystem );
		if( FMOD_OK != result )
		{
			ErrorLog( KEM_ERROR369 );

			return;
		}
	}
}



HRESULT CKTDSManager::OnFrameMove( double fTime, float fElapsedTime )
{
#ifndef X2VIEWER //JHKang
	KLagCheck( eKnown_LagCheckType_KTDSManager_FrameMove );
#endif //X2VIEWER

	if( NULL != m_pSystem && 
		true == m_bInit )
	{
		FMOD_System_Update( m_pSystem );
	}

	return S_OK;
}




void CKTDSManager::SetListenerData( D3DXVECTOR3 pos, D3DXVECTOR3 frontVec /*= D3DXVECTOR3(0, 0, 1)*/, D3DXVECTOR3 upVec /*= D3DXVECTOR3( 0, 1, 0)*/ )
{
	if( false == m_bInit )
		return;

	if( false == m_bCapable3DSound ||
		false == m_bEnable3DSound )
		return;


	FMOD_VECTOR listenerpos = { pos.x, pos.y, pos.z };
	FMOD_VECTOR forward		= { frontVec.x, frontVec.y, frontVec.z };
	FMOD_VECTOR up			= { upVec.x, upVec.y, upVec.z };

	FMOD_RESULT result;
	result = FMOD_System_Set3DListenerAttributes( m_pSystem, 0, &listenerpos, NULL, &forward, &up ); 
	if( FMOD_OK != result )
	{
		ErrorLog( KEM_ERROR370 );
	}
}





