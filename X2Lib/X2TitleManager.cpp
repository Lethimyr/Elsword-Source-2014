#include "StdAfx.h"

//#ifdef TITLE_SYSTEM

//{{ 최민철 [2013/1/4]  게임내 정보 스트링을 엑셀파일로 출력
#ifdef PRINT_INGAMEINFO_TO_EXCEL
#include "BasicExcel.hpp"
using namespace YExcel;
#endif PRINT_INGAMEINFO_TO_EXCEL
//}} 최민철 [2013/1/4]  게임내 정보 스트링을 엑셀파일로 출력


// 칭호 추가하는 방법
// UIMajorParticle.txt에 칭호 파티클 추가, 이름은 "Title_칭호ID", 파티클이름과 동일한 이름의 .tga 파일 텍스쳐가 UI에서 보여진다. titletable.lua에 추가하면 완료.



#include ".\x2titlemanager.h"

CX2TitleManager::CX2TitleManager(void)
{	
    m_pDlgTitle = new CX2TitleSystem(g_pMain->GetNowState());
    m_bInit = false;
    m_iSelTitleId = 0;
//#if 0 // 칭호 프리뷰
//    m_hSeqEmblem = INVALID_PARTICLE_SEQUENCE_HANDLE;
//    m_pPart_Emblem_200 = NULL;
//#endif

    m_strDesc = GET_STRING( STR_ID_5378 );
	m_strImgName = L"";
    m_bProcess = false;
    m_bNewTitle = false;
    m_bShowNewTitle = false;
    m_bNewOpenMission = false;
	m_nString_Index = 0;

#ifdef SHOW_NEW_TITLE_NAME
	m_strNewTitle = L"";
#endif //SHOW_NEW_TITLE_NAME
}

CX2TitleManager::~CX2TitleManager(void)
{
	std::map< int, TitleInfo* >::iterator mit;
	for( mit = m_mapTitleInfo.begin(); mit != m_mapTitleInfo.end(); ++mit )
	{
		SAFE_DELETE( mit->second );
	}

//#if 0 // 칭호 프리뷰
//    if(m_hSeqEmblem != INVALID_PARTICLE_SEQUENCE_HANDLE)
//        g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hSeqEmblem );
//    m_hSeqEmblem = INVALID_PARTICLE_SEQUENCE_HANDLE;
//    m_pPart_Emblem_200 = NULL;
//#endif

    SAFE_DELETE(m_pDlgTitle);
}

HRESULT CX2TitleManager::OnFrameMove(double fTime, float fElapsedTime)
{
 
    //if(m_pDlgTitle != NULL && GetShow() == true)
    //{
    //    m_pDlgTitle->ShowTitleGuideDesc();
    //}       

    return S_OK;
}

void CX2TitleManager::CheckNewTitle()
{
    //if(g_pData != NULL && g_pData->GetCharacterRoom() != NULL)
    //{
    //    g_pData->GetCharacterRoom()->SetTitleNew(m_bNewTitle);
    //}   

    // 새로 얻은 칭호에대한 알림
    if(m_bShowNewTitle == true)
    {
		switch( g_pMain->GetNowStateID() )
		{
		case CX2Main::XS_DUNGEON_GAME:
			{
				m_bShowNewTitle = false;
				m_bNewOpenMission = false;

				wstring wstrSystemMessageColor = L"#CFF3F4D";								// 255, 63, 77
				D3DXCOLOR coSystemMessageTextColor(1.f, 0.24705f, 0.30196f, 1.f);			// 255, 63, 77
				
#ifdef SHOW_NEW_TITLE_NAME
				// 동시에 여러 칭호를 획득하였을경우 마지막 획득한 칭호 이름만 보여준다.
				g_pX2Game->GetInfoTextManager().PushText( XUF_DODUM_20_BOLD, GET_REPLACED_STRING( (STR_ID_17352, "L", m_strNewTitle) ), coSystemMessageTextColor, D3DXCOLOR(0,0,0,1), DT_CENTER, 1.f, 1.f );
				g_pChatBox->AddChatLog( GET_REPLACED_STRING( (STR_ID_17352, "L", m_strNewTitle) )  , KEGS_CHAT_REQ::CPT_SYSTEM, coSystemMessageTextColor, wstrSystemMessageColor );
#else
				g_pX2Game->GetInfoTextManager().PushText( XUF_DODUM_20_BOLD, GET_STRING( STR_ID_756 ), coSystemMessageTextColor, D3DXCOLOR(0,0,0,1), DT_CENTER, 1.f, 1.f );
				g_pChatBox->AddChatLog( GET_STRING( STR_ID_756 ), KEGS_CHAT_REQ::CPT_SYSTEM, coSystemMessageTextColor, wstrSystemMessageColor );				
#endif //SHOW_NEW_TITLE_NAME

			} break;

		case CX2Main::XS_PVP_GAME:
		case CX2Main::XS_PVP_RESULT:
		case CX2Main::XS_DUNGEON_RESULT:
		case CX2Main::XS_TRAINING_GAME:
		case CX2Main::XS_TRAINING_RESULT:
			{
				// 게임중에는 알리지 않는다
			} break;

		default:
			{
				m_bShowNewTitle = false;
				m_bNewOpenMission = false;
#ifdef SHOW_NEW_TITLE_NAME
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250, 300), GET_REPLACED_STRING( (STR_ID_17352, "L", m_strNewTitle) ), (CKTDXStage*) g_pMain->GetNowState() ); 
#else
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250, 300), GET_STRING( STR_ID_756 ), (CKTDXStage*) g_pMain->GetNowState() ); 
#endif //SHOW_NEW_TITLE_NAME
			} break;
		}
		if( NULL != g_pData && NULL != g_pData->GetPlayGuide())
		{	//칭호 획득 시 칭호 가이드 출력
			if( NULL != g_pMain && NULL != g_pMain->GetKeyPad() )
			{
				g_pData->GetPlayGuide()->SetPlayGuide(CX2PlayGuide::PGT_OBTAIN_TITLE, true, g_pMain->GetKeyPad()->GetKeyString( GAMEACTION_INFO ));
			}
		}
    }

    if(m_bNewOpenMission == true)
    {

		switch( g_pMain->GetNowStateID() )
		{
		case CX2Main::XS_DUNGEON_GAME:
		case CX2Main::XS_BATTLE_FIELD:
			{
				m_bNewOpenMission = false;

				wstring wstrSystemMessageColor = L"#CFF3F4D";								// 255, 63, 77
				D3DXCOLOR coSystemMessageTextColor(1.f, 0.24705f, 0.30196f, 1.f);			// 255, 63, 77

				g_pX2Game->GetInfoTextManager().PushText( XUF_DODUM_20_BOLD, GET_STRING( STR_ID_757 ), coSystemMessageTextColor, D3DXCOLOR(0,0,0,1), DT_CENTER, 1.f, 1.f );
				g_pChatBox->AddChatLog( GET_STRING( STR_ID_757 ), KEGS_CHAT_REQ::CPT_SYSTEM, coSystemMessageTextColor, wstrSystemMessageColor );

			} break;

		case CX2Main::XS_PVP_GAME:
		case CX2Main::XS_PVP_RESULT:
		case CX2Main::XS_DUNGEON_RESULT:
		case CX2Main::XS_TRAINING_GAME:
		case CX2Main::XS_TRAINING_RESULT:
			{
				// 게임중에는 알리지 않는다
			} break;

		default:
			{
				m_bNewOpenMission = false;
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250, 300), GET_STRING( STR_ID_757 ), (CKTDXStage*) g_pMain->GetNowState() );            
			} break;
		}
    }
}

bool CX2TitleManager::OpenScriptFile( const WCHAR* pFileName )
{
	lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(),  "TitleManager", this );	

    if ( g_pKTDXApp->LoadLuaTinker( pFileName ) == false )
    {
		ErrorLogMsg( XEM_ERROR9, pFileName );

		return false;
    }

	return true;
}

bool CX2TitleManager::AddTitleInfo_LUA()
{
	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
#ifndef X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
    TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING

	TitleInfo* pTitleInfo = new TitleInfo();

	
	LUA_GET_VALUE_RETURN(	luaManager, "m_iTitleID",				pTitleInfo->m_iTitleID,					0,				goto end_proc );
	LUA_GET_VALUE(			luaManager, "m_iSortNum",				pTitleInfo->m_iSortNum,					0 );

	LUA_GET_VALUE_ENUM(		luaManager, "m_eTitleType",			pTitleInfo->m_eTitleType,				TITLE_TYPE,		CX2TitleManager::TT_NONE );
	
	//09. 05. 14 김정협 머지를 위해서 인덱스 읽는 부분 막음
	LUA_GET_VALUE(			luaManager, "m_TitleName",			    pTitleInfo->m_wstrTitleName,			L"" );
	//LUA_GET_VALUE(	luaManager, "m_iTitleName_Index",			m_nString_Index,		0);
	//pTitleInfo->m_wstrTitleName = GET_SCRIPT_STRING(m_nString_Index);

	//09. 05. 14 김정협 머지를 위해서 인덱스 읽는 부분 막음
    LUA_GET_VALUE(			luaManager, "m_Description",			pTitleInfo->m_wstrDescription,			L"" );
	//LUA_GET_VALUE(	luaManager, "m_iDescription_Index",			m_nString_Index,		0);
	//pTitleInfo->m_wstrDescription = GET_SCRIPT_STRING(m_nString_Index);

    LUA_GET_VALUE(			luaManager, "m_ParticleName",			pTitleInfo->m_wstrParticleName,			L"" );

#ifdef SERV_GROW_UP_TITLE
	if( pTitleInfo->m_wstrParticleName.find( L"," ) != -1 )
	{
		// 여기 오면 진화 타이틀이다
		wstring tempTitleID;
		int stringIndex = -1;
		int iLevel = 1;
		while( pTitleInfo->m_wstrParticleName.find( L",", stringIndex + 1 ) != -1 )
		{
			int debugIndex = pTitleInfo->m_wstrParticleName.find( L",", stringIndex + 1 );
			tempTitleID = pTitleInfo->m_wstrParticleName.substr( stringIndex + 1, debugIndex - stringIndex - 1 );
			if( tempTitleID.compare( L"TITLE" ) != 0 )
			{
				pTitleInfo->m_mapGrowUpParticleName.insert( std::make_pair( iLevel, tempTitleID ) );
				iLevel++;
			}
			stringIndex = debugIndex;
		}

		tempTitleID = pTitleInfo->m_wstrParticleName.substr( stringIndex + 1 );
		if ( tempTitleID.compare( L"TITLE" ) != 0 )
			pTitleInfo->m_mapGrowUpParticleName.insert( std::make_pair( iLevel, tempTitleID ) );

		pTitleInfo->m_wstrParticleName = pTitleInfo->m_mapGrowUpParticleName[1];
	}
#endif //SERV_GROW_UP_TITLE
	LUA_GET_VALUE_ENUM(		luaManager, "m_eUnitType",				pTitleInfo->m_eUnitType,				CX2Unit::UNIT_TYPE,		CX2Unit::UT_NONE );
	LUA_GET_VALUE_ENUM(		luaManager, "m_eUnitClass",			pTitleInfo->m_eUnitClass,				CX2Unit::UNIT_CLASS,	CX2Unit::UC_NONE );
	LUA_GET_VALUE(			luaManager, "m_iOpenLevel",			pTitleInfo->m_iOpenLevel,				0 );
	LUA_GET_VALUE(			luaManager, "m_bIsSecretTitle",		pTitleInfo->m_bIsSecretTitle,			false );
    
	LUA_GET_VALUE(			luaManager, "m_iBaseHP",				pTitleInfo->m_iBaseHP,					0 );
	LUA_GET_VALUE(			luaManager, "m_iAtkPhysic",			pTitleInfo->m_iAtkPhysic,				0 );
	LUA_GET_VALUE(			luaManager, "m_iAtkMagic",				pTitleInfo->m_iAtkMagic,				0 );
	LUA_GET_VALUE(			luaManager, "m_iDefPhysic",			pTitleInfo->m_iDefPhysic,				0 );
	LUA_GET_VALUE(			luaManager, "m_iDefMagic",				pTitleInfo->m_iDefMagic,				0 );

	LUA_GET_VALUE(			luaManager, "m_bVisible",				pTitleInfo->m_bVisible,					true );

	if( luaManager.BeginTable( "SOCKET_OPTION" ) == true )
	{
		int index = 1;
		int iSocketOption	= -1;
		while( luaManager.GetValue( index, iSocketOption ) == true )
		{
			if( iSocketOption > 0 )
				pTitleInfo->m_vecSocketOption.push_back( iSocketOption );
			++index;
		}
		luaManager.EndTable();
	}

#ifndef CLIENT_COUNTRY_TW
	if( pTitleInfo->m_iTitleID == 5160 )	//대만 전용 감마니아 칭호
	{

	}
	else
#endif CLIENT_COUNTRY_TW
	m_mapTitleInfo.insert( std::make_pair( pTitleInfo->m_iTitleID, pTitleInfo ) );
	return true;

end_proc:
	SAFE_DELETE(pTitleInfo); 
	ErrorLog(XEM_ERROR132);
	return false;
}

#ifdef LUA_TRANS_DEVIDE
bool CX2TitleManager::AddTitleInfoTrans_LUA()
{
	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
#ifndef X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
	TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );
#endif X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING

	TitleInfo* pTitleInfo = new TitleInfo();
	LUA_GET_VALUE(	luaManager, "m_iTitleID",				pTitleInfo->m_iTitleID,					0);
	LUA_GET_VALUE(	luaManager, "m_TitleName",			    pTitleInfo->m_wstrTitleName,			L"" );
	LUA_GET_VALUE(	luaManager, "m_Description",			pTitleInfo->m_wstrDescription,			L"" );

	// 여기서 치환 해주어야 함.
	std::map< int, TitleInfo* >::iterator mit;
	mit = m_mapTitleInfo.find( pTitleInfo->m_iTitleID );
	if( mit == m_mapTitleInfo.end() )
	{
		wstringstream wstrStream;
		wstrStream << L"TitleTableTrans.lua And TitleTable.lua Not Equal ID..." << (pTitleInfo->m_iTitleID) << L"_";
		ErrorLogMsg( XEM_ERROR105, wstrStream.str().c_str() );	
		// 에러로그 
		SAFE_DELETE(pTitleInfo); 
		return false;
	}
	else
	{
		mit->second->m_wstrTitleName = pTitleInfo->m_wstrTitleName;
		mit->second->m_wstrDescription = pTitleInfo->m_wstrDescription;
	}

	return true;
}

bool CX2TitleManager::AddTitleMissionInfoTrans_LUA()
{
	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
#ifndef X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
	TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );
#endif X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING

	MissionTemplet kMissionTemplet;
	LUA_GET_VALUE_RETURN(		luaManager, "m_iMissionID",			kMissionTemplet.m_iMissionID,				0,			goto LoadFail );
	LUA_GET_VALUE_RETURN(		luaManager, "m_MissionName",			kMissionTemplet.m_wstrMissionName,			L"",		goto LoadFail );

	// 여기서 치환 해주어야 함.
	std::map< int, MissionTemplet >::iterator mit;
	mit = m_mapTitleMission.find( kMissionTemplet.m_iMissionID );
	if( mit == m_mapTitleMission.end() )
	{
		wstringstream wstrStream;
		wstrStream << L"TitleMissionTrans.lua And TitleMission.lua Not Equal ID..." << (kMissionTemplet.m_iMissionID) << L"_";
		ErrorLogMsg( XEM_ERROR105, wstrStream.str().c_str() );	
		// 에러로그 
		return false;
	}
	else
	{
		mit->second.m_wstrMissionName = kMissionTemplet.m_wstrMissionName;
	}

	return true;

LoadFail:
	ErrorLog(XEM_ERROR132);
	return false;
}

bool CX2TitleManager::AddSubTitleMissionInfoTrans_LUA()
{
	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
#ifndef X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
	TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );
#endif X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING

	SubMissionTemplet kSubMissionTemplet;
	LUA_GET_VALUE_RETURN(		luaManager, "m_iID",				kSubMissionTemplet.m_iSubMissionID,		0,		goto LoadFail; );
	LUA_GET_VALUE_RETURN(		luaManager, "m_wstrDescription",	kSubMissionTemplet.m_wstrDescription,	L"",	goto LoadFail; );

	// 여기서 치환 해주어야 함.
	std::map< int, SubMissionTemplet >::iterator mit;
	mit = m_mapSubMission.find( kSubMissionTemplet.m_iSubMissionID );
	if( mit == m_mapSubMission.end() )
	{
		wstringstream wstrStream;
		wstrStream << L"SubTitleMissionTrans.lua And SubTitleMission.lua Not Equal ID..." << (kSubMissionTemplet.m_iSubMissionID) << L"_";
		ErrorLogMsg( XEM_ERROR105, wstrStream.str().c_str() );	
		// 에러로그 
		return false;
	}
	else
	{
		mit->second.m_wstrDescription = kSubMissionTemplet.m_wstrDescription;
	}

	return true;

LoadFail:
	ErrorLog(XEM_ERROR132);
	return false;
}
#endif LUA_TRANS_DEVIDE

bool CX2TitleManager::AddTitleMissionInfo_LUA()
{
    KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
#ifndef X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
    TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );
#endif X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING

    MissionTemplet kMissionTemplet;

    LUA_GET_VALUE_RETURN(		luaManager, "m_iMissionID",			kMissionTemplet.m_iMissionID,				0,			goto LoadFail );
    LUA_GET_VALUE_RETURN_ENUM(	luaManager, "m_eClearType",			kMissionTemplet.m_eClearType,				TITLE_MISSION_CLEAR_TYPE, TMCT_NONE, goto LoadFail; );
	
	//09. 05. 14 김정협 머지를 위해서 인덱스 읽는 부분 막음
    LUA_GET_VALUE_RETURN(		luaManager, "m_MissionName",			kMissionTemplet.m_wstrMissionName,			L"",		goto LoadFail );
	//LUA_GET_VALUE_RETURN(	luaManager, "m_iMissionName_Index",			m_nString_Index,		0,		goto LoadFail; );	
	//kMissionTemplet.m_wstrMissionName = GET_SCRIPT_STRING(m_nString_Index);

    // Load Mission Condition
    LUA_GET_VALUE_RETURN(		luaManager, "m_iConditionLv",			kMissionTemplet.m_kCondition.m_iLevel,		-1,			goto LoadFail; );
    LUA_GET_VALUE_RETURN_ENUM(	luaManager, "m_eConditionUnitType",	kMissionTemplet.m_kCondition.m_eUnitType,	CX2Unit::UNIT_TYPE,	CX2Unit::UT_NONE,	goto LoadFail; );
    LUA_GET_VALUE_RETURN_ENUM(	luaManager, "m_eConditionUnitClass",	kMissionTemplet.m_kCondition.m_eUnitClass,	CX2Unit::UNIT_CLASS,	CX2Unit::UC_NONE,	goto LoadFail; );

    int iClearMissionID = 0;
    LUA_GET_VALUE_RETURN(		luaManager, "m_iConditionClearMissionID", iClearMissionID,							-1,			goto LoadFail );




    if( iClearMissionID > 0 )
        kMissionTemplet.m_kCondition.m_vecClearMissionID.push_back( iClearMissionID );

    // Load Sub Mission
    if( luaManager.BeginTable( "m_SubMission" ) == true )
    {
        int index	= 1; 
        int buf		= -1;
        while( luaManager.GetValue( index, buf ) == true )
        {
            if( buf > 0 )
                kMissionTemplet.m_vecSubMission.push_back( buf );
            index++;
        }

        luaManager.EndTable();
    }

    // Load Reward
    LUA_GET_VALUE(				luaManager, "m_sPeriod",				kMissionTemplet.m_sPeriod,		0 );
    LUA_GET_VALUE_RETURN(		luaManager, "m_iTitleID",				kMissionTemplet.m_iTitleID,		0,		goto LoadFail );	

    m_mapTitleMission.insert( std::make_pair( kMissionTemplet.m_iMissionID, kMissionTemplet ) );
    return true;

LoadFail:
    ErrorLog(XEM_ERROR132);
    return false;
}

const CX2TitleManager::MissionTemplet* CX2TitleManager::GetMissionInfo( int iMissionID )
{
    std::map< int, MissionTemplet >::const_iterator mit;
    mit = m_mapTitleMission.find( iMissionID );
    if( mit == m_mapTitleMission.end() )
        return NULL;

    return &mit->second;
}

bool CX2TitleManager::AddSubTitleMissionInfo_LUA()
{
    KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
#ifndef X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
    TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING

    SubMissionTemplet kSubMissionTemplet;

    LUA_GET_VALUE_RETURN(		luaManager, "m_iID",				kSubMissionTemplet.m_iSubMissionID,		0,		goto LoadFail; );
    
	//09. 05. 14 김정협 머지를 위해서 인덱스 읽는 부분 막음
	LUA_GET_VALUE_RETURN(		luaManager, "m_wstrDescription",	kSubMissionTemplet.m_wstrDescription,	L"",	goto LoadFail; );
	//LUA_GET_VALUE_RETURN(	luaManager, "m_iDescription_Index",			m_nString_Index,		0,		goto LoadFail; );	
	//kSubMissionTemplet.m_wstrDescription = GET_SCRIPT_STRING(m_nString_Index);

    LUA_GET_VALUE_RETURN_ENUM(	luaManager, "m_eClearType",		kSubMissionTemplet.m_eClearType,	TITLE_MISSION_CLEAR_TYPE,	TMCT_NONE,	goto LoadFail; );

    LUA_GET_VALUE_RETURN(		luaManager, "m_bAutomaticDescription",	kSubMissionTemplet.m_bAutomaticDescription,	true,		goto LoadFail; );


    if( LoadClearCondition( luaManager, kSubMissionTemplet ) == false )
        goto LoadFail;

    m_mapSubMission.insert( std::make_pair( kSubMissionTemplet.m_iSubMissionID, kSubMissionTemplet ) );

    return true;

LoadFail:
    return false;
}

const CX2TitleManager::SubMissionTemplet* CX2TitleManager::GetSubMissionInfo( int iSubMissionID )
{
    std::map< int, SubMissionTemplet >::const_iterator mit;
    mit = m_mapSubMission.find( iSubMissionID );
    if( mit == m_mapSubMission.end() )
        return NULL;

    return &mit->second;
}

bool CX2TitleManager::LoadClearCondition( KLuaManager& luaManager, SubMissionTemplet& kSubMissionTemplet )
{
    int iDifficulty;

    if( luaManager.BeginTable( "m_ClearCondition" ) == true )
    {
        switch( kSubMissionTemplet.m_eClearType )
        {
        case TMCT_NPC_TALK:
            {
                LUA_GET_VALUE_RETURN_ENUM(	luaManager, "m_eTalkNPCID",		kSubMissionTemplet.m_ClearCondition.m_eTalkNPCID,		CX2UnitManager::NPC_UNIT_ID,	CX2UnitManager::NUI_NONE, goto error_proc; );				
            }
            break;

        case TMCT_NPC_HUNT:
            {
                LUA_GET_VALUE_RETURN_ENUM(	luaManager, "m_eDungeonID",		kSubMissionTemplet.m_ClearCondition.m_eDungeonID,		SEnum::DUNGEON_ID,		SEnum::DI_NONE, goto error_proc; );
                LUA_GET_VALUE_RETURN(		luaManager, "m_cDifficulty",		iDifficulty,											-1, goto error_proc; );
                kSubMissionTemplet.m_ClearCondition.m_cDifficulty = static_cast<char>( iDifficulty );

				//{{ 2012. 1. 11	Merge 박세훈	2012.12.26 임규수 타이틀 미션 클리어 NPC 복수 적용
#ifdef SERV_SUB_TITLE_MISSION_PLURAL_NPC
				if( luaManager.BeginTable( "m_eKillNPCID" ) == true )
				{
					int index	= 1; 
					int buf		= -1;
					while( luaManager.GetValue( index, buf ) == true )
					{
						if( buf > 0 )
							kSubMissionTemplet.m_ClearCondition.m_setKillNPCID.insert( static_cast<CX2UnitManager::NPC_UNIT_ID>(buf) );
						index++;
					}

					luaManager.EndTable();
				}
#else
                LUA_GET_VALUE_RETURN_ENUM(	luaManager, "m_eKillNPCID",		kSubMissionTemplet.m_ClearCondition.m_eKillNPCID,		CX2UnitManager::NPC_UNIT_ID,	CX2UnitManager::NUI_NONE, goto error_proc; );
#endif SERV_SUB_TITLE_MISSION_PLURAL_NPC
				//}}
                LUA_GET_VALUE_RETURN(		luaManager, "m_iKillNum",			kSubMissionTemplet.m_ClearCondition.m_iKillNum,			0, goto error_proc; );

#ifdef NEW_HENIR_DUNGEON
				LUA_GET_VALUE_ENUM(			luaManager, "m_eDungeonMode",		kSubMissionTemplet.m_ClearCondition.m_eDungeonMode,		CX2Dungeon::DUNGEON_MODE,		CX2Dungeon::DM_INVALID );
#endif NEW_HENIR_DUNGEON

            }
            break;

        case TMCT_ITEM_COLLECTION:
            {				
                LUA_GET_VALUE_RETURN(		luaManager, "m_iCollectionItemID",	kSubMissionTemplet.m_ClearCondition.m_iCollectionItemID,	0, goto error_proc; );
                LUA_GET_VALUE_RETURN(		luaManager, "m_iCollectionItemNum",kSubMissionTemplet.m_ClearCondition.m_iCollectionItemNum,	0, goto error_proc; );
            }
            break;

        case TMCT_QUEST_ITEM_COLLECTION:
            {
                LUA_GET_VALUE_RETURN_ENUM(	luaManager, "m_eDungeonID",		kSubMissionTemplet.m_ClearCondition.m_eDungeonID,		SEnum::DUNGEON_ID,		SEnum::DI_NONE, goto error_proc; );
                LUA_GET_VALUE_RETURN(		luaManager, "m_cDifficulty",		iDifficulty,											-1, goto error_proc; );
                kSubMissionTemplet.m_ClearCondition.m_cDifficulty = static_cast<char>( iDifficulty );

				//{{ 2012. 1. 11	Merge 박세훈	2012.12.26 임규수 타이틀 미션 클리어 NPC 복수 적용
#ifdef SERV_SUB_TITLE_MISSION_PLURAL_NPC
				if( luaManager.BeginTable( "m_eKillNPCID" ) == true )
				{
					int index	= 1; 
					int buf		= -1;
					while( luaManager.GetValue( index, buf ) == true )
					{
						if( buf > 0 )
							kSubMissionTemplet.m_ClearCondition.m_setKillNPCID.insert( static_cast<CX2UnitManager::NPC_UNIT_ID>(buf) );
						index++;
					}

					luaManager.EndTable();
				}
#else
                LUA_GET_VALUE_RETURN_ENUM(	luaManager, "m_eKillNPCID",		kSubMissionTemplet.m_ClearCondition.m_eKillNPCID,		CX2UnitManager::NPC_UNIT_ID,	CX2UnitManager::NUI_NONE, goto error_proc; );
#endif SERV_SUB_TITLE_MISSION_PLURAL_NPC
				//}}
                LUA_GET_VALUE_RETURN(		luaManager, "m_iCollectionItemID",	kSubMissionTemplet.m_ClearCondition.m_iCollectionItemID,	0,		goto error_proc; );
                LUA_GET_VALUE_RETURN(		luaManager, "m_iCollectionItemNum",kSubMissionTemplet.m_ClearCondition.m_iCollectionItemNum,	0,		goto error_proc; );
                LUA_GET_VALUE_RETURN(		luaManager, "m_fQuestItemDropRate",kSubMissionTemplet.m_ClearCondition.m_fQuestItemDropRate,	0.0f,	goto error_proc; );
            }
            break;

        case TMCT_DUNGEON_TIME:
            {
                LUA_GET_VALUE_RETURN_ENUM(	luaManager, "m_eDungeonID",		kSubMissionTemplet.m_ClearCondition.m_eDungeonID,		SEnum::DUNGEON_ID,		    SEnum::DI_NONE, goto error_proc; );
                LUA_GET_VALUE_RETURN(		luaManager, "m_cDifficulty",		iDifficulty,											-1, goto error_proc; );
                kSubMissionTemplet.m_ClearCondition.m_cDifficulty = static_cast<char>( iDifficulty );

                LUA_GET_VALUE_RETURN(		luaManager, "m_iDungeonClearTime",	kSubMissionTemplet.m_ClearCondition.m_iDungeonClearTime,	0,		goto error_proc; );
				//{{ 2011. 05. 16  김민성	칭호 획득 조건 추가
#ifdef SERV_ADD_TITLE_CONDITION
				LUA_GET_VALUE(		luaManager, "m_iDungeonClearCount",	kSubMissionTemplet.m_ClearCondition.m_iDungeonClearCount,	1 );
#endif SERV_ADD_TITLE_CONDITION
				//}}

            }
            break;

        case TMCT_DUNGEON_RANK:
            {
                LUA_GET_VALUE_RETURN_ENUM(	luaManager, "m_eDungeonID",		kSubMissionTemplet.m_ClearCondition.m_eDungeonID,		SEnum::DUNGEON_ID,		SEnum::DI_NONE, goto error_proc; );
                LUA_GET_VALUE_RETURN(		luaManager, "m_cDifficulty",		iDifficulty,											-1, goto error_proc; );
                kSubMissionTemplet.m_ClearCondition.m_cDifficulty = static_cast<char>( iDifficulty );

                LUA_GET_VALUE_RETURN_ENUM(	luaManager, "m_eDungeonRank",		kSubMissionTemplet.m_ClearCondition.m_eDungeonRank,	CX2DungeonRoom::RANK_TYPE,		CX2DungeonRoom::RT_NONE, goto error_proc; );
				//{{ 2011. 05. 16  김민성	칭호 획득 조건 추가
#ifdef SERV_ADD_TITLE_CONDITION
				LUA_GET_VALUE(		luaManager, "m_iDungeonClearCount",	kSubMissionTemplet.m_ClearCondition.m_iDungeonClearCount,	1 );
#endif SERV_ADD_TITLE_CONDITION
				//}}
            }
            break;

        case TMCT_DUNGEON_DAMAGE:
            {
                LUA_GET_VALUE_RETURN_ENUM(	luaManager, "m_eDungeonID",		kSubMissionTemplet.m_ClearCondition.m_eDungeonID,		SEnum::DUNGEON_ID,		SEnum::DI_NONE, goto error_proc; );
                LUA_GET_VALUE_RETURN(		luaManager, "m_cDifficulty",		iDifficulty,											-1, goto error_proc; );
                kSubMissionTemplet.m_ClearCondition.m_cDifficulty = static_cast<char>( iDifficulty );

                LUA_GET_VALUE_RETURN(		luaManager, "m_iDungeonDamage",	kSubMissionTemplet.m_ClearCondition.m_iDungeonDamage,	-1, goto error_proc; );
				//{{ 2011. 05. 16  김민성	칭호 획득 조건 추가
#ifdef SERV_ADD_TITLE_CONDITION
				LUA_GET_VALUE(		luaManager, "m_iDungeonClearCount",	kSubMissionTemplet.m_ClearCondition.m_iDungeonClearCount,	1 );
#endif SERV_ADD_TITLE_CONDITION
				//}}
            }
            break;

        case TMCT_DUNGEON_CLEAR_COUNT:
            {
                LUA_GET_VALUE_RETURN_ENUM(	luaManager, "m_eDungeonID",		kSubMissionTemplet.m_ClearCondition.m_eDungeonID,		SEnum::DUNGEON_ID,		SEnum::DI_NONE, goto error_proc; );
				//{{ 2010. 08. 23  최육사	비밀던전 헬모드
#ifdef SERV_HELL_MODE_TITLE
				LUA_GET_VALUE_ENUM(			luaManager, "m_eDungeonMode",		kSubMissionTemplet.m_ClearCondition.m_eDungeonMode,		CX2Dungeon::DUNGEON_MODE,		CX2Dungeon::DM_INVALID );
#endif SERV_HELL_MODE_TITLE
				//}}
                LUA_GET_VALUE_RETURN(		luaManager, "m_cDifficulty",		iDifficulty,											-1, goto error_proc; );
                kSubMissionTemplet.m_ClearCondition.m_cDifficulty = static_cast<char>( iDifficulty );
				//{{ 2010. 08. 23  최육사	비밀던전 헬모드
#ifdef SERV_HELL_MODE_TITLE
				LUA_GET_VALUE(				luaManager, "m_bUpperDifficulty",	kSubMissionTemplet.m_ClearCondition.m_bUpperDifficulty,	false );
#endif SERV_HELL_MODE_TITLE
				//}}
                LUA_GET_VALUE_RETURN(		luaManager, "m_iDungeonClearCount",	kSubMissionTemplet.m_ClearCondition.m_iDungeonClearCount,	-1, goto error_proc; );
            }
            break;

        case TMCT_PVP_PLAY:
            {
                LUA_GET_VALUE_RETURN_ENUM(	luaManager, "m_ePVPType",			kSubMissionTemplet.m_ClearCondition.m_ePVPType,		CX2PVPRoom::PVP_GAME_TYPE,		CX2PVPRoom::PGT_TEAM, goto error_proc; );
                LUA_GET_VALUE_RETURN(		luaManager, "m_iPVPPlay",			kSubMissionTemplet.m_ClearCondition.m_iPVPPlay,		-1, goto error_proc; );
            }
            break;

        case TMCT_PVP_WIN:
            {
                LUA_GET_VALUE_RETURN_ENUM(	luaManager, "m_ePVPType",			kSubMissionTemplet.m_ClearCondition.m_ePVPType,		CX2PVPRoom::PVP_GAME_TYPE,		CX2PVPRoom::PGT_TEAM, goto error_proc; );
                LUA_GET_VALUE_RETURN(		luaManager, "m_iPVPWin",			kSubMissionTemplet.m_ClearCondition.m_iPVPWin,		-1, goto error_proc; );
            }
            break;

        case TMCT_PVP_KILL:
            {
                LUA_GET_VALUE_RETURN_ENUM(	luaManager, "m_ePVPType",			kSubMissionTemplet.m_ClearCondition.m_ePVPType,		CX2PVPRoom::PVP_GAME_TYPE,		CX2PVPRoom::PGT_TEAM, goto error_proc; );
                LUA_GET_VALUE_RETURN(		luaManager, "m_iPVPKill",			kSubMissionTemplet.m_ClearCondition.m_iPVPKill,		-1, goto error_proc; );
            }
            break;

        case TMCT_QUEST:
            {
                LUA_GET_VALUE_RETURN(		luaManager, "m_iQuestID",			kSubMissionTemplet.m_ClearCondition.m_iQuestID,		-1, goto error_proc; );
            }
            break;

        case TMCT_COLLECT_TITLE:
            {
                LUA_GET_VALUE_RETURN(		luaManager, "m_iTitleID",			kSubMissionTemplet.m_ClearCondition.m_iTitleID,		-1, goto error_proc; );
            }
            break;

        case TMCT_USE_ITEM:
            {
                LUA_GET_VALUE_RETURN(		luaManager, "m_iItemID",			kSubMissionTemplet.m_ClearCondition.m_iItemID,		-1, goto error_proc; );
            }
            break;

		case TMCT_NONE:
			{
				// 아케이드 랭킹 보상임
			}
			break;

			//{{ 2010. 05. 17  최육사	대전 던전 서버군 통합
#ifdef SERV_INTEGRATION
		case TMCT_WITH_DIF_SERV_USER:
			{
				LUA_GET_VALUE_RETURN_ENUM(	luaManager, "m_eDungeonID",		kSubMissionTemplet.m_ClearCondition.m_eDungeonID,		SEnum::DUNGEON_ID,		SEnum::DI_NONE, goto error_proc; );
				LUA_GET_VALUE_RETURN(		luaManager, "m_cDifficulty",		iDifficulty,											-1, goto error_proc; );
				kSubMissionTemplet.m_ClearCondition.m_cDifficulty = static_cast<char>( iDifficulty );

				LUA_GET_VALUE_RETURN(		luaManager, "m_iDungeonClearCount",	kSubMissionTemplet.m_ClearCondition.m_iDungeonClearCount,	-1, goto error_proc; );
			}
			break;
#endif SERV_INTEGRATION
			//}}		
			//{{ 2011. 05. 16  김민성	칭호 획득 조건 추가
#ifdef SERV_ADD_TITLE_CONDITION
		case TMCT_GIVE_PET_FEED:
			{
				LUA_GET_VALUE_RETURN_ENUM(	luaManager, "m_eItemGrade",		kSubMissionTemplet.m_ClearCondition.m_eItemGrade,		CX2Item::ITEM_GRADE,		CX2Item::IG_NONE, goto error_proc; );
				LUA_GET_VALUE_RETURN(		luaManager, "m_iPetFeedCount",		kSubMissionTemplet.m_ClearCondition.m_iPetFeedCount,	-1, goto error_proc; );
			}
			break;
		case TMCT_PLAYER_WITH_DUNGEON_CLEAR:
			{
				LUA_GET_VALUE_RETURN_ENUM(	luaManager, "m_eDungeonID",			kSubMissionTemplet.m_ClearCondition.m_eDungeonID,			SEnum::DUNGEON_ID,		SEnum::DI_NONE, goto error_proc; );
				LUA_GET_VALUE_RETURN(		luaManager, "m_cDifficulty",			iDifficulty,												-1,					goto error_proc; );
				LUA_GET_VALUE(				luaManager, "m_bUpperDifficulty",	kSubMissionTemplet.m_ClearCondition.m_bUpperDifficulty,	false );
				LUA_GET_VALUE_RETURN(		luaManager, "m_iPlayerCount",			kSubMissionTemplet.m_ClearCondition.m_iPlayerCount,			-1,					goto error_proc; );
				LUA_GET_VALUE_RETURN(		luaManager, "m_iDungeonClearTime",		kSubMissionTemplet.m_ClearCondition.m_iDungeonClearTime,	-1,					goto error_proc; );
				LUA_GET_VALUE_RETURN(		luaManager, "m_iDungeonClearCount",	kSubMissionTemplet.m_ClearCondition.m_iDungeonClearCount,	-1,					goto error_proc; );
			}
			break;
		case TMCT_RESURRECTION_STONE:
			{
				LUA_GET_VALUE_RETURN_ENUM(	luaManager, "m_eDungeonID",			kSubMissionTemplet.m_ClearCondition.m_eDungeonID,			SEnum::DUNGEON_ID,		SEnum::DI_NONE, goto error_proc; );
				LUA_GET_VALUE_RETURN(		luaManager, "m_cDifficulty",			iDifficulty,												-1,					goto error_proc; );
				LUA_GET_VALUE(				luaManager, "m_bUpperDifficulty",	kSubMissionTemplet.m_ClearCondition.m_bUpperDifficulty,	false );
				LUA_GET_VALUE_RETURN(		luaManager, "m_iDungeonClearCount",	kSubMissionTemplet.m_ClearCondition.m_iDungeonClearCount,	-1,					goto error_proc; );
				LUA_GET_VALUE(				luaManager, "m_bCheckResurrectionStone",	kSubMissionTemplet.m_ClearCondition.m_bCheckResurrectionStone,	false );
			}
			break;
		case TMCT_TOGETHER_MISSION_CLEAR:
			{
				LUA_GET_VALUE_RETURN(		luaManager, "m_iDungeonClearCount",	kSubMissionTemplet.m_ClearCondition.m_iDungeonClearCount,	-1,					goto error_proc; );
			}
			break;
		case TMCT_USER_UNIT_DIE:
			{
				LUA_GET_VALUE_RETURN_ENUM(	luaManager, "m_eDieReason",	kSubMissionTemplet.m_ClearCondition.m_eDieReason,	KEGS_USER_UNIT_DIE_REQ::USER_UNIT_DIE_REASON,	KEGS_USER_UNIT_DIE_REQ::UUDR_UNKNOWN,	goto error_proc; );
				LUA_GET_VALUE_RETURN(		luaManager, "m_iDieCount",		kSubMissionTemplet.m_ClearCondition.m_iDungeonClearCount,	-1,					goto error_proc; );
			}
			break;
#endif SERV_ADD_TITLE_CONDITION
			//}}
#ifdef SERV_ADD_TITLE_CONDITION_2013_08 //굳이 int형 변수를 하나 더 만들지 않고 m_iDungeonClearCount변수를 활용.
		case TMCT_ITEM_SOCKET:
			{
				LUA_GET_VALUE_RETURN(		luaManager, "m_iItemSocketCount",	kSubMissionTemplet.m_ClearCondition.m_iDungeonClearCount,	-1,					goto error_proc; );
			}
			break;
		case TMCT_ITEM_ENCHANT_COUNT:
			{
				LUA_GET_VALUE_RETURN(		luaManager, "m_iItemEnchantCount",	kSubMissionTemplet.m_ClearCondition.m_iDungeonClearCount,	-1,					goto error_proc; );
			}
			break;
		case TMCT_ITEM_ATTRIB:
			{
				LUA_GET_VALUE_RETURN(		luaManager, "m_iItemAttribCount",	kSubMissionTemplet.m_ClearCondition.m_iDungeonClearCount,	-1,					goto error_proc; );
			}
			break;
		case TMCT_ITEM_RESOLVE:
			{
				LUA_GET_VALUE_RETURN(		luaManager, "m_iItemResolveCount",	kSubMissionTemplet.m_ClearCondition.m_iDungeonClearCount,	-1,					goto error_proc; );
			}
			break;
#endif //SERV_ADD_TITLE_CONDITION_2013_08

        default:
            {
                luaManager.EndTable();
                return false;
            }
        }

        luaManager.EndTable();
        return true;
    }

error_proc:
    luaManager.EndTable();
    return false;
}



const CX2TitleManager::TitleInfo* CX2TitleManager::GetTitleInfo( int iTitleID )
{
	std::map< int, TitleInfo* >::const_iterator mit;
	mit = m_mapTitleInfo.find( iTitleID );
	if( mit == m_mapTitleInfo.end() )
		return NULL;

	return mit->second;
}

bool CX2TitleManager::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    switch(wParam)
    {
        case TMUI_CLOSE:
            CloseTitle();
            return true;
        case TMUI_LIST_SELECT:                        
            if(m_pDlgTitle != NULL && m_pDlgTitle->GetShow() == true)
            {
                int iTitleId = m_pDlgTitle->GetSelectedTitle();

				if(iTitleId == 0)
				{
					m_pDlgTitle->ClearTitle();
				}    
				if(iTitleId != m_iSelTitleId)
				{
					m_iSelTitleId = iTitleId;
					SelectTitle(iTitleId);  
				}
            }
            return true;
        case TMUI_ATTACH:
            if(m_pDlgTitle != NULL && m_pDlgTitle->GetShow() == true)
            {
				//{{ kimhc // 실시간 엘소드 중 던전내에서 유저가 죽었거나, wait, mpCharge 상태가 아닐때 칭호 교체 막기
#ifdef	REAL_TIME_ELSWORD
				if ( g_pMain->GetNowStateID() == CX2Main::XS_DUNGEON_GAME )
				{
					if ( g_pX2Game == NULL || g_pX2Game->GetMyUnit() == NULL )
						return false;

					if ( g_pX2Game->GetMyUnit()->GetNowHp() <= 0 )
					{
						g_pMain->KTDGUIOKMsgBox ( D3DXVECTOR2(-999,-999), GET_STRING( STR_ID_2691 ),g_pMain->GetNowState() );

						return false;
					}

					if ( g_pX2Game->GetMyUnit()->GetFutureStateID() != g_pX2Game->GetMyUnit()->GetWaitStateID() &&
#ifdef RIDING_SYSTEM //JHKang
						g_pX2Game->GetMyUnit()->GetFutureStateID() != g_pX2Game->GetMyUnit()->GetRidingWaitStateID() &&
						g_pX2Game->GetMyUnit()->GetFutureStateID() != g_pX2Game->GetMyUnit()->GetRidingWaitHabitStateID() &&
#endif //RIDING_SYSTEM
						g_pX2Game->GetMyUnit()->GetFutureStateID() != g_pX2Game->GetMyUnit()->GetChargeMpStateID() )
						return false;
				}
#endif	REAL_TIME_ELSWORD
				//{{ kimhc // 실시간 엘소드 중 던전내에서 유저가 죽었거나, wait, mpCharge 상태가 아닐때 칭호 교체 막기

                int iTitleId = m_pDlgTitle->GetSelectedTitle();

				if(iTitleId == 0)
				{
					m_pDlgTitle->ClearTitle();
				}				
				if(iTitleId != m_iSelTitleId)
				{
					m_iSelTitleId = iTitleId;
					SelectTitle(iTitleId);  
				}

				if( g_pData->GetMyUser()->GetSelectUnit()->GetTitleId() != iTitleId )
				{
					if(iTitleId > 0)
					{
						if(m_pDlgTitle != NULL)
						{
							if(m_pDlgTitle->GetTitleState(iTitleId) == 0)
							{
								Handler_EGS_EQUIP_TITLE_REQ(iTitleId);
							}
							else
							{
								g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250, 300), GET_STRING( STR_ID_758 ), (CKTDXStage*) g_pMain->GetNowState() );
							}
                        
						}                    
					}
					else if(iTitleId == 0)
					{
						// 칭호해제
						DetachTitle();
					}
				}
            }
            return true;
        case TMUI_CHECK_TAKE:
            if(m_pDlgTitle != NULL && m_pDlgTitle->GetShow() == true)
            {
                m_pDlgTitle->SetCheckTake();
            }
            return true;
        case TMUI_TAB:
            if(m_pDlgTitle != NULL && m_pDlgTitle->GetShow() == true)
            {
                int iGroup = 0;
                CKTDGUIControl* pControl = (CKTDGUIControl*)lParam;
                iGroup = pControl->GetDummyInt( 0 );

				if( iGroup >= 0 )
					ChangeTitleGroup(iGroup);
            }
            return true;
    }

    return false;
}

bool CX2TitleManager::UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    switch( wParam)
    {
    case EGS_EQUIP_TITLE_ACK:
        return Handler_EGS_EQUIP_TITLE_ACK( hWnd, uMsg, wParam, lParam );
    case EGS_UPDATE_MISSION_NOT:
        return Handler_EGS_UPDATE_MISSION_NOT( hWnd, uMsg, wParam, lParam );
    case EGS_NEW_MISSION_NOT:
        return Handler_EGS_NEW_MISSION_NOT( hWnd, uMsg, wParam, lParam );
    case EGS_REWARD_TITLE_NOT:
        return Handler_EGS_REWARD_TITLE_NOT( hWnd, uMsg, wParam, lParam );
    case EGS_TITLE_EXPIRATION_NOT:
        return Handler_EGS_TITLE_EXPIRATION_NOT( hWnd, uMsg, wParam, lParam );
    }

    return false;
}

void CX2TitleManager::ChangeTitleGroup(int iGroup)
{
    if(m_pDlgTitle != NULL && m_pDlgTitle->GetShow() == true)
    {
        m_pDlgTitle->ChangeTitleGroup(iGroup);
    }    
}

bool CX2TitleManager::GetShow()
{
    if(m_pDlgTitle != NULL)
    {
        return m_pDlgTitle->GetShow();        
    }

    return false;
}

void CX2TitleManager::OpenTitle()
{
    if(m_pDlgTitle != NULL)
    {
        //InitRecord();
        // 기간제 검사
        if(m_pDlgTitle != NULL && GetShow() == true)
        {
            m_pDlgTitle->CheckPeriod();
        }    

        if(g_pData != NULL && g_pData->GetMyUser() != NULL)
        {
            m_iSelTitleId = g_pData->GetMyUser()->GetSelectUnit()->GetTitleId();               
        }
        else
            m_iSelTitleId = 0;
        
        m_bNewTitle = false;
		
        m_pDlgTitle->OpenTitle();        

        if(m_iSelTitleId > 0)
        {
            m_pDlgTitle->SetSelectTitle(m_iSelTitleId);
            SelectTitle(m_iSelTitleId);
        }
    }
}

void CX2TitleManager::CloseTitle()
{
    if(m_pDlgTitle != NULL)
    {
//#if 0 // 칭호 프리뷰
//        if(m_hSeqEmblem != INVALID_PARTICLE_SEQUENCE_HANDLE)
//        {            
//            CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqEmblem );
//            if(pSeq != NULL)
//                pSeq->SetShowObject(false);            
//
//            g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hSeqEmblem );
//            m_pPart_Emblem_200 = NULL;
//        }        
//#endif

        m_bProcess = false;

        m_pDlgTitle->CloseTitle();
    }
}

void CX2TitleManager::InitRecord()
{
    CX2TitleSystem::TitleRecord record;

    if(m_pDlgTitle != NULL)
    {
        m_pDlgTitle->ClearRecord();

        TitleInfo *titleInfo;
        std::map< int, TitleInfo* >::iterator it;
        for (it=m_mapTitleInfo.begin(); it!=m_mapTitleInfo.end(); ++it) 
        {
            titleInfo = it->second;

            record.m_iTitleID       = titleInfo->m_iTitleID;
            record.m_iSort          = titleInfo->m_iSortNum;
            record.m_eTitleType     = titleInfo->m_eTitleType;
            record.m_wstrTitleName  = titleInfo->m_wstrTitleName;
            record.m_wstrTitleDesc  = titleInfo->m_wstrDescription;            
            record.m_nSecret        = 2;    // 습득/습득가능/???       

            if(titleInfo->m_bVisible == false || titleInfo->m_bIsSecretTitle == true)
                record.m_bSecret = true;        
            else
                record.m_bSecret = false;

            m_pDlgTitle->AddRecord(record);
        }
        
        m_pDlgTitle->SortRecord();
    }

    m_bInit = true;
}

bool CX2TitleManager::SelectTitle(int val)
{
    std::map< int, TitleInfo* >::iterator it;

    it = m_mapTitleInfo.find(val);
    if(m_mapTitleInfo.end() == it)
        return false;

    TitleInfo *titleInfo = NULL;
    titleInfo = it->second;

    if(titleInfo == NULL)
        return false;

    // 타이틀 프리뷰
    int titleId = titleInfo->m_iTitleID;

    int iState = 2;
    if(m_pDlgTitle != NULL)
    {
        iState = m_pDlgTitle->GetTitleState(titleId);
    }

#if 1 // 칭호 2d이미지 프리뷰
	if( titleId > 0)
	{
		wstring titleName;

		// 2d 이미지 이름을 얻어온다.
		// 칭호 파티클 이름은 2d이미지 이름과 동일!
		if(iState == 2)
			titleName = L"Title_Empty";
		else
		{
#ifdef SERV_GROW_UP_TITLE
			// NULL 체크 할필요 없어 보이지만...
			if( g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetSelectUnit() != NULL )
			{
				int iLevel = g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().GetGrowUpLevelByTitle( titleId );
				titleName = g_pData->GetTitleManager()->GetTitleModel( titleId, iLevel );
				if( titleName == L"")
					titleName = L"Title_Empty";
			}
			else
				titleName = L"Title_Empty";
#else
			titleName = g_pData->GetTitleManager()->GetTitleModel(titleId);
#endif
		}
		titleName += L".tga";
		
		if(m_pDlgTitle != NULL)
			m_pDlgTitle->SetPreviewTitle(titleName);		
	}
#endif

//#if 0 // // 칭호 파티클 프리뷰
//    if( titleId > 0 )
//    {   
//        if(m_hSeqEmblem != INVALID_PARTICLE_SEQUENCE_HANDLE)
//            g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hSeqEmblem );
//        m_pPart_Emblem_200 = NULL;
//
//        if( m_hSeqEmblem == INVALID_PARTICLE_SEQUENCE_HANDLE )
//        {
//            wstring titleName;
//            
//            if(iState == 2)
//                titleName = L"TITLE_SECRET";
//            else
//                titleName = g_pData->GetTitleManager()->GetTitleModel(titleId);
//            m_hSeqEmblem = g_pData->GetUIMajorParticle()->CreateSequenceHandle( NULL,  titleName.c_str(), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f );            
//        }
//
//        if( m_hSeqEmblem != INVALID_PARTICLE_SEQUENCE_HANDLE )
//        {
//            if( m_pPart_Emblem_200 == NULL )
//            {
//                CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqEmblem );
//                if(pSeq != NULL)
//                    m_pPart_Emblem_200 = pSeq->CreateNewParticle( D3DXVECTOR3(0.0f,0.0f,0.0f) );
//                //CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqEmblem );
//                //if(pSeq != NULL)
//                //    pSeq->SetShow(false);
//            }
//        }
//        
//        if(m_pPart_Emblem_200)
//            m_pPart_Emblem_200->SetPos( D3DXVECTOR3(-90, 195, 1) );
//        
//        
//        //m_pPart_Emblem_200->SetSize( D3DXVECTOR3(1.3f, 1.3f, 1.0f) );
//
//        CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqEmblem );
//        if(pSeq != NULL)
//        {		
//			pSeq->SetShowObject(true);        
//            pSeq->SetOverUI(true);
//        }
//    }
//    else
//    {
//        if(m_hSeqEmblem != INVALID_PARTICLE_SEQUENCE_HANDLE)
//        {
//            CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hSeqEmblem );
//            if(pSeq != NULL)
//                pSeq->SetShowObject(false);          
//            g_pData->GetUIMajorParticle()->DestroyInstanceHandle( m_hSeqEmblem );
//            m_pPart_Emblem_200 = NULL;
//        }        
//    }
//#endif
	
    return true;
}

bool CX2TitleManager::AttachTitle(int val)
{
	int iTitleID = 0;
    if(val > 0)
    {
        std::map< int, TitleInfo* >::iterator it;

        it = m_mapTitleInfo.find(val);
        if(m_mapTitleInfo.end() == it)
            return false;

        TitleInfo *titleInfo = NULL;
        titleInfo = it->second;

        if(titleInfo == NULL)
            return false;

		iTitleID = titleInfo->m_iTitleID;
        g_pData->GetMyUser()->GetSelectUnit()->SetTitleId(titleInfo->m_iTitleID);

        if(m_pDlgTitle != NULL)
        {
            m_strDesc = titleInfo->m_wstrTitleName;
			
			if(iTitleID == 0)
				m_strImgName = L"NoAlphaImage.dds";
			else
			{
#ifdef SERV_GROW_UP_TITLE
				// 해당 타이틀이 갖고 있는 소켓의 타입을 이용하여 레벨을 찾아내도록 수정하였음 by 박진웅
				int iLevel = g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().GetGrowUpLevelByTitle( iTitleID );
				m_strImgName = g_pData->GetTitleManager()->GetTitleModel( iTitleID, iLevel );
				if( m_strImgName == L"" )
					m_strImgName = L"NoAlphaImage.dds";
				else
#else
				m_strImgName = g_pData->GetTitleManager()->GetTitleModel(iTitleID);
#endif //SERV_GROW_UP_TITLE
				m_strImgName += L".tga";
			}
        }
    }
    else
    {
        if(m_pDlgTitle != NULL)
        {
            g_pData->GetMyUser()->GetSelectUnit()->SetTitleId(0);
            m_strDesc = GET_STRING( STR_ID_5378 );
			m_strImgName = L"NoAlphaImage.dds";
        }
    }    

	if(g_pData != NULL && g_pData->GetUIManager() != NULL && g_pData->GetUIManager()->GetUICharInfo() != NULL)
	{
		g_pData->GetUIManager()->GetUICharInfo()->SetTitle();
		g_pData->GetUIManager()->GetUICharInfo()->ResetStat();

	}

    //if(g_pData != NULL && g_pData->GetCharacterRoom() != NULL)
    //{
    //    g_pData->GetCharacterRoom()->SetTitleName();
    //    g_pData->GetCharacterRoom()->ResetStat();
    //}
    

    CloseTitle();

    return true;
}

void CX2TitleManager::DetachTitle()
{
    if(m_bProcess == true)
        return;
    
    m_strDesc = GET_STRING( STR_ID_5378 ); //L"";
	m_strImgName = L"";
    Handler_EGS_EQUIP_TITLE_REQ(0);   
}

wstring CX2TitleManager::GetTitleModel(int val)
{
    std::map< int, TitleInfo* >::iterator it;

    it = m_mapTitleInfo.find(val);
    if(m_mapTitleInfo.end() == it)
        return L"";

    TitleInfo *titleInfo = it->second;

    if(titleInfo == NULL)
        return L"";

    return titleInfo->m_wstrParticleName;
}

#ifdef SERV_GROW_UP_TITLE
wstring CX2TitleManager::GetTitleModel( int val_, int iLevel_ )
{
	std::map< int, TitleInfo* >::iterator it;

	it = m_mapTitleInfo.find( val_ );
	if( m_mapTitleInfo.end() == it )
		return L"";

	TitleInfo *titleInfo = it->second;

	if( titleInfo == NULL )
		return L"";

	if( titleInfo->m_eTitleType != TT_GROW_UP )
		return titleInfo->m_wstrParticleName;

	if( 0 == iLevel_ )
		return titleInfo->m_wstrParticleName;
	else
	{
		std::map<int, std::wstring>::iterator mit = titleInfo->m_mapGrowUpParticleName.find( iLevel_ );
		if( mit != titleInfo->m_mapGrowUpParticleName.end() )
			return mit->second;
		else
			return L"";
	}
}
#endif //SERV_GROW_UP_TITLE

wstring CX2TitleManager::GetMIssionDesc(int missionId)
{
    std::map< int, MissionTemplet >::iterator it;

    it = m_mapTitleMission.find(missionId);
    if(m_mapTitleMission.end() == it)
        return L"";

    wstring wstrMissionDesc;
    wstring wstrSubMissionDesc;
    MissionTemplet missionInfo = it->second;

    //wstrMissionDesc = missionInfo.m_wstrMissionName;
    wstrMissionDesc = L"";

    for(int i=0; i<(int)missionInfo.m_vecSubMission.size(); ++i)
    {
        wstrSubMissionDesc = GetSubMissionDesc(missionInfo.m_vecSubMission[i]);
        wstrMissionDesc += L"\n";
        wstrMissionDesc += wstrSubMissionDesc;
    }

    wstrMissionDesc += L"\n";

    return wstrMissionDesc;
}

wstring CX2TitleManager::GetSubMissionDesc(int subMissionId)
{
    std::map< int, SubMissionTemplet >::iterator sit;

    sit = m_mapSubMission.find(subMissionId);
    if(m_mapSubMission.end() == sit)
        return L"";

    wstring wstrDesc;
    SubMissionTemplet nodeSub = sit->second;
    
    bool bSuccess = nodeSub.m_ClearCondition.m_bSuccess;
    wstrDesc = nodeSub.m_wstrDescription;
    if(bSuccess)
    {
        wstrDesc += L" (";
        wstrDesc += GET_STRING( STR_ID_759 );
        wstrDesc += L")";
        return wstrDesc;
    }

    ClearCondition cd = nodeSub.m_ClearCondition;

	WCHAR descBuf[256] = {0,};

    switch(nodeSub.m_eClearType)
    {
    case TMCT_NONE:
        return wstrDesc;
    case TMCT_NPC_TALK: 
        //wsprintf(descBuf, L" (%s)", g_pData->GetUnitManager()->GetNPCUnitTemplet(cd.m_eTalkNPCID)->m_Name.c_str());
        StringCchPrintf(descBuf, 256, L" (%s)", g_pData->GetUnitManager()->GetNPCUnitTemplet(cd.m_eTalkNPCID)->m_Name.c_str());
		wstrDesc += descBuf;
        return wstrDesc; 
    case TMCT_NPC_HUNT:    
        //wsprintf(descBuf, L" (%d/%d)", cd.m_nCount, cd.m_iKillNum);
		StringCchPrintf(descBuf, 256, L" (%d/%d)", cd.m_nCount, cd.m_iKillNum);
        wstrDesc += descBuf;
        return wstrDesc;    
        //m_eDungeonID
        //m_eKillNPCID
        //m_cDifficulty
        //m_iKillNum
    case TMCT_ITEM_COLLECTION:
        //wsprintf(descBuf, L" (%d/%d)", cd.m_nCount, cd.m_iCollectionItemNum);
		StringCchPrintf(descBuf, 256, L" (%d/%d)", cd.m_nCount, cd.m_iCollectionItemNum);
        wstrDesc += descBuf;
        return wstrDesc;   
        //m_iCollectionItemID
        //m_iCollectionItemNum
    case TMCT_QUEST_ITEM_COLLECTION:
        //wsprintf(descBuf, L" (%d/%d)", cd.m_nCount, cd.m_iCollectionItemNum);
		StringCchPrintf(descBuf, 256, L" (%d/%d)", cd.m_nCount, cd.m_iCollectionItemNum);
        wstrDesc += descBuf;
        return wstrDesc;   
        //m_eDungeonID
        //m_cDifficulty
        //m_eKillNPCID
        //m_iCollectionItemID
        //m_iCollectionItemNum
    case TMCT_DUNGEON_TIME:
		//{{ 2011. 05. 16  김민성	칭호 획득 조건 추가
#ifdef SERV_ADD_TITLE_CONDITION
		if( cd.m_iDungeonClearCount > 1 )  // 기존의 형식을 최대한 유지한다.
		{
			StringCchPrintf(descBuf, 256, L" (%d/%d)", cd.m_nCount, cd.m_iDungeonClearCount);
			wstrDesc += descBuf;
		}
#endif SERV_ADD_TITLE_CONDITION
		//}}
        return wstrDesc; 
        //m_eDungeonID
        //m_cDifficulty
        //m_iDungeonClearTime
    case TMCT_DUNGEON_RANK:
		//{{ 2011. 05. 16  김민성	칭호 획득 조건 추가
#ifdef SERV_ADD_TITLE_CONDITION
		if( cd.m_iDungeonClearCount > 1 )
		{
			StringCchPrintf(descBuf, 256, L" (%d/%d)", cd.m_nCount, cd.m_iDungeonClearCount);
			wstrDesc += descBuf;
		}
#endif SERV_ADD_TITLE_CONDITION
		//}}
        return wstrDesc; 
        //m_eDungeonID
        //m_cDifficulty
        //m_eDungeonRank
    case TMCT_DUNGEON_DAMAGE:
		//{{ 2011. 05. 16  김민성	칭호 획득 조건 추가
#ifdef SERV_ADD_TITLE_CONDITION
		if( cd.m_iDungeonClearCount > 1 )
		{
			StringCchPrintf(descBuf, 256, L" (%d/%d)", cd.m_nCount, cd.m_iDungeonClearCount);
			wstrDesc += descBuf;
		}
#endif SERV_ADD_TITLE_CONDITION
		//}}
		return wstrDesc;
        //m_eDungeonID
        //m_cDifficulty
        //m_iDungeonDamage
        break;
    case TMCT_DUNGEON_CLEAR_COUNT:
        //wsprintf(descBuf, L" (%d/%d)", cd.m_nCount, cd.m_iDungeonClearCount);
		StringCchPrintf(descBuf, 256, L" (%d/%d)", cd.m_nCount, cd.m_iDungeonClearCount);
        wstrDesc += descBuf;
        return wstrDesc;
        //m_eDungeonID
        //m_cDifficulty
        //m_iDungeonClearCount
    case TMCT_PVP_PLAY:
        return wstrDesc; 
        //m_ePVPType
        //m_iPVPPlay
    case TMCT_PVP_WIN:
        return wstrDesc; 
        //m_ePVPType
        //m_iPVPWin
    case TMCT_PVP_KILL:
        return wstrDesc; 
        //m_ePVPType
        //m_iPVPKill
    case TMCT_QUEST:
        return wstrDesc; 
        //m_iQuestID
    case TMCT_COLLECT_TITLE:
        return wstrDesc; 
        //m_iTitleID
    case TMCT_USE_ITEM:
        return wstrDesc; 
        //m_iItemID
#ifdef SERV_INTEGRATION
//{{ oasis907 : 김상윤 [2010.5.18] // 대전 던전 서버군 통합
	case TMCT_WITH_DIF_SERV_USER:
		//wsprintf(descBuf, L" (%d/%d)", cd.m_nCount, cd.m_iDungeonClearCount);
		StringCchPrintf(descBuf, 256, L" (%d/%d)", cd.m_nCount, cd.m_iDungeonClearCount);
		wstrDesc += descBuf;
		return wstrDesc;
		//m_eDungeonID
		//m_cDifficulty
		//m_iDungeonClearCount
//}}
#endif SERV_INTEGRATION
		//{{ 2011. 05. 16  김민성	칭호 획득 조건 추가
#ifdef SERV_ADD_TITLE_CONDITION
	case TMCT_GIVE_PET_FEED:
		StringCchPrintf(descBuf, 256, L" (%d/%d)", cd.m_nCount, cd.m_iPetFeedCount);
		wstrDesc += descBuf;
		return wstrDesc;
	case TMCT_PLAYER_WITH_DUNGEON_CLEAR:
	case TMCT_RESURRECTION_STONE:
	case TMCT_TOGETHER_MISSION_CLEAR:
		if( cd.m_iDungeonClearCount > 1 )
		{
			StringCchPrintf(descBuf, 256, L" (%d/%d)", cd.m_nCount, cd.m_iDungeonClearCount);
			wstrDesc += descBuf;
		}
		return wstrDesc;
	case TMCT_USER_UNIT_DIE:
		if( cd.m_iDungeonClearCount > 1 )
		{
			StringCchPrintf(descBuf, 256, L" (%d/%d)", cd.m_nCount, cd.m_iDungeonClearCount);
			wstrDesc += descBuf;
		}
		return wstrDesc;
#endif SERV_ADD_TITLE_CONDITION
			//}}
#ifdef SERV_ADD_TITLE_CONDITION_2013_08		// 적용날짜: 2013-08-13
	case TMCT_ITEM_SOCKET:
		{
			StringCchPrintf(descBuf, 256, L" (%d/%d)", cd.m_nCount, cd.m_iDungeonClearCount );
			wstrDesc += descBuf;
		}
		return wstrDesc;
	case TMCT_ITEM_ENCHANT_LEVEL:
		return wstrDesc;
	case TMCT_ITEM_ENCHANT_COUNT:
		{
			StringCchPrintf(descBuf, 256, L" (%d/%d)", cd.m_nCount, cd.m_iDungeonClearCount );
			wstrDesc += descBuf;
		}
		return wstrDesc;
	case TMCT_ITEM_ATTRIB:
		{
			StringCchPrintf(descBuf, 256, L" (%d/%d)", cd.m_nCount, cd.m_iDungeonClearCount );
			wstrDesc += descBuf;
		}
		return wstrDesc;
	case TMCT_ITEM_RESOLVE:
		{
			StringCchPrintf(descBuf, 256, L" (%d/%d)", cd.m_nCount, cd.m_iDungeonClearCount );
			wstrDesc += descBuf;
		}
		return wstrDesc;
#endif // SERV_ADD_TITLE_CONDITION_2013_08
    }


    return L"";
}

void CX2TitleManager::TakeTitle(std::vector<KTitleInfo> vecTitleInfo)
{
    CX2TitleSystem::TitleRecord record;
    
    if(m_pDlgTitle != NULL)
    {
        int iTitleId;
        KTitleInfo nodeTitle;       

        for(int i=0; i<(int)vecTitleInfo.size(); ++i)
        {
            nodeTitle = vecTitleInfo[i];
            iTitleId = nodeTitle.m_iTitleID;

            UpdateTitle(nodeTitle, 0);
            //UpdateTitle(iTitleId, 0, nodeTitle.m_bInfinity);
        }        
    }
}

bool CX2TitleManager::UpdateTitle(KTitleInfo ktitleInfo, int iState)
{
    CX2TitleSystem::TitleRecord record;
    TitleInfo *titleInfo;    
    std::map< int, TitleInfo* >::iterator tit;
    int iTitleId = ktitleInfo.m_iTitleID;

    if(iState < 0 || iState > 2)
        return false;

    if(m_pDlgTitle != NULL)
    {
        tit = m_mapTitleInfo.find(iTitleId);
        if(m_mapTitleInfo.end() == tit)
            return false;

        titleInfo = tit->second;

        if(titleInfo->m_bVisible == false)
            return false;

        record.m_iTitleID       = titleInfo->m_iTitleID;
        record.m_iSort          = titleInfo->m_iSortNum;
        record.m_eTitleType     = titleInfo->m_eTitleType;
        record.m_wstrTitleName  = titleInfo->m_wstrTitleName;
        record.m_wstrTitleDesc  = titleInfo->m_wstrDescription;            
        record.m_nSecret        = iState;    // 습득/습득가능/???       
        //record.m_bSecret        = titleInfo->m_bIsSecretTitle;  
        record.m_wstrEndDate    = ktitleInfo.m_wstrEndDate;
        record.m_periodTime     = ktitleInfo.m_kCTime;        
        record.m_bPeriod        = !ktitleInfo.m_bInfinity;
        
        if(titleInfo->m_bVisible == false || titleInfo->m_bIsSecretTitle == true)
            record.m_bSecret = true;        
        else
            record.m_bSecret = false;

        if(iState == 0)
            record.m_bSecret = false;

        m_pDlgTitle->UpdateRecord(record);
    }

    return true;
}

bool CX2TitleManager::UpdateTitle(int iTitleId, int iState, bool bInfinity)
{
    CX2TitleSystem::TitleRecord record;
    TitleInfo *titleInfo;    
    std::map< int, TitleInfo* >::iterator tit;

    if(iState < 0 || iState > 2)
        return false;

    if(m_pDlgTitle != NULL)
    {
        tit = m_mapTitleInfo.find(iTitleId);
        if(m_mapTitleInfo.end() == tit)
            return false;

        titleInfo = tit->second;

        if(titleInfo->m_bVisible == false)
            return false;

        record.m_iTitleID       = titleInfo->m_iTitleID;
        record.m_iSort          = titleInfo->m_iSortNum;
        record.m_eTitleType     = titleInfo->m_eTitleType;
        record.m_wstrTitleName  = titleInfo->m_wstrTitleName;
        record.m_wstrTitleDesc  = titleInfo->m_wstrDescription;            
        record.m_nSecret        = iState;    // 습득/습득가능/???    
        record.m_bPeriod        = !bInfinity;
        //record.m_bSecret        = titleInfo->m_bIsSecretTitle;  

        if(titleInfo->m_bVisible == false || titleInfo->m_bIsSecretTitle == true)
            record.m_bSecret = true;        
        else
            record.m_bSecret = false;

        if(iState == 0)
            record.m_bSecret = false;

        m_pDlgTitle->UpdateRecord(record);
    }

    return true;
}

bool CX2TitleManager::UpdateMission(std::vector< KMissionInstance > vecMisstionInfo)
{
    bool tempRet = false, ret = false;    

    if(m_pDlgTitle != NULL)
    {
        int iTitleId;
        KMissionInstance nodeMission;                    

        ret = false;
        for(int i=0; i<(int)vecMisstionInfo.size(); ++i)
        {
            nodeMission = vecMisstionInfo[i];
            iTitleId = nodeMission.m_iID;

            tempRet = UpdateTitle(iTitleId, 1);    
            if(ret == false && tempRet == true)
                ret = true;

            UpdateSubMission(iTitleId, nodeMission.m_vecSubMissionInstance);
        }
    }

    return ret;
}

void CX2TitleManager::UpdateSubMission(int iMissionId, std::vector< KSubMissionInstance > vecSubMission)
{
    KSubMissionInstance kSubMission;

    for(int i=0; i<(int)vecSubMission.size(); ++i)
    {            
        kSubMission = vecSubMission[i];
        UpdateClearCondition(kSubMission);
    }
}

void CX2TitleManager::UpdateClearCondition(KSubMissionInstance kSubMission)
{
    int iSubId;
    int nCount;
    bool bSuccess;    
    std::map< int, SubMissionTemplet >::iterator sit;

    iSubId = kSubMission.m_iID;
    nCount = kSubMission.m_sClearData;
    bSuccess = kSubMission.m_bIsSuccess;

    sit = m_mapSubMission.find(iSubId);
    if(m_mapSubMission.end() == sit)
        return;

    sit->second.m_ClearCondition.m_bSuccess = bSuccess;
    sit->second.m_ClearCondition.m_nCount = nCount;
}

wstring CX2TitleManager::GetTitleAbilityDesc(int titleId)
{
    TitleInfo *titleInfo = NULL;    
    std::map< int, TitleInfo* >::iterator tit;

    if(m_pDlgTitle != NULL)
    {
        tit = m_mapTitleInfo.find(titleId);
        if(m_mapTitleInfo.end() == tit)
            return L"";

        titleInfo = tit->second;
    }

	if( titleInfo == NULL )
		return L"";

    wstring socketDesc = L"\n \n[";
    socketDesc += GET_STRING( STR_ID_760 );
    socketDesc += L"]\n";
    const CX2SocketItem::SocketData* pSocketData = NULL;

    if(titleInfo->m_iBaseHP != 0)
    {
        if(titleInfo->m_iBaseHP > 0)
            socketDesc += GET_REPLACED_STRING( ( STR_ID_761, "i", titleInfo->m_iBaseHP ) );
        else
            socketDesc += GET_REPLACED_STRING( ( STR_ID_762, "i", titleInfo->m_iBaseHP ) );
    }
    if(titleInfo->m_iAtkPhysic != 0)
    {
        if(titleInfo->m_iAtkPhysic > 0)
            socketDesc += GET_REPLACED_STRING( ( STR_ID_763, "i", titleInfo->m_iAtkPhysic ) );
        else
            socketDesc += GET_REPLACED_STRING( ( STR_ID_764, "i", titleInfo->m_iAtkPhysic ) );
    }
    if(titleInfo->m_iAtkMagic != 0)
    {
        if(titleInfo->m_iAtkMagic > 0)
            socketDesc += GET_REPLACED_STRING( ( STR_ID_765, "i", titleInfo->m_iAtkMagic ) );
        else
            socketDesc += GET_REPLACED_STRING( ( STR_ID_766, "i", titleInfo->m_iAtkMagic ) );
    }
    if(titleInfo->m_iDefPhysic != 0)
    {
        if(titleInfo->m_iDefPhysic > 0)
            socketDesc += GET_REPLACED_STRING( ( STR_ID_767, "i", titleInfo->m_iDefPhysic ) );
        else
            socketDesc += GET_REPLACED_STRING( ( STR_ID_768, "i", titleInfo->m_iDefPhysic ) );
    }
    if(titleInfo->m_iDefMagic != 0)
    {
        if(titleInfo->m_iDefMagic > 0)
            socketDesc += GET_REPLACED_STRING( ( STR_ID_769, "i", titleInfo->m_iDefMagic ) );
        else
            socketDesc += GET_REPLACED_STRING( ( STR_ID_770, "i", titleInfo->m_iDefMagic ) );
    }
    socketDesc += L"\n \n";

	//{{ kimhc // 2011-07-05 // 옵션데이타 수치화 작업
	const int iLevel_ = g_pData->GetSelectUnitLevel();
	//}} kimhc // 2011-07-05 // 옵션데이타 수치화 작업

    wstring tempSockDesc = L"";

#ifdef SERV_GROW_UP_SOCKET
	std::vector<int> vecTempSocketOption;
	g_pData->GetSocketItem()->GetGrowUpSocketData( titleInfo->m_vecSocketOption, vecTempSocketOption );
	BOOST_TEST_FOREACH( const int&, iSocketID, vecTempSocketOption )
#else SERV_GROW_UP_SOCKET
    for(int i=0; i<(int)titleInfo->m_vecSocketOption.size(); ++i)
#endif SERV_GROW_UP_SOCKET
    {
#ifdef SERV_GROW_UP_SOCKET
		pSocketData = g_pData->GetSocketItem()->GetSocketData( iSocketID );
#else SERV_GROW_UP_SOCKET
        pSocketData = g_pData->GetSocketItem()->GetSocketData( titleInfo->m_vecSocketOption[i] );
#endif SERV_GROW_UP_SOCKET
        if ( pSocketData != NULL )
        {
			//{{ kimhc // 2011-07-05 // 옵션데이타 수치화 작업
			tempSockDesc = pSocketData->GetSocketDesc( iLevel_ );
			//}} kimhc // 2011-07-05 // 옵션데이타 수치화 작업

            tempSockDesc += L"\n";
#ifdef CLIENT_GLOBAL_LINEBREAK
			socketDesc += CWordLineHandler::GetStrByLineBreakInX2Main( tempSockDesc.c_str(), 260, XUF_DODUM_13_SEMIBOLD );            
#else //CLIENT_GLOBAL_LINEBREAK
            socketDesc += g_pMain->GetStrByLienBreak( tempSockDesc.c_str(), 260, XUF_DODUM_13_SEMIBOLD );            
#endif //CLIENT_GLOBAL_LINEBREAK  
        }
    }    

    return socketDesc;
}


CX2Stat::Stat CX2TitleManager::GetSocketStat()
{
    CX2Stat::Stat tempStat;
    const TitleInfo *pTitleInfo;

    pTitleInfo = GetTitleInfo(g_pData->GetMyUser()->GetSelectUnit()->GetTitleId());
    if(pTitleInfo == NULL)
        return tempStat;
    
    tempStat.m_fBaseHP += (float)pTitleInfo->m_iBaseHP;
    tempStat.m_fAtkPhysic += (float)pTitleInfo->m_iAtkPhysic;
    tempStat.m_fAtkMagic += (float)pTitleInfo->m_iAtkMagic;
    tempStat.m_fDefPhysic += (float)pTitleInfo->m_iDefPhysic;
    tempStat.m_fDefMagic += (float)pTitleInfo->m_iDefMagic;
	

    return tempStat;
}

bool CX2TitleManager::GetUnitType(int titleid)
{
    const TitleInfo *pTitle = GetTitleInfo(titleid);

    if(pTitle == NULL)
        return false;

	if(	pTitle->m_eUnitType == CX2Unit::UT_NONE || 
		(	g_pData != NULL && g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetSelectUnit() != NULL &&
			g_pData->GetMyUser()->GetSelectUnit()->GetType() == pTitle->m_eUnitType ) )
		return true;

    return false;
}

bool CX2TitleManager::GetUnitClass(int titleid)
{
    const TitleInfo *pTitle = GetTitleInfo(titleid);

    if(pTitle == NULL)
        return false;

    if(pTitle->m_eUnitClass == CX2Unit::UC_NONE || pTitle->m_eUnitClass == g_pData->GetMyUser()->GetSelectUnit()->GetClass())
        return true;

    return false;
}


bool CX2TitleManager::GetTakeTitle(int titleid)
{
    if(m_pDlgTitle != NULL && m_pDlgTitle->GetTitleState(titleid) == 0)
        return true;
    return false;
}

void CX2TitleManager::EqipTitle(int titleid)
{
	SelectTitle(titleid);

	if(titleid > 0)
	{
		if(m_pDlgTitle != NULL)
		{
			if(m_pDlgTitle->GetTitleState(titleid) == 0)
			{
				Handler_EGS_EQUIP_TITLE_REQ(titleid);
			}
		}                    
	}
	else if(titleid == 0)
	{
		// 칭호해제
		DetachTitle();
	}
}

//{{ 최민철 [2013/1/4]  게임내 정보 스트링을 엑셀파일로 출력
#ifdef PRINT_INGAMEINFO_TO_EXCEL
void CX2TitleManager::PrintTitleInfo_ToExcel()
{
	/* // txt 파일로
	TitleInfo* pTitleInfo = NULL;
	
	wofstream of(L"칭호 목록.txt");
	of.imbue(std::locale("kor",locale::ctype));


	std::map< int, TitleInfo* >::iterator iter;
	for( iter = m_mapTitleInfo.begin(); iter != m_mapTitleInfo.end(); iter++ )
	{
		pTitleInfo = iter->second;
		
		of << endl << pTitleInfo->m_iTitleID << endl << pTitleInfo->m_wstrTitleName << endl << endl << L"[설명]" << endl << pTitleInfo->m_wstrDescription  << endl << endl;
		//조건
		wstring str1 = pTitleInfo->m_wstrDescription;
		wstring strCondition = GET_REPLACED_STRING( ( STR_ID_777, "L", g_pData->GetTitleManager()->GetMIssionDesc(pTitleInfo->m_iTitleID) ) );
		g_pMain->GetStrByLienBreak( str1.c_str(), 260, XUF_DODUM_13_SEMIBOLD );
		wstring::size_type lastPosCond = strCondition.find_first_not_of(L"\n", 0);
		wstring::size_type posCond     = strCondition.find_first_of(L"\n", lastPosCond);
		
		while (wstring::npos != posCond || wstring::npos != lastPosCond)
		{        
			wstring tempStr = strCondition.substr(lastPosCond, posCond - lastPosCond);
			//pListbox->AddItem(tempStr.c_str(), NULL);
			of << tempStr << endl;
			lastPosCond = strCondition.find_first_not_of(L"\n", posCond);
			posCond = strCondition.find_first_of(L"\n", lastPosCond);
		}
		//옵션
		wstring str2 = g_pData->GetTitleManager()->GetTitleAbilityDesc(pTitleInfo->m_iTitleID);
		wstring::size_type lastPos = str2.find_first_not_of(L"\n", 0);
		wstring::size_type pos     = str2.find_first_of(L"\n", lastPos);

		while (wstring::npos != pos || wstring::npos != lastPos)
		{        
			wstring tempStr = str2.substr(lastPos, pos - lastPos);
			//pListbox->AddItem(tempStr.c_str(), NULL);
			of << tempStr << endl;
			lastPos = str2.find_first_not_of(L"\n", pos);
			pos = str2.find_first_of(L"\n", lastPos);
		}
		of << L"----------------------------------------------------------------------------------------"<< endl;
	}


	of.close();
	*/
	//엑셀로
	BasicExcel e;
	e.New(1);
	
	BasicExcelWorksheet* sheet = e.GetWorksheet((size_t)0);
	int cal = 1;
	TitleInfo* pTitleInfo = NULL;

	std::map< int, TitleInfo* >::iterator iter;
	// Title 출력 
	sheet->Cell(0,0)->SetWString(L"칭호 ID");	
	sheet->Cell(0,1)->SetWString(L"칭호 이름");	
	sheet->Cell(0,2)->SetWString(L"칭호 설명");	
	sheet->Cell(0,3)->SetWString(L"획득 조건");	
	sheet->Cell(0,4)->SetWString(L"옵션");	

	// 내용 출력
	for( iter = m_mapTitleInfo.begin(); iter != m_mapTitleInfo.end(); iter++ )
	{
		pTitleInfo = iter->second;
		sheet->Cell(cal,0)->SetInteger( pTitleInfo->m_iTitleID );
		sheet->Cell(cal,1)->SetWString( pTitleInfo->m_wstrTitleName.c_str() );
		sheet->Cell(cal,2)->SetWString( pTitleInfo->m_wstrDescription.c_str() );	
		
		//조건
		wstring str1 = pTitleInfo->m_wstrDescription;
		wstring strCondition = GET_REPLACED_STRING( ( STR_ID_777, "L", g_pData->GetTitleManager()->GetMIssionDesc(pTitleInfo->m_iTitleID) ) );
		g_pMain->GetStrByLienBreak( str1.c_str(), 260, XUF_DODUM_13_SEMIBOLD );
		wstring::size_type lastPosCond = strCondition.find_first_not_of(L"\n", 0);
		wstring::size_type posCond     = strCondition.find_first_of(L"\n", lastPosCond);
		wstring tempPlusStr;
		while (wstring::npos != posCond || wstring::npos != lastPosCond)
		{        
			wstring tempStr = strCondition.substr(lastPosCond, posCond - lastPosCond);
			tempPlusStr = tempPlusStr + tempStr + L" ";
			
			//tempStr = strCondition.substr(lastPosCond, posCond - lastPosCond);
			lastPosCond = strCondition.find_first_not_of(L"\n", posCond);
			posCond = strCondition.find_first_of(L"\n", lastPosCond);
		}
			sheet->Cell(cal,3)->SetWString( tempPlusStr.c_str() );
		//옵션
		wstring str2 = g_pData->GetTitleManager()->GetTitleAbilityDesc(pTitleInfo->m_iTitleID);
		wstring::size_type lastPos = str2.find_first_not_of(L"\n", 0);
		wstring::size_type pos     = str2.find_first_of(L"\n", lastPos);

		wstring tempPlusStr1;
		while (wstring::npos != pos || wstring::npos != lastPos)
		{        
			wstring tempStr1 = str2.substr(lastPos, pos - lastPos);
			tempPlusStr1 = tempPlusStr1 + tempStr1 + L" ";;
			lastPos = str2.find_first_not_of(L"\n", pos);
			pos = str2.find_first_of(L"\n", lastPos);
		}
		sheet->Cell(cal,4)->SetWString( tempPlusStr1.c_str() );
		cal++;
	}
	e.SaveAs("TITLE_LIST.xls");
}
#endif PRINT_INGAMEINFO_TO_EXCEL
//}} 최민철 [2013/1/4]  게임내 정보 스트링을 엑셀파일로 출력

// 타이틀 장착
bool CX2TitleManager::Handler_EGS_EQUIP_TITLE_REQ(int titleId)
{
    KEGS_EQUIP_TITLE_REQ kPacket;

    kPacket.m_iTitleID = titleId;
    
    g_pData->GetServerProtocol()->SendPacket( EGS_EQUIP_TITLE_REQ, kPacket );
    g_pMain->AddServerPacket( EGS_EQUIP_TITLE_ACK, 30.0f ); 
 
    m_bProcess = true;

    return true;
}

bool CX2TitleManager::Handler_EGS_EQUIP_TITLE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    // 패킷 처리
    KSerBuffer* pBuff = (KSerBuffer*)lParam;
    KEGS_EQUIP_TITLE_ACK kEvent;

    DeSerialize( pBuff, &kEvent );	

    if(g_pMain->DeleteServerPacket( EGS_EQUIP_TITLE_ACK ))
    {
        if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
        {
			int titleId = kEvent.m_iTitleID;
            AttachTitle(titleId);

			m_bProcess = false;

#ifdef SERV_CHINA_SPIRIT_EVENT
			g_pMain->GetMemoryHolder()->UpdateUseSpiritEvent();
#endif SERV_CHINA_SPIRIT_EVENT

#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
			if ( NULL != g_pData &&
				NULL != g_pData->GetMyUser() &&
				NULL != g_pData->GetMyUser()->GetSelectUnit() )
			{
				/// 스킬 추가 레벨 효과 갱신
				g_pData->GetMyUser()->GetSelectUnit()->ResetIncreaseSkillLevelBySocket();
			}
#endif // UPGRADE_SKILL_SYSTEM_2013

            return true;
        }        
    }

    return false;
}

// 진행중인 미션
bool CX2TitleManager::Handler_EGS_NEW_MISSION_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    // 패킷 처리
    KSerBuffer* pBuff = (KSerBuffer*)lParam;
    KEGS_NEW_MISSION_NOT kEvent;

    DeSerialize( pBuff, &kEvent );	

    bool ret = UpdateMission(kEvent.m_vecNewMission);

    if(kEvent.m_bFirst == false && ret == true)
    {
        m_bNewOpenMission = true;
    }

    return true;
}

// 획득한 미션
bool CX2TitleManager::Handler_EGS_REWARD_TITLE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    // 패킷 처리
    KSerBuffer* pBuff = (KSerBuffer*)lParam;
    KEGS_REWARD_TITLE_NOT kEvent;

    DeSerialize( pBuff, &kEvent );	
    
    bool ret = UpdateTitle(kEvent.m_kTitle, 0);
    
    if(ret == true)
    {
        m_bNewTitle = true;
        m_bShowNewTitle = true;
#ifdef SHOW_NEW_TITLE_NAME
		m_strNewTitle = GetTitleInfo(kEvent.m_kTitle.m_iTitleID)->m_wstrTitleName;
#endif //SHOW_NEW_TITLE_NAME
    }    

    return true;
}


// 타이틀 미션이 업데이트된 내용이 있을때
bool CX2TitleManager::Handler_EGS_UPDATE_MISSION_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    // 패킷 처리
    KSerBuffer* pBuff = (KSerBuffer*)lParam;
    KEGS_UPDATE_MISSION_NOT kEvent;

    DeSerialize( pBuff, &kEvent );	

    int iTitleId;
    KMissionInstance nodeMission;        

    for(int i=0; i<(int)kEvent.m_vecMissionInst.size(); ++i)
    {
        nodeMission = kEvent.m_vecMissionInst[i];
        iTitleId = nodeMission.m_iID;
       
        UpdateSubMission(iTitleId, nodeMission.m_vecSubMissionInstance);
    }

    return true;
}

//현재 장착중인 타이틀의 기간이 다되었을경우
bool CX2TitleManager::Handler_EGS_TITLE_EXPIRATION_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_TITLE_EXPIRATION_NOT kEvent;

	DeSerialize( pBuff, &kEvent );	
	 

    // 장착중인 타이틀 Close로 업데이트
    int titleid = kEvent.m_iTitleID; // g_pData->GetMyUser()->GetSelectUnit()->GetTitleId();
    UpdateTitle(titleid, 1);

    // 장착 해제
    AttachTitle(0);

	//{{ kimhc // 기간이 만료됬다는 알림을 채팅 창으로 변경
	if( g_pChatBox != NULL)
	{
		const TitleInfo *pTitleInfo;
		pTitleInfo = GetTitleInfo( titleid );

		if( pTitleInfo != NULL )
		{
			std::wstringstream strStream;
			strStream << GET_REPLACED_STRING( ( STR_ID_195, "L", pTitleInfo->m_wstrTitleName ) );

			wstring wstrColor = L"#CECEC88";							// (236, 236, 136)
			D3DXCOLOR coTextColor(0.92549f, 0.92549f, 0.53333f, 1.f);	// (236, 236, 136)

			g_pChatBox->AddChatLog( strStream.str().c_str(), KEGS_CHAT_REQ::CPT_TOTAL, coTextColor, wstrColor );
		}
	}
	
	switch ( g_pMain->GetNowStateID() )
	{
	case CX2Main::XS_PVP_GAME:
	case CX2Main::XS_PVP_RESULT:
	case CX2Main::XS_DUNGEON_RESULT:
	case CX2Main::XS_TRAINING_GAME:
	case CX2Main::XS_TRAINING_RESULT:
		break;

	default:
		if(GetTitleInfo( titleid ) != NULL)
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_REPLACED_STRING( ( STR_ID_195, "L", GetTitleInfo( titleid )->m_wstrTitleName ) ), g_pMain->GetNowState() );
		break;
	}
	//}} kimhc // 기간이 만료됬다는 알림을 채팅 창으로 변경

    return true;
}



//#endif