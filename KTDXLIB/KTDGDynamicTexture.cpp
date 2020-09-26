#include "StdAfx.h"
#include ".\ktdgdynamictexture.h"


CKTDGDynamicTexture::CKTDGDynamicTexture(void)
{
	m_d3dFormat = D3DFMT_A8R8G8B8;
	m_dwWidth = 0;
	m_dwHeight = 0;
	m_bUpdateTexture = false;

	m_pTextureSys = NULL;
	m_pTextureVid = NULL;

	m_LockedRect.Pitch = 0;
	m_LockedRect.pBits = NULL;
}

CKTDGDynamicTexture::~CKTDGDynamicTexture(void)
{
	if ( m_pTextureSys != NULL )
		m_pTextureSys->UnlockRect( 0 );
	SAFE_RELEASE(m_pTextureSys);
	SAFE_RELEASE(m_pTextureVid);
}

bool CKTDGDynamicTexture::CreateTexture(DWORD dwWidth, DWORD dwHeight, D3DFORMAT d3dFormat)
{
	m_dwWidth  = dwWidth;
	m_dwHeight = dwHeight;
	m_d3dFormat = d3dFormat;

	HRESULT hr = S_OK;

	if( FAILED(hr = D3DXCreateTexture( g_pKTDXApp->GetDevice(), m_dwWidth, m_dwHeight, 1, NULL, d3dFormat, D3DPOOL_SYSTEMMEM, &m_pTextureSys ) ) )
	{	
		switch( hr )
		{
			case D3DERR_NOTAVAILABLE:
				{
					ErrorLogMsg( KEM_ERROR9, L"D3DERR_NOTAVAILABLE" );
				}
				break;
			case D3DERR_OUTOFVIDEOMEMORY:
				{
					ErrorLogMsg( KEM_ERROR9, L"D3DERR_OUTOFVIDEOMEMORY" );
				}
				break;
			case D3DERR_INVALIDCALL:
				{
					ErrorLogMsg( KEM_ERROR9, L"D3DERR_INVALIDCALL" );
				}
				break;
			case E_OUTOFMEMORY:
				{
					ErrorLogMsg( KEM_ERROR9, L"E_OUTOFMEMORY" );
				}
				break;
			default:
				{
					ErrorLog( KEM_ERROR9 );
				}
				break;
		}
		
		return false;
	}

#ifdef DXUT_KTDXLIB_SAFE_ON_RESET
	if ( DXUTGetDeviceObjectsReset() == true )
#endif
	{
		if( FAILED( hr = D3DXCreateTexture( g_pKTDXApp->GetDevice(), m_dwWidth, m_dwHeight, 1, NULL, d3dFormat, D3DPOOL_DEFAULT, &m_pTextureVid ) ) )
		{
			switch( hr )
			{
				case D3DERR_NOTAVAILABLE:
					{
						ErrorLogMsg( KEM_ERROR114, L"D3DERR_NOTAVAILABLE" );
					}
					break;
				case D3DERR_OUTOFVIDEOMEMORY:
					{
						ErrorLogMsg( KEM_ERROR114, L"D3DERR_OUTOFVIDEOMEMORY" );
					}
					break;
				case D3DERR_INVALIDCALL:
					{
						ErrorLogMsg( KEM_ERROR114, L"D3DERR_INVALIDCALL" );
					}
					break;
				case E_OUTOFMEMORY:
					{
						ErrorLogMsg( KEM_ERROR114, L"E_OUTOFMEMORY" );
					}
					break;
				default:
					{
						ErrorLog( KEM_ERROR114 );
					}
					break;
			}
			//ErrorLog( KEM_ERROR10 );
			return false;
		}
	}

//#ifdef DYNAMIC_VERTEX_BUFFER_OPT
	m_bUpdateTexture = false;
	m_pTextureSys->LockRect(0,&m_LockedRect, NULL, D3DLOCK_NO_DIRTY_UPDATE );
//#else
//	m_pTextureSys->LockRect(0,&m_LockedRect, NULL, 0);
//#endif
	return true;
}

void CKTDGDynamicTexture::ResetDevice()
{
	HRESULT hr;
	SAFE_RELEASE(m_pTextureVid);
	if( FAILED( hr = D3DXCreateTexture( g_pKTDXApp->GetDevice(), m_dwWidth, m_dwHeight, 1, NULL, m_d3dFormat, D3DPOOL_DEFAULT, &m_pTextureVid ) ) )
	{
		switch( hr )
		{
		case D3DERR_NOTAVAILABLE:
			{
				ErrorLogMsg( KEM_ERROR11, L"D3DERR_NOTAVAILABLE" );
			}
			break;
		case D3DERR_OUTOFVIDEOMEMORY:
			{
				ErrorLogMsg( KEM_ERROR11, L"D3DERR_OUTOFVIDEOMEMORY" );
			}
			break;
		case D3DERR_INVALIDCALL:
			{
				ErrorLogMsg( KEM_ERROR11, L"D3DERR_INVALIDCALL" );
			}
			break;
		case E_OUTOFMEMORY:
			{
				ErrorLogMsg( KEM_ERROR11, L"E_OUTOFMEMORY" );
			}
			break;
		default:
			{
				ErrorLog( KEM_ERROR11 );
			}
			break;
		}

		//이것조차 실패하면 할말 없음
		return;
	}

//#ifdef DYNAMIC_VERTEX_BUFFER_OPT
	if ( m_pTextureSys == NULL )
		return;
//#endif 
	m_pTextureSys->UnlockRect(0);

//#ifdef DYNAMIC_VERTEX_BUFFER_OPT
	m_pTextureSys->AddDirtyRect( NULL );
//#endif
	if( FAILED( g_pKTDXApp->GetDevice()->UpdateTexture( m_pTextureSys, m_pTextureVid ) ) )
	{
		ErrorLogMsg( KEM_ERROR12, L"D3DERR_INVALIDCALL." );
	}
//#ifdef DYNAMIC_VERTEX_BUFFER_OPT
	m_bUpdateTexture = false;
	m_pTextureSys->LockRect(0,&m_LockedRect, NULL, D3DLOCK_NO_DIRTY_UPDATE );
//#else
//	m_pTextureSys->LockRect(0,&m_LockedRect, NULL, 0);
//#endif
}

void CKTDGDynamicTexture::LostDevice()
{
	SAFE_RELEASE(m_pTextureVid);
}

void CKTDGDynamicTexture::SetTexture( int iStage )
{
	if( m_bUpdateTexture )
	{
//#ifdef DYNAMIC_VERTEX_BUFFER_OPT
		m_bUpdateTexture = false;
//#endif
		m_pTextureSys->UnlockRect(0);
		g_pKTDXApp->GetDevice()->UpdateTexture( m_pTextureSys, m_pTextureVid );
//#ifdef DYNAMIC_VERTEX_BUFFER_OPT
		m_pTextureSys->LockRect(0,&m_LockedRect, NULL, D3DLOCK_NO_DIRTY_UPDATE );
//#else
//		m_pTextureSys->LockRect(0,&m_LockedRect, NULL, 0);
//#endif
	}	
//{{ robobeg : 2008-10-24
	//if( FAILED( g_pKTDXApp->GetDevice()->SetTexture(iStage, m_pTextureVid ) ) )
	//{
	//	ErrorLog( KEM_ERROR13 );
	//}
	//g_pKTDXApp->SetBeforeTexture( iStage, m_pTextureVid );
    if ( FAILED( CKTDGStateManager::SetTexture( iStage, m_pTextureVid ) ) )
    {
	    ErrorLog( KEM_ERROR13 );
    }//if
//}} robobeg : 2008-10-24
}

//#ifdef DYNAMIC_VERTEX_BUFFER_OPT
void CKTDGDynamicTexture::SetChange( const RECT& rect )
{
	if ( rect.right <= rect.left
		|| rect.bottom <= rect.top )
		return;

	if ( m_pTextureSys != NULL )
	{
		m_pTextureSys->AddDirtyRect( &rect );
		m_bUpdateTexture = true;
	}//if
}
//#endif