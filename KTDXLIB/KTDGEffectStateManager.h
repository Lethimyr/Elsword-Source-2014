#pragma once

#define DEVICE_STATE_STACK_SIZE     512

class   CKTDGEffectStateManager : public ID3DXEffectStateManager
{
public:

    CKTDGEffectStateManager();

    // The user must correctly implement QueryInterface, AddRef, and Release.

    // IUnknown
    STDMETHOD(QueryInterface)(THIS_ REFIID iid, LPVOID *ppv);
    STDMETHOD_(ULONG, AddRef)(THIS);
    STDMETHOD_(ULONG, Release)(THIS);

    // The following methods are called by the Effect when it wants to make 
    // the corresponding device call.  Note that:
    // 1. Users manage the state and are therefore responsible for making the 
    //    the corresponding device calls themselves inside their callbacks.  
    // 2. Effects pay attention to the return values of the callbacks, and so 
    //    users must pay attention to what they return in their callbacks.

    STDMETHOD(SetTransform)(THIS_ D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX *pMatrix);
    STDMETHOD(SetMaterial)(THIS_ CONST D3DMATERIAL9 *pMaterial);
    STDMETHOD(SetLight)(THIS_ DWORD Index, CONST D3DLIGHT9 *pLight);
    STDMETHOD(LightEnable)(THIS_ DWORD Index, BOOL Enable);
    STDMETHOD(SetRenderState)(THIS_ D3DRENDERSTATETYPE State, DWORD Value);
    STDMETHOD(SetTexture)(THIS_ DWORD Stage, LPDIRECT3DBASETEXTURE9 pTexture);
    STDMETHOD(SetTextureStageState)(THIS_ DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value);
    STDMETHOD(SetSamplerState)(THIS_ DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD Value);
    STDMETHOD(SetNPatchMode)(THIS_ FLOAT NumSegments);
    STDMETHOD(SetFVF)(THIS_ DWORD FVF);
    STDMETHOD(SetVertexShader)(THIS_ LPDIRECT3DVERTEXSHADER9 pShader);
#ifdef X2OPTIMIZE_SETSHADERCONSTANT
	__forceinline STDMETHOD(SetVertexShaderConstantF)(THIS_ UINT RegisterIndex, CONST FLOAT *pConstantData, UINT RegisterCount);
#else//X2OPTIMIZE_SETSHADERCONSTANT
	STDMETHOD(SetVertexShaderConstantF)(THIS_ UINT RegisterIndex, CONST FLOAT *pConstantData, UINT RegisterCount);
#endif//X2OPTIMIZE_SETSHADERCONSTANT
	STDMETHOD(SetVertexShaderConstantI)(THIS_ UINT RegisterIndex, CONST INT *pConstantData, UINT RegisterCount);
	STDMETHOD(SetVertexShaderConstantB)(THIS_ UINT RegisterIndex, CONST BOOL *pConstantData, UINT RegisterCount);
    STDMETHOD(SetPixelShader)(THIS_ LPDIRECT3DPIXELSHADER9 pShader);
    STDMETHOD(SetPixelShaderConstantF)(THIS_ UINT RegisterIndex, CONST FLOAT *pConstantData, UINT RegisterCount);
    STDMETHOD(SetPixelShaderConstantI)(THIS_ UINT RegisterIndex, CONST INT *pConstantData, UINT RegisterCount);
    STDMETHOD(SetPixelShaderConstantB)(THIS_ UINT RegisterIndex, CONST BOOL *pConstantData, UINT RegisterCount);

    HRESULT PrepareEffectState();
    HRESULT RestoreEffectState();

    bool    IsRenderStateChanged( D3DRENDERSTATETYPE State ) const 
    { 
        ASSERT( State >= D3DRENDERSTATETYPE( 0 ) && State < D3DRS_MAX );
        return m_bitsetRenderState.test( State ); 
    }//IsRenderStateChanged()
    bool    IsTextureStageStateChanged( DWORD Stage, D3DTEXTURESTAGESTATETYPE Type ) const
    {
        ASSERT( Stage >= 0 && Stage < TEXTURE_STAGE_NUM && Type >= D3DTEXTURESTAGESTATETYPE( 0 ) && Type < D3DTSS_MAX );
        return m_bitsetTextureStageState.test( Stage * D3DTSS_MAX + Type );
    }//IsTextureStageStateChanged()
    bool    IsSamplerStateChanged( DWORD Sampler, D3DSAMPLERSTATETYPE Type ) const
    {
        ASSERT( Sampler >= 0 && Sampler < SAMPLER_STAGE_NUM && Type >= D3DSAMPLERSTATETYPE( 0 ) && Type < D3DSSS_MAX );
        return m_bitsetSamplerState.test( Sampler * D3DSSS_MAX + Type );
    }//IsSamplerStateChanged()

private:

    ~CKTDGEffectStateManager();



    enum    EStateType
    {
        STATE_TYPE_INVALID = -1,
        STATE_TYPE_RENDER = 0,
        STATE_TYPE_TEXTURE_STAGE = 1,
        STATE_TYPE_SAMPLER = 2,
    };//enum    EStackType


    __forceinline   CKTDGStateManager::KState&   _GetStackTop() 
    { 
        ASSERT( m_iStackPointer < DEVICE_STATE_STACK_SIZE );
        return m_kStack[ m_iStackPointer ];
    }//

    __forceinline int   _PushStackTop( const CKTDGStateManager::KState& kInState_ )
    {
        int iOldStackPointer = m_iStackPointer;
        ASSERT( iOldStackPointer < DEVICE_STATE_STACK_SIZE );
        m_kStack[ m_iStackPointer++ ] = kInState_;
        return iOldStackPointer;
    }//

private:

    bool                                        m_bd3dVertexShader;
    LPDIRECT3DVERTEXSHADER9                     m_pd3dVertexShader;
    bool                                        m_bd3dPixelShader;
    LPDIRECT3DPIXELSHADER9                      m_pd3dPixelShader;
    bool                                        m_bFVF;
    LPDIRECT3DVERTEXDECLARATION9                m_pd3dVertexDeclaration;
    bool                                        m_bd3dMaterial;
    D3DMATERIAL9                                m_d3dMaterial;
//
//#define MAX_NUM_STAGE_TEXTURES                  max( TEXTURE_STAGE_NUM, SAMPLER_STAGE_NUM )
//#define MAX_NUM_TEXTURES                        (MAX_NUM_STAGE_TEXTURES + 5)
//
//    LPDIRECT3DBASETEXTURE9                      m_apd3dTextures[ MAX_NUM_TEXTURES ];
//    std::bitset<MAX_NUM_TEXTURES>               m_bitsetTextures;

    std::bitset<D3DRS_MAX>                      m_bitsetRenderState;
    std::bitset<TEXTURE_STAGE_NUM * D3DTSS_MAX> m_bitsetTextureStageState;
    std::bitset<SAMPLER_STAGE_NUM * D3DSSS_MAX> m_bitsetSamplerState;
    CKTDGStateManager::KState                                      m_kStack[DEVICE_STATE_STACK_SIZE];
    int                                         m_iStackPointer;
    bool                                        m_bInEffect;

    int                                         m_iStateManagerStackTop;

    ULONG   m_nRefCount;

#ifdef X2OPTIMIZE_SETSHADERCONSTANT
public:
	__forceinline void PostCommitChanges();
	__forceinline void OnLostDevice();

private:
	//VS - float constants
	float* m_pfFloatVSConstantData;
	UINT m_uiFirstDirtyFloatVSReg;
	UINT m_uiFirstCleanFloatVSReg;
	UINT m_uiNumFloatVSConstants;
#endif//X2OPTIMIZE_SETSHADERCONSTANT
};//class   CKTDGEffectStateManager

#ifdef X2OPTIMIZE_SETSHADERCONSTANT
extern LPDIRECT3DDEVICE9        g_pd3dDevice;

__forceinline HRESULT STDMETHODCALLTYPE CKTDGEffectStateManager::SetVertexShaderConstantF(THIS_ UINT RegisterIndex, CONST FLOAT *pConstantData, UINT RegisterCount)
{
	unsigned int uiBase = RegisterIndex * 4;

	if ( RegisterIndex >= m_uiFirstDirtyFloatVSReg
		&& RegisterIndex + RegisterCount <= m_uiFirstCleanFloatVSReg )
	{
		unsigned int uiByteSize = RegisterCount * 4 * sizeof(*pConstantData);
		memcpy(m_pfFloatVSConstantData + uiBase, pConstantData, uiByteSize);
	}
	else
	{
        if ( RegisterIndex >= m_uiNumFloatVSConstants )
            return E_FAIL;
        if ( RegisterIndex + RegisterCount > m_uiNumFloatVSConstants )
            RegisterCount = m_uiNumFloatVSConstants - RegisterIndex;

		unsigned	uiDirty = RegisterCount;
		unsigned	uiClean = 0;
		for( unsigned u = 0; u != RegisterCount; u++ )
		{
			if ( memcmp( pConstantData + 4 * u, m_pfFloatVSConstantData + uiBase + 4 * u, sizeof(*pConstantData)*4 ) != 0 )
			{
				if ( u < uiDirty )
					uiDirty = u;
				uiClean = u + 1;
			}//if
		}//for
		if ( uiDirty < uiClean )
		{
			RegisterIndex += uiDirty;
			RegisterCount = uiClean - uiDirty;
			memcpy( m_pfFloatVSConstantData + RegisterIndex * 4, pConstantData + uiDirty * 4
				, RegisterCount * 4 * sizeof(*pConstantData) );

			if (m_uiFirstDirtyFloatVSReg > RegisterIndex)
				m_uiFirstDirtyFloatVSReg = RegisterIndex;
			if (m_uiFirstCleanFloatVSReg < RegisterIndex + RegisterCount)
				m_uiFirstCleanFloatVSReg = RegisterIndex + RegisterCount;
		}//if
	}
	return S_OK;
}//CKTDGEffectStateManager::SetVertexShaderConstantF()

__forceinline void CKTDGEffectStateManager::PostCommitChanges()
{
	ASSERT( m_bInEffect );

	if (m_uiFirstDirtyFloatVSReg < m_uiFirstCleanFloatVSReg)
	{
		HRESULT hr = g_pd3dDevice->SetVertexShaderConstantF(
			m_uiFirstDirtyFloatVSReg, 
			m_pfFloatVSConstantData + m_uiFirstDirtyFloatVSReg * 4,
			m_uiFirstCleanFloatVSReg - m_uiFirstDirtyFloatVSReg);
		ASSERT(SUCCEEDED(hr));
	}

	m_uiFirstDirtyFloatVSReg = m_uiNumFloatVSConstants;
	m_uiFirstCleanFloatVSReg = 0;
}

__forceinline void CKTDGEffectStateManager::OnLostDevice()
{
	m_uiFirstDirtyFloatVSReg = 0;
	m_uiFirstCleanFloatVSReg = m_uiNumFloatVSConstants;
}
#endif//X2OPTIMIZE_SETSHADERCONSTANT