#include <KncUtil.h>
#include "CmdLineTool.h"
#include <iostream>

KCommand::KCommand( const char* szCmd, 
                    bool bHaveMoreParam,
                    const char* szHelpText,
                    boost::function<bool (const char*) >& fHandler ) 
:
    m_strCmd( szCmd ), 
    m_bHaveMoreParam( bHaveMoreParam ), 
    m_strHelpText( szHelpText ), 
    m_fHandler( fHandler ) 
{

}


KCmdLineTool::KCmdLineTool(void)
{
    KncUtil::GetAppName( m_strAppname );
}

KCmdLineTool::~KCmdLineTool(void)
{
}

#include <iomanip>

std::ostream& operator<<( std::ostream& stm, const KCommand& kCmd )
{
    return stm << " /" << std::setiosflags( std::ios::left ) << std::setw( 7 ) << kCmd.m_strCmd 
               << " : " << kCmd.m_strHelpText << std::endl;
}

void KCmdLineTool::PrintHelp()
{
    std::cout << "\n" << m_strAppname << " : " << m_strComment << std::endl;

    std::copy( m_vecCmd.begin(), m_vecCmd.end(), std::ostream_iterator<KCommand>( std::cout ) );
}

bool KCmdLineTool::ParseCmdParam( const int argc, const char** argv )
{
    if( argc < 2 )  
        return false;

    for( int i = 1; i < argc; i++ )
    {
        if( argv[i][0] != '/' && argv[i][0] != '-') 
            return false;

        std::vector<KCommand>::iterator vit;
        for( vit = m_vecCmd.begin(); vit != m_vecCmd.end(); vit++ )
        {
            if( vit->m_strCmd.compare( &argv[i][1] ) == 0 )
            {
                if( vit->m_bHaveMoreParam )
                {
                    i = i + 1;
                    if( i >= argc ) 
                        return false;
                }
                
                if( ! vit->m_fHandler( argv[i] ) )  
                    return false;
            }
        }
    }

    return true;
}