#include "StdAfx.h"
#include ".\x2unitloader.h"



CX2UnitLoader::CX2UnitLoader(void)
{
    Initialize();
}

CX2UnitLoader::~CX2UnitLoader(void)
{
    Finalize();
}

void    CX2UnitLoader::Initialize()
{
    Finalize();

    //m_RenderCount = 0;
    m_fElapsedTime = 0.f;

	m_listLoadUnitData.clear();
    m_mapLoadUnitDataLoaded.clear();


    m_listNPCResource.clear();
	m_mapNPCUnitUIDResourceLoaded.clear();
	m_mapNPCUnitIDResourceLoaded.clear();

}//CX2UnitLoader::Initialize()


void    CX2UnitLoader::Finalize()
{
    ClearUnitLoadData();
    RemoveAllNPCResources();
}//CX2UnitLoader::Finalize()

void CX2UnitLoader::OnFrameMove( float fElapsedTime )
{
	KTDXPROFILE();

	m_fElapsedTime = fElapsedTime;

	//if( m_RenderCount + 2 < g_pKTDXApp->GetRenderCount() )
	//{
	//	m_RenderCount = g_pKTDXApp->GetRenderCount();

	while( m_listLoadUnitData.empty() == false )
    {
		LoadUserUnitData& loadUserUnitData = m_listLoadUnitData.front();

		while( loadUserUnitData.m_listKTDXDeviceData.empty() == false )
		{
			CKTDXDeviceData& deviceData = loadUserUnitData.m_listKTDXDeviceData.front();
			if ( deviceData.m_pDevice == NULL )
			{
				loadUserUnitData.m_listKTDXDeviceData.pop_front();
			}
			else
			{
				CKTDXDevice::EDeviceState eDeviceState = deviceData.m_pDevice->GetDeviceState();
				if ( eDeviceState == CKTDXDevice::DEVICE_STATE_LOADED 
					|| eDeviceState == CKTDXDevice::DEVICE_STATE_FAILED )
				{
					loadUserUnitData.m_listKTDXDeviceDataLoaded.splice( loadUserUnitData.m_listKTDXDeviceDataLoaded.end()
						, loadUserUnitData.m_listKTDXDeviceData, loadUserUnitData.m_listKTDXDeviceData.begin() );
				}
				else
				{
					break;
				}
			}//if.. else..
		}
        if ( loadUserUnitData.m_listKTDXDeviceData.empty() == true )
        {
			LoadUserUnitData& loadeUserUnitDataLoaded = m_mapLoadUnitDataLoaded[ loadUserUnitData.m_UnitUID ];
			loadeUserUnitDataLoaded.m_listKTDXDeviceData.swap( loadUserUnitData.m_listKTDXDeviceData );
			loadeUserUnitDataLoaded.m_listKTDXDeviceDataLoaded.swap( loadUserUnitData.m_listKTDXDeviceDataLoaded );
			m_listLoadUnitData.pop_front();
        }
		else
		{
			break;
		}
    }//if

    while( m_listNPCResource.empty() == false )
    {
        NPCResource& npcResource = m_listNPCResource.front();
        while( npcResource.m_listKTDXDeviceData.empty() == false )
        {
            CKTDXDeviceData& deviceData = npcResource.m_listKTDXDeviceData.front();
            if ( deviceData.m_pDevice == NULL )
            {
                npcResource.m_listKTDXDeviceData.pop_front();
            }
            else
            {
                CKTDXDevice::EDeviceState eDeviceState = deviceData.m_pDevice->GetDeviceState();
                if ( eDeviceState == CKTDXDevice::DEVICE_STATE_LOADED 
                    || eDeviceState == CKTDXDevice::DEVICE_STATE_FAILED )
                {
                    npcResource.m_listKTDXDeviceDataLoaded.splice( npcResource.m_listKTDXDeviceDataLoaded.end()
                        , npcResource.m_listKTDXDeviceData, npcResource.m_listKTDXDeviceData.begin() );
                }
				else
				{
					break;
				}
            }//if.. else..
        }//if
        if ( npcResource.m_listKTDXDeviceData.empty() == true )
        {
			if ( npcResource.m_uid != UINT(-1) )
			{
				NPCResource& npcResourceLoaded = m_mapNPCUnitUIDResourceLoaded[ npcResource.m_uid ];
				npcResourceLoaded.m_listKTDXDeviceData.swap( npcResource.m_listKTDXDeviceData );
				npcResourceLoaded.m_listKTDXDeviceDataLoaded.swap( npcResource.m_listKTDXDeviceDataLoaded );
			}
			else
			{
				NPCResource& npcResourceLoaded = m_mapNPCUnitIDResourceLoaded[ npcResource.m_id ];
				npcResourceLoaded.m_listKTDXDeviceData.swap( npcResource.m_listKTDXDeviceData );
				npcResourceLoaded.m_listKTDXDeviceDataLoaded.swap( npcResource.m_listKTDXDeviceDataLoaded );
			}
			m_listNPCResource.pop_front();
        }
		else
		{
			break;
		}
    }//if
}

void CX2UnitLoader::AddUnitLoadDataForGame( CX2Unit* pUnit
#ifdef	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD
						, bool bAddPetResource
#endif	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD
						)
{
    ASSERT( pUnit != NULL );
    if ( pUnit == NULL )
        return;




	UidType uidUnitUID = pUnit->GetUID();
	if ( m_mapLoadUnitDataLoaded.find( uidUnitUID ) != m_mapLoadUnitDataLoaded.end() )
		return;
    BOOST_FOREACH( const LoadUserUnitData& unitResource, m_listLoadUnitData )
    {
        if ( unitResource.m_UnitUID == uidUnitUID )
            return;
    }//BOOST_FOREACH()
	m_listLoadUnitData.resize( m_listLoadUnitData.size() + 1 );

	LoadUserUnitData& kLoadUserUnitData = m_listLoadUnitData.back();
	
   
    kLoadUserUnitData.m_UnitUID = uidUnitUID;

	vector<CX2EqipPtr> viewEqipList;
	g_pData->GetItemManager()->UpdateEqip( pUnit, CKTDGXSkinAnimPtr(), NULL, NULL, IN OUT viewEqipList, false );

    BOOST_TEST_FOREACH( CX2EqipPtr, pCX2Eqip, viewEqipList )
	{
        if ( pCX2Eqip == NULL )
            continue;
        pCX2Eqip->AppendToDeviceList( kLoadUserUnitData.m_listKTDXDeviceData );
	}

	viewEqipList.resize(0);

#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	const CX2UnitManager::UnitTypeTemplet* pUnitTypeTemplet 
		= g_pData->GetUnitManager()->GetUnitTypeLuaTemplet( pUnit->GetType() ).m_pUnitTypeTemplet;
#else	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	const CX2UnitManager::UnitTypeTemplet* pUnitTypeTemplet = g_pData->GetUnitManager()->GetUnitTypeTemplet( pUnit->GetType() );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD

	if ( pUnitTypeTemplet != NULL )
	{
		pUnitTypeTemplet->m_init.AppendToDeviceList( kLoadUserUnitData.m_listKTDXDeviceData, pUnit );
	}

#ifdef	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD
#ifdef SERV_PET_SYSTEM

	if( g_pData != NULL && g_pData->GetPetManager() != NULL && pUnit->GetPetInfo() != NULL )
	{
		const KPetInfo *pPetInfo = pUnit->GetPetInfo();
#ifdef SERV_PETID_DATA_TYPE_CHANGE //2013.07.02
		const CX2PetManager::PetTemplet* pPetTemplet = g_pData->GetPetManager()->GetPetTemplet((CX2PetManager::PET_UNIT_ID)pPetInfo->m_iPetID);
#else //SERV_PETID_DATA_TYPE_CHANGE
		const CX2PetManager::PetTemplet* pPetTemplet = g_pData->GetPetManager()->GetPetTemplet((CX2PetManager::PET_UNIT_ID)pPetInfo->m_cPetID);
#endif //SERV_PETID_DATA_TYPE_CHANGE
		if ( pPetTemplet && pPetInfo->m_cEvolutionStep >= 0 && pPetInfo->m_cEvolutionStep < (int) pPetTemplet->m_Evolution_Step_InitLuaTemplet.size() )
		{
			const CX2PetManager::PetInitTemplet* pPetInitTemplet 
				= pPetTemplet->m_Evolution_Step_InitLuaTemplet[ pPetInfo->m_cEvolutionStep ].m_pInitTemplet;
			if ( pPetInitTemplet != NULL )
			{
				pPetInitTemplet->m_init.AppendToDeviceList( kLoadUserUnitData.m_listKTDXDeviceData );
			}
		}
	}

#endif	SERV_PET_SYSTEM
#endif	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD

}

void    CX2UnitLoader::AddUnitLoadDataForSquareOrField( CX2Unit* pUnit, bool bTField )
{
    ASSERT( pUnit != NULL );
    if ( pUnit == NULL )
        return;

	UidType uidUnitUID = pUnit->GetUID();
	if ( m_mapLoadUnitDataLoaded.find( uidUnitUID ) != m_mapLoadUnitDataLoaded.end() )
		return;
    BOOST_FOREACH( const LoadUserUnitData& unitResource, m_listLoadUnitData )
    {
        if ( unitResource.m_UnitUID == uidUnitUID )
            return;
    }//BOOST_FOREACH()
	m_listLoadUnitData.resize( m_listLoadUnitData.size() + 1 );


	LoadUserUnitData& kLoadUserUnitData = m_listLoadUnitData.back();
	
   
    kLoadUserUnitData.m_UnitUID = uidUnitUID;

	CX2SquareUnit::AppendToDeviceList( kLoadUserUnitData.m_listKTDXDeviceData, pUnit, bTField, CX2UnitViewerUI::UVS_FIELD );

#ifdef  X2OPTIMIZE_VILLAGE_OR_SQUARE_PET_BACKGROUND_LOAD
#ifdef SERV_PET_SYSTEM

	if( g_pData != NULL && g_pData->GetPetManager() != NULL && pUnit->GetPetInfo() != NULL )
	{
		const KPetInfo *pPetInfo = pUnit->GetPetInfo();
#ifdef SERV_PETID_DATA_TYPE_CHANGE //2013.07.02
		const CX2PetManager::PetTemplet* pPetTemplet = g_pData->GetPetManager()->GetPetTemplet((CX2PetManager::PET_UNIT_ID)pPetInfo->m_iPetID);
#else //SERV_PETID_DATA_TYPE_CHANGE
		const CX2PetManager::PetTemplet* pPetTemplet = g_pData->GetPetManager()->GetPetTemplet((CX2PetManager::PET_UNIT_ID)pPetInfo->m_cPetID);
#endif //SERV_PETID_DATA_TYPE_CHANGE
		if ( pPetTemplet && pPetInfo->m_cEvolutionStep >= 0 && pPetInfo->m_cEvolutionStep < (int) pPetTemplet->m_Evolution_Step_InitLuaTemplet.size() )
		{
			const CX2PetManager::PetInitTemplet* pPetInitTemplet 
				= pPetTemplet->m_Evolution_Step_InitLuaTemplet[ pPetInfo->m_cEvolutionStep ].m_pInitTemplet;
			if ( pPetInitTemplet != NULL )
			{
				pPetInitTemplet->m_init.AppendToDeviceList( kLoadUserUnitData.m_listKTDXDeviceData );
			}
		}
	}

#endif	SERV_PET_SYSTEM
#endif  X2OPTIMIZE_VILLAGE_OR_SQUARE_PET_BACKGROUND_LOAD

}//CX2UnitLoader::AddUnitLoadDataForSquareOrField()


bool CX2UnitLoader::IsUnitLoadComplete( UidType unitUID )
{
	return m_mapLoadUnitDataLoaded.find( unitUID ) != m_mapLoadUnitDataLoaded.end();
}

void CX2UnitLoader::DeleteUnitLoadData( UidType unitUID )
{
    for( LoadUserUnitDataList::iterator iter = m_listLoadUnitData.begin();
        iter != m_listLoadUnitData.end(); )
    {
        if ( iter->m_UnitUID == unitUID )
            iter = m_listLoadUnitData.erase( iter );
        else
            iter++;
    }//for

	m_mapLoadUnitDataLoaded.erase( unitUID );

}

void CX2UnitLoader::ClearUnitLoadData()
{
	m_listLoadUnitData.clear();
    m_mapLoadUnitDataLoaded.clear();
}


void	CX2UnitLoader::AddNPCUnitUIDResource( UINT uid, CX2UnitManager::NPC_UNIT_ID id, const KAttribEnchantNpcInfo* pkAttribInfo )
{
	if ( uid == UINT(-1) )
		return;
    const CX2UnitManager::NPCUnitInfo* pInfo = g_pData->GetUnitManager()->GetNPCUnitInfo( id );
    if ( pInfo == NULL || pInfo->m_pInitData == NULL )
        return;

	if ( m_mapNPCUnitUIDResourceLoaded.find( uid ) != m_mapNPCUnitUIDResourceLoaded.end() )
		return;
    BOOST_FOREACH( const NPCResource& npcResource, m_listNPCResource )
    {
        if ( npcResource.m_uid == uid )
            return;
    }//BOOST_FOREACH()
    m_listNPCResource.resize( m_listNPCResource.size() + 1 );
    NPCResource& npcResource = m_listNPCResource.back();
    npcResource.m_uid = uid;
	npcResource.m_id = id;
    const CX2GUNPC::InitData& initData = pInfo->m_pInitData->m_init;
    initData.AppendToDeviceList( npcResource.m_listKTDXDeviceData );
	if ( pkAttribInfo != NULL && pkAttribInfo->IsValidInfo() == true )
		CX2GUNPC::AppendEnchantMonsterResourcesToDeviceList( npcResource.m_listKTDXDeviceData, *pkAttribInfo );
}


void    CX2UnitLoader::AddNPCUnitIDResource( CX2UnitManager::NPC_UNIT_ID id )
{
    const CX2UnitManager::NPCUnitInfo* pInfo = g_pData->GetUnitManager()->GetNPCUnitInfo( id );
    if ( pInfo == NULL || pInfo->m_pInitData == NULL )
        return;

	if ( m_mapNPCUnitIDResourceLoaded.find( id ) != m_mapNPCUnitIDResourceLoaded.end() )
		return;
    BOOST_FOREACH( const NPCResource& npcResource, m_listNPCResource )
    {
        if ( npcResource.m_uid == UINT(-1) && npcResource.m_id == id )
            return;
    }//BOOST_FOREACH()

    m_listNPCResource.resize( m_listNPCResource.size() + 1 );
    NPCResource& npcResource = m_listNPCResource.back();
	npcResource.m_uid = UINT(-1);
    npcResource.m_id = id;
    const CX2GUNPC::InitData& initData = pInfo->m_pInitData->m_init;
    initData.AppendToDeviceList( npcResource.m_listKTDXDeviceData );

}//CX2UnitLoader::AddNPCResources()


bool	CX2UnitLoader::IsNPCUnitUIDResourceLoadCompleted( UINT uid )
{
	if ( uid == UINT(-1) )
		return false;
	return	m_mapNPCUnitUIDResourceLoaded.find( uid ) != m_mapNPCUnitUIDResourceLoaded.end();
}

bool	CX2UnitLoader::IsNPCUnitIDResourceLoadCompleted( CX2UnitManager::NPC_UNIT_ID id )
{
	return m_mapNPCUnitIDResourceLoaded.find( id ) != m_mapNPCUnitIDResourceLoaded.end();
}

void	CX2UnitLoader::RemoveNPCUnitUIDResource( UINT uid )
{
	if ( uid == UINT(-1) )
		return;

    for( NPCResourceList::iterator iter = m_listNPCResource.begin();
        iter != m_listNPCResource.end(); )
    {
        if ( iter->m_uid == uid )
            iter = m_listNPCResource.erase( iter );
        else
            iter++;
    }//for

	m_mapNPCUnitUIDResourceLoaded.erase( uid );
}

void	CX2UnitLoader::RemoveNPCUnitIDResource( CX2UnitManager::NPC_UNIT_ID id )
{
    for( NPCResourceList::iterator iter = m_listNPCResource.begin();
        iter != m_listNPCResource.end(); )
    {
        if ( iter->m_uid == UINT(-1) && iter->m_id == id )
            iter = m_listNPCResource.erase( iter );
        else
            iter++;
    }//for

	m_mapNPCUnitIDResourceLoaded.erase( id );
}

void    CX2UnitLoader::RemoveAllNPCResources()
{
    m_listNPCResource.clear();
	m_mapNPCUnitUIDResourceLoaded.clear();
    m_mapNPCUnitIDResourceLoaded.clear();

}//CX2UnitLoader::RemoveAllNPCResources()

