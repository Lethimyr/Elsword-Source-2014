#include "dxstdafx.h"
#include "X2EffectToolEditor.h"
#include "X2EffectToolParticleEditor.h"
#include "X2EffectToolFileManager.h"
#include "resource.h"

CX2EffectToolParticleEditor* g_pParticleEditor = NULL;

#pragma region Struct
NameAndIndex g_ParticleType[MAX_PARTICLE_TYPE] =
{			
	{L"PT_3D_PLANE",		CKTDGParticleSystem::PT_3D_PLANE },
	{L"PT_2D_PLANE",		CKTDGParticleSystem::PT_2D_PLANE },
	{L"PT_MESH",			CKTDGParticleSystem::PT_MESH },
	{L"PT_SKINMESH",		CKTDGParticleSystem::PT_SKINMESH },
};
NameAndIndex g_BillBoardType[MAX_BILL_BOARD_TYPE] =
{
	{L"BT_NONE",		CKTDGMatrix::BT_NONE},
	{L"BT_ALL",			CKTDGMatrix::BT_ALL},
	{L"BT_X",			CKTDGMatrix::BT_X},
	{L"BT_Y",			CKTDGMatrix::BT_Y},
	{L"BT_Z",			CKTDGMatrix::BT_Z},
};
NameAndIndex g_RenderType[MAX_RENDER_TYPE] =
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
NameAndIndex g_BlendMode[MAX_BLEND_MODE] =
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
NameAndIndex g_CullMode[MAX_CULL_MODE] =
{
	{L"D3DCULL_NONE",					D3DCULL_NONE},		
	{L"D3DCULL_CW",						D3DCULL_CW},		
	{L"D3DCULL_CCW",					D3DCULL_CCW},		
	{L"D3DCULL_FORCE_DWORD",			D3DCULL_FORCE_DWORD},
};
NameAndIndex g_EventListTimeType[MAX_EVENT_LIST_TIME_TYPE] =
{
	{L"Initital",      								ELTT_INITITAL },
	{L"At @1",      								ELTT_AT },
	{L"Fade So From @1 to @2",      				ELTT_FADE_SO_FROM },
	{L"Fade so Final",      						ELTT_FADE_SO_FINAL },
};
NameAndIndex g_EventType[MAX_EVENT_TYPE] =
{
	{L"ET_BASIC",			CKTDGParticleSystem::ET_BASIC},
	{L"ET_COLOR",			CKTDGParticleSystem::ET_COLOR},
	{L"ET_SIZE",			CKTDGParticleSystem::ET_SIZE},
	{L"ET_SCALE",			CKTDGParticleSystem::ET_SCALE},
	{L"ET_VELOCITY",		CKTDGParticleSystem::ET_VELOCITY},
	{L"ET_POSITION",		CKTDGParticleSystem::ET_POSITION},
	{L"ET_TEX0UV",			CKTDGParticleSystem::ET_TEX0UV},
	{L"ET_TEX1UV",			CKTDGParticleSystem::ET_TEX1UV},
	{L"ET_TEX2UV",			CKTDGParticleSystem::ET_TEX2UV},
	{L"ET_ROTATE",			CKTDGParticleSystem::ET_ROTATE},
	{L"ET_DIRSPEED",		CKTDGParticleSystem::ET_DIRSPEED},
	{L"ET_BLACKHOLE",		CKTDGParticleSystem::ET_BLACKHOLE},
	{L"ET_CRASH",			CKTDGParticleSystem::ET_CRASH},
	{L"ET_RESETCRASH",		CKTDGParticleSystem::ET_RESETCRASH},
	{L"ET_TEXTURE",			CKTDGParticleSystem::ET_TEXTURE},
	{L"ET_EVENTTIMER",		CKTDGParticleSystem::ET_EVENTTIMER},
	{L"ET_STRETCH",			CKTDGParticleSystem::ET_STRETCH},
};
#pragma endregion Struct

int CX2EffectToolParticleEditor::GetIndexByEnum( CKTDGParticleSystem::PARTICLE_TYPE eEnumValue )
{
	for( int i = 0; i < MAX_PARTICLE_TYPE ; ++i )
	{
		if( eEnumValue == g_ParticleType[i].iIndex )
		{
			return i;
		}
	}
	return -1;
}
int CX2EffectToolParticleEditor::GetIndexByEnum( CKTDGMatrix::BILLBOARD_TYPE  eEnumValue  )
{
	for( int i = 0; i < MAX_BILL_BOARD_TYPE ; ++i )
	{
		if( eEnumValue == g_BillBoardType[i].iIndex )
		{
			return i;
		}
	}
	return -1;
}
int CX2EffectToolParticleEditor::GetIndexByEnum( CKTDGXRenderer::RENDER_TYPE eEnumValue )
{
	for( int i = 0; i < MAX_RENDER_TYPE ; ++i )
	{
		if( eEnumValue == g_RenderType[i].iIndex )
		{
			return i;
		}
	}
	return -1;
}
int CX2EffectToolParticleEditor::GetIndexByEnum( D3DBLEND eEnumValue )
{
	for( int i = 0; i < MAX_BLEND_MODE ; ++i )
	{
		if( eEnumValue == g_BlendMode[i].iIndex )
		{
			return i;
		}
	}
	return -1;
}
int CX2EffectToolParticleEditor::GetIndexByEnum( _D3DCULL  eEnumValue  )
{
	for( int i = 0; i < MAX_CULL_MODE ; ++i )
	{
		if( eEnumValue == g_CullMode[i].iIndex )
		{
			return i;
		}
	}
	return -1;
}
int CX2EffectToolParticleEditor::GetIndexByEnum( EVENT_LIST_TIME_TYPE eEnumValue )
{
	for( int i = 0; i < MAX_EVENT_LIST_TIME_TYPE ; ++i )
	{
		if( eEnumValue == g_EventListTimeType[i].iIndex )
		{
			return i;
		}
	}
	return -1;
}
int CX2EffectToolParticleEditor::GetIndexByEnum( CKTDGParticleSystem::EVENT_TYPE eEnumValue  )
{
	for( int i = 0; i < MAX_EVENT_TYPE ; ++i )
	{
		if( eEnumValue == g_EventType[i].iIndex )
		{
			return i;
		}
	}
	return -1;
}

BOOL CALLBACK ParticleEditorBaseDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	if( NULL == g_pParticleEditor )
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
	
	WCHAR wszTextOut[64];
	StringCchPrintf( wszTextOut, ARRAY_SIZE(wszTextOut), L"lParam:%d\n", lParam );	
TRACE( wszTextOut );

	StringCchPrintf( wszTextOut, ARRAY_SIZE(wszTextOut), L"wParam:%d\n", wParam );	
	TRACE( wszTextOut );

	switch(LOWORD(wParam))
	{
#pragma region PA_LIST_EventList
	case PA_LIST_EventList:
		switch(HIWORD(wParam))
		{
		case LBN_SELCHANGE:
			{
					int index = SendDlgItemMessage(hDlg, PA_LIST_EventList, LB_GETCURSEL, 0, 0);
					g_pParticleEditor->UpdateEventDataByIndex( index );
				}
			return TRUE;
		}
		return TRUE;
#pragma endregion PA_LIST_EventList
	}


	switch(iMessage)
	{
#pragma region WM_CREATE
		case WM_CREATE:
	case WM_INITDIALOG:
		{
				for(int i=0; i<MAX_PARTICLE_TYPE; ++i)
				{
					SendDlgItemMessage(hDlg, PA_COMBO_Particle, CB_ADDSTRING,      0, (LPARAM)g_ParticleType[i].strName);
					SendDlgItemMessage(hDlg, PA_COMBO_Particle, CB_SETITEMDATA,    i, (LPARAM)g_ParticleType[i].iIndex);
				}
				for(int i=0; i<MAX_BILL_BOARD_TYPE; ++i)
				{
					SendDlgItemMessage(hDlg, PA_COMBO_Billboard, CB_ADDSTRING,      0, (LPARAM)g_BillBoardType[i].strName);
					SendDlgItemMessage(hDlg, PA_COMBO_Billboard, CB_SETITEMDATA,    i, (LPARAM)g_BillBoardType[i].iIndex);
				}
				for(int i=0; i<MAX_RENDER_TYPE; ++i)
				{
					SendDlgItemMessage(hDlg, PA_COMBO_MeshRender, CB_ADDSTRING,      0, (LPARAM)g_RenderType[i].strName);
					SendDlgItemMessage(hDlg, PA_COMBO_MeshRender, CB_SETITEMDATA,    i, (LPARAM)g_RenderType[i].iIndex);
				}
				for(int i=0; i<MAX_BLEND_MODE; ++i)
				{
					SendDlgItemMessage(hDlg, PA_COMBO_SourceBlend, CB_ADDSTRING,      0, (LPARAM)g_BlendMode[i].strName);
					SendDlgItemMessage(hDlg, PA_COMBO_SourceBlend, CB_SETITEMDATA,    i, (LPARAM)g_BlendMode[i].iIndex);
				}
				for(int i=0; i<MAX_BLEND_MODE; ++i)
				{
					SendDlgItemMessage(hDlg, PA_COMBO_DestBlend, CB_ADDSTRING,      0, (LPARAM)g_BlendMode[i].strName);
					SendDlgItemMessage(hDlg, PA_COMBO_DestBlend, CB_SETITEMDATA,    i, (LPARAM)g_BlendMode[i].iIndex);
				}
				for(int i=0; i<MAX_CULL_MODE; ++i)
				{
					SendDlgItemMessage(hDlg, PA_COMBO_CullMode, CB_ADDSTRING,      0, (LPARAM)g_CullMode[i].strName);
					SendDlgItemMessage(hDlg, PA_COMBO_CullMode, CB_SETITEMDATA,    i, (LPARAM)g_CullMode[i].iIndex);
				}

				for(int i=0; i<MAX_EVENT_LIST_TIME_TYPE; ++i)
				{
					SendDlgItemMessage(hDlg, PA_COMBO_TimeType, CB_ADDSTRING,      0, (LPARAM)g_EventListTimeType[i].strName);
					SendDlgItemMessage(hDlg, PA_COMBO_TimeType, CB_SETITEMDATA,    i, (LPARAM)g_EventListTimeType[i].iIndex);
				}
				for(int i=0; i<MAX_EVENT_TYPE; ++i)
				{
					SendDlgItemMessage(hDlg, PA_COMBO_EventType, CB_ADDSTRING,      0, (LPARAM)g_EventType[i].strName);
					SendDlgItemMessage(hDlg, PA_COMBO_EventType, CB_SETITEMDATA,    i, (LPARAM)g_EventType[i].iIndex);
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
					g_pParticleEditor->SaveParticleData();
				} break;
			case IDCANCEL:
				{
					g_pParticleEditor->CloseDialog();
				} break;
			case PA_IDC_EvnetList_ADD:
				{
					g_pParticleEditor->ON_PA_IDC_EvnetList_ADD();
				} return TRUE; 
			case PA_IDC_EvnetList_DEL:
				{
					g_pParticleEditor->ON_PA_IDC_EvnetList_DEL();
				} return TRUE;
			case PA_IDC_EvnetList_UP:
				{
					g_pParticleEditor->ON_PA_IDC_EvnetList_UP();
				} return TRUE; 
			case PA_IDC_EvnetList_DOWN:
				{
					g_pParticleEditor->ON_PA_IDC_EvnetList_DOWN();
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
						SetWindowText(GetDlgItem( hDlg , PA_EDIT_Mesh), wszTemp.c_str() );
					}
				} return TRUE;
			} return TRUE;	
		case WM_CHAR:
			{
				int i =0;
			}break;
		case WM_KEYDOWN:
			{
				switch( wParam )
				{
				case VK_TAB:
					{
						 int i =0;
					}	break;
				}
			} break;
	#pragma endregion WM_COMMAND
	}
	return FALSE;
}

BOOL CALLBACK ParticleEditorEtcDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	if( NULL == g_pParticleEditor )
		return FALSE;

	switch(iMessage)
	{ 
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDOK:
			{
				g_pParticleEditor->SaveParticleData();
			} break;
		case IDCANCEL:
			{
				g_pParticleEditor->CloseDialog();
			} break;
		case PA_BUTTON_Final_Seq_ADD:
			{
				g_pParticleEditor->ON_PA_BUTTON_Final_Seq_ADD();
			} break;
		case PA_BUTTON_Final_Seq_DEL:
			{
				g_pParticleEditor->ON_PA_BUTTON_Final_Seq_DEL();
			} break;
		case PA_BUTTON_Trace_Seq_ADD:
			{
				g_pParticleEditor->ON_PA_BUTTON_Trace_Seq_ADD();
			} break;
		case PA_BUTTON_Trace_Seq_DEL:
			{
				g_pParticleEditor->ON_PA_BUTTON_Trace_Seq_DEL();
			} break;
		} return TRUE;
	}
	return FALSE;
}
CX2EffectToolParticleEditor::CX2EffectToolParticleEditor(CX2EffectToolMain* pEffectTool_):
m_pEffectTool( pEffectTool_ )
,m_pParticleSequence(NULL)
,m_eDialogType( PDT_NONE )
{
}


CX2EffectToolParticleEditor::~CX2EffectToolParticleEditor(void)
{
}

void CX2EffectToolParticleEditor::OnFrameMove_HotKey()
{
	if( GetAsyncKeyState( VK_F2 ) & 0x8000 )
	{
		SaveParticleData();
	}
	else if( GetAsyncKeyState( VK_ESCAPE ) & 0x8000 )
	{
		CloseDialog();
	}
}

void CX2EffectToolParticleEditor::CloseDialog()
{
	DestroyWindow( m_hDialogEditor ) ;
	m_hDialogEditor = NULL;
}

void CX2EffectToolParticleEditor::CreateParticleEditor_Base( CKTDGParticleSystem::CParticleEventSequence* pParticle_ /*= NULL*/, wstring wstParticleFileName_ /*= L""*/)
{
	CloseDialog();
	if( NULL == m_hDialogEditor )
	{
		g_pParticleEditor = this;
		m_hDialogEditor = CreateDialog(g_pKTDXApp->GetHInstance(), 
										MAKEINTRESOURCE(IDD_PARTICLE_EDITOR), 
										g_pKTDXApp->GetHWND(), 
										ParticleEditorBaseDlgProc);
		ShowWindow(m_hDialogEditor, SW_SHOW);

		m_eDialogType = PDT_BASE;
	}
	if( NULL != pParticle_ )
	{
		UpdateParticleData(pParticle_, wstParticleFileName_ );
	}
}
void CX2EffectToolParticleEditor::CreateParticleEditor_Etc(CKTDGParticleSystem::CParticleEventSequence* pParticle_ /*= NULL*/, wstring wstParticleFileName_ /*= L""*/)
{
	CloseDialog();
	if( NULL == m_hDialogEditor )
	{
		g_pParticleEditor = this;
		m_hDialogEditor = CreateDialog(g_pKTDXApp->GetHInstance(), 
										MAKEINTRESOURCE(IDD_PARTICLE_EDITOR2), 
										g_pKTDXApp->GetHWND(), 
										ParticleEditorEtcDlgProc);
		ShowWindow(m_hDialogEditor, SW_SHOW);

		m_eDialogType = PDT_ETC;
	}
	if( NULL != pParticle_ )
	{
		UpdateParticleData(pParticle_, wstParticleFileName_ );
	}
}

void CX2EffectToolParticleEditor::UpdateParticleData( CKTDGParticleSystem::CParticleEventSequence* pParticle_, wstring wstParticleFileName_ /*= L""*/)
{
	if( NULL == m_hDialogEditor )
		return;

	if( NULL == pParticle_ )
		return;

	if( L"" != wstParticleFileName_ )
		m_wstrParticleFileName = wstParticleFileName_;

	m_pParticleSequence = pParticle_;

	SetWindowText(GetDlgItem( m_hDialogEditor, PA_EDIT_PARTICLE_NAME), m_pParticleSequence->GetName());
	SetWindowText(GetDlgItem( m_hDialogEditor, PA_EDIT_PARTICLE_FILE_NAME), m_wstrParticleFileName.c_str() );

	switch( m_eDialogType )
	{
	case PDT_BASE:
		{
			UpdateParticleData_Column1();
			UpdateParticleData_Column2();
			UpdateEventList();
		} break;
	case PDT_ETC:
		{
			UpdateParticleData_ETC_Column1();
			UpdateParticleData_ETC_Column2();
		} break;
	}
}
void CX2EffectToolParticleEditor::UpdateParticleData_Column1()
{
	if( NULL == m_pParticleSequence )
		return;

	SendDlgItemMessage( m_hDialogEditor , PA_COMBO_Particle, CB_SETCURSEL, (WPARAM)GetIndexByEnum(m_pParticleSequence->GetParticleType()), 0);
	SendDlgItemMessage( m_hDialogEditor , PA_COMBO_Billboard, CB_SETCURSEL, (WPARAM)GetIndexByEnum(m_pParticleSequence->GetBillBoardType()), 0);
	SendDlgItemMessage( m_hDialogEditor , PA_COMBO_MeshRender, CB_SETCURSEL, (WPARAM)GetIndexByEnum(m_pParticleSequence->GetMeshRenderType()), 0);	

	D3DBLEND eBlend = static_cast<D3DBLEND>(m_pParticleSequence->GetSrcBlendMode());
	SendDlgItemMessage( m_hDialogEditor , PA_COMBO_SourceBlend, CB_SETCURSEL, (WPARAM)GetIndexByEnum(eBlend), 0);

	eBlend = static_cast<D3DBLEND>(m_pParticleSequence->GetDestBlendMode());
	SendDlgItemMessage( m_hDialogEditor , PA_COMBO_DestBlend, CB_SETCURSEL, (WPARAM)GetIndexByEnum(eBlend), 0);

	D3DCULL eCull = static_cast<D3DCULL>(m_pParticleSequence->GetCullMode());
	SendDlgItemMessage( m_hDialogEditor , PA_COMBO_CullMode, CB_SETCURSEL, (WPARAM)GetIndexByEnum(eCull), 0);

	SetIntValue( PA_EDIT_Layer, m_pParticleSequence->GetLayer() );
	SetIntValue( PA_EDIT_Numparticles, m_pParticleSequence->GetMaxParticleNum() );
	SetWindowText(GetDlgItem( m_hDialogEditor, PA_EDIT_Mesh ), m_pParticleSequence->GetMeshName());
	SetWindowText(GetDlgItem( m_hDialogEditor, PA_EDIT_ChangeXet), m_pParticleSequence->GetChangeTexXETName() );
	
	SetCheckValue( PA_IDC_ZWriteEnable2, m_pParticleSequence->GetZWriteEnable() );
	SetCheckValue( PA_IDC_ZEnable, m_pParticleSequence->GetZEnable() );
	SetCheckValue( PA_IDC_ApplyUnitScale, m_pParticleSequence->GetApplyUnitScale() );
	SetCheckValue( PA_IDC_Trace, m_pParticleSequence->GetTrace() );
	SetCheckValue( PA_IDC_UseLand, m_pParticleSequence->GetUseLand() );

	vector<int> vecControlID;
	vecControlID.push_back( PA_EDIT_TriggerCount_Min );
	vecControlID.push_back( PA_EDIT_TriggerCount_Max);
	SetEditControlByValue( &m_pParticleSequence->GetTriggerCount_Origin(), vecControlID);

	vecControlID.clear();
	vecControlID.push_back( PA_EDIT_TriggerTime_Min );
	vecControlID.push_back( PA_EDIT_TriggerTime_Max );
	SetEditControlByValue( &m_pParticleSequence->GetTriggerTime_Origin(), vecControlID);

	vecControlID.clear();
	vecControlID.push_back( PA_EDIT_LifeTime_Min );
	vecControlID.push_back( PA_EDIT_LifeTime_Max);
	SetEditControlByValue( &m_pParticleSequence->GetLifeTime(), vecControlID);

	vecControlID.clear();
	vecControlID.push_back( PA_EDIT_EmitRate_Min );
	vecControlID.push_back( PA_EDIT_EmitRate_Max);
	SetEditControlByValue( &m_pParticleSequence->GetEmitRate(), vecControlID);

	vecControlID.clear();
	vecControlID.push_back( PA_EDIT_DrawCount_Min );
	vecControlID.push_back( PA_EDIT_DrawCount_Max );
	SetEditControlByValue( &m_pParticleSequence->GetDrawCount_Origin(), vecControlID);
}
void CX2EffectToolParticleEditor::UpdateParticleData_Column2()
{
	if( NULL == m_pParticleSequence )
		return;

	vector<int> vecControlID;
	vecControlID.push_back( PA_EDIT_LandPosition_Min );
	vecControlID.push_back( PA_EDIT_LandPosition_Max);
	SetEditControlByValue( &m_pParticleSequence->GetLandPosition_Origin(), vecControlID);

	vecControlID.clear();
	vecControlID.push_back( PA_EDIT_EmitRadius_Minx );
	vecControlID.push_back( PA_EDIT_EmitRadius_MinY );
	vecControlID.push_back( PA_EDIT_EmitRadius_MinZ );
	vecControlID.push_back( PA_EDIT_EmitRadius_MaxX );
	vecControlID.push_back( PA_EDIT_EmitRadius_MaxY );
	vecControlID.push_back( PA_EDIT_EmitRadius_MaxZ );
	SetEditControlByValue( &m_pParticleSequence->GetEmitRadius(), vecControlID);

	vecControlID.clear();
	vecControlID.push_back( PA_EDIT_Gravity_MinX );
	vecControlID.push_back( PA_EDIT_Gravity_MinY);
	vecControlID.push_back( PA_EDIT_Gravity_MinZ );
	vecControlID.push_back( PA_EDIT_Gravity_MaxX);
	vecControlID.push_back( PA_EDIT_Gravity_MaxY );
	vecControlID.push_back( PA_EDIT_Gravity_MaxZ );
	SetEditControlByValue( &m_pParticleSequence->GetGravity(), vecControlID);

	vecControlID.clear();
	vecControlID.push_back( PA_EDIT_RotateRel_MinX );
	vecControlID.push_back( PA_EDIT_RotateRel_MinY );
	vecControlID.push_back( PA_EDIT_RotateRel_MinZ );
	vecControlID.push_back( PA_EDIT_RotateRel_MaxX );
	vecControlID.push_back( PA_EDIT_RotateRel_MaxY );
	vecControlID.push_back( PA_EDIT_RotateRel_MaxZ );
	SetEditControlByValue( &m_pParticleSequence->GetAddRotateRel_Origin(), vecControlID);

	vecControlID.clear();
	vecControlID.push_back( PA_EDIT_LightPos_MinX );
	vecControlID.push_back( PA_EDIT_LightPos_MinY );
	vecControlID.push_back( PA_EDIT_LightPos_MinZ );
	vecControlID.push_back( PA_EDIT_LightPos_MaxX );
	vecControlID.push_back( PA_EDIT_LightPos_MaxY );
	vecControlID.push_back( PA_EDIT_LightPos_MaxZ );
	SetEditControlByValue( &m_pParticleSequence->GetLightPos_Origin(), vecControlID);
}
void CX2EffectToolParticleEditor::UpdateEventList()
{
	if( NULL == m_pParticleSequence )
		return;

	int nDel = SendDlgItemMessage( m_hDialogEditor , PA_LIST_EventList, LB_DELETESTRING, 0, 0);
	while(nDel > 0)
	{
		nDel = SendDlgItemMessage( m_hDialogEditor , PA_LIST_EventList, LB_DELETESTRING, 0, 0);
	}

	CKTDGParticleSystem::CParticleEventList::iterator it = m_pParticleSequence->GetEventList()->begin();
	for( ; it != m_pParticleSequence->GetEventList()->end(); ++it )
	{
		wstring wstrEventData;
		ConvertCharToWCHAR( wstrEventData, CX2EffectToolFileManager::GetStringByEventData( *it ) );
		SendDlgItemMessage( m_hDialogEditor , PA_LIST_EventList, LB_ADDSTRING, 0, (LPARAM)wstrEventData.c_str() );
	}
}
EVENT_LIST_TIME_TYPE CX2EffectToolParticleEditor::GetEventListTimeType ( CKTDGParticleSystem::CParticleEvent* pEventData_ )
{
	if( true == pEventData_->IsFade() )
	{
		//fade so final
		if( 0.f == pEventData_->GetActualTime().m_Min && 
			pEventData_->GetActualTime().m_Min == pEventData_->GetActualTime().m_Max )
			return ELTT_FADE_SO_FINAL;
		else // fade so from
			return ELTT_FADE_SO_FROM;
	}
	else
	{
		// Initial 
		if( 0.f == pEventData_->GetActualTime().m_Min && 
			pEventData_->GetActualTime().m_Min == pEventData_->GetActualTime().m_Max )
			return ELTT_INITITAL;
		else //at
			return ELTT_AT;
	}
	return ELTT_NONE;
}
void CX2EffectToolParticleEditor::UpdateEventData( CKTDGParticleSystem::CParticleEvent* pEventData_ )
{
	if( NULL == pEventData_ )
		return;

	char buf[100];

	int iComboBoxIndex = GetIndexByEnum(GetEventListTimeType( pEventData_ ));
	SendDlgItemMessage( m_hDialogEditor , PA_COMBO_TimeType, CB_SETCURSEL, (WPARAM)iComboBoxIndex , 0);
	iComboBoxIndex = GetIndexByEnum( pEventData_->GetEventType() );
	SendDlgItemMessage( m_hDialogEditor , PA_COMBO_EventType, CB_SETCURSEL, (WPARAM)iComboBoxIndex, 0);

	sprintf(buf, "%0.3f", pEventData_->GetActualTime().m_Min );
	SetWindowTextA(GetDlgItem( m_hDialogEditor, PA_EDIT_TimeValue1), buf);

	sprintf(buf, "%0.3f", pEventData_->GetActualTime().m_Max );
	SetWindowTextA(GetDlgItem( m_hDialogEditor, PA_EDIT_TimeValue2), buf);

	InitEventDataValue(); 

	switch( pEventData_->GetEventType() )
	{
		case CKTDGParticleSystem::ET_COLOR:
			{
				CKTDGParticleSystem::CParticleEvent_Color* pEvent = 
					static_cast<CKTDGParticleSystem::CParticleEvent_Color*>(pEventData_);

				SetEventDataValue( &pEvent->GetColor() );
			} break; //case CKTDGParticleSystem::ET_COLOR:

		case CKTDGParticleSystem::ET_SIZE:
			{
				CKTDGParticleSystem::CParticleEvent_Size* pEvent = 
					static_cast<CKTDGParticleSystem::CParticleEvent_Size*>(pEventData_);
				SetEventDataValue( &pEvent->GetSize() );
			} break; //case CKTDGParticleSystem::ET_SIZE:

		case CKTDGParticleSystem::ET_SCALE:
			{
				CKTDGParticleSystem::CParticleEvent_Scale* pEvent = 
					static_cast<CKTDGParticleSystem::CParticleEvent_Scale*>(pEventData_);

				SetEventDataValue( &pEvent->GetScale() );
			} break;

		case CKTDGParticleSystem::ET_VELOCITY:
			{
				CKTDGParticleSystem::CParticleEvent_Velocity* pEvent = 
					static_cast<CKTDGParticleSystem::CParticleEvent_Velocity*>(pEventData_);

				SetEventDataValue( &pEvent->GetVelocity() );
			} break;

		case CKTDGParticleSystem::ET_POSITION:
			{
				CKTDGParticleSystem::CParticleEvent_Position* pEvent = 
					static_cast<CKTDGParticleSystem::CParticleEvent_Position*>(pEventData_);

				SetEventDataValue( &pEvent->GetPosition() );
			} break;
		case CKTDGParticleSystem::ET_TEX0UV:
			{
				CKTDGParticleSystem::CParticleEvent_Tex0UV* pEvent = 
					static_cast<CKTDGParticleSystem::CParticleEvent_Tex0UV*>(pEventData_);

				SetEventDataValue( &pEvent->GetTexUV() );
			} break;
		case CKTDGParticleSystem::ET_TEX1UV:
			{
				CKTDGParticleSystem::CParticleEvent_Tex1UV* pEvent = 
					static_cast<CKTDGParticleSystem::CParticleEvent_Tex1UV*>(pEventData_);

				SetEventDataValue( &pEvent->GetTexUV() );
			} break;
		case CKTDGParticleSystem::ET_TEX2UV:
			{
				CKTDGParticleSystem::CParticleEvent_Tex2UV* pEvent = 
					static_cast<CKTDGParticleSystem::CParticleEvent_Tex2UV*>(pEventData_);

				SetEventDataValue( &pEvent->GetTexUV() );
			} break;
		case CKTDGParticleSystem::ET_ROTATE:
			{
				CKTDGParticleSystem::CParticleEvent_Rotate* pEvent = 
					static_cast<CKTDGParticleSystem::CParticleEvent_Rotate*>(pEventData_);

				SetEventDataValue( &pEvent->GetRotate() );
			} break; //case CKTDGParticleSystem::ET_SIZE:
		case CKTDGParticleSystem::ET_DIRSPEED:
			{
				CKTDGParticleSystem::CParticleEvent_DirSpeed* pEvent = 
					static_cast<CKTDGParticleSystem::CParticleEvent_DirSpeed*>(pEventData_);

				SetEventDataValue( &pEvent->GetValue() );
			} break;
		case CKTDGParticleSystem::ET_BLACKHOLE:
			{
				CKTDGParticleSystem::CParticleEvent_BlackHole* pEvent = 
					static_cast<CKTDGParticleSystem::CParticleEvent_BlackHole*>(pEventData_);

				SetEventDataValue( &pEvent->GetEventTimer() );
			} break;
		case CKTDGParticleSystem::ET_CRASH:
			{
				CKTDGParticleSystem::CParticleEvent_Crash* pEvent = 
					static_cast<CKTDGParticleSystem::CParticleEvent_Crash*>(pEventData_);

				SetEventDataValue( &pEvent->GetCrash() );
			} break;
		case CKTDGParticleSystem::ET_RESETCRASH:
			{
				CKTDGParticleSystem::CParticleEvent_ResetCrash* pEvent = 
					static_cast<CKTDGParticleSystem::CParticleEvent_ResetCrash*>(pEventData_);

				SetEventDataValue( &pEvent->GetResetCrash() );
			} break;
		case CKTDGParticleSystem::ET_TEXTURE:
			{
				CKTDGParticleSystem::CParticleEvent_Texture* pEvent = 
					static_cast<CKTDGParticleSystem::CParticleEvent_Texture*>(pEventData_);

				SetEventDataValue( pEvent->GetTextureName());
			} break;
		case CKTDGParticleSystem::ET_EVENTTIMER:
			{
				CKTDGParticleSystem::CParticleEvent_EventTimer* pEvent = 
					static_cast<CKTDGParticleSystem::CParticleEvent_EventTimer*>(pEventData_);

				SetEventDataValue( &pEvent->GetEventTimer());
			} break;
		case CKTDGParticleSystem::ET_STRETCH:
			{
				CKTDGParticleSystem::CParticleEvent_Stretch* pEvent = 
					static_cast<CKTDGParticleSystem::CParticleEvent_Stretch*>(pEventData_);

				SetEventDataValue( &pEvent->GetValue());
			} break;
		}
}
void CX2EffectToolParticleEditor::UpdateEventDataByIndex( int iIndex_ )
{
	if( 0 > iIndex_ )
		return;

	CKTDGParticleSystem::CParticleEvent* pEvent = NULL;

	CKTDGParticleSystem::CParticleEventList::iterator it = m_pParticleSequence->GetEventList()->begin();
	for( int iIndex = 0; it != m_pParticleSequence->GetEventList()->end(); ++it, ++iIndex )
	{
		if( iIndex_ == iIndex)
		{
			pEvent = *it;
			break;
		}
	}

	if( NULL != pEvent )
	{
		UpdateEventData( pEvent );
	}
}
void CX2EffectToolParticleEditor::SaveParticleData()
{
	if( NULL == m_pParticleSequence )
		return;

	if( NULL == m_hDialogEditor )
		return;

	if( NULL == m_pEffectTool )
		return;

	// 	// 다른이름으로 저장.
	bool bIsSaveAs = SaveParticleData_OtherName();

	int iOldSelectEventList = -1;
	switch( m_eDialogType )
	{
	case PDT_BASE:
		{
			// 기존 선택되어 있는 이벤트 리스트 인덱스 기록.
			iOldSelectEventList = GetSelectEventListIndex();

			//저장
			SaveParticleData_Column1();
			SaveParticleData_Column2();
			SaveParticleData_EventList();
		} break;
	case PDT_ETC:
		{
			SaveParticleData_ETC_Column1();
		} break;
	}

	//갱신
	UpdateParticleData( m_pParticleSequence );

	if( PDT_BASE == m_eDialogType )
	{
		// 이벤트 리스트 선택.
		SelectEventListByIndex( iOldSelectEventList );
		UpdateEventData( GetSelectEventData() );
	}

	if( NULL != m_pEffectTool)
	{
 		if( true == bIsSaveAs )
 		{
			m_pEffectTool->UpdateParticleListByFileName( m_wstrParticleFileName.c_str() );
			m_pEffectTool->SelectParticleListLastIndex();
 		}
		// 파티클 재생.
		m_pEffectTool->PreViewParticle( m_pParticleSequence->GetName() );
	}
}
void CX2EffectToolParticleEditor::SaveParticleData_Column1()
{
	if( NULL == m_pParticleSequence )
		return;

	m_pParticleSequence->SetParticleType( static_cast<CKTDGParticleSystem::PARTICLE_TYPE>(GetComboValue( PA_COMBO_Particle )));
	m_pParticleSequence->SetBillBoardType( static_cast<CKTDGMatrix::BILLBOARD_TYPE>(GetComboValue( PA_COMBO_Billboard )));
	m_pParticleSequence->SetMeshRenderType( static_cast<CKTDGXRenderer::RENDER_TYPE>(GetComboValue( PA_COMBO_MeshRender )));
	m_pParticleSequence->SetSrcBlendMode( static_cast<D3DBLEND>(GetComboValue( PA_COMBO_SourceBlend )));
	m_pParticleSequence->SetDestBlendMode( static_cast<D3DBLEND>(GetComboValue( PA_COMBO_DestBlend )));
	m_pParticleSequence->SetCullMode( static_cast<D3DCULL>(GetComboValue( PA_COMBO_CullMode )));

	m_pParticleSequence->SetLayer( GetIntValue( PA_EDIT_Layer ));
	m_pParticleSequence->SetMaxParticleNum( GetIntValue( PA_EDIT_Numparticles ));
	m_pParticleSequence->SetMeshName( GetString( PA_EDIT_Mesh ).c_str() );
	m_pParticleSequence->SetChangeTexXETName( GetString( PA_EDIT_ChangeXet ).c_str());

	m_pParticleSequence->SetZEnable( GetChecked( PA_IDC_ZEnable ));
	m_pParticleSequence->SetZWriteEnable( GetChecked( PA_IDC_ZWriteEnable2 ));
	m_pParticleSequence->SetApplyUnitScale( GetChecked( PA_IDC_ApplyUnitScale ));
	m_pParticleSequence->SetTrace( GetChecked( PA_IDC_Trace ));
	m_pParticleSequence->SetUseLand( GetChecked( PA_IDC_UseLand ));

	CMinMax<float> cFloatTemp;
	vector<int> vecControlID;
	vecControlID.push_back( PA_EDIT_TriggerCount_Min );
	vecControlID.push_back( PA_EDIT_TriggerCount_Max);
	SetValueByEditControl( &cFloatTemp, vecControlID );
	m_pParticleSequence->SetTriggerCount_Origin( cFloatTemp );

	vecControlID.clear();
	vecControlID.push_back( PA_EDIT_TriggerTime_Min );
	vecControlID.push_back( PA_EDIT_TriggerTime_Max);
	SetValueByEditControl( &cFloatTemp, vecControlID );
	m_pParticleSequence->SetTriggerTime_Origin( cFloatTemp );

	vecControlID.clear();
	vecControlID.push_back( PA_EDIT_LifeTime_Min );
	vecControlID.push_back( PA_EDIT_LifeTime_Max);
	SetValueByEditControl( &cFloatTemp, vecControlID );
	m_pParticleSequence->SetLifetime( cFloatTemp );

	vecControlID.clear();
	vecControlID.push_back( PA_EDIT_EmitRate_Min );
	vecControlID.push_back( PA_EDIT_EmitRate_Max);
	SetValueByEditControl( &cFloatTemp, vecControlID );
	m_pParticleSequence->SetEmitRate( cFloatTemp );

	vecControlID.clear();
	vecControlID.push_back( PA_EDIT_DrawCount_Min );
	vecControlID.push_back( PA_EDIT_DrawCount_Max );
	SetValueByEditControl( &cFloatTemp, vecControlID );
	m_pParticleSequence->SetDrawCount_Origin( cFloatTemp );
}
void CX2EffectToolParticleEditor::SaveParticleData_Column2()
{
	if( NULL == m_pParticleSequence )
		return;

	CMinMax<float> cFloatTemp;
	vector<int> vecControlID;
	vecControlID.push_back( PA_EDIT_LandPosition_Min );
	vecControlID.push_back( PA_EDIT_LandPosition_Max);
	SetValueByEditControl( &cFloatTemp, vecControlID );
	m_pParticleSequence->SetLandPosition_Origin( cFloatTemp );


	CMinMax<D3DXVECTOR3> cVec3Temp;
	vecControlID.clear();
	vecControlID.push_back( PA_EDIT_EmitRadius_Minx );
	vecControlID.push_back( PA_EDIT_EmitRadius_MinY );
	vecControlID.push_back( PA_EDIT_EmitRadius_MinZ );
	vecControlID.push_back( PA_EDIT_EmitRadius_MaxX );
	vecControlID.push_back( PA_EDIT_EmitRadius_MaxY );
	vecControlID.push_back( PA_EDIT_EmitRadius_MaxZ );
	SetValueByEditControl( &cVec3Temp, vecControlID );
	m_pParticleSequence->SetEmitRadius( cVec3Temp );

	vecControlID.clear();
	vecControlID.push_back( PA_EDIT_Gravity_MinX );
	vecControlID.push_back( PA_EDIT_Gravity_MinY);
	vecControlID.push_back( PA_EDIT_Gravity_MinZ );
	vecControlID.push_back( PA_EDIT_Gravity_MaxX);
	vecControlID.push_back( PA_EDIT_Gravity_MaxY );
	vecControlID.push_back( PA_EDIT_Gravity_MaxZ );
	SetValueByEditControl( &cVec3Temp, vecControlID );
	m_pParticleSequence->SetGravity( cVec3Temp );

	vecControlID.clear();
	vecControlID.push_back( PA_EDIT_RotateRel_MinX );
	vecControlID.push_back( PA_EDIT_RotateRel_MinY );
	vecControlID.push_back( PA_EDIT_RotateRel_MinZ );
	vecControlID.push_back( PA_EDIT_RotateRel_MaxX );
	vecControlID.push_back( PA_EDIT_RotateRel_MaxY );
	vecControlID.push_back( PA_EDIT_RotateRel_MaxZ );
	SetValueByEditControl( &cVec3Temp, vecControlID );
	m_pParticleSequence->SetAddRotateRel_Origin( cVec3Temp );

	vecControlID.clear();
	vecControlID.push_back( PA_EDIT_LightPos_MinX );
	vecControlID.push_back( PA_EDIT_LightPos_MinY );
	vecControlID.push_back( PA_EDIT_LightPos_MinZ );
	vecControlID.push_back( PA_EDIT_LightPos_MaxX );
	vecControlID.push_back( PA_EDIT_LightPos_MaxY );
	vecControlID.push_back( PA_EDIT_LightPos_MaxZ );
	SetValueByEditControl( &cVec3Temp, vecControlID );
	m_pParticleSequence->SetLightPos_Origin( cVec3Temp );
}
void CX2EffectToolParticleEditor::SaveParticleData_EventList()
{
	if( NULL == m_pParticleSequence )
		return;

	CKTDGParticleSystem::CParticleEventList* pEventList = m_pParticleSequence->GetEventList();
	if( NULL == pEventList )
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
	CKTDGParticleSystem::EVENT_TYPE eEventType = static_cast<CKTDGParticleSystem::EVENT_TYPE>(GetComboValue( PA_COMBO_EventType ));

	// 선택된 이벤트 리스트 요소 얻기.
	int iEventIndex = GetSelectEventListIndex();	
	CKTDGParticleSystem::CParticleEvent* pEventData = GetSelectEventData();
	if( NULL != pEventData )
	{
		if( eEventType == pEventData->GetEventType() )
 			SetEventDataByUIValue( pEventData );
 		else
 		{//타입이 다르다면 새로 생성.
 			pEventData = CreateEventList( eEventType );
 			if( NULL != pEventData )
 			{
				//기존 값 제거.
				EraseEventListByIndex( iEventIndex );
 
 				//새로운 값 삽입
				if( static_cast<int>(pEventList->size()) == iEventIndex )
					pEventList->push_back( pEventData);
 				else
					InsertEventListByIndex( iEventIndex, pEventData );
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
				pEventList->push_back( pEventData);
 		}
	}

	if( NULL != pEventData )
	{
		pEventData->SetFade( bFade );
		pEventData->SetActualTime( cActualTime );
	}
}
int CX2EffectToolParticleEditor::GetSelectEventListIndex()
{
	return SendDlgItemMessage( m_hDialogEditor, PA_LIST_EventList, LB_GETCURSEL, 0, 0);
}
void CX2EffectToolParticleEditor::SetEventDataByUIValue( CKTDGParticleSystem::CParticleEvent* pEventData_ , bool bInit_ /*= false */)
{
	if( NULL == pEventData_ )
		return;

	// 모든 값들을 미리 읽어드린 다음, 
	// 각 타입에 맞게 필요한 값만 저장하도록 작업함.
	char buf[100];
	// D3DXCOLOR
	CMinMax<D3DXCOLOR> cColor;
	if( true == bInit_ )
	{
		cColor.m_Max = D3DXCOLOR(1,1,1,1);
		cColor.m_Min = D3DXCOLOR(1,1,1,1);
	}
	else
	{
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
	}

	// D3DXVECTOR3
	CMinMax<D3DXVECTOR3> cVec3;	
	if( true == bInit_ )
	{
		cVec3.m_Max = D3DXVECTOR3(1,1,1);
		cVec3.m_Min = D3DXVECTOR3(1,1,1);
	}
	else
	{
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
	}

	// D3DXVECTOR2
	CMinMax<D3DXVECTOR2> cVec2;		
	if( true == bInit_ )
	{
		cVec2.m_Max = D3DXVECTOR2(1,1);
		cVec2.m_Min = D3DXVECTOR2(1,1);
	}
	else
	{
		GetWindowTextA(GetDlgItem(m_hDialogEditor, PA_COMBO_VECTOR2_MinX ), buf, 100);
		cVec2.m_Min.x = static_cast<float>(atof(buf));
		GetWindowTextA(GetDlgItem(m_hDialogEditor, PA_COMBO_VECTOR2_MinY ), buf, 100);
		cVec2.m_Min.y = static_cast<float>(atof(buf));
		GetWindowTextA(GetDlgItem(m_hDialogEditor, PA_COMBO_VECTOR2_MaxX ), buf, 100);
		cVec2.m_Max.x = max( cVec2.m_Min.x, static_cast<float>(atof(buf)) );
		GetWindowTextA(GetDlgItem(m_hDialogEditor, PA_COMBO_VECTOR2_MaxY ), buf, 100);
		cVec2.m_Max.y = max( cVec2.m_Min.y, static_cast<float>(atof(buf)) );
	}

	// float
	CMinMax<float> cFloat;	
	if( true == bInit_ )
	{
		cFloat.m_Max = 1.f;
		cFloat.m_Min = 1.f;
	}
	else
	{
		GetWindowTextA(GetDlgItem(m_hDialogEditor, PA_COMBO_FLOAT_Min ), buf, 100);
		cFloat.m_Min = static_cast<float>(atof(buf));
		GetWindowTextA(GetDlgItem(m_hDialogEditor, PA_COMBO_FLOAT_Max ), buf, 100);
		cFloat.m_Max = max( cFloat.m_Min, static_cast<float>(atof(buf)) );
	}

	// string	
	wstring wsValue = L"Temp";
	if( false == bInit_ )
		wsValue = GetString( PA_EDIT_STRING );

	switch( pEventData_->GetEventType() )
	{
	case CKTDGParticleSystem::ET_COLOR:
		{
			CKTDGParticleSystem::CParticleEvent_Color* pEvent = 
				static_cast<CKTDGParticleSystem::CParticleEvent_Color*>(pEventData_);

			pEvent->SetColor( cColor );
		} break; //case CKTDGParticleSystem::ET_COLOR:

	case CKTDGParticleSystem::ET_SIZE:
		{
			CKTDGParticleSystem::CParticleEvent_Size* pEvent = 
				static_cast<CKTDGParticleSystem::CParticleEvent_Size*>(pEventData_);

			pEvent->SetSize(cVec3);
		} break; //case CKTDGParticleSystem::ET_SIZE:

	case CKTDGParticleSystem::ET_SCALE:
		{
			CKTDGParticleSystem::CParticleEvent_Scale* pEvent = 
				static_cast<CKTDGParticleSystem::CParticleEvent_Scale*>(pEventData_);

			pEvent->SetScale( cFloat );
		} break;

	case CKTDGParticleSystem::ET_VELOCITY:
		{
			CKTDGParticleSystem::CParticleEvent_Velocity* pEvent = 
				static_cast<CKTDGParticleSystem::CParticleEvent_Velocity*>(pEventData_);

			pEvent->SetVelocity(cVec3);
		} break;

	case CKTDGParticleSystem::ET_POSITION:
		{
			CKTDGParticleSystem::CParticleEvent_Position* pEvent = 
				static_cast<CKTDGParticleSystem::CParticleEvent_Position*>(pEventData_);

			pEvent->SetPosition(cVec3);
		} break;
	case CKTDGParticleSystem::ET_TEX0UV:
		{
			CKTDGParticleSystem::CParticleEvent_Tex0UV* pEvent = 
				static_cast<CKTDGParticleSystem::CParticleEvent_Tex0UV*>(pEventData_);

			pEvent->SetTexUV( cVec2 );
		} break;
	case CKTDGParticleSystem::ET_TEX1UV:
		{
			CKTDGParticleSystem::CParticleEvent_Tex1UV* pEvent = 
				static_cast<CKTDGParticleSystem::CParticleEvent_Tex1UV*>(pEventData_);

			pEvent->SetTexUV( cVec2 );
		} break;
	case CKTDGParticleSystem::ET_TEX2UV:
		{
			CKTDGParticleSystem::CParticleEvent_Tex2UV* pEvent = 
				static_cast<CKTDGParticleSystem::CParticleEvent_Tex2UV*>(pEventData_);

			pEvent->SetTexUV( cVec2 );
		} break;
	case CKTDGParticleSystem::ET_ROTATE:
		{
			CKTDGParticleSystem::CParticleEvent_Rotate* pEvent = 
				static_cast<CKTDGParticleSystem::CParticleEvent_Rotate*>(pEventData_);

			pEvent->SetRotate( cVec3 );
		} break; //case CKTDGParticleSystem::ET_SIZE:
	case CKTDGParticleSystem::ET_DIRSPEED:
		{
			CKTDGParticleSystem::CParticleEvent_DirSpeed* pEvent = 
				static_cast<CKTDGParticleSystem::CParticleEvent_DirSpeed*>(pEventData_);

			pEvent->SetValue(cFloat);
		} break;
	case CKTDGParticleSystem::ET_BLACKHOLE:
		{
			CKTDGParticleSystem::CParticleEvent_BlackHole* pEvent = 
				static_cast<CKTDGParticleSystem::CParticleEvent_BlackHole*>(pEventData_);

			pEvent->SetEventTimer(cFloat);
		} break;
	case CKTDGParticleSystem::ET_CRASH:
		{
			CKTDGParticleSystem::CParticleEvent_Crash* pEvent = 
				static_cast<CKTDGParticleSystem::CParticleEvent_Crash*>(pEventData_);

			pEvent->SetCrash(cVec3);
		} break;
	case CKTDGParticleSystem::ET_RESETCRASH:
		{
			CKTDGParticleSystem::CParticleEvent_ResetCrash* pEvent = 
				static_cast<CKTDGParticleSystem::CParticleEvent_ResetCrash*>(pEventData_);
			pEvent->SetResetCrash(cFloat);
		} break;
	case CKTDGParticleSystem::ET_TEXTURE:
		{
			CKTDGParticleSystem::CParticleEvent_Texture* pEvent = 
				static_cast<CKTDGParticleSystem::CParticleEvent_Texture*>(pEventData_);

			pEvent->SetTextureName( wsValue.c_str() );
		} break;
	case CKTDGParticleSystem::ET_EVENTTIMER:
		{
			CKTDGParticleSystem::CParticleEvent_EventTimer* pEvent = 
				static_cast<CKTDGParticleSystem::CParticleEvent_EventTimer*>(pEventData_);

			pEvent->SetEventTimer( cFloat );
		} break;
	case CKTDGParticleSystem::ET_STRETCH:
		{
			CKTDGParticleSystem::CParticleEvent_Stretch* pEvent = 
				static_cast<CKTDGParticleSystem::CParticleEvent_Stretch*>(pEventData_);

			pEvent->SetValue(cFloat);
		} break;
	}
}
void CX2EffectToolParticleEditor::ON_PA_IDC_EvnetList_ADD()
{
	if( NULL == m_pParticleSequence )
		return;

	if( NULL == m_pParticleSequence->GetEventList())
		return;

	//더미 이벤트 추가
	SendDlgItemMessage( m_hDialogEditor , PA_LIST_EventList, LB_ADDSTRING, 0, (LPARAM)L"추가" );
	SetWindowTextA(GetDlgItem( m_hDialogEditor, PA_COMBO_EventType ), "");
	SelectEventListByIndex( m_pParticleSequence->GetEventList()->size() );
	InitEventDataValue();
}
void CX2EffectToolParticleEditor::ON_PA_IDC_EvnetList_UP()
{
	CKTDGParticleSystem::CParticleEventList* pParticleEventList = m_pParticleSequence->GetEventList();
	if( NULL == pParticleEventList )
		return;

	// 선택된 이벤트리스트 인덱스.
	UINT uiEventListIndex = GetSelectEventListIndex();
	
	// 선택된 이벤트가 최상위라면 더이상 UP 불가.
	if( 0 == uiEventListIndex  )
		return;

	CKTDGParticleSystem::CParticleEvent* pEvent = GetSelectEventData();
	if( NULL == pEvent )
		return;

	// 복사본 생성.
	CKTDGParticleSystem::CParticleEvent* pTempEvent = CreateEventList( pEvent->GetEventType() );
	if( NULL != pTempEvent )
	{
		pTempEvent = pEvent;

		// 복사본을 한칸 위에 추가.
		InsertEventListByIndex( uiEventListIndex - 1, pTempEvent );

		// 원본 제거
		EraseEventListByIndex( uiEventListIndex + 1);
	}

	// 갱신
	UpdateEventList();
	SelectEventListByIndex( uiEventListIndex - 1);
}
void CX2EffectToolParticleEditor::ON_PA_IDC_EvnetList_DOWN()
{
	CKTDGParticleSystem::CParticleEventList* pParticleEventList = m_pParticleSequence->GetEventList();
	if( NULL == pParticleEventList )
		return;

	// 선택된 이벤트리스트 인덱스.
	UINT uiEventListIndex = GetSelectEventListIndex();

	// 선택된 이벤트가 최하위라면 더이상 DOWN 불가.
	if( pParticleEventList->size() == (uiEventListIndex + 1) )
		return;

	CKTDGParticleSystem::CParticleEvent* pEvent = GetSelectEventData();
	if( NULL == pEvent )
		return;

	// 복사본 생성.
	CKTDGParticleSystem::CParticleEvent* pTempEvent = CreateEventList( pEvent->GetEventType() );
	if( NULL != pTempEvent )
	{
		pTempEvent = pEvent;

		// 복사본을 한칸 아래에 추가.
		InsertEventListByIndex( uiEventListIndex + 2, pTempEvent );

		// 원본 제거
		EraseEventListByIndex( uiEventListIndex );
	}

	// 갱신
	UpdateEventList();
	SelectEventListByIndex( uiEventListIndex + 1);
}
void CX2EffectToolParticleEditor::ON_PA_IDC_EvnetList_DEL()// 이벤트 리스트 제거
{	
	if( NULL == m_pParticleSequence )
		return;

	CKTDGParticleSystem::CParticleEventList* pEventList = m_pParticleSequence->GetEventList();
	if( NULL == pEventList )
		return;

	int iEventIndex = GetSelectEventListIndex();	
	
	// 제거
	EraseEventListByIndex( iEventIndex );
	// 갱신
	UpdateEventList();

	// 이벤트 리스트 선택.
	iEventIndex = min( static_cast<int>( pEventList->size()) - 1, iEventIndex );
	SelectEventListByIndex( iEventIndex );
	UpdateEventData( GetSelectEventData() );
}
void CX2EffectToolParticleEditor::EraseEventListByIndex( int iIndex_ )
{
	if( NULL == m_pParticleSequence )
		return;

	if( 0 > iIndex_ )
		return;

	CKTDGParticleSystem::CParticleEventList* pEventList = m_pParticleSequence->GetEventList();
	if( NULL == pEventList )
		return;

	if( 0 == iIndex_ )
	{
		pEventList->pop_front();
	}
	else if(  pEventList->size() == iIndex_ )
	{
		pEventList->pop_back();
	}
	else
	{
		CKTDGParticleSystem::CParticleEventList::iterator it = pEventList->begin();
		for( int iIndex = 0; it != pEventList->end(); ++it, ++iIndex )
		{
			if( iIndex_ == iIndex )
			{
				pEventList->erase( it );
				break;
			}
		}
	}
}
void CX2EffectToolParticleEditor::InsertEventListByIndex ( int iIndex_, CKTDGParticleSystem::CParticleEvent* pEventData_)
{
	if( NULL == m_pParticleSequence )
		return;

	CKTDGParticleSystem::CParticleEventList* pEventList = m_pParticleSequence->GetEventList();
	if( NULL == pEventList )
		return;

	if( 0 > iIndex_ )
		return;

	if( NULL == pEventData_ )
		return;

	if( 0 == iIndex_ )
	{
		pEventList->push_front( pEventData_ );
	}
	else if(  pEventList->size() == iIndex_ )
	{
		pEventList->push_back( pEventData_ );
	}
	else
	{
		CKTDGParticleSystem::CParticleEventList::iterator it = pEventList->begin();
		for( int iIndex = 0; it != pEventList->end(); ++it, ++iIndex )
		{
			if( iIndex_ == iIndex )
			{
				pEventList->insert( it, pEventData_ );
				break;
			}
		}
	}
}
void CX2EffectToolParticleEditor::SelectEventListByIndex( int iIndex_ )
{
	SendDlgItemMessage( m_hDialogEditor, PA_LIST_EventList, LB_SETCURSEL, iIndex_, 0);
}
CKTDGParticleSystem::CParticleEvent* CX2EffectToolParticleEditor::GetSelectEventData()
{
	if( NULL == m_pParticleSequence )
		return NULL;

	CKTDGParticleSystem::CParticleEventList* pEventList = m_pParticleSequence->GetEventList();
	if( NULL == pEventList )
		return NULL;

	int iEventIndex = GetSelectEventListIndex();	
	CKTDGParticleSystem::CParticleEventList::iterator it = pEventList->begin();
	for( int iIndex = 0; it != pEventList->end(); ++it, ++iIndex )
	{
		if( iEventIndex == iIndex )
		{
			return *it;
			break;
		}
	}

	return NULL;
}
CKTDGParticleSystem::CParticleEvent* CX2EffectToolParticleEditor::CreateEventList( CKTDGParticleSystem::CParticleEvent* pEvent_ )
{
	CKTDGParticleSystem::CParticleEvent* pEvent = NULL;

	switch ( pEvent_->GetEventType() ) 
	{
	case CKTDGParticleSystem::ET_COLOR:;
		{
			pEvent = new CKTDGParticleSystem::CParticleEvent_Color;
			CKTDGParticleSystem::CParticleEvent_Color* pDestEvent = static_cast<CKTDGParticleSystem::CParticleEvent_Color*>(pEvent);
			CKTDGParticleSystem::CParticleEvent_Color* pSrcEvent = static_cast<CKTDGParticleSystem::CParticleEvent_Color*>(pEvent_);
			pDestEvent->SetColor( pSrcEvent->GetColor() );
		}break;
	case CKTDGParticleSystem::ET_SIZE:			  
		{
			pEvent = new CKTDGParticleSystem::CParticleEvent_Size;
			CKTDGParticleSystem::CParticleEvent_Size* pDestEvent = static_cast<CKTDGParticleSystem::CParticleEvent_Size*>(pEvent);
			CKTDGParticleSystem::CParticleEvent_Size* pSrcEvent = static_cast<CKTDGParticleSystem::CParticleEvent_Size*>(pEvent_);
			pDestEvent->SetSize( pSrcEvent->GetSize() );
		}break;
	case CKTDGParticleSystem::ET_SCALE:			  
		{
			pEvent = new CKTDGParticleSystem::CParticleEvent_Scale;		
			CKTDGParticleSystem::CParticleEvent_Scale* pDestEvent = static_cast<CKTDGParticleSystem::CParticleEvent_Scale*>(pEvent);
			CKTDGParticleSystem::CParticleEvent_Scale* pSrcEvent = static_cast<CKTDGParticleSystem::CParticleEvent_Scale*>(pEvent_);
			pDestEvent->SetScale( pSrcEvent->GetScale() );
		}break;
	case CKTDGParticleSystem::ET_VELOCITY:		  
		{
			pEvent = new CKTDGParticleSystem::CParticleEvent_Velocity;		
			CKTDGParticleSystem::CParticleEvent_Velocity* pDestEvent = static_cast<CKTDGParticleSystem::CParticleEvent_Velocity*>(pEvent);
			CKTDGParticleSystem::CParticleEvent_Velocity* pSrcEvent = static_cast<CKTDGParticleSystem::CParticleEvent_Velocity*>(pEvent_);
			pDestEvent->SetVelocity( pSrcEvent->GetVelocity() );
		}break;
	case CKTDGParticleSystem::ET_POSITION:		  
		{
			pEvent = new CKTDGParticleSystem::CParticleEvent_Position;		
			CKTDGParticleSystem::CParticleEvent_Position* pDestEvent = static_cast<CKTDGParticleSystem::CParticleEvent_Position*>(pEvent);
			CKTDGParticleSystem::CParticleEvent_Position* pSrcEvent = static_cast<CKTDGParticleSystem::CParticleEvent_Position*>(pEvent_);
			pDestEvent->SetPosition( pSrcEvent->GetPosition() );	
		}break;
	case CKTDGParticleSystem::ET_TEX0UV:		  
		{
			pEvent = new CKTDGParticleSystem::CParticleEvent_Tex0UV;	
			CKTDGParticleSystem::CParticleEvent_Tex0UV* pDestEvent = static_cast<CKTDGParticleSystem::CParticleEvent_Tex0UV*>(pEvent);
			CKTDGParticleSystem::CParticleEvent_Tex0UV* pSrcEvent = static_cast<CKTDGParticleSystem::CParticleEvent_Tex0UV*>(pEvent_);
			pDestEvent->SetTexUV( pSrcEvent->GetTexUV() );
		}break;
	case CKTDGParticleSystem::ET_TEX1UV:		  
		{
			pEvent = new CKTDGParticleSystem::CParticleEvent_Tex1UV;	
			CKTDGParticleSystem::CParticleEvent_Tex1UV* pDestEvent = static_cast<CKTDGParticleSystem::CParticleEvent_Tex1UV*>(pEvent);
			CKTDGParticleSystem::CParticleEvent_Tex1UV* pSrcEvent = static_cast<CKTDGParticleSystem::CParticleEvent_Tex1UV*>(pEvent_);
			pDestEvent->SetTexUV( pSrcEvent->GetTexUV() );		
		}break;
	case CKTDGParticleSystem::ET_TEX2UV:		  
		{
			pEvent = new CKTDGParticleSystem::CParticleEvent_Tex2UV;		
			CKTDGParticleSystem::CParticleEvent_Tex2UV* pDestEvent = static_cast<CKTDGParticleSystem::CParticleEvent_Tex2UV*>(pEvent);
			CKTDGParticleSystem::CParticleEvent_Tex2UV* pSrcEvent = static_cast<CKTDGParticleSystem::CParticleEvent_Tex2UV*>(pEvent_);
			pDestEvent->SetTexUV( pSrcEvent->GetTexUV() );	
		}break;
	case CKTDGParticleSystem::ET_ROTATE:		  
		{
			pEvent = new CKTDGParticleSystem::CParticleEvent_Rotate;	
			CKTDGParticleSystem::CParticleEvent_Rotate* pDestEvent = static_cast<CKTDGParticleSystem::CParticleEvent_Rotate*>(pEvent);
			CKTDGParticleSystem::CParticleEvent_Rotate* pSrcEvent = static_cast<CKTDGParticleSystem::CParticleEvent_Rotate*>(pEvent_);
			pDestEvent->SetRotate( pSrcEvent->GetRotate() );		
		}break;
	case CKTDGParticleSystem::ET_DIRSPEED:		  
		{
			pEvent = new CKTDGParticleSystem::CParticleEvent_DirSpeed;			
			CKTDGParticleSystem::CParticleEvent_DirSpeed* pDestEvent = static_cast<CKTDGParticleSystem::CParticleEvent_DirSpeed*>(pEvent);
			CKTDGParticleSystem::CParticleEvent_DirSpeed* pSrcEvent = static_cast<CKTDGParticleSystem::CParticleEvent_DirSpeed*>(pEvent_);
			pDestEvent->SetValue( pSrcEvent->GetValue() );		
		}break;
	case CKTDGParticleSystem::ET_BLACKHOLE:		  
		{
			pEvent = new CKTDGParticleSystem::CParticleEvent_BlackHole;				
			CKTDGParticleSystem::CParticleEvent_BlackHole* pDestEvent = static_cast<CKTDGParticleSystem::CParticleEvent_BlackHole*>(pEvent);
			CKTDGParticleSystem::CParticleEvent_BlackHole* pSrcEvent = static_cast<CKTDGParticleSystem::CParticleEvent_BlackHole*>(pEvent_);
			pDestEvent->SetEventTimer( pSrcEvent->GetEventTimer() );			
		}break;
	case CKTDGParticleSystem::ET_CRASH:			  
		{
			pEvent = new CKTDGParticleSystem::CParticleEvent_Crash;				
			CKTDGParticleSystem::CParticleEvent_Crash* pDestEvent = static_cast<CKTDGParticleSystem::CParticleEvent_Crash*>(pEvent);
			CKTDGParticleSystem::CParticleEvent_Crash* pSrcEvent = static_cast<CKTDGParticleSystem::CParticleEvent_Crash*>(pEvent_);
			pDestEvent->SetCrash( pSrcEvent->GetCrash() );			
		}break;
	case CKTDGParticleSystem::ET_RESETCRASH:	  
		{
			pEvent = new CKTDGParticleSystem::CParticleEvent_ResetCrash;			
			CKTDGParticleSystem::CParticleEvent_ResetCrash* pDestEvent = static_cast<CKTDGParticleSystem::CParticleEvent_ResetCrash*>(pEvent);
			CKTDGParticleSystem::CParticleEvent_ResetCrash* pSrcEvent = static_cast<CKTDGParticleSystem::CParticleEvent_ResetCrash*>(pEvent_);
			pDestEvent->SetResetCrash( pSrcEvent->GetResetCrash() );					
		}break;
	case CKTDGParticleSystem::ET_TEXTURE:		  
		{
			pEvent = new CKTDGParticleSystem::CParticleEvent_Texture;				
			CKTDGParticleSystem::CParticleEvent_Texture* pDestEvent = static_cast<CKTDGParticleSystem::CParticleEvent_Texture*>(pEvent);
			CKTDGParticleSystem::CParticleEvent_Texture* pSrcEvent = static_cast<CKTDGParticleSystem::CParticleEvent_Texture*>(pEvent_);
			pDestEvent->SetTextureName( pSrcEvent->GetTextureName() );		
		}break;
	case CKTDGParticleSystem::ET_EVENTTIMER:	  
		{
			pEvent = new CKTDGParticleSystem::CParticleEvent_EventTimer;			
			CKTDGParticleSystem::CParticleEvent_EventTimer* pDestEvent = static_cast<CKTDGParticleSystem::CParticleEvent_EventTimer*>(pEvent);
			CKTDGParticleSystem::CParticleEvent_EventTimer* pSrcEvent = static_cast<CKTDGParticleSystem::CParticleEvent_EventTimer*>(pEvent_);
			pDestEvent->SetEventTimer( pSrcEvent->GetEventTimer() );				
		}break;
	case CKTDGParticleSystem::ET_STRETCH:		  
		{
			pEvent = new CKTDGParticleSystem::CParticleEvent_Stretch;				
			CKTDGParticleSystem::CParticleEvent_Stretch* pDestEvent = static_cast<CKTDGParticleSystem::CParticleEvent_Stretch*>(pEvent);
			CKTDGParticleSystem::CParticleEvent_Stretch* pSrcEvent = static_cast<CKTDGParticleSystem::CParticleEvent_Stretch*>(pEvent_);
			pDestEvent->SetValue( pSrcEvent->GetValue() );		
		}break;
	default:
		return NULL;
	}

	pEvent->SetFade( pEvent_->IsFade());
	pEvent->SetActualTime( pEvent_->GetActualTime() ) ;

	return pEvent;
}

CKTDGParticleSystem::CParticleEvent* CX2EffectToolParticleEditor::CreateEventList( CKTDGParticleSystem::EVENT_TYPE eEventType_ )
{
	CKTDGParticleSystem::CParticleEvent* pEvent = NULL;

	switch ( eEventType_ ) 
	{
	case CKTDGParticleSystem::ET_COLOR:			  pEvent = new CKTDGParticleSystem::CParticleEvent_Color;		 break;
	case CKTDGParticleSystem::ET_SIZE:			  pEvent = new CKTDGParticleSystem::CParticleEvent_Size;			break;
	case CKTDGParticleSystem::ET_SCALE:			  pEvent = new CKTDGParticleSystem::CParticleEvent_Scale;		break;
	case CKTDGParticleSystem::ET_VELOCITY:		  pEvent = new CKTDGParticleSystem::CParticleEvent_Velocity;		break;
	case CKTDGParticleSystem::ET_POSITION:		  pEvent = new CKTDGParticleSystem::CParticleEvent_Position;		break;
	case CKTDGParticleSystem::ET_TEX0UV:		  pEvent = new CKTDGParticleSystem::CParticleEvent_Tex0UV;		break;
	case CKTDGParticleSystem::ET_TEX1UV:		  pEvent = new CKTDGParticleSystem::CParticleEvent_Tex1UV;		break;
	case CKTDGParticleSystem::ET_TEX2UV:		  pEvent = new CKTDGParticleSystem::CParticleEvent_Tex2UV;		break;
	case CKTDGParticleSystem::ET_ROTATE:		  pEvent = new CKTDGParticleSystem::CParticleEvent_Rotate;		break;
	case CKTDGParticleSystem::ET_DIRSPEED:		  pEvent = new CKTDGParticleSystem::CParticleEvent_DirSpeed;		break;
	case CKTDGParticleSystem::ET_BLACKHOLE:		  pEvent = new CKTDGParticleSystem::CParticleEvent_BlackHole;		break;
	case CKTDGParticleSystem::ET_CRASH:			  pEvent = new CKTDGParticleSystem::CParticleEvent_Crash;		break;
	case CKTDGParticleSystem::ET_RESETCRASH:	  pEvent = new CKTDGParticleSystem::CParticleEvent_ResetCrash;		break;
	case CKTDGParticleSystem::ET_TEXTURE:		  pEvent = new CKTDGParticleSystem::CParticleEvent_Texture;		break;
	case CKTDGParticleSystem::ET_EVENTTIMER:	  pEvent = new CKTDGParticleSystem::CParticleEvent_EventTimer;		break;
	case CKTDGParticleSystem::ET_STRETCH:		  pEvent = new CKTDGParticleSystem::CParticleEvent_Stretch;		break;
	default:
		return NULL;
	}
	SetEventDataByUIValue( pEvent, true );

	return pEvent;
}

bool CX2EffectToolParticleEditor::SaveParticleData_OtherName()
{
	if( NULL == m_pEffectTool ) 
		return false;

	if( NULL == m_pParticleSequence )
		return false;

	CKTDGParticleSystem::CParticleEventList* pEventList = m_pParticleSequence->GetEventList();
	if( NULL == pEventList )
		return false;

	bool bAsSave = false;

	// 파티클명 체크
	wstring wsParticleName = GetString( PA_EDIT_PARTICLE_NAME );
	if( 0 != wsParticleName.compare( m_pParticleSequence->GetName() ) )
	{
		bAsSave = true;
	}

	// 파일명 체크
	wstring wsTempFileName = GetString( PA_EDIT_PARTICLE_FILE_NAME );
	if( 0 != wsTempFileName.compare( m_wstrParticleFileName.c_str() ) )
	{
		m_wstrParticleFileName = wsTempFileName;
		bAsSave = true;
	}

	if( true == bAsSave &&
		m_uiMAXParticleNum < m_pEffectTool->GetParticleNumByFileName( m_wstrParticleFileName.c_str()) )
	{
		WCHAR wszFileName[512] = L"";
		StringCchPrintf( wszFileName, ARRAY_SIZE(wszFileName), L"\"%s\"에는 더이상 추가 할 수 없습니다. ", m_wstrParticleFileName.c_str() );	

		MessageBox( g_pKTDXApp->GetHWND(), wszFileName, L"Error", MB_OK );
		SetWindowText(GetDlgItem( m_hDialogEditor, PA_EDIT_PARTICLE_NAME), m_pParticleSequence->GetName());
		SetWindowText(GetDlgItem( m_hDialogEditor, PA_EDIT_PARTICLE_FILE_NAME), m_wstrParticleFileName.c_str() );
		return false;
	}

	if( true == bAsSave )
	{
		if( PDT_ETC == m_eDialogType )
		{
			MessageBox( g_pKTDXApp->GetHWND(), L"기타 설정에서는\n다른이름으로 저장을\n지원하지 않습니다.", L"Error", MB_OK );
			SetWindowText(GetDlgItem( m_hDialogEditor, PA_EDIT_PARTICLE_NAME), m_pParticleSequence->GetName());
			SetWindowText(GetDlgItem( m_hDialogEditor, PA_EDIT_PARTICLE_FILE_NAME), m_wstrParticleFileName.c_str() );
			return false;
		}

		// 파티클 생성
		CKTDGParticleSystem::CParticleEventSequence* pTempEventSequence = 
			m_pEffectTool->CreateParticleEventSequence( wsParticleName.c_str(), m_wstrParticleFileName.c_str() );

		if( NULL != pTempEventSequence )
		{
				// 이벤트 리스트 복사
			CKTDGParticleSystem::CParticleEventList::iterator it = pEventList->begin();
			for( ; it != pEventList->end(); ++it )
			{
				CKTDGParticleSystem::CParticleEvent* pTempEvent = CreateEventList( *it );
				if( NULL != pTempEvent )
				{
					pTempEventSequence->GetEventList()->push_back( pTempEvent );
				}
			}
		}
		else
			return false;

		BOOST_FOREACH ( wstring Name, m_pParticleSequence->GetTraceSeqNameList() )
		{
			pTempEventSequence->AddTraceSeqName( Name.c_str() );
		}
		BOOST_FOREACH ( wstring Name, m_pParticleSequence->GetFinalSeqNameList() )
		{
			pTempEventSequence->AddFinalSeqName( Name.c_str() );
		}

		m_pParticleSequence = pTempEventSequence;
	}

	return bAsSave;
}

void CX2EffectToolParticleEditor::UpdateParticleData_ETC_Column1()
{
	if( NULL == m_pParticleSequence )
		return;

	SetCheckValue( PA_CHECK_CullingCheck, m_pParticleSequence->GetCullCheck() );
	SetCheckValue( PA_CHECK_TriggerWait, m_pParticleSequence->GetTriggerWait() );
	SetCheckValue( PA_CHECK_ResolutionConvert, m_pParticleSequence->GetResolutionConvert() );

	SetWindowText(GetDlgItem( m_hDialogEditor, PA_EDIT_MultiTexXetName ), m_pParticleSequence->GetMultiTexXETName());
	SetWindowText(GetDlgItem( m_hDialogEditor, PA_EDIT_AniXetName ), m_pParticleSequence->GetAniXETName());
	SetWindowText(GetDlgItem( m_hDialogEditor, PA_EDIT_AniName ), m_pParticleSequence->GetAniName());

	SetIntValue( PA_EDIT_ScrewValue, m_pParticleSequence->GetScrewValue() );
	SetIntValue( PA_EDIT_ScrewRotateSpeed, m_pParticleSequence->GetScrewRotateSpeed() );

	vector<int> vecControlID;
	vecControlID.push_back( PA_EDIT_SphericalEmitRadius_Min );
	vecControlID.push_back( PA_EDIT_SphericalEmitRadius_Max );
	SetEditControlByValue( &m_pParticleSequence->GetSphericalEmitRadius(), vecControlID);

	vecControlID.clear();
	vecControlID.push_back( PA_EDIT_SphericalEmitAzimuth_Min );
	vecControlID.push_back( PA_EDIT_SphericalEmitAzimuth_Max );
	SetEditControlByValue( &ConvertRadianToDegree(&m_pParticleSequence->GetSphericalEmitAzimuth()), vecControlID);

	vecControlID.clear();
	vecControlID.push_back( PA_EDIT_SphericalEmitPolar_Min );
	vecControlID.push_back( PA_EDIT_SphericalEmitPolar_Max );
	SetEditControlByValue( &ConvertRadianToDegree(&m_pParticleSequence->GetSphericalEmitPolar()), vecControlID);
}

void CX2EffectToolParticleEditor::UpdateParticleData_ETC_Column2()
{
	if( NULL == m_pParticleSequence )
		return;

	int nDel = SendDlgItemMessage( m_hDialogEditor , PA_LIST_TRACE_SEQ_NAME, LB_DELETESTRING, 0, 0);
	while(nDel > 0)
	{
		nDel = SendDlgItemMessage( m_hDialogEditor , PA_LIST_TRACE_SEQ_NAME, LB_DELETESTRING, 0, 0);
	}
	
	BOOST_FOREACH( wstring Name, m_pParticleSequence->GetTraceSeqNameList() )
	{
		SendDlgItemMessage( m_hDialogEditor , PA_LIST_TRACE_SEQ_NAME, LB_ADDSTRING, 0, (LPARAM)Name.c_str() );
	}

	nDel = SendDlgItemMessage( m_hDialogEditor , PA_LIST_FINAL_SEQ_NAME, LB_DELETESTRING, 0, 0);
	while(nDel > 0)
	{
		nDel = SendDlgItemMessage( m_hDialogEditor , PA_LIST_FINAL_SEQ_NAME, LB_DELETESTRING, 0, 0);
	}

	BOOST_FOREACH( wstring Name, m_pParticleSequence->GetFinalSeqNameList() )
	{
		SendDlgItemMessage( m_hDialogEditor , PA_LIST_FINAL_SEQ_NAME, LB_ADDSTRING, 0, (LPARAM)Name.c_str() );
	}
}

void CX2EffectToolParticleEditor::SaveParticleData_ETC_Column1()
{
	if( NULL == m_pParticleSequence )
		return;

	m_pParticleSequence->SetCullCheck( GetChecked(PA_CHECK_CullingCheck ));
	m_pParticleSequence->SetTriggerWait( GetChecked( PA_CHECK_TriggerWait ));
	m_pParticleSequence->SetResolutionConvert( GetChecked( PA_CHECK_ResolutionConvert ));

	m_pParticleSequence->SetMultiTexXETName( GetString(PA_EDIT_MultiTexXetName ).c_str());
	m_pParticleSequence->SetAniXETName( GetString(PA_EDIT_AniXetName ).c_str());
	m_pParticleSequence->SetAniName( GetString(PA_EDIT_AniName).c_str());

	m_pParticleSequence->SetScrewValue( GetIntValue(PA_EDIT_ScrewValue));
	m_pParticleSequence->SetScrewRotateSpeed( GetIntValue(PA_EDIT_ScrewRotateSpeed ));

	CMinMax<float> cFloatTemp;
	vector<int> vecControlID;
	vecControlID.push_back( PA_EDIT_SphericalEmitRadius_Min );
	vecControlID.push_back( PA_EDIT_SphericalEmitRadius_Max);
	SetValueByEditControl( &cFloatTemp, vecControlID );
	m_pParticleSequence->SetSphericalEmitRadius( cFloatTemp );

	vecControlID.clear();
	vecControlID.push_back( PA_EDIT_SphericalEmitAzimuth_Min );
	vecControlID.push_back( PA_EDIT_SphericalEmitAzimuth_Max);
	SetValueByEditControl( &cFloatTemp, vecControlID );
	m_pParticleSequence->SetSphericalEmitAzimuth( ConvertDegreeToRadian(&cFloatTemp) );

	vecControlID.clear();
	vecControlID.push_back( PA_EDIT_SphericalEmitPolar_Min );
	vecControlID.push_back( PA_EDIT_SphericalEmitPolar_Max);
	SetValueByEditControl( &cFloatTemp, vecControlID );
	m_pParticleSequence->SetSphericalEmitPolar( ConvertDegreeToRadian(&cFloatTemp) );
}


void CX2EffectToolParticleEditor::ON_PA_BUTTON_Final_Seq_ADD()
{
	if( NULL == m_pParticleSequence )
		return;

	wstring wsTemp = GetString( PA_EDIT_FINAL_SEQ_NAME );
	if( false == wsTemp.empty())
		m_pParticleSequence->AddFinalSeqName( wsTemp.c_str() );

	UpdateParticleData( m_pParticleSequence );
}
void CX2EffectToolParticleEditor::ON_PA_BUTTON_Final_Seq_DEL()
{
	if( NULL == m_pParticleSequence )
		return;

	int iIndex = SendDlgItemMessage( m_hDialogEditor, PA_LIST_FINAL_SEQ_NAME, LB_GETCURSEL, 0, 0);
	if( iIndex >= 0 && false == m_pParticleSequence->GetFinalSeqNameList().empty() )
	{
		m_pParticleSequence->GetFinalSeqNameList().erase( m_pParticleSequence->GetFinalSeqNameList().begin() + iIndex );

		SendDlgItemMessage( m_hDialogEditor, PA_LIST_FINAL_SEQ_NAME, LB_SETCURSEL, -1, 0);
		UpdateParticleData( m_pParticleSequence );
	}
}
void CX2EffectToolParticleEditor::ON_PA_BUTTON_Trace_Seq_ADD()
{
	if( NULL == m_pParticleSequence )
		return;

	wstring wsTemp = GetString( PA_EDIT_TRACE_SEQ_NAME);
	if( false == wsTemp.empty())
		m_pParticleSequence->AddTraceSeqName( wsTemp.c_str() );

	UpdateParticleData( m_pParticleSequence );
}
void CX2EffectToolParticleEditor::ON_PA_BUTTON_Trace_Seq_DEL()
{
	if( NULL == m_pParticleSequence )
		return;

	int iIndex = SendDlgItemMessage( m_hDialogEditor, PA_LIST_TRACE_SEQ_NAME, LB_GETCURSEL, 0, 0);
	if( iIndex >= 0 && false == m_pParticleSequence->GetFinalSeqNameList().empty() )
	{
		m_pParticleSequence->GetTraceSeqNameList().erase( m_pParticleSequence->GetTraceSeqNameList().begin() + iIndex );

		SendDlgItemMessage( m_hDialogEditor, PA_LIST_FINAL_SEQ_NAME, LB_SETCURSEL, -1, 0);
		UpdateParticleData( m_pParticleSequence );
	}
}