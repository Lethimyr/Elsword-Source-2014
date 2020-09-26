
#include "FileBackup.h"

void main( int argc, const char** argv )
{
    KFileBackup kBackup;

    // set skipping file extension
    if( ! kBackup.ParseCmdParam( argc, argv ) )
        goto release;

    // make folder, set logfile, ...
    if( ! kBackup.Init() )
        goto release;

    // start backup
    kBackup.Backup();

release:
    KSingleton<KLogManager>::ReleaseInstance();
}