#include "stdafx.h"
#include "../X2Lib/X2Main.h"

#ifdef SERV_CLIENT_DIRECT_CONNECT_AUTH_PCBANG
#include "../X2Lib/OnlyGlobal/ID/NetCafe/AESCryptManager.h"
#define WM_PREMIUM_SERVICE                  WM_USER + 5749

CString GetAESCryptManager(CString strPassword)
{
	TCHAR szPassword[256];
	memset(szPassword, 0x00, sizeof(szPassword));	

	CAESCryptManager Mgr;
	Mgr.GetCryptInfo(strPassword.GetBuffer(0), szPassword, sizeof(szPassword));	

	CString strDESPassword = CString(szPassword);
	return strDESPassword;
}

CString GetGameMemoryKey(int nGame)
{
	CString strReturn = "";

	LPCTSTR lpszENC[62] = {
		L"a", L"b", L"c", L"d", L"e", L"f", L"g", L"h", L"i", L"j", L"k", L"l", L"m", L"n", L"o", L"p", L"q", L"r", L"s", L"t", L"u", L"v", L"w", L"x", L"y", L"z",
		L"A", L"B", L"C", L"D", L"E", L"F", L"G", L"H", L"I", L"J", L"K", L"L", L"M", L"N", L"O", L"P", L"Q", L"R", L"S", L"T", L"U", L"V", L"W", L"X", L"Y", L"Z",
		L"1", L"2", L"3", L"4", L"5", L"6", L"7", L"8", L"9", L"0"};

		if(nGame == 0)
			strReturn.Format(L"%s%s%s%s%s%s%s%s_%s%s%s%s%s", lpszENC[15],lpszENC[12],lpszENC[2],lpszENC[11],lpszENC[8],lpszENC[4],lpszENC[13],lpszENC[19],lpszENC[41],lpszENC[32],lpszENC[38],lpszENC[34],lpszENC[29]); 
		else if(nGame == 1)
			strReturn.Format(L"%s%s%s%s%s%s%s", lpszENC[4],lpszENC[11],lpszENC[18],lpszENC[22],lpszENC[14],lpszENC[17],lpszENC[3]);

		return strReturn;
}

CString GetPcbnameProductKey()
{
#define REG_BUFF_SIZE   4096
	CRegKey	regKey;
	TCHAR szBuffer [REG_BUFF_SIZE];
	DWORD dwCount = 0;
	DWORD dwBuffer = 0;

	if (ERROR_SUCCESS != regKey.Open(HKEY_LOCAL_MACHINE, _T("Software\\Mediaweb\\PicaManager\\ClientSettings"), KEY_READ))	
	{
		return "";
	}

	dwCount = REG_BUFF_SIZE + 1;

	if (ERROR_SUCCESS != regKey.QueryValue(szBuffer, L"ProductKey", &dwCount))
	{
		regKey.Close();
		return "";
	}

	regKey.Close();

	return (CString)szBuffer;
}

BOOL SendMessagePmclient(int nWparam, int nLparam)
{
	HWND hpmclient = ::FindWindow(L"pmclient", NULL);
	if(hpmclient)
	{
		DWORD dwResult = 0;
		::SendMessageTimeout( hpmclient, WM_PREMIUM_SERVICE, (WPARAM)nWparam, (LPARAM)nLparam, SMTO_NORMAL, 3000, &dwResult );

		return TRUE;
	}
	return FALSE;
}

//memory read
BOOL GetReadPgmIDMapFile(CString &strPGMID)
{	
	char lc_InfoBuff[512] = {""};
	HANDLE hMapping;
	LPVOID pViewOf;

	hMapping = OpenFileMapping(FILE_MAP_READ | FILE_MAP_WRITE, FALSE, GetGameMemoryKey(0));

	pViewOf = MapViewOfFile(hMapping, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);

	if(pViewOf == NULL) 
	{	
		//메모리 해제 요청
		SendMessagePmclient(0, 0);
		return FALSE;
	}

	int  nRu;

	nRu = strcspn((char*)pViewOf, "\n" );
	strncat(lc_InfoBuff, ((char*)pViewOf), nRu);

	strPGMID = (char*)strdup(lc_InfoBuff);

	strcpy(lc_InfoBuff, "");

	UnmapViewOfFile (pViewOf);
	CloseHandle(hMapping);
	hMapping = NULL;

	//완료후 메모리 해제 요청
	SendMessagePmclient(0, 0);

	return TRUE;
}

#endif //CLIENT_DIRECT_CONNECT_AUTH_PCBANG

#ifdef SERV_CLIENT_DIRECT_CONNECT_AUTH_PCBANG
bool CX2Main::CheckPcBangModule()
{
	HWND hpmclient = ::FindWindow(L"pmclient", NULL);
	HWND hagent = ::FindWindow(L"pmagent", NULL);
	if(hpmclient && hagent)
	{
		//wparam,lparam 두개다 1일경우에만 엘소드임
		SendMessagePmclient(1, 1);
	}

	CString strPGMID;
	if( GetReadPgmIDMapFile(strPGMID) == TRUE)
	{	
		if( GetAESCryptManager(strPGMID) == GetGameMemoryKey(1) )
		{	
			HWND hpmclient = ::FindWindow(L"pmclient", NULL);
			HWND hagent = ::FindWindow(L"pmagent", NULL);
			if(hpmclient && hagent)
			{
				//MessageBox( g_pKTDXApp->GetHWND(), L"게임 혜택 가능", L"OK", MB_OK );
				return true;
			}
			else
			{
				//MessageBox( g_pKTDXApp->GetHWND(), L"게임 혜택 불가능", L"Error", MB_OK );
				return false;
			}
		}
		else
		{
			//MessageBox( g_pKTDXApp->GetHWND(), L"게임 혜택 불가능", L"Error", MB_OK );
			return false;
		}
	}

	return false;

}
bool CX2Main::IsStartPcBang()
{
	bool bCheckNetCafe = false;
	int iIsGameBangType = -1;

	bCheckNetCafe = CheckPcBangModule();

	if ( bCheckNetCafe == true )
	{
		iIsGameBangType = 0;		// 2013.03.05 lygan_조성욱 // 우선 인도네시아에서는 PC 방이면 타입 0만 있다. ( 추가될 가능성 있음 )
	}

	KEGS_CLIENT_DIRECT_CONNECT_AUTH_PCBAG_REQ kPacket;
	kPacket.m_bCheckClinetDirectPcBang = bCheckNetCafe;
	kPacket.m_iIsGameBangType = iIsGameBangType;	// 2013.05.23 lygan_조성욱 // 인도네시아는 PC 방 타입을 클라이언트에서 서버로 알려줘야 한다. ( 그래야 혜택을 받아옴 )

#ifdef SERV_ID_NETMARBLE_PCBANG
	kPacket.m_wstrPublicIP = g_pInstanceData->GetPublicIP();
	kPacket.m_wstrMacAdress = g_pInstanceData->GetMacAdress();
#endif //SERV_ID_NETMARBLE_PCBANG

	g_pData->GetServerProtocol()->SendPacket( EGS_CLIENT_DIRECT_CONNECT_AUTH_PCBAG_REQ, kPacket );

	return bCheckNetCafe;

}
#endif //CLIENT_DIRECT_CONNECT_AUTH_PCBANG