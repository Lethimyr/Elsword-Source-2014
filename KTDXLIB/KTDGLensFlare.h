#pragma once

#define D3DFVF_LENSFLARE (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

class CKTDGLensFlare : public CKTDGObject
{
	public:
		// Our custom lens flare FVF
		typedef struct 
		{
			D3DXVECTOR3 position; // The position
			float       rhw;
			D3DCOLOR    color;    // The color
			float       tu, tv;
		}VERTEX_LENSFLARE;


		class CLensFlareSpot
		{
			public:
				CLensFlareSpot();
				CLensFlareSpot(CKTDXDeviceTexture* pTex, float fSize, float fLinePos, D3DXCOLOR Color);

				virtual ~CLensFlareSpot();

				void SetTexture(CKTDXDeviceTexture* pTexture)	{ m_pTexture = pTexture;	}
				CKTDXDeviceTexture* GetTexture(void)			{ return(m_pTexture);		}

				float GetSize(void) const						{ return(m_fSize);			}
				void SetSize(const float data)					{ m_fSize = data;			}

				float GetLinePos(void) const					{ return(m_fLinePos);		}
				void SetLinePos(const float data)				{ m_fLinePos = data;		}

				D3DXCOLOR GetColor(void) const					{ return(m_Color);			}
				void SetColor(const D3DXCOLOR &data)			{ m_Color = data;			}

			protected:
				CKTDXDeviceTexture*	m_pTexture;
				float				m_fSize;
				float				m_fLinePos;
				D3DXCOLOR			m_Color;
		};

	private:
		//CKTDGLensFlare() : CKTDGObject( NULL ) { ASSERT( !"invalid" ); }
		CKTDGLensFlare( HWND hWnd,
						WCHAR* pHaloTex,
						WCHAR* pFlareTex1,
						WCHAR* pFlareTex2,
						WCHAR* pFlareTex3 );
		~CKTDGLensFlare(void);

	public:
		static CKTDGLensFlare* CreateLensFlare( HWND hWnd, 
			WCHAR* pHaloTex,
			WCHAR* pFlareTex1,
			WCHAR* pFlareTex2,
			WCHAR* pFlareTex3 )
		{
			return new CKTDGLensFlare( hWnd, pHaloTex, pFlareTex1, pFlareTex2, pFlareTex3 );
		}


		//static void DeleteKTDGObject( CKTDGLensFlare* pObject )
		//{ 	
		//	if( NULL != pObject )
		//	{
		//		CKTDGObject::DeleteKTDGObject( pObject ); 
		//	} 
		//}





		

//{{ robobeg : 2008-10-28
		//virtual HRESULT OnFrameRender();
        virtual RENDER_HINT OnFrameRender_Prepare();
        virtual void            OnFrameRender_Draw();
//}} robobeg : 2008-10-28

//#ifndef DYNAMIC_VERTEX_BUFFER_OPT
//		virtual HRESULT OnResetDevice();
//		virtual HRESULT OnLostDevice();
//#endif

		void	AddSpot( CLensFlareSpot &spot );

		void	SetIntensity(const CMinMax<float> &data)	{ m_fIntensity = data; }
		float	GetIntensityBorder(void) const				{ return(m_fIntensityBorder); }
		void	SetIntensityBorder(const float data)		{ m_fIntensityBorder = data; }
		void	SetLightPosition( D3DXVECTOR3 pos )			{ m_LightPosition = pos; }


	private:
//#ifndef DYNAMIC_VERTEX_BUFFER_OPT
//		HRESULT	RecreateVB();
//#endif
		void	CalcLightSourceScreenCoords();


		HWND						m_hWnd;

		CMinMax<float>				m_fIntensity;
		float						m_fIntensityBorder;
		D3DXVECTOR3					m_LightPosition;
		D3DXVECTOR3					m_vScreen;

//#ifndef DYNAMIC_VERTEX_BUFFER_OPT
//		LPDIRECT3DVERTEXBUFFER9		m_pVBSpots;
//		int							m_iVBSize;
//#endif

		CKTDXDeviceTexture*			m_pTexHalo;
		CKTDXDeviceTexture*			m_pTex1;
		CKTDXDeviceTexture*			m_pTex2;
		CKTDXDeviceTexture*			m_pTex3;

		std::vector<CLensFlareSpot>	m_Spots;
//#ifdef DYNAMIC_VERTEX_BUFFER_OPT
		std::vector<VERTEX_LENSFLARE> m_vertices;
//#endif
};
