#pragma once

#define FIRSTGROUP 1
#define SECONDGROUP 2

class CXFileCompareMain : public CKTDXStage
{
public:

	bool operator()( const CKTDXDeviceXSkinMesh::MultiAnimFrame* p, const CKTDXDeviceXSkinMesh::MultiAnimFrame* q ) const
	{ 
		if( strcmp(p->Name,q->Name) < 0 )
			return true;
		else
			return false;
	}

	enum COMPARE_EVENT_MSG
	{
		CEM_OPENFIRSTMODELFILE = 0,
		CEM_OPENSECONDMODELFILE,
		CEM_COMPARE,
		CEM_SORTFIRSTMODEL,
		CEM_SORTSECONDMODEL,
		CEM_SELECTFIRSTMODEL,
		CEM_SELECTSECONDMODEL,

		CEM_FIRSTGROUP_CHEKEDED,
		CEM_FIRSTGROUP_UNCHECKED,

		CEM_SECONDGROUP_CHEKEDED,
		CEM_SECONDGROUP_UNCHECKED,

		CEM_SELECTION_CHANGED,

		CEM_LISTBOX_SELECTION,
		CEM_LISTBOX_SELECTION_END, 
		CEM_LISTBOX_ITEM_DOUBLECLICK,

		CEM_MODEL_SORTCHECKED,
		CEM_MODEL_SORTUNCHECKED,

		CEM_ALLSORT,
		CEM_REMOVE_TOP_FRAME_NAME,

		CEM_FILE_TO_REMOVE_FRAME_CHECKED,
		CEM_FILE_TO_REMOVE_FRAME_UN_CHECKED,
	};

public:
	CXFileCompareMain(void);
	~CXFileCompareMain(void);

	virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
	virtual HRESULT OnFrameRender();

	virtual bool MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	virtual HRESULT OnResetDevice();
	virtual HRESULT OnLostDevice();

private:
	bool CatchFile( WPARAM wParam );
	bool Compare();

	bool OpenFirstModelFile();
	bool OpenSecondModelFile();

	void AllSort();

	void ParceFileToRemoveFrame();

private:
	CKTDGUIDialogType			m_pDiaXFileCompare;

	wstring					m_TempFileName;
	wstring					m_FirstFileName;
	wstring					m_SecondFileName;

	CFCCompare*				m_pFCCompare;
	CFCSort*				m_pFCSort;

	bool					m_bSortChecked;

	int						m_SliderValue;

	vector< wstring >		m_vecFileToRemoveFrame;
	bool					m_bFileToRemoveFrame;
};
