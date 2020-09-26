// BugTrapWinServerDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "BugTrapWinServer.h"
#include "BugTrapWinServerDlg.h"
#include ".\bugtrapwinserverdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#define countof(arr) (sizeof(arr)/sizeof((arr)[0]))


static int GetTextLenEx(CListBox& box, LPCTSTR lpszText)
{
    CSize size;
    CDC *pDC = box.GetDC();

    // 현재 리스트박스의 폰트를 얻어와 DC에 적용시킨다.
    CFont* pOld = pDC->SelectObject(box.GetFont());

    // 스타일에 따라 약간의 오프셑 차이가 있다.
    if ((box.GetStyle() & LBS_USETABSTOPS) == 0)
    {
        size = pDC->GetTextExtent(lpszText, _tcslen(lpszText));
        size.cx += 3;
    }
    else
    {
        size = pDC->GetTabbedTextExtent(lpszText, _tcslen(lpszText), 0, NULL);
        size.cx += 2;
    }
    pDC->SelectObject(pOld);
    box.ReleaseDC(pDC);

    // 구한 문자열의 Pixel 단위를 넘긴다.
    return size.cx;
}



bool resultSort(const LOG_RESULT *fi, const LOG_RESULT *se)
{
    if(fi->count > se->count)
        return true;

    return false;
}


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


// CBugTrapWinServerDlg 대화 상자



CBugTrapWinServerDlg::CBugTrapWinServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBugTrapWinServerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CBugTrapWinServerDlg::~CBugTrapWinServerDlg()
{
    releaseFileNameList();
    ReleaseMap();
    ReleaseResult();
    ReleaseLineInfo();
}

void CBugTrapWinServerDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST1, m_fNameList);
    DDX_Control(pDX, IDC_STATIC_PROGRESS, m_progressText);
    DDX_Control(pDX, IDC_STATIC_PROGRESS2, m_textProgress);
    DDX_Control(pDX, IDC_EDIT1, m_textAddress);
    DDX_Control(pDX, IDC_LIST_CRASH_LIST, m_crashList);
    DDX_Control(pDX, IDC_LIST_CRASH_SUBLIST, m_crashSubList);
    DDX_Control(pDX, IDC_STATIC_VERSION, m_versionText);
    DDX_Control(pDX, IDC_EDIT2, m_bugReportText);
    DDX_Control(pDX, IDC_STATIC_VERSION2, m_logFileText);
}

BEGIN_MESSAGE_MAP(CBugTrapWinServerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
    ON_EN_CHANGE(IDC_EDIT1, OnEnChangeEdit1)
    ON_LBN_SELCHANGE(IDC_LIST_CRASH_LIST, OnLbnSelchangeListCrashList)
    ON_LBN_DBLCLK(IDC_LIST_CRASH_SUBLIST, OnLbnDblclkListCrashSublist)
    ON_LBN_SELCHANGE(IDC_LIST_CRASH_SUBLIST, OnLbnSelchangeListCrashSublist)
END_MESSAGE_MAP()


// CBugTrapWinServerDlg 메시지 처리기

BOOL CBugTrapWinServerDlg::OnInitDialog()
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
	
	return TRUE;  // 컨트롤에 대한 포커스를 설정하지 않을 경우 TRUE를 반환합니다.
}

void CBugTrapWinServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CBugTrapWinServerDlg::OnPaint() 
{
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
HCURSOR CBugTrapWinServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CBugTrapWinServerDlg::releaseFileNameList()
{
    for(unsigned int i=0; i<m_vecFileName.size(); ++i)
    {
        if(m_vecFileName[i] != NULL)
            delete m_vecFileName[i];
    }

    m_vecFileName.clear();
}


std::string CBugTrapWinServerDlg::GetUndecoratedFunctionName(string m_strFunction) const
{
    /*
    * Name decoration usually refers to C++ naming conventions, but can apply
    * to a number of C cases as well. By default, C++ uses the function name,
    * parameters, and return type to create a linker name for the function.
    * Consider the following function:
    *
    * void CALLTYPE test(void)
    *
    * The following table shows the linker name for various calling conventions.
    *
    *  Calling convention                          | extern "C" or .c file | .cpp, .cxx or /TP
    * ---------------------------------------------+-----------------------+-------------------
    *  C naming convention (__cdecl)               | _test                 | ?test@@ZAXXZ
    *  Fast-call naming convention (__fastcall)    | @test@0               | ?test@@YIXXZ
    *  Standard Call naming convention (__stdcall) | _test@0               | ?test@@YGXXZ
    */
    char chFirstChar = m_strFunction[0];
    if (chFirstChar == '_' || chFirstChar == '@')
    {
        std::string strUndecoratedName, strDecoratedName = m_strFunction.substr(1);
        size_t nAtPos = strDecoratedName.find('@');
        if (nAtPos != std::string::npos)
        {
            strDecoratedName.resize(nAtPos);
            if (chFirstChar == '_')
                strUndecoratedName = "__stdcall ";
            else /* if (chFirstChar == '@') */
                strUndecoratedName = "__fastcall ";
        }
        else
            strUndecoratedName = "__cdecl ";
        strUndecoratedName += strDecoratedName;
        strUndecoratedName += "()";
        return strUndecoratedName;
    }
    else
    {
        CHAR szNameBuffer[2048];
        if (UnDecorateSymbolName(m_strFunction.c_str(), szNameBuffer, countof(szNameBuffer), UNDNAME_COMPLETE))
            return std::string(szNameBuffer);
        else
            return m_strFunction;
    }
}


void CBugTrapWinServerDlg::OnBnClickedOk()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    //OnOK();

    // 파싱할 버그로그 폴더를 선택한다.
    BROWSEINFO  biBrowseInfo;
    TCHAR   szDisplayName[MAX_PATH];
    CString   strInitFolder;
    LPITEMIDLIST lpItemIDList;



    // BROWSEINFO 구조체 생성 : 스타일 변경
    ZeroMemory(&biBrowseInfo, sizeof(biBrowseInfo));
    biBrowseInfo.hwndOwner  = AfxGetMainWnd ()->GetSafeHwnd();
    biBrowseInfo.pidlRoot  = NULL;
    biBrowseInfo.pszDisplayName = szDisplayName;
    biBrowseInfo.lpszTitle  = "Select X2 BugLog folder.";
    // biBrowseInfo.lpfn   = BrowseCallbackProc; // 콜백함수, 바로 위쪽에 정의함.
    biBrowseInfo.ulFlags  = 0; // 함수에 전달되어질 인자.
    strInitFolder    = AfxGetApp()->GetProfileString(_T("FolderPath"), _T("LogFolder"), _T("C:\\"));
    biBrowseInfo.lParam   = (LPARAM)(LPCTSTR)strInitFolder; // 폴더선택 대화상자가 초기화될때 선택되는 폴더명을 지정
    biBrowseInfo.iImage   = 0;

    // 다이얼로그 표시 & 선택한 폴더를 위한 itemIDList 획득
    lpItemIDList    = SHBrowseForFolder(&biBrowseInfo);

    if (lpItemIDList != NULL)
    {
        // 선택경로 획득
        HANDLE hSrch;
        WIN32_FIND_DATA wfd;
        BOOL bResult = TRUE;
        char szPath[MAX_PATH];
        char szSear[MAX_PATH];
        char fname[MAX_PATH];
        char szName[MAX_PATH], szExt[10];
        char szCurrentDir[MAX_PATH];
        string resultLog;

        SHGetPathFromIDList(lpItemIDList, szPath);  // 패스를 얻어온다.

        // 파일 리스트를 초기화한다.        
        releaseFileNameList();
        ReleaseMap();
        ReleaseResult();  
        ReleaseLineInfo();
        ClearListBox();

        // 선택한 경로를 저장한다.
        m_strFullPath = szPath;

        // 버전명을 가져온다.
        char *ptVersion;
        int offsetVersion = strlen(szPath) -2;
        while(szPath[offsetVersion] != '\\')
        {
            --offsetVersion;
        }

        ptVersion = szPath + offsetVersion+1;
        m_versionText.SetWindowText(ptVersion);

        // 선택된 경로에서 MAP파일을 찾는다.
        bool bFindMap = false;

        bResult = FALSE;
        sprintf(wfd.cFileName, "");        
        strcpy(szSear, szPath);
        strcat(szSear, "\\X2.map");
        
        while(!bFindMap)
        {
            hSrch = FindFirstFile(szSear, &wfd);
            if(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                // 디렉토리
            }
            else
            {
                // 파일
                wsprintf(fname, "%s", wfd.cFileName);
                if(strcmp(fname, "X2.map") == 0)
                {
                    bFindMap = true;
                }
                else
                {
                    MessageBox("X2.map파일이 존재하지 않습니다.", "Info!", MB_OK);
                    return;
                }

            }
        }

        // map 파일을 로드한다.
        if(LoadMapFile(szSear) == false)
        {
            MessageBox("X2.map파일을 로드할 수 없습니다.", "Error!", MB_OK);
            return;
        }       

        // 최종통계자료를 로드한다.
        LoadTotalLog(szPath);

        // 선택된 경로에서 압축된 로그파일들을 찾아서 보관한다.
        bResult = TRUE;
        strcpy(szSear, szPath);
        strcat(szSear, "\\*.zip");
        hSrch = FindFirstFile(szSear, &wfd);
        while(bResult)
        {
            if(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                // 디렉토리
            }
            else
            {
                // 파일
                wsprintf(fname, "%s", wfd.cFileName);

                if(strstr(fname, ".zip") != NULL)
                {
                    _splitpath(fname, NULL, NULL, szName, szExt);

                    FILE_NAME *fInfo = new FILE_NAME();
                    fInfo->fileName = szName;
                    fInfo->fullFileName = fname;

                    m_vecFileName.push_back(fInfo);
                }                
            }
            
            bResult = FindNextFile(hSrch, &wfd);
        }

        char sysCommand[2048], sysCommand2[2048], szProgress[200], szProgress2[200];
        float fProgress = 0.0f;
        
        // 현재 실행파일이 존재하는 폴더위치를 얻어온다.
        GetCurrentDirectory(MAX_PATH, szCurrentDir);
        float stepProgress = 5.0f;

        for(unsigned int i=0; i<m_vecFileName.size(); ++i)
        {
            // 진행율
            fProgress = ((float)(i+1) / (float)(m_vecFileName.size())) * 100.0f;

            strcpy(szProgress, "");
            for(int j=0; j<(int)fProgress; ++j)
            {
                if(j % 2 == 0)
                    strcat(szProgress, "■");
            }
            
                
            

            // 압축파일이 존재하는 폴더에 압축을 푼다.            
            //sprintf(sysCommand, "7za -y e -o%s\\%s %s\\%s", szPath, m_vecFileName[i]->fileName.c_str(), szPath, m_vecFileName[i]->fullFileName.c_str());
            //system(sysCommand);
            sprintf(sysCommand, " -y e -o%s\\%s %s\\%s", szPath, m_vecFileName[i]->fileName.c_str(), szPath, m_vecFileName[i]->fullFileName.c_str());            
            STARTUPINFO             sui;
            PROCESS_INFORMATION     pi;
            DWORD                   Ret;

            memset(&sui, 0x00, sizeof(STARTUPINFO));
            sui.cb = sizeof(STARTUPINFO);

            Ret = CreateProcess("7za.exe", sysCommand, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS | CREATE_NO_WINDOW, NULL, NULL,&sui, &pi);
            if (Ret) 
            {
                WaitForSingleObject(pi.hProcess, 0xffffffff);
                CloseHandle(pi.hProcess);
                CloseHandle(pi.hThread);
            }

            
            // 압축 해제한 로그파일은 backup폴더로 옮겨둔다.
            sprintf(sysCommand, "%s\\BackupLog", szPath);
            CreateDirectory(sysCommand, NULL);  // backup 폴더 생성
            sprintf(sysCommand, "%s\\%s", szPath, m_vecFileName[i]->fullFileName.c_str());
            sprintf(sysCommand2, "%s\\BackupLog\\%s", szPath, m_vecFileName[i]->fullFileName.c_str());
            MoveFile(sysCommand, sysCommand2);  // backup 폴더로 로그파일 이동
            
            // 압축 해제한 로그파일은 지운다.
            //char delFile[MAX_PATH];
            //sprintf(delFile, "%s\\%s", szPath, m_vecFileName[i]->fullFileName.c_str());
            //DeleteFile(delFile);

            // 테스트를 위해서 압축된 로그파일 리스트를 출력한다.
            //m_fNameList.AddString(sysCommand);
            strcat(sysCommand, " 분석중입니다...");
            m_logFileText.SetWindowText(sysCommand);

            // 진행율을 보여준다.            
            m_progressText.SetWindowText(szProgress);

            sprintf(szProgress2, "(%5.2f%%)", fProgress);
            m_textProgress.SetWindowText(szProgress2);


            // 압축해제된 각 로그들을 파싱하여 추가통계를 낸후 플래그 체크한다. (다시 통계를 낼때 기존 로그자료는 적용되지 않도록 한다.)
            sprintf(sysCommand, "%s\\%s\\", szPath, m_vecFileName[i]->fileName.c_str());
            LoadLog(sysCommand, (char*)m_vecFileName[i]->fileName.c_str());
        }        

        // 최종 통계자료를 저장한다.
        SaveTotalLog(szPath);

        // 최종 통계자료 출력한다.
        OutputLog();

        m_logFileText.SetWindowText("");
    }
}

void CBugTrapWinServerDlg::OutputLog()
{
    LOG_RESULT *node;    
    float logAvg;
    char buf[2048];

    for(unsigned int i=0; i<m_vecResultLog.size(); ++i)
    {
        node = m_vecResultLog[i];
        logAvg = (float)(node->count / (float)m_totalLogCnt) * 100.0f;
        
        sprintf(buf, "%5d/%5d (%05.2f%%) %s", node->count, m_totalLogCnt, logAvg, node->name);
        m_crashList.AddString(buf);

        // 길이를 계산하여 기존 길이보다 넓으면 새로운 길이를 적용시킨다.
        int iExt = GetTextLenEx(m_crashList, buf);
        if (iExt > m_crashList.GetHorizontalExtent())
            m_crashList.SetHorizontalExtent(iExt);


    }    
}

void CBugTrapWinServerDlg::LoadTotalLog(char *szPath)
{
    int logCount, logNameCount;
    LOG_RESULT *node;
    char buf[2048], szLog[MAX_PATH];

    sprintf(szLog, "%s\\DumpLog.log", szPath);
    FILE *fp = fopen(szLog, "r");
    if(fp == NULL)
    {
        return;
    }

    ReleaseResult();

    fscanf(fp, "%d", &logCount);    
    for(int i=0; i<logCount; ++i)
    {
        fgets(buf, 2047, fp); // 공란 제거
        fgets(buf, 2047, fp);
        fscanf(fp, "%d", &logNameCount);
        
        buf[strlen(buf)-1] = 0;

        node = new LOG_RESULT();
        node->name = new char[strlen(buf) + 1];
        strcpy(node->name, buf);
        node->count = logNameCount;

        for(int j=0; j<logNameCount; ++j)
        {
            fscanf(fp, "%s", buf);
            node->m_fileName.push_back(buf);
        }

        m_vecResultLog.push_back(node);
   }

    fclose(fp);
        
}

void CBugTrapWinServerDlg::SaveTotalLog(char *szPath)
{
    LOG_RESULT *node;
    char szLog[MAX_PATH];

    m_totalLogCnt = 0;

    sprintf(szLog, "%s\\DumpLog.log", szPath);
    
    FILE *fp = fopen(szLog, "w");
    if(fp == NULL)
    {
        MessageBox("통계파일을 저장할 수 없습니다.", MB_OK);
        return;
    }

    sort(m_vecResultLog.begin(), m_vecResultLog.end(), resultSort);

    fprintf(fp, "%d\n", m_vecResultLog.size());
    for(unsigned int i=0; i<m_vecResultLog.size(); ++i)
    {
        node = m_vecResultLog[i];
        m_totalLogCnt += node->count;

        fprintf(fp, "%s\n%d\n", node->name, node->count);

        for(unsigned int j=0; j<node->m_fileName.size(); ++j)
        {
            string logName = node->m_fileName[j];
            fprintf(fp, "%s ", logName.c_str());
        }
        fprintf(fp, "\n");
    }

    fclose(fp);
}

void CBugTrapWinServerDlg::ReleaseLineInfo()
{
    for(unsigned int i=0; i<m_vecLineInfo.size(); ++i)
    {
        if(m_vecLineInfo[i])
        {
            delete m_vecLineInfo[i];
        }
    }

    m_vecLineInfo.clear();
}

void CBugTrapWinServerDlg::ReleaseResult()
{
    for(unsigned int i=0; i<m_vecResultLog.size(); ++i)
    {
        LOG_RESULT *node = m_vecResultLog[i];

        if(node->name)
            delete node->name;
        delete node;
    }

    m_vecResultLog.clear();
}

void CBugTrapWinServerDlg::ReleaseMap()
{
    for(unsigned int i=0; i<m_vecMapInfo.size(); ++i)
    {
        MAP_INFO *mapInfo = m_vecMapInfo[i];

        if(mapInfo->name)
            delete mapInfo->name;
        delete mapInfo;
    }

    m_vecMapInfo.clear();
}

void CBugTrapWinServerDlg::addResult(MAP_INFO *mapInfo, char*szName)
{    
    LOG_RESULT *logResult, *node;

    for(unsigned int i=0; i<m_vecResultLog.size(); ++i)
    {
        node = m_vecResultLog[i];
        if(strcmp(node->name, mapInfo->name) == 0)
        {
            node->m_fileName.push_back(szName);
            node->count += 1;
            return;
        }
    }

    logResult = new LOG_RESULT();
    logResult->name = new char[strlen(mapInfo->name) + 1];
    strcpy(logResult->name, mapInfo->name);
    logResult->count = 1;
    logResult->m_fileName.push_back(szName);

    m_vecResultLog.push_back(logResult);
}

void CBugTrapWinServerDlg::LoadLog(char *szPath, char*szName)
{
    int addr, offset, nCount;
    MAP_INFO *mapInfo;
    char *ptName, *ptSear, *ptAddr;
    char strLog[MAX_PATH], strDump[MAX_PATH], strLine[2048], tempBuf[2048];
    sprintf(strLog, "%serrorlog.log", szPath);
    sprintf(strDump, "%sdump.log", szPath);

    // 해당 패스에서 log파일을 로드한다.
    FILE *fpLog = fopen(strLog, "r");
    FILE *fpDump = fopen(strDump, "w");

    if(fpLog == NULL || fpDump == NULL)
    {
        return;
    }

    nCount = 0;

    while(1)
    {
        if(feof(fpLog)) 
        {            
            break;
        }

        fgets(strLine, 2047, fpLog);

        ptSear = strstr(strLine, " at ");
        if(ptSear != NULL)
        {
            ptAddr = ptSear + 9;
            ptName = strchr(ptAddr, ',');
        }
        
        if(ptSear == NULL || ptName != NULL)
        {
            fputs(strLine, fpDump);
            continue;
        }

        sscanf(ptAddr, "%x", &addr);
        mapInfo = GetMapInfo(addr, offset);
        if(mapInfo == NULL)
        {
            fputs(strLine, fpDump);
            continue;
        }
        
        // 라인정보
        string lineFileName;
        char *tempfileName;
        int lineNum = 0;
        tempfileName = GetLineInfo(addr, lineNum);
        if(tempfileName == NULL)
        {
            lineFileName = "Not Search";
        }
        else
        {
            lineFileName = tempfileName;
        }

        unsigned int strInx = 0;
        while(strLine[strInx] != 0)
        {
            if(strLine[strInx] == 10)
            {
                strLine[strInx] = 0;
                break;
            }
            ++strInx;
        }
        //strLine[strlen(strLine)-1] = 0;
        sprintf(tempBuf, "%s, %s+%d byte(s) (%s + %d line)\n", strLine, mapInfo->name, offset, lineFileName.c_str(), lineNum);
        fputs(tempBuf, fpDump);

        if(nCount == 0)
            addResult(mapInfo, szName);        
        ++nCount;
    }

    fclose(fpLog);
    fclose(fpDump);    
}
unsigned int CBugTrapWinServerDlg::GetAddressValue(char *token)
{
    unsigned int address = 0;
    int addr1, addr2;
    char ptAddr1[100], ptAddr2[100];
    char *ptTemp, strTemp[1000];;

    address = m_preferredAddress;

    strcpy(ptAddr1, token);
    for(int i=0; i<strlen(ptAddr1); ++i)
    {
        if(ptAddr1[i] == ':')
        {
            ptAddr1[i] = 0;
            break;
        }
    }
    addr1 = atoi(ptAddr1);
    sprintf(strTemp, "%d", addr1 * 1000);
    sscanf(strTemp, "%x", &addr1);

    strcpy(ptAddr2, strchr(token, ':')+1);
    sscanf(ptAddr2, "%x", &addr2);

    address += (addr1 + addr2);

    return address;
}

bool CBugTrapWinServerDlg::LoadMapFile(const char *szFile)
{
    // Map파일을 로드한다.
    LINE_INFO *lineInfo;
    FILE *fpMap = NULL;
    fpMap = fopen(szFile, "r");
    if(fpMap == NULL)
    {
        MessageBox("X2.map파일을 로드할 수 없습니다.", "Error!", MB_OK);
        return false;
    }

    int tempCount = 0;
    unsigned int addr;
    char buf[2048], name[2048];
    m_preferredAddress = 0;    
    
    m_progressText.SetWindowText("Map 파싱 중입니다. 잠시만 기다리세요!");

    while(fscanf(fpMap, "%s", buf) != EOF)
    {
        switch(tempCount)
        {
        case 0:
            if(strcmp(buf, "Preferred") == 0)
            {
                tempCount = 1;                
            }
            break;
        case 1:
            if(strcmp(buf, "is") == 0)
            {
                fscanf(fpMap, "%x", &addr);
                m_preferredAddress = addr;
                tempCount = 2;
            }
            break;
        case 2:
            if(strcmp(buf, "Rva+Base") == 0)
            {
                tempCount = 3;
            }
            break;
        case 3:
            {
                int tokenCount = 0;
                char*  token  =  NULL;
                char sep[] = " \t\n";

                // 공란제거
                fgets(buf, 2047, fpMap); 
                fgets(buf, 2047, fpMap); 

                while(1)
                {               
                    tokenCount = 0;
                    // 실제 데이터를 가져온다.
                    fgets(buf, 2047, fpMap);                   

                    token = strtok(buf, sep);
                    if(token == NULL || token[0] == 10)
                    {
                        tempCount = 4;
                        break;
                    }

                    while(token != NULL)
                    {
                        // 분리된 토큰
                        ++tokenCount;

                        if(tokenCount == 2)
                        {
                            // Publics by Value
                            string strFuncName = GetUndecoratedFunctionName(token);
                            strcpy(name, strFuncName.c_str());

                        }
                        else if(tokenCount == 3)
                        {
                            // Rva+Base
                            sscanf(token, "%x", &addr);
                        }
                        token = strtok(NULL, sep);
                    }

                    MAP_INFO *mapInfo = new MAP_INFO();
                    mapInfo->address = addr;
                    mapInfo->name = new char[strlen(name) + 1];
                    strcpy(mapInfo->name, name);
                    m_vecMapInfo.push_back(mapInfo);
                }
            }            
            break;
        case 4:
            // line 정보를 파싱
            while(1)
            {
                char *ptFileName, *tempPt;
                if(feof(fpMap)) 
                {          
                    tempCount = 100;
                    break;
                }

                fgets(buf, 2047, fpMap);
                if(strstr(buf, "numbers for ") != NULL)
                {
                    ptFileName = strchr(buf, '(') + 1;
                    tempPt = strchr(buf, ')');
                    tempPt[0]  = 0;     
                    tempCount = 5;

                    lineInfo = new LINE_INFO();
                    lineInfo->fileName = ptFileName;

                    m_vecLineInfo.push_back(lineInfo);            

                    break;
                }                
            }
            fgets(buf, 2047, fpMap); // 공란제거
            
            if(tempCount == 100)
                break;

        case 5:
            // Line
            {
            LINE_SUBINFO subNode;

            int tokenCount = 0;
            char*  token  =  NULL;
            char sep[] = " \t\n";

            while(1)
            {   
                tokenCount = 0;
                // 실제 데이터를 가져온다.
                if(feof(fpMap)) 
                {          
                    tempCount = 100;
                    break;
                }
                fgets(buf, 2047, fpMap);                   

                token = strtok(buf, sep);
                if(token == NULL || token[0] == 10)
                {
                    tempCount = 4;
                    break;
                }

                while(token != NULL)
                {
                    // 분리된 토큰
                    ++tokenCount;

                    if(tokenCount % 2 == 1)
                    {
                        // line Value
                        subNode.lineNum = atoi(token);
                    }
                    else 
                    {
                        // address Value
                        subNode.Address = GetAddressValue(token);
                        lineInfo->m_vecAddress.push_back(subNode);
                    }
                    token = strtok(NULL, sep);
                }
            }
            }
            break;
        case 100:
            fclose(fpMap);
            m_progressText.SetWindowText("Map 파싱 완료!");            
            //sort(m_vecMapInfo.begin(), m_vecMapInfo.end(), mapSort);
            return true;
        }       
    }

    fclose(fpMap);
    m_progressText.SetWindowText("Map 파싱 완료!"); 

    return true;
}

char *CBugTrapWinServerDlg::GetLineInfo(unsigned int address, int &lineNum)
{
    char *strFileName;
    unsigned int step = 0xffffffff;
    LINE_INFO *node, *preNode;
    LINE_SUBINFO subNode, preSubNode;

    strFileName = new char[2048];
    preNode = NULL;

    for(unsigned int i=0; i<m_vecLineInfo.size(); ++i)
    {
        node = m_vecLineInfo[i];

        for(unsigned int j=0; j<node->m_vecAddress.size(); ++j)
        {
            subNode = node->m_vecAddress[j];

            if(address == subNode.Address)
            {
                if(j+1 < node->m_vecAddress.size())
                    subNode = node->m_vecAddress[j+1];

                lineNum = subNode.lineNum;
                strFileName = new char[node->fileName.length() + 1];
                strcpy(strFileName, node->fileName.c_str());
                return strFileName;
            }
            if(address < subNode.Address)
            {
                if(j==0)
                {
                    if(step >= subNode.Address - address)
                    {
                        step = subNode.Address - address;

                        if(preNode != NULL)
                        {
                            lineNum = preSubNode.lineNum;                        
                            strcpy(strFileName, preNode->fileName.c_str());
                        }                        
                    }                    

                    //return strFileName;
                }
                else
                {
                    if(step >= subNode.Address - address)
                    {
                        step = subNode.Address - address;

                        if(node != NULL)
                        {
                            lineNum = preSubNode.lineNum;                    
                            strcpy(strFileName, node->fileName.c_str());
                        }
                    }
                    //return strFileName;
                }
                
            }

            preSubNode = subNode;
        }

        preNode = node;
    }

    if(step < 0xffffffff)
    {
        return strFileName;
    }

    return NULL;
}

MAP_INFO* CBugTrapWinServerDlg::GetMapInfo(unsigned int address, int &offset)
{
    MAP_INFO *preMapInfo, *nowMapInfo;

    preMapInfo = nowMapInfo = NULL;
    for(unsigned int i=0; i<m_vecMapInfo.size(); ++i)
    {
        nowMapInfo = m_vecMapInfo[i];

        if(nowMapInfo->address == address)
        {
            offset = address - nowMapInfo->address;
            return nowMapInfo;
        }

        if(nowMapInfo->address > address)
        {
            offset = address - preMapInfo->address;
            return preMapInfo;
        }

        preMapInfo = nowMapInfo;
    }

    return NULL;
}

void CBugTrapWinServerDlg::ClearListBox()
{
    m_fNameList.ResetContent();
    m_crashList.ResetContent();
    m_crashSubList.ResetContent();
}

void CBugTrapWinServerDlg::OnEnChangeEdit1()
{
    // 테스트용
    unsigned int address;
    int offset;
    CString strAddress;
    m_textAddress.GetWindowText(strAddress);

    sscanf(strAddress.GetString(), "%x", &address);

    MAP_INFO *mapInfo;
    mapInfo = GetMapInfo(address, offset);

    if(mapInfo == NULL)
    {
        MessageBox("MAP에서 해당 주소의 함수를 찾지 못했습니다.", "Info!", MB_OK);
        return;
    }

    MessageBox(mapInfo->name, "Info!", MB_OK);
}

void CBugTrapWinServerDlg::OnLbnSelchangeListCrashList()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    // crash list item 선택
    m_crashSubList.ResetContent();

    int selIndex = -1;   
    selIndex = m_crashList.GetCurSel();

    LOG_RESULT *node = m_vecResultLog[selIndex];

    for(unsigned int i=0; i<node->m_fileName.size(); ++i)
    {
        m_crashSubList.AddString(node->m_fileName[i].c_str());
    }
}

void CBugTrapWinServerDlg::OnLbnDblclkListCrashSublist()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    // carsh sub list item 더블클릭시

    char fullPath[MAX_PATH];
    CString bufReport;
    int selIndex = -1;
    selIndex = m_crashSubList.GetCurSel();

    m_crashSubList.GetText(selIndex, bufReport);

    sprintf(fullPath, "%s\\%s", m_strFullPath.c_str(), bufReport.GetString());
    
    OutPutImage(fullPath);

}

void CBugTrapWinServerDlg::OutPutImage(char *fullPath)
{
    char pathName[MAX_PATH];
    sprintf(pathName, "%s\\Crash_ScreenShot.jpg", fullPath);

    m_imageDlg = new CCrashImageDlg(this);
    m_imageDlg->Create(IDD_DIALOG1);
    m_imageDlg->ShowWindow(SW_SHOW);
    m_imageDlg->LoadImage(pathName);
    m_imageDlg->Draw();
    
}


void CBugTrapWinServerDlg::OnLbnSelchangeListCrashSublist()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    char fullPath[MAX_PATH];
    CString bufReport;
    int selIndex = -1;
    selIndex = m_crashSubList.GetCurSel();

    m_crashSubList.GetText(selIndex, bufReport);

    sprintf(fullPath, "%s\\%s", m_strFullPath.c_str(), bufReport.GetString());

    OutPutReport(fullPath);
}

void CBugTrapWinServerDlg::OutPutReport(char *fullPath)
{    
    string strReport;
    char tempBuf[2049], buf[2049];
    char pathName[MAX_PATH];
    sprintf(pathName, "%s\\dump.log", fullPath);

    FILE *fp = fopen(pathName, "rt");
    strReport = "";
    while(1)
    {
        if(feof(fp))
        {
            break;
        }
        
        
        fgets(tempBuf, 2048, fp);               
        strReport += "\r\n";
        strReport += tempBuf;        
    }

    fclose(fp);
   
    
    m_bugReportText.SetWindowText(strReport.c_str());
}


