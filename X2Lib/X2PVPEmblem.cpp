#include "StdAfx.h"
#include ".\x2pvpemblem.h"

CX2PVPEmblem::CX2PVPEmblem(void)
{
#ifdef SERV_PVP_NEW_SYSTEM
#ifdef PVP_SEASON2
	OpenScript( L"PVPEmblem_Season2.lua" );
#else
	OpenScript( L"PVPEmblem_new.lua" );
#endif
#else
	OpenScript( L"PVPEmblem.lua" );
#endif
}

CX2PVPEmblem::~CX2PVPEmblem(void)
{
#ifdef PVP_SEASON2
	map< PVP_RANK, PVPEmblemData* >::iterator i;
#else
	map< PVP_EMBLEM, PVPEmblemData* >::iterator i;
#endif

	for ( i = m_mapEmblemData.begin(); i != m_mapEmblemData.end(); i++ )
	{
		CX2PVPEmblem::PVPEmblemData* pPVPEmblemData = i->second;
		SAFE_DELETE( pPVPEmblemData );
	}
	m_mapEmblemData.clear();

	for ( int i = 0; i < (int)m_vecReadyTexture.size(); i++ )
	{
		CKTDXDeviceTexture* pTexture = m_vecReadyTexture[i];
		SAFE_CLOSE( pTexture );
	}
	m_vecReadyTexture.clear();
}

#ifdef PVP_SEASON2
CX2PVPEmblem::PVPEmblemData* CX2PVPEmblem::GetPVPEmblemData( IN PVP_RANK eEmblemID_ )
{
	CX2PVPEmblem::PVPEmblemData* pPVPEmblemData = NULL;

	map< CX2PVPEmblem::PVP_RANK, PVPEmblemData* >::iterator i;
	i = m_mapEmblemData.find( eEmblemID_ );

	if ( i != m_mapEmblemData.end() )
	{
		pPVPEmblemData = i->second;
	}

	return pPVPEmblemData;
}
#else
#ifdef SERV_PVP_NEW_SYSTEM
CX2PVPEmblem::PVPEmblemData* CX2PVPEmblem::GetPVPEmblemData( int iRating )
{
	CX2PVPEmblem::PVP_EMBLEM emblemID = GetEmblemID( iRating );
	return GetPVPEmblemData( emblemID );
}
#endif

CX2PVPEmblem::PVPEmblemData* CX2PVPEmblem::GetPVPEmblemData( CX2PVPEmblem::PVP_EMBLEM emblemID )
{
	CX2PVPEmblem::PVPEmblemData* pPVPEmblemData = NULL;
	
	map< CX2PVPEmblem::PVP_EMBLEM, PVPEmblemData* >::iterator i;
	i = m_mapEmblemData.find( emblemID );

	if ( i != m_mapEmblemData.end() )
	{
		pPVPEmblemData = i->second;
	}

	return pPVPEmblemData;
}
#endif

CX2PVPEmblem::PVPEmblemData* CX2PVPEmblem::GetMyNowUnitPVPEmblem()
{
	if ( g_pData->GetMyUser() == NULL || g_pData->GetMyUser()->GetSelectUnit() == NULL )
		return NULL;

#ifdef SERV_PVP_NEW_SYSTEM
	#ifdef PVP_SEASON2
		char cRank = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_cRank;
		return  GetPVPEmblemData( static_cast<CX2PVPEmblem::PVP_RANK>( cRank ) );
	#else
		return GetPVPEmblemData( g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_iRating );
	#endif
#else
	return GetPVPEmblemData( g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_PVPEmblem );
#endif
}

#ifdef PVP_SEASON2
void CX2PVPEmblem::AddEmblemData_LUA( int emblemID, const char* pName, const char* pTextureName, const char* pTextureKey )
#else
void CX2PVPEmblem::AddEmblemData_LUA( int emblemID, const char* pName, D3DXVECTOR2 vpPoint, const char* pTextureName, const char* pTextureKey )
#endif
{
#ifdef PVP_SEASON2
	CX2PVPEmblem::PVP_RANK pvpEmblemID = static_cast<CX2PVPEmblem::PVP_RANK>( emblemID );
#else
	CX2PVPEmblem::PVP_EMBLEM pvpEmblemID = (CX2PVPEmblem::PVP_EMBLEM)emblemID;
#endif
	
	if ( m_mapEmblemData.find( pvpEmblemID ) != m_mapEmblemData.end() )
	{
		MessageBox( g_pKTDXApp->GetHWND(), GET_STRING( STR_ID_434 ), GET_STRING( STR_ID_435 ), MB_OK );
		return;
	}

	CX2PVPEmblem::PVPEmblemData* pPVPEmblemData = new CX2PVPEmblem::PVPEmblemData();
	pPVPEmblemData->m_EmblemID = pvpEmblemID;
	ConvertUtf8ToWCHAR( pPVPEmblemData->m_EmblemName, pName );
#ifndef PVP_SEASON2
	pPVPEmblemData->m_MinVPPoint = (int)vpPoint.x;
	pPVPEmblemData->m_MaxVPPoint = (int)vpPoint.y;
#endif
	
	ConvertUtf8ToWCHAR( pPVPEmblemData->m_TextureName, pTextureName );
	ConvertUtf8ToWCHAR( pPVPEmblemData->m_TextureKey, pTextureKey );

	CKTDXDeviceTexture* pTexture = g_pKTDXApp->GetDeviceManager()->OpenTexture( pPVPEmblemData->m_TextureName );
	if ( pTexture != NULL )
		m_vecReadyTexture.push_back( pTexture );

	m_mapEmblemData.insert( std::make_pair( pvpEmblemID, pPVPEmblemData ) );
}

bool CX2PVPEmblem::OpenScript( const WCHAR* pFileName )
{
	if ( pFileName == NULL )
		return false;

	//바인더에 등록
	lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(),  "g_pX2PVPEmblem",	this );

    if ( g_pKTDXApp->LoadLuaTinker( pFileName ) == false )
    {
		MessageBox( g_pKTDXApp->GetHWND(), pFileName, GET_STRING( STR_ID_436 ), MB_OK );
		return false;
    }

	return true;
}

#ifndef SERV_PVP_NEW_SYSTEM
CX2PVPEmblem::PVP_EMBLEM CX2PVPEmblem::PvpEmblemEnumToPvpEmblem( CX2PVPEmblem::PVP_EMBLEM_ENUM eEmblemEnum )
{
	switch( eEmblemEnum )
	{
	case CX2PVPEmblem::PEE_RECRUIT:
		return CX2PVPEmblem::PE_RECRUIT;
	case CX2PVPEmblem::PEE_GUARDS:	
		return CX2PVPEmblem::PE_GUARDS;
	case CX2PVPEmblem::PEE_MERCENARY:
			return CX2PVPEmblem::PE_MERCENARY;
	case CX2PVPEmblem::PEE_KNIGHTS:		
		return CX2PVPEmblem::PE_KNIGHTS;
	case CX2PVPEmblem::PEE_COMMANDER:
			return CX2PVPEmblem::PE_COMMANDER;
	case CX2PVPEmblem::PEE_EMPEROR:		
		return CX2PVPEmblem::PE_EMPEROR;
	case CX2PVPEmblem::PEE_HERO:	
		return CX2PVPEmblem::PE_HERO;
	case CX2PVPEmblem::PEE_GOTOWAR:		
		return CX2PVPEmblem::PE_GOTOWAR;
	default:						
		return CX2PVPEmblem::PE_RECRUIT;
	}
}

CX2PVPEmblem::PVP_EMBLEM_ENUM CX2PVPEmblem::PvpEmblemToPvpEmblemEnum( CX2PVPEmblem::PVP_EMBLEM eEmblemEnum )
{
	switch( eEmblemEnum )
	{
	case CX2PVPEmblem::PE_RECRUIT:
		return CX2PVPEmblem::PEE_RECRUIT;
	case CX2PVPEmblem::PE_GUARDS:	
		return CX2PVPEmblem::PEE_GUARDS;
	case CX2PVPEmblem::PE_MERCENARY:
		return CX2PVPEmblem::PEE_MERCENARY;
	case CX2PVPEmblem::PE_KNIGHTS:		
		return CX2PVPEmblem::PEE_KNIGHTS;
	case CX2PVPEmblem::PE_COMMANDER:
		return CX2PVPEmblem::PEE_COMMANDER;
	case CX2PVPEmblem::PE_EMPEROR:		
		return CX2PVPEmblem::PEE_EMPEROR;
	case CX2PVPEmblem::PE_HERO:	
		return CX2PVPEmblem::PEE_HERO;
	case CX2PVPEmblem::PE_GOTOWAR:		
		return CX2PVPEmblem::PEE_GOTOWAR;
	default:						
		return CX2PVPEmblem::PEE_RECRUIT;
	}
}
#else //SERV_PVP_NEW_SYSTEM

#ifdef PVP_SEASON2
CX2PVPEmblem::PVP_EMBLEM CX2PVPEmblem::GetEmblemID( int iRating )
{
	if( iRating < CX2PVPEmblem::PE_RANK_D )
		return CX2PVPEmblem::PE_RANK_E;
	else if( iRating < CX2PVPEmblem::PE_RANK_C )
		return CX2PVPEmblem::PE_RANK_D;
	else if( iRating < CX2PVPEmblem::PE_RANK_B )
		return CX2PVPEmblem::PE_RANK_C;
	else if( iRating < CX2PVPEmblem::PE_RANK_A )
		return CX2PVPEmblem::PE_RANK_B;
	else if( iRating < CX2PVPEmblem::PE_RANK_S )
		return CX2PVPEmblem::PE_RANK_S;
	else if( iRating < CX2PVPEmblem::PE_RANK_SS )
		return CX2PVPEmblem::PE_RANK_S;
	else if( iRating < CX2PVPEmblem::PE_RANK_SSS )
		return CX2PVPEmblem::PE_RANK_SS;
	else 
		return CX2PVPEmblem::PE_RANK_SSS;
}
CX2PVPEmblem::PVP_EMBLEM CX2PVPEmblem::GetEmblemID( CX2PVPEmblem::PVP_RANK eRank )
{
	switch( eRank )
	{
	case PVPRANK_NONE:
		return PE_NONE;
	case PVPRANK_RANK_E:
		return PE_RANK_E;
	case PVPRANK_RANK_D:
		return PE_RANK_D;
	case PVPRANK_RANK_C:
		return PE_RANK_C;
	case PVPRANK_RANK_B:
		return PE_RANK_B;
	case PVPRANK_RANK_A:
		return PE_RANK_A;
	case PVPRANK_RANK_S:
		return PE_RANK_S;
	case PVPRANK_RANK_SS:
		return PE_RANK_SS;
	case PVPRANK_RANK_SSS:
		return PE_RANK_SSS;
	default:
		return PE_NONE;
	}
}
#else
CX2PVPEmblem::PVP_EMBLEM CX2PVPEmblem::GetEmblemID( int iRating )
{
	if( iRating < CX2PVPEmblem::PE_RANK_E )
		return CX2PVPEmblem::PE_RANK_F;
	else if( iRating < CX2PVPEmblem::PE_RANK_D )
		return CX2PVPEmblem::PE_RANK_E;
	else if( iRating < CX2PVPEmblem::PE_RANK_C )
		return CX2PVPEmblem::PE_RANK_D;
	else if( iRating < CX2PVPEmblem::PE_RANK_B )
		return CX2PVPEmblem::PE_RANK_C;
	else if( iRating < CX2PVPEmblem::PE_RANK_A )
		return CX2PVPEmblem::PE_RANK_B;
	else if( iRating < CX2PVPEmblem::PE_RANK_S )
		return CX2PVPEmblem::PE_RANK_A;
	else if( iRating < CX2PVPEmblem::PE_RANK_SS )
		return CX2PVPEmblem::PE_RANK_S;
	else 
		return CX2PVPEmblem::PE_RANK_SS;
}
CX2PVPEmblem::PVP_EMBLEM CX2PVPEmblem::GetEmblemID( CX2PVPEmblem::PVP_RANK eRank )
{
	switch( eRank )
	{
	case PVPRANK_NONE:
		return PE_NONE;
	case PVPRANK_RANK_F:
		return PE_RANK_F;
	case PVPRANK_RANK_E:
		return PE_RANK_E;
	case PVPRANK_RANK_D:
		return PE_RANK_D;
	case PVPRANK_RANK_C:
		return PE_RANK_C;
	case PVPRANK_RANK_B:
		return PE_RANK_B;
	case PVPRANK_RANK_A:
		return PE_RANK_A;
	case PVPRANK_RANK_S:
		return PE_RANK_S;
	case PVPRANK_RANK_SS:
		return PE_RANK_SS;
	default:
		return PE_NONE;
	}
}
#endif

#endif //SERV_PVP_NEW_SYSTEM