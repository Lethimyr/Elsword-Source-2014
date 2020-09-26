#include "StdAfx.h"
#include ".\ktdsmp3play.h"

CKTDSMP3Play::CKTDSMP3Play( FMOD_SYSTEM* pSystem )
{
	m_bLoop			= true;
	m_Volume		= 0;
	m_bPaused		= false;

	m_pSystem = pSystem;
	m_pSound = NULL;	
	m_pChannel = NULL;
}


CKTDSMP3Play::~CKTDSMP3Play(void)
{
	Clear();
}

void CKTDSMP3Play::Play( const WCHAR* pFileName, bool loop /*= true*/ )
{
	if( g_pKTDXApp->GetDSManager()->GetInit() == false )
		return;

	if( NULL == pFileName )
		return; 
#ifdef ALWAYS_MAPPING_MUSIC
	std::string TempString = KncUtil::toNarrowString( pFileName );
	TempString = g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->MappingFileMusic(TempString);
	std::wstring TempWstring;
	ConvertCharToWCHAR(TempWstring,TempString);
	pFileName = TempWstring.c_str();
#endif ALWAYS_MAPPING_MUSIC

	WCHAR wcsFileName[MAX_PATH] = L"";

#ifdef	CONVERSION_VS
	wcscpy_s( wcsFileName, _countof(wcsFileName), pFileName );
#else	CONVERSION_VS
	wcscpy( wcsFileName, pFileName );
#endif	CONVERSION_VS
	
	if( g_pKTDXApp->GetDeviceManager()->GetPullPath( wcsFileName ) == NULL )
		return;

	m_FileName	= pFileName;
	m_bLoop		= loop;



	if( NULL == m_pSystem )
		return;

	Clear();

	std::string strFileName;
	ConvertWCHARToChar( strFileName, wcsFileName );
	
	FMOD_RESULT result;
	FMOD_MODE mode = FMOD_HARDWARE | FMOD_2D;
	if( true == m_bLoop )
	{
		mode |= FMOD_LOOP_NORMAL;
	}
	else 
	{
		mode |= FMOD_LOOP_OFF;
	}

	result = FMOD_System_CreateStream( m_pSystem, strFileName.c_str(), mode, NULL, &m_pSound );	
	if( FMOD_OK != result )
	{
		ErrorLog( KEM_ERROR383 );
		return;
	}


	result = FMOD_System_PlaySound( m_pSystem, FMOD_CHANNEL_FREE, m_pSound, m_bPaused, &m_pChannel );
	if( FMOD_OK != result )
	{
		ErrorLog( KEM_ERROR384 );
		return; 
	}

	// [-3000,0] 사이의 값을 옵션에서 입력으로 받는다
	const float MAGIC_BOUND = 3000.f;
	float fVolume = MAGIC_BOUND + m_Volume;
	fVolume = (float) ( ( (int)fVolume ) % (int)MAGIC_BOUND );
	fVolume /= MAGIC_BOUND;

	result = FMOD_Channel_SetVolume( m_pChannel, fVolume );
	if( FMOD_OK != result )
	{
		ErrorLog( KEM_ERROR385 );
	}

#ifdef FIX_STOLEN_MUSIC_CHANNEL
	FMOD_Channel_SetPriority( m_pChannel, 0 );
#endif FIX_STOLEN_MUSIC_CHANNEL
	//SetMute( m_bMute );

}

void CKTDSMP3Play::Stop()
{
	Clear();
}

void CKTDSMP3Play::SetVolume( int volume )
{
	if( g_pKTDXApp->GetDSManager()->GetInit() == false )
		return;

	m_Volume = volume;

	FMOD_RESULT result;
	if( NULL == m_pChannel )
		return;

	// [-3000,0] 사이의 값을 옵션에서 입력으로 받는다
	const float MAGIC_BOUND = 3000.f;
	float fVolume = MAGIC_BOUND + m_Volume;
	fVolume = (float) ( ( (int)fVolume ) % (int)MAGIC_BOUND );
	fVolume /= MAGIC_BOUND;

	result = FMOD_Channel_SetVolume( m_pChannel, fVolume ); 
	if( FMOD_OK != result )
	{
		ErrorLog( KEM_ERROR386 );
	}
}


void CKTDSMP3Play::SetPaused( bool bPause )
{
	if( g_pKTDXApp->GetDSManager()->GetInit() == false )
		return;

	if( m_bPaused == bPause )
		return;

	m_bPaused = bPause;


	if( NULL == m_pChannel )
	{
		if( false == bPause )
		{
			Play( m_FileName.c_str(), m_bLoop );
		}
		return;
	}

	FMOD_RESULT result;
	if( true == bPause )
	{
		result = FMOD_Channel_SetPaused( m_pChannel, TRUE );
	}
	else 
	{
		result = FMOD_Channel_SetPaused( m_pChannel, FALSE );
	}

	if( FMOD_OK != result )
	{
		ErrorLogMsg( KEM_ERROR387, L"SetPaused" );
	}


}



void CKTDSMP3Play::Clear()
{
	if( g_pKTDXApp->GetDSManager()->GetInit() == false )
		return;

	FMOD_RESULT       result;

#if 0 // sound release하므로 channel은 stop 시킬 이유가 없을듯... 아니면 말고
 	if( NULL != m_pChannel )
 	{
 		result = FMOD_Channel_Stop( m_pChannel );
 		if( FMOD_OK == result )
 		{
 			m_pChannel = NULL;
 		}
 		else
 		{
 			ErrorLog( KEM_ERROR388 );
 
 		}
 	}
#endif

	if( NULL != m_pSound )
	{
		result = FMOD_Sound_Release( m_pSound );
		if( FMOD_OK == result )
		{
			m_pSound = NULL;
		}
		else
		{
			ErrorLog( KEM_ERROR389 );
		}
	}
}

