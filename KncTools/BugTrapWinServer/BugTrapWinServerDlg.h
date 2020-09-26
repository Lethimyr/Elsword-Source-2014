// BugTrapWinServerDlg.h : 헤더 파일
//

#pragma once

#include<vector>
#include<string.h>
#include "afxwin.h"

#include "CrashImageDlg.h"

using namespace std;

typedef struct 
{
    string fullFileName;
    string fileName;
} FILE_NAME;

typedef struct 
{
    unsigned int address;
    char *name;
} MAP_INFO;

typedef struct 
{
    char *name;
    int count;
    std::vector<string> m_fileName;
} LOG_RESULT;

typedef struct 
{
    unsigned int lineNum;
    unsigned int Address;
} LINE_SUBINFO;

typedef struct 
{
    string fileName;
    std::vector<LINE_SUBINFO> m_vecAddress;
} LINE_INFO;

// CBugTrapWinServerDlg 대화 상자
class CBugTrapWinServerDlg : public CDialog
{
// 생성
public:
	CBugTrapWinServerDlg(CWnd* pParent = NULL);	// 표준 생성자
    virtual ~CBugTrapWinServerDlg();

// 대화 상자 데이터
	enum { IDD = IDD_BUGTRAPWINSERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원


// 구현
protected:
	HICON m_hIcon;

	// 메시지 맵 함수를 생성했습니다.
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedOk();

public:
    unsigned int GetAddressValue(char *token);
    void ClearListBox();
    void releaseFileNameList();
    void ReleaseMap();
    void ReleaseResult();
    void ReleaseLineInfo();
    std::string GetUndecoratedFunctionName(string m_strFunction) const;
    bool LoadMapFile(const char *szFile);
    MAP_INFO *GetMapInfo(unsigned int address, int &offset);
    char *GetLineInfo(unsigned int address, int &lineNum);
    void LoadLog(char *szPath, char*szName);
    void addResult(MAP_INFO *mapInfo, char*szName);
    void LoadTotalLog(char *szPath);
    void SaveTotalLog(char *szPath);
    void OutputLog();
    void OutPutImage(char *fullPath);
    void OutPutReport(char *fullPath);

public:
    std::string m_strFunction;

    std::vector<FILE_NAME *> m_vecFileName;
    CListBox m_fNameList;
    CStatic m_progressText;
    CStatic m_textProgress;

private:
    std::string m_strFullPath;
    int m_totalLogCnt;
    unsigned int m_preferredAddress;
    std::vector<MAP_INFO *> m_vecMapInfo;
    std::vector<LOG_RESULT *> m_vecResultLog;
    std::vector<LINE_INFO *> m_vecLineInfo;

public:
    CEdit m_textAddress;
    afx_msg void OnEnChangeEdit1();
    CListBox m_crashList;
    CListBox m_crashSubList;
    afx_msg void OnLbnSelchangeListCrashList();
    afx_msg void OnLbnDblclkListCrashSublist();
    CStatic m_versionText;

    CCrashImageDlg *m_imageDlg;
    
    CEdit m_bugReportText;
    afx_msg void OnLbnSelchangeListCrashSublist();
    CStatic m_logFileText;
};
