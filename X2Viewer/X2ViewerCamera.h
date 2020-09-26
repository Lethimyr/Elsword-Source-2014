#pragma once
/**	@file	: X2ViewerCamera.h
	@desc	: 카메라 관련 기능 정의
	@author	: AnomyMous

	@date	JHKang on 2013-09-13,
		- 필드 이후 빌드 성공
	@date	JHKang on 2013-09-17,
		- 파일 정리 및 블록화
*/

#pragma region CX2ViewerCamera
/** @class : CX2ViewerCamera
	@brief : 뷰어 카메라
	@date  : 2013-09-17
*/
class CX2ViewerCamera : public CX2ViewerObject
{
public:
	enum CAMERA_MODE
	{
		CM_NORMAL = 0,
		CM_NAVIGATION,
	};

	enum LOCAR_DIR
	{
		LOCAL_X = 0,
		LOCAL_Y,
		LOCAL_Z
	};

public:
	CX2ViewerCamera();
	virtual ~CX2ViewerCamera(void);

	virtual	HRESULT OnFrameMove( double fTime, float fElapsedTime );
	virtual	HRESULT	OnFrameRender();
	virtual	HRESULT	OnResetDevice();

	D3DXVECTOR3	GetEye()	{ return m_sCamera.m_Eye;}
	void SetEye( D3DXVECTOR3 v3Value_ ) { m_sCamera.m_Eye = v3Value_; }
	void SetEyeX( float fValue_ ) { m_sCamera.m_Eye.x = fValue_; }
	void SetEyeY( float fValue_ ) { m_sCamera.m_Eye.y = fValue_; }
	void SetEyeZ( float fValue_ ) { m_sCamera.m_Eye.z = fValue_; }

	D3DXVECTOR3	GetLookVec(){ return m_sCamera.m_LookAt;}
	void SetLookVec( D3DXVECTOR3 v3Value_ ) { m_sCamera.m_LookAt = v3Value_; }
	void SetLookVecX( float fValue_ ) { m_sCamera.m_LookAt.x = fValue_; }
	void SetLookVecY( float fValue_ ) { m_sCamera.m_LookAt.y = fValue_; }
	void SetLookVecZ( float fValue_ ) { m_sCamera.m_LookAt.z = fValue_; }

	D3DXVECTOR3	GetUpVec()	{ return m_sCamera.m_UpVec;}

	void		SetCameraMode( CAMERA_MODE cameraMode );
	CAMERA_MODE	GetCameraMode(){ return m_CameraMode; }

	void		CameraReset(){ Init(); }

	bool GetUsingWheel() { return m_bUseWheel; }
	void SetUsingWheel( bool bValue_ ) { m_bUseWheel = bValue_; }

private:
	void		Init();
	void		CameraMode_Normal();
	void		CameraMode_Function();

	void		SetTracking( float Time = 1.0f );
	void		MoveLocal( float dist, LOCAR_DIR localDir = LOCAL_X );
	void		RotateLocal( LOCAR_DIR localDir, float angle );
	void		SetView( float fElapsedTime = 0.0f );


private:

	//LPDIRECT3DDEVICE9			m_pd3dDevice;
	CKTDIDevice*				m_pMouse;			/// 마우스 장치
	CKTDIDevice*				m_pKeyboard;		/// 키보드 장치

	CKTDGCamera::CameraData		m_sCamera;			/// 카메라 정보
	D3DXMATRIX					m_matWorld;			/// 월드 행렬
	D3DXMATRIX					m_matProjection;	/// 투영 행렬
	D3DXVECTOR3					m_vView;			/// 카메라가 향하는 단위방향벡터
	D3DXVECTOR3					m_vCross;			/// 카메라의 측면벡터 cross( view, up )

	// MMO STyle
	D3DXVECTOR3					m_vAngle;			/// 각도
	float						m_fDist;			/// 거리
	float						m_fElapsedTime;		/// 지난 시간

	long						m_nMouseNowPosX, m_nMousePrePosX;	/// 마우스 현재, 이전 X 좌표
	long						m_nMouseNowPosY, m_nMousePrePosY;	/// 마우스 현재, 이전 Y 좌표
	long						m_nWheel;							/// 마우스 휠 정보

	D3DXVECTOR3					m_TrackAt;			/// Track 정보
	float						m_fTrackTime;		/// 트랙 시간
	bool						m_bIsTrack;			/// 트랙 여부

	CAMERA_MODE					m_CameraMode;

	bool	m_bUseWheel;	/// 카메라 휠을 사용 여부
};
#pragma endregion class