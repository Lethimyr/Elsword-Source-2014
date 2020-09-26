#pragma once


//{{ seojt // 2009-1-14, 23:42
class CX2WorldObjectLightFlow;
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::intrusive_ptr<CX2WorldObjectLightFlow>  CX2WorldObjectLightFlowPtr;
#else   X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::shared_ptr<CX2WorldObjectLightFlow>  CX2WorldObjectLightFlowPtr;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
//}} seojt // 2009-1-14, 23:42


class CX2WorldObjectLightFlow : public CX2WorldObject
{
private:
	CX2WorldObjectLightFlow( bool bBackgroundLoad_ );
	virtual ~CX2WorldObjectLightFlow(void);


public: 
	static CX2WorldObjectLightFlowPtr CreateWorldObjectLightFlow( bool bBackgroundLoad_ )
    {
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
        CX2WorldObjectLightFlowPtr pObject( new CX2WorldObjectLightFlow( bBackgroundLoad_ ) );
#else   X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
        CX2WorldObjectLightFlowPtr pObject( new CX2WorldObjectLightFlow( bBackgroundLoad_ )
            , CKTDGObject::KTDGObjectDeleter() );
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
        return pObject;
    }


	virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );

//{{ robobeg : 2008-10-15
    virtual RENDER_HINT   OnFrameRender_Prepare();
	//virtual HRESULT OnFrameRender();
    virtual void              OnFrameRender_Draw();
//}} robobeg : 2008-10-15



	wstring& GetXMeshName() { return m_XMeshName; }
	
	//LUA
	void SetXMesh_LUA( const char* pFileName );

	CKTDGXRenderer::RenderParam* GetRenderParam() { return &m_RenderParam; }
	void SetFlowMin( float fFlowMin ) { m_fFlowMin = fFlowMin; }
	void SetFlowMax( float fFlowMax ) { m_fFlowMax = fFlowMax; }
	void SetFlowSpeed( float fFlowSpeed ) { m_fFlowSpeed = fFlowSpeed; }
	void SetWide( float fWide ) { m_fWide = fWide; }
	void SetNowPoint( float fNowPoint ) { m_fNowPoint = fNowPoint; }
	void SetRenderType( CKTDGXRenderer::RENDER_TYPE renderType ) { m_RenderParam.renderType = renderType; }
	void SetBillboardType( int billboardType ){ m_BillboardType = (CKTDGMatrix::BILLBOARD_TYPE)billboardType; }



protected:
	wstring							m_XMeshName;
	CKTDXDeviceXMesh*				m_pXMesh;
	bool							m_bXMeshLoaded;

	CKTDGXRenderer::RenderParam		m_RenderParam;
	float		m_fFlowMin;
	float		m_fFlowMax;
	float		m_fFlowSpeed;
	float		m_fNowPoint;
	float		m_fWide;

	CKTDGMatrix::BILLBOARD_TYPE		m_BillboardType;
};
