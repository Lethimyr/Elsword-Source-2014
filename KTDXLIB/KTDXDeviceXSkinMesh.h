/** @file   KTDXDeviceXSkinMesh.h
    @date   jintaeks on 2008-10-15, 15:05
            - 메시가 사용하는 vertex buffer와 texture를 local memory에 preload하는 기능을 구현한다.
*/

#pragma once

// 인터페이스 변경사항
//
// 1. MultiAnimMC의 pSkinInfo 는 더이상 유지되지 않음. 스킨 메쉬 여부를 확인하려면 m_dwMaxSkinningVertices > 0 을 체크할 것
// 3. MultiAnimMC의 m_pBufBoneCombos는 m_pAttributeRange, m_pdwBoneCombinationExtra 로 대체됨
//    GetAttributeRange, GetBoneId로 접근하시오.
// 5. MultiAnimMC의 m_apmxBonePointers는 m_ppBoneFrames로 대체됨
//    m_ppBoneFrames[ BoneId ].combineMatrix 로 엑세스하면 됨
// 6. CKTDXDeviceXSkinMesh의 m_FrameMatrixList은 m_apFrameList로 대체되었음
//    매트릭스는 GetFrame( iFrameIndex )->combineMatrix 로 접근하시오.
// 7. CKTDXDeviceXSkinMesh의 m_DrawFrameList로의 외부 엑세스를 막음
//    GetDrawFrame으로 접근하시오.
// 

#define MAX_FRAME_NUM 255
#define MATRIX_PALETTE_SIZE_DEFAULT 20

#define KOGSKINNEDMESH_FORMAT_MAGIC           (MAKEFOURCC('K','S','M',' '))
#define KOGSKINNEDMESH_FORMAT_VERSION         0x0200

class CKTDXDeviceXSkinMesh	: public CKTDXDevice
{
	public:

		struct  XSkinMeshProxy;

		class CMultiAnimAllocateHierarchy : public ID3DXAllocateHierarchy
		{
			public:
                CMultiAnimAllocateHierarchy( const std::string& strMoveBoneName_ ) : m_strMoveBoneNameMB( strMoveBoneName_ ) {}
				CMultiAnimAllocateHierarchy() {}
				
				// callback to create a D3DXFRAME-derived object and initialize it
				STDMETHOD( CreateFrame )( THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame );
				// callback to create a D3DXMESHCONTAINER-derived object and initialize it
				STDMETHOD( CreateMeshContainer )( THIS_ LPCSTR Name, CONST D3DXMESHDATA * pMeshData, 
								CONST D3DXMATERIAL * pMaterials, CONST D3DXEFFECTINSTANCE * pEffectInstances,
								DWORD NumMaterials, CONST DWORD * pAdjacency, LPD3DXSKININFO pSkinInfo, 
								LPD3DXMESHCONTAINER * ppNewMeshContainer );
				// callback to release a D3DXFRAME-derived object
				STDMETHOD( DestroyFrame )( THIS_ LPD3DXFRAME pFrameToFree );
				// callback to release a D3DXMESHCONTAINER-derived object
				STDMETHOD( DestroyMeshContainer )( THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree );

                //CKTDXDeviceXSkinMesh* m_pKTDXSkinMesh;
				//std::wstring	m_wstrMoveBoneName;
				std::string	m_strMoveBoneNameMB;
		};

		struct MultiAnimFrame : public D3DXFRAME
		{
			D3DXMATRIX		baseMatrix;
			D3DXMATRIX		combineMatrix;
			float			fBoundingRadius;
			bool			m_bDraw;
			bool			m_bMoveBone;
            int             m_iIndex;

			bool			m_bScale;
			bool			m_bApplyChild;
			D3DXVECTOR3		m_vScaleRel;	// 절대값

#ifdef X2VIEWER
            BOOL                    m_bSelected;
#endif
#ifdef JIGGLEBONE_TEST
			float			m_fDistance;
			bool			m_bJiggleFrame;			
			char			m_iJiggleLevel;
#endif
#ifdef MOVE_BONE
			bool			m_bMoveBoneY;
			float			m_fMoveBoneY;
#endif

			MultiAnimFrame()
			{
				Name				= NULL;
				pMeshContainer		= NULL;
				pFrameSibling		= NULL;
				pFrameFirstChild	= NULL;
				fBoundingRadius		= 0.0f;
				m_bDraw				= true;
				m_bMoveBone			= false;
                m_iIndex			= -1;

				m_bScale			= false;
				m_bApplyChild		= true;
				m_vScaleRel			= D3DXVECTOR3(1.f, 1.f, 1.f);

#ifdef X2VIEWER
                m_bSelected         = false;
#endif
#ifdef JIGGLEBONE_TEST
				m_fDistance			= -1.f;
				m_bJiggleFrame		= false;				
				m_iJiggleLevel		= 0;
#endif
#ifdef MOVE_BONE
				m_bMoveBoneY		= false;
				m_fMoveBoneY		= 0.f;
#endif
			}

			~MultiAnimFrame()
			{
				SAFE_DELETE( Name );
			}

            HRESULT WriteToFileSerializer( KFileSerializer& kInOutFile_, DWORD dwInOffset_ ) const;
		};

		struct MultiAnimMC : public D3DXMESHCONTAINER
		{
#ifndef _KSMTOOL
			CKTDXDeviceTexture**	m_apTextures;
#endif // #ifndef

// 			DWORD					m_cPMeshes;
// 			LPD3DXPMESH*			m_ppPMeshes;
// 			LPD3DXPMESH				m_pPMeshFull;
// 			int						m_iPMeshCur;

            DWORD                   m_dwNumBones;
			D3DXMATRIX *			m_amxBoneOffsets;  // Bone offset matrices retrieved from pSkinInfo
            MultiAnimFrame**        m_ppBoneFrames;
			DWORD					m_dwNumPaletteEntries;
			DWORD				    m_dwMaxSkinningVertices;
			DWORD				    m_dwNumAttrGroups;
            LPD3DXATTRIBUTERANGE    m_pAttributeRange;
            DWORD*                  m_pdwBoneCombinationExtra;



            MultiAnimMC()
            {
#ifndef _KSMTOOL
			    m_apTextures = NULL;
#endif // #ifndef
                Name = NULL;
                MeshData.Type = D3DXMESHTYPE_MESH;
                MeshData.pMesh = NULL;

				
// 				m_cPMeshes		= 0;
// 				m_ppPMeshes		= NULL;
// 				m_pPMeshFull	= NULL;
// 				m_iPMeshCur		= 0;

                pMaterials = NULL;
                pEffects = NULL;
                NumMaterials = 0;
                pAdjacency = NULL;
                pSkinInfo = NULL;
                pNextMeshContainer = NULL;
                m_dwNumBones = 0;
                m_amxBoneOffsets = NULL;
                m_ppBoneFrames = NULL;
                m_dwNumPaletteEntries = 0;
                m_dwMaxSkinningVertices = 0;
                m_dwNumAttrGroups = 0;
                //m_pBufBoneCombos = NULL;
                m_pAttributeRange = NULL;
                m_pdwBoneCombinationExtra = NULL;
            }//MultiAnimMC()

			HRESULT SetupBonePtrs( D3DXFRAME * pFrameRoot );
            HRESULT OptimizeMesh();
            HRESULT ConvertToSkinnedMesh();
#ifdef _KSMTOOL
            HRESULT WriteToFileSerializer( KFileSerializer& kInOutFile_ ) const;
#endif // #ifdef _KSMTOOL
            HRESULT ReadKSM( CKTDXDeviceXSkinMesh::XSkinMeshProxy& proxy_, const KMemoryDeserializer& kInMem_, DWORD dwInOffset_ );
#ifndef _KSMTOOL
            void    LoadTextures();
            void    ReleaseUnusedInfo();
#endif // #ifndef _KSMTOOL

            inline const D3DXATTRIBUTERANGE& GetAttributeRange( DWORD dwInSubset_ ) const 
            { 
                if(dwInSubset_ >= m_dwNumAttrGroups)    
                    ErrorLog(KEM_ERROR205);
                ASSERT( dwInSubset_ < m_dwNumAttrGroups ); 
                if(m_pAttributeRange == NULL)    
                    ErrorLog(KEM_ERROR206);
                ASSERT( m_pAttributeRange != NULL );
                return m_pAttributeRange[ dwInSubset_ ]; 
            }
            inline DWORD                     GetBoneId( DWORD dwInSubset_, DWORD dwInInPalette_ ) const
            { 
                if(dwInSubset_ >= m_dwNumAttrGroups || dwInInPalette_ >= m_dwNumPaletteEntries)    
                    ErrorLog(KEM_ERROR207);
                ASSERT( dwInSubset_ < m_dwNumAttrGroups && dwInInPalette_ < m_dwNumPaletteEntries );
                if(m_pdwBoneCombinationExtra == NULL)    
                    ErrorLog(KEM_ERROR208);
                ASSERT( m_pdwBoneCombinationExtra != NULL );
                return  m_pdwBoneCombinationExtra[ dwInSubset_ * m_dwNumPaletteEntries + dwInInPalette_ ];
            }//

            /// managed된 리소스를 local memory에 미리 올리는 일을 한다. - jintaeks on 2008-10-15, 14:53
            void PreLoad();
		};//struct MultiAnimMC : public D3DXMESHCONTAINER

		struct  XSkinMeshProxy
		{
			CKTDXDevice*                        m_pThis;
			std::wstring                        m_DeviceID;
			MultiAnimFrame*						m_pFrameRoot;
			DWORD                               m_dwFrameNum;
			MultiAnimFrame*                     m_apFrameList[MAX_FRAME_NUM];
			D3DXVECTOR3							m_vCenter;
			float								m_fMaxBoundingRadius;
			LPD3DXANIMATIONCONTROLLER		    m_pAC;
			std::string							m_strMoveBoneNameMB;

#ifndef _KSMTOOL       
			CKTDXDeviceXET*						m_pXET;
			CKTDXCollision::CollisionDataList   m_CollisionDataList;
			CKTDXCollision::CollisionDataList   m_AttackDataList;
#endif // #ifndef _KSMTOOL    


			MultiAnimFrame*						m_apDrawFrameList[MAX_FRAME_NUM];
			DWORD   							m_dwDrawFrameNum;
			DWORD                               m_dwWorkingPaletteSize;
			D3DXMATRIX*							m_amxWorkingPalette;
			//DWORD								m_dwBoneNum;
			DWORD                               m_dwMaxSkinningVertices;

			XSkinMeshProxy( const std::wstring& strDeviceID, CKTDXDevice* pThis, const std::string& strMoveBoneMB );
			~XSkinMeshProxy();

			const std::wstring&  GetDeviceID() const { return m_DeviceID; }
			MultiAnimFrame*	    GetFrameRoot()  { return m_pFrameRoot; }
			DWORD   GetFrameNum() const         { return m_dwFrameNum; }
			MultiAnimFrame* GetFrame( DWORD dwInIndex_ )
			{
				return ( dwInIndex_ < m_dwFrameNum ) ? m_apFrameList[ dwInIndex_ ] : NULL;
			}//MUltiAnimFrame* GetFrame( int iInIndex_ )

			HRESULT LoadD3DXSkinnedMesh( const void* pInData_, DWORD dwInSize_ );
			HRESULT LoadKSkinnedMesh( const void* pInData_, DWORD dwInSize_ );

			HRESULT SetupFrameList( MultiAnimFrame* pFrame );
			void    EstimateDrawFrameWorkingPaletteSizeAndBoneNum();
			HRESULT SetupBonePtrs();
			void    SetupDrawFrameList( MultiAnimFrame* pFrame );
			void    ComputeCollisionData( bool bInValidRadius_ );
			void    AddCollisionData( MultiAnimFrame* pFrame, bool bInValidRadius_ );

			void    OptimizeDrawFrameMesh();
			HRESULT ConvertDrawFrameToSkinnedMesh();

#ifndef _KSMTOOL
			void    LoadTextures();
			void    ReleaseUnusedInfo();
			//#ifdef UNIT_BACKGROUND_LOADING_TEST // 2008-12-12
			HRESULT Load_LuaData( DWORD dwParam_ );
			//#endif // UNIT_BACKGROUND_LOADING_TEST // 2008-12-12
#endif // #ifndef _KSMTOOL

		};//struct  XSkinMeshProxy

	public:

		CKTDXDeviceXSkinMesh( LPDIRECT3DDEVICE9 pd3dDevice, std::wstring fileName, std::wstring moveBoneName = L"Bip01" );


		//virtual HRESULT _Load( bool bSkipStateCheck = false );
		//virtual HRESULT _UnLoad();

        /// managed된 리소스를 local video memory에 미리 올린다.
        /// - jintaeks on 2008-10-15, 14:48
        void PreLoad();

		LPD3DXANIMATIONCONTROLLER GetCloneAC()
		{
			HRESULT hr;
			if( m_pAC == NULL )
				return NULL;

			LPD3DXANIMATIONCONTROLLER pNewAC = NULL;
			hr = m_pAC->CloneAnimationController( m_pAC->GetMaxNumAnimationOutputs(),
													m_pAC->GetMaxNumAnimationSets(),
													m_pAC->GetMaxNumTracks(),
													m_pAC->GetMaxNumEvents(),
													&pNewAC );
			if( FAILED( hr ) )
				return NULL;

			return pNewAC;
		}
		//noet : X2 메쉬뷰어에서 애니메이션 추가 기능을 구현하기위해 함수추가
		HRESULT Reset_Animation( LPD3DXANIMATIONCONTROLLER pAnimSet);

        const std::wstring&   GetWMoveBoneName() const { return m_wstrMoveBoneName; }
        const std::string&    GetMoveBoneName() const { return m_strMoveBoneNameMB; }

		MultiAnimFrame*	GetFrameRoot() const { return m_pFrameRoot; }
        DWORD           GetWorkingPaletteSize() const { return m_dwWorkingPaletteSize; }
        DWORD           GetMaxSkinningVertices() const { return m_dwMaxSkinningVertices; }
		D3DXMATRIX*		GetWorkingPalette() { return m_amxWorkingPalette; }
        MultiAnimFrame* FindFrame( const char* pszInName_ ) const;

		bool    GetIsOnlyModelData() const { return m_pAC == NULL; }
		//DWORD   GetBoneNum() const { return m_dwBoneNum; }
		DWORD   GetFrameNum() const { return m_dwFrameNum; }
		DWORD   GetDrawFrameNum() const { return m_dwDrawFrameNum; }

		const   D3DXVECTOR3& GetCenter() const { return m_vCenter; }
		const	float   GetBoundingRadius() const { return m_fMaxBoundingRadius; }

// #ifndef _KSMTOOL
// 
// 		CKTDXDeviceXET* SetXET( CKTDXDeviceXET* pXET ){ m_pXET = pXET; }
// 		CKTDXDeviceXET* GetXET() const { return m_pXET; }
// 
//         virtual HRESULT Load_LuaData( DWORD dwParam_ );
// #endif // #ifndef _KSMTOOL

        MultiAnimFrame* GetFrame( DWORD dwInIndex_ ) const 
        {
            if(dwInIndex_ >= m_dwFrameNum || m_apFrameList[ dwInIndex_ ] == NULL)    
			{
				WCHAR wszText[256] = L"";
				wstring wstrDeviceId = L"";
				ConvertCharToWCHAR( wstrDeviceId, g_CKTDXLog.GetLastErrorLog() );
				swprintf( wszText, sizeof(wszText) / sizeof(WCHAR) - 1, L"max: %d, dwInIndex_: %d, deviceid : %s", (int)m_dwFrameNum, (int)dwInIndex_, wstrDeviceId.c_str() );
                ErrorLogMsg(KEM_ERROR209, wszText);
				//ErrorLog(KEM_ERROR209);	
#ifdef  X2OPTIMIZE_SKIN_ANIM_MESH_CRASH_BUG_FIX
                return NULL;
#endif  X2OPTIMIZE_SKIN_ANIM_MESH_CRASH_BUG_FIX
			}
            ASSERT( dwInIndex_ < m_dwFrameNum && m_apFrameList[ dwInIndex_ ] != NULL );
            return m_apFrameList[ dwInIndex_ ];
        }//MUltiAnimFrame* GetFrame( int iInIndex_ )

        MultiAnimFrame*  GetDrawFrame( DWORD dwInIndex_ ) const
        { 
            if(dwInIndex_ >= m_dwDrawFrameNum || m_apDrawFrameList[ dwInIndex_ ] == NULL)    
			{
				//WCHAR wszText[256] = L"";
				//swprintf( wszText, sizeof(wszText) / sizeof(WCHAR) - 1, L"max: %d, dwInIndex_: %d, deviceid : %s", (int)m_dwDrawFrameNum, (int)dwInIndex_, GetDeviceID() );
				//ErrorLogMsg(KEM_ERROR210, wszText);
                ErrorLog(KEM_ERROR210);
#ifdef  X2OPTIMIZE_SKIN_ANIM_MESH_CRASH_BUG_FIX
                return NULL;
#endif  X2OPTIMIZE_SKIN_ANIM_MESH_CRASH_BUG_FIX
			}
            ASSERT( dwInIndex_ < m_dwDrawFrameNum && m_apDrawFrameList[ dwInIndex_ ] != NULL ); 
            return m_apDrawFrameList[ dwInIndex_ ];
        }//GetDrawFrame()

#ifdef _KSMTOOL

        HRESULT WriteToFileSerializer( KFileSerializer& kInOutFile_ ) const;

#endif // #ifdef _KSMTOOL

		
        void SetAlphaRender(bool flag) { m_bAlphaRender = flag; }
        bool GetAlphaRender() { return m_bAlphaRender; }
#ifdef ARA_CHARACTER_BASE
		void SetUseRenderParam( bool bValue_ ) { m_bUseRenderParam = bValue_; }
		bool GetUseRenderParam() { return m_bUseRenderParam; }
#endif
#ifdef FACE_OFF_MONSTER_HEAD_TEST
		void SetNoRenderable(bool flag) { m_bNoRenderable = flag; }
		bool GetNoRenderable() { return m_bNoRenderable; }
#endif FACE_OFF_MONSTER_HEAD_TEST
        const CKTDXCollision::CollisionDataList& GetCollisionDataList() const { return m_CollisionDataList; }
        const CKTDXCollision::CollisionDataList& GetAttackDataList() const { return m_AttackDataList; }





//#ifdef MONSTER_DIE_PARTICLE_TEST
//		bool GetSurfaceVertices( std::vector<D3DXVECTOR3>& vecVertices );
//#endif MONSTER_DIE_PARTICLE_TEST

		CKTDXDeviceXET* SetXET( CKTDXDeviceXET* pXET ){ m_pXET = pXET; }
		CKTDXDeviceXET* GetXET() const { return m_pXET; }

protected:

		virtual ~CKTDXDeviceXSkinMesh(void);

		virtual HRESULT _Load( bool bSkipStateCheck = false
#ifdef	X2OPTIMIZE_SOUND_BACKGROUND_LOAD
				, bool bBackgroundQueueing = false
#endif	X2OPTIMIZE_SOUND_BACKGROUND_LOAD			
			);
		virtual HRESULT _UnLoad();

private:

		MultiAnimFrame*                     m_apFrameList[MAX_FRAME_NUM];
		MultiAnimFrame*						m_apDrawFrameList[MAX_FRAME_NUM];

#ifndef _KSMTOOL        
		CKTDXDeviceXET*						m_pXET;
#endif // #ifndef _KSMTOOL        

		MultiAnimFrame*						m_pFrameRoot;
		LPD3DXANIMATIONCONTROLLER			m_pAC;

		DWORD                               m_dwFrameNum;

		DWORD   							m_dwDrawFrameNum;
		DWORD                               m_dwWorkingPaletteSize;
		D3DXMATRIX*							m_amxWorkingPalette;
		//DWORD								m_dwBoneNum;
        DWORD                               m_dwMaxSkinningVertices;
		bool                                m_bAlphaRender;
#ifdef ARA_CHARACTER_BASE
		bool								m_bUseRenderParam;
#endif

		D3DXVECTOR3							m_vCenter;
		float								m_fMaxBoundingRadius;
		std::wstring						m_wstrMoveBoneName;
		std::string							m_strMoveBoneNameMB;

#ifndef _KSMTOOL

		CKTDXCollision::CollisionDataList            m_CollisionDataList;
		CKTDXCollision::CollisionDataList            m_AttackDataList;

#endif // #ifndef _KSMTOOL

#ifdef FACE_OFF_MONSTER_HEAD_TEST
		bool								m_bNoRenderable;
#endif FACE_OFF_MONSTER_HEAD_TEST
};


inline bool GetFramePos( D3DXVECTOR3* pPos, CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame )
{
	//KTDXPROFILE();

	if( pFrame != NULL )
	{
		pPos->x = pFrame->combineMatrix._41;
		pPos->y = pFrame->combineMatrix._42;
		pPos->z = pFrame->combineMatrix._43;

		return true;
	}

	pPos->x = 0.0f;
	pPos->y = 0.0f;
	pPos->z = 0.0f;
	return false;
}

inline void GetFrameRotateDegree( D3DXVECTOR3* pvRotateDegree, CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame )
{
	//KTDXPROFILE();

	if( pFrame != NULL )
	{
		*pvRotateDegree = GetDecomposeAngle( &pFrame->combineMatrix );

		pvRotateDegree->x = D3DXToDegree( pvRotateDegree->x ); 
		pvRotateDegree->y = D3DXToDegree( pvRotateDegree->y ); 
		pvRotateDegree->z = D3DXToDegree( pvRotateDegree->z ); 
	}
	else
	{
		pvRotateDegree->x = 0.f;
		pvRotateDegree->y = 0.f;
		pvRotateDegree->z = 0.f;
	} 
}

inline void SetFrameScale( D3DXVECTOR3* vScale, bool bScaleChild, CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame )
{
	//KTDXPROFILE();

	if( pFrame != NULL )
	{
		pFrame->m_bScale = true;
		pFrame->m_bApplyChild = bScaleChild;
		pFrame->m_vScaleRel = *vScale;	 
	}
}

inline void SetFrameScaleRestore(CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame)
{
	if( pFrame != NULL )
	{
		pFrame->m_bScale = false;
		pFrame->m_bApplyChild = true;
		pFrame->m_vScaleRel = D3DXVECTOR3(1.f, 1.f, 1.f);
	}
}

inline void SetFrameMoveBoneY(CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame, float fMove)
{
#ifdef MOVE_BONE
	if( pFrame != NULL )
	{
		pFrame->m_bMoveBoneY = true;
		pFrame->m_fMoveBoneY = fMove;
	}
#endif
}
