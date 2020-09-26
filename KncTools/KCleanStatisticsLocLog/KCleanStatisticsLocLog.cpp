#include "KFolderTraverse.h"

#include <windows.h>
#include <stdio.h>

#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <vector>
using namespace std;

std::vector<std::string> g_vecRemoveFolder;

//---------------------------------------------------------------------------
void LoadRemoveFolder()
{
}
//---------------------------------------------------------------------------
bool IsEquallower(char* pcSrc, const char* pcDest)
{
    std::string strComp = pcSrc;
    std::transform(strComp.begin(), strComp.end(), strComp.begin(), tolower);

    return strComp == pcDest;
}
//---------------------------------------------------------------------------
bool CheckRemoveFolder(char* pcName)
{
    unsigned int iF;
    for (iF = 0; iF < g_vecRemoveFolder.size(); ++iF)
    {
        if (IsEquallower(pcName, g_vecRemoveFolder[iF].c_str()))
            return true;
    }

    return false;
}
//---------------------------------------------------------------------------
bool IsEqualExt(char* pcSrc, char* pcDest)
{
    //strrchr(pcSrc,'.');
    char* pcExt = strrchr(pcSrc,'.') + 1;
    if ((int) pcExt == 1)
        return false;

    std::string strExt = pcExt;
    std::transform(strExt.begin(), strExt.end(), strExt.begin(), tolower);

    return strExt == pcDest;
}
//---------------------------------------------------------------------------
void _DeleteFolder(WIN32_FIND_DATA& wfd)
{
    if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
    {
        if (CheckRemoveFolder(wfd.cFileName))
        {
            KDeleteFolder(wfd.cFileName);
        }
    }
}
//---------------------------------------------------------------------------
void _DeleteFile(WIN32_FIND_DATA& wfd)
{
    if (!(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
    {
		std::string strFilename = wfd.cFileName;
		if ( strFilename.find( "SI_LOG_DUNGEON" ) != std::string::npos &&
			IsEqualExt( wfd.cFileName, "txt" ) == true )
		{
			DeleteFile(KMakeAbsolutePath(wfd.cFileName).c_str());
		}
    }
}
//---------------------------------------------------------------------------
void main()
{
    LoadRemoveFolder();

    KApplyAllFiles(_DeleteFile);

    //KApplyAllFiles(_DeleteFolder);
}
//---------------------------------------------------------------------------