#pragma once

#define D3DFVF_RENDERTARGET_RHW_VERTEX (D3DFVF_XYZRHW | D3DFVF_TEX1 | D3DFVF_DIFFUSE)
#define D3DFVF_RENDERTARGET_VERTEX (D3DFVF_XYZ | D3DFVF_TEX1)

class CKTDXDeviceRenderTargetTexture 
	: public CKTDXDeviceBaseTexture
{
	private:
		struct RENDERTARGET_RHW_VERTEX
		{
			float x, y, z, rhw;
			DWORD color;
			float u, v;
		};

		struct RENDERTARGET_VERTEX
		{
			D3DXVECTOR3	vPos;
			//DWORD		color;
			D3DXVECTOR2 vTex;
		};

	public:
		CKTDXDeviceRenderTargetTexture( LPDIRECT3DDEVICE9 pd3dDevice, wstring deviceID, int width, int height, D3DFORMAT fmt = D3DFMT_UNKNOWN );


		virtual HRESULT OnResetDevice();
		virtual HRESULT OnLostDevice();

		void Draw( int nX, int nY, int nWidth, int nHeight, D3DCOLOR color = 0xffffffff );
		void RenderQuadRHW( D3DCOLOR color = 0xffffffff, float fX = 0.f, float fY = 0.f, float fWidth = -1.f, float fHeight = -1.f );
		void RenderQuad( D3DCOLOR color = 0xffffffff );

		void BeginRender( bool bClear = true );
		void EndRender();

		void Clear( D3DCOLOR color = 0x00000000 );

		void SetDeviceTexture( int stage = 0 );
		HRESULT CopySurface( LPDIRECT3DSURFACE9 pDestSurface );
		HRESULT CopySurfaceToBackBuffer();
		HRESULT CopyBackBufferToSurface();

		LPDIRECT3DSURFACE9 GetSurface(){ return m_pRenderTargetSurface; }
		LPDIRECT3DTEXTURE9 GetTexture(){ return m_pRenderTargetTexture; }
		int GetWidth(){ return m_Width; }
		int GetHeight(){ return m_Height; }


		bool IsOk()
		{
			if( m_pRenderTargetTexture == NULL || m_pRenderTargetSurface == NULL )
			{
				return false;
			}
			else
			{
				return true;
			}
		}

    protected:

		virtual ~CKTDXDeviceRenderTargetTexture(void);

	private:



		int						m_Width;
		int						m_Height;

		LPDIRECT3DTEXTURE9		m_pRenderTargetTexture;

		LPDIRECT3DSURFACE9		m_pRenderTargetSurface;
		LPDIRECT3DSURFACE9		m_pRenderTargetZSurface;
		
		LPDIRECT3DSURFACE9		m_pBackupBuffer;
		LPDIRECT3DSURFACE9		m_pBackupZBuffer;
		
		LPDIRECT3DVERTEXBUFFER9	m_pDisplayVertexBuffer;
		D3DFORMAT				m_D3DFormat;
		
//#ifndef DYNAMIC_VERTEX_BUFFER_OPT
//		LPDIRECT3DVERTEXBUFFER9	m_pVB_RHW_VERTEX;
//		LPDIRECT3DVERTEXBUFFER9	m_pVB_RENDERTARGET_RHW_VERTEX;
//		LPDIRECT3DVERTEXBUFFER9	m_pVB_RENDERTARGET_VERTEX;
//#endif
};
