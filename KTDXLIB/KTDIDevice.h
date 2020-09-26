#pragma once

///////////////////////////////////////////////////////////////////////
// Mouse and Joystick definitions
///////////////////////////////////////////////////////////////////////

#ifdef KEY_MAPPING
#define AXIS_THRESHOLD_TRIGGER_LEFT  50559
#define AXIS_THRESHOLD_TRIGGER_RIGHT  1433
#endif KEY_MAPPING

#ifdef SERV_KEY_MAPPING_INT
#define AXIS_THRESHOLD					500
#endif SERV_KEY_MAPPING_INT

#define BUTTON_RELEASED    FALSE
#define BUTTON_PRESSED      TRUE

#define BUTTON_UNLOCKED    FALSE
#define BUTTON_LOCKED       TRUE

#define MOUSE_LBUTTON          0
#define MOUSE_RBUTTON          1
#define MOUSE_MBUTTON          2

#define JOYSTICK_BUTTON0       0
#define JOYSTICK_BUTTON1       1
#define JOYSTICK_BUTTON2       2
#define JOYSTICK_BUTTON3       3
#define JOYSTICK_BUTTON4       4
#define JOYSTICK_BUTTON5       5

// Enumerated list of of device types
enum InputDevices 
{
	NONE = 0,
	KEYBOARD,
	MOUSE,
	JOYSTICK
};

class CKTDIDevice
{
	public:
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
		CKTDIDevice( HWND hWnd, IDirectInput8* pDI );
		virtual ~CKTDIDevice();

		IDirectInputDevice8 *DeviceCOM();

		// Generic functions - all devices
		BOOL Create( short Type, BOOL Windowed = TRUE );
		BOOL Free();

		BOOL Clear();
		BOOL Read( float fElapsedTime );
		BOOL Acquire( BOOL Active = TRUE );

		BOOL GetLock( unsigned char Num );
		BOOL SetLock( unsigned char Num, BOOL State = TRUE );
		BOOL GetExtraEnter() { return m_bExtraEnter; }
		void SetExtraEnter( bool val ) { m_bExtraEnter = val; }

		long GetXPos();
		BOOL SetXPos( long XPos );
		long GetYPos();
		BOOL SetYPos( long YPos );
		long GetZPos();	// mouse wheel
		long GetXDelta();
		long GetYDelta();
		DWORD GetPOV();

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

		// Mouse/Joystick specific functions
		BOOL  GetButtonState( unsigned char Num );
		BOOL  SetButtonState( unsigned char Num, BOOL State );
		BOOL  GetPureButtonState( unsigned char Num );
		long  GetNumButtonPresses();
		long  GetNumPureButtonPresses();

		void	UpdateMouseDeltaPosition();
		POINT	GetMouseDeltaPosition();

#ifdef KEY_MAPPING
		bool GetActionState( int Action, long* lpData = NULL);
#ifdef KEY_MAPPING_UPGRADE
		void SetAction( int Action, unsigned char Key, bool isDefault = false );
#else KEY_MAPPING_UPGRADE
		void SetAction( int Action, unsigned char Key );
#endif KEY_MAPPING_UPGRADE
		unsigned char GetActionKey( int Action );
#ifdef SERV_KEY_MAPPING_INT
		bool ListenDeviceInput( unsigned char& ucoutUserInput );
		void GetActionKeys( OUT std::map<short, short>& mapMappingInfo );
		void SetActionKeys( IN const std::map<short, short>& mapMappingInfo );
#endif SERV_KEY_MAPPING_INT

#endif KEY_MAPPING

#ifdef RELEASE_ALL_BUTTONS_AND_KEYS_WHEN_FOCUS_LOSING
		BOOL	ReleaseAllButtonsAndKeys();
#endif

	public:
		short                 m_Type;
		IDirectInputDevice8  *m_pDIDevice;

		BOOL                  m_Windowed;

		char                  m_State[256];
		DIMOUSESTATE         *m_MouseState;
		DIJOYSTATE           *m_JoystickState;

		BOOL                  m_Locks[256];

#ifdef KEY_MAPPING
		unsigned char		  m_ActionMap[GAMEACTION_END];
#endif KEY_MAPPING

		POINT				  m_kMousePrevPos;
		POINT				  m_kMouseDeltaPos;

		// Z좌표는 마우스 좌표를 읽어 온다.
		// 밀거나 당기는 한칸의 단위는 120
		// 밀기 : +120
		// 당김 : -120
		long                  m_XPos, m_YPos, m_ZPos;

		// ime enter lock으로 인해 채팅창으로 enter가 가지 않기 때문에 채팅창 전용의 별도의 변수를 배정
		BOOL m_bExtraEnter;

		static BOOL FAR PASCAL EnumJoysticks( LPCDIDEVICEINSTANCE pdInst, LPVOID pvRef );

	private:
		HWND			m_hWnd;
		IDirectInput8*	m_pDI;
		DoubleKey		m_DoubleKeyCheck[256];

};
