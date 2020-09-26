/** @file   KMessageConsole.h
	@brief  interface of KMessageConsole
	@author seojt@kogstudios.com
	@since  2005-5-30, 15:30:57

    @date   seojt on 2005-6-14, 14:10:14
            - we added static OutText() functions.
    @date   seojt on 2005-7-15, 11:00:37
            - example added
    @date   robobeg on 2006-9-5, 09:36:00
            - internal msg type is changed from std:string to std:wstring
*/
#pragma once

#ifndef _KMESSAGECONSOLE_H
#define _KMESSAGECONSOLE_H

//#include <d3dx9.h>
#pragma warning(push)
#pragma warning(disable: 4995)
//#include <list>
//#include <string>


//------------------------------------------------------------------------------
/// @class  KMsgConsoleNode
///
class KMsgConsoleNode
{
public:
    D3DXCOLOR       m_dxColor;
//{{ robobeg : 2006-09-05 // 10:13
    //std::string     m_strMsg;
    std::wstring    m_wstrMsg;
//}} robobeg : 2006-09-05 // 10:13

public:
    /// constructor.
    KMsgConsoleNode()
    {
        m_dxColor = D3DXCOLOR(1,1,1,1);
//{{ robobeg : 2006-09-05 // 10:13
        //m_strMsg  = "";
        m_wstrMsg = L"";
//}} robobeg : 2006-09-05 // 10:13
    }//KMsgConsoleNode()

    /// copy constructor.
    KMsgConsoleNode(const KMsgConsoleNode& _second)
    {
        m_dxColor = _second.m_dxColor;
//{{ robobeg : 2006-09-05 // 10:13
        //m_strMsg  = _second.m_strMsg;
        m_wstrMsg = _second.m_wstrMsg;;
//}} robobeg : 2006-09-05 // 10:13
    }//KMsgConsoleNode()

    KMsgConsoleNode& operator=(KMsgConsoleNode& _second)
    {
        m_dxColor = _second.m_dxColor;
//{{ robobeg : 2006-09-05 // 10:13
        //m_strMsg  = _second.m_strMsg;
        m_wstrMsg = _second.m_wstrMsg;
//}} robobeg : 2006-09-05 // 10:13
        return *this;
    }//operator=()
};//class KMsgConsoleNode


//------------------------------------------------------------------------------
/// @class  KMessageConsole
///
class KMessageConsole
{
public:
    /// render a text.
    /// @param  _x: screen x-pos
    /// @param  _y: screen y-pos
    /// @param  _pszText: [in] text to render
    /// @param  _dxColor: text color
    static void OutText(float _x, float _y, const char* _pszText, D3DXCOLOR _dxColor);
    static void OutText(float _x, float _y, D3DXCOLOR _dxColor, const char* _pszFormat, ...);
//{{ robobeg : 2006-09-04 // 20:57
    static void OutText(float _x, float _y, const wchar_t* _pszText, D3DXCOLOR _dxColor);
    static void OutText(float _x, float _y, D3DXCOLOR _dxColor, const wchar_t* _pszFormat, ...);
//}} robobeg : 2006-09-04 // 20:57

private:
    typedef std::list<KMsgConsoleNode>              STRLIST;
    typedef std::list<KMsgConsoleNode>::iterator    STRLIST_ITOR;

private:
    enum _KMC_STATES
    {
        KMC_STATE_NORMAL,   ///< normal displaying state
        KMC_STATE_FADEOUT,  ///< fade out state

        KMC_STATE_MAX
    };//enum _KMC_STATES

private:
    STRLIST         m_listStr;          ///< string container
    float           m_fStateTimer;      ///< state timer
    int             m_iMaxNumOfMessage; ///< number of maximum messages can be displayed
    _KMC_STATES     m_eState;           ///< message console state
    float           m_fFadeOutTime;     ///< fade out time
    float           m_fLineShowTime;    ///< a line display time
    float           m_fTextHeight;      ///< text height
    float           m_fLineSpaceRatio;  ///< inter text-line height ratio

public:
    /// constructor.
    KMessageConsole();
    /// destructor.
    virtual ~KMessageConsole();

private:
    void _Init();

public:
    /// safely delete all resources.
    void SafeDelete();

    /// add fixed size message.
    /// @param  _pszMsg: [in] message to render
    /// @param  _dwColor: message color
    void AddMessage(const char* _pszMsg, DWORD _dwColor);

    /// add variable argument message.
    /// @param  _dwColor: text color
    /// @param  _pszFormat: variable argument format string
    void AddMessage(DWORD _dwColor, const char* _pszFormat, ...);

//{{ robobeg : 2006-09-05 // 10:13

    /// add fixed size message.
    /// @param  _pszMsg: [in] message to render
    /// @param  _dwColor: message color
    void AddMessage(const wchar_t* _pwszMsg, DWORD _dwColor);

    /// add variable argument message.
    /// @param  _dwColor: text color
    /// @param  _pwszFormat: variable argument format string
    void AddMessage(DWORD _dwColor, const wchar_t* _pwszFormat, ...);

//}} robobeg : 2006-09-05 // 10:13

    /// remove a message from the head of the list.
    BOOL RemoveAMessageAtHead();

    /// set maximum number of message.
    /// @param  _iNumMaxMessage: maximum number of message
    void SetMaxNumOfMessages(int _iNumMaxMessage);

    /// OnRender() handler.
    /// @param  _x: left position of first message in the list.
    /// @param  _y: top position of first message in the list.
    /// @param  _fElapsedTime: inter frame elapsed time
    void OnRender(float _x, float _y, float _fElapsedTime);

    //------------------------------------------------------------------------------
    // writers
    //
public:
    void SetMaxNumOfMessage(int _iMaxNumOfMessage) { m_iMaxNumOfMessage = _iMaxNumOfMessage; }
    void SetFadeOutTime(float _fFadeOutTime) { m_fFadeOutTime = _fFadeOutTime; }
    void SetLineShowTime(float _fLineShowTime) { m_fLineShowTime = _fLineShowTime; }
    void SetTextHeight(float _fTextHeight) { m_fTextHeight = _fTextHeight; }
    void SetLineSpaceRatio(float _fLineSpaceRatio) { m_fLineSpaceRatio = _fLineSpaceRatio; }
};//class KMessageConsole

#pragma warning(pop)

#endif // _KMESSAGECONSOLE_H


/** @example    KMessageConsole
    @code

  1) create an instance:

    KMessageConsole             g_msgConsole;


  2) OnResetDevice():

    g_msgConsole.SetMaxNumOfMessages( 30 );


  3) OnRender():

        g_msgConsole.OnRender( 100, 100, g_fElapsedTime );


  4) KeyboardProc():

    if ( bKeyDown )
    {
        switch( nChar )
        {
        case '1':
            g_msgConsole.AddMessage( 0xffffffff, "hello world" );
            break;
        }//switch
    }//if

    @endcode
*/
