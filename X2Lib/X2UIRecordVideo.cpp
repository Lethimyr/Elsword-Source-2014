
#include "StdAfx.h"

//{{ 2011.9.16 이준호 반디캠 작업
#ifdef	BANDICAM_RECORDING

#include ".\X2UIRecordVideo.h"



CX2UIRecordVideo::CX2UIRecordVideo( const CKTDXStage* const pNowStage, const WCHAR* const pFileName ) :
m_bShow( false ),
m_pNowStage( pNowStage ),
m_pDlgRecordVideo( NULL ),
m_vPosDlg( 0, 0 ),
m_vSizeDlg( 0, 0 ),
m_bPause( false ),
m_pButtonExit( NULL ),
m_pButtonRecordStart( NULL ),
m_pButtonRecordPause( NULL ),
m_pButtonRecordStop( NULL ),
m_pButtonRecordRec( NULL )
//m_pButtonOpenOption( NULL ),
//m_pButtonCloseOption( NULL )
{

	m_pDlgRecordVideo	=	new CKTDGUIDialog( const_cast< CKTDXStage* >( pNowStage ), pFileName );
		
	if ( NULL != m_pDlgRecordVideo )
	{
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDlgRecordVideo );
		m_pDlgRecordVideo->SetDisableUnderWindow( true );

		m_vPosDlg.x			= m_pDlgRecordVideo->GetDummyPos( 0 ).x;		// 0은 posion
		m_vPosDlg.y			= m_pDlgRecordVideo->GetDummyPos( 0 ).y;

		m_vSizeDlg.x		= m_pDlgRecordVideo->GetDummyPos( 1 ).x;		// 1은 size
		m_vSizeDlg.y		= m_pDlgRecordVideo->GetDummyPos( 1 ).y;
		m_pDlgRecordVideo->SetPos(m_vPosDlg);
		m_pDlgRecordVideo->SetSize(m_vSizeDlg.x,m_vSizeDlg.y);

		m_pButtonExit	= static_cast< CKTDGUIButton* >( m_pDlgRecordVideo->GetControl( L"Btn_Record_exit" ) );
		m_pButtonRecordStart	= static_cast< CKTDGUIButton* >( m_pDlgRecordVideo->GetControl( L"Btn_Record_Start" ) );
		m_pButtonRecordPause	= static_cast< CKTDGUIButton* >( m_pDlgRecordVideo->GetControl( L"Btn_Record_Pause" ) );
		m_pButtonRecordStop		= static_cast< CKTDGUIButton* >( m_pDlgRecordVideo->GetControl( L"Btn_Record_Stop" ) );
		m_pButtonRecordRec		= static_cast< CKTDGUIButton* >( m_pDlgRecordVideo->GetControl( L"Btn_Record_Rec" ) );
	}
}

CX2UIRecordVideo::~CX2UIRecordVideo()
{
	m_pButtonExit			= NULL;
	m_pButtonRecordStart	= NULL;
	m_pButtonRecordPause	= NULL;
	m_pButtonRecordStop		= NULL;
	//m_pButtonOpenOption	= NULL;
	//m_pButtonCloseOption	= NULL;

	SAFE_DELETE_DIALOG( m_pDlgRecordVideo );
}

void		CX2UIRecordVideo::InitUIRecordVideo()
{
	/*if( NULL != m_pDlgRecordVideo )
	{
		D3DXVECTOR3 tmp;
		tmp = m_pDlgRecordVideo->GetDummyPos(0);
		m_vPosDlg.x = tmp.x;
		m_vPosDlg.y = tmp.y;
		tmp = m_pDlgRecordVideo->GetDummyPos(1);
		m_vSizeDlg.x = tmp.x;
		m_vSizeDlg.y = tmp.y;

		m_pDlgRecordVideo->SetPos(m_vPosDlg);
		m_pDlgRecordVideo->SetSize(m_vSizeDlg.x,m_vSizeDlg.y);
	}*/
	
	m_bPause = false;
	//this->m_pButtonExit->SetShowEnable(false,false);
	m_pDlgRecordVideo->SetShow(false);
	if( g_pMain->GetGameOption()->GetRecordAutoStart_WhenOpen() )
	{
		g_pMain->Bandi_StartVideoCapture();
		UI_RecdingStart();
	}
	else
		UI_RecdingStop();
}

void		CX2UIRecordVideo::ReadyCloseUIRecordVideo()
{
	g_pMain->Bandi_EndVidioCapture();
}


///////////핸들러////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////



HRESULT		CX2UIRecordVideo::OnFrameMove( double fTime, float fElapsedTime )
{	
	return S_OK;
}

bool		CX2UIRecordVideo::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// 앞에 모달 다이얼로그가 있으면 메시지를 처리하지 않는다
	if( true == g_pKTDXApp->GetDGManager()->GetDialogManager()->CheckFrontModalDlg( m_pDlgRecordVideo ) )
		return false;
	
	bool bFlag	= false;
	
	switch( uMsg )
	{
		// 마우스 메시지들에 대해
	case WM_MOUSEMOVE:
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_LBUTTONDBLCLK:
	case WM_MBUTTONDBLCLK:
	case WM_RBUTTONDBLCLK:
		//case WM_MOUSEWHEEL:
		/*{ 영역에 도달했을때만 x버튼 표시기능 삭제
			if ( m_pDlgRecordVideo->GetIsMouseOver())
			{
				this->m_pButtonExit->SetShowEnable(true,true);
				return false;
			}
			else
			{
				this->m_pButtonExit->SetShowEnable(false,false);
			}
			bFlag = true;
		}*/
		break;
	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
		{
			/*if ( false == m_pDlgRecordVideo->GetIsMouseOver())
			{
				return false;
			}
			g_pData->GetUIManager()->SetUILayerIndex(CX2UIManager::UI_RECORD_VIDEO, true);*/
			bFlag = true;			
		}
		break;
	default:
		break;
	}

	return bFlag;
}

bool		CX2UIRecordVideo::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch ( wParam )
	{
	case URVCM_EXIT:
		{
			// ReadyCloseUIRecordVideo는 SetShow함수 내에서 호출됨 //
			if(g_pMain->IsCapturing())
				ReadyCloseUIRecordVideo();

			m_bShow = false;
			g_pData->GetUIManager()->UIClosed( CX2UIManager::UI_RECORD_VIDEO );
			m_pDlgRecordVideo->SetShowEnable( false, false );

			return true;
		}
		break;

	case URVCM_RECORD_START:
		{
			// Pause 상태 -- Restart 요청
			UI_RecdingStart();
			if( g_pMain->IsCapturing() )
				g_pMain->Bandi_Pause_Restart_VidieCapture(false);
			else
				g_pMain->Bandi_StartVideoCapture();

			return true;
		}
		break;

	case URVCM_RECORD_PAUSE:
		{
			// 현재 Recording 상태 -- Pause 요청
			if( g_pMain->IsCapturing() )
			{
				g_pMain->Bandi_Pause_Restart_VidieCapture(true);
				UI_RecdingPause();
				m_bPause = true;
			}

			// Pause 해제 요청은 URVCM_RECORD_START 쪽에 있음 // 

			return true;
		}
		break;

	case URVCM_RECORD_STOP:
		{
			//StopRecording();
			g_pMain->Bandi_EndVidioCapture();
			m_bPause = false;
			UI_RecdingStop();
			//SetShow(false);

			return true;
		}
		break;

	case URVCM_OPEN_OPTION:
		{
			return true;
		}
		break;

	case URVCM_CLOSE_OPTION:
		{
			return true;
		}
		break;

	default:
		break;
	}

	return false;
}

//정지, 해제 키 입력으로 왔을 때 처리 함수 
void		CX2UIRecordVideo::PauseKeyHandling()
{
	// 현재 Recording 상태 -- Pause 요청
	if( g_pMain->IsCapturing() )
	{
		// 녹화 중 상태 -- 정지 요청
		if( false == m_bPause )
		{
			g_pMain->Bandi_Pause_Restart_VidieCapture(true);

			if( NULL != m_pButtonRecordStart )
				m_pButtonRecordStart->SetEnable(true);
			if( NULL != m_pButtonRecordPause )
				m_pButtonRecordPause->SetEnable(false);

			m_bPause = true;
		}
		// 일시 정지 상태 -- 재시작 요청
		else
		{
			g_pMain->Bandi_Pause_Restart_VidieCapture(false);

			if( NULL != m_pButtonRecordStart )
				m_pButtonRecordStart->SetEnable(false);
			if( NULL != m_pButtonRecordPause )
				m_pButtonRecordPause->SetEnable(true);

			m_bPause = false;
		}
	}
}


bool		CX2UIRecordVideo::UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
// 	switch ( wParam )
// 	{
// 	default:
// 		break;
// 	}

	return false;
}

bool		CX2UIRecordVideo::GetShow() const
{
	return m_bShow;
}

void		CX2UIRecordVideo::SetShow( bool bShow )
{
	if ( bShow )
	{
		m_bShow = bShow;
		InitUIRecordVideo();
		
		g_pData->GetUIManager()->UIOpened( CX2UIManager::UI_RECORD_VIDEO );
		m_pDlgRecordVideo->SetShowEnable( m_bShow, m_bShow );
		//m_pDlgRecordVideo->SetShow(false);
	}
	else if(g_pMain->IsCapturing()==false)
	{
		g_pMain->Bandi_StartVideoCapture();
		UI_RecdingStart();

	}
	else
	{
		ReadyCloseUIRecordVideo();
		if(g_pMain->GetGameOption()->GetRecordAutoClose_WhenStop())
		{
			m_bShow = bShow;
			g_pData->GetUIManager()->UIClosed( CX2UIManager::UI_RECORD_VIDEO );
			m_pDlgRecordVideo->SetShowEnable( m_bShow, m_bShow );
			//m_pDlgRecordVideo->SetShow(false);
		}	
		else
			UI_RecdingStop();
	}
}

D3DXVECTOR2	CX2UIRecordVideo::GetDLGSize() const
{
	return m_vSizeDlg;		// 임시
}

void		CX2UIRecordVideo::SetPosition( D3DXVECTOR2 vec )
{
	m_vPosDlg = vec;
	m_pDlgRecordVideo->SetPos( m_vPosDlg );
}

void		CX2UIRecordVideo::SetLayer( X2_DIALOG_LAYER layer )
{
	if( m_pDlgRecordVideo != NULL) 
	{
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeLayer( m_pDlgRecordVideo, layer);
		g_pKTDXApp->GetDGManager()->GetDialogManager()->ChangeSequence( m_pDlgRecordVideo, false );
	}
}
void		CX2UIRecordVideo::UI_RecdingStart()
{
	if( NULL != m_pButtonRecordStart )
		m_pButtonRecordStart->SetShowEnable(false,false);
	if( NULL != m_pButtonRecordStart )
		m_pButtonRecordRec->SetShowEnable(false,false);
	if( NULL != m_pButtonRecordPause )
		m_pButtonRecordPause->SetShowEnable(true,true);
	if( NULL != m_pButtonRecordStop )
		m_pButtonRecordStop->SetEnable(true);
	m_bPause = false;
}
void		CX2UIRecordVideo::UI_RecdingPause()
{
	if( NULL != m_pButtonRecordStart )
		m_pButtonRecordStart->SetShowEnable(true,true);
	if( NULL != m_pButtonRecordStart )
		m_pButtonRecordRec->SetShowEnable(false,false);
	if( NULL != m_pButtonRecordPause )
		m_pButtonRecordPause->SetShowEnable(false,false);
	if( NULL != m_pButtonRecordStop )
		m_pButtonRecordStop->SetEnable(true);

}
void		CX2UIRecordVideo::UI_RecdingStop()
{
	if( NULL != m_pButtonRecordStart )
		m_pButtonRecordStart->SetShowEnable(false,false);
	if( NULL != m_pButtonRecordStart )
		m_pButtonRecordRec->SetShowEnable(true,true);
	if( NULL != m_pButtonRecordPause )
		m_pButtonRecordPause->SetShowEnable(false,false);
	if( NULL != m_pButtonRecordStop )
		m_pButtonRecordStop->SetEnable(false);

}
#endif	BANDICAM_RECORDING
//}}

