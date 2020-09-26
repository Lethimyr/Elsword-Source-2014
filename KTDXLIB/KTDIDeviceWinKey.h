#ifdef CHANGE_KEY_DEVICE

#pragma once

///////////////////////////////////////////////////////////////////////
// Mouse and Joystick definitions
///////////////////////////////////////////////////////////////////////


class CKTDIDeviceWinKey
{
	public:

        struct  KeyTime
        {
            DWORD   m_dwStamp;
            BYTE    m_byVirtualKey;
            bool    m_bDown;

            KeyTime()
                : m_dwStamp( 0 )
                , m_byVirtualKey( 0 )
                , m_bDown( false )
            {
            }
        };//struct  KeyTime

		struct DoubleKey
		{
			bool	bFirstDown;
			bool	bKeyUp;
			float	fTimeOut;

			bool	bDoubleOK;
			bool	bDoubleKeyLock;

			void Init()
			{
				bFirstDown		= false;
				bKeyUp			= false;
				fTimeOut		= 0.0f;
				
				bDoubleOK		= false;
				bDoubleKeyLock	= false;
			}

			DoubleKey()
			{
				Init();
			}
		};

	public:

		CKTDIDeviceWinKey();
		virtual ~CKTDIDeviceWinKey();

		// Generic functions - all devices
		BOOL Create();
		BOOL Free();

		BOOL Clear();
		BOOL Read( DWORD dwStepTime_, float fElapsedTime_ );
		BOOL Acquire( BOOL Active = TRUE );

		BOOL GetLock( unsigned char Num );
		BOOL SetLock( unsigned char Num, BOOL State = TRUE );
		BOOL GetExtraEnter() 
		{
			return m_bExtraEnter; 
		}
		void SetExtraEnter( bool val ) 
		{
			m_bExtraEnter = val; 
		}

		// Keyboard specific functions
		BOOL  GetKeyState( unsigned char Num );
		BOOL  SetKeyState( unsigned char Num, BOOL State );
		BOOL  GetPureKeyState( unsigned char Num );
		BOOL  GetDoubleKeyPureState( unsigned char Num );
		BOOL  GetDoubleKeyState( unsigned char Num );
		void  SetDoubleKeyReset( unsigned char Num );
		short GetKeypress( long TimeOut = 0 );
		long  GetNumKeyPresses();
		long  GetNumPureKeyPresses();

#ifdef KEY_MAPPING
		bool GetActionState( int Action, long* lpData = NULL);
#ifdef KEY_MAPPING_UPGRADE
		void SetAction( int Action, unsigned char Key, bool isDefault = false );
#else KEY_MAPPING_UPGRADE
		void SetAction( int Action, unsigned char Key );
#endif KEY_MAPPING_UPGRADE
		unsigned char GetActionKey( int Action );
#endif KEY_MAPPING

        void    SetupVKToDIKMap();

        void    SetPushedKeys( DWORD dwNowTime );
        void    ReleaseAllKeys( DWORD dwReleaseTime );
        void    MsgProc_KeyMsg(     HWND hWnd_
                , UINT uMsg_
                , WPARAM wParam_
                , LPARAM lParam_ );

        bool    IsActivated() const { return m_bActivated; }

#ifdef RELEASE_ALL_BUTTONS_AND_KEYS_WHEN_FOCUS_LOSING
		BOOL	ReleaseAllButtonsAndKeys();
#endif

	public:

        BYTE                  m_aVKtoDIKMap[256];
        bool                  m_abQueuedVKState[256];
        bool                  m_abCurrentVKState[256];
        bool                  m_bActivated;

		BYTE                  m_State[256];
		BOOL                  m_Locks[256];

#ifdef KEY_MAPPING
		unsigned char		  m_ActionMap[GAMEACTION_END];
#endif KEY_MAPPING

        // ime enter lock으로 인해 채팅창으로 enter가 가지 않기 때문에 채팅창 전용의 별도의 변수를 배정
		BOOL m_bExtraEnter;



	private:

        std::deque<KeyTime> m_dequeKeyTime;

		DoubleKey		m_DoubleKeyCheck[256];


};

#endif //CHANGE_KEY_DEVICE