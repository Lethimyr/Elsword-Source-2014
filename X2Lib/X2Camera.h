#pragma once

class CX2GameUnit;
class CX2SquareUnit;
class CX2GUUser;
class CX2Camera : public CKTDXStage
{
	public:
		enum CAMERA_STATE
		{
			CS_NORMAL,		/// 기본 카메라
			CS_EVENT,		/// 이벤트 카메라
		};

		enum CAMERA_TYPE
		{
			CT_NORMAL_TRACKING = 0,		/// 정해진 궤도를 따라가는 카메라
			CT_PARTS_LOOK_TRACKING,		/// LOOK_PART를 바라보며 궤도를 따라가는 카메라
			CT_NORMAL_DIRECT,			/// 카메라
			CT_PARTS_LOOK_DIRECT,		/// LOOK_PART를 직접 바라보는 카메라
			CT_BONE_ATTACHED,			/// 뼈대에 뿥은 카메라
		};

		enum LOOK_TYPE
		{
			LT_BONE = 0,		/// 뼈대
			LT_IMPACT_POINT,	/// 충돌 위치
			LT_WEAPON,			/// 무기
			LT_UNIT_POSITION,	/// 유닛 위치
		};

		enum EYE_TYPE
		{
			ET_BONE = 0,		/// 뻐대
			ET_IMPACT_POINT,	/// 충돌 위치
			ET_WEAPON,			/// 무기
			ET_UNIT_POSITION,	/// 유닛 위치
		};


#ifdef BUBBLE_BOBBLE_TEST
		enum ARCADE_CAMERA_MODE
		{
			ACM_FIXED,
			ACM_HORIZONTAL_SCROLL,
			ACM_VERTICAL_SCROLL,
			ACM_FULL_SCROLL,
			ACM_UNIT_FOCUS,
		};
#endif BUBBLE_BOBBLE_TEST

		enum EYE_POS_RELATIVE_UNIT
		{
			EPRU_FOUCS_UNIT,
			EPRU_MY_UNIT,
		};




		struct LineScriptedCameraData
		{
			D3DXVECTOR3 m_vFinalRelativeEyePosition;
			D3DXVECTOR3 m_vCurrRelativeEyePosition;
			float m_fSpeed;

			LineScriptedCameraData()
			: m_vFinalRelativeEyePosition( 0, 0, 0 )
			, m_vCurrRelativeEyePosition( 0, 0, 0 )
			, m_fSpeed( 100.f )
			{
			}

			void OnFrameMove( double fTime, float fElapsedTime )
			{
				if( m_vFinalRelativeEyePosition != m_vCurrRelativeEyePosition )
				{
					ASSERT( m_fSpeed > 0.f );
					const float fDelta = m_fSpeed * fElapsedTime;

					

					if( m_vFinalRelativeEyePosition.x > m_vCurrRelativeEyePosition.x )
					{
						if( m_vFinalRelativeEyePosition.x - m_vCurrRelativeEyePosition.x < fDelta )
						{
							m_vCurrRelativeEyePosition.x = m_vFinalRelativeEyePosition.x;
						}
						else
						{
							m_vCurrRelativeEyePosition.x += fDelta;
						}
					}
					else
					{
						if( m_vCurrRelativeEyePosition.x - m_vFinalRelativeEyePosition.x < fDelta )
						{
							m_vCurrRelativeEyePosition.x = m_vFinalRelativeEyePosition.x;
						}
						else
						{
							m_vCurrRelativeEyePosition.x -= fDelta;
						}
					}


					if( m_vFinalRelativeEyePosition.y > m_vCurrRelativeEyePosition.y )
					{
						if( m_vFinalRelativeEyePosition.y - m_vCurrRelativeEyePosition.y < fDelta )
						{
							m_vCurrRelativeEyePosition.y = m_vFinalRelativeEyePosition.y;
						}
						else
						{
							m_vCurrRelativeEyePosition.y += fDelta;
						}
					}
					else
					{
						if( m_vCurrRelativeEyePosition.y - m_vFinalRelativeEyePosition.y < fDelta )
						{
							m_vCurrRelativeEyePosition.y = m_vFinalRelativeEyePosition.y;
						}
						else
						{
							m_vCurrRelativeEyePosition.y -= fDelta;
						}
					}



					if( m_vFinalRelativeEyePosition.z > m_vCurrRelativeEyePosition.z )
					{
						if( m_vFinalRelativeEyePosition.z - m_vCurrRelativeEyePosition.z < fDelta )
						{
							m_vCurrRelativeEyePosition.z = m_vFinalRelativeEyePosition.z;
						}
						else
						{
							m_vCurrRelativeEyePosition.z += fDelta;
						}
					}
					else
					{
						if( m_vCurrRelativeEyePosition.z - m_vFinalRelativeEyePosition.z < fDelta )
						{
							m_vCurrRelativeEyePosition.z = m_vFinalRelativeEyePosition.z;
						}
						else
						{
							m_vCurrRelativeEyePosition.z -= fDelta;
						}
					}


					
				}
			}
		};

#ifdef KEYFRAME_CAMERA
		enum UI_CAMERA_EDIT_CUSTOM_MSG
		{
			UCECM_EXIT							= 47000,
			UCECM_PREV_KEYFRAME,
			UCECM_NEXT_KEYFRAME,
			UCECM_ADD_KEYFRAME,
			UCECM_SAVE_KEYFRAME,
			UCECM_DEL_KEYFRAME,
			UCECM_COMBO_ANGLE,
			UCECM_ENTER_MOV_VELO,
			UCECM_ENTER_ROT_VELO,
			UCECM_ENTER_FIXED_COORD_X,
			UCECM_ENTER_FIXED_COORD_Y,
			UCECM_ENTER_FIXED_COORD_Z,
		};

		enum ANGULAR_INTERPOLATION_TYPE
		{
			AIT_LINEAR,
			AIT_LOOK,
			AIT_FIXED,
			AIT_TRACE,
		};


		struct EyeInterPosition
		{
			D3DXVECTOR3 m_vEyeInterPosition;
			float		m_fCurveLength;

				EyeInterPosition()
				: m_vEyeInterPosition(0, 0, 0 )
				, m_fCurveLength( 0.f )
				{

				}

		};

		struct KeyFrameCameraData
		{
			D3DXVECTOR3 m_vEyePosition;
			D3DXVECTOR3 m_vLookAt;
			D3DXVECTOR3	m_vUpVec;
			float		m_fTime;
			ANGULAR_INTERPOLATION_TYPE m_eAITType;
			float		m_fMoveVelocity;
			float		m_fRotateTime;
			float		m_fCurveLength;
			D3DXVECTOR3 m_vLookAtPos;
			vector<EyeInterPosition> m_vecEyeInterPosition;

			KeyFrameCameraData()
				: m_vEyePosition( 0, 0, 0 )
				, m_vLookAt( 0, 0, 0 )
				, m_vUpVec( 0, 0, 0 )
				, m_fTime( 0.f )
				, m_eAITType(AIT_LINEAR)
				, m_fMoveVelocity( 0.f )
				, m_fRotateTime( 0.f )
				, m_fCurveLength( 0.f )
				, m_vLookAtPos(0, 0, 0)
			{
			}
		};

		struct WorldCameraData
		{
			int	m_iWorldCameraID;
			float m_fWorldCameraDuration;
			int m_iLoopSequenceStart;
			int m_iLoopSequenceEnd;
			int m_iLoopCount;
			bool m_bHoldViewWhenCameraEnd;
			vector<KeyFrameCameraData> m_vecKeyFrameCameraData;
			WorldCameraData()
				: m_iWorldCameraID( -1 )
				, m_fWorldCameraDuration( 0.f )
				, m_iLoopSequenceStart( 0 )
				, m_iLoopSequenceEnd( 0 )
				, m_iLoopCount( 0 )
				, m_bHoldViewWhenCameraEnd( false )
			{

			}

		};
#endif KEYFRAME_CAMERA

	public:
		CX2Camera(void);
		~CX2Camera(void);

		virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );

		const float GetLandHeight() { return m_fLand; }
		void SetLandHeight( float fLand ){ m_fLand = fLand; }
		void PlayLuaCamera( CX2GameUnit* pFocusUnit, KLuaManager& luaManger, int index );
		void PlayLuaCamera( CX2GameUnit* pFocusUnit, KLuaManager& luaManger, const char* pszTableNameUTF8 );
		void PlayLuaCamera( CX2GameUnit* pFocusUnit, KLuaManager& luaManger );

		void NomalTrackingCamera( CX2GameUnit* pFocusUnit, float distance = 1500.0f, float height = 200.0f, float angleDegree = 0.0f, float eyeDistance = 0.0f, float lookatDistance = 0.0f, float trackingTime = 0.3f, float trackingAtTime = 0.0f );
#ifdef MODIFY_LINEMAP_JUMPUP_IN_VILLAGE
		void NomalTrackingCamera( CX2SquareUnit* pFocusUnit, float distance = 1500.0f, float height = 200.0f, float angleDegree = 0.0f, float eyeDistance = 0.0f, float lookatDistance = 0.0f, float trackingTime = 0.3f, float trackingAtTime = 0.0f );
#endif //MODIFY_LINEMAP_JUMPUP_IN_VILLAGE
		void PartsLookTrackingCamera( CX2GameUnit* pFocusUnit, D3DXVECTOR3 look, float distance = 1500.0f, float height = 200.0f, float angleDegree = 0.0f, float eyeDistance = 0.0f, float lookatDistance = 0.0f, float trackingTime = 0.3f );
		void PartsLookTrackingCameraFromPartsEye( CX2GameUnit* pFocusUnit, const D3DXVECTOR3& vLookPos_, const D3DXVECTOR3& vEyePos_, float fTrackingTime_ = 0.3f );

		void NomalDirectCamera( CX2GameUnit* pFocusUnit, float distance = 1500.0f, float height = 200.0f, float angleDegree = 0.0f, float eyeDistance = 0.0f, float lookatDistance = 0.0f );
		
		void PartsLookDirectCamera( CX2GameUnit* pFocusUnit, D3DXVECTOR3 look, float distance = 1500.0f, float height = 200.0f, float angleDegree = 0.0f, float eyeDistance = 0.0f, float lookatDistance = 0.0f );
		void PartsLookDirectCameraFromPartsEye( CX2GameUnit* pFocusUnit, const D3DXVECTOR3& vLookPos_, const D3DXVECTOR3& vEyePos_ );


		bool ParsingLookTypeAndPos( KLuaManager& luaManger_, const CX2GameUnit* pFocusUnit_, OUT D3DXVECTOR3& vLookPos_ );
		bool ParsingEyeTypeAndPos( KLuaManager& luaManger_, const CX2GameUnit* pFocusUnit_, OUT D3DXVECTOR3& vEyePos_ );

		void NomalTrackingCamera_LUA( CX2GameUnit* pFocusUnit, D3DXVECTOR3 distance_Height_angleDegree, D3DXVECTOR3 eyeDistance_lookatDistance_trackingTime )
		{
			NomalTrackingCamera( pFocusUnit, 
									distance_Height_angleDegree.x, 
									distance_Height_angleDegree.y, 
									distance_Height_angleDegree.z,
									eyeDistance_lookatDistance_trackingTime.x, 
									eyeDistance_lookatDistance_trackingTime.y, 
									eyeDistance_lookatDistance_trackingTime.z );
		}
		void PartsLookTrackingCamera_LUA( CX2GameUnit* pFocusUnit, D3DXVECTOR3 look, D3DXVECTOR3 distance_Height_angleDegree, D3DXVECTOR3 eyeDistance_lookatDistance_trackingTime )
		{
			PartsLookTrackingCamera( pFocusUnit, look,
									distance_Height_angleDegree.x, 
									distance_Height_angleDegree.y, 
									distance_Height_angleDegree.z,
									eyeDistance_lookatDistance_trackingTime.x, 
									eyeDistance_lookatDistance_trackingTime.y, 
									eyeDistance_lookatDistance_trackingTime.z );
		}

		void NomalDirectCamera_LUA( CX2GameUnit* pFocusUnit, D3DXVECTOR3 distance_Height_angleDegree, D3DXVECTOR2 eyeDistance_lookatDistance )
		{
			NomalDirectCamera( pFocusUnit, 
								distance_Height_angleDegree.x, 
								distance_Height_angleDegree.y, 
								distance_Height_angleDegree.z,
								eyeDistance_lookatDistance.x, 
								eyeDistance_lookatDistance.y );
		}
		void PartsLookDirectCamera_LUA( CX2GameUnit* pFocusUnit, D3DXVECTOR3 look, D3DXVECTOR3 distance_Height_angleDegree, D3DXVECTOR2 eyeDistance_lookatDistance )
		{
			PartsLookDirectCamera( pFocusUnit, look,
									distance_Height_angleDegree.x, 
									distance_Height_angleDegree.y, 
									distance_Height_angleDegree.z,
									eyeDistance_lookatDistance.x, 
									eyeDistance_lookatDistance.y );
		}
#ifdef NPC_TRACKINGCAMERA
		void PartsLookTrackingCameraByNpc_LUA( CX2GameUnit* pFocusUnit, D3DXVECTOR3 look, D3DXVECTOR3 distance_Height_angleDegree, D3DXVECTOR3 eyeDistance_lookatDistance_trackingTime, float fDuration )
		{
			m_bNpcTrackingCamera = true;
			m_fTrackingTime = fDuration;

			PartsLookTrackingCamera( pFocusUnit, look,
				distance_Height_angleDegree.x, 
				distance_Height_angleDegree.y, 
				distance_Height_angleDegree.z,
				eyeDistance_lookatDistance_trackingTime.x, 
				eyeDistance_lookatDistance_trackingTime.y, 
				eyeDistance_lookatDistance_trackingTime.z );
		}
#endif

		//CKTDGCamera* GetCamera(){ return m_pCamera; }
        CKTDGCamera& GetCamera(){ return m_kCamera; }
		CAMERA_STATE GetCameraState(){ return m_CameraState; }


		void FieldCamera( CX2SquareUnit* pFocusUnit, float distance = 1200.0f, float lookatheight = 100.f, float eyeHeight = 300.0f, float angleDegree = 0.0f, float trackingTime = 0.4f );
		void TFieldCameraUpdate( CX2SquareUnit* pFocusUnit, float distance = 1200.0f, float lookatHeight = 100.f, float height = 200.0f, float angleDegree = 0.0f, float eyeDistance = 0.0f, float lookatDistance = 0.0f );


		void SquareCamera( CX2SquareUnit* pFocusUnit, float distance = 1200.0f, float eyeHeight = 200.0f, float lookAtHeight = 100.f, float angleDegree = 0.0f, float eyeDistance = 0.0f, float lookatDistance = 0.0f, float trackingTime = 0.3f );
		void SquareCamera( CX2SquareUnit* pFocusUnit, D3DXVECTOR3 vLookAt, float distance = 1200.0f, float height = 200.0f, float angleDegree = 0.0f, float eyeDistance = 0.0f, float lookatDistance = 0.0f, float trackingTime = 0.3f );

		//{{ seojt // 2008-10-23, 15:43
		void SquareCameraUpdate( CX2SquareUnit* pFocusUnit, float distance = 1200.0f, float height = 200.0f, float angleDegree = 0.0f, float eyeDistance = 0.0f, float lookatDistance = 0.0f );		
		//}} seojt // 2008-10-23, 15:43

#ifdef NEW_VILLAGE_RENDERING_TEST
		void SquareLineCamera( CX2SquareUnit* pFocusUnit, const D3DXVECTOR3& vEye, const D3DXVECTOR3& vLookAt, bool bFocusOnUnit = true, float trackingTime = 0.5f );
#endif NEW_VILLAGE_RENDERING_TEST
		void GameLineCamera( CX2GUUser* pFocusUnit, const D3DXVECTOR3& vEye, const D3DXVECTOR3& vLookAt, bool bFocusOnUnit = true, float trackingTime = 0.5f );


#ifdef BUBBLE_BOBBLE_TEST
		void FixedCamera( CX2GameUnit* pFocusUnit, D3DXVECTOR3 vLookAt, D3DXVECTOR3 vEye, float fXMin = 0.f, float fXMax = 1000.f, float fYMin = 0.f, float fYMax = 1000.f, 
			float distance = 1000.f, float height = 100.0f, float angleDegree = 0.0f, float eyeDistance = 0.0f, float lookatDistance = 0.0f, bool bDirect = false );
#endif BUBBLE_BOBBLE_TEST


#ifdef HEAD_ATTACHED_CAMERA_TEST
		void ManualDirectCamera( const D3DXVECTOR3& vEye, const D3DXVECTOR3& vLookAt );
#endif HEAD_ATTACHED_CAMERA_TEST


#ifdef IN_GAME_MANUAL_CAMERA_POSITION_TEST

		void CalcManualCameraPosition( CX2GameUnit* pFocusUnit, const D3DXVECTOR3& vEyePosition );
		void ManualTrackingCamera( CX2GameUnit* pFocusUnit, float distance, const D3DXVECTOR3& vRelativeEyePos );

		bool GetEnabledManualCameraPosition() const { return m_bEnabledManualCameraPosition; }
		void SetEnabledManualCameraPosition(const bool val) { m_bEnabledManualCameraPosition = val; }


		D3DXVECTOR3 GetManualCameraPosition() const { return m_vManualCameraPosition; }
		void SetManualCameraPosition(const D3DXVECTOR3 val) { m_vManualCameraPosition = val; }

#endif IN_GAME_MANUAL_CAMERA_POSITION_TEST




		CX2Camera::LineScriptedCameraData& GetLineScriptedCameraData() { return m_LineScriptedCameraData; }
#ifdef CAMERA_SCRIPTING_TEST
		const D3DXVECTOR3 CalcNormalCameraPosition( CX2GameUnit* pFocusUnit, float distance, float angleDegree, float lookatDistance, float height, float eyeDistance );
#endif CAMERA_SCRIPTING_TEST

#ifdef KEYFRAME_CAMERA
		virtual bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		
		void SetViewNowKeyFrame();
		void PrevKeyFrame();
		void NextKeyFrame();

		void AddKeyFrame();
		void SaveKeyFrame();
		void DeleteKeyFrame();

		void CreateFirstKeyFrame();

		CFirstPersonCamera* GetFPSCamera();
		


		void UpdateUIAngleInterpolation();
		void SetEnableRotateVelocity(bool bEnable);
		void SetEnableFixedCoord(bool bEnable);	

		void WorldCameraKeyProcess();
		void SetWorldCameraEdit( bool bEnable);

		bool ReLoadWorldCamera();
		bool LoadWorldCamera( const WCHAR* pFileName );
		void UnLoadWorldCamera();


		void PreProcessOnCurve( bool bWorldCameraEdit );
		bool AddWorldCameraData_LUA();



		void WorldCameraFrameMove( double fTime, float fElapsedTime );
		
		bool StartWorldCamera(int iWorldCameraID );
		void EndWorldCamera();

		bool GetEnableWorldCamera(){ return m_bEnableWorldCamera; };
		bool GetEnableWorldCameraManualPlay(){ return m_bWorldCameraManualPlay; };


		bool GetHoldViewWorldCamera(){ return m_bHoldViewWorldCamera; }
		bool GetWorldCameraEdit(){ return m_bWorldCameraEdit; }

#endif KEYFRAME_CAMERA
#ifdef NPC_TRACKINGCAMERA
		bool GetNpcTrackingCamera() {return m_bNpcTrackingCamera; }
#endif //NPC_TRACKINGCAMERA
	private:
		CAMERA_STATE	m_CameraState;
		//CKTDGCamera*	m_pCamera;
        CKTDGCamera&	m_kCamera;
		float			m_fLand;

		float			m_fAngleDegree; //부드럽게 해보장.
		float			m_fSetAngleDegree;	//셋팅하겠다는 앵글
		bool			m_bBigAngle;
		bool			m_bRight;




#ifdef BUBBLE_BOBBLE_TEST
		
		ARCADE_CAMERA_MODE	m_eArcadeCameraMode;
		D3DXVECTOR3			m_vArcadeCameraLookAt;
		D3DXVECTOR3			m_vArcadeCameraEye;

#endif BUBBLE_BOBBLE_TEST



#ifdef IN_GAME_MANUAL_CAMERA_POSITION_TEST
		bool			m_bEnabledManualCameraPosition;
		D3DXVECTOR3		m_vManualCameraPosition;
#endif IN_GAME_MANUAL_CAMERA_POSITION_TEST

		LineScriptedCameraData m_LineScriptedCameraData;		

#ifdef KEYFRAME_CAMERA
		bool					m_bWorldCameraEdit;
		bool					m_bWorldCameraManualPlay;

		bool					m_bHoldViewWorldCamera;
		float					m_fNowWorldCameraDuration;
		float					m_fNowWorldCameraElapsedTime;

		int						m_iEditKeyFrameIndex;

		bool					m_bEnableWorldCamera;
		WorldCameraData*		m_pNowPlayingWorldCamera;
		wstring					m_wstrNowWorldCameraFilename;
		vector<WorldCameraData> m_vecWorldCameraData;

		int				m_iNowKeyFrameIndex;
		float			m_fNowKeyFrameElapsedTime;
		float           m_fNowKeyFrameTotalTime;
		
		float			m_fNowKeyFrameMoveDistance;

		float			m_fNowKeyFrameTotalDistance;

		
		int				m_iNowEyeInterPositionIndex;
		float			m_iNowEyeInterPositionMoveDistance;		

		int				m_iNowLoopCount;




		int				m_iLastAngleInterpolationIndex;
		int				m_iLastMoveVelocity;
		int				m_iLastRotateTime;




		CKTDGUIDialogType		m_pDlgWorldCameraEdit;

		CKTDGUIStatic*			m_pStaticKeyFrameState;


		CKTDGUIComboBox*		m_pComboAngleInterpolation;
		CKTDGUIStatic*			m_pStaticMoveVelocity;
		CKTDGUIStatic*			m_pStaticRotateTime;

		CKTDGUIIMEEditBox*		m_pIMEEditMoveVelocity;
		CKTDGUIIMEEditBox*		m_pIMEEditRotateTime;

		CKTDGUIIMEEditBox*		m_pIMEEditCoordX;
		CKTDGUIIMEEditBox*		m_pIMEEditCoordY;
		CKTDGUIIMEEditBox*		m_pIMEEditCoordZ;
		
		int						m_iLastCoordX;
		int						m_iLastCoordY;
		int						m_iLastCoordZ;

#endif KEYFRAME_CAMERA

#ifdef NPC_TRACKINGCAMERA
		bool					m_bNpcTrackingCamera;
		float					m_fTrackingTime;
#endif
};

