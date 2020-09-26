#include "dxstdafx.h"
#include "X2EffectToolEditor.h"
#include "X2EffectToolMeshEditor.h"
#include "X2EffectToolFileManager.h"
#include "resource.h"

CX2EffectToolMeshEditor* g_pMeshEditor = NULL;


#pragma region Struct
NameAndIndex g_PlayType[MAX_PLAY_TYPE] =
{			
	{L"XAP_LOOP",			CKTDGXSkinAnim::XAP_LOOP },
	{L"XAP_ONE",			CKTDGXSkinAnim::XAP_ONE },
	{L"XAP_ONE_WAIT",		CKTDGXSkinAnim::XAP_ONE_WAIT },
};
NameAndIndex g_MeshType[MAX_MESH_TYPE] =
{
	{L"X_MESH",      							CKTDGXMeshPlayer::X_MESH },
	{L"X_SKIN_MESH",      						CKTDGXMeshPlayer::X_SKIN_MESH },
};
NameAndIndex g_LifeEType[MAX_LIFE_TYPE] =
{
	{L"LT_PLAY_COUNT",		CKTDGXMeshPlayer::LT_PLAY_COUNT},
	{L"LT_TIME",			CKTDGXMeshPlayer::LT_TIME},
	{L"LT_MANUAL",			CKTDGXMeshPlayer::LT_MANUAL},
};
NameAndIndex g_CartoonTexType[MAX_CARTOON_TEX_TYPE] =
{
	{L"CTT_NORMAL",					CKTDGXRenderer::CTT_NORMAL},		
	{L"CTT_RED",					CKTDGXRenderer::CTT_RED},			
	{L"CTT_GREEN",					CKTDGXRenderer::CTT_GREEN},			
	{L"CTT_VIOLET",					CKTDGXRenderer::CTT_VIOLET},			
	{L"CTT_NORMAL_COUNTER",			CKTDGXRenderer::CTT_NORMAL_COUNTER},	
	{L"CTT_DAMAGE",					CKTDGXRenderer::CTT_DAMAGE},			
	{L"CTT_INVINCIBLE",				CKTDGXRenderer::CTT_INVINCIBLE},		
	{L"CTT_PETRIFIED",				CKTDGXRenderer::CTT_PETRIFIED},		
	{L"CTT_SKYBLUE",				CKTDGXRenderer::CTT_SKYBLUE},		
	{L"CTT_SCARLET",				CKTDGXRenderer::CTT_SCARLET},		
};
NameAndIndex g_CurveType[MAX_CURVE_TYPE] =
{
	{L"CT_SMOOTH_CURVE",			CKTDGSlashTrace::CT_SMOOTH_CURVE},
	{L"CT_TOUGH_CURVE",				CKTDGSlashTrace::CT_TOUGH_CURVE},	
	{L"CT_STRAIGHT_LINE",			CKTDGSlashTrace::CT_STRAIGHT_LINE},
};
NameAndIndex g_SlashTraceType[MAX_SLASH_TRACE_TYPE] =
{
	{L"STT_DEFAULT",				CKTDGSlashTrace::STT_DEFAULT},
	{L"STT_CONSTANT_WIDTH",			CKTDGSlashTrace::STT_CONSTANT_WIDTH},
	{L"STT_CIRCULAR",				CKTDGSlashTrace::STT_CIRCULAR},
	{L"STT_DOUBLE_TIP",				CKTDGSlashTrace::STT_DOUBLE_TIP},
	{L"STT_NONE",					CKTDGSlashTrace::STT_NONE},
};
NameAndIndex g_EventType_Mesh[MAX_MESH_EVENT_TYPE] =
{
	{L"ET_BASIC",			CKTDGXMeshPlayer::ET_BASIC},			
	{L"ET_ANISPEED",		CKTDGXMeshPlayer::ET_ANISPEED},		
	{L"ET_ANINAME",			CKTDGXMeshPlayer::ET_ANINAME},			
	{L"ET_COLOR",			CKTDGXMeshPlayer::ET_COLOR},			
	{L"ET_OUTLINECOLOR",	CKTDGXMeshPlayer::ET_OUTLINECOLOR},	
	{L"ET_SIZE",			CKTDGXMeshPlayer::ET_SIZE},			
	{L"ET_VELOCITY",		CKTDGXMeshPlayer::ET_VELOCITY},		
	{L"ET_TEX0UV",			CKTDGXMeshPlayer::ET_TEX0UV},			
	{L"ET_TEX1UV",			CKTDGXMeshPlayer::ET_TEX1UV},			
	{L"ET_TEX2UV",			CKTDGXMeshPlayer::ET_TEX2UV},			
	{L"ET_ROTATE",			CKTDGXMeshPlayer::ET_ROTATE},			
	{L"ET_DIRSPEED",		CKTDGXMeshPlayer::ET_DIRSPEED},		
	{L"ET_BLACKHOLETIME",	CKTDGXMeshPlayer::ET_BLACKHOLETIME},	
	{L"ET_CRASH",			CKTDGXMeshPlayer::ET_CRASH},			
	{L"ET_RESETCRASH",		CKTDGXMeshPlayer::ET_RESETCRASH},		
	{L"ET_LIGHTFLOWIMPACT",	CKTDGXMeshPlayer::ET_LIGHTFLOWIMPACT},	
	{L"ET_LIGHTFLOWPOINT",	CKTDGXMeshPlayer::ET_LIGHTFLOWPOINT},	
	{L"ET_SOUND",			CKTDGXMeshPlayer::ET_SOUND},			
	{L"ET_GLOBALTIMER",		CKTDGXMeshPlayer::ET_GLOBALTIMER},		
	{L"ET_SLASHTRACE",		CKTDGXMeshPlayer::ET_SLASHTRACE},		
	{L"ET_RENDERTYPE",		CKTDGXMeshPlayer::ET_RENDERTYPE},		
	{L"ET_POSITION",		CKTDGXMeshPlayer::ET_POSITION},		
};
NameAndIndex g_CullMode_Mesh[MAX_CULL_MODE] =
{
	{L"D3DCULL_NONE",					D3DCULL_NONE},		
	{L"D3DCULL_CW",						D3DCULL_CW},		
	{L"D3DCULL_CCW",					D3DCULL_CCW},		
	{L"D3DCULL_FORCE_DWORD",			D3DCULL_FORCE_DWORD},
};
NameAndIndex g_EventListTimeType_Mesh[MAX_EVENT_LIST_TIME_TYPE] =
{
	{L"Initital",      								ELTT_INITITAL },
	{L"At @1",      								ELTT_AT },
	{L"Fade So From @1 to @2",      				ELTT_FADE_SO_FROM },
	{L"Fade so Final",      						ELTT_FADE_SO_FINAL },
};
NameAndIndex g_BillBoardType_Mesh[MAX_BILL_BOARD_TYPE] =
{
	{L"BT_NONE",		CKTDGMatrix::BT_NONE},
	{L"BT_ALL",			CKTDGMatrix::BT_ALL},
	{L"BT_X",			CKTDGMatrix::BT_X},
	{L"BT_Y",			CKTDGMatrix::BT_Y},
	{L"BT_Z",			CKTDGMatrix::BT_Z},
};
NameAndIndex g_RenderType_Mesh[MAX_RENDER_TYPE] =
{
	{L"RT_REAL_COLOR",					CKTDGXRenderer::RT_REAL_COLOR},				
	{L"RT_CARTOON_BLACK_EDGE",			CKTDGXRenderer::RT_CARTOON_BLACK_EDGE},		
	{L"RT_CARTOON_COLOR_EDGE",			CKTDGXRenderer::RT_CARTOON_COLOR_EDGE},		
	{L"RT_CARTOON",						CKTDGXRenderer::RT_CARTOON},					
	{L"RT_ADD_CARTOON_BLACK_EDGE",		CKTDGXRenderer::RT_ADD_CARTOON_BLACK_EDGE},	
	{L"RT_ADD_CARTOON_COLOR_EDGE",		CKTDGXRenderer::RT_ADD_CARTOON_COLOR_EDGE},	
	{L"RT_ADD_CARTOON",					CKTDGXRenderer::RT_ADD_CARTOON},				
	{L"RT_BLACK_EDGE",					CKTDGXRenderer::RT_BLACK_EDGE},				
	{L"RT_COLOR_EDGE",					CKTDGXRenderer::RT_COLOR_EDGE},		
	{L"RT_CARTOON_FADE",				CKTDGXRenderer::RT_CARTOON_FADE},				
	{L"RT_CARTOON_FADE_NONTEX",			CKTDGXRenderer::RT_CARTOON_FADE_NONTEX},
};
NameAndIndex g_BlendMode_Mesh[MAX_BLEND_MODE] =
{
	{L"D3DBLEND_ZERO",					D3DBLEND_ZERO},				
	{L"D3DBLEND_ONE",					D3DBLEND_ONE},				
	{L"D3DBLEND_SRCCOLOR",				D3DBLEND_SRCCOLOR},			
	{L"D3DBLEND_INVSRCCOLOR",			D3DBLEND_INVSRCCOLOR},		
	{L"D3DBLEND_SRCALPHA",				D3DBLEND_SRCALPHA},			
	{L"D3DBLEND_INVSRCALPHA",			D3DBLEND_INVSRCALPHA},		
	{L"D3DBLEND_DESTALPHA",				D3DBLEND_DESTALPHA},		
	{L"D3DBLEND_INVDESTALPHA",			D3DBLEND_INVDESTALPHA},		
	{L"D3DBLEND_DESTCOLOR",				D3DBLEND_DESTCOLOR},		
	{L"D3DBLEND_INVDESTCOLOR",			D3DBLEND_INVDESTCOLOR},		
	{L"D3DBLEND_SRCALPHASAT",			D3DBLEND_SRCALPHASAT},		
	{L"D3DBLEND_BOTHSRCALPHA",			D3DBLEND_BOTHSRCALPHA},		
	{L"D3DBLEND_BOTHINVSRCALPHA",		D3DBLEND_BOTHINVSRCALPHA},	
	{L"D3DBLEND_BLENDFACTOR",			D3DBLEND_BLENDFACTOR},		
	{L"D3DBLEND_INVBLENDFACTOR",		D3DBLEND_INVBLENDFACTOR},	
};
#pragma endregion Struct

int CX2EffectToolMeshEditor::GetIndexByEnum( CKTDGXSkinAnim::XSKIN_ANIM_PLAYTYPE eEnumValue )
{
	for( int i = 0; i < MAX_PLAY_TYPE ; ++i )
	{
		if( eEnumValue == g_PlayType[i].iIndex )
		{
			return i;
		}
	}
	return -1;
}
int CX2EffectToolMeshEditor::GetIndexByEnum( CKTDGXMeshPlayer::MESH_TYPE eEnumValue )
{
	for( int i = 0; i < MAX_MESH_TYPE ; ++i )
	{
		if( eEnumValue == g_MeshType[i].iIndex )
		{
			return i;
		}
	}
	return -1;
}
int CX2EffectToolMeshEditor::GetIndexByEnum( CKTDGXMeshPlayer::LIFE_TYPE eEnumValue)
{
	for( int i = 0; i < MAX_LIFE_TYPE ; ++i )
	{
		if( eEnumValue == g_LifeEType[i].iIndex )
		{
			return i;
		}
	}
	return -1;
}
int CX2EffectToolMeshEditor::GetIndexByEnum( CKTDGXRenderer::CARTOON_TEX_TYPE eEnumValue )
{
	for( int i = 0; i < MAX_CARTOON_TEX_TYPE ; ++i )
	{
		if( eEnumValue == g_CartoonTexType[i].iIndex )
		{
			return i;
		}
	}
	return -1;
}
int CX2EffectToolMeshEditor::GetIndexByEnum( CKTDGSlashTrace::CURVE_TYPE eEnumValue)
{
	for( int i = 0; i < MAX_CURVE_TYPE ; ++i )
	{
		if( eEnumValue == g_CurveType[i].iIndex )
		{
			return i;
		}
	}
	return -1;
}
int CX2EffectToolMeshEditor::GetIndexByEnum( CKTDGSlashTrace::SLASH_TRACE_TYPE eEnumValue  )
{
	for( int i = 0; i < MAX_SLASH_TRACE_TYPE ; ++i )
	{
		if( eEnumValue == g_SlashTraceType[i].iIndex )
		{
			return i;
		}
	}
	return -1;
}
int CX2EffectToolMeshEditor::GetIndexByEnum( CKTDGXMeshPlayer::EVENT_TYPE eEnumValue  )
{
	for( int i = 0; i < MAX_MESH_EVENT_TYPE ; ++i )
	{
		if( eEnumValue == g_EventType_Mesh[i].iIndex )
		{
			return i;
		}
	}
	return -1;
}
int CX2EffectToolMeshEditor::GetIndexByEnum( D3DBLEND eEnumValue )
{
	for( int i = 0; i < MAX_BLEND_MODE ; ++i )
	{
		if( eEnumValue == g_BlendMode_Mesh[i].iIndex )
		{
			return i;
		}
	}
	return -1;
}
int CX2EffectToolMeshEditor::GetIndexByEnum( _D3DCULL  eEnumValue  )
{
	for( int i = 0; i < MAX_CULL_MODE ; ++i )
	{
		if( eEnumValue == g_CullMode_Mesh[i].iIndex )
		{
			return i;
		}
	}
	return -1;
}
int CX2EffectToolMeshEditor::GetIndexByEnum( CKTDGXRenderer::RENDER_TYPE eEnumValue )
{
	for( int i = 0; i < MAX_RENDER_TYPE ; ++i )
	{
		if( eEnumValue == g_RenderType_Mesh[i].iIndex )
		{
			return i;
		}
	}
	return -1;
}
int CX2EffectToolMeshEditor::GetIndexByEnum( CKTDGMatrix::BILLBOARD_TYPE  eEnumValue  )
{
	for( int i = 0; i < MAX_BILL_BOARD_TYPE ; ++i )
	{
		if( eEnumValue == g_BillBoardType_Mesh[i].iIndex )
		{
			return i;
		}
	}
	return -1;
}
int CX2EffectToolMeshEditor::GetIndexByEnum( EVENT_LIST_TIME_TYPE eEnumValue )
{
	for( int i = 0; i < MAX_EVENT_LIST_TIME_TYPE ; ++i )
	{
		if( eEnumValue == g_EventListTimeType_Mesh[i].iIndex )
		{
			return i;
		}
	}
	return -1;
}

BOOL CALLBACK MeshEditorDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	if( NULL == g_pMeshEditor )
		return FALSE;


	OPENFILENAME ofn;        // common dialog box structure
	WCHAR wszFileName[512];  // path까지 포함한 파일 이름

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize		= sizeof(ofn);
	ofn.hwndOwner		= g_pKTDXApp->GetHWND(); 
	ofn.lpstrFile		= (LPWSTR)wszFileName;
	ofn.lpstrFile[0]	= '\0';
	ofn.nMaxFile		= sizeof(wszFileName);
	ofn.nFilterIndex	= 1;
	ofn.lpstrFileTitle	= NULL;
	ofn.nMaxFileTitle	= 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags			= OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	switch(LOWORD(wParam))
	{
#pragma region PA_LIST_EventList
	case PA_LIST_EventList_Global:
		switch(HIWORD(wParam))
		{
		case LBN_SELCHANGE:
			{
				int index = SendDlgItemMessage(hDlg, PA_LIST_EventList_Global, LB_GETCURSEL, 0, 0);
				g_pMeshEditor->ON_PA_LIST_EventList_Global_SelChnage( index );
			}
			return TRUE;
		}
		return TRUE;
	case PA_LIST_EventList_Anim:
		switch(HIWORD(wParam))
		{
		case LBN_SELCHANGE:
			{
				int index = SendDlgItemMessage(hDlg, PA_LIST_EventList_Anim, LB_GETCURSEL, 0, 0);
				g_pMeshEditor->ON_PA_LIST_EventList_Anim_SelChnage( index );
			}
			return TRUE;
		}
		return TRUE;
	case PA_LIST_EventList_Die:
		switch(HIWORD(wParam))
		{
		case LBN_SELCHANGE:
			{
				int index = SendDlgItemMessage(hDlg, PA_LIST_EventList_Die, LB_GETCURSEL, 0, 0);
				g_pMeshEditor->ON_PA_LIST_EventList_Die_SelChnage( index );
			}
			return TRUE;
		}
		return TRUE;
#pragma endregion PA_LIST_EventList
	case PA_COMBO_EventType:
		switch(HIWORD(wParam))
		{
		case CBN_SELCHANGE:
			{
// 				CKTDGParticleSystem::EVENT_TYPE eEventType = sfasfsfd
// 					static_cast<CKTDGParticleSystem::EVENT_TYPE>(SendDlgItemMessage(hDlg, PA_COMBO_EventType, CB_GETCURSEL, 0, 0));
// 				if(eEventType != -1)
// 				{   
// 					int iEventIndex = SendDlgItemMessage(hDlg, PA_LIST_EventList, LB_GETCURSEL, 0, 0);
// 					CKTDGParticleSystem::CParticleEvent* pParticleEvent = g_pMeshEditor->GetEventDataByIndex(iEventIndex);
// 
// 					if( NULL != pParticleEvent )
// 					{
// 						if( eEventType == pParticleEvent->GetEventType() )
// 							g_pMeshEditor->UpdateEventData( pParticleEvent );
// 						else
// 							g_pMeshEditor->InitEventDataValue( eEventType );
// 					}
// 					// 새롭게 추가하는 경우라면 NULL임.
// 					// 컨테이너에 존재하지 않고 UI에서만 출력.
// 					else
// 					{
// 						g_pMeshEditor->InitEventDataValue( eEventType );
// 					}
// 				}
			}                               
		}
	}

	switch(iMessage)
	{
#pragma region WM_CREATE
	case WM_CREATE:
	case WM_INITDIALOG:
		{
			for(int i=0; i<MAX_MESH_TYPE; ++i)
			{
				SendDlgItemMessage(hDlg, PA_COMBO_MeshType, CB_ADDSTRING,      0, (LPARAM)g_MeshType[i].strName);
				SendDlgItemMessage(hDlg, PA_COMBO_MeshType, CB_SETITEMDATA,    i, (LPARAM)g_MeshType[i].iIndex);
			}
			for(int i=0; i<MAX_EVENT_LIST_TIME_TYPE; ++i)
			{
				SendDlgItemMessage(hDlg, PA_COMBO_TimeType, CB_ADDSTRING,      0, (LPARAM)g_EventListTimeType_Mesh[i].strName);
				SendDlgItemMessage(hDlg, PA_COMBO_TimeType, CB_SETITEMDATA,    i, (LPARAM)g_EventListTimeType_Mesh[i].iIndex);
			}
			for(int i=0; i<MAX_MESH_EVENT_TYPE; ++i)
			{
				SendDlgItemMessage(hDlg, PA_COMBO_EventType, CB_ADDSTRING,      0, (LPARAM)g_EventType_Mesh[i].strName);
				SendDlgItemMessage(hDlg, PA_COMBO_EventType, CB_SETITEMDATA,    i, (LPARAM)g_EventType_Mesh[i].iIndex);
			}
			for(int i=0; i<MAX_BILL_BOARD_TYPE; ++i)
			{
				SendDlgItemMessage(hDlg, PA_COMBO_Billboard, CB_ADDSTRING,      0, (LPARAM)g_BillBoardType_Mesh[i].strName);
				SendDlgItemMessage(hDlg, PA_COMBO_Billboard, CB_SETITEMDATA,    i, (LPARAM)g_BillBoardType_Mesh[i].iIndex);
			}
			for(int i=0; i<MAX_RENDER_TYPE; ++i)
			{
				SendDlgItemMessage(hDlg, PA_COMBO_MeshRender, CB_ADDSTRING,      0, (LPARAM)g_RenderType_Mesh[i].strName);
				SendDlgItemMessage(hDlg, PA_COMBO_MeshRender, CB_SETITEMDATA,    i, (LPARAM)g_RenderType_Mesh[i].iIndex);
			}
			for(int i=0; i<MAX_BLEND_MODE; ++i)
			{
				SendDlgItemMessage(hDlg, PA_COMBO_SourceBlend, CB_ADDSTRING,      0, (LPARAM)g_BlendMode_Mesh[i].strName);
				SendDlgItemMessage(hDlg, PA_COMBO_SourceBlend, CB_SETITEMDATA,    i, (LPARAM)g_BlendMode_Mesh[i].iIndex);
			}
			for(int i=0; i<MAX_BLEND_MODE; ++i)
			{
				SendDlgItemMessage(hDlg, PA_COMBO_DestBlend, CB_ADDSTRING,      0, (LPARAM)g_BlendMode_Mesh[i].strName);
				SendDlgItemMessage(hDlg, PA_COMBO_DestBlend, CB_SETITEMDATA,    i, (LPARAM)g_BlendMode_Mesh[i].iIndex);
			}
			for(int i=0; i<MAX_CULL_MODE; ++i)
			{
				SendDlgItemMessage(hDlg, PA_COMBO_CullMode, CB_ADDSTRING,      0, (LPARAM)g_CullMode_Mesh[i].strName);
				SendDlgItemMessage(hDlg, PA_COMBO_CullMode, CB_SETITEMDATA,    i, (LPARAM)g_CullMode_Mesh[i].iIndex);
			}
			for(int i=0; i<MAX_LIFE_TYPE; ++i)
			{
				SendDlgItemMessage(hDlg, PA_COMBO_LifeType, CB_ADDSTRING,      0, (LPARAM)g_LifeEType[i].strName);
				SendDlgItemMessage(hDlg, PA_COMBO_LifeType, CB_SETITEMDATA,    i, (LPARAM)g_LifeEType[i].iIndex);
			}
			for(int i=0; i<MAX_CARTOON_TEX_TYPE; ++i)
			{
				SendDlgItemMessage(hDlg, PA_COMBO_CartoonTexType, CB_ADDSTRING,      0, (LPARAM)g_CartoonTexType[i].strName);
				SendDlgItemMessage(hDlg, PA_COMBO_CartoonTexType, CB_SETITEMDATA,    i, (LPARAM)g_CartoonTexType[i].iIndex);
			}
			for(int i=0; i<MAX_CURVE_TYPE; ++i)
			{
				SendDlgItemMessage(hDlg, PA_COMBO_TraceCurveType, CB_ADDSTRING,      0, (LPARAM)g_CurveType[i].strName);
				SendDlgItemMessage(hDlg, PA_COMBO_TraceCurveType, CB_SETITEMDATA,    i, (LPARAM)g_CurveType[i].iIndex);
			}
			for(int i=0; i<MAX_SLASH_TRACE_TYPE; ++i)
			{
				SendDlgItemMessage(hDlg, PA_COMBO_SlashTraceType, CB_ADDSTRING,      0, (LPARAM)g_SlashTraceType[i].strName);
				SendDlgItemMessage(hDlg, PA_COMBO_SlashTraceType, CB_SETITEMDATA,    i, (LPARAM)g_SlashTraceType[i].iIndex);
			}
			for(int i=0; i<MAX_LIFE_TYPE; ++i)
			{
				SendDlgItemMessage(hDlg, PA_COMBO_DieType, CB_ADDSTRING,      0, (LPARAM)g_LifeEType[i].strName);
				SendDlgItemMessage(hDlg, PA_COMBO_DieType, CB_SETITEMDATA,    i, (LPARAM)g_LifeEType[i].iIndex);
			}
			for(int i=0; i<MAX_PLAY_TYPE; ++i)
			{
				SendDlgItemMessage(hDlg, PA_COMBO_PlayType, CB_ADDSTRING,      0, (LPARAM)g_PlayType[i].strName);
				SendDlgItemMessage(hDlg, PA_COMBO_PlayType, CB_SETITEMDATA,    i, (LPARAM)g_PlayType[i].iIndex);
			}
			for(int i=0; i<MAX_RENDER_TYPE; ++i)
			{
				SendDlgItemMessage(hDlg, PA_COMBO_EVENT_RENDER_TYPE, CB_ADDSTRING,      0, (LPARAM)g_RenderType_Mesh[i].strName);
				SendDlgItemMessage(hDlg, PA_COMBO_EVENT_RENDER_TYPE, CB_SETITEMDATA,    i, (LPARAM)g_RenderType_Mesh[i].iIndex);
			}
			
		}
		return TRUE;    
#pragma endregion WM_CREATE
#pragma region WM_COMMAND
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDOK:
			{
				g_pMeshEditor->SaveMeshData();
			} return TRUE;
		case IDCANCEL:
			{
				g_pMeshEditor->CloseDialog();
			} return TRUE;
		case PA_BUTTON_EVENT_STRING:
			{
				ofn.lpstrFilter		= L"Texture\0*.dds\0Texture\0*.tga\0";
				if( TRUE == GetOpenFileName( &ofn ) )
				{
					// file path와 name을 분리
					WCHAR drive[_MAX_DRIVE];
					WCHAR dir[_MAX_DIR];
					WCHAR fname[_MAX_FNAME];
					WCHAR ext[_MAX_EXT];
					_wsplitpath( ofn.lpstrFile, drive, dir, fname, ext);

					wstring wszTemp = fname;
					wszTemp += ext;
					SetWindowText(GetDlgItem( hDlg , PA_EDIT_STRING), wszTemp.c_str() );
				}
			} return TRUE; //case PA_BUTTON_EVENT_STRING:
		case PA_BUTTON_MESH_NAME_STRING:
			{
				ofn.lpstrFilter		= L"XMesh\0*.x\0YMesh\0*.y\0";
				if( TRUE == GetOpenFileName( &ofn ) )
				{
					// file path와 name을 분리
					WCHAR drive[_MAX_DRIVE];
					WCHAR dir[_MAX_DIR];
					WCHAR fname[_MAX_FNAME];
					WCHAR ext[_MAX_EXT];
					_wsplitpath( ofn.lpstrFile, drive, dir, fname, ext);

					wstring wszTemp = fname;
					wszTemp += ext;
					SetWindowText(GetDlgItem( hDlg , PA_EDIT_MeshName), wszTemp.c_str() );
				}
			} return TRUE;
		case PA_IDC_EvnetList_Global_ADD:
			{
				g_pMeshEditor->ON_PA_IDC_EvnetList_Global_ADD();
			} return TRUE; 
		case PA_IDC_EvnetList_Global_DEL:
			{
				g_pMeshEditor->ON_PA_IDC_EvnetList_Global_DEL();
			} return TRUE;
		case PA_IDC_EvnetList_Anim_ADD:
			{
				g_pMeshEditor->ON_PA_IDC_EvnetList_Anim_ADD();
			} return TRUE; 
		case PA_IDC_EvnetList_Anim_DEL:
			{
				g_pMeshEditor->ON_PA_IDC_EvnetList_Anim_DEL();
			} return TRUE;
		case PA_IDC_EvnetList_Die_ADD:
			{
				g_pMeshEditor->ON_PA_IDC_EvnetList_Die_ADD();
			} return TRUE; 
		case PA_IDC_EvnetList_Die_DEL:
			{
				g_pMeshEditor->ON_PA_IDC_EvnetList_Die_DEL();
			} return TRUE;
		// UP / DOWN 버튼
		case PA_IDC_EvnetList_Global_UP:
			{
				g_pMeshEditor->ON_PA_IDC_EvnetList_Global_UP();
			} return TRUE; 
		case PA_IDC_EvnetList_Global_DOWN:
			{
				g_pMeshEditor->ON_PA_IDC_EvnetList_Global_DOWN();
			} return TRUE;
		case PA_IDC_EvnetList_Anim_UP:
			{
				g_pMeshEditor->ON_PA_IDC_EvnetList_Anim_UP();
			} return TRUE; 
		case PA_IDC_EvnetList_Anim_DOWN:
			{
				g_pMeshEditor->ON_PA_IDC_EvnetList_Anim_DOWN();
			} return TRUE;
		case PA_IDC_EvnetList_Die_UP:
			{
				g_pMeshEditor->ON_PA_IDC_EvnetList_Die_UP();
			} return TRUE; 
		case PA_IDC_EvnetList_Die_DOWN:
			{
				g_pMeshEditor->ON_PA_IDC_EvnetList_Die_DOWN();
			} return TRUE;
		}
		return TRUE;
#pragma endregion WM_COMMAND
	case WM_KEYDOWN:
		{
			switch( wParam )
			{
			case VK_F5:
				{	
		//			g_pMeshEditor->SaveParticleData();
				}
			}
		}
	}

	return FALSE;
}


CX2EffectToolMeshEditor::CX2EffectToolMeshEditor( CX2EffectToolMain* pEffectTool_):
m_pEffectTool( pEffectTool_ )
,m_eSelectEventListType ( ELT_NONE )
,m_pMeshTemplet(NULL)
{
}


CX2EffectToolMeshEditor::~CX2EffectToolMeshEditor(void)
{
	CloseDialog();
}

void CX2EffectToolMeshEditor::CloseDialog()
{
	DestroyWindow( m_hDialogEditor ) ;
	m_hDialogEditor = NULL;
}
void CX2EffectToolMeshEditor::CreateDialogMeshEditor( CKTDGXMeshPlayer::XMeshTemplet* pMeshTemplet_ /*= NULL*/)
{
	if( NULL == m_hDialogEditor )
	{
		g_pMeshEditor = this;
		m_hDialogEditor = CreateDialog(g_pKTDXApp->GetHInstance(), 
			MAKEINTRESOURCE(IDD_MESH_EDITOR), 
			g_pKTDXApp->GetHWND(), 
			MeshEditorDlgProc);
		ShowWindow(m_hDialogEditor, SW_SHOW);
	}

	if( NULL != pMeshTemplet_ )
	{
		UpdateMeshData( pMeshTemplet_ );
	}
}

void CX2EffectToolMeshEditor::UpdateMeshData( CKTDGXMeshPlayer::XMeshTemplet* pMeshTemplet_ )
{
	if( NULL == m_hDialogEditor)
		return;

	if( NULL == pMeshTemplet_ )
		return;

	m_pMeshTemplet = pMeshTemplet_;

	SendDlgItemMessage( m_hDialogEditor , PA_COMBO_MeshType, CB_SETCURSEL, (WPARAM)GetIndexByEnum(pMeshTemplet_->meshType), 0);
	SendDlgItemMessage( m_hDialogEditor , PA_COMBO_Billboard, CB_SETCURSEL, (WPARAM)GetIndexByEnum(pMeshTemplet_->billBoardType), 0);
	SendDlgItemMessage( m_hDialogEditor , PA_COMBO_MeshRender, CB_SETCURSEL, (WPARAM)GetIndexByEnum(pMeshTemplet_->renderType), 0);	
	SendDlgItemMessage( m_hDialogEditor , PA_COMBO_SourceBlend, CB_SETCURSEL, (WPARAM)GetIndexByEnum( pMeshTemplet_->srcBlend ), 0);

	SendDlgItemMessage( m_hDialogEditor , PA_COMBO_DestBlend, CB_SETCURSEL, (WPARAM)GetIndexByEnum(pMeshTemplet_->destBlend), 0);
	SendDlgItemMessage( m_hDialogEditor , PA_COMBO_CullMode, CB_SETCURSEL, (WPARAM)GetIndexByEnum(pMeshTemplet_->cullMode), 0);
	SendDlgItemMessage( m_hDialogEditor , PA_COMBO_LifeType, CB_SETCURSEL, (WPARAM)GetIndexByEnum(pMeshTemplet_->lifeType), 0);
	SendDlgItemMessage( m_hDialogEditor , PA_COMBO_CartoonTexType, CB_SETCURSEL, (WPARAM)GetIndexByEnum(pMeshTemplet_->cartoonTexType), 0);
	SendDlgItemMessage( m_hDialogEditor , PA_COMBO_TraceCurveType, CB_SETCURSEL, (WPARAM)GetIndexByEnum(pMeshTemplet_->slashTraceCurveType), 0);
	SendDlgItemMessage( m_hDialogEditor , PA_COMBO_SlashTraceType, CB_SETCURSEL, (WPARAM)GetIndexByEnum(static_cast<CKTDGSlashTrace::SLASH_TRACE_TYPE>(pMeshTemplet_->iSlashTraceType)), 0);
	SendDlgItemMessage( m_hDialogEditor , PA_COMBO_DieType, CB_SETCURSEL, (WPARAM)GetIndexByEnum(pMeshTemplet_->dieType), 0);

	SetWindowText(GetDlgItem( m_hDialogEditor, PA_EDIT_MESH_TEMPLET_NAME), pMeshTemplet_->templetName.c_str() );

	char buf[100];
	sprintf(buf, "%d", pMeshTemplet_->layer );
	SetWindowTextA(GetDlgItem( m_hDialogEditor, PA_EDIT_Layer), buf);

	sprintf(buf, "%0.3f", pMeshTemplet_->fNotCheckLandTime );
	SetWindowTextA(GetDlgItem( m_hDialogEditor, PA_EDIT_NotCheckLandTime), buf);

	SetWindowText(GetDlgItem( m_hDialogEditor, PA_EDIT_ChangeTexXetName), pMeshTemplet_->changeTexXETName.c_str() );
	SetWindowText(GetDlgItem( m_hDialogEditor, PA_EDIT_MultiTexXetName), pMeshTemplet_->multiTexXETName.c_str() );
	SetWindowText(GetDlgItem( m_hDialogEditor, PA_EDIT_AniXetName), pMeshTemplet_->aniXETName.c_str() );
	SetWindowText(GetDlgItem( m_hDialogEditor, PA_EDIT_MeshName), pMeshTemplet_->meshName.c_str() );
	SetWindowText(GetDlgItem( m_hDialogEditor, PA_EDIT_SlashTraceTextureName), pMeshTemplet_->slashTraceTextureName.c_str() );

	SendDlgItemMessage(m_hDialogEditor, PA_IDC_ZEnable, BM_SETCHECK, (WPARAM)pMeshTemplet_->bZEnable, 0);	
	SendDlgItemMessage(m_hDialogEditor, PA_IDC_ApplyUnitScale, BM_SETCHECK, (WPARAM)pMeshTemplet_->bApplyUnitScale, 0);	
	SendDlgItemMessage(m_hDialogEditor, PA_IDC_UseLand, BM_SETCHECK, (WPARAM)pMeshTemplet_->bUseLand, 0);	
	SendDlgItemMessage(m_hDialogEditor, PA_IDC_bForceLayer, BM_SETCHECK, (WPARAM)pMeshTemplet_->bForceLayer, 0);	
	SendDlgItemMessage(m_hDialogEditor, PA_IDC_ZWriteEnable, BM_SETCHECK, (WPARAM)pMeshTemplet_->bZWriteEnable, 0);	
	SendDlgItemMessage(m_hDialogEditor, PA_IDC_SetLandHeightOnStart, BM_SETCHECK, (WPARAM)pMeshTemplet_->bSetLandHeightOnStart, 0);	
	SendDlgItemMessage(m_hDialogEditor, PA_IDC_CrashLand, BM_SETCHECK, (WPARAM)pMeshTemplet_->bCrashLand, 0);	
	SendDlgItemMessage(m_hDialogEditor, PA_IDC_USE_SLASH_TRACE, BM_SETCHECK, (WPARAM)pMeshTemplet_->bUseSlashTrace, 0);	
	SendDlgItemMessage(m_hDialogEditor, PA_IDC_bUseDieEvent, BM_SETCHECK, (WPARAM)pMeshTemplet_->bUseDieEvent , 0);	


	vector<int> vecControlID;
	vecControlID.push_back( PA_EDIT_PlayCount_Min );
	vecControlID.push_back( PA_EDIT_PlayCount_Max );
	SetEditControlByValue( &pMeshTemplet_->playCount, vecControlID);

	vecControlID.clear();
	vecControlID.push_back( PA_EDIT_DrawCount_Min );
	vecControlID.push_back( PA_EDIT_DrawCount_Max );
	SetEditControlByValue( &pMeshTemplet_->drawCount, vecControlID);

	vecControlID.clear();
	vecControlID.push_back( PA_EDIT_LifeTime_Min );
	vecControlID.push_back( PA_EDIT_LifeTime_Max );
	SetEditControlByValue( &pMeshTemplet_->lifeTime, vecControlID);

	vecControlID.clear();
	vecControlID.push_back(	PA_EDIT_LandPosition_Min );
	vecControlID.push_back( PA_EDIT_LandPosition_Max );
	SetEditControlByValue( &pMeshTemplet_->landPosition, vecControlID);

	vecControlID.clear();
	vecControlID.push_back( PA_EDIT_OutLineWide_Min );
	vecControlID.push_back( PA_EDIT_OutLineWide_Max );
	SetEditControlByValue( &pMeshTemplet_->outLineWide, vecControlID);

	vecControlID.clear();
	vecControlID.push_back( PA_EDIT_LightFlowWide_Min );
	vecControlID.push_back( PA_EDIT_LightFlowWide_Max);
	SetEditControlByValue( &pMeshTemplet_->lightFlowWide, vecControlID);

	vecControlID.clear();
	vecControlID.push_back( PA_EDIT_elasticCoeff_Min );
	vecControlID.push_back( PA_EDIT_elasticCoeff_Max);
	SetEditControlByValue( &pMeshTemplet_->elasticCoeff, vecControlID);

	vecControlID.clear();
	vecControlID.push_back( PA_EDIT_elasticCoeffX_Min );
	vecControlID.push_back( PA_EDIT_elasticCoeffX_Max);
	SetEditControlByValue( &pMeshTemplet_->elasticCoeffX, vecControlID);

	vecControlID.clear();
	vecControlID.push_back( PA_EDIT_Gravity_MinX );
	vecControlID.push_back( PA_EDIT_Gravity_MinY );
	vecControlID.push_back( PA_EDIT_Gravity_MinZ );
	vecControlID.push_back( PA_EDIT_Gravity_MaxX );
	vecControlID.push_back( PA_EDIT_Gravity_MaxY );
	vecControlID.push_back( PA_EDIT_Gravity_MaxZ );
	SetEditControlByValue( &pMeshTemplet_->gravity, vecControlID);

	vecControlID.clear();
	vecControlID.push_back( PA_EDIT_LightPos_MinX );
	vecControlID.push_back( PA_EDIT_LightPos_MinY);
	vecControlID.push_back( PA_EDIT_LightPos_MinZ );
	vecControlID.push_back( PA_EDIT_LightPos_MaxX );
	vecControlID.push_back( PA_EDIT_LightPos_MaxY );
	vecControlID.push_back( PA_EDIT_LightPos_MaxZ );
	SetEditControlByValue( &pMeshTemplet_->lightPos, vecControlID);

	vecControlID.clear();
	vecControlID.push_back( PA_EDIT_BlackHolePos_MinX );
	vecControlID.push_back( PA_EDIT_BlackHolePos_MinY );
	vecControlID.push_back( PA_EDIT_BlackHolePos_MinZ );
	vecControlID.push_back( PA_EDIT_BlackHolePos_MaxX );
	vecControlID.push_back( PA_EDIT_BlackHolePos_MaxY );
	vecControlID.push_back( PA_EDIT_BlackHolePos_MaxZ );
	SetEditControlByValue( &pMeshTemplet_->blackHolePosition, vecControlID);

	vecControlID.clear();
	vecControlID.push_back( PA_EDIT_DiePlayCount_Min );
	vecControlID.push_back( PA_EDIT_DiePlayCount_Max );
	SetEditControlByValue( &pMeshTemplet_->diePlayCount, vecControlID);

	vecControlID.clear();
	vecControlID.push_back( PA_EDIT_DieLifeTime_Min );
	vecControlID.push_back( PA_EDIT_DieLifeTime_Max );
	SetEditControlByValue( &pMeshTemplet_->dieLifeTime, vecControlID);

	UpdateEventList();
}


void CX2EffectToolMeshEditor::UpdateGlobalEventDataByIndex( int iIndex_ )
{
	CKTDGXMeshPlayer::CXMeshEvent* pSelectEventData = GetGlobalEventDataByIndex(iIndex_);

	if( NULL != pSelectEventData )
	{
		UpdateEventData( pSelectEventData );
	}
	/*
	else
		{
	// 		SendDlgItemMessage( m_hMeshEditor , PA_COMBO_TimeType, CB_SETCURSEL, (WPARAM)0, 0);
	// 		SetWindowTextA(GetDlgItem( m_hMeshEditor, PA_EDIT_TimeValue1), "0");// 		SetWindowTextA(GetDlgItem( m_hMeshEditor, PA_EDIT_TimeValue2), "0");// 		InitEventDataValue();		;	}*/
}
CKTDGXMeshPlayer::CXMeshEvent* CX2EffectToolMeshEditor::GetGlobalEventDataByIndex( int iIndex_ )
{
	if( 0 > iIndex_ )
		return NULL;

	int iIndex = 0;
	BOOST_FOREACH (CKTDGXMeshPlayer::CXMeshEvent* pEventList, m_pMeshTemplet->globalTimeEventList )
	{
		if( iIndex_ == iIndex++ )
		{
			return pEventList;
		}
	}

	return NULL;
}
void CX2EffectToolMeshEditor::UpdateAnimEventDataByIndex( int iIndex_ )
{
	CKTDGXMeshPlayer::CXMeshEvent* pSelectEventData = GetAnimEventDataByIndex(iIndex_);

	if( NULL != pSelectEventData )
	{
		UpdateEventData( pSelectEventData );
	}
/*
	else
	{
		// 		SendDlgItemMessage( m_hMeshEditor , PA_COMBO_TimeType, CB_SETCURSEL, (WPARAM)0, 0);
		// 		SetWindowTextA(GetDlgItem( m_hMeshEditor, PA_EDIT_TimeValue1), "0");		// 		SetWindowTextA(GetDlgItem( m_hMeshEditor, PA_EDIT_TimeValue2), "0");		// 		InitEventDataValue();		;	}*/

}
CKTDGXMeshPlayer::CXMeshEvent* CX2EffectToolMeshEditor::GetAnimEventDataByIndex( int iIndex_ )
{
	if( 0 > iIndex_ )
		return NULL;

	int iIndex = 0;
	BOOST_FOREACH (CKTDGXMeshPlayer::CXMeshEvent* pEventList, m_pMeshTemplet->animTimeEventList )
	{
		if( iIndex_ == iIndex++ )
		{
			return pEventList;
		}
	}
	return NULL;
}
void CX2EffectToolMeshEditor::UpdateDieEventDataByIndex( int iIndex_ )
{
	CKTDGXMeshPlayer::CXMeshEvent* pSelectEventData = GetDieEventDataByIndex(iIndex_);

	if( NULL != pSelectEventData )
	{
		UpdateEventData( pSelectEventData );
	}
/*
	else
	{
		// 		SendDlgItemMessage( m_hMeshEditor , PA_COMBO_TimeType, CB_SETCURSEL, (WPARAM)0, 0);
		// 		SetWindowTextA(GetDlgItem( m_hMeshEditor, PA_EDIT_TimeValue1), "0");		// 		SetWindowTextA(GetDlgItem( m_hMeshEditor, PA_EDIT_TimeValue2), "0");		// 		InitEventDataValue();		;	}*/

}
CKTDGXMeshPlayer::CXMeshEvent* CX2EffectToolMeshEditor::GetDieEventDataByIndex( int iIndex_ )
{
	if( 0 > iIndex_ )
		return NULL;

	int iIndex = 0;
	BOOST_FOREACH (CKTDGXMeshPlayer::CXMeshEvent* pEventList, m_pMeshTemplet->dieEventList )
	{
		if( iIndex_ == iIndex++ )
		{
			return pEventList;
		}
	}
	return NULL;
}
void CX2EffectToolMeshEditor::UpdateEventData( CKTDGXMeshPlayer::CXMeshEvent* pEventData )
{
	if( NULL == pEventData )
		return;

	char buf[100];
	 
	int iComboBoxIndex = GetIndexByEnum(GetEventListTimeType( pEventData ));
	SendDlgItemMessage( m_hDialogEditor , PA_COMBO_TimeType, CB_SETCURSEL, (WPARAM)iComboBoxIndex , 0);
	iComboBoxIndex = GetIndexByEnum( pEventData->GetEventType() );
	SendDlgItemMessage( m_hDialogEditor , PA_COMBO_EventType, CB_SETCURSEL, (WPARAM)iComboBoxIndex, 0);

	sprintf(buf, "%0.3f", pEventData->GetActualTime().m_Min );
	SetWindowTextA(GetDlgItem( m_hDialogEditor, PA_EDIT_TimeValue1), buf);

	sprintf(buf, "%0.3f", pEventData->GetActualTime().m_Max );
	SetWindowTextA(GetDlgItem( m_hDialogEditor, PA_EDIT_TimeValue2), buf);

	InitEventDataValue(); 

	switch( pEventData->GetEventType() )
	{
	case CKTDGXMeshPlayer::ET_ANISPEED:
		{
			CKTDGXMeshPlayer::CXMeshEvent_AniSpeed* pEvent = 
				static_cast<CKTDGXMeshPlayer::CXMeshEvent_AniSpeed*>(pEventData);

			SetEventDataValue( &pEvent->GetAniSpeed() );
		} break;
	case CKTDGXMeshPlayer::ET_ANINAME:
		{
			CKTDGXMeshPlayer::CXMeshEvent_AniName* pEvent = 
				static_cast<CKTDGXMeshPlayer::CXMeshEvent_AniName*>(pEventData);

			SetEventDataValue( pEvent->GetAniName() );
			iComboBoxIndex = GetIndexByEnum( pEvent->GetPlayType() );
			SendDlgItemMessage( m_hDialogEditor , PA_COMBO_PlayType, CB_SETCURSEL, (WPARAM)iComboBoxIndex, 0);
		} break;
	case CKTDGXMeshPlayer::ET_COLOR:
		{
			CKTDGXMeshPlayer::CXMeshEvent_Color* pEvent = 
				static_cast<CKTDGXMeshPlayer::CXMeshEvent_Color*>(pEventData);

			SetEventDataValue( &pEvent->GetColor() );
		} break;
	case CKTDGXMeshPlayer::ET_OUTLINECOLOR:
		{
			CKTDGXMeshPlayer::CXMeshEvent_OutLineColor* pEvent = 
				static_cast<CKTDGXMeshPlayer::CXMeshEvent_OutLineColor*>(pEventData);

			SetEventDataValue( &pEvent->GetOutLineColor() );
		} break;
	case CKTDGXMeshPlayer::ET_SIZE:
		{
			CKTDGXMeshPlayer::CXMeshEvent_Size* pEvent = 
				static_cast<CKTDGXMeshPlayer::CXMeshEvent_Size*>(pEventData);

			SetEventDataValue( &pEvent->GetSize() );
		} break;
	case CKTDGXMeshPlayer::ET_VELOCITY:
		{
			CKTDGXMeshPlayer::CXMeshEvent_Velocity* pEvent = 
				static_cast<CKTDGXMeshPlayer::CXMeshEvent_Velocity*>(pEventData);

			SetEventDataValue( &pEvent->GetVelocity() );
		} break;
	case CKTDGXMeshPlayer::ET_TEX0UV:
		{
			CKTDGXMeshPlayer::CXMeshEvent_Tex0UV* pEvent = 
				static_cast<CKTDGXMeshPlayer::CXMeshEvent_Tex0UV*>(pEventData);

			SetEventDataValue( &pEvent->GetTex0UV() );
		} break;
	case CKTDGXMeshPlayer::ET_TEX1UV:
		{
			CKTDGXMeshPlayer::CXMeshEvent_Tex1UV* pEvent = 
				static_cast<CKTDGXMeshPlayer::CXMeshEvent_Tex1UV*>(pEventData);

			SetEventDataValue( &pEvent->GetTex1UV() );
		} break;
	case CKTDGXMeshPlayer::ET_TEX2UV:
		{
			CKTDGXMeshPlayer::CXMeshEvent_Tex2UV* pEvent = 
				static_cast<CKTDGXMeshPlayer::CXMeshEvent_Tex2UV*>(pEventData);

			SetEventDataValue( &pEvent->GetTex2UV() );
		} break;
	case CKTDGXMeshPlayer::ET_ROTATE:
		{
			CKTDGXMeshPlayer::CXMeshEvent_Rotate* pEvent = 
				static_cast<CKTDGXMeshPlayer::CXMeshEvent_Rotate*>(pEventData);

			SetEventDataValue( &pEvent->GetRotate() );
		} break;
	case CKTDGXMeshPlayer::ET_DIRSPEED:
		{
			CKTDGXMeshPlayer::CXMeshEvent_DirSpeed* pEvent = 
				static_cast<CKTDGXMeshPlayer::CXMeshEvent_DirSpeed*>(pEventData);

			SetEventDataValue( &pEvent->GetDirSpeed() );
		} break;
	case CKTDGXMeshPlayer::ET_BLACKHOLETIME:
		{
			CKTDGXMeshPlayer::CXMeshEvent_BlackHoleTime* pEvent = 
				static_cast<CKTDGXMeshPlayer::CXMeshEvent_BlackHoleTime*>(pEventData);

			SetEventDataValue( &pEvent->GetBlackHoleTime() );
		} break;
	case CKTDGXMeshPlayer::ET_CRASH:
		{
			CKTDGXMeshPlayer::CXMeshEvent_Crash* pEvent = 
				static_cast<CKTDGXMeshPlayer::CXMeshEvent_Crash*>(pEventData);

			SetEventDataValue( &pEvent->GetCrash() );
		} break;
	case CKTDGXMeshPlayer::ET_RESETCRASH:
		{
			CKTDGXMeshPlayer::CXMeshEvent_ResetCrash* pEvent = 
				static_cast<CKTDGXMeshPlayer::CXMeshEvent_ResetCrash*>(pEventData);

			SetEventDataValue( &pEvent->GetResetCrash() );
		} break;
	case CKTDGXMeshPlayer::ET_LIGHTFLOWIMPACT:
		{
			CKTDGXMeshPlayer::CXMeshEvent_LightFlowImpact* pEvent = 
				static_cast<CKTDGXMeshPlayer::CXMeshEvent_LightFlowImpact*>(pEventData);

			SetEventDataValue( &pEvent->GetLightFlowImpact() );
		} break;
	case CKTDGXMeshPlayer::ET_LIGHTFLOWPOINT:
		{
			CKTDGXMeshPlayer::CXMeshEvent_LightFlowPoint* pEvent = 
				static_cast<CKTDGXMeshPlayer::CXMeshEvent_LightFlowPoint*>(pEventData);

			SetEventDataValue( &pEvent->GetLightFlowPoint() );
		} break;
	case CKTDGXMeshPlayer::ET_SOUND:
		{
			CKTDGXMeshPlayer::CXMeshEvent_Sound* pEvent = 
				static_cast<CKTDGXMeshPlayer::CXMeshEvent_Sound*>(pEventData);

			SetEventDataValue( pEvent->GetSoundName() );
		} break;
	case CKTDGXMeshPlayer::ET_SLASHTRACE:
		{
			CKTDGXMeshPlayer::CXMeshEvent_SlashTrace* pEvent = 
				static_cast<CKTDGXMeshPlayer::CXMeshEvent_SlashTrace*>(pEventData);

			SendDlgItemMessage(m_hDialogEditor, PA_IDC_EVENT_USE_SLASH_TRACE, BM_SETCHECK, (WPARAM)pEvent->GetSlashTrace(), 0);	
		} break;
	case CKTDGXMeshPlayer::ET_RENDERTYPE:
		{
			CKTDGXMeshPlayer::CXMeshEvent_RenderType* pEvent = 
				static_cast<CKTDGXMeshPlayer::CXMeshEvent_RenderType*>(pEventData);

			SendDlgItemMessage( m_hDialogEditor , PA_COMBO_EVENT_RENDER_TYPE, CB_SETCURSEL, (WPARAM)GetIndexByEnum(pEvent->GetRenderType()), 0);
		} break;
	case CKTDGXMeshPlayer::ET_POSITION:
		{
			CKTDGXMeshPlayer::CXMeshEvent_Position* pEvent = 
				static_cast<CKTDGXMeshPlayer::CXMeshEvent_Position*>(pEventData);

			SetEventDataValue( &pEvent->GetPosition() );
		} break;
	} //switch( pEventData->GetEventType() )
}
EVENT_LIST_TIME_TYPE CX2EffectToolMeshEditor::GetEventListTimeType ( CKTDGXMeshPlayer::CXMeshEvent* pEventData )
{
	if( true == pEventData->IsFade() )
	{
		//fade so final
		if( 0.f == pEventData->GetActualTime().m_Min && 
			pEventData->GetActualTime().m_Min == pEventData->GetActualTime().m_Max )
			return ELTT_FADE_SO_FINAL;
		else // fade so from
			return ELTT_FADE_SO_FROM;
	}
	else
	{
		// Initial 
		if( 0.f == pEventData->GetActualTime().m_Min && 
			pEventData->GetActualTime().m_Min == pEventData->GetActualTime().m_Max )
			return ELTT_INITITAL;
		else //at
			return ELTT_AT;
	}

	return ELTT_NONE;
}
void CX2EffectToolMeshEditor::SaveMeshData()
{
	if( NULL == m_pMeshTemplet )
		return;

	if( NULL == m_hDialogEditor )
		return;

	// 기존 선택되어 있는 이벤트 리스트 인덱스 기록.
	int iOldSelectEventList = GetSelectEventListIndex();
	
	// 다른이름으로 저장.
	bool bIsSaveAs = SaveMeshData_OtherName();

	// UI에 설정되어 있는 값으로 데이타 변경
	SaveMeshData_Column1();
	SaveMeshData_Column2();
	SaveMeshData_EventList();

	//갱신
	UpdateMeshData( m_pMeshTemplet );

	// 이벤트 리스트 선택.
	SelectEventListByIndex( iOldSelectEventList );
	UpdateEventData( GetEventDataByIndex( iOldSelectEventList ) );

	
	if( NULL != m_pEffectTool)
	{
		// 메쉬 리스트 선택
		if( true == bIsSaveAs )
		{
			m_pEffectTool->UpdateMeshList();
			m_pEffectTool->SelectMeshListLastIndex();
		}
		// 파티클 재생.
		m_pEffectTool->PreVieweXMesh( m_pMeshTemplet->templetName.c_str() );
	}
}
void CX2EffectToolMeshEditor::SaveMeshData_Column1()
{
	if( NULL == m_pMeshTemplet )
		return;

 	m_pMeshTemplet->meshType = static_cast<CKTDGXMeshPlayer::MESH_TYPE>(GetComboValue( PA_COMBO_MeshType ));
 	m_pMeshTemplet->billBoardType = static_cast<CKTDGMatrix::BILLBOARD_TYPE>(GetComboValue(PA_COMBO_Billboard));
 	m_pMeshTemplet->renderType = static_cast<CKTDGXRenderer::RENDER_TYPE>(GetComboValue(PA_COMBO_MeshRender));
 	m_pMeshTemplet->srcBlend = static_cast<D3DBLEND>(GetComboValue(PA_COMBO_SourceBlend));
 	m_pMeshTemplet->destBlend = static_cast<D3DBLEND>(GetComboValue(PA_COMBO_DestBlend));
 	m_pMeshTemplet->cullMode = static_cast<D3DCULL>(GetComboValue(PA_COMBO_CullMode));
 	
	m_pMeshTemplet->layer = GetIntValue( PA_EDIT_Layer );
	
	m_pMeshTemplet->meshName = GetString( PA_EDIT_MeshName );
	m_pMeshTemplet->changeTexXETName = GetString( PA_EDIT_ChangeTexXetName );
	m_pMeshTemplet->multiTexXETName = GetString( PA_EDIT_MultiTexXetName );
	m_pMeshTemplet->aniXETName = GetString( PA_EDIT_AniXetName );

	m_pMeshTemplet->bZEnable = GetChecked( PA_IDC_ZEnable );
	m_pMeshTemplet->bApplyUnitScale = GetChecked( PA_IDC_ApplyUnitScale );
	m_pMeshTemplet->bForceLayer = GetChecked( PA_IDC_bForceLayer );
	m_pMeshTemplet->bZWriteEnable = GetChecked( PA_IDC_ZWriteEnable );
	m_pMeshTemplet->bSetLandHeightOnStart = GetChecked( PA_IDC_SetLandHeightOnStart );
	///////////Life Type
	m_pMeshTemplet->lifeType = static_cast<CKTDGXMeshPlayer::LIFE_TYPE>(GetComboValue(PA_COMBO_LifeType));
	m_pMeshTemplet->cartoonTexType = static_cast<CKTDGXRenderer::CARTOON_TEX_TYPE>( GetComboValue( PA_COMBO_CartoonTexType ));

	CMinMax<int> cIntTemp;
	vector<int> vecControlID;
	vecControlID.clear();
	vecControlID.push_back(PA_EDIT_PlayCount_Min);
	vecControlID.push_back(PA_EDIT_PlayCount_Max);
	SetValueByEditControl( &cIntTemp, vecControlID );
	m_pMeshTemplet->playCount = cIntTemp;

	CMinMax<float> cFloatTemp;
	vecControlID.clear();
	vecControlID.push_back( PA_EDIT_LifeTime_Min );
	vecControlID.push_back( PA_EDIT_LifeTime_Max );
	SetValueByEditControl( &cFloatTemp, vecControlID );
	m_pMeshTemplet->lifeTime = cFloatTemp;

	vecControlID.clear();
	vecControlID.push_back( PA_EDIT_DrawCount_Min );
	vecControlID.push_back( PA_EDIT_DrawCount_Max );
	SetValueByEditControl( &cIntTemp, vecControlID );
	m_pMeshTemplet->drawCount = cIntTemp;

	/////////////UseSlashTrace
	m_pMeshTemplet->bUseSlashTrace = GetChecked( PA_IDC_USE_SLASH_TRACE );
	m_pMeshTemplet->slashTraceCurveType = static_cast<CKTDGSlashTrace::CURVE_TYPE>(GetComboValue( PA_COMBO_TraceCurveType ));
	m_pMeshTemplet->iSlashTraceType = GetComboValue( PA_COMBO_SlashTraceType );
	m_pMeshTemplet->slashTraceTextureName = GetString( PA_EDIT_SlashTraceTextureName );

	///LAND
	m_pMeshTemplet->fNotCheckLandTime = GetFloatValue( PA_EDIT_NotCheckLandTime );
	m_pMeshTemplet->bCrashLand = GetChecked( PA_IDC_CrashLand );
	m_pMeshTemplet->bUseLand = GetChecked( PA_IDC_UseLand );

	vecControlID.clear();
	vecControlID.push_back( PA_EDIT_LandPosition_Min );
	vecControlID.push_back( PA_EDIT_LandPosition_Max );
	SetValueByEditControl( &cFloatTemp, vecControlID );
	m_pMeshTemplet->landPosition = cFloatTemp;
}
void CX2EffectToolMeshEditor::SaveMeshData_Column2()
{
	if( NULL == m_pMeshTemplet )
		return;

	CMinMax<float> cFloatTemp;
	vector<int> vecControlID;
	vecControlID.clear();
	vecControlID.push_back( PA_EDIT_OutLineWide_Min );
	vecControlID.push_back( PA_EDIT_OutLineWide_Max );
	SetValueByEditControl( &cFloatTemp, vecControlID );
	m_pMeshTemplet->outLineWide = cFloatTemp;

	vecControlID.clear();
	vecControlID.push_back( PA_EDIT_LightFlowWide_Min );
	vecControlID.push_back( PA_EDIT_LightFlowWide_Max);
	SetValueByEditControl( &cFloatTemp, vecControlID );
	m_pMeshTemplet->lightFlowWide = cFloatTemp;

	vecControlID.clear();
	vecControlID.push_back( PA_EDIT_elasticCoeff_Min );
	vecControlID.push_back( PA_EDIT_elasticCoeff_Max );
	SetValueByEditControl( &cFloatTemp, vecControlID );
	m_pMeshTemplet->elasticCoeff = cFloatTemp;

	vecControlID.clear();
	vecControlID.push_back( PA_EDIT_elasticCoeffX_Min );
	vecControlID.push_back( PA_EDIT_elasticCoeffX_Max );
	SetValueByEditControl( &cFloatTemp, vecControlID );
	m_pMeshTemplet->elasticCoeffX = cFloatTemp;

	CMinMax<D3DXVECTOR3> cVec3Temp;
	vecControlID.clear();
	vecControlID.push_back( PA_EDIT_Gravity_MinX );
	vecControlID.push_back( PA_EDIT_Gravity_MinY );
	vecControlID.push_back( PA_EDIT_Gravity_MinZ );
	vecControlID.push_back( PA_EDIT_Gravity_MaxX );
	vecControlID.push_back( PA_EDIT_Gravity_MaxY );
	vecControlID.push_back( PA_EDIT_Gravity_MaxZ );
	SetValueByEditControl( &cVec3Temp, vecControlID );
	m_pMeshTemplet->gravity = cVec3Temp;

	vecControlID.clear();
	vecControlID.push_back( PA_EDIT_LightPos_MinX );
	vecControlID.push_back( PA_EDIT_LightPos_MinY );
	vecControlID.push_back( PA_EDIT_LightPos_MinZ );
	vecControlID.push_back( PA_EDIT_LightPos_MaxX );
	vecControlID.push_back( PA_EDIT_LightPos_MaxY );
	vecControlID.push_back( PA_EDIT_LightPos_MaxZ );
	SetValueByEditControl( &cVec3Temp, vecControlID );
	m_pMeshTemplet->lightPos = cVec3Temp;

	vecControlID.clear();
	vecControlID.push_back( PA_EDIT_BlackHolePos_MinX );
	vecControlID.push_back( PA_EDIT_BlackHolePos_MinY );
	vecControlID.push_back( PA_EDIT_BlackHolePos_MinZ );
	vecControlID.push_back( PA_EDIT_BlackHolePos_MaxX );
	vecControlID.push_back( PA_EDIT_BlackHolePos_MaxY );
	vecControlID.push_back( PA_EDIT_BlackHolePos_MaxZ );
	SetValueByEditControl( &cVec3Temp, vecControlID );
	m_pMeshTemplet->blackHolePosition = cVec3Temp;

	/////UseDieEvent
	m_pMeshTemplet->dieType = static_cast<CKTDGXMeshPlayer::LIFE_TYPE>(GetComboValue( PA_COMBO_DieType ));
	m_pMeshTemplet->bUseDieEvent = GetChecked( PA_IDC_bUseDieEvent );

	vecControlID.clear();
	vecControlID.push_back( PA_EDIT_DieLifeTime_Min );
	vecControlID.push_back( PA_EDIT_DieLifeTime_Max );
	SetValueByEditControl( &cFloatTemp, vecControlID );
	m_pMeshTemplet->dieLifeTime = cFloatTemp;

	CMinMax<int> cIntTemp;
	vecControlID.clear();
	vecControlID.push_back( PA_EDIT_DiePlayCount_Min );
	vecControlID.push_back( PA_EDIT_DiePlayCount_Max );
	SetValueByEditControl( &cIntTemp, vecControlID );
	m_pMeshTemplet->diePlayCount = cIntTemp;
}
void CX2EffectToolMeshEditor::SaveMeshData_EventList()
{
	if( NULL == m_pMeshTemplet )
		return;

	// UI에 설정되어 있는 값 얻기.
	//////////////////////////////////////////////////////////////////////////
	/// 시간 설정.
	bool bFade = false;
	EVENT_LIST_TIME_TYPE eTimeType = static_cast<EVENT_LIST_TIME_TYPE>(GetComboValue( PA_COMBO_TimeType ));
	if( ELTT_FADE_SO_FROM == eTimeType || ELTT_FADE_SO_FINAL == eTimeType )
		bFade = true;

	CMinMax<float> cActualTime;
	vector<int> vecContorlID;
	vecContorlID.push_back( PA_EDIT_TimeValue1 );
	vecContorlID.push_back( PA_EDIT_TimeValue2 );
	SetValueByEditControl( &cActualTime, vecContorlID );


	//////////////////////////////////////////////////////////////////////////
	/// 세부 수치 설정
	CKTDGXMeshPlayer::EVENT_TYPE eEventType = static_cast<CKTDGXMeshPlayer::EVENT_TYPE>(GetComboValue( PA_COMBO_EventType ));

	// 선택된 이벤트 리스트 요소 얻기.
	int iEventIndex = GetSelectEventListIndex();
	CKTDGXMeshPlayer::CXMeshEvent* pEventData = GetEventDataByIndex(iEventIndex);
	
	if( NULL != pEventData )
	{
		if( eEventType == pEventData->GetEventType() )
			SetEventDataByUIValue( pEventData );
		else
		{//타입이 다르다면 새로 생성.
			pEventData = CreateEventList( eEventType );
			if( NULL != pEventData )
			{
				vector<CKTDGXMeshPlayer::CXMeshEvent*>* vecMeshEventList = GetSelectEventList();
				if( NULL != vecMeshEventList )
				{
					//기존 값 제거.
					vecMeshEventList->erase( vecMeshEventList->begin() + iEventIndex );

					//새로운 값 삽입
					if( static_cast<int>(vecMeshEventList->size()) == iEventIndex )
						vecMeshEventList->push_back( pEventData );
					else
						vecMeshEventList->insert( vecMeshEventList->begin()+iEventIndex, pEventData );
				}
			}
		}
	}
	else	
	{//NULL일 경우 새롭게 추가 시도하는 이벤트 리스트.
		if( -1 != iEventIndex )
		{
			//CreateEventList에서 UI에 설정된 값을 포인터로 옮김.
			pEventData = CreateEventList( eEventType );
			if( NULL != pEventData )
			{
				vector<CKTDGXMeshPlayer::CXMeshEvent*>* vecMeshEventList = GetSelectEventList();
				if( NULL != vecMeshEventList )
					vecMeshEventList->push_back( pEventData );
			}
		}
	}

	if( NULL != pEventData )
	{
		pEventData->SetFade( bFade );
		pEventData->SetActualTime( cActualTime );
	}
}
bool CX2EffectToolMeshEditor::SaveMeshData_OtherName()
{
	// 메쉬명이 저장시점과 다르다면 새로운 메쉬 생성하기
	char buf[101];
	wstring wsTemp;
	GetWindowTextA(GetDlgItem(m_hDialogEditor, PA_EDIT_MESH_TEMPLET_NAME), buf, 100);
	ConvertCharToWCHAR( wsTemp, buf );

	if( 0 != wsTemp.compare( m_pMeshTemplet->templetName.c_str() ))
	{
		CKTDGXMeshPlayer::XMeshTemplet* pTemplet = 
			m_pEffectTool->CreateXMeshTemplet( wsTemp.c_str() );

		if( NULL != pTemplet )
		{
			// 이벤트 리스트 복사
			BOOST_FOREACH( CKTDGXMeshPlayer::CXMeshEvent* pMeshEvent, m_pMeshTemplet->globalTimeEventList )
			{
				CKTDGXMeshPlayer::CXMeshEvent* pTempMeshEvent = CreateEventList( pMeshEvent );
				if( NULL != pTempMeshEvent )
				{
					pTemplet->globalTimeEventList.push_back( pTempMeshEvent );
				}
			}
			BOOST_FOREACH( CKTDGXMeshPlayer::CXMeshEvent* pMeshEvent, m_pMeshTemplet->dieEventList )
			{
				CKTDGXMeshPlayer::CXMeshEvent* pTempMeshEvent = CreateEventList( pMeshEvent );
				if( NULL != pTempMeshEvent )
				{
					pTemplet->dieEventList.push_back( pTempMeshEvent );
				}
			}
			BOOST_FOREACH( CKTDGXMeshPlayer::CXMeshEvent* pMeshEvent, m_pMeshTemplet->animTimeEventList )
			{
				CKTDGXMeshPlayer::CXMeshEvent* pTempMeshEvent = CreateEventList( pMeshEvent );
				if( NULL != pTempMeshEvent )
				{
					pTemplet->animTimeEventList.push_back( pTempMeshEvent );
				}
			}
		}
		m_pMeshTemplet = pTemplet;
		return true;
	}

	return false;
}
vector<CKTDGXMeshPlayer::CXMeshEvent*>* CX2EffectToolMeshEditor::GetSelectEventList()
{
	if( NULL == m_pMeshTemplet )
		return NULL;

	switch( m_eSelectEventListType )
	{
		case ELT_GLOBAL:
			return &m_pMeshTemplet->globalTimeEventList;
		case ELT_ANIM:
			return &m_pMeshTemplet->animTimeEventList;
		case ELT_DIE:
			return &m_pMeshTemplet->dieEventList;
	}

	return NULL;
}
void CX2EffectToolMeshEditor::OnFrameMove_HotKey()
{
	if( GetAsyncKeyState( VK_F2 ) & 0x8000 )
	{
		SaveMeshData();
	}
	else if( GetAsyncKeyState( VK_ESCAPE ) & 0x8000 )
	{
		CloseDialog();
	}
}
CKTDGXMeshPlayer::CXMeshEvent* CX2EffectToolMeshEditor::GetEventDataByIndex( int iIndex_ , EVENT_LIST_TYPE eListType_ /*= ELT_NONE*/ )
{
	if( 0 > iIndex_ )
		return NULL;

	vector<CKTDGXMeshPlayer::CXMeshEvent*>  vecMeshEventList;
	switch ( eListType_ )
	{
	case ELT_ANIM:
		vecMeshEventList = m_pMeshTemplet->animTimeEventList;
	case ELT_DIE:
		vecMeshEventList = m_pMeshTemplet->dieEventList;
	case ELT_GLOBAL:
		vecMeshEventList = m_pMeshTemplet->globalTimeEventList;
	default:
		vecMeshEventList = *GetSelectEventList();
		break;
	}

	if( static_cast<int>(vecMeshEventList.size()) <= iIndex_ )
		return NULL;

	return vecMeshEventList[iIndex_];
}
void CX2EffectToolMeshEditor::SetEventDataByUIValue( OUT CKTDGXMeshPlayer::CXMeshEvent* pEventData_ )
{
	if( NULL == pEventData_ )
		return;


	// 모든 값들을 미리 읽어드린 다음, 
	// 각 타입에 맞게 필요한 값만 저장하도록 작업함.
	char buf[100];
	// D3DXCOLOR
	CMinMax<D3DXCOLOR> cColor;
	GetWindowTextA(GetDlgItem(m_hDialogEditor, PA_COMBO_COLOR_MinR ), buf, 100);
	cColor.m_Min.r = static_cast<float>(atof(buf));
	GetWindowTextA(GetDlgItem(m_hDialogEditor, PA_COMBO_COLOR_MinG ), buf, 100);
	cColor.m_Min.g = static_cast<float>(atof(buf));
	GetWindowTextA(GetDlgItem(m_hDialogEditor, PA_COMBO_COLOR_MinB ), buf, 100);
	cColor.m_Min.b = static_cast<float>(atof(buf));
	GetWindowTextA(GetDlgItem(m_hDialogEditor, PA_COMBO_COLOR_MinA ), buf, 100);
	cColor.m_Min.a = static_cast<float>(atof(buf));
	GetWindowTextA(GetDlgItem(m_hDialogEditor, PA_COMBO_COLOR_MaxR ), buf, 100);
	cColor.m_Max.r = max( cColor.m_Min.r , static_cast<float>(atof(buf)) );
	GetWindowTextA(GetDlgItem(m_hDialogEditor, PA_COMBO_COLOR_MaxG ), buf, 100);
	cColor.m_Max.g = max( cColor.m_Min.g , static_cast<float>(atof(buf)) );
	GetWindowTextA(GetDlgItem(m_hDialogEditor, PA_COMBO_COLOR_MaxB ), buf, 100);
	cColor.m_Max.b = max( cColor.m_Min.b , static_cast<float>(atof(buf)) );
	GetWindowTextA(GetDlgItem(m_hDialogEditor, PA_COMBO_COLOR_MaxA ), buf, 100);
	cColor.m_Max.a = max( cColor.m_Min.a , static_cast<float>(atof(buf)) );

	// D3DXVECTOR3
	CMinMax<D3DXVECTOR3> cVec3;	
	GetWindowTextA(GetDlgItem(m_hDialogEditor, PA_COMBO_VECTOR3_MinX ), buf, 100);
	cVec3.m_Min.x = static_cast<float>(atof(buf));
	GetWindowTextA(GetDlgItem(m_hDialogEditor, PA_COMBO_VECTOR3_MinY ), buf, 100);
	cVec3.m_Min.y = static_cast<float>(atof(buf));
	GetWindowTextA(GetDlgItem(m_hDialogEditor, PA_COMBO_VECTOR3_MinZ ), buf, 100);
	cVec3.m_Min.z = static_cast<float>(atof(buf));
	GetWindowTextA(GetDlgItem(m_hDialogEditor, PA_COMBO_VECTOR3_MaxX ), buf, 100);
	cVec3.m_Max.x = max( cVec3.m_Min.x, static_cast<float>(atof(buf)) );
	GetWindowTextA(GetDlgItem(m_hDialogEditor, PA_COMBO_VECTOR3_MaxY ), buf, 100);
	cVec3.m_Max.y = max( cVec3.m_Min.y, static_cast<float>(atof(buf)) );
	GetWindowTextA(GetDlgItem(m_hDialogEditor, PA_COMBO_VECTOR3_MaxZ ), buf, 100);
	cVec3.m_Max.z = max( cVec3.m_Min.z, static_cast<float>(atof(buf)) );

	// D3DXVECTOR2
	CMinMax<D3DXVECTOR2> cVec2;	
	GetWindowTextA(GetDlgItem(m_hDialogEditor, PA_COMBO_VECTOR2_MinX ), buf, 100);
	cVec2.m_Min.x = static_cast<float>(atof(buf));
	GetWindowTextA(GetDlgItem(m_hDialogEditor, PA_COMBO_VECTOR2_MinY ), buf, 100);
	cVec2.m_Min.y = static_cast<float>(atof(buf));
	GetWindowTextA(GetDlgItem(m_hDialogEditor, PA_COMBO_VECTOR2_MaxX ), buf, 100);
	cVec2.m_Max.x = max( cVec2.m_Min.x, static_cast<float>(atof(buf)) );
	GetWindowTextA(GetDlgItem(m_hDialogEditor, PA_COMBO_VECTOR2_MaxY ), buf, 100);
	cVec2.m_Max.y = max( cVec2.m_Min.y, static_cast<float>(atof(buf)) );

	// float
	CMinMax<float> cFloat;
	GetWindowTextA(GetDlgItem(m_hDialogEditor, PA_COMBO_FLOAT_Min ), buf, 100);
	cFloat.m_Min = static_cast<float>(atof(buf));
	GetWindowTextA(GetDlgItem(m_hDialogEditor, PA_COMBO_FLOAT_Max ), buf, 100);
	cFloat.m_Max = max( cFloat.m_Min, static_cast<float>(atof(buf)) );

	// string
	wstring wsValue;
	GetWindowTextA(GetDlgItem(m_hDialogEditor, PA_EDIT_STRING ), buf, 100);
	ConvertCharToWCHAR( wsValue, buf);

	// Render Type
	CKTDGXRenderer::RENDER_TYPE eRenderType = static_cast<CKTDGXRenderer::RENDER_TYPE>(GetComboValue( PA_COMBO_EVENT_RENDER_TYPE ));

	// Play Type
	CKTDGXSkinAnim::XSKIN_ANIM_PLAYTYPE ePlayType = static_cast<CKTDGXSkinAnim::XSKIN_ANIM_PLAYTYPE>(GetComboValue( PA_COMBO_PlayType ));

	// USE_SLASH_TRACE
	bool bUseSlashTrace = GetChecked( PA_IDC_EVENT_USE_SLASH_TRACE );

	switch( pEventData_->GetEventType() )
	{
	case CKTDGXMeshPlayer::ET_ANISPEED:
		{
			CKTDGXMeshPlayer::CXMeshEvent_AniSpeed* pEvent = 
				static_cast<CKTDGXMeshPlayer::CXMeshEvent_AniSpeed*>(pEventData_);

			pEvent->SetAniSpeed( cFloat );
		} break;
	case CKTDGXMeshPlayer::ET_ANINAME:
		{
			CKTDGXMeshPlayer::CXMeshEvent_AniName* pEvent = 
				static_cast<CKTDGXMeshPlayer::CXMeshEvent_AniName*>(pEventData_);

			pEvent->SetAniName( wsValue.c_str() );
			pEvent->SetPlayType( ePlayType );
		} break;
	case CKTDGXMeshPlayer::ET_COLOR:
		{
			CKTDGXMeshPlayer::CXMeshEvent_Color* pEvent = 
				static_cast<CKTDGXMeshPlayer::CXMeshEvent_Color*>(pEventData_);
			
			pEvent->SetColor( cColor );
		} break;
	case CKTDGXMeshPlayer::ET_OUTLINECOLOR:
		{
			CKTDGXMeshPlayer::CXMeshEvent_OutLineColor* pEvent = 
				static_cast<CKTDGXMeshPlayer::CXMeshEvent_OutLineColor*>(pEventData_);

			pEvent->SetOutLineColor(cColor);
		} break;
	case CKTDGXMeshPlayer::ET_SIZE:
		{
			CKTDGXMeshPlayer::CXMeshEvent_Size* pEvent = 
				static_cast<CKTDGXMeshPlayer::CXMeshEvent_Size*>(pEventData_);

			pEvent->SetSize( cVec3 );
		} break;
	case CKTDGXMeshPlayer::ET_VELOCITY:
		{
			CKTDGXMeshPlayer::CXMeshEvent_Velocity* pEvent = 
				static_cast<CKTDGXMeshPlayer::CXMeshEvent_Velocity*>(pEventData_);

			pEvent->SetVelocity( cVec3 );
		} break;
	case CKTDGXMeshPlayer::ET_TEX0UV:
		{
			CKTDGXMeshPlayer::CXMeshEvent_Tex0UV* pEvent = 
				static_cast<CKTDGXMeshPlayer::CXMeshEvent_Tex0UV*>(pEventData_);

			pEvent->SetTex0UV( cVec2 );
		} break;
	case CKTDGXMeshPlayer::ET_TEX1UV:
		{
			CKTDGXMeshPlayer::CXMeshEvent_Tex1UV* pEvent = 
				static_cast<CKTDGXMeshPlayer::CXMeshEvent_Tex1UV*>(pEventData_);

			pEvent->SetTex1UV( cVec2 );
		} break;
	case CKTDGXMeshPlayer::ET_TEX2UV:
		{
			CKTDGXMeshPlayer::CXMeshEvent_Tex2UV* pEvent = 
				static_cast<CKTDGXMeshPlayer::CXMeshEvent_Tex2UV*>(pEventData_);

			pEvent->SetTex2UV( cVec2 );
		} break;
	case CKTDGXMeshPlayer::ET_ROTATE:
		{
			CKTDGXMeshPlayer::CXMeshEvent_Rotate* pEvent = 
				static_cast<CKTDGXMeshPlayer::CXMeshEvent_Rotate*>(pEventData_);

			pEvent->SetRotate( cVec3 );
		} break;
	case CKTDGXMeshPlayer::ET_DIRSPEED:
		{
			CKTDGXMeshPlayer::CXMeshEvent_DirSpeed* pEvent = 
				static_cast<CKTDGXMeshPlayer::CXMeshEvent_DirSpeed*>(pEventData_);

			pEvent->SetDirSpeed( cFloat );
		} break;
	case CKTDGXMeshPlayer::ET_BLACKHOLETIME:
		{
			CKTDGXMeshPlayer::CXMeshEvent_BlackHoleTime* pEvent = 
				static_cast<CKTDGXMeshPlayer::CXMeshEvent_BlackHoleTime*>(pEventData_);

			pEvent->SetBlackHoleTime( cFloat );
		} break;
	case CKTDGXMeshPlayer::ET_CRASH:
		{
			CKTDGXMeshPlayer::CXMeshEvent_Crash* pEvent = 
				static_cast<CKTDGXMeshPlayer::CXMeshEvent_Crash*>(pEventData_);

			pEvent->SetCrash( cVec3 );
		} break;
	case CKTDGXMeshPlayer::ET_RESETCRASH:
		{
			CKTDGXMeshPlayer::CXMeshEvent_ResetCrash* pEvent = 
				static_cast<CKTDGXMeshPlayer::CXMeshEvent_ResetCrash*>(pEventData_);

			pEvent->SetResetCrash( cFloat );
		} break;
	case CKTDGXMeshPlayer::ET_LIGHTFLOWIMPACT:
		{
			CKTDGXMeshPlayer::CXMeshEvent_LightFlowImpact* pEvent = 
				static_cast<CKTDGXMeshPlayer::CXMeshEvent_LightFlowImpact*>(pEventData_);

			pEvent->SetLightFlowImpact( cFloat );
		} break;
	case CKTDGXMeshPlayer::ET_LIGHTFLOWPOINT:
		{
			CKTDGXMeshPlayer::CXMeshEvent_LightFlowPoint* pEvent = 
				static_cast<CKTDGXMeshPlayer::CXMeshEvent_LightFlowPoint*>(pEventData_);

			pEvent->SetLightFlowPoint( cVec3 );
		} break;
	case CKTDGXMeshPlayer::ET_SOUND:
		{
			CKTDGXMeshPlayer::CXMeshEvent_Sound* pEvent = 
				static_cast<CKTDGXMeshPlayer::CXMeshEvent_Sound*>(pEventData_);

			pEvent->SetSoundName( wsValue.c_str() );
		} break;
	case CKTDGXMeshPlayer::ET_SLASHTRACE:
		{
			CKTDGXMeshPlayer::CXMeshEvent_SlashTrace* pEvent = 
				static_cast<CKTDGXMeshPlayer::CXMeshEvent_SlashTrace*>(pEventData_);

			pEvent->SetSlashTrace( bUseSlashTrace );
		} break;
	case CKTDGXMeshPlayer::ET_RENDERTYPE:
		{
			CKTDGXMeshPlayer::CXMeshEvent_RenderType* pEvent = 
				static_cast<CKTDGXMeshPlayer::CXMeshEvent_RenderType*>(pEventData_);

			pEvent->SetRenderType( eRenderType );
		} break;
	case CKTDGXMeshPlayer::ET_POSITION:
		{
			CKTDGXMeshPlayer::CXMeshEvent_Position* pEvent = 
				static_cast<CKTDGXMeshPlayer::CXMeshEvent_Position*>(pEventData_);

			pEvent->SetPosition( cVec3 );
		} break;
	} //switch( pEventData_->GetEventType() )
}
int CX2EffectToolMeshEditor::GetSelectEventListIndex()
{
	switch( m_eSelectEventListType )
	{
	case ELT_GLOBAL:
		return SendDlgItemMessage( m_hDialogEditor, PA_LIST_EventList_Global, LB_GETCURSEL, 0, 0);
	case ELT_ANIM:
		return SendDlgItemMessage( m_hDialogEditor, PA_LIST_EventList_Anim, LB_GETCURSEL, 0, 0);
	case ELT_DIE:
		return SendDlgItemMessage( m_hDialogEditor, PA_LIST_EventList_Die, LB_GETCURSEL, 0, 0);
	}

	return -1;
}

void CX2EffectToolMeshEditor::SelectEventListByIndex( int iIndex_ )
{
	switch( m_eSelectEventListType )
	{
	case ELT_GLOBAL:
		SendDlgItemMessage( m_hDialogEditor, PA_LIST_EventList_Global, LB_SETCURSEL, iIndex_, 0);
		break;
	case ELT_ANIM:
		SendDlgItemMessage( m_hDialogEditor, PA_LIST_EventList_Anim, LB_SETCURSEL, iIndex_, 0);
		break;
	case ELT_DIE:
		SendDlgItemMessage( m_hDialogEditor, PA_LIST_EventList_Die, LB_SETCURSEL, iIndex_, 0);
		break;
	}
}
void CX2EffectToolMeshEditor::ON_PA_IDC_EvnetList_Global_ADD()
{
	//더미 이벤트 추가
	SendDlgItemMessage( m_hDialogEditor , PA_LIST_EventList_Global, LB_ADDSTRING, 0, (LPARAM)L"추가" );
	SetWindowTextA(GetDlgItem( m_hDialogEditor, PA_COMBO_EventType ), "");
	SelectEventListByIndex( m_pMeshTemplet->globalTimeEventList.size() );
	InitEventDataValue();
}
void CX2EffectToolMeshEditor::ON_PA_IDC_EvnetList_Global_DEL()// 이벤트 리스트 제거
{
	// 선택된 이벤트 리스트 요소 얻기.
	int iEventIndex = SendDlgItemMessage( m_hDialogEditor, PA_LIST_EventList_Global, LB_GETCURSEL, 0, 0);
	CKTDGXMeshPlayer::CXMeshEvent* pEventData = GetEventDataByIndex(iEventIndex, ELT_GLOBAL);
	if( NULL == pEventData )
		return;
	
	// 제거	
	m_pMeshTemplet->globalTimeEventList.erase( m_pMeshTemplet->globalTimeEventList.begin() + iEventIndex );

	// 갱신
	UpdateMeshData( m_pMeshTemplet );

	// 이벤트 리스트 선택.
	iEventIndex = min( static_cast<int>(m_pMeshTemplet->globalTimeEventList.size()) - 1, iEventIndex );
	SelectEventListByIndex( iEventIndex );
	UpdateEventData( GetEventDataByIndex( iEventIndex ) );
}
void CX2EffectToolMeshEditor::ON_PA_IDC_EvnetList_Anim_ADD()
{
	//더미 이벤트 추가
	SendDlgItemMessage( m_hDialogEditor , PA_LIST_EventList_Anim, LB_ADDSTRING, 0, (LPARAM)L"추가" );
	SetWindowTextA(GetDlgItem( m_hDialogEditor, PA_COMBO_EventType ), "");
	SelectEventListByIndex( m_pMeshTemplet->animTimeEventList.size() );
	InitEventDataValue();
}
void CX2EffectToolMeshEditor::ON_PA_IDC_EvnetList_Anim_DEL()
{
	// 선택된 이벤트 리스트 요소 얻기.
	int iEventIndex = SendDlgItemMessage( m_hDialogEditor, PA_LIST_EventList_Anim, LB_GETCURSEL, 0, 0);
	CKTDGXMeshPlayer::CXMeshEvent* pEventData = GetEventDataByIndex(iEventIndex, ELT_ANIM );
	if( NULL == pEventData )
		return;

	// 제거	
	m_pMeshTemplet->animTimeEventList.erase( m_pMeshTemplet->animTimeEventList.begin() + iEventIndex );

	// 갱신
	UpdateMeshData( m_pMeshTemplet );

	// 이벤트 리스트 선택.
	iEventIndex = min(  static_cast<int>(m_pMeshTemplet->animTimeEventList.size()) - 1, iEventIndex );
	SelectEventListByIndex( iEventIndex );
	UpdateEventData( GetEventDataByIndex( iEventIndex ) );
}
void CX2EffectToolMeshEditor::ON_PA_IDC_EvnetList_Die_ADD()
{
	//더미 이벤트 추가
	SendDlgItemMessage( m_hDialogEditor , PA_LIST_EventList_Die, LB_ADDSTRING, 0, (LPARAM)L"추가" );
	SetWindowTextA(GetDlgItem( m_hDialogEditor, PA_COMBO_EventType ), "");
	SelectEventListByIndex( m_pMeshTemplet->dieEventList.size() );
	InitEventDataValue();
}
void CX2EffectToolMeshEditor::ON_PA_IDC_EvnetList_Die_DEL()
{
	// 선택된 이벤트 리스트 요소 얻기.
	int iEventIndex = SendDlgItemMessage( m_hDialogEditor, PA_LIST_EventList_Die, LB_GETCURSEL, 0, 0);
	CKTDGXMeshPlayer::CXMeshEvent* pEventData = GetEventDataByIndex(iEventIndex, ELT_DIE );
	if( NULL == pEventData )
		return;

	// 제거	
	m_pMeshTemplet->dieEventList.erase( m_pMeshTemplet->dieEventList.begin() + iEventIndex );

	// 갱신
	UpdateMeshData( m_pMeshTemplet );

	// 이벤트 리스트 선택.
	iEventIndex = min(  static_cast<int>(m_pMeshTemplet->dieEventList.size()) - 1, iEventIndex );
	SelectEventListByIndex( iEventIndex );
	UpdateEventData( GetEventDataByIndex( iEventIndex ) );
}
void CX2EffectToolMeshEditor::ON_PA_IDC_EvnetList_Global_UP()
{
	if( NULL == m_pMeshTemplet )
		return;

	Move_EventList_Up( ELT_GLOBAL, m_pMeshTemplet->globalTimeEventList );
}
void CX2EffectToolMeshEditor::ON_PA_IDC_EvnetList_Global_DOWN()
{
	if( NULL == m_pMeshTemplet )
		return;

	Move_EventList_Down( ELT_GLOBAL, m_pMeshTemplet->globalTimeEventList );
}
void CX2EffectToolMeshEditor::ON_PA_IDC_EvnetList_Anim_UP()
{
	if( NULL == m_pMeshTemplet )
		return;

	Move_EventList_Up( ELT_ANIM, m_pMeshTemplet->animTimeEventList );
}
void CX2EffectToolMeshEditor::ON_PA_IDC_EvnetList_Anim_DOWN()
{
	if( NULL == m_pMeshTemplet )
		return;

	Move_EventList_Down( ELT_ANIM, m_pMeshTemplet->animTimeEventList);
}
void CX2EffectToolMeshEditor::ON_PA_IDC_EvnetList_Die_UP()
{
	if( NULL == m_pMeshTemplet )
		return;

	Move_EventList_Up( ELT_DIE, m_pMeshTemplet->dieEventList );
}
void CX2EffectToolMeshEditor::ON_PA_IDC_EvnetList_Die_DOWN()
{
	if( NULL == m_pMeshTemplet )
		return;

	Move_EventList_Down( ELT_DIE, m_pMeshTemplet->dieEventList );
}
void CX2EffectToolMeshEditor::Move_EventList_Up( EVENT_LIST_TYPE eEventListType_, vector<CKTDGXMeshPlayer::CXMeshEvent*>& vecEventList_ )
{
	// 선택된 이벤트리스트 인덱스.
	m_eSelectEventListType = eEventListType_;
	UINT uiEventListIndex = GetSelectEventListIndex();

	// 선택된 이벤트가 최상위라면 더이상 UP 불가.
	if( 0 == uiEventListIndex  )
		return;

	CKTDGXMeshPlayer::CXMeshEvent* pEvent = GetEventDataByIndex( uiEventListIndex, eEventListType_);
	if( NULL == pEvent )
		return;

	// 복사본 생성.
	CKTDGXMeshPlayer::CXMeshEvent* pTempEvent = CreateEventList( pEvent->GetEventType() );
	if( NULL != pTempEvent )
	{
		pTempEvent = pEvent;

		// 복사본을 한칸 위에 추가.
		vecEventList_.insert( vecEventList_.begin() + uiEventListIndex - 1, pTempEvent );

		// 원본 제거
		vecEventList_.erase( vecEventList_.begin() + uiEventListIndex + 1 );
	}

	// 갱신
	UpdateEventList();
	SelectEventListByIndex( uiEventListIndex - 1 );
}
void CX2EffectToolMeshEditor::Move_EventList_Down( EVENT_LIST_TYPE eEventListType_, vector<CKTDGXMeshPlayer::CXMeshEvent*>& vecEventList_ )
{
	// 선택된 이벤트리스트 인덱스.
	m_eSelectEventListType = eEventListType_;
	UINT uiEventListIndex = GetSelectEventListIndex();

	// 선택된 이벤트가 최하위라면 더이상 DOWN 불가.
	if( vecEventList_.size() == (uiEventListIndex + 1) )
		return;

	CKTDGXMeshPlayer::CXMeshEvent* pEvent = GetEventDataByIndex( uiEventListIndex, eEventListType_);
	if( NULL == pEvent )
		return;

	// 복사본 생성.
	CKTDGXMeshPlayer::CXMeshEvent* pTempEvent = CreateEventList( pEvent->GetEventType() );
	if( NULL != pTempEvent )
	{
		pTempEvent = pEvent;

		// 복사본을 한칸 아래에 추가.
		vecEventList_.insert( vecEventList_.begin() + uiEventListIndex + 2, pTempEvent );

		// 원본 제거
		vecEventList_.erase( vecEventList_.begin() + uiEventListIndex );
	}

	// 갱신
	UpdateEventList();
	SelectEventListByIndex( uiEventListIndex + 1 );
}
CKTDGXMeshPlayer::CXMeshEvent* CX2EffectToolMeshEditor::CreateEventList( CKTDGXMeshPlayer::CXMeshEvent* pEvent_ )
{
	CKTDGXMeshPlayer::CXMeshEvent* pEvent = NULL;

	switch ( pEvent_->GetEventType() ) 
	{
	case CKTDGXMeshPlayer::ET_ANISPEED:			
		{
			pEvent = new CKTDGXMeshPlayer::CXMeshEvent_AniSpeed;  
			CKTDGXMeshPlayer::CXMeshEvent_AniSpeed* pDestEvent = static_cast<CKTDGXMeshPlayer::CXMeshEvent_AniSpeed*>(pEvent);
			CKTDGXMeshPlayer::CXMeshEvent_AniSpeed* pSrcEvent = static_cast<CKTDGXMeshPlayer::CXMeshEvent_AniSpeed*>(pEvent_);
			pDestEvent->SetAniSpeed( pSrcEvent->GetAniSpeed() );
		}break;
	case CKTDGXMeshPlayer::ET_ANINAME:			
		{
			pEvent = new CKTDGXMeshPlayer::CXMeshEvent_AniName; 
			CKTDGXMeshPlayer::CXMeshEvent_AniName* pDestEvent = static_cast<CKTDGXMeshPlayer::CXMeshEvent_AniName*>(pEvent);
			CKTDGXMeshPlayer::CXMeshEvent_AniName* pSrcEvent = static_cast<CKTDGXMeshPlayer::CXMeshEvent_AniName*>(pEvent_);
			pDestEvent->SetAniName( pSrcEvent->GetAniName() ); 
			pDestEvent->SetPlayType( pSrcEvent->GetPlayType() );
		}break;
	case CKTDGXMeshPlayer::ET_COLOR:			
		{
			pEvent = new CKTDGXMeshPlayer::CXMeshEvent_Color;   
			CKTDGXMeshPlayer::CXMeshEvent_Color* pDestEvent = static_cast<CKTDGXMeshPlayer::CXMeshEvent_Color*>(pEvent);
			CKTDGXMeshPlayer::CXMeshEvent_Color* pSrcEvent = static_cast<CKTDGXMeshPlayer::CXMeshEvent_Color*>(pEvent_);
			pDestEvent->SetColor( pSrcEvent->GetColor() ); 
		}break;
	case CKTDGXMeshPlayer::ET_OUTLINECOLOR:		
		{
			pEvent = new CKTDGXMeshPlayer::CXMeshEvent_OutLineColor;   
			CKTDGXMeshPlayer::CXMeshEvent_OutLineColor* pDestEvent = static_cast<CKTDGXMeshPlayer::CXMeshEvent_OutLineColor*>(pEvent);
			CKTDGXMeshPlayer::CXMeshEvent_OutLineColor* pSrcEvent = static_cast<CKTDGXMeshPlayer::CXMeshEvent_OutLineColor*>(pEvent_);
			pDestEvent->SetOutLineColor( pSrcEvent->GetOutLineColor() );  
		}break;
	case CKTDGXMeshPlayer::ET_SIZE:				
		{
			pEvent = new CKTDGXMeshPlayer::CXMeshEvent_Size;  
			CKTDGXMeshPlayer::CXMeshEvent_Size* pDestEvent = static_cast<CKTDGXMeshPlayer::CXMeshEvent_Size*>(pEvent);
			CKTDGXMeshPlayer::CXMeshEvent_Size* pSrcEvent = static_cast<CKTDGXMeshPlayer::CXMeshEvent_Size*>(pEvent_);
			pDestEvent->SetSize( pSrcEvent->GetSize() );  
		}break;
	case CKTDGXMeshPlayer::ET_VELOCITY:			
		{
			pEvent = new CKTDGXMeshPlayer::CXMeshEvent_Velocity;  
			CKTDGXMeshPlayer::CXMeshEvent_Velocity* pDestEvent = static_cast<CKTDGXMeshPlayer::CXMeshEvent_Velocity*>(pEvent);
			CKTDGXMeshPlayer::CXMeshEvent_Velocity* pSrcEvent = static_cast<CKTDGXMeshPlayer::CXMeshEvent_Velocity*>(pEvent_);
			pDestEvent->SetVelocity( pSrcEvent->GetVelocity() );  
		}break;
	case CKTDGXMeshPlayer::ET_TEX0UV:			
		{
			pEvent = new CKTDGXMeshPlayer::CXMeshEvent_Tex0UV;  
			CKTDGXMeshPlayer::CXMeshEvent_Tex0UV* pDestEvent = static_cast<CKTDGXMeshPlayer::CXMeshEvent_Tex0UV*>(pEvent);
			CKTDGXMeshPlayer::CXMeshEvent_Tex0UV* pSrcEvent = static_cast<CKTDGXMeshPlayer::CXMeshEvent_Tex0UV*>(pEvent_);
			pDestEvent->SetTex0UV( pSrcEvent->GetTex0UV() );  
		}break;
	case CKTDGXMeshPlayer::ET_TEX1UV:			
		{
			pEvent = new CKTDGXMeshPlayer::CXMeshEvent_Tex1UV;  
			CKTDGXMeshPlayer::CXMeshEvent_Tex1UV* pDestEvent = static_cast<CKTDGXMeshPlayer::CXMeshEvent_Tex1UV*>(pEvent);
			CKTDGXMeshPlayer::CXMeshEvent_Tex1UV* pSrcEvent = static_cast<CKTDGXMeshPlayer::CXMeshEvent_Tex1UV*>(pEvent_);
			pDestEvent->SetTex1UV( pSrcEvent->GetTex1UV() );  
		}break;
	case CKTDGXMeshPlayer::ET_TEX2UV:			
		{
			pEvent = new CKTDGXMeshPlayer::CXMeshEvent_Tex2UV;  
			CKTDGXMeshPlayer::CXMeshEvent_Tex2UV* pDestEvent = static_cast<CKTDGXMeshPlayer::CXMeshEvent_Tex2UV*>(pEvent);
			CKTDGXMeshPlayer::CXMeshEvent_Tex2UV* pSrcEvent = static_cast<CKTDGXMeshPlayer::CXMeshEvent_Tex2UV*>(pEvent_);
			pDestEvent->SetTex2UV( pSrcEvent->GetTex2UV() );  
		}break;
	case CKTDGXMeshPlayer::ET_ROTATE:			
		{
			pEvent = new CKTDGXMeshPlayer::CXMeshEvent_Rotate;    
			CKTDGXMeshPlayer::CXMeshEvent_Rotate* pDestEvent = static_cast<CKTDGXMeshPlayer::CXMeshEvent_Rotate*>(pEvent);
			CKTDGXMeshPlayer::CXMeshEvent_Rotate* pSrcEvent = static_cast<CKTDGXMeshPlayer::CXMeshEvent_Rotate*>(pEvent_);
			pDestEvent->SetRotate( pSrcEvent->GetRotate() );  
		}break;
	case CKTDGXMeshPlayer::ET_DIRSPEED:			
		{
			pEvent = new CKTDGXMeshPlayer::CXMeshEvent_DirSpeed;      
			CKTDGXMeshPlayer::CXMeshEvent_DirSpeed* pDestEvent = static_cast<CKTDGXMeshPlayer::CXMeshEvent_DirSpeed*>(pEvent);
			CKTDGXMeshPlayer::CXMeshEvent_DirSpeed* pSrcEvent = static_cast<CKTDGXMeshPlayer::CXMeshEvent_DirSpeed*>(pEvent_);
			pDestEvent->SetDirSpeed( pSrcEvent->GetDirSpeed() );  
		}break;
	case CKTDGXMeshPlayer::ET_BLACKHOLETIME:	
		{
			pEvent = new CKTDGXMeshPlayer::CXMeshEvent_BlackHoleTime;      
			CKTDGXMeshPlayer::CXMeshEvent_BlackHoleTime* pDestEvent = static_cast<CKTDGXMeshPlayer::CXMeshEvent_BlackHoleTime*>(pEvent);
			CKTDGXMeshPlayer::CXMeshEvent_BlackHoleTime* pSrcEvent = static_cast<CKTDGXMeshPlayer::CXMeshEvent_BlackHoleTime*>(pEvent_);
			pDestEvent->SetBlackHoleTime( pSrcEvent->GetBlackHoleTime() );
		}break;
	case CKTDGXMeshPlayer::ET_CRASH:			
		{
			pEvent = new CKTDGXMeshPlayer::CXMeshEvent_Crash; 
			CKTDGXMeshPlayer::CXMeshEvent_Crash* pDestEvent = static_cast<CKTDGXMeshPlayer::CXMeshEvent_Crash*>(pEvent);
			CKTDGXMeshPlayer::CXMeshEvent_Crash* pSrcEvent = static_cast<CKTDGXMeshPlayer::CXMeshEvent_Crash*>(pEvent_);
			pDestEvent->SetCrash( pSrcEvent->GetCrash() ); 
		}break;
	case CKTDGXMeshPlayer::ET_RESETCRASH:		
		{
			pEvent = new CKTDGXMeshPlayer::CXMeshEvent_ResetCrash;  
			CKTDGXMeshPlayer::CXMeshEvent_ResetCrash* pDestEvent = static_cast<CKTDGXMeshPlayer::CXMeshEvent_ResetCrash*>(pEvent);
			CKTDGXMeshPlayer::CXMeshEvent_ResetCrash* pSrcEvent = static_cast<CKTDGXMeshPlayer::CXMeshEvent_ResetCrash*>(pEvent_);
			pDestEvent->SetResetCrash( pSrcEvent->GetResetCrash() ); 
		}break;
	case CKTDGXMeshPlayer::ET_LIGHTFLOWIMPACT:	
		{
			pEvent = new CKTDGXMeshPlayer::CXMeshEvent_LightFlowImpact;  
			CKTDGXMeshPlayer::CXMeshEvent_LightFlowImpact* pDestEvent = static_cast<CKTDGXMeshPlayer::CXMeshEvent_LightFlowImpact*>(pEvent);
			CKTDGXMeshPlayer::CXMeshEvent_LightFlowImpact* pSrcEvent = static_cast<CKTDGXMeshPlayer::CXMeshEvent_LightFlowImpact*>(pEvent_);
			pDestEvent->SetLightFlowImpact( pSrcEvent->GetLightFlowImpact() ); 
		}break;
	case CKTDGXMeshPlayer::ET_LIGHTFLOWPOINT:	
		{
			pEvent = new CKTDGXMeshPlayer::CXMeshEvent_LightFlowPoint;  
			CKTDGXMeshPlayer::CXMeshEvent_LightFlowPoint* pDestEvent = static_cast<CKTDGXMeshPlayer::CXMeshEvent_LightFlowPoint*>(pEvent);
			CKTDGXMeshPlayer::CXMeshEvent_LightFlowPoint* pSrcEvent = static_cast<CKTDGXMeshPlayer::CXMeshEvent_LightFlowPoint*>(pEvent_);
			pDestEvent->SetLightFlowPoint( pSrcEvent->GetLightFlowPoint() ); 
		}break;
	case CKTDGXMeshPlayer::ET_SOUND:			
		{
			pEvent = new CKTDGXMeshPlayer::CXMeshEvent_Sound;  
			CKTDGXMeshPlayer::CXMeshEvent_Sound* pDestEvent = static_cast<CKTDGXMeshPlayer::CXMeshEvent_Sound*>(pEvent);
			CKTDGXMeshPlayer::CXMeshEvent_Sound* pSrcEvent = static_cast<CKTDGXMeshPlayer::CXMeshEvent_Sound*>(pEvent_);
			pDestEvent->SetSoundName( pSrcEvent->GetSoundName() ); 
		}break;
	case CKTDGXMeshPlayer::ET_GLOBALTIMER:		
		{
			pEvent = new CKTDGXMeshPlayer::CXMeshEvent_RenderType;  
			CKTDGXMeshPlayer::CXMeshEvent_RenderType* pDestEvent = static_cast<CKTDGXMeshPlayer::CXMeshEvent_RenderType*>(pEvent);
			CKTDGXMeshPlayer::CXMeshEvent_RenderType* pSrcEvent = static_cast<CKTDGXMeshPlayer::CXMeshEvent_RenderType*>(pEvent_);
			pDestEvent->SetRenderType( pSrcEvent->GetRenderType() ); 
		}break;
	case CKTDGXMeshPlayer::ET_SLASHTRACE:		
		{
			pEvent = new CKTDGXMeshPlayer::CXMeshEvent_SlashTrace; 
			CKTDGXMeshPlayer::CXMeshEvent_SlashTrace* pDestEvent = static_cast<CKTDGXMeshPlayer::CXMeshEvent_SlashTrace*>(pEvent);
			CKTDGXMeshPlayer::CXMeshEvent_SlashTrace* pSrcEvent = static_cast<CKTDGXMeshPlayer::CXMeshEvent_SlashTrace*>(pEvent_);
			pDestEvent->SetSlashTrace( pSrcEvent->GetSlashTrace() ); 
		}break;
	case CKTDGXMeshPlayer::ET_RENDERTYPE:		
		{
			pEvent = new CKTDGXMeshPlayer::CXMeshEvent_RenderType;  
			CKTDGXMeshPlayer::CXMeshEvent_RenderType* pDestEvent = static_cast<CKTDGXMeshPlayer::CXMeshEvent_RenderType*>(pEvent);
			CKTDGXMeshPlayer::CXMeshEvent_RenderType* pSrcEvent = static_cast<CKTDGXMeshPlayer::CXMeshEvent_RenderType*>(pEvent_);
			pDestEvent->SetRenderType( pSrcEvent->GetRenderType() ); 
		}break;
	case CKTDGXMeshPlayer::ET_POSITION:			
		{
			pEvent = new CKTDGXMeshPlayer::CXMeshEvent_Position;  
			CKTDGXMeshPlayer::CXMeshEvent_Position* pDestEvent = static_cast<CKTDGXMeshPlayer::CXMeshEvent_Position*>(pEvent);
			CKTDGXMeshPlayer::CXMeshEvent_Position* pSrcEvent = static_cast<CKTDGXMeshPlayer::CXMeshEvent_Position*>(pEvent_);
			pDestEvent->SetPosition( pSrcEvent->GetPosition() ); 
		}break;
	default:
		return NULL;
		break;
	}

	pEvent->SetFinal( pEvent_->IsFinal() );
	pEvent->SetFade( pEvent_->IsFade());
	pEvent->SetActualTime( pEvent_->GetActualTime() ) ;

	return pEvent;
}

CKTDGXMeshPlayer::CXMeshEvent* CX2EffectToolMeshEditor::CreateEventList( CKTDGXMeshPlayer::EVENT_TYPE eEventType )
{
	CKTDGXMeshPlayer::CXMeshEvent* pEvent = NULL;

	switch ( eEventType ) 
	{
	case CKTDGXMeshPlayer::ET_ANISPEED:			pEvent = new CKTDGXMeshPlayer::CXMeshEvent_AniSpeed;  break;
	case CKTDGXMeshPlayer::ET_ANINAME:			pEvent = new CKTDGXMeshPlayer::CXMeshEvent_AniName;  break;
	case CKTDGXMeshPlayer::ET_COLOR:			pEvent = new CKTDGXMeshPlayer::CXMeshEvent_Color;  break;
	case CKTDGXMeshPlayer::ET_OUTLINECOLOR:		pEvent = new CKTDGXMeshPlayer::CXMeshEvent_OutLineColor;  break;
	case CKTDGXMeshPlayer::ET_SIZE:				pEvent = new CKTDGXMeshPlayer::CXMeshEvent_Size;  break;
	case CKTDGXMeshPlayer::ET_VELOCITY:			pEvent = new CKTDGXMeshPlayer::CXMeshEvent_Velocity;  break;
	case CKTDGXMeshPlayer::ET_TEX0UV:			pEvent = new CKTDGXMeshPlayer::CXMeshEvent_Tex0UV;  break;
	case CKTDGXMeshPlayer::ET_TEX1UV:			pEvent = new CKTDGXMeshPlayer::CXMeshEvent_Tex1UV;  break;
	case CKTDGXMeshPlayer::ET_TEX2UV:			pEvent = new CKTDGXMeshPlayer::CXMeshEvent_Tex2UV;  break;
	case CKTDGXMeshPlayer::ET_ROTATE:			pEvent = new CKTDGXMeshPlayer::CXMeshEvent_Rotate;  break;
	case CKTDGXMeshPlayer::ET_DIRSPEED:			pEvent = new CKTDGXMeshPlayer::CXMeshEvent_DirSpeed;  break;
	case CKTDGXMeshPlayer::ET_BLACKHOLETIME:	pEvent = new CKTDGXMeshPlayer::CXMeshEvent_BlackHoleTime;  break;
	case CKTDGXMeshPlayer::ET_CRASH:			pEvent = new CKTDGXMeshPlayer::CXMeshEvent_Crash;  break;
	case CKTDGXMeshPlayer::ET_RESETCRASH:		pEvent = new CKTDGXMeshPlayer::CXMeshEvent_ResetCrash;  break;
	case CKTDGXMeshPlayer::ET_LIGHTFLOWIMPACT:	pEvent = new CKTDGXMeshPlayer::CXMeshEvent_LightFlowImpact;  break;
	case CKTDGXMeshPlayer::ET_LIGHTFLOWPOINT:	pEvent = new CKTDGXMeshPlayer::CXMeshEvent_LightFlowPoint;  break;
	case CKTDGXMeshPlayer::ET_SOUND:			pEvent = new CKTDGXMeshPlayer::CXMeshEvent_Sound;  break;
	case CKTDGXMeshPlayer::ET_GLOBALTIMER:		pEvent = new CKTDGXMeshPlayer::CXMeshEvent_RenderType;  break;
	case CKTDGXMeshPlayer::ET_SLASHTRACE:		pEvent = new CKTDGXMeshPlayer::CXMeshEvent_SlashTrace; break;
	case CKTDGXMeshPlayer::ET_RENDERTYPE:		pEvent = new CKTDGXMeshPlayer::CXMeshEvent_RenderType;  break;
	case CKTDGXMeshPlayer::ET_POSITION:			pEvent = new CKTDGXMeshPlayer::CXMeshEvent_Position;  break;
	default:
		return NULL;
		break;
	}

	SetEventDataByUIValue( pEvent );

	return pEvent;
}

void CX2EffectToolMeshEditor::ON_PA_LIST_EventList_Global_SelChnage( int iIndex_ )
{
	UpdateGlobalEventDataByIndex( iIndex_ );
	SetSelectEventListType( ELT_GLOBAL );
}
void CX2EffectToolMeshEditor::ON_PA_LIST_EventList_Anim_SelChnage( int iIndex_ )
{
	UpdateAnimEventDataByIndex( iIndex_ );
	SetSelectEventListType( ELT_ANIM );
}
void CX2EffectToolMeshEditor::ON_PA_LIST_EventList_Die_SelChnage( int iIndex_ )
{
	UpdateDieEventDataByIndex( iIndex_ );
	SetSelectEventListType( ELT_DIE );
}
void CX2EffectToolMeshEditor::UpdateEventList()
{
	if( NULL == m_pMeshTemplet )
		return;

	
	int nDel = SendDlgItemMessage( m_hDialogEditor , PA_LIST_EventList_Anim, LB_DELETESTRING, 0, 0);
	while(nDel > 0)
	{
		nDel = SendDlgItemMessage( m_hDialogEditor , PA_LIST_EventList_Anim, LB_DELETESTRING, 0, 0);
	}
	BOOST_FOREACH (CKTDGXMeshPlayer::CXMeshEvent* pEventList, m_pMeshTemplet->animTimeEventList )
	{
		wstring wstrEventData;
		ConvertCharToWCHAR( wstrEventData, CX2EffectToolFileManager::GetStringByEventData( pEventList ) );
		SendDlgItemMessage( m_hDialogEditor , PA_LIST_EventList_Anim, LB_ADDSTRING, 0, (LPARAM)wstrEventData.c_str() );
	}
	
	nDel = SendDlgItemMessage( m_hDialogEditor , PA_LIST_EventList_Global, LB_DELETESTRING, 0, 0);
	while(nDel > 0)
	{
		nDel = SendDlgItemMessage( m_hDialogEditor , PA_LIST_EventList_Global, LB_DELETESTRING, 0, 0);
	}
	BOOST_FOREACH (CKTDGXMeshPlayer::CXMeshEvent* pEventList, m_pMeshTemplet->globalTimeEventList )
	{
		wstring wstrEventData;
		ConvertCharToWCHAR( wstrEventData, CX2EffectToolFileManager::GetStringByEventData( pEventList ) );
		SendDlgItemMessage( m_hDialogEditor , PA_LIST_EventList_Global, LB_ADDSTRING, 0, (LPARAM)wstrEventData.c_str() );
	}
	
	nDel = SendDlgItemMessage( m_hDialogEditor , PA_LIST_EventList_Die, LB_DELETESTRING, 0, 0);
	while(nDel > 0)
	{
		nDel = SendDlgItemMessage( m_hDialogEditor , PA_LIST_EventList_Die, LB_DELETESTRING, 0, 0);
	}
	BOOST_FOREACH (CKTDGXMeshPlayer::CXMeshEvent* pEventList, m_pMeshTemplet->dieEventList )
	{
		wstring wstrEventData;
		ConvertCharToWCHAR( wstrEventData, CX2EffectToolFileManager::GetStringByEventData( pEventList ) );
		SendDlgItemMessage( m_hDialogEditor , PA_LIST_EventList_Die, LB_ADDSTRING, 0, (LPARAM)wstrEventData.c_str() );
	}
	
}