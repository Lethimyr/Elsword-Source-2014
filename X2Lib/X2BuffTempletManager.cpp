/** @file : X2BuffTempletManager.h
    @breif : 버프 템플릿을 관리하는 매니저 클래스
*/

#pragma once

#include "StdAfx.h"
#include ".\X2BuffTempletManager.h"

CX2BuffTempletManager* CX2BuffTempletManager::pInstance = NULL;

/** @function : OpenScriptFile
	@brief : 버프템플릿 루아스크립트 파싱
	@param : 루아파일명
*/
void CX2BuffTempletManager::OpenScriptFileForTemplet()
{
	lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(), "g_pBuffTempletManager", this );



	const wchar_t* wszScriptFileName = L"BattleFieldBuffTemplet.lua";

    if ( g_pKTDXApp->LoadLuaTinker( wszScriptFileName ) == false )
    {
		ASSERT( !"DoMemory doesn't work!" );
		ErrorLogMsg( XEM_ERROR2, wszScriptFileName );
    }
}


void CX2BuffTempletManager::OpenScriptFileForFactor()
{
	lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(), "g_pBuffTempletManager", this );

	const wchar_t* wszScriptFileName = L"BattleFieldBuffFactor.lua";

    if ( g_pKTDXApp->LoadLuaTinker( wszScriptFileName ) == false )
    {
		ASSERT( !"DoMemory doesn't work!" );
		ErrorLogMsg( XEM_ERROR2, wszScriptFileName );
    }
}

/** @function : AddBuffTemplet_LUA
	@brief : 버프템플릿 인스턴스 생성, 스크립트 파싱 및 map에 저장
*/
void CX2BuffTempletManager::AddBuffTemplet_LUA()
{
	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
	
	CX2BuffTempletPtr	ptrBuffTemplet = CX2BuffTemplet::CreateBuffTempletPtr();

	if ( NULL != ptrBuffTemplet )
	{
		if ( ptrBuffTemplet->ParsingScript( luaManager ) )
			m_mapBuffTempletPtr.insert( make_pair( ptrBuffTemplet->GetBuffTempletID(), ptrBuffTemplet ) );
	}
	else
	{
		DISPLAY_ERROR( L"new BuffTemplet didn't work" );
	}
}

/** @function : AddBuffFactor_LUA
	@brief : 특정 버프가 유지되는 동안 생성되는 버프 팩터를 위해서 만드는 버프 팩터
*/
void CX2BuffTempletManager::AddBuffFactor_LUA()
{
	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );

	UINT uiNum = 0;		/// BUFF_FACTOR_ID 를 order로 사용함
	LUA_GET_VALUE_RETURN( luaManager, "BUFF_FACTOR_ID", uiNum, 0, DISPLAY_ERROR( L"BUFF_FACTOR_ID doesn't exist" ) );
	
	if ( m_vecBuffFactorPtr.size() == uiNum - 1 )
	{
		if ( luaManager.BeginTable( "BUFF_FACTOR" ) )
		{
			CX2BuffFactorPtr ptrBuffFactor = CX2BuffFactor::CreateBuffFactorPtr();
			if ( NULL != ptrBuffFactor )
			{
				if ( ptrBuffFactor->ParsingBuffFactor( luaManager ) )
					m_vecBuffFactorPtr.push_back( ptrBuffFactor );
			}
			else
			{
				DISPLAY_ERROR( L"new BUFF_FACTOR didn't work" );
			}

			luaManager.EndTable();
		}
		else
		{
			DISPLAY_ERROR( L"BUFF_FACTOR table doesn't exist" );
		}
	}
	else
	{
		DISPLAY_ERROR( L"BUFF_FACTOR_ID invalid" );
	}
}

/** @function : GetBuffTempletPtr
	@brief : 버프템플릿아이디와 매칭되는 버프템플릿을 얻어옴
	@param : 버프템플릿아이디(eBuffTempletID)
	@return : 버프아이디와 매칭되는 템플릿(CX2BuffTempletPtr)
*/
CX2BuffTempletPtr CX2BuffTempletManager::GetBuffTempletPtr( const BUFF_TEMPLET_ID eBuffTempletID_ ) const
{
	if ( !m_mapBuffTempletPtr.empty() )
	{
		map<BUFF_TEMPLET_ID, CX2BuffTempletPtr>::const_iterator mItr = m_mapBuffTempletPtr.find( eBuffTempletID_ );

		if ( m_mapBuffTempletPtr.end() != mItr )
			return mItr->second;
	}

	return CX2BuffTempletPtr();
}

/** @function : GetCombination
	@brief : 해당 요소가 어떠한 종류의 요소들로 조합되어 있는지를 알아내는 함수
	@param : 파싱 중인 스크립트의 루아매니저(luaManager_), 조합들을 얻어오는 벡터(vecType_)
	@return : 파싱 성공시 true, 실패시 false
*/
/*static*/ bool CX2BuffTempletManager::GetCombination( KLuaManager& luaManager_, OUT vector<UINT>& vecType_ )
{
	if ( luaManager_.BeginTable( "COMBINATION" ) )
	{
		int		iIndex = 1;
		UINT	uiType = 0;

		while ( luaManager_.GetValue( iIndex, uiType ) )
		{
			vecType_.push_back( uiType );

			++iIndex;
		}

		luaManager_.EndTable();

		if ( vecType_.empty() )
		{
			return DISPLAY_ERROR( L"COMBINATION elements don't exist" );	/// 벡터가 비어있으면 컴비네이션이 지정되어 있지 않은 것이므로 파싱 실패
		}
		else
			return true;
	}
	else
		return DISPLAY_ERROR( L"COMBINATION doesn't exist" );	/// 컴비네이션이 지정 되어 있지 않으므로 파싱 실패
}

CX2BuffFactorPtr CX2BuffTempletManager::GetBuffFactorPtr( const UINT uiBuffFactorNumber_ ) const
{
	if ( uiBuffFactorNumber_ != 0 && uiBuffFactorNumber_ <= m_vecBuffFactorPtr.size() )	/// 작다로 비교 하지 않은 이유는 인덱스 0번이 uiBuffFactorNumber 1로 시작하기 때문
	{
		return m_vecBuffFactorPtr[uiBuffFactorNumber_ - 1];
	}
	else
	{
		DISPLAY_ERROR( L"uiBuffFactorNumber_ invaild" );
		return CX2BuffFactorPtr();
	}
}

void CX2BuffTempletManager::GetBuffFactorPtrFromBuffFactorList( IN KLuaManager& luaManager_, OUT vector<CX2BuffFactorPtr>& vecBuffFactorPtr_, OUT vector<UINT>& vecBuffFactorID_)
{
	if ( luaManager_.BeginTable( "BUFF_FACTOR" ) )
	{
		int iIndex = 1;
		UINT uiBuffFactorId = 0;
		while( luaManager_.GetValue( iIndex, uiBuffFactorId ) == true )
		{
			if ( 0 != uiBuffFactorId )
			{
				CX2BuffFactorPtr ptrBuffFactor = GetBuffFactorPtr( uiBuffFactorId );
				if ( NULL != ptrBuffFactor )
				{
					vecBuffFactorPtr_.push_back( ptrBuffFactor );
					vecBuffFactorID_.push_back( uiBuffFactorId );
				}
			}
			else
				DISPLAY_ERROR( L"uiBuffFactorId Invalid" );

			++iIndex;
		}

		luaManager_.EndTable();
	}
}


