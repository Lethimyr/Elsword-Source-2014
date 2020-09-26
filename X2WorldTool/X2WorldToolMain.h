#pragma once
/********************************************************************
	created:	2006/12/04
	created:	4:12:2006   15:08
	filename: 	d:\projectX2\X2Project\X2WorldTool\X2WorldToolMain.h
	file path:	d:\projectX2\X2Project\X2WorldTool
	file base:	X2WorldToolMain
	file ext:	h
	author:		
	
	purpose:	
*********************************************************************/



// 
// class CDeviceReady : public CKTDXStage
// {
// public:
// 	void DeviceUnready( const WCHAR* wszDeviceID )
// 	{
// 		wstring wstrDeviceID = wszDeviceID;
// 		MakeUpperCase( wstrDeviceID );
// 		
// 		for( UINT i=0; i<m_pReadyDeviceList.size(); i++ )
// 		{
// 			CKTDXDevice* pDevice = m_pReadyDeviceList[i];
// 			if( 0 == pDevice->GetDeviceID().compare( wstrDeviceID ) )
// 			{
// 				SAFE_CLOSE( pDevice );
// 				m_pReadyDeviceList.erase( m_pReadyDeviceList.begin() + i );
// 				i -= 1;
// 			}
// 		}
// 	}
// 
// 	void DeviceUnreadyAll()
// 	{
// 		for( UINT i=0; i<m_pReadyDeviceList.size(); i++ )
// 		{
// 			CKTDXDevice* pDevice = m_pReadyDeviceList[i];
// 			SAFE_CLOSE( pDevice );
// 		}
// 		m_pReadyDeviceList.clear();
// 	}
// };


class CWorldToolGame : public CX2Game
{
public:
	CWorldToolGame(void);
	virtual ~CWorldToolGame(void);

	virtual void Release();

	virtual HRESULT			OnFrameMove( double fTime, float fElapsedTime );
	virtual HRESULT			OnFrameRender();

	virtual void			GameLoading( CX2Room* pRoom );
	virtual void			WorldLoading();
	virtual void			SystemLoading();
	virtual void			EtcLoading();
	//virtual bool			DeleteUserUnit( int index );
	virtual bool			DetermineLastKill() { return false;  }

	void ReloadWorld( const WCHAR* pwszFileName );
	void ResetUnitPosition();
    void SetCamera();


protected:
	void ReleaseCurrentWorld();
	void SquareCamera( CX2GUUser* pFocusUnit, float distance = 1200.0f, float eyeHeight = 200.0f, float lookAtHeight = 100.f, 
		float angleDegree = 0.0f, 
		float eyeDistance = 0.0f, float lookatDistance = 0.0f, float trackingTime = 0.3f );

public:
	struct SquareCameraParam
	{
		float	m_fLookAtHeight;
		float	m_fEyeHeight;
		float	m_fEyeDistance;

		SquareCameraParam()
		{
			m_fLookAtHeight = 100.f;
			m_fEyeHeight	= 200.f;
			m_fEyeDistance	= 1500.f;
		}
	};

public:
	CX2PVPRoom*	m_pPVPRoom;
	bool	m_bSquareCamera;
	bool    m_bOrthoFreeCamera;
	bool	m_bFixedCamera;

	float   m_fOrthoScale;

	SquareCameraParam m_SquareCameraParam;
};


class CSkinAnimObject
{
public:
	CSkinAnimObject(void);
	virtual ~CSkinAnimObject(void);

	virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
	virtual HRESULT OnFrameRender();
	//virtual HRESULT OnResetDevice();
	//virtual HRESULT OnLostDevice();

public:
	void	Clear();
	bool	SetAnimXSkinMesh( std::wstring fileName );
	//SKINMESH_OPEN_TYPE	InsertSkinMesh( std::wstring& fileName, std::wstring& dir );
	//bool				AddWeapon( const WCHAR* pFullFileName, const WCHAR* attachFrameName, D3DXVECTOR3 rot );
	//bool				DelModelXSkinMesh( std::wstring fileName );

	//void	Reset();
	void	ChangeAnim( std::wstring animName );
	void	ChangeAnim( int index );

	void	Play( CKTDGXSkinAnim::XSKIN_ANIM_PLAYTYPE playType );
	void	SetPos( const D3DXVECTOR3& vPos );
	D3DXVECTOR3 GetPos();

	void SetRotationDegree( const D3DXVECTOR3& vRotation );
	D3DXVECTOR3 GetRotationDegree();


	D3DXVECTOR3 GetMotionOffset() { return m_pXSkinAnim->GetMotionOffset(); }
	bool	IsPlaying() { return !m_pXSkinAnim->IsAnimationEnd(); }

	CKTDGXRenderer::RenderParam* GetRenderParam();

	void	SetShow( bool bShow	) { m_bShow = bShow; } 

	CKTDGXSkinAnim* GetXSkinAnim() const { return m_pXSkinAnim; }



	//void	SetScale( D3DXVECTOR3& vScale ){ m_vScale = vScale; }
	//void	SetScale( float fX, float fY, float fZ );
	//void	SetScaleX( float fX ){ m_vScale.x = fX; }
	//void	SetScaleY( float fY ){ m_vScale.y = fY; }
	//void	SetScaleZ( float fZ ){ m_vScale.z = fZ; }

	//void	SetLightPos( D3DXVECTOR3& vLightPos ){ m_RenderParam.lightPos = vLightPos; }
	//void	SetLightPos( float fX, float fY, float fZ );
	//void	SetLightPosX( float fX ){ m_RenderParam.lightPos.x = fX; }
	//void	SetLightPosY( float fY ){ m_RenderParam.lightPos.y = fY; }
	//void	SetLightPosZ( float fZ ){ m_RenderParam.lightPos.z = fZ; }

	//void	SetWireFrameMode( bool bWireFrame ){ m_bWireframeMode = bWireFrame; }

	//D3DXVECTOR3&	GetScale(){ return m_vScale; }
	//D3DXVECTOR3&	GetLightPos(){ return m_RenderParam.lightPos; }
	//void	SetLightOnOff( bool bIsLight ){ m_bIsLight = bIsLight; }

	bool	GetAnimNameList( std::vector<std::wstring>& vecAnimNameList );
	//bool	SetPlayOnOff();
	//void	SetMotionOnOff( bool bIsMotion );
	//bool	GetMotionOnOff(){ return m_bIsMotion; }
	//void	SetPlaySpeed( float fPlaySpeed );
	//void	GetAnimTime( float& fNowTime, float& fMaxTime );
	//void	SetPlayType( WCHAR* wszPlayType );
	//void	SetBounding( bool bIsBounding ){ m_bIsBounding = bIsBounding; }
	//bool	GetBounding(){ return m_bIsBounding; }
	//void	SetShowAttackBox( bool bShowAttackBox ) { m_bShowAttackBox = bShowAttackBox; }
	//bool	GetShowAttackBox() { return m_bShowAttackBox; }

	//CKTDGXRenderer::RenderParam*	GetRenderParam(){ return &m_RenderParam; }
	//IMPACT_DATA*					GetImpactData(){ return &m_ImpactData; }
	//std::vector<TEX_STAGE_DATA>*	GetTexStageData(){ return &m_vecTexStageData; }
	//CKTDGXSkinAnim::XSKIN_ANIM_PLAYTYPE	GetPlayType() { return m_AnimPlaytype; }
	//

public:
	CKTDGXSkinAnim*									m_pXSkinAnim;
	std::map<std::wstring, CKTDXDeviceXSkinMesh*>	m_mapSkinMesh;

	bool											m_bShow;
	int												m_iNowAniNameIndex;
	//CKTDXDeviceXET*									m_pXET;

	//D3DXVECTOR3			m_vScale;

	//bool				m_bWireframeMode;
	//bool				m_bIsLight;
	//bool				m_bIsAnimPlay;
	//bool				m_bIsMotion;
	//bool				m_bIsBounding;
	//bool				m_bShowAttackBox;

	//CKTDGXSkinAnim::XSKIN_ANIM_PLAYTYPE	m_AnimPlaytype;

	//CKTDGXRenderer::RenderParam	m_RenderParam;
	//std::vector<TEX_STAGE_DATA>	m_vecTexStageData;

	//D3DXMATRIX*			m_pMatrix;
	
};

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

//{{ 2009.02.25 김태완 : fix
// 예전 방식으로는 제대로 삭제 안 됨 : 벡터 복사 -> 삭제. World의 벡터에는 Object가 그대로 남아 있다
// 		vector<CX2WorldObject*> vecpWorldObjectList = g_pX2Game->GetWorld()->GetWorldObjectList();
// 		for( UINT i=0; i<vecpWorldObjectList.size(); i++ )
// 		{
// 			CX2WorldObject* pWorldObject = vecpWorldObjectList.at(i);
// 			if( pWorldObject == m_pWorldObject )
// 			{
// 				vecpWorldObjectList.erase( vecpWorldObjectList.begin()+i );
// 				SAFE_DELETE( m_pWorldObject );
// 				break;
// 			}
// 		}
		g_pX2Game->GetWorld()->RemoveWorldObject( m_pWorldObject );
//}}
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


class CWorldToolCamera : public CFirstPersonCamera
{
public:
	CWorldToolCamera()
	{
		m_fMoveSpeed = 1000.f;
	};
	~CWorldToolCamera() {};


	//const float GetMoveSpeed() { return m_fMoveSpeed; }
	//void IncMoveSpeed() { m_fMoveSpeed += 100.f; }
	//void DecMoveSpeed() { m_fMoveSpeed -= 100.f; if( m_fMoveSpeed < 0.f ) m_fMoveSpeed = 100.f; }	

public:
	float		m_fMoveSpeed;
};


class CWorldToolDialog : public CDXUTDialog
{
public:
	CWorldToolDialog()
	{
		m_bEnable = true;
	}
	~CWorldToolDialog() {};

	void SetEnable( bool bEnable ) { m_bEnable = bEnable; }
	const bool GetEnable() { return m_bEnable; }


private:
	bool	m_bEnable;
};



class CX2WorldToolMain : public CX2Main
{
public:
	class CCollision
	{
	public:
		static float PointToSegment( float& fLerpCoef, const D3DXVECTOR3* pvPoint, const D3DXVECTOR3* pvSegmentStart, const D3DXVECTOR3* pvSegmentEnd )
		{
			D3DXVECTOR3 vSegmentDir = *pvSegmentEnd - *pvSegmentStart;
			D3DXVECTOR3 vSegmentStartToPoint = *pvPoint - *pvSegmentStart;

			fLerpCoef = D3DXVec3Dot( &vSegmentDir, &vSegmentStartToPoint ) / D3DXVec3Dot( &vSegmentDir, &vSegmentDir );

			if( fLerpCoef < 0.f )
			{
				float fDistance = D3DXVec3Length( &(*pvSegmentStart - *pvPoint) );
				return fDistance;
			}
			else if( fLerpCoef > 1.f )
			{
				float fDistance = D3DXVec3Length( &(*pvSegmentEnd - *pvPoint) );
				return fDistance;
			}
			else
			{
				D3DXVECTOR3 vPointOnSegment = *pvSegmentStart + vSegmentDir * fLerpCoef;
				float fDistance = D3DXVec3Length( &(vPointOnSegment - *pvPoint) );
				return fDistance;
			}
		}


		static bool RayToPlane( float& fDistance, D3DXVECTOR3& vIntersection, 
			const D3DXVECTOR3& vRayOrigin, const D3DXVECTOR3& vRayDirection, 
			const D3DXVECTOR3& vPlaneVertex, const D3DXVECTOR3& vPlaneNormal )
		{
			// ray: P = P0 + t*V
			// plane: P.N + d = 0
			float VdotN = D3DXVec3Dot( &vRayDirection, &vPlaneNormal );
			float P0dotN = D3DXVec3Dot( &vRayOrigin, &vPlaneNormal );
			float d = -D3DXVec3Dot( &vPlaneVertex, &vPlaneNormal );
			if( VdotN == 0.f )
				return false;

			float t = -(P0dotN + d) / VdotN;
			fDistance = t;

			if( t < 0.f ) // no intersection
			{				
				return false;
			}
			else
			{				
				vIntersection = vRayOrigin + vRayDirection * t;
				return true;
			}
		}

		static bool RayToRect( float& fDistance, D3DXVECTOR3& vIntersection, 
			const D3DXVECTOR3& vRayOrigin, const D3DXVECTOR3& vRayDirection, 
			const D3DXVECTOR3& vRectVertex1, const D3DXVECTOR3& vRectVertex2, const D3DXVECTOR3& vRectNormal )
		{
			bool bResult = RayToPlane( fDistance, vIntersection, vRayOrigin, vRayDirection, vRectVertex1, vRectNormal);
			if( bResult == true )
			{
				D3DXVECTOR3 vTemp1 = vIntersection - vRectVertex1;
				D3DXVECTOR3 vTemp2 = vIntersection - vRectVertex2;
				float fIsInside = D3DXVec3Dot( &vTemp1, &vTemp2 );
				if( fIsInside < 0.f )
				{
					return true; 					
				}
			}
			return false;
		}		


		/*
		http://local.wasp.uwa.edu.au/~pbourke/geometry/lineline3d/
		Calculate the line segment PaPb that is the shortest route between
		two lines P1P2 and P3P4. Calculate also the values of mua and mub where
		Pa = P1 + mua (P2 - P1)
		Pb = P3 + mub (P4 - P3)
		Return FALSE if no solution exists.
		*/
		static bool LineToLine( D3DXVECTOR3& pa, D3DXVECTOR3& pb, 
			float& mua, float& mub,
			const D3DXVECTOR3& p1, const D3DXVECTOR3& p2,
			const D3DXVECTOR3& p3, const D3DXVECTOR3& p4,
			const float EPS = 0.000001f )
		{
			D3DXVECTOR3 p13, p43, p21;	
			float d1343,d4321,d1321,d4343,d2121; // dot products
			float numer,denom;

			p43 = p4 - p3;			
			if( fabs(p43.x) < EPS && fabs(p43.y) < EPS && fabs(p43.z) < EPS )
				return(false);			
			p21 = p2 - p1;
			if( fabs(p21.x) < EPS && fabs(p21.y) < EPS && fabs(p21.z) < EPS)
				return(false);
			p13 = p1 - p3;

			d1343 = D3DXVec3Dot(&p13, &p43);
			d4321 = D3DXVec3Dot(&p43, &p21);
			d1321 = D3DXVec3Dot(&p13, &p21);
			d4343 = D3DXVec3Dot(&p43, &p43);
			d2121 = D3DXVec3Dot(&p21, &p21);

			denom = d2121 * d4343 - d4321 * d4321;
			if( fabs(denom) < EPS)
				return(false);
			numer = d1343 * d4321 - d1321 * d4343;

			mua = numer / denom;
			mub = (d1343 + d4321 * mua) / d4343;

			pa = p1 + p21 * mua;
			pb = p3 + p43 * mub;

			return(true);
		}


	};


public:
	enum LINEMAP_FILENAME_TYPE
	{
		LFT_PREFIX,
		LFT_SUFFIX,
	};

	enum TOOL_MODE
	{
		MODE_WORLDTOOL,
		MODE_EFFECTTOOL,
		MODE_GAMEPLAY,
	};	

	enum DIALOG_ID
	{
		DLG_MAIN			= 0,
		DLG_OBJECT_LIST,
		DLG_OBJECT_PROPERTY,
		DLG_EFFECT_TOOL,
		DLG_EFFECT_PROPERTY,
		DLG_EFFECT_TOOL2, 
		DLG_ETC,
	};

	enum DIALOG_CONTROL_ID
	{
		IDC_STATIC_DUMMY,
		IDC_BUTTON_HELP,

		IDC_BUTTON_FILE_OPEN_DUNGEON,
		IDC_BUTTON_FILE_OPEN_WORLD,
		IDC_BUTTON_FILE_OPEN_LINEMAP,

		IDC_BUTTON_FILE_SAVE_DUNGEON,
		IDC_BUTTON_FILE_SAVE_WORLD,
		IDC_BUTTON_FILE_SAVE_LINEMAP,

		IDC_COMBO_LINEMAP_FILENAME,

		IDC_CHECK_SQUARE_CAMERA,
		IDC_EDIT_SQUARE_CAMERA_DISTANCE,
		IDC_EDIT_SQUARE_CAMERA_LOOKAT_HEIGHT,
		IDC_EDIT_SQUARE_CAMERA_EYE_HEIGHT,
		IDC_EDIT_CAMERA_MOVE_SPEED,
		IDC_EDIT_OBJECT_MOVE_SPEED,

		IDC_LIST_OBJECT_PALETTE,
		IDC_LIST_OBJECT_IN_WORLD,
		IDC_BUTTON_ADD_OBJECT_TO_WORLD_AS_STATIC_MESH,
		IDC_BUTTON_ADD_OBJECT_TO_WORLD_AS_SKIN_MESH,
		IDC_BUTTON_RELOAD_OBJECT_IN_WORLD,
		IDC_BUTTON_CLEAR_OBJECT_PALETTE,
		IDC_BUTTON_REMOVE_OBJECT_FROM_WORLD,
		IDC_CHECK_RENDER_BOUNDING_SPHERE,
		IDC_CHECK_REDNER_FLICKER,
		IDC_CHECK_ENABLE_CENTER_GREEN_DOT,
		


		IDC_EDIT_POS_X,				// object 위치
		IDC_EDIT_POS_Y,
		IDC_EDIT_POS_Z,
		IDC_EDIT_SCALE_X,
		IDC_EDIT_SCALE_Y,
		IDC_EDIT_SCALE_Z,
		IDC_EDIT_ROTATION_DEGREE,

		IDC_EDIT_OCCATIONAL_EVENT_TIME,
		IDC_EDIT_OCCATIONAL_EVENT_RATE,

		IDC_EDIT_ROTATION_SPEED,
		IDC_EDIT_TEX0_UV_SPEED,
		IDC_EDIT_SKIN_ANIM_NAME,

#ifdef ADD_ALPHATESTENABLE
		IDC_CHECK_IS_NOT_CULL,
		IDC_CHECK_IS_ALPHATEST_ENABLE,
#endif
		IDC_CHECK_ADDITIVE_BLEND,        
		IDC_CHECK_IS_ALPHA_OBJECT,
		IDC_CHECK_CAN_HIDE,
		IDC_CHECK_NOT_RENDER,
		IDC_CHECK_USE_WORLD_COLOR,

		IDC_COMBO_SKIN_ANIM_PLAY_TYPE,
		IDC_COMBO_LAYER,
		IDC_COMBO_BILLBOARD,
		IDC_COMBO_OBJECT_TYPE,
		
        
		IDC_BUTTON_FILE_OPEN_NPC_SKINANIM,
		IDC_LIST_MONSTER,
		IDC_BUTTON_KILL_ALL_MONSTER,
		IDC_COMBO_MY_UNIT_CLASS,

		

		IDC_EDIT_WORLD_LIGHT_COLOR,				
		IDC_EDIT_FADE_WORLD_LIGHT_COLOR,
		IDC_EDIT_ORIGIN_WORLD_LIGHT_COLOR,

		
		
		IDC_SLIDER_ORIGIN_WORLD_LIGHT_COLOR_RED,
		IDC_SLIDER_ORIGIN_WORLD_LIGHT_COLOR_GREEN,
		IDC_SLIDER_ORIGIN_WORLD_LIGHT_COLOR_BLUE,


		IDC_EDIT_OBJECT_LIGHT_COLOR,
		IDC_SLIDER_OBJECT_LIGHT_COLOR_RED,
		IDC_SLIDER_OBJECT_LIGHT_COLOR_GREEN,
		IDC_SLIDER_OBJECT_LIGHT_COLOR_BLUE,


        // FOG
        IDC_CHECK_IS_FOG,
        IDC_CHECK_IS_FOGWORLD,
        IDC_EDIT_FOG_NEAR_X,
        IDC_EDIT_FOG_FAR_X,
        IDC_EDIT_FOG_NEAR_Y,
        IDC_EDIT_FOG_FAR_Y,
        IDC_EDIT_FOG_DENSITY,
        IDC_EDIT_FOG_DENSITY_OBJECT,
        IDC_EDIT_ORIGIN_FOG_COLOR,
        IDC_SLIDER_ORIGIN_WORLD_FOG_COLOR_RED,
        IDC_SLIDER_ORIGIN_WORLD_FOG_COLOR_GREEN,
        IDC_SLIDER_ORIGIN_WORLD_FOG_COLOR_BLUE,
        IDC_SLIDER_FOG_NEAR_X,
        IDC_SLIDER_FOG_FAR_X,
        IDC_SLIDER_FOG_NEAR_Y,
        IDC_SLIDER_FOG_FAR_Y,
        IDC_SLIDER_FOG_DENSITY,
        IDC_SLIDER_FOG_DENSITY_OBJECT,

		// 이펙트 툴
		IDC_BUTTON_LOAD_EFFECT_SET,
		IDC_BUTTON_SAVE_EFFECT_SET,
		IDC_BUTTON_LOAD_XMESH_PLAYER,
		IDC_BUTTON_LOAD_PARTICLE,

		IDC_LIST_XMESH_PLAYER_PALETTE,
		IDC_LIST_XMESH_PLAYER_LOADED,
		
		IDC_LIST_PARTICLE_PALETTE,
		IDC_LIST_PARTICLE_LOADED,

		IDC_CHECK_USER_UNIT_STATE,
		IDC_LIST_USER_UNIT_STATE,

		IDC_CHECK_SKIN_ANIM_NAME,
		IDC_LIST_SKIN_ANIM_NAME,
		IDC_CHECK_FIX_SKIN_ANIM_POSITION,
		IDC_EDIT_SKIN_ANIM_POSITION,
		IDC_EDIT_SKIN_ANIM_ROTATION,

		IDC_CHECK_MESH_PLAYER_POS_IS_RELATIVE_TO_UNIT,
		IDC_CHECK_MESH_PLAYER_TRACE,
		IDC_EDIT_MESH_PLAYER_BONE_NAME,
		IDC_EDIT_MESH_PLAYER_DELAY,
		IDC_EDIT_MESH_PLAYER_POS_X,
		IDC_EDIT_MESH_PLAYER_POS_Y,
		IDC_EDIT_MESH_PLAYER_POS_Z,


		IDC_CHECK_PARTICLE_POS_IS_RELATIVE_TO_UNIT,
		IDC_CHECK_PARTICLE_TRACE,
		IDC_EDIT_PARTICLE_BONE_NAME,
		IDC_EDIT_PARTICLE_DELAY,
		IDC_EDIT_PARTICLE_POS_X,
		IDC_EDIT_PARTICLE_POS_Y,
		IDC_EDIT_PARTICLE_POS_Z,

		IDC_CHECK_WORLD_COLOR,
		IDC_EDIT_WORLD_COLOR_TIME,
		IDC_EDIT_WORLD_COLOR_FADE_TIME,


		IDC_BUTTON_READY_EFFECT_DEVICES,

		
		IDC_LIST_CAMERA_SHAKE_UP,
		IDC_EDIT_CAMERA_SHAKE_UP,
		IDC_BUTTON_ADD_CAMERA_SHAKE_UP,
		IDC_BUTTON_DELETE_CAMERA_SHAKE_UP,

        IDC_CHECK_ORTHOFREE,
        IDC_EDIT_ORTHOSCALE,

		// 09.02.26 김태완 : 다이나믹 카메라 on/off
		IDC_CHECK_DYNAMIC_CAMERA,
		// 09.03.02 김태완 : 일정거리 이상에서 object 숨기는 기능
		IDC_EDIT_HIDE_DISTANCE,
		// 09.03.02 김태완 : ForceShow 기능 ( 신마을 )
		IDC_CHECK_FORCE_SHOW,	
		// 09.03.14 김태완 : 카메라 고정 기능
		IDC_CHECK_FIXED_CAMERA,
		
	};


	struct TimedXMeshPlayer
	{
		float			m_fDelayTimeToStart;
		D3DXVECTOR3		m_vRelativePosition;
		bool			m_bRelativeToUnit;
		wstring			m_wstrXMeshPlayerName;
		bool			m_bTraceUnit;
		wstring			m_wstrBoneName;
		CKTDGXMeshPlayer::CXMeshInstance* m_pInstance;

		TimedXMeshPlayer()
		{
			m_fDelayTimeToStart		= 0.f;
			m_vRelativePosition		= D3DXVECTOR3(0,0,0);
			m_wstrXMeshPlayerName	= L"";
			m_bTraceUnit			= false;
			m_wstrBoneName			= L"";
			m_pInstance				= NULL;
			m_bRelativeToUnit		= true;
		}
	};

	struct PreviewXMeshPlayer
	{
		bool			m_bIsCreate;
		wstring			m_wstrXMeshPlayerName;
		CKTDGXMeshPlayer::CXMeshInstance* m_pInstance;

		bool	IsCreate(){ return m_bIsCreate; }
		void	SetIsCreate( bool bIsCreate ){ m_bIsCreate = bIsCreate; }

		void	SetXMeshName( WCHAR* wstrXMeshName ){ m_wstrXMeshPlayerName = wstrXMeshName; }
		const wstring& GetXMeshName(){ return m_wstrXMeshPlayerName; }

		CKTDGXMeshPlayer::CXMeshInstance* GetXMeshInstPointer(){ return m_pInstance; }
		void	SetXMeshInstPointer( CKTDGXMeshPlayer::CXMeshInstance* pXMeshInst ){ m_pInstance = pXMeshInst; }
		void	SetNULLPointerbyXMeshInst(){ m_pInstance = NULL; }

		PreviewXMeshPlayer()
		{
			m_bIsCreate = false;
			m_pInstance = NULL;
		}

	};

	struct TimedParticle
	{
		float			m_fDelayTimeToStart;
		D3DXVECTOR3		m_vRelativePosition;
		bool			m_bRelativeToUnit;
		wstring			m_wstrParticleName;
		bool			m_bTraceUnit;
		wstring			m_wstrBoneName;
		CKTDGParticleSystem::CParticleEventSequence* m_pEventSequence; 

		TimedParticle()
		{
			m_fDelayTimeToStart		= 0.f;
			m_vRelativePosition		= D3DXVECTOR3(0,0,0);
			m_wstrParticleName		= L"";
			m_bTraceUnit			= false;
			m_wstrBoneName			= L"";
			m_pEventSequence		= NULL;
			m_bRelativeToUnit		= true;
		}
	};

	struct	PreviewParticle
	{
		bool			m_bIsCreate;
		wstring			m_wstrParticleName;
		CKTDGParticleSystem::CParticleEventSequence* m_pEventSequence; 

		bool	IsCreate(){ return m_bIsCreate; }
		void	SetIsCreate( bool bIsCreate ){ m_bIsCreate = bIsCreate; }

		void	SetParticleName( WCHAR* wstrParticleName ){ m_wstrParticleName = wstrParticleName; }
		const wstring& GetParticleName(){ return m_wstrParticleName; }

		CKTDGParticleSystem::CParticleEventSequence* GetEventSequencePointer(){ return m_pEventSequence; }
		void	SetEventSequencePointer( CKTDGParticleSystem::CParticleEventSequence* pEventSequence ){ m_pEventSequence = pEventSequence; }
		void	SetNULLPointerbyEventSequence(){ m_pEventSequence = NULL; }

		PreviewParticle()
		{
			m_bIsCreate = false;
			m_pEventSequence = NULL;
		}
	};

	struct CameraShake
	{
		float m_fStartTime;
		float m_fAmplitude;
		float m_fDuration;

		CameraShake()
		{
			m_fStartTime = -1.f;
			m_fAmplitude = 0.f;
			m_fDuration = 0.f;
		}
	};

	enum START_DIR
	{
		DIR_LEFT,
		DIR_RIGHT,
	};

	enum TEAM_COLOR
	{
		TEAM_NONE,
		TEAM_RED,
		TEAM_BLUE,
	};

	struct STeamStartPos
	{
		int key;
		int iTeamColor;
		int iInitialDir;
		D3DXVECTOR3 vPos;
		int iLineIndex;

		STeamStartPos()
		{

		}

		STeamStartPos( const int key, const int iTeamColor, const D3DXVECTOR3& vPos, const int iInitialDir, int iLineIndex = 0 )
		{
			this->key				= key;
			this->iTeamColor		= iTeamColor;
			this->vPos				= vPos;
			this->iInitialDir		= iInitialDir;
			this->iLineIndex		= iLineIndex;
		}
	};

	struct SNPCStartPos
	{
		CX2DungeonSubStage::NPCData*		m_pNPCData;
		int									m_iStartPosIndex;

		SNPCStartPos( CX2DungeonSubStage::NPCData* pNPCData, int iStartIndex )
		{
			m_pNPCData			= pNPCData;
			m_iStartPosIndex	= iStartIndex;
		}
	};

	class CTeamStartPosSort
	{
	public: 
		bool operator() ( const STeamStartPos& lhs, const STeamStartPos& rhs ) const
		{ 
			if( lhs.iTeamColor > rhs.iTeamColor )
				return true;
			else if( lhs.iTeamColor == rhs.iTeamColor && lhs.key < rhs.key )
				return true;
			else
				return false;
		}
	};	
	

public:
	CX2WorldToolMain(void);
	virtual ~CX2WorldToolMain(void);

	virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
	virtual HRESULT OnFrameRender();
	virtual HRESULT OnResetDevice();
	virtual HRESULT OnLostDevice();
	virtual bool	MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	
	void LoadWorld( const WCHAR* pwszFileName );
	void PostProcLoadWorld();
	void LoadLineMap( const WCHAR* pwszFileName );

	HRESULT SaveWorld( const WCHAR* pwszFileFullPath );
	bool SaveTempLineMap( const WCHAR* wszFileName );

	void LoadXFileToPalette( WCHAR* wszFileName );
	void ClearXFilePalette();

	void AddPickedPaletteObjectToWorld( CX2WorldObject::OBJECT_TYPE eObjType = CX2WorldObject::OT_STATIC_MESH );
	void AddObjectToWorld( const WCHAR* wszFileName, CX2WorldObject::OBJECT_TYPE eObjType = CX2WorldObject::OT_STATIC_MESH );
	void RemoveObjectFromWorld( CWorldToolObject* pWorldToolObject );
	void RemoveSelectedObjectFromWorld();

	void PickWorldObjectInListBox( bool bClearBefore );
	void UpdatePickedObjectPropertyUI();
    void InitFog();

	void ChangeUserUnit( CX2Unit::UNIT_CLASS eUnitClass );
	CX2Unit::UNIT_CLASS& GetCurrUnitClass() { return m_CurrUnitClass; }

    D3DXCOLOR GetFogColor()
    {
        return m_fogColor;
    }

    void SetFogColor( float rColor, float gColor, float bColor )
    {
        m_fogColor = D3DXCOLOR(rColor, gColor, bColor, 1.0f);
    }

	void UpdateWorldToolObjectList();

protected:
	void ResetDialogAtModeChange();
	void UpdateDialog();
	void FinalDialog();
	void InitDialog();
	void InitX2Data();

    
	void ClearPickedWorldToolObjectList() { m_vecpPickedWorldToolObject.clear(); }
	void ClearWorldToolObjectList();
//	void UpdateWorldToolObjectList();	// public으로
	void UpdateWorldToolObjectListBox();

	bool DropFileProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


	void CreateGame();
	void DisableGameKey();

	void RenderBoundingSphere( D3DXVECTOR3& vCenter, float fRadius );



public:
    bool DecomposePathAndFileName( WCHAR* wszPath, WCHAR* wszFileName, const WCHAR* wszFullPath );

	CWorldToolCamera& GetCamera() { return m_FPSCamera; }
	vector<CWorldToolObject*>& GetPickedWorldToolObjectList() { return m_vecpPickedWorldToolObject; }
    vector<CWorldToolObject*>& GetWorldToolObjectList() { return m_vecpWorldToolObject; }
	CWorldToolDialog* GetDialog( int index ) { return m_vecpDialog[index]; }

	// 이펙트 툴
	//CX2DamageEffect* GetDamageEffect() { return m_pDamageEffect; }
	void ResetXMeshPlayer()
	{
		if( m_pXMeshPlayer != NULL )
		{
			SAFE_DELETE( m_pXMeshPlayer );
		}
		m_pXMeshPlayer = new CKTDGXMeshPlayer;
	}

	CKTDGXMeshPlayer* GetXMeshPlayer() { return m_pXMeshPlayer; }
	//void SetDamageEffectName( const WCHAR* wszName ) { m_DamageEffectName = wszName; }
	//void SetXMeshPlayerName( const WCHAR* wszName ) { m_XMeshPlayerName = wszName; }

	void ClearTimedXMeshPlayerList();
	void ClearTimedParticleList();
	void ClearCameraShakeList();


	void UpdatePickedXMeshPlayerUI();
	void UpdatePickedParticleUI();
	void UpdatePickedCameraShakeUI();
	void UpdateSquareCameraUI();




	void ResetUserUnitStateListBox();
	void ClearUserUnitStateListBox();
	HRESULT SaveEffectSet( const WCHAR* pwszFileFullPath );
	HRESULT LoadEffectSet( const WCHAR* pwszFileFullPath );

	void CreateCharacterSkinMesh( const WCHAR* wszCharacterName );
	void CreateCharacterSkinMeshFromFile( const WCHAR* wszFileName );

	void On_IDC_BUTTON_LOAD_XMESH_PLAYER();
	void On_IDC_BUTTON_LOAD_PARTICLE();


	D3DXCOLOR GetFadeWorldColor() const { return m_coFadeWorldColor; }
	void SetFadeWorldColor(D3DXCOLOR val) { m_coFadeWorldColor = val; }

	void ParseIntArrayFromEditBox( CDXUTEditBox* pEditBox, int *pNumbers, int nCount );
	void SetTextIntArrayToEditBox( CDXUTEditBox* pEditBox, const int *pNumbers, int nCount );

	void ReadyEffectDevices();

	void UpdateMonsterList();

	void LoadLineMapInfo();
	void LineMapFloatToInt();
	void RecomputeStartPosLineIndex();
	float ComputeLandHeight();
	void SaveLineMapToText();
	bool SaveLineMapToFile( const WCHAR* wszFileName, bool bUpdateFileName );
	
	bool GetMousePosOnLine( D3DXVECTOR3* pvPosOnLine, const POINTS& mousepos );
	void AddNewStartPosition(const POINTS& mousepos);
	bool PickClickedNPC( int iDungeonLevel, POINTS mousepos, bool bClear = true );
	float DistanceBetweenPointAndSegment(float& fLerpCoef, D3DXVECTOR3* pvPoint, D3DXVECTOR3* pvSegmentStart, D3DXVECTOR3* pvSegmentEnd);

	int GetNextUniqueNPCStartPosKey()
	{
		for( int i=1; i<sizeof(m_abKeyUsed); i++ )
		{
			m_CurrKeyUsed = (m_CurrKeyUsed + 1) % (int)sizeof(m_abKeyUsed);
			if( false == m_abKeyUsed[m_CurrKeyUsed] )
			{
				return m_CurrKeyUsed;
			}
		}

		return -1;
	}

	void ResetNpcStartPosKey()
	{
		m_CurrKeyUsed = 0;
		for( int i=0; i<sizeof(m_abKeyUsed); ++i)
		{
			m_abKeyUsed[i] = false;
		}
		for( int i = 0; i < int(vecTeamStartPos.size()); ++i)
		{
			if( vecTeamStartPos[i].iTeamColor == 0 )
			{
				int key = GetNextUniqueNPCStartPosKey();
				if( key == -1 )
					continue;
				vecTeamStartPos.at(i).key = key;
			}
		}
	}

	void DeleteNpcStartPos()
	{
		m_CurrKeyUsed = 0;
		for( int i=0; i<sizeof(m_abKeyUsed); ++i)
		{
			m_abKeyUsed[i] = false;
		}
		for( int i=vecTeamStartPos.size() - 1; i>=0; --i)
		{
			if( vecTeamStartPos[i].iTeamColor == 0 )
			{
				vecTeamStartPos.erase(vecTeamStartPos.begin() + i);
			}
		}

		for( unsigned i=0; i < vecpXSkinAnim.size(); i++ )
		{
			SAFE_DELETE_KTDGOBJECT( (CKTDGXSkinAnim*)vecpXSkinAnim[i] );
		}
		vecpXSkinAnim.clear();
	}
	
	STeamStartPos& GetStartPos(int iIndex) { return (STeamStartPos&)vecTeamStartPos[iIndex]; }
	
	int& GetDir() { return iInitialDir; }
	int& GetTeamColor() { return iTeamColor; }
	int& GetShowDungeonLevel() { return m_iShowDungeonLevel; }
	void GetLineIndexInRect( float fLeft, float fTop, float fRight, float fBottom );
	
	
	//void ToggleNavigate() { bNavigate = !bNavigate; }
	int& GetPickedStartPosIndex() { return m_iPickedStartPosIndex; }	
	//vector<int>& GetPickedSegmentVector() { return m_veciPickedSegmentIndex; }
	void ComputeRay( D3DXVECTOR3* pvRayOrigin, D3DXVECTOR3* pvRayDirection, const POINTS& mouspos, const D3DXMATRIX* pmatView, const D3DXMATRIX* pmatProj );
	void CollisionCheckNPCStartPos( CX2DungeonSubStage::SubStageData* pSubStageData, D3DXVECTOR3 vRayStart, D3DXVECTOR3 vRayEnd, 
		float fHitDistance, float fShortestHitDistance, CX2DungeonSubStage::NPCData* &pPickedNPCData, 
		int &iPickedNPCStartIndex );
	bool NPCRayHit( D3DXVECTOR3 vPos, D3DXVECTOR3 rayStart, D3DXVECTOR3 rayEnd, float* pHitDistance );
	void DeleteAllSkinMeshAndSkinAnim();
	void OnFrameRenderUnit();
	void RenderUserUnitStartPos();
	void RenderNPCUnitStartPostion( CX2Dungeon::DIFFICULTY_LEVEL dungeonDifficulty );
	void RenderNPCStartPosIndex( CX2DungeonSubStage::NPCData* pNPCData, int iStageNPCIndex = -1, int iNPCSubIndex = -1 );
	HRESULT CreateD3DXTextMesh( IDirect3DDevice9* pd3dDevice, LPD3DXMESH* ppMesh, TCHAR* pstrFont, DWORD dwSize, BOOL bBold, BOOL bItalic, WCHAR* wszText);
	STeamStartPos *GetTeamStartPos( int iNPCStartPosKey );
	

public:
	float						m_fObjectMoveSpeed;
	vector<TimedXMeshPlayer>	m_vecTimedXMeshPlayer;					// 
	vector<TimedXMeshPlayer>	m_vecReadyToPlayTimedXMeshPlayer;		// 플레이 되기를 기다리고 있는 xmeshplayer
	vector<TimedXMeshPlayer>	m_vecPlayingTimedXMeshPlayer;			// 현재 플레이 중인 xmeshplayer
	
	int							m_iPickedXMeshPlayerIndex;


	
	vector<TimedParticle>		m_vecTimedParticle;
	vector<TimedParticle>		m_vecReadyToPlayTimedParticle;
	vector<TimedParticle>		m_vecPlayingTimedParticle;
	int							m_iPickedParticleIndex;

	PreviewParticle				m_sPreviewParticle;
	PreviewXMeshPlayer			m_sPreviewXMeshPlayer;


	int							m_iPickedUserUnitState;
	bool						m_bUseUserUnitState;				// user unit state에 따라서 animation을 플레이 하는 것을 사용할지 말지 'e' 키를 눌렀을때

	CKTDGXMeshPlayer*			m_pXMeshPlayer;
	CKTDGParticleSystem*		m_pParticleSystem;


	vector<CameraShake>			m_vecCameraShake;
	vector<CameraShake>			m_vecReadyToPlayCameraShake;
	vector<int>					m_vecPickedCameraShake;

    D3DXCOLOR                   m_fogColor;
	


public:
	CWorldToolGame*				m_pGame;
	CWorldToolCamera			m_FPSCamera;

	vector<CWorldToolDialog*>	m_vecpDialog;
	vector<CWorldToolObject*>	m_vecpWorldToolObject;
	vector<CWorldToolObject*>	m_vecpPickedWorldToolObject;
    vector<CX2WorldLayer*>      m_vecWorldLayer;


	// 상태 변수, UI 변수 
	int							m_iPrevToolMode;
	int							m_iToolMode;
	bool						m_bPaused;
	

	wstring						m_wstrWorldFileName;
	wstring						m_wstrLineMapFileName;

	wstring						m_wstrWorldFilePath;
	wstring						m_wstrLineMapFilePath;
	
	CX2Unit::UNIT_CLASS			m_CurrUnitClass;
	bool						m_bRenderLineMap;
	bool						m_bWorldColor;
	float						m_fWorldColorTime;
	float						m_fWorldColorFadeTime;

	float						m_fNowWorldColorTime;

	CKTDXDeviceXMesh*			m_pXMeshBoundingSphere;

	D3DXVECTOR3					m_vFixedCharacterSkinAnimPosition;
	D3DXVECTOR3					m_vFixedCharacterSkinAnimRotation;
	bool						m_bFixPostionCharacterSkinAnim;
	bool						m_bShowCharacterSkinAnim;
	bool						m_bShowUserUnit;
	CSkinAnimObject*			m_pCharacterSkinAnim;
	
	D3DXCOLOR					m_coFadeWorldColor;

	LINEMAP_FILENAME_TYPE		m_eLineMapFileNameType;

	wstring						m_wstrTempWorldScript;
	wstring						m_wstrTempLineMapScript_Prefix;
	wstring						m_wstrTempLineMapScript_Suffix;


	CKTDGFontManager::CUKFont*	m_pFont_A;	

	CKTDXTimer					m_TimerRenderPickedObject;
	bool						m_bRenderBoundingSphere;
	bool						m_bRenderFlicker;


	bool						m_bCenterGreenDot;
    bool                        m_bFog;

	bool						m_bShowLineMapModifier;
	WCHAR						wszTempFilePath[MAX_PATH];
	vector<STeamStartPos>		vecTeamStartPos;

	bool							m_bMouseDragging;
	D3DXVECTOR2						m_vDragStart;
	int								m_CurrKeyUsed;
	bool							m_abKeyUsed[1024];
	vector< SNPCStartPos >			m_vecPickedNPCStartPosKey;
	bool				m_bUseNPCMesh;	
	bool				m_bHideStartPosMesh;
	bool				m_bShowUserUnitStartPos;
	int					m_iPickedStartPosIndex;
	
	int		iTeamColor;
	int		iInitialDir;
	int								m_iShowDungeonLevel;

	CKTDGMatrix*					m_pMatrix;
	CKTDXDeviceXSkinMesh*		m_pXSkinMesh;
	vector<CKTDGXSkinAnim*>		vecpXSkinAnim;

	ID3DXMesh*					m_pMesh3DText[12];
	float						m_f3DTextAngle, m_f3DTextAngleStep;	
	int							m_iStartPosListIndex;

	bool						m_bShowUserStartPos;
	bool						m_bShowNpcStartPos;	
	bool						m_bShowWorld;
};



