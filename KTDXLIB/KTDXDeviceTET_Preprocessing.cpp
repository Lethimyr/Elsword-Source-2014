#include <StdAfx.h>

#ifdef  X2OPTIMIZE_TET_XET_PREPROCESSING


CKTDXDeviceTET_Preprocessing::CKTDXDeviceTET_Preprocessing()
{
}

/*static*/
void    CKTDXDeviceTET_Preprocessing::RegisterLuabind( lua_State* L )
{
	lua_tinker::class_add<CKTDXDeviceTET_Preprocessing::TETProxy>( L, "TETProxy" );
	lua_tinker::class_def<CKTDXDeviceTET_Preprocessing::TETProxy>( L, "AddRect",					&CKTDXDeviceTET_Preprocessing::TETProxy::AddRect_LUA );
	lua_tinker::class_def<CKTDXDeviceTET_Preprocessing::TETProxy>( L, "RotateRect",				&CKTDXDeviceTET_Preprocessing::TETProxy::RotateRect_LUA );
}


bool CKTDXDeviceTET_Preprocessing::OpenFile( const BYTE* pbFileData, int iFileSize,  const std::wstring& wstrDeviceID )
{
    m_vecData.resize( 0 );
    if ( pbFileData == NULL || iFileSize <= 0 )
        return false;

#ifndef X2OPTIMIZE_SCRIPT_PREPROCESSING_TOOL

    if ( iFileSize >= sizeof(KTETFormatHeader) )
    {
        KTETFormatHeader kHeader = *( (const KTETFormatHeader*) ( pbFileData ) );
        if ( kHeader.m_dwMagic == KTET_FORMAT_MAGIC && kHeader.m_dwVersion == KTET_FORMAT_VERSION )
        {
            if ( iFileSize >= (int) ( sizeof(KTETFormatHeader) + kHeader.m_dwNumTextureUV * ( sizeof(DWORD) + sizeof(KTETFormatTextureUV) ) ) )
            {
                m_vecData.assign( &pbFileData[0], &pbFileData[iFileSize] );
                return true;
            }
            else
            {
                return false;
            }
        }//if
    }//if

#endif  X2OPTIMIZE_SCRIPT_PREPROCESSING_TOOL


#ifndef  X2OPTIMIZE_REMOVE_LUA_INTERPRETER_MODULE

    {
        TETProxy proxy( wstrDeviceID );
        KLuaManager kLuaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState(), 0, true );
	    lua_tinker::decl( kLuaManager.GetLuaState(), "m_pTEX", &proxy );
	    if( kLuaManager.DoMemoryNotEncrypt( (const char*) pbFileData, iFileSize ) == false )
	    {
            return false;
	    }

        proxy.PostProcess( m_vecData );
        return true;
    }

#endif  X2OPTIMIZE_REMOVE_LUA_INTERPRETER_MODULE

    
    return false;

}

const wchar_t*              CKTDXDeviceTET_Preprocessing::GetString( DWORD dwOffset ) const
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

unsigned                    CKTDXDeviceTET_Preprocessing::GetNumTexUVs() const
{
    if ( m_vecData.size() < sizeof(KTETFormatHeader) )
        return 0;
    const BYTE* pData = &m_vecData.front();
    const KTETFormatHeader& kHeader = * (const KTETFormatHeader*)( pData );
    return kHeader.m_dwNumTextureUV;
}

const wchar_t*              CKTDXDeviceTET_Preprocessing::GetKeyFromIndex( unsigned uIndex ) const
{
    if ( m_vecData.size() >= sizeof(KTETFormatHeader) )
    {
        const BYTE* pData = &m_vecData.front();
        const KTETFormatHeader& kHeader = * (const KTETFormatHeader*)( pData );
        if ( uIndex < kHeader.m_dwNumTextureUV )
        {
            DWORD dwOffset = * ( (const DWORD*) ( pData + sizeof(KTETFormatHeader) + sizeof(DWORD) * uIndex ) );
            return GetString( dwOffset );
        }
    }
    return L"";
}

const KTETFormatTextureUV*  CKTDXDeviceTET_Preprocessing::GetTexUVFromIndex( unsigned uIndex ) const
{
    if ( m_vecData.size() >= sizeof(KTETFormatHeader) )
    {
        const BYTE* pData = &m_vecData.front();
        const KTETFormatHeader& kHeader = * (const KTETFormatHeader*)( pData );    
        if ( uIndex < kHeader.m_dwNumTextureUV )
        {
            return ( const KTETFormatTextureUV* ) ( pData + sizeof(KTETFormatHeader) + kHeader.m_dwNumTextureUV * sizeof(DWORD)
                + uIndex * sizeof(KTETFormatTextureUV) );
        }
    }
    return NULL;
}


const KTETFormatTextureUV*  CKTDXDeviceTET_Preprocessing::GetTexUV( const wchar_t* pwszKey ) const
{
    if ( pwszKey == NULL || pwszKey[0] == NULL )
        return NULL;
    if ( m_vecData.size() < sizeof(KTETFormatHeader) )
        return NULL;
    const BYTE* pData = &m_vecData.front();
    const KTETFormatHeader& kHeader = * (const KTETFormatHeader*)( pData );
    const DWORD* pKeyArray = (const DWORD*) ( pData + sizeof(KTETFormatHeader) );
    const DWORD* pKeyArrayEnd = pKeyArray + kHeader.m_dwNumTextureUV;

    const CKTDXDeviceTET_Preprocessing* pThis = this;
    auto StringLessCB = [pThis]( DWORD dwOffset, const wchar_t* pwszKey ) -> bool
    {
        return  wcscmp( pThis->GetString( dwOffset ), pwszKey ) < 0;
    };
    const DWORD* pKeyFound = std::lower_bound( pKeyArray, pKeyArrayEnd, pwszKey, StringLessCB );
    if ( pKeyFound == pKeyArrayEnd || wcscmp( pwszKey, pThis->GetString( *pKeyFound ) ) < 0 )
        return NULL;
    unsigned uIndex = (unsigned) ( pKeyFound - pKeyArray );
    return ( const KTETFormatTextureUV* ) ( pData + sizeof(KTETFormatHeader) + kHeader.m_dwNumTextureUV * sizeof(DWORD)
        + uIndex * sizeof(KTETFormatTextureUV) );
}

void    CKTDXDeviceTET_Preprocessing::TETProxy::PostProcess( std::vector<BYTE>& vecOut )
{
    KFileSerializer kFileSerializer;
    kFileSerializer.SetSaveStringEndNull( true );

    KTETFormatHeader    kHeader;
    kHeader.m_dwMagic = KTET_FORMAT_MAGIC;
    kHeader.m_dwVersion = KTET_FORMAT_VERSION;
    kHeader.m_dwNumTextureUV = m_mapTexUVRect.size();

    DWORD   dwReserveSize = sizeof(KTETFormatHeader) + kHeader.m_dwNumTextureUV*( sizeof(DWORD) + sizeof(KTETFormatTextureUV) );
    kFileSerializer.AppendSpace( dwReserveSize );
    DWORD   dwOffset = 0;
    kFileSerializer.CopyData( dwOffset, sizeof(KTETFormatHeader), &kHeader );
    dwOffset += sizeof(KTETFormatHeader);
    for( KeyTexUVMap::const_iterator iter = m_mapTexUVRect.begin(); iter != m_mapTexUVRect.end(); ++iter )
    {
        DWORD   dwKeyOffset = kFileSerializer.AppendWideString( true, true, iter->first );
        kFileSerializer.CopyData( dwOffset, sizeof(DWORD), &dwKeyOffset );
        dwOffset += sizeof(DWORD);
    }
    for( KeyTexUVMap::const_iterator iter = m_mapTexUVRect.begin(); iter != m_mapTexUVRect.end(); ++iter )
    {
        kFileSerializer.CopyData( dwOffset, sizeof(KTETFormatTextureUV), &iter->second );
        dwOffset += sizeof(KTETFormatTextureUV);
    }
    kFileSerializer.Swap( vecOut );
}


void    CKTDXDeviceTET_Preprocessing::TETProxy::AddRect_LUA( const char* pKey, const D3DXVECTOR2& leftTop, const D3DXVECTOR2& rightTop, const D3DXVECTOR2& leftBottom, const D3DXVECTOR2& rightBottom )
{
	if ( pKey == NULL || pKey[0] == NULL )
		return;

	wstring wstrKeyName;
	ConvertUtf8ToWCHAR( wstrKeyName, pKey );
	MakeUpperCase(wstrKeyName);

    KeyTexUVMap::_Pairib ib = m_mapTexUVRect.insert( KeyTexUVMap::value_type( wstrKeyName, KTETFormatTextureUV() ) );
	if ( ib.second == false )
	{
		WCHAR buff[256];
		StringCchPrintf( buff, ARRAY_SIZE( buff ), L"redundant key : %s\nKey : %s", m_DeviceID.c_str(), wstrKeyName.c_str() );
		ErrorLogMsg( KEM_ERROR95, buff );
		ASSERT(false);
		return;
	}

	KTETFormatTextureUV& texUV = ib.first->second;
	texUV.leftTop	= leftTop;
	texUV.rightBottom = rightBottom;
		
	if ( leftBottom.x == -1  && leftBottom.y == -1 )
	{
		texUV.leftBottom.x = leftTop.x;
		texUV.leftBottom.y = rightBottom.y;
	}
	else
	{
		texUV.leftBottom = leftBottom;
	}

	if ( rightTop.x == -1 && rightTop.y == -1 )
	{
		texUV.rightTop.x = rightBottom.x;
		texUV.rightTop.y = leftTop.y;
	}
	else
	{
		texUV.rightTop = rightTop;
	}

	texUV.rectSize.x = fabs( texUV.leftTop.x - texUV.rightTop.x );
	texUV.rectSize.y = fabs( texUV.leftTop.y - texUV.leftBottom.y );
}


void    CKTDXDeviceTET_Preprocessing::TETProxy::RotateRect_LUA( const char* pKey, int roateValue )
{
//#ifdef DYNAMIC_VERTEX_BUFFER_OPT
	if ( pKey == NULL || pKey[0] == NULL )
		return;
//#endif

	wstring wstrKeyName;
	ConvertUtf8ToWCHAR( wstrKeyName, pKey );
	MakeUpperCase(wstrKeyName);
	
	KeyTexUVMap::iterator iter = m_mapTexUVRect.find( wstrKeyName );
	if ( iter == m_mapTexUVRect.end() )
        return;

	KTETFormatTextureUV& texUV = iter->second;

	switch ( roateValue )
	{
	case 90:
		{
			D3DXVECTOR2 tempPoint = texUV.leftTop;
			D3DXVECTOR2 tempPoint2 = texUV.rightTop;

			texUV.leftTop = texUV.leftBottom;
			texUV.leftBottom = texUV.rightBottom;
			texUV.rightTop = tempPoint;
			texUV.rightBottom = tempPoint2;

			float tempValue;
			tempValue = texUV.rectSize.y;
			texUV.rectSize.y = texUV.rectSize.x;
			texUV.rectSize.x = tempValue;

		}
		break;

	case 180:
		{
			D3DXVECTOR2 tempPoint = texUV.leftTop;
			D3DXVECTOR2 tempPoint2 = texUV.leftBottom;

			texUV.leftTop = texUV.rightBottom;
			texUV.leftBottom = texUV.rightTop;
			texUV.rightTop = tempPoint2;
			texUV.rightBottom = tempPoint;
		}
		break;

	case 270:
		{
			D3DXVECTOR2 tempPoint = texUV.leftTop;
			D3DXVECTOR2 tempPoint2 = texUV.leftBottom;

			texUV.leftTop = texUV.rightTop;
			texUV.leftBottom = tempPoint;
			texUV.rightTop = texUV.rightBottom;
			texUV.rightBottom = tempPoint2;

			float tempValue;
			tempValue = texUV.rectSize.y;
			texUV.rectSize.y = texUV.rectSize.x;
			texUV.rectSize.x = tempValue;
		}
		break;
	}
}

#endif  X2OPTIMIZE_TET_XET_PREPROCESSING