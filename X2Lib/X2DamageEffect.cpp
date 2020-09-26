#include "StdAfx.h"
#include ".\x2damageeffect.h"

CX2DamageEffect::CX2DamageEffect(void)
//{{ robobeg : 2008-10-28
: m_LuaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState(), 0, true )
//}} robobeg : 2008-10-28
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
, m_coInstanceHandleList( LIST_NUM, 128 )
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX
    m_bInCriticalLoop = false;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX
	m_fElapsedTime		= 0.0f;
	//m_iCurrUniqueIndex	 = 0;

#ifndef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	m_InstanceList.reserve( 64 );
	m_ReserveInstanceList.reserve( 64 );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

}

CX2DamageEffect::~CX2DamageEffect(void)
{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

    KInstanceHandleList::iterator iterNext;
    KInstanceHandleList::iterator iterEnd = m_coInstanceHandleList.end(LIST_LIVE);
    for( KInstanceHandleList::iterator iter = m_coInstanceHandleList.begin(LIST_LIVE);
        iter != iterEnd;
        iter = iterNext )
    {
        iterNext = iter; ++iterNext;
        CEffect* pCEffect =iter->m_pInstance;
        iter->m_pInstance = NULL;
        iter->m_eListType = LIST_FREE;
        SAFE_DELETE( pCEffect );
    }
    iterEnd = m_coInstanceHandleList.end(LIST_RESERVE);
    for( KInstanceHandleList::iterator iter = m_coInstanceHandleList.begin(LIST_RESERVE);
        iter != iterEnd;
        iter = iterNext )
    {
        iterNext = iter; ++iterNext;
        CEffect* pCEffect =iter->m_pInstance;
        iter->m_pInstance = NULL;
        iter->m_eListType = LIST_FREE;
        SAFE_DELETE( pCEffect )
    }
    m_coInstanceHandleList.splice_list( m_coInstanceHandleList.begin(LIST_FREE), LIST_LIVE );
    m_coInstanceHandleList.splice_list( m_coInstanceHandleList.begin(LIST_FREE), LIST_RESERVE );

#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	for( int i = 0; i < (int)m_InstanceList.size(); i++ )
	{
		CEffect* pCEffect = m_InstanceList[i];
		SAFE_DELETE( pCEffect );
	}
	m_InstanceList.clear();

	for( int i = 0; i < (int)m_ReserveInstanceList.size(); i++ )
	{
		CEffect* pCEffect = m_ReserveInstanceList[i];
		SAFE_DELETE( pCEffect );
	}
	m_ReserveInstanceList.clear();
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
}

void CX2DamageEffect::OpenScriptFile( const WCHAR* pFilename )
{

    if ( g_pKTDXApp->LoadAndDoMemory( &m_LuaManager, pFilename ) == false )
    {
        return;
    }
}

HRESULT CX2DamageEffect::OnFrameMove( double fTime, float fElapsedTime )
{
	KTDXPROFILE();

	m_fElapsedTime = fElapsedTime;

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    if ( m_coInstanceHandleList.empty( LIST_RESERVE ) == false )
    {
        KInstanceHandleList::iterator iterNext;
        KInstanceHandleList::iterator iterEnd = m_coInstanceHandleList.end( LIST_RESERVE );
        for( KInstanceHandleList::iterator iter = m_coInstanceHandleList.begin( LIST_RESERVE );
            iter != iterEnd;
            iter = iterNext )
        {
            iterNext = iter; ++iterNext;
            CEffect* pCEffect = iter->m_pInstance;
            if ( pCEffect != NULL && true == pCEffect->IsCreationDelayElapsed() == true )
            {
                iter->m_eListType = LIST_LIVE;
                m_coInstanceHandleList.splice( m_coInstanceHandleList.end( LIST_LIVE ), iter );
            }
        }
    }
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	for( int i = 0; i < (int)m_ReserveInstanceList.size(); i++ )
	{
		CEffect* pCEffect = m_ReserveInstanceList[i];
		if( NULL != pCEffect && true == pCEffect->IsCreationDelayElapsed() )
		{
			m_InstanceList.push_back( pCEffect );
			m_ReserveInstanceList.erase( m_ReserveInstanceList.begin() + i );
			i--;
		}
	}
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX
    ASSERT( m_bInCriticalLoop == false );
    m_bInCriticalLoop = true;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    KInstanceHandleList::iterator iterEnd = m_coInstanceHandleList.end(LIST_LIVE);
    for( KInstanceHandleList::iterator iter = m_coInstanceHandleList.begin( LIST_LIVE );
            iter != iterEnd;
            ++iter )
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	for( int i = 0; i < (int)m_InstanceList.size(); i++ )
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        CEffect* pCEffect = iter->m_pInstance;
        if ( pCEffect == NULL
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX
            || pCEffect->GetLive() == false
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX
            )
            continue;
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		CEffect* pCEffect = m_InstanceList[i];
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

		//{{ kimhc // 2009-10-20 // CEffect 전과 후에 BeginTable, EndTable을 호출 하므로 삭제,(일명 독구름 작업)
#ifdef	DAMAGE_EFFECT_WORK
			pCEffect->OnFrameMove( fTime, fElapsedTime );
#else	DAMAGE_EFFECT_WORK
		if( m_LuaManager.BeginTable( pCEffect->GetName() ) == true )
		{
			pCEffect->OnFrameMove( fTime, fElapsedTime );
			m_LuaManager.EndTable();
		}
#endif	DAMAGE_EFFECT_WORK		
		//{{ kimhc // 2009-10-20 // CEffect 전과 후에 BeginTable, EndTable을 호출 하므로 삭제,(일명 독구름 작업)
	}

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    iterEnd = m_coInstanceHandleList.end(LIST_LIVE);
    KInstanceHandleList::iterator iterNext;
    for( KInstanceHandleList::iterator iter = m_coInstanceHandleList.begin( LIST_LIVE );
            iter != iterEnd;
            iter = iterNext )
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	for( int i = 0; i < (int)m_InstanceList.size(); i++ )
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        iterNext = iter; ++iterNext;
        CEffect* pCEffect = iter->m_pInstance;
        if ( pCEffect == NULL )
        {
            iter->m_eListType = LIST_FREE;
            m_coInstanceHandleList.splice( m_coInstanceHandleList.begin( LIST_FREE ), iter );
            continue;        
        }
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		CEffect* pCEffect = m_InstanceList[i];
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		if( pCEffect->GetLive() == false )
		{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            iter->m_pInstance = NULL;
            iter->m_eListType = LIST_FREE;
            m_coInstanceHandleList.splice( m_coInstanceHandleList.begin( LIST_FREE ), iter );
            SAFE_DELETE( pCEffect );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			SAFE_DELETE( pCEffect );
			m_InstanceList.erase( m_InstanceList.begin() + i );
			i--;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		}
	}

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX
    m_bInCriticalLoop = false;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX

	return S_OK;
}



CX2DamageEffect::CEffect* CX2DamageEffect::CreateInstanceParabolic_LUA( CX2GameUnit* pGameUnit, const char* pName, D3DXVECTOR3 vPos, 
																	   D3DXVECTOR3 vTargetPos, D3DXVECTOR3 vAcceleration, float fTimeToReachTarget, float fTimeToLive )
{
	// 이 함수는 몬스터만 사용해야 합니다. power rate 에 기존 코드와의 호환성 유지를 위해 1이 더해지기 때문입니다.
	if( NULL != pGameUnit )
	{
		ASSERT( CX2GameUnit::GUT_NPC == pGameUnit->GetGameUnitType() );
	}


	wstring wstrName = L"";
	ConvertUtf8ToWCHAR( wstrName, pName );


	// note!! 기존에 powerrate = 2.f를 기준으로 모든 몬스터의 projectile 공격력이 정해져 있어서 그대로 간다.
	float fPowerRate = 1.f + 1.f;	
	if( NULL != pGameUnit )
	{
		fPowerRate = pGameUnit->GetPowerRate() + 1.f;
	}

	return CreateInstanceParabolic( pGameUnit, wstrName.c_str(), fPowerRate, vPos, vTargetPos, vAcceleration, fTimeToReachTarget, fTimeToLive );
}



CX2DamageEffect::CEffect*  CX2DamageEffect::CreateInstance_LUA( CX2GameUnit* pGameUnit, const char* pName, D3DXVECTOR3 pos, float fLandPos )
{

	// 이 함수는 몬스터만 사용해야 합니다. power rate 에 기존 코드와의 호환성 유지를 위해 1이 더해지기 때문입니다.
	if( NULL != pGameUnit )
	{
		ASSERT( CX2GameUnit::GUT_NPC == pGameUnit->GetGameUnitType() );
	}


	wstring wstrName = L"";
	ConvertUtf8ToWCHAR( wstrName, pName );


	// note!! 기존에 powerrate = 2.f를 기준으로 모든 몬스터의 projectile 공격력이 정해져 있어서 그대로 간다.
	float fPowerRate = 1.f + 1.f;	
	if( NULL != pGameUnit )
	{
		fPowerRate = pGameUnit->GetPowerRate() + 1.f;
	}
	
	return CreateInstance( 
		pGameUnit, wstrName.c_str(), fPowerRate, pos,
		pGameUnit->GetRotateDegree(),
		pGameUnit->GetRotateDegree(), fLandPos, false );
}

#ifdef CREATEINSTANCE_WITH_LIFETIME_IN_LUA
CX2DamageEffect::CEffect*  CX2DamageEffect::CreateInstanceWithLifeTime_LUA( CX2GameUnit* pGameUnit, const char* pName, D3DXVECTOR3 pos, float fLandPos, float fLifeTimeRate  )
{
	// 이 함수는 몬스터만 사용해야 합니다. power rate 에 기존 코드와의 호환성 유지를 위해 1이 더해지기 때문입니다.
	if( NULL != pGameUnit )
	{
		ASSERT( CX2GameUnit::GUT_NPC == pGameUnit->GetGameUnitType() );
	}

	wstring wstrName = L"";
	ConvertUtf8ToWCHAR( wstrName, pName );

	// note!! 기존에 powerrate = 2.f를 기준으로 모든 몬스터의 projectile 공격력이 정해져 있어서 그대로 간다.
	float fPowerRate = 1.f + 1.f;	
	if( NULL != pGameUnit )
	{
		fPowerRate = pGameUnit->GetPowerRate() + 1.f;
	}

	return CreateInstance( 
		pGameUnit, wstrName.c_str(), fPowerRate, pos,
		pGameUnit->GetRotateDegree(),
		pGameUnit->GetRotateDegree(), fLandPos, false, -1.f, 1.f, fLifeTimeRate  );
}
#endif //CREATEINSTANCE_WITH_LIFETIME_IN_LUA

CX2DamageEffect::CEffect*  CX2DamageEffect::CreateInstance_LUA2( CX2GameUnit* pGameUnit, const char* pName, D3DXVECTOR3 pos, float fLandPos, D3DXVECTOR3 vRot )
{

	// 이 함수는 몬스터만 사용해야 합니다. power rate 에 기존 코드와의 호환성 유지를 위해 1이 더해지기 때문입니다.
	if( NULL != pGameUnit )
	{
		ASSERT( CX2GameUnit::GUT_NPC == pGameUnit->GetGameUnitType() );
	}


	wstring wstrName = L"";
	ConvertUtf8ToWCHAR( wstrName, pName );

	// note!! 기존에 powerrate = 2.f를 기준으로 모든 몬스터의 projectile 공격력이 정해져 있어서 그대로 간다.
	float fPowerRate = 1.f + 1.f;	
	if( NULL != pGameUnit )
	{
		fPowerRate = pGameUnit->GetPowerRate() + 1.f;
	}
	

	return CreateInstance( 
		pGameUnit, wstrName.c_str(), fPowerRate, pos,
		vRot,
		vRot, fLandPos, false );
}



CX2DamageEffect::CEffect*  CX2DamageEffect::CreateInstance_LUA3 (CX2GameUnit* pGameUnit, const char* pName, D3DXVECTOR3 pos, float fLandPos, D3DXVECTOR3 vRot, float fReserveTime )
{

	// 이 함수는 몬스터만 사용해야 합니다. power rate 에 기존 코드와의 호환성 유지를 위해 1이 더해지기 때문입니다.
	if( NULL != pGameUnit )
	{
		ASSERT( CX2GameUnit::GUT_NPC == pGameUnit->GetGameUnitType() );
	}


	wstring wstrName = L"";
	ConvertUtf8ToWCHAR( wstrName, pName );

	// note!! 기존에 powerrate = 2.f를 기준으로 모든 몬스터의 projectile 공격력이 정해져 있어서 그대로 간다.
	float fPowerRate = 1.f + 1.f;	
	if( NULL != pGameUnit )
	{
		fPowerRate = pGameUnit->GetPowerRate() + 1.f;
	}


	return CreateInstance( 
		pGameUnit, wstrName.c_str(), fPowerRate, pos,
		vRot,
		vRot, fLandPos, true, fReserveTime  );
}

#ifdef SERV_PET_SYSTEM
CX2DamageEffect::CEffect*  CX2DamageEffect::CreateInstanceByPet_LUA( CX2GameUnit* pGameUnit, const char* pName, D3DXVECTOR3 pos, float fLandPos, D3DXVECTOR3 vRot )
{
	wstring wstrName = L"";
	ConvertUtf8ToWCHAR( wstrName, pName );

	// note!! 기존에 powerrate = 2.f를 기준으로 모든 몬스터의 projectile 공격력이 정해져 있어서 그대로 간다.
	float fPowerRate = 0.f;
	fPowerRate = 1.f;	
	if( pGameUnit->GetRemainHyperModeTime() > 0.f )
		fPowerRate = 1.2f;

	float fPowerRatePet = 1.f;
	// 펫 친밀도가 70%이상일 경우 배율 증가
	if( pGameUnit->GetGameUnitType() == CX2GameUnit::GUT_USER )
	{
		CX2GUUser *pMaster = (CX2GUUser *)pGameUnit;		
		CX2PET *pPet = g_pData->GetPetManager()->GetPet( pMaster->GetUnitUID() );
		
		if( pPet != NULL && pPet->GetNowIntimacy() >= 0.7f )
			fPowerRatePet = ( pPet->GetNowIntimacy() - 0.6f + 1.f );
	}

	return CreateInstance( 
		pGameUnit, wstrName.c_str(), fPowerRate * fPowerRatePet, pos,
		vRot,
		vRot, fLandPos, false 
#ifdef PET_SKILL_EFFECT_LINE_MAP
		, -1.f, -1.f, 1.f, -1, -1.f, 0, -1.f, false, true
#endif //PET_SKILL_EFFECT_LINE_MAP		
		);
}
#endif

CKTDGXMeshPlayer::CXMeshInstance* CX2DamageEffect::CEffect::GetMainEffect()
{
	if( NULL == g_pX2Game ||
		NULL == g_pX2Game->GetMajorXMeshPlayer() )
		return NULL;

	if( INVALID_MESH_INSTANCE_HANDLE == m_hMeshInstHandle )
		return NULL;

	CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->GetMeshInstance( m_hMeshInstHandle );
	return pMeshInst;
}


void CX2DamageEffect::DestroyAllInstances()
{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX
    if ( IsInCriticalLoop() == true )
    {
        KInstanceHandleList::iterator iterEnd = m_coInstanceHandleList.end(LIST_LIVE);
        for( KInstanceHandleList::iterator iter = m_coInstanceHandleList.begin(LIST_LIVE);
            iter != iterEnd;
            ++iter )
        {
            CEffect* pCEffect =iter->m_pInstance;
            if ( pCEffect != NULL )
                pCEffect->_SetDead();
        }
        iterEnd = m_coInstanceHandleList.end(LIST_RESERVE);
        for( KInstanceHandleList::iterator iter = m_coInstanceHandleList.begin(LIST_RESERVE);
            iter != iterEnd;
            ++iter )
        {
            CEffect* pCEffect =iter->m_pInstance;
            if ( pCEffect != NULL )
                pCEffect->_SetDead();
        }
    }
    else
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX
    {
        KInstanceHandleList::iterator iterNext;
        KInstanceHandleList::iterator iterEnd = m_coInstanceHandleList.end(LIST_LIVE);
        for( KInstanceHandleList::iterator iter = m_coInstanceHandleList.begin(LIST_LIVE);
            iter != iterEnd;
            iter = iterNext )
        {
            iterNext = iter; ++iterNext;
            CEffect* pCEffect =iter->m_pInstance;
            iter->m_pInstance = NULL;
            iter->m_eListType = LIST_FREE;
            SAFE_DELETE( pCEffect );
        }
        iterEnd = m_coInstanceHandleList.end(LIST_RESERVE);
        for( KInstanceHandleList::iterator iter = m_coInstanceHandleList.begin(LIST_RESERVE);
            iter != iterEnd;
            iter = iterNext )
        {
            iterNext = iter; ++iterNext;
            CEffect* pCEffect =iter->m_pInstance;
            iter->m_pInstance = NULL;
            iter->m_eListType = LIST_FREE;
            SAFE_DELETE( pCEffect )
        }
        m_coInstanceHandleList.splice_list( m_coInstanceHandleList.begin(LIST_FREE), LIST_LIVE );
        m_coInstanceHandleList.splice_list( m_coInstanceHandleList.begin(LIST_FREE), LIST_RESERVE );
    }

#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

	for( int i = 0; i < (int)m_InstanceList.size(); i++ )
	{
		CEffect* pCEffect = m_InstanceList[i];
		SAFE_DELETE( pCEffect );
	}
	m_InstanceList.clear();

#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

}

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

bool CX2DamageEffect::IsLiveInstance( CEffect* pInstance )
{
	if( NULL == pInstance )
		return false;
	CEffectHandle handle = pInstance->GetHandle();
    if ( handle == INVALID_DAMAGE_EFFECT_HANDLE )
        return false;

#ifdef  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
    DWORD   dwHandle = handle.GetValue();
#else   X2OPTIMIZE_HANDLE_VALIDITY_CHECK
    DWORD   dwHandle = static_cast<DWORD>( handle );
#endif  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
    WORD    wIndex = 0;
    WORD    wStamp = 0;
    DecomposeHandle( dwHandle, wIndex, wStamp );
    if ( wIndex >= m_coInstanceHandleList.storage_size() )
        return false;
    KInstanceHandleInfo& info = m_coInstanceHandleList.data( wIndex );
    if ( info.m_pInstance != pInstance || info.m_wStamp != wStamp || info.m_eListType != LIST_LIVE
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX
        || pInstance->GetLive() == false
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX
        )
        return false;
    return true;
}

bool CX2DamageEffect::IsLiveInstanceHandle( CEffectHandle handle )
{
    if ( handle == INVALID_DAMAGE_EFFECT_HANDLE )
        return false;

#ifdef  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
    DWORD   dwHandle = handle.GetValue();
#else   X2OPTIMIZE_HANDLE_VALIDITY_CHECK
    DWORD   dwHandle = static_cast<DWORD>( handle );
#endif  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
    WORD    wIndex = 0;
    WORD    wStamp = 0;
    DecomposeHandle( dwHandle, wIndex, wStamp );
    if ( wIndex >= m_coInstanceHandleList.storage_size() )
        return false;
    KInstanceHandleInfo& info = m_coInstanceHandleList.data( wIndex );
    if ( info.m_pInstance == NULL || info.m_wStamp != wStamp || info.m_eListType != LIST_LIVE
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX
        || info.m_pInstance->GetLive() == false
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX        
        )
        return false;
    return true;
}

CX2DamageEffect::CEffect*  CX2DamageEffect::GetInstance( CEffectHandle handle, bool bLiveOnly )
{
    if ( handle == INVALID_DAMAGE_EFFECT_HANDLE )
        return NULL;

#ifdef  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
    DWORD   dwHandle = handle.GetValue();
#else   X2OPTIMIZE_HANDLE_VALIDITY_CHECK
    DWORD   dwHandle = static_cast<DWORD>( handle );
#endif  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
    WORD    wIndex = 0;
    WORD    wStamp = 0;
    DecomposeHandle( dwHandle, wIndex, wStamp );
    if ( wIndex >= m_coInstanceHandleList.storage_size() )
        return NULL;
    KInstanceHandleInfo& info = m_coInstanceHandleList.data( wIndex );
    if ( info.m_pInstance == NULL || info.m_wStamp != wStamp || bLiveOnly == true && 
        ( info.m_eListType != LIST_LIVE
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX
            || info.m_pInstance->GetLive() == false
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX     
         )
        )
        return NULL;
    return info.m_pInstance;
}

void    CX2DamageEffect::DestroyInstance_LUA( CEffect* pInstance )
{
    DestroyInstance( pInstance, false );
}

void    CX2DamageEffect::DestroyInstanceHandle( CEffectHandle& hInstance, bool bSilently )
{
	if( hInstance == INVALID_DAMAGE_EFFECT_HANDLE )
		return;

#ifdef  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
    DWORD   dwHandle = hInstance.GetValue();
#else   X2OPTIMIZE_HANDLE_VALIDITY_CHECK
    DWORD   dwHandle = static_cast<DWORD>( hInstance );
#endif  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
    WORD    wIndex = 0;
    WORD    wStamp = 0;
    DecomposeHandle( dwHandle, wIndex, wStamp );
    if ( wIndex >= m_coInstanceHandleList.storage_size() )
    {
        hInstance = INVALID_DAMAGE_EFFECT_HANDLE;
        return;
    }
    KInstanceHandleInfo& info = m_coInstanceHandleList.data( wIndex );
    CEffect* pInstance = info.m_pInstance;
    if ( pInstance == NULL || info.m_wStamp != wStamp )
    {
        hInstance = INVALID_DAMAGE_EFFECT_HANDLE;
        return;
    }

#ifdef NEW_SKILL_2010_11
    if ( bSilently == false && info.m_eListType == LIST_LIVE
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX
        && pInstance->GetLive() == true
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX
		)
		pInstance->SetDieDamageEffect();
#endif NEW_SKILL_2010_11

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX
    if ( IsInCriticalLoop() == true )
    {
        pInstance->_SetDead();
        hInstance = INVALID_DAMAGE_EFFECT_HANDLE;
        return;
    }
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX

    info.m_pInstance = NULL;
    info.m_eListType = LIST_FREE;
    SAFE_DELETE( pInstance );
    m_coInstanceHandleList.splice( m_coInstanceHandleList.begin( LIST_FREE ), wIndex );
    hInstance = INVALID_DAMAGE_EFFECT_HANDLE;
}

#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
void CX2DamageEffect::DestroyInstance( CEffect*& pInstance, bool bSilently )
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
void CX2DamageEffect::DestroyInstance( CEffect* pInstance )
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
{
	if( NULL == pInstance )
		return;

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	CEffectHandle handle = pInstance->GetHandle();
    if ( handle == INVALID_DAMAGE_EFFECT_HANDLE )
    {
        pInstance = NULL;
        return;
    }

#ifdef  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
    DWORD   dwHandle = handle.GetValue();
#else   X2OPTIMIZE_HANDLE_VALIDITY_CHECK
    DWORD   dwHandle = static_cast<DWORD>( handle );
#endif  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
    WORD    wIndex = 0;
    WORD    wStamp = 0;
    DecomposeHandle( dwHandle, wIndex, wStamp );
    if ( wIndex >= m_coInstanceHandleList.storage_size() )
    {
        pInstance = NULL;
        return;
    }
    KInstanceHandleInfo& info = m_coInstanceHandleList.data( wIndex );
    if ( info.m_pInstance != pInstance || info.m_wStamp != wStamp )
    {
        pInstance = NULL;
        return;
    }

#ifdef NEW_SKILL_2010_11
    if ( info.m_eListType == LIST_LIVE
        && bSilently == false
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX
        && pInstance->GetLive() == true
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX
        )
	    pInstance->SetDieDamageEffect();
#endif NEW_SKILL_2010_11

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX
    if ( IsInCriticalLoop() == true )
    {
        pInstance->_SetDead();
        pInstance = NULL;
        return;
    }
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX

    info.m_pInstance = NULL;
    info.m_eListType = LIST_FREE;
    SAFE_DELETE( pInstance );
    m_coInstanceHandleList.splice( m_coInstanceHandleList.begin( LIST_FREE ), wIndex );
    pInstance = NULL;

#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

	for( int i = 0; i < (int)m_InstanceList.size(); i++ )
	{
		CEffect* pCEffect = m_InstanceList[i];
		if( pInstance == pCEffect )
		{
			//{{ JHKang / 강정훈 / 2010/11/24
#ifdef NEW_SKILL_2010_11
			pCEffect->SetDieDamageEffect();
#endif NEW_SKILL_2010_11
			//}} JHKang / 강정훈 / 2010/11/24

			SAFE_DELETE( pCEffect );
			m_InstanceList.erase( m_InstanceList.begin() + i );
			return;
		}
	}

#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

}

#ifdef EVE_ELECTRA

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

void CX2DamageEffect::DestroyInstanceHandleSilently( CEffectHandle& hInstance )
{
    DestroyInstanceHandle( hInstance, true );
}


void CX2DamageEffect::DestroyInstanceSilently( CEffect*& pInstance )
{
    DestroyInstance( pInstance, true );
}

#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

void CX2DamageEffect::DestroyInstanceSilently( CEffect*& pInstance )
{

	for( int i = 0; i < (int)m_InstanceList.size(); i++ )
	{
		CEffect* pCEffect = m_InstanceList[i];
		if( pInstance == pCEffect )
		{
			SAFE_DELETE( pCEffect );
			pInstance = NULL;
			m_InstanceList.erase( m_InstanceList.begin() + i );
			return;
		}
	}


}

#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

void CX2DamageEffect::AddLaserGroupHitUnit( int _iLaserGroupID, CX2DamageManager::HitUnit _hitUnit )
{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

    KInstanceHandleList::iterator iterEnd = m_coInstanceHandleList.end(LIST_LIVE);
    for( KInstanceHandleList::iterator iter = m_coInstanceHandleList.begin( LIST_LIVE );
        iter != iterEnd;
        ++iter )
    {
        CEffect* pCEffect = iter->m_pInstance;
        if ( pCEffect != NULL && 
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX
            pCEffect->GetLive() == true &&
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX
            _iLaserGroupID == pCEffect->GetLaserGroupID() )
        {
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX
            // 이 호출은 side effect 없음
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX
			g_pData->GetDamageManager()->AddHitUnit( pCEffect->GetDamageData(), _hitUnit);

			//int tmp = i;
        }
    }

#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

	for( int i = 0; i < (int)m_InstanceList.size(); i++ )
	{
		CEffect* pCEffect = m_InstanceList[i];
		if( _iLaserGroupID == pCEffect->GetLaserGroupID() )
		{
			g_pData->GetDamageManager()->AddHitUnit( pCEffect->GetDamageData(), _hitUnit);

			int tmp = i;
		}
	}

#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
}
#endif EVE_ELECTRA



CX2DamageEffect::CEffect* CX2DamageEffect::CreateInstanceParabolic( CX2GameUnit* pGameUnit, const WCHAR* pName, float fPowerRate, 
																   const D3DXVECTOR3& vPos, const D3DXVECTOR3& vTargetPos, const D3DXVECTOR3& vAcceleration, 
																   const float fTimeToReachTarget, const float fTimeToLive, const int iMeshPlayerIndex /*= 0*/ )
{


	ASSERT( NULL != pName );
	if( NULL == pName )
	{
#ifdef NO_MORE_ERROR_TEST
		InHouseLog( "이름이 없는 데미지 이펙트 생성 시도" );
#endif NO_MORE_ERROR_TEST

		return NULL;
	}

	CEffect* pCEffect = NULL;

    char aszBufUtf8[255] = {0,};
    WideCharToMultiByte( CP_UTF8, 0, pName, -1, aszBufUtf8, 255, NULL, NULL );

	if( m_LuaManager.BeginTable( aszBufUtf8 ) == true )
	{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        KInstanceHandleList::iterator iterEmpty = m_coInstanceHandleList.begin(LIST_FREE);
        if ( iterEmpty == m_coInstanceHandleList.end(LIST_FREE) )
        {
            if ( m_coInstanceHandleList.storage_size() >= 0x10000L )
            {
                return NULL;
            }
            m_coInstanceHandleList.push_back_default(LIST_FREE);
            iterEmpty = m_coInstanceHandleList.begin(LIST_FREE);
            iterEmpty->m_eListType = LIST_FREE;
            ASSERT( iterEmpty != m_coInstanceHandleList.end(LIST_FREE) );
        }
        WORD   wIndex = (WORD) iterEmpty.GetIndex();
        CEffectHandle   hHandle;
        DWORD   dwHandle = 0;
        do
        {
            ++iterEmpty->m_wStamp;
            dwHandle = ComposeHandle( wIndex, iterEmpty->m_wStamp );
#ifdef  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
            hHandle.SetValue( (int) dwHandle );
#else   X2OPTIMIZE_HANDLE_VALIDITY_CHECK
            hHandle = (int) dwHandle;
#endif  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
        } while ( hHandle == INVALID_DAMAGE_EFFECT_HANDLE );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		pCEffect = new CEffect( this, &m_LuaManager, pGameUnit, pName, fPowerRate, 
			vPos, vTargetPos, vAcceleration, 0.f, -1.f, fTimeToReachTarget, fTimeToLive, iMeshPlayerIndex
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            , hHandle
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            );

		if( pCEffect->GetLive() == false )
		{
			SAFE_DELETE( pCEffect );
			m_LuaManager.EndTable();
			return NULL;
		}
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        iterEmpty->m_pInstance = pCEffect;
        iterEmpty->m_eListType = LIST_LIVE;
        m_coInstanceHandleList.splice( m_coInstanceHandleList.end( LIST_LIVE ), iterEmpty );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		m_InstanceList.push_back( pCEffect );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		m_LuaManager.EndTable();
	}
	else
		return NULL;

	//PostProcessCreateInstance( pCEffect, fForceDownValueRate, fLifeTimeRate, iAddHitCount, fHitGap );

	return pCEffect;

}

														  

#ifdef EVE_ELECTRA
CX2DamageEffect::CEffect* CX2DamageEffect::CreateInstance( CX2GameUnit* pGameUnit, const WCHAR* pName, float fPowerRate, 
															float x, float y, float z, 
															float angleXDegree, float angleYDegree, float angleZDegree,
															float moveAxisXDegree, float moveAxisYDegree, float moveAxisZDegree,
															float fLandPos /*= 0*/, bool bReserve /*= false*/, 															
															float fReserveTime /*= -1.f*/, 
															float fForceDownValueRate /*= 1.f*/, float fLifeTimeRate /*= 1.f*/, 
															int iAddHitCount /*= -1*/, float fHitGap /*= -1.f*/, const int iMeshPlayerIndex /*= 0*/, 
															float fHitAddMp /*= -1.f*/, bool bInsertFront /*= false */
#ifdef PET_SKILL_EFFECT_LINE_MAP
															, bool bPetSkillEffect /* = false*/
#endif //PET_SKILL_EFFECT_LINE_MAP
															)
#else
CX2DamageEffect::CEffect* CX2DamageEffect::CreateInstance( CX2GameUnit* pGameUnit, const WCHAR* pName, float fPowerRate, 
	float x, float y, float z, 
	float angleXDegree, float angleYDegree, float angleZDegree,
	float moveAxisXDegree, float moveAxisYDegree, float moveAxisZDegree,
	float fLandPos /*= 0*/, bool bReserve /*= false*/, float fReserveTime /*= -1.f*/, 
	float fForceDownValueRate /*= 1.f*/, float fLifeTimeRate /*= 1.f*/, 
	int iAddHitCount /*= -1*/, float fHitGap /*= -1.f*/, const int iMeshPlayerIndex /*= 0*/, 
	float fHitAddMp /*= -1.f*/ )
#endif EVE_ELECTRA
{

	ASSERT( NULL != pName );
	if( NULL == pName )
	{
#ifdef NO_MORE_ERROR_TEST
		InHouseLog( "이름이 없는 데미지 이펙트 생성 시도" );
#endif NO_MORE_ERROR_TEST

		return NULL;
	}




	CEffect* pCEffect = NULL;

    char aszBufUtf8[255] = {0,};
    WideCharToMultiByte( CP_UTF8, 0, pName, -1, aszBufUtf8, 255, NULL, NULL );

	if( m_LuaManager.BeginTable( aszBufUtf8 ) == true )
	{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        KInstanceHandleList::iterator iterEmpty = m_coInstanceHandleList.begin(LIST_FREE);
        if ( iterEmpty == m_coInstanceHandleList.end(LIST_FREE) )
        {
            if ( m_coInstanceHandleList.storage_size() >= 0x10000L )
            {
                return NULL;
            }
            m_coInstanceHandleList.push_back_default(LIST_FREE);
            iterEmpty = m_coInstanceHandleList.begin(LIST_FREE);
            iterEmpty->m_eListType = LIST_FREE;
            ASSERT( iterEmpty != m_coInstanceHandleList.end(LIST_FREE) );
        }
        WORD   wIndex = (WORD) iterEmpty.GetIndex();
        CEffectHandle  hHandle;
        DWORD   dwHandle = 0;
        do
        {
            ++iterEmpty->m_wStamp;
            dwHandle = ComposeHandle( wIndex, iterEmpty->m_wStamp );
#ifdef  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
            hHandle.SetValue( (int) dwHandle );
#else   X2OPTIMIZE_HANDLE_VALIDITY_CHECK
            hHandle = (int) dwHandle;
#endif  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
        } while ( hHandle == INVALID_DAMAGE_EFFECT_HANDLE );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

		pCEffect = new CEffect( this, &m_LuaManager, pGameUnit, pName, fPowerRate, D3DXVECTOR3(x,y,z),
			D3DXVECTOR3(angleXDegree,angleYDegree,angleZDegree),
			D3DXVECTOR3(moveAxisXDegree,moveAxisYDegree,moveAxisZDegree),
			fLandPos, fReserveTime, 0.f, 0.f, iMeshPlayerIndex, 
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            hHandle,
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            fHitAddMp 
#ifdef PET_SKILL_EFFECT_LINE_MAP
			, bPetSkillEffect
#endif //PET_SKILL_EFFECT_LINE_MAP
			);

#ifdef FIX_EFFECT_REFLECT
		if ( NULL == pCEffect )
			return NULL;
#endif

		if( pCEffect->GetLive() == false )
		{
			SAFE_DELETE( pCEffect );
			m_LuaManager.EndTable();
			return NULL;
		}

#ifdef GIANT_UNIT_GIANT_EFFECT_TEST
		
		if( NULL != pGameUnit && pCEffect->GetApplyScaleByUnit() == true )
		{
			if( pGameUnit->GetScaleByUnit() != D3DXVECTOR3( 1, 1, 1 ) )
			{
#ifdef VERIFY_STAT_BY_BUFF
				pCEffect->SetScaleByUnit( pGameUnit->GetVec3ScaleByUnit() );
#else	VERIFY_STAT_BY_BUFF
				pCEffect->SetScaleByUnit( pGameUnit->GetScaleByUnit() );
#endif // VERIFY_STAT_BY_BUFF

			}
		}

#endif GIANT_UNIT_GIANT_EFFECT_TEST

		if( bReserve == true )
		{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            iterEmpty->m_pInstance = pCEffect;
            iterEmpty->m_eListType = LIST_RESERVE;
            m_coInstanceHandleList.splice( m_coInstanceHandleList.end( LIST_RESERVE ), iterEmpty );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			m_ReserveInstanceList.push_back( pCEffect );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            CKTDGXMeshPlayer::CXMeshInstance *pMeshInstance = ( pCEffect != NULL ) ? pCEffect->GetMainEffect() : NULL;
			if( NULL != pMeshInstance &&
				fReserveTime > 0.f )
			{
				pMeshInstance->SetDelayTime( fReserveTime );
			}
		}
		else
		{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            iterEmpty->m_pInstance = pCEffect;
            iterEmpty->m_eListType = LIST_LIVE;
#ifdef  EVE_ELECTRA
            if ( bInsertFront == true )
                m_coInstanceHandleList.splice( m_coInstanceHandleList.begin( LIST_LIVE ), iterEmpty );
            else
                m_coInstanceHandleList.splice( m_coInstanceHandleList.end( LIST_LIVE ), iterEmpty );
#else   EVE_ELECTRA
            m_coInstanceHandleList.splice( m_coInstanceHandleList.end( LIST_LIVE ), iterEmpty );
#endif  EVE_ELECTRA

#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

#ifdef EVE_ELECTRA
			if( bInsertFront == true)
			{
				vector<CEffect*> vecTempInstanceList;

				vecTempInstanceList.push_back(pCEffect);
				for( UINT i=0; i< m_InstanceList.size(); ++i )
				{
					vecTempInstanceList.push_back( m_InstanceList[i]);
				}
				m_InstanceList = vecTempInstanceList;
			}
			else
				m_InstanceList.push_back( pCEffect );
#else
			m_InstanceList.push_back( pCEffect );
#endif EVE_ELECTRA

#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		}

		m_LuaManager.EndTable();
	}
	else
		return NULL;


	PostProcessCreateInstance( pCEffect, fForceDownValueRate, fLifeTimeRate, iAddHitCount, fHitGap );


	CKTDGXMeshPlayer::CXMeshInstance* pMeshInstance = pCEffect->GetMainEffect();
	if( NULL != pMeshInstance )
	{
		if( true == pMeshInstance->GetIsSetLandHeightOnStart() )
		{
			if( NULL != pGameUnit )
			{
				pMeshInstance->SetLandPosition( pGameUnit->GetLandPos().y );
#ifdef RAVEN_WEAPON_TAKER
				pCEffect->SetLandPositionOnLineMap( pGameUnit->GetLandPos() );
#endif RAVEN_WEAPON_TAKER
			}
		}
	}

#ifdef EFFECT_USE_LOG
	if( NULL != g_pMain->GetGameEdit() &&
		CX2GameEdit::ELL_EFFECT_LOG_1 <= g_pMain->GetGameEdit()->GetEffectLogLevel() )
	{
		dbg::clog << L"(데미지이펙트)" << pName << dbg::endl;
		KLOG("EffectLog.txt")  << L"DamageEffect : " << pName << fileout;
	}
#endif // EFFECT_USE_LOG


	return pCEffect;
}


void CX2DamageEffect::PostProcessCreateInstance( CEffect* pCEffect, float fForceDownValueRate, float fLifeTimeRate, int iAddHitCount, float fHitGap )
{
	if( NULL != pCEffect )
	{
		if( 1.f != fForceDownValueRate )
		{
			pCEffect->GetDamageData().fForceDownValue *= fForceDownValueRate;
#ifdef INHERIT_FORCEDOWNRATE
			pCEffect->SetForceDownRate(fForceDownValueRate);
#endif INHERIT_FORCEDOWNRATE
		}

		if( 1.f != fLifeTimeRate )
		{
            CKTDGXMeshPlayer::CXMeshInstance* pMeshInstance = pCEffect->GetMainEffect();
			if( NULL != pMeshInstance )
			{
				pMeshInstance->SetMaxLifeTime( pMeshInstance->GetMaxLifeTime() * fLifeTimeRate );
			}

			pCEffect->SetPassiveparticleTriggerTimeRate( fLifeTimeRate );
		}

		if( iAddHitCount > 0 && fHitGap <= 0.f )
		{
			fHitGap = pCEffect->GetDamageData().fHitGap;
		}

		if( iAddHitCount > 0 && fHitGap >= 0.f )
		{
			int iNewHitCount = pCEffect->GetDamageTime() + iAddHitCount;
			pCEffect->SetDamageTime( iNewHitCount );
			pCEffect->GetDamageData().fHitGap = fHitGap;
			pCEffect->GetDamageData().bReAttack = true;
		}
	}
}


void CX2DamageEffect::UnitDeleteProcess( CX2GameUnit* pGameUnit )
{

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX
    if ( IsInCriticalLoop() == true )
    {
        KInstanceHandleList::iterator iterEnd = m_coInstanceHandleList.end(LIST_LIVE);
        for( KInstanceHandleIterator iter = m_coInstanceHandleList.begin( LIST_LIVE );
            iter != iterEnd; 
            ++iter )
        {
            CEffect* pCEffect = iter->m_pInstance;
            if ( pCEffect != NULL && pGameUnit == pCEffect->GetOwnerUnit() )
            {
                pCEffect->_SetDead();
            }
        }
        iterEnd = m_coInstanceHandleList.end( LIST_RESERVE );
        for( KInstanceHandleIterator iter = m_coInstanceHandleList.begin( LIST_RESERVE );
            iter != iterEnd; 
            ++iter )
        {
            CEffect* pCEffect = iter->m_pInstance;
            if ( pCEffect != NULL && pGameUnit == pCEffect->GetOwnerUnit() )
            {
                pCEffect->_SetDead();
            }
        }
    }
    else
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX
    {
        KInstanceHandleList::iterator iterNext;
        KInstanceHandleList::iterator iterEnd = m_coInstanceHandleList.end(LIST_LIVE);
        for( KInstanceHandleIterator iter = m_coInstanceHandleList.begin( LIST_LIVE );
            iter != iterEnd; 
            iter = iterNext )
        {
            iterNext = iter; ++iterNext;
            CEffect* pCEffect = iter->m_pInstance;
            if ( pCEffect == NULL )
            {
                iter->m_eListType = LIST_FREE;
                m_coInstanceHandleList.splice( m_coInstanceHandleList.begin( LIST_FREE ), iter );
            }
            else if ( pGameUnit == pCEffect->GetOwnerUnit() )
            {
                iter->m_pInstance = NULL;
                iter->m_eListType = LIST_FREE;
                m_coInstanceHandleList.splice( m_coInstanceHandleList.begin( LIST_FREE ), iter );
                SAFE_DELETE( pCEffect );
            }
        }
        iterEnd = m_coInstanceHandleList.end( LIST_RESERVE );
        for( KInstanceHandleIterator iter = m_coInstanceHandleList.begin( LIST_RESERVE );
            iter != iterEnd; 
            iter = iterNext )
        {
            iterNext = iter; ++iterNext;
            CEffect* pCEffect = iter->m_pInstance;
            if ( pCEffect == NULL )
            {
                iter->m_eListType = LIST_FREE;
                m_coInstanceHandleList.splice( m_coInstanceHandleList.begin( LIST_FREE ), iter );
            }
            else if ( pGameUnit == pCEffect->GetOwnerUnit() )
            {
                iter->m_pInstance = NULL;
                iter->m_eListType = LIST_FREE;
                m_coInstanceHandleList.splice( m_coInstanceHandleList.begin( LIST_FREE ), iter );
                SAFE_DELETE( pCEffect );
            }
        }
    }


#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

	for( int i = 0; i < (int)m_InstanceList.size(); i++ )
	{
		CEffect* pCEffect = m_InstanceList[i];
		
		if ( pCEffect == NULL )
			continue;

		if( pGameUnit == pCEffect->GetOwnerUnit() )
		{
			SAFE_DELETE( pCEffect );
			m_InstanceList.erase( m_InstanceList.begin() + i );
			i--;
		}
	}

#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
}






/////////////////////////////////////////////////////////////////////////////
// CEffect
/////////////////////////////////////////////////////////////////////////////

CX2DamageEffect::CEffect::CEffect( CX2DamageEffect* pManager, KLuaManager* pLuaManager, CX2GameUnit* pGameUnit, 
								  const WCHAR* pName, float fPowerRate, D3DXVECTOR3 pos, D3DXVECTOR3 angleDegree, D3DXVECTOR3 moveAxisDegree, 
								  float fLandPos /*= 0.f*/, float fCreationDelayTime /*= -1.f*/, float fParabolicTimeToReachTarget /*= 0.f*/, float fParabolicTimeToLive /*= 0.f*/,
								  const int iMeshPlayerIndex /*= 0*/, 
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
                                  CEffectHandle  hHandle /*=INVALID_DAMAGE_EFFECT_HANDLE*/,
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
                                  float fHitAddMp /*=-1.f*/ 
#ifdef PET_SKILL_EFFECT_LINE_MAP
								  , bool bPetSkillEffect /*= false*/
#endif //PET_SKILL_EFFECT_LINE_MAP
								  )
#ifdef GIANT_UNIT_GIANT_EFFECT_TEST
: m_vHitDamageScaleByUnit( 1, 1, 1 )
, m_vDieDamageScaleByUnit( 1, 1, 1 )
#endif GIANT_UNIT_GIANT_EFFECT_TEST
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
, m_hHandle( hHandle )
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
#ifdef STICKY_DAMAGE_EFFECT_TEST
, m_bEnableAttack( true )
, m_bTraceAttackedUnit( false )
, m_fLifeTimeAfterAttack( -1.f )
, m_pTraceUnit( NULL )
, m_vTracePosition( 0, 0, 0 )
#endif STICKY_DAMAGE_EFFECT_TEST

, m_bFollowLineNew( false )

#ifdef FIX_EFFECT_REFLECT
, m_iReflectCount( 2 )
#endif

#ifdef FIX_AFTER_IMAGE
, m_pAfterImage( NULL )
, m_fDurationTime( 0.f )
#endif

#ifdef SERV_RENA_NIGHT_WATCHER
, m_iIndex( -1 )					/// 해당 DamageEffect의 Index ( 지연 폭발용 DamageEffect 인식 )
, m_iType( DET_NONE )			/// 해당 DamageEffect의 Type  ( 현재 화살의 DamageEffect가 어떤 것인지 인식 )
, m_iOrderNum( -1 )				/// 폭발의 화살 발사 순서
, m_fAddDieEffectDamage( 0.f )	/// 해당 데미지 이펙트의 fPhysic, fMagic에 추가되는 수치
#endif SERV_RENA_NIGHT_WATCHER
#ifdef SERV_EVE_BATTLE_SERAPH
, m_bCheckSpectro( false )
#endif
, m_bFollowLineRight( false )
#ifdef NOTIFY_TO_OWNER_UNIT_WHEN_DAMAGE_EFFECT_DIE
, m_bShouldNotifyToOwnerUnitWhenDie( false )
#endif // NOTIFY_TO_OWNER_UNIT_WHEN_DAMAGE_EFFECT_DIE
{
	KTDXPROFILE();

#ifdef FIX_AFTER_IMAGE
	INIT_VECTOR2( m_afterImageTime, -1.f, -1.f );
#endif

#ifdef RAVEN_WEAPON_TAKER
	m_vLandPositionOnLineMap = D3DXVECTOR3( 0, 0, 0 );
#endif RAVEN_WEAPON_TAKER

	m_bFirstFrame			= true;
	m_bLive					= true;
	m_pManager				= pManager;
	m_pLuaManager			= pLuaManager;
	m_Pos					= pos;
	m_BeforePos				= D3DXVECTOR3(0,0,0);
	m_AngleDegree			= angleDegree;
	m_MoveAxisDegree		= moveAxisDegree;
	m_fLandPos				= fLandPos;
	m_fPowerRate			= fPowerRate;

	m_bUseDXMatrix			= false;

	m_DamageData.attackerType		= CX2DamageManager::AT_EFFECT;
	m_DamageData.m_bMeleeAttack		= false;	/// 데미지이펙트는 기본적으로 원거리 공격으로 셋팅
	m_DamageData.optrAttackerGameUnit		= pGameUnit;
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
    m_DamageData.hAttackerEffect	= hHandle;
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	m_DamageData.pAttackerEffect	= this;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

	m_Name					= pName;

	if( NULL != g_pX2Game && NULL != pGameUnit )
	{
		m_optrOwnerUnit	= pGameUnit;
		if( null != m_optrOwnerUnit )
			m_bFollowLineRight	= m_optrOwnerUnit->GetIsRight();
#ifdef PET_SKILL_EFFECT_LINE_MAP
		if ( pGameUnit != NULL && bPetSkillEffect == true )
		{
			CX2GUUser *pMaster = (CX2GUUser *)pGameUnit;
			if ( pMaster != NULL && g_pData != NULL && g_pData->GetPetManager() != NULL )
			{	
				CX2PET *pPet = g_pData->GetPetManager()->GetPet( pMaster->GetUnitUID() );
				if ( pPet != NULL )
					m_bFollowLineRight = pPet->GetIsRight();
			}

		}
#endif //PET_SKILL_EFFECT_LINE_MAP
	}
	else
	{
		m_optrOwnerUnit.Reset();
	}
	
	m_hMeshInstHandle		= INVALID_MESH_INSTANCE_HANDLE;

	m_DamageTime						= 1;
	m_DamageTimeNow						= 0;
#ifdef CHUNG_SECOND_CLASS_CHANGE
	m_PrevDamageTime					= 0;
#endif
	m_bAttackedByUnit					= false;
	m_bAttackedByEffect					= false;
	m_bReflect							= false;
	m_fDamageDataChangeTime				= -1.0f;
	m_fDamageDataChangeTimeGlobalTime	= -1.0f;
		

	m_fCreationDelayTime				= fCreationDelayTime;

#ifdef NEW_SKILL_TREE
	m_vPassiveParticleScale	= D3DXVECTOR3(1.f,1.f,1.f);
	m_vHitDamageScale		= D3DXVECTOR3(1.f,1.f,1.f);
	m_vDieDamageScale		= D3DXVECTOR3(1.f,1.f,1.f);

	m_fPassiveParticleTriggerTimeRate = -1.f;
	m_bReflected			= false;
#endif NEW_SKILL_TREE

#ifdef SKILL_CASH_10_TEST
	m_fReactAnimSpeed	= 0.f;
	m_bFollowOnLine		= false;
#endif

#ifdef RAVEN_WEAPON_TAKER
	m_bMustOnLine		= false;
#endif RAVEN_WEAPON_TAKER

	//{{ kimhc // 2010.2.3 //	이브 2차 전직
#ifdef	EVE_SECOND_CLASS_CHANGE
	m_vMoveAngle				= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );	// 움직이는 각도 Rotate에 plus 함
#endif	EVE_SECOND_CLASS_CHANGE
	//}} kimhc // 2010.2.3 //	이브 2차 전직

	m_bApplyScaleByUnit = true;
#ifdef INHERIT_FORCEDOWNRATE
	m_fForceDownRate		= 1.f;
	m_bInheritForceDownRate	= false;
#endif INHERIT_FORCEDOWNRATE

#ifdef EVE_ELECTRA
	m_iLaserGroupID			= 0;
#endif EVE_ELECTRA

#ifdef DAMAGE_EFFECT_GROUP_DAMAGE
	m_bGroupDamage = false;			/// 데미지 이펙트 그룹 데미지 설정
#endif DAMAGE_EFFECT_GROUP_DAMAGE

#ifdef SERV_ADD_CHUNG_SHELLING_GUARDIAN
	m_bUseCannonBall = false;
#endif //SERV_ADD_CHUNG_SHELLING_GUARDIAN

	m_StartParticleDataList.reserve( 8 );
	m_PassiveParticleDataList.reserve( 8 );
	m_TimeParticleDataList.reserve( 8 );
	m_CameraDataList.reserve( 4 );
	m_ScreenDataList.reserve( 4 );
	m_SoundDataList.reserve( 4 );
	
	m_DamageEffectDataList.reserve( 4 );

	m_vecHitDamageEffect.reserve( 4 );
	m_vecDieDamageEffect.reserve( 4 );




	//자신의 테이블에서 메인 이펙트와 데미지 데이타, 기타 옵션들을 읽어온다.
	wstring mainEffName = L"";
	if( 0 == iMeshPlayerIndex )
	{
		PLUA_GET_VALUE( m_pLuaManager, "MAIN_EFFECT_NAME", mainEffName, L"" );
	}
	else
	{
		char tableName[32] = "";
		StringCchPrintfA( tableName, ARRAY_SIZE(tableName), "MAIN_EFFECT_NAME%d", iMeshPlayerIndex );
		PLUA_GET_VALUE( m_pLuaManager, tableName, mainEffName, L"" );
	}

	ASSERT( false == mainEffName.empty() );

	CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = NULL;
    if ( g_pX2Game != NULL )
    {
	    if( fParabolicTimeToLive > 0.f )
	    {
		    const D3DXVECTOR3 vTargetPos = m_AngleDegree;
		    const D3DXVECTOR3 vAcceleration = m_MoveAxisDegree;
		    pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->CreateInstanceParabolic( static_cast<CKTDGObject*>( m_optrOwnerUnit.GetObservable() ), mainEffName.c_str(), m_Pos, vTargetPos, vAcceleration, fParabolicTimeToReachTarget, fParabolicTimeToLive );
	    }
	    else
	    {
		    pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->CreateInstance( static_cast<CKTDGObject*>( m_optrOwnerUnit.GetObservable() ),  mainEffName.c_str(), m_Pos, m_AngleDegree, m_MoveAxisDegree );
	    }
    }


	if( NULL == pMeshInst )
	{
		m_bLive = false;
	}
	else
	{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
        pMeshInst->SetPerFrameSimulation( true );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
#ifdef RIDING_SYSTEM
		float fLiveTime = -1.f;
		PLUA_GET_VALUE( m_pLuaManager, "MAIN_EFFECT_TIME", fLiveTime, -1.f );
		if ( fLiveTime > -1.f  )
			pMeshInst->SetMaxLifeTime( fLiveTime );
#endif //RIDING_SYSTEM
		m_hMeshInstHandle = pMeshInst->GetHandle();
		pMeshInst->SetLandPosition( fLandPos );

		m_vMainEffectAxisAngleDegree = pMeshInst->GetMatrix().GetRotateDegree();
		m_vMainEffectPosition = pMeshInst->GetMatrix().GetPos();


		bool bNotCull = false;
		PLUA_GET_VALUE( m_pLuaManager, "NOT_CULL", bNotCull, false );
		if( bNotCull == true )
			pMeshInst->SetBoundingRadius( 0.0f );

	}

#ifdef SERV_ADD_CHUNG_SHELLING_GUARDIAN
	PLUA_GET_VALUE( m_pLuaManager, "USE_CANNONBALL",						m_bUseCannonBall,		false );
	float fUseCannonBallDamageRel = 1.f;
#endif //SERV_ADD_CHUNG_SHELLING_GUARDIAN

	g_pData->GetDamageManager()->SetDamageDataFromLUA( &m_DamageData, *pLuaManager, "DAMAGE_DATA", m_fPowerRate );

	
	if(fHitAddMp >= 0.f)
	{
		m_DamageData.fHitAddMP = fHitAddMp;
	}

	if( m_DamageData.attackType != CX2DamageManager::AT_SPECIAL )
	{
		if( NULL != g_pX2Game && null != m_optrOwnerUnit )
		{
			// 패시브 스킬에 따라 강제다운 수치 조정
			if( 1.f != m_optrOwnerUnit->GetSkillRelatedData().GetForceDownValueRateOfRangeAttack() )
			{
				m_DamageData.fForceDownValue *= m_optrOwnerUnit->GetSkillRelatedData().GetForceDownValueRateOfRangeAttack();
			}		
		}		
	}
#ifdef SERV_ADD_CHUNG_SHELLING_GUARDIAN

#ifndef UPGRADE_SKILL_SYSTEM_2013
	else
	{
		fUseCannonBallDamageRel = 0.3f;
	}
#endif // UPGRADE_SKILL_SYSTEM_2013

	if( m_bUseCannonBall == true && null != m_optrOwnerUnit && m_optrOwnerUnit->GetGameUnitType() == CX2GameUnit::GUT_USER )
	{
		CX2GUUser *pUser = static_cast<CX2GUUser*>( m_optrOwnerUnit.GetObservable() );
#ifdef SERV_CHUNG_TACTICAL_TROOPER
		if( ( pUser->GetUnitClass() == CX2Unit::UC_CHUNG_SHELLING_GUARDIAN || pUser->GetUnitClass() == CX2Unit::UC_CHUNG_TACTICAL_TROOPER ) 
			&& pUser->GetCannonBallDamageUp() > 0.f )
#else  SERV_CHUNG_TACTICAL_TROOPER
		if( pUser->GetUnitClass() == CX2Unit::UC_CHUNG_SHELLING_GUARDIAN && pUser->GetCannonBallDamageUp() > 0.f )
#endif SERV_CHUNG_TACTICAL_TROOPER
		{			
			float fUpDamage = pUser->GetCannonBallDamageUp() * fUseCannonBallDamageRel;
			float fUpPhysicDamage = m_DamageData.damage.fPhysic;
			float fUpMagicDamage = m_DamageData.damage.fMagic;
			fUpPhysicDamage = (fUpPhysicDamage * fUpDamage);
			fUpMagicDamage = (fUpMagicDamage * fUpDamage);

			m_DamageData.damage.fPhysic					+= fUpPhysicDamage;
			m_DamageData.damage.fMagic					+= fUpMagicDamage;

			float fCannonBallDamageUp = m_fPowerRate + (m_fPowerRate * fUpDamage);
			m_DamageData.m_fDecreaseForceDown = fCannonBallDamageUp * pUser->GetForceDownRelDamageRateBase();
			
			if( m_DamageData.m_ExtraDamage.m_bFixedDamage == false )
			{
				m_DamageData.m_ExtraDamage.m_DamagePerSec	*= fCannonBallDamageUp;
				m_DamageData.m_ExtraDamage.m_Damage			*= fCannonBallDamageUp;
			}	
		}
	}
#endif //SERV_ADD_CHUNG_SHELLING_GUARDIAN

	PLUA_GET_VALUE_ENUM( m_pLuaManager, "EFFECT_TYPE", m_EffectType, CX2DamageEffect::EFFECT_TYPE, DEET_NONE );
	PLUA_GET_VALUE( m_pLuaManager, "DAMAGE_TIME",							m_DamageTime,						1 );
	PLUA_GET_VALUE( m_pLuaManager, "ATTACKED_BY_UNIT",						m_bAttackedByUnit,					false );
	PLUA_GET_VALUE( m_pLuaManager, "ATTACKED_BY_EFFECT",					m_bAttackedByEffect,				false );
	PLUA_GET_VALUE( m_pLuaManager, "REFLECT",								m_bReflect,							false );
	PLUA_GET_VALUE( m_pLuaManager, "ATTACK_TIME_START",						m_AttackTime.x,						0.0f );
	PLUA_GET_VALUE( m_pLuaManager, "ATTACK_TIME_END",						m_AttackTime.y,						100.0f );
	PLUA_GET_VALUE( m_pLuaManager, "USE_GLOBAL_TIME",						m_bGlobalTime,						false );
	PLUA_GET_VALUE( m_pLuaManager, "DAMAGE_DATA_CHANGE_TIME",				m_fDamageDataChangeTime,			-1.0f );
	PLUA_GET_VALUE( m_pLuaManager, "DAMAGE_DATA_CHANGE_TIME_GLOBAL_TIME",	m_fDamageDataChangeTimeGlobalTime,	-1.0f );
	PLUA_GET_VALUE( m_pLuaManager, "CLEAR_HIT_UNIT_ON_DAMAGE_DATA_CHANGE",	m_bClearHitUnitOnChangeDamageData,	false );
	PLUA_GET_VALUE( m_pLuaManager, "FOLLOW_LINE",							m_bFollowLine,						false );

	PLUA_GET_VALUE( m_pLuaManager, "FOLLOW_LINE_NEW",						m_bFollowLineNew,						false );

	ASSERT( false == m_bFollowLineNew || true == m_bFollowLine );

//{{ kimhc // 2010.2.10 //	매 프레임 LandPosition 갱신 안되도록
#ifdef	EVE_SECOND_CLASS_CHANGE
	PLUA_GET_VALUE( m_pLuaManager, "REFRESH_LAND_POS",						m_bRefreshLandPos,					true );
#endif	EVE_SECOND_CLASS_CHANGE
//}} kimhc // 2010.2.10 //	매 프레임 LandPosition 갱신 안되도록


#ifdef SKILL_CASH_10_TEST
	PLUA_GET_VALUE( m_pLuaManager, "REACT_ANIMSPEED",						m_fReactAnimSpeed,					0.f );
	PLUA_GET_VALUE( m_pLuaManager, "FOLLOW_ON_LINE",						m_bFollowOnLine,					false );
	
#endif

#ifdef RAVEN_WEAPON_TAKER
	PLUA_GET_VALUE( m_pLuaManager, "MUST_ON_LINE",							m_bMustOnLine,						false );
#endif RAVEN_WEAPON_TAKER



#ifdef STICKY_DAMAGE_EFFECT_TEST

	PLUA_GET_VALUE( m_pLuaManager, "TRACE_ATTACKED_UNIT",				m_bTraceAttackedUnit,		false );
	PLUA_GET_VALUE( m_pLuaManager, "LIFE_TIME_AFTER_ATTACK",			m_fLifeTimeAfterAttack,		-1.f );
	PLUA_GET_VALUE( m_pLuaManager, "TRACE_ATTACKED_UNIT_POSITION_X",	m_vTracePosition.x,			0.f );
	PLUA_GET_VALUE( m_pLuaManager, "TRACE_ATTACKED_UNIT_POSITION_Y",	m_vTracePosition.y,			0.f );
	PLUA_GET_VALUE( m_pLuaManager, "TRACE_ATTACKED_UNIT_POSITION_Z",	m_vTracePosition.z,			0.f );

#endif STICKY_DAMAGE_EFFECT_TEST

	if( m_bFollowLine == true )
		FollowLine();

	LoadLockOnData();
	LoadBlackHoleData();


#ifdef _SIN_MOVE_TEST_
	LoadSinMoveData();
#endif _SIN_MOVE_TEST_
	
	//{{ kimhc // 2009-10-20 // DamageEffect.lua에 있는 테이블 로드(일명 독구름 작업)
#ifdef	DAMAGE_EFFECT_WORK
	LoadDieDamageEffect();
	LoadStartParticle();
	LoadPassiveParticle();
	LoadTimeParticle();
	LoadCameraData();
	LoadScreenData();
	LoadSoundData();
	LoadDamageEffect();
	LoadHitParticle();
	LoadHitDamageEffect();

	m_DamageEffectDataInLua.m_pDamageDataNext = new CX2DamageManager::DamageData();
	*(m_DamageEffectDataInLua.m_pDamageDataNext) = m_DamageData;

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	m_DamageEffectDataInLua.m_pDamageDataNext->m_vecBuffFactorPtr.clear();
#endif //UPGRADE_SKILL_SYSTEM_2013
	if ( false == g_pData->GetDamageManager()->SetDamageDataFromLUA( m_DamageEffectDataInLua.m_pDamageDataNext, *pLuaManager, "DAMAGE_DATA_NEXT", m_fPowerRate ) )
		SAFE_DELETE( m_DamageEffectDataInLua.m_pDamageDataNext );

#endif	DAMAGE_EFFECT_WORK
	//}} kimhc // 2009-10-20 // DamageEffect.lua에 있는 테이블 로드(일명 독구름 작업)

#ifdef RAVEN_SECOND_CLASS_CHANGE
	// damage times 변경
	if( pGameUnit != NULL )
	{
		bool bChangeDamageTimes = false;
		PLUA_GET_VALUE( m_pLuaManager, "CHANGE_DAMAGE_TIME",				bChangeDamageTimes,		false );

		int iDamageTimes = pGameUnit->GetDamageTimes();
		if( bChangeDamageTimes == true && iDamageTimes >= 0 )
		{
			m_DamageTime = iDamageTimes;
		}
	}
#endif

	PLUA_GET_VALUE( m_pLuaManager, "APPLY_SCALE_BY_UNIT",					m_bApplyScaleByUnit,	true );
#ifdef INHERIT_FORCEDOWNRATE
	PLUA_GET_VALUE( m_pLuaManager, "INHERIT_FORCEDOWNRATE",					m_bInheritForceDownRate,	false );
#endif INHERIT_FORCEDOWNRATE

#ifdef FIX_AFTER_IMAGE
	LoadAfterImage();
#endif

#ifdef DAMAGE_EFFECT_GROUP_DAMAGE
	PLUA_GET_VALUE(	m_pLuaManager, "GROUP_DAMAGE",				m_bGroupDamage,				false	);		/// 그룹 데미지 설정 추가
#endif DAMAGE_EFFECT_GROUP_DAMAGE
	

#ifdef ADDED_RELATIONSHIP_SYSTEM
	// 자기 자신 or 인연에게만 적용되는 데미지 이펙트.
	PLUA_GET_VALUE(	m_pLuaManager, "ONLY_RELATIONSHIP",		m_bOnlyRelationShip,		false	);	
#endif // ADDED_RELATIONSHIP_SYSTEM

	

#ifdef NOTIFY_TO_OWNER_UNIT_WHEN_DAMAGE_EFFECT_DIE
	PLUA_GET_VALUE( m_pLuaManager, "NOTIFY_TO_OWNER_WHEN_DIE",	m_bShouldNotifyToOwnerUnitWhenDie,	false );
#endif // NOTIFY_TO_OWNER_UNIT_WHEN_DAMAGE_EFFECT_DIE

#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
	/// SetLinkMainEffectByPos() 를 쓰기 위해 필요한 길이 배율
	PLUA_GET_VALUE( m_pLuaManager, "LINK_EFFECT_RATE",			m_fLinkEffectRate,		-1.f );
#endif //SERV_ADD_LUNATIC_PSYKER

	m_fElapsedTime = 0.f;
}; 



CX2DamageEffect::CEffect::~CEffect()
{
	KTDXPROFILE();


	//BOOST_TEST_FOREACH( CKTDXDeviceSound*, pSound, m_vecPlayingLoopSound )
	//{
	//	if( NULL != pSound )
	//	{
	//		pSound->Stop();
	//	}
	//}
	//m_vecPlayingLoopSound.clear();


#ifdef CLOSE_SOUND_TEST
	m_SoundCloseManager.CloseSound();
#endif CLOSE_SOUND_TEST



	if( m_hMeshInstHandle != INVALID_MESH_INSTANCE_HANDLE &&
		g_pX2Game != NULL && 
		g_pX2Game->GetMajorXMeshPlayer() )
	{
		g_pX2Game->GetMajorXMeshPlayer()->DestroyInstanceHandle( m_hMeshInstHandle );
	}


	for( int i = 0; i < (int)m_StartParticleDataList.size(); i++ )
	{
		ParticleData* pParticleData = m_StartParticleDataList[i];
#ifdef BALANCE_PATCH_20120329
		pParticleData->DestroyParticleSequence();
#endif
		SAFE_DELETE( pParticleData );
	}
	m_StartParticleDataList.clear();

	for( int i = 0; i < (int)m_PassiveParticleDataList.size(); i++ )
	{
		ParticleData* pParticleData = m_PassiveParticleDataList[i];
		pParticleData->DestroyParticleSequence();
		SAFE_DELETE( pParticleData );
	}
	m_PassiveParticleDataList.clear();

	for( int i = 0; i < (int)m_TimeParticleDataList.size(); i++ )
	{
		ParticleData* pParticleData = m_TimeParticleDataList[i];
		pParticleData->DestroyParticleSequence();
		SAFE_DELETE( pParticleData );
	}
	m_TimeParticleDataList.clear();

	m_CameraDataList.clear();
	m_ScreenDataList.clear();
	m_SoundDataList.clear();

#ifdef FIX_AFTER_IMAGE
	if( m_pAfterImage != NULL )
		SAFE_DELETE_KTDGOBJECT( m_pAfterImage );
#endif


	for ( int i = 0; i < (int)m_DamageEffectDataList.size(); i++ )
	{
		DamageEffectData* pDamageEffectData = m_DamageEffectDataList[i];
		SAFE_DELETE( pDamageEffectData );
	}
	m_DamageEffectDataList.clear();


	for ( int i = 0; i < (int)m_vecHitDamageEffect.size(); i++ )
	{
		DamageEffectData* pDamageEffectData = m_vecHitDamageEffect[i];
		SAFE_DELETE( pDamageEffectData );
	}
	m_vecHitDamageEffect.clear();

	for ( int i = 0; i < (int)m_vecDieDamageEffect.size(); i++ )
	{
		DamageEffectData* pDamageEffectData = m_vecDieDamageEffect[i];
		SAFE_DELETE( pDamageEffectData );
	}
	m_vecDieDamageEffect.clear();

#ifdef NOTIFY_TO_OWNER_UNIT_WHEN_DAMAGE_EFFECT_DIE
	// 데미지 이펙트를 생성한 유닛 객체에게만 불려 짐
	if( true == m_bShouldNotifyToOwnerUnitWhenDie &&
		null != m_optrOwnerUnit )
	{
		m_optrOwnerUnit->DieDamageEffectProc(m_DamageData);
	}
#endif // NOTIFY_TO_OWNER_UNIT_WHEN_DAMAGE_EFFECT_DIE

	if( true == m_BlackHoleData.m_bEnabled && NULL != g_pX2Game )
	{
		for( int i=0; i<g_pX2Game->GetUnitNum(); i++ )
		{
			CX2GameUnit* pUnit = g_pX2Game->GetUnit(i);
			if( NULL == pUnit )
				continue;

			if( null != m_optrOwnerUnit && pUnit->GetTeam() == m_optrOwnerUnit->GetTeam() )
				continue;

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            pUnit->RemoveEffectiveBlackHoleDamageEffect( GetHandle() );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			pUnit->RemoveEffectiveBlackHoleDamageEffect( this );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		}
	}

	//{{ kimhc // 2009-10-20 // 독구름 작업 관련 추가
#ifdef	DAMAGE_EFFECT_WORK
	for ( UINT i = 0; i < m_DamageEffectDataInLua.m_vecHitParticle.size(); i++ )
	{
		ParticleData* pParticleData = m_DamageEffectDataInLua.m_vecHitParticle[i];
		SAFE_DELETE( pParticleData );
	}
	m_DamageEffectDataInLua.m_vecHitParticle.clear();

	for ( UINT i = 0; i < m_DamageEffectDataInLua.m_vecHitDamageEffect.size(); i++ )
	{
		DamageEffectData* pDamageEffectData = m_DamageEffectDataInLua.m_vecHitDamageEffect[i];
		SAFE_DELETE( pDamageEffectData );
	}
	m_DamageEffectDataInLua.m_vecHitDamageEffect.clear();

	for ( UINT i = 0; i < m_DamageEffectDataInLua.m_vecDieDamageEffect.size(); i++ )
	{
		DamageEffectData* pDamageEffectData = m_DamageEffectDataInLua.m_vecDieDamageEffect[i];
		SAFE_DELETE( pDamageEffectData );
	}
	m_DamageEffectDataInLua.m_vecDieDamageEffect.clear();

	SAFE_DELETE( m_DamageEffectDataInLua.m_pDamageDataNext );

#endif	DAMAGE_EFFECT_WORK
		//}} kimhc // 2009-10-20 // 독구름 작업 관련 추가

}

void CX2DamageEffect::CEffect::OnFrameMove( double fTime, float fElapsedTime )
{
	KTDXPROFILE();



#ifdef STOP_UNIT_STOP_EFFECT_TEST

	m_StopTime.OnFrameMove( fTime, fElapsedTime );
	if( m_StopTime.m_fTimeLeft > 0.f )
	{
		return; 
	}

#endif STOP_UNIT_STOP_EFFECT_TEST



	m_fElapsedTime = fElapsedTime;
    if ( g_pX2Game == NULL )
    {
		SetDieDamageEffect();
		m_hMeshInstHandle = INVALID_MESH_INSTANCE_HANDLE;
		m_bLive			= false;
    }
    else if( g_pX2Game->GetMajorXMeshPlayer()->IsLiveInstanceHandle( m_hMeshInstHandle ) == false
#ifdef RAVEN_WEAPON_TAKER
		|| g_pX2Game->GetMajorXMeshPlayer()->IsDyingByCrash( m_hMeshInstHandle ) == true
#endif RAVEN_WEAPON_TAKER
		)
	{
		SetDieDamageEffect();
		g_pX2Game->GetMajorXMeshPlayer()->PlayEnd( m_hMeshInstHandle );
		m_hMeshInstHandle = INVALID_MESH_INSTANCE_HANDLE;
		m_bLive			= false;
	}
	else
	{   
		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->GetMeshInstance( m_hMeshInstHandle );
		ASSERT( pMeshInst != NULL );


		if( m_bUseDXMatrix == true )
		{
			pMeshInst->SetUseDXMatrix( m_DXMATRIX );
		}
		m_vMainEffectAxisAngleDegree = pMeshInst->GetMatrix().GetRotateDegree();
		m_vMainEffectPosition = pMeshInst->GetMatrix().GetPos();


		if( m_bFollowLine == true )
		{
			FollowLine();
		}

		for( int i = 0; i < (int)m_PassiveParticleDataList.size(); i++ )
		{
			ParticleData* pParticleData = m_PassiveParticleDataList[i];		
			SetParticleData( pParticleData );
		}

		for( int i = 0; i < (int)m_TimeParticleDataList.size(); i++ )
		{
			ParticleData* pParticleData = m_TimeParticleDataList[i];

			if( INVALID_PARTICLE_SEQUENCE_HANDLE != pParticleData->m_hSeq )
			{	
				if( g_pX2Game->GetMajorParticle()->IsLiveInstanceHandle( pParticleData->m_hSeq ) == true 
					&& true == pParticleData->bTrace )
				{
					SetParticleData( pParticleData );
				}
				else
				{
					pParticleData->m_hSeq = INVALID_PARTICLE_SEQUENCE_HANDLE;
				}
			}
#ifdef DAMAGE_EFFECT_TIMED_PARTICLE_GLOBAL_TIME
			else if( true == pParticleData->bUseGlobalTime )
			{
#ifdef ELSWORD_SHEATH_KNIGHT
				if( pMeshInst->EventTimerGlobal( pParticleData->fEventTime ) == true )
#else
				if(pParticleData->fEventTime < pMeshInst->GetGlobalTime() )
#endif ELSWORD_SHEATH_KNIGHT
				{
					pParticleData->m_hSeq = g_pX2Game->GetMajorParticle()->CreateSequenceHandle( NULL, pParticleData->particleName.c_str(), 0,0,0, -1, -1, -1, pParticleData->triggerCount, pParticleData->triggerTime );
					CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( pParticleData->m_hSeq );
					if( NULL != pSeq )
					{
						pSeq->SetLandPosition( m_fLandPos );
					}

					SetParticleData( pParticleData );
				}
			}
			else if( pMeshInst->GetXSkinAnim()->EventTimer( pParticleData->fEventTime ) == true )
#endif DAMAGE_EFFECT_TIMED_PARTICLE_GLOBAL_TIME
			{
				pParticleData->m_hSeq = g_pX2Game->GetMajorParticle()->CreateSequenceHandle( NULL, pParticleData->particleName.c_str(), 0,0,0, -1, -1, -1, pParticleData->triggerCount, pParticleData->triggerTime );
				CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( pParticleData->m_hSeq );
				if( NULL != pSeq )
				{
					pSeq->SetLandPosition( m_fLandPos );
				}
				
				SetParticleData( pParticleData );
			}
		}

		if( null != m_optrOwnerUnit )
		{
			for( int i = 0; i < (int)m_CameraDataList.size(); i++ )
			{
				CameraData* pCameraData = &m_CameraDataList[i];	
				if( pMeshInst->GetXSkinAnim()->EventTimer( pCameraData->fEventTime ) == true )
				{
					if( pCameraData->unitSelect == DEUS_FOCUS && m_optrOwnerUnit->IsFocusUnit() != true )
						continue;
					if( pCameraData->unitSelect == DEUS_MY && m_optrOwnerUnit->IsMyUnit() != true )
						continue;

					switch( pCameraData->crashType )
					{
					case CKTDGCamera::DECT_UP:
						g_pX2Game->GetX2Camera()->GetCamera().UpCrashCamera( pCameraData->fSpeed, pCameraData->fAccel );
						break;

					case CKTDGCamera::DECT_DOWN:
						g_pX2Game->GetX2Camera()->GetCamera().DownCrashCamera( pCameraData->fSpeed, pCameraData->fAccel );
						break;

					case CKTDGCamera::DECT_UP_DOWN:
						g_pX2Game->GetX2Camera()->GetCamera().UpDownCrashCamera( pCameraData->fGap, pCameraData->fTime );
						break;

					case CKTDGCamera::DECT_UP_DOWN_NO_RESET:
						g_pX2Game->GetX2Camera()->GetCamera().UpDownCrashCameraNoReset( pCameraData->fGap, pCameraData->fTime );
						break;


					case CKTDGCamera::DECT_LEFT:
						{
							g_pX2Game->GetX2Camera()->GetCamera().LeftCrashCamera( pCameraData->fSpeed, pCameraData->fAccel );
						} break;
					case CKTDGCamera::DECT_RIGHT:
						{
							g_pX2Game->GetX2Camera()->GetCamera().RightCrashCamera( pCameraData->fSpeed, pCameraData->fAccel );
						} break;
					case CKTDGCamera::DECT_LEFT_RIGHT:
						{
							g_pX2Game->GetX2Camera()->GetCamera().LeftRightCrashCamera( pCameraData->fSpeed, pCameraData->fAccel );
						} break;
					case CKTDGCamera::DECT_LEFT_RIGHT_NO_RESET:
						{
							g_pX2Game->GetX2Camera()->GetCamera().LeftRightCrashCameraNoReset( pCameraData->fSpeed, pCameraData->fAccel );
						} break;


					case CKTDGCamera::DECT_RANDOM:
						{
							g_pX2Game->GetX2Camera()->GetCamera().ShakeRandom( 15, pCameraData->fSpeed*0.8f, pCameraData->fSpeed*1.2f, pCameraData->fGap*0.8f, pCameraData->fGap*1.2f, 
																						15, pCameraData->fSpeed*0.8f, pCameraData->fSpeed*1.2f, pCameraData->fGap*0.8f, pCameraData->fGap*1.2f, 
																						true ); // todo!!!! 스크립트에 입력할 수 있게 해야함
						} break;

					}
					m_CameraDataList.erase( m_CameraDataList.begin() + i );
					i--;
				}
			}

			for( int i = 0; i < (int)m_ScreenDataList.size(); i++ )
			{
				ScreenData* pScreenData = &m_ScreenDataList[i];	
				if( pMeshInst->GetXSkinAnim()->EventTimer( pScreenData->fEventTime ) == true )
				{
					if( pScreenData->unitSelect == DEUS_FOCUS && m_optrOwnerUnit->IsFocusUnit() != true )
						continue;
					if( pScreenData->unitSelect == DEUS_MY && m_optrOwnerUnit->IsMyUnit() != true )
						continue;

					g_pKTDXApp->GetDGManager()->ClearScreen( pScreenData->clearCount, pScreenData->color );

					m_ScreenDataList.erase( m_ScreenDataList.begin() + i );
					i--;
				}
			}
		}

		
		OnFrameMove_SoundData();

		//{{ kimhc // 2009-10-20 // CEffect 전과 후에 BeginTable, EndTable을 호출 하므로 삭제,(일명 독구름 작업)
#ifndef	DAMAGE_EFFECT_WORK		
		// 현재 열려있는 damage effect 테이블을 잠시 닫고 
		m_pLuaManager->EndTable();
#endif	DAMAGE_EFFECT_WORK		
		//{{ kimhc // 2009-10-20 // CEffect 전과 후에 BeginTable, EndTable을 호출 하므로 삭제,(일명 독구름 작업)
		
		for( int i = 0; i < (int)m_DamageEffectDataList.size(); i++ )
		{
			DamageEffectData* pData = m_DamageEffectDataList[i];

		#ifdef LOOP_CREATE_DAMAGE_EFFECT // 김태환
			if ( NULL == pData )
				continue;

			/// 반복 종료 시간을 넘겼다면, 해당 정보는 제거
			if ( pMeshInst->GetNowLifeTime() >= pData->fLoopEndTime )
			{
				SAFE_DELETE( pData );
				m_DamageEffectDataList.erase( m_DamageEffectDataList.begin() + i );
				i--;

				continue;
			}

			/// 반복 생성을 고려한 쿨타임 연산 = 생성 시간 + ( 반복 간격 * ( 생성된 횟수 ) )
			const float fCheckEventTime = pData->fEventTime + ( pData->fLoopInterval * static_cast<float>( pData->uiCreateCount ) );

			if( pMeshInst->EventTimerGlobal( fCheckEventTime ) == true )
		#else //LOOP_CREATE_DAMAGE_EFFECT
			if( pMeshInst->EventTimerGlobal( pData->fEventTime ) == true )
		#endif //LOOP_CREATE_DAMAGE_EFFECT
			{
				if( null != m_optrOwnerUnit )
				{

					D3DXVECTOR3 vAxisAngleDegree( 0, 0, 0);
					D3DXVECTOR3 vRotateAngleDegree( 0, 0, 0);
					D3DXVECTOR3 vPosition(0,0,0);


					switch( pData->posType )
					{
					case DEPT_BONE:
						{
							if(pData->bApplyRotateOrigin == true)
							{
								vAxisAngleDegree = m_AngleDegree;
								vRotateAngleDegree = vAxisAngleDegree;
							}
							else if( pData->bApplyRotate == true )
							{
								vAxisAngleDegree = pMeshInst->GetXSkinAnim()->GetMatrix().GetRotateDegree();
								vRotateAngleDegree = vAxisAngleDegree;
							}
		#ifdef SERV_ADD_CHUNG_SHELLING_GUARDIAN
							else if( pData->bApplyMatrix == true )
							{
								D3DXMATRIX* pCombineMatrix = &(pMeshInst->GetXSkinAnim()->GetCloneFrame(pData->boneName.c_str())->combineMatrix);
								if( pCombineMatrix != NULL )
								{
		#ifdef DAMAGE_EFFECT_BUG_FIX
									D3DXVECTOR3 vRot = GetDecomposeAngle( pCombineMatrix, GetDecomposeScale(pCombineMatrix) );
		#else
									D3DXVECTOR3 vRot = GetDecomposeAngle( pCombineMatrix );
		#endif
									vRot.x = D3DXToDegree(vRot.x);
									vRot.y = D3DXToDegree(vRot.y);
									vRot.z = D3DXToDegree(vRot.z);
									vAxisAngleDegree = vRot;
									vRotateAngleDegree = vAxisAngleDegree;
								}
							}
		#endif //SERV_ADD_CHUNG_SHELLING_GUARDIAN
							else
							{
								vAxisAngleDegree = pData->vRotateAngleDegree;
								vRotateAngleDegree = vAxisAngleDegree;
							}
							vPosition = pMeshInst->GetXSkinAnim()->GetCloneFramePosition( pData->boneName.c_str() ); 
						}
						break;

					case DEPT_UNIT_BONE:
						{
							if( null != m_optrOwnerUnit )
							{
								vAxisAngleDegree = m_optrOwnerUnit->GetRotateDegree();

								if(pData->bApplyRotateOrigin == true)
								{
									vAxisAngleDegree = m_AngleDegree;
									vRotateAngleDegree = vAxisAngleDegree;
								}
								else if( pData->bApplyRotate == true )
								{
									vRotateAngleDegree = vAxisAngleDegree;
								}

								vPosition = m_optrOwnerUnit->GetBonePos( pData->boneName.c_str() );
							}
						}
						break;

					case DEPT_IMPACT_POINT:
						{
							vPosition = m_DamageData.impactPoint;
						}
						break;
		#ifdef RAVEN_WEAPON_TAKER
					case DEPT_DIE_POS:
						{
							vPosition = m_vLandPositionOnLineMap;
						}
						break;
		#endif RAVEN_WEAPON_TAKER

					case DEPT_POS:
						{
							if(pData->bApplyRotateOrigin == true)
							{
								vAxisAngleDegree = m_AngleDegree;
								vRotateAngleDegree = vAxisAngleDegree;
							}
							else if( pData->bApplyRotate == true )
							{
								vAxisAngleDegree = pMeshInst->GetMatrix().GetRotateDegree();
								vRotateAngleDegree = vAxisAngleDegree;

		#ifdef SERV_ADD_CHUNG_SHELLING_GUARDIAN
								if( pData->bApplyRotatedOffset == true )
								{
									vRotateAngleDegree = vRotateAngleDegree + pData->vRotateAngleDegree;
								}
		#endif
							}
							else
							{
								vAxisAngleDegree = pData->vRotateAngleDegree;
								vRotateAngleDegree = vAxisAngleDegree;
							}

							vPosition = pMeshInst->GetMatrix().GetPos();
						}
						break;
					}                   
					//{{ oasis907 : 김상윤 [2010.2.10] // 
					if(pData->bApplyRotatedOffset == true)
					{
						D3DXVECTOR3 vDirVector = pMeshInst->GetDirVec();

						if( 0.f != vDirVector.y )
						{
							vDirVector.y = 0.f;
							D3DXVec3Normalize( &vDirVector, &vDirVector );
						}
						if( 0.f != pData->vOffset.x )
						{
							vPosition += vDirVector * pData->vOffset.x;
						}
						if( 0.f != pData->vOffset.y )
						{
							vPosition.y += pData->vOffset.y;
						}
						if( 0.f != pData->vOffset.z )
						{
							if( vDirVector.y != 0.f )
							{
								vDirVector.y = vDirVector.z;
								vDirVector.z = vDirVector.x;
								vDirVector.x = -vDirVector.y;
								vDirVector.y = 0.f;
								D3DXVec3Normalize( &vDirVector, &vDirVector );
							}
							else
							{
								vDirVector.y = vDirVector.z;
								vDirVector.z = vDirVector.x;
								vDirVector.x = -vDirVector.y;
								vDirVector.y = 0.f;
							}
							vPosition += vDirVector * pData->vOffset.z;
						}
					}
					//}} oasis907 : 김상윤 [2010.2.10] // 
					else
					{
						vPosition += pData->vOffset;
					}

					if( m_pManager != NULL )
					{
		#ifdef INHERIT_FORCEDOWNRATE
						float fForceDownRate = 1.f;
						if(true == m_bInheritForceDownRate)
						{
							fForceDownRate *= m_fForceDownRate;
						}
						CEffect* pEffect = m_pManager->CreateInstance( m_optrOwnerUnit.GetObservable(), pData->damageEffectName.c_str(), m_fPowerRate,
							vPosition, vRotateAngleDegree, vAxisAngleDegree, m_fLandPos, false, -1.f, fForceDownRate);
		#ifdef ADDITIONAL_MEMO

						if ( NULL != pEffect )
						{
							/// 버프팩터를 상속함
							if ( pData->bInheritBuffFactor && !GetDamageData().m_vecBuffFactorPtr.empty() )
								pEffect->GetDamageData().m_vecBuffFactorPtr = GetDamageData().m_vecBuffFactorPtr;

							if( GetLockOnData().m_LockOnType != CX2DamageEffect::LOT_NONE && pEffect->GetLockOnData().m_LockOnType != CX2DamageEffect::LOT_NONE )
							{
								LockOnData& kLockOnData = pEffect->GetLockOnData();
								LOCK_ON_TYPE lockontype = kLockOnData.m_LockOnType;

								kLockOnData = GetLockOnData();
								kLockOnData.m_LockOnType = lockontype;
							}								
		#endif
							
		#ifdef ADD_GAME_STAGE_DELETE_DAMAGEEFFECT
							if( pData->bAutoDie )
							{
		#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
                                g_pX2Game->AddRemoveDamageEffect( pEffect->GetHandle() );
		#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
								g_pX2Game->AddRemoveDamageEffect( pEffect );
		#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
							}
		#endif //ADD_GAME_STAGE_DELETE_DAMAGEEFFECT

		#else //INHERIT_FORCEDOWNRATE
							CEffect* pEffect = m_pManager->CreateInstance( m_optrOwnerUnit.GetObservable(), pData->damageEffectName.c_str(), m_fPowerRate,
								vPosition, vRotateAngleDegree, vAxisAngleDegree, 
								m_fLandPos, false );
		#endif INHERIT_FORCEDOWNRATE

		#ifdef INHERIT_FORCEDOWNRATE
							pEffect->SetInheritForceDownRate(m_bInheritForceDownRate);
		#ifdef SERV_RAVEN_VETERAN_COMMANDER
							if( -1 != pData->iGroupID )		/// 그룹 데미지 설정
								pEffect->SetLaserGroupID( pData->iGroupID );
		#endif SERV_RAVEN_VETERAN_COMMANDER

		#endif INHERIT_FORCEDOWNRATE

		#ifdef ADD_RENA_SYSTEM //김창한
							//스킬, 관련 데이터 값, 첫타인지 체크 하는 값을 상속함
							pEffect->GetDamageData().m_eFirstAttack = m_DamageData.m_eFirstAttack;
							pEffect->GetDamageData().m_RelateSkillData = m_DamageData.m_RelateSkillData;
		#endif //ADD_RENA_SYSTEM

		#ifdef LOOP_CREATE_DAMAGE_EFFECT // 김태환
							++pData->uiCreateCount;		/// 생성 횟수 갱신

							/////////////////////////////////////////////////////////////////////////////////////
							/// 아래 구문에서 생성 횟수에 따라 지워버런 후 Continue를 시켜 버리니, 
							/// 기능을 추가하시려면 이 주석 위에 작업해 주세요!
							/////////////////////////////////////////////////////////////////////////////////////

							/// 반복 생성 횟수를 만족하면, 지워 버리자.
							if ( pData->uiLoopEndCount <= pData->uiCreateCount )
							{
								SAFE_DELETE( pData );
								m_DamageEffectDataList.erase( m_DamageEffectDataList.begin() + i );
								i--;

								/// 혹시 이후 코드에서 pData를 쓸 지도 모르니, continue 시켜 버리자.
								continue;
							}

							continue;
		#endif //LOOP_CREATE_DAMAGE_EFFECT
						}

					}

				}

				SAFE_DELETE( pData );
				m_DamageEffectDataList.erase( m_DamageEffectDataList.begin() + i );
				i--;
			}
		}


		//{{ kimhc // 2009-10-20 // CEffect 전과 후에 BeginTable, EndTable을 호출 하므로 삭제,(일명 독구름 작업)
#ifndef	DAMAGE_EFFECT_WORK		
		m_pLuaManager->BeginTable( GetName() );
#endif	DAMAGE_EFFECT_WORK		
		//{{ kimhc // 2009-10-20 // CEffect 전과 후에 BeginTable, EndTable을 호출 하므로 삭제,(일명 독구름 작업)

		if( m_DamageData.bReAttack == true )
		{
			for( CX2DamageManager::HitUnitVector::iterator iter = m_DamageData.hitUnitList.begin(); 
                iter != m_DamageData.hitUnitList.end(); )
			{
				CX2DamageManager::HitUnit* pHitUnit = &*iter;
				pHitUnit->fRemainGap -= fElapsedTime;
				if( pHitUnit->fRemainGap <= 0.0f )
				{
#ifdef  X2OPTIMIZE_STL_CONTAINER_USAGE
                    iter = EraseUnorderlyUsingSwap( m_DamageData.hitUnitList, iter );
#else   X2OPTIMIZE_STL_CONTAINER_USAGE
					iter = m_DamageData.hitUnitList.erase( iter );
#endif  X2OPTIMIZE_STL_CONTAINER_USAGE
				}
                else
                {
                    ++iter;
                }
			}
		}


#ifdef STICKY_DAMAGE_EFFECT_TEST

		if( true == m_bEnableAttack )
#endif STICKY_DAMAGE_EFFECT_TEST

#ifdef X2OPTIMIZE_DAMAGE_EFFECT_TEST
		if( g_pMain->GetDamageEffectTest() == true )
#endif//X2OPTIMIZE_DAMAGE_EFFECT_TEST
		{
			if ( m_bGlobalTime == true )
			{
				if( m_AttackTime.x <= pMeshInst->GetGlobalTime()
					&& m_AttackTime.y > pMeshInst->GetGlobalTime() )
				{
					g_pData->GetDamageManager()->DamageCheck( &m_DamageData );
				}
			}
			else
			{
				if( m_AttackTime.x <= pMeshInst->GetXSkinAnim()->GetNowAnimationTime()
					&& m_AttackTime.y > pMeshInst->GetXSkinAnim()->GetNowAnimationTime() )
				{
					g_pData->GetDamageManager()->DamageCheck( &m_DamageData );
				}
			}
		}

		

		if( g_pX2Game->GetMajorXMeshPlayer()->IsLiveInstanceHandle( m_hMeshInstHandle ) == false
#ifdef RAVEN_WEAPON_TAKER
			|| g_pX2Game->GetMajorXMeshPlayer()->IsDyingByCrash( m_hMeshInstHandle ) == true
#endif RAVEN_WEAPON_TAKER
			)
		{
			SetDieDamageEffect();
			g_pX2Game->GetMajorXMeshPlayer()->PlayEnd( m_hMeshInstHandle );
			m_hMeshInstHandle = INVALID_MESH_INSTANCE_HANDLE;
			m_bLive			= false;
			return;
		}


		if( pMeshInst->GetXSkinAnim()->EventTimer( m_fDamageDataChangeTime ) == true && m_fDamageDataChangeTime != -1.0f )
		{
			m_fDamageDataChangeTime = -1.0f;

			// kimhc // 2009-10-22 // 데미지 이펙트에 DAMAGE_DATA_NEXT가 나오지 않는 문제 수정(일명 독구름 작업)
			SetDamageDataNext( &m_DamageData );

			if( true == m_bClearHitUnitOnChangeDamageData )
			{
				m_DamageData.hitUnitList.resize(0);
			}
		}
		if(  m_fDamageDataChangeTimeGlobalTime != -1.0f && pMeshInst->EventTimerGlobal( m_fDamageDataChangeTimeGlobalTime ) == true )
		{
			m_fDamageDataChangeTimeGlobalTime = -1.0f;

			// kimhc // 2009-10-22 // 데미지 이펙트에 DAMAGE_DATA_NEXT가 나오지 않는 문제 수정(일명 독구름 작업)
			SetDamageDataNext( &m_DamageData );

			if( true == m_bClearHitUnitOnChangeDamageData )
			{
				m_DamageData.hitUnitList.resize(0);
			}
		}

		if( m_bFirstFrame == true )
		{
			SetStartParticle();
			SetPassiveParticle();
			SetTimeParticle();
			SetCameraData();
			SetScreenData();
			SetSoundData();
			SetDamageEffect();
			m_bFirstFrame = false;
		}

		OnFrameMove_BlackHoleData(fElapsedTime);
		OnFrameMove_LockOnData( fElapsedTime );
		OnFrameMove_SineMoveData(fElapsedTime);

#ifdef FIX_AFTER_IMAGE
		if ( IsSamef(m_afterImageTime.x, -1.f) == false 
			 && IsSamef(m_afterImageTime.y, -1.f) == false )
		{
#ifdef X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
			if( g_pMain->GetGameOption().GetOptionList().m_eEffect == CX2GameOption::OL_HIGH )
#else//X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
			if( g_pMain->GetGameOption().GetOptionList().m_bEffect == true )
#endif//X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION			
			{
				if( m_pAfterImage != NULL && GetMainEffect() != NULL && 
					GetMainEffect()->GetXSkinMesh() != NULL &&
					GetMainEffect()->GetXSkinAnim() != NULL && 
					GetMainEffect()->GetState() == CKTDGXMeshPlayer::IS_PLAY )
				{
					m_fDurationTime += fElapsedTime;

					if( m_afterImageTime.x <= GetMainEffect()->GetXSkinAnim()->GetNowAnimationTime()
						&& m_afterImageTime.y > m_fDurationTime )
					{
						m_pAfterImage->Enable();
						m_pAfterImage->OnFrameMove( fTime, m_fElapsedTime );
					}
					else
					{
						m_fDurationTime = 0.f;
					}
				}
			}
		}
		else
		{
			if( m_pAfterImage != NULL )
				m_pAfterImage->Disable();

			m_fDurationTime = 0.f;
		}
#endif


#ifdef STICKY_DAMAGE_EFFECT_TEST
		OnFrameMove_Trace();

#endif STICKY_DAMAGE_EFFECT_TEST


		m_BeforePos = pMeshInst->GetPos();
		
		if( IsSamef(pMeshInst->GetTempletData()->elasticCoeffX.GetRandomNumInRange(), 0.f) == false && m_bRefreshLandPos == true )
		{
			int iLineIndex = -1;
			D3DXVECTOR3 landPosition = g_pX2Game->GetWorld()->GetLineMap()->GetLandPosition( m_BeforePos, LINE_RADIUS, &iLineIndex );
			if( iLineIndex < 0 )
			{
				landPosition = g_pX2Game->GetWorld()->GetLineMap()->GetNearest_LUA( m_BeforePos );
			}
			pMeshInst->SetLandPosition( landPosition.y );
		}

		//if ( pMeshInst->GetIsUsedLand() == true )
		if( pMeshInst->GetIsCrashLand() == true 
			//{{ kimhc // 2010.2.10 //	매 프레임 LandPosition 갱신 안되도록
#ifdef	EVE_SECOND_CLASS_CHANGE
			 && m_bRefreshLandPos == true
#endif	EVE_SECOND_CLASS_CHANGE
				//}} kimhc // 2010.2.10 //	매 프레임 LandPosition 갱신 안되도록
		)
		{			
			unsigned char* pLineIndex = NULL;
			D3DXVECTOR3 landPosition = g_pX2Game->GetWorld()->GetLineMap()->GetLandPosition( m_BeforePos, LINE_RADIUS * 3, pLineIndex );

			pMeshInst->SetLandPosition( landPosition.y );
#ifdef RAVEN_WEAPON_TAKER
			SetLandPositionOnLineMap( landPosition );
#endif RAVEN_WEAPON_TAKER
		}
		
	}	
}

void CX2DamageEffect::CEffect::OnFrameMove_SoundData()
{
    if ( g_pX2Game == NULL )
        return;

	CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->GetMeshInstance( m_hMeshInstHandle );
	ASSERT( pMeshInst != NULL );

	if( NULL == pMeshInst )
		return; 


	for( int i = 0; i < (int)m_SoundDataList.size(); i++ )
	{
		SoundData* pSoundData = &m_SoundDataList[i];

		bool bIsRightTimingToPlaySound = false;
		if( pSoundData->bUseGlobalTime == true )
		{
			if( pSoundData->fEventTime < pMeshInst->GetGlobalTime() )
			{
				bIsRightTimingToPlaySound = true;
			}
		}
		else
		{
			if( pMeshInst->GetXSkinAnim()->EventTimer( pSoundData->fEventTime ) == true )
			{
				bIsRightTimingToPlaySound = true;
			}
		}

		if( true == bIsRightTimingToPlaySound )
		{
			if( rand()%100 < pSoundData->iRate )
			{
				CKTDXDeviceSound* pSound = g_pKTDXApp->GetDeviceManager()->OpenSound( pSoundData->fileName.c_str() );
				if( NULL != pSound )
				{


#pragma DMLEE_NOTE( "loop sound 는 채널 문제 수정하고 나서 사용가능" )


					pSound->Set3DPosition( pMeshInst->GetPos() );
					pSound->Play();


// 					if( true == g_pKTDXApp->GetDSManager()->GetCapable3DSound() &&
// 						true == g_pKTDXApp->GetDSManager()->GetEnable3DSound() )
// 					{
// 						pSound->Set3DPosition( pMeshInst->GetPos() );
// 					}



					//if( true == pSoundData->bLoop )
					//{
					//	m_vecPlayingLoopSound.push_back( pSound );
					//}


#ifdef CLOSE_SOUND_TEST
					m_SoundCloseManager.AddSound( pSoundData->fileName );
#endif CLOSE_SOUND_TEST
				}
			}

			m_SoundDataList.erase( m_SoundDataList.begin() + i );
			i--;
		}
	}
}

void CX2DamageEffect::CEffect::OnFrameMove_BlackHoleData( float fElapsedTime )
{
    if ( g_pX2Game == NULL )
        return;

	CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->GetMeshInstance( m_hMeshInstHandle );
	ASSERT( pMeshInst != NULL );

	if( NULL == pMeshInst )
		return; 



	if( true == m_BlackHoleData.m_bEnabled && 
		null != m_optrOwnerUnit )
	{
		D3DXVECTOR3 vPos = pMeshInst->GetPos();

		for( int i=0; i<g_pX2Game->GetUnitNum(); i++ )
		{
			CX2GameUnit* pUnit = g_pX2Game->GetUnit(i);
			if( NULL == pUnit )
				continue;

			if( pUnit->GetTeam() == m_optrOwnerUnit->GetTeam() )
				continue;

			if( pUnit->GetNowHp() <= 0.f )
				continue;


			if( true == m_BlackHoleData.m_bDragOnlyHittedUnit )
			{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
                if( false == pUnit->IsEffectiveBlackHoleDamageEffect( GetHandle() ) )
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
				if( false == pUnit->IsEffectiveBlackHoleDamageEffect( this ) )
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
				{
					continue;
				}
			}


			// 그외 블랙홀에 빨려들지 않을 다른 조건 검사 
			switch( pUnit->GetGameUnitType() )
			{
			case CX2GameUnit::GUT_USER:
				{
					CX2GUUser* pUser = (CX2GUUser*) pUnit;

					if( pUser->GetSyncData().nowState == pUser->GetDamageDownLandFront() ||
						pUser->GetSyncData().nowState == pUser->GetDamageDownLandBack() ||
						pUser->GetSyncData().nowState == pUser->GetDamageAirDownLanding() )
					{
						continue;
					}

				} break;

			case CX2GameUnit::GUT_NPC:
				{
					CX2GUNPC* pNPC = (CX2GUNPC*) pUnit;

					if( true == pNPC->GetNeverMove() )
						continue;

					if( false == pNPC->GetIsFallDownMonster() )		
						continue;

					if( true == pNPC->GetSiegeMode() )
						continue;

					if( pNPC->GetNowStateID() == pNPC->GetDownDamageLandFront() ||
						pNPC->GetNowStateID() == pNPC->GetDownDamageLandBack() )
					{
						continue;
					}

					switch( pNPC->GetNPCTemplet().m_ClassType )
					{
					case CX2UnitManager::NCT_THING_GATE:
					case CX2UnitManager::NCT_THING_BOX:			
					case CX2UnitManager::NCT_THING_TRAP:		
					case CX2UnitManager::NCT_THING_DEVICE:		
					case CX2UnitManager::NCT_THING_HOUSE:		
					case CX2UnitManager::NCT_THING_WORLD_BLOCK:
					case CX2UnitManager::NCT_THING_CHECKER:
						//{{ JHKang / 강정훈 / 2010/12/07 / 버프 및 디버프 받지 않는 NPC TYPE
#ifdef NEW_SKILL_2010_11
					case CX2UnitManager::NCT_THING_NOBUFF:		/// 버프/디버프에 영향을 받지 않음()
#endif NEW_SKILL_2010_11
						//}} JHKang / 강정훈 / 2010/12/07 / 버프 및 디버프 받지 않는 NPC TYPE
						continue;
					}
#ifdef SERV_EVENT_VALENTINE_DUNGEON
					switch ( pNPC->GetNPCTemplet().m_nNPCUnitID )
					{
					case CX2UnitManager::NUI_VALENTINE_POISON_CUP_CAKE:
						continue;
					}
#endif //SERV_EVENT_VALENTINE_DUNGEON
#ifdef SERV_EVENT_VALENTINE_DUNGEON_INT
					switch( pNPC->GetNPCTemplet().m_nNPCUnitID )
					{
					case CX2UnitManager::NUI_EVENT_VALENTINEGIFT_BOX:
						continue;
					}
#endif SERV_EVENT_VALENTINE_DUNGEON_INT
				} break;
			}


			D3DXVECTOR3 vDir = vPos - pUnit->GetPos( true );
			float fDist = D3DXVec3Length( &vDir );
			if( fDist > m_BlackHoleData.m_fDraggingRange )
			{
				if( CX2GameUnit::GUT_USER == pUnit->GetGameUnitType() )
				{
					CX2GUUser* pUser = (CX2GUUser*) pUnit;
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
                    pUser->RemoveEffectiveBlackHoleDamageEffect( GetHandle() );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
					pUser->RemoveEffectiveBlackHoleDamageEffect( this );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
				}
				continue;
			}

			//const float MAGIC_ABSOLUTE_BLACKHOLE_RANGE = 200.f; // 이 안에 들어오면 무조건 안 움직이게 잡아버린다.
			//if( fDist < MAGIC_ABSOLUTE_BLACKHOLE_RANGE )
			//{
			//	pUnit->SetFixedUnitPosition( vPos );
			//}
			//else
			{
				D3DXVec3Normalize( &vDir, &vDir );
				ASSERT( m_BlackHoleData.m_fDraggingRange != 0.f );

				float fSpeed = (1.5f - fDist / m_BlackHoleData.m_fDraggingRange);
				if( fSpeed > 1.f )
				{
					fSpeed = 1.f;
				}
				fSpeed *= m_BlackHoleData.m_fDraggingSpeed;
				pUnit->SetDisplaceUnitPosition( vDir * fSpeed * fElapsedTime );
#ifdef ARA_CHARACTER_BASE
				if ( true == m_BlackHoleData.m_bCanPassUnit )
					pUnit->SetCanPassUnit( m_BlackHoleData.m_bCanPassUnit );
#endif
			}


			// 블랙홀에 빨려들어가고 있는 중에 각성이나 필살기를 사용할 수 없다. 
			if( CX2GameUnit::GUT_USER == pUnit->GetGameUnitType() )
			{
				CX2GUUser* pUser = (CX2GUUser*) pUnit;
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
                pUser->AddEffectiveBlackHoleDamageEffect( GetHandle() );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
				pUser->AddEffectiveBlackHoleDamageEffect( this );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			}
		}
	}
}






void CX2DamageEffect::CEffect::OnFrameMove_LockOnData(  float fElapsedTime_ )
{
    if ( g_pX2Game == NULL )
        return;

	CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->GetMeshInstance( m_hMeshInstHandle );
	ASSERT( pMeshInst != NULL );

	if( NULL == pMeshInst )
		return; 

	//{{ kimhc // 2011.1.18 // 청 1차 전직
#ifdef	CHUNG_FIRST_CLASS_CHANGE
	// Unlock 시간이 주어졌을 때 Unlock 시간이 경과하지 않았으면 리턴
	if ( 0.0f < m_LockOnData.m_fTimeUnlockFromStart )
	{
		m_LockOnData.m_fTimeUnlockFromStart -= fElapsedTime_;
		return;
	} // if	
#endif	CHUNG_FIRST_CLASS_CHANGE
	//}} kimhc // 2011.1.18 // 청 1차 전직

#ifdef UPGRADE_RAVEN
	if( m_LockOnData.m_fUnLockTime > 0.f )
		m_LockOnData.m_fUnLockTime -= fElapsedTime_;

	if( m_LockOnData.m_fUnLockTime <= 0.f )
		return;
#endif

	//락온 기능
	switch( m_LockOnData.m_LockOnType )
	{
	case LOT_POS_BLACK_HOLE:
		{
			pMeshInst->SetBlackHolePos( m_LockOnData.m_LockOnPos );
		}
		break;

	case LOT_UID_BLACK_HOLE:
		{
			CX2GUUser* pCX2GUUser = g_pX2Game->GetUserUnitByUID( m_LockOnData.m_LockOnUnitUID );
			if( pCX2GUUser != NULL )
			{
				D3DXVECTOR3 pos = pCX2GUUser->GetPos();
				pos.y += 100.0f;
				pMeshInst->SetBlackHolePos( pos );
			}
			else
			{
				CX2GUNPC* pCX2GUNPC = g_pX2Game->GetNPCUnitByUID( m_LockOnData.m_LockOnNPCUID );
				if( pCX2GUNPC != NULL )
				{
					D3DXVECTOR3 pos = pCX2GUNPC->GetPos();
					pos.y += 100.0f;
					pMeshInst->SetBlackHolePos( pos );
				}
			}
		}
		break;

	case LOT_POS_VECTOR:
		{
			VectorLockOn( m_LockOnData.m_LockOnPos );
		}
		break;

	case LOT_UID_VECTOR:
		{
			CX2GUUser* pCX2GUUser = g_pX2Game->GetUserUnitByUID( m_LockOnData.m_LockOnUnitUID );
			if( pCX2GUUser != NULL )
			{
				//D3DXVECTOR3 pos = pCX2GUUser->GetPos();
				//pos.y += 100.0f;

				D3DXVECTOR3 vPos( 0.f, 0.f, 0.f );

				/*vPos = pCX2GUUser->GetPos();
				vPos.y += 100.f;*/

				CX2GameUnit* pOwnerUnit = GetOwnerUnit();
				if ( NULL == pOwnerUnit )
					pOwnerUnit = g_pX2Game->GetMyUnit();

				const D3DXVECTOR3& vOwnerUnitPos = pOwnerUnit->GetPos();

				const CKTDXCollision::CollisionDataListSet& AttackListSet = pCX2GUUser->GetCollisionListSet();
#ifdef  X2OPTIMIZE_LOCKONDATA_COLLISION_CHECK_CRASH_BUG_FIX
				bool    bInit = false;
#endif  X2OPTIMIZE_LOCKONDATA_COLLISION_CHECK_CRASH_BUG_FIX
				BOOST_FOREACH( const CKTDXCollision::CollisionDataList* collisionDataList, AttackListSet )
				{
					BOOST_FOREACH( CKTDXCollision::CollisionData* collisionData, *collisionDataList )
					{
						D3DXVECTOR3 vCollPos = collisionData->GetPointStart();
#ifdef  X2OPTIMIZE_LOCKONDATA_COLLISION_CHECK_CRASH_BUG_FIX
						if ( bInit == false )
						{
							bInit = true;
							vPos = vCollPos;
							continue;
						}
#endif  X2OPTIMIZE_LOCKONDATA_COLLISION_CHECK_CRASH_BUG_FIX
						float fDist = GetDistance( vOwnerUnitPos, vCollPos );
						float fOldDist = GetDistance( vOwnerUnitPos, vPos );

						if ( fDist <= fOldDist )
							vPos = vCollPos;
					}
				}
#ifdef  X2OPTIMIZE_LOCKONDATA_COLLISION_CHECK_CRASH_BUG_FIX
				if ( bInit == false )
				{
					vPos = pCX2GUUser->GetPos();
					vPos.y += 100.0f;
				}
#endif  X2OPTIMIZE_LOCKONDATA_COLLISION_CHECK_CRASH_BUG_FIX
				VectorLockOn( vPos );

				if( pCX2GUUser->GetNowHp() <= 0.f && 
					null != m_optrOwnerUnit )
				{
					m_LockOnData.m_LockOnUnitUID = -1;

					bool bUserUnit;
					UidType uid = g_pX2Game->GetLockOnTarget( (CX2Room::TEAM_NUM)m_optrOwnerUnit->GetTeam(), m_LockOnData.m_iRandomSeed, bUserUnit );
					if( bUserUnit == true )
						m_LockOnData.m_LockOnUnitUID = uid;
					else
						m_LockOnData.m_LockOnNPCUID = (int) uid;
				}
			}
			else
			{
				CX2GUNPC* pCX2GUNPC = g_pX2Game->GetNPCUnitByUID( m_LockOnData.m_LockOnNPCUID );
				if( pCX2GUNPC != NULL )
				{
					//D3DXVECTOR3 pos = pCX2GUNPC->GetPos();
					//pos.y += 100.0f;
					D3DXVECTOR3 vPos( 0.f, 0.f, 0.f );
					
					CX2GameUnit* pOwnerUnit = GetOwnerUnit();
					if ( NULL == pOwnerUnit )
						pOwnerUnit = g_pX2Game->GetMyUnit();

					const D3DXVECTOR3& vOwnerUnitPos = pOwnerUnit->GetPos();
					
					const CKTDXCollision::CollisionDataListSet& AttackListSet = pCX2GUNPC->GetCollisionListSet();
#ifdef  X2OPTIMIZE_LOCKONDATA_COLLISION_CHECK_CRASH_BUG_FIX
					bool    bInit = false;
#else   X2OPTIMIZE_LOCKONDATA_COLLISION_CHECK_CRASH_BUG_FIX
					const CKTDXCollision::CollisionDataList* collisionDataList = *AttackListSet.begin();
					CKTDXCollision::CollisionData* collisionData = *collisionDataList->begin();
					vPos = collisionData->GetPointStart();
#endif  X2OPTIMIZE_LOCKONDATA_COLLISION_CHECK_CRASH_BUG_FIX

					BOOST_FOREACH( const CKTDXCollision::CollisionDataList* collisionDataList, AttackListSet )
					{
						BOOST_FOREACH( CKTDXCollision::CollisionData* collisionData, *collisionDataList )
						{
							D3DXVECTOR3 vCollPos = collisionData->GetPointStart();
#ifdef  X2OPTIMIZE_LOCKONDATA_COLLISION_CHECK_CRASH_BUG_FIX
                            if ( bInit == false )
                            {
                                bInit = true;
                                vPos = vCollPos;
                                continue;
                            }
#endif  X2OPTIMIZE_LOCKONDATA_COLLISION_CHECK_CRASH_BUG_FIX
							float fDist = GetDistance( vOwnerUnitPos, vCollPos );
							float fOldDist = GetDistance( vOwnerUnitPos, vPos );

							if ( fDist <= fOldDist )
								vPos = vCollPos;
						}
					}
#ifdef  X2OPTIMIZE_LOCKONDATA_COLLISION_CHECK_CRASH_BUG_FIX
                    if ( bInit == false )
                    {
					    vPos = pCX2GUNPC->GetPos();
					    vPos.y += 100.0f;
                    }
#endif  X2OPTIMIZE_LOCKONDATA_COLLISION_CHECK_CRASH_BUG_FIX
					VectorLockOn( vPos );


					if( pCX2GUNPC->GetNowHp() <= 0.f && 
						null != m_optrOwnerUnit )
					{
						m_LockOnData.m_LockOnNPCUID = -1;

						bool bUserUnit;
						UidType uid = g_pX2Game->GetLockOnTarget( (CX2Room::TEAM_NUM)m_optrOwnerUnit->GetTeam(), m_LockOnData.m_iRandomSeed, bUserUnit );
						if( bUserUnit == true )
							m_LockOnData.m_LockOnUnitUID = uid;
						else
							m_LockOnData.m_LockOnNPCUID = (int) uid;
					}
				}
			}
		}
		break;

	case LOT_NEARST_UID_VECTOR:
		{
			const CX2GameUnit* pGameUnit = NULL;

			if ( -1 != m_LockOnData.m_LockOnUnitUID )
			{
				pGameUnit = g_pX2Game->GetUserUnitByUID( m_LockOnData.m_LockOnUnitUID );
			} // if
			else if ( -1 != m_LockOnData.m_LockOnNPCUID )
			{
				pGameUnit = g_pX2Game->GetNPCUnitByUID( m_LockOnData.m_LockOnNPCUID );
			} // else if

			if ( NULL != pGameUnit )
			{
				/*D3DXVECTOR3 vPos = pGameUnit->GetPos();
				vPos.y += 100.0f;*/

				D3DXVECTOR3 vPos( 0.f, 0.f, 0.f );

				CX2GameUnit* pOwnerUnit = GetOwnerUnit();
				if ( NULL == pOwnerUnit )
					pOwnerUnit = g_pX2Game->GetMyUnit();

				const D3DXVECTOR3& vOwnerUnitPos = pOwnerUnit->GetPos();

				const CKTDXCollision::CollisionDataListSet& AttackListSet = pGameUnit->GetCollisionListSet();
#ifdef  X2OPTIMIZE_LOCKONDATA_COLLISION_CHECK_CRASH_BUG_FIX
				bool    bInit = false;
#else   X2OPTIMIZE_LOCKONDATA_COLLISION_CHECK_CRASH_BUG_FIX
				const CKTDXCollision::CollisionDataList* collisionDataList = *AttackListSet.begin();
				CKTDXCollision::CollisionData* collisionData = *collisionDataList->begin();
				vPos = collisionData->GetPointStart();
#endif  X2OPTIMIZE_LOCKONDATA_COLLISION_CHECK_CRASH_BUG_FIX

				BOOST_FOREACH( const CKTDXCollision::CollisionDataList* collisionDataList, AttackListSet )
				{
					BOOST_FOREACH( CKTDXCollision::CollisionData* collisionData, *collisionDataList )
					{
						D3DXVECTOR3 vCollPos = collisionData->GetPointStart();
#ifdef  X2OPTIMIZE_LOCKONDATA_COLLISION_CHECK_CRASH_BUG_FIX
						if ( bInit == false )
						{
							bInit = true;
							vPos = vCollPos;
							continue;
						}
#endif  X2OPTIMIZE_LOCKONDATA_COLLISION_CHECK_CRASH_BUG_FIX
						float fDist = GetDistance( vOwnerUnitPos, vCollPos );
						float fOldDist = GetDistance( vOwnerUnitPos, vPos );

						if ( fDist <= fOldDist )
							vPos = vCollPos;
					}
				}
#ifdef  X2OPTIMIZE_LOCKONDATA_COLLISION_CHECK_CRASH_BUG_FIX
				if ( bInit == false )
				{
					vPos = pGameUnit->GetPos();
					vPos.y += 100.0f;
				}
#endif  X2OPTIMIZE_LOCKONDATA_COLLISION_CHECK_CRASH_BUG_FIX

				VectorLockOn( vPos );

				// 타겟을 잃었으면
				if( 0.f >= pGameUnit->GetNowHp() && null != m_optrOwnerUnit )
				{
					// 기존의 타켓을 초기화
					m_LockOnData.m_LockOnUnitUID	= -1;
					m_LockOnData.m_LockOnNPCUID		= -1;

					CX2GameUnit* pOwnerUnit = GetOwnerUnit();
					if ( NULL == pOwnerUnit )
						pOwnerUnit = g_pX2Game->GetMyUnit();

					const D3DXVECTOR3& vOwnerUnitPos = pOwnerUnit->GetPos();

					bool bUserUnit = false;
					UidType gameUnitUID = 
						g_pX2Game->GetLockOnNearstTarget( static_cast<CX2Room::TEAM_NUM>( m_optrOwnerUnit->GetTeam() ), vOwnerUnitPos, bUserUnit );

					if( bUserUnit == true )
						m_LockOnData.m_LockOnUnitUID = gameUnitUID;
					else
						m_LockOnData.m_LockOnNPCUID = static_cast<int>( gameUnitUID );
				} // if
			} // if 
		} break;

	case LOT_NEARST_UID_VECTOR_IN_RANGE:
		{
			const CX2GameUnit* pGameUnit = NULL;

			if ( -1 != m_LockOnData.m_LockOnUnitUID )
			{
				pGameUnit = g_pX2Game->GetUserUnitByUID( m_LockOnData.m_LockOnUnitUID );
			} // if
			else if ( -1 != m_LockOnData.m_LockOnNPCUID )
			{
				pGameUnit = g_pX2Game->GetNPCUnitByUID( m_LockOnData.m_LockOnNPCUID );
			} // else if

			if ( NULL != pGameUnit )
			{

#ifdef BALANCE_PATCH_20131107
				if( LOCSD_SAVE_DIR_SPEED == m_LockOnData.m_eNotFindTargetMoveStop )
				{// 이제 타겟을 찾았으므로 저장된 dir_speed 값이 있다면 그 값으로 바꿔준다.
					CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->GetMeshInstance( m_hMeshInstHandle );
					if( NULL != pMeshInst )
					{
						pMeshInst->SetDirSpeed(m_LockOnData.m_fSaveDirSpeed);
						m_LockOnData.m_eNotFindTargetMoveStop = LOCSD_NOT_SAVE_DIR_SPEED;
					}
				}
#endif //BALANCE_PATCH_20131107

				D3DXVECTOR3 vPos( 0.f, 0.f, 0.f );
				
				//vPos = pGameUnit->GetPos();
				//vPos.y += 100.f;

				CX2GameUnit* pOwnerUnit = GetOwnerUnit();
				if ( NULL == pOwnerUnit )
					pOwnerUnit = g_pX2Game->GetMyUnit();

				const D3DXVECTOR3& vOwnerUnitPos = pOwnerUnit->GetPos();

				const CKTDXCollision::CollisionDataListSet& AttackListSet = pGameUnit->GetCollisionListSet();
#ifdef  X2OPTIMIZE_LOCKONDATA_COLLISION_CHECK_CRASH_BUG_FIX
                bool    bInit = false;
#else   X2OPTIMIZE_LOCKONDATA_COLLISION_CHECK_CRASH_BUG_FIX
				const CKTDXCollision::CollisionDataList* collisionDataList = *AttackListSet.begin();
				CKTDXCollision::CollisionData* collisionData = *collisionDataList->begin();
				vPos = collisionData->GetPointStart();
#endif  X2OPTIMIZE_LOCKONDATA_COLLISION_CHECK_CRASH_BUG_FIX

				BOOST_FOREACH( const CKTDXCollision::CollisionDataList* collisionDataList, AttackListSet )
				{
					BOOST_FOREACH( CKTDXCollision::CollisionData* collisionData, *collisionDataList )
					{
						D3DXVECTOR3 vCollPos = collisionData->GetPointStart();
#ifdef  X2OPTIMIZE_LOCKONDATA_COLLISION_CHECK_CRASH_BUG_FIX
                        if ( bInit == false )
                        {
                            bInit = true;
                            vPos = vCollPos;
                            continue;
                        }
#endif  X2OPTIMIZE_LOCKONDATA_COLLISION_CHECK_CRASH_BUG_FIX

						float fDist = GetDistance( vOwnerUnitPos, vCollPos );
						float fOldDist = GetDistance( vOwnerUnitPos, vPos );

						if ( fDist <= fOldDist )
							vPos = vCollPos;
					}
				}

				//D3DXVECTOR3 vPos = pGameUnit->GetPos();
				//vPos.y += 100.f;
#ifdef  X2OPTIMIZE_LOCKONDATA_COLLISION_CHECK_CRASH_BUG_FIX
                if ( bInit == false )
                {
					vPos = pGameUnit->GetPos();
					vPos.y += 100.0f;
                }
#endif  X2OPTIMIZE_LOCKONDATA_COLLISION_CHECK_CRASH_BUG_FIX
				VectorLockOn( vPos );

				if ( m_DamageTimeNow > m_PrevDamageTime )
				{
					m_PrevDamageTime = m_DamageTimeNow;

					bool bUserUnit = false;
					UidType gameUnitUID = -1;

					D3DXVECTOR2 vRange = (g_pMain->GetNowStateID() == CX2Main::XS_PVP_GAME) ? D3DXVECTOR2( 150.f, 300.f) : D3DXVECTOR2( 150.f, 750.f);
#ifdef BALANCE_DEADLY_CHASER_20130214
					if ( true == m_LockOnData.m_bIsNotTargetPreLockOnTarget )
					{
						gameUnitUID = g_pX2Game->GetLockOnNearstTargetInRange( static_cast<CX2Room::TEAM_NUM>( m_optrOwnerUnit->GetTeam() ), vPos, vRange, bUserUnit, true );
					}
					else
#endif //BALANCE_DEADLY_CHASER_20130214
					{
						gameUnitUID = g_pX2Game->GetLockOnNearstTargetInRange( static_cast<CX2Room::TEAM_NUM>( m_optrOwnerUnit->GetTeam() ), vPos, vRange, bUserUnit );
					}

					if( bUserUnit == true )
						m_LockOnData.m_LockOnUnitUID = gameUnitUID;
					else
						m_LockOnData.m_LockOnNPCUID = static_cast<int>( gameUnitUID );
				}

				// 타겟을 잃었으면
				if( 0.f >= pGameUnit->GetNowHp() && null != m_optrOwnerUnit )
				{
					// 기존의 타켓을 초기화
					m_LockOnData.m_LockOnUnitUID	= -1;
					m_LockOnData.m_LockOnNPCUID		= -1;


#ifdef EVE_ELECTRA
					if( GetMainEffect() != NULL)
#endif EVE_ELECTRA
					{
						D3DXVECTOR3 vPos = GetMainEffect()->GetPos();

						bool bUserUnit = false;
						UidType gameUnitUID = -1;

						D3DXVECTOR2 vRange = ( g_pMain->GetNowStateID() == CX2Main::XS_PVP_GAME ? D3DXVECTOR2( 150.f, 300.f) : D3DXVECTOR2( 150.f, 750.f) );

#ifdef SERV_ADD_CHUNG_SHELLING_GUARDIAN
						if( m_LockOnData.m_fRange > 0.f )
						{
							vRange = vRange * m_LockOnData.m_fRange;
						}
#endif

#ifdef BALANCE_DEADLY_CHASER_20130214
						if ( true == m_LockOnData.m_bIsNotTargetPreLockOnTarget )
						{
							gameUnitUID = g_pX2Game->GetLockOnNearstTargetInRange( static_cast<CX2Room::TEAM_NUM>( m_optrOwnerUnit->GetTeam() ), vPos, vRange, bUserUnit, true );
						}
						else
#endif //BALANCE_DEADLY_CHASER_20130214
						{
							gameUnitUID = g_pX2Game->GetLockOnNearstTargetInRange( static_cast<CX2Room::TEAM_NUM>( m_optrOwnerUnit->GetTeam() ), vPos, vRange, bUserUnit );
						}

						if( bUserUnit == true )
							m_LockOnData.m_LockOnUnitUID = gameUnitUID;
						else
							m_LockOnData.m_LockOnNPCUID = static_cast<int>( gameUnitUID );
					} // if
				}  // if
			} // if
			else
			{
				if ( NULL != GetMainEffect() && 
					m_DamageTime >= m_DamageTimeNow && m_DamageTimeNow >= m_PrevDamageTime
#ifdef SUMMON_NPC_GUIDANCE_MISSILE_CRASH_FIX
					&& null != m_optrOwnerUnit
#endif //SUMMON_NPC_GUIDANCE_MISSILE_CRASH_FIX
					)
				{
					D3DXVECTOR3 vPos = GetMainEffect()->GetPos();

					bool bUserUnit = false;
					UidType gameUnitUID = -1;

					D3DXVECTOR2 vRange = ( g_pMain->GetNowStateID() == CX2Main::XS_PVP_GAME ? D3DXVECTOR2( 150.f, 300.f) : D3DXVECTOR2( 150.f, 750.f) );

#ifdef SERV_ADD_CHUNG_SHELLING_GUARDIAN
					if( m_LockOnData.m_fRange > 0.f )
					{
						vRange = vRange * m_LockOnData.m_fRange;
					}
#endif

#ifdef BALANCE_DEADLY_CHASER_20130214
					if ( true == m_LockOnData.m_bIsNotTargetPreLockOnTarget )
					{
						gameUnitUID = g_pX2Game->GetLockOnNearstTargetInRange( static_cast<CX2Room::TEAM_NUM>( m_optrOwnerUnit->GetTeam() ), vPos, vRange, bUserUnit, true );
					}
					else
#endif //BALANCE_DEADLY_CHASER_20130214
					{
						gameUnitUID = g_pX2Game->GetLockOnNearstTargetInRange( static_cast<CX2Room::TEAM_NUM>( m_optrOwnerUnit->GetTeam() ), vPos, vRange, bUserUnit );
					}
					
					if( bUserUnit == true )
						m_LockOnData.m_LockOnUnitUID = gameUnitUID;
					else
						m_LockOnData.m_LockOnNPCUID = static_cast<int>( gameUnitUID );

#ifdef BALANCE_PATCH_20131107
					if( LOCSD_NOT_SAVE_DIR_SPEED == m_LockOnData.m_eNotFindTargetMoveStop )
					{		
						CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->GetMeshInstance( m_hMeshInstHandle );
						if( NULL != pMeshInst )
						{
							if( ( m_LockOnData.m_LockOnUnitUID < 0 ) && ( m_LockOnData.m_LockOnNPCUID < 0 ) )
							{//타겟이 없으므로 dir_Speed를 저장하고 dir_Speed값을 0으로 만든다.
								m_LockOnData.m_fSaveDirSpeed = pMeshInst->GetDirSpeed();
								pMeshInst->SetDirSpeed(0.f);
								m_LockOnData.m_eNotFindTargetMoveStop = LOCSD_SAVE_DIR_SPEED;
							}
						}
					}
#endif //BALANCE_PATCH_20131107

				}
			}
		} break;
#ifdef SERV_9TH_NEW_CHARACTER // 김태환
			case LOT_UID_BONE_POS:		/// 록온된 유저의 특정 본을 록온
				{
					CX2GUUser* pCX2GUUser = g_pX2Game->GetUserUnitByUID( m_LockOnData.m_LockOnUnitUID );

					if( pCX2GUUser != NULL )
					{
						/// Bone Pos 반환
						D3DXVECTOR3 vBonePos = pCX2GUUser->GetBonePos( m_LockOnData.m_wstrTargetBoneName.c_str() );

						/// Bone Pos가 없다면, 그냥 해댱 유저 위치 설정 하자
						if ( D3DXVECTOR3( 0.f, 0.f, 0.f ) == vBonePos )
							vBonePos = pCX2GUUser->GetPos();

						VectorLockOn( vBonePos );
					}
					else
					{
						CX2GUNPC* pCX2GUNPC = g_pX2Game->GetNPCUnitByUID( m_LockOnData.m_LockOnNPCUID );

						if ( NULL != pCX2GUNPC )
						{
							/// Bone Pos 반환
							D3DXVECTOR3 vBonePos = pCX2GUNPC->GetBonePos( m_LockOnData.m_wstrTargetBoneName.c_str() );

							/// Bone Pos가 없다면, 그냥 해댱 NPC 위치 설정 하자
							if ( D3DXVECTOR3( 0.f, 0.f, 0.f ) == vBonePos )
								vBonePos = pCX2GUNPC->GetPos();

							VectorLockOn( vBonePos );
						}
					}
				} break;
#endif //SERV_9TH_NEW_CHARACTER
	}
}




void CX2DamageEffect::CEffect::OnFrameMove_SineMoveData( float fElapsedTime )
{
#ifdef _SIN_MOVE_TEST_
	//**

    if ( g_pX2Game == NULL )
        return;

	CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->GetMeshInstance( m_hMeshInstHandle );
	//ASSERT( pMeshInst != NULL );
	if( NULL == pMeshInst )
		return; 

	if(m_SinMoveData.m_bSinMove)
	{
		D3DXVECTOR3 Position;
		Position = pMeshInst->GetPos();

		float sinvalue = sin(m_SinMoveData.m_fCurrentTime/m_SinMoveData.m_fTime*3.141592f*2);

		// subtracting old sin movement from position
		Position.y = Position.y - m_SinMoveData.m_fMove * sinvalue;

		// update time
		m_SinMoveData.m_fCurrentTime = m_SinMoveData.m_fCurrentTime + fElapsedTime;
		if(m_SinMoveData.m_fCurrentTime > m_SinMoveData.m_fTime )
		{
			m_SinMoveData.m_fCurrentTime = m_SinMoveData.m_fCurrentTime - m_SinMoveData.m_fTime;
			if( null != m_optrOwnerUnit )
			{
				float RandomSeed = GetOwnerUnit()->GetRandomFloat();
				m_SinMoveData.m_fTime = (m_SinMoveData.m_fTimeMax-m_SinMoveData.m_fTimeMin)*RandomSeed + m_SinMoveData.m_fTimeMin;
				m_SinMoveData.m_fMove = (m_SinMoveData.m_fMoveMax-m_SinMoveData.m_fMoveMin)*RandomSeed + m_SinMoveData.m_fMoveMin;
			}
			else
			{
				m_SinMoveData.m_fTime = (m_SinMoveData.m_fTimeMin + m_SinMoveData.m_fTimeMax) / 2;
				m_SinMoveData.m_fMove = (m_SinMoveData.m_fMoveMin + m_SinMoveData.m_fMoveMax) / 2;
			}

		}

		// add new sin movement to position
		sinvalue = sin(m_SinMoveData.m_fCurrentTime/m_SinMoveData.m_fTime*3.141592f*2);
		Position.y = Position.y + m_SinMoveData.m_fMove * sinvalue;

		pMeshInst->SetPos(Position);

	}
#endif _SIN_MOVE_TEST_
}



void CX2DamageEffect::CEffect::OnFrameMove_Trace()
{
#ifdef STICKY_DAMAGE_EFFECT_TEST

	if( false == m_bTraceAttackedUnit )
		return; 



	if( NULL == m_pTraceUnit || g_pX2Game == NULL )
		return; 


	CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->GetMeshInstance( m_hMeshInstHandle );
	if( NULL == pMeshInst )
		return; 

	

	if( false == g_pX2Game->IsValidUnit( m_pTraceUnit ) )
	{
		m_pTraceUnit = NULL;

		pMeshInst->SetMaxLifeTime( 0.f );

		return;
	}


	D3DXVECTOR3 vPos = m_pTraceUnit->GetPos();
	vPos += m_vTracePosition.x * m_pTraceUnit->GetDirVector();
	vPos.y += m_vTracePosition.y;
	vPos += m_vTracePosition.z * m_pTraceUnit->GetZVector();

	pMeshInst->SetPos( vPos );

	pMeshInst->SetVelocity( D3DXVECTOR3( 0.f, 0.f, 0.f ) );
	pMeshInst->SetDirSpeed( 0.f );
#endif STICKY_DAMAGE_EFFECT_TEST


}


//
//void CX2DamageEffect::CEffect::OnFrameRender()
//{
//	KTDXPROFILE();
//}




void CX2DamageEffect::CEffect::DamageReact( CX2DamageManager::DamageData* pDamageData )
{
#if defined( _SERVICE_ ) 
	// checked
	ELSWORD_VIRTUALIZER_START
#endif

	CX2GameUnit* pDefenderGameUnit = ( null != pDamageData->optrDefenderGameUnit ? pDamageData->optrDefenderGameUnit.GetObservable() : NULL );

    if ( g_pX2Game != NULL )
    {
	    g_pX2Game->GetMajorParticle()->CreateSequence( static_cast<CKTDGObject*>( pDefenderGameUnit ),  L"RevengeImpact", 
													    pDamageData->impactPoint,
													    50, 50, 1, 1 );

	    g_pX2Game->GetMajorParticle()->CreateSequence( static_cast<CKTDGObject*>( pDefenderGameUnit ),  L"DamageImpact", 
		    pDamageData->impactPoint,
		    100, 200, 2, 10 );
	    g_pX2Game->GetMajorParticle()->CreateSequence( static_cast<CKTDGObject*>( pDefenderGameUnit ),  L"DamageImpactCore", 
		    pDamageData->impactPoint,
		    100, 200, 2, 10 );
	    g_pX2Game->GetMajorParticle()->CreateSequence( static_cast<CKTDGObject*>( pDefenderGameUnit ),  L"DamageImpactSlash", 
		    pDamageData->impactPoint,
		    50, 50, 2, 2 );

	    g_pX2Game->GetX2Camera()->GetCamera().UpDownCrashCamera();
    }

	/*
	if( m_bReflect == true )
	{
		D3DXVECTOR3 angle = pMeshInst->GetXSkinAnim()->GetMatrix().GetRotateDegree();
		angle.z += 180.0f;
		m_pManager->CreateInstance( NULL,  pDamageData->pAttackerUnit, m_Name.c_str(),
														pMeshInst->GetXSkinAnim()->GetMatrix().GetPos(),
														angle, angle, m_fLandPos, true );
	}
	*/

	for( int i = 0; i < (int)m_StartParticleDataList.size(); i++ )
	{
		ParticleData* pParticleData = m_StartParticleDataList[i];
		if( pParticleData->bForceDieHit == true )
		{
            if ( g_pX2Game != NULL )
			    g_pX2Game->GetMajorParticle()->DestroyInstanceHandle( pParticleData->m_hSeq );
			SAFE_DELETE( pParticleData );
			m_StartParticleDataList.erase( m_StartParticleDataList.begin() + i );
			i--;
		}			
	}

	for( int i = 0; i < (int)m_PassiveParticleDataList.size(); i++ )
	{
		ParticleData* pParticleData = m_PassiveParticleDataList[i];
		if( pParticleData->bForceDieHit == true )
		{
            if ( g_pX2Game != NULL )
			    g_pX2Game->GetMajorParticle()->DestroyInstanceHandle( pParticleData->m_hSeq );
			SAFE_DELETE( pParticleData );
			m_PassiveParticleDataList.erase( m_PassiveParticleDataList.begin() + i );
			i--;
		}	
	}

	for( int i = 0; i < (int)m_TimeParticleDataList.size(); i++ )
	{
		ParticleData* pParticleData = m_TimeParticleDataList[i];
		if( pParticleData->bForceDieHit == true )
		{
            if ( g_pX2Game != NULL )
			    g_pX2Game->GetMajorParticle()->DestroyInstanceHandle( pParticleData->m_hSeq );
			SAFE_DELETE( pParticleData );
			m_TimeParticleDataList.erase( m_TimeParticleDataList.begin() + i );
			i--;
		}	
	}

	m_bLive = false;

#if defined( _SERVICE_ ) 
	ELSWORD_VIRTUALIZER_END
#endif
}

void CX2DamageEffect::CEffect::AttackResult()
{
#if defined( _SERVICE_ )
	// checked
	ELSWORD_VIRTUALIZER_START
#endif

	KTDXPROFILE();
    
    if( m_DamageData.reActResult == CX2DamageManager::RT_NO_DAMAGE )
        return;

	CX2GameUnit* pOwnerGameUnit = ( null != m_optrOwnerUnit ? m_optrOwnerUnit.GetObservable() : NULL );
	CX2GameUnit *pDefenderGameUnit = m_DamageData.optrDefenderGameUnit.GetObservable();

    bool bIsValidUnit = false;

    if( NULL != g_pX2Game && NULL != pOwnerGameUnit )
    {
        bIsValidUnit = true;

        if( CX2GameUnit::GUT_NPC == pOwnerGameUnit->GetGameUnitType() )
        {
            CX2GUNPC *pNPC = static_cast<CX2GUNPC*>( pOwnerGameUnit );
            pNPC->SetAttackedUnit();
        }

#ifdef SERV_PET_SYSTEM

		if( NULL != pDefenderGameUnit )
		{
			pOwnerGameUnit->SetGameUnitAttackedByMe( pDefenderGameUnit );
		}
#endif

#ifdef SERV_NEW_DEFENCE_DUNGEON // 적용날짜: 2013-04-14
		/// 일반 공격 타격시 이펙트 셋 생성하는 함수 검사
		pOwnerGameUnit->PlayEffectSetNormalAttackByBuff( m_DamageData.attackType );
#endif // SERV_NEW_DEFENCE_DUNGEON

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
		if ( 0.f < m_DamageData.fHitAddHPPer )		/// 공격 성공시, 체력 회복 설정
			pOwnerGameUnit->UpNowHpPerHitOthers( m_DamageData.fHitAddHPPer );
#endif // SERV_ARA_CHANGE_CLASS_SECOND
#ifdef ADD_MEMO_1ST_CLASS //김창한
		if( 0.f < m_DamageData.fHitAddHPbyAttackPower ) /// 공격 성공시, 체력 회복 설정(공격자 물마공 평균에 비례)
			pOwnerGameUnit->UpNowHpPerHitbyAttackPower( m_DamageData.fHitAddHPbyAttackPower );
#endif //ADD_MEMO_1ST_CLASS
    }      


	CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = NULL;
	if( INVALID_MESH_INSTANCE_HANDLE != m_hMeshInstHandle && g_pX2Game != NULL )
		pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->GetMeshInstance( m_hMeshInstHandle );


	if( NULL != pDefenderGameUnit )
	{
		if( true == m_BlackHoleData.m_bEnabled && 
			true == m_BlackHoleData.m_bDragOnlyHittedUnit )
		{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            pDefenderGameUnit->AddEffectiveBlackHoleDamageEffect( GetHandle() );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			pDefenderGameUnit->AddEffectiveBlackHoleDamageEffect( this );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		}


#ifdef STICKY_DAMAGE_EFFECT_TEST
		if( true == m_bTraceAttackedUnit && 
			NULL == m_pTraceUnit &&
			true == m_bEnableAttack )
		{
			m_bEnableAttack = false;

			m_pTraceUnit = pDefenderGameUnit;
			
			if( m_fLifeTimeAfterAttack > 0.f )
			{
				ASSERT( CKTDGXMeshPlayer::LT_TIME == pMeshInst->GetTempletData()->lifeType );
				pMeshInst->SetNowLifeTime( 0.f );
				pMeshInst->SetMaxLifeTime( m_fLifeTimeAfterAttack );

				m_fLifeTimeAfterAttack = -1.f;
			}
		}
#endif STICKY_DAMAGE_EFFECT_TEST


	}	

	switch( m_DamageData.reActResult )
	{
		case CX2DamageManager::RT_NO_DAMAGE:        
			return;
			break;

		case CX2DamageManager::RT_REVENGE:
#ifdef NEW_SKILL_2010_11 // oasis907 : 김상윤 [2010.11.1] // 룬 슬레이어 - 마법저항 수련
		case CX2DamageManager::RT_REFLECT:
#endif NEW_SKILL_2010_11
			{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
                int iReflectCount = 0;
                if ( m_DamageData.hAttackerEffect == GetHandle() )
                    iReflectCount = GetReflectCount();
                else if ( CEffect* pEffect = m_pManager->GetInstance( m_DamageData.hAttackerEffect ) )
                {
                    iReflectCount = pEffect->GetReflectCount();
                }
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
				int iReflectCount = ((CEffect*)m_DamageData.pAttackerEffect)->GetReflectCount();
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
				
				if ( iReflectCount > 0 && g_pX2Game != NULL )
				{
					g_pX2Game->GetMajorParticle()->CreateSequence( static_cast<CKTDGObject*>( pOwnerGameUnit ),  L"RevengeImpactSlashCW", 
						m_DamageData.impactPoint, 
						50, 50, 1, 3 );
					g_pX2Game->GetMajorParticle()->CreateSequence( static_cast<CKTDGObject*>( pOwnerGameUnit ),  L"RevengeImpactSlashCCW", 
						m_DamageData.impactPoint, 
						50, 50, 1, 3 );
					g_pX2Game->GetMajorParticle()->CreateSequence( static_cast<CKTDGObject*>( pOwnerGameUnit ),  L"RevengeImpact", 
						m_DamageData.impactPoint,
						50, 50, 1, 1 );
					CKTDGParticleSystem::CParticleEventSequence* pSeq = NULL;
					pSeq = g_pX2Game->GetMinorParticle()->CreateSequence( NULL,  L"RevengeImpactTick", 
						m_DamageData.impactPoint,
						200, 200, 2, 20 );

					if( pSeq != NULL )
						pSeq->SetLandPosition( m_fLandPos );
				}				

                if ( g_pX2Game != NULL )
                {
				    g_pX2Game->GetWorld()->SetWorldColor( 0xff000000 );
				    g_pX2Game->GetWorld()->FadeWorldColor( g_pX2Game->GetWorld()->GetOriginColor(), 1.0f );
                }

#ifdef FIX_EFFECT_REFLECT
				if( m_bReflect == true && iReflectCount > 0  )
#else
				if( m_bReflect == true )
#endif
				{
					//{{ kimhc // 2009-10-20 // CEffect 전과 후에 BeginTable, EndTable을 호출 하므로 삭제,(일명 독구름 작업)
#ifndef	DAMAGE_EFFECT_WORK		
					m_pLuaManager->EndTable();
#endif	DAMAGE_EFFECT_WORK		
					//{{ kimhc // 2009-10-20 // CEffect 전과 후에 BeginTable, EndTable을 호출 하므로 삭제,(일명 독구름 작업)

					m_bLive = false;
#ifdef FIX_EFFECT_REFLECT
					D3DXVECTOR3 angle = D3DXVECTOR3(0.f, 0.f, 0.f);
					if ( NULL != pMeshInst && NULL != pMeshInst->GetXSkinAnim() )
					{
						angle = pMeshInst->GetXSkinAnim()->GetMatrix().GetRotateDegree();

						angle.z += 180.0f;
						CX2DamageEffect::CEffect* pReflectEffect = m_pManager->CreateInstance( pDefenderGameUnit, m_Name.c_str(), m_fPowerRate,
							pMeshInst->GetXSkinAnim()->GetMatrix().GetPos(),
							angle, angle, m_fLandPos );

						//{{ 09.06.10 태완 : 반사시 3D Plain이고 SetRotate = false인 Passive 파티클 뒤집히게
						if( NULL != pReflectEffect )
						{
							pReflectEffect->SetReflected(true);
							pReflectEffect->SetReflectCount( --iReflectCount );
						}						
					}
#else
					D3DXVECTOR3 angle = pMeshInst->GetXSkinAnim()->GetMatrix().GetRotateDegree();
					angle.z += 180.0f;
					CX2DamageEffect::CEffect* pReflectEffect = m_pManager->CreateInstance( pDefenderGameUnit, m_Name.c_str(), m_fPowerRate,
						pMeshInst->GetXSkinAnim()->GetMatrix().GetPos(),
						angle, angle, m_fLandPos );

					//{{ 09.06.10 태완 : 반사시 3D Plain이고 SetRotate = false인 Passive 파티클 뒤집히게
					if( NULL != pReflectEffect )
					{
						pReflectEffect->SetReflected(true);
					}
					//}}
#endif

					//{{ kimhc // 2009-10-20 // CEffect 전과 후에 BeginTable, EndTable을 호출 하므로 삭제,(일명 독구름 작업)
#ifndef	DAMAGE_EFFECT_WORK		
					m_pLuaManager->BeginTable( m_Name.c_str() );
#endif	DAMAGE_EFFECT_WORK		
					//{{ kimhc // 2009-10-20 // CEffect 전과 후에 BeginTable, EndTable을 호출 하므로 삭제,(일명 독구름 작업)

					return;
				}
			}
			break;

		case CX2DamageManager::RT_CREATE_DAMAGE_EFFECT:
			{
				if( NULL != pOwnerGameUnit )
				{
					if( NULL != pMeshInst )
					{
						D3DXVECTOR3 angle = pMeshInst->GetXSkinAnim()->GetMatrix().GetRotateDegree();
#ifdef ADD_RENA_SYSTEM //김창한
						//스킬, 관련 데이터 값, 첫타인지 체크 하는 값을 상속함
						CX2DamageEffect::CEffect* pDamageEffect = m_pManager->CreateInstance( pOwnerGameUnit,m_DamageData.m_wstrCreateDamageEffectName.c_str(), m_fPowerRate,
							m_DamageData.impactPoint, angle, angle, m_fLandPos );

						if( NULL != pDamageEffect )
						{
							pDamageEffect->GetDamageData().m_eFirstAttack = m_DamageData.m_eFirstAttack;
							pDamageEffect->GetDamageData().m_RelateSkillData = m_DamageData.m_RelateSkillData;
						}
#else //ADD_RENA_SYSTEM
						m_pManager->CreateInstance( pOwnerGameUnit,m_DamageData.m_wstrCreateDamageEffectName.c_str(), m_fPowerRate,
							m_DamageData.impactPoint, angle, angle, m_fLandPos );
#endif //ADD_RENA_SYSTEM

					}
				}
			} break;



		case CX2DamageManager::RT_CREATE_EFFECT_SET:
			{
				if( NULL != pOwnerGameUnit )
				{
					//D3DXVECTOR3 angle = pMeshInst->GetXSkinAnim()->GetMatrix().ATTACK_TIME_END();


					if( NULL != g_pX2Game )
					{
						CX2GameUnit* pTraceUnit = NULL;
						if( true == m_DamageData.m_bCreateEffectSetTraceUnit )
						{
							pTraceUnit = m_DamageData.optrDefenderGameUnit.GetObservable();
						}
#ifdef TRACE_UNIT_DIE_DELETE_EFFECTSET
						CX2EffectSet::Handle hEffectSet = g_pX2Game->GetEffectSet()->PlayEffectSet( m_DamageData.m_wstrCreateEffectSetName.c_str(), pOwnerGameUnit, pTraceUnit, false, m_fPowerRate );

						CX2EffectSet::EffectSetInstance* pInstance = g_pX2Game->GetEffectSet()->GetEffectSetInstance( hEffectSet );
						if( NULL != pInstance )
						{
							pInstance->m_bTraceUnitDieDeleteEffectSet = m_DamageData.m_bTraceUnitDieDeleteEffectSet;
							pInstance->m_bCreateEffectSetTraceUnit = m_DamageData.m_bCreateEffectSetTraceUnit;

#ifdef ADD_RENA_SYSTEM //김창한
							//스킬, 관련 데이터 값, 첫타인지 체크 하는 값을 상속함
							pInstance->m_eFirstAttack = m_DamageData.m_eFirstAttack;
							pInstance->m_RelateSkillData = m_DamageData.m_RelateSkillData;
#endif //ADD_RENA_SYSTEM
						}

#else //TRACE_UNIT_DIE_DELETE_EFFECTSET
						CX2EffectSet::Handle hEffectSet = g_pX2Game->GetEffectSet()->PlayEffectSet( m_DamageData.m_wstrCreateEffectSetName.c_str(), pOwnerGameUnit, pTraceUnit, false, m_fPowerRate
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
                            , -1.f, D3DXVECTOR3( 1, 1, 1 ), false, D3DXVECTOR3( 0, 0, 0 ), D3DXVECTOR3( 0, 0, 0 ), D3DXVECTOR3( 0, 0, 0 ), m_bSkipNpcReactionSimulation
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
                            );
#endif //TRACE_UNIT_DIE_DELETE_EFFECTSET
					}
				}

			} break;

		default:
			{
				if( bIsValidUnit == true )
				{
					if( NULL != pOwnerGameUnit &&  NULL != pOwnerGameUnit->GetComboManager() )
					{
#ifdef DUNGEON_ITEM
						if( m_DamageData.m_fHpPercentUp > 0.f || m_DamageData.m_fMpPercentUp > 0.f )
							break;
#endif


#ifdef SERV_CHUNG_TACTICAL_TROOPER		/// 택티컬 필드는 예외
						if( m_DamageData.m_fIncreaseMP > 0.f )
							break;
#endif SERV_CHUNG_TACTICAL_TROOPER

						if ( m_DamageData.m_bNoDamage )
							break;

						pOwnerGameUnit->GetComboManager()->AddCombo();

#ifdef SERV_PET_SYSTEM
						if(pOwnerGameUnit->GetGameUnitType() == CX2GameUnit::GUT_USER)
						{
							CX2PET *pPet = g_pData->GetPetManager()->GetPet( pOwnerGameUnit->GetUnitUID() );
							if( pPet != NULL )
							{
								pPet->AttackResult( m_DamageData.fHitAddMP );
							}
						}						
#endif
						if( pOwnerGameUnit->GetGameUnitType() == CX2GameUnit::GUT_NPC )
						{
							CX2GUNPC *pNpc = static_cast< CX2GUNPC* >( pOwnerGameUnit );
							CX2AllyNPCAI* pNPCAI = (CX2AllyNPCAI*) pNpc->GetNPCAI();
							if ( pNPCAI != NULL && pNPCAI->GetAIType() == CX2NPCAI::NAT_ALLY 
								&& NULL != pNPCAI->GetAllyGameUnit() )
							{
								CX2GUUser *pUser = static_cast<CX2GUUser*>( pNPCAI->GetAllyGameUnit() );
								if( pUser->GetComboManager() != NULL )
									pUser->GetComboManager()->AddCombo();
							}
						}

#ifdef SERV_ARME_DIMENSION_WITCH
						if( (	pOwnerGameUnit->GetHitCreateDamageEffectRate() > 0.f && 
								pOwnerGameUnit->GetRandomFloat() <= pOwnerGameUnit->GetHitCreateDamageEffectRate() ) ||
							(	m_DamageData.m_fCreateDamageEffectRate > 0.f &&
								pOwnerGameUnit->GetRandomFloat() <= m_DamageData.m_fCreateDamageEffectRate ) )
						{
							D3DXVECTOR3 angle = pMeshInst->GetXSkinAnim()->GetMatrix().GetRotateDegree();
							m_pManager->CreateInstance( pOwnerGameUnit,m_DamageData.m_wstrCreateDamageEffectName.c_str(), 
								pOwnerGameUnit->GetHitCreateDamageEffectPowerRate(),
								m_DamageData.impactPoint, angle, angle, m_fLandPos );
						}						
#endif

						pOwnerGameUnit->DoDelegateProcessInAttackResult( m_DamageData );
					}
				}
			}
			break;
	}

#ifdef SKILL_CASH_10_TEST
	// 이펙트 타격시 메쉬플레이어의 속성값을 바꾼다.
	// ex:타격시 메쉬 이동속도를 낮춘다.
	if( m_fReactAnimSpeed > 0.f && pMeshInst != NULL )
	{
		 pMeshInst->SetAnimSpeed( m_fReactAnimSpeed );
	}
#endif

	if( bIsValidUnit == true )
	{
		if( pOwnerGameUnit != NULL && pOwnerGameUnit->IsLocalUnit() == true )
		{
#ifdef SERV_9TH_NEW_CHARACTER // 김태환
			/// 애드일 땐, 타격시 DP 수치 올려 주자
			if ( CX2GameUnit::GUT_USER == pOwnerGameUnit->GetGameUnitType() )
			{
				CX2GUUser* pUser = static_cast< CX2GUUser* >( pOwnerGameUnit );

				if ( NULL != pUser && NULL != pUser->GetUnit() && CX2Unit::UT_ADD == pUser->GetUnit()->GetType() )
				{
					/// 프레임당 한번만 DP 회복 처리
					if ( false == pUser->GetUpDPByAttackThisFrame() )
					{
						pOwnerGameUnit->UpNowDPValueByHit( m_DamageData );
						pUser->SetUpDPByAttackThisFrame( true );
					}
				}
			}
#endif //SERV_9TH_NEW_CHARACTER


			if( pOwnerGameUnit->GetUpMPThisFrame() == false 
#ifdef ADD_DAMAGE_TYPE_BUFF
				&& CX2DamageManager::DT_BUFF != m_DamageData.damageType 
#endif //ADD_DAMAGE_TYPE_BUFF
				)
			{
				int		redAdv						= 0;
				int		blueAdv						= 0;
				float	fIncreaseMpRate				= 1.0f;		// 피*타격시 마나 + 증가치(1.0은 증가가 없는 비율)
				float	fIncreateSoulRate			= 1.0f;		// 피*타격시 각성 + 증가치

#ifdef ELSWORD_WAY_OF_SWORD
				int iOwnerUnitWayOfSwordState = 0;
				if ( pOwnerGameUnit->GetGameUnitType() == CX2GameUnit::GUT_USER )
				{
					CX2GUUser* pUser = static_cast< CX2GUUser* >( pOwnerGameUnit );
					if( CX2Unit::UT_ELSWORD == pUser->GetUnit()->GetType() )
					{
						iOwnerUnitWayOfSwordState = pUser->GetWayOfSwordState();
						float fWSPGain = 0.f;

						fWSPGain = g_pData->GetDamageManager()->CalcWayOfSwordPoint(m_DamageData.m_WayofSwordType, m_DamageData.damage.GetAllDamage(), iOwnerUnitWayOfSwordState );
						pUser->SetWayOfSwordPoint( pUser->GetWayOfSwordPoint() + fWSPGain );

						fIncreaseMpRate *= g_pData->GetDamageManager()->CalcVigorMpUpOnAttack(m_DamageData.m_WayofSwordType, iOwnerUnitWayOfSwordState );
					}
#ifdef NEW_CHARACTER_EL // 검의 길
					else if( CX2Unit::UT_ELESIS == pUser->GetUnit()->GetType() )
					{
						iOwnerUnitWayOfSwordState = pUser->GetWayOfSwordState();
						float fWSPGain = 0.f;

						fWSPGain = g_pData->GetDamageManager()->CalcWayOfSwordPoint(m_DamageData.m_WayofSwordType, m_DamageData.damage.GetAllDamage(), iOwnerUnitWayOfSwordState );
						pUser->SetWayOfSwordPoint( pUser->GetWayOfSwordPoint() + fWSPGain );

						fIncreaseMpRate  *= g_pData->GetDamageManager()->CalcVigorMpUpOnAttack_Elesis(m_DamageData.m_WayofSwordType, iOwnerUnitWayOfSwordState );
					}
#endif //NEW_CHARACTER_EL
				}

#endif ELSWORD_WAY_OF_SWORD
				//{{ kimhc // 2009-11-17 // 길드 스킬 중 정신집중, 집중된 마력, 격노, 분노해방
#ifdef	GUILD_SKILL
				if ( pOwnerGameUnit->GetGameUnitType() == CX2GameUnit::GUT_USER )
				{
					CX2GUUser* pUser = static_cast< CX2GUUser* >( pOwnerGameUnit );
					BYTE byMemberShipGrade = pUser->GetUnit()->GetUnitData().m_byMemberShipGrade;

					const CX2SkillTree::SkillTemplet* pSkillTemplet;

					// 집중된 마력

					pSkillTemplet = pUser->GetUnit()->GetUnitData().m_UserSkillTree.GetUserSkillTemplet( CX2SkillTree::SI_GP_COMMON_CONCENTRATION_MAGICAL_POWER, byMemberShipGrade);

					if( NULL != pSkillTemplet )
					{
	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
						const CX2UserSkillTree& userSkillTree = pUser->GetUnit()->GetUnitData().m_UserSkillTree;
	
						const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( CX2SkillTree::SI_GP_COMMON_CONCENTRATION_MAGICAL_POWER, true ) );	/// 스킬 레벨
	
						fIncreaseMpRate *= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_MP_GAIN_ON_HIT_REL, iSkillTempletLevel );
	#else // UPGRADE_SKILL_SYSTEM_2013
						fIncreaseMpRate *= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_MP_GAIN_ON_HIT_REL );
	#endif // UPGRADE_SKILL_SYSTEM_2013
					}
					
					// 분노해방

					pSkillTemplet = pUser->GetUnit()->GetUnitData().m_UserSkillTree.GetUserSkillTemplet( CX2SkillTree::SI_GP_COMMON_LIBERATION_OF_ANGER, byMemberShipGrade);

					if( NULL != pSkillTemplet)
					{
#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
						const CX2UserSkillTree& userSkillTree = pUser->GetUnit()->GetUnitData().m_UserSkillTree;

						const int iSkillTempletLevel = max( 1, userSkillTree.GetSkillLevel( CX2SkillTree::SI_GP_COMMON_LIBERATION_OF_ANGER, true ) );	/// 스킬 레벨

						fIncreateSoulRate *= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_SOUL_GAIN_ON_HIT_REL, iSkillTempletLevel );
#else // UPGRADE_SKILL_SYSTEM_2013
						fIncreateSoulRate *= pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_SOUL_GAIN_ON_HIT_REL );
#endif // UPGRADE_SKILL_SYSTEM_2013
					}
				}

#endif	GUILD_SKILL
				//}} kimhc // 2009-11-17 // 길드 스킬 중 정신집중, 집중된 마력, 격노, 분노해방
				if( g_pX2Room != NULL && g_pData->GetPVPRoom() != NULL )
				{
					if( g_pX2Room->GetRoomType() == CX2Room::RT_PVP )
					{
						if( g_pData->GetPVPRoom()->GetPVPGameType() == CX2PVPRoom::PGT_TEAM
							|| g_pData->GetPVPRoom()->GetPVPGameType() == CX2PVPRoom::PGT_TEAM_DEATH )
						{
							int redNum		= g_pX2Room->GetTeamNum( CX2Room::TN_RED, true );
							int blueNum		= g_pX2Room->GetTeamNum( CX2Room::TN_BLUE, true );
							if( redNum > blueNum )
							{
								blueAdv = redNum - blueNum;
							}
							else if( redNum < blueNum )
							{
								redAdv = blueNum - redNum;
							}
						}					
					}
				}
				//{{ dmlee 2008.04.11 캐시 아이템 착용시
				switch( pOwnerGameUnit->GetGameUnitType() )
				{
				case CX2GameUnit::GUT_USER:
					{
						CX2GUUser* pGUUser = static_cast<CX2GUUser*>( pOwnerGameUnit );
						if( pGUUser->GetCashItemAbility().m_fUpMPAtAttackOrDamage > 0.f )
						{
							pGUUser->UpNowMp( pGUUser->GetCashItemAbility().m_fUpMPAtAttackOrDamage );
						}
					} break;
				}
				//}} dmlee 2008.04.11 캐시 아이템 착용시

				if( NULL != pOwnerGameUnit && pOwnerGameUnit->GetGameUnitType() == CX2GameUnit::GUT_USER &&
					CX2DamageManager::AT_SPECIAL != m_DamageData.attackType )
				{
					CX2GUUser* pGUUser = static_cast<CX2GUUser*>( pOwnerGameUnit );
					pGUUser->ProcessAttackResult();
				}


				float fUpMPScaleBySkill = 1.f;
#ifdef NEW_SKILL_TREE
				if( m_DamageData.attackType != CX2DamageManager::AT_SPECIAL && 
					CX2GameUnit::GUT_USER == pOwnerGameUnit->GetGameUnitType() )
				{
					CX2GUUser* pUser = static_cast<CX2GUUser*>( pOwnerGameUnit );
					
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
					int iSkillLevel = pUser->GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_ESK_GET_MANA_UP, true );

					if( iSkillLevel > 0 )
					{
						const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_ESK_GET_MANA_UP );

						if( NULL != pSkillTemplet )
						{
							fUpMPScaleBySkill = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_MP_GAIN_ON_HIT_REL, iSkillLevel );
						}
					}
#else //UPGRADE_SKILL_SYSTEM_2013
					int iSkillLevel = pUser->GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_COMMON_GET_MANA_UP );

					if( iSkillLevel > 0 )
					{
						const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_COMMON_GET_MANA_UP, iSkillLevel );

						if( NULL != pSkillTemplet )
						{
							fUpMPScaleBySkill = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_MP_GAIN_ON_HIT_REL );
						}
					}
#endif //UPGRADE_SKILL_SYSTEM_2013

				}
#endif NEW_SKILL_TREE

#ifdef BALANCE_GRAND_ARCHER_20121213
				if( m_DamageData.attackType != CX2DamageManager::AT_SPECIAL && 
					CX2GameUnit::GUT_USER == pOwnerGameUnit->GetGameUnitType() )
				{
					CX2GUUser* pUser = static_cast<CX2GUUser*>( pOwnerGameUnit );

	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
					int iSkillLevel = pUser->GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_LGA_COMMUNE_OF_NATURE, true );
					if( iSkillLevel > 0 )
					{
						const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_LGA_COMMUNE_OF_NATURE );

						if( NULL != pSkillTemplet )
						{
							fUpMPScaleBySkill = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_MP_GAIN_ON_HIT_REL, iSkillLevel );
						}
					}
	#else // UPGRADE_SKILL_SYSTEM_2013
					int iSkillLevel = pUser->GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_LGA_COMMUNE_OF_NATURE );
					if( iSkillLevel > 0 )
					{
						const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_LGA_COMMUNE_OF_NATURE, iSkillLevel );

						if( NULL != pSkillTemplet )
						{
							fUpMPScaleBySkill = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_MP_GAIN_ON_HIT_REL );
						}
					}
	#endif // UPGRADE_SKILL_SYSTEM_2013
				}
#endif //BALANCE_GRAND_ARCHER_20121213
				//{{ JHKang / 강정훈 / 2011.7.4 / 청 2차 전직 데들리 체이서 - 정밀한 사격술(마나 회수)
#ifdef CHUNG_SECOND_CLASS_CHANGE
				if ( CX2DamageManager::DT_MAGIC == m_DamageData.damageType &&
					CX2GameUnit::GUT_USER == pOwnerGameUnit->GetGameUnitType() )
				{
					CX2GUUser* pUser = static_cast<CX2GUUser*>( pOwnerGameUnit );

	#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
					int iSkillLevel = pUser->GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_CDC_ACCURATE_MARKMANSHIP, true );
					if ( iSkillLevel > 0 )
					{
						const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_CDC_ACCURATE_MARKMANSHIP );

						if ( NULL != pSkillTemplet )
						{
							fUpMPScaleBySkill = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_MP_GAIN_ON_HIT_REL, iSkillLevel );
						}
					}
	#else // UPGRADE_SKILL_SYSTEM_2013
					int iSkillLevel = pUser->GetUnit()->GetUnitData().m_UserSkillTree.GetSkillLevel( CX2SkillTree::SI_P_CDC_ACCURATE_MARKMANSHIP );
					if ( iSkillLevel > 0 )
					{
						const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( CX2SkillTree::SI_P_CDC_ACCURATE_MARKMANSHIP, iSkillLevel );

						if ( NULL != pSkillTemplet )
						{
							fUpMPScaleBySkill = pSkillTemplet->GetSkillAbilityValue( CX2SkillTree::SA_MP_GAIN_ON_HIT_REL );
						}
					}
	#endif // UPGRADE_SKILL_SYSTEM_2013
				}
#endif

				//}}
				
				if( pOwnerGameUnit->GetTeam() == CX2Room::TN_RED )
				{
					pOwnerGameUnit->UpNowMpPerHitOthersWithDamageEffect( m_DamageData.fHitAddMP, redAdv, fUpMPScaleBySkill * fIncreaseMpRate );

#ifdef DUNGEON_ITEM

	#ifdef SERV_CHUNG_TACTICAL_TROOPER			/// 택티컬 필드는 예외
					if( pOwnerGameUnit->GetHyperModeCount() < 3 && m_DamageData.m_fHpPercentUp <= 0.f && m_DamageData.m_fMpPercentUp <= 0.f && 
						m_DamageData.m_fIncreaseMP <= 0.f )
	#else  SERV_CHUNG_TACTICAL_TROOPER
					if( pOwnerGameUnit->GetHyperModeCount() < 3 && m_DamageData.m_fHpPercentUp <= 0.f && m_DamageData.m_fMpPercentUp <= 0.f )
	#endif SERV_CHUNG_TACTICAL_TROOPER

#else
					if( pOwnerGameUnit->GetHyperModeCount() < 3 )
#endif
					{
						//{{ kimhc // 2011-07-05 // 옵션데이타 수치화 작업
			#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
						/// 버프 적용 구조로 변경
						pOwnerGameUnit->UpNowSoul( ( (1.0f * (1.0f + pOwnerGameUnit->GetHyperChargeSpeed() ) ) + (2.0f * redAdv) ) * fIncreateSoulRate );
			#else // SERV_ADD_LUNATIC_PSYKER
						pOwnerGameUnit->UpNowSoul( ( (1.0f * (1.0f + CX2SocketItem::GetFinalHyperGageChargeSpeedPercent( pOwnerGameUnit->GetSocketData().m_fHyperModeChargeRate, pOwnerGameUnit->GetUnitLevel() ) ) ) + (2.0f * redAdv) ) * fIncreateSoulRate );
			#endif //SERV_ADD_LUNATIC_PSYKER
						//}} kimhc // 2011-07-05 // 옵션데이타 수치화 작업
					}
				}
				else if( pOwnerGameUnit->GetTeam() == CX2Room::TN_BLUE )
				{
					pOwnerGameUnit->UpNowMpPerHitOthersWithDamageEffect( m_DamageData.fHitAddMP, blueAdv, fUpMPScaleBySkill * fIncreaseMpRate );

#ifdef DUNGEON_ITEM

	#ifdef SERV_CHUNG_TACTICAL_TROOPER			/// 택티컬 필드는 예외
					if( pOwnerGameUnit->GetHyperModeCount() < 3 && m_DamageData.m_fHpPercentUp <= 0.f && m_DamageData.m_fMpPercentUp <= 0.f && 
						m_DamageData.m_fIncreaseMP <= 0.f )
	#else  SERV_CHUNG_TACTICAL_TROOPER
					if( pOwnerGameUnit->GetHyperModeCount() < 3 && m_DamageData.m_fHpPercentUp <= 0.f && m_DamageData.m_fMpPercentUp <= 0.f )
	#endif SERV_CHUNG_TACTICAL_TROOPER

#else
					if( pOwnerGameUnit->GetHyperModeCount() < 3 )
#endif
					{
						//{{ kimhc // 2011-07-05 // 옵션데이타 수치화 작업
		#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
						/// 버프 적용 구조로 변경
						pOwnerGameUnit->UpNowSoul( ( (1.0f * (1.0f + pOwnerGameUnit->GetHyperChargeSpeed() ) ) + (2.0f * blueAdv) ) * fIncreateSoulRate );
		#else // SERV_ADD_LUNATIC_PSYKER
						pOwnerGameUnit->UpNowSoul( ( (1.0f * (1.0f + CX2SocketItem::GetFinalHyperGageChargeSpeedPercent( pOwnerGameUnit->GetSocketData().m_fHyperModeChargeRate, pOwnerGameUnit->GetUnitLevel() ) ) ) + (2.0f * blueAdv) ) * fIncreateSoulRate );
		#endif //SERV_ADD_LUNATIC_PSYKER
						//}} kimhc // 2011-07-05 // 옵션데이타 수치화 작업
					}
				}
				else
				{
					pOwnerGameUnit->UpNowMpPerHitOthersWithDamageEffect( m_DamageData.fHitAddMP, 0, fUpMPScaleBySkill * fIncreaseMpRate );

#ifdef DUNGEON_ITEM

	#ifdef SERV_CHUNG_TACTICAL_TROOPER			/// 택티컬 필드는 예외
					if( pOwnerGameUnit->GetHyperModeCount() < 3 && m_DamageData.m_fHpPercentUp <= 0.f && m_DamageData.m_fMpPercentUp <= 0.f && 
						m_DamageData.m_fIncreaseMP <= 0.f )
	#else  SERV_CHUNG_TACTICAL_TROOPER
					if( pOwnerGameUnit->GetHyperModeCount() < 3 && m_DamageData.m_fHpPercentUp <= 0.f && m_DamageData.m_fMpPercentUp <= 0.f)
	#endif SERV_CHUNG_TACTICAL_TROOPER

#else
					if( pOwnerGameUnit->GetHyperModeCount() < 3 )
#endif
					{
						//{{ kimhc // 2011-07-05 // 옵션데이타 수치화 작업
		#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
						/// 버프 적용 구조로 변경
						pOwnerGameUnit->UpNowSoul( ( (1.0f * (1.0f + pOwnerGameUnit->GetHyperChargeSpeed() ) ) ) * fIncreateSoulRate );
		#else // SERV_ADD_LUNATIC_PSYKER
						pOwnerGameUnit->UpNowSoul( ( (1.0f * (1.0f + CX2SocketItem::GetFinalHyperGageChargeSpeedPercent( pOwnerGameUnit->GetSocketData().m_fHyperModeChargeRate, pOwnerGameUnit->GetUnitLevel() ) ) ) ) * fIncreateSoulRate );
		#endif // SERV_ADD_LUNATIC_PSYKER
						
						//}} kimhc // 2011-07-05 // 옵션데이타 수치화 작업
					}
				}
				pOwnerGameUnit->SetUpMPThisFrame( true );



				if( CX2GameUnit::GUT_NPC == pOwnerGameUnit->GetGameUnitType() )
				{
					CX2GUNPC* pNPC = static_cast<CX2GUNPC*>( pOwnerGameUnit );

					if( NULL != pNPC->GetNPCAI() &&
						CX2NPCAI::NAT_ALLY == pNPC->GetNPCAI()->GetAIType() )
					{
						CX2AllyNPCAI* pNPCAI = static_cast<CX2AllyNPCAI*>( pNPC->GetNPCAI() );

						CX2GameUnit *pGameUnit = pNPCAI->GetAllyGameUnit();
					
						if( NULL != pGameUnit )
						{
							pGameUnit->UpNowMp( m_DamageData.fHitAddMP * fUpMPScaleBySkill * fIncreaseMpRate );
						}
					}
				}


				// fix!! 요고는 damage effect 쪽에만 구현되어있음. guuser, gunpc쪽에도 구현해줘야함
				if( CX2GameUnit::GUT_USER == pOwnerGameUnit->GetGameUnitType() )
				{
					if( 0.f != m_DamageData.attackerGet.m_fHP )
					{
						pOwnerGameUnit->UpNowHp( m_DamageData.attackerGet.m_fHP );
					}

					if( 0.f != m_DamageData.attackerGet.m_fMP )
					{
						if( g_pX2Game != NULL && g_pX2Game->GetGameType() == CX2Game::GT_PVP )
						{
							pOwnerGameUnit->UpNowMp( m_DamageData.attackerGet.m_fMP * fUpMPScaleBySkill * fIncreaseMpRate );
						}
						else if( g_pX2Game != NULL && g_pX2Game->GetGameType() == CX2Game::GT_DUNGEON )
						{
							pOwnerGameUnit->UpNowMp( m_DamageData.attackerGet.m_fMP*0.8f * fUpMPScaleBySkill * fIncreaseMpRate );
						}
					}
				}
			}

			if( CX2GameUnit::GUT_NPC == pOwnerGameUnit->GetGameUnitType() )
			{
				if( 0.f != m_DamageData.attackerGet.m_fHP )
				{
					pOwnerGameUnit->UpNowHp( m_DamageData.attackerGet.m_fHP );
				}

				if( 0.f != m_DamageData.attackerGet.m_fMP )
				{
					if( g_pX2Game != NULL && g_pX2Game->GetGameType() == CX2Game::GT_PVP )
					{
						pOwnerGameUnit->UpNowMp( m_DamageData.attackerGet.m_fMP );
					}
					else if( g_pX2Game != NULL && g_pX2Game->GetGameType() == CX2Game::GT_DUNGEON )
					{
						pOwnerGameUnit->UpNowMp( m_DamageData.attackerGet.m_fMP*0.8f );
					}
				}

				CX2GUNPC *pUserNpc = static_cast<CX2GUNPC*>( pOwnerGameUnit );

				if( pUserNpc != NULL && pUserNpc->GetNPCAI() != NULL && 
					pUserNpc->GetNPCAI()->GetAIType() == CX2NPCAI::NAT_ALLY &&
					m_DamageData.fCameraCrashGap > 0.f &&
					m_DamageData.fCameraCrashTime > 0.f &&
                    g_pX2Game != NULL )
				{
					g_pX2Game->GetX2Camera()->GetCamera().UpDownCrashCamera( m_DamageData.fCameraCrashGap, m_DamageData.fCameraCrashTime );
				}
			}

#ifdef SERV_CHUNG_TACTICAL_TROOPER		///  택티컬 필드의 스톱 타임( 0.f )은 적용되면 않된다.
			
	#ifdef SERV_9TH_NEW_CHARACTER // 김태환
			if( 0.f >= m_DamageData.m_fIncreaseMP && CX2DamageManager::DT_BUFF != m_DamageData.damageType )
	#else // SERV_9TH_NEW_CHARACTER
			if( 0.f >= m_DamageData.m_fIncreaseMP )
	#endif // SERV_9TH_NEW_CHARACTER
			
#endif SERV_CHUNG_TACTICAL_TROOPER
				pOwnerGameUnit->SetStopTime( m_DamageData.fStopTimeAtt );	
		}
	
		if( pOwnerGameUnit != NULL && pOwnerGameUnit->IsMyUnit() == true )
		{
			if( m_DamageData.techPoint > 0 && g_pX2Game != NULL )
				g_pX2Game->AddTechPoint( m_DamageData.techPoint );

//#ifdef ATTACK_DELAY_GAGE
//			if( CX2GameUnit::GUT_USER == m_pOwnerUnit->GetGameUnitType() )
//			{
//				CX2GUUser* pGUUser = (CX2GUUser*)m_pOwnerUnit;
//				pGUUser->AddAttackDelay(  ); // 일단 어택딜레이 게이지 사용안함
//			}
//#endif ATTACK_DELAY_GAGE
		}
	
		if(  pOwnerGameUnit != NULL && pOwnerGameUnit->IsFocusUnit() == true )
		{
			if( m_DamageData.fCameraCrashGap > 0.f &&
				m_DamageData.fCameraCrashTime > 0.f )
			{
                if ( g_pX2Game != NULL )
				    g_pX2Game->GetX2Camera()->GetCamera().UpDownCrashCamera( m_DamageData.fCameraCrashGap, m_DamageData.fCameraCrashTime );
			}

#ifdef CLEAR_SCREEN_ON_HIT
	#ifdef VERTICAL_SYNC_OPTION
			if ( false == g_pMain->GetGameOption().GetFlashEffect() )
			{
				g_pKTDXApp->GetDGManager()->ClearScreen( m_DamageData.clearScreen );
				g_pKTDXApp->GetDGManager()->SetClearScreenColor( m_DamageData.clearScreenColor );
			}
	#else
			g_pKTDXApp->GetDGManager()->ClearScreen( m_DamageData.clearScreen );
			g_pKTDXApp->GetDGManager()->SetClearScreenColor( m_DamageData.clearScreenColor );
	#endif
#endif // CLEAR_SCREEN_ON_HIT
		}
	
		if ( CX2DamageManager::RT_DUMMY_DAMAGE != m_DamageData.reActResult &&
			pOwnerGameUnit != NULL && pOwnerGameUnit->IsMyUnit() )
			pOwnerGameUnit->SetHitCount( pOwnerGameUnit->GetHitCount() + 1 );
	}
 
	bool bCreateHitEffect = true;
#ifdef SKILL_CASH_10_TEST

	if( NULL != pDefenderGameUnit 
		&& !(pDefenderGameUnit->EmptyAbsorbEffectAttackList())
		&& true == m_DamageData.bCanReflexMagic )
			bCreateHitEffect = false;	
#endif

#ifdef SEASON3_MONSTER_2011_7
	bool bCreateAbsorbMagicEffect = false;
	if( pDefenderGameUnit != NULL && pDefenderGameUnit->GetGameUnitType() == CX2GameUnit::GUT_NPC )
	{
		CX2GUNPC* pGUNPC = (CX2GUNPC*) pDefenderGameUnit;
#ifdef SERV_ARME_DIMENSION_WITCH	// ADW_DISTORTION
		if( pGUNPC->GetAbsorbMagicState() == true && 
			( m_DamageData.bCanReflexMagic == true || true == pGUNPC->GetAbsorbMagicSpecial() &&
			CX2DamageManager::AT_EFFECT == m_DamageData.attackerType && CX2DamageManager::AT_NORMAL == m_DamageData.attackType ) )
#else
		if( pGUNPC->GetAbsorbMagicState() == true && m_DamageData.bCanReflexMagic == true )
#endif
		{
			bCreateHitEffect = false;
			bCreateAbsorbMagicEffect = true;
		}
	}
#endif SEASON3_MONSTER_2011_7

	if( bCreateHitEffect == true)
	{		
		SetHitParticle();
		SetHitDamageEffect();

		//{{ kimhc // 2010.02.10 //	네메시스 패시브 나소드 기어 강화
#ifdef	EVE_SECOND_CLASS_CHANGE
		if ( NULL != GetOwnerUnit() && GetOwnerUnit()->GetGameUnitType() == CX2GameUnit::GUT_USER )
		{
			CX2GUUser* pUser = static_cast< CX2GUUser* >( GetOwnerUnit() );
			if ( pUser->GetUnitClass() == CX2Unit::UC_EVE_CODE_NEMESIS )
			{
				if ( pUser->DidHitNasodWeapon() == true &&
					 g_pX2Game != NULL &&
					 g_pX2Game->GetEffectSet() != NULL )
				{
						g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_Eve_NasodWeapon", GetOwnerUnit(),
						NULL, false, -1.f, -1.f, D3DXVECTOR3( 1, 1, 1 ), true, m_DamageData.impactPoint );
						pUser->SetHitNasodWeapon( false );
				}
			}
		}
#endif	EVE_SECOND_CLASS_CHANGE
		//}} kimhc // 2010.1.28 //	네메시스 패시브 나소드 기어 강화

	}
#ifdef SEASON3_MONSTER_2011_7
	else if(bCreateAbsorbMagicEffect == true)
	{
#ifdef SERV_ARME_DIMENSION_WITCH	// ADW_DISTORTION
		CX2GUNPC* pGUNPC = (CX2GUNPC*) pDefenderGameUnit;
		
		if ( NULL != pGUNPC && true == pGUNPC->GetAbsorbMagicSpecial() && g_pX2Game != NULL )
		{
			g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_ABSORB_MAGIC_Distortion", pDefenderGameUnit, NULL, false, -1.f, -1.f,
													  D3DXVECTOR3(1, 1, 1), true, m_DamageData.impactPoint );

			g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_ABSORB_MAGIC_Distortion_02", pDefenderGameUnit, NULL, false, -1.f, -1.f,
				D3DXVECTOR3(1, 1, 1), true, pGUNPC->GetBonePos( L"effect_Dummy01" ) );
		}
		else
		{
            if ( g_pX2Game != NULL )
			    g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_ABSORB_MAGIC", pDefenderGameUnit);
		}
#else
        if ( g_pX2Game != NULL )
		g_pX2Game->GetEffectSet()->PlayEffectSet( L"EffectSet_ABSORB_MAGIC", pDefenderGameUnit);
#endif
	}
#endif SEASON3_MONSTER_2011_7
	else
	{
		// 마법흡수 관련 파티클 생성
		SetSuckMagicParticle(L"Flare_Particle_Elsword_Phoenix_Talon01");
		SetSuckMagicParticle(L"Condense_Fire_Elsword_Phoenix_Talon01");
		SetSuckMagicParticle(L"Condense_Fire_Elsword_Phoenix_Talon02");

        if ( g_pX2Game != NULL )    
		g_pX2Game->GetX2Camera()->GetCamera().UpDownCrashCamera();
	}

	m_DamageTimeNow++;
	if( m_DamageTime <= m_DamageTimeNow )
	{
		m_bLive = false;

		for( int i = 0; i < (int)m_StartParticleDataList.size(); i++ )
		{
			ParticleData* pParticleData = m_StartParticleDataList[i];
			if( pParticleData->bForceDieHit == true )
			{
                if ( g_pX2Game != NULL )     
				g_pX2Game->GetMajorParticle()->DestroyInstanceHandle( pParticleData->m_hSeq );
				SAFE_DELETE( pParticleData );
				m_StartParticleDataList.erase( m_StartParticleDataList.begin() + i );
				i--;
			}			
		}

		for( int i = 0; i < (int)m_PassiveParticleDataList.size(); i++ )
		{
			ParticleData* pParticleData = m_PassiveParticleDataList[i];
			if( pParticleData->bForceDieHit == true )
			{
                if ( g_pX2Game != NULL )
				g_pX2Game->GetMajorParticle()->DestroyInstanceHandle( pParticleData->m_hSeq );
				SAFE_DELETE( pParticleData );
				m_PassiveParticleDataList.erase( m_PassiveParticleDataList.begin() + i );
				i--;
			}	
		}
		for( int i = 0; i < (int)m_TimeParticleDataList.size(); i++ )
		{
			ParticleData* pParticleData = m_TimeParticleDataList[i];
			if( pParticleData->bForceDieHit == true )
			{
                if ( g_pX2Game != NULL )
				g_pX2Game->GetMajorParticle()->DestroyInstanceHandle( pParticleData->m_hSeq );
				SAFE_DELETE( pParticleData );
				m_TimeParticleDataList.erase( m_TimeParticleDataList.begin() + i );
				i--;
			}	
		}
	}

#ifdef UPGRADE_RAVEN
	if( NULL != pOwnerGameUnit && pOwnerGameUnit->GetGameUnitType() == CX2GameUnit::GUT_USER )
	{
		CX2GUUser* pGUUser = static_cast<CX2GUUser*>( pOwnerGameUnit );
		pGUUser->AttackResultByType(m_DamageData);

#ifdef PVP_ZOOM_CAMERA
		pGUUser->SetSubZoomCameraTimer();
#endif

	}
#endif

#if defined( _SERVICE_ )
	ELSWORD_VIRTUALIZER_END
#endif
}

#ifdef NEW_SKILL_TREE

bool CX2DamageEffect::CEffect::SetMainEffectScale( float fX, float fY, float fZ )
{
	if( m_hMeshInstHandle != INVALID_MESH_INSTANCE_HANDLE )
	{
		CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = ( g_pX2Game != NULL ) ? g_pX2Game->GetMajorXMeshPlayer()->GetMeshInstance( m_hMeshInstHandle ) : NULL;
		if( pMeshInst != NULL )
		{
			pMeshInst->SetScale( fX, fY, fZ );
			return true;
		}
	}

	return false;
}
bool CX2DamageEffect::CEffect::SetMainEffectScale( D3DXVECTOR3 vScale )
{
	return SetMainEffectScale( vScale.x, vScale.y, vScale.z );
}

void CX2DamageEffect::CEffect::SetPassiveParticleScale( float fX, float fY, float fZ )
{
	m_vPassiveParticleScale = D3DXVECTOR3(fX,fY,fZ);
	//for( int i = 0; i < static_cast<int>(m_PassiveParticleDataList.size()); ++i )
	//{
	//	ParticleData* pData = m_PassiveParticleDataList[i];
	//	if( pData != NULL )
	//	{
	//		pData->vScale.x	= fX;
	//		pData->vScale.y	= fY;
	//		pData->vScale.z	= fZ;
	//	}
	//}
	//return true;
}
void CX2DamageEffect::CEffect::SetPassiveParticleScale( D3DXVECTOR3 vScale )
{
	m_vPassiveParticleScale = vScale;
}

void CX2DamageEffect::CEffect::SetHitDamageEffectScale( float fX, float fY, float fZ )
{
	m_vHitDamageScale = D3DXVECTOR3(fX,fY,fZ);
	//for( int i = 0; i < static_cast<int>(m_vecHitDamageEffect.size()); ++i )
	//{
	//	DamageEffectData* pData = m_vecHitDamageEffect[i];
	//	if( pData != NULL )
	//	{
	//		pData->vMainEffectScale.x = fX;
	//		pData->vMainEffectScale.y = fY;
	//		pData->vMainEffectScale.z = fZ;

	//		pData->vPassiveParticleScale.x = fX;
	//		pData->vPassiveParticleScale.y = fY;
	//		pData->vPassiveParticleScale.z = fZ;
	//	}
	//}

	//return true;
}


void CX2DamageEffect::CEffect::SetHitDamageEffectScale( D3DXVECTOR3 vScale )
{
	m_vHitDamageScale = vScale;
}

void CX2DamageEffect::CEffect::SetDieDamageEffectScale( float fX, float fY, float fZ )
{
	m_vDieDamageScale = D3DXVECTOR3(fX,fY,fZ);
	//for( int i = 0; i < static_cast<int>(m_vecDieDamageEffect.size()); ++i )
	//{
	//	DamageEffectData* pData = m_vecDieDamageEffect[i];
	//	if( pData != NULL )
	//	{
	//		pData->vMainEffectScale.x = fX;
	//		pData->vMainEffectScale.y = fY;
	//		pData->vMainEffectScale.z = fZ;

	//		pData->vPassiveParticleScale.x = fX;
	//		pData->vPassiveParticleScale.y = fY;
	//		pData->vPassiveParticleScale.z = fZ;
	//	}
	//}

	//return true;
}
void CX2DamageEffect::CEffect::SetDieDamageEffectScale( D3DXVECTOR3 vScale )
{
	m_vDieDamageScale = vScale;
}

void CX2DamageEffect::CEffect::SetScale(float fX, float fY, float fZ)
{
	SetMainEffectScale(fX,fY,fZ);
	SetPassiveParticleScale(fX,fY,fZ);
	SetHitDamageEffectScale(fX,fY,fZ);
	SetDieDamageEffectScale(fX,fY,fZ);
}

void CX2DamageEffect::CEffect::SetScale(D3DXVECTOR3 vScale)
{
	SetScale(vScale.x,vScale.y,vScale.z);
}

#endif NEW_SKILL_TREE



#ifdef GIANT_UNIT_GIANT_EFFECT_TEST
	void CX2DamageEffect::CEffect::SetScaleByUnit( const D3DXVECTOR3& vScale )
	{
		ASSERT( 0.f != vScale.x ); 
		ASSERT( 0.f != vScale.y ); 
		ASSERT( 0.f != vScale.z ); 

		SetMainEffectScaleByUnit( vScale );
		//SetPassiveParticleScaleByUnit( vScale );
		SetHitDamageEffectScaleByUnit( vScale );
		SetDieDamageEffectScaleByUnit( vScale );
	}


	bool CX2DamageEffect::CEffect::SetMainEffectScaleByUnit( const D3DXVECTOR3& vScale )
	{
		if( m_hMeshInstHandle != INVALID_MESH_INSTANCE_HANDLE )
		{
			CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = ( g_pX2Game != NULL ) ? g_pX2Game->GetMajorXMeshPlayer()->GetMeshInstance( m_hMeshInstHandle ) : NULL;
			if( pMeshInst != NULL )
			{
				pMeshInst->SetScaleByUnit( vScale );
				return true;
			}
		}

		return false;
	}


#endif GIANT_UNIT_GIANT_EFFECT_TEST
#ifdef EVE_ELECTRA
	D3DXVECTOR3 CX2DamageEffect::CEffect::GetMainEffectScaleByUnit()
	{
		D3DXVECTOR3	vScale = D3DXVECTOR3(1.f, 1.f, 1.f);
		if( m_hMeshInstHandle != INVALID_MESH_INSTANCE_HANDLE )
		{
			CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = ( g_pX2Game != NULL ) ? g_pX2Game->GetMajorXMeshPlayer()->GetMeshInstance( m_hMeshInstHandle ) : NULL;
			if( pMeshInst != NULL )
			{
#ifdef VERIFY_STAT_BY_BUFF
				vScale = pMeshInst->GetVec3ScaleByUnit();
#else
				vScale = pMeshInst->GetScaleByUnit();
#endif // VERIFY_STAT_BY_BUFF

			}
		}
		return vScale;
	}
#endif EVE_ELECTRA








void CX2DamageEffect::CEffect::SetStartParticle()
{
	//{{ kimhc // 2009-10-20 // 기존의 Set함수를 Load와 Set 으로 분류(일명 독구름 작업)
#ifdef	DAMAGE_EFFECT_WORK
	
	if ( m_DamageEffectDataInLua.m_vecStartParticle.size() != 0 )
	{
		BOOST_TEST_FOREACH( ParticleData*, pStartParticleData, m_DamageEffectDataInLua.m_vecStartParticle )
		{
			if ( pStartParticleData != NULL )
				SetParticleTable( pStartParticleData );
		}

		//m_StartParticleDataList.reserve( m_DamageEffectDataInLua.m_vecStartParticle.size() );

		/*std::copy( m_DamageEffectDataInLua.m_vecStartParticle.begin(),
			m_DamageEffectDataInLua.m_vecStartParticle.end(), m_StartParticleDataList.begin() );*/

		m_StartParticleDataList = m_DamageEffectDataInLua.m_vecStartParticle;

		// m_StartParticleDataList 가 소멸자에서 메모리를 해제해 주므로 여기서는 하지 않는다.
		m_DamageEffectDataInLua.m_vecStartParticle.clear();
	}
#else	DAMAGE_EFFECT_WORK
	KTDXPROFILE();

	int index = 0;
	while( m_pLuaManager->BeginTable( "START_PARTICLE", index ) == true )
	{
		ParticleData* pData = LoadParticleTable();
		if( pData != NULL )
			m_StartParticleDataList.push_back( pData );

		index++;

		m_pLuaManager->EndTable();
	}
#endif	DAMAGE_EFFECT_WORK
	//}} kimhc // 2009-10-20 // 기존의 Set함수를 Load와 Set 으로 분류(일명 독구름 작업)

	
}

void CX2DamageEffect::CEffect::SetPassiveParticle()
{

	KTDXPROFILE();



	//{{ kimhc // 2009-10-20 // 기존의 Set함수를 Load와 Set 으로 분류(일명 독구름 작업)
#ifdef	DAMAGE_EFFECT_WORK
	
	if ( m_DamageEffectDataInLua.m_vecPassiveParticle.size() != 0 )
	{
		BOOST_TEST_FOREACH( ParticleData*, pPassiveParticleData, m_DamageEffectDataInLua.m_vecPassiveParticle )
		{
			if ( pPassiveParticleData != NULL )
			{
				SetParticleTable( pPassiveParticleData, true, GetPassiveparticleTriggerTimeRate() );

				if( m_bReflected == true )
				{
					if( pPassiveParticleData->bApplyRotate == false )
					{
						CKTDGParticleSystem::CParticleEventSequence* pSeq = ( g_pX2Game != NULL ) ? g_pX2Game->GetMajorParticle()->GetInstanceSequence( pPassiveParticleData->m_hSeq ) : NULL;
						if(pSeq != NULL && pSeq->GetParticleType() == CKTDGParticleSystem::PT_3D_PLANE)
						{
							pPassiveParticleData->vScale.x = pPassiveParticleData->vScale.x * -1.f;
						}

					}
				}
				//}}

				pPassiveParticleData->vScale.x	*= m_vPassiveParticleScale.x;
				pPassiveParticleData->vScale.y	*= m_vPassiveParticleScale.y;
				pPassiveParticleData->vScale.z	*= m_vPassiveParticleScale.z;

//#ifdef GIANT_UNIT_GIANT_EFFECT_TEST
//				pPassiveParticleData->vScale.x	*= m_vPassiveParticleScaleByUnit.x;
//				pPassiveParticleData->vScale.y	*= m_vPassiveParticleScaleByUnit.y;
//				pPassiveParticleData->vScale.z	*= m_vPassiveParticleScaleByUnit.z;
//#endif GIANT_UNIT_GIANT_EFFECT_TEST
//


			}
		}

		//m_PassiveParticleDataList.reserve( m_DamageEffectDataInLua.m_vecPassiveParticle.size() );

		/*std::copy( m_DamageEffectDataInLua.m_vecPassiveParticle.begin(),
			m_DamageEffectDataInLua.m_vecPassiveParticle.end(), m_PassiveParticleDataList.begin() );*/

		m_PassiveParticleDataList = m_DamageEffectDataInLua.m_vecPassiveParticle;
		
		// m_PassiveParticleDataList 가 소멸자에서 메모리를 해제해 주므로 여기서는 하지 않는다.
		m_DamageEffectDataInLua.m_vecPassiveParticle.clear();
	}
#else	DAMAGE_EFFECT_WORK
//{{AFX

	int index = 0;
	while( m_pLuaManager->BeginTable( "PASSIVE_PARTICLE", index ) == true )
	{
		ParticleData* pData = LoadParticleTable( true, GetPassiveparticleTriggerTimeRate() );	
		if( pData != NULL )
		{
			m_PassiveParticleDataList.push_back( pData );

#ifdef NEW_SKILL_TREE
			//{{ 09.06.10 태완 : 데미지이펙트 반사시 3D_PLANE이고 ApplyRotate == false인 Passive 이펙트 뒤집기
			// 글리터 네크로맨서 유령 때문에 예외처리
			if(m_bReflected == true)
			{
				if( pData->bApplyRotate == false )
				{
					CKTDGParticleSystem::CParticleEventSequence* pSeq = ( g_pX2Game != NULL ) ? g_pX2Game->GetMajorParticle()->GetInstanceSequence( pData->m_hSeq ) : NULL;
					if(pSeq != NULL && pSeq->GetParticleType() == CKTDGParticleSystem::PT_3D_PLANE)
					{
						pData->vScale.x = pData->vScale.x * -1.f;
					}

				}
			}
			//}}

			pData->vScale.x	*= m_vPassiveParticleScale.x;
			pData->vScale.y	*= m_vPassiveParticleScale.y;
			pData->vScale.z	*= m_vPassiveParticleScale.z;
#endif NEW_SKILL_TREE

//
//#ifdef GIANT_UNIT_GIANT_EFFECT_TEST
//			pData->vScale.x	*= m_vPassiveParticleScaleByUnit.x;
//			pData->vScale.y	*= m_vPassiveParticleScaleByUnit.y;
//			pData->vScale.z	*= m_vPassiveParticleScaleByUnit.z;
//#endif GIANT_UNIT_GIANT_EFFECT_TEST


		}

		index++;

		m_pLuaManager->EndTable();
	}
//}}AFX

#endif	DAMAGE_EFFECT_WORK
	//}} kimhc // 2009-10-20 // 기존의 Set함수를 Load와 Set 으로 분류(일명 독구름 작업)
}

void CX2DamageEffect::CEffect::SetTimeParticle()
{
	//{{ kimhc // 2009-10-20 // 기존의 Set함수를 Load와 Set 으로 분류(일명 독구름 작업)
#ifdef	DAMAGE_EFFECT_WORK
	
	if ( m_DamageEffectDataInLua.m_vecTimeParticle.size() != 0 )
	{
		BOOST_TEST_FOREACH( ParticleData*, pTimeParticleData, m_DamageEffectDataInLua.m_vecTimeParticle )
		{
			if ( pTimeParticleData != NULL )
				SetParticleTable( pTimeParticleData, false );
		}

		//m_TimeParticleDataList.reserve( m_DamageEffectDataInLua.m_vecTimeParticle.size() );
		/*std::copy( m_DamageEffectDataInLua.m_vecTimeParticle.begin(),
			m_DamageEffectDataInLua.m_vecTimeParticle.end(), m_TimeParticleDataList.begin() );*/

		m_TimeParticleDataList = m_DamageEffectDataInLua.m_vecTimeParticle;

		// m_TimeParticleDataList 가 소멸자에서 메모리를 해제해 주므로 여기서는 하지 않는다.
		m_DamageEffectDataInLua.m_vecTimeParticle.clear();
	}
#else	DAMAGE_EFFECT_WORK
	KTDXPROFILE();

	int index = 0;
	while( m_pLuaManager->BeginTable( "TIME_PARTICLE", index ) == true )
	{
		ParticleData* pData = LoadParticleTable( false );	
		if( pData != NULL )
			m_TimeParticleDataList.push_back( pData );

		index++;

		m_pLuaManager->EndTable();
	}
#endif	DAMAGE_EFFECT_WORK
	//}} kimhc // 2009-10-20 // 기존의 Set함수를 Load와 Set 으로 분류(일명 독구름 작업)
}

void CX2DamageEffect::CEffect::SetSuckMagicParticle(WCHAR *particleName)
{
	KTDXPROFILE();
	
	ParticleData* pData = LoadParticleSuckMagic(particleName);
	SAFE_DELETE( pData );
}

void CX2DamageEffect::CEffect::SetHitParticle()
{
	//{{ kimhc // 2009-10-20 // 기존의 Set함수를 Load와 Set 으로 분류(일명 독구름 작업)
#ifdef	DAMAGE_EFFECT_WORK
	if ( m_DamageEffectDataInLua.m_vecHitParticle.size() == 0 )
		return;

	BOOST_TEST_FOREACH( ParticleData*, pHitParticleData, m_DamageEffectDataInLua.m_vecHitParticle )
	{
		if ( pHitParticleData != NULL )
		{
			ParticleData* pTempParticleData= new ParticleData( *pHitParticleData );
			SetParticleTable( pTempParticleData );
			SAFE_DELETE( pTempParticleData );
		}
	}

	// 다단 히트시 여러번 불러질 수 있으므로 m_DamageEffectDataInLua.m_vecHitParticle는 여기서 소멸해 주지 않는다.
	//m_DamageEffectDataInLua.m_vecHitParticle.clear();	
#else	DAMAGE_EFFECT_WORK
	KTDXPROFILE();

	int index = 0;
	while( m_pLuaManager->BeginTable( "HIT_PARTICLE", index ) == true )
	{
		ParticleData* pData = LoadParticleTable();
		SAFE_DELETE( pData );
		index++;

		m_pLuaManager->EndTable();
	}
#endif	DAMAGE_EFFECT_WORK
	//}} kimhc // 2009-10-20 // 기존의 Set함수를 Load와 Set 으로 분류(일명 독구름 작업)
}

void CX2DamageEffect::CEffect::SetHitDamageEffect()
{
	//{{ kimhc // 2009-10-20 // 기존의 Set함수를 Load와 Set 으로 분류(일명 독구름 작업)
#ifdef	DAMAGE_EFFECT_WORK
	if ( m_DamageEffectDataInLua.m_vecHitDamageEffect.size() != 0 )
	{
		BOOST_TEST_FOREACH( DamageEffectData*, pHitDamageEffect, m_DamageEffectDataInLua.m_vecHitDamageEffect )
		{
			if ( pHitDamageEffect != NULL )
			{
				DamageEffectData* pTempHitDamageEffect= new DamageEffectData( *pHitDamageEffect );

				m_vecHitDamageEffect.push_back( pTempHitDamageEffect );
			}
		}

		//m_vecHitDamageEffect = m_DamageEffectDataInLua.m_vecHitDamageEffect;

		// 다단 히트시 여러번 불러질 수 있으므로 m_DamageEffectDataInLua.m_vecHitDamageEffect는 여기서 소멸해 주지 않는다.
		//m_DamageEffectDataInLua.m_vecHitDamageEffect.clear();
	}
#else	DAMAGE_EFFECT_WORK
	KTDXPROFILE();


	int index = 0;
	while( m_pLuaManager->BeginTable( "HIT_DAMAGE_EFFECT", index ) == true )
	{

		DamageEffectData* pData = new DamageEffectData;

		PLUA_GET_VALUE_ENUM(	m_pLuaManager, "POSITION_TYPE",		pData->posType,			CX2DamageEffect::POSITION_TYPE, DEPT_BONE );
		PLUA_GET_VALUE(			m_pLuaManager, "DAMAGE_EFFECT_NAME",	pData->damageEffectName,	L""		);
		PLUA_GET_VALUE(			m_pLuaManager, "BONE_NAME",			pData->boneName,		L""		);
		PLUA_GET_VALUE(			m_pLuaManager, "APPLY_ROTATE",			pData->bApplyRotate,	true	);
		PLUA_GET_VALUE(			m_pLuaManager, "APPLY_ROTATE_ORIGIN",		pData->bApplyRotateOrigin,	false	);

		PLUA_GET_VALUE(			m_pLuaManager, "ROTATE_X",				pData->vRotateAngleDegree.x,	0.0f	);
		PLUA_GET_VALUE(			m_pLuaManager, "ROTATE_Y",				pData->vRotateAngleDegree.y,	0.0f	);
		PLUA_GET_VALUE(			m_pLuaManager, "ROTATE_Z",				pData->vRotateAngleDegree.z,	0.0f	);

		PLUA_GET_VALUE(			m_pLuaManager, "OFFSET_X",				pData->vOffset.x,	0.0f	);
		PLUA_GET_VALUE(			m_pLuaManager, "OFFSET_Y",				pData->vOffset.y,	0.0f	);        
		PLUA_GET_VALUE(			m_pLuaManager, "OFFSET_Z",				pData->vOffset.z,	0.0f	);

		PLUA_GET_VALUE(			m_pLuaManager, "MAIN_EFFECT_SCALE_X",	pData->vMainEffectScale.x,	0.0f	);
		PLUA_GET_VALUE(			m_pLuaManager, "MAIN_EFFECT_SCALE_Y",	pData->vMainEffectScale.y,	0.0f	);        
		PLUA_GET_VALUE(			m_pLuaManager, "MAIN_EFFECT_SCALE_Z",	pData->vMainEffectScale.z,	0.0f	);

		PLUA_GET_VALUE(			m_pLuaManager, "PASSIVE_PARTICLE_X",	pData->vPassiveParticleScale.x,	0.0f	);
		PLUA_GET_VALUE(			m_pLuaManager, "PASSIVE_PARTICLE_Y",	pData->vPassiveParticleScale.y,	0.0f	);        
		PLUA_GET_VALUE(			m_pLuaManager, "PASSIVE_PARTICLE_Z",	pData->vPassiveParticleScale.z,	0.0f	);



		//PLUA_GET_VALUE(			m_pLuaManager, "FORCE_DIE",		pData->bForceDie,		false	);
		//PLUA_GET_VALUE(			m_pLuaManager, "FORCE_DIE_HIT",	pData->bForceDieHit,	false	);
		//PLUA_GET_VALUE(			m_pLuaManager, "TRIGGER_COUNT",	pData->triggerCount,	-1		);
		//PLUA_GET_VALUE(			m_pLuaManager, "TRIGGER_TIME",		pData->triggerTime,		-1.0f	);
		//PLUA_GET_VALUE(			m_pLuaManager, "EVENT_TIME",		pData->fEventTime,		-1.0f	);

		m_vecHitDamageEffect.push_back( pData );

		index++;

		m_pLuaManager->EndTable();
	}
#endif	DAMAGE_EFFECT_WORK
	//}} kimhc // 2009-10-20 // 기존의 Set함수를 Load와 Set 으로 분류(일명 독구름 작업)

	CreateHitDamageEffect( m_vecHitDamageEffect );

	for( UINT i=0; i<m_vecHitDamageEffect.size(); i++ )
	{
		DamageEffectData* pData = m_vecHitDamageEffect[i];
		SAFE_DELETE( pData );
	}
	m_vecHitDamageEffect.erase( m_vecHitDamageEffect.begin(), m_vecHitDamageEffect.end() );

}

void CX2DamageEffect::CEffect::SetDieDamageEffect()
{
	//{{ kimhc // 2009-10-20 // 기존의 Set함수를 Load와 Set 으로 분류(일명 독구름 작업)
#ifdef	DAMAGE_EFFECT_WORK
	if ( m_DamageEffectDataInLua.m_vecDieDamageEffect.size() != 0 )
	{
		BOOST_TEST_FOREACH( DamageEffectData*, pDieDamageEffect, m_DamageEffectDataInLua.m_vecDieDamageEffect )
		{
			if ( pDieDamageEffect != NULL )
			{
				DamageEffectData* pTempDieDamageEffect= new DamageEffectData( *pDieDamageEffect );
				if ( NULL != pTempDieDamageEffect )
					m_vecDieDamageEffect.push_back( pTempDieDamageEffect );
			}
		}

		// 다단 히트시 여러번 불러질 수 있으므로 m_DamageEffectDataInLua.m_vecDieDamageEffect는 여기서 소멸해 주지 않는다.
		//m_DamageEffectDataInLua.m_vecDieDamageEffect.clear();

		//m_vecDieDamageEffect = m_DamageEffectDataInLua.m_vecDieDamageEffect;
		//m_DamageEffectDataInLua.m_vecDieDamageEffect.clear();
	}
#else	DAMAGE_EFFECT_WORK
	KTDXPROFILE();


	int index = 0;
	while( m_pLuaManager->BeginTable( "DIE_DAMAGE_EFFECT", index ) == true )
	{

		DamageEffectData* pData = new DamageEffectData;

		PLUA_GET_VALUE_ENUM(	m_pLuaManager, "POSITION_TYPE",			pData->posType,				CX2DamageEffect::POSITION_TYPE, DEPT_BONE );
		PLUA_GET_VALUE(			m_pLuaManager, "DAMAGE_EFFECT_NAME",	pData->damageEffectName,	L""		);
		PLUA_GET_VALUE(			m_pLuaManager, "BONE_NAME",				pData->boneName,			L""		);
		PLUA_GET_VALUE(			m_pLuaManager, "APPLY_ROTATE",			pData->bApplyRotate,		true	);

		PLUA_GET_VALUE(			m_pLuaManager, "ROTATE_X",				pData->vRotateAngleDegree.x,	0.0f	);
		PLUA_GET_VALUE(			m_pLuaManager, "ROTATE_Y",				pData->vRotateAngleDegree.y,	0.0f	);
		PLUA_GET_VALUE(			m_pLuaManager, "ROTATE_Z",				pData->vRotateAngleDegree.z,	0.0f	);

		PLUA_GET_VALUE(			m_pLuaManager, "OFFSET_X",				pData->vOffset.x,	0.0f	);
		PLUA_GET_VALUE(			m_pLuaManager, "OFFSET_Y",				pData->vOffset.y,	0.0f	);        
		PLUA_GET_VALUE(			m_pLuaManager, "OFFSET_Z",				pData->vOffset.z,	0.0f	);

		//PLUA_GET_VALUE(			m_pLuaManager, "FORCE_DIE",		pData->bForceDie,		false	);
		//PLUA_GET_VALUE(			m_pLuaManager, "FORCE_DIE_HIT",	pData->bForceDieHit,	false	);
		//PLUA_GET_VALUE(			m_pLuaManager, "TRIGGER_COUNT",	pData->triggerCount,	-1		);
		//PLUA_GET_VALUE(			m_pLuaManager, "TRIGGER_TIME",		pData->triggerTime,		-1.0f	);
		//PLUA_GET_VALUE(			m_pLuaManager, "EVENT_TIME",		pData->fEventTime,		-1.0f	);

		m_vecDieDamageEffect.push_back( pData );

		index++;

		m_pLuaManager->EndTable();
	}
#endif	DAMAGE_EFFECT_WORK
	//}} kimhc // 2009-10-20 // 기존의 Set함수를 Load와 Set 으로 분류(일명 독구름 작업)

	if ( m_vecDieDamageEffect.empty() == false )
	{
		CreateDieDamageEffect( m_vecDieDamageEffect );
	}

	for( UINT i=0; i<m_vecDieDamageEffect.size(); i++ )
	{
		DamageEffectData* pData = m_vecDieDamageEffect[i];
		SAFE_DELETE( pData );
	}
	m_vecDieDamageEffect.erase( m_vecDieDamageEffect.begin(), m_vecDieDamageEffect.end() );


}


void CX2DamageEffect::CEffect::SetCameraData()
{
#ifdef X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
	if( g_pMain->GetGameOption().GetOptionList().m_bDynamicCamera == false )
		return;
#endif//X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION

	//{{ kimhc // 2009-10-20 // 기존의 Set함수를 Load와 Set 으로 분류(일명 독구름 작업)
#ifdef	DAMAGE_EFFECT_WORK
	if ( m_DamageEffectDataInLua.m_vecCameraData.size() == 0 )
		return;

	//m_CameraDataList.reserve( m_DamageEffectDataInLua.m_vecCameraData.size() );

	/*std::copy( m_DamageEffectDataInLua.m_vecCameraData.begin(), 
		m_DamageEffectDataInLua.m_vecCameraData.end(), m_CameraDataList.begin() );*/

	m_CameraDataList = m_DamageEffectDataInLua.m_vecCameraData;

	m_DamageEffectDataInLua.m_vecCameraData.clear();
#else	DAMAGE_EFFECT_WORK
	KTDXPROFILE();

	int index = 0;
	while( m_pLuaManager->BeginTable( "CAMERA", index ) == true )
	{
		CameraData data;
		PLUA_GET_VALUE_ENUM(	m_pLuaManager, "UNIT_SELECT",		data.unitSelect,	UNIT_SELECT,	DEUS_ALL );
		PLUA_GET_VALUE_ENUM(	m_pLuaManager, "CRASH_TYPE",		data.crashType,		CKTDGCamera::SHAKE_TYPE,		CKTDGCamera::DECT_UP_DOWN );
		PLUA_GET_VALUE(			m_pLuaManager, "SPEED",				data.fSpeed,		0 );
		PLUA_GET_VALUE(			m_pLuaManager, "ACCEL",				data.fAccel,		0 );
		PLUA_GET_VALUE(			m_pLuaManager, "GAP",				data.fGap,			0 );
		PLUA_GET_VALUE(			m_pLuaManager, "TIME",				data.fTime,			0 );
		PLUA_GET_VALUE(			m_pLuaManager, "EVENT_TIME",		data.fEventTime,	0 );				

		m_CameraDataList.push_back( data );

		index++;

		m_pLuaManager->EndTable();
	}
#endif	DAMAGE_EFFECT_WORK
	//}} kimhc // 2009-10-20 // 기존의 Set함수를 Load와 Set 으로 분류(일명 독구름 작업)
}

void CX2DamageEffect::CEffect::SetScreenData()
{
	//{{ kimhc // 2009-10-20 // 기존의 Set함수를 Load와 Set 으로 분류(일명 독구름 작업)
#ifdef	DAMAGE_EFFECT_WORK
	if ( m_DamageEffectDataInLua.m_vecScreenData.size() == 0 )
		return;

	//m_ScreenDataList.reserve( m_DamageEffectDataInLua.m_vecScreenData.size() );

	/*std::copy( m_damageeffectdatainlua.m_vecscreendata.begin(), 
		m_damageeffectdatainlua.m_vecscreendata.end(), m_screendatalist.begin() );*/


	m_ScreenDataList = m_DamageEffectDataInLua.m_vecScreenData;

	m_DamageEffectDataInLua.m_vecScreenData.clear();
#else	DAMAGE_EFFECT_WORK
	KTDXPROFILE();

	int index = 0;
	while( m_pLuaManager->BeginTable( "SCREEN", index ) == true )
	{
		ScreenData data;
		PLUA_GET_VALUE_ENUM(	m_pLuaManager, "UNIT_SELECT",		data.unitSelect,	UNIT_SELECT,	DEUS_ALL );
		PLUA_GET_VALUE(			m_pLuaManager, "EVENT_TIME",		data.fEventTime,	0		);
		PLUA_GET_VALUE(			m_pLuaManager, "COUNT",				data.clearCount,	1		);
		PLUA_GET_VALUE(			m_pLuaManager, "COLOR_A",			data.color.a,		1.0f	);
		PLUA_GET_VALUE(			m_pLuaManager, "COLOR_R",			data.color.r,		1.0f	);
		PLUA_GET_VALUE(			m_pLuaManager, "COLOR_G",			data.color.g,		1.0f	);
		PLUA_GET_VALUE(			m_pLuaManager, "COLOR_B",			data.color.b,		1.0f	);

		m_ScreenDataList.push_back( data );

		index++;

		m_pLuaManager->EndTable();
	}
#endif	DAMAGE_EFFECT_WORK
	//}} kimhc // 2009-10-20 // 기존의 Set함수를 Load와 Set 으로 분류(일명 독구름 작업)
}

void CX2DamageEffect::CEffect::SetSoundData()
{
	//{{ kimhc // 2009-10-20 // 기존의 Set함수를 Load와 Set 으로 분류(일명 독구름 작업)
#ifdef	DAMAGE_EFFECT_WORK
	if ( m_DamageEffectDataInLua.m_vecSoundData.size() == 0 )
		return;

	//m_SoundDataList.reserve( m_DamageEffectDataInLua.m_vecSoundData.size() );

	/*std::copy( m_DamageEffectDataInLua.m_vecSoundData.begin(), 
		m_DamageEffectDataInLua.m_vecSoundData.end(), m_SoundDataList.begin() );*/

	/*std::vector< SoundData >::reverse_iterator vrItr = m_DamageEffectDataInLua.m_vecSoundData.rbegin();

	while( vrItr != m_DamageEffectDataInLua.m_vecSoundData.rend() )
	{
		m_SoundDataList.push_back( *vrItr );
		
	}*/

	m_SoundDataList = m_DamageEffectDataInLua.m_vecSoundData;

	m_DamageEffectDataInLua.m_vecSoundData.clear();
#else	DAMAGE_EFFECT_WORK
	KTDXPROFILE();

	int index = 0;
	while( m_pLuaManager->BeginTable( "SOUND", index ) == true )
	{
		SoundData data;
		PLUA_GET_VALUE(			m_pLuaManager, "EVENT_TIME",		data.fEventTime,		0		);
		PLUA_GET_VALUE(			m_pLuaManager, "FILE_NAME",			data.fileName,			L""		);
		PLUA_GET_VALUE(			m_pLuaManager, "RATE",				data.iRate,				100		);
		PLUA_GET_VALUE(			m_pLuaManager, "USE_GLOBAL_TIME",	data.bUseGlobalTime,	false	);

		PLUA_GET_VALUE(			m_pLuaManager, "LOOP",	data.bLoop,	false	);


		m_SoundDataList.push_back( data );
		index++;
		m_pLuaManager->EndTable();
	}
#endif	DAMAGE_EFFECT_WORK
	//}} kimhc // 2009-10-20 // 기존의 Set함수를 Load와 Set 으로 분류(일명 독구름 작업)
}

void CX2DamageEffect::CEffect::SetDamageEffect()
{
	//{{ kimhc // 2009-10-20 // 기존의 Set함수를 Load와 Set 으로 분류(일명 독구름 작업)
#ifdef	DAMAGE_EFFECT_WORK
	if ( m_DamageEffectDataInLua.m_vecCreateDamageEffect.empty() )
		return;

	m_DamageEffectDataList = m_DamageEffectDataInLua.m_vecCreateDamageEffect;

	// m_DamageEffectDataList 가 소멸자에서 메모리를 해제해 주므로 여기서는 하지 않는다.
	m_DamageEffectDataInLua.m_vecCreateDamageEffect.clear();
#else	DAMAGE_EFFECT_WORK
	int index = 0;
	while( m_pLuaManager->BeginTable( "CREATE_DAMAGE_EFFECT", index ) == true )
	{
		DamageEffectData* pData = new DamageEffectData;
		PLUA_GET_VALUE(			m_pLuaManager, "EVENT_TIME",			pData->fEventTime,	-1.0f		);
		PLUA_GET_VALUE_ENUM(	m_pLuaManager, "POSITION_TYPE",		pData->posType,			CX2DamageEffect::POSITION_TYPE, DEPT_BONE );
		PLUA_GET_VALUE(			m_pLuaManager, "DAMAGE_EFFECT_NAME",	pData->damageEffectName,	L""		);
		PLUA_GET_VALUE(			m_pLuaManager, "BONE_NAME",			pData->boneName,		L""		);
		PLUA_GET_VALUE(			m_pLuaManager, "APPLY_ROTATE",			pData->bApplyRotate,	true	);
		PLUA_GET_VALUE(			m_pLuaManager, "APPLY_ROTATE_ORIGIN",	pData->bApplyRotateOrigin,	false	);

		PLUA_GET_VALUE(			m_pLuaManager, "ROTATE_X",				pData->vRotateAngleDegree.x,	0.0f	);
		PLUA_GET_VALUE(			m_pLuaManager, "ROTATE_Y",				pData->vRotateAngleDegree.y,	0.0f	);
		PLUA_GET_VALUE(			m_pLuaManager, "ROTATE_Z",				pData->vRotateAngleDegree.z,	0.0f	);

		PLUA_GET_VALUE(			m_pLuaManager, "OFFSET_X",				pData->vOffset.x,	0.0f	);
		PLUA_GET_VALUE(			m_pLuaManager, "OFFSET_Y",				pData->vOffset.y,	0.0f	);        
		PLUA_GET_VALUE(			m_pLuaManager, "OFFSET_Z",				pData->vOffset.z,	0.0f	);



		m_DamageEffectDataList.push_back( pData );
		index++;
		m_pLuaManager->EndTable();
	}
#endif	DAMAGE_EFFECT_WORK
	//}} kimhc // 2009-10-20 // 기존의 Set함수를 Load와 Set 으로 분류(일명 독구름 작업)
}

CX2DamageEffect::ParticleData* CX2DamageEffect::CEffect::LoadParticleSuckMagic( WCHAR *particleName )
{
	KTDXPROFILE();   

	ParticleData* pData = new ParticleData;

	pData->posType = DEPT_IMPACT_POINT;
	pData->particleName = particleName;
	pData->vScale = D3DXVECTOR3(1.f, 1.f, 1.f);

	CKTDGParticleSystem::CParticleEventSequence* pSeq = ( g_pX2Game != NULL ) 
        ? g_pX2Game->GetMajorParticle()->CreateSequence( NULL, pData->particleName.c_str(), 0,0,0 ) : NULL;

	if( NULL == pSeq )
	{
		SAFE_DELETE( pData );
		return NULL;
	}

    pData->m_hSeq = pSeq->GetHandle();

	pSeq->SetLandPosition( m_fLandPos );

	SetParticleData( pData );	

	return pData;
}

//{{ kimhc // 2009-10-20 // 기존의 LoadParticleTable을 Load와 Set 으로 분류(일명 독구름 작업)
#ifdef	DAMAGE_EFFECT_WORK
CX2DamageEffect::ParticleData* CX2DamageEffect::CEffect::LoadParticleTable()
{
	KTDXPROFILE();   

	ParticleData* pData = new ParticleData;

	PLUA_GET_VALUE_ENUM(	m_pLuaManager, "POSITION_TYPE",	pData->posType,			CX2DamageEffect::POSITION_TYPE, DEPT_BONE );
	PLUA_GET_VALUE(			m_pLuaManager, "PARTICLE_NAME",	pData->particleName,	L""		);
	PLUA_GET_VALUE(			m_pLuaManager, "APPLY_ROTATE",	pData->bApplyRotate,	true	);
	PLUA_GET_VALUE(			m_pLuaManager, "FORCE_DIE",		pData->bForceDie,		false	);
	PLUA_GET_VALUE(			m_pLuaManager, "FORCE_DIE_HIT",	pData->bForceDieHit,	false	);
	PLUA_GET_VALUE(			m_pLuaManager, "TRIGGER_COUNT",	pData->triggerCount,	-1		);
	PLUA_GET_VALUE(			m_pLuaManager, "TRIGGER_TIME",	pData->triggerTime,		-1.0f	);
	PLUA_GET_VALUE(			m_pLuaManager, "BONE_NAME",		pData->boneName,		L""		);
	PLUA_GET_VALUE(			m_pLuaManager, "EVENT_TIME",	pData->fEventTime,		-1.0f	);
	PLUA_GET_VALUE(			m_pLuaManager, "OFFSET_X",		pData->fOffsetX,		0.0f	);  // added by wonpok
	PLUA_GET_VALUE(			m_pLuaManager, "OFFSET_Y",		pData->fOffsetY,		0.0f	);
	PLUA_GET_VALUE(			m_pLuaManager, "OFFSET_Z",		pData->fOffsetZ,		0.0f	);
	PLUA_GET_VALUE(         m_pLuaManager, "APPLY_ROTATED_OFFSET", pData->bApplyRotatedOffset  , false ); // added by lucidash : 2010.11.24

	//{{ kimhc // 2010.2.9 //	이브 2차전직 아토믹 블래스터에 필요로 추가
#ifdef	EVE_SECOND_CLASS_CHANGE
	PLUA_GET_VALUE(			m_pLuaManager, "ROTATE_X",		pData->vRotateAngleDegree.x,	0.0f	);
	PLUA_GET_VALUE(			m_pLuaManager, "ROTATE_Y",		pData->vRotateAngleDegree.y,	0.0f	);
	PLUA_GET_VALUE(			m_pLuaManager, "ROTATE_Z",		pData->vRotateAngleDegree.z,	0.0f	);
#endif	EVE_SECOND_CLASS_CHANGE
	//}} kimhc // 2010.2.9 //	이브 2차전직 아토믹 블래스터에 필요로 추가

#ifdef DAMAGE_EFFECT_TIMED_PARTICLE_GLOBAL_TIME
	PLUA_GET_VALUE(			m_pLuaManager, "USE_GLOBAL_TIME",	pData->bUseGlobalTime,	false	);
#endif DAMAGE_EFFECT_TIMED_PARTICLE_GLOBAL_TIME

//{{ kimhc // 2010.2.3 //	이브 2차 전직
#ifdef	EVE_SECOND_CLASS_CHANGE
	// 프레임당 움직이는 각
	PLUA_GET_VALUE(			m_pLuaManager, "MOVE_ANGLE_X_PER_FRAME", pData->vMoveAnglePerFrame.x, 0.0f );
	PLUA_GET_VALUE(			m_pLuaManager, "MOVE_ANGLE_Y_PER_FRAME", pData->vMoveAnglePerFrame.y, 0.0f );
	PLUA_GET_VALUE(			m_pLuaManager, "MOVE_ANGLE_Z_PER_FRAME", pData->vMoveAnglePerFrame.z, 0.0f );
#endif	EVE_SECOND_CLASS_CHANGE
//}} kimhc // 2010.2.3 //	이브 2차 전직
	PLUA_GET_VALUE(			m_pLuaManager, "SCALE_X",		pData->vScale.x,		1.0f	);
	PLUA_GET_VALUE(			m_pLuaManager, "SCALE_Y",		pData->vScale.y,		1.0f	);
	PLUA_GET_VALUE(			m_pLuaManager, "SCALE_Z",		pData->vScale.z,		1.0f	);

	PLUA_GET_VALUE(			m_pLuaManager, "TRACE",			pData->bTrace,			true	);
#ifdef EVE_ELECTRA
	PLUA_GET_VALUE(			m_pLuaManager, "TRACE_MORE",	pData->bTraceMore,		false	);
#endif EVE_ELECTRA
#ifdef HIT_PARTICLE_TRACE_UNIT
	PLUA_GET_VALUE(			m_pLuaManager, "TRACE_DEFENDER_UNIT",	pData->bTraceDefenderUnit,		false	);
#endif HIT_PARTICLE_TRACE_UNIT

#ifdef EVE_ELECTRA
	PLUA_GET_VALUE(			m_pLuaManager, "SCALE_BY_MAINEFFECT_SCALE_Y",		pData->bScaleByMainEffectScaleY,		false	);
#endif EVE_ELECTRA

#ifdef BALANCE_PATCH_20120329
	PLUA_GET_VALUE(			m_pLuaManager, "SHOW_INNER_LANDHEIGHT",		pData->fShowLandHeight,		99999.f	);
	
#endif

#ifdef ARA_CHANGE_CLASS_FIRST
	PLUA_GET_VALUE(			m_pLuaManager, "REVERSE_Y",					pData->m_bReverseY,		false	);
#endif // ARA_CHANGE_CLASS_FIRST

#ifdef X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
	PLUA_GET_VALUE_ENUM( m_pLuaManager, "SHOW_LEVEL", pData->m_iShowLevel, CX2GameOption::OptionLevel, (int) CX2GameOption::OL_LOW );
#endif//X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION

	return pData;
	
}

void CX2DamageEffect::CEffect::SetParticleTable( ParticleData* pData, bool bCreate /*= true*/, float fTriggerTimeRate /*= -1.f*/ )
{
	if( bCreate == true )
	{
        pData->m_hSeq = INVALID_PARTICLE_SEQUENCE_HANDLE;
		CKTDGParticleSystem::CParticleEventSequence* pSeq = ( g_pX2Game != NULL )
            ? g_pX2Game->GetMajorParticle()->CreateSequence( NULL, pData->particleName.c_str(), 0,0,0, -1, -1, -1, pData->triggerCount, pData->triggerTime ) : NULL;
        if ( pSeq == NULL )
        {
			//SAFE_DELETE( pData );
            return;
        }
        pData->m_hSeq = pSeq->GetHandle();

		pSeq->SetLandPosition( m_fLandPos );

#ifdef NEW_SKILL_TREE
		if( fTriggerTimeRate != -1.f && pSeq->GetTriggerTime() != -1.f )
		{
			float fTriggerTime = pSeq->GetTriggerTime() * fTriggerTimeRate;	//모든 트리거 타임 셋팅이 끝난후 호출하여 Rate를 적용해준다.
			pSeq->SetTriggerTime( fTriggerTime );
		}
#endif NEW_SKILL_TREE

		SetParticleData( pData );
	}

#ifdef HIT_PARTICLE_TRACE_UNIT
	if( g_pX2Game != NULL && g_pX2Game->GetMajorParticle()->IsLiveInstanceHandle( pData->m_hSeq ) == true 
		&& true == pData->bTraceDefenderUnit )
	{
		if( null != m_DamageData.optrDefenderGameUnit )
		{
			m_DamageData.optrDefenderGameUnit->AddTraceUnitHitParticle( pData->m_hSeq );
		}
	}
#endif HIT_PARTICLE_TRACE_UNIT
}
#else	DAMAGE_EFFECT_WORK
CX2DamageEffect::ParticleData* CX2DamageEffect::CEffect::LoadParticleTable( bool bCreate, float fTriggerTimeRate )
{
	KTDXPROFILE();   

	ParticleData* pData = new ParticleData;

	PLUA_GET_VALUE_ENUM(	m_pLuaManager, "POSITION_TYPE",	pData->posType,			CX2DamageEffect::POSITION_TYPE, DEPT_BONE );
	PLUA_GET_VALUE(			m_pLuaManager, "PARTICLE_NAME",	pData->particleName,	L""		);
	PLUA_GET_VALUE(			m_pLuaManager, "APPLY_ROTATE",	pData->bApplyRotate,	true	);
	PLUA_GET_VALUE(			m_pLuaManager, "FORCE_DIE",		pData->bForceDie,		false	);
	PLUA_GET_VALUE(			m_pLuaManager, "FORCE_DIE_HIT",	pData->bForceDieHit,	false	);
	PLUA_GET_VALUE(			m_pLuaManager, "TRIGGER_COUNT",	pData->triggerCount,	-1		);
	PLUA_GET_VALUE(			m_pLuaManager, "TRIGGER_TIME",	pData->triggerTime,		-1.0f	);
	PLUA_GET_VALUE(			m_pLuaManager, "BONE_NAME",		pData->boneName,		L""		);
	PLUA_GET_VALUE(			m_pLuaManager, "EVENT_TIME",	pData->fEventTime,		-1.0f	);
    PLUA_GET_VALUE(			m_pLuaManager, "OFFSET_X",		pData->fOffsetX,		0.0f	);  // added by wonpok
    PLUA_GET_VALUE(			m_pLuaManager, "OFFSET_Y",		pData->fOffsetY,		0.0f	);
    PLUA_GET_VALUE(			m_pLuaManager, "OFFSET_Z",		pData->fOffsetZ,		0.0f	);

	PLUA_GET_VALUE(			m_pLuaManager, "SCALE_X",		pData->vScale.x,		1.0f	);
	PLUA_GET_VALUE(			m_pLuaManager, "SCALE_Y",		pData->vScale.y,		1.0f	);
	PLUA_GET_VALUE(			m_pLuaManager, "SCALE_Z",		pData->vScale.z,		1.0f	);

	PLUA_GET_VALUE(			m_pLuaManager, "TRACE",			pData->bTrace,			true	);
	PLUA_GET_VALUE(         m_pLuaManager, "APPLY_ROTATED_OFFSET" , pData->bApplyRotatedOffset ,  false ); // added by lucidash.

	if( bCreate == true )
	{
		pData->m_hSeq = g_pX2Game->GetMajorParticle()->CreateSequenceHandle( NULL, pData->particleName.c_str(), 0,0,0, -1, -1, -1, pData->triggerCount, pData->triggerTime );

		CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( pData->m_hSeq );
		if( NULL == pSeq )
		{
			SAFE_DELETE( pData );
			return NULL;
		}
		
		pSeq->SetLandPosition( m_fLandPos );

#ifdef NEW_SKILL_TREE
		if( fTriggerTimeRate != -1.f && pSeq->GetTriggerTime() != -1.f )
		{
			float fTriggerTime = pSeq->GetTriggerTime() * fTriggerTimeRate;	//모든 트리거 타임 셋팅이 끝난후 호출하여 Rate를 적용해준다.
			pSeq->SetTriggerTime( fTriggerTime );
		}
#endif NEW_SKILL_TREE

		SetParticleData( pData );
	
	}

	return pData;
}
#endif	DAMAGE_EFFECT_WORK
//}} kimhc // 2009-10-20 // 기존의 LoadParticleTable을 Load와 Set 으로 분류(일명 독구름 작업)

void CX2DamageEffect::CEffect::CreateHitDamageEffect( vector<DamageEffectData*>& vecHitDamageEffect )
{
	KTDXPROFILE();

	if( NULL == g_pX2Game ||
		null == m_optrOwnerUnit ||
		false == g_pX2Game->GetMajorXMeshPlayer()->IsLiveInstanceHandle( m_hMeshInstHandle ) )
	{
		return;
	}


	CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->GetMeshInstance( m_hMeshInstHandle );
	ASSERT( pMeshInst != NULL );
	if( pMeshInst == NULL )
		return;


	//{{ kimhc // 2009-10-20 // CEffect 전과 후에 BeginTable, EndTable을 호출 하므로 삭제,(일명 독구름 작업)
#ifndef	DAMAGE_EFFECT_WORK	
	// 현재 열려있는 damage effect 테이블을 잠시 닫고 
	m_pLuaManager->EndTable();
#endif	DAMAGE_EFFECT_WORK		
	//{{ kimhc // 2009-10-20 // CEffect 전과 후에 BeginTable, EndTable을 호출 하므로 삭제,(일명 독구름 작업)

	


	for( UINT i=0; i<vecHitDamageEffect.size(); i++ )
	{
		DamageEffectData* pData = vecHitDamageEffect[i];


		D3DXVECTOR3 vAxisAngleDegree( 0, 0, 0);
		D3DXVECTOR3 vRotateAngleDegree( 0, 0, 0);
		D3DXVECTOR3 vPosition(0,0,0);

#ifdef SERV_RENA_NIGHT_WATCHER
		if( DET_ARROW_OF_EXPLOSION != GetType() )
#endif SERV_RENA_NIGHT_WATCHER
		{
			// 본섭 DamageEffect.lua 롤백하지 않고 반영시키기 위해 주석 해제
			// oasis907 : 김상윤 [2010.11.4] // 그랜드 아처 - 진동 사격(패시브)
			if(GetOwnerUnit() != NULL )
			{
				if(GetOwnerUnit()->GetRandomFloat() > GetDamageData().m_fHitDamageEffectInvokeRate )
					continue;
			}
		}

		switch( pData->posType )
		{
		case DEPT_BONE:
			{
				if( pData->bApplyRotate == true )
				{
					vAxisAngleDegree = pMeshInst->GetXSkinAnim()->GetMatrix().GetRotateDegree();
					vRotateAngleDegree = vAxisAngleDegree;
				}
				else
				{
					vAxisAngleDegree = pData->vRotateAngleDegree;
					vRotateAngleDegree = vAxisAngleDegree;
				}
				vPosition = pMeshInst->GetXSkinAnim()->GetCloneFramePosition( pData->boneName.c_str() ); 
			}
			break;

		case DEPT_UNIT_BONE:
			{
				if( null != m_optrOwnerUnit )
				{
					vAxisAngleDegree = m_optrOwnerUnit->GetRotateDegree();

					if( pData->bApplyRotate == true )
					{
						vRotateAngleDegree = vAxisAngleDegree;
					}

					vPosition = m_optrOwnerUnit->GetBonePos( pData->boneName.c_str() );
				}
			}
			break;

		case DEPT_IMPACT_POINT:
			{
#ifdef NEW_SKILL_2010_11 
				//{{ oasis907 : 김상윤 [2010.11.22] // 그랜드 아처 - 진동 사격(패시브)
				if(pData->bApplyRotateOrigin == true)
				{
					vAxisAngleDegree = m_AngleDegree;
					vRotateAngleDegree = vAxisAngleDegree;
				}
				//}} bApplyRotate 도 처리하고 싶지만 기본으로 true가 되어있어 기존 데미지 이펙트에 미칠 영향이 커 처리 안함.
#endif NEW_SKILL_2010_11
				vPosition = m_DamageData.impactPoint;
			}
			break;

#ifdef RAVEN_WEAPON_TAKER
		case DEPT_DIE_POS:
#endif RAVEN_WEAPON_TAKER
		case DEPT_POS:
			{
#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE //김창한
				if( pData->bOnlyApplyRotateOffset == true )
				{
					vAxisAngleDegree = pMeshInst->GetMatrix().GetRotateDegree();
					vRotateAngleDegree = pData->vRotateAngleDegree + vAxisAngleDegree;
				}
				else
#endif //SERV_ELESIS_SECOND_CLASS_CHANGE
				{
					if( pData->bApplyRotate == true )
					{
						vAxisAngleDegree = pMeshInst->GetMatrix().GetRotateDegree();
						vRotateAngleDegree = vAxisAngleDegree;
					}
					else
					{
						vAxisAngleDegree = pData->vRotateAngleDegree;
						vRotateAngleDegree = vAxisAngleDegree;
					}
				}
				vPosition = pMeshInst->GetMatrix().GetPos();			
				
			}
			break;
		}

#ifdef SPECIAL_USE_ITEM
		// oasis907 : 김상윤 [2010.2.10] // 
		if(pData->bApplyRotatedOffset == true)
		{
			D3DXVECTOR3 vDirVector = pMeshInst->GetDirVec();

			if( 0.f != vDirVector.y )
			{
				vDirVector.y = 0.f;
				D3DXVec3Normalize( &vDirVector, &vDirVector );
			}
			if( 0.f != pData->vOffset.x )
			{
				vPosition += vDirVector * pData->vOffset.x;
			}
			if( 0.f != pData->vOffset.y )
			{
				vPosition.y += pData->vOffset.y;
			}
			if( 0.f != pData->vOffset.z )
			{
				if( vDirVector.y != 0.f )
				{
					vDirVector.y = vDirVector.z;
					vDirVector.z = vDirVector.x;
					vDirVector.x = -vDirVector.y;
					vDirVector.y = 0.f;
					D3DXVec3Normalize( &vDirVector, &vDirVector );
				}
				else
				{
					vDirVector.y = vDirVector.z;
					vDirVector.z = vDirVector.x;
					vDirVector.x = -vDirVector.y;
					vDirVector.y = 0.f;
				}
				vPosition += vDirVector * pData->vOffset.z;
			}
		}
		else
		{
			vPosition += pData->vOffset;
		}
#else
		vPosition += pData->vOffset;
#endif SPECIAL_USE_ITEM

		if( m_pManager != NULL )
		{
			float fLandPos = min( m_fLandPos, vPosition.y );

#ifdef INHERIT_FORCEDOWNRATE
			float fForceDownRate = 1.f;
			if(true == m_bInheritForceDownRate)
			{
				fForceDownRate *= m_fForceDownRate;
			}
			CEffect* pEffect = m_pManager->CreateInstance( m_optrOwnerUnit.GetObservable(), pData->damageEffectName.c_str(), m_fPowerRate,
															vPosition, vRotateAngleDegree, vAxisAngleDegree, 
															m_fLandPos, false, -1.f, fForceDownRate);
#else
			CEffect* pEffect = m_pManager->CreateInstance( m_optrOwnerUnit.GetObservable(), pData->damageEffectName.c_str(), m_fPowerRate,
															vPosition, vRotateAngleDegree, vAxisAngleDegree, 
															fLandPos, false );
#endif INHERIT_FORCEDOWNRATE

			if( pEffect != NULL )
			{
				/// 버프팩터를 상속함
				if ( pData->bInheritBuffFactor && !GetDamageData().m_vecBuffFactorPtr.empty() )
					pEffect->GetDamageData().m_vecBuffFactorPtr = GetDamageData().m_vecBuffFactorPtr;

#ifdef NEW_SKILL_2010_11
	#ifdef SERV_RENA_NIGHT_WATCHER
				if( 0.f < GetDamageData().m_fHitDamageEffectDamageRate )
	#endif SERV_RENA_NIGHT_WATCHER
				{
					// oasis907 : 김상윤 [2010.11.4] // 그랜드 아처 - 진동 사격(패시브)
					pEffect->GetDamageData().damage.fPhysic *= GetDamageData().m_fHitDamageEffectDamageRate;
					pEffect->GetDamageData().damage.fMagic	*= GetDamageData().m_fHitDamageEffectDamageRate;
				}
#endif NEW_SKILL_2010_11

				pEffect->SetScale( m_vHitDamageScale );
#ifdef GIANT_UNIT_GIANT_EFFECT_TEST
				pEffect->SetScaleByUnit( m_vHitDamageScaleByUnit );
#endif GIANT_UNIT_GIANT_EFFECT_TEST

#ifdef INHERIT_FORCEDOWNRATE
				pEffect->SetInheritForceDownRate(m_bInheritForceDownRate);
#endif INHERIT_FORCEDOWNRATE

#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE //김창한
				CKTDGXMeshPlayer::CXMeshInstance* pMeshInstHitEffect = pEffect->GetMainEffect();
				if( NULL != pMeshInstHitEffect && pData->fLifeTime > 0.f )
					pMeshInstHitEffect->SetMaxLifeTime( pData->fLifeTime );
#endif //SERV_ELESIS_SECOND_CLASS_CHANGE

#ifdef ADD_RENA_SYSTEM //김창한
				//스킬, 관련 데이터 값, 첫타인지 체크 하는 값을 상속함
				pEffect->GetDamageData().m_eFirstAttack = m_DamageData.m_eFirstAttack;
				pEffect->GetDamageData().m_RelateSkillData = m_DamageData.m_RelateSkillData;
#endif //ADD_RENA_SYSTEM
			}
		}

	}

	//{{ kimhc // 2009-10-20 // CEffect 전과 후에 BeginTable, EndTable을 호출 하므로 삭제,(일명 독구름 작업)
#ifndef	DAMAGE_EFFECT_WORK		
	// 기존의 damage effect 테이블을 다시 열어놓는다
	m_pLuaManager->BeginTable( this->GetName() );
#endif	DAMAGE_EFFECT_WORK		
	//{{ kimhc // 2009-10-20 // CEffect 전과 후에 BeginTable, EndTable을 호출 하므로 삭제,(일명 독구름 작업)

}


void CX2DamageEffect::CEffect::CreateDieDamageEffect( vector<DamageEffectData*>& vecDieDamageEffect )
{
	KTDXPROFILE();

	if( NULL == g_pX2Game || null == m_optrOwnerUnit )
	{
		return;
	}

	//{{ kimhc // 2009-10-20 // CEffect 전과 후에 BeginTable, EndTable을 호출 하므로 삭제,(일명 독구름 작업)
#ifndef	DAMAGE_EFFECT_WORK		
	// 현재 열려있는 damage effect 테이블을 잠시 닫고 
	m_pLuaManager->EndTable();
#endif	DAMAGE_EFFECT_WORK		
	//{{ kimhc // 2009-10-20 // CEffect 전과 후에 BeginTable, EndTable을 호출 하므로 삭제,(일명 독구름 작업)

	for( UINT i=0; i<vecDieDamageEffect.size(); i++ )
	{
		DamageEffectData* pData = vecDieDamageEffect[i];



		D3DXVECTOR3 vAxisAngleDegree( 0, 0, 0);
		D3DXVECTOR3 vRotateAngleDegree( 0, 0, 0);
		D3DXVECTOR3 vPosition(0,0,0);


		switch( pData->posType )
		{
		case DEPT_BONE:
			{
				//{{ JHKang / 강정훈 / 2010/11/24 / 이펙트 종료 시 본 선택
#ifdef NEW_SKILL_2010_11
				if ( false == g_pX2Game->GetMajorXMeshPlayer()->IsLiveInstanceHandle( m_hMeshInstHandle ) )
					return;

				CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->GetMeshInstance( m_hMeshInstHandle );
				ASSERT( pMeshInst != NULL );
				if( pMeshInst == NULL )
					return;

				if( pData->bApplyRotate == true )
				{
					vAxisAngleDegree = pMeshInst->GetXSkinAnim()->GetMatrix().GetRotateDegree();
					vRotateAngleDegree = vAxisAngleDegree;
				}
				else
				{
					vAxisAngleDegree = pData->vRotateAngleDegree;
					vRotateAngleDegree = vAxisAngleDegree;
				}
				vPosition = pMeshInst->GetXSkinAnim()->GetCloneFramePosition( pData->boneName.c_str() ); 
#else
				// not implemented yet!!
				continue;

				//if( pData->bApplyRotate == true )
				//{
				//	vAxisAngleDegree = pMeshInst->GetXSkinAnim()->GetMatrix().GetRotateDegree();
				//	vRotateAngleDegree = vAxisAngleDegree;
				//}
				//vPosition = pMeshInst->GetXSkinAnim()->GetCloneFramePosition( pData->boneName.c_str() );
#endif NEW_SKILL_2010_11
				//}} JHKang / 강정훈 / 2010/11/24
			}
			break;

		case DEPT_UNIT_BONE:
			{
				if( null != m_optrOwnerUnit )
				{
					vAxisAngleDegree = m_optrOwnerUnit->GetRotateDegree();
					if( pData->bApplyRotate == true )
					{
						vRotateAngleDegree = vAxisAngleDegree;
					}

					vPosition = m_optrOwnerUnit->GetBonePos( pData->boneName.c_str() );
				}
			}
			break;

		case DEPT_IMPACT_POINT:
			{
#ifdef NEW_SKILL_2010_11 
				//{{ oasis907 : 김상윤 [2010.11.22] // 그랜드 아처 - 진동 사격(패시브)
				if(pData->bApplyRotateOrigin == true)
				{
					vAxisAngleDegree = m_AngleDegree;
					vRotateAngleDegree = vAxisAngleDegree;
				}
				//}} bApplyRotate 도 처리하고 싶지만 기본으로 true가 되어있어 기존 데미지 이펙트에 미칠 영향이 커 처리 안함.
#endif NEW_SKILL_2010_11
				vPosition = m_DamageData.impactPoint;
			}
			break;
#ifdef RAVEN_WEAPON_TAKER
		case DEPT_DIE_POS:
			{
				if( g_pX2Game->GetMajorXMeshPlayer() &&
					INVALID_MESH_INSTANCE_HANDLE != m_hMeshInstHandle )
				{
					vPosition = m_vLandPositionOnLineMap;

					if( vPosition.y < 0.f )
						continue;

					if( pData->bApplyRotate == true )
					{
						vAxisAngleDegree = m_vMainEffectAxisAngleDegree;
						vRotateAngleDegree = vAxisAngleDegree;
					}
					else
					{
						vAxisAngleDegree = pData->vRotateAngleDegree;
						vRotateAngleDegree = vAxisAngleDegree;
					}
				}
				else
					vPosition = D3DXVECTOR3( 0.f, -1.f, 0.f );
			}
			break;
#endif RAVEN_WEAPON_TAKER

		case DEPT_POS:
			{
#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE //김창한
				if( pData->bOnlyApplyRotateOffset == true )
				{
					vAxisAngleDegree = m_vMainEffectAxisAngleDegree;
					vRotateAngleDegree = pData->vRotateAngleDegree + vAxisAngleDegree;
				}
				else
#endif //SERV_ELESIS_SECOND_CLASS_CHANGE
				{
					if( pData->bApplyRotate == true )
					{
						vAxisAngleDegree = m_vMainEffectAxisAngleDegree;
						vRotateAngleDegree = vAxisAngleDegree;
					}
					else
					{
						vAxisAngleDegree = pData->vRotateAngleDegree;
						vRotateAngleDegree = vAxisAngleDegree;
					}
				}

				vPosition = m_vMainEffectPosition;
			}
			break;
		}

#ifdef SPECIAL_USE_ITEM
		// oasis907 : 김상윤 [2011.12.20]
		if(pData->bApplyRotatedOffset == true)
		{
			CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->GetMeshInstance( m_hMeshInstHandle );
			D3DXVECTOR3	vDirVector = D3DXVECTOR3(0.f, 0.f, 0.f);
			if( pMeshInst == NULL)
			{
				if( g_pX2Game->GetWorld() != NULL && g_pX2Game->GetWorld()->GetLineMap() != NULL )
				{
					D3DXVECTOR3 outPos;
					int			lineIndex = 0;
					if( g_pX2Game->GetWorld()->GetLineMap()->CanDown( GetPos() , LINE_RADIUS, &outPos, &lineIndex, false ) == true )
					{
						const CKTDGLineMap::LineData* pLineData = g_pX2Game->GetWorld()->GetLineMap()->GetLineData( lineIndex );
						if( pLineData != NULL )
						{
							vDirVector = pLineData->dirVector;
							if( false == m_bFollowLineRight )
							{
								vDirVector.x = -vDirVector.x;
								vDirVector.z = -vDirVector.z;
							}
						}
					}
				}
			}
			else
			{
				vDirVector = pMeshInst->GetDirVec();
			}

			if( 0.f != vDirVector.y )
			{
				vDirVector.y = 0.f;
				D3DXVec3Normalize( &vDirVector, &vDirVector );
			}
			if( 0.f != pData->vOffset.x )
			{
				vPosition += vDirVector * pData->vOffset.x;
			}
			if( 0.f != pData->vOffset.y )
			{
				vPosition.y += pData->vOffset.y;
			}
			if( 0.f != pData->vOffset.z )
			{
				if( vDirVector.y != 0.f )
				{
					vDirVector.y = vDirVector.z;
					vDirVector.z = vDirVector.x;
					vDirVector.x = -vDirVector.y;
					vDirVector.y = 0.f;
					D3DXVec3Normalize( &vDirVector, &vDirVector );
				}
				else
				{
					vDirVector.y = vDirVector.z;
					vDirVector.z = vDirVector.x;
					vDirVector.x = -vDirVector.y;
					vDirVector.y = 0.f;
				}
				vPosition += vDirVector * pData->vOffset.z;
			}
		}
		else
		{
			vPosition += pData->vOffset;
		}
#else
		vPosition += pData->vOffset;
#endif SPECIAL_USE_ITEM

		if( m_pManager != NULL )
		{
			float fLandPos = min( m_fLandPos, vPosition.y );

#ifdef INHERIT_FORCEDOWNRATE
			float fForceDownRate = 1.f;
			if(true == m_bInheritForceDownRate)
			{
				fForceDownRate *= m_fForceDownRate;
			}
#ifdef SERV_RENA_NIGHT_WATCHER
			CEffect* pEffect = NULL;
			if( pData->bLandCrashOnly == true || pData->bLifeTimeOnly == true )
			{
				if( (pData->bLandCrashOnly == true && g_pX2Game->GetMajorXMeshPlayer()->IsDyingByCrash( m_hMeshInstHandle ) == true ) ||
					(pData->bLifeTimeOnly == true && g_pX2Game->GetMajorXMeshPlayer()->IsDyingByCrash( m_hMeshInstHandle ) == false ) )
				{
					pEffect = m_pManager->CreateInstance( m_optrOwnerUnit.GetObservable(), pData->damageEffectName.c_str(), m_fPowerRate,
						vPosition, vRotateAngleDegree, vAxisAngleDegree, 
						fLandPos, false, -1.f, fForceDownRate);
				}
			}   
			else
			{
				pEffect = m_pManager->CreateInstance( m_optrOwnerUnit.GetObservable(), pData->damageEffectName.c_str(), m_fPowerRate,
					vPosition, vRotateAngleDegree, vAxisAngleDegree, 
					fLandPos, false, -1.f, fForceDownRate);
			}



#else
			CEffect* pEffect = m_pManager->CreateInstance( m_optrOwnerUnit.GetObservable(), pData->damageEffectName.c_str(), m_fPowerRate,
															vPosition, vRotateAngleDegree, vAxisAngleDegree, 
															fLandPos, false, -1.f, fForceDownRate);
#endif SERV_RENA_NIGHT_WATCHER

#else
			CEffect* pEffect = m_pManager->CreateInstance( m_optrOwnerUnit.GetObservable(), pData->damageEffectName.c_str(), m_fPowerRate,
															vPosition, vRotateAngleDegree, vAxisAngleDegree, 
															fLandPos, false );
#endif INHERIT_FORCEDOWNRATE
			if( pEffect != NULL )
			{	
				/// 버프팩터를 상속함
				if ( pData->bInheritBuffFactor && !GetDamageData().m_vecBuffFactorPtr.empty() )
					pEffect->GetDamageData().m_vecBuffFactorPtr = GetDamageData().m_vecBuffFactorPtr;

				pEffect->SetScale( m_vDieDamageScale );
#ifdef GIANT_UNIT_GIANT_EFFECT_TEST
				pEffect->SetScaleByUnit( m_vDieDamageScaleByUnit );
#endif GIANT_UNIT_GIANT_EFFECT_TEST

#ifdef INHERIT_FORCEDOWNRATE
				pEffect->SetInheritForceDownRate(m_bInheritForceDownRate);
#endif INHERIT_FORCEDOWNRATE

#ifdef SERV_RENA_NIGHT_WATCHER
				pEffect->SetIndex( GetIndex() );					/// 인덱스 상속
				pEffect->SetType( GetType() );						/// 타입 상속

				if( DET_NONE != GetType() )		/// 설정된 타입에 해당하는 동작 검사
				{
					if( null != GetDamageData().optrAttackerGameUnit )
						GetDamageData().optrAttackerGameUnit->CheckDamageEffectType( pEffect );
				}
#endif SERV_RENA_NIGHT_WATCHER

#ifdef SERV_RENA_NIGHT_WATCHER
				AddDieEffectDamage( pEffect );
#endif SERV_RENA_NIGHT_WATCHER

#ifdef SERV_RAVEN_VETERAN_COMMANDER
				if( -1 != pData->iGroupID )		/// 그룹 데미지 설정
					pEffect->SetLaserGroupID( pData->iGroupID );
#endif SERV_RAVEN_VETERAN_COMMANDER
#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE //김창한
				CKTDGXMeshPlayer::CXMeshInstance* pMeshInstDieEffect = pEffect->GetMainEffect();
				if( NULL != pMeshInstDieEffect && pData->fLifeTime > 0.f )
					pMeshInstDieEffect->SetMaxLifeTime( pData->fLifeTime );
#endif //SERV_ELESIS_SECOND_CLASS_CHANGE

#ifdef ADD_RENA_SYSTEM //김창한
				//스킬, 관련 데이터 값, 첫타인지 체크 하는 값을 상속함
				pEffect->GetDamageData().m_eFirstAttack = m_DamageData.m_eFirstAttack;
				pEffect->GetDamageData().m_RelateSkillData = m_DamageData.m_RelateSkillData;
#endif //ADD_RENA_SYSTEM
			}
		}
	}

	//{{ kimhc // 2009-10-20 // CEffect 전과 후에 BeginTable, EndTable을 호출 하므로 삭제,(일명 독구름 작업)
#ifndef	DAMAGE_EFFECT_WORK		
	// 기존의 damage effect 테이블을 다시 열어놓는다
	m_pLuaManager->BeginTable( this->GetName() );
#endif	DAMAGE_EFFECT_WORK		
	//{{ kimhc // 2009-10-20 // CEffect 전과 후에 BeginTable, EndTable을 호출 하므로 삭제,(일명 독구름 작업)

}



void CX2DamageEffect::CEffect::SetParticleData( ParticleData* pData )
{
	KTDXPROFILE();

	if ( g_pX2Game == NULL || pData == NULL || pData->m_hSeq == INVALID_PARTICLE_SEQUENCE_HANDLE )
		return;

	CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( pData->m_hSeq );
	if( NULL == pSeq )
	{
		pData->m_hSeq = INVALID_PARTICLE_SEQUENCE_HANDLE;
		return; 
	}


	CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->GetMeshInstance( m_hMeshInstHandle );
	if( NULL == pMeshInst )
	{
		return;
	}

	//{{ kimhc // 2010.2.3 //	매 프레임 Rotate Angle이 바뀌도록
#ifdef	EVE_SECOND_CLASS_CHANGE
	m_vMoveAngle += pData->vMoveAnglePerFrame;
#endif	EVE_SECOND_CLASS_CHANGE
	//}} kimhc // 2010.2.3 //	매 프레임 Rotate Angle이 바뀌도록



	switch( pData->posType )
	{
	case DEPT_BONE:
		{
			D3DXVECTOR3 angleDegree = pMeshInst->GetXSkinAnim()->GetMatrix().GetRotateDegree();

			//{{ kimhc // 2010.2.3 //	이브 2차 전직
#ifdef	EVE_SECOND_CLASS_CHANGE
			angleDegree += pData->vRotateAngleDegree;
#endif	EVE_SECOND_CLASS_CHANGE
			//}} kimhc // 2010.2.3 //	이브 2차 전직

			
			//if ( pData->bApplyRotatedOffset == false )
			{
				if( pData->bApplyRotate == true )
				{
					//{{ kimhc // 2010.2.3 //	매 프레임 Rotate Angle이 바뀌도록
#ifdef	EVE_SECOND_CLASS_CHANGE
					angleDegree	+= m_vMoveAngle;
#endif	EVE_SECOND_CLASS_CHANGE
					//}} kimhc // 2010.2.3 //	매 프레임 Rotate Angle이 바뀌도록

					pSeq->SetAxisAngle( angleDegree );
					pSeq->SetAddRotate( angleDegree );
				}
			}
						

			// modified by wonpok.
			//{{ lucidash // 2010.11.24 // particle의 offset 을 rotation direction을 고려 해서 더하기.
			//if(pData->bTrace == true)
			//{
				D3DXVECTOR3 framePosition = pMeshInst->GetXSkinAnim()->GetCloneFramePosition( pData->boneName.c_str() );
				D3DXVECTOR3 vPosition(0,0,0);

				if ( pData->bApplyRotatedOffset == true )
				{
					D3DXVECTOR3 vDirVector = pMeshInst->GetDirVec();
					

					if( 0.f != vDirVector.y )
					{
						vDirVector.y = 0.f;
						D3DXVec3Normalize( &vDirVector, &vDirVector );
					}
					if( 0.f != pData->fOffsetX )
					{
						vPosition += vDirVector * pData->fOffsetX;
					}
					if( 0.f != pData->fOffsetY )
					{
						vPosition.y += pData->fOffsetY;
					}
					if( 0.f != pData->fOffsetZ )
					{
						if( vDirVector.y != 0.f )
						{
							vDirVector.y = vDirVector.z;
							vDirVector.z = vDirVector.x;
							vDirVector.x = -vDirVector.y;
							vDirVector.y = 0.f;
							D3DXVec3Normalize( &vDirVector, &vDirVector );
						}
						else
						{
							vDirVector.y = vDirVector.z;
							vDirVector.z = vDirVector.x;
							vDirVector.x = -vDirVector.y;
							vDirVector.y = 0.f;
						}
						vPosition += vDirVector * pData->fOffsetZ;
					}

					/*pData->fOffsetX = vPosition.x;
					pData->fOffsetY = vPosition.y;
					pData->fOffsetZ = vPosition.z;*/
				}
				else
				{
					vPosition = D3DXVECTOR3( pData->fOffsetX ,pData->fOffsetY , pData->fOffsetZ );
				}
				

				//framePosition += D3DXVECTOR3(pData->fOffsetX, pData->fOffsetY, pData->fOffsetZ);
				framePosition += vPosition;
#ifdef EVE_ELECTRA
				pSeq->SetPosition(framePosition, pData->bTraceMore, pData->bTraceMore ); 
#else
				pSeq->SetPosition(framePosition); 
#endif EVE_ELECTRA
			//}		
			//}} lucidash // 2010.11.24 // particle의 offset 을 rotation direction을 고려 해서 더하기.

			//pSeq->SetPosition( pMeshInst->GetXSkinAnim()->GetCloneFramePosition( pData->boneName.c_str() ) );

#ifdef BALANCE_PATCH_20120329
				if( pData->fShowLandHeight <= 50000.f && g_pX2Game->GetWorld() != NULL && g_pX2Game->GetWorld()->GetLineMap() != NULL )
				{
					int iLineIndex = 0;
					D3DXVECTOR3 vPos = g_pX2Game->GetWorld()->GetLineMap()->GetLandPosition( framePosition, 3.f, &iLineIndex );

					if( iLineIndex < 0 || framePosition.y - vPos.y > pData->fShowLandHeight )
						pSeq->SetShowObject(false);
					else
						pSeq->SetShowObject(true);
				}			
#endif
		}
		

		break;

	case DEPT_UNIT_BONE:
		{
			if( null != m_optrOwnerUnit )
			{
				

				//{{ kimhc // 2010.2.3 //	매 프레임 Rotate Angle이 바뀌도록
#ifdef	EVE_SECOND_CLASS_CHANGE
				D3DXVECTOR3 angleDegree = m_optrOwnerUnit->GetRotateDegree();
				angleDegree += pData->vRotateAngleDegree;

				pSeq->SetAxisAngle( angleDegree + m_vMoveAngle );
				if( pData->bApplyRotate == true )
					pSeq->SetAddRotate( angleDegree + m_vMoveAngle );
#else	EVE_SECOND_CLASS_CHANGE
				pSeq->SetAxisAngle( m_optrOwnerUnit->GetRotateDegree() );
				if( pData->bApplyRotate == true )
					pSeq->SetAddRotate( m_optrOwnerUnit->GetRotateDegree() );
#endif	EVE_SECOND_CLASS_CHANGE
				//}} kimhc // 2010.2.3 //	매 프레임 Rotate Angle이 바뀌도록
				
				//if(pData->bTrace == true)
#ifdef EVE_ELECTRA
					pSeq->SetPosition( m_optrOwnerUnit->GetBonePos( pData->boneName.c_str() ), pData->bTraceMore, pData->bTraceMore ); 
#else
					pSeq->SetPosition( m_optrOwnerUnit->GetBonePos( pData->boneName.c_str() ) );
#endif EVE_ELECTRA
			}
		}
		break;

	case DEPT_IMPACT_POINT:
		{
			pSeq->SetPosition( m_DamageData.impactPoint );
#ifdef HIT_PARTICLE_TRACE_UNIT
			if( pData->bTraceDefenderUnit == true)
			{
				if( null != m_DamageData.optrDefenderGameUnit  )
				{
					D3DXVECTOR3 vEffectPos = m_DamageData.optrDefenderGameUnit->GetPos();
					vEffectPos.y += 110.f;
#ifdef EVE_ELECTRA
					pSeq->SetPosition( vEffectPos, pData->bTraceMore, pData->bTraceMore ); 
#else
					pSeq->SetPosition( vEffectPos );
#endif EVE_ELECTRA
					
				}
			}
#endif HIT_PARTICLE_TRACE_UNIT
		}
		break;

	case DEPT_POS:
		{
			D3DXVECTOR3 angleDegree = pMeshInst->GetMatrix().GetRotateDegree();

			//{{ kimhc // 2010.2.3 //	이브 2차 전직
#ifdef	EVE_SECOND_CLASS_CHANGE
			angleDegree += pData->vRotateAngleDegree;
#endif	EVE_SECOND_CLASS_CHANGE
			//}} kimhc // 2010.2.3 //	이브 2차 전직

			if( pData->bApplyRotate == true )
			{
				//{{ kimhc // 2010.2.3 //	매 프레임 Rotate Angle이 바뀌도록
#ifdef	EVE_SECOND_CLASS_CHANGE
				angleDegree	+= m_vMoveAngle;
#endif	EVE_SECOND_CLASS_CHANGE
				//}} kimhc // 2010.2.3 //	매 프레임 Rotate Angle이 바뀌도록

				pSeq->SetAxisAngle( angleDegree );
				pSeq->SetAddRotate( angleDegree );
			}
#ifdef EVE_ELECTRA
			pSeq->SetPosition( pMeshInst->GetMatrix().GetPos(), pData->bTraceMore, pData->bTraceMore ); 
#else
			pSeq->SetPosition( pMeshInst->GetMatrix().GetPos() );
#endif EVE_ELECTRA
			
		}
		break;

	default:
		break;
	}

	pSeq->SetScaleFactor( pData->vScale );
#ifdef EVE_ELECTRA
	if( pData->bScaleByMainEffectScaleY == true )
	{
		float fScaleY = pMeshInst->GetScale().y;
		pSeq->SetScaleFactor(D3DXVECTOR3( fScaleY, fScaleY, 1.f) );
	}
#endif EVE_ELECTRA


#ifdef GIANT_UNIT_GIANT_EFFECT_TEST
	pSeq->SetScaleByUnit( pMeshInst->GetScaleByUnit() );
#endif GIANT_UNIT_GIANT_EFFECT_TEST


#ifdef ARA_CHANGE_CLASS_FIRST
	if( true == pData->m_bReverseY && 
		CKTDGMatrix::BT_NONE != pSeq->GetBillBoardType() )
	{
		if( null != m_optrOwnerUnit && false == m_optrOwnerUnit->GetIsRight() )
		{
			pSeq->SetReverseY( true );
		}
	}
#endif // ARA_CHANGE_CLASS_FIRST

}





void CX2DamageEffect::CEffect::FollowLine()
{
	D3DXVECTOR3 outPos;
	int			lineIndex = 0;

    if ( g_pX2Game == NULL )
        return;

	CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->GetMeshInstance( m_hMeshInstHandle );
	if( NULL == pMeshInst )
	{
		return;
	}

	if( g_pX2Game->GetWorld() != NULL && g_pX2Game->GetWorld()->GetLineMap() != NULL )
	{
		bool bUpdateLandPos = false;
		float fLineRadius = LINE_RADIUS;
		if( IsSamef( pMeshInst->GetTempletData()->elasticCoeffX.GetRandomNumInRange(), 0.f ) == false )
		{
			bUpdateLandPos = true;
			fLineRadius -= 15.f;
		}

		if( g_pX2Game->GetWorld()->GetLineMap()->CanDown( pMeshInst->GetPos() , fLineRadius, &outPos, &lineIndex, false ) == true )
		{
			const CKTDGLineMap::LineData* pLineData = g_pX2Game->GetWorld()->GetLineMap()->GetLineData( lineIndex );
			if( pLineData != NULL )
			{
				D3DXVECTOR3 angle = pLineData->dirDegree;
				angle.x = 0.0f;
				angle.z = 0.0f;
				if( m_bFollowLineRight == false )
				{
					angle.y += 180.0f;
				}
				pMeshInst->SetRotateDegree( angle );
				pMeshInst->SetMoveAxisAngleDegree( angle );

				if( IsSamef(m_BeforePos.x, outPos.x) == false || IsSamef(m_BeforePos.z, outPos.z) == false )
				{
					//pMeshInst->SetLandPosition( outPos.y );

					#ifdef RAVEN_WEAPON_TAKER
					if( true == m_bMustOnLine )
					{
						if( pMeshInst->GetPos().y - outPos.y > 200 )
						{
							pMeshInst->SetVelocity( D3DXVECTOR3( 0.f, 0.f, 0.f ) );
							pMeshInst->SetDirSpeed( 0.f );
							pMeshInst->PlayEnd();
						}
					}
					#endif RAVEN_WEAPON_TAKER
					if( true == m_bFollowLineNew )
					{
						if( fabs(outPos.y - pMeshInst->GetPos().y) > 150.f )
						{
							outPos.y = pMeshInst->GetPos().y;
						}
					}
					else
					{
						outPos.y = pMeshInst->GetPos().y;
					}

					pMeshInst->SetPos( outPos );
				}				
#ifdef ADD_UPDATE_LANDPOS
				else if( bUpdateLandPos == true && pMeshInst->GetToggleRotae() == false )
				{
					pMeshInst->SetToggleRotate(true);
					m_bFollowLineRight = !m_bFollowLineRight;
					D3DXVECTOR3 vRot = pMeshInst->GetRotateDegree();
					if( m_bFollowLineRight == false )
					{
						vRot.y += 180.f;						
					}
					else
					{
						vRot.y -= 180.f;
					}
					pMeshInst->SetRotateDegree( vRot );
					pMeshInst->SetMoveAxisAngleDegree( vRot );
				}
#endif
			}
		}
#ifdef SKILL_CASH_10_TEST
		else
		{
			if(m_bFollowOnLine)
			{
				D3DXVECTOR3 vPos = m_BeforePos;
				m_BeforePos.y = pMeshInst->GetPos().y;
				pMeshInst->SetPos( m_BeforePos );
				if( bUpdateLandPos == true && pMeshInst->GetToggleRotae() == false )
				{
 					pMeshInst->SetToggleRotate(true);
 					m_bFollowLineRight = !m_bFollowLineRight;
 					D3DXVECTOR3 vRot = pMeshInst->GetRotateDegree();
 					if( m_bFollowLineRight == false )
 					{
 						vRot.y += 180.f;				
 					}
 					else
 					{
 						vRot.y -= 180.f;
 					}
 					pMeshInst->SetRotateDegree( vRot );
					pMeshInst->SetMoveAxisAngleDegree( vRot );
				}
			}

#ifdef RAVEN_WEAPON_TAKER
			if( true == m_bMustOnLine )
			{
				pMeshInst->SetVelocity( D3DXVECTOR3( 0.f, 0.f, 0.f ) );
				pMeshInst->SetDirSpeed( 0.f );
				pMeshInst->PlayEnd();
			}
#endif RAVEN_WEAPON_TAKER

		}
#endif
	}
}

void CX2DamageEffect::CEffect::LoadLockOnData()
{
	if( m_pLuaManager->BeginTable( "LOCK_ON_DATA" ) == true )
	{
		PLUA_GET_VALUE_ENUM( m_pLuaManager, "LOCK_ON_TYPE", m_LockOnData.m_LockOnType, LOCK_ON_TYPE, LOT_NONE );
		PLUA_GET_VALUE( m_pLuaManager, "CURVE_SPEED",	m_LockOnData.m_fCurveSpeed, 0.0f );
		//{{ kimhc // 2010.12.17 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
		PLUA_GET_VALUE( m_pLuaManager, "IN_DISTANCE_TO_DIE_EFFECT",	m_LockOnData.m_fInDistanceToDieEffect, -1.0f );
#endif	NEW_CHARACTER_CHUNG
		//}} kimhc // 2010.12.17 //  2010-12-23 New Character CHUNG

		//{{ kimhc // 2011.1.18 // 청 1차 전직
#ifdef	CHUNG_FIRST_CLASS_CHANGE
		// 생성 후 몇초 이후부터 Lock을 수행하기 위해 사용
		PLUA_GET_VALUE( m_pLuaManager, "TIME_UNLOCK_FROM_START",	m_LockOnData.m_fTimeUnlockFromStart, 0.0f );
#endif	CHUNG_FIRST_CLASS_CHANGE
		//}} kimhc // 2011.1.18 // 청 1차 전직
#ifdef UPGRADE_RAVEN
		PLUA_GET_VALUE( m_pLuaManager, "TIME_UNLOCK_AFTER_START",	m_LockOnData.m_fUnLockTime, 9999.f );
#endif
#ifdef SERV_ADD_CHUNG_SHELLING_GUARDIAN
		PLUA_GET_VALUE( m_pLuaManager, "TARGET_RANGE",	m_LockOnData.m_fRange, -1.f );
#endif
#ifdef BALANCE_DEADLY_CHASER_20130214
		// 이전 타겟팅 한 대상을 재 타겟 하지 않도록 하는 기능
		PLUA_GET_VALUE( m_pLuaManager, "NOT_TARGET_PRE_LOCK_ON_TARGET", m_LockOnData.m_bIsNotTargetPreLockOnTarget, false );

	#ifdef _IN_HOUSE_
		//NOT_TARGET_PRE_LOCK_ON_TARGET 기능은 LOT_NEARST_UID_VECTOR_IN_RANGE에서만 사용 가능.
		if( true == m_LockOnData.m_bIsNotTargetPreLockOnTarget && 
			LOT_NEARST_UID_VECTOR_IN_RANGE != m_LockOnData.m_LockOnType )
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), L"LOT_NEARST_UID_VECTOR_IN_RANGE 타입에서만 NOT_TARGET_PRE_LOCK_ON_TARGET 적용 가능", NULL );
		}
	#endif //_IN_HOUSE_
#endif //BALANCE_DEADLY_CHASER_20130214

#ifdef FINALITY_SKILL_SYSTEM //김창한
		PLUA_GET_VALUE( m_pLuaManager, "ONLY_FRONT",	m_LockOnData.m_bIsOnlyFront, false );
		if( m_LockOnData.m_bIsOnlyFront == true && m_optrOwnerUnit != null )
		{
			m_LockOnData.m_bSaveIsRight = m_optrOwnerUnit->GetIsRight();
			m_LockOnData.m_iSaveLastTouchLineIndex = m_optrOwnerUnit->GetLastTouchLineIndex();
			m_LockOnData.m_vecSaveOwnerPos = m_optrOwnerUnit->GetPos();
		}
#endif //FINALITY_SKILL_SYSTEM

#ifdef LOCK_ON_USER_ONLY_ON
		PLUA_GET_VALUE( m_pLuaManager, "ONLY_TARGET_ATTACK",	m_LockOnData.m_bIsOnlyTargetAttack, false );
#endif //LOCK_ON_USER_ONLY_ON

#ifdef BALANCE_PATCH_20131107
		//기능 자체가 범위안에 적이 없으면 멈추는 기능이므로
		//NOT_FIND_TARGET_MOVE_STOP 기능은 LOT_NEARST_UID_VECTOR_IN_RANGE 에서만 사용가능.
		//또한 MeshPlayer 자체에서 DirSpeed값을 Initial로 했을때만을 고려해서 작업함.
		//at 0.200 등으로 중간에 변하는 속도에 대해서는 고려되지 않음.
		bool bNotFindTargetMoveStop = false;
		PLUA_GET_VALUE( m_pLuaManager, "NOT_FIND_TARGET_MOVE_STOP",	bNotFindTargetMoveStop, false );
		m_LockOnData.m_eNotFindTargetMoveStop = (bNotFindTargetMoveStop)? LOCSD_NOT_SAVE_DIR_SPEED : LOCSD_NONE;
#endif //BALANCE_PATCH_20131107

		m_pLuaManager->EndTable();
	}
}



void CX2DamageEffect::CEffect::LoadBlackHoleData()
{
	if( m_pLuaManager->BeginTable( "BLACKHOLE_DATA" ) == true )
	{
		m_BlackHoleData.m_bEnabled = true;
		PLUA_GET_VALUE( m_pLuaManager, "DRAGGING_SPEED", m_BlackHoleData.m_fDraggingSpeed, 1.f );
		PLUA_GET_VALUE( m_pLuaManager, "DRAGGING_RANGE", m_BlackHoleData.m_fDraggingRange, 1.f );

		PLUA_GET_VALUE( m_pLuaManager, "DRAG_ONLY_HITTED_UNIT", m_BlackHoleData.m_bDragOnlyHittedUnit, false );
#ifdef ARA_CHARACTER_BASE
		PLUA_GET_VALUE( m_pLuaManager, "CAN_PASS_UNIT", m_BlackHoleData.m_bCanPassUnit, false );
#endif

		m_pLuaManager->EndTable();
	}
	else
	{
		m_BlackHoleData.m_bEnabled = false;
	}
}



//**
#ifdef _SIN_MOVE_TEST_

void CX2DamageEffect::CEffect::LoadSinMoveData()
{
	if( m_pLuaManager->BeginTable( "SIN_MOVE_DATA" ) == true )
	{
		m_SinMoveData.m_bSinMove = true;
		
		PLUA_GET_VALUE( m_pLuaManager, "SIN_MOVE_MAX",			m_SinMoveData.m_fMoveMax,					0.0f );
		PLUA_GET_VALUE( m_pLuaManager, "SIN_MOVE_MIN",			m_SinMoveData.m_fMoveMin,					0.0f );
		PLUA_GET_VALUE( m_pLuaManager, "SIN_MOVE_TIME_MAX",		m_SinMoveData.m_fTimeMax,					0.0f );
		PLUA_GET_VALUE( m_pLuaManager, "SIN_MOVE_TIME_MIN",		m_SinMoveData.m_fTimeMin,					0.0f );
		
		m_pLuaManager->EndTable();

		if( null != m_optrOwnerUnit )
		{
			float RandomSeed = m_optrOwnerUnit->GetRandomFloat();
			m_SinMoveData.m_fTime = (m_SinMoveData.m_fTimeMax-m_SinMoveData.m_fTimeMin)*RandomSeed + m_SinMoveData.m_fTimeMin;
			m_SinMoveData.m_fMove = (m_SinMoveData.m_fMoveMax-m_SinMoveData.m_fMoveMin)*RandomSeed + m_SinMoveData.m_fMoveMin;
		}
		else
		{
			m_SinMoveData.m_fTime = (m_SinMoveData.m_fTimeMin + m_SinMoveData.m_fTimeMax) / 2;
			m_SinMoveData.m_fMove = (m_SinMoveData.m_fMoveMin + m_SinMoveData.m_fMoveMax) / 2;
		}	
	}
}
#endif _SIN_MOVE_TEST_

//kimhc // 2010-12-17 // D3DXVECTOR3를 인자로 넘기는 것을 const D3DXVECTOR3&로 변경함
void CX2DamageEffect::CEffect::VectorLockOn( const D3DXVECTOR3& targetPos )
{
    if ( g_pX2Game == NULL )
        return;

	CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = g_pX2Game->GetMajorXMeshPlayer()->GetMeshInstance( m_hMeshInstHandle );
	if( NULL == pMeshInst )
	{
		return;
	}

//{{ kimhc // 2010.12.17 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
	// m_fInDistanceToDieEffect 변수에 값이 지정 되어 있으면 판단함
	if ( 0.0f < m_LockOnData.m_fInDistanceToDieEffect )
	{
		// 사정 거리내에 들어오면
		if ( GetDistance( targetPos, pMeshInst->GetPos() ) < m_LockOnData.m_fInDistanceToDieEffect )
		{
			m_hMeshInstHandle = INVALID_MESH_INSTANCE_HANDLE;
			return;
			
		}
	}
#endif	NEW_CHARACTER_CHUNG
//}} kimhc // 2010.12.17 //  2010-12-23 New Character CHUNG
#ifdef FINALITY_SKILL_SYSTEM //김창한
	if( true == m_LockOnData.m_bIsOnlyFront )
	{
		const CKTDGLineMap::LineData* pCurrLineData = g_pX2Game->GetWorld()->GetLineMap()->GetLineData( m_LockOnData.m_iSaveLastTouchLineIndex );
		if(pCurrLineData != NULL)
		{
			D3DXVECTOR3 vFinalDestDir = targetPos - m_LockOnData.m_vecSaveOwnerPos;
			D3DXVec3Normalize( &vFinalDestDir, &vFinalDestDir );
			D3DXVECTOR3 vRightVec = pCurrLineData->dirVector;
			D3DXVec3Normalize( &vRightVec, &vRightVec );

			float fIsRight = D3DXVec3Dot( &vRightVec, &vFinalDestDir );
			bool bRight = true;
			if( fIsRight > 0.f )
				bRight = true;
			else
				bRight = false;

			if( bRight != m_LockOnData.m_bSaveIsRight )
				return;
		}
	}
#endif //FINALITY_SKILL_SYSTEM

	D3DXVECTOR3 axis;	// 회전축
	D3DXVECTOR3 relVec = targetPos - pMeshInst->GetPos();	// 목표와의 상대적 벡터
	D3DXVec3Normalize( &relVec, &relVec );

	//float estTime = D3DXVec3Length(&relVec) / speed;
	//relVec = (targetPos + targetVel*estTime) - pos;		// 예측형 유도
	D3DXVECTOR3 nowVec = pMeshInst->GetManualDirDegree();
	nowVec.x = D3DXToRadian(nowVec.x);
	nowVec.y = D3DXToRadian(nowVec.y);
	nowVec.z = D3DXToRadian(nowVec.z);
	D3DXVec3Normalize( &nowVec, &nowVec );

	D3DXVECTOR3 gapVec = relVec - nowVec;
	nowVec += gapVec * m_LockOnData.m_fCurveSpeed * m_fElapsedTime;

	//D3DXVec3Normalize( &nowVec, &nowVec );
		
	pMeshInst->UseManualDirDegree( true );
	D3DXVECTOR3 nowVecDegree;
	nowVecDegree.x = D3DXToDegree(nowVec.x);
	nowVecDegree.y = D3DXToDegree(nowVec.y);
	nowVecDegree.z = D3DXToDegree(nowVec.z);

	pMeshInst->SetManualDirDegree( nowVecDegree );
	
	//nowVec = GetDirVecToDegree( nowVec );
	//pMeshInst->SetRotateDegree( nowVec );

	/*
	D3DXVec3Cross( &axis, &nowVec, &relVec );

	float inner = D3DXVec3Dot( &nowVec, &relVec );// 내적값

	// 방향의 수정. 상하좌우 90도 이상이면 유도하지 않는다.
	if( inner > 0 )
	{
		// 회전각 : 적당한 크기의 상수 각을 잡아 돌려준다.
		float theta = m_LockOnData.m_fCurveSpeed * m_fElapsedTime;

		// 외적을 통해 나온 축을 중심으로 회전시킨다.
		// 벡터만 따로 회전시키기는 애매하므로,
		// 행렬에 집어넣은 뒤 DirectX에서 지원하는 회전 method를 이용한다.

		D3DXMATRIX velMat(				// 벡터의 행렬화
			0.0F, 0.0F, 0.0F, nowVec.x,	// 4번째 열이 속도
			0.0F, 0.0F, 0.0F, nowVec.y,
			0.0F, 0.0F, 0.0F, nowVec.z,
			0.0F, 0.0F, 0.0F, 0.0F );
		D3DXMATRIX newVelMat;

		// 회전변환용 행렬 생성
		D3DXMATRIX matRotAxis;
		D3DXMatrixRotationAxis( &matRotAxis, &axis, theta );

		newVelMat = matRotAxis * velMat;		// 회전시킨다

		// 변환된 행렬로부터 속도 성분을 뽑아낸다.
		nowVec = D3DXVECTOR3( newVelMat._14, newVelMat._24, newVelMat._34 );
	}
	pMeshInst->UseManualDirDegree( true );
	D3DXVECTOR3 nowVecDegree;
	nowVecDegree.x = D3DXToDegree(nowVec.x);
	nowVecDegree.y = D3DXToDegree(nowVec.y);
	nowVecDegree.z = D3DXToDegree(nowVec.z);

	pMeshInst->SetManualDirDegree( nowVecDegree );
	nowVec = GetDirVecToDegree( nowVec );
	pMeshInst->SetRotateDegree( nowVec );

	//pos += frameTime * vel * speed;		// 유도탄의 위치 변경
	//speed += frameTime * 30.0;			// 유도탄 가속
	*/
}






void CX2DamageEffect::CEffect::SetPos( const D3DXVECTOR3& pos )
{
	m_Pos = pos;

	if( NULL != GetMainEffect() )
	{
		GetMainEffect()->SetPos( pos );
	}


	if( NULL != g_pX2Game &&
		NULL != g_pX2Game->GetMajorParticle() )
	{

		BOOST_TEST_FOREACH( ParticleData*, pParticleData, m_PassiveParticleDataList )
		{
			if( INVALID_PARTICLE_SEQUENCE_HANDLE != pParticleData->m_hSeq )
			{
				CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pX2Game->GetMajorParticle()->GetInstanceSequence( pParticleData->m_hSeq );
				if( NULL != pSeq && pParticleData->posType == CX2DamageEffect::DEPT_POS)
				{
					pSeq->SetPosition( pos );
				}
			}
		}
	}
}

//{{ kimhc // 2009-10-20 // DamageEffect.lua에 있는 테이블 로드(독구름)
#ifdef DAMAGE_EFFECT_WORK
void CX2DamageEffect::CEffect::LoadDieDamageEffect()
{
	KTDXPROFILE();

	int index = 0;
	while( m_pLuaManager->BeginTable( "DIE_DAMAGE_EFFECT", index ) == true )
	{

		DamageEffectData* pData = new DamageEffectData;

		PLUA_GET_VALUE_ENUM(	m_pLuaManager, "POSITION_TYPE",			pData->posType,				CX2DamageEffect::POSITION_TYPE, DEPT_BONE );
		PLUA_GET_VALUE(			m_pLuaManager, "DAMAGE_EFFECT_NAME",	pData->damageEffectName,	L""		);
		PLUA_GET_VALUE(			m_pLuaManager, "BONE_NAME",				pData->boneName,			L""		);
		PLUA_GET_VALUE(			m_pLuaManager, "APPLY_ROTATE",			pData->bApplyRotate,		true	);

#ifdef SPECIAL_USE_ITEM
		PLUA_GET_VALUE(			m_pLuaManager, "APPLY_ROTATED_OFFSET",	pData->bApplyRotatedOffset,	false	);
#endif SPECIAL_USE_ITEM

#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE //김창한
		PLUA_GET_VALUE(			m_pLuaManager, "ONLY_APPLY_ROTATE",		pData->bOnlyApplyRotateOffset,		false	);
#endif //SERV_ELESIS_SECOND_CLASS_CHANGE

		PLUA_GET_VALUE(			m_pLuaManager, "ROTATE_X",				pData->vRotateAngleDegree.x,	0.0f	);
		PLUA_GET_VALUE(			m_pLuaManager, "ROTATE_Y",				pData->vRotateAngleDegree.y,	0.0f	);
		PLUA_GET_VALUE(			m_pLuaManager, "ROTATE_Z",				pData->vRotateAngleDegree.z,	0.0f	);

		PLUA_GET_VALUE(			m_pLuaManager, "OFFSET_X",				pData->vOffset.x,	0.0f	);
		PLUA_GET_VALUE(			m_pLuaManager, "OFFSET_Y",				pData->vOffset.y,	0.0f	);        
		PLUA_GET_VALUE(			m_pLuaManager, "OFFSET_Z",				pData->vOffset.z,	0.0f	);

#ifdef SERV_RENA_NIGHT_WATCHER
		PLUA_GET_VALUE(			m_pLuaManager, "LAND_CRASH_ONLY",		pData->bLandCrashOnly,	false	);
		PLUA_GET_VALUE(			m_pLuaManager, "LIFE_TIME_ONLY",		pData->bLifeTimeOnly,	false	);
#endif SERV_RENA_NIGHT_WATCHER

#ifdef SERV_RAVEN_VETERAN_COMMANDER
		PLUA_GET_VALUE(			m_pLuaManager, "SET_GROUP_ID",			pData->iGroupID,		-1	);		/// 그룹 데미지 설정 추가
#endif SERV_RAVEN_VETERAN_COMMANDER

		/// 버프팩터를 상속 받을 것인가?
		PLUA_GET_VALUE(			m_pLuaManager, "INHERIT_BUFF_FACTOR",	pData->bInheritBuffFactor,	false );

		m_DamageEffectDataInLua.m_vecDieDamageEffect.push_back( pData );

		index++;

		m_pLuaManager->EndTable();
	}
}

void CX2DamageEffect::CEffect::LoadStartParticle()
{
	KTDXPROFILE();

	int index = 0;
	while( m_pLuaManager->BeginTable( "START_PARTICLE", index ) == true )
	{
		ParticleData* pData = LoadParticleTable();

#ifdef X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
		if( pData != NULL )
		{
			if( pData->m_iShowLevel >= g_pMain->GetGameOption().GetOptionList().m_eEffect )
				m_DamageEffectDataInLua.m_vecStartParticle.push_back( pData );
		}
#else//X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
		if( pData != NULL )
			m_DamageEffectDataInLua.m_vecStartParticle.push_back( pData );
#endif//X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION

		index++;

		m_pLuaManager->EndTable();
	}
}

void CX2DamageEffect::CEffect::LoadPassiveParticle()
{
	KTDXPROFILE();

	int index = 0;
	while( m_pLuaManager->BeginTable( "PASSIVE_PARTICLE", index ) == true )
	{
		ParticleData* pData = LoadParticleTable();

#ifdef X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
		if( pData != NULL )
		{
			if( pData->m_iShowLevel >= g_pMain->GetGameOption().GetOptionList().m_eEffect )
				m_DamageEffectDataInLua.m_vecPassiveParticle.push_back( pData );
		}
#else//X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
		if( pData != NULL )
			m_DamageEffectDataInLua.m_vecPassiveParticle.push_back( pData );
#endif//X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION

		index++;

		m_pLuaManager->EndTable();
	}
}

void CX2DamageEffect::CEffect::LoadTimeParticle()
{
	KTDXPROFILE();

	int index = 0;
	while( m_pLuaManager->BeginTable( "TIME_PARTICLE", index ) == true )
	{
		ParticleData* pData = LoadParticleTable();

#ifdef X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
		if( pData != NULL )
		{
			if( pData->m_iShowLevel >= g_pMain->GetGameOption().GetOptionList().m_eEffect )
				m_DamageEffectDataInLua.m_vecTimeParticle.push_back( pData );
		}
#else//X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
		if( pData != NULL )
			m_DamageEffectDataInLua.m_vecTimeParticle.push_back( pData );
#endif//X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION

		index++;

		m_pLuaManager->EndTable();
	}
}

void CX2DamageEffect::CEffect::LoadCameraData()
{
	KTDXPROFILE();

	int index = 0;
	while( m_pLuaManager->BeginTable( "CAMERA", index ) == true )
	{
		CameraData data;
		PLUA_GET_VALUE_ENUM(	m_pLuaManager, "UNIT_SELECT",		data.unitSelect,	UNIT_SELECT,	DEUS_ALL );
		PLUA_GET_VALUE_ENUM(	m_pLuaManager, "CRASH_TYPE",		data.crashType,		CKTDGCamera::SHAKE_TYPE,		CKTDGCamera::DECT_UP_DOWN );
		PLUA_GET_VALUE(			m_pLuaManager, "SPEED",				data.fSpeed,		0 );
		PLUA_GET_VALUE(			m_pLuaManager, "ACCEL",				data.fAccel,		0 );
		PLUA_GET_VALUE(			m_pLuaManager, "GAP",				data.fGap,			0 );
		PLUA_GET_VALUE(			m_pLuaManager, "TIME",				data.fTime,			0 );
		PLUA_GET_VALUE(			m_pLuaManager, "EVENT_TIME",		data.fEventTime,	0 );				

		m_DamageEffectDataInLua.m_vecCameraData.push_back( data );

		index++;

		m_pLuaManager->EndTable();
	}
}

void CX2DamageEffect::CEffect::LoadScreenData()
{
	KTDXPROFILE();

	int index = 0;
	while( m_pLuaManager->BeginTable( "SCREEN", index ) == true )
	{
		ScreenData data;
		PLUA_GET_VALUE_ENUM(	m_pLuaManager, "UNIT_SELECT",		data.unitSelect,	UNIT_SELECT,	DEUS_ALL );
		PLUA_GET_VALUE(			m_pLuaManager, "EVENT_TIME",		data.fEventTime,	0		);
		PLUA_GET_VALUE(			m_pLuaManager, "COUNT",				data.clearCount,	1		);
		PLUA_GET_VALUE(			m_pLuaManager, "COLOR_A",			data.color.a,		1.0f	);
		PLUA_GET_VALUE(			m_pLuaManager, "COLOR_R",			data.color.r,		1.0f	);
		PLUA_GET_VALUE(			m_pLuaManager, "COLOR_G",			data.color.g,		1.0f	);
		PLUA_GET_VALUE(			m_pLuaManager, "COLOR_B",			data.color.b,		1.0f	);

		m_DamageEffectDataInLua.m_vecScreenData.push_back( data );

		index++;

		m_pLuaManager->EndTable();
	}
}

void CX2DamageEffect::CEffect::LoadSoundData()
{
	KTDXPROFILE();

	int index = 0;
	while( m_pLuaManager->BeginTable( "SOUND", index ) == true )
	{
		SoundData data;
		PLUA_GET_VALUE(			m_pLuaManager, "EVENT_TIME",		data.fEventTime,		0		);
		PLUA_GET_VALUE(			m_pLuaManager, "FILE_NAME",		data.fileName,			L""		);
		PLUA_GET_VALUE(			m_pLuaManager, "RATE",				data.iRate,				100		);
		PLUA_GET_VALUE(			m_pLuaManager, "USE_GLOBAL_TIME",	data.bUseGlobalTime,	false	);

		PLUA_GET_VALUE(			m_pLuaManager, "LOOP",	data.bLoop,	false	);	// 동작안할꺼임..... 왜냐.. 플레이하는곳 note확인해보삼
		
		m_DamageEffectDataInLua.m_vecSoundData.push_back( data );
		index++;
		m_pLuaManager->EndTable();
	}
}

void CX2DamageEffect::CEffect::LoadDamageEffect()
{
	KTDXPROFILE();

	int index = 0;
	while( m_pLuaManager->BeginTable( "CREATE_DAMAGE_EFFECT", index ) == true )
	{
		DamageEffectData* pData = new DamageEffectData;
		PLUA_GET_VALUE(			m_pLuaManager, "EVENT_TIME",			pData->fEventTime,	-1.0f		);
		PLUA_GET_VALUE_ENUM(	m_pLuaManager, "POSITION_TYPE",		pData->posType,			CX2DamageEffect::POSITION_TYPE, DEPT_BONE );
		PLUA_GET_VALUE(			m_pLuaManager, "DAMAGE_EFFECT_NAME",	pData->damageEffectName,	L""		);
		PLUA_GET_VALUE(			m_pLuaManager, "BONE_NAME",			pData->boneName,		L""		);
		PLUA_GET_VALUE(			m_pLuaManager, "APPLY_ROTATE",			pData->bApplyRotate,	true	);
		PLUA_GET_VALUE(			m_pLuaManager, "APPLY_ROTATE_ORIGIN",	pData->bApplyRotateOrigin,	false	);

		//{{ oasis907 : 김상윤 [2010.2.10] //  
		PLUA_GET_VALUE(			m_pLuaManager, "APPLY_ROTATED_OFFSET",	pData->bApplyRotatedOffset,	false	);
		//}}

		PLUA_GET_VALUE(			m_pLuaManager, "ROTATE_X",				pData->vRotateAngleDegree.x,	0.0f	);
		PLUA_GET_VALUE(			m_pLuaManager, "ROTATE_Y",				pData->vRotateAngleDegree.y,	0.0f	);
		PLUA_GET_VALUE(			m_pLuaManager, "ROTATE_Z",				pData->vRotateAngleDegree.z,	0.0f	);

		PLUA_GET_VALUE(			m_pLuaManager, "OFFSET_X",				pData->vOffset.x,	0.0f	);
		PLUA_GET_VALUE(			m_pLuaManager, "OFFSET_Y",				pData->vOffset.y,	0.0f	);        
		PLUA_GET_VALUE(			m_pLuaManager, "OFFSET_Z",				pData->vOffset.z,	0.0f	);

#ifdef SERV_ADD_CHUNG_SHELLING_GUARDIAN
		PLUA_GET_VALUE(			m_pLuaManager, "APPLY_MATRIX",			pData->bApplyMatrix,	false	);
		PLUA_GET_VALUE(			m_pLuaManager, "AUTO_DIE",				pData->bAutoDie,		false	);		
#endif

#ifdef SERV_RAVEN_VETERAN_COMMANDER
		PLUA_GET_VALUE(			m_pLuaManager, "SET_GROUP_ID",			pData->iGroupID,		-1 );		/// 그룹 데미지 설정 추가
#endif SERV_RAVEN_VETERAN_COMMANDER

		/// 버프팩터를 상속 받을 것인가?
		PLUA_GET_VALUE(			m_pLuaManager, "INHERIT_BUFF_FACTOR",	pData->bInheritBuffFactor,	false );

#ifdef LOOP_CREATE_DAMAGE_EFFECT // 김태환
		PLUA_GET_VALUE(			m_pLuaManager, "LOOP_INTERVAL",			pData->fLoopInterval,	0.f		);	/// 반복 생성 간격
		PLUA_GET_VALUE(			m_pLuaManager, "LOOP_END_COUNT",		pData->uiLoopEndCount,	1		);	/// 반복 생성 종료 횟수
		PLUA_GET_VALUE(			m_pLuaManager, "LOOP_END_TIME",			pData->fLoopEndTime,	99999.f );	/// 반복 생성 종료 시간
#endif //LOOP_CREATE_DAMAGE_EFFECT

		
		m_DamageEffectDataInLua.m_vecCreateDamageEffect.push_back( pData );
		index++;
		m_pLuaManager->EndTable();
	}
}

void CX2DamageEffect::CEffect::LoadHitParticle()
{
	KTDXPROFILE();

	int index = 0;
	while( m_pLuaManager->BeginTable( "HIT_PARTICLE", index ) == true )
	{
		ParticleData* pData = LoadParticleTable();
		
#ifdef X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
		if( pData != NULL )
		{
			if( pData->m_iShowLevel >= g_pMain->GetGameOption().GetOptionList().m_eEffect )
				m_DamageEffectDataInLua.m_vecHitParticle.push_back( pData );
		}
#else//X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
		if ( pData != NULL )
			m_DamageEffectDataInLua.m_vecHitParticle.push_back( pData );
#endif//X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
		
		index++;

		m_pLuaManager->EndTable();
	}
}

void CX2DamageEffect::CEffect::LoadHitDamageEffect()
{
	KTDXPROFILE();

	int index = 0;
	while( m_pLuaManager->BeginTable( "HIT_DAMAGE_EFFECT", index ) == true )
	{

		DamageEffectData* pData = new DamageEffectData;

		PLUA_GET_VALUE_ENUM(	m_pLuaManager, "POSITION_TYPE",			pData->posType,						CX2DamageEffect::POSITION_TYPE, DEPT_BONE	);
		PLUA_GET_VALUE(			m_pLuaManager, "DAMAGE_EFFECT_NAME",	pData->damageEffectName,			L""		);
		PLUA_GET_VALUE(			m_pLuaManager, "BONE_NAME",				pData->boneName,					L""		);
		PLUA_GET_VALUE(			m_pLuaManager, "APPLY_ROTATE",			pData->bApplyRotate,				true	);
		PLUA_GET_VALUE(			m_pLuaManager, "APPLY_ROTATE_ORIGIN",	pData->bApplyRotateOrigin,			false	);

#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE //김창한
		PLUA_GET_VALUE(			m_pLuaManager, "ONLY_APPLY_ROTATE",		pData->bOnlyApplyRotateOffset,		false	);
#endif //SERV_ELESIS_SECOND_CLASS_CHANGE

#ifdef SPECIAL_USE_ITEM
		PLUA_GET_VALUE(			m_pLuaManager, "APPLY_ROTATED_OFFSET",	pData->bApplyRotatedOffset,			false	);
#endif SPECIAL_USE_ITEM


		PLUA_GET_VALUE(			m_pLuaManager, "ROTATE_X",				pData->vRotateAngleDegree.x,	0.0f	);
		PLUA_GET_VALUE(			m_pLuaManager, "ROTATE_Y",				pData->vRotateAngleDegree.y,	0.0f	);
		PLUA_GET_VALUE(			m_pLuaManager, "ROTATE_Z",				pData->vRotateAngleDegree.z,	0.0f	);

		PLUA_GET_VALUE(			m_pLuaManager, "OFFSET_X",				pData->vOffset.x,	0.0f	);
		PLUA_GET_VALUE(			m_pLuaManager, "OFFSET_Y",				pData->vOffset.y,	0.0f	);        
		PLUA_GET_VALUE(			m_pLuaManager, "OFFSET_Z",				pData->vOffset.z,	0.0f	);

		PLUA_GET_VALUE(			m_pLuaManager, "MAIN_EFFECT_SCALE_X",	pData->vMainEffectScale.x,	0.0f	);
		PLUA_GET_VALUE(			m_pLuaManager, "MAIN_EFFECT_SCALE_Y",	pData->vMainEffectScale.y,	0.0f	);        
		PLUA_GET_VALUE(			m_pLuaManager, "MAIN_EFFECT_SCALE_Z",	pData->vMainEffectScale.z,	0.0f	);

		PLUA_GET_VALUE(			m_pLuaManager, "PASSIVE_PARTICLE_X",	pData->vPassiveParticleScale.x,	0.0f	);
		PLUA_GET_VALUE(			m_pLuaManager, "PASSIVE_PARTICLE_Y",	pData->vPassiveParticleScale.y,	0.0f	);        
		PLUA_GET_VALUE(			m_pLuaManager, "PASSIVE_PARTICLE_Z",	pData->vPassiveParticleScale.z,	0.0f	);

		/// 버프팩터를 상속 받을 것인가?
		PLUA_GET_VALUE(			m_pLuaManager, "INHERIT_BUFF_FACTOR",	pData->bInheritBuffFactor,	false );

		m_DamageEffectDataInLua.m_vecHitDamageEffect.push_back( pData );

		index++;

		m_pLuaManager->EndTable();
	}
}

void CX2DamageEffect::CEffect::SetDamageDataNext( CX2DamageManager::DamageData* pDamageData )
{
	if ( m_DamageEffectDataInLua.m_pDamageDataNext != NULL )
	{
		// 기존의 DamageData의 값중 DamageDataNext에서도 유지해야 할것들 미리 대입
		m_DamageEffectDataInLua.m_pDamageDataNext->hitUnitList = pDamageData->hitUnitList;
		m_DamageEffectDataInLua.m_pDamageDataNext->m_bApplyExtraDamage = pDamageData->m_bApplyExtraDamage;

#ifdef PVP_SEASON2
		m_DamageEffectDataInLua.m_pDamageDataNext->m_fPvpRate = pDamageData->m_fPvpRate;
#endif

#ifdef EVE_ELECTRA
		m_DamageEffectDataInLua.m_pDamageDataNext->m_iLaserGroupID = pDamageData->m_iLaserGroupID;
		if( m_iLaserGroupID > 0 )
		{
			m_DamageEffectDataInLua.m_pDamageDataNext->damage = pDamageData->damage;
			m_DamageEffectDataInLua.m_pDamageDataNext->fHitAddMP = pDamageData->fHitAddMP;
			m_DamageEffectDataInLua.m_pDamageDataNext->m_fRateModifier = pDamageData->m_fRateModifier;
		}
#endif EVE_ELECTRA

#ifdef ADD_RENA_SYSTEM //김창한
		//스킬, 관련 데이터 값, 첫타인지 체크 하는 값을 상속함
		m_DamageEffectDataInLua.m_pDamageDataNext->m_eFirstAttack = pDamageData->m_eFirstAttack;
		m_DamageEffectDataInLua.m_pDamageDataNext->m_RelateSkillData = pDamageData->m_RelateSkillData;
#endif //ADD_RENA_SYSTEM

		// 다시 DamageData에 넣어줌
		*pDamageData =  *(m_DamageEffectDataInLua.m_pDamageDataNext);
	}	
}

#endif DAMAGE_EFFECT_WORK
//}} kimhc // 2009-10-20 // DamageEffect.lua에 있는 테이블 로드(독구름)

#ifdef EVE_ELECTRA
void CX2DamageEffect::CEffect::SetLaserGroupID( int _iLaserGroupID )
{
	m_iLaserGroupID					= _iLaserGroupID;
	m_DamageData.m_iLaserGroupID	= _iLaserGroupID;
}
#endif EVE_ELECTRA


void CX2DamageEffect::ParticleData::DestroyParticleSequence()
{
	if( m_hSeq != INVALID_PARTICLE_SEQUENCE_HANDLE )
	{
		CKTDGParticleSystem::CParticleEventSequence* pSeq = ( g_pX2Game != NULL ) ? g_pX2Game->GetMajorParticle()->GetInstanceSequence( m_hSeq ) : NULL;
		if( NULL != pSeq )
		{
			if( bForceDie == true )
			{
				g_pX2Game->GetMajorParticle()->DestroyInstanceHandle( m_hSeq );
			}
			else
			{
				pSeq->SetAutoDie();
			}
		}

		m_hSeq = INVALID_PARTICLE_SEQUENCE_HANDLE;
	}
}

#ifdef FIX_AFTER_IMAGE
void CX2DamageEffect::CEffect::LoadAfterImage()
{
	if( m_pLuaManager->BeginTable( "AFTER_IMAGE" ) == true )
	{
		PLUA_GET_VALUE( m_pLuaManager, "AFTER_IMAGE_STARTTIME",	m_afterImageTime.x,	-1.0f );
		PLUA_GET_VALUE( m_pLuaManager, "AFTER_IMAGE_ENDTIME",	m_afterImageTime.y,	-1.0f );
		X2_LAYER eAfterImageLayer = XL_EFFECT_0;
		PLUA_GET_VALUE_ENUM( m_pLuaManager, "AFTER_IMAGE_LAYER", eAfterImageLayer, X2_LAYER, XL_EFFECT_0 );
		D3DXCOLOR coAfterImage = 0xffcccccc;
		PLUA_GET_VALUE( m_pLuaManager, "AFTER_IMAGE_COLOR_R", coAfterImage.r, 0.797f );
		PLUA_GET_VALUE( m_pLuaManager, "AFTER_IMAGE_COLOR_G", coAfterImage.g, 0.797f );
		PLUA_GET_VALUE( m_pLuaManager, "AFTER_IMAGE_COLOR_B", coAfterImage.b, 0.797f );
		PLUA_GET_VALUE( m_pLuaManager, "AFTER_IMAGE_COLOR_A", coAfterImage.a, 1.f );

		if ( m_pAfterImage == NULL )
		{
			if( NULL != GetMainEffect() && NULL != GetMainEffect()->GetXSkinAnim() )
			{
				m_pAfterImage = CKTDGXSkinAfterImage::CreateSkinAfterImage( GetMainEffect()->GetXSkinAnim(), 10, eAfterImageLayer );

				if ( m_pAfterImage != NULL )
				{
					m_pAfterImage->SetAfterImageColor( coAfterImage );
					m_pAfterImage->AddMesh( GetMainEffect()->GetXSkinMesh() );
				}
			}
		}

		m_pLuaManager->EndTable();
	}
	else
	{
		m_afterImageTime.x	= -1.0f;
		m_afterImageTime.y	= -1.0f;
	}
}
#endif

#ifdef SERV_RENA_NIGHT_WATCHER
void CX2DamageEffect::CEffect::AddDieEffectDamage( CX2DamageEffect::CEffect* pCEffect )
{
	if( 0.f < GetAddDieEffectDamage() )		/// 추가 데미지 값이 설정되어 있을 때, 대입
	{
		if( 0.f < pCEffect->GetDamageData().damage.fPhysic )
			pCEffect->GetDamageData().damage.fPhysic +=  GetAddDieEffectDamage();
		if( 0.f < pCEffect->GetDamageData().damage.fMagic )
			pCEffect->GetDamageData().damage.fMagic +=  GetAddDieEffectDamage();
	}
}
#endif SERV_RENA_NIGHT_WATCHER

#ifdef SERV_RENA_NIGHT_WATCHER
CX2DamageEffect::CEffect* CX2DamageEffect::GetInstanceByIndex( int iIndex )
{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

    KInstanceHandleList::iterator iterEnd = m_coInstanceHandleList.end(LIST_LIVE);
    for( KInstanceHandleIterator iter = m_coInstanceHandleList.begin( LIST_LIVE );
        iter != iterEnd;
        ++iter )
    {
        CX2DamageEffect::CEffect* pEffect = iter->m_pInstance;
        if ( pEffect != NULL && 
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX
            pEffect->GetLive() == true &&
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX            
            pEffect->GetIndex() == iIndex )
            return pEffect;
    }

#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

	vector<CEffect*>::iterator vIt = m_InstanceList.begin();

	while( vIt != m_InstanceList.end() )			//땅에 박힌 화살의 Index를 지닌 DamageEffedt를 현재 생성된 DamageEffect Vector에서 검색
	{
		CX2DamageEffect::CEffect* pEffect = *vIt;

		if ( iIndex == pEffect->GetIndex() )
		{
			return pEffect;
		}
		else
		{
			++vIt;
		}
	}

#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

	return NULL;
}
#endif SERV_RENA_NIGHT_WATCHER

#ifdef BOOST_SINGLETON_POOL_TEST
	typedef boost::singleton_pool<boost::pool_allocator_tag, sizeof(CX2DamageEffect::ParticleData)> ParticleDataPool;
	typedef boost::singleton_pool<boost::pool_allocator_tag, sizeof(CX2DamageEffect::DamageEffectData)> DamageEffectDataPool;
	typedef boost::singleton_pool<boost::pool_allocator_tag, sizeof(CX2DamageEffect::CEffect)> CEffectPool;



	void* CX2DamageEffect::ParticleData::operator new(size_t s)
	{
		return ParticleDataPool::malloc();
	}

	void CX2DamageEffect::ParticleData::operator delete(void *p)
	{
		ParticleDataPool::free(p);
	}

	void* CX2DamageEffect::DamageEffectData::operator new(size_t s)
	{
		return DamageEffectDataPool::malloc();
	}

	void CX2DamageEffect::DamageEffectData::operator delete(void *p)
	{
		DamageEffectDataPool::free(p);
	}


	void* CX2DamageEffect::CEffect::operator new(size_t s)
	{
		return CEffectPool::malloc();
	}

	void CX2DamageEffect::CEffect::operator delete(void *p)
	{
		CEffectPool::free(p);
	}
#endif BOOST_SINGLETON_POOL_TEST

#ifdef EFFECT_TOOL
bool CX2DamageEffect::IsValidDamageEffectByName( const WCHAR* pName )
{
	if( true == m_LuaManager.BeginTable( pName ) )
	{
		m_LuaManager.EndTable();
		return true;
	}

	return false;
}
wstring CX2DamageEffect::GetMainMeshNameByDamageEffectName( const WCHAR* pName )
{
	wstring mainEffName = L"";

	if( true == m_LuaManager.BeginTable( pName ) )
	{
		LUA_GET_VALUE( m_LuaManager, "MAIN_EFFECT_NAME", mainEffName, L"" );
		m_LuaManager.EndTable();
	}

	return mainEffName;
}
// 데미지 이펙트 내부에서 출력하는 파티클 리스트 얻기
void CX2DamageEffect::GetParticleListByEffectName( IN const WCHAR* pName_, OUT vector<wstring>& vecParticleList_ )
{
	vecParticleList_.clear();

	wstring wstrParticleName = L"";
	if( true == m_LuaManager.BeginTable( pName_ ) )
	{
		// START_PARTICLE
		int index = 0;
		while( m_LuaManager.BeginTable( "START_PARTICLE", index ) == true )
		{
			LUA_GET_VALUE( m_LuaManager, "PARTICLE_NAME",	wstrParticleName,	L""		);

			++index;
			m_LuaManager.EndTable();
			vecParticleList_.push_back( wstrParticleName );
		}

		// PASSIVE_PARTICLE
		index = 0;
		while( m_LuaManager.BeginTable( "PASSIVE_PARTICLE", index ) == true )
		{
			LUA_GET_VALUE( m_LuaManager, "PARTICLE_NAME",	wstrParticleName,	L""		);

			++index;
			m_LuaManager.EndTable();
			vecParticleList_.push_back( wstrParticleName );
		}

		// TIME_PARTICLE
		index = 0;
		while( m_LuaManager.BeginTable( "TIME_PARTICLE", index ) == true )
		{
			LUA_GET_VALUE( m_LuaManager, "PARTICLE_NAME",	wstrParticleName,	L""		);

			++index;
			m_LuaManager.EndTable();
			vecParticleList_.push_back( wstrParticleName );
		}
		m_LuaManager.EndTable();
	}
}
/** @function : GetParticleListByEffectName
	@brief : 데미지 이펙트에서 사용하는 파티클 얻기.
*/
void CX2DamageEffect::GetParticleListByEffectName( IN const WCHAR* pName_, OUT vector<ParticleData*>& vecParticleList_ )
{
 	BOOST_FOREACH( ParticleData* pParticleData, vecParticleList_)
 	{
 		SAFE_DELETE(pParticleData);
 	}
	vecParticleList_.clear();

	wstring wstrParticleName = L"";
	if( true == m_LuaManager.BeginTable( pName_ ) )
	{
		int index = 0;
		while( m_LuaManager.BeginTable( "START_PARTICLE", index ) == true )
		{
			ParticleData* pData = LoadParticleTable();
			if( pData != NULL )
				vecParticleList_.push_back( pData );
			index++;
			m_LuaManager.EndTable();
		}

		index = 0;
		while( m_LuaManager.BeginTable( "PASSIVE_PARTICLE", index ) == true )
		{
			ParticleData* pData = LoadParticleTable();	
			if( pData != NULL )
				vecParticleList_.push_back( pData );
			index++;
			m_LuaManager.EndTable();
		}

		index = 0;
		while( m_LuaManager.BeginTable( "TIME_PARTICLE", index ) == true )
		{
			ParticleData* pData = LoadParticleTable();	
			if( pData != NULL )
				vecParticleList_.push_back( pData );
			index++;
			m_LuaManager.EndTable();
		}

		m_LuaManager.EndTable();
	}
}
// 데미지 이펙트 내부에서 생성하는 데미지 이팩트 리스트 얻기
void CX2DamageEffect::GetDamageEffectListByEffectName( IN const WCHAR* pName_,  OUT vector<wstring>& vecEffectList_ )
{
	vecEffectList_.clear();

	wstring wstrEffectName = L"";
	if( true == m_LuaManager.BeginTable( pName_ ) )
	{
		// CREATE_DAMAGE_EFFECT
		int index = 0;
		while( m_LuaManager.BeginTable( "CREATE_DAMAGE_EFFECT", index ) == true )
		{
			LUA_GET_VALUE( m_LuaManager, "DAMAGE_EFFECT_NAME",	wstrEffectName,	L""		);

			++index;
			m_LuaManager.EndTable();
			vecEffectList_.push_back( wstrEffectName );
		}

		// DIE_DAMAGE_EFFECT0
		index = 0;
		while( m_LuaManager.BeginTable( "DIE_DAMAGE_EFFECT", index ) == true )
		{
			LUA_GET_VALUE( m_LuaManager, "DAMAGE_EFFECT_NAME",	wstrEffectName,	L""		);

			++index;
			m_LuaManager.EndTable();
			vecEffectList_.push_back( wstrEffectName );
		}

		m_LuaManager.EndTable();
	}
}
CX2DamageEffect::ParticleData* CX2DamageEffect::LoadParticleTable()
{
	ParticleData* pData = new ParticleData;

	LUA_GET_VALUE_ENUM(	m_LuaManager, "POSITION_TYPE",	pData->posType,			CX2DamageEffect::POSITION_TYPE, DEPT_BONE );
	LUA_GET_VALUE(			m_LuaManager, "PARTICLE_NAME",	pData->particleName,	L""		);
	LUA_GET_VALUE(			m_LuaManager, "APPLY_ROTATE",	pData->bApplyRotate,	true	);
	LUA_GET_VALUE(			m_LuaManager, "FORCE_DIE",		pData->bForceDie,		false	);
	LUA_GET_VALUE(			m_LuaManager, "FORCE_DIE_HIT",	pData->bForceDieHit,	false	);
	LUA_GET_VALUE(			m_LuaManager, "TRIGGER_COUNT",	pData->triggerCount,	-1		);
	LUA_GET_VALUE(			m_LuaManager, "TRIGGER_TIME",	pData->triggerTime,		-1.0f	);
	LUA_GET_VALUE(			m_LuaManager, "BONE_NAME",		pData->boneName,		L""		);
	LUA_GET_VALUE(			m_LuaManager, "EVENT_TIME",	pData->fEventTime,		-1.0f	);
	LUA_GET_VALUE(			m_LuaManager, "OFFSET_X",		pData->fOffsetX,		0.0f	);  // added by wonpok
	LUA_GET_VALUE(			m_LuaManager, "OFFSET_Y",		pData->fOffsetY,		0.0f	);
	LUA_GET_VALUE(			m_LuaManager, "OFFSET_Z",		pData->fOffsetZ,		0.0f	);
	LUA_GET_VALUE(         m_LuaManager, "APPLY_ROTATED_OFFSET", pData->bApplyRotatedOffset  , false ); // added by lucidash : 2010.11.24

	LUA_GET_VALUE(			m_LuaManager, "ROTATE_X",		pData->vRotateAngleDegree.x,	0.0f	);
	LUA_GET_VALUE(			m_LuaManager, "ROTATE_Y",		pData->vRotateAngleDegree.y,	0.0f	);
	LUA_GET_VALUE(			m_LuaManager, "ROTATE_Z",		pData->vRotateAngleDegree.z,	0.0f	);
	LUA_GET_VALUE(			m_LuaManager, "USE_GLOBAL_TIME",	pData->bUseGlobalTime,	false	);
	LUA_GET_VALUE(			m_LuaManager, "MOVE_ANGLE_X_PER_FRAME", pData->vMoveAnglePerFrame.x, 0.0f );
	LUA_GET_VALUE(			m_LuaManager, "MOVE_ANGLE_Y_PER_FRAME", pData->vMoveAnglePerFrame.y, 0.0f );
	LUA_GET_VALUE(			m_LuaManager, "MOVE_ANGLE_Z_PER_FRAME", pData->vMoveAnglePerFrame.z, 0.0f );
	LUA_GET_VALUE(			m_LuaManager, "SCALE_X",		pData->vScale.x,		1.0f	);
	LUA_GET_VALUE(			m_LuaManager, "SCALE_Y",		pData->vScale.y,		1.0f	);
	LUA_GET_VALUE(			m_LuaManager, "SCALE_Z",		pData->vScale.z,		1.0f	);

	LUA_GET_VALUE(			m_LuaManager, "TRACE",			pData->bTrace,			true	);
	LUA_GET_VALUE(			m_LuaManager, "TRACE_MORE",	pData->bTraceMore,		false	);
	LUA_GET_VALUE(			m_LuaManager, "TRACE_DEFENDER_UNIT",	pData->bTraceDefenderUnit,		false	);
	LUA_GET_VALUE(			m_LuaManager, "SCALE_BY_MAINEFFECT_SCALE_Y",		pData->bScaleByMainEffectScaleY,		false	);
	LUA_GET_VALUE(			m_LuaManager, "SHOW_INNER_LANDHEIGHT",		pData->fShowLandHeight,		99999.f	);

	return pData;

}
#endif //EFFECT_TOOL



#ifdef FIELD_BOSS_RAID // 지정한 위치에서 가까운 적 유도 시키기
void CX2DamageEffect::CEffect::SetLockOnNearstTarget( IN const D3DXVECTOR3& vMyPos_, int iTeam_ , int randomOffset/* = 0 */)
{
	bool bUserUnit;
	UidType uid = ( g_pX2Game != NULL ) ? g_pX2Game->GetLockOnNearstTarget( static_cast<CX2Room::TEAM_NUM>(iTeam_), vMyPos_, bUserUnit ) : 0;

	if( bUserUnit == true )
		SetLockOnUnitUID( uid );
	else
		SetLockOnNPCUID( (int)uid );

	SetLockOnRandomSeed( randomOffset );
}
#endif // FIELD_BOSS_RAID

#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
/** @function	: SetLinkMainEffectByPos
	@brief		: 해당 데미지 이펙트의 Main Mesh를 두 위치 사이에 연결 시켜 주는 함수
	@param		: 이펙트 위치, 상대 위치, 길이 배율 ( Main Effect 길이에 따른 스케일 설정 배율 ), 두 이펙트 간 거리
				  ( 참고로, DamageEffect_APT_Pylon_Additional_Shock 의 길이 배율은 800.f 이다. )
	@return		: 연결 성공 여부
*/
const bool CX2DamageEffect::CEffect::SetLinkMainEffectByPos( IN const D3DXVECTOR3& vMyPos_, IN const D3DXVECTOR3& vTargetPos_, 
															 IN const float fDistance_ /*= 99999.f*/ )
{
	CKTDGXMeshPlayer::CXMeshInstance* pMeshInst = GetMainEffect();		/// 연결 데미지 이펙트의 Main Mesh

	if ( NULL == pMeshInst )
		return false;

	/// 두 이펙트 사이 거리 설정 ( 거리 인자값 않들어 왔다면, 직접 구해주자 )
	float fDistance = fDistance_;

	if ( true == IsSamef( fDistance_, 99999.f ) )
		fDistance = GetDistance( vMyPos_, vTargetPos_ );

	if ( 0.f >= fDistance )
		fDistance = 1.f;

	float fLinkEffectRate = GetLinkEffectRate();

	/// 길이 배율 ( Main Effect 길이에 따른 스케일 설정 배율 )
	/// 작을수록 길어 진다.
	/// ( 참고로, ADD_PYLON_DYNAMO_SPARK01.X 의 길이 배율은 800.f 이다. )
	/// 동적으로 값을 연산할 수 있는 방법이 없을까...
	if ( 0.f > fLinkEffectRate )
	{
		ASSERT( ! "LinkEffectRate is InValid!!!" );
		fLinkEffectRate = 800.f;
	}

	/// 두 파일런 간 데미지 이펙트 매시 연결 설정 ( 셰도우 링커 참고 )
	D3DXVECTOR3 vResultPos( vTargetPos_.x - vMyPos_.x, vTargetPos_.y - vMyPos_.y, vTargetPos_.z - vMyPos_.z );
	pMeshInst->SetPos( vMyPos_ );
	vResultPos = GetDirVecToDegree( vResultPos );
	pMeshInst->SetRotateDegree( vResultPos );
	pMeshInst->SetMoveAxisAngleDegree( vResultPos );
	pMeshInst->SetScale( fDistance / fLinkEffectRate, 1.f, 1.f );

	return true;
}
#endif //SERV_ADD_LUNATIC_PSYKER