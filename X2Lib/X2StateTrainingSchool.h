// 2009.01.21 김태완
// 훈련소 UI. 로우와 대화한 뒤 훈련 메뉴 선택으로 진입. 
// 구버전에서 UI 교체, CX2StateHouse 상속에서 CX2StateMenu 상속으로 바꿈.


#pragma once

class CX2StateTrainingSchool : public CX2StateMenu
{
public:
	enum TRAINING_SCHOOL_UI_CUSTOM_MESSAGE
	{
		TSUCM_EXIT = 27000,

		TSUCM_SELECT_TRAIN,
		TSUCM_CATEGORY_BUTTON,
		TSUCM_LIST_UP,
		TSUCM_LIST_DOWN,

		TSUCM_LIST_NAVI_LCLICKED,
		TSUCM_LIST_NAVI_DRAGGING,

		TSUCM_START_TRAIN,

	};

	struct TrainingListSlot
	{
		int				m_iTrainingID;

		TrainingListSlot()
		{
			m_iTrainingID	= -1;
		}
	};


public:
	CX2StateTrainingSchool(void);
	virtual ~CX2StateTrainingSchool(void);

	HRESULT OnFrameMove( double fTime, float fElapsedTime );

	bool MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool OnMouseWheel( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	void LeaveHouse() { Handler_EGS_STATE_CHANGE_VILLAGE_MAP_REQ( SEnum::VMI_RUBEN ); }

protected:

	bool Handler_EGS_STATE_CHANGE_VILLAGE_MAP_REQ( UINT nMapID );
	bool Handler_EGS_STATE_CHANGE_VILLAGE_MAP_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_CREATE_TC_ROOM_REQ( int iTrainingID );
	bool Handler_EGS_CREATE_TC_ROOM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	//void UpdateTrainList();
	void UpdateTrainUI( bool bUpdateNavi = true );
	void ResetTrainUI();
	void UncheckAllTrainList();
	void UpdateTrainDesc( int iTrainingID );
	void UpdateNavi();

	void SelectTrain( int SelectedTrainID );
	void SetRewardItemSlot();


	void StateChangeToVillage();
	
	void SetShowStateDLG( bool bShow );
	//bool ClickMenuBackButton();

	bool LastMsgByESC();
	void LastMsgPopOk();

private:
	int						m_iTrainPerPage;
	int						m_iRewardSlotNum;

	int						m_iTopIndex;
	int						m_iTrainListSize;

	int						m_iPickedTrainID;

	vector< TrainingListSlot >		m_vecTrainingListSlot; 

	CKTDGUIDialogType			m_pDLGUITrainingSchool;  
	CKTDGUIDialogType			m_pDLGSchoolBack;



	// UI NPC
	CX2UIUnit*						m_pUINPC;	
	D3DXVECTOR2						m_vNpcNamePos;
	CX2TFieldNpcShop*				m_pDlgNpcMessage;


	// 마우스 휠 관련
	SHORT				m_SumDelta;
};

