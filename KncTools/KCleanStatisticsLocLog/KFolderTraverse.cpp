#include <string>
#include <algorithm>

#include "KFolderTraverse.h"

//---------------------------------------------------------------------------
bool KIsEqualExt(char* pcSrc, char* pcDest)
{
	char* pcExt = strrchr(pcSrc,'.') + 1;
	if ((int) pcExt == 1)
		return false;

	std::string strExt = pcExt;
	std::transform(strExt.begin(), strExt.end(), strExt.begin(), tolower);

	return strExt == pcDest;
}
//---------------------------------------------------------------------------
void KSetCurFolder(const char* pcName)
{
    SetCurrentDirectory(pcName);

    //char acCurPath[_MAX_PATH] = {0};
    //GetCurrentDirectory(_MAX_PATH, acCurPath);
    //cout << acCurPath << endl;
}
//---------------------------------------------------------------------------
std::string KGetCurFolder()
{
	char acCurPath[_MAX_PATH] = {0};
	GetCurrentDirectory(_MAX_PATH, acCurPath);

	return acCurPath;
}
//---------------------------------------------------------------------------
std::string KMakeAbsolutePath(const char* pcFile)
{
    char acCurPath[_MAX_PATH] = {0};
    GetCurrentDirectory(_MAX_PATH, acCurPath);

    std::ostringstream os;
    os << acCurPath << "\\" << pcFile;
    return os.str();
}
//---------------------------------------------------------------------------
void KDeleteFolder(const char* strPath)
{
    // 절대 경로로 변환
    KSetCurFolder(strPath);
    char acCurPath[_MAX_PATH] = {0};
    GetCurrentDirectory(_MAX_PATH, acCurPath);

    // 재귀적 폴더/파일 삭제
    WIN32_FIND_DATA	wfd;
    HANDLE hFile = FindFirstFile("*.*", &wfd);

    while (FindNextFile(hFile, &wfd))
    {
        if (strcmp(wfd.cFileName, "..") == 0 || strcmp(wfd.cFileName, ".") == 0)
            continue;

        if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            KDeleteFolder(wfd.cFileName);
        }
        else
        {
            DeleteFile(KMakeAbsolutePath(wfd.cFileName).c_str());
        } 
    }

    if(hFile != INVALID_HANDLE_VALUE)
        FindClose(hFile);

    KSetCurFolder("..");
    RemoveDirectory(acCurPath);
}
//---------------------------------------------------------------------------