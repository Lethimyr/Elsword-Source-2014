#pragma once

class CKTDGGrayScale
{
public:
	struct VERTEX_GS
	{
		D3DXVECTOR4 m_vPos; // x, y, z, 1/w
		D3DCOLOR    m_diff; // diffuse color
		D3DXVECTOR2 m_vTex; // tu, tv

		enum _FVF
		{
			FVF = D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1,
		};

		VERTEX_GS( void )
		{
			m_vPos.x = 0.0f;
			m_vPos.y = 0.0f;
			m_vPos.x = 0.0f;
			m_vPos.w = 0.0f;
			m_diff   = D3DCOLOR_ARGB( 0xff, 0xff, 0xff, 0xff );
			m_vTex.x = 0.0f;
			m_vTex.y = 0.0f;
		}

		void SetVertex( float x, float y, float z, float fTu, float fTv )
		{
			m_vPos.x = x;
			m_vPos.y = y;
			m_vPos.z = z;
			m_vPos.w = 1.0f;
			m_diff   = 0xffffffff;
			m_vTex.x = fTu;
			m_vTex.y = fTv;
		}
	};

	class TargetTexture
	{
	public:
		TargetTexture(void);
		~TargetTexture(void);

		void	OnResetDevice();
		void	OnLostDevice();

		bool	CreateTarget( DWORD dwWidth, DWORD dwHeight );
		void	BeginRenderTarget();
		void	EndRenderTarget();

		LPDIRECT3DTEXTURE9	GetDeviceTexture(){ return m_pTexture; }

	private:
		LPDIRECT3DDEVICE9	m_pd3dDevice;
		LPDIRECT3DTEXTURE9	m_pTexture;
		LPDIRECT3DSURFACE9	m_pSurface, m_pOldSurface;

		DWORD				m_dwWidth, m_dwHeight;
	};

	class DeviceShader
	{
	public:
		DeviceShader( wstring wstrFileName );
		~DeviceShader(void);

		HRESULT	OnResetDevice();
		HRESULT	OnLostDevice();

		ID3DXEffect* GetEffect() { return m_pEffect; }

		bool SetTechnique( char* strTechnique );
		void BeginPass( UINT iIndex );
		void EndPass();

	private:
		ID3DXEffect*    m_pEffect;	
		bool			m_bBegin;
		wstring			m_wstrFileName;
		string			m_strTechnique;
	};

public:
	CKTDGGrayScale(void);
	~CKTDGGrayScale(void);

	HRESULT	OnFrameMove( float fElapsedTime );
	HRESULT	OnResetDevice();
	HRESULT	OnLostDevice();

	void	SetGray( float fRate, float fTime );	// max rate : 1.0f
	void	SetGrayInverse( bool bEnable );

	void	BeginRender();
	void	EndRender();
	void	Render();

private:
	LPDIRECT3DDEVICE9	m_pd3dDevice;
	TargetTexture*		m_pTexture;
	VERTEX_GS			m_Vertex[4];
	DeviceShader*		m_pDeviceShader;

	float	m_fCurrRate;
	float	m_fSetRate;
	float	m_fCurrTime;
	float	m_fSetTime;

	bool	m_bGray;
	bool	m_bGrayInverse;
};
