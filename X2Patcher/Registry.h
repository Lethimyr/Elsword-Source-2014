#ifndef REGISTRY_H
#define REGISTRY_H

#include "windows.h"
#include <string>
#include "KAssert.h"

class KRegistry
{
public:
    enum cregRestoreEnum
    {
        regVolatile = REG_WHOLE_HIVE_VOLATILE,
        regRefresh = REG_REFRESH_HIVE,
        regNoLazy = REG_NO_LAZY_FLUSH
    };



    KRegistry()
    {
        m_hKey = NULL;
    };

    ~KRegistry()
    {
        CloseKey();
    };

    bool OpenKey(HKEY hKey, char* szKey);
    bool CreateKey(HKEY hKey, char* szKey);
    bool DeleteKey(HKEY hKey, char* szKey);
    bool DeleteValue(char* lpValueName);
    bool GetValue(char* lpValueName, std::string& strValue);
    bool GetValue(char* lpValueName, DWORD& dwValue);
    bool SetValue(char* lpValueName, char* lpData);
    bool SetValue(char* lpValueName, DWORD dwValue);
    bool SaveKey(char* lpszFileName);
    bool RestoreKey(char* lpszFileName, DWORD dwFlag);
    bool LoadKey(enum Keys hKey, char* lpszSubKey, char* lpszFileName);
    void CloseKey();    

protected:
    HKEY m_hKey;
};

#endif