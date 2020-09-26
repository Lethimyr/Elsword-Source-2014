#include "dxstdafx.h"
#include ".\frameremove.h"

CFrameRemove::CFrameRemove(void)
{
	m_pXSkinMesh = NULL;
}

CFrameRemove::~CFrameRemove(void)
{
	SAFE_CLOSE( m_pXSkinMesh );
}

void CFrameRemove::SetFile( wstring fileName )
{
	SAFE_CLOSE( m_pXSkinMesh );
	m_pXSkinMesh = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( fileName );
}

bool CFrameRemove::RemoveFrameName( wstring frameName )
{
	if( m_pXSkinMesh == NULL )
		return false;

	vector<wstring> frameNameList;
	frameNameList.push_back( frameName );
	RemoveFrameIn( m_pXSkinMesh->GetFrameRoot(), frameNameList );
	return true;
}

bool CFrameRemove::RemoveFrameName( vector<wstring> frameNameList )
{
	if( m_pXSkinMesh == NULL )
		return false;

	RemoveFrameIn( m_pXSkinMesh->GetFrameRoot(), frameNameList );
	return true;
}

HRESULT CFrameRemove::RemoveFrameIn( CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame, vector<wstring>& frameNameList )
{
	HRESULT hr;

	if( pFrame->pFrameSibling != NULL )
	{
		wstring temp;
		ConvertCharToWCHAR( temp, pFrame->pFrameSibling->Name );
		bool bRecheck = false;
		do
		{
			bRecheck = false;
			for( int i = 0; i < (int)frameNameList.size(); i++ )
			{
				wstring removeFrameName = frameNameList[i];
				if( removeFrameName.compare( temp ) == 0 && pFrame->pFrameSibling != NULL )
				{
					pFrame->pFrameSibling = pFrame->pFrameSibling->pFrameSibling;
					bRecheck = true;
					ConvertCharToWCHAR( temp, pFrame->pFrameSibling->Name );
					//return S_OK;
				}
			}
		}
		while( bRecheck == true );
		

		// recursive call
		hr = RemoveFrameIn( (CKTDXDeviceXSkinMesh::MultiAnimFrame *) pFrame->pFrameSibling, frameNameList );
		if(FAILED( hr ) )
			return hr;
	}

	if( pFrame->pFrameFirstChild != NULL )
	{
		wstring temp;
		ConvertCharToWCHAR( temp, pFrame->pFrameFirstChild->Name );
		bool bRecheck = false;
		do
		{
			bRecheck = false;
			for( int i = 0; i < (int)frameNameList.size(); i++ )
			{
				wstring removeFrameName = frameNameList[i];
				if( removeFrameName.compare( temp ) == 0 && pFrame->pFrameFirstChild != NULL )
				{
					pFrame->pFrameFirstChild = pFrame->pFrameFirstChild->pFrameSibling;
					bRecheck = true;
					ConvertCharToWCHAR( temp, pFrame->pFrameFirstChild->Name );
					//return S_OK;
				}
			}
		}
		while( bRecheck == true );

		// recursive call
		hr = RemoveFrameIn( (CKTDXDeviceXSkinMesh::MultiAnimFrame *) pFrame->pFrameFirstChild, frameNameList );
		if(FAILED( hr ) )
			return hr;
	}

	return S_OK;
}

void CFrameRemove::SaveMesh( const WCHAR* pFileName, bool bExpendName )
{
	if( m_pXSkinMesh == NULL )
		return;

	HRESULT			hr = S_OK;
	LPD3DXMESH		pMeshMerged = NULL;
	LPD3DXBUFFER	pbufAdjacencyMerged = NULL;
	LPD3DXBUFFER	pbufMaterialsMerged = NULL;

	BOOL bSaveHierarchy = TRUE;
	DWORD xFormat = D3DXF_FILEFORMAT_COMPRESSED;

	OPENFILENAME m_OFN;

	WCHAR lpstrFile[255] = {0,};

	wcscpy(lpstrFile, pFileName);

	memset(&m_OFN, 0, sizeof(OPENFILENAME) );
	m_OFN.lStructSize = sizeof( OPENFILENAME );
	m_OFN.hwndOwner = g_pKTDXApp->GetHWND();
	m_OFN.lpstrFilter = L"x-file(*.x)\0*.*\0";
	m_OFN.lpstrFile = lpstrFile;
	m_OFN.nMaxFile = 255;


	if ( GetSaveFileName( &m_OFN ) != 0 )
	{
		WCHAR FullPath[MAX_PATH] = L"";
		memset( FullPath, 0, sizeof(FullPath));
		wsprintf( FullPath, lpstrFile );

		if ( bExpendName == true )
		{
			memset(FullPath, 0, sizeof(FullPath));

			WCHAR drive[_MAX_DRIVE];
			WCHAR dir[_MAX_DIR];
			WCHAR fname[_MAX_FNAME];
			WCHAR ext[_MAX_EXT];
			WCHAR special[_MAX_FNAME] = L"_RM";

			_wsplitpath( lpstrFile, drive, dir, fname, ext);
			wcscat(fname, special);

			wcscat(FullPath, drive);
			wcscat(FullPath, dir);
			wcscat(FullPath, fname);
			wcscat(FullPath, ext);
		}

		LPD3DXANIMATIONCONTROLLER pAC = m_pXSkinMesh->GetCloneAC();

		if( strcmp( m_pXSkinMesh->GetFrameRoot()->Name, "<no_name>" ) == 0 )
		{
			hr = D3DXSaveMeshHierarchyToFile(FullPath,
				xFormat,
				(LPD3DXFRAME) m_pXSkinMesh->GetFrameRoot()->pFrameFirstChild,
				pAC,
				NULL);

			SAFE_RELEASE( pAC );

			if (FAILED(hr))
			{
				MessageBox( NULL, L"에러", L"세이브 확인", MB_OK);
				return;
			}
		}
		else
		{
			hr = D3DXSaveMeshHierarchyToFile(FullPath,
				xFormat,
				(LPD3DXFRAME) m_pXSkinMesh->GetFrameRoot(),
				pAC,
				NULL);

			SAFE_RELEASE( pAC );

			if (FAILED(hr))
			{
				MessageBox( NULL, L"에러", L"세이브 확인", MB_OK);
				return;
			}
		}

		WCHAR drive[_MAX_DRIVE];
		WCHAR dir[_MAX_DIR];
		WCHAR fname[_MAX_FNAME];
		WCHAR ext[_MAX_EXT];
		_wsplitpath( FullPath, drive, dir, fname, ext);
		wcscat(fname, ext);

		SAFE_CLOSE( m_pXSkinMesh );
		m_pXSkinMesh = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( fname );
		if( m_pXSkinMesh != NULL )
		{
			pAC = m_pXSkinMesh->GetCloneAC();

			if( strcmp( m_pXSkinMesh->GetFrameRoot()->Name, "<no_name>" ) == 0 )
			{
				hr = D3DXSaveMeshHierarchyToFile(FullPath,
					xFormat,
					(LPD3DXFRAME) m_pXSkinMesh->GetFrameRoot()->pFrameFirstChild,
					pAC,
					NULL);

				SAFE_RELEASE( pAC );

				if (FAILED(hr))
				{
					MessageBox( NULL, L"에러", L"세이브 확인", MB_OK);
					return;
				}
			}
			else
			{
				hr = D3DXSaveMeshHierarchyToFile(FullPath,
					xFormat,
					(LPD3DXFRAME) m_pXSkinMesh->GetFrameRoot(),
					pAC,
					NULL);

				SAFE_RELEASE( pAC );

				if (FAILED(hr))
				{
					MessageBox( NULL, L"에러", L"세이브 확인", MB_OK);
					return;
				}
			}
		}
	}	
}








