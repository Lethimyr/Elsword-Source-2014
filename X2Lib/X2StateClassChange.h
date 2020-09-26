#pragma once

//class CX2StateClassChange : public CX2StateCommonBG
//{
//	public:
//
//		enum STATE_CLASS_CHANGE_UI_CUSTOM_MSG
//		{
//			SCCUCM_STATE_CHANGE_VILLAGE = 0,
//		};
//
//	public:
//
//		CX2StateClassChange(void);
//		virtual ~CX2StateClassChange(void);
//
//		HRESULT OnFrameMove( double fTime, float fElapsedTime );
//
//	protected:
//		bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
//
//		bool LastMsgByESC();
//		void LastMsgPopOk();
//
//	private:
//		CX2UnitViewerUI*	m_pUnitViewerUI;
//		CKTDGUIDialogType	m_pDLGClassChange;
//};



class CX2ClassChangePopup
{
public:
	enum CLASS_CHANGE_UI_CUSTOM_MSG
	{
		CCUCM_STATE_CHANGE_VILLAGE = 35200,
	};

#ifdef SERV_SHARING_BANK_QUEST_CASH
	enum HELP_POPUP_TYPE
	{
		HPT_SHARE_BANK = 1,
	};
#endif // SERV_SHARING_BANK_QUEST_CASH

public:
	CX2ClassChangePopup(void);
	virtual ~CX2ClassChangePopup(void);

	HRESULT OnFrameMove( double fTime, float fElapsedTime );

	bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	void SetShow( bool bShow );
#ifdef SERV_SHARING_BANK_QUEST_CASH
	bool SetShow( bool bShow, int iType );
#endif // SERV_SHARING_BANK_QUEST_CASH
	bool GetShow() { return m_bShow; }

private:
	bool m_bShow;
	CX2UnitViewerUI*	m_pUnitViewerUI;
	CKTDGUIDialogType	m_pDLGClassChange;

#ifdef ADD_PLAY_SOUND //±Ë√¢«—
	void PlaySoundChangeClass( CX2Unit::UNIT_TYPE eUnitType );
#endif //ADD_PLAY_SOUND
};
