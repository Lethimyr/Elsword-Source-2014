#include "StdAfx.h"
#include ".\x2monsterindicator.h"


#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
CX2MonsterIndicator::CX2MonsterIndicator(MONSTER_INDICATOR_TYPE eIndicatorType_ )
#else // SERV_BATTLEFIELD_MIDDLE_BOSS
CX2MonsterIndicator::CX2MonsterIndicator(void)
#endif // SERV_BATTLEFIELD_MIDDLE_BOSS
{	
	m_hMonsterIndicator = INVALID_PARTICLE_SEQUENCE_HANDLE;
	m_hMonsterIndicatorFace = INVALID_PARTICLE_SEQUENCE_HANDLE;
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    m_hMonsterIndicatorParticle = INVALID_PARTICLE_HANDLE;
	m_hMonsterIndicatorFaceParticle = INVALID_PARTICLE_HANDLE;
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    m_pMonsterIndicatorParticle = NULL;
	m_pMonsterIndicatorFaceParticle = NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

	if( NULL != g_pX2Game &&
		NULL != g_pX2Game->GetMajorParticle() )
	{
		m_hMonsterIndicator = g_pX2Game->GetMajorParticle()->CreateSequenceHandle( NULL,  L"TargetArrow", 0, 0, 0 );
		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hMonsterIndicator );
		if( NULL != pSeq )
		{
			pSeq->SetTriggerWait( true );
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            m_hMonsterIndicatorParticle = pSeq->CreateNewParticleHandle( D3DXVECTOR3(0.0f,0.0f,0.0f) );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			m_pMonsterIndicatorParticle = pSeq->CreateNewParticle( D3DXVECTOR3(0.0f,0.0f,0.0f) );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		}

#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
		switch ( eIndicatorType_ )		// Indicator Type 에 맞게 뿌려줌
		{
			case MIT_DEFAULT :			// 기본 형태
				m_hMonsterIndicatorFace = g_pX2Game->GetMajorParticle()->CreateSequenceHandle( NULL,  L"TargetArrowMon", 0, 0, 0 );
				break;
			case MIT_FIELD_MIDDLE_BOSS :// 필드 중간 보스 형태
				m_hMonsterIndicatorFace = g_pX2Game->GetMajorParticle()->CreateSequenceHandle( NULL,  L"TargetMiddleBossMon", 0, 0, 0 );
				break;
#ifdef SERV_BATTLEFIELD_EVENT_BOSS_INT
			case MIT_FIELD_EVENT_BOSS :// 필드 이벤트 보스 형태
				m_hMonsterIndicatorFace = g_pX2Game->GetMajorParticle()->CreateSequenceHandle( NULL,  L"TargetEventBossMon", 0, 0, 0 );
				break;
#endif //SERV_BATTLEFIELD_EVENT_BOSS_INT
		}
#else //  SERV_BATTLEFIELD_MIDDLE_BOSS
		m_hMonsterIndicatorFace = g_pX2Game->GetMajorParticle()->CreateSequenceHandle( NULL,  L"TargetArrowMon", 0, 0, 0 );
#endif // SERV_BATTLEFIELD_MIDDLE_BOSS
		
		
		pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hMonsterIndicatorFace );
		if( NULL != pSeq )
		{
			pSeq->SetTriggerWait( true );
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		    m_hMonsterIndicatorFaceParticle = pSeq->CreateNewParticleHandle( D3DXVECTOR3(0.0f,0.0f,0.0f) );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            m_pMonsterIndicatorFaceParticle = pSeq->CreateNewParticle( D3DXVECTOR3(0.0f,0.0f,0.0f) );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		}
	}

#ifdef MODIFY_DUNGEON_STAGING
	m_bShow = true;
#endif //MODIFY_DUNGEON_STAGING

#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
	m_pTargetNPC.Reset();
	m_eIndicatorType = eIndicatorType_;		// Indicator Type, enum MIT 참조
#endif // SERV_BATTLEFIELD_MIDDLE_BOSS
}

CX2MonsterIndicator::~CX2MonsterIndicator(void)
{
	if( NULL != g_pX2Game &&
		NULL != g_pX2Game->GetMajorParticle() )
	{
		g_pX2Game->GetMajorParticle()->DestroyInstanceHandle( m_hMonsterIndicator );
		g_pX2Game->GetMajorParticle()->DestroyInstanceHandle( m_hMonsterIndicatorFace );
	}
}


void CX2MonsterIndicator::OnFrameMove()
{

	KTDXPROFILE();


	if( INVALID_PARTICLE_SEQUENCE_HANDLE == m_hMonsterIndicator ||
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        INVALID_PARTICLE_HANDLE == m_hMonsterIndicatorParticle
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		NULL == m_pMonsterIndicatorParticle 
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        )
		return;

	if( INVALID_PARTICLE_SEQUENCE_HANDLE == m_hMonsterIndicatorFace ||
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        INVALID_PARTICLE_HANDLE == m_hMonsterIndicatorFaceParticle
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		NULL == m_pMonsterIndicatorFaceParticle 
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        )
		return;

	if( NULL == g_pX2Game )
		return;

	if( NULL == g_pX2Game->GetFocusUnit() )
		return;


	CKTDGParticleSystem::CParticleEventSequence* pSeqArrow	= g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hMonsterIndicator );
	CKTDGParticleSystem::CParticleEventSequence* pSeqFace	= g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hMonsterIndicatorFace );

	if( NULL == pSeqArrow ||
		NULL == pSeqFace )
		return;


	int iNPCCountInFrustum = 0;
	float fNearestDistance = -1.f;
	D3DXVECTOR3 vNearestPos(0, 0, 0);

	// 화면안에 몬스터가 없고, 화면밖에 몬스터가 있다면 
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
#ifdef SERV_BATTLEFIELD_EVENT_BOSS_INT 
	if ( m_eIndicatorType == MIT_FIELD_MIDDLE_BOSS || m_eIndicatorType == MIT_FIELD_EVENT_BOSS)
#else //SERV_BATTLEFIELD_EVENT_BOSS_INT
	if ( m_eIndicatorType == MIT_FIELD_MIDDLE_BOSS )		// FieldMiddleBoss 일 경우
#endif //SERV_BATTLEFIELD_EVENT_BOSS_INT
	{
		if ( null != m_pTargetNPC )		// 타켓으로 지정된 몬스터가 있나?
		{	
			m_bShow = true;
			// 이 부분 크래시 나는 중
			// 죽었거나 없다면 일단 Indicator 를 끔
			if( 0.f >= m_pTargetNPC->GetNowHp() || 
				CX2GameUnit::GUSI_DIE == m_pTargetNPC->GetGameUnitState() || 
				false == m_pTargetNPC->IsActiveMonster() ||
				NULL == g_pX2Game->GetNPCUnitByUID ( m_pTargetNPC->GetUID() ) 			  
			  )				
			{
				m_bShow = false;
				m_pTargetNPC.Reset();
			}

			// 캐시샵 입장 중 가이드 출력하지 않기.
			if( NULL != g_pData->GetCashShop() &&
				true == g_pData->GetCashShop()->GetOpen() )
			{
				m_bShow = false;
			}

			if( true == m_bShow )
			{
				D3DXVECTOR3 center;
				m_pTargetNPC->GetTransformCenter( &center );

				if( m_pTargetNPC->GetBoundingRadius() > 0 &&
					g_pKTDXApp->GetDGManager()->GetFrustum().CheckSphere( center, m_pTargetNPC->GetBoundingRadius() * m_pTargetNPC->GetScale() ) == true )
				{
					iNPCCountInFrustum += 1;	
				}
				else
				{
					float fTempDistance = g_pX2Game->GetFocusUnit()->GetDistanceFrom( center );
					if( fNearestDistance == -1.f || fNearestDistance > fTempDistance )
					{
						fNearestDistance = fTempDistance;
						vNearestPos = center;
					}
				}
			}
		}
	}
	else
#endif // SERV_BATTLEFIELD_MIDDLE_BOSS
	{
		for( int i =0; i<g_pX2Game->GetNPCUnitListSize(); i++ )
		{
			CX2GUNPC* pNPC = g_pX2Game->GetNPCUnit( i );
			if( NULL == pNPC || pNPC->GetNowHp() <= 0.f || 
				pNPC->GetGameUnitState() == CX2GameUnit::GUSI_DIE || 
				false == pNPC->IsActiveMonster() )
			{
				continue;
			}

			// note!! 현재 ally type 몬스터는 이브가 소환한 몬스터 밖에 없으므로 몬스터 화살표에서 제외
			if( NULL == pNPC->GetNPCAI() )
				continue;

			if( CX2NPCAI::NAT_NORMAL != pNPC->GetNPCAI()->GetAIType() )
				continue;
				

			D3DXVECTOR3 center;
			pNPC->GetTransformCenter( &center );

			if( pNPC->GetBoundingRadius() > 0 &&
				g_pKTDXApp->GetDGManager()->GetFrustum().CheckSphere( center, pNPC->GetBoundingRadius() * pNPC->GetScale() ) == true )
			{
				iNPCCountInFrustum += 1;
			}
			else
			{
				float fTempDistance = g_pX2Game->GetFocusUnit()->GetDistanceFrom( center );
				if( fNearestDistance == -1.f || fNearestDistance > fTempDistance )
				{
					fNearestDistance = fTempDistance;
					vNearestPos = center;
				}
			}
		}
	}

	if( iNPCCountInFrustum == 0 && fNearestDistance != -1.f )
	{
		// 3D~~
		//D3DXVECTOR3 vPos = GetFocusUnit()->GetPos();
		//vPos.y += 500.f;
		//m_pMonsterIndicator->SetPosition( vPos );
		//m_pMonsterIndicator->UseLookPoint( true );
		//m_pMonsterIndicator->SetLookPoint( vNearestPos );

		//D3DXVECTOR3 vRot = GetDirVecToDegree( GetFocusUnit()->GetDirVector() );
		//m_pMonsterIndicator->SetAddRotate( vRot );
		//m_pMonsterIndicator->SetAxisAngle( vRot );

		//m_pMonsterIndicator->SetShow( true );




		// 2D
		D3DXVECTOR2 vNPCProjPos2D = g_pKTDXApp->GetProj3DPos( vNearestPos );
		D3DXVECTOR3 vNPCProjPos;
		vNPCProjPos.x = vNPCProjPos2D.x;
		vNPCProjPos.y = vNPCProjPos2D.y;
		vNPCProjPos.z = 0.f;



		//D3DXVECTOR3 vScreenCenter( 512.f * g_pKTDXApp->GetResolutionScaleX(), 384.f * g_pKTDXApp->GetResolutionScaleY(), 0.f );
		//D3DXVECTOR3 vScreenLeftTop( 0, 0, 0 );
		//D3DXVECTOR3 vScreenRightTop( 1024 * g_pKTDXApp->GetResolutionScaleX(), 0, 0 );
		//D3DXVECTOR3 vScreenLeftBottom( 0, 768 * g_pKTDXApp->GetResolutionScaleY(), 0 );
		//D3DXVECTOR3 vScreenRightBottom( 1024 * g_pKTDXApp->GetResolutionScaleX(), 768 * g_pKTDXApp->GetResolutionScaleY(), 0 );


		//D3DXVECTOR3 vScreenCenter( 512, 384, 0 );
		//D3DXVECTOR3 vScreenLeftTop( 0, 0, 0 );
		//D3DXVECTOR3 vScreenRightTop( 1024, 0, 0 );
		//D3DXVECTOR3 vScreenLeftBottom( 0, 768, 0 );
		//D3DXVECTOR3 vScreenRightBottom( 1024, 768, 0 );

		// 던전내 메인메뉴바가 존재할경우 메뉴바를 제외한 스크린영역을 설정한다.
		D3DXVECTOR3 vScreenCenter( 512, 384-25, 0 );
		D3DXVECTOR3 vScreenLeftTop( 0, 0, 0 );
		D3DXVECTOR3 vScreenRightTop( 1024, 0, 0 );
		D3DXVECTOR3 vScreenLeftBottom( 0, 768-50, 0 );
		D3DXVECTOR3 vScreenRightBottom( 1024, 768-50, 0 );


		D3DXVECTOR3 vTargetPosVec = vNPCProjPos - vScreenCenter;
		D3DXVec3Normalize( &vTargetPosVec, &vTargetPosVec );
		vNPCProjPos = vScreenCenter + vTargetPosVec * 5000.f;



		D3DXVECTOR3 vContactPoint1( -1, -1, 0 );
		D3DXVECTOR3 vContactPoint2;

		if( true == g_pKTDXApp->GetCollision()->LineToLine( vScreenCenter, vNPCProjPos, vScreenLeftTop, vScreenRightTop, 1.f, vContactPoint1, vContactPoint2 ) )
		{

		}
		else if( true == g_pKTDXApp->GetCollision()->LineToLine( vScreenCenter, vNPCProjPos, vScreenLeftBottom, vScreenRightBottom, 1.f, vContactPoint1, vContactPoint2 ) )
		{


		}
		else if( true == g_pKTDXApp->GetCollision()->LineToLine( vScreenCenter, vNPCProjPos, vScreenLeftTop, vScreenLeftBottom, 1.f, vContactPoint1, vContactPoint2 ) )
		{


		}
		else if( true == g_pKTDXApp->GetCollision()->LineToLine( vScreenCenter, vNPCProjPos, vScreenRightTop, vScreenRightBottom, 1.f, vContactPoint1, vContactPoint2 ) )
		{


		}


		if( vContactPoint1.x != -1.f && vContactPoint1.y != -1.f )
		{
			D3DXVECTOR3 vPosOnScr;
			vPosOnScr.x = vContactPoint1.x;
			vPosOnScr.y = vContactPoint1.y;
			vPosOnScr.z = 0.f;

			vPosOnScr += (vScreenCenter - vPosOnScr) * 0.1f;

			pSeqArrow->SetPosition( vPosOnScr );
			pSeqFace->SetPosition( vPosOnScr );

			//m_pMonsterIndicatorParticle->m_vPos = vPosOnScr;


			D3DXVECTOR3 vDirVector = vPosOnScr - vScreenCenter;
			//float fCos = D3DXVec3Dot( vDirVector, D3DXVec3Length( &vDirVector );
			D3DXVec3Normalize( &vDirVector, &vDirVector );
			D3DXVECTOR3 vRot = GetDirVecToDegree( vDirVector );

			// fix!!
			//vRot = D3DXVECTOR3( 0, 0, 50 );
			pSeqArrow->SetAddRotate( vRot );
			pSeqArrow->SetAxisAngle( vRot );

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            if ( CKTDGParticleSystem::CParticle* pParticle = pSeqArrow->GetParticle( m_hMonsterIndicatorParticle ) )
            {
			    pParticle->SetAxisRotateDegree( vRot );
			    pParticle->SetRotate( vRot );
            }
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			m_pMonsterIndicatorParticle->SetAxisRotateDegree( vRot );
			m_pMonsterIndicatorParticle->SetRotate( vRot );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE


			float fScaleFactor = fNearestDistance / 2000.f;
			if( fScaleFactor > 1.1f )
				fScaleFactor = 1.1f;
			else if( fScaleFactor < 0.5f )
			{
				fScaleFactor = 0.5f; 
			}

			pSeqArrow->SetScaleFactor( D3DXVECTOR3( fScaleFactor, fScaleFactor, 1.f ) );
			pSeqFace->SetScaleFactor( D3DXVECTOR3( fScaleFactor, fScaleFactor, 1.f ) );


			//m_pPaticleSeqRot->SetScaleFactor( D3DXVECTOR3( fScaleFactor, fScaleFactor, 1.0f ) );



#ifdef DIALOG_SHOW_TOGGLE
			bool bHide = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetHideDialog();
			if( bHide == true )
			{
				pSeqFace->SetShowObject( false );
				pSeqArrow->SetShowObject( false );
				return;
			}
#endif
#ifdef MODIFY_DUNGEON_STAGING
			if( false == m_bShow )
			{
				pSeqFace->SetShowObject( false );
				pSeqArrow->SetShowObject( false );
				return;
			}
#endif //MODIFY_DUNGEON_STAGING


			pSeqFace->SetShowObject( true );
			pSeqArrow->SetShowObject( true );
		}
		else
		{
			pSeqFace->SetShowObject( false );
			pSeqArrow->SetShowObject( false );
		}

	}
	else
	{
		pSeqFace->SetShowObject( false );
		pSeqArrow->SetShowObject( false );
	}

}
