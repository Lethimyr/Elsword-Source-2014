//////////////////////////////////////////////////////////////////////////
// 1. 데이터 파싱 및 보관
// 2. Get 기능
// ... 이렇게 두개만 있으면 되겠네
// 3. UI 제어 기능도 있어야 하겠네.

#ifdef CUBE_OPEN_IMAGE_MANAGER

#pragma once

class CX2CubeOpenImageManager{
	

	// 큐브 열기 그림 정보
	struct CubeOpenImageInfo{
		std::set<int>				m_setCubeID;	// 큐브 ID 셋
		std::vector<std::wstring>	m_vecImageName;	// 큐브 오픈 이미지들 ex> GACHA_PPORU_05.tga
		std::wstring				m_wstrSoundName;	// 출력 사운드 이름 
		float						m_fChangeTime;	// 이미지 교체 시간
		bool						m_bStringendo;	// 점점 빠르게
		bool						m_bLoop;		// 루프 돌 것인가
		int							m_iStrIndexOpen;// 큐브 열기 위해 우클릭시 나올 스트링. 기본은 809
		int							m_iStrIndexNoKey;// 큐브 열기 위해 우클릭시 나올 스트링. 기본은 
		bool						m_bShowResult;		// 결과 창 보여줄 것인가.
		std::map<int, std::wstring> m_mapResultImageInfo;	// 결과 아이템 ID, 그에 따른 이미지 이름
		bool						m_bHideBar;		// 바 숨길꺼냐?
		bool						m_bShowResultCustom;	// 결과 커스텀 버전 보여줄거냐
#ifdef RNW_EVENT_CUBE_IMAGE_FADE_IN_OUT
		bool						m_bImageFadeInOut;
#endif //#ifdef RNW_EVENT_CUBE_IMAGE_FADE_IN_OUT
		CubeOpenImageInfo()
		{
			m_fChangeTime = 0.f;
			m_bStringendo = false;
			m_bLoop			= true;
			m_iStrIndexOpen = STR_ID_EMPTY;
			m_iStrIndexNoKey = STR_ID_EMPTY;
			m_bShowResult = false;
			m_bHideBar	= false;
			m_bShowResultCustom = false;
#ifdef RNW_EVENT_CUBE_IMAGE_FADE_IN_OUT
			m_bImageFadeInOut = false;
#endif //#ifdef RNW_EVENT_CUBE_IMAGE_FADE_IN_OUT
		}
	};


public:
	CX2CubeOpenImageManager();
	virtual ~CX2CubeOpenImageManager();

	bool		OpenScriptFile( const WCHAR* pFileName );

	bool		AddImageInfo_LUA();

	// 큐브 UI 출력 상황제어
	const bool		IsPlaying() const { return m_bPlaying; }
	void			SetPlaying( bool bVal ) { m_bPlaying = bVal; }

	const bool		IsStart() const { return m_bStart; }
	void			SetStart( bool bVal ) { m_bStart = bVal; }

	const bool		IsSoundPlaying() const { return m_bSoundPlaying;}
	void			SetSoundPlaying( bool bVal ) { m_bSoundPlaying = bVal; }

	bool			IsEventCube( const int iCubeID );

	bool			IsNowResultEventCubePlaying(){ return m_bNowResultEventCubePlaying; }
	void			SetNowResultEventCubePlaying(bool bVal){ m_bNowResultEventCubePlaying = bVal; }

	bool			GetCubeImageName(IN float fProgressOpen, IN const int iCubeID, OUT std::wstring& wstrName);
	bool			GetSoundName(IN const int iCubeID, OUT std::wstring& wstrName);
	bool			GetResultImageName(IN const int iCubeID, IN const int iResultItemID, OUT std::wstring& wstrName);

	const wchar_t*	GetEventCubeOpenString(IN const int iCubeID);
	const wchar_t*	GetEventCubeNoKeyString(IN const int iCubeID);
	bool			IsShowResult(IN const int iCubeID);

	bool			IsHideBarCube(IN const int iCubeID);

	void			AddResultItemID(IN const int iResultItemID ) { m_setResultItemID.insert(iResultItemID); }
#ifdef RNW_EVENT_CUBE_IMAGE_FADE_IN_OUT
	bool	        IsFadeInOutImage(IN const int iCubeID);
#endif //RNW_EVENT_CUBE_IMAGE_FADE_IN_OUT
	const bool		IsResultItemID(IN const int iItemID);

	bool			IsShowResultCustom( IN const int iCubeID );

private:

	bool			 GetCubeImageInfo( IN const int iCubeID, OUT CubeOpenImageInfo* &pCubeImageInfo_ );
	
	map<int, CubeOpenImageInfo*>		m_mapCubeOpenImageInfo;
	typedef map<int, CubeOpenImageInfo*>::iterator MapCubeOpenImageInfoItor;

	// 큐브 UI 출력 상황제어
	int			m_iNowCubeID;
	bool		m_bPlaying;
	bool		m_bStart;
	bool		m_bSoundPlaying;
	bool		m_bNowResultEventCubePlaying;
	bool		m_bShowLastImage;

	std::set<int>	m_setResultItemID;

};


#endif CUBE_OPEN_IMAGE_MANAGER