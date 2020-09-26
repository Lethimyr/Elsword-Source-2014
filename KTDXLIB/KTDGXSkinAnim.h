#pragma once

//{{ seojt // 2009-1-9, 15:01
class CKTDGXSkinAnim;
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::intrusive_ptr<CKTDGXSkinAnim>    CKTDGXSkinAnimPtr;
#else   X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::shared_ptr<CKTDGXSkinAnim>   CKTDGXSkinAnimPtr;
//typedef boost::weak_ptr<CKTDGXSkinAnim>     CKTDGXSkinAnimWeakPtr;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
//}} seojt // 2009-1-9, 15:01

class CKTDGXSkinAnim : public CKTDGObject
{
	public:
		enum XSKIN_ANIM_STATE
		{			
			XAS_STOP,
			XAS_PLAYING,
			XAS_WAIT,
		};
		enum XSKIN_ANIM_PLAYTYPE
		{
			XAP_LOOP = 0,
			XAP_ONE,
			XAP_ONE_WAIT,
		};

        struct Alpha_SkinMesh
        { 
            CKTDXDeviceXSkinMesh *m_pAlphaSkinMesh;
            int m_indexMode;
        };

#ifdef ARA_CHARACTER_BASE
		struct RenderParam_SkinMesh
		{ 
			CKTDXDeviceXSkinMesh *m_pRenderParamSkinMesh;
			int m_indexMode;
		};
#endif

	private:
		CKTDGXSkinAnim();
		virtual ~CKTDGXSkinAnim(void);

	public:
		static CKTDGXSkinAnim* CreateSkinAnim() { return new CKTDGXSkinAnim; }
		static CKTDGXSkinAnimPtr CreateSkinAnimPtr() 
		{
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
            CKTDGXSkinAnimPtr ptrXSkinAnim( new CKTDGXSkinAnim ); 
#else   X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
			CKTDGXSkinAnimPtr ptrXSkinAnim( new CKTDGXSkinAnim, CKTDGObject::KTDGObjectDeleter() ); 
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
			return ptrXSkinAnim;
		}

#ifdef JIGGLEBONE_TEST
		void SetJiggleMesh(bool val, float fTension, unsigned char iExpand = 0) 
		{ 
			m_bJiggleMesh = val;
			m_fTension = fTension;
			m_iExpand = iExpand;
		}
#endif


		//static void DeleteKTDGObject( CKTDGXSkinAnim* pObject )  
		//{ 	
		//	if( NULL != pObject )
		//	{
		//		CKTDGObject::DeleteKTDGObject( pObject ); 
		//	} 
		//}

		

#ifdef BOOST_SINGLETON_POOL_TEST
		void* operator new(size_t s);
		void operator delete(void *p);
#endif BOOST_SINGLETON_POOL_TEST

        /// managed된 리소스를 local video memory에 미리 올린다.
        /// - jintaeks on 2008-10-15, 14:48
        void PreLoad();
//{{ robobeg : 2008-10-20
		//void SetShow( bool bShow ){ m_bShow = bShow; }
//}} robobeg : 2008-10-20

		void BuildCloneFrameStructure( CKTDXDeviceXSkinMesh* pAnimXSkinMesh );
		void SetAnimXET( CKTDXDeviceXET* pAnimAniXET );
		bool SetAnimXSkinMesh( CKTDXDeviceXSkinMesh* pAnimXSkinMesh, CKTDXDeviceXET* pAnimAniXET = NULL, bool bSimple = false );
#ifdef SEPARATION_MOTION
		bool MixAnim( CKTDXDeviceXSkinMesh* pAnimXSkinMesh, bool bSimple = false );
#endif

		CKTDXDeviceXSkinMesh* GetAnimXSkinMesh(){ return m_pAnimXSkinMesh; }

		void ReplaceAniXET( CKTDXDeviceXET* pModelAniXETFrom, CKTDXDeviceXET* pModelAniXETTo );
		void ReplaceMultiTexXET( CKTDXDeviceXET* pModelMultiTexXETFrom, CKTDXDeviceXET* pModelMultiTexXETTo );
		CKTDXDeviceXET* ReplaceMultiTexXET( int iIndex, CKTDXDeviceXET* pModelMultiTexXETTo );
		
		void ReplaceChangeXET( CKTDXDeviceXET* pModelTexChangeXETFrom, CKTDXDeviceXET* pModelTexChangeXETTo );
		void ReplaceChangeXET( int iIndex, CKTDXDeviceXET* pModelTexChangeXETTo );


		void AddModelXSkinMesh( CKTDXDeviceXSkinMesh* pModelXSkinMesh, CKTDXDeviceXET* pModelAniXET = NULL, 
			CKTDXDeviceXET* pModelMultiTexXET = NULL, CKTDXDeviceXET* pModelTexChangeXET = NULL,
			bool bCloseDeviceAtDestructor = false
			, bool bAddCollisionData = true 
			);

		void AddAnimXSkinMesh( CKTDXDeviceXSkinMesh* pModelXSkinMesh );	// 툴잡없을 위한 애니메이션 메쉬 삽입함수
		void RemoveModelXSkinMesh( CKTDXDeviceXSkinMesh* pModelXSkinMesh );
		bool IsInModelXSkinMesh( CKTDXDeviceXSkinMesh* pModelXSkinMesh );
		__forceinline void CopyCloneFrameStructure( CKTDXDeviceXSkinMesh::MultiAnimFrame* pSrcCloneFrame )
		{
			CopyFrame( m_pCloneFrameRoot, pSrcCloneFrame );
		}
		
		virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );

//{{ robobeg : 2008-10-16

        //virtual bool	GetShowObject() const;

        virtual RENDER_HINT OnFrameRender_Prepare();
		virtual void            OnFrameRender_Draw();
//}} robobeg : 2008-10-16


		//컨트롤 API//
		void Play( XSKIN_ANIM_PLAYTYPE playType );
		void Reset( float fAnimTime = 0.0f );
		void Wait();
		void Stop();
		const float GetNowAnimationTime() const { return m_fAnimCurrTime; }
		const float GetBeforeAnimationTime() const { return m_fAnimBeforeTime; }
		float GetMaxAnimationTime(){ return m_fAnimMaxTime - m_fTransitionTime;	}
		XSKIN_ANIM_STATE GetState(){ return m_AnimState; }
		void			 SetState( XSKIN_ANIM_STATE animState ){ m_AnimState = animState; }

		XSKIN_ANIM_PLAYTYPE GetPlayType() { return m_PlayType; }
 
		int	GetNowPlayCount(){ return m_NowPlayCount; }
		wstring& GetNowAnimName() { return m_NowAnimName; }
		int GetNowAnimIndex() { return m_NowAnimIndex; }
		
		bool ChangeAnim( int index, bool transition = true, float fStartTime = 0.0f );
		bool ChangeAnim( const WCHAR* name, bool transition = true, float fStartTime = 0.0f );
		bool ChangeAnim( LPD3DXANIMATIONSET pAS, bool transition = true, float fStartTime = 0.0f );

		float GetPlaySpeed() const { return m_fPlaySpeed; }
		void SetPlaySpeed( float fPlaySpeed )
		{ 
			KTDXPROFILE();
			m_fPlaySpeed = fPlaySpeed; 
			if( NULL != m_pAC )
				m_pAC->KeyTrackSpeed( m_NowTrack, m_fPlaySpeed, m_fAnimCurrTime,  (m_fTransitionTime / m_fPlaySpeed), D3DXTRANSITION_LINEAR );
		}
		void SetAnimationTime( float fTime )
		{
			KTDXPROFILE();
			if( NULL != m_pAC )
				m_pAC->SetTrackPosition( m_NowTrack, fTime );
			m_fAnimCurrTime = fTime;
		}
		/////////////


		void SetUseTex( int index, bool bUse ){ m_bUseTex[index] = bUse; }
		LPD3DXANIMATIONSET GetAnimSet( int index ){ return m_vAnimSetList[index]; }
		LPD3DXANIMATIONSET GetAnimSet( const WCHAR* name ){ return m_mAnimSetList[name]; }
		int GetIndexByName( const WCHAR* name ){ return m_mIndexByName[name]; }

		bool IsAnimationEnd(){ return m_bAnimEnd; }
		D3DXVECTOR3 GetMotionOffset(){ return m_MotionOffset; }
		void SetApplyMotionOffset( bool bApply ){ m_bApplyMotionOffset = bApply; }


#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

        float   GetNowAnimationSpeed() const    { return m_fAnimCurrSpeed; }

        bool    Predict_IsAnimationEnd( float fElapsedTime_ )
        {
            return  ( m_AnimState == XAS_PLAYING && m_fAnimCurrTime <= m_fAnimMaxTime - m_fTransitionTime
                && m_fAnimCurrTime + m_fAnimCurrSpeed * fElapsedTime_ > m_fAnimMaxTime - m_fTransitionTime );
        }
        bool    Predict_EventTimer( float fTime, float fElapsedTime_ )
        {
	        return ( m_fAnimCurrTime < fTime
		        && m_fAnimCurrTime + m_fAnimCurrSpeed * fElapsedTime_ >= fTime );
        }

        bool EventTimer( const std::vector<float>& vecfTime_ )
        {
            if ( vecfTime_.empty() == true )
                return false;
            std::vector<float>::const_iterator iterGreaterThanBeforeTime = std::upper_bound( vecfTime_.begin(), vecfTime_.end(), m_fAnimBeforeTime );
            if ( iterGreaterThanBeforeTime == vecfTime_.end()
                || m_fAnimCurrTime < *iterGreaterThanBeforeTime )
                return false;
            return true;
        }//EventTimer()

        bool Predict_EventTimer( const std::vector<float>& vecfTime_, float fElapsedTime_ )
        {
            if ( vecfTime_.empty() == true )
                return false;
            std::vector<float>::const_iterator iterGreaterThanBeforeTime = std::upper_bound( vecfTime_.begin(), vecfTime_.end(), m_fAnimCurrTime );
            if ( iterGreaterThanBeforeTime == vecfTime_.end()
                || m_fAnimCurrTime + m_fAnimCurrSpeed * fElapsedTime_ < *iterGreaterThanBeforeTime )
                return false;
            return true;
        }//EventTimer()
        bool    IntervalTimer( float fStartTime_, float fEndTime_ )
        {
            return fStartTime_ <= fEndTime_ && m_fAnimCurrTime >= fStartTime_ && ( m_fAnimCurrTime <= fEndTime_ || m_fAnimBeforeTime <= fEndTime_ );
        }//IntervalTimer()

        bool    Predict_IntervalTimer( float fStartTime_, float fEndTime_, float fElapsedTime )
        {
            float   fEstimatedFuture = m_fAnimCurrTime + m_fAnimCurrSpeed * fElapsedTime;
            return fStartTime_ <= fEndTime_ && fEstimatedFuture >= fStartTime_ && ( fEstimatedFuture <= fEndTime_ || m_fAnimCurrTime <= fEndTime_ );
        }//
        bool    EventTimerOneshot( float fTime )
        {
	        return m_fAnimOneshotTimer < fTime && EventTimer( fTime );
        }//
        bool    Predict_EventTimerOneshot( float fTime, float fElapsedTime )
        {
	        return ( __max( m_fAnimOneshotTimer, m_fAnimCurrTime ) < fTime
		        && m_fAnimCurrTime + m_fAnimCurrSpeed * fElapsedTime >= fTime );

        }//
        bool    Predict_EventTimerOneshot( const std::vector<float>& vecfTime_, float fElapsedTime )
        {
            if ( vecfTime_.empty() == true )
                return false;
            float   fMax = __max( m_fAnimOneshotTimer, m_fAnimCurrTime );
            std::vector<float>::const_iterator iterGreaterThanBeforeTime = std::upper_bound( vecfTime_.begin(), vecfTime_.end(), fMax );
            if ( iterGreaterThanBeforeTime == vecfTime_.end()
                || m_fAnimCurrTime + m_fAnimCurrSpeed * fElapsedTime < *iterGreaterThanBeforeTime )
                return false;
            return true;
        }//
        void    UpdateBeforeAnimationTime()
        {
            m_fAnimBeforeTime = m_fAnimCurrTime;
            m_fAnimOneshotTimer = __max( m_fAnimOneshotTimer, m_fAnimBeforeTime );
        }//
        void    ResetOneshotPerformed()  { m_fAnimOneshotTimer = 0.f; }

#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		bool EventTimer( float fTime );

		void SetDXMatrix( const D3DXMATRIX& DXMatrix ){ m_DXMatrix = DXMatrix; }
		D3DXMATRIX GetDXMatrix(){ return m_DXMatrix; }
		void UseDXMatrix( bool use ){ m_bUseDXMatrix = use; }
		void SetBillBoardType( CKTDGMatrix::BILLBOARD_TYPE billBoardType ) { m_BillBoardType = billBoardType; }
#ifdef X2OPTIMIZE_CULLING_PARTICLE
		CKTDGMatrix::BILLBOARD_TYPE GetBillBoardType() { return m_BillBoardType; }
#endif//X2OPTIMIZE_CULLING_PARTICLE

		__forceinline CKTDXDeviceXSkinMesh::MultiAnimFrame* GetCloneFrameRoot(){ return m_pCloneFrameRoot; }
		__forceinline CKTDXDeviceXSkinMesh::MultiAnimFrame* GetCloneFrame( const WCHAR* name );
		__forceinline D3DXVECTOR3 GetCloneFramePosition( const WCHAR* pName )
		{
			KTDXPROFILE();
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame = GetCloneFrame(pName);

			D3DXVECTOR3 pos;
			if( pFrame != NULL )
			{
				pos.x = pFrame->combineMatrix._41;
				pos.y = pFrame->combineMatrix._42;
				pos.z = pFrame->combineMatrix._43;
				return pos;
			}
			else
			{
				pos.x = 0.0f;
				pos.y = 0.0f;
				pos.z = 0.0f;
			}

			return pos;
		}
#ifdef MODIFY_GET_BONE_POS
		__forceinline bool GetCloneFramePosition( const WCHAR* pName, OUT D3DXVECTOR3& pos_ )
		{
			KTDXPROFILE();
			CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame = GetCloneFrame(pName);

			if( pFrame == NULL )
			{
				pos_.x = 0.0f;
				pos_.y = 0.0f;
				pos_.z = 0.0f;
				return false;
			}

			pos_.x = pFrame->combineMatrix._41;
			pos_.y = pFrame->combineMatrix._42;
			pos_.z = pFrame->combineMatrix._43;
			return true;
		}
#endif //MODIFY_GET_BONE_POS
		
        const CKTDXCollision::CollisionDataList& GetCollisionDataList() const { return m_CollisionDataList; }
        const CKTDXCollision::CollisionDataList& GetAttackDataList() const { return m_AttackDataList; }

#ifdef EVE_ELECTRA
		CKTDXCollision::CollisionDataList& GetAttackDataListNonConst(){ return m_AttackDataList; }
#endif EVE_ELECTRA

		CKTDGXRenderer::RenderParam* GetRenderParam(){ return &m_RenderParam; }

		void SetModelDetailPercent( int detailPercent );

		static bool GetTestUpdate() { return m_sbTestUpdateAnimation; }
		static void SetTestUpdate( bool bCheck ) { m_sbTestUpdateAnimation = bCheck; }

		CKTDGXRenderer* GetXRenderer() { return m_pRenderer; }

#ifdef SKIN_LIGHTFLOW
        void SetChildLightFlow(CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame);
        void SetLightFlow(wstring szName, CKTDGXRenderer::RENDER_TYPE flowType = CKTDGXRenderer::RT_LIGHT_FLOW_LOCAL_Y);
        void SetFlowType( CKTDGXRenderer::RENDER_TYPE fType ) { m_lightFlowType = fType; }
        void SetFlowMin( float fFlowMin ) { m_fFlowMin = fFlowMin; }
        void SetFlowMax( float fFlowMax ) { m_fFlowMax = fFlowMax; }
        void SetFlowSpeed( float fFlowSpeed ) { m_fFlowSpeed = fFlowSpeed; }
        void SetWide( float fWide ) { m_fWide = fWide; }
        void SetNowPoint( float fNowPoint ) { m_fNowPoint = fNowPoint; }
        void SetFlowColor( D3DXCOLOR flowColor ) { m_coColor = flowColor; }
        HRESULT LightFlowFrameMove(double fTime, float fElapsedTime);
#endif

#ifdef JIGGLEBONE_TEST
	void SetUnitXSkinAnim(CKTDGXSkinAnimPtr pXSkinAnimPtr) { m_pUnitXSkinAnim = pXSkinAnimPtr; }	
#endif
	
#if defined(WORLD_TOOL) || defined(X2TOOL)
	bool GetAnimNameList( std::vector<std::wstring>& vecAnimNameList );
#endif
	
	void UpdateAnimationFrame_Public( CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame, D3DXMATRIX* pmxBase, CKTDXDeviceXSkinMesh::MultiAnimFrame* pCloneFrame )
	{
		UpdateAnimationFrame( pFrame, pmxBase, pCloneFrame );
	}

	//{{ oasis907 : 김상윤 [2010.4.22] // 
	void SetNotCull() { this->SetBoundingRadius(0.0f); }	
	//}}

	void SetApplyMoveBone(bool bVal) { m_bApplyMoveBone = bVal; }
	bool GetApplyMoveBone() { return m_bApplyMoveBone; }
#ifdef FACE_OFF_MONSTER_HEAD_TEST
	void SetHideNoRenderable( bool bVal ){ m_bHideNoRenderable = bVal; }
#endif FACE_OFF_MONSTER_HEAD_TEST

#ifdef FIX_MESH_RENDER_BEFORE_INITINALIZE
	bool GetProcessFrameMove() const { return m_bIsProcessFrameMove; }
	void SetProcessFrameMove(bool val) { m_bIsProcessFrameMove = val; }
#endif // FIX_MESH_RENDER_BEFORE_INITINALIZE


#ifdef MODIFY_AFTER_IMAGE
	void GetModelXSkinMeshList(OUT vector<CKTDXDeviceXSkinMesh*>& vecModelXSkinMeshLIst )  { vecModelXSkinMeshLIst.clear(); vecModelXSkinMeshLIst = m_pModelXSkinMeshList; }
	void ClearModelXSkinMeshList();
#endif // MODIFY_AFTER_IMAGE
	
	private:
		void CleanAnimStructure();
		void BuildCloneFrame( CKTDXDeviceXSkinMesh::MultiAnimFrame* pCloneFrame, CKTDXDeviceXSkinMesh::MultiAnimFrame* pSrcFrame );
		void BuildCloneFrameList( CKTDXDeviceXSkinMesh::MultiAnimFrame* pCloneFrame );

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        __forceinline void UpdateAnimationFrameStructure( float fElapsedTime )
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		__forceinline void UpdateAnimationFrameStructure()
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		{           
			KTDXPROFILE();

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            m_fAnimGlobalTime += fElapsedTime;
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			m_fAnimGlobalTime += m_fElapsedTime;
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

			if( NULL == m_pAC )
				return;

			if( NULL == m_pAnimXSkinMesh )
				return;

			KTDXPROFILE_BEGIN("AdvanceTime");
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            m_pAC->AdvanceTime( fElapsedTime, NULL );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			m_pAC->AdvanceTime( m_fElapsedTime, NULL );
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			KTDXPROFILE_END();

			KTDXPROFILE_BEGIN("GetTrackDesc");
			D3DXTRACK_DESC td;
			m_pAC->GetTrackDesc( m_NowTrack, &td );
			m_fAnimBeforeTime = m_fAnimCurrTime;
			m_fAnimCurrTime = (float)td.Position;
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            m_fAnimOneshotTimer = __max( m_fAnimOneshotTimer, m_fAnimBeforeTime );
            if ( m_fAnimCurrTime < 0.f )
                m_fAnimCurrTime = 0.f;
            if ( m_fAnimCurrTime > m_fAnimMaxTime )
                m_fAnimCurrTime = m_fAnimMaxTime;
            m_fAnimCurrSpeed = (float)td.Speed;
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			KTDXPROFILE_END();

			KTDXPROFILE_BEGIN("UpdateAnimationFrame");
			if( m_bUseDXMatrix == false )
			{
				D3DXMATRIX temp = GetMatrix().GetMatrix( m_BillBoardType );

				UpdateAnimationFrame( m_pAnimXSkinMesh->GetFrameRoot(), &temp, m_pCloneFrameRoot );
			}
			else
			{
				UpdateAnimationFrame( m_pAnimXSkinMesh->GetFrameRoot(), &m_DXMatrix, m_pCloneFrameRoot );
			}
			KTDXPROFILE_END();
		}

		__forceinline void UpdateAnimationFrame( CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame, D3DXMATRIX* pmxBase, CKTDXDeviceXSkinMesh::MultiAnimFrame* pCloneFrame )
		{
			KTDXPROFILE();
 
			if( m_bApplyMoveBone == true && pFrame->m_bMoveBone == true )
			{
				m_MotionMoveBack = m_MotionMoveNow;

				m_MotionMoveNow.x = pFrame->TransformationMatrix._41;
				m_MotionMoveNow.z = pFrame->TransformationMatrix._43;

				m_MotionOffset = m_MotionMoveNow - m_MotionMoveBack;

				if( m_bApplyMotionOffset == true )
				{
					pFrame->TransformationMatrix._41 = 0.0f;
					pFrame->TransformationMatrix._43 = 0.0f;
				}
				else
				{
					m_MotionOffset.x = 0.0f;
					m_MotionOffset.z = 0.0f;

					m_MotionMoveNow.x = 0.0f;
					m_MotionMoveNow.z = 0.0f;
				}
			}

#ifdef MOVE_BONE
			if( pCloneFrame != NULL && pCloneFrame->m_bMoveBoneY == true )
			{
				pFrame->TransformationMatrix._42 += pCloneFrame->m_fMoveBoneY;
			}
#endif

			// transform the bone
			D3DXMatrixMultiply( &pFrame->combineMatrix, &pFrame->TransformationMatrix, pmxBase );

			// 특정 프레임 스케일링
			if( pCloneFrame != NULL )
			{
				pCloneFrame->baseMatrix				= *pmxBase;
				pCloneFrame->TransformationMatrix	= pFrame->TransformationMatrix;
				pCloneFrame->combineMatrix			= pFrame->combineMatrix;

				// 특정본 스케일링은 스케일의 3펙터가 모두 존재해야지 수행한다.
				if(pCloneFrame->m_bScale == true)
				{					
					D3DXMATRIX matScale;
					D3DXMatrixIdentity( &matScale );
					matScale._11 = pCloneFrame->m_vScaleRel.x;
					matScale._22 = pCloneFrame->m_vScaleRel.y;
					matScale._33 = pCloneFrame->m_vScaleRel.z;
					D3DXMatrixMultiply( &pCloneFrame->combineMatrix, &matScale, &pCloneFrame->combineMatrix );
					
					if(pCloneFrame->m_bApplyChild == true)
						pFrame->combineMatrix = pCloneFrame->combineMatrix;	// 자식프레임까지 스케일링 적용되도록 한다.
				}				
			}			

#ifdef JIGGLEBONE_TEST
			// transform siblings by the same matrix
			if( pCloneFrame != NULL && pFrame->pFrameSibling )
			{
				CKTDXDeviceXSkinMesh::MultiAnimFrame *pCloneFrameTemp = (CKTDXDeviceXSkinMesh::MultiAnimFrame *)pCloneFrame->pFrameSibling;

				if(m_bJiggleMesh == true && pCloneFrameTemp->m_bJiggleFrame == true)
					UpdateAnimationFrame( (CKTDXDeviceXSkinMesh::MultiAnimFrame *) pFrame->pFrameSibling, pmxBase, (CKTDXDeviceXSkinMesh::MultiAnimFrame *)pCloneFrame->pFrameSibling, false );
				else
					UpdateAnimationFrame( (CKTDXDeviceXSkinMesh::MultiAnimFrame *) pFrame->pFrameSibling, pmxBase, (CKTDXDeviceXSkinMesh::MultiAnimFrame *)pCloneFrame->pFrameSibling );
			}

			// transform children by the transformed matrix - hierarchical transformation
			if( pCloneFrame != NULL && pFrame->pFrameFirstChild )
			{
				CKTDXDeviceXSkinMesh::MultiAnimFrame *pCloneFrameTemp = (CKTDXDeviceXSkinMesh::MultiAnimFrame *)pCloneFrame->pFrameFirstChild;

				if(m_bJiggleMesh == true && pCloneFrameTemp->m_bJiggleFrame == true)
					UpdateAnimationFrame( (CKTDXDeviceXSkinMesh::MultiAnimFrame *) pFrame->pFrameFirstChild, &pFrame->combineMatrix, (CKTDXDeviceXSkinMesh::MultiAnimFrame *)pCloneFrame->pFrameFirstChild, false );
				else
					UpdateAnimationFrame( (CKTDXDeviceXSkinMesh::MultiAnimFrame *) pFrame->pFrameFirstChild, &pFrame->combineMatrix, (CKTDXDeviceXSkinMesh::MultiAnimFrame *)pCloneFrame->pFrameFirstChild );
			}
#else
			// transform siblings by the same matrix
			if( pFrame->pFrameSibling )
				UpdateAnimationFrame( (CKTDXDeviceXSkinMesh::MultiAnimFrame *) pFrame->pFrameSibling, pmxBase, (CKTDXDeviceXSkinMesh::MultiAnimFrame *)pCloneFrame->pFrameSibling );

			// transform children by the transformed matrix - hierarchical transformation
			if( pFrame->pFrameFirstChild )
				UpdateAnimationFrame( (CKTDXDeviceXSkinMesh::MultiAnimFrame *) pFrame->pFrameFirstChild, &pFrame->combineMatrix, (CKTDXDeviceXSkinMesh::MultiAnimFrame *)pCloneFrame->pFrameFirstChild );
#endif
		}

#ifdef JIGGLEBONE_TEST
		__forceinline void CKTDGXSkinAnim::UpdateAnimationFrame( CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame, D3DXMATRIX* pmxBase, CKTDXDeviceXSkinMesh::MultiAnimFrame* pCloneFrame,  bool bGravity)
		{
			KTDXPROFILE();	

			int iLevel = 0;
			float fTension = m_fTension;
			float fGravity = 0.5f;
			float fDistance = 0.f;
			bool bJiggleBone = pCloneFrame->m_bJiggleFrame;

			//bGravity = true;
			if(bJiggleBone == true)
			{
				iLevel = pCloneFrame->m_iJiggleLevel;
				fTension /= iLevel;	
				fGravity /= iLevel;
			}			

			D3DXMATRIX matTrans;
			D3DXMatrixIdentity(&matTrans);
			D3DXMatrixMultiply( &matTrans, &pCloneFrame->TransformationMatrix, pmxBase );	

			D3DXVECTOR3 vPos0 = D3DXVECTOR3(pmxBase->_41, pmxBase->_42, pmxBase->_43);
			D3DXVECTOR3 vPos1 = D3DXVECTOR3(pCloneFrame->combineMatrix._41, pCloneFrame->combineMatrix._42, pCloneFrame->combineMatrix._43);
			D3DXVECTOR3 vPos2 = D3DXVECTOR3(matTrans._41, matTrans._42, matTrans._43);
			D3DXVECTOR3 vDownPos = D3DXVECTOR3(pmxBase->_41, pmxBase->_42 - 10.f, pmxBase->_43);

			if(pCloneFrame->m_fDistance <= 0.f)
			{
				fDistance = GetDistance(vPos2, vPos0);
				pCloneFrame->m_fDistance = fDistance;
			}
			else
			{
				fDistance = pCloneFrame->m_fDistance;
			}
			
			float fScale = 1.f;
			if(m_pUnitXSkinAnim != NULL)
			{				
				fScale = m_pUnitXSkinAnim->GetMatrix().GetScale().y;
				if(fScale >= 1.4f)
					fScale = 1.f;
			}
			fDistance *= fScale;
			

			D3DXVECTOR3 vPosU = vPos2 - vPos0;
			D3DXVECTOR3 vPosV = vPos1 - vPos0;	
			D3DXVECTOR3 vPosDir = (vPosU * fTension) + vPosV;	
			D3DXVECTOR3 vPosW = vDownPos - vPos0;	
			D3DXVec3Normalize(&vPosU, &vPosU);
			D3DXVec3Normalize(&vPosV, &vPosV);
			D3DXVec3Normalize(&vPosW, &vPosW);		

			if(bGravity == true)
			{
				vPosDir = (vPosV * fTension) + (vPosW * fGravity);
			}

			// transform the bone
			D3DXMatrixMultiply( &pCloneFrame->combineMatrix, &pCloneFrame->TransformationMatrix, pmxBase );

			if(bJiggleBone == true)
			{
				if(m_iExpand > 0)
				{
					float fMoveLen = GetDistance(vPos1, vPos2);	
					if( iLevel >= m_iExpand && fMoveLen >= 20.f )
					{
						fDistance += (fMoveLen * 0.48f);
					}
				}				

				D3DXVec3Normalize(&vPosDir, &vPosDir);
				pCloneFrame->combineMatrix._41 = pmxBase->_41 + (vPosDir.x * fDistance);
				pCloneFrame->combineMatrix._42 = pmxBase->_42 + (vPosDir.y * fDistance);
				pCloneFrame->combineMatrix._43 = pmxBase->_43 + (vPosDir.z * fDistance);
			}		

			pCloneFrame->baseMatrix				= *pmxBase;
			pCloneFrame->TransformationMatrix	= pFrame->TransformationMatrix;

			// transform siblings by the same matrix
			if( pFrame->pFrameSibling )
			{
				UpdateAnimationFrame( (CKTDXDeviceXSkinMesh::MultiAnimFrame *) pFrame->pFrameSibling, pmxBase, (CKTDXDeviceXSkinMesh::MultiAnimFrame *)pCloneFrame->pFrameSibling, bGravity );
			}

			// transform children by the transformed matrix - hierarchical transformation
			if( pFrame->pFrameFirstChild )
			{	
				UpdateAnimationFrame( (CKTDXDeviceXSkinMesh::MultiAnimFrame *) pFrame->pFrameFirstChild, &pCloneFrame->combineMatrix, (CKTDXDeviceXSkinMesh::MultiAnimFrame *)pCloneFrame->pFrameFirstChild, bGravity );
			}
		}
#endif

		__forceinline void CopyModelFrameStructure()
		{
			KTDXPROFILE();

#ifdef  X2OPTIMIZE_SKIN_ANIM_MESH_CRASH_BUG_FIX
            CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame = NULL;
#endif  X2OPTIMIZE_SKIN_ANIM_MESH_CRASH_BUG_FIX
			CKTDXDeviceXSkinMesh* pModel = NULL;
			for( int i = 0; i < (int)m_pModelXSkinMeshList.size(); i++ )
			{
				pModel = m_pModelXSkinMeshList[i];
				ASSERT( pModel != NULL );
				if( NULL == pModel )
					continue;

				LastErrorLog( pModel->GetDeviceID().c_str() ); // GetFrame() 함수 안에서 파일 이름을 사용하기 위해.
				for( int j = 0; j < m_FrameNum; j++ )
				{
//{{ robobeg : 2008-01-05
					//*pModel->m_FrameMatrixList[j] = *m_FrameMatrixList[j];
#ifdef  X2OPTIMIZE_SKIN_ANIM_MESH_CRASH_BUG_FIX
                    pFrame = pModel->GetFrame( j );
                    if ( pFrame != NULL )
                        pFrame->combineMatrix = *m_FrameMatrixList[j];
#else   X2OPTIMIZE_SKIN_ANIM_MESH_CRASH_BUG_FIX
                    pModel->GetFrame( j )->combineMatrix = *m_FrameMatrixList[j];
#endif  X2OPTIMIZE_SKIN_ANIM_MESH_CRASH_BUG_FIX
//}} robobeg : 2008-01-05
				}
			}
		}

		__forceinline void CopyFrame( CKTDXDeviceXSkinMesh::MultiAnimFrame* pDestFrame, CKTDXDeviceXSkinMesh::MultiAnimFrame* pSrcFrame )
		{
			KTDXPROFILE();
#ifdef  X2OPTIMIZE_SKIN_ANIM_MESH_CRASH_BUG_FIX
            if ( pDestFrame == NULL || pSrcFrame == NULL )
                return;
#endif  X2OPTIMIZE_SKIN_ANIM_MESH_CRASH_BUG_FIX

			pDestFrame->combineMatrix	= pSrcFrame->combineMatrix;

			// transform siblings by the same matrix
			if( pSrcFrame->pFrameSibling )
				CopyFrame( (CKTDXDeviceXSkinMesh::MultiAnimFrame *)pDestFrame->pFrameSibling, (CKTDXDeviceXSkinMesh::MultiAnimFrame *) pSrcFrame->pFrameSibling );

			// transform children by the transformed matrix - hierarchical transformation
			if( pSrcFrame->pFrameFirstChild )
				CopyFrame( (CKTDXDeviceXSkinMesh::MultiAnimFrame *)pDestFrame->pFrameFirstChild, (CKTDXDeviceXSkinMesh::MultiAnimFrame *) pSrcFrame->pFrameFirstChild );
		}
		__forceinline void DrawFrameStructure( CKTDXDeviceXSkinMesh::MultiAnimFrame* pModelFrame, int modelIndex )
		{
			KTDXPROFILE();
			if( pModelFrame->pMeshContainer )
			{
				if( pModelFrame->m_bDraw == true )
				{
					DrawFrame( pModelFrame, modelIndex );
				}
			}

			if( pModelFrame->pFrameSibling )
				DrawFrameStructure( (CKTDXDeviceXSkinMesh::MultiAnimFrame *) pModelFrame->pFrameSibling, modelIndex );

			if( pModelFrame->pFrameFirstChild )
			{
				if( pModelFrame->m_bDraw == false )
				{
					return;
				}
				DrawFrameStructure( (CKTDXDeviceXSkinMesh::MultiAnimFrame *) pModelFrame->pFrameFirstChild, modelIndex );
			}
		}
		__forceinline void DrawFrame( CKTDXDeviceXSkinMesh::MultiAnimFrame* pModelFrame, int modelIndex );

		void ResetAnimSet();
		bool LinkCollisionDataFrame( CKTDXCollision::CollisionData* pCollisionData );
		__forceinline CKTDXDeviceBaseTexture* SetNowTexture( CKTDXDeviceBaseTexture* orgTex, int stage,
											CKTDXDeviceXET* pTexChangeXET = NULL, CKTDXDeviceXET* pMultiTexXET = NULL, 
											const CKTDXDeviceXET::AniData* pAniData = NULL, float fAniTime = 0 );

		__forceinline bool SphereToSphere( const D3DXVECTOR3& center1, const float& radius1, 
			const D3DXVECTOR3& center2, const float& radius2, 
			D3DXVECTOR3* point = NULL )
		{
			KTDXPROFILE();
			float fDistance = GetDistance( center1, center2 );

			if( fDistance < (radius1 + radius2) )
			{
				if( point != NULL )
				{
					D3DXVECTOR3 dir = center2 - center1;
					D3DXVec3Normalize( &dir, &dir );
					*point = center1 + (dir * (fDistance / 2.0f));
				}
				
				return true;
			}

			return false;
		}


//{{ robobeg : 2008-10-20		
		//bool								m_bShow;
//}} robobeg : 2008-10-20		

		CKTDXDeviceXSkinMesh*				m_pAnimXSkinMesh;
		CKTDXDeviceXET*						m_pAnimAniXET;
		vector<CKTDXDeviceXSkinMesh*>		m_pModelXSkinMeshList;
		vector<CKTDXDeviceXET*>				m_pModelAniXETList;
		vector<const CKTDXDeviceXET::AniData*>	m_pModelAniDataList;
		vector<CKTDXDeviceXET*>				m_pModelMultiTexXETList;
		vector<CKTDXDeviceXET*>				m_pModelTexChangeXETList;
		CKTDGXRenderer*					    m_pRenderer;
		bool								m_bUseTex[3];

		CKTDXCollision::CollisionDataList            m_CollisionDataList;
		CKTDXCollision::CollisionDataList            m_AttackDataList;


		int									m_NowTrack;
		int									m_NewTrack;
#ifndef X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		float								m_fElapsedTime;
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		float								m_fAnimGlobalTime;
		float								m_fAnimCurrTime;
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        float                               m_fAnimCurrSpeed;
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		float								m_fAnimBeforeTime;
		float								m_fAnimMaxTime;
		int									m_NowAnimIndex;
		wstring								m_NowAnimName;
		LPD3DXANIMATIONCONTROLLER			m_pAC;

		int									m_NowPlayCount;
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        float                               m_fAnimOneshotTimer;
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		float								m_fPlaySpeed;

		vector<LPD3DXANIMATIONSET>			m_vAnimSetList;
		map<wstring, LPD3DXANIMATIONSET>	m_mAnimSetList;
		map<wstring, int>					m_mIndexByName;
		map<wstring, CKTDXDeviceXSkinMesh::MultiAnimFrame*>	m_mCloneFrameMap;

		XSKIN_ANIM_STATE					m_AnimState;
		XSKIN_ANIM_PLAYTYPE					m_PlayType;
		bool								m_bAnimEnd;
		float								m_fTransitionTime;

		bool								m_bApplyMotionOffset;
		D3DXVECTOR3							m_MotionMoveBack;
		D3DXVECTOR3							m_MotionMoveNow;
		D3DXVECTOR3							m_MotionOffset;

		D3DXMATRIX							m_DXMatrix;
		bool								m_bUseDXMatrix;		
		CKTDGMatrix::BILLBOARD_TYPE			m_BillBoardType;

		CKTDXDeviceXSkinMesh::MultiAnimFrame*		m_pCloneFrameRoot;
		int											m_FrameNum;
		D3DXMATRIX*									m_FrameMatrixList[MAX_FRAME_NUM];

		CKTDGXRenderer::RenderParam					m_RenderParam;

		int											m_ModelDetailPercent;

#ifdef SKIN_LIGHTFLOW
        bool                                        m_bLightFlow;
        CKTDGXRenderer::RENDER_TYPE                 m_lightFlowType;
        wstring                                     m_wstrLightFlowName;
        CKTDXDeviceXSkinMesh::MultiAnimFrame*		m_pLightFlow;

        float		m_fFlowMin;
        float		m_fFlowMax;
        float		m_fFlowSpeed;
        float		m_fNowPoint;
        float		m_fWide;

        D3DXCOLOR						m_coColor;
#endif

        std::vector<Alpha_SkinMesh> m_vecAlphaSkinMesh;
#ifdef ARA_CHARACTER_BASE
		std::vector<RenderParam_SkinMesh> m_vecRenderParamSkinMesh;
#endif

		static bool									m_sbTestUpdateAnimation;


		std::vector<CKTDXDeviceXSkinMesh*>		m_vecAddedSkinMesh;

#ifdef JIGGLEBONE_TEST
		bool	m_bJiggleMesh;
		float	m_fTension;
		unsigned char m_iExpand;
		CKTDGXSkinAnimPtr           m_pUnitXSkinAnim;		
#endif

		bool	m_bApplyMoveBone;
#ifdef FACE_OFF_MONSTER_HEAD_TEST
		bool			m_bHideNoRenderable;
#endif FACE_OFF_MONSTER_HEAD_TEST

#ifdef FIX_MESH_RENDER_BEFORE_INITINALIZE
		bool					m_bIsProcessFrameMove;
#endif // FIX_MESH_RENDER_BEFORE_INITINALIZE

};
