#pragma once


//{{ seojt // 2009-1-14, 23:41
class CX2WorldObjectParticle;
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::intrusive_ptr<CX2WorldObjectParticle>   CX2WorldObjectParticlePtr;
#else   X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::shared_ptr<CX2WorldObjectParticle>   CX2WorldObjectParticlePtr;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
//}} seojt // 2009-1-14, 23:41


class CX2WorldObjectParticle : public CX2WorldObject
{
public:
	/*
	struct ParticleObjectData : public CX2WorldObject::ObjectData
	{
	float	fEmitMin;
	float	fEmitMax;
	int		drawCount;

	void Init()
	{
	CX2WorldObject::ObjectData::Init();

	objectType	= CX2WorldObject::OT_PARTICLE;
	fEmitMin	= 0.0f;
	fEmitMax	= 0.0f;
	drawCount	= 0;
	}

	ParticleObjectData()
	{
	Init();
	}
	};
	*/

private:
	//CX2WorldObjectParticle() { ASSERT( !"invalid" ); }
	CX2WorldObjectParticle( CKTDGParticleSystem* pParticleSystem, const char* pszSequenceName, float fDelayTime = 0.f );
	virtual ~CX2WorldObjectParticle(void);

public: 
	static CX2WorldObjectParticlePtr CreateWorldObjectParticle( CKTDGParticleSystem* pParticleSystem, const char* pszSequenceName, float fDelayTime ) 
	{
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
		CX2WorldObjectParticlePtr pObject( new CX2WorldObjectParticle( pParticleSystem, pszSequenceName, fDelayTime ) );
#else   X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
		CX2WorldObjectParticlePtr pObject( new CX2WorldObjectParticle( pParticleSystem, pszSequenceName, fDelayTime )
            , CKTDGObject::KTDGObjectDeleter() );
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
        return pObject;
	}
	
	virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );

//{{ robobeg : 2008-10-28
    virtual RENDER_HINT OnFrameRender_Prepare();
	//virtual HRESULT OnFrameRender();
    //virtual void    OnFrameRender_Draw();
//}} robobeg : 2008-10-28



	// LUA
	void SetCanHide(bool bCanHide)  { m_bCanHide = bCanHide; }
	void SetParticlePos( D3DXVECTOR3 vPos )
	{ 
		if( m_pParticleSystem == NULL )
			return;

		if( m_hParticleEventSequence == INVALID_PARTICLE_SEQUENCE_HANDLE )
		{
			m_vPos = vPos;
			return;
		}

//{{ robobeg : 2008-10-17
		//m_Pos = vPos;
        SetPos( vPos );
//}} robobeg : 2008-10-17

		CKTDGParticleSystem::CParticleEventSequence* pSeq = m_pParticleSystem->GetInstanceSequence( m_hParticleEventSequence );
		if( NULL != pSeq )
		{
			pSeq->SetPosition( vPos ); 
		}
	}		

	void SetEmitMinMax(float fEmitMin, float fEmitMax) 
	{
		if( m_pParticleSystem == NULL )
			return;

		if( m_hParticleEventSequence == INVALID_PARTICLE_SEQUENCE_HANDLE )
			return;

		if( fEmitMin != -1 && fEmitMax != -1 )
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeq = m_pParticleSystem->GetInstanceSequence( m_hParticleEventSequence );
			if( NULL != pSeq )
			{
				pSeq->SetEmitRate( CMinMax<float>(fEmitMin, fEmitMax) );	
			}
		}

#ifdef X2TOOL
		m_fEmitMin = fEmitMin;
		m_fEmitMax = fEmitMax;
#endif
	}
	void SetTriggerCount( int iTriggerCount )
	{
		if( m_pParticleSystem == NULL )
			return;

		if( m_hParticleEventSequence == INVALID_PARTICLE_SEQUENCE_HANDLE )
			return;

		if( iTriggerCount != -1 )
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeq = m_pParticleSystem->GetInstanceSequence( m_hParticleEventSequence );
			if( NULL != pSeq )
			{
				pSeq->SetTriggerCount( iTriggerCount );
			}
		}

#ifdef X2TOOL
		m_iTriggerCount = iTriggerCount;
#endif
	}
	void SetTriggerTime( float fTriggerTime )
	{
		if( m_pParticleSystem == NULL )
			return;

		if( m_hParticleEventSequence == INVALID_PARTICLE_SEQUENCE_HANDLE )
			return;

		if( fTriggerTime != -1.f )
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeq = m_pParticleSystem->GetInstanceSequence( m_hParticleEventSequence );
			if( NULL != pSeq )
			{
				pSeq->SetTriggerTime( fTriggerTime );
			}
		}

#ifdef X2TOOL
		m_fTriggerTime = fTriggerTime;
#endif
	}
	void SetDrawCount( int iDrawCount )
	{
		if( m_pParticleSystem == NULL )
			return;

		if( m_hParticleEventSequence == INVALID_PARTICLE_SEQUENCE_HANDLE )
			return;

		if( iDrawCount != -1 )
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeq = m_pParticleSystem->GetInstanceSequence( m_hParticleEventSequence );
			if( NULL != pSeq )
			{
				pSeq->SetDrawCount( iDrawCount );
			}
		}
#ifdef X2TOOL
		m_iDrawCount = iDrawCount;
#endif
	}

	void SetCullCheck( bool bCullCheck )
	{
		m_bCullCheck = bCullCheck;

		if( m_pParticleSystem == NULL )
			return;

		if( m_hParticleEventSequence == INVALID_PARTICLE_SEQUENCE_HANDLE )
			return;
		
		CKTDGParticleSystem::CParticleEventSequence* pSeq = m_pParticleSystem->GetInstanceSequence( m_hParticleEventSequence );
		if( NULL != pSeq )
		{
			pSeq->SetCullCheck( bCullCheck );
		}
	}
	void SetLatency( float fLatency )
	{

		if( m_pParticleSystem == NULL )
			return;

		if( m_hParticleEventSequence == INVALID_PARTICLE_SEQUENCE_HANDLE )
			return;

		CKTDGParticleSystem::CParticleEventSequence* pSeq = m_pParticleSystem->GetInstanceSequence( m_hParticleEventSequence );
		if( NULL != pSeq )
		{
			pSeq->SetLatency( fLatency );
		}
#ifdef X2TOOL
		m_fLatency = fLatency;
#endif

	}
	void SetParticleType( CKTDGParticleSystem::PARTICLE_TYPE ptType )
	{

		if( m_pParticleSystem == NULL )
			return;

		if( m_hParticleEventSequence == INVALID_PARTICLE_SEQUENCE_HANDLE )
		{			
			return;
		}

		CKTDGParticleSystem::CParticleEventSequence* pSeq = m_pParticleSystem->GetInstanceSequence( m_hParticleEventSequence );
		if( NULL != pSeq )
		{
			pSeq->SetParticleType( ptType );
		}

#ifdef X2TOOL
		m_bChangeType = true;
		m_ptType = ptType;
#endif
	}
	//void CreateParticleSequence( CKTDGParticleSystem* m_pParticleSystem, const char* pszSequenceName );


public: // tool-only 
	CKTDGParticleSystem::CParticleEventSequence*  GetParticleEventSequence() const 
	{ 
		if( NULL == m_pParticleSystem )
			return NULL;

		if( INVALID_PARTICLE_SEQUENCE_HANDLE == m_hParticleEventSequence )
			return NULL;

		return m_pParticleSystem->GetInstanceSequence( m_hParticleEventSequence );
	}	
	CKTDGParticleSystem::CParticleEventSequenceHandle GetHandle() const { return m_hParticleEventSequence; }
protected:
	CKTDGParticleSystem*								m_pParticleSystem;
	CKTDGParticleSystem::CParticleEventSequenceHandle	m_hParticleEventSequence;
	
	
	//CKTDGXRenderer::RenderParam		m_RenderParam;
	
	bool	m_bCanHide;
	float	m_fDelayTime;
	wstring m_wstrSequenceName;
	
	bool m_bCullCheck;
	D3DXVECTOR3 m_vPos;
		
#ifdef X2TOOL
public:

	enum PARTICLE_TYPE
	{
		PT_GAME_MAJOR,
		PT_GAME_MINOR,
		PT_UI_MAJOR,
		PT_UI_MINOR,
	};
	bool GetCanHide() { return m_bCanHide; }
	void SetDelayTime(float fVal) { m_fDelayTime = fVal; }
	float GetDelayTime() { return m_fDelayTime; }
	wstring GetParticleName() { return m_wstrSequenceName; }
	bool GetCullCheck() { return m_bCullCheck; }
	
	bool m_bChangeType;
	CKTDGParticleSystem::PARTICLE_TYPE m_ptType;
	float m_fLatency;	
	float m_fTriggerTime;	
	float m_fEmitMin;
	float m_fEmitMax;
	int m_iTriggerCount;
	int m_iDrawCount;
	PARTICLE_TYPE m_eParticleType;
#endif
};
