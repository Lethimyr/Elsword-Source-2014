#ifndef INIREADER_H
#define INIREADER_H
#include <string>

class CIniReader
{
public:
    CIniReader( const char* szFileName);
    CIniReader() {};
    CIniReader( const CIniReader& right ) { *this = right; }
    CIniReader& operator=(const CIniReader& right) { m_strFileName = right.m_strFileName; }
    int ReadInteger(char* szSection, char* szKey, int iDefaultValue);
    float ReadFloat(char* szSection, char* szKey, float fltDefaultValue);
    bool ReadBoolean(char* szSection, char* szKey, bool bolDefaultValue);
    std::string ReadString(char* szSection, char* szKey, const char* szDefaultValue);
private:
    std::string m_strFileName;
};
#endif//INIREADER_H

