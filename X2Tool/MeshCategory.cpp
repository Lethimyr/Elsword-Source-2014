// MeshCategory.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "X2Tool.h"
#include "MeshCategory.h"
#include "afxdialogex.h"


// CMeshCategory 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMeshCategory, CDialogEx)

CMeshCategory::CMeshCategory(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMeshCategory::IDD, pParent)
{
	m_bCanUseLib = false;
}

CMeshCategory::~CMeshCategory()
{
}

void CMeshCategory::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_OBJECT, m_listObject);
	DDX_Control(pDX, IDC_TREE1, m_treeLib);
}

void CMeshCategory::UpdateTree(wstring wstrPath, HTREEITEM hParent)
{
	BOOL bResult = TRUE;
	HANDLE hSrch;
	WIN32_FIND_DATA wfd;
	wstring wstrSearch = wstrPath + L"\\*.*";

	hSrch = FindFirstFile(wstrSearch.c_str(), &wfd);
	while(bResult)
	{
		if( wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
		{
			if( StrStrW(wfd.cFileName, L".svn") == NULL && StrStrW(wfd.cFileName, L".") == NULL && StrStrW(wfd.cFileName, L"..") == NULL )
			{
				HTREEITEM hClass = hClass = m_treeLib.InsertItem( wfd.cFileName, 1, 1, hParent);
				wstring wstrSubPath = wstrPath + L"\\" + wfd.cFileName + L"\\*.*";
				UpdateTree( wstrSubPath, hClass );
			}			
		}
		bResult = FindNextFile(hSrch, &wfd);
	}
	FindClose( hSrch );
}

void CMeshCategory::UpdateLib()
{
	CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;

	if( pMain->m_wstrLibPath.empty() == true )
	{
		AfxMessageBox(L"WorldLibrary Path가 지정되지 않았습니다.\n사용을 원하시면 경로를 지정해 주세요", MB_OK);
		return;
	}

	GetDlgItem(IDC_EDIT_LIBRARY_PATH)->SetWindowText(pMain->m_wstrLibPath.c_str());

	wstring wstrLibPath = pMain->m_wstrLibPath;
	m_bCanUseLib = true;
	string strCommand = "tortoiseproc /command:update /path:";
	string strPath;
	ConvertWCHARToChar(strPath, wstrLibPath.c_str());
	strCommand = strCommand + "\"" + strPath + "\" /closeonend:2";
	system(strCommand.c_str());
	
	m_treeLib.DeleteAllItems();

	HTREEITEM hRoot = m_treeLib.InsertItem(_T("World Library"), 0, 0);
	m_treeLib.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);

	BOOL bResult = TRUE;
	HANDLE hSrch;
	WIN32_FIND_DATA wfd;
	wstring wstrSearch = wstrLibPath + L"\\*.*";

	hSrch = FindFirstFile(wstrSearch.c_str(), &wfd);
	while(bResult)
	{
		if( wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
		{
			if( StrStrW(wfd.cFileName, L".svn") == NULL && StrStrW(wfd.cFileName, L".") == NULL && StrStrW(wfd.cFileName, L"..") == NULL )
			{
				HTREEITEM hClass = hClass = m_treeLib.InsertItem( wfd.cFileName, 1, 1, hRoot);
				wstring wstrSubPath = wstrLibPath + L"\\" + wfd.cFileName;
				UpdateTree( wstrSubPath, hClass );
			}			
		}
		bResult = FindNextFile(hSrch, &wfd);
	}
	FindClose( hSrch );


	m_treeLib.Expand(hRoot, TVE_EXPAND);
	UpdateData(TRUE);		
}

void CMeshCategory::UpdateList(wstring wstrLib)
{
	if( m_bCanUseLib == false )
		return;

	CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
	wstring wstrLibPath = pMain->m_wstrLibPath;
	vector<wstring> vecLibComponent;
	vecLibComponent.clear();

	BOOL bResult = TRUE;
	HANDLE hSrch;
	WIN32_FIND_DATA wfd;
	
	if( wstrLib.empty() == true )
	{
		wstrLibPath = wstrLibPath;
	}
	else
	{
		wstrLibPath = wstrLibPath + L"\\" + wstrLib;
	}

	g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->AddDataDirectory(wstrLibPath);

	wstring wstrSearch = wstrLibPath + L"\\*.*";

	hSrch = FindFirstFile(wstrSearch.c_str(), &wfd);
	while(bResult)
	{
		if( !(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
		{
			wstring wstrFileName = wstrLibPath + L"\\" + wfd.cFileName;
			MakeUpperCase(wstrFileName);
			if( (StrStrW(wstrFileName.c_str(), L".X") != NULL || StrStrW(wstrFileName.c_str(), L".Y") != NULL) &&
				(StrStrW(wstrFileName.c_str(), L".BMP") == NULL) )
			{
				vecLibComponent.push_back(wstrFileName);
			}			
		}
		bResult = FindNextFile(hSrch, &wfd);
	}
	FindClose( hSrch );
	
	static CImageList imgList;
	imgList.Create(170, 128, ILC_COLOR32, 1, 1);

	int iCount = (int)vecLibComponent.size();
	for(int i=0; i<imgList.GetImageCount(); ++i)
		imgList.Remove(i);

	imgList.SetImageCount(iCount);

	for(int i=0; i<iCount; ++i)
	{
		wstring wstrComponent = vecLibComponent[i];
		wstring wstrComponentThumnail = wstrComponent + L".bmp";
		HBITMAP hBitmap = NULL;
		hBitmap = (HBITMAP)LoadImage(NULL, wstrComponentThumnail.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);

		if( hBitmap == NULL )
		{
			pMain->m_vecThumbnail.push_back(wstrComponent);
			wstring wstrxbox = pMain->m_wstrLibPath;
			wstrxbox = wstrxbox + L"\\xbox.bmp";
			hBitmap = (HBITMAP)LoadImage(NULL, wstrxbox.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);			
		}		

		CBitmap bmp;
		bmp.Attach(hBitmap);		
		imgList.Replace(i, &bmp, NULL);	
	}
	m_listObject.SetImageList(&imgList, LVSIL_NORMAL);

	m_listObject.DeleteAllItems();
	for(int i=0; i<iCount; ++i)
	{
		wstring wstrComponent = vecLibComponent[i];
		WCHAR wszFilePathOnly[512] = {NULL};
		WCHAR wszFileNameOnly[256] = {NULL};
		DecomposePathAndFileName( (WCHAR*)wszFilePathOnly, (WCHAR*)wszFileNameOnly, wstrComponent.c_str() );
		
		m_listObject.InsertItem(0, wszFileNameOnly, i);
	}
}

BEGIN_MESSAGE_MAP(CMeshCategory, CDialogEx)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CMeshCategory::OnTvnSelchangedTree1)
	ON_EN_CHANGE(IDC_EDIT_LIBRARY_PATH, &CMeshCategory::OnEnChangeEditLibraryPath)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_OBJECT, &CMeshCategory::OnLvnItemchangedListObject)
END_MESSAGE_MAP()


// CMeshCategory 메시지 처리기입니다.


void CMeshCategory::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	HTREEITEM hTree = m_treeLib.GetSelectedItem();
	CString selectedItem = m_treeLib.GetItemText(hTree);
		
	UpdateList( selectedItem.GetString() );
}


void CMeshCategory::OnEnChangeEditLibraryPath()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString csTemp;
	GetDlgItem(IDC_EDIT_LIBRARY_PATH)->GetWindowText(csTemp);
	
	CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
	pMain->m_wstrLibPath = csTemp.GetString();

	DXUTDeviceSettings deviceSettings	= DXUTGetDeviceSettings();

	int iWidth = deviceSettings.pp.BackBufferWidth;
	int iHeight = deviceSettings.pp.BackBufferHeight;
	
	FILE *fp = fopen("X2ToolConfig.cfg", "w");
	if( fp != NULL )
	{
		fprintf(fp, "%d %d\n", iWidth, iHeight);
		if( pMain->m_bHeightMap == true )
			fprintf(fp, "1\n");
		else
			fprintf(fp, "0\n");

		string strLibPath;
		ConvertWCHARToChar(strLibPath, pMain->m_wstrLibPath.c_str());
		fprintf(fp, "%s\n", strLibPath.c_str());

		fclose(fp);
	}
}


void CMeshCategory::OnLvnItemchangedListObject(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
	if( (pNMLV->uNewState == 2 || pNMLV->uNewState == 3) && (pNMLV->uOldState == 0 || pNMLV->uOldState == 1) )
	{
		// 선택
		CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
		CString csTemp = m_listObject.GetItemText(pNMLV->iItem, 0);

		pMain->m_bAddObjectByLib = true;
		pMain->m_wstrSelectedLib = csTemp.GetString();
	}
	else if( pNMLV->uNewState == 0 && pNMLV->uOldState == 2 )
	{
		// 선택 해제
		pMain->m_bAddObjectByLib = false;
		pMain->m_wstrSelectedLib = L"";
	}
}

void CMeshCategory::ClearSelected()
{
	for(int i=0; i<m_listObject.GetItemCount(); ++i)
		m_listObject.SetItemState(i, 0, LVIS_SELECTED );
}