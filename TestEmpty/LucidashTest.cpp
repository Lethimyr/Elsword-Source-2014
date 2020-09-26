#include "dxstdafx.h"
#include ".\LucidashTest.h"



CLucidashTest::CLucidashTest(void)
{
	if( g_pKTDXApp->GetDeviceManager()->LoadLuaTinker(L"test.lua") == false )
	{
		::MessageBox(g_pKTDXApp->GetHWND(), L"test.lua parsing fail.!", L"waring", MB_OK);
	}
	
	lua_State* L= g_pKTDXApp->GetLuaBinder()->GetLuaState();
}

CLucidashTest::~CLucidashTest(void)
{
}

HRESULT CLucidashTest::OnFrameMove( double fTime, float fElapsedTime )
{

	return S_OK;
}

HRESULT CLucidashTest::OnFrameRender()
{

	return S_OK;
}

HRESULT CLucidashTest::OnResetDevice()
{
	return S_OK;
}

HRESULT CLucidashTest::OnLostDevice()
{
	return S_OK;
}

bool CLucidashTest::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if( uMsg != NULL )
	{
		vector< int > Datas;

		KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState(), 0, false );

		if( luaManager.BeginTable( "test_table" ) == true )
		{
			string VarName = "data1";
			int LuaData=0;	

			LUA_GET_VALUE( luaManager , (WCHAR*)VarName.c_str() , LuaData , -1 );

			Datas.push_back( LuaData );


			luaManager.EndTable(); // Stage list
		}
	}
	return false;
}