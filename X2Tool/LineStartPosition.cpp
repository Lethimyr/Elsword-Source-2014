// LineStartPosition.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "X2Tool.h"
#include "LineStartPosition.h"
#include "afxdialogex.h"


// CLineStartPosition 대화 상자입니다.

IMPLEMENT_DYNAMIC(CLineStartPosition, CDialog)

CLineStartPosition::CLineStartPosition(CWnd* pParent /*=NULL*/)
	: CDialog(CLineStartPosition::IDD, pParent)
	, m_radioRed(false)
	, m_radioBlue(false)
	, m_radioNpc(false)
{
	m_fAutoGapDist = 100.f;
	m_fAutoGapRate = 0.2f;	
}

CLineStartPosition::~CLineStartPosition()
{
}

void CLineStartPosition::RebuildStartPosition(int iLineIndex)
{
	CX2ToolMain *pToolMain = (CX2ToolMain*)g_pMain;

	CKTDGLineMap *pLineMap = g_pX2Game->GetWorld()->GetLineMap();
	bool bAllRebuild = false;
	if( iLineIndex < 0 )
	{
		bAllRebuild = true;
	}
		
	for(UINT i=0; i<vecTeamStartPos.size(); ++i)
	{
		STeamStartPos& sp = vecTeamStartPos[i];
		int iLineIndexTemp = sp.iLineIndex;
		if( bAllRebuild == true || iLineIndexTemp == iLineIndex )
		{
			float fGap = GetGap( sp.vPos, iLineIndexTemp );
			if( fGap < 0.f )
			{
				fGap = FindNewStartPosition(sp.vPos, iLineIndexTemp);
			}

			sp.fGap = fGap;
			sp.iLineIndex = iLineIndexTemp;

			const bool	bStartRight		= pLineMap->GetStartRight( sp.key );
			const int	iStartLinaIndex = pLineMap->GetStartLineIndex( sp.key );

			pLineMap->AddStartPos( sp.key, sp.vPos, bStartRight, iStartLinaIndex );

			pToolMain->m_bChangeStartPos = true;
		}
	}

	ResetStartPosition(false);
}

void CLineStartPosition::LoadStartPosition()
{
	CKTDGLineMap *pLineMap = g_pX2Game->GetWorld()->GetLineMap();

	m_CurrKeyUsed = 0;
	for( int i=0; i<sizeof(m_abKeyUsed); i++ )
	{
		m_abKeyUsed[i] = false;
	}

	vecTeamStartPos.clear();
	int iTempInitialDir;
	for(int i=0; i<pLineMap->GetRedTeamStartPosNum(); ++i )
	{
		if( pLineMap->GetRedTeamStartRight(i) == true )
			iTempInitialDir = DIR_RIGHT;
		else
			iTempInitialDir = DIR_LEFT;

		int iLineIndex = pLineMap->GetRedTeamStartLineIndex(i);
		D3DXVECTOR3 vPos = pLineMap->GetRedTeamStartPosition(i);
 		float fGap = GetGap( vPos, iLineIndex );
// 		if( fGap < 0.f )
// 		{
// 			fGap = FindNewStartPosition(vPos, iLineIndex);
// 		}

		vecTeamStartPos.push_back( STeamStartPos( -1, TEAM_RED, vPos, iTempInitialDir, iLineIndex, fGap ) );
	}

	for(int i=0; i<pLineMap->GetBlueTeamStartPosNum(); ++i )
	{
		if( pLineMap->GetBlueTeamStartRight(i) == true )
			iTempInitialDir = DIR_RIGHT;
		else
			iTempInitialDir = DIR_LEFT;

		int iLineIndex = pLineMap->GetBlueTeamStartLineIndex(i);
		D3DXVECTOR3 vPos = pLineMap->GetBlueTeamStartPosition(i);
 		float fGap = GetGap( vPos, iLineIndex );
// 		if( fGap < 0.f )
// 		{
// 			fGap = FindNewStartPosition(vPos, iLineIndex);
// 		}

		vecTeamStartPos.push_back( STeamStartPos( -1, TEAM_BLUE, vPos, iTempInitialDir, iLineIndex, fGap ) );
	}


	const CKTDGLineMap::StartInfoMap& mapStartInfo = pLineMap->GetStartInfoMap();
	CKTDGLineMap::StartInfoMap::const_iterator it;
	for( it=mapStartInfo.begin(); it!=mapStartInfo.end(); ++it )
	{
		if( true == pLineMap->GetStartRight( (int)it->first ) )
			iTempInitialDir = DIR_RIGHT;
		else
			iTempInitialDir = DIR_LEFT;

		int iLineIndex = pLineMap->GetStartLineIndex( (int)it->first );
		D3DXVECTOR3 vPos = pLineMap->GetStartPosition( (int)it->first );
 		float fGap = GetGap( vPos, iLineIndex );
// 		if( fGap < 0.f )
// 		{
// 			fGap = FindNewStartPosition(vPos, iLineIndex);
// 		}

		m_abKeyUsed[ (int)it->first ] = true;
		vecTeamStartPos.push_back( STeamStartPos( (int)it->first, TEAM_NONE, vPos, iTempInitialDir, iLineIndex, fGap ) );
	}
}

void CLineStartPosition::ResetStartPosition(bool bReset)
{	
	CKTDGLineMap *pLineMap = g_pX2Game->GetWorld()->GetLineMap();

	m_listRed.ResetContent();
	m_listBlue.ResetContent();
	m_listNpc.ResetContent();
		
	m_iSelectedIndex = -1;
	if( bReset == true )
	{
		LoadStartPosition();
		//ResetNpcStartPosKey();
	}
	

	WCHAR wstr[100];
	WCHAR wstrRight[10];
	int iRed, iBlue;
	iRed = iBlue = -1;

	for(UINT i=0; i < vecTeamStartPos.size(); ++i)
	{
		STeamStartPos& spos = vecTeamStartPos[i];
		wsprintf( wstrRight, L"(L)" );
		if( spos.iInitialDir == DIR_RIGHT )
		{
			wsprintf( wstrRight, L"(R)" );
		}

		switch( spos.iTeamColor )
		{
		case TEAM_NONE:
			{
				wsprintf(wstr, L"%03d %s", spos.key, wstrRight);
				int iCur = m_listNpc.AddString(wstr);
				m_listNpc.SetItemData(iCur, i);
			}
			break;
		case TEAM_RED:
			{
				++iRed;
				wsprintf(wstr, L"%03d %s", iRed, wstrRight);
				int iCur = m_listRed.AddString(wstr);
				m_listRed.SetItemData(iCur, i);
			}
			break;
		case TEAM_BLUE:
			{
				++iBlue;
				wsprintf(wstr, L"%03d %s", iBlue, wstrRight);
				int iCur = m_listBlue.AddString(wstr);
				m_listBlue.SetItemData(iCur, i);
			}
			break;
		}
	}

	if( bReset == true )
	{
		m_comboTeam.ResetContent();
		m_comboTeam.AddString(L"TEAM_NONE");
		m_comboTeam.AddString(L"TEAM_RED");
		m_comboTeam.AddString(L"TEAM_BLUE");
		m_comboTeam.SetCurSel(0);

		m_comboDir.ResetContent();
		m_comboDir.AddString(L"DIR_LEFT");
		m_comboDir.AddString(L"DIR_RIGHT");
		m_comboDir.SetCurSel(0);

		m_comboAutoType.ResetContent();
		m_comboAutoType.AddString(L"고정거리");
		m_comboAutoType.AddString(L"비율");		
		m_comboAutoType.SetCurSel(0);

		char cBuf[100];
		sprintf(cBuf, "%5.2f", m_fAutoGapDist);
		wstring wstrRate;
		ConvertCharToWCHAR(wstrRate, cBuf);
		m_editAutoGap.SetWindowTextW(wstrRate.c_str());
	}	
}

void CLineStartPosition::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SP_RED, m_listRed);
	DDX_Control(pDX, IDC_LIST_SP_BLUE, m_listBlue);
	DDX_Control(pDX, IDC_LIST_SP_NPC, m_listNpc);
	DDX_Control(pDX, IDC_CHECK1, m_CheckHideUser);
	DDX_Control(pDX, IDC_CHECK2, m_CheckHideNpc);
	DDX_Control(pDX, IDC_COMBO_TEAM, m_comboTeam);
	DDX_Control(pDX, IDC_COMBO_DIR, m_comboDir);
	DDX_Control(pDX, IDC_COMBO_AUTO_TYPE, m_comboAutoType);
	DDX_Control(pDX, IDC_EDIT_AUTO_GAP, m_editAutoGap);
}


BEGIN_MESSAGE_MAP(CLineStartPosition, CDialog)
	ON_BN_CLICKED(IDC_CHECK1, &CLineStartPosition::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &CLineStartPosition::OnBnClickedCheck2)
	ON_LBN_SETFOCUS(IDC_LIST_SP_RED, &CLineStartPosition::OnLbnSetfocusListSpRed)
	ON_LBN_SETFOCUS(IDC_LIST_SP_NPC, &CLineStartPosition::OnLbnSetfocusListSpNpc)
	ON_LBN_SELCHANGE(IDC_LIST_SP_RED, &CLineStartPosition::OnLbnSelchangeListSpRed)
	ON_LBN_SELCHANGE(IDC_LIST_SP_BLUE, &CLineStartPosition::OnLbnSelchangeListSpBlue)
	ON_LBN_SELCHANGE(IDC_LIST_SP_NPC, &CLineStartPosition::OnLbnSelchangeListSpNpc)
	ON_BN_CLICKED(IDC_BUTTON_SP_UPDATE, &CLineStartPosition::OnBnClickedButtonSpUpdate)
	ON_BN_CLICKED(IDC_BUTTON_SP_AUTO, &CLineStartPosition::OnBnClickedButtonSpAuto)
	ON_CBN_SELCHANGE(IDC_COMBO_AUTO_TYPE, &CLineStartPosition::OnCbnSelchangeComboAutoType)
	ON_EN_CHANGE(IDC_EDIT_AUTO_GAP, &CLineStartPosition::OnEnChangeEditAutoGap)
	ON_BN_CLICKED(IDC_BUTTON_DELETEALLNPC, &CLineStartPosition::OnBnClickedButtonDeleteallnpc)
	ON_BN_CLICKED(IDC_BUTTON_RESETKEY, &CLineStartPosition::OnBnClickedButtonResetkey)
	ON_BN_CLICKED(IDC_BUTTON1, &CLineStartPosition::OnBnClickedButton1)
END_MESSAGE_MAP()


// CLineStartPosition 메시지 처리기입니다.




void CLineStartPosition::OnBnClickedCheck1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// Hide User

	CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
	if( pMain != NULL )
	{
		UINT bChecked = IsDlgButtonChecked(IDC_CHECK1);
		if( bChecked == BST_CHECKED )
		{
			pMain->m_bHideUser = true;
		}
		else
		{
			pMain->m_bHideUser = false;
		}
	}	
}


void CLineStartPosition::OnBnClickedCheck2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// Hide Npc
	CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
	if( pMain != NULL )
	{
		UINT bChecked = IsDlgButtonChecked(IDC_CHECK2);
		if( bChecked == BST_CHECKED )
		{
			pMain->m_bHideNpc = true;
		}
		else
		{
			pMain->m_bHideNpc = false;
		}
	}
}


void CLineStartPosition::OnLbnSetfocusListSpRed()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_listBlue.SetCurSel(-1);
	m_listNpc.SetCurSel(-1);
}


void CLineStartPosition::OnLbnSetfocusListSpNpc()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_listRed.SetCurSel(-1);
	m_listBlue.SetCurSel(-1);
}

int	CLineStartPosition::GetStartPosIndex(int iTeam, int iIndex)
{	
	int iRed, iBlue, iNpc;
	iRed = iBlue = iNpc = -1;

	for(UINT i=0; i<vecTeamStartPos.size(); ++i)
	{
		switch(vecTeamStartPos[i].iTeamColor)
		{
		case 0: // npc
			++iNpc;
			break;
		case 1: // red
			++iRed;
			break;
		case 2: // blue
			++iBlue;
			break;
		}
		if( vecTeamStartPos[i].iTeamColor == iTeam )
		{
			if( iTeam == 0 && iIndex == iNpc )
			{
				return i;
			}
			else if( iTeam == 1 && iIndex == iRed )
			{
				return i;
			}
			else if( iTeam == 2 && iIndex == iBlue )
			{
				return i;
			}
		}
	}

	return -1;
}
int CLineStartPosition::GetNextUniqueNPCStartPosKey()
{
	for( int i=1; i<sizeof(m_abKeyUsed); ++i )
	{
		m_CurrKeyUsed = (m_CurrKeyUsed + 1) % (int)sizeof(m_abKeyUsed);
		if( false == m_abKeyUsed[m_CurrKeyUsed] )
		{
			return m_CurrKeyUsed;
		}
	}

	return -1;
}

void CLineStartPosition::ResetNpcStartPosKey()
{
	CX2ToolMain *pToolMain = (CX2ToolMain*)g_pMain;

	m_CurrKeyUsed = 0;
	for( int i = 0; i < sizeof(m_abKeyUsed); ++i)
	{
		m_abKeyUsed[i] = false;
	}
	for( int i = 0; i < (int)vecTeamStartPos.size(); ++i)
	{
		if( vecTeamStartPos[i].iTeamColor == 0 )
		{
			int key = GetNextUniqueNPCStartPosKey();
			if( key == -1 )
				continue;
			vecTeamStartPos.at(i).key = key;
			m_abKeyUsed[key] = true;
		}
	}

	pToolMain->m_bChangeStartPos = true;
}

void CLineStartPosition::UpdateStartPosition(int iIndex)
{
	if( iIndex < 0 || iIndex >= (int)vecTeamStartPos.size() )
		return;
		
	STeamStartPos sp = vecTeamStartPos[iIndex];
	if( sp.iTeamColor == TEAM_NONE )
	{
		for(int i=0; i<m_listNpc.GetCount(); ++i)
		{
			if( (int)m_listNpc.GetItemData(i) == iIndex )
			{
				m_listNpc.SetCurSel(i);
				ViewStartPosition(iIndex);
				return;
			}
		}
	}
	else 
	{
		if( sp.iTeamColor == TEAM_RED )
		{
			for(int i=0; i<m_listRed.GetCount(); ++i)
			{
				if( (int)m_listRed.GetItemData(i) == iIndex )
				{
					m_listRed.SetCurSel(i);
					ViewStartPosition(iIndex);
					return;
				}
			}
		}
		else
		{
			for(int i=0; i<m_listBlue.GetCount(); ++i)
			{
				if( (int)m_listBlue.GetItemData(i) == iIndex )
				{
					m_listBlue.SetCurSel(i);
					ViewStartPosition(iIndex);
					return;
				}
			}
		}		
	}	
}

void CLineStartPosition::ViewStartPosition(int iIndex)
{
	if( iIndex < 0 || iIndex >= (int)vecTeamStartPos.size() )
	{
		m_iSelectedIndex = -1;
		return;
	}

	m_iSelectedIndex = iIndex;

	STeamStartPos sp = vecTeamStartPos[iIndex];
	m_comboTeam.SetCurSel(sp.iTeamColor);
	m_comboDir.SetCurSel(sp.iInitialDir);
}

float CLineStartPosition::GetGap(D3DXVECTOR3 vPos, int iLineIndex)
{
	float fResult = -1.f;

	CKTDGLineMap *pLineMap = g_pX2Game->GetWorld()->GetLineMap();
	const CKTDGLineMap::LineData *pLineData = pLineMap->GetLineData(iLineIndex);
	if( pLineData != NULL )
	{
		bool bFind = CX2ToolMain::CCollision::LineToSphere(pLineData->startPos, pLineData->endPos, 10.f, vPos, 10.f, NULL);
		if( bFind == true )
		{
			float fDist1 = GetDistance( pLineData->startPos, vPos );
			float fDist2 = GetDistance( pLineData->startPos, pLineData->endPos );

			if( fDist1 >= 0.f && fDist1 <= fDist2 )
				return fDist1 / fDist2;
		}
	}	
	return fResult;
}

float CLineStartPosition::FindNewStartPosition(D3DXVECTOR3& vPos, int& iLineIndex)
{
	CKTDGLineMap *pLineMap = g_pX2Game->GetWorld()->GetLineMap();
	D3DXVECTOR3 vOutPos = D3DXVECTOR3(0.f, 0.f, 0.f);
	int iNearestIndex = pLineMap->GetNearestByTool( vPos, &vOutPos);

	if( iNearestIndex >= 0 )
	{
		iLineIndex = iNearestIndex;
		vPos = vOutPos;
		float fGap = GetGap( vPos, iLineIndex );
		if( fGap >= 0.f )
			return fGap;
	}

	return 0.f;
}

int CLineStartPosition::AddStartPosition( D3DXVECTOR3 vPos, int iLineIndex)
{
	int key=-1;	
	int iFlag = m_comboTeam.GetCurSel();

	bool bRight = true;
	if( m_comboDir.GetCurSel() == DIR_LEFT )
		bRight = false;

	if( iFlag == TEAM_NONE )
	{
		m_CurrKeyUsed = 0;
		key = GetNextUniqueNPCStartPosKey();
		if( key == -1 )
			return key;
	}

	int initDir = 0;
	if( bRight == true )
		initDir = 1;

	float fGap = GetGap( vPos, iLineIndex );
	if( fGap <= -1.f )
	{
		fGap = FindNewStartPosition( vPos, iLineIndex );
	}

	vecTeamStartPos.push_back( STeamStartPos( key, iFlag, vPos, initDir, iLineIndex, fGap ) );

	if( iFlag == TEAM_NONE )
		m_abKeyUsed[m_CurrKeyUsed] = true;

	ResetStartPosition(false);

	CX2ToolMain *pToolMain = (CX2ToolMain*)g_pMain;
	pToolMain->m_bChangeStartPos = true;

	return key;
}

void CLineStartPosition::MoveStartPosition( D3DXVECTOR3 vPos, int iPickedIndex )
{
	int iLineIndex = -1;
	D3DXVECTOR3 vNewPos = vPos;
	float fGap = FindNewStartPosition( vNewPos, iLineIndex );
	vecTeamStartPos[iPickedIndex].vPos = vNewPos;
	vecTeamStartPos[iPickedIndex].fGap = fGap;
	vecTeamStartPos[iPickedIndex].iLineIndex = iLineIndex;

	CX2ToolMain *pToolMain = (CX2ToolMain*)g_pMain;
	pToolMain->m_bChangeStartPos = true;
}

void CLineStartPosition::SelectStartPos(int iIndex)
{
	int iSelIndex = -1;
	for(int i=0; i<m_listNpc.GetCount(); ++i)
	{
		CString csBuf;
		m_listNpc.GetText(i, csBuf);
		int iStartPos = _ttoi( csBuf.GetString() );
		if( iStartPos == iIndex )
			iSelIndex = i;
	}

	m_listNpc.SetCurSel(iSelIndex);
	int iSelectStartPos = GetStartPosIndex(0, iSelIndex);
	ViewStartPosition(iSelectStartPos);

	CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
	pMain->m_iPickedStartPosition = iSelectStartPos;
}

void CLineStartPosition::OnLbnSelchangeListSpRed()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int iSelected = m_listRed.GetCurSel();
	int iIndex = GetStartPosIndex(1, iSelected);
	ViewStartPosition(iIndex);

	CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
	pMain->m_iPickedStartPosition = iIndex;
}


void CLineStartPosition::OnLbnSelchangeListSpBlue()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int iSelected = m_listBlue.GetCurSel();
	int iIndex = GetStartPosIndex(2, iSelected);
	ViewStartPosition(iIndex);

	CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
	pMain->m_iPickedStartPosition = iIndex;
}


void CLineStartPosition::OnLbnSelchangeListSpNpc()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int iSelected = m_listNpc.GetCurSel();
	int iIndex = GetStartPosIndex(0, iSelected);
	ViewStartPosition(iIndex);

	CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
	pMain->m_iPickedStartPosition = iIndex;
}


void CLineStartPosition::OnBnClickedButtonSpUpdate()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if( m_iSelectedIndex < 0 || m_iSelectedIndex >= (int)vecTeamStartPos.size() )
		return;

	STeamStartPos &sp = vecTeamStartPos[m_iSelectedIndex];
	if( sp.iTeamColor != m_comboTeam.GetCurSel() )
	{
		AfxMessageBox(L"StartPosition의 팀 속성은 변경할 수 없습니다.", MB_OK);
		return;
	}

	sp.iInitialDir = m_comboDir.GetCurSel();	
}

BOOL CLineStartPosition::PreTranslateMessage(MSG* pMsg)
{	
	if( pMsg->message ==WM_KEYDOWN )
	{
		switch( pMsg->wParam )
		{
		case VK_DELETE:
			{
				if( m_iSelectedIndex >= 0 )
				{
					DeleteNpcStartPos( m_iSelectedIndex );
					ResetStartPosition(false);
				}				
			}
			return TRUE;
		case VK_RETURN:
			return TRUE;
		default:
			break;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CLineStartPosition::DeleteNpcStartPos(int index)
{
	int iKey = vecTeamStartPos[index].key;
	vecTeamStartPos.erase( vecTeamStartPos.begin() + index );
	if( iKey > 0)
		m_abKeyUsed[iKey] = false;
	m_CurrKeyUsed = 0;

	CX2ToolMain *pToolMain = (CX2ToolMain*)g_pMain;
	if( pToolMain != NULL )
	{
		pToolMain->DeleteObjectStartPosition(index);
		pToolMain->RefreshNpcData(iKey);
		
		pToolMain->m_bChangeStartPos = true;
	}
}

void CLineStartPosition::OnBnClickedButtonSpAuto()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int iSelected = m_comboAutoType.GetCurSel();
	float fGap = 0.f;
	float fLast = 0.f;

	m_CurrKeyUsed = 0;
	int initDir = m_comboDir.GetCurSel();
	
	bool bSelected = false;
	CKTDGLineMap *pLineMap = g_pX2Game->GetWorld()->GetLineMap();
	for(int i=0; i<pLineMap->GetNumLineData(); ++i)
	{
		const CKTDGLineMap::LineData *pLineData = pLineMap->GetLineData(i);
		if( pLineData->m_bSelected == true )
		{
			bSelected = true;
			break;
		}
	}

	for(int i=0; i<pLineMap->GetNumLineData(); ++i)
	{
		const CKTDGLineMap::LineData *pLineData = pLineMap->GetLineData(i);
		if( (bSelected == true && pLineData->m_bSelected == true) || 
			(bSelected == false && pLineData->lineType != CKTDGLineMap::LT_WALL && pLineData->lineType != CKTDGLineMap::LT_BUNGEE && pLineData->lineType != CKTDGLineMap::LT_POTAL) )
		{
			if( iSelected == 0 )
			{
				fGap = m_fAutoGapDist;
				fLast = pLineData->fLength;
			}
			else
			{
				fGap = m_fAutoGapRate;
				fLast = 1.f;
			}				
			
			while( fGap <= fLast )
			{
				int key = -1;				
				key = GetNextUniqueNPCStartPosKey();
				if( key <= -1 )
					break;

				D3DXVECTOR3 vPos = pLineData->startPos;
				if( iSelected == 0 )
					vPos = pLineData->startPos + ( pLineData->dirVector * fGap );
				else
					vPos = pLineData->startPos + ( pLineData->dirVector * (pLineData->fLength * fGap) );

				float fRate = fGap;
				if( iSelected == 0 )
					fRate = fGap / fLast;

				vecTeamStartPos.push_back( STeamStartPos( key, TEAM_NONE, vPos, initDir, pLineData->lineIndex, fRate ) );				
				m_abKeyUsed[m_CurrKeyUsed] = true;
				
				if( iSelected == 0 )
					fGap += m_fAutoGapDist;
				else
					fGap += m_fAutoGapRate;
			}
		}
	}
	ResetStartPosition(false);
}


void CLineStartPosition::OnCbnSelchangeComboAutoType()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int iSelected = m_comboAutoType.GetCurSel();
	if( iSelected == 0 )
	{
		char cBuf[100];
		sprintf(cBuf, "%5.2f", m_fAutoGapDist);
		wstring wstrDist;
		ConvertCharToWCHAR(wstrDist, cBuf);
		m_editAutoGap.SetWindowTextW(wstrDist.c_str());
	}
	else
	{
		char cBuf[100];
		sprintf(cBuf, "%5.2f", m_fAutoGapRate);
		wstring wstrRate;
		ConvertCharToWCHAR(wstrRate, cBuf);
		m_editAutoGap.SetWindowTextW(wstrRate.c_str());
	}
}


void CLineStartPosition::OnEnChangeEditAutoGap()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialog::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.	
	WCHAR wBuf[100];
	m_editAutoGap.GetWindowText(wBuf, 100);
	string str1;
	ConvertWCHARToChar(str1, wBuf);

	int iSelected = m_comboAutoType.GetCurSel();
	if( iSelected == 0 )
	{
		m_fAutoGapDist = static_cast<float>( atof(str1.c_str()) );
	}
	else
	{
		m_fAutoGapRate = static_cast<float>( atof(str1.c_str()) );
	}
}


void CLineStartPosition::OnBnClickedButtonDeleteallnpc()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.	
	
	for(int i=vecTeamStartPos.size()-1; i>=0; --i)
	{
		if( vecTeamStartPos[i].iTeamColor == 0 )
		{			
			DeleteNpcStartPos(i);			
		}
	}	

	ResetStartPosition(false);
}


void CLineStartPosition::OnBnClickedButtonResetkey()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ResetNpcStartPosKey();
	ResetStartPosition(false);
}


void CLineStartPosition::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	RebuildStartPosition(-1);
}
