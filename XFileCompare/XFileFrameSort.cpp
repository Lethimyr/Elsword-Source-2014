#include "dxstdafx.h"
#include ".\xfileframesort.h"
#include <algorithm>

CXFileFrameSort::CXFileFrameSort(void)
{
	m_pSkinMesh = NULL;
	m_pMesh = NULL;
	OpenFile();
	OpenMesh();
	SortFrame();
	SaveFile();
}

CXFileFrameSort::~CXFileFrameSort(void)
{
}

void CXFileFrameSort::OpenFile()
{

	WCHAR wstrFile[256]			= L"";
	WCHAR wstrFileTitle[256]	= L"";
	WCHAR wstrDir[256]			= L"";
	char  strFile[256]			= "";

	ZeroMemory( &m_OFN, sizeof( OPENFILENAME));
	m_wsFileName.clear();

	m_OFN.lStructSize		= sizeof(OPENFILENAME);
	m_OFN.hwndOwner			= DXUTGetHWND();//m_hWnd;
	m_OFN.lpstrFilter		= L"x-file(*.x)\0*.*\0";
	m_OFN.lpstrFile			= wstrFile;
	m_OFN.nMaxFile			= 256;
	m_OFN.lpstrTitle		= L"::: 파일열기 :::";
	m_OFN.lpstrFileTitle	= wstrFileTitle;
	m_OFN.nMaxFileTitle		= 256;
	GetCurrentDirectory( MAX_PATH, wstrDir);
	m_OFN.lpstrInitialDir	= wstrDir;
	m_OFN.Flags				= OFN_PATHMUSTEXIST;

	if( GetOpenFileNameW(&m_OFN) != 0)
	{
		m_wsFileName		= wstrFileTitle;
		m_wsPullFileName	= wstrFile;

		WideCharToMultiByte( CP_ACP, 0, wstrFile, -1, strFile, 256, NULL, NULL);
		m_strPullFileName = strFile;
		WideCharToMultiByte( CP_ACP, 0, wstrFileTitle, -1, strFile, 256, NULL, NULL);
		m_strFileName = strFile;

		return;
	}

	return;
}

void CXFileFrameSort::OpenMesh()
{
	WCHAR Filename[256] = L"";
	WCHAR drive[_MAX_DRIVE];
	WCHAR dir[_MAX_DIR];
	WCHAR fname[_MAX_FNAME];
	WCHAR ext[_MAX_EXT];

	_wsplitpath( m_wsPullFileName.c_str(), drive, dir, fname, ext);
	wcscat( Filename, fname);
	wcscat( Filename, ext);

	m_pSkinMesh = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( m_wsFileName );
	if( m_pSkinMesh == NULL )
	{
		string	strDir;
		char	csTemp[MAX_PATH] = "";

		WideCharToMultiByte( CP_ACP, 0, drive, -1, csTemp, MAX_PATH, NULL, NULL );
		strDir += csTemp;
		WideCharToMultiByte( CP_ACP, 0, dir, -1, csTemp, MAX_PATH, NULL, NULL );
		strDir += csTemp;

		g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->AddDataDirectory( strDir );
		m_pSkinMesh = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( m_wsFileName );
		if( m_pSkinMesh == NULL )
		{
			return;
		}
	}
}

void CXFileFrameSort::SortFrame()
{
	while( SortFrameIn( m_pSkinMesh->GetFrameRoot(), NULL ) == E_FAIL )
	{
	}
}

HRESULT CXFileFrameSort::SortFrameIn( CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame, CKTDXDeviceXSkinMesh::MultiAnimFrame* pParentFrame )
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

bool CXFileFrameSort::ListSort( CKTDXDeviceXSkinMesh::MultiAnimFrame* pParentFrame)
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

void CXFileFrameSort::SaveFile()
{
	HRESULT			hr = S_OK;
	LPD3DXMESH		pMeshMerged = NULL;
	LPD3DXBUFFER	pbufAdjacencyMerged = NULL;
	LPD3DXBUFFER	pbufMaterialsMerged = NULL;

	BOOL bSaveHierarchy = TRUE;
	DWORD xFormat = D3DXF_FILEFORMAT_COMPRESSED;


	m_wsPullFileName += L"_";
	
	// 애니메이션 파일과 함께 저장할대..
	if( bSaveHierarchy && m_pSkinMesh )
	{
		LPD3DXANIMATIONCONTROLLER pAC = m_pSkinMesh->GetCloneAC();

		hr = D3DXSaveMeshHierarchyToFile(m_wsPullFileName.c_str(),
			xFormat,
			(LPD3DXFRAME) m_pSkinMesh->GetFrameRoot(),
			pAC,
			NULL);

		SAFE_RELEASE( pAC );

		if (FAILED(hr))
		{
			//ERRORMSG( L"HierarchyToFile 저장 실패");
			return;
		}
	}
}