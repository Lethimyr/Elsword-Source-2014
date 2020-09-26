#include "StdAfx.h"
#include ".\ktdxdevicesound.h"

#ifdef CHECK_VOICE_IN_SLIDESHOT
FMOD_RESULT F_CALLBACK FMODChannelCallback(FMOD_CHANNEL *channel, FMOD_CHANNEL_CALLBACKTYPE type, void *commanddata1, void *commanddata2)
{
	//FMOD::Channel *cppChannel = (FMOD::Channel*)channel;

	switch( type )
	{
	case FMOD_CHANNEL_CALLBACKTYPE_END:
		{

		} break;
	default:
		break;
	}


	return FMOD_OK;
}
#endif //CHECK_VOICE_IN_SLIDESHOT


CKTDXDeviceSound::CKTDXDeviceSound( FMOD_SYSTEM* pSystem, wstring fileName, bool bUse3D )
: CKTDXDevice( fileName, DT_SOUND )
{
	m_vFmodPos.x = 0.0f;
	m_vFmodPos.y = 0.0f;
	m_vFmodPos.z = 0.0f;

	m_pSystem = pSystem;
	m_pSound = NULL;
	m_pChannel = NULL;

	m_b3DSound = bUse3D;
    m_fMaxDistance = -1;
}

CKTDXDeviceSound::~CKTDXDeviceSound(void)
{
	_UnLoad();
}

#ifdef	X2OPTIMIZE_SOUND_BACKGROUND_LOAD
void	CKTDXDeviceSound::_CheckLoadFMODSound()
{
    if ( IsLoaded() == false 
		|| m_pSystem == NULL
		|| m_pSound != NULL
		|| !m_spMemoryBuffer )
		return;

	FMOD_RESULT				result;
	FMOD_CREATESOUNDEXINFO	exinfo;

	memset(&exinfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));
	exinfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
	exinfo.length = m_Size;

	FMOD_MODE eMode = FMOD_SOFTWARE | FMOD_OPENMEMORY;		// warning!! FMOD_SOFTWARE or FMOD_HARDWARE
	if( true == g_pKTDXApp->GetDSManager()->GetCapable3DSound() &&
		true == g_pKTDXApp->GetDSManager()->GetEnable3DSound() && 
		true == m_b3DSound )
	{
		eMode |= FMOD_3D;
	}
	
	result = FMOD_System_CreateSound( m_pSystem, (const char *) m_spMemoryBuffer->GetBuffer(), eMode, &exinfo, &m_pSound );
	m_spMemoryBuffer.reset();
	if( FMOD_OK != result || m_pSound == NULL )
	{

		WCHAR wszText[256] = L"";
		swprintf( wszText, sizeof(wszText) / sizeof(WCHAR) - 1, L"%s, %d", m_DeviceID.c_str(), (int)result );
		ErrorLogMsg( KEM_ERROR87, wszText );
        return;
	}

	if( true == g_pKTDXApp->GetDSManager()->GetCapable3DSound() &&
		true == g_pKTDXApp->GetDSManager()->GetEnable3DSound() && 
		true == m_b3DSound )
	{

		FMOD_RESULT result;

		const float MAGIC_DISTANCEFACTOR = g_pKTDXApp->GetDSManager()->GetDistanceUnit();
		result = FMOD_Sound_Set3DMinMaxDistance( m_pSound, 0.1f * MAGIC_DISTANCEFACTOR
            , ( m_fMaxDistance < 0.f ) ? 6.f * MAGIC_DISTANCEFACTOR : m_fMaxDistance ); // warning!!
        m_fMaxDistance = -1.f;
		if( FMOD_OK != result )
		{
			ErrorLog( KEM_ERROR371 );
		}

		result = FMOD_Sound_SetMode( m_pSound, FMOD_3D_LINEARROLLOFF ); // warning!!
		if( FMOD_OK != result )
		{
			ErrorLog( KEM_ERROR372 );
		}
	}
}
#endif	X2OPTIMIZE_SOUND_BACKGROUND_LOAD

#ifdef CHECK_SOUND_LOADING_TIME
void	CKTDXDeviceSound::_CheckCheckLoadFMODSound( bool& bCreateSound )
{
	if ( IsLoaded() == false 
		|| m_pSystem == NULL
		|| m_pSound != NULL
		|| !m_spMemoryBuffer )
		return;

	FMOD_RESULT				result;
	FMOD_CREATESOUNDEXINFO	exinfo;

	memset(&exinfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));
	exinfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
	exinfo.length = m_Size;

	FMOD_MODE eMode = FMOD_SOFTWARE | FMOD_OPENMEMORY;		// warning!! FMOD_SOFTWARE or FMOD_HARDWARE
	if( true == g_pKTDXApp->GetDSManager()->GetCapable3DSound() &&
		true == g_pKTDXApp->GetDSManager()->GetEnable3DSound() && 
		true == m_b3DSound )
	{
		eMode |= FMOD_3D;
	}

	result = FMOD_System_CreateSound( m_pSystem, (const char *) m_spMemoryBuffer->GetBuffer(), eMode, &exinfo, &m_pSound );
	m_spMemoryBuffer.reset();

	if( FMOD_OK != result || m_pSound == NULL )
	{

		WCHAR wszText[256] = L"";
		swprintf( wszText, sizeof(wszText) / sizeof(WCHAR) - 1, L"%s, %d", m_DeviceID.c_str(), (int)result );
		ErrorLogMsg( KEM_ERROR87, wszText );
		return;
	}

	bCreateSound = true;

	if( true == g_pKTDXApp->GetDSManager()->GetCapable3DSound() &&
		true == g_pKTDXApp->GetDSManager()->GetEnable3DSound() && 
		true == m_b3DSound )
	{

		FMOD_RESULT result;

		const float MAGIC_DISTANCEFACTOR = g_pKTDXApp->GetDSManager()->GetDistanceUnit();
		result = FMOD_Sound_Set3DMinMaxDistance( m_pSound, 0.1f * MAGIC_DISTANCEFACTOR
			, ( m_fMaxDistance < 0.f ) ? 6.f * MAGIC_DISTANCEFACTOR : m_fMaxDistance ); // warning!!
		m_fMaxDistance = -1.f;
		if( FMOD_OK != result )
		{
			ErrorLog( KEM_ERROR371 );
		}

		result = FMOD_Sound_SetMode( m_pSound, FMOD_3D_LINEARROLLOFF ); // warning!!
		if( FMOD_OK != result )
		{
			ErrorLog( KEM_ERROR372 );
		}
	}
}

HRESULT CKTDXDeviceSound::_CheckLoad( bool& bCreateSound,
	bool bSkipStateCheck
#ifdef	X2OPTIMIZE_SOUND_BACKGROUND_LOAD
	, bool bBackgroundQueueing
#endif	X2OPTIMIZE_SOUND_BACKGROUND_LOAD	
	)
{
	if( g_pKTDXApp->GetDSManager()->GetInit() == false )
		return S_OK;

	if( NULL == m_pSystem )
		return S_OK;

#ifdef	X2OPTIMIZE_SOUND_BACKGROUND_LOAD

	int     iSize = 0;
	KGCMassFileBufferPtr	spMemoryBuffer;

	{
		KGCMassFileManager::CMassFile::MASSFILE_MEMBERFILEINFO_POINTER Info;
		Info = g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->LoadDataFile( m_DeviceID );
		if( Info == NULL )
		{
			ErrorLogMsg( KEM_ERROR85, m_DeviceID.c_str());
			return E_FAIL;
		}

		iSize = Info.size;
		spMemoryBuffer = Info.pBuffer;
	}

	{
		{
			CSLock  locker( m_csDeviceState );

			if ( bSkipStateCheck == false )
			{
				switch( m_eDeviceState )
				{
				case DEVICE_STATE_INIT:
					//case DEVICE_STATE_PENDING:
				default:
					ASSERT( 0 );
				case DEVICE_STATE_LOADING:
				case DEVICE_STATE_FAILED:
					spMemoryBuffer.swap( m_spMemoryBuffer );
					m_Size = iSize;
					break;
				case DEVICE_STATE_LOADED:
					break;
				}//switch
				m_eDeviceState = DEVICE_STATE_LOADED;
			}
			else
			{
				spMemoryBuffer.swap( m_spMemoryBuffer );
				m_Size = iSize;
			}//if.. else..
		}//if
		spMemoryBuffer.reset();
	}

	if ( bBackgroundQueueing == false )
	{
		_CheckCheckLoadFMODSound( bCreateSound );
	}

	return S_OK;

#endif	X2OPTIMIZE_SOUND_BACKGROUND_LOAD
}
#endif // CHECK_SOUND_LOADING_TIME

HRESULT CKTDXDeviceSound::_Load( bool bSkipStateCheck
#ifdef	X2OPTIMIZE_SOUND_BACKGROUND_LOAD
				, bool bBackgroundQueueing
#endif	X2OPTIMIZE_SOUND_BACKGROUND_LOAD	
	)
{
	if( g_pKTDXApp->GetDSManager()->GetInit() == false )
		return S_OK;

	if( NULL == m_pSystem )
		return S_OK;

#ifdef	X2OPTIMIZE_SOUND_BACKGROUND_LOAD

    int     iSize = 0;
	KGCMassFileBufferPtr	spMemoryBuffer;
	
	{
		KGCMassFileManager::CMassFile::MASSFILE_MEMBERFILEINFO_POINTER Info;
		Info = g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->LoadDataFile( m_DeviceID );
		if( Info == NULL )
		{
			ErrorLogMsg( KEM_ERROR85, m_DeviceID.c_str());
			return E_FAIL;
		}

		iSize = Info.size;
		spMemoryBuffer = Info.pBuffer;
	}

    {
        {
            CSLock  locker( m_csDeviceState );

            if ( bSkipStateCheck == false )
		    {
			    switch( m_eDeviceState )
			    {
			    case DEVICE_STATE_INIT:
			    //case DEVICE_STATE_PENDING:
			    default:
				    ASSERT( 0 );
			    case DEVICE_STATE_LOADING:
			    case DEVICE_STATE_FAILED:
					spMemoryBuffer.swap( m_spMemoryBuffer );
                    m_Size = iSize;
				    break;
			    case DEVICE_STATE_LOADED:
				    break;
			    }//switch
			    m_eDeviceState = DEVICE_STATE_LOADED;
		    }
            else
            {
				spMemoryBuffer.swap( m_spMemoryBuffer );
				m_Size = iSize;
            }//if.. else..
        }//if
		spMemoryBuffer.reset();
    }

	if ( bBackgroundQueueing == false )
	{
		_CheckLoadFMODSound();
	}

    return S_OK;


#else	X2OPTIMIZE_SOUND_BACKGROUND_LOAD

	FMOD_SOUND* pSound = NULL;





	KGCMassFileManager::CMassFile::MASSFILE_MEMBERFILEINFO_POINTER Info;
	Info = g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->LoadDataFile( m_DeviceID );
	if( Info == NULL )
	{
		ErrorLogMsg( KEM_ERROR85, m_DeviceID.c_str());
        goto failed;
	}

	iSize = Info->size;

	FMOD_RESULT				result;
	FMOD_CREATESOUNDEXINFO	exinfo;

	memset(&exinfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));
	exinfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
	exinfo.length = Info->size;    

	FMOD_MODE eMode = FMOD_SOFTWARE | FMOD_OPENMEMORY;		// warning!! FMOD_SOFTWARE or FMOD_HARDWARE
	if( true == g_pKTDXApp->GetDSManager()->GetCapable3DSound() &&
		true == g_pKTDXApp->GetDSManager()->GetEnable3DSound() && 
		true == m_b3DSound )
	{
		eMode |= FMOD_3D;
	}
	
	result = FMOD_System_CreateSound( m_pSystem, (const char *)Info->pRealData, eMode, &exinfo, &pSound );
	if( FMOD_OK != result )
	{

		WCHAR wszText[256] = L"";
		swprintf( wszText, sizeof(wszText) / sizeof(WCHAR) - 1, L"%s, %d", m_DeviceID.c_str(), (int)result );
		ErrorLogMsg( KEM_ERROR87, wszText );
        goto failed;
	}

	if( true == g_pKTDXApp->GetDSManager()->GetCapable3DSound() &&
		true == g_pKTDXApp->GetDSManager()->GetEnable3DSound() && 
		true == m_b3DSound )
	{

		FMOD_RESULT result;

		const float MAGIC_DISTANCEFACTOR = g_pKTDXApp->GetDSManager()->GetDistanceUnit();
		result = FMOD_Sound_Set3DMinMaxDistance( pSound, 0.1f * MAGIC_DISTANCEFACTOR
            , ( m_fMaxDistance < 0.f ) ? 6.f * MAGIC_DISTANCEFACTOR : m_fMaxDistance ); // warning!!
        m_fMaxDistance = -1.f;
		if( FMOD_OK != result )
		{
			ErrorLog( KEM_ERROR371 );
		}

		result = FMOD_Sound_SetMode( pSound, FMOD_3D_LINEARROLLOFF ); // warning!!
		if( FMOD_OK != result )
		{
			ErrorLog( KEM_ERROR372 );
		}
	}

    {
        {
            CSLock  locker( m_csDeviceState );

            if ( bSkipStateCheck == false )
		    {
			    switch( m_eDeviceState )
			    {
			    case DEVICE_STATE_INIT:
			    //case DEVICE_STATE_PENDING:
			    default:
				    ASSERT( 0 );
			    case DEVICE_STATE_LOADING:
			    case DEVICE_STATE_FAILED:
                    std::swap( m_pSound, pSound );
                    m_Size = iSize;
				    break;
			    case DEVICE_STATE_LOADED:
				    break;
			    }//switch
			    m_eDeviceState = DEVICE_STATE_LOADED;
		    }
            else
            {
                std::swap( m_pSound, pSound );
                m_Size = iSize;
            }//if.. else..
        }//if
		if ( pSound != NULL )
		{
	        result = FMOD_Sound_Release( pSound );
	        if( FMOD_OK != result )
	        {
		        ErrorLogMsg( KEM_ERROR373, m_DeviceID.c_str() );
	        }
		}//if
    }

    return S_OK;

failed:

    if ( pSound != NULL )
    {
	    result = FMOD_Sound_Release( pSound );
	    if( FMOD_OK != result )
	    {
		    ErrorLogMsg( KEM_ERROR373, m_DeviceID.c_str() );
	    }
    }//if

	return E_FAIL;

#endif	X2OPTIMIZE_SOUND_BACKGROUND_LOAD
}

HRESULT CKTDXDeviceSound::_UnLoad()
{
	////CKTDXThread::CLocker locker( g_pKTDXApp->GetDeviceManager()->GetDeviceLock() );

    if ( m_pSound != NULL )
    {
	    FMOD_SOUND* pSound = m_pSound;
        m_pSound = NULL;
    	FMOD_RESULT       result;
	    result = FMOD_Sound_Release( pSound );
	    if( FMOD_OK != result )
	    {
		    ErrorLogMsg( KEM_ERROR373, m_DeviceID.c_str() );
		    return E_FAIL;
	    }
    }//if

#ifdef	X2OPTIMIZE_SOUND_BACKGROUND_LOAD
	m_spMemoryBuffer.reset();
#endif	X2OPTIMIZE_SOUND_BACKGROUND_LOAD

	return S_OK;
}

void CKTDXDeviceSound::Play( bool loop /*= false */, bool b3DSound /*= true*/ )
{
	KTDXPROFILE();

	if( g_pKTDXApp->GetDSManager()->GetInit() == false || 
		g_pKTDXApp->GetDSManager()->GetSoundMute() == true )
		return;

	if( false == IsLoaded() )
		return;

	if( NULL == m_pSystem )
		return;

#ifdef	X2OPTIMIZE_SOUND_BACKGROUND_LOAD
	_CheckLoadFMODSound();
#endif	X2OPTIMIZE_SOUND_BACKGROUND_LOAD

	if( NULL == m_pSound )
		return;

	FMOD_RESULT result;

	FMOD_MODE mode;
	// warning!! sound의 모드를 3d, 2d로 setting, 밑에 부분에 channel의 모드를 3d, 2d로 setting하는 부분이 따로 있음.
	//result = FMOD_Sound_GetMode( m_pSound, &mode );
	//if( FMOD_OK == result )
	//{
	//	if( true == b3DSound )
	//	{
	//		if( !(mode & FMOD_3D) )
	//		{
	//			result = FMOD_Sound_SetMode( m_pSound, FMOD_3D );
	//			if( FMOD_OK != result )
	//			{
	//				//error
	//			}
	//		}
	//	}
	//	else 
	//	{
	//		if( !(mode & FMOD_2D) )
	//		{
	//			result = FMOD_Sound_SetMode( m_pSound, FMOD_2D );
	//			if( FMOD_OK != result )
	//			{
	//				//error
	//			}
	//		}
	//	}
	//}



	//int channelsPlaying;
	//FMOD_System_GetChannelsPlaying( m_pSystem, &channelsPlaying);
	//debugWPrintf( L"channelsPlaying: %d", channelsPlaying );
	


#pragma NOTE( "m_pChannel을 덮어써버리는 문제 해결해야합니다." )
	m_pChannel = NULL;
#ifdef FIX_3D_EFFECT_NOT_WORKING_FIRST_SECONDS
	result = FMOD_System_PlaySound( m_pSystem, FMOD_CHANNEL_FREE, m_pSound, true, &m_pChannel );
#else 
	result = FMOD_System_PlaySound( m_pSystem, FMOD_CHANNEL_FREE, m_pSound, false, &m_pChannel );
#endif //FIX_3D_EFFECT_NOT_WORKING_FIRST_SECONDS
	if( FMOD_OK != result )
	{
		ErrorLog( KEM_ERROR374 );

		return;
	}


	result = FMOD_Channel_GetMode( m_pChannel, &mode );
	if( FMOD_OK == result )
	{
		if( true == g_pKTDXApp->GetDSManager()->GetCapable3DSound() &&
			true == g_pKTDXApp->GetDSManager()->GetEnable3DSound() && 
			true == m_b3DSound && 
			true == b3DSound )
		{
			if( !(mode & FMOD_3D) )
			{
				result = FMOD_Channel_SetMode( m_pChannel, FMOD_3D );
				if( FMOD_OK != result )
				{
					ErrorLog( KEM_ERROR376 );
				}
			}

			const bool bResultPosition = Update3DPosition();
			if ( true == bResultPosition )
				FMOD_Channel_SetPaused( m_pChannel, false );

			//result = FMOD_Channel_Set3DSpread( m_pChannel, 90.f );	// warning!!! TEST
			
		}
		else 
		{
			if( !(mode & FMOD_2D) )
			{
				result = FMOD_Channel_SetMode( m_pChannel, FMOD_2D );
				if( FMOD_OK != result )
				{
					ErrorLog( KEM_ERROR377 );
				}
			}

			FMOD_Channel_SetPaused( m_pChannel, false );
		}

		if( true == loop && !(mode & FMOD_LOOP_NORMAL) )
		{
			result = FMOD_Channel_SetMode( m_pChannel, FMOD_LOOP_NORMAL );
			if( FMOD_OK != result )
			{
				ErrorLog( KEM_ERROR375 );
			}
		}
		else if( false == loop && !(mode & FMOD_LOOP_OFF) )
		{
			result = FMOD_Channel_SetMode( m_pChannel, FMOD_LOOP_OFF );
			if( FMOD_OK != result )
			{
				ErrorLog( KEM_ERROR375 );
			}
		}
	}

#ifdef FIX_3D_EFFECT_NOT_WORKING_FIRST_SECONDS
	result = FMOD_Channel_SetPaused( m_pChannel, FALSE );
	if( FMOD_OK != result )
	{
		ErrorLog( KEM_ERROR427 );
	}
#endif //FIX_3D_EFFECT_NOT_WORKING_FIRST_SECONDS

	// [-3000,0] 사이의 값을 옵션에서 입력으로 받는다
	const float MAGIC_BOUND = 3000.f;
	float fVolume = MAGIC_BOUND + g_pKTDXApp->GetDSManager()->GetSoundVolume();
	fVolume = (float) ( ( (int)fVolume ) % (int)MAGIC_BOUND );
	fVolume /= MAGIC_BOUND;

	result = FMOD_Channel_SetVolume( m_pChannel, fVolume ); 
	if( FMOD_OK != result )
	{
		ErrorLog( KEM_ERROR378 );
	}

#ifdef CHECK_VOICE_IN_SLIDESHOT
	result = FMOD_Channel_SetCallback( m_pChannel, FMODChannelCallback );
#endif //CHECK_VOICE_IN_SLIDESHOT	
}

#ifdef CHECK_SOUND_LOADING_TIME
void CKTDXDeviceSound::CheckPlay(bool& bCreateSound, bool loop /*= false */, bool b3DSound /*= true*/ )
{
	KTDXPROFILE();

	if( g_pKTDXApp->GetDSManager()->GetInit() == false || 
		g_pKTDXApp->GetDSManager()->GetSoundMute() == true )
		return;

	if( false == IsLoaded() )
		return;

	if( NULL == m_pSystem )
		return;

#ifdef	X2OPTIMIZE_SOUND_BACKGROUND_LOAD
	_CheckCheckLoadFMODSound( bCreateSound );
#endif	X2OPTIMIZE_SOUND_BACKGROUND_LOAD

	if( NULL == m_pSound )
		return;

	FMOD_RESULT result;

	FMOD_MODE mode;
	// warning!! sound의 모드를 3d, 2d로 setting, 밑에 부분에 channel의 모드를 3d, 2d로 setting하는 부분이 따로 있음.
	//result = FMOD_Sound_GetMode( m_pSound, &mode );
	//if( FMOD_OK == result )
	//{
	//	if( true == b3DSound )
	//	{
	//		if( !(mode & FMOD_3D) )
	//		{
	//			result = FMOD_Sound_SetMode( m_pSound, FMOD_3D );
	//			if( FMOD_OK != result )
	//			{
	//				//error
	//			}
	//		}
	//	}
	//	else 
	//	{
	//		if( !(mode & FMOD_2D) )
	//		{
	//			result = FMOD_Sound_SetMode( m_pSound, FMOD_2D );
	//			if( FMOD_OK != result )
	//			{
	//				//error
	//			}
	//		}
	//	}
	//}



	//int channelsPlaying;
	//FMOD_System_GetChannelsPlaying( m_pSystem, &channelsPlaying);
	//debugWPrintf( L"channelsPlaying: %d", channelsPlaying );



#pragma NOTE( "m_pChannel을 덮어써버리는 문제 해결해야합니다." )
	m_pChannel = NULL;
#ifdef FIX_3D_EFFECT_NOT_WORKING_FIRST_SECONDS
	result = FMOD_System_PlaySound( m_pSystem, FMOD_CHANNEL_FREE, m_pSound, true, &m_pChannel );
#else 
	result = FMOD_System_PlaySound( m_pSystem, FMOD_CHANNEL_FREE, m_pSound, false, &m_pChannel );
#endif //FIX_3D_EFFECT_NOT_WORKING_FIRST_SECONDS
	if( FMOD_OK != result )
	{
		ErrorLog( KEM_ERROR374 );

		return;
	}


	result = FMOD_Channel_GetMode( m_pChannel, &mode );
	if( FMOD_OK == result )
	{
		if( true == g_pKTDXApp->GetDSManager()->GetCapable3DSound() &&
			true == g_pKTDXApp->GetDSManager()->GetEnable3DSound() && 
			true == m_b3DSound && 
			true == b3DSound )
		{
			if( !(mode & FMOD_3D) )
			{
				result = FMOD_Channel_SetMode( m_pChannel, FMOD_3D );
				if( FMOD_OK != result )
				{
					ErrorLog( KEM_ERROR376 );
				}
			}

			const bool bResultPosition = Update3DPosition();
			if ( true == bResultPosition )
				FMOD_Channel_SetPaused( m_pChannel, false );

			//result = FMOD_Channel_Set3DSpread( m_pChannel, 90.f );	// warning!!! TEST

		}
		else 
		{
			if( !(mode & FMOD_2D) )
			{
				result = FMOD_Channel_SetMode( m_pChannel, FMOD_2D );
				if( FMOD_OK != result )
				{
					ErrorLog( KEM_ERROR377 );
				}
			}

			FMOD_Channel_SetPaused( m_pChannel, false );
		}

		if( true == loop && !(mode & FMOD_LOOP_NORMAL) )
		{
			result = FMOD_Channel_SetMode( m_pChannel, FMOD_LOOP_NORMAL );
			if( FMOD_OK != result )
			{
				ErrorLog( KEM_ERROR375 );
			}
		}
		else if( false == loop && !(mode & FMOD_LOOP_OFF) )
		{
			result = FMOD_Channel_SetMode( m_pChannel, FMOD_LOOP_OFF );
			if( FMOD_OK != result )
			{
				ErrorLog( KEM_ERROR375 );
			}
		}
	}

#ifdef FIX_3D_EFFECT_NOT_WORKING_FIRST_SECONDS
	result = FMOD_Channel_SetPaused( m_pChannel, FALSE );
	if( FMOD_OK != result )
	{
		ErrorLog( KEM_ERROR427 );
	}
#endif //FIX_3D_EFFECT_NOT_WORKING_FIRST_SECONDS

	// [-3000,0] 사이의 값을 옵션에서 입력으로 받는다
	const float MAGIC_BOUND = 3000.f;
	float fVolume = MAGIC_BOUND + g_pKTDXApp->GetDSManager()->GetSoundVolume();
	fVolume = (float) ( ( (int)fVolume ) % (int)MAGIC_BOUND );
	fVolume /= MAGIC_BOUND;

	result = FMOD_Channel_SetVolume( m_pChannel, fVolume ); 
	if( FMOD_OK != result )
	{
		ErrorLog( KEM_ERROR378 );
	}

}
#endif // CHECK_SOUND_LOADING_TIME

void CKTDXDeviceSound::Wait()
{
	if( g_pKTDXApp->GetDSManager()->GetInit() == false )
		return;

	if( NULL != m_pChannel )
	{
		FMOD_RESULT result;
		result = FMOD_Channel_SetPaused( m_pChannel, TRUE );
		if( FMOD_OK != result )
		{
			ErrorLog( KEM_ERROR379 );
		}
	}
}

void CKTDXDeviceSound::Stop()
{
	if( g_pKTDXApp->GetDSManager()->GetInit() == false )
		return;

	if( NULL != m_pChannel && 
		true == IsPlaying() )
	{
		FMOD_RESULT result;
		result = FMOD_Channel_Stop( m_pChannel );
		if( FMOD_OK != result )
		{
			ErrorLog( KEM_ERROR380 );
		}
	}

}

void CKTDXDeviceSound::Set3DPosition( const D3DXVECTOR3& pos )
{
	m_vFmodPos.x = pos.x; 
	m_vFmodPos.y = pos.y; 
	m_vFmodPos.z = pos.z; 
}

bool CKTDXDeviceSound::IsPlaying()
{
	KTDXPROFILE();



	if( g_pKTDXApp->GetDSManager()->GetInit() == false )
		return false;

	if( IsLoaded() == false )
		return false;


	if( NULL == m_pChannel )
		return false;

	FMOD_BOOL bPlaying = FALSE;
	FMOD_RESULT result;
	result = FMOD_Channel_IsPlaying( m_pChannel, &bPlaying );
	if( FMOD_OK != result )
	{
		// channel의 play가 끝나는 시점에서 m_pchannel의 값이 invalid해진다.
		//ErrorLog( KEM_ERROR382 );

		m_pChannel = NULL;
		return false;
	}

	if( TRUE == bPlaying )
		return true;
	else 
		return false;


}



void CKTDXDeviceSound::SetMax3DDistance( float fMaxDistance )
{
    if ( IsLoaded() == false )
    {
        m_fMaxDistance = fMaxDistance;
        return;
    }//if

	if( g_pKTDXApp->GetDSManager()->GetInit() == false )
		return;

	if( true == g_pKTDXApp->GetDSManager()->GetCapable3DSound() &&
		true == g_pKTDXApp->GetDSManager()->GetEnable3DSound() && 
		true == m_b3DSound )
	{
#ifdef	X2OPTIMIZE_SOUND_BACKGROUND_LOAD
		_CheckLoadFMODSound();
#endif	X2OPTIMIZE_SOUND_BACKGROUND_LOAD

		if ( m_pSound == NULL )
			return;

        m_fMaxDistance = -1.f;

		FMOD_RESULT result;

		const float MAGIC_DISTANCE_FACTOR = g_pKTDXApp->GetDSManager()->GetDistanceUnit();
		result = FMOD_Sound_Set3DMinMaxDistance( m_pSound, 0.1f * MAGIC_DISTANCE_FACTOR, fMaxDistance );	 // warning!!
		if( FMOD_OK != result )
		{
			ErrorLogMsg( KEM_ERROR371, "max 3d dist" );
		}
	}
}

void CKTDXDeviceSound::SetVolume( float fVolume )
{
	KTDXPROFILE();



	if( g_pKTDXApp->GetDSManager()->GetInit() == false )
		return;

	if( false == IsPlaying() )
		return; 

	FMOD_RESULT result;

	result = FMOD_Channel_SetVolume( m_pChannel, fVolume ); 
	if( FMOD_OK != result )
	{
		ErrorLogMsg( KEM_ERROR378, "volume" );
	}
}

bool CKTDXDeviceSound::Update3DPosition()
{
	KTDXPROFILE();

	FMOD_RESULT result = FMOD_Channel_Set3DAttributes( m_pChannel, &m_vFmodPos, NULL );
	if( FMOD_OK != result )
	{
		ErrorLog( KEM_ERROR381 );
		return false;
	}
	else
		return true;
}
