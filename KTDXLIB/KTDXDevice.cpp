#include <StdAfx.h>
#include ".\ktdxdevice.h"

bool CKTDXDevice::LoadDevice(
#ifdef	X2OPTIMIZE_SOUND_BACKGROUND_LOAD
				bool bBackgroundQueueing
#endif	X2OPTIMIZE_SOUND_BACKGROUND_LOAD
	)
{ 
	HRESULT hr;

    bool    bWithoutFile = GetWithoutFile();

    {
        CSLock  lock( m_csDeviceState );

        switch( m_eDeviceState )
        {
        case DEVICE_STATE_INIT:
        //case DEVICE_STATE_PENDING:
            if ( bWithoutFile == true )
            {
                m_eDeviceState = DEVICE_STATE_LOADED;
                return true;
            }//if
            m_eDeviceState = DEVICE_STATE_LOADING;
            break;
        case DEVICE_STATE_LOADING:
            if ( bWithoutFile == true )
            {
                m_eDeviceState = DEVICE_STATE_LOADED;
                return true;
            }//if
            break;
        case DEVICE_STATE_LOADED:
            return true;
        case DEVICE_STATE_FAILED:
            return false;
        default: // invalid state
            m_eDeviceState = DEVICE_STATE_FAILED;
            return false;
        }//switch
    }

    hr = _Load( false
#ifdef	X2OPTIMIZE_SOUND_BACKGROUND_LOAD
			, bBackgroundQueueing
#endif	X2OPTIMIZE_SOUND_BACKGROUND_LOAD
		);
    if( FAILED(hr) )
	{
		ErrorLogMsg( KEM_ERROR105, m_DeviceID.c_str() );

        {
            CSLock  locker( m_csDeviceState );
            switch( m_eDeviceState )
            {
            case DEVICE_STATE_INIT:
                ASSERT( 0 );
                m_eDeviceState = DEVICE_STATE_FAILED;
                break;
            //case DEVICE_STATE_PENDING:
            //    ASSERT( 0 );
            //    m_eDeviceState = DEVICE_STATE_FAILED;
            //    break;
            case DEVICE_STATE_LOADING:
                m_eDeviceState = DEVICE_STATE_FAILED;
                break;
            case DEVICE_STATE_FAILED:
                break;
            default:
                ASSERT( 0 );
                m_eDeviceState = DEVICE_STATE_FAILED;
                break;
            case DEVICE_STATE_LOADED:
               return true;
            }
        }

		return false;
	}

    return true;
}

#ifdef CHECK_SOUND_LOADING_TIME
bool CKTDXDevice::CheckLoadDevice( bool& bCreateSound,
#ifdef	X2OPTIMIZE_SOUND_BACKGROUND_LOAD
	bool bBackgroundQueueing
#endif	X2OPTIMIZE_SOUND_BACKGROUND_LOAD
	)
{ 
	HRESULT hr;

	bool    bWithoutFile = GetWithoutFile();

	{
		CSLock  lock( m_csDeviceState );

		switch( m_eDeviceState )
		{
		case DEVICE_STATE_INIT:
			//case DEVICE_STATE_PENDING:
			if ( bWithoutFile == true )
			{
				m_eDeviceState = DEVICE_STATE_LOADED;
				return true;
			}//if
			m_eDeviceState = DEVICE_STATE_LOADING;
			break;
		case DEVICE_STATE_LOADING:
			if ( bWithoutFile == true )
			{
				m_eDeviceState = DEVICE_STATE_LOADED;
				return true;
			}//if
			break;
		case DEVICE_STATE_LOADED:
			return true;
		case DEVICE_STATE_FAILED:
			return false;
		default: // invalid state
			m_eDeviceState = DEVICE_STATE_FAILED;
			return false;
		}//switch
	}

	hr = _CheckLoad( bCreateSound , false
#ifdef	X2OPTIMIZE_SOUND_BACKGROUND_LOAD
		, bBackgroundQueueing
#endif	X2OPTIMIZE_SOUND_BACKGROUND_LOAD
		);
	if( FAILED(hr) )
	{
		ErrorLogMsg( KEM_ERROR105, m_DeviceID.c_str() );

		{
			CSLock  locker( m_csDeviceState );
			switch( m_eDeviceState )
			{
			case DEVICE_STATE_INIT:
				ASSERT( 0 );
				m_eDeviceState = DEVICE_STATE_FAILED;
				break;
				//case DEVICE_STATE_PENDING:
				//    ASSERT( 0 );
				//    m_eDeviceState = DEVICE_STATE_FAILED;
				//    break;
			case DEVICE_STATE_LOADING:
				m_eDeviceState = DEVICE_STATE_FAILED;
				break;
			case DEVICE_STATE_FAILED:
				break;
			default:
				ASSERT( 0 );
				m_eDeviceState = DEVICE_STATE_FAILED;
				break;
			case DEVICE_STATE_LOADED:
				return true;
			}
		}

		return false;
	}

	return true;
}
#endif // CHECK_SOUND_LOADING_TIME

void    CKTDXDevice::RefDevice()
{
    {
        CSLock  lock( m_csDeviceState );

        m_RefCount++;
    }

}//CKTDXDevice::RefDevice()

bool CKTDXDevice::UnrefDevice()
{

    {
        CSLock  lock( m_csDeviceState );

        m_RefCount--;
        
        if ( m_RefCount <= 0 )
        {
            m_RefCount = 0;
            switch( m_eDeviceState )
            {
            case DEVICE_STATE_INIT:
                break;
            //case DEVICE_STATE_PENDING:
            //    break;
            case DEVICE_STATE_LOADING:
            //    ASSERT( 0 );
                break;
            case DEVICE_STATE_LOADED:
                break;
            case DEVICE_STATE_FAILED:
                break;
            }//switch

            return true;
        }//if
    }

    return false;
}


bool    CKTDXDevice::UnrefDeviceIfReferedOnce()
{
    {
        CSLock  lock( m_csDeviceState );

        if ( m_RefCount == 1 )
        {
            m_RefCount--;
            return true;
        }
    }

    return false;
}



CKTDXDevice::EDeviceState    CKTDXDevice::PendDevice()
{ 
    bool    bWithoutFile = GetWithoutFile();

    {
        CSLock  lock( m_csDeviceState );

        switch( m_eDeviceState )
        {
        case DEVICE_STATE_INIT:
            if ( bWithoutFile == true )
            {
                m_eDeviceState = DEVICE_STATE_LOADED;
                return DEVICE_STATE_LOADED;
            }//if
            //m_eDeviceState = DEVICE_STATE_PENDING;
			m_eDeviceState = DEVICE_STATE_LOADING;
            return DEVICE_STATE_INIT;

        //case DEVICE_STATE_PENDING:
       //     if ( bWithoutFile == true )
        //    {
        //        m_eDeviceState = DEVICE_STATE_LOADED;
        //        return  DEVICE_STATE_LOADED;
        //    }//if
        //    return  DEVICE_STATE_PENDING;

        case DEVICE_STATE_LOADING:
            if ( bWithoutFile == true )
            {
                m_eDeviceState = DEVICE_STATE_LOADED;
                return  DEVICE_STATE_LOADED;
            }//if
            return  DEVICE_STATE_LOADING;
        case DEVICE_STATE_LOADED:
            return  DEVICE_STATE_LOADED;

        case DEVICE_STATE_FAILED:
            return  DEVICE_STATE_FAILED;

        default: // invalid state
            m_eDeviceState = DEVICE_STATE_FAILED;
            return  DEVICE_STATE_FAILED;
        }//switch
    }
}