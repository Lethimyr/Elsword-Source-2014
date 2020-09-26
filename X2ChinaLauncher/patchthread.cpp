#include "stdafx.h"
#include "X2ChinaLauncher.h"
#include "patchthread.h"
#include "ServerManager.h"
#include "MainDlg.h"
#include "PatchReadyDlg.h"
#include "process.h"
#include "Util.h"



#if defined(BUILD_BY_HUDSON)
#include "../hudsondef.h"
#endif

HWND g_hWnd;
KPatcher* g_patcher = NULL;
HANDLE      g_hMutex = NULL;    ///< Mutex of game client program.
std::ofstream       g_logStream;        ///< Log file stream.
LPSTR				g_lpCmdLine = "";
float g_fTotalDownloadRatio = 0;
float g_fFileDownloadRatio = 0;
std::string g_CurFilename = "";


#ifdef GATHER_PATCH_INFO
// %Y-%m-%d %H:%M:%S 예) 2012-01-09 00:00:00
const int	g_iTimeStrLength = 19;	
std::wstring	g_wstrPatchStartTime = L"";

int	g_iDueSec_BeforePatcherCompare = 0;	// 패처 정보 비교 전 모든 과정에 걸린 시간
int	g_iDueSec_Patcher = 0;				// 패처 정보 비교, 다운로드 포함해 걸린 시간
int	g_iDueSec_LocalInfo = 0;			// 로컬 정보 생성 및 비교까지 포함해 걸린 시간
int	g_iDueSec_ClientDownload = 0;		// 패처 제외한 다른 모든 파일 받는데 걸린 시간

unsigned int	g_uiPatcherAdler32 = 0;
unsigned int	g_uiPatcherSize = 0;
unsigned int	g_uiPatcherFiletime = 0;

int	g_iTotalDownloadFileCnt = 0;		// 다운로드 받은 파일 수 
int	g_iAdditionalDownload = 0;			// 연결 불안으로 추가 시도 횟수

float	g_fTotalDownloadFileSize = 0;	// 다운 관련 파일들 전체 용량
float	g_fTotalDownloadDataSize = 0;	// 실제로 다운 받은 용량

void WritePatchInfoToFile()
{
	FILE *fp = fopen(".\\data\\lastPatchInfo.dat", "wb+");

	if(!fp)
		return;


	try
	{
		// 날짜 형식은 길이 20임
		fwrite(g_wstrPatchStartTime.c_str(),sizeof(WCHAR),g_iTimeStrLength,fp);

		fwrite(&g_iDueSec_BeforePatcherCompare,sizeof(int),1,fp);
		fwrite(&g_iDueSec_Patcher,sizeof(int),1,fp);
		fwrite(&g_iDueSec_LocalInfo,sizeof(int),1,fp);
		fwrite(&g_iDueSec_ClientDownload,sizeof(int),1,fp);

		fwrite(&g_uiPatcherAdler32,sizeof(unsigned int),1,fp);
		fwrite(&g_uiPatcherSize,sizeof(unsigned int),1,fp);
		fwrite(&g_uiPatcherFiletime,sizeof(unsigned int),1,fp);
		

		fwrite(&g_iTotalDownloadFileCnt,sizeof(int),1,fp);
		fwrite(&g_iAdditionalDownload,sizeof(int),1,fp);
		fwrite(&g_fTotalDownloadFileSize,sizeof(float),1,fp);
		fwrite(&g_fTotalDownloadDataSize,sizeof(float),1,fp);
	}
	catch (CException* e)
	{
	}

	if(fp != NULL)
		fclose(fp);
}

void LoadPatchInfoFromFile()
{

	FILE *fp = fopen(".\\data\\lastPatchInfo.dat", "rb");
	
	if(!fp)
		return;


	WCHAR wcharStartTime[g_iTimeStrLength+1];

	std::wstring tmp_wstrPatchStartTime = L"";


	int	tmp_iDueSec_BeforePatcherCompare = 0;	// 패처 정보 비교 전 모든 과정에 걸린 시간
	int	tmp_iDueSec_Patcher = 0;				// 패처 정보 비교, 다운로드 포함해 걸린 시간
	int tmp_iDueSec_LocalInfo = 0;			// 로컬 정보 생성 및 비교까지 포함해 걸린 시간
	int	tmp_iDueSec_ClientDownload = 0;		// 패처 제외한 다른 모든 파일 받는데 걸린 시간

	unsigned int tmp_uiPatcherAdler32 = 0;
	unsigned int tmp_uiPatcherSize = 0;
	unsigned int tmp_uiPatcherFiletime = 0;

	int	tmp_iTotalDownloadFileCnt = 0;
	int tmp_iAdditionalDownload = 0;

	float tmp_fTotalDownloadFileSize = 0;	// 다운 관련 파일들 전체 용량
	float tmp_fTotalDownloadDataSize = 0;	// 실제로 다운 받은 용량

	//int iServerID;
	//fread(&iServerID, sizeof(int), sizeof(BYTE), fp);

	size_t n_size = 0;

	try
	{
		fseek(fp, 0, SEEK_SET);
		fread(&wcharStartTime,sizeof(WCHAR),g_iTimeStrLength,fp);
		wcharStartTime[g_iTimeStrLength] = L'\0';

		tmp_wstrPatchStartTime = wcharStartTime;

		fread(&tmp_iDueSec_BeforePatcherCompare,sizeof(int),1,fp);
		fread(&tmp_iDueSec_Patcher,sizeof(int),1,fp);
		fread(&tmp_iDueSec_LocalInfo,sizeof(int),1,fp);
		fread(&tmp_iDueSec_ClientDownload,sizeof(int),1,fp);

		fread(&tmp_uiPatcherAdler32,sizeof(unsigned int),1,fp);
		fread(&tmp_uiPatcherSize,sizeof(unsigned int),1,fp);
		fread(&tmp_uiPatcherFiletime,sizeof(unsigned int),1,fp);


		fread(&tmp_iTotalDownloadFileCnt,sizeof(int),1,fp);
		fread(&tmp_iAdditionalDownload,sizeof(int),1,fp);
		fread(&tmp_fTotalDownloadFileSize,sizeof(float),1,fp);
		fread(&tmp_fTotalDownloadDataSize,sizeof(float),1,fp);
	}
	catch (CException* e)
	{
	}


	if(fp != NULL)
		fclose(fp);
}

#endif GATHER_PATCH_INFO



void Log( const char* szLogMsg )
{
#if defined(INTERNAL_TEST)
	g_logStream << szLogMsg << std::endl;
#endif
}

/// 로그파일에 로그 에러 메세지를 남긴다.
/// @param szLogMsg 로그 메세지
/// @return void
void LogError( const char* szLogMsg )
{
#if defined(INTERNAL_TEST)
	DWORD   dwErr = ::GetLastError();
	HRESULT hr    = HRESULT_FROM_WIN32( dwErr );
	g_logStream << szLogMsg << std::endl
		<< "    (WIN32 System Error Code : "
		<< dwErr << ")" << std::endl
		<< "    (HRESULT Code : "
		<< hr << ")" << std::endl;
#endif
}


void ThreadReady(HWND hWnd)
{
	g_hWnd = hWnd;
}

void SetFileDownloadRatio(float fRatio)
{
	g_fFileDownloadRatio = fRatio;
}

float GetFileDownloadRatio()
{
	return g_fFileDownloadRatio;
}

void SetTotalDownloadRatio(float fRatio)
{
	g_fTotalDownloadRatio = fRatio;
}

float GetTotalDownloadRatio()
{
	return g_fTotalDownloadRatio;
}

bool TotalProgress(float p)
{
	if(!g_patcher->IsStoped())
		g_fTotalDownloadRatio = p;

//	Invalidate();
	CServerManager::GetInstance()->UpdatePatchAllProgress();

	return !g_patcher->IsStoped();
}


#ifdef GATHER_PATCH_INFO
bool FileProgress(float p, float fFileSize = 0.0f)
{
	if(!g_patcher->IsStoped())
		g_fFileDownloadRatio = p;

	// 실제 다운로드한 사이즈를 구한다

	g_fTotalDownloadDataSize += fFileSize;

#ifdef _DEBUG
	int te=0;	// For Debug Point
	if( fFileSize != 0 )
	{
		te++;
	}
#endif 


	CServerManager::GetInstance()->UpdatePatchCurProgress();

	return !g_patcher->IsStoped();
}
#else GATHER_PATCH_INFO
bool FileProgress(float p)
{
	if(!g_patcher->IsStoped())
		g_fFileDownloadRatio = p;

	CServerManager::GetInstance()->UpdatePatchCurProgress();

	return !g_patcher->IsStoped();
}
#endif GATHER_PATCH_INFO


DWORD WINAPI ThreadProc( LPVOID lpParameter )
{
	ThreadParam* pThreadParam = (ThreadParam*)lpParameter;

	//static_cast<CMainDlg*>(pThreadParam->pMainDlg)->NotifyStartButtonEnable(FALSE);

	g_patcher = new KPatcher;
	CServerManager::GetInstance()->SetPatchThreadStatus(CServerManager::ePatchThread_Status_Ready);

	// 패치 서버 경로를 만든다.
	std::string patchpath = "http://" + pThreadParam->strPatchAddr;// + pThreadParam->strPatchPath;

#ifdef GATHER_PATCH_INFO
	CTime kPatchStartTime = CTime::GetCurrentTime();
	g_wstrPatchStartTime = (const wchar_t*)kPatchStartTime.Format(_T("%Y-%m-%d %H:%M:%S"));
#endif GATHER_PATCH_INFO


#ifndef _IN_HOUSE_
	if(g_patcher->Verify("ELSWORD.exe") == false)
	{
		Log( "패쳐 위치 잘못됨" );

		//"파일에 접근할 수 없습니다" 라는 문구 출력
		CString strMessage1, strMessage2, strMessage3;
		strMessage1.LoadString(IDS_FILEACCESSERROR_PART1_MESSAGE);
		strMessage2.LoadString(IDS_FILEACCESSERROR_PART2_MESSAGE);

		strMessage3 = strMessage1;
		strMessage3 += _T("ELSWORD.exe");
		strMessage3 += strMessage2;

		pThreadParam->pMainDlg->MessageBox(strMessage3, _T("Elsword Launcher"), MB_OK);
		::PostMessage( g_hWnd, WM_CLOSE, 0, 0 );
		delete g_patcher;
		g_patcher = NULL;

		return -1;
	}
#endif

	// 레지스트리에 엘소드 경로 추가
	HKEY hKey;
	long lError = RegOpenKeyExW( HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\艾尔之光", 0, KEY_READ, &hKey );
	if( lError != ERROR_SUCCESS )
	{
		long lError = RegCreateKeyW( HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\艾尔之光", &hKey );
		if( lError == ERROR_SUCCESS )
		{
			std::wstring wstrPath = g_patcher->GetLauncherPath();
			RegSetValueExW( hKey, L"Launcher", 0, REG_SZ, (LPBYTE)&wstrPath[0], wstrPath.size() * sizeof( wstrPath[0] ) );
			RegCloseKey( hKey );
		}
	}

	// 세션 열기 
	if(g_patcher->OpenSession("ELSWORD_PATCHER") == false)
	{
		LogError( "Open Internet Session Fail" );

		//"서버에 접속할 수 없습니다" 라는 문구 출력
		CString strMessage;
		strMessage.LoadString(IDS_SERVERERROR_MESSAGE);
		pThreadParam->pMainDlg->MessageBox(strMessage, _T("Elsword Launcher"), MB_OK);
		::PostMessage( g_hWnd, WM_CLOSE, 0, 0 );
		delete g_patcher;
		g_patcher = NULL;

		return -1;
	}	

	Log( "HTTP 열기에 성공했습니다." );

	// 접속 
	int retry = 0;
	while(false ==g_patcher->Connect(patchpath))
	{
		if(MAX_RETRY == ++retry)
		{
			std::string message = patchpath + " 서버 접속에 실패했습니다.";
			Log( message.c_str() );

			//"서버에 접속할 수 없습니다" 라는 문구 출력
			CString strMessage;
			strMessage.LoadString(IDS_SERVERERROR_MESSAGE);

			pThreadParam->pMainDlg->MessageBox(strMessage, _T("Elsword Launcher"), MB_OK);
			::PostMessage( g_hWnd, WM_CLOSE, 0, 0 );
			delete g_patcher;
			g_patcher = NULL;

			return -1;
		}
		::Sleep( MAX_DELAY );
	}

	//패치인포
	Log( "버전 파일 다운로드" );
	retry=0;
	while(false ==g_patcher->GetServerPatchInfo())
	{
		g_patcher->Reconnect();
		if(MAX_RETRY == ++retry)
		{
			Log( "버전 파일 다운로드 실패" );

			//"패치정보를 받는 도중에 에러가 발생하였습니다. 인터넷이 불안정하여 발생할 수도 있습니다."
			CString strMessage;
			strMessage.LoadString(IDS_PATCHINFOERROR_MESSAGE);

			pThreadParam->pMainDlg->MessageBox(strMessage, _T("Elsword Launcher"), MB_OK);
			::PostMessage( g_hWnd, WM_CLOSE, 0, 0 );
			delete g_patcher;
			g_patcher = NULL;

			return -1;
		}
	}	
	Log( "버전 파일 다운로드 성공" );


#ifdef GATHER_PATCH_INFO
	CTime kBeforePatcherCompareTime = CTime::GetCurrentTime();
	// int 형 범위를 안 넘어가는 시간 내라는 가정이 있음 //
	g_iDueSec_BeforePatcherCompare = (int)( kBeforePatcherCompareTime.GetTime() - kPatchStartTime.GetTime() );
#endif GATHER_PATCH_INFO


#ifndef _DEBUG
	Log( "패쳐 비교" );

#ifdef GATHER_PATCH_INFO
	if(g_patcher->CheckPatcherChange(g_uiPatcherAdler32, g_uiPatcherSize, g_uiPatcherFiletime))
#else GATHER_PATCH_INFO
	if(g_patcher->CheckPatcherChange())
#endif GATHER_PATCH_INFO
	{
		retry=0;
		while(false ==g_patcher->UpdatePatcher())
		{
			if(MAX_RETRY == ++retry)
			{
				Log( "패쳐 다운로드 실패" );

				//"Elsword Launcher 업데이트가 실패하였습니다."
				CString strMessage;
				strMessage.LoadString(IDS_LAUNCHERUPDATEERROR_MESSAGE);

				pThreadParam->pMainDlg->MessageBox(strMessage, _T("Elsword Launcher"), MB_OK);
				::PostMessage( g_hWnd, WM_CLOSE, 0, 0 );
				delete g_patcher;
				g_patcher = NULL;

				return -1;
			}
			::Sleep( MAX_DELAY );
		}

		Log( "패쳐 다운로드 성공" );

		//"Elsword Launcher가 업데이트 되었습니다. 다시 시작하겠습니다." 라는 문구 출력
		CString strMessage;
		strMessage.LoadString(IDS_RERUN_MESSAGE);

		if( false == g_patcher->RunPatcher("1"))
		{
			Log( "패쳐 실행 실패" );

			//"Elsword Launcher를 다시 시작할 수 없습니다." 라는 문구 출력
			CString strMessage;
			strMessage.LoadString(IDS_RERUNERROR_MESSAGE);

			pThreadParam->pMainDlg->MessageBox(strMessage, _T("Elsword Launcher"), MB_OK);
			::PostMessage( g_hWnd, WM_CLOSE, 0, 0 );
			delete g_patcher;
			g_patcher = NULL;

			return -1;
		}
		::PostMessage( g_hWnd, WM_CLOSE, 0, 0 );
		delete g_patcher;
		g_patcher = NULL;

		return -1;
	}

#endif

#ifdef GATHER_PATCH_INFO
	CTime kEndPatcherTime = CTime::GetCurrentTime();
	// int 형 범위를 안 넘어가는 시간 내라는 가정이 있음 //
	g_iDueSec_Patcher = (int)( kEndPatcherTime.GetTime() - kBeforePatcherCompareTime.GetTime() );
#endif GATHER_PATCH_INFO

	Log( "로컬 정보 생성");

	if(g_patcher->GetLocalPatchInfo() == false)
	{
		Log( "로컬 정보 생성 실패");

		//"패치정보를 받는 도중에 에러가 발생하였습니다. 인터넷이 불안정하여 발생할 수도 있습니다."
		CString strMessage;
		strMessage.LoadString(IDS_PATCHINFOERROR_MESSAGE);

		pThreadParam->pMainDlg->MessageBox(strMessage, _T("Elsword Launcher"), MB_OK);
		::PostMessage( g_hWnd, WM_CLOSE, 0, 0 );
		delete g_patcher;
		g_patcher = NULL;

		return -1;
	}

	// Server와 Local 정보 비교하여 다른 파일 리스트 뽑아내고 
	// 관련 파일들 전체 용량 합 구함 
	g_patcher->Diff();

#ifdef GATHER_PATCH_INFO
	CTime kEndLocalInfoTime = CTime::GetCurrentTime();
	// int 형 범위를 안 넘어가는 시간 내라는 가정이 있음 //
	g_iDueSec_LocalInfo = (int)( kEndLocalInfoTime.GetTime() - kEndPatcherTime.GetTime() );

	
	g_fTotalDownloadFileSize = g_patcher->GetTotalSize();
	g_iTotalDownloadFileCnt = g_patcher->GetTotalCount();
#endif GATHER_PATCH_INFO
	
	if(g_patcher->IsPatchComplete() == false)
	{
		// 다운로드 윈도우 띄우자 
		//::ShowWindow( g_hWnd, SW_SHOW );
		//::SetWindowPos( g_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
		//::SetFocus( g_hWnd );

		pThreadParam->pPatchReadyDlg->SendMessage(CPatchReadyDlg::WM_CLOSE_EVENT, 0, 0);
		CServerManager::GetInstance()->SetPatchThreadStatus(CServerManager::ePatchThread_Status_Start);

		while(g_patcher->IsPatchComplete() == false && g_patcher->IsStoped()==false)
		{
			g_CurFilename = g_patcher->GetFirstFileName();

			g_fFileDownloadRatio = 0;
			int retry=0;

			while(1)
			{
				std::string strErrorInfo;
				KPatcher::EPatcherErrorType ePatcherError = g_patcher->ProcessOneFile(TotalProgress,FileProgress, strErrorInfo);


				if(ePatcherError == KPatcher::ePatcherError_Type_Success)
				{
					break;
				}


#ifdef GATHER_PATCH_INFO
				g_iAdditionalDownload++;
#endif GATHER_PATCH_INFO

				
				g_patcher->Reconnect();

				if(MAX_RETRY == ++retry)
				{
					if(ePatcherError == KPatcher::ePatcherError_Type_Connect)
					{
						//"서버에 접속할 수 없습니다" 라는 문구 출력
						CString strMessage;
						strMessage.LoadString(IDS_SERVERERROR_MESSAGE);

						pThreadParam->pMainDlg->MessageBox(strMessage, _T("Elsword Launcher"), MB_OK);
						::PostMessage( g_hWnd, WM_CLOSE, 0, 0 );
						delete g_patcher;
						g_patcher = NULL;
						return -1;
					}
					else if(ePatcherError == KPatcher::ePatcherError_Type_FileAccess)
					{
						//"파일에 접근할 수 없습니다" 라는 문구 출력
						CString strMessage1, strMessage2, strMessage3;
						strMessage1.LoadString(IDS_FILEACCESSERROR_PART1_MESSAGE);
						strMessage2.LoadString(IDS_FILEACCESSERROR_PART2_MESSAGE);

						std::wstring wstrFileName;
						ConvertCharToWCHAR(wstrFileName, strErrorInfo.c_str());

						strMessage3 = strMessage1;
						strMessage3 += wstrFileName.c_str();
						strMessage3 += strMessage2;

						pThreadParam->pMainDlg->MessageBox(strMessage3, _T("Elsword Launcher"), MB_OK);
						::PostMessage( g_hWnd, WM_CLOSE, 0, 0 );
						delete g_patcher;
						g_patcher = NULL;
						return -1;
					}
					else if(ePatcherError == KPatcher::ePatcherError_Type_InvalidFile)
					{
						//"파일을 다운로드 도중에 에러가 발생하였습니다. 인터넷이 불안정하여 발생할 수도 있습니다."
						CString strMessage1, strMessage;
						strMessage1.LoadString(IDS_FILEDOWNLOADERROR_MESSAGE);

						std::wstring wstrError;
						ConvertCharToWCHAR(wstrError, strErrorInfo.c_str());

						strMessage = strErrorInfo.c_str();
						strMessage += strMessage1;

						pThreadParam->pMainDlg->MessageBox(strMessage, _T("Elsword Launcher"), MB_OK);
						::PostMessage( g_hWnd, WM_CLOSE, 0, 0 );
						delete g_patcher;
						g_patcher = NULL;
						return -1;
					}

					continue;
				}
			}	
		}
	}
	else
	{
		TotalProgress(1.0f);
	}


#ifdef GATHER_PATCH_INFO
	CTime kEndClientDownLoad = CTime::GetCurrentTime();
	// int 형 범위를 안 넘어가는 시간 내라는 가정이 있음 //
	g_iDueSec_ClientDownload = (int)( kEndClientDownLoad.GetTime() - kEndLocalInfoTime.GetTime() );
#endif GATHER_PATCH_INFO

	if(g_patcher->IsStoped() == true)
	{
		//Log( "패치 프로그램을 종료합니다." );
		//::PostMessage( g_hWnd, WM_CLOSE, 0, 0 );
		delete g_patcher;
		g_patcher = NULL;

		CServerManager::GetInstance()->SetPatchThreadStatus(CServerManager::ePatchThread_Status_Init);

		//g_patcher가 완전히 종료되면 시작버튼 활성화
		//pThreadParam->pMainDlg->NotifyStartButtonEnable(TRUE);

		_endthreadex(0);

		return 0;
	}

#ifdef GATHER_PATCH_INFO
	WritePatchInfoToFile();
	//Sleep(100);
	//LoadPatchInfoFromFile();
#endif GATHER_PATCH_INFO
	
	// 클라이언트 실행 
	std::string arg = pThreadParam->strZoneAddr+" "+pThreadParam->strZonePort;
	if(g_patcher->RunClient("x2.exe", arg) == false)
	{
		Log( "엘소드 실행에 실패했습니다." );

		//"게임 실행에 실패하였습니다.
		CString strMessage1, strMessage;
		strMessage.LoadString(IDS_GAMERUNERROR_MESSAGE);

		pThreadParam->pMainDlg->MessageBox(strMessage, _T("Elsword Launcher"), MB_OK);
		::PostMessage( g_hWnd, WM_CLOSE, 0, 0 );
		delete g_patcher;
		g_patcher = NULL;

		return -1;
	}

	Log( "엘소드 실행에 성공했습니다." );
	::PostMessage( g_hWnd, WM_CLOSE, 0, 0 );


	delete g_patcher;
	g_patcher = NULL;

	return 0;
	
}