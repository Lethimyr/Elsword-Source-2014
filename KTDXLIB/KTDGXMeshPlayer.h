#pragma once

/**@file  KTDGXMeshPlayer.h
   @breif 메쉬 관련
     - 애니메이션 변경
	 - 렌더 파라미터 수정
	 - 사운드
	 - 파티클
*/

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
#include    "indexed_list.h"
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE


#ifdef  X2OPTIMIZE_HANDLE_VALIDITY_CHECK

struct  CKTDGXMeshPlayer_CXMeshInstance_TAG {};
typedef KHandleType<int,CKTDGXMeshPlayer_CXMeshInstance_TAG>    CKTDGXMeshPlayer_CXMeshInstanceHandle;
#define INVALID_MESH_INSTANCE_HANDLE (CKTDGXMeshPlayer_CXMeshInstanceHandle::invalid_handle())

#else   X2OPTIMIZE_HANDLE_VALIDITY_CHECK

static const int INVALID_MESH_INSTANCE_HANDLE = -1;		/// 전역 상수; 핸들 인스턴스 없음

#endif  X2OPTIMIZE_HANDLE_VALIDITY_CHECK

#define MESHPLAYER_TOKEN_LINE_NUMBER


/** @class CKTDGXMeshPlayer
	@brief 
	  - 열거체 : MESH_TYPE, LIFE_TYPE, EVENT_TYPE, TokenType
	  - 클래스 : CXMeshPlayerToken, CXMeshInstance, CXMeshEvent, CXMeshEvent_AniName, CXMeshEvent_SlashTrace, 
				 CXMeshEvent_AniSpeed, CXMeshEvent_Color, CXMeshEvent_OutLineColor, CXMeshEvent_Position, CXMeshEvent_Size, 
				 CXMeshEvent_Velocity, CXMeshEvent_Tex0UV, CXMeshEvent_Tex1UV, CXMeshEvent_Tex2UV, CXMeshEvent_Rotate, 
				 CXMeshEvent_DirSpeed, CXMeshEvent_BlackHoleTime, CXMeshEvent_Crash, CXMeshEvent_ResetCrash, 
				 CXMeshEvent_LightFlowImpact, CXMeshEvent_LightFlowPoint, CXMeshEvent_Sound, CXMeshEvent_RenderType, 
				 CXMeshPlayerTokenizer, 
	  - 구조체 : BLENDINGMODE, 
	@date 
*/
class CKTDGXMeshPlayer
{
public: 

#ifdef  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
    typedef CKTDGXMeshPlayer_CXMeshInstanceHandle   CXMeshInstanceHandle;
#else   X2OPTIMIZE_HANDLE_VALIDITY_CHECK
	typedef int	CXMeshInstanceHandle;		/// 메쉬 인스턴스 핸들
#endif  X2OPTIMIZE_HANDLE_VALIDITY_CHECK

	public:
		/// 메쉬 형태
		enum MESH_TYPE
		{
			X_MESH,			/// 애니메이션이 없는 메쉬
			X_SKIN_MESH,	/// 애니메이션이 있는 메쉬
		};

		/// 생명 형태
		enum LIFE_TYPE
		{
			LT_PLAY_COUNT,	/// 플레이 카운트, 
			LT_TIME,		/// 시간
			LT_MANUAL,		/// 사용자 정의
		};

		/// 이벤트 형태
		enum EVENT_TYPE
		{
			ET_BASIC,				/// 기본
			ET_ANISPEED,			/// 애니메이션 속도
			ET_ANINAME,				/// 애니메이션 이름
			ET_COLOR,				/// 색상
			ET_OUTLINECOLOR,		/// 외곽선 색상
			ET_SIZE,				/// 크기
			ET_VELOCITY,			/// 속도
			ET_TEX0UV,				/// 텍스처0 UV
			ET_TEX1UV,				/// 텍스처1 UV
			ET_TEX2UV,				/// 텍스처2 UV
			ET_ROTATE,				/// 회전
			ET_DIRSPEED,			/// 방향 속도
			ET_BLACKHOLETIME,		/// 블랙홀 시간
			ET_CRASH,				/// 충돌
			ET_RESETCRASH,			/// ?
			ET_TEXTURECHANGE,		/// 텍스처 변경 // 구현 안됨
			ET_LIGHTFLOWIMPACT,		/// 불꽃 
			ET_LIGHTFLOWPOINT,		/// 
			ET_SOUND,				/// 소리
			//ET_PARTICLE,
			ET_GLOBALTIMER,			/// 전역 시간 // 구현 안됨
			ET_SLASHTRACE,			/// 휘두름 잔상
			ET_RENDERTYPE,			/// 렌더링 형태
			ET_POSITION,			/// 위치
		};

		/// 토큰 타입?
		enum TokenType 
		{
			RealNumber = 1, 
			Equals, Comma, OpenParen, CloseParen, OpenBrace, CloseBrace, Quote, 

			KeywordXMeshPlayer, KeywordTemplet,
			KeywordRandom, KeywordXYZ, KeywordXY, KeywordColor, KeywordTexture, AlphaBlendMode,

			// system property tokens
			SeqAlphaBlendModeProp, SeqLifeTypeProp, SeqMeshTypeProp, SeqBillboardTypeProp,
			SeqRenderTypeProp, SeqCartoonTexTypeProp,
			SeqCullModeProp, 
			SeqBillBoardProblem,	// 빌보드 문제 임시 수정

			// event time tokens
			KeywordGlobal, KeywordDie, KeywordAnim,
			KeywordFade, KeywordSo, KeywordAt, KeywordFrom, KeywordTo, KeywordInitial, KeywordFinal,

			// mesh properties (event types)
			MeshNumericProp, MeshVector3Prop, MeshVector2Prop, MeshColorProp, MeshStringProp,
			MeshBoolProp, MeshRenderTypeProp,

			// event sequence props
			SeqNumericProp, SeqVector3Prop, SeqBoolProp, SeqStringProp,

			UnknownToken,
		};

		/** @class CXMeshPlayerToken
			@brief  
			@date
		*/
		class CXMeshPlayerToken  
		{
			public:
				CXMeshPlayerToken() : m_strValue(), m_Type( RealNumber )
#ifdef          MESHPLAYER_TOKEN_LINE_NUMBER
                    , m_iLineNumber( -1)
#endif          MESHPLAYER_TOKEN_LINE_NUMBER
                {}
				virtual ~CXMeshPlayerToken(){}

				std::string m_strValue;
				TokenType	m_Type;
#ifdef          MESHPLAYER_TOKEN_LINE_NUMBER
                int         m_iLineNumber;
#endif          MESHPLAYER_TOKEN_LINE_NUMBER

				bool IsLifeType(void)			{ return(m_strValue.compare("LIFETYPE")				== 0); }
				bool IsDieType(void)			{ return(m_strValue.compare("DIETYPE")				== 0); }
				bool IsUseDieEvent(void)		{ return(m_strValue.compare("USEDIEEVENT")			== 0); }
				bool IsPlayCount(void)			{ return(m_strValue.compare("PLAYCOUNT")			== 0); }
				bool IsLifeTime(void)			{ return(m_strValue.compare("LIFETIME")				== 0); }
				bool IsDiePlayCount(void)		{ return(m_strValue.compare("DIEPLAYCOUNT")			== 0); }
				bool IsDieLifeTime(void)		{ return(m_strValue.compare("DIELIFETIME")			== 0); }
				bool IsLayer(void)				{ return(m_strValue.compare("LAYER")				== 0); }
				bool IsForceLayer(void)			{ return(m_strValue.compare("FORCELAYER")			== 0); }
				bool IsSlashTraceType(void)		{ return(m_strValue.compare("SLASH_TRACE_TYPE")		== 0); }
				bool IsSlashTraceCurveType(void)		{ return(m_strValue.compare("SLASH_TRACE_CURVE_TYPE")		== 0); }



				bool IsMeshType(void)			{ return(m_strValue.compare("MESHTYPE")				== 0); }
				bool IsMeshName(void)			{ return(m_strValue.compare("MESHNAME")				== 0); }
				bool IsChangeTexXET(void)		{ return(m_strValue.compare("CHANGETEXXET")			== 0); }
				bool IsMultiTexXET(void)		{ return(m_strValue.compare("MULTITEXXET")			== 0); }
				bool IsAniXET(void)				{ return(m_strValue.compare("ANIPACKAGE")			== 0); }
				bool IsSlashTraceTextureName(void) { return( m_strValue.compare("SLASH_TRACE_TEXTURE") == 0 ); }

				bool IsRenderType(void)			{ return(m_strValue.compare("RENDERTYPE")			== 0); }
				bool IsCartoonTexType(void)		{ return(m_strValue.compare("CARTOONTEXTYPE")		== 0); }
				bool IsSkinRenderType(void)		{ return(m_strValue.compare("SKINRENDERTYPE")		== 0); }
				bool IsSkinCartoonTexType(void)	{ return(m_strValue.compare("SKINCARTOONTEXTYPE")	== 0); }

				bool IsLightPos(void)			{ return(m_strValue.compare("LIGHTPOS")				== 0); }
				bool IsOutLineWide(void)		{ return(m_strValue.compare("OUTLINEWIDE")			== 0); }
				bool IsLightFlowWide(void)		{ return(m_strValue.compare("LIGHTFLOWWIDE")		== 0); }
				
				bool IsZEnable(void)			{ return(m_strValue.compare("ZENABLE")				== 0); }
				bool IsZWriteEnable(void)		{ return(m_strValue.compare("ZWRITEENABLE")			== 0); }
				bool IsCullMode(void)			{ return(m_strValue.compare("CULLMODE")				== 0); }
				bool IsSrcBlendMode(void)		{ return(m_strValue.compare("SOURCEBLENDMODE")		== 0); }
				bool IsDestBlendMode(void)		{ return(m_strValue.compare("DESTBLENDMODE")		== 0); }
				bool IsDrawCount(void)			{ return(m_strValue.compare("DRAWCOUNT")			== 0); }
				bool IsBillboardType(void)		{ return(m_strValue.compare("BILLBOARDTYPE")		== 0); }
				
				bool IsGravity(void)			{ return(m_strValue.compare("GRAVITY")				== 0); }
				bool IsApplyGravity(void)       { return(m_strValue.compare("APPLY_GRAVITY")		== 0); }
				bool IsBlackHolePos(void)		{ return(m_strValue.compare("BLACKHOLEPOS")			== 0); }
				bool IsLandPos(void)			{ return(m_strValue.compare("LANDPOS")				== 0); }
				bool IsUseLand(void)			{ return(m_strValue.compare("USELAND")				== 0); }
				
				bool IsSetLandHeightOnStart(void)	{ return(m_strValue.compare("SET_LAND_HEIGHT_ON_START")				== 0); }

				bool IsElasticCoeff(void)		{ return(m_strValue.compare("ELASTICCOEFF")			== 0); }
				bool IsUseSlashTrace(void)		{ return(m_strValue.compare("USESLASHTRACE")		== 0); }
				bool IsCrashLand(void)			{ return(m_strValue.compare("CRASHLAND")			== 0); }

				bool IsNotCheckLandTime(void)   { return(m_strValue.compare("NOT_CHECK_LAND_TIME")	== 0); }
				bool IsElasticCoeffX(void)		{ return(m_strValue.compare("ELASTICCOEFF_X")			== 0); }
#ifdef PARTICLE_NOTAPPLY_UNITSCALE
				bool IsApplyUnitScale(void)		{ return(m_strValue.compare("APPLYUNITSCALE")		== 0); }
#endif
#ifdef FIX_TEMP_MESH_BILLBOARD //2013.10.09
				bool IsUseMeshBillBoard(void)		{ return( m_strValue.compare( "USEMESHBILLBOARD" ) == 0 ); }
#endif //FIX_TEMP_MESH_BILLBOARD

#ifdef ADD_ALPHATESTENABLE
				bool IsAlphatest(void)		{ return( m_strValue.compare( "ALPHATEST" ) == 0 ); }
#endif //ADD_ALPHATESTENABLE
		};

		class CXMeshInstance;
		class CXMeshEvent
		{
			public:
				CXMeshEvent( bool final = false )
				{
					m_EventType = ET_BASIC;
					m_bFinal = final;
					m_bFade = false;
				}
				virtual ~CXMeshEvent(){}

				EVENT_TYPE		GetEventType(){ return m_EventType; }

				bool			IsFade()					{ return m_bFade; }
				void			SetFade( bool fade = true )	{ m_bFade = fade; }

				bool			IsFinal()						{ return m_bFinal; }
				void			SetFinal( bool final = true )	{ m_bFinal = final; }
				void			SetFinalTime( float fFinalTime )
				{ 
					if( m_bFinal == true )
						m_ActualTime.m_Max = fFinalTime; 
				}

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
                virtual void	OnFrameMove( CXMeshInstance* pInstance, float fElapsedTime, float fOldTime, float fNowTime, bool bAniTime ) = 0;
#else   X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
				virtual void	OnFrameMove( CXMeshInstance* pInstance, float fElapsedTime, float fNowTime ) = 0;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
				virtual bool	FadeAllowed() = 0;

				CMinMax<float>	GetActualTime(){ return m_ActualTime; }
				void SetActualTime( CMinMax<float> actualTime ){ m_ActualTime = actualTime; }

				virtual bool ProcessTokenStream(std::vector<CXMeshPlayerToken>::iterator &TokenIter, 
					std::vector<CXMeshPlayerToken>::iterator &EndIter) = 0;

				static void ProcessPropEqualsValue(CMinMax<float> &prop,
					std::vector<CXMeshPlayerToken>::iterator &TokenIter, 
					std::vector<CXMeshPlayerToken>::iterator &EndIter);

				static void ProcessPropEqualsValue(CMinMax<D3DXVECTOR3> &prop,
					std::vector<CXMeshPlayerToken>::iterator &TokenIter, 
					std::vector<CXMeshPlayerToken>::iterator &EndIter);

				static void ProcessPropEqualsValue(CMinMax<D3DXVECTOR2> &prop,
					std::vector<CXMeshPlayerToken>::iterator &TokenIter, 
					std::vector<CXMeshPlayerToken>::iterator &EndIter);

				static void ProcessPropEqualsValue(CMinMax<D3DXCOLOR> &prop,
					std::vector<CXMeshPlayerToken>::iterator &TokenIter, 
					std::vector<CXMeshPlayerToken>::iterator &EndIter);

				static void ProcessPropEqualsValue(wstring &prop,
					std::vector<CXMeshPlayerToken>::iterator &TokenIter, 
					std::vector<CXMeshPlayerToken>::iterator &EndIter);

				static void ProcessPropEqualsValue(bool &prop,
					std::vector<CXMeshPlayerToken>::iterator &TokenIter, 
					std::vector<CXMeshPlayerToken>::iterator &EndIter);

				static void ProcessPropEqualsValue(CKTDGXRenderer::RENDER_TYPE &prop,
					std::vector<CXMeshPlayerToken>::iterator &TokenIter, 
					std::vector<CXMeshPlayerToken>::iterator &EndIter);

			protected:
				EVENT_TYPE		m_EventType;
				bool			m_bFade;
				bool			m_bFinal;
				CMinMax<float>	m_ActualTime;
		};

		class CXMeshEvent_AniName : public CXMeshEvent
		{
			public:
				CXMeshEvent_AniName( bool final = false ) : m_fPlaySpeed( 0.f ),
					CXMeshEvent(final){ m_EventType = ET_ANINAME; m_PlayType = CKTDGXSkinAnim::XAP_LOOP; }

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
                virtual void	OnFrameMove( CXMeshInstance* pInstance, float fElapsedTime, float fOldTime, float fNowTime, bool bAniTime );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
				virtual void	OnFrameMove( CXMeshInstance* pInstance, float fElapsedTime, float fNowTime );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
				virtual bool FadeAllowed() { return false; }

				const WCHAR*	GetAniName()						{ return m_AniName.c_str(); }
				void			SetAniName(const WCHAR* pAniName)	{ m_AniName = pAniName; }
				CKTDGXSkinAnim::XSKIN_ANIM_PLAYTYPE	GetPlayType()	{ return m_PlayType; }
#ifdef EVENT_SCENE
				void			SetPlayType(CKTDGXSkinAnim::XSKIN_ANIM_PLAYTYPE ePlayType){ m_PlayType = ePlayType; }
				void			SetPlaySpeed(float fPlaySpeed){ m_fPlaySpeed = fPlaySpeed; }
#endif EVENT_SCENE
				virtual bool ProcessTokenStream(std::vector<CXMeshPlayerToken>::iterator &TokenIter, 
												std::vector<CXMeshPlayerToken>::iterator &EndIter);
		
			protected:
				wstring								m_AniName;
				CKTDGXSkinAnim::XSKIN_ANIM_PLAYTYPE	m_PlayType;
#ifdef EVENT_SCENE
				float								m_fPlaySpeed;
#endif EVENT_SCENE
		};


		class CXMeshEvent_SlashTrace : public CXMeshEvent
		{
		public:
			CXMeshEvent_SlashTrace( bool final = false ) : m_bSlashTrace( false ), CXMeshEvent(final){ m_EventType = ET_SLASHTRACE; }

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
                virtual void	OnFrameMove( CXMeshInstance* pInstance, float fElapsedTime, float fOldTime, float fNowTime, bool bAniTime );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
				virtual void	OnFrameMove( CXMeshInstance* pInstance, float fElapsedTime, float fNowTime );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
			virtual bool FadeAllowed() { return false; }
#ifdef EFFECT_TOOL
			void SetSlashTrace( bool bVal_ ) { m_bSlashTrace = bVal_; }
#endif // EFFECT_TOOL

			bool GetSlashTrace() { return m_bSlashTrace; }

			virtual bool ProcessTokenStream(std::vector<CXMeshPlayerToken>::iterator &TokenIter, 
				std::vector<CXMeshPlayerToken>::iterator &EndIter);

		protected:
			bool m_bSlashTrace;
		};


		class CXMeshEvent_AniSpeed : public CXMeshEvent
		{
			public:
				CXMeshEvent_AniSpeed( bool final = false ) : CXMeshEvent(final){ m_EventType = ET_ANISPEED; }

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
                virtual void	OnFrameMove( CXMeshInstance* pInstance, float fElapsedTime, float fOldTime, float fNowTime, bool bAniTime );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
				virtual void	OnFrameMove( CXMeshInstance* pInstance, float fElapsedTime, float fNowTime );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
				virtual bool FadeAllowed() { return false; }

				CMinMax<float>	GetAniSpeed()							{ return m_AniSpeed; }
				void			SetAniSpeed(const CMinMax<float> &data)	{ m_AniSpeed = data; }

				virtual bool ProcessTokenStream(std::vector<CXMeshPlayerToken>::iterator &TokenIter, 
												std::vector<CXMeshPlayerToken>::iterator &EndIter);

			protected:
				CMinMax<float> m_AniSpeed;
		};

		class CXMeshEvent_Color : public CXMeshEvent
		{
			public:
				CXMeshEvent_Color( bool final = false ) : CXMeshEvent(final){ m_EventType = ET_COLOR; }

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
                virtual void	OnFrameMove( CXMeshInstance* pInstance, float fElapsedTime, float fOldTime, float fNowTime, bool bAniTime );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
				virtual void	OnFrameMove( CXMeshInstance* pInstance, float fElapsedTime, float fNowTime );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
				virtual bool FadeAllowed() { return true; }

				CMinMax<D3DXCOLOR>	GetColor()									{ return m_Color; }
				void				SetColor(const CMinMax<D3DXCOLOR> &data)	{ m_Color = data; }

				virtual bool ProcessTokenStream(std::vector<CXMeshPlayerToken>::iterator &TokenIter, 
												std::vector<CXMeshPlayerToken>::iterator &EndIter);


			protected:
				CMinMax<D3DXCOLOR> m_Color;
		};

		class CXMeshEvent_OutLineColor : public CXMeshEvent
		{
			public:
				CXMeshEvent_OutLineColor( bool final = false ) : CXMeshEvent(final){ m_EventType = ET_OUTLINECOLOR; }

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
                virtual void	OnFrameMove( CXMeshInstance* pInstance, float fElapsedTime, float fOldTime, float fNowTime, bool bAniTime );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
				virtual void	OnFrameMove( CXMeshInstance* pInstance, float fElapsedTime, float fNowTime );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
				virtual bool FadeAllowed() { return true; }

				CMinMax<D3DXCOLOR>	GetOutLineColor()									{ return m_OutLineColor; }
				void				SetOutLineColor(const CMinMax<D3DXCOLOR> &data)	{ m_OutLineColor = data; }

				virtual bool ProcessTokenStream(std::vector<CXMeshPlayerToken>::iterator &TokenIter, 
					std::vector<CXMeshPlayerToken>::iterator &EndIter);


			protected:
				CMinMax<D3DXCOLOR> m_OutLineColor;
		};

#ifdef EVENT_SCENE

		class CXMeshEvent_Position : public CXMeshEvent
		{
		public:
			CXMeshEvent_Position( bool final = false ) : CXMeshEvent(final){ m_EventType = ET_POSITION; }

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
            virtual void	OnFrameMove( CXMeshInstance* pInstance, float fElapsedTime, float fOldTime, float fNowTime, bool bAniTime );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
			virtual void	OnFrameMove( CXMeshInstance* pInstance, float fElapsedTime, float fNowTime );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
			virtual bool FadeAllowed(){ return true; }

			CMinMax<D3DXVECTOR3>	GetPosition()									{ return m_Position; }
			void					SetPosition(CMinMax<D3DXVECTOR3> &data)	{ m_Position = data; }

			virtual bool ProcessTokenStream(std::vector<CXMeshPlayerToken>::iterator &TokenIter, 
				std::vector<CXMeshPlayerToken>::iterator &EndIter);

		protected:
			CMinMax<D3DXVECTOR3> m_Position;
		};
#endif EVENT_SCENE


		class CXMeshEvent_Size : public CXMeshEvent
		{
			public:
				CXMeshEvent_Size( bool final = false ) : CXMeshEvent(final){ m_EventType = ET_SIZE; }

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
                virtual void	OnFrameMove( CXMeshInstance* pInstance, float fElapsedTime, float fOldTime, float fNowTime, bool bAniTime );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
			    virtual void	OnFrameMove( CXMeshInstance* pInstance, float fElapsedTime, float fNowTime );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
				virtual bool FadeAllowed() { return true; }

				CMinMax<D3DXVECTOR3>	GetSize()									{ return m_Size; }
				void					SetSize(const CMinMax<D3DXVECTOR3> &data)	{ m_Size = data; }

				virtual bool ProcessTokenStream(std::vector<CXMeshPlayerToken>::iterator &TokenIter, 
					std::vector<CXMeshPlayerToken>::iterator &EndIter);


			protected:
				CMinMax<D3DXVECTOR3> m_Size;
		};

		class CXMeshEvent_Velocity : public CXMeshEvent
		{
			public:
				CXMeshEvent_Velocity( bool final = false ) : CXMeshEvent(final){ m_EventType = ET_VELOCITY; }

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
                virtual void	OnFrameMove( CXMeshInstance* pInstance, float fElapsedTime, float fOldTime, float fNowTime, bool bAniTime );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
			    virtual void	OnFrameMove( CXMeshInstance* pInstance, float fElapsedTime, float fNowTime );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
				virtual bool FadeAllowed() { return true; }

				CMinMax<D3DXVECTOR3>	GetVelocity()									{ return m_Velocity; }
				void					SetVelocity(const CMinMax<D3DXVECTOR3> &data)	{ m_Velocity = data; }

				virtual bool ProcessTokenStream(std::vector<CXMeshPlayerToken>::iterator &TokenIter, 
					std::vector<CXMeshPlayerToken>::iterator &EndIter);


			protected:
				CMinMax<D3DXVECTOR3> m_Velocity;
		};

		class CXMeshEvent_Tex0UV : public CXMeshEvent
		{
			public:
				CXMeshEvent_Tex0UV( bool final = false ) : CXMeshEvent(final){ m_EventType = ET_TEX0UV; }

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
                virtual void	OnFrameMove( CXMeshInstance* pInstance, float fElapsedTime, float fOldTime, float fNowTime, bool bAniTime );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
			    virtual void	OnFrameMove( CXMeshInstance* pInstance, float fElapsedTime, float fNowTime );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
				virtual bool FadeAllowed() { return true; }

				CMinMax<D3DXVECTOR2>	GetTex0UV()									{ return m_Tex0UV; }
				void					SetTex0UV(const CMinMax<D3DXVECTOR2> &data)	{ m_Tex0UV = data; }

				virtual bool ProcessTokenStream(std::vector<CXMeshPlayerToken>::iterator &TokenIter, 
					std::vector<CXMeshPlayerToken>::iterator &EndIter);


			protected:
				CMinMax<D3DXVECTOR2> m_Tex0UV;
		};

		class CXMeshEvent_Tex1UV : public CXMeshEvent
		{
			public:
				CXMeshEvent_Tex1UV( bool final = false ) : CXMeshEvent(final){ m_EventType = ET_TEX1UV; }

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
                virtual void	OnFrameMove( CXMeshInstance* pInstance, float fElapsedTime, float fOldTime, float fNowTime, bool bAniTime );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
			    virtual void	OnFrameMove( CXMeshInstance* pInstance, float fElapsedTime, float fNowTime );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
				virtual bool FadeAllowed() { return true; }

				CMinMax<D3DXVECTOR2>	GetTex1UV()									{ return m_Tex1UV; }
				void					SetTex1UV(const CMinMax<D3DXVECTOR2> &data)	{ m_Tex1UV = data; }

				virtual bool ProcessTokenStream(std::vector<CXMeshPlayerToken>::iterator &TokenIter, 
					std::vector<CXMeshPlayerToken>::iterator &EndIter);


			protected:
				CMinMax<D3DXVECTOR2> m_Tex1UV;
		};

		class CXMeshEvent_Tex2UV : public CXMeshEvent
		{
			public:
				CXMeshEvent_Tex2UV( bool final = false ) : CXMeshEvent(final){ m_EventType = ET_TEX2UV; }

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
                virtual void	OnFrameMove( CXMeshInstance* pInstance, float fElapsedTime, float fOldTime, float fNowTime, bool bAniTime );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
			    virtual void	OnFrameMove( CXMeshInstance* pInstance, float fElapsedTime, float fNowTime );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
				virtual bool FadeAllowed() { return true; }

				CMinMax<D3DXVECTOR2>	GetTex2UV()									{ return m_Tex2UV; }
				void					SetTex2UV(const CMinMax<D3DXVECTOR2> &data)	{ m_Tex2UV = data; }

				virtual bool ProcessTokenStream(std::vector<CXMeshPlayerToken>::iterator &TokenIter, 
					std::vector<CXMeshPlayerToken>::iterator &EndIter);


			protected:
				CMinMax<D3DXVECTOR2> m_Tex2UV;
		};

		class CXMeshEvent_Rotate : public CXMeshEvent
		{
			public:
				CXMeshEvent_Rotate( bool final = false ) : CXMeshEvent(final){ m_EventType = ET_ROTATE; }

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
                virtual void	OnFrameMove( CXMeshInstance* pInstance, float fElapsedTime, float fOldTime, float fNowTime, bool bAniTime );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
			    virtual void	OnFrameMove( CXMeshInstance* pInstance, float fElapsedTime, float fNowTime );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
				virtual bool FadeAllowed() { return true; }

				CMinMax<D3DXVECTOR3>	GetRotate()									{ return m_Rotate; }
				void					SetRotate(const CMinMax<D3DXVECTOR3> &data)	{ m_Rotate = data; }

				virtual bool ProcessTokenStream(std::vector<CXMeshPlayerToken>::iterator &TokenIter, 
					std::vector<CXMeshPlayerToken>::iterator &EndIter);


			protected:
				CMinMax<D3DXVECTOR3> m_Rotate;
		};

		class CXMeshEvent_DirSpeed : public CXMeshEvent
		{
			public:
				CXMeshEvent_DirSpeed( bool final = false ) : CXMeshEvent(final){ m_EventType = ET_DIRSPEED; }

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
                virtual void	OnFrameMove( CXMeshInstance* pInstance, float fElapsedTime, float fOldTime, float fNowTime, bool bAniTime );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
			    virtual void	OnFrameMove( CXMeshInstance* pInstance, float fElapsedTime, float fNowTime );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
				virtual bool FadeAllowed() { return true; }

				CMinMax<float>			GetDirSpeed()									{ return m_DirSpeed; }
				void					SetDirSpeed(const CMinMax<float> &data)	{ m_DirSpeed = data; }

				virtual bool ProcessTokenStream(std::vector<CXMeshPlayerToken>::iterator &TokenIter, 
					std::vector<CXMeshPlayerToken>::iterator &EndIter);


			protected:
				CMinMax<float> m_DirSpeed;
		};

		class CXMeshEvent_BlackHoleTime : public CXMeshEvent
		{
			public:
				CXMeshEvent_BlackHoleTime( bool final = false ) : CXMeshEvent(final){ m_EventType = ET_BLACKHOLETIME; }

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
                virtual void	OnFrameMove( CXMeshInstance* pInstance, float fElapsedTime, float fOldTime, float fNowTime, bool bAniTime );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
			    virtual void	OnFrameMove( CXMeshInstance* pInstance, float fElapsedTime, float fNowTime );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
				virtual bool FadeAllowed() { return false; }

				CMinMax<float>	GetBlackHoleTime()								{ return m_BlackHoleTime; }
				void			SetBlackHoleTime(const CMinMax<float> &data)	{ m_BlackHoleTime = data; }

				virtual bool ProcessTokenStream(std::vector<CXMeshPlayerToken>::iterator &TokenIter, 
					std::vector<CXMeshPlayerToken>::iterator &EndIter);


			protected:
				CMinMax<float> m_BlackHoleTime;
		};

		class CXMeshEvent_Crash : public CXMeshEvent
		{
			public:
				CXMeshEvent_Crash( bool final = false ) : CXMeshEvent(final){ m_EventType = ET_CRASH; }

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
                virtual void	OnFrameMove( CXMeshInstance* pInstance, float fElapsedTime, float fOldTime, float fNowTime, bool bAniTime );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
			    virtual void	OnFrameMove( CXMeshInstance* pInstance, float fElapsedTime, float fNowTime );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
				virtual bool FadeAllowed() { return true; }

				CMinMax<D3DXVECTOR3>	GetCrash()									{ return m_Crash; }
				void					SetCrash(const CMinMax<D3DXVECTOR3> &data)	{ m_Crash = data; }

				virtual bool ProcessTokenStream(std::vector<CXMeshPlayerToken>::iterator &TokenIter, 
					std::vector<CXMeshPlayerToken>::iterator &EndIter);


			protected:
				CMinMax<D3DXVECTOR3> m_Crash;
		};

		class CXMeshEvent_ResetCrash : public CXMeshEvent
		{
			public:
				CXMeshEvent_ResetCrash( bool final = false ) : CXMeshEvent(final){ m_EventType = ET_RESETCRASH; }

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
                virtual void	OnFrameMove( CXMeshInstance* pInstance, float fElapsedTime, float fOldTime, float fNowTime, bool bAniTime );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
			    virtual void	OnFrameMove( CXMeshInstance* pInstance, float fElapsedTime, float fNowTime );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
				virtual bool FadeAllowed() { return false; }

				CMinMax<float>	GetResetCrash()								{ return m_ResetCrash; }
				void			SetResetCrash(const CMinMax<float> &data)	{ m_ResetCrash = data; }

				virtual bool ProcessTokenStream(std::vector<CXMeshPlayerToken>::iterator &TokenIter, 
					std::vector<CXMeshPlayerToken>::iterator &EndIter);


			protected:
				CMinMax<float> m_ResetCrash;
		};

		class CXMeshEvent_LightFlowImpact : public CXMeshEvent
		{
			public:
				CXMeshEvent_LightFlowImpact( bool final = false ) : CXMeshEvent(final){ m_EventType = ET_LIGHTFLOWIMPACT; }

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
                virtual void	OnFrameMove( CXMeshInstance* pInstance, float fElapsedTime, float fOldTime, float fNowTime, bool bAniTime );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
			    virtual void	OnFrameMove( CXMeshInstance* pInstance, float fElapsedTime, float fNowTime );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
				virtual bool FadeAllowed() { return true; }

				CMinMax<float>	GetLightFlowImpact()							{ return m_LightFlowImpact; }
				void			SetLightFlowImpact(const CMinMax<float> &data)	{ m_LightFlowImpact = data; }

				virtual bool ProcessTokenStream(std::vector<CXMeshPlayerToken>::iterator &TokenIter, 
					std::vector<CXMeshPlayerToken>::iterator &EndIter);


			protected:
				CMinMax<float> m_LightFlowImpact;
		};

		class CXMeshEvent_LightFlowPoint : public CXMeshEvent
		{
			public:
				CXMeshEvent_LightFlowPoint( bool final = false ) : CXMeshEvent(final){ m_EventType = ET_LIGHTFLOWPOINT; }

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
                virtual void	OnFrameMove( CXMeshInstance* pInstance, float fElapsedTime, float fOldTime, float fNowTime, bool bAniTime );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
			    virtual void	OnFrameMove( CXMeshInstance* pInstance, float fElapsedTime, float fNowTime );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
				virtual bool FadeAllowed() { return true; }

				CMinMax<D3DXVECTOR3>	GetLightFlowPoint()										{ return m_LightFlowPoint; }
				void					SetLightFlowPoint(const CMinMax<D3DXVECTOR3> &data)		{ m_LightFlowPoint = data; }

				virtual bool ProcessTokenStream(std::vector<CXMeshPlayerToken>::iterator &TokenIter, 
					std::vector<CXMeshPlayerToken>::iterator &EndIter);


			protected:
				CMinMax<D3DXVECTOR3> m_LightFlowPoint;
		};

		class CXMeshEvent_Sound : public CXMeshEvent
		{
			public:
				CXMeshEvent_Sound( bool final = false ) : CXMeshEvent(final){ m_EventType = ET_SOUND; }

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
                virtual void	OnFrameMove( CXMeshInstance* pInstance, float fElapsedTime, float fOldTime, float fNowTime, bool bAniTime );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
			    virtual void	OnFrameMove( CXMeshInstance* pInstance, float fElapsedTime, float fNowTime );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
				virtual bool FadeAllowed() { return false; }

				const WCHAR*	GetSoundName()					{ return m_SoundName.c_str(); }
				void			SetSoundName(const WCHAR* data)	{ m_SoundName = data; }

				virtual bool ProcessTokenStream(std::vector<CXMeshPlayerToken>::iterator &TokenIter, 
					std::vector<CXMeshPlayerToken>::iterator &EndIter);


			protected:
				wstring m_SoundName;
		};

		class CXMeshEvent_RenderType : public CXMeshEvent
		{
		public:
			CXMeshEvent_RenderType( bool final = false ) : m_RenderType( CKTDGXRenderer::RT_REAL_COLOR ), CXMeshEvent(final){ m_EventType = ET_RENDERTYPE; }

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
            virtual void	OnFrameMove( CXMeshInstance* pInstance, float fElapsedTime, float fOldTime, float fNowTime, bool bAniTime );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
			virtual void	OnFrameMove( CXMeshInstance* pInstance, float fElapsedTime, float fNowTime );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
			virtual bool FadeAllowed() { return false; }

			const CKTDGXRenderer::RENDER_TYPE	GetRenderType()					{ return m_RenderType; }
			void			SetRenderType(const CKTDGXRenderer::RENDER_TYPE data)	{ m_RenderType = data; }

			virtual bool ProcessTokenStream(std::vector<CXMeshPlayerToken>::iterator &TokenIter, 
				std::vector<CXMeshPlayerToken>::iterator &EndIter);


		protected:
			CKTDGXRenderer::RENDER_TYPE m_RenderType;
		};

		struct XMeshTemplet
		{
			//메인 정보
			wstring						templetName;
			int							layer;
			bool						bForceLayer;

			//라이프 정보
			LIFE_TYPE					lifeType;
			LIFE_TYPE					dieType;
			bool						bUseDieEvent;

			CMinMax<int>				playCount;
			CMinMax<float>				lifeTime;
			CMinMax<int>				diePlayCount;
			CMinMax<float>				dieLifeTime;

			//메시 정보
			MESH_TYPE					meshType;
			wstring						meshName;
			wstring						changeTexXETName;
			wstring						multiTexXETName;
			wstring						aniXETName;

			//메시랜더 정보
			CKTDGXRenderer::RENDER_TYPE				renderType;
			CKTDGXRenderer::CARTOON_TEX_TYPE		cartoonTexType;

			CMinMax<D3DXVECTOR3>			lightPos;
			CMinMax<float>					outLineWide;
			CMinMax<float>					lightFlowWide;
			
			bool							bZEnable;
			bool							bZWriteEnable;
			D3DCULL							cullMode;
			D3DBLEND						srcBlend;
			D3DBLEND						destBlend;
			
			CMinMax<int>					drawCount;
			CKTDGMatrix::BILLBOARD_TYPE		billBoardType;
			
			//움직임 정보
			CMinMax<D3DXVECTOR3>			gravity;
			CMinMax<D3DXVECTOR3>			blackHolePosition;
			CMinMax<float>					landPosition;
			bool							bSetLandHeightOnStart;		// meshplayer가 생성될 때 생성되는 위치에서 라인 높이를 계산한다
			bool							bUseLand;
			bool							bCrashLand;			
			CMinMax<float>					elasticCoeff;
			CMinMax<float>					elasticCoeffX;

			vector<CXMeshEvent*>			globalTimeEventList;
			vector<CXMeshEvent*>			animTimeEventList;
			vector<CXMeshEvent*>			dieEventList;

			bool							bUseSlashTrace;
			int								iSlashTraceType;
			std::wstring					slashTraceTextureName;
			CKTDGSlashTrace::CURVE_TYPE		slashTraceCurveType;

			float							fNotCheckLandTime;

#ifdef PARTICLE_NOTAPPLY_UNITSCALE
			bool							bApplyUnitScale;
#endif //PARTICLE_NOTAPPLY_UNITSCALE

#ifdef FIX_TEMP_MESH_BILLBOARD //2013.10.09
			bool	bUseMeshBillBoard;
#endif //FIX_TEMP_MESH_BILLBOARD

#ifdef ADD_ALPHATESTENABLE
			bool	bAlphaTestEnable;
#endif

			XMeshTemplet()
			{
				//메인 정보
				layer				= 0;
				bForceLayer			= false;

				//라이프 정보
				lifeType			= LT_PLAY_COUNT;
				dieType				= LT_PLAY_COUNT;
				bUseDieEvent		= false;

				playCount			= CMinMax<int>(0,0);
				lifeTime			= CMinMax<float>(0.0f,0.0f);
				diePlayCount		= CMinMax<int>(0,0);
				dieLifeTime			= CMinMax<float>(0.0f,0.0f);

				//메시 정보
				meshType			= X_SKIN_MESH;

				//메시랜더 정보
				renderType			= CKTDGXRenderer::RT_REAL_COLOR;
				cartoonTexType		= CKTDGXRenderer::CTT_NORMAL;

				lightPos			= CMinMax<D3DXVECTOR3>(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
				outLineWide			= CMinMax<float>(0.0f,0.0f);
				lightFlowWide		= CMinMax<float>(0.0f,0.0f);

				bZEnable			= false;
				bZWriteEnable		= false;
				cullMode			= D3DCULL_NONE;
				srcBlend			= D3DBLEND_SRCALPHA;
				destBlend			= D3DBLEND_INVSRCALPHA;

				drawCount			= CMinMax<int>(1,1);
				billBoardType		= CKTDGMatrix::BT_NONE;

				//움직임 정보
				gravity				= CMinMax<D3DXVECTOR3>(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
				blackHolePosition	= CMinMax<D3DXVECTOR3>(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
				landPosition		= CMinMax<float>(0.0f,0.0f);
				bSetLandHeightOnStart = false;
				bUseLand			= false;
				bCrashLand			= false;
				elasticCoeff		= CMinMax<float>(0.5f, 0.5f);
				elasticCoeffX		= CMinMax<float>(0.f, 0.f);

				bUseSlashTrace		= false;
				iSlashTraceType		= (int) CKTDGSlashTrace::STT_DEFAULT;
				slashTraceTextureName	= L"";
				slashTraceCurveType = CKTDGSlashTrace::CT_SMOOTH_CURVE;

				fNotCheckLandTime	= 0.f;
#ifdef PARTICLE_NOTAPPLY_UNITSCALE
				bApplyUnitScale		= true;
#endif //PARTICLE_NOTAPPLY_UNITSCALE
#ifdef FIX_TEMP_MESH_BILLBOARD //2013.10.09
				bUseMeshBillBoard	= false;
#endif //FIX_TEMP_MESH_BILLBOARD
#ifdef ADD_ALPHATESTENABLE
				bAlphaTestEnable	= false;
#endif //ADD_ALPHATESTENABLE
			}
			~XMeshTemplet()
			{
				for( int i = 0; i < (int)globalTimeEventList.size(); i++ )
				{
					SAFE_DELETE( globalTimeEventList[i] );
				}
				globalTimeEventList.clear();
				for( int i = 0; i < (int)animTimeEventList.size(); i++ )
				{
					SAFE_DELETE( animTimeEventList[i] );
				}
				animTimeEventList.clear();
				for( int i = 0; i < (int)dieEventList.size(); i++ )
				{
					SAFE_DELETE( dieEventList[i] );
				}
				dieEventList.clear();
			}


#if defined(EFFECT_TOOL) || defined(EXPAND_DEVELOPER_SCRIPT)
// #ifdef EFFECT_TOOL
			static bool IsSamef_( float a, float b = 0.f )
			{
				if( fabs( a - b ) > 0.0009 )
					return false;
				return true;
			}
			static bool IsSameMinMax( CMinMax<float> lhs_, CMinMax<float> rhs_ )
			{
				return true == IsSamef_(lhs_.m_Max, rhs_.m_Max ) && true == IsSamef_( lhs_.m_Min, rhs_.m_Min ) ;
			}
			static bool IsSameMinMax( CMinMax<int> lhs_, CMinMax<int> rhs_ )
			{
				return (lhs_.m_Max == rhs_.m_Max ) && ( lhs_.m_Min == rhs_.m_Min ) ;
			}
			static bool IsSameMinMax( CMinMax<D3DXVECTOR3> lhs_, CMinMax<D3DXVECTOR3> rhs_ )
			{
				return true == IsSamef_( lhs_.m_Max.x, rhs_.m_Max.x ) &&
					true == IsSamef_( lhs_.m_Max.y,  rhs_.m_Max.y ) &&
					true == IsSamef_( lhs_.m_Max.z, rhs_.m_Max.z );
			}
			bool operator==( const XMeshTemplet& rhs_ ) const
			{

				if( templetName				!= rhs_.templetName) return false;
				if( layer					!= rhs_.layer) return false;
				if( bForceLayer				!= rhs_.bForceLayer) return false;
				
				if( lifeType				!= rhs_.lifeType) return false;
				if( dieType					!= rhs_.dieType) return false;
				if( bUseDieEvent			!= rhs_.bUseDieEvent) return false;
			
				if( meshType				!= rhs_.meshType) return false;
				if( meshName				!= rhs_.meshName) return false;
				if( changeTexXETName		!= rhs_.changeTexXETName) return false;
				if( multiTexXETName			!= rhs_.multiTexXETName) return false;
				if( aniXETName				!= rhs_.aniXETName) return false;
				
				if( renderType				!= rhs_.renderType) return false;
				if( cartoonTexType			!= rhs_.cartoonTexType) return false;
				
				if( bZEnable				!= rhs_.bZEnable) return false;
				if( bZWriteEnable			!= rhs_.bZWriteEnable) return false;
				if( cullMode				!= rhs_.cullMode) return false;
				if( srcBlend				!= rhs_.srcBlend) return false;
				if( destBlend				!= rhs_.destBlend) return false;
				
				if( bSetLandHeightOnStart	!= rhs_.bSetLandHeightOnStart) return false;
				if( bUseLand				!= rhs_.bUseLand) return false;
				if( bCrashLand				!= rhs_.bCrashLand) return false;
				
				if( bUseSlashTrace			!= rhs_.bUseSlashTrace) return false;
				if( iSlashTraceType			!= rhs_.iSlashTraceType) return false;
				if( slashTraceTextureName	!= rhs_.slashTraceTextureName) return false;
				if( slashTraceCurveType		!= rhs_.slashTraceCurveType) return false;
				
				if( fNotCheckLandTime		!= rhs_.fNotCheckLandTime) return false;
#ifdef PARTICLE_NOTAPPLY_UNITSCALE
				if( bApplyUnitScale			!= rhs_.bApplyUnitScale) return false;
#endif //PARTICLE_NOTAPPLY_UNITSCALE
				if( billBoardType			!= rhs_.billBoardType) return false;
#ifdef FIX_TEMP_MESH_BILLBOARD //2013.10.09
				if( bUseMeshBillBoard		!= rhs_.bUseMeshBillBoard ) return false;
#endif //FIX_TEMP_MESH_BILLBOARD
#ifdef ADD_ALPHATESTENABLE
				if( bAlphaTestEnable		!= rhs_.bAlphaTestEnable ) return false;
#endif //ADD_ALPHATESTENABLE



				if( false == IsSameMinMax( playCount		, rhs_.playCount)) return false;
				if( false == IsSameMinMax( lifeTime			, rhs_.lifeTime)) return false;
				if( false == IsSameMinMax( diePlayCount		, rhs_.diePlayCount)) return false;	
				if( false == IsSameMinMax( dieLifeTime		, rhs_.dieLifeTime)) return false;	
				
				if( false == IsSameMinMax( elasticCoeff		, rhs_.elasticCoeff)) return false;
				if( false == IsSameMinMax( elasticCoeffX	, rhs_.elasticCoeffX)) return false;
				
				if( false == IsSameMinMax( drawCount		, rhs_.drawCount)) return false;
				if( false == IsSameMinMax( gravity			, rhs_.gravity)) return false;				
				if( false == IsSameMinMax( blackHolePosition, rhs_.blackHolePosition)) return false;
				if( false == IsSameMinMax( landPosition		, rhs_.landPosition)) return false;
				
				if( false == IsSameMinMax( lightPos			, rhs_.lightPos)) return false;
				if( false == IsSameMinMax( outLineWide		, rhs_.outLineWide)) return false;
				if( false == IsSameMinMax( lightFlowWide	, rhs_.lightFlowWide)) return false;
				
				return true;
			}

// #endif // EFFECT_TOOL
#endif // defined(EFFECT_TOOL) || defined(EXPAND_DEVELOPER_SCRIPT)

			XMeshTemplet& operator=( const XMeshTemplet& templet )
			{
				//메인 정보
				templetName		= templet.templetName;
				layer			= templet.layer;
				bForceLayer		= templet.bForceLayer;

				//라이프 정보
				lifeType		= templet.lifeType;
				dieType			= templet.dieType;
				bUseDieEvent	= templet.bUseDieEvent;

				playCount		= templet.playCount;
				lifeTime		= templet.lifeTime;
				diePlayCount	= templet.diePlayCount;	
				dieLifeTime		= templet.dieLifeTime;	

				//메시 정보
				meshType			= templet.meshType;
				meshName			= templet.meshName;
				changeTexXETName	= templet.changeTexXETName;
				multiTexXETName		= templet.multiTexXETName;
				aniXETName			= templet.aniXETName;

				//메시랜더 정보
				renderType			= templet.renderType;
				cartoonTexType		= templet.cartoonTexType;

				lightPos			= templet.lightPos;
				outLineWide			= templet.outLineWide;
				lightFlowWide		= templet.lightFlowWide;

				bZEnable			= templet.bZEnable;
				bZWriteEnable		= templet.bZWriteEnable;
				cullMode			= templet.cullMode;
				srcBlend			= templet.srcBlend;
				destBlend			= templet.destBlend;

				drawCount			= templet.drawCount;
				billBoardType		= templet.billBoardType;

				//움직임 정보
				gravity				= templet.gravity;				
				blackHolePosition	= templet.blackHolePosition;
				landPosition		= templet.landPosition;
				bSetLandHeightOnStart = templet.bSetLandHeightOnStart;
				bUseLand			= templet.bUseLand;
				bCrashLand			= templet.bCrashLand;
				elasticCoeff		= templet.elasticCoeff;
				elasticCoeffX		= templet.elasticCoeffX;

				bUseSlashTrace		= templet.bUseSlashTrace;
				iSlashTraceType		= templet.iSlashTraceType;
				slashTraceTextureName	= templet.slashTraceTextureName;
				slashTraceCurveType = templet.slashTraceCurveType;

				fNotCheckLandTime	= templet.fNotCheckLandTime;
#ifdef PARTICLE_NOTAPPLY_UNITSCALE
				bApplyUnitScale		= templet.bApplyUnitScale;
#endif //PARTICLE_NOTAPPLY_UNITSCALE
#ifdef FIX_TEMP_MESH_BILLBOARD //2013.10.09
				bUseMeshBillBoard	= templet.bUseMeshBillBoard;
#endif //FIX_TEMP_MESH_BILLBOARD

#ifdef ADD_ALPHATESTENABLE
				bAlphaTestEnable	= templet.bAlphaTestEnable;
#endif //ADD_ALPHATESTENABLE

				return *this;
			}
		};

		enum INSTANCE_STATE
		{
			IS_PLAY,
#ifndef X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION_VELOCITY_ACCUM_BUG_FIX
			IS_DYING,
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION_VELOCITY_ACCUM_BUG_FIX
			IS_DIE,
//#ifdef RAVEN_WEAPON_TAKER
			IS_DYING_BY_CRASH,
//#endif RAVEN_WEAPON_TAKER
		};

		class CXMeshInstance : public CKTDGObject
		{
			friend class CXMeshEvent_AniName;
			friend class CXMeshEvent_AniSpeed;
			friend class CXMeshEvent_SlashTrace;
			friend class CXMeshEvent_Color;
			friend class CXMeshEvent_OutLineColor;
#ifdef EVENT_SCENE
			friend class CXMeshEvent_Position;
#endif EVENT_SCENE
			friend class CXMeshEvent_Size;
			friend class CXMeshEvent_Velocity;
			friend class CXMeshEvent_Tex0UV;
			friend class CXMeshEvent_Tex1UV;
			friend class CXMeshEvent_Tex2UV;
			friend class CXMeshEvent_Rotate;
			friend class CXMeshEvent_DirSpeed;
			friend class CXMeshEvent_BlackHoleTime;
			friend class CXMeshEvent_Crash;
			friend class CXMeshEvent_ResetCrash;
			friend class CXMeshEvent_LightFlowImpact;
			friend class CXMeshEvent_LightFlowPoint;
			friend class CXMeshEvent_Sound;
			friend class CXMeshEvent_RenderType;

			private:
				//CXMeshInstance() { ASSERT( !"invalid" ); }
				CXMeshInstance( CKTDGXMeshPlayer* pMeshPlayer, XMeshTemplet* pTemplet, D3DXVECTOR3 pos, D3DXVECTOR3 angleDegree, D3DXVECTOR3 moveAxisAngleDegree );
				~CXMeshInstance();


			public:
				static CXMeshInstance* CreateMeshInstance( CKTDGXMeshPlayer* pMeshPlayer, XMeshTemplet* pTemplet, 
					const D3DXVECTOR3& pos, const D3DXVECTOR3& angleDegree, const D3DXVECTOR3& moveAxisAngleDegree )
				{
					return new CXMeshInstance( pMeshPlayer, pTemplet, pos, angleDegree, moveAxisAngleDegree );
				}

#ifdef BOOST_SINGLETON_POOL_TEST
				void* operator new(size_t s);
				void operator delete(void *p);
#endif BOOST_SINGLETON_POOL_TEST


				virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );

//{{ robobeg : 2008-10-24
		        //virtual HRESULT OnFrameRender();
                virtual RENDER_HINT   OnFrameRender_Prepare();
                virtual void              OnFrameRender_Draw();
//}} robobeg : 2008-10-24

				void SetDelayTime( float fDelayTime ){ m_fDelayTime = fDelayTime; }
				float GetDelayTime(){ return m_fDelayTime; }

				void PlayProcess();
#ifndef X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION_VELOCITY_ACCUM_BUG_FIX
				void DyingProcess();
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION_VELOCITY_ACCUM_BUG_FIX

				void PlayEnd();

				void SetUseDXMatrix( const D3DXMATRIX& dxMATRIX )
				{
					m_bUseDXMatrix	= true;					
					m_DXMATRIX		= dxMATRIX;

					// dmlee. 2009-02-17, 이 부분은 없는게 맞는듯
					//if( true == m_bSlashTrace )
					//{
					//	if( NULL != m_pSlashTrace )
					//	{
					//		m_pSlashTrace->SetCenter( D3DXVECTOR3(m_DXMATRIX._41, m_DXMATRIX._42, m_DXMATRIX._43) );
					//	}

					//	if( NULL != m_pSlashTraceTip )
					//	{
					//		m_pSlashTraceTip->SetCenter( D3DXVECTOR3(m_DXMATRIX._41, m_DXMATRIX._42, m_DXMATRIX._43) );
					//	}
					//}
				}

				XMeshTemplet* GetTempletData(){ return &m_TempletData; }
				INSTANCE_STATE GetState(){ return m_State; }






				void RunEvent();

				CKTDGXSkinAnim*			GetXSkinAnim(){ return m_pXSkinAnim; }
				CKTDXDeviceXMesh*		GetXMesh(){ return m_pXMesh; }
				CKTDXDeviceXSkinMesh*	GetXSkinMesh(){ return m_pXSkinMesh; }

				void ChangeXSkinMesh( const WCHAR* wszMeshName, const WCHAR* wszChangeTexXETName, 
					const WCHAR* wszMultiTexXETName, const WCHAR* wszAniXETName );

				//void SetShow( bool bShow ){ m_bShow = bShow; }

				const D3DXVECTOR3& GetPos() const { return m_vPos; }
				void SetPos( const D3DXVECTOR3& pos ){ m_vPos = pos; }
				D3DXVECTOR3 GetRotateDegree(){ return m_vRotate; }
				void SetRotateDegree( D3DXVECTOR3 degree ){ m_vRotate = degree; }
				D3DXVECTOR3 GetMoveAxisAngleDegree() const { return m_MoveAxisAngleDegree; }
				void SetMoveAxisAngleDegree( D3DXVECTOR3 degree ){ m_MoveAxisAngleDegree = degree; }
//#ifdef EVE_ELECTRA
#ifdef LOCAL_ROTATE_EFFECT
				D3DXVECTOR3 GetRotateLocalDegree(){ return m_vRotateLocal; }
				void SetRotateLocalDegree( D3DXVECTOR3 degree ){ m_vRotateLocal = degree; }
#endif
				float GetMeshAlphaColor(){ return m_Color.a; }
				void SetMeshAlphaColor( float _fVal){ m_Color.a = _fVal; }
				D3DXVECTOR3 GetScale(){ return m_vScale; }
//#endif EVE_ELECTRA
#ifdef BALANCE_BLADE_MASTER_20130117
				D3DXVECTOR3 GetSize(){ return m_vSize; }
#endif BALANCE_BLADE_MASTER_20130117
				void SetGlobalTime( float fTime )
                    {   m_fGlobalTime = fTime; 
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION_VELOCITY_ACCUM_BUG_FIX
                        m_fGlobalTimeVelocity = fTime;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION_VELOCITY_ACCUM_BUG_FIX
                    }
				float GetGlobalTime() { return m_fGlobalTime; }

				bool GetIsSetLandHeightOnStart() { return m_TempletData.bSetLandHeightOnStart; }
				bool GetIsCrashLand() { return m_TempletData.bCrashLand; }
				bool GetIsUsedLand() { return m_TempletData.bUseLand; }
				void SetLandPosition( float fLandPos ){ m_fLandPosition = fLandPos; }
				//{{ 허상형 : [2011/12/25] //	추가
				float GetLandPosition() { return m_fLandPosition; }
				//}} 허상형 : [2011/12/25] //	추가

				void SetAge( float fAge ) { m_NowLifeTime = fAge; }
				void SetBlackHolePos( D3DXVECTOR3 pos ){ m_BlackHolePosition = pos; }

				void UseManualDirDegree( bool bManual ){ m_bManualDir = bManual; } 
				void SetManualDirDegree( D3DXVECTOR3 dir ){ m_ManualDirDegree = dir; }
				D3DXVECTOR3 GetManualDirDegree(){ return m_ManualDirDegree; }
				D3DXVECTOR3 GetDirVec();

                void SetManualSize( float sx, float sy, float sz) { m_vManualSize = D3DXVECTOR3(sx, sy, sz); } // added by wonpok
#ifdef NEW_SKILL_TREE
				void SetScale( float fX, float fY, float fZ ){ m_vScale = D3DXVECTOR3(fX,fY,fZ); }
				void SetScale( D3DXVECTOR3 vScale ){ m_vScale = vScale; }
#endif NEW_SKILL_TREE
				
				//{{ JHKang / 강정훈 / 2010/12/21 / 함수 형태 문제로 Lua에서 사용할 SetScale_LUA 생성
#ifdef SEASON3_MONSTER_2010_12
				void SetScale_LUA( float fX, float fY, float fZ ){ SetScale( fX, fY, fZ ); }
#endif SEASON3_MONSTER_2010_12
				//}} JHKang / 강정훈 / 2010/12/21 / 함수 형태 문제로 Lua에서 사용할 SetScale_LUA 생성

//#ifdef GIANT_UNIT_GIANT_EFFECT_TEST
//				const D3DXVECTOR3& GetScaleByUnit() const 
//				{ 
//					ASSERT( 0.f != m_vScaleByUnit.x ); 
//					ASSERT( 0.f != m_vScaleByUnit.y ); 
//					ASSERT( 0.f != m_vScaleByUnit.z ); 
//					
//					return m_vScaleByUnit; 
//				}
//				void SetScaleByUnit(const D3DXVECTOR3& val) 
//				{ 
//					ASSERT( 0.f != val.x ); 
//					ASSERT( 0.f != val.y ); 
//					ASSERT( 0.f != val.z ); 
//
//					m_vScaleByUnit = val; 
//				}
//#endif GIANT_UNIT_GIANT_EFFECT_TEST
				
				void SetGravity(const D3DXVECTOR3& val) { m_Gravity = val; }

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
                // 주의!!! 아래 함수가 제대로 동작하려면 해당 mesh instance 는 SetPerFrameSimulation( true )  설정되어 있어야 함!!!
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
				bool EventTimerGlobal( float fTime );


				CKTDXDeviceXET* GetChangeTexXET() const { return m_pChangeTexXET; }
				void SetChangeTexXET(CKTDXDeviceXET* val) { m_pChangeTexXET = val; }
				void SetVelocity( const D3DXVECTOR3 &vSpeed) { m_vVelocity = vSpeed; }
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
                void    ResetVelocityToAccumPos() { INIT_VECTOR3( m_vVelocityToAccumPos, 0, 0, 0 ); }
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION_VELOCITY_ACCUM_BUG_FIX
                float   GetGlobalTimeVelocity() const   { return m_fGlobalTimeVelocity; }
                float   GetAniTimeVelocity() const
                        {
                            if ( m_fGlobalTimeBefore <= m_fGlobalTime
                                && m_fAniTimeBefore <= m_fAniTime )
                            {
                                float   fRatio = ( m_fGlobalTimeVelocity - m_fGlobalTimeBefore ) / ( m_fGlobalTime - m_fGlobalTimeBefore );
                                fRatio = __max( 0.f, __min( 1.f , fRatio ) );
                                return  m_fAniTimeBefore * ( 1.f - fRatio ) + m_fAniTime * fRatio;
                            }
                            return  m_fAniTime;
                        }
                void    UpdateVelocityAccumPosAndEventTimer( float fTime, bool bAniTime )
                        {
                            if ( bAniTime == true )
                            {
                                if ( m_fAniTimeBefore <= fTime && fTime <= m_fAniTime
                                    && m_fGlobalTimeBefore <= m_fGlobalTime )
                                {
                                    float   fRatio = ( fTime - m_fAniTimeBefore ) / ( m_fAniTime - m_fAniTimeBefore );
                                    fRatio = __max( 0.f, __min( 1.f , fRatio ) );
                                    float   fGlobalTime = m_fGlobalTimeBefore * ( 1.f - fRatio ) + fRatio * m_fGlobalTime;
                                    if ( m_fGlobalTimeVelocity < fGlobalTime )  
                                        m_vVelocityToAccumPos += m_vVelocity * ( fGlobalTime - m_fGlobalTimeVelocity );
                                    m_fGlobalTimeVelocity = fGlobalTime;
                                }
                            }
                            else
                            {
                                if ( m_fGlobalTimeVelocity < fTime )    m_vVelocityToAccumPos += m_vVelocity * ( fTime - m_fGlobalTimeVelocity );
                                m_fGlobalTimeVelocity = fTime;
                            }//if.. else..
                        }
#else   X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION_VELOCITY_ACCUM_BUG_FIX
                void    AddVelocityToAccumPos( float fElapsedTime, bool bAniTime )
                {
                    if ( bAniTime == false )
                        m_vVelocityToAccumPos += m_vVelocity * fElapsedTime;
                    else
                        m_vVelocityToAccumPos += m_vVelocity * ( fElapsedTime / m_fAniPlaySpeed );
                }
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION_VELOCITY_ACCUM_BUG_FIX

#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
				// kimhc // 2010.02.10 // Get 핸들러 추가
				D3DXVECTOR3 GetVelocity() const { return m_vVelocity; }
				void SetDirSpeed(float val) { m_fDirSpeed = val; }
//#ifdef RAVEN_WEAPON_TAKER
				float GetDirSpeed() { return m_fDirSpeed; }
//#endif RAVEN_WEAPON_TAKER
				void SetNowLifeTime(float val) { m_NowLifeTime = val; }
				void SetMaxLifeTime(float val) { m_MaxLifeTime = val; }
				float GetMaxLifeTime() const { return m_MaxLifeTime; }


				CKTDGXMeshPlayer::CXMeshInstanceHandle GetHandle() const { return m_Handle; }
				void SetHandle(CKTDGXMeshPlayer::CXMeshInstanceHandle val) { m_Handle = val; }

				//090212.hoons.메쉬플레이어를 가지고 있다가 특정시간에 특정본의 위치를 뽑아내기 위해
				D3DXVECTOR3 GetBonePos( const WCHAR* pBoneName ){ return m_pXSkinAnim->GetCloneFramePosition( pBoneName ); }
				bool GetBonePos( const WCHAR* pBoneName_, OUT D3DXVECTOR3& vPos_ ){ return m_pXSkinAnim->GetCloneFramePosition( pBoneName_, vPos_ ); }
				// hoons.
#ifdef EVE_ELECTRA
				CKTDXDeviceXSkinMesh::MultiAnimFrame* GetCloneFrame( const WCHAR* name ){ return m_pXSkinAnim->GetCloneFrame( name ); }
#endif EVE_ELECTRA

				//{{ kimhc // 2009-09-23 // NOT_CHECK_LAND_TIME을 가져오거나 설정하는 함수
				float		GetNotCheckLandTime() const { return m_TempletData.fNotCheckLandTime; }
				void		SetNotCheckLandTime( float fTime );
				//}} kimhc // 2009-09-23 // NOT_CHECK_LAND_TIME을 가져오거나 설정하는 함수

				void		SetAnimSpeed(float val) { m_fAniPlaySpeed = val; }
				void		SetXAnimSpeed(float val) 
				{
					if( m_pXSkinAnim != NULL )
						m_pXSkinAnim->SetPlaySpeed( val );
					m_fAniPlaySpeed = val;
				}
				
				bool GetParabolicCurve() const { return m_bParabolicCurve; }
				void SetParabolicCurve(const bool val) { m_bParabolicCurve = val; }

				bool GetToggleRotae() { return m_bToggleRotate; }
				void SetToggleRotate(bool bVal) { m_bToggleRotate = bVal; }

#ifdef STOP_UNIT_STOP_EFFECT_TEST

				void SetStopTime( float fTime ) { m_StopTime.m_fTimeLeft = fTime; }
				float GetStopTime() { return m_StopTime.m_fTimeLeft; }

#endif STOP_UNIT_STOP_EFFECT_TEST

#ifdef NEW_HENIR_TEST
				void ChangeAnim_LUA( char* cAnimName, CKTDGXSkinAnim::XSKIN_ANIM_PLAYTYPE ePlayType, float fAnimSpeed );
				void ChangeAnim( const WCHAR* wcAnimName, CKTDGXSkinAnim::XSKIN_ANIM_PLAYTYPE ePlayType, float fAnimSpeed );
#endif NEW_HENIR_TEST

#ifdef UPGRADE_RAVEN
				void SetOutlineColor(D3DXCOLOR color) { m_OutLineColor = color; }
#endif
#ifdef EVE_ELECTRA
				float GetNowLifeTime() const { return m_NowLifeTime; }
#endif EVE_ELECTRA
#ifdef FIX_CLASS_CHANGE_MESH
				void SetOutlineWide( IN float fWide_ ) { m_fOutLineWide = fWide_; }
#endif

#ifdef PARTICLE_NOTAPPLY_UNITSCALE
				bool GetApplyUnitScale() { return m_TempletData.bApplyUnitScale; }
#endif

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
                void    SetPerFrameSimulation( bool bPerFrame ) { m_bPerFrameSimulation = bPerFrame; }
                bool    GetPerFrameSimulation() const           { return m_bPerFrameSimulation; }
#else   X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
                void    SetPerFrameSimulation( bool ) {}
                bool    GetPerFrameSimulation() const           { return true; }
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION





			private:
				void OpenResource();


			private:
				CXMeshInstanceHandle	    m_Handle;
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
                bool                        m_bPerFrameSimulation;
                float                       m_fAccumElapsedTime;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
				float						m_fDelayTime;
				//객체 데이터
				INSTANCE_STATE				m_State;
				//bool						m_bShow;
				
				CKTDGXMeshPlayer*			m_pMeshPlayer;
				XMeshTemplet*				m_pOrgTemplet;
				XMeshTemplet				m_TempletData;

				bool						m_bUseDXMatrix;
				D3DXMATRIX					m_DXMATRIX;

				CKTDXDeviceXMesh*			m_pXMesh;
				CKTDXDeviceXSkinMesh*		m_pXSkinMesh;
				//{{ seojt // 2009-1-12, 14:19
                // 내부적으로 관리하는 객체이므로 smart pointer를 사용하지 않는다.
				CKTDGXSkinAnim*				m_pXSkinAnim;
                //CKTDGXSkinAnimWeakPtr       m_pXSkinAnim;
				//}} seojt // 2009-1-12, 14:19
				CKTDXDeviceXET*				m_pChangeTexXET;
				CKTDXDeviceXET*				m_pMultiTexXET;
				CKTDXDeviceXET*				m_pAniXET;
				const CKTDXDeviceXET::AniData*	m_pAniData;
				float						m_fAniPlaySpeed;

				float						m_fElapsedTime;
				float						m_fGlobalTimeBefore;
				float						m_fGlobalTime;
#ifndef X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION_VELOCITY_ACCUM_BUG_FIX
				float						m_fDieTimeBefore;
				float						m_fDieTime;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION_VELOCITY_ACCUM_BUG_FIX
				float						m_fAniTimeBefore;
				float						m_fAniTime;
				float						m_fFinalTime;
#ifndef X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION_VELOCITY_ACCUM_BUG_FIX
				float						m_fDieFinalTime;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION_VELOCITY_ACCUM_BUG_FIX
				
				D3DXVECTOR3					m_vPos;
				D3DXVECTOR3					m_vPosBefore;
				D3DXVECTOR3					m_vAngleDegree;
				D3DXVECTOR3					m_MoveAxisAngleDegree;

				map<wstring,CKTDXDeviceSound*>		m_SoundMap;
				map<wstring,CKTDXDeviceSound*>		m_ParticleMap;

				//시퀀스 데이터
				int							m_NowPlayCount;
				int							m_MaxPlayCount;
				float						m_NowLifeTime;
				float						m_MaxLifeTime;
				int							m_NowDiePlayCount;
				int							m_MaxDiePlayCount;
				float						m_NowDieLifeTime;
				float						m_MaxDieLifeTime;

				D3DXVECTOR3					m_LightPos;
				float						m_fOutLineWide;
				float						m_fLightFlowWide;

				int							m_DrawCount;
				D3DXVECTOR3					m_Gravity;								
				D3DXVECTOR3					m_BlackHolePosition;
				float						m_fLandPosition;

				bool						m_bManualDir;
				D3DXVECTOR3					m_ManualDirDegree;

				

				//이벤트 데이터
				D3DXCOLOR					m_Color;
				D3DXCOLOR					m_ColorFinal;

				D3DXCOLOR					m_OutLineColor;
				D3DXCOLOR					m_OutLineColorFinal;

				D3DXVECTOR3					m_vSize;
				D3DXVECTOR3					m_vSizeFinal;
                D3DXVECTOR3                 m_vManualSize; // added by wonpok
				D3DXVECTOR3					m_vScale;
				
//#ifdef GIANT_UNIT_GIANT_EFFECT_TEST
//				D3DXVECTOR3					m_vScaleByUnit;	// 유닛 크기가 커졌을 때 이펙트 크기 조절
//#endif GIANT_UNIT_GIANT_EFFECT_TEST

#ifdef EVENT_SCENE
				D3DXVECTOR3					m_vPositionGap;
				D3DXVECTOR3					m_vPositionFinal;
#endif EVENT_SCENE
				D3DXVECTOR3					m_vVelocity;
				D3DXVECTOR3					m_vVelocityFinal;
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
                D3DXVECTOR3                 m_vVelocityToAccumPos;
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION_VELOCITY_ACCUM_BUG_FIX
                float                       m_fGlobalTimeVelocity;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION_VELOCITY_ACCUM_BUG_FIX
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION

				D3DXVECTOR2					m_vTexStage0UV;
				D3DXVECTOR2					m_vTexStage0UVFinal;

				D3DXVECTOR2					m_vTexStage1UV;
				D3DXVECTOR2					m_vTexStage1UVFinal;

				D3DXVECTOR2					m_vTexStage2UV;
				D3DXVECTOR2					m_vTexStage2UVFinal;

#ifdef LOCAL_ROTATE_EFFECT
				D3DXVECTOR3					m_vRotateLocal;
#endif
				D3DXVECTOR3					m_vRotate;
				D3DXVECTOR3					m_vRotateFinal;

				float						m_fDirSpeed;		//로테이션 방향으로 전진속도				
				float						m_fDirSpeedFinal;

				float						m_fLightFlowImpact;
				float						m_fLightFlowImpactFinal;

				D3DXVECTOR3					m_LightFlowPoint;
				D3DXVECTOR3					m_LightFlowPointFinal;

				D3DXVECTOR3					m_vBlackHoleSpeed;
				float						m_BlackHoleTime;

				float						m_fResetCrash;
				float						m_fNowResetCrash;
				float						m_fResetCrashFinal;
				D3DXVECTOR3					m_Crash;


				bool						m_bSlashTrace;
				CKTDGSlashTrace*			m_pSlashTrace;
				CKTDGSlashTrace*			m_pSlashTraceTip;

				CKTDXDeviceXSkinMesh::MultiAnimFrame*			m_pFrame_TRACE_START0;
				CKTDXDeviceXSkinMesh::MultiAnimFrame*			m_pFrame_TRACE_END0;	


				bool		m_bParabolicCurve;		// 포물선 그리는 meshplayer의 경우에 m_vRotate와 m_vMoveAxisAngle을 동일하게 맞추기 위해

				bool		m_bToggleRotate;

#ifdef STOP_UNIT_STOP_EFFECT_TEST
				TimeLimited m_StopTime;
#endif STOP_UNIT_STOP_EFFECT_TEST

#ifdef PARTICLE_NOTAPPLY_UNITSCALE
				bool						m_bApplyUnitScale;
#endif
#ifdef SHOW_ATTACK_BOX_DUMMY
				CKTDXDeviceXMesh*								m_pXMeshSphere;
				CKTDGMatrix*									m_pMatrixSphere;
				bool											m_bShowAttackBox;
#endif // SHOW_ATTACK_BOX_DUMMY

//#ifdef X2OPTIMIZE_CULLING_PARTICLE
//			private:
//				void _DecideWorldMatrix_SkinMesh( CKTDGMatrixSet& kWorldMatrix, CKTDGCamera& kCamera, D3DXVECTOR3& vCenter, float fRadius );
//				void _DecideWorldMatrix_Mesh( D3DXMATRIX& kWorldMatrix, CKTDGCamera& kCamera, D3DXVECTOR3& vCenter, float fRadius );
//#endif//X2OPTIMIZE_CULLING_PARTICLE
		};

		struct BLENDINGMODE
		{
			char	name[25];
			int		mode;
		};

		class CXMeshPlayerTokenizer  
		{
			public:
				void Tokenize( const char *str, const int iLength = -1 );

				static bool ProcessLifeType(LIFE_TYPE &lifeType, 
					std::vector<CXMeshPlayerToken>::iterator &TokenIter,
					std::vector<CXMeshPlayerToken>::iterator &EndIter);

				static bool ProcessMeshType(MESH_TYPE &meshType, 
					std::vector<CXMeshPlayerToken>::iterator &TokenIter,
					std::vector<CXMeshPlayerToken>::iterator &EndIter);

				static bool ProcessRenderType(CKTDGXRenderer::RENDER_TYPE &renderType, 
					std::vector<CXMeshPlayerToken>::iterator &TokenIter,
					std::vector<CXMeshPlayerToken>::iterator &EndIter);

				static bool ProcessCartoonTexType(CKTDGXRenderer::CARTOON_TEX_TYPE &cartoonTexType, 
					std::vector<CXMeshPlayerToken>::iterator &TokenIter,
					std::vector<CXMeshPlayerToken>::iterator &EndIter);

				static bool ProcessCullMode(D3DCULL &cullMode, 
					std::vector<CXMeshPlayerToken>::iterator &TokenIter,
					std::vector<CXMeshPlayerToken>::iterator &EndIter);

				static bool ProcessAlphaBlendMode(int &alphablendmode, 
					std::vector<CXMeshPlayerToken>::iterator &TokenIter,
					std::vector<CXMeshPlayerToken>::iterator &EndIter);

				static bool ProcessBillboardType(CKTDGMatrix::BILLBOARD_TYPE &billboardType, 
					std::vector<CXMeshPlayerToken>::iterator &TokenIter,
					std::vector<CXMeshPlayerToken>::iterator &EndIter);

				static bool ProcessBool(bool &bValue, 
					std::vector<CXMeshPlayerToken>::iterator &TokenIter,
					std::vector<CXMeshPlayerToken>::iterator &EndIter);

				static bool ProcessString(wstring &str, 
					std::vector<CXMeshPlayerToken>::iterator &TokenIter,
					std::vector<CXMeshPlayerToken>::iterator &EndIter);

				static bool ProcessColor(CMinMax<D3DXCOLOR> &c, 
					std::vector<CXMeshPlayerToken>::iterator &TokenIter,
					std::vector<CXMeshPlayerToken>::iterator &EndIter);

				static bool ProcessTime(CMinMax<float> &TimeRange, 
					bool &IsFade, bool &IsFinal, const float InitialTime,
					std::vector<CXMeshPlayerToken>::iterator &TokenIter, 
					std::vector<CXMeshPlayerToken>::iterator &EndIter);

				static bool ProcessVector3(CMinMax<D3DXVECTOR3> &v, 
					std::vector<CXMeshPlayerToken>::iterator &TokenIter, 
					std::vector<CXMeshPlayerToken>::iterator &EndIter);

				static bool ProcessVector2(CMinMax<D3DXVECTOR2> &v, 
					std::vector<CXMeshPlayerToken>::iterator &TokenIter, 
					std::vector<CXMeshPlayerToken>::iterator &EndIter);

				static bool ProcessNumber(CMinMax<float> &number, 
					std::vector<CXMeshPlayerToken>::iterator &TokenIter,
					std::vector<CXMeshPlayerToken>::iterator &EndIter);

				static void NextToken(std::vector<CXMeshPlayerToken>::iterator &TokenIter, 
					std::vector<CXMeshPlayerToken>::iterator &EndIter) 
				{
					if (TokenIter == EndIter) throw("Unexpected end-of-file.");
					TokenIter++;
				}

				static const int NUMBLENDINGMODES;
				static const BLENDINGMODE m_BlendingModes[15];

				std::vector<CXMeshPlayerToken> m_TokenVector;

			private:
				void AddToken(CXMeshPlayerToken token);
				void DetermineTokenType(CXMeshPlayerToken &token);
		};

	public:
		CKTDGXMeshPlayer(
#ifdef  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
            unsigned char ucSystemID = 0
#endif  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
            );
		~CKTDGXMeshPlayer(void);

		void OpenScriptFile( const WCHAR* pFilename );
		bool Compile( const char *pScript, const int iLength = -1 );
        bool DoMemoryNotEncrypt( const char* pBuffer, long nSize ) { return Compile( pBuffer, (int) nSize ); }

		HRESULT OnFrameMove( double fTime, float fElapsedTime );

		CXMeshInstance* CreateInstance_LUA( char* pTempletName, 
											D3DXVECTOR3 pos, 
											D3DXVECTOR3 angleDegree,
											D3DXVECTOR3 moveAxisDegree,
											int layer = -1 )
		{
			wstring wstrTempletName;
			ConvertUtf8ToWCHAR( wstrTempletName, pTempletName );

#pragma DMLEE_NOTE( "일단은 스크립트에서 사용하는 부분에서는 NULL 로 입력, 나중에 몬스터 스크립트 전체 수정하면서 코드 수정해야함" )
			return  CreateInstance( NULL, wstrTempletName.c_str(), pos, angleDegree, moveAxisDegree, layer );
		}


#ifdef EVENT_SCENE
		//{{ oasis907 : 김상윤 [2010.6.10] // 
		// txt에서 파싱된 Templet들이 등록된 TempletMap과 상관없이 동작할 수 있게금 하였습니다.
		// 대신 pTemplet의 내용은 다른 곳에서 수동으로 입력해야합니다. 
		CXMeshInstance* CreateInstanceNonTemplet( CKTDGXMeshPlayer* pMeshPlayer, XMeshTemplet* pTemplet, 
			const D3DXVECTOR3& pos, const D3DXVECTOR3& angleDegree, const D3DXVECTOR3& moveAxisAngleDegree );
		//}}
#endif EVENT_SCENE

		CXMeshInstance* CreateInstanceParabolic( CKTDGObject* pKTDGObject, const WCHAR* pTempletName, const D3DXVECTOR3& vPos, const D3DXVECTOR3& vTargetPos, 
			const D3DXVECTOR3& vAcceleration, const float fTimeToReachTarget, const float fTimeToLive );


		CXMeshInstance* CreateInstance( CKTDGObject* pKTDGObject, 
										const WCHAR* pTempletName, 
										D3DXVECTOR3 pos, 
										D3DXVECTOR3 angleDegree,
										D3DXVECTOR3 moveAxisDegree,
										int layer = -1 )
		{
			return CreateInstance( pKTDGObject, pTempletName, pos.x, pos.y, pos.z, 
									angleDegree.x, angleDegree.y, angleDegree.z,
									moveAxisDegree.x, moveAxisDegree.y, moveAxisDegree.z, layer );
		}

		CXMeshInstance* CreateInstance( CKTDGObject* pKTDGObject, 
										const WCHAR* pTempletName, 
										float x, float y, float z, 
										float angleXDegree, float angleYDegree, float angleZDegree,
										float moveAxisXDegree, float moveAxisYDegree, float moveAxisZDegree,
										int layer = -1 );

		void DestroyAllInstances();
		void DestroyInstanceHandle( CXMeshInstanceHandle& handle );	

		void DestroyInstance_LUA( CXMeshInstanceHandle handle )
		{
			DestroyInstanceHandle( handle );
		}

		bool IsLiveInstanceHandle( const CXMeshInstanceHandle handle );
		bool IsLiveInstance( const CXMeshInstance* pMeshInst )		// 기존에 몬스터 스크립트에서 사용하던 코드와의 호환성을 유지하기 위해 남겨둠, 앞으로 제작하는 몬스터는 이 함수 대신에 IsLiveInstanceHandle() 함수를 사용하기.
		{
			if( NULL == pMeshInst )
				return false;

			return IsLiveInstanceHandle( pMeshInst->GetHandle() );
		}
		CXMeshInstance* GetMeshInstance( CXMeshInstanceHandle handle, bool bLiveOnly = true );
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        CXMeshInstance* ValidateInstanceHandle( CXMeshInstanceHandle& handle, bool bLiveOnly = true )
        {
            CXMeshInstance* pInstance = GetMeshInstance( handle, bLiveOnly );
            if ( pInstance == NULL )
                handle = INVALID_MESH_INSTANCE_HANDLE;
            return pInstance;
        }
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

		static CXMeshEvent* EventFactory( std::string EventName, bool bFinal );
		void SetEnable( bool bEnable ){ m_bEnable = bEnable; }

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        UINT EstimateInstanceNum();

        template< typename FUNCTION >
        void                            ApplyFunctionToLiveInstances( FUNCTION fn );

#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		CXMeshInstance* GetInstance( int index ) { return m_InstanceList[index]; }
		UINT GetInstanceNum() { return m_InstanceList.size(); } 
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

		map<wstring, XMeshTemplet*>& GetTempletMap() { return m_TempletMap; }
		const XMeshTemplet* GetTempletByName( const wstring wstrName );

#ifdef EFFECT_TOOL
		void EraseXMeshTemplet( UINT uiMeshIndex_ );
		vector<XMeshTemplet*> GetVecTemplet() { return m_TempletVec; }
		CKTDGXMeshPlayer::XMeshTemplet* CreateXMeshTemplet( const WCHAR* strMeshName_ );
#endif //EFFECT_TOOL

		bool GetEnableSlashTrace() const { return m_bEnableSlashTrace; }
		void SetEnableSlashTrace(bool val) { m_bEnableSlashTrace = val; }
#ifdef RAVEN_WEAPON_TAKER
		bool IsDyingByCrash( CXMeshInstanceHandle hMeshInstanceHandle );
		void PlayEnd( CXMeshInstanceHandle hMeshInstanceHandle );
#endif RAVEN_WEAPON_TAKER

//#ifdef	X2OPTIMIZE_REFACTORING_RESOURCE_BACKGROUND_LOAD
		bool	AppendToDeviceList( CKTDXDeviceDataList& listInOut_, const WCHAR* pTempletName, CKTDXDeviceManager::EPriority ePriority = CKTDXDeviceManager::PRIORITY_HIGH );
		bool	AppendToDeviceList( CKTDXDeviceDataList& listInOut_, const std::wstring& wstrTempletName, CKTDXDeviceManager::EPriority ePriority = CKTDXDeviceManager::PRIORITY_HIGH );
//#endif	X2OPTIMIZE_REFACTORING_RESOURCE_BACKGROUND_LOAD

#ifdef NOT_RENDER_EFFECT_MADE_BY_GAME_UNIT
		bool GetRenderEffectMadeByGameUnit() const { return m_bRenderEffectMadeByGameUnit; }
		void SetRenderEffectMadeByGameUnit(bool bVal_) { m_bRenderEffectMadeByGameUnit = bVal_; }
#endif //NOT_RENDER_EFFECT_MADE_BY_GAME_UNIT

#ifdef SHOW_ATTACK_BOX_DUMMY
		bool GetShowAttackBox() const { return m_bShowAttackBox; }
		void SetShowAttackBox(bool val) { m_bShowAttackBox = val; }
		void ToggleShowAttackBox() { SetShowAttackBox( !m_bShowAttackBox ); }
#endif // SHOW_ATTACK_BOX_DUMMY

#ifdef EXPAND_DEVELOPER_SCRIPT	  // 김종훈, 개발자 스크립트 확장 기능 추가
		typedef map< wstring, XMeshTemplet* > XMeshTempletMap;
		bool MergeXMeshTemplet( CKTDGXMeshPlayer::XMeshTemplet* pNewXMeshTemplet_, CKTDGXMeshPlayer::XMeshTemplet* pOrgXMeshTemplet_ );
		bool CreateXMeshTemplet( CKTDGXMeshPlayer::XMeshTemplet* pXMesshTemplet_ );
		bool EraseXMeshTemplet ( CKTDGXMeshPlayer::XMeshTemplet* pXMesshTemplet_ );
		bool CopyXMeshTempletTimeEvent( CKTDGXMeshPlayer::XMeshTemplet* pXMeshTemplet_ );
#endif // EXPAND_DEVELOPER_SCRIPT  // 김종훈, 개발자 스크립트 확장 기능 추가

        DWORD   ComposeHandle( WORD wIndex, OUT WORD& wStamp )
        {
#ifdef  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
            wStamp &= 0x3fff;
            return wIndex | ( wStamp << 16L ) | ( m_ucSystemID << 30L );
#else   X2OPTIMIZE_HANDLE_VALIDITY_CHECK
            return wIndex | ( wStamp << 16L );
#endif  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
        }
        bool    DecomposeHandle( DWORD dwHandle, OUT WORD& wIndex, OUT WORD& wStamp )
        {
            wIndex = (WORD) dwHandle;
            wStamp = (WORD) ( ( dwHandle & 0xffff0000 ) >> 16L );
#ifdef  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
            unsigned char ucSystemID = (unsigned char) ( wStamp >> 14L );
            wStamp &= 0x3fff;
            return ucSystemID == m_ucSystemID;
#else   X2OPTIMIZE_HANDLE_VALIDITY_CHECK
            return true;
#endif  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
        }
#ifdef  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
        unsigned char GetSystemID()                     { return m_ucSystemID; }
#endif  X2OPTIMIZE_HANDLE_VALIDITY_CHECK

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX
         bool   IsInCriticalLoop() const { return m_bInCriticalLoop; }
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX

	private:
		bool ProcessXMeshPlayerBlock(
			std::vector<CXMeshPlayerToken>::iterator &TokenIter,
			std::vector<CXMeshPlayerToken>::iterator &EndIter);

		bool ProcessTempletBlock(XMeshTemplet &templet,
			std::vector<CXMeshPlayerToken>::iterator &TokenIter,
			std::vector<CXMeshPlayerToken>::iterator &EndIter);
		
		bool IsValidTemplet( const XMeshTemplet* pXMesshTemplet_ );
		wstring							m_Name;
//{{ robobeg : 2008-10-13
	    CKTDGStateManager::KStateID     m_RenderStateID;
//}} robobeg : 2008-10-13
		float							m_fElapsedTime;

		std::map<wstring,XMeshTemplet*>						m_TempletMap;

#ifdef EFFECT_TOOL
		vector<XMeshTemplet*>							m_TempletVec;
#endif //EFFECT_TOOL
		
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

        enum    EListType
        {
            LIST_LIVE = 0,
            LIST_FREE = 1,
            LIST_NUM
        };
        struct  KInstanceHandleInfo
        {
            CXMeshInstance*         m_pInstance;
            WORD                    m_wStamp;
            KInstanceHandleInfo()
                : m_pInstance( NULL )
                , m_wStamp(0)
            {
            }
        };
        typedef kog::indexed_list<KInstanceHandleInfo> KInstanceHandleList;
        KInstanceHandleList             m_coInstanceHandleList;

#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

		/// 내부적으로 관리하는 객체이므로 smart pointer를 사용하지 않는다.
        /// - jintaeks on 2009-01-12, 17:48
		std::vector<CXMeshInstance*>						m_InstanceList;
		std::map< CXMeshInstanceHandle, CXMeshInstance* >	m_mapInstance;
		static CXMeshInstanceHandle		s_iNextMeshInstanceHandle;
		CRITICAL_SECTION				m_csMeshPlayerLock;

#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX
        bool                            m_bInCriticalLoop;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX

		string							m_strLastError;
		bool							m_bEnable;

		bool							m_bEnableSlashTrace;

#ifdef NOT_RENDER_EFFECT_MADE_BY_GAME_UNIT
		bool									m_bRenderEffectMadeByGameUnit;
#endif //NOT_RENDER_EFFECT_MADE_BY_GAME_UNIT


#ifdef SHOW_ATTACK_BOX_DUMMY
		bool							m_bShowAttackBox;
#endif // SHOW_ATTACK_BOX_DUMMY

#ifdef  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
        unsigned char                           m_ucSystemID;
#endif  X2OPTIMIZE_HANDLE_VALIDITY_CHECK

};

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

template< typename FUNCTION >
void    CKTDGXMeshPlayer::ApplyFunctionToLiveInstances( FUNCTION fn )
{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX
    bool bOldInCriticalLoop = m_bInCriticalLoop;
    m_bInCriticalLoop = true;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX


    KInstanceHandleList::iterator iterEnd = m_coInstanceHandleList.end( LIST_LIVE );
    for( KInstanceHandleList::iterator iter = m_coInstanceHandleList.begin( LIST_LIVE );
        iter != iterEnd;
        ++iter )
    {
        CXMeshInstance* pInstance = iter->m_pInstance;
        if ( pInstance == NULL || pInstance->GetState() == IS_DIE )
            contniue;
        fn(*pInstance );
    }

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX
    m_bInCriticalLoop = bOldInCriticalLoop;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX
}

#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE