#pragma once

class CX2StateTrainingResult : public CX2StateDungeonResult
{
public:
	enum STATE_TRAINING_RESULT_UI_CUSTOM_MSG
	{
		STSUCM_START_NEXT_TRAINING_OK,
		STSUCM_START_NEXT_TRAINING_CANCEL,
	};


public:
	CX2StateTrainingResult(void);
	virtual ~CX2StateTrainingResult(void);
	
	virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
	virtual bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

protected:



	
};
