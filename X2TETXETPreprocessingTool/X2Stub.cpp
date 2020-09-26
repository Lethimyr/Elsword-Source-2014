#include <StdAfx.h>



CKTDXApp::CKTDXApp()
{
    m_pLuaBinder = new KLuabinder;

	//D3D Å¬·¡½º
	lua_State* L = m_pLuaBinder->GetLuaState();

	lua_tinker::class_add<D3DXVECTOR2>( L, "D3DXVECTOR2" );
	lua_tinker::class_con<D3DXVECTOR2>( L, lua_tinker::constructor<float, float>() );
	lua_tinker::class_mem<D3DXVECTOR2>( L, "x", &D3DXVECTOR2::x );
	lua_tinker::class_mem<D3DXVECTOR2>( L, "y", &D3DXVECTOR2::y );

	lua_tinker::class_add<D3DXVECTOR3>( L, "D3DXVECTOR3" );
	lua_tinker::class_con<D3DXVECTOR3>( L, lua_tinker::constructor<float, float, float>() );
	lua_tinker::class_mem<D3DXVECTOR3>( L, "x", &D3DXVECTOR3::x );
	lua_tinker::class_mem<D3DXVECTOR3>( L, "y", &D3DXVECTOR3::y );
	lua_tinker::class_mem<D3DXVECTOR3>( L, "z", &D3DXVECTOR3::z );

	lua_tinker::class_add<D3DXCOLOR>( L, "D3DXCOLOR" );
	lua_tinker::class_con<D3DXCOLOR>( L, lua_tinker::constructor<float,float,float,float>() );
	lua_tinker::class_mem<D3DXCOLOR>( L, "a", &D3DXCOLOR::a);
	lua_tinker::class_mem<D3DXCOLOR>( L, "r", &D3DXCOLOR::r);
	lua_tinker::class_mem<D3DXCOLOR>( L, "g", &D3DXCOLOR::g);
	lua_tinker::class_mem<D3DXCOLOR>( L, "b", &D3DXCOLOR::b);

    CKTDXDeviceTET_Preprocessing::RegisterLuabind( L );
    CKTDXDeviceXET_Preprocessing::RegisterLuabind( L );

}


CKTDXApp::~CKTDXApp()
{
    delete  m_pLuaBinder;
}