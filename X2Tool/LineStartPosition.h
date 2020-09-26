#pragma once
#include "afxwin.h"


// CLineStartPosition 대화 상자입니다.

class CLineStartPosition : public CDialog
{
	DECLARE_DYNAMIC(CLineStartPosition)

	enum START_DIR
	{
		DIR_LEFT,
		DIR_RIGHT,
	};

	enum TEAM_COLOR
	{
		TEAM_NONE,
		TEAM_RED,
		TEAM_BLUE,
	};

	struct STeamStartPos
	{
		int key;
		int iTeamColor;
		int iInitialDir;
		D3DXVECTOR3 vPos;
		int iLineIndex;
		float fGap;

		STeamStartPos()
		{

		}

		STeamStartPos( const int key, const int iTeamColor, const D3DXVECTOR3& vPos, const int iInitialDir, int iLineIndex = 0, float fGap = 0.5f )
		{
			this->key				= key;
			this->iTeamColor		= iTeamColor;
			this->vPos				= vPos;
			this->iInitialDir		= iInitialDir;
			this->iLineIndex		= iLineIndex;
			this->fGap = fGap;
		}
	};

	class CTeamStartPosSort
	{
	public: 
		bool operator() ( const STeamStartPos& lhs, const STeamStartPos& rhs ) const
		{ 
			if( lhs.iTeamColor > rhs.iTeamColor )
				return true;
			else if( lhs.iTeamColor == rhs.iTeamColor && lhs.key < rhs.key )
				return true;
			else
				return false;
		}
	};

public:
	CLineStartPosition(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CLineStartPosition();

	void RebuildStartPosition(int iLineIndex);
	void LoadStartPosition();
	void ResetStartPosition(bool bReset = true);
	void UpdateStartPosition(int iIndex);
	void ViewStartPosition(int iIndex);
	float GetGap(D3DXVECTOR3 vPos, int iLineIndex);
	float FindNewStartPosition(D3DXVECTOR3& vPos, int& iLineIndex);
	int AddStartPosition(D3DXVECTOR3 vPos, int iLineIndex);
	void MoveStartPosition( D3DXVECTOR3 vPos, int iPickedIndex );
	int	GetStartPosIndex(int iTeam, int iIndex);
	void SelectStartPos(int iIndex);
	int GetNextUniqueNPCStartPosKey();
	void ResetNpcStartPosKey();
	void DeleteNpcStartPos(int index);
	void SortStartPosition()
	{
		std::sort( vecTeamStartPos.begin(), vecTeamStartPos.end(), CTeamStartPosSort() );
	}

	virtual BOOL PreTranslateMessage(MSG* pMsg);

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_LINE_STARTPOSITION };

protected:
	
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	vector<STeamStartPos>		vecTeamStartPos;
	bool						m_abKeyUsed[1024];
	int							m_CurrKeyUsed;
	int							m_iSelectedIndex;
	float						m_fAutoGapDist;
	float						m_fAutoGapRate;
	
	bool m_radioRed;
	bool m_radioBlue;
	bool m_radioNpc;
	CListBox m_listRed;
	CListBox m_listBlue;
	CListBox m_listNpc;
	CButton m_CheckHideUser;
	CButton m_CheckHideNpc;


	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnLbnSetfocusListSpRed();
	afx_msg void OnLbnSetfocusListSpNpc();
	CComboBox m_comboTeam;
	CComboBox m_comboDir;
	afx_msg void OnLbnSelchangeListSpRed();
	afx_msg void OnLbnSelchangeListSpBlue();
	afx_msg void OnLbnSelchangeListSpNpc();
	afx_msg void OnBnClickedButtonSpUpdate();
	afx_msg void OnBnClickedButtonSpAuto();
	CComboBox m_comboAutoType;
	CEdit m_editAutoGap;
	afx_msg void OnCbnSelchangeComboAutoType();
	afx_msg void OnEnChangeEditAutoGap();
	afx_msg void OnBnClickedButtonDeleteallnpc();
	afx_msg void OnBnClickedButtonResetkey();
	afx_msg void OnBnClickedButton1();
};
