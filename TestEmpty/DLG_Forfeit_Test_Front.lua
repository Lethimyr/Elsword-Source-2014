g_pUIDialog:SetName( "DLG_Test_Front" )
g_pUIDialog:SetPos( 0,0 )
g_pUIDialog:SetModal( true )
g_pUIDialog:SetFront( true )

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


}



-- 체크박스

g_pCheckBox1 = g_pUIDialog:CreateCheckBox()
g_pCheckBox1:SetName( "CheckBox1" )
g_pUIDialog:AddControl( g_pCheckBox1 )

g_pCheckBox1:SetNormalTex( "DLG_Login3.TGA", "오토로긴 노멀" )



g_pCheckBox1:SetCheckedTex( "DLG_Login3.TGA", "오토로긴 체크" )


g_pCheckBox1:SetBGPoint
{
	"LEFT_TOP		= D3DXVECTOR2(25,711)",
	"RIGHT_TOP		= D3DXVECTOR2(57,711)",
	"LEFT_BOTTOM		= D3DXVECTOR2(25,743)",
	"RIGHT_BOTTOM		= D3DXVECTOR2(57,743)",
	"COLOR			= D3DXCOLOR(1.0,1.0,1.0,1.0)",
	CHANGE_TIME		= 0.0,
}
g_pCheckBox1:SetCheckPoint
{
	"LEFT_TOP		= D3DXVECTOR2(25,711)",
	"RIGHT_TOP		= D3DXVECTOR2(57,711)",
	"LEFT_BOTTOM		= D3DXVECTOR2(25,743)",
	"RIGHT_BOTTOM		= D3DXVECTOR2(57,743)",
	"COLOR			= D3DXCOLOR(1.0,1.0,1.0,1.0)",
	CHANGE_TIME		= 0.0,

}

g_pCheckBox1:SetCustomMsgChecked( STATE_TEST_UI_CUSTOM_MSG["STUCM_CHECK1SET"] )
g_pCheckBox1:SetCustomMsgUnChecked( STATE_TEST_UI_CUSTOM_MSG["STUCM_CHECK1UNSET"] )


-- 체크박스2

g_pCheckBox2 = g_pUIDialog:CreateCheckBox()
g_pCheckBox2:SetName( "CheckBox2" )
g_pUIDialog:AddControl( g_pCheckBox2 )

g_pCheckBox2:SetNormalTex( "DLG_Login3.TGA", "오토로긴 노멀" )
g_pCheckBox2:SetCheckedTex( "DLG_Login3.TGA", "오토로긴 체크" )


g_pCheckBox2:SetBGPoint
{
	"LEFT_TOP		= D3DXVECTOR2(25+40,711)",
	"RIGHT_TOP		= D3DXVECTOR2(57+40,711)",
	"LEFT_BOTTOM		= D3DXVECTOR2(25+40,743)",
	"RIGHT_BOTTOM		= D3DXVECTOR2(57+40,743)",
	"COLOR			= D3DXCOLOR(1.0,1.0,1.0,1.0)",
	CHANGE_TIME		= 0.0,
}
g_pCheckBox2:SetCheckPoint
{
	"LEFT_TOP		= D3DXVECTOR2(25+40,711)",
	"RIGHT_TOP		= D3DXVECTOR2(57+40,711)",
	"LEFT_BOTTOM		= D3DXVECTOR2(25+40,743)",
	"RIGHT_BOTTOM		= D3DXVECTOR2(57+40,743)",
	"COLOR			= D3DXCOLOR(1.0,1.0,1.0,1.0)",
	CHANGE_TIME		= 0.0,

}

g_pCheckBox2:SetCustomMsgChecked( STATE_TEST_UI_CUSTOM_MSG["STUCM_CHECK2SET"] )
g_pCheckBox2:SetCustomMsgUnChecked( STATE_TEST_UI_CUSTOM_MSG["STUCM_CHECK2UNSET"] )

--[[
-- 체크박스3

g_pCheckBox3 = g_pUIDialog:CreateCheckBox()
g_pCheckBox3:SetName( "CheckBox3" )
g_pUIDialog:AddControl( g_pCheckBox3 )

g_pCheckBox3:SetNormalTex( "DLG_Login3.TGA", "오토로긴 노멀" )
g_pCheckBox3:SetCheckedTex( "DLG_Login3.TGA", "오토로긴 체크" )


g_pCheckBox3:SetBGPoint
{
	"LEFT_TOP		= D3DXVECTOR2(25+80,711)",
	"RIGHT_TOP		= D3DXVECTOR2(57+80,711)",
	"LEFT_BOTTOM		= D3DXVECTOR2(25+80,743)",
	"RIGHT_BOTTOM		= D3DXVECTOR2(57+80,743)",
	"COLOR			= D3DXCOLOR(1.0,1.0,1.0,1.0)",
	CHANGE_TIME		= 0.0,
}
g_pCheckBox3:SetCheckPoint
{
	"LEFT_TOP		= D3DXVECTOR2(25+80,711)",
	"RIGHT_TOP		= D3DXVECTOR2(57+80,711)",
	"LEFT_BOTTOM		= D3DXVECTOR2(25+80,743)",
	"RIGHT_BOTTOM		= D3DXVECTOR2(57+80,743)",
	"COLOR			= D3DXCOLOR(1.0,1.0,1.0,1.0)",
	CHANGE_TIME		= 0.0,

}

g_pCheckBox3:SetCustomMsgChecked( STATE_TEST_UI_CUSTOM_MSG["STUCM_CHECK3SET"] )
g_pCheckBox3:SetCustomMsgUnChecked( STATE_TEST_UI_CUSTOM_MSG["STUCM_CHECK3UNSET"] )


-- 체크박스4

g_pCheckBox4 = g_pUIDialog:CreateCheckBox()
g_pCheckBox4:SetName( "CheckBox4" )
g_pUIDialog:AddControl( g_pCheckBox4 )

g_pCheckBox4:SetNormalTex( "DLG_Login3.TGA", "오토로긴 노멀" )
g_pCheckBox4:SetCheckedTex( "DLG_Login3.TGA", "오토로긴 체크" )


g_pCheckBox4:SetBGPoint
{
	"LEFT_TOP		= D3DXVECTOR2(25+120,711)",
	"RIGHT_TOP		= D3DXVECTOR2(57+120,711)",
	"LEFT_BOTTOM		= D3DXVECTOR2(25+120,743)",
	"RIGHT_BOTTOM		= D3DXVECTOR2(57+120,743)",
	"COLOR			= D3DXCOLOR(1.0,1.0,1.0,1.0)",
	CHANGE_TIME		= 0.0,
}
g_pCheckBox4:SetCheckPoint
{
	"LEFT_TOP		= D3DXVECTOR2(25+120,711)",
	"RIGHT_TOP		= D3DXVECTOR2(57+120,711)",
	"LEFT_BOTTOM		= D3DXVECTOR2(25+120,743)",
	"RIGHT_BOTTOM		= D3DXVECTOR2(57+120,743)",
	"COLOR			= D3DXCOLOR(1.0,1.0,1.0,1.0)",
	CHANGE_TIME		= 0.0,

}

g_pCheckBox4:SetCustomMsgChecked( STATE_TEST_UI_CUSTOM_MSG["STUCM_CHECK4SET"] )
g_pCheckBox4:SetCustomMsgUnChecked( STATE_TEST_UI_CUSTOM_MSG["STUCM_CHECK4UNSET"] )



-- 본격라디오

g_pRadioButton1 = g_pUIDialog:CreateRadioButton()
g_pUIDialog:AddControl( g_pRadioButton1 )

g_pRadioButton1:SetFixOverByCheck(true)
g_pRadioButton1:SetShowOffBGByCheck(true)

g_pRadioButton1:SetName( "RadioButton1" )

g_pRadioButton1:SetNormalTex( "DLG_Common_Texture09.tga", "Option_Not_Vitality" )
g_pRadioButton1:SetOverTex( "DLG_Common_Texture09.tga", "Option_Vitality" )
g_pRadioButton1:SetCheckedTex( "DLG_Common_Texture09.tga", "Option_Vitality" )

g_pRadioButton1:SetBGPoint
{

	"LEFT_TOP		= D3DXVECTOR2(25,660)",
	"COLOR			= D3DXCOLOR(1.0,1.0,1.0,1.0)",
	CHANGE_TIME		= 0.0,
}

g_pRadioButton1:SetBGMouseOverPoint
{

	"LEFT_TOP		= D3DXVECTOR2(25,660)",
	"COLOR			= D3DXCOLOR(1.0,1.0,1.0,1.0)",
	CHANGE_TIME		= 0.0,
}

g_pRadioButton1:SetRBPoint
{

 	USE_TEXTURE_SIZE	= FALSE,

 	"LEFT_TOP		= D3DXVECTOR2(25,660)",
	"RIGHT_BOTTOM	= D3DXVECTOR2(25+19,660+19)",
	"COLOR			= D3DXCOLOR(1.0,1.0,1.0,1.0)",
	CHANGE_TIME		= 0.0,
}

g_pRadioButton1:SetCheckPoint
{

 	"LEFT_TOP		= D3DXVECTOR2(25,660)",
 	"COLOR			= D3DXCOLOR(1.0,1.0,1.0,1.0)",
	CHANGE_TIME		= 0.0,
}


g_pRadioButton1:SetGroupID( 4 )
g_pRadioButton1:SetCustomMsgChecked( STATE_TEST_UI_CUSTOM_MSG["STUCM_RADIO1SET"] ) 


-- 라디오2


g_pRadioButton2 = g_pUIDialog:CreateRadioButton()
g_pUIDialog:AddControl( g_pRadioButton2 )

g_pRadioButton2:SetFixOverByCheck(true)
g_pRadioButton2:SetShowOffBGByCheck(true)

g_pRadioButton2:SetName( "RadioButton2" )

g_pRadioButton2:SetNormalTex( "DLG_Common_Texture09.tga", "Option_Not_Vitality" )
g_pRadioButton2:SetOverTex( "DLG_Common_Texture09.tga", "Option_Vitality" )
g_pRadioButton2:SetCheckedTex( "DLG_Common_Texture09.tga", "Option_Vitality" )

g_pRadioButton2:SetBGPoint
{

	"LEFT_TOP		= D3DXVECTOR2(25+30,660)",
	"COLOR			= D3DXCOLOR(1.0,1.0,1.0,1.0)",
	CHANGE_TIME		= 0.0,
}

g_pRadioButton2:SetBGMouseOverPoint
{

	"LEFT_TOP		= D3DXVECTOR2(25+30,660)",
	"COLOR			= D3DXCOLOR(1.0,1.0,1.0,1.0)",
	CHANGE_TIME		= 0.0,
}

g_pRadioButton2:SetRBPoint
{

 	USE_TEXTURE_SIZE	= FALSE,

 	"LEFT_TOP		= D3DXVECTOR2(25+30,660)",
	"RIGHT_BOTTOM	= D3DXVECTOR2(25+30+19,660+19)",
	"COLOR			= D3DXCOLOR(1.0,1.0,1.0,1.0)",
	CHANGE_TIME		= 0.0,
}

g_pRadioButton2:SetCheckPoint
{

 	"LEFT_TOP		= D3DXVECTOR2(25+30,660)",
 	"COLOR			= D3DXCOLOR(1.0,1.0,1.0,1.0)",
	CHANGE_TIME		= 0.0,
}


g_pRadioButton2:SetGroupID( 4 )
g_pRadioButton2:SetCustomMsgChecked( STATE_TEST_UI_CUSTOM_MSG["STUCM_RADIO2SET"] ) 





-- 라디오3


g_pRadioButton3 = g_pUIDialog:CreateRadioButton()
g_pUIDialog:AddControl( g_pRadioButton3 )

g_pRadioButton3:SetFixOverByCheck(true)
g_pRadioButton3:SetShowOffBGByCheck(true)

g_pRadioButton3:SetName( "RadioButton3" )

g_pRadioButton3:SetNormalTex( "DLG_Common_Texture09.tga", "Option_Not_Vitality" )
g_pRadioButton3:SetOverTex( "DLG_Common_Texture09.tga", "Option_Vitality" )
g_pRadioButton3:SetCheckedTex( "DLG_Common_Texture09.tga", "Option_Vitality" )

g_pRadioButton3:SetBGPoint
{

	"LEFT_TOP		= D3DXVECTOR2(25+60,660)",
	"COLOR			= D3DXCOLOR(1.0,1.0,1.0,1.0)",
	CHANGE_TIME		= 0.0,
}

g_pRadioButton3:SetBGMouseOverPoint
{

	"LEFT_TOP		= D3DXVECTOR2(25+60,660)",
	"COLOR			= D3DXCOLOR(1.0,1.0,1.0,1.0)",
	CHANGE_TIME		= 0.0,
}

g_pRadioButton3:SetRBPoint
{

 	USE_TEXTURE_SIZE	= FALSE,

 	"LEFT_TOP		= D3DXVECTOR2(25+60,660)",
	"RIGHT_BOTTOM	= D3DXVECTOR2(25+60+19,660+19)",
	"COLOR			= D3DXCOLOR(1.0,1.0,1.0,1.0)",
	CHANGE_TIME		= 0.0,
}

g_pRadioButton3:SetCheckPoint
{

 	"LEFT_TOP		= D3DXVECTOR2(25+60,660)",
 	"COLOR			= D3DXCOLOR(1.0,1.0,1.0,1.0)",
	CHANGE_TIME		= 0.0,
}


g_pRadioButton3:SetGroupID( 4 )
g_pRadioButton3:SetCustomMsgChecked( STATE_TEST_UI_CUSTOM_MSG["STUCM_RADIO3SET"] ) 
----]]

