#pragma once

#ifdef  X2OPTIMIZE_TET_XET_PREPROCESSING

#define KTET_FORMAT_MAGIC           (MAKEFOURCC('T','E','T',' '))
#define KTET_FORMAT_VERSION         0x0100

#pragma pack( push, 1 )

struct	KTETFormatHeader
{
	DWORD				m_dwMagic;
	DWORD				m_dwVersion;
	DWORD				m_dwNumTextureUV;

    KTETFormatHeader()
    {
        m_dwMagic = 0;
        m_dwVersion = 0;
        m_dwNumTextureUV = 0;
    }
};//

struct KTETFormatTextureUV
{
	D3DXVECTOR2 leftTop;
	D3DXVECTOR2	rightBottom;
	D3DXVECTOR2 leftBottom;
	D3DXVECTOR2 rightTop;
    D3DXVECTOR2 rectSize;

	KTETFormatTextureUV()
        : leftTop( 0, 0 )
        , rightBottom( 0, 0 )
        , leftBottom( -1, -1 )
        , rightTop( -1, -1 )
        , rectSize( 0, 0 )
	{
	}
};

#pragma pack( pop )

class   CKTDXDeviceTET_Preprocessing
{
public:

    CKTDXDeviceTET_Preprocessing();

    void                        Release() { m_vecData.resize( 0 ); }
    void                        Swap( CKTDXDeviceTET_Preprocessing& rhs_ ) { m_vecData.swap( rhs_.m_vecData ); }
    void                        Swap( std::vector<BYTE>& vecData ) { m_vecData.swap( vecData ); }
    bool	                    OpenFile( const BYTE* pbFileData, int iFileSize, const std::wstring& wstrDeviceID );

    const wchar_t*              GetString( DWORD dwOffset ) const;

    unsigned                    GetNumTexUVs() const;
    const wchar_t*              GetKeyFromIndex( unsigned uIndex ) const;
    const KTETFormatTextureUV*  GetTexUVFromIndex( unsigned uIndex ) const;
    const KTETFormatTextureUV*  GetTexUV( const wchar_t* pwszKey ) const;
    const KTETFormatTextureUV*  GetTexUV( const std::wstring& wstrKey ) const { return GetTexUV( wstrKey.c_str() );  }

    static void                 RegisterLuabind( lua_State* L );


private:

    typedef std::map<std::wstring, KTETFormatTextureUV> KeyTexUVMap;
    struct  TETProxy
    {
        TETProxy( const std::wstring& wstrDeviceID )
            : m_DeviceID( wstrDeviceID ) {}

		void			AddRect_LUA( const char* pKey, const D3DXVECTOR2& leftTop, const D3DXVECTOR2& rightTop, const D3DXVECTOR2& leftBottom, const D3DXVECTOR2& rightBottom );
		void			RotateRect_LUA( const char* pKey, int roateValue );
        void            PostProcess( std::vector<BYTE>& vecOut );

        std::wstring    m_DeviceID;
        KeyTexUVMap     m_mapTexUVRect;
    };

    std::vector<BYTE>   m_vecData;
};//

#endif  X2OPTIMIZE_TET_XET_PREPROCESSING