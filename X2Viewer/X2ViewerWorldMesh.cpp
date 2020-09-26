#include "dxstdafx.h"
#include ".\x2viewerworldmesh.h"

CX2ViewerWorldMesh::CX2ViewerWorldMesh(void)
{
	CX2ViewerObject::SetObjectStyle( CX2ViewerObject::OS_WORLD_MESH );

	m_pXMesh	= NULL;
	m_pMatrix	= new CKTDGMatrix( g_pKTDXApp->GetDevice() );
}

CX2ViewerWorldMesh::~CX2ViewerWorldMesh(void)
{
	ReleaseMesh();
	SAFE_DELETE( m_pMatrix );
}

HRESULT CX2ViewerWorldMesh::OnFrameMove( double fTime, float fElapsedTime )
{
	return S_OK;
}

HRESULT CX2ViewerWorldMesh::OnFrameRender()
{
	if( m_pXMesh == NULL )
		return S_OK;

	m_pMatrix->UpdateWorldMatrix();
	m_pXMesh->Render();

	return S_OK;
}

HRESULT CX2ViewerWorldMesh::OnResetDevice()
{
	return S_OK;
}

HRESULT CX2ViewerWorldMesh::OnLostDevice()
{
	return S_OK;
}


bool CX2ViewerWorldMesh::SetMesh( const WCHAR* wstrPullName )
{
	WCHAR drive[10] = L"";
	WCHAR dir[256]   = L"";
	WCHAR fname[256] = L"";
	WCHAR ext[10]    = L"";

	WCHAR fileName[256] = L"";
	WCHAR PullDir[256] = L"";
	_wsplitpath( wstrPullName, drive, dir, fname, ext);

	wcscat( fileName, fname);
	wcscat( fileName, ext);

	wcscat( PullDir, drive );
	wcscat( PullDir, dir );

	ReleaseMesh();
	m_pXMesh	= g_pKTDXApp->GetDeviceManager()->OpenXMesh( fileName );

	if( m_pXMesh == NULL )
	{
		std::string strDir;
		ConvertWCHARToChar( strDir, PullDir );
		g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->AddDataDirectory( strDir );

		m_pXMesh = g_pKTDXApp->GetDeviceManager()->OpenXMesh( fileName );

		if( m_pXMesh == NULL )
		{
			WARNINGMSG( L"World Mesh 로딩실패(CX2ViewerWorldMesh::SetMesh())" );
			return false;
		}
	}

	return true;
}


void CX2ViewerWorldMesh::ReleaseMesh()
{
	if( m_pXMesh == NULL )
		return;

	m_pMatrix->Move( 0.0f, 0.0f, 0.0f );
	m_pMatrix->Rotate( 0.0f, 0.0f, 0.0f );
	m_pMatrix->Scale( 1.0f, 1.0f, 1.0f );

	g_pKTDXApp->GetDeviceManager()->CloseDevice( m_pXMesh->GetDeviceID() );
	m_pXMesh	= NULL;
}