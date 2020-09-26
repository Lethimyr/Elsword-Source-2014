#ifndef X2KEY_PAD_H_
#define X2KEY_PAD_H_
#pragma once

#include <boost/unordered_map.hpp>
#include <boost/bimap/bimap.hpp>

#pragma region CX2KeyPad
/** @class : CX2KeyPad
	@brief : 키설정 UI 클래스
	@date  : 2012/05/08
*/
class CX2KeyPad
{
#pragma region enum, const, typedef
public:
	enum KEY_BUTTON_INDEX
	{
		KBI_NONE,

		KBI_GRAVE = 1,		/// ~
		KBI_1,				/// 1
		KBI_2,				/// 2
		KBI_3,				/// 3
		KBI_4,				/// 4
		KBI_5,				/// 5
		KBI_6,				/// 6
		KBI_7,				/// 7
		KBI_8,				/// 8
		KBI_9,				/// 9
		KBI_0,				/// 0
		KBI_MINUS,			/// -
		KBI_EQUAL,			/// =
		KBI_BACKSPACE,		/// <--
		KBI_TAB,			/// Tab
		KBI_Q,				/// Q
		KBI_W,				/// W
		KBI_E,				/// E
		KBI_R,				/// R
		KBI_T,				/// T
		KBI_Y,				/// Y
		KBI_U,				/// U
		KBI_I,				/// I
		KBI_O,				/// O
		KBI_P,				/// P
		KBI_LBRACKET,		/// [
		KBI_RBRACKET,		/// ]
		KBI_ROOT,			/// |
		KBI_CAPITAL,		/// Caps Lock
		KBI_A,				/// A
		KBI_S,				/// S
		KBI_D,				/// D
		KBI_F,				/// F
		KBI_G,				/// G
		KBI_H,				/// H
		KBI_J,				/// J
		KBI_K,				/// K
		KBI_L,				/// L
		KBI_SEMICOLON,		/// ;
		KBI_APOSTROPHE,		/// '
		KBI_RETURN,			/// Enter
		KBI_LSHIFT,			/// Left Shift
		KBI_Z,				/// Z
		KBI_X,				/// X
		KBI_C,				/// C
		KBI_V,				/// V
		KBI_B,				/// B
		KBI_N,				/// N
		KBI_M,				/// M
		KBI_COMMA,			/// ,
		KBI_PERIOD,			/// .
		KBI_SLASH,			/// /
		KBI_RSHIFT,			/// Right Shift
		KBI_UP,				/// Up Key
		KBI_LCTRL,			/// Left Control
		KBI_LALT,			/// Left Alt
		KBI_SPACE,			/// Space
		KBI_RALT,			/// Right Alt
		KBI_RCTRL,			/// Right Control
		KBI_LEFT,			/// Left Key
		KBI_DOWN,			/// Down Key
		KBI_RIGHT,			/// Right Key
#ifdef KEYPAD_MAPPING
		KBI_NUMLOCK,		// Num Lock
		KBI_NSLASH,			// KeyPad Slash
		KBI_NMULTIPLY,		// KeyPad Multiply
		KBI_NMINUS,			// KeyPad Minus
		KBI_NPLUS,			// KeyPad Plus
		KBI_N1,				// KeyPad 1
		KBI_N2,				// KeyPad 2
		KBI_N3,				// KeyPad 3
		KBI_N4,				// KeyPad 4
		KBI_N5,				// KeyPad 5
		KBI_N6,				// KeyPad 6
		KBI_N7,				// KeyPad 7
		KBI_N8,				// KeyPad 8
		KBI_N9,				// KeyPad 9
		KBI_N0,				// KeyPad 0
		KBI_NDEL,			// KeyPad Delete
		KBI_NENTER,			// KeyPad Enter
#endif KEYPAD_MAPPING		

		KBI_END,
	};

	enum KEY_ACTION_ID
	{
		KAI_NONE,			/// 동작 없음

		KAI_QUICKSLOT1,		/// 퀵슬롯 1
		KAI_QUICKSLOT2,		/// 퀵슬롯 2
		KAI_QUICKSLOT3,		/// 퀵슬롯 3
		KAI_QUICKSLOT4,		/// 퀵슬롯 4
		KAI_QUICKSLOT5,		/// 퀵슬롯 5
		KAI_QUICKSLOT6,		/// 퀵슬롯 6

		KAI_COMMAND_FAST,	/// 커맨드 약
		KAI_COMMAND_STRONG,	/// 커맨드 강
		KAI_HYPER,			/// 각성
		KAI_CHANGE_SLOT,	/// 슬롯 체인지
		KAI_ZOOM_IN,		/// 화면 확대
		KAI_ZOOM_OUT,		/// 화면 축소
		KAI_TOGGLE_NAME,	/// 몬스터 이름 보기

		KAI_SKILLSLOT1,		/// 스킬 슬롯 1
		KAI_SKILLSLOT2,		/// 스킬 슬롯 2
		KAI_SKILLSLOT3,		/// 스킬 슬롯 3
		KAI_SKILLSLOT4,		/// 스킬 슬롯 4

		KAI_EX_SKILLSLOT1,	/// 확장 스킬 슬롯 1
		KAI_EX_SKILLSLOT2,	/// 확장 스킬 슬롯 2
		KAI_EX_SKILLSLOT3,	/// 확장 스킬 슬롯 3
		KAI_EX_SKILLSLOT4,	/// 확장 스킬 슬롯 4

		KAI_COMMUNITY_UI,	/// 커뮤니티 창
		KAI_CHARACTER_UI,	/// 캐릭터 창
		KAI_INVENTORY_UI,	/// 인벤토리 창
		KAI_OPTION_UI,		/// 옵션 창
		KAI_PET_UI,			/// 펫 창
		KAI_PVE_UI,			/// 던전 창
		KAI_PVP_UI,			/// 대전 창
		KAI_SKILL_UI,		/// 스킬 창
		KAI_QUEST_UI,		/// 퀘스트 창
		KAI_MAP_UI,			/// 월드맵 창
		KAI_GUILD_UI,		/// 길드 창

#ifdef RIDING_SYSTEM
		KAI_RINDING,		/// 라이딩 펫 타기
#endif //RIDING_SYSTEM

		KAI_END,
	};

	enum KEY_PAD_UI_MSG
	{
		KPUIM_EXIT		= 53000,	/// 종료
		KPUIM_DEFAULT,				/// 초기화
		KPUIM_CLEAR,				/// 모두 없애기
		KPUIM_PREV,					/// 원래대로
		KPUIM_SAVE,					/// 저장
		KPUIM_CANCEL,				/// 취소

		KPUIM_USESLOT_DROPPED,		/// 드래그 & 드롭
		KPUIM_USESLOT_MOUSEIN,		/// 마우스 인 포커스
		KPUIM_USESLOT_MOUSEOUT,		/// 마우스 아웃 포커스
	};

	typedef boost::unordered_map< GAME_ACTION, int > KeyMap;								/// 액션 키 boost::map
	typedef boost::unordered_map< unsigned char, wstring > KeyStringMap;					/// Key에 맞는 String, boost::map
	typedef boost::bimaps::bimap< unsigned char, KEY_BUTTON_INDEX > DeviceKeyMap;			/// DeviceKey에 맞는 Pad Slot Index, Boost::bimap
	typedef boost::bimaps::bimap< unsigned char, KEY_BUTTON_INDEX >::value_type value_t;	/// DeviceKeyMap의 값에 접근하기 위한 Value_type
#pragma endregion 열거체, 상수, 사용자 타입

#pragma region public function
public:
	CX2KeyPad(void);
	~CX2KeyPad(void);

	void ShowKeyPad( IN bool bShow_ );
	bool GetShowKeyPad() { return m_bShowKeyPad; };
	void LoadSlotData();
	void SaveSlotData( bool bfirst = false );
	void InitKeySlotData();
#ifdef STRING_KEYMARK
	wstring GetKeyString ( int iKeyIndex );
#endif STRING_KEYMARK
	wstring GetKeyString ( GAME_ACTION eActionKey_ );

	bool MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#pragma endregion 함수

#pragma region private function
private:
	void ClearSlotData();
	void UpdateSlotData();
	void DefaultSlotData();
	void InitUseKeySlotData();
	void InitDisUseKeySlotData();
	bool ProcessUseSlotDropped( IN LPARAM lParam_ );
	void UpdateEtcString();
	
	bool Handler_EGS_KEYBOARD_MAPPING_INFO_WRITE_REQ();
	bool Handler_EGS_KEYBOARD_MAPPING_INFO_WRITE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#pragma endregion 함수

#pragma region public variable
public:
	static const int USE_KEY_NUMBER;				/// 사용되는 키의 수
	static const int DISUSE_KEY_NUMBER;				/// 사용되지 않는 키의 수

#ifdef KEYPAD_MAPPING
	static const unsigned char USE_DIK_LIST[KBI_END];	/// DirectX Input Keyboard에 정의된 것 중 게임에 사용하는 키 나열
#else KEYPAD_MAPPING
	static const unsigned char USE_DIK_LIST[63];	/// DirectX Input Keyboard에 정의된 것 중 게임에 사용하는 키 나열
#endif KEYPAD_MAPPING

#ifdef KEY_MAPPING_USE_STRING_TABLE
	static const int KEYPAD_STRING_INDEX[KBI_END];	/// KeyPad String Index
#else KEY_MAPPING_USE_STRING_TABLE
	static const wstring KEYPAD_STRING[KBI_END];	/// KeyPad String
#endif KEY_MAPPING_USE_STRING_TABLE
#pragma endregion 변수

#pragma region private varable
private:
	CKTDGUIDialogType	m_DLGKeyPad;		/// 키 변경 다이얼로그
	KeyMap				m_KeyPadMap;		/// 키 액션 값과 슬롯 인덱스 값을 가짐
	DeviceKeyMap		m_DeviceKeyMap;		/// DIK 키 값과 슬롯 키 패드 상의 슬롯 값을 가짐
	KeyStringMap		m_KeyStringMap;		/// Key Device에 맞는 String 값으로 가짐

	bool				m_bShowKeyPad;		/// KeyPad 활성화 여부 확인
#pragma endregion 변수
};
#pragma endregion 클래스

#pragma region CX2UseKeySlotData
class CX2UseKeySlotData : public CKTDGUISlotData
{
#pragma region public function
public:
	CX2UseKeySlotData();
	CX2UseKeySlotData( IN int index_ );
	virtual ~CX2UseKeySlotData();

	virtual void UpdateUI( IN CKTDGUISlot* pSlot_ );
	void Clear( IN int index_ = -1 );
#pragma endregion 함수

#pragma region public variable
public:
	int	m_iIndex;					/// KeyPad Slot Index
	GAME_ACTION m_eKeyActionID;		/// Key Action ID
#pragma endregion 변수
};
#pragma endregion 클래스

#pragma region CX2DisUseKeySlotData
class CX2DisUseKeySlotData : public CKTDGUISlotData
{
#pragma region public function
public:
	CX2DisUseKeySlotData();
	CX2DisUseKeySlotData( IN int index_ );
	virtual ~CX2DisUseKeySlotData();

	virtual void UpdateUI( IN CKTDGUISlot* pSlot_ );
	void Clear( IN int index_ = -1 );
#pragma endregion 함수
	
#pragma region public variable
public:
	int	m_iIndex;					/// KeyPad Slot Index
	GAME_ACTION m_eKeyActionID;		/// Key Action ID
#pragma endregion 변수
};
#pragma endregion 클래스


#endif