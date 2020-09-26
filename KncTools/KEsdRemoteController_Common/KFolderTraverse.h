#pragma once

#include <windows.h>

#include <sstream>
#include <string>

bool KIsEqualExt(char* pcSrc, char* pcDest);
std::string KGetCurFolder();
void KSetCurFolder(const char* pcName);
std::string KMakeAbsolutePath(const char* pcFile);
void KDeleteFolder(const char* strPath);

//---------------------------------------------------------------------------
template <class TFunc>
void KApplyAllFiles(TFunc tFunc)
{
    char acCurPath[_MAX_PATH] = {0};
    GetCurrentDirectory(_MAX_PATH, acCurPath);

    WIN32_FIND_DATA	wfd;
    HANDLE hFile = FindFirstFile("*.*", &wfd);

    while (FindNextFile(hFile, &wfd))
    {
        if (strcmp(wfd.cFileName, "..") == 0 ||
            strcmp(wfd.cFileName, ".") == 0)
            continue;

        if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            KSetCurFolder(wfd.cFileName);
            KApplyAllFiles(tFunc);
            KSetCurFolder("..");
        }

        tFunc(wfd);
    }

    if(hFile != INVALID_HANDLE_VALUE)
        FindClose(hFile);
}
//---------------------------------------------------------------------------