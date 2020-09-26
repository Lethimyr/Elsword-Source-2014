#include <StdAfx.h>


#include <boost/scoped_array.hpp>
#ifdef  X2OPTIMIZE_SCRIPT_PREPROCESSING_TOOL
#include <X2Stub.h>
#endif  X2OPTIMIZE_SCRIPT_PREPROCESSING_TOOL

#ifdef  X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING


/*static*/
void CX2ItemManager::RegisterProxyLuabind( lua_State* L )
{
	lua_tinker::class_add<CX2ItemManager::KProxy>( L, "CX2ItemManagerProxy" );
	lua_tinker::class_def<CX2ItemManager::KProxy>( L, "AddItemTemplet",				&CX2ItemManager::KProxy::AddItemTemplet_LUA );
	lua_tinker::class_def<CX2ItemManager::KProxy>( L, "AddSetItemData_LUA",				&CX2ItemManager::KProxy::AddSetItemData_LUA );
#ifdef SERV_ITEM_LUA_TRANS_DEVIDE
	lua_tinker::class_def<CX2ItemManager::KProxy>( L, "AddItemTempletTrans",		&CX2ItemManager::KProxy::AddItemTempletTrans_LUA );
	lua_tinker::class_def<CX2ItemManager::KProxy>( L, "AddSetItemDataTrans_LUA",	&CX2ItemManager::KProxy::AddSetItemDataTrans_LUA );
#endif SERV_ITEM_LUA_TRANS_DEVIDE
	lua_tinker::class_add<CX2ItemManager::KProxy2>( L, "CX2ItemManagerProxy2" );
	lua_tinker::class_def<CX2ItemManager::KProxy2>( L, "AddItemTemplet",			&CX2ItemManager::KProxy2::AddItemTemplet_LUA );
	lua_tinker::class_def<CX2ItemManager::KProxy2>( L, "AddSetItemData_LUA",		&CX2ItemManager::KProxy2::AddSetItemData_LUA );
}


bool    CX2ItemManager::KProxy::AddItemTemplet_LUA()
{
	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
#ifndef X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
    TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING

    CX2Item::USE_CONDITION  eUseCondition = CX2Item::UC_ANYONE;
	LUA_GET_VALUE_ENUM(	luaManager, "m_UseCondition",		eUseCondition,			CX2Item::USE_CONDITION,		CX2Item::UC_ANYONE );
	if( CX2Item::UC_NONE == eUseCondition )
	{
		// 사용하지 못하는 아이템이다
		return false;
	}

    DWORD   dwItemID = 0;
    std::wstring        wstrName;
    CX2Item::ITEM_TYPE  eItemType;
	LUA_GET_VALUE_RETURN(	luaManager, "m_ItemID",	dwItemID, 0, return false; );
	LUA_GET_VALUE_RETURN(	luaManager, "m_Name", wstrName, L"", return false; );
	LUA_GET_VALUE_RETURN_ENUM(	luaManager, "m_ItemType", eItemType, CX2Item::ITEM_TYPE, CX2Item::IT_NONE, return false; );

    m_setItemIDs.insert( dwItemID );
    return true;
}

bool    CX2ItemManager::KProxy::AddSetItemData_LUA()
{
    DWORD dwSetID = 0;
	bool bCheckNew = false;

	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
#ifndef X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
	TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING

	LUA_GET_VALUE(	luaManager, "m_SetID",						dwSetID,					0	);

	if ( dwSetID == 0 )
		return false;

    m_setSetIDs.insert( dwSetID );

	return true;
}

#ifdef SERV_ITEM_LUA_TRANS_DEVIDE
bool    CX2ItemManager::KProxy::AddItemTempletTrans_LUA()
{
	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
	TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );

    ItemTransData strItemTransData;
	DWORD   dwItemID = 0;
	std::wstring wstrTemp;
	// 여기서 입력 받아야 함.
    LUA_GET_VALUE_RETURN(	luaManager, "m_ItemID",	dwItemID, 0, return false; );
    LUA_GET_VALUE_RETURN(	luaManager, "m_Name", wstrTemp, L"", return false; );
	
	strItemTransData.m_Name = wstrTemp;
	LUA_GET_VALUE(			luaManager, "m_Description",				strItemTransData.m_Description,			L"" );
	LUA_GET_VALUE(			luaManager, "m_DescriptionInShop",			strItemTransData.m_DescriptionInShop,		L"" );
	LUA_GET_VALUE(			luaManager, "m_DescriptionInSkillNote",	strItemTransData.m_DescriptionInSkillNote,	L"" );

	// 치환은 KProxy2 데이터 입력받을 때
	m_mapItemTrans.insert(std::make_pair(dwItemID, strItemTransData));

	return true;
}

bool    CX2ItemManager::KProxy::AddSetItemDataTrans_LUA()
{
	DWORD dwSetID = 0;
	bool bCheckNew = false;
	std::wstring wstrTemp;

	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
	TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );
	
    LUA_GET_VALUE_RETURN(	luaManager, "m_SetID",	dwSetID, 0, return false; );
    LUA_GET_VALUE_RETURN(	luaManager, "m_SetName", wstrTemp, L"", return false; );
	// 치환은 KProxy2 데이터 입력받을 때
	m_mapSetItemTrans.insert(std::make_pair(dwSetID, wstrTemp));
	return true;
}
#endif //SERV_ITEM_LUA_TRANS_DEVIDE

#ifdef SERV_ITEM_LUA_TRANS_DEVIDE
CX2ItemManager::KProxy2::KProxy2( const std::set<DWORD>& setItemIDs, const std::set<DWORD>& setSetIDs, const std::map<DWORD, ItemTransData>& mapItemTrans, const std::map<DWORD, std::wstring>& mapSetItemTrans )
#else //SERV_ITEM_LUA_TRANS_DEVIDE
CX2ItemManager::KProxy2::KProxy2( const std::set<DWORD>& setItemIDs, const std::set<DWORD>& setSetIDs )
#endif //SERV_ITEM_LUA_TRANS_DEVIDE
{
#ifdef SERV_ITEM_LUA_TRANS_DEVIDE
		m_mapItemTrans = mapItemTrans;
		m_mapSetItemTrans = mapSetItemTrans;
#endif //SERV_ITEM_LUA_TRANS_DEVIDE

    m_setSetIDs = setSetIDs;
    DWORD   dwNumItem = setItemIDs.size();
    DWORD   dwNumSetIDs = setSetIDs.size();
    DWORD   dwReserveSize = sizeof(CX2Item::KItemFormatHeader) + dwNumItem*( sizeof(DWORD) + sizeof(CX2Item::KItemFormatTemplet) )
        + dwNumSetIDs*( sizeof(DWORD) + sizeof(CX2Item::KItemFormatSetItemData) );
    m_kFileSerializer.SetSaveStringEndNull( true );
    m_kFileSerializer.Reserve( dwReserveSize );

    {
        CX2Item::KItemFormatHeader   kHeader;
        kHeader.m_dwMagic = KX2ITEMMANAGER_FORMAT_MAGIC;
        kHeader.m_dwVersion = KX2ITEMMANAGER_FORMAT_VERSION;
        kHeader.m_dwNumItems = dwNumItem;
        kHeader.m_dwNumSetIDs = dwNumSetIDs;

        m_kFileSerializer.AppendData( sizeof(CX2Item::KItemFormatHeader), &kHeader );
    }
    for( std::set<DWORD>::const_iterator iter = setItemIDs.begin(); iter != setItemIDs.end(); ++iter )
    {
        m_kFileSerializer.AppendData( sizeof(DWORD), &(*iter) );
    }
    {
        CX2Item::KItemFormatTemplet kDummy;
        DWORD dwDummy = 0;
        for( DWORD dwItem = 0; dwItem != dwNumItem; ++dwItem )
        {
            m_kFileSerializer.AppendData( sizeof(CX2Item::KItemFormatTemplet), &kDummy );
        }
    }
    for( std::set<DWORD>::const_iterator iter = setSetIDs.begin(); iter != setSetIDs.end(); ++iter )
    {
        m_kFileSerializer.AppendData( sizeof(DWORD), &(*iter) );
    }
    {
        CX2Item::KItemFormatSetItemData kDummy2;
        DWORD dwDummy2 = 0;
        for( DWORD dwSetID = 0; dwSetID != dwNumSetIDs; ++dwSetID )
        {
            m_kFileSerializer.AppendData( sizeof(CX2Item::KItemFormatSetItemData), &kDummy2 );
        }
    }
}


DWORD   CX2ItemManager::KProxy2::AppendD3DXVECTOR3( const D3DXVECTOR3& v, bool bDefaultOne )
{
    if ( bDefaultOne == false && v.x == 0.f && v.y == 0.f && v.z == 0.f 
        || bDefaultOne == true && v.x == 1.f && v.y == 1.f && v.z == 1.f )
        return  0;
    KD3DXVECTOR3OffsetMap::const_iterator iter = m_mapVectorOffset.find( v );
    if ( iter != m_mapVectorOffset.end() )
        return  iter->second;
    m_kFileSerializer.AlignSpace(sizeof(float));
    DWORD   dwOffset = m_kFileSerializer.AppendData( sizeof(D3DXVECTOR3), &v );
    m_mapVectorOffset.insert( KD3DXVECTOR3OffsetMap::value_type( v, dwOffset ) );
    return  dwOffset;
}

unsigned int    CX2ItemManager::GetItemNum() const
{
    if ( m_vecItemTempletInfo.size() < sizeof(CX2Item::KItemFormatHeader) )
        return 0;
    const CX2Item::KItemFormatHeader* pHeader = (const CX2Item::KItemFormatHeader*) (&m_vecItemTempletInfo.front() );
    return  pHeader->m_dwNumItems;
}


unsigned int    CX2ItemManager::GetNumSetIDs() const
{
    if ( m_vecItemTempletInfo.size() < sizeof(CX2Item::KItemFormatHeader) )
        return 0;
    const CX2Item::KItemFormatHeader* pHeader = (const CX2Item::KItemFormatHeader*) (&m_vecItemTempletInfo.front() );
    return  pHeader->m_dwNumSetIDs;
}


int CX2ItemManager::GetIndexFromItemID( int iItemID ) const
{
    DWORD   dwNumItems = GetItemNum();
    if ( dwNumItems == 0 )
        return -1;
    const DWORD* pdwItemID = (const DWORD*) ( &m_vecItemTempletInfo.front() + sizeof(CX2Item::KItemFormatHeader) );
    const DWORD* pdwFound = std::lower_bound( &pdwItemID[0], &pdwItemID[dwNumItems], (DWORD) iItemID );
    if ( pdwFound == &pdwItemID[dwNumItems]
         || *pdwFound != (DWORD) iItemID )
        return -1;
    return  (int) ( pdwFound - pdwItemID );
}

DWORD   CX2ItemManager::GetItemIDFromIndex( unsigned uiIndex ) const
{
    DWORD   dwNumItems = GetItemNum();
    if ( uiIndex >= dwNumItems )
        return 0;
    DWORD   dwItemOffset = sizeof(CX2Item::KItemFormatHeader) + sizeof(DWORD) * uiIndex;
    return  *( (const DWORD*) ( &m_vecItemTempletInfo.front() + dwItemOffset ) );
}

const CX2Item::ItemTemplet* CX2ItemManager::GetItemTempletFromIndex( unsigned uiIndex ) const
{
    DWORD   dwNumItems = GetItemNum();
    if ( uiIndex >= dwNumItems )
        return NULL;
    DWORD   dwItemOffset = sizeof(CX2Item::KItemFormatHeader) + sizeof(DWORD) * uiIndex;
    const BYTE* pData = (const BYTE*) &m_vecItemTempletInfo.front();
    dwItemOffset = sizeof(CX2Item::KItemFormatHeader) + sizeof(DWORD) * dwNumItems + sizeof(CX2Item::KItemFormatTemplet)*uiIndex;
    return  (const CX2Item::KItemFormatTemplet*) ( pData + dwItemOffset );
}

const CX2Item::ItemTemplet* CX2ItemManager::GetItemTemplet( const int itemID ) const
{
    if ( itemID == 0 )
        return NULL;
    DWORD   dwNumItems = GetItemNum();
    if ( dwNumItems == 0 )
        return NULL;
    const BYTE* pData = (const BYTE*) &m_vecItemTempletInfo.front();
    const DWORD* pdwItemID = (const DWORD*) ( pData + sizeof(CX2Item::KItemFormatHeader) );
    const DWORD* pdwFound = std::lower_bound( &pdwItemID[0], &pdwItemID[dwNumItems], (DWORD) itemID );
    if ( pdwFound == &pdwItemID[dwNumItems]
         || *pdwFound != (DWORD) itemID )
        return NULL;

    DWORD   dwIndex = pdwFound - pdwItemID;
    DWORD   dwItemOffset = sizeof(CX2Item::KItemFormatHeader) + sizeof(DWORD) * dwNumItems + sizeof(CX2Item::KItemFormatTemplet)*dwIndex;
    return  (const CX2Item::KItemFormatTemplet*) ( &m_vecItemTempletInfo.front() + dwItemOffset );
}

bool    CX2ItemManager::KProxy2::AddSetItemData_LUA()
{
	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
#ifndef X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
	TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
    
    char aszError[MAX_PATH];
    bool    bCheckNew = false;
    DWORD   dwSetID = 0;
	LUA_GET_VALUE(	luaManager, "m_SetID",						dwSetID,					0	);
	if ( dwSetID == 0 )
		return false;

	// dwItemID로 비교해서 매핑할 때 바꾸어준다
#ifdef SERV_ITEM_LUA_TRANS_DEVIDE
	bool bSetItemTransData = false;
	std::map<DWORD, std::wstring>::iterator mitSetItemTrans;
	mitSetItemTrans = m_mapSetItemTrans.find(dwSetID);
	if( mitSetItemTrans == m_mapSetItemTrans.end() )
	{
		//return false; // 에러로그 출력 후 한글 보여준다.
	}
	else
	{
		bSetItemTransData = true;
	}
#endif //SERV_ITEM_LUA_TRANS_DEVIDE

    const CX2Item::KItemFormatHeader* pkHeader = (const CX2Item::KItemFormatHeader*) m_kFileSerializer.AccessData( 0, sizeof(CX2Item::KItemFormatHeader) );
    ASSERT( pkHeader );
    if ( pkHeader == NULL )
    {
        ErrorLogMsg( XEM_ERROR145, "INVALID_HEADER" );
        return false;
    }
    DWORD dwNumSetIDs = pkHeader->m_dwNumSetIDs;

    DWORD   dwOffset =  sizeof(CX2Item::KItemFormatHeader) + 
        pkHeader->m_dwNumItems * ( sizeof(DWORD) + sizeof(CX2Item::KItemFormatTemplet) );

    const DWORD* pdwSetID = (const DWORD*) m_kFileSerializer.AccessData( dwOffset, sizeof(DWORD) * dwNumSetIDs );
    ASSERT( pdwSetID );
    if ( pdwSetID == NULL )
    {
        ErrorLogMsg( XEM_ERROR145, "INVALID_SETID_TABLE" );
        return false;
    }//if

    const DWORD* pdwFound = std::lower_bound( &pdwSetID[0], &pdwSetID[dwNumSetIDs], dwSetID );
    if ( pdwFound == &pdwSetID[dwNumSetIDs] || *pdwFound != dwSetID )
    {
        ASSERT( 0 );
        sprintf_s( aszError, ARRAY_SIZE( aszError ), "INVALID_SETID_1_%d", dwSetID );
        ErrorLogMsg( XEM_ERROR145, aszError );
        return false;
    }//if
    DWORD dwIndex = pdwFound - pdwSetID;
    dwOffset += dwNumSetIDs * sizeof(DWORD) + dwIndex * sizeof(CX2Item::KItemFormatSetItemData);
    CX2Item::KItemFormatSetItemData kSetItemData;
    kSetItemData.m_dwSetID = dwSetID;
    {
        const CX2Item::KItemFormatSetItemData* pSetItemData = (const CX2Item::KItemFormatSetItemData*) 
            m_kFileSerializer.AccessData( dwOffset, sizeof(CX2Item::KItemFormatSetItemData) );
        ASSERT( pSetItemData );
        if ( pSetItemData == NULL )
        {
            sprintf_s( aszError, ARRAY_SIZE( aszError ), "SETID_%d: INVALID_SETITEMDATA", dwSetID );
            ErrorLogMsg( XEM_ERROR145, aszError );
            return false;
        }
        if ( pSetItemData->m_dwSetID == dwSetID )
        {
            kSetItemData = *pSetItemData;
        }
        else
        {
            bCheckNew = true;
        }
    }
    KSetItemDataMap::iterator iter = m_mapSetItemDataMap.find( dwSetID );
    if ( iter == m_mapSetItemDataMap.end() )
    {
        iter = m_mapSetItemDataMap.insert( KSetItemDataMap::value_type( dwSetID, KSetItemData() ) ).first;
    }

    std::wstring wstrTemp;
	//09. 05. 14 김정협 머지를 위해서 인덱스 읽는 부분 막음
	LUA_GET_VALUE(	luaManager, "m_SetName",					wstrTemp,				L""	);
	//LUA_GET_VALUE(	luaManager, "m_iSetName_Index",			m_nString_Index,		0);
	//pSetItemData->m_SetName = GET_SCRIPT_STRING(m_nString_Index);
#ifdef SERV_ITEM_LUA_TRANS_DEVIDE
	if(bSetItemTransData)
	{
		wstrTemp = mitSetItemTrans->second;
	}
#endif //SERV_ITEM_LUA_TRANS_DEVIDE
    kSetItemData.m_dwOffset_SetName = m_kFileSerializer.AppendWideString( true, false, wstrTemp );

    DWORD   dwNeedPartsNum = 0;
	LUA_GET_VALUE(	luaManager, "m_NeedPartsNum", dwNeedPartsNum,			0 );

    if ( dwNeedPartsNum > 0 )
    {
        std::vector<int>  vecOptions;

	    for ( int i = 1; i <= 15; i++ )
	    {
		    stringstream strstm;
		    int iOptionNum;
		    strstm << "m_Option" << i;
		    LUA_GET_VALUE(	luaManager, strstm.str().c_str(),	iOptionNum,		0 );
		    if ( iOptionNum!= 0 )
		    {
			    vecOptions.push_back( iOptionNum );
		    }
	    }
        iter->second.m_mapNeedPartsNumNOptions.insert( std::map< DWORD, vector< int > >::value_type( dwNeedPartsNum, vecOptions ) );
    }

    m_kFileSerializer.CopyData( dwOffset, sizeof(kSetItemData), &kSetItemData );

	return true;
}


void    CX2ItemManager::KProxy2::PostProcess()
{
    char aszError[MAX_PATH];

    const CX2Item::KItemFormatHeader* pkHeader = (const CX2Item::KItemFormatHeader*) m_kFileSerializer.AccessData( 0, sizeof(CX2Item::KItemFormatHeader) );
    ASSERT( pkHeader );
    if ( pkHeader == NULL )
    {
        ErrorLogMsg( XEM_ERROR145, "INVALID_HEADER" );
        return;
    }
    DWORD dwNumItems = pkHeader->m_dwNumItems;
    DWORD dwNumSetIDs = pkHeader->m_dwNumSetIDs;

    {
        DWORD   dwOffsetItem = sizeof(CX2Item::KItemFormatHeader) + dwNumItems * sizeof(DWORD);
        const CX2Item::KItemFormatTemplet* pTemplet = (const CX2Item::KItemFormatTemplet*) 
            m_kFileSerializer.AccessData( dwOffsetItem, sizeof(CX2Item::KItemFormatTemplet) * dwNumItems );
        if ( pTemplet == NULL )
        {
            ASSERT( 0 );
            ErrorLogMsg( XEM_ERROR144, "INVALID_ITEM_TEMPLET" );
        }
        else
        {
            for( DWORD dwIndex = 0; dwIndex != dwNumItems; ++dwIndex, ++pTemplet )
            {
                if ( pTemplet->GetSetID() != 0 && m_setSetIDs.find( pTemplet->GetSetID() ) == m_setSetIDs.end() )
                {
                    sprintf_s( aszError, ARRAY_SIZE( aszError ), "ITEMID_%d: INVALID_SETID_%d", pTemplet->GetSetID() );
                    ErrorLogMsg( XEM_ERROR144, aszError );
                }
            }//for
        }//if.. else..
    }

    DWORD   dwOffset =  sizeof(CX2Item::KItemFormatHeader) + 
        dwNumItems * ( sizeof(DWORD) + sizeof(CX2Item::KItemFormatTemplet) )
        + dwNumSetIDs * sizeof(DWORD);
    for( DWORD dwIndex = 0; dwIndex != dwNumSetIDs; ++dwIndex, dwOffset += sizeof(CX2Item::KItemFormatSetItemData) )
    {
        CX2Item::KItemFormatSetItemData kSetItemData;
        {
            m_kFileSerializer.RetrieveData( &kSetItemData, dwOffset, sizeof(CX2Item::KItemFormatSetItemData) );
            if ( kSetItemData.m_dwSetID == 0 )
            {
                ASSERT( 0 );
                ErrorLogMsg( XEM_ERROR145, "INVALID_SETITEMDATA_1" );
                continue;
            }
        }
        KSetItemDataMap::const_iterator iter = m_mapSetItemDataMap.find( kSetItemData.m_dwSetID );
        if ( iter == m_mapSetItemDataMap.end() )
        {
            ASSERT( 0 );
            sprintf_s( aszError, ARRAY_SIZE( aszError ), "INVALID_SETID_2_%d", kSetItemData.m_dwSetID );
            ErrorLogMsg( XEM_ERROR145, aszError );
            continue;
        }
        kSetItemData.m_iMaxLevel = iter->second.m_iMaxLevel;
        if ( iter->second.m_vecSetItemIDList.empty() == false )
        {
            DWORD   dwSize = iter->second.m_vecSetItemIDList.size();
            m_kFileSerializer.AlignSpace( sizeof(DWORD) );
            kSetItemData.m_dwOffset_ItemIDs = m_kFileSerializer.AppendData( sizeof(DWORD), &dwSize );
            m_kFileSerializer.AppendData( sizeof(DWORD)*dwSize, &iter->second.m_vecSetItemIDList.front() );
        }
        if ( iter->second.m_mapNeedPartsNumNOptions.empty() == false )
        {
            DWORD   dwCount = 0;
            for( std::map< DWORD, vector< int > >::const_iterator it2 = iter->second.m_mapNeedPartsNumNOptions.begin();
                it2 != iter->second.m_mapNeedPartsNumNOptions.end(); ++it2 )
            {
                dwCount += it2->second.size();
            }
            m_kFileSerializer.AlignSpace( sizeof(DWORD) );
            kSetItemData.m_dwOffset_NeedPartsNumNOptions = m_kFileSerializer.AppendData( sizeof(DWORD), &dwCount );
            CX2Item::KItemForamtNeedPartsNumAndOption    kData;
            for( std::map< DWORD, vector< int > >::const_iterator it2 = iter->second.m_mapNeedPartsNumNOptions.begin();
                it2 != iter->second.m_mapNeedPartsNumNOptions.end(); ++it2 )
            {
                kData.m_dwNeedPartsNum = it2->first;
                for( std::vector<int>::const_iterator it3 = it2->second.begin(); it3 != it2->second.end(); ++it3 )
                {
                    kData.m_iOption = *it3;
                    m_kFileSerializer.AppendData( sizeof(CX2Item::KItemForamtNeedPartsNumAndOption), &kData );
                }
            }
        }
        m_kFileSerializer.CopyData( dwOffset, sizeof(kSetItemData), &kSetItemData );
    }
}


bool    CX2ItemManager::KProxy2::AddItemTemplet_LUA()
{
	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
#ifndef X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
	TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
    
    static const char* s_aszSmallName[] = 
    {
        "", "Elsword", "Aisha", "Rena", "Raven", "Eve", "Chung", "Ara", "Elesis", "Add"
    };
    static const char* s_aszCapitalName[] = 
    {
        "", "ELSWORD", "AISHA", "RENA", "RAVEN", "EVE", "CHUNG", "ARA", "ELESIS", "ADD"
    };
    BOOST_STATIC_ASSERT( ARRAY_SIZE( s_aszSmallName ) == ARRAY_SIZE( s_aszCapitalName ) );
#ifndef X2OPTIMIZE_SCRIPT_PREPROCESSING_TOOL
#endif  X2OPTIMIZE_SCRIPT_PREPROCESSING_TOOL

    char    aszError[MAX_PATH];
    CX2Item::KItemFormatTemplet     kTemplet;
	
	/* 오현빈 // X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING주석 // 제가 이해한 내용을 바탕으로 작성 해서 틀린 내용 일 수 있습니다.
	꼭 필요한 값이 아닐 경우(각 캐릭별 offset 등) 스크립트에 값이 기록 되어 있을 때만 메모리에 등록 시키기 위해
	boost::scoped_array<CX2Item::KItemFormatType_OffsetOrValue>을 사용함.
	
	여기서 등록시킨 값을 읽어올 때는 각 타입별로 Offset을 모두 더해주는 방식이기 때문에 순서가 변경되면 오동작 할 수 있음*/
    boost::scoped_array<CX2Item::KItemFormatType_OffsetOrValue>
        apExtra( new CX2Item::KItemFormatType_OffsetOrValue[ARRAY_SIZE( s_aszSmallName )*5+CX2Item::STT_END] );
    DWORD   dwNumExtra = 0;
    DWORD   dwItemID = 0;
    DWORD   dwItemOffset = 0;

    std::wstring wstrTemp;
    std::wstring wstrTemp2;
    std::string strTemp;
    wstrTemp.reserve( MAX_PATH );
			
	/* 오현빈 // X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING주석 // 제가 이해한 내용을 바탕으로 작성 해서 틀린 내용 일 수 있습니다.
	더미로 삽입한 ItempTempltet을 얻어와서, 아이템 이름 설정*/
    {
        CX2Item::USE_CONDITION  eUseCondition = CX2Item::UC_ANYONE;
        CX2Item::ITEM_TYPE  eItemType;
        DWORD   dwNumItems = 0;
        DWORD   dwIndex = 0;

	    LUA_GET_VALUE_RETURN(	luaManager, "m_ItemID",	dwItemID, 0, return false; );
// dwItemID로 비교해서 매핑할 때 바꾸어준다
#ifdef SERV_ITEM_LUA_TRANS_DEVIDE
		bool bItemTransData = false;
		std::map<DWORD, ItemTransData>::iterator mitItemTrans;
		mitItemTrans = m_mapItemTrans.find(dwItemID);
		if( mitItemTrans == m_mapItemTrans.end() )
		{
			//return false; // 에러로그 출력 후 한글 보여준다.
		}
		else
		{
			bItemTransData = true;
		}
#endif //SERV_ITEM_LUA_TRANS_DEVIDE
	    LUA_GET_VALUE_RETURN(	luaManager, "m_Name", wstrTemp, L"", return false; );
#ifdef SERV_ITEM_LUA_TRANS_DEVIDE
		if( bItemTransData )
		{
			wstrTemp = mitItemTrans->second.m_Name;
		}
#endif //SERV_ITEM_LUA_TRANS_DEVIDE
	    LUA_GET_VALUE_RETURN_ENUM(	luaManager, "m_ItemType", eItemType, CX2Item::ITEM_TYPE, CX2Item::IT_NONE, return false; );

	    LUA_GET_VALUE_ENUM(	luaManager, "m_UseCondition",		eUseCondition,			CX2Item::USE_CONDITION,		CX2Item::UC_ANYONE );
	    if( CX2Item::UC_NONE == eUseCondition )
	    {
		    // 사용하지 못하는 아이템이다
		    return false;
	    }
        if ( false == ( eUseCondition >= CX2Item::USE_CONDITION(0) && eUseCondition < CX2Item::UC_NUM ) )
        {
            sprintf_s( aszError, ARRAY_SIZE( aszError ), "ITEMID_%d: INVALID_USE_CONDITION_%d", dwItemID, eUseCondition );
            ErrorLogMsg( XEM_ERROR144, aszError );
        }//if
        if ( false == ( eItemType >= CX2Item::ITEM_TYPE(0) && eItemType < CX2Item::IT_NUM ) )
        {
            sprintf_s( aszError, ARRAY_SIZE( aszError ), "ITEMID_%d: INVALID_ITEM_TYPE_%d", dwItemID, eItemType );
            ErrorLogMsg( XEM_ERROR144, aszError );
        }//if

        kTemplet.SetItemID( dwItemID );
        kTemplet.SetUseCondition( eUseCondition );
        kTemplet.SetItemType( eItemType );

        const CX2Item::KItemFormatHeader* pkHeader = (const CX2Item::KItemFormatHeader*) m_kFileSerializer.AccessData( 0, sizeof(CX2Item::KItemFormatHeader) );
        ASSERT( pkHeader );
        if ( pkHeader == NULL )
        {
            ErrorLogMsg( XEM_ERROR144, "INVALID_HEADER" );
            return false;
        }
        dwNumItems = pkHeader->m_dwNumItems;

        const DWORD* pdwItemID = (const DWORD*) m_kFileSerializer.AccessData( sizeof(CX2Item::KItemFormatHeader), sizeof(DWORD) * dwNumItems );
        ASSERT( pdwItemID );
        if ( pdwItemID == NULL )
        {
            ErrorLogMsg( XEM_ERROR144, "INVALID_ITEMID_TABLE" );
            return false;
        }

        const DWORD* pdwFound = std::lower_bound( &pdwItemID[0], &pdwItemID[dwNumItems], dwItemID );
        if ( pdwFound == &pdwItemID[dwNumItems] || *pdwFound != dwItemID )
        {
            ASSERT( 0 );
            sprintf_s( aszError, ARRAY_SIZE( aszError ), "INVALID_ITEMID_%d", dwItemID );
            ErrorLogMsg( XEM_ERROR144, aszError );
            return false;
        }//if
        dwIndex = pdwFound - pdwItemID;
        dwItemOffset = sizeof(CX2Item::KItemFormatHeader) + sizeof(DWORD) * dwNumItems + sizeof(CX2Item::KItemFormatTemplet)*dwIndex;
        {
            CX2Item::KItemFormatTemplet* pTemplet = (CX2Item::KItemFormatTemplet*) 
                m_kFileSerializer.AccessData( dwItemOffset, sizeof(CX2Item::KItemFormatTemplet) );
            ASSERT( pTemplet );
            if ( pTemplet == NULL )
            {
                sprintf_s( aszError, ARRAY_SIZE( aszError ), "ITEMID_%d: INVALID_ITEM_TEMPLET", dwItemID );
                ErrorLogMsg( XEM_ERROR144, aszError );
                return false;
            }
            if ( pTemplet->GetUseCondition() != CX2Item::UC_NONE )
            {
                // 이미 등록된 아이템이다.
                sprintf_s( aszError, ARRAY_SIZE( aszError ), "ITEMID_%d: DUPLICATED_ITEM_TEMPLET", dwItemID );
                ErrorLogMsg( XEM_ERROR144, aszError );
                return false;
            }//if
        }

		/* 오현빈 // X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING주석 // 제가 이해한 내용을 바탕으로 작성 해서 틀린 내용 일 수 있습니다.
		4바이트 초과 되는 값은 DWORD 자료형에 포인터주소를 기록함*/
        kTemplet.m_dwOffset_Name = m_kFileSerializer.AppendWideString( true, true, wstrTemp );
    }
	
	wstrTemp.resize( 0 );

    {
#ifdef  X2OPTIMIZE_SCRIPT_PREPROCESSING_TOOL
        int ePVPRank = 0;
	    LUA_GET_VALUE(	luaManager, "m_BuyPvpRankCondition", ePVPRank,	0 );
        kTemplet.SetBuyPvpRankCondition( ePVPRank );
        if ( ePVPRank < 0 )
        {
            sprintf_s( aszError, ARRAY_SIZE( aszError ), "ITEMID_%d: INVALID_PVPRANK_%d", dwItemID, ePVPRank );
            ErrorLogMsg( XEM_ERROR144, aszError );
        }//if

#else   X2OPTIMIZE_SCRIPT_PREPROCESSING_TOOL
#ifdef  SERV_PVP_NEW_SYSTEM
        CX2PVPEmblem::PVP_RANK  ePVPRank = CX2PVPEmblem::PVPRANK_NONE;
        BOOST_STATIC_ASSERT( CX2PVPEmblem::PVPRANK_NONE == 0 );
	    LUA_GET_VALUE_ENUM(	luaManager, "m_BuyPvpRankCondition", ePVPRank,	CX2PVPEmblem::PVP_RANK,	CX2PVPEmblem::PVPRANK_NONE );
        kTemplet.SetBuyPvpRankCondition( ePVPRank );

        if ( false == ( ePVPRank >= CX2PVPEmblem::PVP_RANK(0) && ePVPRank < CX2PVPEmblem::PVPRANK_MAX ) )
        {
            sprintf_s( aszError, ARRAY_SIZE( aszError ), "ITEMID_%d: INVALID_PVPRANK_%d", dwItemID, ePVPRank );
            ErrorLogMsg( XEM_ERROR144, aszError );
        }//if

#endif  SERV_PVP_NEW_SYSTEM
#endif  X2OPTIMIZE_SCRIPT_PREPROCESSING_TOOL
    }

    {
        CX2Item::USE_TYPE   eUseType;
        int                 iUseLevel;

	    LUA_GET_VALUE_ENUM(	luaManager, "m_UseType",			eUseType,				CX2Item::USE_TYPE,			CX2Item::UT_NONE );
	    LUA_GET_VALUE(		luaManager, "m_UseLevel",			iUseLevel,				0 );
		
		/* 오현빈 // X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING주석 // 제가 이해한 내용을 바탕으로 작성 해서 틀린 내용 일 수 있습니다.
		4바이트 미만으로 처리 할 수 있는 값은 bit단위로 쪼개서 사용
		KItemFormatTemplet::EFlags사용*/
		kTemplet.SetUseType( eUseType );
		kTemplet.SetUseLevel( iUseLevel );
		if ( false == ( eUseType >= CX2Item::USE_TYPE(0) && eUseType < CX2Item::UT_NUM ) )
        {
            sprintf_s( aszError, ARRAY_SIZE( aszError ), "ITEMID_%d: INVALID_USETYPE_%d", dwItemID, eUseType );
            ErrorLogMsg( XEM_ERROR144, aszError );
        }//if
    }

    {
#ifdef  X2OPTIMIZE_SCRIPT_PREPROCESSING_TOOL
        int eUnitType = 0;
        int eUnitClass = 0;
	    LUA_GET_VALUE(	luaManager, "m_UnitType",			eUnitType, 0);
	    LUA_GET_VALUE(	luaManager, "m_UnitClass",			eUnitClass, 0 );
        kTemplet.SetUnitType( eUnitType );
        kTemplet.SetUnitClass( eUnitClass );
#else   X2OPTIMIZE_SCRIPT_PREPROCESSING_TOOL
        CX2Unit::UNIT_TYPE  eUnitType = CX2Unit::UT_NONE;
        CX2Unit::UNIT_CLASS eUnitClass = CX2Unit::UC_NONE;
        BOOST_STATIC_ASSERT( CX2Unit::UT_NONE == 0 );
        BOOST_STATIC_ASSERT( CX2Unit::UC_NONE == 0 );
	    LUA_GET_VALUE_ENUM(	luaManager, "m_UnitType",			eUnitType,				CX2Unit::UNIT_TYPE,			CX2Unit::UT_NONE );
	    LUA_GET_VALUE_ENUM(	luaManager, "m_UnitClass",			eUnitClass,				CX2Unit::UNIT_CLASS,		CX2Unit::UC_NONE );
        kTemplet.SetUnitType( eUnitType );
        kTemplet.SetUnitClass( eUnitClass );

        if ( false == ( eUnitType >= CX2Unit::UNIT_TYPE(0) && eUnitType < CX2Unit::UT_END ) )
        {
            sprintf_s( aszError, ARRAY_SIZE( aszError ), "ITEMID_%d: INVALID_UNITTYPE_%d", dwItemID, eUnitType );
            ErrorLogMsg( XEM_ERROR144, aszError );
        }//if
        if ( false == ( eUnitClass >= CX2Unit::UNIT_TYPE(0) && eUnitClass < CX2Unit::UC_END ) )
        {
            sprintf_s( aszError, ARRAY_SIZE( aszError ), "ITEMID_%d: INVALID_UNITCLASS_%d", dwItemID, eUnitClass );
            ErrorLogMsg( XEM_ERROR144, aszError );
        }//if

#endif  X2OPTIMIZE_SCRIPT_PREPROCESSING_TOOL
    }

#ifdef SERV_ITEM_LUA_TRANS_DEVIDE
	bool bItemTransData = false;
	std::map<DWORD, ItemTransData>::iterator mitItemTrans;
	mitItemTrans = m_mapItemTrans.find(dwItemID);
	if( mitItemTrans == m_mapItemTrans.end() )
	{
		//return false; // 에러로그 출력 후 한글 보여준다.
	}
	else
	{
		bItemTransData = true;
	}
#endif //SERV_ITEM_LUA_TRANS_DEVIDE

    wstrTemp.resize( 0 );
	LUA_GET_VALUE(			luaManager, "m_Description",				wstrTemp,			L"" );
#ifdef SERV_ITEM_LUA_TRANS_DEVIDE
	if( bItemTransData )
	{
		wstrTemp = mitItemTrans->second.m_Description;
	}
#endif //SERV_ITEM_LUA_TRANS_DEVIDE
    kTemplet.m_dwOffset_Description = m_kFileSerializer.AppendWideString( true, true, wstrTemp );
	
    wstrTemp.resize( 0 );
	LUA_GET_VALUE(			luaManager, "m_DescriptionInShop",			wstrTemp,		L"" );
#ifdef SERV_ITEM_LUA_TRANS_DEVIDE
	if( bItemTransData )
	{
		wstrTemp = mitItemTrans->second.m_DescriptionInShop;
	}
#endif //SERV_ITEM_LUA_TRANS_DEVIDE
    kTemplet.m_dwOffset_DescriptionInShop = m_kFileSerializer.AppendWideString( true, true, wstrTemp );
	
    wstrTemp.resize( 0 );
	LUA_GET_VALUE( luaManager, "m_ModelName",							wstrTemp,			L"" );
    kTemplet.m_adwOffset_ModelName[0] = m_kFileSerializer.AppendWideString( true, false, wstrTemp );
    wstrTemp.resize( 0 );
	LUA_GET_VALUE( luaManager, "m_ModelName1",							wstrTemp,			L"" );
    kTemplet.m_adwOffset_ModelName[1] = m_kFileSerializer.AppendWideString( true, false, wstrTemp );
    wstrTemp.resize( 0 );
	LUA_GET_VALUE( luaManager, "m_TextureChangeXETName",				wstrTemp,	L"" );
    kTemplet.m_dwOffset_TextureChangeXETName = m_kFileSerializer.AppendWideString( true, false, wstrTemp );
    wstrTemp.resize( 0 );
	LUA_GET_VALUE( luaManager, "m_AniXETName",							wstrTemp,				L"" );
    kTemplet.m_dwOffset_AniXETName = m_kFileSerializer.AppendWideString( true, false, wstrTemp );
    wstrTemp.resize( 0 );
	LUA_GET_VALUE( luaManager, "m_AniName",								wstrTemp,				L"" );
    kTemplet.m_dwOffset_AniName = m_kFileSerializer.AppendWideString( true, false, wstrTemp );
    wstrTemp.resize( 0 );
	LUA_GET_VALUE( luaManager, "m_ShopImage",							wstrTemp,				L"" );
    kTemplet.m_dwOffset_ShopImage = m_kFileSerializer.AppendWideString( true, false, wstrTemp );
    wstrTemp.resize( 0 );
	LUA_GET_VALUE( luaManager, "m_DropViewer",							wstrTemp,				L"" );
    kTemplet.m_dwOffset_DropViewer = m_kFileSerializer.AppendWideString( true, false, wstrTemp );
	
    {
        CX2Item::ITEM_GRADE eItemGrade;
        bool    bFashion;
        bool    bVested;
#ifdef HIDE_SET_DESCRIPTION
		bool	bHideSetDesc;
#endif HIDE_SET_DESCRIPTION
#ifdef SERV_PVP_EQUIPMENT
		bool    bIsPvpItem;
#endif SERV_PVP_EQUIPMENT
        bool    bCanEnchant;
        bool    bCanUseInventory;
        bool    bNoEquip;
        bool    bIsPcBang;

	    //LUA_GET_VALUE_RETURN_ENUM(	luaManager, "m_ItemType",				eItemType,				CX2Item::ITEM_TYPE,		CX2Item::IT_NONE, SAFE_DELETE(pItemTemplet); return false; );
	    LUA_GET_VALUE_ENUM(			luaManager, "m_ItemGrade",				eItemGrade,				CX2Item::ITEM_GRADE,	CX2Item::IG_NORMAL );
	    LUA_GET_VALUE(				luaManager, "m_bFashion",				bFashion,				false );
	    LUA_GET_VALUE(				luaManager, "m_bVested",				bVested,				false );	
#ifdef HIDE_SET_DESCRIPTION
		LUA_GET_VALUE(				luaManager, "m_bHideSetDesc",			bHideSetDesc,			false );
#endif HIDE_SET_DESCRIPTION
#ifdef SERV_PVP_EQUIPMENT
		LUA_GET_VALUE(				luaManager, "m_bIsPvpItem",				bIsPvpItem,				false );
#endif SERV_PVP_EQUIPMENT
	    LUA_GET_VALUE(				luaManager, "m_bCanEnchant",			bCanEnchant,			false );
	    LUA_GET_VALUE(				luaManager, "m_bCanUseInventory",		bCanUseInventory,		false );	

	    //{{ 허상형 : [2009/7/29] //	착용 불가능한 장비 루아 파싱
	    LUA_GET_VALUE(				luaManager, "m_bNoEquip",				bNoEquip,				false );
	    //{{ 2010. 01. 05  최육사	PC방 프리미엄
	    LUA_GET_VALUE(				luaManager, "m_bPcBang",				bIsPcBang,				false );

        kTemplet.SetItemGrade( eItemGrade );
        kTemplet.SetFashion(bFashion );
        kTemplet.SetVested( bVested );
#ifdef HIDE_SET_DESCRIPTION
		kTemplet.SetHideSetDesc( bHideSetDesc );
#endif HIDE_SET_DESCRIPTION
#ifdef SERV_PVP_EQUIPMENT
		kTemplet.SetPvpItem( bIsPvpItem );
#endif SERV_PVP_EQUIPMENT
        kTemplet.SetCanEnchant( bCanEnchant );
        kTemplet.SetCanUseInventory( bCanUseInventory );
        kTemplet.SetNoEquip( bNoEquip );
        kTemplet.SetIsPcBang( bIsPcBang );

        if ( false == ( eItemGrade >= CX2Item::ITEM_GRADE(0) && eItemGrade < CX2Item::IG_NUM ) )
        {
            sprintf_s( aszError, ARRAY_SIZE( aszError ), "ITEMID_%d: INVALID_ITEM_GRADE_%d", dwItemID, eItemGrade );
            ErrorLogMsg( XEM_ERROR144, aszError );
        }//if

    }

	//}}
	//{{ 2011. 10. 14	최육사	배틀필드 아이템 레벨 시스템
//#ifdef SERV_BATTLE_FIELD_ITEM_LEVEL
    {
        int iItemLevel;
	    LUA_GET_VALUE(				luaManager, "m_ItemLevel",				iItemLevel,				0 );
        kTemplet.SetItemLevel( iItemLevel );
    }
//#endif SERV_BATTLE_FIELD_ITEM_LEVEL
	//}}

	//{{ kimhc // 2009-08-27 // 아이템 봉인(최대로 봉인 가능한 횟수, 봉인에 필요한 '봉인의 인장' 갯수)
//#ifdef	SEAL_ITEM
    {
	    int iMaxSealCount		= 0;	
	    LUA_GET_VALUE(				luaManager, "m_ucMaxSealCount",			iMaxSealCount,			0 );
        if ( iMaxSealCount < 0 || iMaxSealCount >= 256 )
        {
            sprintf_s( aszError, ARRAY_SIZE( aszError ), "ITEMID_%d: INVALID_MAXSEALCOUNT_%d", dwItemID, iMaxSealCount );
            ErrorLogMsg( XEM_ERROR144, aszError );
        }//if
        kTemplet.SetMaxSealCount( (BYTE) iMaxSealCount );
    }
//#endif	SEAL_ITEM
	//}} kimhc // 2009-08-27 // 아이템 봉인(최대로 봉인 가능한 횟수, 봉인에 필요한 '봉인의 인장' 갯수)

	

//#ifdef TRIPLE_ENCHANT_TEST
    {
        int iMaxEnchantCount;
	    LUA_GET_VALUE(				luaManager, "m_iMaxAttribEnchantCount",		iMaxEnchantCount,		0 );	
        kTemplet.SetMaxAttribEnchantCount( iMaxEnchantCount );
    }
//#else TRIPLE_ENCHANT_TEST
//	//{{AFX
//	switch( kTemplet.GetItemType() )
//	{
//	case CX2Item::IT_WEAPON:
//		{
//			kTemplet.SetMaxAttribEnchantCount( 2 );
//		} break;
//
//	case CX2Item::IT_DEFENCE:
//		{
//			kTemplet.SetMaxAttribEnchantCount( 1 );
//		} break;
//
//	default:
//		{
//			kTemplet.SetMaxAttribEnchantCount( 0 );
//		} break;
//	}
//	//}}AFX
//#endif TRIPLE_ENCHANT_TEST

    {
#ifdef  X2OPTIMIZE_SCRIPT_PREPROCESSING_TOOL
        int eEqipPosition = 1;
	    LUA_GET_VALUE(			luaManager, "m_EqipPosition",			eEqipPosition, 1 );
        kTemplet.SetEqipPosition( eEqipPosition );
        if ( eEqipPosition < 0 )
        {
            sprintf_s( aszError, ARRAY_SIZE( aszError ), "ITEMID_%d: INVALID_EQIP_POSITION_%d", dwItemID, eEqipPosition );
            ErrorLogMsg( XEM_ERROR144, aszError );
        }//if
#else   X2OPTIMIZE_SCRIPT_PREPROCESSING_TOOL
        CX2Unit::EQIP_POSITION  eEqipPosition = CX2Unit::EP_QUICK_SLOT;
        BOOST_STATIC_ASSERT( CX2Unit::EP_QUICK_SLOT == 1 );
	    LUA_GET_VALUE_ENUM(			luaManager, "m_EqipPosition",			eEqipPosition,			CX2Unit::EQIP_POSITION,		CX2Unit::EP_QUICK_SLOT );
        kTemplet.SetEqipPosition( eEqipPosition );
        if ( false == ( eEqipPosition >= CX2Unit::EQIP_POSITION(0) && eEqipPosition < CX2Unit::EP_END ) )
        {
            sprintf_s( aszError, ARRAY_SIZE( aszError ), "ITEMID_%d: INVALID_EQIP_POSITION_%d", dwItemID, eEqipPosition );
            ErrorLogMsg( XEM_ERROR144, aszError );
        }//if
#endif  X2OPTIMIZE_SCRIPT_PREPROCESSING_TOOL
    }

    wstrTemp.resize( 0 );
    LUA_GET_VALUE(				luaManager, "m_AttachFrameName",		wstrTemp,		L"" );
    kTemplet.m_adwOffset_AttachFrameName[0] = m_kFileSerializer.AppendWideString( true, false, wstrTemp );
    wstrTemp.resize( 0 );
	LUA_GET_VALUE(				luaManager, "m_AttachFrameName1",		wstrTemp,		L"" );
    kTemplet.m_adwOffset_AttachFrameName[1] = m_kFileSerializer.AppendWideString( true, false, wstrTemp );
    
    {


        char    aszBuffer[MAX_PATH];
        D3DXVECTOR3 vOffset;
        DWORD       dwOffset;
        for( int i = 1; i < ARRAY_SIZE( s_aszSmallName ); ++i )
        {
            sprintf_s( aszBuffer, ARRAY_SIZE(aszBuffer), "m_FrameOffset%sX", s_aszSmallName[i] );
	        LUA_GET_VALUE(				luaManager, aszBuffer,	vOffset.x,	0.0f );
            sprintf_s( aszBuffer, ARRAY_SIZE(aszBuffer), "m_FrameOffset%sY", s_aszSmallName[i] );
	        LUA_GET_VALUE(				luaManager, aszBuffer,	vOffset.y,	0.0f );
            sprintf_s( aszBuffer, ARRAY_SIZE(aszBuffer), "m_FrameOffset%sZ", s_aszSmallName[i] );
	        LUA_GET_VALUE(				luaManager, aszBuffer,	vOffset.z,	0.0f );
            dwOffset = AppendD3DXVECTOR3( vOffset, false );
            if ( dwOffset != 0 )
            {
                apExtra[dwNumExtra].m_dwType = (DWORD) i;
                apExtra[dwNumExtra].m_dwOffsetOrValue = dwOffset;
                dwNumExtra++;
                kTemplet.m_byNumFrameOffsets++;
            }
        }
    }

    {

        char    aszBuffer[MAX_PATH];
        D3DXVECTOR3 vScale[ARRAY_SIZE( s_aszCapitalName )];
        D3DXVECTOR3 vRotate[ARRAY_SIZE( s_aszCapitalName )];
        DWORD       dwOffset;
        for( int i = 1; i < ARRAY_SIZE( s_aszCapitalName ); ++i )
        {
            sprintf_s( aszBuffer, ARRAY_SIZE(aszBuffer), "%s_ITEM_SCALE_ROTATE", s_aszCapitalName[i] );
            strTemp.resize( 0 );
	        LUA_GET_VALUE_UTF8( luaManager, aszBuffer, strTemp, "" );
	        TokenizeByScaleRotate( strTemp, vScale[i], vRotate[i] );
        }
        for( int i = 1; i < ARRAY_SIZE( s_aszCapitalName ); ++i )
        {
            dwOffset = AppendD3DXVECTOR3( vScale[i], true );
            if ( dwOffset != 0 )
            {
                apExtra[dwNumExtra].m_dwType = (DWORD) i;
                apExtra[dwNumExtra].m_dwOffsetOrValue = dwOffset;
                dwNumExtra++;
                kTemplet.m_byNumRenderScales++;
            }
        }
        for( int i = 1; i < ARRAY_SIZE( s_aszCapitalName ); ++i )
        {
            dwOffset = AppendD3DXVECTOR3( vRotate[i], false );
            if ( dwOffset != 0 )
            {
                apExtra[dwNumExtra].m_dwType = (DWORD) i;
                apExtra[dwNumExtra].m_dwOffsetOrValue = dwOffset;
                dwNumExtra++;
                kTemplet.m_byNumRenderRotates++;
            }
        }
    }

    {
        bool                    bCanHyperMode;
        CX2Item::PERIOD_TYPE    ePeriodType;
        int                     iEndurance;
        int                     iEnduranceMin;
        int                     iEnduranceMax;
        float                   fRepairED;
        int                     iRepairVP;
        int                     iQuantity;
        CX2Item::SHOP_PRICE_TYPE    ePriceType;
        int                     iPrice;
        int                     iPricePvPPoint;

	    LUA_GET_VALUE(				luaManager, "m_bCanHyperMode",			bCanHyperMode,			false );
		LUA_GET_VALUE_ENUM(			luaManager, "m_PeriodType",				ePeriodType,				CX2Item::PERIOD_TYPE,		CX2Item::PT_INFINITY );
	    LUA_GET_VALUE(				luaManager, "m_Endurance",				iEndurance,				-1 );
	    LUA_GET_VALUE(				luaManager, "m_EnduranceDamageMin",		iEnduranceMin,	0 );
	    LUA_GET_VALUE(				luaManager, "m_EnduranceDamageMax",		iEnduranceMax,	0 );
	    LUA_GET_VALUE(				luaManager, "m_RepairED",				fRepairED,				0.0f );
	    LUA_GET_VALUE(				luaManager, "m_RepairVP",				iRepairVP,				0 );
	    LUA_GET_VALUE(				luaManager, "m_Quantity",				iQuantity,				0 );
	
	    LUA_GET_VALUE_ENUM(	luaManager, "m_PriceType",			ePriceType,				CX2Item::SHOP_PRICE_TYPE,	CX2Item::SPT_GP );
	    LUA_GET_VALUE(		luaManager, "m_Price",				iPrice,					0 );
	    LUA_GET_VALUE(		luaManager, "m_PricePvPPoint",		iPricePvPPoint,			0 );

        kTemplet.SetCanHyperMode( bCanHyperMode );
        kTemplet.SetPeriodType( ePeriodType );
        kTemplet.SetEndurance( iEndurance );
        kTemplet.SetEnduranceDamageMinMax( iEnduranceMin, iEnduranceMax );
        kTemplet.SetRepairED( fRepairED );
        kTemplet.SetRepairVP( iRepairVP );
        kTemplet.SetQuantity( iQuantity );
        kTemplet.SetShopPriceType( ePriceType );
        kTemplet.SetPrice( iPrice );
        kTemplet.SetPricePvPPoint( iPricePvPPoint );

        if ( false == ( ePeriodType >= CX2Item::PERIOD_TYPE(0) && ePeriodType < CX2Item::PT_NUM ) )
        {
            sprintf_s( aszError, ARRAY_SIZE( aszError ), "ITEMID_%d: INVALID_PERIOD_TYPE_%d", dwItemID, ePeriodType );
            ErrorLogMsg( XEM_ERROR144, aszError );
        }//if
        if ( false == ( ePriceType >= CX2Item::SHOP_PRICE_TYPE(0) && ePriceType < CX2Item::SPT_NUM ) )
        {
            sprintf_s( aszError, ARRAY_SIZE( aszError ), "ITEMID_%d: INVALID_SHOP_PRICE_TYPE_%d", dwItemID, ePriceType );
            ErrorLogMsg( XEM_ERROR144, aszError );
        }//if
    }

	//{{ 2010. 03. 22  최육사	기술의 노트
//#ifdef SERV_SKILL_NOTE
    wstrTemp.resize( 0 );
	LUA_GET_VALUE(				luaManager, "m_DescriptionInSkillNote",	wstrTemp,	L"" );
#ifdef SERV_ITEM_LUA_TRANS_DEVIDE
	if( bItemTransData )
	{
		wstrTemp = mitItemTrans->second.m_DescriptionInSkillNote;
	}
#endif //SERV_ITEM_LUA_TRANS_DEVIDE
    kTemplet.m_dwOffset_DescriptionInSkillNote = m_kFileSerializer.AppendWideString( true, true, wstrTemp );
//#endif SERV_SKILL_NOTE
	//}}
	
//#ifdef PVP_SEASON2_SOCKET
    {
        int iAttributeLevel;
	    LUA_GET_VALUE( luaManager, "m_iAttributeLevel",	iAttributeLevel,	0 );	
        kTemplet.SetAttributeLevel( iAttributeLevel );
    }
//#endif
    
//#ifdef COMMON_ITEM_TEMPLET_TEST

	if( kTemplet.GetUseCondition() == CX2Item::UC_ANYONE &&
		kTemplet.GetUseType() != CX2Item::UT_NONE ) // 아무나 사용할 수 있는 공용템이고 장착장비라면
	{
        char    aszBuffer[MAX_PATH];
        DWORD   adwModelNameOffset[ARRAY_SIZE( s_aszCapitalName )];
        DWORD   adwXETNameOffset[ARRAY_SIZE( s_aszCapitalName )];
        ZeroMemory( adwModelNameOffset, sizeof(adwModelNameOffset) );
        ZeroMemory( adwXETNameOffset, sizeof(adwXETNameOffset) );

        for( int i = 1; i < ARRAY_SIZE( s_aszCapitalName ); ++i )
        {
            sprintf_s( aszBuffer, ARRAY_SIZE(aszBuffer), "%s_MODEL_NAME", s_aszCapitalName[i] );
            wstrTemp.resize( 0 );
	        LUA_GET_VALUE( luaManager, aszBuffer, wstrTemp, L"" );
            if ( wstrTemp.empty() == false )
            {
                adwModelNameOffset[ i ] = m_kFileSerializer.AppendWideString( true, false, wstrTemp );
                sprintf_s( aszBuffer, ARRAY_SIZE(aszBuffer), "%s_XET_NAME", s_aszCapitalName[i] );
                wstrTemp.resize( 0 );
	            LUA_GET_VALUE( luaManager, aszBuffer, wstrTemp, L"" );
                if ( wstrTemp.empty() == false )
                    adwXETNameOffset[ i ] = m_kFileSerializer.AppendWideString( true, false, wstrTemp );
            }
        }

        for( int i = 1; i < ARRAY_SIZE( s_aszCapitalName ); ++i )
        {
            if ( adwModelNameOffset[i] != 0 )
            {
                apExtra[dwNumExtra].m_dwType = (DWORD) i;
                apExtra[dwNumExtra].m_dwOffsetOrValue = adwModelNameOffset[i];
                dwNumExtra++;
                kTemplet.m_byNumCommonItemModelNames++;
            }
        }
        for( int i = 1; i < ARRAY_SIZE( s_aszCapitalName ); ++i )
        {
            if ( adwXETNameOffset[i] != 0 )
            {
                apExtra[dwNumExtra].m_dwType = (DWORD) i;
                apExtra[dwNumExtra].m_dwOffsetOrValue = adwXETNameOffset[i];
                dwNumExtra++;
                kTemplet.m_byNumCommonItemXETNames++;
            }
        }
	}

//#endif COMMON_ITEM_TEMPLET_TEST

	if( luaManager.BeginTable( "m_Stat" ) == true )
	{
        CX2Item::KItemFormatStatData kStatData;
		LUA_GET_VALUE( luaManager, "m_fBaseHP",			kStatData.m_fBaseHP,			0.f );
		LUA_GET_VALUE( luaManager, "m_fAtkPhysic",		kStatData.m_fAtkPhysic,			0.f );
		LUA_GET_VALUE( luaManager, "m_fAtkMagic",		kStatData.m_fAtkMagic,			0.f );
		LUA_GET_VALUE( luaManager, "m_fDefPhysic",		kStatData.m_fDefPhysic,			0.f );
		LUA_GET_VALUE( luaManager, "m_fDefMagic",		kStatData.m_fDefMagic,			0.f );
		luaManager.EndTable();
        m_kFileSerializer.AlignSpace( sizeof(DWORD) );
        kTemplet.m_dwOffset_Stat = m_kFileSerializer.AppendData( sizeof(kStatData), &kStatData );
	}

//#ifdef ADD_ITEM_TEMPLET_ITEM
	if( luaManager.BeginTable( "m_Stat_Relation_Level" ) == true )
	{
		CX2Item::KItemFormatStatRelLVData kStatRelLVData;
		// 체력
		int iTempValue = -1;
		LUA_GET_VALUE( luaManager,	"m_byBaseHPRelLV",			iTempValue,			0 );
		if ( iTempValue < 0 || iTempValue >= 256 )
		{
			sprintf_s( aszError, ARRAY_SIZE( aszError ), "ITEMID_%d: INVALID_STAT_RELATION_LEVEL_%d", dwItemID, iTempValue );
			ErrorLogMsg( XEM_ERROR144, aszError );
		}//if
		kStatRelLVData.m_byBaseHPRelLV = static_cast<BYTE>(iTempValue);
	
		// 물공
		iTempValue = -1;
		LUA_GET_VALUE( luaManager,	"m_byAtkPhysicRelLV",			iTempValue,			0 );
		if ( iTempValue < 0 || iTempValue >= 256 )
		{
			sprintf_s( aszError, ARRAY_SIZE( aszError ), "ITEMID_%d: INVALID_STAT_RELATION_LEVEL_%d", dwItemID, iTempValue );
			ErrorLogMsg( XEM_ERROR144, aszError );
		}//if
		kStatRelLVData.m_byAtkPhysicRelLV = static_cast<BYTE>(iTempValue);

		// 마공
		iTempValue = -1;
		LUA_GET_VALUE( luaManager,	"m_byAtkMagicRelLV",			iTempValue,			0 );
		if ( iTempValue < 0 || iTempValue >= 256 )
		{
			sprintf_s( aszError, ARRAY_SIZE( aszError ), "ITEMID_%d: INVALID_STAT_RELATION_LEVEL_%d", dwItemID, iTempValue );
			ErrorLogMsg( XEM_ERROR144, aszError );
		}//if
		kStatRelLVData.m_byAtkMagicRelLV = static_cast<BYTE>(iTempValue);

		// 물방
		iTempValue = -1;
		LUA_GET_VALUE( luaManager,	"m_byDefPhysicRelLV",			iTempValue,			0 );
		if ( iTempValue < 0 || iTempValue >= 256 )
		{
			sprintf_s( aszError, ARRAY_SIZE( aszError ), "ITEMID_%d: INVALID_STAT_RELATION_LEVEL_%d", dwItemID, iTempValue );
			ErrorLogMsg( XEM_ERROR144, aszError );
		}//if
		kStatRelLVData.m_byDefPhysicRelLV = static_cast<BYTE>(iTempValue);
		
		// 마방
		iTempValue = -1;
		LUA_GET_VALUE( luaManager,	"m_byDefMagicRelLV",			iTempValue,			0 );
		if ( iTempValue < 0 || iTempValue >= 256 )
		{
			sprintf_s( aszError, ARRAY_SIZE( aszError ), "ITEMID_%d: INVALID_STAT_RELATION_LEVEL_%d", dwItemID, iTempValue );
			ErrorLogMsg( XEM_ERROR144, aszError );
		}//if
		kStatRelLVData.m_byDefMagicRelLV = static_cast<BYTE>(iTempValue);

		luaManager.EndTable();
		m_kFileSerializer.AlignSpace( sizeof(DWORD) );
		kTemplet.m_dwOffset_StatRelationLevel = m_kFileSerializer.AppendData( sizeof(kStatRelLVData), &kStatRelLVData );
	}
//#endif // ADD_ITEM_TEMPLET_ITEM


	//{{ kimhc // 2009-10-19 // 최대 MP 증가 값 추가
    {
	    float fAddMaxMP = 0.0f;
	    LUA_GET_VALUE( luaManager,		"m_fAddMaxMP",			fAddMaxMP,			0.f );
	    kTemplet.SetAddMaxMP( fAddMaxMP );
    }

	//}} kimhc // 2009-10-19 // 최대 MP 증가 값 추가

    {
        std::vector<WORD> vecusBuffFactorIndices;
	    if ( luaManager.BeginTable( "BUFF_FACTOR" ) )
	    {
		    int iIndex = 1;
		    UINT uiBuffFactorId = 0;
		    while( luaManager.GetValue( iIndex, uiBuffFactorId ) == true )
		    {
			    if ( uiBuffFactorId != 0 )
			    {
                    vecusBuffFactorIndices.push_back( uiBuffFactorId );
#ifndef  X2OPTIMIZE_SCRIPT_PREPROCESSING_TOOL
                    if ( CX2BuffTempletManager::GetInstance()->GetBuffFactorPtr( uiBuffFactorId ) == NULL )
                    {
                        sprintf_s( aszError, ARRAY_SIZE( aszError ), "ITEMID_%d: INVALID_BUFF_FACTOR_%d", dwItemID, uiBuffFactorId );
                        ErrorLogMsg( XEM_ERROR144, aszError );
                    }//if
#endif  X2OPTIMIZE_SCRIPT_PREPROCESSING_TOOL
			    }
			    ++iIndex;
		    }
		    luaManager.EndTable();
	    }
	    //CX2BuffTempletManager::GetInstance()->GetBuffFactorPtrFromBuffFactorList( luaManager, vecusBuffFactorIndices );
        if ( vecusBuffFactorIndices.empty() == false )
        {
            WORD    wSize = vecusBuffFactorIndices.size();
            m_kFileSerializer.AlignSpace( sizeof(WORD) );
            kTemplet.m_dwOffset_BuffFactorIndices = m_kFileSerializer.AppendData( sizeof(WORD), &wSize );
            m_kFileSerializer.AppendData( sizeof(WORD)*wSize, &vecusBuffFactorIndices.front() );
        }//if
    }


	if( luaManager.BeginTable( "m_SpecialAbilityList" ) == true )
	{
		int index = 1;
        std::vector<CX2Item::KItemFormatSpecialAbility>  vecSA;
		while( luaManager.BeginTable( index ) == true )
		{
			CX2Item::KItemFormatSpecialAbility sa;
            CX2Item::SPECIAL_ABILITY_TYPE   eSAType;
			LUA_GET_VALUE_ENUM( luaManager, "TYPE",				eSAType,		CX2Item::SPECIAL_ABILITY_TYPE, CX2Item::SAT_NONE );
            sa.SetType( eSAType );

            if ( false == ( eSAType >= CX2Item::SPECIAL_ABILITY_TYPE(0) && eSAType < CX2Item::SAT_END ) )
            {
                sprintf_s( aszError, ARRAY_SIZE( aszError ), "ITEMID_%d: INVALID_SPECIAL_ABILITY_TYPE_%d", dwItemID, eSAType );
                ErrorLogMsg( XEM_ERROR144, aszError );
            }//if

//#ifdef GUIDE_UNDEFINED_SAT_TYPE
//			wstring strSatType = L"";
//			switch(sa.m_Type) 
//			{
//			case CX2Item::SAT_TRANSFORM_MONSTER:
//				{
//					strSatType = L"SAT_TRANSFORM_MONSTER";
//				} break;
//			case CX2Item::SAT_WAY_OF_SWORD_GAUGE_UP:
//				{
//					strSatType = L"SAT_WAY_OF_SWORD_GAUGE_UP";
//				} break;
//			case CX2Item::SAT_FORCE_CRITICAL_MAX:
//				{
//					strSatType = L"SAT_FORCE_CRITICAL_MAX";
//				} break;
//			default:
//				break;
//			}
//
//			if( false == strSatType.empty() )
//			{
//				sa.m_Type = CX2Item::SAT_NONE;
//				MessageBox( g_pKTDXApp->GetHWND(), GET_REPLACED_STRING( ( STR_ID_18058, "L", strSatType) ), L"Error", MB_OK );
//			} 
//#endif //GUIDE_UNDEFINED_SAT_TYPE
			LUA_GET_VALUE(		luaManager, "COOL_TIME",		sa.m_CoolTime,	0 );
			LUA_GET_VALUE(		luaManager, "VALUE1",			sa.m_Value1,	0 );
			LUA_GET_VALUE(		luaManager, "VALUE2",			sa.m_Value2,	0 );
			LUA_GET_VALUE(		luaManager, "VALUE3",			sa.m_Value3,	0 );
//#ifdef SUMMON_MONSTER_CARD_SYSTEM
            wstrTemp.resize( 0 );
			LUA_GET_VALUE(		luaManager, "STRING_VALUE1",	wstrTemp,	L"Group 0" );		//Group으로 된 정보를 받아 올 수 있는 인자
            if ( wstrTemp.empty() == false && wstrTemp != L"Group 0" )
            {
                sa.m_dwOffset_StringValue1 = m_kFileSerializer.AppendWideString( true, false, wstrTemp );
            }
//#endif SUMMON_MONSTER_CARD_SYSTEM
            vecSA.push_back( sa );

			luaManager.EndTable();
			index++;
		}
		luaManager.EndTable();

        if ( vecSA.empty() == false )
        {
            DWORD    dwSize = vecSA.size();
            m_kFileSerializer.AlignSpace( sizeof(DWORD) );
            kTemplet.m_dwOffset_SpecialAbilityList = m_kFileSerializer.AppendData( sizeof(DWORD), &dwSize );
            m_kFileSerializer.AppendData( sizeof(CX2Item::KItemFormatSpecialAbility)*dwSize, &vecSA.front() );
        }//if
	}

    
	
	/* 오현빈 // X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING주석 // 제가 이해한 내용을 바탕으로 작성 해서 틀린 내용 일 수 있습니다.
	Vector 컨테이너의 경우 기본적으로 주소값을 저장하고, 해당 size를 얻기 위해, vector의 첫 번 째 주소값부터 DWORD만큼 얻어와서 size를 구함*/
	if( luaManager.BeginTable( "SOCKET_OPTION" ) == true )
	{
        std::vector<int>  vecdwSocket;
		int socketID = 0;
		int index = 1;
		while( true == luaManager.GetValue( index, socketID ) )
		{
			vecdwSocket.push_back( socketID );
			index++;
		}
		luaManager.EndTable();
        if ( vecdwSocket.empty() == false )
        {
            DWORD   dwSize = vecdwSocket.size();
            m_kFileSerializer.AlignSpace( sizeof(DWORD) );
            kTemplet.m_dwOffset_SocketOptions = m_kFileSerializer.AppendData( sizeof(DWORD), &dwSize );
            m_kFileSerializer.AppendData( sizeof(int)*dwSize, &vecdwSocket.front() );
        }
	}

//#ifdef ADD_ITEM_TEMPLET_ITEM
	if( luaManager.BeginTable( "RANDOM_SOCKET_GROUP_ID" ) == true )
	{
		std::vector<int>  vecdwSocket;
		int socketID = 0;
		int index = 1;
		while( true == luaManager.GetValue( index, socketID ) )
		{
			vecdwSocket.push_back( socketID );
			index++;
		}
		luaManager.EndTable();
		if ( vecdwSocket.empty() == false )
		{
			DWORD   dwSize = vecdwSocket.size();
			m_kFileSerializer.AlignSpace( sizeof(DWORD) );
			kTemplet.m_dwOffset_RandomSocketOptions = m_kFileSerializer.AppendData( sizeof(DWORD), &dwSize );
			m_kFileSerializer.AppendData( sizeof(int)*dwSize, &vecdwSocket.front() );
		}
	}
//#endif // ADD_ITEM_TEMPLET_ITEM

//#ifdef USER_WEAPON_PARTICLE_SCRIPT_TEST
	if( luaManager.BeginTable( "ATTACHED_PARTICLE" ) == true )
	{
        std::vector<CX2Item::KItemFormatAttachedData>    vecParticle;
		int index = 1;
		while( luaManager.BeginTable( index ) == true )
		{
			CX2Item::KItemFormatAttachedData kParticle;
            wstrTemp.resize( 0 );
			LUA_GET_VALUE( luaManager, "PARTICLE_NAME", wstrTemp, L"" );
            wstrTemp2.resize( 0 );
		    LUA_GET_VALUE( luaManager, "BONE_NAME",		wstrTemp2, L"" );
			if( false == wstrTemp.empty() &&
				false == wstrTemp2.empty() )
			{
                kParticle.m_dwOffset_Name = m_kFileSerializer.AppendWideString( true, false, wstrTemp );
                kParticle.m_dwOffset_BoneName = m_kFileSerializer.AppendWideString( true, false, wstrTemp2 );
                vecParticle.push_back( kParticle );
			}
		    luaManager.EndTable();
		    index++;
		}
		luaManager.EndTable();	// ATTACHED_PARTICLE
        if ( vecParticle.empty() == false )
        {
            DWORD   dwSize = vecParticle.size();
            m_kFileSerializer.AlignSpace( sizeof(DWORD) );
            kTemplet.m_dwOffset_AttachedParticleData = m_kFileSerializer.AppendData( sizeof(DWORD), &dwSize );
            m_kFileSerializer.AppendData( sizeof(CX2Item::KItemFormatAttachedData)*dwSize, &vecParticle.front() );
        }//if
	}
//#endif USER_WEAPON_PARTICLE_SCRIPT_TEST

//#ifdef SUB_EQUIP_TEST
	if( luaManager.BeginTable( "ATTACHED_MESH" ) == true )
	{
        std::vector<CX2Item::KItemFormatAttachedData>    vecMesh;
		int index = 1;
		while( luaManager.BeginTable( index ) == true )
		{
			CX2Item::KItemFormatAttachedData mesh;
            wstrTemp.resize( 0 );
			LUA_GET_VALUE( luaManager, "MESH_NAME", wstrTemp, L"" );
            wstrTemp2.resize( 0 );
			LUA_GET_VALUE( luaManager, "BONE_NAME",	wstrTemp2, L"" );

            if ( wstrTemp.empty() == false && wstrTemp2.empty() == false )
            {
                mesh.m_dwOffset_Name = m_kFileSerializer.AppendWideString( true, false, wstrTemp );
                mesh.m_dwOffset_BoneName = m_kFileSerializer.AppendWideString( true, false, wstrTemp2 );
//#ifdef ATTACH_SKINMESH
//			    data.bSkinMesh = false;
			    //if(	pItemTemplet->m_UseType == CX2Item::UT_SKIN )
			    //{
				//    data.bSkinMesh = true;
			    //}			
			
                ASSERT( ( mesh.m_dwOffset_Name & 0x800000000 ) == 0 );
			    if( (int)wstrTemp.find( L"Jiggle_Bone" ) != std::wstring::npos ) // 찾았다
			    {
                    mesh.m_dwOffset_Name |= 0x80000000;
			    }
			    //LUA_GET_VALUE( luaManager, "JIGGLE_BONE",	data.bJiggle,	false );
//#endif
                vecMesh.push_back( mesh );
            }
			luaManager.EndTable();
			index++;
		}
		luaManager.EndTable();	// ATTACHED_MESH

        if ( vecMesh.empty() == false )
        {
            DWORD   dwSize = vecMesh.size();
            m_kFileSerializer.AlignSpace( sizeof(DWORD) );
            kTemplet.m_dwOffset_AttachedMeshData = m_kFileSerializer.AppendData( sizeof(DWORD), &dwSize );
            m_kFileSerializer.AppendData( sizeof(CX2Item::KItemFormatAttachedData)*dwSize, &vecMesh.front() );
        }//if
	}
//#endif SUB_EQUIP_TEST

    {
        int iCoolTime = 0;
        int iSetID = 0;

	    LUA_GET_VALUE(		luaManager, "m_CoolTime",			iCoolTime,				0 );
	    LUA_GET_VALUE(		luaManager, "m_SetID",				iSetID,					0 );
        kTemplet.SetCoolTime( iCoolTime );
        kTemplet.SetSetID( iSetID );
    }

//#ifdef ITEM_SLASH_TRACE_COLOR_TEST

	if( luaManager.BeginTable( "SLASH_TRACE_COLOR" ) == true )
	{
		float afValue[4] = { 1.f, };
		for( int valueIndex = 0; valueIndex < ARRAY_SIZE(afValue); valueIndex++ )
		{
			if( false == luaManager.GetValue( valueIndex+1, afValue[valueIndex] ) )
				break;

			afValue[valueIndex] /= 255.f;
		}
        apExtra[dwNumExtra].m_dwType = (DWORD) CX2Item::STT_SLASH_TRACE;
        apExtra[dwNumExtra].m_dwOffsetOrValue = D3DXCOLOR( &afValue[0] );
        dwNumExtra++;
        kTemplet.m_byNumSlashTraces++;
		luaManager.EndTable(); // SLASH_TRACE_COLOR
	}

	if( luaManager.BeginTable( "SLASH_TRACE_TIP_COLOR" ) == true )
	{
		float afValue[4] = { 1.f, };
		for( int valueIndex = 0; valueIndex < ARRAY_SIZE(afValue); valueIndex++ )
		{
			if( false == luaManager.GetValue( valueIndex+1, afValue[valueIndex] ) )
				break;
			afValue[valueIndex] /= 255.f;
		}
        apExtra[dwNumExtra].m_dwType = (DWORD) CX2Item::STT_SLASH_TRACE_TIP;
        apExtra[dwNumExtra].m_dwOffsetOrValue = D3DXCOLOR( &afValue[0] );
        dwNumExtra++;
        kTemplet.m_byNumSlashTraces++;
		luaManager.EndTable(); // SLASH_TRACE_TIP_COLOR
	}


	if( luaManager.BeginTable( "HYPER_SLASH_TRACE_COLOR" ) == true )
	{
		float afValue[4] = { 1.f, };
		for( int valueIndex = 0; valueIndex < ARRAY_SIZE(afValue); valueIndex++ )
		{
			if( false == luaManager.GetValue( valueIndex+1, afValue[valueIndex] ) )
				break;
			afValue[valueIndex] /= 255.f;
		}
        apExtra[dwNumExtra].m_dwType = (DWORD) CX2Item::STT_HYPER_SLASH_TRACE;
        apExtra[dwNumExtra].m_dwOffsetOrValue = D3DXCOLOR( &afValue[0] );
        dwNumExtra++;
        kTemplet.m_byNumSlashTraces++;
		luaManager.EndTable(); // HYPER_SLASH_TRACE_COLOR
	}

	if( luaManager.BeginTable( "HYPER_SLASH_TRACE_TIP_COLOR" ) == true )
	{
		float afValue[4] = { 1.f, };
		for( int valueIndex = 0; valueIndex < ARRAY_SIZE(afValue); valueIndex++ )
		{
			if( false == luaManager.GetValue( valueIndex+1, afValue[valueIndex] ) )
				break;
			afValue[valueIndex] /= 255.f;
		}
        apExtra[dwNumExtra].m_dwType = (DWORD) CX2Item::STT_HYPER_SLASH_TRACE_TIP;
        apExtra[dwNumExtra].m_dwOffsetOrValue = D3DXCOLOR( &afValue[0] );
        dwNumExtra++;
        kTemplet.m_byNumSlashTraces++;
		luaManager.EndTable(); // HYPER_SLASH_TRACE_TIP_COLOR
	}

//#endif ITEM_SLASH_TRACE_COLOR_TEST

//#ifdef BUFF_TEMPLET_SYSTEM
    {
        int iBuffID = 0;
	    LUA_GET_VALUE(		luaManager,	"m_Buff_ID",		iBuffID,	0 );
        kTemplet.SetBuffID( iBuffID );
    }
//#endif BUFF_TEMPLET_SYSTEM


    {
#ifdef  X2OPTIMIZE_SCRIPT_PREPROCESSING_TOOL
        int eNPCID = 0;
	    LUA_GET_VALUE( luaManager, "m_Summon_Npc",		eNPCID,  0 );
        kTemplet.SetSummonNpcID( eNPCID );

        if ( false == ( eNPCID >= 0 && eNPCID < 65536 ) )
        {
            sprintf_s( aszError, ARRAY_SIZE( aszError ), "ITEMID_%d: NPC_UNIT_ID_%d", dwItemID, eNPCID );
            ErrorLogMsg( XEM_ERROR144, aszError );
        }//if

#else   X2OPTIMIZE_SCRIPT_PREPROCESSING_TOOL
//#ifdef  ADD_TRAININGGAME_NPC
        CX2UnitManager::NPC_UNIT_ID eNPCID = CX2UnitManager::NUI_NONE;
        BOOST_STATIC_ASSERT( CX2UnitManager::NUI_NONE == 0 );
	    LUA_GET_VALUE_ENUM( luaManager, "m_Summon_Npc",		eNPCID, CX2UnitManager::NPC_UNIT_ID, CX2UnitManager::NUI_NONE );
        kTemplet.SetSummonNpcID( eNPCID );

        if ( false == ( eNPCID >= CX2UnitManager::NPC_UNIT_ID(0) && eNPCID < CX2UnitManager::NUI_MAX_LIMIT ) )
        {
            sprintf_s( aszError, ARRAY_SIZE( aszError ), "ITEMID_%d: NPC_UNIT_ID_%d", dwItemID, eNPCID );
            ErrorLogMsg( XEM_ERROR144, aszError );
        }//if

//#endif  ADD_TRAININGGAME_NPC
#endif  X2OPTIMIZE_SCRIPT_PREPROCESSING_TOOL
    }


    if ( dwNumExtra != 0 )
    {
        m_kFileSerializer.AlignSpace( sizeof(DWORD) );
        kTemplet.m_dwOffset_Extra = m_kFileSerializer.AppendData( sizeof(CX2Item::KItemFormatType_OffsetOrValue) * dwNumExtra, apExtra.get() );
    }//

    m_kFileSerializer.CopyData( dwItemOffset, sizeof(kTemplet), &kTemplet );

    if ( kTemplet.GetSetID() != 0 )
    {
		KSetItemDataMap::iterator iter = m_mapSetItemDataMap.find( kTemplet.GetSetID() );
		if ( iter == m_mapSetItemDataMap.end() )
            iter = m_mapSetItemDataMap.insert( KSetItemDataMap::value_type( kTemplet.GetSetID(), KSetItemData() ) ).first;
		iter->second.m_vecSetItemIDList.push_back( kTemplet.GetItemID() );
        int iLevel = kTemplet.GetUseLevel();
        if ( 0 < iLevel )
        {
            if ( iter->second.m_iMaxLevel < iLevel )
                iter->second.m_iMaxLevel = iLevel;
        }
    }//if

    return true;
}


bool CX2ItemManager::TokenizeByScaleRotate( const std::string& strTokenInfo, D3DXVECTOR3& vOutScale, D3DXVECTOR3& vOutRotate )
{
	//## 문자열은 항상 EX.) SX:1.0 SY:1.0 SZ:1.0 RX:1.0 RY:1.0 RZ:1.0 으로 와야함.

    vOutScale.x = vOutScale.y = vOutScale.z = 1.f;
    vOutRotate.x = vOutRotate.y = vOutRotate.z = 0.f;

	if( strTokenInfo.size() < 19 )
	{
		return false;
	}

	//## 0. Scale X
	//## 1. Scale Y
	//## 2. Scale Z
	//## 3. Rotate X
	//## 4. Rotate Y
	//## 5. Rotate Z
	std::string strToken[6];
	std::string strTemp;

	const char* pstr = strTokenInfo.c_str();

	while( (*pstr) != 0 )
	{
		switch( (*pstr) )
		{
		case 'R':
		case 'S':
		case 'X':
		case 'Y':
		case 'Z':
			{
				strTemp += (*pstr);
			}
			break;

		case ' ':
			{
				strTemp.clear();
			}
			break;

		default:
			{
				if( ((*pstr) >= '0' && (*pstr) <= '9') || (*pstr) == '.' || (*pstr) == '-' )
				{
					if( strTemp.compare("SX") == 0 )
						strToken[0] += (*pstr);
					else if( strTemp.compare("SY") == 0 )
						strToken[1] += (*pstr);
					else if( strTemp.compare("SZ") == 0 )
						strToken[2] += (*pstr);
					else if( strTemp.compare("RX") == 0 )
						strToken[3] += (*pstr);
					else if( strTemp.compare("RY") == 0 )
						strToken[4] += (*pstr);
					else if( strTemp.compare("RZ") == 0 )
						strToken[5] += (*pstr);
				}
			}
		}

		++pstr;
	}

	if( strToken[0].size() > 0 )
		vOutScale.x = static_cast<float>(atof( strToken[0].c_str() ));
	if( strToken[1].size() > 0 )
		vOutScale.y = static_cast<float>(atof( strToken[1].c_str() ));
	if( strToken[2].size() > 0 )
		vOutScale.z = static_cast<float>(atof( strToken[2].c_str() ));

	if( strToken[3].size() > 0 )
		vOutRotate.x = static_cast<float>((atof( strToken[3].c_str() ) / 180.0f) * D3DX_PI);
	if( strToken[4].size() > 0 )
		vOutRotate.y = static_cast<float>((atof( strToken[4].c_str() ) / 180.0f) * D3DX_PI);
	if( strToken[5].size() > 0 )
		vOutRotate.z = static_cast<float>((atof( strToken[5].c_str() ) / 180.0f) * D3DX_PI);

	return true;
}


const CX2Item::KItemFormatSetItemData*  CX2ItemManager::GetSetItem( DWORD dwSetID ) const
{
    if ( dwSetID == 0 || m_vecItemTempletInfo.size() < sizeof(CX2Item::KItemFormatHeader) )
        return NULL;

    const BYTE* pData = (const BYTE*) &m_vecItemTempletInfo.front();
    const CX2Item::KItemFormatHeader* pkHeader = (const CX2Item::KItemFormatHeader*) pData;
    ASSERT( pkHeader );
    if ( pkHeader == NULL )
        return NULL;
    DWORD dwNumSetIDs = pkHeader->m_dwNumSetIDs;
    DWORD   dwOffset =  sizeof(CX2Item::KItemFormatHeader) + 
        pkHeader->m_dwNumItems * ( sizeof(DWORD) + sizeof(CX2Item::KItemFormatTemplet) );

    const DWORD* pdwSetID = (const DWORD*) ( pData + dwOffset );
    ASSERT( pdwSetID );
    if ( pdwSetID == NULL )
        return NULL;

    const DWORD* pdwFound = std::lower_bound( &pdwSetID[0], &pdwSetID[dwNumSetIDs], dwSetID );
    if ( pdwFound == &pdwSetID[dwNumSetIDs] || *pdwFound != dwSetID )
    {
        ASSERT( 0 );
        return NULL;
    }//if
    DWORD dwIndex = pdwFound - pdwSetID;
    dwOffset += dwNumSetIDs * sizeof(DWORD) + dwIndex * sizeof(CX2Item::KItemFormatSetItemData);

    return  (const CX2Item::KItemFormatSetItemData*) ( pData + dwOffset );
}

int CX2ItemManager::GetSetItemOptions( DWORD dwSetID, const int iNumOfEquippedItems_, IN vector< int >& vecOptions ) const
{
    const CX2Item::KItemFormatSetItemData* pSetItemData = GetSetItem( dwSetID );
    if ( pSetItemData == NULL )
        return 0;

    pSetItemData->GetSetItemOptions( iNumOfEquippedItems_, vecOptions );

    return  pSetItemData->m_iMaxLevel;
}

#endif  //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING