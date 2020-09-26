#pragma once

#define D3DFVF_WORLDLAYER (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

class CX2WorldLayer : public CX2WorldObject
{
	public:
		
		typedef struct 
		{
			D3DXVECTOR3 position; 
			float       rhw;
			D3DCOLOR    color;    
			float       tu, tv;
		} VERTEX_WORLDLAYER;		

	private:
		//CX2WorldLayer() { ASSERT( !"invalid" ); }
		CX2WorldLayer( HWND hWnd );
		~CX2WorldLayer(void);

	public:
		static CX2WorldLayer* CreateWorldLayer( HWND hWnd ) 
		{ 
			return new CX2WorldLayer( hWnd ); 
		}
		//static void DeleteKTDGObject( CX2WorldLayer* pObject ) 
		//{ 	
		//	if( NULL != pObject )
		//	{
		//		CKTDGObject::DeleteKTDGObject( pObject ); 
		//	} 
		//}


        
//{{ robobeg : 2008-10-28
		//virtual HRESULT OnFrameRender();
        virtual void    OnFrameRender_Draw();
//}} robobeg : 2008-10-28
        virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
		virtual HRESULT OnResetDevice();
		virtual HRESULT OnLostDevice();

        void InitTexUV()                                { m_minTexUV = D3DXVECTOR2(0.01f, 0.01f); m_maxTexUV = D3DXVECTOR2(0.99f, 0.99f); }


		void FadeColor( D3DXCOLOR color, float fFadeTime )
		{
			if( fFadeTime > 0.f )
			{
				m_FadeColor = color;
				m_fFadeTime = fFadeTime;
				m_color = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
			}
			
		}
		
#ifdef X2TOOL
		void SetTexW( wstring wstrTexname );
#endif
        void SetTex( const char *texName );
        void SetBlendType(int blendType)                { m_BlendType = blendType; }
        void SetColor( D3DXCOLOR color )                { m_color = color; }           		
        void SetTexUV(float u, float v)                 { m_SpeedUV = D3DXVECTOR2(u, v); }
        void SetFeedBackUV(float u, float v)            { m_feedbakUV = D3DXVECTOR2(u, v); }
        void SetPos(float x, float y, float wx, float wy);        
        void SetLayerId(int iVal)						{ m_iLayerId = iVal; }
		void SetInitShow(bool bVal)						
		{ 
			m_bInitShow = bVal; 
			if( m_bInitShow == true )
				SetShowObject( true );
			else
				SetShowObject( false );
		}

		//const char* GetTex()                            { return m_TexName; }
		const wstring& GetTexName()						{ return m_wstrTexName; }
        D3DXCOLOR GetColor()                            { return m_color; }
        int GetBlendType()                              { return m_BlendType; }
        void GetPos(float *x, float *y, float *wx, float *wy)   { *x = m_PosX; *y = m_PosY; *wx = m_fWidth; *wy = m_fHeight; }
        D3DXVECTOR2 GetTexUV()                          { return m_SpeedUV; }
        D3DXVECTOR2 GetFeedBackUV()                     { return m_feedbakUV; }
		int GetLayerId()								{ return m_iLayerId; }
		bool GetInitShow()								{ return m_bInitShow; }

	private:
		HWND						m_hWnd;

        int                         m_BlendType;
        D3DXCOLOR                   m_color;
        float                       m_PosX;
        float                       m_PosY;
        float                       m_fWidth;
        float                       m_fHeight;
        D3DXVECTOR2                 m_SpeedUV;

        D3DXVECTOR2                 m_feedbakUV;

        float                       m_x, m_y, m_w, m_h;


        //char*                       m_TexName;
		wstring						m_wstrTexName;
		CKTDXDeviceTexture*			m_pLayerTex1;		

        D3DXVECTOR2                 m_minTexUV, m_maxTexUV;

        D3DXVECTOR3                 m_prePos;

		D3DXCOLOR                   m_FadeColor;
		float						m_fFadeTime;

		int							m_iLayerId;
		bool						m_bInitShow;

#ifdef X2TOOL
	public:
		bool						m_bSelected;
#endif
};
