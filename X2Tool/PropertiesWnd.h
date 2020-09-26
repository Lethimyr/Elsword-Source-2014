
#pragma once

#include "UserProperty.h"

class CPropertiesToolBar : public CMFCToolBar
{
public:
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};



class CPropertiesWnd : public CDockablePane
{
// 생성입니다.
public:
	CPropertiesWnd();

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
	CComboBox m_wndObjectCombo;
	CPropertiesToolBar m_wndToolBar;
	//CMFCPropertyGridCtrl m_wndPropList;
	CUserPropertyGrid m_wndPropList;

// 구현입니다.
public:
	virtual ~CPropertiesWnd();

	void UpdateProp(int iFlag);
	CMFCPropertyGridProperty *GetProp(int iData);
	void SetPropValue(int iData, const COleVariant& varValue)
	{
		CMFCPropertyGridProperty *pProp = GetProp( iData );
		if( pProp != NULL )
		{			
			pProp->SetOriginalValue(varValue);
			pProp->SetValue(varValue);	

			switch(iData)
			{
			case PI_EW_WORLD_LAYER_COLOR_R:
			case PI_EW_WORLD_LAYER_COLOR_G:
			case PI_EW_WORLD_LAYER_COLOR_B:
			case PI_EW_WORLD_LAYER_COLOR_A:
			case PI_EW_WORLD_FOG_NEAR_X:
			case PI_EW_WORLD_FOG_FAR_X:
			case PI_EW_WORLD_FOG_NEAR_Y:
			case PI_EW_WORLD_FOG_FAR_Y:
			case PI_EW_WORLD_FOG_COLOR_R:
			case PI_EW_WORLD_FOG_COLOR_G:
			case PI_EW_WORLD_FOG_COLOR_B:
			case PI_EW_WORLD_FOG_DENSITY:
			case PI_EW_WORLD_UNIT_COLOR_R:
			case PI_EW_WORLD_UNIT_COLOR_G:
			case PI_EW_WORLD_UNIT_COLOR_B:
			case PI_EW_WORLD_COLOR_R:
			case PI_EW_WORLD_COLOR_G:
			case PI_EW_WORLD_COLOR_B:
			case PI_EW_WORLD_SKYDOME_COLOR_R:
			case PI_EW_WORLD_SKYDOME_COLOR_G:
			case PI_EW_WORLD_SKYDOME_COLOR_B:
			case PI_EW_WORLD_SKYDOME_COLOR_A:
			case PI_EW_WORLD_SKYDOME_FOG_COLOR_R:
			case PI_EW_WORLD_SKYDOME_FOG_COLOR_G:
			case PI_EW_WORLD_SKYDOME_FOG_COLOR_B:
			case PI_EW_WORLD_SKYDOME_FOG_NEAR_X:
			case PI_EW_WORLD_SKYDOME_FOG_FAR_X:
			case PI_EW_WORLD_SKYDOME_FOG_NEAR_Y:
			case PI_EW_WORLD_SKYDOME_FOG_FAR_Y:
			case PI_EW_WORLD_SKYDOME_FOG_DENSITY:
			case PI_EW_WORLD_OBJECT_COLOR_R:
			case PI_EW_WORLD_OBJECT_COLOR_G:
			case PI_EW_WORLD_OBJECT_COLOR_B:
			case PI_EW_WORLD_OBJECT_COLOR_A:
			case PI_EW_WORLD_OBJECT_FOG_COLOR_R:
			case PI_EW_WORLD_OBJECT_FOG_COLOR_G:
			case PI_EW_WORLD_OBJECT_FOG_COLOR_B:
			case PI_EW_WORLD_OBJECT_FOG_NEAR_X:
			case PI_EW_WORLD_OBJECT_FOG_FAR_X:
			case PI_EW_WORLD_OBJECT_FOG_NEAR_Y:
			case PI_EW_WORLD_OBJECT_FOG_FAR_Y:
			case PI_EW_WORLD_OBJECT_FOG_DENSITY:
				{
					CUserPropertyGridSlider *pPropSlider = (CUserPropertyGridSlider*)pProp;
					pPropSlider->UpdateSlider();
				}
				break;
			default:
				break;
			}
		}
	}

	int GetEnumIndex(ENUM_TYPE enumType, wstring wstrEnum);
	WCHAR *GetEnumString(ENUM_TYPE enumType, int iVal);

	void UpdateWorldBase();
	void UpdateWorldFog();
	void UpdateWorldlayer();
	void UpdateWorldSkyDome();
	void UpdateWorldObject();

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
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	DECLARE_MESSAGE_MAP()

	void InitPropList();
	void SetPropListFont();
};

