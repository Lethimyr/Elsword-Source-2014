#pragma once


class CX2GUUser;
class CX2Curtain
{
	public:
		CX2Curtain( int iLineIndex, bool bAtStart, bool bLookLeft, int curtainGroupIndex );
		virtual ~CX2Curtain(void);

		HRESULT		OnFrameMove( double fTime, float fElapsedTime );
		int			GetCurtainGroupIndex(){ return m_iCurtainGroupIndex; }
		void		Open();
		void		CurtainBreak();

		int			GetLineIndex() { return m_LineIndex; }

	protected:
		void		CollisionCheck();
		void		CurtainHit( CX2GUUser* pUserUnit );
		

		int												m_LineIndex;
		bool											m_bAtStart;
		bool											m_bLookLeft;
		int												m_iCurtainGroupIndex;
		
		D3DXVECTOR3										m_vPos;	
		D3DXVECTOR3										m_vRotateDegree;	

		D3DXVECTOR2										m_ReactLength;
		float											m_fReactGapLengthX;
        std::set< int >									m_CollisionCheckedUnitIndex;

		CKTDXDeviceSound*								m_pCrashSound;
		CKTDXDeviceSound*								m_pHitSound;

		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hCurtainRing1;
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hCurtainRing2;
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hCurtainCenter;
};
