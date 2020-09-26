#pragma once

class CX2PVPEmblem
{
	public:

#ifdef SERV_PVP_NEW_SYSTEM
	#ifdef PVP_SEASON2
		enum PVP_EMBLEM
		{
			PE_NONE			= -1,
			PE_RANK_E		= 0,
			PE_RANK_D		= 251,
			PE_RANK_C		= 551,
			PE_RANK_B		= 951,
			PE_RANK_A		= 1351,
			PE_RANK_S		= 1851,
			PE_RANK_SS		= 2351,
			PE_RANK_SSS		= 2951,
			PE_END			= 3300,
		};

		enum PVP_RANK
		{
			PVPRANK_NONE			= 0,
			PVPRANK_RANK_ARRANGE	= 1,
			PVPRANK_RANK_E			= 2,
			PVPRANK_RANK_D			= 3,
			PVPRANK_RANK_C			= 4,
			PVPRANK_RANK_B			= 5,
			PVPRANK_RANK_A			= 6,
			PVPRANK_RANK_S			= 7,
			PVPRANK_RANK_SS			= 8,
			PVPRANK_RANK_SSS		= 9,
//{{ robobeg : 2013-02-27
            PVPRANK_MAX,
//}} robobeg : 2013-02-27
		};
	#else
		enum PVP_EMBLEM
		{
			PE_NONE			= -1,
			PE_RANK_F		= 0,
			PE_RANK_E		= 251,
			PE_RANK_D		= 501,
			PE_RANK_C		= 1001,
			PE_RANK_B		= 1501,
			PE_RANK_A		= 2001,
			PE_RANK_S		= 2501,
			PE_RANK_SS		= 2701,
			PE_END			= 100000000,
		};

		enum PVP_RANK
		{
			PVPRANK_NONE		= 0,
			PVPRANK_RANK_F		= 1,
			PVPRANK_RANK_E		= 2,
			PVPRANK_RANK_D		= 3,
			PVPRANK_RANK_C		= 4,
			PVPRANK_RANK_B		= 5,
			PVPRANK_RANK_A		= 6,
			PVPRANK_RANK_S		= 7,
			PVPRANK_RANK_SS		= 8,
		};
	#endif
#else
		enum PVP_EMBLEM
		{
			PE_NONE			= -1,
			PE_RECRUIT		=        0,		//훈련병
			PE_GUARDS		=     1500,		//수비대
			PE_MERCENARY	=    10000,		//용병단
			PE_KNIGHTS		=   125000,		//기사단
			PE_COMMANDER	=   615000,		//장군
			PE_EMPEROR		=  1835000,		//황제
			PE_HERO			=  4300000,		//영웅
			PE_GOTOWAR		=  9500000,		//투신
			PE_END			=100000000,		//100000(기존맥시멈)
		};

		//{{ 2009. 3. 30  최육사	대전랭크
		enum PVP_EMBLEM_ENUM
		{
			PEE_NONE	= -1,
			PEE_RECRUIT = 0,	//훈련병
			PEE_GUARDS,			//수비대
			PEE_MERCENARY,		//용병단
			PEE_KNIGHTS,		//기사단
			PEE_COMMANDER,		//장군
			PEE_EMPEROR,		//황제
			PEE_HERO,			//영웅
			PEE_GOTOWAR,		//투신
			PEE_END,			//100000(기존맥시멈)
		};
#endif

		struct PVPEmblemData
		{
			int m_EmblemID;
			wstring m_EmblemName;
#ifndef PVP_SEASON2
			int m_MinVPPoint;
			int m_MaxVPPoint;
#endif
			wstring m_TextureName;
			wstring m_TextureKey;
		};

	public:

		CX2PVPEmblem(void);
		~CX2PVPEmblem(void);

#ifdef PVP_SEASON2
		PVPEmblemData* GetPVPEmblemData( IN PVP_RANK eEmblemID_ );
		void AddEmblemData_LUA( int emblemID, const char* pName, const char* pTextureName, const char* pTextureKey );
#else
#ifdef SERV_PVP_NEW_SYSTEM
		PVPEmblemData* GetPVPEmblemData( int iRating );
#endif
		PVPEmblemData* GetPVPEmblemData( PVP_EMBLEM m_EmblemID );
		void AddEmblemData_LUA( int emblemID, const char* pName, D3DXVECTOR2 vpPoint, const char* pTextureName, const char* pTextureKey );
#endif
		PVPEmblemData* GetMyNowUnitPVPEmblem();

#ifndef SERV_PVP_NEW_SYSTEM
		CX2PVPEmblem::PVP_EMBLEM PvpEmblemEnumToPvpEmblem( CX2PVPEmblem::PVP_EMBLEM_ENUM eEmblemEnum );
		CX2PVPEmblem::PVP_EMBLEM_ENUM PvpEmblemToPvpEmblemEnum( CX2PVPEmblem::PVP_EMBLEM eEmblemEnum );
#else
		CX2PVPEmblem::PVP_EMBLEM GetEmblemID( int iRating );
		CX2PVPEmblem::PVP_EMBLEM GetEmblemID( CX2PVPEmblem::PVP_RANK eRank );
#endif

	protected:

		bool OpenScript( const WCHAR* pFileName );

#ifdef PVP_SEASON2
		map< PVP_RANK, PVPEmblemData* >	m_mapEmblemData;
#else
		map< PVP_EMBLEM, PVPEmblemData* >	m_mapEmblemData;
#endif
		vector< CKTDXDeviceTexture* >		m_vecReadyTexture;
};


