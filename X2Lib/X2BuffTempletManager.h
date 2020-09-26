/**@file : X2BuffTempletManager.h
   @breif : 버프 템플릿을 관리하는 매니저 클래스 정의 헤더
*/

#pragma once

/** @class : CX2BuffTempletManager
	@brief : 버프 템플릿을 관리하는 매니저 클래스
	@date : 2012/7/17/
*/
class CX2BuffTempletManager
{
public:
	static bool GetCombination( KLuaManager& luaManager_, OUT vector<UINT>& vecType_ );

public:
	static CX2BuffTempletManager* GetInstance()
	{
		if ( NULL == pInstance )
			pInstance = new CX2BuffTempletManager;
		
		return pInstance;
	}

	static void DestroyInstance() { SAFE_DELETE( pInstance ); }

private:
	static CX2BuffTempletManager* pInstance;

public:
	void OpenScriptFileForTemplet();
	void OpenScriptFileForFactor();

	void AddBuffTemplet_LUA();
	void AddBuffFactor_LUA();

	CX2BuffTempletPtr GetBuffTempletPtr( const BUFF_TEMPLET_ID eBuffTempletID_ ) const;
	CX2BuffFactorPtr GetBuffFactorPtr( const UINT uiBuffFactorNumber_ ) const;

	void GetBuffFactorPtrFromBuffFactorList( IN KLuaManager& luaManager_, OUT vector<CX2BuffFactorPtr>& vecBuffFactorPtr_, OUT vector<UINT>& vecBuffFactorID_ );

private:
	CX2BuffTempletManager()
	{}

private:

	map<BUFF_TEMPLET_ID, CX2BuffTempletPtr>	m_mapBuffTempletPtr;
	vector<CX2BuffFactorPtr>				m_vecBuffFactorPtr;		/// 버프에 의해서 발생하는 버프 팩터용
	
};