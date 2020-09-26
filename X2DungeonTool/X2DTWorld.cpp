#include "dxstdafx.h"
#include ".\x2dtworld.h"

KX2DTWorld::KX2DTWorld( WorldData* pWorldData ) : CX2World( pWorldData )
{
	m_bRenderingWorld	 = true;
	m_bRenderingLineMap  = false;
}

KX2DTWorld::~KX2DTWorld(void)
{
}

HRESULT KX2DTWorld::OnFrameRender()
{
	// ±×·¡ÇÈ ·»´õ¸µ
	if( m_bRenderingWorld )
	{
		g_pKTDXApp->GetDGManager()->ObjectChainSort();
		g_pKTDXApp->GetDGManager()->ObjectChainNonAlphaRender();
		g_pKTDXApp->GetDGManager()->ObjectChainAlphaRender();
	}

	// ¶óÀÎ¸Ê ·»´õ¸µ
	if( m_bRenderingLineMap )
	{
		GetLineMap()->OnFrameRender();
	}

    return S_OK;
}

bool KX2DTWorld::OpenScriptFile( const WCHAR* pFileName )
{
	lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(), "g_pWorld", this );
	if( g_pX2Game != NULL )
	{
		lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(), "g_pMajorParticle", g_pX2Game->GetMajorParticle() );
		lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(), "g_pMinorParticle", g_pX2Game->GetMinorParticle() );
	}

	KGCMassFileManager::CMassFile::MASSFILE_MEMBERFILEINFO_POINTER Info;
	Info = g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->LoadDataFile( pFileName );
	if( Info == NULL )
	{
		string strFileName;
		ConvertWCHARToChar( strFileName, pFileName );
		ErrorLogMsg( XEM_ERROR70, strFileName.c_str() );

		return false;
	}

	if( g_pKTDXApp->GetLuaBinder()->DoMemory( Info->pRealData, Info->size ) == E_FAIL )
	{
		string strFileName;
		ConvertWCHARToChar( strFileName, pFileName );
		ErrorLogMsg( XEM_ERROR71, strFileName.c_str() );

		return false;
	}
	
	SetMapDetail( CX2GameOption::OptionLevel::OL_HIGH ); // LOW
	return true;
}