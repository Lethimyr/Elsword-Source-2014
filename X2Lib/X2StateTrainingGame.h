#pragma once

class CX2StateTrainingGame : public CX2StateDungeonGame
{
public:
	enum STATE_TRAINING_GAME_UI_CUSTOM_MSG
	{
		STGUCM_NPC_TYPE = 400,
		STGUCM_NPC_COUNT,
		STGUCM_NPC_LEVEL,
		STGUCM_NPC_AGGRESIVE,
		STGUCM_RESET,
		STGUCM_COMBO_LIST,
		STGUCM_EXIT,
	};
	
	
public:
	CX2StateTrainingGame(void);
	virtual ~CX2StateTrainingGame(void);


	virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );

	virtual bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual bool UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	virtual bool QuitGame();


	bool Handler_EGS_LEAVE_ROOM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_PLAY_TIME_OUT_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_REMAINING_PLAY_TIME_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_END_TC_GAME_REQ( bool bSuccess );
	bool Handler_EGS_END_TC_GAME_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual bool Handler_EGS_END_GAME_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_SET_TC_REMAINING_TIME_REQ( bool bEnable );
	bool Handler_EGS_SET_TC_REMAINING_TIME_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


#ifdef OLD_COMBO_LIST_UI
	//{{AFX
	void OpenComboListWindow( bool bOpen );
	bool GetIsOpenComboListWindow()
	{
		if( m_pDLGComboList != NULL )
		{
			return m_pDLGComboList->GetShow();
		}
		
		return false;
	}
	//}}AFX

	void CreateComboListUI( CKTDGUIStatic* pStatic, CX2Unit::UNIT_TYPE eUnitType, CX2Unit::UNIT_CLASS eUnitClass );
	void AddComboPicture( CKTDGUIStatic* pStatic, int iStateID, CX2TrainingGameUI::COMMAND_PICTURE eCommandKey, D3DXVECTOR2 vPos, D3DXVECTOR2 vSize );
	void SetPictureTex( CKTDGUIStatic::CPictureData* pPicture, CX2TrainingGameUI::COMMAND_PICTURE eKey );

#endif OLD_COMBO_LIST_UI


	void ResetUnit();

	virtual void PopTalkBox( UidType iUnitUID_, const WCHAR* pWstrMsg_, 
		const D3DXCOLOR& coTextColor_, const D3DXCOLOR& coOutColor_, const D3DXCOLOR& coBackColor_ );

private:
	float 	m_fRemainTimeToGameEnd;

	map< int, int > m_mapComboList;	// stateID, picture index
	
	CKTDGUIDialogType m_pDLGComboList;

	CX2UIUnit*		m_pUINPC;

	bool			m_bSend_EGS_END_TC_GAME_REQ;
};

