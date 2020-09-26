#pragma once

#ifdef  X2OPTIMIZE_TET_XET_PREPROCESSING

#define KXET_FORMAT_MAGIC           (MAKEFOURCC('X','E','T',' '))
#define KXET_FORMAT_VERSION         0x0100

#pragma pack( push, 1 )


struct  KXETFormatChangeTexture
{
    DWORD               m_dwOffset_OrgTexName;
    DWORD               m_dwOffset_ChangeTexName;

    KXETFormatChangeTexture()
    {
        m_dwOffset_OrgTexName = 0;
        m_dwOffset_ChangeTexName = 0;
    }
};//
    
struct  KXETFormatMultiStage
{
    DWORD               m_dwOffset_BlendTexName;
    DWORD               m_dwColorOP;

    KXETFormatMultiStage()
    {
        m_dwOffset_BlendTexName = 0;
        m_dwColorOP = 0;
    }
};//

struct  KXETFormatAniData
{
    DWORD               m_dwOffset_AniName;
    float               m_fTransitionTime;
    float               m_fAniMaxTime;
    WORD                m_wNumAniEventTexChange;
    WORD                m_wIndexAniEventTexChange;


    KXETFormatAniData()
    {
        m_dwOffset_AniName = 0;
        m_fTransitionTime = 0.f;
        m_fAniMaxTime = 0.f;
        m_wNumAniEventTexChange = 0;
        m_wIndexAniEventTexChange = 0;
    }
};//

struct  KXETFormatAniTexChangeEvent
{
    DWORD               m_dwOffset_OrgTexName;
    DWORD               m_dwOffset_ChangeTexName;
    float               m_fEventStart;
    float               m_fEventEnd;

    KXETFormatAniTexChangeEvent()
    {
        m_dwOffset_OrgTexName = 0;
        m_dwOffset_ChangeTexName = 0;
        m_fEventStart = 0.f;
        m_fEventEnd = 0.f;
    }
};//

struct	KXETFormatHeader
{
	DWORD				    m_dwMagic;
	DWORD				    m_dwVersion;
    DWORD                   m_dwNumChangeTexture;
    DWORD                   m_dwNumAniData;
    DWORD                   m_dwNumAniEventTexChange;
    KXETFormatMultiStage    m_kMultiStage1;
    KXETFormatMultiStage    m_kMultiStage2;

    KXETFormatHeader()
    {
        m_dwMagic = KXET_FORMAT_MAGIC;
        m_dwVersion = KXET_FORMAT_VERSION;
        m_dwNumChangeTexture = 0;
        m_dwNumAniData = 0;
        m_dwNumAniEventTexChange = 0;
    }
    DWORD                   GetChangeTextureOffset() const  { return sizeof(KXETFormatHeader); }
    DWORD                   GetAniDataOffset() const        { return GetChangeTextureOffset() + m_dwNumChangeTexture * sizeof(KXETFormatChangeTexture); }
    DWORD                   GetAniEventTexChangeOffset() const { return GetAniDataOffset() + m_dwNumAniData * sizeof(KXETFormatAniData); }
    DWORD                   GetFixedPartSize() const        { return GetAniEventTexChangeOffset() + m_dwNumAniEventTexChange * sizeof(KXETFormatAniTexChangeEvent ); }
};//


#pragma pack( pop )


class   CKTDXDeviceXET_Preprocessing
{
public:

    void                        Release() { m_vecData.resize( 0 ); }
    void                        Swap( CKTDXDeviceXET_Preprocessing& rhs_ ) { m_vecData.swap( rhs_.m_vecData ); }
    void                        Swap( std::vector<BYTE>& vecData ) { m_vecData.swap( vecData ); }
    bool	                    OpenFile( const wchar_t* pwszFileName, int& iSize );

    int                         FindChangeTextureIndex( const wchar_t* pwszTexName ) const;
    int                         FindAniDataIndex( const wchar_t* pwszAniName ) const;
    int                         FindAniTexChangeEventIndex( const KXETFormatAniData& kAniData, float fAniTime, const wchar_t* pwszTexName ) const;

    const KXETFormatHeader&     GetHeader() const;
    const wchar_t*              GetString( DWORD dwOffset ) const;

    static void                 RegisterLuabind( lua_State* L );

private:

    std::vector<BYTE>           m_vecData;

    struct  XETProxy
    {

        struct  MultiTextureData
        {
            std::wstring                m_wstrBlendTexName;
            D3DTEXTUREOP                m_d3dColorOP;
        };//struct  MultiTextureData

        struct  AniTexChangeEventData
        {
            std::wstring                m_wstrChangeTexName;
            float                       m_fEventStart;
            float                       m_fEventEnd;
        };

        struct  AniData
        {
            float                       m_fTransitionTime;
            float                       m_fAniMaxTime;
            typedef std::multimap<std::wstring,AniTexChangeEventData>   AniTexChangeEventDataMultimap;
            AniTexChangeEventDataMultimap   m_TexChangeEventMap;
		    void SetTexChangeEvent_LUA( const D3DXVECTOR2& eventTime, const char* pOrgTexName, const char* pChangeTexName );
        };

        XETProxy( const std::wstring& wstrDeviceID )
            : m_DeviceID( wstrDeviceID ) {}

	    void    AddChangeTexture_LUA( const char* pOrgTexName, const char* pChangeTexName );
	    void    SetMultiTexStage1_LUA( const char* pOrgTexName, const char* pBlendTexName, int colorOP );
	    void    SetMultiTexStage2_LUA( const char* pOrgTexName, const char* pBlendTexName, int colorOP );
	    const   AniData* AddAniData_LUA( const char* pAniName, float fTransitionTime = 0.0f, float	fAniMaxTime = 0.0f );
	    const   AniData* AddAniDataByFrame_LUA( const char* pAniName, int transitionFrame = 0, float	fAniMaxTime = 0.0f );

        void    PostProcess( std::vector<BYTE>& vecOut );

    public:

        typedef std::map<std::wstring,std::wstring> ChangeTextureMap;
        typedef std::map<std::wstring,AniData>  NameAniDataMap;

        std::wstring                m_DeviceID;
        ChangeTextureMap            m_ChangeTextureMap;
        MultiTextureData			m_MultiTexStage1;
        MultiTextureData			m_MultiTexStage2;
        NameAniDataMap              m_AniDataMap;
  
    };//

};

#endif  X2OPTIMIZE_TET_XET_PREPROCESSING