
#pragma once


class CTriggerPropertiesToolBar : public CMFCToolBar
{
public:
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

#if 0
class CPropertyGridButtonProperty : public CMFCPropertyGridProperty
{
	DECLARE_DYNAMIC(CPropertyGridButtonProperty)

	friend class CMFCPropertyGridCtrl;

	// Construction
public:

	CPropertyGridButtonProperty(const CString& strName, const CString& strFolderName, DWORD_PTR dwData = 0, LPCTSTR lpszDescr = NULL);
	CPropertyGridButtonProperty(const CString& strName, BOOL bOpenFileDialog, const CString& strFileName, LPCTSTR lpszDefExt = NULL,
		DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, LPCTSTR lpszFilter = NULL, LPCTSTR lpszDescr = NULL, DWORD_PTR dwData = 0);
	virtual ~CPropertyGridButtonProperty();

	void SetButtonDown(bool bVal) { m_bButtonIsDown = bVal; }
	bool HasButton() { return CMFCPropertyGridProperty::HasButton(); }
	void SetPropertyGridCtrl(CMFCPropertyGridCtrl* pWndList) { m_pWndList = pWndList; }
	// Overrides
public:
	virtual void OnClickButton(CPoint point);
	virtual void AdjustButtonRect();
	virtual void OnDrawButton(CDC* pDC, CRect rectButton);
	virtual void OnDrawValue(CDC* pDC, CRect rect);
	// Attributes
protected:
	BOOL    m_bIsFolder;

	// File open dialog atributes:
	BOOL    m_bOpenFileDialog; // TRUE - use "File Open/Save" diaog; otherwise - folder selection dialog
	DWORD   m_dwFileOpenFlags;
	CString m_strDefExt;
	CString m_strFilter;

public:
	wstring wstrButtonName;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CPropertyGridCtrl : public CMFCPropertyGridCtrl
{
	DECLARE_DYNAMIC(CPropertyGridCtrl)

	friend class CTriggerPropertiesWnd;
public:
	CPropertyGridCtrl()
	{
		m_pTriggerPropertiesWnd = NULL;
	}
	virtual ~CPropertyGridCtrl()
	{

	}

	void InitPropertiesWnd()
	{
		RemoveAll();
		//Init();
	}

	void SetOwnerClass(CTriggerPropertiesWnd *pWnd) { m_pTriggerPropertiesWnd = pWnd; }
	CTriggerPropertiesWnd *m_pTriggerPropertiesWnd;

protected:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()
	
};
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CTriggerPropertiesWnd : public CDockablePane
{
// 생성입니다.
public:
	DECLARE_DYNAMIC(CTriggerPropertiesWnd)

	CTriggerPropertiesWnd();

	void AdjustLayout();

// 특성입니다.
public:
	void SetVSDotNetLook(BOOL bSet)
	{
		m_wndPropList.SetVSDotNetLook(bSet);
		m_wndPropList.SetGroupNameFullWidth(bSet);
	}

protected:
	CFont m_fntPropList;
	CTriggerPropertiesToolBar m_wndToolBar;
	CMFCPropertyGridCtrl m_wndPropList;

public:
	virtual ~CTriggerPropertiesWnd();

	void InitWorld();
	void InitPropList();
	void UpdateProp(int iFlag);
	CMFCPropertyGridProperty *GetProp(int iData);
	void SetPropValue(int iData, const COleVariant& varValue)
	{
		CMFCPropertyGridProperty *pProp = GetProp( iData );
		if( pProp != NULL )
		{			
			pProp->SetOriginalValue(varValue);
			pProp->SetValue(varValue);	
		}
	}
	void AddTrigger(int iSelectedAddTrigger);

	int	MakeTriggerID(bool bLoop, int iTriggerSeg, int iTriggerData, int iTriggerElement, int Flag, bool bAddGroup = false);
	int GetTriggerID(int iSelectedItem, bool &triggerLoop, int &triggerId, int &triggerDataId, int &triggerElementId, int &FlagId);
	void DeleteTrigger(int triggerId, int triggerDataId, int triggerElementId, int FlagId);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
		
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnExpandAllProperties();
	afx_msg void OnUpdateExpandAllProperties(CCmdUI* pCmdUI);
	afx_msg void OnSortProperties();
	afx_msg void OnUpdateSortProperties(CCmdUI* pCmdUI);
	afx_msg void OnProperties1();
	afx_msg void OnUpdateProperties1(CCmdUI* pCmdUI);
	afx_msg void OnProperties2();
	afx_msg void OnUpdateProperties2(CCmdUI* pCmdUI);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg LRESULT OnPropertyChanged( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()

	
	void SetPropListFont();
};

