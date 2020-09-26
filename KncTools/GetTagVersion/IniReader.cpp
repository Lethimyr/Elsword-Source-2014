#include "stdafx.h"
#include "IniReader.h"
#include <iostream>
#include <Windows.h>
#include <cctype>
#include <algorithm>
CIniReader::CIniReader(const char* szFileName)
{ 
    m_strFileName = szFileName;
}

int CIniReader::ReadInteger(char* szSection, char* szKey, int iDefaultValue)
{ 
    int iResult = GetPrivateProfileInt(szSection,  szKey, iDefaultValue, m_strFileName.c_str());  
    return iResult;
}

float CIniReader::ReadFloat(char* szSection, char* szKey, float fltDefaultValue)
{ 
    char szResult[255]; 
    char szDefault[255]; 
    float fltResult; 
    sprintf(szDefault, "%f",fltDefaultValue); 
    GetPrivateProfileString(szSection,  szKey, szDefault, szResult, 255, m_strFileName.c_str());  
    fltResult =  (float)atof(szResult); 
    return fltResult;
}

bool CIniReader::ReadBoolean(char* szSection, char* szKey, bool bolDefaultValue)
{ 
    char szResult[255]; 
    std::string strDefault;
    strDefault = bolDefaultValue? "true" : "false";
    GetPrivateProfileString(szSection, szKey, strDefault.c_str(), szResult, 255, m_strFileName.c_str());
    
    std::string strResult = szResult;
    std::transform( strResult.begin(), strResult.end(), strResult.begin(), std::tolower);
    
    return (strResult == "true");
}

std::string CIniReader::ReadString(char* szSection, char* szKey, const char* szDefaultValue)
{ 
    char szResult[255];
    memset(szResult, 0x00, 255);
    GetPrivateProfileString(szSection,  szKey, szDefaultValue, szResult, 255, m_strFileName.c_str());  
    std::string strResult = szResult;
    return strResult;
}

