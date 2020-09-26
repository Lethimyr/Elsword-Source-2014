#include "StdAfx.h"
#include ".\ktdidevice.h"

#define DOUBLE_INPUT_GAP 0.4f

CKTDIDevice::CKTDIDevice( HWND hWnd, IDirectInput8* pDI )
{
	m_hWnd = hWnd;
	m_pDI = pDI;

	// Setup device to none
	m_Type = NONE;

	// Set windowed usage to TRUE
	m_Windowed = TRUE;

	// Clear the DirectInput interface pointer
	m_pDIDevice = NULL;

	// Point the mouse and joystick structures to the state buffer
	m_MouseState    = (DIMOUSESTATE*)&m_State;
	m_JoystickState = (DIJOYSTATE*)&m_State;

	m_kMousePrevPos.x = 0;
	m_kMousePrevPos.y = 0;

	m_kMouseDeltaPos.x = 0;
	m_kMouseDeltaPos.y = 0;

	for ( int i = 0; i < 256; ++i )
		m_Locks[i] = false;

	// Clear the device variables
	Clear();
}

CKTDIDevice::~CKTDIDevice()
{
	// Free a prior install
	Free();
}

IDirectInputDevice8* CKTDIDevice::DeviceCOM()
{
	// return the pointer to the IDirectDevice8 object
	return m_pDIDevice;
}

// Generic functions - all devices
BOOL CKTDIDevice::Create( short Type, BOOL Windowed )
{
#ifdef DLL_BUILD
	// 디바이스 만들기 생략
	Free();
	m_Windowed = Windowed;
	m_Type = Type;
	Clear();
	return TRUE;
#endif 

	DIDATAFORMAT *DataFormat;
	DIPROPRANGE   DIprg;
	DIPROPDWORD   DIpdw;
	HRESULT	hr;

	// Free a prior device
	Free();

	// Create the device and remember device data format
	switch(Type) 
	{
	case KEYBOARD:
		if(FAILED(m_pDI->CreateDevice(GUID_SysKeyboard, &m_pDIDevice, NULL)))
		{
			ErrorLog(KEM_ERROR51);
			return FALSE;
		}

		DataFormat = (DIDATAFORMAT*)&c_dfDIKeyboard;
		break;

	case MOUSE:
		if(FAILED(m_pDI->CreateDevice(GUID_SysMouse, &m_pDIDevice, NULL))) 
		{
			ErrorLog(KEM_ERROR52);
			return FALSE;
		}

		DataFormat = (DIDATAFORMAT*)&c_dfDIMouse;
		break;

	case JOYSTICK:
		if(FAILED(m_pDI->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoysticks, this, DIEDFL_ATTACHEDONLY)))
		{
			ErrorLog(KEM_ERROR53);
			return FALSE;
		}

		if(m_pDIDevice == NULL)
		{
			ErrorLog(KEM_ERROR54);
			return FALSE;
		}

		DataFormat = (DIDATAFORMAT*)&c_dfDIJoystick;
		break;

	default: 
		{
			ErrorLog(KEM_ERROR55);
			return FALSE;
		}
	}

	// Set the windowed usage
	m_Windowed = Windowed;

	// Set the data format of keyboard
	if(FAILED(m_pDIDevice->SetDataFormat(DataFormat)))
	{
		ErrorLog(KEM_ERROR56);
		return FALSE;
	}

	// Set the cooperative level - Foreground & Nonexclusive
	// 윈도우키 막을려면 DISCL_NOWINKEY 요 옵션 추가해부러
	if(FAILED(m_pDIDevice->SetCooperativeLevel(m_hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY )))
	{
		ErrorLog(KEM_ERROR57);
		return FALSE;
	}

	// Set the special properties if it's a joystick
	if(Type == JOYSTICK) 
	{
		// Set the special properties of the joystick - range
		DIprg.diph.dwSize       = sizeof(DIPROPRANGE);
		DIprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		DIprg.diph.dwHow        = DIPH_BYOFFSET;
		DIprg.lMin              = -1024;
		DIprg.lMax              = +1024;

		// Set X range
		DIprg.diph.dwObj = DIJOFS_X;
		if(FAILED(m_pDIDevice->SetProperty(DIPROP_RANGE, &DIprg.diph)))
		{
			ErrorLog(KEM_ERROR58);
			return FALSE;
		}

		// Set Y rangine
		DIprg.diph.dwObj = DIJOFS_Y;
		if(FAILED(m_pDIDevice->SetProperty(DIPROP_RANGE, &DIprg.diph)))
		{
			ErrorLog(KEM_ERROR59);
			return FALSE;
		}

		// Set the special properties of the joystick - deadzone 12%
		DIpdw.diph.dwSize       = sizeof(DIPROPDWORD);
		DIpdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		DIpdw.diph.dwHow        = DIPH_BYOFFSET;
		DIpdw.dwData            = 128;

		// Set X deadzone
		DIpdw.diph.dwObj = DIJOFS_X;
		if(FAILED(m_pDIDevice->SetProperty(DIPROP_DEADZONE, &DIpdw.diph)))
		{
			ErrorLog(KEM_ERROR60);
			return FALSE;
		}

		// Set Y deadzone
		DIpdw.diph.dwObj = DIJOFS_Y;
		if(FAILED(m_pDIDevice->SetProperty(DIPROP_DEADZONE, &DIpdw.diph)))
		{
			ErrorLog(KEM_ERROR61);
			return FALSE;
		}
	}

	// Acquire the device for use
	if(FAILED( hr = m_pDIDevice->Acquire()))
	{
		switch( hr )
		{
		case DIERR_INVALIDPARAM:
			//return FALSE;
			break;

		case DIERR_NOTINITIALIZED:
			//return FALSE;
			break;

		case DIERR_OTHERAPPHASPRIO:
			//return FALSE;
			break;
		}
	}
		

	// Set the device type
	m_Type = Type;

	// Clear the device information
	Clear();

	// Return a success
	return TRUE;
}
BOOL CKTDIDevice::Free()
{
#ifdef DLL_BUILD
	m_Type = NONE;
	Clear();
	return TRUE;
#endif

	// Unacquire and release the object
	if(m_pDIDevice != NULL) 
	{
		m_pDIDevice->Unacquire();
		SAFE_RELEASE(m_pDIDevice);
	}

	// Set to no device installed
	m_Type = NONE;

	// Clear the data  
	Clear();

	return TRUE;
}

BOOL CKTDIDevice::Clear()
{
	short i;

	ZeroMemory( &m_State, 256 );
#ifdef KEY_MAPPING
	ZeroMemory( &m_ActionMap, GAMEACTION_END*sizeof(unsigned char) );
#endif KEY_MAPPING

	for( i = 0; i < 256; i++ )
		m_Locks[i]  = FALSE;

	m_XPos = 0;
	m_YPos = 0;
	m_ZPos = 0;

	m_bExtraEnter = false;

	return TRUE;
}

#ifdef RELEASE_ALL_BUTTONS_AND_KEYS_WHEN_FOCUS_LOSING
BOOL CKTDIDevice::ReleaseAllButtonsAndKeys()
{
	ZeroMemory( &m_State, 256 );

	for( short i = 0; i < 256; i++ )
		m_Locks[i]  = FALSE;

	m_bExtraEnter = false;

	if( m_Type == JOYSTICK )
		m_JoystickState->rgdwPOV[0] = static_cast< DWORD >( -1 );

	return TRUE;
}
#endif

BOOL CKTDIDevice::Read( float fElapsedTime )
{
#ifdef DLL_BUILD
	return TRUE;
#endif DLL_BUILD

	HRESULT hr;
	long BufferSizes[3] = { 256, sizeof(DIMOUSESTATE), sizeof(DIJOYSTATE) };
	short i;

	// Make sure to have a valid IDirectInputDevice8 object
	if(m_pDIDevice == NULL)
	{
		return FALSE;
	}

	// Make sure device type if in range
	if(m_Type < 1 || m_Type > 3)
	{
		return FALSE;
	}

	// Loop polling and reading until succeeded or unknown error
	// Also take care of lost-focus problems
	while(1) 
	{
		// Poll
		m_pDIDevice->Poll();

		// Read in state
		if(SUCCEEDED(hr = m_pDIDevice->GetDeviceState(BufferSizes[m_Type-1], (LPVOID)&m_State)))
			break;

		// Return on an unknown error  
		if(hr != DIERR_INPUTLOST && hr != DIERR_NOTACQUIRED)
			return FALSE;

		// Reacquire and try again
		if(FAILED(m_pDIDevice->Acquire()))
		{
			return FALSE;
		}
	}

	// Since only the mouse coordinates are relative, you'll
	// have to deal with them now
	if(m_Type == MOUSE) 
	{
		// If windowed usage, ask windows for coordinates
		if( m_Windowed == TRUE ) 
		{
			POINT pt;
			GetCursorPos( &pt );
			ScreenToClient( m_hWnd, &pt );
			m_XPos = pt.x;
			m_YPos = pt.y;
			m_ZPos = m_MouseState->lZ;	// mouse wheel
		} 
		else 
		{
			m_XPos += m_MouseState->lX;
			m_YPos += m_MouseState->lY;
			m_ZPos  = m_MouseState->lZ;	// mouse wheel
		}
	}

	// Released keys and button need to be unlocked
	switch(m_Type) 
	{
		case KEYBOARD:
			for( i = 0; i < 256; i++ ) 
			{
				if( !( m_State[i] & 0x80 ) )
					m_Locks[i] = FALSE;

				if( m_DoubleKeyCheck[i].bFirstDown == true 
					&& m_DoubleKeyCheck[i].bDoubleOK == false )
				{
					m_DoubleKeyCheck[i].fTimeOut -= fElapsedTime;
					if( m_DoubleKeyCheck[i].fTimeOut < 0.0f )
					{
						m_DoubleKeyCheck[i].Init();						
					}
				}
			}
			break;

		case MOUSE:
			for( i = 0; i < 4; i++ ) 
			{
				if(!(m_MouseState->rgbButtons[i]))
					m_Locks[i] = FALSE;
			}
			break;

		case JOYSTICK:
			for( i = 0; i < 32; i++ ) 
			{
				if(!(m_JoystickState->rgbButtons[i]))
					m_Locks[i] = FALSE;
			}
			break;
	}

	// return a success
	return TRUE;
}
BOOL CKTDIDevice::Acquire( BOOL Active )
{
#ifdef DLL_BUILD
	return TRUE;
#endif DLL_BUILD
	if(m_pDIDevice == NULL)
		return FALSE;

	if(Active == TRUE)
		m_pDIDevice->Acquire();
	else
		m_pDIDevice->Unacquire();

	return TRUE;
}

BOOL CKTDIDevice::GetLock( unsigned char Num )
{
	return m_Locks[Num];
}
BOOL CKTDIDevice::SetLock( unsigned char Num, BOOL State )
{
	m_Locks[Num] = State;
	return TRUE;
}

long CKTDIDevice::GetXPos()
{
	// Update coordinates if a joystick
	if(m_Type == JOYSTICK)
		m_XPos = m_JoystickState->lX;

	return m_XPos;
}
BOOL CKTDIDevice::SetXPos( long XPos )
{
	m_XPos = XPos;
	return TRUE;
}
long CKTDIDevice::GetYPos()
{
	// Update coordinates if a joystick
	if(m_Type == JOYSTICK)
		m_YPos = m_JoystickState->lY;

	return m_YPos;
}
BOOL CKTDIDevice::SetYPos( long YPos )
{
	m_YPos = YPos;
	return TRUE;
}
long CKTDIDevice::GetZPos()
{
	// Update coordinates if a joystick
	if(m_Type == JOYSTICK)
		m_ZPos = m_JoystickState->lZ;

	return m_ZPos;
}
long CKTDIDevice::GetXDelta()
{
	switch(m_Type) 
	{
	case MOUSE:
		return m_MouseState->lX;
		//return GetMouseDeltaPosition().x;

	case JOYSTICK:
		return m_JoystickState->lX - m_XPos;

	default: 
		return 0;
	}
}
long CKTDIDevice::GetYDelta()
{
	switch(m_Type) 
	{
	case MOUSE:
		return m_MouseState->lY;
		//return GetMouseDeltaPosition().y;

	case JOYSTICK:
		return m_JoystickState->lY - m_YPos;

	default: 
		return 0;
	}
}

DWORD CKTDIDevice::GetPOV()
{
	if(m_Type != JOYSTICK)
		return static_cast<DWORD>( -1 );

	return m_JoystickState->rgdwPOV[0];
}


// Keyboard specific functions
BOOL  CKTDIDevice::GetKeyState( unsigned char Num )
{
	// Check if key/button is pressed
	if( m_State[Num] & 0x80 && m_Locks[Num] == FALSE )
	{
		m_Locks[Num] = TRUE;
		return TRUE;
	}

	return FALSE;
}
BOOL  CKTDIDevice::SetKeyState( unsigned char Num, BOOL State )
{
	m_State[Num] = State;
	return TRUE;
}
BOOL  CKTDIDevice::GetPureKeyState( unsigned char Num )
{
	return ((m_State[Num] & 0x80) ? TRUE : FALSE);
}

BOOL  CKTDIDevice::GetDoubleKeyPureState( unsigned char Num )
{
	if( m_DoubleKeyCheck[Num].bDoubleOK != true )
	{
		if( m_DoubleKeyCheck[Num].bFirstDown == false )
		{
			if( m_State[Num] & 0x80 && m_Locks[Num] == FALSE )
			{
				m_DoubleKeyCheck[Num].bFirstDown = true;
				m_DoubleKeyCheck[Num].fTimeOut = DOUBLE_INPUT_GAP;
			}
			return FALSE;

		}
		else
		{
			if( m_DoubleKeyCheck[Num].bKeyUp == false )
			{
				if( GetPureKeyState( Num ) == FALSE )
				{
					m_DoubleKeyCheck[Num].bKeyUp = true;
				}
				return FALSE;
			}
			else if( m_State[Num] & 0x80 && m_Locks[Num] == FALSE )
			{
				m_DoubleKeyCheck[Num].bDoubleOK = true;
			}
			else
				return FALSE;
		}
	}

	if( m_DoubleKeyCheck[Num].bDoubleOK == true
		&& GetPureKeyState( Num ) == TRUE )
		return TRUE;
	else
	{
		m_DoubleKeyCheck[Num].Init();
		return FALSE;
	}
}

BOOL  CKTDIDevice::GetDoubleKeyState( unsigned char Num )
{
	if( m_DoubleKeyCheck[Num].bDoubleOK != true )
	{
		if( m_DoubleKeyCheck[Num].bFirstDown == false )
		{
			if( m_State[Num] & 0x80 && m_Locks[Num] == FALSE )
			{
				m_DoubleKeyCheck[Num].bFirstDown = true;
				m_DoubleKeyCheck[Num].fTimeOut = DOUBLE_INPUT_GAP;
			}
			return FALSE;

		}
		else
		{
			if( m_DoubleKeyCheck[Num].bKeyUp == false )
			{
				if( GetPureKeyState( Num ) == FALSE )
				{
					m_DoubleKeyCheck[Num].bKeyUp = true;
				}
				return FALSE;
			}
			else if( m_State[Num] & 0x80 && m_Locks[Num] == FALSE )
			{
				m_DoubleKeyCheck[Num].bDoubleOK = true;
			}
			else
				return FALSE;
		}
	}

	if( m_DoubleKeyCheck[Num].bDoubleOK == true
		&& m_DoubleKeyCheck[Num].bDoubleKeyLock == false
		&& GetPureKeyState( Num ) == TRUE )
	{
		m_DoubleKeyCheck[Num].bDoubleKeyLock = true;
		return TRUE;
	}
	else
	{
		m_DoubleKeyCheck[Num].Init();
		return FALSE;
	}
}

void  CKTDIDevice::SetDoubleKeyReset( unsigned char Num )
{
	m_DoubleKeyCheck[Num].bDoubleOK			= false;
	m_DoubleKeyCheck[Num].bDoubleKeyLock	= false;
	m_DoubleKeyCheck[Num].bFirstDown		= false;
	m_DoubleKeyCheck[Num].bKeyUp			= false;
	m_DoubleKeyCheck[Num].fTimeOut			= 0.0f;
}

short CKTDIDevice::GetKeypress( long TimeOut )
{
#ifdef DLL_BUILD
	return 0;
#endif DLL_BUILD

	static HKL KeyboardLayout = GetKeyboardLayout(0);
	unsigned char WinKeyStates[256], DIKeyStates[256];
	unsigned short i, ScanCode, VirtualKey, Keys, Num;
	unsigned long EndTime;

	// Make sure it's a keyboard and its initialized
	if((m_Type != KEYBOARD) || (m_pDIDevice == NULL))
		return 0;

	// Calculate end time for TimeOut
	EndTime = GetTickCount() + TimeOut;

	// Loop until timeout or key pressed
	while(1) 
	{
		// Get Windows keyboard state
		GetKeyboardState( WinKeyStates );

		// Get DirectInput keyboard state
		m_pDIDevice->GetDeviceState(256, DIKeyStates);

		// Scan through looking for key presses
		for( i = 0; i < 256; i++) 
		{
			// If one found, try to convert it
			if(DIKeyStates[i] & 0x80) 
			{
				// Get virtual key code
				if((VirtualKey = MapVirtualKeyEx((ScanCode = i), 1, KeyboardLayout))) 
				{
					// Get ASCII code of key and return it
					Num = ToAsciiEx(VirtualKey, ScanCode, WinKeyStates, &Keys, 0, KeyboardLayout);
					if(Num)
						return Keys;
				}
			}
		}

		// Check for TimeOut
		if(TimeOut) 
		{
			if(GetTickCount() > EndTime)
				return 0;
		}
	}
	return 0;
}
long  CKTDIDevice::GetNumKeyPresses()
{
	long i, Num = 0;

	for(i=0;i<256;i++) 
	{
		if(m_State[i] & 0x80 && m_Locks[i] == FALSE)
			Num++;
	}

	return Num;
}
long  CKTDIDevice::GetNumPureKeyPresses()
{
	long i, Num = 0;

	for(i=0;i<256;i++) 
	{
		if(m_State[i] & 0x80)
			Num++;
	}

	return Num;
}

// Mouse/Joystick specific functions
BOOL  CKTDIDevice::GetButtonState( unsigned char Num )
{
	char State = 0;

	if(m_Type == MOUSE)
		State = m_MouseState->rgbButtons[Num];

	if(m_Type == JOYSTICK)
		State = m_JoystickState->rgbButtons[Num];

	// Check if key/button is pressed
	if(State & 0x80 && m_Locks[Num] == FALSE)
	{
		m_Locks[Num] = TRUE;		
		return TRUE;
	}

	return FALSE;
}
BOOL  CKTDIDevice::SetButtonState( unsigned char Num, BOOL State )
{
	if(m_Type == MOUSE) 
	{
		m_MouseState->rgbButtons[Num] = State;
		return TRUE;
	}

	if(m_Type == JOYSTICK) 
	{
		m_JoystickState->rgbButtons[Num] = State;
		return TRUE;
	}

	return FALSE;
}
BOOL  CKTDIDevice::GetPureButtonState( unsigned char Num )
{
	char State = 0;

	if(m_Type == MOUSE)
		State = m_MouseState->rgbButtons[Num];

	if(m_Type == JOYSTICK)
		State = m_JoystickState->rgbButtons[Num];

	if(State & 0x80)
	{
		return TRUE;
	}

	return FALSE;
}
long  CKTDIDevice::GetNumButtonPresses()
{
	long i, Num = 0;

	if(m_Type == MOUSE) 
	{
		for(i=0;i<4;i++) 
		{
			if(m_MouseState->rgbButtons[i] & 0x80 && m_Locks[i] == FALSE)
				Num++;
		}
	} 
	else
	{
		if(m_Type == JOYSTICK) 
		{
			for(i=0;i<32;i++) 
			{
				if(m_JoystickState->rgbButtons[i] & 0x80 && m_Locks[i] == FALSE)
					Num++;
			}
		}
	}

	return Num;
}
long  CKTDIDevice::GetNumPureButtonPresses()
{
	long i, Num = 0;

	if(m_Type == MOUSE) 
	{
		for(i=0;i<4;i++) 
		{
			if(m_MouseState->rgbButtons[i] & 0x80)
				Num++;
		}
	} 
	else
	{
		if(m_Type == JOYSTICK) 
		{
			for(i=0;i<32;i++) 
			{
				if(m_JoystickState->rgbButtons[i] & 0x80)
					Num++;
			}
		}
	}

	return Num;
}

BOOL FAR PASCAL CKTDIDevice::EnumJoysticks( LPCDIDEVICEINSTANCE pdInst, LPVOID pvRef )
{
	CKTDIDevice *Input;

	// Stop enumeration if no parent cInputDevice pointer
	if((Input = (CKTDIDevice*)pvRef) == NULL)
		return DIENUM_STOP;

	// Try to create a joystick interface
	if(FAILED(Input->m_pDI->CreateDevice(pdInst->guidInstance, &Input->m_pDIDevice, NULL)))
		return DIENUM_CONTINUE;

	// All done - stop enumeration
	return DIENUM_STOP;
}

void CKTDIDevice::UpdateMouseDeltaPosition()
{
	POINT pt;
	GetCursorPos( &pt );
	ScreenToClient( g_pKTDXApp->GetHWND(), &pt );

	m_kMouseDeltaPos.x = pt.x - m_kMousePrevPos.x;
	m_kMouseDeltaPos.y = pt.y - m_kMousePrevPos.y;

	m_kMousePrevPos = pt;
}

POINT CKTDIDevice::GetMouseDeltaPosition()
{
	return m_kMouseDeltaPos;
}

#ifdef KEY_MAPPING
bool CKTDIDevice::GetActionState( int Action, long* lpData )
{	
#ifdef SERV_KEY_MAPPING_INT
	if( m_Type == JOYSTICK )
	{
		switch( m_ActionMap[Action] )
		{
		case JB_UP:
			{
				if( GetYPos() < -AXIS_THRESHOLD || GetPOV() == 31500 || GetPOV() == 0 || GetPOV() == 4500 )
					return true;
			}
			return false;
		case JB_DOWN:
			{
				if( GetYPos() > AXIS_THRESHOLD || GetPOV() == 13500 || GetPOV() == 18000 || GetPOV() == 22500 )
					return true;
			}
			return false;
		case JB_LEFT:
			{
				if( GetXPos() < -AXIS_THRESHOLD || GetPOV() == 22500 || GetPOV() == 27000 || GetPOV() == 31500 )
					return true;
			}
			return false;
		case JB_RIGHT:
			{
				if( GetXPos() > AXIS_THRESHOLD || GetPOV() == 4500 || GetPOV() == 9000 || GetPOV() == 13500 )
					return true;
			}
			return false;
		case JB_LEFT_TRIGGER:
			{
				if( GetZPos() > AXIS_THRESHOLD_TRIGGER_LEFT )
					return true;
			}
			return false;
		case JB_RIGHT_TRIGGER:
			{
				if( GetZPos() < AXIS_THRESHOLD_TRIGGER_RIGHT )
					return true;
			}
			return false;
		case 0:
			return false;
		default:
			break;
		}
	}
#endif // SERV_KEY_MAPPING_INT

	return (m_State[m_ActionMap[Action]] ? true : false);
}

#ifdef KEY_MAPPING_UPGRADE
void CKTDIDevice::SetAction( int Action, unsigned char Key, bool isDefault )
{
	if(!isDefault)
	{	
		for(int i = 0; i < GAMEACTION_END; i++)
		{
			if(i == Action)
				continue;
			if(m_ActionMap[i] == Key)
			{
				switch(m_Type)
				{
				case JOYSTICK:
					m_ActionMap[i] = DIK_UP;
					break;
				case KEYBOARD : 
					m_ActionMap[i] = 0;
					break;  
				}
			}
		}
	}
	m_ActionMap[Action] = Key;
	return;
}
#else KEY_MAPPING_UPGRADE
void CKTDIDevice::SetAction( int Action, unsigned char Key )
{
	for(int i = 0; i < GAMEACTION_END; ++i)
	{
		if(i == Action)
			continue;
		if(m_ActionMap[i] == Key)
		{
			switch(m_Type)
			{
			case JOYSTICK:
				m_ActionMap[i] = DIK_UP;
				break;
			case KEYBOARD : 
				m_ActionMap[i] = 0;
				break;  
			}
		}
	}
	m_ActionMap[Action] = Key;
	return;
}
#endif KEY_MAPPING_UPGRADE

unsigned char CKTDIDevice::GetActionKey( int Action )
{
	return m_ActionMap[Action];
}

#ifdef SERV_KEY_MAPPING_INT
bool CKTDIDevice::ListenDeviceInput( unsigned char& ucoutUserInput )
{
	switch( m_Type ) 
	{
	case KEYBOARD:
		{
			for( int iKeyIndex = 0; iKeyIndex < 256; ++iKeyIndex )
			{
				if( GetKeyState( iKeyIndex ) == TRUE )
				{
					ucoutUserInput = ( unsigned char )iKeyIndex;
					return true;
				}
			}
		} return false;
	case JOYSTICK:
		{
			for( int iButtonIndex = 0; iButtonIndex < 32; ++iButtonIndex )		// 32:DIJOYSTATE의 버튼 수
			{
				if( GetButtonState( iButtonIndex ) == TRUE )
				{
					ucoutUserInput = ( unsigned char )DIJOFS_BUTTON( iButtonIndex );
					return true;
				}
			}

			if( GetZPos() > AXIS_THRESHOLD_TRIGGER_LEFT )
			{
				ucoutUserInput = (unsigned char)JB_LEFT_TRIGGER;
				return true;
			}
			if( GetZPos() < AXIS_THRESHOLD_TRIGGER_RIGHT && GetZPos() != 0 )
			{
				ucoutUserInput = (unsigned char)JB_RIGHT_TRIGGER;
				return true;
			}
			if( GetXPos() < -AXIS_THRESHOLD || GetPOV() == 22500 || GetPOV() == 27000 || GetPOV() == 31500 )
			{
				ucoutUserInput = (unsigned char)JB_LEFT;
				return true;
			}
			if( GetXPos() > AXIS_THRESHOLD || GetPOV() == 4500 || GetPOV() == 9000 || GetPOV() == 13500 )
			{
				ucoutUserInput = (unsigned char)JB_RIGHT;
				return true;
			}
			if( GetYPos() < -AXIS_THRESHOLD || GetPOV() == 31500 || GetPOV() == 0 || GetPOV() == 4500 )
			{
				ucoutUserInput = (unsigned char)JB_UP;
				return true;
			}					
			if( GetYPos() > AXIS_THRESHOLD || GetPOV() == 13500 || GetPOV() == 18000 || GetPOV() == 22500 )
			{
				ucoutUserInput = (unsigned char)JB_DOWN;
				return true;
			}
		} return false;

	default: 
		return false;
	}
}
void CKTDIDevice::GetActionKeys( OUT std::map<short, short>& mapMappingInfo )
{
	for( short sGameAction = 0; sGameAction < GAMEACTION_END; ++sGameAction )
	{
		mapMappingInfo[ sGameAction ]	= m_ActionMap[ sGameAction ];
	}
}
void CKTDIDevice::SetActionKeys( IN const std::map<short, short>& mapMappingInfo )
{
	for each( const std::pair< short, short >& pAction in mapMappingInfo )
	{
		m_ActionMap[ pAction.first ] = pAction.second;
	}
}
#endif SERV_KEY_MAPPING_INT

#endif KEY_MAPPING