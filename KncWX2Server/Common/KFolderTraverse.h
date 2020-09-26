#pragma once

#include <windows.h>

#include <sstream>
#include <string>

std::wstring KGetCurFolder();
void KSetCurFolder(WCHAR* pcName);
std::wstring KMakeAbsolutePath(WCHAR* pcFile);
void KDeleteFolder(WCHAR* strPath);

//---------------------------------------------------------------------------
template <class TFunc>
void KApplyAllFiles(TFunc tFunc)
{
    WCHAR acCurPath[_MAX_PATH] = {0};
    GetCurrentDirectory(_MAX_PATH, acCurPath);

    WIN32_FIND_DATA	wfd;
    HANDLE hFile = FindFirstFile(L"*.*", &wfd);

    while (FindNextFile(hFile, &wfd))
    {
        if (wcscmp(wfd.cFileName, L"..") == 0 ||
            wcscmp(wfd.cFileName, L".") == 0)
            continue;

        if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            KSetCurFolder(wfd.cFileName);
            KApplyAllFiles(tFunc);
            KSetCurFolder(L"..");
        }

        tFunc(wfd);
    }

    if(hFile != INVALID_HANDLE_VALUE)
        FindClose(hFile);
}
//---------------------------------------------------------------------------