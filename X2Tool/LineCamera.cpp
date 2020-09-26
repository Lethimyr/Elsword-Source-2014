// LineCamera.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "X2Tool.h"
#include "LineCamera.h"
#include "afxdialogex.h"


// CLineCamera 대화 상자입니다.

IMPLEMENT_DYNAMIC(CLineCamera, CDialog)

CLineCamera::CLineCamera(CWnd* pParent /*=NULL*/)
	: CDialog(CLineCamera::IDD, pParent)
{
	m_iSelectedItem = -1;
}

CLineCamera::~CLineCamera()
{
}

void CLineCamera::ResetCameraList()
{
	if( g_pMain != NULL && g_pX2Game != NULL )
	{
		m_CameraList.DeleteAllItems();
		CKTDGLineMap *pLineMap = g_pX2Game->GetWorld()->GetLineMap();
		for(int i=0; i<pLineMap->GetNumCameraData(); ++i)
		{
			CKTDGLineMap::CameraData *pCameraData = pLineMap->GetCameraDataListByTool(i);
			if( pCameraData != NULL )
			{
				WCHAR wBuf[30];
				wsprintfW(wBuf, L"    %d         ", i);				
				m_CameraList.InsertItem(i, wBuf);
			}
		}

		m_ComboLineType.ResetContent();
		m_ComboLineType.AddString(L"LCT_INVALID");
		m_ComboLineType.AddString(L"LCT_ABSOULTE_POSITION");
		m_ComboLineType.AddString(L"LCT_RELATIE_POSITION");
		
		m_ComboLineType.SetCurSel(0);
		m_editEye.SetWindowTextW(L"0 0 0");
		m_editLookat.SetWindowTextW(L"0 0 0");
		m_CheckFocusUnit.SetCheck(MF_CHECKED);
		m_editReleativeEye.SetWindowTextW(L"0 0 0");
		m_editRepositionSpeed.SetWindowTextW(L"100");
	}

	m_CameraList.UpdateData(TRUE);
}

void CLineCamera::UpdateCameraInfo(int iItem)
{
	if( g_pMain != NULL && g_pX2Game != NULL )
	{
		CKTDGLineMap *pLineMap = g_pX2Game->GetWorld()->GetLineMap();
		CKTDGLineMap::CameraData *pCameraData = pLineMap->GetCameraDataListByTool(iItem);
		if( pCameraData != NULL )
		{
			char cBuf[100];			
			m_ComboLineType.SetCurSel((int)pCameraData->m_eType);

			sprintf(cBuf, "%5.2f %5.2f %5.2f", pCameraData->m_vEye.x, pCameraData->m_vEye.y, pCameraData->m_vEye.z);
			wstring wstrEye;
			ConvertCharToWCHAR(wstrEye, cBuf);
			m_editEye.SetWindowText( wstrEye.c_str() );

			sprintf(cBuf, "%5.2f %5.2f %5.2f", pCameraData->m_vLookAt.x, pCameraData->m_vLookAt.y, pCameraData->m_vLookAt.z);
			wstring wstrLookAt;
			ConvertCharToWCHAR(wstrLookAt, cBuf);
			m_editLookat.SetWindowText( wstrLookAt.c_str() );

			if( pCameraData->m_bFocusUnit == true )
				m_CheckFocusUnit.SetCheck(MF_CHECKED);
			else
				m_CheckFocusUnit.SetCheck(MF_UNCHECKED);

			sprintf(cBuf, "%5.2f %5.2f %5.2f", pCameraData->m_vRelativeEye.x, pCameraData->m_vRelativeEye.y, pCameraData->m_vRelativeEye.z);
			wstring wstrRelativeEye;
			ConvertCharToWCHAR(wstrRelativeEye, cBuf);
			m_editReleativeEye.SetWindowText( wstrRelativeEye.c_str() );

			sprintf(cBuf, "%5.2f", pCameraData->m_fCameraRepositionSpeed);
			wstring wstrRepositionSpeed;
			ConvertCharToWCHAR(wstrRepositionSpeed, cBuf);
			m_editRepositionSpeed.SetWindowText( wstrRepositionSpeed.c_str() );
		}
	}
}

BOOL CLineCamera::PreTranslateMessage(MSG* pMsg)
{
	if( pMsg->message ==WM_KEYDOWN )
	{
		switch( pMsg->wParam )
		{
		case VK_DELETE:
			{
				if( m_iSelectedItem >= 0 )
				{
					//m_CameraList.DeleteItem(m_iSelectedItem);
					CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
					pMain->DeleteCameraData(m_iSelectedItem);
					ResetCameraList();
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

void CLineCamera::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_CameraList);
	DDX_Control(pDX, IDC_COMBO1, m_ComboLineType);
	DDX_Control(pDX, IDC_EDIT_CAMERA_EYE, m_editEye);
	DDX_Control(pDX, IDC_EDIT_CAMERA_LOOKAT, m_editLookat);
	DDX_Control(pDX, IDC_CHECK_FOCUSUNIT, m_CheckFocusUnit);
	DDX_Control(pDX, IDC_EDIT_RELEATIVE_EYE, m_editReleativeEye);
	DDX_Control(pDX, IDC_EDIT_REPOSITION_SPEED, m_editRepositionSpeed);
}


BEGIN_MESSAGE_MAP(CLineCamera, CDialog)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CLineCamera::OnLvnItemchangedList1)
	ON_BN_CLICKED(IDC_BUTTON_GETCAMERA, &CLineCamera::OnBnClickedButtonGetcamera)
	ON_BN_CLICKED(IDC_BUTTON_CAMERA_INIT, &CLineCamera::OnBnClickedButtonCameraInit)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE_CAMERA, &CLineCamera::OnBnClickedButtonUpdateCamera)
	ON_BN_CLICKED(IDC_BUTTON_ADD_CAMERA, &CLineCamera::OnBnClickedButtonAddCamera)
END_MESSAGE_MAP()


// CLineCamera 메시지 처리기입니다.


void CLineCamera::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	if( pNMLV->uNewState == 3 )
	{
		m_iSelectedItem = pNMLV->iItem;
		UpdateCameraInfo(m_iSelectedItem);
	}
	else
	{
		m_iSelectedItem = -1;
	}
}



void CLineCamera::OnBnClickedButtonGetcamera()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
	if( pMain->m_bUserCamera == true )
	{
		AfxMessageBox(L"User Camera가 활성화 되어 있어\n현재 카메라 정보를 가져 올 수 없습니다.", MB_OK);
		return;
	}

	D3DXVECTOR3 vEye = g_pX2Game->GetX2Camera()->GetCamera().GetEye();
	D3DXVECTOR3 vLookAt = g_pX2Game->GetX2Camera()->GetCamera().GetLookAt();

	char cBuf[100];

	sprintf(cBuf, "%5.2f %5.2f %5.2f", vEye.x, vEye.y, vEye.z);
	wstring wstrEye;
	ConvertCharToWCHAR(wstrEye, cBuf);
	m_editEye.SetWindowText( wstrEye.c_str() );

	sprintf(cBuf, "%5.2f %5.2f %5.2f", vLookAt.x, vLookAt.y, vLookAt.z);
	wstring wstrLookAt;
	ConvertCharToWCHAR(wstrLookAt, cBuf);
	m_editLookat.SetWindowText( wstrLookAt.c_str() );
}


void CLineCamera::OnBnClickedButtonCameraInit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_ComboLineType.SetCurSel(0);
	m_editEye.SetWindowTextW(L"0 0 0");
	m_editLookat.SetWindowTextW(L"0 0 0");
	m_CheckFocusUnit.SetCheck(MF_CHECKED);
	m_editReleativeEye.SetWindowTextW(L"0 0 0");
	m_editRepositionSpeed.SetWindowTextW(L"100");
}


void CLineCamera::OnBnClickedButtonUpdateCamera()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CKTDGLineMap *pLineMap = g_pX2Game->GetWorld()->GetLineMap();
	int iCameraIndex = m_iSelectedItem;
	
	if( iCameraIndex >= 0 && iCameraIndex < pLineMap->GetNumCameraData() )
	{
		CKTDGLineMap::CameraData *pCameraData = pLineMap->GetCameraDataListByTool(iCameraIndex);
		if( pCameraData != NULL )
		{
			pCameraData->m_eType = (CKTDGLineMap::LINE_CAMERA_TYPE)m_ComboLineType.GetCurSel();

			UINT bChecked = IsDlgButtonChecked(IDC_CHECK_FOCUSUNIT);
			if( bChecked == BST_CHECKED )
				pCameraData->m_bFocusUnit = true;
			else
				pCameraData->m_bFocusUnit = false;

			WCHAR wBuf[100];
			m_editRepositionSpeed.GetWindowText(wBuf, 100);
			string str1;
			ConvertWCHARToChar(str1, wBuf);
			pCameraData->m_fCameraRepositionSpeed = static_cast<float>( atof(str1.c_str()) );

			m_editEye.GetWindowText(wBuf, 100);			
			D3DXVECTOR3 vEye;
			ConvertWCHARToChar(str1, wBuf);
			sscanf(str1.c_str(), "%f%f%f", &vEye.x, &vEye.y, &vEye.z);
			pCameraData->m_vEye = vEye;

			m_editLookat.GetWindowText(wBuf, 100);			
			D3DXVECTOR3 vLookAt;
			ConvertWCHARToChar(str1, wBuf);
			sscanf(str1.c_str(), "%f%f%f", &vLookAt.x, &vLookAt.y, &vLookAt.z);
			pCameraData->m_vLookAt = vLookAt;

			m_editReleativeEye.GetWindowText(wBuf, 100);			
			D3DXVECTOR3 vReleativeEye;
			ConvertWCHARToChar(str1, wBuf);
			sscanf(str1.c_str(), "%f%f%f", &vReleativeEye.x, &vReleativeEye.y, &vReleativeEye.z);
			pCameraData->m_vRelativeEye = vReleativeEye;
		}
	}	
}


void CLineCamera::OnBnClickedButtonAddCamera()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CKTDGLineMap *pLineMap = g_pX2Game->GetWorld()->GetLineMap();
	CKTDGLineMap::CameraData cameraData;

	cameraData.m_eType = (CKTDGLineMap::LINE_CAMERA_TYPE)m_ComboLineType.GetCurSel();
	UINT bChecked = IsDlgButtonChecked(IDC_CHECK_FOCUSUNIT);
	if( bChecked == BST_CHECKED )
		cameraData.m_bFocusUnit = true;
	else
		cameraData.m_bFocusUnit = false;

	WCHAR wBuf[100];
	m_editRepositionSpeed.GetWindowText(wBuf, 100);
	string str1;
	ConvertWCHARToChar(str1, wBuf);
	cameraData.m_fCameraRepositionSpeed = static_cast<float>( atof(str1.c_str()) );

	m_editEye.GetWindowText(wBuf, 100);			
	D3DXVECTOR3 vEye;
	ConvertWCHARToChar(str1, wBuf);
	sscanf(str1.c_str(), "%f%f%f", &vEye.x, &vEye.y, &vEye.z);
	cameraData.m_vEye = vEye;

	m_editLookat.GetWindowText(wBuf, 100);			
	D3DXVECTOR3 vLookAt;
	ConvertWCHARToChar(str1, wBuf);
	sscanf(str1.c_str(), "%f%f%f", &vLookAt.x, &vLookAt.y, &vLookAt.z);
	cameraData.m_vLookAt = vLookAt;

	m_editReleativeEye.GetWindowText(wBuf, 100);			
	D3DXVECTOR3 vReleativeEye;
	ConvertWCHARToChar(str1, wBuf);
	sscanf(str1.c_str(), "%f%f%f", &vReleativeEye.x, &vReleativeEye.y, &vReleativeEye.z);
	cameraData.m_vRelativeEye = vReleativeEye;

	pLineMap->AddCameraData(cameraData);
	
	ResetCameraList();
}
