/**	@file	: X2ViewerCamera.cpp
	@desc	: 카메라 관련 기능 정의
	@author	: AnomyMous

	@date	JHKang on 2013-09-13,
		- 필드 이후 빌드 성공
	@date	JHKang on 2013-09-17,
		- 파일 정리 및 블록화
*/

#include "dxstdafx.h"
#include ".\x2viewercamera.h"

extern bool		g_bShowUI;

#pragma region CX2ViewerCamera
CX2ViewerCamera::CX2ViewerCamera()
{
	CX2ViewerObject::SetObjectStyle( CX2ViewerObject::OS_CAMERA );
	ZeroMemory( &m_sCamera, sizeof(m_sCamera) );
	m_CameraMode = CM_NORMAL;
	m_bUseWheel = true;

	Init();
}
#pragma endregion 생성자

#pragma region ~CX2ViewerCamera
CX2ViewerCamera::~CX2ViewerCamera(void)
{
}
#pragma endregion 소멸장

#pragma region OnFrameMove
HRESULT CX2ViewerCamera::OnFrameMove( double fTime, float fElapsedTime )
{
	m_fElapsedTime = fElapsedTime;

	switch( m_CameraMode )
	{
	case CM_NORMAL:
		CameraMode_Normal();
		break;

	case CM_NAVIGATION:
		CameraMode_Function();
		break;
	}

	SetView( fElapsedTime );

	return S_OK;
}
#pragma endregion 매 프레임 처리

#pragma region OnFrameRender
HRESULT CX2ViewerCamera::OnFrameRender()
{
	if ( g_bShowUI )
	{
		CDXUTTextHelper txtHelper( g_pFont, g_pTextSprite, 12 );

		RECT rt;
		SetRect( &rt, 0, 0, 0, 0 );
		GetClientRect( g_pKTDXApp->GetHWND(), &rt );

		float fScaleY = rt.bottom / 768.f;

		// Output statistics
		txtHelper.Begin();
		txtHelper.SetInsertionPos( 5, (int)(850*fScaleY) );
		txtHelper.SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f ) );
		txtHelper.DrawTextLine( DXUTGetFrameStats() );
		txtHelper.DrawTextLine( DXUTGetDeviceStats() );

		/*WCHAR appData[255] = {0,};
		swprintf( appData, L"EYE : X %.2f | Y %.2f | Z %.2f", m_sCamera.m_Eye.x, m_sCamera.m_Eye.y, m_sCamera.m_Eye.z );
		txtHelper.DrawTextLine( appData );
		swprintf( appData, L"LookAt : X %.2f | Y %.2f | Z %.2f", m_sCamera.m_LookAt.x, m_sCamera.m_LookAt.y, m_sCamera.m_LookAt.z );
		txtHelper.DrawTextLine( appData );
		swprintf( appData, L"m_vAngle : %.2f | Y %.2f | Z %.2f", m_vAngle.x, m_vAngle.y, m_vAngle.z );
		txtHelper.DrawTextLine( appData );
		swprintf( appData, L"m_fDist : %.2f", m_fDist );
		txtHelper.DrawTextLine( appData );*/

		txtHelper.End();
	}

	return S_OK;
}
#pragma endregion 매 프레임 렌더링

#pragma region OnResetDevice
HRESULT CX2ViewerCamera::OnResetDevice()
{
	Init();

	return S_OK;
}
#pragma endregion 장치 재설정

#pragma region SetCameraMode
void CX2ViewerCamera::SetCameraMode( CAMERA_MODE cameraMode )
{
	m_CameraMode = cameraMode;

	//if( m_CameraMode == CM_NORMAL )
	//	SetTracking( 0.1f );
}
#pragma endregion 카메라 모드 변경

#pragma region Init
void CX2ViewerCamera::Init()
{
	//m_pd3dDevice	= g_pKTDXApp->GetDevice();
	m_pMouse		= g_pKTDXApp->GetDIManager()->GetMouse();
	m_pKeyboard		= g_pKTDXApp->GetDIManager()->Getkeyboard();

	D3DXMatrixIdentity( &m_sCamera.m_ViewMatrix );
	D3DXMatrixIdentity( &m_matWorld );
	D3DXMatrixIdentity( &m_matProjection );
	m_sCamera.m_Eye.x		=    0.0f;
	m_sCamera.m_Eye.y		=  100.0f;
	m_sCamera.m_Eye.z		= -995.12f;
	m_sCamera.m_LookAt.x	=    0.0f;
	m_sCamera.m_LookAt.y	=  100.0f;
	m_sCamera.m_LookAt.z	=    0.0f;
	m_sCamera.m_UpVec.x		=    0.0f;
	m_sCamera.m_UpVec.y		=    1.0f;
	m_sCamera.m_UpVec.z		=    0.0f;
	D3DXMatrixLookAtLH( &m_sCamera.m_ViewMatrix, &m_sCamera.m_Eye, &m_sCamera.m_LookAt, &m_sCamera.m_UpVec );
	SetView();

	m_vAngle		= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	m_fDist			= 1000.0f;

	//m_TrackAt		= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	//m_fTrackTime	= 0.0f;
	m_bIsTrack		= false;

	m_nMousePrePosX	= 0;
	m_nMousePrePosY	= 0;
	m_nMouseNowPosX = 0;
	m_nMouseNowPosY = 0;
	m_nWheel		= 0;

	m_fElapsedTime		= 0.0f;
}
#pragma endregion 속성 초기화

#pragma region CameraMode_Normal
void CX2ViewerCamera::CameraMode_Normal()
{
	if( m_bIsTrack == true )
		return;

	if( m_pMouse->GetButtonState( MOUSE_RBUTTON ) == TRUE )
	{
		m_nMouseNowPosX = m_pMouse->GetXPos();
		m_nMouseNowPosY = m_pMouse->GetYPos();
		m_nMousePrePosX = m_nMouseNowPosX;
		m_nMousePrePosY = m_nMouseNowPosY;
	}
	// forward/back Move
	if( m_bUseWheel && ( m_nWheel = m_pMouse->GetZPos() ) )
	{
		if( m_nWheel >= -240 && m_nWheel <= 240 )
		{
			float	fMoveDelta	= 3000.0f*m_fElapsedTime;//50.0f;
			long	tempZ		= m_nWheel/120;

			debugPrintf( "wheel %d", m_nWheel );

			if( m_nWheel > 0 )
				MoveLocal( tempZ*fMoveDelta );
			if( m_nWheel < 0 )
				MoveLocal( tempZ*fMoveDelta );
		}
	}

	// View Move
	if( m_pMouse->GetPureButtonState( MOUSE_RBUTTON) == TRUE )
	{
		m_nMouseNowPosX = m_pMouse->GetXPos();
		m_nMouseNowPosY = m_pMouse->GetYPos();

		float	fAngleDelta = 0.6f*m_fElapsedTime;
		long	tempX = m_nMouseNowPosX - m_nMousePrePosX;
		long	tempY = m_nMouseNowPosY - m_nMousePrePosY;

		// Y축 회전
		if( tempX < 0 )
			RotateLocal( LOCAL_Y, tempX*fAngleDelta );
		else if( tempX > 0 )
			RotateLocal( LOCAL_Y, tempX*fAngleDelta );

		// X축 회전
		if( tempY < 0 )
			RotateLocal( LOCAL_X, tempY*fAngleDelta );
		else if( tempY > 0 )
			RotateLocal( LOCAL_X, tempY*fAngleDelta );

		m_nMousePrePosX = m_nMouseNowPosX;
		m_nMousePrePosY = m_nMouseNowPosY;
	}

	SetView();
}
#pragma endregion 카메라 기본 모드

#pragma region CameraMode_Function
void CX2ViewerCamera::CameraMode_Function()
{
	if( m_bIsTrack == true )
		return;

	if( m_pMouse->GetButtonState( MOUSE_LBUTTON ) == TRUE ||
		m_pMouse->GetButtonState( MOUSE_RBUTTON ) == TRUE )
	{
		m_nMouseNowPosX = m_pMouse->GetXPos();
		m_nMouseNowPosY = m_pMouse->GetYPos();
		m_nMousePrePosX = m_nMouseNowPosX;
		m_nMousePrePosY = m_nMouseNowPosY;
	}
	// forward/back Move
	if( m_bUseWheel && ( m_nWheel = m_pMouse->GetZPos() ) )
	{
		float	fMoveDelta	= 3000.0f*m_fElapsedTime;//50.0f;
		long	tempZ		= m_nWheel/120;

		if( m_nWheel > 0 )
			MoveLocal( tempZ*fMoveDelta, LOCAL_Z );

		if( m_nWheel < 0 )
			MoveLocal( tempZ*fMoveDelta, LOCAL_Z );
	}

	// View Move
	if( m_pMouse->GetPureButtonState( MOUSE_LBUTTON ) == TRUE )
	{
		m_nMouseNowPosX = m_pMouse->GetXPos();
		m_nMouseNowPosY = m_pMouse->GetYPos();

		float	fMoveDelta	= 120.0f*m_fElapsedTime;//2.0f;// * fElapsedTime;
		long	tempX		= m_nMouseNowPosX - m_nMousePrePosX;
		long	tempY		= m_nMouseNowPosY - m_nMousePrePosY;

		// X축 이동
		if( tempX < 0 )
			MoveLocal( -tempX*fMoveDelta, LOCAL_X );
		else if( tempX > 0 )
			MoveLocal( -tempX*fMoveDelta, LOCAL_X );

		// Y축 이동
		if( tempY < 0 )
			MoveLocal( tempY*fMoveDelta, LOCAL_Y );
		else if( tempY > 0 )
			MoveLocal( tempY*fMoveDelta, LOCAL_Y );

		m_nMousePrePosX = m_nMouseNowPosX;
		m_nMousePrePosY = m_nMouseNowPosY;
	}

	// Rotate
	if( m_pMouse->GetPureButtonState( MOUSE_RBUTTON ) == TRUE )
	{
		m_nMouseNowPosX = m_pMouse->GetXPos();
		m_nMouseNowPosY = m_pMouse->GetYPos();

		float	fAngleDelta	= 0.12f*m_fElapsedTime;//0.002f;
		long	tempX		= m_nMouseNowPosX - m_nMousePrePosX;
		long	tempY		= m_nMouseNowPosY - m_nMousePrePosY;

		// Y축 회전
		if( tempX < 0 )
			RotateLocal( LOCAL_Y, tempX*fAngleDelta );
		else if( tempX > 0 )
			RotateLocal( LOCAL_Y, tempX*fAngleDelta );

		// X축 회전
		if( tempY < 0 )
			RotateLocal( LOCAL_X, tempY*fAngleDelta );
		else if( tempY > 0 )
			RotateLocal( LOCAL_X, tempY*fAngleDelta );

		m_nMousePrePosX = m_nMouseNowPosX;
		m_nMousePrePosY = m_nMouseNowPosY;
	}
}
#pragma endregion 카메라 모드 함수?

#pragma region SetTracking
void CX2ViewerCamera::SetTracking( float Time )
{
	//m_sCamera.m_LookAt.x = 0.0f;
	//m_sCamera.m_LookAt.y = 100.0f;
	//m_sCamera.m_LookAt.z = 0.0f;
	//m_sCamera.m_Eye.x		=    0.0f;
	//m_sCamera.m_Eye.y		=    0.0f;
	//m_sCamera.m_Eye.z		= -900.0f;
	//m_TrackAt.x = m_sCamera.m_LookAt.x/Time;
	//m_TrackAt.y = (m_sCamera.m_LookAt.y - 100.0f)/Time;
	//m_TrackAt.z = m_sCamera.m_LookAt.z/Time;

	//m_fTrackTime = Time;

	//m_bIsTrack = true;
}
#pragma endregion 트래킹 설정

#pragma region MoveLoacal
void CX2ViewerCamera::MoveLocal( float dist, LOCAR_DIR localDir /* = LOCAL_X  */)
{
	switch( m_CameraMode )
	{
	case CM_NORMAL:
		{
			m_fDist += dist;

			if( m_fDist < 100.0f)
				m_fDist -= dist;
		}
		break;

	case CM_NAVIGATION:
		{
			D3DXVECTOR3 vMove, vDir;

			switch( localDir )
			{
			case LOCAL_X:
				vDir = m_vCross;
				break;

			case LOCAL_Y:
				vDir = m_sCamera.m_UpVec;
				break;

			case LOCAL_Z:
				vDir = m_vView;
				break;
			}

			D3DXVec3Normalize( &vMove, &vDir );
			vMove				*= dist;
			m_sCamera.m_Eye		+= vMove;
			m_sCamera.m_LookAt	+= vMove;
		}
		break;
	}

	SetView();
}
#pragma endregion 지역 좌표 이동?

#pragma region RotateLocal
void CX2ViewerCamera::RotateLocal( LOCAR_DIR localDir, float angle )
{
	switch( m_CameraMode )
	{
	case CM_NORMAL:
		{
			switch( localDir)
			{
			case LOCAL_X:
				{
					m_vAngle.x -= angle;

					if( m_vAngle.x > 1.5f || m_vAngle.x < -1.5f)
						m_vAngle.x += angle;
				}
				break;
			case LOCAL_Y:
				m_vAngle.y += angle;
				break;
			}
		}
		break;

	case CM_NAVIGATION:
		{
			D3DXMATRIXA16	matRot;
			D3DXVECTOR3		vDir;

			switch( localDir )
			{
			case LOCAL_X:
				vDir = m_vCross;
				break;
			case LOCAL_Y:
				vDir = m_sCamera.m_UpVec;
				break;
			}

			D3DXVECTOR3 vNewDst;
			D3DXMatrixRotationAxis( &matRot, &vDir, angle);
			D3DXVec3TransformCoord( &vNewDst, &m_vView, &matRot);

			vNewDst += m_sCamera.m_Eye;
			m_sCamera.m_LookAt = vNewDst;
		}
		break;
	}

	SetView();
}
#pragma endregion 지역 좌표 회전

#pragma region SetView
void CX2ViewerCamera::SetView( float fElapsedTime )
{
	//if( m_fTrackTime != 0.0f )
	//{
	//	m_sCamera.m_LookAt.x	-= m_TrackAt.x * fElapsedTime;
	//	m_sCamera.m_LookAt.y	-= m_TrackAt.y * fElapsedTime;
	//	m_sCamera.m_LookAt.z	-= m_TrackAt.z * fElapsedTime;

	//	m_fTrackTime -= fElapsedTime;

	//	if( m_fTrackTime <= 0.0f )
	//	{
	//		m_fTrackTime	= 0.0f;
	//		m_bIsTrack		= false;

	//		m_sCamera.m_LookAt.x	=    0.0f;
	//		m_sCamera.m_LookAt.y	=  100.0f;
	//		m_sCamera.m_LookAt.z	=    0.0f;
	//	}
	//}

	switch( m_CameraMode )
	{
	case CM_NORMAL:
		{
			if( m_bIsTrack == true )
				break;

			D3DXVECTOR3 vTemp;
			//카메라의 각방향 벡터를 만든다
			vTemp.x = cosf(m_vAngle.x) * sinf(m_vAngle.y);
			vTemp.y = sinf(m_vAngle.x);
			vTemp.z = cosf(m_vAngle.x) * cosf(m_vAngle.y);

			m_sCamera.m_Eye = m_sCamera.m_LookAt - vTemp * m_fDist;

			D3DXVec3Normalize( &m_vView, &( m_sCamera.m_LookAt - m_sCamera.m_Eye));
		}
		break;

	case CM_NAVIGATION:
		{
			D3DXVec3Normalize( &m_vView, &( m_sCamera.m_LookAt - m_sCamera.m_Eye ) );
			D3DXVec3Cross( &m_vCross, &m_sCamera.m_UpVec, &m_vView );
		}
		break;
	}

	D3DXMatrixLookAtLH( &m_sCamera.m_ViewMatrix, &m_sCamera.m_Eye, &m_sCamera.m_LookAt, &m_sCamera.m_UpVec );

	g_pKTDXApp->SetWorldTransform( &m_matWorld );
	g_pKTDXApp->SetViewTransform( &m_sCamera.m_ViewMatrix );
	// 09.12.23 태완 : 창크기에 따라 모델 찌그러지는 현상 수정
	RECT WindowRect;
	GetClientRect( g_pKTDXApp->GetHWND(), &WindowRect );

	float fWidth	= (float)(WindowRect.right - WindowRect.left);
	float fHeight	= (float)(WindowRect.bottom - WindowRect.top);

	D3DXMatrixPerspectiveFovLH( &m_matProjection, D3DX_PI/4, fWidth / fHeight, 1.0f, 100000.0f );
	g_pKTDXApp->SetProjectionTransform( &m_matProjection );
}
#pragma endregion 뷰 설정