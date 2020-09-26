#include <StdAfx.h>

#ifdef  X2OPTIMIZE_TET_XET_PREPROCESSING

#include <boost/format.hpp>

const static KXETFormatHeader   s_kDummyHeader;

/*static*/
void CKTDXDeviceXET_Preprocessing::RegisterLuabind( lua_State* L )
{
	lua_tinker::class_add<CKTDXDeviceXET_Preprocessing::XETProxy>( L, "CKTDXDeviceXET" );
	lua_tinker::class_def<CKTDXDeviceXET_Preprocessing::XETProxy>( L, "AddChangeTexture",				&CKTDXDeviceXET_Preprocessing::XETProxy::AddChangeTexture_LUA );
	lua_tinker::class_def<CKTDXDeviceXET_Preprocessing::XETProxy>( L, "SetMultiTexStage1",			&CKTDXDeviceXET_Preprocessing::XETProxy::SetMultiTexStage1_LUA );
	lua_tinker::class_def<CKTDXDeviceXET_Preprocessing::XETProxy>( L, "SetMultiTexStage2",			&CKTDXDeviceXET_Preprocessing::XETProxy::SetMultiTexStage2_LUA );
	lua_tinker::class_def<CKTDXDeviceXET_Preprocessing::XETProxy>( L, "AddAniData",					&CKTDXDeviceXET_Preprocessing::XETProxy::AddAniData_LUA );
	lua_tinker::class_def<CKTDXDeviceXET_Preprocessing::XETProxy>( L, "AddAniDataByFrame",			&CKTDXDeviceXET_Preprocessing::XETProxy::AddAniDataByFrame_LUA );
	//lua_tinker::class_def<CKTDXDeviceXET_Preprocessing>( L, "CreateCollisionData",			&CKTDXDeviceXET_Preprocessing::CreateCollisionData_LUA );

	lua_tinker::class_add<CKTDXDeviceXET_Preprocessing::XETProxy::AniData>( L, "AniData" );
	lua_tinker::class_def<CKTDXDeviceXET_Preprocessing::XETProxy::AniData>( L, "SetTexChangeEvent",		&CKTDXDeviceXET_Preprocessing::XETProxy::AniData::SetTexChangeEvent_LUA );
}


int CKTDXDeviceXET_Preprocessing::FindChangeTextureIndex( const wchar_t* pwszTexName ) const
{
    if ( pwszTexName == NULL || pwszTexName[0] == NULL )
        return -1;
    if ( m_vecData.size() < sizeof(KXETFormatHeader) )
        return -1;

    const BYTE* pData = &m_vecData.front();
    const KXETFormatHeader& kHeader = * (const KXETFormatHeader*)( pData );
    const KXETFormatChangeTexture* pChangeTextureArray = (const KXETFormatChangeTexture*) ( pData + kHeader.GetChangeTextureOffset() );
    const KXETFormatChangeTexture* pChangeTextureArrayEnd = pChangeTextureArray + kHeader.m_dwNumChangeTexture;

    const CKTDXDeviceXET_Preprocessing* pThis = this;
    auto StringLessCB = [pThis]( const KXETFormatChangeTexture& kChangeTexture, const wchar_t* pwszTexName ) -> bool
    {
        return  _wcsicmp( pThis->GetString( kChangeTexture.m_dwOffset_OrgTexName ), pwszTexName ) < 0;
    };
    const KXETFormatChangeTexture* pChangeTextureFound = std::lower_bound( pChangeTextureArray, pChangeTextureArrayEnd, pwszTexName, StringLessCB );
    if ( pChangeTextureFound == pChangeTextureArrayEnd
        || _wcsicmp( pwszTexName, GetString( pChangeTextureFound->m_dwOffset_OrgTexName ) ) < 0 )
        return -1;
    return (int) ( pChangeTextureFound - pChangeTextureArray );
}


int CKTDXDeviceXET_Preprocessing::FindAniDataIndex( const wchar_t* pwszAniName ) const
{
    if ( pwszAniName == NULL || pwszAniName[0] == NULL )
        return -1;
    if ( m_vecData.size() < sizeof(KXETFormatHeader) )
        return -1;

    const BYTE* pData = &m_vecData.front();
    const KXETFormatHeader& kHeader = * (const KXETFormatHeader*)( pData );
    const KXETFormatAniData* pAniDataArray = (const KXETFormatAniData*) ( pData + kHeader.GetAniDataOffset() );
    const KXETFormatAniData* pAniDataArrayEnd = pAniDataArray + kHeader.m_dwNumAniData;

    const CKTDXDeviceXET_Preprocessing* pThis = this;
    auto StringLessCB = [pThis]( const KXETFormatAniData& kAniData, const wchar_t* pwszAniName ) -> bool
    {
        return  _wcsicmp( pThis->GetString( kAniData.m_dwOffset_AniName ), pwszAniName ) < 0;
    };
    const KXETFormatAniData* pAniDataArrayFound = std::lower_bound( pAniDataArray, pAniDataArrayEnd, pwszAniName, StringLessCB );
    if ( pAniDataArrayFound == pAniDataArrayEnd
        || _wcsicmp( pwszAniName, GetString( pAniDataArrayFound->m_dwOffset_AniName ) ) < 0 )
        return -1;
    return (int) ( pAniDataArrayFound - pAniDataArray );
}


int CKTDXDeviceXET_Preprocessing::FindAniTexChangeEventIndex( const KXETFormatAniData& kAniData, float fAniTime, const wchar_t* pwszTexName ) const
{
    if ( pwszTexName == NULL || pwszTexName[0] == NULL || kAniData.m_wNumAniEventTexChange == 0 )
        return -1;
    if ( m_vecData.size() < sizeof(KXETFormatHeader) )
        return -1;

    const BYTE* pData = &m_vecData.front();
    const KXETFormatHeader& kHeader = * (const KXETFormatHeader*)( pData );
    if ( (DWORD) kAniData.m_wIndexAniEventTexChange >= kHeader.m_dwNumAniEventTexChange 
        || (DWORD) kAniData.m_wIndexAniEventTexChange + (DWORD) kAniData.m_wNumAniEventTexChange > kHeader.m_dwNumAniEventTexChange  )
        return -1;
    const KXETFormatAniTexChangeEvent* pAniEventArrayBegin = (const KXETFormatAniTexChangeEvent*) ( pData + kHeader.GetAniEventTexChangeOffset() );
    const KXETFormatAniTexChangeEvent* pAniEventArray = pAniEventArrayBegin + kAniData.m_wIndexAniEventTexChange;
    const KXETFormatAniTexChangeEvent* pAniEventArrayEnd = pAniEventArray + kAniData.m_wNumAniEventTexChange;

    const CKTDXDeviceXET_Preprocessing* pThis = this;
    auto StringLessCB = [pThis]( const KXETFormatAniTexChangeEvent& kAniEvent, const wchar_t* pwszTexName ) -> bool
    {
        return  _wcsicmp( pThis->GetString( kAniEvent.m_dwOffset_OrgTexName ), pwszTexName ) < 0;
    };

    const KXETFormatAniTexChangeEvent* pAniEventArrayFound = std::lower_bound( pAniEventArray, pAniEventArrayEnd, pwszTexName, StringLessCB );
    if ( pAniEventArrayFound == pAniEventArrayEnd
        || _wcsicmp( pwszTexName, GetString( pAniEventArrayFound->m_dwOffset_OrgTexName ) ) < 0 )
        return -1;
    DWORD   dwOffsetTexName = pAniEventArrayFound->m_dwOffset_OrgTexName;
	for( ; pAniEventArrayFound != pAniEventArrayEnd && pAniEventArrayFound->m_dwOffset_OrgTexName == dwOffsetTexName; pAniEventArrayFound++ )
	{
        if( pAniEventArrayFound->m_fEventStart <= fAniTime
			&& pAniEventArrayFound->m_fEventEnd > fAniTime )
		{
			return (int) ( pAniEventArrayFound - pAniEventArrayBegin );
		}
	}
    return -1;
}


bool    CKTDXDeviceXET_Preprocessing::OpenFile( const wchar_t* pwszFileName, int& iSize )
{
    iSize = 0;
    m_vecData.resize( 0 );
    if ( pwszFileName == NULL || pwszFileName[0] == NULL )
        return false;

    const BYTE* pbFileData = NULL;
    int         iFileSize = 0;

#ifdef  X2OPTIMIZE_SCRIPT_PREPROCESSING_TOOL

    std::vector<BYTE> vecbFileData;
    {
        KFileSerializer kReadFile;
        HRESULT hr = kReadFile.LoadFile( pwszFileName );
        if ( FAILED( hr ) )
            return false;
        kReadFile.Swap( vecbFileData );
    }
    if ( vecbFileData.empty() == false )
    {
        pbFileData = &vecbFileData.front();
        iFileSize = vecbFileData.size();
    }

    {
        XETProxy proxy( pwszFileName );
        KLuaManager kLuaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState(), 0, true );
	    lua_tinker::decl( kLuaManager.GetLuaState(),  "g_pXET", &proxy );
	    if( kLuaManager.DoMemoryNotEncrypt( (const char*) pbFileData, iFileSize ) == false )
	    {
            return false;
	    }
        proxy.PostProcess( m_vecData );
        iSize = iFileSize;
    }

#else   X2OPTIMIZE_SCRIPT_PREPROCESSING_TOOL

	KGCMassFileManager::CMassFile::MASSFILE_MEMBERFILEINFO_POINTER info;
	info = g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->LoadDataFile( pwszFileName );
    if ( info == NULL || info->pRealData == NULL || info->size <= 0 )
    {
        return false;
    }

    pbFileData = (const BYTE*) info->pRealData;
    iFileSize = info->size;

    if ( iFileSize >= sizeof(KXETFormatHeader) )
    {
        const KXETFormatHeader* pkHeader = (const KXETFormatHeader*) ( pbFileData );
        if ( pkHeader->m_dwMagic == KXET_FORMAT_MAGIC && pkHeader->m_dwVersion == KXET_FORMAT_VERSION )
        {
            if ( iFileSize >= (int) pkHeader->GetFixedPartSize() )
            {
                m_vecData.assign( &pbFileData[0], &pbFileData[iFileSize] );
                iSize = iFileSize;
                return true;
            }
            else
            {
                return false;
            }
        }//if
    }//if

#ifndef X2OPTIMIZE_REMOVE_LUA_INTERPRETER_MODULE
    {
        XETProxy proxy( pwszFileName );
        KLuaManager kLuaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState(), 0, true );
	    lua_tinker::decl( kLuaManager.GetLuaState(),  "g_pXET", &proxy );
	    if( kLuaManager.DoMemoryNotEncrypt( (const char*) pbFileData, iFileSize ) == false )
	    {
            return false;
	    }
        proxy.PostProcess( m_vecData );
        iSize = iFileSize;
    }
#endif  X2OPTIMIZE_REMOVE_LUA_INTERPRETER_MODULE

#endif  X2OPTIMIZE_SCRIPT_PREPROCESSING_TOOL


    
    return true;
}


const KXETFormatHeader& CKTDXDeviceXET_Preprocessing::GetHeader() const
{
    if ( m_vecData.size() < sizeof(KXETFormatHeader) )
        return s_kDummyHeader;
    return * (const KXETFormatHeader*)( &m_vecData.front() );
}


const wchar_t*  CKTDXDeviceXET_Preprocessing::GetString( DWORD dwOffset ) const
{
    if ( dwOffset == 0 )
        return L"";
    ASSERT( dwOffset + sizeof(WORD) <= m_vecData.size() );
    if ( dwOffset + sizeof(WORD) > m_vecData.size() )
        return L"";
    const BYTE* pStringLength = ( (const BYTE*) &m_vecData.front() ) + dwOffset;
    WORD   wLength = * ( (const WORD*) pStringLength );
    if ( wLength == 0 )
        return L"";
    ASSERT( dwOffset + sizeof(WORD) + wLength + 1 <= m_vecData.size() );
    if ( dwOffset + sizeof(WORD) + wLength + 1 > m_vecData.size() )
        return L"";
    const wchar_t* pwszString = (const wchar_t*) ( pStringLength + sizeof(WORD) );
    ASSERT( pwszString[ wLength ] == 0 );
    if ( pwszString[ wLength ] != 0 )
        return L"";
    return pwszString;
}


void    CKTDXDeviceXET_Preprocessing::XETProxy::AddChangeTexture_LUA( const char* pOrgTexName, const char* pChangeTexName )
{
	if ( pOrgTexName == NULL || pOrgTexName[0] == NULL
        || pChangeTexName == NULL || pChangeTexName[0] == NULL )
    {
        ASSERT( 0 );
        return;
    }//if

	wstring wOrgTexName;	
	ConvertUtf8ToWCHAR( wOrgTexName, pOrgTexName );
	MakeUpperCase( wOrgTexName ); 
	
	wstring wChangeTexName;	
	ConvertUtf8ToWCHAR( wChangeTexName, pChangeTexName );
	MakeUpperCase( wChangeTexName ); 

    m_ChangeTextureMap[ wOrgTexName ] = wChangeTexName;
}

void    CKTDXDeviceXET_Preprocessing::XETProxy::SetMultiTexStage1_LUA( const char* , const char* pBlendTexName, int colorOP )
{
    if ( pBlendTexName == NULL || pBlendTexName[0] == NULL )
        return;
		
	ConvertUtf8ToWCHAR( m_MultiTexStage1.m_wstrBlendTexName, pBlendTexName );
	MakeUpperCase( m_MultiTexStage1.m_wstrBlendTexName ); 
	m_MultiTexStage1.m_d3dColorOP = (D3DTEXTUREOP)colorOP;
}

void    CKTDXDeviceXET_Preprocessing::XETProxy::SetMultiTexStage2_LUA( const char*, const char* pBlendTexName, int colorOP )
{
    if ( pBlendTexName == NULL || pBlendTexName[0] == NULL )
        return;
		
	ConvertUtf8ToWCHAR( m_MultiTexStage2.m_wstrBlendTexName, pBlendTexName );
	MakeUpperCase( m_MultiTexStage2.m_wstrBlendTexName ); 
	m_MultiTexStage2.m_d3dColorOP = (D3DTEXTUREOP)colorOP;
}

const CKTDXDeviceXET_Preprocessing::XETProxy::AniData* CKTDXDeviceXET_Preprocessing::XETProxy::AddAniData_LUA( const char* pAniName, float fTransitionTime, float fAniMaxTime )
{
	//{{ kimhc // 2011-03-16 // 메모리 릭 수정 코드
	if ( pAniName == NULL || pAniName[0] == NULL )
	{
		ASSERT( false );
		boost::format fmt("유효하지 않은 애니메이션 이름을 사용하였습니다: %1%, %2%");
		ErrorLogMsg( KEM_ERROR96, boost::str(fmt % m_DeviceID.c_str() % pAniName).c_str() );
		return NULL;
	}

	wstring wstrAniName;
	ConvertUtf8ToWCHAR( wstrAniName, pAniName );

    NameAniDataMap::_Pairib ib = m_AniDataMap.insert( NameAniDataMap::value_type( wstrAniName, AniData() ) );
    if ( ib.second == false )
    {
		ASSERT( false );
		boost::format fmt("이미 등록된 애니메이션 이름을 사용하였습니다: %1%, %2%");
		ErrorLogMsg( KEM_ERROR96, boost::str(fmt % m_DeviceID.c_str() % pAniName).c_str() );

		return &ib.first->second;
		//return NULL;
    }

    AniData& aniData = ib.first->second;
	aniData.m_fTransitionTime     = fTransitionTime;
	aniData.m_fAniMaxTime         = fAniMaxTime;

	return &aniData;
}

const CKTDXDeviceXET_Preprocessing::XETProxy::AniData* CKTDXDeviceXET_Preprocessing::XETProxy::AddAniDataByFrame_LUA( const char* pAniName, int transitionFrame, float	fAniMaxTime )
{
	float fTransitionTime = (float)((float)transitionFrame / 30.0f);
	return AddAniData_LUA( pAniName, fTransitionTime,fAniMaxTime );
}

void    CKTDXDeviceXET_Preprocessing::XETProxy::AniData::SetTexChangeEvent_LUA( const D3DXVECTOR2& eventTime, const char* pOrgTexName, const char* pChangeTexName )
{
	if ( pOrgTexName == NULL || pOrgTexName[0] == NULL
        || pChangeTexName == NULL || pChangeTexName[0] == NULL )
        return;

	wstring wOrgTexName;	
	ConvertUtf8ToWCHAR( wOrgTexName, pOrgTexName );
	MakeUpperCase( wOrgTexName );

    AniTexChangeEventDataMultimap::iterator iter = m_TexChangeEventMap.insert( AniTexChangeEventDataMultimap::value_type( wOrgTexName, AniTexChangeEventData() ) );
    ASSERT( iter != m_TexChangeEventMap.end() );
    AniTexChangeEventData& data = iter->second;

	ConvertUtf8ToWCHAR( data.m_wstrChangeTexName, pChangeTexName );
	MakeUpperCase( data.m_wstrChangeTexName );
    data.m_fEventStart = eventTime.x;
    data.m_fEventEnd = eventTime.y;
}

void    CKTDXDeviceXET_Preprocessing::XETProxy::PostProcess( std::vector<BYTE>& vecOut )
{
    KFileSerializer kFileSerializer;
    kFileSerializer.SetSaveStringEndNull( true );

    KXETFormatHeader    kHeader;
    kHeader.m_dwMagic = KXET_FORMAT_MAGIC;
    kHeader.m_dwVersion = KXET_FORMAT_VERSION;
    kHeader.m_dwNumChangeTexture = m_ChangeTextureMap.size();
    kHeader.m_dwNumAniData = m_AniDataMap.size();
    kHeader.m_dwNumAniEventTexChange = 0;
    BOOST_FOREACH( const NameAniDataMap::value_type& value, m_AniDataMap )
    {
        kHeader.m_dwNumAniEventTexChange += value.second.m_TexChangeEventMap.size();
    }

    DWORD   dwOffset = 0;
    kFileSerializer.Reserve( kHeader.GetFixedPartSize() );
    kFileSerializer.AppendSpace( kHeader.GetFixedPartSize() );
    dwOffset = kHeader.GetChangeTextureOffset();
    BOOST_FOREACH( const ChangeTextureMap::value_type& pair, m_ChangeTextureMap )
    {
        KXETFormatChangeTexture kChangeTexture;
        kChangeTexture.m_dwOffset_OrgTexName = kFileSerializer.AppendWideString( true, false, pair.first.c_str() );
        kChangeTexture.m_dwOffset_ChangeTexName = kFileSerializer.AppendWideString( true, false, pair.second.c_str() );
        kFileSerializer.CopyData( dwOffset, sizeof(KXETFormatChangeTexture), &kChangeTexture );
        dwOffset += sizeof(KXETFormatChangeTexture);
    }

    dwOffset = kHeader.GetAniDataOffset();
    DWORD   dwOffset_AniEventTexChange = kHeader.GetAniEventTexChangeOffset();
    WORD    wIndex = 0;
    BOOST_FOREACH( const NameAniDataMap::value_type& value, m_AniDataMap )
    {
        KXETFormatAniData   kAniData;
        kAniData.m_dwOffset_AniName = kFileSerializer.AppendWideString( true, false, value.first.c_str() );
        kAniData.m_fTransitionTime = value.second.m_fTransitionTime;
        kAniData.m_fAniMaxTime = value.second.m_fAniMaxTime;
        kAniData.m_wNumAniEventTexChange = value.second.m_TexChangeEventMap.size();
        kAniData.m_wIndexAniEventTexChange = wIndex;

        kFileSerializer.CopyData( dwOffset, sizeof(KXETFormatAniData), &kAniData );
        dwOffset += sizeof(KXETFormatAniData);
        BOOST_FOREACH( const AniData::AniTexChangeEventDataMultimap::value_type& value_event, value.second.m_TexChangeEventMap )
        {
            KXETFormatAniTexChangeEvent kEvent;
            kEvent.m_dwOffset_OrgTexName = kFileSerializer.AppendWideString( true, false, value_event.first.c_str() ); 
            kEvent.m_dwOffset_ChangeTexName = kFileSerializer.AppendWideString( true, false, value_event.second.m_wstrChangeTexName.c_str() );
            kEvent.m_fEventStart = value_event.second.m_fEventStart;
            kEvent.m_fEventEnd = value_event.second.m_fEventEnd;

            kFileSerializer.CopyData( dwOffset_AniEventTexChange, sizeof(KXETFormatAniTexChangeEvent), &kEvent );
            dwOffset_AniEventTexChange += sizeof(KXETFormatAniTexChangeEvent);
            wIndex++;
        }
    }
    if ( m_MultiTexStage1.m_wstrBlendTexName.empty() == false )
    {
        kHeader.m_kMultiStage1.m_dwOffset_BlendTexName = kFileSerializer.AppendWideString( true, false, m_MultiTexStage1.m_wstrBlendTexName.c_str() );
        kHeader.m_kMultiStage1.m_dwColorOP = (DWORD) m_MultiTexStage1.m_d3dColorOP;
    }
    if ( m_MultiTexStage2.m_wstrBlendTexName.empty() == false )
    {
        kHeader.m_kMultiStage2.m_dwOffset_BlendTexName = kFileSerializer.AppendWideString( true, false, m_MultiTexStage2.m_wstrBlendTexName.c_str() );
        kHeader.m_kMultiStage2.m_dwColorOP = (DWORD) m_MultiTexStage2.m_d3dColorOP;
    }
    kFileSerializer.CopyData( 0, sizeof(KXETFormatHeader), &kHeader );

    kFileSerializer.Swap( vecOut );
}

#endif  X2OPTIMIZE_TET_XET_PREPROCESSING