#include "StdAfx.h"
#include ".\x2itemtoolcommon.h"


CX2ItemToolCommon* g_pItemToolCommon = NULL;


CX2ItemToolCommon::CX2ItemToolCommon(void)
{
	g_pItemToolCommon = this;
	
	m_MassFileManager.SetSubDirectory();

	RegisterLuabind();
	m_ItemManager.OpenScriptFile( L"enum.lua" );
}

CX2ItemToolCommon::~CX2ItemToolCommon(void)
{
}

void CX2ItemToolCommon::RegisterLuabind()
{
	m_LuaBinder.Destroy();
	m_LuaBinder.Initialize();
	m_LuaBinder.SetLuaTinker();


	lua_tinker::class_<CX2ItemManager>("CX2ItemManager")
		.def("AddItemTemplet",			CX2ItemManager::AddItemTemplet_LUA)
		;


}
