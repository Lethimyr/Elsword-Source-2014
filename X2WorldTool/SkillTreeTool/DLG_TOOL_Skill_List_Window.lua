



g_pUIDialog:SetName( "DLG_Skill_List_Window" )
g_pUIDialog:SetPos( 0, 0 )
g_pUIDialog:SetModal( false )
g_pUIDialog:SetFront( true )
g_pUIDialog:SetEnableMoveByDrag( true )
g_pUIDialog:SetLayer( X2_DIALOG_LAYER["XDL_POP_UP"] )






g_pStatic = g_pUIDialog:CreateStatic()	
g_pUIDialog:AddControl( g_pStatic )
g_pStatic:SetName( "Static_Window_Frame" )



--  상단 가운데
g_pPicture = g_pUIDialog:CreatePicture()
g_pStatic:AddPicture( g_pPicture )
g_pPicture:SetTex( "DLG_SquareUI.TGA", "Chat_Window3" )
g_pPicture:SetPoint
{
    ADD_SIZE_X = 400,
	"LEFT_TOP		= D3DXVECTOR2(0,0)",
	"COLOR			= D3DXCOLOR(1.0,1.0,1.0,1.0)",
	CHANGE_TIME		= 0.0,
}






--------------------------------------------------------------------------------

g_pListBox = g_pUIDialog:CreateListBox()
g_pUIDialog:AddControl( g_pListBox )
g_pListBox:SetName( "ListBox_Monster_ID" )

g_pListBox:SetString	
{
	MSG 			= "NoName",
	FONT_INDEX		= XUF_DODUM_13_SEMIBOLD,
	"POS 			= D3DXVECTOR2(0,0)",
	"COLOR			= D3DXCOLOR(1.0,1.0,1.0,1.0)",
	"OUTLINE_COLOR	= D3DXCOLOR(0.0,0.0,0.0,1.0)",
}

g_pListBox:SetTextTex( "DLG_Room_Button0.tga", "Select_Box" )

g_pListBox:SetBoundingBoxTex( "DLG_Room_Button0.tga","Select_Box" )

g_pListBox:SetScollBarUpButtonTex( "DLG_SquareUI.tga", "Up_Arrow_Yllow" )

g_pListBox:SetScollBarUpButtonDownTex( "DLG_SquareUI.tga", "Up_Arrow_Red" )

g_pListBox:SetScollBarDownButtonTex( "DLG_SquareUI.tga", "Down_Arrow_Yllow" )

g_pListBox:SetScollBarDownButtonDownTex( "DLG_SquareUI.tga", "Down_Arrow_Red" )

g_pListBox:SetScollBarThumbTex( "DLG_Room_Button0.tga", "Thumb_Tex" )

g_pListBox:SetScollBarTrackTex( "DLG_Room_Button0.tga", "Invisible" )




g_pListBox:InitScrollBar()
g_pListBox:SetScrollBarWidth( 10 )
g_pListBox:SetPos( 0,50 )
g_pListBox:SetSize( 400, 400 )

g_pListBox:SetCustomMsgItemDoubleClick( SKILL_TREE_TOOL_UI_CUSTOM_MSG["STTUCM_SKILL_LIST_SELECTED"] )






g_pButton = g_pUIDialog:CreateButton()
g_pUIDialog:AddControl( g_pButton )
g_pButton:SetName( "Button_Close" )
g_pButton:SetNormalTex( "DLG_Common_Texture01.TGA", "Left_Triangle_Normal" )
g_pButton:SetOverTex( "DLG_Common_Texture01.TGA", "Left_Triangle_Over" )
g_pButton:SetDownTex( "DLG_Common_Texture01.TGA", "Left_Triangle_Down" )

g_pButton:SetNormalPoint
{
 	"LEFT_TOP		= D3DXVECTOR2(0,0)",
	"COLOR			= D3DXCOLOR(1.0,1.0,1.0,1.0)",
	CHANGE_		= 0.0,
}
g_pButton:SetOverPoint
{
	"LEFT_TOP		= D3DXVECTOR2(0,0)",
	"COLOR			= D3DXCOLOR(1.0,1.0,1.0,1.0)",
	CHANGE_		= 0.0,
}
g_pButton:SetDownPoint
{
     ADD_SIZE_X = -3,
     ADD_SIZE_Y = -2,
 	"LEFT_TOP		= D3DXVECTOR2(0,0)",

	"COLOR			= D3DXCOLOR(1.0,1.0,1.0,1.0)",
	CHANGE_		= 0.0,
}
g_pButton:SetCustomMsgMouseUp( SKILL_TREE_TOOL_UI_CUSTOM_MSG["STTUCM_SKILL_LIST_CLOSE"] )

