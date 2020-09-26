// kelpDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "kelp.h"
#include "kelpDlg.h"
#include ".\kelpdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BOOL g_bThread = false;
CkelpDlg *g_cwnd;
std::vector<CString> m_convertFileList;

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 대화 상자 데이터
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원

// 구현
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CkelpDlg 대화 상자



CkelpDlg::CkelpDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CkelpDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CkelpDlg::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);
   DDX_Control(pDX, IDC_LIST1, m_fileListBox);
}

BEGIN_MESSAGE_MAP(CkelpDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
   ON_LBN_SELCHANGE(IDC_LIST1, OnLbnSelchangeList1)
   ON_LBN_DBLCLK(IDC_LIST1, OnLbnDblclkList1)
   ON_BN_CLICKED(IDOK, OnBnClickedOk)
   ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()


// CkelpDlg 메시지 처리기

BOOL CkelpDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	// 프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	GetDrive();

	return TRUE;  // 컨트롤에 대한 포커스를 설정하지 않을 경우 TRUE를 반환합니다.
}

void CkelpDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면 
// 아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
// 프레임워크에서 이 작업을 자동으로 수행합니다.

void CkelpDlg::OnPaint() 
{
   outputList();

	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다. 
HCURSOR CkelpDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CkelpDlg::GetDrive()
{
   // 시스템의 드라이브 비트 마스크를 얻는다
   DWORD dwDrive = ::GetLogicalDrives();
   int i, nDriveCount = 0;

   // 시스템의 드라이브 갯수를 센다
   for(i = 0; i < 32; i++)
   {
      if(dwDrive & ((DWORD)0x01 << i))nDriveCount++;
   }

   // 드라이브 이름을 얻을 버퍼를 만든다
   m_pDriveName = new char[nDriveCount * 4 + 1];
   // 드라이브 이름 배열을 얻는다
   ::GetLogicalDriveStrings(nDriveCount * 4 + 1, m_pDriveName);

   UINT nDriveType;
   SHFILEINFO ShellFileInfo;

   m_nDriveCnt = nDriveCount;
   // 모든 드라이브에 대해
   for(i = 0; i < nDriveCount; i++)
   {
      // 드라이브의 정보를 얻는다
      ::SHGetFileInfo(m_pDriveName + i * 4, FILE_ATTRIBUTE_DIRECTORY,
         &ShellFileInfo, sizeof(SHFILEINFO),
         SHGFI_USEFILEATTRIBUTES | SHGFI_SYSICONINDEX | SHGFI_LARGEICON);

      nDriveType = ::GetDriveType(m_pDriveName + i * 4);
      if(nDriveType == DRIVE_REMOVABLE)
      {
         if(stricmp(m_pDriveName + i * 4, "A:\\") == 0) continue;
         if(stricmp(m_pDriveName + i * 4, "B:\\") == 0) continue;

         // pDriveName에 드라이브 이름이 있으므로
         // 여기서 USB 저장장치에 파일을 생성하거나 작업을 합니다.
      }
   }

   // 드라이브 이름 버퍼를 삭제한다
   //delete[] pDriveName;

   for(i=0; i<nDriveCount; ++i)
      m_fileList.push_back(m_pDriveName+i*4);
}

int CkelpDlg::MoveDirectroy(CString &strPath)
{
   BOOL ret;
   CFileFind ff;
   CString efound, efront, epath;  //배열선언
   epath = strPath+"\\";

   int ei = 0, ecount, enchanged=1;

   ecount = 0;
   m_fileList.clear();

   BOOL ebWorking = ff.FindFile( epath+CString(_T("\\*")) );

   if(m_pathStack.size() >= 2)
   {   
      m_fileList.push_back("..");
      ++ecount;
   }

   while (ebWorking)
   {      
      ebWorking = ff.FindNextFile();  
      efront = ff.GetFileName();   
      if(strcmp(efront.GetString() + strlen(efront.GetString())-2, ".x") == 0 || 
         strcmp(efront.GetString() + strlen(efront.GetString())-2, ".X") == 0 ||
         strchr(efront.GetString(), '.') == NULL)
      {      
         efound = epath+efront;
         m_fileList.push_back(efound);
         ++ecount;
      }
      ++ei;
   }      

   return ecount;
}

void CkelpDlg::outputList()
{   
   int cnt;

   cnt = m_fileListBox.GetCount();

   for(int i=cnt; i>=0; --i)
      m_fileListBox.DeleteString(i);

   if(m_pathStack.size() == 1)
      m_fileListBox.AddString("..");

   for(int i=0; i<m_nDriveCnt; ++i)
      m_fileListBox.AddString(m_fileList[i]);
}

void CkelpDlg::OnLbnSelchangeList1()
{
   // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
   
}

void CkelpDlg::OnLbnDblclkList1()
{
   // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
   CString getStr;
   int sel;

   m_oldcnt = m_nDriveCnt;

   sel = m_fileListBox.GetCurSel();
   m_fileListBox.GetText(sel, getStr);

   m_fileList.clear();

   if(strcmp(getStr.GetString(), "..") == 0) // 이전폴더로...
   {      
      if(m_pathStack.size() <= 1)
      {         
         m_pathStack.pop();
         GetDrive();
      }
      else
      {      
         m_pathStack.pop();
         m_currentPath = m_pathStack.top();
         m_pathStack.pop();
         m_nDriveCnt = MoveDirectroy(m_currentPath);
      }
      
   }
   else if(strcmp(getStr.GetString(), ".") == 0)
   {
      // 현재 디렉토리
   }
   else
   {      
      m_currentPath = getStr;
      m_pathStack.push(m_currentPath);

      m_nDriveCnt = MoveDirectroy(m_currentPath);
   }
     

   GetDlgItem(IDC_STATIC)->SetWindowText(m_currentPath.GetString());
   OnPaint();



   //m_fileListBox.UpdateData();
   //m_fileListBox.Invalidate();
   //MessageBox(getStr.GetString(), "info");
}

void CkelpDlg::OnBnClickedOk()
{
   // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
   //OnOK();
   int ret;
   int count = 0;
   
   CString strSel;

   //GetDlgItem(IDOK)->EnableWindow(false);

   for(int i=0; i< m_fileListBox.GetCount(); ++i)
   {
      ret = m_fileListBox.GetSel(i);
      if(ret)
      {
         m_fileListBox.GetText(i, strSel);

         if(strcmp(strSel.GetString() + strlen(strSel.GetString())-2, ".x") == 0 || 
            strcmp(strSel.GetString() + strlen(strSel.GetString())-2, ".X") == 0)
         {
#if 0 
            ++count;
            if(count == 1)
            {
                char buf[1024];
                sprintf(buf, "%s", m_currentPath);
                strcat(buf, "Result");
                CreateDirectory(buf, NULL);
            }
#endif
            m_convertFileList.push_back(strSel);
         }
         else
         {
            addFolder(strSel);
         }
      }
   }   

   g_cwnd = this;
    
   AfxBeginThread(threadFunc, NULL);
   g_bThread = true;
   // 변환될 파일들 출력
      
#if 0 
   m_fileList.clear();   
   for(int i=0; i<m_convertFileList.size(); ++i)
   {
      m_fileList.push_back(m_convertFileList[i]);
   }

   m_nDriveCnt = m_convertFileList.size();
   OnPaint();
#endif
   
   m_nDriveCnt = m_convertFileList.size();   

   //m_nDriveCnt = MoveDirectroy(m_currentPath);
}

void CkelpDlg::addFolder(CString strPath)
{
   BOOL ret;
   CFileFind ff;
   CString efound, efront, epath;  //배열선언
   epath = strPath+"\\";
   int count;
   int ei = 0, ecount, enchanged=1;

   count = ecount = 0;  

   BOOL ebWorking = ff.FindFile( epath+CString(_T("\\*")) );   

   while (ebWorking)
   {      
      ebWorking = ff.FindNextFile();  
      efront = ff.GetFileName();
      efound = epath+efront;

      if(strcmp(efront.GetString() + strlen(efront.GetString())-2, ".x") == 0 || 
         strcmp(efront.GetString() + strlen(efront.GetString())-2, ".X") == 0)         
      {            
#if 0 
          ++count;
          if(count == 1)
          {
              char buf[1024];
              sprintf(buf, "%s", epath);
              strcat(buf, "Result");
              CreateDirectory(buf, NULL);
          }
#endif

         m_convertFileList.push_back(efound);         
      }
      else if(strchr(efront.GetString(), '.') == NULL)
      {
         addFolder(efound = epath+efront);
      }

      ++ei;
   }      
}

UINT threadFunc(LPVOID ss)
{
   char strCmd[1024];

   for(int i=0; i<m_convertFileList.size(); ++i)
   {
      if(g_bThread)
      {      
        sprintf(strCmd, "%s [%d]", m_convertFileList[i].GetString(), i+1);
        g_cwnd->GetDlgItem(IDC_STATIC)->SetWindowText(strCmd);
        sprintf(strCmd, "KOGSkinMesh.exe %s %s", m_convertFileList[i].GetString(), m_convertFileList[i].GetString());
        system(strCmd);
      }
      else
          return 0;
   }

   m_convertFileList.clear();
   return 0;
}


void CkelpDlg::OnBnClickedCancel()
{
   // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
   if(g_bThread)
   {
      g_bThread = false;
      //AfxEndThread(0);
      Sleep(500);
   }
   OnCancel();
}
