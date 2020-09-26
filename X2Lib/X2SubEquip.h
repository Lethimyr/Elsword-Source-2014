#pragma once

// cx2eqip에 attach되는 메시이다.
class CX2Eqip;
class CX2SubEquip : public CKTDGObject
{
private:
	CX2SubEquip( D3DXMATRIX* pFrameMatrix, const wstring& modelName );
#ifdef ATTACH_SKINMESH
	CX2SubEquip( D3DXMATRIX* pFrameMatrix, const wstring& modelName, bool bSkin );
#endif
	virtual ~CX2SubEquip(void);

public:
	static CX2SubEquip* CreateSubEquip( D3DXMATRIX* pFrameMatrix, const wstring& modelName ) { return new CX2SubEquip( pFrameMatrix, modelName ); }
#ifdef ATTACH_SKINMESH
	static CX2SubEquip* CreateSubEquip( D3DXMATRIX* pFrameMatrix, const wstring& modelName, bool bSkin ) { return new CX2SubEquip( pFrameMatrix, modelName, bSkin ); }
#endif

#ifdef ATTACH_SKINMESH
	virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
#endif

	virtual CKTDGXRenderer::RenderParam* GetRenderParam() { if( NULL != m_pSkinAnim) return m_pSkinAnim->GetRenderParam(); return NULL;}
	virtual RENDER_HINT OnFrameRender_Prepare();
	virtual void OnFrameRender_Draw();

	CKTDGXRenderer::RenderParam& GetNormalRenderParam() { return m_NormalRenderParam; }
	
#ifdef ATTACH_SKINMESH
	void SetUnitXSkinAnim(CKTDGXSkinAnimPtr pXSkinAnimPtr);
	void SetJiggle(bool bJiggle, unsigned char fTension);
	void SetRenderParam( CKTDGXRenderer::RenderParam& pRenderParam );
	bool GetIsSkinMesh() { return m_bSkinMesh; }
#endif

	void SetScale(float fVal) { m_fScale = fVal; }
	void SetRotate(D3DXVECTOR3 vRot) { m_vRotate = vRot; }
	void SetOffset(D3DXVECTOR3 vOffset) { m_vOffset = vOffset; }

private:
	D3DXMATRIX* m_pFrameMatrix;	// attach된 부모 eqip의 해당 frame의 combine_matrix

	CKTDXDeviceXMesh* m_pNormalXMesh;
	CKTDGXRenderer::RenderParam m_NormalRenderParam;

#ifdef ATTACH_SKINMESH
	bool					m_bSkinMesh;
	CKTDGXSkinAnim			*m_pSkinAnim;
	CKTDXDeviceXSkinMesh	*m_pSkinModel;
#endif

	float		m_fScale;
	D3DXVECTOR3 m_vRotate;
	D3DXVECTOR3 m_vOffset;
};
