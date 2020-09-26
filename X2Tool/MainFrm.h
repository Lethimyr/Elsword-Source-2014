#pragma once

/**
	@file	MainFrm.h
	@author	Wonpok
	@brief	CMainFrame 클래스의 인터페이스
		
	@date	JHKang on 2013/02/19,
			- 기능 파악
			- 수정 작업( 주석처리, 블럭 나누기, 가로 넓이 지정 )
*/

#pragma region Includes
#include "ClassView.h"
#include "PropertiesWnd.h"
#include "TriggerPropertiesWnd.h"
#include "Resource.h"
#include "SelectToolUnit.h"
#include "DungeonInfo.h"
#include "CameraInfo.h"
#include "LineStartPositionInfo.h"
#include "LineBaseEditor.h"
#include "ToolStartNotice.h"
#include "EditWorld.h"
#include "EditDungeon.h"
#include "TriggerEdit.h"
#pragma endregion MainFrm에 사용되는 기능들

#pragma region CMainFrame
class CMainFrame : public CFrameWndEx
{
	#pragma region protected
protected:
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);
	#pragma endregion function
	
	#pragma region public
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);
		
	void CreateDockingWindowAfter();
	void OnResetMain( bool bEditLine, bool bEditWorld, bool bViewLine, bool bViewWorld, bool bEditTrigger );

	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	#pragma endregion function

	#pragma region public
public:  // 컨트롤 모음이 포함된 멤버입니다.
	CMFCMenuBar				m_wndMenuBar;				/// 메뉴바
	CMFCToolBar				m_wndToolBar;				/// 툴바
	CMFCStatusBar			m_wndStatusBar;				/// 상태바
	CMFCToolBarImages		m_UserImages;				/// 툴바 이미지
	CClassView				m_wndClassView;				/// 뷰 클래스( 던전 리스트 )
	CPropertiesWnd			m_wndProperties;			/// 속성창
	CTriggerPropertiesWnd	m_wndTriggerProperties;		/// 트리거 속성창
	CMFCShellTreeCtrl		m_wndTree;					/// 트리 컨트롤
	CDungeonInfo			m_wndDungeonInfo;			/// 던전 정보
	CCameraInfo				m_wndCameraInfo;			/// 카메라 정보
	CLineStartPositionInfo	m_wndStartPosition;			/// 라인정보
	CLineBaseEditor			m_DlgLineBaseEditor;		/// 라인 에디터
	CEditWorld				m_DlgWorldEditor;			/// 월드 에디터
	CEditDungeon			m_DlgDungeonEditor;			/// 던전 에디터
	CSelectToolUnit			m_DlgSelectUnit;			/// 유닛 선택

	CTriggerEdit			m_DlgTriggerEdit;			/// 배경팀을 위한 트리거 수정창
	#pragma endregion variable 다양한 컨트롤들

	#pragma region protected
protected:
	#pragma endregion variable
	
	#pragma region afx_msg
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	DECLARE_MESSAGE_MAP()

public:	
	afx_msg void OnEditLinemap();
	afx_msg void OnUpdateEditLinemap(CCmdUI *pCmdUI);
	afx_msg void OnEditWorld();
	afx_msg void OnUpdateEditWorld(CCmdUI *pCmdUI);
	afx_msg void OnViewLine();
	afx_msg void OnUpdateViewLine(CCmdUI *pCmdUI);
	afx_msg void OnViewLineInfo();
	afx_msg void OnUpdateViewLineInfo(CCmdUI *pCmdUI);
	afx_msg void OnViewWorld();
	afx_msg void OnUpdateViewWorld(CCmdUI *pCmdUI);
	afx_msg void OnDropFiles( HDROP hDropInfo );
	afx_msg void OnWorldObjectMove();
	afx_msg void OnWorldobjectRotation();
	afx_msg void OnWorldobjectScale();
	afx_msg void OnViewWorldObject();
	afx_msg void OnUpdateViewWorldobject(CCmdUI *pCmdUI);
	void OnViewWorldObjectSelected();
	afx_msg void OnEditDungeon();
	afx_msg void OnViewLineNumber();
	afx_msg void OnViewLineSetNumber();
	afx_msg void OnViewLineCameraNumber();
	afx_msg void OnViewLineTriggerNumber();
	afx_msg void OnViewWaterInfo();
	afx_msg void OnTriggerBck();
	#pragma endregion 생성된 메시지 맵 함수
};
#pragma endregion 메인 프레임