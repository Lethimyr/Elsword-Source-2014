#include "StdAfx.h"
#include ".\x2uiunit.h"

CX2UIUnit::CX2UIUnit( const wstring& fileName )
//{{ robobeg : 2008-10-28
: m_LuaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState(), 0, true )
//}} robobeg : 2008-10-28
{
	m_ScriptFileName	= fileName;
	m_fTime				= 0.0f;
	m_fElapsedTime		= 0.0f;
	m_fStateTime		= 0.0f;
	m_StatePlayCount	= 0;

	m_Pos				= D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_Rotate			= D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_Scale				= D3DXVECTOR3(1.0f,1.0f,1.0f);

	m_pXSkinAnim		= NULL;
	m_pMesh				= NULL;
	m_pChangeTexXET		= NULL;
	m_pMultiTexXET		= NULL;
	m_pAnimXET			= NULL;

	m_TexStage0Speed	= D3DXVECTOR2(0.0f,0.0f);
	m_TexStage1Speed	= D3DXVECTOR2(0.0f,0.0f);
	m_TexStage2Speed	= D3DXVECTOR2(0.0f,0.0f);

	m_pTalkBoxMgr		= NULL;
	//m_pTalkBoxMgr		= CX2TalkBoxManager::CreateTalkBoxManger();
	m_pTalkBoxMgr		= CX2TalkBoxManager::CreateTalkBoxManger();
	
	m_bCameIn			= false;

	Init();
}

CX2UIUnit::~CX2UIUnit(void)
{
	//SAFE_DELETE( m_pXSkinAnim );
	CKTDGXSkinAnim::DeleteKTDGObject( m_pXSkinAnim );
    m_pXSkinAnim = NULL;

	SAFE_CLOSE( m_pMesh );
	SAFE_CLOSE( m_pAnimXET );
	SAFE_CLOSE( m_pChangeTexXET );
	SAFE_CLOSE( m_pMultiTexXET );

	//SAFE_DELETE( m_pTalkBoxMgr );
	CX2TalkBoxManager::DeleteKTDGObject( m_pTalkBoxMgr );

}

void CX2UIUnit::Init()
{
	//스크립트 파일을 로드한다
	bool bResult = false;
//{{ robobeg : 2008-10-28
// 	bResult = g_pKTDXApp->LoadLuaTinker( m_ScriptFileName.c_str() );
// 	ASSERT( false != bResult );
	//g_pKTDXApp->LoadAndDoMemory( &m_LuaManager, L"Enum.lua" );
	bResult = g_pKTDXApp->LoadAndDoMemory( &m_LuaManager, m_ScriptFileName.c_str() );
	ASSERT( false != bResult );

	bResult = m_LuaManager.ExportFunctionsToGlobalEnv();
	ASSERT( false != bResult );
//}} robobeg : 2008-10-28


	if( m_LuaManager.BeginTable( "INIT" ) == true )
	{
		wstring modelName;
		LUA_GET_VALUE( m_LuaManager, "MODEL_NAME", modelName, L"" );
		wstring modelMoveBoneName;
		LUA_GET_VALUE( m_LuaManager, "MODEL_MOVE_BONE_NAME", modelMoveBoneName, L"" );
		wstring animXETName;
		LUA_GET_VALUE( m_LuaManager, "ANIM_XET_NAME", animXETName, L"" );
		wstring changeTexXET;
		LUA_GET_VALUE( m_LuaManager, "CHANGE_TEX_XET_NAME", changeTexXET, L"" );
		wstring multiTexXET;
		LUA_GET_VALUE( m_LuaManager, "MULTI_TEX_XET_NAME", multiTexXET, L"" );

		LUA_GET_VALUE( m_LuaManager, "TEX0_SPEED_X", m_TexStage0Speed.x, 0.0f );
		LUA_GET_VALUE( m_LuaManager, "TEX0_SPEED_Y", m_TexStage0Speed.y, 0.0f );
		LUA_GET_VALUE( m_LuaManager, "TEX1_SPEED_X", m_TexStage1Speed.x, 0.0f );
		LUA_GET_VALUE( m_LuaManager, "TEX1_SPEED_Y", m_TexStage1Speed.y, 0.0f );
		LUA_GET_VALUE( m_LuaManager, "TEX2_SPEED_X", m_TexStage2Speed.x, 0.0f );
		LUA_GET_VALUE( m_LuaManager, "TEX2_SPEED_Y", m_TexStage2Speed.y, 0.0f );

		if( m_LuaManager.BeginTable( "POS" ) == true )
		{
			LUA_GET_VALUE( m_LuaManager, 1, m_Pos.x, 0.0f );
			LUA_GET_VALUE( m_LuaManager, 2, m_Pos.y, 0.0f );
			LUA_GET_VALUE( m_LuaManager, 3, m_Pos.z, 0.0f );
			m_LuaManager.EndTable();
		}
		if( m_LuaManager.BeginTable( "ROTATE" ) == true )
		{
			LUA_GET_VALUE( m_LuaManager, 1, m_Rotate.x, 0.0f );
			LUA_GET_VALUE( m_LuaManager, 2, m_Rotate.y, 0.0f );
			LUA_GET_VALUE( m_LuaManager, 3, m_Rotate.z, 0.0f );
			m_LuaManager.EndTable();
		}
		if( m_LuaManager.BeginTable( "SCALE" ) == true )
		{
			LUA_GET_VALUE( m_LuaManager, 1, m_Scale.x, 0.0f );
			LUA_GET_VALUE( m_LuaManager, 2, m_Scale.y, 0.0f );
			LUA_GET_VALUE( m_LuaManager, 3, m_Scale.z, 0.0f );
			m_LuaManager.EndTable();
		}


		LUA_GET_VALUE_UTF8( m_LuaManager, "WAIT_STATE_ID",		m_WaitStateID,		"" );
		LUA_GET_VALUE_UTF8( m_LuaManager, "COME_IN_STATE_ID",	m_ComeInStateID,	"" );
		LUA_GET_VALUE_UTF8( m_LuaManager, "BUY_IN_STATE_ID",		m_BuyInStateID,		"" );
		LUA_GET_VALUE_UTF8( m_LuaManager, "QUEST_IN_STATE_ID",	m_QuestInStateID,	"" );
		LUA_GET_VALUE_UTF8( m_LuaManager, "MENU_IN_STATE_ID",	m_MenuInStateID,	"" );
		

		m_pMesh			= g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( modelName, modelMoveBoneName.c_str() );
		m_pAnimXET		= g_pKTDXApp->GetDeviceManager()->OpenXET( animXETName );
		m_pChangeTexXET	= g_pKTDXApp->GetDeviceManager()->OpenXET( changeTexXET );
		m_pMultiTexXET		= g_pKTDXApp->GetDeviceManager()->OpenXET( multiTexXET );
		if( m_pMesh != NULL )
		{
			//m_pXSkinAnim = CKTDGXSkinAnim::CreateSkinAnim();
            ASSERT( m_pXSkinAnim == NULL );
			m_pXSkinAnim = CKTDGXSkinAnim::CreateSkinAnim();

			m_pXSkinAnim->SetApplyMotionOffset( true );
			m_pXSkinAnim->SetAnimXSkinMesh( m_pMesh );
			m_pXSkinAnim->AddModelXSkinMesh( m_pMesh, m_pAnimXET, m_pMultiTexXET, m_pChangeTexXET );
		}		

		m_LuaManager.EndTable();
	}

	StateChange( m_WaitStateID );	
	
}

//{{ robobeg : 2008-10-18
/*virtual*/
void    CX2UIUnit::NotifyShowObjectChanged()
{ 
}
//}} robobeg : 2008-10-18


HRESULT CX2UIUnit::OnFrameMove( double fTime, float fElapsedTime )
{
//{{ robobeg : 2008-10-18
	//if ( m_bShow == false )
	//	return S_OK;
    if ( !GetShowObject())	
        return S_OK;
	
//}} robobeg : 2008-10-18

	m_fTime			= (float)fTime;
	m_fElapsedTime	= fElapsedTime;

	CommonFrameMove();
	CommonEventProcess();		

	return S_OK;
}

//{{ robobeg : 2008-10-28
//HRESULT CX2UIUnit::OnFrameRender()
void    CX2UIUnit::OnFrameRender_Draw()
//}} robobeg : 2008-10-28
{

	D3DXMATRIX matOrgView, matView;
	g_pKTDXApp->GetViewTransform( &matOrgView );
	bool bPersfective;
	bPersfective = g_pKTDXApp->GetDGManager()->GetPerspective();
	if( bPersfective == true )
	{
		g_pKTDXApp->GetDGManager()->SetProjection( 
			g_pKTDXApp->GetDGManager()->GetNear(),
			g_pKTDXApp->GetDGManager()->GetFar(), false );
	}

	D3DXMatrixLookAtLH( &matView, &D3DXVECTOR3(0,0,-1300), &D3DXVECTOR3(0,0,0), &D3DXVECTOR3(0,1,0) );
	g_pKTDXApp->SetViewTransform( &matView );

	if( m_pXSkinAnim != NULL )
	{
		m_pXSkinAnim->GetMatrix().Move( m_Pos );
		m_pXSkinAnim->GetMatrix().Rotate( D3DXToRadian(m_Rotate.x),D3DXToRadian(m_Rotate.y),D3DXToRadian(m_Rotate.z) );
		m_pXSkinAnim->GetMatrix().Scale( m_Scale );

		CKTDGXRenderer::RenderParam* pRenderParam = m_pXSkinAnim->GetRenderParam();
		pRenderParam->bAlphaBlend		= true;
		pRenderParam->bZEnable			= false;
		pRenderParam->renderType		= CKTDGXRenderer::RT_REAL_COLOR;
		pRenderParam->cullMode			= D3DCULL_NONE;
//{{ robobeg : 2008-10-24
		//pRenderParam->worldMatrix		= m_pXSkinAnim->GetMatrix().GetMatrix();
//}} robobeg : 2008-10-24
		

		m_pXSkinAnim->OnFrameRender();
	}

	g_pKTDXApp->SetViewTransform( &matOrgView );
	if( bPersfective == true )
	{
		g_pKTDXApp->GetDGManager()->SetProjection( 
			g_pKTDXApp->GetDGManager()->GetNear(),
			g_pKTDXApp->GetDGManager()->GetFar(), true );
	}

	//return S_OK;
}

void CX2UIUnit::StateChange( string stateID )
{
	CommonStateEnd();
	m_NowState.m_StateTable = stateID;
	CommonStateStart();
}


void CX2UIUnit::CommonStateStart()
{
	//테스트 할 때 이거 주석 풀어주면 쵝오!
	//g_pKTDXApp->LoadAndDoMemory( &m_LuaManager, m_ScriptFileName.c_str() );

	if( m_LuaManager.BeginTable( m_NowState.m_StateTable.c_str() ) == true )
	{
		m_fStateTime			= 0.0f;
		m_StatePlayCount		= 0;

		LUA_GET_VALUE( m_LuaManager, "PLAY_COUNT",			m_NowState.m_PlayCount,			0 );
		LUA_GET_VALUE( m_LuaManager, "STATE_TIME",			m_NowState.m_fStateTime,		0.0f );

		LUA_GET_VALUE_UTF8( m_LuaManager, "LUA_STATE_START",		m_NowState.m_Lua_StateStart,	"" );
		LUA_GET_VALUE_UTF8( m_LuaManager, "LUA_FRAME_MOVE",		m_NowState.m_Lua_FrameMove,		"" );
		LUA_GET_VALUE_UTF8( m_LuaManager, "LUA_EVENT_PROCESS",	m_NowState.m_Lua_EventProcess,	"" );
		LUA_GET_VALUE_UTF8( m_LuaManager, "LUA_STATE_END",		m_NowState.m_Lua_StateEnd,		"" );

		if( m_LuaManager.BeginTable( "POS" ) == true )
		{
			LUA_GET_VALUE( m_LuaManager, 1, m_Pos.x, 0.0f );
			LUA_GET_VALUE( m_LuaManager, 2, m_Pos.y, 0.0f );
			LUA_GET_VALUE( m_LuaManager, 3, m_Pos.z, 0.0f );
			m_LuaManager.EndTable();

			//{{ 오현빈 // 2012-07-19 // 메인메뉴가 변경됨에 따라 모든 NPC스크립트에 대해 Y값을 감소시켜줘야 하기 때문에 파싱후에 증감연사 추가
			m_Pos.y -= 50.f;
			//}} 오현빈 // 2012-07-19 // 메인메뉴가 변경됨에 따라 모든 NPC스크립트에 대해 Y값을 감소시켜줘야 하기 때문에 파싱후에 증감연사 추가

		}
		if( m_LuaManager.BeginTable( "ROTATE" ) == true )
		{
			LUA_GET_VALUE( m_LuaManager, 1, m_Rotate.x, 0.0f );
			LUA_GET_VALUE( m_LuaManager, 2, m_Rotate.y, 0.0f );
			LUA_GET_VALUE( m_LuaManager, 3, m_Rotate.z, 0.0f );
			m_LuaManager.EndTable();
		}
		if( m_LuaManager.BeginTable( "SCALE" ) == true )
		{
			LUA_GET_VALUE( m_LuaManager, 1, m_Scale.x, 0.0f );
			LUA_GET_VALUE( m_LuaManager, 2, m_Scale.y, 0.0f );
			LUA_GET_VALUE( m_LuaManager, 3, m_Scale.z, 0.0f );
			m_LuaManager.EndTable();
		}



		wstring animName;
		LUA_GET_VALUE( m_LuaManager, "ANIM_NAME", animName, L"" );
		bool bTransition;
		LUA_GET_VALUE( m_LuaManager, "TRANSITION", bTransition, true );
		CKTDGXSkinAnim::XSKIN_ANIM_PLAYTYPE playType;
		LUA_GET_VALUE_ENUM( m_LuaManager, "PLAY_TYPE", playType, CKTDGXSkinAnim::XSKIN_ANIM_PLAYTYPE, CKTDGXSkinAnim::XAP_ONE_WAIT );
		float fPlaySpeed;
		LUA_GET_VALUE( m_LuaManager, "PLAY_SPEED", fPlaySpeed, 1.0f );
		if( m_pXSkinAnim != NULL )
		{
			m_pXSkinAnim->ChangeAnim( animName.c_str(), bTransition );

			m_pXSkinAnim->GetMatrix().Move( m_Pos );
			m_pXSkinAnim->GetMatrix().Rotate( D3DXToRadian(m_Rotate.x),D3DXToRadian(m_Rotate.y),D3DXToRadian(m_Rotate.z) );
			m_pXSkinAnim->GetMatrix().Scale( m_Scale );

			m_pXSkinAnim->Play( playType );
			m_pXSkinAnim->SetPlaySpeed( fPlaySpeed );
		}

		if( m_LuaManager.BeginTable( "START_TALK" ) == true )
		{
			LoadTalkBox();
			m_LuaManager.EndTable();
		}

		m_LuaManager.EndTable();
	}

	if( false == m_NowState.m_Lua_StateStart.empty() )
	{
		//string func;
		//ConvertWCHARToChar( func, m_NowState.m_Lua_StateStart.c_str() );
		lua_tinker::call<void>( g_pKTDXApp->GetLuaBinder()->GetLuaState(), m_NowState.m_Lua_StateStart.c_str(), g_pKTDXApp, g_pX2Game, this );
	}
}

void CX2UIUnit::CommonFrameMove()
{
	m_fStateTime += m_fElapsedTime;
	if( m_pXSkinAnim != NULL )
	{
		m_pXSkinAnim->OnFrameMove( m_fTime, m_fElapsedTime );
		m_Pos.x += m_pXSkinAnim->GetMotionOffset().x;
		if( m_pXSkinAnim->IsAnimationEnd() == true )
		{
			m_StatePlayCount++;
		}

		m_pXSkinAnim->GetRenderParam()->texOffsetStage0 += m_TexStage0Speed * m_fElapsedTime;
		m_pXSkinAnim->GetRenderParam()->texOffsetStage1 += m_TexStage1Speed * m_fElapsedTime;
		m_pXSkinAnim->GetRenderParam()->texOffsetStage2 += m_TexStage2Speed * m_fElapsedTime;
	}


	if( false == m_NowState.m_Lua_FrameMove.empty() )
	{
		//string func;
		//ConvertWCHARToChar( func, m_NowState.m_Lua_FrameMove.c_str() );
		lua_tinker::call<void>( g_pKTDXApp->GetLuaBinder()->GetLuaState(), m_NowState.m_Lua_FrameMove.c_str(), g_pKTDXApp, g_pX2Game, this );
	}
}

void CX2UIUnit::CommonEventProcess()
{
	if( m_NowState.m_StateTable.compare( m_WaitStateID ) == 0 )
		return;

	if( m_NowState.m_fStateTime > 0.0f && m_fStateTime >= m_NowState.m_fStateTime )
	{
		string strNextStateID = PopNextStateID();
		if( strNextStateID.length() == 0 )
		{
			StateChange( m_WaitStateID );
		}
		else
		{
			StateChange( strNextStateID );
		}
	}
	else if( m_NowState.m_PlayCount > 0 && m_StatePlayCount >= m_NowState.m_PlayCount )
	{
		string strNextStateID = PopNextStateID();
		if( strNextStateID.length() == 0 )
		{
			StateChange( m_WaitStateID );
		}
		else
		{
			StateChange( strNextStateID );
		}
	}

	if( false == m_NowState.m_Lua_EventProcess.empty() )
	{
		//string func;
		//ConvertWCHARToChar( func, m_NowState.m_Lua_EventProcess.c_str() );
		lua_tinker::call<void>( g_pKTDXApp->GetLuaBinder()->GetLuaState(), m_NowState.m_Lua_EventProcess.c_str(), g_pKTDXApp, g_pX2Game, this );
	}
}

void CX2UIUnit::CommonStateEnd()
{	
	if( false == m_bCameIn && 0 == m_ComeInStateID.compare( m_NowState.m_StateTable ) )
	{
		m_bCameIn = true;
	}
	

	if( m_LuaManager.BeginTable( m_NowState.m_StateTable.c_str() ) == true )
	{
		if( m_LuaManager.BeginTable( "END_TALK" ) == true )
		{
			LoadTalkBox();
			m_LuaManager.EndTable();
		}
		
		m_LuaManager.EndTable();
	}
	

	if( false == m_NowState.m_Lua_StateEnd.empty() )
	{
		//string func;
		//ConvertWCHARToChar( func, m_NowState.m_Lua_StateEnd.c_str() );
		lua_tinker::call<void>( g_pKTDXApp->GetLuaBinder()->GetLuaState(), m_NowState.m_Lua_StateEnd.c_str(), g_pKTDXApp, g_pX2Game, this );
	}
}

void CX2UIUnit::LoadTalkBox()
{
	CX2TalkBoxManagerImp::TalkBox talkBox;
	talkBox.m_OwnerUnitUID		= 0;
	talkBox.m_GameUnitType		= CX2GameUnit::GUT_USER;
	talkBox.m_bTraceUnit		= false;

	if( m_LuaManager.BeginTable( "POS" ) == true )
	{
		LUA_GET_VALUE( m_LuaManager, 1, talkBox.m_vPos.x, 0.0f );
		LUA_GET_VALUE( m_LuaManager, 2, talkBox.m_vPos.y, 0.0f );
		m_LuaManager.EndTable();
	}

	LUA_GET_VALUE_ENUM( m_LuaManager, "TYPE", talkBox.m_TalkBoxType, CX2TalkBoxManagerImp::TALK_BOX_TYPE, CX2TalkBoxManagerImp::TBT_FROM_DOWN_LEFT );
	
    int iStringIndex;
    LUA_GET_VALUE( m_LuaManager, "STRING", iStringIndex, STR_ID_EMPTY );
    talkBox.m_wstrTalkContent = GET_STRING( iStringIndex );
	
    LUA_GET_VALUE( m_LuaManager, "REMAIN_TIME", talkBox.m_fRemainTime, 1.0f );

}


// 들어오고 있는 중이다 
bool CX2UIUnit::IsComingIn() 
{
	if( false == m_bCameIn && 0 == m_ComeInStateID.compare( m_NowState.m_StateTable ) )
	{
		return true;
	}

	return false;
}

string CX2UIUnit::PopNextStateID()
{
	if( m_vecNextStateID.size() > 0 )
	{
		string strNextStateID = m_vecNextStateID[0];
		m_vecNextStateID.erase( m_vecNextStateID.begin() );
		return strNextStateID;
	}
	else
	{
		return "";
	}
}
