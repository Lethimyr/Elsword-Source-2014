#include "StdAfx.h"
#include ".\x2curtain.h"

CX2Curtain::CX2Curtain( int iLineIndex, bool bAtStart, bool bLookLeft, int curtainGroupIndex )
{
	m_LineIndex					= iLineIndex;
	m_bAtStart					= bAtStart;
	m_bLookLeft					= bLookLeft;
	m_iCurtainGroupIndex		= curtainGroupIndex;

	m_ReactLength				= D3DXVECTOR2( 10.0f, 200.0f );
	m_fReactGapLengthX			= 15.0f;

	const CKTDGLineMap::LineData* pLineData = g_pX2Game->GetWorld()->GetLineMap()->GetLineData( iLineIndex );
	if( bAtStart == true )
	{
		m_vPos = pLineData->startPos;
	}
	else
	{
		m_vPos = pLineData->endPos;
	}
	m_vPos.y += 200;

	m_vRotateDegree = pLineData->dirDegree;
	if( bLookLeft == false )
	{
		m_vRotateDegree.y += 90.0f + 20.0f;
	}
	else
	{
		m_vRotateDegree.y += 90.0f - 20.0f;
	}

	m_hCurtainRing1		= INVALID_PARTICLE_SEQUENCE_HANDLE;
	m_hCurtainRing2		= INVALID_PARTICLE_SEQUENCE_HANDLE;
	m_hCurtainCenter	= INVALID_PARTICLE_SEQUENCE_HANDLE;
	m_pCrashSound		= g_pKTDXApp->GetDeviceManager()->OpenSound( L"GlassChash.ogg" );
	m_pHitSound			= g_pKTDXApp->GetDeviceManager()->OpenSound( L"CurtainHit.ogg" );

	D3DXVECTOR3 posRing = m_vPos;
	posRing += pLineData->dirVector * 5.0f;

	m_hCurtainRing1 = g_pX2Game->GetMajorParticle()->CreateSequenceHandle( NULL,  L"CurtainRing", posRing, 10000, 10000, 1 );
	CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hCurtainRing1 );
	if( NULL != pSeq )
	{
		pSeq->SetAddRotate( m_vRotateDegree );
	}

	posRing = m_vPos;
	posRing -= pLineData->dirVector * 5.0f;

	m_hCurtainRing2 = g_pX2Game->GetMajorParticle()->CreateSequenceHandle( NULL,  L"CurtainRingCounter", posRing, 10000, 10000, 1 );

	pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hCurtainRing2 );
	if( NULL != pSeq )
	{
		pSeq->SetAddRotate( m_vRotateDegree );
	}

	m_hCurtainCenter = g_pX2Game->GetMajorParticle()->CreateSequenceHandle( NULL,  L"CurtainCenter", m_vPos, 10000, 10000, 1 );
	m_vRotateDegree.z += 45.0f;
	
	pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hCurtainCenter );
	if( NULL != pSeq )
	{
		pSeq->SetAddRotate( m_vRotateDegree );
	}

	m_vRotateDegree.z -= 45.0f;
}

CX2Curtain::~CX2Curtain(void)
{
	g_pX2Game->GetMajorParticle()->DestroyInstanceHandle( m_hCurtainRing1 );
	g_pX2Game->GetMajorParticle()->DestroyInstanceHandle( m_hCurtainRing2 );
	g_pX2Game->GetMajorParticle()->DestroyInstanceHandle( m_hCurtainCenter );
	SAFE_CLOSE( m_pCrashSound );
	SAFE_CLOSE( m_pHitSound );
	//CurtainBreak();
}

HRESULT CX2Curtain::OnFrameMove( double fTime, float fElapsedTime )
{
	CollisionCheck();
	return S_OK;
}

void CX2Curtain::Open()
{
	g_pX2Game->GetMajorParticle()->DestroyInstanceHandle( m_hCurtainCenter );
	m_hCurtainCenter = INVALID_PARTICLE_SEQUENCE_HANDLE;
}

void CX2Curtain::CollisionCheck()
{
	for ( int i = 0; i < g_pX2Game->GetUserUnitListSize(); i++ )
	{
		CX2GUUser* pGUUser = g_pX2Game->GetUserUnit(i);
		if ( pGUUser == NULL )
			continue;

		D3DXVECTOR3 impactPos;
		impactPos = m_vPos;

		D3DXVECTOR2 vPosCurtain = D3DXVECTOR2( m_vPos.x, m_vPos.z );
		D3DXVECTOR2 vPosUser    = D3DXVECTOR2( pGUUser->GetPos().x, pGUUser->GetPos().z );

		D3DXVECTOR2 vPosCurtainToUser = vPosCurtain - vPosUser;
		float		fLen = D3DXVec2Length( &vPosCurtainToUser );

		if ( m_CollisionCheckedUnitIndex.find(i) == m_CollisionCheckedUnitIndex.end() &&  
			fLen <= m_ReactLength.x &&
			m_vPos.y + m_ReactLength.y >= pGUUser->GetPos().y &&
			m_vPos.y - m_ReactLength.y <= pGUUser->GetPos().y )
		{
			m_CollisionCheckedUnitIndex.insert( i );
			CurtainHit( pGUUser );
		}
		else
		{
			D3DXVECTOR2 m_vPosCurtain = D3DXVECTOR2( m_vPos.x, m_vPos.z );
			D3DXVECTOR2 m_vPosUser    = D3DXVECTOR2( pGUUser->GetPos().x, pGUUser->GetPos().z );

			if ( fLen >= m_fReactGapLengthX )
			{
				m_CollisionCheckedUnitIndex.erase( i );
			}
		}
	}
}

void CX2Curtain::CurtainHit( CX2GUUser* pUserUnit )
{
	//Ãæµ¹ ÀÌÆåÆ®
	CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMinorParticle()->CreateSequence( NULL,  L"CurtainHitTick", pUserUnit->GetPos(), 10000, 10000, -1, 20 );
	if( pSeq != NULL )
		pSeq->SetLandPosition( m_vPos.y - 200.0f );
	pSeq = g_pX2Game->GetMinorParticle()->CreateSequence( NULL,  L"CurtainHitCircle", pUserUnit->GetPos(), 10000, 10000, -1, 1 );
	if( pSeq != NULL )
		pSeq->SetAddRotate( m_vRotateDegree );

	CX2GameUnit* pFocusUnit = g_pX2Game->GetFocusUnit();
	if( GetDistance( pUserUnit->GetPos(), pFocusUnit->GetPos() ) < 300 )
	{
		g_pX2Game->GetX2Camera()->GetCamera().UpDownCrashCameraNoReset( 20.0f, 0.3f );
	}

	if( NULL != m_pHitSound )
	{
		m_pHitSound->Play( false, false ); 
	}
}

void CX2Curtain::CurtainBreak()
{
	CKTDGParticleSystem::CParticleEventSequence* pSeq = NULL;
	g_pX2Game->GetMinorParticle()->CreateSequence( NULL,  L"CurtainBreakSlash", m_vPos, 20, 20, -1, 3 );
	pSeq = g_pX2Game->GetMinorParticle()->CreateSequence( NULL,  L"CurtainBreak1", m_vPos, 10000, 10000, -1, 4 );
	if( pSeq != NULL )
		pSeq->SetLandPosition( m_vPos.y - 200.0f );
	pSeq = g_pX2Game->GetMinorParticle()->CreateSequence( NULL,  L"CurtainBreak2", m_vPos, 10000, 10000, -1, 4 );
	if( pSeq != NULL )
		pSeq->SetLandPosition( m_vPos.y - 200.0f );
	pSeq = g_pX2Game->GetMinorParticle()->CreateSequence( NULL,  L"CurtainBreak3", m_vPos, 10000, 10000, -1, 4 );
	if( pSeq != NULL )
		pSeq->SetLandPosition( m_vPos.y - 200.0f );
	pSeq = g_pX2Game->GetMinorParticle()->CreateSequence( NULL,  L"CurtainBreakCircle", m_vPos, 2, 3, -1, 1 );
	if( pSeq != NULL )
		pSeq->SetAddRotate( m_vRotateDegree );

	CX2GameUnit* pFocusUnit = g_pX2Game->GetFocusUnit();

	if( NULL != pFocusUnit )
	{
		if( GetDistance( m_vPos, pFocusUnit->GetPos() ) < 500 )
		{
			g_pX2Game->GetX2Camera()->GetCamera().UpDownCrashCameraNoReset( 20.0f, 0.3f );
		}
	}

	if( NULL != m_pCrashSound )
	{
		m_pCrashSound->Play( false, false ); 
	}
}


