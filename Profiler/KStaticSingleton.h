/** @file   KStaticSingleton.h
	@brief  interface of KStaticSingleton
	@author seojt@kogstudios.com
	@since  2003-12-08 ¿ÀÈÄ 6:06:27
*/
#pragma once

#ifndef _KSTATICSINGLETON_H
#define _KSTATICSINGLETON_H


//------------------------------------------------------------------------------
/// @class  KStaticSingleton<>
///
template<typename T>
class KStaticSingleton
{
public:
    static T& GetSingleton()
    {
        static T s_singleton;
        return s_singleton;
    }//GetSingletonPtr()
};//class KStaticSingleton

#endif // _KSTATICSINGLETON_H


/** @example    KStaticSingleton<>
    @code

    class KHelpSystem : public KStaticSingleton<KHelpSystem>
    {
    public:
        void AddHelpObject(int, char*);
    };//class KHelpSystem

    #define KAddHelpSystem(type)   KStaticSingleton<KHelpSystem>::GetSingleton().\
    AddHelpObject( #type, type::type##HelpSystem )

    @endcode
*/
