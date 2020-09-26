g_pUIDialog:SetName( "DLG_Test_Back" )
g_pUIDialog:SetPos( 0,0 )
g_pUIDialog:SetModal( true )
g_pUIDialog:SetFront( false )

FALSE = 0
TRUE = 1


STATE_TEST_UI_CUSTOM_MSG =
{
	STUCM_RADIO1SET		= 0,
	STUCM_RADIO2SET		= 1,
	STUCM_RADIO3SET		= 2,

	STUCM_CHECK1SET		= 11,
	STUCM_CHECK2SET		= 12,
	STUCM_CHECK3SET		= 13,
	STUCM_CHECK4SET		= 14,

	STUCM_CHECK1UNSET	= 21, 
	STUCM_CHECK2UNSET	= 22,
	STUCM_CHECK3UNSET	= 23,
	STUCM_CHECK4UNSET	= 24,

	STUCM_IME		= 30,

	STUCM_BUTTONON		= 40,

	STUCM_COMBO_SELECT	= 50,

	STUCM_LIST_SELECT	= 60,

}


XF_DODUM_15_NORMAL		= 0
XUF_DODUM_13_SEMIBOLD		= 1

g_pFontManager:CreateNewUIFont( XF_DODUM_15_NORMAL, "2002L_KOG", 16, 400 )
g_pFontManager:CreateNewUIUKFont( XUF_DODUM_13_SEMIBOLD,"2002L_KOG", 14, 400, 1	)


-- 그림 하나

g_pStaticTest = g_pUIDialog:CreateStatic()
g_pUIDialog:AddControl( g_pStaticTest )
g_pStaticTest:SetName( "Test_Back_Pic1" )

g_pPictureTestBG1 = g_pUIDialog:CreatePicture()
g_pStaticTest:AddPicture( g_pPictureTestBG1 )

g_pPictureTestBG1:SetTex( "DLG_Login0.TGA", "어허" )

g_pPictureTestBG1:SetPoint
{
	USE_TEXTURE_SIZE = FALSE,
	IS_RECT = FALSE,

	"LEFT_TOP		= D3DXVECTOR2(0,0)",
	"RIGHT_TOP		= D3DXVECTOR2(512,0)",
	"LEFT_BOTTOM		= D3DXVECTOR2(0,512)",
	"RIGHT_BOTTOM		= D3DXVECTOR2(512,512)",
	"COLOR			= D3DXCOLOR(1.0,1.0,1.0,1.0)",
	CHANGE_TIME		= 0.0,
}


-- 그림 둘

g_pStaticTest2 = g_pUIDialog:CreateStatic()
g_pUIDialog:AddControl( g_pStaticTest2 )
g_pStaticTest2:SetName( "Test_Back_Pic2" )

g_pPictureTestBG2 = g_pUIDialog:CreatePicture()
g_pStaticTest2:AddPicture( g_pPictureTestBG2 )

g_pPictureTestBG2:SetTex( "DLG_Login1.TGA", "어허1" )

g_pPictureTestBG2:SetPoint
{
	"LEFT_TOP		= D3DXVECTOR2(512,0)",
	"RIGHT_TOP		= D3DXVECTOR2(1024,0)",
	"LEFT_BOTTOM		= D3DXVECTOR2(512,512)",
	"RIGHT_BOTTOM		= D3DXVECTOR2(1024,512)",
	"COLOR			= D3DXCOLOR(1.0,1.0,1.0,1.0)",
	CHANGE_TIME		= 0.0,
}



-- 그림 셋

g_pStaticTest3 = g_pUIDialog:CreateStatic()
g_pUIDialog:AddControl( g_pStaticTest3 )
g_pStaticTest3:SetName( "Test_Back_Pic3" )

g_pPictureTestBG3 = g_pUIDialog:CreatePicture()
g_pStaticTest3:AddPicture( g_pPictureTestBG3 )

g_pPictureTestBG3:SetTex( "DLG_Login2.TGA", "조각" )

g_pPictureTestBG3:SetPoint
{
	"LEFT_TOP		= D3DXVECTOR2(0,512)",
	"RIGHT_TOP		= D3DXVECTOR2(512,512)",
	"LEFT_BOTTOM		= D3DXVECTOR2(0,768)",
	"RIGHT_BOTTOM		= D3DXVECTOR2(512,768)",
	"COLOR			= D3DXCOLOR(1.0,1.0,1.0,1.0)",
	CHANGE_TIME		= 0.0,
}


-- 그림 넷

g_pStaticTest4 = g_pUIDialog:CreateStatic()
g_pUIDialog:AddControl( g_pStaticTest4 )
g_pStaticTest4:SetName( "Test_Back_Pic4" )

g_pPictureTestBG4 = g_pUIDialog:CreatePicture()
g_pStaticTest4:AddPicture( g_pPictureTestBG4 )

g_pPictureTestBG4:SetTex( "DLG_Login2.TGA", "조각1" )

g_pPictureTestBG4:SetPoint
{
	"LEFT_TOP		= D3DXVECTOR2(512,512)",
	"RIGHT_TOP		= D3DXVECTOR2(1024,512)",
	"LEFT_BOTTOM		= D3DXVECTOR2(512,768)",
	"RIGHT_BOTTOM		= D3DXVECTOR2(1024,768)",
	"COLOR			= D3DXCOLOR(1.0,1.0,1.0,1.0)",
	CHANGE_TIME		= 0.0,
}


-- 로긴버튼

g_pButtonLogin = g_pUIDialog:CreateButton()
g_pUIDialog:AddControl( g_pButtonLogin )
g_pButtonLogin:SetName( "ButtonLogin" )
g_pButtonLogin:SetEnableLeftTop( D3DXVECTOR2(194,566) )
g_pButtonLogin:SetEnableRightBottom( D3DXVECTOR2(293,614) )
g_pButtonLogin:SetNormalTex( "DLG_Login3.tga", "로긴버튼 텍스쳐" )
g_pButtonLogin:SetOverTex( "DLG_Login3.tga", "로긴버튼 텍스쳐" )
g_pButtonLogin:SetDownTex( "DLG_Login3.tga", "로긴버튼 텍스쳐" )

g_pButtonLogin:SetNormalPoint
{
	USE_TEXTURE_SIZE = FALSE,
	IS_RECT = FALSE,

	"LEFT_TOP		= D3DXVECTOR2(194,566)",
	"RIGHT_TOP		= D3DXVECTOR2(293,566)",
	"LEFT_BOTTOM		= D3DXVECTOR2(194,614)",
	"RIGHT_BOTTOM		= D3DXVECTOR2(293,614)",
	"COLOR			= D3DXCOLOR(1.0,1.0,1.0,1.0)",
	CHANGE_TIME		= 0.0,
}
g_pButtonLogin:SetOverPoint
{
    USE_TEXTURE_SIZE = FALSE,
	IS_RECT = FALSE,
	
	"LEFT_TOP		= D3DXVECTOR2(194,566)",
	"RIGHT_TOP		= D3DXVECTOR2(293,566)",
	"LEFT_BOTTOM		= D3DXVECTOR2(194,614)",
	"RIGHT_BOTTOM		= D3DXVECTOR2(293,614)",
	"COLOR			= D3DXCOLOR(1.0,1.0,1.0,1.0)",
	CHANGE_TIME		= 0.0,
}
g_pButtonLogin:SetDownPoint
{
	USE_TEXTURE_SIZE = FALSE,
	IS_RECT = FALSE,

	"LEFT_TOP		= D3DXVECTOR2(197,569)",
	"RIGHT_TOP		= D3DXVECTOR2(296,569)",
	"LEFT_BOTTOM		= D3DXVECTOR2(197,617)",
	"RIGHT_BOTTOM		= D3DXVECTOR2(296,617)",
	"COLOR			= D3DXCOLOR(1.0,1.0,1.0,1.0)",
	CHANGE_TIME		= 0.0,
}
g_pButtonLogin:SetCustomMsgMouseUp( STATE_TEST_UI_CUSTOM_MSG["STUCM_BUTTONON"] )




-- IME상자

g_pIMETest = g_pUIDialog:CreateIMEEditBox()
g_pUIDialog:AddControl( g_pIMETest )

g_pIMETest:SetName( "IMETest" )
g_pIMETest:SetTextPoint
{
	USE_TEXTURE_SIZE= FALSE,
	"LEFT_TOP		= D3DXVECTOR2(250,506)",
	"RIGHT_BOTTOM		= D3DXVECTOR2(388,524)",
	"COLOR			= D3DXCOLOR(1.0,1.0,1.0,1.0)",
	CHANGE_TIME		= 0.0,
}
g_pIMETest:SetFont( XF_DODUM_15_NORMAL )

g_pIMETest:SetTextColor( 1.0, 1.0, 1.0, 1.0 )
g_pIMETest:SetSelectedTextColor( 0.0, 0.0, 0.0, 1.0 )
g_pIMETest:SetSelectedBackColor( 1.0, 0.0, 0.0, 1.0 )
g_pIMETest:SetCaretColor( 1.0, 1.0, 1.0, 1.0 )

g_pIMETest:SetTabControlName( "EditBoxLoginPassword" )

--g_pIMETest:SetCustomMsgEnter( STATE_TEST_UI_CUSTOM_MSG["STUCM_IME"] )


g_pEditBoxLoginPassword = g_pUIDialog:CreateEditBox()
g_pUIDialog:AddControl( g_pEditBoxLoginPassword )
g_pEditBoxLoginPassword:SetName( "EditBoxLoginPassword" )
g_pEditBoxLoginPassword:SetTextPoint
{
	USE_TEXTURE_SIZE= FALSE,
	"LEFT_TOP		= D3DXVECTOR2(250,535)",
	"RIGHT_BOTTOM		= D3DXVECTOR2(388,553)",
	"COLOR			= D3DXCOLOR(1.0,1.0,1.0,1.0)",
	CHANGE_TIME		= 0.0,
}
g_pEditBoxLoginPassword:SetFont( XF_DODUM_15_NORMAL )
g_pEditBoxLoginPassword:SetTextColor( 1.0, 1.0, 1.0, 1.0 )
g_pEditBoxLoginPassword:SetSelectedTextColor( 0.0, 0.0, 0.0, 1.0 )
g_pEditBoxLoginPassword:SetSelectedBackColor( 1.0, 0.0, 0.0, 1.0 )
g_pEditBoxLoginPassword:SetCaretColor( 1.0, 1.0, 1.0, 1.0 )
g_pEditBoxLoginPassword:SetTabControlName( "IMETest" )
g_pEditBoxLoginPassword:SetHideChar( "*" )
--g_pEditBoxLoginPassword:SetCustomMsgEnter( STATE_LOGIN_UI_CUSTOM_MSG["SLUCM_LOGIN"] )





-- 어디서 훔쳐온 콤보박스

g_pComboBoxOption_Window_Graphic = g_pUIDialog:CreateComboBox()
g_pUIDialog:AddControl( g_pComboBoxOption_Window_Graphic )
g_pComboBoxOption_Window_Graphic:SetName( "Option_Resolution" )

--거꾸로 나오게 할것인가?
--g_pComboBoxOption_Window_Graphic:SetReverseDropdown( TRUE )

--버튼 클릭했을 때 드랍 텍스쳐 높이
--g_pComboBoxOption_Window_Graphic:SetDropHeight( 200 )


g_pComboBoxOption_Window_Graphic:SetString
{
	MSG 			= "NoName",
 	 FONT_INDEX      = XUF_DODUM_13_SEMIBOLD,
	"POS 			= D3DXVECTOR2(0,0)",
	"COLOR			= D3DXCOLOR(0.0,0.0,0.0,1.0)",
	"OUTLINE_COLOR	= D3DXCOLOR(0.0,0.0,0.0,1.0)",
}

g_pComboBoxOption_Window_Graphic:SetTextTex( "DLG_Common_Button00.tga", "invisible" )


g_pComboBoxOption_Window_Graphic:SetButtonTex ( "DLG_Common_Texture09.tga", "Arrow_Down_Normal" )


g_pComboBoxOption_Window_Graphic:SetButtonMouseOverTex( "DLG_Common_Texture09.tga", "Arrow_Down_Over" )


g_pComboBoxOption_Window_Graphic:SetButtonMouseDownTex ( "DLG_Common_Texture09.tga", "Arrow_Down_Over" )


g_pComboBoxOption_Window_Graphic:SetDropdownTex ( "DLG_Common_Texture01.tga", "White" )


g_pComboBoxOption_Window_Graphic:SetDropdownTextTex ( "DLG_Common_Texture09.tga", "Black" )


g_pComboBoxOption_Window_Graphic:SetScollBarUpButtonTex( "DLG_Common_New_Texture11.tga", "Scroll_Up_Over" )

g_pComboBoxOption_Window_Graphic:SetScollBarUpButtonDownTex( "DLG_Common_New_Texture11.tga", "Scroll_Up_Normal" )

g_pComboBoxOption_Window_Graphic:SetScollBarDownButtonTex( "DLG_Common_New_Texture11.tga", "Scroll_Down_Over" )

g_pComboBoxOption_Window_Graphic:SetScollBarDownButtonDownTex( "DLG_Common_New_Texture11.tga", "Scroll_Down_Normal" )

g_pComboBoxOption_Window_Graphic:SetScollBarThumbTex( "DLG_Common_New_Texture02.dds", "Scroll" )

g_pComboBoxOption_Window_Graphic:SetScollBarTrackTex( "DLG_Common_New_Texture11.tga", "Messenger_1Pix_Deepgrey" )

g_pComboBoxOption_Window_Graphic:InitScrollBar()


--자동으로 만들 때
--g_pComboBoxOption_Window_Graphic:SetPos( 200, 129 )
--g_pComboBoxOption_Window_Graphic:SetSize( 100, 50 )

--수동으로 만들 때
g_pComboBoxOption_Window_Graphic:SetTextPos( 475, 454)
g_pComboBoxOption_Window_Graphic:SetTextSize( 170, 25)
--텍스트 시작 포인트
g_pComboBoxOption_Window_Graphic:SetTextStringOffsetPos( 64, 6 )
g_pComboBoxOption_Window_Graphic:SetButtonPos( 645, 453)
g_pComboBoxOption_Window_Graphic:SetButtonSize( 26, 26)
g_pComboBoxOption_Window_Graphic:SetCustomMsgSelectionChanged( STATE_TEST_UI_CUSTOM_MSG["STUCM_COMBO_SELECT"] )


--리스트박스

g_pListBox = g_pUIDialog:CreateListBox()
g_pUIDialog:AddControl( g_pListBox )
g_pListBox:SetName( "ListBox_Test" )
--g_pListBox:SetIgnoreListClick( true )

g_pListBox:SetString	
{
	MSG 			= "NoName",
	FONT_INDEX		= XUF_DODUM_13_SEMIBOLD,
	"POS 			= D3DXVECTOR2(0,0)",
	"COLOR			= D3DXCOLOR(1.0,1.0,1.0,1.0)",
	"OUTLINE_COLOR	= D3DXCOLOR(0.0,0.0,0.0,1.0)",
}

g_pListBox:SetTextTex( "DLG_Common_Texture00.tga", "Background_Lena_Color" )

g_pListBox:SetBoundingBoxTex( "DLG_Room_Button0.tga","Select_Box" )

g_pListBox:SetScollBarUpButtonTex( "DLG_SquareUI.tga", "Up_Arrow_Yllow" )

g_pListBox:SetScollBarUpButtonDownTex( "DLG_SquareUI.tga", "Up_Arrow_Red" )

g_pListBox:SetScollBarDownButtonTex( "DLG_SquareUI.tga", "Down_Arrow_Yllow" )

g_pListBox:SetScollBarDownButtonDownTex( "DLG_SquareUI.tga", "Down_Arrow_Red" )

g_pListBox:SetScollBarThumbTex( "DLG_Room_Button0.tga", "Thumb_Tex" )

g_pListBox:SetScollBarTrackTex( "DLG_Room_Button0.tga", "Invisible" )




g_pListBox:InitScrollBar()
g_pListBox:SetScrollBarWidth( 10 )
g_pListBox:SetPos( 0, 50 )
g_pListBox:SetSize( 400, 400 )

g_pListBox:SetCustomMsgItemDoubleClick( STATE_TEST_UI_CUSTOM_MSG["STUCM_LIST_SELECT"] )

