/** @file   KDynamicSingleton.h
	@brief  interface of KDynamicSingleton
	@author seojt@kogstudios.com
	@since  2003-12-08 ¿ÀÈÄ 6:06:27
*/

#ifndef _KDYNAMICSINGLETON_H
#define _KDYNAMICSINGLETON_H

#include <assert.h>

//------------------------------------------------------------------------------
/// @class  KDynamicSingleton<>
template<typename T>
class KDynamicSingleton
{
protected:
    static T*       ms_singleton;

public:
    /// constructor.
	KDynamicSingleton()
    {
        assert( NULL == ms_singleton );
    }//KDynamicSingleton()

	virtual ~KDynamicSingleton()
    {
        assert( ms_singleton );
        DestroySingleton();
    }//KDynamicSingleton()

    static T& GetInstance()
    {
        if ( ms_singleton == NULL )
            ms_singleton = new T();

        return *ms_singleton;
    }//CreateSingleton()

    static void DestroySingleton()
    {
        if ( ms_singleton )
        {
            ::delete ms_singleton;
            ms_singleton = NULL;
        }//if
    }//DestroySingleton()
};//class KDynamicSingleton

template<typename T> T* KDynamicSingleton<T>::ms_singleton = NULL;

#endif // _KDYNAMICSINGLETON_H
