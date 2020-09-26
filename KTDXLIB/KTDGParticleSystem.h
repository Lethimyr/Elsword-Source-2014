#pragma once

#define D3DFVF_PARTICLE (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)
#define D3DFVF_PARTICLE_RHW (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)


#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
#include    "indexed_list.h"
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

#ifdef  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
struct  CKTDGParticleSystem_CParticleEventSequence_TAG {};
struct  CKTDGParticleSystem_CParticle_TAG {};

typedef KHandleType<int,CKTDGParticleSystem_CParticleEventSequence_TAG> CKTDGParticleSystem_CParticleEventSequenceHandle;
#define INVALID_PARTICLE_SEQUENCE_HANDLE    (CKTDGParticleSystem_CParticleEventSequenceHandle::invalid_handle())
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
typedef KHandleType<int,CKTDGParticleSystem_CParticle_TAG>              CKTDGParticleSystem_CParticleHandle;
#define INVALID_PARTICLE_HANDLE             (CKTDGParticleSystem_CParticleHandle::invalid_handle())
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

#else   X2OPTIMIZE_HANDLE_VALIDITY_CHECK

static const int  INVALID_PARTICLE_SEQUENCE_HANDLE = -1;
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
static const int  INVALID_PARTICLE_HANDLE = -1;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE


#endif  X2OPTIMIZE_HANDLE_VALIDITY_CHECK




class CKTDGParticleSystem
{
	public:
	
#ifdef  X2OPTIMIZE_HANDLE_VALIDITY_CHECK

		typedef CKTDGParticleSystem_CParticleEventSequenceHandle    CParticleEventSequenceHandle;
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        typedef CKTDGParticleSystem_CParticleHandle                 CParticleHandle;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

#else   X2OPTIMIZE_HANDLE_VALIDITY_CHECK

		typedef int	CParticleEventSequenceHandle;
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        typedef int CParticleHandle;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

#endif  X2OPTIMIZE_HANDLE_VALIDITY_CHECK

		class CParticleEvent;
		class CParticleEventSequence;

		struct VERTEX_PARTICLE
		{
			D3DXVECTOR3 position;
			D3DCOLOR    color;
			D3DXVECTOR2 tex;
		};

		struct VERTEX_PARTICLE_RHW
		{
			D3DXVECTOR3 position;
			float		rhw;
			D3DCOLOR    color;
			D3DXVECTOR2 tex;
		};

		enum PARTICLE_TYPE
		{
			PT_3D_PLANE,
			PT_2D_PLANE,
			PT_MESH,
			PT_SKINMESH,			
		};

		enum TokenType 
		{
			// kimhc // 2011-07-22 // 토큰이 무엇을 의미하는지는 DeterminTokenType() 함수를 보면 알 수 있음
			RealNumber = 1, 
			Equals, Comma, OpenParen, CloseParen, OpenBrace, CloseBrace, Quote, 

			KeywordParticleSystem, KeywordEventSequence,
			KeywordRandom, KeywordXYZ, KeywordXY, KeywordColor, KeywordTexture, AlphaBlendMode, CullMode,

			// system property tokens
			SeqAlphaBlendModeProp, SeqParticleTypeProp, SeqMeshRenderTypeProp, SeqBillboardTypeProp, SeqCullModeProp,

			// event time tokens
			KeywordFade, KeywordSo, KeywordAt, KeywordFrom, KeywordTo, KeywordInitial, KeywordFinal,

			// particle properties (event types)
			ParticleNumericProp, ParticleVectorProp, ParticleVector2Prop, ParticleColorProp, ParticleStringProp,

			// event sequence props
			SeqNumericProp, SeqVectorProp, SeqBoolProp, SeqStringProp,

			UnknownToken,
		};

		class CParticleEmitterToken  
		{
			public:
				CParticleEmitterToken() : m_Type( UnknownToken ) {}
				virtual ~CParticleEmitterToken(){}



				CParticleEmitterToken( const CParticleEmitterToken& rhs )
				{					
					*this = rhs;
				}

				CParticleEmitterToken& operator=( const CParticleEmitterToken& rhs )
				{
					m_strValue		= rhs.m_strValue;
					m_Type			= rhs.m_Type;

					return *this;
				}

				std::string m_strValue;
				TokenType	m_Type;

				bool IsEmitRate(void)			{ return(m_strValue.compare("EMITRATE")				== 0); }
				bool IsEmitRadius(void)			{ return(m_strValue.compare("EMITRADIUS")			== 0); }
				bool IsEmitRadiusMin(void)		{ return(m_strValue.compare("EMITRADIUSMIN")		== 0); }

#ifdef PARTICLE_SPHERICAL_EMIT_TEST
				bool IsSphericalEmitRadius(void)	{ return(m_strValue.compare("SPHERICAL_EMITRADIUS")		== 0); }
				bool IsSphericalEmitAzimuth(void)	{ return(m_strValue.compare("SPHERICAL_EMITAZIMUTH")	== 0); }
				bool IsSpehricalEmitPolar(void)		{ return(m_strValue.compare("SPHERICAL_EMITPOLAR")		== 0); }
				bool IsSpehricalEmitRotation(void)	{ return(m_strValue.compare("SPHERICAL_EMITROTATION")	== 0); }
#endif PARTICLE_SPHERICAL_EMIT_TEST

				bool IsLifeTime(void)			{ return(m_strValue.compare("LIFETIME")				== 0); }
				bool IsNumParticles(void)		{ return(m_strValue.compare("NUMPARTICLES")			== 0); }
				bool IsGravity(void)			{ return(m_strValue.compare("GRAVITY")				== 0); }
				bool IsSrcBlendMode(void)		{ return(m_strValue.compare("SOURCEBLENDMODE")		== 0); }
				bool IsDestBlendMode(void)		{ return(m_strValue.compare("DESTBLENDMODE")		== 0); }
				bool IsCullMode(void)			{ return(m_strValue.compare("CULLMODE")				== 0); }
				bool IsParticleType(void)		{ return(m_strValue.compare("PARTICLETYPE")			== 0); }
				bool IsResolutionConvert(void)	{ return(m_strValue.compare("RESOLUTIONCONVERT")	== 0); }
				bool IsLayer(void)				{ return(m_strValue.compare("LAYER")				== 0); }
				bool IsForceLayer(void)			{ return(m_strValue.compare("FORCELAYER")			== 0); }
				bool IsAddRotateRel(void)		{ return(m_strValue.compare("ADDROTATEREL")			== 0); }

				bool IsZWriteEnable(void)		{ return(m_strValue.compare("ZWRITEENABLE")			== 0); }
				bool IsZEnable(void)			{ return(m_strValue.compare("ZENABLE")				== 0); }

				bool IsMesh(void)				{ return(m_strValue.compare("MESH")					== 0); }
				bool IsSkinMesh(void)			{ return(m_strValue.compare("SKINMESH")				== 0); }

				bool IsChangeTexXET(void)		{ return(m_strValue.compare("CHANGETEXXET")			== 0); }
				bool IsMultiTexXET(void)		{ return(m_strValue.compare("MULTITEXXET")			== 0); }
				bool IsAniXET(void)				{ return(m_strValue.compare("ANIXET")				== 0); }
				bool IsAniName(void)			{ return(m_strValue.compare("ANINAME")				== 0); }
				bool IsMeshRenderType(void)		{ return(m_strValue.compare("MESHRENDERTYPE")		== 0); }
				bool IsLightPos(void)			{ return(m_strValue.compare("LIGHTPOS")				== 0); }

				bool IsTriggerCount(void)		{ return(m_strValue.compare("TRIGGERCOUNT")			== 0); }
				bool IsTriggerTime(void)		{ return(m_strValue.compare("TRIGGERTIME")			== 0); }
				bool IsDrawCount(void)			{ return(m_strValue.compare("DRAWCOUNT")			== 0); }
				bool IsBillboardType(void)		{ return(m_strValue.compare("BILLBOARDTYPE")		== 0); }
				bool IsTraceParticleList(void)	{ return(m_strValue.compare("TRACEPARTICLELIST")	== 0); }
				bool IsFinalParticleList(void)	{ return(m_strValue.compare("FINALPARTICLELIST")	== 0); }
				bool IsBlackHolePos(void)		{ return(m_strValue.compare("BLACKHOLEPOS")			== 0); }
				bool IsUseLand(void)			{ return(m_strValue.compare("USELAND")				== 0); }
				bool IsLandPos(void)			{ return(m_strValue.compare("LANDPOS")				== 0); }
				bool IsTrace(void)				{ return(m_strValue.compare("TRACE")				== 0); }
				bool IsScrew(void)				{ return(m_strValue.compare("SCREWVALUE")			== 0); }
				bool IsScrewRotateSpeed(void)	{ return(m_strValue.compare("SCREWROTATESPEED")		== 0); }
#ifdef PARTICLE_NOTAPPLY_UNITSCALE
				bool IsApplyUnitScale(void)		{ return(m_strValue.compare("APPLYUNITSCALE")		== 0); }
#endif
#ifdef ADD_ALPHATESTENABLE
				bool IsAlphaTest(void)		{ return(m_strValue.compare("ALPHATEST")			== 0); }
#endif //ADD_ALPHATESTENABLE
		};


		class CParticle
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            : private boost::noncopyable
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		{
            private:

                friend class    CParticleEventSequence;

                void    Init()
                {
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
                    m_hHandle = INVALID_PARTICLE_HANDLE;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

					m_pMasterSeq		= NULL;
					m_fLifetime			= 1.0f; 
					m_fAge				= 0.0f; 
					m_fEventTimer		= 0.0f;
					m_fEventTimerOld	= 0.0f;
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION_VELOCITY_ACCUM_BUG_FIX
                    m_fEventTimerVelocity = 0.0f;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION_VELOCITY_ACCUM_BUG_FIX
					m_Color				= D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
					m_ColorFinal		= D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);

					INIT_VECTOR3( m_vSize, 1.0f, 1.0f, 1.0f );
					INIT_VECTOR3( m_vSizeFinal, 1.0f, 1.0f, 1.0f );


#ifdef PARTICLE_STRETCH_TEST
					INIT_VECTOR3( m_vPosOld, 0.0f,0.0f,0.0f );
					m_fStretchScale = 1.f;
					m_fStretchScaleFinal = 1.f;
#endif PARTICLE_STRETCH_TEST


					INIT_VECTOR3( m_vPos, 0.0f,0.0f,0.0f );
//{{ robobeg : 2008-10-28
                    INIT_VECTOR3( m_vPosCrash, 0.0f,0.0f,0.0f );
//}} robobeg : 2008-10-28
					INIT_VECTOR3( m_vPosOrg, 0.0f, 0.0f, 0.0f );
					INIT_VECTOR3( m_vVelocity, 0.0f,0.0f,0.0f );
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
                    m_bNoVelocityToAccumPos = false;
                    INIT_VECTOR3( m_vVelocityToAccumPos, 0.f, 0.f, 0.f );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
					INIT_VECTOR3( m_vVelocityFinal, 0.0f,0.0f,0.0f );

					INIT_VECTOR3( m_vPositionGap, 0.0f,0.0f,0.0f );
					INIT_VECTOR3( m_vPositionFinal, 0.0f,0.0f,0.0f );

					INIT_VECTOR2( m_vTexStage0UV, 0.0f,0.0f );
					INIT_VECTOR2( m_vTexStage0UVFinal, 0.0f,0.0f );

					INIT_VECTOR2( m_vTexStage1UV, 0.0f,0.0f );
					INIT_VECTOR2( m_vTexStage1UVFinal, 0.0f,0.0f );

					INIT_VECTOR2( m_vTexStage2UV, 0.0f,0.0f );
					INIT_VECTOR2( m_vTexStage2UVFinal, 0.0f,0.0f );

					INIT_VECTOR3( m_vAxisRotateDegree, 0.0f,0.0f,0.0f );
					INIT_VECTOR3( m_vRotate, 0.0f,0.0f,0.0f );
					INIT_VECTOR3( m_vRotateFinal, 0.0f,0.0f,0.0f );

//#ifdef LOCAL_ROTATE_EFFECT_P
					INIT_VECTOR3( m_vRotateLocal, 0.0f,0.0f,0.0f );
//#endif

					m_fDirSpeed			= 0.0f;
					m_fDirSpeedFinal	= 0.0f;

					INIT_VECTOR3( m_vBlackHoleSpeed, 0.0f,0.0f,0.0f );
					m_fBlackHoleTime		= 0.0f;

					m_fResetCrash		= 0.01f;
					m_fNowResetCrash	= 0.0f;

					INIT_VECTOR3( m_vCrash, 0.0f,0.0f,0.0f );

					m_TextureID		= 0;

					m_ScrewValue	= -1;
					m_ScrewRotateSpeed = 10;
					m_fScrewAngle	= 0.0f;

                    m_TraceSeqList.resize( 0 );

#ifdef SKINMESH_PARTICLE_TEST
					m_pXSkinAnim.reset();
#endif SKINMESH_PARTICLE_TEST
                }

                void    Finalize()
                {
#ifdef SKINMESH_PARTICLE_TEST
					m_pXSkinAnim.reset();
#endif SKINMESH_PARTICLE_TEST

					ClearTraceSeqList();
                }

			public:

				CParticle()
				{
                    Init();
				}
				~CParticle()
				{
					Finalize();
				}
#ifndef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
				CParticle( const CParticle& rhs )
				{					
					*this = rhs;
				}
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
                CParticleHandle GetHandle() const { return m_hHandle; }
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

				bool OnFrameMove( double fTime, float fElapsedTime )
				{
					KTDXPROFILE();

					// age the particle
					m_fAge				+= fElapsedTime;
					m_fEventTimerOld	= m_fEventTimer;
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION_VELOCITY_ACCUM_BUG_FIX
                    m_fEventTimerVelocity = m_fEventTimer;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION_VELOCITY_ACCUM_BUG_FIX
					m_fEventTimer		+= fElapsedTime;
					m_fNowResetCrash	+= fElapsedTime;

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
                    if ( m_bNoVelocityToAccumPos == true )
                    {
                        m_bNoVelocityToAccumPos = false;
                        m_vVelocityToAccumPos += m_vVelocity * fElapsedTime;
                    }
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION

					//m_vPos = m_vPosOrg;

#ifdef PARTICLE_STRETCH_TEST
					m_vPosOld = m_vPos;
#endif PARTICLE_STRETCH_TEST

					// if this particle's age is greater than it's lifetime, it dies.
					if (m_fAge >= m_fLifetime)
					{
						ClearTraceSeqList();
						return false; // dead!
					}

					// move particle
					D3DXMATRIX	mDir;
					D3DXVECTOR4 dirVec4;

					if( m_vAxisRotateDegree == D3DXVECTOR3(0.0f,0.0f,0.0f) )
					{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
                        m_vPos += m_vVelocityToAccumPos;
                        ResetVelocityToAccumPos();
#else   X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
						m_vPos += m_vVelocity * fElapsedTime;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
					}
					else
					{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
                        D3DXVECTOR3 tempMoveGap = m_vVelocityToAccumPos;
                        ResetVelocityToAccumPos();
#else   X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
                        D3DXVECTOR3 tempMoveGap = m_vVelocity * fElapsedTime;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
						D3DXMatrixRotationYawPitchRoll( &mDir, D3DXToRadian(m_vAxisRotateDegree.y), D3DXToRadian(m_vAxisRotateDegree.x), D3DXToRadian(m_vAxisRotateDegree.z) );
						D3DXVec3Transform( &dirVec4, &tempMoveGap, &mDir );
						tempMoveGap.x	= dirVec4.x;
						tempMoveGap.y	= dirVec4.y;
						tempMoveGap.z	= dirVec4.z;

						m_vPos		+= tempMoveGap;
					}

					m_vPos += m_vPositionGap;
					
					if( m_fDirSpeed != 0.0f )
					{
						D3DXVECTOR3 dirVec( 1.0f, 0.0f, 0.0f );					
//#ifdef LOCAL_ROTATE_EFFECT_P
						D3DXVECTOR3 vRot = D3DXVECTOR3( D3DXToRadian(m_vRotate.x+m_vRotateLocal.x), D3DXToRadian(m_vRotate.y+m_vRotateLocal.y), D3DXToRadian(m_vRotate.z+m_vRotateLocal.z) );
//#else
//						D3DXVECTOR3 vRot = D3DXVECTOR3( D3DXToRadian(m_vRotate.x), D3DXToRadian(m_vRotate.y), D3DXToRadian(m_vRotate.z) );
//#endif

						D3DXMatrixRotationYawPitchRoll( &mDir, vRot.y, vRot.x, vRot.z );
						D3DXVec3Transform( &dirVec4, &dirVec, &mDir );
						dirVec.x	= dirVec4.x;
						dirVec.y	= dirVec4.y;
						dirVec.z	= dirVec4.z;

						m_vPos		+= dirVec * m_fDirSpeed * fElapsedTime;
					}
					m_vPos		+= m_vBlackHoleSpeed		* fElapsedTime;

					/*
					if ( m_ScrewValue != -1 )
					{
						m_fScrewAngle += ( fElapsedTime * m_ScrewRotateSpeed );
						if ( m_fScrewAngle > 6.28f )
							m_fScrewAngle = 0.0f;

						D3DXVECTOR3 tempMoveGap = D3DXVECTOR3( ( cosf(m_fScrewAngle) * m_ScrewValue )+m_vPos.x, m_vPos.y , ( sinf(m_fScrewAngle) * m_ScrewValue) +m_vPos.z) - m_vPos;
						
						D3DXMatrixRotationYawPitchRoll( &mDir, D3DXToRadian(m_vAxisRotateDegree.y), D3DXToRadian(m_vAxisRotateDegree.x ), D3DXToRadian(m_vAxisRotateDegree.z + 90) );
						D3DXVec3Transform( &dirVec4, &tempMoveGap, &mDir );
						tempMoveGap.x	= dirVec4.x;
						tempMoveGap.y	= dirVec4.y;
						tempMoveGap.z	= dirVec4.z;

						m_vPos		+= tempMoveGap;
					}
					*/

#ifdef SKINMESH_PARTICLE_TEST

					if( NULL != m_pMasterSeq && 
						m_pMasterSeq->GetParticleType() == PT_SKINMESH )
					{
//#ifdef LOCAL_ROTATE_EFFECT_P
						D3DXVECTOR3 vRot = D3DXVECTOR3( D3DXToRadian(m_vRotate.x+m_vRotateLocal.x), D3DXToRadian(m_vRotate.y+m_vRotateLocal.y), D3DXToRadian(m_vRotate.z+m_vRotateLocal.z) );
//#else
//						D3DXVECTOR3 vRot = D3DXVECTOR3( D3DXToRadian(m_vRotate.x), D3DXToRadian(m_vRotate.y), D3DXToRadian(m_vRotate.z) );
//#endif LOCAL_ROTATE_EFFECT_P

						m_vPosCrash = m_vPos + m_vCrash; 
						m_vCrash = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );

						ASSERT( NULL != m_pXSkinAnim );
						if( NULL != m_pXSkinAnim )
						{
							m_pXSkinAnim->GetMatrix().Rotate( vRot );
							m_pXSkinAnim->GetMatrix().Move( m_vPosCrash );

#ifdef GIANT_UNIT_GIANT_EFFECT_TEST
							m_pXSkinAnim->GetMatrix().Scale( m_vSize.x * m_pMasterSeq->GetScaleFactor().x * m_pMasterSeq->GetScaleByUnit().x, 
								m_vSize.y * m_pMasterSeq->GetScaleFactor().y * m_pMasterSeq->GetScaleByUnit().y,
								m_vSize.z * m_pMasterSeq->GetScaleFactor().z * m_pMasterSeq->GetScaleByUnit().z); 
#else GIANT_UNIT_GIANT_EFFECT_TEST
							m_pXSkinAnim->GetMatrix().Scale( m_vSize.x * m_pMasterSeq->GetScaleFactor().x, m_vSize.y * m_pMasterSeq->GetScaleFactor().y, m_vSize.z * m_pMasterSeq->GetScaleFactor().z ); 
#endif GIANT_UNIT_GIANT_EFFECT_TEST

							m_pXSkinAnim->OnFrameMove( fTime, fElapsedTime );
						}
					}

#endif SKINMESH_PARTICLE_TEST



					if( m_pMasterSeq != NULL && 
						m_pMasterSeq->GetParticleSystem() != NULL )
					{
						for( int i = 0; i < (int)m_TraceSeqList.size(); i++ )
						{
							CParticleEventSequence* pSeq = m_pMasterSeq->GetParticleSystem()->GetInstanceSequence( m_TraceSeqList[i] );
							if( NULL != pSeq )
							{
								pSeq->SetPosition( m_vPos );
							}
							else
							{
								m_TraceSeqList.erase( m_TraceSeqList.begin() + i );
								--i;
							}
						}
					}

					return true; // particle stays alive
				}

				void Die(){ m_fAge = m_fLifetime; }
				void ClearTraceSeqList()
				{
					if( m_pMasterSeq != NULL && 
						m_pMasterSeq->GetParticleSystem() != NULL )
					{
						for( UINT i = 0; i < m_TraceSeqList.size(); i++ )
						{
							CParticleEventSequence* pSeq = m_pMasterSeq->GetParticleSystem()->GetInstanceSequence( m_TraceSeqList[i] );
							if( NULL != pSeq )
							{
								pSeq->SetAutoDie();
							}
						}
					}

					m_TraceSeqList.resize(0);
				}	
//{{ robobeg : 2013-11-06
                CParticleEventSequence*     GetMasterSequence() const { return m_pMasterSeq; }
                float                       GetAge() const  { return m_fAge; }
                void                        SetAge( float fAge )    { m_fAge = fAge; }
                float                       GetEventTimer() const   { return    m_fEventTimer; }
                float                       GetEventTimerOld() const   { return    m_fEventTimerOld; }
                void                        SetEventTimer( float fEventTime )   
                                            { 
                                                m_fEventTimer = fEventTime; 
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION_VELOCITY_ACCUM_BUG_FIX
                                                m_fEventTimerVelocity = fEventTime;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION_VELOCITY_ACCUM_BUG_FIX
                                            }


                float                       GetStretchScale() const     { return m_fStretchScale; }
                void                        SetStretchScale( float fValue )     { m_fStretchScale = fValue; }
                float                       GetStretchScaleFinal() const     { return m_fStretchScaleFinal; }
                void                        SetStretchScaleFinal( float fValue )     { m_fStretchScaleFinal = fValue; }


                float                       GetDirSpeed() const     { return m_fDirSpeed; }
                void                        SetDirSpeed( float fValue ) { m_fDirSpeed = fValue; }
                float                       GetDirSpeedFinal() const     { return m_fDirSpeedFinal; }
                void                        SetDirSpeedFinal( float fValue ) { m_fDirSpeedFinal = fValue; }
                float                       GetBlackHoleTime() const     { return m_fBlackHoleTime; }
                void                        SetBlackHoleTime( float fValue ) { m_fBlackHoleTime = fValue; }
                float                       GetResetCrash() const     { return m_fResetCrash; }
                void                        SetResetCrash( float fValue ) { m_fResetCrash = fValue; }
                float                       GetNowResetCrash() const     { return m_fNowResetCrash; }
                void                        SetNowResetCrash( float fValue ) { m_fNowResetCrash = fValue; }
                float                       GetScrewAngle() const     { return m_fScrewAngle; }
                void                        SetScrewAngle( float fValue ) { m_fScrewAngle = fValue; }

                int                         GetTextureID() const    { return m_TextureID; }
                void                        SetTextureID( char id ) { m_TextureID = id; }
                int                         GetScrewValue() const   { return m_ScrewValue; }
                int                         GetScrewRotateSpeed() const{    return m_ScrewRotateSpeed; }


                const D3DXCOLOR&            GetColor() const        { return m_Color; }
                void                        SetColor( const D3DXCOLOR& color )  { m_Color = color; }
                const D3DXCOLOR&            GetColorFinal() const        { return m_ColorFinal; }
                void                        SetColorFinal( const D3DXCOLOR& color )  { m_ColorFinal = color; }

                const D3DXVECTOR3&          GetSize() const                     { return m_vSize; }
                void                        SetSize( const D3DXVECTOR3& vSize ) { m_vSize = vSize; }
                const D3DXVECTOR3&          GetSizeFinal() const                     { return m_vSizeFinal; }
                void                        SetSizeFinal( const D3DXVECTOR3& vSizeFinal ) { m_vSizeFinal = vSizeFinal; }
                const D3DXVECTOR3&          GetPos() const                      { return m_vPos; }
                void                        SetPos( const D3DXVECTOR3& vPos )   { m_vPos = vPos; }
                const D3DXVECTOR3&          GetPosOrg() const                      { return m_vPosOrg; }
                void                        SetPosOrg( const D3DXVECTOR3& vPos )   { m_vPosOrg = vPos; }
                const D3DXVECTOR3&          GetPosCrash() const                      { return m_vPosCrash; }
                void                        SetPosCrash( const D3DXVECTOR3& vPos )   { m_vPosCrash = vPos; }

                const D3DXVECTOR3&          GetVelocity() const                         { return m_vVelocity; }
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
                void                        SetVelocity( const D3DXVECTOR3& vVelocity, bool bInvalidateAccumPos = true )         
                                            { 
                                                m_vVelocity = vVelocity; 
                                                if ( bInvalidateAccumPos == true )
                                                {
                                                    INIT_VECTOR3( m_vVelocityToAccumPos, 0, 0, 0 );
                                                    m_bNoVelocityToAccumPos = true;
                                                }
                                            }
                void                        ResetVelocityToAccumPos()   { INIT_VECTOR3( m_vVelocityToAccumPos, 0, 0, 0 ); m_bNoVelocityToAccumPos = false; }
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION_VELOCITY_ACCUM_BUG_FIX
                float                       GetEventTimerVelocity() const                   { return m_fEventTimerVelocity; }
                void                        UpdateVelocityAccumPosAndEventTimer( float fTime )
                                            {
                                                if ( m_fEventTimerVelocity < fTime )    m_vVelocityToAccumPos += m_vVelocity * ( fTime - m_fEventTimerVelocity );
                                                m_fEventTimerVelocity = fTime;
                                            }
#else   X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION_VELOCITY_ACCUM_BUG_FIX
                void                        AddVelocityToAccumPos( float fElapsedTime ) { m_vVelocityToAccumPos += m_vVelocity * fElapsedTime; }
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION_VELOCITY_ACCUM_BUG_FIX

#else   X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
                void                        SetVelocity( const D3DXVECTOR3& vVelocity )         { m_vVelocity = vVelocity; }
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
                const D3DXVECTOR3&          GetVelocityFinal() const                         { return m_vVelocityFinal; }
                void                        SetVelocityFinal( const D3DXVECTOR3& vVelocityFinal )         { m_vVelocityFinal = vVelocityFinal; }
                const D3DXVECTOR3&          GetPositionGap() const                         { return m_vPositionGap; }
                void                        SetPositionGap( const D3DXVECTOR3& vPositionGap )         { m_vPositionGap = vPositionGap; }
                const D3DXVECTOR3&          GetPositionFinal() const                         { return m_vPositionFinal; }
                void                        SetPositionFinal( const D3DXVECTOR3& vPositionFinal )         { m_vPositionFinal = vPositionFinal; }
                const D3DXVECTOR3&          GetAxisRotateDegree() const                         { return m_vAxisRotateDegree; }
                void                        SetAxisRotateDegree( const D3DXVECTOR3& vAxisRotateDegree )         { m_vAxisRotateDegree = vAxisRotateDegree; }
                const D3DXVECTOR3&          GetRotate() const                         { return m_vRotate; }
                void                        SetRotate( const D3DXVECTOR3& vRotate )         { m_vRotate = vRotate; }
                const D3DXVECTOR3&          GetRotateFinal() const                         { return m_vRotateFinal; }
                void                        SetRotateFinal( const D3DXVECTOR3& vRotateFinal )         { m_vRotateFinal = vRotateFinal; }
                const D3DXVECTOR3&          GetBlackHoleSpeed() const                         { return m_vBlackHoleSpeed; }
                void                        SetBlackHoleSpeed( const D3DXVECTOR3& vBlackHoleSpeed )         { m_vBlackHoleSpeed = vBlackHoleSpeed; }
//#ifdef  LOCAL_ROTATE_EFFECT_P
                const D3DXVECTOR3&          GetRotateLocal() const                         { return m_vRotateLocal; }
                void                        SetRotateLocal( const D3DXVECTOR3& vRotateLocal )         { m_vRotateLocal = vRotateLocal; }
//#endif  LOCAL_ROTATE_EFFECT_P
                const D3DXVECTOR3&          GetCrash() const                         { return m_vCrash; }
                void                        SetCrash( const D3DXVECTOR3& vCrash )         { m_vCrash = vCrash; }

                const D3DXVECTOR2&          GetTexStage0UV() const             { return m_vTexStage0UV; }
                void                        SetTexStage0UV( const D3DXVECTOR2& vText ) { m_vTexStage0UV = vText; }
                const D3DXVECTOR2&          GetTexStage0UVFinal() const             { return m_vTexStage0UVFinal; }
                void                        SetTexStage0UVFinal( const D3DXVECTOR2& vText ) { m_vTexStage0UVFinal = vText; }
                const D3DXVECTOR2&          GetTexStage1UV() const             { return m_vTexStage1UV; }
                void                        SetTexStage1UV( const D3DXVECTOR2& vText ) { m_vTexStage1UV = vText; }
                const D3DXVECTOR2&          GetTexStage1UVFinal() const             { return m_vTexStage1UVFinal; }
                void                        SetTexStage1UVFinal( const D3DXVECTOR2& vText ) { m_vTexStage1UVFinal = vText; }
                const D3DXVECTOR2&          GetTexStage2UV() const             { return m_vTexStage2UV; }
                void                        SetTexStage2UV( const D3DXVECTOR2& vText ) { m_vTexStage2UV = vText; }
                const D3DXVECTOR2&          GetTexStage2UVFinal() const             { return m_vTexStage2UVFinal; }
                void                        SetTexStage2UVFinal( const D3DXVECTOR2& vText ) { m_vTexStage2UVFinal = vText; }

//}} robobeg : 2013-11-06

            private:

				//Variable
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
                CParticleHandle             m_hHandle;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
				CParticleEventSequence*		m_pMasterSeq;
				float						m_fLifetime;
				float						m_fAge;
				float						m_fEventTimer;			// different from age
				float						m_fEventTimerOld;		// different from age

				D3DXCOLOR					m_Color;
				D3DXCOLOR					m_ColorFinal;

				D3DXVECTOR3					m_vSize;
				D3DXVECTOR3					m_vSizeFinal;

#ifdef PARTICLE_STRETCH_TEST
				float						m_fStretchScale;
				float						m_fStretchScaleFinal;
				D3DXVECTOR3					m_vPosOld;
#endif PARTICLE_STRETCH_TEST
				D3DXVECTOR3					m_vPos;				// current position of particle
//{{ robobeg : 2008-10-28
                // m_vCrash 가 반영된 m_vPos, OnFrameRenderMesh_Draw 및 OnFrameRenderSkinMesh_Draw 에서 사용됨
                D3DXVECTOR3                 m_vPosCrash;
//}} robobeg : 2008-10-28
				D3DXVECTOR3					m_vPosOrg;
				D3DXVECTOR3					m_vVelocity;
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
                bool                        m_bNoVelocityToAccumPos;
                D3DXVECTOR3                 m_vVelocityToAccumPos;
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION_VELOCITY_ACCUM_BUG_FIX
                float                       m_fEventTimerVelocity;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION_VELOCITY_ACCUM_BUG_FIX
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
				D3DXVECTOR3					m_vVelocityFinal;

				D3DXVECTOR3					m_vPositionGap;
				D3DXVECTOR3					m_vPositionFinal;

				D3DXVECTOR2					m_vTexStage0UV;
				D3DXVECTOR2					m_vTexStage0UVFinal;

				D3DXVECTOR2					m_vTexStage1UV;
				D3DXVECTOR2					m_vTexStage1UVFinal;

				D3DXVECTOR2					m_vTexStage2UV;
				D3DXVECTOR2					m_vTexStage2UVFinal;

				D3DXVECTOR3					m_vAxisRotateDegree;
				D3DXVECTOR3					m_vRotate;
				D3DXVECTOR3					m_vRotateFinal;

//#ifdef LOCAL_ROTATE_EFFECT_P
				D3DXVECTOR3					m_vRotateLocal;
//#endif

				float						m_fDirSpeed;		//로테이션 방향으로 전진속도
				float						m_fDirSpeedFinal;

				D3DXVECTOR3					m_vBlackHoleSpeed;
				float						m_fBlackHoleTime;

				float						m_fResetCrash;
				float						m_fNowResetCrash;
				D3DXVECTOR3					m_vCrash;

				//char						m_TextureID;
                int                         m_TextureID;

				int							m_ScrewValue;
				int							m_ScrewRotateSpeed;
				float						m_fScrewAngle;

				vector<CParticleEventSequenceHandle> m_TraceSeqList;

#ifdef SKINMESH_PARTICLE_TEST
				CKTDGXSkinAnimPtr				m_pXSkinAnim;
#endif SKINMESH_PARTICLE_TEST

		};//class CParticle

#ifndef X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		typedef std::list<CParticle*> CParticleList;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

		enum EVENT_TYPE
		{
			ET_BASIC,
			ET_COLOR,
			ET_SIZE,
			ET_SCALE,
			ET_VELOCITY,
			ET_POSITION,
			ET_TEX0UV,
			ET_TEX1UV,
			ET_TEX2UV,
			ET_ROTATE,
			ET_DIRSPEED,
			ET_BLACKHOLE,
			ET_CRASH,
			ET_RESETCRASH,
			ET_TEXTURE,
			ET_EVENTTIMER,
#ifdef PARTICLE_STRETCH_TEST
			ET_STRETCH,
#endif PARTICLE_STRETCH_TEST
#ifdef X2VIEWER
			// 2009.12 태완 : X2Viewer에서 파티클 수정 기능 관련을 위해 추가. 새 이벤트 타입을 추가하실때는 이 위로 해 주세요.
			ET_END,
#endif
		};
		
		class CParticleEvent
		{
			public:
				CParticleEvent() : m_bFade( false ) { m_EventType = ET_BASIC; }
				virtual ~CParticleEvent(){}

				EVENT_TYPE		GetEventType(){ return m_EventType; }

				bool			IsFade()					{ return m_bFade; }
				void			SetFade( bool fade = true )	{ m_bFade = fade; }

				virtual void	OnFrameMove( CParticle &part, float fElapsedTime ) = 0;
				virtual bool	FadeAllowed() = 0;

				const CMinMax<float>&	GetActualTime() const { return m_ActualTime; }
				void SetActualTime( const CMinMax<float>& actualTime ){ m_ActualTime = actualTime; }

				virtual bool ProcessTokenStream(std::vector<CParticleEmitterToken>::iterator &TokenIter, 
												std::vector<CParticleEmitterToken>::iterator &EndIter) = 0;

				static void ProcessPropEqualsValue(CMinMax<float> &prop,
									std::vector<CParticleEmitterToken>::iterator &TokenIter, 
									std::vector<CParticleEmitterToken>::iterator &EndIter);

				static void ProcessPropEqualsValue(CMinMax<D3DXVECTOR3> &prop,
									std::vector<CParticleEmitterToken>::iterator &TokenIter, 
									std::vector<CParticleEmitterToken>::iterator &EndIter);

				static void ProcessPropEqualsValue(CMinMax<D3DXVECTOR2> &prop,
									std::vector<CParticleEmitterToken>::iterator &TokenIter, 
									std::vector<CParticleEmitterToken>::iterator &EndIter);

				static void ProcessPropEqualsValue(CMinMax<D3DXCOLOR> &prop,
									std::vector<CParticleEmitterToken>::iterator &TokenIter, 
									std::vector<CParticleEmitterToken>::iterator &EndIter);

				static void ProcessPropEqualsValue(wstring &prop,
									std::vector<CParticleEmitterToken>::iterator &TokenIter, 
									std::vector<CParticleEmitterToken>::iterator &EndIter);
#ifdef EFFECT_TOOL
				void SetEventType( EVENT_TYPE eType ) { m_EventType = eType; }
#endif //EFFECT_TOOL


			protected:
				EVENT_TYPE		m_EventType;
				bool			m_bFade;
				CMinMax<float>	m_ActualTime;
		};

		class CParticleEvent_Color : public CParticleEvent
		{
			public:
				CParticleEvent_Color(){ m_EventType = ET_COLOR; }
				
				virtual void	OnFrameMove( CParticle &part, float fElapsedTime );
				bool FadeAllowed() { return true; }
				
				CMinMax<D3DXCOLOR>	GetColor()									{ return m_Color; }
				void				SetColor(const CMinMax<D3DXCOLOR> &data)	{ m_Color = data; }

				virtual bool ProcessTokenStream(std::vector<CParticleEmitterToken>::iterator &TokenIter, 
												std::vector<CParticleEmitterToken>::iterator &EndIter);

			protected:
				CMinMax<D3DXCOLOR> m_Color;
		};

		class CParticleEvent_Size : public CParticleEvent
		{
			public:
				CParticleEvent_Size(){ m_EventType = ET_SIZE; }

				virtual void	OnFrameMove( CParticle &part, float fElapsedTime );
				bool FadeAllowed(){ return true; }
				
				CMinMax<D3DXVECTOR3>	GetSize(){ return m_Size; }
				void					SetSize(CMinMax<D3DXVECTOR3> data) { m_Size = data; }

				virtual bool ProcessTokenStream(std::vector<CParticleEmitterToken>::iterator &TokenIter, 
												std::vector<CParticleEmitterToken>::iterator &EndIter);

			protected:
				CMinMax<D3DXVECTOR3> m_Size;   
		};


		// CParticleEvent_Size, XYZ축 scaling 값이 같다는 점, 즉 모양이 유지되면서 scaling
		// initial			size는 꼭 필요
		// fade size, fade scale은 서로 충돌, 같이 사용하면 무슨일이 발생할지 알 수 없음.
		// 사용예:
		//		initial			size		= XYZ(64,64,1)
		//		fade so final	size		= XYZ(0,0,0)
		//		initial			scale		= random(1, 5)
		class CParticleEvent_Scale : public CParticleEvent
		{
		public:
			CParticleEvent_Scale(){ m_EventType = ET_SCALE; }

			virtual void	OnFrameMove( CParticle &part, float fElapsedTime );
			bool FadeAllowed(){ return true; }

			CMinMax<float>			GetScale(){ return m_Scale; }
			void					SetScale(CMinMax<float> data) { m_Scale = data; }

			virtual bool ProcessTokenStream(std::vector<CParticleEmitterToken>::iterator &TokenIter, 
				std::vector<CParticleEmitterToken>::iterator &EndIter);

		protected:
			CMinMax<float> m_Scale;   
		};

		class CParticleEvent_Velocity : public CParticleEvent
		{
			public:
				CParticleEvent_Velocity(){ m_EventType = ET_VELOCITY; }
				
				virtual void	OnFrameMove( CParticle &part, float fElapsedTime );
				bool FadeAllowed(){ return true; }
				
				CMinMax<D3DXVECTOR3>	GetVelocity()									{ return m_Velocity; }
				void					SetVelocity(const CMinMax<D3DXVECTOR3> &data)	{ m_Velocity = data; }

				virtual bool ProcessTokenStream(std::vector<CParticleEmitterToken>::iterator &TokenIter, 
												std::vector<CParticleEmitterToken>::iterator &EndIter);

			protected:
				CMinMax<D3DXVECTOR3> m_Velocity;
		};

		class CParticleEvent_Position : public CParticleEvent
		{
			public:
				CParticleEvent_Position(){ m_EventType = ET_POSITION; }

				virtual void	OnFrameMove( CParticle &part, float fElapsedTime );
				bool FadeAllowed(){ return true; }

				CMinMax<D3DXVECTOR3>	GetPosition()									{ return m_Position; }
				void					SetPosition(CMinMax<D3DXVECTOR3> &data)	{ m_Position = data; }

				virtual bool ProcessTokenStream(std::vector<CParticleEmitterToken>::iterator &TokenIter, 
					std::vector<CParticleEmitterToken>::iterator &EndIter);

			protected:
				CMinMax<D3DXVECTOR3> m_Position;
		};

		class CParticleEvent_Tex0UV : public CParticleEvent
		{
			public:
				CParticleEvent_Tex0UV(){ m_EventType = ET_TEX0UV; }

				virtual void	OnFrameMove( CParticle &part, float fElapsedTime );
				bool FadeAllowed(){ return true; }
				
				CMinMax<D3DXVECTOR2>	GetTexUV()									{ return m_TexUV; }
				void					SetTexUV(const CMinMax<D3DXVECTOR2> &data)	{ m_TexUV = data; }

				virtual bool ProcessTokenStream(std::vector<CParticleEmitterToken>::iterator &TokenIter, 
												std::vector<CParticleEmitterToken>::iterator &EndIter);

			protected:
				CMinMax<D3DXVECTOR2>	m_TexUV;
		};

		class CParticleEvent_Tex1UV : public CParticleEvent
		{
			public:
				CParticleEvent_Tex1UV(){ m_EventType = ET_TEX1UV; }

				virtual void	OnFrameMove( CParticle &part, float fElapsedTime );
				bool FadeAllowed(){ return true; }

				CMinMax<D3DXVECTOR2>	GetTexUV()									{ return m_TexUV; }
				void					SetTexUV(const CMinMax<D3DXVECTOR2> &data)	{ m_TexUV = data; }

				virtual bool ProcessTokenStream(std::vector<CParticleEmitterToken>::iterator &TokenIter, 
					std::vector<CParticleEmitterToken>::iterator &EndIter);

			protected:
				CMinMax<D3DXVECTOR2>	m_TexUV;
		};

		class CParticleEvent_Tex2UV : public CParticleEvent
		{
			public:
				CParticleEvent_Tex2UV(){ m_EventType = ET_TEX2UV; }

				virtual void	OnFrameMove( CParticle &part, float fElapsedTime );
				bool FadeAllowed(){ return true; }

				CMinMax<D3DXVECTOR2>	GetTexUV()									{ return m_TexUV; }
				void					SetTexUV(const CMinMax<D3DXVECTOR2> &data)	{ m_TexUV = data; }

				virtual bool ProcessTokenStream(std::vector<CParticleEmitterToken>::iterator &TokenIter, 
					std::vector<CParticleEmitterToken>::iterator &EndIter);

			protected:
				CMinMax<D3DXVECTOR2>	m_TexUV;
		};

		class CParticleEvent_Rotate : public CParticleEvent
		{
			public:
				CParticleEvent_Rotate(){ m_EventType = ET_ROTATE; }

				virtual void	OnFrameMove( CParticle &part, float fElapsedTime );
				bool FadeAllowed(){ return true; }
				
				CMinMax<D3DXVECTOR3>	GetRotate()									{ return m_Rotate; }
				void					SetRotate(const CMinMax<D3DXVECTOR3> &data)	{ m_Rotate = data; }

				virtual bool ProcessTokenStream(std::vector<CParticleEmitterToken>::iterator &TokenIter, 
												std::vector<CParticleEmitterToken>::iterator &EndIter);

			protected:
				CMinMax<D3DXVECTOR3> m_Rotate;
		};

		class CParticleEvent_DirSpeed : public CParticleEvent
		{
			public:
				CParticleEvent_DirSpeed(){ m_EventType = ET_DIRSPEED; }

				virtual void	OnFrameMove( CParticle &part, float fElapsedTime );
				bool FadeAllowed(){ return true; }
				
				CMinMax<float>	GetValue()								{ return m_DirSpeed; }
				void			SetValue(const CMinMax<float> &data)	{ m_DirSpeed = data; }

				virtual bool ProcessTokenStream(std::vector<CParticleEmitterToken>::iterator &TokenIter, 
												std::vector<CParticleEmitterToken>::iterator &EndIter);

			protected:
				CMinMax<float> m_DirSpeed;
		};

		class CParticleEvent_BlackHole : public CParticleEvent
		{
			public:
				CParticleEvent_BlackHole(){ m_EventType = ET_BLACKHOLE; }

				virtual void	OnFrameMove( CParticle &part, float fElapsedTime );
				bool FadeAllowed(){ return false; }
				
				CMinMax<float> GetEventTimer()					{ return m_BlackHole; }
				void SetEventTimer(const CMinMax<float> &data)	{ m_BlackHole = data; }

				virtual bool ProcessTokenStream(std::vector<CParticleEmitterToken>::iterator &TokenIter, 
												std::vector<CParticleEmitterToken>::iterator &EndIter);

			protected:
				CMinMax<float> m_BlackHole;
		};

		class CParticleEvent_Crash : public CParticleEvent
		{
			public:
				CParticleEvent_Crash(){ m_EventType = ET_CRASH; }

				virtual void	OnFrameMove( CParticle &part, float fElapsedTime );
				bool FadeAllowed(){ return true; }
				
				CMinMax<D3DXVECTOR3>	GetCrash()									{ return m_vCrash; }
				void					SetCrash(const CMinMax<D3DXVECTOR3> &data)	{ m_vCrash = data; }

				virtual bool ProcessTokenStream(std::vector<CParticleEmitterToken>::iterator &TokenIter, 
												std::vector<CParticleEmitterToken>::iterator &EndIter);

			protected:
				CMinMax<D3DXVECTOR3> m_vCrash;
		};

		class CParticleEvent_ResetCrash : public CParticleEvent
		{
			public:
				CParticleEvent_ResetCrash(){ m_EventType = ET_RESETCRASH; }

				virtual void	OnFrameMove( CParticle &part, float fElapsedTime );
				bool FadeAllowed(){ return true; }
				
				CMinMax<float>	GetResetCrash()								{ return m_ResetCrash; }
				void			SetResetCrash(const CMinMax<float> &data)	{ m_ResetCrash = data; }

				virtual bool ProcessTokenStream(std::vector<CParticleEmitterToken>::iterator &TokenIter, 
												std::vector<CParticleEmitterToken>::iterator &EndIter);

			protected:
				CMinMax<float> m_ResetCrash;
		};

		class CParticleEvent_Texture : public CParticleEvent
		{
			public:
				CParticleEvent_Texture(){ m_EventType = ET_TEXTURE; m_TextureID = 0; }

				virtual void	OnFrameMove( CParticle &part, float fElapsedTime );
				bool FadeAllowed(){ return false; }
				
				const WCHAR*	GetTextureName()						{ return m_TexName.c_str(); }
				void			SetTextureName(const WCHAR* texname)	{ m_TexName = texname; }

				void			SetTextureID( int texID ){ m_TextureID = texID; }

				virtual bool ProcessTokenStream(std::vector<CParticleEmitterToken>::iterator &TokenIter, 
												std::vector<CParticleEmitterToken>::iterator &EndIter);

			protected:
				int		m_TextureID;
				wstring	m_TexName;
		};

		class CParticleEvent_EventTimer : public CParticleEvent
		{
			public:
				CParticleEvent_EventTimer(){ m_EventType = ET_EVENTTIMER; }
				
				virtual void	OnFrameMove( CParticle &part, float fElapsedTime );
				bool FadeAllowed(){ return false; }
				
				CMinMax<float> GetEventTimer()					{ return m_EventTimer; }
				void SetEventTimer(const CMinMax<float> &data)	{ m_EventTimer = data; }

				virtual bool ProcessTokenStream(std::vector<CParticleEmitterToken>::iterator &TokenIter, 
												std::vector<CParticleEmitterToken>::iterator &EndIter);

			protected:
				CMinMax<float> m_EventTimer;
		};


#ifdef PARTICLE_STRETCH_TEST
		class CParticleEvent_Stretch : public CParticleEvent
		{
		public:
			CParticleEvent_Stretch(){ m_EventType = ET_STRETCH; }

			virtual void	OnFrameMove( CParticle &part, float fElapsedTime );
			bool FadeAllowed(){ return true; }

			CMinMax<float>	GetValue()								{ return m_StretchScale; }
			void			SetValue(const CMinMax<float> &data)	{ m_StretchScale = data; }

			virtual bool ProcessTokenStream(std::vector<CParticleEmitterToken>::iterator &TokenIter, 
				std::vector<CParticleEmitterToken>::iterator &EndIter);

		protected:
			CMinMax<float> m_StretchScale;
		};
#endif PARTICLE_STRETCH_TEST


		typedef std::list< CParticleEvent* > CParticleEventList;

		class CParticleEventSequence : public CKTDGObject
		{
			private:
				//CParticleEventSequence() { ASSERT( !"invalid" ); }
				CParticleEventSequence( CKTDGParticleSystem* pParticleSystem, CParticleEventSequence* pTempletSequence );
				virtual ~CParticleEventSequence();

			public:
				static CParticleEventSequence* CreateParticleEventSequence( CKTDGParticleSystem* pParticleSystem, CParticleEventSequence* pTempletSequence )
				{
					return new CParticleEventSequence( pParticleSystem, pTempletSequence );
				}
#ifdef EVENT_SCENE
				void SetParticleEventSequence( CParticleEventSequence* pSeq ){ m_pTempletSequence = pSeq; }
#endif EVENT_SCENE

#if defined(X2VIEWER) || defined(EFFECT_TOOL)
				// 09.12.18 태완 : 파티클 수정 기능을 위해 추가.
				void SetParticleSystem( CKTDGParticleSystem* pSystem ){ m_pParticleSystem = pSystem; }
				CParticleEventList* GetEventList(){ return &m_EventList; }
				vector<wstring>& GetTraceSeqNameList() { return m_TraceSeqNameList; }
				vector<wstring>& GetFinalSeqNameList() { return m_FinalSeqNameList; }

#endif defined(X2VIEWER) || defined(EFFECT_TOOL)
#ifdef BOOST_SINGLETON_POOL_TEST
				void* operator new(size_t s);
				void operator delete(void *p);
#endif BOOST_SINGLETON_POOL_TEST

#ifdef EFFECT_TOOL
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
				static bool IsSameMinMax( CMinMax<D3DXVECTOR3> lhs_, CMinMax<D3DXVECTOR3> rhs_ )
				{
					return true == IsSamef_( lhs_.m_Max.x, rhs_.m_Max.x ) &&
						true == IsSamef_( lhs_.m_Max.y,  rhs_.m_Max.y ) &&
						true == IsSamef_( lhs_.m_Max.z, rhs_.m_Max.z );
				}
				bool operator== ( const CParticleEventSequence& rhs_) const
				{
					if( m_iCullMode			!= rhs_.m_iCullMode				)	return false;
					if( m_Priority			!= rhs_.m_Priority				)	return false;
					if( m_bUseDXMatrix		!= rhs_.m_bUseDXMatrix			)	return false;
					if( m_bReverseY			!= rhs_.m_bReverseY			   )	return false;
					if( m_bZWriteEnable		!= rhs_.m_bZWriteEnable		   )	return false;
					if( m_bZEnable			!= rhs_.m_bZEnable			   )	return false;
					if( m_LatencyTime		!= rhs_.m_LatencyTime		   )	return false;
					if( m_bCullingCheck		!= rhs_.m_bCullingCheck		   )	return false;
					if( m_bTriggerWait		!= rhs_.m_bTriggerWait		   )	return false;
					if( m_bDelete			!= rhs_.m_bDelete			   )	return false;
					if( m_bInChain			!= rhs_.m_bInChain			   )	return false;
					if( m_bDynamicChain		!= rhs_.m_bDynamicChain		   )	return false;
					if( m_ScrewValue		!= rhs_.m_ScrewValue		   )	return false;
					if( m_ScrewRotateSpeed	!= rhs_.m_ScrewRotateSpeed	   )	return false;
					if( m_bApplyUnitScale	!= rhs_.m_bApplyUnitScale	   )	return false;
					if( m_Time				!= rhs_.m_Time				   )	return false;
					//if( m_TriggerTime		!= rhs_.m_TriggerTime		   )	return false;
					if( m_Count				!= rhs_.m_Count				   )	return false;
					//if( m_TriggerCount		!= rhs_.m_TriggerCount		   )	return false;
					//if( m_DrawCount			!= rhs_.m_DrawCount			   )	return false;
					if( m_iMaxParticleNum	!= rhs_.m_iMaxParticleNum	   )	return false;
					if( m_Trace				!= rhs_.m_Trace				   )	return false;
					if( m_iSrcBlendMode		!= rhs_.m_iSrcBlendMode		   )	return false;
					if( m_iDestBlendMode	!= rhs_.m_iDestBlendMode	   )	return false;
					if( m_bUseLand			!= rhs_.m_bUseLand			   )	return false;
					if( m_bResolutionConvert!= rhs_.m_bResolutionConvert   )	return false;
					if( m_ParticleType		!= rhs_.m_ParticleType		   )	return false;
					if( m_BillBoardType		!= rhs_.m_BillBoardType		   )	return false;
					if( m_bUseLookPoint		!= rhs_.m_bUseLookPoint		   )	return false;
					//if( false == IsSamef(m_fLandPosition, rhs_.m_fLandPosition)			   )	return false;
					if( false == IsSamef(m_fElapsedTime, rhs_.m_fElapsedTime)			   )	return false;
					if( false == IsSamef(m_fNumNewPartsExcess, rhs_.m_fNumNewPartsExcess)  )	return false;

					//if( m_LightPos					!= rhs_.m_LightPos				)  return false;
					if( m_Position					!= rhs_.m_Position				 ) return false;
					if( m_OldPosition				!= rhs_.m_OldPosition			 ) return false;
					//f( m_BlackHolePosition			!= rhs_.m_BlackHolePosition		 ) return false;
					if( m_LookPoint					!= rhs_.m_LookPoint				 ) return false;
					if( m_AddRotate					!= rhs_.m_AddRotate				 ) return false;
					//if( m_AddRotateRel				!= rhs_.m_AddRotateRel			 ) return false;
					if( m_ScaleFactor				!= rhs_.m_ScaleFactor			 ) return false;
					//if( m_vSphericalEmitRotation	!= rhs_.m_vSphericalEmitRotation ) return false;
					if( m_AxisAngleDegree			!= rhs_.m_AxisAngleDegree		 ) return false;
					if( m_vVelocity					!= rhs_.m_vVelocity				 ) return false;

					if( false == IsSameMinMax( m_Lifetime				,rhs_.m_Lifetime				)) return false;
					if( false == IsSameMinMax( m_EmitRate				,rhs_.m_EmitRate				)) return false;
					if( false == IsSameMinMax( m_vEmitRadius			,rhs_.m_vEmitRadius				)) return false;
					if( false == IsSameMinMax( m_vEmitRadiusMin			,rhs_.m_vEmitRadiusMin			)) return false;
					if( false == IsSameMinMax( m_SphericalEmitRadius	,rhs_.m_SphericalEmitRadius		)) return false;
					if( false == IsSameMinMax( m_SphericalEmitAzimuth	,rhs_.m_SphericalEmitAzimuth	)) return false;
					if( false == IsSameMinMax( m_SphericalEmitPolar		,rhs_.m_SphericalEmitPolar		)) return false;
					if( false == IsSameMinMax( m_vGravity				,rhs_.m_vGravity				)) return false;


					if( m_MeshRenderType			!= rhs_.m_MeshRenderType		 ) return false;

					if( m_DXMATRIX					!= rhs_.m_DXMATRIX				 ) return false;

					if( 0 != m_MeshName.compare( rhs_.m_MeshName ) ) return false;
					if( 0 != m_strName.compare( rhs_.m_strName					)) return false;
					if( 0 != m_ChangeTexXETName.compare( rhs_.m_ChangeTexXETName)) return false;
					if( 0 != m_MultiTexXETName.compare( rhs_.m_MultiTexXETName	)) return false;
					if( 0 != m_AniXETName.compare( rhs_.m_AniXETName			)) return false;
					if( 0 != m_AniName.compare( rhs_.m_AniName					)) return false;


					vector<wstring>::const_iterator vecLhs = m_TraceSeqNameList.begin();
					vector<wstring>::const_iterator vecRhs = rhs_.m_TraceSeqNameList.begin();
					for( ; vecLhs != m_TraceSeqNameList.end() && vecRhs != rhs_.m_TraceSeqNameList.end() ; ++vecLhs, ++vecRhs )
					{
						if( 0 != vecLhs->compare( *vecRhs )) return false;
					}

					vecLhs = m_FinalSeqNameList.begin();
					vecRhs = rhs_.m_FinalSeqNameList.begin();
					for( ; vecLhs != m_FinalSeqNameList.end() && vecRhs != rhs_.m_FinalSeqNameList.end() ; ++vecLhs, ++vecRhs )
					{
						if( 0 != vecLhs->compare( *vecRhs )) return false;
					}

					if( NULL != m_pXSkinMesh && NULL != rhs_.m_pXSkinMesh )
					{
						if( 0 != m_pXSkinMesh->GetDeviceID().compare( rhs_.m_pXSkinMesh->GetDeviceID() ) ) return false;						
					}

					if( NULL != m_pAniData )
					{
						if( 0 != m_pAniData->aniName.compare( rhs_.m_pAniData->aniName ) ) return false;						
					}

					if( NULL != m_pAniXET ) 
					{
						if( 0 != m_pAniXET->GetDeviceID().compare( rhs_.m_pAniXET->GetDeviceID() ) ) return false;	
					}

					if( NULL != m_pMultiTexXET ) 
					{
						if( 0 != m_pMultiTexXET->GetDeviceID().compare( rhs_.m_pMultiTexXET->GetDeviceID() ) ) return false;	
					}

					if( NULL != m_pChangeTexXET ) 
					{
						if( 0 != m_pChangeTexXET->GetDeviceID().compare( rhs_.m_pChangeTexXET->GetDeviceID() ) ) return false;	
					}

					if( NULL != m_pXMesh ) 
					{
						if( 0 != m_pXMesh->GetDeviceID().compare( rhs_.m_pXMesh->GetDeviceID() ) ) return false;	
					}
					/// 범위형으로 설정하고, 멤버변수에서는 확정값을 설정하는 부분때문에 더미값 추가.
					if( false == IsSameMinMax( m_TriggerCount_Origin,			rhs_.m_TriggerCount_Origin)) return false;
					if( false == IsSameMinMax( m_TriggerTime_Origin,			rhs_.m_TriggerTime_Origin)) return false;
					if( false == IsSameMinMax( m_DrawCount_Origin,				rhs_.m_DrawCount_Origin)) return false;
					if( false == IsSameMinMax( m_fLandPosition_Origin,			rhs_.m_fLandPosition_Origin)) return false;
					if( false == IsSameMinMax( m_BlackHolePosition_Origin,		rhs_.m_BlackHolePosition_Origin)) return false;
					if( false == IsSameMinMax( m_LightPos_Origin,				rhs_.m_LightPos_Origin)) return false;
					if( false == IsSameMinMax( m_AddRotateRel_Origin,			rhs_.m_AddRotateRel_Origin)) return false;
					if( false == IsSameMinMax( m_vSphericalEmitRotation_Origin, rhs_.m_vSphericalEmitRotation_Origin )) return false;

					return true;
				}
#endif //EFFECT_TOOL
				void SetAutoDie()
				{
					SetEmitRate( CMinMax<float>(0.0f, 0.0f) );
					SetTriggerTime( 0.0f );
				}
				void ClearAllParticle();

				virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );

//{{ robobeg : 2008-10-28
				//virtual HRESULT OnFrameRender();
                virtual RENDER_HINT OnFrameRender_Prepare();
                virtual void            OnFrameRender_Draw();

//}} robobeg : 2008-10-28


//#ifdef  X2OPTIMIZE_PARTICLE_SEQUENCE_DVB
				void OnFrameRender_VERTEX_PARTICLE();
				void OnFrameRender_VERTEX_PARTICLE_RHW();
//#else   X2OPTIMIZE_PARTICLE_SEQUENCE_DVB
//				void OnFrameRender( LPDIRECT3DVERTEXBUFFER9 pVB, int iVBSize );
//				void OnFrameRenderRHW( LPDIRECT3DVERTEXBUFFER9 pVB, int iVBSize );
//#endif  X2OPTIMIZE_PARTICLE_SEQUENCE_DVB


				CParticle*	CreateNewParticle( D3DXVECTOR3 m_vPartSysPos );

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
                CParticleHandle CreateNewParticleHandle( D3DXVECTOR3 m_vPartSysPos )
                {
                    CParticle* pParticle =	CreateNewParticle( m_vPartSysPos );
                    return ( pParticle != NULL ) ? pParticle->GetHandle() : INVALID_PARTICLE_HANDLE;
                }
                CParticle*  GetParticle( CParticleHandle hParticle );
                bool        IsLiveParticleHandle( CParticleHandle hParticle ) { return GetParticle( hParticle ) != NULL; }
                CParticle*  ValidateParticleHandle( CParticleHandle& hParticle )
                { 
                    CParticle*  pParticle = GetParticle( hParticle );
                    if ( pParticle == NULL )
                        hParticle = INVALID_PARTICLE_HANDLE;
                    return pParticle;
                }
				int			GetLiveParticleNum();
                template< typename FUNCTION >
                void        ApplyFunctionToParticles( FUNCTION fn );

#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
				void        ValidateParticlePointer( CParticle*& pParticle );
				int			GetLiveParticleNum(){ return m_ParticleList.size(); }
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
				
				void SetParticleType( PARTICLE_TYPE plarticleType ){ m_ParticleType = plarticleType; }
				const PARTICLE_TYPE GetParticleType() const { return m_ParticleType; }

				void SetName( wstring strName ){ m_strName = strName; }
				const WCHAR* GetName() const { return m_strName.c_str(); }

				void SetMeshName( const WCHAR* pMeshName ){ m_MeshName = pMeshName; }
				const WCHAR* GetMeshName() const { return m_MeshName.c_str(); }

				void SetChangeTexXETName( const WCHAR* pName ){ m_ChangeTexXETName = pName; }
				const WCHAR* GetChangeTexXETName() const { return m_ChangeTexXETName.c_str(); }

				void SetMultiTexXETName( const WCHAR* pName ){ m_MultiTexXETName = pName; }
				const WCHAR* GetMultiTexXETName() const { return m_MultiTexXETName.c_str(); }

				void SetAniXETName( const WCHAR* pName ){ m_AniXETName = pName; }
				const WCHAR* GetAniXETName() const { return m_AniXETName.c_str(); }

				void SetAniName( const WCHAR* pName ){ m_AniName = pName; }
				const WCHAR* GetAniName() const { return m_AniName.c_str(); }

				bool GetZWriteEnable() const { return m_bZWriteEnable; }
				void SetZWriteEnable(bool val) { m_bZWriteEnable = val; }

				void SetZEnable( bool bZEnable ){ m_bZEnable = bZEnable; }
				bool GetZEnable() const { return m_bZEnable; }

				void SetLightPos( D3DXVECTOR3 lightPos ){ m_LightPos = lightPos; }
				D3DXVECTOR3 GetLightPos() const { return m_LightPos; }

				void SetMeshRenderType( CKTDGXRenderer::RENDER_TYPE renderType ){ m_MeshRenderType = renderType; }
				CKTDGXRenderer::RENDER_TYPE GetMeshRenderType(){ return m_MeshRenderType; }

				void SetBillBoardType( CKTDGMatrix::BILLBOARD_TYPE billBoardType ){ m_BillBoardType = billBoardType; }
				CKTDGMatrix::BILLBOARD_TYPE GetBillBoardType(){ return m_BillBoardType; }

				void OpenTexture();
				void OpenMesh();

#ifdef SKINMESH_PARTICLE_TEST
				void OpenSkinMesh();
#endif SKINMESH_PARTICLE_TEST

				void SetResolutionConvert( bool bResolutionConvert ){ m_bResolutionConvert = bResolutionConvert; }
				bool GetResolutionConvert(){ return m_bResolutionConvert; }


#ifdef EQUIP_ATTACHED_PARTICLE_TRACE_TEST
				void SetPosition( const D3DXVECTOR3& pos, bool bUpdateOldPos = false, bool bUpdateParticlePos = false );
#else EQUIP_ATTACHED_PARTICLE_TRACE_TEST
//{{AFX
				void SetPosition( const D3DXVECTOR3& pos, bool bUpdateOldPos = false );
//}}AFX
#endif EQUIP_ATTACHED_PARTICLE_TRACE_TEST

				D3DXVECTOR3 GetPosition(){ return m_Position; }

				void SetParticlePosition( const D3DXVECTOR3& pos );

				void SetAxisAngle( const D3DXVECTOR3& angleDegree ){ m_AxisAngleDegree = angleDegree; }
				D3DXVECTOR3 GetAxisAngle(){ return m_AxisAngleDegree; }

				void SetBlackHolePosition( const D3DXVECTOR3& pos ){ m_BlackHolePosition = pos; }
				D3DXVECTOR3 GetBlackHolePosition(){ return m_BlackHolePosition; }

				void SetScrewValue( int screwValue ) { m_ScrewValue = screwValue;}
				int	GetScrewValue() { return m_ScrewValue; }

				void SetScrewRotateSpeed( int screwRotateSpeed ) { m_ScrewRotateSpeed = screwRotateSpeed; }
				int	GetScrewRotateSpeed() { return m_ScrewRotateSpeed; }

				void SetUseLand( bool bUseLand ){ m_bUseLand = bUseLand; }
				bool GetUseLand(){ return m_bUseLand; }

				void SetLandPosition( float fLand ){ m_fLandPosition = fLand; }
				float GetLandPosition(){ return m_fLandPosition; }

				void SetLifetime( const CMinMax<float>& lifeTime ){ m_Lifetime = lifeTime; }
				CMinMax<float> GetLifeTime(){ return m_Lifetime; }

				void SetEmitRate( const CMinMax<float>& emitRate ){ m_EmitRate = emitRate; }
				void SetEmitRate( float fMin, float fMax ){ m_EmitRate.m_Min = fMin; m_EmitRate.m_Max = fMax; }
				CMinMax<float> GetEmitRate(){ return m_EmitRate; }

				void SetEmitRadius( const CMinMax<D3DXVECTOR3>& emitRadius ){ m_vEmitRadius = emitRadius; }
				CMinMax<D3DXVECTOR3> GetEmitRadius(){ return m_vEmitRadius; }
				void SetEmitRadiusMin( const CMinMax<D3DXVECTOR3>& emitRadiusMin ){ m_vEmitRadiusMin = emitRadiusMin; }
				CMinMax<D3DXVECTOR3> GetEmitRadiusMin(){ return m_vEmitRadiusMin; }


#ifdef PARTICLE_SPHERICAL_EMIT_TEST
				void SetSphericalEmitRadius(const CMinMax<float>& val) { m_SphericalEmitRadius = val; }
				const CMinMax<float>& GetSphericalEmitRadius() const { return m_SphericalEmitRadius; }

				void SetSphericalEmitAzimuth(const CMinMax<float>& val) { m_SphericalEmitAzimuth = val; }
				const CMinMax<float>& GetSphericalEmitAzimuth() const { return m_SphericalEmitAzimuth; }
				
				void SetSphericalEmitPolar(const CMinMax<float>& val) { m_SphericalEmitPolar = val; }
				const CMinMax<float>& GetSphericalEmitPolar() const { return m_SphericalEmitPolar; }

				const D3DXVECTOR3& GetSphericalEmitRotation() const { return m_vSphericalEmitRotation; }
				void SetSphericalEmitRotation( const D3DXVECTOR3& val) { m_vSphericalEmitRotation = val; }
#endif PARTICLE_SPHERICAL_EMIT_TEST
				

				void SetGravity( const CMinMax<D3DXVECTOR3>& gravity ){ m_vGravity = gravity; }
				CMinMax<D3DXVECTOR3> GetGravity(){ return m_vGravity; }

				void SetMaxParticleNum( int maxParticleNum ){ m_iMaxParticleNum = maxParticleNum; /*m_ParticleList.ResetCapacity( m_iMaxParticleNum + 3 );*/ }
				int GetMaxParticleNum(){ return m_iMaxParticleNum; }

				void SetTrace( bool trace ){ m_Trace = trace; }
				bool GetTrace(){ return m_Trace; }

//{{ robobeg : 2008-10-28
				//void SetShow( bool show ){ m_bShow = show; }
				//bool GetShow(){ return m_bShow; }
//}} robobeg : 2008-10-28

				void SetSrcBlendMode( int srcBlend ){ m_iSrcBlendMode = srcBlend; }
				int GetSrcBlendMode(){ return m_iSrcBlendMode; }

				void SetDestBlendMode( int destBlend ){ m_iDestBlendMode = destBlend; }
				int GetDestBlendMode(){ return m_iDestBlendMode; }

				void SetCullMode( int cullMode ){ m_iCullMode = cullMode; }
				int GetCullMode(){ return m_iCullMode; }

				void UseLookPoint( bool bLookPoint ){ m_bUseLookPoint = bLookPoint; }
				void SetLookPoint( D3DXVECTOR3 lookPoint ){ m_LookPoint = lookPoint; }

				const D3DXVECTOR3& GetScaleFactor() const { return m_ScaleFactor; }
				void SetScaleFactor(const D3DXVECTOR3& val) { m_ScaleFactor = val; }
				
//#ifdef GIANT_UNIT_GIANT_EFFECT_TEST
//				const D3DXVECTOR3& GetScaleByUnit() const 
//				{ 
//					ASSERT( 0.f != m_ScaleFactorByUnit.x ); 
//					ASSERT( 0.f != m_ScaleFactorByUnit.y ); 
//					ASSERT( 0.f != m_ScaleFactorByUnit.z ); 
//					return m_ScaleFactorByUnit; 
//				}
//				void SetScaleByUnit(const D3DXVECTOR3& val) 
//				{
//					ASSERT( 0.f != val.x ); 
//					ASSERT( 0.f != val.y ); 
//					ASSERT( 0.f != val.z ); 
//					m_ScaleFactorByUnit = val; 
//				}
//#endif GIANT_UNIT_GIANT_EFFECT_TEST

				void SetAddRotate( D3DXVECTOR3 addRotate ){ m_AddRotate = addRotate; }
				//{{kimhc // 2010.12.21 // Get Eancapsulation 추가
				const D3DXVECTOR3& GetAddRotate() const { return m_AddRotateRel; }
				//}}kimhc // 2010.12.21 // Get Eancapsulation 추가
				void SetAddRotateRel( D3DXVECTOR3 addRotateRel ){ m_AddRotateRel = addRotateRel; }
				D3DXVECTOR3 GetAddRotateRel(){ return m_AddRotateRel; }

				void SetCameraZoomDistance( float fCameraZoom );
				void AddTraceSeqName( const WCHAR* pName )
				{  
					m_TraceSeqNameList.push_back( pName );
				}
				void AddFinalSeqName( const WCHAR* pName )
				{  
					m_FinalSeqNameList.push_back( pName );
				}

				float	GetTime(){ return m_Time; }
				void	SetTime( float fTime ){ m_Time = fTime; }
				float	GetTriggerTime(){ return m_TriggerTime; }
				void	SetTriggerTime( float fTime ){ m_TriggerTime = fTime; m_Time = 0.0f; }
				int		GetCount(){ return m_Count; }
				void	SetCount( int iCount ){ m_Count = iCount; }
				int		GetTriggerCount(){ return m_TriggerCount; }
				void	SetTriggerCount( int cnt ){ m_TriggerCount = cnt; m_Count = 0; }

				int		GetDrawCount(){ return m_DrawCount; }
				void	SetDrawCount( int drawCount ){ m_DrawCount = drawCount; }

				void	SetCullCheck( bool cull ){ m_bCullingCheck = cull; }
				bool	GetCullCheck() { return m_bCullingCheck; }
#ifdef  X2OPTIMIZE_CULLING_PARTICLE
                void    SetRenderCullCheck( bool cull ) { m_bRenderCullCheck = cull; }
                bool    GetRenderCullCheck() const { return m_bRenderCullCheck; }
#endif  X2OPTIMIZE_CULLING_PARTICLE
				void	SetLatency( float latency ){ m_LatencyTime = latency; }
				void	SetTriggerWait( bool bTriggerWait ){ m_bTriggerWait = bTriggerWait; }
				bool	GetTriggerWait(){ return m_bTriggerWait; }
				bool	GetDelete(){ return m_bDelete; }
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX
                void    _SetDelete() { m_bDelete = true; }
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX
				void	InChain();
				void	OutChain();
				void	SetDynamicChain( bool bDynamicChain ){ m_bDynamicChain = bDynamicChain; }
				void	ChangeTex( const WCHAR* pTexName );
				void	ChangeTexForce( const WCHAR* pTexName );
				void	ChangeTexForce_LUA( const char* pTexName ) 
				{
					if( NULL == pTexName )
						return;
					wstring wstrTexName = L"";
					ConvertUtf8ToWCHAR( wstrTexName, pTexName );
					ChangeTexForce( wstrTexName.c_str() );
				}
#ifdef ELSWORD_SHEATH_KNIGHT
				void	ChangeSizeForce( const D3DXVECTOR3& vSize_ );
				void	ChangeColorForce( const D3DXCOLOR& vColor_ );
#endif ELSWORD_SHEATH_KNIGHT


				CKTDGParticleSystem* GetParticleSystem(){ return m_pParticleSystem; }
				CParticleEventSequence* GetTemplet(){ return m_pTempletSequence; }

				D3DXVECTOR3 GetVelocity() const { return m_vVelocity; }
				void SetVelocity(D3DXVECTOR3 val) { m_vVelocity = val; }


				
				CKTDGParticleSystem::CParticleEventSequenceHandle GetHandle() const { return m_Handle; }
				void SetHandle(CKTDGParticleSystem::CParticleEventSequenceHandle val) { m_Handle = val; }

#ifdef PARTICLE_NOTAPPLY_UNITSCALE
				void SetApplyUnitScale(bool bVal) { m_bApplyUnitScale = bVal; }
				bool GetApplyUnitScale() { return m_bApplyUnitScale; }
#endif

#ifdef ADD_ALPHATESTENABLE
				bool GetAlphTest() const { return m_bAlphaTest; }
				void SetAlphTest(bool val) { m_bAlphaTest = val; }
#endif

#ifdef STOP_UNIT_STOP_EFFECT_TEST

				void SetStopTime( float fTime ) { m_StopTime.m_fTimeLeft = fTime; }
				float GetStopTime() { return m_StopTime.m_fTimeLeft; }

#endif STOP_UNIT_STOP_EFFECT_TEST

#ifdef NEW_HENIR_TEST
				void SetUseDXMatrix( const D3DXMATRIX& dxMATRIX )
				{
					m_bUseDXMatrix	= true;					
					m_DXMATRIX		= dxMATRIX;
				}
#endif NEW_HENIR_TEST

#ifdef ARA_CHANGE_CLASS_FIRST
				void SetReverseY( bool bVal_ ) { m_bReverseY = bVal_; }
#endif

#ifdef EFFECT_TOOL
				/// 스크립트에서는 범위형으로 설정하고, 멤버변수에서는 확정값을 설정하는 부분때문에 더미값 추가.
				CMinMax<float> GetTriggerCount_Origin() const { return m_TriggerCount_Origin; }
				void SetTriggerCount_Origin( const CMinMax<float>& val) { m_TriggerCount_Origin = val; }

				CMinMax<float> GetTriggerTime_Origin() const { return m_TriggerTime_Origin; }
				void SetTriggerTime_Origin( const CMinMax<float>& val) { m_TriggerTime_Origin = val; }

				CMinMax<float> GetDrawCount_Origin() const { return m_DrawCount_Origin; }
				void SetDrawCount_Origin( const CMinMax<float>& val) { m_DrawCount_Origin = val; }

				CMinMax<float> GetLandPosition_Origin() const { return m_fLandPosition_Origin; }
				void SetLandPosition_Origin(const CMinMax<float>& val) { m_fLandPosition_Origin = val; }
				
				const CMinMax<D3DXVECTOR3>& GetBlackHolePosition_Origin() const { return m_BlackHolePosition_Origin; }
				void SetBlackHolePosition_Origin( const CMinMax<D3DXVECTOR3>& val) { m_BlackHolePosition_Origin = val; }

				const CMinMax<D3DXVECTOR3>& GetLightPos_Origin() const { return m_LightPos_Origin; }
				void SetLightPos_Origin( const CMinMax<D3DXVECTOR3>& val) { m_LightPos_Origin = val; }

				const CMinMax<D3DXVECTOR3>& GetAddRotateRel_Origin() const { return m_AddRotateRel_Origin; }
				void SetAddRotateRel_Origin( const CMinMax<D3DXVECTOR3>& val) { m_AddRotateRel_Origin = val; }

				const CMinMax<D3DXVECTOR3>& GetvSphericalEmitRotation_Origin() const { return m_vSphericalEmitRotation_Origin; }
				void SetvSphericalEmitRotation_Origin( const CMinMax<D3DXVECTOR3>& val) { m_vSphericalEmitRotation_Origin = val; }
#endif //EFFECT_TOOL

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
                void    SetPerFrameSimulation( bool bSet )  { m_bPerFrameSimulation = bSet; }
                bool    GetPerFrameSimulation() const       { return m_bPerFrameSimulation; }
#else   X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
                void    SetPerFrameSimulation( bool )       {}
                bool    GetPerFrameSimulation() const       { return true; }
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
                CParticle*                          GetFrontParticle();
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE


			public:
				
#ifndef X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
				CParticleList						m_ParticleList;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
				CParticleEventList					m_EventList;
				map<int,CKTDXDeviceTexture*>		m_TextureMap;
				CParticleEventSequence* GetCloneSequence();

            private:

				void RunEvent( CParticle &part );
                void OnFrameRenderMesh_Draw();

#ifdef SKINMESH_PARTICLE_TEST

                void OnFrameRenderSkinMesh_Draw();

#endif SKINMESH_PARTICLE_TEST




				CKTDGParticleSystem*			m_pParticleSystem;
				CParticleEventSequence*			m_pTempletSequence;



				int								m_Priority;
				PARTICLE_TYPE					m_ParticleType;
				wstring							m_strName;
				vector<CKTDXDeviceXMesh*>		m_MeshList;
				CKTDGMatrix*					m_pMathMatrix;
				CKTDGMatrix::BILLBOARD_TYPE		m_BillBoardType;				

#ifdef NEW_HENIR_TEST
				bool							m_bUseDXMatrix;
				D3DXMATRIX						m_DXMATRIX;
#endif NEW_HENIR_TEST

#ifdef ARA_CHANGE_CLASS_FIRST
				bool							m_bReverseY;
#endif

				bool							m_bZWriteEnable;
				bool							m_bZEnable;
				D3DXVECTOR3						m_LightPos;
				CKTDGXRenderer::RENDER_TYPE		m_MeshRenderType;

				wstring							m_MeshName;
				wstring							m_ChangeTexXETName;
				wstring							m_MultiTexXETName;
				wstring							m_AniXETName;
				wstring							m_AniName;

				CKTDXDeviceXMesh*				m_pXMesh;
				CKTDXDeviceXET*					m_pChangeTexXET;
				CKTDXDeviceXET*					m_pMultiTexXET;
				CKTDXDeviceXET*					m_pAniXET;
				const CKTDXDeviceXET::AniData*	m_pAniData;


#ifdef SKINMESH_PARTICLE_TEST
				CKTDXDeviceXSkinMesh*			m_pXSkinMesh;
#endif SKINMESH_PARTICLE_TEST


				bool							m_bResolutionConvert;
				D3DXVECTOR3						m_Position;
				D3DXVECTOR3						m_OldPosition;
				D3DXVECTOR3						m_BlackHolePosition;
				bool							m_bUseLand;
				float							m_fLandPosition;
				CMinMax<float>					m_Lifetime;
				CMinMax<float>					m_EmitRate;
				CMinMax<D3DXVECTOR3>			m_vEmitRadius;
				CMinMax<D3DXVECTOR3>			m_vEmitRadiusMin;

#ifdef PARTICLE_SPHERICAL_EMIT_TEST
				CMinMax<float>					m_SphericalEmitRadius;
				CMinMax<float>					m_SphericalEmitAzimuth;
				CMinMax<float>					m_SphericalEmitPolar;
				D3DXVECTOR3						m_vSphericalEmitRotation;
#endif PARTICLE_SPHERICAL_EMIT_TEST

				CMinMax<D3DXVECTOR3>			m_vGravity;
				int								m_iMaxParticleNum;
				bool							m_Trace;
//{{ robobeg : 2008-10-28
				//bool							m_bShow;
//}} robobeg : 2008-10-28

				int								m_iSrcBlendMode;
				int								m_iDestBlendMode;
				float							m_fNumNewPartsExcess;

				int								m_iCullMode;	// 09.05.06 태완

				float							m_Time;
				float							m_fElapsedTime;
				float							m_TriggerTime;
				int								m_Count;
				int								m_TriggerCount;

				int								m_DrawCount;

				bool							m_bUseLookPoint;
				D3DXVECTOR3						m_LookPoint;

				D3DXVECTOR3						m_AddRotate;
				D3DXVECTOR3						m_AddRotateRel;
				D3DXVECTOR3						m_ScaleFactor;

//#ifdef GIANT_UNIT_GIANT_EFFECT_TEST
//				D3DXVECTOR3						m_ScaleFactorByUnit;
//#endif GIANT_UNIT_GIANT_EFFECT_TEST


				D3DXVECTOR3						m_AxisAngleDegree;

				float							m_LatencyTime;
				bool							m_bCullingCheck;	
#ifdef  X2OPTIMIZE_CULLING_PARTICLE
                bool							m_bRenderCullCheck;	
#endif  X2OPTIMIZE_CULLING_PARTICLE
				bool							m_bTriggerWait;
				bool							m_bDelete;
				bool							m_bInChain;
				bool							m_bDynamicChain;

				int								m_ScrewValue;
				int								m_ScrewRotateSpeed;

				D3DXVECTOR3						m_vVelocity;

				CParticleEventSequenceHandle	m_Handle;

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
                int                             m_iParticleList;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

				vector<wstring>					m_TraceSeqNameList;
				vector<wstring>					m_FinalSeqNameList;


#ifdef STOP_UNIT_STOP_EFFECT_TEST
				TimeLimited m_StopTime;
#endif STOP_UNIT_STOP_EFFECT_TEST


#ifdef PARTICLE_NOTAPPLY_UNITSCALE
				bool						m_bApplyUnitScale;
#endif

#ifdef ADD_ALPHATESTENABLE
				bool	m_bAlphaTest;
#endif


#ifdef EFFECT_TOOL
	/// 범위형으로 설정하고, 멤버변수에서는 확정값을 설정하는 부분때문에 더미값 추가.
				CMinMax<float> m_TriggerCount_Origin;				//m_TriggerCount = -1;
				CMinMax<float> m_TriggerTime_Origin;				//m_TriggerTime =1.f;
				CMinMax<float> m_DrawCount_Origin;					//m_DrawCount =1;
				CMinMax<float> m_fLandPosition_Origin;				//m_fLandPosition = 0.f

				CMinMax<D3DXVECTOR3> m_BlackHolePosition_Origin;			//m_BlackHolePosition
				CMinMax<D3DXVECTOR3> m_LightPos_Origin;					//m_LightPos
				CMinMax<D3DXVECTOR3> m_AddRotateRel_Origin;				//m_AddRotateRel
				CMinMax<D3DXVECTOR3> m_vSphericalEmitRotation_Origin;	//m_vSphericalEmitRotation	
#endif //EFFECT_TOOL
				
#ifdef X2OPTIMIZE_CULLING_PARTICLE
			private:
				//bool _CullingTest( const CKTDGFrustum& kFrustum, const D3DXVECTOR3& vCenter, float fRadius )
				//{
				//	if( kFrustum.CheckSphere( vCenter, fRadius ) )
				//		return false;
				//	return true;
				//}

				//void _DecideWorldMatrix_Normal( CKTDGCamera& kCamera, D3DXVECTOR3& vCenter, float fRadius, bool& bIdentityWorldMatrix );
				//void _DecideWorldMatrix_Mesh( D3DXMATRIX& kMatrix, CKTDGCamera& kCamera, D3DXVECTOR3& vCenter, float fRadius );
#endif//X2OPTIMIZE_CULLING_PARTICLE

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
			private:
				float                       m_fAccumElapsedTime;
                bool                        m_bPerFrameSimulation;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION

#ifdef X2OPTIMIZE_RENDER_BOUNDING_SPHERE_TEST
			private:
				void _RenderBoundingSphere( const D3DXVECTOR3& vCenter, float fRadius );

				CKTDXDeviceXMesh* m_pkBoundingSphere;
#endif//X2OPTIMIZE_RENDER_BOUNDING_SPHERE_TEST
		};


		struct BLENDINGMODE
		{
			char	name[25];
			int		mode;
		};

		struct CULLMODE
		{
			char	name[25];
			int		mode;
		};

		class CParticleEmitterTokenizer  
		{
			public:
				void Tokenize(const char *str, const int iLength = -1 );

				static bool ProcessAlphaBlendMode(int &alphablendmode, 
					std::vector<CParticleEmitterToken>::iterator &TokenIter,
					std::vector<CParticleEmitterToken>::iterator &EndIter);

				// 09.05.06 태완
				static bool ProcessCullMode(int &cullmode, 
					std::vector<CParticleEmitterToken>::iterator &TokenIter,
					std::vector<CParticleEmitterToken>::iterator &EndIter);

				static bool ProcessParticleType(PARTICLE_TYPE &particleType, 
					std::vector<CParticleEmitterToken>::iterator &TokenIter,
					std::vector<CParticleEmitterToken>::iterator &EndIter);

				static bool ProcessMeshRenderType(CKTDGXRenderer::RENDER_TYPE &renderType, 
					std::vector<CParticleEmitterToken>::iterator &TokenIter,
					std::vector<CParticleEmitterToken>::iterator &EndIter);

				static bool ProcessBillboardType(CKTDGMatrix::BILLBOARD_TYPE &billboardType, 
					std::vector<CParticleEmitterToken>::iterator &TokenIter,
					std::vector<CParticleEmitterToken>::iterator &EndIter);

				static bool ProcessBool(bool &bValue, 
					std::vector<CParticleEmitterToken>::iterator &TokenIter,
					std::vector<CParticleEmitterToken>::iterator &EndIter);

				static bool ProcessString(wstring &str, 
					std::vector<CParticleEmitterToken>::iterator &TokenIter,
					std::vector<CParticleEmitterToken>::iterator &EndIter);

				static bool ProcessColor(CMinMax<D3DXCOLOR> &c, 
					std::vector<CParticleEmitterToken>::iterator &TokenIter,
					std::vector<CParticleEmitterToken>::iterator &EndIter);

				static bool ProcessTime(CMinMax<float> &TimeRange, bool &IsFade, 
					const float InitialTime, const float FinalTime,
					std::vector<CParticleEmitterToken>::iterator &TokenIter, 
					std::vector<CParticleEmitterToken>::iterator &EndIter);

				static bool ProcessVector(CMinMax<D3DXVECTOR3> &v, 
					std::vector<CParticleEmitterToken>::iterator &TokenIter, 
					std::vector<CParticleEmitterToken>::iterator &EndIter);

				static bool ProcessVector2(CMinMax<D3DXVECTOR2> &v, 
					std::vector<CParticleEmitterToken>::iterator &TokenIter, 
					std::vector<CParticleEmitterToken>::iterator &EndIter);

				static bool ProcessNumber(CMinMax<float> &number, 
					std::vector<CParticleEmitterToken>::iterator &TokenIter,
					std::vector<CParticleEmitterToken>::iterator &EndIter);

				static void NextToken(std::vector<CParticleEmitterToken>::iterator &TokenIter, 
					std::vector<CParticleEmitterToken>::iterator &EndIter) 
				{
					if (TokenIter == EndIter) throw("Unexpected end-of-file.");
					TokenIter++;
				}

				static const int NUMBLENDINGMODES;
				static const BLENDINGMODE m_BlendingModes[15];

				static const int NUMCULLMODES;
				static const CULLMODE m_CullModes[4];


				std::vector<CParticleEmitterToken> m_TokenVector;

			private:
				void AddToken(CParticleEmitterToken token);
				void DetermineTokenType(CParticleEmitterToken &token);
		};

	public:
		CKTDGParticleSystem( LPDIRECT3DDEVICE9 pd3dDevice
#ifdef  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
            , unsigned char ucSystemID = 0
#endif  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
            );
		~CKTDGParticleSystem();

		HRESULT OnFrameMove( double fTime, float fElapsedTime );

		virtual HRESULT OnResetDevice();
		virtual HRESULT OnLostDevice();

		void OpenScriptFile( const WCHAR* pFilename );
		bool Compile( const char *pScript, const int iLength = -1 );
        bool DoMemoryNotEncrypt( const char* pBuffer, long nSize ) { return Compile( pBuffer, (int) nSize ); }

		const map<wstring, CParticleEventSequence*>& GetTempletSequences() { return m_TempletSequences; }
		const CParticleEventSequence* GetTempletSequencesByName( const wstring wstrName );

#ifdef EFFECT_TOOL
		bool					GetVecTempletSequences(const WCHAR* strFileName_, OUT vector<CParticleEventSequence*>& vecParticleTemplet_ ) ;
		bool					EraseParticleSequnce( UINT uiParticleIndex_, const WCHAR* wcFileName_ );
		wstring					GetCurrentFileName() { return m_strCurrentFileName ; }
		CParticleEventSequence* CreateEventSequence( const WCHAR* strParticleName_, const WCHAR* strParticleFileName_);

		void					MoveParticleTempletFile( const WCHAR* pSrcFileName_, const WCHAR* pDestFileName_ );
#endif //EFFECT_TOOL

#ifdef X2VIEWER
		// 09.12.18 fatswallow : Particle Edit 기능을 위해 추가.
		void AddTempletSequence( wstring name, CParticleEventSequence* pNewSeq );
		void DeleteTempletSequence( wstring name );
#endif


		CParticleEventSequenceHandle CreateSequenceHandle( CKTDGObject* pKTDGObject, const WCHAR* pSequenceName, 
												const D3DXVECTOR3& pos,
												float minEmitrate = -1, float maxEmitrate = -1, int drawCount = -1, 
												int triggerCount = -1, float triggerTime = -1.0f,
												bool bCullCheck = true, float latency = 0.0f, bool bNextFrame = false )
		{
			return CreateSequenceHandle( pKTDGObject, pSequenceName, pos.x, pos.y, pos.z, minEmitrate, maxEmitrate, 
									drawCount, triggerCount, triggerTime, bCullCheck, latency, bNextFrame );
		}

		CParticleEventSequenceHandle CreateSequenceHandle( CKTDGObject* pKTDGObject, const WCHAR* pSequenceName, 
												float x, float y, float z, 
												float minEmitrate = -1, float maxEmitrate = -1, int drawCount = -1, 
												int triggerCount = -1, float triggerTime = -1.0f,
												bool bCullCheck = true, float latency = 0.0f, bool bNextFrame = false )
		{

			CParticleEventSequence* pSeq = CreateSequence( pKTDGObject, pSequenceName, x, y, z, minEmitrate, maxEmitrate, drawCount, triggerCount,
				triggerTime, bCullCheck, latency, bNextFrame );
			
			if( NULL != pSeq )
				return pSeq->GetHandle();
			else
				return INVALID_PARTICLE_SEQUENCE_HANDLE;
		}

		CParticleEventSequence* CreateSequence_LUA( const char* pSequenceName, D3DXVECTOR3 pos, D3DXVECTOR2 emitRate, D3DXVECTOR2 trigger )
		{
			wstring wSequenceName;
			ConvertUtf8ToWCHAR( wSequenceName, pSequenceName );

#pragma DMLEE_NOTE( "일단은 스크립트에서 사용하는 부분에서는 NULL 로 입력, 나중에 몬스터 스크립트 전체 수정하면서 코드 수정해야함" )
			return CreateSequence( NULL, wSequenceName.c_str(), 
				pos.x, pos.y, pos.z, emitRate.x, emitRate.y, 
				-1, (int)trigger.x, trigger.y );
		}

		CParticleEventSequence* GameUnitCreateSequence_LUA( CKTDGObject* pKTDGObject_, const char* pSequenceName, D3DXVECTOR3 pos, D3DXVECTOR2 emitRate, D3DXVECTOR2 trigger )
		{
			wstring wSequenceName;
			ConvertUtf8ToWCHAR( wSequenceName, pSequenceName );

#pragma DMLEE_NOTE( "일단은 스크립트에서 사용하는 부분에서는 NULL 로 입력, 나중에 몬스터 스크립트 전체 수정하면서 코드 수정해야함" )
			return CreateSequence( pKTDGObject_, wSequenceName.c_str(), 
				pos.x, pos.y, pos.z, emitRate.x, emitRate.y, 
				-1, (int)trigger.x, trigger.y );
		}

		CParticleEventSequence* CreateSequence( CKTDGObject* pKTDGObject, const WCHAR* pSequenceName, 
			const D3DXVECTOR3& pos,
			float minEmitrate = -1, float maxEmitrate = -1, int drawCount = -1, 
			int triggerCount = -1, float triggerTime = -1.0f,
			bool bCullCheck = true, float latency = 0.0f, bool bNextFrame = false )
		{
			return CreateSequence( pKTDGObject, pSequenceName, pos.x, pos.y, pos.z, minEmitrate, maxEmitrate, 
				drawCount, triggerCount, triggerTime, bCullCheck, latency, bNextFrame );
		}

		CParticleEventSequence* CreateSequence( CKTDGObject* pKTDGObject, const WCHAR* pSequenceName, 
			float x, float y, float z, 
			float minEmitrate = -1, float maxEmitrate = -1, int drawCount = -1, 
			int triggerCount = -1, float triggerTime = -1.0f,
			bool bCullCheck = true, float latency = 0.0f, bool bNextFrame = false );

#ifdef EVENT_SCENE
		//{{ oasis907 : 김상윤 [2010.6.10] // 
		// txt에서 파싱된 Templet들이 등록된 TempletMap과 상관없이 동작할 수 있게금 하였습니다.
		// 대신 CParticleEventSequence의 내용은 다른 곳에서 수동으로 입력해야합니다. 
		CKTDGParticleSystem::CParticleEventSequenceHandle CreateInstanceNonTemplet( CParticleEventSequence* pSeq, D3DXVECTOR3 pos, D3DXVECTOR2 emitRate, D3DXVECTOR2 trigger );

#endif EVENT_SCENE
		
		void DestroyAllInstances();
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        void DestroyInstance( CParticleEventSequence*& pSeq );
        void DestroyInstance_LUA( CParticleEventSequence* pSeq );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		void DestroyInstance( CParticleEventSequence* pSeq );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		bool IsLiveInstance( CParticleEventSequence* pSeq );
		void DestroyInstanceHandle( CParticleEventSequenceHandle& hSeq );
		bool IsLiveInstanceHandle( CParticleEventSequenceHandle hSeq );
		
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

        CParticleEventSequence* GetInstanceSequence( CParticleEventSequenceHandle hSeq, bool bLiveOnly = true );
        CParticleEventSequence* ValidateInstanceHandle( CParticleEventSequenceHandle& hSeq, bool bLiveOnly = true )
        {
            CParticleEventSequence* pSeq = GetInstanceSequence( hSeq, bLiveOnly );
            if ( pSeq == NULL )
                hSeq = INVALID_PARTICLE_SEQUENCE_HANDLE;
            return  pSeq;
        }

        CParticle*  GetParticle( CParticleHandle hParticle );
        bool        IsLiveParticleHandle( CParticleHandle hParticle ) { return GetParticle( hParticle ) != NULL; }
        CParticle*  ValidateParticleHandle( CParticleHandle& hParticle ) 
        { 
            CParticle*  pParticle = GetParticle( hParticle );
            if ( pParticle == NULL )
                hParticle = INVALID_PARTICLE_HANDLE;
            return pParticle;
        }

#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		CParticleEventSequence* GetInstanceSequence( CParticleEventSequenceHandle hSeq );

		//{{ robobeg : 2010-11-17
		void    ValidateParticlePointer( CParticleEventSequenceHandle hSeq, CParticle*& pParticle )
		{
			if ( pParticle == NULL || hSeq == INVALID_PARTICLE_SEQUENCE_HANDLE )
			{
				pParticle = NULL;
				return;
			}
			CParticleEventSequence* pSeq = GetInstanceSequence( hSeq );
			if ( pSeq == NULL )
			{
				pParticle = NULL;
			}
			else
			{
				pSeq->ValidateParticlePointer( pParticle );
			}//if.. else..
		}
		//}} robobeg : 2010-11-17

#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE


#ifdef  X2OPTIMIZE_CULLING_PARTICLE
        static void         EnableParticleCulling( bool bEnable )   { ms_bParticleCullingEnabled = bEnable; }
#endif  X2OPTIMIZE_CULLING_PARTICLE

//#ifdef  X2OPTIMIZE_PARTICLE_SEQUENCE_DVB

        std::vector<VERTEX_PARTICLE>&
                                GetVecVERTEX_PARTICLE() { return m_vecVERTEX_PARTICLE; }
        std::vector<VERTEX_PARTICLE_RHW>&
                                GetVecVERTEX_PARTICLE_RHW() { return m_vecVERTEX_PARTICLE_RHW; }


//#else   X2OPTIMIZE_PARTICLE_SEQUENCE_DVB
//
//		LPDIRECT3DVERTEXBUFFER9	GetVB(){ return m_vbParticles; }
//		LPDIRECT3DVERTEXBUFFER9	GetVBRHW(){ return m_vbParticlesRHW; }
//		int						GetVBSize(){ return	m_iVBSize; }
//
//		bool					GetUseDynamicTexture() { return m_bUseDynamicTexture; }
//
//		int						GetOffsetSize() { return m_Offset; }
//		void					SetOffsetSize( int offset ) { m_Offset = offset; }
//
//		int						GetOffsetRHWSize() { return m_OffsetRHW; }
//		void					SetOffsetRHWSize( int offset ) { m_OffsetRHW = offset; }
//
//		int						GetBatchSize() { return m_BatchSize; }
//
//#endif  X2OPTIMIZE_PARTICLE_SEQUENCE_DVB

		static CParticleEvent* CKTDGParticleSystem::EventFactory( std::string EventName );

		void SetEnable( bool bEnable ){ m_bEnable = bEnable; }

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        UINT EstimateParticleSequenceNum();
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		CKTDGParticleSystem::CParticleEventSequence* GetParticleSequence( int index ) { return m_InstanceSequences[index]; }
		UINT GetParticleSequenceNum() { return m_InstanceSequences.size(); } 
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

//#ifdef	X2OPTIMIZE_REFACTORING_RESOURCE_BACKGROUND_LOAD
		bool	AppendToDeviceList( CKTDXDeviceDataList& listInOut_, const WCHAR* pSequenceName, CKTDXDeviceManager::EPriority ePriority = CKTDXDeviceManager::PRIORITY_HIGH );
		bool	AppendToDeviceList( CKTDXDeviceDataList& listInOut_, const std::wstring& wstrSequenceName, CKTDXDeviceManager::EPriority ePriority = CKTDXDeviceManager::PRIORITY_HIGH );
//#endif	X2OPTIMIZE_REFACTORING_RESOURCE_BACKGROUND_LOAD
#ifdef NOT_RENDER_EFFECT_MADE_BY_GAME_UNIT
		bool GetRenderEffectMadeByGameUnit() const { return m_bRenderEffectMadeByGameUnit; }
		void SetRenderEffectMadeByGameUnit(bool bVal_) { m_bRenderEffectMadeByGameUnit = bVal_; }
#endif //NOT_RENDER_EFFECT_MADE_BY_GAME_UNIT

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        template< typename FUNCTION >
        void                            ApplyFunctionToLiveInstanceSequences( FUNCTION fn );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

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



	protected:
		bool ProcessParticleSystemBlock(
					std::vector<CParticleEmitterToken>::iterator &TokenIter,
					std::vector<CParticleEmitterToken>::iterator &EndIter);

		bool ProcessEventSequenceBlock(CParticleEventSequence &seq,
					std::vector<CParticleEmitterToken>::iterator &TokenIter,
					std::vector<CParticleEmitterToken>::iterator &EndIter);
		
//#ifdef  X2OPTIMIZE_PARTICLE_SEQUENCE_DVB
        std::vector<VERTEX_PARTICLE>    m_vecVERTEX_PARTICLE;
        std::vector<VERTEX_PARTICLE_RHW>
                                        m_vecVERTEX_PARTICLE_RHW;
//#else   X2OPTIMIZE_PARTICLE_SEQUENCE_DVB
//		LPDIRECT3DVERTEXBUFFER9			m_vbParticles;
//		LPDIRECT3DVERTEXBUFFER9			m_vbParticlesRHW;
//
//		bool							m_bUseDynamicTexture;
//
//		int								m_iVBSize;
//		DWORD							m_Offset;    // offset in vb to lock   
//		DWORD							m_OffsetRHW;
//		DWORD							m_BatchSize; // number of vertices to lock starting at _vbOffset
//#endif  X2OPTIMIZE_PARTICLE_SEQUENCE_DVB

		string							m_strName;
//{{ robobeg : 2008-10-13
		CKTDGStateManager::KStateID     m_RenderStateID;
		CKTDGStateManager::KStateID     m_RHWRenderStateID;
		CKTDGStateManager::KStateID     m_MeshRenderStateID;
		CKTDGStateManager::KStateID     m_SkinMeshRenderStateID;
//}} robobeg : 2008-10-13
		int								m_Priority;
		bool							m_bEnable;

		map<wstring,CParticleEventSequence*>	m_TempletSequences;
#ifdef EFFECT_TOOL
		//이펙트툴에서 정렬하지 않고 기존 파티클 순서를 유지하기 위해 사용.

		//map<파티클 파일명, 템플릿 >
		map< wstring, vector<CParticleEventSequence*> >	m_mapTempletSequences;
		wstring									m_strCurrentFileName;
#endif //EFFECT_TOOL


#ifdef X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        enum    ESequenceListType
        {
            SEQUENCELIST_LIVE = 0,
            SEQUENCELIST_READY = 1,
            SEQUENCELIST_FREE = 2,
            SEQUENCELIST_NUM
        };
        struct  KSequenceHandleInfo
        {
            CParticleEventSequence*         m_pSequence;
            WORD                            m_wStamp;
            ESequenceListType               m_eType;
            KSequenceHandleInfo()
                : m_pSequence( NULL )
                , m_wStamp(0)
                , m_eType( SEQUENCELIST_FREE )
            {
            }
        };
        typedef kog::indexed_list<KSequenceHandleInfo> KSequenceHandleList;
        KSequenceHandleList                     m_coParticleSequence;

        enum    EParticleListType
        {
            PARTICLELIST_FREE = 0,
            PARTICLELIST_NUM
        };
        struct  KParticleHandleInfo
        {
            CParticle*                      m_pParticle;
            WORD                            m_wStamp;

            KParticleHandleInfo()
                : m_pParticle( NULL )
                , m_wStamp(0) {}
        };
        typedef kog::indexed_list<KParticleHandleInfo> KParticleHandleList;
        KParticleHandleList                     m_coParticleList;
public:
        KParticleHandleList&                    _AccessParticleList()   { return m_coParticleList; }
private:

#else//X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		vector<CParticleEventSequence*>			m_InstanceSequencesReady;
		vector<CParticleEventSequence*>			m_InstanceSequences;
		map< CParticleEventSequenceHandle, CParticleEventSequence* >	m_mapInstanceSequences;
		static CParticleEventSequenceHandle		s_iNextParticleEventSequenceHandle;
		CRITICAL_SECTION						m_csParticleLock;
#endif//X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX
        bool                                    m_bInCriticalLoop;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX

		string									m_strLastError;

#ifdef NOT_RENDER_EFFECT_MADE_BY_GAME_UNIT
		bool									m_bRenderEffectMadeByGameUnit;
#endif //NOT_RENDER_EFFECT_MADE_BY_GAME_UNIT

#ifdef  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
        unsigned char                           m_ucSystemID;
#endif  X2OPTIMIZE_HANDLE_VALIDITY_CHECK

#ifdef  X2OPTIMIZE_CULLING_PARTICLE
        static bool                             ms_bParticleCullingEnabled;
#endif  X2OPTIMIZE_CULLING_PARTICLE

};


#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

template< typename FUNCTION >
void        CKTDGParticleSystem::ApplyFunctionToLiveInstanceSequences( FUNCTION fn )
{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX
    bool bOldInCriticalLoop = m_bInCriticalLoop;
    m_bInCriticalLoop = true;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX

    KSequenceHandleList::iterator iterEnd = m_coParticleSequence.end(SEQUENCELIST_LIVE);
    for( KSequenceHandleList::iterator iter = m_coParticleSequence.begin( SEQUENCELIST_LIVE );
        iter != iterEnd;
        ++iter )
    {
        KSequenceHandleInfo& info = *iter;
        CParticleEventSequence* pSeq = info.m_pSequence;
        if ( pSeq == NULL || pSeq->GetDelete() == true )
            continue;
        fn( *pSeq );
    }

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX
    m_bInCriticalLoop = bOldInCriticalLoop;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX
}


template< typename FUNCTION >
void        CKTDGParticleSystem::CParticleEventSequence::ApplyFunctionToParticles( FUNCTION fn )
{
    KParticleHandleList& coParticleList = m_pParticleSystem->_AccessParticleList();
    int iParticleList = m_iParticleList;
    if ( iParticleList >= 0 && iParticleList < (int) coParticleList.storage_size() )
    {
        KParticleHandleList::iterator iterEnd = coParticleList.end( iParticleList );
        KParticleHandleList::iterator iterNext;
        for( KParticleHandleList::iterator iter = coParticleList.begin( iParticleList );
            iter != iterEnd;
            iter = iterNext )
        {
            iterNext = iter; ++iterNext;
            KParticleHandleInfo& info = *iter;
            if ( info.m_pParticle != NULL )
                fn( *info.m_pParticle );
        }
    }
}
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE