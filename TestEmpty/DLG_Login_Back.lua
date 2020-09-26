
g_pUIDialog:SetName( "DLG_Login_Back" )
g_pUIDialog:SetPos( 0,0 )
g_pUIDialog:SetModal( true )
g_pUIDialog:SetFront( false )



--[[
XF_DODUM_15_NORMAL		= 0
XUF_DODUM_13_SEMIBOLD		= 1

g_pFontManager:CreateNewUIFont( XF_DODUM_15_NORMAL, "2002L_KOG", 16, 400 )
g_pFontManager:CreateNewUIUKFont( XUF_DODUM_13_SEMIBOLD,"2002L_KOG", 14, 400, 1	)
--]]

g_pStaticLogin = g_pUIDialog:CreateStatic()
g_pUIDialog:AddControl( g_pStaticLogin )
g_pStaticLogin:SetName( "Login" )

-- 배경 그림
g_pPictureLoginBG1 = g_pUIDialog:CreatePicture()
g_pStaticLogin:AddPicture( g_pPictureLoginBG1 )
g_pPictureLoginBG1:SetTex( "DLG_Login0.TGA", "어허" )

g_pPictureLoginBG1:SetPoint
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

g_pPictureLoginBG2 = g_pUIDialog:CreatePicture()
g_pStaticLogin:AddPicture( g_pPictureLoginBG2 )

g_pPictureLoginBG2:SetTex( "DLG_Login1.TGA", "어허1" )

g_pPictureLoginBG2:SetPoint
{
	"LEFT_TOP		= D3DXVECTOR2(512,0)",
	"RIGHT_TOP		= D3DXVECTOR2(1024,0)",
	"LEFT_BOTTOM		= D3DXVECTOR2(512,512)",
	"RIGHT_BOTTOM		= D3DXVECTOR2(1024,512)",
	"COLOR			= D3DXCOLOR(1.0,1.0,1.0,1.0)",
	CHANGE_TIME		= 0.0,
}

g_pPictureLoginBG3 = g_pUIDialog:CreatePicture()
g_pStaticLogin:AddPicture( g_pPictureLoginBG3 )

g_pPictureLoginBG3:SetTex( "DLG_Login2.TGA", "조각" )

g_pPictureLoginBG3:SetPoint
{
	"LEFT_TOP		= D3DXVECTOR2(0,512)",
	"RIGHT_TOP		= D3DXVECTOR2(512,512)",
	"LEFT_BOTTOM		= D3DXVECTOR2(0,768)",
	"RIGHT_BOTTOM		= D3DXVECTOR2(512,768)",
	"COLOR			= D3DXCOLOR(1.0,1.0,1.0,1.0)",
	CHANGE_TIME		= 0.0,
}

g_pPictureLoginBG4 = g_pUIDialog:CreatePicture()
g_pStaticLogin:AddPicture( g_pPictureLoginBG4 )

g_pPictureLoginBG4:SetTex( "DLG_Login2.TGA", "조각1" )

g_pPictureLoginBG4:SetPoint
{
	"LEFT_TOP		= D3DXVECTOR2(512,512)",
	"RIGHT_TOP		= D3DXVECTOR2(1024,512)",
	"LEFT_BOTTOM		= D3DXVECTOR2(512,768)",
	"RIGHT_BOTTOM		= D3DXVECTOR2(1024,768)",
	"COLOR			= D3DXCOLOR(1.0,1.0,1.0,1.0)",
	CHANGE_TIME		= 0.0,
}

-- 로그인 버튼
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
--g_pButtonLogin:SetCustomMsgMouseUp( STATE_TEST_UI_CUSTOM_MSG["STUCM_BUTTONON"] )

-- 회원가입 버튼
g_pButtonJoin = g_pUIDialog:CreateButton()
g_pUIDialog:AddControl( g_pButtonJoin )
g_pButtonJoin:SetName( "ButtonJoin" )
g_pButtonJoin:SetNormalTex( "DLG_Login3.tga", "조인버튼 텍스쳐" )

g_pButtonJoin:SetOverTex( "DLG_Login3.tga", "조인버튼 텍스쳐" )

g_pButtonJoin:SetDownTex( "DLG_Login3.tga", "조인버튼 텍스쳐" )

g_pButtonJoin:SetNormalPoint
{
	"LEFT_TOP		= D3DXVECTOR2(299,566)",
	"RIGHT_TOP		= D3DXVECTOR2(398,566)",
	"LEFT_BOTTOM		= D3DXVECTOR2(299,614)",
	"RIGHT_BOTTOM		= D3DXVECTOR2(398,614)",
	"COLOR			= D3DXCOLOR(1.0,1.0,1.0,1.0)",
	CHANGE_TIME		= 0.0,
}
g_pButtonJoin:SetOverPoint
{
	"LEFT_TOP		= D3DXVECTOR2(299,566)",
	"RIGHT_TOP		= D3DXVECTOR2(398,566)",
	"LEFT_BOTTOM		= D3DXVECTOR2(299,614)",
	"RIGHT_BOTTOM		= D3DXVECTOR2(398,614)",
	"COLOR			= D3DXCOLOR(1.0,1.0,1.0,1.0)",
	CHANGE_TIME		= 0.0,
}
g_pButtonJoin:SetDownPoint
{
	"LEFT_TOP		= D3DXVECTOR2(302,569)",
	"RIGHT_TOP		= D3DXVECTOR2(401,569)",
	"LEFT_BOTTOM		= D3DXVECTOR2(302,617)",
	"RIGHT_BOTTOM		= D3DXVECTOR2(401,617)",
	"COLOR			= D3DXCOLOR(1.0,1.0,1.0,1.0)",
	CHANGE_TIME		= 0.0,
}
--g_pButtonJoin:SetCustomMsgMouseUp( STATE_TEST_UI_CUSTOM_MSG["STUCM_BUTTONON"] )

g_pCheckBoxAutoLogin = g_pUIDialog:CreateCheckBox()
g_pCheckBoxAutoLogin:SetName( "CheckBoxAutoLogin" )
g_pUIDialog:AddControl( g_pCheckBoxAutoLogin )

g_pCheckBoxAutoLogin:SetNormalTex( "DLG_Login3.TGA", "오토로긴 노멀" )
g_pCheckBoxAutoLogin:SetCheckedTex( "DLG_Login3.TGA", "오토로긴 체크" )


g_pCheckBoxAutoLogin:SetBGPoint
{
	"LEFT_TOP		= D3DXVECTOR2(25,711)",
	"RIGHT_TOP		= D3DXVECTOR2(57,711)",
	"LEFT_BOTTOM		= D3DXVECTOR2(25,743)",
	"RIGHT_BOTTOM		= D3DXVECTOR2(57,743)",
	"COLOR			= D3DXCOLOR(1.0,1.0,1.0,1.0)",
	CHANGE_TIME		= 0.0,
}
g_pCheckBoxAutoLogin:SetCheckPoint
{
	"LEFT_TOP		= D3DXVECTOR2(25,711)",
	"RIGHT_TOP		= D3DXVECTOR2(57,711)",
	"LEFT_BOTTOM		= D3DXVECTOR2(25,743)",
	"RIGHT_BOTTOM		= D3DXVECTOR2(57,743)",
	"COLOR			= D3DXCOLOR(1.0,1.0,1.0,1.0)",
	CHANGE_TIME		= 0.0,

}

g_pCheckBoxAutoLogin:SetCustomMsgChecked( STATE_LOGIN_UI_CUSTOM_MSG["SLUCM_AUTO_LOGIN"] )
g_pCheckBoxAutoLogin:SetCustomMsgUnChecked( STATE_LOGIN_UI_CUSTOM_MSG["SLUCM_NOT_AUTO_LOGIN"] )
--[[
g_pButtonLogin:SetNormalAnimTex
{
	FileName		= "DLG_Login3.tga",
	KeyName			= "로긴버튼 텍스쳐",
	
	RotateAxis		= TEX_AXIS["TA_CENTER_CENTER"],
	RotateDegree	= 99999 * 90,
	"POS			= D3DXVECTOR2(0,0)",
	SequenceID		= 0,
	fShowTime		= 99999,
}
--]]

--g_pButtonLogin:SetNormalTex( "DLG_Room_Button0.tga", "Invisible" )
--g_pButtonLogin:SetNormalStatic( g_pStaticQuest_Popup_Window )





--[[	
g_pButtonLogin:SetOverAnimTex
{
	FileName		= "DLG_Login3.tga",
	KeyName			= "조인버튼 텍스쳐",
	
	RotateAxis		= TEX_AXIS["TA_CENTER_CENTER"],
	RotateDegree	= 180,
	"POS			= D3DXVECTOR2(0,0)",
	SequenceID		= 0,
	fShowTime		= 0.4,
}

g_pButtonLogin:SetOverAnimTex
{
	FileName		= "DLG_Login3.tga",
	KeyName			= "로긴버튼 텍스쳐",
	
	RotateDegree	= 170,
	"POS			= D3DXVECTOR2(0,0)",
	SequenceID		= 1,
	fShowTime		= 0.15,
}

g_pButtonLogin:SetOverAnimTex
{
	RotateDegree	= 180,
	"POS			= D3DXVECTOR2(40,0)",
	SequenceID		= 2,
	fShowTime		= 0.15,
}

g_pButtonLogin:SetOverAnimTex
{
	RotateDegree	= 180,
	"POS			= D3DXVECTOR2(-20,0)",
	SequenceID		= 3,
	fShowTime		= 0.15,
}

g_pButtonLogin:SetOverAnimTex
{
	RotateDegree	= 0,
	"POS			= D3DXVECTOR2(0,0)",
	SequenceID		= 4,
	fShowTime		= 0.15,
}

g_pButtonLogin:SetOverAnimTex
{
	"POS			= D3DXVECTOR2(0,0)",
	SequenceID		= 5,
	fShowTime		= 0.15,
	ScaleAxis		= TEX_AXIS["TA_BOTTOM_CENTER"],
	"SCALE			= D3DXVECTOR2(2,2)",
}

g_pButtonLogin:SetOverAnimTex
{
	"POS			= D3DXVECTOR2(0,0)",
	SequenceID		= 6,
	fShowTime		= 0.15,
	ScaleAxis		= TEX_AXIS["TA_BOTTOM_CENTER"],
	"SCALE			= D3DXVECTOR2(1,1)",
}

g_pButtonLogin:SetOverAnimTex
{
	RotateAxis		= TEX_AXIS["TA_BOTTOM_LEFT"],
	RotateDegree	= -180,

	"POS			= D3DXVECTOR2(0,0)",
	SequenceID		= 7,
	fShowTime		= 1.15,
}

g_pButtonLogin:SetOverAnimTex
{
	RotateAxis		= TEX_AXIS["TA_BOTTOM_LEFT"],
	RotateDegree	= 0,

	"POS			= D3DXVECTOR2(0,0)",
	SequenceID		= 8,
	fShowTime		= 0.65,
	
}

g_pButtonLogin:SetOverAnimTex
{
	RotateAxis		= TEX_AXIS["TA_BOTTOM_RIGHT"],
	RotateDegree	= 180,
	
	ScaleAxis		= TEX_AXIS["TA_BOTTOM_RIGHT"],
	"SCALE			= D3DXVECTOR2(1,4)",

	"POS			= D3DXVECTOR2(0,0)",
	SequenceID		= 9,
	fShowTime		= 1.15,
}

g_pButtonLogin:SetOverAnimTex
{
	RotateAxis		= TEX_AXIS["TA_BOTTOM_RIGHT"],
	RotateDegree	= 0,

	"POS			= D3DXVECTOR2(0,0)",
	SequenceID		= 10,
	fShowTime		= 0.65,

}

g_pButtonLogin:SetOverAnimTex
{
	ScaleAxis		= TEX_AXIS["TA_CENTER_CENTER"],
	"SCALE			= D3DXVECTOR2(1,4)",
	ScaleRotaionDegree = 360,

	"POS			= D3DXVECTOR2(0,0)",
	SequenceID		= 11,
	fShowTime		= 0.65,
}

g_pButtonLogin:SetOverAnimTex
{	
	ScaleAxis		= TEX_AXIS["TA_CENTER_CENTER"],
	"SCALE			= D3DXVECTOR2(1,1)",
	ScaleRotaionDegree = 0,
	
	"POS			= D3DXVECTOR2(0,0)",
	SequenceID		= 12,
	fShowTime		= 0.65,
	
	bEndTexChange	= TRUE,
}

--]]



g_pIMELoginID = g_pUIDialog:CreateIMEEditBox()
g_pUIDialog:AddControl( g_pIMELoginID )
g_pIMELoginID:SetName( "IMELoginID" )
g_pIMELoginID:SetTextPoint
{
	USE_TEXTURE_SIZE= FALSE,
	"LEFT_TOP		= D3DXVECTOR2(250,506)",
	"RIGHT_BOTTOM		= D3DXVECTOR2(388,524)",
	"COLOR			= D3DXCOLOR(1.0,1.0,1.0,1.0)",
	CHANGE_TIME		= 0.0,
}
g_pIMELoginID:SetFont( XF_DODUM_15_NORMAL )
g_pIMELoginID:SetTextColor( 1.0, 1.0, 1.0, 1.0 )
g_pIMELoginID:SetSelectedTextColor( 0.0, 0.0, 0.0, 1.0 )
g_pIMELoginID:SetSelectedBackColor( 1.0, 0.0, 0.0, 1.0 )
g_pIMELoginID:SetCaretColor( 1.0, 1.0, 1.0, 1.0 )
g_pIMELoginID:SetTabControlName( "EditBoxLoginPassword" )
--g_pIMELoginID:SetDefTextAlign( DRAW_TEXT["DT_CENTER"] )
--g_pIMELoginID:SetCustomMsgEnter( STATE_LOGIN_UI_CUSTOM_MSG["SLUCM_NEXON_KOREA_LOGIN"] )
--[[
	g_pIMELoginID:SetWidthLimit( 60 )
	g_pIMELoginID:SetMultiLineOption( 1, 1 )

	g_pIMELoginID = g_pUIDialog:CreateIMEEditBox()
	g_pUIDialog:AddControl( g_pIMELoginID )
	g_pIMELoginID:SetName( "IMELoginID2" )
	g_pIMELoginID:SetTextPoint
	{
		USE_TEXTURE_SIZE= FALSE,
		"LEFT_TOP		= D3DXVECTOR2(250,524)",
		"RIGHT_BOTTOM		= D3DXVECTOR2(388,524 + 524 - 506)",
		"COLOR			= D3DXCOLOR(1.0,1.0,1.0,1.0)",
		CHANGE_TIME		= 0.0,
	}
	g_pIMELoginID:SetFont( XF_DODUM_15_NORMAL )
	g_pIMELoginID:SetTextColor( 1.0, 1.0, 1.0, 1.0 )
	g_pIMELoginID:SetSelectedTextColor( 0.0, 0.0, 0.0, 1.0 )
	g_pIMELoginID:SetSelectedBackColor( 1.0, 0.0, 0.0, 1.0 )
	g_pIMELoginID:SetCaretColor( 1.0, 1.0, 1.0, 1.0 )
	g_pIMELoginID:SetWidthLimit( 60 )
	g_pIMELoginID:SetMultiLineOption( 1, 2 )
--]]

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
g_pEditBoxLoginPassword:SetTabControlName( "IMELoginID" )
g_pEditBoxLoginPassword:SetHideChar( "*" )
g_pEditBoxLoginPassword:SetCustomMsgEnter( STATE_LOGIN_UI_CUSTOM_MSG["SLUCM_LOGIN"] )



