
#include "registry.h"

bool KRegistry::OpenKey(HKEY hKey, char* szKey)
{   
	WCHAR wcharBuf[255] = {0,};
	MultiByteToWideChar( CP_ACP, 0, szKey, -1, wcharBuf, 255 );	

    if( RegOpenKeyEx((HKEY)hKey,
                     wcharBuf, 
                      0, 
                      KEY_ALL_ACCESS, 
                     &m_hKey) == ERROR_SUCCESS )
    {
        return true;
    }
    else
    {
        m_hKey = NULL;
        return false;
    } //else
}

bool KRegistry::CreateKey(HKEY hKey, char* szKey)
{   
	WCHAR wcharBuf[255] = {0,};
	MultiByteToWideChar( CP_ACP, 0, szKey, -1, wcharBuf, 255 );	

    if( RegCreateKeyEx(hKey,
                       wcharBuf, 
                        0, 
                        NULL, 
                        REG_OPTION_NON_VOLATILE, 
                        KEY_ALL_ACCESS, 
                        NULL, 
                       &m_hKey, 
                        NULL) == ERROR_SUCCESS )
    {
        return true;
    }
    else
    {
        m_hKey = NULL;
        return false;
    } // else
}

bool KRegistry::SetValue(char* lpValueName, char* lpData)
{
    ASSERT(m_hKey != NULL);

    DWORD dwType = REG_SZ;

	WCHAR wcharBuf[255] = {0,};
	WCHAR wcharBuf2[255] = {0,};
	MultiByteToWideChar( CP_ACP, 0, lpValueName, -1, wcharBuf, 255 );	
	MultiByteToWideChar( CP_ACP, 0, lpData, -1, wcharBuf2, 255 );	

    if( ::RegSetValueEx(m_hKey, 
                       wcharBuf, 
                        0, 
                        dwType, 
                        (BYTE*)wcharBuf2, 
                        (DWORD)wcslen(wcharBuf2)) == ERROR_SUCCESS )
    {
        ::RegFlushKey(m_hKey);
        return true;
    } // if

    return false;
}

bool KRegistry::SetValue(char* lpValueName, DWORD dwValue)
{
    ASSERT(m_hKey != NULL);
    
    DWORD dwType = REG_DWORD;

	WCHAR wcharBuf[255] = {0,};
	MultiByteToWideChar( CP_ACP, 0, lpValueName, -1, wcharBuf, 255 );	

    
    if( ::RegSetValueEx(m_hKey, 
                       wcharBuf, 
                        0, 
                        dwType, 
                       (LPBYTE)&dwValue, 
                        sizeof(DWORD)) == ERROR_SUCCESS )
    {
        ::RegFlushKey(m_hKey);
        return true;
    }
    return false;
}


bool KRegistry::GetValue(char* lpValueName, std::string& strValue)
{
    bool bRet = false;
    LPSTR lpstrValue;
    DWORD dwType = REG_SZ;
    DWORD lpcbData;
    
    bRet = false;   
    lpstrValue = (LPSTR)new char[256];
    memset(lpstrValue, 0, sizeof(char) * 256);
    lpcbData = 1024;
    
	WCHAR wcharBuf[255] = {0,};


	WCHAR wcharBuf2[255] = {0,};
	MultiByteToWideChar( CP_ACP, 0, lpValueName, -1, wcharBuf2, 255 );	

    if( ::RegQueryValueEx(m_hKey,
                          wcharBuf2,
                          NULL,
                         &dwType, 
                         (BYTE*)wcharBuf,
                         &lpcbData) == ERROR_SUCCESS )
    {
        bRet = true;
		char charBuf[255] = {0,};
		WideCharToMultiByte( CP_ACP, 0, wcharBuf, -1, charBuf, 255, NULL, NULL );
        strValue = (char*)charBuf;
    }
    else
    {
        strValue.empty();
    }
    
    SAFE_DELETE_ARRAY(lpstrValue);
    
    return bRet;
}   


bool KRegistry::GetValue(char* lpValueName, DWORD& dwValue)
{
    bool bRet = false;
    DWORD dwType = REG_DWORD;
    DWORD lpcbData = sizeof(DWORD);
    dwValue = 0;    
    
	WCHAR wcharBuf[255] = {0,};
	MultiByteToWideChar( CP_ACP, 0, lpValueName, -1, wcharBuf, 255 );	
        
    if(RegQueryValueEx(m_hKey,
                       wcharBuf,
                       NULL,
                      &dwType, 
                      (BYTE*)&dwValue,
                      &lpcbData) == ERROR_SUCCESS )
        bRet = true;

    return bRet;
}

bool KRegistry::DeleteKey(HKEY hKey, char* szKey)
{   
	WCHAR wcharBuf[255] = {0,};
	MultiByteToWideChar( CP_ACP, 0, szKey, -1, wcharBuf, 255 );	

    return ::RegDeleteKey(hKey,wcharBuf) == ERROR_SUCCESS;
}

bool KRegistry::DeleteValue(char* lpValueName)
{   
	WCHAR wcharBuf[255] = {0,};
	MultiByteToWideChar( CP_ACP, 0, lpValueName, -1, wcharBuf, 255 );	

    if( ::RegDeleteValue(m_hKey,wcharBuf) == ERROR_SUCCESS )
    {
        return true;
    }
    else
    {
        return false;
    }
    
}

void KRegistry::CloseKey()
{
    if( NULL != m_hKey )
    {
        ::RegCloseKey(m_hKey);
        m_hKey = NULL;
    } // if
}

bool KRegistry::SaveKey(char* lpszFileName)
{
    ASSERT(m_hKey != NULL);

	WCHAR wcharBuf[255] = {0,};
	MultiByteToWideChar( CP_ACP, 0, lpszFileName, -1, wcharBuf, 255 );	


    return ::RegSaveKey(m_hKey, wcharBuf, NULL) == ERROR_SUCCESS;
}

bool KRegistry::RestoreKey(char* lpszFileName, DWORD dwFlags)
{
    ASSERT(m_hKey != NULL);

	WCHAR wcharBuf[255] = {0,};
	MultiByteToWideChar( CP_ACP, 0, lpszFileName, -1, wcharBuf, 255 );	

    return ::RegRestoreKey(m_hKey, wcharBuf, dwFlags) == ERROR_SUCCESS;
}

bool KRegistry::LoadKey(enum Keys hKey, char* lpszSubKey, char* lpszFileName)
{
	WCHAR wcharBuf[255] = {0,};
	MultiByteToWideChar( CP_ACP, 0, lpszFileName, -1, wcharBuf, 255 );	

	WCHAR wcharBuf2[255] = {0,};
	MultiByteToWideChar( CP_ACP, 0, lpszSubKey, -1, wcharBuf2, 255 );	

    return ::RegLoadKey((HKEY)hKey, wcharBuf2, wcharBuf) == ERROR_SUCCESS;
}
