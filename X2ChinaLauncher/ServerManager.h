#pragma once

#include "KNCSingleton.h"
#include "ServerList.h"

class TiXmlNode;
class CPatchReadyDlg;
struct ThreadParam;

class CServerManager
{
	DeclareSingleton(CServerManager);

public:
	enum EPatchThreadStatus
	{
		ePatchThread_Status_Init = 0,
		ePatchThread_Status_Ready,
		ePatchThread_Status_Start,
		//ePatchThread_Status_Suspend,
		ePatchThread_Status_Destroy,
	};

	enum Line
	{
		LINE_TELECOM,
		LINE_CNC,
	};

	struct Patch
	{
		int iID;
		std::string strAddr;
	};

	struct Zone
	{
		int iID;
		std::wstring wstrName;
		std::vector<std::string> vecAddr;
		std::vector<std::string> vecPort;
		int iPatch;
		int iLine;
		int iHot;
		int iNew;
		int iOpen;
		int iEnable;
		std::wstring wstrHint;
	};

	CServerManager();
	~CServerManager();

	BOOL Load(CWnd* pParentWnd);
	int GetDefaultServerID() { return m_iDefaultZone; }
	std::wstring GetServerNameFromServerID(int iServerID);
	int GetServerIDFromServerName(const std::wstring& wstrServerName);
	void SetServerIDToSaveFile(int iServerID);
	int GetServerIDFromSaveFile();
	BOOL IsValidServerID(int  iServerID);
	void SetSelectedServer(int iServerID);
	void SetSelectedServer(const std::wstring& wstrServerName);
	int GetSelectedServer() { return m_iCurServerID; }
	int GetSelectedLine() { return m_mapZone[m_iCurServerID].iLine; }
	void SetCurPatchServerName(int iCurPatchServerName) { m_iCurPatchServerID = iCurPatchServerName; }
	int GetCurPatchServerName() { return m_iCurPatchServerID; }

	BOOL IsExecutingGameClient();
	void CreatePatchThread(int iZoneID);
	void DestroyPatchThread();
	void SuspendPatchThread();
	void ResumePatchThread();
	//BOOL IsPatchThreading();
	const std::string& GetPatchCurProgressFilename();
	float GetPatchCurProgressRatio();
	float GetPatchAllProgressRatio();

	void UpdatePatchCurProgress();
	void UpdatePatchAllProgress();

	void SetPatchCurProgressRegion(const RECT& rc) { m_PatchCurProgressRegion = rc; }
	RECT GetPatchCurProgressRegion() { return m_PatchCurProgressRegion; }
	void SetPatchAllProgressRegion(const RECT& rc) { m_PatchAllProgressRegion = rc; }
	RECT GetPatchAllProgressRegion() { return m_PatchAllProgressRegion; }

	void ShowPatchReadyDlg();
	void HidePatchReadyDlg();
	void SetPatchThreadStatus(EPatchThreadStatus eStatus) { m_ePatchThreadStatus = eStatus; }
	EPatchThreadStatus GetPatchThreadStatus() { return m_ePatchThreadStatus; }
	//BOOL IsVisiblePatchReadyDlg();

	void SetCloseEvent(HANDLE hCloseEvent) { m_hCloseEvent = hCloseEvent; }

	//BOOL IsEnableServer(const std::wstring& wstrServerName);

	//서버 리스트 xml 정보 저장
	std::map<int, Patch> m_mapPatch;
	std::map<int, Zone> m_mapZone;
	int m_iDefaultZone;

private:


	BOOL LoadPatchTag(TiXmlNode* pkNode);
	BOOL LoadZonesTag(TiXmlNode* pkNode);
	void TokenizeAddr(std::string strAddr, std::vector<std::string>* pVecAddr);
	void TokenizePort(std::string strPort, std::vector<std::string>* pVecPort);
	void tokenize(const std::string& str,const std::string& delimiters, std::vector<std::string>* pVecToken);

	void SearchPatchServerInfo(int iZoneID, std::string& strPatchAddr);
	void SearchZoneServerInfo(int iZoneID, std::string& strZoneAddr, std::string& strZonePath);

	CWnd* m_pParentWnd;

	//왼쪽 서버 리스트
	CServerList m_TeleComServer;
	//오른쪽 서버 리스트
	CServerList m_CNCServer;

	int m_iCurServerID;//현재 선택된 server id
	int m_iCurPatchServerID;//현재 패치받는 server id

	ThreadParam* m_pThreadParam;
	HANDLE m_hThread;
	
	RECT m_PatchCurProgressRegion;
	RECT m_PatchAllProgressRegion;

	CPatchReadyDlg* m_pPatchReadyDlg;

	EPatchThreadStatus m_ePatchThreadStatus;

	HANDLE m_hCloseEvent;
};
