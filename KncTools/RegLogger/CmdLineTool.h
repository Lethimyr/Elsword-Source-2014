#pragma once

#include <boost/function.hpp>
#include <vector>

struct KCommand
{
    KCommand( const char*, bool, const char*, boost::function<bool (const char*) >& );

    std::string                             m_strCmd;
    bool                                    m_bHaveMoreParam;
    std::string                             m_strHelpText;
    boost::function<bool (const char*) >    m_fHandler;
};

class KCmdLineTool
{
public:
    KCmdLineTool(void);
    virtual ~KCmdLineTool(void);

    void PrintHelp();

    virtual bool ParseCmdParam( const int argc, const char** argv );
    virtual bool Init() { return true; }
    virtual bool Run() = 0;
    virtual void ShutDown() {}

protected:
    std::vector<KCommand>   m_vecCmd;
    std::string             m_strAppname;
    std::string             m_strComment;
};

#define DECL_ON_FUNC( param )       bool ON_##param( const char* szParam )
#define IMPL_ON_FUNC( param )       bool ON_##param( const char* szParam_ )
#define ADD_CMD( param, use_next )  m_vecCmd.push_back( KCommand( #param, use_next, stm.str().c_str(), \
                                        boost::function<bool (const char*)>( ON_##param ) ) ); \
                                        stm.str("")

