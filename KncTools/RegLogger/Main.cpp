#include "RegLogger.h"

void main( int argc, const char** argv )
{
    KCmdLineTool* pkTool = new KRegLogger;

    if( ! pkTool->ParseCmdParam( argc, argv ) )
    {
        pkTool->PrintHelp();
        goto idx_release;
    }

    if( ! pkTool->Init() )
    {
        pkTool->PrintHelp();
        goto idx_release;
    }

    pkTool->Run();


idx_release:

    pkTool->ShutDown();

    delete pkTool;
}