#include "StdAfx.h"
#include ".\x2soundclosemanager.h"


#ifdef CLOSE_SOUND_TEST

	void CX2SoundCloseManager::CloseSound()
	{
		std::map< wstring, int >::iterator it;
		for( it = m_mapOpenedSoundRefCount.begin(); it != m_mapOpenedSoundRefCount.end() ; it++ )
		{
			for( int i=0; i<it->second ; i++ )
			{
				g_pKTDXApp->GetDeviceManager()->CloseDevice( it->first );
			}
		}
		m_mapOpenedSoundRefCount.clear();
	}

	void CX2SoundCloseManager::AddSound( const std::wstring& soundName )
	{
		std::map< wstring, int >::iterator it = m_mapOpenedSoundRefCount.find( soundName );
		if( it != m_mapOpenedSoundRefCount.end() )
		{
			it->second += 1;
		}
		else
		{
			m_mapOpenedSoundRefCount[ soundName ] = 1;
		}
	}
#endif CLOSE_SOUND_TEST
