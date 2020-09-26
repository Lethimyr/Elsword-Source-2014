#pragma once

class CX2StatePVPResult : public CX2StateCommonBG
{
	public:
//#ifdef SERV_PVP_REMATCH
		enum PVP_RESULT_UI_CUSTOM_MSG
		{
			PRUCM_REMATCH_ACCEPT = 0,
			PRUCM_REMATCH_REJECT,
		};
//#endif SERV_PVP_REMATCH

		CX2StatePVPResult(void);
		virtual ~CX2StatePVPResult(void);

		virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
		virtual HRESULT OnFrameRender();

	protected:
		virtual bool UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#ifdef SERV_PVP_REMATCH
		virtual bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif SERV_PVP_REMATCH

		bool Handler_EGS_CHANGE_EQUIPPED_ITEM_IN_ROOM_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool Handler_EGS_RESULT_SUCCESS_REQ();
		bool Handler_EGS_RESULT_SUCCESS_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool Handler_EGS_RESULT_ALL_UNIT_SUCCESS_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		bool Handler_EGS_LEAVE_ROOM_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool Handler_EGS_JOIN_ROOM_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

#ifdef SERV_PVP_REMATCH
		bool Handler_EGS_PVP_REMATCH_REQ( bool bAcceptRematch );
		bool Handler_EGS_PVP_REMATCH_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool Handler_EGS_PVP_REMATCH_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif SERV_PVP_REMATCH

		bool InitUI();
		void UnitViewerProcess( CX2UnitViewerUI* pViewer, int slotNum );

		void SetUnitCard( int index, CX2Unit* pUnit, int rankNum );

	protected:

		CKTDGUIDialogType		m_pDLGBack;
		CKTDGUIDialogType		m_pDLGFront;
#ifdef SERV_PVP_REMATCH
		CKTDGUIDialogType		m_pDLGRematch;
		map< UidType, int >		m_mapSlotIndexByUnitUID;
		bool					m_bCanRematch;
#endif SERV_PVP_REMATCH

		vector< CKTDGUIDialogType > m_pDLGResultInfoList;
		vector< CKTDGUIDialogType > m_pDLGResultCardBackList;
		vector< CKTDGUIDialogType > m_pDLGResultCardFrontList;

		CKTDGUIDialogType		m_pDLGFrontFreeChannel;

		bool				m_bInitUIFail;
		float				m_fPlayBGM;

		float				m_fRemainTime;
		bool				m_bSendRefreshRoomReq;

};
