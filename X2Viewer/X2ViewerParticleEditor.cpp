#include "dxstdafx.h"
#include ".\x2viewerparticleeditor.h"
#include <fstream>

struct ENUMSTRING
{
	WCHAR	name[25];
	int		mode;
};

// 여기 있는 값들은 입력형식으로 VT_COMBO_CUSTOM을 사용하는 값들을 위한 것이다.
// VALUE NAME, 실제 VALUE 순으로 작성.
// 사용은 GetCustomComboBox, SetCustomComboBox
const int PARTICLETYPES = 4;
const ENUMSTRING m_ParticleTypes[13] = 
{
	{ L"PT_3D_PLANE",			CKTDGParticleSystem::PT_3D_PLANE			},
	{ L"PT_2D_PLANE",			CKTDGParticleSystem::PT_2D_PLANE			},
	{ L"PT_MESH",				CKTDGParticleSystem::PT_MESH			},
	{ L"PT_SKINMESH",			CKTDGParticleSystem::PT_SKINMESH			},
};

const int MESHRENDERTYPES = 4;
const ENUMSTRING m_MeshRenderTypes[4] = 
{
	{ L"RT_REAL_COLOR",				CKTDGXRenderer::RT_REAL_COLOR			},
	{ L"RT_CARTOON",					CKTDGXRenderer::RT_CARTOON			},
	{ L"RT_CARTOON_BLACK_EDGE",		CKTDGXRenderer::RT_CARTOON_BLACK_EDGE			},
	{ L"RT_CARTOON_COLOR_EDGE",		CKTDGXRenderer::RT_CARTOON_COLOR_EDGE			},
};

const int NUMBLENDINGMODES = 13;
const ENUMSTRING m_BlendingModes[13] = 
{
	{ L"D3DBLEND_ZERO",			D3DBLEND_ZERO			},
	{ L"D3DBLEND_ONE",			D3DBLEND_ONE			},
	{ L"D3DBLEND_SRCCOLOR",		D3DBLEND_SRCCOLOR		},
	{ L"D3DBLEND_INVSRCCOLOR",	D3DBLEND_INVSRCCOLOR	},
	{ L"D3DBLEND_SRCALPHA",		D3DBLEND_SRCALPHA		},
	{ L"D3DBLEND_INVSRCALPHA",	D3DBLEND_INVSRCALPHA	},
	{ L"D3DBLEND_DESTALPHA",		D3DBLEND_DESTALPHA		},
	{ L"D3DBLEND_INVDESTALPHA",	D3DBLEND_INVDESTALPHA	},
	{ L"D3DBLEND_DESTCOLOR",		D3DBLEND_DESTCOLOR		},
	{ L"D3DBLEND_INVDESTCOLOR",	D3DBLEND_INVDESTCOLOR	},
	{ L"D3DBLEND_SRCALPHASAT",	D3DBLEND_SRCALPHASAT	},
	{ L"D3DBLEND_BOTHSRCALPHA",	D3DBLEND_BOTHSRCALPHA	},
	{ L"D3DBLEND_BOTHINVSRCALPHA",	D3DBLEND_BOTHINVSRCALPHA	}
};

const int NUMCULLMODES = 4;
const ENUMSTRING m_CullModes[4] = 
{
	{ L"D3DCULL_NONE",			D3DCULL_NONE			},
	{ L"D3DCULL_CW",				D3DCULL_CW				},
	{ L"D3DCULL_CCW",			D3DCULL_CCW				},
	{ L"D3DCULL_FORCE_DWORD",	D3DCULL_FORCE_DWORD		}
};

const int BILLBOARDMODES = 5;
const ENUMSTRING m_BillBoardMode[5] = 
{
	{ L"BT_NONE",			CKTDGMatrix::BT_NONE			},
	{ L"BT_ALL",				CKTDGMatrix::BT_ALL				},
	{ L"BT_X",				CKTDGMatrix::BT_X				},
	{ L"BT_Y",				CKTDGMatrix::BT_Y		},
	{ L"BT_Z",				CKTDGMatrix::BT_Z		}
};

CX2ViewerParticleEditor::CX2ViewerParticleEditor()
{
	//					 EMITTER_PROPERTIES				EMITTER_PROPERTIES	툴 메뉴에 표시될 이름		속성값 type		random 허용여부
	m_aEmitterProperties[EP_PARTICLETYPE].Init(			EP_PARTICLETYPE,	L"Particle Type",			VT_COMBO_CUSTOM, false );
	m_aEmitterProperties[EP_MESHRENDERTYPE].Init(		EP_MESHRENDERTYPE,	L"Mesh Rendering Type",		VT_COMBO_CUSTOM, false );
	m_aEmitterProperties[EP_LAYER].Init(				EP_LAYER,			L"Layer Number",			VT_INT, false );
	m_aEmitterProperties[EP_SRCBLENDMODE].Init(			EP_SRCBLENDMODE,	L"Source Blend Mode",		VT_COMBO_CUSTOM, false );
	m_aEmitterProperties[EP_DSTBLENDMODE].Init(			EP_DSTBLENDMODE,	L"Destination Blend Mode",	VT_COMBO_CUSTOM, false );
	m_aEmitterProperties[EP_CULLMODE].Init(				EP_CULLMODE,		L"Cull Mode",				VT_COMBO_CUSTOM, false );
	m_aEmitterProperties[EP_NUMPARTICLES].Init(			EP_NUMPARTICLES,	L"Num. of Particles",		VT_INT, false );
	m_aEmitterProperties[EP_GRAVITY].Init(				EP_GRAVITY,			L"Gravity Position",		VT_XYZ, true );
	m_aEmitterProperties[EP_LIFETIME].Init(				EP_LIFETIME,		L"Life Time",				VT_FLOAT, true );
	m_aEmitterProperties[EP_TRIGGERTIME].Init(			EP_TRIGGERTIME,		L"Trigger Time",			VT_FLOAT, false );
	m_aEmitterProperties[EP_TRIGGERCOUNT].Init(			EP_TRIGGERCOUNT,	L"Trigger Count",			VT_INT, false );
	m_aEmitterProperties[EP_EMITRATE].Init(				EP_EMITRATE,		L"Emit Rate",				VT_FLOAT, true );
	m_aEmitterProperties[EP_EMITRADIUS].Init(			EP_EMITRADIUS,		L"Emit Radius",				VT_XYZ, true );
	m_aEmitterProperties[EP_EMITRADIUSMIN] .Init(		EP_EMITRADIUSMIN,	L"Min. Emit Radius",		VT_XYZ, true );
	m_aEmitterProperties[EP_ADDROTATEREL] .Init(		EP_ADDROTATEREL,	L"AddRotateRel",			VT_XYZ, false );
	m_aEmitterProperties[EP_TRACEPARTICLELIST].Init(	EP_TRACEPARTICLELIST, L"Trace Particle",		VT_STRING, false );
	m_aEmitterProperties[EP_FINALPARTICLELIST].Init(	EP_FINALPARTICLELIST, L"Final Particle",		VT_STRING, false );
	m_aEmitterProperties[EP_BILLBOARDTYPE].Init(		EP_BILLBOARDTYPE,	L"BillBoard Type",			VT_COMBO_CUSTOM, false );
	m_aEmitterProperties[EP_MESHNAME].Init(				EP_MESHNAME,		L"Mesh fileName(.x, .y)",	VT_STRING, false );
	m_aEmitterProperties[EP_CHANGETEXNAME].Init(		EP_CHANGETEXNAME,	L"Change Tex filename",		VT_STRING, false );
	m_aEmitterProperties[EP_MULTITEXXET].Init(			EP_MULTITEXXET,		L"MultiTex Tex filename",	VT_STRING, false );
	m_aEmitterProperties[EP_ANIXET].Init(				EP_ANIXET,			L"AniXet Filename",			VT_STRING, false );
	m_aEmitterProperties[EP_ANINAME].Init(				EP_ANINAME,			L"Anim Name of Skinmesh",	VT_STRING, false );
	m_aEmitterProperties[EP_LANDPOS].Init(				EP_LANDPOS,			L"Land Position",			VT_FLOAT, false );
	m_aEmitterProperties[EP_USELAND].Init(				EP_USELAND,			L"UseLand?",				VT_BOOL, false );
	m_aEmitterProperties[EP_BLACKHOLEPOS].Init(			EP_BLACKHOLEPOS,	L"BlackHole Position",		VT_XYZ, false );
	m_aEmitterProperties[EP_LIGHTPOS].Init(				EP_LIGHTPOS,		L"Light Position",			VT_XYZ, false );
	m_aEmitterProperties[EP_RESOLUTIONCONVERT].Init(	EP_RESOLUTIONCONVERT, L"Resolution Convert?",	VT_BOOL, false );
	m_aEmitterProperties[EP_TRACE].Init(				EP_TRACE,			L"Trace?",					VT_BOOL, false );
	m_aEmitterProperties[EP_ZENABLE].Init(				EP_ZENABLE,			L"ZEnable?",				VT_BOOL, false );
	m_aEmitterProperties[EP_ZWRITEENABLE].Init(			EP_ZWRITEENABLE,	L"ZWriteEnable?",			VT_BOOL, false );
	m_aEmitterProperties[EP_FORCELAYER].Init(			EP_FORCELAYER,		L"ForceLayer?",				VT_BOOL, false );
	m_aEmitterProperties[EP_DRAWCOUNT].Init(			EP_DRAWCOUNT,		L"Draw Count",				VT_INT, false );
	m_aEmitterProperties[EP_SCREW_VALUE].Init(			EP_SCREW_VALUE,		L"Screw Value",				VT_INT, false );
	m_aEmitterProperties[EP_SCREW_ROTATESPEED].Init(	EP_SCREW_ROTATESPEED, L"Screw Rotate Speed",	VT_INT, false );
	m_aEmitterProperties[EP_SPHERICAL_EMITRADIUS].Init( EP_SPHERICAL_EMITRADIUS, L"Spherical EmitRadius", VT_FLOAT, true );
	m_aEmitterProperties[EP_SPHERICAL_EMITAZIMUTH].Init( EP_SPHERICAL_EMITAZIMUTH, L"Spherical Emit Azimuth", VT_FLOAT, true );
	m_aEmitterProperties[EP_SPHERICAL_EMITPOLAR].Init(	EP_SPHERICAL_EMITPOLAR, L"Spherical Emit Polar", VT_FLOAT, true );
	m_aEmitterProperties[EP_SPHERICAL_EMITROTATION].Init( EP_SPHERICAL_EMITROTATION, L"Spherical Emit Rotation", VT_XYZ, false );
	
	//				 CKTDGParticleSystem::EVENT_TYPE			CKTDGParticleSystem::EVENT_TYPE		메뉴표시명		속성type	random 허용여부
	m_aEventProperties[CKTDGParticleSystem::ET_COLOR].Init(		CKTDGParticleSystem::ET_COLOR,		L"Color",		VT_RGBA,	true );
	m_aEventProperties[CKTDGParticleSystem::ET_SIZE].Init(		CKTDGParticleSystem::ET_SIZE,		L"Size",		VT_XYZ,		true );
	m_aEventProperties[CKTDGParticleSystem::ET_SCALE].Init(		CKTDGParticleSystem::ET_SCALE,		L"Scale",		VT_FLOAT,	true );
	m_aEventProperties[CKTDGParticleSystem::ET_VELOCITY].Init(	CKTDGParticleSystem::ET_VELOCITY,	L"Velocity",	VT_XYZ,		true );
	m_aEventProperties[CKTDGParticleSystem::ET_POSITION].Init(	CKTDGParticleSystem::ET_POSITION,	L"Position",	VT_XYZ,		true );
	m_aEventProperties[CKTDGParticleSystem::ET_TEX0UV].Init(	CKTDGParticleSystem::ET_TEX0UV,		L"TEX0UV",		VT_XY,		true );
	m_aEventProperties[CKTDGParticleSystem::ET_TEX1UV].Init(	CKTDGParticleSystem::ET_TEX1UV,		L"TEX1UV",		VT_XY,		true );
	m_aEventProperties[CKTDGParticleSystem::ET_TEX2UV].Init(	CKTDGParticleSystem::ET_TEX2UV,		L"TEX2UV",		VT_XY,		true );
	m_aEventProperties[CKTDGParticleSystem::ET_ROTATE].Init(	CKTDGParticleSystem::ET_ROTATE,		L"Rotate",		VT_XYZ,		true );
	m_aEventProperties[CKTDGParticleSystem::ET_DIRSPEED].Init(	CKTDGParticleSystem::ET_DIRSPEED,	L"DirSpeed",	VT_FLOAT,	true );
	m_aEventProperties[CKTDGParticleSystem::ET_BLACKHOLE].Init(	CKTDGParticleSystem::ET_BLACKHOLE,	L"BlackHoleTime", VT_FLOAT, true );
	m_aEventProperties[CKTDGParticleSystem::ET_CRASH].Init(		CKTDGParticleSystem::ET_CRASH,		L"Crash",		VT_XYZ,		true );
	m_aEventProperties[CKTDGParticleSystem::ET_RESETCRASH].Init(CKTDGParticleSystem::ET_RESETCRASH,	L"ResetCrash",	VT_FLOAT,	true );
	m_aEventProperties[CKTDGParticleSystem::ET_TEXTURE].Init(	CKTDGParticleSystem::ET_TEXTURE,		L"Texture",		VT_STRING,	false );
	m_aEventProperties[CKTDGParticleSystem::ET_EVENTTIMER].Init(CKTDGParticleSystem::ET_EVENTTIMER,	L"EventTimer",	VT_FLOAT,	true );
#ifdef PARTICLE_STRETCH_TEST
	m_aEventProperties[CKTDGParticleSystem::ET_STRETCH].Init(	CKTDGParticleSystem::ET_STRETCH,		L"Stretch",		VT_FLOAT, true );
#endif PARTICLE_STRETCH_TEST
	
}
CX2ViewerParticleEditor::~CX2ViewerParticleEditor(void)
{

}


CMinMax<float> CX2ViewerParticleEditor::ParseCMinMaxFloat( wstring str )
{
	MakeUpperCase(str);
	CMinMax<float> retval;
	retval.m_Min = -1;
	retval.m_Max = -1;
	if( str.find( L"RANDOM" ) != wstring::npos )
	{
		int poslparen = (int) str.find( L'(' );
		int poscomma = (int) str.find( L',' );
		int posrparen = (int) str.find( L')' );
		
		ASSERT( poslparen < poscomma );
		ASSERT( poscomma < posrparen );

		if( poslparen < poscomma && poscomma < posrparen )
		{
			wstring strFirst = str.substr( poslparen+1, poscomma - poslparen );
			wstring strSecond = str.substr( poscomma+1, posrparen - poscomma );
			retval.m_Min = (float)_wtof( strFirst.c_str() );
			retval.m_Max = (float)_wtof( strSecond.c_str() );
		}
		else
		{
			WARNINGMSG( L"잘못된 입력 형식 : random( a, b ) 형식을 사용해 주세요" );
		}
	}
	else
	{
		retval.m_Max = (float)_wtof( str.c_str() );
		retval.m_Min = retval.m_Max;
	}
	
	return retval;

}

CMinMax<D3DXVECTOR2> CX2ViewerParticleEditor::ParseCMinMaxD3DVECTOR2( wstring strX, wstring strY )
{
	CMinMax<D3DXVECTOR2> retval;
	CMinMax<float> tempX;
	CMinMax<float> tempY;	
	
	tempX = ParseCMinMaxFloat( strX );
	tempY = ParseCMinMaxFloat( strY );
	
	retval.m_Max.x = tempX.m_Max;
	retval.m_Min.x = tempX.m_Min;

	retval.m_Max.y = tempY.m_Max;
	retval.m_Min.y = tempY.m_Min;

	return retval;

}

CMinMax<D3DXVECTOR3> CX2ViewerParticleEditor::ParseCMinMaxD3DVECTOR3( wstring strX, wstring strY, wstring strZ )
{	
	CMinMax<D3DXVECTOR3> retval;
	CMinMax<float> tempX;
	CMinMax<float> tempY;	
	CMinMax<float> tempZ;

	tempX = ParseCMinMaxFloat( strX );
	tempY = ParseCMinMaxFloat( strY );
	tempZ = ParseCMinMaxFloat( strZ );
	
	retval.m_Max.x = tempX.m_Max;
	retval.m_Min.x = tempX.m_Min;

	retval.m_Max.y = tempY.m_Max;
	retval.m_Min.y = tempY.m_Min;

	retval.m_Max.z = tempZ.m_Max;
	retval.m_Min.z = tempZ.m_Min;

	return retval;

}

bool CX2ViewerParticleEditor::ExportParticleScript( CKTDGParticleSystem* pSystem, const WCHAR* pwszFileFullPath )
{

	// file path와 name을 분리
	WCHAR drive[_MAX_DRIVE];
	WCHAR dir[_MAX_DIR];
	WCHAR fname[_MAX_FNAME];
	WCHAR ext[_MAX_EXT];
	_wsplitpath( pwszFileFullPath, drive, dir, fname, ext);

	string strFileName;
	ConvertWCHARToChar( strFileName, fname );
	strFileName += ".txt";

	std::wfstream fstrm;
	fstrm.open( strFileName.c_str(), std::fstream::out | std::fstream::trunc );

	fstrm << L"particlesystem \"GameMajor\" 1.00" << std::endl;
	fstrm << L"{" << std::endl;

	const map<wstring, CKTDGParticleSystem::CParticleEventSequence*> refTemplet = pSystem->GetTempletSequences();

	for( map<wstring, CKTDGParticleSystem::CParticleEventSequence*>::const_iterator it = refTemplet.begin(); it != refTemplet.end(); ++it )
	{
		ASSERT( it->second != NULL );
		if( it->second == NULL )
			continue;

		fstrm << L"\teventsequence \"" << it->first << L"\"" << std::endl;
		fstrm << L"\t{" << std::endl;
		fstrm << MakeParticleEventSequenceString( it->second );
		fstrm << L"\t}" << std::endl;        
	}

	fstrm << L"}" << std::endl;
	fstrm.close();
	return true;
}



wstring CX2ViewerParticleEditor::MakeParticleEventSequenceString( CKTDGParticleSystem::CParticleEventSequence* pSeq )
{
	if( pSeq == NULL ) 
		return L"";

	if( pSeq->GetTemplet() != NULL )
	{
		return MakeParticleEventSequenceString( pSeq->GetTemplet() );
	}
	wstringstream wstrmRet;
	CMinMax<float> valfloat;
	CMinMax<D3DXVECTOR3> valV3;

	wstrmRet << L"\t\t" << L"particleType" << L"\t\t= ";
	switch( pSeq->GetParticleType() )
	{
	case CKTDGParticleSystem::PT_3D_PLANE:
		{
			wstrmRet << L"PT_3D_PLANE" << std::endl;
		} break;
	case CKTDGParticleSystem::PT_2D_PLANE:
		{
			wstrmRet << L"PT_2D_PLANE" << std::endl;
		} break;
	case CKTDGParticleSystem::PT_MESH:
		{
			wstrmRet << L"PT_MESH" << std::endl;
		} break;
	case CKTDGParticleSystem::PT_SKINMESH:
		{
			wstrmRet << L"PT_SKINMESH" << std::endl;
		} break;
	default:
		{
			ASSERT( !"Unexpected value" );
			return L"";
		}
	}

	wstrmRet << L"\t\t" << L"meshRenderType" << L"\t\t= ";
	switch( pSeq->GetMeshRenderType() )
	{
	case CKTDGXRenderer::RT_REAL_COLOR:
		{
			wstrmRet << L"RT_REAL_COLOR" << std::endl;
		} break;
	case CKTDGXRenderer::RT_CARTOON:
		{
			wstrmRet << L"RT_CARTOON" << std::endl;
		} break;
	case CKTDGXRenderer::RT_CARTOON_BLACK_EDGE:
		{
			wstrmRet << L"RT_CARTOON_BLACK_EDGE" << std::endl;
		} break;
	case CKTDGXRenderer::RT_CARTOON_COLOR_EDGE:
		{
			wstrmRet << L"RT_CARTOON_COLOR_EDGE" << std::endl;
		} break;
	}

	// Layer
	wstrmRet << L"\t\t" << L"Layer" << L"\t\t= ";
	wstrmRet << pSeq->GetLayer() << std::endl;

	if( pSeq->GetForceLayer() )
	{
		wstrmRet << L"\t\tforcelayer\t\t= true" << std::endl;
	}

	// Src / Dest Blend Mode
	wstrmRet << L"\t\t" << L"sourceblendmode" << L"\t\t= ";
	switch( pSeq->GetSrcBlendMode() )
	{
	case D3DBLEND_ZERO:
		{
			wstrmRet << L"D3DBLEND_ZERO" << std::endl;
		} break;
	case D3DBLEND_ONE:
		{
			wstrmRet << L"D3DBLEND_ONE" << std::endl;
		} break;
	case D3DBLEND_SRCCOLOR:
		{
			wstrmRet << L"D3DBLEND_SRCCOLOR" << std::endl;
		} break;
	case D3DBLEND_INVSRCCOLOR:
		{
			wstrmRet << L"D3DBLEND_INVSRCCOLOR" << std::endl;
		} break;
	case D3DBLEND_SRCALPHA:
		{
			wstrmRet << L"D3DBLEND_SRCALPHA" << std::endl;
		} break;
	case D3DBLEND_INVSRCALPHA:
		{
			wstrmRet << L"D3DBLEND_INVSRCALPHA" << std::endl;
		} break;
	case D3DBLEND_DESTALPHA:
		{
			wstrmRet << L"D3DBLEND_DESTALPHA" << std::endl;
		} break;
	case D3DBLEND_INVDESTALPHA:
		{
			wstrmRet << L"D3DBLEND_INVDESTALPHA" << std::endl;
		} break;
	case D3DBLEND_DESTCOLOR:
		{
			wstrmRet << L"D3DBLEND_DESTCOLOR" << std::endl;
		} break;
	case D3DBLEND_INVDESTCOLOR:
		{
			wstrmRet << L"D3DBLEND_INVDESTCOLOR" << std::endl;
		} break;
	case D3DBLEND_SRCALPHASAT:
		{
			wstrmRet << L"D3DBLEND_SRCALPHASAT" << std::endl;
		} break;
	case D3DBLEND_BOTHSRCALPHA:
		{
			wstrmRet << L"D3DBLEND_BOTHSRCALPHA" << std::endl;
		} break;
	case D3DBLEND_BOTHINVSRCALPHA:
		{
			wstrmRet << L"D3DBLEND_BOTHINVSRCALPHA" << std::endl;
		} break;
	}
	wstrmRet << L"\t\t" << L"destblendmode" << L"\t\t= ";
	switch( pSeq->GetDestBlendMode() )
	{
	case D3DBLEND_ZERO:
		{
			wstrmRet << L"D3DBLEND_ZERO" << std::endl;
		} break;
	case D3DBLEND_ONE:
		{
			wstrmRet << L"D3DBLEND_ONE" << std::endl;
		} break;
	case D3DBLEND_SRCCOLOR:
		{
			wstrmRet << L"D3DBLEND_SRCCOLOR" << std::endl;
		} break;
	case D3DBLEND_INVSRCCOLOR:
		{
			wstrmRet << L"D3DBLEND_INVSRCCOLOR" << std::endl;
		} break;
	case D3DBLEND_SRCALPHA:
		{
			wstrmRet << L"D3DBLEND_SRCALPHA" << std::endl;
		} break;
	case D3DBLEND_INVSRCALPHA:
		{
			wstrmRet << L"D3DBLEND_INVSRCALPHA" << std::endl;
		} break;
	case D3DBLEND_DESTALPHA:
		{
			wstrmRet << L"D3DBLEND_DESTALPHA" << std::endl;
		} break;
	case D3DBLEND_INVDESTALPHA:
		{
			wstrmRet << L"D3DBLEND_INVDESTALPHA" << std::endl;
		} break;
	case D3DBLEND_DESTCOLOR:
		{
			wstrmRet << L"D3DBLEND_DESTCOLOR" << std::endl;
		} break;
	case D3DBLEND_INVDESTCOLOR:
		{
			wstrmRet << L"D3DBLEND_INVDESTCOLOR" << std::endl;
		} break;
	case D3DBLEND_SRCALPHASAT:
		{
			wstrmRet << L"D3DBLEND_SRCALPHASAT" << std::endl;
		} break;
	case D3DBLEND_BOTHSRCALPHA:
		{
			wstrmRet << L"D3DBLEND_BOTHSRCALPHA" << std::endl;
		} break;
	case D3DBLEND_BOTHINVSRCALPHA:
		{
			wstrmRet << L"D3DBLEND_BOTHINVSRCALPHA" << std::endl;
		} break;
	}

	// cull mode
	if( pSeq->GetCullMode() != D3DCULL_NONE )
	{
		wstrmRet << L"\t\t" << L"cullmode" << L"\t\t= ";
		switch( pSeq->GetCullMode() )
		{
		case D3DCULL_NONE:
			{
				wstrmRet << L"D3DCULL_NONE" << std::endl;
			} break;
		case D3DCULL_CW:
			{
				wstrmRet << L"D3DCULL_CW" << std::endl;
			} break;
		case D3DCULL_CCW:
			{
				wstrmRet << L"D3DCULL_CCW" << std::endl;
			} break;
		case D3DCULL_FORCE_DWORD:
			{
				wstrmRet << L"D3DCULL_FORCE_DWORD" << std::endl;
			} break;
		}
	}

	wstrmRet << L"\t\t" << L"numparticles" << L"\t\t= ";
	wstrmRet << pSeq->GetMaxParticleNum() << std::endl;

	wstrmRet << L"\t\t" << L"gravity" << L"\t\t= ";
	wstrmRet << GetMinMaxString( pSeq->GetGravity() ) << std::endl;

	wstrmRet << L"\t\t" << L"lifetime" << L"\t\t= ";
	wstrmRet << GetMinMaxString( pSeq->GetLifeTime() ) << std::endl;

	// Emitrate
	valfloat = pSeq->GetEmitRate();
	if( valfloat.m_Max != 0.0f || valfloat.m_Min != 0.0f )
	{
		wstrmRet << L"\t\t" << L"emitrate" << L"\t\t= ";
		wstrmRet << GetMinMaxString( valfloat ) << std::endl;
	}

	// EmitRadius
	valV3 = pSeq->GetEmitRadius();
	if( valV3.m_Max != D3DXVECTOR3(0.0f, 0.0f, 0.0f) || valV3.m_Min != D3DXVECTOR3(0.0f, 0.0f, 0.0f) )
	{
		wstrmRet << L"\t\t" << L"emitradius" << L"\t\t= ";
		wstrmRet << GetMinMaxString( valV3 ) << std::endl;	
	}

	// EmitRadiusMin
	valV3 = pSeq->GetEmitRadiusMin();
	if( valV3.m_Max != D3DXVECTOR3(0.0f, 0.0f, 0.0f) || valV3.m_Min != D3DXVECTOR3(0.0f, 0.0f, 0.0f) )
	{
		wstrmRet << L"\t\t" << L"emitradiusmin" << L"\t\t= ";
		wstrmRet << GetMinMaxString( valV3 ) << std::endl;	
	}

	if( pSeq->GetTriggerCount() != -1 )
	{
		wstrmRet << L"\t\t" << L"triggercount" << L"\t\t= ";
		wstrmRet << pSeq->GetTriggerCount() << std::endl;	
	}

	if( pSeq->GetTriggerTime() != -1 )
	{
		wstrmRet << L"\t\t" << L"TriggerTime" << L"\t\t= ";
		wstrmRet << pSeq->GetTriggerTime() << std::endl;	
	}

	// AddRotateRel
	valV3 = pSeq->GetAddRotateRel();
	if( valV3.m_Max != D3DXVECTOR3(0.0f, 0.0f, 0.0f) || valV3.m_Min != D3DXVECTOR3(0.0f, 0.0f, 0.0f) )
	{
		wstrmRet << L"\t\t" << L"AddRotateRel" << L"\t\t= ";
		wstrmRet << GetMinMaxString( valV3 ) << std::endl;	
	}

	// Trace Particle List
	for( vector<wstring>::iterator it = pSeq->GetTraceSeqNameList().begin(); it < pSeq->GetTraceSeqNameList().end(); ++it )
	{
		wstrmRet << L"\t\t" << L"TRACEPARTICLELIST" << L"\t\t= " << *it << std::endl;
	}
	vector<wstring> vec = pSeq->GetFinalSeqNameList();
	for( vector<wstring>::iterator it = pSeq->GetTraceSeqNameList().begin(); it < pSeq->GetTraceSeqNameList().end(); ++it )
	{
		wstrmRet << L"\t\t" << L"FINALPARTICLELIST" << L"\t\t= " << *it << std::endl;
	}

	wstrmRet << L"\t\t" << L"BillboardType" << L"\t\t= ";
	switch( pSeq->GetBillBoardType() )
	{
	case CKTDGMatrix::BT_NONE:
		{
			wstrmRet << L"BT_NONE" << std::endl;
		} break;
	case CKTDGMatrix::BT_ALL:
		{
			wstrmRet << L"BT_ALL" << std::endl;
		} break;
	case CKTDGMatrix::BT_X:
		{
			wstrmRet << L"BT_X" << std::endl;
		} break;
	case CKTDGMatrix::BT_Y:
		{
			wstrmRet << L"BT_Y" << std::endl;
		} break;
	case CKTDGMatrix::BT_Z:
		{
			wstrmRet << L"BT_Z" << std::endl;
		} break;
	}

	wstring meshname(pSeq->GetMeshName());
	if( meshname.length() > 0 )
	{
		wstrmRet << L"\t\t" << L"Mesh" << L"\t\t= \"" << meshname << L"\"" << std::endl;
	}

	wstring CTexName(pSeq->GetChangeTexXETName());
	if( CTexName.length() > 0 )
	{
		wstrmRet << L"\t\t" << L"CHANGETEXXET" << L"\t\t= \"" << CTexName << L"\"" << std::endl;
	}
	wstring MTexName(pSeq->GetMultiTexXETName());
	if( MTexName.length() > 0 )
	{
		wstrmRet << L"\t\t" << L"MULTITEXXET" << L"\t\t= \"" << MTexName << L"\"" << std::endl;
	}
	wstring AXetName(pSeq->GetAniXETName());
	if( AXetName.length() > 0 )
	{
		wstrmRet << L"\t\t" << L"ANIXET" << L"\t\t= \"" << AXetName << L"\"" << std::endl;
	}

	wstring AniName(pSeq->GetAniName());
	if( AniName.length() > 0 )
	{
		wstrmRet << L"\t\t" << L"ANINAME" << L"\t\t= \"" << AniName << L"\"" << std::endl;
	}

	if( pSeq->GetLandPosition() != 0.0f )
	{
		wstrmRet << L"\t\t" << L"landpos" << L"\t\t= ";
		wstrmRet << pSeq->GetLandPosition() << std::endl;
	}

	if( pSeq->GetUseLand() )
	{
		wstrmRet << L"\t\t" << L"UseLand" << L"\t\t= true" << std::endl;		
	}

	if( pSeq->GetBlackHolePosition() != D3DXVECTOR3(0.0f, 0.0f, 0.0f) )
	{
		wstrmRet << L"\t\t" << L"blackholepos" << L"\t\t= ";
		wstrmRet << L"XYZ( " << pSeq->GetBlackHolePosition().x << L"," << pSeq->GetBlackHolePosition().y << L"," << pSeq->GetBlackHolePosition().z << L")" << std::endl;		
	}

	if( pSeq->GetLightPos() != D3DXVECTOR3(0.0f, 0.0f, 0.0f) )
	{
		wstrmRet << L"\t\t" << L"LightPos" << L"\t\t= ";
		wstrmRet << L"XYZ( " << pSeq->GetLightPos().x << L"," << pSeq->GetBlackHolePosition().y << L"," << pSeq->GetBlackHolePosition().z << L")" << std::endl;		
	}

	if( pSeq->GetResolutionConvert() )
	{
		wstrmRet << L"\t\t" << L"RESOLUTIONCONVERT" << L"\t\t= true" << std::endl;		
	}
	if( pSeq->GetTrace() )
	{
		wstrmRet << L"\t\t" << L"Trace" << L"\t\t= true" << std::endl;		
	}

	wstrmRet << L"\t\t" << L"ZEnable" << L"\t\t= ";
	if( pSeq->GetZEnable() )
	{
		wstrmRet << L"true" << std::endl;;
	}
	else
	{
		wstrmRet << L"false" << std::endl;;
	}
	wstrmRet << L"\t\t" << L"ZWriteEnable" << L"\t\t= ";
	if( pSeq->GetZWriteEnable() )
	{
		wstrmRet << L"true" << std::endl;;
	}
	else
	{
		wstrmRet << L"false" << std::endl;;
	}

	wstrmRet << L"\t\t" << L"DrawCount" << L"\t\t= ";
	wstrmRet << pSeq->GetDrawCount() << std::endl;

	// Screw
	if( pSeq->GetScrewValue() != -1 )
	{
		wstrmRet << L"\t\t" << L"SCREWVALUE" << L"\t\t= ";
		wstrmRet << pSeq->GetScrewValue() << std::endl;
	}
	if( pSeq->GetScrewRotateSpeed() != 10 )
	{
		wstrmRet << L"\t\t" << L"SCREWROTATESPEED" << L"\t\t= ";
		wstrmRet << pSeq->GetScrewRotateSpeed() << std::endl;
	}

#ifdef PARTICLE_SPHERICAL_EMIT_TEST
	valfloat = pSeq->GetSphericalEmitRadius();
	if( valfloat.m_Max != 0.0f || valfloat.m_Min != 0.0f )
	{
		wstrmRet << L"\t\t" << L"SPHERICAL_EMITRADIUS" << L"\t\t= ";
		wstrmRet << GetMinMaxString( valfloat ) << std::endl;
	}

	valfloat = pSeq->GetSphericalEmitAzimuth();
	valfloat.m_Max *= 180.f/D3DX_PI; 
	valfloat.m_Min *= 180.f/D3DX_PI;
	if( valfloat.m_Max != 0.0f || valfloat.m_Min != 0.0f )
	{
		wstrmRet << L"\t\t" << L"SPHERICAL_EMITAZIMUTH" << L"\t\t= ";
		wstrmRet << GetMinMaxString( valfloat ) << std::endl;
	}

	valfloat = pSeq->GetSphericalEmitPolar();
	valfloat.m_Max *= 180.f/D3DX_PI; 
	valfloat.m_Min *= 180.f/D3DX_PI;
	if( valfloat.m_Max != 0.0f || valfloat.m_Min != 0.0f )
	{
		wstrmRet << L"\t\t" << L"SPHERICAL_EMITPOLAR" << L"\t\t= ";
		wstrmRet << GetMinMaxString( valfloat ) << std::endl;
	}

	if( pSeq->GetSphericalEmitRotation() != D3DXVECTOR3(0,0,0) )
	{
		wstrmRet << L"\t\t" << L"SPHERICAL_EMITROTATION" << L"\t\t= ";
		wstrmRet << L"XYZ(" << pSeq->GetSphericalEmitRotation().x << L"," << pSeq->GetSphericalEmitRotation().y << L"," << pSeq->GetSphericalEmitRotation().z << L")" << std::endl;
	}
#endif PARTICLE_SPHERICAL_EMIT_TEST

	wstrmRet << std::endl;
	//////////////////////////////////////////////////////////////////////////
	// 이제 이벤트 시퀀스들...

	//vector<CKTDGParticleSystem::CParticleEvent*>* pvecList = pSeq->GetEventList();
	//for( vector<CKTDGParticleSystem::CParticleEvent*>::iterator it = pvecList->begin(); it<pvecList->end(); ++it )
	list<CKTDGParticleSystem::CParticleEvent*>* pvecList = pSeq->GetEventList();
	for( list<CKTDGParticleSystem::CParticleEvent*>::iterator it = pvecList->begin(); it != pvecList->end(); ++it )
	{
		CKTDGParticleSystem::CParticleEvent* pEvent = *it;
		wstrmRet << L"\t\t" << MakeParticleEventString( pEvent, pSeq->GetLifeTime().m_Max ) << std::endl;
	}

	return wstrmRet.str();
}

wstring CX2ViewerParticleEditor::MakeParticleEventString( CKTDGParticleSystem::CParticleEvent* pEvent, const float FinalTime )
{
	wstringstream wstrmRet;
	//////////////////////////////////////////////////////////////////////////
	// 일단 시간 표시

	if( pEvent->IsFade() )
	{
		// Fade 일 때
		wstrmRet << L"fade so ";
	}

	if( pEvent->GetActualTime().m_Min == pEvent->GetActualTime().m_Max )
	{
		if( pEvent->GetActualTime().m_Min == 0.0f )
		{				
			wstrmRet << L"initial\t\t\t\t";
		}
		else
		{
			wstrmRet << L"at " << pEvent->GetActualTime().m_Min << L"\t\t\t";
		}
	}
	else
	{
		if( pEvent->GetActualTime().m_Min == 0 && pEvent->GetActualTime().m_Max == FinalTime )
		{
			wstrmRet << L"final\t\t\t\t";
		}
		else
		{
			wstrmRet << L"from " << pEvent->GetActualTime().m_Min << L" to " << pEvent->GetActualTime().m_Max << L"\t\t";
		}
	}	


	switch( pEvent->GetEventType() )
	{

	case CKTDGParticleSystem::ET_COLOR:
		{
			CKTDGParticleSystem::CParticleEvent_Color* pColorEvent = (CKTDGParticleSystem::CParticleEvent_Color*) pEvent;
			wstrmRet << L"color\t\t= rgba(" << GetMinMaxString(pColorEvent->GetColor().m_Min.r, pColorEvent->GetColor().m_Max.r) << L"," 
				<< GetMinMaxString(pColorEvent->GetColor().m_Min.g, pColorEvent->GetColor().m_Max.g) << L","
				<< GetMinMaxString(pColorEvent->GetColor().m_Min.b, pColorEvent->GetColor().m_Max.b) << L","
				<< GetMinMaxString(pColorEvent->GetColor().m_Min.a, pColorEvent->GetColor().m_Max.a) << L")";

		} break;
	case CKTDGParticleSystem::ET_SIZE:
		{
			CKTDGParticleSystem::CParticleEvent_Size* pSizeEvent = (CKTDGParticleSystem::CParticleEvent_Size*) pEvent;
			wstrmRet << L"size\t\t= " << GetMinMaxString(pSizeEvent->GetSize());
		} break;
	case CKTDGParticleSystem::ET_SCALE:
		{
			CKTDGParticleSystem::CParticleEvent_Scale* pScaleEvent = (CKTDGParticleSystem::CParticleEvent_Scale*) pEvent;
			wstrmRet << L"scale\t\t= " << GetMinMaxString(pScaleEvent->GetScale());
		} break;
	case CKTDGParticleSystem::ET_VELOCITY:
		{
			CKTDGParticleSystem::CParticleEvent_Velocity* pVelocityEvent = (CKTDGParticleSystem::CParticleEvent_Velocity*) pEvent;
			wstrmRet << L"Velocity\t= " << GetMinMaxString(pVelocityEvent->GetVelocity());
		} break;
	case CKTDGParticleSystem::ET_POSITION:
		{
			CKTDGParticleSystem::CParticleEvent_Position* pPosEvent = (CKTDGParticleSystem::CParticleEvent_Position*) pEvent;
			wstrmRet << L"position\t= " << GetMinMaxString(pPosEvent->GetPosition());
		} break;
	case CKTDGParticleSystem::ET_TEX0UV:
		{
			CKTDGParticleSystem::CParticleEvent_Tex0UV* pTex0Event = (CKTDGParticleSystem::CParticleEvent_Tex0UV*) pEvent;
			wstrmRet << L"TEX0UV\t\t= " << GetMinMaxString(pTex0Event->GetTexUV());
		} break;
	case CKTDGParticleSystem::ET_TEX1UV:
		{
			CKTDGParticleSystem::CParticleEvent_Tex1UV* pTex1Event = (CKTDGParticleSystem::CParticleEvent_Tex1UV*) pEvent;
			wstrmRet << L"TEX1UV\t\t= " << GetMinMaxString(pTex1Event->GetTexUV());
		} break;
	case CKTDGParticleSystem::ET_TEX2UV:
		{
			CKTDGParticleSystem::CParticleEvent_Tex2UV* pTex2Event = (CKTDGParticleSystem::CParticleEvent_Tex2UV*) pEvent;
			wstrmRet << L"TEX2UV\t\t= " << GetMinMaxString(pTex2Event->GetTexUV());
		} break;
	case CKTDGParticleSystem::ET_ROTATE:
		{
			CKTDGParticleSystem::CParticleEvent_Rotate* pRotEvent = (CKTDGParticleSystem::CParticleEvent_Rotate*) pEvent;
			wstrmRet << L"rotate\t\t= " << GetMinMaxString(pRotEvent->GetRotate());
		} break;
	case CKTDGParticleSystem::ET_DIRSPEED:
		{
			CKTDGParticleSystem::CParticleEvent_DirSpeed* pDirSpeedEvent = (CKTDGParticleSystem::CParticleEvent_DirSpeed*) pEvent;
			wstrmRet << L"dirSpeed\t= " << GetMinMaxString(pDirSpeedEvent->GetValue());
		} break;
	case CKTDGParticleSystem::ET_BLACKHOLE:
		{
			CKTDGParticleSystem::CParticleEvent_BlackHole* pBlackHoleEvent = (CKTDGParticleSystem::CParticleEvent_BlackHole*) pEvent;
			wstrmRet << L"BLACKHOLETIME\t= " << GetMinMaxString(pBlackHoleEvent->GetEventTimer());
		} break;
	case CKTDGParticleSystem::ET_CRASH:
		{
			CKTDGParticleSystem::CParticleEvent_Crash* pCrashEvent = (CKTDGParticleSystem::CParticleEvent_Crash*) pEvent;
			wstrmRet << L"crash\t\t= " << GetMinMaxString(pCrashEvent->GetCrash());
		} break;
	case CKTDGParticleSystem::ET_RESETCRASH:
		{
			CKTDGParticleSystem::CParticleEvent_ResetCrash* pResetCrashEvent = (CKTDGParticleSystem::CParticleEvent_ResetCrash*) pEvent;
			wstrmRet << L"resetcrash\t= " << GetMinMaxString(pResetCrashEvent->GetResetCrash());
		} break;
	case CKTDGParticleSystem::ET_TEXTURE:
		{
			CKTDGParticleSystem::CParticleEvent_Texture* pTextureEvent = (CKTDGParticleSystem::CParticleEvent_Texture*) pEvent;
			wstrmRet << L"texture\t= " << L"\"" << pTextureEvent->GetTextureName() << L"\"";

		} break;
	case CKTDGParticleSystem::ET_EVENTTIMER:
		{
			CKTDGParticleSystem::CParticleEvent_EventTimer* pEventTimerEvent = (CKTDGParticleSystem::CParticleEvent_EventTimer*) pEvent;
			wstrmRet << L"EventTimer\t= " << GetMinMaxString(pEventTimerEvent->GetEventTimer());
		} break;
#ifdef PARTICLE_STRETCH_TEST
	case CKTDGParticleSystem::ET_STRETCH:
		{
			CKTDGParticleSystem::CParticleEvent_Stretch* pStretchEvent = (CKTDGParticleSystem::CParticleEvent_Stretch*) pEvent;
			wstrmRet << L"stretch\t= " << GetMinMaxString(pStretchEvent->GetValue());
		} break;
#endif PARTICLE_STRETCH_TEST
	default:
		{
			WARNINGMSG( L"이벤트 타입 에러" );
			ASSERT( !"Unexpected EventType!" );
			return L"";
		} break;
	}

	return wstrmRet.str();
}


// Utility Function
template <class T>
wstring CX2ViewerParticleEditor::GetMinMaxString( T valmin, T valmax )
{
	wstringstream wstrmRet;
	if( valmax == valmin )
		wstrmRet << valmin;
	else
		wstrmRet << L"random(" << valmin << L"," << valmax << L")";

	return wstrmRet.str();

}

wstring CX2ViewerParticleEditor::GetMinMaxString( CMinMax<float> valfloat )
{
	wstringstream wstrmRet;
	if( valfloat.m_Max == valfloat.m_Min )
		wstrmRet << valfloat.m_Max;
	else
		wstrmRet << L"random(" << valfloat.m_Min << L"," << valfloat.m_Max << L")";

	return wstrmRet.str();
}

wstring CX2ViewerParticleEditor::GetMinMaxString( CMinMax<D3DXVECTOR3> valV3 )
{
	wstringstream wstrmRet;

	wstrmRet << L"XYZ(";
	if( valV3.m_Max.x == valV3.m_Min.x )
		wstrmRet << valV3.m_Max.x;
	else
		wstrmRet << L"random(" << valV3.m_Min.x << L"," << valV3.m_Max.x << L")";

	wstrmRet << L",";
	if( valV3.m_Max.y == valV3.m_Min.y )
		wstrmRet << valV3.m_Max.y;
	else
		wstrmRet << L"random(" << valV3.m_Min.y << L"," << valV3.m_Max.y << L")";

	wstrmRet << L",";
	if( valV3.m_Max.z == valV3.m_Min.z )
		wstrmRet << valV3.m_Max.z;
	else
		wstrmRet << L"random(" << valV3.m_Min.z << L"," << valV3.m_Max.z << L")";
	wstrmRet << L")";

	return wstrmRet.str();
}

wstring CX2ViewerParticleEditor::GetMinMaxString( CMinMax<D3DXVECTOR2> valV2 )
{
	wstringstream wstrmRet;

	wstrmRet << L"XY(";
	if( valV2.m_Max.x == valV2.m_Min.x )
		wstrmRet << valV2.m_Max.x;
	else
		wstrmRet << L"random(" << valV2.m_Min.x << L"," << valV2.m_Max.x << L")";

	wstrmRet << L",";
	if( valV2.m_Max.y == valV2.m_Min.y )
		wstrmRet << valV2.m_Max.y;
	else
		wstrmRet << L"random(" << valV2.m_Min.y << L"," << valV2.m_Max.y << L")";

	wstrmRet << L")";

	return wstrmRet.str();
}
//////////////////////////////////////////////////////////////////////////

CMinMax<D3DXVECTOR3> CX2ViewerParticleEditor::GetXYZValue( CKTDGParticleSystem::CParticleEventSequence* pSeq, EMITTER_PROPERTIES prop )
{
	switch(prop)
	{
	case EP_GRAVITY:
		{
			return pSeq->GetGravity();
		} break;
	case EP_ADDROTATEREL:
		{
			D3DXVECTOR3 val = pSeq->GetAddRotateRel();
			return CMinMax<D3DXVECTOR3>(val, val);
		} break;
	case EP_BLACKHOLEPOS:
		{
			D3DXVECTOR3 val = pSeq->GetBlackHolePosition();
			return CMinMax<D3DXVECTOR3>(val, val);
		} break;
	case EP_LIGHTPOS:
		{
			D3DXVECTOR3 val = pSeq->GetLightPos();
			return CMinMax<D3DXVECTOR3>(val, val);

		} break;
	case EP_SPHERICAL_EMITROTATION:
		{
			D3DXVECTOR3 val = pSeq->GetSphericalEmitRotation();
			return CMinMax<D3DXVECTOR3>(val, val);
		} break;
	case EP_EMITRADIUS:
		{
			return pSeq->GetEmitRadius();
		} break;
	case EP_EMITRADIUSMIN:
		{
			return pSeq->GetEmitRadiusMin();
		} break;
	default:
		{
			WARNINGMSG( L"내부 오류 : 잘못된 변수 타입" );
		}
	}

	return CMinMax<D3DXVECTOR3>( D3DXVECTOR3(0,0,0), D3DXVECTOR3(0,0,0) );

}
CMinMax<D3DXVECTOR2> CX2ViewerParticleEditor::GetXYValue( CKTDGParticleSystem::CParticleEventSequence* pSeq, EMITTER_PROPERTIES prop )
{
	// 여기로 오면 안됨!
	ASSERT( !"No Way!" );
	WARNINGMSG( L"내부 오류 : 잘못된 변수 타입" );
	return CMinMax<D3DXVECTOR2>( D3DXVECTOR2(0,0),D3DXVECTOR2(0,0));
}
int	CX2ViewerParticleEditor::GetIntValue( CKTDGParticleSystem::CParticleEventSequence* pSeq, EMITTER_PROPERTIES prop )
{
	switch(prop)
	{
	case EP_LAYER:
		{
			return pSeq->GetLayer();
		} break;
	case EP_TRIGGERCOUNT:
		{
			return pSeq->GetTriggerCount();
		}
	case EP_NUMPARTICLES:
		{
			return pSeq->GetMaxParticleNum();
		} break;
	case EP_DRAWCOUNT:
		{
			return pSeq->GetDrawCount();
		} break;
	case EP_SCREW_VALUE:
		{
			return pSeq->GetScrewValue();
		} break;
	case EP_SCREW_ROTATESPEED:
		{
			return pSeq->GetScrewRotateSpeed();
		} break;
	default:
		{
			WARNINGMSG( L"내부 오류 : 잘못된 변수 타입" );
		} break;
	}
	return 0;

}
CMinMax<float> CX2ViewerParticleEditor::GetFloatValue( CKTDGParticleSystem::CParticleEventSequence* pSeq, EMITTER_PROPERTIES prop )
{
	switch(prop)
	{
	
	case EP_LIFETIME:
		{
			return pSeq->GetLifeTime();
		} break;
	case EP_TRIGGERTIME:
		{
			float val = pSeq->GetTriggerTime();
			return CMinMax<float>( val, val );
		}
	case EP_EMITRATE:
		{
			return pSeq->GetEmitRate();
		} break;
	
	case EP_LANDPOS:
		{
			float val = pSeq->GetLandPosition();
			return CMinMax<float>( val, val );
		} break;


	case EP_SPHERICAL_EMITRADIUS:
		{
			return pSeq->GetSphericalEmitRadius();
		} break;
	case EP_SPHERICAL_EMITAZIMUTH:
		{
			return pSeq->GetSphericalEmitAzimuth();
		} break;
	case EP_SPHERICAL_EMITPOLAR:
		{
			return pSeq->GetSphericalEmitPolar();
		} break;
	default:
		{
			WARNINGMSG( L"내부 오류 : 잘못된 변수 타입" );
		} break;

	}

	return CMinMax<float>( 0, 0 );

}
wstring CX2ViewerParticleEditor::GetStringValue( CKTDGParticleSystem::CParticleEventSequence* pSeq, EMITTER_PROPERTIES prop )
{
	switch( prop )
	{

	case EP_TRACEPARTICLELIST:
		{
			vector<wstring> vecList = pSeq->GetTraceSeqNameList();
			if(vecList.empty())
			{
				return L"";
			}
			else
			{
				// 여러개 할 수 있어야 하는데.. 현재는 하나만 지원..
				return vecList[0];
			}
		} break;
	case EP_FINALPARTICLELIST:
		{
			vector<wstring> vecList = pSeq->GetFinalSeqNameList();
			if(vecList.empty())
			{
				return L"";
			}
			else
			{
				// 여러개 할 수 있어야 하는데.. 현재는 하나만 지원..
				return vecList[0];
			}
		} break;
	case EP_MESHNAME:
		{
			return pSeq->GetMeshName();
		} break;
	case EP_CHANGETEXNAME:
		{
			return pSeq->GetChangeTexXETName();
		} break;
	case EP_MULTITEXXET:
		{
			return pSeq->GetMultiTexXETName();
		} break;
	case EP_ANIXET:
		{
			return pSeq->GetAniXETName();
		} break;
	case EP_ANINAME:
		{
			return pSeq->GetAniName();
		} break;
	default:
		{
			WARNINGMSG( L"내부 오류 : 잘못된 변수 타입" );
		} break;
	
	}
	return L"";

}
bool CX2ViewerParticleEditor::GetBoolValue( CKTDGParticleSystem::CParticleEventSequence* pSeq, EMITTER_PROPERTIES prop )
{
	switch(prop)
	{
	case EP_USELAND:
		{
			return pSeq->GetUseLand();
		} break;
	case EP_RESOLUTIONCONVERT:
		{
			return pSeq->GetResolutionConvert();
		} break;
	case EP_TRACE:
		{
			return pSeq->GetTrace();
		} break;
	case EP_ZENABLE:
		{
			return pSeq->GetZEnable();
		} break;
	case EP_ZWRITEENABLE:
		{
			return pSeq->GetZWriteEnable();
		} break;
	case EP_FORCELAYER:
		{
			return pSeq->GetForceLayer();
		} break;
	default:
		{
			WARNINGMSG( L"내부 오류 : 잘못된 변수 타입" );
		} break;
	}
	return false;
	
}
void CX2ViewerParticleEditor::GetCustomComboBoxValue( CKTDGParticleSystem::CParticleEventSequence* pSeq, EMITTER_PROPERTIES prop, CDXUTComboBox* pCombo )
{
	pCombo->RemoveAllItems();

	switch( prop )
	{
	case EP_PARTICLETYPE:
		{
			for( int i = 0; i < PARTICLETYPES; ++i )
			{
				pCombo->AddItem( m_ParticleTypes[i].name, (void*)&(m_ParticleTypes[i]) );
				if( m_ParticleTypes[i].mode == pSeq->GetParticleType() )
				{
					pCombo->SetSelectedByIndex( i );
				}
			}
		} break;
	case EP_MESHRENDERTYPE:
		{	
			for( int i = 0; i < MESHRENDERTYPES; ++i )
			{
				pCombo->AddItem( m_MeshRenderTypes[i].name, (void*)&(m_MeshRenderTypes[i]) );
				if( m_MeshRenderTypes[i].mode == pSeq->GetMeshRenderType() )
				{
					pCombo->SetSelectedByIndex( i );
				}
			}

		} break;
	case EP_SRCBLENDMODE:
		{
			for( int i = 0; i < NUMBLENDINGMODES; ++i )
			{
				pCombo->AddItem( m_BlendingModes[i].name, (void*)&(m_BlendingModes[i]) );
				if( m_BlendingModes[i].mode == pSeq->GetSrcBlendMode() )
				{
					pCombo->SetSelectedByIndex( i );
				}
			}

		} break;
	case EP_DSTBLENDMODE:
		{
			for( int i = 0; i < NUMBLENDINGMODES; ++i )
			{
				pCombo->AddItem( m_BlendingModes[i].name, (void*)&(m_BlendingModes[i]) );
				if( m_BlendingModes[i].mode == pSeq->GetDestBlendMode() )
				{
					pCombo->SetSelectedByIndex( i );
				}
			}

		} break;
	case EP_CULLMODE:
		{
			for( int i = 0; i < NUMCULLMODES; ++i )
			{
				pCombo->AddItem( m_CullModes[i].name, (void*)&(m_CullModes[i]) );
				if( m_CullModes[i].mode == pSeq->GetCullMode() )
				{
					pCombo->SetSelectedByIndex( i );
				}
			}

		} break;
	case EP_BILLBOARDTYPE:
		{
			for( int i = 0; i < BILLBOARDMODES; ++i )
			{
				pCombo->AddItem( m_BillBoardMode[i].name, (void*)&(m_BillBoardMode[i]) );
				if( m_BillBoardMode[i].mode == pSeq->GetBillBoardType() )
				{
					pCombo->SetSelectedByIndex( i );
				}
			}

		} break;
	default:
		{
			WARNINGMSG( L"내부 오류 : 잘못된 변수 타입" );
		} break;
	}
}

void CX2ViewerParticleEditor::SetXYZValue( CKTDGParticleSystem::CParticleEventSequence* pSeq, EMITTER_PROPERTIES prop, CMinMax<D3DXVECTOR3> val )
{
	switch(prop)
	{
	case EP_GRAVITY:
		{
			return pSeq->SetGravity( val );
		} break;
	case EP_ADDROTATEREL:
		{
			pSeq->SetAddRotateRel( val.m_Max );
		} break;
	case EP_BLACKHOLEPOS:
		{
			pSeq->SetBlackHolePosition( val.m_Max );
		} break;
	case EP_LIGHTPOS:
		{
			pSeq->SetLightPos( val.m_Max );
		} break;
	case EP_SPHERICAL_EMITROTATION:
		{
			pSeq->SetSphericalEmitRotation( val.m_Max );
		} break;
	case EP_EMITRADIUS:
		{
			pSeq->SetEmitRadius( val );
		} break;
	case EP_EMITRADIUSMIN:
		{
			pSeq->SetEmitRadiusMin( val );
		} break;
	default:
		{
			WARNINGMSG( L"내부 오류 : 잘못된 변수 타입" );
		}
	}

	return;
}
void CX2ViewerParticleEditor::SetXYValue( CKTDGParticleSystem::CParticleEventSequence* pSeq, EMITTER_PROPERTIES prop, CMinMax<D3DXVECTOR2> val )
{
	// 여기로 오면 안됨!
	ASSERT( !"No Way!" );
	WARNINGMSG( L"내부 오류 : 잘못된 변수 타입" );
	return;

}
void CX2ViewerParticleEditor::SetFloatValue( CKTDGParticleSystem::CParticleEventSequence* pSeq, EMITTER_PROPERTIES prop, CMinMax<float> val )
{
	switch(prop)
	{

	case EP_LIFETIME:
		{
			pSeq->SetLifetime( val );
		} break;
	case EP_TRIGGERTIME:
		{
			pSeq->SetTriggerTime( val.m_Max );
		}
	case EP_EMITRATE:
		{
			pSeq->SetEmitRate( val );
		} break;

	case EP_LANDPOS:
		{
			pSeq->SetLandPosition( val.m_Max );
		} break;
	case EP_SPHERICAL_EMITRADIUS:
		{
			pSeq->SetSphericalEmitRadius( val );
		} break;
	case EP_SPHERICAL_EMITAZIMUTH:
		{
			pSeq->SetSphericalEmitAzimuth( val );
		} break;
	case EP_SPHERICAL_EMITPOLAR:
		{
			pSeq->SetSphericalEmitPolar( val );
		} break;
	default:
		{
			WARNINGMSG( L"내부 오류 : 잘못된 변수 타입" );
		} break;

	}

	return;

}
void CX2ViewerParticleEditor::SetIntValue( CKTDGParticleSystem::CParticleEventSequence* pSeq, EMITTER_PROPERTIES prop, int val )
{
	switch(prop)
	{
	case EP_LAYER:
		{
			pSeq->SetLayer( val );
		} break;
	case EP_TRIGGERCOUNT:
		{
			return pSeq->SetTriggerCount( val );
		}
	case EP_NUMPARTICLES:
		{
			pSeq->SetMaxParticleNum( val );
		} break;
	case EP_DRAWCOUNT:
		{
			pSeq->SetDrawCount( val );
		} break;
	case EP_SCREW_VALUE:
		{
			pSeq->SetScrewValue( val );
		} break;
	case EP_SCREW_ROTATESPEED:
		{
			pSeq->SetScrewRotateSpeed( val );
		} break;
	default:
		{
			WARNINGMSG( L"내부 오류 : 잘못된 변수 타입" );
		} break;
	}
	return;
}
void CX2ViewerParticleEditor::SetStringValue( CKTDGParticleSystem::CParticleEventSequence* pSeq, EMITTER_PROPERTIES prop, wstring val )
{
	switch( prop )
	{

	case EP_TRACEPARTICLELIST:
		{
			vector<wstring> vecList = pSeq->GetTraceSeqNameList();
			if(val.empty())
			{
				return;
			}
			else
			{
				// 여러개 할 수 있어야 하는데.. 현재는 하나만 지원..
				vecList.clear();
				vecList.push_back(val);
			}
		} break;
	case EP_FINALPARTICLELIST:
		{
			vector<wstring> vecList = pSeq->GetFinalSeqNameList();
			if(val.empty())
			{
				return;
			}
			else
			{
				// 여러개 할 수 있어야 하는데.. 현재는 하나만 지원..
				vecList.clear();
				vecList.push_back(val);
			}
		} break;
	case EP_MESHNAME:
		{
			pSeq->SetMeshName( val.c_str() );
		} break;
	case EP_CHANGETEXNAME:
		{
			pSeq->SetChangeTexXETName( val.c_str() );
		} break;
	case EP_MULTITEXXET:
		{
			pSeq->SetMultiTexXETName( val.c_str() );
		} break;
	case EP_ANIXET:
		{
			pSeq->SetAniXETName( val.c_str() );
		} break;
	case EP_ANINAME:
		{
			pSeq->SetAniName( val.c_str() );
		} break;
	default:
		{
			WARNINGMSG( L"내부 오류 : 잘못된 변수 타입" );
		} break;

	}
	return;
}
void CX2ViewerParticleEditor::SetBoolValue( CKTDGParticleSystem::CParticleEventSequence* pSeq, EMITTER_PROPERTIES prop, bool val )
{
	switch(prop)
	{
	case EP_USELAND:
		{
			pSeq->SetUseLand( val );
		} break;
	case EP_RESOLUTIONCONVERT:
		{
			pSeq->SetResolutionConvert( val );
		} break;
	case EP_TRACE:
		{
			pSeq->SetTrace( val );
		} break;
	case EP_ZENABLE:
		{
			pSeq->SetZEnable( val );
		} break;
	case EP_ZWRITEENABLE:
		{
			pSeq->SetZWriteEnable( val );
		} break;
	case EP_FORCELAYER:
		{
			pSeq->SetForceLayer( val );
		} break;
	default:
		{
			WARNINGMSG( L"내부 오류 : 잘못된 변수 타입" );
		} break;
	}
	return;

}
void CX2ViewerParticleEditor::SetCustomComboBoxValue( CKTDGParticleSystem::CParticleEventSequence* pSeq, EMITTER_PROPERTIES prop, DXUTComboBoxItem* pItem )
{	
	if( pSeq == NULL )
		return;
	if( pItem == NULL )
		return;

	ENUMSTRING* pData = (ENUMSTRING*)pItem->pData;
	if(pData == NULL)
		return;

	switch( prop )
	{
	case EP_PARTICLETYPE:
		{
			pSeq->SetParticleType( (CKTDGParticleSystem::PARTICLE_TYPE)pData->mode );
		} break;
	case EP_MESHRENDERTYPE:
		{	
			pSeq->SetMeshRenderType( (CKTDGXRenderer::RENDER_TYPE)pData->mode );
		} break;
	case EP_SRCBLENDMODE:
		{
			pSeq->SetSrcBlendMode( pData->mode );
		} break;
	case EP_DSTBLENDMODE:
		{
			pSeq->SetDestBlendMode( pData->mode );
		} break;
	case EP_CULLMODE:
		{
			pSeq->SetCullMode( pData->mode );
		} break;
	case EP_BILLBOARDTYPE:
		{
			pSeq->SetBillBoardType( (CKTDGMatrix::BILLBOARD_TYPE)pData->mode );
		} break;
	default:
		{
			WARNINGMSG( L"내부 오류 : 잘못된 변수 타입" );
		} break;
	}
	return;

}
wstring CX2ViewerParticleEditor::GetEventTimeString( CKTDGParticleSystem::CParticleEvent* pEvent, float Finaltime )
{
	if( pEvent == NULL )
		return L"";

	wstringstream wstrmRet;

	if( pEvent->IsFade() )
	{
		// Fade 일 때
		wstrmRet << L"fade ";
	}

	if( pEvent->GetActualTime().m_Min == pEvent->GetActualTime().m_Max )
	{
		if( pEvent->IsFade() )
		{
			WARNINGMSG( L"Fade의 시작 시간과 끝 시간이 같습니다" );
			return L"Fade ERROR";
		}
		
		if( pEvent->GetActualTime().m_Min == 0.0f )
		{				
			if( pEvent->IsFade())
			{				
				return L"Fade-Initial ERROR";
			}
			wstrmRet << L"initial";
		}
		else
		{
			wstrmRet << L"at " << pEvent->GetActualTime().m_Min;
		}
	}
	else
	{
		if( pEvent->GetActualTime().m_Min == 0 && pEvent->GetActualTime().m_Max == Finaltime )
		{
			wstrmRet << L"final";
		}
		else
		{
			wstrmRet << pEvent->GetActualTime().m_Min << L" ~ " << pEvent->GetActualTime().m_Max;
		}
	}	

	return wstrmRet.str();

}


CMinMax<D3DXVECTOR3> CX2ViewerParticleEditor::GetXYZValue( CKTDGParticleSystem::CParticleEvent* pEvent, CKTDGParticleSystem::EVENT_TYPE prop )
{
	CMinMax<D3DXVECTOR3> retval;
	retval.m_Min = D3DXVECTOR3(0,0,0);
	retval.m_Max = D3DXVECTOR3(0,0,0);
	switch( prop )
	{
	case CKTDGParticleSystem::ET_SIZE:
		{
			CKTDGParticleSystem::CParticleEvent_Size* pInheritedEvent = (CKTDGParticleSystem::CParticleEvent_Size*) pEvent;
			retval = pInheritedEvent->GetSize();
		} break;
	case CKTDGParticleSystem::ET_VELOCITY:
		{
			CKTDGParticleSystem::CParticleEvent_Velocity* pInheritedEvent = (CKTDGParticleSystem::CParticleEvent_Velocity*) pEvent;
			retval = pInheritedEvent->GetVelocity();
		} break;
	case CKTDGParticleSystem::ET_POSITION:
		{
			CKTDGParticleSystem::CParticleEvent_Position* pInheritedEvent = (CKTDGParticleSystem::CParticleEvent_Position*) pEvent;
			retval = pInheritedEvent->GetPosition();
		} break;
	case CKTDGParticleSystem::ET_ROTATE:
		{
			CKTDGParticleSystem::CParticleEvent_Rotate* pInheritedEvent = (CKTDGParticleSystem::CParticleEvent_Rotate*) pEvent;
			retval = pInheritedEvent->GetRotate();
		} break;
	case CKTDGParticleSystem::ET_CRASH:
		{
			CKTDGParticleSystem::CParticleEvent_Crash* pInheritedEvent = (CKTDGParticleSystem::CParticleEvent_Crash*) pEvent;
			retval = pInheritedEvent->GetCrash();
		} break;
	default:
		{
			WARNINGMSG( L"내부 오류 : 잘못된 변수 타입" );
		} break;
	}

	return retval;
	
}
CMinMax<D3DXVECTOR2> CX2ViewerParticleEditor::GetXYValue( CKTDGParticleSystem::CParticleEvent* pEvent, CKTDGParticleSystem::EVENT_TYPE prop )
{
	CMinMax<D3DXVECTOR2> retval;
	retval.m_Min = D3DXVECTOR2(0,0);
	retval.m_Max = D3DXVECTOR2(0,0);

	switch(prop)
	{
	case CKTDGParticleSystem::ET_TEX0UV:
		{
			CKTDGParticleSystem::CParticleEvent_Tex0UV* pInherEvent = (CKTDGParticleSystem::CParticleEvent_Tex0UV*) pEvent;
			retval = pInherEvent->GetTexUV();
		} break;
	case CKTDGParticleSystem::ET_TEX1UV:
		{
			CKTDGParticleSystem::CParticleEvent_Tex1UV* pInherEvent = (CKTDGParticleSystem::CParticleEvent_Tex1UV*) pEvent;
			retval = pInherEvent->GetTexUV();
		} break;
	case CKTDGParticleSystem::ET_TEX2UV:
		{
			CKTDGParticleSystem::CParticleEvent_Tex2UV* pInherEvent = (CKTDGParticleSystem::CParticleEvent_Tex2UV*) pEvent;
			retval = pInherEvent->GetTexUV();
		} break;
	default:
		{
			WARNINGMSG( L"내부 오류 : 잘못된 변수 타입" );
		} break;
	}
	return retval;
}
CMinMax<float> CX2ViewerParticleEditor::GetFloatValue( CKTDGParticleSystem::CParticleEvent* pEvent, CKTDGParticleSystem::EVENT_TYPE prop )
{
	CMinMax<float> retval;
	retval.m_Min = 0;
	retval.m_Max = 0;
	switch( prop )
	{
	case CKTDGParticleSystem::ET_SCALE:
		{
			CKTDGParticleSystem::CParticleEvent_Scale* pScaleEvent = (CKTDGParticleSystem::CParticleEvent_Scale*) pEvent;
			retval = pScaleEvent->GetScale();
		} break;
	case CKTDGParticleSystem::ET_DIRSPEED:
		{
			CKTDGParticleSystem::CParticleEvent_DirSpeed* pDirSpeedEvent = (CKTDGParticleSystem::CParticleEvent_DirSpeed*) pEvent;
			retval = pDirSpeedEvent->GetValue();
		} break;
	case CKTDGParticleSystem::ET_BLACKHOLE:
		{
			CKTDGParticleSystem::CParticleEvent_BlackHole* pBlackHoleEvent = (CKTDGParticleSystem::CParticleEvent_BlackHole*) pEvent;
			retval = pBlackHoleEvent->GetEventTimer();
		} break;
	case CKTDGParticleSystem::ET_RESETCRASH:
		{
			CKTDGParticleSystem::CParticleEvent_ResetCrash* pResetCrashEvent = (CKTDGParticleSystem::CParticleEvent_ResetCrash*) pEvent;
			retval = pResetCrashEvent->GetResetCrash();
		} break;
	case CKTDGParticleSystem::ET_EVENTTIMER:
		{
			CKTDGParticleSystem::CParticleEvent_EventTimer* pEventTimerEvent = (CKTDGParticleSystem::CParticleEvent_EventTimer*) pEvent;
			retval = pEventTimerEvent->GetEventTimer();
		} break;
	case CKTDGParticleSystem::ET_STRETCH:
		{
			CKTDGParticleSystem::CParticleEvent_Stretch* pStretchEvent = (CKTDGParticleSystem::CParticleEvent_Stretch*) pEvent;
			retval = pStretchEvent->GetValue();
		} break;
	default:
		{
			WARNINGMSG( L"내부 오류 : 잘못된 변수 타입" );
		} break;
	}

	return retval;

}
wstring CX2ViewerParticleEditor::GetStringValue( CKTDGParticleSystem::CParticleEvent* pEvent, CKTDGParticleSystem::EVENT_TYPE prop )
{
	switch( prop )
	{
	case CKTDGParticleSystem::ET_TEXTURE:
		{
			CKTDGParticleSystem::CParticleEvent_Texture* pInhEvent = (CKTDGParticleSystem::CParticleEvent_Texture*) pEvent;
			return pInhEvent->GetTextureName();
		} break;
	default:
		{
			WARNINGMSG( L"내부 오류 : 잘못된 변수 타입" );
		} break;
	}	

	return L"";

}
CMinMax<D3DXCOLOR> CX2ViewerParticleEditor::GetRGBAValue( CKTDGParticleSystem::CParticleEvent* pEvent, CKTDGParticleSystem::EVENT_TYPE prop )
{
	CMinMax<D3DXCOLOR> retval;
	retval.m_Min = D3DXCOLOR(0,0,0,0);
	retval.m_Max = D3DXCOLOR(0,0,0,0);

	switch( prop )
	{
	case CKTDGParticleSystem::ET_COLOR:
		{
			CKTDGParticleSystem::CParticleEvent_Color* pColorEvent = (CKTDGParticleSystem::CParticleEvent_Color*) pEvent;
			retval = pColorEvent->GetColor();
		} break;
	default:
		{
			WARNINGMSG( L"내부 오류 : 잘못된 변수 타입" );
		} break;
	}

	return retval;
}

void CX2ViewerParticleEditor::SetXYZValue( CKTDGParticleSystem::CParticleEvent* pEvent, CKTDGParticleSystem::EVENT_TYPE prop, CMinMax<D3DXVECTOR3> val )
{
	switch( prop )
	{
	case CKTDGParticleSystem::ET_SIZE:
		{
			CKTDGParticleSystem::CParticleEvent_Size* pInheritedEvent = (CKTDGParticleSystem::CParticleEvent_Size*) pEvent;
			pInheritedEvent->SetSize(val);
		} break;
	case CKTDGParticleSystem::ET_VELOCITY:
		{
			CKTDGParticleSystem::CParticleEvent_Velocity* pInheritedEvent = (CKTDGParticleSystem::CParticleEvent_Velocity*) pEvent;
			pInheritedEvent->SetVelocity(val);
		} break;
	case CKTDGParticleSystem::ET_POSITION:
		{
			CKTDGParticleSystem::CParticleEvent_Position* pInheritedEvent = (CKTDGParticleSystem::CParticleEvent_Position*) pEvent;
			pInheritedEvent->SetPosition(val);
		} break;
	case CKTDGParticleSystem::ET_ROTATE:
		{
			CKTDGParticleSystem::CParticleEvent_Rotate* pInheritedEvent = (CKTDGParticleSystem::CParticleEvent_Rotate*) pEvent;
			pInheritedEvent->SetRotate(val);
		} break;
	case CKTDGParticleSystem::ET_CRASH:
		{
			CKTDGParticleSystem::CParticleEvent_Crash* pInheritedEvent = (CKTDGParticleSystem::CParticleEvent_Crash*) pEvent;
			pInheritedEvent->SetCrash(val);
		} break;
	default:
		{
			WARNINGMSG( L"내부 오류 : 잘못된 변수 타입" );
		} break;
	}

}
void CX2ViewerParticleEditor::SetXYValue( CKTDGParticleSystem::CParticleEvent* pEvent, CKTDGParticleSystem::EVENT_TYPE prop, CMinMax<D3DXVECTOR2> val )
{
	switch(prop)
	{
	case CKTDGParticleSystem::ET_TEX0UV:
		{
			CKTDGParticleSystem::CParticleEvent_Tex0UV* pInherEvent = (CKTDGParticleSystem::CParticleEvent_Tex0UV*) pEvent;
			pInherEvent->SetTexUV(val);
		} break;
	case CKTDGParticleSystem::ET_TEX1UV:
		{
			CKTDGParticleSystem::CParticleEvent_Tex1UV* pInherEvent = (CKTDGParticleSystem::CParticleEvent_Tex1UV*) pEvent;
			pInherEvent->SetTexUV(val);
		} break;
	case CKTDGParticleSystem::ET_TEX2UV:
		{
			CKTDGParticleSystem::CParticleEvent_Tex2UV* pInherEvent = (CKTDGParticleSystem::CParticleEvent_Tex2UV*) pEvent;
			pInherEvent->SetTexUV(val);
		} break;
	default:
		{
			WARNINGMSG( L"내부 오류 : 잘못된 변수 타입" );
		} break;

	}
}
void CX2ViewerParticleEditor::SetFloatValue( CKTDGParticleSystem::CParticleEvent* pEvent, CKTDGParticleSystem::EVENT_TYPE prop, CMinMax<float> val )
{
	switch( prop )
	{
	case CKTDGParticleSystem::ET_SCALE:
		{
			CKTDGParticleSystem::CParticleEvent_Scale* pScaleEvent = (CKTDGParticleSystem::CParticleEvent_Scale*) pEvent;
			pScaleEvent->SetScale( val );
		} break;
	case CKTDGParticleSystem::ET_DIRSPEED:
		{
			CKTDGParticleSystem::CParticleEvent_DirSpeed* pDirSpeedEvent = (CKTDGParticleSystem::CParticleEvent_DirSpeed*) pEvent;
			pDirSpeedEvent->SetValue( val );
		} break;
	case CKTDGParticleSystem::ET_BLACKHOLE:
		{
			CKTDGParticleSystem::CParticleEvent_BlackHole* pBlackHoleEvent = (CKTDGParticleSystem::CParticleEvent_BlackHole*) pEvent;
			pBlackHoleEvent->SetEventTimer( val );
		} break;
	case CKTDGParticleSystem::ET_RESETCRASH:
		{
			CKTDGParticleSystem::CParticleEvent_ResetCrash* pResetCrashEvent = (CKTDGParticleSystem::CParticleEvent_ResetCrash*) pEvent;
			pResetCrashEvent->SetResetCrash( val );
		} break;
	case CKTDGParticleSystem::ET_EVENTTIMER:
		{
			CKTDGParticleSystem::CParticleEvent_EventTimer* pEventTimerEvent = (CKTDGParticleSystem::CParticleEvent_EventTimer*) pEvent;
			pEventTimerEvent->SetEventTimer( val );
		} break;
	case CKTDGParticleSystem::ET_STRETCH:
		{
			CKTDGParticleSystem::CParticleEvent_Stretch* pStretchEvent = (CKTDGParticleSystem::CParticleEvent_Stretch*) pEvent;
			pStretchEvent->SetValue( val );
		} break;
	default:
		{
			WARNINGMSG( L"내부 오류 : 잘못된 변수 타입" );
		} break;
	}
}
void CX2ViewerParticleEditor::SetStringValue( CKTDGParticleSystem::CParticleEvent* pEvent, CKTDGParticleSystem::EVENT_TYPE prop, wstring val )
{
	switch( prop )
	{
	case CKTDGParticleSystem::ET_TEXTURE:
		{
			CKTDGParticleSystem::CParticleEvent_Texture* pInhEvent = (CKTDGParticleSystem::CParticleEvent_Texture*) pEvent;
			pInhEvent->SetTextureName( val.c_str() );
		} break;
	default:
		{
			WARNINGMSG( L"내부 오류 : 잘못된 변수 타입" );
		} break;
	}	
}
void CX2ViewerParticleEditor::SetRGBAValue( CKTDGParticleSystem::CParticleEvent* pEvent, CKTDGParticleSystem::EVENT_TYPE prop, CMinMax<D3DXCOLOR> val )
{
	switch( prop )
	{
	case CKTDGParticleSystem::ET_COLOR:
		{
			CKTDGParticleSystem::CParticleEvent_Color* pColorEvent = (CKTDGParticleSystem::CParticleEvent_Color*) pEvent;
			pColorEvent->SetColor( val );
		} break;
	default:
		{
			WARNINGMSG( L"내부 오류 : 잘못된 변수 타입" );
		} break;
	}	
}


CKTDGParticleSystem::CParticleEvent* CX2ViewerParticleEditor::EventFactory( CKTDGParticleSystem::EVENT_TYPE eventtype )
{
	CKTDGParticleSystem::CParticleEvent* event = NULL;
	switch( eventtype )
	{
	case CKTDGParticleSystem::ET_COLOR:
		{
			event = new CKTDGParticleSystem::CParticleEvent_Color();
		} break;
	case CKTDGParticleSystem::ET_SIZE:
		{
			event = new CKTDGParticleSystem::CParticleEvent_Size();
		} break;

	case CKTDGParticleSystem::ET_SCALE:
		{
			event = new CKTDGParticleSystem::CParticleEvent_Scale();
		} break;
	case CKTDGParticleSystem::ET_VELOCITY:
		{
			event = new CKTDGParticleSystem::CParticleEvent_Velocity();
		} break;
	case CKTDGParticleSystem::ET_POSITION:
		{
			event = new CKTDGParticleSystem::CParticleEvent_Position();
		} break;
	case CKTDGParticleSystem::ET_TEX0UV:
		{
			event = new CKTDGParticleSystem::CParticleEvent_Tex0UV();
		} break;
	case CKTDGParticleSystem::ET_TEX1UV:
		{
			event = new CKTDGParticleSystem::CParticleEvent_Tex1UV();
		} break;
	case CKTDGParticleSystem::ET_TEX2UV:
		{
			event = new CKTDGParticleSystem::CParticleEvent_Tex2UV();
		} break;
	case CKTDGParticleSystem::ET_ROTATE:
		{
			event = new CKTDGParticleSystem::CParticleEvent_Rotate();
		} break;
	case CKTDGParticleSystem::ET_DIRSPEED:
		{
			event = new CKTDGParticleSystem::CParticleEvent_DirSpeed();
		} break;
	case CKTDGParticleSystem::ET_BLACKHOLE:
		{
			event = new CKTDGParticleSystem::CParticleEvent_BlackHole();
		} break;
	case CKTDGParticleSystem::ET_CRASH:
		{
			event = new CKTDGParticleSystem::CParticleEvent_Crash();
		} break;
	case CKTDGParticleSystem::ET_RESETCRASH:
		{
			event = new CKTDGParticleSystem::CParticleEvent_ResetCrash();
		} break;
	case CKTDGParticleSystem::ET_TEXTURE:
		{
			event = new CKTDGParticleSystem::CParticleEvent_Texture();
		} break;
	case CKTDGParticleSystem::ET_EVENTTIMER:
		{
			event = new CKTDGParticleSystem::CParticleEvent_EventTimer();
		} break;
#ifdef PARTICLE_STRETCH_TEST
	case CKTDGParticleSystem::ET_STRETCH:
		{
			event = new CKTDGParticleSystem::CParticleEvent_Stretch();
		} break;
#endif PARTICLE_STRETCH_TEST
	default:
		{
			WARNINGMSG( L"내부 오류 : 잘못된 변수 타입" );
		} break;
	}

	return(event);
}