#include "StdAfx.h"

CX2ComboGuide::CX2ComboGuide()
{
	m_DlgComboGuide = NULL;	
	m_DlgComboGuide = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_Combo_Guide.lua" );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_DlgComboGuide );
	m_DlgComboGuide->SetShow(false);

	m_bInitedGuide = false;
}

CX2ComboGuide::~CX2ComboGuide()
{
	SAFE_DELETE_DIALOG( m_DlgComboGuide );
}

void CX2ComboGuide::InitComboGuide( CX2Unit::UNIT_CLASS eClass )
{
	if( m_bInitedGuide == true )
		return;

	KLuaManager kLuamanager( g_pKTDXApp->GetLuaBinder()->GetLuaState(), 0, true );
	g_pKTDXApp->LoadAndDoMemory( &kLuamanager, L"DLG_Command_Guide.lua" );

	if ( kLuamanager.BeginTable( "COMBO_GUIDE" ) == true )
	{
		if( kLuamanager.BeginTable( eClass ) == true )
		{
			int comboIndex = 0;
			while( kLuamanager.BeginTable( "COMBO_COMMAND", comboIndex ) == true )
			{
				ComboGuide comboGuide;

				int index = 1;
				while( kLuamanager.BeginTable( index ) == true )
				{
					if( index == 1 )
					{
						LUA_GET_VALUE( kLuamanager, 1,	comboGuide.m_wstrComboId,		L"" );
					}
					else
					{
						wstring wstrKey;
						int offsetX, offsetY;
						wstring wstrFullCommand;
						
						LUA_GET_VALUE( kLuamanager, 1,	wstrKey,			L"" );
						LUA_GET_VALUE( kLuamanager, 2,	offsetX,			0 );
						LUA_GET_VALUE( kLuamanager, 3,	offsetY,			0 );
						LUA_GET_VALUE( kLuamanager, 4,	wstrFullCommand,	L"" );

						string strKey;
						ConvertWCHARToChar( strKey, wstrKey.c_str() );
						char cKey = strKey.c_str()[0];
						comboGuide.AddComboCommand( cKey, offsetX, offsetY, wstrFullCommand );
					}

					kLuamanager.EndTable();
					index++;
				}

				CreateComboGuide( comboGuide );

				comboIndex++;
				kLuamanager.EndTable();
			}

			kLuamanager.EndTable();
		}

		kLuamanager.EndTable();
	}

	m_bInitedGuide = true;
}

void CX2ComboGuide::SetShow(bool val)
{
	if( m_DlgComboGuide != NULL )
	{
		m_DlgComboGuide->SetShow(val);
	}
}

void CX2ComboGuide::SetPos(D3DXVECTOR2 &vPos)
{
	if( m_DlgComboGuide != NULL )
	{
		m_DlgComboGuide->SetPos( vPos );
	}
}

void CX2ComboGuide::CreateComboGuide( ComboGuide combo )
{
	ComboGuide comboguide = combo;
	CKTDGUIStatic *pComboStatic = NULL;

	if( m_DlgComboGuide != NULL )
	{
		pComboStatic = m_DlgComboGuide->CreateStatic();		
		pComboStatic->SetName( comboguide.m_wstrComboId.c_str() );		

		for(UINT i=0; i<comboguide.m_vecCommand.size(); ++i)
		{
			wstring wstrTextureName = L"";
			wstring wstrTextureKeyNormal = L"";
			wstring wstrTextureKeyOver = L"";
			float offsetX = 0.0f, offsetY = 0.0f;
			D3DXCOLOR vColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

			ComboCommand &comboCommand = comboguide.m_vecCommand[i];
			switch(comboCommand.m_cCommandKey)
			{
			case 'u':	//'+'
				{
					wstrTextureName = L"DLG_UI_Common_Texture10.tga";
					wstrTextureKeyNormal = L"TREE_UP";
					offsetX = comboCommand.m_offsetX*24.f + 10.f;
					offsetY = comboCommand.m_offsetY*24.f + 11.f;
					vColor = D3DXCOLOR(1.f, 1.f, 1.f, 0.6f);
				}
				break;
			case 'd':	//'*'
				{
					wstrTextureName = L"DLG_UI_Common_Texture10.tga";
					wstrTextureKeyNormal = L"TREE_DOWN";
					offsetX = comboCommand.m_offsetX*24.f + 10.f;
					offsetY = comboCommand.m_offsetY*24.f - 2.f;
					vColor = D3DXCOLOR(1.f, 1.f, 1.f, 0.6f);
				}
				break;
			case 'm' : // '-'
				{
					wstrTextureName = L"DLG_UI_Common_Texture10.tga";
					wstrTextureKeyNormal = L"Tree_Down_Double";
					offsetX = comboCommand.m_offsetX*24.f + 10.f;
					offsetY = comboCommand.m_offsetY*25.f - 5.f;
					vColor = D3DXCOLOR(1.f, 1.f, 1.f, 0.6f);
				}
				break;
			case '|' : // '/'
				{
					wstrTextureName = L"DLG_UI_Common_Texture10.tga";
					wstrTextureKeyNormal = L"Tree_Down_Double_B";
					offsetX = comboCommand.m_offsetX*24.f + 10.f;
					offsetY = comboCommand.m_offsetY*25.f - 5.f;
					vColor = D3DXCOLOR(1.f, 1.f, 1.f, 0.6f);
				}
				break;
			case 'Z':	// z
				{
					wstrTextureName = L"DLG_UI_Common_Texture10.tga";
					wstrTextureKeyNormal = L"Z_NORMAL";
					wstrTextureKeyOver = L"Z_Over";
					offsetX = comboCommand.m_offsetX*24.f;
					offsetY = comboCommand.m_offsetY*25.f;
				}
				break;
			case 'X':	// x
				{
					wstrTextureName = L"DLG_UI_Common_Texture10.tga";
					wstrTextureKeyNormal = L"X_NORMAL";
					wstrTextureKeyOver = L"X_Over";
					offsetX = comboCommand.m_offsetX*24.f;
					offsetY = comboCommand.m_offsetY*25.f;
				}
				break;
			case '1': // Up Z
				{
					wstrTextureName = L"DLG_UI_Common_Texture10.tga";
					wstrTextureKeyNormal = L"JUMP_Z_NORMAL";
					wstrTextureKeyOver = L"JUMP_Z_OVER";
					offsetX = comboCommand.m_offsetX*24.f;
					offsetY = comboCommand.m_offsetY*25.f;
				}
				break;
			case '2': // Down Z
				{
					wstrTextureName = L"DLG_UI_Common_Texture11.tga";
					wstrTextureKeyNormal = L"Down_Z_Normal";
					wstrTextureKeyOver = L"Down_Z_Over";
					offsetX = comboCommand.m_offsetX*24.f;
					offsetY = comboCommand.m_offsetY*25.f;
					  
				}
				break;
			case '3': // Up X	 
				{
					wstrTextureName = L"DLG_UI_Common_Texture11.tga";
					wstrTextureKeyNormal = L"Jump_X_Normal";
					wstrTextureKeyOver = L"Jump_X_Over";
					offsetX = comboCommand.m_offsetX*24.f;
					offsetY = comboCommand.m_offsetY*25.f;
					
				}
				break;
			case '4': // Down X
				{
					wstrTextureName = L"DLG_UI_Common_Texture11.tga";
					wstrTextureKeyNormal = L"Down_X_Normal";
					wstrTextureKeyOver = L"Down_X_Over";
					offsetX = comboCommand.m_offsetX*24.f;
					offsetY = comboCommand.m_offsetY*25.f;
				}
				break;
			case '5': // Right Z
				{
					wstrTextureName = L"DLG_UI_Common_Texture10.tga";
					wstrTextureKeyNormal = L"R_Z_Normal";
					wstrTextureKeyOver = L"R_Z_Over";
					offsetX = comboCommand.m_offsetX*24.f;
					offsetY = comboCommand.m_offsetY*25.f;
				}
				break;
			case '6': // Left Z
				{
					wstrTextureName = L"DLG_UI_Common_Texture10.tga";
					wstrTextureKeyNormal = L"L_Z_Normal";
					wstrTextureKeyOver = L"L_Z_Over";
					offsetX = comboCommand.m_offsetX*24.f;
					offsetY = comboCommand.m_offsetY*25.f;
				}
				break;
			case '7': // Right X
				{
					wstrTextureName = L"DLG_UI_Common_Texture10.tga";
					wstrTextureKeyNormal = L"R_X_Normal";
					wstrTextureKeyOver = L"R_X_Over";
					offsetX = comboCommand.m_offsetX*24.f;
					offsetY = comboCommand.m_offsetY*25.f;
				}
				break;
			case '8': // Left X
				{
					wstrTextureName = L"DLG_UI_Common_Texture10.tga";
					wstrTextureKeyNormal = L"L_X_Normal";
					wstrTextureKeyOver = L"L_X_Over";
					offsetX = comboCommand.m_offsetX*24.f;
					offsetY = comboCommand.m_offsetY*25.f;
				}
				break;
			case 'J':	// jump
				{
					wstrTextureName = L"DLG_UI_Common_Texture10.tga";
					wstrTextureKeyNormal = L"Jump_Normal";
					wstrTextureKeyOver = L"Jump_Over";
					offsetX = comboCommand.m_offsetX*24.f;
					offsetY = comboCommand.m_offsetY*25.f;
				}
				break;
			case 'j':	// double jump
				{
					wstrTextureName = L"DLG_UI_Common_Texture11.tga";
					wstrTextureKeyNormal = L"Jump_Double_Normal";
					wstrTextureKeyOver = L"Jump_Double_Over";
					offsetX = comboCommand.m_offsetX*24.f;
					offsetY = comboCommand.m_offsetY*25.f;
				}
				break;
			case 'D':	// down
				{
					wstrTextureName = L"DLG_UI_Common_Texture11.tga";
					wstrTextureKeyNormal = L"Down_normal";
					wstrTextureKeyOver = L"Down_over";
					offsetX = comboCommand.m_offsetX*24.f;
					offsetY = comboCommand.m_offsetY*25.f;
				}
				break;
			case 'W':	// walk
				{
				}
				break;
			case 'R':	// run
				{
					wstrTextureName = L"DLG_UI_Common_Texture10.tga";
					wstrTextureKeyNormal = L"DASH_NORMAL";
					wstrTextureKeyOver = L"DASH_OVER";
					offsetX = comboCommand.m_offsetX*24.f;
					offsetY = comboCommand.m_offsetY*25.f;
				}
				break;
			case 'A':	// Dash Jump
				{
					wstrTextureName = L"DLG_UI_Common_Texture10.tga";
					wstrTextureKeyNormal = L"Dash_Jump_Normal";
					wstrTextureKeyOver = L"Dash_Jump_Over";
					offsetX = comboCommand.m_offsetX*24.f;
					offsetY = comboCommand.m_offsetY*25.f;
				}
				break;

//{{ kimhc // 2010.12.20 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
			case 'G':	// Guard
				{
					wstrTextureName = L"DLG_UI_Common_Texture11.tga";
					wstrTextureKeyNormal = L"CHUNG_guard";
					wstrTextureKeyOver = L"CHUNG_guard";
					offsetX = comboCommand.m_offsetX*24.f;
					offsetY = comboCommand.m_offsetY*25.f;
				} break;
#endif	NEW_CHARACTER_CHUNG
//}} kimhc // 2010.12.20 //  2010-12-23 New Character CHUNG
			default:
				break;
			}				

			if( wstrTextureKeyNormal != L"" )
			{
				CKTDGUIControl::UIPointData* pPointData = new CKTDGUIControl::UIPointData();
				pPointData->leftTopPoint = D3DXVECTOR2(offsetX, offsetY);
				pPointData->color = vColor;

				CKTDGUIControl::CPictureData *pPictureNormal = m_DlgComboGuide->CreatePicture();
				pComboStatic->AddPicture( pPictureNormal );
				if( pPictureNormal != NULL )
				{
					comboCommand.m_iIndex = (comboCommand.m_offsetY+1)*10 + (comboCommand.m_offsetX+1);
					pPictureNormal->SetIndex(comboCommand.m_iIndex);						
					pPictureNormal->SetTex( wstrTextureName.c_str(), wstrTextureKeyNormal.c_str() );
					pPictureNormal->SetPoint( pPointData );
				}
			}				

			if( wstrTextureKeyOver != L"" )
			{
				CKTDGUIControl::UIPointData* pPointData = new CKTDGUIControl::UIPointData();
				pPointData->leftTopPoint = D3DXVECTOR2(offsetX, offsetY);
				pPointData->color = vColor;


				CKTDGUIControl::CPictureData *pPictureOver = m_DlgComboGuide->CreatePicture();
				pComboStatic->AddPicture( pPictureOver );
				if( pPictureOver != NULL )
				{
					pPictureOver->SetIndex(100 + (comboCommand.m_offsetY+1)*10 + (comboCommand.m_offsetX+1));						
					pPictureOver->SetTex( wstrTextureName.c_str(), wstrTextureKeyOver.c_str() );
					pPictureOver->SetPoint( pPointData );
					pPictureOver->SetShow(false);
				}
			}
		}

		m_vecComboGuide.push_back(comboguide);		
		m_DlgComboGuide->AddControl( pComboStatic );		
	}	
}

void CX2ComboGuide::ShowComboCommand( wstring wstrComboId, wstring wstrCommand, bool bShow )
{
	if( m_DlgComboGuide == NULL )
		return;

	for(UINT i=0; i<m_vecComboGuide.size(); ++i)
	{
		ComboGuide &comboguide = m_vecComboGuide[i];	

		bool bComboId = false;
		if( comboguide.m_wstrComboId == wstrComboId )
			bComboId = true;


		CKTDGUIStatic *pStatic = (CKTDGUIStatic*)m_DlgComboGuide->GetControl( comboguide.m_wstrComboId.c_str() );
		if( pStatic == NULL )
			continue;

		for(UINT j=0; j<comboguide.m_vecCommand.size(); ++j)
		{
			ComboCommand comboCommand = comboguide.m_vecCommand[j];

			if(comboCommand.m_iIndex == 0)
				continue;

			if( comboCommand.m_wstrFullCommand == wstrCommand )
			{
				// over È°¼ºÈ­
				CKTDGUIControl::CPictureData *pPicture = pStatic->GetPictureIndex( comboCommand.m_iIndex + 100 );
				if(pPicture != NULL)
				{
					if( bShow == true && bComboId == true )
						pPicture->SetShow(true);
					else
						pPicture->SetShow(false);
				}					
				pPicture = pStatic->GetPictureIndex( comboCommand.m_iIndex );
				if(pPicture != NULL)
				{						
					if( bShow == false || bComboId == false )
						pPicture->SetShow( false );
				}

			}
			else
			{					
				CKTDGUIControl::CPictureData *pPicture = pStatic->GetPictureIndex( comboCommand.m_iIndex );
				if(pPicture != NULL)
				{						
					if( bShow == true && bComboId == true )
						pPicture->SetShow( true );
					else
						pPicture->SetShow( false );
				}

				pPicture = pStatic->GetPictureIndex( comboCommand.m_iIndex + 100 );
				if(pPicture != NULL)
				{
					if( bShow == false || bComboId == false )
						pPicture->SetShow(false);
				}
			}
		}
	}
}


