#include "StdAfx.h"
#include ".\ktdxdevicetexture.h"
#include ".\KTDXDeviceManager.h"

CKTDXDeviceTexture::CKTDXDeviceTexture( LPDIRECT3DDEVICE9 pd3dDevice, wstring fileName, DETAIL_LEVEL detailLevel, D3DFORMAT texFormat, bool bAlwaysHigh )
	: CKTDXDeviceBaseTexture( fileName, DT_TEXTURE )
{
	m_DeviceType	= DT_TEXTURE;
	
	m_pTexture		= NULL;
	ZeroMemory( &m_Info, sizeof(D3DXIMAGE_INFO)	);
    m_uWidth = 0;
    m_uHeight = 0;

	m_DetailLevel	= detailLevel;
	m_LoadTexFormat	= texFormat;
	m_bAlwaysHigh	= bAlwaysHigh;

	//m_pVB = NULL;
}

CKTDXDeviceTexture::~CKTDXDeviceTexture()
{
	_UnLoad();
}

/*virtual*/ HRESULT CKTDXDeviceTexture::_Load( bool bSkipStateCheck
#ifdef	X2OPTIMIZE_SOUND_BACKGROUND_LOAD
				, bool
#endif	X2OPTIMIZE_SOUND_BACKGROUND_LOAD		
	)
{

	HRESULT hr;
    int     iSize = 0;
    D3DXIMAGE_INFO  imgInfo;
    LPDIRECT3DTEXTURE9	pTexture = NULL;
    unsigned uWidth = 0;
    unsigned uHeight = 0;
	unsigned uMipmap = 1;
	D3DFORMAT d3dFormat = D3DFMT_UNKNOWN;
	DWORD	dwFilter = D3DX_FILTER_NONE;


    KGCMassFileManager::CMassFile::MASSFILE_MEMBERFILEINFO_POINTER Info;
#ifdef  X2OPTIMIZE_SUPPORT_LOADING_TWO_ADJACENT_MASS_FILES
    KGCMassFileManager::CMassFile::MASSFILE_MEMBERFILEINFO_POINTER Info2;
	wstring deviceTexUVRectName; 
	int strSize = m_DeviceID.size();
	if ( strSize > 3 )
    {
        deviceTexUVRectName.reserve( m_DeviceID.size() );
	    deviceTexUVRectName.assign( m_DeviceID.begin(), m_DeviceID.begin() + strSize - 3 );
	    deviceTexUVRectName += L"TET";
    }
	Info = g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->LoadTwoDataFiles( m_DeviceID, deviceTexUVRectName, Info2 );
#else   X2OPTIMIZE_SUPPORT_LOADING_TWO_ADJACENT_MASS_FILES
	Info = g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->LoadDataFile( m_DeviceID );
#endif  X2OPTIMIZE_SUPPORT_LOADING_TWO_ADJACENT_MASS_FILES
	if( Info == NULL )
	{
		pTexture		= NULL;

		string strName;
		ConvertWCHARToChar( strName, m_DeviceID.c_str() );
		ErrorLogMsg( KEM_ERROR88, strName.c_str());

		hr = E_FAIL;
		goto fail;
	}

	iSize = Info->size;

	hr = D3DXGetImageInfoFromFileInMemory( (void*)Info->pRealData, Info->size, &imgInfo );
	if( hr != S_OK )
	{
		pTexture		= NULL;

		string strName;
		ConvertWCHARToChar( strName, m_DeviceID.c_str() );
		ErrorLogMsg( KEM_ERROR89, strName.c_str());

		goto fail;
	}

	uWidth = imgInfo.Width;
	uHeight = imgInfo.Height;
	d3dFormat = m_LoadTexFormat;
	dwFilter = D3DX_FILTER_NONE;

	if ( false == m_bAlwaysHigh )
	{
		switch( m_DetailLevel )
		{
		case DL_HIGH:
			break;
		case DL_MEDIUM:
		    uWidth = imgInfo.Width;
            uHeight = imgInfo.Height;
            d3dFormat = imgInfo.Format;
			if( uWidth >= 128 )
				uWidth /= 2;
			if( uHeight >= 128 )
				uHeight /= 2;
			if( d3dFormat == D3DFMT_R8G8B8 || d3dFormat == D3DFMT_R5G6B5 )
				d3dFormat = D3DFMT_DXT1;
			else
				d3dFormat = m_LoadTexFormat;
			break;
		case DL_LOW:
            uWidth = imgInfo.Width;
            uHeight = imgInfo.Height;
            d3dFormat = imgInfo.Format;
			if( uWidth >= 64 && uWidth < 128 )
				uWidth /= 2;
			else if( uWidth >= 128 )
				uWidth /= 4;
			if( uHeight >= 64 && uHeight < 128 )
				uHeight /= 2;
			else if( uHeight >= 128 )
				uHeight /= 4;
			if( d3dFormat == D3DFMT_R8G8B8 || d3dFormat == D3DFMT_R5G6B5 )
				d3dFormat = D3DFMT_DXT1;
			else
				d3dFormat = D3DFMT_DXT3;
			break;
		}//switch
	}

	hr = D3DXCheckTextureRequirements( g_pKTDXApp->GetDevice(), 
		&uWidth, &uHeight, &uMipmap, 0, &d3dFormat, D3DPOOL_MANAGED );
	if ( hr != D3D_OK )
		goto fail;

	if ( uWidth != imgInfo.Width
		|| uHeight != imgInfo.Height )
	{
		dwFilter = D3DX_FILTER_LINEAR;
	}//if

	hr = D3DXCreateTextureFromFileInMemoryEx( g_pKTDXApp->GetDevice()
		, (void*)Info->pRealData, Info->size
		, uWidth, uHeight, 1, 0, d3dFormat, D3DPOOL_MANAGED, dwFilter
		, D3DX_FILTER_NONE, 0, &imgInfo, NULL, &pTexture );
	if ( hr != D3D_OK || pTexture == NULL )
		goto fail;
		
	{
#ifdef  X2OPTIMIZE_TET_XET_PREPROCESSING
        CKTDXDeviceTET_Preprocessing   proxy;
        {
#ifdef  X2OPTIMIZE_SUPPORT_LOADING_TWO_ADJACENT_MASS_FILES
            if ( Info2 != NULL )
            {
                if ( proxy.OpenFile( (const BYTE*) Info2.pRealData, Info2.size, m_DeviceID ) == false )
                {
                    ErrorLogMsg( KEM_ERROR95, deviceTexUVRectName.c_str() );
                }
            }
#else   X2OPTIMIZE_SUPPORT_LOADING_TWO_ADJACENT_MASS_FILES
			wstring deviceTexUVRectName; 
			int strSize = m_DeviceID.size();
			ASSERT( strSize > 3 );
			deviceTexUVRectName.assign( m_DeviceID.begin(), m_DeviceID.begin() + strSize - 3 );
			deviceTexUVRectName += L"TET";
			KGCMassFileManager::CMassFile::MASSFILE_MEMBERFILEINFO_POINTER Info;
			Info = g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->LoadDataFile( deviceTexUVRectName.c_str() );
            if ( Info != NULL )
            {
                if ( proxy.OpenFile( deviceTexUVRectName.c_str(), m_DeviceID ) == false )
                {
                    ErrorLogMsg( KEM_ERROR95, deviceTexUVRectName.c_str() );
                }
            }
#endif  X2OPTIMIZE_SUPPORT_LOADING_TWO_ADJACENT_MASS_FILES
        }
#else   X2OPTIMIZE_TET_XET_PREPROCESSING
		TETProxy    proxy( GetDeviceID() );
		{
#ifdef  X2OPTIMIZE_SUPPORT_LOADING_TWO_ADJACENT_MASS_FILES
            if ( Info2 != NULL )
            {
				lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(), "m_pTEX", &proxy );
				if( g_pKTDXApp->GetLuaBinder()->DoMemoryNotEncrypt( Info2->pRealData, Info2->size ) == false )
				{
					ErrorLogMsg( KEM_ERROR95, deviceTexUVRectName.c_str() );
				}
            }
#else   X2OPTIMIZE_SUPPORT_LOADING_TWO_ADJACENT_MASS_FILES
			wstring deviceTexUVRectName; 
			int strSize = m_DeviceID.size();
			ASSERT( strSize > 3 );
			deviceTexUVRectName.assign( m_DeviceID.begin(), m_DeviceID.begin() + strSize - 3 );
			deviceTexUVRectName += L"TET";

			KGCMassFileManager::CMassFile::MASSFILE_MEMBERFILEINFO_POINTER Info;
			Info = g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->LoadDataFile( deviceTexUVRectName.c_str() );
			if ( Info != NULL )
			{
				lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(), "m_pTEX", &proxy );
				if( g_pKTDXApp->GetLuaBinder()->DoMemoryNotEncrypt( Info->pRealData, Info->size ) == false )
				{
					ErrorLogMsg( KEM_ERROR95, deviceTexUVRectName.c_str() );
				}
			}//if
#endif  X2OPTIMIZE_SUPPORT_LOADING_TWO_ADJACENT_MASS_FILES
		}
#endif  X2OPTIMIZE_TET_XET_PREPROCESSING


        {
            CSLock  locker( m_csDeviceState );

            if ( bSkipStateCheck == false )
		    {
			    switch( m_eDeviceState )
			    {
			    case DEVICE_STATE_INIT:
			    //case DEVICE_STATE_PENDING:
			    default:
				    ASSERT( 0 );
			    case DEVICE_STATE_LOADING:
			    case DEVICE_STATE_FAILED:
                    std::swap( m_pTexture, pTexture );
//#ifdef  REMOVE_DRAW_PRIMITIVE_UP
//                    std::swap( m_pVB, pVB );
//#endif	REMOVE_DRAW_PRIMITIVE_UP
                    m_Size = iSize;
#ifdef  X2OPTIMIZE_TET_XET_PREPROCESSING
                    m_tet.Swap( proxy );
#else   X2OPTIMIZE_TET_XET_PREPROCESSING
				    m_mapTexUVRect.swap( proxy.m_mapTexUVRect );
#endif  X2OPTIMIZE_TET_XET_PREPROCESSING
                    m_uWidth = uWidth;
                    m_uHeight = uHeight;
					m_Info = imgInfo;
				    m_eDeviceState = DEVICE_STATE_LOADED;
				    break;
			    case DEVICE_STATE_LOADED:
				    break;
			    }//switch
		    }
            else
            {
                ASSERT( m_eDeviceState == DEVICE_STATE_LOADED );

                std::swap( m_pTexture, pTexture );
//#ifdef  REMOVE_DRAW_PRIMITIVE_UP
//                std::swap( m_pVB, pVB );
//#endif	REMOVE_DRAW_PRIMITIVE_UP
                m_Size = iSize;
#ifdef  X2OPTIMIZE_TET_XET_PREPROCESSING
                m_tet.Swap( proxy );
#else   X2OPTIMIZE_TET_XET_PREPROCESSING
                m_mapTexUVRect.swap( proxy.m_mapTexUVRect );
#endif  X2OPTIMIZE_TET_XET_PREPROCESSING
                m_uWidth = uWidth;
                m_uHeight = uHeight;
				m_Info = imgInfo;
            }//if.. else..
        }

    	SAFE_RELEASE( pTexture );
//#ifdef  REMOVE_DRAW_PRIMITIVE_UP
//		SAFE_RELEASE( pVB );
//#endif	REMOVE_DRAW_PRIMITIVE_UP

    }

	return S_OK;

fail:

	SAFE_RELEASE( pTexture );
//#ifdef  REMOVE_DRAW_PRIMITIVE_UP
//	SAFE_RELEASE( pVB );
//#endif  REMOVE_DRAW_PRIMITIVE_UP

	wstring errorMSG;
	if ( hr == D3DERR_NOTAVAILABLE )
	{
		errorMSG += L"D3DERR_NOTAVAILABLE";
	}
	else if ( hr == D3DERR_OUTOFVIDEOMEMORY )
	{
		errorMSG += L"D3DERR_OUTOFVIDEOMEMORY";
	}
	else if ( hr == D3DERR_INVALIDCALL )
	{
		errorMSG += L"D3DERR_INVALIDCALL";
	}
	else if ( hr == D3DXERR_INVALIDDATA )
	{
		errorMSG += L"D3DXERR_INVALIDDATA";
	}
	else if ( hr == E_OUTOFMEMORY )
	{
		errorMSG += L"E_OUTOFMEMORY";
	}
	else
	{
		errorMSG += L"알수없는에러";
	}

	errorMSG += L" ";
	errorMSG += m_DeviceID;

	ErrorLogMsg( KEM_ERROR90, errorMSG.c_str() );

	return  E_FAIL;
}//CKTDXDeviceTexture::Load()

HRESULT CKTDXDeviceTexture::_UnLoad()
{
	////CKTDXThread::CLocker locker( g_pKTDXApp->GetDeviceManager()->GetDeviceLock() );
//#ifdef  REMOVE_DRAW_PRIMITIVE_UP
//	SAFE_RELEASE( m_pVB );
//#endif  REMOVE_DRAW_PRIMITIVE_UP
	SAFE_RELEASE( m_pTexture );
	//m_bLoading = false;

#ifdef  X2OPTIMIZE_TET_XET_PREPROCESSING
    m_tet.Release();
#else   X2OPTIMIZE_TET_XET_PREPROCESSING
	KeyTexUVMap::iterator i;
	for ( i = m_mapTexUVRect.begin(); i != m_mapTexUVRect.end(); i++ )
	{
		TEXTURE_UV* pTextrueUV = i->second;
		SAFE_DELETE( pTextrueUV );
	}
	m_mapTexUVRect.clear();
#endif  X2OPTIMIZE_TET_XET_PREPROCESSING
	m_eDeviceState = DEVICE_STATE_INIT;

	return S_OK;
}


//{{ seojt // 2008-10-15, 15:03
void CKTDXDeviceTexture::PreLoad()
{
    if ( m_pTexture )
        m_pTexture->PreLoad();
//#ifdef  REMOVE_DRAW_PRIMITIVE_UP
//    if ( m_pVB )
//        m_pVB->PreLoad();
//#endif  REMOVE_DRAW_PRIMITIVE_UP
}//CKTDXDeviceTexture::PreLoad()
//}} seojt // 2008-10-15, 15:03


void CKTDXDeviceTexture::SetDeviceTexture( int stage )
{
	KTDXPROFILE();

//{{ robobeg : 2008-10-24
	//if( m_pTexture == NULL )
	//{
	//	ErrorLogMsg( KEM_ERROR94, m_DeviceID.c_str() );
	//}
	//else
	//{
	//	wstring errMsg;
	//	HRESULT hr;
	//	if( FAILED ( hr = g_pKTDXApp->GetDevice()->SetTexture( stage, m_pTexture ) ) )
	//	{
	//		switch( hr )
	//		{
	//		case D3DERR_INVALIDCALL:
	//			errMsg = L"D3DERR_INVALIDCALL";
	//			errMsg += L" ";
	//			errMsg += m_DeviceID.c_str();
	//			ErrorLogMsg( KEM_ERROR128, errMsg.c_str() );
	//			break;

	//		default:
	//			errMsg = L"D3DERR_UNKNOWN";
	//			errMsg += L" ";
	//			errMsg += m_DeviceID.c_str();
	//			ErrorLogMsg( KEM_ERROR128, errMsg.c_str() );
	//			break;
	//		}
	//	}
	//}
	//g_pKTDXApp->SetBeforeTexture( stage, m_pTexture );

    if ( m_pTexture == NULL )
    {
        ErrorLogMsg( KEM_ERROR94, m_DeviceID.c_str() );
    }
    else
    {
        HRESULT hr = CKTDGStateManager::SetTexture( stage, m_pTexture );
        if ( FAILED( hr ) )
        {
	        wstring errMsg;
			switch( hr )
			{
			case D3DERR_INVALIDCALL:
				errMsg = L"D3DERR_INVALIDCALL";
				errMsg += L" ";
				errMsg += m_DeviceID.c_str();
				ErrorLogMsg( KEM_ERROR128, errMsg.c_str() );
				break;

			default:
				errMsg = L"D3DERR_UNKNOWN";
				errMsg += L" ";
				errMsg += m_DeviceID.c_str();
				ErrorLogMsg( KEM_ERROR128, errMsg.c_str() );
				break;
			}
        }//if
    }//if
//}} robobeg : 2008-10-24
}

void CKTDXDeviceTexture::SetDetailLevel( DETAIL_LEVEL detailLevel )
{
	if( m_DetailLevel == detailLevel )
		return;

	m_DetailLevel = detailLevel;
	//if( m_bLoading == true && m_bAlwaysHigh == false )
    if ( IsLoaded() == true && m_bAlwaysHigh == false )
	{
		_Load( true );
	}
}

#ifndef  X2OPTIMIZE_TET_XET_PREPROCESSING

const CKTDXDeviceTexture::TEXTURE_UV* CKTDXDeviceTexture::GetTexUV( const wstring& key )
{
	KTDXPROFILE();
	
	if ( key.empty() )
		return NULL;


//#ifdef MAKE_SURE_TEXTURE_PIECE_UPPER_CASE
//	if( false == key.empty() )
//	{
//		string strKey;
//		ConvertWCHARToChar( strKey, key.c_str() );
//		BOOST_TEST_FOREACH( WCHAR, ch, strKey )
//		{
//			ASSERT( 0 == isupper( ch ) );
//		}
//	}
//#endif MAKE_SURE_TEXTURE_PIECE_UPPER_CASE

	KeyTexUVMap::const_iterator i = m_mapTexUVRect.find( key );
    if ( i != m_mapTexUVRect.end() )
	{
		return i->second;
	}

	wstring errorMsg;
	errorMsg = L"GetTexUV실패! 파일 이름 : ";
	errorMsg += m_DeviceID;
	errorMsg += L", 키 이름 : ";
	errorMsg += key;

	ErrorLogMsg( KEM_ERROR113, errorMsg.c_str() );

	return NULL;
}

#endif  X2OPTIMIZE_TET_XET_PREPROCESSING

void CKTDXDeviceTexture::DrawLayer( float nX, float nY, float nWidth, float nHeight, D3DCOLOR color, D3DXVECTOR2 minUV, D3DXVECTOR2 maxUV, int blendType)
{
    KD3DPUSH( g_pKTDXApp->GetDGManager()->GetRSICKTDXDeviceLayer(blendType) )

    D3DXVECTOR3 pos     = D3DXVECTOR3(nX, nY, 0);
    D3DXVECTOR3 size    = D3DXVECTOR3(nWidth, nHeight, 0);

    TEXTURE_RHW_VERTEX vertex[4];
    ZeroMemory( vertex, sizeof(TEXTURE_RHW_VERTEX) * 4 );

    float X1 = (float)pos.x - 0.5f;
    float Y1 = (float)pos.y - 0.5f;
    float X2 = (float)(pos.x + size.x);
    float Y2 = (float)(pos.y + size.y);
    float UMin, UMax, VMin, VMax;

    UMin = minUV.x;
    VMin = minUV.y;
    UMax = maxUV.x;
    VMax = maxUV.y;

    //Set up the 4 corners of a small square
    vertex[0].x = X1;     vertex[0].y = Y1;
    vertex[0].z = 1.0f;   vertex[0].rhw = 1.0f;
    vertex[0].u = UMin;   vertex[0].v = VMin;

    vertex[1].x = X1;     vertex[1].y = Y2;
    vertex[1].u = UMin;   vertex[1].v = VMax;
    vertex[1].z = 1.0f;   vertex[1].rhw = 1.0f;

    vertex[2].x = X2;     vertex[2].y = Y1;
    vertex[2].u = UMax;   vertex[2].v = VMin;
    vertex[2].z = 1.0f;   vertex[2].rhw = 1.0f;

    vertex[3].x = X2;     vertex[3].y = Y2;
    vertex[3].u = UMax;   vertex[3].v = VMax;
    vertex[3].z = 1.0f;   vertex[3].rhw = 1.0f;

    vertex[0].color = vertex[1].color = 
        vertex[2].color = vertex[3].color = color;

    SetDeviceTexture();

    //g_pKTDXApp->GetDevice()->SetFVF( D3DFVF_TEXTURE_RHW_VERTEX );

//#ifdef DYNAMIC_VERTEX_BUFFER_OPT
	BOOST_STATIC_ASSERT( D3DFVF_TEXTURE_RHW_VERTEX == D3DFVF_XYZRHW_DIFFUSE_TEX1 );
	g_pKTDXApp->GetDVBManager()->DrawPrimitive( CKTDGDynamicVBManager::DVB_TYPE_XYZRHW_DIFFUSE_TEX1, D3DPT_TRIANGLESTRIP, 2, vertex );
//#else
//	g_pKTDXApp->GetDevice()->SetFVF( D3DFVF_TEXTURE_RHW_VERTEX );
//	g_pKTDXApp->GetDevice()->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, vertex, sizeof(TEXTURE_RHW_VERTEX) );
//#endif

    KD3DEND()
}


void CKTDXDeviceTexture::Draw( int nX, int nY, int nWidth, int nHeight, D3DCOLOR color /* = 0xffffffff  */, float fRotateDegree /*= 0.f*/ )
{
    KD3DPUSH( g_pKTDXApp->GetDGManager()->GetRSICKTDXDeviceTexture() )

	D3DXVECTOR3 pos		= g_pKTDXApp->ConvertByResolution( (float)nX, (float)nY );
	D3DXVECTOR3 size	= g_pKTDXApp->ConvertByResolution( (float)nWidth, (float)nHeight );



	float X1 = (float)pos.x - 0.5f;
	float Y1 = (float)pos.y - 0.5f;
	float X2 = (float)(pos.x + size.x);
	float Y2 = (float)(pos.y + size.y);


	D3DXVECTOR2 vLT( X1, Y1 );
	D3DXVECTOR2 vRT( X2, Y1 );
	D3DXVECTOR2 vLB( X1, Y2 );
	D3DXVECTOR2 vRB( X2, Y2 );


	if( 0.f != fRotateDegree )
	{
		const float COS_X = cos( fRotateDegree / 180.f * D3DX_PI );
		const float SIN_X = sin( fRotateDegree / 180.f * D3DX_PI );


		D3DXVECTOR2 vCenter( (X1 + X2)*0.5f, (Y1 + Y2)*0.5f );
		D3DXVECTOR2 vLTRotate = vLT - vCenter;	
		D3DXVECTOR2 vRTRotate = vRT - vCenter;
		D3DXVECTOR2 vLBRotate = vLB - vCenter;
		D3DXVECTOR2 vRBRotate = vRB - vCenter;


		vLT.x = vLTRotate.x * COS_X - vLTRotate.y * SIN_X;
		vLT.y = vLTRotate.x * SIN_X + vLTRotate.y * COS_X;
	
		vRT.x = vRTRotate.x * COS_X - vRTRotate.y * SIN_X;
		vRT.y = vRTRotate.x * SIN_X + vRTRotate.y * COS_X;

		vLB.x = vLBRotate.x * COS_X - vLBRotate.y * SIN_X;
		vLB.y = vLBRotate.x * SIN_X + vLBRotate.y * COS_X;

		vRB.x = vRBRotate.x * COS_X - vRBRotate.y * SIN_X;
		vRB.y = vRBRotate.x * SIN_X + vRBRotate.y * COS_X;


		vLT += vCenter;
		vRT += vCenter;
		vLB += vCenter;
		vRB += vCenter;
	}

	

	float UMax = 1.0f;
	float VMax = 1.0f;


	TEXTURE_RHW_VERTEX vertex[4];
	ZeroMemory( vertex, sizeof(TEXTURE_RHW_VERTEX) * 4 );

	//Set up the 4 corners of a small square
	vertex[0].x	= vLT.x;	vertex[0].y		= vLT.y;
	vertex[0].z	= 1.0f;		vertex[0].rhw	= 1.0f;
	vertex[0].u	= 0.0f;		vertex[0].v =	0.0f;

	vertex[1].x	= vLB.x;	vertex[1].y =	vLB.y;
	vertex[1].u	= 0.0f;	  	vertex[1].v =	VMax;
	vertex[1].z	= 1.0f;	  	vertex[1].rhw	= 1.0f;

	vertex[2].x	= vRT.x;	vertex[2].y =	vRT.y;
	vertex[2].u	= UMax;	  	vertex[2].v =	0.0f;
	vertex[2].z	= 1.0f;	  	vertex[2].rhw	= 1.0f;

	vertex[3].x	= vRB.x;	vertex[3].y =	vRB.y;
	vertex[3].u	= UMax;	  	vertex[3].v =	VMax;
	vertex[3].z	= 1.0f;	  	vertex[3].rhw	= 1.0f;

	vertex[0].color	= vertex[1].color =	vertex[2].color	= vertex[3].color =	color;


	SetDeviceTexture();

	//g_pKTDXApp->GetDevice()->SetFVF( D3DFVF_TEXTURE_RHW_VERTEX );



//#ifdef DYNAMIC_VERTEX_BUFFER_OPT
	BOOST_STATIC_ASSERT( D3DFVF_TEXTURE_RHW_VERTEX == D3DFVF_XYZRHW_DIFFUSE_TEX1 );
	g_pKTDXApp->GetDVBManager()->DrawPrimitive( CKTDGDynamicVBManager::DVB_TYPE_XYZRHW_DIFFUSE_TEX1, D3DPT_TRIANGLESTRIP, 2, vertex );
//#else
//	g_pKTDXApp->GetDevice()->SetFVF( D3DFVF_TEXTURE_RHW_VERTEX );
//	g_pKTDXApp->GetDevice()->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, vertex, sizeof(TEXTURE_RHW_VERTEX) );
//#endif
	KD3DEND()
}

void CKTDXDeviceTexture::Draw( int nX, int nY, int nWidth, int nHeight, const TEXTURE_UV* pTextureUV, D3DCOLOR color /* = 0xffffffff  */)
{
    KD3DPUSH( g_pKTDXApp->GetDGManager()->GetRSICKTDXDeviceTexture() )

	D3DXVECTOR3 pos		= g_pKTDXApp->ConvertByResolution( (float)nX, (float)nY );
	D3DXVECTOR3 size	= g_pKTDXApp->ConvertByResolution( (float)nWidth, (float)nHeight );



	float X1 = (float)pos.x - 0.5f;
	float Y1 = (float)pos.y - 0.5f;
	float X2 = (float)(pos.x + size.x);
	float Y2 = (float)(pos.y + size.y);

	float UMax = 1.0f;
	float VMax = 1.0f;

	TEXTURE_RHW_VERTEX vertex[4];
	ZeroMemory( vertex, sizeof(TEXTURE_RHW_VERTEX) * 4 );

	//Set up the 4 corners of a small square
	vertex[0].x = X1;     vertex[0].y = Y1;
	//vertex[0].u = 0.0f;   vertex[0].v = 0.0f;
	//vertex[0].u = (float)rect.left/GetWidth();
	//vertex[0].v = (float)rect.top/GetHeight();
    vertex[0].u = ( pTextureUV != NULL ) ? pTextureUV->leftTop.x/GetWidth() : 0.f;
    vertex[0].v = ( pTextureUV != NULL ) ? pTextureUV->leftTop.y/GetHeight() : 0.f;
	vertex[0].z = 1.0f;   vertex[0].rhw = 1.0f;


	vertex[1].x = X1;     vertex[1].y = Y2;
	//vertex[1].u = 0.0f;   vertex[1].v = VMax;
	//vertex[1].u = (float)rect.left/GetWidth();
	//vertex[1].v = (float)rect.bottom/GetHeight();
    vertex[1].u = ( pTextureUV != NULL ) ? pTextureUV->leftBottom.x/GetWidth() : 0.f;
    vertex[1].v = ( pTextureUV != NULL ) ? pTextureUV->leftBottom.y/GetHeight() : 1.f;
	vertex[1].z = 1.0f;   vertex[1].rhw = 1.0f;

	vertex[2].x = X2;     vertex[2].y = Y1;
	//vertex[2].u = UMax;   vertex[2].v = 0.0f;
	//vertex[2].u = (float)rect.right/GetWidth();
	//vertex[2].v = (float)rect.top/GetHeight();
    vertex[2].u = ( pTextureUV != NULL ) ? pTextureUV->rightTop.x/GetWidth() : 1.f;
    vertex[2].v = ( pTextureUV != NULL ) ? pTextureUV->rightTop.y/GetHeight() : 0.f;
	vertex[2].z = 1.0f;   vertex[2].rhw = 1.0f;

	vertex[3].x = X2;     vertex[3].y = Y2;
	//vertex[3].u = UMax;   vertex[3].v = VMax;
	//vertex[3].u = (float)rect.right/GetWidth();
	//vertex[3].v = (float)rect.bottom/GetHeight();
    vertex[3].u = ( pTextureUV != NULL ) ? pTextureUV->rightBottom.x/GetWidth() : 1.f;
    vertex[3].v = ( pTextureUV != NULL ) ? pTextureUV->rightBottom.y/GetHeight() : 1.f;
	vertex[3].z = 1.0f;   vertex[3].rhw = 1.0f;

	vertex[0].color = vertex[1].color = 
		vertex[2].color = vertex[3].color = color;

	SetDeviceTexture();

//#ifdef DYNAMIC_VERTEX_BUFFER_OPT
	BOOST_STATIC_ASSERT( D3DFVF_TEXTURE_RHW_VERTEX == D3DFVF_XYZRHW_DIFFUSE_TEX1 );
	g_pKTDXApp->GetDVBManager()->DrawPrimitive( CKTDGDynamicVBManager::DVB_TYPE_XYZRHW_DIFFUSE_TEX1, D3DPT_TRIANGLESTRIP, 2, vertex );
//#else
//	g_pKTDXApp->GetDevice()->SetFVF( D3DFVF_TEXTURE_RHW_VERTEX );
//	g_pKTDXApp->GetDevice()->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, vertex, sizeof(TEXTURE_RHW_VERTEX) );
//#endif
    KD3DEND()
}

void CKTDXDeviceTexture::Draw( float fX, float fY, float fWidth, float fHeight, int nTexU, int nTexV, int nTexWidth, int nTexHeight, D3DCOLOR color /* = 0xffffffff  */ )
{
    KD3DPUSH( g_pKTDXApp->GetDGManager()->GetRSICKTDXDeviceTexture() )

	D3DXVECTOR2 pos		= g_pKTDXApp->ConvertByResolution( fX, fY );
	D3DXVECTOR2 size	= g_pKTDXApp->ConvertByResolution( fWidth, fHeight );



	float X1 = (float)pos.x - 0.5f;
	float Y1 = (float)pos.y - 0.5f;
	float X2 = (float)(pos.x + size.x);
	float Y2 = (float)(pos.y + size.y);

	float UMin = (float)nTexU / (float)GetWidth();
	float VMin = (float)nTexV / (float)GetHeight();
	float UMax = (float)(nTexU+nTexWidth) / (float)GetWidth();
	float VMax = (float)(nTexV+nTexHeight) / (float)GetHeight();

	TEXTURE_RHW_VERTEX vertex[4];
	ZeroMemory( vertex, sizeof(TEXTURE_RHW_VERTEX) * 4 );

	//Set up the 4 corners of a small square
	vertex[0].x = X1;     vertex[0].y = Y1;
	vertex[0].z = 1.0f;   vertex[0].rhw = 1.0f;
	vertex[0].u = UMin;   vertex[0].v = VMin;

	vertex[1].x = X1;     vertex[1].y = Y2;
	vertex[1].z = 1.0f;   vertex[1].rhw = 1.0f;
	vertex[1].u = UMin;   vertex[1].v = VMax;

	vertex[2].x = X2;     vertex[2].y = Y1;
	vertex[2].z = 1.0f;   vertex[2].rhw = 1.0f;
	vertex[2].u = UMax;   vertex[2].v = VMin;

	vertex[3].x = X2;     vertex[3].y = Y2;
	vertex[3].z = 1.0f;   vertex[3].rhw = 1.0f;
	vertex[3].u = UMax;   vertex[3].v = VMax;

	vertex[0].color = vertex[1].color = vertex[2].color = vertex[3].color = color;

	SetDeviceTexture();

//#ifdef DYNAMIC_VERTEX_BUFFER_OPT
	BOOST_STATIC_ASSERT( D3DFVF_TEXTURE_RHW_VERTEX == D3DFVF_XYZRHW_DIFFUSE_TEX1 );
	g_pKTDXApp->GetDVBManager()->DrawPrimitive( CKTDGDynamicVBManager::DVB_TYPE_XYZRHW_DIFFUSE_TEX1, D3DPT_TRIANGLESTRIP, 2, vertex );
//#else
//	g_pKTDXApp->GetDevice()->SetFVF( D3DFVF_TEXTURE_RHW_VERTEX );
//	g_pKTDXApp->GetDevice()->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, vertex, sizeof(TEXTURE_RHW_VERTEX) );
//#endif
    KD3DEND()
}

#ifndef X2OPTIMIZE_TET_XET_PREPROCESSING

void CKTDXDeviceTexture::TETProxy::AddRect_LUA( const char* pKey, D3DXVECTOR2 leftTop, D3DXVECTOR2 rightTop /* = D3DXVECTOR2( -1, -1 ) */,
									D3DXVECTOR2 leftBottom /* = D3DXVECTOR2( -1, -1 ) */, D3DXVECTOR2 rightBottom )
{
	if ( pKey == NULL || pKey[0] == NULL )
		return;

	wstring wstrKeyName;
	ConvertUtf8ToWCHAR( wstrKeyName, pKey );

	KeyTexUVMap::iterator iter = m_mapTexUVRect.find( wstrKeyName );
	if ( iter != m_mapTexUVRect.end() )
	{
		WCHAR buff[256];
		StringCchPrintf( buff, ARRAY_SIZE( buff ), L"redundant key : %s\nKey : %s", m_DeviceID.c_str(), wstrKeyName.c_str() );
		StateLog( buff );
		ASSERT(false);
		return;
	}

	CKTDXDeviceTexture::TEXTURE_UV* pTexUV = new CKTDXDeviceTexture::TEXTURE_UV();
	if( pTexUV != NULL )
	{
		pTexUV->leftTop	= leftTop;
		pTexUV->rightBottom = rightBottom;
		
		if ( leftBottom.x == -1  && leftBottom.y == -1 )
		{
			pTexUV->leftBottom.x = leftTop.x;
			pTexUV->leftBottom.y = rightBottom.y;
		}
		else
		{
			pTexUV->leftBottom = leftBottom;
		}

		if ( rightTop.x == -1 && rightTop.y == -1 )
		{
			pTexUV->rightTop.x = rightBottom.x;
			pTexUV->rightTop.y = leftTop.y;
		}
		else
		{
			pTexUV->rightTop = rightTop;
		}

		pTexUV->rectSize.x = abs( pTexUV->leftTop.x - pTexUV->rightTop.x );
		pTexUV->rectSize.y = abs( pTexUV->leftTop.y - pTexUV->leftBottom.y );
	}

	MakeUpperCase(wstrKeyName);

	m_mapTexUVRect.insert( KeyTexUVMap::value_type( wstrKeyName, pTexUV ) );
}

void CKTDXDeviceTexture::TETProxy::RotateRect_LUA( const char* pKey, int roateValue )
{
//#ifdef DYNAMIC_VERTEX_BUFFER_OPT
	if ( pKey == NULL || pKey[0] == NULL )
		return;
//#endif

	wstring wstrKeyName;
	ConvertUtf8ToWCHAR( wstrKeyName, pKey );
	MakeUpperCase(wstrKeyName);
	
	KeyTexUVMap::iterator i = m_mapTexUVRect.find( wstrKeyName );
	if ( i != m_mapTexUVRect.end() )
	{
		TEXTURE_UV* pTEXTURE_UV = i->second;
		pTEXTURE_UV->rotateValue = roateValue;

		if ( pTEXTURE_UV != NULL )
		{	
			switch ( roateValue )
			{
			case 90:
				{
					D3DXVECTOR2 tempPoint = pTEXTURE_UV->leftTop;
					D3DXVECTOR2 tempPoint2 = pTEXTURE_UV->rightTop;

					pTEXTURE_UV->leftTop = pTEXTURE_UV->leftBottom;
					pTEXTURE_UV->leftBottom = pTEXTURE_UV->rightBottom;
					pTEXTURE_UV->rightTop = tempPoint;
					pTEXTURE_UV->rightBottom = tempPoint2;

					float tempValue;
					tempValue = pTEXTURE_UV->rectSize.y;
					pTEXTURE_UV->rectSize.y = pTEXTURE_UV->rectSize.x;
					pTEXTURE_UV->rectSize.x = tempValue;

				}
				break;

			case 180:
				{
					D3DXVECTOR2 tempPoint = pTEXTURE_UV->leftTop;
					D3DXVECTOR2 tempPoint2 = pTEXTURE_UV->leftBottom;

					pTEXTURE_UV->leftTop = pTEXTURE_UV->rightBottom;
					pTEXTURE_UV->leftBottom = pTEXTURE_UV->rightTop;
					pTEXTURE_UV->rightTop = tempPoint2;
					pTEXTURE_UV->rightBottom = tempPoint;
				}
				break;

			case 270:
				{
					D3DXVECTOR2 tempPoint = pTEXTURE_UV->leftTop;
					D3DXVECTOR2 tempPoint2 = pTEXTURE_UV->leftBottom;

					pTEXTURE_UV->leftTop = pTEXTURE_UV->rightTop;
					pTEXTURE_UV->leftBottom = tempPoint;
					pTEXTURE_UV->rightTop = pTEXTURE_UV->rightBottom;
					pTEXTURE_UV->rightBottom = tempPoint2;

					float tempValue;
					tempValue = pTEXTURE_UV->rectSize.y;
					pTEXTURE_UV->rectSize.y = pTEXTURE_UV->rectSize.x;
					pTEXTURE_UV->rectSize.x = tempValue;
				}
				break;
			}
		}
		

	}
}

CKTDXDeviceTexture::TETProxy::~TETProxy()
{
	// TETProxy
	KeyTexUVMap::iterator i;
	for ( i = m_mapTexUVRect.begin(); i != m_mapTexUVRect.end(); i++ )
	{
		TEXTURE_UV* pTextrueUV = i->second;
		SAFE_DELETE( pTextrueUV );
	}
	m_mapTexUVRect.clear();
}

#endif  X2OPTIMIZE_TET_XET_PREPROCESSING

// bool CKTDXDeviceTexture::LoadTET( const WCHAR* pFileName )
// {
//     //{{ seojt // 2008-10-20, 16:45
//     KGCMassFileManager::CMassFile::MASSFILE_MEMBERFILEINFO_POINTER Info;
// 	Info = g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->LoadDataFile( pFileName );
// 	if( Info == NULL )
// 	{
// 		return false;
// 	}
// 
// 	lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(),  "m_pTEX", this );
// 
// 	if( g_pKTDXApp->GetLuaBinder()->DoMemoryNotEncrypt( Info->pRealData, Info->size ) == false )
// 	{
// 		string strFileName;
// 		ConvertWCHARToChar( strFileName, pFileName );
// 		ErrorLogMsg( KEM_ERROR95, strFileName.c_str() );
// 		return false;
// 	}
// 
// 	return true;
// }//CKTDXDeviceTexture::LoadTET()

