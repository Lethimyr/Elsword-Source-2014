// UncompressDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "X2KomFileViewer.h"
#include "X2KomFileManager.h"
#include "UncompressDlg.h"
#include ".\uncompressdlg.h"


// CUncompressDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CUncompressDlg, CDialog)
CUncompressDlg::CUncompressDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUncompressDlg::IDD, pParent)
{
}

CUncompressDlg::~CUncompressDlg()
{
}

void CUncompressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_KOM_LIST, m_komList);
}


BEGIN_MESSAGE_MAP(CUncompressDlg, CDialog)
	ON_BN_CLICKED(IDSEL_FILE, OnBnClickedFile)
	ON_BN_CLICKED(IDSEL_KOM, OnBnClickedKom)
	ON_BN_CLICKED(ID_ALL_KOM, OnBnClickedAllKom)
	ON_BN_CLICKED(ID_ONE_KOM, OnBnClickedOneKom)
END_MESSAGE_MAP()


// CUncompressDlg 메시지 처리기입니다.
BOOL CUncompressDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 초기화
	std::map< std::wstring, std::wstring >::const_iterator mit;
	for( mit = m_mapKomList.begin(); mit != m_mapKomList.end(); ++mit )
	{
		m_komList.InsertString( 0, mit->first.c_str() );
	}

	return TRUE;
}
void CUncompressDlg::OnBnClickedFile()	// 선택된 리소스파일만 압축해제
{
	m_eMode = UM_SEL_FILE;
	EndDialog( 0 );
}

void CUncompressDlg::OnBnClickedKom()
{	
	// 선택한 kom만 압축풀기
	std::wstring wstrCurKomName;
	m_vecSelectedFileList.clear();
	int iSelCount = m_komList.GetSelCount();

	if( iSelCount == 0 )
	{
		MessageBox( L"압축해제할 파일을 선택하세요!" );
		return;
	}
	else
	{
		int* pArray = new int[iSelCount];
		m_komList.GetSelItems( iSelCount, pArray );		// pArray에 선택된 아이템 번호들을 얻음

		for( int iIndex = 0; iIndex < iSelCount; ++iIndex )
		{			
			CString strTemp;
			m_komList.GetText( pArray[iIndex], strTemp );
			
			std::wstring wstrTemp = strTemp.GetBuffer();
			m_vecSelectedFileList.push_back( wstrTemp );
		}
		delete []pArray;
	}

	m_eMode = UM_SEL_KOM;
	EndDialog( 0 );	
}

void CUncompressDlg::OnBnClickedAllKom()
{
	m_eMode = UM_ALL_KOM;
	EndDialog( 0 );	
}

void CUncompressDlg::OnBnClickedOneKom()
{
	m_eMode = UM_ONE_KOM;
	EndDialog( 0 );	
}
