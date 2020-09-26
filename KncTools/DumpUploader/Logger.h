#pragma once
#include "StringEx.h"
class CLogger
{
public:
    CLogger(void);
    ~CLogger(void);

    void AddLog( char* strLogString );
    char* GetLogString();
private:
    CStringEx m_strLog;
};
