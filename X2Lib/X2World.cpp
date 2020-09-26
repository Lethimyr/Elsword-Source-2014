#include "stdafx.h"
#include ".\x2world.h"
#include "KHelpSystem.h"


#ifdef KHELP_SYSTEM_TEST // 2008-10-23

IMPLEMENT_HELP_SYSTEM( CX2World )
{
    WCHAR   wbuffer[1024];
    int     ix = 16;
    int     iy = 16;
    StringCchPrintfW( wbuffer, ARRAY_SIZE(wbuffer), L"%S", __FUNCTION__ );

    if ( g_pMain && g_pData )
    {
        CX2World*   pWorld = g_pData->GetWorldManager()->GetWorldLatest();
	    g_pMain->GetFontForDebuf()->OutTextMultiline( ix, iy, wbuffer, D3DXCOLOR( 1, 1, 0, 1), 
		    CKTDGFontManager::FS_SHELL, D3DXCOLOR( 0, 0, 0, 1 ) );
        if ( pWorld )
            pWorld->Debug_RenderHelp( ix, iy );
    }//if
}//CX2World::DisplayHelp()

#endif // KHELP_SYSTEM_TEST // 2008-10-23


// constructor.
CX2World::CX2World( WorldData* pWorldData, const bool bBackgroundLoad_ /*= false*/ )
: m_bBackgroundLoad( bBackgroundLoad_ )
, m_bObjectListSorted(false)
, m_pLineMap( NULL )
, m_pLineMapFuture( NULL )
, m_bFirstLineMapFrameMove( true )
{
	m_pWorldData	= pWorldData;

	m_bRenderLineMap = false;
	m_LodPercent = 100;



	INIT_VECTOR3( m_LightPos, 0.0f, 0.0f, 0.0f );
	m_LightColor			= 0xffffffff;		
	m_FadeLightColor		= 0xffffffff;	
	m_fFadeLightTime		= 0.f;



	m_OriginColor		= 0xffffffff;
	m_FadeOriginColor	= 0xffffffff;	
	m_fFadeOriginTime	= 0.0f;


	m_NowColor			= 0xffffffff;
	m_FadeColor			= 0xffffffff;	
	m_fFadeTime			= 0.0f;

#ifdef FOG_WORLD	
    m_bFog = false;   
    m_bFogShow = false;
	m_fDensity = 0.f;
	m_fFarX = 0.f;
	m_fFarY = 0.f;
	m_fNearX = 0.f;
	m_fNearY = 0.f;
	m_FogColor = D3DXCOLOR(0xd8e3fe);
#endif


#ifdef BUBBLE_BOBBLE_TEST
	m_vLookAt	= D3DXVECTOR3( 0, 0, 0 );
	m_vEye		= D3DXVECTOR3( 0, 0, -100 );
#endif BUBBLE_BOBBLE_TEST

    m_bDistanceColor = false;

	m_pKLuabinder	= NULL;





#ifdef DEEP_WATER_DUNGEON_TEST
	m_fGravityAndSpeedRate = 1.f;
	m_fUnitAnimSpeedRate = 1.f;
#endif DEEP_WATER_DUNGEON_TEST

#ifdef WORLD_TRIGGER
	m_iTriggerCameraIndex		= -1;
	m_fDurationTriggerCamera	= 0.f;
#endif

#ifdef MODIFY_FRUSTUM
	m_fBaseCameraDistance = 0.f;
	m_fCameraDistanceStep[0] = 0.f;
	m_fCameraDistanceStep[1] = 0.f;
	m_fCameraDistanceStep[2] = 0.f;
#endif

#ifdef X2TOOL
	m_wstrWorldName = L"";
	m_wstrWorldCamera = L"";
#endif

#ifdef OPTIMIZATION_DEV_MODE
	SetRenderLineMap(true);
#endif OPTIMIZATION_DEV_MODE
}

CX2World::~CX2World(void)
{
    //{{ seojt // 2008-10-22, 10:18
    /** 월드 객체를 파괴하기 바로 전에, 월드 매니저에게 월드가 파괴된다는 것을 알려준다.
        background thread loading작업을 하면서, 게임 상태와 상관없이 현 시점의 생성된 월드를
        얻어내는 작업을 하기 위해서 처음으로 추가하다.
        - jintaeks on 2008-10-22, 10:19 */
    if ( g_pData && g_pData->GetWorldManager() )
    {
        g_pData->GetWorldManager()->OnPreDestroyWorld( this );
    }//if
    //}} seojt // 2008-10-22, 10:18

	SAFE_DELETE_KTDGOBJECT( m_pLineMap );
	SAFE_DELETE_KTDGOBJECT( m_pLineMapFuture );


	for( int i = 0; i < (int)m_SkyDomeList.size(); i++ )
	{
		CKTDGSkyDome* pCKTDGSkyDome = m_SkyDomeList[i];
		SAFE_DELETE_KTDGOBJECT( pCKTDGSkyDome );
	}
	m_SkyDomeList.clear();

	m_ObjectList.clear();    
    
	for( int i = 0; i < (int)m_pLensFlareList.size(); i++ )
	{
		CKTDGLensFlare* pCKTDGLensFlare = m_pLensFlareList[i];
		SAFE_DELETE_KTDGOBJECT( pCKTDGLensFlare );
	}
	m_pLensFlareList.clear();

#ifdef WORLD_LAYER
    for( int i = 0; i < (int)m_LayerList.size(); i++ )
    {
        CX2WorldLayer* pWorldLayer = m_LayerList[i];
		SAFE_DELETE_KTDGOBJECT( pWorldLayer );
    }
    m_LayerList.clear();
#endif

	for ( int i = 0; i < (int)m_vecEffectBGM.size(); i++ )
	{
		CKTDXDeviceSound* pSound = m_vecEffectBGM[i];

		if( pSound != NULL && pSound->IsPlaying() == true )
			pSound->Stop();

		SAFE_CLOSE( pSound );
	}
	m_vecEffectBGM.clear();

	for ( int i = 0; i < (int)m_vec3DEffectBGM.size(); i++ )
	{
		CKTDXDeviceSound* pSound = m_vec3DEffectBGM[i];

		if( pSound != NULL && pSound->IsPlaying() == true )
			pSound->Stop();

		SAFE_CLOSE( pSound );
	}
	m_vec3DEffectBGM.clear();


    m_vecSyncObject.clear();

	//if( g_pKTDXApp->GetDSManager()->GetInit() == true && g_pKTDXApp->GetDSManager()->GetMP3Play() != NULL )
	//	g_pKTDXApp->GetDSManager()->GetMP3Play()->Stop();
#ifdef KEYFRAME_CAMERA
	UnLoadWorldCamera();
#endif KEYFRAME_CAMERA
}

HRESULT CX2World::OnFrameMove( double fTime, float fElapsedTime )
{
	KTDXPROFILE();



#ifdef WORLD_TRIGGER
	DoTriggerFrameMove( fTime, fElapsedTime );
	if( m_iTriggerCameraIndex >= 0 )
	{
		m_fDurationTriggerCamera -= fElapsedTime;
		if( m_fDurationTriggerCamera <= 0.f )
		{
			m_iTriggerCameraIndex = -1;
			m_fDurationTriggerCamera = 0.f;
		}
	}
#endif

	if( 0.f != m_fFadeTime && 
		m_FadeColor != m_NowColor )
	{
		D3DXCOLOR remainColor = m_FadeColor - m_NowColor;
		remainColor /= m_fFadeTime;
		m_NowColor += remainColor * fElapsedTime;

		m_fFadeTime -= fElapsedTime;
		if( m_fFadeTime <= 0.0f )
		{
			m_NowColor = m_FadeColor;
		}
	}


	if( m_fFadeOriginTime > 0.f )
	{
		m_fFadeOriginTime -= fElapsedTime;
		if( m_fFadeOriginTime <= 0.f )
		{
			m_fFadeOriginTime = 0.f;
			m_LightColor = m_FadeOriginColor;
		}
		else
		{
			D3DXCOLOR remainColor = m_FadeOriginColor - m_OriginColor;
			remainColor /= m_fFadeOriginTime;
			m_OriginColor += remainColor * fElapsedTime;
		}
	}



	if( m_fFadeLightTime > 0.f )
	{
		m_fFadeLightTime -= fElapsedTime;
		if( m_fFadeLightTime <= 0.f )
		{
			m_fFadeLightTime = 0.f;
			m_LightColor = m_FadeLightColor;
		}
		else
		{
			D3DXCOLOR remainColor = m_FadeLightColor - m_LightColor;
			remainColor /= m_fFadeLightTime;
			m_LightColor += remainColor * fElapsedTime;
		}
	}







	KTDXPROFILE_BEGIN( "SKY_DOME" );    

	for( int i = 0; i < (int)m_SkyDomeList.size(); i++ )
	{
		CKTDGSkyDome* pCKTDGSkyDome = m_SkyDomeList[i];
		if( pCKTDGSkyDome->GetShowObject() == true )
		{
			pCKTDGSkyDome->OnFrameMove( fTime, fElapsedTime );            
			pCKTDGSkyDome->SetColorFactor( m_NowColor );
		}		
	}
	KTDXPROFILE_END();

	KTDXPROFILE_BEGIN( "OBJECT" );

    if( m_bObjectListSorted == false )
    {
        SortOnetime();
    }//if

	for( int i = 0; i < (int)m_ObjectList.size(); i++ )
	{
		CX2WorldObjectPtr pCX2WorldObject = m_ObjectList[i];

		if( pCX2WorldObject->GetShowObject() == true )
		{
			pCX2WorldObject->OnFrameMove( fTime, fElapsedTime );
			if( true == pCX2WorldObject->GetUseWorldColor() )
			{
                D3DXCOLOR nowColor = m_NowColor;
#if 1
                if(m_bDistanceColor)
                {                    
                    nowColor.r += 0.0923f;
                    nowColor.g += 0.1198f;
                    nowColor.b += 0.3864f;
                }

				pCX2WorldObject->SetColorFactor( nowColor );
#endif
			}
		}		

	}

	KTDXPROFILE_END();

#ifdef WORLD_LAYER
    KTDXPROFILE_BEGIN( "LAYER" );
    for( int i = 0; i < (int)m_LayerList.size(); i++ )
    {
        CX2WorldLayer* pCX2WorldLayer = m_LayerList[i];
        if( pCX2WorldLayer->GetShowObject() == true )
        {
            pCX2WorldLayer->OnFrameMove( fTime, fElapsedTime );
            if( true == pCX2WorldLayer->GetUseWorldColor() )
            {
                D3DXCOLOR nowColor = m_NowColor;

                if(m_bDistanceColor)
                {
                    nowColor.r += 0.0923f;
                    nowColor.g += 0.1198f;
                    nowColor.b += 0.3864f;
                }

                pCX2WorldLayer->SetColorFactor( nowColor );
            }
        }		
    }
    KTDXPROFILE_END();
#endif

	return S_OK;
}

HRESULT CX2World::OnFrameRender()
{
	if( true == m_bRenderLineMap )
	{
		if( NULL != m_pLineMap )
			m_pLineMap->OnFrameRender();
	}
	return S_OK;
}

HRESULT CX2World::OnResetDevice()
{
	if( NULL != m_pLineMap )
		m_pLineMap->OnResetDevice();
	
	for( int i = 0; i < (int)m_SkyDomeList.size(); i++ )
	{
		CKTDGSkyDome* pCKTDGSkyDome = m_SkyDomeList[i];
		if ( pCKTDGSkyDome != NULL )
			pCKTDGSkyDome->OnResetDevice(); 
	}

	for( int i = 0; i < (int)m_ObjectList.size(); i++ )
	{
		CX2WorldObjectPtr pCX2WorldObject = m_ObjectList[i];
		if ( pCX2WorldObject != NULL )
			pCX2WorldObject->OnResetDevice();
	}

#ifdef WORLD_LAYER
    for( int i = 0; i < (int)m_LayerList.size(); i++ )
    {
        CX2WorldLayer* pCX2WorldLayer = m_LayerList[i];
		if ( pCX2WorldLayer != NULL )
			pCX2WorldLayer->OnResetDevice();
    }
#endif

	return S_OK;
}

HRESULT CX2World::OnLostDevice()
{
	if( NULL != m_pLineMap )
		m_pLineMap->OnLostDevice();

	for( int i = 0; i < (int)m_SkyDomeList.size(); i++ )
	{
		CKTDGSkyDome* pCKTDGSkyDome = m_SkyDomeList[i];
		if ( pCKTDGSkyDome != NULL )
			pCKTDGSkyDome->OnLostDevice();
	}

	for( int i = 0; i < (int)m_ObjectList.size(); i++ )
	{
		CX2WorldObjectPtr pCX2WorldObject = m_ObjectList[i];
		if ( pCX2WorldObject != NULL )
			pCX2WorldObject->OnLostDevice();
	}

#ifdef WORLD_LAYER
    for( int i = 0; i < (int)m_LayerList.size(); i++ )
    {
        CX2WorldLayer* pCX2WorldLayer = m_LayerList[i];
		if ( pCX2WorldLayer != NULL )
			pCX2WorldLayer->OnLostDevice();
    }
#endif

	return S_OK;
}

void CX2World::SetMapDetail( CX2GameOption::OptionLevel optionLevel )
{
	for( int i = 0; i < (int)m_SkyDomeList.size(); i++ )
	{
		CKTDGSkyDome* pCKTDGSkyDome = m_SkyDomeList[i];
		if( optionLevel == CX2GameOption::OL_LOW )
		{			
			//pCKTDGSkyDome->SetShowObject( false );
						
#ifdef FOG_WORLD
            pCKTDGSkyDome->SetFogShow( false );
#endif
		}
		else
		{
			//pCKTDGSkyDome->SetShowObject( true );
#ifdef FOG_WORLD            
            pCKTDGSkyDome->SetFogShow( true );            
#endif
		}
	}

#ifdef WORLD_LAYER
    for( int i = 0; i < (int)m_LayerList.size(); i++ )
    {
        CX2WorldLayer* pLayer = m_LayerList[i];
        if( optionLevel == CX2GameOption::OL_LOW )
        {
            pLayer->SetShowObject( false );
        }
        else
        {
			if( pLayer->GetInitShow() == true )
				pLayer->SetShowObject( true );
        }
    }
#endif
    
	for( int i = 0; i < (int)m_ObjectList.size(); i++ )
	{
		CX2WorldObjectPtr pCX2WorldObject = m_ObjectList[i];

        if( g_pSquareGame != NULL)            
        {
            if( pCX2WorldObject->GetObjectType() == CX2WorldObject::OT_STATIC_MESH )
            {
                CX2WorldObjectMesh* pWorldMesh = (CX2WorldObjectMesh *)pCX2WorldObject.get();
                if(pWorldMesh->GetXMeshName().compare(L"Square_Moon.Y") == 0)
                {
                    pCX2WorldObject->SetShowObject( true );
                    continue;
                }
            }
        }

		if( optionLevel == CX2GameOption::OL_LOW || optionLevel == CX2GameOption::OL_MEDIUM )
		{
			if( pCX2WorldObject->GetObjectType() != CX2WorldObject::OT_STATIC_MESH || pCX2WorldObject->IsAlphaObject() == true) 
			{                
				if( pCX2WorldObject->GetForceShow() == true )
				{
					pCX2WorldObject->SetShowObject( true );
				}
				else
				{
					pCX2WorldObject->SetShowObject( false );					
				}				
			}
			else
			{
				pCX2WorldObject->SetShowObject( true );
			}
		}
		else
		{
			pCX2WorldObject->SetShowObject( true );
		}

#ifdef FOG_WORLD
        // Fog Setting
        if( optionLevel != CX2GameOption::OL_LOW)
        {            
            if( pCX2WorldObject->GetObjectType() == CX2WorldObject::OT_STATIC_MESH )
            {
                CX2WorldObjectMesh *pWorldMesh = (CX2WorldObjectMesh *)pCX2WorldObject.get();
                pWorldMesh->SetFogShow(true);                
            }
            else if(pCX2WorldObject->GetObjectType() == CX2WorldObject::OT_SKIN_MESH)
            {
                CX2WorldObjectSkinMesh *pWorldMesh = (CX2WorldObjectSkinMesh *)pCX2WorldObject.get();
                pWorldMesh->SetFogShow(true);
            }   
        }
        else
        {
            if( pCX2WorldObject->GetObjectType() == CX2WorldObject::OT_STATIC_MESH )
            {
                CX2WorldObjectMesh *pWorldMesh = (CX2WorldObjectMesh *)pCX2WorldObject.get();
                pWorldMesh->SetFogShow(false);
            }
            else if(pCX2WorldObject->GetObjectType() == CX2WorldObject::OT_SKIN_MESH)
            {
                CX2WorldObjectSkinMesh *pWorldMesh = (CX2WorldObjectSkinMesh *)pCX2WorldObject.get();
                pWorldMesh->SetFogShow(false);
            }                    
        }
#endif
	}
}

#ifdef FOG_WORLD
void CX2World::ToggleFog(bool flag)
{
    for( int i = 0; i < (int)m_SkyDomeList.size(); i++ )
    {
        CKTDGSkyDome* pCKTDGSkyDome = m_SkyDomeList[i];            

        if(flag == false)
        {
            if( pCKTDGSkyDome->GetFogShow() == true )
                pCKTDGSkyDome->SetFogShow( false );
            else
                pCKTDGSkyDome->SetFogShow( true );
        }
        else
        {
            pCKTDGSkyDome->SetFogShow( true );
        }

        

    }

    for( int i = 0; i < (int)m_ObjectList.size(); i++ )
    {
        CX2WorldObjectPtr pCX2WorldObject = m_ObjectList[i];
                
        if( pCX2WorldObject->GetObjectType() == CX2WorldObject::OT_STATIC_MESH )
        {
            CX2WorldObjectMesh *pWorldMesh = (CX2WorldObjectMesh *)pCX2WorldObject.get();

            if(flag == false)
            {
                if( pWorldMesh->GetFogShow() == true )
                    pWorldMesh->SetFogShow(false);
                else
                    pWorldMesh->SetFogShow(true);
            }
            else
            {
                pWorldMesh->SetFogShow(true);
            }
            
        }
        else if(pCX2WorldObject->GetObjectType() == CX2WorldObject::OT_SKIN_MESH)
        {
            CX2WorldObjectSkinMesh *pWorldMesh = (CX2WorldObjectSkinMesh *)pCX2WorldObject.get();

            if(flag == false)
            {
                if( pWorldMesh->GetFogShow() == true )
                    pWorldMesh->SetFogShow(false);
                else
                    pWorldMesh->SetFogShow(true);
            }
            else
            {
                pWorldMesh->SetFogShow(true);
            }
            
        }        
    }

    if(flag == false)
    {
        if(m_bFogShow == true)
        {
            if(m_bFog == true)
                m_bFog = false;
            else
                m_bFog = true;
        }
    }    
}
#endif

void CX2World::PlayBGM()
{
	if( true == m_BGMName.empty() )
		return;

	if( NULL == g_pKTDXApp->GetDSManager() )
		return;

	if( g_pKTDXApp->GetDSManager()->GetInit() == false || g_pKTDXApp->GetDSManager()->GetMP3Play() == NULL )
		return;

#ifdef DUNGEONSTAGE_CONTINUOUS_BGM
	if(StrCmpW(m_BGMName.c_str(), g_pKTDXApp->GetDSManager()->GetMP3Play()->GetPlayingFileName().c_str()) == 0 )
		return;
#endif DUNGEONSTAGE_CONTINUOUS_BGM

	g_pKTDXApp->GetDSManager()->GetMP3Play()->Play( m_BGMName.c_str() );
}

void CX2World::StopBGM()
{
	if( g_pKTDXApp->GetDSManager()->GetInit() == false || g_pKTDXApp->GetDSManager()->GetMP3Play() == NULL )
		return;
	g_pKTDXApp->GetDSManager()->GetMP3Play()->Stop();
}//CX2World::StopBGM()


//{{ seojt // 2008-10-24, 11:11
struct CompareObject
{
    bool operator()( const CX2WorldObjectPtr& left_, const CX2WorldObjectPtr& right_ )
    {
        return _wcsicmp( left_->GetName(), right_->GetName() ) < 0;
    }//operator()()
};//struct CompareObject


void CX2World::SortOnetime()
{
    std::sort( m_ObjectList.begin(), m_ObjectList.end(), CompareObject() );
    m_bObjectListSorted = true;

#ifdef _DEBUG // 2008-10-24
    //TRACEA( "[CX2World::SortOnetime]\r\n" );
    //UINT    uiSize = m_ObjectList.size();
    //for ( UINT ui = 0; ui < uiSize; ++ui )
    //{
    //    CX2WorldObject* pWorldObject = m_ObjectList[ui];
    //    TRACEA( "%S\r\n", pWorldObject->GetName() );
    //}//for
#endif // _DEBUG // 2008-10-24

}//CX2World::SortOnetime()
//}} seojt // 2008-10-24, 11:11


//{{ seojt // 2008-10-21, 17:27
CX2WorldObject* CX2World::FindObjectByName(const std::wstring& wstrName_)
{
    /** m_ObjectList를 sort하여 유지한 다음 binary search를 사용하도록 수정할 것
        - jintaeks on 2008-10-22, 11:12 */

    //UINT    uiSize = m_ObjectList.size();
    //for ( UINT ui = 0; ui < uiSize; ++ui )
    //{
    //    CX2WorldObject* pWorldObject = m_ObjectList[ui];
    //    if ( _wcsicmp(wstrName_.c_str(),pWorldObject->GetName() ) == 0 )
    //        return pWorldObject;
    //}//for


    UINT            uiSize = m_ObjectList.size();
    UINT            uiMid = uiSize / 2;
    UINT            uiBegin = 0;
    UINT            uiEnd   = uiSize - 1;
    //CX2WorldObject* pWorldObject = NULL;
    int             iCompareResult;

    if ( uiSize <= 0 )
        return NULL;

    while ( uiBegin <= uiEnd )
    {
        uiMid = uiBegin + (uiEnd - uiBegin) / 2;
        CX2WorldObjectPtr pWorldObject = m_ObjectList[uiMid];
        iCompareResult = _wcsicmp(wstrName_.c_str(),pWorldObject->GetName() );
        if ( iCompareResult < 0 )
            uiEnd = uiMid - 1;
        else if ( iCompareResult > 0 )
            uiBegin = uiMid + 1;
        else
            return pWorldObject.get();
    }//while

    return NULL;
}//CX2World::FindObjectByName()
//}} seojt // 2008-10-21, 17:27


bool CX2World::OpenScriptFile( const WCHAR* pFileName, KLuabinder* pKLuabinder /* = NULL */ )
{
#ifdef MODIFY_FRUSTUM
	g_pKTDXApp->GetDGManager()->SetNear(10.f);
	g_pKTDXApp->GetDGManager()->SetFar(50000.f);
	g_pKTDXApp->GetDGManager()->SetPerspectiveValue(4.f);
#endif

	KLuabinder* pRealKLuabinder = g_pKTDXApp->GetLuaBinder();


	lua_tinker::decl( pRealKLuabinder->GetLuaState(),  "g_pWorld", this );
	if( g_pX2Game != NULL )
	{
		lua_tinker::decl( pRealKLuabinder->GetLuaState(),  "g_pMajorParticle", g_pX2Game->GetMajorParticle() );
		lua_tinker::decl( pRealKLuabinder->GetLuaState(),  "g_pMinorParticle", g_pX2Game->GetMinorParticle() );
		lua_tinker::decl( pRealKLuabinder->GetLuaState(),  "g_pUIMajorParticle", g_pData->GetUIMajorParticle() );
		lua_tinker::decl( pRealKLuabinder->GetLuaState(),  "g_pUIMinorParticle", g_pData->GetUIMinorParticle() );
	}
	else
	{
		if( g_pData != NULL )
		{
			lua_tinker::decl( pRealKLuabinder->GetLuaState(),  "g_pMajorParticle", g_pData->GetUIMajorParticle() );
			lua_tinker::decl( pRealKLuabinder->GetLuaState(),  "g_pMinorParticle", g_pData->GetUIMinorParticle() );
			lua_tinker::decl( pRealKLuabinder->GetLuaState(),  "g_pUIMajorParticle", g_pData->GetUIMajorParticle() );
			lua_tinker::decl( pRealKLuabinder->GetLuaState(),  "g_pUIMinorParticle", g_pData->GetUIMinorParticle() );
		}
	}

    // 게임월드의 스크립트를 파싱해서, 게임 월드의 게임 객체들을 생성한다.
    // DoMemory()가 파싱한 스크립트 함수의 호출이 내부적으로 g_pKTDXApp->GetLuaBinder()를
    // 또 사용하고 있다는 사실을 주의하자.
    // - jintaeks on 2008-10-21, 10:04, qff
    if ( g_pKTDXApp->LoadAndDoMemory( pRealKLuabinder, pFileName ) == false )
    {
		ErrorLogMsg( XEM_ERROR71, pFileName );

		return false;
    }


	//{{ seojt // 2008-10-24, 11:08
    // 메시 파일 이름으로 객체를 찾을 때, log(n)만에 검색하기 위해 
    // 메시 파일 이름으로 객체 리스트를 한 번 정렬해 둔다.
    // - jintaeks on 2008-10-24, 11:09
	SortOnetime();
	//}} seojt // 2008-10-24, 11:08



	//{{ dmlee 2009/07/28 현재는 전처리 파일을 사용하지 않아서 comment out 해둠, 나중에 사용할 수도 있음
//#ifdef BACKGROUND_LOADING_TEST // 2008-10-21
//	// 전처리된 파일이 있으면 그것을 읽어 들여서 정보를 설정한다.
//	// 전처리 파일의 이름은 "스크립트이름" + ".ppd"이다.
//	{
//		std::wstring wstrPreProcessingFile = pFileName;
//		wstrPreProcessingFile += L".ppd";
//		Info = g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->LoadDataFile( wstrPreProcessingFile.c_str() );
//		if ( Info != NULL )
//		{
//			if ( pRealKLuabinder->DoMemoryNotEncrypt( Info->pRealData, Info->size ) == false )
//			{
//				string strFileName;
//				ConvertWCHARToChar( strFileName, pFileName );
//				ErrorLogMsg( XEM_ERROR71, strFileName.c_str() );
//
//				return false;
//			}//if
//		}//if
//	}//block
//#endif // BACKGROUND_LOADING_TEST // 2008-10-21
	//}} dmlee 2009/07/28 현재는 전처리 파일을 사용하지 않아서 comment out 해둠, 나중에 사용할 수도 있음


	SetMapDetail( g_pMain->GetGameOption().GetOptionList().m_MapDetail );

#ifdef MODIFY_FRUSTUM
	g_pKTDXApp->GetDGManager()->SetProjection( g_pKTDXApp->GetDGManager()->GetNear(), g_pKTDXApp->GetDGManager()->GetFar(), 
		g_pKTDXApp->GetDGManager()->GetPerspective(), 
		g_pKTDXApp->GetDGManager()->GetOrthoScale() );	
#endif

#ifdef X2TOOL
	m_wstrWorldName = pFileName;
#endif

	return true;
}


void CX2World::SetShowObject( bool bShow )
{
	for( int i = 0; i < (int)m_SkyDomeList.size(); i++ )
	{
		CKTDGSkyDome* pCKTDGSkyDome = m_SkyDomeList[i];
		pCKTDGSkyDome->SetShowObject( bShow );
	}

	for( int i = 0; i < (int)m_ObjectList.size(); i++ )
	{
		CX2WorldObjectPtr pCX2WorldObject = m_ObjectList[i];
		pCX2WorldObject->SetShowObject( bShow );
	}

	for( int i = 0; i < (int)m_pLensFlareList.size(); i++ )
	{
		CKTDGLensFlare* pObject = m_pLensFlareList[i];
		pObject->SetShowObject( bShow );
	}
	
#ifdef WORLD_LAYER
	for( int i = 0; i < (int)m_LayerList.size(); i++ )
	{
		CX2WorldLayer* pObject = m_LayerList[i];
		if( pObject->GetInitShow() == true )
			pObject->SetShowObject( bShow );
	}
#endif
}

#ifdef X2TOOL
void CX2World::SetShowObjectByTool( bool bShow )
{
	SetMapDetail( g_pMain->GetGameOption().GetOptionList().m_MapDetail );

	for( int i = 0; i < (int)m_SkyDomeList.size(); i++ )
	{
		CKTDGSkyDome* pCKTDGSkyDome = m_SkyDomeList[i];
		if( pCKTDGSkyDome->m_bShowObject == true && pCKTDGSkyDome->GetShowObject() == true )
			pCKTDGSkyDome->SetShowObject( bShow );
		else
			pCKTDGSkyDome->SetShowObject( false );
	}

	for( int i = 0; i < (int)m_ObjectList.size(); i++ )
	{
		CX2WorldObjectPtr pCX2WorldObject = m_ObjectList[i];
		if( pCX2WorldObject->m_bShowObject == true && pCX2WorldObject->GetShowObject() == true )
			pCX2WorldObject->SetShowObject( bShow );
		else
			pCX2WorldObject->SetShowObject( false );		
	}

	for( int i = 0; i < (int)m_pLensFlareList.size(); i++ )
	{
		CKTDGLensFlare* pObject = m_pLensFlareList[i];
		pObject->SetShowObject( bShow );
	}

#ifdef WORLD_LAYER
	for( int i = 0; i < (int)m_LayerList.size(); i++ )
	{
		CX2WorldLayer* pObject = m_LayerList[i];
		if( pObject->GetInitShow() == true )
			pObject->SetShowObject( bShow );
	}
#endif

}
#endif

void CX2World::SetLightPos( float x, float y, float z )
{
	m_LightPos.x = x;
	m_LightPos.y = y;
	m_LightPos.z = z;
}

void CX2World::SetLightColor_LUA( float fRed, float fGreen, float fBlue )
{
	m_LightColor.r = fRed;
	m_LightColor.g = fGreen;
	m_LightColor.b = fBlue;
}

void CX2World::SetOriginColor_LUA( float fRed, float fGreen, float fBlue )
{
	m_OriginColor.r = fRed;
	m_OriginColor.g = fGreen;
	m_OriginColor.b = fBlue;

	m_FadeOriginColor = m_OriginColor;

	m_FadeColor		= m_OriginColor;
	m_NowColor		= m_OriginColor;
}

bool CX2World::SetLineMap_LUA( const char* pFileName )
{
	SAFE_DELETE_KTDGOBJECT( m_pLineMap );
	SAFE_DELETE_KTDGOBJECT( m_pLineMapFuture );


	wstring strBuf = L"";
	ConvertUtf8ToWCHAR( strBuf, pFileName );

	m_pLineMap = CKTDGLineMap::CreateLineMap();
	bool retval = m_pLineMap->LoadLineMap( strBuf );
	
#if defined(WORLD_TOOL) || defined(X2TOOL)
	if( retval == false )
	{
		//WARNINGMSG( L"라인맵 파일 없음! 스크립트 안의 SetLineMap 항목 확인하세요." );
		strBuf = L"World_WorldTool_Default_linemap.lua";
		retval = m_pLineMap->LoadLineMap( strBuf );
		
	}
#endif WORLD_TOOL

	return retval;
}

#ifdef X2TOOL
bool CX2World::GetLineMapByTool( wstring wstrFileName )
{
	SAFE_DELETE_KTDGOBJECT( m_pLineMap );
	SAFE_DELETE_KTDGOBJECT( m_pLineMapFuture );

	m_pLineMap = CKTDGLineMap::CreateLineMap();
	bool retval = m_pLineMap->LoadLineMap( wstrFileName );

	if( retval == false )
	{
		//WARNINGMSG( L"라인맵 파일 없음! 스크립트 안의 SetLineMap 항목 확인하세요." );
		wstring strBuf = L"World_WorldTool_Default_linemap.lua";
		retval = m_pLineMap->LoadLineMap( strBuf );

	}

	return retval;
}
#endif

void CX2World::SetBGM_LUA( const char* pFileName )
{
	ConvertUtf8ToWCHAR( m_BGMName, pFileName );
}
void CX2World::StopAllEffectBGM()
{	
	if( true == m_vecEffectBGM.empty() )
		return;

	BOOST_FOREACH( CKTDXDeviceSound* pDeviceSound,  m_vecEffectBGM )
	{
		pDeviceSound->Stop();
	}
}
void CX2World::PlayAllEffectBGM()
{	
	if( true == m_vecEffectBGM.empty() )
		return;

	BOOST_FOREACH( CKTDXDeviceSound* pDeviceSound,  m_vecEffectBGM )
	{
		pDeviceSound->Play( true, false );
	}
}
void CX2World::AddEffectBGM_LUA( const char* pFileName )
{
	wstring fileName;
	ConvertUtf8ToWCHAR( fileName, pFileName );
	CKTDXDeviceSound* pDevice = g_pKTDXApp->GetDeviceManager()->OpenSound( fileName.c_str(), 10, false );
	if( NULL != pDevice )
	{
		pDevice->Play( true, false );
		m_vecEffectBGM.push_back( pDevice );
	}
}

void CX2World::Add3DEffectBGM_LUA( const char* pFileName, float x, float y, float z, float fMax3DDistance )
{
	wstring fileName;
	ConvertUtf8ToWCHAR( fileName, pFileName );
	
	CKTDXDeviceSound* pDevice = g_pKTDXApp->GetDeviceManager()->OpenSound( fileName.c_str(), 10, true, fMax3DDistance );
    if( NULL != pDevice )
    {
	    pDevice->Set3DPosition( D3DXVECTOR3(x, y, z) );

		// 배경 3D sound는 옵션이 켜져있을 때만 play 한다
		if( true == g_pKTDXApp->GetDSManager()->GetCapable3DSound() &&
			true == g_pKTDXApp->GetDSManager()->GetEnable3DSound() &&
			true == g_pMain->GetGameOption().GetOptionList().m_bEnable3DSound )
		{
			pDevice->Play( true, true );
		}
    }

	if ( pDevice != NULL)
		m_vec3DEffectBGM.push_back( pDevice );
}


bool CX2World::SetWorldCamera_LUA( const char* pFileName )
{
#ifdef KEYFRAME_CAMERA
	wstring fileName;
	ConvertUtf8ToWCHAR( fileName, pFileName );

#ifdef X2TOOL
	m_wstrWorldCamera = fileName;
#endif

	switch(g_pMain->GetNowStateID())
	{
	case CX2Main::XS_DUNGEON_GAME:
		{
			if(g_pX2Game != NULL)
			{
#ifdef INIT_FOCUS_UNIT
				g_pX2Game->InitFocusUnit();
#endif //INIT_FOCUS_UNIT
				return g_pX2Game->GetX2Camera()->LoadWorldCamera( fileName.c_str() );	
			}
		}
		break;

#ifdef ELSWORD_NEW_BEGINNING
	case CX2Main::XS_BEGINNING:
		{
			if(g_pBeginningGame != NULL)
				return g_pBeginningGame->GetX2Camera()->LoadWorldCamera( fileName.c_str() );	
		}
		break;
#endif ELSWORD_NEW_BEGINNING
	}
#endif KEYFRAME_CAMERA
	return false;
}
#ifdef KEYFRAME_CAMERA
void CX2World::UnLoadWorldCamera()
{
	switch(g_pMain->GetNowStateID())
	{
	case CX2Main::XS_DUNGEON_GAME:
		{
			if(g_pX2Game != NULL)
				g_pX2Game->GetX2Camera()->UnLoadWorldCamera();	
		}
		break;

#ifdef ELSWORD_NEW_BEGINNING
	case CX2Main::XS_BEGINNING:
		{
			if(g_pBeginningGame != NULL)
				g_pBeginningGame->GetX2Camera()->UnLoadWorldCamera();	
		}
		break;
#endif ELSWORD_NEW_BEGINNING
	}

	return;
}
#endif KEYFRAME_CAMERA


void CX2World::PlayEffectBGM( bool bPlay )
{
	for ( int i = 0; i < (int)m_vecEffectBGM.size(); i++ )
	{
		CKTDXDeviceSound* pSound = m_vecEffectBGM[i];
		if ( pSound != NULL )
		{
			if( true == bPlay &&
				pSound->IsPlaying() == false )
			{
				pSound->Play( true, false );
			}
			else if ( false == bPlay && 
				pSound->IsPlaying() == true )
			{
				pSound->Stop();
			}
		}
	}
	
}



void CX2World::Play3DEffectBGM( bool bPlay )
{
	for ( int i = 0; i < (int)m_vec3DEffectBGM.size(); i++ )
	{
		CKTDXDeviceSound* pSound = m_vec3DEffectBGM[i];
		if ( pSound != NULL )
		{
			if( true == bPlay &&
				pSound->IsPlaying() == false )
			{
				// 배경 3D sound는 옵션이 켜져있을 때만 play 한다
				if( true == g_pKTDXApp->GetDSManager()->GetCapable3DSound() &&
					true == g_pKTDXApp->GetDSManager()->GetEnable3DSound() &&
					true == g_pMain->GetGameOption().GetOptionList().m_bEnable3DSound )
				{
					pSound->Play( true, true );
				}
			}
			else if( false == bPlay && 
				pSound->IsPlaying() == true )
			{
				pSound->Stop();
			}
		}
	}
}


void CX2World::SetEffectBGMVolume( float fSoundVolume )
{
	for ( int i = 0; i < (int)m_vecEffectBGM.size(); i++ )
	{
		CKTDXDeviceSound* pSound = m_vecEffectBGM[i];
		if ( pSound != NULL )
		{
			pSound->SetVolume( fSoundVolume );
		}
	}

	for ( int i = 0; i < (int)m_vec3DEffectBGM.size(); i++ )
	{
		CKTDXDeviceSound* pSound = m_vec3DEffectBGM[i];
		if ( pSound != NULL )
		{
			pSound->SetVolume( fSoundVolume );
		}
	}

}

CX2WorldObjectParticle* CX2World::CreateObjectParticleDelay( CKTDGParticleSystem* pParticleSystem, const char* pszSequenceName, float fDelay )
{
	//CX2WorldObjectParticle* pCX2WorldObjectParticle = new CX2WorldObjectParticle( pParticleSystem, pszSequenceName );
	CX2WorldObjectParticlePtr pCX2WorldObjectParticle = CX2WorldObjectParticle::CreateWorldObjectParticle( pParticleSystem, pszSequenceName, fDelay );
	m_ObjectList.push_back( pCX2WorldObjectParticle );	
	m_bObjectListSorted = false;
	return pCX2WorldObjectParticle.get();
}

CX2WorldObjectParticle* CX2World::CreateObjectParticle( CKTDGParticleSystem* pParticleSystem, const char* pszSequenceName )
{
	//CX2WorldObjectParticle* pCX2WorldObjectParticle = new CX2WorldObjectParticle( pParticleSystem, pszSequenceName );
	CX2WorldObjectParticlePtr pCX2WorldObjectParticle = CX2WorldObjectParticle::CreateWorldObjectParticle( pParticleSystem, pszSequenceName, 0.f );
	m_ObjectList.push_back( pCX2WorldObjectParticle );	
    m_bObjectListSorted = false;
	return pCX2WorldObjectParticle.get();
}


CX2WorldObjectLightFlow* CX2World::CreateObjectLightFlow()
{
	//CX2WorldObjectLightFlow* pCX2WorldObjectLightFlow = new CX2WorldObjectLightFlow();
	CX2WorldObjectLightFlowPtr pCX2WorldObjectLightFlow = CX2WorldObjectLightFlow::CreateWorldObjectLightFlow( m_bBackgroundLoad );
	
	m_ObjectList.push_back( pCX2WorldObjectLightFlow );
    m_bObjectListSorted = false;
	g_pKTDXApp->GetDGManager()->AddObjectChain( pCX2WorldObjectLightFlow.get() );

	return pCX2WorldObjectLightFlow.get();
}


CX2WorldObjectSkinMesh* CX2World::CreateObjectSkinMesh()
{
	//CX2WorldObjectSkinMesh* pCX2WorldObjectSkinMesh = new CX2WorldObjectSkinMesh();
	CX2WorldObjectSkinMeshPtr pCX2WorldObjectSkinMesh = CX2WorldObjectSkinMesh::CreateWorldObjectSkinMesh( m_bBackgroundLoad );
	m_ObjectList.push_back( pCX2WorldObjectSkinMesh );
    m_bObjectListSorted = false;
	g_pKTDXApp->GetDGManager()->AddObjectChain( pCX2WorldObjectSkinMesh.get() );

	pCX2WorldObjectSkinMesh->SetLightPos( GetLightPos() );

	return pCX2WorldObjectSkinMesh.get();
}//CX2World::CreateObjectSkinMesh()


CX2WorldObjectMesh* CX2World::CreateObjectMesh()
{
#ifdef OPTIMIZATION_DEV_MODE

	return NULL;

#else

	//CX2WorldObjectMesh* pCX2WorldObjectMesh = new CX2WorldObjectMesh();
	CX2WorldObjectMeshPtr pCX2WorldObjectMesh = CX2WorldObjectMesh::CreateWorldObjectMesh( m_bBackgroundLoad );
	if ( pCX2WorldObjectMesh->GetMesh() != NULL )
		pCX2WorldObjectMesh->GetMesh()->SetLOD( m_LodPercent );

	m_ObjectList.push_back( pCX2WorldObjectMesh );
    m_bObjectListSorted = false;
	g_pKTDXApp->GetDGManager()->AddObjectChain( pCX2WorldObjectMesh.get() );

	pCX2WorldObjectMesh->SetLightPos( GetLightPos() );

	return pCX2WorldObjectMesh.get();

#endif OPTIMIZATION_DEV_MODE
}


CKTDGSkyDome* CX2World::CreateSkyDome()
{
	//CKTDGSkyDome* pCKTDGSkyDome = new CKTDGSkyDome();
	CKTDGSkyDome* pCKTDGSkyDome = CKTDGSkyDome::CreateSkyDome();

	m_SkyDomeList.push_back( pCKTDGSkyDome );
    g_pKTDXApp->GetDGManager()->AddObjectChain( pCKTDGSkyDome );

	return pCKTDGSkyDome;
}


CKTDGLensFlare* CX2World::CreateLensFlare()
{
	//CKTDGLensFlare* pLensFlare = new CKTDGLensFlare( g_pKTDXApp->GetHWND(), g_pKTDXApp->GetDevice(),
	//									L"LensFlare_Halo.dds",
	//									L"LensFlare_01.dds",
	//									L"LensFlare_02.dds",
	//									L"LensFlare_03.dds" );

	CKTDGLensFlare* pLensFlare = CKTDGLensFlare::CreateLensFlare( g_pKTDXApp->GetHWND(), 
		L"LensFlare_Halo.dds",
		L"LensFlare_01.dds",
		L"LensFlare_02.dds",
		L"LensFlare_03.dds" );


	if( NULL != pLensFlare )
	{
		m_pLensFlareList.push_back( pLensFlare );
		g_pKTDXApp->GetDGManager()->AddObjectChain( pLensFlare );
	}

	return pLensFlare;
}

#ifdef WORLD_LAYER
CX2WorldLayer* CX2World::CreateWorldLayer()
{
    //CX2WorldLayer* pWorldlayer = new CX2WorldLayer( g_pKTDXApp->GetHWND(), g_pKTDXApp->GetDevice() );
	CX2WorldLayer* pWorldlayer = CX2WorldLayer::CreateWorldLayer( g_pKTDXApp->GetHWND() );
	pWorldlayer->SetLayerId( m_LayerList.size() );

    if( NULL != pWorldlayer )
    {
        m_LayerList.push_back( pWorldlayer );
        g_pKTDXApp->GetDGManager()->AddObjectChain( pWorldlayer );
    }

    return pWorldlayer;
}

void CX2World::RemoveWordLayer(int index)
{
    if(index < 0)
    {
        for( int i = 0; i < (int)m_LayerList.size(); i++ )
        {
            CX2WorldLayer* pWorldlayer = m_LayerList[i];
            //g_pKTDXApp->GetDGManager()->RemoveObjectChain(pWorldlayer);
            //SAFE_DELETE( pWorldlayer );
			CX2WorldLayer::DeleteKTDGObject( pWorldlayer );
			
        }
        m_LayerList.clear();
    }
    else
    {
        CX2WorldLayer* pWorldlayer = m_LayerList[index];
        if(pWorldlayer != NULL)
        {
            //g_pKTDXApp->GetDGManager()->RemoveObjectChain(pWorldlayer);
            //SAFE_DELETE( pWorldlayer );
			
			CX2WorldLayer::DeleteKTDGObject( pWorldlayer );

            m_LayerList.erase(m_LayerList.begin() + index);
        }        
    }

}
#endif

void CX2World::SetLOD( int lodPercent )
{
	if ( lodPercent > 100 )
		lodPercent = 100;

	if ( lodPercent < 0 )
		lodPercent = 0;

	m_LodPercent = lodPercent;

	for ( int i = 0; i < (int)m_ObjectList.size(); i++ )
	{
		CX2WorldObjectPtr pObject = m_ObjectList[i];
		if ( pObject == NULL )
			continue;

		if ( pObject->GetObjectType() == CX2WorldObject::OT_STATIC_MESH )
		{
			CX2WorldObjectMesh* pMeshObject = (CX2WorldObjectMesh*)pObject.get();
			if ( pMeshObject->GetMesh() != NULL )
			{
				pMeshObject->GetMesh()->SetLOD( m_LodPercent );
			}
		}
	}
}

void CX2World::ClearObjectMesh()
{
    m_ObjectList.clear();
}


#ifdef FOG_WORLD
void CX2World::SetFogWorld(float fNearX, float fFarX, float fNearY, float fFarY, float fDensity, D3DXCOLOR fogColor)
{
	m_bFog      = true;
	m_bFogShow  = true;
	m_fNearX    = fNearX;
	m_fFarX     = fFarX;
	m_fNearY    = fNearY;
	m_fFarY     = fFarY;
	m_fDensity  = fDensity;
	m_FogColor  = fogColor;
}
#endif

CX2WorldObject* CX2World::GetSyncObject(int index)
{
    if((int)m_vecSyncObject.size() > index)
    {
        return m_vecSyncObject[index];
    }
    else
        return NULL;
}

void CX2World::FrameMoveSyncObject(float fx, float fy)
{
}


//{{ seojt // 2008-10-24, 14:06
void CX2World::Debug_RenderHelp(int ix_,int iy_)
{
    int     ix = ix_ + 16;
    int     iy = iy_ + 16;
    WCHAR   wbuffer[1024];

    if ( m_pWorldData )
    {
        // World Data 정보 출력
        //
        StringCchPrintfW( wbuffer, ARRAY_SIZE(wbuffer),
            L"WorldID = %i\n"
            L"WorldName = %s\n"
            L"DataFileName = %s\n",
            m_pWorldData->worldID,
            m_pWorldData->worldName.c_str(),
            m_pWorldData->dataFileName.c_str() );
	    g_pMain->GetFontForDebuf()->OutTextMultiline( ix, iy, wbuffer, D3DXCOLOR( 1, 1, 0, 1), 
		    CKTDGFontManager::FS_SHELL, D3DXCOLOR( 0, 0, 0, 1 ) );
        iy += 16 * 4;

		// Culling 정보 출력
		//
		UINT    uiNumTotalObjects   = m_ObjectList.size();
		UINT    uiNumCulled         = 0;
		std::vector<CX2WorldObjectPtr>::iterator  vitor;
		for ( vitor = m_ObjectList.begin(); vitor != m_ObjectList.end(); ++vitor )
		{
			CX2WorldObjectPtr pWorldObject = *vitor;
			if ( pWorldObject->IsCulled() )
			{
				++uiNumCulled;
			}//if
		}//for

		StringCchPrintfW( wbuffer, ARRAY_SIZE(wbuffer),
			L"Culled = %i / %i", uiNumCulled, uiNumTotalObjects );
		g_pMain->GetFontForDebuf()->OutTextMultiline( ix, iy, wbuffer, D3DXCOLOR( 1, 1, 0, 1), 
			CKTDGFontManager::FS_SHELL, D3DXCOLOR( 0, 0, 0, 1 ) );
		iy += 16;


		//wstringstream wstrm;
		//for( UINT i=0; i<m_ObjectList.size(); i++ )
		//{
		//	CX2WorldObject* pWorldObject = m_ObjectList[i];
		//	if( NULL == pWorldObject )
		//		continue;

		//	wstrm << L"idx: " << i << ", "
		//		<< pWorldObject->GetName() << ", " 
		//		<< L"culled: " << pWorldObject->IsCulled() << ", "
		//		<< L"visible: " << pWorldObject->GetShowObject() << ", "
		//		<< L"center: ("
		//		<< (int)pWorldObject->GetCenter().x << ", "
		//		<< (int)pWorldObject->GetCenter().y << ", "
		//		<< (int)pWorldObject->GetCenter().z << "), "
		//		<< std::endl;
		//}

		//g_pMain->GetFontForDebuf()->OutTextMultiline( ix, iy, wstrm.str().c_str(), D3DXCOLOR( 1, 1, 0, 1 ), 
		//	CKTDGFontManager::FS_SHELL, D3DXCOLOR( 0, 0, 0, 1 ) );
		//iy += 16;

    }//if
#ifdef SHOW_LINEMAP_FILE_NAME
	if( NULL != m_pLineMap )
	{
		StringCchPrintfW( wbuffer, ARRAY_SIZE(wbuffer),	L"LineMap = %s\n", m_pLineMap->GetLineFullName().c_str() );
		g_pMain->GetFontForDebuf()->OutTextMultiline( ix, iy, wbuffer, D3DXCOLOR( 1, 1, 0, 1), 
			CKTDGFontManager::FS_SHELL, D3DXCOLOR( 0, 0, 0, 1 ) );
		iy += 16 * 4;
	}
#endif //SHOW_LINEMAP_FILE_NAME
}//CX2World::Debug_RenderHelp()
//}} seojt // 2008-10-24, 14:06


#if defined(WORLD_TOOL) || defined(X2TOOL)
//{{ 2009.02.25 김태완 : GetWorldObjectList에서 받은 vector을 변경시킬 수 없는 관계로
//World에서 특정 오브젝트를 삭제하는 기능 추가 : 월드툴 용
void CX2World::RemoveWorldObject(CX2WorldObject* pTargetWorldObject)
{
	for( UINT i=0; i<m_ObjectList.size(); i++ )
	{
		CX2WorldObject* pWorldObject = m_ObjectList[i].get();
		if( pWorldObject == pTargetWorldObject )
		{
			m_ObjectList[i].reset(); 
			m_ObjectList.erase( m_ObjectList.begin()+i );
			//SAFE_DELETE( pTargetWorldObject );
			break;
		}
	}
}

void CX2World::RemoveSkyDome(int index)
{
	if(index < 0)
	{
		for( int i = 0; i < (int)m_SkyDomeList.size(); i++ )
		{
			CKTDGSkyDome* pSkyDome = m_SkyDomeList[i];
			CKTDGSkyDome::DeleteKTDGObject( pSkyDome );

		}
		m_SkyDomeList.clear();
	}
	else
	{
		CKTDGSkyDome* pSkyDome = m_SkyDomeList[index];
		if(pSkyDome != NULL)
		{
			CKTDGSkyDome::DeleteKTDGObject( pSkyDome );

			m_SkyDomeList.erase(m_SkyDomeList.begin() + index);
		}        
	}

}

void CX2World::AddWorldObject4Undo(CX2WorldObjectPtr pTargetWorldObjectPtr)
{
	if( pTargetWorldObjectPtr == NULL )
		return;

	switch( pTargetWorldObjectPtr->GetObjectType() )
	{
	case CX2WorldObject::OT_STATIC_MESH:
		{
			m_ObjectList.push_back( pTargetWorldObjectPtr );
			g_pKTDXApp->GetDGManager()->AddObjectChain( pTargetWorldObjectPtr.get() );
		}
		break;
	case CX2WorldObject::OT_SKIN_MESH:
		{			
			m_ObjectList.push_back( pTargetWorldObjectPtr );
			g_pKTDXApp->GetDGManager()->AddObjectChain( pTargetWorldObjectPtr.get() );
		}
		break;
	case CX2WorldObject::OT_PARTICLE:
		{
			m_ObjectList.push_back( pTargetWorldObjectPtr );
		}
		break;
	default:
		break;
	}
}

void CX2World::DeleteWorldObject4Undo(CX2WorldObject* pTargetWorldObject)
{
	if( pTargetWorldObject == NULL )
		return;

	for( UINT i=0; i<m_ObjectList.size(); i++ )
	{
		CX2WorldObject* pWorldObject = m_ObjectList[i].get();
		if( pWorldObject == pTargetWorldObject )
		{
			g_pKTDXApp->GetDGManager()->RemoveObjectChain( pWorldObject );
			m_ObjectList.erase( m_ObjectList.begin()+i );
			break;
		}
	}
}
//}}
#endif




void CX2World::AddWorldMonster_LUA()
{
#ifdef MARIO_LIKE_BLOCK_TEST

	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
#ifndef X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
    TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING

	WorldMonsterData worldMonsterData;
	
	LUA_GET_VALUE( luaManager, "TEAM_NUMBER",		worldMonsterData.m_iTeamNumber,		-1 );
	if( worldMonsterData.m_iTeamNumber < 0 )
		return;

	LUA_GET_VALUE( luaManager, "NPC_ID",			worldMonsterData.m_iNPCID,			-1 );
	if( worldMonsterData.m_iNPCID < 0 )
		return; 

	LUA_GET_VALUE( luaManager, "LEVEL",				worldMonsterData.m_iLevel,			-1 );
	if( worldMonsterData.m_iLevel <= 0 )
		return; 


#ifdef  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
    LUA_GET_USER_DEFINED_TYPE_VALUE( luaManager, "POSITION", worldMonsterData.m_vPosition, D3DXVECTOR3(0,0,0) );
#else   X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
	// lua script에 값이 없는 경우가 있으면 lua_tinker::get() 사용하면 안됨
	worldMonsterData.m_vPosition = lua_tinker::get<D3DXVECTOR3>( luaManager.GetLuaState(), "POSITION" );
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING

	m_vecWorldMonsterData.push_back( worldMonsterData );
#endif MARIO_LIKE_BLOCK_TEST

}

#ifdef WORLD_TRIGGER
CX2WorldObjectSkinMesh* CX2World::GetTriggerMesh(const wstring &wstrMeshName)
{
	for( int i = 0; i < (int)m_ObjectList.size(); i++ )
	{
		CX2WorldObjectPtr pCX2WorldObject = m_ObjectList[i];
		
		if( pCX2WorldObject->GetObjectType() == CX2WorldObject::OT_SKIN_MESH )
		{
			CX2WorldObjectSkinMesh* pWorldMesh = (CX2WorldObjectSkinMesh *)pCX2WorldObject.get();

			wstring wstrMeshName1 = wstrMeshName;
			wstring wstrMeshName2 = pWorldMesh->GetXSkinMeshName();
			MakeUpperCase(wstrMeshName1);
			MakeUpperCase(wstrMeshName2);
			if( pWorldMesh != NULL && wstrMeshName1 == wstrMeshName2 )
				return pWorldMesh;			
		}
	}
	return NULL;	
}

void CX2World::DoTriggerMesh(CKTDGLineMap::TRIGGER_DATA_MESH &triggerMesh)
{	
	triggerMesh.m_bDone = true;
	
	if( triggerMesh.m_wstrMeshName.empty() )
		return;
	
	CX2WorldObjectSkinMesh *pWorldMesh = GetTriggerMesh( triggerMesh.m_wstrMeshName );
	if( pWorldMesh == NULL )
		return;

	switch(triggerMesh.m_iMeshTriggerAction)
	{
	case CKTDGLineMap::TAT_MESH_CHANGE_ANIM:
		{			
			pWorldMesh->ChangePlayAnim_LUA( triggerMesh.m_wstrName, (CX2WorldObjectSkinMesh::WORLD_OBJECT_SKIN_MESH_PLAY_TYPE)triggerMesh.m_iPlayType );			
		}
		break;
	case CKTDGLineMap::TAT_MESH_CHANGE_SPEED:
		{
			pWorldMesh->SetAnimSpeed( triggerMesh.m_fAnimSpeed );
		}
		break;
	default:
		break;
	}
}
void CX2World::DoTriggerLine(CKTDGLineMap::TRIGGER_DATA_LINE &triggerLine)
{
	triggerLine.m_bDone = true;

	if( triggerLine.m_iTriggerLineIndex < 0 )
		return;

	CKTDGLineMap::LineData *pLineData = GetLineMap()->AccessLineData( triggerLine.m_iTriggerLineIndex);
	if( pLineData == NULL )
		return;

	switch(triggerLine.m_iLineTriggerAction)
	{
	case CKTDGLineMap::TAT_LINE_CHANGE_ENABLE:
		{
			pLineData->bEnable	= triggerLine.m_bEnable;
		}
		break;
	case CKTDGLineMap::TAT_LINE_CHANGE_SPEED:
		{
			pLineData->m_fSpeed = triggerLine.m_fSpeed;
		}
		break;
	case CKTDGLineMap::TAT_LINE_CHANGE_JUMPSPEED:
		{
			pLineData->m_vJumpSpeed = triggerLine.m_vJumpSpeed;
		}
		break;
	case CKTDGLineMap::TAT_LINE_CHANGE_CAMERA:
		{
			pLineData->m_iCameraIndex = triggerLine.m_iCamera;
		}
		break;
	case CKTDGLineMap::TAT_LINE_CHANGE_CANPASS:
		{
			pLineData->m_bCanPass = triggerLine.m_bEnable;
		}
		break;
	case CKTDGLineMap::TAT_LINE_CHANGE_TELEPORT:
		{
			pLineData->m_iTeleportLineIndex = triggerLine.m_iCamera;
		}
		break;
	default:
		break;
	}
}

void CX2World::DoTriggerEtc(CKTDGLineMap::TRIGGER_DATA_ETC &triggerEtc)
{
	triggerEtc.m_bDone = true;

	switch(triggerEtc.m_iEtcTriggerAction)
	{
	case CKTDGLineMap::TAT_ETC_PLAY_SOUND:
		{
			CKTDXDeviceSound* pDevice = g_pKTDXApp->GetDeviceManager()->OpenSound( triggerEtc.m_wstrActionName, 10, false );
			if( NULL != pDevice )
			{
				pDevice->Play( false, false );
			}
		}
		break;
	case CKTDGLineMap::TAT_ETC_DURATION_TIME:
		{
		}
		break;
	case CKTDGLineMap::TAT_ETC_CAMERA:
		{
			SetTriggerCamera(triggerEtc.m_iDummyValue01, triggerEtc.m_fDummyValue01);
		}
		break;
	case CKTDGLineMap::TAT_ETC_CAMERA_SHAKE:
		{
			if( g_pX2Game != NULL && g_pX2Game->GetX2Camera() != NULL )
			{
				if( triggerEtc.m_iDummyValue01 >= 0 )
				{
					g_pX2Game->GetX2Camera()->GetCamera().UpDownCrashCamera( (float)triggerEtc.m_iDummyValue01, triggerEtc.m_fDummyValue01 );
				}
				else
				{
					float fGap = (float)abs(triggerEtc.m_iDummyValue01);
					g_pX2Game->GetX2Camera()->GetCamera().LeftRightCrashCamera( fGap, triggerEtc.m_fDummyValue01 );
				}
			}
		}
		break;
	case CKTDGLineMap::TAT_ETC_ACTIVE_LAYER:
		{
#ifdef WORLD_LAYER
			for(UINT i=0; i<m_LayerList.size(); ++i)
			{
				CX2WorldLayer *pWorldLayer = m_LayerList[i];
				if( pWorldLayer != NULL )
				{
					int iLayerId = pWorldLayer->GetLayerId();
					if( iLayerId >= 0 && iLayerId == triggerEtc.m_iDummyValue01 )
					{
						pWorldLayer->FadeColor( pWorldLayer->GetColor(), triggerEtc.m_fDummyValue01 );
						pWorldLayer->SetShowObject(true);
					}
				}
			}
#endif
		}
		break;
	default:
		break;
	}
}

void CX2World::DoTriggerFrameMove( double fTime, float fElapsedTime )
{
	if( GetLineMap() == NULL )
		return;

	CKTDGLineMap::LINEMAP_TRIGGER *lineMapTrigger = GetLineMap()->GetLineMapTrigger();
	if( lineMapTrigger->m_vecNowTrigger.size() <= 0 || lineMapTrigger->m_vecTriggerSeg.size() <= 0 )
		return;
	
	for(UINT iActiveTrigger = 0; iActiveTrigger<lineMapTrigger->m_vecNowTrigger.size(); ++iActiveTrigger)
	{	
		bool bTriggerDataDone = true;
		int iActiveTriggerIndex = lineMapTrigger->m_vecNowTrigger[iActiveTrigger];
		CKTDGLineMap::TRIGGER_SEGMENT &triggerSeg = lineMapTrigger->m_vecTriggerSeg[iActiveTriggerIndex];

		if( triggerSeg.m_iTriggerDataIndex < 0 )
		{
			lineMapTrigger->m_vecNowTrigger.erase( lineMapTrigger->m_vecNowTrigger.begin() + iActiveTrigger );
			--iActiveTrigger;			
			continue;
		}

		CKTDGLineMap::TRIGGER_DATA &triggerData = triggerSeg.m_vecTriggerData[triggerSeg.m_iTriggerDataIndex];

		triggerData.m_fElapsedTime += fElapsedTime;

		UINT iTriggerDataIndex = 0;
		for(iTriggerDataIndex = 0; iTriggerDataIndex<triggerData.m_vecTriggerMesh.size(); ++iTriggerDataIndex)
		{
			CKTDGLineMap::TRIGGER_DATA_MESH &trigger = triggerData.m_vecTriggerMesh[iTriggerDataIndex];
			if( trigger.m_bDone == false &&
				triggerData.m_fElapsedTime >= trigger.m_fTriggerTime &&
				trigger.m_iMeshTriggerAction > CKTDGLineMap::TAT_MESH_NONE )
			{
				DoTriggerMesh(trigger);
			}
			bTriggerDataDone = bTriggerDataDone && trigger.m_bDone;
		}
		for(iTriggerDataIndex = 0; iTriggerDataIndex<triggerData.m_vecTriggerLine.size(); ++iTriggerDataIndex)
		{
			CKTDGLineMap::TRIGGER_DATA_LINE &trigger = triggerData.m_vecTriggerLine[iTriggerDataIndex];
			if( trigger.m_bDone == false &&
				triggerData.m_fElapsedTime >= trigger.m_fTriggerTime &&
				trigger.m_iLineTriggerAction > CKTDGLineMap::TAT_LINE_NONE )
			{
				DoTriggerLine(trigger);
			}
			bTriggerDataDone = bTriggerDataDone && trigger.m_bDone;	
		}
		for(iTriggerDataIndex = 0; iTriggerDataIndex<triggerData.m_vecTriggerEtc.size(); ++iTriggerDataIndex)
		{
			CKTDGLineMap::TRIGGER_DATA_ETC &trigger = triggerData.m_vecTriggerEtc[iTriggerDataIndex];
			if( trigger.m_bDone == false &&
				triggerData.m_fElapsedTime >= trigger.m_fTriggerTime &&
				trigger.m_iEtcTriggerAction > CKTDGLineMap::TAT_ETC_NONE )
			{
				DoTriggerEtc(trigger);
			}
			bTriggerDataDone = bTriggerDataDone && trigger.m_bDone;
		}

		if( bTriggerDataDone == true )
		{
			InitTriggerDone(triggerData);
			lineMapTrigger->m_vecNowTrigger.erase( lineMapTrigger->m_vecNowTrigger.begin() + iActiveTrigger );
			--iActiveTrigger;			
			if( triggerSeg.m_iTriggerDataIndex < (int)triggerSeg.m_vecTriggerData.size() - 1)
			{
				++triggerSeg.m_iTriggerDataIndex;
			}
			else
			{
				if( triggerSeg.m_bLoop == true )
					triggerSeg.m_iTriggerDataIndex = 0;
				else
					triggerSeg.m_iTriggerDataIndex = -1;
			}			
		}
	}
}

void CX2World::InitTriggerDone(CKTDGLineMap::TRIGGER_DATA &triggerData, bool bDone)
{
	triggerData.m_fElapsedTime = 0;	
	
	UINT iTriggerDataIndex = 0;
	for(iTriggerDataIndex = 0; iTriggerDataIndex<triggerData.m_vecTriggerMesh.size(); ++iTriggerDataIndex)
	{
		CKTDGLineMap::TRIGGER_DATA_MESH &trigger = triggerData.m_vecTriggerMesh[iTriggerDataIndex];
		trigger.m_bDone = bDone;
	}
	for(iTriggerDataIndex = 0; iTriggerDataIndex<triggerData.m_vecTriggerLine.size(); ++iTriggerDataIndex)
	{
		CKTDGLineMap::TRIGGER_DATA_LINE &trigger = triggerData.m_vecTriggerLine[iTriggerDataIndex];
		trigger.m_bDone = bDone;
	}
	for(iTriggerDataIndex = 0; iTriggerDataIndex<triggerData.m_vecTriggerEtc.size(); ++iTriggerDataIndex)
	{
		CKTDGLineMap::TRIGGER_DATA_ETC &trigger = triggerData.m_vecTriggerEtc[iTriggerDataIndex];
		trigger.m_bDone = bDone;	
	}
}
#endif //WORLD_TRIGGER


#ifdef MODIFY_FRUSTUM
void CX2World::SetProjection( float fNear, float fFar, float fPerspective, float fBaseDistance, float fStepDist1, float fStepDist2, float fStepDist3 )
{
	g_pKTDXApp->GetDGManager()->SetNear( fNear );
	g_pKTDXApp->GetDGManager()->SetFar( fFar );
	g_pKTDXApp->GetDGManager()->SetPerspectiveValue( fPerspective );

	m_fBaseCameraDistance = fBaseDistance;
	m_fCameraDistanceStep[0] = fStepDist1;
	m_fCameraDistanceStep[1] = fStepDist2;
	m_fCameraDistanceStep[2] = fStepDist3;
}
#endif //MODIFY_FRUSTUM

