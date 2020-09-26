
CEM_OPENFIRSTMODELFILE = 0
CEM_OPENSECONDMODELFILE = 1
CEM_COMPARE = 2
CEM_SORTFIRSTMODEL = 3
CEM_SORTSECONDMODEL = 4
CEM_SELECTFIRSTMODEL = 5
CEM_SELECTSECONDMODEL = 6

CEM_FIRSTGROUP_CHEKEDED =7
CEM_FIRSTGROUP_UNCHECKED = 8

CEM_SECONDGROUP_CHEKEDED = 9
CEM_SECONDGROUP_UNCHECKED  = 10

CEM_SELECTION_CHANGED = 11

CEM_LISTBOX_SELECTION = 12
CEM_LISTBOX_SELECTION_END = 13
CEM_LISTBOX_ITEM_DOUBLECLICK = 14

CEM_MODEL_SORTCHECKED = 15
CEM_MODEL_SORTUNCHECKED = 16

CEM_ALLSORT = 17
CEM_REMOVE_TOP_FRAME_NAME = 18

CEM_FILE_TO_REMOVE_FRAME_CHECKED = 19
CEM_FILE_TO_REMOVE_FRAME_UN_CHECKED = 20


XF_DODUM_15_NORMAL = 0

TRUE = 1
FALSE = 0

g_pUIDialog:SetName( "CompareDialog" )
g_pUIDialog:SetPos( 0,0 )
g_pUIDialog:SetModal( true )

g_pButton = g_pUIDialog:CreateButton()
g_pButton:SetName( "OpenFirstModel" )
g_pUIDialog:AddControl( g_pButton )
g_pButton:SetNormalTex( "temp1.TGA", "temp1" )

g_pButton:SetOverTex( "temp1.TGA", "temp1" )

g_pButton:SetDownTex( "temp1.TGA", "temp1" )

g_pButton:SetNormalPoint
{
	USE_TEXTURE_SIZE	= FALSE,
	IS_RECT			= FALSE,

	"LEFT_TOP		= D3DXVECTOR2(33,443)",
	"RIGHT_TOP		= D3DXVECTOR2(146,443)",
	"LEFT_BOTTOM	= D3DXVECTOR2(33,482)",
	"RIGHT_BOTTOM	= D3DXVECTOR2(146,482)",
	"COLOR			= D3DXCOLOR(1.0,1.0,1.0,1.0)",
	CHANGE_TIME		= 0.0,
}
g_pButton:SetOverPoint
{
	USE_TEXTURE_SIZE	= FALSE,
	IS_RECT			= FALSE,

	"LEFT_TOP		= D3DXVECTOR2(31,440)",
	"RIGHT_TOP		= D3DXVECTOR2(148,440)",
	"LEFT_BOTTOM	= D3DXVECTOR2(31,486)",
	"RIGHT_BOTTOM	= D3DXVECTOR2(148,486)",
	"COLOR			= D3DXCOLOR(1.0,1.0,1.0,1.0)",
	CHANGE_TIME		= 1.0,
}
g_pButton:SetDownPoint
{
	USE_TEXTURE_SIZE	= FALSE,
	IS_RECT			= FALSE,

	"LEFT_TOP		= D3DXVECTOR2(31,440)",
	"RIGHT_TOP		= D3DXVECTOR2(148,440)",
	"LEFT_BOTTOM	= D3DXVECTOR2(31,486)",
	"RIGHT_BOTTOM	= D3DXVECTOR2(148,486)",
	"COLOR			= D3DXCOLOR(1.0,1.0,1.0,1.0)",
	CHANGE_TIME		= 0.0,
}
g_pButton:SetCustomMsgMouseUp( CEM_OPENFIRSTMODELFILE )


g_pButton = g_pUIDialog:CreateButton()
g_pButton:SetName( "SortFirstModel" )
g_pUIDialog:AddControl( g_pButton )
g_pButton:SetNormalTex( "sort.tga", "sort" )

g_pButton:SetOverTex( "sort.tga", "sort" )

g_pButton:SetDownTex( "sort.tga", "sort" )

g_pButton:SetNormalPoint
{
	USE_TEXTURE_SIZE	= FALSE,
	IS_RECT			= FALSE,

	"LEFT_TOP		= D3DXVECTOR2(233,443)",
	"RIGHT_TOP		= D3DXVECTOR2(346,443)",
	"LEFT_BOTTOM	= D3DXVECTOR2(233,482)",
	"RIGHT_BOTTOM	= D3DXVECTOR2(346,482)",
	"COLOR			= D3DXCOLOR(1.0,1.0,1.0,1.0)",
	CHANGE_TIME		= 0.0,
}
g_pButton:SetOverPoint
{
	USE_TEXTURE_SIZE	= FALSE,
	IS_RECT			= FALSE,

	"LEFT_TOP		= D3DXVECTOR2(231,440)",
	"RIGHT_TOP		= D3DXVECTOR2(348,440)",
	"LEFT_BOTTOM	= D3DXVECTOR2(231,486)",
	"RIGHT_BOTTOM	= D3DXVECTOR2(348,486)",
	"COLOR			= D3DXCOLOR(1.0,1.0,1.0,1.0)",
	CHANGE_TIME		= 0.0,
}
g_pButton:SetDownPoint
{
	USE_TEXTURE_SIZE	= FALSE,
	IS_RECT			= FALSE,

	"LEFT_TOP		= D3DXVECTOR2(231,440)",
	"RIGHT_TOP		= D3DXVECTOR2(348,440)",
	"LEFT_BOTTOM	= D3DXVECTOR2(231,486)",
	"RIGHT_BOTTOM	= D3DXVECTOR2(348,486)",
	"COLOR			= D3DXCOLOR(1.0,1.0,1.0,1.0)",
	CHANGE_TIME		= 0.0,
}
g_pButton:SetCustomMsgMouseUp( CEM_SORTFIRSTMODEL )


g_pButton = g_pUIDialog:CreateButton()
g_pButton:SetName( "OpenSecondModel" )
g_pUIDialog:AddControl( g_pButton )
g_pButton:SetNormalTex( "temp2.tga", "temp2" )

g_pButton:SetOverTex( "temp2.tga", "temp2" )

g_pButton:SetDownTex( "temp2.tga", "temp2" )

g_pButton:SetNormalPoint
{
	USE_TEXTURE_SIZE	= FALSE,
	IS_RECT			= FALSE,

	"LEFT_TOP		= D3DXVECTOR2(33,503)",
	"RIGHT_TOP		= D3DXVECTOR2(146,503)",
	"LEFT_BOTTOM	= D3DXVECTOR2(33,542)",
	"RIGHT_BOTTOM	= D3DXVECTOR2(146,542)",
	"COLOR			= D3DXCOLOR(1.0,1.0,1.0,1.0)",
	CHANGE_TIME		= 0.0,
}
g_pButton:SetOverPoint
{
	USE_TEXTURE_SIZE	= FALSE,
	IS_RECT			= FALSE,

	"LEFT_TOP		= D3DXVECTOR2(31,500)",
	"RIGHT_TOP		= D3DXVECTOR2(148,500)",
	"LEFT_BOTTOM	= D3DXVECTOR2(31,546)",
	"RIGHT_BOTTOM	= D3DXVECTOR2(148,546)",
	"COLOR			= D3DXCOLOR(1.0,1.0,1.0,1.0)",
	CHANGE_TIME		= 0.0,
}
g_pButton:SetDownPoint
{
	USE_TEXTURE_SIZE	= FALSE,
	IS_RECT			= FALSE,

	"LEFT_TOP		= D3DXVECTOR2(31,500)",
	"RIGHT_TOP		= D3DXVECTOR2(148,500)",
	"LEFT_BOTTOM	= D3DXVECTOR2(31,546)",
	"RIGHT_BOTTOM	= D3DXVECTOR2(148,546)",
	"COLOR			= D3DXCOLOR(1.0,1.0,1.0,1.0)",
	CHANGE_TIME		= 0.0,
}
g_pButton:SetCustomMsgMouseUp( CEM_OPENSECONDMODELFILE )


g_pButton = g_pUIDialog:CreateButton()
g_pButton:SetName( "SortSecondModel" )
g_pUIDialog:AddControl( g_pButton )
g_pButton:SetNormalTex( "sort.tga", "sort" )

g_pButton:SetOverTex( "sort.tga", "sort" )

g_pButton:SetDownTex( "sort.tga", "sort" )

g_pButton:SetNormalPoint
{
	USE_TEXTURE_SIZE	= FALSE,
	IS_RECT			= FALSE,

	"LEFT_TOP		= D3DXVECTOR2(233,503)",
	"RIGHT_TOP		= D3DXVECTOR2(346,503)",
	"LEFT_BOTTOM	= D3DXVECTOR2(233,542)",
	"RIGHT_BOTTOM	= D3DXVECTOR2(346,542)",
	"COLOR			= D3DXCOLOR(1.0,1.0,1.0,1.0)",
	CHANGE_TIME		= 0.0,
}
g_pButton:SetOverPoint
{
	USE_TEXTURE_SIZE	= FALSE,
	IS_RECT			= FALSE,

	"LEFT_TOP		= D3DXVECTOR2(231,500)",
	"RIGHT_TOP		= D3DXVECTOR2(348,500)",
	"LEFT_BOTTOM	= D3DXVECTOR2(231,546)",
	"RIGHT_BOTTOM	= D3DXVECTOR2(348,546)",
	"COLOR			= D3DXCOLOR(1.0,1.0,1.0,1.0)",
	CHANGE_TIME		= 0.0,
}
g_pButton:SetDownPoint
{
	USE_TEXTURE_SIZE	= FALSE,
	IS_RECT			= FALSE,

	"LEFT_TOP		= D3DXVECTOR2(231,500)",
	"RIGHT_TOP		= D3DXVECTOR2(348,500)",
	"LEFT_BOTTOM	= D3DXVECTOR2(231,546)",
	"RIGHT_BOTTOM	= D3DXVECTOR2(348,546)",
	"COLOR			= D3DXCOLOR(1.0,1.0,1.0,1.0)",
	CHANGE_TIME		= 0.0,
}
g_pButton:SetCustomMsgMouseUp( CEM_SORTSECONDMODEL )


g_pButton = g_pUIDialog:CreateButton()
g_pButton:SetName( "CompareButton" )
g_pUIDialog:AddControl( g_pButton )
g_pButton:SetNormalTex( "temp3.tga", "temp3" )

g_pButton:SetOverTex( "temp3.tga", "temp3" )

g_pButton:SetDownTex( "temp3.tga", "temp3" )

g_pButton:SetNormalPoint
{
	USE_TEXTURE_SIZE	= FALSE,
	IS_RECT			= FALSE,

	"LEFT_TOP		= D3DXVECTOR2(33,563)",
	"RIGHT_TOP		= D3DXVECTOR2(146,563)",
	"LEFT_BOTTOM	= D3DXVECTOR2(33,602)",
	"RIGHT_BOTTOM	= D3DXVECTOR2(146,602)",
	"COLOR			= D3DXCOLOR(1.0,1.0,1.0,1.0)",
	CHANGE_TIME		= 0.0,
}
g_pButton:SetOverPoint
{
	USE_TEXTURE_SIZE	= FALSE,
	IS_RECT			= FALSE,

	"LEFT_TOP		= D3DXVECTOR2(31,560)",
	"RIGHT_TOP		= D3DXVECTOR2(148,560)",
	"LEFT_BOTTOM	= D3DXVECTOR2(31,606)",
	"RIGHT_BOTTOM	= D3DXVECTOR2(148,606)",
	"COLOR			= D3DXCOLOR(1.0,1.0,1.0,1.0)",
	CHANGE_TIME		= 0.0,
}
g_pButton:SetDownPoint
{
	USE_TEXTURE_SIZE	= FALSE,
	IS_RECT			= FALSE,

	"LEFT_TOP		= D3DXVECTOR2(31,560)",
	"RIGHT_TOP		= D3DXVECTOR2(148,560)",
	"LEFT_BOTTOM	= D3DXVECTOR2(31,606)",
	"RIGHT_BOTTOM	= D3DXVECTOR2(148,606)",
	"COLOR			= D3DXCOLOR(1.0,1.0,1.0,1.0)",
	CHANGE_TIME		= 0.0,
}
g_pButton:SetCustomMsgMouseUp( CEM_COMPARE )





g_pButton = g_pUIDialog:CreateButton()
g_pButton:SetName( "SelectFirstModel" )
g_pUIDialog:AddControl( g_pButton )
g_pButton:SetNormalTex( "FirstModel.tga", "FirstModel" )

g_pButton:SetOverTex( "FirstModel.tga", "FirstModel" )

g_pButton:SetDownTex( "FirstModel.tga", "FirstModel" )

g_pButton:SetNormalPoint
{
	USE_TEXTURE_SIZE	= FALSE,
	IS_RECT			= FALSE,

	"LEFT_TOP		= D3DXVECTOR2(233,603)",
	"RIGHT_TOP		= D3DXVECTOR2(346,603)",
	"LEFT_BOTTOM	= D3DXVECTOR2(233,642)",
	"RIGHT_BOTTOM	= D3DXVECTOR2(346,642)",
	"COLOR			= D3DXCOLOR(1.0,1.0,1.0,1.0)",
	CHANGE_TIME		= 0.0,
}
g_pButton:SetOverPoint
{
	USE_TEXTURE_SIZE	= FALSE,
	IS_RECT			= FALSE,

	"LEFT_TOP		= D3DXVECTOR2(231,600)",
	"RIGHT_TOP		= D3DXVECTOR2(348,600)",
	"LEFT_BOTTOM	= D3DXVECTOR2(231,646)",
	"RIGHT_BOTTOM	= D3DXVECTOR2(348,646)",
	"COLOR			= D3DXCOLOR(1.0,1.0,1.0,1.0)",
	CHANGE_TIME		= 0.0,
}
g_pButton:SetDownPoint
{
	USE_TEXTURE_SIZE	= FALSE,
	IS_RECT			= FALSE,

	"LEFT_TOP		= D3DXVECTOR2(231,600)",
	"RIGHT_TOP		= D3DXVECTOR2(348,600)",
	"LEFT_BOTTOM	= D3DXVECTOR2(231,646)",
	"RIGHT_BOTTOM	= D3DXVECTOR2(348,646)",
	"COLOR			= D3DXCOLOR(1.0,1.0,1.0,1.0)",
	CHANGE_TIME		= 0.0,
}
g_pButton:SetCustomMsgMouseUp( CEM_SELECTFIRSTMODEL )


g_pButton = g_pUIDialog:CreateButton()
g_pButton:SetName( "SelectSecondModel" )
g_pUIDialog:AddControl( g_pButton )
g_pButton:SetNormalTex( "SecondModel.tga", "SecondModel" )

g_pButton:SetOverTex( "SecondModel.tga", "SecondModel" )

g_pButton:SetDownTex( "SecondModel.tga", "SecondModel" )

g_pButton:SetNormalPoint
{
	USE_TEXTURE_SIZE	= FALSE,
	IS_RECT			= FALSE,

	"LEFT_TOP		= D3DXVECTOR2(233,653)",
	"RIGHT_TOP		= D3DXVECTOR2(346,653)",
	"LEFT_BOTTOM	= D3DXVECTOR2(233,692)",
	"RIGHT_BOTTOM	= D3DXVECTOR2(346,692)",
	"COLOR			= D3DXCOLOR(1.0,1.0,1.0,1.0)",
	CHANGE_TIME		= 0.0,
}
g_pButton:SetOverPoint
{
	USE_TEXTURE_SIZE	= FALSE,
	IS_RECT			= FALSE,

	"LEFT_TOP		= D3DXVECTOR2(231,650)",
	"RIGHT_TOP		= D3DXVECTOR2(348,650)",
	"LEFT_BOTTOM	= D3DXVECTOR2(231,696)",
	"RIGHT_BOTTOM	= D3DXVECTOR2(348,696)",
	"COLOR			= D3DXCOLOR(1.0,1.0,1.0,1.0)",
	CHANGE_TIME		= 0.0,
}
g_pButton:SetDownPoint
{
	USE_TEXTURE_SIZE	= FALSE,
	IS_RECT			= FALSE,

	"LEFT_TOP		= D3DXVECTOR2(231,650)",
	"RIGHT_TOP		= D3DXVECTOR2(348,650)",
	"LEFT_BOTTOM	= D3DXVECTOR2(231,696)",
	"RIGHT_BOTTOM	= D3DXVECTOR2(348,696)",
	"COLOR			= D3DXCOLOR(1.0,1.0,1.0,1.0)",
	CHANGE_TIME		= 0.0,
}
g_pButton:SetCustomMsgMouseUp( CEM_SELECTSECONDMODEL )



g_pCheckBox = g_pUIDialog:CreateCheckBox()
g_pCheckBox:SetName( "SortCheck" )
g_pUIDialog:AddControl( g_pCheckBox )

g_pCheckBox:SetNormalTex( "CheckNormalBG.tga", "CheckNormalBG" )

g_pCheckBox:SetOverTex( "CheckOverBG.tga", "CheckOverBG" )

g_pCheckBox:SetDisableTex( "CheckNormalBG.tga", "CheckNormalBG" )

g_pCheckBox:SetCheckedTex( "CheckedBG.tga", "CheckedBG" )


g_pCheckBox:SetBGPoint
{
	USE_TEXTURE_SIZE	= FALSE,
	IS_RECT			= FALSE,

	"LEFT_TOP		= D3DXVECTOR2(360,440)",
	"RIGHT_TOP		= D3DXVECTOR2(400,440)",
	"LEFT_BOTTOM		= D3DXVECTOR2(360,486)",
	"RIGHT_BOTTOM		= D3DXVECTOR2(400,486)",
	"COLOR			= D3DXCOLOR(1.0,1.0,1.0,1.0)",
	CHANGE_TIME		= 0.0,
}
g_pCheckBox:SetCheckPoint
{
	USE_TEXTURE_SIZE	= FALSE,
	IS_RECT			= FALSE,

	"LEFT_TOP		= D3DXVECTOR2(360,440)",
	"RIGHT_TOP		= D3DXVECTOR2(400,440)",
	"LEFT_BOTTOM		= D3DXVECTOR2(360,486)",
	"RIGHT_BOTTOM		= D3DXVECTOR2(400,486)",
	"COLOR			= D3DXCOLOR(1.0,1.0,1.0,1.0)",
	CHANGE_TIME		= 0.0,
}

g_pCheckBox:SetCustomMsgChecked( CEM_MODEL_SORTCHECKED )
g_pCheckBox:SetCustomMsgUnChecked( CEM_MODEL_SORTUNCHECKED )
g_pCheckBox:SetChecked( 1 )



g_pButton = g_pUIDialog:CreateButton()
g_pButton:SetName( "SortButton" )
g_pUIDialog:AddControl( g_pButton )
g_pButton:SetNormalTex( "Allsort.tga", "Allsort" )

g_pButton:SetOverTex( "Allsort.tga", "Allsort" )

g_pButton:SetDownTex( "Allsort.tga", "Allsort" )

g_pButton:SetNormalPoint
{
	USE_TEXTURE_SIZE	= FALSE,
	IS_RECT			= FALSE,

	"LEFT_TOP		= D3DXVECTOR2(33,680)",
	"RIGHT_TOP		= D3DXVECTOR2(146,680)",
	"LEFT_BOTTOM	= D3DXVECTOR2(33,720)",
	"RIGHT_BOTTOM	= D3DXVECTOR2(146,720)",
	"COLOR			= D3DXCOLOR(1.0,1.0,1.0,1.0)",
	CHANGE_TIME		= 0.0,
}
g_pButton:SetOverPoint
{
	USE_TEXTURE_SIZE	= FALSE,
	IS_RECT			= FALSE,

	"LEFT_TOP		= D3DXVECTOR2(31,680)",
	"RIGHT_TOP		= D3DXVECTOR2(148,680)",
	"LEFT_BOTTOM	= D3DXVECTOR2(31,720)",
	"RIGHT_BOTTOM	= D3DXVECTOR2(148,720)",
	"COLOR			= D3DXCOLOR(1.0,1.0,1.0,1.0)",
	CHANGE_TIME		= 0.0,
}
g_pButton:SetDownPoint
{
	USE_TEXTURE_SIZE	= FALSE,
	IS_RECT			= FALSE,

	"LEFT_TOP		= D3DXVECTOR2(31,680)",
	"RIGHT_TOP		= D3DXVECTOR2(148,680)",
	"LEFT_BOTTOM	= D3DXVECTOR2(31,720)",
	"RIGHT_BOTTOM	= D3DXVECTOR2(148,720)",
	"COLOR			= D3DXCOLOR(1.0,1.0,1.0,1.0)",
	CHANGE_TIME		= 0.0,
}
g_pButton:SetCustomMsgMouseUp( CEM_ALLSORT )


g_pStaticMessenger_Chat = g_pUIDialog:CreateStatic()
g_pUIDialog:AddControl( g_pStaticMessenger_Chat )
g_pStaticMessenger_Chat:SetName( "Option_Window" )

g_pPictureMessenger_Chat_Window_BG1 = g_pUIDialog:CreatePicture()
g_pStaticMessenger_Chat:AddPicture( g_pPictureMessenger_Chat_Window_BG1 )

g_pPictureMessenger_Chat_Window_BG1:SetTex2( "RemoveFrameNameIMEEdit.tga"  )

g_pPictureMessenger_Chat_Window_BG1:SetPoint
{
	ADD_SIZE_X = 100,
	ADD_SIZE_Y = -10,
	
	"LEFT_TOP		= D3DXVECTOR2(30,730)",
	"COLOR			= D3DXCOLOR(1.0,1.0,1.0,1.0)",
	CHANGE_TIME		= 0.0,
}


g_pIMEID = g_pUIDialog:CreateIMEEditBox()
g_pUIDialog:AddControl( g_pIMEID )
g_pIMEID:SetName( "IMEEditFrameNameToRemove" )
g_pIMEID:SetTextPoint
{
	USE_TEXTURE_SIZE	= FALSE,
	
	"LEFT_TOP			= D3DXVECTOR2(35,735)",
	"RIGHT_BOTTOM		= D3DXVECTOR2(330,755)",
	
	"COLOR				= D3DXCOLOR(1.0,1.0,1.0,1.0)",
	CHANGE_TIME			= 0.0,
}
g_pIMEID:SetFont( XF_DODUM_15_NORMAL )
g_pIMEID:SetTextColor( 1.0, 0.0, 0.0, 1.0 )
g_pIMEID:SetSelectedTextColor( 0.0, 0.0, 0.0, 1.0 )
g_pIMEID:SetSelectedBackColor( 1.0, 0.0, 0.0, 1.0 )
g_pIMEID:SetCaretColor( 1.0, 0.0, 0.0, 1.0 )
g_pIMEID:RequestFocus()



g_pButton = g_pUIDialog:CreateButton()
g_pButton:SetName( "RemoveButton" )
g_pUIDialog:AddControl( g_pButton )
g_pButton:SetNormalTex( "RemoveFrameButton.tga", "RemoveFrameButton" )

g_pButton:SetOverTex( "RemoveFrameButton.tga", "RemoveFrameButton" )

g_pButton:SetDownTex( "RemoveFrameButton.tga", "RemoveFrameButton" )

g_pButton:SetNormalPoint
{
	"LEFT_TOP		= D3DXVECTOR2(350,720)",

	"COLOR			= D3DXCOLOR(1.0,1.0,1.0,1.0)",
	CHANGE_TIME		= 0.0,
}
g_pButton:SetOverPoint
{
	ADD_SIZE_X = 5,
	ADD_SIZE_Y = 5,

	"LEFT_TOP		= D3DXVECTOR2(350,720)",

	"COLOR			= D3DXCOLOR(1.0,1.0,1.0,1.0)",
	CHANGE_TIME		= 0.0,
}
g_pButton:SetDownPoint
{
	"LEFT_TOP		= D3DXVECTOR2(350,720)",

	"COLOR			= D3DXCOLOR(1.0,1.0,1.0,1.0)",
	CHANGE_TIME		= 0.0,
}
g_pButton:SetCustomMsgMouseUp( CEM_REMOVE_TOP_FRAME_NAME )



g_pCheckBox = g_pUIDialog:CreateCheckBox()
g_pCheckBox:SetName( "FileToRemoveFrame" )
g_pUIDialog:AddControl( g_pCheckBox )

g_pCheckBox:SetNormalTex( "CheckNormalBG.tga", "CheckNormalBG" )

g_pCheckBox:SetOverTex( "CheckOverBG.tga", "CheckOverBG" )

g_pCheckBox:SetDisableTex( "CheckNormalBG.tga", "CheckNormalBG" )

g_pCheckBox:SetCheckedTex( "CheckedBG.tga", "CheckedBG" )


g_pCheckBox:SetBGPoint
{
	USE_TEXTURE_SIZE	= FALSE,


	"LEFT_TOP		= D3DXVECTOR2(360 + 100,440 + 280)",

	"RIGHT_BOTTOM		= D3DXVECTOR2(400 + 100,486 + 280)",
	"COLOR			= D3DXCOLOR(1.0,1.0,1.0,1.0)",
	CHANGE_TIME		= 0.0,
}
g_pCheckBox:SetCheckPoint
{
	USE_TEXTURE_SIZE	= FALSE,

	"LEFT_TOP		= D3DXVECTOR2(360 + 100,440 + 280)",

	"RIGHT_BOTTOM		= D3DXVECTOR2(400 + 100,486 + 280)",
	"COLOR			= D3DXCOLOR(1.0,1.0,1.0,1.0)",
	CHANGE_TIME		= 0.0,
}

g_pCheckBox:SetCustomMsgChecked( CEM_FILE_TO_REMOVE_FRAME_CHECKED )
g_pCheckBox:SetCustomMsgUnChecked( CEM_FILE_TO_REMOVE_FRAME_UN_CHECKED )
