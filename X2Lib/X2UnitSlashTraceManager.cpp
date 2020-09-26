#include "StdAfx.h"
#include ".\x2unitslashtracemanager.h"



#ifdef UNIT_SLASH_TRACE_MANAGER_TEST

void CX2UnitSlashTraceManager::OnStateStart()
{
	m_mapSlashTraceTiming.clear();
}


void CX2UnitSlashTraceManager::OnFrameMove( double fTime, float fElapsedTime )
{
	KTDXPROFILE();

#ifdef X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
	if( g_pMain->GetGameOption().GetOptionList().m_eEffect == CX2GameOption::OL_HIGH )
#else//X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
	if( true == g_pMain->GetGameOption().GetOptionList().m_bEffect )
#endif//X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
	{
		SetEnable( true );
	}
	else
	{
		SetEnable( false );
	}

	if( false == GetEnable() )
		return;

	if( false == GetShow() ) 
		return;


	for( int i=0; i<(int)m_vecUnitSlashTraceData.size(); i++ )
	{
		UnitSlashTraceData& slashTraceData = m_vecUnitSlashTraceData[i];

		if( NULL != slashTraceData.m_pSlashTrace )
		{
			slashTraceData.m_pSlashTrace->GetMatrix().Move( m_pGameUnit->GetMatrix().GetPos() );
			slashTraceData.m_pSlashTrace->OnFrameMove( fElapsedTime );
		}
		
		if( NULL != slashTraceData.m_pSlashTraceTip )
		{
			slashTraceData.m_pSlashTraceTip->GetMatrix().Move( m_pGameUnit->GetMatrix().GetPos() );
			slashTraceData.m_pSlashTraceTip->OnFrameMove( fElapsedTime );
		}
		
		if( NULL != slashTraceData.m_pFrame_TRACE_START &&
			NULL != slashTraceData.m_pFrame_TRACE_END )
		{
			std::map< int, UnitSlashTraceTiming >::iterator it = m_mapSlashTraceTiming.find( i );
			if( it != m_mapSlashTraceTiming.end() )
			{
				UnitSlashTraceTiming& timing = it->second;
				if( m_pUnitSkinAnim->GetNowAnimationTime() >= timing.m_fStartAnimationTime &&
					m_pUnitSkinAnim->GetNowAnimationTime() < timing.m_fEndAnimationTime )
				{	
					D3DXVECTOR3 up;
					GetFramePos( &up, slashTraceData.m_pFrame_TRACE_START );
					D3DXVECTOR3 down;
					GetFramePos( &down, slashTraceData.m_pFrame_TRACE_END );

					D3DXVECTOR3 dirTip	= down - up;
					D3DXVec3Normalize( &dirTip, &dirTip );
					D3DXVECTOR3 tipDown = up + dirTip * m_fSlashTraceTipWidth;

					slashTraceData.AddSlashTraceByType( timing.m_eSlashTraceType, up, down, tipDown, m_pGameUnit );
				}
// 				else
// 				{
// 					slashTraceData.AddSlashTraceByType( CKTDGSlashTrace::STT_NONE, up, down, tipDown, m_pGameUnit );
// 				}
			}
// 			else
// 			{
// 				slashTraceData.AddSlashTraceByType( CKTDGSlashTrace::STT_NONE, up, down, tipDown, m_pGameUnit );
// 			}
		}
	}
}

void CX2UnitSlashTraceManager::AddSlashTraceData( const wstring& topBoneName, const wstring& bottomBoneName, 
												 const D3DXCOLOR& coSlashTrace, const D3DXCOLOR& coSlashTraceHyper, 
												 const D3DXCOLOR& coSlashTraceTip, const D3DXCOLOR& coSlashTraceTipHyper, 
												 const int vertexNum, float fSlashTraceDisableTime /*= 0.4f*/, int iSlashTraceDrawCount /*= 1*/, const int vertexNumTip /*= 0*/, 
												 float fSlashTraceTipDisableTime /*= 0.4f*/, int iSlashTraceTipDrawCount /*= 1*/, 
												 bool bTexturedSlashTrace /*= false*/, const float fTextureStride /*= 0.f*/, const wstring& slashTraceTextureName /*= L"" */, 
												 CKTDGSlashTrace::CURVE_TYPE eCurveType /*= CKTDGSlashTrace::CT_SMOOTH_CURVE*/ )
{
	UnitSlashTraceData slashTraceData;
	slashTraceData.m_pFrame_TRACE_START = m_pUnitSkinAnim->GetCloneFrame( topBoneName.c_str() );
	slashTraceData.m_pFrame_TRACE_END = m_pUnitSkinAnim->GetCloneFrame( bottomBoneName.c_str() );

	slashTraceData.m_pSlashTrace = CKTDGSlashTrace::CreateSlashTrace( vertexNum, bTexturedSlashTrace, fTextureStride, slashTraceTextureName );
	slashTraceData.m_pSlashTrace->SetCenter( m_pUnitSkinAnim->GetAnimXSkinMesh()->GetCenter() );
	slashTraceData.m_pSlashTrace->SetBoundingRadius( m_pUnitSkinAnim->GetAnimXSkinMesh()->GetBoundingRadius() * 1.5f );
	slashTraceData.m_pSlashTrace->SetDisableTime( fSlashTraceDisableTime );
	slashTraceData.m_pSlashTrace->SetDrawCount( iSlashTraceDrawCount );
	slashTraceData.m_pSlashTrace->SetLayer( XL_EFFECT_0 );
	slashTraceData.m_pSlashTrace->SetCurveType( eCurveType );
	g_pKTDXApp->GetDGManager()->AddObjectChain( slashTraceData.m_pSlashTrace );


	if( 0 != vertexNumTip )
	{
		slashTraceData.m_pSlashTraceTip = CKTDGSlashTrace::CreateSlashTrace( vertexNumTip, bTexturedSlashTrace, fTextureStride, slashTraceTextureName );
		slashTraceData.m_pSlashTraceTip->SetCenter( m_pUnitSkinAnim->GetAnimXSkinMesh()->GetCenter() );
		slashTraceData.m_pSlashTraceTip->SetBoundingRadius( m_pUnitSkinAnim->GetAnimXSkinMesh()->GetBoundingRadius() * 1.5f );
		slashTraceData.m_pSlashTraceTip->SetDisableTime( fSlashTraceTipDisableTime );
		slashTraceData.m_pSlashTraceTip->SetDrawCount( iSlashTraceTipDrawCount );
		slashTraceData.m_pSlashTraceTip->SetLayer( XL_EFFECT_0 );
		slashTraceData.m_pSlashTraceTip->SetCurveType( eCurveType );
		g_pKTDXApp->GetDGManager()->AddObjectChain( slashTraceData.m_pSlashTraceTip );
	}

	slashTraceData.m_coSlashTrace			= coSlashTrace;
	slashTraceData.m_coSlashTraceHyper		= coSlashTraceHyper;
	slashTraceData.m_coSlashTraceTip		= coSlashTraceTip;
	slashTraceData.m_coSlashTraceTipHyper	= coSlashTraceTipHyper;

	m_vecUnitSlashTraceData.push_back( slashTraceData );


	if( false == m_bShow )
	{
		SetShow( false );
	}
}

//////////////////////////////////////////////////////////////////////////
void CX2UnitSlashTraceManager::UnitSlashTraceData::AddSlashTraceByType( CKTDGSlashTrace::SLASH_TRACE_TYPE eType, const D3DXVECTOR3& up, const D3DXVECTOR3& down, const D3DXVECTOR3& tipDown, 
																	   CX2GameUnit* pGameUnit )
{
	KTDXPROFILE();

	if( NULL == pGameUnit )
		return;

	if( false == pGameUnit->GetShowObject() )
		return;

	bool bHyperMode = false;
	if( pGameUnit->GetRemainHyperModeTime() > 0.f )
	{
		bHyperMode = true;
	}


	switch( eType )
	{
	default:
	case CKTDGSlashTrace::STT_DEFAULT:
		{
			if( true == bHyperMode )
			{
				if( NULL != m_pSlashTrace )
				{
					m_pSlashTrace->AddSlashData( up, down, m_coSlashTraceHyper );
				}
				if( NULL != m_pSlashTraceTip )
				{
					m_pSlashTraceTip->AddSlashData( up, tipDown, m_coSlashTraceTipHyper );
				}
			}
			else
			{
				if( NULL != m_pSlashTrace )
				{
					m_pSlashTrace->AddSlashData( up, down, m_coSlashTrace );
				}
				if( NULL != m_pSlashTraceTip )
				{
					m_pSlashTraceTip->AddSlashData( up, tipDown, m_coSlashTraceTip );
				}
			}
		} break;

	case CKTDGSlashTrace::STT_NONE:
		{
			// slash trace가 안보이는 동안에도 지속적으로 slash trace 데이터를 업데이트 해줘야 한다
			if( NULL != m_pSlashTrace )
			{
				m_pSlashTrace->AddSlashData( up, down, D3DXCOLOR( 0, 0, 0, 0 ) );
			}

			if( NULL != m_pSlashTraceTip )
			{
				m_pSlashTraceTip->AddSlashData( up, tipDown, D3DXCOLOR( 0, 0, 0, 0 ) );
			}

		} break;

	case CKTDGSlashTrace::STT_CONSTANT_WIDTH:
		{
			D3DXVECTOR3 vLineDir = pGameUnit->GetDirVector();
			D3DXVec3Normalize( &vLineDir, &vLineDir );

			if( true == bHyperMode )
			{
				if( NULL != m_pSlashTrace )
				{
					m_pSlashTrace->AddSlashDataConstantWidth( up, down, m_coSlashTraceHyper, vLineDir );
				}
			}
			else
			{
				if( NULL != m_pSlashTrace )
				{
					m_pSlashTrace->AddSlashDataConstantWidth( up, down, m_coSlashTrace, vLineDir );
				}
			}
		} break;

	case CKTDGSlashTrace::STT_CIRCULAR:
		{
			D3DXVECTOR3 vBonePos = pGameUnit->GetBonePos( L"Bip01_Spine" );
			D3DXVECTOR3 vLineDir = pGameUnit->GetDirVector();
			D3DXVec3Normalize( &vLineDir, &vLineDir );

			const float MAGIC_THRESHOLD = 50.f;
			if( true == bHyperMode )
			{
				if( NULL != m_pSlashTrace )
				{
					m_pSlashTrace->AddSlashDataCircular( vBonePos, up, down, m_coSlashTraceHyper, vLineDir, MAGIC_THRESHOLD, false );
				}
			}
			else
			{
				if( NULL != m_pSlashTrace )
				{
					m_pSlashTrace->AddSlashDataCircular( vBonePos, up, down, m_coSlashTrace, vLineDir, MAGIC_THRESHOLD, false );
				}
			}
		} break;
	} // switch
}



#endif UNIT_SLASH_TRACE_MANAGER_TEST