#include "stdafx.h"
#include "X2ChinaLauncher.h"
#include "ServerManager.h"
#include "tinyxml/tinyxml.h"
#include "ThreadDefs.h"
#include "patchthread.h"
#include "PatchReadyDlg.h"
#include <wininet.h>
#include "MainDlg.h"
#include "Config.h"
#include "Util.h"


ImplementSingleton(CServerManager)

CServerManager::CServerManager()
{
	//RECT rc1= {422, 526, 422+23, 526+194};
	//RECT rc2= {450, 524, 450+148, 524+194};
	//RECT rc3= {610, 526, 610+23, 526+194};
	//RECT rc4= {640, 524, 640+148, 524+194};

	//m_TeleComServer.Init(rc1, rc2);
	//m_CNCServer.Init(rc3, rc4);
	
	m_TeleComServer.Init(CPoint(426, 538-123+5), CPoint(451-6, 538-123+5), IDC_SCROLLBAR_LEFTVIEW, IDC_SCROLLBAR_LEFTBUTTON, IDC_SCROLLELEMENT_LEFTVIEW, IDB_SCROLLELEMENT_LEFTVIEW_BITMAP);
	m_CNCServer.Init(CPoint(764, 538-123+5), CPoint(614+8, 538-123+5), IDC_SCROLLBAR_RIGHTVIEW, IDC_SCROLLBAR_RIGHTBUTTON, IDC_SCROLLELEMENT_RIGHTVIEW, IDB_SCROLLELEMENT_RIGHTVIEW_BITMAP);


	m_hThread  = NULL;

	m_pPatchReadyDlg = NULL;

	m_ePatchThreadStatus = ePatchThread_Status_Init;

	m_pThreadParam = new ThreadParam;

	m_iCurServerID = -1;

	m_iCurPatchServerID = -1;

	m_hCloseEvent = NULL;

	m_pParentWnd = NULL;
}

CServerManager::~CServerManager()
{
	//m_pPatchReadyDlg->EndDialog(0);
	//delete m_pPatchReadyDlg;

	delete m_pThreadParam;

	SetServerIDToSaveFile(m_iCurPatchServerID);
}

BOOL CServerManager::Load(CWnd* pParentWnd)
{
	m_pParentWnd = pParentWnd;

#if 0
	TiXmlDocument docLocal("TestServer/srvlist_test.xml");

	if(!docLocal.LoadFile())
		return FALSE;

#else
	TiXmlDocument docLocal;

	HINTERNET hInternet, hHttp;
	HINTERNET hReq;
	DWORD size;
	DWORD dwRead;

	const int BUF_SIZE = 2048;
	char buf[BUF_SIZE+1] = {0,};
	std::string data;

	hInternet = ::InternetOpenA("Game Server List", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if(!hInternet)
	{
		::HttpEndRequestA(hHttp, NULL, 0, 0);

		return FALSE;
	}
	
	hHttp = ::InternetConnectA(hInternet, g_pcGameServerListXmlAddr, INTERNET_DEFAULT_HTTP_PORT, "", "", INTERNET_SERVICE_HTTP, 0, 0);
	if(!hHttp)
	{
		::HttpEndRequestA(hHttp, NULL, 0, 0);
		::HttpEndRequestA(hInternet, NULL, 0, 0);

		return FALSE;
	}

	hReq = ::HttpOpenRequestA(hHttp, "GET", g_pcGameServerListXmlPath, "HTTP/1.1",NULL,NULL,				
								INTERNET_FLAG_KEEP_CONNECTION|
								INTERNET_FLAG_PRAGMA_NOCACHE|
								INTERNET_FLAG_NO_AUTH|
								//#if defined(KR_SERVICE_PATCHER) || defined(KR_OPENTEST_PATCHER)
								//{{ oasis907 : 김상윤 [2010.8.31] // 
								INTERNET_FLAG_RELOAD|
								INTERNET_FLAG_NO_CACHE_WRITE|
								//}} oasis907 : 김상윤 [2010.8.31] // 
								//#endif
								INTERNET_FLAG_NO_COOKIES,
								NULL);
	
	if(!::HttpSendRequestA(hReq, NULL, 0, NULL, 0))
	{
		if(!::HttpEndRequestA(hReq, NULL, 0, 0))
		{
			// printf( "Error on HttpEndRequest %lu\n", GetLastError() );
		}

		::HttpEndRequestA(hHttp, NULL, 0, 0);
		::HttpEndRequestA(hInternet, NULL, 0, 0);

		return FALSE;
	}

	while(1) 
	{
		::InternetQueryDataAvailable(hReq, &size, 0, 0);
		::InternetReadFile(hReq, buf, BUF_SIZE, &dwRead);

		if(!dwRead)
			break;

		buf[dwRead] = '\0';
		data += buf;
	};

	if(!::InternetCloseHandle(hReq))
	{
		// printf( "Failed to clse Request handle\n" );
	}
	if(!::InternetCloseHandle(hHttp))
	{

	}
	if(!::InternetCloseHandle(hInternet))
	{

	}

	//std::string temp;
	//ConvertWCHARToChar(temp, data);

	//if(!docLocal.LoadBuf(temp.c_str()))
	if(!docLocal.LoadBuf(data.c_str()))
		return FALSE;

#endif

	TiXmlNode*      nodeLocal = NULL;
	TiXmlElement*   fileElementLocal = NULL;

	nodeLocal = docLocal.FirstChild( "server" );
	if(!nodeLocal)
		return FALSE;

	//<patch>
	if(!LoadPatchTag(nodeLocal))
		return FALSE;

	//<zones>
	if(!LoadZonesTag(nodeLocal))
		return FALSE;


	//pParentWnd->Invalidate();

	//
#if 1
	std::map<int, Zone>::iterator it;
	std::map<int, Zone>::iterator iend = m_mapZone.end();

	for(it=m_mapZone.begin(); it!=iend; ++it)
	{
		switch(it->second.iLine)
		{
			//telecom server
			case LINE_TELECOM:
				m_TeleComServer.AddElement(pParentWnd, GetServerNameFromServerID(it->first), (BOOL)it->second.iHot, (BOOL)it->second.iNew, (BOOL)it->second.iOpen ,(BOOL)it->second.iEnable);
				break;
			//cnc
			case LINE_CNC:
				m_CNCServer.AddElement(pParentWnd, GetServerNameFromServerID(it->first), (BOOL)it->second.iHot, (BOOL)it->second.iNew, (BOOL)it->second.iOpen ,(BOOL)it->second.iEnable);
				break;
			default:
				ASSERT(FALSE);
				break;
		}
	}
#else
	m_TeleComServer.AddElement(pParentWnd, _T("test1"), (BOOL)FALSE, FALSE, TRUE);
	m_TeleComServer.AddElement(pParentWnd, _T("test2"), (BOOL)TRUE, FALSE, TRUE);
	m_TeleComServer.AddElement(pParentWnd, _T("test3"), (BOOL)FALSE, TRUE, TRUE);

	m_CNCServer.AddElement(pParentWnd, _T("test4"), (BOOL)FALSE, FALSE, TRUE);
	m_CNCServer.AddElement(pParentWnd, _T("test5"), (BOOL)TRUE, FALSE, TRUE);
	m_CNCServer.AddElement(pParentWnd, _T("test6"), (BOOL)FALSE, TRUE, TRUE);

	

#endif

	//free(buf);

	//디폴트 서버를 선택
	//이전에 선택한 서버가 존재하는지 localinfo.dat에서 확인
	int iSavedServerID = GetServerIDFromSaveFile();

	if(iSavedServerID > -1)
	{
		//wstrSavedElement가 현재도 유효한 서버인지 확인
		if(IsValidServerID(iSavedServerID))
			SetSelectedServer(iSavedServerID);
		else
			SetSelectedServer(m_iDefaultZone);
	}
	else
	{
		SetSelectedServer(m_iDefaultZone);
	}

	return TRUE;
}

const std::string& CServerManager::GetPatchCurProgressFilename()
{
	return g_CurFilename;
}

float CServerManager::GetPatchCurProgressRatio()
{
	return GetFileDownloadRatio();
}

float CServerManager::GetPatchAllProgressRatio()
{
	return GetTotalDownloadRatio();
}

void CServerManager::UpdatePatchCurProgress()
{
	m_pParentWnd->InvalidateRect(&m_PatchCurProgressRegion);
}

void CServerManager::UpdatePatchAllProgress()
{
	m_pParentWnd->InvalidateRect(&m_PatchAllProgressRegion);
}

BOOL CServerManager::LoadPatchTag(TiXmlNode* pkNode)
{
	pkNode = pkNode->FirstChild("patch");

	if(!pkNode)
		return FALSE;

	//<item>
	pkNode = pkNode->FirstChild("item");

	if(!pkNode)
		return FALSE;

	while(pkNode)
	{
		Patch patch;

		pkNode->ToElement()->Attribute("type", &patch.iID);				// TODO: OBT때 "id"로 변경할것. 잘못된 ID로 m_mapPatch 참조시의 널처리도 할 것
		patch.strAddr = pkNode->ToElement()->Attribute("addr");

		m_mapPatch.insert(std::make_pair(patch.iID, patch));

		pkNode = pkNode->NextSibling("item");
	}

	return TRUE;
}

BOOL CServerManager::LoadZonesTag(TiXmlNode* pkNode)
{
	//<zones>의 속성 읽기
	pkNode = pkNode->FirstChild("zones");

	if(!pkNode)
		return FALSE;
	
	pkNode->ToElement()->Attribute("default", &m_iDefaultZone);

	//<zone>
	pkNode = pkNode->FirstChild("zone");

	if(!pkNode)
		return FALSE;
	
	while(pkNode)
	{
		Zone zone;

		ConvertCharToWCHAR(zone.wstrName, pkNode->ToElement()->Attribute("name"), (int)CP_UTF8 );
		pkNode->ToElement()->Attribute("id", &zone.iID);

		TokenizeAddr(pkNode->ToElement()->Attribute("addr"), &zone.vecAddr);
		TokenizePort(pkNode->ToElement()->Attribute("port"), &zone.vecPort);
		
		pkNode->ToElement()->Attribute("patch", &zone.iPatch);
		pkNode->ToElement()->Attribute("line", &zone.iLine);
		pkNode->ToElement()->Attribute("hot", &zone.iHot);
		pkNode->ToElement()->Attribute("new", &zone.iNew);
		pkNode->ToElement()->Attribute("open", &zone.iOpen);
		pkNode->ToElement()->Attribute("enable", &zone.iEnable);
		ConvertCharToWCHAR(zone.wstrHint, pkNode->ToElement()->Attribute("hint"));

		m_mapZone.insert(std::make_pair(zone.iID, zone));

		pkNode = pkNode->NextSibling("zone");
	}

	return TRUE;
}

void CServerManager::TokenizeAddr(std::string strAddr, std::vector<std::string>* pVecAddr)
{
	tokenize(strAddr, ":", pVecAddr);
}

void CServerManager::TokenizePort(std::string strPort, std::vector<std::string>* pVecPort)
{
	//std::vector<std::string> vecToken;

	tokenize(strPort, ":", pVecPort);

	//for(int i=0; i<vecToken.size(); i++)
	//{
	//	pVecPort->push_back(atoi(vecToken[i].c_str()));
	//}
}

void CServerManager::tokenize(const std::string& str,const std::string& delimiters, std::vector<std::string>* pVecToken)
{
	// skip delimiters at beginning.
	std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);

	// find first "non-delimiter".
	std::string::size_type pos = str.find_first_of(delimiters, lastPos);

	while (std::string::npos != pos || std::string::npos != lastPos)
	{
		// found a token, add it to the vector.
		pVecToken->push_back(str.substr(lastPos, pos - lastPos));

		// skip delimiters.  Note the "not_of"
		lastPos = str.find_first_not_of(delimiters, pos);

		// find next "non-delimiter"
		pos = str.find_first_of(delimiters, lastPos);
	}
}

std::wstring CServerManager::GetServerNameFromServerID(int iServerID)
{
	std::map<int, Zone>::iterator it = m_mapZone.find(iServerID);

	if(it== m_mapZone.end())
	{
		return std::wstring();
	}

	return it->second.wstrName;
}

int CServerManager::GetServerIDFromServerName(const std::wstring& wstrServerName)
{
	std::map<int, Zone>::iterator it;
	std::map<int, Zone>::iterator iend = m_mapZone.end();

	for(it=m_mapZone.begin(); it!=iend; ++it)
	{
		if(it->second.wstrName == wstrServerName)
			return it->first;
	}

	return 0;
}

void CServerManager::SetServerIDToSaveFile(int iServerID)
{
	FILE *fp = fopen("./localinfo.dat", "wb+");

	if(!fp)
		return;

	//fprintf(fp, "%d", iServerID);
	//const int BUF_SIZE = sizeof(int);
	//BYTE buf[BUF_SIZE] = {0,};

	//for(int i=0; i<BUF_SIZE; i++)
	//	iTemp = iTemp >> 8;

	fwrite(&iServerID, sizeof(int), sizeof(BYTE), fp);

	fclose(fp);
}

int CServerManager::GetServerIDFromSaveFile()
{
	FILE *fp = fopen("./localinfo.dat", "rb");

	if(!fp)
		return -1;

	int iServerID;
	fread(&iServerID, sizeof(int), sizeof(BYTE), fp);

	fclose(fp);
	return iServerID;
}

BOOL CServerManager::IsValidServerID(int  iServerID)
{
	std::map<int, Zone>::iterator it = m_mapZone.find(iServerID);

	if(it == m_mapZone.end())
		return FALSE;

	return TRUE;
}

void CServerManager::SetSelectedServer(int iServerID)
{
	std::wstring wstrElementName = GetServerNameFromServerID(iServerID);
	std::wstring wstrCurElementName = GetServerNameFromServerID(m_iCurServerID);

	//element가 선택되었음을 알린다.
	m_TeleComServer.NotifyElementSelected(wstrElementName, wstrCurElementName);
	m_CNCServer.NotifyElementSelected(wstrElementName, wstrCurElementName);

	//이미 패치 받고 있는 서버 선택시 return;
	if(GetCurPatchServerName() == iServerID)
	{
		static_cast<CMainDlg*>(m_pParentWnd)->NotifyStartButtonEnable(FALSE);
		return;
	}

	//선택된 server element가 enable/disable 여부에 따라 start button이 enable/disable 되도록 설정
	std::map<int, Zone>::iterator it = m_mapZone.find(iServerID);

	if(it != m_mapZone.end())
	{
		if(it->second.iEnable)
		{
			static_cast<CMainDlg*>(m_pParentWnd)->NotifyStartButtonEnable(TRUE);
		}
		else
		{
			static_cast<CMainDlg*>(m_pParentWnd)->NotifyStartButtonEnable(FALSE);
		}
	}

	m_iCurServerID = iServerID;
}

void CServerManager::SetSelectedServer(const std::wstring& wstrServerName)
{
	SetSelectedServer(GetServerIDFromServerName(wstrServerName));
}

void CServerManager::CreatePatchThread(int iZoneID)
{
	if(m_hThread)
		return;

	//HANDLE hThread;
	DWORD dwThreadID;

	ThreadReady(AfxGetMainWnd()->m_hWnd);

	CServerManager::GetInstance()->ShowPatchReadyDlg();
	SetCurPatchServerName(iZoneID);

	//zone 이름에 맞는 패치 서버 addr와 path를 찾는다.
	SearchPatchServerInfo(iZoneID, m_pThreadParam->strPatchAddr);
	SearchZoneServerInfo(iZoneID, m_pThreadParam->strZoneAddr, m_pThreadParam->strZonePort);

	m_pThreadParam->pMainDlg = static_cast<CMainDlg*>(m_pParentWnd);
	m_pThreadParam->pPatchReadyDlg = m_pPatchReadyDlg;

	if ( NULL == ( m_hThread = _BEGINTHREADEX( NULL, 0,
		ThreadProc,
		(LPVOID)m_pThreadParam, 0, &dwThreadID ) ) )
	{
		//"패치에 실패하였습니다"
		CString strMessage;
		strMessage.LoadString(IDS_PATCHERROR_MESSAGE);

		m_pParentWnd->MessageBox(strMessage, _T("Elsword Launcher"));
	}
	else
	{
		//static_cast<CMainDlg*>(m_pParentWnd)->NotifyStartButtonEnable(FALSE);
	}
}

void CServerManager::DestroyPatchThread()
{
	//CServerManager::GetInstance()->SetPatchThreadStatus(CServerManager::ePatchThread_Status_Destroy);
	//SetTotalDownloadRatio(0.0f);

	//while(1)
	//{
	//	if(CServerManager::GetInstance()->GetPatchThreadStatus() == CServerManager::ePatchThread_Status_Init)
	//	{
	//		return;
	//	}
	//	Sleep(10000);
	//}
	
    #define SAFE_CLOSE_HANDLE(h)  { if (h) { ::CloseHandle(h);  (h) = NULL; } }


	if(g_patcher)
		g_patcher->Stop();

	::ResumeThread( m_hThread );

	SAFE_CLOSE_HANDLE(m_hThread);

	SetFileDownloadRatio(0.0f);
	SetTotalDownloadRatio(0.0f);
}

void CServerManager::SuspendPatchThread()
{
	//CServerManager::GetInstance()->SetPatchThreadStatus(CServerManager::ePatchThread_Status_Suspend);
	//Sleep(500);
	////m_ePatchThreadStatus = ePatchThread_Status_Init;
	::SuspendThread( m_hThread );

	DWORD error = ::GetLastError();

	if( error != ERROR_SUCCESS)
	{
		int i=0;
	}

	//patch thread가 중지할 시간적 여유를 준다.
	Sleep(500);
}

void CServerManager::ResumePatchThread()
{
	//CServerManager::GetInstance()->SetPatchThreadStatus(CServerManager::ePatchThread_Status_Start);
	//Sleep(500);
	::ResumeThread( m_hThread );
}

//BOOL CServerManager::IsPatchThreading()
//{
//
//	//return (m_hThread == NULL) ? FALSE: TRUE;
//}
//BOOL CServerManager::IsPatchStop()
//{
//	return g_patcher.IsStoped();
//}

void CServerManager::ShowPatchReadyDlg()
{
	if(m_pPatchReadyDlg)
	{	
		HidePatchReadyDlg();
		return;
	}
	
	m_pPatchReadyDlg = new CPatchReadyDlg;
	m_pPatchReadyDlg->Create(CPatchReadyDlg::IDD, m_pParentWnd);
	m_pPatchReadyDlg->ShowWindow(SW_SHOW);
}

void CServerManager::HidePatchReadyDlg()
{
	m_pPatchReadyDlg->DestroyWindow();
	delete m_pPatchReadyDlg;
	m_pPatchReadyDlg = NULL;
}

//BOOL CServerManager::IsEnableServer(const std::wstring& wstrServerName)
//{
//	std::map<int, Zone>::iterator it = m_mapZone.find(wstrServerName);
//
//	if(it != m_mapZone.end())
//	{
//		if(it->second.iEnable)
//			return TRUE;
//	}
//
//	return FALSE;
//}

//BOOL CServerManager::IsVisiblePatchReadyDlg()
//{
//	if(!m_pPatchReadyDlg)
//		return FALSE;
//
//	return m_pPatchReadyDlg->IsWindowVisible();
//}

void CServerManager::SearchPatchServerInfo(int iZoneID, std::string& strPatchAddr)
{
	//zone map으로 부터 해당 zone을 찾는다.
	std::map<int, Zone>::iterator itZone = m_mapZone.find(iZoneID);
	ASSERT(itZone != m_mapZone.end());
	if(itZone  != m_mapZone.end())
	{
		//patch map으로 부터 addr과 path를 알아온다.
		int i = itZone->second.iPatch;
		std::map<int, Patch>::iterator itPatch = m_mapPatch.find(itZone->second.iPatch);
		ASSERT(itPatch != m_mapPatch.end());
		if(itPatch != m_mapPatch.end())
			strPatchAddr = itPatch->second.strAddr;
	}
}

void CServerManager::SearchZoneServerInfo(int iZoneID, std::string& strZoneAddr, std::string& strZonePort)
{
	//zone map으로 부터 해당 zone을 찾는다.
	std::map<int, Zone>::iterator itZone = m_mapZone.find(iZoneID);
	ASSERT(itZone != m_mapZone.end());

	//random하게 선택됨
	int iAddr = rand()%itZone->second.vecAddr.size();

	strZoneAddr = itZone->second.vecAddr[iAddr];
	strZonePort = itZone->second.vecPort[iAddr];
}