#include "StdAfx.h"
#include ".\ktdgslashtrace.h"




// fix!!! 현재는 그냥 slash trace랑 textured slash trace랑 같이 렌더링 되게 되어있는데, 한쪽으로만 렌더링 되게 수정하자.
// fix!!! 하지만 renderstate는 일단 생성될 때 선택되는 것에서 변경하지 않는 것으로 구현되어있어서, 차후에 2가지 slash trace를 동시에 사용한다면 수정이 필요하다.

//
//#pragma NOTE( "임시로 텍스쳐입힌 칼궤적 render param 스크립트에서 읽어올 수 있게" )
//static bool bAlphaBlend = true;
//static DWORD dwSlashTraceSrcBlend = D3DBLEND_SRCALPHA;
//static DWORD dwSlashTraceDestBlend = D3DBLEND_DESTALPHA;
//static float fSlashTraceAlpha = 0.5f;
//static int s_DrawCount = 1;



CKTDGSlashTrace::CKTDGSlashTrace( int vertexNum, bool bTexturedSlashTrace /*= false*/, const float fTextureStride /*= 0.f*/, const wstring& slashTraceTextureName /*= L""*/ )
: m_vOldCenter( 0, 0, 0 )
, m_fOldCircleRadius( 0.f ) 
, m_TexturedSlashTraceOpaqueAlphaThreshold( 1.f ) 
, m_TexturedSlashTraceInvisibleAlphaThreshold( 0.1f )
, m_eCurveType( CT_SMOOTH_CURVE )
, m_SplineBufUp1(0,0,0)
, m_SplineBufUp2(0,0,0)
, m_SplineBufUp3(0,0,0)
, m_SplineBufUp4(0,0,0)
, m_SplineBufDown1(0,0,0)
, m_SplineBufDown2(0,0,0)
, m_SplineBufDown3(0,0,0)
, m_SplineBufDown4(0,0,0)
, m_DisableTime( 0.f )
{
#ifdef  X2OPTIMIZE_DYNAMICVB_SUPPORT_SPLIT_DATA
    if ( vertexNum < 2 )
        vertexNum = 2;
    if ( ( vertexNum & 1 ) != 0 )
        vertexNum++;
#endif  X2OPTIMIZE_DYNAMICVB_SUPPORT_SPLIT_DATA

    SetAlphaObject( true );
	m_bHasVisibleVertex	= false;
	
	m_VertexNum			= vertexNum;
	m_pSlashVertexList	= new VERTEX_SLASH_TRACE[m_VertexNum];
	ZeroMemory( m_pSlashVertexList, sizeof(VERTEX_SLASH_TRACE) * m_VertexNum );
#ifdef  X2OPTIMIZE_DYNAMICVB_SUPPORT_SPLIT_DATA
    m_iSlashVertexList_StartIndex = 0;
#endif  X2OPTIMIZE_DYNAMICVB_SUPPORT_SPLIT_DATA

#ifdef TEXTURED_SLASH_TRACE_TEST
	m_bHasVisibleVertexTextured = false;
	m_pCurrSlashTraceTexture = NULL;
	
	if( 0.f == fTextureStride )
	{
		const float MAGIC_TEXTURE_STRIDE = 100.f;
		m_fTextureStride = MAGIC_TEXTURE_STRIDE;
	}
	else
	{
		m_fTextureStride = fTextureStride;
	}

	m_pSlashVertexTexturedList = NULL;
#ifdef  X2OPTIMIZE_DYNAMICVB_SUPPORT_SPLIT_DATA
    m_iSlashVertexTexturedList_StartIndex = 0;
#endif  X2OPTIMIZE_DYNAMICVB_SUPPORT_SPLIT_DATA
	m_DefaultSlashTraceTextureName = slashTraceTextureName;
	m_bEnabledSlashTraceTexture = bTexturedSlashTrace;
	if( true == m_bEnabledSlashTraceTexture )
	{
		m_pSlashVertexTexturedList = new VERTEX_SLASH_TRACE_TEXTURED[ m_VertexNum ];
		ZeroMemory( m_pSlashVertexTexturedList, sizeof(VERTEX_SLASH_TRACE_TEXTURED) * m_VertexNum );
	}
#endif TEXTURED_SLASH_TRACE_TEST


	m_bHighLight		= false;
	m_DrawCount			= 1;



//#ifndef DYNAMIC_VERTEX_BUFFER_OPT
//	HRESULT hr;
//
//	const D3DCAPS9* pD3DCAPS9 = DXUTGetDeviceCaps();
//	
//	if ( pD3DCAPS9 != NULL && (  pD3DCAPS9->Caps2 & D3DCAPS2_DYNAMICTEXTURES ) != 0 )
//	{
//		m_bUseDynamic = true;
//
//		if(FAILED(hr = g_pKTDXApp->GetDevice()->CreateVertexBuffer( m_VertexNum * sizeof(VERTEX_SLASH_TRACE), 
//			D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, D3DFVF_SLASH_TRACE, 
//			D3DPOOL_DEFAULT, &m_pSlashVB, NULL )))
//		{
//			wcout << L"Fail: CKTDGSlashTrace::CreateVertexBuffer" << std::endl;
//			ErrorLog( KEM_ERROR34 );
//			return;
//		}
//	}
//	else
//	{
//		m_bUseDynamic = false;
//
//		if(FAILED(hr = g_pKTDXApp->GetDevice()->CreateVertexBuffer( m_VertexNum * sizeof(VERTEX_SLASH_TRACE), 
//			D3DUSAGE_WRITEONLY, D3DFVF_SLASH_TRACE, 
//			D3DPOOL_MANAGED, &m_pSlashVB, NULL )))
//		{
//			wcout << L"Fail: CKTDGSlashTrace::CreateVertexBuffer" << std::endl;
//			ErrorLog( KEM_ERROR34 );
//			return;
//		}
//	}
//
//
//
//
//
//#ifdef TEXTURED_SLASH_TRACE_TEST
//	m_pSlashTexturedVB = NULL;
//	if( true == m_bEnabledSlashTraceTexture )
//	{
//		if( FAILED(hr = g_pKTDXApp->GetDevice()->CreateVertexBuffer( m_VertexNum * sizeof(VERTEX_SLASH_TRACE_TEXTURED), 
//			D3DUSAGE_WRITEONLY, D3DFVF_SLASH_TRACE_TEXTURED, 
//			D3DPOOL_MANAGED, &m_pSlashTexturedVB, NULL )))
//		{
//			wcout << L"Fail: CKTDGSlashTrace::CreateVertexBuffer textured" << std::endl;
//			ErrorLog( KEM_ERROR34 );
//			return;
//		}
//	}
//#endif TEXTURED_SLASH_TRACE_TEST
//
//#endif


#ifdef TEXTURED_SLASH_TRACE_TEST
	if( true == m_bEnabledSlashTraceTexture )
	{
		SetRenderStateID( g_pKTDXApp->GetDGManager()->GetRSICKTDGTexturedSlashTrace() );


//#pragma NOTE( "임시로 텍스쳐입힌 칼궤적 render param 스크립트에서 읽어올 수 있게" )
//		//{{AFX
//		FILE *fp = fopen("textured_slash_trace.txt", "r");	
//		if(fp != NULL)
//		{
//			int iAlphaBlend = 0;
//			int iSlashTraceSrcBlend = 0;
//			int iSlashTraceDestBlend = 0;
//
//			char dummyString[512] = "";
//
//			fscanf(fp, "%s%d", &dummyString, &iAlphaBlend );
//			fscanf(fp, "%s%d", &dummyString, &iSlashTraceSrcBlend );
//			fscanf(fp, "%s%d", &dummyString, &iSlashTraceDestBlend );
//			fscanf(fp, "%s%d", &dummyString, &s_DrawCount );
//			fscanf(fp, "%s%f", &dummyString, &m_DisableTime );
//			fscanf(fp, "%s%f", &dummyString, &m_TexturedSlashTraceOpaqueAlphaThreshold );
//			fscanf(fp, "%s%f", &dummyString, &m_TexturedSlashTraceInvisibleAlphaThreshold );
//			fscanf(fp, "%s%f", &dummyString, &fSlashTraceAlpha );
//
//
//			if( iAlphaBlend > 0 )
//			{
//				bAlphaBlend = true;
//			}
//			else
//			{
//				bAlphaBlend = false;
//			}
//			dwSlashTraceSrcBlend = (DWORD) iSlashTraceSrcBlend;
//			dwSlashTraceDestBlend = (DWORD) iSlashTraceDestBlend;
//
//			fclose(fp);
//		}
//		//}}AFX


	}
	else
	{
		SetRenderStateID( g_pKTDXApp->GetDGManager()->GetRSICKTDGSlashTrace() );
	}
#else TEXTURED_SLASH_TRACE_TEST
	//{{AFX
	SetRenderStateID( g_pKTDXApp->GetDGManager()->GetRSICKTDGSlashTrace() );
	//}}AFX
#endif TEXTURED_SLASH_TRACE_TEST
	

}





CKTDGSlashTrace::~CKTDGSlashTrace(void)
{
//#ifndef DYNAMIC_VERTEX_BUFFER_OPT
//	SAFE_RELEASE( m_pSlashVB );
//#endif
	SAFE_DELETE_ARRAY( m_pSlashVertexList );

#ifdef TEXTURED_SLASH_TRACE_TEST
//#ifndef DYNAMIC_VERTEX_BUFFER_OPT
//	SAFE_RELEASE( m_pSlashTexturedVB );
//#endif
	SAFE_DELETE_ARRAY( m_pSlashVertexTexturedList );

	std::map<wstring, CKTDXDeviceTexture*>::iterator it;
	for( it = m_mapSlashTraceTexture.begin(); it != m_mapSlashTraceTexture.end(); it++ )
	{
		SAFE_CLOSE( it->second );
	}
#endif TEXTURED_SLASH_TRACE_TEST
}




HRESULT CKTDGSlashTrace::OnFrameMove( float fElapsedTime )
{
	KTDXPROFILE();


	if( true == m_bHasVisibleVertex )
	{
		D3DXCOLOR tempColor;
		m_bHasVisibleVertex = false;

		for( int i=0; i<m_VertexNum; i++ )
		{
			tempColor = m_pSlashVertexList[i].color;
			if( tempColor.a > 0.f  )
			{
				tempColor.a -= (fElapsedTime / m_DisableTime);
				if( tempColor.a < 0.f )
				{
					tempColor.a = 0.f;
				}
				else
				{
					m_bHasVisibleVertex = true;
				}
				
				m_pSlashVertexList[i].color = tempColor;
			}		
		}
	}



#ifdef TEXTURED_SLASH_TRACE_TEST
	if( true == m_bEnabledSlashTraceTexture )
	{
		const int MAGIC_INITIAL_VALUE = m_VertexNum-1;
		int iFirstNonZeroAlphaVertexIndex = MAGIC_INITIAL_VALUE;
		int iFirstZeroAlphaVertexIndex = MAGIC_INITIAL_VALUE;
		if( true == m_bHasVisibleVertexTextured )
		{
			m_bHasVisibleVertexTextured = false;
			D3DXCOLOR tempColor;

#ifdef  X2OPTIMIZE_DYNAMICVB_SUPPORT_SPLIT_DATA
            int j = m_iSlashVertexTexturedList_StartIndex % m_VertexNum;
#else   X2OPTIMIZE_DYNAMICVB_SUPPORT_SPLIT_DATA
            int j = 0;
#endif  X2OPTIMIZE_DYNAMICVB_SUPPORT_SPLIT_DATA
			for( int i=0; i<m_VertexNum; i++ )
			{
				tempColor = m_pSlashVertexTexturedList[j].color;
				if( tempColor.a > 0.f  )
				{
					tempColor.a -= (fElapsedTime / m_DisableTime);

					if( tempColor.a > m_TexturedSlashTraceOpaqueAlphaThreshold )
					{
						tempColor.a = 1.f;
					}
					else if( tempColor.a < m_TexturedSlashTraceInvisibleAlphaThreshold )
					{
						tempColor.a = 0.f;
					}

					if( tempColor.a > 0.f )
					{
						m_bHasVisibleVertexTextured = true;
						if( MAGIC_INITIAL_VALUE == iFirstNonZeroAlphaVertexIndex )
						{
							iFirstNonZeroAlphaVertexIndex = i;
						}
					}
				
					m_pSlashVertexTexturedList[j].color = tempColor;
                    j++;
#ifdef  X2OPTIMIZE_DYNAMICVB_SUPPORT_SPLIT_DATA
                    if ( j >= m_VertexNum )
                        j = 0;
#endif  X2OPTIMIZE_DYNAMICVB_SUPPORT_SPLIT_DATA
				}


				if( tempColor.a <= 0.f )
				{
					if( MAGIC_INITIAL_VALUE == iFirstZeroAlphaVertexIndex && 
						MAGIC_INITIAL_VALUE != iFirstNonZeroAlphaVertexIndex )
					{
						iFirstZeroAlphaVertexIndex = i;
					}
				}

			}
		}


		

		// 텍스쳐 맵핑 UV 좌표를 계산한다
		if( true == m_bHasVisibleVertexTextured )
		{
			if( iFirstNonZeroAlphaVertexIndex % 2 == 1 )
			{
				iFirstNonZeroAlphaVertexIndex -= 1;
			}
			
			if( iFirstNonZeroAlphaVertexIndex == 0 &&
				iFirstZeroAlphaVertexIndex > 0 ) // 지금 만들어져서 보이게되는 slash trace의 texture 좌표만 갱신해준다.
			{
				const float fTextureStride = 0.5f * 1.f / (float) ( (iFirstZeroAlphaVertexIndex-iFirstNonZeroAlphaVertexIndex) * 0.5f );
				for( int i=iFirstNonZeroAlphaVertexIndex; i<iFirstZeroAlphaVertexIndex && i+1<m_VertexNum; i+=2 )
				{
#ifdef  X2OPTIMIZE_DYNAMICVB_SUPPORT_SPLIT_DATA
                    int j = ( i + m_iSlashVertexTexturedList_StartIndex ) % m_VertexNum;
#else   X2OPTIMIZE_DYNAMICVB_SUPPORT_SPLIT_DATA
                    int j = i;
#endif  X2OPTIMIZE_DYNAMICVB_SUPPORT_SPLIT_DATA
					VERTEX_SLASH_TRACE_TEXTURED& vertexUp	= m_pSlashVertexTexturedList[j];
					VERTEX_SLASH_TRACE_TEXTURED& vertexDown	= m_pSlashVertexTexturedList[j+1];

					vertexUp.texUV.y = 0.f;
					vertexUp.texUV.x = (float) i * fTextureStride;

					vertexDown.texUV.y = 1.f;
					vertexDown.texUV.x = (float) i * fTextureStride;
				}
			}
		}
	}



#endif TEXTURED_SLASH_TRACE_TEST



	return S_OK;
}


//{{ robobeg : 2008-10-24
//virtual HRESULT OnFrameRender();
/*virtual*/
RENDER_HINT   CKTDGSlashTrace::OnFrameRender_Prepare()
{
	KTDXPROFILE();


#ifdef TEXTURED_SLASH_TRACE_TEST
	if( false == m_bHasVisibleVertex && false == m_bHasVisibleVertexTextured )
		return RENDER_HINT_NORENDER;
#else TEXTURED_SLASH_TRACE_TEST
	//{{AFX
	if( false == m_bHasVisibleVertex )
		return RENDER_HINT_NORENDER; 
	//}}AFX
#endif TEXTURED_SLASH_TRACE_TEST



//#ifndef DYNAMIC_VERTEX_BUFFER_OPT
//	if( true == m_bHasVisibleVertex )
//	{
//		VERTEX_SLASH_TRACE* pVertices;
//		if ( m_bUseDynamic == true )
//		{
//			if(FAILED( m_pSlashVB->Lock(0, 0, (void**) &pVertices, D3DLOCK_DISCARD) ))
//			{
//				wcout << L"Fail: CKTDGSlashTrace::Lock" << std::endl;
//				return RENDER_HINT_NORENDER;
//			}
//		}
//		else
//		{
//			if(FAILED( m_pSlashVB->Lock(0, 0, (void**) &pVertices, 0) ))
//			{
//				wcout << L"Fail: CKTDGSlashTrace::Lock" << std::endl;
//				return RENDER_HINT_NORENDER;
//			}
//		}
//
//		memcpy( pVertices, m_pSlashVertexList, sizeof(VERTEX_SLASH_TRACE) * m_VertexNum );
//		m_pSlashVB->Unlock();
//	}
//
//
//
//#ifdef TEXTURED_SLASH_TRACE_TEST
//	if( true == m_bEnabledSlashTraceTexture )
//	{
//		if( true == m_bHasVisibleVertexTextured )
//		{
//			VERTEX_SLASH_TRACE_TEXTURED* pVerticesTextured = NULL;
//			if( FAILED( m_pSlashTexturedVB->Lock(0, 0, (void**) &pVerticesTextured, 0) ) )
//			{
//				wcout << L"Fail: CKTDGSlashTrace::Lock textured" << std::endl;
//				return RENDER_HINT_NORENDER;
//			}
//
//			memcpy( pVerticesTextured, m_pSlashVertexTexturedList, sizeof(VERTEX_SLASH_TRACE_TEXTURED) * m_VertexNum );
//			m_pSlashTexturedVB->Unlock();
//		}
//	}
//#endif TEXTURED_SLASH_TRACE_TEST
//
//#endif

    return RENDER_HINT_DEFAULT;
}//CKTDGSlashTrace::OnFrameRender_Prepare()


/*virtual*/
void    CKTDGSlashTrace::OnFrameRender_Draw()
{
	//CKTDGMatrixSet  oldMatrixSet = GetMatrix().GetMatrixSet();
	//GetMatrix().Move( 0.0f, 0.0f, 0.0f );
	//GetMatrix().Scale( 1.0f, 1.0f, 1.0f );
	//GetMatrix().Rotate( 0.0f, 0.0f, 0.0f );
	//GetMatrix().UpdateWorldMatrix();
    g_pKTDXApp->ResetWorldTransform();



//
//#pragma NOTE( "임시로 텍스쳐입힌 칼궤적 render param 스크립트에서 읽어올 수 있게" )
////{{AFX
//	CKTDGStateManager::PushRenderState( D3DRS_ALPHABLENDENABLE,		bAlphaBlend );
//	CKTDGStateManager::PushRenderState( D3DRS_SRCBLEND,				dwSlashTraceSrcBlend );
//	CKTDGStateManager::PushRenderState( D3DRS_DESTBLEND,			dwSlashTraceDestBlend );
////}}AFX





#ifdef TEXTURED_SLASH_TRACE_TEST
	if( true == m_bEnabledSlashTraceTexture )
	{
		if( true == m_bHasVisibleVertexTextured && m_VertexNum > 2 )
		{
			if( NULL != m_pCurrSlashTraceTexture )
			{
				m_pCurrSlashTraceTexture->SetDeviceTexture( 0 );
			}

//#ifdef DYNAMIC_VERTEX_BUFFER_OPT
			BOOST_STATIC_ASSERT( D3DFVF_SLASH_TRACE_TEXTURED == D3DFVF_XYZ_DIFFUSE_TEX1 );
#ifdef  X2OPTIMIZE_DYNAMICVB_SUPPORT_SPLIT_DATA
            m_iSlashVertexTexturedList_StartIndex %= m_VertexNum;
			g_pKTDXApp->GetDVBManager()->DrawPrimitive_SplitData( CKTDGDynamicVBManager::DVB_TYPE_XYZ_DIFFUSE_TEX1
				, D3DPT_TRIANGLESTRIP, m_VertexNum - 2
                , m_VertexNum - m_iSlashVertexTexturedList_StartIndex
                , &m_pSlashVertexTexturedList[m_iSlashVertexTexturedList_StartIndex]
                , &m_pSlashVertexTexturedList[0], m_DrawCount );
#else   X2OPTIMIZE_DYNAMICVB_SUPPORT_SPLIT_DATA
			g_pKTDXApp->GetDVBManager()->DrawPrimitive( CKTDGDynamicVBManager::DVB_TYPE_XYZ_DIFFUSE_TEX1
				, D3DPT_TRIANGLESTRIP, m_VertexNum - 2, m_pSlashVertexTexturedList, m_DrawCount );
#endif  X2OPTIMIZE_DYNAMICVB_SUPPORT_SPLIT_DATA
//#else
//			g_pKTDXApp->GetDevice()->SetFVF(D3DFVF_SLASH_TRACE_TEXTURED);
//			for( int i = 0; i < m_DrawCount; i++ )
//			{
//				g_pKTDXApp->GetDevice()->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, m_VertexNum - 2
//					, m_pSlashVertexTexturedList, sizeof(VERTEX_SLASH_TRACE_TEXTURED) );
//			}
//#endif
		}
	}
#endif TEXTURED_SLASH_TRACE_TEST



//
//#pragma NOTE( "임시로 텍스쳐입힌 칼궤적 render param 스크립트에서 읽어올 수 있게" )
//	//{{AFX
//	CKTDGStateManager::PopRenderState( D3DRS_DESTBLEND);
//	CKTDGStateManager::PopRenderState( D3DRS_SRCBLEND);
//	CKTDGStateManager::PopRenderState( D3DRS_ALPHABLENDENABLE);
//	//}}AFX



	if( true == m_bHasVisibleVertex && m_VertexNum > 2 )
	{
		CKTDGStateManager::SetTexture( 0, NULL );
//#ifdef DYNAMIC_VERTEX_BUFFER_OPT
		BOOST_STATIC_ASSERT( D3DFVF_XYZ_DIFFUSE == D3DFVF_SLASH_TRACE );
#ifdef  X2OPTIMIZE_DYNAMICVB_SUPPORT_SPLIT_DATA
        m_iSlashVertexList_StartIndex %= m_VertexNum;
		g_pKTDXApp->GetDVBManager()->DrawPrimitive_SplitData( CKTDGDynamicVBManager::DVB_TYPE_XYZ_DIFFUSE
			, D3DPT_TRIANGLESTRIP, m_VertexNum - 2
            , m_VertexNum - m_iSlashVertexList_StartIndex
            , &m_pSlashVertexList[m_iSlashVertexList_StartIndex]
            , &m_pSlashVertexList[0]
            , m_DrawCount );    
#else   X2OPTIMIZE_DYNAMICVB_SUPPORT_SPLIT_DATA
		g_pKTDXApp->GetDVBManager()->DrawPrimitive( CKTDGDynamicVBManager::DVB_TYPE_XYZ_DIFFUSE
			, D3DPT_TRIANGLESTRIP, m_VertexNum - 2, m_pSlashVertexList, m_DrawCount );        
#endif  X2OPTIMIZE_DYNAMICVB_SUPPORT_SPLIT_DATA
//#else
//		g_pKTDXApp->GetDevice()->SetFVF(D3DFVF_SLASH_TRACE);
//		for( int i = 0; i < m_DrawCount; i++ )
//		{
//			g_pKTDXApp->GetDevice()->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, m_VertexNum - 2
//				, m_pSlashVertexList, sizeof(VERTEX_SLASH_TRACE) );
//		}//if
//#endif
	}

	//GetMatrix().SetMatrixSet( oldMatrixSet );
}//CKTDGSlashTrace::OnFrameRender_Draw()

//}} robobeg : 2008-10-24

//#ifndef DYNAMIC_VERTEX_BUFFER_OPT
//HRESULT CKTDGSlashTrace::OnResetDevice()
//{
//	HRESULT hr = S_OK;
//
//	if ( m_bUseDynamic == true )
//	{
//		SAFE_RELEASE( m_pSlashVB );
//
//		if(FAILED(hr = g_pKTDXApp->GetDevice()->CreateVertexBuffer( m_VertexNum * sizeof(VERTEX_SLASH_TRACE), 
//			D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, D3DFVF_SLASH_TRACE, 
//			D3DPOOL_DEFAULT, &m_pSlashVB, NULL )))
//		{
//			wcout << L"Fail: CKTDGSlashTrace::CreateVertexBuffer" << std::endl;
//			ErrorLog( KEM_ERROR34 );
//			return hr;
//		}
//	}
//
//	return hr;
//}
//
//HRESULT CKTDGSlashTrace::OnLostDevice()
//{
//	HRESULT hr = S_OK;
//
//	if ( m_bUseDynamic == true )
//	{
//		SAFE_RELEASE( m_pSlashVB );
//	}
//
//	return hr;
//}
//#endif



void CKTDGSlashTrace::AddSlashData( D3DXVECTOR3 posUp, D3DXVECTOR3 posDown, D3DXCOLOR color, bool bColorGradation /*= true*/, 
								   const float fTextureStride /*= 0.f*/, const wstring& wstrTextureName /*= L""*/ )
{
	KTDXPROFILE();

	bool bTexturedSlashTrace = false;

#ifdef TEXTURED_SLASH_TRACE_TEST
	if( true == m_bEnabledSlashTraceTexture )
	{
		if( false == wstrTextureName.empty() )
		{
			m_pCurrSlashTraceTexture = GetSlashTraceTexture( wstrTextureName );
			if( NULL != m_pCurrSlashTraceTexture )
			{
				bTexturedSlashTrace = true;
			}
		}

		if( false == bTexturedSlashTrace )
		{
			if( false == m_DefaultSlashTraceTextureName.empty() )
			{
				m_pCurrSlashTraceTexture = GetSlashTraceTexture( m_DefaultSlashTraceTextureName );
				if( NULL != m_pCurrSlashTraceTexture )
				{
					bTexturedSlashTrace = true;
				}
			}
		}
	}





	if( true == bTexturedSlashTrace )
	{
		if( false == m_bHasVisibleVertexTextured )
		{
			if( 0.f == color.a )
				return;

			m_SplineBufUp1 = posUp;
			m_SplineBufUp2 = posUp;
			m_SplineBufUp3 = posUp;
			m_SplineBufUp4 = posUp;

			m_SplineBufDown1 = posDown;
			m_SplineBufDown2 = posDown;
			m_SplineBufDown3 = posDown;
			m_SplineBufDown4 = posDown;

			for( int i=0; i<m_VertexNum; i++ )
			{
				m_pSlashVertexTexturedList[i].position = posUp;
				m_pSlashVertexTexturedList[i].color = D3DCOLOR_RGBA( 0, 0, 0, 0 );
				//m_pSlashVertexTexturedList[i].texUV = D3DXVECTOR2( 0, 0 );
			}

			m_bHasVisibleVertexTextured = true;
		}
		else
		{
			m_SplineBufUp1 = m_SplineBufUp2;
			m_SplineBufUp2 = m_SplineBufUp3;
			m_SplineBufUp3 = m_SplineBufUp4;
			m_SplineBufUp4 = posUp;

			//D3DXVECTOR3 v2To3 = m_SplineBufUp3 - m_SplineBufUp2;
			//D3DXVECTOR3 v3To4 = m_SplineBufUp4 - m_SplineBufUp3;
			//float fLengthSquare23 = D3DXVec3LengthSq( &v2To3 );
			//float fLengthSquare34 = D3DXVec3LengthSq( &v3To4 );
			//float fLengthSquare45 = ( fLengthSquare23 + fLengthSquare34 ) * 0.5f;


			//D3DXVec3Normalize( &v2To3, &v2To3 );
			//D3DXVec3Normalize( &v3To4, &v3To4 );
			//float fDot = D3DXVec3Dot( &v2To3, &v3To4 );
			//float fRad = acosf( fDot );
			//D3DXVECTOR3 vCross = D3DXVec3Cross( &v2To3, &v3To4 );

			//D3DXMATRIX matRotation;
			//D3DXMatrixRotationAxis( &matRotation, &vCross, fRad );
			//m_SplineBufUp5 = 
				
				
			m_SplineBufDown1 = m_SplineBufDown2;
			m_SplineBufDown2 = m_SplineBufDown3;
			m_SplineBufDown3 = m_SplineBufDown4;
			m_SplineBufDown4 = posDown;
		}
	}
	else
#endif TEXTURED_SLASH_TRACE_TEST
	{
		if( false == m_bHasVisibleVertex )
		{
			if( 0.f == color.a )
				return;

			m_SplineBufUp1 = posUp;
			m_SplineBufUp2 = posUp;
			m_SplineBufUp3 = posUp;
			m_SplineBufUp4 = posUp;

			m_SplineBufDown1 = posDown;
			m_SplineBufDown2 = posDown;
			m_SplineBufDown3 = posDown;
			m_SplineBufDown4 = posDown;

			for( int i=0; i<m_VertexNum; i++ )
			{
				m_pSlashVertexList[i].position = posUp;
				m_pSlashVertexList[i].color = D3DCOLOR_RGBA( 0, 0, 0, 0 );
			}

			m_bHasVisibleVertex = true;
		}
		else
		{
			m_SplineBufUp1 = m_SplineBufUp2;
			m_SplineBufUp2 = m_SplineBufUp3;
			m_SplineBufUp3 = m_SplineBufUp4;
			m_SplineBufUp4 = posUp;

			m_SplineBufDown1 = m_SplineBufDown2;
			m_SplineBufDown2 = m_SplineBufDown3;
			m_SplineBufDown3 = m_SplineBufDown4;
			m_SplineBufDown4 = posDown;
		}
	}


	
	
	
	switch( m_eCurveType )
	{
	case CT_SMOOTH_CURVE:
		{
#ifdef SLASH_TRACE_BUG_FIX
			int split = 0;
#endif
			if( color.a == 0.0f )
			{
#ifdef TEXTURED_SLASH_TRACE_TEST
				if( true == m_bEnabledSlashTraceTexture )
				{
					PushRenderBuffer( m_SplineBufUp3, m_SplineBufDown3, color, bColorGradation, true );		// textured vertex buffer에 안보이는 slash trace를 추가
				}

				PushRenderBuffer( m_SplineBufUp3, m_SplineBufDown3, color, bColorGradation, false );		// vertex buffer에 안보이는 slash trace를 추가

#else TEXTURED_SLASH_TRACE_TEST
				//{{AFX
				PushRenderBuffer( m_SplineBufUp3, m_SplineBufDown3, color, bColorGradation, bTexturedSlashTrace );
				//}}AFX
#endif TEXTURED_SLASH_TRACE_TEST
			}
			else
			{


				//#pragma NOTE( "임시로 텍스쳐입힌 칼궤적 render param 스크립트에서 읽어올 수 있게" )
				//		if( true == bTexturedSlashTrace )
				//		{
				//			color.a = fSlashTraceAlpha;
				//		}



				const float MAGIC_GAP_THRESHOLD = 20.f;
				float dist = GetDistance( m_SplineBufUp2, m_SplineBufUp3 );
				if( dist <= MAGIC_GAP_THRESHOLD )
				{
					PushRenderBuffer( m_SplineBufUp3, m_SplineBufDown3, color, bColorGradation, bTexturedSlashTrace );
				}
				else
				{
					D3DXVECTOR3 outDataUp;
					D3DXVECTOR3 outDataDown;


					//D3DXVECTOR3 vUpDir = m_SplineBufUp4 - m_SplineBufUp3;
					//D3DXVec3Normalize( &vUpDir, &vUpDir );
					//D3DXVECTOR3 vDownDir = m_SplineBufDown4 - m_SplineBufDown3;
					//D3DXVec3Normalize( &vDownDir, &vDownDir );
					//D3DXVECTOR3 vSplineUp5 = m_SplineBufUp4 + vUpDir * 10.f;
					//D3DXVECTOR3 vSplineDown5 = m_SplineBufDown4 + vDownDir * 10.f;


#ifdef SLASH_TRACE_BUG_FIX
					split = (int)(dist / MAGIC_GAP_THRESHOLD) + 1;
					split = __min( split, SLASH_TRACE_MAX_SPLIT );
#else
					int split = (int)(dist / MAGIC_GAP_THRESHOLD) + 1;
#endif
					float gap = 1.0f / split;
					for( int i = 0; i < split; i++ )
					{
						D3DXVec3CatmullRom( &outDataUp, &m_SplineBufUp1, &m_SplineBufUp2, &m_SplineBufUp3, &m_SplineBufUp4, i * gap );
						D3DXVec3CatmullRom( &outDataDown, &m_SplineBufDown1, &m_SplineBufDown2, &m_SplineBufDown3, &m_SplineBufDown4, i * gap );

						PushRenderBuffer( outDataUp, outDataDown, color, bColorGradation, bTexturedSlashTrace );
					}
				}
			}

		} break;

	case CT_TOUGH_CURVE:
		{
#ifdef SLASH_TRACE_BUG_FIX
			int split = 0;
#endif
			if( color.a == 0.0f )
			{
				if( true == m_bEnabledSlashTraceTexture )
				{
					PushRenderBuffer( m_SplineBufUp4, m_SplineBufDown4, color, bColorGradation, true );		// textured vertex buffer에 안보이는 slash trace를 추가
				}

				PushRenderBuffer( m_SplineBufUp4, m_SplineBufDown4, color, bColorGradation, false );		// vertex buffer에 안보이는 slash trace를 추가
			}
			else
			{
				const float MAGIC_GAP_THRESHOLD = 20.f;
				float dist = GetDistance( m_SplineBufUp3, m_SplineBufUp4 );
				if( dist <= MAGIC_GAP_THRESHOLD )
				{
					PushRenderBuffer( m_SplineBufUp4, m_SplineBufDown4, color, bColorGradation, bTexturedSlashTrace );
				}
				else
				{
					D3DXVECTOR3 outDataUp;
					D3DXVECTOR3 outDataDown;

#ifdef SLASH_TRACE_BUG_FIX
					split = (int)(dist / MAGIC_GAP_THRESHOLD) + 1;
					split = __min( split, SLASH_TRACE_MAX_SPLIT );
#else
					int split = (int)(dist / MAGIC_GAP_THRESHOLD) + 1;
#endif
					float gap = 1.0f / split;
					for( int i = 0; i < split; i++ )
					{
						D3DXVec3CatmullRom( &outDataUp, &m_SplineBufUp2, &m_SplineBufUp3, &m_SplineBufUp4, &m_SplineBufUp4, i * gap );
						D3DXVec3CatmullRom( &outDataDown, &m_SplineBufDown2, &m_SplineBufDown3, &m_SplineBufDown4, &m_SplineBufUp4, i * gap );

						PushRenderBuffer( outDataUp, outDataDown, color, bColorGradation, bTexturedSlashTrace );
					}
				}
			}
		} break;

	case CT_STRAIGHT_LINE:
		{
			if( color.a == 0.0f )
			{
				if( true == m_bEnabledSlashTraceTexture )
				{
					PushRenderBuffer( posUp, posDown, color, bColorGradation, true );		// textured vertex buffer에 안보이는 slash trace를 추가
				}
				PushRenderBuffer( posUp, posDown, color, bColorGradation, false );		// vertex buffer에 안보이는 slash trace를 추가
			}
			else
			{
				PushRenderBuffer( posUp, posDown, color, bColorGradation, bTexturedSlashTrace );
			}
		} break;
	}
}





void CKTDGSlashTrace::PushRenderBuffer( D3DXVECTOR3 posUp, D3DXVECTOR3 posDown, D3DXCOLOR color, bool bColorGradation, bool bTexturedSlashTrace /*= false*/, float fTextureStride /*= 0.f*/ )
{
	KTDXPROFILE();

#ifdef TEXTURED_SLASH_TRACE_TEST
	if( true == m_bEnabledSlashTraceTexture && true == bTexturedSlashTrace )
	{
		VERTEX_SLASH_TRACE_TEXTURED	vertexUp;
		VERTEX_SLASH_TRACE_TEXTURED	vertexDown;

		vertexUp.position	= posUp;
		vertexUp.color		= color;
		vertexDown.position	= posDown;
		vertexDown.color	= color;
#ifndef  X2OPTIMIZE_DYNAMICVB_SUPPORT_SPLIT_DATA
		for( int i = m_VertexNum - 1; i >= 3; i -= 2 )
		{
			m_pSlashVertexTexturedList[i]	= m_pSlashVertexTexturedList[i-2];
			m_pSlashVertexTexturedList[i-1]	= m_pSlashVertexTexturedList[i-3];
		}
#endif  X2OPTIMIZE_DYNAMICVB_SUPPORT_SPLIT_DATA

		if( m_bHighLight == true )
		{
			color.a				= 1.0f;
			vertexUp.color		= color;
			vertexDown.color	= color;
		}
		
#ifdef  X2OPTIMIZE_DYNAMICVB_SUPPORT_SPLIT_DATA
        if ( m_VertexNum >= 2 )
        {
            m_iSlashVertexTexturedList_StartIndex = ( m_iSlashVertexTexturedList_StartIndex + m_VertexNum - 2 ) % m_VertexNum;
            m_pSlashVertexTexturedList[m_iSlashVertexTexturedList_StartIndex] = vertexUp;
            m_pSlashVertexTexturedList[m_iSlashVertexTexturedList_StartIndex+1] = vertexDown;
        }
#else   X2OPTIMIZE_DYNAMICVB_SUPPORT_SPLIT_DATA
		m_pSlashVertexTexturedList[0] = vertexUp;
		m_pSlashVertexTexturedList[1] = vertexDown;
#endif  X2OPTIMIZE_DYNAMICVB_SUPPORT_SPLIT_DATA
	}
	else
#endif TEXTURED_SLASH_TRACE_TEST
	{

		VERTEX_SLASH_TRACE	vertexUp;
		VERTEX_SLASH_TRACE	vertexDown;

		vertexUp.position	= posUp;
		vertexUp.color		= color;
		vertexDown.position	= posDown;
		if( true == bColorGradation )
		{
			color.a /= 10.f;
		}
		vertexDown.color	= color;

#ifndef  X2OPTIMIZE_DYNAMICVB_SUPPORT_SPLIT_DATA
		for( int i = m_VertexNum - 1; i >= 3; i-- )
		{
			m_pSlashVertexList[i]	= m_pSlashVertexList[i-2];
			m_pSlashVertexList[i-1]	= m_pSlashVertexList[i-3];
			i--;
		}
#endif  X2OPTIMIZE_DYNAMICVB_SUPPORT_SPLIT_DATA

		if( m_bHighLight == true )
		{
			color.a				= 1.0f;
			vertexUp.color		= color;
			vertexDown.color	= color;
		}
#ifdef  X2OPTIMIZE_DYNAMICVB_SUPPORT_SPLIT_DATA
        if ( m_VertexNum >= 2 )
        {
            m_iSlashVertexList_StartIndex = ( m_iSlashVertexList_StartIndex + m_VertexNum - 2 ) % m_VertexNum;
            m_pSlashVertexList[m_iSlashVertexList_StartIndex] = vertexUp;
            m_pSlashVertexList[m_iSlashVertexList_StartIndex+1] = vertexDown;
        }
#else   X2OPTIMIZE_DYNAMICVB_SUPPORT_SPLIT_DATA
		m_pSlashVertexList[0] = vertexUp;
		m_pSlashVertexList[1] = vertexDown;
#endif  X2OPTIMIZE_DYNAMICVB_SUPPORT_SPLIT_DATA
	}
}


void CKTDGSlashTrace::AddSlashDataConstantWidth( D3DXVECTOR3 posUp, D3DXVECTOR3 posDown, const D3DXCOLOR& color, 
												const D3DXVECTOR3& vNormalizedLineDir, bool bColorGradation /*= false*/  )
{
	D3DXVECTOR3 vCenter = ( posUp + posDown ) * 0.5f;
	D3DXVECTOR3 vDir = vCenter - m_vOldCenter;
	m_vOldCenter = vCenter;
	

	D3DXVECTOR3 vRadius = posUp - posDown;
	float fRadius = D3DXVec3Length( &vRadius ) * 0.5f;


	D3DXVECTOR3 vUp(0,1,0);
	D3DXVECTOR3 vLookVec(0,0,0);

	//D3DXVECTOR3 vRightVec(0,0,0);
	//D3DXVec3Cross( &vRightVec, &vUp, &vLookVec );
	D3DXVec3Cross( &vLookVec, &vNormalizedLineDir, &vUp );
	

	float fDotX = D3DXVec3Dot( &vDir, &vNormalizedLineDir );
	D3DXVECTOR3 vProjected = fDotX * vNormalizedLineDir;
	vProjected.y = vDir.y;
	
	D3DXVECTOR3 vCross( 0, 0, 0);
	D3DXVec3Cross( &vCross, &vProjected, &vLookVec );
	D3DXVec3Normalize( &vCross, &vCross );

	posUp	= vCenter + vCross * fRadius;
	posDown = vCenter - vCross * fRadius;

	AddSlashData( posUp, posDown, color, bColorGradation );
}


void CKTDGSlashTrace::AddSlashDataCircular( D3DXVECTOR3 vCircleCenter, D3DXVECTOR3 posUp, D3DXVECTOR3 posDown, const D3DXCOLOR& color, 
											const D3DXVECTOR3& vNormalizedLineDir, const float fThreshold/* = 50.f*/, bool bColorGradation /*= true */)
{
	D3DXVECTOR3 vUp( 0, 1, 0 );
	D3DXVECTOR3 vPos = ( posUp + posDown ) * 0.5f;
	D3DXVECTOR3 vCircleRadius = vPos - vCircleCenter;
	float fDotX = D3DXVec3Dot( &vCircleRadius, &vNormalizedLineDir );
	float fDotY = D3DXVec3Dot( &vCircleRadius, &vUp );
	vPos = vCircleCenter + vNormalizedLineDir * fDotX + vUp * fDotY;

	float fCircleRadius = D3DXVec3Length( &vCircleRadius );
	float fSlashWidth = D3DXVec3Length( &( posUp - posDown ) );

	if( fabs( fCircleRadius - m_fOldCircleRadius ) > fThreshold )
	{
		m_fOldCircleRadius = fCircleRadius;
	}
	else
	{
		fCircleRadius = m_fOldCircleRadius;
	}

	D3DXVec3Normalize( &vCircleRadius, &vCircleRadius );

	D3DXVECTOR3 vOuterPos = vCircleCenter + vCircleRadius * ( fCircleRadius + fSlashWidth * 0.5f );
	D3DXVECTOR3 vInnerPos = vCircleCenter + vCircleRadius * ( fCircleRadius - fSlashWidth * 0.5f );
	
	AddSlashData( vOuterPos, vInnerPos, color, bColorGradation );
}


#ifdef TEXTURED_SLASH_TRACE_TEST
	CKTDXDeviceTexture* CKTDGSlashTrace::GetSlashTraceTexture( const wstring& wstrTextureName )
	{
		if( true == wstrTextureName.empty() )
			return NULL;

		SlashTraceTextureMap::const_iterator it = m_mapSlashTraceTexture.find( wstrTextureName );
		if( it != m_mapSlashTraceTexture.end() )
		{
			return (CKTDXDeviceTexture*) it->second;
		}
			
		CKTDXDeviceTexture* pTexture = g_pKTDXApp->GetDeviceManager()->OpenTexture( wstrTextureName );
		if( NULL == pTexture )
			return NULL;

		m_mapSlashTraceTexture[ wstrTextureName ] = pTexture;
		return pTexture;
	}

#endif TEXTURED_SLASH_TRACE_TEST
