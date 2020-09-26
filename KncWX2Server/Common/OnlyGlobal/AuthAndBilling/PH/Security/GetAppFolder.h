#ifndef _D_GETAPPFOLDER_H__
#define _D_GETAPPFOLDER_H__

inline string_t GetAppFolder()
{
	char szPath[MAX_PATH] = {0};
	GetModuleFileNameA(NULL, szPath, MAX_PATH);
	string_t strPath = szPath;
	int index = strPath.find_last_of("\\");
	if(index > 0)
	{
		return strPath.substr(0, index+1);
	}
	else
	{
		return "";
	}
}

inline string_t GetAppName()
{
	char szPath[MAX_PATH] = {0};
	GetModuleFileNameA(NULL, szPath, MAX_PATH);
	string_t strPath = szPath;
	int index = strPath.find_last_of("\\");
	if(index > 0)
	{
		return strPath.substr(index+1);
	}
	else
	{
		return strPath;
	}
}

#endif