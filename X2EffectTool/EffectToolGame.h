#pragma once

class CEffectToolGame : public CX2Game
{
public:
	CEffectToolGame(CX2EffectToolMain* pEffectToolMain_);
	virtual ~CEffectToolGame();

	virtual void Release();

	virtual HRESULT			OnFrameMove( double fTime, float fElapsedTime );
	virtual HRESULT			OnFrameRender();

	virtual void			GameLoading( CX2Room* pRoom );
	virtual void			WorldLoading();
	virtual void			SystemLoading();
	virtual void			EtcLoading();
	//virtual bool			DeleteUserUnit( int index );
	virtual bool			DetermineLastKill() { return false;  }
	virtual/*FORCEINLINE*/ CKTDGParticleSystem*		GetMajorParticle();			//{ return g_pData->GetGameMajorParticle(); }
	virtual/*FORCEINLINE*/ CKTDGParticleSystem*		GetMinorParticle();			//{ return g_pData->GetGameMinorParticle(); }
	virtual/*FORCEINLINE*/ CKTDGXMeshPlayer*		GetMajorXMeshPlayer();		//{ return g_pData->GetGameMajorXMeshPlayer(); }
	virtual/*FORCEINLINE*/ CKTDGXMeshPlayer*		GetMinorXMeshPlayer();		//{ return g_pData->GetGameMinorXMeshPlayer(); }

	void ResetDamageEffect( const WCHAR* pwszFileFullPath = L"DamageEffect.lua" )
	{
		SAFE_DELETE( m_pDamageEffect );
		m_pDamageEffect		= new CX2DamageEffect();
		m_pDamageEffect->OpenScriptFile( pwszFileFullPath );
	}

	void ReloadWorld( const WCHAR* pwszFileName );
	void ResetUnitPosition();
	void SetCamera();


protected:
	void ReleaseCurrentWorld();
	void SquareCamera( CX2GUUser* pFocusUnit, float distance = 1200.0f, float eyeHeight = 200.0f, float lookAtHeight = 100.f, 
		float angleDegree = 0.0f, 
		float eyeDistance = 0.0f, float lookatDistance = 0.0f, float trackingTime = 0.3f );

public:
	struct SquareCameraParam
	{
		float	m_fLookAtHeight;
		float	m_fEyeHeight;
		float	m_fEyeDistance;

		SquareCameraParam()
		{
			m_fLookAtHeight = 100.f;
			m_fEyeHeight	= 200.f;
			m_fEyeDistance	= 1500.f;
		}
	};

public:
	CX2PVPRoom*	m_pPVPRoom;
	bool	m_bSquareCamera;
	bool    m_bOrthoFreeCamera;
	bool	m_bFixedCamera;

	float   m_fOrthoScale;

	SquareCameraParam m_SquareCameraParam;

private:
	CX2EffectToolMain* m_pEffectToolMain;
};