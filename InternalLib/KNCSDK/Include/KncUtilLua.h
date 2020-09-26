#pragma once

#ifndef IN
#define IN
#endif
#ifndef OUT
#define OUT
#endif

#define BEGIN_NAMESPACE( name ) namespace name {
#define END_NAMESPACE }

#include <windows.h>

#define LUABIND_NO_RTTI
#include "luabind/luabind.hpp"
#include "luabind/object.hpp"   // luabind::object

BEGIN_NAMESPACE( KncUtil )

// lua table에서 값을 얻어낸다. 얻어내지 못할 경우 default 값을 반환한다.
template< class T, class Y >
Y GetLuaData( IN const luabind::object& obj, IN const T& tKey, IN const Y& yDefault )
{
    try
    {
        return luabind::object_cast<Y>( obj[ tKey ] );
    }
    catch( ... )
    {
        return yDefault;
    }
}

// table의 값들을 모두 벡터에 담는다. 테이블이 아닌 경우도 element로 삽입시도한다.
template< class T >
bool Table2vector( IN const luabind::object& obj, OUT std::vector<T>& vecOut )
{
    if( obj.type() != LUA_TTABLE ) 
    {
        vecOut.push_back( luabind::object_cast<T>( obj ) );
        return true;
    }

    luabind::object::iterator oit;
    for( oit = obj.begin(); oit != obj.end(); ++oit )
    {
        T tElement = luabind::object_cast<T>( *oit );
        vecOut.push_back( tElement );
    }

    return true;
}

// table 내에 들어있는 값을 map에 담는다.
template< class T, class Y >
bool Table2map( IN const luabind::object& obj, OUT std::map<T,Y>& mapOut )
{
    if( obj.type() != LUA_TTABLE ) return false;

    luabind::object::iterator oit;
    for( oit = obj.begin(); oit != obj.end(); ++oit )
    {
        mapOut.insert( std::make_pair( 
            luabind::object_cast<T>( oit.key() ),
            luabind::object_cast<Y>( *oit ) ) );
    }

    return true;
}

// table 내에 들어있는 값을 pair에 담는다. 값을 읽지 못하면 기존의 값을 건드리지 않는다.
template< class T, class Y >
bool Table2pair( IN const luabind::object& obj, OUT std::pair<T,Y>& pairOut )
{
    if( obj.type() != LUA_TTABLE ) return false;

    std::pair<T,Y> pairTemp;

    try
    {
        pairTemp.first  = luabind::object_cast<T>( obj[ 1 ] );
        pairTemp.second = luabind::object_cast<Y>( obj[ 2 ] );
    }
    catch( ... )
    {
        return false;
    }

    pairOut.swap( pairTemp );

    return true;
}

END_NAMESPACE