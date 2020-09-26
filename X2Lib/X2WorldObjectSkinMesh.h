#pragma once


//{{ seojt // 2009-1-13, 17:04
class CX2WorldObjectSkinMesh;
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::intrusive_ptr<CX2WorldObjectSkinMesh>   CX2WorldObjectSkinMeshPtr;
#else   X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::shared_ptr<CX2WorldObjectSkinMesh>   CX2WorldObjectSkinMeshPtr;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
//}} seojt // 2009-1-13, 17:04


class CX2WorldObjectSkinMesh : public CX2WorldObject
{
	public:
		
		enum WORLD_OBJECT_SKIN_MESH_PLAY_TYPE
		{
			WOSMPY_ONE = 0,
			WOSMPY_ONE_WAIT,
			WOSMPY_LOOP,
			WOSMPY_PINGPONG,
			WOSMPY_OCCASIONAL_ONE,
			WOSMPY_OCCASIONAL_ONE_WAIT,
		};

	private:
		CX2WorldObjectSkinMesh( bool bBackgroundLoad_ );
		virtual ~CX2WorldObjectSkinMesh(void);
		
	public:
		static CX2WorldObjectSkinMeshPtr CreateWorldObjectSkinMesh( bool bBackgroundLoad_ )
        {
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
            CX2WorldObjectSkinMeshPtr pObject( new CX2WorldObjectSkinMesh( bBackgroundLoad_ ) );
#else   X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
			CX2WorldObjectSkinMeshPtr pObject( new CX2WorldObjectSkinMesh( bBackgroundLoad_ ), CKTDGObject::KTDGObjectDeleter() );
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
            return pObject;
        }//CreateWorldObjectSkinMesh()

		virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );

//{{ robobeg : 2008-10-16
        virtual RENDER_HINT   OnFrameRender_Prepare();
		//virtual HRESULT OnFrameRender();
        virtual void              OnFrameRender_Draw();
//}} robobeg : 2008-10-16

//#ifdef UNIT_BACKGROUND_LOADING_TEST // 2008-12-12
        bool            IsValidMesh() const { return m_bXSkinMeshLoaded && m_pXSkinMotionMesh != NULL && m_pXSkinAnim != NULL; }
        bool            IsValidModelMesh() const { return m_bModelXSkinMeshLoaded 
            && m_XSkinModelMeshList.empty() == false
            && m_XSkinModelMeshList.front().m_bLoaded == true
			&& m_XSkinModelMeshList.front().m_pXSkinMesh != NULL; }
//#endif // UNIT_BACKGROUND_LOADING_TEST // 2008-12-12

		virtual HRESULT OnResetDevice();
		virtual HRESULT OnLostDevice();

//#ifdef  _HACKPROOF_CLIENT_
//
//#ifdef UNIT_BACKGROUND_LOADING_TEST // 2008-12-11
//		virtual bool OnThreadLoad(DWORD dwParam_, DWORD);
//#endif // UNIT_BACKGROUND_LOADING_TEST // 2008-12-11
//
//#endif  _HACKPROOF_CLIENT_

		CKTDXDeviceXSkinMesh* GetXSkinMotionMesh() { return m_pXSkinMotionMesh; } 
		wstring& GetXSkinMeshName() { return m_XSkinMeshName; }

		const CKTDGMatrix::BILLBOARD_TYPE GetBillboardType() { return m_BillboardType; }
		
		//LUA
		bool PlayAnim_LUA( WORLD_OBJECT_SKIN_MESH_PLAY_TYPE playType, bool btransition = true );
		void AddAnim_LUA( const char* pAnimName );

#ifdef WORLD_TRIGGER		
		void ChangePlayAnim_LUA( wstring playName, WORLD_OBJECT_SKIN_MESH_PLAY_TYPE playType );
#endif

#if defined(WORLD_TOOL) || defined(X2TOOL)
		CKTDGXSkinAnim *GetXSkinAnim() { return m_pXSkinAnim; }
		void SetPlayType(int iPlayType) { m_PlayType = (WORLD_OBJECT_SKIN_MESH_PLAY_TYPE)iPlayType; }
#endif

		void SetTransition_LUA( bool btransition ){ m_bTransition = btransition; }
		void SetAnimAniXET_LUA( const char* pFileName );
		void SetXSkinMotion_LUA( const char* pFileName );

		void SetMeshAniXET_LUA( const char * pFileName );
		void SetMultiTexXET_LUA( const char* pFileName );
		void SetChangeTexXET_LUA( const char* pFileName );
		void AddXSkinModel_LUA( const char* pFileName );

		CKTDGXRenderer::RenderParam* GetRenderParam(){ return &m_RenderParam; }

		bool GetCanHide() { return m_bCanHide; }
		void SetCanHide( bool bCanHide, bool bAnyCamera = false ){ m_bCanHide = bCanHide; m_bAnyCamera = bAnyCamera; }
		void SetBillboardType( int billboardType ){ m_BillboardType = (CKTDGMatrix::BILLBOARD_TYPE)billboardType; }
		void SetTex0UVSpeed( float x, float y ){ m_Tex0UVSpeed.x = x; m_Tex0UVSpeed.y = y; }
		void SetTex1UVSpeed( float x, float y ){ m_Tex1UVSpeed.x = x; m_Tex1UVSpeed.y = y; }
		void SetTex2UVSpeed( float x, float y ){ m_Tex2UVSpeed.x = x; m_Tex2UVSpeed.y = y; }

		D3DXVECTOR2 GetTex0UVSpeed() { return m_Tex0UVSpeed; }
		D3DXVECTOR2 GetTex1UVSpeed() { return m_Tex1UVSpeed; }
		D3DXVECTOR2 GetTex2UVSpeed() { return m_Tex2UVSpeed; }
		
		vector< wstring >& GetPlayList() { return m_PlayList; }
		CX2WorldObjectSkinMesh::WORLD_OBJECT_SKIN_MESH_PLAY_TYPE GetPlayType() const { return m_PlayType; }

		float GetOccasionalEventTime() const { return m_fOccasionalEventTime; }
		void SetOccasionalEventTime(float val) { m_fOccasionalEventTime = val; }
		int GetOccasionalPlayRate() const { return m_iOccasionalPlayRate; }
		void SetOccasionalPlayRate(int val) { m_iOccasionalPlayRate = val; }

#ifdef FOG_WORLD
        void SetFog(float xNear, float xFar, float yNear, float yFar, D3DXCOLOR color = D3DXCOLOR(0xd3, 0xe3, 0xf3, 0xff), float density = 0.0f);
        void SetFogShow(bool flag)  { m_RenderParam.bFogShow = flag; }
        bool GetFogShow()  { return m_RenderParam.bFogShow; }
#endif

		void SetCartoonRender();

		void SetAnimSpeed(float fSpeed) 
		{ 
			if( fSpeed <= 0.1f )
				fSpeed = 0.1f;


			m_fAnimSpeed = fSpeed; 
			if( NULL != m_pXSkinAnim )
			{
				m_pXSkinAnim->SetPlaySpeed( fSpeed );
			}
		}

		void SetAnimPlayLocalInterval(float fInterval) { m_fLocalInterval = fInterval; }
		
	protected:

		bool RayHit( D3DXVECTOR3 rayStart, D3DXVECTOR3 rayEnd, float* pHitDistance=NULL );
		//void RayCheck(CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame, D3DXVECTOR3 rayPos, D3DXVECTOR3 rayDir );

		wstring							m_XSkinMeshName;

		float							m_fAnimSpeed;	
		CKTDGXSkinAnim*					m_pXSkinAnim;
		bool							m_bTransition;
		
		CKTDXDeviceXSkinMesh*			m_pXSkinMotionMesh;
		CKTDXDeviceXET*					m_pAnimXET;
		

		struct	XSkinMeshXETState
		{
			bool						m_bLoaded;
			CKTDXDeviceXSkinMesh*		m_pXSkinMesh;
			CKTDXDeviceXET*				m_pMeshXET;
			CKTDXDeviceXET*				m_pMultiTexXET;
			CKTDXDeviceXET*				m_pChangeTexXET;

			XSkinMeshXETState()
				: m_bLoaded( false )
				, m_pXSkinMesh( NULL )
				, m_pMeshXET( NULL )
				, m_pMultiTexXET( NULL )
				, m_pChangeTexXET( NULL )
			{
			}
		};//

        vector< XSkinMeshXETState >		m_XSkinModelMeshList;
		CKTDXDeviceXET*					m_pMeshXET;
		CKTDXDeviceXET*					m_pMultiTexXET;
		CKTDXDeviceXET*					m_pChangeTexXET;

		//vector< CKTDXDeviceXET* >		m_XETList;

		CKTDGXRenderer::RenderParam		m_RenderParam;

		float							m_fNowAlpha;

		bool							m_bCanHide;
		bool							m_bAnyCamera;
		CKTDGMatrix::BILLBOARD_TYPE		m_BillboardType;

		D3DXVECTOR2						m_Tex0UVSpeed;
		D3DXVECTOR2						m_Tex1UVSpeed;
		D3DXVECTOR2						m_Tex2UVSpeed;

		bool							 m_bPingPongRightDir;
		WORLD_OBJECT_SKIN_MESH_PLAY_TYPE m_PlayType;
		

		int								m_NowPlayAnimIndex;
		vector< wstring >				m_PlayList;

		float							m_fElapsedLastOccansionalEventTime;
		float							m_fOccasionalEventTime;
		int 							m_iOccasionalPlayRate;
//#ifdef UNIT_BACKGROUND_LOADING_TEST // 2008-12-11
	private:		
		std::wstring					    m_wstrModelXSkinMeshName;
		bool							    m_bXSkinMeshLoaded;
		bool							    m_bModelXSkinMeshLoaded;
        WORLD_OBJECT_SKIN_MESH_PLAY_TYPE    m_playTypeReq;
        bool                                m_bPlayTypeTransitionReq;
        bool                                m_bPendingPlayAnim;
//#endif // UNIT_BACKGROUND_LOADING_TEST // 2008-12-11

		float							m_fLocalInterval;
		float							m_fLocalElapsedTime;


#ifdef WORLD_TRIGGER
		int								m_iTriggerObject;
#endif

#if defined(WORLD_TOOL) || defined(X2TOOL)
	public:
		wstring							m_wstrOriAnimName;
		int								m_iOriPlayType;		
#endif

};
