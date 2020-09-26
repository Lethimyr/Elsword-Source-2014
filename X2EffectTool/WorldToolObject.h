#pragma once

class CWorldToolObject
{
public:
	CWorldToolObject( CX2WorldObject* pWorldObject, CKTDGSkyDome* pSkyDome ) 
	{
		m_pWorldObject	= pWorldObject;
		m_pSkyDome		= pSkyDome;
		m_bAdditiveBlend = false;

		m_fMaxOccasionalEventTime = 0.f;
		m_fMinOccasionalEventTime = 0.f;
		m_iMaxOccasionalPlayRate = 0;
		m_iMinOccasionalPlayRate = 0;

		if( NULL != m_pWorldObject )
		{
			switch( m_pWorldObject->GetObjectType() )
			{
			case CX2WorldObject::OT_SKIN_MESH:
				{
					CX2WorldObjectSkinMesh* pSkinMesh = (CX2WorldObjectSkinMesh*)m_pWorldObject;
					m_fMinOccasionalEventTime = pSkinMesh->GetOccasionalEventTime();
					m_fMaxOccasionalEventTime = pSkinMesh->GetOccasionalEventTime();

					m_iMinOccasionalPlayRate	= pSkinMesh->GetOccasionalPlayRate();
					m_iMaxOccasionalPlayRate	= pSkinMesh->GetOccasionalPlayRate();

				} break;
			}
		}
	}

	virtual ~CWorldToolObject()
	{
		m_pWorldObject	= NULL;
		m_pSkyDome		= NULL;
	}


	void ReleaseSkyDome()
	{
		if( NULL == m_pSkyDome )
			return;

		vector<CKTDGSkyDome*>& vecpSkyDomeList = g_pX2Game->GetWorld()->GetSkyDomeList();
		for( UINT i=0; i<vecpSkyDomeList.size(); i++ )
		{
			CKTDGSkyDome* pSkyDome = vecpSkyDomeList.at(i);
			if( pSkyDome == m_pSkyDome )
			{
				vecpSkyDomeList.erase( vecpSkyDomeList.begin()+i );
				CKTDGSkyDome::DeleteKTDGObject( m_pSkyDome );
				//SAFE_DELETE( m_pSkyDome );

				break;
			}
		}
	}

	void ReleaseWorldObject()
	{
		if( NULL == m_pWorldObject )
			return;

		g_pX2Game->GetWorld()->RemoveWorldObject( m_pWorldObject );
	}

	void CreateSkyDome( const WCHAR* wszFileName )
	{
		CX2World* pWorld = g_pX2Game->GetWorld();

		string strFileName;
		ConvertWCHARToChar( strFileName, wszFileName );

		m_pSkyDome = pWorld->CreateSkyDome();
		m_pSkyDome->SetDomeMesh_LUA( strFileName.c_str() );
		m_pSkyDome->SetPos( 0, 0, 0 );
		m_pSkyDome->SetLayer( XL_SKY_DOME_0 );
		m_pSkyDome->SetRotationSpeed( 0 );
		m_pSkyDome->SetAlphaObject( false );
		m_pSkyDome->SetViewTrace( false );
		m_pSkyDome->SetTex0UVSpeed( 0.0, 0.0 );
		m_pSkyDome->SetTex1UVSpeed( 0.0, 0.0 );
		m_pSkyDome->SetTex2UVSpeed( 0.0, 0.0 );
		CKTDGXRenderer::RenderParam* pRenderParam = m_pSkyDome->GetRenderParam();
		pRenderParam->bAlphaBlend = false;
	}

	void CreateWorldObject( const WCHAR* wszFileName, CX2WorldObject::OBJECT_TYPE iObjType )
	{
		CX2World* pWorld = g_pX2Game->GetWorld();

		string strFileName;
		ConvertWCHARToChar( strFileName, wszFileName );

		switch( iObjType )
		{
		case CX2WorldObject::OT_STATIC_MESH:
			{
				CX2WorldObjectMesh* pObjectMesh = pWorld->CreateObjectMesh();
				pObjectMesh->SetXMesh_LUA( strFileName.c_str() );
				pObjectMesh->SetXMeshLOD_LUA( strFileName.c_str() );
				//{{ 09.02.27 김태완 : SetCanHide 기본값 false로 일괄적용
				pObjectMesh->SetCanHide( false );	
				//}}
				pObjectMesh->SetPos( D3DXVECTOR3(0,0,0) );
				pObjectMesh->SetColorFactor( D3DXCOLOR(1,1,1,1) );
				pObjectMesh->SetLayer( XL_SKY_WORLD_BASE_0 );
				m_pWorldObject = (CX2WorldObject*) pObjectMesh;
			} break;

		case CX2WorldObject::OT_SKIN_MESH:
			{
				CX2WorldObjectSkinMesh* pSkinMesh = pWorld->CreateObjectSkinMesh();

				pSkinMesh->SetXSkinMotion_LUA( strFileName.c_str() );
				pSkinMesh->AddXSkinModel_LUA( strFileName.c_str() );

				pSkinMesh->SetPos( D3DXVECTOR3(0,0,0) );
				pSkinMesh->SetLayer( XL_SKY_WORLD_BASE_1 );
				//{{ 09.02.27 김태완 : SetCanHide 기본값 false로 일괄적용
				pSkinMesh->SetCanHide( false );
				//}}

				pSkinMesh->SetColorFactor( D3DXCOLOR(1,1,1,1) );
				pSkinMesh->SetAngleDegree( D3DXVECTOR3(0, 0, 0) );
				pSkinMesh->SetAlphaObject( true );
				CKTDGXRenderer::RenderParam* pRenderParam = pSkinMesh->GetRenderParam();
				pRenderParam->bAlphaBlend = true;	

				pSkinMesh->AddAnim_LUA( "Anim01" );
				pSkinMesh->PlayAnim_LUA( CX2WorldObjectSkinMesh::WOSMPY_LOOP );
				m_pWorldObject = (CX2WorldObject*) pSkinMesh;
			} break;

		case CX2WorldObject::OT_PARTICLE:
			{
			} break;

		case CX2WorldObject::OT_LIGHT_FLOW:
			{

			} break;
		case CX2WorldObject::OT_REACT_MESH:
			{
			} break;
		}
	}

	CKTDGMatrix GetMatrix()
	{
		if( NULL != m_pWorldObject )
		{
			return m_pWorldObject->GetMatrix();
		}
		else if( NULL != m_pSkyDome )
		{
			return m_pSkyDome->GetMatrix();
		}
		else
		{
			return NULL;
		}
	}

	wstring GetMeshFileName()
	{
		wstring wstrName;

		if( NULL != m_pWorldObject )
		{
			switch( m_pWorldObject->GetObjectType() )
			{
			case CX2WorldObject::OT_STATIC_MESH:
				{ 
					CX2WorldObjectMesh* pStaticMesh = (CX2WorldObjectMesh*) m_pWorldObject;
					wstrName = pStaticMesh->GetXMeshName();
				} break;

			case CX2WorldObject::OT_SKIN_MESH:
				{
					CX2WorldObjectSkinMesh* pSkinMesh = (CX2WorldObjectSkinMesh*) m_pWorldObject;
					wstrName = pSkinMesh->GetXSkinMeshName();
				} break;

				//case CX2WorldObject::OT_REACT_MESH:
				//	{
				//		CX2WorldObjectReactMesh* pReactMesh = (CX2WorldObjectReactMesh*) m_pWorldObject;
				//		wstrName = pReactMesh->GetXSkinMeshName();
				//	} break;

			case CX2WorldObject::OT_LIGHT_FLOW:
				{
					CX2WorldObjectLightFlow* pLightFlow = (CX2WorldObjectLightFlow*) m_pWorldObject;
					wstrName = pLightFlow->GetXMeshName();
				} break;

			case CX2WorldObject::OT_PARTICLE:
				{
					wstrName = L"PARTICLE";
				} break;

			default:
				{
					wstrName = L"UNKNOWN";
				} break;
			}

		}
		else if( NULL != m_pSkyDome )
		{
			wstrName = ((CKTDXDevice*)m_pSkyDome->GetXMesh())->GetDeviceID();

		}

		return wstrName;		
	}

	const bool GetIsFogObject()
	{
		if( NULL != m_pWorldObject )
		{
			switch( m_pWorldObject->GetObjectType() )
			{
			case CX2WorldObject::OT_STATIC_MESH:
				{
					CX2WorldObjectMesh* pWorldMesh = (CX2WorldObjectMesh*) m_pWorldObject;
					return pWorldMesh->GetRenderParam()->bFog;
				} break;

			case CX2WorldObject::OT_SKIN_MESH:
				{
					CX2WorldObjectSkinMesh* pWorldMesh = (CX2WorldObjectSkinMesh*) m_pWorldObject;
					return pWorldMesh->GetRenderParam()->bFog;
				} break;            
			}
		}
		else if( NULL != m_pSkyDome )
		{
			return m_pSkyDome->GetRenderParam()->bFog;
		}

		return false;

	}

	const bool GetIsAlphaObject()
	{
		if( NULL != m_pWorldObject )
		{
			return m_pWorldObject->IsAlphaObject();
		}
		else if( NULL != m_pSkyDome )
		{
			return m_pSkyDome->IsAlphaObject();
		}
		else
		{
			//{{ 09.02.27 김태완 : SetIsAlphaObject에는 있는데 GetIsAlphaObject에는 없는 부분(renderparam관련) 추가
			CKTDGXRenderer::RenderParam* pRenderParam = GetRenderParam();
			return pRenderParam->bAlphaBlend;

			//return false;
			//}}
		}
	}

	const bool GetIsAlphaTestObject()
	{
		if( NULL != m_pWorldObject )
		{
			return m_pWorldObject->GetRenderParam()->bAlphaTestEnable;
		}
		else if( NULL != m_pSkyDome )
		{
			return m_pSkyDome->GetRenderParam()->bAlphaTestEnable;
		}
		else
		{
			//{{ 09.02.27 김태완 : SetIsAlphaObject에는 있는데 GetIsAlphaObject에는 없는 부분(renderparam관련) 추가
			CKTDGXRenderer::RenderParam* pRenderParam = GetRenderParam();
			return pRenderParam->bAlphaTestEnable;

			//return false;
			//}}
		}
	}


	void SetCanHide( bool bCanHide )
	{
		if( NULL != m_pWorldObject )
		{
			switch( m_pWorldObject->GetObjectType() )
			{
			case CX2WorldObject::OT_STATIC_MESH:
				{
					CX2WorldObjectMesh* pWorldMesh = (CX2WorldObjectMesh*) m_pWorldObject;
					pWorldMesh->SetCanHide( bCanHide, bCanHide );
				} break;

			case CX2WorldObject::OT_SKIN_MESH:
				{
					CX2WorldObjectSkinMesh* pWorldMesh = (CX2WorldObjectSkinMesh*) m_pWorldObject;
					pWorldMesh->SetCanHide( bCanHide, bCanHide );
				} break;
				//case CX2WorldObject::OT_PARTICLE:
				//case CX2WorldObject::OT_LIGHT_FLOW:
				//case CX2WorldObject::OT_REACT_MESH:

			default:
				{
				} break;
			}
		}
		else if( NULL != m_pSkyDome )
		{
		}
	}

	const bool GetCanHide()
	{
		if( NULL != m_pWorldObject )
		{
			switch( m_pWorldObject->GetObjectType() )
			{
			case CX2WorldObject::OT_STATIC_MESH:
				{
					CX2WorldObjectMesh* pWorldMesh = (CX2WorldObjectMesh*) m_pWorldObject;
					return pWorldMesh->GetCanHide();
				} break;

			case CX2WorldObject::OT_SKIN_MESH:
				{
					CX2WorldObjectSkinMesh* pWorldMesh = (CX2WorldObjectSkinMesh*) m_pWorldObject;
					return pWorldMesh->GetCanHide();

				} break;
				//case CX2WorldObject::OT_PARTICLE:
				//case CX2WorldObject::OT_LIGHT_FLOW:
				//case CX2WorldObject::OT_REACT_MESH:

			default:
				{
					return false;
				} break;
			}
		}
		else if( NULL != m_pSkyDome )
		{
			return false;
		}
		else
		{
			return false;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	void SetNotRender( bool bCanHide )
	{
		if( NULL != m_pWorldObject )
		{
			switch( m_pWorldObject->GetObjectType() )
			{
			case CX2WorldObject::OT_STATIC_MESH:
				{
					CX2WorldObjectMesh* pWorldMesh = (CX2WorldObjectMesh*) m_pWorldObject;
					pWorldMesh->SetNotRenderWhenHidden( bCanHide );
				} break;

			default:
				{
				} break;
			}
		}
		else if( NULL != m_pSkyDome )
		{
		}
	}

	const bool GetNotRender()
	{
		if( NULL != m_pWorldObject )
		{
			switch( m_pWorldObject->GetObjectType() )
			{
			case CX2WorldObject::OT_STATIC_MESH:
				{
					CX2WorldObjectMesh* pWorldMesh = (CX2WorldObjectMesh*) m_pWorldObject;
					return pWorldMesh->GetNotRenderWhenHidden();
				} break;

			default:
				{
					return false;
				} break;
			}
		}
		else if( NULL != m_pSkyDome )
		{
			return false;
		}
		else
		{
			return false;
		}
	}
	//////////////////////////////////////////////////////////////////////////

	void SetIsFog( bool bIsFog )
	{
		CKTDGXRenderer::RenderParam* pRenderParam = GetRenderParam();    

		if(pRenderParam == NULL)
			return;

		pRenderParam->bFog = bIsFog;

		if( NULL != m_pWorldObject )
		{
			switch( m_pWorldObject->GetObjectType() )
			{
			case CX2WorldObject::OT_STATIC_MESH:
				{
					CX2WorldObjectMesh* pWorldMesh = (CX2WorldObjectMesh*) m_pWorldObject;
					pWorldMesh->GetRenderParam()->bFog = bIsFog;
				} break;

			case CX2WorldObject::OT_SKIN_MESH:
				{
					CX2WorldObjectSkinMesh* pWorldMesh = (CX2WorldObjectSkinMesh*) m_pWorldObject;
					pWorldMesh->GetRenderParam()->bFog = bIsFog;
				} break;            
			}
		}
		else if( NULL != m_pSkyDome )
		{
			m_pSkyDome->GetRenderParam()->bFog = bIsFog;
		}
	}

	void SetFogNearX( float nearx )
	{
		CKTDGXRenderer::RenderParam* pRenderParam = GetRenderParam();    

		if(pRenderParam == NULL)
			return;

		if( NULL != m_pWorldObject )
		{
			switch( m_pWorldObject->GetObjectType() )
			{
			case CX2WorldObject::OT_STATIC_MESH:
				{
					CX2WorldObjectMesh* pWorldMesh = (CX2WorldObjectMesh*) m_pWorldObject;
					pWorldMesh->GetRenderParam()->fogNearX = nearx;
				} break;

			case CX2WorldObject::OT_SKIN_MESH:
				{
					CX2WorldObjectSkinMesh* pWorldMesh = (CX2WorldObjectSkinMesh*) m_pWorldObject;
					pWorldMesh->GetRenderParam()->fogNearX = nearx;
				} break;            
			}
		}
		else if( NULL != m_pSkyDome )
		{
			m_pSkyDome->GetRenderParam()->fogNearX = nearx;
		}
	}
	void SetFogFarX( float farx )
	{
		CKTDGXRenderer::RenderParam* pRenderParam = GetRenderParam();    

		if( NULL != m_pWorldObject )
		{
			switch( m_pWorldObject->GetObjectType() )
			{
			case CX2WorldObject::OT_STATIC_MESH:
				{
					CX2WorldObjectMesh* pWorldMesh = (CX2WorldObjectMesh*) m_pWorldObject;
					pWorldMesh->GetRenderParam()->fogFarX = farx;
				} break;

			case CX2WorldObject::OT_SKIN_MESH:
				{
					CX2WorldObjectSkinMesh* pWorldMesh = (CX2WorldObjectSkinMesh*) m_pWorldObject;
					pWorldMesh->GetRenderParam()->fogFarX = farx;
				} break;            
			}
		}
		else if( NULL != m_pSkyDome )
		{
			m_pSkyDome->GetRenderParam()->fogFarX = farx;
		}
	}
	void SetFogNearY( float neary )
	{
		CKTDGXRenderer::RenderParam* pRenderParam = GetRenderParam();    

		if( NULL != m_pWorldObject )
		{
			switch( m_pWorldObject->GetObjectType() )
			{
			case CX2WorldObject::OT_STATIC_MESH:
				{
					CX2WorldObjectMesh* pWorldMesh = (CX2WorldObjectMesh*) m_pWorldObject;
					pWorldMesh->GetRenderParam()->fogNearY = neary;
				} break;

			case CX2WorldObject::OT_SKIN_MESH:
				{
					CX2WorldObjectSkinMesh* pWorldMesh = (CX2WorldObjectSkinMesh*) m_pWorldObject;
					pWorldMesh->GetRenderParam()->fogNearY = neary;
				} break;            
			}
		}
		else if( NULL != m_pSkyDome )
		{
			m_pSkyDome->GetRenderParam()->fogNearY = neary;
		}
	}
	void SetFogFarY( float fary )
	{
		CKTDGXRenderer::RenderParam* pRenderParam = GetRenderParam();    

		if( NULL != m_pWorldObject )
		{
			switch( m_pWorldObject->GetObjectType() )
			{
			case CX2WorldObject::OT_STATIC_MESH:
				{
					CX2WorldObjectMesh* pWorldMesh = (CX2WorldObjectMesh*) m_pWorldObject;
					pWorldMesh->GetRenderParam()->fogFarY = fary;
				} break;

			case CX2WorldObject::OT_SKIN_MESH:
				{
					CX2WorldObjectSkinMesh* pWorldMesh = (CX2WorldObjectSkinMesh*) m_pWorldObject;
					pWorldMesh->GetRenderParam()->fogFarY = fary;
				} break;            
			}
		}
		else if( NULL != m_pSkyDome )
		{
			m_pSkyDome->GetRenderParam()->fogFarY = fary;
		}
	}
	void SetFogDensity( float density )
	{
		CKTDGXRenderer::RenderParam* pRenderParam = GetRenderParam();    

		if( NULL != m_pWorldObject )
		{
			switch( m_pWorldObject->GetObjectType() )
			{
			case CX2WorldObject::OT_STATIC_MESH:
				{
					CX2WorldObjectMesh* pWorldMesh = (CX2WorldObjectMesh*) m_pWorldObject;
					pWorldMesh->GetRenderParam()->fogDensity = density;
				} break;

			case CX2WorldObject::OT_SKIN_MESH:
				{
					CX2WorldObjectSkinMesh* pWorldMesh = (CX2WorldObjectSkinMesh*) m_pWorldObject;
					pWorldMesh->GetRenderParam()->fogDensity = density;
				} break;            
			}
		}
		else if( NULL != m_pSkyDome )
		{
			m_pSkyDome->GetRenderParam()->fogDensity = density;
		}
	}
	void SetFogColor( float rColor, float gColor, float bColor )
	{
		CKTDGXRenderer::RenderParam* pRenderParam = GetRenderParam();    

		if( NULL != m_pWorldObject )
		{
			switch( m_pWorldObject->GetObjectType() )
			{
			case CX2WorldObject::OT_STATIC_MESH:
				{
					CX2WorldObjectMesh* pWorldMesh = (CX2WorldObjectMesh*) m_pWorldObject;                    
					pWorldMesh->GetRenderParam()->fogColor = D3DXCOLOR(rColor, gColor, bColor, 1.0f);
				} break;

			case CX2WorldObject::OT_SKIN_MESH:
				{
					CX2WorldObjectSkinMesh* pWorldMesh = (CX2WorldObjectSkinMesh*) m_pWorldObject;
					pWorldMesh->GetRenderParam()->fogColor = D3DXCOLOR(rColor, gColor, bColor, 1.0f);
				} break;            
			}
		}
		else if( NULL != m_pSkyDome )
		{
			m_pSkyDome->GetRenderParam()->fogColor = D3DXCOLOR(rColor, gColor, bColor, 1.0f);
		}
	}

	float GetFogNearX()
	{
		CKTDGXRenderer::RenderParam* pRenderParam = GetRenderParam();    

		if( NULL != m_pWorldObject )
		{
			switch( m_pWorldObject->GetObjectType() )
			{
			case CX2WorldObject::OT_STATIC_MESH:
				{
					CX2WorldObjectMesh* pWorldMesh = (CX2WorldObjectMesh*) m_pWorldObject;
					return pWorldMesh->GetRenderParam()->fogNearX;
				} break;

			case CX2WorldObject::OT_SKIN_MESH:
				{
					CX2WorldObjectSkinMesh* pWorldMesh = (CX2WorldObjectSkinMesh*) m_pWorldObject;
					return pWorldMesh->GetRenderParam()->fogNearX;
				} break;            
			}
		}
		else if( NULL != m_pSkyDome )
		{
			return m_pSkyDome->GetRenderParam()->fogNearX;
		}

		//{{ 2009.02.25 김태완 : return이 없는 control path 수정, ASSERT
		ASSERT( !"GetFogNearX : WorldToolObject have no WorldObject" );
		return 0;
		//}}
	}
	float GetFogFarX()
	{
		CKTDGXRenderer::RenderParam* pRenderParam = GetRenderParam();    

		if( NULL != m_pWorldObject )
		{
			switch( m_pWorldObject->GetObjectType() )
			{
			case CX2WorldObject::OT_STATIC_MESH:
				{
					CX2WorldObjectMesh* pWorldMesh = (CX2WorldObjectMesh*) m_pWorldObject;
					return pWorldMesh->GetRenderParam()->fogFarX;
				} break;

			case CX2WorldObject::OT_SKIN_MESH:
				{
					CX2WorldObjectSkinMesh* pWorldMesh = (CX2WorldObjectSkinMesh*) m_pWorldObject;
					return pWorldMesh->GetRenderParam()->fogFarX;
				} break;            
			}
		}
		else if( NULL != m_pSkyDome )
		{
			return m_pSkyDome->GetRenderParam()->fogFarX;
		}
		//{{ 2009.02.25 김태완 : return이 없는 control path 수정, ASSERT
		ASSERT( !"GetFogFarX : WorldToolObject have no WorldObject" );
		return 0;
		//}}
	}
	float GetFogNearY()
	{
		CKTDGXRenderer::RenderParam* pRenderParam = GetRenderParam();    

		if( NULL != m_pWorldObject )
		{
			switch( m_pWorldObject->GetObjectType() )
			{
			case CX2WorldObject::OT_STATIC_MESH:
				{
					CX2WorldObjectMesh* pWorldMesh = (CX2WorldObjectMesh*) m_pWorldObject;
					return pWorldMesh->GetRenderParam()->fogNearY;
				} break;

			case CX2WorldObject::OT_SKIN_MESH:
				{
					CX2WorldObjectSkinMesh* pWorldMesh = (CX2WorldObjectSkinMesh*) m_pWorldObject;
					return pWorldMesh->GetRenderParam()->fogNearY;
				} break;            
			}
		}
		else if( NULL != m_pSkyDome )
		{
			return m_pSkyDome->GetRenderParam()->fogNearY;
		}
		//{{ 2009.02.25 김태완 : return이 없는 control path 수정, ASSERT
		ASSERT( !"GetFogNearY : WorldToolObject have no WorldObject" );
		return 0;
		//}}
	}
	float GetFogFarY()
	{
		CKTDGXRenderer::RenderParam* pRenderParam = GetRenderParam();    

		if( NULL != m_pWorldObject )
		{
			switch( m_pWorldObject->GetObjectType() )
			{
			case CX2WorldObject::OT_STATIC_MESH:
				{
					CX2WorldObjectMesh* pWorldMesh = (CX2WorldObjectMesh*) m_pWorldObject;
					return pWorldMesh->GetRenderParam()->fogFarY;
				} break;

			case CX2WorldObject::OT_SKIN_MESH:
				{
					CX2WorldObjectSkinMesh* pWorldMesh = (CX2WorldObjectSkinMesh*) m_pWorldObject;
					return pWorldMesh->GetRenderParam()->fogFarY;
				} break;            
			}
		}
		else if( NULL != m_pSkyDome )
		{
			return m_pSkyDome->GetRenderParam()->fogFarY;
		}
		//{{ 2009.02.25 김태완 : return이 없는 control path 수정, ASSERT
		ASSERT( !"GetFogFarY : WorldToolObject have no WorldObject" );
		return 0;
		//}}
	}
	float GetFogDensity()
	{
		CKTDGXRenderer::RenderParam* pRenderParam = GetRenderParam();    

		if( NULL != m_pWorldObject )
		{
			switch( m_pWorldObject->GetObjectType() )
			{
			case CX2WorldObject::OT_STATIC_MESH:
				{
					CX2WorldObjectMesh* pWorldMesh = (CX2WorldObjectMesh*) m_pWorldObject;
					return pWorldMesh->GetRenderParam()->fogDensity;
				} break;

			case CX2WorldObject::OT_SKIN_MESH:
				{
					CX2WorldObjectSkinMesh* pWorldMesh = (CX2WorldObjectSkinMesh*) m_pWorldObject;
					return pWorldMesh->GetRenderParam()->fogDensity;
				} break;            
			}
		}
		else if( NULL != m_pSkyDome )
		{
			return m_pSkyDome->GetRenderParam()->fogDensity;
		}
		//{{ 2009.02.25 김태완 : return이 없는 control path 수정, ASSERT
		ASSERT( !"GetFogDensity : WorldToolObject have no WorldObject" );
		return 0;
		//}}
	}

	void SetIsAlphaObject( bool bIsAlphaObject )
	{
		CKTDGXRenderer::RenderParam* pRenderParam = GetRenderParam();
		pRenderParam->bAlphaBlend = bIsAlphaObject;

		if( NULL != m_pWorldObject )
		{
			return m_pWorldObject->SetAlphaObject( bIsAlphaObject );
		}
		else if( NULL != m_pSkyDome )
		{
			return m_pSkyDome->SetAlphaObject( bIsAlphaObject );
		}
	}

	void SetIsAlphaTest( bool bIsAlphaTest )
	{
		CKTDGXRenderer::RenderParam* pRenderParam = GetRenderParam();
		pRenderParam->bAlphaTestEnable = bIsAlphaTest;
	}

	const bool GetAdditiveBlend() { return m_bAdditiveBlend; }
	void SetAdditiveBlend( bool bAddtiveBlend ) 
	{ 
		m_bAdditiveBlend = bAddtiveBlend; 

		CKTDGXRenderer::RenderParam* pRenderParam = GetRenderParam();	
		if( true == bAddtiveBlend )
		{
			pRenderParam->srcBlend	= D3DBLEND_SRCALPHA;
			pRenderParam->destBlend = D3DBLEND_DESTALPHA;
		}
		else
		{
			pRenderParam->srcBlend	= D3DBLEND_SRCALPHA;
			pRenderParam->destBlend = D3DBLEND_INVSRCALPHA;
		}
	}

	const int GetLayer()
	{
		if( NULL != m_pWorldObject )
		{
			return m_pWorldObject->GetLayer();
		}
		else if( NULL != m_pSkyDome )
		{
			return m_pSkyDome->GetLayer();
		}
		else
		{
			return -1;
		}
	}

	void SetLayer( int iLayer )
	{
		if( NULL != m_pWorldObject )
		{
			return m_pWorldObject->SetLayer( iLayer );
		}
		else if( NULL != m_pSkyDome )
		{
			return m_pSkyDome->SetLayer( iLayer );
		}	
	}

	const CX2WorldObject::OBJECT_TYPE GetObjectType()
	{
		if( NULL != m_pWorldObject )
		{
			return m_pWorldObject->GetObjectType();
		}
		else if( NULL != m_pSkyDome )
		{
			return (CX2WorldObject::OBJECT_TYPE) (CX2WorldObject::OT_REACT_MESH+1);
		}	
		else
		{
			return (CX2WorldObject::OBJECT_TYPE)-1;
		}

	}

	void SetObjectType( CX2WorldObject::OBJECT_TYPE iObjType )
	{
		switch( iObjType )
		{
		case CX2WorldObject::OT_STATIC_MESH:
		case CX2WorldObject::OT_SKIN_MESH:
		case CX2WorldObject::OT_PARTICLE:
		case CX2WorldObject::OT_LIGHT_FLOW:
		case CX2WorldObject::OT_REACT_MESH:
			{
				if( NULL == m_pWorldObject )					
				{
					CreateWorldObject( GetMeshFileName().c_str(), iObjType );
				}				

				if( iObjType != m_pWorldObject->GetObjectType() )
				{
					wstring wstrFileName = GetMeshFileName();
					ReleaseWorldObject();
					CreateWorldObject( wstrFileName.c_str(), iObjType );
				}

				if( NULL != m_pSkyDome )
				{
					ReleaseSkyDome();
				}
			} break;

		case CX2WorldObject::OT_REACT_MESH+1:
			{
				if( NULL == m_pSkyDome )
				{
					CreateSkyDome( GetMeshFileName().c_str() );
				}

				if( NULL != m_pWorldObject )
				{							
					ReleaseWorldObject();
				}						
			} break;
		}
	}



	D3DXVECTOR3 GetPosition()
	{
		if( NULL != m_pWorldObject )
		{
			switch( m_pWorldObject->GetObjectType() )
			{
			case CX2WorldObject::OT_PARTICLE:
				{
					CX2WorldObjectParticle* pParticle = (CX2WorldObjectParticle*) m_pWorldObject;
					CKTDGParticleSystem::CParticleEventSequence* pSeq = pParticle->GetParticleEventSequence();
					if( NULL != pSeq )
					{
						return pSeq->GetPosition();
					}
					else
					{
						return m_pWorldObject->GetMatrix().GetPos();
					}
				} break;

			default:
				{
					return m_pWorldObject->GetMatrix().GetPos();

				} break;
			}
		}
		else if( NULL != m_pSkyDome )
		{
			return m_pSkyDome->GetMatrix().GetPos();
		}

		return D3DXVECTOR3( 0, 0, 0 );
	}

	void SetPosition( D3DXVECTOR3 vPos )
	{
		if( NULL != m_pWorldObject )
		{
			switch( m_pWorldObject->GetObjectType() )
			{
			case CX2WorldObject::OT_PARTICLE:
				{
					CX2WorldObjectParticle* pParticle = (CX2WorldObjectParticle*) m_pWorldObject;
					CKTDGParticleSystem::CParticleEventSequence* pSeq = pParticle->GetParticleEventSequence();
					if( NULL != pSeq )
					{
						return pSeq->SetPosition( vPos );
					}
					else
					{
						return m_pWorldObject->GetMatrix().Move( vPos );
					}
				} break;

			default:
				{
					return m_pWorldObject->GetMatrix().Move( vPos );

				} break;
			}
		}
		else if( NULL != m_pSkyDome )
		{
			return m_pSkyDome->GetMatrix().Move( vPos );
		}

	}

	void AddPositionRelative( D3DXVECTOR3 vAddPos )
	{
		if( NULL == &GetMatrix() )
			return;

		GetMatrix().MoveRel( vAddPos );
	}

	void SetScale( D3DXVECTOR3 vScale )
	{
		if( NULL != m_pWorldObject )
		{
			m_pWorldObject->SetScale( vScale );
		}
		else if( NULL != m_pSkyDome )
		{
			m_pSkyDome->SetScale( vScale );
		}
	}

	D3DXVECTOR3 GetScale()
	{
		if( NULL != m_pWorldObject )
		{
			return (D3DXVECTOR3)m_pWorldObject->GetMatrix().GetScale();
		}
		else if( NULL != m_pSkyDome )
		{
			return (D3DXVECTOR3)m_pSkyDome->GetMatrix().GetScale();
		}
		return D3DXVECTOR3(1,1,1);
	}

	void SetRotationDegree( D3DXVECTOR3 vRotDegree )
	{
		if( NULL != m_pWorldObject )
		{
			m_pWorldObject->SetAngleDegree( vRotDegree );
		}
		else if( NULL != m_pSkyDome )
		{
			// nothing
		}
	}

	D3DXVECTOR3 GetRotationDegree()
	{
		if( NULL != m_pWorldObject )
		{
			return m_pWorldObject->GetMatrix().GetRotateDegree();
		}

		return D3DXVECTOR3( 0, 0, 0 );
	}

	D3DXVECTOR2 GetTex0UVSpeed()
	{
		if( NULL != m_pWorldObject )
		{
			switch( m_pWorldObject->GetObjectType() )
			{
			case CX2WorldObject::OT_STATIC_MESH:
				{
					return ((CX2WorldObjectMesh*)m_pWorldObject)->GetTex0UVSpeed();
				} break;

			case CX2WorldObject::OT_SKIN_MESH:
				{
					return ((CX2WorldObjectSkinMesh*)m_pWorldObject)->GetTex0UVSpeed(); 
				} break;
			}
		}
		else if( NULL != m_pSkyDome )
		{
			return m_pSkyDome->GetTex0UVSpeed();
		}

		return D3DXVECTOR2(0, 0);
	}

	void SetTex0UVSpeed( float fSpeedX, float fSpeedY )
	{
		if( NULL != m_pWorldObject )
		{
			switch( m_pWorldObject->GetObjectType() )
			{
			case CX2WorldObject::OT_STATIC_MESH:
				{
					((CX2WorldObjectMesh*)m_pWorldObject)->SetTex0UVSpeed( fSpeedX, fSpeedY );
				} break;

			case CX2WorldObject::OT_SKIN_MESH:
				{
					((CX2WorldObjectSkinMesh*)m_pWorldObject)->SetTex0UVSpeed( fSpeedX, fSpeedY );
				} break;
			}
		}
		else if( NULL != m_pSkyDome )
		{
			m_pSkyDome->SetTex0UVSpeed( fSpeedX, fSpeedY );
		}
	}

	void SetRotationSpeed( float fRotSpeed )
	{
		if( NULL == m_pSkyDome )
			return; 

		m_pSkyDome->SetRotationSpeed( fRotSpeed );
	}

	CKTDGXRenderer::RenderParam* GetRenderParam()
	{ 
		if( NULL != m_pWorldObject )
		{
			switch( m_pWorldObject->GetObjectType() )
			{
			case CX2WorldObject::OT_STATIC_MESH:
				{
					CX2WorldObjectMesh* pObjectMesh = (CX2WorldObjectMesh*)m_pWorldObject;
					return pObjectMesh->GetRenderParam();
				} break;

			case CX2WorldObject::OT_SKIN_MESH:
				{
					CX2WorldObjectSkinMesh* pSkinMesh = (CX2WorldObjectSkinMesh*) m_pWorldObject;
					return pSkinMesh->GetRenderParam();
				} break;

			case CX2WorldObject::OT_PARTICLE:
				return NULL;
				break;

			case CX2WorldObject::OT_LIGHT_FLOW:
				{ 
					CX2WorldObjectLightFlow* pLightFlow = (CX2WorldObjectLightFlow*) m_pWorldObject;
					return pLightFlow->GetRenderParam();
				} break;

			case CX2WorldObject::OT_REACT_MESH:
				return NULL;
				break;
			}
		}
		else if( NULL != m_pSkyDome )
		{
			return m_pSkyDome->GetRenderParam();
		}

		return NULL;
	}

	bool GetBoundingSphere( D3DXVECTOR3& vCenter, float& fRadius )
	{

		if( NULL != m_pWorldObject )
		{
			vCenter	= m_pWorldObject->GetCenter();
			fRadius = m_pWorldObject->GetBoundingRadius();
			return true;
		}
		else if( NULL != m_pSkyDome )
		{
			vCenter = m_pSkyDome->GetCenter();
			fRadius = m_pSkyDome->GetBoundingRadius();
			return true;
		}

		return false;
	}


	wstring GetAnimationName()
	{
		if( NULL != m_pWorldObject )
		{
			switch( m_pWorldObject->GetObjectType() )
			{
			case CX2WorldObject::OT_SKIN_MESH:
				{
					vector<wstring>& wstrPlayList = ((CX2WorldObjectSkinMesh*)m_pWorldObject)->GetPlayList();
					if( wstrPlayList.size() > 0 )
					{
						return wstrPlayList[0];
					}
				} break;
			}
		}

		return L"";
	}


	void SetAnimationName( wstring wstrAniName )
	{
		if( NULL != m_pWorldObject )
		{
			switch( m_pWorldObject->GetObjectType() )
			{
			case CX2WorldObject::OT_SKIN_MESH:
				{
					CX2WorldObjectSkinMesh* pSkinMesh = (CX2WorldObjectSkinMesh*)m_pWorldObject;
					vector<wstring>& wstrPlayList = pSkinMesh->GetPlayList();
					wstrPlayList.clear();
					wstrPlayList.push_back( wstrAniName );
					pSkinMesh->PlayAnim_LUA( CX2WorldObjectSkinMesh::WOSMPY_LOOP );
				} break;
			}
		}
	}


	CX2WorldObjectSkinMesh::WORLD_OBJECT_SKIN_MESH_PLAY_TYPE GetPlayType() 
	{
		if( NULL != m_pWorldObject )
		{
			switch( m_pWorldObject->GetObjectType() )
			{
			case CX2WorldObject::OT_SKIN_MESH:
				{
					CX2WorldObjectSkinMesh* pSkinMesh = (CX2WorldObjectSkinMesh*)m_pWorldObject;
					return pSkinMesh->GetPlayType();
				} break;
			}
		}

		return CX2WorldObjectSkinMesh::WOSMPY_PINGPONG;
	}


	void SetPlayType( CX2WorldObjectSkinMesh::WORLD_OBJECT_SKIN_MESH_PLAY_TYPE ePlayType ) 
	{
		if( NULL != m_pWorldObject )
		{
			switch( m_pWorldObject->GetObjectType() )
			{
			case CX2WorldObject::OT_SKIN_MESH:
				{
					CX2WorldObjectSkinMesh* pSkinMesh = (CX2WorldObjectSkinMesh*)m_pWorldObject;
					pSkinMesh->PlayAnim_LUA( ePlayType );
				} break;
			}
		}

	}


	D3DXVECTOR2 GetOccasionalEventTime()
	{
		if( NULL != m_pWorldObject )
		{
			switch( m_pWorldObject->GetObjectType() )
			{
			case CX2WorldObject::OT_SKIN_MESH:
				{
					D3DXVECTOR2 vValue;
					vValue.x = m_fMinOccasionalEventTime;
					vValue.y = m_fMaxOccasionalEventTime;
					return vValue;

					//CX2WorldObjectSkinMesh* pSkinMesh = (CX2WorldObjectSkinMesh*)m_pWorldObject;
					//return pSkinMesh->GetOccasionalEventTime();
				} break;
			}
		}

		return D3DXVECTOR2(0, 0);
	}

	void SetOccasionalEventTime(float fMin, float fMax )
	{
		if( NULL != m_pWorldObject )
		{
			switch( m_pWorldObject->GetObjectType() )
			{
			case CX2WorldObject::OT_SKIN_MESH:
				{
					m_fMinOccasionalEventTime = fMin;
					m_fMaxOccasionalEventTime = fMax;

					CX2WorldObjectSkinMesh* pSkinMesh = (CX2WorldObjectSkinMesh*)m_pWorldObject;
					pSkinMesh->SetOccasionalEventTime( RandomFloat(fMin, fMax) );
				} break;
			}
		}
	}

	D3DXVECTOR2 GetOccasionalPlayRate() 
	{
		if( NULL != m_pWorldObject )
		{
			switch( m_pWorldObject->GetObjectType() )
			{
			case CX2WorldObject::OT_SKIN_MESH:
				{
					D3DXVECTOR2 vValue;
					vValue.x = (float)m_iMinOccasionalPlayRate;
					vValue.y = (float)m_iMaxOccasionalPlayRate;
					return vValue;

					//CX2WorldObjectSkinMesh* pSkinMesh = (CX2WorldObjectSkinMesh*)m_pWorldObject;
					//return pSkinMesh->GetOccasionalPlayRate();
				} break;
			}
		}

		return D3DXVECTOR2(0, 0);
	}

	void SetOccasionalPlayRate( int iMin, int iMax )
	{
		if( NULL != m_pWorldObject )
		{
			switch( m_pWorldObject->GetObjectType() )
			{
			case CX2WorldObject::OT_SKIN_MESH:
				{

					m_iMinOccasionalPlayRate = iMin;
					m_iMaxOccasionalPlayRate = iMax;

					CX2WorldObjectSkinMesh* pSkinMesh = (CX2WorldObjectSkinMesh*)m_pWorldObject;
					return pSkinMesh->SetOccasionalPlayRate( RandomInt( iMin, iMax ) );
				} break;
			}
		}

	}


	void SetUseWorldColor( bool bUseWorldColor )
	{
		if( NULL != m_pWorldObject )
		{
			m_pWorldObject->SetUseWorldColor( bUseWorldColor );
		}
	}

	bool GetUseWorldColor()
	{
		if( NULL != m_pWorldObject )
		{
			return m_pWorldObject->GetUseWorldColor();
		}
		return false;
	}
#ifdef COLORFACTOR_FOR_EVERY_WORLDMESH
	void SetOriginalColorFactor( D3DXCOLOR coColor )
	{
		if( NULL != m_pWorldObject )
		{
			return m_pWorldObject->SetOriginalColorFactor( coColor );
		}
	}
	D3DXCOLOR GetOriginalColorFactor()
	{
		if( NULL != m_pWorldObject )
		{
			return m_pWorldObject->GetOriginalColorFactor();
		}
		return D3DXCOLOR(1, 1, 1, 1);
	}
#endif
	void SetColorFactor( D3DXCOLOR coColor )
	{
		if( NULL != m_pWorldObject )
		{
			return m_pWorldObject->SetColorFactor( coColor );
		}

		if( NULL != m_pSkyDome )
		{
			return m_pSkyDome->SetColorFactor( coColor );
		}
	}

	D3DXCOLOR GetColorFactor()
	{
		if( NULL != m_pWorldObject )
		{
			return m_pWorldObject->GetColorFactor();
		}

		if( NULL != m_pSkyDome )
		{
			return D3DXCOLOR(1, 1, 1, 1);
		}
		return D3DXCOLOR(1, 1, 1, 1);
	}


	void Render()
	{
		if( NULL != m_pWorldObject )
		{
			m_pWorldObject->OnFrameRender();
		}
		else if( NULL != m_pSkyDome )
		{
			m_pSkyDome->OnFrameRender();
		}
	}

	//{{ 09.03.02 김태완
	float GetHideDistance()
	{
		if(NULL != m_pWorldObject )
		{
			return m_pWorldObject->GetHideDistance();
		}
		return 0;
	}
	void SetHideDistance(float fDist)
	{
		if(NULL != m_pWorldObject )
		{
			m_pWorldObject->SetHideDistance(fDist);
		}
	}
	bool GetForceShow()
	{
		if(NULL != m_pWorldObject )
		{
			return m_pWorldObject->GetForceShow();
		}
		return false;
	}
	void SetForceShow( bool bVal )
	{
		if(NULL != m_pWorldObject )
		{
			m_pWorldObject->SetForceShow( bVal );
		}
		return;
	}
	//}}

public:
	CX2WorldObject*		m_pWorldObject;
	CKTDGSkyDome*		m_pSkyDome;

	bool				m_bAdditiveBlend;

	float				m_fMaxOccasionalEventTime;
	float				m_fMinOccasionalEventTime;
	int					m_iMaxOccasionalPlayRate;
	int					m_iMinOccasionalPlayRate;

};