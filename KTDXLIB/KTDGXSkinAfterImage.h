#pragma once

class CKTDGXSkinAfterImage : public CKTDGObject
{
	public:
		struct AfterImageData
		{
            // 내부적으로 관리하는 객체이므로 smart pointer를 사용하지 않는다.
            // - jintaeks on 2009-01-12, 14:40
			CKTDGXSkinAnim*	pAnim;
			float			fAlpha;

            AfterImageData() : pAnim(NULL), fAlpha(0.0f) {}
		};

	private:
		//CKTDGXSkinAfterImage() : CKTDGObject( NULL ) { ASSERT( !"invalid" ); }
		CKTDGXSkinAfterImage( CKTDGXSkinAnim* pOriginAnim, int imageNum, int layer );
		virtual ~CKTDGXSkinAfterImage(void);
	

	public:

#ifdef MODIFY_AFTER_IMAGE
		void UpdateEquipView( CKTDGXSkinAnim* pOriginAnim );
		void SetpHairSkinMesh ( CKTDXDeviceXSkinMesh* pSkinMesh_ ){ m_pHairXSkinMesh = pSkinMesh_; }
#endif // MODIFY_AFTER_IMAGE


		static CKTDGXSkinAfterImage* CreateSkinAfterImage( CKTDGXSkinAnim* pOriginAnim, int imageNum, int layer )
		{
#ifndef WORLD_TOOL		// 09.12.24 태완 : 월드툴에서 오작동.
			return new CKTDGXSkinAfterImage( pOriginAnim, imageNum, layer );
#else
			return NULL;
#endif WORLD_TOOL
		};


		virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
//{{ robobeg : 2008-10-24
		//virtual HRESULT OnFrameRender();
        virtual RENDER_HINT   OnFrameRender_Prepare();
        virtual void              OnFrameRender_Draw();
//}} robobeg : 2008-10-24

		void AddMesh( CKTDXDeviceXSkinMesh* pMesh );
		void Enable(){ m_bEnable = true; }
		void Disable(){ m_bEnable = false; }
		bool GetEnable() { return m_bEnable; }

		void SetAfterImageColor( const D3DXCOLOR& afterImageColor ){ m_AfterImageColor = afterImageColor; }
		void SetNextImageTime( float nextImageTime ){ m_NextImageTimeNow = m_NextImageTime = nextImageTime; }
		void SetHideImageSpeed( float hideImageSpeed ){ m_HideImageSpeed = hideImageSpeed; }

		CKTDGXSkinAnim* GetXSkinAnim() { return m_pOriginalAnim; }


		CKTDGXRenderer::RenderParam* GetRenderParam() 
		{ 
			if( true == m_AfterImageList.empty() )
				return NULL;

			return m_AfterImageList[0].pAnim->GetRenderParam();
		}

		void SetRenderParam( CKTDGXRenderer::RenderParam& renderParam )
		{
			BOOST_TEST_FOREACH( AfterImageData&, afterImageData, m_AfterImageList )
			{
				CKTDGXRenderer::RenderParam* pRenderParam = afterImageData.pAnim->GetRenderParam();
				*pRenderParam = renderParam;
			}
		}


	private:
		void CopyMatrix( int matrixList, int saveMatrixIndex, CKTDXDeviceXSkinMesh::MultiAnimFrame* pRootFrame );

		CKTDGXSkinAnim*					m_pOriginalAnim;
        //CKTDGXSkinAnimPtr               m_pOriginalAnim;
		bool							m_bEnable;
		int								m_AfterImageNum;
		int								m_NewImageIndex;
//{{ robobeg : 2008-10-17
		//int								m_Layer;
//}} robobeg : 2008-10-17

		D3DXCOLOR						m_AfterImageColor;
		float							m_NextImageTime;
		float							m_NextImageTimeNow;
		float							m_HideImageSpeed;

		vector<AfterImageData>			m_AfterImageList;

#ifdef MODIFY_AFTER_IMAGE // 오현빈 // 2013-07-19	
		CKTDXDeviceXSkinMesh* m_pHairXSkinMesh; // 모자 착용 중 애프터 이미지에 현재 착용중인 Hair를 추가 하기 위한 변수
#endif // MODIFY_AFTER_IMAGE
};
