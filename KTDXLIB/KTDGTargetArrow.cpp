#include "StdAfx.h"
#include ".\ktdgtargetarrow.h"

//CKTDGTargetArrow::CKTDGTargetArrow(void)
//{	
//    m_pParticle = NULL;
//	m_pPaticleSeqRot = NULL;
//	m_pPaticleSeq = NULL;
//}
//
//CKTDGTargetArrow::~CKTDGTargetArrow(void)
//{
//    if(m_pParticle != NULL && m_pPaticleSeqRot != NULL && m_pParticle->IsLiveInstance(m_pPaticleSeqRot))
//    {
//        m_pPaticleSeqRot->ClearAllParticle();
//        m_pPaticleSeqRot->SetAutoDie();
//        m_pPaticleSeqRot = NULL;
//    }
//
//    if(m_pParticle != NULL && m_pPaticleSeq != NULL && m_pParticle->IsLiveInstance(m_pPaticleSeq))
//    {
//        m_pPaticleSeq->ClearAllParticle();
//        m_pPaticleSeq->SetAutoDie();
//        m_pPaticleSeq = NULL;
//    }
//}
//
//void CKTDGTargetArrow::SetCurserParticle( CKTDGParticleSystem::CParticleEventSequence* pPaticleSeqRot, CKTDGParticleSystem::CParticleEventSequence* pPaticleSeq, CKTDGParticleSystem *pParticleSystem)
//{
//    
//    m_pParticle         = pParticleSystem;
//	m_pPaticleSeqRot	= pPaticleSeqRot;
//	m_pPaticleSeq		= pPaticleSeq;
//}
//
//void CKTDGTargetArrow::CalcPos( D3DXVECTOR3 myPos, D3DXVECTOR3 targetPos )
//{
//	if( m_pParticle == NULL || m_pPaticleSeqRot == NULL || m_pPaticleSeq == NULL )
//		return;
//
//	D3DXVECTOR2 myScreenPos = g_pKTDXApp->GetProj3DPos( myPos ); 
//	D3DXVECTOR2 targetScreenPos = g_pKTDXApp->GetProj3DPos( targetPos ); 
//
//	//타겟 포지션이 스크린 밖에 있는지를 검사한다
//    D3DXVECTOR2 screenSize = g_pKTDXApp->ConvertByResolution( 1024, 768 );
//
//	if( targetScreenPos.x < 0.0f 
//		|| targetScreenPos.y < 0.0f
//		|| targetScreenPos.x > screenSize.x 
//		|| targetScreenPos.y > screenSize.y )
//	{
//		D3DXVECTOR3 lineAStartPos;
//		D3DXVECTOR3 lineAEndPos;
//		D3DXVECTOR3 lineBStartPos;
//		D3DXVECTOR3 lineBEndPos;
//		D3DXVECTOR3 collisionPosA;
//		D3DXVECTOR3 collisionPosB;
//
//		lineAStartPos.x	= myScreenPos.x;
//		lineAStartPos.y	= myScreenPos.y;
//		lineAStartPos.z	= 0.0f;
//
//		lineAEndPos.x	= targetScreenPos.x;
//		lineAEndPos.y	= targetScreenPos.y;
//		lineAEndPos.z	= 0.0f;
//
//
//        lineBStartPos.x	= 0.0f;
//		lineBStartPos.y	= 0.0f;
//		lineBStartPos.z	= 0.0f;
//
//		lineBEndPos.x	= screenSize.x;
//		lineBEndPos.y	= 0.0f;
//		lineBEndPos.z	= 0.0f;
//		if( g_pKTDXApp->GetCollision()->LineToLine( lineAStartPos, lineAEndPos, lineBStartPos, lineBEndPos, 1, collisionPosA, collisionPosB ) == true )
//		{
//			float fDistance = GetDistance( lineAStartPos, lineAEndPos );
//			SetData( fDistance, lineAStartPos, lineAEndPos, collisionPosA );
//			return;
//		}
//		
//		lineBStartPos.x	= screenSize.x;
//		lineBStartPos.y	= 0.0f;
//		lineBStartPos.z	= 0.0f;
//
//		lineBEndPos.x	= screenSize.x;
//		lineBEndPos.y	= screenSize.y;
//		lineBEndPos.z	= 0.0f;
//		if( g_pKTDXApp->GetCollision()->LineToLine( lineAStartPos, lineAEndPos, lineBStartPos, lineBEndPos, 1, collisionPosA, collisionPosB ) == true )
//		{
//			float fDistance = GetDistance( lineAStartPos, lineAEndPos );
//			SetData( fDistance, lineAStartPos, lineAEndPos, collisionPosA );
//			return;
//		}
//
//		lineBStartPos.x	= screenSize.x;
//		lineBStartPos.y	= screenSize.y;
//		lineBStartPos.z	= 0.0f;
//
//		lineBEndPos.x	= 0.0f;
//		lineBEndPos.y	= screenSize.y;
//		lineBEndPos.z	= 0.0f;
//		if( g_pKTDXApp->GetCollision()->LineToLine( lineAStartPos, lineAEndPos, lineBStartPos, lineBEndPos, 1, collisionPosA, collisionPosB ) == true )
//		{
//			float fDistance = GetDistance( lineAStartPos, lineAEndPos );
//			SetData( fDistance, lineAStartPos, lineAEndPos, collisionPosA );
//			return;
//		}
//
//		lineBStartPos.x	= 0.0f;
//		lineBStartPos.y	= screenSize.y;
//		lineBStartPos.z	= 0.0f;
//
//		lineBEndPos.x	= 0.0f;
//		lineBEndPos.y	= 0.0f;
//		lineBEndPos.z	= 0.0f;
//		if( g_pKTDXApp->GetCollision()->LineToLine( lineAStartPos, lineAEndPos, lineBStartPos, lineBEndPos, 1, collisionPosA, collisionPosB ) == true )
//		{
//			float fDistance = GetDistance( lineAStartPos, lineAEndPos );
//			SetData( fDistance, lineAStartPos, lineAEndPos, collisionPosA );
//			return;
//		}
//	}
//	else
//	{
//		m_pPaticleSeqRot->SetShow( false );
//		m_pPaticleSeq->SetShow( false );
//	}
//}
//
//void CKTDGTargetArrow::SetData( float fDistance, D3DXVECTOR3 lineAStartPos, D3DXVECTOR3 lineAEndPos, D3DXVECTOR3 collisionPos )
//{
//    if( m_pParticle == NULL || m_pPaticleSeqRot == NULL || m_pPaticleSeq == NULL)
//        return;
//    
//    if( m_pParticle->IsLiveInstance(m_pPaticleSeqRot) == false || m_pParticle->IsLiveInstance(m_pPaticleSeq) == false)
//        return;
//
//	m_pPaticleSeqRot->SetShow( true );
//	m_pPaticleSeq->SetShow( true );
//
//	float fScaleFactor = 1.0f;
//	fDistance -= 1000.0f;
//	if( fDistance < 0.0f )
//		fDistance = 0.0f;
//
//	fScaleFactor -= fDistance / 1000.0f;
//	if( fScaleFactor < 0.5f )
//	{
//		fScaleFactor = 0.5f;
//	}
//
//	D3DXVECTOR3 vDirVector = lineAEndPos - lineAStartPos;
//	D3DXVec3Normalize( &vDirVector, &vDirVector );
//	D3DXVECTOR3 vRot = GetDirVecToDegree( vDirVector );
//
//	collisionPos = collisionPos - vDirVector * (100.0f * g_pKTDXApp->GetResolutionScaleX());
//	CKTDGParticleSystem::CParticle** ppParticle = m_pPaticleSeqRot->m_Particles.GetLiveElementList();
//	for( int i = 0; i < m_pPaticleSeqRot->m_Particles.GetSize(); i++ )
//	{	
//		// Render each particle a bunch of times to get a blurring effect		
//		CKTDGParticleSystem::CParticle* pPart = ppParticle[i];
//		pPart->m_vRotate = vRot;
//	}
//
//    
//    m_pPaticleSeqRot->SetPosition( collisionPos );
//    m_pPaticleSeqRot->SetScaleFactor( D3DXVECTOR3( fScaleFactor, fScaleFactor, 1.0f ) );
//
//    m_pPaticleSeq->SetPosition( collisionPos );
//    m_pPaticleSeq->SetScaleFactor( D3DXVECTOR3( fScaleFactor, fScaleFactor, 1.0f ) );
//    
//}
//
//
//
//
//
//
