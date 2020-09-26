/** @file   KHelpSystem.h
	@brief  interface of KHelpSystem
	@author seojt@kogstudios.com
	@since  jintaeks on 2005-9-5, 16:01:53

    @date   jintaeks on 2007-1-31, 15:44
            - KSetCurrentHelp()
*/

#ifndef _KHELPSYSTEM_H
#define _KHELPSYSTEM_H

#include "AssertTrace.h"
#include <string.h>
#include <vector>
#include "KGlobalSingleton.h"


//------------------------------------------------------------------------------
// defines
//
#define DECLARE_PARTIAL_HELP_SYSTEM(type)\
    static char ms_szHelp##type[];\
    static char* GetHelpTypeName() { return ms_szHelp##type; }\
    static void type##HelpSystem(void* pParam)\
    {\
        type::StaticDisplayHelp( pParam );\
    }

#define DECLARE_HELP_SYSTEM(type)\
    DECLARE_PARTIAL_HELP_SYSTEM(type)\
    static void StaticDisplayHelp(void*);

#define IMPLEMENT_HELP_SYSTEM(type)\
    char type::ms_szHelp##type[] = #type;\
    struct _##type##HelpInstall\
    {\
        _##type##HelpInstall()\
        {\
            _KHelpSystem.AddHelpObject(\
                #type, type::type##HelpSystem );\
        }\
    };\
    static _##type##HelpInstall _s_##type##HelpInstall;\
    void type::StaticDisplayHelp(void*)


//------------------------------------------------------------------------------
/// @class  KHelpSystem
///
class KHelpSystem : public KGlobalSingleton<KHelpSystem>
{
    IMPLEMENT_KGLOBALSINGLETON( KHelpSystem )

#ifdef CONVERSION_VS
public:
#else CONVERSION_VS
private:
#endif CONVERSION_VS
    /// constructor.
    KHelpSystem();
    /// destructor.
    ~KHelpSystem();

public:
    typedef void (*HELP_SYSTEM_CALLBACK)(void* pParam);

    //------------------------------------------------------------------------------
    /// @struct     KHelpNode
    ///
    struct KHelpNode
    {
        char                    m_szHelpTypeName[256];
        HELP_SYSTEM_CALLBACK    m_fpCallback;
    };//struct KHelpNode

public:
    typedef std::vector<KHelpNode>              HELP_VECTOR;

private:
    HELP_VECTOR     m_vecHelps;     ///< help nodes
    int             m_iCursor;      ///< help node cursor
    bool            m_bShowHelp;    ///< show help system

private:
    void _Init();

public:
    /// safely delete resources.
    void Clear();

    /// add help node.
    bool AddHelpObject(const char* szHelpTypeName, HELP_SYSTEM_CALLBACK fpCallback);

    /// display current help.
    bool DisplayHelp(void* pParam=NULL);

    /// display object help.
    /// @param  pObject: target object which showing help display
    /// @param  szHelpTypeName: [in] class name
    ///         which has KHelpSystem implementation
    bool DisplayHelp(void* pObject, const char* szHelpTypeName, void* pParam=NULL);

    /// move to next help.
    void NextHelp();

    /// move to previous help.
    void PrevHelp();

    const char* CurrentHelpName();

    /// find and set current help.
    bool SetCurrentHelp(const char* pszHelpTypeName);

    bool    IsDisplayHelp() const { return m_bShowHelp; }
    void    EnableDisplayHelp(bool bEnable_) { m_bShowHelp = bEnable_; }

    //------------------------------------------------------------------------------
    // debug function(s)
    //
public:
    void Debug_Dump();
};//class KHelpSystem

#define _KHelpSystem    KHelpSystem::Singleton()

#endif // _KHELPSYSTEM_H


/** @example    KHelpSystem
    @code


    @endcode
*/
