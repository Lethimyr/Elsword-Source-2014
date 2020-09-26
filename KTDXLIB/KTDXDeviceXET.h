#pragma once


class   CKTDXDeviceTexture;

class CKTDXDeviceXET : public CKTDXDevice
{
public:

	struct MultiTexureData
	{
		//wstring					orgTexName;
		CKTDXDeviceTexture* 	    pTexture;
		D3DTEXTUREOP				colorOP;

        MultiTexureData() : pTexture(NULL), colorOP(D3DTOP_MODULATE) {}
        ~MultiTexureData()          { Release(); }
        void    Release();
        void    Swap( MultiTexureData& rhs_ )
        {
            std::swap( pTexture, rhs_.pTexture );
            std::swap( colorOP, rhs_.colorOP );
        }
	};

#ifdef  X2OPTIMIZE_TET_XET_PREPROCESSING

    struct  AniData
    {
        friend class CKTDXDeviceXET;

        AniData()
            : m_pDeviceXET( NULL )
            , m_pAniData( NULL )
            , fTransitionTime( 0.f )
            , fAniMaxTime( 0.f )
        {
        }

        CKTDXDeviceTexture*     GetAniTexture( float fAniTime, const wchar_t* pTexName ) const;
        
        float                   fTransitionTime;
        float                   fAniMaxTime;

    private:

        CKTDXDeviceXET*          m_pDeviceXET;
        const KXETFormatAniData* m_pAniData;
    };

#else   X2OPTIMIZE_TET_XET_PREPROCESSING

		struct ChangeTexture
		{
			wstring						orgTexName;
			CKTDXDeviceTexture* 	pTexture;

			ChangeTexture();
			~ChangeTexture();
            void    Release();
		};

		struct AniEventTexChange
		{
			D3DXVECTOR2					eventTime;
			wstring						orgTexName;
			CKTDXDeviceTexture* 	pTexture;

			AniEventTexChange();
			~AniEventTexChange();
		};

		struct AniData
		{
			wstring					aniName;
			float					fTransitionTime;
			float					fAniMaxTime;
			
            typedef std::multimap<wstring,AniEventTexChange*>  TexChangeEventMultimap;
			TexChangeEventMultimap	m_TexChangeEventlist;

			AniData();
			~AniData();

			CKTDXDeviceTexture* GetAniTexture( float fAniTime, const WCHAR* pTexName ) const;
			void SetTexChangeEvent_LUA( D3DXVECTOR2 eventTime, const char* pOrgTexName, const char* pChangeTexName );
		};
		
        typedef map<wstring, ChangeTexture*>    ChangeTextureMap;
        typedef map<wstring, AniData*>          NameAniDataMap;

        struct  XETProxy
        {
    		ChangeTextureMap            m_ChangeTextureList;
	    	MultiTexureData			    m_MultiTexStage1;
		    MultiTexureData			    m_MultiTexStage2;
		    NameAniDataMap              m_AniDataList;

			~XETProxy();

		    void AddChangeTexture_LUA( const char* pOrgTexName, const char* pChangeTexName );
		    void SetMultiTexStage1_LUA( const char* pOrgTexName, const char* pBlendTexName, int colorOP );
		    void SetMultiTexStage2_LUA( const char* pOrgTexName, const char* pBlendTexName, int colorOP );
		    AniData* AddAniData_LUA( const char* pAniName, float fTransitionTime = 0.0f, float	fAniMaxTime = 0.0f );
		    AniData* AddAniDataByFrame_LUA( const char* pAniName, int transitionFrame = 0, float	fAniMaxTime = 0.0f );
        };//struct  XETProxy

#endif  X2OPTIMIZE_TET_XET_PREPROCESSING

	public:

		CKTDXDeviceXET( const WCHAR* pFileName ); //, CKTDXDevice* pMesh = NULL );


		CKTDXDeviceTexture*     GetChangeTexture( const wchar_t* pTexName );
		const MultiTexureData*  GetMultiTex( int stage ) const;
		void SetMultiTexStage1( const char* pOrgTexName, const char* pBlendTexName, int colorOP );
		void SetMultiTexStage2( const char* pOrgTexName, const char* pBlendTexName, int colorOP );
        void DeleteMultiTexStage1()	    { m_MultiTexStage1.Release(); }
        void DeleteMultiTexStage2()	    { m_MultiTexStage2.Release(); }
		const AniData*  GetAniData( const wchar_t* pAniName );

#ifdef  X2OPTIMIZE_TET_XET_PREPROCESSING
        CKTDXDeviceTexture*    GetAniTexture( const KXETFormatAniData& kAniData, float fAniTime, const wchar_t* pwszTexName ) const;
#else   X2OPTIMIZE_TET_XET_PREPROCESSING
		void AddChangeTexture( const WCHAR* wszOrgTexName, CKTDXDeviceTexture* pChangeTexture );
#endif  X2OPTIMIZE_TET_XET_PREPROCESSING

    protected:

		virtual HRESULT _Load( bool bSkipStateCheck = false
#ifdef	X2OPTIMIZE_SOUND_BACKGROUND_LOAD
				, bool bBackgroundQueueing = false
#endif	X2OPTIMIZE_SOUND_BACKGROUND_LOAD			
			);
		virtual HRESULT _UnLoad();

		virtual ~CKTDXDeviceXET(void);

	private:

		//CKTDXDevice*				m_pMesh;
				
		MultiTexureData			    m_MultiTexStage1;
		MultiTexureData			    m_MultiTexStage2;

#ifdef  X2OPTIMIZE_TET_XET_PREPROCESSING

        CKTDXDeviceXET_Preprocessing        m_preprocessed;
        std::vector<CKTDXDeviceTexture*>    m_vecTexture;
        std::vector<AniData>                m_vecAniData;

#else   X2OPTIMIZE_TET_XET_PREPROCESSING

		ChangeTextureMap            m_ChangeTextureList;
		NameAniDataMap              m_AniDataList;

#endif  X2OPTIMIZE_TET_XET_PREPROCESSING

// 		CKTDXCollision::CollisionDataList                m_CollisionList;
// 		CKTDXCollision::CollisionDataList                m_AttackList;        
};

/*
CT_LINE = 0
CT_SPHERE = 1

--텍스쳐 변경 만들기
g_pXET:AddChangeTexture( "orgTex.tga", "changeTex.tga" )

--멀티텍스쳐 만들기
g_pXET:SetMultiTexStage1( "orgTex.tga", "blendTex1.tga", D3DTOP_ADD )
g_pXET:SetMultiTexStage2( "orgTex.tga", "blendTex2.tga", D3DTOP_ADD )

--애니메이션 패키지 만들기
g_pTestAni = g_pXET:AddAniData( "Wait", 0.5, 1.0 )
g_pTestAni:SetTexChangeEvent( D3DXVECTOR2(0.3,0.9), "orgTex.tga", "changeTex.tga" )

--충돌데이타 만들기
g_pTestCollisionData = g_pXET:CreateCollisionData( "TestCollisionData", CT_LINE, false )
g_pTestCollisionData:SetPoint1( "frameName", D3DXVECTOR3(0,0,0) )
g_pTestCollisionData:SetPoint2( "frameNameLineEnd", D3DXVECTOR3(0,0,0) )
*/




