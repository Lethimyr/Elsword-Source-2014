#include "dxstdafx.h"
#include ".\fcsort.h"

CFCSort::CFCSort(void)
{
	m_pMesh = NULL;
}

CFCSort::~CFCSort(void)
{
	SAFE_CLOSE( m_pMesh );
	m_TempList.clear();
}

bool CFCSort::SortMesh( const WCHAR* pFileName )
{
	m_pMesh = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( pFileName );
	if( m_pMesh == NULL )
		return false;

	while( SortFrameIn( m_pMesh->GetFrameRoot(), NULL ) == E_FAIL )
	{
	}

	return true;
}

void CFCSort::SaveMesh( const WCHAR* pFileName, bool bExpendName )
{
	if( m_pMesh == NULL )
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
			WCHAR special[_MAX_FNAME] = L"_Sorted";

			_wsplitpath( lpstrFile, drive, dir, fname, ext);
			wcscat(fname, special);

			wcscat(FullPath, drive);
			wcscat(FullPath, dir);
			wcscat(FullPath, fname);
			wcscat(FullPath, ext);
		}

		LPD3DXANIMATIONCONTROLLER pAC = m_pMesh->GetCloneAC();

		if( strcmp( m_pMesh->GetFrameRoot()->Name, "<no_name>" ) == 0 )
		{
			hr = D3DXSaveMeshHierarchyToFile(FullPath,
				xFormat,
				(LPD3DXFRAME) m_pMesh->GetFrameRoot()->pFrameFirstChild,
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
				(LPD3DXFRAME) m_pMesh->GetFrameRoot(),
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

void CFCSort::SaveMeshAuto( const WCHAR* pPath, const WCHAR* pFileName, bool bExpendName )
{
	HRESULT			hr = S_OK;
	WCHAR drive[_MAX_DRIVE];
	WCHAR dir[_MAX_DIR];
	WCHAR fname[_MAX_FNAME];
	WCHAR ext[_MAX_EXT];
	WCHAR special[_MAX_FNAME] = L"_Sorted";

	DWORD xFormat = D3DXF_FILEFORMAT_COMPRESSED;

	wstring tempFullPath = pPath;
	tempFullPath += L'\\';
	tempFullPath += pFileName;

	if ( bExpendName == true )
	{
		_wsplitpath( tempFullPath.c_str(), drive, dir, fname, ext);

		tempFullPath = drive;
		tempFullPath += dir;
		tempFullPath += fname;
		tempFullPath += special;
		tempFullPath += ext;
	}

	LPD3DXANIMATIONCONTROLLER pAC = m_pMesh->GetCloneAC();

	if( strcmp( m_pMesh->GetFrameRoot()->Name, "<no_name>" ) == 0 )
	{
		hr = D3DXSaveMeshHierarchyToFile(tempFullPath.c_str(),
			xFormat,
			(LPD3DXFRAME) m_pMesh->GetFrameRoot()->pFrameFirstChild,
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
		hr = D3DXSaveMeshHierarchyToFile(tempFullPath.c_str(),
			xFormat,
			(LPD3DXFRAME) m_pMesh->GetFrameRoot(),
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

HRESULT CFCSort::SortFrameIn( CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame, CKTDXDeviceXSkinMesh::MultiAnimFrame* pParentFrame )
{
	HRESULT hr = S_OK;

	m_TempList.push_back( pFrame );
	if( pFrame->pFrameSibling )
	{
		// recursive call		
		hr = SortFrameIn( (CKTDXDeviceXSkinMesh::MultiAnimFrame *) pFrame->pFrameSibling, pParentFrame );
		if(FAILED( hr ) )
			return hr;
	}

	if( pFrame->pFrameFirstChild )
	{
		if( ListSort( pParentFrame ) == true )
			return E_FAIL;
		// recursive call
		hr = SortFrameIn( (CKTDXDeviceXSkinMesh::MultiAnimFrame *) pFrame->pFrameFirstChild, (CKTDXDeviceXSkinMesh::MultiAnimFrame *) pFrame );
		if(FAILED( hr ) )
			return hr;
	}

	return hr;
}


bool CFCSort::ListSort( CKTDXDeviceXSkinMesh::MultiAnimFrame* pParentFrame)
{
	vector<CKTDXDeviceXSkinMesh::MultiAnimFrame*> m_TempListOrg;
	for( int i = 0; i < (int)m_TempList.size(); i++ )
	{
		m_TempListOrg.push_back(m_TempList[i]);
	}

	std::sort( m_TempList.begin(), m_TempList.end(), CListSort() );

	bool bChange = false;
	for( int i = 0; i < (int)m_TempList.size(); i++ )
	{
		if( m_TempListOrg[i] != m_TempList[i] )
		{
			bChange = true;
			break;
		}
	}

	if( bChange == false )
	{
		m_TempList.clear();
		return false;
	}

	for( int i = 0; i < (int)m_TempList.size()-1; i++ )
	{
		CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame1 = m_TempList[i];
		CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame2 = m_TempList[i+1];

		pFrame1->pFrameSibling = (D3DXFRAME*)pFrame2;
		pFrame2->pFrameSibling = NULL;
		if( i == 0 && pParentFrame != NULL )
		{
			pParentFrame->pFrameFirstChild = (D3DXFRAME*)pFrame1;
		}
	}
	m_TempList.clear();

	return true;
}


