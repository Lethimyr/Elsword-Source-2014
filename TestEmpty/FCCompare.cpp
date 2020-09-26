#include "dxstdafx.h"
#include ".\fccompare.h"

CFCCompare::CFCCompare(void)
{
	m_bSame			= true;
	m_pFirstMesh	= NULL;
	m_pSecondMesh	= NULL;
}

CFCCompare::~CFCCompare(void)
{
	SAFE_CLOSE( m_pFirstMesh );
	SAFE_CLOSE( m_pSecondMesh );

	m_FirstMeshLog.clear();
	m_SecondMeshLog.clear();
}

void CFCCompare::SetFirstFileName( const WCHAR* pFileName )
{
	m_FirstFileName = pFileName;
}

void CFCCompare::SetSecondFileName( const WCHAR* pFileName )
{
	m_SecondFileName = pFileName;
}

bool CFCCompare::Compare()
{
	m_pFirstMesh = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( m_FirstFileName );
	m_pSecondMesh = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( m_SecondFileName );

	if( m_pFirstMesh == NULL || m_pSecondMesh == NULL )
		return false;

	m_bSame = true;
	CompareHierarchy( m_pFirstMesh->GetFrameRoot(), m_pSecondMesh->GetFrameRoot() );

	return m_bSame;
}

int CFCCompare::GetFirstFrameNum()
{
	if( m_pFirstMesh == NULL )
		return 0;

	return m_pFirstMesh->GetFrameNum();
}

int CFCCompare::GetSecondFrameNum()
{
	if( m_pSecondMesh == NULL )
		return 0;

	return m_pSecondMesh->GetFrameNum();
}

wstring CFCCompare::GetFirstMeshLogByIndex( int index )
{
	wstring TempLog = L"";

	if ( index >= 0 && index < (int)m_FirstMeshLog.size() )
	{
		TempLog = m_FirstMeshLog[index];
	}

	return TempLog;
}

wstring CFCCompare::GetSecondMeshLogByIndex( int index )
{
	wstring TempLog = L"";

	if ( index >= 0 && index < (int)m_SecondMeshLog.size() )
	{
		TempLog = m_SecondMeshLog[index];
	}

	return TempLog;
}

void CFCCompare::CompareHierarchy( CKTDXDeviceXSkinMesh::MultiAnimFrame* p1Frame, CKTDXDeviceXSkinMesh::MultiAnimFrame* p2Frame )
{
	if( m_bSame == false )
		return;

	if( (p1Frame->pFrameSibling != NULL && p2Frame->pFrameSibling == NULL)
		|| (p1Frame->pFrameSibling == NULL && p2Frame->pFrameSibling != NULL) )
	{
		ComputeHierarchyExplain(p1Frame, p2Frame);
		m_bSame = false;
		return;
	}

	if( (p1Frame->pFrameFirstChild != NULL && p2Frame->pFrameFirstChild == NULL)
		|| (p1Frame->pFrameFirstChild == NULL && p2Frame->pFrameFirstChild != NULL) )
	{
		wstring temp;
		ConvertCharToWCHAR( temp, p1Frame->Name );
		m_FirstMeshLog.push_back( temp );
		ConvertCharToWCHAR( temp, p2Frame->Name );
		m_SecondMeshLog.push_back( temp );
		ComputeHierarchyExplain(p1Frame, p2Frame);
		m_bSame = false;
		return;
	}

	if( p1Frame->pFrameSibling != NULL && p2Frame->pFrameSibling != NULL )
	{
		CompareHierarchy( (CKTDXDeviceXSkinMesh::MultiAnimFrame*)p1Frame->pFrameSibling, (CKTDXDeviceXSkinMesh::MultiAnimFrame*)p2Frame->pFrameSibling );
	}

	if( p1Frame->pFrameFirstChild != NULL && p2Frame->pFrameFirstChild != NULL )
	{
		if( m_bSame == true )
		{
			wstring temp;
			ConvertCharToWCHAR( temp, p1Frame->Name );
			m_FirstMeshLog.push_back( temp );
			ConvertCharToWCHAR( temp, p1Frame->Name );
			m_SecondMeshLog.push_back( temp );
		}

		CompareHierarchy( (CKTDXDeviceXSkinMesh::MultiAnimFrame*)p1Frame->pFrameFirstChild, (CKTDXDeviceXSkinMesh::MultiAnimFrame*)p2Frame->pFrameFirstChild );

		if( m_bSame == true )
		{
			m_FirstMeshLog.erase( m_FirstMeshLog.end() - 1 );
			m_SecondMeshLog.erase( m_SecondMeshLog.end() - 1 );
		}
	}
}


void CFCCompare::ComputeHierarchyExplain( CKTDXDeviceXSkinMesh::MultiAnimFrame* p1Frame, CKTDXDeviceXSkinMesh::MultiAnimFrame* p2Frame )
{
	if ( p1Frame->pFrameSibling != NULL )
	{
		m_FirstMeshLog.push_back( L"Sibling 존재" );
	}
	else
	{
		m_FirstMeshLog.push_back( L"Sibling 존재하지 않음" );
	}

	if (p2Frame->pFrameSibling != NULL )
	{
		m_SecondMeshLog.push_back( L"Sibling 존재" );
	}
	else
	{
		m_SecondMeshLog.push_back( L"Sibling 존재하지 않음" );
	}

	if ( p1Frame->pFrameFirstChild != NULL )
	{
		m_FirstMeshLog.push_back( L"Child 존재" );
	}
	else
	{
		m_FirstMeshLog.push_back( L"Child 존재하지 않음" );
	}

	if (p2Frame->pFrameFirstChild != NULL )
	{
		m_SecondMeshLog.push_back( L"Child 존재" );
	}
	else
	{
		m_SecondMeshLog.push_back( L"Child 존재하지 않음" );
	}
}








