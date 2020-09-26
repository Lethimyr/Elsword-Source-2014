#pragma once
#include "zip.h"
#include "StringEx.h"
class IElement;

class CZipper
{
public:
    CZipper(void);
    ~CZipper(void);

    bool CreateZipFile( char* strFileName );
    bool AddFile( IElement* pElement ); // file name 
    void Close();
    char* GetZipFileName();

private:
    HZIP m_hz;
    CStringEx m_strFileName;
};
