#pragma once



#ifdef CLOSE_SOUND_TEST
	
	class CX2SoundCloseManager
	{
	public: 
		void CloseSound();
		void AddSound( const std::wstring& soundName );

	private:
		std::map< wstring, int > m_mapOpenedSoundRefCount; 
	};

#endif CLOSE_SOUND_TEST
