#pragma once

class CKTDGCamera  
{
	public:

		// camera_shake_type
		enum SHAKE_TYPE
		{
			DECT_INVALID,
			DECT_UP,
			DECT_DOWN,
			DECT_UP_DOWN,
			DECT_UP_DOWN_NO_RESET,


			DECT_LEFT,
			DECT_RIGHT,
			DECT_LEFT_RIGHT,
			DECT_LEFT_RIGHT_NO_RESET,

			DECT_RANDOM,
		};




		struct CameraData
		{
			D3DXMATRIX	m_ViewMatrix;
			D3DXVECTOR3	m_Eye;
			D3DXVECTOR3	m_LookAt;
			D3DXVECTOR3	m_UpVec;
		};


		struct UpSideDownCameraData
		{
			float m_fTimeLeft;

			UpSideDownCameraData()
			: m_fTimeLeft( 0.f )
			{
			}

			void OnFrameMove( float fElapsedTime )
			{
				if( m_fTimeLeft > 0.f )
				{
					m_fTimeLeft -= fElapsedTime;
					if( m_fTimeLeft < 0.f )
					{
						m_fTimeLeft = 0.f;
					}
				}
			}
		};

		struct RandomCameraShakeData
		{
			bool			m_bFixedLookVec;		// lookvec를 고정하고 eye, lookat위치를 이동할지

			int				m_iEyeShakeCount;		// camera shake동안 방향전환이 이루어질 횟수 
			CMinMax<float>	m_EyeMoveSpeed;			// camera shake동안 한방향으로 이동할 때 speed
			CMinMax<float>	m_EyeMoveRange;			// camera shake동안 한방향으로 이동할 범위
			D3DXVECTOR3		m_vEyeMove;
			D3DXVECTOR3 	m_vEyeMoveDir;
			float			m_fEyeMoveTimeLeft;
			float			m_fEyeMoveSpeed;


			int				m_iLookAtShakeCount;	// camera shake동안 방향전환이 이루어질 횟수 
			CMinMax<float>	m_LookAtMoveSpeed;		// camera shake동안 한방향으로 이동할 때 speed
			CMinMax<float>	m_LookAtMoveRange;		// camera shake동안 한방향으로 이동할 범위
			D3DXVECTOR3 	m_vLookAtMove;
			D3DXVECTOR3 	m_vLookAtMoveDir;
			float			m_fLookAtMoveTimeLeft;
			float			m_fLookAtMoveSpeed;



			RandomCameraShakeData()
			{
				m_bFixedLookVec			= true;

				m_iEyeShakeCount		= 0;		
				m_EyeMoveSpeed 			= CMinMax<float>(0, 0);
				m_EyeMoveRange 			= CMinMax<float>(0, 0);
				m_vEyeMove				= D3DXVECTOR3(0, 0, 0);
				m_vEyeMoveDir			= D3DXVECTOR3(0, 0, 0);
				m_fEyeMoveTimeLeft		= 0.f;
				m_fEyeMoveSpeed			= 0.f;

				m_iLookAtShakeCount 	= 0;		
				m_LookAtMoveSpeed 		= CMinMax<float>(0, 0);
				m_LookAtMoveRange 		= CMinMax<float>(0, 0);
				m_vLookAtMove			= D3DXVECTOR3(0, 0, 0);
				m_vLookAtMoveDir		= D3DXVECTOR3(0, 0, 0);
				m_fLookAtMoveTimeLeft	= 0.f;
				m_fLookAtMoveSpeed		= 0.f;
			}

		};



		


	public:
		CKTDGCamera( LPDIRECT3DDEVICE9 pd3dDevice );
		virtual ~CKTDGCamera();

		void Point( float XEye, float YEye, float ZEye, 
					float XAt, float YAt, float ZAt,
					float XUpVec = 0.0f, float YUpVec = 1.0f, float ZUpVec = 0.0f );

		void Move( float XPos, float YPos, float ZPos );
		void MoveRel( float XAdd, float YAdd, float ZAdd );
		void MoveRel( D3DXVECTOR3 vAdd );

		void LookAt( float XAt, float YAt, float ZAt );
		void LookAtRel( float XAdd, float YAdd, float ZAdd);
		void LookAtRel( D3DXVECTOR3 vAdd );
		
		void RotateX( float radian );
		void RotateY( float radian );
		void RotateZ( float radian );

		void UpVec( float XAt, float YAt, float ZAt );
		void UpVecRel( float XAdd, float YAdd, float ZAdd);

		void UpdateCamera( float fElapsedTime = 0.0f );

		
        //{{ seojt // 2008-10-23, 15:50
        void UpdateViewTransform( const D3DXVECTOR3& vEye_, const D3DXVECTOR3& vLookAt_ );
        //}} seojt // 2008-10-23, 15:50

		void SetTracking( float XTo, float YTo, float ZTo, float XAt, float YAt, float ZAt, float Time, float atTime = 0.0f );

		void PushCamera();
		int  PopCamera();

		void StopTracking(){ m_fTrackTime = 0.0f; }
		float GetRemainTrackingTime(){ return m_fTrackTime; }

		const D3DXVECTOR3& GetEye() const { return m_Camera.m_Eye; }
		float GetXEye() const { return m_Camera.m_Eye.x; }
		float GetYEye() const { return m_Camera.m_Eye.y; }
		float GetZEye() const { return m_Camera.m_Eye.z; }
		
		const D3DXVECTOR3& GetLookAt() const { return m_Camera.m_LookAt; }
		float GetXLookAt() const { return m_Camera.m_LookAt.x; }
		float GetYLookAt() const { return m_Camera.m_LookAt.y; }
		float GetZLookAt() const { return m_Camera.m_LookAt.z; }

		const D3DXVECTOR3& GetUpVec() const { return m_Camera.m_UpVec; }
		float GetXUpVec() const { return m_Camera.m_UpVec.x; }
		float GetYUpVec() const { return m_Camera.m_UpVec.y; }
		float GetZUpVec() const { return m_Camera.m_UpVec.z; }

		D3DXVECTOR3 GetLookVec() const { return m_Camera.m_LookAt - m_Camera.m_Eye; }
		D3DXVECTOR3 GetRightVec()
		{
			KTDXPROFILE();

			D3DXVECTOR3 vLookVec = GetLookVec();
			D3DXVec3Normalize( &vLookVec, &vLookVec );
			
			D3DXVECTOR3 vUpVec(0, 1, 0); //GetUpVec();

			D3DXVECTOR3 vRightVec(0,0,0);
			D3DXVec3Cross( &vRightVec, &vUpVec, &vLookVec );
			//D3DXVec3Normalize( &vRightVec, &vRightVec );

			return vRightVec;
		}


		const D3DXMATRIX& GetViewMatrix()
		{ 
			D3DXMatrixLookAtLH( &m_Camera.m_ViewMatrix, &m_Camera.m_Eye, &m_Camera.m_LookAt, &m_Camera.m_UpVec );
			return m_Camera.m_ViewMatrix; 
		}

		void Reset();
		float GetCameraDistance();



		// camera shake
		void DownCrashCamera( float speed = -100.0f, float accel = 1500.0f );
		void UpCrashCamera( float speed = 100.0f, float accel = -1500.0f );
		void UpDownCrashCamera( float fGap = 10.0f, float fTime = 0.2f );
		void UpDownCrashCameraNoReset( float fGap = 10.0f, float fTime = 0.2f );
		void ShakeUpDown( float fGap = 10.f, float fTime = 0.2f, float fTimeGap = 0.05f );

		void LeftCrashCamera( float speed = 100.0f, float accel = -1500.0f );
		void RightCrashCamera( float speed = -100.0f, float accel = 1500.0f );
		void LeftRightCrashCamera( float fGap = 10.0f, float fTime = 0.2f );
		void LeftRightCrashCameraNoReset( float fGap = 10.0f, float fTime = 0.2f );
		void ShakeLeftRight( float fGap = 10.f, float fTime = 0.2f, float fTimeGap = 0.05f );

		void ShakeRandom( int iEyeShakeCount, float m_fEyeMoveSpeedMin, float m_fEyeMoveSpeedMax, float m_fEyeMoveRangeMin, float m_fEyeMoveRangeMax, 
			int iLookAtShakeCount, float m_fLookAtMoveSpeedMin, float m_fLookAtMoveSpeedMax, float m_fLookAtMoveRangeMin, float m_fLookAtMoveRangeMax, 
			bool bFixedLookVec );

		void ImpactForward( float fDistance = 20.f, float fSpeed = 100.f, float fRetrieveAccel = 20.f, float fDampingRate = 0.6f, int iRetrieveBounceCount = 2 );

		void UpSideDown( float fDuration = 3.f );

#ifdef KEYFRAME_CAMERA
		void SetTrackingTime( float bVal ){ m_fTrackTime = bVal; }
		void SetFixedUpVec( bool bEnable ){ m_bFixedUpVec = bEnable; }
#endif KEYFRAME_CAMERA

#ifdef TOGETHER_FESTIVAL_2012_AUTUMN		/// 부드러운 카메라 크래시 ( 점점 강해졌다가 최대값 이후 점점 약해지는 카메라 크래시 )  
		void UpDownCrashCameraSmooth( float fGap = 10.0f, float fTime = 0.2f );		/// 시간, 강도 설정
		void InitUpDownCrashCameraSmooth();											/// 초기화
		void ProcessUpDownCrashCameraSmooth();										/// 시간에 따른 강도 연산
#endif TOGETHER_FESTIVAL_2012_AUTUMN

	protected:
		void UpdateCameraShake( float fElapsedTime );
		void UpdateCameraImpact( float fElapsedTime );


	private:
		CameraData			m_Camera;

		D3DXVECTOR3			m_Track;
		D3DXVECTOR3			m_TrackAt;
		float				m_fTrackTime;
		float				m_fTrackAtTime;

		float				m_fDownCrashGap;
		float				m_fDownCrashSpeed;
		float				m_fDownCrashAccel;

		float				m_fUpCrashGap;
		float				m_fUpCrashSpeed;
		float				m_fUpCrashAccel;

		bool				m_bUpDownCrashPositive;
		float				m_fUpDownCrashGap;		/// 진동 크기
		float				m_fUpDownCrashTime;
		float				m_fUpDownCrashTimeGap;
		float				m_fUpDownCrashTimeGapInit;


		float				m_fLeftCrashGap;
		float				m_fLeftCrashSpeed;
		float				m_fLeftCrashAccel;

		float				m_fRightCrashGap;
		float				m_fRightCrashSpeed;
		float				m_fRightCrashAccel;

		bool				m_bLeftRightCrashPositive;
		float				m_fLeftRightCrashGap;
		float				m_fLeftRightCrashTime;
		float				m_fLeftRightCrashTimeGap;
		float				m_fLeftRightCrashTimeGapInit;


		// 화면 안쪽, 바깥쪽으로 흔들리는 카메라 관련 변수
		float m_fImpactForwardDistance;
		float m_fImpactForwardSpeed;
		float m_fImpactForwardBounceSpeed;
		float m_fImpactForwardDamping;				// 0~1사이의 값
		float m_fImpactForwardBounceRadian;
		float m_fImpactForwardCurrentBounceRange;
		int m_iImpactForwardMaxBounceCount; 
		int m_iImpactForwardCurrentBounceCount;
		
#ifdef KEYFRAME_CAMERA
		bool				m_bFixedUpVec;
#endif KEYFRAME_CAMERA

		stack<CameraData>	m_CameraStack;		


		RandomCameraShakeData m_RandomCameraShakeData;
		UpSideDownCameraData m_UpSideDownCameraData;

#ifdef TOGETHER_FESTIVAL_2012_AUTUMN
		float m_fSmoothCameraCreshMaxTime;		/// 부드러운 카메라 크래시에 적용되는 시간
		float m_fSmoothCameraCreshMaxGap;		/// 부드러운 카메라 크래시 강도
		bool  m_bEnableSmoothCameraCresh;		/// 부드러운 카메라 크래시 적용 여부
#endif TOGETHER_FESTIVAL_2012_AUTUMN
};
